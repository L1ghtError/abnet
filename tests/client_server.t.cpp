#include <gtest/gtest.h>

#include "abnet/abnet.hpp"
#include "test_util.hpp"

#include <mutex>
#include <thread>

class ClientServerT : public ::testing::Test {
public:
  void SetUp() override {
    serv_sock = abnet::invalid_socket;
    client_sock = abnet::invalid_socket;
    const char srv_addr[] = "127.0.0.1";
    abnet::error_code ec;
    abnet::sockaddr_in4_type sa;
    sa.sin_family = AF_INET;
    sa.sin_port = 0;
    abnet::socket_ops::inet_pton(ABNET_OS_DEF(AF_INET), srv_addr, &sa.sin_addr, 0, ec);
    ASSERT_EQ(ec.value(), 0) << ERRMSG("inet_pton failed with error: ") << ec.message();

    // Preserve sa and link to s_info
    std::memset(&s_info, 0, sizeof(s_info));
    s_info.ai_family = sa.sin_family;
    s_info.ai_socktype = ABNET_OS_DEF(SOCK_STREAM);
    s_info.ai_protocol = ABNET_OS_DEF(IPPROTO_TCP);
    s_info.ai_addr = reinterpret_cast<abnet::socket_addr_type *>(&sa);
    s_info.ai_addrlen = sizeof(sa);

    serv_sock = abnet::socket_ops::socket(sa.sin_family, ABNET_OS_DEF(SOCK_STREAM), ABNET_OS_DEF(IPPROTO_TCP), ec);
    ASSERT_EQ(ec.value(), 0) << ERRMSG("socket failed with error: ") << ec.message();

    client_sock = abnet::socket_ops::socket(sa.sin_family, ABNET_OS_DEF(SOCK_STREAM), ABNET_OS_DEF(IPPROTO_TCP), ec);
    ASSERT_EQ(ec.value(), 0) << ERRMSG("socket failed with error: ") << ec.message();

    int so_reuseaddr = 1;
    abnet::socket_ops::setsockopt(serv_sock, 0, ABNET_OS_DEF(SOL_SOCKET), ABNET_OS_DEF(SO_REUSEADDR), &so_reuseaddr,
                                  sizeof(so_reuseaddr), ec);
    ASSERT_EQ(ec.value(), 0) << ERRMSG("setsockopt failed with error: ") << ec.message();
    abnet::socket_ops::bind(serv_sock, &sa, sizeof(sa), ec);
    ASSERT_EQ(ec.value(), 0) << ERRMSG("bind failed with error: ") << ec.message();

    size_t len = sizeof(sa);
    abnet::socket_ops::getsockname(serv_sock, reinterpret_cast<abnet::socket_addr_type *>(&sa), &len, ec);
    ASSERT_EQ(ec.value(), 0) << ERRMSG("getsockname failed with error: ") << ec.message();
    s_port = ntohs(sa.sin_port); // Store the port number
    std::memcpy(&s_storage, &sa, sizeof(sa));
  };
  void TearDown() override {
    abnet::error_code ec;
    if (serv_sock != abnet::invalid_socket)
      abnet::socket_ops::close(serv_sock, 0, 0, ec);
    if (client_sock != abnet::invalid_socket)
      abnet::socket_ops::close(client_sock, 0, 0, ec);
  };

protected:
  abnet::sockaddr_storage_type s_storage;
  abnet::sockaddr_storage_type c_storage;

  abnet::addrinfo_type s_info;
  abnet::addrinfo_type c_info;

  abnet::socket_type serv_sock;
  abnet::socket_type client_sock;

  uint16_t s_port;
};

TEST_F(ClientServerT, client_server) {
  abnet::error_code ec;
  std::mutex srv_ready_mutex;
  std::condition_variable srv_ready_cv;
  bool server_ready = false;
  auto var = this->s_info;
  std::thread server_thread([=, &ec, &server_ready, &srv_ready_mutex, &srv_ready_cv]() {
    abnet::socket_ops::listen(serv_sock, 5, ec);
    ASSERT_EQ(ec.value(), 0) << ERRMSG("listen failed with error: ") << ec.message();

    // Notify that the server is ready
    {
      std::lock_guard<std::mutex> lock(srv_ready_mutex);
      server_ready = true;
    }
    srv_ready_cv.notify_one();

    abnet::sockaddr_in4_type connected_addr;
    size_t client_addr_len = sizeof(connected_addr);
    abnet::socket_type tmp_sock = abnet::socket_ops::accept(serv_sock, &connected_addr, &client_addr_len, ec);
    ASSERT_EQ(ec.value(), 0) << ERRMSG("accept failed with error: ") << ec.message();

    char ping[5] = {0};
    char pong[5] = "pong";
    abnet::socket_ops::buf req_buf;
    abnet::socket_ops::init_buf(req_buf, ping, sizeof(ping));
    size_t bytesReceived = abnet::socket_ops::recv(tmp_sock, &req_buf, 1, 0, ec);
    ASSERT_EQ(bytesReceived, 5) << ERRMSG("recv failed with error: ") << ec.message();

    abnet::socket_ops::buf resp_buf;
    abnet::socket_ops::init_buf(resp_buf, pong, sizeof(pong));
    abnet::socket_ops::send(tmp_sock, &resp_buf, 1, 0, ec);
    ASSERT_EQ(ec.value(), 0) << ERRMSG("send failed with error: ") << ec.message();
  });

  std::thread client_thread([=, &ec, &server_ready, &srv_ready_mutex, &srv_ready_cv]() {
    {
      std::unique_lock<std::mutex> lock(srv_ready_mutex);
      srv_ready_cv.wait(lock, [&]() { return server_ready; });
    }

    abnet::socket_ops::connect(client_sock, reinterpret_cast<abnet::socket_addr_type *>(&s_storage), sizeof(s_storage),
                               ec);
    ASSERT_EQ(ec.value(), 0) << ERRMSG("connect failed with error: ") << ec.message();

    char ping[5] = "ping";
    char pong[5] = {0};
    abnet::socket_ops::buf buf;
    abnet::socket_ops::init_buf(buf, ping, sizeof(ping));
    abnet::socket_ops::send(client_sock, &buf, 1, 0, ec);
    ASSERT_EQ(ec.value(), 0) << ERRMSG("send failed with error: ") << ec.message();

    abnet::socket_ops::init_buf(buf, pong, sizeof(pong));
    size_t bytesReceived = abnet::socket_ops::recv(client_sock, &buf, 1, 0, ec);
    ASSERT_EQ(ec.value(), 0) << ERRMSG("recv failed with error: ") << ec.message();
    ASSERT_EQ(bytesReceived, 5) << ERRMSG("recv failed with error: ") << ec.message();
  });

  server_thread.join();
  client_thread.join();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#include <gtest/gtest.h>

#include "abnet/abnet.hpp"
#include "test_util.hpp"

TEST(HttpBasic, getHttpExample) {
  const char *hostname = "example.com";
  uint16_t port = 80;
  const char request[] = "GET / HTTP/1.1\r\nHost: example.com\r\nConnection: close\r\n\r\n";
  abnet::addrinfo_type hints = {0};
  abnet::addrinfo_type *res = nullptr;
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  abnet::error_code ec;
  abnet::socket_ops::getaddrinfo(hostname, NULL, hints, &res, ec);
  ASSERT_EQ(ec.value(), 0) << ERRMSG("getaddrinfo failed with error: ") << ec.message();

  abnet::socket_type sock = abnet::invalid_socket;

  sock = abnet::socket_ops::socket(res->ai_family, res->ai_socktype, res->ai_protocol, ec);
  ASSERT_EQ(ec.value(), 0) << ERRMSG("socket failed with error: ") << ec.message();

  abnet::sockaddr_in4_type *s_in;
  s_in = (abnet::sockaddr_in4_type *)res->ai_addr;
  s_in->sin_port = abnet::socket_ops::host_to_network_short(port);
  abnet::socket_ops::connect(sock, s_in, sizeof(*s_in), ec);
  abnet::socket_ops::freeaddrinfo(res);
  ASSERT_EQ(ec.value(), 0) << ERRMSG("connect failed with error: ") << ec.message();

  abnet::socket_ops::buf req_buf;
  abnet::socket_ops::init_buf(req_buf, request, sizeof(request));
  abnet::socket_ops::send(sock, &req_buf, 1, 0, ec);
  ASSERT_EQ(ec.value(), 0) << ERRMSG("send failed with error: ") << ec.message();

  char response[4096];
  abnet::socket_ops::buf resp_buf;
  abnet::socket_ops::init_buf(resp_buf, response, sizeof(response));
  size_t bytesReceived = abnet::socket_ops::recv(sock, &resp_buf, 1, 0, ec);
  ASSERT_GT(bytesReceived, 0) << ERRMSG("recv failed: ") << ec.message();

  abnet::socket_ops::close(sock, 0, 0, ec);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
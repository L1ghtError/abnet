#include <gtest/gtest.h>

#include "abnet/abnet.hpp"
#include "test_util.hpp"

constexpr const char *test_hostname = "google.com";

TEST(DnsRequest, AsciiToBinaryV4) {

  const char addr[] = "255.123.8.1";
  abnet::sockaddr_in4_type addr_in;
  abnet::error_code ec;

  abnet::socket_ops::inet_pton(ABNET_OS_DEF(AF_INET), addr, &addr_in.sin_addr, 0, ec);
  ASSERT_EQ(ec.value(), 0) << ERRMSG("inet_pton failed with error: ") << ec.message();
  uint32_t expected_binary = abnet::socket_ops::host_to_network_long(0xFF7B0801);
  ASSERT_EQ(addr_in.sin_addr.s_addr, expected_binary)
      << ERRMSG("Address conversion mismatch: expected ") << expected_binary << ", got " << addr_in.sin_addr.s_addr;
}

TEST(DnsRequest, AsciiToBinaryV4_2) {

  const char *hostname = "255.123.8.1";
  uint16_t port = 80;
  abnet::addrinfo_type hints = {0};
  abnet::addrinfo_type *res = nullptr;
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  abnet::error_code ec;
  abnet::socket_ops::getaddrinfo(hostname, NULL, hints, &res, ec);
  ASSERT_EQ(ec.value(), 0) << "Expected failure for invalid hostname";

  abnet::sockaddr_in4_type addr_in;
  addr_in.sin_addr = reinterpret_cast<struct sockaddr_in *>(res->ai_addr)->sin_addr;

  uint32_t expected_binary = abnet::socket_ops::host_to_network_long(0xFF7B0801);
  ASSERT_EQ(addr_in.sin_addr.s_addr, expected_binary)
      << ERRMSG("Address conversion mismatch: expected ") << expected_binary << ", got " << addr_in.sin_addr.s_addr;
}

TEST(DnsRequest, AsciiToBinaryV6) {

  const char addr[] = "2001:0db8:85a3:0000:0000:8a2e:0370:7334";
  abnet::sockaddr_in6_type addr_in;
  abnet::error_code ec;

  abnet::socket_ops::inet_pton(ABNET_OS_DEF(AF_INET6), addr, &addr_in.sin6_addr, 0, ec);
  ASSERT_EQ(ec.value(), 0) << ERRMSG("inet_pton failed with error: ") << ec.message();

  // Construct expected binary representation of the IPv6 address
  uint8_t expected_binary[16] = {0x20, 0x01, 0x0d, 0xb8, 0x85, 0xa3, 0x00, 0x00,
                                 0x00, 0x00, 0x8a, 0x2e, 0x03, 0x70, 0x73, 0x34};

  ASSERT_EQ(memcmp(addr_in.sin6_addr.s6_addr, expected_binary, sizeof(expected_binary)), 0)
      << ERRMSG("Address conversion mismatch for IPv6");
}

TEST(DnsRequest, invalidHostname) {

  const char *hostname = "nonexistent.example.com";
  uint16_t port = 80;
  abnet::addrinfo_type hints = {0};
  abnet::addrinfo_type *res = nullptr;
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  abnet::error_code ec;
  abnet::socket_ops::getaddrinfo(hostname, NULL, hints, &res, ec);
  ASSERT_NE(ec.value(), 0) << "Expected failure for invalid hostname";
}

TEST(DnsRequest, ResolveGitHubAddrV4) {

  const char *host = test_hostname;
  abnet::addrinfo_type hints = {0};
  abnet::addrinfo_type *res = nullptr;
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  abnet::error_code ec;
  abnet::socket_ops::getaddrinfo(host, NULL, hints, &res, ec);
  // Assert that the address resolution was successful
  ASSERT_EQ(ec.value(), 0) << ERRMSG("getaddrinfo failed with error: ") << ec.message();
  ASSERT_NE(res, nullptr) << ERRMSG("getaddrinfo returned null for host: ") << host;

  char ip[INET_ADDRSTRLEN] = {0};
  abnet::sockaddr_in4_type *addr;
  addr = (abnet::sockaddr_in4_type *)res->ai_addr;
  abnet::socket_ops::inet_ntop(AF_INET, &(addr->sin_addr), ip, INET_ADDRSTRLEN, 0, ec);
  // Assert that inet_ntop was successful
  abnet::socket_ops::freeaddrinfo(res);
  ASSERT_EQ(ec.value(), 0) << ERRMSG("inet_ntop failed with error: ") << ec.message();
  ASSERT_GT(std::strlen(ip), 0) << ERRMSG("inet_ntop returned an empty IP string");
}

TEST(DnsRequest, ResolveGitHubAddrV6) {

  const char *host = test_hostname;
  abnet::addrinfo_type hints = {0};
  abnet::addrinfo_type *res = nullptr;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = 0;
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;
  hints.ai_flags = AI_ALL;

  hints.ai_family = AF_UNSPEC;
  abnet::error_code ec;
  abnet::socket_ops::getaddrinfo(host, NULL, hints, &res, ec);
  // Assert that the address resolution was successful
  ASSERT_EQ(ec.value(), 0) << ERRMSG("getaddrinfo failed with error: ") << ec.message();
  ASSERT_NE(res, nullptr) << ERRMSG("getaddrinfo returned null for host: ") << host;

  bool ipv6Found = false;
  for (addrinfo *ptr = res; ptr != nullptr; ptr = ptr->ai_next) {
    if (ptr->ai_family == AF_INET6) {
      sockaddr_in6 *sockaddr_ipv6 = reinterpret_cast<sockaddr_in6 *>(ptr->ai_addr);
      char ipStringBuffer[INET6_ADDRSTRLEN];

      // Convert IPv6 address to a readable string
      if (inet_ntop(AF_INET6, &(sockaddr_ipv6->sin6_addr), ipStringBuffer, sizeof(ipStringBuffer))) {
        ipv6Found = true;
        break;
      }
    }
  }
  abnet::socket_ops::freeaddrinfo(res);
  ASSERT_EQ(ipv6Found, true) << ERRMSG("Valid IPv6 addr not found: ") << ec.message();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
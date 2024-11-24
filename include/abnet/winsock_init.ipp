//
// detail/impl/winsock_init.ipp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2024 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ABNET_IMPL_WINSOCK_INIT_IPP
#define ABNET_IMPL_WINSOCK_INIT_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "abnet/config.hpp"

#if defined(ABNET_WINDOWS) || defined(__CYGWIN__)

#include "abnet/error.hpp"
#include "abnet/socket_types.hpp"
#include "abnet/winsock_init.hpp"

#include "abnet/push_options.hpp"

namespace abnet {

void winsock_init_base::startup(data &d, unsigned char major, unsigned char minor) {
  if (::InterlockedIncrement(&d.init_count_) == 1) {
    WSADATA wsa_data;
    long result = ::WSAStartup(MAKEWORD(major, minor), &wsa_data);
    ::InterlockedExchange(&d.result_, result);
  }
}

void winsock_init_base::manual_startup(data &d) {
  if (::InterlockedIncrement(&d.init_count_) == 1) {
    ::InterlockedExchange(&d.result_, 0);
  }
}

void winsock_init_base::cleanup(data &d) {
  if (::InterlockedDecrement(&d.init_count_) == 0) {
    ::WSACleanup();
  }
}

void winsock_init_base::manual_cleanup(data &d) { ::InterlockedDecrement(&d.init_count_); }

void winsock_init_base::throw_on_error(data &d) {
  long result = ::InterlockedExchangeAdd(&d.result_, 0);
  if (result != 0) {
    abnet::error_code ec(result, abnet::error::get_system_category());
    std::system_error e(ec, "Operation not permitted");

    throw std::runtime_error("An error occurred");
  }
}

} // namespace abnet

#include "abnet/pop_options.hpp"

#endif // defined(ABNET_WINDOWS) || defined(__CYGWIN__)

#endif // ABNET_IMPL_WINSOCK_INIT_IPP
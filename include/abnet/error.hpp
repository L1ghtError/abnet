//
// error.hpp
// ~~~~~~~~~
//
// Copyright (c) 2003-2024 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ABNET_ERROR_HPP
#define ABNET_ERROR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "abnet/config.hpp"
#include "abnet/error_code.hpp"
#include "abnet/system_error.hpp"
#if defined(ABNET_WINDOWS) || defined(__CYGWIN__) || defined(ABNET_WINDOWS_RUNTIME)
#include <winerror.h>
#else
#include <cerrno>
#include <netdb.h>
#endif

#if defined(GENERATING_DOCUMENTATION)
/// INTERNAL ONLY.
#define ABNET_NATIVE_ERROR(e) implementation_defined
/// INTERNAL ONLY.
#define ABNET_SOCKET_ERROR(e) implementation_defined
/// INTERNAL ONLY.
#define ABNET_NETDB_ERROR(e) implementation_defined
/// INTERNAL ONLY.
#define ABNET_GETADDRINFO_ERROR(e) implementation_defined
/// INTERNAL ONLY.
#define ABNET_WIN_OR_POSIX(e_win, e_posix) implementation_defined
#elif defined(ABNET_WINDOWS_RUNTIME)
#define ABNET_NATIVE_ERROR(e) __HRESULT_FROM_WIN32(e)
#define ABNET_SOCKET_ERROR(e) __HRESULT_FROM_WIN32(WSA##e)
#define ABNET_NETDB_ERROR(e) __HRESULT_FROM_WIN32(WSA##e)
#define ABNET_GETADDRINFO_ERROR(e) __HRESULT_FROM_WIN32(WSA##e)
#define ABNET_WIN_OR_POSIX(e_win, e_posix) e_win
#elif defined(ABNET_WINDOWS) || defined(__CYGWIN__)
#define ABNET_NATIVE_ERROR(e) e
#define ABNET_SOCKET_ERROR(e) WSA##e
#define ABNET_NETDB_ERROR(e) WSA##e
#define ABNET_GETADDRINFO_ERROR(e) WSA##e
#define ABNET_WIN_OR_POSIX(e_win, e_posix) e_win
#else
#define ABNET_NATIVE_ERROR(e) e
#define ABNET_SOCKET_ERROR(e) e
#define ABNET_NETDB_ERROR(e) e
#define ABNET_GETADDRINFO_ERROR(e) e
#define ABNET_WIN_OR_POSIX(e_win, e_posix) e_posix
#endif

#include "abnet/push_options.hpp"

namespace abnet {
namespace error {

enum basic_errors {
  /// Permission denied.
  access_denied = ABNET_SOCKET_ERROR(EACCES),

  /// Address family not supported by protocol.
  address_family_not_supported = ABNET_SOCKET_ERROR(EAFNOSUPPORT),

  /// Address already in use.
  address_in_use = ABNET_SOCKET_ERROR(EADDRINUSE),

  /// Transport endpoint is already connected.
  already_connected = ABNET_SOCKET_ERROR(EISCONN),

  /// Operation already in progress.
  already_started = ABNET_SOCKET_ERROR(EALREADY),

  /// Broken pipe.
  broken_pipe = ABNET_WIN_OR_POSIX(ABNET_NATIVE_ERROR(ERROR_BROKEN_PIPE), ABNET_NATIVE_ERROR(EPIPE)),

  /// A connection has been aborted.
  connection_aborted = ABNET_SOCKET_ERROR(ECONNABORTED),

  /// Connection refused.
  connection_refused = ABNET_SOCKET_ERROR(ECONNREFUSED),

  /// Connection reset by peer.
  connection_reset = ABNET_SOCKET_ERROR(ECONNRESET),

  /// Bad file descriptor.
  bad_descriptor = ABNET_SOCKET_ERROR(EBADF),

  /// Bad address.
  fault = ABNET_SOCKET_ERROR(EFAULT),

  /// No route to host.
  host_unreachable = ABNET_SOCKET_ERROR(EHOSTUNREACH),

  /// Operation now in progress.
  in_progress = ABNET_SOCKET_ERROR(EINPROGRESS),

  /// Interrupted system call.
  interrupted = ABNET_SOCKET_ERROR(EINTR),

  /// Invalid argument.
  invalid_argument = ABNET_SOCKET_ERROR(EINVAL),

  /// Message too long.
  message_size = ABNET_SOCKET_ERROR(EMSGSIZE),

  /// The name was too long.
  name_too_long = ABNET_SOCKET_ERROR(ENAMETOOLONG),

  /// Network is down.
  network_down = ABNET_SOCKET_ERROR(ENETDOWN),

  /// Network dropped connection on reset.
  network_reset = ABNET_SOCKET_ERROR(ENETRESET),

  /// Network is unreachable.
  network_unreachable = ABNET_SOCKET_ERROR(ENETUNREACH),

  /// Too many open files.
  no_descriptors = ABNET_SOCKET_ERROR(EMFILE),

  /// No buffer space available.
  no_buffer_space = ABNET_SOCKET_ERROR(ENOBUFS),

  /// Cannot allocate memory.
  no_memory = ABNET_WIN_OR_POSIX(ABNET_NATIVE_ERROR(ERROR_OUTOFMEMORY), ABNET_NATIVE_ERROR(ENOMEM)),

  /// Operation not permitted.
  no_permission = ABNET_WIN_OR_POSIX(ABNET_NATIVE_ERROR(ERROR_ACCESS_DENIED), ABNET_NATIVE_ERROR(EPERM)),

  /// Protocol not available.
  no_protocol_option = ABNET_SOCKET_ERROR(ENOPROTOOPT),

  /// No such device.
  no_such_device = ABNET_WIN_OR_POSIX(ABNET_NATIVE_ERROR(ERROR_BAD_UNIT), ABNET_NATIVE_ERROR(ENODEV)),

  /// Transport endpoint is not connected.
  not_connected = ABNET_SOCKET_ERROR(ENOTCONN),

  /// Socket operation on non-socket.
  not_socket = ABNET_SOCKET_ERROR(ENOTSOCK),

  /// Operation cancelled.
  operation_aborted = ABNET_WIN_OR_POSIX(ABNET_NATIVE_ERROR(ERROR_OPERATION_ABORTED), ABNET_NATIVE_ERROR(ECANCELED)),

  /// Operation not supported.
  operation_not_supported = ABNET_SOCKET_ERROR(EOPNOTSUPP),

  /// Cannot send after transport endpoint shutdown.
  shut_down = ABNET_SOCKET_ERROR(ESHUTDOWN),

  /// Connection timed out.
  timed_out = ABNET_SOCKET_ERROR(ETIMEDOUT),

  /// Resource temporarily unavailable.
  try_again = ABNET_WIN_OR_POSIX(ABNET_NATIVE_ERROR(ERROR_RETRY), ABNET_NATIVE_ERROR(EAGAIN)),

  /// The socket is marked non-blocking and the requested operation would block.
  would_block = ABNET_SOCKET_ERROR(EWOULDBLOCK)
};

enum netdb_errors {
  /// Host not found (authoritative).
  host_not_found = ABNET_NETDB_ERROR(HOST_NOT_FOUND),

  /// Host not found (non-authoritative).
  host_not_found_try_again = ABNET_NETDB_ERROR(TRY_AGAIN),

  /// The query is valid but does not have associated address data.
  no_data = ABNET_NETDB_ERROR(NO_DATA),

  /// A non-recoverable error occurred.
  no_recovery = ABNET_NETDB_ERROR(NO_RECOVERY)
};

enum addrinfo_errors {
  /// The service is not supported for the given socket type.
  service_not_found = ABNET_WIN_OR_POSIX(ABNET_NATIVE_ERROR(WSATYPE_NOT_FOUND), ABNET_GETADDRINFO_ERROR(EAI_SERVICE)),

  /// The socket type is not supported.
  socket_type_not_supported =
      ABNET_WIN_OR_POSIX(ABNET_NATIVE_ERROR(WSAESOCKTNOSUPPORT), ABNET_GETADDRINFO_ERROR(EAI_SOCKTYPE))
};

enum misc_errors {
  /// Already open.
  already_open = 1,

  /// End of file or stream.
  eof,

  /// Element not found.
  not_found,

  /// The descriptor cannot fit into the select system call's fd_set.
  fd_set_failure
};

// boostify: non-boost code starts here
#if !defined(ABNET_ERROR_LOCATION)
#define ABNET_ERROR_LOCATION(e) (void)0
#endif // !defined(ABNET_ERROR_LOCATION)

// boostify: non-boost code ends here
#if !defined(ABNET_ERROR_LOCATION) && !defined(ABNET_DISABLE_ERROR_LOCATION) && defined(ABNET_HAS_BOOST_CONFIG) &&     \
    (BOOST_VERSION >= 107900)

#define ABNET_ERROR_LOCATION(e)                                                                                        \
  do {                                                                                                                 \
    BOOST_STATIC_CONSTEXPR boost::source_location loc = BOOST_CURRENT_LOCATION;                                        \
    (e).assign((e), &loc);                                                                                             \
  } while (false)

#else // !defined(ABNET_ERROR_LOCATION)
      //   && !defined(ABNET_DISABLE_ERROR_LOCATION)
      //   && defined(ABNET_HAS_BOOST_CONFIG)
      //   && (BOOST_VERSION >= 107900)

#define ABNET_ERROR_LOCATION(e) (void)0

#endif // !defined(ABNET_ERROR_LOCATION)
       //   && !defined(ABNET_DISABLE_ERROR_LOCATION)
       //   && defined(ABNET_HAS_BOOST_CONFIG)
       //   && (BOOST_VERSION >= 107900)

inline void clear(abnet::error_code &ec) { ec.assign(0, ec.category()); }

inline const abnet::error_category &get_system_category() { return abnet::system_category(); }

#if !defined(ABNET_WINDOWS) && !defined(__CYGWIN__)

extern ABNET_DECL const abnet::error_category &get_netdb_category();

extern ABNET_DECL const abnet::error_category &get_addrinfo_category();

#else // !defined(ABNET_WINDOWS) && !defined(__CYGWIN__)

inline const abnet::error_category &get_netdb_category() { return get_system_category(); }

inline const abnet::error_category &get_addrinfo_category() { return get_system_category(); }

#endif // !defined(ABNET_WINDOWS) && !defined(__CYGWIN__)

extern ABNET_DECL const abnet::error_category &get_misc_category();

static const abnet::error_category &system_category ABNET_UNUSED_VARIABLE = abnet::error::get_system_category();
static const abnet::error_category &netdb_category ABNET_UNUSED_VARIABLE = abnet::error::get_netdb_category();
static const abnet::error_category &addrinfo_category ABNET_UNUSED_VARIABLE = abnet::error::get_addrinfo_category();
static const abnet::error_category &misc_category ABNET_UNUSED_VARIABLE = abnet::error::get_misc_category();

} // namespace error
} // namespace abnet

namespace std {

template <> struct is_error_code_enum<abnet::error::basic_errors> {
  static const bool value = true;
};

template <> struct is_error_code_enum<abnet::error::netdb_errors> {
  static const bool value = true;
};

template <> struct is_error_code_enum<abnet::error::addrinfo_errors> {
  static const bool value = true;
};

template <> struct is_error_code_enum<abnet::error::misc_errors> {
  static const bool value = true;
};

} // namespace std

namespace abnet {
namespace error {

inline abnet::error_code make_error_code(basic_errors e) {
  return abnet::error_code(static_cast<int>(e), get_system_category());
}

inline abnet::error_code make_error_code(netdb_errors e) {
  return abnet::error_code(static_cast<int>(e), get_netdb_category());
}

inline abnet::error_code make_error_code(addrinfo_errors e) {
  return abnet::error_code(static_cast<int>(e), get_addrinfo_category());
}

inline abnet::error_code make_error_code(misc_errors e) {
  return abnet::error_code(static_cast<int>(e), get_misc_category());
}

} // namespace error
namespace stream_errc {
// Simulates the proposed stream_errc scoped enum.
using error::eof;
using error::not_found;
} // namespace stream_errc
namespace socket_errc {
// Simulates the proposed socket_errc scoped enum.
using error::already_open;
using error::not_found;
} // namespace socket_errc
namespace resolver_errc {
// Simulates the proposed resolver_errc scoped enum.
using error::host_not_found;
const error::netdb_errors try_again = error::host_not_found_try_again;
using error::service_not_found;
} // namespace resolver_errc
} // namespace abnet

#include "abnet/pop_options.hpp"

#undef ABNET_NATIVE_ERROR
#undef ABNET_SOCKET_ERROR
#undef ABNET_NETDB_ERROR
#undef ABNET_GETADDRINFO_ERROR
#undef ABNET_WIN_OR_POSIX

#if defined(ABNET_HEADER_ONLY)
#include "abnet/error.ipp"
#endif // defined(ABNET_HEADER_ONLY)

#endif // ABNET_ERROR_HPP
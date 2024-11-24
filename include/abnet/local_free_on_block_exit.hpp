//
// detail/local_free_on_block_exit.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2024 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ABNET_LOCAL_FREE_ON_BLOCK_EXIT_HPP
#define ABNET_LOCAL_FREE_ON_BLOCK_EXIT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "abnet/config.hpp"

#if defined(ABNET_WINDOWS) || defined(__CYGWIN__)
#if !defined(ABNET_WINDOWS_APP)

#include "abnet/noncopyable.hpp"
#include "abnet/socket_types.hpp"

#include "abnet/push_options.hpp"

namespace abnet {

class local_free_on_block_exit : private noncopyable {
public:
  // Constructor blocks all signals for the calling thread.
  explicit local_free_on_block_exit(void *p) : p_(p) {}

  // Destructor restores the previous signal mask.
  ~local_free_on_block_exit() { ::LocalFree(p_); }

private:
  void *p_;
};

} // namespace abnet

#include "abnet/pop_options.hpp"

#endif // !defined(ABNET_WINDOWS_APP)
#endif // defined(ABNET_WINDOWS) || defined(__CYGWIN__)

#endif // ABNET_DETAIL_LOCAL_FREE_ON_BLOCK_EXIT_HPP
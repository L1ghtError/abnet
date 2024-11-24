//
// detail/noncopyable.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2024 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ABNET_NONCOPYABLE_HPP
#define ABNET_NONCOPYABLE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "abnet/config.hpp"

#include "abnet/push_options.hpp"

namespace abnet {

class noncopyable {
protected:
  noncopyable() {}
  ~noncopyable() {}

private:
  noncopyable(const noncopyable &);
  const noncopyable &operator=(const noncopyable &);
};

using abnet::noncopyable;

} // namespace abnet

#include "abnet/pop_options.hpp"

#endif // ABNET_NONCOPYABLE_HPP
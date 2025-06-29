///
/// @file   int128_t.hpp
///
/// Copyright (C) 2025 Kim Walisch, <kim.walisch@gmail.com>
///
/// This file is distributed under the BSD License. See the COPYING
/// file in the top level directory.
///

#ifndef INT128_T_HPP
#define INT128_T_HPP

#include <stdint.h>
#include <algorithm>
#include <ostream>
#include <string>

#if !defined(INT128_MAX)

using int128_t = __int128_t;
using uint128_t = __uint128_t;

namespace {

inline std::string to_string(uint128_t n)
{
  std::string str;

  while (n > 0)
  {
    str += '0' + n % 10;
    n /= 10;
  }

  if (str.empty())
    str = "0";

  std::reverse(str.begin(), str.end());

  return str;
}

inline std::string to_string(int128_t n)
{
  if (n >= 0)
    return to_string((uint128_t) n);
  else
    return "-" + to_string((uint128_t) -n);
}

inline std::ostream& operator<<(std::ostream& stream, int128_t n)
{
  stream << to_string(n);
  return stream;
}

inline std::ostream& operator<<(std::ostream& stream, uint128_t n)
{
  stream << to_string(n);
  return stream;
}

} // namespace

#endif

#endif

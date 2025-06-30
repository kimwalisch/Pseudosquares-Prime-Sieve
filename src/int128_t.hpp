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
#include <stdexcept>
#include <string>
#include <string_view>

#if !defined(INT128_MAX)

using int128_t = __int128_t;
using uint128_t = __uint128_t;

namespace {

#if __cplusplus >= 202002L

consteval uint128_t to_uint128(std::string_view str)
{
    if (str.empty())
        throw std::invalid_argument("Input string cannot be empty.");

    uint128_t value = 0;
    constexpr uint128_t max_val = uint128_t(-1);
    constexpr uint128_t limit = max_val / 10;
    constexpr int remainder = max_val % 10;

    for (char c : str)
    {
        if (c < '0' || c > '9')
            throw std::invalid_argument("Invalid character in input string.");

        const int digit = c - '0';

        if (value > limit || (value == limit && digit > remainder))
            throw std::overflow_error("String represents a number too large for uint128_t.");

        value = value * 10 + digit;
    }

    return value;
}

#else

constexpr uint128_t to_uint128_impl(std::string_view str, uint128_t value = 0)
{
    return str.empty() ? value
        : (str.front() >= '0' && str.front() <= '9'
            ? to_uint128_impl(str.substr(1), value * 10 + uint128_t(str.front() - '0'))
            // Crash using infinite recursion in case of invalid input
            : to_uint128_impl(str, value)); 
}

constexpr uint128_t to_uint128(std::string_view str)
{
    return to_uint128_impl(str);
}

#endif

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

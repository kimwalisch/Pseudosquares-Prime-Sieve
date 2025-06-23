///
/// @file   pseudosquares_prime_sieve.hpp
/// @brief  This is a C++ implementation of J. P. Sorenson's
///         Pseudosquares Prime Sieve algorithm for 64-bit integers.
///         The Pseudosquares Prime Sieve uses much less memory
///         than most other prime sieving algorithms: it has a
///         conjectured runtime complexity of O(n log n) operations
///         and uses O((log n)^2) space.
///
/// Copyright (C) 2025 Kim Walisch, <kim.walisch@gmail.com>
///
/// This file is distributed under the BSD License. See the COPYING
/// file in the top level directory.
///

#ifndef PSEUDOSQUARES_PRIME_SIEVE_HPP
#define PSEUDOSQUARES_PRIME_SIEVE_HPP

#include <algorithm>
#include <iostream>
#include <string>
#include <stdint.h>

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

// Sieve primes inside [start, stop]
uint64_t pseudosquares_prime_sieve(uint128_t start,
                                   uint128_t stop,
                                   bool is_print = false);

#endif

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

#include "int128_t.hpp"
#include <stdint.h>

// Sieve primes inside [start, stop]
uint64_t pseudosquares_prime_sieve(uint128_t start,
                                   uint128_t stop,
                                   bool print_primes = false,
                                   bool verbose = false);

#endif

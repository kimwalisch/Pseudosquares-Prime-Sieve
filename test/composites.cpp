///
/// @file   composites.cpp
/// @brief  Test that the pseudosquares prime test rejects
///         composites n ≡ 1 (mod 8) whose prime factors are all
///         larger than the sieving limit s. pseudosquares_prime_sieve
///         <= 1.1 incorrectly identified these composites as primes.
///
/// Copyright (C) 2026 Kim Walisch, <kim.walisch@gmail.com>
///
/// This file is distributed under the BSD License. See the COPYING
/// file in the top level directory.
///

#include <pseudosquares_prime_sieve.hpp>
#include <int128_t.hpp>

#include <stdint.h>
#include <iostream>
#include <cstdlib>
#include <array>

void check(bool OK)
{
  std::cout << "   " << (OK ? "OK" : "ERROR") << "\n";
  if (!OK)
    std::exit(1);
}

int main()
{
  const std::array<uint128_t, 2> composites =
  {
    // 64460537 * 451223753 * 837986969
    // 3^((n-1)/2) ≡ -1 (mod n), but 5^((n-1)/2) ≢ ±1 (mod n).
    // Bug: returned true after the first -1 result without
    // checking the remaining bases pi ≤ p.
    to_uint128("24373794085298212366710809"),

    // 63960667 * 127921333 * 191881999 (Carmichael number)
    // pi^((n-1)/2) ≡ +1 (mod n) for all primes pi.
    // Bug: returned true once Lq > n, but if all primes ≤ q
    // give +1 and Lq > n, then n is composite.
    to_uint128("1569965809815914854692889")
  };

  for (uint128_t n : composites)
  {
    uint64_t count = pseudosquares_prime_sieve(n, n);
    std::cout << "pseudosquares_prime_sieve(" << n << ", " << n << ") = " << count;
    check(count == 0);
  }

  std::cout << std::endl;
  std::cout << "All tests passed successfully!" << std::endl;

  return 0;
}

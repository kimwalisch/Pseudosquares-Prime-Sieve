#include "pseudosquares_prime_sieve.hpp"

#include <array>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <stdint.h>

/// Correct pi(x) values to compare with test results
const std::array<uint64_t, 8> pix =
{
  4,        // pi(10^1)
  25,       // pi(10^2)
  168,      // pi(10^3)
  1229,     // pi(10^4)
  9592,     // pi(10^5)
  78498,    // pi(10^6)
  664579,   // pi(10^7)
  5761455,  // pi(10^8)
};

// Prime counts from Sorenson's paper:
// https://digitalcommons.butler.edu/cgi/viewcontent.cgi?article=1095&context=facsch_papers
const std::array<uint64_t, 24> pix_2 =
{
  43427, // pi[10^10, 10^10+10^6]
  39434, // pi[10^11, 10^11+10^6]
  36249, // pi[10^12, 10^12+10^6]
  33456, // pi[10^13, 10^13+10^6]
  30892, // pi[10^14, 10^14+10^6]
  28845, // pi[10^15, 10^15+10^6]
  27168, // pi[10^16, 10^16+10^6]
  25463, // pi[10^17, 10^17+10^6]
  24280, // pi[10^18, 10^18+10^6]
  23069, // pi[10^19, 10^19+10^6]
  21632, // pi[10^20, 10^20+10^6]
  20832, // pi[10^21, 10^21+10^6]
  19757, // pi[10^22, 10^22+10^6]
  18939, // pi[10^23, 10^23+10^6]
  18149, // pi[10^24, 10^24+10^6]
  17549, // pi[10^25, 10^25+10^6]
  16587, // pi[10^26, 10^26+10^6]
  16139, // pi[10^27, 10^27+10^6]
  15606, // pi[10^28, 10^28+10^6]
  15002, // pi[10^29, 10^29+10^6]
  14496, // pi[10^30, 10^30+10^6]
  13955, // pi[10^31, 10^31+10^6]
  13653, // pi[10^32, 10^32+10^6]
  13284  // pi[10^33, 10^33+10^6]
};

void check(bool OK)
{
  std::cout << "   " << (OK ? "OK" : "ERROR") << "\n";
  if (!OK)
    std::exit(1);
}

int main()
{
  std::cout << std::left;
  uint64_t count = 0;
  uint128_t stop = 1;

  // PrimePi(x) with x = 10^(i+1)
  for (size_t i = 0; i < pix.size(); i++)
  {
    uint128_t start = stop + 1;
    stop *= 10;
    count += pseudosquares_prime_sieve(start, stop);
    std::cout << "PrimePi(10^" << i + 1 << ") = " << std::setw(8) << count;
    check(count == pix[i]);
  }

  std::cout << std::endl;
  uint128_t start = (uint128_t) 1e10;
  std::size_t j = 10;

  // PrimePi(10^n, 10^n+10^6) for 10 <= n <= 33
  for (std::size_t i = 0; i < pix_2.size(); i++)
  {
    uint128_t stop = start + (uint64_t) 1e6;
    uint64_t count = pseudosquares_prime_sieve(start, stop);
    std::cout << "PrimePi(10^" << j << ", 10^" << j << "+10^6) = " << std::setw(7) << count;
    check(count == pix_2[i]);

    start *= 10;
    j++;
  }

  std::cout << std::endl;
  std::cout << "All tests passed successfully!" << std::endl;

  return 0;
}

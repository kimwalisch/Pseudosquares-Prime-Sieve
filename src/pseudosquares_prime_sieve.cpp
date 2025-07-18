///
/// @file   pseudosquares_prime_sieve.cpp
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

#include "pseudosquares_prime_sieve.hpp"
#include "int128_t.hpp"
#include "modpow.hpp"
#include "Sieve.hpp"
#include "Vector.hpp"

#include <primesieve.hpp>

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <stdexcept>
#include <stdint.h>

namespace {

// First 128 primes
const Array<int, 128> primes =
{
      2,   3,   5,   7,  11,  13,  17,  19,  23,  29,
     31,  37,  41,  43,  47,  53,  59,  61,  67,  71,
     73,  79,  83,  89,  97, 101, 103, 107, 109, 113,
    127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
    233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
    283, 293, 307, 311, 313, 317, 331, 337, 347, 349,
    353, 359, 367, 373, 379, 383, 389, 397, 401, 409,
    419, 421, 431, 433, 439, 443, 449, 457, 461, 463,
    467, 479, 487, 491, 499, 503, 509, 521, 523, 541,
    547, 557, 563, 569, 571, 577, 587, 593, 599, 601,
    607, 613, 617, 619, 631, 641, 643, 647, 653, 659,
    661, 673, 677, 683, 691, 701, 709, 719
};

/// Number of primes < 450
const Array<uint8_t, 450> prime_pi =
{
     0,   0,   1,   2,   2,   3,   3,   4,   4,   4,   4,   5,   5,   6,   6,
     6,   6,   7,   7,   8,   8,   8,   8,   9,   9,   9,   9,   9,   9,  10,
    10,  11,  11,  11,  11,  11,  11,  12,  12,  12,  12,  13,  13,  14,  14,
    14,  14,  15,  15,  15,  15,  15,  15,  16,  16,  16,  16,  16,  16,  17,
    17,  18,  18,  18,  18,  18,  18,  19,  19,  19,  19,  20,  20,  21,  21,
    21,  21,  21,  21,  22,  22,  22,  22,  23,  23,  23,  23,  23,  23,  24,
    24,  24,  24,  24,  24,  24,  24,  25,  25,  25,  25,  26,  26,  27,  27,
    27,  27,  28,  28,  29,  29,  29,  29,  30,  30,  30,  30,  30,  30,  30,
    30,  30,  30,  30,  30,  30,  30,  31,  31,  31,  31,  32,  32,  32,  32,
    32,  32,  33,  33,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  35,
    35,  36,  36,  36,  36,  36,  36,  37,  37,  37,  37,  37,  37,  38,  38,
    38,  38,  39,  39,  39,  39,  39,  39,  40,  40,  40,  40,  40,  40,  41,
    41,  42,  42,  42,  42,  42,  42,  42,  42,  42,  42,  43,  43,  44,  44,
    44,  44,  45,  45,  46,  46,  46,  46,  46,  46,  46,  46,  46,  46,  46,
    46,  47,  47,  47,  47,  47,  47,  47,  47,  47,  47,  47,  47,  48,  48,
    48,  48,  49,  49,  50,  50,  50,  50,  51,  51,  51,  51,  51,  51,  52,
    52,  53,  53,  53,  53,  53,  53,  53,  53,  53,  53,  54,  54,  54,  54,
    54,  54,  55,  55,  55,  55,  55,  55,  56,  56,  56,  56,  56,  56,  57,
    57,  58,  58,  58,  58,  58,  58,  59,  59,  59,  59,  60,  60,  61,  61,
    61,  61,  61,  61,  61,  61,  61,  61,  62,  62,  62,  62,  62,  62,  62,
    62,  62,  62,  62,  62,  62,  62,  63,  63,  63,  63,  64,  64,  65,  65,
    65,  65,  66,  66,  66,  66,  66,  66,  66,  66,  66,  66,  66,  66,  66,
    66,  67,  67,  67,  67,  67,  67,  68,  68,  68,  68,  68,  68,  68,  68,
    68,  68,  69,  69,  70,  70,  70,  70,  71,  71,  71,  71,  71,  71,  72,
    72,  72,  72,  72,  72,  72,  72,  73,  73,  73,  73,  73,  73,  74,  74,
    74,  74,  74,  74,  75,  75,  75,  75,  76,  76,  76,  76,  76,  76,  77,
    77,  77,  77,  77,  77,  77,  77,  78,  78,  78,  78,  79,  79,  79,  79,
    79,  79,  79,  79,  80,  80,  80,  80,  80,  80,  80,  80,  80,  80,  81,
    81,  82,  82,  82,  82,  82,  82,  82,  82,  82,  82,  83,  83,  84,  84,
    84,  84,  84,  84,  85,  85,  85,  85,  86,  86,  86,  86,  86,  86,  87
};

struct Pseudosquare
{
    int p;
    uint128_t Lp;
};

// List of known pseudosquares <= 4.2 * 10^24
const Array<Pseudosquare, 74> pseudosquares =
{{
      { 2, to_uint128("17") },
      { 3, to_uint128("73") },
      { 5, to_uint128("241") },
      { 7, to_uint128("1009") },
     { 11, to_uint128("2641") },
     { 13, to_uint128("8089") },
     { 17, to_uint128("18001") },
     { 19, to_uint128("53881") },
     { 23, to_uint128("87481") },
     { 29, to_uint128("117049") },
     { 31, to_uint128("515761") },
     { 37, to_uint128("1083289") },
     { 41, to_uint128("3206641") },
     { 43, to_uint128("3818929") },
     { 47, to_uint128("9257329") },
     { 53, to_uint128("22000801") },
     { 59, to_uint128("48473881") },
     { 61, to_uint128("48473881") },
     { 67, to_uint128("175244281") },
     { 71, to_uint128("427733329") },
     { 73, to_uint128("427733329") },
     { 79, to_uint128("898716289") },
     { 83, to_uint128("2805544681") },
     { 89, to_uint128("2805544681") },
     { 97, to_uint128("2805544681") },
    { 101, to_uint128("10310263441") },
    { 103, to_uint128("23616331489") },
    { 107, to_uint128("85157610409") },
    { 109, to_uint128("85157610409") },
    { 113, to_uint128("196265095009") },
    { 127, to_uint128("196265095009") },
    { 131, to_uint128("2871842842801") },
    { 137, to_uint128("2871842842801") },
    { 139, to_uint128("2871842842801") },
    { 149, to_uint128("26250887023729") },
    { 151, to_uint128("26250887023729") },
    { 157, to_uint128("112434732901969") },
    { 163, to_uint128("112434732901969") },
    { 167, to_uint128("112434732901969") },
    { 173, to_uint128("178936222537081") },
    { 179, to_uint128("178936222537081") },
    { 181, to_uint128("696161110209049") },
    { 191, to_uint128("696161110209049") },
    { 193, to_uint128("2854909648103881") },
    { 197, to_uint128("6450045516630769") },
    { 199, to_uint128("6450045516630769") },
    { 211, to_uint128("11641399247947921") },
    { 223, to_uint128("11641399247947921") },
    { 227, to_uint128("190621428905186449") },
    { 229, to_uint128("196640148121928601") },
    { 233, to_uint128("712624335095093521") },
    { 239, to_uint128("1773855791877850321") },
    { 241, to_uint128("2327687064124474441") },
    { 251, to_uint128("6384991873059836689") },
    { 257, to_uint128("8019204661305419761") },
    { 263, to_uint128("10198100582046287689") },
    { 269, to_uint128("10198100582046287689") },
    { 271, to_uint128("10198100582046287689") },
    { 277, to_uint128("69848288320900186969") },
    { 281, to_uint128("208936365799044975961") },
    { 283, to_uint128("533552663339828203681") },
    { 293, to_uint128("936664079266714697089") },
    { 307, to_uint128("936664079266714697089") },
    { 311, to_uint128("2142202860370269916129") },
    { 313, to_uint128("2142202860370269916129") },
    { 317, to_uint128("2142202860370269916129") },
    { 331, to_uint128("13649154491558298803281") },
    { 337, to_uint128("34594858801670127778801") },
    { 347, to_uint128("99492945930479213334049") },
    { 349, to_uint128("99492945930479213334049") },
    { 353, to_uint128("295363187400900310880401") },
    { 359, to_uint128("295363187400900310880401") },
    { 367, to_uint128("3655334429477057460046489") },
    { 373, to_uint128("4235025223080597503519329") }
}};

struct SievingPrime
{
    uint32_t prime;
    int32_t i;

    SievingPrime(uint64_t p, int64_t index)
    {
        set_prime(p);
        set_index(index);
    }

    void set_prime(uint64_t p)
    {
        ASSERT(p <= std::numeric_limits<decltype(prime)>::max());
        prime = (decltype(prime)) p;
    }

    void set_index(int64_t index)
    {
        ASSERT(index <= std::numeric_limits<decltype(i)>::max());
        i = (decltype(i)) index;
    }
};

// Generate sieving primes <= n
Vector<SievingPrime> get_sieving_primes(uint64_t n)
{
    // We sieve using all sieving primes <= s (n).
    // Hence, s is the maximum sieving prime.
    // We store the sieving prime in the uint32_t
    // SievingPrime::prime variable.
    if (n > std::numeric_limits<uint32_t>::max())
        throw std::runtime_error("get_sieving_primes: n must be <= 2^32");

    // We sieve using all sieving primes <= s (n).
    // Hence, s is the maximum sieving prime. For each
    // sieving prime we store its sieve array index
    // in the int32_t SievingPrime::i variable. Since
    // we use i += prime * 2; the maximum sieving
    // prime must be <= max(int32_t) / 2.
    if (n > std::numeric_limits<int32_t>::max() / 2)
        throw std::runtime_error("get_sieving_primes: n must be <= 2^30");

    // pi(x) <= x / (log(x) - 1.1) + 5, for x >= 4.
    // Pierre Dusart, https://arxiv.org/abs/1002.0442 eq. 6.6.
    double x = (double) n;
    x = std::max(100.0, x);
    double pix = x / (std::log(x) - 1.1) + 5;

    std::size_t size = (std::size_t) pix;
    Vector<SievingPrime> sieving_primes;
    sieving_primes.reserve(size);
    primesieve::iterator it(3, n);
    uint64_t prime;

    while ((prime = it.next_prime()) <= n)
        sieving_primes.emplace_back(prime, -1);

    return sieving_primes;
}

// In Sorenson's paper the semgent size is named ∆,
// with ∆ = s / log(n). We also have ∆ = Θ(π(p) log n).
// Sorenson's paper also mentions that using a larger
// segment size improves performance. Hence, we use a
// segment size of O(n^(1/4.5)).
//
uint64_t get_segment_size(uint128_t stop)
{
    // Default sieve array size = 256 kilobytes
    uint64_t segment_size = (256 << 10) * Sieve::numbers_per_byte();
    uint64_t root4_stop = (uint64_t) std::pow(stop, 1.0 / 4.5);
    segment_size = std::max(segment_size, root4_stop);
    return segment_size;
}

void initialize(uint128_t stop,
                uint64_t& delta,
                uint64_t& s,
                uint64_t& p,
                bool verbose)
{
    // In Sorenson's paper the segment size is named ∆,
    // with ∆ = s / log(n). s is the upper bound for
    // sieving, we sieve using the sieving primes <= s.
    delta = get_segment_size(stop);

    double log_delta = std::log(delta);
    log_delta = std::max(1.0, log_delta);
    s = delta * log_delta;
    uint128_t Lp;

    // We have a list of known pseudosquares up to
    // max(Lp) = L_373 ~ 4.2 * 10^24. Hence, using
    // delta = n^(1/4.5) and s = delta * log(delta)
    // we can sieve primes up to n:
    // n / s < Lp
    // n / (n^(1/4.5) * log(n^(1/4.5))) < 4.2 * 10^24
    // n < 1.74083 * 10^33
    if (stop / s >= pseudosquares.back().Lp)
        throw std::runtime_error("pseudosquares_prime_sieve: n/s must be < max(Lp)");

    // Pick the prime p so that Lp such that n/s < Lp
    for (const auto& pss : pseudosquares)
    {
        p = pss.p;
        Lp = pss.Lp;
        if (pss.Lp > stop / s)
            break;
    }

    if (verbose)
    {
        std::cout << "Sieve size: " << delta / Sieve::numbers_per_byte() << " bytes" << std::endl;
        std::cout << "delta: " << delta << std::endl;
        std::cout << "s: " << s << " (max sieving prime)" << std::endl;
        std::cout << "p: " << p << " (pseudosquare prime)" << std::endl;
        std::cout << "Lp: " << Lp << " (pseudosquare)" << std::endl;
    }
}

// Sorenson's Pseudosquares Prime Test
bool pseudosquares_prime_test(uint128_t n, int p)
{
    ASSERT(p >= 2);
    uint128_t e = (n - 1) >> 1;
    uint128_t minus1 = n - 1;

    // 2^((n−1)/2) mod n
    uint128_t res = modpow<2>(e, n);

    // Condition (4) for n ≡ 1 mod 8: found -1 result
    if ((n & 7) == 1 && res == minus1)
        return true;
    // Condition (4) for n ≡ 5 mod 8: 2^((n−1)/2) ≡ −1 mod n
    if ((n & 7) == 5 && res != minus1)
        return false;
    // Condition (3): 2^((n−1)/2) ≡ ±1 mod n
    if (res != 1 && res != minus1)
        return false;

    // For 3 <= pi ≤ p: pi^((n−1)/2) mod n
    for (std::size_t i = 1; primes[i] <= p; i++)
    {
        res = modpow(primes[i], e, n);

        // Condition (4) for n ≡ 1 mod 8: found -1 result
        if ((n & 7) == 1 && res == minus1)
            return true;
        // Condition (3): pi^((n−1)/2) ≡ ±1 mod n
        if (res != 1 && res != minus1)
            return false;
    }

    // Condition (4): for n ≡ 1 mod 8:
    if ((n & 7) == 1)
    {
        // In case we have not found any -1 result so far,
        // check all pi > p while Lpi <= n: pi^((n−1)/2) ≡ ±1 mod n
        // This step is missing in Sorenson's paper. Sorenson
        // confirmed it was a bug and suggested this fix.
        for (std::size_t i = prime_pi[p] + 1; pseudosquares.at(i).Lp <= n; i++)
        {
            res = modpow(primes[i], e, n);

            if (res == minus1)
                return true;
            if (res != 1)
                return false;
        }
    }

    return true;
}

} // namespace

// Sieve primes inside [start, stop]
uint64_t pseudosquares_prime_sieve(uint128_t start,
                                   uint128_t stop,
                                   bool print_primes,
                                   bool verbose)
{
    // After having run sieving and the pseudosquares prime
    // test, one has to remove perfect powers. Our implementation
    // misses this final step and is hence limited to
    // n <= 6.4 * 10^37 (according to Sorenson's paper).
    //
    // However, our implementation is also limited by the
    // formula n / s < max(Lp) and since we only have a list of
    // pseudosqaures up to max(Lp) = L_373 our implementation
    // requires n <= 1.73 * 10^33, see initialize().
    if ((double) stop > 1.73 * 1e33)
        throw std::runtime_error("pseudosquares_prime_sieve: stop must be <= 1.73 * 10^33");

    uint64_t count = 0;

    if (start < 2)
        start = 2;
    if (start > stop)
        return 0;
    if (start <= 2)
    {
        count++;
        if (print_primes)
            std::cout << 2 << "\n";
        start = 3;
        if (start > stop)
            return count;
    }

    // Same variable names as in Sorenson's paper
    uint64_t delta, s, p;
    initialize(stop, delta, s, p, verbose);
    Sieve sieve(delta);

    uint64_t sqrt_stop = (uint64_t) std::sqrt(stop);
    uint64_t max_sieving_prime = std::min(s, sqrt_stop);
    Vector<SievingPrime> sieving_primes = get_sieving_primes(max_sieving_prime);

    for (uint128_t low = start; low <= stop; low += sieve.size())
    {
        // Sieve current segment [low, high]
        uint128_t high = low + sieve.size() - 1;
        high = std::min(high, stop);
        uint64_t sqrt_high = (uint64_t) std::sqrt(high);
        uint64_t max_i = uint64_t(high - low) + 1;
        max_sieving_prime = std::min(s, sqrt_high);
        sieve.set_all_bits();

        // Sieve out multiples of primes <= s
        for (auto& sp : sieving_primes)
        {
            uint64_t prime = sp.prime;
            uint64_t i = sp.i;

            if (prime > max_sieving_prime)
                break;
            if (sp.i == -1)
            {
                uint128_t q = low / prime;
                uint128_t n = q * prime;
                uint128_t pp = uint64_t(prime) * prime;
                n += prime & -(n < low);
                n += prime & -((n & 1) == 0);
                n = std::max(n, pp);
                ASSERT(n % 2 == 0);
                i = uint64_t(n - low);
            }

            for (; i < max_i; i += prime * 2)
                sieve.unset_bit(i);

            sp.set_index(i - max_i);
        }

        for (uint128_t n = low + (~low & 1); n <= high; n += 2)
        {
            // sieve[n]=true is a potential prime
            if (sieve[n - low])
            {
                if (max_sieving_prime >= sqrt_high)
                {
                    count++;
                    if (print_primes)
                        std::cout << n << "\n";
                }
                else if (pseudosquares_prime_test(n, p))
                {
                    count++;
                    if (print_primes)
                        std::cout << n << "\n";
                }
            }
        }
    }

    return count;
}

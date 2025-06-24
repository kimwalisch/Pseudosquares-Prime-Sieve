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

#include <flint/fmpz.h>

#include <iostream>
#include <array>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <stdexcept>
#include <stdint.h>

// First 128 primes
const std::array<int, 128> primes =
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
const std::array<uint8_t, 450> prime_pi =
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
    uint64_t Lp;
};

// Static pseudosquare table up to p = 271
const std::array<Pseudosquare, 58> pseudosquares =
{{
    { 2,   17 },
    { 3,   73 },
    { 5,   241 },
    { 7,   1009 },
    { 11,  2641 },
    { 13,  8089 },
    { 17,  18001 },
    { 19,  53881 },
    { 23,  87481 },
    { 29,  117049 },
    { 31,  515761 },
    { 37,  1083289 },
    { 41,  3206641 },
    { 43,  3818929 },
    { 47,  9257329 },
    { 53,  22000801 },
    { 59,  48473881 },
    { 61,  48473881 },
    { 67,  175244281 },
    { 71,  427733329 },
    { 73,  427733329 },
    { 79,  898716289 },
    { 83,  2805544681 },
    { 89,  2805544681 },
    { 97,  2805544681 },
    { 101, 10310263441 },
    { 103, 23616331489 },
    { 107, 85157610409 },
    { 109, 85157610409 },
    { 113, 196265095009 },
    { 127, 196265095009 },
    { 131, 2871842842801 },
    { 137, 2871842842801 },
    { 139, 2871842842801 },
    { 149, 26250887023729 },
    { 151, 26250887023729 },
    { 157, 112434732901969 },
    { 163, 112434732901969 },
    { 167, 112434732901969 },
    { 173, 178936222537081 },
    { 179, 178936222537081 },
    { 181, 696161110209049 },
    { 191, 696161110209049 },
    { 193, 2854909648103881 },
    { 197, 6450045516630769 },
    { 199, 6450045516630769 },
    { 211, 11641399247947921 },
    { 223, 11641399247947921 },
    { 227, 190621428905186449 },
    { 229, 196640148121928601 },
    { 233, 712624335095093521 },
    { 239, 1773855791877850321 },
    { 241, 2327687064124474441 },
    { 251, 6384991873059836689 },
    { 257, 8019204661305419761 },
    { 263, 10198100582046287689ull },
    { 269, 10198100582046287689ull },
    { 271, 10198100582046287689ull }
}};

struct SievingPrime
{
    uint64_t prime;
    int64_t i;
};

// Generate sieving primes <= n
std::vector<SievingPrime> get_sieving_primes(uint64_t n)
{
    std::vector<bool> sieve(n + 1, true);

    for (uint64_t i = 3; i * i <= n; i += 2)
        if (sieve[i])
            for (uint64_t j = i * i; j <= n; j += i * 2)
                sieve[j] = false;

    std::vector<SievingPrime> sieving_primes;
    sieving_primes.push_back({2, -1});

    for (uint64_t i = 3; i <= n; i += 2)
        if (sieve[i])
            sieving_primes.push_back({i, -1});

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
    uint64_t segment_size = 1 << 14;
    uint64_t root4_stop = (uint64_t) std::pow(stop, 1.0 / 4.5);
    segment_size = std::max(segment_size, root4_stop);
    return segment_size;
}

// Set z = x
void fmpz_set_ui128(fmpz_t z, uint128_t  x, fmpz_t tmp)
{
    uint64_t lo = (uint64_t)x;
    uint64_t hi = (uint64_t)(x >> 64);

    if (hi == 0)
        fmpz_set_ui(z, lo);
    else
    {
        // compute tmp = hi << 64
        fmpz_set_ui(tmp, hi);
        fmpz_mul_2exp(tmp, tmp, 64);
        // z = lo + tmp
        fmpz_set_ui(z, lo);
        fmpz_add(z, z, tmp);
    }
}

// Sorenson's Pseudosquares Prime Test
bool pseudosquares_prime_test(uint128_t n,
                              int p,
                              fmpz_t fn,
                              fmpz_t fe,
                              fmpz_t fone,
                              fmpz_t fminus1,
                              fmpz_t fbase,
                              fmpz_t fres,
                              fmpz_t ftmp)
{
    fmpz_set_ui128(fn, n, ftmp);
    fmpz_set_ui128(fe, (n - 1) >> 1, ftmp);
    fmpz_sub_ui(fminus1, fn, 1);

    int count_minus1 = 0;

    // Condition (4) for n ≡ 5 mod 8: 2^((n−1)/2) ≡ −1 mod n
    if ((n & 7) == 5)
    {
        fmpz_set_ui(fbase, 2);
        fmpz_powm(fres, fbase, fe, fn);
        if (fmpz_cmp(fres, fminus1) != 0)
            return false;
    }

    // Condition (3): for all pi ≤ p: pi^((n−1)/2) ≡ ±1 mod n
    for (std::size_t i = 0; primes[i] <= p; i++)
    {
        fmpz_set_ui(fbase, primes[i]);
        fmpz_powm(fres, fbase, fe, fn);

        if (fmpz_cmp(fres, fone) != 0 && fmpz_cmp(fres, fminus1) != 0)
            return false;
        if (fmpz_cmp(fres, fminus1) == 0)
            count_minus1++;
    }

    // Condition (4) for n ≡ 1 mod 8:
    if ((n & 7) == 1)
    {
        // Must either have at least one res ≡ -1
        if (count_minus1 > 0)
            return true;

        // Or check all q > p while Lq <= n: q^((n−1)/2) ≡ ±1 mod n
        // This step is missing in Sorenson's paper. Sorenson
        // confirmed it was a bug and suggested this fix.
        for (std::size_t i = prime_pi[p] + 1; i < pseudosquares.size() && pseudosquares[i].Lp <= n; i++)
        {
            fmpz_set_ui(fbase, pseudosquares[i].p);
            fmpz_powm(fres, fbase, fe, fn);

            if (fmpz_cmp(fres, fminus1) == 0)
                return true;
            if (fmpz_cmp(fres, fone) != 0)
                return false;
        }
    }

    return true;
}

void initialize(uint128_t stop,
                uint64_t& delta,
                uint64_t& s,
                uint64_t& p,
                uint64_t& Lp,
                bool is_print)
{
    // In Sorenson's paper the segment size is named ∆,
    // with ∆ = s / log(n). s is the upper bound for
    // sieving, we sieve using the sieving primes <= s.
    delta = get_segment_size(stop);

    double log_stop = std::log(stop);
    log_stop = std::max(1.0, log_stop);
    s = delta * log_stop;

    // Pick the prime p so that Lp such that n/s < Lp
    for (const auto& pss : pseudosquares)
    {
        p = pss.p;
        Lp = pss.Lp;
        if (Lp > stop / s)
            break;
    }

    if (is_print)
    {
        std::cout << "Sieve size: " << delta << " bytes" << std::endl;
        std::cout << "s: " << s << " (max sieving prime)" << std::endl;
        std::cout << "p: " << p << " (pseudosquare prime)" << std::endl;
        std::cout << "Lp: " << Lp << " (pseudosquare)" << std::endl;
    }
}

void initialize_fmpz(fmpz_t fn,
                     fmpz_t fe,
                     fmpz_t fone,
                     fmpz_t fminus1,
                     fmpz_t fbase,
                     fmpz_t fres,
                     fmpz_t ftmp)
{
    fmpz_init(fbase);
    fmpz_init(fres);
    fmpz_init(fminus1);
    fmpz_init(ftmp);
    fmpz_init2(fn, 2);
    fmpz_init2(fe, 2);
    fmpz_set_ui(fone, 1);
}

// Sieve primes inside [start, stop]
uint64_t pseudosquares_prime_sieve(uint128_t start,
                                   uint128_t stop,
                                   bool is_print)
{
    if (start < 2)
        start = 2;
    if (start > stop)
        return 0;

    // After having run sieving and the pseudosquares prime
    // test, one has to remove perfect powers. Our implementation
    // misses this final step and is hence limited to
    // n <= 6.4 * 10^37 (according to Sorenson's paper).
    if ((double) stop > 6e37)
        throw std::runtime_error("stop must be <= 6 * 10^37");

    // Same variable names as in Sorenson's paper
    uint64_t delta, s, p, Lp;
    initialize(stop, delta, s, p, Lp, is_print);
    std::vector<bool> sieve(delta);

    // Flint arbitrary integer variables used
    // in pseudosquares_prime_test()
    fmpz_t fn, fe, fone, fminus1, fbase, fres, ftmp;
    initialize_fmpz(fn, fe, fone, fminus1, fbase, fres, ftmp);

    uint64_t count = 0;
    uint64_t sqrt_stop = (uint64_t) std::sqrt(stop);
    uint64_t max_sieving_prime = std::min(s, sqrt_stop);
    std::vector<SievingPrime> sieving_primes = get_sieving_primes(max_sieving_prime);

    for (uint128_t low = start; low <= stop; low += sieve.size())
    {
        // Sieve current segment [low, high]
        uint128_t high = low + sieve.size() - 1;
        high = std::min(high, stop);
        uint64_t sqrt_high = (uint64_t) std::sqrt(high);
        uint64_t max_i = uint64_t(high - low) + 1;
        max_sieving_prime = std::min(s, sqrt_high);
        std::fill(sieve.begin(), sieve.end(), true);

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
                uint128_t pp = uint128_t(prime) * prime;
                n += prime * (n < low);
                n = std::max(n, pp);
                i = uint64_t(n - low);
            }

            // Cross-off the multiples of prime
            for (; i < max_i; i += prime)
                sieve[i] = false;

            sp.i = i - max_i;
        }

        // Count primes inside [low, high]
        for (uint128_t n = low; n <= std::min(high, stop); n++)
        {
            if (sieve[n - low])
            {
                if (max_sieving_prime >= sqrt_high)
                    count++;
                else if (pseudosquares_prime_test(n, p, fn, fe, fone, fminus1, fbase, fres, ftmp))
                    count++;
            }
        }
    }

    fmpz_clear(fn);
    fmpz_clear(fe);
    fmpz_clear(fone);
    fmpz_clear(fminus1);
    fmpz_clear(fbase);
    fmpz_clear(fres);
    fmpz_clear(ftmp);

    return count;
}

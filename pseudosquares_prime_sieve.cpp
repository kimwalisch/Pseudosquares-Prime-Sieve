#include <iostream>
#include <cmath>
#include <cstdlib>
#include <array>
#include <vector>
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

// Generate sieving primes <= n
std::vector<uint64_t> get_sieving_primes(uint64_t n)
{
    std::vector<bool> sieve(n + 1, true);

    for (uint64_t i = 3; i * i <= n; i += 2)
        if (sieve[i])
            for (uint64_t j = i * i; j <= n; j += i * 2)
                sieve[j] = false;

    std::vector<uint64_t> sieving_primes;
    sieving_primes.push_back(2);

    for (uint64_t i = 3; i <= n; i += 2)
        if (sieve[i])
            sieving_primes.push_back(i);

    return sieving_primes;
}

// Modular exponentiation
uint64_t mod_pow(uint64_t base, uint64_t exponent, uint64_t modulus)
{
    if (modulus == 1)
        return 0;

    uint64_t res = 1;
    base %= modulus;

    while (exponent > 0)
    {
        if (exponent & 1)
            res = uint64_t((res * __uint128_t(base)) % modulus);

        base = uint64_t((base * __uint128_t(base)) % modulus);
        exponent >>= 1;
    }

    return res;
}

// Sorenson's Pseudosquares Prime Test
bool pseudosquares_prime_test(uint64_t n, int p)
{
    uint64_t exponent = (n - 1) / 2;
    uint64_t count_res_minus_1 = 0;

    // Condition (4) for n ≡ 5 mod 8: 2^((n−1)/2) ≡ −1 mod n
    if (n % 8 == 5 && mod_pow(2, exponent, n) != n - 1)
        return false;

    // Condition (3): for all pi ≤ p: pi^((n−1)/2) ≡ ±1 mod n
    for (uint64_t i = 0; primes[i] <= p; i++)
    {
        uint64_t res = mod_pow(primes[i], exponent, n);

        if (res != 1 && res != n - 1)
            return false; // Violates ±1 requirement
        if (res == n - 1)
            count_res_minus_1++;
    }

    // Condition (4) for n ≡ 1 mod 8: must have at least one res ≡ -1
    if (n % 8 == 1)
    {
        if (count_res_minus_1 > 0)
            return true;
        else
        {
            // check all q > p: q^((n−1)/2) ≡ ±1 mod n
            //     while Lq <= n    
            //
            // This case is missing in Sorenson's 
            // "The Pseudosquares Prime Sieve" paper.
            // Sorenson acknowledged the bug and shared
            // this fix in an email.
            for (uint64_t i = prime_pi[p] + 1; i < pseudosquares.size() && pseudosquares[i].Lp <= n; i++)
            {
                uint64_t res = mod_pow(pseudosquares[i].p, exponent, n);

                if (res == n - 1)
                    return true; // res ≡ -1 ⇒ prime
                if (res != 1)
                    return false; // composite
            }
        }
    }

    // Passed all tests — n is prime or a prime power
    return true;
}

// Sorenson's paper uses a semgent size of: s / log(n).
// However, the paper also mentions that using a
// larger segment size improves performance. Hence,
// we use a segment size of O(n^(1/4)).
//
uint64_t get_segment_size(uint64_t start, uint64_t stop)
{
    uint64_t segment_size = 1 << 14;
    uint64_t root4_stop = (uint64_t) std::sqrt(std::sqrt(stop));
    segment_size = std::max(segment_size, root4_stop);
    segment_size = std::min(segment_size, (stop - start) + 1);
    return segment_size;
}

int main(int argc, char** argv)
{
    uint64_t start = uint64_t(1e10);
    uint64_t stop = start + uint64_t(1e6);

    if (argc > 1)
        start = std::atoll(argv[1]);
    if (argc > 2)
        stop = std::atoll(argv[2]);
    if (stop < start)
      stop = start + uint64_t(1e6);

    std::cout << "Sieving primes inside [" << start << ", " << stop << "]" << std::endl;

    // We use s as sieve array size.
    // This is slightly different from Sorenson's paper where
    // s = delta * log(n) and where delta is used as sieve
    // size. However, the paper also mentions that using a
    // larger segment size improves performance. Hence, we use
    // a segment size of O(n^(1/4)) to improve performance.
    uint64_t s = get_segment_size(start, stop);
    std::vector<bool> sieve(s);

    // Variables used in Sorenson's paper
    uint64_t p = 0;
    uint64_t Lp = 0;

    // Pick the prime p so that Lp such that n/s < Lp
    for (const auto& pss : pseudosquares)
    {
        p = pss.p;
        Lp = pss.Lp;
        if (Lp > stop / s)
            break;
    }

    std::cout << "s: " << s << " (sieve size in bytes)" << std::endl; 
    std::cout << "p: " << p << std::endl;
    std::cout << "Lp: " << Lp << std::endl;

    uint64_t sqrt_stop = (uint64_t) std::sqrt(stop);
    uint64_t max_sieving_prime = std::min(s, sqrt_stop);
    const std::vector<uint64_t> sieving_primes = get_sieving_primes(max_sieving_prime);
    uint64_t count = 0;

    for (uint64_t low = start; low <= stop; low += sieve.size())
    {
        uint64_t sqrt_low = (uint64_t) std::sqrt(low);
        uint64_t high = std::min(low + sieve.size() - 1, stop);
        uint64_t limit = std::min((uint64_t) sieve.size(), sqrt_low);
        std::fill(sieve.begin(), sieve.end(), true);

        // Sieve out multiple of primes <= s
        for (uint64_t prime : sieving_primes)
        {
            if (prime > limit) break;
            uint64_t q = low / prime;
            uint64_t n = q * prime;
            n += prime * (n < low);
            n = std::max(n, prime * prime);

            // Cross-off multiples inside [low, high]
            for (; n <= high; n += prime)
                sieve[n - low] = false;
        }

        for (uint64_t i = 0; i < sieve.size() && low + i <= stop; i++)
            if (sieve[i] && pseudosquares_prime_test(low + i, p))
                count++;
    }

    std::cout << "\nPrimes: " << count << std::endl;

    return 0;
}

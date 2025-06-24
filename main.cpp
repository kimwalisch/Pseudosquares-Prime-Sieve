#include "pseudosquares_prime_sieve.hpp"
#include "calculator.hpp"

#include <omp.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <stdint.h>

namespace {

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

} // namespace

int main(int argc, char** argv)
{
    if (argc <= 1)
    {
        std::cout << "Usage: pseudosquares_prime_sieve [START] STOP"
                  << "Sieve the primes inside [START, STOP] (<= 10^34) using"
                  << "J. P. Sorenson's Pseudosquares Prime Sieve."
                  << std::endl;
    }

    try
    {
        uint128_t start = 0;
        uint128_t stop = 0;

        if (argc <= 2)
        {
            stop = calculator::eval<uint128_t>(argv[1]);
            std::cout << "Sieving primes inside [0, " << argv[1] << "]" << std::endl;
        }
        else
        {
            start = calculator::eval<uint128_t>(argv[1]);
            stop  = calculator::eval<uint128_t>(argv[2]);
            std::cout << "Sieving primes inside [" << argv[1] << ", " << argv[2] << "]" << std::endl;
        }

        auto t1 = std::chrono::system_clock::now();
        uint64_t count = 0;

        if (start <= stop)
        {
            uint64_t delta = get_segment_size(stop);
            double s = delta * std::log(std::max(10.0, (double) stop));
            double t = std::min((stop - start) / s, (double) omp_get_max_threads());
            int threads = (int) std::max(1.0, t);
            uint128_t thread_dist = ((stop - start) / threads) + 1;

            std::cout << "Thread dist: " << thread_dist << std::endl;
            std::cout << "Threads: " << threads << std::endl;
            std::cout << std::endl;

            #pragma omp parallel for num_threads(threads) reduction(+: count)
            for (int i = 0; i < threads; i++)
            {
                // Sieve primes inside [low, high]
                uint64_t low = start + i * thread_dist;
                uint64_t high = low + thread_dist - 1;
                bool verbose = (i == 0);

                count += pseudosquares_prime_sieve(low, high, verbose);
            }
        }

        auto t2 = std::chrono::system_clock::now();
        std::chrono::duration<double> seconds = t2 - t1;

        std::cout << "\nPrimes: " << count << std::endl;
        std::cout << "Seconds: " << std::fixed << std::setprecision(3) << seconds.count() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

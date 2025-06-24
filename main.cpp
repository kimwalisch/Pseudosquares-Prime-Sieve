#include "pseudosquares_prime_sieve.hpp"
#include "CmdOptions.hpp"

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

void help(int exit_code)
{
    const std::string help_menu =
        "Usage: pseudosquares_prime_sieve [START] STOP\n"
        "Sieve the primes inside [START, STOP] (<= 10^34) using\n"
        "J. P. Sorenson's Pseudosquares Prime Sieve.\n"
        "\n"
        "Options:\n"
        "  -h, --help         Print this help menu.\n"
        "  -p, --print        Print primes to the standard output.\n"
        "  -t, --threads=NUM  Set the number of threads, NUM <= CPU cores.\n"
        "                     Default setting: use all available CPU cores.\n";

    std::cout << help_menu << std::endl;
    std::exit(exit_code);
}

int main(int argc, char** argv)
{
    try
    {
        CmdOptions opts = parseOptions(argc, argv);

        if (opts.numbers.empty())
            help(1);

        uint128_t start = 0;
        uint128_t stop = 0;

        if (opts.numbers.size() == 1)
        {
            stop = opts.numbers.at(0);
            std::string stop_str = opts.numbers_str.at(0);
            if (!opts.print_primes)
                std::cout << "Sieving primes inside [0, " << stop_str << "]" << std::endl;
        }
        else
        {
            start = opts.numbers.at(0);
            stop  = opts.numbers.at(1);
            std::string start_str = opts.numbers_str.at(0);
            std::string stop_str = opts.numbers_str.at(1);
            if (!opts.print_primes)
                std::cout << "Sieving primes inside [" << start_str << ", " << stop_str << "]" << std::endl;
        }

        auto t1 = std::chrono::system_clock::now();
        uint64_t count = 0;

        if (start <= stop)
        {
            int threads = opts.threads;

            if (threads < 1)
            {
                uint64_t delta = get_segment_size(stop);
                double s = delta * std::log(std::max(10.0, (double) stop));
                double max_threads = (double) omp_get_max_threads();
                double t = std::min((stop - start) / s, max_threads);
                threads = (int) std::max(1.0, t);
            }

            // Use only 1 thread with --print option
            if (opts.print_primes)
                threads = 1;

            uint128_t thread_dist = (stop - start) / threads + 1;

            if (!opts.print_primes)
            {
                std::cout << "Thread dist: " << thread_dist << std::endl;
                std::cout << "Threads: " << threads << std::endl;
                std::cout << std::endl;
            }

            #pragma omp parallel for num_threads(threads) reduction(+: count)
            for (int i = 0; i < threads; i++)
            {
                // Sieve primes inside [low, high]
                uint128_t low = start + i * thread_dist;
                uint128_t high = low + thread_dist - 1;
                high = std::min(high, stop);
                bool verbose = (i == 0) && !opts.print_primes;

                count += pseudosquares_prime_sieve(low, high, opts.print_primes, verbose);
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

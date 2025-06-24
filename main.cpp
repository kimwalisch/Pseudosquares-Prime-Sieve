///
/// @file   main.cpp
/// @brief  Command-line program which uses the Pseudosquares Prime
///         Sieve algorithm to generate primes ≤ 1.23 * 10^34.
///         The algorithm has been parallelized using std::async.
///
/// Copyright (C) 2025 Kim Walisch, <kim.walisch@gmail.com>
///
/// This file is distributed under the BSD License. See the COPYING
/// file in the top level directory.
///

#include "pseudosquares_prime_sieve.hpp"
#include "CmdOptions.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <future>
#include <iomanip>
#include <iostream>
#include <stdint.h>
#include <thread>
#include <vector>

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

namespace {

double get_min_thread_dist(uint128_t stop)
{
    double min_thread_dist = 1e4;
    double root5_stop = std::pow(stop, 1.0 / 5.0);
    min_thread_dist = std::max(min_thread_dist, root5_stop);
    return min_thread_dist;
}

} // namespace

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
            int max_threads = std::thread::hardware_concurrency();
            max_threads = std::max(1, max_threads);

            if (threads > 1)
                threads = std::min(threads, max_threads);
            else
            {
                double min_thread_dist = get_min_thread_dist(stop);
                double t = (stop - start) / min_thread_dist;
                t = std::min(t, (double) max_threads);
                threads = (int) std::max(1.0, t);
            }

            if (opts.print_primes)
                threads = 1;

            uint128_t thread_dist = (stop - start) / threads + 1;

            if (!opts.print_primes)
            {
                std::cout << "Thread dist: " << thread_dist << std::endl;
                std::cout << "Threads: " << threads << std::endl;
                std::cout << std::endl;
            }

            std::vector<std::future<uint64_t>> futures;
            futures.reserve(threads);

            for (int i = 0; i < threads; i++)
            {
                uint128_t low = start + i * thread_dist;
                uint128_t high = low + thread_dist - 1;
                high = std::min(high, stop);
                bool verbose = (i == 0) && !opts.print_primes;

                futures.emplace_back(std::async(std::launch::async, [=]() {
                    return pseudosquares_prime_sieve(low, high, opts.print_primes, verbose);
                }));
            }

            for (auto& fut : futures)
                count += fut.get();
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

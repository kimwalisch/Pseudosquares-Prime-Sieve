#include "pseudosquares_prime_sieve.hpp"
#include "calculator.hpp"

#include <iostream>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <stdint.h>

int main(int argc, char** argv)
{
    if (argc <= 1)
    {
        std::cout << "Usage: pseudosquares_prime_sieve [START] STOP"
                  << "Sieve the primes inside [START, STOP] (< 2^64) using"
                  << "J. P. Sorenson's Pseudosquares Prime Sieve."
                  << std::endl;
    }

    try
    {
        uint64_t start = 0;
        uint64_t stop = 0;

        if (argc <= 2)
            stop = calculator::eval<uint64_t>(argv[1]);
        else
        {
            start = calculator::eval<uint64_t>(argv[1]);
            stop  = calculator::eval<uint64_t>(argv[2]);
        }

        auto t1 = std::chrono::system_clock::now();

        pseudosquares_prime_sieve(start, stop, true);

        auto t2 = std::chrono::system_clock::now();
        std::chrono::duration<double> seconds = t2 - t1;
        std::cout << "Seconds: " << std::fixed << std::setprecision(3) << seconds.count() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

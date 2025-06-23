# Pseudosquares Prime Sieve

This is a C++ implementation of J. P. Sorenson's Pseudosquares Prime Sieve algorithm, which is one of the few prime sieving algorithms that is well suited for generating primes > 2^64. The Pseudosquares Prime Sieve uses much less memory than most other prime sieving algorithms: it has a conjectured runtime complexity of O(n log n) operations and uses O((log n)^2) space.

# How to use it

You need to have installed a C++ compiler and the GNU make tool.

```bash
# Build and run tests
$ make
$ make check

# Count the primes inside [low, high]
$ ./pseudosquares_prime_sieve 1e15 1e15+1e7
```

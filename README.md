# Pseudosquares Prime Sieve

This is a C++ implementation of J. P. Sorenson's [Pseudosquares Prime Sieve](https://digitalcommons.butler.edu/cgi/viewcontent.cgi?article=1095&context=facsch_papers) algorithm, which is one of the few prime sieving algorithms that is well suited for generating primes > 2^64. The Pseudosquares Prime Sieve uses much less memory than most other prime sieving algorithms: it has a conjectured runtime complexity of O(n log n) operations and uses O((log n)^2) space.

## Prerequisites

You need to have installed a C++ compiler which supports 128-bit integers (either GNU GCC or LLVM/Clang), CMake ≥ 3.10 and the FLINT C library. The performance of the Pseudosquares Prime Sieve algorithm depends on fast modular exponentiation, which the FLINT C library provides.

<table>
    <tr>
        <td><b>Arch Linux:</b></td>
        <td><code>sudo pacman -S gcc cmake flint</code></td>
    </tr>
    <tr>
        <td><b>Debian/Ubuntu:</b></td>
        <td><code>sudo apt install g++ cmake libflint-dev</code></td>
    </tr>
    <tr>
        <td><b>Fedora:</b></td>
        <td><code>sudo dnf install gcc-c++ cmake flint-devel</code></td>
    </tr>
    <tr>
        <td><b>macOS:</b></td>
        <td><code>brew install cmake libomp flint</code></td>
    </tr>
    <tr>
        <td><b>openSUSE:</b></td>
        <td><code>sudo zypper install gcc-c++ cmake libflint-devel</code></td>
    </tr>
</table>

# Build instructions

```bash
# Build
$ cmake .
$ cmake --build . --parallel
```

# Usage examples

```bash
# Count the primes inside [low, high]
$ ./pseudosquares_prime_sieve 1e15 1e15+1e7

# Run tests
$ ./tests
```

# Pseudosquares Prime Sieve

[![Build status](https://github.com/kimwalisch/Pseudosquares-Prime-Sieve/actions/workflows/ci.yml/badge.svg)](https://github.com/kimwalisch/Pseudosquares-Prime-Sieve/actions/workflows/ci.yml)

This is a fast C++ implementation of J. P. Sorenson's [Pseudosquares Prime Sieve](https://digitalcommons.butler.edu/cgi/viewcontent.cgi?article=1095&context=facsch_papers) algorithm, which is one of the few prime sieving algorithms that is well suited for generating primes > $2^{64}$. The Pseudosquares Prime Sieve is a deterministic primality algorithm that outputs only proven primes, without relying on probabilistic assumptions. The Pseudosquares Prime Sieve uses much less memory than most other prime sieving algorithms: it has a conjectured runtime complexity of $O(n\cdot\log{n})$ operations and uses $O(\log^2{n})$ space.

The performance of the Pseudosquares Prime Sieve algorithm relies heavily on fast modular exponentiation of 128-bit integers, for which we are using the [hurchalla/modular_arithmetic](https://github.com/hurchalla/modular_arithmetic) C++ library. For fast generation of sieving primes we are using the [primesieve C/C++](https://github.com/kimwalisch/primesieve) library. Both libraries have been vendored (included directly) into the Pseudosquares-Prime-Sieve repository to simplify building the project and eliminate any external dependencies!

## Prerequisites

You need to have installed a C++ compiler which supports 128-bit integers (either GNU GCC or LLVM/Clang) and CMake ≥ 3.10.

<table>
    <tr>
        <td><b>Arch Linux:</b></td>
        <td><code>sudo pacman -S gcc cmake</code></td>
    </tr>
    <tr>
        <td><b>Debian/Ubuntu:</b></td>
        <td><code>sudo apt install g++ cmake</code></td>
    </tr>
    <tr>
        <td><b>Fedora:</b></td>
        <td><code>sudo dnf install gcc-c++ cmake</code></td>
    </tr>
    <tr>
        <td><b>macOS:</b></td>
        <td><code>brew install cmake</code></td>
    </tr>
    <tr>
        <td><b>openSUSE:</b></td>
        <td><code>sudo zypper install gcc-c++ cmake</code></td>
    </tr>
</table>

# Build instructions

```bash
cmake .
cmake --build . --parallel

# Run tests
./pseudosquares_prime_sieve --test
```

# Usage examples

The ```pseudosquares_prime_sieve``` program can generate primes ≤ $10^{33}$ using little memory. Our implementation uses $O(\sqrt[4.5]{n})$ memory. In practice, our implementation uses about 30 MiB of memory per thread when sieving near $10^{18}$ and about 33 MiB of memory per thread when sieving near $10^{30}$.

```bash
# Count primes inside [1e15 1e15+1e8] using all CPU cores
./pseudosquares_prime_sieve 1e15 1e15+1e8

# Count primes inside [1e15 1e15+1e8] using 4 threads
./pseudosquares_prime_sieve 1e15 -d1e8 --threads=4

# Print primes inside [1e25, 1e25+1e4] to stdout
./pseudosquares_prime_sieve 1e25 -d1e4 --print

# Store primes inside [1e25, 1e25+1e4] in a text file
./pseudosquares_prime_sieve 1e25 -d1e4 --print > primes.txt
```

# Command-line options

```
Usage: pseudosquares_prime_sieve [START] STOP
Sieve the primes inside [START, STOP] (<= 10^33) using
J. P. Sorenson's Pseudosquares Prime Sieve.

Options:
  -d, --dist=DIST    Sieve the interval [START, START + DIST].
  -h, --help         Print this help menu.
  -p, --print        Print primes to the standard output.
      --test         Run the unit tests.
  -t, --threads=NUM  Set the number of threads, NUM <= CPU cores.
                     Default setting: use all available CPU cores.
  -v, --version      Print version and license information.
```

# Benchmarks

Each run counts the primes in the interval [x, x + 10<sup>9</sup>], where x is the interval start.

<table>
  <tr align="center">
    <td><b>Interval</b></td>
    <td><b>Prime count</b></td>
    <td><b>Time elapsed</b></td>
  </tr>
  <tr align="right">
    <td>[10<sup>18</sup>, 10<sup>18</sup> + 10<sup>9</sup>]</td>
    <td>24,127,085</td>
    <td>3.962s</td>
  </tr>
  <tr align="right">
    <td>[10<sup>19</sup>, 10<sup>19</sup> + 10<sup>9</sup>]</td>
    <td>22,854,258</td>
    <td>4.769s</td>
  </tr>
  <tr align="right">
    <td>[10<sup>20</sup>, 10<sup>20</sup> + 10<sup>9</sup>]</td>
    <td>21,710,426</td>
    <td>15.973s</td>
  </tr>
  <tr align="right">
    <td>[10<sup>21</sup>, 10<sup>21</sup> + 10<sup>9</sup>]</td>
    <td>20,684,249</td>
    <td>17.089s</td>
  </tr>
  <tr align="right">
    <td>[10<sup>22</sup>, 10<sup>22</sup> + 10<sup>9</sup>]</td>
    <td>19,737,627</td>
    <td>19.231s</td>
  </tr>
  <tr align="right">
    <td>[10<sup>23</sup>, 10<sup>23</sup> + 10<sup>9</sup>]</td>
    <td>18,879,205</td>
    <td>20.599s</td>
  </tr>
  <tr align="right">
    <td>[10<sup>24</sup>, 10<sup>24</sup> + 10<sup>9</sup>]</td>
    <td>18,099,721</td>
    <td>22.671s</td>
  </tr>
  <tr align="right">
    <td>[10<sup>25</sup>, 10<sup>25</sup> + 10<sup>9</sup>]</td>
    <td>17,376,254</td>
    <td>20.599s</td>
  </tr>
  <tr align="right">
    <td>[10<sup>26</sup>, 10<sup>26</sup> + 10<sup>9</sup>]</td>
    <td>16,705,096</td>
    <td>24.358s</td>
  </tr>
  <tr align="right">
    <td>[10<sup>27</sup>, 10<sup>27</sup> + 10<sup>9</sup>]</td>
    <td>16,081,189</td>
    <td>26.722s</td>
  </tr>
  <tr align="right">
    <td>[10<sup>28</sup>, 10<sup>28</sup> + 10<sup>9</sup>]</td>
    <td>15,514,691</td>
    <td>26.968s</td>
  </tr>
  <tr align="right">
    <td>[10<sup>29</sup>, 10<sup>29</sup> + 10<sup>9</sup>]</td>
    <td>14,974,110</td>
    <td>29.775s</td>
  </tr>
  <tr align="right">
    <td>[10<sup>30</sup>, 10<sup>30</sup> + 10<sup>9</sup>]</td>
    <td>14,473,890</td>
    <td>31.075s</td>
  </tr>
  <tr align="right">
    <td>[10<sup>31</sup>, 10<sup>31</sup> + 10<sup>9</sup>]</td>
    <td>14,006,222</td>
    <td>29.830s</td>
  </tr>
  <tr align="right">
    <td>[10<sup>32</sup>, 10<sup>32</sup> + 10<sup>9</sup>]</td>
    <td>13,573,489</td>
    <td>32.867s</td>
  </tr>
  <tr align="right">
    <td>[10<sup>33</sup>, 10<sup>33</sup> + 10<sup>9</sup>]</td>
    <td>13,155,690</td>
    <td>31.554s</td>
  </tr>
</table>

Benchmarks were run on an Intel Core Ultra 5 245K with 14 CPU cores (from 2024) and the `pseudosquares_prime_sieve` program was compiled using GCC 16.

# Errors in Sorenson's paper

J. P. Sorenson's original 2006 [paper on the Pseudosquares Prime Sieve](https://digitalcommons.butler.edu/cgi/viewcontent.cgi?article=1095&context=facsch_papers) algorithm contains two minor errors, which Sorenson confirmed to me in a private communication. Below are fixes suggested by Sorenson for these two errors. Both of these fixes have been implemented in our ```pseudosquares_prime_sieve``` program.

## Error: min(s,sqrt(l))

```C++
//** Sieve by integers d up to s, gcd(d,m)=1
int d, m=W.size();
// m is the size of the wheel
for(d=W[p%m].next; d<=min(s,sqrt(l)); d=d+W[d%m].next)
    // Loop through multiples of d:
        for(x=B.first(d); x<=r; x=x+d)
            B.clear(x);
```

In the code snippet above from Sorenson's paper, one sieves using the integers ≤ `min(s,sqrt(l))`. The variable `l` (left) corresponds to the lower bound of the current segment, but its use is incorrect here. One needs to use the current segment's upper bound instead, which is named `r` (right) in Sorenson's paper:

```C++
//** Sieve by integers d up to s, gcd(d,m)=1
int d, m=W.size();
// m is the size of the wheel
for(d=W[p%m].next; d<=min(s,sqrt(r)); d=d+W[d%m].next)
    // Loop through multiples of d:
        for(x=B.first(d); x<=r; x=x+d)
            B.clear(x);
```

## Error in Condition 4 of Lemma 3.1

$p_i^{(n-1)/2} \equiv -1 \pmod{n}$ for some $p_i \leq p$ when $n \equiv 1 \pmod{8}$

Some numbers $n \equiv 1 \pmod{8}$ can be primes but without any -1 result in the formula above. For example 10001584849 is such a prime. The formula above from Sorenson's paper fails to detect such primes. In order to fix this issue, we add the following checks for numbers $n \equiv 1 \pmod{8}$ without any -1 result:

Start testing $q^{(n-1)/2} \pmod{n}$ for primes $q$ above $p$, one at a time, until either:

* You get a -1 result, in which case $n$ is prime or a prime power.
* You get something other than a ±1 result, in which case $n$ is not prime.
* You can't keep getting +1 results forever, unless $n$ is a perfect power. According to Sorenson's paper this case can only occur if $n > 6.4 \cdot 10^{37}$.
* You stop once $n > L_p$, in which case $n$ is a prime or a prime power.

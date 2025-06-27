///
/// @file  Sieve.hpp
/// @brief This file contains lookup tables that are
///        needed to implement a prime sieving algorithm where each
///        bit corresponds to an integer that is not divisible by 2,
///        3 and 5. The 8 bits of each byte correspond to the offsets
///        { 1, 7, 11, 13, 17, 19, 23, 29 }. Since the sieve array
///        uses the uint64_t data type, one sieve array element
///        (8 bytes) corresponds to an interval of size 30 * 8 = 240.
///
/// Copyright (C) 2025 Kim Walisch, <kim.walisch@gmail.com>
///
/// This file is distributed under the BSD License. See the COPYING
/// file in the top level directory.
///

#ifndef SIEVE_HPP
#define SIEVE_HPP

#include "macros.hpp"
#include "Vector.hpp"

#include <stdint.h>

namespace {

class Sieve
{
public:
    Sieve(std::size_t size)
        : size_(size),
          sieve_((size + 15) / 16)
    { }

    std::size_t size() const
    {
        return size_;
    }

    void set_all_bits()
    {
        std::fill(sieve_.begin(), sieve_.end(), 0xff);
    }

    ALWAYS_INLINE bool operator[](std::size_t i) const
    {
        return (sieve_[i >> 4] & is_bit_[i & 15]) != 0;
    }

    ALWAYS_INLINE void unset_bit(std::size_t i)
    {
        sieve_[i >> 4] &= unset_bit_[i & 15];
    }

private:
    std::size_t size_;
    Vector<uint8_t> sieve_;
    static const Array<uint64_t, 16> is_bit_;
    static const Array<uint64_t, 16> unset_bit_;
};

const Array<uint64_t, 16> Sieve::is_bit_ =
{
    (1ull << 0),(1ull << 0),
    (1ull << 1),(1ull << 1),
    (1ull << 2),(1ull << 2),
    (1ull << 3),(1ull << 3),
    (1ull << 4),(1ull << 4),
    (1ull << 5),(1ull << 5),
    (1ull << 6),(1ull << 6),
    (1ull << 7),(1ull << 7)
};

const Array<uint64_t, 16> Sieve::unset_bit_ =
{
    ~(1ull << 0), ~(1ull << 0),
    ~(1ull << 1), ~(1ull << 1),
    ~(1ull << 2), ~(1ull << 2),
    ~(1ull << 3), ~(1ull << 3),
    ~(1ull << 4), ~(1ull << 4),
    ~(1ull << 5), ~(1ull << 5),
    ~(1ull << 6), ~(1ull << 6),
    ~(1ull << 7), ~(1ull << 7)
};

} // namespace

#endif

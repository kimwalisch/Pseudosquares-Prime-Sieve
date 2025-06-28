///
/// @file  Sieve.hpp
/// @brief This is a bit sieve array where each bit corresponds to an
///        odd integer. Hence, each byte (8 bits) corresponds to an
///        interval of size 16.
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

    static std::size_t numbers_per_byte()
    {
        return 16;
    }

    void set_all_bits()
    {
        std::fill(sieve_.begin(), sieve_.end(), 0xff);
    }

    ALWAYS_INLINE bool operator[](std::size_t i) const
    {
        ASSERT(i < size_);
        return sieve_[i >> 4] & is_bit_[i & 15];
    }

    ALWAYS_INLINE void unset_bit(std::size_t i)
    {
        ASSERT(i < size_);
        sieve_[i >> 4] &= unset_bit_[i & 15];
    }

private:
    std::size_t size_;
    Vector<uint8_t> sieve_;
    static const Array<std::size_t, 16> is_bit_;
    static const Array<std::size_t, 16> unset_bit_;
};

const Array<std::size_t, 16> Sieve::is_bit_ =
{
    (1 << 0),(1 << 0),
    (1 << 1),(1 << 1),
    (1 << 2),(1 << 2),
    (1 << 3),(1 << 3),
    (1 << 4),(1 << 4),
    (1 << 5),(1 << 5),
    (1 << 6),(1 << 6),
    (1 << 7),(1 << 7)
};

const Array<std::size_t, 16> Sieve::unset_bit_ =
{
    ~(1u << 0), ~(1u << 0),
    ~(1u << 1), ~(1u << 1),
    ~(1u << 2), ~(1u << 2),
    ~(1u << 3), ~(1u << 3),
    ~(1u << 4), ~(1u << 4),
    ~(1u << 5), ~(1u << 5),
    ~(1u << 6), ~(1u << 6),
    ~(1u << 7), ~(1u << 7)
};

} // namespace

#endif

///
/// @file   modow.hpp
/// @brief  Fast modular exponentiation of 64-bit and 128-bit
///         integers using the hurchalla/modular_arithmetic library:
///         https://github.com/hurchalla/modular_arithmetic
///
/// Copyright (C) 2025 Kim Walisch, <kim.walisch@gmail.com>
///
/// This file is distributed under the BSD License. See the COPYING
/// file in the top level directory.
///

#ifndef MODPOW_HPP
#define MODPOW_HPP

#include "int128_t.hpp"
#include "macros.hpp"

#include <hurchalla/modular_arithmetic/modular_pow.h>
#include <hurchalla/montgomery_arithmetic/MontgomeryForm.h>
#include <hurchalla/montgomery_arithmetic/montgomery_form_aliases.h>
#include <hurchalla/montgomery_arithmetic/detail/platform_specific/montgomery_two_pow.h>

#include <stdint.h>

namespace {

/// modpow<2>(e, m) = 2^e mod m
template <int two>
uint128_t modpow(uint128_t exponent, uint128_t modulus)
{
    static_assert(two == 2, "modpow: two != 2");

    // Montgomery modular exponentiation
    // requires that the modulus is odd.
    ASSERT(modulus % 2 == 1);
    ASSERT(exponent < modulus);

    if (modulus <= std::numeric_limits<uint64_t>::max() / 4)
    {
        uint64_t e = (uint64_t) exponent;
        uint64_t m = (uint64_t) modulus;
        hurchalla::MontgomeryQuarter<uint64_t> mf(m);
        auto res_montval = hurchalla::detail::montgomery_two_pow::call(mf, e);
        uint64_t res = mf.convertOut(res_montval);
        return res;
    }
    else if (modulus <= std::numeric_limits<uint64_t>::max())
    {
        uint64_t e = (uint64_t) exponent;
        uint64_t m = (uint64_t) modulus;
        hurchalla::MontgomeryForm<uint64_t> mf(m);
        auto res_montval = hurchalla::detail::montgomery_two_pow::call(mf, e);
        uint64_t res = mf.convertOut(res_montval);
        return res;
    }
    else
    {
        ASSERT(modulus <= std::numeric_limits<uint128_t>::max() / 4);
        hurchalla::MontgomeryQuarter<uint128_t> mf(modulus);
        auto res_montval = hurchalla::detail::montgomery_two_pow::call(mf, exponent);
        uint128_t res = mf.convertOut(res_montval);
        return res;
    }
}

uint128_t modpow(uint64_t base, uint128_t exponent, uint128_t modulus)
{
    // Montgomery modular exponentiation
    // requires that the modulus is odd.
    ASSERT(modulus % 2 == 1);
    ASSERT(exponent < modulus);

    if (modulus <= std::numeric_limits<uint64_t>::max() / 4)
    {
        uint64_t e = (uint64_t) exponent;
        uint64_t m = (uint64_t) modulus;
        hurchalla::MontgomeryQuarter<uint64_t> mf(m);
        auto base_montval = mf.convertIn(base);
        auto res_montval = mf.pow(base_montval, e);
        uint64_t res = mf.convertOut(res_montval);
        return res;
    }
    else if (modulus <= std::numeric_limits<uint64_t>::max())
    {
        uint64_t e = (uint64_t) exponent;
        uint64_t m = (uint64_t) modulus;
        hurchalla::MontgomeryForm<uint64_t> mf(m);
        auto base_montval = mf.convertIn(base);
        auto res_montval = mf.pow(base_montval, e);
        uint64_t res = mf.convertOut(res_montval);
        return res;
    }
    else
    {
        // Our Pseudosquares Prime Sieve implementation
        // is limited by n (modulus) <= 1.73 * 10^33
        ASSERT(modulus <= std::numeric_limits<uint128_t>::max() / 4);
        hurchalla::MontgomeryQuarter<uint128_t> mf(modulus);
        auto base_montval = mf.convertIn(base);
        auto res_montval = mf.pow(base_montval, exponent);
        uint128_t res = mf.convertOut(res_montval);
        return res;
    }
}

} // namespace

#endif

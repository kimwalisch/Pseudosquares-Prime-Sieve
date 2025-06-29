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

#include <stdint.h>

namespace {

/// We use the hurchalla/modular_arithmetic
/// library for doing modular exponentiations
/// of 64-bit and 128-bit integers.
///
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
        // is limited by n (modulus) <= 1.23 * 10^34
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

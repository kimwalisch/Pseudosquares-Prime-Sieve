///
/// @file   modpow.hpp
/// @brief  Fast modular exponentiation of 64-bit and 128-bit
///         integers using the hurchalla/modular_arithmetic library:
///         https://github.com/hurchalla/modular_arithmetic
///
/// Copyright (C) 2026 Kim Walisch, <kim.walisch@gmail.com>
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

/// Return the canonical Montgomery value of 2^e mod mf.getModulus().
template <int two, typename Montgomery>
typename Montgomery::CanonicalValue modpow(const Montgomery& mf,
                                            typename Montgomery::IntegerType exponent)
{
    static_assert(two == 2, "modpow: two != 2");

    // Montgomery modular exponentiation
    // requires that the modulus is odd.
    ASSERT(mf.getModulus() % 2 == 1);
    ASSERT(exponent < mf.getModulus());

    auto res_montval = mf.two_pow(exponent);
    return mf.getCanonicalValue(res_montval);
}

/// Return the canonical Montgomery value of base^e mod mf.getModulus().
template <typename Montgomery>
typename Montgomery::CanonicalValue modpow(const Montgomery& mf,
                                            uint64_t base,
                                            typename Montgomery::IntegerType exponent)
{
    using T = typename Montgomery::IntegerType;

    // Montgomery modular exponentiation
    // requires that the modulus is odd.
    ASSERT(mf.getModulus() % 2 == 1);
    ASSERT(exponent < mf.getModulus());

    auto base_montval = mf.convertIn((T) base);
    auto res_montval = mf.pow(base_montval, exponent);
    return mf.getCanonicalValue(res_montval);
}

} // namespace

#endif

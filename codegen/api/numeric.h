#pragma once
/* /////////////////////////////////////////////////////////////////////////////
//                          __    ___
//                         /\ \__/\_ \
//   _____   _____   __  __\ \ ,_\//\ \
//  /\ '__`\/\ '__`\/\ \/\ \\ \ \/ \ \ \
//  \ \ \_\ \ \ \_\ \ \ \_\ \\ \ \_ \_\ \_
//   \ \ ,__/\ \ ,__/\ \____/ \ \__\/\____\
//    \ \ \   \ \ \   \/___/   \/__/\/____/
//     \/_/    \/_/
//
//  pputl Preprocessor Utilities
//  Copyright (C) 2020 - 2022 Justin Collier <m@jpcx.dev>
//
//	   This program is free software: you can redistribute it and/or modify
//	   it under the terms of the GNU General Public License as published by
//	   the Free Software Foundation, either version 3 of the License, or
//	   (at your option) any later version.
//
//	   This program is distributed in the hope that it will be useful,
//	   but WITHOUT ANY WARRANTY; without even the implied warranty of
//	   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	   GNU General Public License for more details.
//                                                                            //
//  You should have received a copy of the GNU General Public License        ///
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.  ////
///////////////////////////////////////////////////////////////////////////// */

#include "codegen.h"
#include "config.h"
#include "lang.h"
#include "traits.h"
#include "type.h"

namespace api {

inline codegen::category<"numeric"> numeric;

extern codegen::def<"inc(...: n: uint) -> uint{n + 1}"> const&            inc;
extern codegen::def<"dec(...: n: uint) -> uint{n - 1}"> const&            dec;
extern codegen::def<"eqz(...: n: uint) -> uint{n == 0}"> const&           eqz;
extern codegen::def<"nez(...: n: uint) -> uint{n != 0}"> const&           nez;
extern codegen::def<"mul2(...: n: uint) -> uint{n * 2}"> const&           mul2;
extern codegen::def<"sqrt(...: n: uint) -> uint{sqrt(n)}"> const&         sqrt;
extern codegen::def<"pow2(...: n: uint) -> uint{n * n}"> const&           pow2;
extern codegen::def<"div2(...: n: uint) -> uint{n / 2}"> const&           div2;
extern codegen::def<"log2(...: n: uint) -> uint{log2 n}"> const&          log2;
extern codegen::def<"mod2(...: n: uint) -> uint{n % 2}"> const&           mod2;
extern codegen::def<"mod4(...: n: uint) -> uint{n % 4}"> const&           mod4;
extern codegen::def<"mod8(...: n: uint) -> uint{n % 8}"> const&           mod8;
extern codegen::def<"mod16(...: n: uint) -> uint{n % 16}"> const&         mod16;
extern codegen::def<"mod32(...: n: uint) -> uint{n % 32}"> const&         mod32;
extern codegen::def<"mod64(...: n: uint) -> uint{n % 64}"> const&         mod64;
extern codegen::def<"factor(...: n: uint) -> ...prime_factors(n)"> const& factor;
extern codegen::def<"binary(...: n: uint) -> tuple{bool...}"> const&      binary;
extern codegen::def<"decimal(...: n: tuple{bool...}) -> uint"> const&     decimal;

NIFTY_DECL(inc);
NIFTY_DECL(dec);
namespace detail {
extern codegen::def<>& eqz_0;
NIFTY_DECL(eqz_0);
} // namespace detail
NIFTY_DECL(eqz);
NIFTY_DECL(nez);
NIFTY_DECL(mul2);
NIFTY_DECL(sqrt);
NIFTY_DECL(pow2);
NIFTY_DECL(div2);
NIFTY_DECL(log2);
NIFTY_DECL(mod2);
NIFTY_DECL(mod4);
NIFTY_DECL(mod8);
NIFTY_DECL(mod16);
NIFTY_DECL(mod32);
NIFTY_DECL(mod64);
NIFTY_DECL(factor);
NIFTY_DECL(binary);
NIFTY_DECL(decimal);

inline codegen::end_category<"numeric"> numeric_end;

} // namespace api

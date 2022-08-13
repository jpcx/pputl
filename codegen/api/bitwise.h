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
#include "compare.h"
#include "config.h"
#include "lang.h"
#include "logic.h"
#include "traits.h"
#include "type.h"

namespace api {

inline codegen::category<"bitwise"> bitwise;

// TODO: bitzip

extern codegen::def<"bitget(...: v: int|uint|word, i: int|uint) -> v[i]: bool"> const& bitget;
extern codegen::def<
    "bitset(...: v: int|uint|word, i: int|uint, b: bool) -> (v[i] = b): word"> const& bitset;
// extern codegen::def<"bitflip(...: v: uint|int, i: int) -> (v[i] = !v[i]): typeof(v)"> const&
//                                                                         bitflip_;
// extern codegen::def<"bitnot(...: v: uint|int) -> ~v: typeof(v)"> const& bitnot_;
// extern codegen::def<"bitshift_left(...: v: uint|int, ct: ibase10) -> (v << i): typeof(v)"> const&
//     bitshift_left_;

NIFTY_DECL(bitget);
NIFTY_DECL(bitset);
// NIFTY_DECL(bitflip_);
// NIFTY_DECL(bitnot_);
// NIFTY_DECL(bitshift_left_);

inline codegen::end_category<"bitwise"> bitwise_end;

} // namespace api

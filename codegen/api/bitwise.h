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
#include "logic.h"
#include "traits.h"
#include "type.h"

namespace api {

inline codegen::category<"bitwise"> bitwise;

// TODO: bitzip

extern codegen::def<"bitget(...: v: uint|int, i: uint) -> v[i]: bool"> const& bitget_;
extern codegen::def<"bitset(...: v: uint|int, i: uint, b: bool) -> (v[i] = b): typeof(v)"> const&
    bitset_;
extern codegen::def<"bitflip(...: v: uint|int, i: uint) -> (v[i] = !v[i]): typeof(v)"> const&
                                                                        bitflip_;
extern codegen::def<"bitnot(...: v: uint|int) -> ~v: typeof(v)"> const& bitnot_;
// extern codegen::def<"bitshift_left(...: v: uint, i: uint) -> (v << i): typeof(v)"> const&
//     bitshift_left_;

NIFTY_DECL(bitget_);
NIFTY_DECL(bitset_);
NIFTY_DECL(bitflip_);
NIFTY_DECL(bitnot_);
// NIFTY_DECL(bitshift_left_);

inline codegen::end_category<"bitwise"> bitwise_end;

} // namespace api

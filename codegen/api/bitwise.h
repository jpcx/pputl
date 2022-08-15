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
#include "numeric.h"
#include "traits.h"
#include "type.h"

namespace api {

inline codegen::category<"bitwise"> bitwise;

// TODO: bitzip

extern codegen::def<"bitget(...: v: word, i: idec) -> v[i]: bool"> const&              bitget;
extern codegen::def<"bitset(...: v: word, i: idec, b: bool) -> word{v[i] = b}"> const& bitset;
extern codegen::def<"bitflip(...: v: word, i: idec) -> word{v[i] = !v[i]}"> const&     bitflip;
extern codegen::def<"bitnot(...: v: word) -> word{~v}"> const&                         bitnot;
// extern codegen::def<"bitsl(...: v: word, ct: idec) -> word{v << i}"> const&            bitsl;

NIFTY_DECL(bitget);
NIFTY_DECL(bitset);
NIFTY_DECL(bitflip);
NIFTY_DECL(bitnot);
// NIFTY_DECL(bitsl);

inline codegen::end_category<"bitwise"> bitwise_end;

} // namespace api

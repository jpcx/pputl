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
#include "util.h"

namespace api {

inline codegen::category<"bitwise"> bitwise;

extern codegen::def<"bdump(...: v: word) -> ...bool"> const&                bdump;
extern codegen::def<"bsll(...: v: word, n: idec) -> word{v << n}"> const&   bsll;
extern codegen::def<"bsrl(...: v: word, n: idec) -> word{v >> n}"> const&   bsrl;
extern codegen::def<"bsra(...: v: int, n: idec) -> int{v / 2**n}"> const&   bsra;
extern codegen::def<"bnot(...: v: word) -> word{~v}"> const&                bnot;
extern codegen::def<"band(...: a: word, b: word) -> int{a & b}"> const&     band;
extern codegen::def<"bor(...: a: word, b: word) -> int{a | b}"> const&      bor;
extern codegen::def<"bxor(...: a: word, b: word) -> int{a ^ b}"> const&     bxor;
extern codegen::def<"bnand(...: a: word, b: word) -> int{~(a & b)}"> const& bnand;
extern codegen::def<"bnor(...: a: word, b: word) -> int{~(a | b)}"> const&  bnor;
extern codegen::def<"bxnor(...: a: word, b: word) -> int{~(a ^ b)}"> const& bxnor;
extern codegen::def<"bget(...: v: word, i: idec) -> v[i]: bool"> const&     bget;
extern codegen::def<"bset(...: v: word, i: idec, b: bool) -> word{v[i] = b}"> const& bset;
extern codegen::def<"bflip(...: v: word, i: idec) -> word{v[i] = !v[i]}"> const& bflip;
extern codegen::def<"brotl(...: a: word, n: idec) -> word"> const&               brotl;
extern codegen::def<"brotr(...: a: word, n: idec) -> word"> const&               brotr;

NIFTY_DECL(bdump);
NIFTY_DECL(bsll);
NIFTY_DECL(bsrl);
NIFTY_DECL(bsra);
NIFTY_DECL(bnot);
NIFTY_DECL(band);
NIFTY_DECL(bor);
NIFTY_DECL(bxor);
NIFTY_DECL(bnand);
NIFTY_DECL(bnor);
NIFTY_DECL(bxnor);
NIFTY_DECL(bget);
NIFTY_DECL(bset);
NIFTY_DECL(bflip);
NIFTY_DECL(brotl);
NIFTY_DECL(brotr);

inline codegen::end_category<"bitwise"> bitwise_end;

} // namespace api

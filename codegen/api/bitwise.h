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

extern codegen::def<"bitsll(...: v: word, n: idec) -> word{v << n}"> const&   bitsll;
extern codegen::def<"bitsrl(...: v: word, n: idec) -> word{v >> n}"> const&   bitsrl;
extern codegen::def<"bitsra(...: v: int, n: idec) -> int{v / 2**n}"> const&   bitsra;
extern codegen::def<"bitnot(...: v: word) -> word{~v}"> const&                bitnot;
extern codegen::def<"bitand(...: a: word, b: word) -> int{a & b}"> const&     bitand_;
extern codegen::def<"bitor(...: a: word, b: word) -> int{a | b}"> const&      bitor_;
extern codegen::def<"bitxor(...: a: word, b: word) -> int{a ^ b}"> const&     bitxor;
extern codegen::def<"bitnand(...: a: word, b: word) -> int{~(a & b)}"> const& bitnand;
extern codegen::def<"bitnor(...: a: word, b: word) -> int{~(a | b)}"> const&  bitnor;
extern codegen::def<"bitxnor(...: a: word, b: word) -> int{~(a ^ b)}"> const& bitxnor;
extern codegen::def<"bitget(...: v: word, i: idec) -> v[i]: bool"> const&     bitget;
extern codegen::def<"bitset(...: v: word, i: idec, b: bool) -> word{v[i] = b}"> const&
    bitset;
extern codegen::def<"bitflip(...: v: word, i: idec) -> word{v[i] = !v[i]}"> const&
                                                                     bitflip;
extern codegen::def<"bitrotl(...: a: word, n: idec) -> word"> const& bitrotl;
extern codegen::def<"bitrotr(...: a: word, n: idec) -> word"> const& bitrotr;

NIFTY_DECL(bitsll);
NIFTY_DECL(bitsrl);
NIFTY_DECL(bitsra);
NIFTY_DECL(bitnot);
NIFTY_DECL(bitand_);
NIFTY_DECL(bitor_);
NIFTY_DECL(bitxor);
NIFTY_DECL(bitnand);
NIFTY_DECL(bitnor);
NIFTY_DECL(bitxnor);
NIFTY_DECL(bitget);
NIFTY_DECL(bitset);
NIFTY_DECL(bitflip);
NIFTY_DECL(bitrotl);
NIFTY_DECL(bitrotr);

inline codegen::end_category<"bitwise"> bitwise_end;

} // namespace api

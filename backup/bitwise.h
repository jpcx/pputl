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
//
#include "impl/math.h"

namespace api {

inline codegen::category<"bitwise"> bitwise;

extern codegen::def<"hexdump(...: word) -> enum<0|1|2|...|D|E|F>..."> const& hexdump;
extern codegen::def<"bitdump(...: word) -> bool..."> const&                  bitdump;
extern codegen::def<"bitsll(...: word, size=1) -> word"> const&              bitsll;
extern codegen::def<"bitsrl(...: word, size=1) -> word"> const&              bitsrl;
extern codegen::def<"bitsra(...: word, size=1) -> word"> const&              bitsra;
extern codegen::def<"bitnot(...: word) -> word"> const&                      bitnot;
extern codegen::def<"bitand(...: word, word) -> word"> const&                bitand_;
extern codegen::def<"bitor(...: word, word) -> word"> const&                 bitor_;
extern codegen::def<"bitxor(...: word, word) -> word"> const&                bitxor;
extern codegen::def<"bitnand(...: word, word) -> word"> const&               bitnand;
extern codegen::def<"bitnor(...: word, word) -> word"> const&                bitnor;
extern codegen::def<"bitxnor(...: word, word) -> word"> const&               bitxnor;
extern codegen::def<"bitget(...: word, ofs) -> bool"> const&                 bitget;
extern codegen::def<"bitset(...: word, ofs, bool) -> word"> const&           bitset;
extern codegen::def<"bitflip(...: word, ofs) -> word"> const&                bitflip;
extern codegen::def<"bitrotl(...: word, size=1) -> word"> const&             bitrotl;
extern codegen::def<"bitrotr(...: word, size=1) -> word"> const&             bitrotr;

NIFTY_DECL(hexdump);
NIFTY_DECL(bitdump);
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

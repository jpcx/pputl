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
#include "impl/range.h"

namespace api {

inline codegen::category<"bitwise"> bitwise;

extern codegen::def<"bdump(...: word) -> bool..."> const&         bdump;
extern codegen::def<"bsll(...: word, n=1: size) -> word"> const&  bsll;
extern codegen::def<"bsrl(...: word, n=1: size) -> word"> const&  bsrl;
extern codegen::def<"bsra(...: word, n=1: size) -> word"> const&  bsra;
extern codegen::def<"bnot(...: word) -> word"> const&             bnot;
extern codegen::def<"band(...: word, word) -> word"> const&       band;
extern codegen::def<"bor(...: word, word) -> word"> const&        bor;
extern codegen::def<"bxor(...: word, word) -> word"> const&       bxor;
extern codegen::def<"bnand(...: word, word) -> word"> const&      bnand;
extern codegen::def<"bnor(...: word, word) -> word"> const&       bnor;
extern codegen::def<"bxnor(...: word, word) -> word"> const&      bxnor;
extern codegen::def<"bget(...: word, idx) -> bool"> const&        bget;
extern codegen::def<"bset(...: word, idx, bool) -> word"> const&  bset;
extern codegen::def<"bflip(...: word, idx) -> word"> const&       bflip;
extern codegen::def<"brotl(...: word, n=1: size) -> word"> const& brotl;
extern codegen::def<"brotr(...: word, n=1: size) -> word"> const& brotr;

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

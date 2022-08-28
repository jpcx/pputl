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
//
#include "impl/traits/detail.h"

namespace api {
namespace impl {

inline codegen::category<"impl.traits"> traits;

extern codegen::def<"hex(v, t: {<atom>, IS}|{<hex>, NOT|DEC|INC|NYBL|BITS})"> const& hex;
extern codegen::def<
    "hexhex(v, t: {<atom>, IS}|{<hex##hex>, LT|AND|OR|XOR|SUB|ADD0|ADD1})"> const& hexhex;
extern codegen::def<"nybl(v, t: {<atom>, IS}|{<nybl>, HEX|BITS})"> const&          nybl;
extern codegen::def<"udec(v, t: {<atom>, IS}|{<udec>, UHEX|LOG2|SQRT|FACT})"> const& udec;
extern codegen::def<
    "uhex(v, t: {<atom>, IS}|{<uhex>, UDEC|UTUP|IHEX|ICAST|ILTZ|BNOT})"> const& uhex;
extern codegen::def<"arithhint(a, b: enum<IDEC|IHEX|UDEC|UHEX|UTUP>, "
                    "enum<IDEC|IHEX|UDEC|UHEX|UTUP>)"> const&                   arithhint;

NIFTY_DECL(hex);
NIFTY_DECL(hexhex);
NIFTY_DECL(nybl);
NIFTY_DECL(udec);
NIFTY_DECL(uhex);
NIFTY_DECL(arithhint);

inline codegen::end_category<"impl.traits"> traits_end;

} // namespace impl
} // namespace api

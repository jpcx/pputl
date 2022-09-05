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

inline std::string const arith_rules{
    "returns unsigned if either operand is unsigned, decimal if\n"
    "either operand is decimal (and the result is non-negative),\n"
    "utup if both operands are utup, and hex otherwise."};

extern std::string& hex_prefix;
extern std::string& nybl_prefix;
extern std::string& udec_prefix;
extern std::string& uhex_prefix;

extern codegen::def<
    "hex(v, t: enum<0|1|...|E|F>, enum<NOT|DEC0|DEC1|INC0|INC1|NYBL|BITS>)"> const& hex;
extern codegen::def<
    "hexhex(v, t: enum<00|01|...|FE|FF>, enum<LT|AND|OR|XOR|SUB0|SUB1|ADD0|ADD1>)"> const&
    hexhex;
extern codegen::def<"nybl(v, t: enum<0000|0001|...|1110|1111>, enum<HEX|BITS>)"> const&
    nybl;
extern std::conditional_t<
    (codegen::conf::word_size > 2 and codegen::conf::cpp20_arglimit),
    codegen::def<
        "udec(v, t: enum<0u|1u|...>, enum<UHEX|ISIZE|USIZE|IIDX|UIDX|LOG2|SQRT|FACT>)">,
    codegen::def<
        "udec(v, t: enum<0u|1u|...>, enum<UHEX|IIDX|UIDX|LOG2|SQRT|FACT>)">> const& udec;
extern codegen::def<"uhex(v, t: enum<...>, enum<UDEC|UTUP|IHEX|ICAST|ILTZ|BNOT>)"> const&
                                                              uhex;
extern codegen::def<"arithhint(a, b: enum<IDEC|IHEX|UDEC|UHEX|UTUP>, "
                    "enum<IDEC|IHEX|UDEC|UHEX|UTUP>)"> const& arithhint;
extern codegen::def<"xarithhint(...: enum<IDEC|IHEX|UDEC|UHEX|UTUP>, "
                    "enum<IDEC|IHEX|UDEC|UHEX|UTUP>)"> const& xarithhint;

NIFTY_DECL(hex_prefix);
NIFTY_DECL(nybl_prefix);
NIFTY_DECL(udec_prefix);
NIFTY_DECL(uhex_prefix);
NIFTY_DECL(hex);
NIFTY_DECL(hexhex);
NIFTY_DECL(nybl);
NIFTY_DECL(udec);
NIFTY_DECL(uhex);
NIFTY_DECL(arithhint);
NIFTY_DECL(xarithhint);

inline codegen::end_category<"impl.traits"> traits_end;

} // namespace impl
} // namespace api

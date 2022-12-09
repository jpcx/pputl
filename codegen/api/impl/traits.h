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
#include "obj_fwd.h"

namespace api {
namespace impl {

inline codegen::category<"impl.traits"> traits;

inline std::string const arith_rules{"returns unsigned if either operand is unsigned,\n"
                                     "decimal if either operand is decimal (and the\n"
                                     "result is non-negative), and hex otherwise."};

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
        "udec(v, t: enum<0u|1u|...>, enum<UHEX|ISIZE|USIZE|IOFS|UOFS|LOG2|SQRT|FACT>)">,
    codegen::def<
        "udec(v, t: enum<0u|1u|...>, enum<UHEX|IOFS|UOFS|LOG2|SQRT|FACT>)">> const& udec;
extern codegen::def<"uhex(v, t: enum<...>, enum<UDEC|IHEX|ICAST|ILTZ|BNOT|HDUMP>)"> const&
                                                                 uhex;
extern codegen::def<"arithhint(a, b: enum<IDEC|IHEX|UDEC|UHEX>, "
                    "enum<IDEC|IHEX|UDEC|UHEX>)"> const&         arithhint;
extern codegen::def<"xarithhint(...: enum<IDEC|IHEX|UDEC|UHEX>, "
                    "enum<IDEC|IHEX|UDEC|UHEX>)"> const&         xarithhint;
extern codegen::def<"hex_cat(...: enum<0|1|...|E|F>...)"> const& hex_cat;
extern codegen::def<"is_udec(sym)"> const&                       is_udec;
extern codegen::def<"is_uhex(sym)"> const&                       is_uhex;
extern codegen::def<"is_idec(sym)"> const&                       is_idec;
extern codegen::def<"is_ihex(sym)"> const&                       is_ihex;
extern codegen::def<"is_uint(...: sym)"> const&                  is_uint;
extern codegen::def<"is_int(...: sym)"> const&                   is_int;
extern codegen::def<"is_word(...: sym)"> const&                  is_word;
extern codegen::def<"is_obj(...: any)"> const&                   is_obj;

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
NIFTY_DECL(hex_cat);
NIFTY_DECL(is_udec);
NIFTY_DECL(is_uhex);
NIFTY_DECL(is_idec);
NIFTY_DECL(is_ihex);
NIFTY_DECL(is_uint);
NIFTY_DECL(is_int);
NIFTY_DECL(is_word);
NIFTY_DECL(is_obj);

inline codegen::end_category<"impl.traits"> traits_end;

} // namespace impl
} // namespace api

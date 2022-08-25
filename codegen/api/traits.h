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
#include "impl/traits.h"

namespace api {

inline codegen::category<"traits"> traits;

extern codegen::def<"is_any(...: list) -> bool"> const&                   is_any;
extern codegen::def<"is_none(...: list) -> bool"> const&                  is_none;
extern codegen::def<"is_obj(...: list) -> bool"> const&                   is_obj;
extern codegen::def<"is_atom(...: list) -> bool"> const&                  is_atom;
extern codegen::def<"is_enum(...: chkprefix: atom, list) -> bool"> const& is_enum;
extern codegen::def<"is_bool(...: list) -> bool"> const&                  is_bool;
extern codegen::def<"is_hex(...: list) -> bool"> const&                   is_hex;
extern codegen::def<"is_nybl(...: list) -> bool"> const&                  is_nybl;
extern codegen::def<"is_int(...: list) -> bool"> const&                   is_int;
extern codegen::def<"is_idec(...: list) -> bool"> const&                  is_idec;
extern codegen::def<"is_ihex(...: list) -> bool"> const&                  is_ihex;
extern codegen::def<"is_uint(...: list) -> bool"> const&                  is_uint;
extern codegen::def<"is_udec(...: list) -> bool"> const&                  is_udec;
extern codegen::def<"is_uhex(...: list) -> bool"> const&                  is_uhex;
extern codegen::def<"is_tup(...: list) -> bool"> const&                   is_tup;
extern codegen::def<"is_utup(...: list) -> bool"> const&                  is_utup;
extern codegen::def<"is_word(...: list) -> bool"> const&                  is_word;
extern codegen::def<"is_size(...: list) -> bool"> const&                  is_size;
extern codegen::def<
    "typeof(...: list) -> "
    "enum<LIST|NONE|UTUP|TUP|IDEC|IHEX|UDEC|UHEX|HEX|NYBL|ATOM>"> const& typeof;
extern codegen::def<"sizeof(...: list) -> udec&size"> const& sizeof_;

namespace detail {
extern codegen::def<>& is_any_o;
extern codegen::def<>& is_obj_o;
extern codegen::def<>& is_atom_o;
extern codegen::def<>& is_enum_o;
extern codegen::def<>& is_hex_o;
extern codegen::def<>& is_nybl_o;
extern codegen::def<>& is_int_o;
extern codegen::def<>& is_idec_o;
extern codegen::def<>& is_ihex_o;
extern codegen::def<>& is_uint_o;
extern codegen::def<>& is_udec_o;
extern codegen::def<>& is_uhex_o;
extern codegen::def<>& is_tup_o;
extern codegen::def<>& is_utup_o;
extern codegen::def<>& is_word_o;
extern codegen::def<>& is_size_o;
NIFTY_DECL(is_any_o);
NIFTY_DECL(is_obj_o);
NIFTY_DECL(is_atom_o);
NIFTY_DECL(is_enum_o);
NIFTY_DECL(is_hex_o);
NIFTY_DECL(is_nybl_o);
NIFTY_DECL(is_int_o);
NIFTY_DECL(is_idec_o);
NIFTY_DECL(is_ihex_o);
NIFTY_DECL(is_uint_o);
NIFTY_DECL(is_udec_o);
NIFTY_DECL(is_uhex_o);
NIFTY_DECL(is_tup_o);
NIFTY_DECL(is_utup_o);
NIFTY_DECL(is_word_o);
NIFTY_DECL(is_size_o);
} // namespace detail

NIFTY_DECL(is_any);
NIFTY_DECL(is_none);
NIFTY_DECL(is_obj);
NIFTY_DECL(is_atom);
NIFTY_DECL(is_enum);
NIFTY_DECL(is_bool);
NIFTY_DECL(is_hex);
NIFTY_DECL(is_nybl);
NIFTY_DECL(is_int);
NIFTY_DECL(is_idec);
NIFTY_DECL(is_ihex);
NIFTY_DECL(is_uint);
NIFTY_DECL(is_udec);
NIFTY_DECL(is_uhex);
NIFTY_DECL(is_tup);
NIFTY_DECL(is_utup);
NIFTY_DECL(is_word);
NIFTY_DECL(is_size);
NIFTY_DECL(typeof);
NIFTY_DECL(sizeof_);

inline codegen::end_category<"traits"> traits_end;

} // namespace api

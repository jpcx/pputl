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
#include "fwd.h"
#include "impl/traits.h"

namespace api {

inline codegen::category<"traits"> traits;

extern codegen::def<"is_list(...: list) -> enum<1>"> const&               is_list;
extern codegen::def<"is_none(...: list) -> bool"> const&                  is_none;
extern codegen::def<"is_object(...: list) -> bool"> const&                is_object;
extern codegen::def<"is_tuple(...: list) -> bool"> const&                 is_tuple;
extern codegen::def<"is_atom(...: list) -> bool"> const&                  is_atom;
extern codegen::def<"is_enum(...: chkprefix: atom, list) -> bool"> const& is_enum;
extern codegen::def<"is_idec(...: list) -> bool"> const&                  is_idec;
extern codegen::def<"is_bool(...: list) -> bool"> const&                  is_bool;
extern codegen::def<"is_ihex(...: list) -> bool"> const&                  is_ihex;
extern codegen::def<"is_udec(...: list) -> bool"> const&                  is_udec;
extern codegen::def<"is_uhex(...: list) -> bool"> const&                  is_uhex;
extern codegen::def<"is_int(...: list) -> bool"> const&                   is_int;
extern codegen::def<"is_uint(...: list) -> bool"> const&                  is_uint;
extern codegen::def<"is_word(...: list) -> bool"> const&                  is_word;
extern codegen::def<"is_size(...: list) -> bool"> const&                  is_size;
extern codegen::def<"is_offset(...: list) -> bool"> const&                is_offset;
extern codegen::def<"is_array(...: list) -> bool"> const&                 is_array;
extern codegen::def<"is_map(...: list) -> bool"> const&                   is_map;
extern codegen::def<"is_pqueue(...: list) -> bool"> const&                is_pqueue;
extern codegen::def<"is_set(...: list) -> bool"> const&                   is_set;
extern codegen::def<"is_stack(...: list) -> bool"> const&                 is_stack;
extern codegen::def<"is_queue(...: list) -> bool"> const&                 is_queue;
extern codegen::def<"is_range(...: list) -> bool"> const&                 is_range;
extern codegen::def<"is_any(...: list) -> bool"> const&                   is_any;
extern codegen::def<"typeof(...: list) -> "
                    "enum<NONE|LIST|TUPLE|ARRAY|MAP|PQUEUE|SET|STACK|QUEUE|ATOM|IDEC|"
                    "IHEX|UDEC|UHEX>"> const& typeof;
extern codegen::def<"countof(...: list) -> udec&size"> const& countof;

namespace detail {
extern codegen::def<>& is_tuple_o;
extern codegen::def<>& is_atom_o;
extern codegen::def<>& is_enum_o;
extern codegen::def<>& is_enum_oo;
extern codegen::def<>& is_idec_o;
extern codegen::def<>& is_ihex_o;
extern codegen::def<>& is_udec_o;
extern codegen::def<>& is_uhex_o;
extern codegen::def<>& is_int_o;
extern codegen::def<>& is_uint_o;
extern codegen::def<>& is_word_o;
extern codegen::def<>& is_size_o;
extern codegen::def<>& is_offset_o;
extern codegen::def<>& is_array_o;
extern codegen::def<>& is_array_oo;
extern codegen::def<>& is_map_o;
extern codegen::def<>& is_map_oo;
extern codegen::def<>& is_pqueue_o;
extern codegen::def<>& is_pqueue_oo;
extern codegen::def<>& is_set_o;
extern codegen::def<>& is_set_oo;
extern codegen::def<>& is_stack_o;
extern codegen::def<>& is_stack_oo;
extern codegen::def<>& is_queue_o;
extern codegen::def<>& is_queue_oo;
extern codegen::def<>& is_range_o;
NIFTY_DECL(is_tuple_o);
NIFTY_DECL(is_atom_o);
NIFTY_DECL(is_enum_o);
NIFTY_DECL(is_enum_oo);
NIFTY_DECL(is_idec_o);
NIFTY_DECL(is_ihex_o);
NIFTY_DECL(is_udec_o);
NIFTY_DECL(is_uhex_o);
NIFTY_DECL(is_int_o);
NIFTY_DECL(is_uint_o);
NIFTY_DECL(is_word_o);
NIFTY_DECL(is_size_o);
NIFTY_DECL(is_offset_o);
NIFTY_DECL(is_array_o);
NIFTY_DECL(is_array_oo);
NIFTY_DECL(is_map_o);
NIFTY_DECL(is_map_oo);
NIFTY_DECL(is_pqueue_o);
NIFTY_DECL(is_pqueue_oo);
NIFTY_DECL(is_set_o);
NIFTY_DECL(is_set_oo);
NIFTY_DECL(is_stack_o);
NIFTY_DECL(is_stack_oo);
NIFTY_DECL(is_queue_o);
NIFTY_DECL(is_queue_oo);
NIFTY_DECL(is_range_o);
} // namespace detail

NIFTY_DECL(is_list);
NIFTY_DECL(is_none);
NIFTY_DECL(is_object);
NIFTY_DECL(is_tuple);
NIFTY_DECL(is_atom);
NIFTY_DECL(is_enum);
NIFTY_DECL(is_idec);
NIFTY_DECL(is_bool);
NIFTY_DECL(is_ihex);
NIFTY_DECL(is_udec);
NIFTY_DECL(is_uhex);
NIFTY_DECL(is_int);
NIFTY_DECL(is_uint);
NIFTY_DECL(is_word);
NIFTY_DECL(is_size);
NIFTY_DECL(is_offset);
NIFTY_DECL(is_array);
NIFTY_DECL(is_map);
NIFTY_DECL(is_pqueue);
NIFTY_DECL(is_set);
NIFTY_DECL(is_stack);
NIFTY_DECL(is_queue);
NIFTY_DECL(is_range);
NIFTY_DECL(is_any);
NIFTY_DECL(typeof);
NIFTY_DECL(countof);

inline codegen::end_category<"traits"> traits_end;

} // namespace api

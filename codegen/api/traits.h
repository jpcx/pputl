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

// TODO: valueof

extern codegen::def<"is_any(...: any...) -> bool"> const&  is_any;
extern codegen::def<"is_none(...: any...) -> bool"> const& is_none;
extern codegen::def<"is_atom(...: any...) -> bool"> const& is_atom;
extern codegen::def<"is_idec(...: any...) -> bool"> const& is_idec;
extern codegen::def<"is_bool(...: any...) -> bool"> const& is_bool;
extern codegen::def<"is_ihex(...: any...) -> bool"> const& is_ihex;
extern codegen::def<"is_udec(...: any...) -> bool"> const& is_udec;
extern codegen::def<"is_uhex(...: any...) -> bool"> const& is_uhex;
// extern codegen::def<"is_int(...: any...) -> bool"> const&  is_int;

extern codegen::def<"is_tup(...: any...) -> bool"> const&  is_tup;
extern codegen::def<"is_pair(...: any...) -> bool"> const& is_pair;
/* extern codegen::def<"is_kv(...: any...) -> bool"> const&   is_kv; */
// extern codegen::def<"is_object(...: list) -> bool"> const&             is_object;
// extern codegen::def<"is_uint(...: list) -> bool"> const&               is_uint;
// extern codegen::def<"is_word(...: list) -> bool"> const&               is_word;
// extern codegen::def<"is_size(...: list) -> bool"> const&               is_size;
// extern codegen::def<"is_offset(...: list) -> bool"> const&             is_offset;
// extern codegen::def<"is_array(...: list) -> bool"> const&              is_array;
// extern codegen::def<"is_stack(...: list) -> bool"> const&              is_stack;
// extern codegen::def<"is_queue(...: list) -> bool"> const&              is_queue;
// extern codegen::def<"is_order(...: list) -> bool"> const&              is_order;
// extern codegen::def<"is_heap(...: list) -> bool"> const&               is_heap;
// extern codegen::def<"is_map(...: list) -> bool"> const&                is_map;
// extern codegen::def<"is_set(...: list) -> bool"> const&                is_set;
// extern codegen::def<"is_pqueue(...: list) -> bool"> const&             is_pqueue;
// extern codegen::def<"is_range(...: list) -> bool"> const&              is_range;
// extern codegen::def<"is_buffer(...: list) -> bool"> const&             is_buffer;
// extern codegen::def<"typeof(...: list) -> "
//                     "enum<NONE|LIST|TUPLE|ARRAY|STACK|QUEUE|HEAP|MAP|SET|PQUEUE|ATOM|"
//                     "IDEC|IHEX|UDEC|UHEX>"> const& typeof;
// extern codegen::def<"countof(...: list) -> udec&size"> const& countof;

namespace detail {
extern codegen::def<>& is_atom_o;
[[nodiscard]] inline auto
is_atom(codegen::arg any) {
  using codegen::pp::call;
  return call(call(call(is_atom_o(any), any), any), any);
}
extern codegen::def<>& is_bool_o;
//
extern codegen::def<>& is_pair_o;
/* extern codegen::def<>& is_kv_o; */
// extern codegen::def<>& is_size_o;
// extern codegen::def<>& is_offset_o;
// extern codegen::def<>& is_array_o;
// extern codegen::def<>& is_stack_o;
// extern codegen::def<>& is_queue_o;
// extern codegen::def<>& is_order_o;
// extern codegen::def<>& is_heap_o;
// extern codegen::def<>& is_map_o;
// extern codegen::def<>& is_set_o;
// extern codegen::def<>& is_pqueue_o;
// extern codegen::def<>& is_range_o;
// extern codegen::def<>& is_buffer_o;
NIFTY_DECL(is_atom_o);
NIFTY_DECL(is_bool_o);
//
NIFTY_DECL(is_pair_o);
/* NIFTY_DECL(is_kv_o); */
// NIFTY_DECL(is_ihex_o);
// NIFTY_DECL(is_udec_o);
// NIFTY_DECL(is_uhex_o);
// NIFTY_DECL(is_int_o);
// NIFTY_DECL(is_uint_o);
// NIFTY_DECL(is_word_o);
// NIFTY_DECL(is_size_o);
// NIFTY_DECL(is_offset_o);
// NIFTY_DECL(is_array_o);
// NIFTY_DECL(is_stack_o);
// NIFTY_DECL(is_queue_o);
// NIFTY_DECL(is_order_o);
// NIFTY_DECL(is_heap_o);
// NIFTY_DECL(is_map_o);
// NIFTY_DECL(is_set_o);
// NIFTY_DECL(is_pqueue_o);
// NIFTY_DECL(is_range_o);
// NIFTY_DECL(is_buffer_o);
} // namespace detail

NIFTY_DECL(is_any);
NIFTY_DECL(is_none);
NIFTY_DECL(is_atom);
NIFTY_DECL(is_idec);
NIFTY_DECL(is_bool);
NIFTY_DECL(is_ihex);
NIFTY_DECL(is_udec);
NIFTY_DECL(is_uhex);
// NIFTY_DECL(is_int);

NIFTY_DECL(is_tup);
NIFTY_DECL(is_pair);
/* NIFTY_DECL(is_kv); */
// NIFTY_DECL(is_object);
// NIFTY_DECL(is_uint);
// NIFTY_DECL(is_word);
// NIFTY_DECL(is_size);
// NIFTY_DECL(is_offset);
// NIFTY_DECL(is_array);
// NIFTY_DECL(is_stack);
// NIFTY_DECL(is_queue);
// NIFTY_DECL(is_order);
// NIFTY_DECL(is_heap);
// NIFTY_DECL(is_map);
// NIFTY_DECL(is_set);
// NIFTY_DECL(is_pqueue);
// NIFTY_DECL(is_range);
// NIFTY_DECL(is_buffer);
// NIFTY_DECL(typeof);
// NIFTY_DECL(countof);

inline codegen::end_category<"traits"> traits_end;

} // namespace api

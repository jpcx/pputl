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
#include "traits.h"

namespace api {

inline codegen::category<"type"> type;

// TODO: arr; tup construction from arr; arr construction from tup
//
// TODO: mem w/ hint

extern codegen::def<"list(...: list) -> list"> const&                            list;
extern codegen::def<"none(...: none) -> none"> const&                            none;
extern codegen::def<"obj(...: obj) -> obj"> const&                               obj;
extern codegen::def<"atom(...: obj) -> atom"> const&                             atom;
extern codegen::def<"enum(...: chkprefix: atom, enum<...>) -> enum<...>"> const& enum_;
extern codegen::def<"bool(...: bool) -> bool"> const&                            bool_;
extern codegen::def<"idec(...: word) -> idec"> const&                            idec;
extern codegen::def<"ihex(...: word) -> ihex"> const&                            ihex;
extern codegen::def<"udec(...: word) -> udec"> const&                            udec;
extern codegen::def<"uhex(...: word) -> uhex"> const&                            uhex;
extern codegen::def<"int(...: word, hint=AUTO: enum<IDEC|IHEX|AUTO>) -> int"> const& int_;
extern codegen::def<"tup(...: tup) -> tup"> const&                                   tup;
extern codegen::def<"uint(...: word, hint=AUTO: enum<UDEC|UHEX|AUTO>) -> uint"> const&
    uint;
extern codegen::def<
    "word(...: word, hint=AUTO: enum<IDEC|IHEX|UDEC|UHEX|AUTO>) -> word"> const& word;
extern codegen::def<
    "size(...: word, hint=AUTO: enum<IDEC|IHEX|UDEC|UHEX|AUTO>) -> size"> const& size;
extern codegen::def<
    "ofs(...: word, hint=AUTO: enum<IDEC|IHEX|UDEC|UHEX|AUTO>) -> ofs"> const& ofs;
extern codegen::def<"arr(...: o?: arr|tup) -> arr"> const&                     arr;
extern codegen::def<"map(...: o?: arr|tup) -> map"> const&                     map;
extern codegen::def<"set(...: o?: arr|tup) -> set"> const&                     set;
extern codegen::def<"stack(...: o?: arr|tup) -> stack"> const&                 stack;
extern codegen::def<"queue(...: o?: arr|tup) -> queue"> const&                 queue;
extern codegen::def<"pqueue(...: o?: arr|tup) -> pqueue"> const&               pqueue;
extern codegen::def<"any(...: any) -> any"> const&                             any;

NIFTY_DECL(list);
NIFTY_DECL(none);
NIFTY_DECL(obj);
NIFTY_DECL(atom);
NIFTY_DECL(enum_);
NIFTY_DECL(bool_);
NIFTY_DECL(idec);
NIFTY_DECL(ihex);
NIFTY_DECL(udec);
NIFTY_DECL(uhex);
NIFTY_DECL(int_);
NIFTY_DECL(tup);
NIFTY_DECL(uint);
NIFTY_DECL(word);
NIFTY_DECL(size);
NIFTY_DECL(ofs);
namespace detail {
extern codegen::def<>& arr_o;
NIFTY_DECL(arr_o);
} // namespace detail
NIFTY_DECL(arr);
NIFTY_DECL(map);
NIFTY_DECL(set);
NIFTY_DECL(stack);
NIFTY_DECL(queue);
NIFTY_DECL(pqueue);
NIFTY_DECL(any);

inline codegen::end_category<"type"> type_end;

} // namespace api

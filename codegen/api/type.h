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

// TODO: arr; range; range construction
// TODO: enum/is_enum word check
// TODO: update WhitespaceSensitiveMacros during build
// TODO: word defaults

extern codegen::def<"list(...: list) -> list"> const&                            list;
extern codegen::def<"none(...: none) -> none"> const&                            none;
extern codegen::def<"object(...: object=0) -> object"> const&                    object;
extern codegen::def<"tuple(...: range=()) -> tuple"> const&                      tuple;
extern codegen::def<"atom(...: atom=0) -> atom"> const&                          atom;
extern codegen::def<"enum(...: v: enum<...>, prefix: atom) -> enum<...>"> const& enum_;
extern codegen::def<"idec(...: word=0) -> idec"> const&                          idec;
extern codegen::def<"bool(...: bool=0) -> bool"> const&                          bool_;
extern codegen::def<"ihex(...: word=0) -> ihex"> const&                          ihex;
extern codegen::def<"udec(...: word=0) -> udec"> const&                          udec;
extern codegen::def<"uhex(...: word=0) -> uhex"> const&                          uhex;
extern codegen::def<"int(...: word=0, hint=AUTO: enum<IDEC,IHEX,AUTO>) -> int"> const&
    int_;
extern codegen::def<"uint(...: word=0, hint=AUTO: enum<UDEC,UHEX,AUTO>) -> uint"> const&
    uint;
extern codegen::def<
    "word(...: word=0, hint=AUTO: enum<IDEC,IHEX,UDEC,UHEX,AUTO>) -> word"> const& word;
extern codegen::def<
    "size(...: word=0, hint=AUTO: enum<IDEC,IHEX,UDEC,UHEX,AUTO>) -> size"> const& size;
extern codegen::def<
    "offset(...: word=0, hint=AUTO: enum<IDEC,IHEX,UDEC,UHEX,AUTO>) -> offset"> const&
                                                            offset;
extern codegen::def<"array(...: range=()) -> array"> const& array;
extern codegen::def<
    "order(...: {order}|{dir?: enum<ASC|DESC>, prefix?: atom}) -> order"> const& order;
extern codegen::def<"map(...: {map}|{prefix?: atom}) -> map"> const&             map;
extern codegen::def<"set(...: {set}|{prefix?: atom}) -> set"> const&             set;
extern codegen::def<"stack(...: range=()) -> stack"> const&                      stack;
extern codegen::def<"queue(...: range=()) -> queue"> const&                      queue;
extern codegen::def<"pqueue(...: {pqueue}|{prefix?: atom}) -> pqueue"> const&    pqueue;
extern codegen::def<
    "range(...: range=(), hint=AUTO: "
    "enum<TUPLE|ARRAY|ORDER|MAP|SET|STACK|QUEUE|PQUEUE|AUTO>) -> range"> const& range;
// extern codegen::def<
//     "buffer(...: buffer=array(), hint=AUTO: enum<TUPLE|ARRAY|AUTO>) -> buffer"> const&
//                                                    buffer;
extern codegen::def<"any(...: any) -> any"> const&                              any;

NIFTY_DECL(list);
NIFTY_DECL(none);
NIFTY_DECL(object);
NIFTY_DECL(tuple);
NIFTY_DECL(atom);
NIFTY_DECL(enum_);
NIFTY_DECL(idec);
NIFTY_DECL(bool_);
NIFTY_DECL(ihex);
NIFTY_DECL(udec);
NIFTY_DECL(uhex);
NIFTY_DECL(int_);
NIFTY_DECL(uint);
NIFTY_DECL(word);
NIFTY_DECL(size);
NIFTY_DECL(offset);
NIFTY_DECL(array);
NIFTY_DECL(order);
NIFTY_DECL(map);
NIFTY_DECL(set);
NIFTY_DECL(stack);
NIFTY_DECL(queue);
NIFTY_DECL(pqueue);
NIFTY_DECL(range);
/* NIFTY_DECL(buffer); */
NIFTY_DECL(any);

inline codegen::end_category<"type"> type_end;

} // namespace api

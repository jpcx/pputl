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

extern codegen::def<"none(...: <nothing>) -> none{}"> const&                  none;
extern codegen::def<"some(...: v: some) -> some{v}"> const&                   some;
extern codegen::def<"obj(...: v: obj) -> obj{v}"> const&                      obj;
extern codegen::def<"atom(...: v: obj) -> atom{v}"> const&                    atom;
extern codegen::def<"bool(...: v: 0|1) -> bool{b}"> const&                    bool_;
extern codegen::def<"hex(...: v: hex|nybl) -> hex{h}"> const&                 hex;
extern codegen::def<"nybl(...: v: hex|nybl) -> nybl{h}"> const&               nybl;
extern codegen::def<"int(...: v: word, hint?: IDEC|IHEX) -> int{n}"> const&   int_;
extern codegen::def<"idec(...: v: word) -> idec{n}"> const&                   idec;
extern codegen::def<"ihex(...: v: word) -> ihex{n}"> const&                   ihex;
extern codegen::def<"uint(...: v: word, hint?: UDEC|UHEX) -> uint{n}"> const& uint;
extern codegen::def<"udec(...: v: word) -> udec{n}"> const&                   udec;
extern codegen::def<"uhex(...: v: word) -> uhex{n}"> const&                   uhex;
extern codegen::def<"tup(...: v: tup) -> t"> const&                           tup;
extern codegen::def<"utup(...: v: word) -> utup{n}"> const&                   utup;
extern codegen::def<
    "word(...: v: word, [hint]: UTUP|IDEC|IHEX|UDEC|UHEX) -> word{n}"> const& word;
extern codegen::def<
    "size(...: v: word, [hint]: UTUP|IDEC|IHEX|UDEC|UHEX) -> size{n}"> const& size;
extern codegen::def<"any(...: v: any) -> any{v}"> const&                      any;

// TODO: ctor

NIFTY_DECL(none);
NIFTY_DECL(some);
NIFTY_DECL(obj);
NIFTY_DECL(atom);
NIFTY_DECL(bool_);
NIFTY_DECL(hex);
NIFTY_DECL(nybl);
NIFTY_DECL(int_);
NIFTY_DECL(idec);
NIFTY_DECL(ihex);
NIFTY_DECL(uint);
NIFTY_DECL(udec);
NIFTY_DECL(uhex);
NIFTY_DECL(tup);
NIFTY_DECL(utup);
NIFTY_DECL(word);
NIFTY_DECL(size);
NIFTY_DECL(any);

inline codegen::end_category<"type"> type_end;

} // namespace api

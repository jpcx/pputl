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

extern codegen::def<"none(...: <nothing>) -> none"> const&                     none;
extern codegen::def<"some(...: args: <something>...) -> some{...args}"> const& some;
extern codegen::def<"any(...: v: any) -> any{v}"> const&                       any;
extern codegen::def<"tup(...: t: tup) -> t"> const&                            tup;
extern codegen::def<"atom(...: v: any) -> v"> const&                           atom;
extern codegen::def<"bool(...: b: 0|1) -> b"> const&                           bool_;
extern codegen::def<"nybl(...: n: 0-F) -> nybl{n}: <0-F>"> const&              nybl;
extern codegen::def<"uint(...: n: uint|int) -> uint{n}"> const&                uint;
extern codegen::def<"udec(...: n: uint|int) -> udec{n}"> const&                udec;
extern codegen::def<"uhex(...: n: uint|int) -> uhex{n}"> const&                uhex;
extern codegen::def<"int(...: n: uint|int) -> int{n}"> const&                  int_;
extern codegen::def<"idec(...: n: uint|int) -> idec{n}"> const&                idec;
extern codegen::def<"ihex(...: n: uint|int) -> ihex{n}"> const&                ihex;
extern codegen::def<
    "typeof(...: v: none|some) -> ctor<none|some|tup|atom|udec|uhex|idec|ihex|nybl>"> const& typeof;

NIFTY_DECL(none);
NIFTY_DECL(some);
NIFTY_DECL(any);
NIFTY_DECL(tup);
NIFTY_DECL(atom);
NIFTY_DECL(bool_);
NIFTY_DECL(nybl);
NIFTY_DECL(uint);
NIFTY_DECL(udec);
NIFTY_DECL(uhex);
NIFTY_DECL(int_);
NIFTY_DECL(idec);
NIFTY_DECL(ihex);
NIFTY_DECL(typeof);

inline codegen::end_category<"type"> type_end;

} // namespace api

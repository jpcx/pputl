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
extern codegen::def<"bit(...: b: 0|1) -> b"> const&                            bit;
// extern codegen::def<"nybl(...: n: 0-F) -> nybl{n}: <0-F>"> const&              nybl;
extern codegen::def<"uint(...: n: uint|int|nybl) -> uint{n}"> const&           uint;
extern codegen::def<"int(...: n: uint|int|nybl) -> int{n}"> const&             int_;
extern codegen::def<"uhex(...: n: uint|int|nybl) -> uhex{n}"> const&           uhex;
extern codegen::def<"ihex(...: n: uint|int|nybl) -> ihex{n}"> const&           ihex;
extern codegen::def<"udec(...: n: uint|int|nybl) -> udec{n}"> const&           udec;
extern codegen::def<"idec(...: n: uint|int|nybl) -> idec{n}"> const&           idec;
// extern codegen::def<"typeof(...: v: tuple|uint|int|any) -> <ctor>"> const& typeof;

NIFTY_DECL(none);
NIFTY_DECL(some);
NIFTY_DECL(any);
NIFTY_DECL(tup);
NIFTY_DECL(atom);
NIFTY_DECL(bool_);
NIFTY_DECL(bit);
// NIFTY_DECL(nybl);
namespace detail {
using uint_traits_array = std::array<codegen::def<>, (codegen::conf::uint_max + 1) * 2>;
extern uint_traits_array& uint_traits;
extern codegen::def<>&    uint_trait;
NIFTY_DECL(uint_traits);
NIFTY_DECL(uint_trait);
} // namespace detail
NIFTY_DECL(uint);
NIFTY_DECL(int_);
NIFTY_DECL(uhex);
NIFTY_DECL(ihex);
NIFTY_DECL(udec);
NIFTY_DECL(idec);
// NIFTY_DECL(typeof);

inline codegen::end_category<"type"> type_end;

} // namespace api

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

namespace api {

inline codegen::category<"type"> type;

extern codegen::def<"tuple(...: t: tuple) -> t"> const&         tuple;
extern codegen::def<"bool(...: b: bool) -> b"> const&           bool_;
extern codegen::def<"uint(...: n: uint|int) -> uint{n}"> const& uint;
extern codegen::def<"int(...: n: uint|int) -> int{n}"> const&   int_;
extern codegen::def<"uhex(...: n: uint|int) -> uhex{n}"> const& uhex;
extern codegen::def<"ihex(...: n: uint|int) -> ihex{n}"> const& ihex;
extern codegen::def<"udec(...: n: uint|int) -> udec{n}"> const& udec;
extern codegen::def<"idec(...: n: uint|int) -> idec{n}"> const& idec;
extern codegen::def<"any(...: v: any) -> any{v}"> const&        any;
extern codegen::def<"typeof(...: v: tuple|uint|int|any) -> <ctor>"> const& typeof;

NIFTY_DECL(tuple);
namespace detail {
extern codegen::def<>& bool_pass;
extern codegen::def<>& bool_fail;
extern codegen::def<>& bool_o;
NIFTY_DECL(bool_pass);
NIFTY_DECL(bool_fail);
NIFTY_DECL(bool_o);
} // namespace detail
NIFTY_DECL(bool_);
namespace detail {
using uint_traits_array = std::array<codegen::def<>, (codegen::conf::uint_max + 1) * 2>;
extern uint_traits_array& uint_traits;
extern codegen::def<>&    uint_trait;
extern codegen::def<>&    uint_upass;
extern codegen::def<>&    uint_ipass;
extern codegen::def<>&    uint_fail;
extern codegen::def<>&    uint_o;
NIFTY_DECL(uint_traits);
NIFTY_DECL(uint_trait);
NIFTY_DECL(uint_upass);
NIFTY_DECL(uint_ipass);
NIFTY_DECL(uint_fail);
NIFTY_DECL(uint_o);
} // namespace detail
NIFTY_DECL(uint);
NIFTY_DECL(int_);
NIFTY_DECL(uhex);
NIFTY_DECL(ihex);
NIFTY_DECL(udec);
NIFTY_DECL(idec);
NIFTY_DECL(any);
NIFTY_DECL(typeof);

inline codegen::end_category<"type"> type_end;

} // namespace api

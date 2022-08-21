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

namespace api {

inline codegen::category<"lang"> lang;

extern codegen::def<"eat(...) -> <nothing>"> const&                             eat;
extern codegen::def<"esc(...: ...v: any) -> ...v"> const&                       esc;
extern codegen::def<"cat(...: [a]: any, [b]: any) -> a##b"> const&              cat;
extern codegen::def<"str(...: ...v: any) -> <string literal #...v>"> const&     str;
extern codegen::def<"xstr(...: ...v: any) -> <string literal #...v>"> const&    xstr;
extern codegen::def<"first(_, ...: first: any, rest: any...) -> first"> const&  first;
extern codegen::def<"xfirst(...: ...v: any) -> v[0]"> const&                    xfirst;
extern codegen::def<"rest(_, ...: first: any, rest: any...) -> ...rest"> const& rest;
extern codegen::def<"xrest(...: ...v: any) -> ...v"> const&                     xrest;
extern codegen::def<
    "trim(...: ...v: any) -> v[0] ? (v[1:] ? ...v : v[0]) : ...v[1:]"> const& trim;
extern codegen::def<"default(...: default: <any>, ...argument: <any...>) -> "> const&
    default_;
extern codegen::def<"fail(...: msg: <string literal>) -> <preprocessor error>"> const&
    fail;

// TODO fmt.hex

NIFTY_DECL(eat);
NIFTY_DECL(esc);
NIFTY_DECL(cat);
NIFTY_DECL(str);
NIFTY_DECL(xstr);
NIFTY_DECL(first);
NIFTY_DECL(xfirst);
NIFTY_DECL(rest);
NIFTY_DECL(xrest);
NIFTY_DECL(trim);
NIFTY_DECL(default_);
NIFTY_DECL(fail);

inline codegen::end_category<"lang"> lang_end;

} // namespace api

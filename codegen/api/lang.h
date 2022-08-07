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

extern codegen::def<"lp() -> <left parens>"> const&                lp;
extern codegen::def<"rp() -> <left parens>"> const&                rp;
extern codegen::def<"eat(...) -> <nothing>"> const&                eat;
extern codegen::def<"esc(...: v: any...) -> ...v"> const&          esc;
extern codegen::def<"cat(...: [a]: any, [b]: any) -> a##b"> const& cat;
extern codegen::def<"trycat(...: [a]: any, [b]: any, [default=]: any...) -> a##b | default"> const&
                                                                             trycat;
extern codegen::def<"istr(...: v: any...) -> <string literal #...v>"> const& istr;
extern codegen::def<"str(...: v: any...) -> <string literal #...v>"> const&  str;
extern codegen::def<"first(...: v: any...) -> v[0]"> const&                  first;
extern codegen::def<"rest(...:  v: any...) -> ...v"> const&                  rest;
extern codegen::def<"trim(...: v: any...) -> v[0] ? (v[1:] ? ...v : v[0]) : ...v[1:]"> const& trim;
extern codegen::def<"fail(...: msg: <string literal>) -> <preprocessor error>"> const&        fail;

NIFTY_DECL(lp);
NIFTY_DECL(rp);
NIFTY_DECL(eat);
NIFTY_DECL(esc);
NIFTY_DECL(cat);
NIFTY_DECL(trycat);
NIFTY_DECL(istr);
NIFTY_DECL(str);
NIFTY_DECL(first);
NIFTY_DECL(rest);
NIFTY_DECL(trim);
NIFTY_DECL(fail);

inline codegen::end_category<"lang"> lang_end;

} // namespace api

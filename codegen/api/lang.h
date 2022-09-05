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

extern codegen::def<"eat(...) -> none"> const&                                   eat;
extern codegen::def<"esc(...) -> any..."> const&                                 esc;
extern codegen::def<"cat(a, b: a: any, b: any) -> any"> const&                   cat;
extern codegen::def<"xcat(...: a: any, b: any) -> any"> const&                   xcat;
extern codegen::def<"str(...: any...) -> obj"> const&                            str;
extern codegen::def<"xstr(...: any...) -> obj"> const&                           xstr;
extern codegen::def<"first(_, ...: first: any, ...rest: any) -> any"> const&     first;
extern codegen::def<"xfirst(...: any...) -> any"> const&                         xfirst;
extern codegen::def<"rest(_, ...: first: any, ...rest: any) -> any..."> const&   rest;
extern codegen::def<"xrest(...: any...) -> any..."> const&                       xrest;
extern codegen::def<"trim(...: any...) -> any..."> const&                        trim;
extern codegen::def<"default(...: default: any, ...args: any) -> any..."> const& default_;
extern codegen::def<"fail(...: msg: obj)"> const&                                fail;

// creates an error string for possible failure
// see type.any for a simple usage example
[[nodiscard]] inline std::string
error(codegen::def_base const& targ, std::string const& msg, codegen::va args) {
  return str(codegen::pp::str("[" + targ + "] " + msg) + " : " + args);
}

// TODO fmt.hex

NIFTY_DECL(eat);
NIFTY_DECL(esc);
NIFTY_DECL(cat);
NIFTY_DECL(xcat);
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

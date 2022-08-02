#pragma once
////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////

#include "codegen.h"
#include "config.h"

namespace api {

inline codegen::category<"lang"> lang;

extern codegen::def<"eat(...) -> <nothing>"> const&                      eat;
extern codegen::def<"esc(...: args: any...) -> ...args"> const&          esc;
extern codegen::def<"str(...: args: any...) -> #...args"> const&         str;
extern codegen::def<"cat(...: a: any, b: any) -> a##b"> const&           cat;
extern codegen::def<"first(...: _: any, args: any...) -> a"> const&      first;
extern codegen::def<"rest(...: _: any, args: any...) -> ...args"> const& rest;

NIFTY_DECL(eat);
NIFTY_DECL(esc);
NIFTY_DECL(str);
NIFTY_DECL(cat);
NIFTY_DECL(first);
NIFTY_DECL(rest);

} // namespace api

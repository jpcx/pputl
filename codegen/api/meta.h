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
#include "numeric.h"
#include "traits.h"
#include "type.h"

namespace api {

inline codegen::category<"meta"> meta;

std::string                                                                    xtrace_expected(unsigned n);
extern codegen::def<"xtrace -> <xtrace expr>"> const&                          xtrace;
extern codegen::def<"xcount(...: <xtrace expr>) -> uint"> const&               xcount;
extern codegen::def<"x(...: n: uint) -> (args: any...) -<n>-> ...args"> const& x;
extern codegen::def<"paste(...: v: any...) -> v[0] v[1] ..."> const&           paste;

namespace detail {
extern codegen::def<>& xtrace_a;
extern codegen::def<>& xtrace_b;
NIFTY_DECL(xtrace_a);
NIFTY_DECL(xtrace_b);
} // namespace detail
NIFTY_DECL(xtrace);
NIFTY_DECL(xcount);
NIFTY_DECL(x);
NIFTY_DECL(paste);

} // namespace api

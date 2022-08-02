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
#include "lang.h"
#include "type.h"

namespace api {

inline codegen::category<"meta"> meta;

extern codegen::def<"lp -> <left parens>"> const& lp;
extern codegen::def<"rp -> <left parens>"> const& rp;

std::string                                                                    xct_expected(unsigned n);
extern codegen::def<"xct -> <xct expr>"> const&                                xct;
extern codegen::def<"x(...: n: uint) -> (args: any...) -<n>-> ...args"> const& x;

NIFTY_DECL(lp);
NIFTY_DECL(rp);
namespace detail {
extern codegen::def<>& xct_a;
extern codegen::def<>& xct_b;
NIFTY_DECL(xct_a);
NIFTY_DECL(xct_b);
} // namespace detail
NIFTY_DECL(xct);
NIFTY_DECL(x);

} // namespace api

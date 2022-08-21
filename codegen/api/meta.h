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
//
#include "impl/meta.h"

namespace api {

inline codegen::category<"meta"> meta;

extern codegen::def<"id(...: v: any...) -> ...v"> const&                     id;
extern codegen::def<"lp(...: [n=0]: idec) -> <deferred left paren>"> const&  lp;
extern codegen::def<"rp(...: [n=0]: idec) -> <deferred right paren>"> const& rp;
std::string                                    xtrace_expected(unsigned n);
extern codegen::def<"xtrace -> xtrace"> const& xtrace;
extern codegen::def<"xtrace_read(...: xtrace) -> udec&size"> const& xtrace_read;
// extern codegen::def<"ropen(...: n: uint, f: <fn>) -> 'f lp'{n}"> const& ropen;
// extern codegen::def<"rclose(...: n: uint) -> 'rp'{n}"> const&           rclose;

// template<std::convertible_to<std::string>... Args>
// inline std::string
// meta_recur(std::string const& x, std::string const& n, std::string const& f, Args&&...
// args) {
//   return x + "(" + ropen(n, f) + " "
//        + codegen::utl::cat(std::array{std::string{std::forward<Args>(args)}...}, ", ")
//        + " "
//        + rclose(n) + ")";
// }

NIFTY_DECL(id);
NIFTY_DECL(lp);
NIFTY_DECL(rp);
namespace detail {
extern codegen::def<>& xtrace_a;
extern codegen::def<>& xtrace_b;
NIFTY_DECL(xtrace_a);
NIFTY_DECL(xtrace_b);
} // namespace detail
NIFTY_DECL(xtrace);
NIFTY_DECL(xtrace_read);
// NIFTY_DECL(ropen);
// NIFTY_DECL(rclose);

inline codegen::end_category<"meta"> meta_end;

} // namespace api

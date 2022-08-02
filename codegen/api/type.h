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

extern codegen::def<"tup(...: t: any...) -> tup{t}"> const&   tup;
extern codegen::def<"bool(...: b: any...) -> bool{b}"> const& bool_;
extern codegen::def<"uint(...: n: any...) -> uint{n}"> const& uint;

NIFTY_DECL(tup);
NIFTY_DECL(bool_);
namespace detail {
extern codegen::def<>&                                          uint_seq;
extern codegen::def<>&                                          uint_rseq;
extern std::array<codegen::def<>, codegen::conf::uint_max + 1>& uint_next;
extern std::array<codegen::def<>, codegen::conf::uint_max + 1>& uint_prev;
NIFTY_DECL(uint_seq);
NIFTY_DECL(uint_rseq);
NIFTY_DECL(uint_next);
NIFTY_DECL(uint_prev);
} // namespace detail
NIFTY_DECL(uint);

} // namespace api

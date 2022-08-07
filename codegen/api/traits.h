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
#include "type.h"

namespace api {

inline codegen::category<"traits"> traits;

extern codegen::def<"is_none(...) -> bool"> const&         is_none;
extern codegen::def<"is_some(...) -> bool"> const&         is_some;
extern codegen::def<"size(...) -> uint"> const&            size;
extern codegen::def<"is_tuple(...) -> bool"> const&        is_tuple;
extern codegen::def<"is_bool(...) -> bool"> const&         is_bool;
extern codegen::def<"is_uint(...) -> bool"> const&         is_uint;
/* extern codegen::def<"is_binary(...) -> bool"> const&       is_binary; */
extern codegen::def<"items(...: v: tuple) -> ...v"> const& items;

NIFTY_DECL(is_none);
NIFTY_DECL(is_some);
NIFTY_DECL(size);
NIFTY_DECL(is_tuple);
NIFTY_DECL(is_bool);
NIFTY_DECL(is_uint);
// NIFTY_DECL(is_binary);
NIFTY_DECL(items);

inline codegen::end_category<"traits"> traits_end;

} // namespace api

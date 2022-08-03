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

extern codegen::def<"items(...: tup) -> ...tup"> const& items;
extern codegen::def<"is_none(...) -> bool"> const&      is_none;
extern codegen::def<"is_some(...) -> bool"> const&      is_some;
extern codegen::def<"is_tup(...) -> bool"> const&       is_tup;
extern codegen::def<"is_uint(...) -> bool"> const&      is_uint;
extern codegen::def<"size(...) -> uint"> const&         size;

NIFTY_DECL(items);
NIFTY_DECL(is_none);
NIFTY_DECL(is_some);
NIFTY_DECL(is_tup);
NIFTY_DECL(is_uint);
NIFTY_DECL(size);

} // namespace api

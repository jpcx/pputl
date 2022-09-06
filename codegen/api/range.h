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

#include "bitwise.h"
#include "codegen.h"
#include "compare.h"
#include "config.h"
#include "control.h"
#include "lang.h"
#include "logic.h"
#include "math.h"
#include "meta.h"
#include "numeric.h"
#include "traits.h"
#include "type.h"
//
#include "impl/range.h"

namespace api {

inline codegen::category<"range"> range;

// TODO sizeof, is_empty

extern codegen::def<"index(...: idx, size) -> size"> const&           index;
extern codegen::def<"items(...: tup) -> list"> const&                 items;
extern codegen::def<"bisect(...: idx, tup) -> tup, tup, bool"> const& bisect;
extern codegen::def<"unite(...: tup, tup, bool=0) -> tup"> const&     unite;

NIFTY_DECL(index);
NIFTY_DECL(items);
NIFTY_DECL(bisect);
NIFTY_DECL(unite);

inline codegen::end_category<"range"> range_end;

} // namespace api

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
#include "logic.h"
#include "traits.h"
#include "type.h"

namespace api {

inline codegen::category<"compare"> compare;

extern codegen::def<"lt(...: word, word) -> bool"> const&  lt;
extern codegen::def<"gt(...: word, word) -> bool"> const&  gt;
extern codegen::def<"le(...: word, word) -> bool"> const&  le;
extern codegen::def<"ge(...: word, word) -> bool"> const&  ge;
extern codegen::def<"eq(...: word, word) -> bool"> const&  eq;
extern codegen::def<"ne(...: word, word) -> bool"> const&  ne;
extern codegen::def<"min(...: word, word) -> word"> const& min;
extern codegen::def<"max(...: word, word) -> word"> const& max;

NIFTY_DECL(lt);
NIFTY_DECL(gt);
NIFTY_DECL(le);
NIFTY_DECL(ge);
NIFTY_DECL(eq);
NIFTY_DECL(ne);
NIFTY_DECL(min);
NIFTY_DECL(max);

inline codegen::end_category<"compare"> compare_end;

} // namespace api

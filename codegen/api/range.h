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

// TODO: push_* -> lpush, rpush
// TODO: pop_*  -> lpop,  rpop

extern codegen::def<"sizeof(...: tup) -> udec&size"> const&           sizeof_;
extern codegen::def<"items(...: tup) -> any..."> const&               items;
extern codegen::def<"is_empty(...: tup) -> bool"> const&              is_empty;
extern codegen::def<"index(...: size, ofs) -> size"> const&           index;
extern codegen::def<"bisect(...: tup, ofs) -> tup, tup, bool"> const& bisect;
extern codegen::def<"unite(...: tup, tup, bool=0) -> tup"> const&     unite;
extern codegen::def<"head(...: tup, size=1) -> tup"> const&           head;
extern codegen::def<"tail(...: tup, size=1) -> tup"> const&           tail;
extern codegen::def<"lpush(...: tup, any...) -> tup"> const&          lpush;
extern codegen::def<"rpush(...: tup, any...) -> tup"> const&          rpush;
extern codegen::def<"lpop(...: tup, size=1) -> tup"> const&           lpop;
extern codegen::def<"rpop(...: tup, size=1) -> tup"> const&           rpop;
extern codegen::def<"tup_get(...: tup, ofs) -> any"> const&           tup_get;
extern codegen::def<"tup_set(...: tup, ofs, any) -> tup"> const&      tup_set;
/* extern codegen::def<"front(...: tup) -> any"> const&                  front; */
/* extern codegen::def<"back(...: tup) -> any"> const&                   back; */
/* extern codegen::def<"slice(...: tup, ofs, ofs) -> tup"> const&          slice; */
/* extern codegen::def<"splice(...: tup, ofs, ins: tup, del: size=0) -> tup"> const&
 * splice; */
/* extern codegen::def<"insert(...: tup, ofs, any) -> tup"> const&      insert; */

NIFTY_DECL(sizeof_);
NIFTY_DECL(items);
NIFTY_DECL(is_empty);
NIFTY_DECL(index);
NIFTY_DECL(bisect);
NIFTY_DECL(unite);
NIFTY_DECL(head);
NIFTY_DECL(tail);
NIFTY_DECL(lpush);
NIFTY_DECL(rpush);
NIFTY_DECL(lpop);
NIFTY_DECL(rpop);
NIFTY_DECL(tup_get);
NIFTY_DECL(tup_set);
/* NIFTY_DECL(front); */
/* NIFTY_DECL(back); */
/* NIFTY_DECL(slice); */
/* NIFTY_DECL(splice); */
/* NIFTY_DECL(insert); */

inline codegen::end_category<"range"> range_end;

} // namespace api

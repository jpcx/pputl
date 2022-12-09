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

namespace api {

inline codegen::category<"range"> range_;

// TODO: arr overloads

extern codegen::def<"sizeof(...: range) -> udec&size"> const&           sizeof_;
extern codegen::def<"itemsof(...: range) -> list"> const&               itemsof;
extern codegen::def<"is_empty(...: range) -> bool"> const&              is_empty;
extern codegen::def<"bisect(...: buf, ofs) -> buf, buf, bool"> const&   bisect;
extern codegen::def<"unite(...: buf, buf, bool=0) -> buf"> const&       unite;
extern codegen::def<"head(...: buf, size=1) -> mem"> const&             head;
extern codegen::def<"tail(...: buf, size=1) -> mem"> const&             tail;
extern codegen::def<"push_front(...: buf, any) -> mem"> const&          push_front;
extern codegen::def<"push_back(...: buf, any) -> mem"> const&           push_back;
extern codegen::def<"pop_front(...: buf, size=1) -> tup"> const&        pop_front;
extern codegen::def<"pop_back(...: buf, size=1) -> tup"> const&         pop_back;
extern codegen::def<"get_at(...: range, k: ofs) -> any"> const&         get_item;
extern codegen::def<"set_at(...: range, k: ofs, v: any) -> mem"> const& set_item;
/* extern codegen::def<"ins_item(...: tup, ofs, any) -> tup"> const&      ins_item; */
/* extern codegen::def<"del_item(...: tup, ofs, any) -> tup"> const&      del_item; */
/* extern codegen::def<"front(...: tup) -> any"> const&                  front; */
/* extern codegen::def<"back(...: tup) -> any"> const&                   back; */
/* extern codegen::def<"slice(...: tup, ofs, ofs) -> tup"> const&          slice; */
/* extern codegen::def<"splice(...: tup, ofs, ins: tup, del: size=0) -> tup"> const&
 * splice; */
/* extern codegen::def<"push(...: {m: stack|queue, v: any}|{m: pqueue, pri: word, v: any})
 * -> typeof(m)"> const&          push; */
/* extern codegen::def<"pop(...: m: stack|queue|pqueue) -> typeof(m)"> const& pop;
 */

NIFTY_DECL(sizeof_);
NIFTY_DECL(itemsof);
NIFTY_DECL(is_empty);
NIFTY_DECL(bisect);
NIFTY_DECL(unite);
NIFTY_DECL(head);
NIFTY_DECL(tail);
NIFTY_DECL(push_front);
NIFTY_DECL(push_back);
NIFTY_DECL(pop_front);
NIFTY_DECL(pop_back);
NIFTY_DECL(get_item);
NIFTY_DECL(set_item);

inline codegen::end_category<"range"> range_end;

} // namespace api

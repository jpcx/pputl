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
#include "traits.h"
#include "type.h"

namespace api {

inline codegen::category<"logic"> logic;

extern codegen::def<"not(...: bool) -> bool"> const&        not_;
extern codegen::def<"and(...: bool, bool) -> bool"> const&  and_;
extern codegen::def<"or(...: bool, bool) -> bool"> const&   or_;
extern codegen::def<"xor(...: bool, bool) -> bool"> const&  xor_;
extern codegen::def<"nand(...: bool, bool) -> bool"> const& nand_;
extern codegen::def<"nor(...: bool, bool) -> bool"> const&  nor_;
extern codegen::def<"xnor(...: bool, bool) -> bool"> const& xnor_;

NIFTY_DECL(not_);
NIFTY_DECL(and_);
NIFTY_DECL(or_);
NIFTY_DECL(xor_);
NIFTY_DECL(nand_);
NIFTY_DECL(nor_);
NIFTY_DECL(xnor_);

inline codegen::end_category<"logic"> logic_end;

} // namespace api

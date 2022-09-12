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
//
#include "impl/compare.h"
#include "impl/math.h"
#include "impl/numeric.h"
#include "impl/traits.h"

namespace api {
namespace impl {

inline codegen::category<"impl.math"> math;

extern codegen::def<"add(a, b: uhex, uhex) -> uhex"> const& add;
extern codegen::def<"sub(a, b: uhex, uhex) -> uhex"> const& sub;
extern codegen::def<"index(i, sign, sz, err: uhex, bool, uhex, obj) -> utup"> const&
    index;

NIFTY_DECL(add);
NIFTY_DECL(sub);
NIFTY_DECL(index);

inline codegen::end_category<"impl.math"> math_end;

} // namespace impl
} // namespace api

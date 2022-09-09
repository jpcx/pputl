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
//
#include "impl/compare.h"
#include "impl/math.h"
#include "impl/numeric.h"

namespace api {

inline codegen::category<"numeric"> numeric;

extern codegen::def<"inc(...: word) -> word"> const&     inc;
extern codegen::def<"dec(...: word) -> word"> const&     dec;
extern codegen::def<"neg(...: word) -> word"> const&     neg;
extern codegen::def<"eqz(...: word) -> bool"> const&     eqz;
extern codegen::def<"nez(...: word) -> bool"> const&     nez;
extern codegen::def<"ltz(...: word) -> bool"> const&     ltz;
extern codegen::def<"gtz(...: word) -> bool"> const&     gtz;
extern codegen::def<"lez(...: word) -> bool"> const&     lez;
extern codegen::def<"gez(...: word) -> bool"> const&     gez;
extern codegen::def<"abs(...: word) -> word"> const&     abs;
extern codegen::def<"log2(...: word) -> word"> const&    log2;
extern codegen::def<"sqrt(...: word) -> word"> const&    sqrt;
extern codegen::def<"fact(...: word) -> word..."> const& fact;
extern codegen::def<"prime(...: word) -> bool"> const&   prime;

NIFTY_DECL(inc);
NIFTY_DECL(dec);
NIFTY_DECL(neg);
namespace detail {
extern codegen::def<>& eqz_0u;
NIFTY_DECL(eqz_0u);
} // namespace detail
NIFTY_DECL(eqz);
NIFTY_DECL(nez);
NIFTY_DECL(ltz);
NIFTY_DECL(gtz);
NIFTY_DECL(lez);
NIFTY_DECL(gez);
NIFTY_DECL(abs);
NIFTY_DECL(log2);
NIFTY_DECL(sqrt);
NIFTY_DECL(fact);
NIFTY_DECL(prime);

inline codegen::end_category<"numeric"> numeric_end;

} // namespace api

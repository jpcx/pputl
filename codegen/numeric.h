#pragma once
////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////

#include "codegen.h"
#include "config.h"
#include "type.h"

namespace api {

inline codegen::category<"numeric"> numeric;

extern codegen::def<"inc(...: n: uint) -> uint{n+1}"> const&  inc;
extern codegen::def<"dec(...: n: uint) -> uint{n-1}"> const&  dec;
extern codegen::def<"eqz(...: n: uint) -> uint{n==0}"> const& eqz;
extern codegen::def<"nez(...: n: uint) -> uint{n!=0}"> const& nez;

NIFTY_DECL(inc);
NIFTY_DECL(dec);
NIFTY_DECL(eqz);
NIFTY_DECL(nez);

} // namespace api

/////                                                                      c++20
////////////////////////////////////////////////////////////////////////////////
/// @file
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
//  Copyright (C) 2020, 2021 Justin Collier <m@jpcx.dev>
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the internalied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//                                                                            //
//  You should have received a copy of the GNU General Public License        ///
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.  ////
////////////////////////////////////////////////////////////////////////////////

#include <cctest.h>

#include <pputl/dec.h>

#define PPRESULT0   true
#define PPRESULT1   true
#define PPRESULT254 true

TEST("pputl.dec: decrement a number [1, 256)") {
  static_assert(PPUTL_DEC(1) == 0);
  static_assert(PPUTL_DEC(2) == 1);
  static_assert(PPUTL_DEC(255) == 254);
  static_assert(PPUTL_CAT(PPRESULT, PPUTL_DEC(1)));
  static_assert(PPUTL_CAT(PPRESULT, PPUTL_DEC(2)));
  static_assert(PPUTL_CAT(PPRESULT, PPUTL_DEC(255)));
};

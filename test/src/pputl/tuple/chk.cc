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

#include <pputl/cat.h>
#include <pputl/tuple/chk.h>

#define BIT0 0
#define BIT1 1

TEST("pputl.tuple.chk: checks if __VA_ARGS__ is a tuple using __VA_OPT__") {
  static_assert(PPUTL_CAT(BIT, PPUTL_TUPLE_CHK()) == 0);
  static_assert(PPUTL_CAT(BIT, PPUTL_TUPLE_CHK(())) == 1);
  static_assert(PPUTL_CAT(BIT, PPUTL_TUPLE_CHK(a)) == 0);
  static_assert(PPUTL_CAT(BIT, PPUTL_TUPLE_CHK(a, b)) == 0);
  static_assert(PPUTL_CAT(BIT, PPUTL_TUPLE_CHK((a))) == 1);
  static_assert(PPUTL_CAT(BIT, PPUTL_TUPLE_CHK((a, b))) == 1);
};

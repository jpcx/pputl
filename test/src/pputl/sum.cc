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

#include <pputl/sum.h>

#define SAMP1 2
#define SAMP2 3
#define SAMP3 4
#define SAMP4 5

TEST("pputl.sum: sum N numbers [0, 256)") {
  static_assert(PPUTL_SUM(0) == 0);
  static_assert(PPUTL_SUM(0, 0) == 0);
  static_assert(PPUTL_SUM(0, 0, 0) == 0);
  static_assert(PPUTL_SUM(0, 1) == 1);
  static_assert(PPUTL_SUM(0, 1, 0) == 1);
  static_assert(PPUTL_SUM(0, 1, 1) == 2);
  static_assert(PPUTL_SUM(0, 1, 2) == 3);
  static_assert(PPUTL_SUM(1, 0) == 1);
  static_assert(PPUTL_SUM(1, 0, 0) == 1);
  static_assert(PPUTL_SUM(1, 0, 1) == 2);
  static_assert(PPUTL_SUM(1, 0, 2) == 3);
  static_assert(PPUTL_SUM(1, 1) == 2);
  static_assert(PPUTL_SUM(1, 1, 0) == 2);
  static_assert(PPUTL_SUM(1, 1, 1) == 3);
  static_assert(PPUTL_SUM(1, 1, 2) == 4);
  static_assert(PPUTL_SUM(1, 2) == 3);
  static_assert(PPUTL_SUM(1, 2, 0) == 3);
  static_assert(PPUTL_SUM(1, 2, 1) == 4);
  static_assert(PPUTL_SUM(1, 2, 2) == 5);
  static_assert(PPUTL_SUM(2, 1) == 3);
  static_assert(PPUTL_SUM(2, 1, 0) == 3);
  static_assert(PPUTL_SUM(2, 1, 1) == 4);
  static_assert(PPUTL_SUM(2, 1, 2) == 5);
  static_assert(PPUTL_SUM(2, 2) == 4);
  static_assert(PPUTL_SUM(2, 2, 0) == 4);
  static_assert(PPUTL_SUM(2, 2, 1) == 5);
  static_assert(PPUTL_SUM(2, 2, 2) == 6);
  static_assert(PPUTL_SUM(254, 1) == 255);
  static_assert(PPUTL_SUM(254, 1, 0) == 255);
  static_assert(PPUTL_SUM(253, 1, 1) == 255);
  static_assert(PPUTL_SUM(252, 1, 2) == 255);
  static_assert(PPUTL_SUM(1, 254) == 255);
  static_assert(PPUTL_SUM(1, 254, 0) == 255);
  static_assert(PPUTL_SUM(1, 253, 1) == 255);
  static_assert(PPUTL_SUM(1, 252, 2) == 255);

  static_assert(
      PPUTL_SUM(
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
          1, 1) == 255);

  static_assert(PPUTL_CAT(SAMP, PPUTL_SUM(0, 1)) == 2);
  static_assert(PPUTL_CAT(SAMP, PPUTL_SUM(0, 1, 0)) == 2);
  static_assert(PPUTL_CAT(SAMP, PPUTL_SUM(0, 1, 1)) == 3);
  static_assert(PPUTL_CAT(SAMP, PPUTL_SUM(0, 1, 2)) == 4);
  static_assert(PPUTL_CAT(SAMP, PPUTL_SUM(0, 2)) == 3);
  static_assert(PPUTL_CAT(SAMP, PPUTL_SUM(0, 2, 0)) == 3);
  static_assert(PPUTL_CAT(SAMP, PPUTL_SUM(0, 2, 1)) == 4);
  static_assert(PPUTL_CAT(SAMP, PPUTL_SUM(0, 2, 2)) == 5);
};

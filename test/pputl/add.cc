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
//  Copyright (C) 2020 Justin Collier <m@jpcx.dev>
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

#include <cctest/cctest.h>

#include <pputl/add.h>

#define SAMP1 2
#define SAMP2 3

TEST(pputl.add, "add two numbers together [0, 256)") << [] {
  STATIC_CHECK(PPUTL_ADD(0, 0) == 0);
  STATIC_CHECK(PPUTL_ADD(0, 1) == 1);
  STATIC_CHECK(PPUTL_ADD(1, 0) == 1);
  STATIC_CHECK(PPUTL_ADD(1, 1) == 2);
  STATIC_CHECK(PPUTL_ADD(1, 2) == 3);
  STATIC_CHECK(PPUTL_ADD(2, 1) == 3);
  STATIC_CHECK(PPUTL_ADD(2, 2) == 4);
  STATIC_CHECK(PPUTL_ADD(254, 1) == 255);
  STATIC_CHECK(PPUTL_ADD(1, 254) == 255);

  STATIC_CHECK(PPUTL_CAT(SAMP, PPUTL_ADD(0, 1)) == 2);
  STATIC_CHECK(PPUTL_CAT(SAMP, PPUTL_ADD(0, 2)) == 3);
};

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

#include <pputl/includes.h>

TEST(
    pputl.includes,
    "expands to 1 if numeric args [0, 256) have at least one `n` value; else 0")
    << [] {
         STATIC_CHECK(PPUTL_INCLUDES(0, 0) == 1);
         STATIC_CHECK(PPUTL_INCLUDES(0, 1) == 0);
         STATIC_CHECK(PPUTL_INCLUDES(1, 1) == 1);
         STATIC_CHECK(PPUTL_INCLUDES(1, 2) == 0);
         STATIC_CHECK(PPUTL_INCLUDES(2, 2) == 1);
         STATIC_CHECK(PPUTL_INCLUDES(0, 0, 1, 2, 3, 5, 7, 9) == 1);
         STATIC_CHECK(PPUTL_INCLUDES(1, 0, 1, 2, 3, 5, 7, 9) == 1);
         STATIC_CHECK(PPUTL_INCLUDES(2, 0, 1, 2, 3, 5, 7, 9) == 1);
         STATIC_CHECK(PPUTL_INCLUDES(3, 0, 1, 2, 3, 5, 7, 9) == 1);
         STATIC_CHECK(PPUTL_INCLUDES(4, 0, 1, 2, 3, 5, 7, 9) == 0);
         STATIC_CHECK(PPUTL_INCLUDES(5, 0, 1, 2, 3, 5, 7, 9) == 1);
         STATIC_CHECK(PPUTL_INCLUDES(6, 0, 1, 2, 3, 5, 7, 9) == 0);
         STATIC_CHECK(PPUTL_INCLUDES(7, 0, 1, 2, 3, 5, 7, 9) == 1);
         STATIC_CHECK(PPUTL_INCLUDES(8, 0, 1, 2, 3, 5, 7, 9) == 0);
         STATIC_CHECK(PPUTL_INCLUDES(9, 0, 1, 2, 3, 5, 7, 9) == 1);
       };

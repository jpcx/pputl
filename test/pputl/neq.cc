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

#include <pputl/neq.h>

#define PPRESULT0 false
#define PPRESULT1 true

TEST(
    pputl.neq,
    "expands to 1 if two numbers a and b [0, 256) are NOT equal, or 0 if not")
    << [] {
         STATIC_CHECK(!PPUTL_NEQ(1, 1));
         STATIC_CHECK(!PPUTL_NEQ(2, 2));
         STATIC_CHECK(PPUTL_NEQ(1, 2));
         STATIC_CHECK(PPUTL_NEQ(2, 1));
         STATIC_CHECK(!PPUTL_CAT(PPRESULT, PPUTL_NEQ(1, 1)));
         STATIC_CHECK(!PPUTL_CAT(PPRESULT, PPUTL_NEQ(2, 2)));
         STATIC_CHECK(PPUTL_CAT(PPRESULT, PPUTL_NEQ(1, 2)));
         STATIC_CHECK(PPUTL_CAT(PPRESULT, PPUTL_NEQ(2, 1)));
       };

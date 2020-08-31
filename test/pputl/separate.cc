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

#include <pputl/separate.h>
#include <pputl/stringize.h>

#include "streq.h"

#define STR PPUTL_STRINGIZE

using namespace testpputl;

TEST(
    pputl.separate,
    "separates [0, 256) args with spaces (removes commas from args)")
    <<
    [] {
      STATIC_CHECK(streq(STR(PPUTL_SEPARATE()), ""));
      STATIC_CHECK(PPUTL_SEPARATE(1, +, 2) == 3);
      STATIC_CHECK(PPUTL_SEPARATE(1, +, 2, +, 3) == 6);
      STATIC_CHECK(
          PPUTL_SEPARATE(
              1, +, 3, +, 5, +, 7, +, 9, +, 11, +, 13, +, 15, +, 17, +, 19, +,
              21, +, 23, +, 25, +, 27, +, 29, +, 31, +, 33, +, 35, +, 37, +, 39,
              +, 41, +, 43, +, 45, +, 47, +, 49, +, 51, +, 53, +, 55, +, 57, +,
              59, +, 61, +, 63, +, 65, +, 67, +, 69, +, 71, +, 73, +, 75, +, 77,
              +, 79, +, 81, +, 83, +, 85, +, 87, +, 89, +, 91, +, 93, +, 95, +,
              97, +, 99, +, 101, +, 103, +, 105, +, 107, +, 109, +, 111, +, 113,
              +, 115, +, 117, +, 119, +, 121, +, 123, +, 125, +, 127, +, 129, +,
              131, +, 133, +, 135, +, 137, +, 139, +, 141, +, 143, +, 145, +,
              147, +, 149, +, 151, +, 153, +, 155, +, 157, +, 159, +, 161, +,
              163, +, 165, +, 167, +, 169, +, 171, +, 173, +, 175, +, 177, +,
              179, +, 181, +, 183, +, 185, +, 187, +, 189, +, 191, +, 193, +,
              195, +, 197, +, 199, +, 201, +, 203, +, 205, +, 207, +, 209, +,
              211, +, 213, +, 215, +, 217, +, 219, +, 221, +, 223, +, 225, +,
              227, +, 229, +, 231, +, 233, +, 235, +, 237, +, 239, +, 241, +,
              243, +, 245, +, 247, +, 249, +, 251, +, 253, +, 255) == 16384);
    };

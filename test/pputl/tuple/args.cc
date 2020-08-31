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

#include <pputl/stringize.h>
#include <pputl/choose.h>
#include <pputl/tuple/args.h>

#include "streq.h"

#define STR PPUTL_STRINGIZE

#define TUP (3, 2, 1)

using namespace testpputl;

TEST(pputl.tuple.args, "returns tuple elements as args") << [] {
  STATIC_CHECK(streq(STR(PPUTL_TUPLE_ARGS(())), ""));
  STATIC_CHECK(streq(STR(PPUTL_TUPLE_ARGS((a))), "a"));
  STATIC_CHECK(streq(STR(PPUTL_TUPLE_ARGS((1, 2, 3))), "1, 2, 3"));
  STATIC_CHECK(streq(STR(PPUTL_CHOOSE(0)(PPUTL_TUPLE_ARGS(TUP))), "3"));
  STATIC_CHECK(PPUTL_TUPLE_ARGS((123)) == 123);
};

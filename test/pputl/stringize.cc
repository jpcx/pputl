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

#include <pputl/pputl.h>

#include "streq.h"

#define STR PPUTL_STRINGIZE

using namespace testpputl;

TEST(pputl.stringize, "stringizes provided args") << [] {
  STATIC_CHECK(streq(STR(), ""));
  STATIC_CHECK(streq(STR(foo), "foo"));
  STATIC_CHECK(streq(STR(abc123!@#%^&*()-_=+[]{},.;<>:/\\?|), "abc123!@#%^&*()-_=+[]{},.;<>:/\\?|"));
  STATIC_CHECK(!streq(STR(foo), "bar"));
  STATIC_CHECK(!streq(STR(foo), "fo"));
  STATIC_CHECK(!streq(STR(fo), "foo"));
  STATIC_CHECK(!streq(STR(), "foo"));
  STATIC_CHECK(!streq(STR(foo), ""));
};
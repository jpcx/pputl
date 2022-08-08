/* /////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////// */

#include "compare.h"

namespace api {

using namespace codegen;

decltype(max) max = NIFTY_DEF(max, [&](va args) {
  docs << "uint maximum operation.";

  auto lessmax = std::to_string(conf::uint_max - 1);

  tests << max("0, 0")                                 = "0" >> docs;
  tests << max("0, 1")                                 = "1" >> docs;
  tests << max("0, 2")                                 = "2";
  tests << max("0, 3")                                 = "3";
  tests << max("1, 0")                                 = "1" >> docs;
  tests << max("1, 1")                                 = "1" >> docs;
  tests << max("1, 2")                                 = "2";
  tests << max("1, 3")                                 = "3";
  tests << max("2, 0")                                 = "2";
  tests << max("2, 1")                                 = "2";
  tests << max("2, 2")                                 = "2";
  tests << max("2, 3")                                 = "3";
  tests << max("3, 0")                                 = "3";
  tests << max("3, 1")                                 = "3";
  tests << max("3, 2")                                 = "3";
  tests << max("3, 3")                                 = "3";
  tests << max(0, conf::uint_max)                      = uint_max_s;
  tests << max(0, conf::uint_max - 1)                  = lessmax;
  tests << max(1, conf::uint_max)                      = uint_max_s;
  tests << max(1, conf::uint_max - 1)                  = lessmax;
  tests << max(conf::uint_max, 0)                      = uint_max_s;
  tests << max(conf::uint_max - 1, 0)                  = lessmax;
  tests << max(conf::uint_max, 1)                      = uint_max_s;
  tests << max(conf::uint_max - 1, 1)                  = lessmax;
  tests << max(conf::uint_max, conf::uint_max)         = uint_max_s;
  tests << max(conf::uint_max, conf::uint_max - 1)     = uint_max_s;
  tests << max(conf::uint_max - 1, conf::uint_max)     = uint_max_s;
  tests << max(conf::uint_max - 1, conf::uint_max - 1) = lessmax;

  def<"0(a, b)"> _0 = [&](arg, arg b) {
    return b;
  };

  def<"1(a, b)">{} = [&](arg a, arg) {
    return a;
  };

  return pp::call(cat(utl::slice(_0, -1), gt(args)), args);
});

} // namespace api

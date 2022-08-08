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

decltype(min) min = NIFTY_DEF(min, [&](va args) {
  docs << "uint minimum operation.";

  tests << min("0, 0")                                 = "0" >> docs;
  tests << min("0, 1")                                 = "0" >> docs;
  tests << min("0, 2")                                 = "0";
  tests << min("0, 3")                                 = "0";
  tests << min("1, 0")                                 = "0" >> docs;
  tests << min("1, 1")                                 = "1" >> docs;
  tests << min("1, 2")                                 = "1";
  tests << min("1, 3")                                 = "1";
  tests << min("2, 0")                                 = "0";
  tests << min("2, 1")                                 = "1";
  tests << min("2, 2")                                 = "2";
  tests << min("2, 3")                                 = "2";
  tests << min("3, 0")                                 = "0";
  tests << min("3, 1")                                 = "1";
  tests << min("3, 2")                                 = "2";
  tests << min("3, 3")                                 = "3";
  tests << min(0, conf::uint_max)                      = "0";
  tests << min(0, conf::uint_max - 1)                  = "0";
  tests << min(1, conf::uint_max)                      = "1";
  tests << min(1, conf::uint_max - 1)                  = "1";
  tests << min(conf::uint_max, 0)                      = "0";
  tests << min(conf::uint_max - 1, 0)                  = "0";
  tests << min(conf::uint_max, 1)                      = "1";
  tests << min(conf::uint_max - 1, 1)                  = "1";
  tests << min(conf::uint_max, conf::uint_max)         = uint_max_s;
  tests << min(conf::uint_max, conf::uint_max - 1)     = std::to_string(conf::uint_max - 1);
  tests << min(conf::uint_max - 1, conf::uint_max)     = std::to_string(conf::uint_max - 1);
  tests << min(conf::uint_max - 1, conf::uint_max - 1) = std::to_string(conf::uint_max - 1);

  def<"0(a, b)"> _0 = [&](arg, arg b) {
    return b;
  };

  def<"1(a, b)">{} = [&](arg a, arg) {
    return a;
  };

  return pp::call(cat(utl::slice(_0, -1), lt(args)), args);
});

} // namespace api

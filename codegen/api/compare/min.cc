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
  docs << "word minimum operation."
       << "prohibits comparison of different signedness."
       << "utups are interpreted as (and are comparable with) unsigned.";

  using std::to_string;
  using conf::uint_max;
  using conf::int_max;
  using conf::word_size;
  using std::vector;
  using std::string;

  tests << min("0, 0")   = "0" >> docs;
  tests << min("0, 1")   = "0" >> docs;
  tests << min("7u, 8u") = "7u" >> docs;
  tests << min("8u, 7u") = "7u";
  tests << min(int_(uint_max_s), "0") =
      ("0x" + utl::cat(vector<string>(word_size, "F"))) >> docs;
  tests << min(int_max_s, int_min_s)                          = int_min_s >> docs;
  tests << min(int_min_s, int_max_s)                          = int_min_s;
  tests << min(int_min_s, int_(to_string(int_max + 1) + "u")) = int_min_s >> docs;
  tests << min(int_min_s, int_(to_string(int_max + 2) + "u")) = int_min_s >> docs;
  tests << min("0u", uint_max_s)                              = "0u";
  tests << min(uint_max_s, "0u")                              = "0u";
  tests << min(to_string(uint_max / 2) + "u", to_string((uint_max / 2) - 1) + "u") =
      to_string((uint_max / 2) - 1) + "u";
  tests << min(to_string(uint_max / 2) + "u", to_string((uint_max / 2)) + "u") =
      to_string(uint_max / 2) + "u";
  tests << min(to_string(uint_max / 2) + "u", to_string((uint_max / 2) + 1) + "u") =
      to_string(uint_max / 2) + "u";
  tests << min(to_string(int_max / 2), to_string((int_max / 2) - 1)) =
      to_string((int_max / 2) - 1);
  tests << min(to_string(int_max / 2), to_string((int_max / 2))) =
      to_string((int_max / 2));
  tests << min(to_string(int_max / 2), to_string((int_max / 2) + 1)) =
      to_string(int_max / 2);

  def<"\\0(a, b)"> _0 = [&](arg, arg b) { return b; };
  def<"\\1(a, b)">{}  = [&](arg a, arg) { return a; };

  return pp::call(xcat(utl::slice(_0, -1), lt(args)), args);
});

} // namespace api

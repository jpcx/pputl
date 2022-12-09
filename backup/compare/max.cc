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
  docs << "integral maximum operation."
       << "prohibits comparison of different signedness.";

  using std::to_string;
  using conf::uint_max;
  using conf::int_max;
  using conf::word_size;
  using std::vector;
  using std::string;

  tests << max("0, 0")                                        = "0" >> docs;
  tests << max("0, 1")                                        = "1" >> docs;
  tests << max("7u, 8u")                                      = "8u" >> docs;
  tests << max("8u, 7u")                                      = "8u";
  tests << max(int_(uint_max_s), "0")                         = "0" >> docs;
  tests << max(int_max_s, int_min_s)                          = int_max_s >> docs;
  tests << max(int_min_s, int_max_s)                          = int_max_s;
  tests << max(int_min_s, int_(to_string(int_max + 1) + "u")) = int_min_s >> docs;
  if constexpr (word_size > 1) {
    tests << max(int_min_s, int_(to_string(int_max + 2) + "u")) =
        ("0x8" + utl::cat(vector<string>(word_size - 2, "0")) + "1") >> docs;
  }
  tests << max("0u", uint_max_s) = uint_max_s;
  tests << max(uint_max_s, "0u") = uint_max_s;
  tests << max(to_string(uint_max / 2) + "u", to_string((uint_max / 2) - 1) + "u") =
      to_string((uint_max / 2)) + "u";
  tests << max(to_string(uint_max / 2) + "u", to_string((uint_max / 2)) + "u") =
      to_string(uint_max / 2) + "u";
  tests << max(to_string(uint_max / 2) + "u", to_string((uint_max / 2) + 1) + "u") =
      to_string((uint_max / 2) + 1) + "u";
  tests << max(to_string(int_max / 2), to_string((int_max / 2) - 1)) =
      to_string((int_max / 2));
  tests << max(to_string(int_max / 2), to_string((int_max / 2))) =
      to_string((int_max / 2));
  tests << max(to_string(int_max / 2), to_string((int_max / 2) + 1)) =
      to_string((int_max / 2) + 1);

  def<"\\0(a, b)"> _0 = [&](arg, arg b) {
    return b;
  };
  def<"\\1(a, b)">{} = [&](arg a, arg) {
    return a;
  };

  return pp::call(xcat(utl::slice(_0, -1), gt(args)), args);
});

} // namespace api

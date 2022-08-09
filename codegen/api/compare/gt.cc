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

decltype(gt) gt = NIFTY_DEF(gt, [&](va args) {
  docs << "integral greater-than comparison."
       << "prohibits comparison of different signedness.";

  using std::to_string;
  using conf::uint_max;
  using conf::int_max;

  tests << gt("0, 0")                                                             = "0" >> docs;
  tests << gt("0, 1")                                                             = "0" >> docs;
  tests << gt("7u, 8u")                                                           = "0" >> docs;
  tests << gt("8u, 7u")                                                           = "1";
  tests << gt(int_(uint_max_s), "0")                                              = "0" >> docs;
  tests << gt(int_max_s, int_min_s)                                               = "1" >> docs;
  tests << gt(int_min_s, int_max_s)                                               = "0";
  tests << gt(int_min_s, int_(to_string(int_max + 1) + "u"))                      = "0" >> docs;
  tests << gt(int_min_s, int_(to_string(int_max + 2) + "u"))                      = "0" >> docs;
  tests << gt("0u", uint_max_s)                                                   = "0";
  tests << gt(uint_max_s, "0u")                                                   = "1";
  tests << gt(to_string(uint_max / 2) + "u", to_string((uint_max / 2) - 1) + "u") = "1";
  tests << gt(to_string(uint_max / 2) + "u", to_string((uint_max / 2)) + "u")     = "0";
  tests << gt(to_string(uint_max / 2) + "u", to_string((uint_max / 2) + 1) + "u") = "0";
  tests << gt(to_string(int_max / 2), to_string((int_max / 2) - 1))               = "1";
  tests << gt(to_string(int_max / 2), to_string((int_max / 2)))                   = "0";
  tests << gt(to_string(int_max / 2), to_string((int_max / 2) + 1))               = "0";

  return def<"x(l, r)">{[&](arg l, arg r) {
    return lt(r, l);
  }}(args);
});

} // namespace api

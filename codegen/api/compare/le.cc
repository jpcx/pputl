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

decltype(le) le = NIFTY_DEF(le, [&](va args) {
  docs << "integral less-than-or-equal-to comparison."
       << "prohibits comparison of different signedness.";

  using std::to_string;
  using conf::uint_max;
  using conf::int_max;

  tests << le("0, 0")                                                             = "1" >> docs;
  tests << le("0, 1")                                                             = "1" >> docs;
  tests << le("7u, 8u")                                                           = "1" >> docs;
  tests << le("8u, 7u")                                                           = "0";
  tests << le(int_(uint_max_s), "0")                                              = "1" >> docs;
  tests << le(int_max_s, int_min_s)                                               = "0" >> docs;
  tests << le(int_min_s, int_max_s)                                               = "1";
  tests << le(int_min_s, int_(to_string(int_max + 1) + "u"))                      = "1" >> docs;
  tests << le(int_min_s, int_(to_string(int_max + 2) + "u"))                      = "1" >> docs;
  tests << le("0u", uint_max_s)                                                   = "1";
  tests << le(uint_max_s, "0u")                                                   = "0";
  tests << le(to_string(uint_max / 2) + "u", to_string((uint_max / 2) - 1) + "u") = "0";
  tests << le(to_string(uint_max / 2) + "u", to_string((uint_max / 2)) + "u")     = "1";
  tests << le(to_string(uint_max / 2) + "u", to_string((uint_max / 2) + 1) + "u") = "1";
  tests << le(to_string(int_max / 2), to_string((int_max / 2) - 1))               = "0";
  tests << le(to_string(int_max / 2), to_string((int_max / 2)))                   = "1";
  tests << le(to_string(int_max / 2), to_string((int_max / 2) + 1))               = "1";

  return not_(gt(args));
});

} // namespace api

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

decltype(ne) ne = NIFTY_DEF(ne, [&](va args) {
  docs << "integral not-equal-to comparison."
       << "prohibits comparison of different signedness.";

  using std::to_string;
  using conf::uint_max;
  using conf::int_max;

  tests << ne("0, 0")                                        = "0" >> docs;
  tests << ne("0, 1")                                        = "1" >> docs;
  tests << ne("7u, 8u")                                      = "1" >> docs;
  tests << ne("8u, 7u")                                      = "1";
  tests << ne(int_(uint_max_s), "0")                         = "1" >> docs;
  tests << ne(int_max_s, int_min_s)                          = "1" >> docs;
  tests << ne(int_min_s, int_max_s)                          = "1";
  tests << ne(int_min_s, int_(to_string(int_max + 1) + "u")) = "0" >> docs;
  tests << ne(int_min_s, int_(to_string(int_max + 2) + "u")) = "1" >> docs;
  tests << ne("0u", uint_max_s)                              = "1";
  tests << ne(uint_max_s, "0u")                              = "1";
  tests << ne(to_string(uint_max / 2) + "u", to_string((uint_max / 2) - 1) + "u") = "1";
  tests << ne(to_string(uint_max / 2) + "u", to_string((uint_max / 2)) + "u")     = "0";
  tests << ne(to_string(uint_max / 2) + "u", to_string((uint_max / 2) + 1) + "u") = "1";
  tests << ne(to_string(int_max / 2), to_string((int_max / 2) - 1))               = "1";
  tests << ne(to_string(int_max / 2), to_string((int_max / 2)))                   = "0";
  tests << ne(to_string(int_max / 2), to_string((int_max / 2) + 1))               = "1";

  return not_(eq(args));
});

} // namespace api

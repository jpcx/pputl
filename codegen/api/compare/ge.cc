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

decltype(ge) ge = NIFTY_DEF(ge, [&](va args) {
  docs << "O(logn) uint greater-than-or-equal-to comparison.";

  tests << ge("0, 0")                                 = "1" >> docs;
  tests << ge("0, 1")                                 = "0" >> docs;
  tests << ge("0, 2")                                 = "0";
  tests << ge("0, 3")                                 = "0";
  tests << ge("1, 0")                                 = "1" >> docs;
  tests << ge("1, 1")                                 = "1" >> docs;
  tests << ge("1, 2")                                 = "0";
  tests << ge("1, 3")                                 = "0";
  tests << ge("2, 0")                                 = "1";
  tests << ge("2, 1")                                 = "1";
  tests << ge("2, 2")                                 = "1";
  tests << ge("2, 3")                                 = "0";
  tests << ge("3, 0")                                 = "1";
  tests << ge("3, 1")                                 = "1";
  tests << ge("3, 2")                                 = "1";
  tests << ge("3, 3")                                 = "1";
  tests << ge(0, conf::uint_max)                      = "0";
  tests << ge(0, conf::uint_max - 1)                  = "0";
  tests << ge(1, conf::uint_max)                      = "0";
  tests << ge(1, conf::uint_max - 1)                  = "0";
  tests << ge(conf::uint_max, 0)                      = "1";
  tests << ge(conf::uint_max, 0)                      = "1";
  tests << ge(conf::uint_max - 1, 0)                  = "1";
  tests << ge(conf::uint_max - 1, 0)                  = "1";
  tests << ge(conf::uint_max, 1)                      = "1";
  tests << ge(conf::uint_max, 1)                      = "1";
  tests << ge(conf::uint_max - 1, 1)                  = "1";
  tests << ge(conf::uint_max - 1, 1)                  = "1";
  tests << ge(conf::uint_max, conf::uint_max)         = "1";
  tests << ge(conf::uint_max, conf::uint_max - 1)     = "1";
  tests << ge(conf::uint_max - 1, conf::uint_max)     = "0";
  tests << ge(conf::uint_max - 1, conf::uint_max - 1) = "1";

  return not_(lt(args));
});

} // namespace api

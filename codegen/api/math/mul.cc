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

#include "math.h"

namespace api {

using namespace codegen;

decltype(mul) mul = NIFTY_DEF(mul, [&](va args) {
  docs << "uint multiplication with overflow.";

  /* constexpr auto max = conf::uint_max; */

  tests << mul("0, 0")   = "0" >> docs;
  tests << mul("0, 1")   = "0" >> docs;
  tests << mul("1, 1")   = "1" >> docs;
  tests << mul("1, 2")   = "2" >> docs;
  tests << mul("2, 2")   = "4" >> docs;
  /* tests << mul(max, 1)   = uint_max_s >> docs; */
  /* tests << mul(max, max) = std::to_string((max * max) % (max + 1)) >> docs; */

  def<"recur(...)"> recur = [&](va args) {
    return def<"x(a, b)">{[&](arg a, arg b) {
      return add(a, b) + ", " + b;
    }}(args);
  };

  def<"x(...)"> x = [&](va args) {
    return args;
  };

  return first(meta_recur(x, first(args), recur, "0", rest(args)));
});

} // namespace api

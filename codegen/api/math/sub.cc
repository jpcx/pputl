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

namespace detail {
decltype(sub_impl) sub_impl = NIFTY_DEF(sub_impl);
}

decltype(sub) sub = NIFTY_DEF(sub, [&](va args) {
  docs << "uint subtraction with underflow.";

  constexpr auto max = conf::uint_max;

  tests << sub("0, 0") = "0" >> docs;
  tests << sub("0, 1") = uint_max_s >> docs;
  tests << sub("1, 0") = "1" >> docs;
  tests << sub("1, 1") = "0" >> docs;
  tests << sub("3, 1") = "2" >> docs;
  tests << sub("1, 3") = std::to_string(max - 1) >> docs;
  tests << sub(max, 0) = uint_max_s;
  tests << sub(max, 1) = std::to_string(max - 1);
  tests << sub(0, max) = "1" >> docs;
  for (int _ = 0; _ < 5; ++_) {
    int a              = std::rand() % (max + 1);
    int b              = std::rand() % (max + 1);
    tests << sub(a, b) = std::to_string(((max + 1) + (a - b)) % (max + 1));
  }

  def<"x(...)"> x = [&](va args) {
    return args;
  };

  def<"recur(...)"> recur = [&](va args) {
    return def<"recur(v, uflow)">{[&](arg v, arg uflow) {
      return dec(v) + ", " + or_(uflow, eqz(v));
    }}(args);
  };

  detail::sub_impl = def{"impl(l, r) -> uint, bool"} = [&](arg l, arg r) {
    return meta_recur(x, r, recur, l, "0");
  };

  return first(detail::sub_impl(args));
});

} // namespace api

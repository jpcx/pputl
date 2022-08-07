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

  constexpr auto max = conf::uint_max;

  tests << mul("0, 0")   = "0" >> docs;
  tests << mul("0, 1")   = "0" >> docs;
  tests << mul("1, 1")   = "1" >> docs;
  tests << mul("1, 2")   = "2" >> docs;
  tests << mul("2, 2")   = "4" >> docs;
  tests << mul(max, 1)   = uint_max_s >> docs;
  tests << mul(max, max) = std::to_string((max * max) % (max + 1)) >> docs;
  for (int _ = 0; _ < 5; ++_) {
    int a              = std::rand() % (max + 1);
    int b              = std::rand() % (max + 1);
    tests << mul(a, b) = std::to_string((a * b) % (max + 1));
  }

  def<"res(...)"> res = [&](va args) {
    return def<"x(a, b, s)">{[&](arg a, arg, arg s) {
      return add(a, s);
    }}(args);
  };

  def<"recur(...)"> recur = [&](va args) {
    return def<"x(a, b, s)">{[&](arg a, arg b, arg s) {
      return mul2(a) + ", " + div2(b) + ", " + if_(mod2(b), pp::tup(add(s, a)), pp::tup(s));
    }}(args);
  };

  def<"x(...)"> x = [&](va args) {
    return args;
  };

  def<"zero_b(a, b)"> zero_b = [&](arg, arg) {
    return "0";
  };

  def<"pos_b(a, b)"> pos_b = [&](arg a, arg b) {
    return res(meta_recur(x, log2(b), recur, a, b, "0"));
  };

  return pp::call(if_(eqz(rest(args)), pp::tup(zero_b), pp::tup(pos_b)), args);
});

} // namespace api

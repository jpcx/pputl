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

  /* tests << mul("0, 0")   = "0" >> docs; */
  /* tests << mul("0, 1")   = "0" >> docs; */
  /* tests << mul("1, 1")   = "1" >> docs; */
  /* tests << mul("1, 2")   = "2" >> docs; */
  /* tests << mul("2, 2")   = std::to_string((2 * 2) % (max + 1)) >> docs; */
  /* tests << mul(max, 1)   = uint_max_s >> docs; */
  /* tests << mul(max, max) = std::to_string((max * max) % (max + 1)) >> docs; */

  def<"a(s, a, b)"> a;
  def<"b(s, a, b)"> b;

  def<"return(s, a, b)"> return_ = [&](arg s_, arg, arg) {
    docs << "returns result";
    return s_;
  };

  a = [&](arg s_, arg a_, arg b_) {
    docs << "recursive side A";
    def<"continue(s, a, b)"> continue_ = [&](arg s_, arg a_, arg b_) {
      return b + " " + lp() + " " + add(s_, a_) + ", " + a_ + ", " + dec(b_) + " " + rp();
    };

    return pp::call(if_(eqz(b_), pp::tup(return_), pp::tup(continue_)), s_, a_, b_);
  };

  b = [&](arg s_, arg a_, arg b_) {
    docs << "recursive side B";
    def<"continue(s, a, b)"> continue_ = [&](arg s_, arg a_, arg b_) {
      return a + " " + lp() + " " + add(s_, a_) + ", " + a_ + ", " + dec(b_) + " " + rp();
    };

    return pp::call(if_(eqz(b_), pp::tup(return_), pp::tup(continue_)), s_, a_, b_);
  };

  def<"x0(...)"> x0 = [&](va args) {
    return args;
  };

  def<"x1(...)"> x1 = [&](va args) {
    return args;
  };

  return x1(ropen(rest(args), x0) + " " + a(0, uint(first(args)), uint(rest(args))) + " "
            + rclose(rest(args)));
});

} // namespace api

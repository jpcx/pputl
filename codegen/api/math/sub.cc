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

  def<"a(a, b)"> a;
  def<"b(a, b)"> b;

  def<"return(a, b)"> return_ = [&](arg a_, arg) {
    docs << "returns result";
    return a_;
  };

  a = [&](arg a_, arg b_) {
    docs << "mutually recursive side A";
    def<"continue(a, b)"> continue_ = [&](arg a_, arg b_) {
      return b + " " + lp() + " " + dec(a_) + ", " + dec(b_) + " " + rp();
    };

    return pp::call(if_(eqz(b_), pp::tup(return_), pp::tup(continue_)), a_, b_);
  };

  b = [&](arg a_, arg b_) {
    docs << "mutually recursive side B";
    def<"continue(a, b)"> continue_ = [&](arg a_, arg b_) {
      return a + " " + lp() + " " + dec(a_) + ", " + dec(b_) + " " + rp();
    };

    return pp::call(if_(eqz(b_), pp::tup(return_), pp::tup(continue_)), a_, b_);
  };

  return pp::call(x(rest(args)), a(uint(first(args)), uint(rest(args))));
});

} // namespace api

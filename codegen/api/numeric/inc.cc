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

#include "numeric.h"

namespace api {

using namespace codegen;

decltype(inc) inc = NIFTY_DEF(inc, [&](va args) {
  docs << "numerical increment w/ overflow.";

  static_assert(conf::word_size > 1, "TODO");

  tests << inc(0)                            = "1" >> docs;
  tests << inc("1u")                         = "2u" >> docs;
  tests << inc(int_max_s)                    = int_min_s >> docs;
  tests << inc("0x" + utl::cat(samp::himax)) = (int_min_s) >> docs;

  if constexpr (conf::word_size > 1) {
    tests << inc("15u")      = "16u" >> docs;
    tests << inc(uint_max_s) = "0u" >> docs;
  } else {
    tests << inc("15u") = "0u" >> docs;
  }

  def<"x(...)"> x = [&](va args) { return args; };

  def init =
      def{"init(" + utl::cat(utl::alpha_base52_seq(conf::word_size), ", ") + ")"} = [&](pack args) {
        svect head = svect(args.begin(), std::prev(args.end()));
        return utl::cat(head, ", ") + ", " + esc + " " + impl::hex(args.back(), "INC");
      };

  def<"r0(...)"> r0 = [&](va args) {
    def o = def{"o(" + utl::cat(utl::alpha_base52_seq(conf::word_size - 1), ", ")
                + ", _carry, _inc)"} = [&](pack args) {
      def<"0(b)"> _0 = [&](arg b) { return "0, " + b; };
      def<"1(b)">{}  = [&](arg b) { return x(esc + " " + impl::hex(b, "INC")); };

      return args.back() + ", " + args.front() + ", "
           + pp::call(pp::cat(utl::slice(_0, -1), *(args.rbegin() + 1)), args[1]);
    };

    return o(args);
  };

  def<"r1(...)"> r1 = [&](va args) {
    def o = def{"o(" + utl::cat(utl::alpha_base52_seq(conf::word_size - 1), ", ")
                + ", _carry, _inc)"} = [&](pack args) { //
      return args.back() + ", " + args.front() + ", 0, " + args[1];
    };

    return o(args);
  };

  def<"res(...)"> res = [&](va args) {
    def o = def{"o(_hint, " + utl::cat(utl::alpha_base52_seq(conf::word_size - 1), ", ")
                + ", _carry, _inc)"} = [&](pack args) { //
      return word(pp::tup(args[1], args[2], args.back()), args.front());
    };
    return o(args);
  };

  return res(typeof(args), def<"o(...)">{[&](va args) {
               return r1(utl::cat(svect(conf::word_size - 1, r0 + "(")) + args
                         + utl::cat(svect(conf::word_size - 1, ")")));
             }}(x(init + " " + utup(args))));
});

} // namespace api

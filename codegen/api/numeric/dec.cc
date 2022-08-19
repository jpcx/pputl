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

decltype(dec) dec = NIFTY_DEF(dec, [&](va args) {
  docs << "numerical decrement w/ underflow.";

  static_assert(conf::word_size > 1, "TODO");

  tests << dec(1)         = "0" >> docs;
  tests << dec("2u")      = "1u" >> docs;
  tests << dec(0)         = ("0x" + utl::cat(samp::hmax)) >> docs;
  tests << dec(int_min_s) = ("0x" + utl::cat(samp::himax)) >> docs;

  if constexpr (conf::word_size > 1)
    tests << dec("16u") = "15u" >> docs;

  tests << dec("0u") = uint_max_s >> docs;

  def<"x(...)"> x = [&](va args) { return args; };

  def init =
      def{"init(" + utl::cat(utl::alpha_base52_seq(conf::word_size), ", ") + ")"} = [&](pack args) {
        svect head = svect(args.begin(), std::prev(args.end()));
        return utl::cat(head, ", ") + ", " + esc + " " + impl::hex(args.back(), "DEC");
      };

  def<"r0(...)"> r0 = [&](va args) {
    def o = def{"o(" + utl::cat(utl::alpha_base52_seq(conf::word_size - 1), ", ")
                + ", _carry, _dec)"} = [&](pack args) {
      def<"0(b)"> _0 = [&](arg b) { return "0, " + b; };
      def<"1(b)">{}  = [&](arg b) { return x(esc + " " + impl::hex(b, "DEC")); };

      return args.back() + ", " + args.front() + ", "
           + pp::call(pp::cat(utl::slice(_0, -1), *(args.rbegin() + 1)), args[1]);
    };

    return o(args);
  };

  def<"r1(...)"> r1 = [&](va args) {
    def o = def{"o(" + utl::cat(utl::alpha_base52_seq(conf::word_size - 1), ", ")
                + ", _carry, _dec)"} = [&](pack args) { //
      return args.back() + ", " + args.front() + ", 0, " + args[1];
    };

    return o(args);
  };

  def<"res(...)"> res = [&](va args) {
    def o = def{"o(_hint, " + utl::cat(utl::alpha_base52_seq(conf::word_size - 1), ", ")
                + ", _carry, _dec)"} = [&](pack args) { //
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

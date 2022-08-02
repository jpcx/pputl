////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////

#include "control.h"

namespace api {

using namespace codegen;

decltype(switch_) switch_ = NIFTY_DEF(switch_, [&](va args) {
  docs << "conditionally expands items based on a uint.";

  std::vector<std::string> maxcases{};
  for (size_t i = 0; i <= conf::uint_max; ++i)
    maxcases.push_back(pp::tup(utl::alpha_base52(i)));
  auto maxcases_s = utl::cat(maxcases, ", ");

  tests << switch_(0, "(1)")                                     = "1" >> docs;
  tests << switch_(0, "(1), (2)")                                = "1";
  tests << switch_(1, "(1), (2)")                                = "2" >> docs;
  tests << switch_(1, "(1), (2), (3)")                           = "2";
  tests << switch_(2, "(1), (2), (3)")                           = "3";
  tests << switch_(2, "(1), (2), (3, 4)")                        = "3, 4" >> docs;
  tests << switch_(0, maxcases_s)                                = "a";
  tests << switch_(1, maxcases_s)                                = "b";
  tests << switch_(conf::uint_max, maxcases_s)                   = utl::alpha_base52(conf::uint_max);
  tests << str(switch_(0, pp::tup(xct)))                         = pp::str(xct_expected(4));
  tests << str(switch_(1, "()", pp::tup(xct)))                   = pp::str(xct_expected(6));
  tests << str(switch_(2, "()", "()", pp::tup(xct)))             = pp::str(xct_expected(8));
  tests << str(switch_(3, "()", "()", "()", pp::tup(xct)))       = pp::str(xct_expected(10));
  tests << str(switch_(4, "()", "()", "()", "()", pp::tup(xct))) = pp::str(xct_expected(12));

  def<"a(i)"> a;
  def<"b(i)"> b;

  a = [&](arg i) {
    docs << "mutually recursive branch selector A";
    def<"cont(i, _, ...)"> cont = [&](arg i, arg, va args) {
      return b + " " + lp + " " + dec(i) + " " + rp + " " + pp::tup(dec(i), args);
    };
    def<"break(i, _, ...)"> break_ = [&](arg, arg _, va args) {
      docs << "A branches";
      return esc + " " + _;
    };
    return if_(eqz(i), pp::tup(break_), pp::tup(cont));
  };

  b = [&](arg i) {
    docs << "mutually recursive branch selector B";
    def<"cont(i, _, ...)"> cont = [&](arg i, arg, va args) {
      return a + " " + lp + " " + dec(i) + " " + rp + " " + pp::tup(dec(i), args);
    };
    def<"break(i, _, ...)"> break_ = [&](arg, arg _, va args) {
      docs << "B branches";
      return esc + " " + _;
    };
    return if_(eqz(i), pp::tup(break_), pp::tup(cont));
  };

  return pp::call(x(first(args)), pp::call(a(uint(first(args))), args));
});

} // namespace api

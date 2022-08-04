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

#include "control.h"

namespace api {

using namespace codegen;

decltype(switch_) switch_ = NIFTY_DEF(switch_, [&](va args) {
  docs << "conditionally expands items based on a uint."
       << "the final tuple is the default case."
       << ""
       << "throws if no cases, non-uint cs, or any non-tuple cases.";

  std::vector<std::string> maxcases{};
  for (size_t i = 0; i <= conf::uint_max; ++i)
    maxcases.push_back(pp::tup(utl::alpha_base52(i)));
  auto maxcases_s = utl::cat(maxcases, ", ");

  tests << switch_(0, "(1)")                           = "1" >> docs;
  tests << switch_(1, "(1)")                           = "1" >> docs;
  tests << switch_(2, "(1)")                           = "1" >> docs;
  tests << switch_(0, "(1), (2)")                      = "1";
  tests << switch_(1, "(1), (2)")                      = "2" >> docs;
  tests << switch_(2, "(1), (2)")                      = "2" >> docs;
  tests << switch_(1, "(1), (2), (3)")                 = "2";
  tests << switch_(2, "(1), (2), (3)")                 = "3";
  tests << switch_(2, "(1), (2), (3, 4)")              = "3, 4" >> docs;
  tests << switch_(conf::uint_max, "(1), (2), (3, 4)") = "3, 4" >> docs;
  tests << switch_(0, maxcases_s)                      = "a";
  tests << switch_(1, maxcases_s)                      = "b";
  tests << switch_(conf::uint_max, maxcases_s)         = utl::alpha_base52(conf::uint_max);
  tests << str(switch_())                              = pp::str(switch_()) >> docs;
  tests << str(switch_(0))                             = pp::str(switch_(0)) >> docs;
  tests << str(switch_(0, "foo"))                      = pp::str(switch_(0, "foo")) >> docs;
  tests << str(switch_(1, "(foo)", "(bar)"))           = pp::str("bar") >> docs;
  tests << str(switch_(1, "foo", "(bar)")) = pp::str(switch_(1, "foo", "(bar)")) >> docs;

  def<"throw(...)"> throw_ = [&](va args) {
    docs << "terminates expansion";
    return switch_(args);
  };

  def<"a(i, ...)"> a;
  def<"b(i, ...)"> b;

  a = [&](arg i, va args) {
    docs << "mutually recursive branch selector A";
    def<"cont(va, i, _, ...)"> cont = [&](arg va_, arg i, arg _, va args) {
      def<"pass(va, i, _, ...)"> pass = [&](arg va_, arg i, arg, va args) {
        return b + " " + lp() + " " + dec(i) + ", " + args + " " + rp() + " "
             + pp::tup(va_, dec(i), args);
      };
      def<"fail(va, ...)"> fail = [&](arg va_, va) {
        return throw_ + " " + va_;
      };
      return pp::call(if_(is_tuple(_), pp::tup(pass), pp::tup(fail)), va_, i, _, args);
    };
    def<"break(va, i, _, ...)"> break_ = [&](arg va_, arg, arg _, va) {
      docs << "A branches";
      return if_(is_tuple(_), pp::tup(esc + " " + _), pp::tup(throw_ + " " + va_));
    };
    return if_(if_(eqz(i), pp::tup(1), pp::tup(is_none(rest(args)))), pp::tup(break_),
               pp::tup(cont));
  };

  b = [&](arg i, va args) {
    docs << "mutually recursive branch selector B";
    def<"cont(va, i, _, ...)"> cont = [&](arg va_, arg i, arg _, va args) {
      def<"pass(va, i, _, ...)"> pass = [&](arg va_, arg i, arg, va args) {
        return a + " " + lp() + " " + dec(i) + ", " + args + " " + rp() + " "
             + pp::tup(va_, dec(i), args);
      };
      def<"fail(va, ...)"> fail = [&](arg va_, va) {
        return throw_ + " " + va_;
      };
      return pp::call(if_(is_tuple(_), pp::tup(pass), pp::tup(fail)), va_, i, _, args);
    };
    def<"break(va, i, _, ...)"> break_ = [&](arg va_, arg, arg _, va) {
      docs << "B branches";
      return if_(is_tuple(_), pp::tup(esc + " " + _), pp::tup(throw_ + " " + va_));
    };
    return if_(if_(eqz(i), pp::tup(1), pp::tup(is_none(rest(args)))), pp::tup(break_),
               pp::tup(cont));
  };

  def<"init(i, ...)"> init = [&](arg i, va args) {
    docs << "verifies uint cs and spawns recursion";
    return if_(is_uint(i), pp::tup(pp::call(x(i), pp::call(a(i, args), pp::tup(i, args), i, args))),
               pp::tup(throw_(i, args)));
  };

  return pp::call(if_(is_some(rest(args)), pp::tup(init), pp::tup(throw_)), args);
});

} // namespace api

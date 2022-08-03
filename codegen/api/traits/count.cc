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

#include "traits.h"

namespace api {

using namespace codegen;

decltype(count) count = NIFTY_DEF(count, [&](va args) {
  docs << "computes the uint quantity of args."
       << "terminates expansion with an error if too many args passed.";

  tests << count()                                = "0" >> docs;
  tests << count("a")                             = "1" >> docs;
  tests << count("a, b")                          = "2" >> docs;
  tests << count("a, b, c")                       = "3";
  tests << count(utl::base10_seq(conf::uint_max)) = uint_max_s;
  tests << count(", ")                            = "2" >> docs;
  tests << count(", , ")                          = "3";
  tests << count("a, ")                           = "2";
  tests << count("a, , ")                         = "3";
  tests << count(", a")                           = "2";
  tests << count(", a, ")                         = "3";
  tests << count(", , a")                         = "3";

  def<"a(i, ...)"> a;
  def<"b(i, ...)"> b;

  def<"return(i, ...)"> return_ = [&](arg i, va) {
    return dec(i);
  };

  def<"throw(...)"> throw_ = [&](va) {
    docs << "exit macros";
    return count("Error : too many args");
  };

  a = [&](arg i, va args) {
    docs << "mutually recursive branch A";
    def<"cont(i, _, ...)"> cont_ = [&](arg i, arg, va args) {
      return b + " " + lp + " " + inc(i) + ", " + args + " " + rp;
    };
    return pp::call(
        if_(is_none(args), pp::tup(return_), pp::tup(if_(eqz(i), pp::tup(throw_), pp::tup(cont_)))), i, args);
  };

  b = [&](arg i, va args) {
    docs << "mutually recursive branch B";
    def<"cont(i, _, ...)"> cont_ = [&](arg i, arg, va args) {
      return a + " " + lp + " " + inc(i) + ", " + args + " " + rp;
    };
    return pp::call(
        if_(is_none(args), pp::tup(return_), pp::tup(if_(eqz(i), pp::tup(throw_), pp::tup(cont_)))), i, args);
  };

  return if_(is_none(args), pp::tup(0), pp::tup(pp::call(x(conf::uint_max - 1), a(1, args + ","))));
});

} // namespace api

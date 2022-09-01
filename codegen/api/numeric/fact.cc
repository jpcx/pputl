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

decltype(fact) fact = NIFTY_DEF(fact, [&](va args) {
  docs << "numeric prime factorization. value must be non-negative."
       << "casts each result according to the input.";

  tests << fact("0u")  = "" >> docs;
  tests << fact(13)    = "" >> docs;
  tests << fact(4)     = "2, 2" >> docs;
  tests << fact("12u") = "2u, 2u, 3u" >> docs;

  def<"x(...)"> x = [&](va args) { return args; };

  def<"update(t, _, ...)"> update = [&](arg t, arg first, va args) {
    return pp::tup(args, word(first, t));
  };

  def<"init(...)"> init = [&](va args) {
    def<"<\\0(t, ...)"> _0    = [&](arg t, va) { return "(), " + t; };
    def<"<\\01(t, _, ...)">{} = [&](arg t, arg first, va rest) {
      return pp::tup(word(first, t)) + ", " + t + ", " + rest;
    };
    return def<"o(t, ...)">{[&](arg t, va args) {
      return pp::call(pp::cat(_0, pp::va_opt(1)), t, args);
    }}(args);
  };

  def<"r(...)"> r = [&](va args) {
    def<"<\\0(a, t, ...)"> _0    = [&](arg a, arg t, va) { return a + ", " + t; };
    def<"<\\01(a, t, _, ...)">{} = [&](arg a, arg t, arg first, va rest) {
      return update(t, first, esc + " " + a) + ", " + t + ", " + rest;
    };
    return def<"o(a, t, ...)">{[&](arg a, arg t, va args) {
      return pp::call(pp::cat(_0, pp::va_opt(1)), a, t, args);
    }}(args);
  };

  def<"\\0(e, r, n)"> _0 = [&](arg, arg r, arg n) {
    return x(esc + " "
             + xfirst(utl::cat(svect{conf::bit_length - 2, r + "("}) + " "
                      + init(typeof(n) + ", " + impl::udec(udec(n), "FACT"))
                      + utl::cat(svect{conf::bit_length - 2, ")"})));
  };
  def<"\\1(e, r, n)">{} = [&](arg e, arg, arg) { return fail(e); };

  return pp::call(
      xcat(utl::slice(_0, -1), ltz(args)),
      str(pp::str("[" + fact + "] value must be non-negative") + " : " + args), r, args);
});

} // namespace api

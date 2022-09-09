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
//         This program is free software: you can redistribute it and/or modify
//         it under the terms of the GNU General Public License as published by
//         the Free Software Foundation, either version 3 of the License, or
//         (at your option) any later version.
//
//         This program is distributed in the hope that it will be useful,
//         but WITHOUT ANY WARRANTY; without even the implied warranty of
//         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//         GNU General Public License for more details.
//                                                                            //
//  You should have received a copy of the GNU General Public License        ///
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.  ////
///////////////////////////////////////////////////////////////////////////// */

#include "range.h"

namespace api {

using namespace codegen;

decltype(lpush) lpush = NIFTY_DEF(lpush, [&](va args) {
  docs << "pushes items to the front of a tuple.";

  tests << lpush("()")               = "()" >> docs;
  tests << lpush("()", 'a')          = "(a)" >> docs;
  tests << lpush("(a)", 'b')         = "(b, a)" >> docs;
  tests << lpush("(b, a)", 'c', 'd') = "(c, d, b, a)" >> docs;
  tests << lpush("(c, d, b, a)")     = "(c, d, b, a)" >> docs;

  return def<"o(t, ...)">{[&](arg t, va args) {
    def<"\\00(t, ...)"> _00 = [&](arg t, va args) {
      return pp::tup(args, esc + " " + t);
    };
    def<"\\01(t, ...)">{} = [&](arg t, va) {
      return t;
    };
    def<"\\10(t, ...)">{} = [&](arg, va args) {
      return pp::tup(args);
    };
    def<"\\11(t, ...)">{} = [&](arg, va) {
      return "()";
    };

    return pp::call(xcat(utl::slice(_00, -2), xcat(is_empty(t), is_none(args))), t, args);
  }}(args);
});

} // namespace api

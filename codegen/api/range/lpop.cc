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

decltype(lpop) lpop = NIFTY_DEF(lpop, [&](va args) {
  docs << "removes the first n items from a tuple."
       << "n must be less than or equal to the tuple size.";

  tests << lpop("()", 0)        = "()" >> docs;
  tests << lpop("(a)", 0)       = "(a)" >> docs;
  tests << lpop("(a)", 1)       = "()" >> docs;
  tests << lpop("(a, b)")       = "(b)" >> docs;
  tests << lpop("(a, b)", 0)    = "(a, b)";
  tests << lpop("(a, b)", 1)    = "(b)" >> docs;
  tests << lpop("(a, b)", 2)    = "()" >> docs;
  tests << lpop("(a, b, c)", 0) = "(a, b, c)";
  tests << lpop("(a, b, c)", 1) = "(b, c)";
  tests << lpop("(a, b, c)", 2) = "(c)" >> docs;
  tests << lpop("(a, b, c)", 3) = "()";

  return def<"o(t, ...)">{[&](arg t, va sz) {
    return def<"<o(...)">{[&](va args) {
      return def<"<o(head, tail, type)">{[&](arg, arg tail, arg) {
        return tail;
      }}(args);
    }}(bisect(t, default_(1, sz)));
  }}(args);
});

} // namespace api

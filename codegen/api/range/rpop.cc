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

decltype(rpop) rpop = NIFTY_DEF(rpop, [&](va args) {
  docs << "removes the last n items from a tuple."
       << "n must be less than or equal to the tuple size.";

  tests << rpop("()", 0)        = "()" >> docs;
  tests << rpop("(a)", 0)       = "(a)" >> docs;
  tests << rpop("(a)", 1)       = "()" >> docs;
  tests << rpop("(a, b)")       = "(a)" >> docs;
  tests << rpop("(a, b)", 0)    = "(a, b)";
  tests << rpop("(a, b)", 1)    = "(a)" >> docs;
  tests << rpop("(a, b)", 2)    = "()" >> docs;
  tests << rpop("(a, b, c)", 0) = "(a, b, c)";
  tests << rpop("(a, b, c)", 1) = "(a, b)";
  tests << rpop("(a, b, c)", 2) = "(a)" >> docs;
  tests << rpop("(a, b, c)", 3) = "()";

  return def<"o(t, ...)">{[&](arg t, va sz) {
    return xfirst(bisect(t, size(sub(sizeof_(t), default_(1, sz)))));
  }}(args);
});

} // namespace api

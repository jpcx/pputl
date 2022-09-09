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

decltype(head) head = NIFTY_DEF(head, [&](va args) {
  docs << "returns a tuple of the first n elements of a tuple."
       << "n must be less than or equal to the tuple size.";

  auto bigargs = utl::alpha_base52_seq(conf::size_max);
  auto bigtup  = pp::tup(bigargs);

  tests << head("(a)", 0)       = "()" >> docs;
  tests << head("(a)", 1)       = "(a)" >> docs;
  tests << head("(a)")          = "(a)" >> docs;
  tests << head("()", 0)        = "()" >> docs;
  tests << head("(a, b)", 0)    = "()";
  tests << head("(a, b)", 1)    = "(a)";
  tests << head("(a, b, c)", 1) = "(a)" >> docs;
  tests << head("(a, b, c)", 2) = "(a, b)" >> docs;
  tests << head("(a, b, c)", 3) = "(a, b, c)" >> docs;
  tests << head("(, )", 0)      = "()" >> docs;
  tests << head("(, )", 1)      = "()" >> docs;
  tests << head("(, )", 2)      = "(,)" >> docs;
  tests << head("(, , )", 0)    = "()";
  tests << head("(, , )", 1)    = "()";
  tests << head("(, , )", 2)    = "(,)" >> docs;
  tests << head("(, , )", 3)    = "(,,)";
  tests << head("(a, )", 0)     = "()";
  tests << head("(a, )", 1)     = "(a)";
  tests << head("(a, )", 2)     = "(a,)";
  tests << head("(a, , )", 0)   = "()";
  tests << head("(a, , )", 1)   = "(a)";
  tests << head("(a, , )", 2)   = "(a,)";
  tests << head("(a, , )", 3)   = "(a,,)";
  tests << head("(, a)", 0)     = "()";
  tests << head("(, a)", 1)     = "()";
  tests << head("(, a)", 2)     = "(, a)";
  tests << head("(, a, )", 0)   = "()";
  tests << head("(, a, )", 1)   = "()";
  tests << head("(, a, )", 2)   = "(, a)";
  tests << head("(, a, )", 3)   = "(, a,)";
  tests << head("(, , a)", 0)   = "()";
  tests << head("(, , a)", 1)   = "()";
  tests << head("(, , a)", 2)   = "(,)";
  tests << head("(, , a)", 3)   = "(,, a)";

  tests << head(bigtup, 0)              = pp::tup();
  tests << head(bigtup, 1)              = pp::tup(bigargs[0]);
  tests << head(bigtup, 2)              = pp::tup(bigargs[0], bigargs[1]);
  tests << head(bigtup, conf::size_max) = bigtup;
  tests << head(bigtup, conf::size_max - 1) =
      pp::tup(svect{bigargs.begin(), bigargs.end() - 1});
  tests << head(bigtup, conf::size_max - 2) =
      pp::tup(svect{bigargs.begin(), bigargs.end() - 2});

  return def<"o(t, ...)">{[&](arg t, va sz) {
    return xfirst(bisect(t, size(default_(1, sz))));
  }}(args);
});

} // namespace api

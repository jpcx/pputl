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

decltype(tail) tail = NIFTY_DEF(tail, [&](va args) {
  docs << "returns a tuple of the last n elements of a tuple."
       << "n must be less than or equal to the tuple size.";

  auto bigargs = utl::alpha_base52_seq(conf::size_max);
  auto bigtup  = pp::tup(bigargs);

  tests << tail("(a)", 0)       = "()" >> docs;
  tests << tail("(a)", 1)       = "(a)" >> docs;
  tests << tail("(a)")          = "(a)" >> docs;
  tests << tail("()", 0)        = "()" >> docs;
  tests << tail("(a, b)", 0)    = "()";
  tests << tail("(a, b)", 1)    = "(b)";
  tests << tail("(a, b)", 2)    = "(a, b)";
  tests << tail("(a, b, c)", 1) = "(c)" >> docs;
  tests << tail("(a, b, c)", 2) = "(b, c)" >> docs;
  tests << tail("(a, b, c)", 3) = "(a, b, c)" >> docs;
  tests << tail("(, )", 0)      = "()" >> docs;
  tests << tail("(, )", 1)      = "()" >> docs;
  tests << tail("(, )", 2)      = "(,)" >> docs;
  tests << tail("(, , )", 0)    = "()";
  tests << tail("(, , )", 1)    = "()";
  tests << tail("(, , )", 2)    = "(,)" >> docs;
  tests << tail("(, , )", 3)    = "(, ,)";
  tests << tail("(a, )", 0)     = "()";
  tests << tail("(a, )", 1)     = "()";
  tests << tail("(a, )", 2)     = "(a,)";
  tests << tail("(a, , )", 0)   = "()";
  tests << tail("(a, , )", 1)   = "()";
  tests << tail("(a, , )", 2)   = "(,)";
  tests << tail("(a, , )", 3)   = "(a, ,)";
  tests << tail("(, a)", 0)     = "()";
  tests << tail("(, a)", 1)     = "(a)";
  tests << tail("(, a)", 2)     = "(, a)";
  tests << tail("(, a, )", 0)   = "()";
  tests << tail("(, a, )", 1)   = "()";
  tests << tail("(, a, )", 2)   = "(a,)";
  tests << tail("(, a, )", 3)   = "(, a,)";
  tests << tail("(, , a)", 0)   = "()";
  tests << tail("(, , a)", 1)   = "(a)";
  tests << tail("(, , a)", 2)   = "(, a)";
  tests << tail("(, , a)", 3)   = "(, , a)";

  tests << tail(bigtup, 0) = pp::tup();
  tests << tail(bigtup, 1) = pp::tup(bigargs.back());
  tests << tail(bigtup, 2) = pp::tup(*(bigargs.rbegin() + 1), bigargs.back());
  tests << tail(bigtup, conf::size_max) = bigtup;
  tests << tail(bigtup, conf::size_max - 1) =
      pp::tup(svect{bigargs.begin() + 1, bigargs.end()});
  tests << tail(bigtup, conf::size_max - 2) =
      pp::tup(svect{bigargs.begin() + 2, bigargs.end()});

  return def<"o(t, ...)">{[&](arg t, va sz) {
    return def<"<o(...)">{[&](va args) {
      return def<"<o(head, tail, type)">{[&](arg, arg tail, arg) {
        return tail;
      }}(args);
    }}(bisect(t, size(sub(countof(itemsof(t)), default_(1, sz)))));
  }}(args);
});

} // namespace api

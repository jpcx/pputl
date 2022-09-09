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

decltype(bisect) bisect = NIFTY_DEF(bisect, [&](va args) {
  docs << "splits a tuple in two at a given index."
       << ""
       << "abs(index) must be less than or equal to the tuple size."
       << ""
       << "returns:"
       << "  (1) head: a tuple of the items strictly before the index"
       << "  (2) tail: a tuple of the items from the index to the end"
       << "  (3) type: 1 if two empty elements were split (information loss), else 0";

  auto bigargs = utl::alpha_base52_seq(conf::size_max);
  auto bigtup  = pp::tup(bigargs);

  tests << bisect("(a)", 0)            = "(), (a), 0" >> docs;
  tests << bisect("(a)", 1)            = "(a), (), 0" >> docs;
  tests << bisect("()", 0)             = "(), (), 0" >> docs;
  tests << bisect("(a, b)", 0)         = "(), (a, b), 0";
  tests << bisect("(a, b)", 1)         = "(a), (b), 0";
  tests << bisect("(a, b, c)", 1)      = "(a), (b, c), 0" >> docs;
  tests << bisect("(a, b, c)", 2)      = "(a, b), (c), 0" >> docs;
  tests << bisect("(a, b, c)", 3)      = "(a, b, c), (), 0" >> docs;
  tests << bisect("(a, b, c)", neg(1)) = "(a, b), (c), 0" >> docs;
  tests << bisect("(a, b, c)", neg(2)) = "(a), (b, c), 0" >> docs;
  tests << bisect("(a, b, c)", neg(3)) = "(), (a, b, c), 0" >> docs;
  tests << bisect("(, )", 0)           = "(), (,), 0" >> docs;
  tests << bisect("(, )", 1)           = "(), (), 1" >> docs;
  tests << bisect("(, )", 2)           = "(,), (), 0" >> docs;
  tests << bisect("(, , )", 0)         = "(), (, ,), 0";
  tests << bisect("(, , )", 1)         = "(), (,), 1";
  tests << bisect("(, , )", 2)         = "(,), (), 1" >> docs;
  tests << bisect("(, , )", 3)         = "(,,), (), 0";
  tests << bisect("(a, )", 0)          = "(), (a,), 0";
  tests << bisect("(a, )", 1)          = "(a), (), 1";
  tests << bisect("(a, )", 2)          = "(a,), (), 0";
  tests << bisect("(a, , )", 0)        = "(), (a, ,), 0";
  tests << bisect("(a, , )", 1)        = "(a), (,), 1";
  tests << bisect("(a, , )", 2)        = "(a,), (), 1";
  tests << bisect("(a, , )", 3)        = "(a,,), (), 0";
  tests << bisect("(, a)", 0)          = "(), (, a), 0";
  tests << bisect("(, a)", 1)          = "(), (a), 1";
  tests << bisect("(, a)", 2)          = "(, a), (), 0";
  tests << bisect("(, a, )", 0)        = "(), (, a,), 0";
  tests << bisect("(, a, )", 1)        = "(), (a,), 1";
  tests << bisect("(, a, )", 2)        = "(, a), (), 1";
  tests << bisect("(, a, )", 3)        = "(, a,), (), 0";
  tests << bisect("(, , a)", 0)        = "(), (, , a), 0";
  tests << bisect("(, , a)", 1)        = "(), (, a), 1";
  tests << bisect("(, , a)", 2)        = "(,), (a), 1";
  tests << bisect("(, , a)", 3)        = "(,, a), (), 0";

  tests << bisect(bigtup, 0) = pp::tup() + ", " + bigtup + ", 0";
  tests << bisect(bigtup, 1) = pp::tup(bigargs[0]) + ", "
                             + pp::tup(svect{bigargs.begin() + 1, bigargs.end()}) + ", 0";
  tests << bisect(bigtup, 2) = pp::tup(bigargs[0], bigargs[1]) + ", "
                             + pp::tup(svect{bigargs.begin() + 2, bigargs.end()}) + ", 0";
  tests << bisect(bigtup, conf::size_max) = bigtup + ", " + pp::tup() + ", 0";
  tests << bisect(bigtup, conf::size_max - 1) =
      pp::tup(svect{bigargs.begin(), bigargs.end() - 1}) + ", "
      + pp::tup(*bigargs.rbegin()) + ", 0";
  tests << bisect(bigtup, conf::size_max - 2) =
      pp::tup(svect{bigargs.begin(), bigargs.end() - 2}) + ", "
      + pp::tup(*(bigargs.rbegin() + 1), *bigargs.rbegin()) + ", 0";
  tests << bisect(bigtup, neg(1)) = pp::tup(svect{bigargs.begin(), bigargs.end() - 1})
                                  + ", " + pp::tup(*bigargs.rbegin()) + ", 0";
  tests << bisect(bigtup, neg(2)) =
      pp::tup(svect{bigargs.begin(), bigargs.end() - 2}) + ", "
      + pp::tup(*(bigargs.rbegin() + 1), *bigargs.rbegin()) + ", 0";

  def<"x(...)"> x = [&](va args) {
    return args;
  };

  def<"type(_, __, ...)"> type = [&](arg _0, arg _1, va args) {
    return or_(and_(is_none(_0), not_(is_none(args))), is_none(_1));
  };

  def<"r(...)"> r = [&](va args) {
    return def<"o(head, _, ...)">{[&](arg head, arg first, va rest) {
      return pp::tup(esc + " " + head, first) + ", " + rest;
    }}(args);
  };

  def<"res(...)"> res = [&](va args) {
    return def<"o(head, ...)">{[&](arg head, va args) {
      return pp::tup(xrest(esc + " " + head, first(args))) + ", " + pp::tup(rest(args))
           + ", " + type(args, ".");
    }}(args);
  };

  def<"eqz(n, ...)"> eq = [&](arg, va range) {
    return "(), " + pp::tup(range) + ", 0";
  };
  def<"nez(n, ...)"> ne = [&](arg n, va range) {
    return def<"o(n, ...)">{[&](arg n, va range) {
      return res(recur(x, n, r, "()", range));
    }}(dec(n), range);
  };

  return def<"o(t, i)">{[&](arg t, arg i) {
    return def<"<o(i, ...)">{[&](arg i, va range) {
      return def<"<o(n, ...)">{[&](arg n, va range) {
        return pp::call(if_(eqz(n), eq, ne), n, range);
      }}(index(countof(range), i), range);
    }}(i, items(t));
  }}(args);
});

} // namespace api

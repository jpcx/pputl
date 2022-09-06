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

#include "codegen.h"
#include "range.h"

namespace api {

using namespace codegen;

decltype(unite) unite = NIFTY_DEF(unite, [&](va args) {
  docs << "unites two ranges given an optional bisection type."
       << "type is used to preserve information when two empty elements are split.";

  tests << unite("(a)", "()")        = "(a)" >> docs;
  tests << unite("()", "(b)")        = "(b)" >> docs;
  tests << unite("(a)", "(b)")       = "(a, b)" >> docs;
  tests << unite("(a, b)", "(c, d)") = "(a, b, c, d)" >> docs;
  tests << bisect(0, "()")           = "(), (), 0" >> docs;
  tests << bisect(1, "(, )")         = "(), (), 1" >> docs;
  tests << unite("()", "()")         = "()" >> docs;
  tests << unite("()", "()", 0)      = "()" >> docs;
  tests << unite("()", "()", 1)      = "(, )" >> docs;
  tests << unite(bisect(0, "()"))    = "()" >> docs;
  tests << unite(bisect(1, "(, )"))  = "(, )" >> docs;
  tests << unite(pp::tup('a'), pp::tup(utl::alpha_base52_seq(conf::size_max - 1, "b"))) =
      pp::tup(utl::alpha_base52_seq(conf::size_max));
  tests << unite(pp::tup('a'), "(, )")    = "(a, ,)";
  tests << unite(pp::tup('a'), "(, , )")  = "(a, , ,)";
  tests << unite(pp::tup('a'), "(b, )")   = "(a, b,)";
  tests << unite(pp::tup('a'), "(b, , )") = "(a, b, ,)";
  tests << unite(pp::tup('a'), "(, b)")   = "(a, , b)";
  tests << unite(pp::tup('a'), "(, b, )") = "(a, , b,)";
  tests << unite(pp::tup('a'), "(, , b)") = "(a, , , b)";
  tests << unite(bisect(0, "(a)"))        = "(a)";
  tests << unite(bisect(1, "(a)"))        = "(a)";
  tests << unite(bisect(0, "(a, b)"))     = "(a, b)";
  tests << unite(bisect(1, "(a, b)"))     = "(a, b)";
  tests << unite(bisect(1, "(a, b, c)"))  = "(a, b, c)";
  tests << unite(bisect(2, "(a, b, c)"))  = "(a, b, c)";
  tests << unite(bisect(3, "(a, b, c)"))  = "(a, b, c)";
  tests << unite(bisect(0, "(, )"))       = "(,)";
  tests << unite(bisect(2, "(, )"))       = "(,)";
  tests << unite(bisect(0, "(, , )"))     = "(, ,)";
  tests << unite(bisect(1, "(, , )"))     = "(, ,)";
  tests << unite(bisect(2, "(, , )"))     = "(,, )";
  tests << unite(bisect(3, "(, , )"))     = "(,,)";
  tests << unite(bisect(0, "(a, )"))      = "(a,)";
  tests << unite(bisect(1, "(a, )"))      = "(a, )";
  tests << unite(bisect(2, "(a, )"))      = "(a,)";
  tests << unite(bisect(0, "(a, , )"))    = "(a, ,)";
  tests << unite(bisect(1, "(a, , )"))    = "(a, ,)";
  tests << unite(bisect(2, "(a, , )"))    = "(a,, )";
  tests << unite(bisect(3, "(a, , )"))    = "(a,,)";
  tests << unite(bisect(0, "(, a)"))      = "(, a)";
  tests << unite(bisect(1, "(, a)"))      = "(, a)";
  tests << unite(bisect(2, "(, a)"))      = "(, a)";
  tests << unite(bisect(0, "(, a, )"))    = "(, a,)";
  tests << unite(bisect(1, "(, a, )"))    = "(, a,)";
  tests << unite(bisect(2, "(, a, )"))    = "(, a, )";
  tests << unite(bisect(3, "(, a, )"))    = "(, a,)";
  tests << unite(bisect(0, "(, , a)"))    = "(, , a)";
  tests << unite(bisect(1, "(, , a)"))    = "(, , a)";
  tests << unite(bisect(2, "(, , a)"))    = "(,, a)";
  tests << unite(bisect(3, "(, , a)"))    = "(,, a)";
  tests << unite(bisect(0, "()"))         = "()";
  tests << unite(bisect(1, "(, )"))       = "(, )";

  def<"000(head, tail, ...)"> _000 = [&](arg head, arg tail, va) {
    return pp::tup(items(head) + ", " + items(tail));
  };
  def<"001(head, tail, ...)">{} = [&](arg head, arg, va) { return head; };
  def<"010(head, tail, ...)">{} = [&](arg, arg tail, va) { return tail; };
  def<"011(head, tail, ...)">{} = [&](pack) { return "()"; };
  def<"100(head, tail, ...)">{} = [&](arg head, arg tail, va) {
    return pp::tup(items(head) + ", " + items(tail));
  };
  def<"101(head, tail, ...)">{} = [&](arg head, arg, va) {
    return pp::tup(items(head) + ", ");
  };
  def<"110(head, tail, ...)">{} = [&](arg, arg tail, va) {
    return pp::tup(", " + items(tail));
  };
  def<"111(head, tail, ...)">{} = [&](arg, arg, va) { return "(, )"; };

  return pp::call(def<"o(head, tail, ...)">{[&](arg head, arg tail, va type) {
                    return xcat(xcat(utl::slice(_000, -3), bool_(default_(0, type))),
                                xcat(is_none(items(head)), is_none(items(tail))));
                  }}(args),
                  args);
});

} // namespace api

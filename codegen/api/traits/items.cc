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

decltype(items) items = NIFTY_DEF(items, [&](va args) {
  docs << "extracts tuple items. terminates expansion on non-tup.";

  tests << items("()")              = "" >> docs;
  tests << items("(a)")             = "a" >> docs;
  tests << items("(a, b)")          = "a, b" >> docs;
  tests << items("(a, b, c)")       = "a, b, c" >> docs;
  tests << str(items("(a, b, c)"))  = pp::str("a, b, c") >> docs;
  tests << str(items())             = pp::str(items()) >> docs;
  tests << str(items("not, tuple")) = pp::str(items("not, tuple")) >> docs;
  tests << items("((a), (b), (c))") = "(a), (b), (c)";
  tests << items("(, )")            = ",";
  tests << items("(, , )")          = ", ,";
  tests << items("(a, )")           = "a,";
  tests << items("(a, , )")         = "a, ,";
  tests << items("(, a)")           = ", a";
  tests << items("(, a, )")         = ", a,";
  tests << items("(, , a)")         = ", , a";

  def<"0(...)"> _0 = [&](va args) {
    docs << "non-tuple; throw";
    return items(args);
  };

  def<"1(...)">{} = [&](va args) {
    docs << "tuple; extract and return";
    return esc + " " + args;
  };

  return pp::call(cat(utl::slice(_0, -1), is_tuple(args)), args);
});

} // namespace api

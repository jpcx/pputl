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

decltype(itemsof) itemsof = NIFTY_DEF(itemsof, [&](va args) {
  docs << "extracts item from a tuple or array.";

  tests << itemsof("()")              = "" >> docs;
  tests << itemsof("(a)")             = "a" >> docs;
  tests << itemsof("(a, b)")          = "a, b" >> docs;
  tests << itemsof("(a, b, c)")       = "a, b, c" >> docs;
  tests << itemsof("((a), (b), (c))") = "(a), (b), (c)";
  tests << itemsof("(, )")            = ",";
  tests << itemsof("(, , )")          = ", ,";
  tests << itemsof("(a, )")           = "a,";
  tests << itemsof("(a, , )")         = "a, ,";
  tests << itemsof("(, a)")           = ", a";
  tests << itemsof("(, a, )")         = ", a,";
  tests << itemsof("(, , a)")         = ", , a";

  return def<"x(...)">{[&](va args) {
    return esc + " " + args;
  }}(tuple(args));
});

} // namespace api

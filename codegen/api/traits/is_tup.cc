////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////

#include "traits.h"

namespace api {

using namespace codegen;

decltype(is_tup) is_tup = NIFTY_DEF(is_tup, [&](va args) {
  docs << "detects if args is a tuple.";

  tests << is_tup()                 = "0" >> docs;
  tests << is_tup("1, 2")           = "0" >> docs;
  tests << is_tup("()")             = "1" >> docs;
  tests << is_tup("(1, 2)")         = "1" >> docs;
  tests << is_tup("(), ()")         = "0";
  tests << is_tup(esc + "(())")     = "1";
  tests << is_tup(esc + "((1, 2))") = "1";
  tests << is_tup(", ")             = "0";
  tests << is_tup(", , ")           = "0";
  tests << is_tup("a, ")            = "0";
  tests << is_tup("a, , ")          = "0";
  tests << is_tup(", a")            = "0";
  tests << is_tup(", a, ")          = "0";
  tests << is_tup(", , a")          = "0";
  tests << is_tup("(, )")           = "1";
  tests << is_tup("(, , )")         = "1";
  tests << is_tup("(a, )")          = "1";
  tests << is_tup("(a, , )")        = "1";
  tests << is_tup("(, a)")          = "1";
  tests << is_tup("(, a, )")        = "1";
  tests << is_tup("(, , a)")        = "1";

  return is_none(eat + " " + args);
});

} // namespace api

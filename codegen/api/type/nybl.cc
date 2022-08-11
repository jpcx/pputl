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

#include "type.h"

namespace api {

using namespace codegen;

decltype(nybl) nybl = NIFTY_DEF(nybl, [&](va args) {
  docs << "[inherits from " + atom + "] nybl type (capital hex digit)."
       << "expands to n if valid, else fails.";

  tests << nybl(0)   = "0" >> docs;
  tests << nybl(1)   = "1" >> docs;
  tests << nybl(2)   = "2";
  tests << nybl(3)   = "3";
  tests << nybl(4)   = "4";
  tests << nybl(5)   = "5";
  tests << nybl(6)   = "6";
  tests << nybl(7)   = "7";
  tests << nybl(8)   = "8";
  tests << nybl(9)   = "9";
  tests << nybl("A") = "A";
  tests << nybl("B") = "B" >> docs;
  tests << nybl("C") = "C";
  tests << nybl("D") = "D";
  tests << nybl("E") = "E";
  tests << nybl("F") = "F" >> docs;

  def<"0(e, ...)"> _0 = [](arg e, va) {
    return fail(e);
  };

  def<"1(e, nybl)">{} = [](arg, arg nybl) {
    return nybl;
  };

  return def<"o(e, atom)">{[&](arg e, arg atom) {
    return pp::call(cat(utl::slice(_0, -1), detail::is_nybl_o(atom)), e, atom);
  }}(istr("[" + nybl
          + "] nybl cannot describe anything but literal, capital hex digits 0-F : " + args),
     atom(args));
});

} // namespace api

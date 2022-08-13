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

decltype(hex) hex = NIFTY_DEF(hex, [&](va args) {
  docs << "[inherits from " + atom + "] capital hex digit type."
       << "expands to h if valid, else fails.";

  tests << hex(0)   = "0" >> docs;
  tests << hex(1)   = "1" >> docs;
  tests << hex(2)   = "2";
  tests << hex(3)   = "3";
  tests << hex(4)   = "4";
  tests << hex(5)   = "5";
  tests << hex(6)   = "6";
  tests << hex(7)   = "7";
  tests << hex(8)   = "8";
  tests << hex(9)   = "9";
  tests << hex("A") = "A";
  tests << hex("B") = "B" >> docs;
  tests << hex("C") = "C";
  tests << hex("D") = "D";
  tests << hex("E") = "E";
  tests << hex("F") = "F" >> docs;

  def<"0(e, ...)"> _0 = [](arg e, va) {
    return fail(e);
  };

  def<"1(e, hex)">{} = [](arg, arg hex) {
    return hex;
  };

  return def<"o(e, atom)">{[&](arg e, arg atom) {
    return pp::call(cat(utl::slice(_0, -1), detail::is_hex_o(atom)), e, atom);
  }}(istr("[" + hex
          + "] hex cannot describe anything but literal, capital hex digits 0-F : " + args),
     atom(args));
});

} // namespace api

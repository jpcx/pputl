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
  docs << "[" + enum_
              + "<0|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F>] an uppercase hexadecimal digit."
       << "constructible from either hex or nybl.";

  tests << hex(0)      = "0" >> docs;
  tests << hex("F")    = "F" >> docs;
  tests << hex("0110") = "6" >> docs;
  tests << hex("1010") = "A" >> docs;

  def<"\\00(e, ...)"> _00 = [](arg e, va) {
    return fail(e);
  };
  def<"\\01(e, nybl)">{} = [](arg, arg nybl) {
    return impl::nybl(nybl, "HEX");
  };
  def<"\\10(e, hex)">{} = [](arg, arg hex) {
    return hex;
  };

  return pp::call(xcat(utl::slice(_00, -2), xcat(is_hex(args), is_nybl(args))),
                  error(hex, "invalid arguments; must be hex or nybl", args), args);
});

} // namespace api

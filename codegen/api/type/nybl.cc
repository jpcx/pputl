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
  docs << "[" + enum_ + "<0000|0001|...|1110|1111>] 4-bit bool concatenation type."
       << "constructible from either nybl or hex.";

  tests << nybl("0000") = "0000" >> docs;
  tests << nybl("0110") = "0110" >> docs;
  tests << nybl("5")    = "0101" >> docs;
  tests << nybl("A")    = "1010" >> docs;

  def<"\\00(e, ...)"> _00 = [](arg e, va) {
    return fail(e);
  };
  def<"\\01(e, hex)">{} = [](arg, arg hex) {
    return impl::hex(hex, "NYBL");
  };
  def<"\\10(e, nybl)">{} = [](arg, arg nybl) {
    return nybl;
  };

  return pp::call(xcat(utl::slice(_00, -2), xcat(is_nybl(args), is_hex(args))),
                  error(nybl, "invalid arguments; must be nybl or hex", args), args);
});

} // namespace api

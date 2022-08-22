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
  docs << "[inherits from " + atom + "] 4-bit bool concatenation type."
       << "constructible from either nybl or hex."
       << "expands to v if valid, else fails.";

  tests << nybl("0000") = "0000" >> docs;
  tests << nybl("0110") = "0110" >> docs;
  tests << nybl("5")    = "0101" >> docs;
  tests << nybl("A")    = "1010" >> docs;

  def<"\\00(e, ...)"> _00 = [](arg e, va) { return fail(e); };
  def<"\\01(e, hex)">{}   = [](arg, arg hex) { return impl::hex(hex, "NYBL"); };
  def<"\\10(e, nybl)">{}  = [](arg, arg nybl) { return nybl; };

  return def<"o(e, atom)">{[&](arg e, arg atom) {
    return pp::call(
        xcat(utl::slice(_00, -2), xcat(detail::is_nybl_o(atom), detail::is_hex_o(atom))),
        e, atom);
  }}(str("[" + nybl + "] invalid arguments; must be nybl or hex : " + args), atom(args));
});

} // namespace api

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

namespace detail {
decltype(is_nybl_o) is_nybl_o = NIFTY_DEF(is_nybl_o);
}

decltype(is_nybl) is_nybl = NIFTY_DEF(is_nybl, [&](va args) {
  docs << "[extends " + is_enum + "] detects if args is a 4-bit bool concatenation.";

  tests << is_nybl()       = "0" >> docs;
  tests << is_nybl(0)      = "0" >> docs;
  tests << is_nybl('B')    = "0" >> docs;
  tests << is_nybl("000")  = "0" >> docs;
  tests << is_nybl("0000") = "1" >> docs;
  tests << is_nybl("0110") = "1" >> docs;

  detail::is_nybl_o = def{"o(atom)"} = [&](arg atom) { return impl::nybl(atom, "IS"); };

  def<"\\0"> _0 = [&] { return def<"fail(...)">{[&](va) { return "0"; }}; };
  def<"\\1">{}  = [&] { return detail::is_nybl_o; };

  return pp::call(xcat(utl::slice(_0, -1), is_atom(args)), args);
});

} // namespace api

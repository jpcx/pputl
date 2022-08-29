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
decltype(is_hex_o) is_hex_o = NIFTY_DEF(is_hex_o);
}

decltype(is_hex) is_hex = NIFTY_DEF(is_hex, [&](va args) {
  docs << "[extends " + is_enum + "] detects if args is an uppercase hexadecimal digit.";

  tests << is_hex()      = "0" >> docs;
  tests << is_hex(0)     = "1" >> docs;
  tests << is_hex('Q')   = "0" >> docs;
  tests << is_hex("foo") = "0" >> docs;
  tests << is_hex('B')   = "1" >> docs;
  tests << is_hex('b')   = "0" >> docs;
  tests << is_hex('F')   = "1" >> docs;

  detail::is_hex_o = def{"o(atom)"} = [&](arg atom) { return impl::hex(atom, "IS"); };

  def<"\\0"> _0 = [&] { return def<"fail(...)">{[&](va) { return "0"; }}; };
  def<"\\1">{}  = [&] { return detail::is_hex_o; };

  return pp::call(xcat(utl::slice(_0, -1), is_atom(args)), args);
});

} // namespace api

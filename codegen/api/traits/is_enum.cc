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
decltype(is_enum_o) is_enum_o = NIFTY_DEF(is_enum_o);
}

decltype(is_enum) is_enum = NIFTY_DEF(is_enum, [&](va args) {
  docs << "[extends " + is_atom + "] detects if args matches a specified atom union."
       << ""
       << "to use this function, define a set of"
       << "macros with the following characteristics:"
       << " ‐ object-like"
       << " ‐ common prefix"
       << " ‐ enum value suffixes"
       << " ‐ expand to nothing"
       << "pass the common prefix as chkprefix."
       << ""
       << "example: (identifying an enum<GOOD|BAD>)"
       << " #define FOO_GOOD"
       << " #define FOO_BAD"
       << " " + is_enum + "(FOO_, BLEH) // 0"
       << " " + is_enum + "(FOO_, GOOD) // 1"
       << " " + is_enum + "(FOO_, ,,,)  // 0";

  detail::is_enum_o = def{"o(chkatom, vatom)"} = [&](arg chkatom, arg vatom) {
    return is_none(xcat(chkatom, vatom));
  };

  def<"\\0"> _0 = [&] { return def<"fail(...)">{[&](va) { return "0"; }}; };
  def<"\\1">{}  = [&] { return detail::is_enum_o; };

  return pp::call(xcat(utl::slice(_0, -1), is_atom(rest(args))), args);
});

} // namespace api

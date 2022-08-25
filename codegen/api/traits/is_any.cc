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
decltype(is_any_o) is_any_o = NIFTY_DEF(is_any_o);
}

decltype(is_any) is_any = NIFTY_DEF(is_any, [&](va args) {
  docs << "detects if the list has either 0 or 1 elements.";

  tests << is_any()         = "1" >> docs;
  tests << is_any("foo")    = "1" >> docs;
  tests << is_any("(a, b)") = "1" >> docs;
  tests << is_any("a, b")   = "0" >> docs;
  tests << is_any(", ")     = "0" >> docs;
  tests << is_any(", , ")   = "0" >> docs;
  tests << is_any("a, ")    = "0";
  tests << is_any("a, , ")  = "0";
  tests << is_any(", a")    = "0";
  tests << is_any(", a, ")  = "0";
  tests << is_any(", , a")  = "0";

  def<"\\0"> _0 = [&] { return "1"; };
  def<"\\01">{} = [&] { return "0"; };

  detail::is_any_o = def{"o(_, ...: <args + token; e.g. __VA_ARGS__.foo>)"} =
      [&](arg, va) { return pp::cat(_0, pp::va_opt(1)); };

  return detail::is_any_o(args + ".");
});

} // namespace api

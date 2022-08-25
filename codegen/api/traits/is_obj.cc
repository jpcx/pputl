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
decltype(is_obj_o) is_obj_o = NIFTY_DEF(is_obj_o);
}

decltype(is_obj) is_obj = NIFTY_DEF(is_obj, [&](va args) {
  docs << "[extends " + is_any + "] detects if args is a non-empty generic value.";

  tests << is_obj("")         = "0" >> docs;
  tests << is_obj(",")        = "0" >> docs;
  tests << is_obj("foo,")     = "0" >> docs;
  tests << is_obj("foo, bar") = "0" >> docs;
  tests << is_obj("foo")      = "1" >> docs;
  tests << is_obj("(42)")     = "1" >> docs;

  detail::is_obj_o = def{"o(any)"} = [&](arg any) {
    def<"\\0"> _0 = [&] { return "1"; };
    def<"\\1">{}  = [&] { return "0"; };
    return xcat(utl::slice(_0, -1), is_none(any));
  };

  def<"\\0"> _0 = [&] { return def<"fail(...)">{[&](va) { return "0"; }}; };
  def<"\\1">{}  = [&] { return detail::is_obj_o; };

  return pp::call(xcat(utl::slice(_0, -1), detail::is_any_o(args + ".")), args);
});

} // namespace api

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
decltype(is_bool_o) is_bool_o = NIFTY_DEF(is_bool_o);
}

decltype(is_bool) is_bool = NIFTY_DEF(is_bool, [&](va args) {
  docs << "[extends " + is_atom + "] detects if args is a bool (literal '1' or '0').";

  auto min = "0x" + utl::cat(std::vector<std::string>(conf::hex_length, "0"));

  tests << is_bool()         = "0" >> docs;
  tests << is_bool(0)        = "1" >> docs;
  tests << is_bool(1)        = "1" >> docs;
  tests << is_bool("1u")     = "0" >> docs;
  tests << is_bool(min)      = "0" >> docs;
  tests << is_bool(0, 1)     = "0" >> docs;
  tests << is_bool("(0)")    = "0" >> docs;
  tests << is_bool("()")     = "0";
  tests << is_bool("(), ()") = "0";
  tests << is_bool("0, ")    = "0";
  tests << is_bool(", ")     = "0";
  tests << is_bool(", , ")   = "0";
  tests << is_bool("a, ")    = "0";
  tests << is_bool("a, , ")  = "0";
  tests << is_bool(", a")    = "0";
  tests << is_bool(", a, ")  = "0";
  tests << is_bool(", , a")  = "0";

  detail::is_bool_o = def{"o(atom)"} = [&](arg atom) {
    def<"0"> _0 = [&] {
      return "";
    };

    def<"1">{} = [&] {
      return "";
    };

    return is_none(cat(utl::slice(_0, -1), atom));
  };

  def<"0"> _0 = [&] {
    return def<"fail(...)">{[&](va) {
      return "0";
    }};
  };

  def<"1">{} = [&] {
    return detail::is_bool_o;
  };

  return pp::call(cat(utl::slice(_0, -1), is_atom(args)), args);
});

} // namespace api

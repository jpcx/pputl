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
decltype(is_pair_o) is_pair_o = NIFTY_DEF(is_pair_o);
}

decltype(is_pair) is_pair = NIFTY_DEF(is_pair, [&](va args) {
  docs << "[extends is_tup] detects if args is a two-tuple.";

  tests << is_pair()          = "0" >> docs;
  tests << is_pair("1, 2")    = "0" >> docs;
  tests << is_pair("()")      = "0" >> docs;
  tests << is_pair("(1, 2)")  = "1" >> docs;
  tests << is_pair("(), ()")  = "0";
  tests << is_pair(", ")      = "0";
  tests << is_pair(", , ")    = "0";
  tests << is_pair("a, ")     = "0";
  tests << is_pair("a, , ")   = "0";
  tests << is_pair(", a")     = "0";
  tests << is_pair(", a, ")   = "0";
  tests << is_pair(", , a")   = "0";
  tests << is_pair("(, )")    = "1";
  tests << is_pair("(, , )")  = "0";
  tests << is_pair("(a, )")   = "1";
  tests << is_pair("(a, , )") = "0";
  tests << is_pair("(, a)")   = "1";
  tests << is_pair("(, a, )") = "0";
  tests << is_pair("(, , a)") = "0";

  detail::is_pair_o = def{"o(tup)"} = [&](arg tup) {
    return def<"<o(...)">{[&](va args) {
      return def<"<o(_, ...)">{[&](arg, va rest) {
        def<"\\0(...)"> _0 = [](va) {
          return "0";
        };
        def<"\\01(_, ...)">{} = [](arg, va rest) {
          return is_none(rest);
        };

        return pp::call(pp::cat(_0, pp::va_opt("1")), rest);
      }}(args + ".");
    }}(esc + " " + tup);
  };

  def<"fail(...)"> fail = [&](va) {
    return "0";
  };
  def<"\\0"> _0 = [&] {
    return fail;
  };
  def<"\\1">{} = [&] {
    return detail::is_pair_o;
  };

  return pp::call(xcat(utl::slice(_0, -1), is_tup(args)), args);
});

} // namespace api
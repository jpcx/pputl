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
decltype(is_arr_o)  is_arr_o  = NIFTY_DEF(is_arr_o);
decltype(is_arr_oo) is_arr_oo = NIFTY_DEF(is_arr_oo);
} // namespace detail

decltype(is_arr) is_arr = NIFTY_DEF(is_arr, [&](va args) {
  docs << "[extends " + is_obj + "] detects if args is a pputl array object."
       << "note: does not parse contained items during validity check.";

  tests << is_arr()                = "0" >> docs;
  tests << is_arr("1, 2")          = "0" >> docs;
  tests << is_arr(fwd::arr + "()") = "1" >> docs;

  def chk = def{"chk_\\" + fwd::arr + "(...)"} = [&](va) {
    return "";
  };

  detail::is_arr_o = def{"o(obj)"} = [&](arg obj) {
    detail::is_arr_oo = def{"<o(obj: <non tuple>)"} = [&](arg obj) {
      return is_none(
          pp::cat(utl::slice(chk, -(((std::string const&)fwd::arr).size())), obj));
    };

    def<"\\0"> _0 = [&] {
      return detail::is_arr_oo;
    };
    def<"\\1">{} = [&] {
      return def<"fail(...)">{[&](va) {
        return "0";
      }};
    };

    return pp::call(xcat(utl::slice(_0, -1), detail::is_tup_o(obj)), obj);
  };

  def<"\\0"> _0 = [&] {
    return def<"fail(...)">{[&](va) {
      return "0";
    }};
  };
  def<"\\1">{} = [&] {
    return detail::is_arr_o;
  };

  return pp::call(xcat(utl::slice(_0, -1), is_obj(args)), args);
});

} // namespace api
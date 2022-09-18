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

decltype(order) order = NIFTY_DEF(order, [&](va args) {
  docs << "[extends atom] a sorted sequence of words or enums."
       << "provide a prefix to use enums instead of words."
       << ""
       << "see [range] for available operations."
       << ""
       << "items are stored in either ascending or descending order."
       << "resultant atom is an expansion-terminated self-reference.";

  tests << xstr(order())        = pp::str(order("0u", "ASC", "", pp::tup())) >> docs;
  tests << xstr(order(order())) = pp::str(order("0u", "ASC", "", pp::tup())) >> docs;
  tests << xstr(order("ASC"))   = pp::str(order("0u", "ASC", "", pp::tup())) >> docs;
  tests << xstr(order("DESC"))  = pp::str(order("0u", "DESC", "", pp::tup())) >> docs;
  tests << xstr(order("ASC", "ENUM_FOO_")) =
      pp::str(order("0u", "ASC", "ENUM_FOO_", pp::tup())) >> docs;

  def<"dir_\\ASC"> dir_asc = [&] {
    return "";
  };
  def<"dir_\\DESC">{} = [&] {
    return "";
  };

  def<"\\0(_, ...)"> _0 = [&](arg first, va rest) {
    def<"\\0(dir, prefix)"> _0 = [&](arg dir, arg prefix) {
      return order("0u", enum_(utl::slice(dir_asc, -3), dir), prefix, pp::tup());
    };

    def<"\\1(first, ...)">{} = [&](arg first, va) {
      def<"\\0(dir)"> _0 = [&](arg dir) {
        return order("0u", enum_(utl::slice(dir_asc, -3), dir), "", pp::tup());
      };

      def<"\\1(order)">{} = [&](arg order) {
        return order;
      };

      return pp::call(xcat(utl::slice(_0, -1), detail::is_order_o(first)), first);
    };

    return pp::call(xcat(utl::slice(_0, -1), is_none(rest)), atom(first), rest);
  };

  def<"\\1(...)">{} = [&](va) {
    return order("0u", "ASC", "", pp::tup());
  };

  return pp::call(xcat(utl::slice(_0, -1), is_none(args)), args);
});

} // namespace api

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
decltype(is_enum_o)  is_enum_o  = NIFTY_DEF(is_enum_o);
decltype(is_enum_oo) is_enum_oo = NIFTY_DEF(is_enum_oo);
} // namespace detail

decltype(is_enum) is_enum = NIFTY_DEF(is_enum, [&](va args) {
  docs << "[extends is_atom] detects if args matches a defined enumeration."
       << ""
       << "to use enums, define two sets of object-like macros with a common prefix:"
       << " - the first set maps enum key suffixes to unique word values"
       << " - the second set maps enum value suffixes to the corresponding enum key"
       << ""
       << "note: any mapping macro may include additional data for other purposes;"
       << "      the library checks the first element only."
       << ""
       << "example: (identifying an enum<GOOD,BAD>)"
       << " #define FOO_GOOD 0"
       << " #define FOO_BAD  1"
       << " #define FOO_0 GOOD"
       << " #define FOO_1  BAD"
       << " " + is_enum + "(FOO_, BLEH) // 0"
       << " " + is_enum + "(FOO_, GOOD) // 1"
       << " " + is_enum + "(FOO_, ,,,)  // 0";

  def<"fail(...)"> fail = [&](va) {
    return "0";
  };

  detail::is_enum_o = def{"o(chk_atom, ...)"} = [&](arg chk_atom, va args) {
    detail::is_enum_oo = def{"<o(chk_atom, atom)"} = [&](arg chk_atom, arg atom) {
      /* return def<"o(...)">{[&](va args) { */
      /*   def<"\\0(_, ...)"> _0 = [&](arg first, va) { */
      /*     return ; */
      /*   }; */
      /*   def<"\\1(...)">{} = [&](va args) { */
      /*     return "0"; */
      /*   }; */
      /*   return pp::call(xcat(utl::slice(_0, -1), is_none(args)), args); */
      /* }}(xcat(chk_atom, atom)); */

      def<"\\0"> _0 = [&] {
        return "1";
      };
      def<"\\1">{} = [&] {
        return "0";
      };
      return xcat(utl::slice(_0, -1), is_object(xcat(chk_atom, atom)));
    };

    def<"\\0"> _0 = [&] {
      return fail;
    };
    def<"\\1">{} = [&] {
      return detail::is_enum_oo;
    };

    return pp::call(xcat(utl::slice(_0, -1), is_atom(args)), chk_atom, args);
  };

  def<"\\0(e, ...)"> _0 = [&](arg e, va) {
    return api::fail(e);
  };
  def<"\\1(e, chk_atom, ...)">{} = [&](arg, arg chk_atom, va args) {
    return detail::is_enum_o(chk_atom, args);
  };

  return pp::call(xcat(utl::slice(_0, -1), is_atom(first(args))),
                  error(is_enum, "chkprefix must be an atom", args), args);
});

} // namespace api

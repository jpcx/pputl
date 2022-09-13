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
decltype(is_array_o)  is_array_o  = NIFTY_DEF(is_array_o);
decltype(is_array_oo) is_array_oo = NIFTY_DEF(is_array_oo);
} // namespace detail

decltype(is_array) is_array = NIFTY_DEF(is_array, [&](va args) {
  docs << "[extends " + is_object + "] detects if args is a pputl array object."
       << "note: does not parse contained items during validity check.";

  tests << is_array()                   = "0" >> docs;
  tests << is_array("1, 2")             = "0" >> docs;
  tests << is_array("(a, b)")           = "0" >> docs;
  tests << is_array(fwd::arr + "()")    = "1" >> docs;
  tests << is_array(fwd::map + "()")    = "1" >> docs;
  tests << is_array(fwd::pqueue + "()") = "1" >> docs;
  tests << is_array(fwd::set + "()")    = "1" >> docs;
  tests << is_array(fwd::stack + "()")  = "1" >> docs;
  tests << is_array(fwd::queue + "()")  = "1" >> docs;

  def<"fail(...)"> fail{[&](va) {
    return "0";
  }};

  detail::is_array_o = def{"o(obj)"} = [&](arg obj) {
    detail::is_array_oo = def{"<o(atom)"} = [&](arg atom) {
      def arr = def{"\\" + fwd::arr + "(...)"} = [&](va) {
        return "";
      };
      def{"\\" + fwd::map + "(...)"} = [&](va) {
        return "";
      };
      def{"\\" + fwd::pqueue + "(...)"} = [&](va) {
        return "";
      };
      def{"\\" + fwd::set + "(...)"} = [&](va) {
        return "";
      };
      def{"\\" + fwd::stack + "(...)"} = [&](va) {
        return "";
      };
      def{"\\" + fwd::queue + "(...)"} = [&](va) {
        return "";
      };
      return is_none(
          pp::cat(utl::slice(arr, -(((std::string const&)fwd::arr).size())), atom));
    };

    def<"\\0"> _0 = [&] {
      return detail::is_array_oo;
    };
    def<"\\1">{} = [&] {
      return fail;
    };

    return pp::call(xcat(utl::slice(_0, -1), detail::is_tuple_o(obj)), obj);
  };

  def<"\\0"> _0 = [&] {
    return fail;
  };
  def<"\\1">{} = [&] {
    return detail::is_array_o;
  };

  return pp::call(xcat(utl::slice(_0, -1), is_object(args)), args);
});

} // namespace api

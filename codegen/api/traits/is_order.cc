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
decltype(is_order_o) is_order_o = NIFTY_DEF(is_order_o);
} // namespace detail

decltype(is_order) is_order = NIFTY_DEF(is_order, [&](va args) {
  docs << "[extends is_atom] detects if args is a pputl sorted sequence."
       << "note: does not parse contained items during validity check.";

  tests << is_order()                  = "0" >> docs;
  tests << is_order("1, 2")            = "0" >> docs;
  tests << is_order("(a, b)")          = "0" >> docs;
  tests << is_order(fwd::order + "()") = "1" >> docs;

  def chk = def{"chk_\\" + fwd::order + "(...)"} = [&](va) {
    return "";
  };

  def<"fail(...)"> fail{[&](va) {
    return "0";
  }};

  detail::is_order_o = def{"o(atom)"} = [&](arg atom) {
    return is_none(
        pp::cat(utl::slice(chk, -(((std::string const&)fwd::order).size())), atom));
  };

  def<"\\0"> _0 = [&] {
    return fail;
  };
  def<"\\1">{} = [&] {
    return detail::is_order_o;
  };

  return pp::call(xcat(utl::slice(_0, -1), is_atom(args)), args);
});

} // namespace api
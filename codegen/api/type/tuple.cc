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

decltype(tuple) tuple = NIFTY_DEF(tuple, [&](va args) {
  docs << "[extends object] a parenthesized list."
       << "constructibe from any range.";

  tests << tuple()                                  = "()" >> docs;
  tests << tuple(pp::tup())                         = "()" >> docs;
  tests << tuple(pp::tup(1, 2))                     = "(1, 2)" >> docs;
  tests << tuple(fwd::array + pp::tup("(a, b, c)")) = "(a, b, c)" >> docs;

  def<"\\0(e, atom)"> _0 = [](arg e, arg atom) {
    def<"\\0(e, ...)"> _0 = [](arg e, va) {
      return fail(e);
    };

    def<"\\1(e, range)">{} = [](arg, arg range) {
      return xrest(impl::sized_items(range));
    };

    return pp::call(xcat(utl::slice(_0, -1), detail::is_range_o(atom)), e, atom);
  };

  def<"\\1(e, tup)">{} = [](arg, arg tup) {
    return tup;
  };

  return def<"o(e, obj)">{[&](arg e, arg obj) {
    return pp::call(xcat(utl::slice(_0, -1), detail::is_tuple_o(obj)), e, obj);
  }}(error(tuple, "must be a parenthesized list", args), object(default_("()", args)));
});

} // namespace api

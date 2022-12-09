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

// #include "type.h"
// 
// namespace api {
// 
// using namespace codegen;
// 
// decltype(array) array = NIFTY_DEF(array, [&](va args) {
//   docs << "[extends atom] a sized sequence of items."
//        << "copies items from any range; see range.itemsof for details."
//        << ""
//        << "see [range] for available operations."
//        << ""
//        << "resultant atom is an expansion-terminated self-reference."
//        << ""
//        << "note: prefix is discarded when constructing"
//        << "      from map and set; only items are copied.";
// 
//   tests << xstr(array())          = pp::str(array("0u", pp::tup())) >> docs;
//   tests << xstr(array(pp::tup())) = pp::str(array("0u", pp::tup())) >> docs;
//   tests << xstr(array(pp::tup('a', 'b'))) =
//       pp::str(array("2u", pp::tup('a', 'b'))) >> docs;
//   tests << xstr(array(array(pp::tup("foo", "bar")))) =
//       pp::str(array("2u", pp::tup("foo", "bar"))) >> docs;
//   tests << xstr(array(fwd::set + pp::tup())) = pp::str(array("0u", pp::tup())) >> docs;
//   tests << xstr(array(fwd::map + pp::tup("ENUM_FOO_"))) =
//       pp::str(array("0u", pp::tup())) >> docs;
// 
//   def<"\\0(e, atom)"> _0 = [&](arg e, arg atom) {
//     def<"\\0(e, atom)"> _0 = [&](arg e, arg) {
//       return fail(e);
//     };
// 
//     def<"\\1(e, range)">{} = [&](arg, arg range) {
//       return array(impl::sized_items(range));
//     };
// 
//     return pp::call(xcat(utl::slice(_0, -1), detail::is_range_o(atom)), e, atom);
//   };
// 
//   def<"\\1(e, tuple)">{} = [&](arg, arg tuple) {
//     return array(countof + " " + tuple, tuple);
//   };
// 
//   return def<"o(e, object)">{[&](arg e, arg object) {
//     return pp::call(xcat(utl::slice(_0, -1), detail::is_tuple_o(object)), e, object);
//   }}(error(array, "invalid range", args), object(default_(pp::tup(), args)));
// });
// 
// } // namespace api

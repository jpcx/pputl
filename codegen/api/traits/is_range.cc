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

// #include "traits.h"
// 
// namespace api {
// 
// using namespace codegen;
// 
// namespace detail {
// decltype(is_range_o) is_range_o = NIFTY_DEF(is_range_o);
// }
// 
// decltype(is_range) is_range = NIFTY_DEF(is_range, [&](va args) {
//   docs << "[extends is_object; union is_tuple|is_array|is_map|is_set|is_stack|is_queue].";
// 
//   tests << is_range()                   = "0" >> docs;
//   tests << is_range("foo")              = "0" >> docs;
//   tests << is_range("(foo)")            = "1" >> docs;
//   tests << is_range(fwd::array + "()")  = "1" >> docs;
//   tests << is_range(fwd::order + "()")  = "1" >> docs;
//   tests << is_range(fwd::map + "()")    = "1" >> docs;
//   tests << is_range(fwd::set + "()")    = "1" >> docs;
//   tests << is_range(fwd::stack + "()")  = "1" >> docs;
//   tests << is_range(fwd::queue + "()")  = "1" >> docs;
//   tests << is_range(fwd::pqueue + "()") = "1" >> docs;
// 
//   def chk_array = def{"chk_\\" + fwd::array + "(...)"} = [&](va) {
//     return "";
//   };
//   def{"chk_\\" + fwd::order + "(...)"} = [&](va) {
//     return "";
//   };
//   def{"chk_\\" + fwd::map + "(...)"} = [&](va) {
//     return "";
//   };
//   def{"chk_\\" + fwd::set + "(...)"} = [&](va) {
//     return "";
//   };
//   def{"chk_\\" + fwd::stack + "(...)"} = [&](va) {
//     return "";
//   };
//   def{"chk_\\" + fwd::queue + "(...)"} = [&](va) {
//     return "";
//   };
//   def{"chk_\\" + fwd::pqueue + "(...)"} = [&](va) {
//     return "";
//   };
// 
//   detail::is_range_o = def{"o(object)"} = [&](arg object) {
//     def<"\\0(atom)"> _0 = [&](arg atom) {
//       return is_none(
//           xcat(utl::slice(chk_array, -((std::string const&)fwd::array).size()), atom));
//     };
// 
//     def<"\\1(tuple)">{} = [&](arg) {
//       return "1";
//     };
// 
//     return pp::call(xcat(utl::slice(_0, -1), detail::is_tuple_o(object)), object);
//   };
// 
//   def<"fail(...)"> fail{[&](va) {
//     return "0";
//   }};
//   def<"\\0">       _0 = [&] {
//     return fail;
//   };
//   def<"\\1">{} = [&] {
//     return detail::is_range_o;
//   };
// 
//   return pp::call(xcat(utl::slice(_0, -1), is_object(args)), args);
// });
// 
// } // namespace api

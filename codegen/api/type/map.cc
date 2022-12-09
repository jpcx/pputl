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
// decltype(map) map = NIFTY_DEF(map, [&](va args) {
//   docs << "[extends atom] a sorted map of words or enums to any."
//        << "provide a prefix to use enums as keys instead of words."
//        << ""
//        << "see [range] for available operations."
//        << ""
//        << "items are stored as (key, value) pairs sorted in ascending order by key."
//        << "resultant atom is an expansion-terminated self-reference.";
// 
//   tests << xstr(map())            = pp::str(map("0u", "", pp::tup())) >> docs;
//   tests << xstr(map("ENUM_FOO_")) = pp::str(map("0u", "ENUM_FOO_", pp::tup())) >> docs;
//   tests << xstr(map(map()))       = pp::str(map("0u", "", pp::tup())) >> docs;
// 
//   def<"\\0(...)"> _0 = [&](va args) {
//     def<"\\0(prefix)"> _0 = [&](arg prefix) {
//       return map("0u", prefix, pp::tup());
//     };
// 
//     def<"\\1(map)">{} = [&](arg map) {
//       return map;
//     };
// 
//     return pp::call(xcat(utl::slice(_0, -1), is_map(args)), atom(args));
//   };
// 
//   def<"\\1(...)">{} = [&](va) {
//     return map("0u", "", pp::tup());
//   };
// 
//   return pp::call(xcat(utl::slice(_0, -1), is_none(args)), args);
// });
// 
// } // namespace api

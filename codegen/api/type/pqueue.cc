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
// decltype(pqueue) pqueue = NIFTY_DEF(pqueue, [&](va args) {
//   docs << "[extends atom] a queue prioritized by words or enums."
//        << "provide a prefix to use enums as priorities instead of words."
//        << ""
//        << "see [range] for available operations."
//        << ""
//        << "items are stored as (priority, value) pairs in descending order by priority."
//        << "resultant atom is an expansion-terminated self-reference.";
// 
//   tests << xstr(pqueue()) = pp::str(pqueue("0u", "", pp::tup())) >> docs;
//   tests << xstr(pqueue("ENUM_FOO_")) =
//       pp::str(pqueue("0u", "ENUM_FOO_", pp::tup())) >> docs;
//   tests << xstr(pqueue(pqueue())) = pp::str(pqueue("0u", "", pp::tup())) >> docs;
// 
//   def<"\\0(...)"> _0 = [&](va args) {
//     def<"\\0(prefix)"> _0 = [&](arg prefix) {
//       return pqueue("0u", prefix, pp::tup());
//     };
// 
//     def<"\\1(pqueue)">{} = [&](arg pqueue) {
//       return pqueue;
//     };
// 
//     return pp::call(xcat(utl::slice(_0, -1), is_pqueue(args)), atom(args));
//   };
// 
//   def<"\\1(...)">{} = [&](va) {
//     return pqueue("0u", "", pp::tup());
//   };
// 
//   return pp::call(xcat(utl::slice(_0, -1), is_none(args)), args);
// });
// 
// } // namespace api

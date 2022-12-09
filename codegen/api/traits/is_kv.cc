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
// decltype(is_kv_o) is_kv_o = NIFTY_DEF(is_kv_o);
// }
// 
// decltype(is_kv) is_kv = NIFTY_DEF(is_kv, [&](va args) {
//   docs << "[extends is_pair] detects if args is a word-keyed value.";
// 
//   tests << is_kv()          = "0" >> docs;
//   tests << is_kv("1, 2")    = "0" >> docs;
//   tests << is_kv("()")      = "0" >> docs;
//   tests << is_kv("(1, 2)")  = "1" >> docs;
//   tests << is_kv("(1, b)")  = "1" >> docs;
//   tests << is_kv("(a, 2)")  = "0" >> docs;
//   tests << is_kv("(), ()")  = "0";
//   tests << is_kv(", ")      = "0";
//   tests << is_kv(", , ")    = "0";
//   tests << is_kv("a, ")     = "0";
//   tests << is_kv("a, , ")   = "0";
//   tests << is_kv(", a")     = "0";
//   tests << is_kv(", a, ")   = "0";
//   tests << is_kv(", , a")   = "0";
//   tests << is_kv("(, )")    = "1";
//   tests << is_kv("(, , )")  = "0";
//   tests << is_kv("(a, )")   = "0";
//   tests << is_kv("(9, )")   = "1";
//   tests << is_kv("(a, , )") = "0";
//   tests << is_kv("(, a)")   = "0";
//   tests << is_kv("(, a, )") = "0";
//   tests << is_kv("(, , a)") = "0";
// 
//   detail::is_kv_o = def{"o(pair)"} = [&](arg pair) {
//     return def<"o(...)">{[&](va args) {
//       def<"\\0(...)"> _0 = [&] {
//         return "0";
//       };
//       def<"\\1(sym)">{} = [&](arg sym) {
//         return impl::is_word(sym);
//       };
// 
//       return pp::call(xcat(utl::slice(_0, -1), is_none(args)), args);
//     }}(first + " " + pair);
//   };
// 
//   def<"fail(...)"> fail = [&](va) {
//     return "0";
//   };
//   def<"\\0"> _0 = [&] {
//     return fail;
//   };
//   def<"\\1">{} = [&] {
//     return detail::is_kv_o;
//   };
// 
//   return pp::call(xcat(utl::slice(_0, -1), is_pair(args)), args);
// });
// 
// } // namespace api

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
// namespace detail {
// decltype(nybl_pass) nybl_pass = NIFTY_DEF(nybl_pass);
// decltype(nybl_fail) nybl_fail = NIFTY_DEF(nybl_fail);
// decltype(nybl_o)    nybl_o    = NIFTY_DEF(nybl_o);
// } // namespace detail
// 
// decltype(nybl) nybl = NIFTY_DEF(nybl, [&](va args) {
//   docs << "nybl type (0 or 1)."
//        << "expands to b if valid, else fails.";
// 
//   tests << nybl(0) = "0" >> docs;
//   tests << nybl(1) = "1" >> docs;
// 
//   def<"chk_0"> chk_0 = [&] {
//     return "";
//   };
// 
//   def<"chk_1">{} = [&] {
//     docs << "concat existence checks";
//     return "";
//   };
// 
//   detail::nybl_fail = def{"fail(err, ...)"} = [&](arg err, va) {
//     return fail(err);
//   };
// 
//   detail::nybl_pass = def{"pass(err, ...)"} = [&](arg, va args) {
//     docs << "fourth parentheses; returns";
//     return args;
//   };
// 
//   def<>           ooo_no_pass{};
//   def<"ooo(...)"> ooo = [&](va args) {
//     docs << "third parentheses; asserts either 0 or 1.";
// 
//     ooo_no_pass = def{"no_pass(...)"} = [&](va) {
//       return detail::nybl_fail;
//     };
// 
//     def<"pass(...)"> ooo_pass = [&](va) {
//       return detail::nybl_pass;
//     };
// 
//     return def<"res(...)">{[&](va) {
//       std::string const prefix    = utl::slice(ooo_pass, -4);
//       std::string const pass_s    = utl::slice(ooo_pass, prefix.size(), 0);
//       std::string const no_pass_s = utl::slice(ooo_no_pass, prefix.size(), 0);
// 
//       return pp::call(pp::cat(
//           prefix,
//           pp::va_opt(utl::slice(no_pass_s, (no_pass_s.size() == 7 ? 3 : 2) - no_pass_s.size())),
//           pass_s));
//     }}(pp::cat(utl::slice(chk_0, -1), args));
//   };
// 
//   def<>             oo_fail{};
//   def<"oo(_, ...)"> oo = [&](arg _, va) {
//     docs << "second parentheses; asserts non-tuple.";
// 
//     oo_fail = def{"fail(...)"} = [&](va) {
//       return ooo_no_pass;
//     };
// 
//     def<"no_fail(...)"> oo_no_fail = [&](va) {
//       return ooo;
//     };
// 
//     return def<"res(...)">{[&](va) {
//       std::string const prefix    = utl::slice(oo_fail, -4);
//       std::string const fail_s    = utl::slice(oo_fail, prefix.size(), 0);
//       std::string const no_fail_s = utl::slice(oo_no_fail, prefix.size(), 0);
// 
//       return pp::call(pp::cat(
//           prefix,
//           pp::va_opt(utl::slice(no_fail_s, (no_fail_s.size() == 7 ? 3 : 2) - no_fail_s.size())),
//           fail_s));
//     }}(eat + " " + _);
//   };
// 
//   detail::nybl_o = def{"o(_, ...)"} = [&](arg, va) {
//     docs << "first parentheses; asserts only one arg.";
// 
//     def<"pass(...)"> pass = [&](va) {
//       return oo;
//     };
// 
//     def<"no_pass(...)"> no_pass = [&](va) {
//       return oo_fail;
//     };
// 
//     std::string const prefix    = utl::slice(pass, -4);
//     std::string const pass_s    = utl::slice(pass, prefix.size(), 0);
//     std::string const no_pass_s = utl::slice(no_pass, prefix.size(), 0);
// 
//     return pp::call(pp::cat(
//         prefix,
//         pp::va_opt(utl::slice(no_pass_s, (no_pass_s.size() == 7 ? 3 : 2) - no_pass_s.size())),
//         pass_s));
//   };
// 
//   return pp::call(pp::call(pp::call(detail::nybl_o(args + "."), args), args),
//                   istr("[" + nybl + "] invalid nybl : " + args), args);
// });
// 
// } // namespace api

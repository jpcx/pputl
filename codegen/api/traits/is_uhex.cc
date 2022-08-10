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
// decltype(is_uhex) is_uhex = NIFTY_DEF(is_uhex, [&](va args) {
//   docs << "detects if args is an unsigned int in hex form (requires 'u' suffix)."
//        << "hex length is fixed at " + hex_length + " (" + std::to_string(conf::hex_length) + ").";
// 
//   auto min = "0x" + utl::cat(std::vector<std::string>(conf::hex_length, "0"));
//   auto max = "0x" + utl::cat(std::vector<std::string>(conf::hex_length, "F"));
// 
//   tests << is_uhex("1")       = "0" >> docs;
//   tests << is_uhex("1u")      = "0" >> docs;
//   tests << is_uhex(min + "u") = "1" >> docs;
//   tests << is_uhex(max)       = "0" >> docs;
//   tests << is_uhex("(), ()")  = "0" >> docs;
// 
//   def uhex_ = def{(std::string const&)uhex} = [&] {
//     return "";
//   };
// 
//   def<"0(...)"> _0 = [&](va) {
//     return "0";
//   };
// 
//   def<"1(...)">{} = [&](va args) {
//     return is_none(cat(utl::slice(uhex_, -((std::string const&)uhex).size()), typeof(args)));
//   };
// 
//   return pp::call(cat(utl::slice(_0, -1), is_any(args)), args);
// });
// 
// } // namespace api

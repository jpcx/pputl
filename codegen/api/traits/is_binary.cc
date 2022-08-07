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
// decltype(is_binary) is_binary = NIFTY_DEF(is_binary, [&](va args) {
//   docs << "detects if args is a valid uint binary tuple."
//        << "requires all " + std::to_string(conf::uint_bits) + " bits to be present.";
// 
//   tests << is_binary()            = "0" >> docs;
//   tests << is_binary("()")        = "0" >> docs;
//   tests << is_binary("(1,0,1)")   = "0" >> docs;
//   tests << is_binary("(" + utl::cat(std::vector<std::string>(conf::uint_bits - 3, "0"), ",")
//                      + ",0,0,1)") = "1" >> docs;
//   tests << is_binary("(" + utl::cat(std::vector<std::string>(conf::uint_bits - 3, "0"), ",")
//                      + ",0,2,1)") = "0" >> docs;
// 
//   auto chksz = def{"chksz_" + std::to_string(conf::uint_bits)} = [&] {
//     return "";
//   };
// 
//   def<"oo0(...)"> oo0 = [&](va) {
//     return "0";
//   };
// 
//   def<"oo1(...)">{} = [&](va args) {
//     return def<"x(...)">{[&](va args) {
//       return def{"x(" + utl::cat(utl::alpha_base52_seq(conf::uint_bits), ", ") + ")",
//                  [&](pack args) {
//                    return is_uint(pp::cat(utl::slice(detail::uint_decimal[0], -conf::uint_bits),
//                                           utl::cat(args, "##")));
//                  }}(args);
//     }}(items(args));
//   };
// 
//   def<"o0(...)"> o0 = [&](va) {
//     return oo0;
//   };
// 
//   def<"o1(...)">{} = [&](va args) {
//     return cat(utl::slice(oo0, -1),
//                is_none(cat(utl::slice(chksz, -(std::to_string(conf::uint_bits)).size()),
//                            size(items(args)))));
//   };
// 
//   return pp::call(pp::call(cat(utl::slice(o0, -1), is_tuple(args)), args), args);
// });
// 
// } // namespace api

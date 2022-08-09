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

// #include <span>
// 
// #include "bitwise.h"
// 
// namespace api {
// 
// using namespace codegen;
// 
// decltype(bitshift_left_) bitshift_left_ = NIFTY_DEF(bitshift_left_, [&](va args) {
//   docs << "TODO";
// 
//   auto binmax       = "0b" + utl::cat(std::vector<std::string>(conf::bit_length, "1")) + "u";
//   auto binmaxminus1 = "0b" + utl::cat(std::vector<std::string>(conf::bit_length - 1, "1")) + "0u";
// 
//   tests << bitshift_left_(binmax, 1) = binmaxminus1 >> docs;
// 
//   auto params_v = utl::alpha_base52_seq(conf::bit_length);
//   auto params   = std::deque<std::string>{params_v.begin(), params_v.end()};
//   for (auto&& v : params) {
//     if (v == "b")
//       v = "B";
//     else if (v == "u")
//       v = "U";
//     else if (v == "B")
//       v = "_B";
//     else if (v == "U")
//       v = "_U";
//   }
//   auto params_s = utl::cat(params, ", ");
// 
//   def _0 = def{"0(" + params_s + ")"} = [&](pack args) {
//     return pp::cat("0b", pp::cat(args), "u");
//   };
// 
//   for (std::size_t i = 1; i < conf::bit_length; ++i) {
//     def{std::to_string(i) + "(" + params_s + ")"} = [&](pack args) {
//       std::vector<std::string> cpy     = args;
//       auto                     new_end = std::shift_left(cpy.begin(), cpy.end(), i);
//       return pp::cat("0b", pp::cat(std::span{cpy.begin(), new_end}),
//                      utl::cat(std::vector<std::string>(i, "0")) + "u");
//     };
//   }
// 
//   return def<"o(v, i)">{[&](arg v, arg i) {
//     return pp::call(typeof(v), def<"o(i, ...)">{[&](arg i, va args) {
//                       def<"0(i, ...)"> o0 = [&](arg, va) {
//                         return "0";
//                       };
// 
//                       def<"1(i, ...)">{} = [&](arg i, va args) {
//                         return pp::call(cat(utl::slice(_0, -1), i), args);
//                       };
// 
//                       return pp::call(cat(utl::slice(o0, -1), lt(i, conf::bit_length)), i, args);
//                     }}(ibase10(i), bits(v)));
//   }}(args);
// });
// 
// } // namespace api

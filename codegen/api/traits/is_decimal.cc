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
// decltype(is_decimal) is_decimal = NIFTY_DEF(is_decimal, [&](va args) {
//   docs << "detects if args is a uint represented as decimal.";
// 
//   auto binmin = "0b" + utl::cat(std::vector<std::string>(conf::bit_length, "0")) + "u";
//   auto binmax = "0b" + utl::cat(std::vector<std::string>(conf::bit_length, "1")) + "u";
// 
//   tests << is_decimal()               = "0" >> docs;
//   tests << is_decimal(48)             = "1" >> docs;
//   tests << is_decimal(conf::uint_max) = "1" >> docs;
//   tests << is_decimal("foo")          = "0" >> docs;
//   tests << is_decimal(binmin)         = "0" >> docs;
//   tests << is_decimal(binmax)         = "0" >> docs;
// 
//   def dec = def{std::string{decimal}} = [&] {
//     return "";
//   };
// 
//   def<"o_0(...)"> o_0 = [&](va) {
//     return "0";
//   };
// 
//   def<"o_1(u)">{} = [&](arg u) {
//     return is_none(cat(utl::slice(dec, -((std::string const&)decimal).size()), typeof(u)));
//   };
// 
//   return pp::call(cat(utl::slice(o_0, -1), is_uint(args)), args);
// });
// 
// } // namespace api

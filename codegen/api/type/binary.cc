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

#include "type.h"

// namespace api {
// 
// using namespace codegen;
// 
// decltype(binary) binary = NIFTY_DEF(binary, [&](va args) {
//   docs << "casts a uint to its binary subtype.";
// 
//   auto binmin = "0b" + utl::cat(std::vector<std::string>(conf::bit_length, "0")) + "u";
//   auto binone = "0b" + utl::cat(std::vector<std::string>(conf::bit_length - 1, "0")) + "1u";
//   auto binmax = "0b" + utl::cat(std::vector<std::string>(conf::bit_length, "1")) + "u";
// 
//   tests << binary(0)          = binmin >> docs;
//   tests << binary(1)          = binone >> docs;
//   tests << binary(uint_max_s) = binmax >> docs;
//   tests << binary(binmin)     = binmin >> docs;
//   tests << binary(binone)     = binone >> docs;
//   tests << binary(binmax)     = binmax >> docs;
// 
//   def<"\\DEC(n, bin, ...)"> dec = [&](arg, arg bin, va) {
//     return bin;
//   };
// 
//   def<"\\BIN(n, dec, ...)">{} = [&](arg n, arg, va) {
//     return n;
//   };
// 
//   return def<"o(n)">{[&](arg n) {
//     return def<"o(...)">{[&](va args) {
//       return def<"x(n, t, ...)">{[&](arg n, arg t, va args) {
//         return pp::call(cat(utl::slice(dec, -3), t), n, args);
//       }}(args);
//     }}(n, cat(utl::slice(detail::uint_traits[0], -1), n));
//   }}(uint(args));
// });
// 
// } // namespace api

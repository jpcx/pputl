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
// decltype(bitflip_) bitflip_ = NIFTY_DEF(bitflip_, [&](va args) {
//   docs << "flips the ith bit in the uint."
//        << "i must be less than " + bit_length + " (" + std::to_string(conf::bit_length) + ").";
// 
//   auto binmax       = "0b" + utl::cat(std::vector<std::string>(conf::bit_length, "1")) + "u";
//   auto binmaxminus1 = "0b" + utl::cat(std::vector<std::string>(conf::bit_length - 1, "1")) + "0u";
// 
//   tests << bitflip_(0, conf::bit_length - 1)            = "1" >> docs;
//   tests << bitflip_(0, conf::bit_length - 3)            = "4" >> docs;
//   tests << bitflip_(binmaxminus1, conf::bit_length - 1) = binmax >> docs;
// 
//   return def<"x(v, i)">{[&](arg v, arg i) {
//     return bitset_(v, i, not_(bitget_(v, i)));
//   }}(args);
// });
// 
// } // namespace api

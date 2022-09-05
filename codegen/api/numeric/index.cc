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

// #include "numeric.h"
// 
// namespace api {
// 
// using namespace codegen;
// 
// decltype(index) index = NIFTY_DEF(index, [&](va args) {
//   docs << "translates an idx to a positive zero-offset index for a given range size."
//        << "fails if out of bounds.";
// 
//   tests << index(0, 5)      = "0" >> docs;
//   tests << index(1, 5)      = "1" >> docs;
//   tests << index(3, 5)      = "3" >> docs;
//   tests << index(4, 5)      = "4" >> docs;
//   tests << index(neg(1), 5) = "4" >> docs;
//   tests << index(neg(2), 5) = "3" >> docs;
//   tests << index(neg(4), 5) = "1" >> docs;
//   tests << index(neg(5), 5) = "0" >> docs;
// 
//   def<"\\0(idx, range)"> _0 = [&](arg idx, arg range) {
//     //
//     return "";
//   };
//   def<"\\1(idx, range)">{} = [&](arg idx, arg range) {
//     //
//     return "";
//   };
// 
//   return pp::call(xcat(utl::slice(_0, -1), ltz(first(args))), args);
// });
// 
// } // namespace api

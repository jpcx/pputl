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

// #include "math.h"
// 
// namespace api {
// 
// using namespace codegen;
// 
// decltype(add) add = NIFTY_DEF(add, [&](va args) {
//   docs << "uint addition with overflow.";
// 
//   tests << add("0, 0")            = "0" >> docs;
//   tests << add("0, 1")            = "1" >> docs;
//   tests << add("1, 2")            = "3" >> docs;
//   tests << add(conf::uint_max, 1) = "0" >> docs;
//   tests << add(conf::uint_max, 2) = "1" >> docs;
//   tests << add(1, conf::uint_max) = "0";
//   tests << add(2, conf::uint_max) = "1";
// 
//   def<"x(...)"> x = [&](va args) {
//     return args;
//   };
// 
//   return meta_recur(x, first(args), inc, rest(args));
// });
// 
// } // namespace api

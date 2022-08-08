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

// #include "compare.h"
// 
// namespace api {
// 
// using namespace codegen;
// 
// decltype(gt) gt = NIFTY_DEF(gt, [&](va args) {
//   docs << "uint greater-than comparison.";
// 
//   tests << gt("0, 0")                                 = "0" >> docs;
//   tests << gt("0, 1")                                 = "0" >> docs;
//   tests << gt("0, 2")                                 = "0";
//   tests << gt("0, 3")                                 = "0";
//   tests << gt("1, 0")                                 = "1" >> docs;
//   tests << gt("1, 1")                                 = "0" >> docs;
//   tests << gt("1, 2")                                 = "0";
//   tests << gt("1, 3")                                 = "0";
//   tests << gt("2, 0")                                 = "1";
//   tests << gt("2, 1")                                 = "1";
//   tests << gt("2, 2")                                 = "0";
//   tests << gt("2, 3")                                 = "0";
//   tests << gt("3, 0")                                 = "1";
//   tests << gt("3, 1")                                 = "1";
//   tests << gt("3, 2")                                 = "1";
//   tests << gt("3, 3")                                 = "0";
//   tests << gt(0, conf::uint_max)                      = "0";
//   tests << gt(0, conf::uint_max - 1)                  = "0";
//   tests << gt(1, conf::uint_max)                      = "0";
//   tests << gt(1, conf::uint_max - 1)                  = "0";
//   tests << gt(conf::uint_max, 0)                      = "1";
//   tests << gt(conf::uint_max - 1, 0)                  = "1";
//   tests << gt(conf::uint_max, 1)                      = "1";
//   tests << gt(conf::uint_max - 1, 1)                  = "1";
//   tests << gt(conf::uint_max, conf::uint_max)         = "0";
//   tests << gt(conf::uint_max, conf::uint_max - 1)     = "1";
//   tests << gt(conf::uint_max - 1, conf::uint_max)     = "0";
//   tests << gt(conf::uint_max - 1, conf::uint_max - 1) = "0";
// 
//   return def<"x(l, r)">{[&](arg l, arg r) {
//     return lt(r, l);
//   }}(args);
// });
// 
// } // namespace api

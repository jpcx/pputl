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
// decltype(ne) ne = NIFTY_DEF(ne, [&](va args) {
//   docs << "uint not-equal-to comparison.";
// 
//   tests << eq("0, 0")                                 = "1" >> docs;
//   tests << eq("0, 1")                                 = "0" >> docs;
//   tests << eq("0, 2")                                 = "0";
//   tests << eq("0, 3")                                 = "0";
//   tests << eq("1, 0")                                 = "0" >> docs;
//   tests << eq("1, 1")                                 = "1" >> docs;
//   tests << eq("1, 2")                                 = "0";
//   tests << eq("1, 3")                                 = "0";
//   tests << eq("2, 0")                                 = "0";
//   tests << eq("2, 1")                                 = "0";
//   tests << eq("2, 2")                                 = "1";
//   tests << eq("2, 3")                                 = "0";
//   tests << eq("3, 0")                                 = "0";
//   tests << eq("3, 1")                                 = "0";
//   tests << eq("3, 2")                                 = "0";
//   tests << eq("3, 3")                                 = "1";
//   tests << eq(0, conf::uint_max)                      = "0";
//   tests << eq(0, conf::uint_max - 1)                  = "0";
//   tests << eq(1, conf::uint_max)                      = "0";
//   tests << eq(1, conf::uint_max - 1)                  = "0";
//   tests << eq(conf::uint_max, 0)                      = "0";
//   tests << eq(conf::uint_max, 0)                      = "0";
//   tests << eq(conf::uint_max - 1, 0)                  = "0";
//   tests << eq(conf::uint_max - 1, 0)                  = "0";
//   tests << eq(conf::uint_max, 1)                      = "0";
//   tests << eq(conf::uint_max, 1)                      = "0";
//   tests << eq(conf::uint_max - 1, 1)                  = "0";
//   tests << eq(conf::uint_max - 1, 1)                  = "0";
//   tests << eq(conf::uint_max, conf::uint_max)         = "1";
//   tests << eq(conf::uint_max, conf::uint_max - 1)     = "0";
//   tests << eq(conf::uint_max - 1, conf::uint_max)     = "0";
//   tests << eq(conf::uint_max - 1, conf::uint_max - 1) = "1";
// 
//   return not_(eq(args));
// });
// 
// } // namespace api

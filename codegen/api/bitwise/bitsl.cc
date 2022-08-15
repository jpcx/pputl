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
// decltype(bitsl) bitsl = NIFTY_DEF(bitsl, [&](va args) {
//   docs << "TODO";
// 
//   def rev = def{"rev" + pp::tup(utl::alpha_base52_seq(conf::word_size))} = [&](pack args) {
//     svect rev{args};
//     std::ranges::reverse(rev);
//     return utl::cat(rev, ", ");
//   };
// 
//   def<"r(...)"> r = [&](va args) {
//     return def<"o(n, ccur, slcur, ...)">{[&](arg n, arg ccur, arg slcur, va rest) {
//       // TODO select i%4, dec by at most 4 each iteration
//       def<"0(...)"> _0 = [&](va args) { return ""; };
//       def<"1(...)">{}  = [&](va args) { return ""; };
//       return pp::call(pp::cat(utl::slice(_0, -1), cprev));
//       return impl::hex(first, "SL");
//     }}(args);
//   };
// 
//   return word(pp::tup(rev(def<"o(...)">{[&](va args) {
//                 auto open  = utl::cat(svect(conf::word_size, r + "("));
//                 auto close = utl::cat(svect(conf::word_size, ")"));
//                 return open + " " + args + " " + close;
//               }}(rev + " " + xword(args)))),
//               typeof(args));
// });
// 
// } // namespace api

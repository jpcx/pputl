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
// // TODO: optimization
// 
// decltype(fulldiv) fulldiv = NIFTY_DEF(fulldiv, [&](va args) {
//   docs << "uint division that returns both the quotient and remainder."
//        << "throws an error on division by zero.";
// 
//   tests << fulldiv("6, 3") = "2, 0" >> docs;
//   tests << fulldiv("7, 4") = "1, 3" >> docs;
//   /* tests << fulldiv(conf::uint_max, 7) = */
//   /*     (std::to_string(conf::uint_max / 7) + ", " + std::to_string(conf::uint_max % 7)) >> docs;
//    */
// 
//   def<"res(...)"> res = [&](va args) {
//     return def<"x(q, r, b)">{[&](arg q, arg r, arg) {
//       return q + ", " + r;
//     }}(args);
//   };
// 
//   def<"recur(...)"> recur = [&](va args) {
//     return def<"x(q, r, b)">{[&](arg q, arg r, arg b) {
//       return if_(rest(detail::sub_impl(r, b)), pp::tup(q, r, b), pp::tup(inc(q), sub(r, b), b));
//     }}(args);
//   };
// 
//   def<"x(...)"> x = [&](va args) {
//     return args;
//   };
// 
//   def<"zero_b(err, a, b)"> zero_b = [&](arg err, arg, arg) {
//     return fail(err);
//   };
// 
//   def<"pos_b(err, a, b)"> pos_b = [&](arg, arg a, arg b) {
//     return res(meta_recur(x, a, recur, "0", a, b));
//   };
// 
//   return pp::call(if_(eqz(rest(args)), pp::tup(zero_b), pp::tup(pos_b)),
//                   istr("[" + fulldiv + "] division by zero error : " + args), args);
// });
// 
// } // namespace api

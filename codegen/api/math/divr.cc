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
// decltype(divr) divr = NIFTY_DEF(divr, [&](va args) {
//   docs << "division with remainder."
//        << "fails on division by zero."
//        << "" << impl::arith_rules;
// 
//   tests << divr("6, 3") = "2, 0" >> docs;
//   tests << divr("7, 4") = "1, 3" >> docs;
//   tests << divr(conf::uint_max, 7) =
//       (std::to_string(conf::uint_max / 7) + ", " + std::to_string(conf::uint_max % 7))
//       >> docs;
// 
//   def<"x(...)"> x = [&](va args) { return args; };
// 
//   def<"res(...)"> res = [&](va args) {
//     return def<"o(a, b, s)">{[&](arg a, arg, arg s) { return add(a, s); }}(args);
//   };
// 
//   def<"r(...)"> r = [&](va args) {
//     def<"base(s, a)"> base = [&](arg s, arg) { return s; };
//     def<"recr(s, a)"> recr = [&](arg s, arg a) { return add(s, a); };
// 
//     return def<"o(i, r, q, a, b)">{[&](arg i, arg r, arg q, arg a, arg b) {
//       return dec(i) + ", " + bset(bsll(r), 0, bget(a, i)) + ", "
//            + pp::call(if_(bget(b, conf::bit_length - 1), recr, base), r, q, b);
//     }}(args);
//   };
// 
//   def<"beqz(e, a, b)"> beqz = [&](arg e, arg, arg) { return fail(e); };
//   def<"bnez(a, b)">    bnez = [&](arg a, arg b) {
//     def<"\\00(ta, tb, a, b)"> _00 = [&](arg ta, arg tb, arg a, arg b) {
//       return res(recur(x, std::to_string(conf::bit_length), r, a, b, "0"),
//                     impl::arithhint(ta, tb), 0);
//     };
//     def<"\\01(ta, tb, a, b)">{} = [&](arg ta, arg tb, arg a, arg b) {
//       return res(recur(x, std::to_string(conf::bit_length), r, a, neg(b), "0"),
//                     impl::arithhint(ta, tb), 1);
//     };
//     def<"\\10(ta, tb, a, b)">{} = [&](arg ta, arg tb, arg a, arg b) {
//       return res(recur(x, std::to_string(conf::bit_length), r, neg(a), b, "0"),
//                     impl::arithhint(ta, tb), 1);
//     };
//     def<"\\11(ta, tb, a, b)">{} = [&](arg ta, arg tb, arg a, arg b) {
//       return res(recur(x, std::to_string(conf::bit_length), r, neg(a), neg(b), "0"),
//                     impl::arithhint(ta, tb), 0);
//     };
// 
//     return pp::call(xcat(utl::slice(_00, -2), xcat(ltz(a), ltz(b))), typeof(a), typeof(b),
//                        a, b);
//   };
// 
//   return pp::call(if_(eqz(rest(args)), beqz, bnez),
//                   str(pp::str("[" + divr + "] division by zero") + " : " + args), args);
// });
// 
// } // namespace api

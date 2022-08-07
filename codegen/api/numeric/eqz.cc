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
// namespace detail {
// decltype(eqz_0) eqz_0 = NIFTY_DEF(eqz_0);
// }
// 
// decltype(eqz) eqz = NIFTY_DEF(eqz, [&](va args) {
//   docs << "uint zero detection.";
// 
//   tests << eqz("0")             = "1" >> docs;
//   tests << eqz("1")             = "0";
//   tests << eqz("2")             = "0";
//   tests << eqz(uint_max_s)      = "0";
//   tests << eqz(conf::uint_max)  = "0" >> docs;
// 
//   detail::eqz_0 = def{"0"} = [&] {
//     return "";
//   };
// 
//   return is_none(cat(utl::slice(detail::eqz_0, -1), uint(args)));
// });
// 
// } // namespace api

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

#include "algo.h"

namespace api {

using namespace codegen;

/*
#define PPUTLPASTE_B(expr, ...)             PPUTLPASTE_B##__VA_OPT__(_NOT)##_DONE(expr,
__VA_ARGS__) #define PPUTLPASTE_B_DONE(expr, ...)        expr #define
PPUTLPASTE_B_NOT_DONE(expr, _, ...) PPUTLPASTE_A PTL_LP expr _, __VA_ARGS__ PTL_RP #define
PPUTLPASTE_A(expr, ...)             PPUTLPASTE_A##__VA_OPT__(_NOT)##_DONE(expr,
__VA_ARGS__) #define PPUTLPASTE_A_DONE(expr, ...)        expr #define
PPUTLPASTE_A_NOT_DONE(expr, _, ...) PPUTLPASTE_B PTL_LP expr _, __VA_ARGS__ PTL_RP

better to design a symmetric algorithm to allow for PASTE(LP, (), RP, ())

PASTE(...) = PASTE(args/2) PASTE(args/2)

> PASTE(LP, (), RP, ()) = PASTE(LP, ()) PASTE(RP, ())
>                       = PASTE(LP) PASTE(()) PASTE(RP) PASTE(())
>                       = LP () RP ()
>                       = ( )

*/

// decltype(paste) paste = NIFTY_DEF(paste, [&](va args) {
//   docs << "pastes an expression by separating arguments with spaces.";
//
//   auto max                      = utl::alpha_base52_seq(conf::uint_max);
//   tests << paste()              = "" >> docs;
//   tests << paste('a')           = "a" >> docs;
//   tests << paste('a', 'b')      = "a b" >> docs;
//   tests << paste('a', 'b', 'c') = "a b c" >> docs;
//   tests << paste(max)           = utl::cat(max, " ");
//
//   def<"a(_, ...)"> a;
//   def<"b(_, ...)"> b;
//
//   a = [&](arg _0, va args) {
//     return _0 + " " + pp::va_opt(b + " " + lp() + " " + args + " " + rp());
//   };
//   b = [&](arg _0, va args) {
//     return _0 + " " + pp::va_opt(a + " " + lp() + " " + args + " " + rp());
//   };
//
//   return pp::va_opt(items(pp::tup(pp::call(x(size(args)), a(args)))));
// });

} // namespace api

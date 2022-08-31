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

#include "codegen.h"
#include "numeric.h"

namespace api {

using namespace codegen;

decltype(abs) abs = NIFTY_DEF(abs, [&](va args) {
  docs << "numeric absolute value."
       << "casts result according to the input.";

  tests << abs(0)       = "0" >> docs;
  tests << abs(1)       = "1" >> docs;
  tests << abs(neg(1))  = ("0x" + utl::cat(samp::h1)) >> docs;
  tests << abs(neg(15)) = ("0x" + utl::cat(samp::h15)) >> docs;

  def<"\\0(n)"> _0 = [&](arg n) { return n; };
  def<"\\1(n)">{}  = [&](arg n) { return neg(n); };

  return pp::call(xcat(utl::slice(_0, -1), ltz(args)), args);
});

} // namespace api

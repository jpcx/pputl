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

#include "numeric.h"

namespace api {

using namespace codegen;

decltype(ltz) ltz = NIFTY_DEF(ltz, [&](va args) {
  docs << "numeric less-than-zero detection.";

  tests << ltz("0")            = "0" >> docs;
  tests << ltz("1")            = "0" >> docs;
  tests << ltz("0u")           = "0" >> docs;
  tests << ltz("1u")           = "0" >> docs;
  tests << ltz(int_max_s)      = "0" >> docs;
  tests << ltz(int_min_s)      = "1" >> docs;
  tests << ltz(inc(int_max_s)) = "1" >> docs;

  def<"0(n)"> _0 = [&](arg) {
    return "0";
  };
  def<"1(n)">{} = [&](arg n) {
    return impl::ltz(n);
  };

  return pp::call(xcat(utl::slice(_0, -1), is_int(args)), uhex(args));
});

} // namespace api

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

decltype(prime) prime = NIFTY_DEF(prime, [&](va args) {
  docs << "numeric primality test. value must be non-negative.";

  tests << prime(0)                                        = "1" >> docs;
  tests << prime(1)                                        = "1" >> docs;
  tests << prime(2)                                        = "1" >> docs;
  tests << prime(3)                                        = "1" >> docs;
  tests << prime(4)                                        = "0" >> docs;
  tests << prime(13)                                       = "1" >> docs;
  tests << prime(conf::int_max / 2)                        = "0" >> docs;
  tests << prime(std::to_string(conf::uint_max / 2) + "u") = "0" >> docs;

  def<"\\0(e, n)"> _0 = [&](arg, arg n) { return is_none(impl::udec(udec(n), "FACT")); };
  def<"\\1(e, n)">{}  = [&](arg e, arg) { return fail(e); };

  return pp::call(xcat(utl::slice(_0, -1), ltz(args)),
                  str("[" + prime + "] value must be non-negative : " + args), args);
});

} // namespace api

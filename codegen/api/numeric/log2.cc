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

decltype(log2) log2 = NIFTY_DEF(log2, [&](va args) {
  docs << "numeric log2. value must be greater than zero."
       << "casts result according to the input.";

  tests << log2("1u")                      = "0u" >> docs;
  tests << log2(2)                         = "1" >> docs;
  tests << log2("0x" + utl::cat(samp::h4)) = ("0x" + utl::cat(samp::h2)) >> docs;
  tests << log2(uint_max_s) = (std::to_string(conf::bit_length - 1) + "u") >> docs;

  def<"\\0(e, n)"> _0 = [&](arg e, arg) { return fail(e); };
  def<"\\1(e, n)">{}  = [&](arg, arg n) {
    return word(impl::udec(udec(n), "LOG2"), typeof(n));
  };

  return pp::call(xcat(utl::slice(_0, -1), gtz(args)),
                  str("[" + log2 + "] value must be greater than zero : " + args), args);
});

} // namespace api

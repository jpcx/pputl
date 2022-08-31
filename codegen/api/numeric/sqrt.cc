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

decltype(sqrt) sqrt = NIFTY_DEF(sqrt, [&](va args) {
  docs << "numeric sqrt. value must be non-negative."
       << "casts result according to the input.";

  tests << sqrt("0u") = "0u" >> docs;
  tests << sqrt(4)    = "2" >> docs;
  if constexpr (conf::word_size > 1)
    tests << sqrt("0x" + utl::cat(samp::h16)) = ("0x" + utl::cat(samp::h4)) >> docs;
  tests << sqrt(uint_max_s) =
      (std::to_string((unsigned)::sqrt(conf::uint_max)) + "u") >> docs;

  def<"\\0(e, n)"> _0 = [&](arg, arg n) {
    return word(impl::udec(udec(n), "SQRT"), typeof(n));
  };
  def<"\\1(e, n)">{} = [&](arg e, arg) { return fail(e); };

  return pp::call(xcat(utl::slice(_0, -1), ltz(args)),
                  str("[" + sqrt + "] value must be non-negative : " + args), args);
});

} // namespace api

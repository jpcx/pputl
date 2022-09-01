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

namespace detail {
decltype(eqz_0u) eqz_0u = NIFTY_DEF(eqz_0u);
}

decltype(eqz) eqz = NIFTY_DEF(eqz, [&](va args) {
  docs << "numeric zero detection.";

  tests << eqz("0")                               = "1" >> docs;
  tests << eqz("0u")                              = "1" >> docs;
  tests << eqz("0x" + utl::cat(samp::hmin))       = "1" >> docs;
  tests << eqz("0x" + utl::cat(samp::hmin) + "u") = "1" >> docs;
  tests << eqz(pp::tup(samp::hmin))               = "1" >> docs;
  tests << eqz("1u")                              = "0" >> docs;
  tests << eqz("2")                               = "0" >> docs;
  tests << eqz(uint_max_s)                        = "0" >> docs;
  tests << eqz(int_min_s)                         = "0" >> docs;

  detail::eqz_0u = def{"\\0u"} = [&] { return ""; };

  return is_none(xcat(utl::slice(detail::eqz_0u, -2), udec(args)));
});

} // namespace api

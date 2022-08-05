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

decltype(div2) div2 = NIFTY_DEF(div2, [&](va args) {
  docs << "O(1) uint division by 2.";

  tests << div2(0)              = "0" >> docs;
  tests << div2(1)              = "0";
  tests << div2(2)              = "1";
  tests << div2(7)              = "3";
  tests << div2(conf::uint_max) = std::to_string(conf::uint_max / 2) >> docs;

  return def<"x(...)">{[&](va args) {
    return def<"x(d, i, dv, ml, sq, p, m2, m4, m8, m16, m32, m64, ...)">{[&](pack args) {
      return args[2];
    }}(args);
  }}(cat(utl::slice(detail::uint_traits[0], -1), uint(args)));
});

} // namespace api

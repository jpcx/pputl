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
  docs << "O(1) positive uint logarithm base 2.";

  tests << log2(1) = "0" >> docs;
  tests << log2(conf::uint_max) =
      std::to_string(((unsigned)std::log2(conf::uint_max)) % conf::uint_max) >> docs;

  def<"nez1(...)">{} = [&](va args) {
    return def<"x(de, in, lg, dv, ml, sq, pw, m2, m4, m8, m16, m32, m64, ...)">{[&](pack args) {
      return args[2];
    }}(args);
  };

  def<"nez0(...)"> nez0 = [&](va) {
    return fail(pp::str("[" + log2 + "] log2 of zero not supported"));
  };

  return pp::call(cat(utl::slice(nez0, -1), nez(args)),
                  cat(utl::slice(detail::uint_traits[0], -1), uint(args)));
});

} // namespace api

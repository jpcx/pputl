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

decltype(factor) factor = NIFTY_DEF(factor, [&](va args) {
  docs << "O(1) uint prime factorization.";

  auto                     facts0 = utl::prime_factors(conf::uint_max / 3);
  auto                     facts1 = utl::prime_factors(conf::uint_max);
  std::vector<std::string> sfacts0(facts0.size());
  std::vector<std::string> sfacts1(facts1.size());
  std::ranges::transform(facts0, std::begin(sfacts0), [](auto&& v) {
    return std::to_string(v);
  });
  std::ranges::transform(facts1, std::begin(sfacts1), [](auto&& v) {
    return std::to_string(v);
  });

  tests << factor(conf::uint_max / 3) = utl::cat(sfacts0, ", ") >> docs;
  tests << factor(conf::uint_max)     = utl::cat(sfacts1, ", ") >> docs;

  return def<"x(...)">{[&](va args) {
    return def<"x(de, in, lg, dv, ml, sq, pw, m2, m4, m8, m16, m32, m64, ...)">{[&](pack args) {
      return args.back();
    }}(args);
  }}(cat(utl::slice(detail::uint_traits[0], -1), uint(args)));
});

} // namespace api

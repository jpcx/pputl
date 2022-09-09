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

#include "impl/traits.h"
#include "math.h"

namespace api {

using namespace codegen;

decltype(add) add = NIFTY_DEF(add, [&](va args) {
  docs << "addition with overflow."
       << "" << impl::arith_rules;

  tests << add("0, 0")        = "0" >> docs;
  tests << add("0, 1")        = "1" >> docs;
  tests << add("1, 2")        = "3" >> docs;
  tests << add("3u, 4")       = "7u" >> docs;
  tests << add("5, 6u")       = "11u" >> docs;
  tests << add(uint_max_s, 1) = "0u" >> docs;
  tests << add(uint_max_s, 2) = "1u" >> docs;
  tests << add(1, uint_max_s) = "0u";
  tests << add(2, uint_max_s) = "1u";
  tests << add(uint_max_s, uint_max_s) =
      (std::to_string(conf::uint_max - 1) + "u") >> docs;
  tests << add(int_max_s, 1) = int_min_s >> docs;
  tests << add(int_max_s, pp::tup(samp::h1)) =
      (std::to_string(conf::int_max + 1) + "u") >> docs;

  return def<"o(a, b)">{[&](arg a, arg b) {
    return word(impl::add(utup(a), utup(b)),
                impl::xarithhint(typeof(word(a)), typeof(word(b))));
  }}(args);
});

} // namespace api

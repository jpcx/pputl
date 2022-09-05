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

#include "math.h"

namespace api {

using namespace codegen;

decltype(sub) sub = NIFTY_DEF(sub, [&](va args) {
  docs << "subtraction with underflow."
       << "" << impl::arith_rules;

  tests << sub("0, 0")          = "0" >> docs;
  tests << sub("0, 1")          = ("0x" + utl::cat(samp::hmax)) >> docs;
  tests << sub("0u, 1u")        = uint_max_s >> docs;
  tests << sub("1, 0")          = "1" >> docs;
  tests << sub("1, 1")          = "0" >> docs;
  tests << sub("3, 1")          = "2" >> docs;
  tests << sub("1u, 3u")        = (std::to_string(conf::uint_max - 1) + "u") >> docs;
  tests << sub(uint_max_s, 0)   = uint_max_s;
  tests << sub(uint_max_s, 1)   = (std::to_string(conf::uint_max - 1) + "u");
  tests << sub(0, int_min_s)    = int_min_s >> docs;
  tests << sub("0u", int_min_s) = (std::to_string(conf::int_max + 1) + "u") >> docs;

  return def<"o(a, b)">{[&](arg a, arg b) {
    return word(impl::sub(utup(a), utup(b)),
                impl::xarithhint(typeof(word(a)), typeof(word(b))));
  }}(args);
});

} // namespace api

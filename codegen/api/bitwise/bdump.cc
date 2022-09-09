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
//         This program is free software: you can redistribute it and/or modify
//         it under the terms of the GNU General Public License as published by
//         the Free Software Foundation, either version 3 of the License, or
//         (at your option) any later version.
//
//         This program is distributed in the hope that it will be useful,
//         but WITHOUT ANY WARRANTY; without even the implied warranty of
//         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//         GNU General Public License for more details.
//                                                                            //
//  You should have received a copy of the GNU General Public License        ///
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.  ////
///////////////////////////////////////////////////////////////////////////// */

#include "bitwise.h"

namespace api {

using namespace codegen;

decltype(bdump) bdump = NIFTY_DEF(bdump, [&](va args) {
  docs << "dumps the bits of a word."
       << "returns exactly " + bit_length + " (" + std::to_string(conf::bit_length)
              + ") bools.";

  tests << bdump("0") = utl::cat(svect(conf::bit_length, "0"), ", ") >> docs;
  tests << bdump("0x" + utl::cat(samp::himin)) =
      ("1, " + utl::cat(svect(conf::bit_length - 1, "0"), ", ")) >> docs;

  def bits = def{"bits(" + utl::cat(utl::alpha_base52_seq(conf::word_size), ", ")
                 + ")"} = [&](pack args) {
    std::vector<std::string> res{};
    std::ranges::transform(args, std::back_inserter(res), [&](auto&& v) {
      return impl::hex(v, "BITS");
    });
    return utl::cat(res, ", ");
  };

  return def<"o(...)">{[&](va args) {
    return args;
  }}(bits + " " + utup(args));
});

} // namespace api

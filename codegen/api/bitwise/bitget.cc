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

#include "bitwise.h"

namespace api {

using namespace codegen;

decltype(bitget_) bitget_ = NIFTY_DEF(bitget_, [&](va args) {
  docs << "gets the ith bit from the uint."
       << "i must be less than " + uint_bits + " (" + std::to_string(conf::uint_bits) + ").";

  auto binmaxminus1 = "0b" + utl::cat(std::vector<std::string>(conf::uint_bits - 1, "1")) + "0u";

  tests << bitget_(2, 7)            = "0" >> docs;
  tests << bitget_(2, 8)            = "1" >> docs;
  tests << bitget_(2, 9)            = "0" >> docs;
  tests << bitget_(binmaxminus1, 9) = "0" >> docs;

  auto bits = utl::cat(utl::alpha_base52_seq(conf::uint_bits), ", ");

  def _0 = def{"0(" + bits + ")"} = [&](pack args) {
    return args[0];
  };

  for (std::size_t i = 1; i < conf::uint_bits; ++i) {
    def{"" + std::to_string(i) + "(" + bits + ")"} = [&](pack args) {
      return args[i];
    };
  }

  return def<"o(op, bits)">{[&](arg op, arg bits) {
    return op + " " + bits;
  }}(cat(utl::slice(_0, -1), decimal(rest(args))),
     first(rest(rest(cat(utl::slice(detail::uint_traits[0], -1), binary(first(args)))))));
});

} // namespace api

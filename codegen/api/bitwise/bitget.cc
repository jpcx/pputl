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

decltype(bitget) bitget = NIFTY_DEF(bitget, [&](va args) {
  docs << "gets the ith bit from the uint."
       << "i must be less than " + bit_length + " (" + std::to_string(conf::bit_length) + ").";

  auto binmaxminus1 = "0b" + utl::cat(std::vector<std::string>(conf::bit_length - 1, "1")) + "0u";

  tests << bitget(2, 7)            = "0" >> docs;
  tests << bitget(2, 8)            = "1" >> docs;
  tests << bitget(2, 9)            = "0" >> docs;
  tests << bitget(binmaxminus1, 9) = "0" >> docs;

  auto bits_ = utl::cat(utl::alpha_base52_seq(conf::bit_length), ", ");

  def _0 = def{"0(" + bits_ + ")"} = [&](pack args) {
    return args[0];
  };

  for (std::size_t i = 1; i < conf::bit_length; ++i) {
    def{"" + std::to_string(i) + "(" + bits_ + ")"} = [&](pack args) {
      return args[i];
    };
  }

  return def<"o(v, i)">{[&](arg v, arg i) {
    return def<"o(i, ...)">{[&](arg i, va args) {
      return pp::call(cat(utl::slice(_0, -1), i), args);
    }}(ibase10(i), bits(v));
  }}(args);
});

} // namespace api

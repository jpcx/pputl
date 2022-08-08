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

#include <span>

#include "bitwise.h"

namespace api {

using namespace codegen;

decltype(bitset_) bitset_ = NIFTY_DEF(bitset_, [&](va args) {
  docs << "sets the ith bit in the uint to b."
       << "i must be less than " + uint_bits + " (" + std::to_string(conf::uint_bits) + ").";

  auto binmax       = "0b" + utl::cat(std::vector<std::string>(conf::uint_bits, "1")) + "u";
  auto binmaxminus1 = "0b" + utl::cat(std::vector<std::string>(conf::uint_bits - 1, "1")) + "0u";

  tests << bitset_(0, conf::uint_bits - 2, 1)      = "2" >> docs;
  tests << bitset_(1, conf::uint_bits - 3, 1)      = "5" >> docs;
  tests << bitset_(binmax, conf::uint_bits - 1, 0) = binmaxminus1 >> docs;

  auto params_v = utl::alpha_base52_seq(conf::uint_bits);
  auto params   = std::deque<std::string>{params_v.begin(), params_v.end()};
  params.push_front("a");
  for (auto&& v : params) {
    if (v == "b")
      v = "B";
    else if (v == "u")
      v = "U";
    else if (v == "B")
      v = "_B";
    else if (v == "U")
      v = "_U";
  }
  auto bitswapped = [&](unsigned i) -> std::string {
    auto cpy   = params;
    cpy[0]     = cpy[i + 1];
    cpy[i + 1] = '_';
    return utl::cat(cpy, ", ");
  };

  def _0 = def{"0(" + bitswapped(0) + ")"} = [&](pack args) {
    auto rest_bits = std::vector<std::string>{args.begin() + 2, args.end()};
    return pp::cat("0b", args[0], pp::cat(rest_bits), "u");
  };

  {
    std::size_t i = 1;

    for (; i < conf::uint_bits - 1; ++i) {
      def{std::to_string(i) + "(" + bitswapped(i) + ")"} = [&](pack args) {
        auto first_bits = std::vector<std::string>{args.begin() + 1, args.begin() + 1 + i};
        auto rest_bits  = std::vector<std::string>{args.begin() + 2 + i, args.end()};
        return pp::cat("0b", pp::cat(first_bits), args[0], pp::cat(rest_bits), "u");
      };
    }

    def{std::to_string(i) + "(" + bitswapped(i) + ")"} = [&](pack args) {
      auto first_bits = std::vector<std::string>{args.begin() + 1, args.begin() + 1 + i};
      return pp::cat("0b", pp::cat(first_bits), args[0], "u");
    };
  }

  return def<"o(v, i, b)">{[&](arg v, arg i, arg b) {
    return pp::call(typeof(v),
                    def<"o(b, op, bits)">{[&](arg b, arg op, arg bits) {
                      return def<"x(b, op, ...)">{[&](arg b, arg op, va args) {
                        return pp::call(op, b, args);
                      }}(b, op, items(bits));
                    }}(b, cat(utl::slice(_0, -1), decimal(i)),
                       first(rest(rest(cat(utl::slice(detail::uint_traits[0], -1), binary(v)))))));
  }}(args);
});

} // namespace api

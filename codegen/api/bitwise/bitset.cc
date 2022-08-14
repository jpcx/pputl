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

decltype(bitset) bitset = NIFTY_DEF(bitset, [&](va args) {
  docs << "sets the ith bit of the word to b."
       << "i must be between 0 and " + bit_length + " (" + std::to_string(conf::bit_length) + ").";

  /* auto binmax       = "0b" + utl::cat(std::vector<std::string>(conf::bit_length, "1")) + "u"; */
  /* auto binmaxminus1 = "0b" + utl::cat(std::vector<std::string>(conf::bit_length - 1, "1")) +
   * "0u"; */

  auto zero   = std::vector<std::string>(conf::word_size, "0");
  auto two    = zero;
  two.back()  = "2";
  auto five   = zero;
  five.back() = "5";
  auto _21    = zero;
  if constexpr (conf::word_size > 1) {
    *(_21.rbegin() + 1) = "1";
    *(_21.rbegin() + 0) = "5";
  }

  tests << bitset(0, conf::bit_length - 2, 1)    = pp::tup(two) >> docs;
  tests << bitset("1u", conf::bit_length - 3, 1) = pp::tup(five) >> docs;
  if constexpr (conf::word_size > 1)
    tests << bitset(5, conf::bit_length - 5, 1) = pp::tup(_21) >> docs;

  std::vector<std::string> params{"_"};
  std::ranges::copy(utl::alpha_base52_seq(conf::bit_length), std::back_inserter(params));

  def<"n0000"> n0000 = [&] {
    return "0";
  };

  def<"n0001">{} = [&] {
    return "1";
  };

  def<"n0010">{} = [&] {
    return "2";
  };

  def<"n0011">{} = [&] {
    return "3";
  };

  def<"n0100">{} = [&] {
    return "4";
  };

  def<"n0101">{} = [&] {
    return "5";
  };

  def<"n0110">{} = [&] {
    return "6";
  };

  def<"n0111">{} = [&] {
    return "7";
  };

  def<"n1000">{} = [&] {
    return "8";
  };

  def<"n1001">{} = [&] {
    return "9";
  };

  def<"n1010">{} = [&] {
    return "A";
  };

  def<"n1011">{} = [&] {
    return "B";
  };

  def<"n1100">{} = [&] {
    return "C";
  };

  def<"n1101">{} = [&] {
    return "D";
  };

  def<"n1110">{} = [&] {
    return "E";
  };

  def<"n1111">{} = [&] {
    return "F";
  };

  def<"x(...)"> x = [&](va args) {
    return args;
  };

  def bin =
      def{"bin(" + utl::cat(utl::alpha_base52_seq(conf::word_size), ", ") + ")"} = [&](pack args) {
        std::vector<std::string> res{};
        std::ranges::transform(args, std::back_inserter(res), [&](auto&& v) {
          return esc + " " + impl::hex_trait(v, "BIN");
        });
        return utl::cat(res, ", ");
      };

  std::array<std::string, conf::word_size> res{};
  for (std::size_t i = 0; i < res.size(); ++i) {
    res[i] =
        pp::cat(utl::slice(n0000, -4),
                pp::cat(utl::alpha_base52_seq(4, std::string{static_cast<char>('a' + i * 4)})));
  }

  auto p0 = params;
  std::swap(p0[0], p0[1]);
  def _0 = def{"0(" + utl::cat(p0, ", ") + ")"} = [&](pack) {
    return pp::tup(res);
  };

  for (std::size_t i = 1; i < conf::bit_length; ++i) {
    auto pn = params;
    std::swap(pn[0], pn[i + 1]);
    def{"" + std::to_string(i) + "(" + utl::cat(pn, ", ") + ")"} = [&](pack) {
      return pp::tup(res);
    };
  }

  return def<"o(e, v, i, b)">{[&](arg e, arg v, arg i, arg b) {
    return def<"<o(...)">{[&](va args) {
      return def<"<o(e, i, b, gelt, ...)">{[&](arg e, arg i, arg b, arg gelt, va args) {
        def<"0(e, ...)"> gelt0 = [&](arg e, va) {
          return fail(e);
        };

        def<"1(e, i, b, ...)">{} = [&](arg, arg i, arg b, va args) {
          return pp::call(pp::cat(utl::slice(_0, -1), i), b, args);
        };

        return pp::call(pp::cat(utl::slice(gelt0, -1), gelt), e, i, b, args);
      }}(args);
    }}(e, idec(i), bool_(b), and_(ge(i, 0), lt(i, conf::bit_length)), x(bin + " " + hword(v)));
  }}(istr("[" + bitset + "] invalid index; args : " + args), args);
});

} // namespace api

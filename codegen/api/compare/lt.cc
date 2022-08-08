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

#include "compare.h"

namespace api {

using namespace codegen;

decltype(lt) lt = NIFTY_DEF(lt, [&](va args) {
  docs << "uint less-than comparison.";

  constexpr auto max = conf::uint_max;

  tests << lt("0, 0")           = "0" >> docs;
  tests << lt("0, 1")           = "1" >> docs;
  tests << lt("0, 2")           = "1";
  tests << lt("0, 3")           = "1";
  tests << lt("1, 0")           = "0" >> docs;
  tests << lt("1, 1")           = "0" >> docs;
  tests << lt("1, 2")           = "1";
  tests << lt("1, 3")           = "1";
  tests << lt("2, 0")           = "0";
  tests << lt("2, 1")           = "0";
  tests << lt("2, 2")           = "0";
  tests << lt("2, 3")           = "1";
  tests << lt("3, 0")           = "0";
  tests << lt("3, 1")           = "0";
  tests << lt("3, 2")           = "0";
  tests << lt("3, 3")           = "0";
  tests << lt(0, max)           = "1";
  tests << lt(0, max - 1)       = "1";
  tests << lt(1, max)           = "1";
  tests << lt(1, max - 1)       = "1";
  tests << lt(max, 0)           = "0";
  tests << lt(max - 1, 0)       = "0";
  tests << lt(max, 1)           = "0";
  tests << lt(max - 1, 1)       = "0";
  tests << lt(max, max)         = "0";
  tests << lt(max, max - 1)     = "0";
  tests << lt(max - 1, max)     = "1";
  tests << lt(max - 1, max - 1) = "0";

  def<"0000"> _0000 = [&] {
    return "0";
  };

  def<"0001">{} = [&] {
    return "1";
  };

  def<"0010">{} = [&] {
    return "0";
  };

  def<"0011">{} = [&] {
    return "0";
  };

  def<"0100">{} = [&] {
    return "0";
  };

  def<"0101">{} = [&] {
    return "0";
  };

  def<"0110">{} = [&] {
    return "0";
  };

  def<"0111">{} = [&] {
    return "0";
  };

  def<"1000">{} = [&] {
    return "1";
  };

  def<"1001">{} = [&] {
    return "1";
  };

  def<"1010">{} = [&] {
    return "1";
  };

  def<"1011">{} = [&] {
    return "1";
  };

  def<"1100">{} = [&] {
    return "1";
  };

  def<"1101">{} = [&] {
    return "1";
  };

  def<"1110">{} = [&] {
    return "1";
  };

  def<"1111">{} = [&] {
    return "1";
  };

  def<"r(...)"> r = [&](va args) {
    return def<"o(fl, fg, a, b, ...)">{[&](arg fl, arg fg, arg a, arg b, va args) {
      return pp::cat(utl::slice(_0000, -4), fl, fg, a, b) + ", "
           + pp::cat(utl::slice(_0000, -4), fg, fl, b, a) + ", " + args;
    }}(args);
  };

  def zip = def{"zip(" + utl::cat(utl::alpha_base52_seq(conf::uint_bits * 2), ", ")
                + ")"} = [&](pack args) {
    std::vector<std::string> res;
    for (std::size_t i = 0; i < args.size() / 2; ++i) {
      res.push_back(args[i]);
      res.push_back(args[i + (args.size() / 2)]);
    }
    return utl::cat(res, ", ");
  };

  return def<"o(l, r)">{[&](arg l, arg r_) {
    return def<"o(...)">{[&](va args) {
      return first(utl::cat(std::vector<std::string>(conf::uint_bits, r + "(")) + "0, 0, "
                   + zip(args) + utl::cat(std::vector<std::string>(conf::uint_bits, ")")));
    }}(bits(l), bits(r_));
  }}(args);
});

} // namespace api

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

decltype(decimal) decimal = NIFTY_DEF(decimal, [&](va args) {
  docs << "uint decimal from binary lookup."
       << "number of bits must equal " + uint_bits + " (" + std::to_string(conf::uint_bits) + ").";

  using std::string;
  using pp::tup;
  using std::vector;

  tests << decimal(tup(utl::cat(vector<string>(conf::uint_bits, "0"), ", ")))                = "0";
  tests << decimal(tup(utl::cat(vector<string>(conf::uint_bits - 1, "0"), ", ") + ", 1"))    = "1";
  tests << decimal(tup(utl::cat(vector<string>(conf::uint_bits - 2, "0"), ", ") + ", 1, 0")) = "2";
  tests << decimal(tup(utl::cat(vector<string>(conf::uint_bits - 2, "0"), ", ") + ", 1, 1")) = "3";
  tests << decimal(tup(utl::cat(vector<string>(conf::uint_bits - 3, "0"), ", ") + ", 1, 0, 0")) =
      "4";
  tests << decimal(tup(utl::cat(vector<string>(conf::uint_bits - 3, "0"), ", ") + ", 1, 0, 1")) =
      "5" >> docs;
  tests << decimal(tup(utl::cat(vector<string>(conf::uint_bits - 3, "0"), ", ") + ", 1, 1, 0")) =
      "6";
  tests << decimal(tup(utl::cat(vector<string>(conf::uint_bits - 3, "0"), ", ") + ", 1, 1, 1")) =
      "7";
  tests << decimal(tup(utl::cat(vector<string>(conf::uint_bits, "1"), ", "))) = uint_max_s;

  auto res =
      def{"res(" + utl::cat(utl::alpha_base52_seq(conf::uint_bits), ", ") + ")"} = [&](pack args) {
        return pp::cat(utl::slice(detail::uint_decimal[0], -conf::uint_bits), utl::cat(args, "##"));
      };

  return def<"x(...)">{[&](va args) {
    return res + " " + args;
  }}(tuple(args));
});

} // namespace api

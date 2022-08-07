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

decltype(binary) binary = NIFTY_DEF(binary, [&](va args) {
  docs << "uint binary lookup.";

  using std::string;
  using pp::tup;
  using std::vector;

  tests << binary(0) = tup(utl::cat(vector<string>(conf::uint_bits, "0"), ", "));
  tests << binary(1) = tup(utl::cat(vector<string>(conf::uint_bits - 1, "0"), ", ") + ", 1");
  tests << binary(2) = tup(utl::cat(vector<string>(conf::uint_bits - 2, "0"), ", ") + ", 1, 0");
  tests << binary(3) = tup(utl::cat(vector<string>(conf::uint_bits - 2, "0"), ", ") + ", 1, 1");
  tests << binary(4) = tup(utl::cat(vector<string>(conf::uint_bits - 3, "0"), ", ") + ", 1, 0, 0");
  tests << binary(5) =
      (tup(utl::cat(vector<string>(conf::uint_bits - 3, "0"), ", ") + ", 1, 0, 1")) >> docs;
  tests << binary(6) = tup(utl::cat(vector<string>(conf::uint_bits - 3, "0"), ", ") + ", 1, 1, 0");
  tests << binary(7) = tup(utl::cat(vector<string>(conf::uint_bits - 3, "0"), ", ") + ", 1, 1, 1");
  tests << binary(conf::uint_max) =
      tup(utl::cat(vector<string>(conf::uint_bits, "1"), ", ")) >> docs;

  auto res =
      def{"res(" + utl::cat(utl::alpha_base52_seq(conf::uint_bits), ", ") + ")"} = [&](pack args) {
        return tup(utl::cat(args, ", "));
      };

  return def<"x(...)">{[&](va args) {
    return def<"x(de, in, lg, dv, ml, mlf, sq, pw, pwf, m2, m4, m8, m16, m32, m64, fact, bin)">{
        [&](pack args) {
          return res + " " + args.back();
        }}(args);
  }}(cat(utl::slice(detail::uint_traits[0], -1), uint(args)));
});

} // namespace api

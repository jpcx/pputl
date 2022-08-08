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

decltype(bitnot_) bitnot_ = NIFTY_DEF(bitnot_, [&](va args) {
  docs << "bitwise NOT."
       << "returns the same uint representation as its input.";

  auto binmin       = "0b" + utl::cat(std::vector<std::string>(conf::uint_bits, "0")) + "u";
  auto binone       = "0b" + utl::cat(std::vector<std::string>(conf::uint_bits - 1, "0")) + "1u";
  auto binmax       = "0b" + utl::cat(std::vector<std::string>(conf::uint_bits, "1")) + "u";
  auto binmaxminus1 = "0b" + utl::cat(std::vector<std::string>(conf::uint_bits - 1, "1")) + "0u";

  tests << bitnot_("0")    = uint_max_s >> docs;
  tests << bitnot_("1")    = std::to_string(conf::uint_max - 1) >> docs;
  tests << bitnot_(binmin) = binmax >> docs;
  tests << bitnot_(binone) = binmaxminus1 >> docs;

  def<"not(...)"> not_ = [&](va args) {
    return def<"x(t, d, b, n)">{[&](arg, arg, arg, arg n) {
      return n;
    }}(args);
  };

  return pp::call(typeof(args), not_(cat(utl::slice(detail::uint_traits[0], -1), binary(args))));
});

} // namespace api

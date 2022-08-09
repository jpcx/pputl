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

#include "traits.h"

namespace api {

using namespace codegen;

decltype(bits) bits = NIFTY_DEF(bits, [&](va args) {
  docs << "extracts uint bits."
       << "returns exactly " + bit_length + " (" + std::to_string(conf::bit_length) + ") args.";

  auto binmaxminus1 = "0b" + utl::cat(std::vector<std::string>(conf::bit_length - 1, "1")) + "0u";

  tests << bits(0) = utl::cat(std::vector<std::string>(conf::bit_length, "0"), ", ") >> docs;
  tests << bits(1) =
      (utl::cat(std::vector<std::string>(conf::bit_length - 1, "0"), ", ") + ", 1") >> docs;
  tests << bits(binmaxminus1) =
      (utl::cat(std::vector<std::string>(conf::bit_length - 1, "1"), ", ") + ", 0") >> docs;

  def bits =
      def{"x(" + utl::cat(utl::alpha_base52_seq(conf::bit_length), ", ") + ")"} = [&](pack args) {
        return utl::cat(args, ", ");
      };

  return esc(bits + " " + detail::uint_trait(ubase2(args), "BIN_BITS"));
});

} // namespace api

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

#include "type.h"

namespace api {

using namespace codegen;

decltype(ubase2) ubase2 = NIFTY_DEF(ubase2, [&](va args) {
  docs << "casts to the unsigned int binary subtype.";

  auto binmin     = "0b" + utl::cat(std::vector<std::string>(conf::bit_length, "0")) + "u";
  auto binone     = "0b" + utl::cat(std::vector<std::string>(conf::bit_length - 1, "0")) + "1u";
  auto binmax     = "0b" + utl::cat(std::vector<std::string>(conf::bit_length, "1")) + "u";
  auto binmax_int = binmax;
  binmax_int.pop_back();
  auto binfive = "0b" + utl::cat(std::vector<std::string>(conf::bit_length - 3, "0")) + "101u";

  tests << ubase2(0)          = binmin >> docs;
  tests << ubase2(1u)         = binone >> docs;
  tests << ubase2(5)          = binfive >> docs;
  tests << ubase2(uint_max_s) = binmax >> docs;
  tests << ubase2(binmin)     = binmin >> docs;
  tests << ubase2(binone)     = binone >> docs;
  tests << ubase2(binmax_int) = binmax >> docs;

  def<"\\DEC(n)"> dec = [&](arg n) {
    return cat(detail::uint_trait(n, "DEC_IBIN"), "u");
  };

  def<"\\BIN(n)">{} = [&](arg n) {
    return n;
  };

  return def<"o(n)">{[&](arg n) {
    return pp::call(cat(utl::slice(dec, -3), detail::uint_trait(n, "TYPE")), n);
  }}(uint(args));
});

} // namespace api

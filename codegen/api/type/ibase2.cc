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

decltype(ibase2) ibase2 = NIFTY_DEF(ibase2, [&](va args) {
  docs << "casts to the signed int binary subtype.";

  auto binmin  = "0b" + utl::cat(std::vector<std::string>(conf::bit_length, "0"));
  auto binfive = "0b" + utl::cat(std::vector<std::string>(conf::bit_length - 3, "0")) + "101";
  auto binmax  = "0b" + utl::cat(std::vector<std::string>(conf::bit_length, "1"));
  auto binimax = "0b0" + utl::cat(std::vector<std::string>(conf::bit_length - 1, "1"));

  tests << ibase2(0)                                   = binmin >> docs;
  tests << ibase2(5u)                                  = binfive >> docs;
  tests << ibase2(uint_max_s)                          = binmax >> docs;
  tests << ibase2(std::to_string(conf::int_max) + "u") = binimax >> docs;

  def<"\\DEC(n, u)"> dec = [&](arg, arg u) {
    return detail::uint_trait(u, "DEC_IBIN");
  };

  def<"\\BIN(n, u)">{} = [&](arg n, arg) {
    return n;
  };

  return def<"o(n)">{[&](arg n) {
    return def<"o(n, u)">{[&](arg n, arg u) {
      return pp::call(cat(utl::slice(dec, -3), detail::uint_trait(u, "TYPE")), n, u);
    }}(n, cat(n, "u"));
  }}(int_(args));
});

} // namespace api

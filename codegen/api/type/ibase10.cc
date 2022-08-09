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

decltype(ibase10) ibase10 = NIFTY_DEF(ibase10, [&](va args) {
  docs << "[positive values only] casts to the signed int decimal subtype."
       << ""
       << "value will remain as binary if negative."
       << "use fmt.paste_int to get a negative decimal.";

  auto binmin     = "0b" + utl::cat(std::vector<std::string>(conf::bit_length, "0"));
  auto binfive    = "0b" + utl::cat(std::vector<std::string>(conf::bit_length - 3, "0")) + "101";
  auto binmax     = "0b" + utl::cat(std::vector<std::string>(conf::bit_length, "1"));
  auto binimax    = "0b0" + utl::cat(std::vector<std::string>(conf::bit_length - 1, "1"));
  auto binmin_int = "0b1" + utl::cat(std::vector<std::string>(conf::bit_length - 1, "0"));

  tests << ibase10(binmin)         = "0" >> docs;
  tests << ibase10(binfive + "u")  = "5" >> docs;
  tests << ibase10(binimax)        = int_max_s >> docs;
  tests << ibase10(binmin_int)     = binmin_int >> docs;
  tests << ibase10(conf::int_max)  = int_max_s >> docs;
  tests << ibase10(conf::uint_max) = binmax >> docs;

  def<"msb(a, ...)"> msb = [&](arg a, va) {
    return a;
  };

  def<"\\DEC(n, u)"> dec = [&](arg n, arg) {
    return n;
  };

  def<"\\BIN(n, u)">{} = [&](arg n, arg u) {
    def<"0(n, u)"> _0 = [&](arg, arg u) {
      return detail::uint_trait(u, "BIN_IDEC");
    };

    def<"1(n, u)">{} = [&](arg n, arg) {
      return n;
    };

    return pp::call(cat(utl::slice(_0, -1), esc(msb + " " + detail::uint_trait(u, "BIN_BITS"))), n,
                    u);
  };

  return def<"o(n)">{[&](arg n) {
    return def<"o(n, u)">{[&](arg n, arg u) {
      return pp::call(cat(utl::slice(dec, -3), detail::uint_trait(u, "TYPE")), n, u);
    }}(n, cat(n, "u"));
  }}(int_(args));
});

} // namespace api

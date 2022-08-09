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

decltype(idec) idec = NIFTY_DEF(idec, [&](va args) {
  docs << "[positive-only] casts to the signed int decimal subtype."
       << "fails on negative ints."
       << ""
       << "use fmt.paste with ihex to get negative decimals.";

  auto zero = "0x" + utl::cat(std::vector<std::string>(conf::hex_length, "0"));
  auto one  = "0x" + utl::cat(std::vector<std::string>(conf::hex_length - 1, "0")) + "1";
  auto max  = "0x7" + utl::cat(std::vector<std::string>(conf::hex_length - 1, "F"));
  auto five = "0x" + utl::cat(std::vector<std::string>(conf::hex_length - 1, "0")) + "5";

  tests << idec(zero)          = "0" >> docs;
  tests << idec(one)           = "1" >> docs;
  tests << idec(five + "u")    = "5" >> docs;
  tests << idec(max)           = int_max_s >> docs;
  tests << idec(conf::int_max) = int_max_s >> docs;

  def<"\\DEC(e, n, u)"> dec = [&](arg, arg n, arg) {
    return n;
  };

  def<"\\HEX(e, n, u)">{} = [&](arg e, arg n, arg u) {
    def<"0(e, n, u)"> _0 = [&](arg, arg, arg u) {
      return detail::uint_trait(u, "HEX_IDEC");
    };

    def<"1(e, n, u)">{} = [&](arg e, arg, arg) {
      return fail(e);
    };

    return pp::call(cat(utl::slice(_0, -1), esc(ifirst + " " + detail::uint_trait(u, "HEX_BITS"))),
                    e, n, u);
  };

  return def<"o(e, n)">{[&](arg e, arg n) {
    return def<"o(e, n, u)">{[&](arg e, arg n, arg u) {
      return pp::call(cat(utl::slice(dec, -3), detail::uint_trait(u, "TYPE")), e, n, u);
    }}(e, n, cat(n, "u"));
  }}(istr("[" + idec + "] cannot represent negative in base10 : " + args), int_(args));
});

} // namespace api

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
  docs << "[inherits from " + int_ + "] casts to the signed int decimal subtype."
       << "positive values only! fails on negative ints."
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

  def<"0(e, ihex)"> _0 = [&](arg e, arg ihex) {
    return def<"o(e, uhex)">{[&](arg e, arg uhex) {
      def<"0(e, uhex)"> _0 = [&](arg, arg uhex) {
        return impl::uint_trait(uhex, "HIDEC");
      };

      def<"1(e, uhex)">{} = [&](arg e, arg) {
        return fail(e);
      };

      return pp::call(
          cat(utl::slice(_0, -1), impl::uint_trait(impl::uint_trait(uhex, "HUDEC"), "DINEG")), e,
          uhex);
    }}(e, pp::cat(ihex, 'u'));
  };

  def<"1(e, idec)">{} = [&](arg, arg idec) {
    return idec;
  };

  return def<"o(e, int)">{[&](arg e, arg int_) {
    return pp::call(cat(utl::slice(_0, -1), detail::is_idec_o(int_)), e, int_);
  }}(istr("[" + idec + "] cannot represent negative in base10 : " + args), int_(args));
});

} // namespace api

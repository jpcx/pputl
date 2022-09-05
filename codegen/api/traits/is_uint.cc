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

namespace detail {
decltype(is_uint_o) is_uint_o = NIFTY_DEF(is_uint_o);
}

decltype(is_uint) is_uint = NIFTY_DEF(is_uint, [&](va args) {
  docs << "[extends " + is_udec + "|" + is_uhex + "|" + is_utup
              + "] detects if args is an unsigned integer.";

  auto min = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "0"));
  auto max = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "F"));

  tests << is_uint()                     = "0" >> docs;
  tests << is_uint("foo")                = "0" >> docs;
  tests << is_uint(0)                    = "0" >> docs;
  tests << is_uint("0u")                 = "1" >> docs;
  tests << is_uint(pp::tup(samp::himax)) = "1" >> docs;
  tests << is_uint(conf::uint_max)       = "0" >> docs;
  tests << is_uint(uint_max_s)           = "1" >> docs;
  tests << is_uint(min + "u")            = "1" >> docs;
  tests << is_uint(max)                  = "0" >> docs;
  tests << is_uint("0b110u")             = "0" >> docs;
  tests << is_uint("(), ()")             = "0" >> docs;

  detail::is_uint_o = def{"o(obj)"} = [&](arg obj) {
    def<"0(atom)"> _0 = [&](arg atom) {
      def<"\\00"> _00 = [&] { return "0"; };
      def<"\\01">{}   = [&] { return "1"; };
      def<"\\10">{}   = [&] { return "1"; };

      return xcat(utl::slice(_00, -2), xcat(detail::is_enum_oo(impl::udec_prefix, atom),
                                            detail::is_enum_oo(impl::uhex_prefix, atom)));
    };

    def<"1(tup)">{} = [&](arg tup) { return detail::is_utup_o(tup); };

    return pp::call(xcat(utl::slice(_0, -1), detail::is_tup_o(obj)), obj);
  };

  def<"\\0"> _0 = [&] { return def<"fail(...)">{[&](va) { return "0"; }}; };
  def<"\\1">{}  = [&] { return detail::is_uint_o; };

  return pp::call(xcat(utl::slice(_0, -1), is_obj(args)), args);
});

} // namespace api

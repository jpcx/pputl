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
decltype(is_int_o) is_int_o = NIFTY_DEF(is_int_o);
}

decltype(is_int) is_int = NIFTY_DEF(is_int, [&](va args) {
  docs << "[extends " + is_atom + "] detects if args is a signed integer."
       << "hex length is fixed at " + word_size + " (" + std::to_string(conf::word_size)
              + ")."
       << "decimals must be smaller than " + int_max + " (" + int_max_s + ")."
       << "negative values must be hex; concatenation with '-' is not supported.";

  auto min = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "0"));
  auto max = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "F"));

  tests << is_int()               = "0" >> docs;
  tests << is_int("foo")          = "0" >> docs;
  tests << is_int(0)              = "1" >> docs;
  tests << is_int("0u")           = "0" >> docs;
  tests << is_int(conf::uint_max) = "0" >> docs;
  tests << is_int(min + "u")      = "0" >> docs;
  tests << is_int(max)            = "1" >> docs;
  if constexpr (conf::word_size > 1)
    tests << is_int("0xF") = "0" >> docs;
  tests << is_int("(), ()") = "0" >> docs;

  detail::is_int_o = def{"o(atom)"} = [&](arg atom) {
    def<"\\00(...)"> _00 = [&] { return "0"; };
    def<"\\01(uhex)">{}  = [&](arg) { return "1"; };
    def<"\\10(udec)">{}  = [&](arg udec) {
      def<"<\\0"> _0 = [&] { return "1"; };
      def<"<\\1">{}  = [&] { return "0"; };

      return xcat(utl::slice(_0, -1), impl::uhex(impl::udec(udec, "UHEX"), "ILTZ"));
    };

    return pp::call(xcat(utl::slice(_00, -2), xcat(impl::udec(pp::cat(atom, 'u'), "IS"),
                                                   impl::uhex(pp::cat(atom, 'u'), "IS"))),
                    pp::cat(atom, 'u'));
  };

  def<"\\0"> _0 = [&] { return def<"fail(...)">{[&](va) { return "0"; }}; };
  def<"\\1">{}  = [&] { return detail::is_int_o; };

  return pp::call(xcat(utl::slice(_0, -1), is_atom(args)), args);
});

} // namespace api

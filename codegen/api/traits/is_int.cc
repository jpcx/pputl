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
  docs << "[extends " + is_idec + "|" + is_ihex
              + "] detects if args is a signed integer.";

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
    def<"\\00"> _00 = [&] { return "0"; };
    def<"\\01">{}   = [&] { return "1"; };
    def<"\\10">{}   = [&] { return "1"; };

    return xcat(utl::slice(_00, -2),
                xcat(detail::is_idec_o(atom), detail::is_ihex_o(atom)));
  };

  def<"\\0"> _0 = [&] { return def<"fail(...)">{[&](va) { return "0"; }}; };
  def<"\\1">{}  = [&] { return detail::is_int_o; };

  return pp::call(xcat(utl::slice(_0, -1), is_atom(args)), args);
});

} // namespace api

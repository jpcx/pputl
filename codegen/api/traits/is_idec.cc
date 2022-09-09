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

#include <string>

#include "codegen.h"
#include "traits.h"

namespace api {

using namespace codegen;

namespace detail {
decltype(is_idec_o) is_idec_o = NIFTY_DEF(is_idec_o);
}

decltype(is_idec) is_idec = NIFTY_DEF(is_idec, [&](va args) {
  docs << "[extends " + is_enum + "] detects if args is an enum<0|1|...|"
              + std::to_string(conf::int_max - 1) + "|" + std::to_string(conf::int_max)
              + ">.";

  auto min = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "0"));
  auto max = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "F"));

  tests << is_idec("1")            = "1" >> docs;
  tests << is_idec("1u")           = "0" >> docs;
  tests << is_idec(int_max_s)      = "1" >> docs;
  tests << is_idec(conf::uint_max) = "0" >> docs;
  tests << is_idec(min + "u")      = "0" >> docs;
  tests << is_idec(max)            = "0" >> docs;
  tests << is_idec("(), ()")       = "0" >> docs;

  detail::is_idec_o = def{"o(atom)"} = [&](arg atom) {
    return def<"o(atom)">{[&](arg atom) {
      def<"\\0(atom)"> _0 = [&](arg) {
        return "0";
      };
      def<"\\1(udec)">{} = [&](arg udec) {
        def<"\\0"> _0 = [&] {
          return "1";
        };
        def<"\\1">{} = [&] {
          return "0";
        };
        return xcat(utl::slice(_0, -1), impl::uhex(impl::udec(udec, "UHEX"), "ILTZ"));
      };

      return pp::call(
          xcat(utl::slice(_0, -1), detail::is_enum_oo(impl::udec_prefix, atom)), atom);
    }}(pp::cat(atom, 'u'));
  };

  def<"\\0"> _0 = [&] {
    return def<"fail(...)">{[&](va) {
      return "0";
    }};
  };
  def<"\\1">{} = [&] {
    return detail::is_idec_o;
  };

  return pp::call(xcat(utl::slice(_0, -1), is_atom(args)), args);
});

} // namespace api

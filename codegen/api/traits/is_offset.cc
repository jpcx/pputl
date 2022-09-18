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

#include "codegen.h"
#include "traits.h"

namespace api {

using namespace codegen;

namespace detail {
decltype(is_offset_o) is_offset_o = NIFTY_DEF(is_offset_o);
}

decltype(is_offset) is_offset = NIFTY_DEF(is_offset, [&](va args) {
  docs << "[extends is_word] detects if args is any word whose abs is a valid size.";

  constexpr auto size_lt_max = conf::word_size > 2 and conf::cpp20_arglimit;

  tests << is_offset("0")                = "1" >> docs;
  tests << is_offset(conf::size_max - 1) = "1" >> docs;
  tests << is_offset(size_max_s)         = "1" >> docs;
  if constexpr (size_lt_max and conf::word_size > 2)
    tests << is_offset(std::to_string(conf::size_max + 1) + "u") = "0" >> docs;
  tests << is_offset("0x" + utl::cat(samp::hmax)) = "1" >> docs;
  if constexpr (conf::word_size > 2)
    tests << is_offset("0xF" + utl::cat(svect{conf::word_size - 2, "0"}) + "1") =
        "1" >> docs;
  if constexpr (conf::word_size > 1)
    tests << is_offset("0xF" + utl::cat(svect{conf::word_size - 1, "0"})) = "0" >> docs;

  detail::is_offset_o = def{"o(atom)"} = [&](arg atom) {
    if constexpr (size_lt_max) {
      def<"\\0000(atom)"> _0000 = [&](arg) {
        return "0";
      };
      def<"\\0001(uhex)">{} = [&](arg uhex) {
        return impl::udec(impl::uhex(uhex, "UDEC"), "UOFS");
      };
      def<"\\0010(udec)">{} = [&](arg udec) { //
        return impl::udec(udec, "UOFS");
      };
      def<"\\0100(ihex)">{} = [&](arg ihex) {
        return impl::udec(impl::uhex(pp::cat(ihex, 'u'), "UDEC"), "IOFS");
      };
      def<"\\1000(idec)">{} = [&](arg idec) { //
        return impl::udec(pp::cat(idec, 'u'), "IOFS");
      };

      return pp::call(xcat(utl::slice(_0000, -4),
                           xcat(xcat(detail::is_idec_o(atom), detail::is_ihex_o(atom)),
                                xcat(detail::is_enum_oo(impl::udec_prefix, atom),
                                     detail::is_enum_oo(impl::uhex_prefix, atom)))),
                      atom);
    } else {
      return detail::is_word_o(atom);
    }
  };

  def<"fail(...)"> fail{[&](va) {
    return "0";
  }};
  def<"\\0">       _0 = [&] {
    return fail;
  };
  def<"\\1">{} = [&] {
    return detail::is_offset_o;
  };

  return pp::call(xcat(utl::slice(_0, -1), is_atom(args)), args);
});

} // namespace api

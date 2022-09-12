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
decltype(is_word_o) is_word_o = NIFTY_DEF(is_word_o);
}

decltype(is_word) is_word = NIFTY_DEF(is_word, [&](va args) {
  docs << "[extends " + is_int + "|" + is_uint + "] detects if args is an integer.";

  tests << is_word("0")                               = "1" >> docs;
  tests << is_word("0u")                              = "1" >> docs;
  tests << is_word("foo")                             = "0" >> docs;
  tests << is_word("()")                              = "0" >> docs;
  tests << is_word("A")                               = "0" >> docs;
  tests << is_word(int_min_s)                         = "1" >> docs;
  tests << is_word(uint_max_s)                        = "1" >> docs;
  tests << is_word("0x" + utl::cat(samp::hmax) + "u") = "1" >> docs;

  detail::is_word_o = def{"o(obj)"} = [&](arg obj) {
    def<"\\0(atom)"> _0 = [&](arg atom) {
      def<"\\0(atom)"> _0 = [&](arg atom) {
        def<"\\00"> _00 = [&] {
          return "0";
        };
        def<"\\01">{} = [&] {
          return "1";
        };
        def<"\\10">{} = [&] {
          return "1";
        };

        return xcat(utl::slice(_00, -2),
                    xcat(detail::is_enum_oo(impl::udec_prefix, atom),
                         detail::is_enum_oo(impl::uhex_prefix, atom)));
      };
      def<"\\1(int)">{} = [&](arg) {
        return "1";
      };

      return pp::call(xcat(utl::slice(_0, -1), detail::is_int_o(atom)), atom);
    };

    def<"\\1(tup)">{} = [&](arg tup) {
      return detail::is_utup_o(tup);
    };

    return pp::call(xcat(utl::slice(_0, -1), detail::is_tup_o(obj)), obj);
  };

  def<"\\0"> _0 = [&] {
    return def<"fail(...)">{[&](va) {
      return "0";
    }};
  };
  def<"\\1">{} = [&] {
    return detail::is_word_o;
  };

  return pp::call(xcat(utl::slice(_0, -1), is_obj(args)), args);
});

} // namespace api

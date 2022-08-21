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
decltype(is_size_o) is_size_o = NIFTY_DEF(is_size_o);
}

decltype(is_size) is_size = NIFTY_DEF(is_size, [&](va args) {
  docs << "[extends " + is_word + "] detects if args is a word within 0 and " + size_max
              + " (" + size_max_s + ").";

  constexpr auto size_lt_max = conf::word_size > 2 and conf::cpp20_arglimit;

  tests << is_size("0")       = "1" >> docs;
  tests << is_size("0u")      = "1" >> docs;
  tests << is_size("foo")     = "0" >> docs;
  tests << is_size("()")      = "0" >> docs;
  tests << is_size("A")       = "0" >> docs;
  tests << is_size(int_min_s) = "0" >> docs;
  if constexpr (size_lt_max) {
    tests << is_size(uint_max_s)                        = "0" >> docs;
    tests << is_size("0x" + utl::cat(samp::hmax) + "u") = "0" >> docs;
    tests << is_size(size_max_s)                        = "1" >> docs;
  } else {
    tests << is_size(uint_max_s)                        = "1" >> docs;
    tests << is_size("0x" + utl::cat(samp::hmax) + "u") = "1" >> docs;
  }
  tests << is_size(pp::tup(samp::h8)) = "1" >> docs;

  detail::is_size_o = def{"o(word)"} = [&](arg word) {
    def<> chk;
    if constexpr (size_lt_max) {
      chk = def{"chk(" + utl::cat(utl::alpha_base52_seq(conf::word_size), ", ")
                + ")"} = [&](pack args) {
        def verify =
            def{"\\" + utl::cat(svect(conf::word_size - 2, "0"))} = [&] { return ""; };

        return is_none(pp::cat(utl::slice(verify, -(conf::word_size - 2)),
                               pp::cat(svect(args.begin(), args.end() - 2))));
      };
    }

    def<"\\0(atom)"> _0 = [&](arg atom) {
      def<"<\\0(uint)"> _0 = [&](arg uint) {
        if constexpr (size_lt_max) {
          def<"<\\0(uhex)"> _0 = [&](arg uhex) {
            return esc(chk + " " + impl::uhex(uhex, "UTUP"));
          };
          def<"<\\1(udec)">{} = [&](arg udec) {
            return esc(chk + " " + impl::uhex(impl::udec(udec, "UHEX"), "UTUP"));
          };

          return pp::call(cat(utl::slice(_0, -1), detail::is_udec_o(uint)), uint);
        } else {
          return "1";
        }
      };

      def<"<\\1(int)">{} = [&](arg int_) {
        def<"<\\0(ihex)"> _0 = [&](arg ihex) {
          if constexpr (size_lt_max) {
            def<"<\\0"> _0 = [&] { return "1"; };
            def<"<\\1">{}  = [&] { return "0"; };
            return cat(utl::slice(_0, -1), impl::uhex(pp::cat(ihex, 'u'), "ILTZ"));
          } else {
            def<"<\\0"> _0 = [&] { return "1"; };
            def<"<\\1">{}  = [&] { return "0"; };
            return cat(utl::slice(_0, -1), impl::uhex(pp::cat(ihex, 'u'), "ILTZ"));
          }
        };
        def<"<\\1(idec)">{} = [&](arg idec) {
          if constexpr (size_lt_max) {
            def<"<\\0"> _0 = [&] { return "1"; };
            def<"<\\1">{}  = [&] { return "0"; };
            return cat(utl::slice(_0, -1),
                       impl::uhex(impl::udec(pp::cat(idec, 'u'), "UHEX"), "ILTZ"));
          } else {
            return "1";
          }
        };

        return pp::call(cat(utl::slice(_0, -1), detail::is_idec_o(int_)), int_);
      };

      return pp::call(cat(utl::slice(_0, -1), detail::is_int_o(atom)), atom);
    };

    def<"\\1(utup)">{} = [&](arg utup) {
      if constexpr (size_lt_max) {
        return esc(chk + " " + utup);
      } else {
        return "1";
      }
    };

    return pp::call(cat(utl::slice(_0, -1), detail::is_tup_o(word)), word);
  };

  def<"\\0"> _0 = [&] { return def<"fail(...)">{[&](va) { return "0"; }}; };
  def<"\\1">{}  = [&] { return detail::is_size_o; };

  return pp::call(cat(utl::slice(_0, -1), is_word(args)), args);
});

} // namespace api

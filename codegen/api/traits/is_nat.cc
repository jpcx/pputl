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
decltype(is_nat_o) is_nat_o = NIFTY_DEF(is_nat_o);
}

decltype(is_nat) is_nat = NIFTY_DEF(is_nat, [&](va args) {
  docs << "[extends " + is_word + "] detects if args is a non-negative (natural) word.";

  tests << is_nat("0")                               = "1" >> docs;
  tests << is_nat("0u")                              = "1" >> docs;
  tests << is_nat("foo")                             = "0" >> docs;
  tests << is_nat("()")                              = "0" >> docs;
  tests << is_nat("A")                               = "0" >> docs;
  tests << is_nat(int_min_s)                         = "0" >> docs;
  tests << is_nat(size_max_s)                        = "1" >> docs;
  tests << is_nat(uint_max_s)                        = "1" >> docs;
  tests << is_nat("0x" + utl::cat(samp::hmax))       = "0" >> docs;
  tests << is_nat("0x" + utl::cat(samp::hmax) + "u") = "1" >> docs;
  tests << is_nat(pp::tup(samp::h8))                 = "1" >> docs;

  detail::is_nat_o = def{"o(word)"} = [&](arg word) {
    def<"\\0(atom)"> _0 = [&](arg atom) {
      def<"<\\0(uint)"> _0 = [&](arg) { return "1"; };

      def<"<\\1(int)">{} = [&](arg int_) {
        def<"<\\0(ihex)"> _0 = [&](arg ihex) {
          def<"<\\0"> _0 = [&] { return "1"; };
          def<"<\\1">{}  = [&] { return "0"; };
          return xcat(utl::slice(_0, -1), impl::uhex(pp::cat(ihex, 'u'), "ILTZ"));
        };
        def<"<\\1(idec)">{} = [&](arg) { return "1"; };

        return pp::call(xcat(utl::slice(_0, -1), detail::is_idec_o(int_)), int_);
      };

      return pp::call(xcat(utl::slice(_0, -1), detail::is_int_o(atom)), atom);
    };

    def<"\\1(utup)">{} = [&](arg) { return "1"; };

    return pp::call(xcat(utl::slice(_0, -1), detail::is_tup_o(word)), word);
  };

  def<"\\0"> _0 = [&] { return def<"fail(...)">{[&](va) { return "0"; }}; };
  def<"\\1">{}  = [&] { return detail::is_nat_o; };

  return pp::call(xcat(utl::slice(_0, -1), is_word(args)), args);
});

} // namespace api

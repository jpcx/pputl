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

decltype(typeof) typeof = NIFTY_DEF(typeof, [&](va args) {
  docs << "detects the value type. must be compatible with the ## operator."
       << "literal 0 through 9 are considered ibase10 rather than bool or hex."
       << ""
       << "returns one of:"
       << "- " + none << "- " + some << "- " + hword << "- " + tup << "- " + idec << "- " + ihex
       << "- " + udec << "- " + uhex << "- " + hex << "- " + atom;

  auto ihexneg1 = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "F"));
  auto ubinmax  = ihexneg1 + "u";

  tests << typeof("(foo)")        = tup >> docs;
  tests << typeof(0)              = idec >> docs;
  tests << typeof("0u")           = udec >> docs;
  tests << typeof("D")            = hex >> docs;
  tests << typeof(conf::uint_max) = atom >> docs;
  tests << typeof(uint_max_s)     = udec >> docs;
  tests << typeof(ihexneg1)       = ihex >> docs;
  tests << typeof(ubinmax)        = uhex >> docs;
  tests << typeof("foo")          = atom >> docs;
  tests << typeof("foo, bar")     = some >> docs;
  if constexpr (conf::word_size > 1)
    tests << typeof("(A)") = tup >> docs;
  tests << typeof(pp::tup(utl::cat(std::vector<std::string>(conf::word_size, "0"), ", "))) =
      hword >> docs;
  tests << typeof(pp::tup(utl::cat(std::vector<std::string>(conf::word_size, "F"), ", "))) =
      hword >> docs;
  tests << typeof() = none >> docs;

  // !none
  def<"0(...)"> _0 = [&](va some_) {
    docs << "^!none";

    // !any
    def<"<0(...)"> _0 = [&](va) {
      docs << "^!none → !any";
      return some;
    };

    // any
    def<"<1(any)">{} = [&](arg any) {
      docs << "^!none → any";

      // !tup
      def<"<0(atom)"> _0 = [&](arg atom_) {
        docs << "^!none → any → !tup";

        // !int
        def<"<0(atom)"> _0 = [&](arg atom_) {
          docs << "^!none → any → !tup → !int";

          // !uint
          def<"<0(atom)"> _0 = [&](arg atom_) {
            docs << "^!none → any → !tup → !int → !uint";

            // !hex
            def<"<0(atom)"> _0 = [&](arg) {
              docs << "^!none → any → !tup → !int → !uint → !hex";
              return atom;
            };

            def<"<1(hex)">{} = [&](arg) {
              docs << "^!none → any → !tup → !int → !uint → hex";
              return hex;
            };

            return pp::call(cat(utl::slice(_0, -1), detail::is_hex_o(atom_)), atom_);
          };

          // uint
          def<"<1(uint)">{} = [&](arg uint) {
            docs << "^!none → any → !tup → !int → uint";

            // !udec
            def<"<0(uhex)"> _0 = [&](arg) {
              docs << "^!none → any → !tup → !int → uint → !udec";
              return uhex;
            };

            // udec
            def<"<1(udec)">{} = [&](arg) {
              docs << "^!none → any → !tup → !int → uint → udec";
              return udec;
            };

            return pp::call(cat(utl::slice(_0, -1), detail::is_udec_o(uint)), uint);
          };

          return pp::call(cat(utl::slice(_0, -1), detail::is_uint_o(atom_)), atom_);
        };

        // int
        def<"<1(int)">{} = [&](arg int_) {
          docs << "^!none → any → !tup → int";

          // !idec
          def<"<0(ihex)"> _0 = [&](arg) {
            docs << "^!none → any → !tup → int → !idec";
            return ihex;
          };

          // idec
          def<"<1(idec)">{} = [&](arg) {
            docs << "^!none → any → !tup → int → idec";
            return idec;
          };

          return pp::call(cat(utl::slice(_0, -1), detail::is_idec_o(int_)), int_);
        };

        return pp::call(cat(utl::slice(_0, -1), detail::is_int_o(atom_)), atom_);
      };

      // tup
      def<"<1(tup)">{} = [&](arg tup_) {
        docs << "^!none → any → tup";

        // !hword
        def<"<0(tup)"> _0 = [&](arg) {
          docs << "^!none → any → tup → !hword";
          return tup;
        };

        // hword
        def<"<1(hword)">{} = [&](arg) {
          docs << "^!none → any → tup → hword";
          return hword;
        };

        return pp::call(cat(utl::slice(_0, -1), detail::is_hword_o(tup_)), tup_);
      };

      return pp::call(cat(utl::slice(_0, -1), detail::is_tup_o(any)), any);
    };

    return pp::call(cat(utl::slice(_0, -1), detail::is_any_o(some_ + ".")), some_);
  };

  // nothing
  def<"1(...)">{} = [&](va) {
    docs << "none";
    return none;
  };

  return pp::call(cat(utl::slice(_0, -1), is_none(args)), args);
});

} // namespace api

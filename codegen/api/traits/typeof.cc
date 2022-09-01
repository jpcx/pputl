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

decltype(typeof) typeof = NIFTY_DEF(typeof, [&](va args) {
  docs << "detects the value type. must be compatible with the ## operator."
       << ""
       << "note: literal 0-9 are always considered idec, not hex."
       << "      literal 1000-1111 are considered idec (not nybl) iff less than "
              + int_max + "."
       << "      words less than " + size_max
              + " are not specially considered to be size "
                "values."
       << ""
       << "returns one of:"
       << ""
       << "  | LIST | NONE | UTUP | TUP  | IDEC | IHEX"
       << "  | UDEC | UHEX | HEX  | NYBL | ATOM";

  auto ihexneg1 = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "F"));
  auto ubinmax  = ihexneg1 + "u";

  tests << typeof("(foo)")        = "TUP" >> docs;
  tests << typeof(0)              = "IDEC" >> docs;
  tests << typeof("0u")           = "UDEC" >> docs;
  tests << typeof("D")            = "HEX" >> docs;
  tests << typeof(conf::uint_max) = "ATOM" >> docs;
  tests << typeof(uint_max_s)     = "UDEC" >> docs;
  tests << typeof(ihexneg1)       = "IHEX" >> docs;
  tests << typeof(ubinmax)        = "UHEX" >> docs;
  tests << typeof("foo")          = "ATOM" >> docs;
  tests << typeof("001")          = "ATOM" >> docs;
  tests << typeof("0010")         = "NYBL" >> docs;
  tests << typeof("foo, bar")     = "LIST" >> docs;
  if constexpr (conf::word_size > 1)
    tests << typeof("(A)") = "TUP" >> docs;
  tests << typeof(pp::tup(
      utl::cat(std::vector<std::string>(conf::word_size, "0"), ", "))) = "UTUP" >> docs;
  tests << typeof(pp::tup(
      utl::cat(std::vector<std::string>(conf::word_size, "F"), ", "))) = "UTUP" >> docs;
  tests << typeof()                                                    = "NONE" >> docs;

  // !any
  def<"\\0(...)"> _0 = [&](va) {
    docs << "^!any";
    return "LIST";
  };

  // any
  def<"\\1(any)">{} = [&](arg any) {
    docs << "^!any";

    // !none
    def<"<\\0(obj)"> _0 = [&](arg obj) {
      docs << "^!any → !none";

      // !tup
      def<"<\\0(atom)"> _0 = [&](arg atom_) {
        docs << "^!any → !none → !tup";

        // !int
        def<"<\\0(atom)"> _0 = [&](arg atom_) {
          docs << "^!any → !none → !tup → !int";

          // !uint
          def<"<\\0(atom)"> _0 = [&](arg atom_) {
            docs << "^!any → !none → !tup → !int → !uint";

            // !hex
            def<"<\\0(atom)"> _0 = [&](arg atom_) {
              docs << "^!any → !none → !tup → !int → !uint → !hex";

              // !nybl
              def<"<\\0(atom)"> _0 = [&](arg) {
                docs << "^!any → !none → !tup → !int → !uint → !hex → !nybl";
                return "ATOM";
              };

              // nybl
              def<"<1(nybl)">{} = [&](arg) {
                docs << "^!any → !none → !tup → !int → !uint → !hex → nybl";
                return "NYBL";
              };

              return pp::call(xcat(utl::slice(_0, -1), is_nybl(atom_)), atom_);
            };

            // hex
            def<"<1(hex)">{} = [&](arg) {
              docs << "^!any → !none → !tup → !int → !uint → hex";
              return "HEX";
            };

            return pp::call(xcat(utl::slice(_0, -1), is_hex(atom_)), atom_);
          };

          // uint
          def<"<1(uint)">{} = [&](arg uint) {
            docs << "^!any → !none → !tup → !int → uint";

            // !udec
            def<"<\\0(uhex)"> _0 = [&](arg) {
              docs << "^!any → !none → !tup → !int → uint → !udec";
              return "UHEX";
            };

            // udec
            def<"<1(udec)">{} = [&](arg) {
              docs << "^!any → !none → !tup → !int → uint → udec";
              return "UDEC";
            };

            return pp::call(xcat(utl::slice(_0, -1), detail::is_udec_o(uint)), uint);
          };

          return pp::call(xcat(utl::slice(_0, -1), detail::is_uint_o(atom_)), atom_);
        };

        // int
        def<"<1(int)">{} = [&](arg int_) {
          docs << "^!any → !none → !tup → int";

          // !idec
          def<"<\\0(ihex)"> _0 = [&](arg) {
            docs << "^!any → !none → !tup → int → !idec";
            return "IHEX";
          };

          // idec
          def<"<1(idec)">{} = [&](arg) {
            docs << "^!any → !none → !tup → int → idec";
            return "IDEC";
          };

          return pp::call(xcat(utl::slice(_0, -1), detail::is_idec_o(int_)), int_);
        };

        return pp::call(xcat(utl::slice(_0, -1), detail::is_int_o(atom_)), atom_);
      };

      // tup
      def<"<1(tup)">{} = [&](arg tup_) {
        docs << "^!any → !none → tup";

        // !utup
        def<"<\\0(tup)"> _0 = [&](arg) {
          docs << "^!any → !none → tup → !utup";
          return "TUP";
        };

        // utup
        def<"<1(utup)">{} = [&](arg) {
          docs << "^!any → !none → tup → utup";
          return "UTUP";
        };

        return pp::call(xcat(utl::slice(_0, -1), detail::is_utup_o(tup_)), tup_);
      };

      return pp::call(xcat(utl::slice(_0, -1), detail::is_tup_o(obj)), obj);
    };

    // none
    def<"<1(...)">{} = [&](va) {
      docs << "^!any → none";
      return "NONE";
    };

    return pp::call(xcat(utl::slice(_0, -1), is_none(any)), any);
  };

  return pp::call(xcat(utl::slice(_0, -1), detail::is_any_o(args + ".")), args);
});

} // namespace api

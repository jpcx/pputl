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
       << " | NONE   | LIST  | TUP  | ARR  | MAP  | SET  | STACK | QUEUE"
       << " | PQUEUE | ATOM  | HEX  | NYBL | IDEC | IHEX | UDEC  | UHEX";

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
  tests << typeof()                   = "NONE" >> docs;
  tests << typeof(fwd::arr + "()")    = "ARR" >> docs;
  tests << typeof(fwd::map + "()")    = "MAP" >> docs;
  tests << typeof(fwd::set + "()")    = "SET" >> docs;
  tests << typeof(fwd::stack + "()")  = "STACK" >> docs;
  tests << typeof(fwd::queue + "()")  = "QUEUE" >> docs;
  tests << typeof(fwd::pqueue + "()") = "PQUEUE" >> docs;

  // !none
  def<"\\0(...)"> _0 = [&](va args) {
    docs << "^!none";

    // !none → !obj
    def<"\\0(...)"> _0 = [&](va) {
      docs << "^!none → !obj";
      return "LIST";
    };

    // !none → obj
    def<"\\1(obj)">{} = [&](arg obj) {
      docs << "^!none → obj";

      // !none → obj → !tup
      def<"\\0(obj)"> _0 = [&](arg obj) {
        docs << "^!none → obj → !tup";

        def<"\\000000(atom)"> _000000 = [&](arg atom) {
          docs << "^!none → obj → !tup → !(map|set|stack|queue|pqueue)";

          def<"\\000000"> _000000 = [&] {
            return "ATOM";
          }; // no match
          def<"\\000001">{} = [&] {
            return "UHEX";
          }; // uhex
          def<"\\000010">{} = [&] {
            return "UDEC";
          }; // udec
          def<"\\000100">{} = [&] {
            return "IHEX";
          }; // ihex
          def<"\\001000">{} = [&] {
            return "IDEC";
          }; // hex|bool|idec
          def<"\\010000">{} = [&] {
            return "NYBL";
          }; // nybl
          if constexpr (conf::int_max >= 1000)
            def<"\\011000">{} = [&] {
              return "IDEC";
            }; // nybl&idec
          def<"\\100000">{} = [&] {
            return "HEX";
          }; // hex
          def<"\\101000">{} = [&] {
            return "IDEC";
          }; // hex&idec

          return xcat(xcat(utl::slice(_000000, -6), xcat(is_hex(atom), is_nybl(atom))),
                      xcat(xcat(detail::is_idec_o(atom), detail::is_ihex_o(atom)),
                           xcat(is_udec(atom), is_uhex(atom))));
        };

        def<"\\000001(mem)">{} = [&](arg) {
          return "PQUEUE";
        };

        def<"\\000010(mem)">{} = [&](arg) {
          return "QUEUE";
        };

        def<"\\000100(mem)">{} = [&](arg) {
          return "STACK";
        };

        def<"\\001000(mem)">{} = [&](arg) {
          return "SET";
        };

        def<"\\010000(mem)">{} = [&](arg) {
          return "MAP";
        };

        def<"\\100000(mem)">{} = [&](arg) {
          return "ARR";
        };

        return pp::call(
            xcat(xcat(utl::slice(_000000, -6),
                      xcat(detail::is_arr_oo(obj), detail::is_map_oo(obj))),
                 xcat(xcat(detail::is_set_oo(obj), detail::is_stack_oo(obj)),
                      xcat(detail::is_queue_oo(obj), detail::is_pqueue_oo(obj)))),
            obj);
      };

      // !none → obj → tup
      def<"\\1(tup)">{} = [&](arg) {
        docs << "^!none → obj → tup";

        return "TUP";
      };

      return pp::call(xcat(utl::slice(_0, -1), detail::is_tup_o(obj)), obj);
    };

    return pp::call(xcat(utl::slice(_0, -1), is_obj(args)), args);
  };

  // none
  def<"\\1(...)">{} = [&](va) {
    docs << "^none";
    return "NONE";
  };

  return pp::call(xcat(utl::slice(_0, -1), is_none(args)), args);
});

} // namespace api

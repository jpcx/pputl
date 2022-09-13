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
  docs << "detects the most appropriate value type."
       << ""
       << "warn: must be one of the defined value types."
       << "      inputs such as `-`, `\"`, `foo()`, etc., are not"
       << "      detectable and will cause an unexpected pp error."
       << ""
       << "returns one of:"
       << ""
       << " | NONE  | LIST  | TUPLE | ARRAY | MAP  | PQUEUE | SET"
       << " | STACK | QUEUE | ATOM  | IDEC  | IHEX | UDEC   | UHEX";

  auto ihexneg1 = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "F"));
  auto ubinmax  = ihexneg1 + "u";

  tests << typeof("(foo)")            = "TUPLE" >> docs;
  tests << typeof(0)                  = "IDEC" >> docs;
  tests << typeof("0u")               = "UDEC" >> docs;
  tests << typeof(conf::uint_max)     = "ATOM" >> docs;
  tests << typeof(uint_max_s)         = "UDEC" >> docs;
  tests << typeof(ihexneg1)           = "IHEX" >> docs;
  tests << typeof(ubinmax)            = "UHEX" >> docs;
  tests << typeof("foo")              = "ATOM" >> docs;
  tests << typeof("001")              = "ATOM" >> docs;
  tests << typeof("foo, bar")         = "LIST" >> docs;
  tests << typeof()                   = "NONE" >> docs;
  tests << typeof(fwd::arr + "()")    = "ARRAY" >> docs;
  tests << typeof(fwd::map + "()")    = "MAP" >> docs;
  tests << typeof(fwd::pqueue + "()") = "PQUEUE" >> docs;
  tests << typeof(fwd::set + "()")    = "SET" >> docs;
  tests << typeof(fwd::stack + "()")  = "STACK" >> docs;
  tests << typeof(fwd::queue + "()")  = "QUEUE" >> docs;

  def<"\\0(...)"> _0 = [&](va args) {
    docs << "^!none";

    def<"\\0(...)"> _0 = [&](va) {
      docs << "^!none → !obj";
      return "LIST";
    };

    def<"\\1(obj)">{} = [&](arg obj) {
      docs << "^!none → obj";

      def<"\\0(atom)"> _0 = [&](arg atom) {
        docs << "^!none → obj → !tuple";

        def<"\\0(atom)"> _0 = [&](arg atom) {
          docs << "^!none → obj → !tuple → !array";

          def<"\\0000"> _0000 = [&] {
            return "ATOM";
          };
          def<"\\0001">{} = [&] {
            return "UHEX";
          };
          def<"\\0010">{} = [&] {
            return "UDEC";
          };
          def<"\\0100">{} = [&] {
            return "IHEX";
          };
          def<"\\1000">{} = [&] {
            return "IDEC";
          };

          return xcat(utl::slice(_0000, -4),
                      xcat(xcat(detail::is_idec_o(atom), detail::is_ihex_o(atom)),
                           xcat(detail::is_udec_o(atom), detail::is_uhex_o(atom))));
        };

        def<"\\1(array)">{} = [&](arg array) {
          docs << "^!none → obj → !tuple → array";

          def arr = def{"\\" + fwd::arr + "(...)"} = [&](va) {
            return "ARRAY";
          };
          def{"\\" + fwd::map + "(...)"} = [&](va) {
            return "MAP";
          };
          def{"\\" + fwd::pqueue + "(...)"} = [&](va) {
            return "PQUEUE";
          };
          def{"\\" + fwd::set + "(...)"} = [&](va) {
            return "SET";
          };
          def{"\\" + fwd::stack + "(...)"} = [&](va) {
            return "STACK";
          };
          def{"\\" + fwd::queue + "(...)"} = [&](va) {
            return "QUEUE";
          };

          return pp::cat(utl::slice(arr, -((std::string const&)fwd::arr).size()), array);
        };

        return pp::call(xcat(utl::slice(_0, -1), detail::is_array_oo(atom)), atom);
      };

      def<"\\1(tuple)">{} = [&](arg) {
        docs << "^!none → obj → tuple";

        return "TUPLE";
      };

      return pp::call(xcat(utl::slice(_0, -1), detail::is_tuple_o(obj)), obj);
    };

    return pp::call(xcat(utl::slice(_0, -1), is_object(args)), args);
  };

  def<"\\1(...)">{} = [&](va) {
    docs << "^none";
    return "NONE";
  };

  return pp::call(xcat(utl::slice(_0, -1), is_none(args)), args);
});

} // namespace api

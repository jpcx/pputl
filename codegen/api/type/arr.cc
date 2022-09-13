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

namespace detail {
decltype(arr_o) arr_o = NIFTY_DEF(arr_o);
}

decltype(arr) arr = NIFTY_DEF(arr, [&](va args) {
  docs << "[inherits from " + obj + "] a sized sequence of elements."
       << "construction from derived array type casts to base."
       << ""
       << "resultant object is an expansion-terminated self-reference."
       << ""
       << "note: constructor never fails. construction from existing array type"
       << "      assumes that the contained structure is valid, construction from"
       << "      explicit size assumes size is valid, and any inputs that do not"
       << "      match any other constructor fallback to list construction.";

  tests << arr()                  = arr("0u", pp::tup()) >> docs;
  tests << arr('a', 'b')          = arr("2u", pp::tup('a', 'b')) >> docs;
  tests << arr(3, "(, , )")       = arr(3, "(, , )") >> docs;
  tests << arr(pp::tup(1, 2, 3))  = arr("1u", pp::tup(pp::tup(1, 2, 3))) >> docs;
  tests << arr(arr("foo", "bar")) = arr("2u", pp::tup("foo", "bar")) >> docs;
  /* tests << arr(fwd::set + "(2, (8, 12))") = arr(2, pp::tup(8, 12)) >> docs; */
  /* tests << arr(fwd::map + "(2, ((3, foo), (7, bar)))") = */
  /*     arr(2, pp::tup(pp::tup(3, "foo"), pp::tup(7, "bar"))) >> docs; */

  detail::arr_o = def{"o(...)"} = [&](va args) {
    docs << "base array construction. returns array contents without type wrapper.";
    def<"\\0(...)"> _0 = [](va args) {
      def<"\\0(_, ...)"> _0 = [](arg first, va rest) {
        def<"\\00(...: none, list)"> _00 = [](va args) {
          return arr(args) + ", " + pp::tup(args);
        };

        def<"\\01(...: none, obj)">{} = [](va args) {
          return "2u, " + pp::tup(args);
        };

        def<"\\10(...: obj, list)">{} = [](va args) {
          return countof(args) + ", " + pp::tup(args);
        };

        def<"\\11(obj_a, obj_b)">{} = [](arg obj_a, arg obj_b) {
          def<"\\00(...: obj, obj)"> _00 = [](va args) {
            return "2u, " + pp::tup(args);
          };

          def<"\\01(...: obj, tup)">{} = [](va args) {
            return "2u, " + pp::tup(args);
          };

          def<"\\10(...: size, obj)">{} = [](va args) {
            return "2u, " + pp::tup(args);
          };

          def<"\\11(size, tup)">{} = [](arg size, arg tup) {
            return size + ", " + tup;
          };

          // TODO
          return pp::call(
              xcat(utl::slice(_00, -2), xcat(is_size(obj_a), detail::is_tup_o(obj_b))),
              obj_a, obj_b);
        };

        return pp::call(xcat(utl::slice(_00, -2), xcat(is_obj(first), is_obj(rest))),
                        first, rest);
      };

      def<"\\1(obj)">{} = [](arg obj) {
        def<"\\0(obj)"> _0 = [](arg obj) {
          return "1u, " + pp::tup(obj);
        };

        def<"\\1(arr)">{} = [](arg arr_) {
          def items_arr = def{"\\" + arr + "(...)"} = [&](va args) {
            return args;
          };
          def{"\\" + fwd::map + "(...)"} = [&](va args) {
            return args;
          };
          def{"\\" + fwd::set + "(...)"} = [&](va args) {
            return args;
          };
          def{"\\" + fwd::stack + "(...)"} = [&](va args) {
            return args;
          };
          def{"\\" + fwd::queue + "(...)"} = [&](va args) {
            return args;
          };
          def{"\\" + fwd::pqueue + "(...)"} = [&](va args) {
            return args;
          };

          return pp::cat(utl::slice(items_arr, -((std::string const&)arr).size()), arr_);
        };

        return pp::call(xcat(utl::slice(_0, -1), detail::is_arr_o(obj)), obj);
      };

      return pp::call(xcat(utl::slice(_0, -1), is_obj(args)), args);
    };

    def<"\\1(...)">{} = [](va) {
      return "0u, " + pp::tup();
    };

    return pp::call(xcat(utl::slice(_0, -1), is_none(args)), args);
  };

  return arr(detail::arr_o(args));
});

} // namespace api

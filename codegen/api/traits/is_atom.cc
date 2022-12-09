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
decltype(is_atom_o) is_atom_o = NIFTY_DEF(is_atom_o);
} // namespace detail

decltype(is_atom) is_atom = NIFTY_DEF(is_atom, [&](va args) {
  docs << "[extends is_any] matches non-empty sequences of pp-concatable tokens."
       << "pp-concatability implies that atom##atom is valid."
       << ""
       << "tries to return true if the inputs are any, non-none,"
       << "non-tup, and non-obj. asserts concatability by attempting"
       << "atom##atom concatenation (which may trigger a preprocessor error)."
       << ""
       << "invalid concatenations trigger a primitive error message (atom-based, not"
       << "string-based like lang.fail) that contains the text "
              + std::string{conf::api_prefix} + "FAIL_INVALID_ATOM.";

  tests << is_atom()                 = "0" >> docs;
  tests << is_atom("foo")            = "1" >> docs;
  tests << is_atom("0")              = "1" >> docs;
  tests << is_atom("1, 2")           = "0" >> docs;
  tests << is_atom("()")             = "0" >> docs;
  tests << is_atom("(1, 2)")         = "0" >> docs;
  tests << is_atom(fwd::obj + "()")  = "0" >> docs;
  tests << is_atom("(), ()")         = "0";
  tests << is_atom(esc + "(())")     = "0";
  tests << is_atom(esc + "((1, 2))") = "0";
  tests << is_atom(", ")             = "0";
  tests << is_atom(", , ")           = "0";
  tests << is_atom("a, ")            = "0";
  tests << is_atom("a, , ")          = "0";
  tests << is_atom(", a")            = "0";
  tests << is_atom(", a, ")          = "0";
  tests << is_atom(", , a")          = "0";
  tests << is_atom("(, )")           = "0";
  tests << is_atom("(, , )")         = "0";
  tests << is_atom("(a, )")          = "0";
  tests << is_atom("(a, , )")        = "0";
  tests << is_atom("(, a)")          = "0";
  tests << is_atom("(, a, )")        = "0";
  tests << is_atom("(, , a)")        = "0";

  def<> fail_oo;

  def<"fail(...)"> fail = [&](va) {
    return def<"o(...)">{[&](va) {
      fail_oo = def{"<o(...)"} = [&](va) {
        return def{"<o(...)"} = [&](va) {
          return "0";
        };
      };
      return fail_oo;
    }};
  };

  def<"res(...)"> res = [&](va) {
    return "1";
  };

  detail::is_atom_o =
      def{"o(...: any) -> (any) -> (any) -> (any) -> bool"} = [&](va any) {
        def<"<o(any: <non-none, non-tup>)"> o = [&](arg any_non_tup) {
          def<"\\0(atom)"> _0 = [&](arg atom) {
            return res( //
                pp::cat(std::string{conf::api_prefix} + "FAIL_INVALID_ATOM__", atom,
                        "__" + std::string{conf::api_prefix} + "FAIL_INVALID_ATOM"));
          };
          def<"\\1(...)">{} = [](va) {
            return "0";
          };
          return xcat(utl::slice(_0, -1), impl::is_obj(any_non_tup));
        };

        // <is-none>
        def<"\\0(...)"> _0 = [&] {
          return fail_oo;
        };
        // <is-not-none> <is-not-tup>
        def<"\\10(...)">{} = [&] {
          return o;
        };
        // <is-not-none> <is-tup>
        def<"\\11(...)">{} = [&] {
          return fail_oo;
        };
        return xcat(pp::cat(utl::slice(_0, -1), pp::va_opt(1)), is_none(eat + " " + any));
      };

  def<"\\0"> _0 = [&] {
    return fail;
  };

  def<"\\1">{} = [&] {
    return detail::is_atom_o;
  };

  return pp::call(
      pp::call(pp::call(pp::call(xcat(utl::slice(_0, -1), is_any(args)), args), args),
               args),
      args);
});

} // namespace api

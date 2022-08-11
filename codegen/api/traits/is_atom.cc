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
}

decltype(is_atom) is_atom = NIFTY_DEF(is_atom, [&](va args) {
  docs << "[extends " + is_any + "] detects if args is a generic, non-tuple, singular value.";

  tests << is_atom()                 = "0" >> docs;
  tests << is_atom("foo")            = "1" >> docs;
  tests << is_atom("0")              = "1" >> docs;
  tests << is_atom("1, 2")           = "0" >> docs;
  tests << is_atom("()")             = "0" >> docs;
  tests << is_atom("(1, 2)")         = "0" >> docs;
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

  detail::is_atom_o = def{"o(any)"} = [&](arg any) {
    def<"0"> _0 = [] {
      return "1";
    };

    def<"1">{} = [] {
      return "0";
    };

    return cat(utl::slice(_0, -1), is_tup(any));
  };

  def<"0"> _0 = [&] {
    return def<"fail(...)">{[&](va) {
      return "0";
    }};
  };

  def<"1">{} = [&] {
    return detail::is_atom_o;
  };

  return pp::call(cat(utl::slice(_0, -1), is_any(args)), args);
});

} // namespace api

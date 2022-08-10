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

  def<"00"> _00 = [] {
    return "0";
  };

  def<"10">{} = [] {
    return "1";
  };

  def<"11">{} = [] {
    return "0";
  };

  return cat(utl::slice(_00, -2), cat(is_any(args), is_tup(args)));
});

} // namespace api

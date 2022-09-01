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

decltype(atom) atom = NIFTY_DEF(atom, [&](va args) {
  docs << "[inherits from " + obj
              + "] an individual value that may form an identifier tail."
       << ""
       << "this function only tests for tuples and multiple values."
       << ""
       << "while not testable, the true semantics of atom implies"
       << "that its values are able to concatenate with identifiers"
       << "to form new identifiers, meaning that is must match /[\\w\\d_]+/."
       << ""
       << "this property is critical for value-based control flow"
       << "and must be observed by the user where applicable.";

  tests << atom("foo") = "foo" >> docs;

  def<"\\0(e, ...)"> _0 = [](arg e, va) { return fail(e); };
  def<"\\1(e, atom)">{} = [](arg, arg atom) { return atom; };

  return def<"o(e, obj)">{[&](arg e, arg obj) {
    return pp::call(xcat(utl::slice(_0, -1), detail::is_atom_o(obj)), e, obj);
  }}(str(pp::str("[" + atom + "] atom cannot describe tuples") + " : " + args),
     obj(args));
});

} // namespace api

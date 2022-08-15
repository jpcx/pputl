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
  docs << "[inherits from " + any + "] a generic, non-tuple, singular value.";

  tests << atom("foo") = "foo" >> docs;

  def<"0(e, ...)"> _0 = [](arg e, va) { return fail(e); };
  def<"1(e, atom)">{} = [](arg, arg atom) { return atom; };

  return def<"o(e, any)">{[&](arg e, arg any) {
    return pp::call(cat(utl::slice(_0, -1), detail::is_atom_o(any)), e, any);
  }}(istr("[" + atom + "] atom cannot describe tuples : " + args), any(args));
});

} // namespace api

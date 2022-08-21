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

decltype(any) any = NIFTY_DEF(any, [&](va args) {
  docs << "[inherits from " + some + "] any type (generic data). returns arg."
       << "describes exactly one generic value."
       << ""
       << "fails if not exactly one arg.";

  tests << any("foo") = "foo" >> docs;

  def<"\\0(e, ...)"> _0 = [](arg e, va) { return fail(e); };
  def<"\\1(e, any)">{}  = [](arg, arg any) { return any; };

  return def<"o(e, ...)">{[&](arg e, va some) {
    return pp::call(cat(utl::slice(_0, -1), detail::is_any_o(some + ".")), e, some);
  }}(str("[" + any + "] any cannot describe multiple args : " + args), some(args));
});

} // namespace api

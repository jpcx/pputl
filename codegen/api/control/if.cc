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

#include "control.h"

namespace api {

using namespace codegen;

decltype(if_) if_ = NIFTY_DEF(if_, [&](va args) {
  docs << "conditionally expands based on a boolean.";

  tests << if_(1, "t,")   = "t" >> docs;
  tests << if_(0, "t,")   = "" >> docs;
  tests << if_(1, "t, f") = "t" >> docs;
  tests << if_(0, "t, f") = "f" >> docs;

  def<"\\0(_, t, ...)"> _0 = [&](arg, arg, va f) {
    return any(f);
  };
  def<"\\1(_, t, ...)">{} = [&](arg, arg t, va) {
    return t;
  };

  return pp::call(xcat(utl::slice(_0, -1), bool_(first(args))), args);
});

} // namespace api

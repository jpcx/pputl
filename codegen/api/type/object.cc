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

decltype(object) object = NIFTY_DEF(object, [&](va args) {
  docs << "[extends list] a non-empty list without separators (exactly one thing).";

  tests << object()        = "0" >> docs;
  tests << object("foo")   = "foo" >> docs;
  tests << object(".")     = "." >> docs;
  tests << object("()")    = "()" >> docs;
  tests << object("foo()") = "foo()" >> docs;

  def<"\\0(e, ...)"> _0 = [](arg e, va) {
    return fail(e);
  };
  def<"\\1(e, obj)">{} = [](arg, arg obj) {
    return obj;
  };

  return def<"o(e, ...)">{[&](arg e, va args) {
    return pp::call(xcat(utl::slice(_0, -1), is_object(args)), e, args);
  }}(error(object, "must be exactly one thing", args), default_(0, args));
});

} // namespace api

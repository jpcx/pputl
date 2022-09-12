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

decltype(map) map = NIFTY_DEF(map, [&](va args) {
  docs << "[inherits from " + arr + "] a sorted mapping of words to elements."
       << "overloads: map(arr), map(size, tup), map(tup...)."
       << ""
       << "elements are stored as key/value pairs in sorted order; construction from"
       << "list or tuple assumes the inputs are valid (word, any) pairs in sorted order."
       << ""
       << "resultant object is an expansion-terminated self-reference."
       << ""
       << "note: constructor never fails. construction from existing array type"
       << "      assumes that the contained structure is valid, construction from"
       << "      explicit size assumes size is valid, and any inputs that do not"
       << "      match any other constructor fallback to list construction.";

  tests << map()      = map() >> docs;
  tests << map(map()) = map() >> docs;

  def<"\\0(e, ...)"> _0 = [](arg e, va) {
    return fail(e);
  };

  def<"\\1(e, map)">{} = [](arg, arg map) {
    return map;
  };

  return def<"o(e, obj)">{[&](arg e, arg obj) {
    return pp::call(xcat(utl::slice(_0, -1), detail::is_map_o(obj)), e, obj);
  }}(error(map, "invalid map", args), obj(default_(map(), args)));
});

} // namespace api

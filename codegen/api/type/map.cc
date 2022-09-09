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
  docs << "[inherits from " + obj + "] a mapping of sizes to lists."
       << "optionally provide arguments to make a type assertion."
       << ""
       << "note: does not parse contained items during validity check.";

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

////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////

#include "traits.h"

namespace api {

using namespace codegen;

decltype(is_sizey) is_sizey = NIFTY_DEF(is_sizey, [&](va) {
  docs << "detects if args is non-empty.";

  tests << is_sizey("")         = "0" >> docs;
  tests << is_sizey("foo")      = "1" >> docs;
  tests << is_sizey("foo, bar") = "1" >> docs;
  tests << is_sizey(esc())      = "0" >> docs;
  tests << is_sizey(", ")       = "1";
  tests << is_sizey(", , ")     = "1";
  tests << is_sizey("a, ")      = "1";
  tests << is_sizey("a, , ")    = "1";
  tests << is_sizey(", a")      = "1";
  tests << is_sizey(", a, ")    = "1";
  tests << is_sizey(", , a")    = "1";

  def<"o"> o = [&] {
    return "0";
  };

  def<"o1">{} = [&] {
    return "1";
  };

  return pp::cat(o, pp::va_opt("1"));
});

} // namespace api

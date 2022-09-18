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

decltype(is_none) is_none = NIFTY_DEF(is_none, [&](va) {
  docs << "[extends is_list] detects if args is nothing (an empty list).";

  tests << is_none("")         = "1" >> docs;
  tests << is_none("foo")      = "0" >> docs;
  tests << is_none("foo, bar") = "0" >> docs;
  tests << is_none(esc())      = "1" >> docs;
  tests << is_none(", ")       = "0";
  tests << is_none(", , ")     = "0";
  tests << is_none("a, ")      = "0";
  tests << is_none("a, , ")    = "0";
  tests << is_none(", a")      = "0";
  tests << is_none(", a, ")    = "0";
  tests << is_none(", , a")    = "0";

  def<"\\0"> _0 = [] {
    return "1";
  };
  def<"\\01">{} = [] {
    return "0";
  };

  return pp::cat(_0, pp::va_opt("1"));
});

} // namespace api

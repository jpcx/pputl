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

decltype(is_any) is_any = NIFTY_DEF(is_any, [&](va args) {
  docs << "[extends " + is_some + "] detects if args is exactly one generic value.";

  tests << is_any("")         = "0" >> docs;
  tests << is_any(",")        = "0" >> docs;
  tests << is_any("foo,")     = "0" >> docs;
  tests << is_any("foo, bar") = "0" >> docs;
  tests << is_any("foo")      = "1" >> docs;
  tests << is_any("(42)")     = "1" >> docs;

  def<"00"> _00 = [&] {
    return "0";
  };

  def<"01">{} = [&] {
    return "0";
  };

  def<"10">{} = [&] {
    return "1";
  };

  def<"11">{} = [&] {
    return "0";
  };

  return def<"o(_, ...)">{[&](arg first, va args) {
    return cat(utl::slice(_00, -2), cat(is_some(first), is_some(args)));
  }}(args + " " + pp::va_opt("."));
});

} // namespace api

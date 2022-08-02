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

#include "lang.h"

namespace api {

using namespace codegen;

decltype(str) str = NIFTY_DEF(str, [&](va args) {
  docs << "stringizes args after an expansion.";

  tests << str()           = "\"\"" >> docs;
  tests << str("foo")      = "\"foo\"";
  tests << str("foo, bar") = "\"foo, bar\"" >> docs;
  tests << str(", ")       = "\",\"";
  tests << str(", , ")     = "\", ,\"";
  tests << str("a, ")      = "\"a,\"";
  tests << str("a, , ")    = "\"a, ,\"";
  tests << str(", a")      = "\", a\"";
  tests << str(", a, ")    = "\", a,\"";
  tests << str(", , a")    = "\", , a\"";

  return def<"x(...)">{[&](va args) {
    return "#" + args;
  }}(args);
});

} // namespace api
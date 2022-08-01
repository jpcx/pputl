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

#include "va_rest.h"

namespace api {

using namespace codegen;

va_rest::va_rest() {
  def& va_rest = *this;
  using codegen::def;
  va_rest = [&](arg, va args) {
    category = "lang";
    docs << "immediately returns all args except the first from __VA_ARGS__."
         << "must be used with __VA_ARGS__; cannot be used with function invocation results."
         << "" << va_rest("__VA_ARGS__") + " // ...__VA_ARGS__[1:]";

    tests << va_rest("")         = "";
    tests << va_rest(", ")       = "";
    tests << va_rest("a")        = "";
    tests << va_rest("a, b")     = "b";
    tests << va_rest("a, b, c")  = "b, c";
    tests << va_rest("a, , ")    = ",";
    tests << va_rest("a, b, , ") = "b, ,";

    return args;
  };
}

} // namespace api

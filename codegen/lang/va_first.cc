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

#include "va_first.h"

namespace api {

using namespace codegen;

va_first::va_first() {
  def& va_first = *this;
  using codegen::def;
  va_first = [&](arg _, va) {
    category = "lang";
    docs << "immediately returns the first argument from __VA_ARGS__."
         << "must be used with __VA_ARGS__; cannot be used with function invocation results."
         << "" << va_first("__VA_ARGS__") + " // __VA_ARGS__[0]";

    tests << va_first("")     = "";
    tests << va_first(", ")   = "";
    tests << va_first("a")    = "a";
    tests << va_first("a, b") = "a";

    return _;
  };
}

} // namespace api

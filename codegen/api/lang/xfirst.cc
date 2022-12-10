#ifndef PPUTL_CODEGEN_LANG_XFIRST
#define PPUTL_CODEGEN_LANG_XFIRST
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
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//                                                                            //
//  You should have received a copy of the GNU General Public License        ///
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.  ////
///////////////////////////////////////////////////////////////////////////// */

#include "codegen.h"

#include "lang/first.cc"

namespace codegen {
namespace api {

using namespace std;

inline def<"xfirst(...: any...) -> any"> xfirst = [](va args) {
  category = "lang";

  docs << "returns the first argument.";

  tests << xfirst("")     = "" >> docs;
  tests << xfirst(", ")   = "" >> docs;
  tests << xfirst("a")    = "a" >> docs;
  tests << xfirst("a, b") = "a" >> docs;

  return pp::va_opt(first(args));
};

} // namespace api
} // namespace codegen

#endif

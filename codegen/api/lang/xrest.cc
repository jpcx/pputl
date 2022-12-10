#ifndef CODEGEN_API_LANG_XREST_CC
#define CODEGEN_API_LANG_XREST_CC
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

#include "lang/rest.cc"

namespace codegen {
namespace api {

using namespace std;

inline def<"xrest(...: any...) -> any..."> xrest = [](va args) {
  category = "lang";

  docs << "returns all arguments except for the first after an expansion.";

  tests << xrest("")               = "" >> docs;
  tests << xrest(", ")             = "" >> docs;
  tests << xrest("a")              = "" >> docs;
  tests << xrest("a, b")           = "b" >> docs;
  tests << xrest("a, b, c")        = "b, c" >> docs;
  tests << xrest(xrest("a, b, c")) = "c" >> docs;
  tests << xrest("a, , ")          = "," >> docs;
  tests << xrest("a, b, , ")       = "b, ," >> docs;

  return pp::va_opt(rest(args));
};

} // namespace api
} // namespace codegen

#endif

#ifndef PPUTL_CODEGEN_LANG_XCAT
#define PPUTL_CODEGEN_LANG_XCAT
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

#include "lang/cat.cc"
#include "lang/eat.cc"

namespace codegen {
namespace api {

using namespace std;

inline def<"xcat(...: a: any, b: any) -> any"> xcat = [](va args) {
  category = "lang";

  docs << "concatenates two args after an expansion."
       << "args must be compatible with the ## operator.";

  tests << xcat("foo", "bar")      = "foobar" >> docs;
  tests << xcat("foo", eat("bar")) = "foo" >> docs;
  tests << xcat(",")               = "" >> docs;

  return cat(args);
};

} // namespace api
} // namespace codegen

#endif

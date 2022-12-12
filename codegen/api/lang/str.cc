#ifndef CODEGEN_API_LANG_STR_CC
#define CODEGEN_API_LANG_STR_CC
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

namespace codegen {
namespace api {

namespace str_ {

using namespace std;

inline def<"str(...: any...) -> some"> self = [](va args) {
  category = "lang";

  docs << "stringizes args.";

  tests << self()                  = "\"\"" >> docs;
  tests << self("foo")             = "\"foo\"";
  tests << self("foo, bar")        = "\"foo, bar\"" >> docs;
  tests << self(cat("foo", "bar")) = pp::str("foobar") >> docs;
  tests << self(", ")              = "\",\"";
  tests << self(", , ")            = "\", ,\"";
  tests << self("a, ")             = "\"a,\"";
  tests << self("a, , ")           = "\"a, ,\"";
  tests << self(", a")             = "\", a\"";
  tests << self(", a, ")           = "\", a,\"";
  tests << self(", , a")           = "\", , a\"";

  return def<"o(...)">{[](va args) {
    return "#" + args;
  }}(args);
};

} // namespace str_

inline constexpr auto& str = str_::self;

} // namespace api
} // namespace codegen

#endif

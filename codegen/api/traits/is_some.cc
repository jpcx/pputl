#ifndef CODEGEN_API_TRAITS_IS_SOME_CC
#define CODEGEN_API_TRAITS_IS_SOME_CC
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

#include "lang/esc.cc"

namespace codegen {
namespace api {

namespace is_some_ {

using namespace std;

inline def<"is_some(...: any...) -> bool"> self = [](va args) {
  category = "traits";

  docs << "[extends is_any] checks if args is a singluar value";

  tests << self("")         = "false" >> docs;
  tests << self("foo")      = "true" >> docs;
  tests << self("()")       = "true" >> docs;
  tests << self("(a, b)")   = "true" >> docs;
  tests << self("foo, bar") = "false" >> docs;
  tests << self(esc())      = "false" >> docs;
  tests << self(", ")       = "false";
  tests << self(", , ")     = "false";
  tests << self("a, ")      = "false";
  tests << self("a, , ")    = "false";
  tests << self(", a")      = "false";
  tests << self(", a, ")    = "false";
  tests << self(", , a")    = "false";

  def<"\\0(_, ...)"> _0 = [](arg, va) {
    return "false";
  };

  def<"\\01(_, ...)">{} = [](arg, va) {
    def<"\\0"> _0 = [&] {
      return "true";
    };
    def<"\\01">{} = [&] {
      return "false";
    };
    return pp::cat(_0, pp::va_opt(1));
  };

  return pp::call(pp::cat(_0, pp::va_opt("1")), args + ".");
};

} // namespace is_some_

inline constexpr auto& is_some = is_some_::self;

} // namespace api
} // namespace codegen

#endif

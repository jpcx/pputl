#ifndef CODEGEN_API_TRAITS_IS_NONE_CC
#define CODEGEN_API_TRAITS_IS_NONE_CC
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

namespace is_none_ {

using namespace std;

inline def<"is_none(...: any...) -> bool"> self = [](va) {
  category = "traits";

  docs << "[extends is_any] checks if args is the literal nothing";

  tests << self("")         = "true" >> docs;
  tests << self("foo")      = "false" >> docs;
  tests << self("foo, bar") = "false" >> docs;
  tests << self(esc())      = "true" >> docs;
  tests << self(", ")       = "false";
  tests << self(", , ")     = "false";
  tests << self("a, ")      = "false";
  tests << self("a, , ")    = "false";
  tests << self(", a")      = "false";
  tests << self(", a, ")    = "false";
  tests << self(", , a")    = "false";

  def<"\\0"> _0 = [] {
    return "true";
  };
  def<"\\01">{} = [] {
    return "false";
  };

  return pp::cat(_0, pp::va_opt("1"));
};

} // namespace is_none_

inline constexpr auto& is_none = is_none_::self;

} // namespace api
} // namespace codegen

#endif

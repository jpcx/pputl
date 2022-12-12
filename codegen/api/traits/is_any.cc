#ifndef CODEGEN_API_TRAITS_IS_ANY_CC
#define CODEGEN_API_TRAITS_IS_ANY_CC
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

namespace codegen {
namespace api {

namespace is_any_ {

using namespace std;

inline def<"is_any(...: any...) -> bool"> self = [](va args) {
  category = "traits";

  docs << "checks if args is a single, potentially empty arg";

  tests << self()         = "1" >> docs;
  tests << self("foo")    = "1" >> docs;
  tests << self("(a, b)") = "1" >> docs;
  tests << self("a, b")   = "0" >> docs;
  tests << self(", ")     = "0" >> docs;
  tests << self(", , ")   = "0" >> docs;
  tests << self("a, ")    = "0";
  tests << self("a, , ")  = "0";
  tests << self(", a")    = "0";
  tests << self(", a, ")  = "0";
  tests << self(", , a")  = "0";

  return def<"o(_, ...)">{[&](arg, va) {
    def<"\\0"> _0 = [&] {
      return "1";
    };
    def<"\\01">{} = [&] {
      return "0";
    };
    return pp::cat(_0, pp::va_opt(1));
  }}(args + ".");
};

} // namespace is_any_

inline constexpr auto& is_any = is_any_::self;

} // namespace api
} // namespace codegen

#endif

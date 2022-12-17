#ifndef CODEGEN_API_TRAITS_IS_TUP_CC
#define CODEGEN_API_TRAITS_IS_TUP_CC
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
#include "lang/esc.cc"
#include "traits/is_none.cc"
#include "traits/is_some.cc"

namespace codegen {
namespace api {

namespace is_tup_ {

using namespace std;

inline def<"is_tup(...: any...) -> bool"> self = [](va args) {
  category = "traits";

  docs << "[extends is_some] checks if args is a tuple";

  tests << self()                 = "false" >> docs;
  tests << self("1, 2")           = "false" >> docs;
  tests << self("()")             = "true" >> docs;
  tests << self("(1, 2)")         = "true" >> docs;
  tests << self("(), ()")         = "false";
  tests << self(esc + "(())")     = "true";
  tests << self(esc + "((1, 2))") = "true";
  tests << self(", ")             = "false";
  tests << self(", , ")           = "false";
  tests << self("a, ")            = "false";
  tests << self("a, , ")          = "false";
  tests << self(", a")            = "false";
  tests << self(", a, ")          = "false";
  tests << self(", , a")          = "false";
  tests << self("(, )")           = "true";
  tests << self("(, , )")         = "true";
  tests << self("(a, )")          = "true";
  tests << self("(a, , )")        = "true";
  tests << self("(, a)")          = "true";
  tests << self("(, a, )")        = "true";
  tests << self("(, , a)")        = "true";

  def<"\\F"> f = [&] {
    return def<"fail(...)">{[&](va) {
      return "false";
    }};
  };

  def<"\\T">{} = [&] {
    return def<"o(obj)">{[&](arg obj) {
      return is_none(eat + " " + obj);
    }};
  };

  return pp::call(cat(utl::slice(f, -1), is_some(args)), args);
};

} // namespace is_tup_

inline constexpr auto& is_tup = is_tup_::self;

} // namespace api
} // namespace codegen

#endif

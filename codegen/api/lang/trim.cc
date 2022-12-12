#ifndef CODEGEN_API_LANG_TRIM_CC
#define CODEGEN_API_LANG_TRIM_CC
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
#include "lang/first.cc"
#include "lang/rest.cc"

namespace codegen {
namespace api {

namespace trim_ {

using namespace std;

inline def<"trim(...: any...) -> any..."> self = [](va args) {
  category = "lang";

  docs << "removes an unnecessary comma at the front of args"
       << "if either the first arg or the rest args are nothing.";

  tests << self("")        = "" >> docs;
  tests << self(", ")      = "" >> docs;
  tests << self("a")       = "a" >> docs;
  tests << self("a, b")    = "a, b" >> docs;
  tests << self("a, b, c") = "a, b, c";
  tests << self("a, ")     = "a" >> docs;
  tests << self(", b")     = "b";
  tests << self(", b, c")  = "b, c" >> docs;
  tests << self("a, b, ")  = "a, b," >> docs;
  tests << self("a, , c")  = "a,  , c";

  def<"\\00(...)"> _00 = [&](va) {
    return "";
  };
  def<"\\001(_, ...)">{} = [&](arg, va args) {
    return args;
  };
  def<"\\010(_, ...)">{} = [&](arg _, va) {
    return _;
  };
  def<"\\0101(_, ...)">{} = [&](arg _, va args) {
    return _ + ", " + args;
  };

  def<"sel(...)"> sel = [&](va) {
    return pp::cat(0, pp::va_opt(1));
  };

  return pp::call(cat(utl::slice(_00, -2), cat(sel(first(args)), sel(rest(args)))), args);
};

} // namespace trim_

inline constexpr auto& trim = trim_::self;

} // namespace api
} // namespace codegen

#endif

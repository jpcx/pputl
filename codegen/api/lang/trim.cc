#ifndef PPUTL_CODEGEN_LANG_TRIM
#define PPUTL_CODEGEN_LANG_TRIM
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

#include "lang/xcat.cc"
#include "lang/xfirst.cc"
#include "lang/xrest.cc"

namespace codegen {
namespace api {

using namespace std;

inline def<"trim(...: any...) -> any..."> trim = [](va args) {
  category = "lang";

  docs << "removes an unnecessary comma at the front of args"
       << "if either the first arg or the rest args are nothing.";

  tests << trim("")        = "" >> docs;
  tests << trim(", ")      = "" >> docs;
  tests << trim("a")       = "a" >> docs;
  tests << trim("a, b")    = "a, b" >> docs;
  tests << trim("a, b, c") = "a, b, c";
  tests << trim("a, ")     = "a" >> docs;
  tests << trim(", b")     = "b";
  tests << trim(", b, c")  = "b, c" >> docs;
  tests << trim("a, b, ")  = "a, b," >> docs;
  tests << trim("a, , c")  = "a,  , c";

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

  return pp::call(xcat(utl::slice(_00, -2), xcat(sel(xfirst(args)), sel(xrest(args)))),
                  args);
};

} // namespace api
} // namespace codegen

#endif

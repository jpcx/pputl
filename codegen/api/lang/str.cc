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

using namespace std;

inline def<"str(...: any...) -> obj"> str = [](va args) {
  category = "lang";

  docs << "immediately stringizes args.";

  tests << str()                  = "\"\"" >> docs;
  tests << str("foo")             = "\"foo\"";
  tests << str("foo, bar")        = "\"foo, bar\"" >> docs;
  tests << str(cat("foo", "bar")) = pp::str(cat("foo", "bar")) >> docs;
  tests << str(", ")              = "\",\"";
  tests << str(", , ")            = "\", ,\"";
  tests << str("a, ")             = "\"a,\"";
  tests << str("a, , ")           = "\"a, ,\"";
  tests << str(", a")             = "\", a\"";
  tests << str(", a, ")           = "\", a,\"";
  tests << str(", , a")           = "\", , a\"";

  return "#" + args;
};

// creates an error string for possible failure
// see type.any for a simple usage example
[[nodiscard]] inline string
error(def_base const& targ, string const& msg, va args) {
  return str(targ(args) + " -> <" + msg + ">");
}

} // namespace api
} // namespace codegen

#endif

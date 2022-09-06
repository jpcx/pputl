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
//	   This program is free software: you can redistribute it and/or modify
//	   it under the terms of the GNU General Public License as published by
//	   the Free Software Foundation, either version 3 of the License, or
//	   (at your option) any later version.
//
//	   This program is distributed in the hope that it will be useful,
//	   but WITHOUT ANY WARRANTY; without even the implied warranty of
//	   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	   GNU General Public License for more details.
//                                                                            //
//  You should have received a copy of the GNU General Public License        ///
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.  ////
///////////////////////////////////////////////////////////////////////////// */

#include "lang.h"

namespace api {

using namespace codegen;

decltype(default_) default_ = NIFTY_DEF(default_, [&](va args) {
  docs << "returns the first argument iff the rest of the arguments are nothing."
       << "else, returns only the rest of the arguments.";

  tests << default_("a")       = "a" >> docs;
  tests << default_("a,")      = "a" >> docs;
  tests << default_("a, b")    = "b" >> docs;
  tests << default_("a, b, c") = "b, c" >> docs;

  def<"\\0(_, ...)"> _0 = [&](arg first, va) { return first; };
  def<"\\01(_, ...)">{} = [&](arg, va args) { return args; };

  return def<"o(_, ...)">{[&](arg first, va rest) {
    return pp::call(pp::cat(_0, pp::va_opt("1")), first, rest);
  }}(args);
});

} // namespace api
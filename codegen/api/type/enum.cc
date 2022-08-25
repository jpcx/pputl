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

#include "type.h"

namespace api {

using namespace codegen;

decltype(enum_) enum_ = NIFTY_DEF(enum_, [&](va args) {
  docs << "[inherits from " + atom + "] enum type. describes an atom union."
       << ""
       << "to use this function, define a set of"
       << "macros with the following characteristics:"
       << " ‐ object-like"
       << " ‐ common prefix"
       << " ‐ enum value suffixes"
       << " ‐ expand to nothing"
       << "pass the common prefix as chkprefix."
       << ""
       << "example: (asserting an enum<GOOD|BAD>)"
       << " #define FOO_GOOD"
       << " #define FOO_BAD"
       << " " + enum_ + "(FOO_, BLEH) // <fail>"
       << " " + enum_ + "(FOO_, GOOD) // GOOD"
       << " " + enum_ + "(FOO_, ,,,)  // <fail>";

  def<"\\0(e, ...)"> _0 = [](arg e, va) { return fail(e); };
  def<"\\1(e, enum)">{} = [](arg, arg enum_) { return enum_; };

  return def<"o(e, chkatom, vatom)">{[&](arg e, arg chkatom, arg vatom) {
    return pp::call(xcat(utl::slice(_0, -1), detail::is_enum_o(chkatom, vatom)), e,
                    vatom);
  }}(str("[" + enum_ + "] enum validation failure : " + args), atom(first(args)),
     atom(rest(args)));
});

} // namespace api

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

decltype(bool_) bool_ = NIFTY_DEF(bool_, [&](va args) {
  docs << "[inherits from " + atom + "] bool type (0 or 1)."
       << "expands to b if valid, else fails.";

  tests << bool_(0) = "0" >> docs;
  tests << bool_(1) = "1" >> docs;

  def<"0(e, ...)"> _0 = [](arg e, va) { return fail(e); };
  def<"1(e, bool)">{} = [](arg, arg bool_) { return bool_; };

  return def<"o(e, atom)">{[&](arg e, arg atom) {
    return pp::call(cat(utl::slice(_0, -1), detail::is_bool_o(atom)), e, atom);
  }}(str("[" + bool_ + "] bool cannot describe anything but the literal '1' and '0' : " + args),
     atom(args));
});

} // namespace api

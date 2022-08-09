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

#include "control.h"

namespace api {

using namespace codegen;

decltype(if_) if_ = NIFTY_DEF(if_, [&](va args) {
  docs << "conditionally expands items based on a boolean.";

  tests << if_(1, "(t), ()")     = "t" >> docs;
  tests << if_(0, "(t), ()")     = "" >> docs;
  tests << if_(1, "(t), (f)")    = "t" >> docs;
  tests << if_(0, "(t), (f)")    = "f" >> docs;
  tests << if_(1, "(a), (b, c)") = "a" >> docs;
  tests << if_(0, "(a), (b, c)") = "b, c" >> docs;

  def<"0(_, t, f)"> _0 = [&](arg, arg t, arg f) {
    return rest(pp::tup(tuple(t)), items(f));
  };

  def<"1(_, t, f)">{} = [&](arg, arg t, arg f) {
    return rest(pp::tup(tuple(f)), items(t));
  };

  return pp::call(cat(utl::slice(_0, -1), bool_(ifirst(args))), args);
});

} // namespace api

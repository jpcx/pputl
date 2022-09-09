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

#include "logic.h"

namespace api {

using namespace codegen;

decltype(nor_) nor_ = NIFTY_DEF(nor_, [&](va args) {
  docs << "logical NOR.";

  tests << nor_("0, 0") = "1" >> docs;
  tests << nor_("0, 1") = "0" >> docs;
  tests << nor_("1, 0") = "0" >> docs;
  tests << nor_("1, 1") = "0" >> docs;

  def<"\\00"> _00 = [&] {
    return "1";
  };
  def<"\\01">{} = [&] {
    return "0";
  };
  def<"\\10">{} = [&] {
    return "0";
  };
  def<"\\11">{} = [&] {
    return "0";
  };

  return def<"x(a, b)">{[&](arg a, arg b) {
    return xcat(utl::slice(_00, -2), xcat(bool_(a), bool_(b)));
  }}(args);
});

} // namespace api

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

#include "traits.h"

namespace api {

using namespace codegen;

decltype(is_nybl) is_nybl = NIFTY_DEF(is_nybl, [&](va args) {
  docs << "[extends " + is_enum
              + "] detects if args is an enum<0000|0001|...|1110|1111>.";

  tests << is_nybl()       = "0" >> docs;
  tests << is_nybl(0)      = "0" >> docs;
  tests << is_nybl('B')    = "0" >> docs;
  tests << is_nybl("000")  = "0" >> docs;
  tests << is_nybl("0000") = "1" >> docs;
  tests << is_nybl("0110") = "1" >> docs;

  return detail::is_enum_o(impl::nybl_prefix, args);
});

} // namespace api

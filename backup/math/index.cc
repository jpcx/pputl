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

#include "math.h"

namespace api {

using namespace codegen;

decltype(index) index = NIFTY_DEF(index, [&](va args) {
  docs << "translates an ofs to a non-negative zero-offset for a given range size."
       << "positive indices return unchanged, negative indices return added to the size."
       << ""
       << "fails if input is out of bounds: [-size, size];"
       << "allows one-past-the-end indexing."
       << ""
       << "casts to typeof input size.";

  tests << index(5, 0)         = "0" >> docs;
  tests << index("5u", 1)      = "1u" >> docs;
  tests << index(5, 2)         = "2" >> docs;
  tests << index(5, 3)         = "3" >> docs;
  tests << index(5, 4)         = "4" >> docs;
  tests << index(5, 5)         = "5" >> docs;
  tests << index(5, neg(1))    = "4" >> docs;
  tests << index(5, neg(2))    = "3" >> docs;
  tests << index(5, neg(3))    = "2" >> docs;
  tests << index("5u", neg(4)) = "1u" >> docs;
  tests << index(5, neg(5))    = "0" >> docs;

  return def<"o(e, sz, i)">{[&](arg e, arg sz, arg i) {
    return offset(impl::index(uhex(i), is_int(i), uhex(sz), e), typeof(sz));
  }}(error(index, "index out of bounds", args), args);
});

} // namespace api

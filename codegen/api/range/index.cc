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

#include "range.h"

namespace api {

using namespace codegen;

decltype(index) index = NIFTY_DEF(index, [&](va args) {
  docs << "translates an idx to a non-negative zero-offset for a given range size."
       << "positive indices return unchanged, negative indices return added to the size."
       << ""
       << "fails if input is out of bounds: [-size, size];"
       << "allows one-past-the-end indexing.";

  tests << index(0, 5)      = "0" >> docs;
  tests << index(1, 5)      = "1" >> docs;
  tests << index(2, 5)      = "2" >> docs;
  tests << index(3, 5)      = "3" >> docs;
  tests << index(4, 5)      = "4" >> docs;
  tests << index(5, 5)      = "5" >> docs;
  tests << index(neg(1), 5) = ("0x" + utl::cat(samp::h4)) >> docs;
  tests << index(neg(2), 5) = ("0x" + utl::cat(samp::h3)) >> docs;
  tests << index(neg(3), 5) = ("0x" + utl::cat(samp::h2)) >> docs;
  tests << index(neg(4), 5) = ("0x" + utl::cat(samp::h1)) >> docs;
  tests << index(neg(5), 5) = ("0x" + utl::cat(samp::hmin)) >> docs;

  return def<"o(e, i, sz)">{[&](arg e, arg i, arg sz) {
    return idx(impl::index(utup(i), is_int(i), utup(sz), e), typeof(i));
  }}(error(index, "index out of bounds", args), args);
});

} // namespace api

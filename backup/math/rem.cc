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

#include "codegen.h"
#include "config.h"
#include "math.h"

namespace api {

using namespace codegen;

decltype(rem) rem = NIFTY_DEF(rem, [&](va args) {
  docs << "truncated division remainder."
       << "fails on division by zero."
       << "" << impl::arith_rules;

  tests << rem(10, 5)           = "0" >> docs;
  tests << rem(11, 5)           = "1" >> docs;
  tests << rem(12, 5)           = "2" >> docs;
  tests << rem(13, 5)           = "3" >> docs;
  tests << rem(14, 5)           = "4" >> docs;
  tests << rem(neg(10), 5)      = "0" >> docs;
  tests << rem(neg(11), 5)      = neg(1) >> docs;
  tests << rem(neg(12), 5)      = neg(2) >> docs;
  tests << rem(neg(13), 5)      = neg(3) >> docs;
  tests << rem(neg(14), 5)      = neg(4) >> docs;
  tests << rem(10, neg(5))      = "0" >> docs;
  tests << rem(11, neg(5))      = "1" >> docs;
  tests << rem(12, neg(5))      = "2" >> docs;
  tests << rem(13, neg(5))      = "3" >> docs;
  tests << rem(14, neg(5))      = "4" >> docs;
  tests << rem(neg(10), neg(5)) = ("0x" + utl::cat(samp::hmin)) >> docs;
  tests << rem(neg(11), neg(5)) = neg(1) >> docs;
  tests << rem(neg(12), neg(5)) = neg(2) >> docs;
  tests << rem(neg(13), neg(5)) = neg(3) >> docs;
  tests << rem(neg(14), neg(5)) = neg(4) >> docs;

  return xrest(divr(args));
});

} // namespace api

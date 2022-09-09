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

#include "numeric.h"

namespace api {

using namespace codegen;

decltype(inc) inc = NIFTY_DEF(inc, [&](va args) {
  docs << "numeric increment w/ overflow.";

  tests << inc(0)                            = "1" >> docs;
  tests << inc("1u")                         = "2u" >> docs;
  tests << inc(int_max_s)                    = int_min_s >> docs;
  tests << inc("0x" + utl::cat(samp::himax)) = (int_min_s) >> docs;

  if constexpr (conf::word_size > 1) {
    tests << inc("15u")      = "16u" >> docs;
    tests << inc(uint_max_s) = "0u" >> docs;
  } else {
    tests << inc("15u") = "0u" >> docs;
  }

  return word(impl::inc(utup(args)), typeof(args));
});

} // namespace api

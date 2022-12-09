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

decltype(dec) dec = NIFTY_DEF(dec, [&](va args) {
  docs << "numeric decrement w/ underflow.";

  tests << dec(1)         = "0" >> docs;
  tests << dec("2u")      = "1u" >> docs;
  tests << dec(0)         = ("0x" + utl::cat(samp::hmax)) >> docs;
  tests << dec(int_min_s) = ("0x" + utl::cat(samp::himax)) >> docs;

  if constexpr (conf::word_size > 1)
    tests << dec("16u") = "15u" >> docs;

  tests << dec("0u") = uint_max_s >> docs;

  return word(impl::dec(uhex(args)), typeof(args));
});

} // namespace api

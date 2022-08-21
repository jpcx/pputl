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

#include "bitwise.h"

namespace api {

using namespace codegen;

decltype(bnand) bnand = NIFTY_DEF(bnand, [&](va args) {
  docs << "bitwise NAND."
       << "uses arg 'a' for result cast hint.";

  tests << bnand(0, 0) = ("0x" + utl::cat(samp::hmax)) >> docs;
  tests << bnand(5, 7) =
      ("0x" + utl::cat(svect(conf::word_size - 1, "F")) + "A") >> docs;
  tests << bnand(pp::tup(samp::himax), pp::tup(samp::hmax)) =
      pp::tup(samp::himin) >> docs;

  return word(impl::uhex(uhex(band(args)), "BNOT"), typeof(first(args)));
});

} // namespace api

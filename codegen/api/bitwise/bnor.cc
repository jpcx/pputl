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

decltype(bnor) bnor = NIFTY_DEF(bnor, [&](va args) {
  docs << "bitwise NOR."
       << "uses arg 'a' for result cast hint.";

  tests << bnor(0, 0) = ("0x" + utl::cat(samp::hmax)) >> docs;
  tests << bnor(0, 1) = ("0x" + utl::cat(svect(conf::word_size - 1, "F")) + "E") >> docs;
  tests << bnor(5, 7) = ("0x" + utl::cat(svect(conf::word_size - 1, "F")) + "8") >> docs;
  if constexpr (conf::word_size > 1)
    tests << bnor(7, 8) =
        ("0x" + utl::cat(svect(conf::word_size - 1, "F")) + "0") >> docs;

  return word(impl::uhex(uhex(bor(args)), "BNOT"), typeof(va_first(args)));
});

} // namespace api

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

decltype(bflip) bflip = NIFTY_DEF(bflip, [&](va args) {
  docs << "flips the ith bit in the uint. indexed from least to most significant."
       << "fails on invalid bit index.";

  tests << bflip(0, 0)                         = "1" >> docs;
  tests << bflip("1u", 1)                      = "3u" >> docs;
  tests << bflip("0x" + utl::cat(samp::h2), 2) = ("0x" + utl::cat(samp::h6)) >> docs;
  tests << bflip("0x" + utl::cat(samp::h3) + "u", 3) =
      ("0x" + utl::cat(samp::h11) + "u") >> docs;

  return bset(args, not_(bget(args)));
});

} // namespace api

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

#include "compare.h"

namespace api {

using namespace codegen;

decltype(lt) lt = NIFTY_DEF(lt, [&](va args) {
  docs << "uint less-than comparison.";

  constexpr auto max = conf::uint_max;

  tests << lt("0, 0")           = "0" >> docs;
  tests << lt("0, 1")           = "1" >> docs;
  tests << lt("0, 2")           = "1";
  tests << lt("0, 3")           = "1";
  tests << lt("1, 0")           = "0" >> docs;
  tests << lt("1, 1")           = "0" >> docs;
  tests << lt("1, 2")           = "1";
  tests << lt("1, 3")           = "1";
  tests << lt("2, 0")           = "0";
  tests << lt("2, 1")           = "0";
  tests << lt("2, 2")           = "0";
  tests << lt("2, 3")           = "1";
  tests << lt("3, 0")           = "0";
  tests << lt("3, 1")           = "0";
  tests << lt("3, 2")           = "0";
  tests << lt("3, 3")           = "0";
  tests << lt(0, max)           = "1";
  tests << lt(0, max - 1)       = "1";
  tests << lt(1, max)           = "1";
  tests << lt(1, max - 1)       = "1";
  tests << lt(max, 0)           = "0";
  tests << lt(max, 0)           = "0";
  tests << lt(max - 1, 0)       = "0";
  tests << lt(max - 1, 0)       = "0";
  tests << lt(max, 1)           = "0";
  tests << lt(max, 1)           = "0";
  tests << lt(max - 1, 1)       = "0";
  tests << lt(max - 1, 1)       = "0";
  tests << lt(max, max)         = "0";
  tests << lt(max, max - 1)     = "0";
  tests << lt(max - 1, max)     = "1";
  tests << lt(max - 1, max - 1) = "0";
  for (int _ = 0; _ < 5; ++_) {
    int a             = std::rand() % (max + 1);
    int b             = std::rand() % (max + 1);
    tests << lt(a, b) = std::to_string(a < b);
  }

  return rest(detail::sub_impl(args));
});

} // namespace api

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

#include "meta.h"

namespace api {

using namespace codegen;

namespace detail {
decltype(xct_a) xct_a = NIFTY_DEF(xct_a, );
decltype(xct_b) xct_b = NIFTY_DEF(xct_b, );
} // namespace detail

std::string
xct_expected(unsigned n) {
  return (n % 2 == 0 ? detail::xct_a : detail::xct_b) + " ( "
       + utl::cat(std::vector<std::string>(n + 1, ",")) + " )";
}

decltype(xct) xct = NIFTY_DEF(xct, [&] {
  docs << "counts the number of expansions undergone after expression."
       << "uses recursion; can track any number of expansions."
       << "the number of commas indicates the number of expansions.";

  detail::xct_a = def{"a(...)"};
  detail::xct_b = def{"b(...)"};

  detail::xct_a = [&](va args) {
    return detail::xct_b + " " + lp() + " " + args + ", " + rp();
  };

  detail::xct_b = [&](va args) {
    return detail::xct_a + " " + lp() + " " + args + ", " + rp();
  };

  tests << str(xct)                = pp::str(xct_expected(0)) >> docs;
  tests << str(esc(xct))           = pp::str(xct_expected(1)) >> docs;
  tests << str(esc(esc(xct)))      = pp::str(xct_expected(2)) >> docs;
  tests << str(esc(esc(esc(xct)))) = pp::str(xct_expected(3)) >> docs;

  return detail::xct_a + " " + lp() + " /**/, " + rp();
});

} // namespace api

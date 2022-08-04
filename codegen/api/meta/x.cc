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

decltype(x) x = NIFTY_DEF(x, [&](va args) {
  docs << "performs uint n secondary expansions (n=0 -> identity)."
       << "args are expressed after n+1 expansions in total."
       << "useful for implementing mutual recursion.";

  tests << pp::call(x(0), xct)                            = esc(xct) >> docs;
  tests << pp::call(x(1), xct)                            = esc(esc(xct)) >> docs;
  tests << pp::call(x(0), xct)                            = xct_expected(0) >> docs;
  tests << pp::call(x(1), xct)                            = xct_expected(1) >> docs;
  tests << pp::call(x(2), xct)                            = xct_expected(2);
  tests << pp::call(x(3), xct)                            = xct_expected(3);
  tests << pp::call(x(4), xct)                            = xct_expected(4);
  tests << pp::call(x(conf::uint_max), xct)               = xct_expected(conf::uint_max);
  tests << xct_size(pp::call(x(conf::uint_max - 1), xct)) = uint_max_s;

  std::array<def<>, conf::uint_max + 1> _{};

  _[0] = def{"0(...)"} = [&](va args) {
    return args;
  };

  _[1] = def{"1(...)"} = [&](va args) {
    return _[0](args);
  };

  _[2] = def{"2(...)"} = [&](va args) {
    return _[0](_[0](args));
  };

  _[3] = def{"3(...)"} = [&](va args) {
    return _[0](_[0](_[0](args)));
  };

  {
    std::size_t i;
    for (i = 4; i < _.size() - 1; ++i) {
      _[i] = def{std::to_string(i) + "(...)"} = [&](va args) {
        return _[((i - 4) + 0) / 4](
            _[((i - 4) + 1) / 4](_[((i - 4) + 2) / 4](_[((i - 4) + 3) / 4](args))));
      };
    }
    _[i] = def{std::to_string(i) + "(...)"} = [&](va args) {
      return _[((i - 4) + 0) / 4](
          _[((i - 4) + 1) / 4](_[((i - 4) + 2) / 4](_[((i - 4) + 3) / 4](args))));
    };
  }

  return cat(utl::slice(_[0], -1), uint(args));
});

} // namespace api

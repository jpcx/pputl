////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////

#include "numeric.h"

namespace api {

using namespace codegen;

decltype(dec) dec = NIFTY_DEF(dec, [&](va args) {
  docs << "uint decrement w/ underflow.";

  tests << dec(0)                  = uint_max_s >> docs;
  tests << dec(1)                  = "0" >> docs;
  tests << dec(2)                  = "1";
  tests << dec(conf::uint_max)     = utl::to_string(conf::uint_max - 1) >> docs;
  tests << dec(conf::uint_max - 1) = utl::to_string(conf::uint_max - 2);

  std::array<def<>, conf::uint_max + 1> n{};

  n[0] = def{"n_0"} = [&] {
    return uint_max_s;
  };

  for (size_t i = 1; i < n.size(); ++i) {
    n[i] = def{"n_" + utl::to_string(i)} = [&] {
      return utl::to_string(i - 1);
    };
  }

  return def<"x(n)">{[&](arg n_) {
    return def<"x(n)">{[&](arg n_) {
      return pp::cat(utl::slice(n[0], -1), n_);
    }}(n_);
  }}(uint(args));
});

} // namespace api

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

decltype(inc) inc = NIFTY_DEF(inc, [&](va args) {
  docs << "uint increment w/ overflow.";

  tests << inc(0)                  = "1" >> docs;
  tests << inc(1)                  = "2" >> docs;
  tests << inc(2)                  = "3";
  tests << inc(conf::uint_max)     = "0" >> docs;
  tests << inc(conf::uint_max - 1) = uint_max_s;

  std::array<def<>, conf::uint_max + 1> n{};

  for (size_t i = 0; i < n.size() - 1; ++i) {
    n[i] = def{"n_" + utl::to_string(i)} = [&] {
      return utl::to_string(i + 1);
    };
  }

  n[conf::uint_max] = def{"n_" + uint_max_s} = [&] {
    return "0";
  };

  return def<"x(n)">{[&](arg n_) {
    return def<"x(n)">{[&](arg n_) {
      return pp::cat(utl::slice(n[0], -1), n_);
    }}(n_);
  }}(uint(args));
});

} // namespace api
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

#include "impl/math.h"

namespace api {
namespace impl {

using namespace codegen;

decltype(sub) sub = NIFTY_DEF(sub, [&](arg a, arg b) {
  docs << "[internal] subtraction with underflow.";

  constexpr auto sz = conf::word_size;

  auto p = "_, " + utl::cat(utl::alpha_base52_seq(sz * 2), ", ");

  def<"x(...)"> x = [&](va args) { return args; };

  def<"r(...)"> r = [&](va args) {
    def o = def{"o(" + p + ")"} = [&](pack v) {
      return utl::cat(
          std::array{
              hexhex(pp::cat(v[sz], v[sz * 2]), pp::cat("SUB", v[0])),
              utl::cat(svect{&v[1], &v[sz]}, ", "),
              v[sz * 2],
              utl::cat(svect{&v[sz + 1], &v[sz * 2]}, ", "),
          },
          ", ");
    };
    return o(args);
  };

  def<"res(...)"> res = [&](va args) {
    def o = def{"o(" + p + ")"} = [&](pack v) {
      return pp::tup(svect{&v[1], &v[sz + 1]});
    };

    return o(args);
  };

  auto rlp = utl::cat(svect{conf::word_size, r + "("});
  auto rrp = utl::cat(svect{conf::word_size, ")"});
  return res(rlp + "0, " + x(esc + " " + a, esc + " " + b) + rrp);
});

} // namespace impl
} // namespace api

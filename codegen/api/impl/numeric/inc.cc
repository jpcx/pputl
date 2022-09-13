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

#include "impl/numeric.h"

namespace api {
namespace impl {

using namespace codegen;

decltype(inc) inc = NIFTY_DEF(inc, [&](arg n) {
  docs << "[internal] numeric increment w/ overflow.";

  constexpr auto sz = conf::word_size;

  auto p = "_, " + utl::cat(utl::replace(utl::alpha_base52_seq(sz), {{"u", "_u"}}), ", ");

  def<"r(...)"> r = [&](va args) {
    def o = def{"o(" + p + ")"} = [&](pack v) {
      return utl::cat(
          std::array{
              impl::hex(v[sz], pp::cat("INC", v[0])),
              utl::cat(svect{&v[1], &v[sz]}, ", "),
          },
          ", ");
    };
    return o(args);
  };

  def<"res(...)"> res = [&](va args) {
    def o = def{"o(" + p + ")"} = [&](pack v) {
      return pp::cat("0x", pp::cat(svect{&v[1], &v[sz + 1]}), "u");
    };

    return o(args);
  };

  auto rlp = utl::cat(svect{conf::word_size, r + "("});
  auto rrp = utl::cat(svect{conf::word_size, ")"});
  return res(rlp + "1, " + uhex(n, "HDUMP") + rrp);
});

} // namespace impl
} // namespace api

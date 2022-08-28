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

#include "math.h"

namespace api {

using namespace codegen;

decltype(add) add = NIFTY_DEF(add, [&](va args) {
  docs << "addition with overflow."
       << ""
       << "returns unsigned if either operand is unsigned,"
       << "decimal if either operand is decimal, utup if"
       << "both operands are utup, and hex otherwise.";

  tests << add("0, 0")        = "0" >> docs;
  tests << add("0, 1")        = "1" >> docs;
  tests << add("1, 2")        = "3" >> docs;
  tests << add("3u, 4")       = "7u" >> docs;
  tests << add("5, 6u")       = "11u" >> docs;
  tests << add(uint_max_s, 1) = "0u" >> docs;
  tests << add(uint_max_s, 2) = "1u" >> docs;
  tests << add(1, uint_max_s) = "0u";
  tests << add(2, uint_max_s) = "1u";
  tests << add(uint_max_s, uint_max_s) =
      (std::to_string(conf::uint_max - 1) + "u") >> docs;
  tests << add(int_max_s, 1) = int_min_s >> docs;
  tests << add(int_max_s, pp::tup(samp::h1)) =
      (std::to_string(conf::int_max + 1) + "u") >> docs;

  constexpr auto sz = conf::word_size;

  auto p = "_, " + utl::cat(utl::alpha_base52_seq(sz * 2), ", ");

  def<"x(...)"> x = [&](va args) { return args; };

  def<"r(...)"> r = [&](va args) {
    def o = def{"o(" + p + ")"} = [&](pack v) {
      return utl::cat(
          std::array{
              impl::hexhex(pp::cat(v[sz], v[sz * 2]), pp::cat("ADD", v[0])),
              utl::cat(svect{&v[1], &v[sz]}, ", "),
              v[sz * 2],
              utl::cat(svect{&v[sz + 1], &v[sz * 2]}, ", "),
          },
          ", ");
    };
    return o(args);
  };

  def<"res(ta, tb, ...)"> res = [&](arg ta, arg tb, va args) {
    def o = def{"o(" + p + ")"} = [&](pack v) {
      return pp::tup(svect{&v[1], &v[sz + 1]});
    };

    return word(o(args), impl::arithhint(ta, tb));
  };

  return def<"o(a, b)">{[&](arg a, arg b) {
    auto rlp = utl::cat(svect{conf::word_size, r + "("});
    auto rrp = utl::cat(svect{conf::word_size, ")"});
    return res(typeof(a), typeof(b),
               rlp + "0, " + x(esc + " " + utup(a), esc + " " + utup(b)) + rrp);
  }}(args);
});

} // namespace api

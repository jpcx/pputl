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

decltype(bget) bget = NIFTY_DEF(bget, [&](va args) {
  docs << "gets the ith bit from the word, indexed from least to most significant."
       << "fails on invalid bit index.";

  auto maxless1 =
      "0x" + utl::cat(std::vector<std::string>(conf::word_size - 1, "F")) + "E";

  tests << bget(2, 2)                   = "0" >> docs;
  tests << bget(2, 1)                   = "1" >> docs;
  tests << bget(2, 0)                   = "0" >> docs;
  tests << bget("5u", 2)                = "1" >> docs;
  tests << bget(maxless1, 1)            = "1" >> docs;
  tests << bget(maxless1 + "u", 0)      = "0" >> docs;
  tests << bget(pp::tup(samp::hmax), 0) = "1" >> docs;
  tests << bget(int_min_s, neg(1))      = "1" >> docs;

  auto bitparams = utl::cat(utl::alpha_base52_seq(conf::bit_length), ", ");

  def _0 = def{"0(" + bitparams + ")"} = [&](pack args) {
    return args.back();
  };

  for (std::size_t i = 1; i < conf::bit_length; ++i) {
    def{"" + std::to_string(i) + "(" + bitparams + ")"} = [&](pack args) {
      return args[conf::bit_length - 1 - i];
    };
  }

  return def<"o(e, v, i)">{[&](arg e, arg v, arg i) {
    return def<"<o(i, ...)">{[&](arg i, va bin) {
      return pp::call(cat(utl::slice(_0, -1), i), bin);
    }}(idec(impl::index(uhex(i), is_int(i), impl::bitlen, e)), bdump(v));
  }}(str(pp::str("[" + bget + "] invalid index") + " : " + args), args);
});

} // namespace api

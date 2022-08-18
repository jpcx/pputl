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

#include <span>

#include "bitwise.h"

namespace api {

using namespace codegen;

decltype(bitrotr) bitrotr = NIFTY_DEF(bitrotr, [&](va args) {
  docs << "bitwise right rotation by n places.";

  tests << bitrotr("0x" + utl::cat(samp::hmin), 0) =
      ("0x" + utl::cat(samp::hmin)) >> docs;
  tests << bitrotr("0x" + utl::cat(samp::h1), 0) = ("0x" + utl::cat(samp::h1)) >> docs;
  tests << bitrotr("0x" + utl::cat(samp::h1), 1) = ("0x" + utl::cat(samp::himin)) >> docs;
  tests << bitrotr("0x" + utl::cat(samp::h2), 1) = ("0x" + utl::cat(samp::h1)) >> docs;
  tests << bitrotr("0x" + utl::cat(samp::himax), 2) =
      ("0xD" + utl::cat(svect(conf::word_size - 1, "F"))) >> docs;

  auto params{utl::cat(utl::alpha_base52_seq(conf::bit_length), ", ")};

  def bin = def{"bin(" + utl::cat(utl::alpha_base52_seq(conf::word_size), ", ")
                + ")"} = [&](pack args) {
    std::vector<std::string> res{};
    std::ranges::transform(args, std::back_inserter(res),
                           [&](auto&& v) { return impl::hex(v, "BITS"); });
    return utl::cat(res, ", ");
  };

  def _0 = def{"0(" + params + ")"} = [&](pack args) {
    std::array<std::string, conf::word_size> res{};
    for (std::size_t i = 0; i < res.size(); ++i)
      res[i] = impl::nybl(pp::cat(args[(i * 4) + 0], args[(i * 4) + 1], args[(i * 4) + 2],
                                  args[(i * 4) + 3]),
                          "HEX");

    return pp::tup(res);
  };

  for (std::size_t i = 1; i < utl::next_ge_pow2(conf::bit_length); ++i) {
    def{"" + std::to_string(i) + "(" + params + ")"} = [&](pack args) {
      svect rot{args};
      std::rotate(rot.rbegin(), rot.rbegin() + i % rot.size(), rot.rend());
      std::array<std::string, conf::word_size> res{};
      for (std::size_t j = 0; j < res.size(); ++j)
        res[j] = impl::nybl(pp::cat(rot[(j * 4) + 0], rot[(j * 4) + 1], rot[(j * 4) + 2],
                                    rot[(j * 4) + 3]),
                            "HEX");

      return pp::tup(res);
    };
  }

  return def<"o(v, n)">{[&](arg v, arg n) {
    return word(
        def<"<o(n, ...)">{[&](arg n, va bin) {
          return pp::call(
              cat(utl::slice(_0, -1),
                  bitand_(n, "0x" + utl::cat(svect(conf::word_size - 1, "0")) + "F")),
              bin);
        }}(n, esc(bin + " " + xword(v))),
        typeof(v));
  }}(args);
});

} // namespace api

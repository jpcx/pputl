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

decltype(bset) bset = NIFTY_DEF(bset, [&](va args) {
  docs << "sets the ith bit of the word to b, indexed from least to most significant."
       << "fails on invalid bit index.";

  auto zero   = std::vector<std::string>(conf::word_size, "0");
  auto two    = zero;
  two.back()  = "2";
  auto five   = zero;
  five.back() = "5";
  auto _21    = zero;
  if constexpr (conf::word_size > 1) {
    *(_21.rbegin() + 1) = "1";
    *(_21.rbegin() + 0) = "5";
  }

  tests << bset(0, 1, 1)    = "2" >> docs;
  tests << bset("1u", 2, 1) = "5u" >> docs;
  if constexpr (conf::word_size > 1)
    tests << bset(5, 4, 1) = "21" >> docs;
  tests << bset("0x" + utl::cat(samp::h2), 0, 1) = ("0x" + utl::cat(samp::h3)) >> docs;
  tests << bset("0x" + utl::cat(samp::h3) + "u", 0, 0) =
      ("0x" + utl::cat(samp::h2) + "u") >> docs;
  tests << bset(pp::tup(samp::hmax), neg(1), 0) = pp::tup(samp::himax) >> docs;

  std::vector<std::string> params{"_"};
  std::ranges::copy(utl::alpha_base52_seq(conf::bit_length), std::back_inserter(params));

  std::array<std::string, conf::word_size> res{};
  for (std::size_t i = 0; i < res.size(); ++i) {
    res[i] = impl::nybl(
        pp::cat(utl::alpha_base52_seq(4, std::string{static_cast<char>('a' + i * 4)})),
        "HEX");
  }

  auto p0 = params;
  std::swap(p0.front(), p0.back());
  def _0 = def{"0(" + utl::cat(p0, ", ") + ")"} = [&](pack) {
    return pp::tup(res);
  };

  for (std::size_t i = 1; i < conf::bit_length; ++i) {
    auto pn = params;
    std::swap(pn[0], pn[conf::bit_length - i]);
    def{"" + std::to_string(i) + "(" + utl::cat(pn, ", ") + ")"} = [&](pack) {
      return pp::tup(res);
    };
  }

  return def<"o(e, v, i, b)">{[&](arg e, arg v, arg i, arg b) {
    return word(
        def<"<o(i, ...)">{[&](arg i, va args) {
          return pp::call(cat(utl::slice(_0, -1), i), args);
        }}(idec(impl::index(uhex(i), is_int(i), impl::bitlen, e)), bool_(b), bdump(v)),
        typeof(v));
  }}(str(pp::str("[" + bset + "] invalid index") + " : " + args), args);
});

} // namespace api

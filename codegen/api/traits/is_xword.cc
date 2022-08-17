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

#include "traits.h"

namespace api {

using namespace codegen;

namespace detail {
decltype(is_xword_o) is_xword_o = NIFTY_DEF(is_xword_o);
}

decltype(is_xword) is_xword = NIFTY_DEF(is_xword, [&](va args) {
  docs << "[extends " + is_tup + "] detects if args a tup of exactly " + word_size + " ("
              + std::to_string(conf::word_size) + ") hex digits.";

  std::string word = utl::ii << [] {
    std::array<std::string, conf::word_size> hexs{{"9"}};
    for (std::size_t i = 1; i < hexs.size(); ++i)
      hexs[i] = std::string{static_cast<char>('A' + i)};
    return utl::cat(hexs, ", ");
  };

  tests << is_xword("")    = "0" >> docs;
  tests << is_xword("()")  = "0";
  tests << is_xword("foo") = "0" >> docs;
  tests << is_xword("0")   = "0" >> docs;
  if constexpr (conf::word_size > 1)
    tests << is_xword("(0)") = "0";
  tests << is_xword(word)                  = "0" >> docs;
  tests << is_xword(pp::tup(word))         = "1" >> docs;
  tests << is_xword(pp::tup(word + ","))   = "0" >> docs;
  tests << is_xword(pp::tup(word + ", E")) = "0" >> docs;

  def<> _00;
  def<> chk;
  def<> res;
  def<> r;
  if constexpr (conf::word_size > 1) {
    _00 = def{"00"} = [&] { return "0"; };
    def<"01">{}     = [&] { return "0"; };
    def<"10">{}     = [&] { return "0"; };
    def<"11">{}     = [&] { return "1"; };

    chk = def{"chk(" + utl::cat(utl::alpha_base52_seq(conf::word_size - 1), ", ")
              + ", ...)"} = [&](pack args) {
      def verify =
          def{utl::cat(std::vector<std::string>(conf::word_size - 1, "1"))} = [&] { return ""; };

      return pp::cat(utl::slice(verify, -(conf::word_size - 1)),
                     pp::cat(std::vector<std::string>{args.begin(), args.end() - 1}));
    };

    res = def{"res(...)"} = [&](va args) {
      return def<"o(ctup, ...)">{[&](arg ctup, va fin) {
        return def<"<o(...)">{[&](va args) {
          return def<"<o(c, n)">{[&](arg c, arg n) { return pp::cat(utl::slice(_00, -2), c, n); }}(
              args);
        }}(is_none(chk + " " + ctup), is_hex(fin));
      }}(args);
    };

    r = def{"r(...)"} = [&](va args) {
      return def<"o(ctup, _, ...)">{[&](arg ctup, arg first, va rest) {
        return pp::tup(is_hex(first), esc + " " + ctup) + ", " + rest;
      }}(args);
    };
  }

  detail::is_xword_o = def{"o(tup)"} = [&](arg tup) {
    if constexpr (conf::word_size > 1) {
      auto open  = utl::cat(std::vector<std::string>(conf::word_size - 1, r + "("));
      auto close = utl::cat(std::vector<std::string>(conf::word_size - 1, ")"));
      return res(open + "(), " + esc + " " + tup + close);
    } else {
      return is_hex(esc + " " + tup);
    }
  };

  def<"0"> _0 = [&] { return def<"fail(...)">{[&](va) { return "0"; }}; };
  def<"1">{}  = [&] { return detail::is_xword_o; };

  return pp::call(cat(utl::slice(_0, -1), is_tup(args)), args);
});

} // namespace api

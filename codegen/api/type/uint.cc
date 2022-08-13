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

#include "type.h"

namespace api {

using namespace codegen;

decltype(uint) uint = NIFTY_DEF(uint, [&](va args) {
  docs << "[inherits from " + atom + "] " + std::to_string(conf::word_size * 4)
              + "-bit unsigned integer type."
       << "constructible from any integer or word. word construction returns uhex."
       << "cannot parse negative decimals; use math.neg instead."
       << ""
       << "hex length is fixed. cannot parse shorter hex lengths."
       << ""
       << "preserves hex/decimal representation."
       << ""
       << "uses a 'u' suffix for both representations."
       << "see fmt.paste_uint to remove suffix before pasting."
       << ""
       << "cast from signed reinterprets bits as unsigned."
       << "value must be a valid signed int; implicit interpretation"
       << "as unsigned is not allowed (e.g. " + std::to_string(conf::uint_max)
              + " is not a valid integer).";

  auto min    = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "0")) + "u";
  auto max    = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "F")) + "u";
  auto maxpop = max;
  maxpop.pop_back();

  tests << uint(0)          = "0u" >> docs;
  tests << uint(1)          = "1u" >> docs;
  tests << uint("2u")       = "2u" >> docs;
  tests << uint(uint_max_s) = uint_max_s >> docs;
  tests << uint(min)        = min >> docs;
  tests << uint(maxpop)     = max >> docs;
  tests << uint(pp::tup(std::vector<std::string>(conf::word_size, "0"))) =
      ("0x" + utl::cat(std::vector<std::string>(conf::word_size, "0")) + "u") >> docs;
  tests << uint(pp::tup(std::vector<std::string>(conf::word_size, "F"))) =
      ("0x" + utl::cat(std::vector<std::string>(conf::word_size, "F")) + "u") >> docs;

  def<"00(e, ...)"> _00 = [&](arg e, va) {
    return fail(e);
  };

  def<"01(e, i)">{} = [&](arg, arg i) {
    return pp::cat(i, 'u');
  };

  def<"10(e, u)">{} = [&](arg, arg u) {
    return u;
  };

  def<"0(e, ...)"> _0 = [&](arg e, va some) {
    return def<"o(e, atom)">{[&](arg e, arg atom) {
      return pp::call(
          cat(utl::slice(_00, -2), cat(detail::is_uint_o(atom), detail::is_int_o(atom))), e, atom);
    }}(e, atom(some));
  };

  def<"1(e, word)">{} = [&](arg, arg word) {
    auto params = utl::alpha_base52_seq(conf::word_size);
    for (auto&& v : params) {
      if (v == "u") {
        v = "_u";
      } else if (v == "x") {
        v = "_x";
      }
    }
    def cat = def{"cat(" + utl::cat(params, ", ") + ")"} = [&](pack args) {
      return pp::cat("0x", pp::cat(args), "u");
    };

    return esc(cat + " " + word);
  };

  return def<"o(e, ...)">{[&](arg e, va some) {
    return pp::call(cat(utl::slice(_0, -1), detail::is_word_o(some)), e, some);
  }}(istr("[" + uint + "] invalid integer : " + args), some(args));
});

} // namespace api

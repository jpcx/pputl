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

decltype(int_) int_ = NIFTY_DEF(int_, [&](va args) {
  docs << "[inherits from " + atom + "] " + std::to_string(conf::word_size * 4)
              + "-bit signed integer type."
       << "constructible from any integer or word. word construction returns ihex."
       << "cannot parse negative decimals; use math.neg instead."
       << ""
       << "hex length is fixed. cannot parse shorter hex lengths."
       << ""
       << "attempts to preserve hex/decimal representation, but will"
       << "output hex if casting the input yields a negative number"
       << ""
       << "cast from unsigned reinterprets bits as signed two's complement."
       << ""
       << "value must be a valid signed int; implicit interpretation"
       << "as unsigned is not allowed (e.g. " + std::to_string(conf::uint_max)
              + " is not a valid integer).";

  auto min = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "0"));
  auto max = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "F"));

  tests << int_(0)             = "0" >> docs;
  tests << int_("1u")          = "1" >> docs;
  tests << int_(min)           = min >> docs;
  tests << int_(conf::int_max) = std::to_string(conf::int_max) >> docs;
  tests << int_(max + "u")     = max >> docs;
  tests << int_(uint_max_s)    = max >> docs;
  tests << int_(pp::tup((conf::word_size > 1 ? "8, " : "8")
                        + utl::cat(std::vector<std::string>(conf::word_size - 1, "0"), ", "))) =
      int_min_s >> docs;
  tests << int_(pp::tup((conf::word_size > 1 ? "7, " : "7")
                        + utl::cat(std::vector<std::string>(conf::word_size - 1, "F"), ", "))) =
      ("0x7" + utl::cat(std::vector<std::string>(conf::word_size - 1, "F"))) >> docs;

  def<"\\HEX(uint)"> hex = [&](arg uint) {
    return impl::uint_trait(impl::uint_trait(uint, "HUDEC"), "DIHEX");
  };

  def<"\\DEC(uint)">{} = [&](arg uint) {
    def<"0(uint)"> _0 = [&](arg u_) {
      return impl::uint_trait(cat(impl::uint_trait(u_, "DIHEX"), 'u'), "HIDEC");
    };

    def<"1(uint)">{} = [&](arg uint) {
      return impl::uint_trait(uint, "DIHEX");
    };

    return pp::call(cat(utl::slice(_0, -1), impl::uint_trait(uint, "DINEG")), uint);
  };

  def<"o_01(e, uint)">{} = [&](arg, arg uint) {
    docs << "cast from uint";
    return pp::call(cat(utl::slice(hex, -3), impl::uint_trait(uint, "TYPE")), uint);
  };

  def<"o_10(e, i)">{} = [&](arg, arg i) {
    docs << "cast from int";
    return i;
  };

  def<"o_00(e, ...)"> _00 = [&](arg e, va) {
    return fail(e);
  };

  def<"0(e, ...)"> _0 = [&](arg e, va some) {
    return def<"o(e, atom)">{[&](arg e, arg atom) {
      return pp::call(
          cat(utl::slice(_00, -2), cat(detail::is_int_o(atom), detail::is_uint_o(atom))), e, atom);
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
      return pp::cat("0x", pp::cat(args));
    };

    return esc(cat + " " + word);
  };

  return def<"o(e, ...)">{[&](arg e, va some) {
    return pp::call(cat(utl::slice(_0, -1), detail::is_word_o(some)), e, some);
  }}(istr("[" + int_ + "] invalid integer : " + args), some(args));
});

} // namespace api

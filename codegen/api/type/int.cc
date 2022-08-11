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
  docs << "[inherits from " + atom + "] " + std::to_string(conf::bit_length)
              + "-bit signed integer type."
       << "may be constructed from either unsigned or signed ints."
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

  auto min = "0x" + utl::cat(std::vector<std::string>(conf::hex_length, "0"));
  auto max = "0x" + utl::cat(std::vector<std::string>(conf::hex_length, "F"));

  tests << int_(0)             = "0" >> docs;
  tests << int_("1u")          = "1" >> docs;
  tests << int_(min)           = min >> docs;
  tests << int_(conf::int_max) = std::to_string(conf::int_max) >> docs;
  tests << int_(max + "u")     = max >> docs;
  tests << int_(uint_max_s)    = max >> docs;

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

  return def<"o(e, atom)">{[&](arg e, arg atom) {
    return pp::call(cat(utl::slice(_00, -2), cat(detail::is_int_o(atom), detail::is_uint_o(atom))),
                    e, atom);
  }}(istr("[" + int_ + "] invalid integer : " + args), atom(args));
});

} // namespace api

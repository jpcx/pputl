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

decltype(uhex) uhex = NIFTY_DEF(uhex, [&](va args) {
  docs << "[inherits from " + uint + "] casts to the unsigned int hexadecimal subtype.";

  auto min  = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "0")) + "u";
  auto one  = "0x" + utl::cat(std::vector<std::string>(conf::word_size - 1, "0")) + "1u";
  auto max  = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "F")) + "u";
  auto imax = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "F"));
  auto five = "0x" + utl::cat(std::vector<std::string>(conf::word_size - 1, "0")) + "5u";

  tests << uhex(0)          = min >> docs;
  tests << uhex(1u)         = one >> docs;
  tests << uhex(5)          = five >> docs;
  tests << uhex(uint_max_s) = max >> docs;
  tests << uhex(min)        = min >> docs;
  tests << uhex(one)        = one >> docs;
  tests << uhex(imax)       = max >> docs;

  def<"0(udec)"> _0 = [&](arg udec) { return impl::udec(udec, "UHEX"); };
  def<"1(uhex)">{}  = [&](arg uhex) { return uhex; };

  return def<"o(uint)">{[&](arg uint) {
    return pp::call(cat(utl::slice(_0, -1), detail::is_uhex_o(uint)), uint);
  }}(uint(args));
});

} // namespace api

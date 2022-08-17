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

decltype(udec) udec = NIFTY_DEF(udec, [&](va args) {
  docs << "[inherits from " + uint + "] casts to the unsigned int decimal subtype.";

  auto min  = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "0")) + "u";
  auto one  = "0x" + utl::cat(std::vector<std::string>(conf::word_size - 1, "0")) + "1u";
  auto max  = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "F")) + "u";
  auto in1  = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "F"));
  auto five = "0x" + utl::cat(std::vector<std::string>(conf::word_size - 1, "0")) + "5u";

  tests << udec(min)  = "0u" >> docs;
  tests << udec(1)    = "1u" >> docs;
  tests << udec(5u)   = "5u" >> docs;
  tests << udec(five) = "5u" >> docs;
  tests << udec(max)  = uint_max_s >> docs;
  tests << udec(in1)  = uint_max_s >> docs;

  def<"0(uhex)"> _0 = [&](arg uhex) { return impl::uhex(uhex, "UDEC"); };
  def<"1(udec)">{}  = [&](arg udec) { return udec; };

  return def<"o(uint)">{[&](arg uint) {
    return pp::call(cat(utl::slice(_0, -1), detail::is_udec_o(uint)), uint);
  }}(uint(args));
});

} // namespace api

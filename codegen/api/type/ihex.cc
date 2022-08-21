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

decltype(ihex) ihex = NIFTY_DEF(ihex, [&](va args) {
  docs << "[inherits from " + int_ + "] casts to the signed int binary subtype.";

  auto zero = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "0"));
  auto one  = "0x" + utl::cat(std::vector<std::string>(conf::word_size - 1, "0")) + "1";
  auto max  = "0x7" + utl::cat(std::vector<std::string>(conf::word_size - 1, "F"));
  auto umax = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "F"));
  auto five = "0x" + utl::cat(std::vector<std::string>(conf::word_size - 1, "0")) + "5";

  tests << ihex(0)                                   = zero >> docs;
  tests << ihex(1)                                   = one >> docs;
  tests << ihex(5u)                                  = five >> docs;
  tests << ihex(uint_max_s)                          = umax >> docs;
  tests << ihex(std::to_string(conf::int_max) + "u") = max >> docs;

  def<"\\0(idec)"> _0 = [&](arg idec) {
    return impl::uhex(impl::udec(pp::cat(idec, 'u'), "UHEX"), "IHEX");
  };
  def<"\\1(ihex)">{} = [&](arg ihex) { return ihex; };

  return def<"o(int)">{[&](arg int_) {
    return pp::call(cat(utl::slice(_0, -1), detail::is_ihex_o(int_)), int_);
  }}(int_(args));
});

} // namespace api

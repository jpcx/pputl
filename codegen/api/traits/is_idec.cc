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
decltype(is_idec_o) is_idec_o = NIFTY_DEF(is_idec_o);
}

decltype(is_idec) is_idec = NIFTY_DEF(is_idec, [&](va args) {
  docs << "[extends " + is_int + "] detects if args is a signed int in decimal form."
       << "decimals must be smaller than " + int_max + " (" + int_max_s + ")."
       << "negative values must be hex; concatenation with '-' is not supported.";

  auto min = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "0"));
  auto max = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "F"));

  tests << is_idec("1")            = "1" >> docs;
  tests << is_idec("1u")           = "0" >> docs;
  tests << is_idec(int_max_s)      = "1" >> docs;
  tests << is_idec(conf::uint_max) = "0" >> docs;
  tests << is_idec(min + "u")      = "0" >> docs;
  tests << is_idec(max)            = "0" >> docs;
  tests << is_idec("(), ()")       = "0" >> docs;

  detail::is_idec_o = def{"o(int)"} = [&](arg int_) {
    def<"\\0"> _0 = [&] { return "0"; };
    def<"\\1">{}  = [&] { return "1"; };

    return xcat(utl::slice(_0, -1), impl::udec(pp::cat(int_, 'u'), "IS"));
  };

  def<"fail(...)"> fail = [&](va) { return "0"; };

  def<"\\0"> _0 = [&] { return fail; };
  def<"\\1">{}  = [&] { return detail::is_idec_o; };

  return pp::call(xcat(utl::slice(_0, -1), is_int(args)), args);
});

} // namespace api

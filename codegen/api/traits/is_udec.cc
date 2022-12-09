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

decltype(is_udec) is_udec = NIFTY_DEF(is_udec, [&](va args) {
  docs << "[extends is_atom] detects if args matches atoms 0u|1u|...|"
              + std::to_string(conf::uint_max - 1) + "u|" + std::to_string(conf::uint_max)
              + "u.";

  auto min = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "0"));
  auto max = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "F"));

  tests << is_udec("1")            = "0" >> docs;
  tests << is_udec("1u")           = "1" >> docs;
  tests << is_udec(conf::uint_max) = "0" >> docs;
  tests << is_udec(uint_max_s)     = "1" >> docs;
  tests << is_udec(min + "u")      = "0" >> docs;
  tests << is_udec(max)            = "0" >> docs;
  tests << is_udec("(), ()")       = "0" >> docs;

  def<"fail(...)"> fail{[&](va) {
    return "0";
  }};
  def<"\\0">       _0 = [&] {
    return fail;
  };
  def<"\\1">{} = [&] {
    return impl::is_udec;
  };

  return pp::call(xcat(utl::slice(_0, -1), is_atom(args)), args);
});

} // namespace api

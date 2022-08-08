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

decltype(is_binary) is_binary = NIFTY_DEF(is_binary, [&](va args) {
  docs << "detects if args is a uint represented as binary.";

  auto binmin = "0b" + utl::cat(std::vector<std::string>(conf::uint_bits, "0")) + "u";
  auto binmax = "0b" + utl::cat(std::vector<std::string>(conf::uint_bits, "1")) + "u";

  tests << is_binary()               = "0" >> docs;
  tests << is_binary(48)             = "0" >> docs;
  tests << is_binary(conf::uint_max) = "0" >> docs;
  tests << is_binary("foo")          = "0" >> docs;
  tests << is_binary(binmin)         = "1" >> docs;
  tests << is_binary(binmax)         = "1" >> docs;

  def bin = def{std::string{binary}} = [&] {
    return "";
  };

  def<"o_0(...)"> o_0 = [&](va) {
    return "0";
  };

  def<"o_1(u)">{} = [&](arg u) {
    return is_none(cat(utl::slice(bin, -((std::string const&)binary).size()), typeof(u)));
  };

  return pp::call(cat(utl::slice(o_0, -1), is_uint(args)), args);
});

} // namespace api

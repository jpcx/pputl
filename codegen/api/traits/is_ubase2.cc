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

decltype(is_ubase2) is_ubase2 = NIFTY_DEF(is_ubase2, [&](va args) {
  docs << "detects if args is an unsigned int in base2 form (requires 'u' suffix)."
       << "binary bit length is fixed at " + bit_length + " (" + std::to_string(conf::bit_length)
              + ").";

  auto binmin   = "0b" + utl::cat(std::vector<std::string>(conf::bit_length, "0")) + "u";
  auto ibinneg1 = "0b" + utl::cat(std::vector<std::string>(conf::bit_length, "1"));

  tests << is_ubase2("1")      = "0" >> docs;
  tests << is_ubase2("1u")     = "0" >> docs;
  tests << is_ubase2(binmin)   = "1" >> docs;
  tests << is_ubase2(ibinneg1) = "0" >> docs;
  tests << is_ubase2("(), ()") = "0" >> docs;

  def ubase2_ = def{(std::string const&)ubase2} = [&] {
    return "";
  };

  def<"0(...)"> _0 = [&](va) {
    return "0";
  };

  def<"1(...)">{} = [&](va args) {
    return is_none(cat(utl::slice(ubase2_, -((std::string const&)ubase2).size()), typeof(args)));
  };

  return pp::call(cat(utl::slice(_0, -1), is_any(args)), args);
});

} // namespace api

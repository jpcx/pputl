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

decltype(is_ibase10) is_ibase10 = NIFTY_DEF(is_ibase10, [&](va args) {
  docs << "detects if args is a signed int in base10 form.";

  auto binmin   = "0b" + utl::cat(std::vector<std::string>(conf::bit_length, "0")) + "u";
  auto ibinneg1 = "0b" + utl::cat(std::vector<std::string>(conf::bit_length, "1"));

  tests << is_ibase10("1")       = "1" >> docs;
  tests << is_ibase10("1u")      = "0" >> docs;
  tests << is_ibase10(int_max_s) = "1" >> docs;
  tests << is_ibase10(binmin)    = "0" >> docs;
  tests << is_ibase10(ibinneg1)  = "0" >> docs;
  tests << is_ibase10("(), ()")  = "0" >> docs;

  def ibase10_ = def{(std::string const&)ibase10} = [&] {
    return "";
  };

  def<"0(...)"> _0 = [&](va) {
    return "0";
  };

  def<"1(...)">{} = [&](va args) {
    return is_none(cat(utl::slice(ibase10_, -((std::string const&)ibase10).size()), typeof(args)));
  };

  return pp::call(cat(utl::slice(_0, -1), is_any(args)), args);
});

} // namespace api

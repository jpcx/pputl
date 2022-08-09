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

decltype(is_int) is_int = NIFTY_DEF(is_int, [&](va args) {
  docs << "detects if args is a signed integer."
       << "hex length is fixed at " + hex_length + " (" + std::to_string(conf::hex_length) + ").";

  auto min = "0x" + utl::cat(std::vector<std::string>(conf::hex_length, "0"));
  auto max = "0x" + utl::cat(std::vector<std::string>(conf::hex_length, "F"));

  tests << is_int()               = "0" >> docs;
  tests << is_int("foo")          = "0" >> docs;
  tests << is_int(0)              = "1" >> docs;
  tests << is_int("0u")           = "0" >> docs;
  tests << is_int(conf::uint_max) = "0" >> docs;
  tests << is_int(min + "u")      = "0" >> docs;
  tests << is_int(max)            = "1" >> docs;
  tests << is_int("0b110u")       = "0" >> docs;
  tests << is_int("(), ()")       = "0" >> docs;

  def ihex_ = def{(std::string const&)ihex} = [&] {
    return "";
  };

  def{(std::string const&)idec} = [&] {
    return "";
  };

  def<"0(...)"> _0 = [&](va) {
    return "0";
  };

  def<"1(...)">{} = [&](va args) {
    return is_none(cat(utl::slice(ihex_, -((std::string const&)ihex).size()), typeof(args)));
  };

  return pp::call(cat(utl::slice(_0, -1), is_any(args)), args);
});

} // namespace api

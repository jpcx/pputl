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

decltype(is_bool) is_bool = NIFTY_DEF(is_bool, [&](va args) {
  docs << "detects if args is a bool.";

  auto binmin = "0b" + utl::cat(std::vector<std::string>(conf::bit_length, "0"));

  tests << is_bool()         = "0" >> docs;
  tests << is_bool(0)        = "1" >> docs;
  tests << is_bool(1)        = "1" >> docs;
  tests << is_bool("1u")     = "0" >> docs;
  tests << is_bool(binmin)   = "0" >> docs;
  tests << is_bool(0, 1)     = "0" >> docs;
  tests << is_bool("(0)")    = "0" >> docs;
  tests << is_bool("()")     = "0";
  tests << is_bool("(), ()") = "0";
  tests << is_bool("0, ")    = "0";
  tests << is_bool(", ")     = "0";
  tests << is_bool(", , ")   = "0";
  tests << is_bool("a, ")    = "0";
  tests << is_bool("a, , ")  = "0";
  tests << is_bool(", a")    = "0";
  tests << is_bool(", a, ")  = "0";
  tests << is_bool(", , a")  = "0";

  def fail = def{(std::string const&)detail::bool_fail} = [&] {
    return "0";
  };

  def{(std::string const&)detail::bool_pass} = [&] {
    return "1";
  };

  return cat(utl::slice(fail, -((std::string const&)detail::bool_pass).size()),
             pp::call(pp::call(detail::bool_o(args + "."), args), args));
});

} // namespace api

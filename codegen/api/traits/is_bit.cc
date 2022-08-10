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

decltype(is_bit) is_bit = NIFTY_DEF(is_bit, [&] {
  docs << "[aliases " + is_bool + "] detects if args is a bit (bool).";

  auto binmin = "0b" + utl::cat(std::vector<std::string>(conf::bit_length, "0"));

  tests << pp::call(is_bit)           = "0" >> docs;
  tests << pp::call(is_bit, 0)        = "1" >> docs;
  tests << pp::call(is_bit, 1)        = "1" >> docs;
  tests << pp::call(is_bit, "1u")     = "0" >> docs;
  tests << pp::call(is_bit, binmin)   = "0" >> docs;
  tests << pp::call(is_bit, 0, 1)     = "0" >> docs;
  tests << pp::call(is_bit, "(0)")    = "0" >> docs;
  tests << pp::call(is_bit, "()")     = "0";
  tests << pp::call(is_bit, "(), ()") = "0";
  tests << pp::call(is_bit, "0, ")    = "0";
  tests << pp::call(is_bit, ", ")     = "0";
  tests << pp::call(is_bit, ", , ")   = "0";
  tests << pp::call(is_bit, "a, ")    = "0";
  tests << pp::call(is_bit, "a, , ")  = "0";
  tests << pp::call(is_bit, ", a")    = "0";
  tests << pp::call(is_bit, ", a, ")  = "0";
  tests << pp::call(is_bit, ", , a")  = "0";

  return is_bool;
});

} // namespace api

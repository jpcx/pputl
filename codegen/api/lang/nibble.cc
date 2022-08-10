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

#include "lang.h"

namespace api {

using namespace codegen;

decltype(nibble) nibble = NIFTY_DEF(nibble, [&](va args) {
  docs << "translates four bits to a hexadecimal digit.";

  tests << nibble(0, 0, 0, 0) = "0";
  tests << nibble(0, 0, 1, 0) = "2";
  tests << nibble(1, 0, 0, 1) = "9";
  tests << nibble(1, 1, 0, 1) = "D";
  tests << nibble(1, 1, 1, 1) = "F";

  def<"0000"> _0000 = [&] {
    return "0";
  };

  def<"0001">{} = [&] {
    return "1";
  };

  def<"0010">{} = [&] {
    return "2";
  };

  def<"0011">{} = [&] {
    return "3";
  };

  def<"0100">{} = [&] {
    return "4";
  };

  def<"0101">{} = [&] {
    return "5";
  };

  def<"0110">{} = [&] {
    return "6";
  };

  def<"0111">{} = [&] {
    return "7";
  };

  def<"1000">{} = [&] {
    return "8";
  };

  def<"1001">{} = [&] {
    return "9";
  };

  def<"1010">{} = [&] {
    return "A";
  };

  def<"1011">{} = [&] {
    return "B";
  };

  def<"1100">{} = [&] {
    return "C";
  };

  def<"1101">{} = [&] {
    return "D";
  };

  def<"1110">{} = [&] {
    return "E";
  };

  def<"1111">{} = [&] {
    return "F";
  };

  return def<"o(b0, b1, b2, b3)">{[&](pack args) {
    return pp::cat(utl::slice(_0000, -4), pp::cat(args));
  }}(args);
});

} // namespace api

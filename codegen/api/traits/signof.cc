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

decltype(signof) signof = NIFTY_DEF(signof, [&](va args) {
  docs << "detects if a value is signed, unsigned, or a non-integral."
       << "returns literal INT, UINT, or NONE.";

  tests << signof(0)                               = "INT" >> docs;
  tests << signof(int_max_s)                       = "INT";
  tests << signof(int_min_s)                       = "INT" >> docs;
  tests << signof("1u")                            = "UINT" >> docs;
  tests << signof(uint_max_s)                      = "UINT" >> docs;
  tests << signof("0x" + utl::cat(samp::h7) + "u") = "UINT" >> docs;
  tests << signof("foo")                           = "NONE" >> docs;
  tests << signof(pp::tup(samp::himax))            = "NONE" >> docs;

  def<"00"> _00 = [&] { return "NONE"; };
  def<"10">{}   = [&] { return "INT"; };
  def<"01">{}   = [&] { return "UINT"; };

  return cat(utl::slice(_00, -2), cat(is_int(args), is_uint(args)));
});

} // namespace api

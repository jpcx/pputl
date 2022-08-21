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
  docs << "detects the signedness of a word."
       << "returns literal I or U."
       << "utup is considered unsigned."
       << "fails if args is not a word.";

  tests << signof(0)                               = "I" >> docs;
  tests << signof(int_max_s)                       = "I";
  tests << signof(int_min_s)                       = "I" >> docs;
  tests << signof("1u")                            = "U" >> docs;
  tests << signof(uint_max_s)                      = "U" >> docs;
  tests << signof("0x" + utl::cat(samp::h7) + "u") = "U" >> docs;
  tests << signof(pp::tup(samp::himax))            = "U" >> docs;

  def<"000(e)"> _000 = [&](arg e) { return fail(e); };
  def<"100(e)">{}    = [&](arg) { return "I"; };
  def<"010(e)">{}    = [&](arg) { return "U"; };
  def<"001(e)">{}    = [&](arg) { return "U"; };

  return pp::call(
      cat(cat(utl::slice(_000, -3), is_int(args)), cat(is_uint(args), is_utup(args))),
      str("[" + signof + "] invalid word : " + args));
});

} // namespace api

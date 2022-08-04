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

#include "numeric.h"

namespace api {

using namespace codegen;

decltype(nez) nez = NIFTY_DEF(nez, [&](va args) {
  docs << "detects if uint n is not zero. terminates expansion on non-uint.";

  tests << nez("0")                 = "0" >> docs;
  tests << nez("1")                 = "1" >> docs;
  tests << nez("2")                 = "1" >> docs;
  tests << nez(uint_max_s)          = "1" >> docs;
  tests << nez(inc(uint_max_s))     = "0" >> docs;
  tests << str(nez(conf::uint_max)) = pp::str(1) >> docs;
  tests << str(nez())               = pp::str(nez()) >> docs;
  tests << str(nez('a'))            = pp::str(nez('a')) >> docs;
  tests << str(nez("foo"))          = pp::str(nez("foo")) >> docs;

  def<"chk_0(...)"> chk_0 = [&](va args) {
    return nez(args);
  };

  def<"chk_1(n)">{} = [&](arg n) {
    return is_some(pp::cat(utl::slice(detail::eqz_0, -1), n));
  };

  return pp::call(cat(utl::slice(chk_0, -1), is_uint(args)), args);
});

} // namespace api

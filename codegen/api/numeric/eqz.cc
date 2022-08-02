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

decltype(eqz) eqz = NIFTY_DEF(eqz, [&](va args) {
  docs << "detects if uint n is zero.";

  tests << eqz("0")             = "1" >> docs;
  tests << eqz("1")             = "0" >> docs;
  tests << eqz("2")             = "0" >> docs;
  tests << eqz(uint_max_s)      = "0" >> docs;
  tests << eqz(inc(uint_max_s)) = "1" >> docs;

  def<"0"> _0 = [&]() {
    return "";
  };

  def<"pass"> pass = [&] {
    return "1";
  };

  def<"no_pass"> no_pass = [&] {
    return "0";
  };

  def<"res(...)"> res = [&](va) {
    std::string const prefix    = utl::slice(pass, -4);
    std::string const pass_s    = utl::slice(pass, prefix.size(), 0);
    std::string const no_pass_s = utl::slice(no_pass, prefix.size(), 0);

    return pp::cat(prefix,
                   pp::va_opt(utl::slice(no_pass_s, (no_pass_s.size() == 7 ? 3 : 2) - no_pass_s.size())),
                   pass_s);
  };

  return def<"x(n)">{[&](arg n) {
    return def<"x(n)">{[&](arg n) {
      return res(pp::cat(utl::slice(_0, -1), n));
    }}(n);
  }}(uint(args));
});

} // namespace api

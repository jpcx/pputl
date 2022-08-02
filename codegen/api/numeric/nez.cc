////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////

#include "numeric.h"

namespace api {

using namespace codegen;

decltype(nez) nez = NIFTY_DEF(nez, [&](va args) {
  docs << "detects if uint n is not zero.";

  tests << nez("0")             = "0" >> docs;
  tests << nez("1")             = "1" >> docs;
  tests << nez("2")             = "1" >> docs;
  tests << nez(uint_max_s)      = "1" >> docs;
  tests << nez(inc(uint_max_s)) = "0" >> docs;

  def<"0"> _0 = [&]() {
    return "";
  };

  def<"fail"> fail = [&] {
    return "0";
  };

  def<"no_fail"> no_fail = [&] {
    return "1";
  };

  def<"res(...)"> res = [&](va args) {
    std::string const prefix    = utl::slice(fail, -4);
    std::string const fail_s    = utl::slice(fail, prefix.size(), 0);
    std::string const no_fail_s = utl::slice(no_fail, prefix.size(), 0);

    return pp::cat(prefix,
                   pp::va_opt(utl::slice(no_fail_s, (no_fail_s.size() == 7 ? 3 : 2) - no_fail_s.size())),
                   fail_s);
  };

  return def<"x(n)">{[&](arg n) {
    return def<"x(n)">{[&](arg n) {
      return res(pp::cat(utl::slice(_0, -1), n));
    }}(n);
  }}(uint(args));
});

} // namespace api

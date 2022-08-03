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

decltype(inc) inc = NIFTY_DEF(inc, [&](va args) {
  docs << "uint increment w/ overflow. terminates expansion on non-uint.";

  tests << inc(0)                   = "1" >> docs;
  tests << inc(1)                   = "2" >> docs;
  tests << inc(2)                   = "3";
  tests << inc(conf::uint_max)      = "0" >> docs;
  tests << inc(conf::uint_max - 1)  = uint_max_s;
  tests << str(inc(conf::uint_max)) = pp::str(0) >> docs;
  tests << str(inc())               = pp::str(inc()) >> docs;
  tests << str(inc('a'))            = pp::str(inc('a')) >> docs;
  tests << str(inc("foo"))          = pp::str(inc("foo")) >> docs;

  def<"oo_fail(...)"> oo_fail = [&](va args) {
    return inc(args);
  };

  def<"oo_no_fail(n)"> oo_no_fail = [&](arg n) {
    docs << "second parentheses; returns incremented value from uint range or throws.";
    return rest(cat(utl::slice(detail::uint_range[0], -1), n));
  };

  return pp::call(def<"o(...)">{[&](va args) {
                    docs << "first parentheses; asserts uint";
                    return def<"x(_, ...)">{[&](arg, va) {
                      std::string prefix = utl::slice(oo_fail, -4);
                      if (prefix.back() == '_')
                        prefix.pop_back();
                      std::string fail_s    = utl::slice(oo_fail, prefix.size(), 0);
                      std::string no_fail_s = utl::slice(oo_no_fail, prefix.size(), 0);
                      std::string no_s      = utl::slice(no_fail_s, -fail_s.size());

                      return pp::cat(prefix, pp::va_opt(no_s), fail_s);
                    }}(args);
                  }}(cat(utl::slice(detail::uint_range[0], -1), uint(args))),
                  args);
});

} // namespace api

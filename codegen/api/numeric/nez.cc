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

  def<"ooo_throw(...)"> ooo_throw_ = [&](va args) {
    return nez(args);
  };

  def<"ooo_fail(...)"> ooo_fail = [&](va) {
    return "0";
  };

  def<"ooo_no_fail(...)"> ooo_no_fail = [&](va) {
    docs << "third parentheses; returns or throws";
    return "1";
  };

  def<"oo_fail(...)"> oo_fail = [&](va) {
    return ooo_throw_;
  };

  def<"oo_no_fail(n)"> oo_no_fail = [&](arg n) {
    docs << "second parentheses; verifies not literal 0.";
    return def<"x(...)">{[&](va) {
      std::string prefix = utl::slice(ooo_fail, -4);
      if (prefix.back() == '_')
        prefix.pop_back();
      std::string fail_s    = utl::slice(ooo_fail, prefix.size(), 0);
      std::string no_fail_s = utl::slice(ooo_no_fail, prefix.size(), 0);
      std::string no_s      = utl::slice(no_fail_s, -fail_s.size());

      return pp::cat(prefix, pp::va_opt(no_s), fail_s);
    }}(cat(utl::slice(detail::eqz_0, -1), n));
  };

  return pp::call(pp::call(def<"o(...)">{[&](va args) {
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
                           args),
                  args);
});

} // namespace api

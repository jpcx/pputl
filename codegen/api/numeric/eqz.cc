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

namespace detail {
decltype(eqz_0) eqz_0 = NIFTY_DEF(eqz_0);
}

decltype(eqz) eqz = NIFTY_DEF(eqz, [&](va args) {
  docs << "detects if uint n is zero. terminates expansion on non-uint.";

  tests << eqz("0")                 = "1" >> docs;
  tests << eqz("1")                 = "0" >> docs;
  tests << eqz("2")                 = "0" >> docs;
  tests << eqz(uint_max_s)          = "0" >> docs;
  tests << eqz(inc(uint_max_s))     = "1" >> docs;
  tests << str(eqz(conf::uint_max)) = pp::str(0) >> docs;
  tests << str(eqz())               = pp::str(eqz()) >> docs;
  tests << str(eqz('a'))            = pp::str(eqz('a')) >> docs;
  tests << str(eqz("foo"))          = pp::str(eqz("foo")) >> docs;

  detail::eqz_0 = def{"0"} = [&] {
    docs << "validator for literal 0";
    return "";
  };

  def<"ooo_throw(...)"> ooo_throw_ = [&](va args) {
    return eqz(args);
  };

  def<"ooo_pass(...)"> ooo_pass = [&](va) {
    return "1";
  };

  def<"ooo_no_pass(...)"> ooo_no_pass = [&](va) {
    docs << "third parentheses; returns or throws";
    return "0";
  };

  def<"oo_fail(...)"> oo_fail = [&](va) {
    return ooo_throw_;
  };

  def<"oo_no_fail(n)"> oo_no_fail = [&](arg n) {
    docs << "second parentheses; verifies literal 0.";
    return def<"x(...)">{[&](va) {
      std::string prefix = utl::slice(ooo_pass, -4);
      if (prefix.back() == '_')
        prefix.pop_back();
      std::string pass_s    = utl::slice(ooo_pass, prefix.size(), 0);
      std::string no_pass_s = utl::slice(ooo_no_pass, prefix.size(), 0);
      std::string no_s      = utl::slice(no_pass_s, -pass_s.size());

      return pp::cat(prefix, pp::va_opt(no_s), pass_s);
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

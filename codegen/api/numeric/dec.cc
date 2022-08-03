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

decltype(dec) dec = NIFTY_DEF(dec, [&](va args) {
  docs << "uint decrement w/ underflow. terminates expansion on non-uint.";

  tests << dec(0)                   = uint_max_s >> docs;
  tests << dec(1)                   = "0" >> docs;
  tests << dec(2)                   = "1";
  tests << dec(conf::uint_max)      = utl::to_string(conf::uint_max - 1) >> docs;
  tests << dec(conf::uint_max - 1)  = utl::to_string(conf::uint_max - 2);
  tests << str(dec(conf::uint_max)) = pp::str(utl::to_string(conf::uint_max - 1)) >> docs;
  tests << str(dec())               = pp::str(dec()) >> docs;
  tests << str(dec('a'))            = pp::str(dec('a')) >> docs;
  tests << str(dec("foo"))          = pp::str(dec("foo")) >> docs;

  def<"oo_fail(...)"> oo_fail = [&](va args) {
    return dec(args);
  };

  def<"oo_no_fail(n)"> oo_no_fail = [&](arg n) {
    docs << "second parentheses; returns decremented value from uint range or throws.";
    return first(cat(utl::slice(detail::uint_range[0], -1), n));
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

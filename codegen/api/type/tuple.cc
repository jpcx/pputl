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

#include "type.h"

namespace api {

using namespace codegen;

decltype(tuple) tuple = NIFTY_DEF(tuple, [&](va args) {
  docs << "tuple type (any...)."
       << "expands to t if valid. terminates expansion on non-tuple.";

  tests << tuple(pp::tup())     = "()" >> docs;
  tests << tuple(pp::tup(1, 2)) = "(1, 2)" >> docs;
  tests << str(tuple(0))        = pp::str(tuple + "(0)") >> docs;
  tests << str(tuple("1, 2"))   = pp::str(tuple + "(1, 2)") >> docs;
  tests << str(tuple("1,"))     = pp::str(tuple + "(1,)") >> docs;
  tests << str(tuple("foo"))    = pp::str(tuple + "(foo)") >> docs;
  tests << str(tuple("(), ()")) = pp::str(tuple + "((), ())") >> docs;

  def<"oo_pass(...)"> oo_pass = [&](va args) {
    return args;
  };

  def<"oo_no_pass(...)"> oo_no_pass = [&](va args) {
    docs << "second parentheses; returns or throws";
    return tuple(args);
  };

  return pp::call(def<"o(...)">{[&](va) {
                    docs << "first parentheses; detects tuple";
                    std::string prefix = utl::slice(oo_pass, -4);
                    if (prefix.back() == '_')
                      prefix.pop_back();
                    std::string pass_s    = utl::slice(oo_pass, prefix.size(), 0);
                    std::string no_pass_s = utl::slice(oo_no_pass, prefix.size(), 0);
                    std::string no_s      = utl::slice(no_pass_s, -pass_s.size());

                    return pp::cat(prefix, pp::va_opt(no_s), pass_s);
                  }}(eat + " " + args),
                  args);
});

} // namespace api
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

decltype(tup) tup = NIFTY_DEF(tup, [&](va args) {
  docs << "tuple type (any...)."
       << "expands to t if valid, else, terminates expansion with a self-reference.";

  tests << tup(pp::tup())     = "()" >> docs;
  tests << tup(pp::tup(1, 2)) = "(1, 2)" >> docs;
  tests << str(tup(0))        = ("\"" + tup + "(0)" + "\"") >> docs;
  tests << str(tup("1, 2"))   = ("\"" + tup + "(1, 2)" + "\"") >> docs;
  tests << str(tup("1,"))     = ("\"" + tup + "(1,)" + "\"") >> docs;
  tests << str(tup("foo"))    = ("\"" + tup + "(foo)" + "\"") >> docs;
  tests << str(tup("(), ()")) = ("\"" + tup + "((), ())" + "\"") >> docs;

  def<"fail(...)"> fail = [&](va args) {
    return tup(args);
  };

  def<"pass(...)"> pass = [&](va args) {
    docs << "fourth parentheses; returns";
    return args;
  };

  def<>             oo_no_pass{};
  def<"oo(_, ...)"> oo = [&](arg _, va) {
    docs << "second parentheses; asserts tuple.";

    oo_no_pass = def{"no_pass(...)"} = [&](va) {
      return fail;
    };

    def<"pass(...)"> oo_pass = [&](va) {
      return pass;
    };

    return def<"res(...)">{[&](va) {
      std::string const prefix    = utl::slice(oo_pass, -4);
      std::string const pass_s    = utl::slice(oo_pass, prefix.size(), 0);
      std::string const no_pass_s = utl::slice(oo_no_pass, prefix.size(), 0);

      return pp::call(pp::cat(
          prefix, pp::va_opt(utl::slice(no_pass_s, (no_pass_s.size() == 7 ? 3 : 2) - no_pass_s.size())),
          pass_s));
    }}(eat + " " + _);
  };

  def<"o(_, ...)"> o = [&](arg, va) {
    docs << "first parentheses; asserts only one arg.";

    def<"pass(...)"> pass = [&](va) {
      return oo;
    };

    def<"no_pass(...)"> no_pass = [&](va) {
      return oo_no_pass;
    };

    std::string const prefix    = utl::slice(pass, -4);
    std::string const pass_s    = utl::slice(pass, prefix.size(), 0);
    std::string const no_pass_s = utl::slice(no_pass, prefix.size(), 0);

    return pp::call(
        pp::cat(prefix, pp::va_opt(utl::slice(no_pass_s, (no_pass_s.size() == 7 ? 3 : 2) - no_pass_s.size())),
                pass_s));
  };

  return pp::call(pp::call(o(args + "."), args), args);
});

} // namespace api

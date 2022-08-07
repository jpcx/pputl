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

#include "lang.h"

namespace api {

using namespace codegen;

decltype(trycat) trycat = NIFTY_DEF(trycat, [&](va args) {
  docs << "concatenates two args after an expansion."
       << "returns default if either arg is a tuple."
       << ""
       << "args must either be compatible with the ## operator"
       << "or be tuples. several tokens are incompatible with"
       << "the ## operator that cannot be detected; tuples are"
       << "the only detectible fail condition.";

  tests << trycat("foo", "bar")                          = "foobar" >> docs;
  tests << trycat("foo", eat("bar"))                     = "foo" >> docs;
  tests << trycat("foo", pp::tup("bar"))                 = "" >> docs;
  tests << trycat("foo", pp::tup("bar"), "err")          = "err" >> docs;
  tests << trycat(pp::tup("foo"), "bar", "err")          = "err";
  tests << trycat(pp::tup("foo"), pp::tup("bar"), "err") = "err";

  def<"ooo_fail(a, b, ...)"> ooo_fail = [&](arg, arg, va args) {
    return args;
  };

  def<"ooo_no_fail(a, b, ...)"> ooo_no_fail = [&](arg a, arg b, va) {
    return pp::cat(a, b);
  };

  def<"ooo_chk(...)"> ooo_chk = [&](va) {
    std::string prefix = utl::slice(ooo_fail, -4);
    if (prefix.back() == '_')
      prefix.pop_back();
    std::string fail_s    = utl::slice(ooo_fail, prefix.size(), 0);
    std::string no_fail_s = utl::slice(ooo_no_fail, prefix.size(), 0);
    std::string no_s      = utl::slice(no_fail_s, -fail_s.size());

    return pp::cat(prefix, pp::va_opt(no_s), fail_s);
  };

  def<"oo_fail(a, b, ...)"> oo_fail = [&](arg, arg, va) {
    return ooo_fail;
  };

  def<"oo_no_fail(a, b, ...)"> oo_no_fail = [&](arg, arg b, va) {
    return ooo_chk(eat + " " + b);
  };

  def<"oo_chk(...)"> oo_chk = [&](va) {
    std::string prefix = utl::slice(oo_fail, -4);
    if (prefix.back() == '_')
      prefix.pop_back();
    std::string fail_s    = utl::slice(oo_fail, prefix.size(), 0);
    std::string no_fail_s = utl::slice(oo_no_fail, prefix.size(), 0);
    std::string no_s      = utl::slice(no_fail_s, -fail_s.size());

    return pp::cat(prefix, pp::va_opt(no_s), fail_s);
  };

  return pp::va_opt(pp::call(pp::call(def<"o(a, b, ...)">{[&](arg a, arg, va) {
                                        return oo_chk(eat + " " + a);
                                      }}(args),
                                      args),
                             args));
});

} // namespace api

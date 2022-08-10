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

decltype(any) any = NIFTY_DEF(any, [&](va args) {
  docs << "[inherits from " + some + "] any type (generic data). returns arg."
       << "describes exactly one generic value."
       << ""
       << "fails if not exactly one arg.";

  tests << any("foo") = "foo" >> docs;

  def<"ooo_fail_first(e0, e1, ...)"> ooo_fail_first = [&](arg e0, arg, va) {
    return fail(e0);
  };

  def<"ooo_no_fail_first(e0, e1, any)"> ooo_no_fail_first = [&](arg, arg, arg any) {
    return any;
  };

  def<"ooo_fail_rest(e0, e1, ...)"> ooo_fail_rest = [&](arg, arg e1, va) {
    return fail(e1);
  };

  def<"oo_pass(...)"> oo_pass = [&](va) {
    std::string prefix = utl::slice(ooo_fail_first, -5);
    if (prefix.back() == '_')
      prefix.pop_back();
    prefix = utl::slice(prefix, -4);

    std::string fail_first_s    = utl::slice(ooo_fail_first, prefix.size(), 0);
    std::string no_fail_first_s = utl::slice(ooo_no_fail_first, prefix.size(), 0);
    std::string no_s            = utl::slice(no_fail_first_s, -fail_first_s.size());

    return pp::cat(prefix, pp::va_opt(no_s), fail_first_s);
  };

  def<"oo_no_pass(...)"> oo_no_pass = [&](va) {
    return ooo_fail_rest;
  };

  return pp::call(pp::call(def<"o(_, ...)">{[&](arg, va) {
                             std::string prefix = utl::slice(oo_pass, -4);
                             if (prefix.back() == '_')
                               prefix.pop_back();

                             std::string pass_s    = utl::slice(oo_pass, prefix.size(), 0);
                             std::string no_pass_s = utl::slice(oo_no_pass, prefix.size(), 0);
                             std::string no_s      = utl::slice(no_pass_s, -pass_s.size());

                             return pp::cat(prefix, pp::va_opt(no_s), pass_s);
                           }}(args + "."),
                           args),
                  istr("[" + any + "] any cannot describe nothing : " + args),
                  istr("[" + any + "] any cannot describe more than one generic value : " + args),
                  args);
});

} // namespace api

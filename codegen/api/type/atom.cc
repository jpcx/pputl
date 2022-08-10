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

decltype(atom) atom = NIFTY_DEF(atom, [&](va args) {
  docs << "[inherits from " + any + "] a generic, non-tuple, singular value."
       << "must not be a deferred expression."
       << "fails if tuple.";

  tests << atom("foo") = "foo" >> docs;

  def<"oo_fail(err, ...)"> oo_fail = [&](arg err, va) {
    return fail(err);
  };

  def<"oo_no_fail(err, ...)"> oo_no_fail = [&](arg, va args) {
    return args;
  };

  return pp::call(def<"o(...)">{[&](va) {
                    std::string prefix = utl::slice(oo_fail, -4);
                    if (prefix.back() == '_')
                      prefix.pop_back();
                    std::string fail_s    = utl::slice(oo_fail, prefix.size(), 0);
                    std::string no_fail_s = utl::slice(oo_no_fail, prefix.size(), 0);
                    std::string no_s      = utl::slice(no_fail_s, -fail_s.size());

                    return pp::cat(prefix, pp::va_opt(no_s), fail_s);
                  }}(esc(eat + " " + any(args))),
                  istr("[" + atom + "] atom cannot describe a tuple : " + args), args);
});

} // namespace api

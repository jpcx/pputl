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

decltype(none) none = NIFTY_DEF(none, [&](va args) {
  docs << "nothing. fails if something.";

  tests << none() = "" >> docs;

  def<"pass(e)"> pass = [&](arg) {
    return "";
  };

  def<"no_pass(e)"> no_pass = [&](arg e) {
    return fail(e);
  };

  std::string prefix = utl::slice(pass, -4);
  if (prefix.back() == '_')
    prefix.pop_back();

  std::string pass_s    = utl::slice(pass, prefix.size(), 0);
  std::string no_pass_s = utl::slice(no_pass, prefix.size(), 0);
  std::string no_s      = utl::slice(no_pass_s, -pass_s.size());

  return pp::call(pp::cat(prefix, pp::va_opt(no_s), pass_s),
                  istr("[" + none + "] none cannot describe something : " + args));
});

} // namespace api

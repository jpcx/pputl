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

decltype(bool_) bool_ = NIFTY_DEF(bool_, [&](va args) {
  docs << "[inherits from " + atom + "] bool type (0 or 1)."
       << "expands to b if valid, else fails.";

  tests << bool_(0) = "0" >> docs;
  tests << bool_(1) = "1" >> docs;

  def<"0"> _0 = [&] {
    return "";
  };

  def<"1">{} = [&] {
    return "";
  };

  def<"no_pass(e, ...)"> no_pass = [&](arg e, va) {
    return fail(e);
  };

  def<"pass(e, b)"> pass = [&](arg, arg b) {
    return b;
  };

  return pp::call(def<"o(...)">{[&](va) {
                    std::string prefix = utl::slice(pass, -4);
                    if (prefix.back() == '_')
                      prefix.pop_back();

                    std::string pass_s    = utl::slice(pass, prefix.size(), 0);
                    std::string no_pass_s = utl::slice(no_pass, prefix.size(), 0);
                    std::string no_s      = utl::slice(no_pass_s, -pass_s.size());

                    return pp::cat(prefix, pp::va_opt(no_s), pass_s);
                  }}(cat(utl::slice(_0, -1), atom(args))),
                  istr("[" + bool_ + "] invalid bool : " + args), args);
});

} // namespace api

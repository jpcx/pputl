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

#include "control.h"

namespace api {

using namespace codegen;

decltype(switch_) switch_ = NIFTY_DEF(switch_, [&](va args) {
  docs << "conditionally expands based on a size."
       << "the final case is default.";

  std::vector<std::string> maxcases{};
  for (size_t i = 0; i <= conf::size_max; ++i)
    maxcases.push_back(utl::alpha_base52(i));
  auto maxcases_s = utl::cat(maxcases, ", ");

  tests << switch_(0, "1")                     = "1" >> docs;
  tests << switch_(1, "1")                     = "1" >> docs;
  tests << switch_(2, "1")                     = "1" >> docs;
  tests << switch_(0, "1, 2")                  = "1";
  tests << switch_(1, "1, 2")                  = "2" >> docs;
  tests << switch_(2, "1, 2")                  = "2" >> docs;
  tests << switch_(1, "1, 2, 3")               = "2";
  tests << switch_(2, "1, 2, 3")               = "3";
  tests << switch_(conf::size_max, "1, 2, 3")  = "3" >> docs;
  tests << switch_(0, maxcases_s)              = "a";
  tests << switch_(1, maxcases_s)              = "b";
  tests << switch_(conf::size_max, maxcases_s) = utl::alpha_base52(conf::size_max);

  def<"x(...)"> x = [&](va args) {
    return args;
  };

  def<"r(...)"> r = [&](va args) {
    def<"base(i, _, ...)"> base = [&](arg, arg _0, va) {
      return "0, " + _0;
    };
    def<"recr(i, _, ...)"> recr = [&](arg i, arg, va args) {
      return dec(i) + ", " + args;
    };

    return def<"x(i, _, ...)">{[&](arg i, arg _0, va args) {
      return pp::call(if_(or_(eqz(i), is_none(args)), base, recr), i, _0, args);
    }}(args);
  };

  def<"res(...)"> res = [&](va args) {
    return def<"x(i, _, ...)">{[&](arg, arg _0, va) {
      return _0;
    }}(args);
  };

  return res(recur(x, first(args), r, args));
});

} // namespace api

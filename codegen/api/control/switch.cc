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
  docs << "conditionally expands items based on a size."
       << "the final tuple is the default case.";

  std::vector<std::string> maxcases{};
  for (size_t i = 0; i <= conf::size_max; ++i)
    maxcases.push_back(pp::tup(utl::alpha_base52(i)));
  auto maxcases_s = utl::cat(maxcases, ", ");

  tests << switch_(0, "(1)")                           = "1" >> docs;
  tests << switch_(1, "(1)")                           = "1" >> docs;
  tests << switch_(2, "(1)")                           = "1" >> docs;
  tests << switch_(0, "(1), (2)")                      = "1";
  tests << switch_(1, "(1), (2)")                      = "2" >> docs;
  tests << switch_(2, "(1), (2)")                      = "2" >> docs;
  tests << switch_(1, "(1), (2), (3)")                 = "2";
  tests << switch_(2, "(1), (2), (3)")                 = "3";
  tests << switch_(2, "(1), (2), (3, 4)")              = "3, 4" >> docs;
  tests << switch_(conf::size_max, "(1), (2), (3, 4)") = "3, 4" >> docs;
  tests << switch_(0, maxcases_s)                      = "a";
  tests << switch_(1, maxcases_s)                      = "b";
  tests << switch_(conf::size_max, maxcases_s) = utl::alpha_base52(conf::size_max);

  def<"recur(...)"> recur = [&](va args) {
    return def<"x(i, _, ...)">{[&](arg i, arg _0, va args) {
      return if_(if_(eqz(i), pp::tup(1), pp::tup(is_none(args))), pp::tup(0, _0),
                 pp::tup(dec(i), args));
    }}(args);
  };

  def<"x(...)"> x = [&](va args) { return args; };

  def<"res(...)"> res = [&](va args) {
    return def<"x(i, _, ...)">{[&](arg, arg _0, va) { return x(esc + " " + tup(_0)); }}(
        args);
  };

  return res(meta_recur(x, first(args), recur, args));
});

} // namespace api

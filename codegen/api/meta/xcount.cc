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

#include "meta.h"

namespace api {

using namespace codegen;

decltype(xcount) xcount = NIFTY_DEF(xcount, [&](va args) {
  docs << "extracts the result from an " + xtrace + " expression."
       << "expansion count must be no greater than " + uint_max_s + "."
       << ""
       << "ignores the expansion required to read the result;"
       << "result ranges from 0 to " + uint_max_s + ".";

  tests << xcount(xtrace)                = "0" >> docs;
  tests << xcount(esc(xtrace))           = "1" >> docs;
  tests << xcount(esc(esc(xtrace)))      = "2" >> docs;
  tests << xcount(esc(esc(esc(xtrace)))) = "3" >> docs;

  def<"res(_, __, ...)"> res = [&](arg, arg, va args) {
    return size(args);
  };

  def a = def{detail::xtrace_a + "(...)"} = [&](va args) {
    return res(args + ".");
  };

  def{detail::xtrace_b + "(...)"} = [&](va args) {
    return res(args + ".");
  };

  return def<"x(...)">{[&](va args) {
    return pp::cat(utl::slice(a, -((std::string const&)detail::xtrace_a).size()), args);
  }}(args);
});

} // namespace api

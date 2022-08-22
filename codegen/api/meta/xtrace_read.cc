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

decltype(xtrace_read) xtrace_read = NIFTY_DEF(xtrace_read, [&](va args) {
  docs << "measures an xtrace expr to determine the number of expansions it experienced."
       << "ignores the expansion required to read the result."
       << ""
       << "fails if xtrace is not a valid xtrace expression."
       << size + " will fail if the xtrace expression is too large.";

  tests << xtrace_read(xtrace)                              = "0u" >> docs;
  tests << xtrace_read(x(xtrace))                           = "1u" >> docs;
  tests << xtrace_read(x(x(xtrace)))                        = "2u" >> docs;
  tests << xtrace_read(x(x(x(xtrace))))                     = "3u" >> docs;
  tests << xtrace_read(xtrace_expected(conf::size_max - 1)) = size_max_s;

  def detect_a = def{"detect_\\" + detail::xtrace_a + "(...)"} = [&] { return ""; };
  def{"detect_\\" + detail::xtrace_b + "(...)"}                = [&] { return ""; };

  def<"res(_, ...)"> res = [&](arg, va args) { return sizeof_(args); };

  def a = def{"\\" + detail::xtrace_a + "(_, ...)"} = [&](arg, va args) {
    return res(args + ".");
  };
  def{"\\" + detail::xtrace_b + "(_, ...)"} = [&](arg, va args) {
    return res(args + ".");
  };

  def<"\\0(err, ...)"> _0 = [&](arg err, va) { return fail(err); };
  def<"\\1(err, ...)">{}  = [&](arg, va args) {
    return pp::cat(utl::slice(a, -((std::string const&)detail::xtrace_a).size()), args);
  };

  return pp::call(
      cat(utl::slice(_0, -1),
          is_none(
              cat(utl::slice(detect_a, -((std::string const&)detail::xtrace_a).size()),
                  args))),
      str("[" + xtrace_read + "] invalid xtrace expr : " + args), args);
});

} // namespace api

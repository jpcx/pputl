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

decltype(xct_size) xct_size = NIFTY_DEF(xct_size, [&](va args) {
  docs << "measures an xct expr to determine the number of expansions it experienced."
       << "ignores the expansion required to read the result."
       << ""
       << "fails if xct is not a valid xct expression."
       << size + " will fail if the xct expression is too large.";

  tests << xct_size(xct)                              = "0" >> docs;
  tests << xct_size(esc(xct))                         = "1" >> docs;
  tests << xct_size(esc(esc(xct)))                    = "2" >> docs;
  tests << xct_size(esc(esc(esc(xct))))               = "3" >> docs;
  tests << xct_size(xct_expected(conf::uint_max - 1)) = uint_max_s;

  def detect_a = def{"detect_" + detail::xct_a + "(...)"} = [&] {
    return "";
  };

  def{"detect_" + detail::xct_b + "(...)"} = [&] {
    return "";
  };

  def<"res(_, ...)"> res = [&](arg, va args) {
    return size(args);
  };

  def a = def{detail::xct_a + "(__, ...)"} = [&](arg, va args) {
    return res(args + " _");
  };

  def{detail::xct_b + "(__, ...)"} = [&](arg, va args) {
    return res(args + " _");
  };

  def<"0(err, ...)"> _0 = [&](arg err, va) {
    return fail(err);
  };

  def<"1(err, ...)">{} = [&](arg, va args) {
    return pp::cat(utl::slice(a, -((std::string const&)detail::xct_a).size()), args);
  };

  return pp::call(
      cat(utl::slice(_0, -1),
          is_none(cat(utl::slice(detect_a, -((std::string const&)detail::xct_a).size()), args))),
      istr("[" + xct_size + "] invalid xct expr : " + args), args);
});

} // namespace api

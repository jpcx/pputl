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
  docs << "measures an xct object to determine the number of expansions it experienced."
       << "expansion count must be no greater than " + uint_max_s + "."
       << ""
       << "ignores the expansion required to read the result;"
       << "result ranges from 0 to " + uint_max_s + "."
       << ""
       << "terminates expansion on invalid xct or too large.";

  tests << xct_size(xct)                                   = "0" >> docs;
  tests << xct_size(esc(xct))                              = "1" >> docs;
  tests << xct_size(esc(esc(xct)))                         = "2" >> docs;
  tests << xct_size(esc(esc(esc(xct))))                    = "3" >> docs;
  tests << str(xct_size(xct))                              = pp::str(0) >> docs;
  tests << str(xct_size("foo"))                            = pp::str(xct_size("foo")) >> docs;
  tests << str(xct_size(xct_expected(conf::uint_max - 1))) = pp::str(conf::uint_max);
  tests << str(xct_size(xct_expected(conf::uint_max))) =
      pp::str(xct_size(xct_expected(conf::uint_max)));

  def detect_a = def{"detect_" + detail::xct_a + "(...)"} = [&] {
    return "";
  };

  def{"detect_" + detail::xct_b + "(...)"} = [&] {
    return "";
  };

  def<"res(pre, va, _, ...)"> res = [&](arg pre, arg va_, arg, va args) {
    def<"0(pre, va, sz)"> _0 = [&](arg pre, arg va_, arg) {
      return xct_size(pp::call(pp::cat(utl::slice(detail::xct_a, -1), pre), rest(items(va_))));
    };

    def<"1(pre, va, sz)">{} = [&](arg, arg, arg sz) {
      return sz;
    };

    return def<"x(pre, va, sz)">{[&](arg pre, arg va_, arg sz) {
      return pp::call(cat(utl::slice(_0, -1), is_uint(sz)), pre, va_, sz);
    }}(pre, va_, size(args));
  };

  def a = def{detail::xct_a + "(__, ...)"} = [&](arg, va args) {
    return res('A', pp::tup(args), args + " _");
  };

  def{detail::xct_b + "(__, ...)"} = [&](arg, va args) {
    return res('B', pp::tup(args), args + " _");
  };

  def<"0(...)"> _0 = [&](va args) {
    return xct_size(args);
  };

  def<"1(...)">{} = [&](va args) {
    return pp::cat(utl::slice(a, -((std::string const&)detail::xct_a).size()), args);
  };

  return pp::call(
      cat(utl::slice(_0, -1),
          is_none(cat(utl::slice(detect_a, -((std::string const&)detail::xct_a).size()), args))),
      args);
});

} // namespace api

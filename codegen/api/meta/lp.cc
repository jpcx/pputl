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

decltype(lp) lp = NIFTY_DEF(lp, [&](va args) {
  docs << "hides a left paren behind n secondary expansions.";

  def<"a(n)"> a;
  def<"b(n)"> b;

  tests << xstr(lp(1)) = pp::str(b + " ( 0 )") >> docs;
  tests << xstr(lp(2)) = pp::str(b + " ( 1 )") >> docs;
  tests << xstr(lp(3)) = pp::str(b + " ( 2 )") >> docs;

  a = [&](arg n) {
    def<"0(n)"> _0 = [&](arg n) {
      return b + " " + impl::lp + " " + dec(n) + " " + impl::rp;
    };
    def<"1(n)">{} = [&](arg) { return impl::lp; };
    return pp::call(cat(utl::slice(_0, -1), eqz(n)), n);
  };

  b = [&](arg n) {
    def<"0(n)"> _0 = [&](arg n) {
      return a + " " + impl::lp + " " + dec(n) + " " + impl::rp;
    };
    def<"1(n)">{} = [&](arg) { return impl::lp; };
    return pp::call(cat(utl::slice(_0, -1), eqz(n)), n);
  };

  return def<"o(n)">{[&](arg n) { return a(n); }}(idec(default_(0, args)));
});

} // namespace api

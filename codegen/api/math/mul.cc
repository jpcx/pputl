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

#include "math.h"

namespace api {

using namespace codegen;

decltype(mul) mul = NIFTY_DEF(mul, [&](va args) {
  docs << "numeric multiplication with overflow."
       << "" << impl::arith_rules;

  tests << mul("0, 0") = "0" >> docs;
  tests << mul("0, 1") = "0" >> docs;
  tests << mul("1, 1") = "1" >> docs;
  tests << mul("1, 2") = "2" >> docs;
  tests << mul("2, 2") = "4" >> docs;
  tests << mul(neg(2), 2) =
      ("0x" + utl::cat(svect{conf::word_size - 1, "F"}) + "C") >> docs;
  tests << mul(2, neg(2))      = ("0x" + utl::cat(svect{conf::word_size - 1, "F"}) + "C");
  tests << mul(neg(2), neg(2)) = ("0x" + utl::cat(samp::h4)) >> docs;
  tests << mul(uint_max_s, 1)  = uint_max_s >> docs;
  tests << mul(uint_max_s, uint_max_s) = "1u" >> docs;
  tests << mul(int_max_s, uint_max_s) =
      (std::to_string((conf::int_max * conf::uint_max) % (conf::uint_max + 1)) + "u")
      >> docs;

  def<"x(...)"> x = [&](va args) { return args; };

  def<"res(...)"> res = [&](va args) {
    return def<"o(a, b, s)">{[&](arg a, arg, arg s) { return add(a, s); }}(args);
  };

  def<"r(...)"> r = [&](va args) {
    def<"base(s, a)"> base = [&](arg s, arg) { return s; };
    def<"recr(s, a)"> recr = [&](arg s, arg a) { return add(s, a); };

    return def<"o(a, b, s)">{[&](arg a, arg b, arg s) {
      return bsll(a) + ", " + bsra(b) + ", "
           + pp::call(if_(bget(b, conf::bit_length - 1), recr, base), s, a);
    }}(args);
  };

  def<"eqz_b(a, b)"> eqz_b = [&](arg a, arg b) {
    return word(0, impl::xarithhint(typeof(a), typeof(b)));
  };

  def<"nez_b(a, b)"> nez_b = [&](arg a, arg b) {
    def<"\\00(ta, tb, a, b)"> _00 = [&](arg ta, arg tb, arg a, arg b) {
      return word(res(recur(x, log2(b), r, a, b, "0")), impl::arithhint(ta, tb));
    };
    def<"\\01(ta, tb, a, b)">{} = [&](arg ta, arg tb, arg a, arg b) {
      return def<"o(hint, a, b)">{[&](arg hint, arg a, arg b) {
        return neg(word(res(recur(x, log2(b), r, a, b, "0")), hint));
      }}(impl::arithhint(ta, tb), a, neg(b));
    };
    def<"\\10(ta, tb, a, b)">{} = [&](arg ta, arg tb, arg a, arg b) {
      return neg(
          word(res(recur(x, log2(b), r, neg(a), b, "0")), impl::arithhint(ta, tb)));
    };
    def<"\\11(ta, tb, a, b)">{} = [&](arg ta, arg tb, arg a, arg b) {
      return def<"o(hint, a, b)">{[&](arg hint, arg a, arg b) {
        return word(res(recur(x, log2(b), r, a, b, "0")), hint);
      }}(impl::arithhint(ta, tb), neg(a), neg(b));
    };

    return pp::call(xcat(utl::slice(_00, -2), xcat(ltz(a), ltz(b))), typeof(a), typeof(b),
                    a, b);
  };

  return pp::call(if_(eqz(rest(args)), eqz_b, nez_b), args);
});

} // namespace api

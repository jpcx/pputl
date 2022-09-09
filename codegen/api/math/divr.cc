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

#include "codegen.h"
#include "config.h"
#include "math.h"

namespace api {

using namespace codegen;

decltype(divr) divr = NIFTY_DEF(divr, [&](va args) {
  docs << "truncated division with remainder."
       << "fails on division by zero."
       << "" << impl::arith_rules << ""
       << "quotient and remainder may be formatted differently depending on sign.";

  tests << divr(10, 5)      = "2, 0" >> docs;
  tests << divr(11, 5)      = "2, 1" >> docs;
  tests << divr(12, 5)      = "2, 2" >> docs;
  tests << divr(13, 5)      = "2, 3" >> docs;
  tests << divr(14, 5)      = "2, 4" >> docs;
  tests << divr(neg(10), 5) = (neg(2) + ", " + "0") >> docs;
  tests << divr(neg(11), 5) = (neg(2) + ", " + neg(1)) >> docs;
  tests << divr(neg(12), 5) = (neg(2) + ", " + neg(2)) >> docs;
  tests << divr(neg(13), 5) = (neg(2) + ", " + neg(3)) >> docs;
  tests << divr(neg(14), 5) = (neg(2) + ", " + neg(4)) >> docs;
  tests << divr(10, neg(5)) = (neg(2) + ", 0") >> docs;
  tests << divr(11, neg(5)) = (neg(2) + ", 1") >> docs;
  tests << divr(12, neg(5)) = (neg(2) + ", 2") >> docs;
  tests << divr(13, neg(5)) = (neg(2) + ", 3") >> docs;
  tests << divr(14, neg(5)) = (neg(2) + ", 4") >> docs;
  tests << divr(neg(10), neg(5)) =
      ("0x" + utl::cat(samp::h2) + ", " + "0x" + utl::cat(samp::hmin)) >> docs;
  tests << divr(neg(11), neg(5)) = ("0x" + utl::cat(samp::h2) + ", " + neg(1)) >> docs;
  tests << divr(neg(12), neg(5)) = ("0x" + utl::cat(samp::h2) + ", " + neg(2)) >> docs;
  tests << divr(neg(13), neg(5)) = ("0x" + utl::cat(samp::h2) + ", " + neg(3)) >> docs;
  tests << divr(neg(14), neg(5)) = ("0x" + utl::cat(samp::h2) + ", " + neg(4)) >> docs;

  def<"x(...)"> x = [&](va args) {
    return args;
  };

  def<"r(...)"> r = [&](va args) {
    def<"norem(i, q, r, b)"> norem = [&](arg, arg q, arg r, arg) {
      return q + ", " + r;
    };
    def<"rem(i, q, r, b)"> rem = [&](arg i, arg q, arg r, arg b) {
      return bset(q, i, 1) + ", " + sub(r, b);
    };

    return def<"o(i, q, r, a, b)">{[&](arg i, arg q, arg r, arg a, arg b) {
      return def<"<o(i, q, r, a, b)">{[&](arg i, arg q, arg r, arg a, arg b) {
        return dec(i) + ", " + pp::call(if_(ge(r, b), rem, norem), i, q, r, b) + ", " + a
             + ", " + b;
      }}(i, q, bset(bsll(r), 0, bget(a, i)), a, b);
    }}(args);
  };

  def<"beqz(e, a, b)"> beqz = [&](arg e, arg, arg) {
    return fail(e);
  };
  def<"bnez(e, a, b)"> bnez = [&](arg, arg a, arg b) {
    def<"\\00(ta, tb, a, b)"> _00 = [&](arg ta, arg tb, arg a, arg b) {
      def<"res(...)"> res = [&](va args) {
        return def<"o(i, q, r, a, b, hint)">{[&](arg, arg q, arg r, arg, arg, arg hint) {
          return word(q, hint) + ", " + word(r, hint);
        }}(args);
      };
      return def<"o(hint, a, b, i)">{[&](arg hint, arg a, arg b, arg i) {
        return def<"<o(hint, a, b, i, iter)">{
            [&](arg hint, arg a, arg b, arg i, arg iter) {
              return res(recur(x, iter, r, i, "0", "0", a, b), hint);
            }}(hint, a, b, i, inc(i));
      }}(impl::arithhint(ta, tb), a, b, log2(a));
    };
    def<"\\01(ta, tb, a, b)">{} = [&](arg ta, arg tb, arg a, arg b) {
      def<"res(...)"> res = [&](va args) {
        return def<"o(i, q, r, a, b, hint)">{[&](arg, arg q, arg r, arg, arg, arg hint) {
          return word(neg(q), hint) + ", " + word(r, hint);
        }}(args);
      };
      return def<"o(hint, a, b, i)">{[&](arg hint, arg a, arg b, arg i) {
        return def<"<o(hint, a, b, i, iter)">{
            [&](arg hint, arg a, arg b, arg i, arg iter) {
              return res(recur(x, iter, r, i, "0", "0", a, b), hint);
            }}(hint, a, b, i, inc(i));
      }}(impl::arithhint(ta, tb), a, neg(b), log2(a));
    };
    def<"\\10(ta, tb, a, b)">{} = [&](arg ta, arg tb, arg a, arg b) {
      def<"res(...)"> res = [&](va args) {
        return def<"o(i, q, r, a, b, hint)">{[&](arg, arg q, arg r, arg, arg, arg hint) {
          return word(neg(q), hint) + ", " + word(neg(r), hint);
        }}(args);
      };
      return def<"o(hint, a, b)">{[&](arg hint, arg a, arg b) {
        return def<"<o(hint, a, b, i)">{[&](arg hint, arg a, arg b, arg i) {
          return def<"<o(hint, a, b, i, iter)">{
              [&](arg hint, arg a, arg b, arg i, arg iter) {
                return res(recur(x, iter, r, i, "0", "0", a, b), hint);
              }}(hint, a, b, i, inc(i));
        }}(hint, a, b, log2(a));
      }}(impl::arithhint(ta, tb), neg(a), b);
    };
    def<"\\11(ta, tb, a, b)">{} = [&](arg ta, arg tb, arg a, arg b) {
      def<"res(...)"> res = [&](va args) {
        return def<"o(i, q, r, a, b, hint)">{[&](arg, arg q, arg r, arg, arg, arg hint) {
          return word(q, hint) + ", " + word(neg(r), hint);
        }}(args);
      };
      return def<"o(hint, a, b)">{[&](arg hint, arg a, arg b) {
        return def<"<o(hint, a, b, i)">{[&](arg hint, arg a, arg b, arg i) {
          return def<"<o(hint, a, b, i, iter)">{
              [&](arg hint, arg a, arg b, arg i, arg iter) {
                return res(recur(x, iter, r, i, "0", "0", a, b), hint);
              }}(hint, a, b, i, inc(i));
        }}(hint, a, b, log2(a));
      }}(impl::arithhint(ta, tb), neg(a), neg(b));
    };

    return pp::call(xcat(utl::slice(_00, -2), xcat(ltz(a), ltz(b))), typeof(a), typeof(b),
                    a, b);
  };

  return pp::call(if_(eqz(rest(args)), beqz, bnez),
                  str(pp::str("[" + divr + "] division by zero") + " : " + args), args);
});

} // namespace api

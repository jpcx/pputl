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

decltype(recur_rp) recur_rp = NIFTY_DEF(recur_rp, [&](va args) {
  docs << "constructs an in-place recursive expression."
       << "the result must be expanded once to execute."
       << ""
       << "must be preceeded by an recur_lp expression with"
       << "the same value of n."
       << ""
       << "recur_lp repeats the function and open parens;"
       << "recur_rp repeats the close parens."
       << ""
       << "it is necessary to split the syntax of this"
       << "operation into two separate function calls, as"
       << "neither call can expand the target args without"
       << "risking expansion termination. this structure"
       << "allows recursive operations to process data"
       << "obtained from other recursive operations."
       << ""
       << "example:"
       << "  madd(a, b) = add(a, b), b"
       << "  mul(a, b)  = first(x(recur_lp(a, madd) 0, 4 recur_rp(a)))"
       << "  mul(2, 4) -> first(x(madd LP madd LP 0, 4 RP RP))"
       << "            -> first(madd(madd(0, 4)))"
       << "            -> first(madd(4, 4))"
       << "            -> first(8, 4)"
       << "            -> 8"
       << ""
       << "neither f nor the final expansion function may be"
       << "invoked anywhere within the recursive operation"
       << "(and both functions must be distinct)."
       << ""
       << "this operation can be used to perform an arbitrary"
       << "number of expansions by using two identity functions."
       << "this is necessary to implement mutual recursion.";

  tests << xstr(recur_lp(0, inc) + " 0 " + recur_rp(0)) = pp::str("0") >> docs;
  tests << xstr(recur_lp(1, inc) + " 0 " + recur_rp(1)) = pp::str(inc + " ( 0 )") >> docs;
  tests << xstr(recur_lp(2, inc) + " 0 " + recur_rp(2)) =
      pp::str(inc + " ( " + inc + " ( 0 ) )") >> docs;
  tests << xstr(recur_lp(3, inc) + " 0 " + recur_rp(3)) =
      pp::str(inc + " ( " + inc + " ( " + inc + " ( 0 ) ) )");
  tests << x(recur_lp(3, inc) + " 0 " + recur_rp(3)) = "3" >> docs;
  tests << x(recur_lp(2, dec) + " " + recur_lp(3, inc) + " 0 " + recur_rp(3) + " "
             + recur_rp(2))                          = "1";
  tests << xtrace_read(x(recur_lp(0, esc) + " " + xtrace + " " + recur_rp(0))) = "1u";
  tests << xtrace_read(x(recur_lp(1, esc) + " " + xtrace + " " + recur_rp(1))) = "2u";
  tests << xtrace_read(x(recur_lp(2, esc) + " " + xtrace + " " + recur_rp(2))) = "3u";
  tests << xtrace_read(x(recur_lp(3, esc) + " " + xtrace + " " + recur_rp(3))) = "4u";
  tests << xtrace_read(x(recur_lp(4, esc) + " " + xtrace + " " + recur_rp(4))) = "5u";
  tests << xtrace_read(x(recur_lp(5, esc) + " " + xtrace + " " + recur_rp(5))) = "6u";
  tests << xtrace_read(x(recur_lp(6, esc) + " " + xtrace + " " + recur_rp(6))) = "7u";
  tests << xtrace_read(x(recur_lp(conf::size_max - 1, esc) + " " + xtrace + " "
                         + recur_rp(conf::size_max - 1))) = size_max_s;

  std::array<def<>, (conf::size_max + 1) / 4> n{};

  n[0] = def{"\\0"} = [&] {
    return "";
  };
  n[1] = def{"\\1"} = [&] {
    return rp() + " " + rp() + " " + rp() + " " + rp();
  };
  n[2] = def{"\\2"} = [&] {
    return n[1] + " " + n[1];
  };
  n[3] = def{"\\3"} = [&] {
    return n[1] + " " + n[1] + " " + n[1];
  };

  for (std::size_t i = 4; i < n.size(); ++i) {
    n[i] = def{"\\" + std::to_string(i)} = [&] {
      return n[(i + 0) / 4] + " " + n[(i + 1) / 4] + " " + n[(i + 2) / 4] + " "
           + n[(i + 3) / 4];
    };
  }

  // o0: ""
  // o1: f( f( f( f(
  // o2: o1 o1
  // ...
  // c0: ""
  // c1: ) ) ) )
  // c2: c1 c1
  // ...
  //
  // -------------------| "f( "*(n%4) o[n/4] ... c[n/4] " )"*(n%4) |-------------------
  //
  // 0:                                              0/4=0 0%4=0 ->
  // 1: f( )                                         1/4=0 1%4=1 -> f( o0 c0 )
  // 2: f( f( ) )                                    2/4=0 2%4=2 -> f( f( o0 c0 ) )
  // 3: f( f( f( ) ) )                               3/4=0 3%4=3 -> f( f( f( o0 c0 ) ) )
  // 4: f( f( f( f( ) ) ) )                          4/4=1 8%4=0 -> o1 c1
  // 5: f( f( f( f( f( ) ) ) ) )                     5/4=1 5%4=1 -> f( o1 c1 )
  // 6: f( f( f( f( f( f( ) ) ) ) ) )                6/4=1 6%4=2 -> f( f( o1 c1 ) )
  // 7: f( f( f( f( f( f( f( ) ) ) ) ) ) )           7/4=1 7%4=3 -> f( f( f( o1 c1 ) ) )
  // 8: f( f( f( f( f( f( f( f( ) ) ) ) ) ) ) )      8/4=2 8%4=0 -> o2 c2
  // 9: f( f( f( f( f( f( f( f( f( ) ) ) ) ) ) ) ) ) 9/4=2 9%4=1 -> f( o2 c2 )

  return def<"o(n)">{[&](arg n_) {
    def<"\\0u(n)"> _0u = [&](arg n_) {
      return xcat(utl::slice(n[0], -1), n_);
    };
    def<"\\1u(n)">{} = [&](arg n_) {
      return xcat(utl::slice(n[0], -1), n_) + " " + rp();
    };
    def<"\\2u(n)">{} = [&](arg n_) {
      return xcat(utl::slice(n[0], -1), n_) + " " + rp() + " " + rp();
    };
    def<"\\3u(n)">{} = [&](arg n_) {
      return xcat(utl::slice(n[0], -1), n_) + " " + rp() + " " + rp() + " " + rp();
    };
    return pp::call(xcat(utl::slice(_0u, -2), band(n_, 0b11)), idec(bsrl(n_, 2)));
  }}(size(args, "UDEC"));
});

} // namespace api

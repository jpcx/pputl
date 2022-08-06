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

decltype(ropen) ropen = NIFTY_DEF(ropen, [&](va args) {
  docs << "constructs an in-place recursive expression."
       << "the result must be expanded once to execute."
       << ""
       << "must be followed by an rclose expression with"
       << "the same value of n."
       << ""
       << "ropen repeats the function and open parens;"
       << "rclose repeats the close parens."
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
       << "  mul(a, b)  = first(id(ropen(a, madd) 0, b rclose(a)))"
       << "  mul(2, 4) -> first(id(madd LP madd LP 0, 4 RP RP))"
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

  std::array<def<>, (conf::uint_max + 1) / 4> n{};

  n[0] = def{"0(f)"} = [&](arg) {
    return "";
  };

  if constexpr (conf::uint_max / 4 > 1)
    n[1] = def{"1(f)"} = [&](arg f) {
      return f + " " + lp() + " " + f + " " + lp() + " " + f + " " + lp() + " " + f + " " + lp();
    };

  if constexpr (conf::uint_max / 4 > 2)
    n[2] = def{"2(f)"} = [&](arg f) {
      return n[1](f) + " " + n[1](f);
    };

  if constexpr (conf::uint_max / 4 > 3)
    n[3] = def{"3(f)"} = [&](arg f) {
      return n[1](f) + " " + n[1](f) + " " + n[1](f);
    };

  for (std::size_t i = 4; i < n.size(); ++i) {
    n[i] = def{std::to_string(i) + "(f)"} = [&](arg f) {
      return n[(i + 0) / 4](f) + " " + n[(i + 1) / 4](f) + " " + n[(i + 2) / 4](f) + " "
           + n[(i + 3) / 4](f);
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

  return def<"x(n, f)">{[&](arg n_, arg f) {
    def<"0(n, f)"> _0 = [&](arg n_, arg f) {
      return pp::call(cat(utl::slice(n[0], -1), div2(div2(n_))), f);
    };
    def<"1(n, f)">{} = [&](arg n_, arg f) {
      return f + " " + lp() + " " + pp::call(cat(utl::slice(n[0], -1), div2(div2(dec(n_)))), f);
    };
    def<"2(n, f)">{} = [&](arg n_, arg f) {
      return f + " " + lp() + " " + f + " " + lp() + " "
           + pp::call(cat(utl::slice(n[0], -1), div2(div2(n_))), f);
    };
    def<"3(n, f)">{} = [&](arg n_, arg f) {
      return f + " " + lp() + " " + f + " " + lp() + " " + f + " " + lp() + " "
           + pp::call(cat(utl::slice(n[0], -1), div2(div2(n_))), f);
    };
    return pp::call(cat(utl::slice(_0, -1), mod4(n_)), n_, f);
  }}(args);
});

} // namespace api

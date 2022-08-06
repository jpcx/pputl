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

#include "compare.h"

namespace api {

using namespace codegen;

decltype(lt) lt = NIFTY_DEF(lt, [&](va args) {
  docs << "O(logn) uint less-than comparison.";

  tests << lt("0, 0")                                 = "0" >> docs;
  tests << lt("0, 1")                                 = "1" >> docs;
  tests << lt("0, 2")                                 = "1";
  tests << lt("0, 3")                                 = "1";
  tests << lt("1, 0")                                 = "0" >> docs;
  tests << lt("1, 1")                                 = "0" >> docs;
  tests << lt("1, 2")                                 = "1";
  tests << lt("1, 3")                                 = "1";
  tests << lt("2, 0")                                 = "0";
  tests << lt("2, 1")                                 = "0";
  tests << lt("2, 2")                                 = "0";
  tests << lt("2, 3")                                 = "1";
  tests << lt("3, 0")                                 = "0";
  tests << lt("3, 1")                                 = "0";
  tests << lt("3, 2")                                 = "0";
  tests << lt("3, 3")                                 = "0";
  tests << lt(0, conf::uint_max)                      = "1";
  tests << lt(0, conf::uint_max - 1)                  = "1";
  tests << lt(1, conf::uint_max)                      = "1";
  tests << lt(1, conf::uint_max - 1)                  = "1";
  tests << lt(conf::uint_max, 0)                      = "0";
  tests << lt(conf::uint_max, 0)                      = "0";
  tests << lt(conf::uint_max - 1, 0)                  = "0";
  tests << lt(conf::uint_max - 1, 0)                  = "0";
  tests << lt(conf::uint_max, 1)                      = "0";
  tests << lt(conf::uint_max, 1)                      = "0";
  tests << lt(conf::uint_max - 1, 1)                  = "0";
  tests << lt(conf::uint_max - 1, 1)                  = "0";
  tests << lt(conf::uint_max, conf::uint_max)         = "0";
  tests << lt(conf::uint_max, conf::uint_max - 1)     = "0";
  tests << lt(conf::uint_max - 1, conf::uint_max)     = "1";
  tests << lt(conf::uint_max - 1, conf::uint_max - 1) = "0";

  def<"x(...)"> x = [&](va args) {
    return args;
  };

  def<"recur(...)"> recur = [&](va args) {
    return def<"x(l, r, ml, mr)">{[&](arg l, arg r, arg ml, arg mr) {
      return div2(l) + ", " + div2(r) + ", " + ml + ", " + mr;
    }}(args);
  };

  def<"res(...)"> res = [&](va args) {
    return def<"x(l, r, ml, mr)">{[&](arg, arg r, arg ml, arg mr) {
      return or_(and_(nez(r), nez(dec(r))), and_(eqz(dec(r)), and_(not_(ml), mr)));
    }}(args);
  };

  def<"zero_l(l, r)"> zero_l = [&](arg, arg r) {
    return nez(r);
  };

  def<"pos_l(l, r)"> pos_l = [&](arg l, arg r) {
    return res(meta_recur(x, log2(l), recur, l, r, mod2(l), mod2(r)));
  };

  return pp::call(if_(eqz(first(args)), pp::tup(zero_l), pp::tup(pos_l)), args);
});

} // namespace api

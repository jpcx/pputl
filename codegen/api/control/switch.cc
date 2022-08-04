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

#include "control.h"

namespace api {

using namespace codegen;

#define PTL_SWITCH(/* cs: uint, cases: tuple... */...) /* -> ...cases[cs] */ \
  PTL_X(PTL_FIRST(__VA_ARGS__))                                              \
  (PPUTLSWITCH_A(PTL_UINT(PTL_FIRST(__VA_ARGS__)), PTL_REST(__VA_ARGS__))(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

/// mutually recursive branch selector B
#define PPUTLSWITCH_B(i, ...)                                                                  \
  PTL_IF(PTL_IF(PTL_EQZ(i), (1), (PTL_IS_NONE(PTL_REST(__VA_ARGS__)))), (PPUTLSWITCH_B_BREAK), \
         (PPUTLSWITCH_B_CONT))

/// B branches
#define PPUTLSWITCH_B_BREAK(i, _, ...) PTL_ITEMS(_)
#define PPUTLSWITCH_B_CONT(i, _, ...) \
  PPUTLSWITCH_A PTL_LP() PTL_DEC(i), __VA_ARGS__ PTL_RP()(PTL_DEC(i), __VA_ARGS__)

/// mutually recursive branch selector A
#define PPUTLSWITCH_A(i, ...)                                                                  \
  PTL_IF(PTL_IF(PTL_EQZ(i), (1), (PTL_IS_NONE(PTL_REST(__VA_ARGS__)))), (PPUTLSWITCH_A_BREAK), \
         (PPUTLSWITCH_A_CONT))

/// A branches
#define PPUTLSWITCH_A_BREAK(i, _, ...) PTL_ITEMS(_)
#define PPUTLSWITCH_A_CONT(i, _, ...) \
  PPUTLSWITCH_B PTL_LP() PTL_DEC(i), __VA_ARGS__ PTL_RP()(PTL_DEC(i), __VA_ARGS__)

decltype(switch_) switch_ = NIFTY_DEF(switch_, [&](va args) {
  docs << "conditionally expands items based on a uint."
       << "the final tuple is the default case.";

  std::vector<std::string> maxcases{};
  for (size_t i = 0; i <= conf::uint_max; ++i)
    maxcases.push_back(pp::tup(utl::alpha_base52(i)));
  auto maxcases_s = utl::cat(maxcases, ", ");

  tests << switch_(0, "(1)")                           = "1" >> docs;
  tests << switch_(1, "(1)")                           = "1" >> docs;
  tests << switch_(2, "(1)")                           = "1" >> docs;
  tests << switch_(0, "(1), (2)")                      = "1";
  tests << switch_(1, "(1), (2)")                      = "2" >> docs;
  tests << switch_(2, "(1), (2)")                      = "2" >> docs;
  tests << switch_(1, "(1), (2), (3)")                 = "2";
  tests << switch_(2, "(1), (2), (3)")                 = "3";
  tests << switch_(2, "(1), (2), (3, 4)")              = "3, 4" >> docs;
  tests << switch_(conf::uint_max, "(1), (2), (3, 4)") = "3, 4" >> docs;
  tests << switch_(0, maxcases_s)                      = "a";
  tests << switch_(1, maxcases_s)                      = "b";
  tests << switch_(conf::uint_max, maxcases_s)         = utl::alpha_base52(conf::uint_max);

  def<"a(i, ...)"> a;
  def<"b(i, ...)"> b;

  a = [&](arg i, va args) {
    docs << "recursive side A";
    def<"cont(i, _, ...)"> cont = [&](arg i, arg _0, va args) {
      return pp::call(b + " " + lp() + " " + dec(i) + ", " + args + " " + rp() + " ", dec(i),
                      rest(pp::tup(tuple(_0)), args));
    };
    def<"break(i, _, ...)"> break_ = [&](arg, arg _0, va) {
      return items(_0);
    };
    return if_(if_(eqz(i), pp::tup(1), pp::tup(is_none(rest(args)))), pp::tup(break_),
               pp::tup(cont));
  };

  b = [&](arg i, va args) {
    docs << "recursive side B";
    def<"cont(i, _, ...)"> cont = [&](arg i, arg _0, va args) {
      return pp::call(a + " " + lp() + " " + dec(i) + ", " + args + " " + rp() + " ", dec(i),
                      rest(pp::tup(tuple(_0)), args));
    };
    def<"break(i, _, ...)"> break_ = [&](arg, arg _0, va) {
      return items(_0);
    };
    return if_(if_(eqz(i), pp::tup(1), pp::tup(is_none(rest(args)))), pp::tup(break_),
               pp::tup(cont));
  };

  return pp::call(x(first(args)), pp::call(a(uint(first(args)), rest(args)), args));
});

} // namespace api

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
//         This program is free software: you can redistribute it and/or modify
//         it under the terms of the GNU General Public License as published by
//         the Free Software Foundation, either version 3 of the License, or
//         (at your option) any later version.
//
//         This program is distributed in the hope that it will be useful,
//         but WITHOUT ANY WARRANTY; without even the implied warranty of
//         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//         GNU General Public License for more details.
//                                                                            //
//  You should have received a copy of the GNU General Public License        ///
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.  ////
///////////////////////////////////////////////////////////////////////////// */

#include "range.h"

namespace api {

using namespace codegen;

decltype(bisect) bisect = NIFTY_DEF(bisect, [&](va args) {
  docs << "splits a range in two given an index."
       << "index may exceed the range size."
       << ""
       << "returns:"
       << "  (1) head: a tuple of the items from 0 to end_idx"
       << "  (2) tail: a tuple of the items from end_idx to size"
       << "  (3) type: 1 if two empty elements were split, else 0"
       << ""
       << "the bisection type is essential for information"
       << "perservation when working with empty elements,"
       << "but can ignored in other cases. for example:"
       << ""
       << "  bisect(0, ())     -> (), (), 0"
       << "  bisect(1, (, ))   -> (), (), 1"
       << "  bisect(0, (a))    -> (a), (), 0"
       << "  bisect(1, (a, b)) -> (a), (b), 0";

  /* auto                    bigargs = utl::alpha_base52_seq(conf::uint_max + 3); */
  /* std::deque<std::string> pf_bigargs{bigargs.begin(), bigargs.end()}; */
  /* pf_bigargs.pop_front(); */
  /* auto        biggerargs   = utl::alpha_base52_seq(conf::uint_max * 2 + 3); */
  /* auto        bigargs_s    = utl::cat(bigargs, ", "); */
  /* auto        pf_bigargs_s = utl::cat(pf_bigargs, ", "); */
  /* auto        biggerargs_s = utl::cat(biggerargs, ", "); */
  /* std::string biggerargs_back{}; */
  /* for (size_t i = conf::uint_max; i < biggerargs.size() - 1; ++i) */
  /*   biggerargs_back += biggerargs[i] + ", "; */
  /* biggerargs_back += biggerargs[biggerargs.size() - 1]; */

  // tests << bisect(0, "(a)")       = "(), (a), 0" >> docs;
  // tests << bisect(1, "(a)")       = "(a), (), 0" >> docs;
  // tests << bisect(0, "()")        = "(), (), 0" >> docs;
  // tests << bisect(0, "(a, b)")    = "(), (a, b), 0";
  // tests << bisect(1, "(a, b)")    = "(a), (b), 0";
  // tests << bisect(1, "(a, b, c)") = "(a), (b, c), 0" >> docs;
  // tests << bisect(2, "(a, b, c)") = "(a, b), (c), 0" >> docs;
  // tests << bisect(3, "(a, b, c)") = "(a, b, c), (), 0";
  // tests << bisect(0, "(, )")      = "(), (,), 0" >> docs;
  // tests << bisect(1, "(, )")      = "(), (), 1" >> docs;
  // tests << bisect(2, "(, )")      = "(, ), (), 0" >> docs;
  // tests << bisect(0, "(, , )")    = "(), (, ,), 0";
  // tests << bisect(1, "(, , )")    = "(), (,), 1";
  // tests << bisect(2, "(, , )")    = "(, ), (), 1" >> docs;
  // tests << bisect(3, "(, , )")    = "(, , ), (), 0";
  // tests << bisect(0, "(a, )")     = "(), (a,), 0";
  // tests << bisect(1, "(a, )")     = "(a), (), 1";
  // tests << bisect(2, "(a, )")     = "(a, ), (), 0";
  // tests << bisect(0, "(a, , )")   = "(), (a, ,), 0";
  // tests << bisect(1, "(a, , )")   = "(a), (,), 1";
  // tests << bisect(2, "(a, , )")   = "(a, ), (), 1";
  // tests << bisect(3, "(a, , )")   = "(a, , ), (), 0";
  // tests << bisect(0, "(, a)")     = "(), (, a), 0";
  // tests << bisect(1, "(, a)")     = "(), (a), 1";
  // tests << bisect(2, "(, a)")     = "(, a), (), 0";
  // tests << bisect(0, "(, a, )")   = "(), (, a,), 0";
  // tests << bisect(1, "(, a, )")   = "(), (a,), 1";
  // tests << bisect(2, "(, a, )")   = "(, a), (), 1";
  // tests << bisect(3, "(, a, )")   = "(, a, ), (), 0";
  // tests << bisect(0, "(, , a)")   = "(), (, , a), 0";
  // tests << bisect(1, "(, , a)")   = "(), (, a), 1";
  // tests << bisect(2, "(, , a)")   = "(, ), (a), 1";
  // tests << bisect(3, "(, , a)")   = "(, , a), (), 0";
  
  /* tests << bisect(0, bigargs_s) = tup() + ", " + tup(bigargs_s) + ", 0"; */
  /* tests << bisect(1, bigargs_s) = tup(bigargs[0]) + ", " + tup(pf_bigargs_s) + ", 0";
   */
  /* tests << bisect(conf::size_max, biggerargs_s) = */
  /*     tup(utl::cat(utl::alpha_base52_seq(conf::uint_max), ", ")) + ", " */
  /*     + tup(biggerargs_back) + ", 0"; */

  return args;
});

} // namespace api

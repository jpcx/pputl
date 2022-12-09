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

#include "range.h"

namespace api {

using namespace codegen;

decltype(set_item) set_item = NIFTY_DEF(set_item, [&](va args) {
  docs << "set a tuple item to a new value. index must be within [-size, size).";

  // auto bigargs = utl::alpha_base52_seq(conf::size_max);
  // auto pb_args = svect{bigargs.begin(), bigargs.end() - 1};
  // auto pf_args = svect{bigargs.begin() + 1, bigargs.end() - 1};

  // auto bigargs_s = utl::cat(bigargs, ", ");
  // auto pb_args_s = utl::cat(pb_args, ", ");
  // auto pf_args_s = utl::cat(pf_args, ", ");

  // tests << set("(a, b)", 0, 'c')                            = "(c, b)" >> docs;
  // tests << set("(a, b)", 1, 'd')                            = "(a, d)" >> docs;
  // tests << set("(a, b)", neg(1), 'e')                       = "(a, e)" >> docs;
  // tests << set("(2)", 0, 1)                                 = "(1)";
  // tests << set("(2, 3)", 0, 1)                              = "(1, 3)";
  // tests << set("(2, 3)", 1, 1)                              = "(2, 1)";
  // tests << set("(2, 3, 4)", 0, 1)                           = "(1, 3, 4)";
  // tests << set("(2, 3, 4)", 1, 1)                           = "(2, 1, 4)";
  // tests << set("(2, 3, 4)", 2, 1)                           = "(2, 3, 1)";
  // clang-format off
  // tests << set(pp::tup(bigargs_s), 0, '.')                  = pp::tup("., " + pf_args_s);
  // tests << set(pp::tup(bigargs_s), conf::size_max - 1, '.') = pp::tup(pb_args_s + ", .");
  // tests << set("(, )", 0, '.')                              = "(.,)";
  // tests << set("(, , )", 0, '.')                            = "(., ,)";
  // tests << set("(, )", 1, '.')                              = "(, .)";
  // tests << set("(, , )", 1, '.')                            = "(, .,)";
  // tests << set("(, , )", 2, '.')                            = "(,, .)";
  // tests << set("(a, )", 0, '.')                             = "(.,)";
  // tests << set("(a, , )", 0, '.')                           = "(., ,)";
  // tests << set("(a, )", 1, '.')                             = "(a, .)";
  // tests << set("(a, , )", 1, '.')                           = "(a, .,)";
  // tests << set("(a, , )", 2, '.')                           = "(a,, .)";
  // tests << set("(, a)", 0, '.')                             = "(., a)";
  // tests << set("(, a, )", 0, '.')                           = "(., a,)";
  // tests << set("(, a)", 1, '.')                             = "(, .)";
  // tests << set("(, a, )", 1, '.')                           = "(, .,)";
  // tests << set("(, a, )", 2, '.')                           = "(, a, .)";
  // tests << set("(, , a)", 0, '.')                           = "(., , a)";
  // tests << set("(, , a)", 1, '.')                           = "(, ., a)";
  // tests << set("(, , a)", 2, '.')                           = "(,, .)";
  // clang-format on

  return args;
  /* def<"ins(...)"> ins = [&](va args) { */
  /*   return def<"x(val, head, _, spl_t0, __, tail, spl_t1)">{ */
  /*       [&](arg val, arg head, arg _, arg spl_t0, arg __, arg tail, arg spl_t1) { */
  /*         return join(pp::tup(join(head, pp::tup(val), spl_t0)), xxn.n(3, tail),
   * spl_t1); */
  /*       }}(args); */
  /* }; */

  /* return def<"x(ofs, val, ...)">{[&](arg ofs, arg val, va args) { */
  /*   return ins(xxn.n(3, val), split(ofs, args), split(inc(xxn.n(2, ofs)), args)); */
  /* }}(args); */
});

} // namespace api

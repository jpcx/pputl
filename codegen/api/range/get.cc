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

decltype(tup_get) tup_get = NIFTY_DEF(tup_get, [&](va args) {
  docs << "get a tuple item. index must be within [-size, size).";

  auto bigargs   = utl::alpha_base52_seq(conf::size_max);
  auto bigargs_s = pp::tup(bigargs);

  tests << tup_get("(a)", 0)                      = "a" >> docs;
  tests << tup_get("(a, b)", 0)                   = "a" >> docs;
  tests << tup_get("(a, b)", 1)                   = "b" >> docs;
  tests << tup_get("(1, 2, 3)", 1)                = "2";
  tests << tup_get("(1, 2, 3)", 2)                = "3";
  tests << tup_get("(a, b, c)", neg(2))           = "b" >> docs;
  tests << tup_get("(a, b, c)", neg(3))           = "a" >> docs;
  tests << tup_get(bigargs_s, 0)                  = bigargs[0];
  tests << tup_get(bigargs_s, 1)                  = bigargs[1];
  tests << tup_get(bigargs_s, conf::size_max - 2) = bigargs[conf::size_max - 2];
  tests << tup_get(bigargs_s, conf::size_max - 1) = bigargs[conf::size_max - 1];
  tests << tup_get("(, )", 0)                     = "";
  tests << tup_get("(, , )", 0)                   = "";
  tests << tup_get("(, )", 1)                     = "";
  tests << tup_get("(, , )", 1)                   = "";
  tests << tup_get("(, , )", 2)                   = "";
  tests << tup_get("(a, )", 0)                    = "a";
  tests << tup_get("(a, , )", 0)                  = "a";
  tests << tup_get("(a, )", 1)                    = "";
  tests << tup_get("(a, , )", 1)                  = "";
  tests << tup_get("(a, , )", 2)                  = "";
  tests << tup_get("(, a)", 0)                    = "";
  tests << tup_get("(, a, )", 0)                  = "";
  tests << tup_get("(, a)", 1)                    = "a";
  tests << tup_get("(, a, )", 1)                  = "a";
  tests << tup_get("(, a, )", 2)                  = "";
  tests << tup_get("(, , a)", 0)                  = "";
  tests << tup_get("(, , a)", 1)                  = "";
  tests << tup_get("(, , a)", 2)                  = "a";

  def<"pass(e, _, ...)"> pass = [&](arg, arg first, va) {
    return first;
  };

  def<"fail(e, ...)"> fail = [&](arg e, va) {
    return api::fail(e);
  };

  def<"chk(e, type, ...)"> chk = [&](arg e, arg type, va args) {
    return pp::call(if_(and_(is_none(args), not_(type)), fail, pass), e, args);
  };

  return def<"o(...)">{[&](va args) {
    return def<"<o(e, head, tail, type)">{[&](arg e, arg, arg tail, arg type) {
      return chk(e, type, esc + " " + tail);
    }}(args);
  }}(error(tup_get, "invalid index", args), bisect(args));
});

} // namespace api

////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////

#include "traits.h"

namespace api {

using namespace codegen;

decltype(qty) qty = NIFTY_DEF(qty, [&](va args) {
  docs << "computes the quantity of args (size <= " + uint_max_s + ")."
       << "creates an invalid uint if too large.";

  tests << qty()                                = "0" >> docs;
  tests << qty("a")                             = "1" >> docs;
  tests << qty("a, b")                          = "2" >> docs;
  tests << qty("a, b, c")                       = "3";
  tests << qty(utl::base10_seq(conf::uint_max)) = uint_max_s;
  tests << qty(", ")                            = "2" >> docs;
  tests << qty(", , ")                          = "3";
  tests << qty("a, ")                           = "2";
  tests << qty("a, , ")                         = "3";
  tests << qty(", a")                           = "2";
  tests << qty(", a, ")                         = "3";
  tests << qty(", , a")                         = "3";

  std::array<def<>, conf::uint_max> verify{};

  verify[0] = def{"verify_0(_, ...)"} = [&](arg, va) {
    return pp::va_opt(uint("too many args passed to " + qty));
  };
  for (size_t i = 1; i < verify.size(); ++i)
    verify[i] = def{"verify_" + utl::to_string(i) + "(_, ...)"} = [&](arg, va args) {
      return pp::va_opt(verify[i - 1](args));
    };

  def read =
      def{"read(" + utl::cat(utl::alpha_base52_seq(conf::uint_max), ", ") + ", sz, ...)"} = [&](pack args) {
        return args[args.size() - 2] + ", " + verify.back()(args.back());
      };

  auto rseq = utl::base10_seq(conf::uint_max + 1);
  std::ranges::reverse(rseq);
  return first(read(args + " " + pp::va_opt(", ") + " " + utl::cat(rseq, ", ")));
});

} // namespace api

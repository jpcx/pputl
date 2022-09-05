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
#include "impl/numeric.h"

namespace api {
namespace impl {

using namespace codegen;

decltype(neg) neg = NIFTY_DEF(neg, [&](arg n) {
  docs << "[internal] integral negation.";

  def<"x(...)"> x = [&](va args) { return args; };

  auto p = utl::alpha_base52_seq(conf::word_size);
  for (auto&& v : p)
    if (v == "u") {
      v = "_u";
      break;
    }

  def uhex = def{"uhex(" + utl::cat(p, ", ") + ")"} = [&](pack args) {
    return pp::cat("0x", pp::cat(args), "u");
  };

  return inc(impl::uhex(impl::uhex(x(uhex + " " + n), "BNOT"), "UTUP"));
});

} // namespace impl
} // namespace api

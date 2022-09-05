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

#include "impl/traits.h"

namespace api {
namespace impl {

using namespace codegen;

decltype(uhex_prefix) uhex_prefix = NIFTY_DEF(uhex_prefix);

decltype(uhex) uhex = NIFTY_DEF(uhex, [&](arg v, arg t) {
  docs << "[internal] uhex traits";

  std::array<codegen::def<>, codegen::conf::uint_max + 1> uhexs;

  {
    std::size_t i = 0;
    for (; i < uhexs.size() - 1; ++i) {
      auto bin = detail::binary(i);
      uhexs[i] = def{"\\" + detail::uhex(bin)} = [&] {
        return utl::cat(std::array{detail::udec(i), detail::utup(bin), detail::ihex(bin),
                                   detail::icast(bin, i), detail::iltz(i),
                                   detail::bnot(bin)},
                        ", ");
      };
    }
    {
      auto bin = detail::binary(i);
      uhexs[i] = def{"\\" + detail::uhex(bin)} = [&] {
        docs << "UDEC, UTUP, IHEX, ICAST, ILTZ, BNOT";
        return utl::cat(std::array{detail::udec(i), detail::utup(bin), detail::ihex(bin),
                                   detail::icast(bin, i), detail::iltz(i),
                                   detail::bnot(bin)},
                        ", ");
      };
    }
  }

  uhex_prefix = utl::slice(uhexs[0], -(conf::word_size + 3));

  def<"\\UDEC(u, ...) -> udec"> udec             = [&](pack args) { return args[0]; };
  def<"\\UTUP(u, x, ...) -> utup">{}             = [&](pack args) { return args[1]; };
  def<"\\IHEX(u, x, h, ...) -> ihex">{}          = [&](pack args) { return args[2]; };
  def<"\\ICAST(u, x, h, i, ...) -> idec|ihex">{} = [&](pack args) { return args[3]; };
  def<"\\ILTZ(u, x, h, i, z, ...) -> bool">{}    = [&](pack args) { return args[4]; };
  def<"\\BNOT(u, x, h, i, z, b) -> uhex">{}      = [&](pack args) { return args[5]; };

  return def<"o(t, ...)">{[&](arg t, va row) {
    return pp::call(pp::cat(utl::slice(udec, -4), t), row);
  }}(t, xcat(uhex_prefix, v));
});

} // namespace impl
} // namespace api

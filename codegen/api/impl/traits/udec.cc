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

decltype(udec) udec = NIFTY_DEF(udec, [&](arg v, arg t) {
  docs << "[internal] udec traits";

  std::array<codegen::def<>, codegen::conf::uint_max + 1> udecs;

  {
    std::size_t i = 0;
    for (; i < udecs.size() - 1; ++i) {
      auto bin = detail::binary(i);
      udecs[i] = def{std::to_string(i) + "\\u"} = [&] {
        return utl::cat(std::array{detail::uhex(bin), detail::log2(i), detail::sqrt(i),
                                   detail::fact(i)},
                        ", ");
      };
    }
    {
      auto bin = detail::binary(i);
      udecs[i] = def{std::to_string(i) + "\\u"} = [&] {
        docs << "UHEX, LOG2, SQRT, FACT";
        return utl::cat(std::array{detail::uhex(bin), detail::log2(i), detail::sqrt(i),
                                   detail::fact(i)},
                        ", ");
      };
    }
  }

  def<"\\IS(_, ...) -> bool"> is = [&](arg, va) {
    def<"\\0"> _0 = [&] { return "0"; };
    def<"\\01">{} = [&] { return "1"; };
    return pp::cat(_0, pp::va_opt("1"));
  };

  def<"\\UHEX(u, ...) -> uhex">{}          = [&](pack args) { return args[0]; };
  def<"\\LOG2(u, l, ...) -> udec">{}       = [&](pack args) { return args[1]; };
  def<"\\SQRT(u, l, s, ...) -> udec">{}    = [&](pack args) { return args[2]; };
  def<"\\FACT(u, l, s, f) -> (udec...)">{} = [&](pack args) { return args[3]; };

  return def<"o(t, ...)">{[&](arg t, va row) {
    return pp::call(pp::cat(utl::slice(is, -2), t), row);
  }}(t, cat(utl::slice(udecs[0], -2), v));
});

} // namespace impl
} // namespace api

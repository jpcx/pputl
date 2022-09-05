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

decltype(udec_prefix) udec_prefix = NIFTY_DEF(udec_prefix);

constexpr auto size_lt_max = conf::word_size > 2 and conf::cpp20_arglimit;

decltype(udec) udec = NIFTY_DEF(udec, [&](arg v, arg t) {
  docs << "[internal] udec traits";

  std::array<codegen::def<>, codegen::conf::uint_max + 1> udecs;

  if constexpr (size_lt_max) {
    {
      std::size_t i = 0;
      for (; i < udecs.size() - 1; ++i) {
        auto bin = detail::binary(i);
        udecs[i] = def{std::to_string(i) + "\\u"} = [&] {
          return utl::cat(std::array{detail::uhex(bin), detail::isize(i),
                                     detail::usize(i), detail::iidx(i), detail::uidx(i),
                                     detail::log2(i), detail::sqrt(i), detail::fact(i)},
                          ", ");
        };
      }
      {
        auto bin = detail::binary(i);
        udecs[i] = def{std::to_string(i) + "\\u"} = [&] {
          docs << "UHEX, ISIZE, USIZE, IIDX, UIDX, LOG2, SQRT, FACT";
          return utl::cat(std::array{detail::uhex(bin), detail::isize(i),
                                     detail::usize(i), detail::iidx(i), detail::uidx(i),
                                     detail::log2(i), detail::sqrt(i), detail::fact(i)},
                          ", ");
        };
      }
    }

    udec_prefix = utl::slice(udecs[0], -2);

    def<"\\UHEX(u, ...) -> uhex"> uhex              = [&](pack args) { return args[0]; };
    def<"\\ISIZE(u, is, ...) -> bool">{}            = [&](pack args) { return args[1]; };
    def<"\\USIZE(u, is, us, ...) -> bool">{}        = [&](pack args) { return args[2]; };
    def<"\\IIDX(u, is, us, ii, ...) -> bool">{}     = [&](pack args) { return args[3]; };
    def<"\\UIDX(u, is, us, ii, ui, ...) -> bool">{} = [&](pack args) { return args[4]; };
    def<"\\LOG2(u, is, us, ii, ui, l, ...) -> idec">{} = [&](pack args) {
      return args[5];
    };
    def<"\\SQRT(u, is, us, ii, ui, l, sq, ...) -> idec">{} = [&](pack args) {
      return args[6];
    };
    def<"\\FACT(u, is, us, ii, ui, l, sq, ...) -> idec...">{} = [&](pack args) {
      return args[7];
    };

    return def<"o(t, ...)">{[&](arg t, va row) {
      return pp::call(pp::cat(utl::slice(uhex, -4), t), row);
    }}(t, xcat(udec_prefix, v));
  } else {
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
          docs << "UHEX, IIDX, UIDX, LOG2, SQRT, FACT";
          return utl::cat(std::array{detail::uhex(bin), detail::log2(i), detail::sqrt(i),
                                     detail::fact(i)},
                          ", ");
        };
      }
    }

    udec_prefix = utl::slice(udecs[0], -2);

    def<"\\UHEX(u, ...) -> uhex"> uhex             = [&](pack args) { return args[0]; };
    def<"\\IIDX(u, ii, ...) -> bool">{}            = [&](pack args) { return args[1]; };
    def<"\\UIDX(u, ii, ui, ...) -> bool">{}        = [&](pack args) { return args[2]; };
    def<"\\LOG2(u, ii, ui, l, ...) -> idec">{}     = [&](pack args) { return args[3]; };
    def<"\\SQRT(u, ii, ui, l, sq, ...) -> idec">{} = [&](pack args) { return args[4]; };
    def<"\\FACT(u, ii, ui, l, sq, ...) -> idec...">{} = [&](pack args) {
      return args[5];
    };

    return def<"o(t, ...)">{[&](arg t, va row) {
      return pp::call(pp::cat(utl::slice(uhex, -4), t), row);
    }}(t, xcat(udec_prefix, v));
  }
});

} // namespace impl
} // namespace api

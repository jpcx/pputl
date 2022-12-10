#ifndef CODEGEN_API_IMPL_HEX_TRAITS_CC
#define CODEGEN_API_IMPL_HEX_TRAITS_CC
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
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//                                                                            //
//  You should have received a copy of the GNU General Public License        ///
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.  ////
///////////////////////////////////////////////////////////////////////////// */

#include "codegen.h"

#include "lang/xcat.cc"

namespace codegen {
namespace api {

using namespace std;

namespace impl {

inline constexpr char const alpha[] = "0123456789ABCDEF";
static_assert(sizeof alpha == 17);

inline string
not_(size_t i) {
  return string{alpha[~i % 16]};
}

inline string
dec0(size_t i) {
  return "0, " + string{alpha[i]};
}

inline string
dec1(size_t i) {
  return string{i == 0 ? "1" : "0"} + ", " + string{i > 0 ? alpha[i - 1] : alpha[15]};
}

inline string
inc0(size_t i) {
  return "0, " + string{alpha[i]};
}

inline string
inc1(size_t i) {
  return string{i == 15 ? "1" : "0"} + ", " + string{i < 15 ? alpha[i + 1] : alpha[0]};
}

inline string
bits(size_t i) {
  switch (i) {
    case 0:
      return "0, 0, 0, 0";
    case 1:
      return "0, 0, 0, 1";
    case 2:
      return "0, 0, 1, 0";
    case 3:
      return "0, 0, 1, 1";
    case 4:
      return "0, 1, 0, 0";
    case 5:
      return "0, 1, 0, 1";
    case 6:
      return "0, 1, 1, 0";
    case 7:
      return "0, 1, 1, 1";
    case 8:
      return "1, 0, 0, 0";
    case 9:
      return "1, 0, 0, 1";
    case 10:
      return "1, 0, 1, 0";
    case 11:
      return "1, 0, 1, 1";
    case 12:
      return "1, 1, 0, 0";
    case 13:
      return "1, 1, 0, 1";
    case 14:
      return "1, 1, 1, 0";
    case 15:
      return "1, 1, 1, 1";
    default:
      throw logic_error{"bad index"};
  };
}

inline string
to_nybl(size_t i) {
  auto        b     = bits(i);
  static auto comma = regex{", ", regex_constants::optimize};
  return regex_replace(b, comma, "");
}

} // namespace impl

inline string hex_prefix{};

inline def<"impl_hex_traits(v, t: 0|1|...|E|F, NOT|DEC0|DEC1|INC0|INC1|NYBL|BITS)">
    hex_traits = [](arg v, arg t) {
      category = "impl";

      docs << "[internal] hex traits";

      array<def<>, 16> digits{};

      {
        size_t i = 0;
        for (; i < 16 - 1; ++i) {
          digits[i] = def{"\\" + string{impl::alpha[i]}} = [&] {
            return impl::not_(i) + ", " + impl::dec0(i) + ", " + impl::dec1(i) + ", "
                 + impl::inc0(i) + ", " + impl::inc1(i) + ", " + impl::to_nybl(i) + ", "
                 + impl::bits(i);
          };
        }
        digits[i] = def{"\\" + string{impl::alpha[i]}} = [&] {
          docs << "not, dec0carry, dec0, dec1carry, dec1, inc0carry, inc0, inc1carry, "
                  "inc1, "
                  "nybl, ...bits";
          return impl::not_(i) + ", " + impl::dec0(i) + ", " + impl::dec1(i) + ", "
               + impl::inc0(i) + ", " + impl::inc1(i) + ", " + impl::to_nybl(i) + ", "
               + impl::bits(i);
        };
      }

      hex_prefix = utl::slice(digits[0], -1);

      def<"\\NOT(n, ...) -> hex"> not_ = [&](pack args) {
        return args[0];
      };
      def<"\\DEC0(n, d0c, d0, ...) -> bool, hex">{} = [&](pack args) {
        return args[1] + ", " + args[2];
      };
      def<"\\DEC1(n, d0c, d0, d1c, d1, ...) -> bool, hex">{} = [&](pack args) {
        return args[3] + ", " + args[4];
      };
      def<"\\INC0(n, d0c, d0, d1c, d1, i0c, i0, ...) -> bool, hex">{} = [&](pack args) {
        return args[5] + ", " + args[6];
      };
      def<"\\INC1(n, d0c, d0, d1c, d1, i0c, i0, i1c, i1, ...) -> bool, hex">{} =
          [&](pack args) {
            return args[7] + ", " + args[8];
          };
      def<"\\NYBL(n, d0c, d0, d1c, d1, i0c, i0, i1c, i1, ny, ...) -> nybl">{} =
          [&](pack args) {
            return args[9];
          };
      def<"\\BITS(n, d0c, d0, d1c, d1, i0c, i0, i1c, i1, ny, ...) -> ...bool">{} =
          [&](pack args) {
            return args[10];
          };

      return def<"o(t, ...)">{[&](arg t, va row) {
        return pp::call(pp::cat(utl::slice(not_, -3), t), row);
      }}(t, xcat(hex_prefix, v));
    };

} // namespace api
} // namespace codegen

#endif

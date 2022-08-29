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

static constexpr char const alpha[] = "0123456789ABCDEF";
static_assert(sizeof alpha == 17);

static std::string
not_(std::size_t i) {
  return std::string{alpha[~i % 16]};
}

static std::string
dec0(std::size_t i) {
  return "0, " + std::string{alpha[i]};
}

static std::string
dec1(std::size_t i) {
  return std::string{i == 0 ? "1" : "0"} + ", "
       + std::string{i > 0 ? alpha[i - 1] : alpha[15]};
}

static std::string
inc0(std::size_t i) {
  return "0, " + std::string{alpha[i]};
}

static std::string
inc1(std::size_t i) {
  return std::string{i == 15 ? "1" : "0"} + ", "
       + std::string{i < 15 ? alpha[i + 1] : alpha[0]};
}

static std::string
to_nybl(std::size_t i) {
  auto        b     = detail::bits(i);
  static auto comma = std::regex{", ", std::regex_constants::optimize};
  return std::regex_replace(b, comma, "");
}

decltype(hex) hex = NIFTY_DEF(hex, [&](arg v, arg t) {
  docs << "[internal] hex traits";

  std::array<def<>, 16> digits{};

  {
    std::size_t i = 0;
    for (; i < 16 - 1; ++i) {
      digits[i] = def{"\\" + std::string{alpha[i]}} = [&] {
        return not_(i) + ", " + dec0(i) + ", " + dec1(i) + ", " + inc0(i) + ", " + inc1(i)
             + ", " + to_nybl(i) + ", " + detail::bits(i);
      };
    }
    digits[i] = def{"\\" + std::string{alpha[i]}} = [&] {
      docs << "not, dec0carry, dec0, dec1carry, dec1, inc0carry, inc0, inc1carry, inc1, "
              "nybl, ...bits";
      return not_(i) + ", " + dec0(i) + ", " + dec1(i) + ", " + inc0(i) + ", " + inc1(i)
           + ", " + to_nybl(i) + ", " + detail::bits(i);
    };
  }

  def<"\\IS(_, ...) -> bool"> is = [&](arg, va) {
    def<"\\0"> _0 = [&] { return "0"; };
    def<"\\01">{} = [&] { return "1"; };
    return pp::cat(_0, pp::va_opt("1"));
  };

  def<"\\NOT(n, ...) -> hex">{}                 = [&](pack args) { return args[0]; };
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
      [&](pack args) { return args[7] + ", " + args[8]; };
  def<"\\NYBL(n, d0c, d0, d1c, d1, i0c, i0, i1c, i1, ny, ...) -> nybl">{} =
      [&](pack args) { return args[9]; };
  def<"\\BITS(n, d0c, d0, d1c, d1, i0c, i0, i1c, i1, ny, ...) -> ...bool">{} =
      [&](pack args) { return args[10]; };

  return def<"o(t, ...)">{[&](arg t, va row) {
    return pp::call(pp::cat(utl::slice(is, -2), t), row);
  }}(t, xcat(utl::slice(digits[0], -1), v));
});

} // namespace impl
} // namespace api

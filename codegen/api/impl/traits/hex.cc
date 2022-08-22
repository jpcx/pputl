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
dec(std::size_t i) {
  return pp::tup<false>(i == 0, i > 0 ? alpha[i - 1] : alpha[15]);
}

static std::string
inc(std::size_t i) {
  return pp::tup<false>(i == 15, i < 15 ? alpha[i + 1] : alpha[0]);
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
        return not_(i) + ", " + dec(i) + ", " + inc(i) + ", " + to_nybl(i) + ", "
             + detail::bits(i);
      };
    }
    digits[i] = def{"\\" + std::string{alpha[i]}} = [&] {
      docs << "not, (dec carry, dec), (inc carry, inc), nybl, ...bits";
      return not_(i) + ", " + dec(i) + ", " + inc(i) + ", " + to_nybl(i) + ", "
           + detail::bits(i);
    };
  }

  def<"\\IS(_, ...) -> bool"> is = [&](arg, va) {
    def<"\\0"> _0 = [&] { return "0"; };
    def<"\\01">{} = [&] { return "1"; };
    return pp::cat(_0, pp::va_opt("1"));
  };

  def<"\\NOT(n, ...) -> hex">{}                = [&](pack args) { return args[0]; };
  def<"\\DEC(n, d, ...) -> (bool, hex)">{}     = [&](pack args) { return args[1]; };
  def<"\\INC(n, d, i, ...) -> (bool, hex)">{}  = [&](pack args) { return args[2]; };
  def<"\\NYBL(n, d, i, ny, ...) -> nybl">{}    = [&](pack args) { return args[3]; };
  def<"\\BITS(n, d, i, ny, ...) -> ...bool">{} = [&](pack args) { return args[4]; };

  return def<"o(t, ...)">{[&](arg t, va row) {
    return pp::call(pp::cat(utl::slice(is, -2), t), row);
  }}(t, xcat(utl::slice(digits[0], -1), v));
});

} // namespace impl
} // namespace api

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
#include "impl/traits.h"

namespace api {
namespace impl {

using namespace codegen;

static constexpr char const alpha[] = "0123456789ABCDEF";
static_assert(sizeof alpha == 17);

static std::string
lt(std::size_t i, std::size_t j) {
  return i < j ? "1" : "0";
}

static std::string
and_(std::size_t i, std::size_t j) {
  return std::string{alpha[i bitand j]};
}

static std::string
or_(std::size_t i, std::size_t j) {
  return std::string{alpha[i bitor j]};
}

static std::string
xor_(std::size_t i, std::size_t j) {
  return std::string{alpha[i xor j]};
}

static std::string
sub(std::size_t i, std::size_t j, std::size_t carry = 0) {
  return std::string{alpha[1 - ((16 + (i - j - carry)) / 16)]} + ", "
       + alpha[(16 + (i - j - carry)) % 16];
}

static std::string
add(std::size_t i, std::size_t j, std::size_t carry = 0) {
  return std::string{alpha[(i + j + carry) / 16]} + ", " + alpha[(i + j + carry) % 16];
}

decltype(hexhex) hexhex = NIFTY_DEF(hexhex, [&](arg v, arg t) {
  docs << "[internal] hex pair (hex##hex) traits";

  std::array<def<>, 16 * 16> pairs{};
  {
    std::size_t idx = 0;
    {
      for (std::size_t i = 0; i < 16 - 1; ++i) {
        for (std::size_t j = 0; j < 16; ++j) {
          pairs[idx++] = def{"\\" + std::string{alpha[i]} + alpha[j]} = [&] {
            return utl::cat(std::array{lt(i, j), and_(i, j), or_(i, j), xor_(i, j),
                                       sub(i, j, 0), sub(i, j, 1), add(i, j, 0),
                                       add(i, j, 1)},
                            ", ");
          };
        }
      }
      {
        std::size_t i = 15;
        std::size_t j = 0;
        for (; j < 16 - 1; ++j) {
          pairs[idx++] = def{"\\" + std::string{alpha[i]} + alpha[j]} = [&] {
            return utl::cat(std::array{lt(i, j), and_(i, j), or_(i, j), xor_(i, j),
                                       sub(i, j, 0), sub(i, j, 1), add(i, j, 0),
                                       add(i, j, 1)},
                            ", ");
          };
        }
        pairs[idx++] = def{"\\" + std::string{alpha[i]} + alpha[j]} = [&] {
          docs << "lt, and, or, xor, sub0-1 carry, sub0-1, add0-1 carry, add0-1";
          return utl::cat(std::array{lt(i, j), and_(i, j), or_(i, j), xor_(i, j),
                                     sub(i, j, 0), sub(i, j, 1), add(i, j, 0),
                                     add(i, j, 1)},
                          ", ");
        };
      }
    }
  }

  def<"\\IS(_, ...) -> bool"> is = [&](arg, va) {
    def<"\\0"> _0 = [&] {
      return "0";
    };
    def<"\\01">{} = [&] {
      return "1";
    };
    return pp::cat(_0, pp::va_opt("1"));
  };

  def{"\\LT(" + utl::cat(utl::alpha_base52_seq(1), ", ") + ", ...)"} = [&](pack args) {
    return args[0];
  };
  def{"\\AND(" + utl::cat(utl::alpha_base52_seq(2), ", ") + ", ...)"} = [&](pack args) {
    return args[1];
  };
  def{"\\OR(" + utl::cat(utl::alpha_base52_seq(3), ", ") + ", ...)"} = [&](pack args) {
    return args[2];
  };
  def{"\\XOR(" + utl::cat(utl::alpha_base52_seq(4), ", ") + ", ...)"} = [&](pack args) {
    return args[3];
  };
  def{"\\SUB0(" + utl::cat(utl::alpha_base52_seq(6), ", ") + ", ...)"} = [&](pack args) {
    return args[4] + ", " + args[5];
  };
  def{"\\SUB1(" + utl::cat(utl::alpha_base52_seq(8), ", ") + ", ...)"} = [&](pack args) {
    return args[6] + ", " + args[7];
  };
  def{"\\ADD0(" + utl::cat(utl::alpha_base52_seq(10), ", ") + ", ...)"} = [&](pack args) {
    return args[8] + ", " + args[9];
  };
  def{"\\ADD1(" + utl::cat(utl::alpha_base52_seq(12), ", ") + ", ...)"} = [&](pack args) {
    return args[10] + ", " + args[11];
  };

  return def<"o(t, ...)">{[&](arg t, va row) {
    return pp::call(pp::cat(utl::slice(is, -2), t), row);
  }}(t, xcat(utl::slice(pairs[0], -2), v));
});

} // namespace impl
} // namespace api

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

#include "impl/nybl.h"

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
sl(std::size_t i) {
  return pp::tup<false>(alpha[(i << 1) / 16], alpha[(i << 1) % 16]);
}

static std::string
sr(std::size_t i) {
  return pp::tup<false>(i % 2, std::string{alpha[i >> 1]});
}

static std::string
bin(std::size_t i) {
  switch (i) {
    case 0:
      return "(0, 0, 0, 0)";
    case 1:
      return "(0, 0, 0, 1)";
    case 2:
      return "(0, 0, 1, 0)";
    case 3:
      return "(0, 0, 1, 1)";
    case 4:
      return "(0, 1, 0, 0)";
    case 5:
      return "(0, 1, 0, 1)";
    case 6:
      return "(0, 1, 1, 0)";
    case 7:
      return "(0, 1, 1, 1)";
    case 8:
      return "(1, 0, 0, 0)";
    case 9:
      return "(1, 0, 0, 1)";
    case 10:
      return "(1, 0, 1, 0)";
    case 11:
      return "(1, 0, 1, 1)";
    case 12:
      return "(1, 1, 0, 0)";
    case 13:
      return "(1, 1, 0, 1)";
    case 14:
      return "(1, 1, 1, 0)";
    case 15:
      return "(1, 1, 1, 1)";
    default:
      throw std::logic_error{"bad index"};
  };
}

decltype(nybl_trait) nybl_trait = NIFTY_DEF(nybl_trait, [&](arg nybl, arg trait) {
  docs << "[internal] get a nybl digit trait."
       << "argument must be atom for IS. all other traits require nybl.";

  std::array<def<>, 16> digits{};

  {
    std::size_t i = 0;
    for (; i < 16 - 1; ++i) {
      digits[i] = def{std::string{alpha[i]}} = [&] {
        return not_(i) + ", " + dec(i) + ", " + inc(i) + ", " + sl(i) + ", " + sr(i) + ", "
             + bin(i);
      };
    }
    digits[i] = def{std::string{alpha[i]}} = [&] {
      docs << "not, (dec carry, dec), (inc carry, inc), (sl carry, sl), (sr mod, sr), (...bin)";
      return not_(i) + ", " + dec(i) + ", " + inc(i) + ", " + sl(i) + ", " + sr(i) + ", " + bin(i);
    };
  }

  def<"\\IS(_, ...)"> is = [&](arg, va) {
    def<"0"> _0 = [&] {
      return "0";
    };

    def<"01">{} = [&] {
      return "1";
    };

    return pp::cat(_0, pp::va_opt("1"));
  };

  def<"\\NOT(n, ...)">{} = [&](pack args) {
    return args[0];
  };

  def<"\\DEC(n, d, ...)">{} = [&](pack args) {
    return args[1];
  };

  def<"\\INC(n, d, i, ...)">{} = [&](pack args) {
    return args[2];
  };

  def<"\\SL(n, d, i, sl, ...)">{} = [&](pack args) {
    return args[3];
  };

  def<"\\SR(n, d, i, sl, sr, ...)">{} = [&](pack args) {
    return args[4];
  };

  def<"\\BIN(n, d, i, sl, sr, bin)">{} = [&](pack args) {
    return args[5];
  };

  return def<"o(trait, ...)">{[&](arg trait, va args) {
    return pp::call(pp::cat(utl::slice(is, -2), trait), args);
  }}(trait, cat(utl::slice(digits[0], -1), nybl));
});

} // namespace impl
} // namespace api

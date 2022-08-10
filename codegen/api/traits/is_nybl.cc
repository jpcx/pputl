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

#include "traits.h"

namespace api {

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
bin1(std::size_t i) {
  switch (i) {
    case 0:
      return "(0,0,0,0)";
    case 1:
      return "(0,0,0,1)";
    case 2:
      return "(0,0,1,0)";
    case 3:
      return "(0,0,1,1)";
    case 4:
      return "(0,1,0,0)";
    case 5:
      return "(0,1,0,1)";
    case 6:
      return "(0,1,1,0)";
    case 7:
      return "(0,1,1,1)";
    case 8:
      return "(1,0,0,0)";
    case 9:
      return "(1,0,0,1)";
    case 10:
      return "(1,0,1,0)";
    case 11:
      return "(1,0,1,1)";
    case 12:
      return "(1,1,0,0)";
    case 13:
      return "(1,1,0,1)";
    case 14:
      return "(1,1,1,0)";
    case 15:
      return "(1,1,1,1)";
    default:
      throw std::logic_error{"bad index"};
  };
}

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
sub(std::size_t i, std::size_t j) {
  return pp::tup<false>(i < j, alpha[(16 + (i - j)) % 16]);
}

static std::string
add(std::size_t i, std::size_t j) {
  return pp::tup<false>(alpha[(i + j) / 16], alpha[(i + j) % 16]);
}

static std::string
bin2(std::size_t i, std::size_t j) {
  return utl::slice(bin1(i), -1) + "," + utl::slice(bin1(j), 1, 0);
}

decltype(is_nybl) is_nybl = NIFTY_DEF(is_nybl, [&](va args) {
  docs << "[extends " + is_atom + "] detects if args is a nybl (capital hex digit).";

  tests << is_nybl()      = "0" >> docs;
  tests << is_nybl(0)     = "1" >> docs;
  tests << is_nybl('Q')   = "0" >> docs;
  tests << is_nybl("foo") = "0" >> docs;
  tests << is_nybl('B')   = "1" >> docs;
  tests << is_nybl('b')   = "0" >> docs;
  tests << is_nybl('F')   = "1" >> docs;

  clang_format = false;

  std::array<def<>, 16> digits{};
  {
    std::size_t i = 0;
    for (; i < 16 - 1; ++i) {
      digits[i] = def{"digits_" + std::string{alpha[i]}} = [&] {
        return not_(i) + "," + dec(i) + "," + inc(i) + "," + sl(i) + "," + sr(i) + "," + bin1(i);
      };
    }
    digits[i] = def{"digits_" + std::string{alpha[i]}} = [&] {
      docs << "not, (dec carry, dec), (inc carry, inc), (sl carry, sl), (sr mod, sr), (...bin)";
      return not_(i) + "," + dec(i) + "," + inc(i) + "," + sl(i) + "," + sr(i) + "," + bin1(i);
    };
  }
  std::array<def<>, 16 * 16> pairs{};
  {
    std::size_t idx = 0;
    {
      for (std::size_t i = 0; i < 16 - 1; ++i) {
        for (std::size_t j = 0; j < 16; ++j) {
          pairs[idx++] = def{"pairs_" + std::string{alpha[i]} + alpha[j]} = [&] {
            return lt(i, j) + "," + and_(i, j) + "," + or_(i, j) + "," + xor_(i, j) + ","
                 + sub(i, j) + "," + add(i, j) + "," + bin2(i, j);
          };
        }
      }
      {
        std::size_t i = 15;
        std::size_t j = 0;
        for (; j < 16 - 1; ++j) {
          pairs[idx++] = def{"pairs_" + std::string{alpha[i]} + alpha[j]} = [&] {
            return lt(i, j) + "," + and_(i, j) + "," + or_(i, j) + "," + xor_(i, j) + ","
                 + sub(i, j) + "," + add(i, j) + "," + bin2(i, j);
          };
        }
        pairs[idx++] = def{"pairs_" + std::string{alpha[i]} + alpha[j]} = [&] {
          docs << "lt, and, or, xor, (sub carry, sub), (add carry, add), (...bin)";
          return lt(i, j) + "," + and_(i, j) + "," + or_(i, j) + "," + xor_(i, j) + "," + sub(i, j)
               + "," + add(i, j) + "," + bin2(i, j);
        };
      }
    }
  }

  clang_format = true;

  def<"o_0(...)"> o0 = [&] {
    return "0";
  };

  def<"o_1(a)">{} = [&](arg a) {
    return is_some(rest(cat(utl::slice(digits[0], -1), a)));
  };

  return pp::call(cat(utl::slice(o0, -1), is_atom(args)), args);
});

} // namespace api

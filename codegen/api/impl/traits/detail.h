#pragma once
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

namespace api {
namespace impl {
namespace detail {

using namespace codegen;

inline std::string
hex(std::string a, std::string b, std::string c, std::string d) {
  auto grp = a + b + c + d;
  if (grp == "0000") {
    return "0";
  } else if (grp == "0001") {
    return "1";
  } else if (grp == "0010") {
    return "2";
  } else if (grp == "0011") {
    return "3";
  } else if (grp == "0100") {
    return "4";
  } else if (grp == "0101") {
    return "5";
  } else if (grp == "0110") {
    return "6";
  } else if (grp == "0111") {
    return "7";
  } else if (grp == "1000") {
    return "8";
  } else if (grp == "1001") {
    return "9";
  } else if (grp == "1010") {
    return "A";
  } else if (grp == "1011") {
    return "B";
  } else if (grp == "1100") {
    return "C";
  } else if (grp == "1101") {
    return "D";
  } else if (grp == "1110") {
    return "E";
  } else {
    return "F";
  }
}

inline std::string
bits(std::size_t i) {
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
      throw std::logic_error{"bad index"};
  };
}

inline std::string
isize(unsigned n) {
  return (n <= conf::int_max and n <= conf::size_max) ? "1" : "0";
}

inline std::string
usize(unsigned n) {
  return n <= conf::size_max ? "1" : "0";
}

inline std::string
iidx(unsigned n) {
  return (n <= conf::int_max ? (n <= conf::size_max)
                             : (n > (conf::uint_max - conf::size_max)))
           ? "1"
           : "0";
}

inline std::string
uidx(unsigned n) {
  return n <= conf::size_max ? "1" : "0";
}

inline std::string
log2(unsigned n) {
  if (n == 0)
    return "";
  return std::to_string(static_cast<unsigned>(std::log2(n)));
}

inline std::string
sqrt(unsigned n) {
  return std::to_string(static_cast<unsigned>(std::sqrt(n)));
}

inline std::string
fact(unsigned n) {
  auto                     facts = utl::prime_factors(n);
  std::vector<std::string> sfacts(facts.size());
  std::ranges::transform(facts, std::begin(sfacts),
                         [](auto&& v) { return std::to_string(v); });
  return utl::cat(sfacts, ", ");
}

inline std::array<std::string, conf::word_size * 4>
binary(unsigned n) {
  std::array<std::string, conf::word_size * 4> res{};
  std::ranges::fill(res, "0");
  for (unsigned i = 0; n; ++i) {
    res[res.size() - 1 - i] = std::to_string(n % 2);
    n /= 2;
  }
  return res;
}

inline std::string
ihex(std::array<std::string, conf::word_size * 4> const& bin) {
  std::string res{"0x"};
  for (std::size_t ofs = 0; ofs < bin.size(); ofs += 4)
    res += hex(bin[ofs + 0], bin[ofs + 1], bin[ofs + 2], bin[ofs + 3]);

  return res;
}

inline std::string
udec(std::size_t n) {
  return std::to_string(n) + "u";
}

inline std::string
uhex(std::array<std::string, conf::word_size * 4> const& bin) {
  return ihex(bin) + "u";
}

inline std::string
utup(std::array<std::string, conf::word_size * 4> const& bin) {
  std::vector<std::string> res{};
  for (std::size_t ofs = 0; ofs < bin.size(); ofs += 4)
    res.push_back(hex(bin[ofs + 0], bin[ofs + 1], bin[ofs + 2], bin[ofs + 3]));
  return pp::tup(utl::cat(res, ", "));
}

inline std::string
icast(std::array<std::string, conf::word_size * 4> const& bin, unsigned n) {
  auto mod = (conf::uint_max + 1) / 2;
  if (n < mod)
    return std::to_string(n);
  else
    return ihex(bin);
}

inline std::string
bnot(std::array<std::string, conf::word_size * 4> const& n) {
  auto res = n;
  for (auto&& v : res)
    v = (v == "1" ? "0" : "1");
  return uhex(res);
}

inline std::string
iltz(std::size_t n) {
  return n >= ((conf::uint_max + 1) / 2) ? "1" : "0";
}

} // namespace detail
} // namespace impl
} // namespace api

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

#include "impl/uint.h"

namespace api {
namespace impl {

using namespace codegen;

static std::string
log2(unsigned n) {
  if (n == 0)
    return "";
  return std::to_string(static_cast<unsigned>(std::log2(n))) + "u";
}

static std::string
sqrt(unsigned n) {
  return std::to_string(static_cast<unsigned>(std::sqrt(n))) + "u";
}

static std::string
factors(unsigned n) {
  auto                     facts = utl::prime_factors(n);
  std::vector<std::string> sfacts(facts.size());
  std::ranges::transform(facts, std::begin(sfacts), [](auto&& v) {
    return std::to_string(v) + "u";
  });
  return pp::tup(utl::cat(sfacts, ", "));
}

static std::array<std::string, conf::word_size * 4>
bitnot(std::array<std::string, conf::word_size * 4> const& n) {
  auto res = n;
  for (auto&& v : res)
    v = v == "1" ? "0" : "1";
  return res;
}

static std::array<std::string, conf::word_size * 4>
binary(unsigned n) {
  std::array<std::string, conf::word_size * 4> res{};
  std::ranges::fill(res, "0");
  for (unsigned i = 0; n; ++i) {
    res[res.size() - 1 - i] = std::to_string(n % 2);
    n /= 2;
  }
  return res;
}

static std::string
nybl(std::string a, std::string b, std::string c, std::string d) {
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

static std::string
hex_str(std::array<std::string, conf::word_size * 4> const& n, bool uint = true) {
  std::string res{"0x"};
  for (std::size_t ofs = 0; ofs < n.size(); ofs += 4)
    res += nybl(n[ofs + 0], n[ofs + 1], n[ofs + 2], n[ofs + 3]);

  return res + (uint ? "u" : "");
}

static std::string
nybls(std::array<std::string, conf::word_size * 4> const& n) {
  std::vector<std::string> res{};
  for (std::size_t ofs = 0; ofs < n.size(); ofs += 4)
    res.push_back(nybl(n[ofs + 0], n[ofs + 1], n[ofs + 2], n[ofs + 3]));
  return pp::tup(utl::cat(res, ", "));
}

static int
uint_to_int(unsigned n) {
  auto mod = (conf::uint_max + 1) / 2;
  if (n < mod)
    return n;
  else
    return -((mod * 2) - n);
}

decltype(uint_trait) uint_trait = NIFTY_DEF(uint_trait, [&](arg uint, arg trait) {
  docs << "[internal] get a uint trait."
       << "argument must be atom for IS. all other traits require uint."
       << "traits with an 'H' prefix require hex representation,"
       << "and traits with a 'D' prefix require decimal representation.";

  std::array<codegen::def<>, (codegen::conf::uint_max + 1) * 2> traits;

  std::size_t _ = 0;
  std::size_t n = 0;
  for (; _ < (traits.size() / 2) - 1; ++_, ++n) {
    auto bin  = binary(n);
    traits[_] = def{std::to_string(n) + "\\u"} = [&] {
      return utl::cat(std::array{std::string{"DEC"}, hex_str(bin, false),
                                 std::string{n > conf::int_max ? "1" : "0"}, log2(n), sqrt(n),
                                 factors(n)},
                      ", ");
    };
  }
  {
    auto bin    = binary(n);
    traits[_++] = def{std::to_string(n) + "\\u"} = [&] {
      docs << "type, signed hex, signed is negative, log2, sqrt, factors";
      return utl::cat(std::array{std::string{"DEC"}, hex_str(bin, false),
                                 std::string{n > conf::int_max ? "1" : "0"}, log2(n), sqrt(n),
                                 factors(n)},
                      ", ");
    };
  }

  n = 0;
  for (; _ < traits.size() - 1; ++_, ++n) {
    auto bin  = binary(n);
    traits[_] = def{"\\" + hex_str(bin)} = [&] {
      return utl::cat(std::array{std::string{"HEX"}, std::to_string(n) + "u",
                                 std::to_string(uint_to_int(n)), nybls(bin), hex_str(bitnot(bin))},
                      ", ");
    };
  }
  {
    auto bin  = binary(n);
    traits[_] = def{"\\" + hex_str(bin)} = [&] {
      docs << "type, unsigned decimal, signed decimal, word, bitnot";
      return utl::cat(std::array{std::string{"HEX"}, std::to_string(n) + "u",
                                 std::to_string(uint_to_int(n)), nybls(bin), hex_str(bitnot(bin))},
                      ", ");
    };
  }

  def<"\\IS(_, ...) -> bool"> is = [&](arg, va) {
    def<"0"> _0 = [&] {
      return "0";
    };

    def<"01">{} = [&] {
      return "1";
    };

    return pp::cat(_0, pp::va_opt("1"));
  };

  def<"\\TYPE(t, ...) -> HEX|DEC">{} = [&](arg t, va) {
    return t;
  };

  def<"\\HUDEC(t, ud, ...) -> ubase10">{} = [&](pack args) {
    return args[1];
  };

  def<"\\HIDEC(t, ud, id, ...) -> ibase10">{} = [&](pack args) {
    return args[2];
  };

  def<"\\HWORD(t, ud, id, n, ...) -> word">{} = [&](pack args) {
    return args[3];
  };

  def<"\\HBNOT(t, ud, id, n, bn) -> ubase16">{} = [&](pack args) {
    return args[4];
  };

  def<"\\DIHEX(t, ih, ...) -> ibase16">{} = [&](pack args) {
    return args[1];
  };

  def<"\\DINEG(t, ih, in, ...) -> bool{signed<0}">{} = [&](pack args) {
    return args[2];
  };

  def<"\\DLOG2(t, ih, in, l2, ...) -> ubase10{log2(n)}">{} = [&](pack args) {
    return args[3];
  };

  def<"\\DSQRT(t, ih, in, l2, sq, ...) -> ubase10{sqrt(n)}">{} = [&](pack args) {
    return args[4];
  };

  def<"\\DFACT(t, ih, in, l2, sq, f) -> typle<ubase10{factor}...>">{} = [&](pack args) {
    return args[5];
  };

  return def<"o(trait, ...)">{[&](arg trait, va args) {
    return pp::call(pp::cat(utl::slice(is, -2), trait), args);
  }}(trait, cat(utl::slice(traits[0], -2), uint));
});

} // namespace impl
} // namespace api

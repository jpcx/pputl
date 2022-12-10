#ifndef CODEGEN_API_IMPL_UHEX_TRAITS_CC
#define CODEGEN_API_IMPL_UHEX_TRAITS_CC
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

inline string
hex(string a, string b, string c, string d) {
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

inline array<string, conf::word_size * 4>
binary(unsigned n) {
  array<string, conf::word_size * 4> res{};
  ranges::fill(res, "0");
  for (unsigned i = 0; n; ++i) {
    res[res.size() - 1 - i] = to_string(n % 2);
    n /= 2;
  }
  return res;
}

inline string
ihex(array<string, conf::word_size * 4> const& bin) {
  string res{"0x"};
  for (size_t ofs = 0; ofs < bin.size(); ofs += 4)
    res += hex(bin[ofs + 0], bin[ofs + 1], bin[ofs + 2], bin[ofs + 3]);

  return res;
}

inline string
udec(size_t n) {
  return to_string(n) + "u";
}

inline string
uhex(array<string, conf::word_size * 4> const& bin) {
  return ihex(bin) + "u";
}

inline string
icast(array<string, conf::word_size * 4> const& bin, unsigned n) {
  auto mod = (conf::uint_max + 1) / 2;
  if (n < mod)
    return to_string(n);
  else
    return ihex(bin);
}

inline string
bnot(array<string, conf::word_size * 4> const& n) {
  auto res = n;
  for (auto&& v : res)
    v = (v == "1" ? "0" : "1");
  return uhex(res);
}

inline string
iltz(size_t n) {
  return n >= ((conf::uint_max + 1) / 2) ? "1" : "0";
}

inline string
hdump(array<string, conf::word_size * 4> const& bin) {
  vector<string> res{};
  for (size_t ofs = 0; ofs < bin.size(); ofs += 4)
    res.push_back(hex(bin[ofs + 0], bin[ofs + 1], bin[ofs + 2], bin[ofs + 3]));
  return utl::cat(res, ", ");
}

} // namespace impl

inline string uhex_prefix{};

inline def<"impl_uhex_traits(v, t: uhex, UDEC|IHEX|ICAST|ILTZ|BNOT|HDUMP)"> uhex_traits =
    [](arg v, arg t) {
      category = "impl";

      docs << "[internal] uhex traits";

      array<def<>, conf::uint_max + 1> uhexs;

      {
        size_t i = 0;
        for (; i < uhexs.size() - 1; ++i) {
          auto bin = impl::binary(i);
          uhexs[i] = def{"\\" + impl::uhex(bin)} = [&] {
            return utl::cat(array{impl::udec(i), impl::ihex(bin), impl::icast(bin, i),
                                  impl::iltz(i), impl::bnot(bin), impl::hdump(bin)},
                            ", ");
          };
        }
        {
          auto bin = impl::binary(i);
          uhexs[i] = def{"\\" + impl::uhex(bin)} = [&] {
            docs << "UDEC, IHEX, ICAST, ILTZ, BNOT, HDUMP...";
            return utl::cat(array{impl::udec(i), impl::ihex(bin), impl::icast(bin, i),
                                  impl::iltz(i), impl::bnot(bin), impl::hdump(bin)},
                            ", ");
          };
        }
      }

      uhex_prefix = utl::slice(uhexs[0], -(conf::word_size + 3));

      def<"\\UDEC(u, ...) -> udec"> udec = [&](pack args) {
        return args[0];
      };
      def<"\\IHEX(u, h, ...) -> ihex">{} = [&](pack args) {
        return args[1];
      };
      def<"\\ICAST(u, h, i, ...) -> idec|ihex">{} = [&](pack args) {
        return args[2];
      };
      def<"\\ILTZ(u, h, i, z, ...) -> bool">{} = [&](pack args) {
        return args[3];
      };
      def<"\\BNOT(u, h, i, z, b, ...) -> uhex">{} = [&](pack args) {
        return args[4];
      };
      def<"\\HDUMP(u, h, i, z, b, ...) -> 0|1|...|E|F...">{} = [&](pack args) {
        return args[5];
      };

      return def<"o(t, ...)">{[&](arg t, va row) {
        return pp::call(pp::cat(utl::slice(udec, -4), t), row);
      }}(t, xcat(uhex_prefix, v));
    };

} // namespace api
} // namespace codegen

#endif

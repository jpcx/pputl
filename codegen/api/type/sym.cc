#ifndef CODEGEN_API_TYPE_SYM_CC
#define CODEGEN_API_TYPE_SYM_CC
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

#include <iomanip>
#include <sstream>

#include "codegen.h"

#include "traits/is_tup.cc"

namespace codegen {
namespace api {

namespace sym_ {

using namespace std;

inline array<string, conf::word_size * 4>
bin(unsigned n) {
  array<string, conf::word_size * 4> res{};
  ranges::fill(res, "0");
  for (unsigned i = 0; n; ++i) {
    res[res.size() - 1 - i] = to_string(n % 2);
    n /= 2;
  }
  return res;
}

inline string
nybl(string a, string b, string c, string d) {
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

inline array<string, conf::word_size>
nybls(array<string, conf::word_size * 4> const& bin) {
  array<string, conf::word_size> res{};

  for (size_t ofs = 0, i = 0; ofs < bin.size(); ofs += 4, ++i)
    res[i] = nybl(bin[ofs + 0], bin[ofs + 1], bin[ofs + 2], bin[ofs + 3]);

  return res;
}

inline def<"sym(...: any...) -> sym"> self = [](va args) {
  category = "type";

  docs << "[extends some] a global or namespace-qualified static storage pointer."
       << ""
       << "syms are defined in two ways:"
       << ""
       << "globally: (sym matches /[\\w\\d_]+/)"
       << ""
       << "  #define " + apiname("sym") + "_<name>_is_<name> (<data...>)"
       << ""
       << "namespace-qualified: (sym matches /[\\w\\d_]+\\([\\w\\d_]+\\)/)"
       << ""
       << "  #define " + apiname("sym") + "_<ns>(name)              (<ns>, name)"
       << "  #define " + apiname("sym") + "_<ns>_<name1>_is_<name1> (<data...>)"
       << "  #define " + apiname("sym") + "_<ns>_<name2>_is_<name2> (<data...>)"
       << "  ..."
       << ""
       << "this layout allows syms to be compared using compare.eq and compare.ne."
       << ""
       << "the sym type lays the foundation for arithmetic literals, obj member"
       << "access,  and negative integers.  since arithmetic tokens cannot form"
       << "identifiers,  the C++ compl operator is used to ensure that negative"
       << "ints  can be parsed by the library and have the same meaning in both"
       << "the preprocessor and C++ code. when using an int or num to construct"
       << "an identifier, use lang.cat (which converts ints < 0 to 12-bit hex).";

  for (int i = 0; i < conf::int_max + 1; ++i) {
    string s = to_string(i);
    def{"\\" + s + "_is_" + s, [&] {
          clang_format = false;
          auto b       = bin(i);
          return pp::tup("true", pp::tup(nybls(b)));
        }};
  }

  for (int i = 0; i < conf::int_max + 1; ++i) {
    string s = to_string(i);
    def{"\\compl_" + s + "_is_" + s, [&] {
          clang_format = false;
          auto b       = bin(i + conf::int_max + 1);
          return pp::tup("true", pp::tup(nybls(b)));
        }};
  }

  def<"\\compl(int)">{} = [](arg _int) {
    clang_format = false;
    return pp::tup("compl", _int);
  };

  return args;
};

} // namespace sym_

inline constexpr auto& sym = sym_::self;

} // namespace api
} // namespace codegen

#endif

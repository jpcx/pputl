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

inline def<"sym(...: any...) -> sym"> self = [](va args) {
  category = "type";

  docs << "[extends some] an explicitly defined equality-comparable token sequence."
       << ""
       << "syms can be created by defining a macro as follows:"
       << "  #define " + self + "_<sym name>_IS_<sym name> (<any...>)"
       << ""
       << "builtin syms (such as int and uint) are defined"
       << "differently to reduce the number of API identifiers."
       << ""
       << "all syms  are token sequences  that can be used to form"
       << "identifiers except for the builtin negative ints. since"
       << "hyphens or arithmetic symbols can't be used anywhere in"
       << "identifiers,  the alternate bitwise complement operator"
       << "compl  is used to represent negative ints in a way that"
       << "retains the meaning across pputl, the preprocessor, and"
       << "C++.  to illustrate,  the negative int compl(0x7FF) can"
       << "help form a macro function call, but -2048 cannot. this"
       << "exception to the sym model is only necessary to"
       << ""
       << "this format enables sym equality comparisons and data storage."
       << "declaration macros must expand to a tuple, which may be empty."
       << "use lang.lookup to retrieve the items stored in the sym tuple.";

  for (int i = 0; i < conf::int_max + 1; ++i) {
    string s = to_string(i);
    def{"\\" + s + "_IS_" + s, [&] {
          clang_format = false;
          return pp::tup(s, s);
        }};
  }

  for (int i = -conf::int_max - 1; i < 0; ++i) {
    stringstream stream;
    stream << setfill('0') << setw(conf::word_size) << uppercase << hex << -i - 1;
    string s = "0x" + stream.str();
    def{"compl_\\" + s, [&] {
          clang_format = false;
          return pp::tup(pp::call("compl", s), i) + ",";
        }};
  }

  return args;
};

} // namespace sym_

inline constexpr auto& sym = sym_::self;

} // namespace api
} // namespace codegen

#endif

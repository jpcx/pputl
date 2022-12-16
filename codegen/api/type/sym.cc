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
       << "builtin syms (such as int and uint) use a different"
       << "configuration to reduce the number of API identifiers."
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

  for (int i = conf::int_max + 1; i < (conf::int_max + 1) * 2; ++i) {
    stringstream stream;
    stream << setfill('0') << setw(conf::word_size) << uppercase << hex << i;
    string s = "0x" + stream.str();
    def{"\\" + s, [&] {
          clang_format = false;
          return pp::tup(s + "-" + to_string(conf::uint_max + 1),
                         i - ((int)conf::size_max + 1))
               + ",";
        }};
  }

  return args;
};

} // namespace sym_

inline constexpr auto& sym = sym_::self;

} // namespace api
} // namespace codegen

#endif

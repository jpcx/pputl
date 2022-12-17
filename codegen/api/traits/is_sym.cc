#ifndef CODEGEN_API_TRAITS_IS_SYM_CC
#define CODEGEN_API_TRAITS_IS_SYM_CC
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

#include "lang/cat.cc"
#include "traits/is_tup.cc"

namespace codegen {
namespace api {

namespace is_sym_ {

using namespace std;

inline def<"is_sym(...: any...) -> bool"> self = [](va args) {
  category = "traits";

  docs << "[extends is_some] checks if args is a pputl sym."
       << ""
       << "syms are equality-comparable names that point to static storage."
       << "a sym can either be declared globally or wrapped in a namespace."
       << ""
       << "global syms match /[\\w\\d_]+/ and are defined as follows:"
       << ""
       << "  #define " + apiname("sym") + "_<name>_IS_<name> (<sym traits...>)"
       << ""
       << "namespaced syms match /[\\w\\d_]+\\([\\w\\d_]+\\)/ and are defined as follows:"
       << ""
       << "  #define " + apiname("sym") + "_<ns>(name)              (<ns>, name)"
       << "  #define " + apiname("sym") + "_<ns>_<name1>_IS_<name1> (<sym traits...>)"
       << "  #define " + apiname("sym") + "_<ns>_<name2>_IS_<name2> (<sym traits...>)"
       << "  ..."
       << ""
       << "the sym type lays the foundation for pputl artihmetic literals,"
       << "object member access, and negative integers.  negative integers"
       << "cannot be represented using arithmetic symbols  and instead use"
       << "C++ compl expressions  that can be parsed by the library  while"
       << "retaining the same meaning in both the preprocessor and C++. In"
       << "those cases, the namespace is compl and name is an integer.";

  def<"\\falsefalse(e)"> _falsefalse = [](arg) {
    return "false";
  };

  def<"\\falsetrue(e)">{} = [](arg) {
    return "true";
  };

  def<"\\truefalse(e)">{} = [](arg) {
    return "true";
  };

  def<"\\truetrue(e)">{} = [](arg e) {
    return pp::call(apiname("fail"), e);
  };

  return def<"\\o(e, ...)">{[&](arg e, va args) {
    return pp::call(cat(utl::slice(_falsefalse, -10),
                        cat(is_tup(pp::cat(apiname("sym"), "_", args)),
                            is_tup(pp::cat(implname("impl_sym"), "_", args)))),
                    e);
  }}(pp::call(apiname("err"), self, pp::str("detected a redefined reserved sym"), args),
     args);
};

} // namespace is_sym_

inline constexpr auto& is_sym = is_sym_::self;

} // namespace api
} // namespace codegen

#endif

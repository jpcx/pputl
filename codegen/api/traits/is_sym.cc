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

#include "codegen.h"

#include "traits/is_tup.cc"

namespace codegen {
namespace api {

namespace is_sym_ {

using namespace std;

inline def<"is_sym(...: any...) -> bool"> self = [](va args) {
  category = "traits";

  docs << "[extends is_some] checks if args is a pputl sym."
       << ""
       << "syms are created by defining a macro as follows:"
       << "  #define " + apiname("sym") + "_<sym name>_IS_<sym name> (<any...>)"
       << ""
       << "this format enables sym equality comparisons and data storage."
       << "declaration macros must expand to a tuple, which may be empty."
       << "use lang.lookup to retrieve the items stored in the sym tuple.";

  return def<"o(...)">{[](va args) {
    return is_tup(pp::cat(apiname("sym"), "_", args, "_IS_", args));
  }}(args);
};

} // namespace is_sym_

inline constexpr auto& is_sym = is_sym_::self;

} // namespace api
} // namespace codegen

#endif

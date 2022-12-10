#ifndef CODEGEN_API_LANG_REST_CC
#define CODEGEN_API_LANG_REST_CC
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

#include "lang/esc.cc"

namespace codegen {
namespace api {

using namespace std;

inline def<"rest(_, ...: first: any, ...rest: any) -> any..."> rest = [](arg, va rest_) {
  category = "lang";

  docs << "immediately returns all args except for the first."
       << "must have at least one argument."
       << ""
       << "useful for operating directly on __VA_ARGS__ or"
       << "for quickly retrieving the rest tuple elements"
       << "using an identity function such as " + esc + "."
       << ""
       << "rest cannot be used to extract from expression results,"
       << "as the inputs are evaluated immediately. use xrest for"
       << "expressions that should expand before selection."
       << ""
       << "e.g. " + rest("__VA_ARGS__") << "     " + esc(rest + " tup");

  return rest_;
};

} // namespace api
} // namespace codegen

#endif

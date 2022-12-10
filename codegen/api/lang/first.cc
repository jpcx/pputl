#ifndef CODEGEN_API_LANG_FIRST_CC
#define CODEGEN_API_LANG_FIRST_CC
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

inline def<"first(_, ...: first: any, ...rest: any) -> any"> first = [](arg first_, va) {
  category = "lang";

  docs << "immediately returns the first argument."
       << "must have at least one argument."
       << ""
       << "useful for operating directly on __VA_ARGS__ or"
       << "for quickly retrieving the first tuple element"
       << "using an identity function such as " + esc + "."
       << ""
       << "e.g. " + first("__VA_ARGS__") << "     " + esc(first + " tup");

  return first_;
};

} // namespace api
} // namespace codegen

#endif

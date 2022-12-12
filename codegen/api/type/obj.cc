#ifndef CODEGEN_API_TYPE_OBJ_CC
#define CODEGEN_API_TYPE_OBJ_CC
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

namespace obj_ {

using namespace std;

#define FOO_IS_FOO
#define FOO_A 0
#define FOO_B 1
#define FOO_C 2

#define BAR_IS_FOO
#define BAR_IS_BAR
#define BAR_A 3
#define BAR_B 4

inline def<"obj(...: scope: tup, state: tup) -> obj"> self = [](va args) {
  category = "type";

  docs << "[extends some] an inheritable, name-addressable state container";

  tests << self(pp::tup(), pp::tup())      = self(pp::tup(), pp::tup()) >> docs;
  tests << self(pp::tup("FOO"), pp::tup()) = self(pp::tup("FOO"), pp::tup()) >> docs;
  tests << self(pp::tup("BAR"), pp::tup('a', 'b')) =
      self(pp::tup("BAR"), pp::tup('a', 'b')) >> docs;

  return self(args);
};

} // namespace obj_

inline constexpr auto& obj = obj_::self;

} // namespace api
} // namespace codegen

#endif

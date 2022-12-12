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

#define PTL_SYM_EQ(a, b)

#define IS_NAMED(...) IS_NAMED_X(__VA_ARGS__)
#define IS_NAMED_X(name, o) \
  PTL_IS_NONE(PTL_CAT(PTL_CAT(name, _IS_), IS_NAMED_NAME_PTL_OBJ(o)))
#define IS_NAMED_NAME_PTL_OBJ(name, ...) name

#define VEC_IS_VEC
#define VEC(...)          VEC_X(PTL_SIZEOF(__ARGS__), (__VA_ARGS__))
#define VEC_X(size, data) PTL_OBJ(VEC, (size, size, data))

#define VEC_CAP  0
#define VEC_SIZE 1
#define VEC_DATA 2

#define VEC_RESERVE(_, cap) VEC_RESERVE_X(_, PTL_SUB(cap, PTL_GET(_, CAP)))
#define VEC_RESERVE_X(_, diff) \
  PTL_IF(PTL_GTZ(diff), VEC_RESERVE_GROW, VEC_RESERVE_NOOP)(_, diff)
#define VEC_RESERVE_GROW(_, diff) \
  PTL_SET(PTL_SET(_, DATA, (PTL_ITEMS(PTL_GET(_, DATA)), PTL_REPEAT(diff))), CAP, cap)
#define VEC_RESERVE_NOOP(_, cap) _

#define v VEC(a, b, c)
#define x PTL_GET(v, CAP)

inline def<"obj(...: name: sym, state: tup) -> obj"> self = [](va args) {
  category = "type";

  docs << "[extends some] an atom-addressable state container";

  return self(args);
};

} // namespace obj_

inline constexpr auto& obj = obj_::self;

} // namespace api
} // namespace codegen

#endif

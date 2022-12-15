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

namespace codegen {
namespace api {

namespace obj_ {

using namespace std;

inline def<"obj(...: ...state: any...) -> obj"> self = [](va args) {
  category = "type";

  docs << "[extends some] a named, polymorphic, atom-addressable state container."
       << ""
       << "objects have the form of NAME(...state) and can be created either directly"
       << "or using a directly-recursive constructor (direct recursion freezes tokens)."
       << ""
       << "example:"
       << ""
       << "  // obj types must be properly declared as a pputl symbol and object."
       << "  // object declarations represent the heirarchy and root obj construction."
       << "  #define PTL_SYM_POS2D_IS_POS2D (POS2D)"
       << "  #define " + self + "_POS2D(...) " + self + "(__VA_ARGS__)"
       << ""
       << "  // member names are atoms defined as follows:"
       << "  #define PTL_IDX_POS2D_Y 0 // atom Y refers to POS2D idx 0"
       << "  #define PTL_IDX_POS2D_X 1 // atom X refers to POS2D idx 1"
       << ""
       << "  // a constructor that handles copies, default, and member initialization"
       << "  #define POS2D(...)"
       << "    PTL_IF("
       << "        PTL_ISA(__VA_ARGS__, POS2D), POS2D_COPY,"
       << "        PTL_IF(PTL_IS_NONE(__VA_ARGS__), POS2D_DEFAULT, POS2D_ARGS))"
       << "    (__VA_ARGS__)"
       << ""
       << "  // explicitly retrieving Y and X instead of simply"
       << "  // returning o, as it may be a derived object"
       << "  #define POS2D_COPY(o)      PTL_COPY(PTL_COPY(POS2D_DEFAULT(), o, Y), X)"
       << "  #define POS2D_DEFAULT(...) POS2D(0, 0)"
       << "  #define POS2D_ARGS(y, x)   POS2D(y, x)"
       << ""
       << "  // deriving from POS2D. note: derived comes before base in heirarchy"
       << "  #define PTL_SYM_POS3D_IS_POS3D (POS3D, POS2D)"
       << "  #define " + self + "_POS3D(...) " + self + "(__VA_ARGS__)"
       << ""
       << "  // using a new index for this member. reusing an index overrides base."
       << "  #define PTL_IDX_POS3D_Z 2"
       << ""
       << "  // this constructor also handles cast from base"
       << "  #define POS3D(...)"
       << "    PTL_IF(PTL_ISA(__VA_ARGS__, POS3D), POS3D_COPY,"
       << "           PTL_IF(PTL_ISA(__VA_ARGS__, POS2D), POS3D_BASE,"
       << "                  PTL_IF(PTL_IS_NONE(__VA_ARGS__), POS2D_DEFAULT,"
       << "                         POS2D_ARGS)))"
       << "    (__VA_ARGS__)"
       << ""
       << "  #define POS3D_COPY(o)"
       << "    PTL_COPY(PTL_COPY(PTL_COPY(POS3D_DEFAULT(), o, Y), X), Z)"
       << "  #define POS3D_BASE(o)       POS3D(PTL_GET(o, Y), PTL_GET(o, X), 0)"
       << "  #define POS3D_DEFAULT(...)  POS3D(0, 0, 0)"
       << "  #define POS3D_ARGS(y, x, z) POS3D(y, x, z)"
       << ""
       << "  // elements from objects can be indexed using PTL_GET and PTL_SET"
       << "  #define POS   POS3D(3, 5, 4)"
       << "  #define MOVED PTL_SET(FOO, X, PTL_INC(PTL_GET(FOO, X)))";

  return self(args);
};

} // namespace obj_

inline constexpr auto& obj = obj_::self;

} // namespace api
} // namespace codegen

#endif

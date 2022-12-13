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

#define PTL_EQ(a, b)

#define PTL_OBJ_VEC(...) PTL_OBJ(__VA_ARGS__)

#define VEC_CAP  0u
#define VEC_SIZE 1u
#define VEC_DATA 2u

#define VEC(...)        VEC_X(PTL_COUNT(__VA_ARGS__), (__VA_ARGS__))
#define VEC_X(ct, data) VEC(ct, ct, data)

inline def<"obj(...: ...state: any...) -> obj"> self = [](va args) {
  category = "type";

  docs << "[extends some] a named state container";

  return self(args);
};

} // namespace obj_

inline constexpr auto& obj = obj_::self;

} // namespace api
} // namespace codegen

#endif

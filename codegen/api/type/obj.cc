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
//	   This program is free software: you can redistribute it and/or modify
//	   it under the terms of the GNU General Public License as published by
//	   the Free Software Foundation, either version 3 of the License, or
//	   (at your option) any later version.
//
//	   This program is distributed in the hope that it will be useful,
//	   but WITHOUT ANY WARRANTY; without even the implied warranty of
//	   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	   GNU General Public License for more details.
//                                                                            //
//  You should have received a copy of the GNU General Public License        ///
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.  ////
///////////////////////////////////////////////////////////////////////////// */

#include "type.h"

namespace api {

using namespace codegen;

decltype(obj) obj = NIFTY_DEF(obj, [&](va) {
  docs << "[inherits any] an inheritable, indexable, stateful structure."
       << ""
       << "objects are expansion-terminated self-references with the following structure:"
       << "  obj(chain_sz, (...prefixes), (...this))"
       << ""
       << "the inheritance chain grows leftwards. use get and set to access members."
       << "member accessors iterate over each prefix until a match is found,"
       << "meaning that derived classes can override the members of their parents."
       << ""
       << "example:"
       << ""
       << "  // initial declaration macro, used for detection"
       << "  // must be formatted as <name>_IS_<name> and expand to none."
       << "  #define ARR_IS_ARR"
       << ""
       << "  // member data declarations, represented by an enum."
       << "  // values indicate sorage positions and need not be sequential."
       << "  // declarations must be formatted as <name>_DATA_<member>."
       << "  #define ARR_DATA_ITEMS 0"
       << ""
       << "  // member function declarations, represented by an enum."
       << "  // declarations must be formatted as <name>_FN_<member>."
       << "  #define ARR_FN_FRONT      0"
       << "  #define ARR_FN_PUSH_FRONT 1"
       << "  #define ARR_FN_PUSH_BACK  2"
       << "  #define ARR_FN_POP_FRONT  3"
       << "  #define ARR_FN_POP_BACK   4"
       << ""
       << "  // member function definitions"
       << ""
       << "  #define ARR_FRONT(this) PTL_FRONT(PTL_GET(this, ITEMS))"
       << "  #define ARR_PUSH_FRONT(this, v)"
       << "    PTL_SET(this, ITEMS, PTL_PUSH_FRONT(PTL_GET(this, ITEMS), v))"
       << "  #define ARR_PUSH_BACK(this, v)"
       << "    PTL_SET(this, ITEMS, PTL_PUSH_BACK(PTL_GET(this, ITEMS), v)"
       << "  #define ARR_POP_FRONT(this)"
       << "    PTL_SET(this, ITEMS, PTL_POP_FRONT(PTL_GET(this, ITEMS)))"
       << "  #define ARR_POP_BACK(this)"
       << "    PTL_SET(this, ITEMS, PTL_POP_BACK(PTL_GET(this, ITEMS)))"
       << ""
       << "  // writing a base object constructor; obj(name, (...this), ...bases)"
       << "  #define ARR(...)"
       << "    PTL_IF(PTL_IS(ARR, __VA_ARGS__), ARR_COPY, ARR_NEW)(__VA_ARGS__)"
       << "  #define ARR_COPY(arr) arr"
       << "  #define ARR_NEW(...)  PTL_OBJ(ARR, (PTL_TUPLE(__VA_ARGS__)))"
       << ""
       << "  // PTL_OBJ(1u, (ARR), (()))"
       << "  #define AR0 ARR()"
       << ""
       << "  // PTL_OBJ(1u, (ARR), ((foo)))"
       << "  #define AR1 PTL_GET(AR0, PUSH_FRONT)(AR0, foo)"
       << ""
       << "  // inheriting from ARR"
       << "  #define STACK_IS_STACK"
       << "  #define STACK_IS_ARR"
       << ""
       << "  // adding a data member to illustrate that"
       << "  // inherited objects share the same `this` tuple;"
       << "  // data enum values must not conflict with bases"
       << "  #define STACK_DATA_DEPTH 1"
       << ""
       << "  // defining new member functions"
       << "  #define STACK_FN_FRONT 0"
       << "  #define STACK_FN_PUSH  1"
       << "  #define STACK_FN_POP   2"
       << ""
       << "  // adding member functions;"
       << "  // functions are privately inherited by default, but can"
       << "  // be called on any derived object using their fully-qualified name"
       << "  #define STACK_FRONT ARR_FRONT // publicly inheriting ARR_FRONT"
       << "  #define STACK_PUSH(this, v)"
       << "    PTL_SET(ARR_PUSH_FRONT(this, v), DEPTH,"
       << "            PTL_INC(PTL_GET(this, DEPTH)))"
       << "  #define STACK_POP(this)"
       << "    PTL_SET(ARR_POP_FRONT(this), DEPTH,"
       << "            PTL_DEC(PTL_GET(this, DEPTH)))"
       << ""
       << "  // constructor that inherits from ARR"
       << "  #define STACK(...)"
       << "    PTL_IF(PTL_IS(STACK, __VA_ARGS__), STACK_COPY, STACK_NEW)"
       << "    (__VA_ARGS__)"
       << "  #define STACK_COPY(stack) stack"
       << "  #define STACK_NEW(...)"
       << "    PTL_OBJ(STACK, (PTL_TUPLE(__VA_ARGS__), 0u), ARR)"
       << ""
       << "  // PTL_OBJ(2u, (STACK, ARR), ((), 0u))"
       << "  #define ST0 STACK()"
       << ""
       << "  // PTL_OBJ(2u, (STACK, ARR), ((0), 1u))"
       << "  #define ST1 PTL_GET(ST0, PUSH)(ST0, 0)"
       << ""
       << "  // PTL_OBJ(2u, (STACK, ARR), ((1, 0), 2u))"
       << "  #define ST2 PTL_GET(ST1, PUSH)(ST1, 1)"
       << ""
       << "  // PTL_OBJ(2u, (STACK, ARR), ((0), 1u))"
       << "  #define ST3 PTL_GET(ST1, POP)(ST2)";

  /* tests << obj() = "0" >> docs; */

  return obj("0u, (PTL_NULL), ()");
});

} // namespace api

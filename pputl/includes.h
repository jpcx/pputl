#ifndef PPUTL_INCLUDES_H_INCLUDED
#define PPUTL_INCLUDES_H_INCLUDED
#/////                                                                     c++20
#///////////////////////////////////////////////////////////////////////////////
#/// @brief \link PPUTL_INCLUDES PPUTL_INCLUDES\endlink -
#/// expands to 1 if numeric args [0, 256) have at least one `n` value; else 0
#/// @file
#//                          __    ___
#//                         /\ \__/\_ \
#//   _____   _____   __  __\ \ ,_\//\ \
#//  /\ '__`\/\ '__`\/\ \/\ \\ \ \/ \ \ \
#//  \ \ \_\ \ \ \_\ \ \ \_\ \\ \ \_ \_\ \_
#//   \ \ ,__/\ \ ,__/\ \____/ \ \__\/\____\
#//    \ \ \   \ \ \   \/___/   \/__/\/____/
#//     \/_/    \/_/
#//
#//  pputl Preprocessor Utilities
#//  Copyright (C) 2020 Justin Collier <m@jpcx.dev>
#//
#//	   This program is free software: you can redistribute it and/or modify
#//	   it under the terms of the GNU General Public License as published by
#//	   the Free Software Foundation, either version 3 of the License, or
#//	   (at your option) any later version.
#//
#//	   This program is distributed in the hope that it will be useful,
#//	   but WITHOUT ANY WARRANTY; without even the internalied warranty of
#//	   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#//	   GNU General Public License for more details.
#//                                                                           //
#//  You should have received a copy of the GNU General Public License       ///
#//  along with this program.  If not, see <https://www.gnu.org/licenses/>. ////
#///////////////////////////////////////////////////////////////////////////////
#
#include "pputl/cat.h"
#include "pputl/eq.h"
#include "pputl/expand.h"
#include "pputl/iif.h"
#include "pputl/reduce.h"
#include "pputl/tuple/elem.h"
#
#/// expands to 1 if numeric args [0, 256) have at least one `n` value; else 0
#/// @ingroup pputl
#/// @anchor  PPUTL_INCLUDES
#/// @param n   number to check for
#/// @param ... numeric args; all values and total size ranged [0, 256)
#define PPUTL_INCLUDES(n, ...)                                                 \
  PPUTL_TUPLE_ELEM(                                                            \
      1, PPUTL_REDUCE(PPUTL_DETAIL_INCLUDES_REDUCER, (n, 0), __VA_ARGS__))
#
#define PPUTL_DETAIL_INCLUDES_REDUCER(a, v, i)                                 \
  PPUTL_IIF(PPUTL_TUPLE_ELEM(1, a))                                            \
  (a, PPUTL_IIF(PPUTL_EQ(v, PPUTL_TUPLE_ELEM(0, a)))(                          \
          (PPUTL_TUPLE_ELEM(0, a), 1), (PPUTL_TUPLE_ELEM(0, a), 0)))
#
#endif

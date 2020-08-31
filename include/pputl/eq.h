#ifndef PPUTL_EQ_H_INCLUDED
#define PPUTL_EQ_H_INCLUDED
#/////                                                                     c++20
#///////////////////////////////////////////////////////////////////////////////
#/// @brief \link PPUTL_EQ PPUTL_EQ\endlink -
#/// expands to 1 if two numbers a and b [0, 256) are equal, or 0 if not
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
#include "pputl/choose.h"
#include "pputl/gen/repeat.h"
#
#/// expands to 1 if two numbers a and b [0, 256) are equal, or 0 if not
#/// @ingroup pputl
#/// @anchor  PPUTL_EQ
#/// @param a number [0, 256)
#/// @param b number [0, 256)
#define PPUTL_EQ(a, b) PPUTL_DETAIL_EQ_X(a, b)
#
#define PPUTL_DETAIL_EQ_X(a, b) \
  PPUTL_CHOOSE(a)(PPUTL_DETAIL_EQ_BITS_CHOICE(b)(b))
#define PPUTL_DETAIL_EQ_BITS_CHOICE(i) \
  PPUTL_CHOOSE(i)                      \
  (PPUTL_DETAIL_EQ_BITS_EQ0, PPUTL_GEN_REPEAT(255, PPUTL_DETAIL_EQ_BITS_GT0))
#
#define PPUTL_DETAIL_EQ_BITS_EQ0(i) 1, PPUTL_GEN_REPEAT(255, 0)
#define PPUTL_DETAIL_EQ_BITS_GT0(i) \
  PPUTL_GEN_REPEAT(i, 0), 1, PPUTL_GEN_REPEAT(254, 0)
#
#endif

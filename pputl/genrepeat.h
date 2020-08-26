#ifndef PPUTL_GENREPEAT_H_INCLUDED
#define PPUTL_GENREPEAT_H_INCLUDED
#/////                                                                     c++20
#///////////////////////////////////////////////////////////////////////////////
#/// @brief \link PPUTL_GENREPEAT PPUTL_GENREPEAT\endlink -
#/// generates args by repeating __VA_ARGS__ [0, 256) times
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
#include "pputl/gen.h"
#
#/// generates args by repeating __VA_ARGS__ [0, 256) times
#/// @ingroup pputl
#/// @anchor  PPUTL_GENREPEAT
#/// @param n   - number of times to repeat [0, 256)
#/// @param ... - args to repeat
#define PPUTL_GENREPEAT(n, ...)                                                \
  PPUTL_GEN(n, PPUTL_DETAIL_GENREPEAT_GEN, __VA_ARGS__)
#
#define PPUTL_DETAIL_GENREPEAT_GEN(i, ...) __VA_ARGS__
#
#endif

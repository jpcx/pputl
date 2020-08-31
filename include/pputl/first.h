#ifndef PPUTL_FIRST_H_INCLUDED
#define PPUTL_FIRST_H_INCLUDED
#/////                                                                     c++20
#///////////////////////////////////////////////////////////////////////////////
#/// @brief \link PPUTL_FIRST PPUTL_FIRST\endlink -
#/// return the first passed argument
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
#
#/// return the first passed argument
#///
#/// @ingroup pputl
#/// @anchor  PPUTL_FIRST
#/// @param   ... args
#/// @returns first arg
#/// @code
#///   #include <pputl/first.h>
#///   PPUTL_FIRST();     // expands to nothing
#///   PPUTL_FIRST(a);    // a
#///   PPUTL_FIRST(a, b); // a
#/// @endcode
#define PPUTL_FIRST(...) PPUTL_DETAIL_FIRST_X(__VA_ARGS__)
#
#define PPUTL_DETAIL_FIRST_X(...) \
  PPUTL_DETAIL_FIRST_CHOOSER(__VA_OPT__(, ) SIZEY, EMPTY)(__VA_ARGS__)
#define PPUTL_DETAIL_FIRST_CHOOSER(_, choice, ...) \
  PPUTL_CAT(PPUTL_DETAIL_FIRST_, choice)
#define PPUTL_DETAIL_FIRST_EMPTY()
#define PPUTL_DETAIL_FIRST_SIZEY(_1, ...) _1
#
#endif

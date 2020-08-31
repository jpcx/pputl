#ifndef PPUTL_NEQ_H_INCLUDED
#define PPUTL_NEQ_H_INCLUDED
#/////                                                                     c++20
#///////////////////////////////////////////////////////////////////////////////
#/// @brief \link PPUTL_NEQ PPUTL_NEQ\endlink -
#/// expands to 1 if two numbers a and b [0, 256) are NOT equal, or 0 if not
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
#include "pputl/eq.h"
#include "pputl/not.h"
#
#/// expands to 1 if two numbers a and b [0, 256) are NOT equal, or 0 if not
#/// @ingroup pputl
#/// @anchor  PPUTL_NEQ
#/// @param a number [0, 256)
#/// @param b number [0, 256)
#define PPUTL_NEQ(a, b) PPUTL_NOT(PPUTL_EQ(a, b))
#
#endif

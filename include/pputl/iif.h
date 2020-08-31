#ifndef PPUTL_IIF_H_INCLUDED
#define PPUTL_IIF_H_INCLUDED
#/////                                                                     c++20
#///////////////////////////////////////////////////////////////////////////////
#/// @brief \link PPUTL_IIF PPUTL_IIF\endlink -
#/// conditionally expands based on a 0 or 1 bit
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
#include "pputl/nothing.h"
#
#/// conditionally expands based on a 0 or 1 bit
#/// @ingroup pputl
#/// @anchor  PPUTL_IIF
#/// @param bit 0 or 1
#/// @returns if branch function (t, f) => t|f
#/// @code
#///   // PPUTL_IIF(bit)(t, f)
#///   PPUTL_IIF(1)("t", "f") // "t"
#///   PPUTL_IIF(0)("t", "f") // "f"
#/// @endcode
#define PPUTL_IIF(bit) PPUTL_CAT(PPUTL_DETAIL_IIF_, bit)
#
#define PPUTL_DETAIL_IIF_0(t, f) f
#define PPUTL_DETAIL_IIF_1(t, f) t
#
#endif

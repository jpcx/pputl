#ifndef PPUTL_TUPLE_ELEM_H_INCLUDED
#define PPUTL_TUPLE_ELEM_H_INCLUDED
#/////                                                                     c++20
#///////////////////////////////////////////////////////////////////////////////
#/// @brief \link PPUTL_TUPLE_ELEM PPUTL_TUPLE_ELEM\endlink -
#/// chooses tuple element at index idx [0, 256)
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
#//  Copyright (C) 2020, 2021 Justin Collier <m@jpcx.dev>
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
#
#/// chooses tuple element at index idx [0, 256)
#/// @ingroup tuple
#/// @anchor  PPUTL_TUPLE_ELEM
#/// @param idx - index [0, 256)
#/// @param tup - tuple
#define PPUTL_TUPLE_ELEM(idx, tup) PPUTL_CHOOSE(idx) tup
#
#endif

#ifndef PPUTL_TUPLE_ARGS_H_INCLUDED
#define PPUTL_TUPLE_ARGS_H_INCLUDED
#/////                                                                     c++20
#///////////////////////////////////////////////////////////////////////////////
#/// @brief \link PPUTL_TUPLE_ARGS PPUTL_TUPLE_ARGS\endlink -
#/// returns tuple elements as args
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
#/// returns tuple elements as args
#///
#/// @ingroup tuple
#/// @anchor  PPUTL_TUPLE_ARGS
#/// @code
#///   #define TUP (3, 2, 1)
#///   PPUTL_AT(0, PPUTL_TUPLE_ARGS(TUP)) // 3
#/// @endcode
#define PPUTL_TUPLE_ARGS(tup) PPUTL_TUPLE_ARGS_RETURN tup
#
#define PPUTL_TUPLE_ARGS_RETURN(...) __VA_ARGS__
#
#endif

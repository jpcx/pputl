#ifndef PPUTL_REST_H_INCLUDED
#define PPUTL_REST_H_INCLUDED
#/////                                                                     c++20
#///////////////////////////////////////////////////////////////////////////////
#/// @brief \link PPUTL_REST PPUTL_REST\endlink -
#/// return all passed arguments except for the first
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
#include <pputl/cat.h>
#
#/// return all passed arguments except for the first
#///
#/// @ingroup pputl
#/// @anchor  PPUTL_REST
#/// @param   ... args
#/// @returns all args but the first
#/// @code
#///   #include <pputl/rest.h>
#///   PPUTL_REST();        // expands to nothing
#///   PPUTL_REST(a);       // expands to nothing
#///   PPUTL_REST(a, b);    // b
#///   PPUTL_REST(a, b, c); // b, c
#/// @endcode
#define PPUTL_REST(...) PPUTL_DETAIL_REST_X(__VA_ARGS__)
#
#define PPUTL_DETAIL_REST_X(...) \
  PPUTL_DETAIL_REST_CHOOSER(__VA_OPT__(, ) SIZEY, EMPTY)(__VA_ARGS__)
#define PPUTL_DETAIL_REST_CHOOSER(_, choice, ...) \
  PPUTL_CAT(PPUTL_DETAIL_REST_, choice)
#define PPUTL_DETAIL_REST_EMPTY()
#define PPUTL_DETAIL_REST_SIZEY(_1, ...) __VA_ARGS__
#
#endif

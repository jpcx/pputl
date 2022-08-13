#pragma once
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

#include "codegen.h"

namespace api {

inline codegen::category<"config"> config;

inline std::string const uint_max_s{std::to_string(codegen::conf::uint_max) + "u"};
inline std::string const int_max_s{std::to_string(codegen::conf::int_max)};
inline std::string const int_min_s =
    "0x8" + codegen::utl::cat(std::vector<std::string>(codegen::conf::word_size - 1, "0"));

extern codegen::def<"build -> <c++ int>"> const& build;
extern codegen::def<"word_size -> uint"> const&  word_size;
extern codegen::def<"bit_length -> uint"> const&  bit_length;
extern codegen::def<"int_min -> binary"> const&  int_min;
extern codegen::def<"int_max -> int"> const&     int_max;
extern codegen::def<"uint_max -> uint"> const&   uint_max;

NIFTY_DECL(build);
NIFTY_DECL(word_size);
NIFTY_DECL(bit_length);
NIFTY_DECL(int_min);
NIFTY_DECL(int_max);
NIFTY_DECL(uint_max);

inline codegen::end_category<"config"> config_end;

} // namespace api

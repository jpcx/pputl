#ifndef CODEGEN_API_CONFIG_BUILD_CC
#define CODEGEN_API_CONFIG_BUILD_CC
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
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//                                                                            //
//  You should have received a copy of the GNU General Public License        ///
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.  ////
///////////////////////////////////////////////////////////////////////////// */

#include <chrono>
#include <iomanip>
#include <sstream>

#include "codegen.h"

namespace codegen {
namespace api {

namespace build_ {

using namespace std;

inline def<"build -> atom"> self = [] {
  category = "config";

  docs << "the build number of this pputl release (UTC ISO8601).";

  using chrono::system_clock;
  ostringstream ss;
  auto          t = system_clock::to_time_t(system_clock::now());
  ss << put_time(gmtime(&t), "%F%T");
  static regex repl{"[-:]", regex_constants::optimize};

  return regex_replace(ss.str(), repl, "");
};

} // namespace build_

inline constexpr auto& build = build_::self;

} // namespace api
} // namespace codegen

#endif

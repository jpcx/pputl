////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////

#include "build.h"

#include <chrono>
#include <iomanip>
#include <sstream>

namespace api {

using namespace codegen;

build::build() {
  def& build = *this;
  using codegen::def;
  build = [&] {
    category = "config";
    docs << "the build number of this pputl release (ISO8601).";
    using std::chrono::system_clock;
    std::ostringstream ss;
    auto               t = system_clock::to_time_t(system_clock::now());
    ss << std::put_time(gmtime(&t), "%F");
    static std::regex repl{"[-:]", std::regex_constants::optimize};
    return std::regex_replace(ss.str(), repl, "");
  };
}

} // namespace api

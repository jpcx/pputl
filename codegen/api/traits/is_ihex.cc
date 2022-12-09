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
#include "traits.h"

namespace api {

using namespace codegen;

decltype(is_ihex) is_ihex = NIFTY_DEF(is_ihex, [&](va args) {
  docs << "[extends is_atom] detects if args matches atoms 0x" + utl::cat(samp::hmin)
              + "|" + "0x" + utl::cat(samp::h1) + "|...|" + "0x"
              + utl::cat(svect{conf::word_size - 1, "F"}) + "E|" + "0x"
              + utl::cat(samp::hmax) + ".";

  auto min = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "0"));
  auto max = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "F"));

  tests << is_ihex("1")       = "0" >> docs;
  tests << is_ihex("1u")      = "0" >> docs;
  tests << is_ihex(min)       = "1" >> docs;
  tests << is_ihex(max)       = "1" >> docs;
  tests << is_ihex(max + "u") = "0" >> docs;
  tests << is_ihex("(), ()")  = "0" >> docs;

  def<"fail(...)"> fail{[&](va) {
    return "0";
  }};
  def<"\\0">       _0 = [&] {
    return fail;
  };
  def<"\\1">{} = [&] {
    return impl::is_ihex;
  };

  return pp::call(xcat(utl::slice(_0, -1), is_atom(args)), args);
});

} // namespace api

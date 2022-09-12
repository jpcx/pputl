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

#include "range.h"

namespace api {

using namespace codegen;

decltype(sizeof_) sizeof_ = NIFTY_DEF(sizeof_, [&](va args) {
  docs << "detects number of items. compatible with tup and all other pputl "
          "datastructures."
       << "fails if size is larger than " + size_max + " (" + size_max_s + ")"
       << ""
       << "note: complexity is O(size) for tup and O(1) for other pputl datastructures.";

  tests << sizeof_("()")        = "0u" >> docs;
  tests << sizeof_("(a)")       = "1u" >> docs;
  tests << sizeof_("(a, b)")    = "2u" >> docs;
  tests << sizeof_("(a, b, c)") = "3u";
  tests << sizeof_(pp::tup(utl::cat(std::array<std::string, conf::size_max>{}, ","))) =
      size_max_s;
  tests << sizeof_("(, )")    = "2u" >> docs;
  tests << sizeof_("(, , )")  = "3u";
  tests << sizeof_("(a, )")   = "2u";
  tests << sizeof_("(a, , )") = "3u";
  tests << sizeof_("(, a)")   = "2u";
  tests << sizeof_("(, a, )") = "3u";
  tests << sizeof_("(, , a)") = "3u";

  return esc(countof + " " + tup(args));
});

} // namespace api

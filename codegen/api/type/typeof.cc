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

#include "type.h"

namespace api {

using namespace codegen;

decltype(typeof) typeof = NIFTY_DEF(typeof, [&](va args) {
  docs << "detects the value type. will fail if the type is not tuple or uint."
       << "literal 0 and 1 are considered decimal rather than bool."
       << ""
       << "returns one of:"
       << "- " + tuple << "- " + binary << "- " + decimal;

  auto binmax = "0b" + utl::cat(std::vector<std::string>(conf::uint_bits, "1")) + "u";

  tests << typeof("(foo)") = tuple >> docs;
  tests << typeof(0)       = decimal >> docs;
  tests << typeof(binmax)  = binary >> docs;

  def<"oo_tuple(...)"> oo_tuple = [&](va) {
    return tuple;
  };

  def<"oo_no_tuple(...)"> oo_no_tuple = [&](va args) {
    def<"\\DEC"> dec = [&] {
      return decimal;
    };

    def<"\\BIN">{} = [&] {
      return binary;
    };

    return cat(utl::slice(dec, -3), first(cat(utl::slice(detail::uint_traits[0], -1), uint(args))));
  };

  return pp::call(def<"o(...)">{[&](va) {
                    std::string prefix = utl::slice(oo_tuple, -5);
                    if (prefix.back() == '_')
                      prefix.pop_back();
                    std::string tuple_s    = utl::slice(oo_tuple, prefix.size(), 0);
                    std::string no_tuple_s = utl::slice(oo_no_tuple, prefix.size(), 0);
                    std::string no_s       = utl::slice(no_tuple_s, -tuple_s.size());

                    return pp::cat(prefix, pp::va_opt(no_s), tuple_s);
                  }}(eat + " " + args),
                  args);
});

} // namespace api

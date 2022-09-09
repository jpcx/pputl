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

decltype(size) size = NIFTY_DEF(size, [&](va args) {
  docs << "[inherits from " + word + "] any non-negative word up to " + size_max + " ("
              + size_max_s + ")."
       << "constructibe from any word type."
       << ""
       << "cannot parse negative decimals; use numeric.neg instead."
       << "hex length is fixed. cannot parse shorter hex lengths."
       << ""
       << "see type.word for available cast modes."
       << ""
       << "preserves hex/decimal representation by default."
       << ""
       << "cast between signed and unsigned reinterprets bits."
       << ""
       << "values above the int max must have a 'u' suffix; implicit interpretation"
       << "as unsigned is not allowed (e.g. " + std::to_string(conf::uint_max)
              + " is not a valid integer).";

  tests << size(0)                         = "0" >> docs;
  tests << size(1)                         = "1" >> docs;
  tests << size("0x" + utl::cat(samp::h7)) = ("0x" + utl::cat(samp::h7)) >> docs;
  tests << size(size_max_s)                = size_max_s >> docs;

  return def<"o(e, w)">{[&](arg e, arg w) {
    def<"\\0(e, w)"> _0 = [&](arg e, arg) {
      return fail(e);
    };
    def<"\\1(e, w)">{} = [&](arg, arg w) {
      return w;
    };

    return pp::call(xcat(utl::slice(_0, -1), detail::is_size_o(w)), e, w);
  }}(str(pp::str("[" + size + "] invalid size; must be within 0 and " + size_max + " ("
                 + size_max_s + ")")
         + " : " + args),
     word(args));
});

} // namespace api

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

#include "bitwise.h"

namespace api {

using namespace codegen;

decltype(bget) bget = NIFTY_DEF(bget, [&](va args) {
  docs << "gets the ith bit from the word."
       << "i must be between 0 and " + bit_length + " ("
              + std::to_string(conf::bit_length) + ").";

  auto maxless1 =
      "0x" + utl::cat(std::vector<std::string>(conf::word_size - 1, "F")) + "E";

  tests << bget(2, conf::bit_length - 3)                   = "0" >> docs;
  tests << bget(2, conf::bit_length - 2)                   = "1" >> docs;
  tests << bget(2, conf::bit_length - 1)                   = "0" >> docs;
  tests << bget("5u", conf::bit_length - 3)                = "1" >> docs;
  tests << bget(maxless1, conf::bit_length - 2)            = "1" >> docs;
  tests << bget(maxless1 + "u", conf::bit_length - 1)      = "0" >> docs;
  tests << bget(pp::tup(samp::hmax), conf::bit_length - 1) = "1" >> docs;

  auto bitparams = utl::cat(utl::alpha_base52_seq(conf::bit_length), ", ");

  def _0 = def{"0(" + bitparams + ")"} = [&](pack args) { return args[0]; };

  for (std::size_t i = 1; i < conf::bit_length; ++i) {
    def{"" + std::to_string(i) + "(" + bitparams + ")"} = [&](pack args) {
      return args[i];
    };
  }

  return def<"o(e, v, i)">{[&](arg e, arg v, arg i) {
    return def<"<o(e, i, ...)">{[&](arg e, arg i, va bin) {
      return def<"<o(...)">{[&](va args) {
        return def<"<o(e, i, gelt, ...)">{[&](arg e, arg i, arg gelt, va args) {
          def<"\\0(e, ...)"> gelt0 = [&](arg e, va) { return fail(e); };

          def<"\\1(e, i, ...)">{} = [&](arg, arg i, va args) {
            return pp::call(pp::cat(utl::slice(_0, -1), i), args);
          };

          return pp::call(pp::cat(utl::slice(gelt0, -1), gelt), e, i, args);
        }}(args);
      }}(e, i, and_(ge(i, 0), lt(i, conf::bit_length)), bin);
    }}(e, idec(i), bdump(v));
  }}(str(pp::str("[" + bget + "] invalid index") + " : " + args), args);
});

} // namespace api

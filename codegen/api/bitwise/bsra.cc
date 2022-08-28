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

#include <span>

#include "bitwise.h"

namespace api {

using namespace codegen;

decltype(bsra) bsra = NIFTY_DEF(bsra, [&](va args) {
  docs << "performs an arithmetic bitwise right shift by n places.";

  tests << bsra(0, 1) = "0" >> docs;
  tests << bsra(1, 1) = "0";
  tests << bsra(2, 1) = "1" >> docs;
  tests << bsra(int_min_s, 1) =
      ("0xC" + utl::cat(svect(conf::word_size - 1, "0"))) >> docs;
  tests << bsra(int_min_s, 2) =
      ("0xE" + utl::cat(svect(conf::word_size - 1, "0"))) >> docs;
  tests << bsra(int_min_s, 3) =
      ("0xF" + utl::cat(svect(conf::word_size - 1, "0"))) >> docs;
  if constexpr (conf::word_size > 1)
    tests << bsra(int_min_s, 4) =
        ("0xF8" + utl::cat(svect(conf::word_size - 2, "0"))) >> docs;
  tests << bsra(3, 1) = "1";
  tests << bsra(4, 1) = "2";
  tests << bsra(4, 2) = "1";

  auto params = utl::cat(utl::alpha_base52_seq(conf::bit_length), ", ");

  def _0 = def{"0(" + params + ")"} = [&](pack) {
    std::array<std::string, conf::word_size> res{};
    for (std::size_t i = 0; i < res.size(); ++i) {
      res[i] = impl::nybl(
          pp::cat(utl::alpha_base52_seq(4, std::string{static_cast<char>('a' + i * 4)})),
          "HEX");
    }
    return pp::tup(res);
  };

  for (std::size_t i = 1; i < conf::bit_length; ++i) {
    def{"" + std::to_string(i) + "(" + params + ")"} = [&](pack args) {
      if (i == conf::bit_length - 1)
        docs << "bit shifts";

      svect shifted{args.begin(), args.end()};
      std::shift_right(shifted.begin(), shifted.end(), i);
      for (std::size_t j = 0; j < i; ++j)
        shifted[j] = args[0];
      svect res{};
      for (std::size_t j = 0; j < shifted.size(); j += 4) {
        res.push_back(impl::nybl(
            pp::cat(shifted[j + 0], shifted[j + 1], shifted[j + 2], shifted[j + 3]),
            "HEX"));
      }
      return pp::tup(res);
    };
  }

  return def<"o(v, ...)">{[&](arg v, va n) {
    return word(def<"<o(i, ...)">{[&](arg i, va bin) {
                  return def<"<o(...)">{[&](va args) {
                    return def<"<o(i, gelt, ...)">{[&](arg i, arg gelt, va args) {
                      def<"\\0(...)"> gelt0 = [&](va) { return "0"; };
                      def<"\\1(i, ...)">{}  = [&](arg i, va args) {
                        return pp::call(pp::cat(utl::slice(_0, -1), i), args);
                      };

                      return pp::call(pp::cat(utl::slice(gelt0, -1), gelt), i, args);
                    }}(args);
                  }}(i, lt(i, conf::bit_length), bin);
                }}(idec(default_(1, n)), bdump(word(v))),
                typeof(v));
  }}(args);
});

} // namespace api

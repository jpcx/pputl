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

decltype(bsll) bsll = NIFTY_DEF(bsll, [&](va args) {
  docs << "performs a logical bitwise left shift by n places.";

  tests << bsll(0, 1)                         = "0" >> docs;
  tests << bsll("1u", 1)                      = "2u" >> docs;
  tests << bsll("0x" + utl::cat(samp::h2), 2) = ("0x" + utl::cat(samp::h8)) >> docs;

  if constexpr (conf::word_size > 1) {
    tests << bsll("0x" + utl::cat(samp::h2), 3) = ("0x" + utl::cat(samp::h16)) >> docs;
    if constexpr (conf::word_size > 2)
      tests << bsll(uint_max_s, 3) =
          (std::to_string((conf::uint_max << 3) xor 0x7000) + "u") >> docs;
    else
      tests << bsll(uint_max_s, 3) =
          (std::to_string((conf::uint_max << 3) xor 0x700) + "u") >> docs;
  } else {
    tests << bsll(uint_max_s, 3) = "8u" >> docs;
  }

  if constexpr (conf::word_size > 2)
    tests << bsll(1, conf::bit_length - 2) = "1024" >> docs;

  tests << bsll(1, conf::bit_length - 1) = int_min_s >> docs;
  tests << bsll(1, conf::bit_length)     = "0" >> docs;
  tests << bsll(1, conf::bit_length + 1) = "0" >> docs;

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
      for (auto it = std::shift_left(shifted.begin(), shifted.end(), i);
           it != shifted.end(); ++it)
        *it = "0";
      svect res{};
      for (std::size_t j = 0; j < shifted.size(); j += 4) {
        res.push_back(
            pp::cat(shifted[j + 0], shifted[j + 1], shifted[j + 2], shifted[j + 3]));
      }
      for (auto&& v : res) {
        static std::regex const zerocat{"0##0", std::regex_constants::optimize};
        while (std::regex_search(v, zerocat))
          v = std::regex_replace(v, zerocat, "00");
        if (v != "0000")
          v = impl::nybl(v, "HEX");
        else
          v = "0";
      }
      return pp::tup(res);
    };
  }

  return def<"o(v, ct)">{[&](arg v, arg ct) {
    return word(def<"<o(i, ...)">{[&](arg i, va bin) {
                  return def<"<o(...)">{[&](va args) {
                    return def<"<o(i, gelt, ...)">{[&](arg i, arg gelt, va args) {
                      def<"0(...)"> gelt0 = [&](va) { return "0"; };
                      def<"1(i, ...)">{}  = [&](arg i, va args) {
                        return pp::call(pp::cat(utl::slice(_0, -1), i), args);
                      };

                      return pp::call(pp::cat(utl::slice(gelt0, -1), gelt), i, args);
                    }}(args);
                  }}(i, lt(i, conf::bit_length), bin);
                }}(idec(ct), bdump(v)),
                typeof(v));
  }}(args);
});

} // namespace api

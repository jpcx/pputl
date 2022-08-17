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

decltype(bitsrl) bitsrl = NIFTY_DEF(bitsrl, [&](va args) {
  docs << "performs a logical bitwise right shift by n places.";

  tests << bitsrl(0, 1)                            = "0" >> docs;
  tests << bitsrl(1, 1)                            = "0";
  tests << bitsrl(2, 1)                            = "1" >> docs;
  tests << bitsrl(3, 1)                            = "1";
  tests << bitsrl(4, 1)                            = "2" >> docs;
  tests << bitsrl(4, 2)                            = "1" >> docs;
  tests << bitsrl(int_min_s, conf::bit_length - 1) = ("0x" + utl::cat(samp::h1)) >> docs;
  tests << bitsrl(int_min_s, conf::bit_length - 0) =
      ("0x" + utl::cat(samp::hmin)) >> docs;

  def bin = def{"bin(" + utl::cat(utl::alpha_base52_seq(conf::word_size), ", ")
                + ")"} = [&](pack args) {
    std::vector<std::string> res{};
    std::ranges::transform(args, std::back_inserter(res),
                           [&](auto&& v) { return impl::hex(v, "BITS"); });
    return utl::cat(res, ", ");
  };

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
        shifted[j] = "0";
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
                }}(idec(ct), bin + " " + xword(v)),
                typeof(v));
  }}(args);
});

} // namespace api

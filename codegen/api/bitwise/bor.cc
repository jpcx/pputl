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

decltype(bor) bor = NIFTY_DEF(bor, [&](va args) {
  docs << "bitwise OR operation."
       << "uses arg 'a' for result cast hint.";

  tests << bor(0, 0)                 = "0" >> docs;
  tests << bor(0, 1)                 = "1" >> docs;
  tests << bor(3, 4)                 = "7" >> docs;
  tests << bor(5, 6)                 = "7" >> docs;
  tests << bor(int_min_s, int_max_s) = ("0x" + utl::cat(samp::hmax)) >> docs;

  if constexpr (conf::word_size > 1) {
    def<"x(...)"> x = [&](va args) { return args; };

    def<"r(...)"> r = [&](va args) {
      def o = def{"o(" + utl::cat(utl::alpha_base52_seq(conf::word_size * 2), ", ")
                  + ")"} = [&](pack args) {
        std::array<std::string, conf::word_size * 2> res{};
        res[0] = impl::hexhex(
            pp::cat(args[conf::word_size - 1], args[conf::word_size * 2 - 1]), "OR");
        for (std::size_t i = 1; i < res.size() / 2; ++i)
          res[i] = args[i - 1];
        res[res.size() / 2] = args.back();
        for (std::size_t i = res.size() / 2 + 1; i < res.size(); ++i)
          res[i] = args[i - 1];
        return utl::cat(res, ", ");
      };

      return o(args);
    };

    def<"res(...)"> res = [&](va args) {
      def o = def{"o(_type, " + utl::cat(utl::alpha_base52_seq(conf::word_size * 2), ", ")
                  + ")"} = [&](pack args) { //
        auto res = svect{next(args.begin()), std::prev(args.end(), conf::word_size)};
        return word(pp::tup(res), args.front());
      };
      return o(args);
    };

    return def<"o(a, b)">{[&](arg a, arg b) {
      return res(typeof(a), def<"<o(...)">{[&](va args) {
                   return utl::cat(svect(conf::word_size, r + "(")) + args
                        + utl::cat(svect(conf::word_size, ")"));
                 }}(x(esc + " " + utup(a), esc + " " + utup(b))));
    }}(args);
  } else {
    return def<"o(a, b)">{[&](arg a, arg b) {
      return word(pp::tup(def<"<o(a, b)">{[&](arg a, arg b) {
                    return impl::hexhex(cat(a, b), "OR");
                  }}(esc + " " + utup(a), esc + " " + utup(b))),
                  typeof(a));
    }}(args);
  }
});

} // namespace api

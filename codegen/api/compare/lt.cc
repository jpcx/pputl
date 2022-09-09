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

#include "compare.h"

namespace api {

using namespace codegen;

decltype(lt) lt = NIFTY_DEF(lt, [&](va args) {
  docs << "word less-than comparison."
       << "prohibits comparison of different signedness."
       << "utups are interpreted as (and are comparable with) unsigned.";

  using std::to_string;
  using conf::uint_max;
  using conf::int_max;

  tests << lt("0, 0")                                        = "0" >> docs;
  tests << lt("0, 1")                                        = "1" >> docs;
  tests << lt("7u, 8u")                                      = "1" >> docs;
  tests << lt("8u, 7u")                                      = "0";
  tests << lt(int_(uint_max_s), "0")                         = "1" >> docs;
  tests << lt(int_max_s, int_min_s)                          = "0" >> docs;
  tests << lt(int_min_s, int_max_s)                          = "1";
  tests << lt(int_min_s, int_(to_string(int_max + 1) + "u")) = "0" >> docs;
  tests << lt(int_min_s, int_(to_string(int_max + 2) + "u")) = "1" >> docs;
  tests << lt("0u", uint_max_s)                              = "1";
  tests << lt(uint_max_s, "0u")                              = "0";
  tests << lt(to_string(uint_max / 2) + "u", to_string((uint_max / 2) - 1) + "u") = "0";
  tests << lt(to_string(uint_max / 2) + "u", to_string((uint_max / 2)) + "u")     = "0";
  tests << lt(to_string(uint_max / 2) + "u", to_string((uint_max / 2) + 1) + "u") = "1";
  tests << lt(to_string(int_max / 2), to_string((int_max / 2) - 1))               = "0";
  tests << lt(to_string(int_max / 2), to_string((int_max / 2)))                   = "0";
  tests << lt(to_string(int_max / 2), to_string((int_max / 2) + 1))               = "1";
  tests << lt(pp::tup(samp::hmax), pp::tup(samp::hmin)) = "0" >> docs;
  tests << lt(pp::tup(samp::hmin), pp::tup(samp::hmax)) = "1" >> docs;
  tests << lt(pp::tup(samp::himax), std::to_string(conf::int_max + 1) + "u") =
      "1" >> docs;
  tests << lt(std::to_string(conf::int_max + 1) + "u", pp::tup(samp::himax)) =
      "0" >> docs;

  def<"icmp(...)"> icmp = [&](va args) {
    return def<"o(...)">{[&](va args) {
      def o = def{"<o(" + utl::cat(utl::alpha_base52_seq(conf::word_size * 2), ", ")
                  + ")"} = [&](pack args) {
        def<"\\00(...)"> _00 = [&](va args) {
          return impl::lt_ucmp(args);
        };
        def<"\\01(...)">{} = [&](va) {
          return "0";
        };
        def<"\\10(...)">{} = [&](va) {
          return "1";
        };
        def<"\\11(...)">{} = [&](va args) {
          return impl::lt_ucmp(args);
        };

        return pp::call(
            xcat(utl::slice(_00, -2),
                 xcat(impl::hexhex(pp::cat("7", args[0]), "LT"),
                      impl::hexhex(pp::cat("7", args[conf::word_size]), "LT"))),
            args);
      };

      return o(args);
    }}(args);
  };

  def<"signof(word)"> signof = [&](arg word) {
    def<"\\0(atom)"> _0 = [&](arg atom) {
      def<"<\\01"> _01 = [&] {
        return "U";
      };
      def<"<\\10">{} = [&] {
        return "I";
      };
      return xcat(utl::slice(_01, -2),
                  xcat(detail::is_int_o(atom), detail::is_uint_o(atom)));
    };
    def<"\\1(tup)">{} = [&](arg) {
      return "U";
    };
    return pp::call(xcat(utl::slice(_0, -1), detail::is_tup_o(word)), word);
  };

  return pp::call(
      def<"o(l, r)">{[&](arg l, arg r) {
        def<"\\II(e, l, r)"> intint = [&](arg, arg l, arg r) {
          return icmp(esc + " " + utup(l), esc + " " + utup(r));
        };
        def<"\\IU(e, l, r)">{} = [&](arg e, arg, arg) {
          return fail(e);
        };
        def<"\\UI(e, l, r)">{} = [&](arg e, arg, arg) {
          return fail(e);
        };
        def<"\\UU(e, l, r)">{} = [&](arg, arg l, arg r) {
          return impl::lt(utup(l), utup(r));
        };

        return xcat(utl::slice(intint, -2), xcat(signof(word(l)), signof(word(r))));
      }}(args),
      str(pp::str("[" + lt + "] comparison of different signedness not allowed") + " : "
          + args),
      args);
});

} // namespace api

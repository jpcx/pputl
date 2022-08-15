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
  docs << "integral less-than comparison."
       << "prohibits comparison of different signedness.";

  using std::to_string;
  using conf::uint_max;
  using conf::int_max;

  tests << lt("0, 0")                                                             = "0" >> docs;
  tests << lt("0, 1")                                                             = "1" >> docs;
  tests << lt("7u, 8u")                                                           = "1" >> docs;
  tests << lt("8u, 7u")                                                           = "0";
  tests << lt(int_(uint_max_s), "0")                                              = "1" >> docs;
  tests << lt(int_max_s, int_min_s)                                               = "0" >> docs;
  tests << lt(int_min_s, int_max_s)                                               = "1";
  tests << lt(int_min_s, int_(to_string(int_max + 1) + "u"))                      = "0" >> docs;
  tests << lt(int_min_s, int_(to_string(int_max + 2) + "u"))                      = "1" >> docs;
  tests << lt("0u", uint_max_s)                                                   = "1";
  tests << lt(uint_max_s, "0u")                                                   = "0";
  tests << lt(to_string(uint_max / 2) + "u", to_string((uint_max / 2) - 1) + "u") = "0";
  tests << lt(to_string(uint_max / 2) + "u", to_string((uint_max / 2)) + "u")     = "0";
  tests << lt(to_string(uint_max / 2) + "u", to_string((uint_max / 2) + 1) + "u") = "1";
  tests << lt(to_string(int_max / 2), to_string((int_max / 2) - 1))               = "0";
  tests << lt(to_string(int_max / 2), to_string((int_max / 2)))                   = "0";
  tests << lt(to_string(int_max / 2), to_string((int_max / 2) + 1))               = "1";

  def<"000"> _000 = [&] { return "0"; };
  def<"001">{}    = [&] { return "1"; };
  def<"010">{}    = [&] { return "0"; };
  def<"011">{}    = [&] { return "0"; };
  def<"100">{}    = [&] { return "1"; };
  def<"101">{}    = [&] { return "1"; };
  def<"110">{}    = [&] { return "1"; };
  def<"111">{}    = [&] { return "1"; };

  def<"r(...)"> recur = [&](va args) {
    return def<"o(fl, fg, a, b, ...)">{[&](arg fl, arg fg, arg a, arg b, va args) {
      return cat(pp::cat(utl::slice(_000, -3), fl, fg), impl::hexhex(pp::cat(a, b), "LT")) + ", "
           + cat(pp::cat(utl::slice(_000, -3), fg, fl), impl::hexhex(pp::cat(b, a), "LT")) + ", "
           + args;
    }}(args);
  };

  def<"zip(...)"> zip = [&](va args) {
    return def{"o(" + utl::cat(utl::alpha_base52_seq(conf::word_size * 2), ", ") + ")",
               [&](pack args) {
                 std::vector<std::string> res;
                 for (std::size_t i = 0; i < args.size() / 2; ++i) {
                   res.push_back(args[i]);
                   res.push_back(args[i + (args.size() / 2)]);
                 }
                 return utl::cat(res, ", ");
               }}(args);
  };

  def<"ucmp(...)"> ucmp = [&](va args) {
    return first(utl::cat(std::vector<std::string>(conf::word_size, recur + "(")) + "0, 0, "
                 + zip(args) + utl::cat(std::vector<std::string>(conf::word_size, ")")));
  };

  def<"icmp(lw, rw)"> icmp = [&](arg lw, arg rw) {
    def<"00(lw, rw)"> _00 = [&](arg lw, arg rw) { return ucmp(esc + " " + lw, esc + " " + rw); };
    def<"01(lw, rw)">{}   = [&](arg, arg) { return "0"; };
    def<"10(lw, rw)">{}   = [&](arg, arg) { return "1"; };
    def<"11(lw, rw)">{}   = [&](arg lw, arg rw) { return ucmp(esc + " " + lw, esc + " " + rw); };

    return pp::call(
        cat(utl::slice(_00, -2), cat(impl::hexhex(cat("7", esc(ifirst + " " + lw)), "LT"),
                                     impl::hexhex(cat("7", esc(ifirst + " " + rw)), "LT"))),
        lw, rw);
  };

  return pp::call(
      def<"o(l, r)">{[&](arg l, arg r) {
        return def<"<o(...)">{[&](va args) {
          return def<"<o(il, ir, ul, ur)">{[&](arg il, arg ir, arg ul, arg ur) {
            // non-integer l and r
            def<"0000(e0, e1, l, r)"> _0000 = [&](arg e0, arg, arg, arg) { return fail(e0); };
            // non-integer l, unsigned r
            def<"0001(e0, e1, l, r)">{}     = [&](arg e0, arg, arg, arg) { return fail(e0); };
            // unsigned l, non-integer r
            def<"0010(e0, e1, l, r)">{}     = [&](arg e0, arg, arg, arg) { return fail(e0); };
            // unsigned l and r
            def<"0011(e0, e1, l, r)">{}     = [&](arg, arg, arg l, arg r) {
              return ucmp(esc + " " + xword(l), esc + " " + xword(r));
            };
            // non-integer l, signed r
            def<"0100(e0, e1, l, r)">{} = [&](arg e0, arg, arg, arg) { return fail(e0); };
            // unsigned l, signed r
            def<"0110(e0, e1, l, r)">{} = [&](arg, arg e1, arg, arg) { return fail(e1); };
            // signed l, non-integer r
            def<"1000(e0, e1, l, r)">{} = [&](arg e0, arg, arg, arg) { return fail(e0); };
            // signed l, unsigned r
            def<"1001(e0, e1, l, r)">{} = [&](arg, arg e1, arg, arg) { return fail(e1); };
            // signed l, signed r
            def<"1100(e0, e1, l, r)">{} = [&](arg, arg, arg l, arg r) {
              return icmp(xword(l), xword(r));
            };

            return pp::cat(utl::slice(_0000, -4), il, ir, ul, ur);
          }}(args);
        }}(is_int(l), is_int(r), is_uint(l), is_uint(r));
      }}(args),
      istr("[" + lt + "] one or more invalid integer : " + args),
      istr("[" + lt + "] comparison of different signedness not allowed : " + args), args);
});

} // namespace api

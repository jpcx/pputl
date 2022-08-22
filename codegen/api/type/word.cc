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

decltype(word) word = NIFTY_DEF(word, [&](va args) {
  docs << "[inherits from " + obj + "] a union of int|uint|utup."
       << "constructibe from any word."
       << ""
       << "cannot parse negative decimals; use math.neg instead."
       << "hex length is fixed. cannot parse shorter hex lengths."
       << ""
       << "cast modes:"
       << ""
       << "  idec → idec | [default]"
       << "  idec → ihex | requires IHEX hint"
       << "  idec → udec | requires UDEC hint"
       << "  idec → uhex | requires UHEX hint"
       << "  idec → utup | requires UTUP hint"
       << ""
       << "  ihex → ihex | [default]; failback for failed ihex → idec"
       << "  ihex → idec | requires IDEC hint and positive result"
       << "  ihex → udec | requires UDEC hint"
       << "  ihex → uhex | requires UHEX hint"
       << "  ihex → utup | requires UTUP hint"
       << ""
       << "  udec → udec | [default]"
       << "  udec → idec | requires IDEC hint and positive result"
       << "  udec → ihex | requires IHEX hint or udec → idec failure"
       << "  udec → uhex | requires UHEX hint"
       << "  udec → utup | requires UTUP hint"
       << ""
       << "  uhex → uhex | [default]"
       << "  uhex → idec | requires IDEC hint and positive result"
       << "  uhex → ihex | requires IHEX hint or uhex → idec failure"
       << "  uhex → udec | requires UDEC hint"
       << "  uhex → utup | requires UTUP hint"
       << ""
       << "  utup → utup | [default]"
       << "  utup → idec | requires IDEC hint and positive result"
       << "  utup → ihex | requires IHEX hint or utup → idec failure"
       << "  utup → udec | requires UDEC hint"
       << "  utup → uhex | requires UHEX hint"
       << ""
       << "attempts to preserve hex/decimal representation by default, but"
       << "will output hex if casting the input yields a negative number."
       << "hint is ignored only if the result is negative and the hint is IDEC."
       << ""
       << "cast between signed and unsigned reinterprets bits."
       << ""
       << "values above the int max must have a 'u' suffix; implicit interpretation"
       << "as unsigned is not allowed (e.g. " + std::to_string(conf::uint_max)
              + " is not a valid integer).";

  // idec  → idec  | [default]
  tests << word(0)         = "0" >> docs;
  // idec  → ihex  | requires IHEX hint
  tests << word(1, "IHEX") = ("0x" + utl::cat(samp::h1)) >> docs;
  // idec  → udec  | requires UDEC hint
  tests << word(2, "UDEC") = "2u" >> docs;
  // idec  → uhex  | requires UHEX hint
  tests << word(3, "UHEX") = ("0x" + utl::cat(samp::h3) + "u") >> docs;
  // idec  → utup | requires UTUP hint
  tests << word(4, "UTUP") = pp::tup(samp::h4) >> docs;

  // ihex  → ihex  | [default]; failback for failed ihex → idec
  tests << word("0x" + utl::cat(samp::h2)) = ("0x" + utl::cat(samp::h2)) >> docs;
  tests << word("0x" + utl::cat(samp::himin), "IDEC") =
      ("0x" + utl::cat(samp::himin)) >> docs;
  // ihex  → idec  | requires IDEC hint and positive result
  tests << word("0x" + utl::cat(samp::h2), "IDEC")  = "2" >> docs;
  // ihex  → udec  | requires UDEC hint
  tests << word("0x" + utl::cat(samp::h15), "UDEC") = "15u" >> docs;
  // ihex  → uhex  | requires UHEX hint
  tests << word("0x" + utl::cat(samp::h7), "UHEX") =
      ("0x" + utl::cat(samp::h7) + "u") >> docs;
  // ihex  → utup | requires UTUP hint
  tests << word("0x" + utl::cat(samp::h8), "UTUP") = pp::tup(samp::h8) >> docs;

  // udec  → udec  | [default]
  tests << word("8u")               = "8u" >> docs;
  // udec  → idec  | requires IDEC hint and positive result
  tests << word("7u", "IDEC")       = "7" >> docs;
  // udec  → ihex  | requires IHEX hint or udec → idec failure
  tests << word("15u", "IHEX")      = ("0x" + utl::cat(samp::h15)) >> docs;
  tests << word(uint_max_s, "IDEC") = ("0x" + utl::cat(samp::hmax)) >> docs;
  // udec  → uhex  | requires UHEX hint
  tests << word("6u", "UHEX")       = ("0x" + utl::cat(samp::h6) + "u") >> docs;
  // udec  → utup | requires UTUP hint
  tests << word("4u", "UTUP")       = pp::tup(samp::h4) >> docs;

  // uhex  → uhex  | [default]
  tests << word("0x" + utl::cat(samp::h5) + "u") =
      ("0x" + utl::cat(samp::h5) + "u") >> docs;
  // uhex  → idec  | requires IDEC hint and positive result
  tests << word("0x" + utl::cat(samp::h5) + "u", "IDEC") = "5" >> docs;
  // uhex  → ihex  | requires IHEX hint or uhex → idec failure
  tests << word("0x" + utl::cat(samp::h7) + "u", "IHEX") =
      ("0x" + utl::cat(samp::h7)) >> docs;
  tests << word("0x" + utl::cat(samp::hmax) + "u", "IDEC") =
      ("0x" + utl::cat(samp::hmax)) >> docs;
  // uhex  → udec  | requires UDEC hint
  tests << word("0x" + utl::cat(samp::h4) + "u", "UDEC")  = "4u" >> docs;
  // uhex  → utup | requires UTUP hint
  tests << word("0x" + utl::cat(samp::h15) + "u", "UTUP") = pp::tup(samp::h15) >> docs;

  // utup → utup | [default]
  tests << word(pp::tup(samp::h3))            = pp::tup(samp::h3) >> docs;
  // utup → idec  | requires IDEC hint and positive result
  tests << word(pp::tup(samp::himax), "IDEC") = int_max_s >> docs;
  // utup → ihex  | requires IHEX hint or utup → idec failure
  tests << word(pp::tup(samp::hmin), "IHEX")  = ("0x" + utl::cat(samp::hmin)) >> docs;
  tests << word(pp::tup(samp::himin), "IDEC") = ("0x" + utl::cat(samp::himin)) >> docs;
  // utup → udec  | requires UDEC hint
  tests << word(pp::tup(samp::hmax), "UDEC")  = uint_max_s >> docs;
  // utup → uhex  | requires UHEX hint
  tests << uint(pp::tup(samp::hmin), "UHEX") =
      ("0x" + utl::cat(samp::hmin) + "u") >> docs;

  def<"mode(e, t, ...: <err>, <typeof v>, <hint>) -> <cast mode>"> mode =
      [&](arg e, arg t, va hint) {
        docs << "cast mode selector and error detector";

        def<"0\\IDEC"> _0idec = [&] { return ""; };
        def<"0\\IHEX">{}      = [&] { return ""; };
        def<"0\\UDEC">{}      = [&] { return ""; };
        def<"0\\UHEX">{}      = [&] { return ""; };
        def<"0\\UTUP">{}      = [&] { return ""; };
        def<"1\\IDEC"> _1idec = [&] { return ""; };
        def<"1\\IHEX">{}      = [&] { return ""; };
        def<"1\\UDEC">{}      = [&] { return ""; };
        def<"1\\UHEX">{}      = [&] { return ""; };
        def<"1\\UTUP">{}      = [&] { return ""; };
        def<"1">{}            = [&] { return ""; };

        def<"\\00(e, t, ...)"> _00 = [&](arg e, arg, va) { return fail(e); };
        def<"\\01(e, t, ...)">{}   = [&](arg e, arg, va) { return fail(e); };
        def<"\\10(e, t, ...)">{}   = [&](arg e, arg, va) { return fail(e); };
        def<"\\11(e, t, ...)">{} = [&](arg, arg t, va hint) { return default_(t, hint); };

        return pp::call(xcat(utl::slice(_00, -2),
                             xcat(is_none(xcat(utl::slice(_0idec, -4), t)),
                                  is_none(pp::cat(utl::slice(_1idec, -4), hint)))),
                        e, t, hint);
      };

  auto utup_params = utl::alpha_base52_seq(conf::word_size);
  for (auto&& v : utup_params)
    if (v == "u" or v == "x") {
      v = "_" + v;
    }

  def<"\\IDEC(word)"> idec = [&](arg word) { return int_(word, "IDEC"); };
  def<"\\IHEX(word)">{}    = [&](arg word) { return int_(word, "IHEX"); };
  def<"\\UDEC(word)">{}    = [&](arg word) { return uint(word, "UDEC"); };
  def<"\\UHEX(word)">{}    = [&](arg word) { return uint(word, "UHEX"); };
  def<"\\UTUP(word)">{}    = [&](arg word) { return utup(word); };

  return def<"o(e, v, ...)">{[&](arg e, arg v, va hint) {
    return pp::call(xcat(utl::slice(idec, -4), mode(e, typeof(v), hint)), v);
  }}(str("[" + word + "] invalid arguments : " + args), args);
});

} // namespace api

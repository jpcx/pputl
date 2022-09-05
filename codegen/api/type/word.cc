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
  docs << "[union " + int_ + "|" + uint + "] any defined integer representation."
       << "constructibe from any word type."
       << ""
       << "cannot parse negative decimals; use numeric.neg instead."
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

  def<"hint_\\IDEC"> hint_idec = [&] { return ""; };
  def<"hint_\\IHEX">{}         = [&] { return ""; };
  def<"hint_\\UDEC">{}         = [&] { return ""; };
  def<"hint_\\UHEX">{}         = [&] { return ""; };
  def<"hint_\\UTUP">{}         = [&] { return ""; };
  def<"hint_\\AUTO">{}         = [&] { return ""; };

  def<"mode(e, t, hint)"> mode = [&](arg e, arg t, arg hint) {
    docs << "cast mode selector and error detector";

    def<"\\IDEC"> idec_ = [&] { return ""; };
    def<"\\IHEX">{}     = [&] { return ""; };
    def<"\\UDEC">{}     = [&] { return ""; };
    def<"\\UHEX">{}     = [&] { return ""; };
    def<"\\UTUP">{}     = [&] { return ""; };

    def<"\\0(e, t, hint)"> _0 = [&](arg e, arg, arg) { return fail(e); };
    def<"\\1(e, t, hint)">{}  = [&](arg, arg t, arg hint) {
      def<"\\IDEC(t)"> idec_ = [&](arg) { return "IDEC"; };
      def<"\\IHEX(t)">{}     = [&](arg) { return "IHEX"; };
      def<"\\UDEC(t)">{}     = [&](arg) { return "UDEC"; };
      def<"\\UHEX(t)">{}     = [&](arg) { return "UHEX"; };
      def<"\\UTUP(t)">{}     = [&](arg) { return "UTUP"; };
      def<"\\AUTO(t)">{}     = [&](arg t) { return t; };

      return pp::call(pp::cat(utl::slice(idec_, -4), hint), t);
    };

    return pp::call(xcat(utl::slice(_0, -1), is_none(xcat(utl::slice(idec_, -4), t))), e,
                    t, hint);
  };

  auto utup_params = utl::alpha_base52_seq(conf::word_size);
  for (auto&& v : utup_params)
    if (v == "u" or v == "x") {
      v = "_" + v;
    }

  def<"\\IDEC(word)"> idec = [&](arg word) { return int_(word, "IDEC"); };
  def<"\\IHEX(word)">{}    = [&](arg word) { return ihex(word); };
  def<"\\UDEC(word)">{}    = [&](arg word) { return udec(word); };
  def<"\\UHEX(word)">{}    = [&](arg word) { return uhex(word); };
  def<"\\UTUP(word)">{}    = [&](arg word) { return utup(word); };

  return def<"o(e, v, ...)">{[&](arg e, arg v, va hint) {
    return pp::call(xcat(utl::slice(idec, -4),
                         mode(e, typeof(v),
                              enum_(utl::slice(hint_idec, -4), default_("AUTO", hint)))),
                    v);
  }}(str(pp::str("[" + word + "] invalid arguments") + " : " + args), args);
});

} // namespace api

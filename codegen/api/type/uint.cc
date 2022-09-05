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

decltype(uint) uint = NIFTY_DEF(uint, [&](va args) {
  docs << "[union " + udec + "|" + uhex + "|" + utup + "] "
              + std::to_string(conf::word_size * 4) + "-bit unsigned integer type."
       << "constructible from any word type."
       << "instance is either udec, uhex, or utup."
       << ""
       << "cannot parse negative decimals; use numeric.neg instead."
       << "hex length is fixed. cannot parse shorter hex lengths."
       << ""
       << "cast modes:"
       << ""
       << "  idec → udec | [default]"
       << "  idec → uhex | requires UHEX hint"
       << "  idec → utup | requires UTUP hint"
       << ""
       << "  ihex → uhex | [default]"
       << "  ihex → udec | requires UDEC hint"
       << "  ihex → utup | requires UTUP hint"
       << ""
       << "  udec → udec | [default]"
       << "  udec → uhex | requires UHEX hint"
       << "  udec → utup | requires UTUP hint"
       << ""
       << "  uhex → uhex | [default]"
       << "  uhex → udec | requires UDEC hint"
       << "  uhex → utup | requires UTUP hint"
       << ""
       << "  utup → uhex | requires UHEX hint"
       << "  utup → udec | requires UDEC hint"
       << "  utup → utup | [default]"
       << ""
       << "preserves hex/decimal/tup representation."
       << "specify hint to choose a cast mode."
       << ""
       << "uses a 'u' suffix for hex/decimal representations."
       << "see fmt.paste_uint to remove suffix before pasting."
       << ""
       << "cast from signed reinterprets bits as unsigned."
       << ""
       << "values above the int max must have a 'u' suffix; implicit interpretation"
       << "as unsigned is not allowed (e.g. " + std::to_string(conf::uint_max)
              + " is not a valid integer).";

  // idec  → udec | [default]
  tests << uint(0)         = "0u" >> docs;
  // idec  → uhex | requires UHEX hint
  tests << uint(2, "UHEX") = ("0x" + utl::cat(samp::h2) + "u") >> docs;
  // idec  → utup | requires UTUP hint
  tests << uint(4, "UTUP") = pp::tup(samp::h4) >> docs;

  // ihex  → uhex | [default]
  tests << uint("0x" + utl::cat(samp::h7)) = ("0x" + utl::cat(samp::h7) + "u") >> docs;
  // ihex  → udec | requires UDEC hint
  tests << uint("0x" + utl::cat(samp::h15), "UDEC") = "15u" >> docs;
  // ihex  → utup | requires UTUP hint
  tests << uint("0x" + utl::cat(samp::h12), "UTUP") = pp::tup(samp::h12) >> docs;

  // udec  → udec | [default]
  tests << uint("8u")         = "8u" >> docs;
  // udec  → uhex | requires UHEX hint
  tests << uint("6u", "UHEX") = ("0x" + utl::cat(samp::h6) + "u") >> docs;
  // udec  → utup | requires UTUP hint
  tests << uint("4u", "UTUP") = pp::tup(samp::h4) >> docs;

  // uhex  → uhex | [default]
  tests << uint("0x" + utl::cat(samp::h5) + "u") =
      ("0x" + utl::cat(samp::h5) + "u") >> docs;
  // uhex  → udec | requires UDEC hint
  tests << uint("0x" + utl::cat(samp::h4) + "u", "UDEC") = "4u" >> docs;
  // uhex  → utup | requires UTUP hint
  tests << uint("0x" + utl::cat(samp::h3) + "u", "UTUP") = pp::tup(samp::h3) >> docs;

  // utup → uhex | requires UHEX hint
  tests << uint(pp::tup(samp::hmin), "UHEX") =
      ("0x" + utl::cat(samp::hmin) + "u") >> docs;
  // utup → udec | requires UDEC hint
  tests << uint(pp::tup(samp::hmax), "UDEC") = uint_max_s >> docs;
  // utup → utup | [default]
  tests << uint(pp::tup(samp::himax))        = pp::tup(samp::himax) >> docs;

  def<"hint_\\UDEC"> hint_udec = [&] { return ""; };
  def<"hint_\\UHEX">{}         = [&] { return ""; };
  def<"hint_\\UTUP">{}         = [&] { return ""; };
  def<"hint_\\AUTO">{}         = [&] { return ""; };

  def<"mode(e, t, hint) -> <cast mode>"> mode = [&](arg e, arg t, arg hint) {
    docs << "cast mode selector and error detector";

    def<"\\IDEC"> idec_ = [&] { return ""; };
    def<"\\IHEX">{}     = [&] { return ""; };
    def<"\\UDEC">{}     = [&] { return ""; };
    def<"\\UHEX">{}     = [&] { return ""; };
    def<"\\UTUP">{}     = [&] { return ""; };

    def<"\\0(e, t, hint)"> _0 = [&](arg e, arg, arg) { return fail(e); };
    def<"\\1(e, t, hint)">{}  = [&](arg, arg t, arg hint) {
      def<"\\IDECUDEC"> idecidec = [&] { return "ID_UD"; };
      def<"\\IDECUHEX">{}        = [&] { return "ID_UH"; };
      def<"\\IDECUTUP">{}        = [&] { return "ID_UT"; };
      def<"\\IDECAUTO">{}        = [&] { return "ID_UD"; };
      def<"\\IHEXUDEC">{}        = [&] { return "IH_UD"; };
      def<"\\IHEXUHEX">{}        = [&] { return "IH_UH"; };
      def<"\\IHEXUTUP">{}        = [&] { return "IH_UT"; };
      def<"\\IHEXAUTO">{}        = [&] { return "IH_UH"; };
      def<"\\UDECUDEC">{}        = [&] { return "UD_UD"; };
      def<"\\UDECUHEX">{}        = [&] { return "UD_UH"; };
      def<"\\UDECUTUP">{}        = [&] { return "UD_UT"; };
      def<"\\UDECAUTO">{}        = [&] { return "UD_UD"; };
      def<"\\UHEXUDEC">{}        = [&] { return "UH_UD"; };
      def<"\\UHEXUHEX">{}        = [&] { return "UH_UH"; };
      def<"\\UHEXUTUP">{}        = [&] { return "UH_UT"; };
      def<"\\UHEXAUTO">{}        = [&] { return "UH_UH"; };
      def<"\\UTUPUDEC">{}        = [&] { return "UT_UD"; };
      def<"\\UTUPUHEX">{}        = [&] { return "UT_UH"; };
      def<"\\UTUPUTUP">{}        = [&] { return "UT_UT"; };
      def<"\\UTUPAUTO">{}        = [&] { return "UT_UT"; };

      return pp::cat(utl::slice(idecidec, -8), t, hint);
    };

    return pp::call(xcat(utl::slice(_0, -1), is_none(xcat(utl::slice(idec_, -4), t))), e,
                    t, hint);
  };

  auto utup_params = utl::alpha_base52_seq(conf::word_size);
  for (auto&& v : utup_params)
    if (v == "u" or v == "x") {
      v = "_" + v;
    }

  def<"\\ID_UD(idec)"> id_id = [&](arg idec) { return pp::cat(idec, 'u'); };
  def<"\\ID_UH(idec)">{}     = [&](arg idec) {
    return impl::udec(pp::cat(idec, 'u'), "UHEX");
  };
  def<"\\ID_UT(idec)">{} = [&](arg idec) {
    return impl::uhex(impl::udec(pp::cat(idec, 'u'), "UHEX"), "UTUP");
  };
  def<"\\IH_UD(ihex)">{} = [&](arg ihex) {
    return impl::uhex(pp::cat(ihex, 'u'), "UDEC");
  };
  def<"\\IH_UH(ihex)">{} = [&](arg ihex) { return pp::cat(ihex, 'u'); };
  def<"\\IH_UT(ihex)">{} = [&](arg ihex) {
    return impl::uhex(pp::cat(ihex, 'u'), "UTUP");
  };
  def<"\\UD_UD(udec)">{} = [&](arg udec) { return udec; };
  def<"\\UD_UH(udec)">{} = [&](arg udec) { return impl::udec(udec, "UHEX"); };
  def<"\\UD_UT(udec)">{} = [&](arg udec) {
    return impl::uhex(impl::udec(udec, "UHEX"), "UTUP");
  };
  def<"\\UH_UD(uhex)">{} = [&](arg uhex) { return impl::uhex(uhex, "UDEC"); };
  def<"\\UH_UH(uhex)">{} = [&](arg uhex) { return uhex; };
  def<"\\UH_UT(uhex)">{} = [&](arg uhex) { return impl::uhex(uhex, "UTUP"); };
  def<"\\UT_UD(utup)">{} = [&](arg utup) {
    def o = def{"o(" + utl::cat(utup_params, ", ") + ")"} = [&](pack args) {
      return impl::uhex(pp::cat("0x", pp::cat(args), "u"), "UDEC");
    };
    return o + " " + utup;
  };
  def<"\\UT_UH(utup)">{} = [&](arg utup) {
    def o = def{"o(" + utl::cat(utup_params, ", ") + ")"} = [&](pack args) {
      return pp::cat("0x", pp::cat(args), "u");
    };
    return o + " " + utup;
  };
  def<"\\UT_UT(utup)">{} = [&](arg utup) { return utup; };

  return def<"o(e, v, ...)">{[&](arg e, arg v, va hint) {
    return pp::call(xcat(utl::slice(id_id, -5),
                         mode(e, typeof(v),
                              enum_(utl::slice(hint_udec, -4), default_("AUTO", hint)))),
                    v);
  }}(str(pp::str("[" + uint + "] invalid arguments") + " : " + args), args);
});

} // namespace api

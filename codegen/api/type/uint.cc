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
  docs << "[inherits from " + atom + "] " + std::to_string(conf::word_size * 4)
              + "-bit unsigned integer type."
       << "constructible from any word."
       << "instance is either udec or uhex."
       << ""
       << "cannot parse negative decimals; use math.neg instead."
       << "hex length is fixed. cannot parse shorter hex lengths."
       << ""
       << "cast modes:"
       << ""
       << "  idec → udec | [default]"
       << "  idec → uhex | requires UHEX hint"
       << ""
       << "  ihex → uhex | [default]"
       << "  ihex → udec | requires UDEC hint"
       << ""
       << "  udec → udec | [default]"
       << "  udec → uhex | requires UHEX hint"
       << ""
       << "  uhex → uhex | [default]"
       << "  uhex → udec | requires UDEC hint"
       << ""
       << "  utup → uhex | [default]"
       << "  utup → udec | requires UDEC hint"
       << ""
       << "preserves hex/decimal representation."
       << "specify hint to choose a cast mode."
       << ""
       << "uses a 'u' suffix for both representations."
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

  // ihex  → uhex | [default]
  tests << uint("0x" + utl::cat(samp::h7)) = ("0x" + utl::cat(samp::h7) + "u") >> docs;
  // ihex  → udec | requires UDEC hint
  tests << uint("0x" + utl::cat(samp::h15), "UDEC") = "15u" >> docs;

  // udec  → udec | [default]
  tests << uint("8u")         = "8u" >> docs;
  // udec  → uhex | requires UHEX hint
  tests << uint("6u", "UHEX") = ("0x" + utl::cat(samp::h6) + "u") >> docs;

  // uhex  → uhex | [default]
  tests << uint("0x" + utl::cat(samp::h5) + "u") =
      ("0x" + utl::cat(samp::h5) + "u") >> docs;
  // uhex  → udec | requires UDEC hint
  tests << uint("0x" + utl::cat(samp::h4) + "u", "UDEC") = "4u" >> docs;

  // utup → uhex | [default]
  tests << uint(pp::tup(samp::hmin)) = ("0x" + utl::cat(samp::hmin) + "u") >> docs;
  // utup → udec | requires UDEC hint
  tests << uint(pp::tup(samp::hmax), "UDEC") = uint_max_s >> docs;

  def<"mode(e, t, ...: <err>, <typeof v>, <hint>) -> <cast mode>"> mode =
      [&](arg e, arg t, va hint) {
        docs << "cast mode selector and error detector";

        def<"0\\IDEC"> _0idec = [&] { return ""; };
        def<"0\\IHEX">{}      = [&] { return ""; };
        def<"0\\UDEC">{}      = [&] { return ""; };
        def<"0\\UHEX">{}      = [&] { return ""; };
        def<"0\\UTUP">{}      = [&] { return ""; };
        def<"1\\UDEC"> _1udec = [&] { return ""; };
        def<"1\\UHEX">{}      = [&] { return ""; };
        def<"1">{}            = [&] { return ""; };

        def<"00(e, t, ...)"> _00 = [&](arg e, arg, va) { return fail(e); };
        def<"01(e, t, ...)">{}   = [&](arg e, arg, va) { return fail(e); };
        def<"10(e, t, ...)">{}   = [&](arg e, arg, va) { return fail(e); };
        def<"11(e, t, ...)">{}   = [&](arg, arg t, va hint) {
          def<"\\IDECUDEC"> idecidec = [&] { return "ID_UD"; };
          def<"\\IDECUHEX">{}        = [&] { return "ID_UH"; };
          def<"\\IDEC">{}            = [&] { return "ID_UD"; };
          def<"\\IHEXUDEC">{}        = [&] { return "IH_UD"; };
          def<"\\IHEXUHEX">{}        = [&] { return "IH_UH"; };
          def<"\\IHEX">{}            = [&] { return "IH_UH"; };
          def<"\\UDECUDEC">{}        = [&] { return "UD_UD"; };
          def<"\\UDECUHEX">{}        = [&] { return "UD_UH"; };
          def<"\\UDEC">{}            = [&] { return "UD_UD"; };
          def<"\\UHEXUDEC">{}        = [&] { return "UH_UD"; };
          def<"\\UHEXUHEX">{}        = [&] { return "UH_UH"; };
          def<"\\UHEX">{}            = [&] { return "UH_UH"; };
          def<"\\UTUPUDEC">{}        = [&] { return "XW_UD"; };
          def<"\\UTUPUHEX">{}        = [&] { return "XW_UH"; };
          def<"\\UTUP">{}            = [&] { return "XW_UH"; };

          return pp::cat(utl::slice(idecidec, -8), t, hint);
        };

        return pp::call(
            cat(utl::slice(_00, -2), cat(is_none(cat(utl::slice(_0idec, -4), t)),
                                         is_none(pp::cat(utl::slice(_1udec, -4), hint)))),
            e, t, hint);
      };

  auto hword_params = utl::alpha_base52_seq(conf::word_size);
  for (auto&& v : hword_params)
    if (v == "u" or v == "x") {
      v = "_" + v;
    }

  def<"\\ID_UD(idec)"> id_id = [&](arg idec) { return pp::cat(idec, 'u'); };
  def<"\\ID_UH(idec)">{}     = [&](arg idec) {
    return impl::udec(pp::cat(idec, 'u'), "UHEX");
  };
  def<"\\IH_UD(ihex)">{} = [&](arg ihex) {
    return impl::uhex(pp::cat(ihex, 'u'), "UDEC");
  };
  def<"\\IH_UH(ihex)">{}  = [&](arg ihex) { return pp::cat(ihex, 'u'); };
  def<"\\UD_UD(udec)">{}  = [&](arg udec) { return udec; };
  def<"\\UD_UH(udec)">{}  = [&](arg udec) { return impl::udec(udec, "UHEX"); };
  def<"\\UH_UD(uhex)">{}  = [&](arg uhex) { return impl::uhex(uhex, "UDEC"); };
  def<"\\UH_UH(uhex)">{}  = [&](arg uhex) { return uhex; };
  def<"\\XW_UD(hword)">{} = [&](arg hword) {
    def o = def{"o(" + utl::cat(hword_params, ", ") + ")"} = [&](pack args) {
      return impl::uhex(pp::cat("0x", pp::cat(args), "u"), "UDEC");
    };
    return o + " " + hword;
  };
  def<"\\XW_UH(hword)">{} = [&](arg hword) {
    def o = def{"o(" + utl::cat(hword_params, ", ") + ")"} = [&](pack args) {
      return pp::cat("0x", pp::cat(args), "u");
    };
    return o + " " + hword;
  };

  return def<"o(e, v, ...)">{[&](arg e, arg v, va hint) {
    return pp::call(cat(utl::slice(id_id, -5), mode(e, typeof(v), hint)), v);
  }}(va_str("[" + uint + "] invalid arguments : " + args), args);
});

} // namespace api

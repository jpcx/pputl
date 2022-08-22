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

decltype(int_) int_ = NIFTY_DEF(int_, [&](va args) {
  docs << "[inherits from " + atom + "] " + std::to_string(conf::word_size * 4)
              + "-bit signed integer type."
       << "constructible from any word."
       << "instance is either idec or ihex."
       << ""
       << "cannot parse negative decimals; use math.neg instead."
       << "hex length is fixed. cannot parse shorter hex lengths."
       << ""
       << "cast modes:"
       << ""
       << "  idec → idec | [default]"
       << "  idec → ihex | requires IHEX hint"
       << ""
       << "  ihex → ihex | [default]; fallback for failed ihex → idec"
       << "  ihex → idec | requires IDEC hint and positive result"
       << ""
       << "  udec → idec | [default]; requires positive result"
       << "  udec → ihex | requires IHEX hint or udec → idec failure"
       << ""
       << "  uhex → ihex | [default]; fallback for failed uhex → idec"
       << "  uhex → idec | requires IDEC hint and positive result"
       << ""
       << "  utup → ihex | [default]; fallback for failed utup → idec"
       << "  utup → idec | requires IDEC hint and positive result"
       << ""
       << "attempts to preserve hex/decimal representation by default, but"
       << "will output hex if casting the input yields a negative number."
       << "hint is ignored only if the result is negative and the hint is IDEC."
       << ""
       << "cast from unsigned reinterprets bits as signed two's complement."
       << ""
       << "values above the int max must have a 'u' suffix; implicit interpretation"
       << "as unsigned is not allowed (e.g. " + std::to_string(conf::uint_max)
              + " is not a valid integer).";

  // idec  → idec | [default]
  tests << int_(0)         = "0" >> docs;
  // idec  → ihex | requires IHEX hint
  tests << int_(1, "IHEX") = ("0x" + utl::cat(samp::h1)) >> docs;

  // ihex  → ihex | [default]; fallback for failed ihex → idec
  tests << int_("0x" + utl::cat(samp::h2)) = ("0x" + utl::cat(samp::h2)) >> docs;
  tests << int_("0x" + utl::cat(samp::himin), "IDEC") =
      ("0x" + utl::cat(samp::himin)) >> docs;
  // ihex  → idec | requires IDEC hint and positive result
  tests << int_("0x" + utl::cat(samp::h2), "IDEC") = "2" >> docs;

  // udec  → idec | [default]; requires positive result
  tests << int_("7u")          = "7" >> docs;
  // udec  → ihex | requires IHEX hint or udec → idec failure
  tests << int_("15u", "IHEX") = ("0x" + utl::cat(samp::h15)) >> docs;
  tests << int_(uint_max_s)    = ("0x" + utl::cat(samp::hmax)) >> docs;

  // uhex  → ihex | [default]; fallback for failed uhex → idec
  tests << int_("0x" + utl::cat(samp::h7) + "u") = ("0x" + utl::cat(samp::h7)) >> docs;
  tests << int_("0x" + utl::cat(samp::hmax) + "u", "IDEC") =
      ("0x" + utl::cat(samp::hmax)) >> docs;
  // uhex  → idec | requires IDEC hint and positive result
  tests << int_("0x" + utl::cat(samp::h5) + "u", "IDEC") = "5" >> docs;

  // utup → ihex | [default]; fallback for failed utup → idec
  tests << int_(pp::tup(samp::hmin))          = ("0x" + utl::cat(samp::hmin)) >> docs;
  tests << int_(pp::tup(samp::himin), "IDEC") = ("0x" + utl::cat(samp::himin)) >> docs;
  // utup → idec | requires IDEC hint and positive result
  tests << int_(pp::tup(samp::himax), "IDEC") = int_max_s >> docs;

  def<"mode(e, t, ...: <err>, <typeof v>, <hint>) -> <cast mode>"> mode =
      [&](arg e, arg t, va h) {
        docs << "cast mode selector and error detector";

        def<"0\\IDEC"> _0idec = [&] { return ""; };
        def<"0\\IHEX">{}      = [&] { return ""; };
        def<"0\\UDEC">{}      = [&] { return ""; };
        def<"0\\UHEX">{}      = [&] { return ""; };
        def<"0\\UTUP">{}      = [&] { return ""; };
        def<"1\\IDEC"> _1idec = [&] { return ""; };
        def<"1\\IHEX">{}      = [&] { return ""; };
        def<"1">{}            = [&] { return ""; };

        def<"\\00(e, t, ...)"> _00 = [&](arg e, arg, va) { return fail(e); };
        def<"\\01(e, t, ...)">{}   = [&](arg e, arg, va) { return fail(e); };
        def<"\\10(e, t, ...)">{}   = [&](arg e, arg, va) { return fail(e); };
        def<"\\11(e, t, ...)">{}   = [&](arg, arg t, va hint) {
          def<"\\IDECIDEC"> idecidec = [&] { return "ID_ID"; };
          def<"\\IDECIHEX">{}        = [&] { return "ID_IH"; };
          def<"\\IDEC">{}            = [&] { return "ID_ID"; };
          def<"\\IHEXIDEC">{}        = [&] { return "IH_IC"; };
          def<"\\IHEXIHEX">{}        = [&] { return "IH_IH"; };
          def<"\\IHEX">{}            = [&] { return "IH_IH"; };
          def<"\\UDECIDEC">{}        = [&] { return "UD_IC"; };
          def<"\\UDECIHEX">{}        = [&] { return "UD_IH"; };
          def<"\\UDEC">{}            = [&] { return "UD_IC"; };
          def<"\\UHEXIDEC">{}        = [&] { return "UH_IC"; };
          def<"\\UHEXIHEX">{}        = [&] { return "UH_IH"; };
          def<"\\UHEX">{}            = [&] { return "UH_IH"; };
          def<"\\UTUPIDEC">{}        = [&] { return "XW_IC"; };
          def<"\\UTUPIHEX">{}        = [&] { return "XW_IH"; };
          def<"\\UTUP">{}            = [&] { return "XW_IH"; };

          return pp::cat(utl::slice(idecidec, -8), t, hint);
        };

        return pp::call(
            xcat(utl::slice(_00, -2), xcat(is_none(xcat(utl::slice(_0idec, -4), t)),
                                           is_none(pp::cat(utl::slice(_1idec, -4), h)))),
            e, t, h);
      };

  auto utup_params = utl::alpha_base52_seq(conf::word_size);
  for (auto&& v : utup_params)
    if (v == "u" or v == "x") {
      v = "_" + v;
    }

  def<"\\ID_ID(idec)"> id_id = [&](arg idec) { return idec; };
  def<"\\ID_IH(idec)">{}     = [&](arg idec) {
    return impl::uhex(impl::udec(pp::cat(idec, 'u'), "UHEX"), "IHEX");
  };
  def<"\\IH_IC(ihex)">{} = [&](arg ihex) {
    return impl::uhex(pp::cat(ihex, 'u'), "ICAST");
  };
  def<"\\IH_IH(ihex)">{} = [&](arg ihex) { return ihex; };
  def<"\\UD_IC(udec)">{} = [&](arg udec) {
    return impl::uhex(impl::udec(udec, "UHEX"), "ICAST");
  };
  def<"\\UD_IH(udec)">{} = [&](arg udec) {
    return impl::uhex(impl::udec(udec, "UHEX"), "IHEX");
  };
  def<"\\UH_IC(uhex)">{} = [&](arg uhex) { return impl::uhex(uhex, "ICAST"); };
  def<"\\UH_IH(uhex)">{} = [&](arg uhex) { return impl::uhex(uhex, "IHEX"); };
  def<"\\XW_IC(utup)">{} = [&](arg utup) {
    def o = def{"o(" + utl::cat(utup_params, ", ") + ")"} = [&](pack args) {
      return impl::uhex(pp::cat("0x", pp::cat(args), "u"), "ICAST");
    };
    return o + " " + utup;
  };
  def<"\\XW_IH(utup)">{} = [&](arg utup) {
    def o = def{"o(" + utl::cat(utup_params, ", ") + ")"} = [&](pack args) {
      return impl::uhex(pp::cat("0x", pp::cat(args), "u"), "IHEX");
    };
    return o + " " + utup;
  };

  return def<"o(e, v, ...)">{[&](arg e, arg v, va hint) {
    return pp::call(xcat(utl::slice(id_id, -5), mode(e, typeof(v), hint)), v);
  }}(str("[" + int_ + "] invalid arguments : " + args), args);
});

} // namespace api

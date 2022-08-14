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
       << ""
       << "cannot parse negative decimals; use math.neg instead."
       << "hex length is fixed. cannot parse shorter hex lengths."
       << ""
       << "cast modes:"
       << " - idec  → idec | default"
       << " - idec  → ihex | requires IHEX pref"
       << " - ihex  → idec | requires IDEC pref and positive result"
       << " - ihex  → ihex | default; fallback for ihex  → idec"
       << " - udec  → idec | requires positive result"
       << " - udec  → ihex | requires IHEX pref"
       << " - uhex  → idec | requires IDEC pref and positive result"
       << " - uhex  → idec | requires IDEC pref and positive result"
       << " - uhex  → ihex | default; fallback for uhex  → idec"
       << " - hword → idec | requires IDEC pref and positive result"
       << " - hword → ihex | default; fallback for hword → idec"
       << ""
       << "attempts to preserve hex/decimal representation by default, but"
       << "will output hex if casting the input yields a negative number"
       << ""
       << "hint is ignored only if the result is negative and the hint is IDEC."
       << ""
       << "cast from unsigned reinterprets bits as signed two's complement."
       << "value must be a valid signed int; implicit interpretation"
       << "as unsigned is not allowed (e.g. " + std::to_string(conf::uint_max)
              + " is not a valid integer).";

  auto min = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "0"));
  auto max = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "F"));

  tests << int_(0)             = "0" >> docs;
  tests << int_("1u")          = "1" >> docs;
  tests << int_(min)           = min >> docs;
  tests << int_(conf::int_max) = std::to_string(conf::int_max) >> docs;
  tests << int_(max + "u")     = max >> docs;
  tests << int_(uint_max_s)    = max >> docs;
  tests << int_(pp::tup((conf::word_size > 1 ? "8, " : "8")
                        + utl::cat(std::vector<std::string>(conf::word_size - 1, "0"), ", "))) =
      int_min_s >> docs;
  tests << int_(pp::tup((conf::word_size > 1 ? "7, " : "7")
                        + utl::cat(std::vector<std::string>(conf::word_size - 1, "F"), ", "))) =
      ("0x7" + utl::cat(std::vector<std::string>(conf::word_size - 1, "F"))) >> docs;

  def<"mode(...e, t, ...: <err>, <typeof v>, <hint>) -> <cast mode>"> mode = [&](arg e, arg t,
                                                                                 va h) {
    def<"0\\IDEC"> _0idec = [&] { return ""; };
    def<"0\\IHEX">{}      = [&] { return ""; };
    def<"0\\UDEC">{}      = [&] { return ""; };
    def<"0\\UHEX">{}      = [&] { return ""; };
    def<"0\\HWORD">{}     = [&] { return ""; };
    def<"1\\IDEC"> _1idec = [&] { return ""; };
    def<"1\\IHEX">{}      = [&] { return ""; };
    def<"1">{}            = [&] { return ""; };

    def<"00(e, t, ...)"> _00 = [&](arg e, arg, va) { return fail(e); };
    def<"01(e, t, ...)">{}   = [&](arg e, arg, va) { return fail(e); };
    def<"10(e, t, ...)">{}   = [&](arg e, arg, va) { return fail(e); };
    def<"11(e, t, ...)">{}   = [&](arg, arg t, va hint) {
      def<"\\IDECIDEC"> idecidec = [&] { return "ID_ID"; };
      def<"\\IDECIHEX">{}        = [&] { return "ID_IH"; };
      def<"\\IDEC">{}            = [&] { return "ID_ID"; };
      def<"\\IHEXIDEC">{}        = [&] { return "IH_ID"; };
      def<"\\IHEXIHEX">{}        = [&] { return "IH_IH"; };
      def<"\\IHEX">{}            = [&] { return "IH_IH"; };
      def<"\\UDECIDEC">{}        = [&] { return "UD_ID"; };
      def<"\\UDECIHEX">{}        = [&] { return "UD_IH"; };
      def<"\\UDEC">{}            = [&] { return "UD_ID"; };
      def<"\\UHEXIDEC">{}        = [&] { return "UH_ID"; };
      def<"\\UHEXUHEX">{}        = [&] { return "UH_IH"; };
      def<"\\UHEX">{}            = [&] { return "UH_IH"; };
      def<"\\HWORDIDEC">{}       = [&] { return "HW_ID"; };
      def<"\\HWORDIHEX">{}       = [&] { return "HW_IH"; };
      def<"\\HWORD">{}           = [&] { return "HW_IH"; };

      return pp::cat(utl::slice(idecidec, -8), t, hint);
    };

    return pp::call(cat(utl::slice(_00, -2), cat(is_none(cat(utl::slice(_0idec, -4), t)),
                                                 is_none(cat(utl::slice(_1idec, -4), h)))),
                    e, t, h);
  };

  auto hword_params = utl::alpha_base52_seq(conf::word_size);
  for (auto&& v : hword_params)
    if (v == "u" or v == "x") {
      v = "_" + v;
    }

  def<"\\ID_ID(idec)"> idecidec = [&](arg idec) { return idec; };
  def<"\\ID_IH(idec)">{}  = [&](arg idec) { return impl::uint_trait(pp::cat(idec, 'u'), "DIHEX"); };
  def<"\\IH_ID(ihex)">{}  = [&](arg ihex) { return impl::uint_trait(pp::cat(ihex, 'u'), "HIDEC"); };
  def<"\\IH_IH(ihex)">{}  = [&](arg ihex) { return ihex; };
  def<"\\UD_ID(udec)">{}  = [&](arg udec) { return impl::uint_trait(udec, "DITRY"); };
  def<"\\UD_IH(udec)">{}  = [&](arg udec) { return impl::uint_trait(udec, "DIHEX"); };
  def<"\\UH_ID(uhex)">{}  = [&](arg uhex) { return impl::uint_trait(uhex, "HITRY"); };
  def<"\\UH_IH(uhex)">{}  = [&](arg uhex) { return impl::uint_trait(uhex, "HIHEX"); };
  def<"\\HW_ID(hword)">{} = [&](arg hword) {
    def res = def{"res(" + utl::cat(hword_params, ", ") + ")"} = [&](pack args) {
      return impl::uint_trait("0x" + pp::cat(args) + "u", "HITRY");
    };
    return res + " " + hword;
  };
  def<"\\HW_IH(hword)">{} = [&](arg hword) {
    def res = def{"res(" + utl::cat(hword_params, ", ") + ")"} = [&](pack args) {
      return impl::uint_trait("0x" + pp::cat(args) + "u", "HIHEX");
    };
    return res + " " + hword;
  };

  return def<"o(e, v, ...)">{[&](arg e, arg v, va hint) {
    return pp::call(cat(utl::slice(idecidec, -8), mode(e, typeof(v), hint)), v);
  }}(istr("[" + int_ + "] invalid arguments : " + args), args);
});

} // namespace api

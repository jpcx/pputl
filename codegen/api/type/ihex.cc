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

decltype(ihex) ihex = NIFTY_DEF(ihex, [&](va args) {
  docs << "[" + enum_ + "<0x" + utl::cat(samp::hmin) + "|" + "0x" + utl::cat(samp::h1)
              + "|...|" + "0x" + utl::cat(svect{conf::word_size - 1, "F"}) + "E|" + "0x"
              + utl::cat(samp::hmax) + ">] a hexadecimal integer."
       << "constructible from any word within [int_min, int_max].";

  auto zero = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "0"));
  auto one  = "0x" + utl::cat(std::vector<std::string>(conf::word_size - 1, "0")) + "1";
  auto max  = "0x7" + utl::cat(std::vector<std::string>(conf::word_size - 1, "F"));
  auto umax = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "F"));
  auto five = "0x" + utl::cat(std::vector<std::string>(conf::word_size - 1, "0")) + "5";

  tests << ihex(0)                                   = zero >> docs;
  tests << ihex(1)                                   = one >> docs;
  tests << ihex(5u)                                  = five >> docs;
  tests << ihex(uint_max_s)                          = umax >> docs;
  tests << ihex(std::to_string(conf::int_max) + "u") = max >> docs;

  def<"x(...)"> x = [&](va args) {
    return args;
  };

  auto utparams = utl::alpha_base52_seq(conf::word_size);
  def  ut_hex = def{"ut_hex(" + utl::cat(utparams, ", ") + ")"} = [&](pack args) {
    return pp::cat("0x", pp::cat(args));
  };

  def<"\\0(e, obj)"> _0 = [&](arg e, arg obj) {
    def<"\\0(e, obj)"> _0 = [&](arg e, arg) {
      return fail(e);
    };
    def<"\\1(e, atom)">{} = [&](arg e, arg atom) {
      def<"\\0000(e, atom)"> _0000 = [&](arg e, arg) {
        return fail(e);
      };
      def<"\\0001(e, uhex)">{} = [&](arg, arg uhex) {
        return impl::uhex(uhex, "IHEX");
      };
      def<"\\0010(e, udec)">{} = [&](arg, arg udec) {
        return impl::uhex(impl::udec(udec, "UHEX"), "IHEX");
      };
      def<"\\0100(e, ihex)">{} = [&](arg, arg ihex) {
        return ihex;
      };
      def<"\\1000(e, idec)">{} = [&](arg, arg idec) {
        return impl::uhex(impl::udec(pp::cat(idec, 'u'), "UHEX"), "IHEX");
      };

      return pp::call(xcat(utl::slice(_0000, -4),
                           xcat(xcat(detail::is_idec_o(atom), detail::is_ihex_o(atom)),
                                xcat(detail::is_enum_oo(impl::udec_prefix, atom),
                                     detail::is_enum_oo(impl::uhex_prefix, atom)))),
                      e, atom);
    };

    return pp::call(xcat(utl::slice(_0, -1), detail::is_atom_o(obj)), e, obj);
  };

  def<"\\1(e, tup)">{} = [&](arg e, arg tup) {
    def<"\\0(e, tup)"> _0 = [&](arg e, arg) {
      return fail(e);
    };
    def<"\\1(e, utup)">{} = [&](arg, arg utup) {
      return x(ut_hex + " " + utup);
    };

    return pp::call(xcat(utl::slice(_0, -1), detail::is_utup_o(tup)), e, tup);
  };

  return def<"o(e, obj)">{[&](arg e, arg obj) {
    return pp::call(xcat(utl::slice(_0, -1), detail::is_tup_o(obj)), e, obj);
  }}(error(ihex, "invalid word", args), obj(args));
});

} // namespace api

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

decltype(idec) idec = NIFTY_DEF(idec, [&](va args) {
  docs << "[" + enum_ + "<0|1|...|" + std::to_string(conf::int_max - 1) + "|"
              + std::to_string(conf::int_max) + ">] a positive decimal integer."
       << "constructible from any word within [0, int_max]."
       << ""
       << "note: negative decimals may be pasted using fmt.c_int.";

  auto zero = "0x" + utl::cat(samp::hmin);
  auto one  = "0x" + utl::cat(samp::h1);
  auto max  = "0x" + utl::cat(samp::himax);
  auto five = "0x" + utl::cat(samp::h5);

  tests << idec(zero)          = "0" >> docs;
  tests << idec(one)           = "1" >> docs;
  tests << idec(five + "u")    = "5" >> docs;
  tests << idec(max)           = int_max_s >> docs;
  tests << idec(conf::int_max) = int_max_s >> docs;

  def<"x(...)"> x = [&](va args) {
    return args;
  };

  auto utparams = utl::alpha_base52_seq(conf::word_size);
  for (auto&& v : utparams)
    if (v == "u") {
      v = "_" + v;
      break;
    }

  def ut_hex = def{"ut_hex(" + utl::cat(utparams, ", ") + ")"} = [&](pack args) {
    return pp::cat("0x", pp::cat(args), "u");
  };

  def<"\\0(e0, e1, obj)"> _0 = [&](arg e0, arg e1, arg obj) {
    def<"\\0(e0, e1, obj)"> _0 = [&](arg e0, arg, arg) {
      return fail(e0);
    };
    def<"\\1(e0, e1, atom)">{} = [&](arg e0, arg e1, arg atom) {
      def<"\\0000(e0, e1, atom)"> _0000 = [&](arg e0, arg, arg) {
        return fail(e0);
      };
      def<"\\0001(e0, e1, uhex)"> _0001 = [&](arg, arg e1, arg uhex) {
        def<"\\0(e1, uhex)"> _0 = [&](arg, arg uhex) {
          return impl::uhex(uhex, "ICAST");
        };
        def<"\\1(e1, uhex)">{} = [&](arg e1, arg) {
          return fail(e1);
        };

        return pp::call(xcat(utl::slice(_0, -1), impl::uhex(uhex, "ILTZ")), e1, uhex);
      };
      def<"\\0010(e0, e1, udec)">{} = [&](arg e0, arg e1, arg udec) {
        return _0001(e0, e1, impl::udec(udec, "UHEX"));
      };
      def<"\\0100(e0, e1, ihex)">{} = [&](arg e0, arg e1, arg ihex) {
        return _0001(e0, e1, pp::cat(ihex, 'u'));
      };
      def<"\\1000(e0, e1, idec)">{} = [&](arg, arg, arg idec) {
        return idec;
      };

      return pp::call(xcat(utl::slice(_0000, -4),
                           xcat(xcat(detail::is_idec_o(atom), detail::is_ihex_o(atom)),
                                xcat(detail::is_enum_oo(impl::udec_prefix, atom),
                                     detail::is_enum_oo(impl::uhex_prefix, atom)))),
                      e0, e1, atom);
    };

    return pp::call(xcat(utl::slice(_0, -1), detail::is_atom_o(obj)), e0, e1, obj);
  };

  def<"\\1(e0, e1, tup)">{} = [&](arg e0, arg e1, arg tup) {
    def<"\\0(e0, e1, tup)"> _0 = [&](arg e0, arg, arg) {
      return fail(e0);
    };
    def<"\\1(e0, e1, utup)">{} = [&](arg, arg e1, arg utup) {
      def<"res(e1, uhex)"> res = [&](arg e1, arg uhex) {
        def<"\\0(e1, uhex)"> _0 = [&](arg, arg uhex) {
          return impl::uhex(uhex, "ICAST");
        };
        def<"\\1(e1, uhex)">{} = [&](arg e1, arg) {
          return fail(e1);
        };
        return pp::call(xcat(utl::slice(_0, -1), impl::uhex(uhex, "ILTZ")), e1, uhex);
      };

      return res(e1, x(ut_hex + " " + utup));
    };

    return pp::call(xcat(utl::slice(_0, -1), detail::is_utup_o(tup)), e0, e1, tup);
  };

  return def<"o(e0, e1, obj)">{[&](arg e0, arg e1, arg obj) {
    return pp::call(xcat(utl::slice(_0, -1), detail::is_tup_o(obj)), e0, e1, obj);
  }}(error(idec, "invalid word", args),
     error(idec, "idec cannot represent negative words", args), obj(args));
});

} // namespace api

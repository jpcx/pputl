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
  docs << "[extends atom] atoms 0|1|...|" + std::to_string(conf::int_max - 1) + "|"
              + std::to_string(conf::int_max) + ".";

  auto zero = "0x" + utl::cat(samp::hmin);
  auto one  = "0x" + utl::cat(samp::h1);
  auto max  = "0x" + utl::cat(samp::himax);
  auto five = "0x" + utl::cat(samp::h5);

  tests << idec(zero)          = "0" >> docs;
  tests << idec(one)           = "1" >> docs;
  tests << idec(five + "u")    = "5" >> docs;
  tests << idec(max)           = int_max_s >> docs;
  tests << idec(conf::int_max) = int_max_s >> docs;

  return def<"o(e0, e1, atom)">{[&](arg e0, arg e1, arg atom) {
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

    return pp::call(
        xcat(utl::slice(_0000, -4), xcat(xcat(impl::is_idec(atom), impl::is_ihex(atom)),
                                         xcat(impl::is_udec(atom), impl::is_uhex(atom)))),
        e0, e1, atom);
  }}(error(idec, "invalid word", args),
     error(idec, "idec cannot represent negative words", args), atom(default_(0, args)));
});

} // namespace api

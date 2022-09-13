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

decltype(uhex) uhex = NIFTY_DEF(uhex, [&](va args) {
  docs << "[" + enum_ + "<" + "0x" + utl::cat(samp::hmin) + "u|" + "0x"
              + utl::cat(samp::h1) + "u|...|" + "0x"
              + utl::cat(svect{conf::word_size - 1, "F"}) + "Eu|" + "0x"
              + utl::cat(samp::hmax) + "u>] an unsigned hexadecimal integer.";

  auto min  = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "0")) + "u";
  auto one  = "0x" + utl::cat(std::vector<std::string>(conf::word_size - 1, "0")) + "1u";
  auto max  = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "F")) + "u";
  auto imax = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "F"));
  auto five = "0x" + utl::cat(std::vector<std::string>(conf::word_size - 1, "0")) + "5u";

  tests << uhex(0)          = min >> docs;
  tests << uhex(1u)         = one >> docs;
  tests << uhex(5)          = five >> docs;
  tests << uhex(uint_max_s) = max >> docs;
  tests << uhex(min)        = min >> docs;
  tests << uhex(one)        = one >> docs;
  tests << uhex(imax)       = max >> docs;

  return def<"o(e, atom)">{[&](arg e, arg atom) {
    def<"\\0000(e, atom)"> _0000 = [&](arg e, arg) {
      return fail(e);
    };
    def<"\\0001(e, uhex)">{} = [&](arg, arg uhex) {
      return uhex;
    };
    def<"\\0010(e, udec)">{} = [&](arg, arg udec) {
      return impl::udec(udec, "UHEX");
    };
    def<"\\0100(e, ihex)">{} = [&](arg, arg ihex) {
      return pp::cat(ihex, 'u');
    };
    def<"\\1000(e, idec)">{} = [&](arg, arg idec) {
      return impl::udec(pp::cat(idec, 'u'), "UHEX");
    };

    return pp::call(xcat(utl::slice(_0000, -4),
                         xcat(xcat(detail::is_idec_o(atom), detail::is_ihex_o(atom)),
                              xcat(detail::is_enum_oo(impl::udec_prefix, atom),
                                   detail::is_enum_oo(impl::uhex_prefix, atom)))),
                    e, atom);
  }}(error(uhex, "invalid word", args), atom(args));
});

} // namespace api

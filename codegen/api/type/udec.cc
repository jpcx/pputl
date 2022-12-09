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

// #include "type.h"
// 
// namespace api {
// 
// using namespace codegen;
// 
// decltype(udec) udec = NIFTY_DEF(udec, [&](va args) {
//   docs << "[extends enum<0u|1u|...|" + std::to_string(conf::uint_max - 1) + "u|"
//               + std::to_string(conf::uint_max) + "u>] an unsigned decimal integer.";
// 
//   auto min  = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "0")) + "u";
//   auto one  = "0x" + utl::cat(std::vector<std::string>(conf::word_size - 1, "0")) + "1u";
//   auto max  = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "F")) + "u";
//   auto in1  = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "F"));
//   auto five = "0x" + utl::cat(std::vector<std::string>(conf::word_size - 1, "0")) + "5u";
// 
//   tests << udec()     = "0u" >> docs;
//   tests << udec(min)  = "0u" >> docs;
//   tests << udec(1)    = "1u" >> docs;
//   tests << udec(5u)   = "5u" >> docs;
//   tests << udec(five) = "5u" >> docs;
//   tests << udec(max)  = uint_max_s >> docs;
//   tests << udec(in1)  = uint_max_s >> docs;
// 
//   return def<"o(e, atom)">{[&](arg e, arg atom) {
//     def<"\\0000(e, atom)"> _0000 = [&](arg e, arg) {
//       return fail(e);
//     };
//     def<"\\0001(e, uhex)">{} = [&](arg, arg uhex) {
//       return impl::uhex(uhex, "UDEC");
//     };
//     def<"\\0010(e, udec)">{} = [&](arg, arg udec) {
//       return udec;
//     };
//     def<"\\0100(e, ihex)">{} = [&](arg, arg ihex) {
//       return impl::uhex(pp::cat(ihex, 'u'), "UDEC");
//     };
//     def<"\\1000(e, idec)">{} = [&](arg, arg idec) {
//       return pp::cat(idec, 'u');
//     };
// 
//     return pp::call(xcat(utl::slice(_0000, -4),
//                          xcat(xcat(detail::is_idec_o(atom), detail::is_ihex_o(atom)),
//                               xcat(detail::is_enum_oo(impl::udec_prefix, atom),
//                                    detail::is_enum_oo(impl::uhex_prefix, atom)))),
//                     e, atom);
//   }}(error(udec, "invalid word", args), atom(default_(0, args)));
// });
// 
// } // namespace api

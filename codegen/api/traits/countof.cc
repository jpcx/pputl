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

// #include "traits.h"
// 
// namespace api {
// 
// using namespace codegen;
// 
// decltype(countof) countof = NIFTY_DEF(countof, [&](va args) {
//   docs << "counts the number of arguments."
//        << "fails if larger than " + size_max + " (" + size_max_s + ")";
// 
//   tests << countof()          = "0u" >> docs;
//   tests << countof("a")       = "1u" >> docs;
//   tests << countof("a, b")    = "2u" >> docs;
//   tests << countof("a, b, c") = "3u";
//   tests << countof(utl::cat(std::array<std::string, conf::size_max>{}, ",")) = size_max_s;
//   tests << countof(", ")    = "2u" >> docs;
//   tests << countof(", , ")  = "3u";
//   tests << countof("a, ")   = "2u";
//   tests << countof("a, , ") = "3u";
//   tests << countof(", a")   = "2u";
//   tests << countof(", a, ") = "3u";
//   tests << countof(", , a") = "3u";
// 
//   if constexpr (conf::cpp20_arglimit) { // slower implementation but ensures
//                                         // that no more than size_max args are passed
//     std::array<def<>, conf::size_max> n{};
//     n[0] =
//         def{"n" + std::to_string(conf::size_max) + "(e, _, ...)"} = [&](arg e, arg, va) {
//           return pp::va_opt(fail(e) + ", ") + " " + size_max_s;
//         };
// 
//     for (std::size_t i = 1; i < n.size(); ++i) {
//       n[i] = def{"n" + std::to_string(conf::size_max - i)
//                  + "(e, _, ...)"} = [&](arg e, arg, va args) {
//         return pp::va_opt(n[i - 1](e, args) + ", ") + " "
//              + std::to_string(conf::size_max - i) + "u";
//       };
//     }
// 
//     return def<"o(e, ...)">{[&](arg e, va args) {
//       return xfirst(pp::va_opt(n.back()(e, args + ".") + ", ") + " 0u");
//     }}(str(pp::str("[" + countof + "] too many arguments") + " : " + args), args);
//   } else {
//     def read = def{"read(" + utl::cat(utl::alpha_base52_seq(conf::size_max), ", ")
//                    + ", _sz, ...)"};
// 
//     std::string prefix = utl::slice(read, -4);
//     if (prefix.back() == '_')
//       prefix.pop_back();
// 
//     def verifier = def{prefix + "(...)"} = [&] {
//       return "";
//     };
// 
//     read = [&](pack args) {
//       def res = def{prefix + "(...)"} = [&](va args) {
//         return args;
//       };
// 
//       def<"\\0(_sz)"> _0 = [&](arg) {
//         return "FAIL";
//       };
//       def<"\\1(_sz)">{} = [&](arg sz) {
//         return pp::cat(utl::slice(res, -prefix.size()), sz);
//       };
// 
//       auto sz = args[args.size() - 2];
//       return pp::call(cat(utl::slice(_0, -1),
//                           is_none(pp::cat(utl::slice(verifier, -prefix.size()), sz))),
//                       sz);
//     };
// 
//     def<"res(e, ...)"> res = [&](arg e, va args) {
//       def<"\\FAIL"> fail_ = [&] {
//         return "";
//       };
//       def<"\\0(e, sz)"> _0 = [&](arg, arg sz) {
//         return sz;
//       };
//       def<"\\1(e, ...)">{} = [&](arg e, va) {
//         return fail(e);
//       };
//       return pp::call(
//           cat(utl::slice(_0, -1), is_none(pp::cat(utl::slice(fail_, -4), args))), e,
//           args);
//     };
// 
//     return def<"o(e, ...)">{[&](arg e, va args) {
//       return res(e, def<"<o(_, ...)">{[&](arg op, va args) {
//                    auto rseq = utl::base10_seq(conf::size_max + 1);
//                    for (auto&& v : rseq)
//                      v = v + "u";
//                    std::ranges::reverse(rseq);
//                    for (auto&& v : rseq)
//                      v = pp::call(op, v);
//                    return read(args + " " + pp::va_opt(", ") + " "
//                                + utl::cat(rseq, ", "));
//                  }}(prefix, args));
//     }}(str(pp::str("[" + countof + "] too many arguments") + " : " + args), args);
//   }
// });
// 
// } // namespace api

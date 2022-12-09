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
// decltype(typeof) typeof = NIFTY_DEF(typeof, [&](va args) {
//   docs << "determines the most appropriate value type."
//        << ""
//        << "results can be used with constructors that support type hints;"
//        << "see the math and bitwise implementations for examples."
//        << ""
//        << "warning: must be one of the defined value types. inputs"
//        << "         such as `-`, `\"`, etc., are not detectable and"
//        << "         will trigger an unexpected concatenation error."
//        << ""
//        << "returns one of:"
//        << ""
//        << " | NONE | LIST   | TUPLE | ARRAY | ORDER"
//        << " | MAP  | PQUEUE | SET   | STACK | QUEUE"
//        << " | ATOM | IDEC   | IHEX  | UDEC  | UHEX";
// 
//   auto ihexneg1 = "0x" + utl::cat(std::vector<std::string>(conf::word_size, "F"));
//   auto ubinmax  = ihexneg1 + "u";
// 
//   tests << typeof("(foo)")            = "TUPLE" >> docs;
//   tests << typeof(0)                  = "IDEC" >> docs;
//   tests << typeof("0u")               = "UDEC" >> docs;
//   tests << typeof(conf::uint_max)     = "ATOM" >> docs;
//   tests << typeof(uint_max_s)         = "UDEC" >> docs;
//   tests << typeof(ihexneg1)           = "IHEX" >> docs;
//   tests << typeof(ubinmax)            = "UHEX" >> docs;
//   tests << typeof("foo")              = "ATOM" >> docs;
//   tests << typeof("001")              = "ATOM" >> docs;
//   tests << typeof("foo, bar")         = "LIST" >> docs;
//   tests << typeof()                   = "NONE" >> docs;
//   tests << typeof(fwd::array + "()")  = "ARRAY" >> docs;
//   tests << typeof(fwd::order + "()")  = "ORDER" >> docs;
//   tests << typeof(fwd::map + "()")    = "MAP" >> docs;
//   tests << typeof(fwd::pqueue + "()") = "PQUEUE" >> docs;
//   tests << typeof(fwd::set + "()")    = "SET" >> docs;
//   tests << typeof(fwd::stack + "()")  = "STACK" >> docs;
//   tests << typeof(fwd::queue + "()")  = "QUEUE" >> docs;
// 
//   def<"\\0(...)"> _0 = [&](va args) {
//     docs << "^!none";
// 
//     def<"\\0(...)"> _0 = [&](va) {
//       docs << "^!none → !obj";
//       return "LIST";
//     };
// 
//     def<"\\1(obj)">{} = [&](arg obj) {
//       docs << "^!none → obj";
// 
//       def<"\\0(atom)"> _0 = [&](arg atom) {
//         docs << "^!none → obj → !tuple";
// 
//         def<"\\0000000(atom)"> _0000000 = [&](arg atom) {
//           docs << "^!none → obj → !tuple → !(array|map|set|stack|queue|pqueue)";
// 
//           def<"\\0000"> _0000 = [&] {
//             return "ATOM";
//           };
//           def<"\\0001">{} = [&] {
//             return "UHEX";
//           };
//           def<"\\0010">{} = [&] {
//             return "UDEC";
//           };
//           def<"\\0100">{} = [&] {
//             return "IHEX";
//           };
//           def<"\\1000">{} = [&] {
//             return "IDEC";
//           };
// 
//           return xcat(utl::slice(_0000, -4),
//                       xcat(xcat(detail::is_idec_o(atom), detail::is_ihex_o(atom)),
//                            xcat(detail::is_udec_o(atom), detail::is_uhex_o(atom))));
//         };
// 
//         def<"\\0000001(pqueue)">{} = [&](arg) {
//           return "PQUEUE";
//         };
// 
//         def<"\\0000010(queue)">{} = [&](arg) {
//           return "QUEUE";
//         };
// 
//         def<"\\0000100(stack)">{} = [&](arg) {
//           return "STACK";
//         };
// 
//         def<"\\0001000(set)">{} = [&](arg) {
//           return "SET";
//         };
// 
//         def<"\\0010000(map)">{} = [&](arg) {
//           return "MAP";
//         };
// 
//         def<"\\0100000(order)">{} = [&](arg) {
//           return "ORDER";
//         };
// 
//         def<"\\1000000(array)">{} = [&](arg) {
//           return "ARRAY";
//         };
// 
//         return pp::call(
//             xcat(xcat(xcat(utl::slice(_0000000, -7), detail::is_array_o(atom)),
//                       xcat(detail::is_order_o(atom), detail::is_map_o(atom))),
//                  xcat(xcat(detail::is_set_o(atom), detail::is_stack_o(atom)),
//                       xcat(detail::is_queue_o(atom), detail::is_pqueue_o(atom)))),
//             atom);
//       };
// 
//       def<"\\1(tuple)">{} = [&](arg) {
//         docs << "^!none → obj → tuple";
// 
//         return "TUPLE";
//       };
// 
//       return pp::call(xcat(utl::slice(_0, -1), detail::is_tuple_o(obj)), obj);
//     };
// 
//     return pp::call(xcat(utl::slice(_0, -1), is_object(args)), args);
//   };
// 
//   def<"\\1(...)">{} = [&](va) {
//     docs << "^none";
//     return "NONE";
//   };
// 
//   return pp::call(xcat(utl::slice(_0, -1), is_none(args)), args);
// });
// 
// } // namespace api

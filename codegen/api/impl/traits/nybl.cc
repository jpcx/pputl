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

#include "impl/traits.h"

namespace api {
namespace impl {

using namespace codegen;

decltype(nybl) nybl = NIFTY_DEF(nybl, [&](arg v, arg t) {
  docs << "[internal] nybl traits";

  std::array<def<>, 16> nybls{
      def{"0000"} = [&] { return "0, " + detail::bits(0); },
      def{"0001"} = [&] { return "1, " + detail::bits(1); },
      def{"0010"} = [&] { return "2, " + detail::bits(2); },
      def{"0011"} = [&] { return "3, " + detail::bits(3); },
      def{"0100"} = [&] { return "4, " + detail::bits(4); },
      def{"0101"} = [&] { return "5, " + detail::bits(5); },
      def{"0110"} = [&] { return "6, " + detail::bits(6); },
      def{"0111"} = [&] { return "7, " + detail::bits(7); },
      def{"1000"} = [&] { return "8, " + detail::bits(8); },
      def{"1001"} = [&] { return "9, " + detail::bits(9); },
      def{"1010"} = [&] { return "A, " + detail::bits(10); },
      def{"1011"} = [&] { return "B, " + detail::bits(11); },
      def{"1100"} = [&] { return "C, " + detail::bits(12); },
      def{"1101"} = [&] { return "D, " + detail::bits(13); },
      def{"1110"} = [&] { return "E, " + detail::bits(14); },
      def{"1111"} = [&] { return "F, " + detail::bits(15); },
  };

  def<"\\IS(_, ...) -> bool"> is = [&](arg, va) {
    def<"\\0"> _0 = [&] { return "0"; };
    def<"\\01">{} = [&] { return "1"; };
    return pp::cat(_0, pp::va_opt("1"));
  };
  def<"\\HEX(hex, ...) -> hex">{}      = [&](pack args) { return args[0]; };
  def<"\\BITS(hex, ...) -> ...bool">{} = [&](pack args) { return args[1]; };

  return def<"o(t, ...)">{[&](arg t, va row) {
    return pp::call(pp::cat(utl::slice(is, -2), t), row);
  }}(t, xcat(utl::slice(nybls[0], -4), v));
});

} // namespace impl
} // namespace api

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
//         This program is free software: you can redistribute it and/or modify
//         it under the terms of the GNU General Public License as published by
//         the Free Software Foundation, either version 3 of the License, or
//         (at your option) any later version.
//
//         This program is distributed in the hope that it will be useful,
//         but WITHOUT ANY WARRANTY; without even the implied warranty of
//         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//         GNU General Public License for more details.
//                                                                            //
//  You should have received a copy of the GNU General Public License        ///
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.  ////
///////////////////////////////////////////////////////////////////////////// */

#include "util.h"

namespace api {

using namespace codegen;

decltype(bits) bits = NIFTY_DEF(bits, [&](va args) {
  docs << "extracts bits from hex or nybl values."
       << "returns exactly 4 bools.";

  tests << bits("0")    = "0, 0, 0, 0" >> docs;
  tests << bits("A")    = "1, 0, 1, 0" >> docs;
  tests << bits("1100") = "1, 1, 0, 0" >> docs;
  tests << bits("0010") = "0, 0, 1, 0" >> docs;

  def<"00(e, ...)"> _00 = [&](arg e, va) { return fail(e); };
  def<"01(e, nybl)">{}  = [&](arg, arg nybl) { return impl::nybl(nybl, "BITS"); };
  def<"10(e, hex)">{}   = [&](arg, arg hex) { return impl::hex(hex, "BITS"); };

  return pp::call(cat(utl::slice(_00, -2), cat(is_hex(args), is_nybl(args))),
                  icstr("[" + bits + "] invalid arguments : " + args), args);
});

} // namespace api

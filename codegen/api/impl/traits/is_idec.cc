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

decltype(is_idec) is_idec = NIFTY_DEF(is_idec, [&](arg atom) {
  docs << "[internal] detects if atom is an idec.";

  return def<"o(atom)">{[&](arg atom) {
    return def<"<o(...)">{[&](va args) {
      return def<"<o(atom, _, ...)">{[&](arg atom, arg, va) {
        def<"\\0(atom)"> _0 = [](arg) {
          return "0";
        };
        def<"\\01(atom)">{} = [](arg atom) {
          def<"\\0"> _0 = [] {
            return "1";
          };
          def<"\\1">{} = [] {
            return "0";
          };
          return xcat(utl::slice(_0, -1), uhex(udec(atom, "UHEX"), "ILTZ"));
        };

        return pp::call(pp::cat(_0, pp::va_opt("1")), atom);
      }}(args);
    }}(atom, pp::cat(udec_prefix, atom));
  }}(pp::cat(atom, 'u'));
});

} // namespace impl
} // namespace api
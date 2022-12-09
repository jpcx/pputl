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

decltype(is_obj) is_obj = NIFTY_DEF(is_obj, [&](va any) {
  docs << "[internal] detects if sym is an obj.";

  def chk_obj = def{"chk_\\" + fwd::obj + "(...)"} = [&](va) {
    return "";
  };

  def<"\\0(...)"> _0 = [&] {
    return def<"o(...)">{[&](va) {
      return "0";
    }};
  };
  def<"\\01(any)">{} = [&](arg any) {
    return def<"o(any, ...)">{[&](arg, va) {
      def<"\\0(...)"> _0 = [&] {
        return "0";
      };
      def<"\\01(any)">{} = [&](arg any) {
        return def<"o(...)">{[&](va) {
          def<"\\0"> _0 = [&] {
            return "1";
          };
          def<"\\01">{} = [&] {
            return "0";
          };
          return pp::cat(_0, pp::va_opt(1));
        }}(pp::cat(utl::slice(chk_obj, -((std::string const&)fwd::obj).size()), any));
      };
      return pp::cat(_0, pp::va_opt(1));
    }}(any, eat + " " + any);
  };
  return pp::call(pp::call(pp::cat(_0, pp::va_opt(1)), any), any);
});

} // namespace impl
} // namespace api

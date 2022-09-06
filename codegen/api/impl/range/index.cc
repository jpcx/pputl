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

#include "impl/range.h"

namespace api {
namespace impl {

using namespace codegen;

decltype(index) index = NIFTY_DEF(index, [&](arg i, arg sign, arg sz, arg err) {
  docs << "[internal] translates an idx to a non-negative zero-offset for a given range "
          "size.";

  def<"\\0(i, sz, err)"> sign0 = [&](arg i, arg sz, arg err) {
    def<"\\0(i, err)"> _0 = [&](arg, arg err) { return fail(err); };
    def<"\\1(i, err)">{}  = [&](arg i, arg) { return i; };

    return pp::call(xcat(utl::slice(_0, -1), lt(i, inc(sz))), i, err);
  };

  def<"\\1(i, sz, err)">{} = [&](arg i, arg sz, arg err) {
    def<"\\0(i, sz, err)"> _0 = [&](arg i, arg sz, arg err) { return sign0(i, sz, err); };
    def<"\\1(i, sz, err)">{}  = [&](arg i, arg sz, arg err) {
      def<"\\0(i, sz, err)"> _0 = [&](arg, arg, arg err) { return fail(err); };
      def<"\\1(i, sz, err)">{}  = [&](arg i, arg sz, arg) { return add(i, sz); };

      return pp::call(xcat(utl::slice(_0, -1), lt(neg(i), inc(sz))), i, sz, err);
    };

    return pp::call(xcat(utl::slice(_0, -1), ltz(i)), i, sz, err);
  };

  return pp::call(cat(utl::slice(sign0, -1), sign), i, sz, err);
});

} // namespace impl
} // namespace api

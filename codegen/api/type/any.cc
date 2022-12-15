#ifndef CODEGEN_API_TYPE_ANY_CC
#define CODEGEN_API_TYPE_ANY_CC
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
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//                                                                            //
//  You should have received a copy of the GNU General Public License        ///
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.  ////
///////////////////////////////////////////////////////////////////////////// */

#include "codegen.h"

#include "lang/cat.cc"
#include "lang/cstr.cc"
#include "traits/is_any.cc"

namespace codegen {
namespace api {

namespace any_ {

using namespace std;

inline def<"any(...: any...) -> any"> self = [](va args) {
  category = "type";

  docs << "any potentially-empty, individual argument in __VA_ARGS__."
       << "this constructor asserts that args are either 0 or 1 in size.";

  def<"\\0(e, ...)"> _0 = [](arg e, va) {
    return pp::call(apiname("fail"), e);
  };

  def<"\\1(e, ...)">{} = [](arg, va args) {
    return args;
  };

  return pp::call(cat(utl::slice(_0, -1), is_any(args)),
                  pp::call(apiname("err"), self,
                           pp::str("any can only represent args of size 0 or 1"),
                           args),
                  args);
};

} // namespace any_

inline constexpr auto& any = any_::self;

} // namespace api
} // namespace codegen

#endif

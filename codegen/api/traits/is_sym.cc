#ifndef CODEGEN_API_TRAITS_IS_SYM_CC
#define CODEGEN_API_TRAITS_IS_SYM_CC
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

#include <iomanip>
#include <sstream>

#include "codegen.h"

#include "config/int_min.cc"
#include "lang/cat.cc"
#include "traits/is_some.cc"
#include "traits/is_tup.cc"

namespace codegen {
namespace api {

namespace is_sym_ {

using namespace std;

inline def<"is_sym(...: any...) -> bool"> self = [](va args) {
  category = "traits";

  docs << "[extends is_some] checks if args is a pputl sym."
       << ""
       << "warning: breaks on single inputs that are not one of the"
       << "         predefined types and are not compatible with the"
       << "         concatenation operator."
       << ""
       << "syms are defined as follows:"
       << ""
       << "  #define " + apiname("sym") + "_<name>_is_<name> (<data...>)"
       << ""
       << "this layout allows syms to be compared using compare.eq and compare.ne."
       << ""
       << "the sym type lays the foundation for arithmetic literals, obj member"
       << "access,  and negative integers.  since arithmetic tokens cannot form"
       << "identifiers,  the C++ compl operator is used to ensure that negative"
       << "ints  can be parsed by the library and have the same meaning in both"
       << "the preprocessor and C++ code. when using an int or num to construct"
       << "an identifier, use lang.cat (which converts ints < 0 to 12-bit hex).";

  tests << self("")         = "false" >> docs;
  tests << self("()")       = "false" >> docs;
  tests << self("foo")      = "false" >> docs;
  tests << self("foo, bar") = "false" >> docs;
  tests << self("false")    = "true" >> docs;
  tests << self("0x00u")    = "true" >> docs;
  tests << self("0")        = "true" >> docs;
  tests << self(int_min)    = "true" >> docs;

  def<"\\FF(e, ...)"> ff = [](arg, va) {
    return "false";
  };

  def<"\\TF(e, v)">{} = [](arg e, arg v) {
    def<"\\FF(e, v)"> ff = [](arg e, arg v) {
      def<"\\FF(e)"> ff = [](arg) {
        return "false";
      };

      def<"\\FT(e)">{} = [](arg) {
        return "true";
      };

      def<"\\TF(e)">{} = [](arg) {
        return "true";
      };

      def<"\\TT(e)">{} = [](arg e) {
        return pp::call(apiname("fail"), e);
      };

      return pp::call(cat(utl::slice(ff, -2),
                          cat(is_tup(pp::cat(apiname("sym") + "_", v, "_is_", v)),
                              is_tup(pp::cat(implname("impl_sym") + "_", v, "_is_", v)))),
                      e);
    };

    def<"\\FT(e, v)">{} = [](arg, arg) {
      return "true";
    };

    def<"\\TF(e, v)">{} = [](arg, arg) {
      return "true";
    };

    def<"\\TT(e, v)">{} = [](arg e, arg) {
      return pp::call(apiname("fail"), e);
    };

    return pp::call(
        cat(utl::slice(ff, -2), cat(is_tup(pp::cat(apiname("sym") + "_", v)),
                                    is_tup(pp::cat(implname("impl_sym") + "_", v)))),
        e, v);
  };

  def<"\\TT(e, v)">{} = [](arg, arg) {
    return "false";
  };

  return pp::call(
      cat(utl::slice(ff, -2), cat(is_some(args), is_tup(args))),
      pp::call(apiname("err"), self, pp::str("detected a redefined reserved sym"), args),
      args);
};

} // namespace is_sym_

inline constexpr auto& is_sym = is_sym_::self;

} // namespace api
} // namespace codegen

#endif

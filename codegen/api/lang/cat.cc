#ifndef CODEGEN_API_LANG_CAT_CC
#define CODEGEN_API_LANG_CAT_CC
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

#include "lang/eat.cc"
#include "traits/is_none.cc"

namespace codegen {
namespace api {

namespace cat_ {

using namespace std;

inline def<"cat(...: a: any, b: any) -> any"> self = [](va args) {
  category = "lang";

  docs << "sensibly concatenates two args. must be compatible with the ## operator."
       << ""
       << "cat performs the following conversions for simplified identifier construction:"
       << " - true                             -> T"
       << " - false                            -> F"
       << " - compl(0x7FF), compl(0x7FE), etc. -> 0x800, 0x801, etc.";

  tests << self("foo", "bar")           = "foobar" >> docs;
  tests << self("foo", eat("bar"))      = "foo" >> docs;
  tests << self(",")                    = "" >> docs;
  tests << self("is_", "true")          = "is_T" >> docs;
  tests << self("true", "_is")          = "T_is";
  tests << self("is_", "false")         = "is_F" >> docs;
  tests << self("false", "_is")         = "F_is";
  tests << self("true", "false")        = "TF";
  tests << self("foo_", "compl(0x7FF)") = "foo_0x800" >> docs;
  tests << self("compl(0x7FF)", "_foo") = "0x800_foo";

  def<"chk(a, b)"> chk = [](arg a, arg b) {
    def<"\\compl(...)"> _compl = [](va) {
      return "";
    };

    def<"\\false">{} = [] {
      return "";
    };

    def<"\\true">{} = [] {
      return "";
    };

    return is_none(pp::cat(utl::slice(_compl, -5), a)) + ", "
         + is_none(pp::cat(utl::slice(_compl, -5), b));
  };

  return def<"o(...)">{[&](va args) {
    return def<"<o(ca, cb, a, b)">{[](arg ca, arg cb, arg a, arg b) {
      def<"\\falsefalse(a, b)"> _falsefalse = [](arg a, arg b) {
        return pp::cat(a, b);
      };
      def<"\\falsetrue(a, b)">{} = [](arg a, arg b) {
        def<"switch(b)"> _switch = [](arg b) {
          def<"\\compl(...)"> _compl = [](va) {
            return "2";
          };

          def<"\\false">{} = [] {
            return "1";
          };

          def<"\\true">{} = [] {
            return "0";
          };

          return pp::cat(utl::slice(_compl, -5), b);
        };

        return def<"o(...)">{[](va args) {
          return def<"<o(cs, a, b)">{[](arg cs, arg a, arg b) {
            def<"\\2(a, b)"> _2 = [](arg, arg) {
              return "NOT_IMPLEMENTED";
            };

            def<"\\1(a, b)">{} = [](arg a, arg) {
              return pp::cat(a, "F");
            };

            def<"\\0(a, b)">{} = [](arg a, arg) {
              return pp::cat(a, "T");
            };

            return pp::call(pp::cat(utl::slice(_2, -1), cs), a, b);
          }}(args);
        }}(_switch(b), a, b);
      };
      def<"\\truefalse(a, b)">{} = [](arg a, arg b) {
        def<"switch(a)"> _switch = [](arg a) {
          def<"\\compl(...)"> _compl = [](va) {
            return "2";
          };

          def<"\\false">{} = [] {
            return "1";
          };

          def<"\\true">{} = [] {
            return "0";
          };

          return pp::cat(utl::slice(_compl, -5), a);
        };

        return def<"o(...)">{[](va args) {
          return def<"<o(cs, a, b)">{[](arg cs, arg a, arg b) {
            def<"\\2(a, b)"> _2 = [](arg, arg) {
              return "NOT_IMPLEMENTED";
            };

            def<"\\1(a, b)">{} = [](arg, arg b) {
              return pp::cat("F", b);
            };

            def<"\\0(a, b)">{} = [](arg, arg b) {
              return pp::cat("T", b);
            };

            return pp::call(pp::cat(utl::slice(_2, -1), cs), a, b);
          }}(args);
        }}(_switch(a), a, b);
      };
      def<"\\truetrue(a, b)">{} = [](arg a, arg b) {
        def<"switch(a, b)"> _switch = [](arg a, arg b) {
          def<"\\compl(...)"> _compl = [](va) {
            return "2";
          };

          def<"\\false">{} = [] {
            return "1";
          };

          def<"\\true">{} = [] {
            return "0";
          };

          return pp::cat(utl::slice(_compl, -5), a) + ", "
               + pp::cat(utl::slice(_compl, -5), b);
        };

        return def<"o(...)">{[](va args) {
          return def<"<o(csa, csb, a, b)">{[](arg csa, arg csb, arg a, arg b) {
            def<"\\22(a, b)"> _22 = [](arg, arg) {
              return "NOT_IMPLEMENTED";
            };

            def<"\\21(a, b)">{} = [](arg, arg) {
              return "NOT_IMPLEMENTED";
            };

            def<"\\20(a, b)">{} = [](arg, arg) {
              return "NOT_IMPLEMENTED";
            };

            def<"\\12(a, b)">{} = [](arg, arg) {
              return "NOT_IMPLEMENTED";
            };

            def<"\\11(a, b)">{} = [](arg, arg) {
              return "FF";
            };

            def<"\\10(a, b)">{} = [](arg, arg) {
              return "FT";
            };

            def<"\\02(a, b)">{} = [](arg, arg) {
              return "NOT_IMPLEMENTED";
            };

            def<"\\01(a, b)">{} = [](arg, arg) {
              return "TF";
            };

            def<"\\00(a, b)">{} = [](arg, arg) {
              return "TT";
            };

            return pp::call(pp::cat(utl::slice(_22, -2), csa, csb), a, b);
          }}(args);
        }}(_switch(a, b), a, b);
      };
      return pp::call(pp::cat(utl::slice(_falsefalse, -10), ca, cb), a, b);
    }}(args);
  }}(chk(args), args);
};

} // namespace cat_

inline constexpr auto& cat = cat_::self;

} // namespace api
} // namespace codegen

#endif

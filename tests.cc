/* /////////////////////////////////////////////////////////////////////////////
//                          __    ___                                     `/////
//                         /\ \__/\_ \                                     `////
//   _____   _____   __  __\ \ ,_\//\ \                                     `///
//  /\ '__`\/\ '__`\/\ \/\ \\ \ \/ \ \ \                                     `//
//  \ \ \_\ \ \ \_\ \ \ \_\ \\ \ \_ \_\ \_                                    //
//   \ \ ,__/\ \ ,__/\ \____/ \ \__\/\____\                                   //
//    \ \ \   \ \ \   \/___/   \/__/\/____/                                   //
//     \/_/    \/_/                                                           //
//                                                                            //
//  pputl Preprocessor Utilities                                              //
//  Copyright (C) 2020 - 2022 Justin Collier <m@jpcx.dev>                     //
//                                                                            //
//    This program is free software: you can redistribute it and/or modify    //
//    it under the terms of the GNU General Public License as published by    //
//    the Free Software Foundation, either version 3 of the License, or       //
//    (at your option) any later version.                                     //
//                                                                            //
//    This program is distributed in the hope that it will be useful,         //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of          //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           //
//    GNU General Public License for more details.                            //
//                                                                            //
//  You should have received a copy of the GNU General Public License        ///
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.  ////
//                                                                         /////
////////////////////////////////////////////////////////////////////////////////
//                                                                        `/////
//    PREAMBLE                                                             `////
//    --------                                                              `///
//                                                                           `//
//    Macro functions are generally not advisable in production code. They    //
//    are difficult to reason about, pollute the global namespace, and can    //
//    hinder debugging and refactoring efforts.  C++ has evolved to enable    //
//    countless metaprogramming techniques that should be preferred.          //
//                                                                            //
//    This library is built to provide a strong, safe set of functionality    //
//    for edge cases that still uniquely benefit from text replacement and    //
//    would otherwise utilize a separate code generation script or require    //
//    higly verbose/redundant syntax, such as comprehensive test coverage,    //
//    struct reflection, static initialization control, or optimization of    //
//    template specialization algorithms.                                     //
//                                                                            //
//    ABOUT                                                                   //
//    -----                                                                   //
//                                                                            //
//    pputl is a powerful C++ preprocessor utilities library that provides    //
//    many language constructs  including a type system, integers, errors,    //
//    recursion, polymorphic objects, and common datastructures.              //
//                                                                            //
//    Speed, safety, and flexibility are its primary goals.                   //
//                                                                            //
//    Algorithms operate using an inline recursion technique that requires    //
//    fewer expansions than mutually-recursive patterns and can be nested.    //
//    Macros are provided for performing arbitrary, nestable recursion.       //
//                                                                            //
//    Functions verify their arguments and use type casting to ensure that    //
//    useful error messages are generated as soon as an issue manifests.      //
//                                                                            //
//    USAGE                                                                   //
//    -----                                                                   //
//                                                                            //
//    pputl is a standalone single-header library. include pputl.h to use.    //
//    A C++20-compliant preprocessor is required.                             //
//                                                                            //
//    pputl  is completely generated and tested by a custom C++ framework.    //
//    See the codegen/ folder for the full source.                            //
//                                                                            //
//    Various settings including word size and naming rules may be changed    //
//    by modifying the head of codegen/codegen.h and running `make`.          //
//                                                                            //
//    The default build defines 12-bit words and an 8-bit size cap,  which    //
//    complies with the following C++20 implementation limits [implimits]:    //
//                                                                            //
//     ‐ Macro identifiers simultaneously                                     //
//       defined in one translation unit: [65536].                            //
//     ‐ Parameters in one macro definition: [256].                           //
//     ‐ Arguments in one macro invocation: [256].                            //
//                                                                            //
//    Exceeding these limits  is possible but depends on the preprocessor.    //
//    The size cap is bounded by the maximum number of parameters, and the    //
//    word size is directly proportional to the number of macros defined.     //
//                                                                            //
//    pputl has been tested with:                                             //
//                                                                            //
//      gcc   11.2.1                                                          //
//      clang 13.0.0                                                          //
//      MSVC  2022 (requires /Zc:preprocessor)                                //
//                                                                            //
//    Run `make test` to validate on your system.                             //
//                                                                            //
//    TYPES                                                                   //
//    -----                                                                   //
//                                                                            //
//    pputl makes extensive use of duck-typing  for control flow and error    //
//    management.  pputl types are essentially pairs of functions: one for    //
//    traits identification and another for construction and assertion.       //
//                                                                            //
//    API functions are strictly documented using this type system. Inputs    //
//    are validated by invoking the associated constructor or through some    //
//    other form of inference. An argument is valid if it can be converted    //
//    to (or interpreted as) its parameter without losing information.        //
//                                                                            //
//     any: any potentially-empty, individual argument in __VA_ARGS__         //
//      ├╴none: an empty argument; an absence of pp-tokens                    //
//      └╴some: a non-empty argument; a presence of pp-tokens                 //
//         ├╴tup: a parenthesized item sequence [e.g. (a, b, c)]              //
//         │  └╴pair: a two-tuple [e.g. (foo, bar)]                           //
//         ├╴sym: an explicitly defined equality-comparable token sequence    //
//         │  └╴num: a builtin, totally-ordered, arithmetic sym               //
//         │     ├╴bool: false|true                                           //
//         │     ├╴hex:  0x0u|0x1u|...|0xEu|0xFu                              //
//         │     ├╴size: 0x00u|0x01u|...|0xFEu|0xFFu                          //
//         │     └╴word: a 12-bit unsigned or signed two's complement int     //
//         │        ├╴int:  compl(0x7FF)|compl(0x7FE)|...|0|...|2046|2047     //
//         │        └╴uint: 0u|1u|...|4094u|4095u                             //
//         └╴obj: a named, polymorphic, member-addressable state container    //
//            ├╴err:   an error message container for lang.fail               //
//            ├╴vec:   a resizable array                                      //
//            ├╴map:   a mapping of equality-comparable keys to any           //
//            ├╴pq:    a priority queue                                       //
//            ├╴queue: a FIFO queue                                           //
//            └╴stack: a LIFO queue                                           //
//                                                                            //
//    NOTES                                                                   //
//    -----                                                                   //
//                                                                            //
//    pputl errors  execute an invalid preprocessor operation by using the    //
//    concatenation operator (incorrectly) on a string error message.  All    //
//    errors produced by pputl functions include the macro name, a textual    //
//    description, and the primary expansion arguments.                       //
//                                                                            //
//    pputl API functions  are fully variadic and chainable,  meaning that    //
//    their arguments can be populated using macro expansion results. Args    //
//    must not grow, shrink, or change types after the primary expansion.     //
//                                                                            //
//    pputl num types are mutually exclusive token sequences that have the    //
//    same numeric meaning in pputl, the preprocessor, and C++.  Since the    //
//    preprocessor does not support hyphens in identifiers,  negative ints    //
//    are special syms that cannot form part of an identifier  (unlike all    //
//    other syms). Negative ints are designed to be fully parseable by the    //
//    library while still meeting these requirements. When constructing an    //
//    identifier from an int,  use lang.cat instead of the ## operator  to   ///
//    replace negative ints with their 12-bit hex equivalent (e.g. 0x800).  ////
//                                                                         /////
///////////////////////////////////////////////////////////////////////////// */

#include "pputl.h"

namespace {
// streq that trims spacing around outer ASSERT_PP_EQ
// parens to account for preprocessor discrepancies
// / *( *\(.*\) */
static constexpr bool
pp_streq(char const* l, char const* r) {
  for (; *l and *l != '('; ++l)
    ;
  for (; *l and *r != '('; ++r)
    ;
  if (not *l)
    return not *r;
  if (not *r)
    return not *l;
  ++l;
  ++r;
  for (; *l == ' '; ++l)
    ;
  for (; *r == ' '; ++r)
    ;
  char const* le = l;
  char const* re = r;
  for (; *le; ++le)
    ;
  for (; *re; ++re)
    ;
  for (; le > l and *(le - 1) != ')'; --le)
    ;
  for (; re > r and *(re - 1) != ')'; --re)
    ;
  --le;
  --re;
  for (; le > l and *(le - 1) == ' '; --le)
    ;
  for (; re > r and *(re - 1) == ' '; --re)
    ;
  for (; l != le and r != re and *l == *r; ++l, ++r)
    ;
  return l == le and r == re;
}
} // namespace

#define ASSERT_PP_EQ_X(a, b) static_assert(pp_streq(#a, #b), #a " != " #b)
#define ASSERT_PP_EQ(a, b)   ASSERT_PP_EQ_X(a, b)

// clang-format off
ASSERT_PP_EQ((PTL_FIRST()), ());
ASSERT_PP_EQ((PTL_FIRST(, )), ());
ASSERT_PP_EQ((PTL_FIRST(a)), (a));
ASSERT_PP_EQ((PTL_FIRST(a, b)), (a));
ASSERT_PP_EQ((PTL_FIRST((a, b))), ((a, b)));

ASSERT_PP_EQ((PTL_REST()), ());
ASSERT_PP_EQ((PTL_REST(, )), ());
ASSERT_PP_EQ((PTL_REST(a)), ());
ASSERT_PP_EQ((PTL_REST(a, b)), (b));
ASSERT_PP_EQ((PTL_REST(a, b, c)), (b, c));
ASSERT_PP_EQ((PTL_REST(PTL_REST(a, b, c))), (c));
ASSERT_PP_EQ((PTL_REST(a, , )), (,));
ASSERT_PP_EQ((PTL_REST(a, b, , )), (b, ,));

ASSERT_PP_EQ((PTL_EAT()), ());
ASSERT_PP_EQ((PTL_EAT(foo)), ());

ASSERT_PP_EQ((PTL_CAT(foo, bar)), (foobar));
ASSERT_PP_EQ((PTL_CAT(foo, PTL_EAT(bar))), (foo));
ASSERT_PP_EQ((PTL_CAT(,)), ());

ASSERT_PP_EQ((PTL_STR()), (""));
ASSERT_PP_EQ((PTL_STR(foo, bar)), ("foo, bar"));
ASSERT_PP_EQ((PTL_STR(PTL_CAT(foo, bar))), ("foobar"));
ASSERT_PP_EQ((PTL_STR(foo)), ("foo"));
ASSERT_PP_EQ((PTL_STR(, )), (","));
ASSERT_PP_EQ((PTL_STR(, , )), (", ,"));
ASSERT_PP_EQ((PTL_STR(a, )), ("a,"));
ASSERT_PP_EQ((PTL_STR(a, , )), ("a, ,"));
ASSERT_PP_EQ((PTL_STR(, a)), (", a"));
ASSERT_PP_EQ((PTL_STR(, a, )), (", a,"));
ASSERT_PP_EQ((PTL_STR(, , a)), (", , a"));

ASSERT_PP_EQ((PTL_DEFAULT()), ());
ASSERT_PP_EQ((PTL_DEFAULT(a)), (a));
ASSERT_PP_EQ((PTL_DEFAULT(a,)), (a));
ASSERT_PP_EQ((PTL_DEFAULT(a, b)), (b));
ASSERT_PP_EQ((PTL_DEFAULT(a, b, c)), (b, c));

ASSERT_PP_EQ((PTL_TRIM()), ());
ASSERT_PP_EQ((PTL_TRIM(, )), ());
ASSERT_PP_EQ((PTL_TRIM(a)), (a));
ASSERT_PP_EQ((PTL_TRIM(a, b)), (a, b));
ASSERT_PP_EQ((PTL_TRIM(a, )), (a));
ASSERT_PP_EQ((PTL_TRIM(, b, c)), (b, c));
ASSERT_PP_EQ((PTL_TRIM(a, b, )), (a, b,));
ASSERT_PP_EQ((PTL_TRIM(a, b, c)), (a, b, c));
ASSERT_PP_EQ((PTL_TRIM(, b)), (b));
ASSERT_PP_EQ((PTL_TRIM(a, , c)), (a,  , c));

ASSERT_PP_EQ((PTL_IS_NONE()), (true));
ASSERT_PP_EQ((PTL_IS_NONE(foo)), (false));
ASSERT_PP_EQ((PTL_IS_NONE(foo, bar)), (false));
ASSERT_PP_EQ((PTL_IS_NONE(PTL_ESC())), (true));
ASSERT_PP_EQ((PTL_IS_NONE(, )), (false));
ASSERT_PP_EQ((PTL_IS_NONE(, , )), (false));
ASSERT_PP_EQ((PTL_IS_NONE(a, )), (false));
ASSERT_PP_EQ((PTL_IS_NONE(a, , )), (false));
ASSERT_PP_EQ((PTL_IS_NONE(, a)), (false));
ASSERT_PP_EQ((PTL_IS_NONE(, a, )), (false));
ASSERT_PP_EQ((PTL_IS_NONE(, , a)), (false));

ASSERT_PP_EQ((PTL_IS_SOME()), (false));
ASSERT_PP_EQ((PTL_IS_SOME(foo)), (true));
ASSERT_PP_EQ((PTL_IS_SOME(())), (true));
ASSERT_PP_EQ((PTL_IS_SOME((a, b))), (true));
ASSERT_PP_EQ((PTL_IS_SOME(foo, bar)), (false));
ASSERT_PP_EQ((PTL_IS_SOME(PTL_ESC())), (false));
ASSERT_PP_EQ((PTL_IS_SOME(, )), (false));
ASSERT_PP_EQ((PTL_IS_SOME(, , )), (false));
ASSERT_PP_EQ((PTL_IS_SOME(a, )), (false));
ASSERT_PP_EQ((PTL_IS_SOME(a, , )), (false));
ASSERT_PP_EQ((PTL_IS_SOME(, a)), (false));
ASSERT_PP_EQ((PTL_IS_SOME(, a, )), (false));
ASSERT_PP_EQ((PTL_IS_SOME(, , a)), (false));

ASSERT_PP_EQ((PTL_IS_TUP()), (false));
ASSERT_PP_EQ((PTL_IS_TUP(1, 2)), (false));
ASSERT_PP_EQ((PTL_IS_TUP(())), (true));
ASSERT_PP_EQ((PTL_IS_TUP((1, 2))), (true));
ASSERT_PP_EQ((PTL_IS_TUP((), ())), (false));
ASSERT_PP_EQ((PTL_IS_TUP(PTL_ESC(()))), (true));
ASSERT_PP_EQ((PTL_IS_TUP(PTL_ESC((1, 2)))), (true));
ASSERT_PP_EQ((PTL_IS_TUP(, )), (false));
ASSERT_PP_EQ((PTL_IS_TUP(, , )), (false));
ASSERT_PP_EQ((PTL_IS_TUP(a, )), (false));
ASSERT_PP_EQ((PTL_IS_TUP(a, , )), (false));
ASSERT_PP_EQ((PTL_IS_TUP(, a)), (false));
ASSERT_PP_EQ((PTL_IS_TUP(, a, )), (false));
ASSERT_PP_EQ((PTL_IS_TUP(, , a)), (false));
ASSERT_PP_EQ((PTL_IS_TUP((, ))), (true));
ASSERT_PP_EQ((PTL_IS_TUP((, , ))), (true));
ASSERT_PP_EQ((PTL_IS_TUP((a, ))), (true));
ASSERT_PP_EQ((PTL_IS_TUP((a, , ))), (true));
ASSERT_PP_EQ((PTL_IS_TUP((, a))), (true));
ASSERT_PP_EQ((PTL_IS_TUP((, a, ))), (true));
ASSERT_PP_EQ((PTL_IS_TUP((, , a))), (true));

ASSERT_PP_EQ((PTL_IS_ANY()), (true));
ASSERT_PP_EQ((PTL_IS_ANY(foo)), (true));
ASSERT_PP_EQ((PTL_IS_ANY((a, b))), (true));
ASSERT_PP_EQ((PTL_IS_ANY(a, b)), (false));
ASSERT_PP_EQ((PTL_IS_ANY(, )), (false));
ASSERT_PP_EQ((PTL_IS_ANY(, , )), (false));
ASSERT_PP_EQ((PTL_IS_ANY(a, )), (false));
ASSERT_PP_EQ((PTL_IS_ANY(a, , )), (false));
ASSERT_PP_EQ((PTL_IS_ANY(, a)), (false));
ASSERT_PP_EQ((PTL_IS_ANY(, a, )), (false));
ASSERT_PP_EQ((PTL_IS_ANY(, , a)), (false));
// clang-format on

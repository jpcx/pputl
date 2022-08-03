/* /////////////////////////////////////////////////////////////////////////////
//                          __    ___                                         //
//                         /\ \__/\_ \                                        //
//   _____   _____   __  __\ \ ,_\//\ \                                       //
//  /\ '__`\/\ '__`\/\ \/\ \\ \ \/ \ \ \                                      //
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
//    Caution:  macros should be used sparingly or not at all if possible.    //
//    C++ has evolved to facilitate  countless  metaprogramming techniques    //
//    that  should be  preferred in most cases,  as they are  predictable,    //
//    type-safe, scoped, and easier to debug. pputl is primarily  intended    //
//    for  research purposes and for various edge cases that still must be    //
//    solved using  text replacement,  such as optimizations that minimize    //
//    template specializations and syntactical boilerplate reduction.         //
//                                                                            //
//    ABOUT                                                                   //
//    -----                                                                   //
//                                                                            //
//    pputl is a powerful C++ preprocessor utilities library that provides    //
//    many high-level programming constructs including unsigned arithmetic    //
//    and comparisons,  logic,  control flow,  generation, transformation,    //
//    reduction, function binding, overloading, and range selection. pputl    //
//    is  completely  generated  by a  custom  framework  that  transforms    //
//    shorthand signatures and library invocations into macros, tests, and    //
//    documentation.                                                          //
//                                                                            //
//    pputl makes use of recursion to the maximum extent possible to limit    //
//    the number of macro definitions for the configured unsigned maximum.    //
//    See range.split and algo.reduce for useful examples of the two types    //
//    of recursive calls supported by this library.                           //
//                                                                            //
//    pputl requires __VA_ARGS__, __VA_OPT__, and empty variadic arguments    //
//    support (which are guaranteed by C++20)  but has no dependencies and    //
//    is single-header.                                                       //
//                                                                            //
//    USAGE                                                                   //
//    -----                                                                   //
//    Copy pputl.h and include. The default build defines a 10bit unsigned    //
//    type  that  underflows and overflows  according to standard unsigned    //
//    rules.  Variadic argument sizes are usually  capped by the uint max.    //
//    Modify the head of codegen/codegen.h  and make  to set a custom uint    //
//    maximum or change the symbol naming rules.                              //
//                                                                            //
//    GUIDELINES                                                              //
//    ----------                                                              //
//                                                                            //
//    Non-nullary API functions are fully variadic and chainable such that    //
//    the outputs of one may be used as inputs to another. Parameters must    //
//    be fully expanded and distinguishable after the primary expansion.      //
//                                                                            //
//    Tuples are used only when necessary;  most functions that operate on    //
//    data ranges both input and output a variadic argument list. Creating    //
//    a tuple is trivial but extraction costs an expansion.                   //
//                                                                            //
//    pputl defines three types: tuple, uint, and bool. Functions that use    //
//    these types in their parameter documentation  will assert their that    //
//    the inputs are of the correct type.                                     //
//                                                                            //
//    pputl errors  are thrown  by invoking a directly-recursive call with    //
//    the original arguments. This has the effect of terminating expansion    //
//    in a way that preserves both the function name and its arguments.       //
//                                                                            //
//    TESTING                                                                 //
//    -------                                                                 //
//    pputl  is statically tested by the build system.  Run `make test` to   ///
//    validate the library or open tests.cc in an LSP-enabled editor.       ////
//                                                                         /////
///////////////////////////////////////////////////////////////////////////// */

#include "pputl.h"

static constexpr bool
streq(char const* l, char const* r) {
  for (; *l and *r and *l == *r; ++l, ++r)
    ;
  return *l == *r;
}

#define ASSERT_PP_EQ_X(a, b) static_assert(streq(#a, #b), #a " != " #b)
#define ASSERT_PP_EQ(a, b)   ASSERT_PP_EQ_X(a, b)

// clang-format off
ASSERT_PP_EQ((PTL_ESC ()), ());
ASSERT_PP_EQ((PTL_ESC (a, b, c)), (a, b, c));

ASSERT_PP_EQ((PTL_STR()), (""));
ASSERT_PP_EQ((PTL_STR(foo, bar)), ("foo, bar"));
ASSERT_PP_EQ((PTL_STR(foo)), ("foo"));
ASSERT_PP_EQ((PTL_STR(, )), (","));
ASSERT_PP_EQ((PTL_STR(, , )), (", ,"));
ASSERT_PP_EQ((PTL_STR(a, )), ("a,"));
ASSERT_PP_EQ((PTL_STR(a, , )), ("a, ,"));
ASSERT_PP_EQ((PTL_STR(, a)), (", a"));
ASSERT_PP_EQ((PTL_STR(, a, )), (", a,"));
ASSERT_PP_EQ((PTL_STR(, , a)), (", , a"));

ASSERT_PP_EQ((PTL_CAT(foo, bar)), (foobar));
ASSERT_PP_EQ((PTL_CAT(foo, PTL_EAT(bar))), (foo));

ASSERT_PP_EQ((PTL_FIRST()), ());
ASSERT_PP_EQ((PTL_FIRST(, )), ());
ASSERT_PP_EQ((PTL_FIRST(a)), (a));
ASSERT_PP_EQ((PTL_FIRST(a, b)), (a));

ASSERT_PP_EQ((PTL_REST()), ());
ASSERT_PP_EQ((PTL_REST(, )), ());
ASSERT_PP_EQ((PTL_REST(a)), ());
ASSERT_PP_EQ((PTL_REST(a, b)), (b));
ASSERT_PP_EQ((PTL_REST(a, b, c)), (b, c));
ASSERT_PP_EQ((PTL_REST(PTL_REST(a, b, c))), (c));
ASSERT_PP_EQ((PTL_REST(a, , )), (,));
ASSERT_PP_EQ((PTL_REST(a, b, , )), (b, ,));

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

ASSERT_PP_EQ((PTL_TUPLE(())), (()));
ASSERT_PP_EQ((PTL_TUPLE((1, 2))), ((1, 2)));
ASSERT_PP_EQ((PTL_STR(PTL_TUPLE(0))), ("PTL_TUPLE(0)"));
ASSERT_PP_EQ((PTL_STR(PTL_TUPLE(1, 2))), ("PTL_TUPLE(1, 2)"));
ASSERT_PP_EQ((PTL_STR(PTL_TUPLE(1,))), ("PTL_TUPLE(1,)"));
ASSERT_PP_EQ((PTL_STR(PTL_TUPLE(foo))), ("PTL_TUPLE(foo)"));
ASSERT_PP_EQ((PTL_STR(PTL_TUPLE((), ()))), ("PTL_TUPLE((), ())"));

ASSERT_PP_EQ((PTL_BOOL(0)), (0));
ASSERT_PP_EQ((PTL_BOOL(1)), (1));
ASSERT_PP_EQ((PTL_STR(PTL_BOOL(2))), ("PTL_BOOL(2)"));
ASSERT_PP_EQ((PTL_STR(PTL_BOOL(1, 2))), ("PTL_BOOL(1, 2)"));
ASSERT_PP_EQ((PTL_STR(PTL_BOOL(1,))), ("PTL_BOOL(1,)"));
ASSERT_PP_EQ((PTL_STR(PTL_BOOL(foo))), ("PTL_BOOL(foo)"));
ASSERT_PP_EQ((PTL_STR(PTL_BOOL(()))), ("PTL_BOOL(())"));
ASSERT_PP_EQ((PTL_STR(PTL_BOOL((), ()))), ("PTL_BOOL((), ())"));

ASSERT_PP_EQ((PTL_UINT(0)), (0));
ASSERT_PP_EQ((PTL_UINT(1)), (1));
ASSERT_PP_EQ((PTL_UINT(2)), (2));
ASSERT_PP_EQ((PTL_UINT(1023)), (1023));
ASSERT_PP_EQ((PTL_STR(PTL_UINT(1023))), ("1023"));
ASSERT_PP_EQ((PTL_STR(PTL_UINT(1024))), ("PTL_UINT(1024)"));
ASSERT_PP_EQ((PTL_STR(PTL_UINT(1, 2))), ("PTL_UINT(1, 2)"));
ASSERT_PP_EQ((PTL_STR(PTL_UINT(1,))), ("PTL_UINT(1,)"));
ASSERT_PP_EQ((PTL_STR(PTL_UINT(foo))), ("PTL_UINT(foo)"));
ASSERT_PP_EQ((PTL_STR(PTL_UINT(()))), ("PTL_UINT(())"));
ASSERT_PP_EQ((PTL_STR(PTL_UINT((), ()))), ("PTL_UINT((), ())"));

ASSERT_PP_EQ((PTL_INC(0)), (1));
ASSERT_PP_EQ((PTL_INC(1)), (2));
ASSERT_PP_EQ((PTL_INC(1023)), (0));
ASSERT_PP_EQ((PTL_STR(PTL_INC(1023))), ("0"));
ASSERT_PP_EQ((PTL_STR(PTL_INC())), ("PTL_INC()"));
ASSERT_PP_EQ((PTL_STR(PTL_INC(a))), ("PTL_INC(a)"));
ASSERT_PP_EQ((PTL_STR(PTL_INC(foo))), ("PTL_INC(foo)"));
ASSERT_PP_EQ((PTL_INC(2)), (3));
ASSERT_PP_EQ((PTL_INC(1022)), (1023));

ASSERT_PP_EQ((PTL_DEC(0)), (1023));
ASSERT_PP_EQ((PTL_DEC(1)), (0));
ASSERT_PP_EQ((PTL_DEC(1023)), (1022));
ASSERT_PP_EQ((PTL_STR(PTL_DEC(1023))), ("1022"));
ASSERT_PP_EQ((PTL_STR(PTL_DEC())), ("PTL_DEC()"));
ASSERT_PP_EQ((PTL_STR(PTL_DEC(a))), ("PTL_DEC(a)"));
ASSERT_PP_EQ((PTL_STR(PTL_DEC(foo))), ("PTL_DEC(foo)"));
ASSERT_PP_EQ((PTL_DEC(2)), (1));
ASSERT_PP_EQ((PTL_DEC(1022)), (1021));

ASSERT_PP_EQ((PTL_EQZ(0)), (1));
ASSERT_PP_EQ((PTL_EQZ(1)), (0));
ASSERT_PP_EQ((PTL_EQZ(2)), (0));
ASSERT_PP_EQ((PTL_EQZ(1023)), (0));
ASSERT_PP_EQ((PTL_EQZ(PTL_INC(1023))), (1));
ASSERT_PP_EQ((PTL_STR(PTL_EQZ(1023))), ("0"));
ASSERT_PP_EQ((PTL_STR(PTL_EQZ())), ("PTL_EQZ()"));
ASSERT_PP_EQ((PTL_STR(PTL_EQZ(a))), ("PTL_EQZ(a)"));
ASSERT_PP_EQ((PTL_STR(PTL_EQZ(foo))), ("PTL_EQZ(foo)"));

ASSERT_PP_EQ((PTL_NEZ(0)), (0));
ASSERT_PP_EQ((PTL_NEZ(1)), (1));
ASSERT_PP_EQ((PTL_NEZ(2)), (1));
ASSERT_PP_EQ((PTL_NEZ(1023)), (1));
ASSERT_PP_EQ((PTL_NEZ(PTL_INC(1023))), (0));
ASSERT_PP_EQ((PTL_STR(PTL_NEZ(1023))), ("1"));
ASSERT_PP_EQ((PTL_STR(PTL_NEZ())), ("PTL_NEZ()"));
ASSERT_PP_EQ((PTL_STR(PTL_NEZ(a))), ("PTL_NEZ(a)"));
ASSERT_PP_EQ((PTL_STR(PTL_NEZ(foo))), ("PTL_NEZ(foo)"));

ASSERT_PP_EQ((PTL_IS_NONE()), (1));
ASSERT_PP_EQ((PTL_IS_NONE(foo)), (0));
ASSERT_PP_EQ((PTL_IS_NONE(foo, bar)), (0));
ASSERT_PP_EQ((PTL_IS_NONE(PTL_ESC())), (1));
ASSERT_PP_EQ((PTL_IS_NONE(, )), (0));
ASSERT_PP_EQ((PTL_IS_NONE(, , )), (0));
ASSERT_PP_EQ((PTL_IS_NONE(a, )), (0));
ASSERT_PP_EQ((PTL_IS_NONE(a, , )), (0));
ASSERT_PP_EQ((PTL_IS_NONE(, a)), (0));
ASSERT_PP_EQ((PTL_IS_NONE(, a, )), (0));
ASSERT_PP_EQ((PTL_IS_NONE(, , a)), (0));

ASSERT_PP_EQ((PTL_IS_SOME()), (0));
ASSERT_PP_EQ((PTL_IS_SOME(foo)), (1));
ASSERT_PP_EQ((PTL_IS_SOME(foo, bar)), (1));
ASSERT_PP_EQ((PTL_IS_SOME(PTL_ESC())), (0));
ASSERT_PP_EQ((PTL_IS_SOME(, )), (1));
ASSERT_PP_EQ((PTL_IS_SOME(, , )), (1));
ASSERT_PP_EQ((PTL_IS_SOME(a, )), (1));
ASSERT_PP_EQ((PTL_IS_SOME(a, , )), (1));
ASSERT_PP_EQ((PTL_IS_SOME(, a)), (1));
ASSERT_PP_EQ((PTL_IS_SOME(, a, )), (1));
ASSERT_PP_EQ((PTL_IS_SOME(, , a)), (1));

ASSERT_PP_EQ((PTL_IS_TUPLE()), (0));
ASSERT_PP_EQ((PTL_IS_TUPLE(1, 2)), (0));
ASSERT_PP_EQ((PTL_IS_TUPLE(())), (1));
ASSERT_PP_EQ((PTL_IS_TUPLE((1, 2))), (1));
ASSERT_PP_EQ((PTL_IS_TUPLE((), ())), (0));
ASSERT_PP_EQ((PTL_IS_TUPLE(PTL_ESC(()))), (1));
ASSERT_PP_EQ((PTL_IS_TUPLE(PTL_ESC((1, 2)))), (1));
ASSERT_PP_EQ((PTL_IS_TUPLE(, )), (0));
ASSERT_PP_EQ((PTL_IS_TUPLE(, , )), (0));
ASSERT_PP_EQ((PTL_IS_TUPLE(a, )), (0));
ASSERT_PP_EQ((PTL_IS_TUPLE(a, , )), (0));
ASSERT_PP_EQ((PTL_IS_TUPLE(, a)), (0));
ASSERT_PP_EQ((PTL_IS_TUPLE(, a, )), (0));
ASSERT_PP_EQ((PTL_IS_TUPLE(, , a)), (0));
ASSERT_PP_EQ((PTL_IS_TUPLE((, ))), (1));
ASSERT_PP_EQ((PTL_IS_TUPLE((, , ))), (1));
ASSERT_PP_EQ((PTL_IS_TUPLE((a, ))), (1));
ASSERT_PP_EQ((PTL_IS_TUPLE((a, , ))), (1));
ASSERT_PP_EQ((PTL_IS_TUPLE((, a))), (1));
ASSERT_PP_EQ((PTL_IS_TUPLE((, a, ))), (1));
ASSERT_PP_EQ((PTL_IS_TUPLE((, , a))), (1));

ASSERT_PP_EQ((PTL_IS_UINT()), (0));
ASSERT_PP_EQ((PTL_IS_UINT(foo)), (0));
ASSERT_PP_EQ((PTL_IS_UINT(0)), (1));
ASSERT_PP_EQ((PTL_IS_UINT(())), (0));
ASSERT_PP_EQ((PTL_IS_UINT((), ())), (0));
ASSERT_PP_EQ((PTL_IS_UINT(0, 1)), (0));
ASSERT_PP_EQ((PTL_IS_UINT(1023)), (1));
ASSERT_PP_EQ((PTL_IS_UINT(foo, bar)), (0));
ASSERT_PP_EQ((PTL_IS_UINT(1022)), (1));
ASSERT_PP_EQ((PTL_IS_UINT(, )), (0));
ASSERT_PP_EQ((PTL_IS_UINT(, , )), (0));
ASSERT_PP_EQ((PTL_IS_UINT(a, )), (0));
ASSERT_PP_EQ((PTL_IS_UINT(a, , )), (0));
ASSERT_PP_EQ((PTL_IS_UINT(, a)), (0));
ASSERT_PP_EQ((PTL_IS_UINT(, a, )), (0));
ASSERT_PP_EQ((PTL_IS_UINT(, , a)), (0));

ASSERT_PP_EQ((PTL_ITEMS(())), ());
ASSERT_PP_EQ((PTL_ITEMS((a))), (a));
ASSERT_PP_EQ((PTL_ITEMS((a, b))), (a, b));
ASSERT_PP_EQ((PTL_ITEMS((a, b, c))), (a, b, c));
ASSERT_PP_EQ((PTL_STR(PTL_ITEMS((a, b, c)))), ("a, b, c"));
ASSERT_PP_EQ((PTL_STR(PTL_ITEMS())), ("PTL_ITEMS()"));
ASSERT_PP_EQ((PTL_STR(PTL_ITEMS(not, tuple))), ("PTL_ITEMS(not, tuple)"));
ASSERT_PP_EQ((PTL_ITEMS(((a), (b), (c)))), ((a), (b), (c)));
ASSERT_PP_EQ((PTL_ITEMS((, ))), (,));
ASSERT_PP_EQ((PTL_ITEMS((, , ))), (, ,));
ASSERT_PP_EQ((PTL_ITEMS((a, ))), (a,));
ASSERT_PP_EQ((PTL_ITEMS((a, , ))), (a, ,));
ASSERT_PP_EQ((PTL_ITEMS((, a))), (, a));
ASSERT_PP_EQ((PTL_ITEMS((, a, ))), (, a,));
ASSERT_PP_EQ((PTL_ITEMS((, , a))), (, , a));

ASSERT_PP_EQ((PTL_SIZE()), (0));
ASSERT_PP_EQ((PTL_SIZE(a)), (1));
ASSERT_PP_EQ((PTL_SIZE(a, b)), (2));
ASSERT_PP_EQ((PTL_SIZE(, )), (2));
ASSERT_PP_EQ((PTL_SIZE(a, b, c)), (3));
ASSERT_PP_EQ((PTL_SIZE(,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,)), (1023));
ASSERT_PP_EQ((PTL_SIZE(, , )), (3));
ASSERT_PP_EQ((PTL_SIZE(a, )), (2));
ASSERT_PP_EQ((PTL_SIZE(a, , )), (3));
ASSERT_PP_EQ((PTL_SIZE(, a)), (2));
ASSERT_PP_EQ((PTL_SIZE(, a, )), (3));
ASSERT_PP_EQ((PTL_SIZE(, , a)), (3));
ASSERT_PP_EQ((PTL_STR(PTL_SIZE(,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,))), ("PTL_SIZE(,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,)"));

ASSERT_PP_EQ((PTL_STR(PTL_XTRACE)), ("PPUTLXTRACE_A ( , )"));
ASSERT_PP_EQ((PTL_STR(PTL_ESC(PTL_XTRACE))), ("PPUTLXTRACE_B ( ,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_ESC(PTL_ESC(PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_ESC(PTL_ESC(PTL_ESC(PTL_XTRACE))))), ("PPUTLXTRACE_B ( ,,,, )"));

ASSERT_PP_EQ((PTL_XCOUNT(PTL_XTRACE)), (0));
ASSERT_PP_EQ((PTL_XCOUNT(PTL_ESC(PTL_XTRACE))), (1));
ASSERT_PP_EQ((PTL_XCOUNT(PTL_ESC(PTL_ESC(PTL_XTRACE)))), (2));
ASSERT_PP_EQ((PTL_XCOUNT(PTL_ESC(PTL_ESC(PTL_ESC(PTL_XTRACE))))), (3));

ASSERT_PP_EQ((PTL_X(0)(PTL_XTRACE)), (PTL_ESC(PTL_XTRACE)));
ASSERT_PP_EQ((PTL_X(1)(PTL_XTRACE)), (PTL_ESC(PTL_ESC(PTL_XTRACE))));
ASSERT_PP_EQ((PTL_X(0)(PTL_XTRACE)), (PPUTLXTRACE_A ( , )));
ASSERT_PP_EQ((PTL_X(1)(PTL_XTRACE)), (PPUTLXTRACE_B ( ,, )));
ASSERT_PP_EQ((PTL_X(2)(PTL_XTRACE)), (PPUTLXTRACE_A ( ,,, )));
ASSERT_PP_EQ((PTL_X(3)(PTL_XTRACE)), (PPUTLXTRACE_B ( ,,,, )));
ASSERT_PP_EQ((PTL_X(4)(PTL_XTRACE)), (PPUTLXTRACE_A ( ,,,,, )));
ASSERT_PP_EQ((PTL_X(1023)(PTL_XTRACE)), (PPUTLXTRACE_B ( ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,, )));
ASSERT_PP_EQ((PTL_XCOUNT(PTL_X(1022)(PTL_XTRACE))), (1023));

ASSERT_PP_EQ((PTL_IF(1, (t), ())), (t));
ASSERT_PP_EQ((PTL_IF(0, (t), ())), ());
ASSERT_PP_EQ((PTL_IF(1, (t), (f))), (t));
ASSERT_PP_EQ((PTL_IF(0, (t), (f))), (f));
ASSERT_PP_EQ((PTL_IF(1, (a), (b, c))), (a));
ASSERT_PP_EQ((PTL_IF(0, (a), (b, c))), (b, c));

ASSERT_PP_EQ((PTL_SWITCH(0, (1))), (1));
ASSERT_PP_EQ((PTL_SWITCH(1, (1), (2))), (2));
ASSERT_PP_EQ((PTL_SWITCH(2, (1), (2), (3, 4))), (3, 4));
ASSERT_PP_EQ((PTL_SWITCH(0, (1), (2))), (1));
ASSERT_PP_EQ((PTL_SWITCH(1, (1), (2), (3))), (2));
ASSERT_PP_EQ((PTL_SWITCH(2, (1), (2), (3))), (3));
ASSERT_PP_EQ((PTL_SWITCH(0, (a), (b), (c), (d), (e), (f), (g), (h), (i), (j), (k), (l), (m), (n), (o), (p), (q), (r), (s), (t), (u), (v), (w), (x), (y), (z), (A), (B), (C), (D), (E), (F), (G), (H), (I), (J), (K), (L), (M), (N), (O), (P), (Q), (R), (S), (T), (U), (V), (W), (X), (Y), (Z), (ba), (bb), (bc), (bd), (be), (bf), (bg), (bh), (bi), (bj), (bk), (bl), (bm), (bn), (bo), (bp), (bq), (br), (bs), (bt), (bu), (bv), (bw), (bx), (by), (bz), (bA), (bB), (bC), (bD), (bE), (bF), (bG), (bH), (bI), (bJ), (bK), (bL), (bM), (bN), (bO), (bP), (bQ), (bR), (bS), (bT), (bU), (bV), (bW), (bX), (bY), (bZ), (ca), (cb), (cc), (cd), (ce), (cf), (cg), (ch), (ci), (cj), (ck), (cl), (cm), (cn), (co), (cp), (cq), (cr), (cs), (ct), (cu), (cv), (cw), (cx), (cy), (cz), (cA), (cB), (cC), (cD), (cE), (cF), (cG), (cH), (cI), (cJ), (cK), (cL), (cM), (cN), (cO), (cP), (cQ), (cR), (cS), (cT), (cU), (cV), (cW), (cX), (cY), (cZ), (da), (db), (dc), (dd), (de), (df), (dg), (dh), (di), (dj), (dk), (dl), (dm), (dn), (do), (dp), (dq), (dr), (ds), (dt), (du), (dv), (dw), (dx), (dy), (dz), (dA), (dB), (dC), (dD), (dE), (dF), (dG), (dH), (dI), (dJ), (dK), (dL), (dM), (dN), (dO), (dP), (dQ), (dR), (dS), (dT), (dU), (dV), (dW), (dX), (dY), (dZ), (ea), (eb), (ec), (ed), (ee), (ef), (eg), (eh), (ei), (ej), (ek), (el), (em), (en), (eo), (ep), (eq), (er), (es), (et), (eu), (ev), (ew), (ex), (ey), (ez), (eA), (eB), (eC), (eD), (eE), (eF), (eG), (eH), (eI), (eJ), (eK), (eL), (eM), (eN), (eO), (eP), (eQ), (eR), (eS), (eT), (eU), (eV), (eW), (eX), (eY), (eZ), (fa), (fb), (fc), (fd), (fe), (ff), (fg), (fh), (fi), (fj), (fk), (fl), (fm), (fn), (fo), (fp), (fq), (fr), (fs), (ft), (fu), (fv), (fw), (fx), (fy), (fz), (fA), (fB), (fC), (fD), (fE), (fF), (fG), (fH), (fI), (fJ), (fK), (fL), (fM), (fN), (fO), (fP), (fQ), (fR), (fS), (fT), (fU), (fV), (fW), (fX), (fY), (fZ), (ga), (gb), (gc), (gd), (ge), (gf), (gg), (gh), (gi), (gj), (gk), (gl), (gm), (gn), (go), (gp), (gq), (gr), (gs), (gt), (gu), (gv), (gw), (gx), (gy), (gz), (gA), (gB), (gC), (gD), (gE), (gF), (gG), (gH), (gI), (gJ), (gK), (gL), (gM), (gN), (gO), (gP), (gQ), (gR), (gS), (gT), (gU), (gV), (gW), (gX), (gY), (gZ), (ha), (hb), (hc), (hd), (he), (hf), (hg), (hh), (hi), (hj), (hk), (hl), (hm), (hn), (ho), (hp), (hq), (hr), (hs), (ht), (hu), (hv), (hw), (hx), (hy), (hz), (hA), (hB), (hC), (hD), (hE), (hF), (hG), (hH), (hI), (hJ), (hK), (hL), (hM), (hN), (hO), (hP), (hQ), (hR), (hS), (hT), (hU), (hV), (hW), (hX), (hY), (hZ), (ia), (ib), (ic), (id), (ie), (if), (ig), (ih), (ii), (ij), (ik), (il), (im), (in), (io), (ip), (iq), (ir), (is), (it), (iu), (iv), (iw), (ix), (iy), (iz), (iA), (iB), (iC), (iD), (iE), (iF), (iG), (iH), (iI), (iJ), (iK), (iL), (iM), (iN), (iO), (iP), (iQ), (iR), (iS), (iT), (iU), (iV), (iW), (iX), (iY), (iZ), (ja), (jb), (jc), (jd), (je), (jf), (jg), (jh), (ji), (jj), (jk), (jl), (jm), (jn), (jo), (jp), (jq), (jr), (js), (jt), (ju), (jv), (jw), (jx), (jy), (jz), (jA), (jB), (jC), (jD), (jE), (jF), (jG), (jH), (jI), (jJ), (jK), (jL), (jM), (jN), (jO), (jP), (jQ), (jR), (jS), (jT), (jU), (jV), (jW), (jX), (jY), (jZ), (ka), (kb), (kc), (kd), (ke), (kf), (kg), (kh), (ki), (kj), (kk), (kl), (km), (kn), (ko), (kp), (kq), (kr), (ks), (kt), (ku), (kv), (kw), (kx), (ky), (kz), (kA), (kB), (kC), (kD), (kE), (kF), (kG), (kH), (kI), (kJ), (kK), (kL), (kM), (kN), (kO), (kP), (kQ), (kR), (kS), (kT), (kU), (kV), (kW), (kX), (kY), (kZ), (la), (lb), (lc), (ld), (le), (lf), (lg), (lh), (li), (lj), (lk), (ll), (lm), (ln), (lo), (lp), (lq), (lr), (ls), (lt), (lu), (lv), (lw), (lx), (ly), (lz), (lA), (lB), (lC), (lD), (lE), (lF), (lG), (lH), (lI), (lJ), (lK), (lL), (lM), (lN), (lO), (lP), (lQ), (lR), (lS), (lT), (lU), (lV), (lW), (lX), (lY), (lZ), (ma), (mb), (mc), (md), (me), (mf), (mg), (mh), (mi), (mj), (mk), (ml), (mm), (mn), (mo), (mp), (mq), (mr), (ms), (mt), (mu), (mv), (mw), (mx), (my), (mz), (mA), (mB), (mC), (mD), (mE), (mF), (mG), (mH), (mI), (mJ), (mK), (mL), (mM), (mN), (mO), (mP), (mQ), (mR), (mS), (mT), (mU), (mV), (mW), (mX), (mY), (mZ), (na), (nb), (nc), (nd), (ne), (nf), (ng), (nh), (ni), (nj), (nk), (nl), (nm), (nn), (no), (np), (nq), (nr), (ns), (nt), (nu), (nv), (nw), (nx), (ny), (nz), (nA), (nB), (nC), (nD), (nE), (nF), (nG), (nH), (nI), (nJ), (nK), (nL), (nM), (nN), (nO), (nP), (nQ), (nR), (nS), (nT), (nU), (nV), (nW), (nX), (nY), (nZ), (oa), (ob), (oc), (od), (oe), (of), (og), (oh), (oi), (oj), (ok), (ol), (om), (on), (oo), (op), (oq), (or), (os), (ot), (ou), (ov), (ow), (ox), (oy), (oz), (oA), (oB), (oC), (oD), (oE), (oF), (oG), (oH), (oI), (oJ), (oK), (oL), (oM), (oN), (oO), (oP), (oQ), (oR), (oS), (oT), (oU), (oV), (oW), (oX), (oY), (oZ), (pa), (pb), (pc), (pd), (pe), (pf), (pg), (ph), (pi), (pj), (pk), (pl), (pm), (pn), (po), (pp), (pq), (pr), (ps), (pt), (pu), (pv), (pw), (px), (py), (pz), (pA), (pB), (pC), (pD), (pE), (pF), (pG), (pH), (pI), (pJ), (pK), (pL), (pM), (pN), (pO), (pP), (pQ), (pR), (pS), (pT), (pU), (pV), (pW), (pX), (pY), (pZ), (qa), (qb), (qc), (qd), (qe), (qf), (qg), (qh), (qi), (qj), (qk), (ql), (qm), (qn), (qo), (qp), (qq), (qr), (qs), (qt), (qu), (qv), (qw), (qx), (qy), (qz), (qA), (qB), (qC), (qD), (qE), (qF), (qG), (qH), (qI), (qJ), (qK), (qL), (qM), (qN), (qO), (qP), (qQ), (qR), (qS), (qT), (qU), (qV), (qW), (qX), (qY), (qZ), (ra), (rb), (rc), (rd), (re), (rf), (rg), (rh), (ri), (rj), (rk), (rl), (rm), (rn), (ro), (rp), (rq), (rr), (rs), (rt), (ru), (rv), (rw), (rx), (ry), (rz), (rA), (rB), (rC), (rD), (rE), (rF), (rG), (rH), (rI), (rJ), (rK), (rL), (rM), (rN), (rO), (rP), (rQ), (rR), (rS), (rT), (rU), (rV), (rW), (rX), (rY), (rZ), (sa), (sb), (sc), (sd), (se), (sf), (sg), (sh), (si), (sj), (sk), (sl), (sm), (sn), (so), (sp), (sq), (sr), (ss), (st), (su), (sv), (sw), (sx), (sy), (sz), (sA), (sB), (sC), (sD), (sE), (sF), (sG), (sH), (sI), (sJ), (sK), (sL), (sM), (sN), (sO), (sP), (sQ), (sR), (sS), (sT), (sU), (sV), (sW), (sX), (sY), (sZ), (ta), (tb), (tc), (td), (te), (tf), (tg), (th), (ti), (tj), (tk), (tl), (tm), (tn), (to), (tp), (tq), (tr), (ts), (tt), (tu), (tv), (tw), (tx), (ty), (tz), (tA), (tB), (tC), (tD), (tE), (tF), (tG), (tH), (tI), (tJ))), (a));
ASSERT_PP_EQ((PTL_SWITCH(1, (a), (b), (c), (d), (e), (f), (g), (h), (i), (j), (k), (l), (m), (n), (o), (p), (q), (r), (s), (t), (u), (v), (w), (x), (y), (z), (A), (B), (C), (D), (E), (F), (G), (H), (I), (J), (K), (L), (M), (N), (O), (P), (Q), (R), (S), (T), (U), (V), (W), (X), (Y), (Z), (ba), (bb), (bc), (bd), (be), (bf), (bg), (bh), (bi), (bj), (bk), (bl), (bm), (bn), (bo), (bp), (bq), (br), (bs), (bt), (bu), (bv), (bw), (bx), (by), (bz), (bA), (bB), (bC), (bD), (bE), (bF), (bG), (bH), (bI), (bJ), (bK), (bL), (bM), (bN), (bO), (bP), (bQ), (bR), (bS), (bT), (bU), (bV), (bW), (bX), (bY), (bZ), (ca), (cb), (cc), (cd), (ce), (cf), (cg), (ch), (ci), (cj), (ck), (cl), (cm), (cn), (co), (cp), (cq), (cr), (cs), (ct), (cu), (cv), (cw), (cx), (cy), (cz), (cA), (cB), (cC), (cD), (cE), (cF), (cG), (cH), (cI), (cJ), (cK), (cL), (cM), (cN), (cO), (cP), (cQ), (cR), (cS), (cT), (cU), (cV), (cW), (cX), (cY), (cZ), (da), (db), (dc), (dd), (de), (df), (dg), (dh), (di), (dj), (dk), (dl), (dm), (dn), (do), (dp), (dq), (dr), (ds), (dt), (du), (dv), (dw), (dx), (dy), (dz), (dA), (dB), (dC), (dD), (dE), (dF), (dG), (dH), (dI), (dJ), (dK), (dL), (dM), (dN), (dO), (dP), (dQ), (dR), (dS), (dT), (dU), (dV), (dW), (dX), (dY), (dZ), (ea), (eb), (ec), (ed), (ee), (ef), (eg), (eh), (ei), (ej), (ek), (el), (em), (en), (eo), (ep), (eq), (er), (es), (et), (eu), (ev), (ew), (ex), (ey), (ez), (eA), (eB), (eC), (eD), (eE), (eF), (eG), (eH), (eI), (eJ), (eK), (eL), (eM), (eN), (eO), (eP), (eQ), (eR), (eS), (eT), (eU), (eV), (eW), (eX), (eY), (eZ), (fa), (fb), (fc), (fd), (fe), (ff), (fg), (fh), (fi), (fj), (fk), (fl), (fm), (fn), (fo), (fp), (fq), (fr), (fs), (ft), (fu), (fv), (fw), (fx), (fy), (fz), (fA), (fB), (fC), (fD), (fE), (fF), (fG), (fH), (fI), (fJ), (fK), (fL), (fM), (fN), (fO), (fP), (fQ), (fR), (fS), (fT), (fU), (fV), (fW), (fX), (fY), (fZ), (ga), (gb), (gc), (gd), (ge), (gf), (gg), (gh), (gi), (gj), (gk), (gl), (gm), (gn), (go), (gp), (gq), (gr), (gs), (gt), (gu), (gv), (gw), (gx), (gy), (gz), (gA), (gB), (gC), (gD), (gE), (gF), (gG), (gH), (gI), (gJ), (gK), (gL), (gM), (gN), (gO), (gP), (gQ), (gR), (gS), (gT), (gU), (gV), (gW), (gX), (gY), (gZ), (ha), (hb), (hc), (hd), (he), (hf), (hg), (hh), (hi), (hj), (hk), (hl), (hm), (hn), (ho), (hp), (hq), (hr), (hs), (ht), (hu), (hv), (hw), (hx), (hy), (hz), (hA), (hB), (hC), (hD), (hE), (hF), (hG), (hH), (hI), (hJ), (hK), (hL), (hM), (hN), (hO), (hP), (hQ), (hR), (hS), (hT), (hU), (hV), (hW), (hX), (hY), (hZ), (ia), (ib), (ic), (id), (ie), (if), (ig), (ih), (ii), (ij), (ik), (il), (im), (in), (io), (ip), (iq), (ir), (is), (it), (iu), (iv), (iw), (ix), (iy), (iz), (iA), (iB), (iC), (iD), (iE), (iF), (iG), (iH), (iI), (iJ), (iK), (iL), (iM), (iN), (iO), (iP), (iQ), (iR), (iS), (iT), (iU), (iV), (iW), (iX), (iY), (iZ), (ja), (jb), (jc), (jd), (je), (jf), (jg), (jh), (ji), (jj), (jk), (jl), (jm), (jn), (jo), (jp), (jq), (jr), (js), (jt), (ju), (jv), (jw), (jx), (jy), (jz), (jA), (jB), (jC), (jD), (jE), (jF), (jG), (jH), (jI), (jJ), (jK), (jL), (jM), (jN), (jO), (jP), (jQ), (jR), (jS), (jT), (jU), (jV), (jW), (jX), (jY), (jZ), (ka), (kb), (kc), (kd), (ke), (kf), (kg), (kh), (ki), (kj), (kk), (kl), (km), (kn), (ko), (kp), (kq), (kr), (ks), (kt), (ku), (kv), (kw), (kx), (ky), (kz), (kA), (kB), (kC), (kD), (kE), (kF), (kG), (kH), (kI), (kJ), (kK), (kL), (kM), (kN), (kO), (kP), (kQ), (kR), (kS), (kT), (kU), (kV), (kW), (kX), (kY), (kZ), (la), (lb), (lc), (ld), (le), (lf), (lg), (lh), (li), (lj), (lk), (ll), (lm), (ln), (lo), (lp), (lq), (lr), (ls), (lt), (lu), (lv), (lw), (lx), (ly), (lz), (lA), (lB), (lC), (lD), (lE), (lF), (lG), (lH), (lI), (lJ), (lK), (lL), (lM), (lN), (lO), (lP), (lQ), (lR), (lS), (lT), (lU), (lV), (lW), (lX), (lY), (lZ), (ma), (mb), (mc), (md), (me), (mf), (mg), (mh), (mi), (mj), (mk), (ml), (mm), (mn), (mo), (mp), (mq), (mr), (ms), (mt), (mu), (mv), (mw), (mx), (my), (mz), (mA), (mB), (mC), (mD), (mE), (mF), (mG), (mH), (mI), (mJ), (mK), (mL), (mM), (mN), (mO), (mP), (mQ), (mR), (mS), (mT), (mU), (mV), (mW), (mX), (mY), (mZ), (na), (nb), (nc), (nd), (ne), (nf), (ng), (nh), (ni), (nj), (nk), (nl), (nm), (nn), (no), (np), (nq), (nr), (ns), (nt), (nu), (nv), (nw), (nx), (ny), (nz), (nA), (nB), (nC), (nD), (nE), (nF), (nG), (nH), (nI), (nJ), (nK), (nL), (nM), (nN), (nO), (nP), (nQ), (nR), (nS), (nT), (nU), (nV), (nW), (nX), (nY), (nZ), (oa), (ob), (oc), (od), (oe), (of), (og), (oh), (oi), (oj), (ok), (ol), (om), (on), (oo), (op), (oq), (or), (os), (ot), (ou), (ov), (ow), (ox), (oy), (oz), (oA), (oB), (oC), (oD), (oE), (oF), (oG), (oH), (oI), (oJ), (oK), (oL), (oM), (oN), (oO), (oP), (oQ), (oR), (oS), (oT), (oU), (oV), (oW), (oX), (oY), (oZ), (pa), (pb), (pc), (pd), (pe), (pf), (pg), (ph), (pi), (pj), (pk), (pl), (pm), (pn), (po), (pp), (pq), (pr), (ps), (pt), (pu), (pv), (pw), (px), (py), (pz), (pA), (pB), (pC), (pD), (pE), (pF), (pG), (pH), (pI), (pJ), (pK), (pL), (pM), (pN), (pO), (pP), (pQ), (pR), (pS), (pT), (pU), (pV), (pW), (pX), (pY), (pZ), (qa), (qb), (qc), (qd), (qe), (qf), (qg), (qh), (qi), (qj), (qk), (ql), (qm), (qn), (qo), (qp), (qq), (qr), (qs), (qt), (qu), (qv), (qw), (qx), (qy), (qz), (qA), (qB), (qC), (qD), (qE), (qF), (qG), (qH), (qI), (qJ), (qK), (qL), (qM), (qN), (qO), (qP), (qQ), (qR), (qS), (qT), (qU), (qV), (qW), (qX), (qY), (qZ), (ra), (rb), (rc), (rd), (re), (rf), (rg), (rh), (ri), (rj), (rk), (rl), (rm), (rn), (ro), (rp), (rq), (rr), (rs), (rt), (ru), (rv), (rw), (rx), (ry), (rz), (rA), (rB), (rC), (rD), (rE), (rF), (rG), (rH), (rI), (rJ), (rK), (rL), (rM), (rN), (rO), (rP), (rQ), (rR), (rS), (rT), (rU), (rV), (rW), (rX), (rY), (rZ), (sa), (sb), (sc), (sd), (se), (sf), (sg), (sh), (si), (sj), (sk), (sl), (sm), (sn), (so), (sp), (sq), (sr), (ss), (st), (su), (sv), (sw), (sx), (sy), (sz), (sA), (sB), (sC), (sD), (sE), (sF), (sG), (sH), (sI), (sJ), (sK), (sL), (sM), (sN), (sO), (sP), (sQ), (sR), (sS), (sT), (sU), (sV), (sW), (sX), (sY), (sZ), (ta), (tb), (tc), (td), (te), (tf), (tg), (th), (ti), (tj), (tk), (tl), (tm), (tn), (to), (tp), (tq), (tr), (ts), (tt), (tu), (tv), (tw), (tx), (ty), (tz), (tA), (tB), (tC), (tD), (tE), (tF), (tG), (tH), (tI), (tJ))), (b));
ASSERT_PP_EQ((PTL_SWITCH(1023, (a), (b), (c), (d), (e), (f), (g), (h), (i), (j), (k), (l), (m), (n), (o), (p), (q), (r), (s), (t), (u), (v), (w), (x), (y), (z), (A), (B), (C), (D), (E), (F), (G), (H), (I), (J), (K), (L), (M), (N), (O), (P), (Q), (R), (S), (T), (U), (V), (W), (X), (Y), (Z), (ba), (bb), (bc), (bd), (be), (bf), (bg), (bh), (bi), (bj), (bk), (bl), (bm), (bn), (bo), (bp), (bq), (br), (bs), (bt), (bu), (bv), (bw), (bx), (by), (bz), (bA), (bB), (bC), (bD), (bE), (bF), (bG), (bH), (bI), (bJ), (bK), (bL), (bM), (bN), (bO), (bP), (bQ), (bR), (bS), (bT), (bU), (bV), (bW), (bX), (bY), (bZ), (ca), (cb), (cc), (cd), (ce), (cf), (cg), (ch), (ci), (cj), (ck), (cl), (cm), (cn), (co), (cp), (cq), (cr), (cs), (ct), (cu), (cv), (cw), (cx), (cy), (cz), (cA), (cB), (cC), (cD), (cE), (cF), (cG), (cH), (cI), (cJ), (cK), (cL), (cM), (cN), (cO), (cP), (cQ), (cR), (cS), (cT), (cU), (cV), (cW), (cX), (cY), (cZ), (da), (db), (dc), (dd), (de), (df), (dg), (dh), (di), (dj), (dk), (dl), (dm), (dn), (do), (dp), (dq), (dr), (ds), (dt), (du), (dv), (dw), (dx), (dy), (dz), (dA), (dB), (dC), (dD), (dE), (dF), (dG), (dH), (dI), (dJ), (dK), (dL), (dM), (dN), (dO), (dP), (dQ), (dR), (dS), (dT), (dU), (dV), (dW), (dX), (dY), (dZ), (ea), (eb), (ec), (ed), (ee), (ef), (eg), (eh), (ei), (ej), (ek), (el), (em), (en), (eo), (ep), (eq), (er), (es), (et), (eu), (ev), (ew), (ex), (ey), (ez), (eA), (eB), (eC), (eD), (eE), (eF), (eG), (eH), (eI), (eJ), (eK), (eL), (eM), (eN), (eO), (eP), (eQ), (eR), (eS), (eT), (eU), (eV), (eW), (eX), (eY), (eZ), (fa), (fb), (fc), (fd), (fe), (ff), (fg), (fh), (fi), (fj), (fk), (fl), (fm), (fn), (fo), (fp), (fq), (fr), (fs), (ft), (fu), (fv), (fw), (fx), (fy), (fz), (fA), (fB), (fC), (fD), (fE), (fF), (fG), (fH), (fI), (fJ), (fK), (fL), (fM), (fN), (fO), (fP), (fQ), (fR), (fS), (fT), (fU), (fV), (fW), (fX), (fY), (fZ), (ga), (gb), (gc), (gd), (ge), (gf), (gg), (gh), (gi), (gj), (gk), (gl), (gm), (gn), (go), (gp), (gq), (gr), (gs), (gt), (gu), (gv), (gw), (gx), (gy), (gz), (gA), (gB), (gC), (gD), (gE), (gF), (gG), (gH), (gI), (gJ), (gK), (gL), (gM), (gN), (gO), (gP), (gQ), (gR), (gS), (gT), (gU), (gV), (gW), (gX), (gY), (gZ), (ha), (hb), (hc), (hd), (he), (hf), (hg), (hh), (hi), (hj), (hk), (hl), (hm), (hn), (ho), (hp), (hq), (hr), (hs), (ht), (hu), (hv), (hw), (hx), (hy), (hz), (hA), (hB), (hC), (hD), (hE), (hF), (hG), (hH), (hI), (hJ), (hK), (hL), (hM), (hN), (hO), (hP), (hQ), (hR), (hS), (hT), (hU), (hV), (hW), (hX), (hY), (hZ), (ia), (ib), (ic), (id), (ie), (if), (ig), (ih), (ii), (ij), (ik), (il), (im), (in), (io), (ip), (iq), (ir), (is), (it), (iu), (iv), (iw), (ix), (iy), (iz), (iA), (iB), (iC), (iD), (iE), (iF), (iG), (iH), (iI), (iJ), (iK), (iL), (iM), (iN), (iO), (iP), (iQ), (iR), (iS), (iT), (iU), (iV), (iW), (iX), (iY), (iZ), (ja), (jb), (jc), (jd), (je), (jf), (jg), (jh), (ji), (jj), (jk), (jl), (jm), (jn), (jo), (jp), (jq), (jr), (js), (jt), (ju), (jv), (jw), (jx), (jy), (jz), (jA), (jB), (jC), (jD), (jE), (jF), (jG), (jH), (jI), (jJ), (jK), (jL), (jM), (jN), (jO), (jP), (jQ), (jR), (jS), (jT), (jU), (jV), (jW), (jX), (jY), (jZ), (ka), (kb), (kc), (kd), (ke), (kf), (kg), (kh), (ki), (kj), (kk), (kl), (km), (kn), (ko), (kp), (kq), (kr), (ks), (kt), (ku), (kv), (kw), (kx), (ky), (kz), (kA), (kB), (kC), (kD), (kE), (kF), (kG), (kH), (kI), (kJ), (kK), (kL), (kM), (kN), (kO), (kP), (kQ), (kR), (kS), (kT), (kU), (kV), (kW), (kX), (kY), (kZ), (la), (lb), (lc), (ld), (le), (lf), (lg), (lh), (li), (lj), (lk), (ll), (lm), (ln), (lo), (lp), (lq), (lr), (ls), (lt), (lu), (lv), (lw), (lx), (ly), (lz), (lA), (lB), (lC), (lD), (lE), (lF), (lG), (lH), (lI), (lJ), (lK), (lL), (lM), (lN), (lO), (lP), (lQ), (lR), (lS), (lT), (lU), (lV), (lW), (lX), (lY), (lZ), (ma), (mb), (mc), (md), (me), (mf), (mg), (mh), (mi), (mj), (mk), (ml), (mm), (mn), (mo), (mp), (mq), (mr), (ms), (mt), (mu), (mv), (mw), (mx), (my), (mz), (mA), (mB), (mC), (mD), (mE), (mF), (mG), (mH), (mI), (mJ), (mK), (mL), (mM), (mN), (mO), (mP), (mQ), (mR), (mS), (mT), (mU), (mV), (mW), (mX), (mY), (mZ), (na), (nb), (nc), (nd), (ne), (nf), (ng), (nh), (ni), (nj), (nk), (nl), (nm), (nn), (no), (np), (nq), (nr), (ns), (nt), (nu), (nv), (nw), (nx), (ny), (nz), (nA), (nB), (nC), (nD), (nE), (nF), (nG), (nH), (nI), (nJ), (nK), (nL), (nM), (nN), (nO), (nP), (nQ), (nR), (nS), (nT), (nU), (nV), (nW), (nX), (nY), (nZ), (oa), (ob), (oc), (od), (oe), (of), (og), (oh), (oi), (oj), (ok), (ol), (om), (on), (oo), (op), (oq), (or), (os), (ot), (ou), (ov), (ow), (ox), (oy), (oz), (oA), (oB), (oC), (oD), (oE), (oF), (oG), (oH), (oI), (oJ), (oK), (oL), (oM), (oN), (oO), (oP), (oQ), (oR), (oS), (oT), (oU), (oV), (oW), (oX), (oY), (oZ), (pa), (pb), (pc), (pd), (pe), (pf), (pg), (ph), (pi), (pj), (pk), (pl), (pm), (pn), (po), (pp), (pq), (pr), (ps), (pt), (pu), (pv), (pw), (px), (py), (pz), (pA), (pB), (pC), (pD), (pE), (pF), (pG), (pH), (pI), (pJ), (pK), (pL), (pM), (pN), (pO), (pP), (pQ), (pR), (pS), (pT), (pU), (pV), (pW), (pX), (pY), (pZ), (qa), (qb), (qc), (qd), (qe), (qf), (qg), (qh), (qi), (qj), (qk), (ql), (qm), (qn), (qo), (qp), (qq), (qr), (qs), (qt), (qu), (qv), (qw), (qx), (qy), (qz), (qA), (qB), (qC), (qD), (qE), (qF), (qG), (qH), (qI), (qJ), (qK), (qL), (qM), (qN), (qO), (qP), (qQ), (qR), (qS), (qT), (qU), (qV), (qW), (qX), (qY), (qZ), (ra), (rb), (rc), (rd), (re), (rf), (rg), (rh), (ri), (rj), (rk), (rl), (rm), (rn), (ro), (rp), (rq), (rr), (rs), (rt), (ru), (rv), (rw), (rx), (ry), (rz), (rA), (rB), (rC), (rD), (rE), (rF), (rG), (rH), (rI), (rJ), (rK), (rL), (rM), (rN), (rO), (rP), (rQ), (rR), (rS), (rT), (rU), (rV), (rW), (rX), (rY), (rZ), (sa), (sb), (sc), (sd), (se), (sf), (sg), (sh), (si), (sj), (sk), (sl), (sm), (sn), (so), (sp), (sq), (sr), (ss), (st), (su), (sv), (sw), (sx), (sy), (sz), (sA), (sB), (sC), (sD), (sE), (sF), (sG), (sH), (sI), (sJ), (sK), (sL), (sM), (sN), (sO), (sP), (sQ), (sR), (sS), (sT), (sU), (sV), (sW), (sX), (sY), (sZ), (ta), (tb), (tc), (td), (te), (tf), (tg), (th), (ti), (tj), (tk), (tl), (tm), (tn), (to), (tp), (tq), (tr), (ts), (tt), (tu), (tv), (tw), (tx), (ty), (tz), (tA), (tB), (tC), (tD), (tE), (tF), (tG), (tH), (tI), (tJ))), (tJ));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(0, (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(1, (), (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(2, (), (), (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(3, (), (), (), (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(4, (), (), (), (), (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(5, (), (), (), (), (), (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(6, (), (), (), (), (), (), (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(7, (), (), (), (), (), (), (), (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,,,,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(8, (), (), (), (), (), (), (), (), (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,,,,,,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(9, (), (), (), (), (), (), (), (), (), (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,,,,,,,,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(10, (), (), (), (), (), (), (), (), (), (), (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,,,,,,,,,,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(11, (), (), (), (), (), (), (), (), (), (), (), (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,,,,,,,,,,,,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(12, (), (), (), (), (), (), (), (), (), (), (), (), (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,,,,,,,,,,,,,,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(13, (), (), (), (), (), (), (), (), (), (), (), (), (), (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(14, (), (), (), (), (), (), (), (), (), (), (), (), (), (), (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,, )"));

ASSERT_PP_EQ((PTL_NOT(0)), (1));
ASSERT_PP_EQ((PTL_NOT(1)), (0));

ASSERT_PP_EQ((PTL_AND(0, 0)), (0));
ASSERT_PP_EQ((PTL_AND(0, 1)), (0));
ASSERT_PP_EQ((PTL_AND(1, 0)), (0));
ASSERT_PP_EQ((PTL_AND(1, 1)), (1));

ASSERT_PP_EQ((PTL_OR(0, 0)), (0));
ASSERT_PP_EQ((PTL_OR(0, 1)), (1));
ASSERT_PP_EQ((PTL_OR(1, 0)), (1));
ASSERT_PP_EQ((PTL_OR(1, 1)), (1));

ASSERT_PP_EQ((PTL_NAND(0, 0)), (1));
ASSERT_PP_EQ((PTL_NAND(0, 1)), (1));
ASSERT_PP_EQ((PTL_NAND(1, 0)), (1));
ASSERT_PP_EQ((PTL_NAND(1, 1)), (0));

ASSERT_PP_EQ((PTL_NOR(0, 0)), (1));
ASSERT_PP_EQ((PTL_NOR(0, 1)), (0));
ASSERT_PP_EQ((PTL_NOR(1, 0)), (0));
ASSERT_PP_EQ((PTL_NOR(1, 1)), (0));

ASSERT_PP_EQ((PTL_XOR(0, 0)), (0));
ASSERT_PP_EQ((PTL_XOR(0, 1)), (1));
ASSERT_PP_EQ((PTL_XOR(1, 0)), (1));
ASSERT_PP_EQ((PTL_XOR(1, 1)), (0));

ASSERT_PP_EQ((PTL_XNOR(0, 0)), (1));
ASSERT_PP_EQ((PTL_XNOR(0, 1)), (0));
ASSERT_PP_EQ((PTL_XNOR(1, 0)), (0));
ASSERT_PP_EQ((PTL_XNOR(1, 1)), (1));

ASSERT_PP_EQ((PTL_ADD(0, 0)), (0));
ASSERT_PP_EQ((PTL_ADD(0, 1)), (1));
ASSERT_PP_EQ((PTL_ADD(1, 2)), (3));
ASSERT_PP_EQ((PTL_ADD(1023, 1)), (0));
ASSERT_PP_EQ((PTL_ADD(1023, 2)), (1));

ASSERT_PP_EQ((PTL_SUB(0, 0)), (0));
ASSERT_PP_EQ((PTL_SUB(0, 1)), (1023));
ASSERT_PP_EQ((PTL_SUB(1, 0)), (1));
ASSERT_PP_EQ((PTL_SUB(1, 1)), (0));
ASSERT_PP_EQ((PTL_SUB(3, 1)), (2));
ASSERT_PP_EQ((PTL_SUB(1, 3)), (1022));
ASSERT_PP_EQ((PTL_SUB(0, 1023)), (1));
ASSERT_PP_EQ((PTL_SUB(1023, 0)), (1023));
ASSERT_PP_EQ((PTL_SUB(1023, 1)), (1022));

ASSERT_PP_EQ((PTL_PASTE()), ());
ASSERT_PP_EQ((PTL_PASTE(a)), (a));
ASSERT_PP_EQ((PTL_PASTE(a, b)), (a b));
ASSERT_PP_EQ((PTL_PASTE(a, b, c)), (a b c));
ASSERT_PP_EQ((PTL_PASTE(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, ba, bb, bc, bd, be, bf, bg, bh, bi, bj, bk, bl, bm, bn, bo, bp, bq, br, bs, bt, bu, bv, bw, bx, by, bz, bA, bB, bC, bD, bE, bF, bG, bH, bI, bJ, bK, bL, bM, bN, bO, bP, bQ, bR, bS, bT, bU, bV, bW, bX, bY, bZ, ca, cb, cc, cd, ce, cf, cg, ch, ci, cj, ck, cl, cm, cn, co, cp, cq, cr, cs, ct, cu, cv, cw, cx, cy, cz, cA, cB, cC, cD, cE, cF, cG, cH, cI, cJ, cK, cL, cM, cN, cO, cP, cQ, cR, cS, cT, cU, cV, cW, cX, cY, cZ, da, db, dc, dd, de, df, dg, dh, di, dj, dk, dl, dm, dn, do, dp, dq, dr, ds, dt, du, dv, dw, dx, dy, dz, dA, dB, dC, dD, dE, dF, dG, dH, dI, dJ, dK, dL, dM, dN, dO, dP, dQ, dR, dS, dT, dU, dV, dW, dX, dY, dZ, ea, eb, ec, ed, ee, ef, eg, eh, ei, ej, ek, el, em, en, eo, ep, eq, er, es, et, eu, ev, ew, ex, ey, ez, eA, eB, eC, eD, eE, eF, eG, eH, eI, eJ, eK, eL, eM, eN, eO, eP, eQ, eR, eS, eT, eU, eV, eW, eX, eY, eZ, fa, fb, fc, fd, fe, ff, fg, fh, fi, fj, fk, fl, fm, fn, fo, fp, fq, fr, fs, ft, fu, fv, fw, fx, fy, fz, fA, fB, fC, fD, fE, fF, fG, fH, fI, fJ, fK, fL, fM, fN, fO, fP, fQ, fR, fS, fT, fU, fV, fW, fX, fY, fZ, ga, gb, gc, gd, ge, gf, gg, gh, gi, gj, gk, gl, gm, gn, go, gp, gq, gr, gs, gt, gu, gv, gw, gx, gy, gz, gA, gB, gC, gD, gE, gF, gG, gH, gI, gJ, gK, gL, gM, gN, gO, gP, gQ, gR, gS, gT, gU, gV, gW, gX, gY, gZ, ha, hb, hc, hd, he, hf, hg, hh, hi, hj, hk, hl, hm, hn, ho, hp, hq, hr, hs, ht, hu, hv, hw, hx, hy, hz, hA, hB, hC, hD, hE, hF, hG, hH, hI, hJ, hK, hL, hM, hN, hO, hP, hQ, hR, hS, hT, hU, hV, hW, hX, hY, hZ, ia, ib, ic, id, ie, if, ig, ih, ii, ij, ik, il, im, in, io, ip, iq, ir, is, it, iu, iv, iw, ix, iy, iz, iA, iB, iC, iD, iE, iF, iG, iH, iI, iJ, iK, iL, iM, iN, iO, iP, iQ, iR, iS, iT, iU, iV, iW, iX, iY, iZ, ja, jb, jc, jd, je, jf, jg, jh, ji, jj, jk, jl, jm, jn, jo, jp, jq, jr, js, jt, ju, jv, jw, jx, jy, jz, jA, jB, jC, jD, jE, jF, jG, jH, jI, jJ, jK, jL, jM, jN, jO, jP, jQ, jR, jS, jT, jU, jV, jW, jX, jY, jZ, ka, kb, kc, kd, ke, kf, kg, kh, ki, kj, kk, kl, km, kn, ko, kp, kq, kr, ks, kt, ku, kv, kw, kx, ky, kz, kA, kB, kC, kD, kE, kF, kG, kH, kI, kJ, kK, kL, kM, kN, kO, kP, kQ, kR, kS, kT, kU, kV, kW, kX, kY, kZ, la, lb, lc, ld, le, lf, lg, lh, li, lj, lk, ll, lm, ln, lo, lp, lq, lr, ls, lt, lu, lv, lw, lx, ly, lz, lA, lB, lC, lD, lE, lF, lG, lH, lI, lJ, lK, lL, lM, lN, lO, lP, lQ, lR, lS, lT, lU, lV, lW, lX, lY, lZ, ma, mb, mc, md, me, mf, mg, mh, mi, mj, mk, ml, mm, mn, mo, mp, mq, mr, ms, mt, mu, mv, mw, mx, my, mz, mA, mB, mC, mD, mE, mF, mG, mH, mI, mJ, mK, mL, mM, mN, mO, mP, mQ, mR, mS, mT, mU, mV, mW, mX, mY, mZ, na, nb, nc, nd, ne, nf, ng, nh, ni, nj, nk, nl, nm, nn, no, np, nq, nr, ns, nt, nu, nv, nw, nx, ny, nz, nA, nB, nC, nD, nE, nF, nG, nH, nI, nJ, nK, nL, nM, nN, nO, nP, nQ, nR, nS, nT, nU, nV, nW, nX, nY, nZ, oa, ob, oc, od, oe, of, og, oh, oi, oj, ok, ol, om, on, oo, op, oq, or, os, ot, ou, ov, ow, ox, oy, oz, oA, oB, oC, oD, oE, oF, oG, oH, oI, oJ, oK, oL, oM, oN, oO, oP, oQ, oR, oS, oT, oU, oV, oW, oX, oY, oZ, pa, pb, pc, pd, pe, pf, pg, ph, pi, pj, pk, pl, pm, pn, po, pp, pq, pr, ps, pt, pu, pv, pw, px, py, pz, pA, pB, pC, pD, pE, pF, pG, pH, pI, pJ, pK, pL, pM, pN, pO, pP, pQ, pR, pS, pT, pU, pV, pW, pX, pY, pZ, qa, qb, qc, qd, qe, qf, qg, qh, qi, qj, qk, ql, qm, qn, qo, qp, qq, qr, qs, qt, qu, qv, qw, qx, qy, qz, qA, qB, qC, qD, qE, qF, qG, qH, qI, qJ, qK, qL, qM, qN, qO, qP, qQ, qR, qS, qT, qU, qV, qW, qX, qY, qZ, ra, rb, rc, rd, re, rf, rg, rh, ri, rj, rk, rl, rm, rn, ro, rp, rq, rr, rs, rt, ru, rv, rw, rx, ry, rz, rA, rB, rC, rD, rE, rF, rG, rH, rI, rJ, rK, rL, rM, rN, rO, rP, rQ, rR, rS, rT, rU, rV, rW, rX, rY, rZ, sa, sb, sc, sd, se, sf, sg, sh, si, sj, sk, sl, sm, sn, so, sp, sq, sr, ss, st, su, sv, sw, sx, sy, sz, sA, sB, sC, sD, sE, sF, sG, sH, sI, sJ, sK, sL, sM, sN, sO, sP, sQ, sR, sS, sT, sU, sV, sW, sX, sY, sZ, ta, tb, tc, td, te, tf, tg, th, ti, tj, tk, tl, tm, tn, to, tp, tq, tr, ts, tt, tu, tv, tw, tx, ty, tz, tA, tB, tC, tD, tE, tF, tG, tH, tI)), (a b c d e f g h i j k l m n o p q r s t u v w x y z A B C D E F G H I J K L M N O P Q R S T U V W X Y Z ba bb bc bd be bf bg bh bi bj bk bl bm bn bo bp bq br bs bt bu bv bw bx by bz bA bB bC bD bE bF bG bH bI bJ bK bL bM bN bO bP bQ bR bS bT bU bV bW bX bY bZ ca cb cc cd ce cf cg ch ci cj ck cl cm cn co cp cq cr cs ct cu cv cw cx cy cz cA cB cC cD cE cF cG cH cI cJ cK cL cM cN cO cP cQ cR cS cT cU cV cW cX cY cZ da db dc dd de df dg dh di dj dk dl dm dn do dp dq dr ds dt du dv dw dx dy dz dA dB dC dD dE dF dG dH dI dJ dK dL dM dN dO dP dQ dR dS dT dU dV dW dX dY dZ ea eb ec ed ee ef eg eh ei ej ek el em en eo ep eq er es et eu ev ew ex ey ez eA eB eC eD eE eF eG eH eI eJ eK eL eM eN eO eP eQ eR eS eT eU eV eW eX eY eZ fa fb fc fd fe ff fg fh fi fj fk fl fm fn fo fp fq fr fs ft fu fv fw fx fy fz fA fB fC fD fE fF fG fH fI fJ fK fL fM fN fO fP fQ fR fS fT fU fV fW fX fY fZ ga gb gc gd ge gf gg gh gi gj gk gl gm gn go gp gq gr gs gt gu gv gw gx gy gz gA gB gC gD gE gF gG gH gI gJ gK gL gM gN gO gP gQ gR gS gT gU gV gW gX gY gZ ha hb hc hd he hf hg hh hi hj hk hl hm hn ho hp hq hr hs ht hu hv hw hx hy hz hA hB hC hD hE hF hG hH hI hJ hK hL hM hN hO hP hQ hR hS hT hU hV hW hX hY hZ ia ib ic id ie if ig ih ii ij ik il im in io ip iq ir is it iu iv iw ix iy iz iA iB iC iD iE iF iG iH iI iJ iK iL iM iN iO iP iQ iR iS iT iU iV iW iX iY iZ ja jb jc jd je jf jg jh ji jj jk jl jm jn jo jp jq jr js jt ju jv jw jx jy jz jA jB jC jD jE jF jG jH jI jJ jK jL jM jN jO jP jQ jR jS jT jU jV jW jX jY jZ ka kb kc kd ke kf kg kh ki kj kk kl km kn ko kp kq kr ks kt ku kv kw kx ky kz kA kB kC kD kE kF kG kH kI kJ kK kL kM kN kO kP kQ kR kS kT kU kV kW kX kY kZ la lb lc ld le lf lg lh li lj lk ll lm ln lo lp lq lr ls lt lu lv lw lx ly lz lA lB lC lD lE lF lG lH lI lJ lK lL lM lN lO lP lQ lR lS lT lU lV lW lX lY lZ ma mb mc md me mf mg mh mi mj mk ml mm mn mo mp mq mr ms mt mu mv mw mx my mz mA mB mC mD mE mF mG mH mI mJ mK mL mM mN mO mP mQ mR mS mT mU mV mW mX mY mZ na nb nc nd ne nf ng nh ni nj nk nl nm nn no np nq nr ns nt nu nv nw nx ny nz nA nB nC nD nE nF nG nH nI nJ nK nL nM nN nO nP nQ nR nS nT nU nV nW nX nY nZ oa ob oc od oe of og oh oi oj ok ol om on oo op oq or os ot ou ov ow ox oy oz oA oB oC oD oE oF oG oH oI oJ oK oL oM oN oO oP oQ oR oS oT oU oV oW oX oY oZ pa pb pc pd pe pf pg ph pi pj pk pl pm pn po pp pq pr ps pt pu pv pw px py pz pA pB pC pD pE pF pG pH pI pJ pK pL pM pN pO pP pQ pR pS pT pU pV pW pX pY pZ qa qb qc qd qe qf qg qh qi qj qk ql qm qn qo qp qq qr qs qt qu qv qw qx qy qz qA qB qC qD qE qF qG qH qI qJ qK qL qM qN qO qP qQ qR qS qT qU qV qW qX qY qZ ra rb rc rd re rf rg rh ri rj rk rl rm rn ro rp rq rr rs rt ru rv rw rx ry rz rA rB rC rD rE rF rG rH rI rJ rK rL rM rN rO rP rQ rR rS rT rU rV rW rX rY rZ sa sb sc sd se sf sg sh si sj sk sl sm sn so sp sq sr ss st su sv sw sx sy sz sA sB sC sD sE sF sG sH sI sJ sK sL sM sN sO sP sQ sR sS sT sU sV sW sX sY sZ ta tb tc td te tf tg th ti tj tk tl tm tn to tp tq tr ts tt tu tv tw tx ty tz tA tB tC tD tE tF tG tH tI));
// clang-format on

////////////////////////////////////////////////////////////////////////////////
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
//    for research purposes  and for various edge cases that still must be    //
//    solved using text replacement,  such as certain  optimizations  that    //
//    reduce the number of template specializations.                          //
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
//    pputl requires __VA_ARGS__ and __VA_OPT__ support (C++20) but has no    //
//    other dependencies;  it is a single-header library with no includes.    //
//    Any preprocessor that supports __VA_ARGS__ and __VA_OPT__  should be    //
//    able to run pputl.                                                      //
//                                                                            //
//    USAGE                                                                   //
//    -----                                                                   //
//    Copy pputl.h and include. The default build uses 8-bit unsigned ints    //
//    for  arithmetic  and  comparisons.  Integers  overflow and underflow    //
//    according to  standard unsigned rules.  Variadic argument sizes  are    //
//    usually capped by the uint max. Modify the head of codegen/codegen.h    //
//    and run `make` to set custom integer limits and naming preferences.     //
//                                                                            //
//    GUIDELINES                                                              //
//    ----------                                                              //
//                                                                            //
//    Non-nullary API functions are fully variadic and chainable such that    //
//    the outputs of one may be used as inputs to another.                    //
//                                                                            //
//    Tuples are used only when necessary;  most functions that operate on    //
//    data ranges both input and output a variadic argument list. Creating    //
//    a tuple is trivial compared to extracting tuple items.                  //
//                                                                            //
//    Functions that parse or transform generic data are designed to allow    //
//    for algorithmic manipulation of preprocessor syntax. This means that    //
//    expansions  are applied  consistently  to all input elements and are    //
//    known,  documented  and tested.  This is necessary for several pputl    //
//    features including algo.recur and algo.reduce that use deferred left    //
//    and right parens to construct recursive statements.                     //
//                                                                            //
//    API functions are documented with the following shorthand syntax:       //
//                                                                            //
//      name({in: type}) [-<xct>]-> {out[: type]}                             //
//                                                                            //
//      name: name of the feature                                             //
//      in:   any number of input parameter names                             //
//      type: a shorthand type description                                    //
//      xct:  number of secondary expansions applied to generic inputs.       //
//            any generic args deferred past xct will not be expanded.        //
//      out:  name of an output; may be an input                              //
//            name with a shorthand transformation.                           //
//                                                                            //
//    Generic types are documented with "any" and are the only targets for    //
//    expansion documentation. Input parameter separations, variadic sizes    //
//    and non-generic types (e.g. uint) must be fully expressed during the    //
//    primary expansion.                                                      //
//                                                                            //
//    Library example [range.split]:                                          //
//                                                                            //
//      split(n: uint, args: any...) -<1>-> head: (...args[:n]),              //
//                                          tail: (...args[n-size:]),         //
//                                          type: bool                        //
//                                                                            //
//    TESTING                                                                 //
//    -------                                                                 //
//    pputl  is statically tested by the build system.  Run `make test` to   ///
//    validate the library or open tests.cc in an LSP-enabled editor.       ////
//                                                                         /////
////////////////////////////////////////////////////////////////////////////////

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

ASSERT_PP_EQ((PTL_TUP(())), (()));
ASSERT_PP_EQ((PTL_TUP((1, 2))), ((1, 2)));
ASSERT_PP_EQ((PTL_STR(PTL_TUP(0))), ("PTL_TUP(0)"));
ASSERT_PP_EQ((PTL_STR(PTL_TUP(1, 2))), ("PTL_TUP(1, 2)"));
ASSERT_PP_EQ((PTL_STR(PTL_TUP(1,))), ("PTL_TUP(1,)"));
ASSERT_PP_EQ((PTL_STR(PTL_TUP(foo))), ("PTL_TUP(foo)"));
ASSERT_PP_EQ((PTL_STR(PTL_TUP((), ()))), ("PTL_TUP((), ())"));

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
ASSERT_PP_EQ((PTL_UINT(35)), (35));
ASSERT_PP_EQ((PTL_STR(PTL_UINT(36))), ("PTL_UINT(36)"));
ASSERT_PP_EQ((PTL_STR(PTL_UINT(1, 2))), ("PTL_UINT(1, 2)"));
ASSERT_PP_EQ((PTL_STR(PTL_UINT(1,))), ("PTL_UINT(1,)"));
ASSERT_PP_EQ((PTL_STR(PTL_UINT(foo))), ("PTL_UINT(foo)"));
ASSERT_PP_EQ((PTL_STR(PTL_UINT(()))), ("PTL_UINT(())"));
ASSERT_PP_EQ((PTL_STR(PTL_UINT((), ()))), ("PTL_UINT((), ())"));

ASSERT_PP_EQ((PTL_STR(PTL_XCT)), ("PPUTLXCT_A ( , )"));
ASSERT_PP_EQ((PTL_STR(PTL_ESC(PTL_XCT))), ("PPUTLXCT_B ( ,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_ESC(PTL_ESC(PTL_XCT)))), ("PPUTLXCT_A ( ,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_ESC(PTL_ESC(PTL_ESC(PTL_XCT))))), ("PPUTLXCT_B ( ,,,, )"));

ASSERT_PP_EQ((PTL_X(0)(PTL_XCT)), (PTL_ESC(PTL_XCT)));
ASSERT_PP_EQ((PTL_X(1)(PTL_XCT)), (PTL_ESC(PTL_ESC(PTL_XCT))));
ASSERT_PP_EQ((PTL_X(0)(PTL_XCT)), (PPUTLXCT_A ( , )));
ASSERT_PP_EQ((PTL_X(1)(PTL_XCT)), (PPUTLXCT_B ( ,, )));
ASSERT_PP_EQ((PTL_X(2)(PTL_XCT)), (PPUTLXCT_A ( ,,, )));
ASSERT_PP_EQ((PTL_X(3)(PTL_XCT)), (PPUTLXCT_B ( ,,,, )));
ASSERT_PP_EQ((PTL_X(4)(PTL_XCT)), (PPUTLXCT_A ( ,,,,, )));
ASSERT_PP_EQ((PTL_X(35)(PTL_XCT)), (PPUTLXCT_B ( ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,, )));

ASSERT_PP_EQ((PTL_INC(0)), (1));
ASSERT_PP_EQ((PTL_INC(1)), (2));
ASSERT_PP_EQ((PTL_INC(35)), (0));
ASSERT_PP_EQ((PTL_INC(2)), (3));
ASSERT_PP_EQ((PTL_INC(34)), (35));

ASSERT_PP_EQ((PTL_DEC(0)), (35));
ASSERT_PP_EQ((PTL_DEC(1)), (0));
ASSERT_PP_EQ((PTL_DEC(35)), (34));
ASSERT_PP_EQ((PTL_DEC(2)), (1));
ASSERT_PP_EQ((PTL_DEC(34)), (33));

ASSERT_PP_EQ((PTL_EQZ(0)), (1));
ASSERT_PP_EQ((PTL_EQZ(1)), (0));
ASSERT_PP_EQ((PTL_EQZ(2)), (0));
ASSERT_PP_EQ((PTL_EQZ(35)), (0));
ASSERT_PP_EQ((PTL_EQZ(PTL_INC(35))), (1));

ASSERT_PP_EQ((PTL_NEZ(0)), (0));
ASSERT_PP_EQ((PTL_NEZ(1)), (1));
ASSERT_PP_EQ((PTL_NEZ(2)), (1));
ASSERT_PP_EQ((PTL_NEZ(35)), (1));
ASSERT_PP_EQ((PTL_NEZ(PTL_INC(35))), (0));

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
ASSERT_PP_EQ((PTL_SWITCH(0, (a), (b), (c), (d), (e), (f), (g), (h), (i), (j), (k), (l), (m), (n), (o), (p), (q), (r), (s), (t), (u), (v), (w), (x), (y), (z), (A), (B), (C), (D), (E), (F), (G), (H), (I), (J))), (a));
ASSERT_PP_EQ((PTL_SWITCH(1, (a), (b), (c), (d), (e), (f), (g), (h), (i), (j), (k), (l), (m), (n), (o), (p), (q), (r), (s), (t), (u), (v), (w), (x), (y), (z), (A), (B), (C), (D), (E), (F), (G), (H), (I), (J))), (b));
ASSERT_PP_EQ((PTL_SWITCH(35, (a), (b), (c), (d), (e), (f), (g), (h), (i), (j), (k), (l), (m), (n), (o), (p), (q), (r), (s), (t), (u), (v), (w), (x), (y), (z), (A), (B), (C), (D), (E), (F), (G), (H), (I), (J))), (J));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(0, (PTL_XCT)))), ("PPUTLXCT_A ( ,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(1, (), (PTL_XCT)))), ("PPUTLXCT_A ( ,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(2, (), (), (PTL_XCT)))), ("PPUTLXCT_A ( ,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(3, (), (), (), (PTL_XCT)))), ("PPUTLXCT_A ( ,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(4, (), (), (), (), (PTL_XCT)))), ("PPUTLXCT_A ( ,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(5, (), (), (), (), (), (PTL_XCT)))), ("PPUTLXCT_A ( ,,,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(6, (), (), (), (), (), (), (PTL_XCT)))), ("PPUTLXCT_A ( ,,,,,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(7, (), (), (), (), (), (), (), (PTL_XCT)))), ("PPUTLXCT_A ( ,,,,,,,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(8, (), (), (), (), (), (), (), (), (PTL_XCT)))), ("PPUTLXCT_A ( ,,,,,,,,,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(9, (), (), (), (), (), (), (), (), (), (PTL_XCT)))), ("PPUTLXCT_A ( ,,,,,,,,,,,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(10, (), (), (), (), (), (), (), (), (), (), (PTL_XCT)))), ("PPUTLXCT_A ( ,,,,,,,,,,,,,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(11, (), (), (), (), (), (), (), (), (), (), (), (PTL_XCT)))), ("PPUTLXCT_A ( ,,,,,,,,,,,,,,,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(12, (), (), (), (), (), (), (), (), (), (), (), (), (PTL_XCT)))), ("PPUTLXCT_A ( ,,,,,,,,,,,,,,,,,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(13, (), (), (), (), (), (), (), (), (), (), (), (), (), (PTL_XCT)))), ("PPUTLXCT_A ( ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(14, (), (), (), (), (), (), (), (), (), (), (), (), (), (), (PTL_XCT)))), ("PPUTLXCT_A ( ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,, )"));

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

ASSERT_PP_EQ((PTL_IS_TUP()), (0));
ASSERT_PP_EQ((PTL_IS_TUP(1, 2)), (0));
ASSERT_PP_EQ((PTL_IS_TUP(())), (1));
ASSERT_PP_EQ((PTL_IS_TUP((1, 2))), (1));
ASSERT_PP_EQ((PTL_IS_TUP((), ())), (0));
ASSERT_PP_EQ((PTL_IS_TUP(PTL_ESC(()))), (1));
ASSERT_PP_EQ((PTL_IS_TUP(PTL_ESC((1, 2)))), (1));
ASSERT_PP_EQ((PTL_IS_TUP(, )), (0));
ASSERT_PP_EQ((PTL_IS_TUP(, , )), (0));
ASSERT_PP_EQ((PTL_IS_TUP(a, )), (0));
ASSERT_PP_EQ((PTL_IS_TUP(a, , )), (0));
ASSERT_PP_EQ((PTL_IS_TUP(, a)), (0));
ASSERT_PP_EQ((PTL_IS_TUP(, a, )), (0));
ASSERT_PP_EQ((PTL_IS_TUP(, , a)), (0));
ASSERT_PP_EQ((PTL_IS_TUP((, ))), (1));
ASSERT_PP_EQ((PTL_IS_TUP((, , ))), (1));
ASSERT_PP_EQ((PTL_IS_TUP((a, ))), (1));
ASSERT_PP_EQ((PTL_IS_TUP((a, , ))), (1));
ASSERT_PP_EQ((PTL_IS_TUP((, a))), (1));
ASSERT_PP_EQ((PTL_IS_TUP((, a, ))), (1));
ASSERT_PP_EQ((PTL_IS_TUP((, , a))), (1));

ASSERT_PP_EQ((PTL_IS_UINT()), (0));
ASSERT_PP_EQ((PTL_IS_UINT(foo)), (0));
ASSERT_PP_EQ((PTL_IS_UINT(0)), (1));
ASSERT_PP_EQ((PTL_IS_UINT(())), (0));
ASSERT_PP_EQ((PTL_IS_UINT((), ())), (0));
ASSERT_PP_EQ((PTL_IS_UINT(0, 1)), (0));
ASSERT_PP_EQ((PTL_IS_UINT(35)), (1));
ASSERT_PP_EQ((PTL_IS_UINT(foo, bar)), (0));
ASSERT_PP_EQ((PTL_IS_UINT(34)), (1));
ASSERT_PP_EQ((PTL_IS_UINT(, )), (0));
ASSERT_PP_EQ((PTL_IS_UINT(, , )), (0));
ASSERT_PP_EQ((PTL_IS_UINT(a, )), (0));
ASSERT_PP_EQ((PTL_IS_UINT(a, , )), (0));
ASSERT_PP_EQ((PTL_IS_UINT(, a)), (0));
ASSERT_PP_EQ((PTL_IS_UINT(, a, )), (0));
ASSERT_PP_EQ((PTL_IS_UINT(, , a)), (0));

ASSERT_PP_EQ((PTL_QTY()), (0));
ASSERT_PP_EQ((PTL_QTY(a)), (1));
ASSERT_PP_EQ((PTL_QTY(a, b)), (2));
ASSERT_PP_EQ((PTL_QTY(, )), (2));
ASSERT_PP_EQ((PTL_QTY(a, b, c)), (3));
ASSERT_PP_EQ((PTL_QTY(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34)), (35));
ASSERT_PP_EQ((PTL_QTY(, , )), (3));
ASSERT_PP_EQ((PTL_QTY(a, )), (2));
ASSERT_PP_EQ((PTL_QTY(a, , )), (3));
ASSERT_PP_EQ((PTL_QTY(, a)), (2));
ASSERT_PP_EQ((PTL_QTY(, a, )), (3));
ASSERT_PP_EQ((PTL_QTY(, , a)), (3));
// clang-format on

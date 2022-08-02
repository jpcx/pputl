#ifndef PPUTL_H_INCLUDED
#define PPUTL_H_INCLUDED
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
//    pputl requires __VA_ARGS__, __VA_OPT__, and empty variadic arguments    //
//    support (which are guaranteed by C++20)  but has no dependencies and    //
//    is single-header.                                                       //
//                                                                            //
//    USAGE                                                                   //
//    -----                                                                   //
//    Copy pputl.h and include. The default build uses a 10-bit uint range    //
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
///////////////////////////////////////////////////////////////////////////// */

/// [config.build]
/// --------------
/// the build number of this pputl release (ISO8601).
#define PTL_BUILD /* -> <c++ int> */ 20220802

/// [config.uint_max]
/// -----------------
/// the maximum value of a pputl unsigned int.
/// upper bound on the number of args for many pputl functions.
/// see the readme code generation section to configure.
#define PTL_UINT_MAX /* -> uint */ 35

/// [lang.eat]
/// ----------
/// eats arguments; return nothing.
#define PTL_EAT(...) /* -> <nothing> */

/// [lang.esc]
/// ----------
/// escapes parentheses. identity function.
///
/// PTL_ESC ()        // <nothing>
/// PTL_ESC (a, b, c) // a, b, c
#define PTL_ESC(/* args: any... */...) /* -> ...args */ __VA_ARGS__

/// [lang.str]
/// ----------
/// stringizes args after an expansion.
///
/// PTL_STR()         // ""
/// PTL_STR(foo, bar) // "foo, bar"
#define PTL_STR(/* args: any... */...) /* -> #...args */ PPUTLSTR_X(__VA_ARGS__)
#define PPUTLSTR_X(...)                #__VA_ARGS__

/// [lang.cat]
/// ----------
/// concatenates two args after an expansion.
/// args must be compatible with the ## operator.
///
/// PTL_CAT(foo, bar)          // foobar
/// PTL_CAT(foo, PTL_EAT(bar)) // foo
#define PTL_CAT(/* a: any, b: any */...) /* -> a##b */ __VA_OPT__(PPUTLCAT_X(__VA_ARGS__))
#define PPUTLCAT_X(a, b)                 a##b

/// [lang.first]
/// ------------
/// returns the first argument.
///
/// PTL_FIRST()     // <nothing>
/// PTL_FIRST(, )   // <nothing>
/// PTL_FIRST(a)    // a
/// PTL_FIRST(a, b) // a
#define PTL_FIRST(/* _: any, args: any... */...) /* -> a */ __VA_OPT__(PPUTLFIRST_X(__VA_ARGS__))
#define PPUTLFIRST_X(_, ...)                     _

/// [lang.rest]
/// -----------
/// returns all arguments except for the first.
///
/// PTL_REST()                  // <nothing>
/// PTL_REST(, )                // <nothing>
/// PTL_REST(a)                 // <nothing>
/// PTL_REST(a, b)              // b
/// PTL_REST(a, b, c)           // b, c
/// PTL_REST(PTL_REST(a, b, c)) // c
/// PTL_REST(a, , )             // ,
/// PTL_REST(a, b, , )          // b, ,
#define PTL_REST(/* _: any, args: any... */...) /* -> ...args */ __VA_OPT__(PPUTLREST_X(__VA_ARGS__))
#define PPUTLREST_X(_, ...)                     __VA_ARGS__

/// [type.tup]
/// ----------
/// tuple type (any...).
/// expands to t if valid, else, terminates expansion with a self-reference.
///
/// PTL_TUP(())              // ()
/// PTL_TUP((1, 2))          // (1, 2)
/// PTL_STR(PTL_TUP(0))      // "PTL_TUP(0)"
/// PTL_STR(PTL_TUP(1, 2))   // "PTL_TUP(1, 2)"
/// PTL_STR(PTL_TUP(1,))     // "PTL_TUP(1,)"
/// PTL_STR(PTL_TUP(foo))    // "PTL_TUP(foo)"
/// PTL_STR(PTL_TUP((), ())) // "PTL_TUP((), ())"
#define PTL_TUP(/* t: any... */...) /* -> tup{t} */ PPUTLTUP_O(__VA_ARGS__.)(__VA_ARGS__)(__VA_ARGS__)

/// first parentheses; asserts only one arg.
#define PPUTLTUP_O(_, ...)      PPUTLTUP_O_##__VA_OPT__(NO_)##PASS()
#define PPUTLTUP_O_NO_PASS(...) PPUTLTUP_OO_NO_PASS
#define PPUTLTUP_O_PASS(...)    PPUTLTUP_OO

/// second parentheses; asserts tuple.
#define PPUTLTUP_OO(_, ...)      PPUTLTUP_OO_RES(PTL_EAT _)
#define PPUTLTUP_OO_RES(...)     PPUTLTUP_OO_##__VA_OPT__(NO_)##PASS()
#define PPUTLTUP_OO_PASS(...)    PPUTLTUP_PASS
#define PPUTLTUP_OO_NO_PASS(...) PPUTLTUP_FAIL

/// fourth parentheses; returns
#define PPUTLTUP_PASS(...) __VA_ARGS__
#define PPUTLTUP_FAIL(...) PTL_TUP(__VA_ARGS__)

/// [type.bool]
/// -----------
/// bool type (0 or 1).
/// expands to b if valid, else, terminates expansion with a self-reference.
///
/// PTL_BOOL(0)               // 0
/// PTL_BOOL(1)               // 1
/// PTL_STR(PTL_BOOL(2))      // "PTL_BOOL(2)"
/// PTL_STR(PTL_BOOL(1, 2))   // "PTL_BOOL(1, 2)"
/// PTL_STR(PTL_BOOL(1,))     // "PTL_BOOL(1,)"
/// PTL_STR(PTL_BOOL(foo))    // "PTL_BOOL(foo)"
/// PTL_STR(PTL_BOOL(()))     // "PTL_BOOL(())"
/// PTL_STR(PTL_BOOL((), ())) // "PTL_BOOL((), ())"
#define PTL_BOOL(/* b: any... */...) /* -> bool{b} */ \
  PPUTLBOOL_O(__VA_ARGS__.)(__VA_ARGS__)(__VA_ARGS__)(__VA_ARGS__)

/// first parentheses; asserts only one arg.
#define PPUTLBOOL_O(_, ...)      PPUTLBOOL_O_##__VA_OPT__(NO_)##PASS()
#define PPUTLBOOL_O_NO_PASS(...) PPUTLBOOL_OO_FAIL
#define PPUTLBOOL_O_PASS(...)    PPUTLBOOL_OO

/// second parentheses; asserts non-tuple.
#define PPUTLBOOL_OO(_, ...)      PPUTLBOOL_OO_RES(PTL_EAT _)
#define PPUTLBOOL_OO_RES(...)     PPUTLBOOL_OO_##__VA_OPT__(NO_)##FAIL()
#define PPUTLBOOL_OO_NO_FAIL(...) PPUTLBOOL_OOO
#define PPUTLBOOL_OO_FAIL(...)    PPUTLBOOL_OOO_NO_PASS

/// third parentheses; asserts either 0 or 1.
#define PPUTLBOOL_OOO(...)         PPUTLBOOL_OOO_RES(PPUTLBOOL_CHK_##__VA_ARGS__)
#define PPUTLBOOL_OOO_RES(...)     PPUTLBOOL_OOO_##__VA_OPT__(NO_)##PASS()
#define PPUTLBOOL_OOO_PASS(...)    PPUTLBOOL_PASS
#define PPUTLBOOL_OOO_NO_PASS(...) PPUTLBOOL_FAIL

/// fourth parentheses; returns
#define PPUTLBOOL_PASS(...) __VA_ARGS__
#define PPUTLBOOL_FAIL(...) PTL_BOOL(__VA_ARGS__)

/// concat existence checks
#define PPUTLBOOL_CHK_1
#define PPUTLBOOL_CHK_0

/// [type.uint]
/// -----------
/// uint type (0 through 35).
/// expands to n if valid, else, terminates expansion with a self-reference.
///
/// PTL_UINT(0)               // 0
/// PTL_UINT(1)               // 1
/// PTL_UINT(2)               // 2
/// PTL_UINT(35)              // 35
/// PTL_STR(PTL_UINT(36))     // "PTL_UINT(36)"
/// PTL_STR(PTL_UINT(1, 2))   // "PTL_UINT(1, 2)"
/// PTL_STR(PTL_UINT(1,))     // "PTL_UINT(1,)"
/// PTL_STR(PTL_UINT(foo))    // "PTL_UINT(foo)"
/// PTL_STR(PTL_UINT(()))     // "PTL_UINT(())"
/// PTL_STR(PTL_UINT((), ())) // "PTL_UINT((), ())"
#define PTL_UINT(/* n: any... */...) /* -> uint{n} */ \
  PPUTLUINT_O(__VA_ARGS__.)(__VA_ARGS__)(__VA_ARGS__)(__VA_ARGS__)

/// first parentheses; asserts only one arg.
#define PPUTLUINT_O(_, ...)      PPUTLUINT_O_##__VA_OPT__(NO_)##PASS()
#define PPUTLUINT_O_NO_PASS(...) PPUTLUINT_OO_FAIL
#define PPUTLUINT_O_PASS(...)    PPUTLUINT_OO

/// second parentheses; asserts non-tuple.
#define PPUTLUINT_OO(_, ...)      PPUTLUINT_OO_RES(PTL_EAT _)
#define PPUTLUINT_OO_RES(...)     PPUTLUINT_OO_##__VA_OPT__(NO_)##FAIL()
#define PPUTLUINT_OO_NO_FAIL(...) PPUTLUINT_OOO
#define PPUTLUINT_OO_FAIL(...)    PPUTLUINT_OOO_FAIL

/// third parentheses; asserts one of 0 through 35.
#define PPUTLUINT_OOO(...)          PPUTLUINT_OOO_RES(PPUTLUINT_NEXT_##__VA_ARGS__)
#define PPUTLUINT_OOO_RES(...)      PPUTLUINT_OOO_RES_X(__VA_ARGS__)
#define PPUTLUINT_OOO_RES_X(_, ...) PPUTLUINT_OOO_##__VA_OPT__(NO_)##FAIL()
#define PPUTLUINT_OOO_NO_FAIL(...)  PPUTLUINT_PASS
#define PPUTLUINT_OOO_FAIL(...)     PPUTLUINT_FAIL

/// fourth parentheses; returns
#define PPUTLUINT_PASS(...) __VA_ARGS__
#define PPUTLUINT_FAIL(...) PTL_UINT(__VA_ARGS__)

/// uint cycle macros. verifies uint validity if cat result has two values
#define PPUTLUINT_NEXT_35 0, .
#define PPUTLUINT_NEXT_34 35, .
#define PPUTLUINT_NEXT_33 34, .
#define PPUTLUINT_NEXT_32 33, .
#define PPUTLUINT_NEXT_31 32, .
#define PPUTLUINT_NEXT_30 31, .
#define PPUTLUINT_NEXT_29 30, .
#define PPUTLUINT_NEXT_28 29, .
#define PPUTLUINT_NEXT_27 28, .
#define PPUTLUINT_NEXT_26 27, .
#define PPUTLUINT_NEXT_25 26, .
#define PPUTLUINT_NEXT_24 25, .
#define PPUTLUINT_NEXT_23 24, .
#define PPUTLUINT_NEXT_22 23, .
#define PPUTLUINT_NEXT_21 22, .
#define PPUTLUINT_NEXT_20 21, .
#define PPUTLUINT_NEXT_19 20, .
#define PPUTLUINT_NEXT_18 19, .
#define PPUTLUINT_NEXT_17 18, .
#define PPUTLUINT_NEXT_16 17, .
#define PPUTLUINT_NEXT_15 16, .
#define PPUTLUINT_NEXT_14 15, .
#define PPUTLUINT_NEXT_13 14, .
#define PPUTLUINT_NEXT_12 13, .
#define PPUTLUINT_NEXT_11 12, .
#define PPUTLUINT_NEXT_10 11, .
#define PPUTLUINT_NEXT_9  10, .
#define PPUTLUINT_NEXT_8  9, .
#define PPUTLUINT_NEXT_7  8, .
#define PPUTLUINT_NEXT_6  7, .
#define PPUTLUINT_NEXT_5  6, .
#define PPUTLUINT_NEXT_4  5, .
#define PPUTLUINT_NEXT_3  4, .
#define PPUTLUINT_NEXT_2  3, .
#define PPUTLUINT_NEXT_1  2, .
#define PPUTLUINT_NEXT_0  1, .
#define PPUTLUINT_PREV_35 34, .
#define PPUTLUINT_PREV_34 33, .
#define PPUTLUINT_PREV_33 32, .
#define PPUTLUINT_PREV_32 31, .
#define PPUTLUINT_PREV_31 30, .
#define PPUTLUINT_PREV_30 29, .
#define PPUTLUINT_PREV_29 28, .
#define PPUTLUINT_PREV_28 27, .
#define PPUTLUINT_PREV_27 26, .
#define PPUTLUINT_PREV_26 25, .
#define PPUTLUINT_PREV_25 24, .
#define PPUTLUINT_PREV_24 23, .
#define PPUTLUINT_PREV_23 22, .
#define PPUTLUINT_PREV_22 21, .
#define PPUTLUINT_PREV_21 20, .
#define PPUTLUINT_PREV_20 19, .
#define PPUTLUINT_PREV_19 18, .
#define PPUTLUINT_PREV_18 17, .
#define PPUTLUINT_PREV_17 16, .
#define PPUTLUINT_PREV_16 15, .
#define PPUTLUINT_PREV_15 14, .
#define PPUTLUINT_PREV_14 13, .
#define PPUTLUINT_PREV_13 12, .
#define PPUTLUINT_PREV_12 11, .
#define PPUTLUINT_PREV_11 10, .
#define PPUTLUINT_PREV_10 9, .
#define PPUTLUINT_PREV_9  8, .
#define PPUTLUINT_PREV_8  7, .
#define PPUTLUINT_PREV_7  6, .
#define PPUTLUINT_PREV_6  5, .
#define PPUTLUINT_PREV_5  4, .
#define PPUTLUINT_PREV_4  3, .
#define PPUTLUINT_PREV_3  2, .
#define PPUTLUINT_PREV_2  1, .
#define PPUTLUINT_PREV_1  0, .
#define PPUTLUINT_PREV_0  35, .

/// full unsigned integer sequences
#define PPUTLUINT_SEQ                                                                                       \
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, \
      29, 30, 31, 32, 33, 34, 35
#define PPUTLUINT_RSEQ                                                                                       \
  35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, \
      8, 7, 6, 5, 4, 3, 2, 1, 0

/// [meta.lp]
/// ---------
/// hides a left parens behind an expansion.
/// needed for implementing pair recursion.
#define PTL_LP /* -> <left parens> */ (

/// [meta.rp]
/// ---------
/// hides a right parens behind an expansion.
/// needed for implementing pair recursion.
#define PTL_RP /* -> <left parens> */ )

/// [meta.xct]
/// ----------
/// counts the number of expansions undergone after expression.
/// uses recursion; can track any number of expansions.
/// the number of commas indicates the number of expansions.
///
/// PTL_STR(PTL_XCT)                            // "PPUTLXCT_A ( , )"
/// PTL_STR(PTL_ESC(PTL_XCT))                   // "PPUTLXCT_B ( ,, )"
/// PTL_STR(PTL_ESC(PTL_ESC(PTL_XCT)))          // "PPUTLXCT_A ( ,,, )"
/// PTL_STR(PTL_ESC(PTL_ESC(PTL_ESC(PTL_XCT)))) // "PPUTLXCT_B ( ,,,, )"
#define PTL_XCT         /* -> <xct expr> */ PPUTLXCT_A PTL_LP /**/, PTL_RP
#define PPUTLXCT_B(...) PPUTLXCT_A PTL_LP __VA_ARGS__, PTL_RP
#define PPUTLXCT_A(...) PPUTLXCT_B PTL_LP __VA_ARGS__, PTL_RP

/// [meta.x]
/// --------
/// performs n secondary expansions (n=0 -> identity).
/// args are expressed after n+1 expansions in total.
/// n <= 35.
///
/// PTL_X(0)(PTL_XCT) // PTL_ESC(PTL_XCT)
/// PTL_X(1)(PTL_XCT) // PTL_ESC(PTL_ESC(PTL_XCT))
/// PTL_X(0)(PTL_XCT) // PPUTLXCT_A ( , )
/// PTL_X(1)(PTL_XCT) // PPUTLXCT_B ( ,, )
#define PTL_X(/* n: uint */...) /* -> (args: any...) -<n>-> ...args */ PTL_CAT(PPUTLX_, PTL_UINT(__VA_ARGS__))
#define PPUTLX_35(...)          PPUTLX_7(PPUTLX_8(PPUTLX_8(PPUTLX_8(__VA_ARGS__))))
#define PPUTLX_34(...)          PPUTLX_7(PPUTLX_7(PPUTLX_8(PPUTLX_8(__VA_ARGS__))))
#define PPUTLX_33(...)          PPUTLX_7(PPUTLX_7(PPUTLX_7(PPUTLX_8(__VA_ARGS__))))
#define PPUTLX_32(...)          PPUTLX_7(PPUTLX_7(PPUTLX_7(PPUTLX_7(__VA_ARGS__))))
#define PPUTLX_31(...)          PPUTLX_6(PPUTLX_7(PPUTLX_7(PPUTLX_7(__VA_ARGS__))))
#define PPUTLX_30(...)          PPUTLX_6(PPUTLX_6(PPUTLX_7(PPUTLX_7(__VA_ARGS__))))
#define PPUTLX_29(...)          PPUTLX_6(PPUTLX_6(PPUTLX_6(PPUTLX_7(__VA_ARGS__))))
#define PPUTLX_28(...)          PPUTLX_6(PPUTLX_6(PPUTLX_6(PPUTLX_6(__VA_ARGS__))))
#define PPUTLX_27(...)          PPUTLX_5(PPUTLX_6(PPUTLX_6(PPUTLX_6(__VA_ARGS__))))
#define PPUTLX_26(...)          PPUTLX_5(PPUTLX_5(PPUTLX_6(PPUTLX_6(__VA_ARGS__))))
#define PPUTLX_25(...)          PPUTLX_5(PPUTLX_5(PPUTLX_5(PPUTLX_6(__VA_ARGS__))))
#define PPUTLX_24(...)          PPUTLX_5(PPUTLX_5(PPUTLX_5(PPUTLX_5(__VA_ARGS__))))
#define PPUTLX_23(...)          PPUTLX_4(PPUTLX_5(PPUTLX_5(PPUTLX_5(__VA_ARGS__))))
#define PPUTLX_22(...)          PPUTLX_4(PPUTLX_4(PPUTLX_5(PPUTLX_5(__VA_ARGS__))))
#define PPUTLX_21(...)          PPUTLX_4(PPUTLX_4(PPUTLX_4(PPUTLX_5(__VA_ARGS__))))
#define PPUTLX_20(...)          PPUTLX_4(PPUTLX_4(PPUTLX_4(PPUTLX_4(__VA_ARGS__))))
#define PPUTLX_19(...)          PPUTLX_3(PPUTLX_4(PPUTLX_4(PPUTLX_4(__VA_ARGS__))))
#define PPUTLX_18(...)          PPUTLX_3(PPUTLX_3(PPUTLX_4(PPUTLX_4(__VA_ARGS__))))
#define PPUTLX_17(...)          PPUTLX_3(PPUTLX_3(PPUTLX_3(PPUTLX_4(__VA_ARGS__))))
#define PPUTLX_16(...)          PPUTLX_3(PPUTLX_3(PPUTLX_3(PPUTLX_3(__VA_ARGS__))))
#define PPUTLX_15(...)          PPUTLX_2(PPUTLX_3(PPUTLX_3(PPUTLX_3(__VA_ARGS__))))
#define PPUTLX_14(...)          PPUTLX_2(PPUTLX_2(PPUTLX_3(PPUTLX_3(__VA_ARGS__))))
#define PPUTLX_13(...)          PPUTLX_2(PPUTLX_2(PPUTLX_2(PPUTLX_3(__VA_ARGS__))))
#define PPUTLX_12(...)          PPUTLX_2(PPUTLX_2(PPUTLX_2(PPUTLX_2(__VA_ARGS__))))
#define PPUTLX_11(...)          PPUTLX_1(PPUTLX_2(PPUTLX_2(PPUTLX_2(__VA_ARGS__))))
#define PPUTLX_10(...)          PPUTLX_1(PPUTLX_1(PPUTLX_2(PPUTLX_2(__VA_ARGS__))))
#define PPUTLX_9(...)           PPUTLX_1(PPUTLX_1(PPUTLX_1(PPUTLX_2(__VA_ARGS__))))
#define PPUTLX_8(...)           PPUTLX_1(PPUTLX_1(PPUTLX_1(PPUTLX_1(__VA_ARGS__))))
#define PPUTLX_7(...)           PPUTLX_0(PPUTLX_1(PPUTLX_1(PPUTLX_1(__VA_ARGS__))))
#define PPUTLX_6(...)           PPUTLX_0(PPUTLX_0(PPUTLX_1(PPUTLX_1(__VA_ARGS__))))
#define PPUTLX_5(...)           PPUTLX_0(PPUTLX_0(PPUTLX_0(PPUTLX_1(__VA_ARGS__))))
#define PPUTLX_4(...)           PPUTLX_0(PPUTLX_0(PPUTLX_0(PPUTLX_0(__VA_ARGS__))))
#define PPUTLX_3(...)           PPUTLX_0(PPUTLX_0(PPUTLX_0(__VA_ARGS__)))
#define PPUTLX_2(...)           PPUTLX_0(PPUTLX_0(__VA_ARGS__))
#define PPUTLX_1(...)           PPUTLX_0(__VA_ARGS__)
#define PPUTLX_0(...)           __VA_ARGS__

/// [numeric.inc]
/// -------------
/// uint increment w/ overflow.
///
/// PTL_INC(0)  // 1
/// PTL_INC(1)  // 2
/// PTL_INC(35) // 0
#define PTL_INC(/* n: uint */...) /* -> uint{n+1} */ \
  PTL_FIRST(PTL_CAT(PPUTLUINT_NEXT_, PTL_UINT(__VA_ARGS__)))

/// [numeric.dec]
/// -------------
/// uint decrement w/ underflow.
///
/// PTL_DEC(0)  // 35
/// PTL_DEC(1)  // 0
/// PTL_DEC(35) // 34
#define PTL_DEC(/* n: uint */...) /* -> uint{n-1} */ \
  PTL_FIRST(PTL_CAT(PPUTLUINT_PREV_, PTL_UINT(__VA_ARGS__)))

/// [numeric.eqz]
/// -------------
/// detects if uint n is zero.
///
/// PTL_EQZ(0)           // 1
/// PTL_EQZ(1)           // 0
/// PTL_EQZ(2)           // 0
/// PTL_EQZ(35)          // 0
/// PTL_EQZ(PTL_INC(35)) // 1
#define PTL_EQZ(/* n: uint */...) /* -> uint{n==0} */ PPUTLEQZ_X(PTL_UINT(__VA_ARGS__))
#define PPUTLEQZ_X(n)             PPUTLEQZ_XX(n)
#define PPUTLEQZ_XX(n)            PPUTLEQZ_RES(PPUTLEQZ_##n)
#define PPUTLEQZ_RES(...)         PPUTLEQZ_##__VA_OPT__(NO_)##PASS
#define PPUTLEQZ_NO_PASS          0
#define PPUTLEQZ_PASS             1
#define PPUTLEQZ_0

/// [numeric.nez]
/// -------------
/// detects if uint n is not zero.
///
/// PTL_NEZ(0)           // 0
/// PTL_NEZ(1)           // 1
/// PTL_NEZ(2)           // 1
/// PTL_NEZ(35)          // 1
/// PTL_NEZ(PTL_INC(35)) // 0
#define PTL_NEZ(/* n: uint */...) /* -> uint{n!=0} */ PPUTLNEZ_X(PTL_UINT(__VA_ARGS__))
#define PPUTLNEZ_X(n)             PPUTLNEZ_XX(n)
#define PPUTLNEZ_XX(n)            PPUTLNEZ_RES(PPUTLNEZ_##n)
#define PPUTLNEZ_RES(...)         PPUTLNEZ_##__VA_OPT__(NO_)##FAIL
#define PPUTLNEZ_NO_FAIL          1
#define PPUTLNEZ_FAIL             0
#define PPUTLNEZ_0

/// [control.if]
/// ------------
/// conditionally expands items based on a boolean.
///
/// PTL_IF(1, (t), ())     // t
/// PTL_IF(0, (t), ())     // <nothing>
/// PTL_IF(1, (t), (f))    // t
/// PTL_IF(0, (t), (f))    // f
/// PTL_IF(1, (a), (b, c)) // a
/// PTL_IF(0, (a), (b, c)) // b, c
#define PTL_IF(/* b: bool, t: tup, f: tup */...) /* -> b ? ...t : ...f */ PPUTLIF_O(__VA_ARGS__)(__VA_ARGS__)

/// first parentheses; chooses next function based on b
#define PPUTLIF_O(b, ...) PTL_CAT(PPUTLIF_OO_, PTL_BOOL(b))

/// second parentheses; true result
#define PPUTLIF_OO_1(b, t, f) PPUTLIF_OO_1_X(PTL_TUP(t))
#define PPUTLIF_OO_1_X(t)     PTL_ESC t

/// second parentheses; false result
#define PPUTLIF_OO_0(b, t, f) PPUTLIF_OO_0_X(PTL_TUP(f))
#define PPUTLIF_OO_0_X(f)     PTL_ESC f

/// [control.switch]
/// ----------------
/// conditionally expands items based on a uint.
///
/// PTL_SWITCH(0, (1))              // 1
/// PTL_SWITCH(1, (1), (2))         // 2
/// PTL_SWITCH(2, (1), (2), (3, 4)) // 3, 4
#define PTL_SWITCH(/* cs: uint, cases: tup... */...) /* -<4+2cs>-> ...cases[cs] */ \
  PTL_X(PTL_FIRST(__VA_ARGS__))(PPUTLSWITCH_A(PTL_UINT(PTL_FIRST(__VA_ARGS__)))(__VA_ARGS__))

/// mutually recursive branch selector B
#define PPUTLSWITCH_B(i) PTL_IF(PTL_EQZ(i), (PPUTLSWITCH_B_BREAK), (PPUTLSWITCH_B_CONT))

/// B branches
#define PPUTLSWITCH_B_BREAK(i, _, ...) PTL_ESC _
#define PPUTLSWITCH_B_CONT(i, _, ...)  PPUTLSWITCH_A PTL_LP PTL_DEC(i) PTL_RP(PTL_DEC(i), __VA_ARGS__)

/// mutually recursive branch selector A
#define PPUTLSWITCH_A(i) PTL_IF(PTL_EQZ(i), (PPUTLSWITCH_A_BREAK), (PPUTLSWITCH_A_CONT))

/// A branches
#define PPUTLSWITCH_A_BREAK(i, _, ...) PTL_ESC _
#define PPUTLSWITCH_A_CONT(i, _, ...)  PPUTLSWITCH_B PTL_LP PTL_DEC(i) PTL_RP(PTL_DEC(i), __VA_ARGS__)

/// [traits.is_none]
/// ----------------
/// detects if args is nothing.
///
/// PTL_IS_NONE()          // 1
/// PTL_IS_NONE(foo)       // 0
/// PTL_IS_NONE(foo, bar)  // 0
/// PTL_IS_NONE(PTL_ESC()) // 1
#define PTL_IS_NONE(...) /* -> bool */ PPUTLIS_NONE_O##__VA_OPT__(0)
#define PPUTLIS_NONE_O0  0
#define PPUTLIS_NONE_O   1

/// [traits.is_some]
/// ----------------
/// detects if args is something.
///
/// PTL_IS_SOME()          // 0
/// PTL_IS_SOME(foo)       // 1
/// PTL_IS_SOME(foo, bar)  // 1
/// PTL_IS_SOME(PTL_ESC()) // 0
#define PTL_IS_SOME(...) /* -> bool */ PPUTLIS_SOME_O##__VA_OPT__(1)
#define PPUTLIS_SOME_O1  1
#define PPUTLIS_SOME_O   0

/// [traits.is_tup]
/// ---------------
/// detects if args is a tuple.
///
/// PTL_IS_TUP()       // 0
/// PTL_IS_TUP(1, 2)   // 0
/// PTL_IS_TUP(())     // 1
/// PTL_IS_TUP((1, 2)) // 1
#define PTL_IS_TUP(...) /* -> bool */ PTL_IS_NONE(PTL_EAT __VA_ARGS__)

/// [traits.is_uint]
/// ----------------
/// detects if args is a uint.
///
/// PTL_IS_UINT()       // 0
/// PTL_IS_UINT(foo)    // 0
/// PTL_IS_UINT(0)      // 1
/// PTL_IS_UINT(())     // 0
/// PTL_IS_UINT((), ()) // 0
/// PTL_IS_UINT(0, 1)   // 0
/// PTL_IS_UINT(35)     // 1
#define PTL_IS_UINT(...) /* -> bool */ PTL_IS_SOME(PTL_CAT(PPUTLIS_UINT_CHK_, PTL_UINT(__VA_ARGS__)))
#define PPUTLIS_UINT_CHK_PTL_UINT(...)

/// [traits.count]
/// --------------
/// computes the uint quantity of args.
/// terminates expansion with an error if too many args passed.
///
/// PTL_COUNT()     // 0
/// PTL_COUNT(a)    // 1
/// PTL_COUNT(a, b) // 2
/// PTL_COUNT(, )   // 2
#define PTL_COUNT(...) /* -> uint */ \
  PTL_IF(PTL_IS_NONE(__VA_ARGS__), (0), (PTL_X(34)(PPUTLCOUNT_A(1, __VA_ARGS__, ))))
#define PPUTLCOUNT_B(i, ...)                                            \
  PTL_IF(PTL_IS_NONE(__VA_ARGS__), (PPUTLCOUNT_RETURN),                 \
         (PTL_IF(PTL_EQZ(i), (PPUTLCOUNT_THROW), (PPUTLCOUNT_B_CONT)))) \
  (i, __VA_ARGS__)
#define PPUTLCOUNT_B_CONT(i, _, ...) PPUTLCOUNT_A PTL_LP PTL_INC(i), __VA_ARGS__ PTL_RP
#define PPUTLCOUNT_A(i, ...)                                            \
  PTL_IF(PTL_IS_NONE(__VA_ARGS__), (PPUTLCOUNT_RETURN),                 \
         (PTL_IF(PTL_EQZ(i), (PPUTLCOUNT_THROW), (PPUTLCOUNT_A_CONT)))) \
  (i, __VA_ARGS__)
#define PPUTLCOUNT_A_CONT(i, _, ...) PPUTLCOUNT_B PTL_LP PTL_INC(i), __VA_ARGS__ PTL_RP
#define PPUTLCOUNT_THROW(...)        PTL_COUNT(Error : too many args)
#define PPUTLCOUNT_RETURN(i, ...)    PTL_DEC(i)

#endif

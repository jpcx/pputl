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
//    for  research purposes and for various edge cases that still must be    //
//    solved using  text replacement,  such as optimizations that minimize    //
//    template specializations and syntactical boilerplate reductions.        //
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
//    pputl defines three types: tuple, uint, and bool. Features that use     //
//    one of these types  in their  parameter documentation  assert their     //
//    validity by type-casting. Type casts expand to their original value     //
//    if successful, else they trigger a preprocessor error.                  //
//                                                                            //
//    pputl errors execute an invalid preprocessor operation by using the     //
//    concatenation operator (incorrectly) on a string error message. All     //
//    errors  triggered by pputl functions  will include  the macro name,     //
//    a textual description, and the primary expansion arguments.             //
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
#define PTL_BUILD /* -> <c++ int> */ 20220806

/// [config.uint_max]
/// -----------------
/// the maximum value of a pputl unsigned int.
/// upper bound on the number of args for many pputl functions.
/// see the readme code generation section to configure.
#define PTL_UINT_MAX /* -> uint */ 1023

/// [lang.lp]
/// ---------
/// hides a left parens behind an expansion.
/// needed for implementing pair recursion.
#define PTL_LP() /* -> <left parens> */ (

/// [lang.rp]
/// ---------
/// hides a right parens behind an expansion.
/// needed for implementing pair recursion.
#define PTL_RP() /* -> <left parens> */ )

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
#define PTL_ESC(/* v: any... */...) /* -> ...v */ __VA_ARGS__

/// [lang.cat]
/// ----------
/// concatenates two args after an expansion.
/// args must be compatible with the ## operator.
///
/// PTL_CAT(foo, bar)          // foobar
/// PTL_CAT(foo, PTL_EAT(bar)) // foo
#define PTL_CAT(/* a: any, b: any */...) /* -> a##b */ __VA_OPT__(PPUTLCAT_X(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLCAT_X(a, b) a##b

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [lang.istr]
/// -----------
/// immediately stringizes args.
///
/// PTL_ISTR()                  // ""
/// PTL_ISTR(foo, bar)          // "foo, bar"
/// PTL_ISTR(PTL_CAT(foo, bar)) // "PTL_CAT(foo, bar)"
#define PTL_ISTR(/* v: any... */...) /* -> <string literal #...v> */ #__VA_ARGS__

/// [lang.str]
/// ----------
/// stringizes args after an expansion.
///
/// PTL_STR()                  // ""
/// PTL_STR(foo, bar)          // "foo, bar"
/// PTL_STR(PTL_CAT(foo, bar)) // "foobar"
#define PTL_STR(/* v: any... */...) /* -> <string literal #...v> */ PTL_ISTR(__VA_ARGS__)

/// [lang.first]
/// ------------
/// returns the first argument.
///
/// PTL_FIRST()     // <nothing>
/// PTL_FIRST(, )   // <nothing>
/// PTL_FIRST(a)    // a
/// PTL_FIRST(a, b) // a
#define PTL_FIRST(/* v: any... */...) /* -> v[0] */ __VA_OPT__(PPUTLFIRST_X(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLFIRST_X(_, ...) _

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

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
#define PTL_REST(/* v: any... */...) /* -> ...v */ __VA_OPT__(PPUTLREST_X(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLREST_X(_, ...) __VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [lang.trim]
/// -----------
/// removes an unnecessary comma at the front of args
/// if either the first arg or the rest args are nothing.
///
/// PTL_TRIM()       // <nothing>
/// PTL_TRIM(, )     // <nothing>
/// PTL_TRIM(a)      // a
/// PTL_TRIM(a, b)   // a, b
/// PTL_TRIM(a, )    // a
/// PTL_TRIM(, b, c) // b, c
/// PTL_TRIM(a, b, ) // a, b,
#define PTL_TRIM(/* v: any... */...) /* -> v[0] ? (v[1:] ? ...v : v[0]) : ...v[1:] */           \
  PTL_CAT(PPUTLTRIM_,                                                                           \
          PTL_CAT(PPUTLTRIM_SEL(PTL_FIRST(__VA_ARGS__)), PPUTLTRIM_SEL(PTL_REST(__VA_ARGS__)))) \
  (__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLTRIM_SEL(...)     0##__VA_OPT__(1)
#define PPUTLTRIM_0101(_, ...) _, __VA_ARGS__
#define PPUTLTRIM_010(_, ...)  _
#define PPUTLTRIM_001(_, ...)  __VA_ARGS__
#define PPUTLTRIM_00(...)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [lang.fail]
/// -----------
/// executes an invalid preprocessor operation to indicate a failure.
/// must provide a string literal message.
///
/// usage: PTL_FAIL(something bad happened)
///        PTL_FAIL(PTL_ISTR([myfun] invalid args : __VA_ARGS__))
#define PTL_FAIL(/* msg: <string literal> */...) /* -> <preprocessor error> */ PTL_FAIL##__VA_ARGS__

/// [type.tuple]
/// ------------
/// tuple type (any...).
/// expands to t if valid, else fails.
///
/// PTL_TUPLE(())     // ()
/// PTL_TUPLE((1, 2)) // (1, 2)
#define PTL_TUPLE(/* t: any... */...) /* -> tuple{t} */ \
  PPUTLTUPLE_O(PTL_EAT __VA_ARGS__)(PTL_ISTR([PTL_TUPLE] invalid tuple : __VA_ARGS__), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

/// first parentheses; detects tuple
#define PPUTLTUPLE_O(...) PPUTLTUPLE_OO##__VA_OPT__(_NO)##_PASS

/// second parentheses; returns or fails
#define PPUTLTUPLE_OO_NO_PASS(err, ...) PTL_FAIL(err)
#define PPUTLTUPLE_OO_PASS(err, ...)    __VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.bool]
/// -----------
/// bool type (0 or 1).
/// expands to b if valid, else fails.
///
/// PTL_BOOL(0) // 0
/// PTL_BOOL(1) // 1
#define PTL_BOOL(/* b: any... */...) /* -> bool{b} */ \
  PPUTLBOOL_O(__VA_ARGS__.)                           \
  (__VA_ARGS__)(__VA_ARGS__)(PTL_ISTR([PTL_BOOL] invalid bool : __VA_ARGS__), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

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
#define PPUTLBOOL_PASS(err, ...) __VA_ARGS__
#define PPUTLBOOL_FAIL(err, ...) PTL_FAIL(err)

/// concat existence checks
#define PPUTLBOOL_CHK_1
#define PPUTLBOOL_CHK_0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.uint]
/// -----------
/// uint type (0 through 1023).
/// expands to n if valid, else fails.
///
/// PTL_UINT(0)    // 0
/// PTL_UINT(1)    // 1
/// PTL_UINT(2)    // 2
/// PTL_UINT(1023) // 1023
#define PTL_UINT(/* n: any... */...) /* -> uint{n} */ \
  PPUTLUINT_O(__VA_ARGS__.)                           \
  (__VA_ARGS__)(__VA_ARGS__)(PTL_ISTR([PTL_UINT] invalid uint : __VA_ARGS__), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

/// first parentheses; asserts only one arg.
#define PPUTLUINT_O(_, ...)      PPUTLUINT_O_##__VA_OPT__(NO_)##PASS()
#define PPUTLUINT_O_NO_PASS(...) PPUTLUINT_OO_FAIL
#define PPUTLUINT_O_PASS(...)    PPUTLUINT_OO

/// second parentheses; asserts non-tuple.
#define PPUTLUINT_OO(_, ...)      PPUTLUINT_OO_RES(PTL_EAT _)
#define PPUTLUINT_OO_RES(...)     PPUTLUINT_OO_##__VA_OPT__(NO_)##FAIL()
#define PPUTLUINT_OO_NO_FAIL(...) PPUTLUINT_OOO
#define PPUTLUINT_OO_FAIL(...)    PPUTLUINT_OOO_FAIL

/// third parentheses; asserts one of 0 through 1023.
#define PPUTLUINT_OOO(...)          PPUTLUINT_OOO_RES(PPUTLUTRAITS_##__VA_ARGS__)
#define PPUTLUINT_OOO_RES(...)      PPUTLUINT_OOO_RES_X(__VA_ARGS__)
#define PPUTLUINT_OOO_RES_X(_, ...) PPUTLUINT_OOO_##__VA_OPT__(NO_)##FAIL()
#define PPUTLUINT_OOO_NO_FAIL(...)  PPUTLUINT_PASS
#define PPUTLUINT_OOO_FAIL(...)     PPUTLUINT_FAIL

/// fourth parentheses; returns
#define PPUTLUINT_PASS(err, ...) __VA_ARGS__
#define PPUTLUINT_FAIL(err, ...) PTL_FAIL(err)

/// dec, inc, log2, div2, mul2, sqrt, pow2, mod2, mod4, mod8, mod16, mod32, mod64, factor
#define PPUTLUTRAITS_1023 1022, 0, 9, 511, 1022, 31, 1, 1, 3, 7, 15, 31, 63, 3, 11, 31
#define PPUTLUTRAITS_1022 1021, 1023, 9, 511, 1020, 31, 4, 0, 2, 6, 14, 30, 62, 2, 7, 73
#define PPUTLUTRAITS_1021 1020, 1022, 9, 510, 1018, 31, 9, 1, 1, 5, 13, 29, 61,
#define PPUTLUTRAITS_1020 1019, 1021, 9, 510, 1016, 31, 16, 0, 0, 4, 12, 28, 60, 2, 2, 3, 5, 17
#define PPUTLUTRAITS_1019 1018, 1020, 9, 509, 1014, 31, 25, 1, 3, 3, 11, 27, 59,
#define PPUTLUTRAITS_1018 1017, 1019, 9, 509, 1012, 31, 36, 0, 2, 2, 10, 26, 58, 2, 509
#define PPUTLUTRAITS_1017 1016, 1018, 9, 508, 1010, 31, 49, 1, 1, 1, 9, 25, 57, 3, 3, 113
#define PPUTLUTRAITS_1016 1015, 1017, 9, 508, 1008, 31, 64, 0, 0, 0, 8, 24, 56, 2, 2, 2, 127
#define PPUTLUTRAITS_1015 1014, 1016, 9, 507, 1006, 31, 81, 1, 3, 7, 7, 23, 55, 5, 7, 29
#define PPUTLUTRAITS_1014 1013, 1015, 9, 507, 1004, 31, 100, 0, 2, 6, 6, 22, 54, 2, 3, 13, 13
#define PPUTLUTRAITS_1013 1012, 1014, 9, 506, 1002, 31, 121, 1, 1, 5, 5, 21, 53,
#define PPUTLUTRAITS_1012 1011, 1013, 9, 506, 1000, 31, 144, 0, 0, 4, 4, 20, 52, 2, 2, 11, 23
#define PPUTLUTRAITS_1011 1010, 1012, 9, 505, 998, 31, 169, 1, 3, 3, 3, 19, 51, 3, 337
#define PPUTLUTRAITS_1010 1009, 1011, 9, 505, 996, 31, 196, 0, 2, 2, 2, 18, 50, 2, 5, 101
#define PPUTLUTRAITS_1009 1008, 1010, 9, 504, 994, 31, 225, 1, 1, 1, 1, 17, 49,
#define PPUTLUTRAITS_1008 1007, 1009, 9, 504, 992, 31, 256, 0, 0, 0, 0, 16, 48, 2, 2, 2, 2, 3, 3, 7
#define PPUTLUTRAITS_1007 1006, 1008, 9, 503, 990, 31, 289, 1, 3, 7, 15, 15, 47, 19, 53
#define PPUTLUTRAITS_1006 1005, 1007, 9, 503, 988, 31, 324, 0, 2, 6, 14, 14, 46, 2, 503
#define PPUTLUTRAITS_1005 1004, 1006, 9, 502, 986, 31, 361, 1, 1, 5, 13, 13, 45, 3, 5, 67
#define PPUTLUTRAITS_1004 1003, 1005, 9, 502, 984, 31, 400, 0, 0, 4, 12, 12, 44, 2, 2, 251
#define PPUTLUTRAITS_1003 1002, 1004, 9, 501, 982, 31, 441, 1, 3, 3, 11, 11, 43, 17, 59
#define PPUTLUTRAITS_1002 1001, 1003, 9, 501, 980, 31, 484, 0, 2, 2, 10, 10, 42, 2, 3, 167
#define PPUTLUTRAITS_1001 1000, 1002, 9, 500, 978, 31, 529, 1, 1, 1, 9, 9, 41, 7, 11, 13
#define PPUTLUTRAITS_1000 999, 1001, 9, 500, 976, 31, 576, 0, 0, 0, 8, 8, 40, 2, 2, 2, 5, 5, 5
#define PPUTLUTRAITS_999  998, 1000, 9, 499, 974, 31, 625, 1, 3, 7, 7, 7, 39, 3, 3, 3, 37
#define PPUTLUTRAITS_998  997, 999, 9, 499, 972, 31, 676, 0, 2, 6, 6, 6, 38, 2, 499
#define PPUTLUTRAITS_997  996, 998, 9, 498, 970, 31, 729, 1, 1, 5, 5, 5, 37,
#define PPUTLUTRAITS_996  995, 997, 9, 498, 968, 31, 784, 0, 0, 4, 4, 4, 36, 2, 2, 3, 83
#define PPUTLUTRAITS_995  994, 996, 9, 497, 966, 31, 841, 1, 3, 3, 3, 3, 35, 5, 199
#define PPUTLUTRAITS_994  993, 995, 9, 497, 964, 31, 900, 0, 2, 2, 2, 2, 34, 2, 7, 71
#define PPUTLUTRAITS_993  992, 994, 9, 496, 962, 31, 961, 1, 1, 1, 1, 1, 33, 3, 331
#define PPUTLUTRAITS_992  991, 993, 9, 496, 960, 31, 0, 0, 0, 0, 0, 0, 32, 2, 2, 2, 2, 2, 31
#define PPUTLUTRAITS_991  990, 992, 9, 495, 958, 31, 65, 1, 3, 7, 15, 31, 31,
#define PPUTLUTRAITS_990  989, 991, 9, 495, 956, 31, 132, 0, 2, 6, 14, 30, 30, 2, 3, 3, 5, 11
#define PPUTLUTRAITS_989  988, 990, 9, 494, 954, 31, 201, 1, 1, 5, 13, 29, 29, 23, 43
#define PPUTLUTRAITS_988  987, 989, 9, 494, 952, 31, 272, 0, 0, 4, 12, 28, 28, 2, 2, 13, 19
#define PPUTLUTRAITS_987  986, 988, 9, 493, 950, 31, 345, 1, 3, 3, 11, 27, 27, 3, 7, 47
#define PPUTLUTRAITS_986  985, 987, 9, 493, 948, 31, 420, 0, 2, 2, 10, 26, 26, 2, 17, 29
#define PPUTLUTRAITS_985  984, 986, 9, 492, 946, 31, 497, 1, 1, 1, 9, 25, 25, 5, 197
#define PPUTLUTRAITS_984  983, 985, 9, 492, 944, 31, 576, 0, 0, 0, 8, 24, 24, 2, 2, 2, 3, 41
#define PPUTLUTRAITS_983  982, 984, 9, 491, 942, 31, 657, 1, 3, 7, 7, 23, 23,
#define PPUTLUTRAITS_982  981, 983, 9, 491, 940, 31, 740, 0, 2, 6, 6, 22, 22, 2, 491
#define PPUTLUTRAITS_981  980, 982, 9, 490, 938, 31, 825, 1, 1, 5, 5, 21, 21, 3, 3, 109
#define PPUTLUTRAITS_980  979, 981, 9, 490, 936, 31, 912, 0, 0, 4, 4, 20, 20, 2, 2, 5, 7, 7
#define PPUTLUTRAITS_979  978, 980, 9, 489, 934, 31, 1001, 1, 3, 3, 3, 19, 19, 11, 89
#define PPUTLUTRAITS_978  977, 979, 9, 489, 932, 31, 68, 0, 2, 2, 2, 18, 18, 2, 3, 163
#define PPUTLUTRAITS_977  976, 978, 9, 488, 930, 31, 161, 1, 1, 1, 1, 17, 17,
#define PPUTLUTRAITS_976  975, 977, 9, 488, 928, 31, 256, 0, 0, 0, 0, 16, 16, 2, 2, 2, 2, 61
#define PPUTLUTRAITS_975  974, 976, 9, 487, 926, 31, 353, 1, 3, 7, 15, 15, 15, 3, 5, 5, 13
#define PPUTLUTRAITS_974  973, 975, 9, 487, 924, 31, 452, 0, 2, 6, 14, 14, 14, 2, 487
#define PPUTLUTRAITS_973  972, 974, 9, 486, 922, 31, 553, 1, 1, 5, 13, 13, 13, 7, 139
#define PPUTLUTRAITS_972  971, 973, 9, 486, 920, 31, 656, 0, 0, 4, 12, 12, 12, 2, 2, 3, 3, 3, 3, 3
#define PPUTLUTRAITS_971  970, 972, 9, 485, 918, 31, 761, 1, 3, 3, 11, 11, 11,
#define PPUTLUTRAITS_970  969, 971, 9, 485, 916, 31, 868, 0, 2, 2, 10, 10, 10, 2, 5, 97
#define PPUTLUTRAITS_969  968, 970, 9, 484, 914, 31, 977, 1, 1, 1, 9, 9, 9, 3, 17, 19
#define PPUTLUTRAITS_968  967, 969, 9, 484, 912, 31, 64, 0, 0, 0, 8, 8, 8, 2, 2, 2, 11, 11
#define PPUTLUTRAITS_967  966, 968, 9, 483, 910, 31, 177, 1, 3, 7, 7, 7, 7,
#define PPUTLUTRAITS_966  965, 967, 9, 483, 908, 31, 292, 0, 2, 6, 6, 6, 6, 2, 3, 7, 23
#define PPUTLUTRAITS_965  964, 966, 9, 482, 906, 31, 409, 1, 1, 5, 5, 5, 5, 5, 193
#define PPUTLUTRAITS_964  963, 965, 9, 482, 904, 31, 528, 0, 0, 4, 4, 4, 4, 2, 2, 241
#define PPUTLUTRAITS_963  962, 964, 9, 481, 902, 31, 649, 1, 3, 3, 3, 3, 3, 3, 3, 107
#define PPUTLUTRAITS_962  961, 963, 9, 481, 900, 31, 772, 0, 2, 2, 2, 2, 2, 2, 13, 37
#define PPUTLUTRAITS_961  960, 962, 9, 480, 898, 31, 897, 1, 1, 1, 1, 1, 1, 31, 31
#define PPUTLUTRAITS_960  959, 961, 9, 480, 896, 30, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 3, 5
#define PPUTLUTRAITS_959  958, 960, 9, 479, 894, 30, 129, 1, 3, 7, 15, 31, 63, 7, 137
#define PPUTLUTRAITS_958  957, 959, 9, 479, 892, 30, 260, 0, 2, 6, 14, 30, 62, 2, 479
#define PPUTLUTRAITS_957  956, 958, 9, 478, 890, 30, 393, 1, 1, 5, 13, 29, 61, 3, 11, 29
#define PPUTLUTRAITS_956  955, 957, 9, 478, 888, 30, 528, 0, 0, 4, 12, 28, 60, 2, 2, 239
#define PPUTLUTRAITS_955  954, 956, 9, 477, 886, 30, 665, 1, 3, 3, 11, 27, 59, 5, 191
#define PPUTLUTRAITS_954  953, 955, 9, 477, 884, 30, 804, 0, 2, 2, 10, 26, 58, 2, 3, 3, 53
#define PPUTLUTRAITS_953  952, 954, 9, 476, 882, 30, 945, 1, 1, 1, 9, 25, 57,
#define PPUTLUTRAITS_952  951, 953, 9, 476, 880, 30, 64, 0, 0, 0, 8, 24, 56, 2, 2, 2, 7, 17
#define PPUTLUTRAITS_951  950, 952, 9, 475, 878, 30, 209, 1, 3, 7, 7, 23, 55, 3, 317
#define PPUTLUTRAITS_950  949, 951, 9, 475, 876, 30, 356, 0, 2, 6, 6, 22, 54, 2, 5, 5, 19
#define PPUTLUTRAITS_949  948, 950, 9, 474, 874, 30, 505, 1, 1, 5, 5, 21, 53, 13, 73
#define PPUTLUTRAITS_948  947, 949, 9, 474, 872, 30, 656, 0, 0, 4, 4, 20, 52, 2, 2, 3, 79
#define PPUTLUTRAITS_947  946, 948, 9, 473, 870, 30, 809, 1, 3, 3, 3, 19, 51,
#define PPUTLUTRAITS_946  945, 947, 9, 473, 868, 30, 964, 0, 2, 2, 2, 18, 50, 2, 11, 43
#define PPUTLUTRAITS_945  944, 946, 9, 472, 866, 30, 97, 1, 1, 1, 1, 17, 49, 3, 3, 3, 5, 7
#define PPUTLUTRAITS_944  943, 945, 9, 472, 864, 30, 256, 0, 0, 0, 0, 16, 48, 2, 2, 2, 2, 59
#define PPUTLUTRAITS_943  942, 944, 9, 471, 862, 30, 417, 1, 3, 7, 15, 15, 47, 23, 41
#define PPUTLUTRAITS_942  941, 943, 9, 471, 860, 30, 580, 0, 2, 6, 14, 14, 46, 2, 3, 157
#define PPUTLUTRAITS_941  940, 942, 9, 470, 858, 30, 745, 1, 1, 5, 13, 13, 45,
#define PPUTLUTRAITS_940  939, 941, 9, 470, 856, 30, 912, 0, 0, 4, 12, 12, 44, 2, 2, 5, 47
#define PPUTLUTRAITS_939  938, 940, 9, 469, 854, 30, 57, 1, 3, 3, 11, 11, 43, 3, 313
#define PPUTLUTRAITS_938  937, 939, 9, 469, 852, 30, 228, 0, 2, 2, 10, 10, 42, 2, 7, 67
#define PPUTLUTRAITS_937  936, 938, 9, 468, 850, 30, 401, 1, 1, 1, 9, 9, 41,
#define PPUTLUTRAITS_936  935, 937, 9, 468, 848, 30, 576, 0, 0, 0, 8, 8, 40, 2, 2, 2, 3, 3, 13
#define PPUTLUTRAITS_935  934, 936, 9, 467, 846, 30, 753, 1, 3, 7, 7, 7, 39, 5, 11, 17
#define PPUTLUTRAITS_934  933, 935, 9, 467, 844, 30, 932, 0, 2, 6, 6, 6, 38, 2, 467
#define PPUTLUTRAITS_933  932, 934, 9, 466, 842, 30, 89, 1, 1, 5, 5, 5, 37, 3, 311
#define PPUTLUTRAITS_932  931, 933, 9, 466, 840, 30, 272, 0, 0, 4, 4, 4, 36, 2, 2, 233
#define PPUTLUTRAITS_931  930, 932, 9, 465, 838, 30, 457, 1, 3, 3, 3, 3, 35, 7, 7, 19
#define PPUTLUTRAITS_930  929, 931, 9, 465, 836, 30, 644, 0, 2, 2, 2, 2, 34, 2, 3, 5, 31
#define PPUTLUTRAITS_929  928, 930, 9, 464, 834, 30, 833, 1, 1, 1, 1, 1, 33,
#define PPUTLUTRAITS_928  927, 929, 9, 464, 832, 30, 0, 0, 0, 0, 0, 0, 32, 2, 2, 2, 2, 2, 29
#define PPUTLUTRAITS_927  926, 928, 9, 463, 830, 30, 193, 1, 3, 7, 15, 31, 31, 3, 3, 103
#define PPUTLUTRAITS_926  925, 927, 9, 463, 828, 30, 388, 0, 2, 6, 14, 30, 30, 2, 463
#define PPUTLUTRAITS_925  924, 926, 9, 462, 826, 30, 585, 1, 1, 5, 13, 29, 29, 5, 5, 37
#define PPUTLUTRAITS_924  923, 925, 9, 462, 824, 30, 784, 0, 0, 4, 12, 28, 28, 2, 2, 3, 7, 11
#define PPUTLUTRAITS_923  922, 924, 9, 461, 822, 30, 985, 1, 3, 3, 11, 27, 27, 13, 71
#define PPUTLUTRAITS_922  921, 923, 9, 461, 820, 30, 164, 0, 2, 2, 10, 26, 26, 2, 461
#define PPUTLUTRAITS_921  920, 922, 9, 460, 818, 30, 369, 1, 1, 1, 9, 25, 25, 3, 307
#define PPUTLUTRAITS_920  919, 921, 9, 460, 816, 30, 576, 0, 0, 0, 8, 24, 24, 2, 2, 2, 5, 23
#define PPUTLUTRAITS_919  918, 920, 9, 459, 814, 30, 785, 1, 3, 7, 7, 23, 23,
#define PPUTLUTRAITS_918  917, 919, 9, 459, 812, 30, 996, 0, 2, 6, 6, 22, 22, 2, 3, 3, 3, 17
#define PPUTLUTRAITS_917  916, 918, 9, 458, 810, 30, 185, 1, 1, 5, 5, 21, 21, 7, 131
#define PPUTLUTRAITS_916  915, 917, 9, 458, 808, 30, 400, 0, 0, 4, 4, 20, 20, 2, 2, 229
#define PPUTLUTRAITS_915  914, 916, 9, 457, 806, 30, 617, 1, 3, 3, 3, 19, 19, 3, 5, 61
#define PPUTLUTRAITS_914  913, 915, 9, 457, 804, 30, 836, 0, 2, 2, 2, 18, 18, 2, 457
#define PPUTLUTRAITS_913  912, 914, 9, 456, 802, 30, 33, 1, 1, 1, 1, 17, 17, 11, 83
#define PPUTLUTRAITS_912  911, 913, 9, 456, 800, 30, 256, 0, 0, 0, 0, 16, 16, 2, 2, 2, 2, 3, 19
#define PPUTLUTRAITS_911  910, 912, 9, 455, 798, 30, 481, 1, 3, 7, 15, 15, 15,
#define PPUTLUTRAITS_910  909, 911, 9, 455, 796, 30, 708, 0, 2, 6, 14, 14, 14, 2, 5, 7, 13
#define PPUTLUTRAITS_909  908, 910, 9, 454, 794, 30, 937, 1, 1, 5, 13, 13, 13, 3, 3, 101
#define PPUTLUTRAITS_908  907, 909, 9, 454, 792, 30, 144, 0, 0, 4, 12, 12, 12, 2, 2, 227
#define PPUTLUTRAITS_907  906, 908, 9, 453, 790, 30, 377, 1, 3, 3, 11, 11, 11,
#define PPUTLUTRAITS_906  905, 907, 9, 453, 788, 30, 612, 0, 2, 2, 10, 10, 10, 2, 3, 151
#define PPUTLUTRAITS_905  904, 906, 9, 452, 786, 30, 849, 1, 1, 1, 9, 9, 9, 5, 181
#define PPUTLUTRAITS_904  903, 905, 9, 452, 784, 30, 64, 0, 0, 0, 8, 8, 8, 2, 2, 2, 113
#define PPUTLUTRAITS_903  902, 904, 9, 451, 782, 30, 305, 1, 3, 7, 7, 7, 7, 3, 7, 43
#define PPUTLUTRAITS_902  901, 903, 9, 451, 780, 30, 548, 0, 2, 6, 6, 6, 6, 2, 11, 41
#define PPUTLUTRAITS_901  900, 902, 9, 450, 778, 30, 793, 1, 1, 5, 5, 5, 5, 17, 53
#define PPUTLUTRAITS_900  899, 901, 9, 450, 776, 30, 16, 0, 0, 4, 4, 4, 4, 2, 2, 3, 3, 5, 5
#define PPUTLUTRAITS_899  898, 900, 9, 449, 774, 29, 265, 1, 3, 3, 3, 3, 3, 29, 31
#define PPUTLUTRAITS_898  897, 899, 9, 449, 772, 29, 516, 0, 2, 2, 2, 2, 2, 2, 449
#define PPUTLUTRAITS_897  896, 898, 9, 448, 770, 29, 769, 1, 1, 1, 1, 1, 1, 3, 13, 23
#define PPUTLUTRAITS_896  895, 897, 9, 448, 768, 29, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 7
#define PPUTLUTRAITS_895  894, 896, 9, 447, 766, 29, 257, 1, 3, 7, 15, 31, 63, 5, 179
#define PPUTLUTRAITS_894  893, 895, 9, 447, 764, 29, 516, 0, 2, 6, 14, 30, 62, 2, 3, 149
#define PPUTLUTRAITS_893  892, 894, 9, 446, 762, 29, 777, 1, 1, 5, 13, 29, 61, 19, 47
#define PPUTLUTRAITS_892  891, 893, 9, 446, 760, 29, 16, 0, 0, 4, 12, 28, 60, 2, 2, 223
#define PPUTLUTRAITS_891  890, 892, 9, 445, 758, 29, 281, 1, 3, 3, 11, 27, 59, 3, 3, 3, 3, 11
#define PPUTLUTRAITS_890  889, 891, 9, 445, 756, 29, 548, 0, 2, 2, 10, 26, 58, 2, 5, 89
#define PPUTLUTRAITS_889  888, 890, 9, 444, 754, 29, 817, 1, 1, 1, 9, 25, 57, 7, 127
#define PPUTLUTRAITS_888  887, 889, 9, 444, 752, 29, 64, 0, 0, 0, 8, 24, 56, 2, 2, 2, 3, 37
#define PPUTLUTRAITS_887  886, 888, 9, 443, 750, 29, 337, 1, 3, 7, 7, 23, 55,
#define PPUTLUTRAITS_886  885, 887, 9, 443, 748, 29, 612, 0, 2, 6, 6, 22, 54, 2, 443
#define PPUTLUTRAITS_885  884, 886, 9, 442, 746, 29, 889, 1, 1, 5, 5, 21, 53, 3, 5, 59
#define PPUTLUTRAITS_884  883, 885, 9, 442, 744, 29, 144, 0, 0, 4, 4, 20, 52, 2, 2, 13, 17
#define PPUTLUTRAITS_883  882, 884, 9, 441, 742, 29, 425, 1, 3, 3, 3, 19, 51,
#define PPUTLUTRAITS_882  881, 883, 9, 441, 740, 29, 708, 0, 2, 2, 2, 18, 50, 2, 3, 3, 7, 7
#define PPUTLUTRAITS_881  880, 882, 9, 440, 738, 29, 993, 1, 1, 1, 1, 17, 49,
#define PPUTLUTRAITS_880  879, 881, 9, 440, 736, 29, 256, 0, 0, 0, 0, 16, 48, 2, 2, 2, 2, 5, 11
#define PPUTLUTRAITS_879  878, 880, 9, 439, 734, 29, 545, 1, 3, 7, 15, 15, 47, 3, 293
#define PPUTLUTRAITS_878  877, 879, 9, 439, 732, 29, 836, 0, 2, 6, 14, 14, 46, 2, 439
#define PPUTLUTRAITS_877  876, 878, 9, 438, 730, 29, 105, 1, 1, 5, 13, 13, 45,
#define PPUTLUTRAITS_876  875, 877, 9, 438, 728, 29, 400, 0, 0, 4, 12, 12, 44, 2, 2, 3, 73
#define PPUTLUTRAITS_875  874, 876, 9, 437, 726, 29, 697, 1, 3, 3, 11, 11, 43, 5, 5, 5, 7
#define PPUTLUTRAITS_874  873, 875, 9, 437, 724, 29, 996, 0, 2, 2, 10, 10, 42, 2, 19, 23
#define PPUTLUTRAITS_873  872, 874, 9, 436, 722, 29, 273, 1, 1, 1, 9, 9, 41, 3, 3, 97
#define PPUTLUTRAITS_872  871, 873, 9, 436, 720, 29, 576, 0, 0, 0, 8, 8, 40, 2, 2, 2, 109
#define PPUTLUTRAITS_871  870, 872, 9, 435, 718, 29, 881, 1, 3, 7, 7, 7, 39, 13, 67
#define PPUTLUTRAITS_870  869, 871, 9, 435, 716, 29, 164, 0, 2, 6, 6, 6, 38, 2, 3, 5, 29
#define PPUTLUTRAITS_869  868, 870, 9, 434, 714, 29, 473, 1, 1, 5, 5, 5, 37, 11, 79
#define PPUTLUTRAITS_868  867, 869, 9, 434, 712, 29, 784, 0, 0, 4, 4, 4, 36, 2, 2, 7, 31
#define PPUTLUTRAITS_867  866, 868, 9, 433, 710, 29, 73, 1, 3, 3, 3, 3, 35, 3, 17, 17
#define PPUTLUTRAITS_866  865, 867, 9, 433, 708, 29, 388, 0, 2, 2, 2, 2, 34, 2, 433
#define PPUTLUTRAITS_865  864, 866, 9, 432, 706, 29, 705, 1, 1, 1, 1, 1, 33, 5, 173
#define PPUTLUTRAITS_864  863, 865, 9, 432, 704, 29, 0, 0, 0, 0, 0, 0, 32, 2, 2, 2, 2, 2, 3, 3, 3
#define PPUTLUTRAITS_863  862, 864, 9, 431, 702, 29, 321, 1, 3, 7, 15, 31, 31,
#define PPUTLUTRAITS_862  861, 863, 9, 431, 700, 29, 644, 0, 2, 6, 14, 30, 30, 2, 431
#define PPUTLUTRAITS_861  860, 862, 9, 430, 698, 29, 969, 1, 1, 5, 13, 29, 29, 3, 7, 41
#define PPUTLUTRAITS_860  859, 861, 9, 430, 696, 29, 272, 0, 0, 4, 12, 28, 28, 2, 2, 5, 43
#define PPUTLUTRAITS_859  858, 860, 9, 429, 694, 29, 601, 1, 3, 3, 11, 27, 27,
#define PPUTLUTRAITS_858  857, 859, 9, 429, 692, 29, 932, 0, 2, 2, 10, 26, 26, 2, 3, 11, 13
#define PPUTLUTRAITS_857  856, 858, 9, 428, 690, 29, 241, 1, 1, 1, 9, 25, 25,
#define PPUTLUTRAITS_856  855, 857, 9, 428, 688, 29, 576, 0, 0, 0, 8, 24, 24, 2, 2, 2, 107
#define PPUTLUTRAITS_855  854, 856, 9, 427, 686, 29, 913, 1, 3, 7, 7, 23, 23, 3, 3, 5, 19
#define PPUTLUTRAITS_854  853, 855, 9, 427, 684, 29, 228, 0, 2, 6, 6, 22, 22, 2, 7, 61
#define PPUTLUTRAITS_853  852, 854, 9, 426, 682, 29, 569, 1, 1, 5, 5, 21, 21,
#define PPUTLUTRAITS_852  851, 853, 9, 426, 680, 29, 912, 0, 0, 4, 4, 20, 20, 2, 2, 3, 71
#define PPUTLUTRAITS_851  850, 852, 9, 425, 678, 29, 233, 1, 3, 3, 3, 19, 19, 23, 37
#define PPUTLUTRAITS_850  849, 851, 9, 425, 676, 29, 580, 0, 2, 2, 2, 18, 18, 2, 5, 5, 17
#define PPUTLUTRAITS_849  848, 850, 9, 424, 674, 29, 929, 1, 1, 1, 1, 17, 17, 3, 283
#define PPUTLUTRAITS_848  847, 849, 9, 424, 672, 29, 256, 0, 0, 0, 0, 16, 16, 2, 2, 2, 2, 53
#define PPUTLUTRAITS_847  846, 848, 9, 423, 670, 29, 609, 1, 3, 7, 15, 15, 15, 7, 11, 11
#define PPUTLUTRAITS_846  845, 847, 9, 423, 668, 29, 964, 0, 2, 6, 14, 14, 14, 2, 3, 3, 47
#define PPUTLUTRAITS_845  844, 846, 9, 422, 666, 29, 297, 1, 1, 5, 13, 13, 13, 5, 13, 13
#define PPUTLUTRAITS_844  843, 845, 9, 422, 664, 29, 656, 0, 0, 4, 12, 12, 12, 2, 2, 211
#define PPUTLUTRAITS_843  842, 844, 9, 421, 662, 29, 1017, 1, 3, 3, 11, 11, 11, 3, 281
#define PPUTLUTRAITS_842  841, 843, 9, 421, 660, 29, 356, 0, 2, 2, 10, 10, 10, 2, 421
#define PPUTLUTRAITS_841  840, 842, 9, 420, 658, 29, 721, 1, 1, 1, 9, 9, 9, 29, 29
#define PPUTLUTRAITS_840  839, 841, 9, 420, 656, 28, 64, 0, 0, 0, 8, 8, 8, 2, 2, 2, 3, 5, 7
#define PPUTLUTRAITS_839  838, 840, 9, 419, 654, 28, 433, 1, 3, 7, 7, 7, 7,
#define PPUTLUTRAITS_838  837, 839, 9, 419, 652, 28, 804, 0, 2, 6, 6, 6, 6, 2, 419
#define PPUTLUTRAITS_837  836, 838, 9, 418, 650, 28, 153, 1, 1, 5, 5, 5, 5, 3, 3, 3, 31
#define PPUTLUTRAITS_836  835, 837, 9, 418, 648, 28, 528, 0, 0, 4, 4, 4, 4, 2, 2, 11, 19
#define PPUTLUTRAITS_835  834, 836, 9, 417, 646, 28, 905, 1, 3, 3, 3, 3, 3, 5, 167
#define PPUTLUTRAITS_834  833, 835, 9, 417, 644, 28, 260, 0, 2, 2, 2, 2, 2, 2, 3, 139
#define PPUTLUTRAITS_833  832, 834, 9, 416, 642, 28, 641, 1, 1, 1, 1, 1, 1, 7, 7, 17
#define PPUTLUTRAITS_832  831, 833, 9, 416, 640, 28, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 13
#define PPUTLUTRAITS_831  830, 832, 9, 415, 638, 28, 385, 1, 3, 7, 15, 31, 63, 3, 277
#define PPUTLUTRAITS_830  829, 831, 9, 415, 636, 28, 772, 0, 2, 6, 14, 30, 62, 2, 5, 83
#define PPUTLUTRAITS_829  828, 830, 9, 414, 634, 28, 137, 1, 1, 5, 13, 29, 61,
#define PPUTLUTRAITS_828  827, 829, 9, 414, 632, 28, 528, 0, 0, 4, 12, 28, 60, 2, 2, 3, 3, 23
#define PPUTLUTRAITS_827  826, 828, 9, 413, 630, 28, 921, 1, 3, 3, 11, 27, 59,
#define PPUTLUTRAITS_826  825, 827, 9, 413, 628, 28, 292, 0, 2, 2, 10, 26, 58, 2, 7, 59
#define PPUTLUTRAITS_825  824, 826, 9, 412, 626, 28, 689, 1, 1, 1, 9, 25, 57, 3, 5, 5, 11
#define PPUTLUTRAITS_824  823, 825, 9, 412, 624, 28, 64, 0, 0, 0, 8, 24, 56, 2, 2, 2, 103
#define PPUTLUTRAITS_823  822, 824, 9, 411, 622, 28, 465, 1, 3, 7, 7, 23, 55,
#define PPUTLUTRAITS_822  821, 823, 9, 411, 620, 28, 868, 0, 2, 6, 6, 22, 54, 2, 3, 137
#define PPUTLUTRAITS_821  820, 822, 9, 410, 618, 28, 249, 1, 1, 5, 5, 21, 53,
#define PPUTLUTRAITS_820  819, 821, 9, 410, 616, 28, 656, 0, 0, 4, 4, 20, 52, 2, 2, 5, 41
#define PPUTLUTRAITS_819  818, 820, 9, 409, 614, 28, 41, 1, 3, 3, 3, 19, 51, 3, 3, 7, 13
#define PPUTLUTRAITS_818  817, 819, 9, 409, 612, 28, 452, 0, 2, 2, 2, 18, 50, 2, 409
#define PPUTLUTRAITS_817  816, 818, 9, 408, 610, 28, 865, 1, 1, 1, 1, 17, 49, 19, 43
#define PPUTLUTRAITS_816  815, 817, 9, 408, 608, 28, 256, 0, 0, 0, 0, 16, 48, 2, 2, 2, 2, 3, 17
#define PPUTLUTRAITS_815  814, 816, 9, 407, 606, 28, 673, 1, 3, 7, 15, 15, 47, 5, 163
#define PPUTLUTRAITS_814  813, 815, 9, 407, 604, 28, 68, 0, 2, 6, 14, 14, 46, 2, 11, 37
#define PPUTLUTRAITS_813  812, 814, 9, 406, 602, 28, 489, 1, 1, 5, 13, 13, 45, 3, 271
#define PPUTLUTRAITS_812  811, 813, 9, 406, 600, 28, 912, 0, 0, 4, 12, 12, 44, 2, 2, 7, 29
#define PPUTLUTRAITS_811  810, 812, 9, 405, 598, 28, 313, 1, 3, 3, 11, 11, 43,
#define PPUTLUTRAITS_810  809, 811, 9, 405, 596, 28, 740, 0, 2, 2, 10, 10, 42, 2, 3, 3, 3, 3, 5
#define PPUTLUTRAITS_809  808, 810, 9, 404, 594, 28, 145, 1, 1, 1, 9, 9, 41,
#define PPUTLUTRAITS_808  807, 809, 9, 404, 592, 28, 576, 0, 0, 0, 8, 8, 40, 2, 2, 2, 101
#define PPUTLUTRAITS_807  806, 808, 9, 403, 590, 28, 1009, 1, 3, 7, 7, 7, 39, 3, 269
#define PPUTLUTRAITS_806  805, 807, 9, 403, 588, 28, 420, 0, 2, 6, 6, 6, 38, 2, 13, 31
#define PPUTLUTRAITS_805  804, 806, 9, 402, 586, 28, 857, 1, 1, 5, 5, 5, 37, 5, 7, 23
#define PPUTLUTRAITS_804  803, 805, 9, 402, 584, 28, 272, 0, 0, 4, 4, 4, 36, 2, 2, 3, 67
#define PPUTLUTRAITS_803  802, 804, 9, 401, 582, 28, 713, 1, 3, 3, 3, 3, 35, 11, 73
#define PPUTLUTRAITS_802  801, 803, 9, 401, 580, 28, 132, 0, 2, 2, 2, 2, 34, 2, 401
#define PPUTLUTRAITS_801  800, 802, 9, 400, 578, 28, 577, 1, 1, 1, 1, 1, 33, 3, 3, 89
#define PPUTLUTRAITS_800  799, 801, 9, 400, 576, 28, 0, 0, 0, 0, 0, 0, 32, 2, 2, 2, 2, 2, 5, 5
#define PPUTLUTRAITS_799  798, 800, 9, 399, 574, 28, 449, 1, 3, 7, 15, 31, 31, 17, 47
#define PPUTLUTRAITS_798  797, 799, 9, 399, 572, 28, 900, 0, 2, 6, 14, 30, 30, 2, 3, 7, 19
#define PPUTLUTRAITS_797  796, 798, 9, 398, 570, 28, 329, 1, 1, 5, 13, 29, 29,
#define PPUTLUTRAITS_796  795, 797, 9, 398, 568, 28, 784, 0, 0, 4, 12, 28, 28, 2, 2, 199
#define PPUTLUTRAITS_795  794, 796, 9, 397, 566, 28, 217, 1, 3, 3, 11, 27, 27, 3, 5, 53
#define PPUTLUTRAITS_794  793, 795, 9, 397, 564, 28, 676, 0, 2, 2, 10, 26, 26, 2, 397
#define PPUTLUTRAITS_793  792, 794, 9, 396, 562, 28, 113, 1, 1, 1, 9, 25, 25, 13, 61
#define PPUTLUTRAITS_792  791, 793, 9, 396, 560, 28, 576, 0, 0, 0, 8, 24, 24, 2, 2, 2, 3, 3, 11
#define PPUTLUTRAITS_791  790, 792, 9, 395, 558, 28, 17, 1, 3, 7, 7, 23, 23, 7, 113
#define PPUTLUTRAITS_790  789, 791, 9, 395, 556, 28, 484, 0, 2, 6, 6, 22, 22, 2, 5, 79
#define PPUTLUTRAITS_789  788, 790, 9, 394, 554, 28, 953, 1, 1, 5, 5, 21, 21, 3, 263
#define PPUTLUTRAITS_788  787, 789, 9, 394, 552, 28, 400, 0, 0, 4, 4, 20, 20, 2, 2, 197
#define PPUTLUTRAITS_787  786, 788, 9, 393, 550, 28, 873, 1, 3, 3, 3, 19, 19,
#define PPUTLUTRAITS_786  785, 787, 9, 393, 548, 28, 324, 0, 2, 2, 2, 18, 18, 2, 3, 131
#define PPUTLUTRAITS_785  784, 786, 9, 392, 546, 28, 801, 1, 1, 1, 1, 17, 17, 5, 157
#define PPUTLUTRAITS_784  783, 785, 9, 392, 544, 28, 256, 0, 0, 0, 0, 16, 16, 2, 2, 2, 2, 7, 7
#define PPUTLUTRAITS_783  782, 784, 9, 391, 542, 27, 737, 1, 3, 7, 15, 15, 15, 3, 3, 3, 29
#define PPUTLUTRAITS_782  781, 783, 9, 391, 540, 27, 196, 0, 2, 6, 14, 14, 14, 2, 17, 23
#define PPUTLUTRAITS_781  780, 782, 9, 390, 538, 27, 681, 1, 1, 5, 13, 13, 13, 11, 71
#define PPUTLUTRAITS_780  779, 781, 9, 390, 536, 27, 144, 0, 0, 4, 12, 12, 12, 2, 2, 3, 5, 13
#define PPUTLUTRAITS_779  778, 780, 9, 389, 534, 27, 633, 1, 3, 3, 11, 11, 11, 19, 41
#define PPUTLUTRAITS_778  777, 779, 9, 389, 532, 27, 100, 0, 2, 2, 10, 10, 10, 2, 389
#define PPUTLUTRAITS_777  776, 778, 9, 388, 530, 27, 593, 1, 1, 1, 9, 9, 9, 3, 7, 37
#define PPUTLUTRAITS_776  775, 777, 9, 388, 528, 27, 64, 0, 0, 0, 8, 8, 8, 2, 2, 2, 97
#define PPUTLUTRAITS_775  774, 776, 9, 387, 526, 27, 561, 1, 3, 7, 7, 7, 7, 5, 5, 31
#define PPUTLUTRAITS_774  773, 775, 9, 387, 524, 27, 36, 0, 2, 6, 6, 6, 6, 2, 3, 3, 43
#define PPUTLUTRAITS_773  772, 774, 9, 386, 522, 27, 537, 1, 1, 5, 5, 5, 5,
#define PPUTLUTRAITS_772  771, 773, 9, 386, 520, 27, 16, 0, 0, 4, 4, 4, 4, 2, 2, 193
#define PPUTLUTRAITS_771  770, 772, 9, 385, 518, 27, 521, 1, 3, 3, 3, 3, 3, 3, 257
#define PPUTLUTRAITS_770  769, 771, 9, 385, 516, 27, 4, 0, 2, 2, 2, 2, 2, 2, 5, 7, 11
#define PPUTLUTRAITS_769  768, 770, 9, 384, 514, 27, 513, 1, 1, 1, 1, 1, 1,
#define PPUTLUTRAITS_768  767, 769, 9, 384, 512, 27, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 3
#define PPUTLUTRAITS_767  766, 768, 9, 383, 510, 27, 513, 1, 3, 7, 15, 31, 63, 13, 59
#define PPUTLUTRAITS_766  765, 767, 9, 383, 508, 27, 4, 0, 2, 6, 14, 30, 62, 2, 383
#define PPUTLUTRAITS_765  764, 766, 9, 382, 506, 27, 521, 1, 1, 5, 13, 29, 61, 3, 3, 5, 17
#define PPUTLUTRAITS_764  763, 765, 9, 382, 504, 27, 16, 0, 0, 4, 12, 28, 60, 2, 2, 191
#define PPUTLUTRAITS_763  762, 764, 9, 381, 502, 27, 537, 1, 3, 3, 11, 27, 59, 7, 109
#define PPUTLUTRAITS_762  761, 763, 9, 381, 500, 27, 36, 0, 2, 2, 10, 26, 58, 2, 3, 127
#define PPUTLUTRAITS_761  760, 762, 9, 380, 498, 27, 561, 1, 1, 1, 9, 25, 57,
#define PPUTLUTRAITS_760  759, 761, 9, 380, 496, 27, 64, 0, 0, 0, 8, 24, 56, 2, 2, 2, 5, 19
#define PPUTLUTRAITS_759  758, 760, 9, 379, 494, 27, 593, 1, 3, 7, 7, 23, 55, 3, 11, 23
#define PPUTLUTRAITS_758  757, 759, 9, 379, 492, 27, 100, 0, 2, 6, 6, 22, 54, 2, 379
#define PPUTLUTRAITS_757  756, 758, 9, 378, 490, 27, 633, 1, 1, 5, 5, 21, 53,
#define PPUTLUTRAITS_756  755, 757, 9, 378, 488, 27, 144, 0, 0, 4, 4, 20, 52, 2, 2, 3, 3, 3, 7
#define PPUTLUTRAITS_755  754, 756, 9, 377, 486, 27, 681, 1, 3, 3, 3, 19, 51, 5, 151
#define PPUTLUTRAITS_754  753, 755, 9, 377, 484, 27, 196, 0, 2, 2, 2, 18, 50, 2, 13, 29
#define PPUTLUTRAITS_753  752, 754, 9, 376, 482, 27, 737, 1, 1, 1, 1, 17, 49, 3, 251
#define PPUTLUTRAITS_752  751, 753, 9, 376, 480, 27, 256, 0, 0, 0, 0, 16, 48, 2, 2, 2, 2, 47
#define PPUTLUTRAITS_751  750, 752, 9, 375, 478, 27, 801, 1, 3, 7, 15, 15, 47,
#define PPUTLUTRAITS_750  749, 751, 9, 375, 476, 27, 324, 0, 2, 6, 14, 14, 46, 2, 3, 5, 5, 5
#define PPUTLUTRAITS_749  748, 750, 9, 374, 474, 27, 873, 1, 1, 5, 13, 13, 45, 7, 107
#define PPUTLUTRAITS_748  747, 749, 9, 374, 472, 27, 400, 0, 0, 4, 12, 12, 44, 2, 2, 11, 17
#define PPUTLUTRAITS_747  746, 748, 9, 373, 470, 27, 953, 1, 3, 3, 11, 11, 43, 3, 3, 83
#define PPUTLUTRAITS_746  745, 747, 9, 373, 468, 27, 484, 0, 2, 2, 10, 10, 42, 2, 373
#define PPUTLUTRAITS_745  744, 746, 9, 372, 466, 27, 17, 1, 1, 1, 9, 9, 41, 5, 149
#define PPUTLUTRAITS_744  743, 745, 9, 372, 464, 27, 576, 0, 0, 0, 8, 8, 40, 2, 2, 2, 3, 31
#define PPUTLUTRAITS_743  742, 744, 9, 371, 462, 27, 113, 1, 3, 7, 7, 7, 39,
#define PPUTLUTRAITS_742  741, 743, 9, 371, 460, 27, 676, 0, 2, 6, 6, 6, 38, 2, 7, 53
#define PPUTLUTRAITS_741  740, 742, 9, 370, 458, 27, 217, 1, 1, 5, 5, 5, 37, 3, 13, 19
#define PPUTLUTRAITS_740  739, 741, 9, 370, 456, 27, 784, 0, 0, 4, 4, 4, 36, 2, 2, 5, 37
#define PPUTLUTRAITS_739  738, 740, 9, 369, 454, 27, 329, 1, 3, 3, 3, 3, 35,
#define PPUTLUTRAITS_738  737, 739, 9, 369, 452, 27, 900, 0, 2, 2, 2, 2, 34, 2, 3, 3, 41
#define PPUTLUTRAITS_737  736, 738, 9, 368, 450, 27, 449, 1, 1, 1, 1, 1, 33, 11, 67
#define PPUTLUTRAITS_736  735, 737, 9, 368, 448, 27, 0, 0, 0, 0, 0, 0, 32, 2, 2, 2, 2, 2, 23
#define PPUTLUTRAITS_735  734, 736, 9, 367, 446, 27, 577, 1, 3, 7, 15, 31, 31, 3, 5, 7, 7
#define PPUTLUTRAITS_734  733, 735, 9, 367, 444, 27, 132, 0, 2, 6, 14, 30, 30, 2, 367
#define PPUTLUTRAITS_733  732, 734, 9, 366, 442, 27, 713, 1, 1, 5, 13, 29, 29,
#define PPUTLUTRAITS_732  731, 733, 9, 366, 440, 27, 272, 0, 0, 4, 12, 28, 28, 2, 2, 3, 61
#define PPUTLUTRAITS_731  730, 732, 9, 365, 438, 27, 857, 1, 3, 3, 11, 27, 27, 17, 43
#define PPUTLUTRAITS_730  729, 731, 9, 365, 436, 27, 420, 0, 2, 2, 10, 26, 26, 2, 5, 73
#define PPUTLUTRAITS_729  728, 730, 9, 364, 434, 27, 1009, 1, 1, 1, 9, 25, 25, 3, 3, 3, 3, 3, 3
#define PPUTLUTRAITS_728  727, 729, 9, 364, 432, 26, 576, 0, 0, 0, 8, 24, 24, 2, 2, 2, 7, 13
#define PPUTLUTRAITS_727  726, 728, 9, 363, 430, 26, 145, 1, 3, 7, 7, 23, 23,
#define PPUTLUTRAITS_726  725, 727, 9, 363, 428, 26, 740, 0, 2, 6, 6, 22, 22, 2, 3, 11, 11
#define PPUTLUTRAITS_725  724, 726, 9, 362, 426, 26, 313, 1, 1, 5, 5, 21, 21, 5, 5, 29
#define PPUTLUTRAITS_724  723, 725, 9, 362, 424, 26, 912, 0, 0, 4, 4, 20, 20, 2, 2, 181
#define PPUTLUTRAITS_723  722, 724, 9, 361, 422, 26, 489, 1, 3, 3, 3, 19, 19, 3, 241
#define PPUTLUTRAITS_722  721, 723, 9, 361, 420, 26, 68, 0, 2, 2, 2, 18, 18, 2, 19, 19
#define PPUTLUTRAITS_721  720, 722, 9, 360, 418, 26, 673, 1, 1, 1, 1, 17, 17, 7, 103
#define PPUTLUTRAITS_720  719, 721, 9, 360, 416, 26, 256, 0, 0, 0, 0, 16, 16, 2, 2, 2, 2, 3, 3, 5
#define PPUTLUTRAITS_719  718, 720, 9, 359, 414, 26, 865, 1, 3, 7, 15, 15, 15,
#define PPUTLUTRAITS_718  717, 719, 9, 359, 412, 26, 452, 0, 2, 6, 14, 14, 14, 2, 359
#define PPUTLUTRAITS_717  716, 718, 9, 358, 410, 26, 41, 1, 1, 5, 13, 13, 13, 3, 239
#define PPUTLUTRAITS_716  715, 717, 9, 358, 408, 26, 656, 0, 0, 4, 12, 12, 12, 2, 2, 179
#define PPUTLUTRAITS_715  714, 716, 9, 357, 406, 26, 249, 1, 3, 3, 11, 11, 11, 5, 11, 13
#define PPUTLUTRAITS_714  713, 715, 9, 357, 404, 26, 868, 0, 2, 2, 10, 10, 10, 2, 3, 7, 17
#define PPUTLUTRAITS_713  712, 714, 9, 356, 402, 26, 465, 1, 1, 1, 9, 9, 9, 23, 31
#define PPUTLUTRAITS_712  711, 713, 9, 356, 400, 26, 64, 0, 0, 0, 8, 8, 8, 2, 2, 2, 89
#define PPUTLUTRAITS_711  710, 712, 9, 355, 398, 26, 689, 1, 3, 7, 7, 7, 7, 3, 3, 79
#define PPUTLUTRAITS_710  709, 711, 9, 355, 396, 26, 292, 0, 2, 6, 6, 6, 6, 2, 5, 71
#define PPUTLUTRAITS_709  708, 710, 9, 354, 394, 26, 921, 1, 1, 5, 5, 5, 5,
#define PPUTLUTRAITS_708  707, 709, 9, 354, 392, 26, 528, 0, 0, 4, 4, 4, 4, 2, 2, 3, 59
#define PPUTLUTRAITS_707  706, 708, 9, 353, 390, 26, 137, 1, 3, 3, 3, 3, 3, 7, 101
#define PPUTLUTRAITS_706  705, 707, 9, 353, 388, 26, 772, 0, 2, 2, 2, 2, 2, 2, 353
#define PPUTLUTRAITS_705  704, 706, 9, 352, 386, 26, 385, 1, 1, 1, 1, 1, 1, 3, 5, 47
#define PPUTLUTRAITS_704  703, 705, 9, 352, 384, 26, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 11
#define PPUTLUTRAITS_703  702, 704, 9, 351, 382, 26, 641, 1, 3, 7, 15, 31, 63, 19, 37
#define PPUTLUTRAITS_702  701, 703, 9, 351, 380, 26, 260, 0, 2, 6, 14, 30, 62, 2, 3, 3, 3, 13
#define PPUTLUTRAITS_701  700, 702, 9, 350, 378, 26, 905, 1, 1, 5, 13, 29, 61,
#define PPUTLUTRAITS_700  699, 701, 9, 350, 376, 26, 528, 0, 0, 4, 12, 28, 60, 2, 2, 5, 5, 7
#define PPUTLUTRAITS_699  698, 700, 9, 349, 374, 26, 153, 1, 3, 3, 11, 27, 59, 3, 233
#define PPUTLUTRAITS_698  697, 699, 9, 349, 372, 26, 804, 0, 2, 2, 10, 26, 58, 2, 349
#define PPUTLUTRAITS_697  696, 698, 9, 348, 370, 26, 433, 1, 1, 1, 9, 25, 57, 17, 41
#define PPUTLUTRAITS_696  695, 697, 9, 348, 368, 26, 64, 0, 0, 0, 8, 24, 56, 2, 2, 2, 3, 29
#define PPUTLUTRAITS_695  694, 696, 9, 347, 366, 26, 721, 1, 3, 7, 7, 23, 55, 5, 139
#define PPUTLUTRAITS_694  693, 695, 9, 347, 364, 26, 356, 0, 2, 6, 6, 22, 54, 2, 347
#define PPUTLUTRAITS_693  692, 694, 9, 346, 362, 26, 1017, 1, 1, 5, 5, 21, 53, 3, 3, 7, 11
#define PPUTLUTRAITS_692  691, 693, 9, 346, 360, 26, 656, 0, 0, 4, 4, 20, 52, 2, 2, 173
#define PPUTLUTRAITS_691  690, 692, 9, 345, 358, 26, 297, 1, 3, 3, 3, 19, 51,
#define PPUTLUTRAITS_690  689, 691, 9, 345, 356, 26, 964, 0, 2, 2, 2, 18, 50, 2, 3, 5, 23
#define PPUTLUTRAITS_689  688, 690, 9, 344, 354, 26, 609, 1, 1, 1, 1, 17, 49, 13, 53
#define PPUTLUTRAITS_688  687, 689, 9, 344, 352, 26, 256, 0, 0, 0, 0, 16, 48, 2, 2, 2, 2, 43
#define PPUTLUTRAITS_687  686, 688, 9, 343, 350, 26, 929, 1, 3, 7, 15, 15, 47, 3, 229
#define PPUTLUTRAITS_686  685, 687, 9, 343, 348, 26, 580, 0, 2, 6, 14, 14, 46, 2, 7, 7, 7
#define PPUTLUTRAITS_685  684, 686, 9, 342, 346, 26, 233, 1, 1, 5, 13, 13, 45, 5, 137
#define PPUTLUTRAITS_684  683, 685, 9, 342, 344, 26, 912, 0, 0, 4, 12, 12, 44, 2, 2, 3, 3, 19
#define PPUTLUTRAITS_683  682, 684, 9, 341, 342, 26, 569, 1, 3, 3, 11, 11, 43,
#define PPUTLUTRAITS_682  681, 683, 9, 341, 340, 26, 228, 0, 2, 2, 10, 10, 42, 2, 11, 31
#define PPUTLUTRAITS_681  680, 682, 9, 340, 338, 26, 913, 1, 1, 1, 9, 9, 41, 3, 227
#define PPUTLUTRAITS_680  679, 681, 9, 340, 336, 26, 576, 0, 0, 0, 8, 8, 40, 2, 2, 2, 5, 17
#define PPUTLUTRAITS_679  678, 680, 9, 339, 334, 26, 241, 1, 3, 7, 7, 7, 39, 7, 97
#define PPUTLUTRAITS_678  677, 679, 9, 339, 332, 26, 932, 0, 2, 6, 6, 6, 38, 2, 3, 113
#define PPUTLUTRAITS_677  676, 678, 9, 338, 330, 26, 601, 1, 1, 5, 5, 5, 37,
#define PPUTLUTRAITS_676  675, 677, 9, 338, 328, 26, 272, 0, 0, 4, 4, 4, 36, 2, 2, 13, 13
#define PPUTLUTRAITS_675  674, 676, 9, 337, 326, 25, 969, 1, 3, 3, 3, 3, 35, 3, 3, 3, 5, 5
#define PPUTLUTRAITS_674  673, 675, 9, 337, 324, 25, 644, 0, 2, 2, 2, 2, 34, 2, 337
#define PPUTLUTRAITS_673  672, 674, 9, 336, 322, 25, 321, 1, 1, 1, 1, 1, 33,
#define PPUTLUTRAITS_672  671, 673, 9, 336, 320, 25, 0, 0, 0, 0, 0, 0, 32, 2, 2, 2, 2, 2, 3, 7
#define PPUTLUTRAITS_671  670, 672, 9, 335, 318, 25, 705, 1, 3, 7, 15, 31, 31, 11, 61
#define PPUTLUTRAITS_670  669, 671, 9, 335, 316, 25, 388, 0, 2, 6, 14, 30, 30, 2, 5, 67
#define PPUTLUTRAITS_669  668, 670, 9, 334, 314, 25, 73, 1, 1, 5, 13, 29, 29, 3, 223
#define PPUTLUTRAITS_668  667, 669, 9, 334, 312, 25, 784, 0, 0, 4, 12, 28, 28, 2, 2, 167
#define PPUTLUTRAITS_667  666, 668, 9, 333, 310, 25, 473, 1, 3, 3, 11, 27, 27, 23, 29
#define PPUTLUTRAITS_666  665, 667, 9, 333, 308, 25, 164, 0, 2, 2, 10, 26, 26, 2, 3, 3, 37
#define PPUTLUTRAITS_665  664, 666, 9, 332, 306, 25, 881, 1, 1, 1, 9, 25, 25, 5, 7, 19
#define PPUTLUTRAITS_664  663, 665, 9, 332, 304, 25, 576, 0, 0, 0, 8, 24, 24, 2, 2, 2, 83
#define PPUTLUTRAITS_663  662, 664, 9, 331, 302, 25, 273, 1, 3, 7, 7, 23, 23, 3, 13, 17
#define PPUTLUTRAITS_662  661, 663, 9, 331, 300, 25, 996, 0, 2, 6, 6, 22, 22, 2, 331
#define PPUTLUTRAITS_661  660, 662, 9, 330, 298, 25, 697, 1, 1, 5, 5, 21, 21,
#define PPUTLUTRAITS_660  659, 661, 9, 330, 296, 25, 400, 0, 0, 4, 4, 20, 20, 2, 2, 3, 5, 11
#define PPUTLUTRAITS_659  658, 660, 9, 329, 294, 25, 105, 1, 3, 3, 3, 19, 19,
#define PPUTLUTRAITS_658  657, 659, 9, 329, 292, 25, 836, 0, 2, 2, 2, 18, 18, 2, 7, 47
#define PPUTLUTRAITS_657  656, 658, 9, 328, 290, 25, 545, 1, 1, 1, 1, 17, 17, 3, 3, 73
#define PPUTLUTRAITS_656  655, 657, 9, 328, 288, 25, 256, 0, 0, 0, 0, 16, 16, 2, 2, 2, 2, 41
#define PPUTLUTRAITS_655  654, 656, 9, 327, 286, 25, 993, 1, 3, 7, 15, 15, 15, 5, 131
#define PPUTLUTRAITS_654  653, 655, 9, 327, 284, 25, 708, 0, 2, 6, 14, 14, 14, 2, 3, 109
#define PPUTLUTRAITS_653  652, 654, 9, 326, 282, 25, 425, 1, 1, 5, 13, 13, 13,
#define PPUTLUTRAITS_652  651, 653, 9, 326, 280, 25, 144, 0, 0, 4, 12, 12, 12, 2, 2, 163
#define PPUTLUTRAITS_651  650, 652, 9, 325, 278, 25, 889, 1, 3, 3, 11, 11, 11, 3, 7, 31
#define PPUTLUTRAITS_650  649, 651, 9, 325, 276, 25, 612, 0, 2, 2, 10, 10, 10, 2, 5, 5, 13
#define PPUTLUTRAITS_649  648, 650, 9, 324, 274, 25, 337, 1, 1, 1, 9, 9, 9, 11, 59
#define PPUTLUTRAITS_648  647, 649, 9, 324, 272, 25, 64, 0, 0, 0, 8, 8, 8, 2, 2, 2, 3, 3, 3, 3
#define PPUTLUTRAITS_647  646, 648, 9, 323, 270, 25, 817, 1, 3, 7, 7, 7, 7,
#define PPUTLUTRAITS_646  645, 647, 9, 323, 268, 25, 548, 0, 2, 6, 6, 6, 6, 2, 17, 19
#define PPUTLUTRAITS_645  644, 646, 9, 322, 266, 25, 281, 1, 1, 5, 5, 5, 5, 3, 5, 43
#define PPUTLUTRAITS_644  643, 645, 9, 322, 264, 25, 16, 0, 0, 4, 4, 4, 4, 2, 2, 7, 23
#define PPUTLUTRAITS_643  642, 644, 9, 321, 262, 25, 777, 1, 3, 3, 3, 3, 3,
#define PPUTLUTRAITS_642  641, 643, 9, 321, 260, 25, 516, 0, 2, 2, 2, 2, 2, 2, 3, 107
#define PPUTLUTRAITS_641  640, 642, 9, 320, 258, 25, 257, 1, 1, 1, 1, 1, 1,
#define PPUTLUTRAITS_640  639, 641, 9, 320, 256, 25, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 5
#define PPUTLUTRAITS_639  638, 640, 9, 319, 254, 25, 769, 1, 3, 7, 15, 31, 63, 3, 3, 71
#define PPUTLUTRAITS_638  637, 639, 9, 319, 252, 25, 516, 0, 2, 6, 14, 30, 62, 2, 11, 29
#define PPUTLUTRAITS_637  636, 638, 9, 318, 250, 25, 265, 1, 1, 5, 13, 29, 61, 7, 7, 13
#define PPUTLUTRAITS_636  635, 637, 9, 318, 248, 25, 16, 0, 0, 4, 12, 28, 60, 2, 2, 3, 53
#define PPUTLUTRAITS_635  634, 636, 9, 317, 246, 25, 793, 1, 3, 3, 11, 27, 59, 5, 127
#define PPUTLUTRAITS_634  633, 635, 9, 317, 244, 25, 548, 0, 2, 2, 10, 26, 58, 2, 317
#define PPUTLUTRAITS_633  632, 634, 9, 316, 242, 25, 305, 1, 1, 1, 9, 25, 57, 3, 211
#define PPUTLUTRAITS_632  631, 633, 9, 316, 240, 25, 64, 0, 0, 0, 8, 24, 56, 2, 2, 2, 79
#define PPUTLUTRAITS_631  630, 632, 9, 315, 238, 25, 849, 1, 3, 7, 7, 23, 55,
#define PPUTLUTRAITS_630  629, 631, 9, 315, 236, 25, 612, 0, 2, 6, 6, 22, 54, 2, 3, 3, 5, 7
#define PPUTLUTRAITS_629  628, 630, 9, 314, 234, 25, 377, 1, 1, 5, 5, 21, 53, 17, 37
#define PPUTLUTRAITS_628  627, 629, 9, 314, 232, 25, 144, 0, 0, 4, 4, 20, 52, 2, 2, 157
#define PPUTLUTRAITS_627  626, 628, 9, 313, 230, 25, 937, 1, 3, 3, 3, 19, 51, 3, 11, 19
#define PPUTLUTRAITS_626  625, 627, 9, 313, 228, 25, 708, 0, 2, 2, 2, 18, 50, 2, 313
#define PPUTLUTRAITS_625  624, 626, 9, 312, 226, 25, 481, 1, 1, 1, 1, 17, 49, 5, 5, 5, 5
#define PPUTLUTRAITS_624  623, 625, 9, 312, 224, 24, 256, 0, 0, 0, 0, 16, 48, 2, 2, 2, 2, 3, 13
#define PPUTLUTRAITS_623  622, 624, 9, 311, 222, 24, 33, 1, 3, 7, 15, 15, 47, 7, 89
#define PPUTLUTRAITS_622  621, 623, 9, 311, 220, 24, 836, 0, 2, 6, 14, 14, 46, 2, 311
#define PPUTLUTRAITS_621  620, 622, 9, 310, 218, 24, 617, 1, 1, 5, 13, 13, 45, 3, 3, 3, 23
#define PPUTLUTRAITS_620  619, 621, 9, 310, 216, 24, 400, 0, 0, 4, 12, 12, 44, 2, 2, 5, 31
#define PPUTLUTRAITS_619  618, 620, 9, 309, 214, 24, 185, 1, 3, 3, 11, 11, 43,
#define PPUTLUTRAITS_618  617, 619, 9, 309, 212, 24, 996, 0, 2, 2, 10, 10, 42, 2, 3, 103
#define PPUTLUTRAITS_617  616, 618, 9, 308, 210, 24, 785, 1, 1, 1, 9, 9, 41,
#define PPUTLUTRAITS_616  615, 617, 9, 308, 208, 24, 576, 0, 0, 0, 8, 8, 40, 2, 2, 2, 7, 11
#define PPUTLUTRAITS_615  614, 616, 9, 307, 206, 24, 369, 1, 3, 7, 7, 7, 39, 3, 5, 41
#define PPUTLUTRAITS_614  613, 615, 9, 307, 204, 24, 164, 0, 2, 6, 6, 6, 38, 2, 307
#define PPUTLUTRAITS_613  612, 614, 9, 306, 202, 24, 985, 1, 1, 5, 5, 5, 37,
#define PPUTLUTRAITS_612  611, 613, 9, 306, 200, 24, 784, 0, 0, 4, 4, 4, 36, 2, 2, 3, 3, 17
#define PPUTLUTRAITS_611  610, 612, 9, 305, 198, 24, 585, 1, 3, 3, 3, 3, 35, 13, 47
#define PPUTLUTRAITS_610  609, 611, 9, 305, 196, 24, 388, 0, 2, 2, 2, 2, 34, 2, 5, 61
#define PPUTLUTRAITS_609  608, 610, 9, 304, 194, 24, 193, 1, 1, 1, 1, 1, 33, 3, 7, 29
#define PPUTLUTRAITS_608  607, 609, 9, 304, 192, 24, 0, 0, 0, 0, 0, 0, 32, 2, 2, 2, 2, 2, 19
#define PPUTLUTRAITS_607  606, 608, 9, 303, 190, 24, 833, 1, 3, 7, 15, 31, 31,
#define PPUTLUTRAITS_606  605, 607, 9, 303, 188, 24, 644, 0, 2, 6, 14, 30, 30, 2, 3, 101
#define PPUTLUTRAITS_605  604, 606, 9, 302, 186, 24, 457, 1, 1, 5, 13, 29, 29, 5, 11, 11
#define PPUTLUTRAITS_604  603, 605, 9, 302, 184, 24, 272, 0, 0, 4, 12, 28, 28, 2, 2, 151
#define PPUTLUTRAITS_603  602, 604, 9, 301, 182, 24, 89, 1, 3, 3, 11, 27, 27, 3, 3, 67
#define PPUTLUTRAITS_602  601, 603, 9, 301, 180, 24, 932, 0, 2, 2, 10, 26, 26, 2, 7, 43
#define PPUTLUTRAITS_601  600, 602, 9, 300, 178, 24, 753, 1, 1, 1, 9, 25, 25,
#define PPUTLUTRAITS_600  599, 601, 9, 300, 176, 24, 576, 0, 0, 0, 8, 24, 24, 2, 2, 2, 3, 5, 5
#define PPUTLUTRAITS_599  598, 600, 9, 299, 174, 24, 401, 1, 3, 7, 7, 23, 23,
#define PPUTLUTRAITS_598  597, 599, 9, 299, 172, 24, 228, 0, 2, 6, 6, 22, 22, 2, 13, 23
#define PPUTLUTRAITS_597  596, 598, 9, 298, 170, 24, 57, 1, 1, 5, 5, 21, 21, 3, 199
#define PPUTLUTRAITS_596  595, 597, 9, 298, 168, 24, 912, 0, 0, 4, 4, 20, 20, 2, 2, 149
#define PPUTLUTRAITS_595  594, 596, 9, 297, 166, 24, 745, 1, 3, 3, 3, 19, 19, 5, 7, 17
#define PPUTLUTRAITS_594  593, 595, 9, 297, 164, 24, 580, 0, 2, 2, 2, 18, 18, 2, 3, 3, 3, 11
#define PPUTLUTRAITS_593  592, 594, 9, 296, 162, 24, 417, 1, 1, 1, 1, 17, 17,
#define PPUTLUTRAITS_592  591, 593, 9, 296, 160, 24, 256, 0, 0, 0, 0, 16, 16, 2, 2, 2, 2, 37
#define PPUTLUTRAITS_591  590, 592, 9, 295, 158, 24, 97, 1, 3, 7, 15, 15, 15, 3, 197
#define PPUTLUTRAITS_590  589, 591, 9, 295, 156, 24, 964, 0, 2, 6, 14, 14, 14, 2, 5, 59
#define PPUTLUTRAITS_589  588, 590, 9, 294, 154, 24, 809, 1, 1, 5, 13, 13, 13, 19, 31
#define PPUTLUTRAITS_588  587, 589, 9, 294, 152, 24, 656, 0, 0, 4, 12, 12, 12, 2, 2, 3, 7, 7
#define PPUTLUTRAITS_587  586, 588, 9, 293, 150, 24, 505, 1, 3, 3, 11, 11, 11,
#define PPUTLUTRAITS_586  585, 587, 9, 293, 148, 24, 356, 0, 2, 2, 10, 10, 10, 2, 293
#define PPUTLUTRAITS_585  584, 586, 9, 292, 146, 24, 209, 1, 1, 1, 9, 9, 9, 3, 3, 5, 13
#define PPUTLUTRAITS_584  583, 585, 9, 292, 144, 24, 64, 0, 0, 0, 8, 8, 8, 2, 2, 2, 73
#define PPUTLUTRAITS_583  582, 584, 9, 291, 142, 24, 945, 1, 3, 7, 7, 7, 7, 11, 53
#define PPUTLUTRAITS_582  581, 583, 9, 291, 140, 24, 804, 0, 2, 6, 6, 6, 6, 2, 3, 97
#define PPUTLUTRAITS_581  580, 582, 9, 290, 138, 24, 665, 1, 1, 5, 5, 5, 5, 7, 83
#define PPUTLUTRAITS_580  579, 581, 9, 290, 136, 24, 528, 0, 0, 4, 4, 4, 4, 2, 2, 5, 29
#define PPUTLUTRAITS_579  578, 580, 9, 289, 134, 24, 393, 1, 3, 3, 3, 3, 3, 3, 193
#define PPUTLUTRAITS_578  577, 579, 9, 289, 132, 24, 260, 0, 2, 2, 2, 2, 2, 2, 17, 17
#define PPUTLUTRAITS_577  576, 578, 9, 288, 130, 24, 129, 1, 1, 1, 1, 1, 1,
#define PPUTLUTRAITS_576  575, 577, 9, 288, 128, 24, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 3, 3
#define PPUTLUTRAITS_575  574, 576, 9, 287, 126, 23, 897, 1, 3, 7, 15, 31, 63, 5, 5, 23
#define PPUTLUTRAITS_574  573, 575, 9, 287, 124, 23, 772, 0, 2, 6, 14, 30, 62, 2, 7, 41
#define PPUTLUTRAITS_573  572, 574, 9, 286, 122, 23, 649, 1, 1, 5, 13, 29, 61, 3, 191
#define PPUTLUTRAITS_572  571, 573, 9, 286, 120, 23, 528, 0, 0, 4, 12, 28, 60, 2, 2, 11, 13
#define PPUTLUTRAITS_571  570, 572, 9, 285, 118, 23, 409, 1, 3, 3, 11, 27, 59,
#define PPUTLUTRAITS_570  569, 571, 9, 285, 116, 23, 292, 0, 2, 2, 10, 26, 58, 2, 3, 5, 19
#define PPUTLUTRAITS_569  568, 570, 9, 284, 114, 23, 177, 1, 1, 1, 9, 25, 57,
#define PPUTLUTRAITS_568  567, 569, 9, 284, 112, 23, 64, 0, 0, 0, 8, 24, 56, 2, 2, 2, 71
#define PPUTLUTRAITS_567  566, 568, 9, 283, 110, 23, 977, 1, 3, 7, 7, 23, 55, 3, 3, 3, 3, 7
#define PPUTLUTRAITS_566  565, 567, 9, 283, 108, 23, 868, 0, 2, 6, 6, 22, 54, 2, 283
#define PPUTLUTRAITS_565  564, 566, 9, 282, 106, 23, 761, 1, 1, 5, 5, 21, 53, 5, 113
#define PPUTLUTRAITS_564  563, 565, 9, 282, 104, 23, 656, 0, 0, 4, 4, 20, 52, 2, 2, 3, 47
#define PPUTLUTRAITS_563  562, 564, 9, 281, 102, 23, 553, 1, 3, 3, 3, 19, 51,
#define PPUTLUTRAITS_562  561, 563, 9, 281, 100, 23, 452, 0, 2, 2, 2, 18, 50, 2, 281
#define PPUTLUTRAITS_561  560, 562, 9, 280, 98, 23, 353, 1, 1, 1, 1, 17, 49, 3, 11, 17
#define PPUTLUTRAITS_560  559, 561, 9, 280, 96, 23, 256, 0, 0, 0, 0, 16, 48, 2, 2, 2, 2, 5, 7
#define PPUTLUTRAITS_559  558, 560, 9, 279, 94, 23, 161, 1, 3, 7, 15, 15, 47, 13, 43
#define PPUTLUTRAITS_558  557, 559, 9, 279, 92, 23, 68, 0, 2, 6, 14, 14, 46, 2, 3, 3, 31
#define PPUTLUTRAITS_557  556, 558, 9, 278, 90, 23, 1001, 1, 1, 5, 13, 13, 45,
#define PPUTLUTRAITS_556  555, 557, 9, 278, 88, 23, 912, 0, 0, 4, 12, 12, 44, 2, 2, 139
#define PPUTLUTRAITS_555  554, 556, 9, 277, 86, 23, 825, 1, 3, 3, 11, 11, 43, 3, 5, 37
#define PPUTLUTRAITS_554  553, 555, 9, 277, 84, 23, 740, 0, 2, 2, 10, 10, 42, 2, 277
#define PPUTLUTRAITS_553  552, 554, 9, 276, 82, 23, 657, 1, 1, 1, 9, 9, 41, 7, 79
#define PPUTLUTRAITS_552  551, 553, 9, 276, 80, 23, 576, 0, 0, 0, 8, 8, 40, 2, 2, 2, 3, 23
#define PPUTLUTRAITS_551  550, 552, 9, 275, 78, 23, 497, 1, 3, 7, 7, 7, 39, 19, 29
#define PPUTLUTRAITS_550  549, 551, 9, 275, 76, 23, 420, 0, 2, 6, 6, 6, 38, 2, 5, 5, 11
#define PPUTLUTRAITS_549  548, 550, 9, 274, 74, 23, 345, 1, 1, 5, 5, 5, 37, 3, 3, 61
#define PPUTLUTRAITS_548  547, 549, 9, 274, 72, 23, 272, 0, 0, 4, 4, 4, 36, 2, 2, 137
#define PPUTLUTRAITS_547  546, 548, 9, 273, 70, 23, 201, 1, 3, 3, 3, 3, 35,
#define PPUTLUTRAITS_546  545, 547, 9, 273, 68, 23, 132, 0, 2, 2, 2, 2, 34, 2, 3, 7, 13
#define PPUTLUTRAITS_545  544, 546, 9, 272, 66, 23, 65, 1, 1, 1, 1, 1, 33, 5, 109
#define PPUTLUTRAITS_544  543, 545, 9, 272, 64, 23, 0, 0, 0, 0, 0, 0, 32, 2, 2, 2, 2, 2, 17
#define PPUTLUTRAITS_543  542, 544, 9, 271, 62, 23, 961, 1, 3, 7, 15, 31, 31, 3, 181
#define PPUTLUTRAITS_542  541, 543, 9, 271, 60, 23, 900, 0, 2, 6, 14, 30, 30, 2, 271
#define PPUTLUTRAITS_541  540, 542, 9, 270, 58, 23, 841, 1, 1, 5, 13, 29, 29,
#define PPUTLUTRAITS_540  539, 541, 9, 270, 56, 23, 784, 0, 0, 4, 12, 28, 28, 2, 2, 3, 3, 3, 5
#define PPUTLUTRAITS_539  538, 540, 9, 269, 54, 23, 729, 1, 3, 3, 11, 27, 27, 7, 7, 11
#define PPUTLUTRAITS_538  537, 539, 9, 269, 52, 23, 676, 0, 2, 2, 10, 26, 26, 2, 269
#define PPUTLUTRAITS_537  536, 538, 9, 268, 50, 23, 625, 1, 1, 1, 9, 25, 25, 3, 179
#define PPUTLUTRAITS_536  535, 537, 9, 268, 48, 23, 576, 0, 0, 0, 8, 24, 24, 2, 2, 2, 67
#define PPUTLUTRAITS_535  534, 536, 9, 267, 46, 23, 529, 1, 3, 7, 7, 23, 23, 5, 107
#define PPUTLUTRAITS_534  533, 535, 9, 267, 44, 23, 484, 0, 2, 6, 6, 22, 22, 2, 3, 89
#define PPUTLUTRAITS_533  532, 534, 9, 266, 42, 23, 441, 1, 1, 5, 5, 21, 21, 13, 41
#define PPUTLUTRAITS_532  531, 533, 9, 266, 40, 23, 400, 0, 0, 4, 4, 20, 20, 2, 2, 7, 19
#define PPUTLUTRAITS_531  530, 532, 9, 265, 38, 23, 361, 1, 3, 3, 3, 19, 19, 3, 3, 59
#define PPUTLUTRAITS_530  529, 531, 9, 265, 36, 23, 324, 0, 2, 2, 2, 18, 18, 2, 5, 53
#define PPUTLUTRAITS_529  528, 530, 9, 264, 34, 23, 289, 1, 1, 1, 1, 17, 17, 23, 23
#define PPUTLUTRAITS_528  527, 529, 9, 264, 32, 22, 256, 0, 0, 0, 0, 16, 16, 2, 2, 2, 2, 3, 11
#define PPUTLUTRAITS_527  526, 528, 9, 263, 30, 22, 225, 1, 3, 7, 15, 15, 15, 17, 31
#define PPUTLUTRAITS_526  525, 527, 9, 263, 28, 22, 196, 0, 2, 6, 14, 14, 14, 2, 263
#define PPUTLUTRAITS_525  524, 526, 9, 262, 26, 22, 169, 1, 1, 5, 13, 13, 13, 3, 5, 5, 7
#define PPUTLUTRAITS_524  523, 525, 9, 262, 24, 22, 144, 0, 0, 4, 12, 12, 12, 2, 2, 131
#define PPUTLUTRAITS_523  522, 524, 9, 261, 22, 22, 121, 1, 3, 3, 11, 11, 11,
#define PPUTLUTRAITS_522  521, 523, 9, 261, 20, 22, 100, 0, 2, 2, 10, 10, 10, 2, 3, 3, 29
#define PPUTLUTRAITS_521  520, 522, 9, 260, 18, 22, 81, 1, 1, 1, 9, 9, 9,
#define PPUTLUTRAITS_520  519, 521, 9, 260, 16, 22, 64, 0, 0, 0, 8, 8, 8, 2, 2, 2, 5, 13
#define PPUTLUTRAITS_519  518, 520, 9, 259, 14, 22, 49, 1, 3, 7, 7, 7, 7, 3, 173
#define PPUTLUTRAITS_518  517, 519, 9, 259, 12, 22, 36, 0, 2, 6, 6, 6, 6, 2, 7, 37
#define PPUTLUTRAITS_517  516, 518, 9, 258, 10, 22, 25, 1, 1, 5, 5, 5, 5, 11, 47
#define PPUTLUTRAITS_516  515, 517, 9, 258, 8, 22, 16, 0, 0, 4, 4, 4, 4, 2, 2, 3, 43
#define PPUTLUTRAITS_515  514, 516, 9, 257, 6, 22, 9, 1, 3, 3, 3, 3, 3, 5, 103
#define PPUTLUTRAITS_514  513, 515, 9, 257, 4, 22, 4, 0, 2, 2, 2, 2, 2, 2, 257
#define PPUTLUTRAITS_513  512, 514, 9, 256, 2, 22, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 19
#define PPUTLUTRAITS_512  511, 513, 9, 256, 0, 22, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2
#define PPUTLUTRAITS_511  510, 512, 8, 255, 1022, 22, 1, 1, 3, 7, 15, 31, 63, 7, 73
#define PPUTLUTRAITS_510  509, 511, 8, 255, 1020, 22, 4, 0, 2, 6, 14, 30, 62, 2, 3, 5, 17
#define PPUTLUTRAITS_509  508, 510, 8, 254, 1018, 22, 9, 1, 1, 5, 13, 29, 61,
#define PPUTLUTRAITS_508  507, 509, 8, 254, 1016, 22, 16, 0, 0, 4, 12, 28, 60, 2, 2, 127
#define PPUTLUTRAITS_507  506, 508, 8, 253, 1014, 22, 25, 1, 3, 3, 11, 27, 59, 3, 13, 13
#define PPUTLUTRAITS_506  505, 507, 8, 253, 1012, 22, 36, 0, 2, 2, 10, 26, 58, 2, 11, 23
#define PPUTLUTRAITS_505  504, 506, 8, 252, 1010, 22, 49, 1, 1, 1, 9, 25, 57, 5, 101
#define PPUTLUTRAITS_504  503, 505, 8, 252, 1008, 22, 64, 0, 0, 0, 8, 24, 56, 2, 2, 2, 3, 3, 7
#define PPUTLUTRAITS_503  502, 504, 8, 251, 1006, 22, 81, 1, 3, 7, 7, 23, 55,
#define PPUTLUTRAITS_502  501, 503, 8, 251, 1004, 22, 100, 0, 2, 6, 6, 22, 54, 2, 251
#define PPUTLUTRAITS_501  500, 502, 8, 250, 1002, 22, 121, 1, 1, 5, 5, 21, 53, 3, 167
#define PPUTLUTRAITS_500  499, 501, 8, 250, 1000, 22, 144, 0, 0, 4, 4, 20, 52, 2, 2, 5, 5, 5
#define PPUTLUTRAITS_499  498, 500, 8, 249, 998, 22, 169, 1, 3, 3, 3, 19, 51,
#define PPUTLUTRAITS_498  497, 499, 8, 249, 996, 22, 196, 0, 2, 2, 2, 18, 50, 2, 3, 83
#define PPUTLUTRAITS_497  496, 498, 8, 248, 994, 22, 225, 1, 1, 1, 1, 17, 49, 7, 71
#define PPUTLUTRAITS_496  495, 497, 8, 248, 992, 22, 256, 0, 0, 0, 0, 16, 48, 2, 2, 2, 2, 31
#define PPUTLUTRAITS_495  494, 496, 8, 247, 990, 22, 289, 1, 3, 7, 15, 15, 47, 3, 3, 5, 11
#define PPUTLUTRAITS_494  493, 495, 8, 247, 988, 22, 324, 0, 2, 6, 14, 14, 46, 2, 13, 19
#define PPUTLUTRAITS_493  492, 494, 8, 246, 986, 22, 361, 1, 1, 5, 13, 13, 45, 17, 29
#define PPUTLUTRAITS_492  491, 493, 8, 246, 984, 22, 400, 0, 0, 4, 12, 12, 44, 2, 2, 3, 41
#define PPUTLUTRAITS_491  490, 492, 8, 245, 982, 22, 441, 1, 3, 3, 11, 11, 43,
#define PPUTLUTRAITS_490  489, 491, 8, 245, 980, 22, 484, 0, 2, 2, 10, 10, 42, 2, 5, 7, 7
#define PPUTLUTRAITS_489  488, 490, 8, 244, 978, 22, 529, 1, 1, 1, 9, 9, 41, 3, 163
#define PPUTLUTRAITS_488  487, 489, 8, 244, 976, 22, 576, 0, 0, 0, 8, 8, 40, 2, 2, 2, 61
#define PPUTLUTRAITS_487  486, 488, 8, 243, 974, 22, 625, 1, 3, 7, 7, 7, 39,
#define PPUTLUTRAITS_486  485, 487, 8, 243, 972, 22, 676, 0, 2, 6, 6, 6, 38, 2, 3, 3, 3, 3, 3
#define PPUTLUTRAITS_485  484, 486, 8, 242, 970, 22, 729, 1, 1, 5, 5, 5, 37, 5, 97
#define PPUTLUTRAITS_484  483, 485, 8, 242, 968, 22, 784, 0, 0, 4, 4, 4, 36, 2, 2, 11, 11
#define PPUTLUTRAITS_483  482, 484, 8, 241, 966, 21, 841, 1, 3, 3, 3, 3, 35, 3, 7, 23
#define PPUTLUTRAITS_482  481, 483, 8, 241, 964, 21, 900, 0, 2, 2, 2, 2, 34, 2, 241
#define PPUTLUTRAITS_481  480, 482, 8, 240, 962, 21, 961, 1, 1, 1, 1, 1, 33, 13, 37
#define PPUTLUTRAITS_480  479, 481, 8, 240, 960, 21, 0, 0, 0, 0, 0, 0, 32, 2, 2, 2, 2, 2, 3, 5
#define PPUTLUTRAITS_479  478, 480, 8, 239, 958, 21, 65, 1, 3, 7, 15, 31, 31,
#define PPUTLUTRAITS_478  477, 479, 8, 239, 956, 21, 132, 0, 2, 6, 14, 30, 30, 2, 239
#define PPUTLUTRAITS_477  476, 478, 8, 238, 954, 21, 201, 1, 1, 5, 13, 29, 29, 3, 3, 53
#define PPUTLUTRAITS_476  475, 477, 8, 238, 952, 21, 272, 0, 0, 4, 12, 28, 28, 2, 2, 7, 17
#define PPUTLUTRAITS_475  474, 476, 8, 237, 950, 21, 345, 1, 3, 3, 11, 27, 27, 5, 5, 19
#define PPUTLUTRAITS_474  473, 475, 8, 237, 948, 21, 420, 0, 2, 2, 10, 26, 26, 2, 3, 79
#define PPUTLUTRAITS_473  472, 474, 8, 236, 946, 21, 497, 1, 1, 1, 9, 25, 25, 11, 43
#define PPUTLUTRAITS_472  471, 473, 8, 236, 944, 21, 576, 0, 0, 0, 8, 24, 24, 2, 2, 2, 59
#define PPUTLUTRAITS_471  470, 472, 8, 235, 942, 21, 657, 1, 3, 7, 7, 23, 23, 3, 157
#define PPUTLUTRAITS_470  469, 471, 8, 235, 940, 21, 740, 0, 2, 6, 6, 22, 22, 2, 5, 47
#define PPUTLUTRAITS_469  468, 470, 8, 234, 938, 21, 825, 1, 1, 5, 5, 21, 21, 7, 67
#define PPUTLUTRAITS_468  467, 469, 8, 234, 936, 21, 912, 0, 0, 4, 4, 20, 20, 2, 2, 3, 3, 13
#define PPUTLUTRAITS_467  466, 468, 8, 233, 934, 21, 1001, 1, 3, 3, 3, 19, 19,
#define PPUTLUTRAITS_466  465, 467, 8, 233, 932, 21, 68, 0, 2, 2, 2, 18, 18, 2, 233
#define PPUTLUTRAITS_465  464, 466, 8, 232, 930, 21, 161, 1, 1, 1, 1, 17, 17, 3, 5, 31
#define PPUTLUTRAITS_464  463, 465, 8, 232, 928, 21, 256, 0, 0, 0, 0, 16, 16, 2, 2, 2, 2, 29
#define PPUTLUTRAITS_463  462, 464, 8, 231, 926, 21, 353, 1, 3, 7, 15, 15, 15,
#define PPUTLUTRAITS_462  461, 463, 8, 231, 924, 21, 452, 0, 2, 6, 14, 14, 14, 2, 3, 7, 11
#define PPUTLUTRAITS_461  460, 462, 8, 230, 922, 21, 553, 1, 1, 5, 13, 13, 13,
#define PPUTLUTRAITS_460  459, 461, 8, 230, 920, 21, 656, 0, 0, 4, 12, 12, 12, 2, 2, 5, 23
#define PPUTLUTRAITS_459  458, 460, 8, 229, 918, 21, 761, 1, 3, 3, 11, 11, 11, 3, 3, 3, 17
#define PPUTLUTRAITS_458  457, 459, 8, 229, 916, 21, 868, 0, 2, 2, 10, 10, 10, 2, 229
#define PPUTLUTRAITS_457  456, 458, 8, 228, 914, 21, 977, 1, 1, 1, 9, 9, 9,
#define PPUTLUTRAITS_456  455, 457, 8, 228, 912, 21, 64, 0, 0, 0, 8, 8, 8, 2, 2, 2, 3, 19
#define PPUTLUTRAITS_455  454, 456, 8, 227, 910, 21, 177, 1, 3, 7, 7, 7, 7, 5, 7, 13
#define PPUTLUTRAITS_454  453, 455, 8, 227, 908, 21, 292, 0, 2, 6, 6, 6, 6, 2, 227
#define PPUTLUTRAITS_453  452, 454, 8, 226, 906, 21, 409, 1, 1, 5, 5, 5, 5, 3, 151
#define PPUTLUTRAITS_452  451, 453, 8, 226, 904, 21, 528, 0, 0, 4, 4, 4, 4, 2, 2, 113
#define PPUTLUTRAITS_451  450, 452, 8, 225, 902, 21, 649, 1, 3, 3, 3, 3, 3, 11, 41
#define PPUTLUTRAITS_450  449, 451, 8, 225, 900, 21, 772, 0, 2, 2, 2, 2, 2, 2, 3, 3, 5, 5
#define PPUTLUTRAITS_449  448, 450, 8, 224, 898, 21, 897, 1, 1, 1, 1, 1, 1,
#define PPUTLUTRAITS_448  447, 449, 8, 224, 896, 21, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 7
#define PPUTLUTRAITS_447  446, 448, 8, 223, 894, 21, 129, 1, 3, 7, 15, 31, 63, 3, 149
#define PPUTLUTRAITS_446  445, 447, 8, 223, 892, 21, 260, 0, 2, 6, 14, 30, 62, 2, 223
#define PPUTLUTRAITS_445  444, 446, 8, 222, 890, 21, 393, 1, 1, 5, 13, 29, 61, 5, 89
#define PPUTLUTRAITS_444  443, 445, 8, 222, 888, 21, 528, 0, 0, 4, 12, 28, 60, 2, 2, 3, 37
#define PPUTLUTRAITS_443  442, 444, 8, 221, 886, 21, 665, 1, 3, 3, 11, 27, 59,
#define PPUTLUTRAITS_442  441, 443, 8, 221, 884, 21, 804, 0, 2, 2, 10, 26, 58, 2, 13, 17
#define PPUTLUTRAITS_441  440, 442, 8, 220, 882, 21, 945, 1, 1, 1, 9, 25, 57, 3, 3, 7, 7
#define PPUTLUTRAITS_440  439, 441, 8, 220, 880, 20, 64, 0, 0, 0, 8, 24, 56, 2, 2, 2, 5, 11
#define PPUTLUTRAITS_439  438, 440, 8, 219, 878, 20, 209, 1, 3, 7, 7, 23, 55,
#define PPUTLUTRAITS_438  437, 439, 8, 219, 876, 20, 356, 0, 2, 6, 6, 22, 54, 2, 3, 73
#define PPUTLUTRAITS_437  436, 438, 8, 218, 874, 20, 505, 1, 1, 5, 5, 21, 53, 19, 23
#define PPUTLUTRAITS_436  435, 437, 8, 218, 872, 20, 656, 0, 0, 4, 4, 20, 52, 2, 2, 109
#define PPUTLUTRAITS_435  434, 436, 8, 217, 870, 20, 809, 1, 3, 3, 3, 19, 51, 3, 5, 29
#define PPUTLUTRAITS_434  433, 435, 8, 217, 868, 20, 964, 0, 2, 2, 2, 18, 50, 2, 7, 31
#define PPUTLUTRAITS_433  432, 434, 8, 216, 866, 20, 97, 1, 1, 1, 1, 17, 49,
#define PPUTLUTRAITS_432  431, 433, 8, 216, 864, 20, 256, 0, 0, 0, 0, 16, 48, 2, 2, 2, 2, 3, 3, 3
#define PPUTLUTRAITS_431  430, 432, 8, 215, 862, 20, 417, 1, 3, 7, 15, 15, 47,
#define PPUTLUTRAITS_430  429, 431, 8, 215, 860, 20, 580, 0, 2, 6, 14, 14, 46, 2, 5, 43
#define PPUTLUTRAITS_429  428, 430, 8, 214, 858, 20, 745, 1, 1, 5, 13, 13, 45, 3, 11, 13
#define PPUTLUTRAITS_428  427, 429, 8, 214, 856, 20, 912, 0, 0, 4, 12, 12, 44, 2, 2, 107
#define PPUTLUTRAITS_427  426, 428, 8, 213, 854, 20, 57, 1, 3, 3, 11, 11, 43, 7, 61
#define PPUTLUTRAITS_426  425, 427, 8, 213, 852, 20, 228, 0, 2, 2, 10, 10, 42, 2, 3, 71
#define PPUTLUTRAITS_425  424, 426, 8, 212, 850, 20, 401, 1, 1, 1, 9, 9, 41, 5, 5, 17
#define PPUTLUTRAITS_424  423, 425, 8, 212, 848, 20, 576, 0, 0, 0, 8, 8, 40, 2, 2, 2, 53
#define PPUTLUTRAITS_423  422, 424, 8, 211, 846, 20, 753, 1, 3, 7, 7, 7, 39, 3, 3, 47
#define PPUTLUTRAITS_422  421, 423, 8, 211, 844, 20, 932, 0, 2, 6, 6, 6, 38, 2, 211
#define PPUTLUTRAITS_421  420, 422, 8, 210, 842, 20, 89, 1, 1, 5, 5, 5, 37,
#define PPUTLUTRAITS_420  419, 421, 8, 210, 840, 20, 272, 0, 0, 4, 4, 4, 36, 2, 2, 3, 5, 7
#define PPUTLUTRAITS_419  418, 420, 8, 209, 838, 20, 457, 1, 3, 3, 3, 3, 35,
#define PPUTLUTRAITS_418  417, 419, 8, 209, 836, 20, 644, 0, 2, 2, 2, 2, 34, 2, 11, 19
#define PPUTLUTRAITS_417  416, 418, 8, 208, 834, 20, 833, 1, 1, 1, 1, 1, 33, 3, 139
#define PPUTLUTRAITS_416  415, 417, 8, 208, 832, 20, 0, 0, 0, 0, 0, 0, 32, 2, 2, 2, 2, 2, 13
#define PPUTLUTRAITS_415  414, 416, 8, 207, 830, 20, 193, 1, 3, 7, 15, 31, 31, 5, 83
#define PPUTLUTRAITS_414  413, 415, 8, 207, 828, 20, 388, 0, 2, 6, 14, 30, 30, 2, 3, 3, 23
#define PPUTLUTRAITS_413  412, 414, 8, 206, 826, 20, 585, 1, 1, 5, 13, 29, 29, 7, 59
#define PPUTLUTRAITS_412  411, 413, 8, 206, 824, 20, 784, 0, 0, 4, 12, 28, 28, 2, 2, 103
#define PPUTLUTRAITS_411  410, 412, 8, 205, 822, 20, 985, 1, 3, 3, 11, 27, 27, 3, 137
#define PPUTLUTRAITS_410  409, 411, 8, 205, 820, 20, 164, 0, 2, 2, 10, 26, 26, 2, 5, 41
#define PPUTLUTRAITS_409  408, 410, 8, 204, 818, 20, 369, 1, 1, 1, 9, 25, 25,
#define PPUTLUTRAITS_408  407, 409, 8, 204, 816, 20, 576, 0, 0, 0, 8, 24, 24, 2, 2, 2, 3, 17
#define PPUTLUTRAITS_407  406, 408, 8, 203, 814, 20, 785, 1, 3, 7, 7, 23, 23, 11, 37
#define PPUTLUTRAITS_406  405, 407, 8, 203, 812, 20, 996, 0, 2, 6, 6, 22, 22, 2, 7, 29
#define PPUTLUTRAITS_405  404, 406, 8, 202, 810, 20, 185, 1, 1, 5, 5, 21, 21, 3, 3, 3, 3, 5
#define PPUTLUTRAITS_404  403, 405, 8, 202, 808, 20, 400, 0, 0, 4, 4, 20, 20, 2, 2, 101
#define PPUTLUTRAITS_403  402, 404, 8, 201, 806, 20, 617, 1, 3, 3, 3, 19, 19, 13, 31
#define PPUTLUTRAITS_402  401, 403, 8, 201, 804, 20, 836, 0, 2, 2, 2, 18, 18, 2, 3, 67
#define PPUTLUTRAITS_401  400, 402, 8, 200, 802, 20, 33, 1, 1, 1, 1, 17, 17,
#define PPUTLUTRAITS_400  399, 401, 8, 200, 800, 20, 256, 0, 0, 0, 0, 16, 16, 2, 2, 2, 2, 5, 5
#define PPUTLUTRAITS_399  398, 400, 8, 199, 798, 19, 481, 1, 3, 7, 15, 15, 15, 3, 7, 19
#define PPUTLUTRAITS_398  397, 399, 8, 199, 796, 19, 708, 0, 2, 6, 14, 14, 14, 2, 199
#define PPUTLUTRAITS_397  396, 398, 8, 198, 794, 19, 937, 1, 1, 5, 13, 13, 13,
#define PPUTLUTRAITS_396  395, 397, 8, 198, 792, 19, 144, 0, 0, 4, 12, 12, 12, 2, 2, 3, 3, 11
#define PPUTLUTRAITS_395  394, 396, 8, 197, 790, 19, 377, 1, 3, 3, 11, 11, 11, 5, 79
#define PPUTLUTRAITS_394  393, 395, 8, 197, 788, 19, 612, 0, 2, 2, 10, 10, 10, 2, 197
#define PPUTLUTRAITS_393  392, 394, 8, 196, 786, 19, 849, 1, 1, 1, 9, 9, 9, 3, 131
#define PPUTLUTRAITS_392  391, 393, 8, 196, 784, 19, 64, 0, 0, 0, 8, 8, 8, 2, 2, 2, 7, 7
#define PPUTLUTRAITS_391  390, 392, 8, 195, 782, 19, 305, 1, 3, 7, 7, 7, 7, 17, 23
#define PPUTLUTRAITS_390  389, 391, 8, 195, 780, 19, 548, 0, 2, 6, 6, 6, 6, 2, 3, 5, 13
#define PPUTLUTRAITS_389  388, 390, 8, 194, 778, 19, 793, 1, 1, 5, 5, 5, 5,
#define PPUTLUTRAITS_388  387, 389, 8, 194, 776, 19, 16, 0, 0, 4, 4, 4, 4, 2, 2, 97
#define PPUTLUTRAITS_387  386, 388, 8, 193, 774, 19, 265, 1, 3, 3, 3, 3, 3, 3, 3, 43
#define PPUTLUTRAITS_386  385, 387, 8, 193, 772, 19, 516, 0, 2, 2, 2, 2, 2, 2, 193
#define PPUTLUTRAITS_385  384, 386, 8, 192, 770, 19, 769, 1, 1, 1, 1, 1, 1, 5, 7, 11
#define PPUTLUTRAITS_384  383, 385, 8, 192, 768, 19, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 3
#define PPUTLUTRAITS_383  382, 384, 8, 191, 766, 19, 257, 1, 3, 7, 15, 31, 63,
#define PPUTLUTRAITS_382  381, 383, 8, 191, 764, 19, 516, 0, 2, 6, 14, 30, 62, 2, 191
#define PPUTLUTRAITS_381  380, 382, 8, 190, 762, 19, 777, 1, 1, 5, 13, 29, 61, 3, 127
#define PPUTLUTRAITS_380  379, 381, 8, 190, 760, 19, 16, 0, 0, 4, 12, 28, 60, 2, 2, 5, 19
#define PPUTLUTRAITS_379  378, 380, 8, 189, 758, 19, 281, 1, 3, 3, 11, 27, 59,
#define PPUTLUTRAITS_378  377, 379, 8, 189, 756, 19, 548, 0, 2, 2, 10, 26, 58, 2, 3, 3, 3, 7
#define PPUTLUTRAITS_377  376, 378, 8, 188, 754, 19, 817, 1, 1, 1, 9, 25, 57, 13, 29
#define PPUTLUTRAITS_376  375, 377, 8, 188, 752, 19, 64, 0, 0, 0, 8, 24, 56, 2, 2, 2, 47
#define PPUTLUTRAITS_375  374, 376, 8, 187, 750, 19, 337, 1, 3, 7, 7, 23, 55, 3, 5, 5, 5
#define PPUTLUTRAITS_374  373, 375, 8, 187, 748, 19, 612, 0, 2, 6, 6, 22, 54, 2, 11, 17
#define PPUTLUTRAITS_373  372, 374, 8, 186, 746, 19, 889, 1, 1, 5, 5, 21, 53,
#define PPUTLUTRAITS_372  371, 373, 8, 186, 744, 19, 144, 0, 0, 4, 4, 20, 52, 2, 2, 3, 31
#define PPUTLUTRAITS_371  370, 372, 8, 185, 742, 19, 425, 1, 3, 3, 3, 19, 51, 7, 53
#define PPUTLUTRAITS_370  369, 371, 8, 185, 740, 19, 708, 0, 2, 2, 2, 18, 50, 2, 5, 37
#define PPUTLUTRAITS_369  368, 370, 8, 184, 738, 19, 993, 1, 1, 1, 1, 17, 49, 3, 3, 41
#define PPUTLUTRAITS_368  367, 369, 8, 184, 736, 19, 256, 0, 0, 0, 0, 16, 48, 2, 2, 2, 2, 23
#define PPUTLUTRAITS_367  366, 368, 8, 183, 734, 19, 545, 1, 3, 7, 15, 15, 47,
#define PPUTLUTRAITS_366  365, 367, 8, 183, 732, 19, 836, 0, 2, 6, 14, 14, 46, 2, 3, 61
#define PPUTLUTRAITS_365  364, 366, 8, 182, 730, 19, 105, 1, 1, 5, 13, 13, 45, 5, 73
#define PPUTLUTRAITS_364  363, 365, 8, 182, 728, 19, 400, 0, 0, 4, 12, 12, 44, 2, 2, 7, 13
#define PPUTLUTRAITS_363  362, 364, 8, 181, 726, 19, 697, 1, 3, 3, 11, 11, 43, 3, 11, 11
#define PPUTLUTRAITS_362  361, 363, 8, 181, 724, 19, 996, 0, 2, 2, 10, 10, 42, 2, 181
#define PPUTLUTRAITS_361  360, 362, 8, 180, 722, 19, 273, 1, 1, 1, 9, 9, 41, 19, 19
#define PPUTLUTRAITS_360  359, 361, 8, 180, 720, 18, 576, 0, 0, 0, 8, 8, 40, 2, 2, 2, 3, 3, 5
#define PPUTLUTRAITS_359  358, 360, 8, 179, 718, 18, 881, 1, 3, 7, 7, 7, 39,
#define PPUTLUTRAITS_358  357, 359, 8, 179, 716, 18, 164, 0, 2, 6, 6, 6, 38, 2, 179
#define PPUTLUTRAITS_357  356, 358, 8, 178, 714, 18, 473, 1, 1, 5, 5, 5, 37, 3, 7, 17
#define PPUTLUTRAITS_356  355, 357, 8, 178, 712, 18, 784, 0, 0, 4, 4, 4, 36, 2, 2, 89
#define PPUTLUTRAITS_355  354, 356, 8, 177, 710, 18, 73, 1, 3, 3, 3, 3, 35, 5, 71
#define PPUTLUTRAITS_354  353, 355, 8, 177, 708, 18, 388, 0, 2, 2, 2, 2, 34, 2, 3, 59
#define PPUTLUTRAITS_353  352, 354, 8, 176, 706, 18, 705, 1, 1, 1, 1, 1, 33,
#define PPUTLUTRAITS_352  351, 353, 8, 176, 704, 18, 0, 0, 0, 0, 0, 0, 32, 2, 2, 2, 2, 2, 11
#define PPUTLUTRAITS_351  350, 352, 8, 175, 702, 18, 321, 1, 3, 7, 15, 31, 31, 3, 3, 3, 13
#define PPUTLUTRAITS_350  349, 351, 8, 175, 700, 18, 644, 0, 2, 6, 14, 30, 30, 2, 5, 5, 7
#define PPUTLUTRAITS_349  348, 350, 8, 174, 698, 18, 969, 1, 1, 5, 13, 29, 29,
#define PPUTLUTRAITS_348  347, 349, 8, 174, 696, 18, 272, 0, 0, 4, 12, 28, 28, 2, 2, 3, 29
#define PPUTLUTRAITS_347  346, 348, 8, 173, 694, 18, 601, 1, 3, 3, 11, 27, 27,
#define PPUTLUTRAITS_346  345, 347, 8, 173, 692, 18, 932, 0, 2, 2, 10, 26, 26, 2, 173
#define PPUTLUTRAITS_345  344, 346, 8, 172, 690, 18, 241, 1, 1, 1, 9, 25, 25, 3, 5, 23
#define PPUTLUTRAITS_344  343, 345, 8, 172, 688, 18, 576, 0, 0, 0, 8, 24, 24, 2, 2, 2, 43
#define PPUTLUTRAITS_343  342, 344, 8, 171, 686, 18, 913, 1, 3, 7, 7, 23, 23, 7, 7, 7
#define PPUTLUTRAITS_342  341, 343, 8, 171, 684, 18, 228, 0, 2, 6, 6, 22, 22, 2, 3, 3, 19
#define PPUTLUTRAITS_341  340, 342, 8, 170, 682, 18, 569, 1, 1, 5, 5, 21, 21, 11, 31
#define PPUTLUTRAITS_340  339, 341, 8, 170, 680, 18, 912, 0, 0, 4, 4, 20, 20, 2, 2, 5, 17
#define PPUTLUTRAITS_339  338, 340, 8, 169, 678, 18, 233, 1, 3, 3, 3, 19, 19, 3, 113
#define PPUTLUTRAITS_338  337, 339, 8, 169, 676, 18, 580, 0, 2, 2, 2, 18, 18, 2, 13, 13
#define PPUTLUTRAITS_337  336, 338, 8, 168, 674, 18, 929, 1, 1, 1, 1, 17, 17,
#define PPUTLUTRAITS_336  335, 337, 8, 168, 672, 18, 256, 0, 0, 0, 0, 16, 16, 2, 2, 2, 2, 3, 7
#define PPUTLUTRAITS_335  334, 336, 8, 167, 670, 18, 609, 1, 3, 7, 15, 15, 15, 5, 67
#define PPUTLUTRAITS_334  333, 335, 8, 167, 668, 18, 964, 0, 2, 6, 14, 14, 14, 2, 167
#define PPUTLUTRAITS_333  332, 334, 8, 166, 666, 18, 297, 1, 1, 5, 13, 13, 13, 3, 3, 37
#define PPUTLUTRAITS_332  331, 333, 8, 166, 664, 18, 656, 0, 0, 4, 12, 12, 12, 2, 2, 83
#define PPUTLUTRAITS_331  330, 332, 8, 165, 662, 18, 1017, 1, 3, 3, 11, 11, 11,
#define PPUTLUTRAITS_330  329, 331, 8, 165, 660, 18, 356, 0, 2, 2, 10, 10, 10, 2, 3, 5, 11
#define PPUTLUTRAITS_329  328, 330, 8, 164, 658, 18, 721, 1, 1, 1, 9, 9, 9, 7, 47
#define PPUTLUTRAITS_328  327, 329, 8, 164, 656, 18, 64, 0, 0, 0, 8, 8, 8, 2, 2, 2, 41
#define PPUTLUTRAITS_327  326, 328, 8, 163, 654, 18, 433, 1, 3, 7, 7, 7, 7, 3, 109
#define PPUTLUTRAITS_326  325, 327, 8, 163, 652, 18, 804, 0, 2, 6, 6, 6, 6, 2, 163
#define PPUTLUTRAITS_325  324, 326, 8, 162, 650, 18, 153, 1, 1, 5, 5, 5, 5, 5, 5, 13
#define PPUTLUTRAITS_324  323, 325, 8, 162, 648, 18, 528, 0, 0, 4, 4, 4, 4, 2, 2, 3, 3, 3, 3
#define PPUTLUTRAITS_323  322, 324, 8, 161, 646, 17, 905, 1, 3, 3, 3, 3, 3, 17, 19
#define PPUTLUTRAITS_322  321, 323, 8, 161, 644, 17, 260, 0, 2, 2, 2, 2, 2, 2, 7, 23
#define PPUTLUTRAITS_321  320, 322, 8, 160, 642, 17, 641, 1, 1, 1, 1, 1, 1, 3, 107
#define PPUTLUTRAITS_320  319, 321, 8, 160, 640, 17, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 5
#define PPUTLUTRAITS_319  318, 320, 8, 159, 638, 17, 385, 1, 3, 7, 15, 31, 63, 11, 29
#define PPUTLUTRAITS_318  317, 319, 8, 159, 636, 17, 772, 0, 2, 6, 14, 30, 62, 2, 3, 53
#define PPUTLUTRAITS_317  316, 318, 8, 158, 634, 17, 137, 1, 1, 5, 13, 29, 61,
#define PPUTLUTRAITS_316  315, 317, 8, 158, 632, 17, 528, 0, 0, 4, 12, 28, 60, 2, 2, 79
#define PPUTLUTRAITS_315  314, 316, 8, 157, 630, 17, 921, 1, 3, 3, 11, 27, 59, 3, 3, 5, 7
#define PPUTLUTRAITS_314  313, 315, 8, 157, 628, 17, 292, 0, 2, 2, 10, 26, 58, 2, 157
#define PPUTLUTRAITS_313  312, 314, 8, 156, 626, 17, 689, 1, 1, 1, 9, 25, 57,
#define PPUTLUTRAITS_312  311, 313, 8, 156, 624, 17, 64, 0, 0, 0, 8, 24, 56, 2, 2, 2, 3, 13
#define PPUTLUTRAITS_311  310, 312, 8, 155, 622, 17, 465, 1, 3, 7, 7, 23, 55,
#define PPUTLUTRAITS_310  309, 311, 8, 155, 620, 17, 868, 0, 2, 6, 6, 22, 54, 2, 5, 31
#define PPUTLUTRAITS_309  308, 310, 8, 154, 618, 17, 249, 1, 1, 5, 5, 21, 53, 3, 103
#define PPUTLUTRAITS_308  307, 309, 8, 154, 616, 17, 656, 0, 0, 4, 4, 20, 52, 2, 2, 7, 11
#define PPUTLUTRAITS_307  306, 308, 8, 153, 614, 17, 41, 1, 3, 3, 3, 19, 51,
#define PPUTLUTRAITS_306  305, 307, 8, 153, 612, 17, 452, 0, 2, 2, 2, 18, 50, 2, 3, 3, 17
#define PPUTLUTRAITS_305  304, 306, 8, 152, 610, 17, 865, 1, 1, 1, 1, 17, 49, 5, 61
#define PPUTLUTRAITS_304  303, 305, 8, 152, 608, 17, 256, 0, 0, 0, 0, 16, 48, 2, 2, 2, 2, 19
#define PPUTLUTRAITS_303  302, 304, 8, 151, 606, 17, 673, 1, 3, 7, 15, 15, 47, 3, 101
#define PPUTLUTRAITS_302  301, 303, 8, 151, 604, 17, 68, 0, 2, 6, 14, 14, 46, 2, 151
#define PPUTLUTRAITS_301  300, 302, 8, 150, 602, 17, 489, 1, 1, 5, 13, 13, 45, 7, 43
#define PPUTLUTRAITS_300  299, 301, 8, 150, 600, 17, 912, 0, 0, 4, 12, 12, 44, 2, 2, 3, 5, 5
#define PPUTLUTRAITS_299  298, 300, 8, 149, 598, 17, 313, 1, 3, 3, 11, 11, 43, 13, 23
#define PPUTLUTRAITS_298  297, 299, 8, 149, 596, 17, 740, 0, 2, 2, 10, 10, 42, 2, 149
#define PPUTLUTRAITS_297  296, 298, 8, 148, 594, 17, 145, 1, 1, 1, 9, 9, 41, 3, 3, 3, 11
#define PPUTLUTRAITS_296  295, 297, 8, 148, 592, 17, 576, 0, 0, 0, 8, 8, 40, 2, 2, 2, 37
#define PPUTLUTRAITS_295  294, 296, 8, 147, 590, 17, 1009, 1, 3, 7, 7, 7, 39, 5, 59
#define PPUTLUTRAITS_294  293, 295, 8, 147, 588, 17, 420, 0, 2, 6, 6, 6, 38, 2, 3, 7, 7
#define PPUTLUTRAITS_293  292, 294, 8, 146, 586, 17, 857, 1, 1, 5, 5, 5, 37,
#define PPUTLUTRAITS_292  291, 293, 8, 146, 584, 17, 272, 0, 0, 4, 4, 4, 36, 2, 2, 73
#define PPUTLUTRAITS_291  290, 292, 8, 145, 582, 17, 713, 1, 3, 3, 3, 3, 35, 3, 97
#define PPUTLUTRAITS_290  289, 291, 8, 145, 580, 17, 132, 0, 2, 2, 2, 2, 34, 2, 5, 29
#define PPUTLUTRAITS_289  288, 290, 8, 144, 578, 17, 577, 1, 1, 1, 1, 1, 33, 17, 17
#define PPUTLUTRAITS_288  287, 289, 8, 144, 576, 16, 0, 0, 0, 0, 0, 0, 32, 2, 2, 2, 2, 2, 3, 3
#define PPUTLUTRAITS_287  286, 288, 8, 143, 574, 16, 449, 1, 3, 7, 15, 31, 31, 7, 41
#define PPUTLUTRAITS_286  285, 287, 8, 143, 572, 16, 900, 0, 2, 6, 14, 30, 30, 2, 11, 13
#define PPUTLUTRAITS_285  284, 286, 8, 142, 570, 16, 329, 1, 1, 5, 13, 29, 29, 3, 5, 19
#define PPUTLUTRAITS_284  283, 285, 8, 142, 568, 16, 784, 0, 0, 4, 12, 28, 28, 2, 2, 71
#define PPUTLUTRAITS_283  282, 284, 8, 141, 566, 16, 217, 1, 3, 3, 11, 27, 27,
#define PPUTLUTRAITS_282  281, 283, 8, 141, 564, 16, 676, 0, 2, 2, 10, 26, 26, 2, 3, 47
#define PPUTLUTRAITS_281  280, 282, 8, 140, 562, 16, 113, 1, 1, 1, 9, 25, 25,
#define PPUTLUTRAITS_280  279, 281, 8, 140, 560, 16, 576, 0, 0, 0, 8, 24, 24, 2, 2, 2, 5, 7
#define PPUTLUTRAITS_279  278, 280, 8, 139, 558, 16, 17, 1, 3, 7, 7, 23, 23, 3, 3, 31
#define PPUTLUTRAITS_278  277, 279, 8, 139, 556, 16, 484, 0, 2, 6, 6, 22, 22, 2, 139
#define PPUTLUTRAITS_277  276, 278, 8, 138, 554, 16, 953, 1, 1, 5, 5, 21, 21,
#define PPUTLUTRAITS_276  275, 277, 8, 138, 552, 16, 400, 0, 0, 4, 4, 20, 20, 2, 2, 3, 23
#define PPUTLUTRAITS_275  274, 276, 8, 137, 550, 16, 873, 1, 3, 3, 3, 19, 19, 5, 5, 11
#define PPUTLUTRAITS_274  273, 275, 8, 137, 548, 16, 324, 0, 2, 2, 2, 18, 18, 2, 137
#define PPUTLUTRAITS_273  272, 274, 8, 136, 546, 16, 801, 1, 1, 1, 1, 17, 17, 3, 7, 13
#define PPUTLUTRAITS_272  271, 273, 8, 136, 544, 16, 256, 0, 0, 0, 0, 16, 16, 2, 2, 2, 2, 17
#define PPUTLUTRAITS_271  270, 272, 8, 135, 542, 16, 737, 1, 3, 7, 15, 15, 15,
#define PPUTLUTRAITS_270  269, 271, 8, 135, 540, 16, 196, 0, 2, 6, 14, 14, 14, 2, 3, 3, 3, 5
#define PPUTLUTRAITS_269  268, 270, 8, 134, 538, 16, 681, 1, 1, 5, 13, 13, 13,
#define PPUTLUTRAITS_268  267, 269, 8, 134, 536, 16, 144, 0, 0, 4, 12, 12, 12, 2, 2, 67
#define PPUTLUTRAITS_267  266, 268, 8, 133, 534, 16, 633, 1, 3, 3, 11, 11, 11, 3, 89
#define PPUTLUTRAITS_266  265, 267, 8, 133, 532, 16, 100, 0, 2, 2, 10, 10, 10, 2, 7, 19
#define PPUTLUTRAITS_265  264, 266, 8, 132, 530, 16, 593, 1, 1, 1, 9, 9, 9, 5, 53
#define PPUTLUTRAITS_264  263, 265, 8, 132, 528, 16, 64, 0, 0, 0, 8, 8, 8, 2, 2, 2, 3, 11
#define PPUTLUTRAITS_263  262, 264, 8, 131, 526, 16, 561, 1, 3, 7, 7, 7, 7,
#define PPUTLUTRAITS_262  261, 263, 8, 131, 524, 16, 36, 0, 2, 6, 6, 6, 6, 2, 131
#define PPUTLUTRAITS_261  260, 262, 8, 130, 522, 16, 537, 1, 1, 5, 5, 5, 5, 3, 3, 29
#define PPUTLUTRAITS_260  259, 261, 8, 130, 520, 16, 16, 0, 0, 4, 4, 4, 4, 2, 2, 5, 13
#define PPUTLUTRAITS_259  258, 260, 8, 129, 518, 16, 521, 1, 3, 3, 3, 3, 3, 7, 37
#define PPUTLUTRAITS_258  257, 259, 8, 129, 516, 16, 4, 0, 2, 2, 2, 2, 2, 2, 3, 43
#define PPUTLUTRAITS_257  256, 258, 8, 128, 514, 16, 513, 1, 1, 1, 1, 1, 1,
#define PPUTLUTRAITS_256  255, 257, 8, 128, 512, 16, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2
#define PPUTLUTRAITS_255  254, 256, 7, 127, 510, 15, 513, 1, 3, 7, 15, 31, 63, 3, 5, 17
#define PPUTLUTRAITS_254  253, 255, 7, 127, 508, 15, 4, 0, 2, 6, 14, 30, 62, 2, 127
#define PPUTLUTRAITS_253  252, 254, 7, 126, 506, 15, 521, 1, 1, 5, 13, 29, 61, 11, 23
#define PPUTLUTRAITS_252  251, 253, 7, 126, 504, 15, 16, 0, 0, 4, 12, 28, 60, 2, 2, 3, 3, 7
#define PPUTLUTRAITS_251  250, 252, 7, 125, 502, 15, 537, 1, 3, 3, 11, 27, 59,
#define PPUTLUTRAITS_250  249, 251, 7, 125, 500, 15, 36, 0, 2, 2, 10, 26, 58, 2, 5, 5, 5
#define PPUTLUTRAITS_249  248, 250, 7, 124, 498, 15, 561, 1, 1, 1, 9, 25, 57, 3, 83
#define PPUTLUTRAITS_248  247, 249, 7, 124, 496, 15, 64, 0, 0, 0, 8, 24, 56, 2, 2, 2, 31
#define PPUTLUTRAITS_247  246, 248, 7, 123, 494, 15, 593, 1, 3, 7, 7, 23, 55, 13, 19
#define PPUTLUTRAITS_246  245, 247, 7, 123, 492, 15, 100, 0, 2, 6, 6, 22, 54, 2, 3, 41
#define PPUTLUTRAITS_245  244, 246, 7, 122, 490, 15, 633, 1, 1, 5, 5, 21, 53, 5, 7, 7
#define PPUTLUTRAITS_244  243, 245, 7, 122, 488, 15, 144, 0, 0, 4, 4, 20, 52, 2, 2, 61
#define PPUTLUTRAITS_243  242, 244, 7, 121, 486, 15, 681, 1, 3, 3, 3, 19, 51, 3, 3, 3, 3, 3
#define PPUTLUTRAITS_242  241, 243, 7, 121, 484, 15, 196, 0, 2, 2, 2, 18, 50, 2, 11, 11
#define PPUTLUTRAITS_241  240, 242, 7, 120, 482, 15, 737, 1, 1, 1, 1, 17, 49,
#define PPUTLUTRAITS_240  239, 241, 7, 120, 480, 15, 256, 0, 0, 0, 0, 16, 48, 2, 2, 2, 2, 3, 5
#define PPUTLUTRAITS_239  238, 240, 7, 119, 478, 15, 801, 1, 3, 7, 15, 15, 47,
#define PPUTLUTRAITS_238  237, 239, 7, 119, 476, 15, 324, 0, 2, 6, 14, 14, 46, 2, 7, 17
#define PPUTLUTRAITS_237  236, 238, 7, 118, 474, 15, 873, 1, 1, 5, 13, 13, 45, 3, 79
#define PPUTLUTRAITS_236  235, 237, 7, 118, 472, 15, 400, 0, 0, 4, 12, 12, 44, 2, 2, 59
#define PPUTLUTRAITS_235  234, 236, 7, 117, 470, 15, 953, 1, 3, 3, 11, 11, 43, 5, 47
#define PPUTLUTRAITS_234  233, 235, 7, 117, 468, 15, 484, 0, 2, 2, 10, 10, 42, 2, 3, 3, 13
#define PPUTLUTRAITS_233  232, 234, 7, 116, 466, 15, 17, 1, 1, 1, 9, 9, 41,
#define PPUTLUTRAITS_232  231, 233, 7, 116, 464, 15, 576, 0, 0, 0, 8, 8, 40, 2, 2, 2, 29
#define PPUTLUTRAITS_231  230, 232, 7, 115, 462, 15, 113, 1, 3, 7, 7, 7, 39, 3, 7, 11
#define PPUTLUTRAITS_230  229, 231, 7, 115, 460, 15, 676, 0, 2, 6, 6, 6, 38, 2, 5, 23
#define PPUTLUTRAITS_229  228, 230, 7, 114, 458, 15, 217, 1, 1, 5, 5, 5, 37,
#define PPUTLUTRAITS_228  227, 229, 7, 114, 456, 15, 784, 0, 0, 4, 4, 4, 36, 2, 2, 3, 19
#define PPUTLUTRAITS_227  226, 228, 7, 113, 454, 15, 329, 1, 3, 3, 3, 3, 35,
#define PPUTLUTRAITS_226  225, 227, 7, 113, 452, 15, 900, 0, 2, 2, 2, 2, 34, 2, 113
#define PPUTLUTRAITS_225  224, 226, 7, 112, 450, 15, 449, 1, 1, 1, 1, 1, 33, 3, 3, 5, 5
#define PPUTLUTRAITS_224  223, 225, 7, 112, 448, 14, 0, 0, 0, 0, 0, 0, 32, 2, 2, 2, 2, 2, 7
#define PPUTLUTRAITS_223  222, 224, 7, 111, 446, 14, 577, 1, 3, 7, 15, 31, 31,
#define PPUTLUTRAITS_222  221, 223, 7, 111, 444, 14, 132, 0, 2, 6, 14, 30, 30, 2, 3, 37
#define PPUTLUTRAITS_221  220, 222, 7, 110, 442, 14, 713, 1, 1, 5, 13, 29, 29, 13, 17
#define PPUTLUTRAITS_220  219, 221, 7, 110, 440, 14, 272, 0, 0, 4, 12, 28, 28, 2, 2, 5, 11
#define PPUTLUTRAITS_219  218, 220, 7, 109, 438, 14, 857, 1, 3, 3, 11, 27, 27, 3, 73
#define PPUTLUTRAITS_218  217, 219, 7, 109, 436, 14, 420, 0, 2, 2, 10, 26, 26, 2, 109
#define PPUTLUTRAITS_217  216, 218, 7, 108, 434, 14, 1009, 1, 1, 1, 9, 25, 25, 7, 31
#define PPUTLUTRAITS_216  215, 217, 7, 108, 432, 14, 576, 0, 0, 0, 8, 24, 24, 2, 2, 2, 3, 3, 3
#define PPUTLUTRAITS_215  214, 216, 7, 107, 430, 14, 145, 1, 3, 7, 7, 23, 23, 5, 43
#define PPUTLUTRAITS_214  213, 215, 7, 107, 428, 14, 740, 0, 2, 6, 6, 22, 22, 2, 107
#define PPUTLUTRAITS_213  212, 214, 7, 106, 426, 14, 313, 1, 1, 5, 5, 21, 21, 3, 71
#define PPUTLUTRAITS_212  211, 213, 7, 106, 424, 14, 912, 0, 0, 4, 4, 20, 20, 2, 2, 53
#define PPUTLUTRAITS_211  210, 212, 7, 105, 422, 14, 489, 1, 3, 3, 3, 19, 19,
#define PPUTLUTRAITS_210  209, 211, 7, 105, 420, 14, 68, 0, 2, 2, 2, 18, 18, 2, 3, 5, 7
#define PPUTLUTRAITS_209  208, 210, 7, 104, 418, 14, 673, 1, 1, 1, 1, 17, 17, 11, 19
#define PPUTLUTRAITS_208  207, 209, 7, 104, 416, 14, 256, 0, 0, 0, 0, 16, 16, 2, 2, 2, 2, 13
#define PPUTLUTRAITS_207  206, 208, 7, 103, 414, 14, 865, 1, 3, 7, 15, 15, 15, 3, 3, 23
#define PPUTLUTRAITS_206  205, 207, 7, 103, 412, 14, 452, 0, 2, 6, 14, 14, 14, 2, 103
#define PPUTLUTRAITS_205  204, 206, 7, 102, 410, 14, 41, 1, 1, 5, 13, 13, 13, 5, 41
#define PPUTLUTRAITS_204  203, 205, 7, 102, 408, 14, 656, 0, 0, 4, 12, 12, 12, 2, 2, 3, 17
#define PPUTLUTRAITS_203  202, 204, 7, 101, 406, 14, 249, 1, 3, 3, 11, 11, 11, 7, 29
#define PPUTLUTRAITS_202  201, 203, 7, 101, 404, 14, 868, 0, 2, 2, 10, 10, 10, 2, 101
#define PPUTLUTRAITS_201  200, 202, 7, 100, 402, 14, 465, 1, 1, 1, 9, 9, 9, 3, 67
#define PPUTLUTRAITS_200  199, 201, 7, 100, 400, 14, 64, 0, 0, 0, 8, 8, 8, 2, 2, 2, 5, 5
#define PPUTLUTRAITS_199  198, 200, 7, 99, 398, 14, 689, 1, 3, 7, 7, 7, 7,
#define PPUTLUTRAITS_198  197, 199, 7, 99, 396, 14, 292, 0, 2, 6, 6, 6, 6, 2, 3, 3, 11
#define PPUTLUTRAITS_197  196, 198, 7, 98, 394, 14, 921, 1, 1, 5, 5, 5, 5,
#define PPUTLUTRAITS_196  195, 197, 7, 98, 392, 14, 528, 0, 0, 4, 4, 4, 4, 2, 2, 7, 7
#define PPUTLUTRAITS_195  194, 196, 7, 97, 390, 13, 137, 1, 3, 3, 3, 3, 3, 3, 5, 13
#define PPUTLUTRAITS_194  193, 195, 7, 97, 388, 13, 772, 0, 2, 2, 2, 2, 2, 2, 97
#define PPUTLUTRAITS_193  192, 194, 7, 96, 386, 13, 385, 1, 1, 1, 1, 1, 1,
#define PPUTLUTRAITS_192  191, 193, 7, 96, 384, 13, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 3
#define PPUTLUTRAITS_191  190, 192, 7, 95, 382, 13, 641, 1, 3, 7, 15, 31, 63,
#define PPUTLUTRAITS_190  189, 191, 7, 95, 380, 13, 260, 0, 2, 6, 14, 30, 62, 2, 5, 19
#define PPUTLUTRAITS_189  188, 190, 7, 94, 378, 13, 905, 1, 1, 5, 13, 29, 61, 3, 3, 3, 7
#define PPUTLUTRAITS_188  187, 189, 7, 94, 376, 13, 528, 0, 0, 4, 12, 28, 60, 2, 2, 47
#define PPUTLUTRAITS_187  186, 188, 7, 93, 374, 13, 153, 1, 3, 3, 11, 27, 59, 11, 17
#define PPUTLUTRAITS_186  185, 187, 7, 93, 372, 13, 804, 0, 2, 2, 10, 26, 58, 2, 3, 31
#define PPUTLUTRAITS_185  184, 186, 7, 92, 370, 13, 433, 1, 1, 1, 9, 25, 57, 5, 37
#define PPUTLUTRAITS_184  183, 185, 7, 92, 368, 13, 64, 0, 0, 0, 8, 24, 56, 2, 2, 2, 23
#define PPUTLUTRAITS_183  182, 184, 7, 91, 366, 13, 721, 1, 3, 7, 7, 23, 55, 3, 61
#define PPUTLUTRAITS_182  181, 183, 7, 91, 364, 13, 356, 0, 2, 6, 6, 22, 54, 2, 7, 13
#define PPUTLUTRAITS_181  180, 182, 7, 90, 362, 13, 1017, 1, 1, 5, 5, 21, 53,
#define PPUTLUTRAITS_180  179, 181, 7, 90, 360, 13, 656, 0, 0, 4, 4, 20, 52, 2, 2, 3, 3, 5
#define PPUTLUTRAITS_179  178, 180, 7, 89, 358, 13, 297, 1, 3, 3, 3, 19, 51,
#define PPUTLUTRAITS_178  177, 179, 7, 89, 356, 13, 964, 0, 2, 2, 2, 18, 50, 2, 89
#define PPUTLUTRAITS_177  176, 178, 7, 88, 354, 13, 609, 1, 1, 1, 1, 17, 49, 3, 59
#define PPUTLUTRAITS_176  175, 177, 7, 88, 352, 13, 256, 0, 0, 0, 0, 16, 48, 2, 2, 2, 2, 11
#define PPUTLUTRAITS_175  174, 176, 7, 87, 350, 13, 929, 1, 3, 7, 15, 15, 47, 5, 5, 7
#define PPUTLUTRAITS_174  173, 175, 7, 87, 348, 13, 580, 0, 2, 6, 14, 14, 46, 2, 3, 29
#define PPUTLUTRAITS_173  172, 174, 7, 86, 346, 13, 233, 1, 1, 5, 13, 13, 45,
#define PPUTLUTRAITS_172  171, 173, 7, 86, 344, 13, 912, 0, 0, 4, 12, 12, 44, 2, 2, 43
#define PPUTLUTRAITS_171  170, 172, 7, 85, 342, 13, 569, 1, 3, 3, 11, 11, 43, 3, 3, 19
#define PPUTLUTRAITS_170  169, 171, 7, 85, 340, 13, 228, 0, 2, 2, 10, 10, 42, 2, 5, 17
#define PPUTLUTRAITS_169  168, 170, 7, 84, 338, 13, 913, 1, 1, 1, 9, 9, 41, 13, 13
#define PPUTLUTRAITS_168  167, 169, 7, 84, 336, 12, 576, 0, 0, 0, 8, 8, 40, 2, 2, 2, 3, 7
#define PPUTLUTRAITS_167  166, 168, 7, 83, 334, 12, 241, 1, 3, 7, 7, 7, 39,
#define PPUTLUTRAITS_166  165, 167, 7, 83, 332, 12, 932, 0, 2, 6, 6, 6, 38, 2, 83
#define PPUTLUTRAITS_165  164, 166, 7, 82, 330, 12, 601, 1, 1, 5, 5, 5, 37, 3, 5, 11
#define PPUTLUTRAITS_164  163, 165, 7, 82, 328, 12, 272, 0, 0, 4, 4, 4, 36, 2, 2, 41
#define PPUTLUTRAITS_163  162, 164, 7, 81, 326, 12, 969, 1, 3, 3, 3, 3, 35,
#define PPUTLUTRAITS_162  161, 163, 7, 81, 324, 12, 644, 0, 2, 2, 2, 2, 34, 2, 3, 3, 3, 3
#define PPUTLUTRAITS_161  160, 162, 7, 80, 322, 12, 321, 1, 1, 1, 1, 1, 33, 7, 23
#define PPUTLUTRAITS_160  159, 161, 7, 80, 320, 12, 0, 0, 0, 0, 0, 0, 32, 2, 2, 2, 2, 2, 5
#define PPUTLUTRAITS_159  158, 160, 7, 79, 318, 12, 705, 1, 3, 7, 15, 31, 31, 3, 53
#define PPUTLUTRAITS_158  157, 159, 7, 79, 316, 12, 388, 0, 2, 6, 14, 30, 30, 2, 79
#define PPUTLUTRAITS_157  156, 158, 7, 78, 314, 12, 73, 1, 1, 5, 13, 29, 29,
#define PPUTLUTRAITS_156  155, 157, 7, 78, 312, 12, 784, 0, 0, 4, 12, 28, 28, 2, 2, 3, 13
#define PPUTLUTRAITS_155  154, 156, 7, 77, 310, 12, 473, 1, 3, 3, 11, 27, 27, 5, 31
#define PPUTLUTRAITS_154  153, 155, 7, 77, 308, 12, 164, 0, 2, 2, 10, 26, 26, 2, 7, 11
#define PPUTLUTRAITS_153  152, 154, 7, 76, 306, 12, 881, 1, 1, 1, 9, 25, 25, 3, 3, 17
#define PPUTLUTRAITS_152  151, 153, 7, 76, 304, 12, 576, 0, 0, 0, 8, 24, 24, 2, 2, 2, 19
#define PPUTLUTRAITS_151  150, 152, 7, 75, 302, 12, 273, 1, 3, 7, 7, 23, 23,
#define PPUTLUTRAITS_150  149, 151, 7, 75, 300, 12, 996, 0, 2, 6, 6, 22, 22, 2, 3, 5, 5
#define PPUTLUTRAITS_149  148, 150, 7, 74, 298, 12, 697, 1, 1, 5, 5, 21, 21,
#define PPUTLUTRAITS_148  147, 149, 7, 74, 296, 12, 400, 0, 0, 4, 4, 20, 20, 2, 2, 37
#define PPUTLUTRAITS_147  146, 148, 7, 73, 294, 12, 105, 1, 3, 3, 3, 19, 19, 3, 7, 7
#define PPUTLUTRAITS_146  145, 147, 7, 73, 292, 12, 836, 0, 2, 2, 2, 18, 18, 2, 73
#define PPUTLUTRAITS_145  144, 146, 7, 72, 290, 12, 545, 1, 1, 1, 1, 17, 17, 5, 29
#define PPUTLUTRAITS_144  143, 145, 7, 72, 288, 12, 256, 0, 0, 0, 0, 16, 16, 2, 2, 2, 2, 3, 3
#define PPUTLUTRAITS_143  142, 144, 7, 71, 286, 11, 993, 1, 3, 7, 15, 15, 15, 11, 13
#define PPUTLUTRAITS_142  141, 143, 7, 71, 284, 11, 708, 0, 2, 6, 14, 14, 14, 2, 71
#define PPUTLUTRAITS_141  140, 142, 7, 70, 282, 11, 425, 1, 1, 5, 13, 13, 13, 3, 47
#define PPUTLUTRAITS_140  139, 141, 7, 70, 280, 11, 144, 0, 0, 4, 12, 12, 12, 2, 2, 5, 7
#define PPUTLUTRAITS_139  138, 140, 7, 69, 278, 11, 889, 1, 3, 3, 11, 11, 11,
#define PPUTLUTRAITS_138  137, 139, 7, 69, 276, 11, 612, 0, 2, 2, 10, 10, 10, 2, 3, 23
#define PPUTLUTRAITS_137  136, 138, 7, 68, 274, 11, 337, 1, 1, 1, 9, 9, 9,
#define PPUTLUTRAITS_136  135, 137, 7, 68, 272, 11, 64, 0, 0, 0, 8, 8, 8, 2, 2, 2, 17
#define PPUTLUTRAITS_135  134, 136, 7, 67, 270, 11, 817, 1, 3, 7, 7, 7, 7, 3, 3, 3, 5
#define PPUTLUTRAITS_134  133, 135, 7, 67, 268, 11, 548, 0, 2, 6, 6, 6, 6, 2, 67
#define PPUTLUTRAITS_133  132, 134, 7, 66, 266, 11, 281, 1, 1, 5, 5, 5, 5, 7, 19
#define PPUTLUTRAITS_132  131, 133, 7, 66, 264, 11, 16, 0, 0, 4, 4, 4, 4, 2, 2, 3, 11
#define PPUTLUTRAITS_131  130, 132, 7, 65, 262, 11, 777, 1, 3, 3, 3, 3, 3,
#define PPUTLUTRAITS_130  129, 131, 7, 65, 260, 11, 516, 0, 2, 2, 2, 2, 2, 2, 5, 13
#define PPUTLUTRAITS_129  128, 130, 7, 64, 258, 11, 257, 1, 1, 1, 1, 1, 1, 3, 43
#define PPUTLUTRAITS_128  127, 129, 7, 64, 256, 11, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2
#define PPUTLUTRAITS_127  126, 128, 6, 63, 254, 11, 769, 1, 3, 7, 15, 31, 63,
#define PPUTLUTRAITS_126  125, 127, 6, 63, 252, 11, 516, 0, 2, 6, 14, 30, 62, 2, 3, 3, 7
#define PPUTLUTRAITS_125  124, 126, 6, 62, 250, 11, 265, 1, 1, 5, 13, 29, 61, 5, 5, 5
#define PPUTLUTRAITS_124  123, 125, 6, 62, 248, 11, 16, 0, 0, 4, 12, 28, 60, 2, 2, 31
#define PPUTLUTRAITS_123  122, 124, 6, 61, 246, 11, 793, 1, 3, 3, 11, 27, 59, 3, 41
#define PPUTLUTRAITS_122  121, 123, 6, 61, 244, 11, 548, 0, 2, 2, 10, 26, 58, 2, 61
#define PPUTLUTRAITS_121  120, 122, 6, 60, 242, 11, 305, 1, 1, 1, 9, 25, 57, 11, 11
#define PPUTLUTRAITS_120  119, 121, 6, 60, 240, 10, 64, 0, 0, 0, 8, 24, 56, 2, 2, 2, 3, 5
#define PPUTLUTRAITS_119  118, 120, 6, 59, 238, 10, 849, 1, 3, 7, 7, 23, 55, 7, 17
#define PPUTLUTRAITS_118  117, 119, 6, 59, 236, 10, 612, 0, 2, 6, 6, 22, 54, 2, 59
#define PPUTLUTRAITS_117  116, 118, 6, 58, 234, 10, 377, 1, 1, 5, 5, 21, 53, 3, 3, 13
#define PPUTLUTRAITS_116  115, 117, 6, 58, 232, 10, 144, 0, 0, 4, 4, 20, 52, 2, 2, 29
#define PPUTLUTRAITS_115  114, 116, 6, 57, 230, 10, 937, 1, 3, 3, 3, 19, 51, 5, 23
#define PPUTLUTRAITS_114  113, 115, 6, 57, 228, 10, 708, 0, 2, 2, 2, 18, 50, 2, 3, 19
#define PPUTLUTRAITS_113  112, 114, 6, 56, 226, 10, 481, 1, 1, 1, 1, 17, 49,
#define PPUTLUTRAITS_112  111, 113, 6, 56, 224, 10, 256, 0, 0, 0, 0, 16, 48, 2, 2, 2, 2, 7
#define PPUTLUTRAITS_111  110, 112, 6, 55, 222, 10, 33, 1, 3, 7, 15, 15, 47, 3, 37
#define PPUTLUTRAITS_110  109, 111, 6, 55, 220, 10, 836, 0, 2, 6, 14, 14, 46, 2, 5, 11
#define PPUTLUTRAITS_109  108, 110, 6, 54, 218, 10, 617, 1, 1, 5, 13, 13, 45,
#define PPUTLUTRAITS_108  107, 109, 6, 54, 216, 10, 400, 0, 0, 4, 12, 12, 44, 2, 2, 3, 3, 3
#define PPUTLUTRAITS_107  106, 108, 6, 53, 214, 10, 185, 1, 3, 3, 11, 11, 43,
#define PPUTLUTRAITS_106  105, 107, 6, 53, 212, 10, 996, 0, 2, 2, 10, 10, 42, 2, 53
#define PPUTLUTRAITS_105  104, 106, 6, 52, 210, 10, 785, 1, 1, 1, 9, 9, 41, 3, 5, 7
#define PPUTLUTRAITS_104  103, 105, 6, 52, 208, 10, 576, 0, 0, 0, 8, 8, 40, 2, 2, 2, 13
#define PPUTLUTRAITS_103  102, 104, 6, 51, 206, 10, 369, 1, 3, 7, 7, 7, 39,
#define PPUTLUTRAITS_102  101, 103, 6, 51, 204, 10, 164, 0, 2, 6, 6, 6, 38, 2, 3, 17
#define PPUTLUTRAITS_101  100, 102, 6, 50, 202, 10, 985, 1, 1, 5, 5, 5, 37,
#define PPUTLUTRAITS_100  99, 101, 6, 50, 200, 10, 784, 0, 0, 4, 4, 4, 36, 2, 2, 5, 5
#define PPUTLUTRAITS_99   98, 100, 6, 49, 198, 9, 585, 1, 3, 3, 3, 3, 35, 3, 3, 11
#define PPUTLUTRAITS_98   97, 99, 6, 49, 196, 9, 388, 0, 2, 2, 2, 2, 34, 2, 7, 7
#define PPUTLUTRAITS_97   96, 98, 6, 48, 194, 9, 193, 1, 1, 1, 1, 1, 33,
#define PPUTLUTRAITS_96   95, 97, 6, 48, 192, 9, 0, 0, 0, 0, 0, 0, 32, 2, 2, 2, 2, 2, 3
#define PPUTLUTRAITS_95   94, 96, 6, 47, 190, 9, 833, 1, 3, 7, 15, 31, 31, 5, 19
#define PPUTLUTRAITS_94   93, 95, 6, 47, 188, 9, 644, 0, 2, 6, 14, 30, 30, 2, 47
#define PPUTLUTRAITS_93   92, 94, 6, 46, 186, 9, 457, 1, 1, 5, 13, 29, 29, 3, 31
#define PPUTLUTRAITS_92   91, 93, 6, 46, 184, 9, 272, 0, 0, 4, 12, 28, 28, 2, 2, 23
#define PPUTLUTRAITS_91   90, 92, 6, 45, 182, 9, 89, 1, 3, 3, 11, 27, 27, 7, 13
#define PPUTLUTRAITS_90   89, 91, 6, 45, 180, 9, 932, 0, 2, 2, 10, 26, 26, 2, 3, 3, 5
#define PPUTLUTRAITS_89   88, 90, 6, 44, 178, 9, 753, 1, 1, 1, 9, 25, 25,
#define PPUTLUTRAITS_88   87, 89, 6, 44, 176, 9, 576, 0, 0, 0, 8, 24, 24, 2, 2, 2, 11
#define PPUTLUTRAITS_87   86, 88, 6, 43, 174, 9, 401, 1, 3, 7, 7, 23, 23, 3, 29
#define PPUTLUTRAITS_86   85, 87, 6, 43, 172, 9, 228, 0, 2, 6, 6, 22, 22, 2, 43
#define PPUTLUTRAITS_85   84, 86, 6, 42, 170, 9, 57, 1, 1, 5, 5, 21, 21, 5, 17
#define PPUTLUTRAITS_84   83, 85, 6, 42, 168, 9, 912, 0, 0, 4, 4, 20, 20, 2, 2, 3, 7
#define PPUTLUTRAITS_83   82, 84, 6, 41, 166, 9, 745, 1, 3, 3, 3, 19, 19,
#define PPUTLUTRAITS_82   81, 83, 6, 41, 164, 9, 580, 0, 2, 2, 2, 18, 18, 2, 41
#define PPUTLUTRAITS_81   80, 82, 6, 40, 162, 9, 417, 1, 1, 1, 1, 17, 17, 3, 3, 3, 3
#define PPUTLUTRAITS_80   79, 81, 6, 40, 160, 8, 256, 0, 0, 0, 0, 16, 16, 2, 2, 2, 2, 5
#define PPUTLUTRAITS_79   78, 80, 6, 39, 158, 8, 97, 1, 3, 7, 15, 15, 15,
#define PPUTLUTRAITS_78   77, 79, 6, 39, 156, 8, 964, 0, 2, 6, 14, 14, 14, 2, 3, 13
#define PPUTLUTRAITS_77   76, 78, 6, 38, 154, 8, 809, 1, 1, 5, 13, 13, 13, 7, 11
#define PPUTLUTRAITS_76   75, 77, 6, 38, 152, 8, 656, 0, 0, 4, 12, 12, 12, 2, 2, 19
#define PPUTLUTRAITS_75   74, 76, 6, 37, 150, 8, 505, 1, 3, 3, 11, 11, 11, 3, 5, 5
#define PPUTLUTRAITS_74   73, 75, 6, 37, 148, 8, 356, 0, 2, 2, 10, 10, 10, 2, 37
#define PPUTLUTRAITS_73   72, 74, 6, 36, 146, 8, 209, 1, 1, 1, 9, 9, 9,
#define PPUTLUTRAITS_72   71, 73, 6, 36, 144, 8, 64, 0, 0, 0, 8, 8, 8, 2, 2, 2, 3, 3
#define PPUTLUTRAITS_71   70, 72, 6, 35, 142, 8, 945, 1, 3, 7, 7, 7, 7,
#define PPUTLUTRAITS_70   69, 71, 6, 35, 140, 8, 804, 0, 2, 6, 6, 6, 6, 2, 5, 7
#define PPUTLUTRAITS_69   68, 70, 6, 34, 138, 8, 665, 1, 1, 5, 5, 5, 5, 3, 23
#define PPUTLUTRAITS_68   67, 69, 6, 34, 136, 8, 528, 0, 0, 4, 4, 4, 4, 2, 2, 17
#define PPUTLUTRAITS_67   66, 68, 6, 33, 134, 8, 393, 1, 3, 3, 3, 3, 3,
#define PPUTLUTRAITS_66   65, 67, 6, 33, 132, 8, 260, 0, 2, 2, 2, 2, 2, 2, 3, 11
#define PPUTLUTRAITS_65   64, 66, 6, 32, 130, 8, 129, 1, 1, 1, 1, 1, 1, 5, 13
#define PPUTLUTRAITS_64   63, 65, 6, 32, 128, 8, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2
#define PPUTLUTRAITS_63   62, 64, 5, 31, 126, 7, 897, 1, 3, 7, 15, 31, 63, 3, 3, 7
#define PPUTLUTRAITS_62   61, 63, 5, 31, 124, 7, 772, 0, 2, 6, 14, 30, 62, 2, 31
#define PPUTLUTRAITS_61   60, 62, 5, 30, 122, 7, 649, 1, 1, 5, 13, 29, 61,
#define PPUTLUTRAITS_60   59, 61, 5, 30, 120, 7, 528, 0, 0, 4, 12, 28, 60, 2, 2, 3, 5
#define PPUTLUTRAITS_59   58, 60, 5, 29, 118, 7, 409, 1, 3, 3, 11, 27, 59,
#define PPUTLUTRAITS_58   57, 59, 5, 29, 116, 7, 292, 0, 2, 2, 10, 26, 58, 2, 29
#define PPUTLUTRAITS_57   56, 58, 5, 28, 114, 7, 177, 1, 1, 1, 9, 25, 57, 3, 19
#define PPUTLUTRAITS_56   55, 57, 5, 28, 112, 7, 64, 0, 0, 0, 8, 24, 56, 2, 2, 2, 7
#define PPUTLUTRAITS_55   54, 56, 5, 27, 110, 7, 977, 1, 3, 7, 7, 23, 55, 5, 11
#define PPUTLUTRAITS_54   53, 55, 5, 27, 108, 7, 868, 0, 2, 6, 6, 22, 54, 2, 3, 3, 3
#define PPUTLUTRAITS_53   52, 54, 5, 26, 106, 7, 761, 1, 1, 5, 5, 21, 53,
#define PPUTLUTRAITS_52   51, 53, 5, 26, 104, 7, 656, 0, 0, 4, 4, 20, 52, 2, 2, 13
#define PPUTLUTRAITS_51   50, 52, 5, 25, 102, 7, 553, 1, 3, 3, 3, 19, 51, 3, 17
#define PPUTLUTRAITS_50   49, 51, 5, 25, 100, 7, 452, 0, 2, 2, 2, 18, 50, 2, 5, 5
#define PPUTLUTRAITS_49   48, 50, 5, 24, 98, 7, 353, 1, 1, 1, 1, 17, 49, 7, 7
#define PPUTLUTRAITS_48   47, 49, 5, 24, 96, 6, 256, 0, 0, 0, 0, 16, 48, 2, 2, 2, 2, 3
#define PPUTLUTRAITS_47   46, 48, 5, 23, 94, 6, 161, 1, 3, 7, 15, 15, 47,
#define PPUTLUTRAITS_46   45, 47, 5, 23, 92, 6, 68, 0, 2, 6, 14, 14, 46, 2, 23
#define PPUTLUTRAITS_45   44, 46, 5, 22, 90, 6, 1001, 1, 1, 5, 13, 13, 45, 3, 3, 5
#define PPUTLUTRAITS_44   43, 45, 5, 22, 88, 6, 912, 0, 0, 4, 12, 12, 44, 2, 2, 11
#define PPUTLUTRAITS_43   42, 44, 5, 21, 86, 6, 825, 1, 3, 3, 11, 11, 43,
#define PPUTLUTRAITS_42   41, 43, 5, 21, 84, 6, 740, 0, 2, 2, 10, 10, 42, 2, 3, 7
#define PPUTLUTRAITS_41   40, 42, 5, 20, 82, 6, 657, 1, 1, 1, 9, 9, 41,
#define PPUTLUTRAITS_40   39, 41, 5, 20, 80, 6, 576, 0, 0, 0, 8, 8, 40, 2, 2, 2, 5
#define PPUTLUTRAITS_39   38, 40, 5, 19, 78, 6, 497, 1, 3, 7, 7, 7, 39, 3, 13
#define PPUTLUTRAITS_38   37, 39, 5, 19, 76, 6, 420, 0, 2, 6, 6, 6, 38, 2, 19
#define PPUTLUTRAITS_37   36, 38, 5, 18, 74, 6, 345, 1, 1, 5, 5, 5, 37,
#define PPUTLUTRAITS_36   35, 37, 5, 18, 72, 6, 272, 0, 0, 4, 4, 4, 36, 2, 2, 3, 3
#define PPUTLUTRAITS_35   34, 36, 5, 17, 70, 5, 201, 1, 3, 3, 3, 3, 35, 5, 7
#define PPUTLUTRAITS_34   33, 35, 5, 17, 68, 5, 132, 0, 2, 2, 2, 2, 34, 2, 17
#define PPUTLUTRAITS_33   32, 34, 5, 16, 66, 5, 65, 1, 1, 1, 1, 1, 33, 3, 11
#define PPUTLUTRAITS_32   31, 33, 5, 16, 64, 5, 0, 0, 0, 0, 0, 0, 32, 2, 2, 2, 2, 2
#define PPUTLUTRAITS_31   30, 32, 4, 15, 62, 5, 961, 1, 3, 7, 15, 31, 31,
#define PPUTLUTRAITS_30   29, 31, 4, 15, 60, 5, 900, 0, 2, 6, 14, 30, 30, 2, 3, 5
#define PPUTLUTRAITS_29   28, 30, 4, 14, 58, 5, 841, 1, 1, 5, 13, 29, 29,
#define PPUTLUTRAITS_28   27, 29, 4, 14, 56, 5, 784, 0, 0, 4, 12, 28, 28, 2, 2, 7
#define PPUTLUTRAITS_27   26, 28, 4, 13, 54, 5, 729, 1, 3, 3, 11, 27, 27, 3, 3, 3
#define PPUTLUTRAITS_26   25, 27, 4, 13, 52, 5, 676, 0, 2, 2, 10, 26, 26, 2, 13
#define PPUTLUTRAITS_25   24, 26, 4, 12, 50, 5, 625, 1, 1, 1, 9, 25, 25, 5, 5
#define PPUTLUTRAITS_24   23, 25, 4, 12, 48, 4, 576, 0, 0, 0, 8, 24, 24, 2, 2, 2, 3
#define PPUTLUTRAITS_23   22, 24, 4, 11, 46, 4, 529, 1, 3, 7, 7, 23, 23,
#define PPUTLUTRAITS_22   21, 23, 4, 11, 44, 4, 484, 0, 2, 6, 6, 22, 22, 2, 11
#define PPUTLUTRAITS_21   20, 22, 4, 10, 42, 4, 441, 1, 1, 5, 5, 21, 21, 3, 7
#define PPUTLUTRAITS_20   19, 21, 4, 10, 40, 4, 400, 0, 0, 4, 4, 20, 20, 2, 2, 5
#define PPUTLUTRAITS_19   18, 20, 4, 9, 38, 4, 361, 1, 3, 3, 3, 19, 19,
#define PPUTLUTRAITS_18   17, 19, 4, 9, 36, 4, 324, 0, 2, 2, 2, 18, 18, 2, 3, 3
#define PPUTLUTRAITS_17   16, 18, 4, 8, 34, 4, 289, 1, 1, 1, 1, 17, 17,
#define PPUTLUTRAITS_16   15, 17, 4, 8, 32, 4, 256, 0, 0, 0, 0, 16, 16, 2, 2, 2, 2
#define PPUTLUTRAITS_15   14, 16, 3, 7, 30, 3, 225, 1, 3, 7, 15, 15, 15, 3, 5
#define PPUTLUTRAITS_14   13, 15, 3, 7, 28, 3, 196, 0, 2, 6, 14, 14, 14, 2, 7
#define PPUTLUTRAITS_13   12, 14, 3, 6, 26, 3, 169, 1, 1, 5, 13, 13, 13,
#define PPUTLUTRAITS_12   11, 13, 3, 6, 24, 3, 144, 0, 0, 4, 12, 12, 12, 2, 2, 3
#define PPUTLUTRAITS_11   10, 12, 3, 5, 22, 3, 121, 1, 3, 3, 11, 11, 11,
#define PPUTLUTRAITS_10   9, 11, 3, 5, 20, 3, 100, 0, 2, 2, 10, 10, 10, 2, 5
#define PPUTLUTRAITS_9    8, 10, 3, 4, 18, 3, 81, 1, 1, 1, 9, 9, 9, 3, 3
#define PPUTLUTRAITS_8    7, 9, 3, 4, 16, 2, 64, 0, 0, 0, 8, 8, 8, 2, 2, 2
#define PPUTLUTRAITS_7    6, 8, 2, 3, 14, 2, 49, 1, 3, 7, 7, 7, 7,
#define PPUTLUTRAITS_6    5, 7, 2, 3, 12, 2, 36, 0, 2, 6, 6, 6, 6, 2, 3
#define PPUTLUTRAITS_5    4, 6, 2, 2, 10, 2, 25, 1, 1, 5, 5, 5, 5,
#define PPUTLUTRAITS_4    3, 5, 2, 2, 8, 2, 16, 0, 0, 4, 4, 4, 4, 2, 2
#define PPUTLUTRAITS_3    2, 4, 1, 1, 6, 1, 9, 1, 3, 3, 3, 3, 3,
#define PPUTLUTRAITS_2    1, 3, 1, 1, 4, 1, 4, 0, 2, 2, 2, 2, 2,
#define PPUTLUTRAITS_1    0, 2, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1,
#define PPUTLUTRAITS_0    1023, 1, , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

/// full unsigned integer sequences
#define PPUTLUINT_SEQ                                                                             \
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,   \
      26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, \
      49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, \
      72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, \
      95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113,   \
      114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131,   \
      132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,   \
      150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167,   \
      168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185,   \
      186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203,   \
      204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221,   \
      222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,   \
      240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257,   \
      258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275,   \
      276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293,   \
      294, 295, 296, 297, 298, 299, 300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311,   \
      312, 313, 314, 315, 316, 317, 318, 319, 320, 321, 322, 323, 324, 325, 326, 327, 328, 329,   \
      330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347,   \
      348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359, 360, 361, 362, 363, 364, 365,   \
      366, 367, 368, 369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 380, 381, 382, 383,   \
      384, 385, 386, 387, 388, 389, 390, 391, 392, 393, 394, 395, 396, 397, 398, 399, 400, 401,   \
      402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416, 417, 418, 419,   \
      420, 421, 422, 423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433, 434, 435, 436, 437,   \
      438, 439, 440, 441, 442, 443, 444, 445, 446, 447, 448, 449, 450, 451, 452, 453, 454, 455,   \
      456, 457, 458, 459, 460, 461, 462, 463, 464, 465, 466, 467, 468, 469, 470, 471, 472, 473,   \
      474, 475, 476, 477, 478, 479, 480, 481, 482, 483, 484, 485, 486, 487, 488, 489, 490, 491,   \
      492, 493, 494, 495, 496, 497, 498, 499, 500, 501, 502, 503, 504, 505, 506, 507, 508, 509,   \
      510, 511, 512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523, 524, 525, 526, 527,   \
      528, 529, 530, 531, 532, 533, 534, 535, 536, 537, 538, 539, 540, 541, 542, 543, 544, 545,   \
      546, 547, 548, 549, 550, 551, 552, 553, 554, 555, 556, 557, 558, 559, 560, 561, 562, 563,   \
      564, 565, 566, 567, 568, 569, 570, 571, 572, 573, 574, 575, 576, 577, 578, 579, 580, 581,   \
      582, 583, 584, 585, 586, 587, 588, 589, 590, 591, 592, 593, 594, 595, 596, 597, 598, 599,   \
      600, 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611, 612, 613, 614, 615, 616, 617,   \
      618, 619, 620, 621, 622, 623, 624, 625, 626, 627, 628, 629, 630, 631, 632, 633, 634, 635,   \
      636, 637, 638, 639, 640, 641, 642, 643, 644, 645, 646, 647, 648, 649, 650, 651, 652, 653,   \
      654, 655, 656, 657, 658, 659, 660, 661, 662, 663, 664, 665, 666, 667, 668, 669, 670, 671,   \
      672, 673, 674, 675, 676, 677, 678, 679, 680, 681, 682, 683, 684, 685, 686, 687, 688, 689,   \
      690, 691, 692, 693, 694, 695, 696, 697, 698, 699, 700, 701, 702, 703, 704, 705, 706, 707,   \
      708, 709, 710, 711, 712, 713, 714, 715, 716, 717, 718, 719, 720, 721, 722, 723, 724, 725,   \
      726, 727, 728, 729, 730, 731, 732, 733, 734, 735, 736, 737, 738, 739, 740, 741, 742, 743,   \
      744, 745, 746, 747, 748, 749, 750, 751, 752, 753, 754, 755, 756, 757, 758, 759, 760, 761,   \
      762, 763, 764, 765, 766, 767, 768, 769, 770, 771, 772, 773, 774, 775, 776, 777, 778, 779,   \
      780, 781, 782, 783, 784, 785, 786, 787, 788, 789, 790, 791, 792, 793, 794, 795, 796, 797,   \
      798, 799, 800, 801, 802, 803, 804, 805, 806, 807, 808, 809, 810, 811, 812, 813, 814, 815,   \
      816, 817, 818, 819, 820, 821, 822, 823, 824, 825, 826, 827, 828, 829, 830, 831, 832, 833,   \
      834, 835, 836, 837, 838, 839, 840, 841, 842, 843, 844, 845, 846, 847, 848, 849, 850, 851,   \
      852, 853, 854, 855, 856, 857, 858, 859, 860, 861, 862, 863, 864, 865, 866, 867, 868, 869,   \
      870, 871, 872, 873, 874, 875, 876, 877, 878, 879, 880, 881, 882, 883, 884, 885, 886, 887,   \
      888, 889, 890, 891, 892, 893, 894, 895, 896, 897, 898, 899, 900, 901, 902, 903, 904, 905,   \
      906, 907, 908, 909, 910, 911, 912, 913, 914, 915, 916, 917, 918, 919, 920, 921, 922, 923,   \
      924, 925, 926, 927, 928, 929, 930, 931, 932, 933, 934, 935, 936, 937, 938, 939, 940, 941,   \
      942, 943, 944, 945, 946, 947, 948, 949, 950, 951, 952, 953, 954, 955, 956, 957, 958, 959,   \
      960, 961, 962, 963, 964, 965, 966, 967, 968, 969, 970, 971, 972, 973, 974, 975, 976, 977,   \
      978, 979, 980, 981, 982, 983, 984, 985, 986, 987, 988, 989, 990, 991, 992, 993, 994, 995,   \
      996, 997, 998, 999, 1000, 1001, 1002, 1003, 1004, 1005, 1006, 1007, 1008, 1009, 1010, 1011, \
      1012, 1013, 1014, 1015, 1016, 1017, 1018, 1019, 1020, 1021, 1022, 1023
#define PPUTLUINT_RSEQ                                                                             \
  1023, 1022, 1021, 1020, 1019, 1018, 1017, 1016, 1015, 1014, 1013, 1012, 1011, 1010, 1009, 1008,  \
      1007, 1006, 1005, 1004, 1003, 1002, 1001, 1000, 999, 998, 997, 996, 995, 994, 993, 992, 991, \
      990, 989, 988, 987, 986, 985, 984, 983, 982, 981, 980, 979, 978, 977, 976, 975, 974, 973,    \
      972, 971, 970, 969, 968, 967, 966, 965, 964, 963, 962, 961, 960, 959, 958, 957, 956, 955,    \
      954, 953, 952, 951, 950, 949, 948, 947, 946, 945, 944, 943, 942, 941, 940, 939, 938, 937,    \
      936, 935, 934, 933, 932, 931, 930, 929, 928, 927, 926, 925, 924, 923, 922, 921, 920, 919,    \
      918, 917, 916, 915, 914, 913, 912, 911, 910, 909, 908, 907, 906, 905, 904, 903, 902, 901,    \
      900, 899, 898, 897, 896, 895, 894, 893, 892, 891, 890, 889, 888, 887, 886, 885, 884, 883,    \
      882, 881, 880, 879, 878, 877, 876, 875, 874, 873, 872, 871, 870, 869, 868, 867, 866, 865,    \
      864, 863, 862, 861, 860, 859, 858, 857, 856, 855, 854, 853, 852, 851, 850, 849, 848, 847,    \
      846, 845, 844, 843, 842, 841, 840, 839, 838, 837, 836, 835, 834, 833, 832, 831, 830, 829,    \
      828, 827, 826, 825, 824, 823, 822, 821, 820, 819, 818, 817, 816, 815, 814, 813, 812, 811,    \
      810, 809, 808, 807, 806, 805, 804, 803, 802, 801, 800, 799, 798, 797, 796, 795, 794, 793,    \
      792, 791, 790, 789, 788, 787, 786, 785, 784, 783, 782, 781, 780, 779, 778, 777, 776, 775,    \
      774, 773, 772, 771, 770, 769, 768, 767, 766, 765, 764, 763, 762, 761, 760, 759, 758, 757,    \
      756, 755, 754, 753, 752, 751, 750, 749, 748, 747, 746, 745, 744, 743, 742, 741, 740, 739,    \
      738, 737, 736, 735, 734, 733, 732, 731, 730, 729, 728, 727, 726, 725, 724, 723, 722, 721,    \
      720, 719, 718, 717, 716, 715, 714, 713, 712, 711, 710, 709, 708, 707, 706, 705, 704, 703,    \
      702, 701, 700, 699, 698, 697, 696, 695, 694, 693, 692, 691, 690, 689, 688, 687, 686, 685,    \
      684, 683, 682, 681, 680, 679, 678, 677, 676, 675, 674, 673, 672, 671, 670, 669, 668, 667,    \
      666, 665, 664, 663, 662, 661, 660, 659, 658, 657, 656, 655, 654, 653, 652, 651, 650, 649,    \
      648, 647, 646, 645, 644, 643, 642, 641, 640, 639, 638, 637, 636, 635, 634, 633, 632, 631,    \
      630, 629, 628, 627, 626, 625, 624, 623, 622, 621, 620, 619, 618, 617, 616, 615, 614, 613,    \
      612, 611, 610, 609, 608, 607, 606, 605, 604, 603, 602, 601, 600, 599, 598, 597, 596, 595,    \
      594, 593, 592, 591, 590, 589, 588, 587, 586, 585, 584, 583, 582, 581, 580, 579, 578, 577,    \
      576, 575, 574, 573, 572, 571, 570, 569, 568, 567, 566, 565, 564, 563, 562, 561, 560, 559,    \
      558, 557, 556, 555, 554, 553, 552, 551, 550, 549, 548, 547, 546, 545, 544, 543, 542, 541,    \
      540, 539, 538, 537, 536, 535, 534, 533, 532, 531, 530, 529, 528, 527, 526, 525, 524, 523,    \
      522, 521, 520, 519, 518, 517, 516, 515, 514, 513, 512, 511, 510, 509, 508, 507, 506, 505,    \
      504, 503, 502, 501, 500, 499, 498, 497, 496, 495, 494, 493, 492, 491, 490, 489, 488, 487,    \
      486, 485, 484, 483, 482, 481, 480, 479, 478, 477, 476, 475, 474, 473, 472, 471, 470, 469,    \
      468, 467, 466, 465, 464, 463, 462, 461, 460, 459, 458, 457, 456, 455, 454, 453, 452, 451,    \
      450, 449, 448, 447, 446, 445, 444, 443, 442, 441, 440, 439, 438, 437, 436, 435, 434, 433,    \
      432, 431, 430, 429, 428, 427, 426, 425, 424, 423, 422, 421, 420, 419, 418, 417, 416, 415,    \
      414, 413, 412, 411, 410, 409, 408, 407, 406, 405, 404, 403, 402, 401, 400, 399, 398, 397,    \
      396, 395, 394, 393, 392, 391, 390, 389, 388, 387, 386, 385, 384, 383, 382, 381, 380, 379,    \
      378, 377, 376, 375, 374, 373, 372, 371, 370, 369, 368, 367, 366, 365, 364, 363, 362, 361,    \
      360, 359, 358, 357, 356, 355, 354, 353, 352, 351, 350, 349, 348, 347, 346, 345, 344, 343,    \
      342, 341, 340, 339, 338, 337, 336, 335, 334, 333, 332, 331, 330, 329, 328, 327, 326, 325,    \
      324, 323, 322, 321, 320, 319, 318, 317, 316, 315, 314, 313, 312, 311, 310, 309, 308, 307,    \
      306, 305, 304, 303, 302, 301, 300, 299, 298, 297, 296, 295, 294, 293, 292, 291, 290, 289,    \
      288, 287, 286, 285, 284, 283, 282, 281, 280, 279, 278, 277, 276, 275, 274, 273, 272, 271,    \
      270, 269, 268, 267, 266, 265, 264, 263, 262, 261, 260, 259, 258, 257, 256, 255, 254, 253,    \
      252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 237, 236, 235,    \
      234, 233, 232, 231, 230, 229, 228, 227, 226, 225, 224, 223, 222, 221, 220, 219, 218, 217,    \
      216, 215, 214, 213, 212, 211, 210, 209, 208, 207, 206, 205, 204, 203, 202, 201, 200, 199,    \
      198, 197, 196, 195, 194, 193, 192, 191, 190, 189, 188, 187, 186, 185, 184, 183, 182, 181,    \
      180, 179, 178, 177, 176, 175, 174, 173, 172, 171, 170, 169, 168, 167, 166, 165, 164, 163,    \
      162, 161, 160, 159, 158, 157, 156, 155, 154, 153, 152, 151, 150, 149, 148, 147, 146, 145,    \
      144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 127,    \
      126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109,    \
      108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, \
      87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65,  \
      64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42,  \
      41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19,  \
      18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_none]
/// ----------------
/// detects if args is nothing.
///
/// PTL_IS_NONE()          // 1
/// PTL_IS_NONE(foo)       // 0
/// PTL_IS_NONE(foo, bar)  // 0
/// PTL_IS_NONE(PTL_ESC()) // 1
#define PTL_IS_NONE(...) /* -> bool */ PPUTLIS_NONE_O##__VA_OPT__(0)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_NONE_O0 0
#define PPUTLIS_NONE_O  1

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_some]
/// ----------------
/// detects if args is something.
///
/// PTL_IS_SOME()          // 0
/// PTL_IS_SOME(foo)       // 1
/// PTL_IS_SOME(foo, bar)  // 1
/// PTL_IS_SOME(PTL_ESC()) // 0
#define PTL_IS_SOME(...) /* -> bool */ PPUTLIS_SOME_O##__VA_OPT__(1)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_SOME_O1 1
#define PPUTLIS_SOME_O  0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_tuple]
/// -----------------
/// detects if args is a tuple.
///
/// PTL_IS_TUPLE()       // 0
/// PTL_IS_TUPLE(1, 2)   // 0
/// PTL_IS_TUPLE(())     // 1
/// PTL_IS_TUPLE((1, 2)) // 1
#define PTL_IS_TUPLE(...) /* -> bool */ PTL_IS_NONE(PTL_EAT __VA_ARGS__)

/// [traits.is_bool]
/// ----------------
/// detects if args is a bool.
///
/// PTL_IS_BOOL()     // 0
/// PTL_IS_BOOL(0)    // 1
/// PTL_IS_BOOL(1)    // 1
/// PTL_IS_BOOL(0, 1) // 0
/// PTL_IS_BOOL((0))  // 0
#define PTL_IS_BOOL(...) /* -> bool */ \
  PTL_CAT(PPUTLIS_BOOL_, PPUTLBOOL_O(__VA_ARGS__.)(__VA_ARGS__)(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_BOOL_PPUTLBOOL_PASS 1
#define PPUTLIS_BOOL_PPUTLBOOL_FAIL 0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

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
/// PTL_IS_UINT(1023)   // 1
#define PTL_IS_UINT(...) /* -> bool */ \
  PTL_CAT(PPUTLIS_UINT_, PPUTLUINT_O(__VA_ARGS__.)(__VA_ARGS__)(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_UINT_PPUTLUINT_PASS 1
#define PPUTLIS_UINT_PPUTLUINT_FAIL 0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.items]
/// --------------
/// extracts tuple items.
///
/// PTL_ITEMS(())        // <nothing>
/// PTL_ITEMS((a))       // a
/// PTL_ITEMS((a, b))    // a, b
/// PTL_ITEMS((a, b, c)) // a, b, c
#define PTL_ITEMS(/* v: tuple */...) /* -> ...v */ PPUTLITEMS_X(PTL_TUPLE(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLITEMS_X(...) PTL_ESC __VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.size]
/// -------------
/// computes the uint size of args in O(1) time.
/// fails if too many args passed.
///
/// PTL_SIZE()     // 0
/// PTL_SIZE(a)    // 1
/// PTL_SIZE(a, b) // 2
/// PTL_SIZE(, )   // 2
#define PTL_SIZE(...) /* -> uint */ \
  PPUTLSIZE_X(PTL_ISTR([PTL_SIZE] too many args : __VA_ARGS__), PPUTLSIZE, __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLSIZE_X(_err, _, ...)                                                                \
  PPUTLSIZE_READ(                                                                                \
      _err, __VA_ARGS__ __VA_OPT__(, ) _(1023), _(1022), _(1021), _(1020), _(1019), _(1018),     \
      _(1017), _(1016), _(1015), _(1014), _(1013), _(1012), _(1011), _(1010), _(1009), _(1008),  \
      _(1007), _(1006), _(1005), _(1004), _(1003), _(1002), _(1001), _(1000), _(999), _(998),    \
      _(997), _(996), _(995), _(994), _(993), _(992), _(991), _(990), _(989), _(988), _(987),    \
      _(986), _(985), _(984), _(983), _(982), _(981), _(980), _(979), _(978), _(977), _(976),    \
      _(975), _(974), _(973), _(972), _(971), _(970), _(969), _(968), _(967), _(966), _(965),    \
      _(964), _(963), _(962), _(961), _(960), _(959), _(958), _(957), _(956), _(955), _(954),    \
      _(953), _(952), _(951), _(950), _(949), _(948), _(947), _(946), _(945), _(944), _(943),    \
      _(942), _(941), _(940), _(939), _(938), _(937), _(936), _(935), _(934), _(933), _(932),    \
      _(931), _(930), _(929), _(928), _(927), _(926), _(925), _(924), _(923), _(922), _(921),    \
      _(920), _(919), _(918), _(917), _(916), _(915), _(914), _(913), _(912), _(911), _(910),    \
      _(909), _(908), _(907), _(906), _(905), _(904), _(903), _(902), _(901), _(900), _(899),    \
      _(898), _(897), _(896), _(895), _(894), _(893), _(892), _(891), _(890), _(889), _(888),    \
      _(887), _(886), _(885), _(884), _(883), _(882), _(881), _(880), _(879), _(878), _(877),    \
      _(876), _(875), _(874), _(873), _(872), _(871), _(870), _(869), _(868), _(867), _(866),    \
      _(865), _(864), _(863), _(862), _(861), _(860), _(859), _(858), _(857), _(856), _(855),    \
      _(854), _(853), _(852), _(851), _(850), _(849), _(848), _(847), _(846), _(845), _(844),    \
      _(843), _(842), _(841), _(840), _(839), _(838), _(837), _(836), _(835), _(834), _(833),    \
      _(832), _(831), _(830), _(829), _(828), _(827), _(826), _(825), _(824), _(823), _(822),    \
      _(821), _(820), _(819), _(818), _(817), _(816), _(815), _(814), _(813), _(812), _(811),    \
      _(810), _(809), _(808), _(807), _(806), _(805), _(804), _(803), _(802), _(801), _(800),    \
      _(799), _(798), _(797), _(796), _(795), _(794), _(793), _(792), _(791), _(790), _(789),    \
      _(788), _(787), _(786), _(785), _(784), _(783), _(782), _(781), _(780), _(779), _(778),    \
      _(777), _(776), _(775), _(774), _(773), _(772), _(771), _(770), _(769), _(768), _(767),    \
      _(766), _(765), _(764), _(763), _(762), _(761), _(760), _(759), _(758), _(757), _(756),    \
      _(755), _(754), _(753), _(752), _(751), _(750), _(749), _(748), _(747), _(746), _(745),    \
      _(744), _(743), _(742), _(741), _(740), _(739), _(738), _(737), _(736), _(735), _(734),    \
      _(733), _(732), _(731), _(730), _(729), _(728), _(727), _(726), _(725), _(724), _(723),    \
      _(722), _(721), _(720), _(719), _(718), _(717), _(716), _(715), _(714), _(713), _(712),    \
      _(711), _(710), _(709), _(708), _(707), _(706), _(705), _(704), _(703), _(702), _(701),    \
      _(700), _(699), _(698), _(697), _(696), _(695), _(694), _(693), _(692), _(691), _(690),    \
      _(689), _(688), _(687), _(686), _(685), _(684), _(683), _(682), _(681), _(680), _(679),    \
      _(678), _(677), _(676), _(675), _(674), _(673), _(672), _(671), _(670), _(669), _(668),    \
      _(667), _(666), _(665), _(664), _(663), _(662), _(661), _(660), _(659), _(658), _(657),    \
      _(656), _(655), _(654), _(653), _(652), _(651), _(650), _(649), _(648), _(647), _(646),    \
      _(645), _(644), _(643), _(642), _(641), _(640), _(639), _(638), _(637), _(636), _(635),    \
      _(634), _(633), _(632), _(631), _(630), _(629), _(628), _(627), _(626), _(625), _(624),    \
      _(623), _(622), _(621), _(620), _(619), _(618), _(617), _(616), _(615), _(614), _(613),    \
      _(612), _(611), _(610), _(609), _(608), _(607), _(606), _(605), _(604), _(603), _(602),    \
      _(601), _(600), _(599), _(598), _(597), _(596), _(595), _(594), _(593), _(592), _(591),    \
      _(590), _(589), _(588), _(587), _(586), _(585), _(584), _(583), _(582), _(581), _(580),    \
      _(579), _(578), _(577), _(576), _(575), _(574), _(573), _(572), _(571), _(570), _(569),    \
      _(568), _(567), _(566), _(565), _(564), _(563), _(562), _(561), _(560), _(559), _(558),    \
      _(557), _(556), _(555), _(554), _(553), _(552), _(551), _(550), _(549), _(548), _(547),    \
      _(546), _(545), _(544), _(543), _(542), _(541), _(540), _(539), _(538), _(537), _(536),    \
      _(535), _(534), _(533), _(532), _(531), _(530), _(529), _(528), _(527), _(526), _(525),    \
      _(524), _(523), _(522), _(521), _(520), _(519), _(518), _(517), _(516), _(515), _(514),    \
      _(513), _(512), _(511), _(510), _(509), _(508), _(507), _(506), _(505), _(504), _(503),    \
      _(502), _(501), _(500), _(499), _(498), _(497), _(496), _(495), _(494), _(493), _(492),    \
      _(491), _(490), _(489), _(488), _(487), _(486), _(485), _(484), _(483), _(482), _(481),    \
      _(480), _(479), _(478), _(477), _(476), _(475), _(474), _(473), _(472), _(471), _(470),    \
      _(469), _(468), _(467), _(466), _(465), _(464), _(463), _(462), _(461), _(460), _(459),    \
      _(458), _(457), _(456), _(455), _(454), _(453), _(452), _(451), _(450), _(449), _(448),    \
      _(447), _(446), _(445), _(444), _(443), _(442), _(441), _(440), _(439), _(438), _(437),    \
      _(436), _(435), _(434), _(433), _(432), _(431), _(430), _(429), _(428), _(427), _(426),    \
      _(425), _(424), _(423), _(422), _(421), _(420), _(419), _(418), _(417), _(416), _(415),    \
      _(414), _(413), _(412), _(411), _(410), _(409), _(408), _(407), _(406), _(405), _(404),    \
      _(403), _(402), _(401), _(400), _(399), _(398), _(397), _(396), _(395), _(394), _(393),    \
      _(392), _(391), _(390), _(389), _(388), _(387), _(386), _(385), _(384), _(383), _(382),    \
      _(381), _(380), _(379), _(378), _(377), _(376), _(375), _(374), _(373), _(372), _(371),    \
      _(370), _(369), _(368), _(367), _(366), _(365), _(364), _(363), _(362), _(361), _(360),    \
      _(359), _(358), _(357), _(356), _(355), _(354), _(353), _(352), _(351), _(350), _(349),    \
      _(348), _(347), _(346), _(345), _(344), _(343), _(342), _(341), _(340), _(339), _(338),    \
      _(337), _(336), _(335), _(334), _(333), _(332), _(331), _(330), _(329), _(328), _(327),    \
      _(326), _(325), _(324), _(323), _(322), _(321), _(320), _(319), _(318), _(317), _(316),    \
      _(315), _(314), _(313), _(312), _(311), _(310), _(309), _(308), _(307), _(306), _(305),    \
      _(304), _(303), _(302), _(301), _(300), _(299), _(298), _(297), _(296), _(295), _(294),    \
      _(293), _(292), _(291), _(290), _(289), _(288), _(287), _(286), _(285), _(284), _(283),    \
      _(282), _(281), _(280), _(279), _(278), _(277), _(276), _(275), _(274), _(273), _(272),    \
      _(271), _(270), _(269), _(268), _(267), _(266), _(265), _(264), _(263), _(262), _(261),    \
      _(260), _(259), _(258), _(257), _(256), _(255), _(254), _(253), _(252), _(251), _(250),    \
      _(249), _(248), _(247), _(246), _(245), _(244), _(243), _(242), _(241), _(240), _(239),    \
      _(238), _(237), _(236), _(235), _(234), _(233), _(232), _(231), _(230), _(229), _(228),    \
      _(227), _(226), _(225), _(224), _(223), _(222), _(221), _(220), _(219), _(218), _(217),    \
      _(216), _(215), _(214), _(213), _(212), _(211), _(210), _(209), _(208), _(207), _(206),    \
      _(205), _(204), _(203), _(202), _(201), _(200), _(199), _(198), _(197), _(196), _(195),    \
      _(194), _(193), _(192), _(191), _(190), _(189), _(188), _(187), _(186), _(185), _(184),    \
      _(183), _(182), _(181), _(180), _(179), _(178), _(177), _(176), _(175), _(174), _(173),    \
      _(172), _(171), _(170), _(169), _(168), _(167), _(166), _(165), _(164), _(163), _(162),    \
      _(161), _(160), _(159), _(158), _(157), _(156), _(155), _(154), _(153), _(152), _(151),    \
      _(150), _(149), _(148), _(147), _(146), _(145), _(144), _(143), _(142), _(141), _(140),    \
      _(139), _(138), _(137), _(136), _(135), _(134), _(133), _(132), _(131), _(130), _(129),    \
      _(128), _(127), _(126), _(125), _(124), _(123), _(122), _(121), _(120), _(119), _(118),    \
      _(117), _(116), _(115), _(114), _(113), _(112), _(111), _(110), _(109), _(108), _(107),    \
      _(106), _(105), _(104), _(103), _(102), _(101), _(100), _(99), _(98), _(97), _(96), _(95), \
      _(94), _(93), _(92), _(91), _(90), _(89), _(88), _(87), _(86), _(85), _(84), _(83), _(82), \
      _(81), _(80), _(79), _(78), _(77), _(76), _(75), _(74), _(73), _(72), _(71), _(70), _(69), \
      _(68), _(67), _(66), _(65), _(64), _(63), _(62), _(61), _(60), _(59), _(58), _(57), _(56), \
      _(55), _(54), _(53), _(52), _(51), _(50), _(49), _(48), _(47), _(46), _(45), _(44), _(43), \
      _(42), _(41), _(40), _(39), _(38), _(37), _(36), _(35), _(34), _(33), _(32), _(31), _(30), \
      _(29), _(28), _(27), _(26), _(25), _(24), _(23), _(22), _(21), _(20), _(19), _(18), _(17), \
      _(16), _(15), _(14), _(13), _(12), _(11), _(10), _(9), _(8), _(7), _(6), _(5), _(4), _(3), \
      _(2), _(1), _(0))
#define PPUTLSIZE_READ(                                                                          \
    _err, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, A, B, C, \
    D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, ba, bb, bc, bd, be, bf, \
    bg, bh, bi, bj, bk, bl, bm, bn, bo, bp, bq, br, bs, bt, bu, bv, bw, bx, by, bz, bA, bB, bC,  \
    bD, bE, bF, bG, bH, bI, bJ, bK, bL, bM, bN, bO, bP, bQ, bR, bS, bT, bU, bV, bW, bX, bY, bZ,  \
    ca, cb, cc, cd, ce, cf, cg, ch, ci, cj, ck, cl, cm, cn, co, cp, cq, cr, cs, ct, cu, cv, cw,  \
    cx, cy, cz, cA, cB, cC, cD, cE, cF, cG, cH, cI, cJ, cK, cL, cM, cN, cO, cP, cQ, cR, cS, cT,  \
    cU, cV, cW, cX, cY, cZ, da, db, dc, dd, de, df, dg, dh, di, dj, dk, dl, dm, dn, do, dp, dq,  \
    dr, ds, dt, du, dv, dw, dx, dy, dz, dA, dB, dC, dD, dE, dF, dG, dH, dI, dJ, dK, dL, dM, dN,  \
    dO, dP, dQ, dR, dS, dT, dU, dV, dW, dX, dY, dZ, ea, eb, ec, ed, ee, ef, eg, eh, ei, ej, ek,  \
    el, em, en, eo, ep, eq, er, es, et, eu, ev, ew, ex, ey, ez, eA, eB, eC, eD, eE, eF, eG, eH,  \
    eI, eJ, eK, eL, eM, eN, eO, eP, eQ, eR, eS, eT, eU, eV, eW, eX, eY, eZ, fa, fb, fc, fd, fe,  \
    ff, fg, fh, fi, fj, fk, fl, fm, fn, fo, fp, fq, fr, fs, ft, fu, fv, fw, fx, fy, fz, fA, fB,  \
    fC, fD, fE, fF, fG, fH, fI, fJ, fK, fL, fM, fN, fO, fP, fQ, fR, fS, fT, fU, fV, fW, fX, fY,  \
    fZ, ga, gb, gc, gd, ge, gf, gg, gh, gi, gj, gk, gl, gm, gn, go, gp, gq, gr, gs, gt, gu, gv,  \
    gw, gx, gy, gz, gA, gB, gC, gD, gE, gF, gG, gH, gI, gJ, gK, gL, gM, gN, gO, gP, gQ, gR, gS,  \
    gT, gU, gV, gW, gX, gY, gZ, ha, hb, hc, hd, he, hf, hg, hh, hi, hj, hk, hl, hm, hn, ho, hp,  \
    hq, hr, hs, ht, hu, hv, hw, hx, hy, hz, hA, hB, hC, hD, hE, hF, hG, hH, hI, hJ, hK, hL, hM,  \
    hN, hO, hP, hQ, hR, hS, hT, hU, hV, hW, hX, hY, hZ, ia, ib, ic, id, ie, if, ig, ih, ii, ij,  \
    ik, il, im, in, io, ip, iq, ir, is, it, iu, iv, iw, ix, iy, iz, iA, iB, iC, iD, iE, iF, iG,  \
    iH, iI, iJ, iK, iL, iM, iN, iO, iP, iQ, iR, iS, iT, iU, iV, iW, iX, iY, iZ, ja, jb, jc, jd,  \
    je, jf, jg, jh, ji, jj, jk, jl, jm, jn, jo, jp, jq, jr, js, jt, ju, jv, jw, jx, jy, jz, jA,  \
    jB, jC, jD, jE, jF, jG, jH, jI, jJ, jK, jL, jM, jN, jO, jP, jQ, jR, jS, jT, jU, jV, jW, jX,  \
    jY, jZ, ka, kb, kc, kd, ke, kf, kg, kh, ki, kj, kk, kl, km, kn, ko, kp, kq, kr, ks, kt, ku,  \
    kv, kw, kx, ky, kz, kA, kB, kC, kD, kE, kF, kG, kH, kI, kJ, kK, kL, kM, kN, kO, kP, kQ, kR,  \
    kS, kT, kU, kV, kW, kX, kY, kZ, la, lb, lc, ld, le, lf, lg, lh, li, lj, lk, ll, lm, ln, lo,  \
    lp, lq, lr, ls, lt, lu, lv, lw, lx, ly, lz, lA, lB, lC, lD, lE, lF, lG, lH, lI, lJ, lK, lL,  \
    lM, lN, lO, lP, lQ, lR, lS, lT, lU, lV, lW, lX, lY, lZ, ma, mb, mc, md, me, mf, mg, mh, mi,  \
    mj, mk, ml, mm, mn, mo, mp, mq, mr, ms, mt, mu, mv, mw, mx, my, mz, mA, mB, mC, mD, mE, mF,  \
    mG, mH, mI, mJ, mK, mL, mM, mN, mO, mP, mQ, mR, mS, mT, mU, mV, mW, mX, mY, mZ, na, nb, nc,  \
    nd, ne, nf, ng, nh, ni, nj, nk, nl, nm, nn, no, np, nq, nr, ns, nt, nu, nv, nw, nx, ny, nz,  \
    nA, nB, nC, nD, nE, nF, nG, nH, nI, nJ, nK, nL, nM, nN, nO, nP, nQ, nR, nS, nT, nU, nV, nW,  \
    nX, nY, nZ, oa, ob, oc, od, oe, of, og, oh, oi, oj, ok, ol, om, on, oo, op, oq, _or, os, ot, \
    ou, ov, ow, ox, oy, oz, oA, oB, oC, oD, oE, oF, oG, oH, oI, oJ, oK, oL, oM, oN, oO, oP, oQ,  \
    oR, oS, oT, oU, oV, oW, oX, oY, oZ, pa, pb, pc, pd, pe, pf, pg, ph, pi, pj, pk, pl, pm, pn,  \
    po, pp, pq, pr, ps, pt, pu, pv, pw, px, py, pz, pA, pB, pC, pD, pE, pF, pG, pH, pI, pJ, pK,  \
    pL, pM, pN, pO, pP, pQ, pR, pS, pT, pU, pV, pW, pX, pY, pZ, qa, qb, qc, qd, qe, qf, qg, qh,  \
    qi, qj, qk, ql, qm, qn, qo, qp, qq, qr, qs, qt, qu, qv, qw, qx, qy, qz, qA, qB, qC, qD, qE,  \
    qF, qG, qH, qI, qJ, qK, qL, qM, qN, qO, qP, qQ, qR, qS, qT, qU, qV, qW, qX, qY, qZ, ra, rb,  \
    rc, rd, re, rf, rg, rh, ri, rj, rk, rl, rm, rn, ro, rp, rq, rr, rs, rt, ru, rv, rw, rx, ry,  \
    rz, rA, rB, rC, rD, rE, rF, rG, rH, rI, rJ, rK, rL, rM, rN, rO, rP, rQ, rR, rS, rT, rU, rV,  \
    rW, rX, rY, rZ, sa, sb, sc, sd, se, sf, sg, sh, si, sj, sk, sl, sm, sn, so, sp, sq, sr, ss,  \
    st, su, sv, sw, sx, sy, sz, sA, sB, sC, sD, sE, sF, sG, sH, sI, sJ, sK, sL, sM, sN, sO, sP,  \
    sQ, sR, sS, sT, sU, sV, sW, sX, sY, sZ, ta, tb, tc, td, te, tf, tg, th, ti, tj, tk, tl, tm,  \
    tn, to, tp, tq, tr, ts, tt, tu, tv, tw, tx, ty, tz, tA, tB, tC, tD, tE, tF, tG, tH, tI, _sz, \
    ...)                                                                                         \
  PTL_CAT(PPUTLSIZE_READ_, PTL_IS_NONE(PPUTLSIZE_##_sz))(_err, _sz)
#define PPUTLSIZE_READ_1(_err, _sz)   PPUTLSIZE_READ_##_sz
#define PPUTLSIZE_READ_0(_err, _sz)   PTL_FAIL(_err)
#define PPUTLSIZE_READ_PPUTLSIZE(...) __VA_ARGS__
#define PPUTLSIZE_PPUTLSIZE(...)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [numeric.inc]
/// -------------
/// uint increment w/ overflow.
///
/// PTL_INC(1023) // 0
#define PTL_INC(/* n: uint */...) /* -> uint{n + 1} */ \
  PPUTLINC_X(PTL_CAT(PPUTLUTRAITS_, PTL_UINT(__VA_ARGS__)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLINC_X(...) PPUTLINC_XX(__VA_ARGS__)

#define PPUTLINC_XX(de, in, lg, dv, ml, sq, pw, m2, m4, m8, m16, m32, m64, ...) in

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [numeric.dec]
/// -------------
/// uint decrement w/ underflow.
///
/// PTL_DEC(0) // 1023
#define PTL_DEC(/* n: uint */...) /* -> uint{n - 1} */ \
  PPUTLDEC_X(PTL_CAT(PPUTLUTRAITS_, PTL_UINT(__VA_ARGS__)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLDEC_X(...) PPUTLDEC_XX(__VA_ARGS__)

#define PPUTLDEC_XX(de, in, lg, dv, ml, sq, pw, m2, m4, m8, m16, m32, m64, ...) de

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [numeric.eqz]
/// -------------
/// detects if uint n is zero.
///
/// PTL_EQZ(0)    // 1
/// PTL_EQZ(1023) // 0
#define PTL_EQZ(/* n: uint */...) /* -> uint{n == 0} */ \
  PTL_IS_NONE(PTL_CAT(PPUTLEQZ_, PTL_UINT(__VA_ARGS__)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLEQZ_0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [numeric.nez]
/// -------------
/// detects if uint n is not zero.
///
/// PTL_NEZ(0)    // 0
/// PTL_NEZ(1023) // 1
#define PTL_NEZ(/* n: uint */...) /* -> uint{n != 0} */ \
  PTL_IS_SOME(PTL_CAT(PPUTLEQZ_, PTL_UINT(__VA_ARGS__)))

/// [numeric.mul2]
/// --------------
/// O(1) uint multiplication by 2 with overflow.
///
/// PTL_MUL2(0)   // 0
/// PTL_MUL2(511) // 1022
#define PTL_MUL2(/* n: uint */...) /* -> uint{n * 2} */ \
  PPUTLMUL2_X(PTL_CAT(PPUTLUTRAITS_, PTL_UINT(__VA_ARGS__)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLMUL2_X(...) PPUTLMUL2_XX(__VA_ARGS__)

#define PPUTLMUL2_XX(de, in, lg, dv, ml, sq, pw, m2, m4, m8, m16, m32, m64, ...) ml

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [numeric.sqrt]
/// --------------
/// O(1) uint sqrt.
///
/// PTL_SQRT(4)    // 2
/// PTL_SQRT(1023) // 31
#define PTL_SQRT(/* n: uint */...) /* -> uint{sqrt(n)} */ \
  PPUTLSQRT_X(PTL_CAT(PPUTLUTRAITS_, PTL_UINT(__VA_ARGS__)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLSQRT_X(...) PPUTLSQRT_XX(__VA_ARGS__)

#define PPUTLSQRT_XX(de, in, lg, dv, ml, sq, pw, m2, m4, m8, m16, m32, m64, ...) sq

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [numeric.pow2]
/// --------------
/// O(1) uint pow2.
///
/// PTL_POW2(31)  // 961
/// PTL_POW2(255) // 513
#define PTL_POW2(/* n: uint */...) /* -> uint{n * n} */ \
  PPUTLPOW2_X(PTL_CAT(PPUTLUTRAITS_, PTL_UINT(__VA_ARGS__)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLPOW2_X(...) PPUTLPOW2_XX(__VA_ARGS__)

#define PPUTLPOW2_XX(de, in, lg, dv, ml, sq, pw, m2, m4, m8, m16, m32, m64, ...) pw

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [numeric.div2]
/// --------------
/// O(1) uint division by 2.
///
/// PTL_DIV2(0)    // 0
/// PTL_DIV2(1023) // 511
#define PTL_DIV2(/* n: uint */...) /* -> uint{n / 2} */ \
  PPUTLDIV2_X(PTL_CAT(PPUTLUTRAITS_, PTL_UINT(__VA_ARGS__)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLDIV2_X(...) PPUTLDIV2_XX(__VA_ARGS__)

#define PPUTLDIV2_XX(de, in, lg, dv, ml, sq, pw, m2, m4, m8, m16, m32, m64, ...) dv

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [numeric.log2]
/// --------------
/// O(1) positive uint logarithm base 2.
///
/// PTL_LOG2(1)    // 0
/// PTL_LOG2(1023) // 9
#define PTL_LOG2(/* n: uint */...) /* -> uint{log2 n} */ \
  PTL_CAT(PPUTLLOG2_NEZ, PTL_NEZ(__VA_ARGS__))(PTL_CAT(PPUTLUTRAITS_, PTL_UINT(__VA_ARGS__)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLLOG2_NEZ0(...) PTL_FAIL("[PTL_LOG2] log2 of zero not supported")
#define PPUTLLOG2_NEZ1(...) PPUTLLOG2_NEZ1_X(__VA_ARGS__)

#define PPUTLLOG2_NEZ1_X(de, in, lg, dv, ml, sq, pw, m2, m4, m8, m16, m32, m64, ...) lg

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [numeric.mod2]
/// --------------
/// O(1) uint modulo by 2.
///
/// PTL_MOD2(341)  // 1
/// PTL_MOD2(1023) // 1
#define PTL_MOD2(/* n: uint */...) /* -> uint{n % 2} */ \
  PPUTLMOD2_X(PTL_CAT(PPUTLUTRAITS_, PTL_UINT(__VA_ARGS__)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLMOD2_X(...) PPUTLMOD2_XX(__VA_ARGS__)

#define PPUTLMOD2_XX(de, in, lg, dv, ml, sq, pw, m2, m4, m8, m16, m32, m64, ...) m2

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [numeric.mod4]
/// --------------
/// O(1) uint modulo by 4.
///
/// PTL_MOD4(204)  // 0
/// PTL_MOD4(1023) // 3
#define PTL_MOD4(/* n: uint */...) /* -> uint{n % 4} */ \
  PPUTLMOD4_X(PTL_CAT(PPUTLUTRAITS_, PTL_UINT(__VA_ARGS__)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLMOD4_X(...) PPUTLMOD4_XX(__VA_ARGS__)

#define PPUTLMOD4_XX(de, in, lg, dv, ml, sq, pw, m2, m4, m8, m16, m32, m64, ...) m4

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [numeric.mod8]
/// --------------
/// O(1) uint modulo by 8.
///
/// PTL_MOD8(113)  // 1
/// PTL_MOD8(1023) // 7
#define PTL_MOD8(/* n: uint */...) /* -> uint{n % 8} */ \
  PPUTLMOD8_X(PTL_CAT(PPUTLUTRAITS_, PTL_UINT(__VA_ARGS__)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLMOD8_X(...) PPUTLMOD8_XX(__VA_ARGS__)

#define PPUTLMOD8_XX(de, in, lg, dv, ml, sq, pw, m2, m4, m8, m16, m32, m64, ...) m8

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [numeric.mod16]
/// ---------------
/// O(1) uint modulo by 16.
///
/// PTL_MOD16(60)   // 12
/// PTL_MOD16(1023) // 15
#define PTL_MOD16(/* n: uint */...) /* -> uint{n % 16} */ \
  PPUTLMOD16_X(PTL_CAT(PPUTLUTRAITS_, PTL_UINT(__VA_ARGS__)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLMOD16_X(...) PPUTLMOD16_XX(__VA_ARGS__)

#define PPUTLMOD16_XX(de, in, lg, dv, ml, sq, pw, m2, m4, m8, m16, m32, m64, ...) m16

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [numeric.mod32]
/// ---------------
/// O(1) uint modulo by 32.
///
/// PTL_MOD32(60)   // 28
/// PTL_MOD32(1023) // 31
#define PTL_MOD32(/* n: uint */...) /* -> uint{n % 32} */ \
  PPUTLMOD32_X(PTL_CAT(PPUTLUTRAITS_, PTL_UINT(__VA_ARGS__)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLMOD32_X(...) PPUTLMOD32_XX(__VA_ARGS__)

#define PPUTLMOD32_XX(de, in, lg, dv, ml, sq, pw, m2, m4, m8, m16, m32, m64, ...) m32

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [numeric.mod64]
/// ---------------
/// O(1) uint modulo by 64.
///
/// PTL_MOD64(15)   // 15
/// PTL_MOD64(1023) // 63
#define PTL_MOD64(/* n: uint */...) /* -> uint{n % 64} */ \
  PPUTLMOD64_X(PTL_CAT(PPUTLUTRAITS_, PTL_UINT(__VA_ARGS__)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLMOD64_X(...) PPUTLMOD64_XX(__VA_ARGS__)

#define PPUTLMOD64_XX(de, in, lg, dv, ml, sq, pw, m2, m4, m8, m16, m32, m64, ...) m64

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [numeric.factor]
/// ----------------
/// O(1) uint prime factorization.
///
/// PTL_FACTOR(341)  // 11, 31
/// PTL_FACTOR(1023) // 3, 11, 31
#define PTL_FACTOR(/* n: uint */...) /* -> ...prime_factors(n) */ \
  PPUTLFACTOR_X(PTL_CAT(PPUTLUTRAITS_, PTL_UINT(__VA_ARGS__)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLFACTOR_X(...) PPUTLFACTOR_XX(__VA_ARGS__)

#define PPUTLFACTOR_XX(de, in, lg, dv, ml, sq, pw, m2, m4, m8, m16, m32, m64, ...) __VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [meta.id]
/// ---------
/// identity function. performs one expansion.
///
/// PTL_ID()        // <nothing>
/// PTL_ID(foo)     // foo
/// PTL_ID(a, b, c) // a, b, c
#define PTL_ID(/* v: any... */...) /* -> ...v */ __VA_ARGS__

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
#define PTL_XCT /* -> xct */ PPUTLXCT_A PTL_LP() /**/, PTL_RP()

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLXCT_B(...) PPUTLXCT_A PTL_LP() __VA_ARGS__, PTL_RP()
#define PPUTLXCT_A(...) PPUTLXCT_B PTL_LP() __VA_ARGS__, PTL_RP()

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [meta.xct_size]
/// ---------------
/// measures an xct expr to determine the number of expansions it experienced.
/// ignores the expansion required to read the result.
///
/// fails if xct is not a valid xct expression.
/// PTL_SIZE will fail if the xct expression is too large.
///
/// PTL_XCT_SIZE(PTL_XCT)                            // 0
/// PTL_XCT_SIZE(PTL_ESC(PTL_XCT))                   // 1
/// PTL_XCT_SIZE(PTL_ESC(PTL_ESC(PTL_XCT)))          // 2
/// PTL_XCT_SIZE(PTL_ESC(PTL_ESC(PTL_ESC(PTL_XCT)))) // 3
#define PTL_XCT_SIZE(/* xct */...) /* -> uint */                                    \
  PTL_CAT(PPUTLXCT_SIZE_, PTL_IS_NONE(PTL_CAT(PPUTLXCT_SIZE_DETECT_, __VA_ARGS__))) \
  (PTL_ISTR([PTL_XCT_SIZE] invalid xct expr : __VA_ARGS__), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLXCT_SIZE_1(err, ...)         PPUTLXCT_SIZE_##__VA_ARGS__
#define PPUTLXCT_SIZE_0(err, ...)         PTL_FAIL(err)
#define PPUTLXCT_SIZE_PPUTLXCT_B(__, ...) PPUTLXCT_SIZE_RES(__VA_ARGS__ _)
#define PPUTLXCT_SIZE_PPUTLXCT_A(__, ...) PPUTLXCT_SIZE_RES(__VA_ARGS__ _)
#define PPUTLXCT_SIZE_RES(_, ...)         PTL_SIZE(__VA_ARGS__)
#define PPUTLXCT_SIZE_DETECT_PPUTLXCT_B(...)
#define PPUTLXCT_SIZE_DETECT_PPUTLXCT_A(...)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [meta.ropen]
/// ------------
/// constructs an in-place recursive expression.
/// the result must be expanded once to execute.
///
/// must be followed by an rclose expression with
/// the same value of n.
///
/// ropen repeats the function and open parens;
/// rclose repeats the close parens.
///
/// it is necessary to split the syntax of this
/// operation into two separate function calls, as
/// neither call can expand the target args without
/// risking expansion termination. this structure
/// allows recursive operations to process data
/// obtained from other recursive operations.
///
/// example:
///   madd(a, b) = add(a, b), b
///   mul(a, b)  = first(id(ropen(a, madd) 0, b rclose(a)))
///   mul(2, 4) -> first(id(madd LP madd LP 0, 4 RP RP))
///             -> first(madd(madd(0, 4)))
///             -> first(madd(4, 4))
///             -> first(8, 4)
///             -> 8
///
/// neither f nor the final expansion function may be
/// invoked anywhere within the recursive operation
/// (and both functions must be distinct).
///
/// this operation can be used to perform an arbitrary
/// number of expansions by using two identity functions.
/// this is necessary to implement mutual recursion.
#define PTL_ROPEN(/* n: uint, f: <fn> */...) /* -> 'f lp'{n} */ PPUTLRO_X(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLRO_X(n, f) PTL_CAT(PPUTLRO_X_, PTL_MOD4(n))(n, f)
#define PPUTLRO_X_3(n, f) \
  f PTL_LP() f PTL_LP() f PTL_LP() PTL_CAT(PPUTLRO_, PTL_DIV2(PTL_DIV2(n)))(f)
#define PPUTLRO_X_2(n, f) f PTL_LP() f PTL_LP() PTL_CAT(PPUTLRO_, PTL_DIV2(PTL_DIV2(n)))(f)
#define PPUTLRO_X_1(n, f) f PTL_LP() PTL_CAT(PPUTLRO_, PTL_DIV2(PTL_DIV2(PTL_DEC(n))))(f)
#define PPUTLRO_X_0(n, f) PTL_CAT(PPUTLRO_, PTL_DIV2(PTL_DIV2(n)))(f)
#define PPUTLRO_255(f)    PPUTLRO_63(f) PPUTLRO_64(f) PPUTLRO_64(f) PPUTLRO_64(f)
#define PPUTLRO_254(f)    PPUTLRO_63(f) PPUTLRO_63(f) PPUTLRO_64(f) PPUTLRO_64(f)
#define PPUTLRO_253(f)    PPUTLRO_63(f) PPUTLRO_63(f) PPUTLRO_63(f) PPUTLRO_64(f)
#define PPUTLRO_252(f)    PPUTLRO_63(f) PPUTLRO_63(f) PPUTLRO_63(f) PPUTLRO_63(f)
#define PPUTLRO_251(f)    PPUTLRO_62(f) PPUTLRO_63(f) PPUTLRO_63(f) PPUTLRO_63(f)
#define PPUTLRO_250(f)    PPUTLRO_62(f) PPUTLRO_62(f) PPUTLRO_63(f) PPUTLRO_63(f)
#define PPUTLRO_249(f)    PPUTLRO_62(f) PPUTLRO_62(f) PPUTLRO_62(f) PPUTLRO_63(f)
#define PPUTLRO_248(f)    PPUTLRO_62(f) PPUTLRO_62(f) PPUTLRO_62(f) PPUTLRO_62(f)
#define PPUTLRO_247(f)    PPUTLRO_61(f) PPUTLRO_62(f) PPUTLRO_62(f) PPUTLRO_62(f)
#define PPUTLRO_246(f)    PPUTLRO_61(f) PPUTLRO_61(f) PPUTLRO_62(f) PPUTLRO_62(f)
#define PPUTLRO_245(f)    PPUTLRO_61(f) PPUTLRO_61(f) PPUTLRO_61(f) PPUTLRO_62(f)
#define PPUTLRO_244(f)    PPUTLRO_61(f) PPUTLRO_61(f) PPUTLRO_61(f) PPUTLRO_61(f)
#define PPUTLRO_243(f)    PPUTLRO_60(f) PPUTLRO_61(f) PPUTLRO_61(f) PPUTLRO_61(f)
#define PPUTLRO_242(f)    PPUTLRO_60(f) PPUTLRO_60(f) PPUTLRO_61(f) PPUTLRO_61(f)
#define PPUTLRO_241(f)    PPUTLRO_60(f) PPUTLRO_60(f) PPUTLRO_60(f) PPUTLRO_61(f)
#define PPUTLRO_240(f)    PPUTLRO_60(f) PPUTLRO_60(f) PPUTLRO_60(f) PPUTLRO_60(f)
#define PPUTLRO_239(f)    PPUTLRO_59(f) PPUTLRO_60(f) PPUTLRO_60(f) PPUTLRO_60(f)
#define PPUTLRO_238(f)    PPUTLRO_59(f) PPUTLRO_59(f) PPUTLRO_60(f) PPUTLRO_60(f)
#define PPUTLRO_237(f)    PPUTLRO_59(f) PPUTLRO_59(f) PPUTLRO_59(f) PPUTLRO_60(f)
#define PPUTLRO_236(f)    PPUTLRO_59(f) PPUTLRO_59(f) PPUTLRO_59(f) PPUTLRO_59(f)
#define PPUTLRO_235(f)    PPUTLRO_58(f) PPUTLRO_59(f) PPUTLRO_59(f) PPUTLRO_59(f)
#define PPUTLRO_234(f)    PPUTLRO_58(f) PPUTLRO_58(f) PPUTLRO_59(f) PPUTLRO_59(f)
#define PPUTLRO_233(f)    PPUTLRO_58(f) PPUTLRO_58(f) PPUTLRO_58(f) PPUTLRO_59(f)
#define PPUTLRO_232(f)    PPUTLRO_58(f) PPUTLRO_58(f) PPUTLRO_58(f) PPUTLRO_58(f)
#define PPUTLRO_231(f)    PPUTLRO_57(f) PPUTLRO_58(f) PPUTLRO_58(f) PPUTLRO_58(f)
#define PPUTLRO_230(f)    PPUTLRO_57(f) PPUTLRO_57(f) PPUTLRO_58(f) PPUTLRO_58(f)
#define PPUTLRO_229(f)    PPUTLRO_57(f) PPUTLRO_57(f) PPUTLRO_57(f) PPUTLRO_58(f)
#define PPUTLRO_228(f)    PPUTLRO_57(f) PPUTLRO_57(f) PPUTLRO_57(f) PPUTLRO_57(f)
#define PPUTLRO_227(f)    PPUTLRO_56(f) PPUTLRO_57(f) PPUTLRO_57(f) PPUTLRO_57(f)
#define PPUTLRO_226(f)    PPUTLRO_56(f) PPUTLRO_56(f) PPUTLRO_57(f) PPUTLRO_57(f)
#define PPUTLRO_225(f)    PPUTLRO_56(f) PPUTLRO_56(f) PPUTLRO_56(f) PPUTLRO_57(f)
#define PPUTLRO_224(f)    PPUTLRO_56(f) PPUTLRO_56(f) PPUTLRO_56(f) PPUTLRO_56(f)
#define PPUTLRO_223(f)    PPUTLRO_55(f) PPUTLRO_56(f) PPUTLRO_56(f) PPUTLRO_56(f)
#define PPUTLRO_222(f)    PPUTLRO_55(f) PPUTLRO_55(f) PPUTLRO_56(f) PPUTLRO_56(f)
#define PPUTLRO_221(f)    PPUTLRO_55(f) PPUTLRO_55(f) PPUTLRO_55(f) PPUTLRO_56(f)
#define PPUTLRO_220(f)    PPUTLRO_55(f) PPUTLRO_55(f) PPUTLRO_55(f) PPUTLRO_55(f)
#define PPUTLRO_219(f)    PPUTLRO_54(f) PPUTLRO_55(f) PPUTLRO_55(f) PPUTLRO_55(f)
#define PPUTLRO_218(f)    PPUTLRO_54(f) PPUTLRO_54(f) PPUTLRO_55(f) PPUTLRO_55(f)
#define PPUTLRO_217(f)    PPUTLRO_54(f) PPUTLRO_54(f) PPUTLRO_54(f) PPUTLRO_55(f)
#define PPUTLRO_216(f)    PPUTLRO_54(f) PPUTLRO_54(f) PPUTLRO_54(f) PPUTLRO_54(f)
#define PPUTLRO_215(f)    PPUTLRO_53(f) PPUTLRO_54(f) PPUTLRO_54(f) PPUTLRO_54(f)
#define PPUTLRO_214(f)    PPUTLRO_53(f) PPUTLRO_53(f) PPUTLRO_54(f) PPUTLRO_54(f)
#define PPUTLRO_213(f)    PPUTLRO_53(f) PPUTLRO_53(f) PPUTLRO_53(f) PPUTLRO_54(f)
#define PPUTLRO_212(f)    PPUTLRO_53(f) PPUTLRO_53(f) PPUTLRO_53(f) PPUTLRO_53(f)
#define PPUTLRO_211(f)    PPUTLRO_52(f) PPUTLRO_53(f) PPUTLRO_53(f) PPUTLRO_53(f)
#define PPUTLRO_210(f)    PPUTLRO_52(f) PPUTLRO_52(f) PPUTLRO_53(f) PPUTLRO_53(f)
#define PPUTLRO_209(f)    PPUTLRO_52(f) PPUTLRO_52(f) PPUTLRO_52(f) PPUTLRO_53(f)
#define PPUTLRO_208(f)    PPUTLRO_52(f) PPUTLRO_52(f) PPUTLRO_52(f) PPUTLRO_52(f)
#define PPUTLRO_207(f)    PPUTLRO_51(f) PPUTLRO_52(f) PPUTLRO_52(f) PPUTLRO_52(f)
#define PPUTLRO_206(f)    PPUTLRO_51(f) PPUTLRO_51(f) PPUTLRO_52(f) PPUTLRO_52(f)
#define PPUTLRO_205(f)    PPUTLRO_51(f) PPUTLRO_51(f) PPUTLRO_51(f) PPUTLRO_52(f)
#define PPUTLRO_204(f)    PPUTLRO_51(f) PPUTLRO_51(f) PPUTLRO_51(f) PPUTLRO_51(f)
#define PPUTLRO_203(f)    PPUTLRO_50(f) PPUTLRO_51(f) PPUTLRO_51(f) PPUTLRO_51(f)
#define PPUTLRO_202(f)    PPUTLRO_50(f) PPUTLRO_50(f) PPUTLRO_51(f) PPUTLRO_51(f)
#define PPUTLRO_201(f)    PPUTLRO_50(f) PPUTLRO_50(f) PPUTLRO_50(f) PPUTLRO_51(f)
#define PPUTLRO_200(f)    PPUTLRO_50(f) PPUTLRO_50(f) PPUTLRO_50(f) PPUTLRO_50(f)
#define PPUTLRO_199(f)    PPUTLRO_49(f) PPUTLRO_50(f) PPUTLRO_50(f) PPUTLRO_50(f)
#define PPUTLRO_198(f)    PPUTLRO_49(f) PPUTLRO_49(f) PPUTLRO_50(f) PPUTLRO_50(f)
#define PPUTLRO_197(f)    PPUTLRO_49(f) PPUTLRO_49(f) PPUTLRO_49(f) PPUTLRO_50(f)
#define PPUTLRO_196(f)    PPUTLRO_49(f) PPUTLRO_49(f) PPUTLRO_49(f) PPUTLRO_49(f)
#define PPUTLRO_195(f)    PPUTLRO_48(f) PPUTLRO_49(f) PPUTLRO_49(f) PPUTLRO_49(f)
#define PPUTLRO_194(f)    PPUTLRO_48(f) PPUTLRO_48(f) PPUTLRO_49(f) PPUTLRO_49(f)
#define PPUTLRO_193(f)    PPUTLRO_48(f) PPUTLRO_48(f) PPUTLRO_48(f) PPUTLRO_49(f)
#define PPUTLRO_192(f)    PPUTLRO_48(f) PPUTLRO_48(f) PPUTLRO_48(f) PPUTLRO_48(f)
#define PPUTLRO_191(f)    PPUTLRO_47(f) PPUTLRO_48(f) PPUTLRO_48(f) PPUTLRO_48(f)
#define PPUTLRO_190(f)    PPUTLRO_47(f) PPUTLRO_47(f) PPUTLRO_48(f) PPUTLRO_48(f)
#define PPUTLRO_189(f)    PPUTLRO_47(f) PPUTLRO_47(f) PPUTLRO_47(f) PPUTLRO_48(f)
#define PPUTLRO_188(f)    PPUTLRO_47(f) PPUTLRO_47(f) PPUTLRO_47(f) PPUTLRO_47(f)
#define PPUTLRO_187(f)    PPUTLRO_46(f) PPUTLRO_47(f) PPUTLRO_47(f) PPUTLRO_47(f)
#define PPUTLRO_186(f)    PPUTLRO_46(f) PPUTLRO_46(f) PPUTLRO_47(f) PPUTLRO_47(f)
#define PPUTLRO_185(f)    PPUTLRO_46(f) PPUTLRO_46(f) PPUTLRO_46(f) PPUTLRO_47(f)
#define PPUTLRO_184(f)    PPUTLRO_46(f) PPUTLRO_46(f) PPUTLRO_46(f) PPUTLRO_46(f)
#define PPUTLRO_183(f)    PPUTLRO_45(f) PPUTLRO_46(f) PPUTLRO_46(f) PPUTLRO_46(f)
#define PPUTLRO_182(f)    PPUTLRO_45(f) PPUTLRO_45(f) PPUTLRO_46(f) PPUTLRO_46(f)
#define PPUTLRO_181(f)    PPUTLRO_45(f) PPUTLRO_45(f) PPUTLRO_45(f) PPUTLRO_46(f)
#define PPUTLRO_180(f)    PPUTLRO_45(f) PPUTLRO_45(f) PPUTLRO_45(f) PPUTLRO_45(f)
#define PPUTLRO_179(f)    PPUTLRO_44(f) PPUTLRO_45(f) PPUTLRO_45(f) PPUTLRO_45(f)
#define PPUTLRO_178(f)    PPUTLRO_44(f) PPUTLRO_44(f) PPUTLRO_45(f) PPUTLRO_45(f)
#define PPUTLRO_177(f)    PPUTLRO_44(f) PPUTLRO_44(f) PPUTLRO_44(f) PPUTLRO_45(f)
#define PPUTLRO_176(f)    PPUTLRO_44(f) PPUTLRO_44(f) PPUTLRO_44(f) PPUTLRO_44(f)
#define PPUTLRO_175(f)    PPUTLRO_43(f) PPUTLRO_44(f) PPUTLRO_44(f) PPUTLRO_44(f)
#define PPUTLRO_174(f)    PPUTLRO_43(f) PPUTLRO_43(f) PPUTLRO_44(f) PPUTLRO_44(f)
#define PPUTLRO_173(f)    PPUTLRO_43(f) PPUTLRO_43(f) PPUTLRO_43(f) PPUTLRO_44(f)
#define PPUTLRO_172(f)    PPUTLRO_43(f) PPUTLRO_43(f) PPUTLRO_43(f) PPUTLRO_43(f)
#define PPUTLRO_171(f)    PPUTLRO_42(f) PPUTLRO_43(f) PPUTLRO_43(f) PPUTLRO_43(f)
#define PPUTLRO_170(f)    PPUTLRO_42(f) PPUTLRO_42(f) PPUTLRO_43(f) PPUTLRO_43(f)
#define PPUTLRO_169(f)    PPUTLRO_42(f) PPUTLRO_42(f) PPUTLRO_42(f) PPUTLRO_43(f)
#define PPUTLRO_168(f)    PPUTLRO_42(f) PPUTLRO_42(f) PPUTLRO_42(f) PPUTLRO_42(f)
#define PPUTLRO_167(f)    PPUTLRO_41(f) PPUTLRO_42(f) PPUTLRO_42(f) PPUTLRO_42(f)
#define PPUTLRO_166(f)    PPUTLRO_41(f) PPUTLRO_41(f) PPUTLRO_42(f) PPUTLRO_42(f)
#define PPUTLRO_165(f)    PPUTLRO_41(f) PPUTLRO_41(f) PPUTLRO_41(f) PPUTLRO_42(f)
#define PPUTLRO_164(f)    PPUTLRO_41(f) PPUTLRO_41(f) PPUTLRO_41(f) PPUTLRO_41(f)
#define PPUTLRO_163(f)    PPUTLRO_40(f) PPUTLRO_41(f) PPUTLRO_41(f) PPUTLRO_41(f)
#define PPUTLRO_162(f)    PPUTLRO_40(f) PPUTLRO_40(f) PPUTLRO_41(f) PPUTLRO_41(f)
#define PPUTLRO_161(f)    PPUTLRO_40(f) PPUTLRO_40(f) PPUTLRO_40(f) PPUTLRO_41(f)
#define PPUTLRO_160(f)    PPUTLRO_40(f) PPUTLRO_40(f) PPUTLRO_40(f) PPUTLRO_40(f)
#define PPUTLRO_159(f)    PPUTLRO_39(f) PPUTLRO_40(f) PPUTLRO_40(f) PPUTLRO_40(f)
#define PPUTLRO_158(f)    PPUTLRO_39(f) PPUTLRO_39(f) PPUTLRO_40(f) PPUTLRO_40(f)
#define PPUTLRO_157(f)    PPUTLRO_39(f) PPUTLRO_39(f) PPUTLRO_39(f) PPUTLRO_40(f)
#define PPUTLRO_156(f)    PPUTLRO_39(f) PPUTLRO_39(f) PPUTLRO_39(f) PPUTLRO_39(f)
#define PPUTLRO_155(f)    PPUTLRO_38(f) PPUTLRO_39(f) PPUTLRO_39(f) PPUTLRO_39(f)
#define PPUTLRO_154(f)    PPUTLRO_38(f) PPUTLRO_38(f) PPUTLRO_39(f) PPUTLRO_39(f)
#define PPUTLRO_153(f)    PPUTLRO_38(f) PPUTLRO_38(f) PPUTLRO_38(f) PPUTLRO_39(f)
#define PPUTLRO_152(f)    PPUTLRO_38(f) PPUTLRO_38(f) PPUTLRO_38(f) PPUTLRO_38(f)
#define PPUTLRO_151(f)    PPUTLRO_37(f) PPUTLRO_38(f) PPUTLRO_38(f) PPUTLRO_38(f)
#define PPUTLRO_150(f)    PPUTLRO_37(f) PPUTLRO_37(f) PPUTLRO_38(f) PPUTLRO_38(f)
#define PPUTLRO_149(f)    PPUTLRO_37(f) PPUTLRO_37(f) PPUTLRO_37(f) PPUTLRO_38(f)
#define PPUTLRO_148(f)    PPUTLRO_37(f) PPUTLRO_37(f) PPUTLRO_37(f) PPUTLRO_37(f)
#define PPUTLRO_147(f)    PPUTLRO_36(f) PPUTLRO_37(f) PPUTLRO_37(f) PPUTLRO_37(f)
#define PPUTLRO_146(f)    PPUTLRO_36(f) PPUTLRO_36(f) PPUTLRO_37(f) PPUTLRO_37(f)
#define PPUTLRO_145(f)    PPUTLRO_36(f) PPUTLRO_36(f) PPUTLRO_36(f) PPUTLRO_37(f)
#define PPUTLRO_144(f)    PPUTLRO_36(f) PPUTLRO_36(f) PPUTLRO_36(f) PPUTLRO_36(f)
#define PPUTLRO_143(f)    PPUTLRO_35(f) PPUTLRO_36(f) PPUTLRO_36(f) PPUTLRO_36(f)
#define PPUTLRO_142(f)    PPUTLRO_35(f) PPUTLRO_35(f) PPUTLRO_36(f) PPUTLRO_36(f)
#define PPUTLRO_141(f)    PPUTLRO_35(f) PPUTLRO_35(f) PPUTLRO_35(f) PPUTLRO_36(f)
#define PPUTLRO_140(f)    PPUTLRO_35(f) PPUTLRO_35(f) PPUTLRO_35(f) PPUTLRO_35(f)
#define PPUTLRO_139(f)    PPUTLRO_34(f) PPUTLRO_35(f) PPUTLRO_35(f) PPUTLRO_35(f)
#define PPUTLRO_138(f)    PPUTLRO_34(f) PPUTLRO_34(f) PPUTLRO_35(f) PPUTLRO_35(f)
#define PPUTLRO_137(f)    PPUTLRO_34(f) PPUTLRO_34(f) PPUTLRO_34(f) PPUTLRO_35(f)
#define PPUTLRO_136(f)    PPUTLRO_34(f) PPUTLRO_34(f) PPUTLRO_34(f) PPUTLRO_34(f)
#define PPUTLRO_135(f)    PPUTLRO_33(f) PPUTLRO_34(f) PPUTLRO_34(f) PPUTLRO_34(f)
#define PPUTLRO_134(f)    PPUTLRO_33(f) PPUTLRO_33(f) PPUTLRO_34(f) PPUTLRO_34(f)
#define PPUTLRO_133(f)    PPUTLRO_33(f) PPUTLRO_33(f) PPUTLRO_33(f) PPUTLRO_34(f)
#define PPUTLRO_132(f)    PPUTLRO_33(f) PPUTLRO_33(f) PPUTLRO_33(f) PPUTLRO_33(f)
#define PPUTLRO_131(f)    PPUTLRO_32(f) PPUTLRO_33(f) PPUTLRO_33(f) PPUTLRO_33(f)
#define PPUTLRO_130(f)    PPUTLRO_32(f) PPUTLRO_32(f) PPUTLRO_33(f) PPUTLRO_33(f)
#define PPUTLRO_129(f)    PPUTLRO_32(f) PPUTLRO_32(f) PPUTLRO_32(f) PPUTLRO_33(f)
#define PPUTLRO_128(f)    PPUTLRO_32(f) PPUTLRO_32(f) PPUTLRO_32(f) PPUTLRO_32(f)
#define PPUTLRO_127(f)    PPUTLRO_31(f) PPUTLRO_32(f) PPUTLRO_32(f) PPUTLRO_32(f)
#define PPUTLRO_126(f)    PPUTLRO_31(f) PPUTLRO_31(f) PPUTLRO_32(f) PPUTLRO_32(f)
#define PPUTLRO_125(f)    PPUTLRO_31(f) PPUTLRO_31(f) PPUTLRO_31(f) PPUTLRO_32(f)
#define PPUTLRO_124(f)    PPUTLRO_31(f) PPUTLRO_31(f) PPUTLRO_31(f) PPUTLRO_31(f)
#define PPUTLRO_123(f)    PPUTLRO_30(f) PPUTLRO_31(f) PPUTLRO_31(f) PPUTLRO_31(f)
#define PPUTLRO_122(f)    PPUTLRO_30(f) PPUTLRO_30(f) PPUTLRO_31(f) PPUTLRO_31(f)
#define PPUTLRO_121(f)    PPUTLRO_30(f) PPUTLRO_30(f) PPUTLRO_30(f) PPUTLRO_31(f)
#define PPUTLRO_120(f)    PPUTLRO_30(f) PPUTLRO_30(f) PPUTLRO_30(f) PPUTLRO_30(f)
#define PPUTLRO_119(f)    PPUTLRO_29(f) PPUTLRO_30(f) PPUTLRO_30(f) PPUTLRO_30(f)
#define PPUTLRO_118(f)    PPUTLRO_29(f) PPUTLRO_29(f) PPUTLRO_30(f) PPUTLRO_30(f)
#define PPUTLRO_117(f)    PPUTLRO_29(f) PPUTLRO_29(f) PPUTLRO_29(f) PPUTLRO_30(f)
#define PPUTLRO_116(f)    PPUTLRO_29(f) PPUTLRO_29(f) PPUTLRO_29(f) PPUTLRO_29(f)
#define PPUTLRO_115(f)    PPUTLRO_28(f) PPUTLRO_29(f) PPUTLRO_29(f) PPUTLRO_29(f)
#define PPUTLRO_114(f)    PPUTLRO_28(f) PPUTLRO_28(f) PPUTLRO_29(f) PPUTLRO_29(f)
#define PPUTLRO_113(f)    PPUTLRO_28(f) PPUTLRO_28(f) PPUTLRO_28(f) PPUTLRO_29(f)
#define PPUTLRO_112(f)    PPUTLRO_28(f) PPUTLRO_28(f) PPUTLRO_28(f) PPUTLRO_28(f)
#define PPUTLRO_111(f)    PPUTLRO_27(f) PPUTLRO_28(f) PPUTLRO_28(f) PPUTLRO_28(f)
#define PPUTLRO_110(f)    PPUTLRO_27(f) PPUTLRO_27(f) PPUTLRO_28(f) PPUTLRO_28(f)
#define PPUTLRO_109(f)    PPUTLRO_27(f) PPUTLRO_27(f) PPUTLRO_27(f) PPUTLRO_28(f)
#define PPUTLRO_108(f)    PPUTLRO_27(f) PPUTLRO_27(f) PPUTLRO_27(f) PPUTLRO_27(f)
#define PPUTLRO_107(f)    PPUTLRO_26(f) PPUTLRO_27(f) PPUTLRO_27(f) PPUTLRO_27(f)
#define PPUTLRO_106(f)    PPUTLRO_26(f) PPUTLRO_26(f) PPUTLRO_27(f) PPUTLRO_27(f)
#define PPUTLRO_105(f)    PPUTLRO_26(f) PPUTLRO_26(f) PPUTLRO_26(f) PPUTLRO_27(f)
#define PPUTLRO_104(f)    PPUTLRO_26(f) PPUTLRO_26(f) PPUTLRO_26(f) PPUTLRO_26(f)
#define PPUTLRO_103(f)    PPUTLRO_25(f) PPUTLRO_26(f) PPUTLRO_26(f) PPUTLRO_26(f)
#define PPUTLRO_102(f)    PPUTLRO_25(f) PPUTLRO_25(f) PPUTLRO_26(f) PPUTLRO_26(f)
#define PPUTLRO_101(f)    PPUTLRO_25(f) PPUTLRO_25(f) PPUTLRO_25(f) PPUTLRO_26(f)
#define PPUTLRO_100(f)    PPUTLRO_25(f) PPUTLRO_25(f) PPUTLRO_25(f) PPUTLRO_25(f)
#define PPUTLRO_99(f)     PPUTLRO_24(f) PPUTLRO_25(f) PPUTLRO_25(f) PPUTLRO_25(f)
#define PPUTLRO_98(f)     PPUTLRO_24(f) PPUTLRO_24(f) PPUTLRO_25(f) PPUTLRO_25(f)
#define PPUTLRO_97(f)     PPUTLRO_24(f) PPUTLRO_24(f) PPUTLRO_24(f) PPUTLRO_25(f)
#define PPUTLRO_96(f)     PPUTLRO_24(f) PPUTLRO_24(f) PPUTLRO_24(f) PPUTLRO_24(f)
#define PPUTLRO_95(f)     PPUTLRO_23(f) PPUTLRO_24(f) PPUTLRO_24(f) PPUTLRO_24(f)
#define PPUTLRO_94(f)     PPUTLRO_23(f) PPUTLRO_23(f) PPUTLRO_24(f) PPUTLRO_24(f)
#define PPUTLRO_93(f)     PPUTLRO_23(f) PPUTLRO_23(f) PPUTLRO_23(f) PPUTLRO_24(f)
#define PPUTLRO_92(f)     PPUTLRO_23(f) PPUTLRO_23(f) PPUTLRO_23(f) PPUTLRO_23(f)
#define PPUTLRO_91(f)     PPUTLRO_22(f) PPUTLRO_23(f) PPUTLRO_23(f) PPUTLRO_23(f)
#define PPUTLRO_90(f)     PPUTLRO_22(f) PPUTLRO_22(f) PPUTLRO_23(f) PPUTLRO_23(f)
#define PPUTLRO_89(f)     PPUTLRO_22(f) PPUTLRO_22(f) PPUTLRO_22(f) PPUTLRO_23(f)
#define PPUTLRO_88(f)     PPUTLRO_22(f) PPUTLRO_22(f) PPUTLRO_22(f) PPUTLRO_22(f)
#define PPUTLRO_87(f)     PPUTLRO_21(f) PPUTLRO_22(f) PPUTLRO_22(f) PPUTLRO_22(f)
#define PPUTLRO_86(f)     PPUTLRO_21(f) PPUTLRO_21(f) PPUTLRO_22(f) PPUTLRO_22(f)
#define PPUTLRO_85(f)     PPUTLRO_21(f) PPUTLRO_21(f) PPUTLRO_21(f) PPUTLRO_22(f)
#define PPUTLRO_84(f)     PPUTLRO_21(f) PPUTLRO_21(f) PPUTLRO_21(f) PPUTLRO_21(f)
#define PPUTLRO_83(f)     PPUTLRO_20(f) PPUTLRO_21(f) PPUTLRO_21(f) PPUTLRO_21(f)
#define PPUTLRO_82(f)     PPUTLRO_20(f) PPUTLRO_20(f) PPUTLRO_21(f) PPUTLRO_21(f)
#define PPUTLRO_81(f)     PPUTLRO_20(f) PPUTLRO_20(f) PPUTLRO_20(f) PPUTLRO_21(f)
#define PPUTLRO_80(f)     PPUTLRO_20(f) PPUTLRO_20(f) PPUTLRO_20(f) PPUTLRO_20(f)
#define PPUTLRO_79(f)     PPUTLRO_19(f) PPUTLRO_20(f) PPUTLRO_20(f) PPUTLRO_20(f)
#define PPUTLRO_78(f)     PPUTLRO_19(f) PPUTLRO_19(f) PPUTLRO_20(f) PPUTLRO_20(f)
#define PPUTLRO_77(f)     PPUTLRO_19(f) PPUTLRO_19(f) PPUTLRO_19(f) PPUTLRO_20(f)
#define PPUTLRO_76(f)     PPUTLRO_19(f) PPUTLRO_19(f) PPUTLRO_19(f) PPUTLRO_19(f)
#define PPUTLRO_75(f)     PPUTLRO_18(f) PPUTLRO_19(f) PPUTLRO_19(f) PPUTLRO_19(f)
#define PPUTLRO_74(f)     PPUTLRO_18(f) PPUTLRO_18(f) PPUTLRO_19(f) PPUTLRO_19(f)
#define PPUTLRO_73(f)     PPUTLRO_18(f) PPUTLRO_18(f) PPUTLRO_18(f) PPUTLRO_19(f)
#define PPUTLRO_72(f)     PPUTLRO_18(f) PPUTLRO_18(f) PPUTLRO_18(f) PPUTLRO_18(f)
#define PPUTLRO_71(f)     PPUTLRO_17(f) PPUTLRO_18(f) PPUTLRO_18(f) PPUTLRO_18(f)
#define PPUTLRO_70(f)     PPUTLRO_17(f) PPUTLRO_17(f) PPUTLRO_18(f) PPUTLRO_18(f)
#define PPUTLRO_69(f)     PPUTLRO_17(f) PPUTLRO_17(f) PPUTLRO_17(f) PPUTLRO_18(f)
#define PPUTLRO_68(f)     PPUTLRO_17(f) PPUTLRO_17(f) PPUTLRO_17(f) PPUTLRO_17(f)
#define PPUTLRO_67(f)     PPUTLRO_16(f) PPUTLRO_17(f) PPUTLRO_17(f) PPUTLRO_17(f)
#define PPUTLRO_66(f)     PPUTLRO_16(f) PPUTLRO_16(f) PPUTLRO_17(f) PPUTLRO_17(f)
#define PPUTLRO_65(f)     PPUTLRO_16(f) PPUTLRO_16(f) PPUTLRO_16(f) PPUTLRO_17(f)
#define PPUTLRO_64(f)     PPUTLRO_16(f) PPUTLRO_16(f) PPUTLRO_16(f) PPUTLRO_16(f)
#define PPUTLRO_63(f)     PPUTLRO_15(f) PPUTLRO_16(f) PPUTLRO_16(f) PPUTLRO_16(f)
#define PPUTLRO_62(f)     PPUTLRO_15(f) PPUTLRO_15(f) PPUTLRO_16(f) PPUTLRO_16(f)
#define PPUTLRO_61(f)     PPUTLRO_15(f) PPUTLRO_15(f) PPUTLRO_15(f) PPUTLRO_16(f)
#define PPUTLRO_60(f)     PPUTLRO_15(f) PPUTLRO_15(f) PPUTLRO_15(f) PPUTLRO_15(f)
#define PPUTLRO_59(f)     PPUTLRO_14(f) PPUTLRO_15(f) PPUTLRO_15(f) PPUTLRO_15(f)
#define PPUTLRO_58(f)     PPUTLRO_14(f) PPUTLRO_14(f) PPUTLRO_15(f) PPUTLRO_15(f)
#define PPUTLRO_57(f)     PPUTLRO_14(f) PPUTLRO_14(f) PPUTLRO_14(f) PPUTLRO_15(f)
#define PPUTLRO_56(f)     PPUTLRO_14(f) PPUTLRO_14(f) PPUTLRO_14(f) PPUTLRO_14(f)
#define PPUTLRO_55(f)     PPUTLRO_13(f) PPUTLRO_14(f) PPUTLRO_14(f) PPUTLRO_14(f)
#define PPUTLRO_54(f)     PPUTLRO_13(f) PPUTLRO_13(f) PPUTLRO_14(f) PPUTLRO_14(f)
#define PPUTLRO_53(f)     PPUTLRO_13(f) PPUTLRO_13(f) PPUTLRO_13(f) PPUTLRO_14(f)
#define PPUTLRO_52(f)     PPUTLRO_13(f) PPUTLRO_13(f) PPUTLRO_13(f) PPUTLRO_13(f)
#define PPUTLRO_51(f)     PPUTLRO_12(f) PPUTLRO_13(f) PPUTLRO_13(f) PPUTLRO_13(f)
#define PPUTLRO_50(f)     PPUTLRO_12(f) PPUTLRO_12(f) PPUTLRO_13(f) PPUTLRO_13(f)
#define PPUTLRO_49(f)     PPUTLRO_12(f) PPUTLRO_12(f) PPUTLRO_12(f) PPUTLRO_13(f)
#define PPUTLRO_48(f)     PPUTLRO_12(f) PPUTLRO_12(f) PPUTLRO_12(f) PPUTLRO_12(f)
#define PPUTLRO_47(f)     PPUTLRO_11(f) PPUTLRO_12(f) PPUTLRO_12(f) PPUTLRO_12(f)
#define PPUTLRO_46(f)     PPUTLRO_11(f) PPUTLRO_11(f) PPUTLRO_12(f) PPUTLRO_12(f)
#define PPUTLRO_45(f)     PPUTLRO_11(f) PPUTLRO_11(f) PPUTLRO_11(f) PPUTLRO_12(f)
#define PPUTLRO_44(f)     PPUTLRO_11(f) PPUTLRO_11(f) PPUTLRO_11(f) PPUTLRO_11(f)
#define PPUTLRO_43(f)     PPUTLRO_10(f) PPUTLRO_11(f) PPUTLRO_11(f) PPUTLRO_11(f)
#define PPUTLRO_42(f)     PPUTLRO_10(f) PPUTLRO_10(f) PPUTLRO_11(f) PPUTLRO_11(f)
#define PPUTLRO_41(f)     PPUTLRO_10(f) PPUTLRO_10(f) PPUTLRO_10(f) PPUTLRO_11(f)
#define PPUTLRO_40(f)     PPUTLRO_10(f) PPUTLRO_10(f) PPUTLRO_10(f) PPUTLRO_10(f)
#define PPUTLRO_39(f)     PPUTLRO_9(f) PPUTLRO_10(f) PPUTLRO_10(f) PPUTLRO_10(f)
#define PPUTLRO_38(f)     PPUTLRO_9(f) PPUTLRO_9(f) PPUTLRO_10(f) PPUTLRO_10(f)
#define PPUTLRO_37(f)     PPUTLRO_9(f) PPUTLRO_9(f) PPUTLRO_9(f) PPUTLRO_10(f)
#define PPUTLRO_36(f)     PPUTLRO_9(f) PPUTLRO_9(f) PPUTLRO_9(f) PPUTLRO_9(f)
#define PPUTLRO_35(f)     PPUTLRO_8(f) PPUTLRO_9(f) PPUTLRO_9(f) PPUTLRO_9(f)
#define PPUTLRO_34(f)     PPUTLRO_8(f) PPUTLRO_8(f) PPUTLRO_9(f) PPUTLRO_9(f)
#define PPUTLRO_33(f)     PPUTLRO_8(f) PPUTLRO_8(f) PPUTLRO_8(f) PPUTLRO_9(f)
#define PPUTLRO_32(f)     PPUTLRO_8(f) PPUTLRO_8(f) PPUTLRO_8(f) PPUTLRO_8(f)
#define PPUTLRO_31(f)     PPUTLRO_7(f) PPUTLRO_8(f) PPUTLRO_8(f) PPUTLRO_8(f)
#define PPUTLRO_30(f)     PPUTLRO_7(f) PPUTLRO_7(f) PPUTLRO_8(f) PPUTLRO_8(f)
#define PPUTLRO_29(f)     PPUTLRO_7(f) PPUTLRO_7(f) PPUTLRO_7(f) PPUTLRO_8(f)
#define PPUTLRO_28(f)     PPUTLRO_7(f) PPUTLRO_7(f) PPUTLRO_7(f) PPUTLRO_7(f)
#define PPUTLRO_27(f)     PPUTLRO_6(f) PPUTLRO_7(f) PPUTLRO_7(f) PPUTLRO_7(f)
#define PPUTLRO_26(f)     PPUTLRO_6(f) PPUTLRO_6(f) PPUTLRO_7(f) PPUTLRO_7(f)
#define PPUTLRO_25(f)     PPUTLRO_6(f) PPUTLRO_6(f) PPUTLRO_6(f) PPUTLRO_7(f)
#define PPUTLRO_24(f)     PPUTLRO_6(f) PPUTLRO_6(f) PPUTLRO_6(f) PPUTLRO_6(f)
#define PPUTLRO_23(f)     PPUTLRO_5(f) PPUTLRO_6(f) PPUTLRO_6(f) PPUTLRO_6(f)
#define PPUTLRO_22(f)     PPUTLRO_5(f) PPUTLRO_5(f) PPUTLRO_6(f) PPUTLRO_6(f)
#define PPUTLRO_21(f)     PPUTLRO_5(f) PPUTLRO_5(f) PPUTLRO_5(f) PPUTLRO_6(f)
#define PPUTLRO_20(f)     PPUTLRO_5(f) PPUTLRO_5(f) PPUTLRO_5(f) PPUTLRO_5(f)
#define PPUTLRO_19(f)     PPUTLRO_4(f) PPUTLRO_5(f) PPUTLRO_5(f) PPUTLRO_5(f)
#define PPUTLRO_18(f)     PPUTLRO_4(f) PPUTLRO_4(f) PPUTLRO_5(f) PPUTLRO_5(f)
#define PPUTLRO_17(f)     PPUTLRO_4(f) PPUTLRO_4(f) PPUTLRO_4(f) PPUTLRO_5(f)
#define PPUTLRO_16(f)     PPUTLRO_4(f) PPUTLRO_4(f) PPUTLRO_4(f) PPUTLRO_4(f)
#define PPUTLRO_15(f)     PPUTLRO_3(f) PPUTLRO_4(f) PPUTLRO_4(f) PPUTLRO_4(f)
#define PPUTLRO_14(f)     PPUTLRO_3(f) PPUTLRO_3(f) PPUTLRO_4(f) PPUTLRO_4(f)
#define PPUTLRO_13(f)     PPUTLRO_3(f) PPUTLRO_3(f) PPUTLRO_3(f) PPUTLRO_4(f)
#define PPUTLRO_12(f)     PPUTLRO_3(f) PPUTLRO_3(f) PPUTLRO_3(f) PPUTLRO_3(f)
#define PPUTLRO_11(f)     PPUTLRO_2(f) PPUTLRO_3(f) PPUTLRO_3(f) PPUTLRO_3(f)
#define PPUTLRO_10(f)     PPUTLRO_2(f) PPUTLRO_2(f) PPUTLRO_3(f) PPUTLRO_3(f)
#define PPUTLRO_9(f)      PPUTLRO_2(f) PPUTLRO_2(f) PPUTLRO_2(f) PPUTLRO_3(f)
#define PPUTLRO_8(f)      PPUTLRO_2(f) PPUTLRO_2(f) PPUTLRO_2(f) PPUTLRO_2(f)
#define PPUTLRO_7(f)      PPUTLRO_1(f) PPUTLRO_2(f) PPUTLRO_2(f) PPUTLRO_2(f)
#define PPUTLRO_6(f)      PPUTLRO_1(f) PPUTLRO_1(f) PPUTLRO_2(f) PPUTLRO_2(f)
#define PPUTLRO_5(f)      PPUTLRO_1(f) PPUTLRO_1(f) PPUTLRO_1(f) PPUTLRO_2(f)
#define PPUTLRO_4(f)      PPUTLRO_1(f) PPUTLRO_1(f) PPUTLRO_1(f) PPUTLRO_1(f)
#define PPUTLRO_3(f)      PPUTLRO_1(f) PPUTLRO_1(f) PPUTLRO_1(f)
#define PPUTLRO_2(f)      PPUTLRO_1(f) PPUTLRO_1(f)
#define PPUTLRO_1(f)      f PTL_LP() f PTL_LP() f PTL_LP() f PTL_LP()
#define PPUTLRO_0(f)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [meta.rclose]
/// -------------
/// constructs an in-place recursive expression.
/// the result must be expanded once to execute.
///
/// must be preceeded by an ropen expression with
/// the same value of n.
///
/// ropen repeats the function and open parens;
/// rclose repeats the close parens.
///
/// it is necessary to split the syntax of this
/// operation into two separate function calls, as
/// neither call can expand the target args without
/// risking expansion termination. this structure
/// allows recursive operations to process data
/// obtained from other recursive operations.
///
/// example:
///   madd(a, b) = add(a, b), b
///   mul(a, b)  = first(id(ropen(a, madd) 0, 4 rclose(a)))
///   mul(2, 4) -> first(id(madd LP madd LP 0, 4 RP RP))
///             -> first(madd(madd(0, 4)))
///             -> first(madd(4, 4))
///             -> first(8, 4)
///             -> 8
///
/// neither f nor the final expansion function may be
/// invoked anywhere within the recursive operation
/// (and both functions must be distinct).
///
/// this operation can be used to perform an arbitrary
/// number of expansions by using two identity functions.
/// this is necessary to implement mutual recursion.
///
/// PTL_XCT_SIZE(PTL_ID(PTL_ROPEN(0, PTL_ESC) PTL_XCT PTL_RCLOSE(0)))       // 1
/// PTL_XCT_SIZE(PTL_ID(PTL_ROPEN(1, PTL_ESC) PTL_XCT PTL_RCLOSE(1)))       // 2
/// PTL_XCT_SIZE(PTL_ID(PTL_ROPEN(2, PTL_ESC) PTL_XCT PTL_RCLOSE(2)))       // 3
/// PTL_XCT_SIZE(PTL_ID(PTL_ROPEN(3, PTL_ESC) PTL_XCT PTL_RCLOSE(3)))       // 4
/// PTL_XCT_SIZE(PTL_ID(PTL_ROPEN(1022, PTL_ESC) PTL_XCT PTL_RCLOSE(1022))) // 1023
#define PTL_RCLOSE(/* n: uint */...) /* -> 'rp'{n} */ PPUTLRC_X(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLRC_X(n)   PTL_CAT(PPUTLRC_X_, PTL_MOD4(n))(n)
#define PPUTLRC_X_3(n) PTL_CAT(PPUTLRC_, PTL_DIV2(PTL_DIV2(n))) PTL_RP() PTL_RP() PTL_RP()
#define PPUTLRC_X_2(n) PTL_CAT(PPUTLRC_, PTL_DIV2(PTL_DIV2(n))) PTL_RP() PTL_RP()
#define PPUTLRC_X_1(n) PTL_CAT(PPUTLRC_, PTL_DIV2(PTL_DIV2(n))) PTL_RP()
#define PPUTLRC_X_0(n) PTL_CAT(PPUTLRC_, PTL_DIV2(PTL_DIV2(n)))
#define PPUTLRC_255    PPUTLRC_63 PPUTLRC_64 PPUTLRC_64 PPUTLRC_64
#define PPUTLRC_254    PPUTLRC_63 PPUTLRC_63 PPUTLRC_64 PPUTLRC_64
#define PPUTLRC_253    PPUTLRC_63 PPUTLRC_63 PPUTLRC_63 PPUTLRC_64
#define PPUTLRC_252    PPUTLRC_63 PPUTLRC_63 PPUTLRC_63 PPUTLRC_63
#define PPUTLRC_251    PPUTLRC_62 PPUTLRC_63 PPUTLRC_63 PPUTLRC_63
#define PPUTLRC_250    PPUTLRC_62 PPUTLRC_62 PPUTLRC_63 PPUTLRC_63
#define PPUTLRC_249    PPUTLRC_62 PPUTLRC_62 PPUTLRC_62 PPUTLRC_63
#define PPUTLRC_248    PPUTLRC_62 PPUTLRC_62 PPUTLRC_62 PPUTLRC_62
#define PPUTLRC_247    PPUTLRC_61 PPUTLRC_62 PPUTLRC_62 PPUTLRC_62
#define PPUTLRC_246    PPUTLRC_61 PPUTLRC_61 PPUTLRC_62 PPUTLRC_62
#define PPUTLRC_245    PPUTLRC_61 PPUTLRC_61 PPUTLRC_61 PPUTLRC_62
#define PPUTLRC_244    PPUTLRC_61 PPUTLRC_61 PPUTLRC_61 PPUTLRC_61
#define PPUTLRC_243    PPUTLRC_60 PPUTLRC_61 PPUTLRC_61 PPUTLRC_61
#define PPUTLRC_242    PPUTLRC_60 PPUTLRC_60 PPUTLRC_61 PPUTLRC_61
#define PPUTLRC_241    PPUTLRC_60 PPUTLRC_60 PPUTLRC_60 PPUTLRC_61
#define PPUTLRC_240    PPUTLRC_60 PPUTLRC_60 PPUTLRC_60 PPUTLRC_60
#define PPUTLRC_239    PPUTLRC_59 PPUTLRC_60 PPUTLRC_60 PPUTLRC_60
#define PPUTLRC_238    PPUTLRC_59 PPUTLRC_59 PPUTLRC_60 PPUTLRC_60
#define PPUTLRC_237    PPUTLRC_59 PPUTLRC_59 PPUTLRC_59 PPUTLRC_60
#define PPUTLRC_236    PPUTLRC_59 PPUTLRC_59 PPUTLRC_59 PPUTLRC_59
#define PPUTLRC_235    PPUTLRC_58 PPUTLRC_59 PPUTLRC_59 PPUTLRC_59
#define PPUTLRC_234    PPUTLRC_58 PPUTLRC_58 PPUTLRC_59 PPUTLRC_59
#define PPUTLRC_233    PPUTLRC_58 PPUTLRC_58 PPUTLRC_58 PPUTLRC_59
#define PPUTLRC_232    PPUTLRC_58 PPUTLRC_58 PPUTLRC_58 PPUTLRC_58
#define PPUTLRC_231    PPUTLRC_57 PPUTLRC_58 PPUTLRC_58 PPUTLRC_58
#define PPUTLRC_230    PPUTLRC_57 PPUTLRC_57 PPUTLRC_58 PPUTLRC_58
#define PPUTLRC_229    PPUTLRC_57 PPUTLRC_57 PPUTLRC_57 PPUTLRC_58
#define PPUTLRC_228    PPUTLRC_57 PPUTLRC_57 PPUTLRC_57 PPUTLRC_57
#define PPUTLRC_227    PPUTLRC_56 PPUTLRC_57 PPUTLRC_57 PPUTLRC_57
#define PPUTLRC_226    PPUTLRC_56 PPUTLRC_56 PPUTLRC_57 PPUTLRC_57
#define PPUTLRC_225    PPUTLRC_56 PPUTLRC_56 PPUTLRC_56 PPUTLRC_57
#define PPUTLRC_224    PPUTLRC_56 PPUTLRC_56 PPUTLRC_56 PPUTLRC_56
#define PPUTLRC_223    PPUTLRC_55 PPUTLRC_56 PPUTLRC_56 PPUTLRC_56
#define PPUTLRC_222    PPUTLRC_55 PPUTLRC_55 PPUTLRC_56 PPUTLRC_56
#define PPUTLRC_221    PPUTLRC_55 PPUTLRC_55 PPUTLRC_55 PPUTLRC_56
#define PPUTLRC_220    PPUTLRC_55 PPUTLRC_55 PPUTLRC_55 PPUTLRC_55
#define PPUTLRC_219    PPUTLRC_54 PPUTLRC_55 PPUTLRC_55 PPUTLRC_55
#define PPUTLRC_218    PPUTLRC_54 PPUTLRC_54 PPUTLRC_55 PPUTLRC_55
#define PPUTLRC_217    PPUTLRC_54 PPUTLRC_54 PPUTLRC_54 PPUTLRC_55
#define PPUTLRC_216    PPUTLRC_54 PPUTLRC_54 PPUTLRC_54 PPUTLRC_54
#define PPUTLRC_215    PPUTLRC_53 PPUTLRC_54 PPUTLRC_54 PPUTLRC_54
#define PPUTLRC_214    PPUTLRC_53 PPUTLRC_53 PPUTLRC_54 PPUTLRC_54
#define PPUTLRC_213    PPUTLRC_53 PPUTLRC_53 PPUTLRC_53 PPUTLRC_54
#define PPUTLRC_212    PPUTLRC_53 PPUTLRC_53 PPUTLRC_53 PPUTLRC_53
#define PPUTLRC_211    PPUTLRC_52 PPUTLRC_53 PPUTLRC_53 PPUTLRC_53
#define PPUTLRC_210    PPUTLRC_52 PPUTLRC_52 PPUTLRC_53 PPUTLRC_53
#define PPUTLRC_209    PPUTLRC_52 PPUTLRC_52 PPUTLRC_52 PPUTLRC_53
#define PPUTLRC_208    PPUTLRC_52 PPUTLRC_52 PPUTLRC_52 PPUTLRC_52
#define PPUTLRC_207    PPUTLRC_51 PPUTLRC_52 PPUTLRC_52 PPUTLRC_52
#define PPUTLRC_206    PPUTLRC_51 PPUTLRC_51 PPUTLRC_52 PPUTLRC_52
#define PPUTLRC_205    PPUTLRC_51 PPUTLRC_51 PPUTLRC_51 PPUTLRC_52
#define PPUTLRC_204    PPUTLRC_51 PPUTLRC_51 PPUTLRC_51 PPUTLRC_51
#define PPUTLRC_203    PPUTLRC_50 PPUTLRC_51 PPUTLRC_51 PPUTLRC_51
#define PPUTLRC_202    PPUTLRC_50 PPUTLRC_50 PPUTLRC_51 PPUTLRC_51
#define PPUTLRC_201    PPUTLRC_50 PPUTLRC_50 PPUTLRC_50 PPUTLRC_51
#define PPUTLRC_200    PPUTLRC_50 PPUTLRC_50 PPUTLRC_50 PPUTLRC_50
#define PPUTLRC_199    PPUTLRC_49 PPUTLRC_50 PPUTLRC_50 PPUTLRC_50
#define PPUTLRC_198    PPUTLRC_49 PPUTLRC_49 PPUTLRC_50 PPUTLRC_50
#define PPUTLRC_197    PPUTLRC_49 PPUTLRC_49 PPUTLRC_49 PPUTLRC_50
#define PPUTLRC_196    PPUTLRC_49 PPUTLRC_49 PPUTLRC_49 PPUTLRC_49
#define PPUTLRC_195    PPUTLRC_48 PPUTLRC_49 PPUTLRC_49 PPUTLRC_49
#define PPUTLRC_194    PPUTLRC_48 PPUTLRC_48 PPUTLRC_49 PPUTLRC_49
#define PPUTLRC_193    PPUTLRC_48 PPUTLRC_48 PPUTLRC_48 PPUTLRC_49
#define PPUTLRC_192    PPUTLRC_48 PPUTLRC_48 PPUTLRC_48 PPUTLRC_48
#define PPUTLRC_191    PPUTLRC_47 PPUTLRC_48 PPUTLRC_48 PPUTLRC_48
#define PPUTLRC_190    PPUTLRC_47 PPUTLRC_47 PPUTLRC_48 PPUTLRC_48
#define PPUTLRC_189    PPUTLRC_47 PPUTLRC_47 PPUTLRC_47 PPUTLRC_48
#define PPUTLRC_188    PPUTLRC_47 PPUTLRC_47 PPUTLRC_47 PPUTLRC_47
#define PPUTLRC_187    PPUTLRC_46 PPUTLRC_47 PPUTLRC_47 PPUTLRC_47
#define PPUTLRC_186    PPUTLRC_46 PPUTLRC_46 PPUTLRC_47 PPUTLRC_47
#define PPUTLRC_185    PPUTLRC_46 PPUTLRC_46 PPUTLRC_46 PPUTLRC_47
#define PPUTLRC_184    PPUTLRC_46 PPUTLRC_46 PPUTLRC_46 PPUTLRC_46
#define PPUTLRC_183    PPUTLRC_45 PPUTLRC_46 PPUTLRC_46 PPUTLRC_46
#define PPUTLRC_182    PPUTLRC_45 PPUTLRC_45 PPUTLRC_46 PPUTLRC_46
#define PPUTLRC_181    PPUTLRC_45 PPUTLRC_45 PPUTLRC_45 PPUTLRC_46
#define PPUTLRC_180    PPUTLRC_45 PPUTLRC_45 PPUTLRC_45 PPUTLRC_45
#define PPUTLRC_179    PPUTLRC_44 PPUTLRC_45 PPUTLRC_45 PPUTLRC_45
#define PPUTLRC_178    PPUTLRC_44 PPUTLRC_44 PPUTLRC_45 PPUTLRC_45
#define PPUTLRC_177    PPUTLRC_44 PPUTLRC_44 PPUTLRC_44 PPUTLRC_45
#define PPUTLRC_176    PPUTLRC_44 PPUTLRC_44 PPUTLRC_44 PPUTLRC_44
#define PPUTLRC_175    PPUTLRC_43 PPUTLRC_44 PPUTLRC_44 PPUTLRC_44
#define PPUTLRC_174    PPUTLRC_43 PPUTLRC_43 PPUTLRC_44 PPUTLRC_44
#define PPUTLRC_173    PPUTLRC_43 PPUTLRC_43 PPUTLRC_43 PPUTLRC_44
#define PPUTLRC_172    PPUTLRC_43 PPUTLRC_43 PPUTLRC_43 PPUTLRC_43
#define PPUTLRC_171    PPUTLRC_42 PPUTLRC_43 PPUTLRC_43 PPUTLRC_43
#define PPUTLRC_170    PPUTLRC_42 PPUTLRC_42 PPUTLRC_43 PPUTLRC_43
#define PPUTLRC_169    PPUTLRC_42 PPUTLRC_42 PPUTLRC_42 PPUTLRC_43
#define PPUTLRC_168    PPUTLRC_42 PPUTLRC_42 PPUTLRC_42 PPUTLRC_42
#define PPUTLRC_167    PPUTLRC_41 PPUTLRC_42 PPUTLRC_42 PPUTLRC_42
#define PPUTLRC_166    PPUTLRC_41 PPUTLRC_41 PPUTLRC_42 PPUTLRC_42
#define PPUTLRC_165    PPUTLRC_41 PPUTLRC_41 PPUTLRC_41 PPUTLRC_42
#define PPUTLRC_164    PPUTLRC_41 PPUTLRC_41 PPUTLRC_41 PPUTLRC_41
#define PPUTLRC_163    PPUTLRC_40 PPUTLRC_41 PPUTLRC_41 PPUTLRC_41
#define PPUTLRC_162    PPUTLRC_40 PPUTLRC_40 PPUTLRC_41 PPUTLRC_41
#define PPUTLRC_161    PPUTLRC_40 PPUTLRC_40 PPUTLRC_40 PPUTLRC_41
#define PPUTLRC_160    PPUTLRC_40 PPUTLRC_40 PPUTLRC_40 PPUTLRC_40
#define PPUTLRC_159    PPUTLRC_39 PPUTLRC_40 PPUTLRC_40 PPUTLRC_40
#define PPUTLRC_158    PPUTLRC_39 PPUTLRC_39 PPUTLRC_40 PPUTLRC_40
#define PPUTLRC_157    PPUTLRC_39 PPUTLRC_39 PPUTLRC_39 PPUTLRC_40
#define PPUTLRC_156    PPUTLRC_39 PPUTLRC_39 PPUTLRC_39 PPUTLRC_39
#define PPUTLRC_155    PPUTLRC_38 PPUTLRC_39 PPUTLRC_39 PPUTLRC_39
#define PPUTLRC_154    PPUTLRC_38 PPUTLRC_38 PPUTLRC_39 PPUTLRC_39
#define PPUTLRC_153    PPUTLRC_38 PPUTLRC_38 PPUTLRC_38 PPUTLRC_39
#define PPUTLRC_152    PPUTLRC_38 PPUTLRC_38 PPUTLRC_38 PPUTLRC_38
#define PPUTLRC_151    PPUTLRC_37 PPUTLRC_38 PPUTLRC_38 PPUTLRC_38
#define PPUTLRC_150    PPUTLRC_37 PPUTLRC_37 PPUTLRC_38 PPUTLRC_38
#define PPUTLRC_149    PPUTLRC_37 PPUTLRC_37 PPUTLRC_37 PPUTLRC_38
#define PPUTLRC_148    PPUTLRC_37 PPUTLRC_37 PPUTLRC_37 PPUTLRC_37
#define PPUTLRC_147    PPUTLRC_36 PPUTLRC_37 PPUTLRC_37 PPUTLRC_37
#define PPUTLRC_146    PPUTLRC_36 PPUTLRC_36 PPUTLRC_37 PPUTLRC_37
#define PPUTLRC_145    PPUTLRC_36 PPUTLRC_36 PPUTLRC_36 PPUTLRC_37
#define PPUTLRC_144    PPUTLRC_36 PPUTLRC_36 PPUTLRC_36 PPUTLRC_36
#define PPUTLRC_143    PPUTLRC_35 PPUTLRC_36 PPUTLRC_36 PPUTLRC_36
#define PPUTLRC_142    PPUTLRC_35 PPUTLRC_35 PPUTLRC_36 PPUTLRC_36
#define PPUTLRC_141    PPUTLRC_35 PPUTLRC_35 PPUTLRC_35 PPUTLRC_36
#define PPUTLRC_140    PPUTLRC_35 PPUTLRC_35 PPUTLRC_35 PPUTLRC_35
#define PPUTLRC_139    PPUTLRC_34 PPUTLRC_35 PPUTLRC_35 PPUTLRC_35
#define PPUTLRC_138    PPUTLRC_34 PPUTLRC_34 PPUTLRC_35 PPUTLRC_35
#define PPUTLRC_137    PPUTLRC_34 PPUTLRC_34 PPUTLRC_34 PPUTLRC_35
#define PPUTLRC_136    PPUTLRC_34 PPUTLRC_34 PPUTLRC_34 PPUTLRC_34
#define PPUTLRC_135    PPUTLRC_33 PPUTLRC_34 PPUTLRC_34 PPUTLRC_34
#define PPUTLRC_134    PPUTLRC_33 PPUTLRC_33 PPUTLRC_34 PPUTLRC_34
#define PPUTLRC_133    PPUTLRC_33 PPUTLRC_33 PPUTLRC_33 PPUTLRC_34
#define PPUTLRC_132    PPUTLRC_33 PPUTLRC_33 PPUTLRC_33 PPUTLRC_33
#define PPUTLRC_131    PPUTLRC_32 PPUTLRC_33 PPUTLRC_33 PPUTLRC_33
#define PPUTLRC_130    PPUTLRC_32 PPUTLRC_32 PPUTLRC_33 PPUTLRC_33
#define PPUTLRC_129    PPUTLRC_32 PPUTLRC_32 PPUTLRC_32 PPUTLRC_33
#define PPUTLRC_128    PPUTLRC_32 PPUTLRC_32 PPUTLRC_32 PPUTLRC_32
#define PPUTLRC_127    PPUTLRC_31 PPUTLRC_32 PPUTLRC_32 PPUTLRC_32
#define PPUTLRC_126    PPUTLRC_31 PPUTLRC_31 PPUTLRC_32 PPUTLRC_32
#define PPUTLRC_125    PPUTLRC_31 PPUTLRC_31 PPUTLRC_31 PPUTLRC_32
#define PPUTLRC_124    PPUTLRC_31 PPUTLRC_31 PPUTLRC_31 PPUTLRC_31
#define PPUTLRC_123    PPUTLRC_30 PPUTLRC_31 PPUTLRC_31 PPUTLRC_31
#define PPUTLRC_122    PPUTLRC_30 PPUTLRC_30 PPUTLRC_31 PPUTLRC_31
#define PPUTLRC_121    PPUTLRC_30 PPUTLRC_30 PPUTLRC_30 PPUTLRC_31
#define PPUTLRC_120    PPUTLRC_30 PPUTLRC_30 PPUTLRC_30 PPUTLRC_30
#define PPUTLRC_119    PPUTLRC_29 PPUTLRC_30 PPUTLRC_30 PPUTLRC_30
#define PPUTLRC_118    PPUTLRC_29 PPUTLRC_29 PPUTLRC_30 PPUTLRC_30
#define PPUTLRC_117    PPUTLRC_29 PPUTLRC_29 PPUTLRC_29 PPUTLRC_30
#define PPUTLRC_116    PPUTLRC_29 PPUTLRC_29 PPUTLRC_29 PPUTLRC_29
#define PPUTLRC_115    PPUTLRC_28 PPUTLRC_29 PPUTLRC_29 PPUTLRC_29
#define PPUTLRC_114    PPUTLRC_28 PPUTLRC_28 PPUTLRC_29 PPUTLRC_29
#define PPUTLRC_113    PPUTLRC_28 PPUTLRC_28 PPUTLRC_28 PPUTLRC_29
#define PPUTLRC_112    PPUTLRC_28 PPUTLRC_28 PPUTLRC_28 PPUTLRC_28
#define PPUTLRC_111    PPUTLRC_27 PPUTLRC_28 PPUTLRC_28 PPUTLRC_28
#define PPUTLRC_110    PPUTLRC_27 PPUTLRC_27 PPUTLRC_28 PPUTLRC_28
#define PPUTLRC_109    PPUTLRC_27 PPUTLRC_27 PPUTLRC_27 PPUTLRC_28
#define PPUTLRC_108    PPUTLRC_27 PPUTLRC_27 PPUTLRC_27 PPUTLRC_27
#define PPUTLRC_107    PPUTLRC_26 PPUTLRC_27 PPUTLRC_27 PPUTLRC_27
#define PPUTLRC_106    PPUTLRC_26 PPUTLRC_26 PPUTLRC_27 PPUTLRC_27
#define PPUTLRC_105    PPUTLRC_26 PPUTLRC_26 PPUTLRC_26 PPUTLRC_27
#define PPUTLRC_104    PPUTLRC_26 PPUTLRC_26 PPUTLRC_26 PPUTLRC_26
#define PPUTLRC_103    PPUTLRC_25 PPUTLRC_26 PPUTLRC_26 PPUTLRC_26
#define PPUTLRC_102    PPUTLRC_25 PPUTLRC_25 PPUTLRC_26 PPUTLRC_26
#define PPUTLRC_101    PPUTLRC_25 PPUTLRC_25 PPUTLRC_25 PPUTLRC_26
#define PPUTLRC_100    PPUTLRC_25 PPUTLRC_25 PPUTLRC_25 PPUTLRC_25
#define PPUTLRC_99     PPUTLRC_24 PPUTLRC_25 PPUTLRC_25 PPUTLRC_25
#define PPUTLRC_98     PPUTLRC_24 PPUTLRC_24 PPUTLRC_25 PPUTLRC_25
#define PPUTLRC_97     PPUTLRC_24 PPUTLRC_24 PPUTLRC_24 PPUTLRC_25
#define PPUTLRC_96     PPUTLRC_24 PPUTLRC_24 PPUTLRC_24 PPUTLRC_24
#define PPUTLRC_95     PPUTLRC_23 PPUTLRC_24 PPUTLRC_24 PPUTLRC_24
#define PPUTLRC_94     PPUTLRC_23 PPUTLRC_23 PPUTLRC_24 PPUTLRC_24
#define PPUTLRC_93     PPUTLRC_23 PPUTLRC_23 PPUTLRC_23 PPUTLRC_24
#define PPUTLRC_92     PPUTLRC_23 PPUTLRC_23 PPUTLRC_23 PPUTLRC_23
#define PPUTLRC_91     PPUTLRC_22 PPUTLRC_23 PPUTLRC_23 PPUTLRC_23
#define PPUTLRC_90     PPUTLRC_22 PPUTLRC_22 PPUTLRC_23 PPUTLRC_23
#define PPUTLRC_89     PPUTLRC_22 PPUTLRC_22 PPUTLRC_22 PPUTLRC_23
#define PPUTLRC_88     PPUTLRC_22 PPUTLRC_22 PPUTLRC_22 PPUTLRC_22
#define PPUTLRC_87     PPUTLRC_21 PPUTLRC_22 PPUTLRC_22 PPUTLRC_22
#define PPUTLRC_86     PPUTLRC_21 PPUTLRC_21 PPUTLRC_22 PPUTLRC_22
#define PPUTLRC_85     PPUTLRC_21 PPUTLRC_21 PPUTLRC_21 PPUTLRC_22
#define PPUTLRC_84     PPUTLRC_21 PPUTLRC_21 PPUTLRC_21 PPUTLRC_21
#define PPUTLRC_83     PPUTLRC_20 PPUTLRC_21 PPUTLRC_21 PPUTLRC_21
#define PPUTLRC_82     PPUTLRC_20 PPUTLRC_20 PPUTLRC_21 PPUTLRC_21
#define PPUTLRC_81     PPUTLRC_20 PPUTLRC_20 PPUTLRC_20 PPUTLRC_21
#define PPUTLRC_80     PPUTLRC_20 PPUTLRC_20 PPUTLRC_20 PPUTLRC_20
#define PPUTLRC_79     PPUTLRC_19 PPUTLRC_20 PPUTLRC_20 PPUTLRC_20
#define PPUTLRC_78     PPUTLRC_19 PPUTLRC_19 PPUTLRC_20 PPUTLRC_20
#define PPUTLRC_77     PPUTLRC_19 PPUTLRC_19 PPUTLRC_19 PPUTLRC_20
#define PPUTLRC_76     PPUTLRC_19 PPUTLRC_19 PPUTLRC_19 PPUTLRC_19
#define PPUTLRC_75     PPUTLRC_18 PPUTLRC_19 PPUTLRC_19 PPUTLRC_19
#define PPUTLRC_74     PPUTLRC_18 PPUTLRC_18 PPUTLRC_19 PPUTLRC_19
#define PPUTLRC_73     PPUTLRC_18 PPUTLRC_18 PPUTLRC_18 PPUTLRC_19
#define PPUTLRC_72     PPUTLRC_18 PPUTLRC_18 PPUTLRC_18 PPUTLRC_18
#define PPUTLRC_71     PPUTLRC_17 PPUTLRC_18 PPUTLRC_18 PPUTLRC_18
#define PPUTLRC_70     PPUTLRC_17 PPUTLRC_17 PPUTLRC_18 PPUTLRC_18
#define PPUTLRC_69     PPUTLRC_17 PPUTLRC_17 PPUTLRC_17 PPUTLRC_18
#define PPUTLRC_68     PPUTLRC_17 PPUTLRC_17 PPUTLRC_17 PPUTLRC_17
#define PPUTLRC_67     PPUTLRC_16 PPUTLRC_17 PPUTLRC_17 PPUTLRC_17
#define PPUTLRC_66     PPUTLRC_16 PPUTLRC_16 PPUTLRC_17 PPUTLRC_17
#define PPUTLRC_65     PPUTLRC_16 PPUTLRC_16 PPUTLRC_16 PPUTLRC_17
#define PPUTLRC_64     PPUTLRC_16 PPUTLRC_16 PPUTLRC_16 PPUTLRC_16
#define PPUTLRC_63     PPUTLRC_15 PPUTLRC_16 PPUTLRC_16 PPUTLRC_16
#define PPUTLRC_62     PPUTLRC_15 PPUTLRC_15 PPUTLRC_16 PPUTLRC_16
#define PPUTLRC_61     PPUTLRC_15 PPUTLRC_15 PPUTLRC_15 PPUTLRC_16
#define PPUTLRC_60     PPUTLRC_15 PPUTLRC_15 PPUTLRC_15 PPUTLRC_15
#define PPUTLRC_59     PPUTLRC_14 PPUTLRC_15 PPUTLRC_15 PPUTLRC_15
#define PPUTLRC_58     PPUTLRC_14 PPUTLRC_14 PPUTLRC_15 PPUTLRC_15
#define PPUTLRC_57     PPUTLRC_14 PPUTLRC_14 PPUTLRC_14 PPUTLRC_15
#define PPUTLRC_56     PPUTLRC_14 PPUTLRC_14 PPUTLRC_14 PPUTLRC_14
#define PPUTLRC_55     PPUTLRC_13 PPUTLRC_14 PPUTLRC_14 PPUTLRC_14
#define PPUTLRC_54     PPUTLRC_13 PPUTLRC_13 PPUTLRC_14 PPUTLRC_14
#define PPUTLRC_53     PPUTLRC_13 PPUTLRC_13 PPUTLRC_13 PPUTLRC_14
#define PPUTLRC_52     PPUTLRC_13 PPUTLRC_13 PPUTLRC_13 PPUTLRC_13
#define PPUTLRC_51     PPUTLRC_12 PPUTLRC_13 PPUTLRC_13 PPUTLRC_13
#define PPUTLRC_50     PPUTLRC_12 PPUTLRC_12 PPUTLRC_13 PPUTLRC_13
#define PPUTLRC_49     PPUTLRC_12 PPUTLRC_12 PPUTLRC_12 PPUTLRC_13
#define PPUTLRC_48     PPUTLRC_12 PPUTLRC_12 PPUTLRC_12 PPUTLRC_12
#define PPUTLRC_47     PPUTLRC_11 PPUTLRC_12 PPUTLRC_12 PPUTLRC_12
#define PPUTLRC_46     PPUTLRC_11 PPUTLRC_11 PPUTLRC_12 PPUTLRC_12
#define PPUTLRC_45     PPUTLRC_11 PPUTLRC_11 PPUTLRC_11 PPUTLRC_12
#define PPUTLRC_44     PPUTLRC_11 PPUTLRC_11 PPUTLRC_11 PPUTLRC_11
#define PPUTLRC_43     PPUTLRC_10 PPUTLRC_11 PPUTLRC_11 PPUTLRC_11
#define PPUTLRC_42     PPUTLRC_10 PPUTLRC_10 PPUTLRC_11 PPUTLRC_11
#define PPUTLRC_41     PPUTLRC_10 PPUTLRC_10 PPUTLRC_10 PPUTLRC_11
#define PPUTLRC_40     PPUTLRC_10 PPUTLRC_10 PPUTLRC_10 PPUTLRC_10
#define PPUTLRC_39     PPUTLRC_9 PPUTLRC_10 PPUTLRC_10 PPUTLRC_10
#define PPUTLRC_38     PPUTLRC_9 PPUTLRC_9 PPUTLRC_10 PPUTLRC_10
#define PPUTLRC_37     PPUTLRC_9 PPUTLRC_9 PPUTLRC_9 PPUTLRC_10
#define PPUTLRC_36     PPUTLRC_9 PPUTLRC_9 PPUTLRC_9 PPUTLRC_9
#define PPUTLRC_35     PPUTLRC_8 PPUTLRC_9 PPUTLRC_9 PPUTLRC_9
#define PPUTLRC_34     PPUTLRC_8 PPUTLRC_8 PPUTLRC_9 PPUTLRC_9
#define PPUTLRC_33     PPUTLRC_8 PPUTLRC_8 PPUTLRC_8 PPUTLRC_9
#define PPUTLRC_32     PPUTLRC_8 PPUTLRC_8 PPUTLRC_8 PPUTLRC_8
#define PPUTLRC_31     PPUTLRC_7 PPUTLRC_8 PPUTLRC_8 PPUTLRC_8
#define PPUTLRC_30     PPUTLRC_7 PPUTLRC_7 PPUTLRC_8 PPUTLRC_8
#define PPUTLRC_29     PPUTLRC_7 PPUTLRC_7 PPUTLRC_7 PPUTLRC_8
#define PPUTLRC_28     PPUTLRC_7 PPUTLRC_7 PPUTLRC_7 PPUTLRC_7
#define PPUTLRC_27     PPUTLRC_6 PPUTLRC_7 PPUTLRC_7 PPUTLRC_7
#define PPUTLRC_26     PPUTLRC_6 PPUTLRC_6 PPUTLRC_7 PPUTLRC_7
#define PPUTLRC_25     PPUTLRC_6 PPUTLRC_6 PPUTLRC_6 PPUTLRC_7
#define PPUTLRC_24     PPUTLRC_6 PPUTLRC_6 PPUTLRC_6 PPUTLRC_6
#define PPUTLRC_23     PPUTLRC_5 PPUTLRC_6 PPUTLRC_6 PPUTLRC_6
#define PPUTLRC_22     PPUTLRC_5 PPUTLRC_5 PPUTLRC_6 PPUTLRC_6
#define PPUTLRC_21     PPUTLRC_5 PPUTLRC_5 PPUTLRC_5 PPUTLRC_6
#define PPUTLRC_20     PPUTLRC_5 PPUTLRC_5 PPUTLRC_5 PPUTLRC_5
#define PPUTLRC_19     PPUTLRC_4 PPUTLRC_5 PPUTLRC_5 PPUTLRC_5
#define PPUTLRC_18     PPUTLRC_4 PPUTLRC_4 PPUTLRC_5 PPUTLRC_5
#define PPUTLRC_17     PPUTLRC_4 PPUTLRC_4 PPUTLRC_4 PPUTLRC_5
#define PPUTLRC_16     PPUTLRC_4 PPUTLRC_4 PPUTLRC_4 PPUTLRC_4
#define PPUTLRC_15     PPUTLRC_3 PPUTLRC_4 PPUTLRC_4 PPUTLRC_4
#define PPUTLRC_14     PPUTLRC_3 PPUTLRC_3 PPUTLRC_4 PPUTLRC_4
#define PPUTLRC_13     PPUTLRC_3 PPUTLRC_3 PPUTLRC_3 PPUTLRC_4
#define PPUTLRC_12     PPUTLRC_3 PPUTLRC_3 PPUTLRC_3 PPUTLRC_3
#define PPUTLRC_11     PPUTLRC_2 PPUTLRC_3 PPUTLRC_3 PPUTLRC_3
#define PPUTLRC_10     PPUTLRC_2 PPUTLRC_2 PPUTLRC_3 PPUTLRC_3
#define PPUTLRC_9      PPUTLRC_2 PPUTLRC_2 PPUTLRC_2 PPUTLRC_3
#define PPUTLRC_8      PPUTLRC_2 PPUTLRC_2 PPUTLRC_2 PPUTLRC_2
#define PPUTLRC_7      PPUTLRC_1 PPUTLRC_2 PPUTLRC_2 PPUTLRC_2
#define PPUTLRC_6      PPUTLRC_1 PPUTLRC_1 PPUTLRC_2 PPUTLRC_2
#define PPUTLRC_5      PPUTLRC_1 PPUTLRC_1 PPUTLRC_1 PPUTLRC_2
#define PPUTLRC_4      PPUTLRC_1 PPUTLRC_1 PPUTLRC_1 PPUTLRC_1
#define PPUTLRC_3      PPUTLRC_1 PPUTLRC_1 PPUTLRC_1
#define PPUTLRC_2      PPUTLRC_1 PPUTLRC_1
#define PPUTLRC_1      PTL_RP() PTL_RP() PTL_RP() PTL_RP()
#define PPUTLRC_0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

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
#define PTL_IF(/* b: bool, t: tuple, f: tuple */...) /* -> b ? ...t : ...f */ \
  PTL_CAT(PPUTLIF_, PTL_BOOL(PTL_FIRST(__VA_ARGS__)))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIF_1(_, t, f) PTL_REST((PTL_TUPLE(f)), PTL_ITEMS(t))
#define PPUTLIF_0(_, t, f) PTL_REST((PTL_TUPLE(t)), PTL_ITEMS(f))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [control.switch]
/// ----------------
/// conditionally expands items based on a uint.
/// the final tuple is the default case.
///
/// PTL_SWITCH(0, (1))                 // 1
/// PTL_SWITCH(1, (1))                 // 1
/// PTL_SWITCH(2, (1))                 // 1
/// PTL_SWITCH(1, (1), (2))            // 2
/// PTL_SWITCH(2, (1), (2))            // 2
/// PTL_SWITCH(2, (1), (2), (3, 4))    // 3, 4
/// PTL_SWITCH(1023, (1), (2), (3, 4)) // 3, 4
#define PTL_SWITCH(/* cs: uint, cases: tuple... */...) /* -> ...cases[cs] */         \
  PPUTLSWITCH_RES(PPUTLSWITCH_X(PTL_ROPEN(PTL_FIRST(__VA_ARGS__), PPUTLSWITCH_RECUR) \
                                    __VA_ARGS__ PTL_RCLOSE(PTL_FIRST(__VA_ARGS__))))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLSWITCH_RES(...)         PPUTLSWITCH_RES_X(__VA_ARGS__)
#define PPUTLSWITCH_RES_X(i, _, ...) PTL_ITEMS(_)
#define PPUTLSWITCH_X(...)           __VA_ARGS__
#define PPUTLSWITCH_RECUR(...)       PPUTLSWITCH_RECUR_X(__VA_ARGS__)
#define PPUTLSWITCH_RECUR_X(i, _, ...) \
  PTL_IF(PTL_IF(PTL_EQZ(i), (1), (PTL_IS_NONE(__VA_ARGS__))), (0, _), (PTL_DEC(i), __VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [logic.not]
/// -----------
/// logical NOT.
///
/// PTL_NOT(0) // 1
/// PTL_NOT(1) // 0
#define PTL_NOT(/* v: bool */...) /* -> bool{!v} */ PTL_IF(__VA_ARGS__, (0), (1))

/// [logic.and]
/// -----------
/// logical AND.
///
/// PTL_AND(0, 0) // 0
/// PTL_AND(0, 1) // 0
/// PTL_AND(1, 0) // 0
/// PTL_AND(1, 1) // 1
#define PTL_AND(/* a: bool, b: bool */...) /* -> bool{a and b} */ \
  PTL_IF(PTL_FIRST(__VA_ARGS__), (PTL_BOOL(PTL_REST(__VA_ARGS__))), (0))

/// [logic.or]
/// ----------
/// logical OR.
///
/// PTL_OR(0, 0) // 0
/// PTL_OR(0, 1) // 1
/// PTL_OR(1, 0) // 1
/// PTL_OR(1, 1) // 1
#define PTL_OR(/* a: bool, b: bool */...) /* -> bool{a or b} */ \
  PTL_IF(PTL_FIRST(__VA_ARGS__), (1), (PTL_BOOL(PTL_REST(__VA_ARGS__))))

/// [logic.nand]
/// ------------
/// logical NAND.
///
/// PTL_NAND(0, 0) // 1
/// PTL_NAND(0, 1) // 1
/// PTL_NAND(1, 0) // 1
/// PTL_NAND(1, 1) // 0
#define PTL_NAND(/* a: bool, b: bool */...) /* -> bool{!(a and b)} */ \
  PTL_IF(PTL_FIRST(__VA_ARGS__), (PTL_NOT(PTL_REST(__VA_ARGS__))), (1))

/// [logic.nor]
/// -----------
/// logical NOR.
///
/// PTL_NOR(0, 0) // 1
/// PTL_NOR(0, 1) // 0
/// PTL_NOR(1, 0) // 0
/// PTL_NOR(1, 1) // 0
#define PTL_NOR(/* a: bool, b: bool */...) /* -> bool{!(a or b)} */ \
  PTL_IF(PTL_FIRST(__VA_ARGS__), (0), (PTL_NOT(PTL_REST(__VA_ARGS__))))

/// [logic.xor]
/// -----------
/// logical XOR.
///
/// PTL_XOR(0, 0) // 0
/// PTL_XOR(0, 1) // 1
/// PTL_XOR(1, 0) // 1
/// PTL_XOR(1, 1) // 0
#define PTL_XOR(/* a: bool, b: bool */...) /* -> bool{a xor b} */  \
  PTL_IF(PTL_FIRST(__VA_ARGS__), (PTL_NOT(PTL_REST(__VA_ARGS__))), \
         (PTL_BOOL(PTL_REST(__VA_ARGS__))))

/// [logic.xnor]
/// ------------
/// logical XNOR.
///
/// PTL_XNOR(0, 0) // 1
/// PTL_XNOR(0, 1) // 0
/// PTL_XNOR(1, 0) // 0
/// PTL_XNOR(1, 1) // 1
#define PTL_XNOR(/* a: bool, b: bool */...) /* -> bool{!(a xor b)} */ \
  PTL_IF(PTL_FIRST(__VA_ARGS__), (PTL_BOOL(PTL_REST(__VA_ARGS__))),   \
         (PTL_NOT(PTL_REST(__VA_ARGS__))))

/// [math.add]
/// ----------
/// uint addition with overflow.
///
/// PTL_ADD(0, 0)    // 0
/// PTL_ADD(0, 1)    // 1
/// PTL_ADD(1, 2)    // 3
/// PTL_ADD(1023, 1) // 0
/// PTL_ADD(1023, 2) // 1
#define PTL_ADD(/* a: uint, b: uint */...) /* -> uint{a + b} */               \
  PPUTLADD_X(PTL_ROPEN(PTL_FIRST(__VA_ARGS__), PTL_INC) PTL_REST(__VA_ARGS__) \
                 PTL_RCLOSE(PTL_FIRST(__VA_ARGS__)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLADD_X(...) __VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [math.sub]
/// ----------
/// uint subtraction with underflow.
///
/// PTL_SUB(0, 0)    // 0
/// PTL_SUB(0, 1)    // 1023
/// PTL_SUB(1, 0)    // 1
/// PTL_SUB(1, 1)    // 0
/// PTL_SUB(3, 1)    // 2
/// PTL_SUB(1, 3)    // 1022
/// PTL_SUB(0, 1023) // 1
#define PTL_SUB(/* a: uint, b: uint */...) /* -> uint{a - b} */               \
  PPUTLSUB_X(PTL_ROPEN(PTL_REST(__VA_ARGS__), PTL_DEC) PTL_FIRST(__VA_ARGS__) \
                 PTL_RCLOSE(PTL_REST(__VA_ARGS__)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLSUB_X(...) __VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [math.mul]
/// ----------
/// uint multiplication with overflow.
///
/// PTL_MUL(0, 0) // 0
/// PTL_MUL(0, 1) // 0
/// PTL_MUL(1, 1) // 1
/// PTL_MUL(1, 2) // 2
/// PTL_MUL(2, 2) // 4
#define PTL_MUL(/* a: uint, b: uint */...) /* -> uint{a * b} */             \
  PTL_FIRST(PPUTLMUL_X(PTL_ROPEN(PTL_FIRST(__VA_ARGS__), PPUTLMUL_RECUR) 0, \
                       PTL_REST(__VA_ARGS__) PTL_RCLOSE(PTL_FIRST(__VA_ARGS__))))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLMUL_X(...)        __VA_ARGS__
#define PPUTLMUL_RECUR(...)    PPUTLMUL_RECUR_X(__VA_ARGS__)
#define PPUTLMUL_RECUR_X(a, b) PTL_ADD(a, b), b

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [compare.lt]
/// ------------
/// O(logn) uint less-than comparison.
///
/// PTL_LT(0, 0) // 0
/// PTL_LT(0, 1) // 1
/// PTL_LT(1, 0) // 0
/// PTL_LT(1, 1) // 0
#define PTL_LT(/* l: uint, r: uint */...) /* -> uint{l < r} */ \
  PTL_IF(PTL_EQZ(PTL_FIRST(__VA_ARGS__)), (PPUTLLT_ZERO_L), (PPUTLLT_POS_L))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLLT_POS_L(l, r)                                                      \
  PPUTLLT_RES(PPUTLLT_X(PTL_ROPEN(PTL_LOG2(l), PPUTLLT_RECUR) l, r, PTL_MOD2(l), \
                        PTL_MOD2(r) PTL_RCLOSE(PTL_LOG2(l))))
#define PPUTLLT_ZERO_L(l, r) PTL_NEZ(r)
#define PPUTLLT_RES(...)     PPUTLLT_RES_X(__VA_ARGS__)
#define PPUTLLT_RES_X(l, r, ml, mr)                \
  PTL_OR(PTL_AND(PTL_NEZ(r), PTL_NEZ(PTL_DEC(r))), \
         PTL_AND(PTL_EQZ(PTL_DEC(r)), PTL_AND(PTL_NOT(ml), mr)))
#define PPUTLLT_RECUR(...)            PPUTLLT_RECUR_X(__VA_ARGS__)
#define PPUTLLT_RECUR_X(l, r, ml, mr) PTL_DIV2(l), PTL_DIV2(r), ml, mr
#define PPUTLLT_X(...)                __VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [compare.gt]
/// ------------
/// O(logn) uint greater-than comparison.
///
/// PTL_GT(0, 0) // 0
/// PTL_GT(0, 1) // 0
/// PTL_GT(1, 0) // 1
/// PTL_GT(1, 1) // 0
#define PTL_GT(/* l: uint, r: uint */...) /* -> uint{l > r} */ PPUTLGT_X(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLGT_X(l, r) PTL_LT(r, l)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [compare.le]
/// ------------
/// O(logn) uint less-than-or-equal-to comparison.
///
/// PTL_LE(0, 0) // 1
/// PTL_LE(0, 1) // 1
/// PTL_LE(1, 0) // 0
/// PTL_LE(1, 1) // 1
#define PTL_LE(/* l: uint, r: uint */...) /* -> uint{l <= r} */ PTL_NOT(PTL_GT(__VA_ARGS__))

/// [compare.ge]
/// ------------
/// O(logn) uint greater-than-or-equal-to comparison.
///
/// PTL_GE(0, 0) // 1
/// PTL_GE(0, 1) // 0
/// PTL_GE(1, 0) // 1
/// PTL_GE(1, 1) // 1
#define PTL_GE(/* l: uint, r: uint */...) /* -> uint{l >= r} */ PTL_NOT(PTL_LT(__VA_ARGS__))

/// [compare.eq]
/// ------------
/// O(logn) uint equal-to comparison.
///
/// PTL_EQ(0, 0) // 1
/// PTL_EQ(0, 1) // 0
/// PTL_EQ(1, 0) // 0
/// PTL_EQ(1, 1) // 1
#define PTL_EQ(/* l: uint, r: uint */...) /* -> uint{l == r} */ \
  PTL_AND(PTL_LE(__VA_ARGS__), PTL_GE(__VA_ARGS__))

/// [compare.ne]
/// ------------
/// O(logn) uint not-equal-to comparison.
///
/// PTL_EQ(0, 0) // 1
/// PTL_EQ(0, 1) // 0
/// PTL_EQ(1, 0) // 0
/// PTL_EQ(1, 1) // 1
#define PTL_NE(/* l: uint, r: uint */...) /* -> uint{l != r} */ PTL_NOT(PTL_EQ(__VA_ARGS__))

/// [compare.min]
/// -------------
/// O(logn) uint minimum operation.
///
/// PTL_MIN(0, 0) // 0
/// PTL_MIN(0, 1) // 0
/// PTL_MIN(1, 0) // 0
/// PTL_MIN(1, 1) // 1
#define PTL_MIN(/* l: uint, r: uint */...) /* -> uint{l < r ? l : r} */ \
  PTL_IF(PTL_LT(__VA_ARGS__), (PTL_FIRST(__VA_ARGS__)), (PTL_REST(__VA_ARGS__)))

/// [compare.max]
/// -------------
/// O(logn) uint maximum operation.
///
/// PTL_MAX(0, 0) // 0
/// PTL_MAX(0, 1) // 1
/// PTL_MAX(1, 0) // 1
/// PTL_MAX(1, 1) // 1
#define PTL_MAX(/* l: uint, r: uint */...) /* -> uint{l > r ? r : l} */ \
  PTL_IF(PTL_GT(__VA_ARGS__), (PTL_FIRST(__VA_ARGS__)), (PTL_REST(__VA_ARGS__)))

// vim: fdm=marker:fmr={{{,}}}

#endif

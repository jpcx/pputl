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
//    many high-level programming constructs and a 10-bit unsigned integer    //
//    space with binary arithmetic capabilities.                              //
//                                                                            //
//    pputl algorithms  are built using a preprocessor syntax manipulation    //
//    technique for constructing inline recursive call stacks that execute    //
//    much faster than mutually-recursive methods.                            //
//                                                                            //
//    pputl requires __VA_ARGS__, __VA_OPT__, and empty variadic arguments    //
//    support (which are guaranteed by C++20) but has no dependencies.        //
//                                                                            //
//    pputl  is completely generated and tested by a custom C++ framework.    //
//    See the codegen/ folder for the full source.                            //
//                                                                            //
//    USAGE                                                                   //
//    -----                                                                   //
//    Copy pputl.h and include. The distribution is single-header.            //
//                                                                            //
//    Modify the head of codegen/codegen.h  to configure the  unsigned bit    //
//    size or naming preferences and run `make` to regenerate.                //
//                                                                            //
//    Run `make test` to validate the library on your system.                 //
//                                                                            //
//    FUNDAMENTALS                                                            //
//    ------------                                                            //
//                                                                            //
//    Non-nullary API functions are fully variadic and chainable such that    //
//    the outputs of one may be used as inputs to another. Parameters must    //
//    be fully expanded and distinguishable after the primary expansion.      //
//                                                                            //
//    Tuples are used only when necessary.  Most functions that operate on    //
//    generic data ranges  both input and output a variadic argument list.    //
//    Creating a tuple is trivial but extraction costs an expansion.          //
//                                                                            //
//    pputl defines three types: tuple, bool, and uint.  Features that use    //
//    one of  these types  in their  parameter documentation  assert their    //
//    validity by type-casting.  Type casts expand to their original value    //
//    if successful, else they trigger a preprocessor error.                  //
//                                                                            //
//    uint values are one of two subtypes: decimal or binary.  uint may be    //
//    constructed from either of these representations.  Binary values are    //
//    represented using a 0b prefix and are always fixed-size.                //
//                                                                            //
//    pputl errors execute  an invalid preprocessor operation by using the    //
//    concatenation operator (incorrectly) on a string error message.  All    //
//    errors  triggered by  pputl functions  will include  the macro name,   ///
//    a textual description, and the primary expansion arguments.           ////
//                                                                         /////
///////////////////////////////////////////////////////////////////////////// */

/// [config.build]
/// --------------
/// the build number of this pputl release (ISO8601).
#define PTL_BUILD /* -> <c++ int> */ 20220807

/// [config.uint_bits]
/// ------------------
/// the number of bits in a pputl unsigned int.
/// see the readme code generation section to configure.
#define PTL_UINT_BITS /* -> uint */ 10

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
#define PTL_CAT(/* [a]: any, [b]: any */...) /* -> a##b */ __VA_OPT__(PPUTLCAT_X(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLCAT_X(a, b) a##b

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [lang.trycat]
/// -------------
/// concatenates two args after an expansion.
/// returns default if either arg is a tuple.
///
/// args must either be compatible with the ## operator
/// or be tuples. several tokens are incompatible with
/// the ## operator that cannot be detected; tuples are
/// the only detectible fail condition.
///
/// PTL_TRYCAT(foo, bar)          // foobar
/// PTL_TRYCAT(foo, PTL_EAT(bar)) // foo
/// PTL_TRYCAT(foo, (bar))        // <nothing>
/// PTL_TRYCAT(foo, (bar), err)   // err
#define PTL_TRYCAT(/* [a]: any, [b]: any, [default=]: any... */...) /* -> a##b | default */ \
  __VA_OPT__(PPUTLTRYCAT_O(__VA_ARGS__)(__VA_ARGS__)(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLTRYCAT_O(a, b, ...)           PPUTLTRYCAT_OO_CHK(PTL_EAT a)
#define PPUTLTRYCAT_OO_CHK(...)            PPUTLTRYCAT_OO##__VA_OPT__(_NO)##_FAIL
#define PPUTLTRYCAT_OO_NO_FAIL(a, b, ...)  PPUTLTRYCAT_OOO_CHK(PTL_EAT b)
#define PPUTLTRYCAT_OO_FAIL(a, b, ...)     PPUTLTRYCAT_OOO_FAIL
#define PPUTLTRYCAT_OOO_CHK(...)           PPUTLTRYCAT_OOO##__VA_OPT__(_NO)##_FAIL
#define PPUTLTRYCAT_OOO_NO_FAIL(a, b, ...) a##b
#define PPUTLTRYCAT_OOO_FAIL(a, b, ...)    __VA_ARGS__

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
#define PTL_TUPLE(/* t: tuple */...) /* -> t */ \
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
#define PTL_BOOL(/* b: bool */...) /* -> b */ \
  PPUTLBOOL_O(__VA_ARGS__.)                   \
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
/// constructible from binary or decimal.
///
/// PTL_UINT(0)    // 0
/// PTL_UINT(1)    // 1
/// PTL_UINT(2)    // 2
/// PTL_UINT(1023) // 1023
#define PTL_UINT(/* n: uint */...) /* -> n */ \
  PPUTLUINT_O(__VA_ARGS__.)                   \
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
#define PPUTLUINT_OOO(...)          PPUTLUINT_OOO_RES(PPUTLU_##__VA_ARGS__)
#define PPUTLUINT_OOO_RES(...)      PPUTLUINT_OOO_RES_X(__VA_ARGS__)
#define PPUTLUINT_OOO_RES_X(_, ...) PPUTLUINT_OOO_##__VA_OPT__(NO_)##FAIL()
#define PPUTLUINT_OOO_NO_FAIL(...)  PPUTLUINT_PASS
#define PPUTLUINT_OOO_FAIL(...)     PPUTLUINT_FAIL

/// fourth parentheses; returns
#define PPUTLUINT_PASS(err, ...) __VA_ARGS__
#define PPUTLUINT_FAIL(err, ...) PTL_FAIL(err)

// clang-format off

/// dec, inc, log2, div2, mul2, mflo, sqrt, pow2, pflo, mod2, mod4, mod8, mod16, mod32, mod64, fact, bin
#define PPUTLU_1023 1022,0,9,511,1022,1,31,1,1022,1,3,7,15,31,63,(3, 11, 31),(1,1,1,1,1,1,1,1,1,1)
#define PPUTLU_1022 1021,1023,9,511,1020,1,31,4,1020,0,2,6,14,30,62,(2, 7, 73),(1,1,1,1,1,1,1,1,1,0)
#define PPUTLU_1021 1020,1022,9,510,1018,1,31,9,1018,1,1,5,13,29,61,(),(1,1,1,1,1,1,1,1,0,1)
#define PPUTLU_1020 1019,1021,9,510,1016,1,31,16,1016,0,0,4,12,28,60,(2, 2, 3, 5, 17),(1,1,1,1,1,1,1,1,0,0)
#define PPUTLU_1019 1018,1020,9,509,1014,1,31,25,1014,1,3,3,11,27,59,(),(1,1,1,1,1,1,1,0,1,1)
#define PPUTLU_1018 1017,1019,9,509,1012,1,31,36,1012,0,2,2,10,26,58,(2, 509),(1,1,1,1,1,1,1,0,1,0)
#define PPUTLU_1017 1016,1018,9,508,1010,1,31,49,1010,1,1,1,9,25,57,(3, 3, 113),(1,1,1,1,1,1,1,0,0,1)
#define PPUTLU_1016 1015,1017,9,508,1008,1,31,64,1008,0,0,0,8,24,56,(2, 2, 2, 127),(1,1,1,1,1,1,1,0,0,0)
#define PPUTLU_1015 1014,1016,9,507,1006,1,31,81,1006,1,3,7,7,23,55,(5, 7, 29),(1,1,1,1,1,1,0,1,1,1)
#define PPUTLU_1014 1013,1015,9,507,1004,1,31,100,1004,0,2,6,6,22,54,(2, 3, 13, 13),(1,1,1,1,1,1,0,1,1,0)
#define PPUTLU_1013 1012,1014,9,506,1002,1,31,121,1002,1,1,5,5,21,53,(),(1,1,1,1,1,1,0,1,0,1)
#define PPUTLU_1012 1011,1013,9,506,1000,1,31,144,1000,0,0,4,4,20,52,(2, 2, 11, 23),(1,1,1,1,1,1,0,1,0,0)
#define PPUTLU_1011 1010,1012,9,505,998,1,31,169,998,1,3,3,3,19,51,(3, 337),(1,1,1,1,1,1,0,0,1,1)
#define PPUTLU_1010 1009,1011,9,505,996,1,31,196,996,0,2,2,2,18,50,(2, 5, 101),(1,1,1,1,1,1,0,0,1,0)
#define PPUTLU_1009 1008,1010,9,504,994,1,31,225,994,1,1,1,1,17,49,(),(1,1,1,1,1,1,0,0,0,1)
#define PPUTLU_1008 1007,1009,9,504,992,1,31,256,992,0,0,0,0,16,48,(2, 2, 2, 2, 3, 3, 7),(1,1,1,1,1,1,0,0,0,0)
#define PPUTLU_1007 1006,1008,9,503,990,1,31,289,990,1,3,7,15,15,47,(19, 53),(1,1,1,1,1,0,1,1,1,1)
#define PPUTLU_1006 1005,1007,9,503,988,1,31,324,988,0,2,6,14,14,46,(2, 503),(1,1,1,1,1,0,1,1,1,0)
#define PPUTLU_1005 1004,1006,9,502,986,1,31,361,986,1,1,5,13,13,45,(3, 5, 67),(1,1,1,1,1,0,1,1,0,1)
#define PPUTLU_1004 1003,1005,9,502,984,1,31,400,984,0,0,4,12,12,44,(2, 2, 251),(1,1,1,1,1,0,1,1,0,0)
#define PPUTLU_1003 1002,1004,9,501,982,1,31,441,982,1,3,3,11,11,43,(17, 59),(1,1,1,1,1,0,1,0,1,1)
#define PPUTLU_1002 1001,1003,9,501,980,1,31,484,980,0,2,2,10,10,42,(2, 3, 167),(1,1,1,1,1,0,1,0,1,0)
#define PPUTLU_1001 1000,1002,9,500,978,1,31,529,978,1,1,1,9,9,41,(7, 11, 13),(1,1,1,1,1,0,1,0,0,1)
#define PPUTLU_1000 999,1001,9,500,976,1,31,576,976,0,0,0,8,8,40,(2, 2, 2, 5, 5, 5),(1,1,1,1,1,0,1,0,0,0)
#define PPUTLU_999 998,1000,9,499,974,1,31,625,974,1,3,7,7,7,39,(3, 3, 3, 37),(1,1,1,1,1,0,0,1,1,1)
#define PPUTLU_998 997,999,9,499,972,1,31,676,972,0,2,6,6,6,38,(2, 499),(1,1,1,1,1,0,0,1,1,0)
#define PPUTLU_997 996,998,9,498,970,1,31,729,970,1,1,5,5,5,37,(),(1,1,1,1,1,0,0,1,0,1)
#define PPUTLU_996 995,997,9,498,968,1,31,784,968,0,0,4,4,4,36,(2, 2, 3, 83),(1,1,1,1,1,0,0,1,0,0)
#define PPUTLU_995 994,996,9,497,966,1,31,841,966,1,3,3,3,3,35,(5, 199),(1,1,1,1,1,0,0,0,1,1)
#define PPUTLU_994 993,995,9,497,964,1,31,900,964,0,2,2,2,2,34,(2, 7, 71),(1,1,1,1,1,0,0,0,1,0)
#define PPUTLU_993 992,994,9,496,962,1,31,961,962,1,1,1,1,1,33,(3, 331),(1,1,1,1,1,0,0,0,0,1)
#define PPUTLU_992 991,993,9,496,960,1,31,0,961,0,0,0,0,0,32,(2, 2, 2, 2, 2, 31),(1,1,1,1,1,0,0,0,0,0)
#define PPUTLU_991 990,992,9,495,958,1,31,65,959,1,3,7,15,31,31,(),(1,1,1,1,0,1,1,1,1,1)
#define PPUTLU_990 989,991,9,495,956,1,31,132,957,0,2,6,14,30,30,(2, 3, 3, 5, 11),(1,1,1,1,0,1,1,1,1,0)
#define PPUTLU_989 988,990,9,494,954,1,31,201,955,1,1,5,13,29,29,(23, 43),(1,1,1,1,0,1,1,1,0,1)
#define PPUTLU_988 987,989,9,494,952,1,31,272,953,0,0,4,12,28,28,(2, 2, 13, 19),(1,1,1,1,0,1,1,1,0,0)
#define PPUTLU_987 986,988,9,493,950,1,31,345,951,1,3,3,11,27,27,(3, 7, 47),(1,1,1,1,0,1,1,0,1,1)
#define PPUTLU_986 985,987,9,493,948,1,31,420,949,0,2,2,10,26,26,(2, 17, 29),(1,1,1,1,0,1,1,0,1,0)
#define PPUTLU_985 984,986,9,492,946,1,31,497,947,1,1,1,9,25,25,(5, 197),(1,1,1,1,0,1,1,0,0,1)
#define PPUTLU_984 983,985,9,492,944,1,31,576,945,0,0,0,8,24,24,(2, 2, 2, 3, 41),(1,1,1,1,0,1,1,0,0,0)
#define PPUTLU_983 982,984,9,491,942,1,31,657,943,1,3,7,7,23,23,(),(1,1,1,1,0,1,0,1,1,1)
#define PPUTLU_982 981,983,9,491,940,1,31,740,941,0,2,6,6,22,22,(2, 491),(1,1,1,1,0,1,0,1,1,0)
#define PPUTLU_981 980,982,9,490,938,1,31,825,939,1,1,5,5,21,21,(3, 3, 109),(1,1,1,1,0,1,0,1,0,1)
#define PPUTLU_980 979,981,9,490,936,1,31,912,937,0,0,4,4,20,20,(2, 2, 5, 7, 7),(1,1,1,1,0,1,0,1,0,0)
#define PPUTLU_979 978,980,9,489,934,1,31,1001,935,1,3,3,3,19,19,(11, 89),(1,1,1,1,0,1,0,0,1,1)
#define PPUTLU_978 977,979,9,489,932,1,31,68,934,0,2,2,2,18,18,(2, 3, 163),(1,1,1,1,0,1,0,0,1,0)
#define PPUTLU_977 976,978,9,488,930,1,31,161,932,1,1,1,1,17,17,(),(1,1,1,1,0,1,0,0,0,1)
#define PPUTLU_976 975,977,9,488,928,1,31,256,930,0,0,0,0,16,16,(2, 2, 2, 2, 61),(1,1,1,1,0,1,0,0,0,0)
#define PPUTLU_975 974,976,9,487,926,1,31,353,928,1,3,7,15,15,15,(3, 5, 5, 13),(1,1,1,1,0,0,1,1,1,1)
#define PPUTLU_974 973,975,9,487,924,1,31,452,926,0,2,6,14,14,14,(2, 487),(1,1,1,1,0,0,1,1,1,0)
#define PPUTLU_973 972,974,9,486,922,1,31,553,924,1,1,5,13,13,13,(7, 139),(1,1,1,1,0,0,1,1,0,1)
#define PPUTLU_972 971,973,9,486,920,1,31,656,922,0,0,4,12,12,12,(2, 2, 3, 3, 3, 3, 3),(1,1,1,1,0,0,1,1,0,0)
#define PPUTLU_971 970,972,9,485,918,1,31,761,920,1,3,3,11,11,11,(),(1,1,1,1,0,0,1,0,1,1)
#define PPUTLU_970 969,971,9,485,916,1,31,868,918,0,2,2,10,10,10,(2, 5, 97),(1,1,1,1,0,0,1,0,1,0)
#define PPUTLU_969 968,970,9,484,914,1,31,977,916,1,1,1,9,9,9,(3, 17, 19),(1,1,1,1,0,0,1,0,0,1)
#define PPUTLU_968 967,969,9,484,912,1,31,64,915,0,0,0,8,8,8,(2, 2, 2, 11, 11),(1,1,1,1,0,0,1,0,0,0)
#define PPUTLU_967 966,968,9,483,910,1,31,177,913,1,3,7,7,7,7,(),(1,1,1,1,0,0,0,1,1,1)
#define PPUTLU_966 965,967,9,483,908,1,31,292,911,0,2,6,6,6,6,(2, 3, 7, 23),(1,1,1,1,0,0,0,1,1,0)
#define PPUTLU_965 964,966,9,482,906,1,31,409,909,1,1,5,5,5,5,(5, 193),(1,1,1,1,0,0,0,1,0,1)
#define PPUTLU_964 963,965,9,482,904,1,31,528,907,0,0,4,4,4,4,(2, 2, 241),(1,1,1,1,0,0,0,1,0,0)
#define PPUTLU_963 962,964,9,481,902,1,31,649,905,1,3,3,3,3,3,(3, 3, 107),(1,1,1,1,0,0,0,0,1,1)
#define PPUTLU_962 961,963,9,481,900,1,31,772,903,0,2,2,2,2,2,(2, 13, 37),(1,1,1,1,0,0,0,0,1,0)
#define PPUTLU_961 960,962,9,480,898,1,31,897,901,1,1,1,1,1,1,(31, 31),(1,1,1,1,0,0,0,0,0,1)
#define PPUTLU_960 959,961,9,480,896,1,30,0,900,0,0,0,0,0,0,(2, 2, 2, 2, 2, 2, 3, 5),(1,1,1,1,0,0,0,0,0,0)
#define PPUTLU_959 958,960,9,479,894,1,30,129,898,1,3,7,15,31,63,(7, 137),(1,1,1,0,1,1,1,1,1,1)
#define PPUTLU_958 957,959,9,479,892,1,30,260,896,0,2,6,14,30,62,(2, 479),(1,1,1,0,1,1,1,1,1,0)
#define PPUTLU_957 956,958,9,478,890,1,30,393,894,1,1,5,13,29,61,(3, 11, 29),(1,1,1,0,1,1,1,1,0,1)
#define PPUTLU_956 955,957,9,478,888,1,30,528,892,0,0,4,12,28,60,(2, 2, 239),(1,1,1,0,1,1,1,1,0,0)
#define PPUTLU_955 954,956,9,477,886,1,30,665,890,1,3,3,11,27,59,(5, 191),(1,1,1,0,1,1,1,0,1,1)
#define PPUTLU_954 953,955,9,477,884,1,30,804,888,0,2,2,10,26,58,(2, 3, 3, 53),(1,1,1,0,1,1,1,0,1,0)
#define PPUTLU_953 952,954,9,476,882,1,30,945,886,1,1,1,9,25,57,(),(1,1,1,0,1,1,1,0,0,1)
#define PPUTLU_952 951,953,9,476,880,1,30,64,885,0,0,0,8,24,56,(2, 2, 2, 7, 17),(1,1,1,0,1,1,1,0,0,0)
#define PPUTLU_951 950,952,9,475,878,1,30,209,883,1,3,7,7,23,55,(3, 317),(1,1,1,0,1,1,0,1,1,1)
#define PPUTLU_950 949,951,9,475,876,1,30,356,881,0,2,6,6,22,54,(2, 5, 5, 19),(1,1,1,0,1,1,0,1,1,0)
#define PPUTLU_949 948,950,9,474,874,1,30,505,879,1,1,5,5,21,53,(13, 73),(1,1,1,0,1,1,0,1,0,1)
#define PPUTLU_948 947,949,9,474,872,1,30,656,877,0,0,4,4,20,52,(2, 2, 3, 79),(1,1,1,0,1,1,0,1,0,0)
#define PPUTLU_947 946,948,9,473,870,1,30,809,875,1,3,3,3,19,51,(),(1,1,1,0,1,1,0,0,1,1)
#define PPUTLU_946 945,947,9,473,868,1,30,964,873,0,2,2,2,18,50,(2, 11, 43),(1,1,1,0,1,1,0,0,1,0)
#define PPUTLU_945 944,946,9,472,866,1,30,97,872,1,1,1,1,17,49,(3, 3, 3, 5, 7),(1,1,1,0,1,1,0,0,0,1)
#define PPUTLU_944 943,945,9,472,864,1,30,256,870,0,0,0,0,16,48,(2, 2, 2, 2, 59),(1,1,1,0,1,1,0,0,0,0)
#define PPUTLU_943 942,944,9,471,862,1,30,417,868,1,3,7,15,15,47,(23, 41),(1,1,1,0,1,0,1,1,1,1)
#define PPUTLU_942 941,943,9,471,860,1,30,580,866,0,2,6,14,14,46,(2, 3, 157),(1,1,1,0,1,0,1,1,1,0)
#define PPUTLU_941 940,942,9,470,858,1,30,745,864,1,1,5,13,13,45,(),(1,1,1,0,1,0,1,1,0,1)
#define PPUTLU_940 939,941,9,470,856,1,30,912,862,0,0,4,12,12,44,(2, 2, 5, 47),(1,1,1,0,1,0,1,1,0,0)
#define PPUTLU_939 938,940,9,469,854,1,30,57,861,1,3,3,11,11,43,(3, 313),(1,1,1,0,1,0,1,0,1,1)
#define PPUTLU_938 937,939,9,469,852,1,30,228,859,0,2,2,10,10,42,(2, 7, 67),(1,1,1,0,1,0,1,0,1,0)
#define PPUTLU_937 936,938,9,468,850,1,30,401,857,1,1,1,9,9,41,(),(1,1,1,0,1,0,1,0,0,1)
#define PPUTLU_936 935,937,9,468,848,1,30,576,855,0,0,0,8,8,40,(2, 2, 2, 3, 3, 13),(1,1,1,0,1,0,1,0,0,0)
#define PPUTLU_935 934,936,9,467,846,1,30,753,853,1,3,7,7,7,39,(5, 11, 17),(1,1,1,0,1,0,0,1,1,1)
#define PPUTLU_934 933,935,9,467,844,1,30,932,851,0,2,6,6,6,38,(2, 467),(1,1,1,0,1,0,0,1,1,0)
#define PPUTLU_933 932,934,9,466,842,1,30,89,850,1,1,5,5,5,37,(3, 311),(1,1,1,0,1,0,0,1,0,1)
#define PPUTLU_932 931,933,9,466,840,1,30,272,848,0,0,4,4,4,36,(2, 2, 233),(1,1,1,0,1,0,0,1,0,0)
#define PPUTLU_931 930,932,9,465,838,1,30,457,846,1,3,3,3,3,35,(7, 7, 19),(1,1,1,0,1,0,0,0,1,1)
#define PPUTLU_930 929,931,9,465,836,1,30,644,844,0,2,2,2,2,34,(2, 3, 5, 31),(1,1,1,0,1,0,0,0,1,0)
#define PPUTLU_929 928,930,9,464,834,1,30,833,842,1,1,1,1,1,33,(),(1,1,1,0,1,0,0,0,0,1)
#define PPUTLU_928 927,929,9,464,832,1,30,0,841,0,0,0,0,0,32,(2, 2, 2, 2, 2, 29),(1,1,1,0,1,0,0,0,0,0)
#define PPUTLU_927 926,928,9,463,830,1,30,193,839,1,3,7,15,31,31,(3, 3, 103),(1,1,1,0,0,1,1,1,1,1)
#define PPUTLU_926 925,927,9,463,828,1,30,388,837,0,2,6,14,30,30,(2, 463),(1,1,1,0,0,1,1,1,1,0)
#define PPUTLU_925 924,926,9,462,826,1,30,585,835,1,1,5,13,29,29,(5, 5, 37),(1,1,1,0,0,1,1,1,0,1)
#define PPUTLU_924 923,925,9,462,824,1,30,784,833,0,0,4,12,28,28,(2, 2, 3, 7, 11),(1,1,1,0,0,1,1,1,0,0)
#define PPUTLU_923 922,924,9,461,822,1,30,985,831,1,3,3,11,27,27,(13, 71),(1,1,1,0,0,1,1,0,1,1)
#define PPUTLU_922 921,923,9,461,820,1,30,164,830,0,2,2,10,26,26,(2, 461),(1,1,1,0,0,1,1,0,1,0)
#define PPUTLU_921 920,922,9,460,818,1,30,369,828,1,1,1,9,25,25,(3, 307),(1,1,1,0,0,1,1,0,0,1)
#define PPUTLU_920 919,921,9,460,816,1,30,576,826,0,0,0,8,24,24,(2, 2, 2, 5, 23),(1,1,1,0,0,1,1,0,0,0)
#define PPUTLU_919 918,920,9,459,814,1,30,785,824,1,3,7,7,23,23,(),(1,1,1,0,0,1,0,1,1,1)
#define PPUTLU_918 917,919,9,459,812,1,30,996,822,0,2,6,6,22,22,(2, 3, 3, 3, 17),(1,1,1,0,0,1,0,1,1,0)
#define PPUTLU_917 916,918,9,458,810,1,30,185,821,1,1,5,5,21,21,(7, 131),(1,1,1,0,0,1,0,1,0,1)
#define PPUTLU_916 915,917,9,458,808,1,30,400,819,0,0,4,4,20,20,(2, 2, 229),(1,1,1,0,0,1,0,1,0,0)
#define PPUTLU_915 914,916,9,457,806,1,30,617,817,1,3,3,3,19,19,(3, 5, 61),(1,1,1,0,0,1,0,0,1,1)
#define PPUTLU_914 913,915,9,457,804,1,30,836,815,0,2,2,2,18,18,(2, 457),(1,1,1,0,0,1,0,0,1,0)
#define PPUTLU_913 912,914,9,456,802,1,30,33,814,1,1,1,1,17,17,(11, 83),(1,1,1,0,0,1,0,0,0,1)
#define PPUTLU_912 911,913,9,456,800,1,30,256,812,0,0,0,0,16,16,(2, 2, 2, 2, 3, 19),(1,1,1,0,0,1,0,0,0,0)
#define PPUTLU_911 910,912,9,455,798,1,30,481,810,1,3,7,15,15,15,(),(1,1,1,0,0,0,1,1,1,1)
#define PPUTLU_910 909,911,9,455,796,1,30,708,808,0,2,6,14,14,14,(2, 5, 7, 13),(1,1,1,0,0,0,1,1,1,0)
#define PPUTLU_909 908,910,9,454,794,1,30,937,806,1,1,5,13,13,13,(3, 3, 101),(1,1,1,0,0,0,1,1,0,1)
#define PPUTLU_908 907,909,9,454,792,1,30,144,805,0,0,4,12,12,12,(2, 2, 227),(1,1,1,0,0,0,1,1,0,0)
#define PPUTLU_907 906,908,9,453,790,1,30,377,803,1,3,3,11,11,11,(),(1,1,1,0,0,0,1,0,1,1)
#define PPUTLU_906 905,907,9,453,788,1,30,612,801,0,2,2,10,10,10,(2, 3, 151),(1,1,1,0,0,0,1,0,1,0)
#define PPUTLU_905 904,906,9,452,786,1,30,849,799,1,1,1,9,9,9,(5, 181),(1,1,1,0,0,0,1,0,0,1)
#define PPUTLU_904 903,905,9,452,784,1,30,64,798,0,0,0,8,8,8,(2, 2, 2, 113),(1,1,1,0,0,0,1,0,0,0)
#define PPUTLU_903 902,904,9,451,782,1,30,305,796,1,3,7,7,7,7,(3, 7, 43),(1,1,1,0,0,0,0,1,1,1)
#define PPUTLU_902 901,903,9,451,780,1,30,548,794,0,2,6,6,6,6,(2, 11, 41),(1,1,1,0,0,0,0,1,1,0)
#define PPUTLU_901 900,902,9,450,778,1,30,793,792,1,1,5,5,5,5,(17, 53),(1,1,1,0,0,0,0,1,0,1)
#define PPUTLU_900 899,901,9,450,776,1,30,16,791,0,0,4,4,4,4,(2, 2, 3, 3, 5, 5),(1,1,1,0,0,0,0,1,0,0)
#define PPUTLU_899 898,900,9,449,774,1,29,265,789,1,3,3,3,3,3,(29, 31),(1,1,1,0,0,0,0,0,1,1)
#define PPUTLU_898 897,899,9,449,772,1,29,516,787,0,2,2,2,2,2,(2, 449),(1,1,1,0,0,0,0,0,1,0)
#define PPUTLU_897 896,898,9,448,770,1,29,769,785,1,1,1,1,1,1,(3, 13, 23),(1,1,1,0,0,0,0,0,0,1)
#define PPUTLU_896 895,897,9,448,768,1,29,0,784,0,0,0,0,0,0,(2, 2, 2, 2, 2, 2, 2, 7),(1,1,1,0,0,0,0,0,0,0)
#define PPUTLU_895 894,896,9,447,766,1,29,257,782,1,3,7,15,31,63,(5, 179),(1,1,0,1,1,1,1,1,1,1)
#define PPUTLU_894 893,895,9,447,764,1,29,516,780,0,2,6,14,30,62,(2, 3, 149),(1,1,0,1,1,1,1,1,1,0)
#define PPUTLU_893 892,894,9,446,762,1,29,777,778,1,1,5,13,29,61,(19, 47),(1,1,0,1,1,1,1,1,0,1)
#define PPUTLU_892 891,893,9,446,760,1,29,16,777,0,0,4,12,28,60,(2, 2, 223),(1,1,0,1,1,1,1,1,0,0)
#define PPUTLU_891 890,892,9,445,758,1,29,281,775,1,3,3,11,27,59,(3, 3, 3, 3, 11),(1,1,0,1,1,1,1,0,1,1)
#define PPUTLU_890 889,891,9,445,756,1,29,548,773,0,2,2,10,26,58,(2, 5, 89),(1,1,0,1,1,1,1,0,1,0)
#define PPUTLU_889 888,890,9,444,754,1,29,817,771,1,1,1,9,25,57,(7, 127),(1,1,0,1,1,1,1,0,0,1)
#define PPUTLU_888 887,889,9,444,752,1,29,64,770,0,0,0,8,24,56,(2, 2, 2, 3, 37),(1,1,0,1,1,1,1,0,0,0)
#define PPUTLU_887 886,888,9,443,750,1,29,337,768,1,3,7,7,23,55,(),(1,1,0,1,1,1,0,1,1,1)
#define PPUTLU_886 885,887,9,443,748,1,29,612,766,0,2,6,6,22,54,(2, 443),(1,1,0,1,1,1,0,1,1,0)
#define PPUTLU_885 884,886,9,442,746,1,29,889,764,1,1,5,5,21,53,(3, 5, 59),(1,1,0,1,1,1,0,1,0,1)
#define PPUTLU_884 883,885,9,442,744,1,29,144,763,0,0,4,4,20,52,(2, 2, 13, 17),(1,1,0,1,1,1,0,1,0,0)
#define PPUTLU_883 882,884,9,441,742,1,29,425,761,1,3,3,3,19,51,(),(1,1,0,1,1,1,0,0,1,1)
#define PPUTLU_882 881,883,9,441,740,1,29,708,759,0,2,2,2,18,50,(2, 3, 3, 7, 7),(1,1,0,1,1,1,0,0,1,0)
#define PPUTLU_881 880,882,9,440,738,1,29,993,757,1,1,1,1,17,49,(),(1,1,0,1,1,1,0,0,0,1)
#define PPUTLU_880 879,881,9,440,736,1,29,256,756,0,0,0,0,16,48,(2, 2, 2, 2, 5, 11),(1,1,0,1,1,1,0,0,0,0)
#define PPUTLU_879 878,880,9,439,734,1,29,545,754,1,3,7,15,15,47,(3, 293),(1,1,0,1,1,0,1,1,1,1)
#define PPUTLU_878 877,879,9,439,732,1,29,836,752,0,2,6,14,14,46,(2, 439),(1,1,0,1,1,0,1,1,1,0)
#define PPUTLU_877 876,878,9,438,730,1,29,105,751,1,1,5,13,13,45,(),(1,1,0,1,1,0,1,1,0,1)
#define PPUTLU_876 875,877,9,438,728,1,29,400,749,0,0,4,12,12,44,(2, 2, 3, 73),(1,1,0,1,1,0,1,1,0,0)
#define PPUTLU_875 874,876,9,437,726,1,29,697,747,1,3,3,11,11,43,(5, 5, 5, 7),(1,1,0,1,1,0,1,0,1,1)
#define PPUTLU_874 873,875,9,437,724,1,29,996,745,0,2,2,10,10,42,(2, 19, 23),(1,1,0,1,1,0,1,0,1,0)
#define PPUTLU_873 872,874,9,436,722,1,29,273,744,1,1,1,9,9,41,(3, 3, 97),(1,1,0,1,1,0,1,0,0,1)
#define PPUTLU_872 871,873,9,436,720,1,29,576,742,0,0,0,8,8,40,(2, 2, 2, 109),(1,1,0,1,1,0,1,0,0,0)
#define PPUTLU_871 870,872,9,435,718,1,29,881,740,1,3,7,7,7,39,(13, 67),(1,1,0,1,1,0,0,1,1,1)
#define PPUTLU_870 869,871,9,435,716,1,29,164,739,0,2,6,6,6,38,(2, 3, 5, 29),(1,1,0,1,1,0,0,1,1,0)
#define PPUTLU_869 868,870,9,434,714,1,29,473,737,1,1,5,5,5,37,(11, 79),(1,1,0,1,1,0,0,1,0,1)
#define PPUTLU_868 867,869,9,434,712,1,29,784,735,0,0,4,4,4,36,(2, 2, 7, 31),(1,1,0,1,1,0,0,1,0,0)
#define PPUTLU_867 866,868,9,433,710,1,29,73,734,1,3,3,3,3,35,(3, 17, 17),(1,1,0,1,1,0,0,0,1,1)
#define PPUTLU_866 865,867,9,433,708,1,29,388,732,0,2,2,2,2,34,(2, 433),(1,1,0,1,1,0,0,0,1,0)
#define PPUTLU_865 864,866,9,432,706,1,29,705,730,1,1,1,1,1,33,(5, 173),(1,1,0,1,1,0,0,0,0,1)
#define PPUTLU_864 863,865,9,432,704,1,29,0,729,0,0,0,0,0,32,(2, 2, 2, 2, 2, 3, 3, 3),(1,1,0,1,1,0,0,0,0,0)
#define PPUTLU_863 862,864,9,431,702,1,29,321,727,1,3,7,15,31,31,(),(1,1,0,1,0,1,1,1,1,1)
#define PPUTLU_862 861,863,9,431,700,1,29,644,725,0,2,6,14,30,30,(2, 431),(1,1,0,1,0,1,1,1,1,0)
#define PPUTLU_861 860,862,9,430,698,1,29,969,723,1,1,5,13,29,29,(3, 7, 41),(1,1,0,1,0,1,1,1,0,1)
#define PPUTLU_860 859,861,9,430,696,1,29,272,722,0,0,4,12,28,28,(2, 2, 5, 43),(1,1,0,1,0,1,1,1,0,0)
#define PPUTLU_859 858,860,9,429,694,1,29,601,720,1,3,3,11,27,27,(),(1,1,0,1,0,1,1,0,1,1)
#define PPUTLU_858 857,859,9,429,692,1,29,932,718,0,2,2,10,26,26,(2, 3, 11, 13),(1,1,0,1,0,1,1,0,1,0)
#define PPUTLU_857 856,858,9,428,690,1,29,241,717,1,1,1,9,25,25,(),(1,1,0,1,0,1,1,0,0,1)
#define PPUTLU_856 855,857,9,428,688,1,29,576,715,0,0,0,8,24,24,(2, 2, 2, 107),(1,1,0,1,0,1,1,0,0,0)
#define PPUTLU_855 854,856,9,427,686,1,29,913,713,1,3,7,7,23,23,(3, 3, 5, 19),(1,1,0,1,0,1,0,1,1,1)
#define PPUTLU_854 853,855,9,427,684,1,29,228,712,0,2,6,6,22,22,(2, 7, 61),(1,1,0,1,0,1,0,1,1,0)
#define PPUTLU_853 852,854,9,426,682,1,29,569,710,1,1,5,5,21,21,(),(1,1,0,1,0,1,0,1,0,1)
#define PPUTLU_852 851,853,9,426,680,1,29,912,708,0,0,4,4,20,20,(2, 2, 3, 71),(1,1,0,1,0,1,0,1,0,0)
#define PPUTLU_851 850,852,9,425,678,1,29,233,707,1,3,3,3,19,19,(23, 37),(1,1,0,1,0,1,0,0,1,1)
#define PPUTLU_850 849,851,9,425,676,1,29,580,705,0,2,2,2,18,18,(2, 5, 5, 17),(1,1,0,1,0,1,0,0,1,0)
#define PPUTLU_849 848,850,9,424,674,1,29,929,703,1,1,1,1,17,17,(3, 283),(1,1,0,1,0,1,0,0,0,1)
#define PPUTLU_848 847,849,9,424,672,1,29,256,702,0,0,0,0,16,16,(2, 2, 2, 2, 53),(1,1,0,1,0,1,0,0,0,0)
#define PPUTLU_847 846,848,9,423,670,1,29,609,700,1,3,7,15,15,15,(7, 11, 11),(1,1,0,1,0,0,1,1,1,1)
#define PPUTLU_846 845,847,9,423,668,1,29,964,698,0,2,6,14,14,14,(2, 3, 3, 47),(1,1,0,1,0,0,1,1,1,0)
#define PPUTLU_845 844,846,9,422,666,1,29,297,697,1,1,5,13,13,13,(5, 13, 13),(1,1,0,1,0,0,1,1,0,1)
#define PPUTLU_844 843,845,9,422,664,1,29,656,695,0,0,4,12,12,12,(2, 2, 211),(1,1,0,1,0,0,1,1,0,0)
#define PPUTLU_843 842,844,9,421,662,1,29,1017,693,1,3,3,11,11,11,(3, 281),(1,1,0,1,0,0,1,0,1,1)
#define PPUTLU_842 841,843,9,421,660,1,29,356,692,0,2,2,10,10,10,(2, 421),(1,1,0,1,0,0,1,0,1,0)
#define PPUTLU_841 840,842,9,420,658,1,29,721,690,1,1,1,9,9,9,(29, 29),(1,1,0,1,0,0,1,0,0,1)
#define PPUTLU_840 839,841,9,420,656,1,28,64,689,0,0,0,8,8,8,(2, 2, 2, 3, 5, 7),(1,1,0,1,0,0,1,0,0,0)
#define PPUTLU_839 838,840,9,419,654,1,28,433,687,1,3,7,7,7,7,(),(1,1,0,1,0,0,0,1,1,1)
#define PPUTLU_838 837,839,9,419,652,1,28,804,685,0,2,6,6,6,6,(2, 419),(1,1,0,1,0,0,0,1,1,0)
#define PPUTLU_837 836,838,9,418,650,1,28,153,684,1,1,5,5,5,5,(3, 3, 3, 31),(1,1,0,1,0,0,0,1,0,1)
#define PPUTLU_836 835,837,9,418,648,1,28,528,682,0,0,4,4,4,4,(2, 2, 11, 19),(1,1,0,1,0,0,0,1,0,0)
#define PPUTLU_835 834,836,9,417,646,1,28,905,680,1,3,3,3,3,3,(5, 167),(1,1,0,1,0,0,0,0,1,1)
#define PPUTLU_834 833,835,9,417,644,1,28,260,679,0,2,2,2,2,2,(2, 3, 139),(1,1,0,1,0,0,0,0,1,0)
#define PPUTLU_833 832,834,9,416,642,1,28,641,677,1,1,1,1,1,1,(7, 7, 17),(1,1,0,1,0,0,0,0,0,1)
#define PPUTLU_832 831,833,9,416,640,1,28,0,676,0,0,0,0,0,0,(2, 2, 2, 2, 2, 2, 13),(1,1,0,1,0,0,0,0,0,0)
#define PPUTLU_831 830,832,9,415,638,1,28,385,674,1,3,7,15,31,63,(3, 277),(1,1,0,0,1,1,1,1,1,1)
#define PPUTLU_830 829,831,9,415,636,1,28,772,672,0,2,6,14,30,62,(2, 5, 83),(1,1,0,0,1,1,1,1,1,0)
#define PPUTLU_829 828,830,9,414,634,1,28,137,671,1,1,5,13,29,61,(),(1,1,0,0,1,1,1,1,0,1)
#define PPUTLU_828 827,829,9,414,632,1,28,528,669,0,0,4,12,28,60,(2, 2, 3, 3, 23),(1,1,0,0,1,1,1,1,0,0)
#define PPUTLU_827 826,828,9,413,630,1,28,921,667,1,3,3,11,27,59,(),(1,1,0,0,1,1,1,0,1,1)
#define PPUTLU_826 825,827,9,413,628,1,28,292,666,0,2,2,10,26,58,(2, 7, 59),(1,1,0,0,1,1,1,0,1,0)
#define PPUTLU_825 824,826,9,412,626,1,28,689,664,1,1,1,9,25,57,(3, 5, 5, 11),(1,1,0,0,1,1,1,0,0,1)
#define PPUTLU_824 823,825,9,412,624,1,28,64,663,0,0,0,8,24,56,(2, 2, 2, 103),(1,1,0,0,1,1,1,0,0,0)
#define PPUTLU_823 822,824,9,411,622,1,28,465,661,1,3,7,7,23,55,(),(1,1,0,0,1,1,0,1,1,1)
#define PPUTLU_822 821,823,9,411,620,1,28,868,659,0,2,6,6,22,54,(2, 3, 137),(1,1,0,0,1,1,0,1,1,0)
#define PPUTLU_821 820,822,9,410,618,1,28,249,658,1,1,5,5,21,53,(),(1,1,0,0,1,1,0,1,0,1)
#define PPUTLU_820 819,821,9,410,616,1,28,656,656,0,0,4,4,20,52,(2, 2, 5, 41),(1,1,0,0,1,1,0,1,0,0)
#define PPUTLU_819 818,820,9,409,614,1,28,41,655,1,3,3,3,19,51,(3, 3, 7, 13),(1,1,0,0,1,1,0,0,1,1)
#define PPUTLU_818 817,819,9,409,612,1,28,452,653,0,2,2,2,18,50,(2, 409),(1,1,0,0,1,1,0,0,1,0)
#define PPUTLU_817 816,818,9,408,610,1,28,865,651,1,1,1,1,17,49,(19, 43),(1,1,0,0,1,1,0,0,0,1)
#define PPUTLU_816 815,817,9,408,608,1,28,256,650,0,0,0,0,16,48,(2, 2, 2, 2, 3, 17),(1,1,0,0,1,1,0,0,0,0)
#define PPUTLU_815 814,816,9,407,606,1,28,673,648,1,3,7,15,15,47,(5, 163),(1,1,0,0,1,0,1,1,1,1)
#define PPUTLU_814 813,815,9,407,604,1,28,68,647,0,2,6,14,14,46,(2, 11, 37),(1,1,0,0,1,0,1,1,1,0)
#define PPUTLU_813 812,814,9,406,602,1,28,489,645,1,1,5,13,13,45,(3, 271),(1,1,0,0,1,0,1,1,0,1)
#define PPUTLU_812 811,813,9,406,600,1,28,912,643,0,0,4,12,12,44,(2, 2, 7, 29),(1,1,0,0,1,0,1,1,0,0)
#define PPUTLU_811 810,812,9,405,598,1,28,313,642,1,3,3,11,11,43,(),(1,1,0,0,1,0,1,0,1,1)
#define PPUTLU_810 809,811,9,405,596,1,28,740,640,0,2,2,10,10,42,(2, 3, 3, 3, 3, 5),(1,1,0,0,1,0,1,0,1,0)
#define PPUTLU_809 808,810,9,404,594,1,28,145,639,1,1,1,9,9,41,(),(1,1,0,0,1,0,1,0,0,1)
#define PPUTLU_808 807,809,9,404,592,1,28,576,637,0,0,0,8,8,40,(2, 2, 2, 101),(1,1,0,0,1,0,1,0,0,0)
#define PPUTLU_807 806,808,9,403,590,1,28,1009,635,1,3,7,7,7,39,(3, 269),(1,1,0,0,1,0,0,1,1,1)
#define PPUTLU_806 805,807,9,403,588,1,28,420,634,0,2,6,6,6,38,(2, 13, 31),(1,1,0,0,1,0,0,1,1,0)
#define PPUTLU_805 804,806,9,402,586,1,28,857,632,1,1,5,5,5,37,(5, 7, 23),(1,1,0,0,1,0,0,1,0,1)
#define PPUTLU_804 803,805,9,402,584,1,28,272,631,0,0,4,4,4,36,(2, 2, 3, 67),(1,1,0,0,1,0,0,1,0,0)
#define PPUTLU_803 802,804,9,401,582,1,28,713,629,1,3,3,3,3,35,(11, 73),(1,1,0,0,1,0,0,0,1,1)
#define PPUTLU_802 801,803,9,401,580,1,28,132,628,0,2,2,2,2,34,(2, 401),(1,1,0,0,1,0,0,0,1,0)
#define PPUTLU_801 800,802,9,400,578,1,28,577,626,1,1,1,1,1,33,(3, 3, 89),(1,1,0,0,1,0,0,0,0,1)
#define PPUTLU_800 799,801,9,400,576,1,28,0,625,0,0,0,0,0,32,(2, 2, 2, 2, 2, 5, 5),(1,1,0,0,1,0,0,0,0,0)
#define PPUTLU_799 798,800,9,399,574,1,28,449,623,1,3,7,15,31,31,(17, 47),(1,1,0,0,0,1,1,1,1,1)
#define PPUTLU_798 797,799,9,399,572,1,28,900,621,0,2,6,14,30,30,(2, 3, 7, 19),(1,1,0,0,0,1,1,1,1,0)
#define PPUTLU_797 796,798,9,398,570,1,28,329,620,1,1,5,13,29,29,(),(1,1,0,0,0,1,1,1,0,1)
#define PPUTLU_796 795,797,9,398,568,1,28,784,618,0,0,4,12,28,28,(2, 2, 199),(1,1,0,0,0,1,1,1,0,0)
#define PPUTLU_795 794,796,9,397,566,1,28,217,617,1,3,3,11,27,27,(3, 5, 53),(1,1,0,0,0,1,1,0,1,1)
#define PPUTLU_794 793,795,9,397,564,1,28,676,615,0,2,2,10,26,26,(2, 397),(1,1,0,0,0,1,1,0,1,0)
#define PPUTLU_793 792,794,9,396,562,1,28,113,614,1,1,1,9,25,25,(13, 61),(1,1,0,0,0,1,1,0,0,1)
#define PPUTLU_792 791,793,9,396,560,1,28,576,612,0,0,0,8,24,24,(2, 2, 2, 3, 3, 11),(1,1,0,0,0,1,1,0,0,0)
#define PPUTLU_791 790,792,9,395,558,1,28,17,611,1,3,7,7,23,23,(7, 113),(1,1,0,0,0,1,0,1,1,1)
#define PPUTLU_790 789,791,9,395,556,1,28,484,609,0,2,6,6,22,22,(2, 5, 79),(1,1,0,0,0,1,0,1,1,0)
#define PPUTLU_789 788,790,9,394,554,1,28,953,607,1,1,5,5,21,21,(3, 263),(1,1,0,0,0,1,0,1,0,1)
#define PPUTLU_788 787,789,9,394,552,1,28,400,606,0,0,4,4,20,20,(2, 2, 197),(1,1,0,0,0,1,0,1,0,0)
#define PPUTLU_787 786,788,9,393,550,1,28,873,604,1,3,3,3,19,19,(),(1,1,0,0,0,1,0,0,1,1)
#define PPUTLU_786 785,787,9,393,548,1,28,324,603,0,2,2,2,18,18,(2, 3, 131),(1,1,0,0,0,1,0,0,1,0)
#define PPUTLU_785 784,786,9,392,546,1,28,801,601,1,1,1,1,17,17,(5, 157),(1,1,0,0,0,1,0,0,0,1)
#define PPUTLU_784 783,785,9,392,544,1,28,256,600,0,0,0,0,16,16,(2, 2, 2, 2, 7, 7),(1,1,0,0,0,1,0,0,0,0)
#define PPUTLU_783 782,784,9,391,542,1,27,737,598,1,3,7,15,15,15,(3, 3, 3, 29),(1,1,0,0,0,0,1,1,1,1)
#define PPUTLU_782 781,783,9,391,540,1,27,196,597,0,2,6,14,14,14,(2, 17, 23),(1,1,0,0,0,0,1,1,1,0)
#define PPUTLU_781 780,782,9,390,538,1,27,681,595,1,1,5,13,13,13,(11, 71),(1,1,0,0,0,0,1,1,0,1)
#define PPUTLU_780 779,781,9,390,536,1,27,144,594,0,0,4,12,12,12,(2, 2, 3, 5, 13),(1,1,0,0,0,0,1,1,0,0)
#define PPUTLU_779 778,780,9,389,534,1,27,633,592,1,3,3,11,11,11,(19, 41),(1,1,0,0,0,0,1,0,1,1)
#define PPUTLU_778 777,779,9,389,532,1,27,100,591,0,2,2,10,10,10,(2, 389),(1,1,0,0,0,0,1,0,1,0)
#define PPUTLU_777 776,778,9,388,530,1,27,593,589,1,1,1,9,9,9,(3, 7, 37),(1,1,0,0,0,0,1,0,0,1)
#define PPUTLU_776 775,777,9,388,528,1,27,64,588,0,0,0,8,8,8,(2, 2, 2, 97),(1,1,0,0,0,0,1,0,0,0)
#define PPUTLU_775 774,776,9,387,526,1,27,561,586,1,3,7,7,7,7,(5, 5, 31),(1,1,0,0,0,0,0,1,1,1)
#define PPUTLU_774 773,775,9,387,524,1,27,36,585,0,2,6,6,6,6,(2, 3, 3, 43),(1,1,0,0,0,0,0,1,1,0)
#define PPUTLU_773 772,774,9,386,522,1,27,537,583,1,1,5,5,5,5,(),(1,1,0,0,0,0,0,1,0,1)
#define PPUTLU_772 771,773,9,386,520,1,27,16,582,0,0,4,4,4,4,(2, 2, 193),(1,1,0,0,0,0,0,1,0,0)
#define PPUTLU_771 770,772,9,385,518,1,27,521,580,1,3,3,3,3,3,(3, 257),(1,1,0,0,0,0,0,0,1,1)
#define PPUTLU_770 769,771,9,385,516,1,27,4,579,0,2,2,2,2,2,(2, 5, 7, 11),(1,1,0,0,0,0,0,0,1,0)
#define PPUTLU_769 768,770,9,384,514,1,27,513,577,1,1,1,1,1,1,(),(1,1,0,0,0,0,0,0,0,1)
#define PPUTLU_768 767,769,9,384,512,1,27,0,576,0,0,0,0,0,0,(2, 2, 2, 2, 2, 2, 2, 2, 3),(1,1,0,0,0,0,0,0,0,0)
#define PPUTLU_767 766,768,9,383,510,1,27,513,574,1,3,7,15,31,63,(13, 59),(1,0,1,1,1,1,1,1,1,1)
#define PPUTLU_766 765,767,9,383,508,1,27,4,573,0,2,6,14,30,62,(2, 383),(1,0,1,1,1,1,1,1,1,0)
#define PPUTLU_765 764,766,9,382,506,1,27,521,571,1,1,5,13,29,61,(3, 3, 5, 17),(1,0,1,1,1,1,1,1,0,1)
#define PPUTLU_764 763,765,9,382,504,1,27,16,570,0,0,4,12,28,60,(2, 2, 191),(1,0,1,1,1,1,1,1,0,0)
#define PPUTLU_763 762,764,9,381,502,1,27,537,568,1,3,3,11,27,59,(7, 109),(1,0,1,1,1,1,1,0,1,1)
#define PPUTLU_762 761,763,9,381,500,1,27,36,567,0,2,2,10,26,58,(2, 3, 127),(1,0,1,1,1,1,1,0,1,0)
#define PPUTLU_761 760,762,9,380,498,1,27,561,565,1,1,1,9,25,57,(),(1,0,1,1,1,1,1,0,0,1)
#define PPUTLU_760 759,761,9,380,496,1,27,64,564,0,0,0,8,24,56,(2, 2, 2, 5, 19),(1,0,1,1,1,1,1,0,0,0)
#define PPUTLU_759 758,760,9,379,494,1,27,593,562,1,3,7,7,23,55,(3, 11, 23),(1,0,1,1,1,1,0,1,1,1)
#define PPUTLU_758 757,759,9,379,492,1,27,100,561,0,2,6,6,22,54,(2, 379),(1,0,1,1,1,1,0,1,1,0)
#define PPUTLU_757 756,758,9,378,490,1,27,633,559,1,1,5,5,21,53,(),(1,0,1,1,1,1,0,1,0,1)
#define PPUTLU_756 755,757,9,378,488,1,27,144,558,0,0,4,4,20,52,(2, 2, 3, 3, 3, 7),(1,0,1,1,1,1,0,1,0,0)
#define PPUTLU_755 754,756,9,377,486,1,27,681,556,1,3,3,3,19,51,(5, 151),(1,0,1,1,1,1,0,0,1,1)
#define PPUTLU_754 753,755,9,377,484,1,27,196,555,0,2,2,2,18,50,(2, 13, 29),(1,0,1,1,1,1,0,0,1,0)
#define PPUTLU_753 752,754,9,376,482,1,27,737,553,1,1,1,1,17,49,(3, 251),(1,0,1,1,1,1,0,0,0,1)
#define PPUTLU_752 751,753,9,376,480,1,27,256,552,0,0,0,0,16,48,(2, 2, 2, 2, 47),(1,0,1,1,1,1,0,0,0,0)
#define PPUTLU_751 750,752,9,375,478,1,27,801,550,1,3,7,15,15,47,(),(1,0,1,1,1,0,1,1,1,1)
#define PPUTLU_750 749,751,9,375,476,1,27,324,549,0,2,6,14,14,46,(2, 3, 5, 5, 5),(1,0,1,1,1,0,1,1,1,0)
#define PPUTLU_749 748,750,9,374,474,1,27,873,547,1,1,5,13,13,45,(7, 107),(1,0,1,1,1,0,1,1,0,1)
#define PPUTLU_748 747,749,9,374,472,1,27,400,546,0,0,4,12,12,44,(2, 2, 11, 17),(1,0,1,1,1,0,1,1,0,0)
#define PPUTLU_747 746,748,9,373,470,1,27,953,544,1,3,3,11,11,43,(3, 3, 83),(1,0,1,1,1,0,1,0,1,1)
#define PPUTLU_746 745,747,9,373,468,1,27,484,543,0,2,2,10,10,42,(2, 373),(1,0,1,1,1,0,1,0,1,0)
#define PPUTLU_745 744,746,9,372,466,1,27,17,542,1,1,1,9,9,41,(5, 149),(1,0,1,1,1,0,1,0,0,1)
#define PPUTLU_744 743,745,9,372,464,1,27,576,540,0,0,0,8,8,40,(2, 2, 2, 3, 31),(1,0,1,1,1,0,1,0,0,0)
#define PPUTLU_743 742,744,9,371,462,1,27,113,539,1,3,7,7,7,39,(),(1,0,1,1,1,0,0,1,1,1)
#define PPUTLU_742 741,743,9,371,460,1,27,676,537,0,2,6,6,6,38,(2, 7, 53),(1,0,1,1,1,0,0,1,1,0)
#define PPUTLU_741 740,742,9,370,458,1,27,217,536,1,1,5,5,5,37,(3, 13, 19),(1,0,1,1,1,0,0,1,0,1)
#define PPUTLU_740 739,741,9,370,456,1,27,784,534,0,0,4,4,4,36,(2, 2, 5, 37),(1,0,1,1,1,0,0,1,0,0)
#define PPUTLU_739 738,740,9,369,454,1,27,329,533,1,3,3,3,3,35,(),(1,0,1,1,1,0,0,0,1,1)
#define PPUTLU_738 737,739,9,369,452,1,27,900,531,0,2,2,2,2,34,(2, 3, 3, 41),(1,0,1,1,1,0,0,0,1,0)
#define PPUTLU_737 736,738,9,368,450,1,27,449,530,1,1,1,1,1,33,(11, 67),(1,0,1,1,1,0,0,0,0,1)
#define PPUTLU_736 735,737,9,368,448,1,27,0,529,0,0,0,0,0,32,(2, 2, 2, 2, 2, 23),(1,0,1,1,1,0,0,0,0,0)
#define PPUTLU_735 734,736,9,367,446,1,27,577,527,1,3,7,15,31,31,(3, 5, 7, 7),(1,0,1,1,0,1,1,1,1,1)
#define PPUTLU_734 733,735,9,367,444,1,27,132,526,0,2,6,14,30,30,(2, 367),(1,0,1,1,0,1,1,1,1,0)
#define PPUTLU_733 732,734,9,366,442,1,27,713,524,1,1,5,13,29,29,(),(1,0,1,1,0,1,1,1,0,1)
#define PPUTLU_732 731,733,9,366,440,1,27,272,523,0,0,4,12,28,28,(2, 2, 3, 61),(1,0,1,1,0,1,1,1,0,0)
#define PPUTLU_731 730,732,9,365,438,1,27,857,521,1,3,3,11,27,27,(17, 43),(1,0,1,1,0,1,1,0,1,1)
#define PPUTLU_730 729,731,9,365,436,1,27,420,520,0,2,2,10,26,26,(2, 5, 73),(1,0,1,1,0,1,1,0,1,0)
#define PPUTLU_729 728,730,9,364,434,1,27,1009,518,1,1,1,9,25,25,(3, 3, 3, 3, 3, 3),(1,0,1,1,0,1,1,0,0,1)
#define PPUTLU_728 727,729,9,364,432,1,26,576,517,0,0,0,8,24,24,(2, 2, 2, 7, 13),(1,0,1,1,0,1,1,0,0,0)
#define PPUTLU_727 726,728,9,363,430,1,26,145,516,1,3,7,7,23,23,(),(1,0,1,1,0,1,0,1,1,1)
#define PPUTLU_726 725,727,9,363,428,1,26,740,514,0,2,6,6,22,22,(2, 3, 11, 11),(1,0,1,1,0,1,0,1,1,0)
#define PPUTLU_725 724,726,9,362,426,1,26,313,513,1,1,5,5,21,21,(5, 5, 29),(1,0,1,1,0,1,0,1,0,1)
#define PPUTLU_724 723,725,9,362,424,1,26,912,511,0,0,4,4,20,20,(2, 2, 181),(1,0,1,1,0,1,0,1,0,0)
#define PPUTLU_723 722,724,9,361,422,1,26,489,510,1,3,3,3,19,19,(3, 241),(1,0,1,1,0,1,0,0,1,1)
#define PPUTLU_722 721,723,9,361,420,1,26,68,509,0,2,2,2,18,18,(2, 19, 19),(1,0,1,1,0,1,0,0,1,0)
#define PPUTLU_721 720,722,9,360,418,1,26,673,507,1,1,1,1,17,17,(7, 103),(1,0,1,1,0,1,0,0,0,1)
#define PPUTLU_720 719,721,9,360,416,1,26,256,506,0,0,0,0,16,16,(2, 2, 2, 2, 3, 3, 5),(1,0,1,1,0,1,0,0,0,0)
#define PPUTLU_719 718,720,9,359,414,1,26,865,504,1,3,7,15,15,15,(),(1,0,1,1,0,0,1,1,1,1)
#define PPUTLU_718 717,719,9,359,412,1,26,452,503,0,2,6,14,14,14,(2, 359),(1,0,1,1,0,0,1,1,1,0)
#define PPUTLU_717 716,718,9,358,410,1,26,41,502,1,1,5,13,13,13,(3, 239),(1,0,1,1,0,0,1,1,0,1)
#define PPUTLU_716 715,717,9,358,408,1,26,656,500,0,0,4,12,12,12,(2, 2, 179),(1,0,1,1,0,0,1,1,0,0)
#define PPUTLU_715 714,716,9,357,406,1,26,249,499,1,3,3,11,11,11,(5, 11, 13),(1,0,1,1,0,0,1,0,1,1)
#define PPUTLU_714 713,715,9,357,404,1,26,868,497,0,2,2,10,10,10,(2, 3, 7, 17),(1,0,1,1,0,0,1,0,1,0)
#define PPUTLU_713 712,714,9,356,402,1,26,465,496,1,1,1,9,9,9,(23, 31),(1,0,1,1,0,0,1,0,0,1)
#define PPUTLU_712 711,713,9,356,400,1,26,64,495,0,0,0,8,8,8,(2, 2, 2, 89),(1,0,1,1,0,0,1,0,0,0)
#define PPUTLU_711 710,712,9,355,398,1,26,689,493,1,3,7,7,7,7,(3, 3, 79),(1,0,1,1,0,0,0,1,1,1)
#define PPUTLU_710 709,711,9,355,396,1,26,292,492,0,2,6,6,6,6,(2, 5, 71),(1,0,1,1,0,0,0,1,1,0)
#define PPUTLU_709 708,710,9,354,394,1,26,921,490,1,1,5,5,5,5,(),(1,0,1,1,0,0,0,1,0,1)
#define PPUTLU_708 707,709,9,354,392,1,26,528,489,0,0,4,4,4,4,(2, 2, 3, 59),(1,0,1,1,0,0,0,1,0,0)
#define PPUTLU_707 706,708,9,353,390,1,26,137,488,1,3,3,3,3,3,(7, 101),(1,0,1,1,0,0,0,0,1,1)
#define PPUTLU_706 705,707,9,353,388,1,26,772,486,0,2,2,2,2,2,(2, 353),(1,0,1,1,0,0,0,0,1,0)
#define PPUTLU_705 704,706,9,352,386,1,26,385,485,1,1,1,1,1,1,(3, 5, 47),(1,0,1,1,0,0,0,0,0,1)
#define PPUTLU_704 703,705,9,352,384,1,26,0,484,0,0,0,0,0,0,(2, 2, 2, 2, 2, 2, 11),(1,0,1,1,0,0,0,0,0,0)
#define PPUTLU_703 702,704,9,351,382,1,26,641,482,1,3,7,15,31,63,(19, 37),(1,0,1,0,1,1,1,1,1,1)
#define PPUTLU_702 701,703,9,351,380,1,26,260,481,0,2,6,14,30,62,(2, 3, 3, 3, 13),(1,0,1,0,1,1,1,1,1,0)
#define PPUTLU_701 700,702,9,350,378,1,26,905,479,1,1,5,13,29,61,(),(1,0,1,0,1,1,1,1,0,1)
#define PPUTLU_700 699,701,9,350,376,1,26,528,478,0,0,4,12,28,60,(2, 2, 5, 5, 7),(1,0,1,0,1,1,1,1,0,0)
#define PPUTLU_699 698,700,9,349,374,1,26,153,477,1,3,3,11,27,59,(3, 233),(1,0,1,0,1,1,1,0,1,1)
#define PPUTLU_698 697,699,9,349,372,1,26,804,475,0,2,2,10,26,58,(2, 349),(1,0,1,0,1,1,1,0,1,0)
#define PPUTLU_697 696,698,9,348,370,1,26,433,474,1,1,1,9,25,57,(17, 41),(1,0,1,0,1,1,1,0,0,1)
#define PPUTLU_696 695,697,9,348,368,1,26,64,473,0,0,0,8,24,56,(2, 2, 2, 3, 29),(1,0,1,0,1,1,1,0,0,0)
#define PPUTLU_695 694,696,9,347,366,1,26,721,471,1,3,7,7,23,55,(5, 139),(1,0,1,0,1,1,0,1,1,1)
#define PPUTLU_694 693,695,9,347,364,1,26,356,470,0,2,6,6,22,54,(2, 347),(1,0,1,0,1,1,0,1,1,0)
#define PPUTLU_693 692,694,9,346,362,1,26,1017,468,1,1,5,5,21,53,(3, 3, 7, 11),(1,0,1,0,1,1,0,1,0,1)
#define PPUTLU_692 691,693,9,346,360,1,26,656,467,0,0,4,4,20,52,(2, 2, 173),(1,0,1,0,1,1,0,1,0,0)
#define PPUTLU_691 690,692,9,345,358,1,26,297,466,1,3,3,3,19,51,(),(1,0,1,0,1,1,0,0,1,1)
#define PPUTLU_690 689,691,9,345,356,1,26,964,464,0,2,2,2,18,50,(2, 3, 5, 23),(1,0,1,0,1,1,0,0,1,0)
#define PPUTLU_689 688,690,9,344,354,1,26,609,463,1,1,1,1,17,49,(13, 53),(1,0,1,0,1,1,0,0,0,1)
#define PPUTLU_688 687,689,9,344,352,1,26,256,462,0,0,0,0,16,48,(2, 2, 2, 2, 43),(1,0,1,0,1,1,0,0,0,0)
#define PPUTLU_687 686,688,9,343,350,1,26,929,460,1,3,7,15,15,47,(3, 229),(1,0,1,0,1,0,1,1,1,1)
#define PPUTLU_686 685,687,9,343,348,1,26,580,459,0,2,6,14,14,46,(2, 7, 7, 7),(1,0,1,0,1,0,1,1,1,0)
#define PPUTLU_685 684,686,9,342,346,1,26,233,458,1,1,5,13,13,45,(5, 137),(1,0,1,0,1,0,1,1,0,1)
#define PPUTLU_684 683,685,9,342,344,1,26,912,456,0,0,4,12,12,44,(2, 2, 3, 3, 19),(1,0,1,0,1,0,1,1,0,0)
#define PPUTLU_683 682,684,9,341,342,1,26,569,455,1,3,3,11,11,43,(),(1,0,1,0,1,0,1,0,1,1)
#define PPUTLU_682 681,683,9,341,340,1,26,228,454,0,2,2,10,10,42,(2, 11, 31),(1,0,1,0,1,0,1,0,1,0)
#define PPUTLU_681 680,682,9,340,338,1,26,913,452,1,1,1,9,9,41,(3, 227),(1,0,1,0,1,0,1,0,0,1)
#define PPUTLU_680 679,681,9,340,336,1,26,576,451,0,0,0,8,8,40,(2, 2, 2, 5, 17),(1,0,1,0,1,0,1,0,0,0)
#define PPUTLU_679 678,680,9,339,334,1,26,241,450,1,3,7,7,7,39,(7, 97),(1,0,1,0,1,0,0,1,1,1)
#define PPUTLU_678 677,679,9,339,332,1,26,932,448,0,2,6,6,6,38,(2, 3, 113),(1,0,1,0,1,0,0,1,1,0)
#define PPUTLU_677 676,678,9,338,330,1,26,601,447,1,1,5,5,5,37,(),(1,0,1,0,1,0,0,1,0,1)
#define PPUTLU_676 675,677,9,338,328,1,26,272,446,0,0,4,4,4,36,(2, 2, 13, 13),(1,0,1,0,1,0,0,1,0,0)
#define PPUTLU_675 674,676,9,337,326,1,25,969,444,1,3,3,3,3,35,(3, 3, 3, 5, 5),(1,0,1,0,1,0,0,0,1,1)
#define PPUTLU_674 673,675,9,337,324,1,25,644,443,0,2,2,2,2,34,(2, 337),(1,0,1,0,1,0,0,0,1,0)
#define PPUTLU_673 672,674,9,336,322,1,25,321,442,1,1,1,1,1,33,(),(1,0,1,0,1,0,0,0,0,1)
#define PPUTLU_672 671,673,9,336,320,1,25,0,441,0,0,0,0,0,32,(2, 2, 2, 2, 2, 3, 7),(1,0,1,0,1,0,0,0,0,0)
#define PPUTLU_671 670,672,9,335,318,1,25,705,439,1,3,7,15,31,31,(11, 61),(1,0,1,0,0,1,1,1,1,1)
#define PPUTLU_670 669,671,9,335,316,1,25,388,438,0,2,6,14,30,30,(2, 5, 67),(1,0,1,0,0,1,1,1,1,0)
#define PPUTLU_669 668,670,9,334,314,1,25,73,437,1,1,5,13,29,29,(3, 223),(1,0,1,0,0,1,1,1,0,1)
#define PPUTLU_668 667,669,9,334,312,1,25,784,435,0,0,4,12,28,28,(2, 2, 167),(1,0,1,0,0,1,1,1,0,0)
#define PPUTLU_667 666,668,9,333,310,1,25,473,434,1,3,3,11,27,27,(23, 29),(1,0,1,0,0,1,1,0,1,1)
#define PPUTLU_666 665,667,9,333,308,1,25,164,433,0,2,2,10,26,26,(2, 3, 3, 37),(1,0,1,0,0,1,1,0,1,0)
#define PPUTLU_665 664,666,9,332,306,1,25,881,431,1,1,1,9,25,25,(5, 7, 19),(1,0,1,0,0,1,1,0,0,1)
#define PPUTLU_664 663,665,9,332,304,1,25,576,430,0,0,0,8,24,24,(2, 2, 2, 83),(1,0,1,0,0,1,1,0,0,0)
#define PPUTLU_663 662,664,9,331,302,1,25,273,429,1,3,7,7,23,23,(3, 13, 17),(1,0,1,0,0,1,0,1,1,1)
#define PPUTLU_662 661,663,9,331,300,1,25,996,427,0,2,6,6,22,22,(2, 331),(1,0,1,0,0,1,0,1,1,0)
#define PPUTLU_661 660,662,9,330,298,1,25,697,426,1,1,5,5,21,21,(),(1,0,1,0,0,1,0,1,0,1)
#define PPUTLU_660 659,661,9,330,296,1,25,400,425,0,0,4,4,20,20,(2, 2, 3, 5, 11),(1,0,1,0,0,1,0,1,0,0)
#define PPUTLU_659 658,660,9,329,294,1,25,105,424,1,3,3,3,19,19,(),(1,0,1,0,0,1,0,0,1,1)
#define PPUTLU_658 657,659,9,329,292,1,25,836,422,0,2,2,2,18,18,(2, 7, 47),(1,0,1,0,0,1,0,0,1,0)
#define PPUTLU_657 656,658,9,328,290,1,25,545,421,1,1,1,1,17,17,(3, 3, 73),(1,0,1,0,0,1,0,0,0,1)
#define PPUTLU_656 655,657,9,328,288,1,25,256,420,0,0,0,0,16,16,(2, 2, 2, 2, 41),(1,0,1,0,0,1,0,0,0,0)
#define PPUTLU_655 654,656,9,327,286,1,25,993,418,1,3,7,15,15,15,(5, 131),(1,0,1,0,0,0,1,1,1,1)
#define PPUTLU_654 653,655,9,327,284,1,25,708,417,0,2,6,14,14,14,(2, 3, 109),(1,0,1,0,0,0,1,1,1,0)
#define PPUTLU_653 652,654,9,326,282,1,25,425,416,1,1,5,13,13,13,(),(1,0,1,0,0,0,1,1,0,1)
#define PPUTLU_652 651,653,9,326,280,1,25,144,415,0,0,4,12,12,12,(2, 2, 163),(1,0,1,0,0,0,1,1,0,0)
#define PPUTLU_651 650,652,9,325,278,1,25,889,413,1,3,3,11,11,11,(3, 7, 31),(1,0,1,0,0,0,1,0,1,1)
#define PPUTLU_650 649,651,9,325,276,1,25,612,412,0,2,2,10,10,10,(2, 5, 5, 13),(1,0,1,0,0,0,1,0,1,0)
#define PPUTLU_649 648,650,9,324,274,1,25,337,411,1,1,1,9,9,9,(11, 59),(1,0,1,0,0,0,1,0,0,1)
#define PPUTLU_648 647,649,9,324,272,1,25,64,410,0,0,0,8,8,8,(2, 2, 2, 3, 3, 3, 3),(1,0,1,0,0,0,1,0,0,0)
#define PPUTLU_647 646,648,9,323,270,1,25,817,408,1,3,7,7,7,7,(),(1,0,1,0,0,0,0,1,1,1)
#define PPUTLU_646 645,647,9,323,268,1,25,548,407,0,2,6,6,6,6,(2, 17, 19),(1,0,1,0,0,0,0,1,1,0)
#define PPUTLU_645 644,646,9,322,266,1,25,281,406,1,1,5,5,5,5,(3, 5, 43),(1,0,1,0,0,0,0,1,0,1)
#define PPUTLU_644 643,645,9,322,264,1,25,16,405,0,0,4,4,4,4,(2, 2, 7, 23),(1,0,1,0,0,0,0,1,0,0)
#define PPUTLU_643 642,644,9,321,262,1,25,777,403,1,3,3,3,3,3,(),(1,0,1,0,0,0,0,0,1,1)
#define PPUTLU_642 641,643,9,321,260,1,25,516,402,0,2,2,2,2,2,(2, 3, 107),(1,0,1,0,0,0,0,0,1,0)
#define PPUTLU_641 640,642,9,320,258,1,25,257,401,1,1,1,1,1,1,(),(1,0,1,0,0,0,0,0,0,1)
#define PPUTLU_640 639,641,9,320,256,1,25,0,400,0,0,0,0,0,0,(2, 2, 2, 2, 2, 2, 2, 5),(1,0,1,0,0,0,0,0,0,0)
#define PPUTLU_639 638,640,9,319,254,1,25,769,398,1,3,7,15,31,63,(3, 3, 71),(1,0,0,1,1,1,1,1,1,1)
#define PPUTLU_638 637,639,9,319,252,1,25,516,397,0,2,6,14,30,62,(2, 11, 29),(1,0,0,1,1,1,1,1,1,0)
#define PPUTLU_637 636,638,9,318,250,1,25,265,396,1,1,5,13,29,61,(7, 7, 13),(1,0,0,1,1,1,1,1,0,1)
#define PPUTLU_636 635,637,9,318,248,1,25,16,395,0,0,4,12,28,60,(2, 2, 3, 53),(1,0,0,1,1,1,1,1,0,0)
#define PPUTLU_635 634,636,9,317,246,1,25,793,393,1,3,3,11,27,59,(5, 127),(1,0,0,1,1,1,1,0,1,1)
#define PPUTLU_634 633,635,9,317,244,1,25,548,392,0,2,2,10,26,58,(2, 317),(1,0,0,1,1,1,1,0,1,0)
#define PPUTLU_633 632,634,9,316,242,1,25,305,391,1,1,1,9,25,57,(3, 211),(1,0,0,1,1,1,1,0,0,1)
#define PPUTLU_632 631,633,9,316,240,1,25,64,390,0,0,0,8,24,56,(2, 2, 2, 79),(1,0,0,1,1,1,1,0,0,0)
#define PPUTLU_631 630,632,9,315,238,1,25,849,388,1,3,7,7,23,55,(),(1,0,0,1,1,1,0,1,1,1)
#define PPUTLU_630 629,631,9,315,236,1,25,612,387,0,2,6,6,22,54,(2, 3, 3, 5, 7),(1,0,0,1,1,1,0,1,1,0)
#define PPUTLU_629 628,630,9,314,234,1,25,377,386,1,1,5,5,21,53,(17, 37),(1,0,0,1,1,1,0,1,0,1)
#define PPUTLU_628 627,629,9,314,232,1,25,144,385,0,0,4,4,20,52,(2, 2, 157),(1,0,0,1,1,1,0,1,0,0)
#define PPUTLU_627 626,628,9,313,230,1,25,937,383,1,3,3,3,19,51,(3, 11, 19),(1,0,0,1,1,1,0,0,1,1)
#define PPUTLU_626 625,627,9,313,228,1,25,708,382,0,2,2,2,18,50,(2, 313),(1,0,0,1,1,1,0,0,1,0)
#define PPUTLU_625 624,626,9,312,226,1,25,481,381,1,1,1,1,17,49,(5, 5, 5, 5),(1,0,0,1,1,1,0,0,0,1)
#define PPUTLU_624 623,625,9,312,224,1,24,256,380,0,0,0,0,16,48,(2, 2, 2, 2, 3, 13),(1,0,0,1,1,1,0,0,0,0)
#define PPUTLU_623 622,624,9,311,222,1,24,33,379,1,3,7,15,15,47,(7, 89),(1,0,0,1,1,0,1,1,1,1)
#define PPUTLU_622 621,623,9,311,220,1,24,836,377,0,2,6,14,14,46,(2, 311),(1,0,0,1,1,0,1,1,1,0)
#define PPUTLU_621 620,622,9,310,218,1,24,617,376,1,1,5,13,13,45,(3, 3, 3, 23),(1,0,0,1,1,0,1,1,0,1)
#define PPUTLU_620 619,621,9,310,216,1,24,400,375,0,0,4,12,12,44,(2, 2, 5, 31),(1,0,0,1,1,0,1,1,0,0)
#define PPUTLU_619 618,620,9,309,214,1,24,185,374,1,3,3,11,11,43,(),(1,0,0,1,1,0,1,0,1,1)
#define PPUTLU_618 617,619,9,309,212,1,24,996,372,0,2,2,10,10,42,(2, 3, 103),(1,0,0,1,1,0,1,0,1,0)
#define PPUTLU_617 616,618,9,308,210,1,24,785,371,1,1,1,9,9,41,(),(1,0,0,1,1,0,1,0,0,1)
#define PPUTLU_616 615,617,9,308,208,1,24,576,370,0,0,0,8,8,40,(2, 2, 2, 7, 11),(1,0,0,1,1,0,1,0,0,0)
#define PPUTLU_615 614,616,9,307,206,1,24,369,369,1,3,7,7,7,39,(3, 5, 41),(1,0,0,1,1,0,0,1,1,1)
#define PPUTLU_614 613,615,9,307,204,1,24,164,368,0,2,6,6,6,38,(2, 307),(1,0,0,1,1,0,0,1,1,0)
#define PPUTLU_613 612,614,9,306,202,1,24,985,366,1,1,5,5,5,37,(),(1,0,0,1,1,0,0,1,0,1)
#define PPUTLU_612 611,613,9,306,200,1,24,784,365,0,0,4,4,4,36,(2, 2, 3, 3, 17),(1,0,0,1,1,0,0,1,0,0)
#define PPUTLU_611 610,612,9,305,198,1,24,585,364,1,3,3,3,3,35,(13, 47),(1,0,0,1,1,0,0,0,1,1)
#define PPUTLU_610 609,611,9,305,196,1,24,388,363,0,2,2,2,2,34,(2, 5, 61),(1,0,0,1,1,0,0,0,1,0)
#define PPUTLU_609 608,610,9,304,194,1,24,193,362,1,1,1,1,1,33,(3, 7, 29),(1,0,0,1,1,0,0,0,0,1)
#define PPUTLU_608 607,609,9,304,192,1,24,0,361,0,0,0,0,0,32,(2, 2, 2, 2, 2, 19),(1,0,0,1,1,0,0,0,0,0)
#define PPUTLU_607 606,608,9,303,190,1,24,833,359,1,3,7,15,31,31,(),(1,0,0,1,0,1,1,1,1,1)
#define PPUTLU_606 605,607,9,303,188,1,24,644,358,0,2,6,14,30,30,(2, 3, 101),(1,0,0,1,0,1,1,1,1,0)
#define PPUTLU_605 604,606,9,302,186,1,24,457,357,1,1,5,13,29,29,(5, 11, 11),(1,0,0,1,0,1,1,1,0,1)
#define PPUTLU_604 603,605,9,302,184,1,24,272,356,0,0,4,12,28,28,(2, 2, 151),(1,0,0,1,0,1,1,1,0,0)
#define PPUTLU_603 602,604,9,301,182,1,24,89,355,1,3,3,11,27,27,(3, 3, 67),(1,0,0,1,0,1,1,0,1,1)
#define PPUTLU_602 601,603,9,301,180,1,24,932,353,0,2,2,10,26,26,(2, 7, 43),(1,0,0,1,0,1,1,0,1,0)
#define PPUTLU_601 600,602,9,300,178,1,24,753,352,1,1,1,9,25,25,(),(1,0,0,1,0,1,1,0,0,1)
#define PPUTLU_600 599,601,9,300,176,1,24,576,351,0,0,0,8,24,24,(2, 2, 2, 3, 5, 5),(1,0,0,1,0,1,1,0,0,0)
#define PPUTLU_599 598,600,9,299,174,1,24,401,350,1,3,7,7,23,23,(),(1,0,0,1,0,1,0,1,1,1)
#define PPUTLU_598 597,599,9,299,172,1,24,228,349,0,2,6,6,22,22,(2, 13, 23),(1,0,0,1,0,1,0,1,1,0)
#define PPUTLU_597 596,598,9,298,170,1,24,57,348,1,1,5,5,21,21,(3, 199),(1,0,0,1,0,1,0,1,0,1)
#define PPUTLU_596 595,597,9,298,168,1,24,912,346,0,0,4,4,20,20,(2, 2, 149),(1,0,0,1,0,1,0,1,0,0)
#define PPUTLU_595 594,596,9,297,166,1,24,745,345,1,3,3,3,19,19,(5, 7, 17),(1,0,0,1,0,1,0,0,1,1)
#define PPUTLU_594 593,595,9,297,164,1,24,580,344,0,2,2,2,18,18,(2, 3, 3, 3, 11),(1,0,0,1,0,1,0,0,1,0)
#define PPUTLU_593 592,594,9,296,162,1,24,417,343,1,1,1,1,17,17,(),(1,0,0,1,0,1,0,0,0,1)
#define PPUTLU_592 591,593,9,296,160,1,24,256,342,0,0,0,0,16,16,(2, 2, 2, 2, 37),(1,0,0,1,0,1,0,0,0,0)
#define PPUTLU_591 590,592,9,295,158,1,24,97,341,1,3,7,15,15,15,(3, 197),(1,0,0,1,0,0,1,1,1,1)
#define PPUTLU_590 589,591,9,295,156,1,24,964,339,0,2,6,14,14,14,(2, 5, 59),(1,0,0,1,0,0,1,1,1,0)
#define PPUTLU_589 588,590,9,294,154,1,24,809,338,1,1,5,13,13,13,(19, 31),(1,0,0,1,0,0,1,1,0,1)
#define PPUTLU_588 587,589,9,294,152,1,24,656,337,0,0,4,12,12,12,(2, 2, 3, 7, 7),(1,0,0,1,0,0,1,1,0,0)
#define PPUTLU_587 586,588,9,293,150,1,24,505,336,1,3,3,11,11,11,(),(1,0,0,1,0,0,1,0,1,1)
#define PPUTLU_586 585,587,9,293,148,1,24,356,335,0,2,2,10,10,10,(2, 293),(1,0,0,1,0,0,1,0,1,0)
#define PPUTLU_585 584,586,9,292,146,1,24,209,334,1,1,1,9,9,9,(3, 3, 5, 13),(1,0,0,1,0,0,1,0,0,1)
#define PPUTLU_584 583,585,9,292,144,1,24,64,333,0,0,0,8,8,8,(2, 2, 2, 73),(1,0,0,1,0,0,1,0,0,0)
#define PPUTLU_583 582,584,9,291,142,1,24,945,331,1,3,7,7,7,7,(11, 53),(1,0,0,1,0,0,0,1,1,1)
#define PPUTLU_582 581,583,9,291,140,1,24,804,330,0,2,6,6,6,6,(2, 3, 97),(1,0,0,1,0,0,0,1,1,0)
#define PPUTLU_581 580,582,9,290,138,1,24,665,329,1,1,5,5,5,5,(7, 83),(1,0,0,1,0,0,0,1,0,1)
#define PPUTLU_580 579,581,9,290,136,1,24,528,328,0,0,4,4,4,4,(2, 2, 5, 29),(1,0,0,1,0,0,0,1,0,0)
#define PPUTLU_579 578,580,9,289,134,1,24,393,327,1,3,3,3,3,3,(3, 193),(1,0,0,1,0,0,0,0,1,1)
#define PPUTLU_578 577,579,9,289,132,1,24,260,326,0,2,2,2,2,2,(2, 17, 17),(1,0,0,1,0,0,0,0,1,0)
#define PPUTLU_577 576,578,9,288,130,1,24,129,325,1,1,1,1,1,1,(),(1,0,0,1,0,0,0,0,0,1)
#define PPUTLU_576 575,577,9,288,128,1,24,0,324,0,0,0,0,0,0,(2, 2, 2, 2, 2, 2, 3, 3),(1,0,0,1,0,0,0,0,0,0)
#define PPUTLU_575 574,576,9,287,126,1,23,897,322,1,3,7,15,31,63,(5, 5, 23),(1,0,0,0,1,1,1,1,1,1)
#define PPUTLU_574 573,575,9,287,124,1,23,772,321,0,2,6,14,30,62,(2, 7, 41),(1,0,0,0,1,1,1,1,1,0)
#define PPUTLU_573 572,574,9,286,122,1,23,649,320,1,1,5,13,29,61,(3, 191),(1,0,0,0,1,1,1,1,0,1)
#define PPUTLU_572 571,573,9,286,120,1,23,528,319,0,0,4,12,28,60,(2, 2, 11, 13),(1,0,0,0,1,1,1,1,0,0)
#define PPUTLU_571 570,572,9,285,118,1,23,409,318,1,3,3,11,27,59,(),(1,0,0,0,1,1,1,0,1,1)
#define PPUTLU_570 569,571,9,285,116,1,23,292,317,0,2,2,10,26,58,(2, 3, 5, 19),(1,0,0,0,1,1,1,0,1,0)
#define PPUTLU_569 568,570,9,284,114,1,23,177,316,1,1,1,9,25,57,(),(1,0,0,0,1,1,1,0,0,1)
#define PPUTLU_568 567,569,9,284,112,1,23,64,315,0,0,0,8,24,56,(2, 2, 2, 71),(1,0,0,0,1,1,1,0,0,0)
#define PPUTLU_567 566,568,9,283,110,1,23,977,313,1,3,7,7,23,55,(3, 3, 3, 3, 7),(1,0,0,0,1,1,0,1,1,1)
#define PPUTLU_566 565,567,9,283,108,1,23,868,312,0,2,6,6,22,54,(2, 283),(1,0,0,0,1,1,0,1,1,0)
#define PPUTLU_565 564,566,9,282,106,1,23,761,311,1,1,5,5,21,53,(5, 113),(1,0,0,0,1,1,0,1,0,1)
#define PPUTLU_564 563,565,9,282,104,1,23,656,310,0,0,4,4,20,52,(2, 2, 3, 47),(1,0,0,0,1,1,0,1,0,0)
#define PPUTLU_563 562,564,9,281,102,1,23,553,309,1,3,3,3,19,51,(),(1,0,0,0,1,1,0,0,1,1)
#define PPUTLU_562 561,563,9,281,100,1,23,452,308,0,2,2,2,18,50,(2, 281),(1,0,0,0,1,1,0,0,1,0)
#define PPUTLU_561 560,562,9,280,98,1,23,353,307,1,1,1,1,17,49,(3, 11, 17),(1,0,0,0,1,1,0,0,0,1)
#define PPUTLU_560 559,561,9,280,96,1,23,256,306,0,0,0,0,16,48,(2, 2, 2, 2, 5, 7),(1,0,0,0,1,1,0,0,0,0)
#define PPUTLU_559 558,560,9,279,94,1,23,161,305,1,3,7,15,15,47,(13, 43),(1,0,0,0,1,0,1,1,1,1)
#define PPUTLU_558 557,559,9,279,92,1,23,68,304,0,2,6,14,14,46,(2, 3, 3, 31),(1,0,0,0,1,0,1,1,1,0)
#define PPUTLU_557 556,558,9,278,90,1,23,1001,302,1,1,5,13,13,45,(),(1,0,0,0,1,0,1,1,0,1)
#define PPUTLU_556 555,557,9,278,88,1,23,912,301,0,0,4,12,12,44,(2, 2, 139),(1,0,0,0,1,0,1,1,0,0)
#define PPUTLU_555 554,556,9,277,86,1,23,825,300,1,3,3,11,11,43,(3, 5, 37),(1,0,0,0,1,0,1,0,1,1)
#define PPUTLU_554 553,555,9,277,84,1,23,740,299,0,2,2,10,10,42,(2, 277),(1,0,0,0,1,0,1,0,1,0)
#define PPUTLU_553 552,554,9,276,82,1,23,657,298,1,1,1,9,9,41,(7, 79),(1,0,0,0,1,0,1,0,0,1)
#define PPUTLU_552 551,553,9,276,80,1,23,576,297,0,0,0,8,8,40,(2, 2, 2, 3, 23),(1,0,0,0,1,0,1,0,0,0)
#define PPUTLU_551 550,552,9,275,78,1,23,497,296,1,3,7,7,7,39,(19, 29),(1,0,0,0,1,0,0,1,1,1)
#define PPUTLU_550 549,551,9,275,76,1,23,420,295,0,2,6,6,6,38,(2, 5, 5, 11),(1,0,0,0,1,0,0,1,1,0)
#define PPUTLU_549 548,550,9,274,74,1,23,345,294,1,1,5,5,5,37,(3, 3, 61),(1,0,0,0,1,0,0,1,0,1)
#define PPUTLU_548 547,549,9,274,72,1,23,272,293,0,0,4,4,4,36,(2, 2, 137),(1,0,0,0,1,0,0,1,0,0)
#define PPUTLU_547 546,548,9,273,70,1,23,201,292,1,3,3,3,3,35,(),(1,0,0,0,1,0,0,0,1,1)
#define PPUTLU_546 545,547,9,273,68,1,23,132,291,0,2,2,2,2,34,(2, 3, 7, 13),(1,0,0,0,1,0,0,0,1,0)
#define PPUTLU_545 544,546,9,272,66,1,23,65,290,1,1,1,1,1,33,(5, 109),(1,0,0,0,1,0,0,0,0,1)
#define PPUTLU_544 543,545,9,272,64,1,23,0,289,0,0,0,0,0,32,(2, 2, 2, 2, 2, 17),(1,0,0,0,1,0,0,0,0,0)
#define PPUTLU_543 542,544,9,271,62,1,23,961,287,1,3,7,15,31,31,(3, 181),(1,0,0,0,0,1,1,1,1,1)
#define PPUTLU_542 541,543,9,271,60,1,23,900,286,0,2,6,14,30,30,(2, 271),(1,0,0,0,0,1,1,1,1,0)
#define PPUTLU_541 540,542,9,270,58,1,23,841,285,1,1,5,13,29,29,(),(1,0,0,0,0,1,1,1,0,1)
#define PPUTLU_540 539,541,9,270,56,1,23,784,284,0,0,4,12,28,28,(2, 2, 3, 3, 3, 5),(1,0,0,0,0,1,1,1,0,0)
#define PPUTLU_539 538,540,9,269,54,1,23,729,283,1,3,3,11,27,27,(7, 7, 11),(1,0,0,0,0,1,1,0,1,1)
#define PPUTLU_538 537,539,9,269,52,1,23,676,282,0,2,2,10,26,26,(2, 269),(1,0,0,0,0,1,1,0,1,0)
#define PPUTLU_537 536,538,9,268,50,1,23,625,281,1,1,1,9,25,25,(3, 179),(1,0,0,0,0,1,1,0,0,1)
#define PPUTLU_536 535,537,9,268,48,1,23,576,280,0,0,0,8,24,24,(2, 2, 2, 67),(1,0,0,0,0,1,1,0,0,0)
#define PPUTLU_535 534,536,9,267,46,1,23,529,279,1,3,7,7,23,23,(5, 107),(1,0,0,0,0,1,0,1,1,1)
#define PPUTLU_534 533,535,9,267,44,1,23,484,278,0,2,6,6,22,22,(2, 3, 89),(1,0,0,0,0,1,0,1,1,0)
#define PPUTLU_533 532,534,9,266,42,1,23,441,277,1,1,5,5,21,21,(13, 41),(1,0,0,0,0,1,0,1,0,1)
#define PPUTLU_532 531,533,9,266,40,1,23,400,276,0,0,4,4,20,20,(2, 2, 7, 19),(1,0,0,0,0,1,0,1,0,0)
#define PPUTLU_531 530,532,9,265,38,1,23,361,275,1,3,3,3,19,19,(3, 3, 59),(1,0,0,0,0,1,0,0,1,1)
#define PPUTLU_530 529,531,9,265,36,1,23,324,274,0,2,2,2,18,18,(2, 5, 53),(1,0,0,0,0,1,0,0,1,0)
#define PPUTLU_529 528,530,9,264,34,1,23,289,273,1,1,1,1,17,17,(23, 23),(1,0,0,0,0,1,0,0,0,1)
#define PPUTLU_528 527,529,9,264,32,1,22,256,272,0,0,0,0,16,16,(2, 2, 2, 2, 3, 11),(1,0,0,0,0,1,0,0,0,0)
#define PPUTLU_527 526,528,9,263,30,1,22,225,271,1,3,7,15,15,15,(17, 31),(1,0,0,0,0,0,1,1,1,1)
#define PPUTLU_526 525,527,9,263,28,1,22,196,270,0,2,6,14,14,14,(2, 263),(1,0,0,0,0,0,1,1,1,0)
#define PPUTLU_525 524,526,9,262,26,1,22,169,269,1,1,5,13,13,13,(3, 5, 5, 7),(1,0,0,0,0,0,1,1,0,1)
#define PPUTLU_524 523,525,9,262,24,1,22,144,268,0,0,4,12,12,12,(2, 2, 131),(1,0,0,0,0,0,1,1,0,0)
#define PPUTLU_523 522,524,9,261,22,1,22,121,267,1,3,3,11,11,11,(),(1,0,0,0,0,0,1,0,1,1)
#define PPUTLU_522 521,523,9,261,20,1,22,100,266,0,2,2,10,10,10,(2, 3, 3, 29),(1,0,0,0,0,0,1,0,1,0)
#define PPUTLU_521 520,522,9,260,18,1,22,81,265,1,1,1,9,9,9,(),(1,0,0,0,0,0,1,0,0,1)
#define PPUTLU_520 519,521,9,260,16,1,22,64,264,0,0,0,8,8,8,(2, 2, 2, 5, 13),(1,0,0,0,0,0,1,0,0,0)
#define PPUTLU_519 518,520,9,259,14,1,22,49,263,1,3,7,7,7,7,(3, 173),(1,0,0,0,0,0,0,1,1,1)
#define PPUTLU_518 517,519,9,259,12,1,22,36,262,0,2,6,6,6,6,(2, 7, 37),(1,0,0,0,0,0,0,1,1,0)
#define PPUTLU_517 516,518,9,258,10,1,22,25,261,1,1,5,5,5,5,(11, 47),(1,0,0,0,0,0,0,1,0,1)
#define PPUTLU_516 515,517,9,258,8,1,22,16,260,0,0,4,4,4,4,(2, 2, 3, 43),(1,0,0,0,0,0,0,1,0,0)
#define PPUTLU_515 514,516,9,257,6,1,22,9,259,1,3,3,3,3,3,(5, 103),(1,0,0,0,0,0,0,0,1,1)
#define PPUTLU_514 513,515,9,257,4,1,22,4,258,0,2,2,2,2,2,(2, 257),(1,0,0,0,0,0,0,0,1,0)
#define PPUTLU_513 512,514,9,256,2,1,22,1,257,1,1,1,1,1,1,(3, 3, 3, 19),(1,0,0,0,0,0,0,0,0,1)
#define PPUTLU_512 511,513,9,256,0,1,22,0,256,0,0,0,0,0,0,(2, 2, 2, 2, 2, 2, 2, 2, 2),(1,0,0,0,0,0,0,0,0,0)
#define PPUTLU_511 510,512,8,255,1022,0,22,1,255,1,3,7,15,31,63,(7, 73),(0,1,1,1,1,1,1,1,1,1)
#define PPUTLU_510 509,511,8,255,1020,0,22,4,254,0,2,6,14,30,62,(2, 3, 5, 17),(0,1,1,1,1,1,1,1,1,0)
#define PPUTLU_509 508,510,8,254,1018,0,22,9,253,1,1,5,13,29,61,(),(0,1,1,1,1,1,1,1,0,1)
#define PPUTLU_508 507,509,8,254,1016,0,22,16,252,0,0,4,12,28,60,(2, 2, 127),(0,1,1,1,1,1,1,1,0,0)
#define PPUTLU_507 506,508,8,253,1014,0,22,25,251,1,3,3,11,27,59,(3, 13, 13),(0,1,1,1,1,1,1,0,1,1)
#define PPUTLU_506 505,507,8,253,1012,0,22,36,250,0,2,2,10,26,58,(2, 11, 23),(0,1,1,1,1,1,1,0,1,0)
#define PPUTLU_505 504,506,8,252,1010,0,22,49,249,1,1,1,9,25,57,(5, 101),(0,1,1,1,1,1,1,0,0,1)
#define PPUTLU_504 503,505,8,252,1008,0,22,64,248,0,0,0,8,24,56,(2, 2, 2, 3, 3, 7),(0,1,1,1,1,1,1,0,0,0)
#define PPUTLU_503 502,504,8,251,1006,0,22,81,247,1,3,7,7,23,55,(),(0,1,1,1,1,1,0,1,1,1)
#define PPUTLU_502 501,503,8,251,1004,0,22,100,246,0,2,6,6,22,54,(2, 251),(0,1,1,1,1,1,0,1,1,0)
#define PPUTLU_501 500,502,8,250,1002,0,22,121,245,1,1,5,5,21,53,(3, 167),(0,1,1,1,1,1,0,1,0,1)
#define PPUTLU_500 499,501,8,250,1000,0,22,144,244,0,0,4,4,20,52,(2, 2, 5, 5, 5),(0,1,1,1,1,1,0,1,0,0)
#define PPUTLU_499 498,500,8,249,998,0,22,169,243,1,3,3,3,19,51,(),(0,1,1,1,1,1,0,0,1,1)
#define PPUTLU_498 497,499,8,249,996,0,22,196,242,0,2,2,2,18,50,(2, 3, 83),(0,1,1,1,1,1,0,0,1,0)
#define PPUTLU_497 496,498,8,248,994,0,22,225,241,1,1,1,1,17,49,(7, 71),(0,1,1,1,1,1,0,0,0,1)
#define PPUTLU_496 495,497,8,248,992,0,22,256,240,0,0,0,0,16,48,(2, 2, 2, 2, 31),(0,1,1,1,1,1,0,0,0,0)
#define PPUTLU_495 494,496,8,247,990,0,22,289,239,1,3,7,15,15,47,(3, 3, 5, 11),(0,1,1,1,1,0,1,1,1,1)
#define PPUTLU_494 493,495,8,247,988,0,22,324,238,0,2,6,14,14,46,(2, 13, 19),(0,1,1,1,1,0,1,1,1,0)
#define PPUTLU_493 492,494,8,246,986,0,22,361,237,1,1,5,13,13,45,(17, 29),(0,1,1,1,1,0,1,1,0,1)
#define PPUTLU_492 491,493,8,246,984,0,22,400,236,0,0,4,12,12,44,(2, 2, 3, 41),(0,1,1,1,1,0,1,1,0,0)
#define PPUTLU_491 490,492,8,245,982,0,22,441,235,1,3,3,11,11,43,(),(0,1,1,1,1,0,1,0,1,1)
#define PPUTLU_490 489,491,8,245,980,0,22,484,234,0,2,2,10,10,42,(2, 5, 7, 7),(0,1,1,1,1,0,1,0,1,0)
#define PPUTLU_489 488,490,8,244,978,0,22,529,233,1,1,1,9,9,41,(3, 163),(0,1,1,1,1,0,1,0,0,1)
#define PPUTLU_488 487,489,8,244,976,0,22,576,232,0,0,0,8,8,40,(2, 2, 2, 61),(0,1,1,1,1,0,1,0,0,0)
#define PPUTLU_487 486,488,8,243,974,0,22,625,231,1,3,7,7,7,39,(),(0,1,1,1,1,0,0,1,1,1)
#define PPUTLU_486 485,487,8,243,972,0,22,676,230,0,2,6,6,6,38,(2, 3, 3, 3, 3, 3),(0,1,1,1,1,0,0,1,1,0)
#define PPUTLU_485 484,486,8,242,970,0,22,729,229,1,1,5,5,5,37,(5, 97),(0,1,1,1,1,0,0,1,0,1)
#define PPUTLU_484 483,485,8,242,968,0,22,784,228,0,0,4,4,4,36,(2, 2, 11, 11),(0,1,1,1,1,0,0,1,0,0)
#define PPUTLU_483 482,484,8,241,966,0,21,841,227,1,3,3,3,3,35,(3, 7, 23),(0,1,1,1,1,0,0,0,1,1)
#define PPUTLU_482 481,483,8,241,964,0,21,900,226,0,2,2,2,2,34,(2, 241),(0,1,1,1,1,0,0,0,1,0)
#define PPUTLU_481 480,482,8,240,962,0,21,961,225,1,1,1,1,1,33,(13, 37),(0,1,1,1,1,0,0,0,0,1)
#define PPUTLU_480 479,481,8,240,960,0,21,0,225,0,0,0,0,0,32,(2, 2, 2, 2, 2, 3, 5),(0,1,1,1,1,0,0,0,0,0)
#define PPUTLU_479 478,480,8,239,958,0,21,65,224,1,3,7,15,31,31,(),(0,1,1,1,0,1,1,1,1,1)
#define PPUTLU_478 477,479,8,239,956,0,21,132,223,0,2,6,14,30,30,(2, 239),(0,1,1,1,0,1,1,1,1,0)
#define PPUTLU_477 476,478,8,238,954,0,21,201,222,1,1,5,13,29,29,(3, 3, 53),(0,1,1,1,0,1,1,1,0,1)
#define PPUTLU_476 475,477,8,238,952,0,21,272,221,0,0,4,12,28,28,(2, 2, 7, 17),(0,1,1,1,0,1,1,1,0,0)
#define PPUTLU_475 474,476,8,237,950,0,21,345,220,1,3,3,11,27,27,(5, 5, 19),(0,1,1,1,0,1,1,0,1,1)
#define PPUTLU_474 473,475,8,237,948,0,21,420,219,0,2,2,10,26,26,(2, 3, 79),(0,1,1,1,0,1,1,0,1,0)
#define PPUTLU_473 472,474,8,236,946,0,21,497,218,1,1,1,9,25,25,(11, 43),(0,1,1,1,0,1,1,0,0,1)
#define PPUTLU_472 471,473,8,236,944,0,21,576,217,0,0,0,8,24,24,(2, 2, 2, 59),(0,1,1,1,0,1,1,0,0,0)
#define PPUTLU_471 470,472,8,235,942,0,21,657,216,1,3,7,7,23,23,(3, 157),(0,1,1,1,0,1,0,1,1,1)
#define PPUTLU_470 469,471,8,235,940,0,21,740,215,0,2,6,6,22,22,(2, 5, 47),(0,1,1,1,0,1,0,1,1,0)
#define PPUTLU_469 468,470,8,234,938,0,21,825,214,1,1,5,5,21,21,(7, 67),(0,1,1,1,0,1,0,1,0,1)
#define PPUTLU_468 467,469,8,234,936,0,21,912,213,0,0,4,4,20,20,(2, 2, 3, 3, 13),(0,1,1,1,0,1,0,1,0,0)
#define PPUTLU_467 466,468,8,233,934,0,21,1001,212,1,3,3,3,19,19,(),(0,1,1,1,0,1,0,0,1,1)
#define PPUTLU_466 465,467,8,233,932,0,21,68,212,0,2,2,2,18,18,(2, 233),(0,1,1,1,0,1,0,0,1,0)
#define PPUTLU_465 464,466,8,232,930,0,21,161,211,1,1,1,1,17,17,(3, 5, 31),(0,1,1,1,0,1,0,0,0,1)
#define PPUTLU_464 463,465,8,232,928,0,21,256,210,0,0,0,0,16,16,(2, 2, 2, 2, 29),(0,1,1,1,0,1,0,0,0,0)
#define PPUTLU_463 462,464,8,231,926,0,21,353,209,1,3,7,15,15,15,(),(0,1,1,1,0,0,1,1,1,1)
#define PPUTLU_462 461,463,8,231,924,0,21,452,208,0,2,6,14,14,14,(2, 3, 7, 11),(0,1,1,1,0,0,1,1,1,0)
#define PPUTLU_461 460,462,8,230,922,0,21,553,207,1,1,5,13,13,13,(),(0,1,1,1,0,0,1,1,0,1)
#define PPUTLU_460 459,461,8,230,920,0,21,656,206,0,0,4,12,12,12,(2, 2, 5, 23),(0,1,1,1,0,0,1,1,0,0)
#define PPUTLU_459 458,460,8,229,918,0,21,761,205,1,3,3,11,11,11,(3, 3, 3, 17),(0,1,1,1,0,0,1,0,1,1)
#define PPUTLU_458 457,459,8,229,916,0,21,868,204,0,2,2,10,10,10,(2, 229),(0,1,1,1,0,0,1,0,1,0)
#define PPUTLU_457 456,458,8,228,914,0,21,977,203,1,1,1,9,9,9,(),(0,1,1,1,0,0,1,0,0,1)
#define PPUTLU_456 455,457,8,228,912,0,21,64,203,0,0,0,8,8,8,(2, 2, 2, 3, 19),(0,1,1,1,0,0,1,0,0,0)
#define PPUTLU_455 454,456,8,227,910,0,21,177,202,1,3,7,7,7,7,(5, 7, 13),(0,1,1,1,0,0,0,1,1,1)
#define PPUTLU_454 453,455,8,227,908,0,21,292,201,0,2,6,6,6,6,(2, 227),(0,1,1,1,0,0,0,1,1,0)
#define PPUTLU_453 452,454,8,226,906,0,21,409,200,1,1,5,5,5,5,(3, 151),(0,1,1,1,0,0,0,1,0,1)
#define PPUTLU_452 451,453,8,226,904,0,21,528,199,0,0,4,4,4,4,(2, 2, 113),(0,1,1,1,0,0,0,1,0,0)
#define PPUTLU_451 450,452,8,225,902,0,21,649,198,1,3,3,3,3,3,(11, 41),(0,1,1,1,0,0,0,0,1,1)
#define PPUTLU_450 449,451,8,225,900,0,21,772,197,0,2,2,2,2,2,(2, 3, 3, 5, 5),(0,1,1,1,0,0,0,0,1,0)
#define PPUTLU_449 448,450,8,224,898,0,21,897,196,1,1,1,1,1,1,(),(0,1,1,1,0,0,0,0,0,1)
#define PPUTLU_448 447,449,8,224,896,0,21,0,196,0,0,0,0,0,0,(2, 2, 2, 2, 2, 2, 7),(0,1,1,1,0,0,0,0,0,0)
#define PPUTLU_447 446,448,8,223,894,0,21,129,195,1,3,7,15,31,63,(3, 149),(0,1,1,0,1,1,1,1,1,1)
#define PPUTLU_446 445,447,8,223,892,0,21,260,194,0,2,6,14,30,62,(2, 223),(0,1,1,0,1,1,1,1,1,0)
#define PPUTLU_445 444,446,8,222,890,0,21,393,193,1,1,5,13,29,61,(5, 89),(0,1,1,0,1,1,1,1,0,1)
#define PPUTLU_444 443,445,8,222,888,0,21,528,192,0,0,4,12,28,60,(2, 2, 3, 37),(0,1,1,0,1,1,1,1,0,0)
#define PPUTLU_443 442,444,8,221,886,0,21,665,191,1,3,3,11,27,59,(),(0,1,1,0,1,1,1,0,1,1)
#define PPUTLU_442 441,443,8,221,884,0,21,804,190,0,2,2,10,26,58,(2, 13, 17),(0,1,1,0,1,1,1,0,1,0)
#define PPUTLU_441 440,442,8,220,882,0,21,945,189,1,1,1,9,25,57,(3, 3, 7, 7),(0,1,1,0,1,1,1,0,0,1)
#define PPUTLU_440 439,441,8,220,880,0,20,64,189,0,0,0,8,24,56,(2, 2, 2, 5, 11),(0,1,1,0,1,1,1,0,0,0)
#define PPUTLU_439 438,440,8,219,878,0,20,209,188,1,3,7,7,23,55,(),(0,1,1,0,1,1,0,1,1,1)
#define PPUTLU_438 437,439,8,219,876,0,20,356,187,0,2,6,6,22,54,(2, 3, 73),(0,1,1,0,1,1,0,1,1,0)
#define PPUTLU_437 436,438,8,218,874,0,20,505,186,1,1,5,5,21,53,(19, 23),(0,1,1,0,1,1,0,1,0,1)
#define PPUTLU_436 435,437,8,218,872,0,20,656,185,0,0,4,4,20,52,(2, 2, 109),(0,1,1,0,1,1,0,1,0,0)
#define PPUTLU_435 434,436,8,217,870,0,20,809,184,1,3,3,3,19,51,(3, 5, 29),(0,1,1,0,1,1,0,0,1,1)
#define PPUTLU_434 433,435,8,217,868,0,20,964,183,0,2,2,2,18,50,(2, 7, 31),(0,1,1,0,1,1,0,0,1,0)
#define PPUTLU_433 432,434,8,216,866,0,20,97,183,1,1,1,1,17,49,(),(0,1,1,0,1,1,0,0,0,1)
#define PPUTLU_432 431,433,8,216,864,0,20,256,182,0,0,0,0,16,48,(2, 2, 2, 2, 3, 3, 3),(0,1,1,0,1,1,0,0,0,0)
#define PPUTLU_431 430,432,8,215,862,0,20,417,181,1,3,7,15,15,47,(),(0,1,1,0,1,0,1,1,1,1)
#define PPUTLU_430 429,431,8,215,860,0,20,580,180,0,2,6,14,14,46,(2, 5, 43),(0,1,1,0,1,0,1,1,1,0)
#define PPUTLU_429 428,430,8,214,858,0,20,745,179,1,1,5,13,13,45,(3, 11, 13),(0,1,1,0,1,0,1,1,0,1)
#define PPUTLU_428 427,429,8,214,856,0,20,912,178,0,0,4,12,12,44,(2, 2, 107),(0,1,1,0,1,0,1,1,0,0)
#define PPUTLU_427 426,428,8,213,854,0,20,57,178,1,3,3,11,11,43,(7, 61),(0,1,1,0,1,0,1,0,1,1)
#define PPUTLU_426 425,427,8,213,852,0,20,228,177,0,2,2,10,10,42,(2, 3, 71),(0,1,1,0,1,0,1,0,1,0)
#define PPUTLU_425 424,426,8,212,850,0,20,401,176,1,1,1,9,9,41,(5, 5, 17),(0,1,1,0,1,0,1,0,0,1)
#define PPUTLU_424 423,425,8,212,848,0,20,576,175,0,0,0,8,8,40,(2, 2, 2, 53),(0,1,1,0,1,0,1,0,0,0)
#define PPUTLU_423 422,424,8,211,846,0,20,753,174,1,3,7,7,7,39,(3, 3, 47),(0,1,1,0,1,0,0,1,1,1)
#define PPUTLU_422 421,423,8,211,844,0,20,932,173,0,2,6,6,6,38,(2, 211),(0,1,1,0,1,0,0,1,1,0)
#define PPUTLU_421 420,422,8,210,842,0,20,89,173,1,1,5,5,5,37,(),(0,1,1,0,1,0,0,1,0,1)
#define PPUTLU_420 419,421,8,210,840,0,20,272,172,0,0,4,4,4,36,(2, 2, 3, 5, 7),(0,1,1,0,1,0,0,1,0,0)
#define PPUTLU_419 418,420,8,209,838,0,20,457,171,1,3,3,3,3,35,(),(0,1,1,0,1,0,0,0,1,1)
#define PPUTLU_418 417,419,8,209,836,0,20,644,170,0,2,2,2,2,34,(2, 11, 19),(0,1,1,0,1,0,0,0,1,0)
#define PPUTLU_417 416,418,8,208,834,0,20,833,169,1,1,1,1,1,33,(3, 139),(0,1,1,0,1,0,0,0,0,1)
#define PPUTLU_416 415,417,8,208,832,0,20,0,169,0,0,0,0,0,32,(2, 2, 2, 2, 2, 13),(0,1,1,0,1,0,0,0,0,0)
#define PPUTLU_415 414,416,8,207,830,0,20,193,168,1,3,7,15,31,31,(5, 83),(0,1,1,0,0,1,1,1,1,1)
#define PPUTLU_414 413,415,8,207,828,0,20,388,167,0,2,6,14,30,30,(2, 3, 3, 23),(0,1,1,0,0,1,1,1,1,0)
#define PPUTLU_413 412,414,8,206,826,0,20,585,166,1,1,5,13,29,29,(7, 59),(0,1,1,0,0,1,1,1,0,1)
#define PPUTLU_412 411,413,8,206,824,0,20,784,165,0,0,4,12,28,28,(2, 2, 103),(0,1,1,0,0,1,1,1,0,0)
#define PPUTLU_411 410,412,8,205,822,0,20,985,164,1,3,3,11,27,27,(3, 137),(0,1,1,0,0,1,1,0,1,1)
#define PPUTLU_410 409,411,8,205,820,0,20,164,164,0,2,2,10,26,26,(2, 5, 41),(0,1,1,0,0,1,1,0,1,0)
#define PPUTLU_409 408,410,8,204,818,0,20,369,163,1,1,1,9,25,25,(),(0,1,1,0,0,1,1,0,0,1)
#define PPUTLU_408 407,409,8,204,816,0,20,576,162,0,0,0,8,24,24,(2, 2, 2, 3, 17),(0,1,1,0,0,1,1,0,0,0)
#define PPUTLU_407 406,408,8,203,814,0,20,785,161,1,3,7,7,23,23,(11, 37),(0,1,1,0,0,1,0,1,1,1)
#define PPUTLU_406 405,407,8,203,812,0,20,996,160,0,2,6,6,22,22,(2, 7, 29),(0,1,1,0,0,1,0,1,1,0)
#define PPUTLU_405 404,406,8,202,810,0,20,185,160,1,1,5,5,21,21,(3, 3, 3, 3, 5),(0,1,1,0,0,1,0,1,0,1)
#define PPUTLU_404 403,405,8,202,808,0,20,400,159,0,0,4,4,20,20,(2, 2, 101),(0,1,1,0,0,1,0,1,0,0)
#define PPUTLU_403 402,404,8,201,806,0,20,617,158,1,3,3,3,19,19,(13, 31),(0,1,1,0,0,1,0,0,1,1)
#define PPUTLU_402 401,403,8,201,804,0,20,836,157,0,2,2,2,18,18,(2, 3, 67),(0,1,1,0,0,1,0,0,1,0)
#define PPUTLU_401 400,402,8,200,802,0,20,33,157,1,1,1,1,17,17,(),(0,1,1,0,0,1,0,0,0,1)
#define PPUTLU_400 399,401,8,200,800,0,20,256,156,0,0,0,0,16,16,(2, 2, 2, 2, 5, 5),(0,1,1,0,0,1,0,0,0,0)
#define PPUTLU_399 398,400,8,199,798,0,19,481,155,1,3,7,15,15,15,(3, 7, 19),(0,1,1,0,0,0,1,1,1,1)
#define PPUTLU_398 397,399,8,199,796,0,19,708,154,0,2,6,14,14,14,(2, 199),(0,1,1,0,0,0,1,1,1,0)
#define PPUTLU_397 396,398,8,198,794,0,19,937,153,1,1,5,13,13,13,(),(0,1,1,0,0,0,1,1,0,1)
#define PPUTLU_396 395,397,8,198,792,0,19,144,153,0,0,4,12,12,12,(2, 2, 3, 3, 11),(0,1,1,0,0,0,1,1,0,0)
#define PPUTLU_395 394,396,8,197,790,0,19,377,152,1,3,3,11,11,11,(5, 79),(0,1,1,0,0,0,1,0,1,1)
#define PPUTLU_394 393,395,8,197,788,0,19,612,151,0,2,2,10,10,10,(2, 197),(0,1,1,0,0,0,1,0,1,0)
#define PPUTLU_393 392,394,8,196,786,0,19,849,150,1,1,1,9,9,9,(3, 131),(0,1,1,0,0,0,1,0,0,1)
#define PPUTLU_392 391,393,8,196,784,0,19,64,150,0,0,0,8,8,8,(2, 2, 2, 7, 7),(0,1,1,0,0,0,1,0,0,0)
#define PPUTLU_391 390,392,8,195,782,0,19,305,149,1,3,7,7,7,7,(17, 23),(0,1,1,0,0,0,0,1,1,1)
#define PPUTLU_390 389,391,8,195,780,0,19,548,148,0,2,6,6,6,6,(2, 3, 5, 13),(0,1,1,0,0,0,0,1,1,0)
#define PPUTLU_389 388,390,8,194,778,0,19,793,147,1,1,5,5,5,5,(),(0,1,1,0,0,0,0,1,0,1)
#define PPUTLU_388 387,389,8,194,776,0,19,16,147,0,0,4,4,4,4,(2, 2, 97),(0,1,1,0,0,0,0,1,0,0)
#define PPUTLU_387 386,388,8,193,774,0,19,265,146,1,3,3,3,3,3,(3, 3, 43),(0,1,1,0,0,0,0,0,1,1)
#define PPUTLU_386 385,387,8,193,772,0,19,516,145,0,2,2,2,2,2,(2, 193),(0,1,1,0,0,0,0,0,1,0)
#define PPUTLU_385 384,386,8,192,770,0,19,769,144,1,1,1,1,1,1,(5, 7, 11),(0,1,1,0,0,0,0,0,0,1)
#define PPUTLU_384 383,385,8,192,768,0,19,0,144,0,0,0,0,0,0,(2, 2, 2, 2, 2, 2, 2, 3),(0,1,1,0,0,0,0,0,0,0)
#define PPUTLU_383 382,384,8,191,766,0,19,257,143,1,3,7,15,31,63,(),(0,1,0,1,1,1,1,1,1,1)
#define PPUTLU_382 381,383,8,191,764,0,19,516,142,0,2,6,14,30,62,(2, 191),(0,1,0,1,1,1,1,1,1,0)
#define PPUTLU_381 380,382,8,190,762,0,19,777,141,1,1,5,13,29,61,(3, 127),(0,1,0,1,1,1,1,1,0,1)
#define PPUTLU_380 379,381,8,190,760,0,19,16,141,0,0,4,12,28,60,(2, 2, 5, 19),(0,1,0,1,1,1,1,1,0,0)
#define PPUTLU_379 378,380,8,189,758,0,19,281,140,1,3,3,11,27,59,(),(0,1,0,1,1,1,1,0,1,1)
#define PPUTLU_378 377,379,8,189,756,0,19,548,139,0,2,2,10,26,58,(2, 3, 3, 3, 7),(0,1,0,1,1,1,1,0,1,0)
#define PPUTLU_377 376,378,8,188,754,0,19,817,138,1,1,1,9,25,57,(13, 29),(0,1,0,1,1,1,1,0,0,1)
#define PPUTLU_376 375,377,8,188,752,0,19,64,138,0,0,0,8,24,56,(2, 2, 2, 47),(0,1,0,1,1,1,1,0,0,0)
#define PPUTLU_375 374,376,8,187,750,0,19,337,137,1,3,7,7,23,55,(3, 5, 5, 5),(0,1,0,1,1,1,0,1,1,1)
#define PPUTLU_374 373,375,8,187,748,0,19,612,136,0,2,6,6,22,54,(2, 11, 17),(0,1,0,1,1,1,0,1,1,0)
#define PPUTLU_373 372,374,8,186,746,0,19,889,135,1,1,5,5,21,53,(),(0,1,0,1,1,1,0,1,0,1)
#define PPUTLU_372 371,373,8,186,744,0,19,144,135,0,0,4,4,20,52,(2, 2, 3, 31),(0,1,0,1,1,1,0,1,0,0)
#define PPUTLU_371 370,372,8,185,742,0,19,425,134,1,3,3,3,19,51,(7, 53),(0,1,0,1,1,1,0,0,1,1)
#define PPUTLU_370 369,371,8,185,740,0,19,708,133,0,2,2,2,18,50,(2, 5, 37),(0,1,0,1,1,1,0,0,1,0)
#define PPUTLU_369 368,370,8,184,738,0,19,993,132,1,1,1,1,17,49,(3, 3, 41),(0,1,0,1,1,1,0,0,0,1)
#define PPUTLU_368 367,369,8,184,736,0,19,256,132,0,0,0,0,16,48,(2, 2, 2, 2, 23),(0,1,0,1,1,1,0,0,0,0)
#define PPUTLU_367 366,368,8,183,734,0,19,545,131,1,3,7,15,15,47,(),(0,1,0,1,1,0,1,1,1,1)
#define PPUTLU_366 365,367,8,183,732,0,19,836,130,0,2,6,14,14,46,(2, 3, 61),(0,1,0,1,1,0,1,1,1,0)
#define PPUTLU_365 364,366,8,182,730,0,19,105,130,1,1,5,13,13,45,(5, 73),(0,1,0,1,1,0,1,1,0,1)
#define PPUTLU_364 363,365,8,182,728,0,19,400,129,0,0,4,12,12,44,(2, 2, 7, 13),(0,1,0,1,1,0,1,1,0,0)
#define PPUTLU_363 362,364,8,181,726,0,19,697,128,1,3,3,11,11,43,(3, 11, 11),(0,1,0,1,1,0,1,0,1,1)
#define PPUTLU_362 361,363,8,181,724,0,19,996,127,0,2,2,10,10,42,(2, 181),(0,1,0,1,1,0,1,0,1,0)
#define PPUTLU_361 360,362,8,180,722,0,19,273,127,1,1,1,9,9,41,(19, 19),(0,1,0,1,1,0,1,0,0,1)
#define PPUTLU_360 359,361,8,180,720,0,18,576,126,0,0,0,8,8,40,(2, 2, 2, 3, 3, 5),(0,1,0,1,1,0,1,0,0,0)
#define PPUTLU_359 358,360,8,179,718,0,18,881,125,1,3,7,7,7,39,(),(0,1,0,1,1,0,0,1,1,1)
#define PPUTLU_358 357,359,8,179,716,0,18,164,125,0,2,6,6,6,38,(2, 179),(0,1,0,1,1,0,0,1,1,0)
#define PPUTLU_357 356,358,8,178,714,0,18,473,124,1,1,5,5,5,37,(3, 7, 17),(0,1,0,1,1,0,0,1,0,1)
#define PPUTLU_356 355,357,8,178,712,0,18,784,123,0,0,4,4,4,36,(2, 2, 89),(0,1,0,1,1,0,0,1,0,0)
#define PPUTLU_355 354,356,8,177,710,0,18,73,123,1,3,3,3,3,35,(5, 71),(0,1,0,1,1,0,0,0,1,1)
#define PPUTLU_354 353,355,8,177,708,0,18,388,122,0,2,2,2,2,34,(2, 3, 59),(0,1,0,1,1,0,0,0,1,0)
#define PPUTLU_353 352,354,8,176,706,0,18,705,121,1,1,1,1,1,33,(),(0,1,0,1,1,0,0,0,0,1)
#define PPUTLU_352 351,353,8,176,704,0,18,0,121,0,0,0,0,0,32,(2, 2, 2, 2, 2, 11),(0,1,0,1,1,0,0,0,0,0)
#define PPUTLU_351 350,352,8,175,702,0,18,321,120,1,3,7,15,31,31,(3, 3, 3, 13),(0,1,0,1,0,1,1,1,1,1)
#define PPUTLU_350 349,351,8,175,700,0,18,644,119,0,2,6,14,30,30,(2, 5, 5, 7),(0,1,0,1,0,1,1,1,1,0)
#define PPUTLU_349 348,350,8,174,698,0,18,969,118,1,1,5,13,29,29,(),(0,1,0,1,0,1,1,1,0,1)
#define PPUTLU_348 347,349,8,174,696,0,18,272,118,0,0,4,12,28,28,(2, 2, 3, 29),(0,1,0,1,0,1,1,1,0,0)
#define PPUTLU_347 346,348,8,173,694,0,18,601,117,1,3,3,11,27,27,(),(0,1,0,1,0,1,1,0,1,1)
#define PPUTLU_346 345,347,8,173,692,0,18,932,116,0,2,2,10,26,26,(2, 173),(0,1,0,1,0,1,1,0,1,0)
#define PPUTLU_345 344,346,8,172,690,0,18,241,116,1,1,1,9,25,25,(3, 5, 23),(0,1,0,1,0,1,1,0,0,1)
#define PPUTLU_344 343,345,8,172,688,0,18,576,115,0,0,0,8,24,24,(2, 2, 2, 43),(0,1,0,1,0,1,1,0,0,0)
#define PPUTLU_343 342,344,8,171,686,0,18,913,114,1,3,7,7,23,23,(7, 7, 7),(0,1,0,1,0,1,0,1,1,1)
#define PPUTLU_342 341,343,8,171,684,0,18,228,114,0,2,6,6,22,22,(2, 3, 3, 19),(0,1,0,1,0,1,0,1,1,0)
#define PPUTLU_341 340,342,8,170,682,0,18,569,113,1,1,5,5,21,21,(11, 31),(0,1,0,1,0,1,0,1,0,1)
#define PPUTLU_340 339,341,8,170,680,0,18,912,112,0,0,4,4,20,20,(2, 2, 5, 17),(0,1,0,1,0,1,0,1,0,0)
#define PPUTLU_339 338,340,8,169,678,0,18,233,112,1,3,3,3,19,19,(3, 113),(0,1,0,1,0,1,0,0,1,1)
#define PPUTLU_338 337,339,8,169,676,0,18,580,111,0,2,2,2,18,18,(2, 13, 13),(0,1,0,1,0,1,0,0,1,0)
#define PPUTLU_337 336,338,8,168,674,0,18,929,110,1,1,1,1,17,17,(),(0,1,0,1,0,1,0,0,0,1)
#define PPUTLU_336 335,337,8,168,672,0,18,256,110,0,0,0,0,16,16,(2, 2, 2, 2, 3, 7),(0,1,0,1,0,1,0,0,0,0)
#define PPUTLU_335 334,336,8,167,670,0,18,609,109,1,3,7,15,15,15,(5, 67),(0,1,0,1,0,0,1,1,1,1)
#define PPUTLU_334 333,335,8,167,668,0,18,964,108,0,2,6,14,14,14,(2, 167),(0,1,0,1,0,0,1,1,1,0)
#define PPUTLU_333 332,334,8,166,666,0,18,297,108,1,1,5,13,13,13,(3, 3, 37),(0,1,0,1,0,0,1,1,0,1)
#define PPUTLU_332 331,333,8,166,664,0,18,656,107,0,0,4,12,12,12,(2, 2, 83),(0,1,0,1,0,0,1,1,0,0)
#define PPUTLU_331 330,332,8,165,662,0,18,1017,106,1,3,3,11,11,11,(),(0,1,0,1,0,0,1,0,1,1)
#define PPUTLU_330 329,331,8,165,660,0,18,356,106,0,2,2,10,10,10,(2, 3, 5, 11),(0,1,0,1,0,0,1,0,1,0)
#define PPUTLU_329 328,330,8,164,658,0,18,721,105,1,1,1,9,9,9,(7, 47),(0,1,0,1,0,0,1,0,0,1)
#define PPUTLU_328 327,329,8,164,656,0,18,64,105,0,0,0,8,8,8,(2, 2, 2, 41),(0,1,0,1,0,0,1,0,0,0)
#define PPUTLU_327 326,328,8,163,654,0,18,433,104,1,3,7,7,7,7,(3, 109),(0,1,0,1,0,0,0,1,1,1)
#define PPUTLU_326 325,327,8,163,652,0,18,804,103,0,2,6,6,6,6,(2, 163),(0,1,0,1,0,0,0,1,1,0)
#define PPUTLU_325 324,326,8,162,650,0,18,153,103,1,1,5,5,5,5,(5, 5, 13),(0,1,0,1,0,0,0,1,0,1)
#define PPUTLU_324 323,325,8,162,648,0,18,528,102,0,0,4,4,4,4,(2, 2, 3, 3, 3, 3),(0,1,0,1,0,0,0,1,0,0)
#define PPUTLU_323 322,324,8,161,646,0,17,905,101,1,3,3,3,3,3,(17, 19),(0,1,0,1,0,0,0,0,1,1)
#define PPUTLU_322 321,323,8,161,644,0,17,260,101,0,2,2,2,2,2,(2, 7, 23),(0,1,0,1,0,0,0,0,1,0)
#define PPUTLU_321 320,322,8,160,642,0,17,641,100,1,1,1,1,1,1,(3, 107),(0,1,0,1,0,0,0,0,0,1)
#define PPUTLU_320 319,321,8,160,640,0,17,0,100,0,0,0,0,0,0,(2, 2, 2, 2, 2, 2, 5),(0,1,0,1,0,0,0,0,0,0)
#define PPUTLU_319 318,320,8,159,638,0,17,385,99,1,3,7,15,31,63,(11, 29),(0,1,0,0,1,1,1,1,1,1)
#define PPUTLU_318 317,319,8,159,636,0,17,772,98,0,2,6,14,30,62,(2, 3, 53),(0,1,0,0,1,1,1,1,1,0)
#define PPUTLU_317 316,318,8,158,634,0,17,137,98,1,1,5,13,29,61,(),(0,1,0,0,1,1,1,1,0,1)
#define PPUTLU_316 315,317,8,158,632,0,17,528,97,0,0,4,12,28,60,(2, 2, 79),(0,1,0,0,1,1,1,1,0,0)
#define PPUTLU_315 314,316,8,157,630,0,17,921,96,1,3,3,11,27,59,(3, 3, 5, 7),(0,1,0,0,1,1,1,0,1,1)
#define PPUTLU_314 313,315,8,157,628,0,17,292,96,0,2,2,10,26,58,(2, 157),(0,1,0,0,1,1,1,0,1,0)
#define PPUTLU_313 312,314,8,156,626,0,17,689,95,1,1,1,9,25,57,(),(0,1,0,0,1,1,1,0,0,1)
#define PPUTLU_312 311,313,8,156,624,0,17,64,95,0,0,0,8,24,56,(2, 2, 2, 3, 13),(0,1,0,0,1,1,1,0,0,0)
#define PPUTLU_311 310,312,8,155,622,0,17,465,94,1,3,7,7,23,55,(),(0,1,0,0,1,1,0,1,1,1)
#define PPUTLU_310 309,311,8,155,620,0,17,868,93,0,2,6,6,22,54,(2, 5, 31),(0,1,0,0,1,1,0,1,1,0)
#define PPUTLU_309 308,310,8,154,618,0,17,249,93,1,1,5,5,21,53,(3, 103),(0,1,0,0,1,1,0,1,0,1)
#define PPUTLU_308 307,309,8,154,616,0,17,656,92,0,0,4,4,20,52,(2, 2, 7, 11),(0,1,0,0,1,1,0,1,0,0)
#define PPUTLU_307 306,308,8,153,614,0,17,41,92,1,3,3,3,19,51,(),(0,1,0,0,1,1,0,0,1,1)
#define PPUTLU_306 305,307,8,153,612,0,17,452,91,0,2,2,2,18,50,(2, 3, 3, 17),(0,1,0,0,1,1,0,0,1,0)
#define PPUTLU_305 304,306,8,152,610,0,17,865,90,1,1,1,1,17,49,(5, 61),(0,1,0,0,1,1,0,0,0,1)
#define PPUTLU_304 303,305,8,152,608,0,17,256,90,0,0,0,0,16,48,(2, 2, 2, 2, 19),(0,1,0,0,1,1,0,0,0,0)
#define PPUTLU_303 302,304,8,151,606,0,17,673,89,1,3,7,15,15,47,(3, 101),(0,1,0,0,1,0,1,1,1,1)
#define PPUTLU_302 301,303,8,151,604,0,17,68,89,0,2,6,14,14,46,(2, 151),(0,1,0,0,1,0,1,1,1,0)
#define PPUTLU_301 300,302,8,150,602,0,17,489,88,1,1,5,13,13,45,(7, 43),(0,1,0,0,1,0,1,1,0,1)
#define PPUTLU_300 299,301,8,150,600,0,17,912,87,0,0,4,12,12,44,(2, 2, 3, 5, 5),(0,1,0,0,1,0,1,1,0,0)
#define PPUTLU_299 298,300,8,149,598,0,17,313,87,1,3,3,11,11,43,(13, 23),(0,1,0,0,1,0,1,0,1,1)
#define PPUTLU_298 297,299,8,149,596,0,17,740,86,0,2,2,10,10,42,(2, 149),(0,1,0,0,1,0,1,0,1,0)
#define PPUTLU_297 296,298,8,148,594,0,17,145,86,1,1,1,9,9,41,(3, 3, 3, 11),(0,1,0,0,1,0,1,0,0,1)
#define PPUTLU_296 295,297,8,148,592,0,17,576,85,0,0,0,8,8,40,(2, 2, 2, 37),(0,1,0,0,1,0,1,0,0,0)
#define PPUTLU_295 294,296,8,147,590,0,17,1009,84,1,3,7,7,7,39,(5, 59),(0,1,0,0,1,0,0,1,1,1)
#define PPUTLU_294 293,295,8,147,588,0,17,420,84,0,2,6,6,6,38,(2, 3, 7, 7),(0,1,0,0,1,0,0,1,1,0)
#define PPUTLU_293 292,294,8,146,586,0,17,857,83,1,1,5,5,5,37,(),(0,1,0,0,1,0,0,1,0,1)
#define PPUTLU_292 291,293,8,146,584,0,17,272,83,0,0,4,4,4,36,(2, 2, 73),(0,1,0,0,1,0,0,1,0,0)
#define PPUTLU_291 290,292,8,145,582,0,17,713,82,1,3,3,3,3,35,(3, 97),(0,1,0,0,1,0,0,0,1,1)
#define PPUTLU_290 289,291,8,145,580,0,17,132,82,0,2,2,2,2,34,(2, 5, 29),(0,1,0,0,1,0,0,0,1,0)
#define PPUTLU_289 288,290,8,144,578,0,17,577,81,1,1,1,1,1,33,(17, 17),(0,1,0,0,1,0,0,0,0,1)
#define PPUTLU_288 287,289,8,144,576,0,16,0,81,0,0,0,0,0,32,(2, 2, 2, 2, 2, 3, 3),(0,1,0,0,1,0,0,0,0,0)
#define PPUTLU_287 286,288,8,143,574,0,16,449,80,1,3,7,15,31,31,(7, 41),(0,1,0,0,0,1,1,1,1,1)
#define PPUTLU_286 285,287,8,143,572,0,16,900,79,0,2,6,14,30,30,(2, 11, 13),(0,1,0,0,0,1,1,1,1,0)
#define PPUTLU_285 284,286,8,142,570,0,16,329,79,1,1,5,13,29,29,(3, 5, 19),(0,1,0,0,0,1,1,1,0,1)
#define PPUTLU_284 283,285,8,142,568,0,16,784,78,0,0,4,12,28,28,(2, 2, 71),(0,1,0,0,0,1,1,1,0,0)
#define PPUTLU_283 282,284,8,141,566,0,16,217,78,1,3,3,11,27,27,(),(0,1,0,0,0,1,1,0,1,1)
#define PPUTLU_282 281,283,8,141,564,0,16,676,77,0,2,2,10,26,26,(2, 3, 47),(0,1,0,0,0,1,1,0,1,0)
#define PPUTLU_281 280,282,8,140,562,0,16,113,77,1,1,1,9,25,25,(),(0,1,0,0,0,1,1,0,0,1)
#define PPUTLU_280 279,281,8,140,560,0,16,576,76,0,0,0,8,24,24,(2, 2, 2, 5, 7),(0,1,0,0,0,1,1,0,0,0)
#define PPUTLU_279 278,280,8,139,558,0,16,17,76,1,3,7,7,23,23,(3, 3, 31),(0,1,0,0,0,1,0,1,1,1)
#define PPUTLU_278 277,279,8,139,556,0,16,484,75,0,2,6,6,22,22,(2, 139),(0,1,0,0,0,1,0,1,1,0)
#define PPUTLU_277 276,278,8,138,554,0,16,953,74,1,1,5,5,21,21,(),(0,1,0,0,0,1,0,1,0,1)
#define PPUTLU_276 275,277,8,138,552,0,16,400,74,0,0,4,4,20,20,(2, 2, 3, 23),(0,1,0,0,0,1,0,1,0,0)
#define PPUTLU_275 274,276,8,137,550,0,16,873,73,1,3,3,3,19,19,(5, 5, 11),(0,1,0,0,0,1,0,0,1,1)
#define PPUTLU_274 273,275,8,137,548,0,16,324,73,0,2,2,2,18,18,(2, 137),(0,1,0,0,0,1,0,0,1,0)
#define PPUTLU_273 272,274,8,136,546,0,16,801,72,1,1,1,1,17,17,(3, 7, 13),(0,1,0,0,0,1,0,0,0,1)
#define PPUTLU_272 271,273,8,136,544,0,16,256,72,0,0,0,0,16,16,(2, 2, 2, 2, 17),(0,1,0,0,0,1,0,0,0,0)
#define PPUTLU_271 270,272,8,135,542,0,16,737,71,1,3,7,15,15,15,(),(0,1,0,0,0,0,1,1,1,1)
#define PPUTLU_270 269,271,8,135,540,0,16,196,71,0,2,6,14,14,14,(2, 3, 3, 3, 5),(0,1,0,0,0,0,1,1,1,0)
#define PPUTLU_269 268,270,8,134,538,0,16,681,70,1,1,5,13,13,13,(),(0,1,0,0,0,0,1,1,0,1)
#define PPUTLU_268 267,269,8,134,536,0,16,144,70,0,0,4,12,12,12,(2, 2, 67),(0,1,0,0,0,0,1,1,0,0)
#define PPUTLU_267 266,268,8,133,534,0,16,633,69,1,3,3,11,11,11,(3, 89),(0,1,0,0,0,0,1,0,1,1)
#define PPUTLU_266 265,267,8,133,532,0,16,100,69,0,2,2,10,10,10,(2, 7, 19),(0,1,0,0,0,0,1,0,1,0)
#define PPUTLU_265 264,266,8,132,530,0,16,593,68,1,1,1,9,9,9,(5, 53),(0,1,0,0,0,0,1,0,0,1)
#define PPUTLU_264 263,265,8,132,528,0,16,64,68,0,0,0,8,8,8,(2, 2, 2, 3, 11),(0,1,0,0,0,0,1,0,0,0)
#define PPUTLU_263 262,264,8,131,526,0,16,561,67,1,3,7,7,7,7,(),(0,1,0,0,0,0,0,1,1,1)
#define PPUTLU_262 261,263,8,131,524,0,16,36,67,0,2,6,6,6,6,(2, 131),(0,1,0,0,0,0,0,1,1,0)
#define PPUTLU_261 260,262,8,130,522,0,16,537,66,1,1,5,5,5,5,(3, 3, 29),(0,1,0,0,0,0,0,1,0,1)
#define PPUTLU_260 259,261,8,130,520,0,16,16,66,0,0,4,4,4,4,(2, 2, 5, 13),(0,1,0,0,0,0,0,1,0,0)
#define PPUTLU_259 258,260,8,129,518,0,16,521,65,1,3,3,3,3,3,(7, 37),(0,1,0,0,0,0,0,0,1,1)
#define PPUTLU_258 257,259,8,129,516,0,16,4,65,0,2,2,2,2,2,(2, 3, 43),(0,1,0,0,0,0,0,0,1,0)
#define PPUTLU_257 256,258,8,128,514,0,16,513,64,1,1,1,1,1,1,(),(0,1,0,0,0,0,0,0,0,1)
#define PPUTLU_256 255,257,8,128,512,0,16,0,64,0,0,0,0,0,0,(2, 2, 2, 2, 2, 2, 2, 2),(0,1,0,0,0,0,0,0,0,0)
#define PPUTLU_255 254,256,7,127,510,0,15,513,63,1,3,7,15,31,63,(3, 5, 17),(0,0,1,1,1,1,1,1,1,1)
#define PPUTLU_254 253,255,7,127,508,0,15,4,63,0,2,6,14,30,62,(2, 127),(0,0,1,1,1,1,1,1,1,0)
#define PPUTLU_253 252,254,7,126,506,0,15,521,62,1,1,5,13,29,61,(11, 23),(0,0,1,1,1,1,1,1,0,1)
#define PPUTLU_252 251,253,7,126,504,0,15,16,62,0,0,4,12,28,60,(2, 2, 3, 3, 7),(0,0,1,1,1,1,1,1,0,0)
#define PPUTLU_251 250,252,7,125,502,0,15,537,61,1,3,3,11,27,59,(),(0,0,1,1,1,1,1,0,1,1)
#define PPUTLU_250 249,251,7,125,500,0,15,36,61,0,2,2,10,26,58,(2, 5, 5, 5),(0,0,1,1,1,1,1,0,1,0)
#define PPUTLU_249 248,250,7,124,498,0,15,561,60,1,1,1,9,25,57,(3, 83),(0,0,1,1,1,1,1,0,0,1)
#define PPUTLU_248 247,249,7,124,496,0,15,64,60,0,0,0,8,24,56,(2, 2, 2, 31),(0,0,1,1,1,1,1,0,0,0)
#define PPUTLU_247 246,248,7,123,494,0,15,593,59,1,3,7,7,23,55,(13, 19),(0,0,1,1,1,1,0,1,1,1)
#define PPUTLU_246 245,247,7,123,492,0,15,100,59,0,2,6,6,22,54,(2, 3, 41),(0,0,1,1,1,1,0,1,1,0)
#define PPUTLU_245 244,246,7,122,490,0,15,633,58,1,1,5,5,21,53,(5, 7, 7),(0,0,1,1,1,1,0,1,0,1)
#define PPUTLU_244 243,245,7,122,488,0,15,144,58,0,0,4,4,20,52,(2, 2, 61),(0,0,1,1,1,1,0,1,0,0)
#define PPUTLU_243 242,244,7,121,486,0,15,681,57,1,3,3,3,19,51,(3, 3, 3, 3, 3),(0,0,1,1,1,1,0,0,1,1)
#define PPUTLU_242 241,243,7,121,484,0,15,196,57,0,2,2,2,18,50,(2, 11, 11),(0,0,1,1,1,1,0,0,1,0)
#define PPUTLU_241 240,242,7,120,482,0,15,737,56,1,1,1,1,17,49,(),(0,0,1,1,1,1,0,0,0,1)
#define PPUTLU_240 239,241,7,120,480,0,15,256,56,0,0,0,0,16,48,(2, 2, 2, 2, 3, 5),(0,0,1,1,1,1,0,0,0,0)
#define PPUTLU_239 238,240,7,119,478,0,15,801,55,1,3,7,15,15,47,(),(0,0,1,1,1,0,1,1,1,1)
#define PPUTLU_238 237,239,7,119,476,0,15,324,55,0,2,6,14,14,46,(2, 7, 17),(0,0,1,1,1,0,1,1,1,0)
#define PPUTLU_237 236,238,7,118,474,0,15,873,54,1,1,5,13,13,45,(3, 79),(0,0,1,1,1,0,1,1,0,1)
#define PPUTLU_236 235,237,7,118,472,0,15,400,54,0,0,4,12,12,44,(2, 2, 59),(0,0,1,1,1,0,1,1,0,0)
#define PPUTLU_235 234,236,7,117,470,0,15,953,53,1,3,3,11,11,43,(5, 47),(0,0,1,1,1,0,1,0,1,1)
#define PPUTLU_234 233,235,7,117,468,0,15,484,53,0,2,2,10,10,42,(2, 3, 3, 13),(0,0,1,1,1,0,1,0,1,0)
#define PPUTLU_233 232,234,7,116,466,0,15,17,53,1,1,1,9,9,41,(),(0,0,1,1,1,0,1,0,0,1)
#define PPUTLU_232 231,233,7,116,464,0,15,576,52,0,0,0,8,8,40,(2, 2, 2, 29),(0,0,1,1,1,0,1,0,0,0)
#define PPUTLU_231 230,232,7,115,462,0,15,113,52,1,3,7,7,7,39,(3, 7, 11),(0,0,1,1,1,0,0,1,1,1)
#define PPUTLU_230 229,231,7,115,460,0,15,676,51,0,2,6,6,6,38,(2, 5, 23),(0,0,1,1,1,0,0,1,1,0)
#define PPUTLU_229 228,230,7,114,458,0,15,217,51,1,1,5,5,5,37,(),(0,0,1,1,1,0,0,1,0,1)
#define PPUTLU_228 227,229,7,114,456,0,15,784,50,0,0,4,4,4,36,(2, 2, 3, 19),(0,0,1,1,1,0,0,1,0,0)
#define PPUTLU_227 226,228,7,113,454,0,15,329,50,1,3,3,3,3,35,(),(0,0,1,1,1,0,0,0,1,1)
#define PPUTLU_226 225,227,7,113,452,0,15,900,49,0,2,2,2,2,34,(2, 113),(0,0,1,1,1,0,0,0,1,0)
#define PPUTLU_225 224,226,7,112,450,0,15,449,49,1,1,1,1,1,33,(3, 3, 5, 5),(0,0,1,1,1,0,0,0,0,1)
#define PPUTLU_224 223,225,7,112,448,0,14,0,49,0,0,0,0,0,32,(2, 2, 2, 2, 2, 7),(0,0,1,1,1,0,0,0,0,0)
#define PPUTLU_223 222,224,7,111,446,0,14,577,48,1,3,7,15,31,31,(),(0,0,1,1,0,1,1,1,1,1)
#define PPUTLU_222 221,223,7,111,444,0,14,132,48,0,2,6,14,30,30,(2, 3, 37),(0,0,1,1,0,1,1,1,1,0)
#define PPUTLU_221 220,222,7,110,442,0,14,713,47,1,1,5,13,29,29,(13, 17),(0,0,1,1,0,1,1,1,0,1)
#define PPUTLU_220 219,221,7,110,440,0,14,272,47,0,0,4,12,28,28,(2, 2, 5, 11),(0,0,1,1,0,1,1,1,0,0)
#define PPUTLU_219 218,220,7,109,438,0,14,857,46,1,3,3,11,27,27,(3, 73),(0,0,1,1,0,1,1,0,1,1)
#define PPUTLU_218 217,219,7,109,436,0,14,420,46,0,2,2,10,26,26,(2, 109),(0,0,1,1,0,1,1,0,1,0)
#define PPUTLU_217 216,218,7,108,434,0,14,1009,45,1,1,1,9,25,25,(7, 31),(0,0,1,1,0,1,1,0,0,1)
#define PPUTLU_216 215,217,7,108,432,0,14,576,45,0,0,0,8,24,24,(2, 2, 2, 3, 3, 3),(0,0,1,1,0,1,1,0,0,0)
#define PPUTLU_215 214,216,7,107,430,0,14,145,45,1,3,7,7,23,23,(5, 43),(0,0,1,1,0,1,0,1,1,1)
#define PPUTLU_214 213,215,7,107,428,0,14,740,44,0,2,6,6,22,22,(2, 107),(0,0,1,1,0,1,0,1,1,0)
#define PPUTLU_213 212,214,7,106,426,0,14,313,44,1,1,5,5,21,21,(3, 71),(0,0,1,1,0,1,0,1,0,1)
#define PPUTLU_212 211,213,7,106,424,0,14,912,43,0,0,4,4,20,20,(2, 2, 53),(0,0,1,1,0,1,0,1,0,0)
#define PPUTLU_211 210,212,7,105,422,0,14,489,43,1,3,3,3,19,19,(),(0,0,1,1,0,1,0,0,1,1)
#define PPUTLU_210 209,211,7,105,420,0,14,68,43,0,2,2,2,18,18,(2, 3, 5, 7),(0,0,1,1,0,1,0,0,1,0)
#define PPUTLU_209 208,210,7,104,418,0,14,673,42,1,1,1,1,17,17,(11, 19),(0,0,1,1,0,1,0,0,0,1)
#define PPUTLU_208 207,209,7,104,416,0,14,256,42,0,0,0,0,16,16,(2, 2, 2, 2, 13),(0,0,1,1,0,1,0,0,0,0)
#define PPUTLU_207 206,208,7,103,414,0,14,865,41,1,3,7,15,15,15,(3, 3, 23),(0,0,1,1,0,0,1,1,1,1)
#define PPUTLU_206 205,207,7,103,412,0,14,452,41,0,2,6,14,14,14,(2, 103),(0,0,1,1,0,0,1,1,1,0)
#define PPUTLU_205 204,206,7,102,410,0,14,41,41,1,1,5,13,13,13,(5, 41),(0,0,1,1,0,0,1,1,0,1)
#define PPUTLU_204 203,205,7,102,408,0,14,656,40,0,0,4,12,12,12,(2, 2, 3, 17),(0,0,1,1,0,0,1,1,0,0)
#define PPUTLU_203 202,204,7,101,406,0,14,249,40,1,3,3,11,11,11,(7, 29),(0,0,1,1,0,0,1,0,1,1)
#define PPUTLU_202 201,203,7,101,404,0,14,868,39,0,2,2,10,10,10,(2, 101),(0,0,1,1,0,0,1,0,1,0)
#define PPUTLU_201 200,202,7,100,402,0,14,465,39,1,1,1,9,9,9,(3, 67),(0,0,1,1,0,0,1,0,0,1)
#define PPUTLU_200 199,201,7,100,400,0,14,64,39,0,0,0,8,8,8,(2, 2, 2, 5, 5),(0,0,1,1,0,0,1,0,0,0)
#define PPUTLU_199 198,200,7,99,398,0,14,689,38,1,3,7,7,7,7,(),(0,0,1,1,0,0,0,1,1,1)
#define PPUTLU_198 197,199,7,99,396,0,14,292,38,0,2,6,6,6,6,(2, 3, 3, 11),(0,0,1,1,0,0,0,1,1,0)
#define PPUTLU_197 196,198,7,98,394,0,14,921,37,1,1,5,5,5,5,(),(0,0,1,1,0,0,0,1,0,1)
#define PPUTLU_196 195,197,7,98,392,0,14,528,37,0,0,4,4,4,4,(2, 2, 7, 7),(0,0,1,1,0,0,0,1,0,0)
#define PPUTLU_195 194,196,7,97,390,0,13,137,37,1,3,3,3,3,3,(3, 5, 13),(0,0,1,1,0,0,0,0,1,1)
#define PPUTLU_194 193,195,7,97,388,0,13,772,36,0,2,2,2,2,2,(2, 97),(0,0,1,1,0,0,0,0,1,0)
#define PPUTLU_193 192,194,7,96,386,0,13,385,36,1,1,1,1,1,1,(),(0,0,1,1,0,0,0,0,0,1)
#define PPUTLU_192 191,193,7,96,384,0,13,0,36,0,0,0,0,0,0,(2, 2, 2, 2, 2, 2, 3),(0,0,1,1,0,0,0,0,0,0)
#define PPUTLU_191 190,192,7,95,382,0,13,641,35,1,3,7,15,31,63,(),(0,0,1,0,1,1,1,1,1,1)
#define PPUTLU_190 189,191,7,95,380,0,13,260,35,0,2,6,14,30,62,(2, 5, 19),(0,0,1,0,1,1,1,1,1,0)
#define PPUTLU_189 188,190,7,94,378,0,13,905,34,1,1,5,13,29,61,(3, 3, 3, 7),(0,0,1,0,1,1,1,1,0,1)
#define PPUTLU_188 187,189,7,94,376,0,13,528,34,0,0,4,12,28,60,(2, 2, 47),(0,0,1,0,1,1,1,1,0,0)
#define PPUTLU_187 186,188,7,93,374,0,13,153,34,1,3,3,11,27,59,(11, 17),(0,0,1,0,1,1,1,0,1,1)
#define PPUTLU_186 185,187,7,93,372,0,13,804,33,0,2,2,10,26,58,(2, 3, 31),(0,0,1,0,1,1,1,0,1,0)
#define PPUTLU_185 184,186,7,92,370,0,13,433,33,1,1,1,9,25,57,(5, 37),(0,0,1,0,1,1,1,0,0,1)
#define PPUTLU_184 183,185,7,92,368,0,13,64,33,0,0,0,8,24,56,(2, 2, 2, 23),(0,0,1,0,1,1,1,0,0,0)
#define PPUTLU_183 182,184,7,91,366,0,13,721,32,1,3,7,7,23,55,(3, 61),(0,0,1,0,1,1,0,1,1,1)
#define PPUTLU_182 181,183,7,91,364,0,13,356,32,0,2,6,6,22,54,(2, 7, 13),(0,0,1,0,1,1,0,1,1,0)
#define PPUTLU_181 180,182,7,90,362,0,13,1017,31,1,1,5,5,21,53,(),(0,0,1,0,1,1,0,1,0,1)
#define PPUTLU_180 179,181,7,90,360,0,13,656,31,0,0,4,4,20,52,(2, 2, 3, 3, 5),(0,0,1,0,1,1,0,1,0,0)
#define PPUTLU_179 178,180,7,89,358,0,13,297,31,1,3,3,3,19,51,(),(0,0,1,0,1,1,0,0,1,1)
#define PPUTLU_178 177,179,7,89,356,0,13,964,30,0,2,2,2,18,50,(2, 89),(0,0,1,0,1,1,0,0,1,0)
#define PPUTLU_177 176,178,7,88,354,0,13,609,30,1,1,1,1,17,49,(3, 59),(0,0,1,0,1,1,0,0,0,1)
#define PPUTLU_176 175,177,7,88,352,0,13,256,30,0,0,0,0,16,48,(2, 2, 2, 2, 11),(0,0,1,0,1,1,0,0,0,0)
#define PPUTLU_175 174,176,7,87,350,0,13,929,29,1,3,7,15,15,47,(5, 5, 7),(0,0,1,0,1,0,1,1,1,1)
#define PPUTLU_174 173,175,7,87,348,0,13,580,29,0,2,6,14,14,46,(2, 3, 29),(0,0,1,0,1,0,1,1,1,0)
#define PPUTLU_173 172,174,7,86,346,0,13,233,29,1,1,5,13,13,45,(),(0,0,1,0,1,0,1,1,0,1)
#define PPUTLU_172 171,173,7,86,344,0,13,912,28,0,0,4,12,12,44,(2, 2, 43),(0,0,1,0,1,0,1,1,0,0)
#define PPUTLU_171 170,172,7,85,342,0,13,569,28,1,3,3,11,11,43,(3, 3, 19),(0,0,1,0,1,0,1,0,1,1)
#define PPUTLU_170 169,171,7,85,340,0,13,228,28,0,2,2,10,10,42,(2, 5, 17),(0,0,1,0,1,0,1,0,1,0)
#define PPUTLU_169 168,170,7,84,338,0,13,913,27,1,1,1,9,9,41,(13, 13),(0,0,1,0,1,0,1,0,0,1)
#define PPUTLU_168 167,169,7,84,336,0,12,576,27,0,0,0,8,8,40,(2, 2, 2, 3, 7),(0,0,1,0,1,0,1,0,0,0)
#define PPUTLU_167 166,168,7,83,334,0,12,241,27,1,3,7,7,7,39,(),(0,0,1,0,1,0,0,1,1,1)
#define PPUTLU_166 165,167,7,83,332,0,12,932,26,0,2,6,6,6,38,(2, 83),(0,0,1,0,1,0,0,1,1,0)
#define PPUTLU_165 164,166,7,82,330,0,12,601,26,1,1,5,5,5,37,(3, 5, 11),(0,0,1,0,1,0,0,1,0,1)
#define PPUTLU_164 163,165,7,82,328,0,12,272,26,0,0,4,4,4,36,(2, 2, 41),(0,0,1,0,1,0,0,1,0,0)
#define PPUTLU_163 162,164,7,81,326,0,12,969,25,1,3,3,3,3,35,(),(0,0,1,0,1,0,0,0,1,1)
#define PPUTLU_162 161,163,7,81,324,0,12,644,25,0,2,2,2,2,34,(2, 3, 3, 3, 3),(0,0,1,0,1,0,0,0,1,0)
#define PPUTLU_161 160,162,7,80,322,0,12,321,25,1,1,1,1,1,33,(7, 23),(0,0,1,0,1,0,0,0,0,1)
#define PPUTLU_160 159,161,7,80,320,0,12,0,25,0,0,0,0,0,32,(2, 2, 2, 2, 2, 5),(0,0,1,0,1,0,0,0,0,0)
#define PPUTLU_159 158,160,7,79,318,0,12,705,24,1,3,7,15,31,31,(3, 53),(0,0,1,0,0,1,1,1,1,1)
#define PPUTLU_158 157,159,7,79,316,0,12,388,24,0,2,6,14,30,30,(2, 79),(0,0,1,0,0,1,1,1,1,0)
#define PPUTLU_157 156,158,7,78,314,0,12,73,24,1,1,5,13,29,29,(),(0,0,1,0,0,1,1,1,0,1)
#define PPUTLU_156 155,157,7,78,312,0,12,784,23,0,0,4,12,28,28,(2, 2, 3, 13),(0,0,1,0,0,1,1,1,0,0)
#define PPUTLU_155 154,156,7,77,310,0,12,473,23,1,3,3,11,27,27,(5, 31),(0,0,1,0,0,1,1,0,1,1)
#define PPUTLU_154 153,155,7,77,308,0,12,164,23,0,2,2,10,26,26,(2, 7, 11),(0,0,1,0,0,1,1,0,1,0)
#define PPUTLU_153 152,154,7,76,306,0,12,881,22,1,1,1,9,25,25,(3, 3, 17),(0,0,1,0,0,1,1,0,0,1)
#define PPUTLU_152 151,153,7,76,304,0,12,576,22,0,0,0,8,24,24,(2, 2, 2, 19),(0,0,1,0,0,1,1,0,0,0)
#define PPUTLU_151 150,152,7,75,302,0,12,273,22,1,3,7,7,23,23,(),(0,0,1,0,0,1,0,1,1,1)
#define PPUTLU_150 149,151,7,75,300,0,12,996,21,0,2,6,6,22,22,(2, 3, 5, 5),(0,0,1,0,0,1,0,1,1,0)
#define PPUTLU_149 148,150,7,74,298,0,12,697,21,1,1,5,5,21,21,(),(0,0,1,0,0,1,0,1,0,1)
#define PPUTLU_148 147,149,7,74,296,0,12,400,21,0,0,4,4,20,20,(2, 2, 37),(0,0,1,0,0,1,0,1,0,0)
#define PPUTLU_147 146,148,7,73,294,0,12,105,21,1,3,3,3,19,19,(3, 7, 7),(0,0,1,0,0,1,0,0,1,1)
#define PPUTLU_146 145,147,7,73,292,0,12,836,20,0,2,2,2,18,18,(2, 73),(0,0,1,0,0,1,0,0,1,0)
#define PPUTLU_145 144,146,7,72,290,0,12,545,20,1,1,1,1,17,17,(5, 29),(0,0,1,0,0,1,0,0,0,1)
#define PPUTLU_144 143,145,7,72,288,0,12,256,20,0,0,0,0,16,16,(2, 2, 2, 2, 3, 3),(0,0,1,0,0,1,0,0,0,0)
#define PPUTLU_143 142,144,7,71,286,0,11,993,19,1,3,7,15,15,15,(11, 13),(0,0,1,0,0,0,1,1,1,1)
#define PPUTLU_142 141,143,7,71,284,0,11,708,19,0,2,6,14,14,14,(2, 71),(0,0,1,0,0,0,1,1,1,0)
#define PPUTLU_141 140,142,7,70,282,0,11,425,19,1,1,5,13,13,13,(3, 47),(0,0,1,0,0,0,1,1,0,1)
#define PPUTLU_140 139,141,7,70,280,0,11,144,19,0,0,4,12,12,12,(2, 2, 5, 7),(0,0,1,0,0,0,1,1,0,0)
#define PPUTLU_139 138,140,7,69,278,0,11,889,18,1,3,3,11,11,11,(),(0,0,1,0,0,0,1,0,1,1)
#define PPUTLU_138 137,139,7,69,276,0,11,612,18,0,2,2,10,10,10,(2, 3, 23),(0,0,1,0,0,0,1,0,1,0)
#define PPUTLU_137 136,138,7,68,274,0,11,337,18,1,1,1,9,9,9,(),(0,0,1,0,0,0,1,0,0,1)
#define PPUTLU_136 135,137,7,68,272,0,11,64,18,0,0,0,8,8,8,(2, 2, 2, 17),(0,0,1,0,0,0,1,0,0,0)
#define PPUTLU_135 134,136,7,67,270,0,11,817,17,1,3,7,7,7,7,(3, 3, 3, 5),(0,0,1,0,0,0,0,1,1,1)
#define PPUTLU_134 133,135,7,67,268,0,11,548,17,0,2,6,6,6,6,(2, 67),(0,0,1,0,0,0,0,1,1,0)
#define PPUTLU_133 132,134,7,66,266,0,11,281,17,1,1,5,5,5,5,(7, 19),(0,0,1,0,0,0,0,1,0,1)
#define PPUTLU_132 131,133,7,66,264,0,11,16,17,0,0,4,4,4,4,(2, 2, 3, 11),(0,0,1,0,0,0,0,1,0,0)
#define PPUTLU_131 130,132,7,65,262,0,11,777,16,1,3,3,3,3,3,(),(0,0,1,0,0,0,0,0,1,1)
#define PPUTLU_130 129,131,7,65,260,0,11,516,16,0,2,2,2,2,2,(2, 5, 13),(0,0,1,0,0,0,0,0,1,0)
#define PPUTLU_129 128,130,7,64,258,0,11,257,16,1,1,1,1,1,1,(3, 43),(0,0,1,0,0,0,0,0,0,1)
#define PPUTLU_128 127,129,7,64,256,0,11,0,16,0,0,0,0,0,0,(2, 2, 2, 2, 2, 2, 2),(0,0,1,0,0,0,0,0,0,0)
#define PPUTLU_127 126,128,6,63,254,0,11,769,15,1,3,7,15,31,63,(),(0,0,0,1,1,1,1,1,1,1)
#define PPUTLU_126 125,127,6,63,252,0,11,516,15,0,2,6,14,30,62,(2, 3, 3, 7),(0,0,0,1,1,1,1,1,1,0)
#define PPUTLU_125 124,126,6,62,250,0,11,265,15,1,1,5,13,29,61,(5, 5, 5),(0,0,0,1,1,1,1,1,0,1)
#define PPUTLU_124 123,125,6,62,248,0,11,16,15,0,0,4,12,28,60,(2, 2, 31),(0,0,0,1,1,1,1,1,0,0)
#define PPUTLU_123 122,124,6,61,246,0,11,793,14,1,3,3,11,27,59,(3, 41),(0,0,0,1,1,1,1,0,1,1)
#define PPUTLU_122 121,123,6,61,244,0,11,548,14,0,2,2,10,26,58,(2, 61),(0,0,0,1,1,1,1,0,1,0)
#define PPUTLU_121 120,122,6,60,242,0,11,305,14,1,1,1,9,25,57,(11, 11),(0,0,0,1,1,1,1,0,0,1)
#define PPUTLU_120 119,121,6,60,240,0,10,64,14,0,0,0,8,24,56,(2, 2, 2, 3, 5),(0,0,0,1,1,1,1,0,0,0)
#define PPUTLU_119 118,120,6,59,238,0,10,849,13,1,3,7,7,23,55,(7, 17),(0,0,0,1,1,1,0,1,1,1)
#define PPUTLU_118 117,119,6,59,236,0,10,612,13,0,2,6,6,22,54,(2, 59),(0,0,0,1,1,1,0,1,1,0)
#define PPUTLU_117 116,118,6,58,234,0,10,377,13,1,1,5,5,21,53,(3, 3, 13),(0,0,0,1,1,1,0,1,0,1)
#define PPUTLU_116 115,117,6,58,232,0,10,144,13,0,0,4,4,20,52,(2, 2, 29),(0,0,0,1,1,1,0,1,0,0)
#define PPUTLU_115 114,116,6,57,230,0,10,937,12,1,3,3,3,19,51,(5, 23),(0,0,0,1,1,1,0,0,1,1)
#define PPUTLU_114 113,115,6,57,228,0,10,708,12,0,2,2,2,18,50,(2, 3, 19),(0,0,0,1,1,1,0,0,1,0)
#define PPUTLU_113 112,114,6,56,226,0,10,481,12,1,1,1,1,17,49,(),(0,0,0,1,1,1,0,0,0,1)
#define PPUTLU_112 111,113,6,56,224,0,10,256,12,0,0,0,0,16,48,(2, 2, 2, 2, 7),(0,0,0,1,1,1,0,0,0,0)
#define PPUTLU_111 110,112,6,55,222,0,10,33,12,1,3,7,15,15,47,(3, 37),(0,0,0,1,1,0,1,1,1,1)
#define PPUTLU_110 109,111,6,55,220,0,10,836,11,0,2,6,14,14,46,(2, 5, 11),(0,0,0,1,1,0,1,1,1,0)
#define PPUTLU_109 108,110,6,54,218,0,10,617,11,1,1,5,13,13,45,(),(0,0,0,1,1,0,1,1,0,1)
#define PPUTLU_108 107,109,6,54,216,0,10,400,11,0,0,4,12,12,44,(2, 2, 3, 3, 3),(0,0,0,1,1,0,1,1,0,0)
#define PPUTLU_107 106,108,6,53,214,0,10,185,11,1,3,3,11,11,43,(),(0,0,0,1,1,0,1,0,1,1)
#define PPUTLU_106 105,107,6,53,212,0,10,996,10,0,2,2,10,10,42,(2, 53),(0,0,0,1,1,0,1,0,1,0)
#define PPUTLU_105 104,106,6,52,210,0,10,785,10,1,1,1,9,9,41,(3, 5, 7),(0,0,0,1,1,0,1,0,0,1)
#define PPUTLU_104 103,105,6,52,208,0,10,576,10,0,0,0,8,8,40,(2, 2, 2, 13),(0,0,0,1,1,0,1,0,0,0)
#define PPUTLU_103 102,104,6,51,206,0,10,369,10,1,3,7,7,7,39,(),(0,0,0,1,1,0,0,1,1,1)
#define PPUTLU_102 101,103,6,51,204,0,10,164,10,0,2,6,6,6,38,(2, 3, 17),(0,0,0,1,1,0,0,1,1,0)
#define PPUTLU_101 100,102,6,50,202,0,10,985,9,1,1,5,5,5,37,(),(0,0,0,1,1,0,0,1,0,1)
#define PPUTLU_100 99,101,6,50,200,0,10,784,9,0,0,4,4,4,36,(2, 2, 5, 5),(0,0,0,1,1,0,0,1,0,0)
#define PPUTLU_99 98,100,6,49,198,0,9,585,9,1,3,3,3,3,35,(3, 3, 11),(0,0,0,1,1,0,0,0,1,1)
#define PPUTLU_98 97,99,6,49,196,0,9,388,9,0,2,2,2,2,34,(2, 7, 7),(0,0,0,1,1,0,0,0,1,0)
#define PPUTLU_97 96,98,6,48,194,0,9,193,9,1,1,1,1,1,33,(),(0,0,0,1,1,0,0,0,0,1)
#define PPUTLU_96 95,97,6,48,192,0,9,0,9,0,0,0,0,0,32,(2, 2, 2, 2, 2, 3),(0,0,0,1,1,0,0,0,0,0)
#define PPUTLU_95 94,96,6,47,190,0,9,833,8,1,3,7,15,31,31,(5, 19),(0,0,0,1,0,1,1,1,1,1)
#define PPUTLU_94 93,95,6,47,188,0,9,644,8,0,2,6,14,30,30,(2, 47),(0,0,0,1,0,1,1,1,1,0)
#define PPUTLU_93 92,94,6,46,186,0,9,457,8,1,1,5,13,29,29,(3, 31),(0,0,0,1,0,1,1,1,0,1)
#define PPUTLU_92 91,93,6,46,184,0,9,272,8,0,0,4,12,28,28,(2, 2, 23),(0,0,0,1,0,1,1,1,0,0)
#define PPUTLU_91 90,92,6,45,182,0,9,89,8,1,3,3,11,27,27,(7, 13),(0,0,0,1,0,1,1,0,1,1)
#define PPUTLU_90 89,91,6,45,180,0,9,932,7,0,2,2,10,26,26,(2, 3, 3, 5),(0,0,0,1,0,1,1,0,1,0)
#define PPUTLU_89 88,90,6,44,178,0,9,753,7,1,1,1,9,25,25,(),(0,0,0,1,0,1,1,0,0,1)
#define PPUTLU_88 87,89,6,44,176,0,9,576,7,0,0,0,8,24,24,(2, 2, 2, 11),(0,0,0,1,0,1,1,0,0,0)
#define PPUTLU_87 86,88,6,43,174,0,9,401,7,1,3,7,7,23,23,(3, 29),(0,0,0,1,0,1,0,1,1,1)
#define PPUTLU_86 85,87,6,43,172,0,9,228,7,0,2,6,6,22,22,(2, 43),(0,0,0,1,0,1,0,1,1,0)
#define PPUTLU_85 84,86,6,42,170,0,9,57,7,1,1,5,5,21,21,(5, 17),(0,0,0,1,0,1,0,1,0,1)
#define PPUTLU_84 83,85,6,42,168,0,9,912,6,0,0,4,4,20,20,(2, 2, 3, 7),(0,0,0,1,0,1,0,1,0,0)
#define PPUTLU_83 82,84,6,41,166,0,9,745,6,1,3,3,3,19,19,(),(0,0,0,1,0,1,0,0,1,1)
#define PPUTLU_82 81,83,6,41,164,0,9,580,6,0,2,2,2,18,18,(2, 41),(0,0,0,1,0,1,0,0,1,0)
#define PPUTLU_81 80,82,6,40,162,0,9,417,6,1,1,1,1,17,17,(3, 3, 3, 3),(0,0,0,1,0,1,0,0,0,1)
#define PPUTLU_80 79,81,6,40,160,0,8,256,6,0,0,0,0,16,16,(2, 2, 2, 2, 5),(0,0,0,1,0,1,0,0,0,0)
#define PPUTLU_79 78,80,6,39,158,0,8,97,6,1,3,7,15,15,15,(),(0,0,0,1,0,0,1,1,1,1)
#define PPUTLU_78 77,79,6,39,156,0,8,964,5,0,2,6,14,14,14,(2, 3, 13),(0,0,0,1,0,0,1,1,1,0)
#define PPUTLU_77 76,78,6,38,154,0,8,809,5,1,1,5,13,13,13,(7, 11),(0,0,0,1,0,0,1,1,0,1)
#define PPUTLU_76 75,77,6,38,152,0,8,656,5,0,0,4,12,12,12,(2, 2, 19),(0,0,0,1,0,0,1,1,0,0)
#define PPUTLU_75 74,76,6,37,150,0,8,505,5,1,3,3,11,11,11,(3, 5, 5),(0,0,0,1,0,0,1,0,1,1)
#define PPUTLU_74 73,75,6,37,148,0,8,356,5,0,2,2,10,10,10,(2, 37),(0,0,0,1,0,0,1,0,1,0)
#define PPUTLU_73 72,74,6,36,146,0,8,209,5,1,1,1,9,9,9,(),(0,0,0,1,0,0,1,0,0,1)
#define PPUTLU_72 71,73,6,36,144,0,8,64,5,0,0,0,8,8,8,(2, 2, 2, 3, 3),(0,0,0,1,0,0,1,0,0,0)
#define PPUTLU_71 70,72,6,35,142,0,8,945,4,1,3,7,7,7,7,(),(0,0,0,1,0,0,0,1,1,1)
#define PPUTLU_70 69,71,6,35,140,0,8,804,4,0,2,6,6,6,6,(2, 5, 7),(0,0,0,1,0,0,0,1,1,0)
#define PPUTLU_69 68,70,6,34,138,0,8,665,4,1,1,5,5,5,5,(3, 23),(0,0,0,1,0,0,0,1,0,1)
#define PPUTLU_68 67,69,6,34,136,0,8,528,4,0,0,4,4,4,4,(2, 2, 17),(0,0,0,1,0,0,0,1,0,0)
#define PPUTLU_67 66,68,6,33,134,0,8,393,4,1,3,3,3,3,3,(),(0,0,0,1,0,0,0,0,1,1)
#define PPUTLU_66 65,67,6,33,132,0,8,260,4,0,2,2,2,2,2,(2, 3, 11),(0,0,0,1,0,0,0,0,1,0)
#define PPUTLU_65 64,66,6,32,130,0,8,129,4,1,1,1,1,1,1,(5, 13),(0,0,0,1,0,0,0,0,0,1)
#define PPUTLU_64 63,65,6,32,128,0,8,0,4,0,0,0,0,0,0,(2, 2, 2, 2, 2, 2),(0,0,0,1,0,0,0,0,0,0)
#define PPUTLU_63 62,64,5,31,126,0,7,897,3,1,3,7,15,31,63,(3, 3, 7),(0,0,0,0,1,1,1,1,1,1)
#define PPUTLU_62 61,63,5,31,124,0,7,772,3,0,2,6,14,30,62,(2, 31),(0,0,0,0,1,1,1,1,1,0)
#define PPUTLU_61 60,62,5,30,122,0,7,649,3,1,1,5,13,29,61,(),(0,0,0,0,1,1,1,1,0,1)
#define PPUTLU_60 59,61,5,30,120,0,7,528,3,0,0,4,12,28,60,(2, 2, 3, 5),(0,0,0,0,1,1,1,1,0,0)
#define PPUTLU_59 58,60,5,29,118,0,7,409,3,1,3,3,11,27,59,(),(0,0,0,0,1,1,1,0,1,1)
#define PPUTLU_58 57,59,5,29,116,0,7,292,3,0,2,2,10,26,58,(2, 29),(0,0,0,0,1,1,1,0,1,0)
#define PPUTLU_57 56,58,5,28,114,0,7,177,3,1,1,1,9,25,57,(3, 19),(0,0,0,0,1,1,1,0,0,1)
#define PPUTLU_56 55,57,5,28,112,0,7,64,3,0,0,0,8,24,56,(2, 2, 2, 7),(0,0,0,0,1,1,1,0,0,0)
#define PPUTLU_55 54,56,5,27,110,0,7,977,2,1,3,7,7,23,55,(5, 11),(0,0,0,0,1,1,0,1,1,1)
#define PPUTLU_54 53,55,5,27,108,0,7,868,2,0,2,6,6,22,54,(2, 3, 3, 3),(0,0,0,0,1,1,0,1,1,0)
#define PPUTLU_53 52,54,5,26,106,0,7,761,2,1,1,5,5,21,53,(),(0,0,0,0,1,1,0,1,0,1)
#define PPUTLU_52 51,53,5,26,104,0,7,656,2,0,0,4,4,20,52,(2, 2, 13),(0,0,0,0,1,1,0,1,0,0)
#define PPUTLU_51 50,52,5,25,102,0,7,553,2,1,3,3,3,19,51,(3, 17),(0,0,0,0,1,1,0,0,1,1)
#define PPUTLU_50 49,51,5,25,100,0,7,452,2,0,2,2,2,18,50,(2, 5, 5),(0,0,0,0,1,1,0,0,1,0)
#define PPUTLU_49 48,50,5,24,98,0,7,353,2,1,1,1,1,17,49,(7, 7),(0,0,0,0,1,1,0,0,0,1)
#define PPUTLU_48 47,49,5,24,96,0,6,256,2,0,0,0,0,16,48,(2, 2, 2, 2, 3),(0,0,0,0,1,1,0,0,0,0)
#define PPUTLU_47 46,48,5,23,94,0,6,161,2,1,3,7,15,15,47,(),(0,0,0,0,1,0,1,1,1,1)
#define PPUTLU_46 45,47,5,23,92,0,6,68,2,0,2,6,14,14,46,(2, 23),(0,0,0,0,1,0,1,1,1,0)
#define PPUTLU_45 44,46,5,22,90,0,6,1001,1,1,1,5,13,13,45,(3, 3, 5),(0,0,0,0,1,0,1,1,0,1)
#define PPUTLU_44 43,45,5,22,88,0,6,912,1,0,0,4,12,12,44,(2, 2, 11),(0,0,0,0,1,0,1,1,0,0)
#define PPUTLU_43 42,44,5,21,86,0,6,825,1,1,3,3,11,11,43,(),(0,0,0,0,1,0,1,0,1,1)
#define PPUTLU_42 41,43,5,21,84,0,6,740,1,0,2,2,10,10,42,(2, 3, 7),(0,0,0,0,1,0,1,0,1,0)
#define PPUTLU_41 40,42,5,20,82,0,6,657,1,1,1,1,9,9,41,(),(0,0,0,0,1,0,1,0,0,1)
#define PPUTLU_40 39,41,5,20,80,0,6,576,1,0,0,0,8,8,40,(2, 2, 2, 5),(0,0,0,0,1,0,1,0,0,0)
#define PPUTLU_39 38,40,5,19,78,0,6,497,1,1,3,7,7,7,39,(3, 13),(0,0,0,0,1,0,0,1,1,1)
#define PPUTLU_38 37,39,5,19,76,0,6,420,1,0,2,6,6,6,38,(2, 19),(0,0,0,0,1,0,0,1,1,0)
#define PPUTLU_37 36,38,5,18,74,0,6,345,1,1,1,5,5,5,37,(),(0,0,0,0,1,0,0,1,0,1)
#define PPUTLU_36 35,37,5,18,72,0,6,272,1,0,0,4,4,4,36,(2, 2, 3, 3),(0,0,0,0,1,0,0,1,0,0)
#define PPUTLU_35 34,36,5,17,70,0,5,201,1,1,3,3,3,3,35,(5, 7),(0,0,0,0,1,0,0,0,1,1)
#define PPUTLU_34 33,35,5,17,68,0,5,132,1,0,2,2,2,2,34,(2, 17),(0,0,0,0,1,0,0,0,1,0)
#define PPUTLU_33 32,34,5,16,66,0,5,65,1,1,1,1,1,1,33,(3, 11),(0,0,0,0,1,0,0,0,0,1)
#define PPUTLU_32 31,33,5,16,64,0,5,0,1,0,0,0,0,0,32,(2, 2, 2, 2, 2),(0,0,0,0,1,0,0,0,0,0)
#define PPUTLU_31 30,32,4,15,62,0,5,961,0,1,3,7,15,31,31,(),(0,0,0,0,0,1,1,1,1,1)
#define PPUTLU_30 29,31,4,15,60,0,5,900,0,0,2,6,14,30,30,(2, 3, 5),(0,0,0,0,0,1,1,1,1,0)
#define PPUTLU_29 28,30,4,14,58,0,5,841,0,1,1,5,13,29,29,(),(0,0,0,0,0,1,1,1,0,1)
#define PPUTLU_28 27,29,4,14,56,0,5,784,0,0,0,4,12,28,28,(2, 2, 7),(0,0,0,0,0,1,1,1,0,0)
#define PPUTLU_27 26,28,4,13,54,0,5,729,0,1,3,3,11,27,27,(3, 3, 3),(0,0,0,0,0,1,1,0,1,1)
#define PPUTLU_26 25,27,4,13,52,0,5,676,0,0,2,2,10,26,26,(2, 13),(0,0,0,0,0,1,1,0,1,0)
#define PPUTLU_25 24,26,4,12,50,0,5,625,0,1,1,1,9,25,25,(5, 5),(0,0,0,0,0,1,1,0,0,1)
#define PPUTLU_24 23,25,4,12,48,0,4,576,0,0,0,0,8,24,24,(2, 2, 2, 3),(0,0,0,0,0,1,1,0,0,0)
#define PPUTLU_23 22,24,4,11,46,0,4,529,0,1,3,7,7,23,23,(),(0,0,0,0,0,1,0,1,1,1)
#define PPUTLU_22 21,23,4,11,44,0,4,484,0,0,2,6,6,22,22,(2, 11),(0,0,0,0,0,1,0,1,1,0)
#define PPUTLU_21 20,22,4,10,42,0,4,441,0,1,1,5,5,21,21,(3, 7),(0,0,0,0,0,1,0,1,0,1)
#define PPUTLU_20 19,21,4,10,40,0,4,400,0,0,0,4,4,20,20,(2, 2, 5),(0,0,0,0,0,1,0,1,0,0)
#define PPUTLU_19 18,20,4,9,38,0,4,361,0,1,3,3,3,19,19,(),(0,0,0,0,0,1,0,0,1,1)
#define PPUTLU_18 17,19,4,9,36,0,4,324,0,0,2,2,2,18,18,(2, 3, 3),(0,0,0,0,0,1,0,0,1,0)
#define PPUTLU_17 16,18,4,8,34,0,4,289,0,1,1,1,1,17,17,(),(0,0,0,0,0,1,0,0,0,1)
#define PPUTLU_16 15,17,4,8,32,0,4,256,0,0,0,0,0,16,16,(2, 2, 2, 2),(0,0,0,0,0,1,0,0,0,0)
#define PPUTLU_15 14,16,3,7,30,0,3,225,0,1,3,7,15,15,15,(3, 5),(0,0,0,0,0,0,1,1,1,1)
#define PPUTLU_14 13,15,3,7,28,0,3,196,0,0,2,6,14,14,14,(2, 7),(0,0,0,0,0,0,1,1,1,0)
#define PPUTLU_13 12,14,3,6,26,0,3,169,0,1,1,5,13,13,13,(),(0,0,0,0,0,0,1,1,0,1)
#define PPUTLU_12 11,13,3,6,24,0,3,144,0,0,0,4,12,12,12,(2, 2, 3),(0,0,0,0,0,0,1,1,0,0)
#define PPUTLU_11 10,12,3,5,22,0,3,121,0,1,3,3,11,11,11,(),(0,0,0,0,0,0,1,0,1,1)
#define PPUTLU_10 9,11,3,5,20,0,3,100,0,0,2,2,10,10,10,(2, 5),(0,0,0,0,0,0,1,0,1,0)
#define PPUTLU_9 8,10,3,4,18,0,3,81,0,1,1,1,9,9,9,(3, 3),(0,0,0,0,0,0,1,0,0,1)
#define PPUTLU_8 7,9,3,4,16,0,2,64,0,0,0,0,8,8,8,(2, 2, 2),(0,0,0,0,0,0,1,0,0,0)
#define PPUTLU_7 6,8,2,3,14,0,2,49,0,1,3,7,7,7,7,(),(0,0,0,0,0,0,0,1,1,1)
#define PPUTLU_6 5,7,2,3,12,0,2,36,0,0,2,6,6,6,6,(2, 3),(0,0,0,0,0,0,0,1,1,0)
#define PPUTLU_5 4,6,2,2,10,0,2,25,0,1,1,5,5,5,5,(),(0,0,0,0,0,0,0,1,0,1)
#define PPUTLU_4 3,5,2,2,8,0,2,16,0,0,0,4,4,4,4,(2, 2),(0,0,0,0,0,0,0,1,0,0)
#define PPUTLU_3 2,4,1,1,6,0,1,9,0,1,3,3,3,3,3,(),(0,0,0,0,0,0,0,0,1,1)
#define PPUTLU_2 1,3,1,1,4,0,1,4,0,0,2,2,2,2,2,(),(0,0,0,0,0,0,0,0,1,0)
#define PPUTLU_1 0,2,0,0,2,0,1,1,0,1,1,1,1,1,1,(),(0,0,0,0,0,0,0,0,0,1)
#define PPUTLU_0 1023,1,,0,0,0,0,0,0,0,0,0,0,0,0,(),(0,0,0,0,0,0,0,0,0,0)

// clang-format on
#define PPUTLUINT_DECIMAL_1111111111 1023
#define PPUTLUINT_DECIMAL_1111111110 1022
#define PPUTLUINT_DECIMAL_1111111101 1021
#define PPUTLUINT_DECIMAL_1111111100 1020
#define PPUTLUINT_DECIMAL_1111111011 1019
#define PPUTLUINT_DECIMAL_1111111010 1018
#define PPUTLUINT_DECIMAL_1111111001 1017
#define PPUTLUINT_DECIMAL_1111111000 1016
#define PPUTLUINT_DECIMAL_1111110111 1015
#define PPUTLUINT_DECIMAL_1111110110 1014
#define PPUTLUINT_DECIMAL_1111110101 1013
#define PPUTLUINT_DECIMAL_1111110100 1012
#define PPUTLUINT_DECIMAL_1111110011 1011
#define PPUTLUINT_DECIMAL_1111110010 1010
#define PPUTLUINT_DECIMAL_1111110001 1009
#define PPUTLUINT_DECIMAL_1111110000 1008
#define PPUTLUINT_DECIMAL_1111101111 1007
#define PPUTLUINT_DECIMAL_1111101110 1006
#define PPUTLUINT_DECIMAL_1111101101 1005
#define PPUTLUINT_DECIMAL_1111101100 1004
#define PPUTLUINT_DECIMAL_1111101011 1003
#define PPUTLUINT_DECIMAL_1111101010 1002
#define PPUTLUINT_DECIMAL_1111101001 1001
#define PPUTLUINT_DECIMAL_1111101000 1000
#define PPUTLUINT_DECIMAL_1111100111 999
#define PPUTLUINT_DECIMAL_1111100110 998
#define PPUTLUINT_DECIMAL_1111100101 997
#define PPUTLUINT_DECIMAL_1111100100 996
#define PPUTLUINT_DECIMAL_1111100011 995
#define PPUTLUINT_DECIMAL_1111100010 994
#define PPUTLUINT_DECIMAL_1111100001 993
#define PPUTLUINT_DECIMAL_1111100000 992
#define PPUTLUINT_DECIMAL_1111011111 991
#define PPUTLUINT_DECIMAL_1111011110 990
#define PPUTLUINT_DECIMAL_1111011101 989
#define PPUTLUINT_DECIMAL_1111011100 988
#define PPUTLUINT_DECIMAL_1111011011 987
#define PPUTLUINT_DECIMAL_1111011010 986
#define PPUTLUINT_DECIMAL_1111011001 985
#define PPUTLUINT_DECIMAL_1111011000 984
#define PPUTLUINT_DECIMAL_1111010111 983
#define PPUTLUINT_DECIMAL_1111010110 982
#define PPUTLUINT_DECIMAL_1111010101 981
#define PPUTLUINT_DECIMAL_1111010100 980
#define PPUTLUINT_DECIMAL_1111010011 979
#define PPUTLUINT_DECIMAL_1111010010 978
#define PPUTLUINT_DECIMAL_1111010001 977
#define PPUTLUINT_DECIMAL_1111010000 976
#define PPUTLUINT_DECIMAL_1111001111 975
#define PPUTLUINT_DECIMAL_1111001110 974
#define PPUTLUINT_DECIMAL_1111001101 973
#define PPUTLUINT_DECIMAL_1111001100 972
#define PPUTLUINT_DECIMAL_1111001011 971
#define PPUTLUINT_DECIMAL_1111001010 970
#define PPUTLUINT_DECIMAL_1111001001 969
#define PPUTLUINT_DECIMAL_1111001000 968
#define PPUTLUINT_DECIMAL_1111000111 967
#define PPUTLUINT_DECIMAL_1111000110 966
#define PPUTLUINT_DECIMAL_1111000101 965
#define PPUTLUINT_DECIMAL_1111000100 964
#define PPUTLUINT_DECIMAL_1111000011 963
#define PPUTLUINT_DECIMAL_1111000010 962
#define PPUTLUINT_DECIMAL_1111000001 961
#define PPUTLUINT_DECIMAL_1111000000 960
#define PPUTLUINT_DECIMAL_1110111111 959
#define PPUTLUINT_DECIMAL_1110111110 958
#define PPUTLUINT_DECIMAL_1110111101 957
#define PPUTLUINT_DECIMAL_1110111100 956
#define PPUTLUINT_DECIMAL_1110111011 955
#define PPUTLUINT_DECIMAL_1110111010 954
#define PPUTLUINT_DECIMAL_1110111001 953
#define PPUTLUINT_DECIMAL_1110111000 952
#define PPUTLUINT_DECIMAL_1110110111 951
#define PPUTLUINT_DECIMAL_1110110110 950
#define PPUTLUINT_DECIMAL_1110110101 949
#define PPUTLUINT_DECIMAL_1110110100 948
#define PPUTLUINT_DECIMAL_1110110011 947
#define PPUTLUINT_DECIMAL_1110110010 946
#define PPUTLUINT_DECIMAL_1110110001 945
#define PPUTLUINT_DECIMAL_1110110000 944
#define PPUTLUINT_DECIMAL_1110101111 943
#define PPUTLUINT_DECIMAL_1110101110 942
#define PPUTLUINT_DECIMAL_1110101101 941
#define PPUTLUINT_DECIMAL_1110101100 940
#define PPUTLUINT_DECIMAL_1110101011 939
#define PPUTLUINT_DECIMAL_1110101010 938
#define PPUTLUINT_DECIMAL_1110101001 937
#define PPUTLUINT_DECIMAL_1110101000 936
#define PPUTLUINT_DECIMAL_1110100111 935
#define PPUTLUINT_DECIMAL_1110100110 934
#define PPUTLUINT_DECIMAL_1110100101 933
#define PPUTLUINT_DECIMAL_1110100100 932
#define PPUTLUINT_DECIMAL_1110100011 931
#define PPUTLUINT_DECIMAL_1110100010 930
#define PPUTLUINT_DECIMAL_1110100001 929
#define PPUTLUINT_DECIMAL_1110100000 928
#define PPUTLUINT_DECIMAL_1110011111 927
#define PPUTLUINT_DECIMAL_1110011110 926
#define PPUTLUINT_DECIMAL_1110011101 925
#define PPUTLUINT_DECIMAL_1110011100 924
#define PPUTLUINT_DECIMAL_1110011011 923
#define PPUTLUINT_DECIMAL_1110011010 922
#define PPUTLUINT_DECIMAL_1110011001 921
#define PPUTLUINT_DECIMAL_1110011000 920
#define PPUTLUINT_DECIMAL_1110010111 919
#define PPUTLUINT_DECIMAL_1110010110 918
#define PPUTLUINT_DECIMAL_1110010101 917
#define PPUTLUINT_DECIMAL_1110010100 916
#define PPUTLUINT_DECIMAL_1110010011 915
#define PPUTLUINT_DECIMAL_1110010010 914
#define PPUTLUINT_DECIMAL_1110010001 913
#define PPUTLUINT_DECIMAL_1110010000 912
#define PPUTLUINT_DECIMAL_1110001111 911
#define PPUTLUINT_DECIMAL_1110001110 910
#define PPUTLUINT_DECIMAL_1110001101 909
#define PPUTLUINT_DECIMAL_1110001100 908
#define PPUTLUINT_DECIMAL_1110001011 907
#define PPUTLUINT_DECIMAL_1110001010 906
#define PPUTLUINT_DECIMAL_1110001001 905
#define PPUTLUINT_DECIMAL_1110001000 904
#define PPUTLUINT_DECIMAL_1110000111 903
#define PPUTLUINT_DECIMAL_1110000110 902
#define PPUTLUINT_DECIMAL_1110000101 901
#define PPUTLUINT_DECIMAL_1110000100 900
#define PPUTLUINT_DECIMAL_1110000011 899
#define PPUTLUINT_DECIMAL_1110000010 898
#define PPUTLUINT_DECIMAL_1110000001 897
#define PPUTLUINT_DECIMAL_1110000000 896
#define PPUTLUINT_DECIMAL_1101111111 895
#define PPUTLUINT_DECIMAL_1101111110 894
#define PPUTLUINT_DECIMAL_1101111101 893
#define PPUTLUINT_DECIMAL_1101111100 892
#define PPUTLUINT_DECIMAL_1101111011 891
#define PPUTLUINT_DECIMAL_1101111010 890
#define PPUTLUINT_DECIMAL_1101111001 889
#define PPUTLUINT_DECIMAL_1101111000 888
#define PPUTLUINT_DECIMAL_1101110111 887
#define PPUTLUINT_DECIMAL_1101110110 886
#define PPUTLUINT_DECIMAL_1101110101 885
#define PPUTLUINT_DECIMAL_1101110100 884
#define PPUTLUINT_DECIMAL_1101110011 883
#define PPUTLUINT_DECIMAL_1101110010 882
#define PPUTLUINT_DECIMAL_1101110001 881
#define PPUTLUINT_DECIMAL_1101110000 880
#define PPUTLUINT_DECIMAL_1101101111 879
#define PPUTLUINT_DECIMAL_1101101110 878
#define PPUTLUINT_DECIMAL_1101101101 877
#define PPUTLUINT_DECIMAL_1101101100 876
#define PPUTLUINT_DECIMAL_1101101011 875
#define PPUTLUINT_DECIMAL_1101101010 874
#define PPUTLUINT_DECIMAL_1101101001 873
#define PPUTLUINT_DECIMAL_1101101000 872
#define PPUTLUINT_DECIMAL_1101100111 871
#define PPUTLUINT_DECIMAL_1101100110 870
#define PPUTLUINT_DECIMAL_1101100101 869
#define PPUTLUINT_DECIMAL_1101100100 868
#define PPUTLUINT_DECIMAL_1101100011 867
#define PPUTLUINT_DECIMAL_1101100010 866
#define PPUTLUINT_DECIMAL_1101100001 865
#define PPUTLUINT_DECIMAL_1101100000 864
#define PPUTLUINT_DECIMAL_1101011111 863
#define PPUTLUINT_DECIMAL_1101011110 862
#define PPUTLUINT_DECIMAL_1101011101 861
#define PPUTLUINT_DECIMAL_1101011100 860
#define PPUTLUINT_DECIMAL_1101011011 859
#define PPUTLUINT_DECIMAL_1101011010 858
#define PPUTLUINT_DECIMAL_1101011001 857
#define PPUTLUINT_DECIMAL_1101011000 856
#define PPUTLUINT_DECIMAL_1101010111 855
#define PPUTLUINT_DECIMAL_1101010110 854
#define PPUTLUINT_DECIMAL_1101010101 853
#define PPUTLUINT_DECIMAL_1101010100 852
#define PPUTLUINT_DECIMAL_1101010011 851
#define PPUTLUINT_DECIMAL_1101010010 850
#define PPUTLUINT_DECIMAL_1101010001 849
#define PPUTLUINT_DECIMAL_1101010000 848
#define PPUTLUINT_DECIMAL_1101001111 847
#define PPUTLUINT_DECIMAL_1101001110 846
#define PPUTLUINT_DECIMAL_1101001101 845
#define PPUTLUINT_DECIMAL_1101001100 844
#define PPUTLUINT_DECIMAL_1101001011 843
#define PPUTLUINT_DECIMAL_1101001010 842
#define PPUTLUINT_DECIMAL_1101001001 841
#define PPUTLUINT_DECIMAL_1101001000 840
#define PPUTLUINT_DECIMAL_1101000111 839
#define PPUTLUINT_DECIMAL_1101000110 838
#define PPUTLUINT_DECIMAL_1101000101 837
#define PPUTLUINT_DECIMAL_1101000100 836
#define PPUTLUINT_DECIMAL_1101000011 835
#define PPUTLUINT_DECIMAL_1101000010 834
#define PPUTLUINT_DECIMAL_1101000001 833
#define PPUTLUINT_DECIMAL_1101000000 832
#define PPUTLUINT_DECIMAL_1100111111 831
#define PPUTLUINT_DECIMAL_1100111110 830
#define PPUTLUINT_DECIMAL_1100111101 829
#define PPUTLUINT_DECIMAL_1100111100 828
#define PPUTLUINT_DECIMAL_1100111011 827
#define PPUTLUINT_DECIMAL_1100111010 826
#define PPUTLUINT_DECIMAL_1100111001 825
#define PPUTLUINT_DECIMAL_1100111000 824
#define PPUTLUINT_DECIMAL_1100110111 823
#define PPUTLUINT_DECIMAL_1100110110 822
#define PPUTLUINT_DECIMAL_1100110101 821
#define PPUTLUINT_DECIMAL_1100110100 820
#define PPUTLUINT_DECIMAL_1100110011 819
#define PPUTLUINT_DECIMAL_1100110010 818
#define PPUTLUINT_DECIMAL_1100110001 817
#define PPUTLUINT_DECIMAL_1100110000 816
#define PPUTLUINT_DECIMAL_1100101111 815
#define PPUTLUINT_DECIMAL_1100101110 814
#define PPUTLUINT_DECIMAL_1100101101 813
#define PPUTLUINT_DECIMAL_1100101100 812
#define PPUTLUINT_DECIMAL_1100101011 811
#define PPUTLUINT_DECIMAL_1100101010 810
#define PPUTLUINT_DECIMAL_1100101001 809
#define PPUTLUINT_DECIMAL_1100101000 808
#define PPUTLUINT_DECIMAL_1100100111 807
#define PPUTLUINT_DECIMAL_1100100110 806
#define PPUTLUINT_DECIMAL_1100100101 805
#define PPUTLUINT_DECIMAL_1100100100 804
#define PPUTLUINT_DECIMAL_1100100011 803
#define PPUTLUINT_DECIMAL_1100100010 802
#define PPUTLUINT_DECIMAL_1100100001 801
#define PPUTLUINT_DECIMAL_1100100000 800
#define PPUTLUINT_DECIMAL_1100011111 799
#define PPUTLUINT_DECIMAL_1100011110 798
#define PPUTLUINT_DECIMAL_1100011101 797
#define PPUTLUINT_DECIMAL_1100011100 796
#define PPUTLUINT_DECIMAL_1100011011 795
#define PPUTLUINT_DECIMAL_1100011010 794
#define PPUTLUINT_DECIMAL_1100011001 793
#define PPUTLUINT_DECIMAL_1100011000 792
#define PPUTLUINT_DECIMAL_1100010111 791
#define PPUTLUINT_DECIMAL_1100010110 790
#define PPUTLUINT_DECIMAL_1100010101 789
#define PPUTLUINT_DECIMAL_1100010100 788
#define PPUTLUINT_DECIMAL_1100010011 787
#define PPUTLUINT_DECIMAL_1100010010 786
#define PPUTLUINT_DECIMAL_1100010001 785
#define PPUTLUINT_DECIMAL_1100010000 784
#define PPUTLUINT_DECIMAL_1100001111 783
#define PPUTLUINT_DECIMAL_1100001110 782
#define PPUTLUINT_DECIMAL_1100001101 781
#define PPUTLUINT_DECIMAL_1100001100 780
#define PPUTLUINT_DECIMAL_1100001011 779
#define PPUTLUINT_DECIMAL_1100001010 778
#define PPUTLUINT_DECIMAL_1100001001 777
#define PPUTLUINT_DECIMAL_1100001000 776
#define PPUTLUINT_DECIMAL_1100000111 775
#define PPUTLUINT_DECIMAL_1100000110 774
#define PPUTLUINT_DECIMAL_1100000101 773
#define PPUTLUINT_DECIMAL_1100000100 772
#define PPUTLUINT_DECIMAL_1100000011 771
#define PPUTLUINT_DECIMAL_1100000010 770
#define PPUTLUINT_DECIMAL_1100000001 769
#define PPUTLUINT_DECIMAL_1100000000 768
#define PPUTLUINT_DECIMAL_1011111111 767
#define PPUTLUINT_DECIMAL_1011111110 766
#define PPUTLUINT_DECIMAL_1011111101 765
#define PPUTLUINT_DECIMAL_1011111100 764
#define PPUTLUINT_DECIMAL_1011111011 763
#define PPUTLUINT_DECIMAL_1011111010 762
#define PPUTLUINT_DECIMAL_1011111001 761
#define PPUTLUINT_DECIMAL_1011111000 760
#define PPUTLUINT_DECIMAL_1011110111 759
#define PPUTLUINT_DECIMAL_1011110110 758
#define PPUTLUINT_DECIMAL_1011110101 757
#define PPUTLUINT_DECIMAL_1011110100 756
#define PPUTLUINT_DECIMAL_1011110011 755
#define PPUTLUINT_DECIMAL_1011110010 754
#define PPUTLUINT_DECIMAL_1011110001 753
#define PPUTLUINT_DECIMAL_1011110000 752
#define PPUTLUINT_DECIMAL_1011101111 751
#define PPUTLUINT_DECIMAL_1011101110 750
#define PPUTLUINT_DECIMAL_1011101101 749
#define PPUTLUINT_DECIMAL_1011101100 748
#define PPUTLUINT_DECIMAL_1011101011 747
#define PPUTLUINT_DECIMAL_1011101010 746
#define PPUTLUINT_DECIMAL_1011101001 745
#define PPUTLUINT_DECIMAL_1011101000 744
#define PPUTLUINT_DECIMAL_1011100111 743
#define PPUTLUINT_DECIMAL_1011100110 742
#define PPUTLUINT_DECIMAL_1011100101 741
#define PPUTLUINT_DECIMAL_1011100100 740
#define PPUTLUINT_DECIMAL_1011100011 739
#define PPUTLUINT_DECIMAL_1011100010 738
#define PPUTLUINT_DECIMAL_1011100001 737
#define PPUTLUINT_DECIMAL_1011100000 736
#define PPUTLUINT_DECIMAL_1011011111 735
#define PPUTLUINT_DECIMAL_1011011110 734
#define PPUTLUINT_DECIMAL_1011011101 733
#define PPUTLUINT_DECIMAL_1011011100 732
#define PPUTLUINT_DECIMAL_1011011011 731
#define PPUTLUINT_DECIMAL_1011011010 730
#define PPUTLUINT_DECIMAL_1011011001 729
#define PPUTLUINT_DECIMAL_1011011000 728
#define PPUTLUINT_DECIMAL_1011010111 727
#define PPUTLUINT_DECIMAL_1011010110 726
#define PPUTLUINT_DECIMAL_1011010101 725
#define PPUTLUINT_DECIMAL_1011010100 724
#define PPUTLUINT_DECIMAL_1011010011 723
#define PPUTLUINT_DECIMAL_1011010010 722
#define PPUTLUINT_DECIMAL_1011010001 721
#define PPUTLUINT_DECIMAL_1011010000 720
#define PPUTLUINT_DECIMAL_1011001111 719
#define PPUTLUINT_DECIMAL_1011001110 718
#define PPUTLUINT_DECIMAL_1011001101 717
#define PPUTLUINT_DECIMAL_1011001100 716
#define PPUTLUINT_DECIMAL_1011001011 715
#define PPUTLUINT_DECIMAL_1011001010 714
#define PPUTLUINT_DECIMAL_1011001001 713
#define PPUTLUINT_DECIMAL_1011001000 712
#define PPUTLUINT_DECIMAL_1011000111 711
#define PPUTLUINT_DECIMAL_1011000110 710
#define PPUTLUINT_DECIMAL_1011000101 709
#define PPUTLUINT_DECIMAL_1011000100 708
#define PPUTLUINT_DECIMAL_1011000011 707
#define PPUTLUINT_DECIMAL_1011000010 706
#define PPUTLUINT_DECIMAL_1011000001 705
#define PPUTLUINT_DECIMAL_1011000000 704
#define PPUTLUINT_DECIMAL_1010111111 703
#define PPUTLUINT_DECIMAL_1010111110 702
#define PPUTLUINT_DECIMAL_1010111101 701
#define PPUTLUINT_DECIMAL_1010111100 700
#define PPUTLUINT_DECIMAL_1010111011 699
#define PPUTLUINT_DECIMAL_1010111010 698
#define PPUTLUINT_DECIMAL_1010111001 697
#define PPUTLUINT_DECIMAL_1010111000 696
#define PPUTLUINT_DECIMAL_1010110111 695
#define PPUTLUINT_DECIMAL_1010110110 694
#define PPUTLUINT_DECIMAL_1010110101 693
#define PPUTLUINT_DECIMAL_1010110100 692
#define PPUTLUINT_DECIMAL_1010110011 691
#define PPUTLUINT_DECIMAL_1010110010 690
#define PPUTLUINT_DECIMAL_1010110001 689
#define PPUTLUINT_DECIMAL_1010110000 688
#define PPUTLUINT_DECIMAL_1010101111 687
#define PPUTLUINT_DECIMAL_1010101110 686
#define PPUTLUINT_DECIMAL_1010101101 685
#define PPUTLUINT_DECIMAL_1010101100 684
#define PPUTLUINT_DECIMAL_1010101011 683
#define PPUTLUINT_DECIMAL_1010101010 682
#define PPUTLUINT_DECIMAL_1010101001 681
#define PPUTLUINT_DECIMAL_1010101000 680
#define PPUTLUINT_DECIMAL_1010100111 679
#define PPUTLUINT_DECIMAL_1010100110 678
#define PPUTLUINT_DECIMAL_1010100101 677
#define PPUTLUINT_DECIMAL_1010100100 676
#define PPUTLUINT_DECIMAL_1010100011 675
#define PPUTLUINT_DECIMAL_1010100010 674
#define PPUTLUINT_DECIMAL_1010100001 673
#define PPUTLUINT_DECIMAL_1010100000 672
#define PPUTLUINT_DECIMAL_1010011111 671
#define PPUTLUINT_DECIMAL_1010011110 670
#define PPUTLUINT_DECIMAL_1010011101 669
#define PPUTLUINT_DECIMAL_1010011100 668
#define PPUTLUINT_DECIMAL_1010011011 667
#define PPUTLUINT_DECIMAL_1010011010 666
#define PPUTLUINT_DECIMAL_1010011001 665
#define PPUTLUINT_DECIMAL_1010011000 664
#define PPUTLUINT_DECIMAL_1010010111 663
#define PPUTLUINT_DECIMAL_1010010110 662
#define PPUTLUINT_DECIMAL_1010010101 661
#define PPUTLUINT_DECIMAL_1010010100 660
#define PPUTLUINT_DECIMAL_1010010011 659
#define PPUTLUINT_DECIMAL_1010010010 658
#define PPUTLUINT_DECIMAL_1010010001 657
#define PPUTLUINT_DECIMAL_1010010000 656
#define PPUTLUINT_DECIMAL_1010001111 655
#define PPUTLUINT_DECIMAL_1010001110 654
#define PPUTLUINT_DECIMAL_1010001101 653
#define PPUTLUINT_DECIMAL_1010001100 652
#define PPUTLUINT_DECIMAL_1010001011 651
#define PPUTLUINT_DECIMAL_1010001010 650
#define PPUTLUINT_DECIMAL_1010001001 649
#define PPUTLUINT_DECIMAL_1010001000 648
#define PPUTLUINT_DECIMAL_1010000111 647
#define PPUTLUINT_DECIMAL_1010000110 646
#define PPUTLUINT_DECIMAL_1010000101 645
#define PPUTLUINT_DECIMAL_1010000100 644
#define PPUTLUINT_DECIMAL_1010000011 643
#define PPUTLUINT_DECIMAL_1010000010 642
#define PPUTLUINT_DECIMAL_1010000001 641
#define PPUTLUINT_DECIMAL_1010000000 640
#define PPUTLUINT_DECIMAL_1001111111 639
#define PPUTLUINT_DECIMAL_1001111110 638
#define PPUTLUINT_DECIMAL_1001111101 637
#define PPUTLUINT_DECIMAL_1001111100 636
#define PPUTLUINT_DECIMAL_1001111011 635
#define PPUTLUINT_DECIMAL_1001111010 634
#define PPUTLUINT_DECIMAL_1001111001 633
#define PPUTLUINT_DECIMAL_1001111000 632
#define PPUTLUINT_DECIMAL_1001110111 631
#define PPUTLUINT_DECIMAL_1001110110 630
#define PPUTLUINT_DECIMAL_1001110101 629
#define PPUTLUINT_DECIMAL_1001110100 628
#define PPUTLUINT_DECIMAL_1001110011 627
#define PPUTLUINT_DECIMAL_1001110010 626
#define PPUTLUINT_DECIMAL_1001110001 625
#define PPUTLUINT_DECIMAL_1001110000 624
#define PPUTLUINT_DECIMAL_1001101111 623
#define PPUTLUINT_DECIMAL_1001101110 622
#define PPUTLUINT_DECIMAL_1001101101 621
#define PPUTLUINT_DECIMAL_1001101100 620
#define PPUTLUINT_DECIMAL_1001101011 619
#define PPUTLUINT_DECIMAL_1001101010 618
#define PPUTLUINT_DECIMAL_1001101001 617
#define PPUTLUINT_DECIMAL_1001101000 616
#define PPUTLUINT_DECIMAL_1001100111 615
#define PPUTLUINT_DECIMAL_1001100110 614
#define PPUTLUINT_DECIMAL_1001100101 613
#define PPUTLUINT_DECIMAL_1001100100 612
#define PPUTLUINT_DECIMAL_1001100011 611
#define PPUTLUINT_DECIMAL_1001100010 610
#define PPUTLUINT_DECIMAL_1001100001 609
#define PPUTLUINT_DECIMAL_1001100000 608
#define PPUTLUINT_DECIMAL_1001011111 607
#define PPUTLUINT_DECIMAL_1001011110 606
#define PPUTLUINT_DECIMAL_1001011101 605
#define PPUTLUINT_DECIMAL_1001011100 604
#define PPUTLUINT_DECIMAL_1001011011 603
#define PPUTLUINT_DECIMAL_1001011010 602
#define PPUTLUINT_DECIMAL_1001011001 601
#define PPUTLUINT_DECIMAL_1001011000 600
#define PPUTLUINT_DECIMAL_1001010111 599
#define PPUTLUINT_DECIMAL_1001010110 598
#define PPUTLUINT_DECIMAL_1001010101 597
#define PPUTLUINT_DECIMAL_1001010100 596
#define PPUTLUINT_DECIMAL_1001010011 595
#define PPUTLUINT_DECIMAL_1001010010 594
#define PPUTLUINT_DECIMAL_1001010001 593
#define PPUTLUINT_DECIMAL_1001010000 592
#define PPUTLUINT_DECIMAL_1001001111 591
#define PPUTLUINT_DECIMAL_1001001110 590
#define PPUTLUINT_DECIMAL_1001001101 589
#define PPUTLUINT_DECIMAL_1001001100 588
#define PPUTLUINT_DECIMAL_1001001011 587
#define PPUTLUINT_DECIMAL_1001001010 586
#define PPUTLUINT_DECIMAL_1001001001 585
#define PPUTLUINT_DECIMAL_1001001000 584
#define PPUTLUINT_DECIMAL_1001000111 583
#define PPUTLUINT_DECIMAL_1001000110 582
#define PPUTLUINT_DECIMAL_1001000101 581
#define PPUTLUINT_DECIMAL_1001000100 580
#define PPUTLUINT_DECIMAL_1001000011 579
#define PPUTLUINT_DECIMAL_1001000010 578
#define PPUTLUINT_DECIMAL_1001000001 577
#define PPUTLUINT_DECIMAL_1001000000 576
#define PPUTLUINT_DECIMAL_1000111111 575
#define PPUTLUINT_DECIMAL_1000111110 574
#define PPUTLUINT_DECIMAL_1000111101 573
#define PPUTLUINT_DECIMAL_1000111100 572
#define PPUTLUINT_DECIMAL_1000111011 571
#define PPUTLUINT_DECIMAL_1000111010 570
#define PPUTLUINT_DECIMAL_1000111001 569
#define PPUTLUINT_DECIMAL_1000111000 568
#define PPUTLUINT_DECIMAL_1000110111 567
#define PPUTLUINT_DECIMAL_1000110110 566
#define PPUTLUINT_DECIMAL_1000110101 565
#define PPUTLUINT_DECIMAL_1000110100 564
#define PPUTLUINT_DECIMAL_1000110011 563
#define PPUTLUINT_DECIMAL_1000110010 562
#define PPUTLUINT_DECIMAL_1000110001 561
#define PPUTLUINT_DECIMAL_1000110000 560
#define PPUTLUINT_DECIMAL_1000101111 559
#define PPUTLUINT_DECIMAL_1000101110 558
#define PPUTLUINT_DECIMAL_1000101101 557
#define PPUTLUINT_DECIMAL_1000101100 556
#define PPUTLUINT_DECIMAL_1000101011 555
#define PPUTLUINT_DECIMAL_1000101010 554
#define PPUTLUINT_DECIMAL_1000101001 553
#define PPUTLUINT_DECIMAL_1000101000 552
#define PPUTLUINT_DECIMAL_1000100111 551
#define PPUTLUINT_DECIMAL_1000100110 550
#define PPUTLUINT_DECIMAL_1000100101 549
#define PPUTLUINT_DECIMAL_1000100100 548
#define PPUTLUINT_DECIMAL_1000100011 547
#define PPUTLUINT_DECIMAL_1000100010 546
#define PPUTLUINT_DECIMAL_1000100001 545
#define PPUTLUINT_DECIMAL_1000100000 544
#define PPUTLUINT_DECIMAL_1000011111 543
#define PPUTLUINT_DECIMAL_1000011110 542
#define PPUTLUINT_DECIMAL_1000011101 541
#define PPUTLUINT_DECIMAL_1000011100 540
#define PPUTLUINT_DECIMAL_1000011011 539
#define PPUTLUINT_DECIMAL_1000011010 538
#define PPUTLUINT_DECIMAL_1000011001 537
#define PPUTLUINT_DECIMAL_1000011000 536
#define PPUTLUINT_DECIMAL_1000010111 535
#define PPUTLUINT_DECIMAL_1000010110 534
#define PPUTLUINT_DECIMAL_1000010101 533
#define PPUTLUINT_DECIMAL_1000010100 532
#define PPUTLUINT_DECIMAL_1000010011 531
#define PPUTLUINT_DECIMAL_1000010010 530
#define PPUTLUINT_DECIMAL_1000010001 529
#define PPUTLUINT_DECIMAL_1000010000 528
#define PPUTLUINT_DECIMAL_1000001111 527
#define PPUTLUINT_DECIMAL_1000001110 526
#define PPUTLUINT_DECIMAL_1000001101 525
#define PPUTLUINT_DECIMAL_1000001100 524
#define PPUTLUINT_DECIMAL_1000001011 523
#define PPUTLUINT_DECIMAL_1000001010 522
#define PPUTLUINT_DECIMAL_1000001001 521
#define PPUTLUINT_DECIMAL_1000001000 520
#define PPUTLUINT_DECIMAL_1000000111 519
#define PPUTLUINT_DECIMAL_1000000110 518
#define PPUTLUINT_DECIMAL_1000000101 517
#define PPUTLUINT_DECIMAL_1000000100 516
#define PPUTLUINT_DECIMAL_1000000011 515
#define PPUTLUINT_DECIMAL_1000000010 514
#define PPUTLUINT_DECIMAL_1000000001 513
#define PPUTLUINT_DECIMAL_1000000000 512
#define PPUTLUINT_DECIMAL_0111111111 511
#define PPUTLUINT_DECIMAL_0111111110 510
#define PPUTLUINT_DECIMAL_0111111101 509
#define PPUTLUINT_DECIMAL_0111111100 508
#define PPUTLUINT_DECIMAL_0111111011 507
#define PPUTLUINT_DECIMAL_0111111010 506
#define PPUTLUINT_DECIMAL_0111111001 505
#define PPUTLUINT_DECIMAL_0111111000 504
#define PPUTLUINT_DECIMAL_0111110111 503
#define PPUTLUINT_DECIMAL_0111110110 502
#define PPUTLUINT_DECIMAL_0111110101 501
#define PPUTLUINT_DECIMAL_0111110100 500
#define PPUTLUINT_DECIMAL_0111110011 499
#define PPUTLUINT_DECIMAL_0111110010 498
#define PPUTLUINT_DECIMAL_0111110001 497
#define PPUTLUINT_DECIMAL_0111110000 496
#define PPUTLUINT_DECIMAL_0111101111 495
#define PPUTLUINT_DECIMAL_0111101110 494
#define PPUTLUINT_DECIMAL_0111101101 493
#define PPUTLUINT_DECIMAL_0111101100 492
#define PPUTLUINT_DECIMAL_0111101011 491
#define PPUTLUINT_DECIMAL_0111101010 490
#define PPUTLUINT_DECIMAL_0111101001 489
#define PPUTLUINT_DECIMAL_0111101000 488
#define PPUTLUINT_DECIMAL_0111100111 487
#define PPUTLUINT_DECIMAL_0111100110 486
#define PPUTLUINT_DECIMAL_0111100101 485
#define PPUTLUINT_DECIMAL_0111100100 484
#define PPUTLUINT_DECIMAL_0111100011 483
#define PPUTLUINT_DECIMAL_0111100010 482
#define PPUTLUINT_DECIMAL_0111100001 481
#define PPUTLUINT_DECIMAL_0111100000 480
#define PPUTLUINT_DECIMAL_0111011111 479
#define PPUTLUINT_DECIMAL_0111011110 478
#define PPUTLUINT_DECIMAL_0111011101 477
#define PPUTLUINT_DECIMAL_0111011100 476
#define PPUTLUINT_DECIMAL_0111011011 475
#define PPUTLUINT_DECIMAL_0111011010 474
#define PPUTLUINT_DECIMAL_0111011001 473
#define PPUTLUINT_DECIMAL_0111011000 472
#define PPUTLUINT_DECIMAL_0111010111 471
#define PPUTLUINT_DECIMAL_0111010110 470
#define PPUTLUINT_DECIMAL_0111010101 469
#define PPUTLUINT_DECIMAL_0111010100 468
#define PPUTLUINT_DECIMAL_0111010011 467
#define PPUTLUINT_DECIMAL_0111010010 466
#define PPUTLUINT_DECIMAL_0111010001 465
#define PPUTLUINT_DECIMAL_0111010000 464
#define PPUTLUINT_DECIMAL_0111001111 463
#define PPUTLUINT_DECIMAL_0111001110 462
#define PPUTLUINT_DECIMAL_0111001101 461
#define PPUTLUINT_DECIMAL_0111001100 460
#define PPUTLUINT_DECIMAL_0111001011 459
#define PPUTLUINT_DECIMAL_0111001010 458
#define PPUTLUINT_DECIMAL_0111001001 457
#define PPUTLUINT_DECIMAL_0111001000 456
#define PPUTLUINT_DECIMAL_0111000111 455
#define PPUTLUINT_DECIMAL_0111000110 454
#define PPUTLUINT_DECIMAL_0111000101 453
#define PPUTLUINT_DECIMAL_0111000100 452
#define PPUTLUINT_DECIMAL_0111000011 451
#define PPUTLUINT_DECIMAL_0111000010 450
#define PPUTLUINT_DECIMAL_0111000001 449
#define PPUTLUINT_DECIMAL_0111000000 448
#define PPUTLUINT_DECIMAL_0110111111 447
#define PPUTLUINT_DECIMAL_0110111110 446
#define PPUTLUINT_DECIMAL_0110111101 445
#define PPUTLUINT_DECIMAL_0110111100 444
#define PPUTLUINT_DECIMAL_0110111011 443
#define PPUTLUINT_DECIMAL_0110111010 442
#define PPUTLUINT_DECIMAL_0110111001 441
#define PPUTLUINT_DECIMAL_0110111000 440
#define PPUTLUINT_DECIMAL_0110110111 439
#define PPUTLUINT_DECIMAL_0110110110 438
#define PPUTLUINT_DECIMAL_0110110101 437
#define PPUTLUINT_DECIMAL_0110110100 436
#define PPUTLUINT_DECIMAL_0110110011 435
#define PPUTLUINT_DECIMAL_0110110010 434
#define PPUTLUINT_DECIMAL_0110110001 433
#define PPUTLUINT_DECIMAL_0110110000 432
#define PPUTLUINT_DECIMAL_0110101111 431
#define PPUTLUINT_DECIMAL_0110101110 430
#define PPUTLUINT_DECIMAL_0110101101 429
#define PPUTLUINT_DECIMAL_0110101100 428
#define PPUTLUINT_DECIMAL_0110101011 427
#define PPUTLUINT_DECIMAL_0110101010 426
#define PPUTLUINT_DECIMAL_0110101001 425
#define PPUTLUINT_DECIMAL_0110101000 424
#define PPUTLUINT_DECIMAL_0110100111 423
#define PPUTLUINT_DECIMAL_0110100110 422
#define PPUTLUINT_DECIMAL_0110100101 421
#define PPUTLUINT_DECIMAL_0110100100 420
#define PPUTLUINT_DECIMAL_0110100011 419
#define PPUTLUINT_DECIMAL_0110100010 418
#define PPUTLUINT_DECIMAL_0110100001 417
#define PPUTLUINT_DECIMAL_0110100000 416
#define PPUTLUINT_DECIMAL_0110011111 415
#define PPUTLUINT_DECIMAL_0110011110 414
#define PPUTLUINT_DECIMAL_0110011101 413
#define PPUTLUINT_DECIMAL_0110011100 412
#define PPUTLUINT_DECIMAL_0110011011 411
#define PPUTLUINT_DECIMAL_0110011010 410
#define PPUTLUINT_DECIMAL_0110011001 409
#define PPUTLUINT_DECIMAL_0110011000 408
#define PPUTLUINT_DECIMAL_0110010111 407
#define PPUTLUINT_DECIMAL_0110010110 406
#define PPUTLUINT_DECIMAL_0110010101 405
#define PPUTLUINT_DECIMAL_0110010100 404
#define PPUTLUINT_DECIMAL_0110010011 403
#define PPUTLUINT_DECIMAL_0110010010 402
#define PPUTLUINT_DECIMAL_0110010001 401
#define PPUTLUINT_DECIMAL_0110010000 400
#define PPUTLUINT_DECIMAL_0110001111 399
#define PPUTLUINT_DECIMAL_0110001110 398
#define PPUTLUINT_DECIMAL_0110001101 397
#define PPUTLUINT_DECIMAL_0110001100 396
#define PPUTLUINT_DECIMAL_0110001011 395
#define PPUTLUINT_DECIMAL_0110001010 394
#define PPUTLUINT_DECIMAL_0110001001 393
#define PPUTLUINT_DECIMAL_0110001000 392
#define PPUTLUINT_DECIMAL_0110000111 391
#define PPUTLUINT_DECIMAL_0110000110 390
#define PPUTLUINT_DECIMAL_0110000101 389
#define PPUTLUINT_DECIMAL_0110000100 388
#define PPUTLUINT_DECIMAL_0110000011 387
#define PPUTLUINT_DECIMAL_0110000010 386
#define PPUTLUINT_DECIMAL_0110000001 385
#define PPUTLUINT_DECIMAL_0110000000 384
#define PPUTLUINT_DECIMAL_0101111111 383
#define PPUTLUINT_DECIMAL_0101111110 382
#define PPUTLUINT_DECIMAL_0101111101 381
#define PPUTLUINT_DECIMAL_0101111100 380
#define PPUTLUINT_DECIMAL_0101111011 379
#define PPUTLUINT_DECIMAL_0101111010 378
#define PPUTLUINT_DECIMAL_0101111001 377
#define PPUTLUINT_DECIMAL_0101111000 376
#define PPUTLUINT_DECIMAL_0101110111 375
#define PPUTLUINT_DECIMAL_0101110110 374
#define PPUTLUINT_DECIMAL_0101110101 373
#define PPUTLUINT_DECIMAL_0101110100 372
#define PPUTLUINT_DECIMAL_0101110011 371
#define PPUTLUINT_DECIMAL_0101110010 370
#define PPUTLUINT_DECIMAL_0101110001 369
#define PPUTLUINT_DECIMAL_0101110000 368
#define PPUTLUINT_DECIMAL_0101101111 367
#define PPUTLUINT_DECIMAL_0101101110 366
#define PPUTLUINT_DECIMAL_0101101101 365
#define PPUTLUINT_DECIMAL_0101101100 364
#define PPUTLUINT_DECIMAL_0101101011 363
#define PPUTLUINT_DECIMAL_0101101010 362
#define PPUTLUINT_DECIMAL_0101101001 361
#define PPUTLUINT_DECIMAL_0101101000 360
#define PPUTLUINT_DECIMAL_0101100111 359
#define PPUTLUINT_DECIMAL_0101100110 358
#define PPUTLUINT_DECIMAL_0101100101 357
#define PPUTLUINT_DECIMAL_0101100100 356
#define PPUTLUINT_DECIMAL_0101100011 355
#define PPUTLUINT_DECIMAL_0101100010 354
#define PPUTLUINT_DECIMAL_0101100001 353
#define PPUTLUINT_DECIMAL_0101100000 352
#define PPUTLUINT_DECIMAL_0101011111 351
#define PPUTLUINT_DECIMAL_0101011110 350
#define PPUTLUINT_DECIMAL_0101011101 349
#define PPUTLUINT_DECIMAL_0101011100 348
#define PPUTLUINT_DECIMAL_0101011011 347
#define PPUTLUINT_DECIMAL_0101011010 346
#define PPUTLUINT_DECIMAL_0101011001 345
#define PPUTLUINT_DECIMAL_0101011000 344
#define PPUTLUINT_DECIMAL_0101010111 343
#define PPUTLUINT_DECIMAL_0101010110 342
#define PPUTLUINT_DECIMAL_0101010101 341
#define PPUTLUINT_DECIMAL_0101010100 340
#define PPUTLUINT_DECIMAL_0101010011 339
#define PPUTLUINT_DECIMAL_0101010010 338
#define PPUTLUINT_DECIMAL_0101010001 337
#define PPUTLUINT_DECIMAL_0101010000 336
#define PPUTLUINT_DECIMAL_0101001111 335
#define PPUTLUINT_DECIMAL_0101001110 334
#define PPUTLUINT_DECIMAL_0101001101 333
#define PPUTLUINT_DECIMAL_0101001100 332
#define PPUTLUINT_DECIMAL_0101001011 331
#define PPUTLUINT_DECIMAL_0101001010 330
#define PPUTLUINT_DECIMAL_0101001001 329
#define PPUTLUINT_DECIMAL_0101001000 328
#define PPUTLUINT_DECIMAL_0101000111 327
#define PPUTLUINT_DECIMAL_0101000110 326
#define PPUTLUINT_DECIMAL_0101000101 325
#define PPUTLUINT_DECIMAL_0101000100 324
#define PPUTLUINT_DECIMAL_0101000011 323
#define PPUTLUINT_DECIMAL_0101000010 322
#define PPUTLUINT_DECIMAL_0101000001 321
#define PPUTLUINT_DECIMAL_0101000000 320
#define PPUTLUINT_DECIMAL_0100111111 319
#define PPUTLUINT_DECIMAL_0100111110 318
#define PPUTLUINT_DECIMAL_0100111101 317
#define PPUTLUINT_DECIMAL_0100111100 316
#define PPUTLUINT_DECIMAL_0100111011 315
#define PPUTLUINT_DECIMAL_0100111010 314
#define PPUTLUINT_DECIMAL_0100111001 313
#define PPUTLUINT_DECIMAL_0100111000 312
#define PPUTLUINT_DECIMAL_0100110111 311
#define PPUTLUINT_DECIMAL_0100110110 310
#define PPUTLUINT_DECIMAL_0100110101 309
#define PPUTLUINT_DECIMAL_0100110100 308
#define PPUTLUINT_DECIMAL_0100110011 307
#define PPUTLUINT_DECIMAL_0100110010 306
#define PPUTLUINT_DECIMAL_0100110001 305
#define PPUTLUINT_DECIMAL_0100110000 304
#define PPUTLUINT_DECIMAL_0100101111 303
#define PPUTLUINT_DECIMAL_0100101110 302
#define PPUTLUINT_DECIMAL_0100101101 301
#define PPUTLUINT_DECIMAL_0100101100 300
#define PPUTLUINT_DECIMAL_0100101011 299
#define PPUTLUINT_DECIMAL_0100101010 298
#define PPUTLUINT_DECIMAL_0100101001 297
#define PPUTLUINT_DECIMAL_0100101000 296
#define PPUTLUINT_DECIMAL_0100100111 295
#define PPUTLUINT_DECIMAL_0100100110 294
#define PPUTLUINT_DECIMAL_0100100101 293
#define PPUTLUINT_DECIMAL_0100100100 292
#define PPUTLUINT_DECIMAL_0100100011 291
#define PPUTLUINT_DECIMAL_0100100010 290
#define PPUTLUINT_DECIMAL_0100100001 289
#define PPUTLUINT_DECIMAL_0100100000 288
#define PPUTLUINT_DECIMAL_0100011111 287
#define PPUTLUINT_DECIMAL_0100011110 286
#define PPUTLUINT_DECIMAL_0100011101 285
#define PPUTLUINT_DECIMAL_0100011100 284
#define PPUTLUINT_DECIMAL_0100011011 283
#define PPUTLUINT_DECIMAL_0100011010 282
#define PPUTLUINT_DECIMAL_0100011001 281
#define PPUTLUINT_DECIMAL_0100011000 280
#define PPUTLUINT_DECIMAL_0100010111 279
#define PPUTLUINT_DECIMAL_0100010110 278
#define PPUTLUINT_DECIMAL_0100010101 277
#define PPUTLUINT_DECIMAL_0100010100 276
#define PPUTLUINT_DECIMAL_0100010011 275
#define PPUTLUINT_DECIMAL_0100010010 274
#define PPUTLUINT_DECIMAL_0100010001 273
#define PPUTLUINT_DECIMAL_0100010000 272
#define PPUTLUINT_DECIMAL_0100001111 271
#define PPUTLUINT_DECIMAL_0100001110 270
#define PPUTLUINT_DECIMAL_0100001101 269
#define PPUTLUINT_DECIMAL_0100001100 268
#define PPUTLUINT_DECIMAL_0100001011 267
#define PPUTLUINT_DECIMAL_0100001010 266
#define PPUTLUINT_DECIMAL_0100001001 265
#define PPUTLUINT_DECIMAL_0100001000 264
#define PPUTLUINT_DECIMAL_0100000111 263
#define PPUTLUINT_DECIMAL_0100000110 262
#define PPUTLUINT_DECIMAL_0100000101 261
#define PPUTLUINT_DECIMAL_0100000100 260
#define PPUTLUINT_DECIMAL_0100000011 259
#define PPUTLUINT_DECIMAL_0100000010 258
#define PPUTLUINT_DECIMAL_0100000001 257
#define PPUTLUINT_DECIMAL_0100000000 256
#define PPUTLUINT_DECIMAL_0011111111 255
#define PPUTLUINT_DECIMAL_0011111110 254
#define PPUTLUINT_DECIMAL_0011111101 253
#define PPUTLUINT_DECIMAL_0011111100 252
#define PPUTLUINT_DECIMAL_0011111011 251
#define PPUTLUINT_DECIMAL_0011111010 250
#define PPUTLUINT_DECIMAL_0011111001 249
#define PPUTLUINT_DECIMAL_0011111000 248
#define PPUTLUINT_DECIMAL_0011110111 247
#define PPUTLUINT_DECIMAL_0011110110 246
#define PPUTLUINT_DECIMAL_0011110101 245
#define PPUTLUINT_DECIMAL_0011110100 244
#define PPUTLUINT_DECIMAL_0011110011 243
#define PPUTLUINT_DECIMAL_0011110010 242
#define PPUTLUINT_DECIMAL_0011110001 241
#define PPUTLUINT_DECIMAL_0011110000 240
#define PPUTLUINT_DECIMAL_0011101111 239
#define PPUTLUINT_DECIMAL_0011101110 238
#define PPUTLUINT_DECIMAL_0011101101 237
#define PPUTLUINT_DECIMAL_0011101100 236
#define PPUTLUINT_DECIMAL_0011101011 235
#define PPUTLUINT_DECIMAL_0011101010 234
#define PPUTLUINT_DECIMAL_0011101001 233
#define PPUTLUINT_DECIMAL_0011101000 232
#define PPUTLUINT_DECIMAL_0011100111 231
#define PPUTLUINT_DECIMAL_0011100110 230
#define PPUTLUINT_DECIMAL_0011100101 229
#define PPUTLUINT_DECIMAL_0011100100 228
#define PPUTLUINT_DECIMAL_0011100011 227
#define PPUTLUINT_DECIMAL_0011100010 226
#define PPUTLUINT_DECIMAL_0011100001 225
#define PPUTLUINT_DECIMAL_0011100000 224
#define PPUTLUINT_DECIMAL_0011011111 223
#define PPUTLUINT_DECIMAL_0011011110 222
#define PPUTLUINT_DECIMAL_0011011101 221
#define PPUTLUINT_DECIMAL_0011011100 220
#define PPUTLUINT_DECIMAL_0011011011 219
#define PPUTLUINT_DECIMAL_0011011010 218
#define PPUTLUINT_DECIMAL_0011011001 217
#define PPUTLUINT_DECIMAL_0011011000 216
#define PPUTLUINT_DECIMAL_0011010111 215
#define PPUTLUINT_DECIMAL_0011010110 214
#define PPUTLUINT_DECIMAL_0011010101 213
#define PPUTLUINT_DECIMAL_0011010100 212
#define PPUTLUINT_DECIMAL_0011010011 211
#define PPUTLUINT_DECIMAL_0011010010 210
#define PPUTLUINT_DECIMAL_0011010001 209
#define PPUTLUINT_DECIMAL_0011010000 208
#define PPUTLUINT_DECIMAL_0011001111 207
#define PPUTLUINT_DECIMAL_0011001110 206
#define PPUTLUINT_DECIMAL_0011001101 205
#define PPUTLUINT_DECIMAL_0011001100 204
#define PPUTLUINT_DECIMAL_0011001011 203
#define PPUTLUINT_DECIMAL_0011001010 202
#define PPUTLUINT_DECIMAL_0011001001 201
#define PPUTLUINT_DECIMAL_0011001000 200
#define PPUTLUINT_DECIMAL_0011000111 199
#define PPUTLUINT_DECIMAL_0011000110 198
#define PPUTLUINT_DECIMAL_0011000101 197
#define PPUTLUINT_DECIMAL_0011000100 196
#define PPUTLUINT_DECIMAL_0011000011 195
#define PPUTLUINT_DECIMAL_0011000010 194
#define PPUTLUINT_DECIMAL_0011000001 193
#define PPUTLUINT_DECIMAL_0011000000 192
#define PPUTLUINT_DECIMAL_0010111111 191
#define PPUTLUINT_DECIMAL_0010111110 190
#define PPUTLUINT_DECIMAL_0010111101 189
#define PPUTLUINT_DECIMAL_0010111100 188
#define PPUTLUINT_DECIMAL_0010111011 187
#define PPUTLUINT_DECIMAL_0010111010 186
#define PPUTLUINT_DECIMAL_0010111001 185
#define PPUTLUINT_DECIMAL_0010111000 184
#define PPUTLUINT_DECIMAL_0010110111 183
#define PPUTLUINT_DECIMAL_0010110110 182
#define PPUTLUINT_DECIMAL_0010110101 181
#define PPUTLUINT_DECIMAL_0010110100 180
#define PPUTLUINT_DECIMAL_0010110011 179
#define PPUTLUINT_DECIMAL_0010110010 178
#define PPUTLUINT_DECIMAL_0010110001 177
#define PPUTLUINT_DECIMAL_0010110000 176
#define PPUTLUINT_DECIMAL_0010101111 175
#define PPUTLUINT_DECIMAL_0010101110 174
#define PPUTLUINT_DECIMAL_0010101101 173
#define PPUTLUINT_DECIMAL_0010101100 172
#define PPUTLUINT_DECIMAL_0010101011 171
#define PPUTLUINT_DECIMAL_0010101010 170
#define PPUTLUINT_DECIMAL_0010101001 169
#define PPUTLUINT_DECIMAL_0010101000 168
#define PPUTLUINT_DECIMAL_0010100111 167
#define PPUTLUINT_DECIMAL_0010100110 166
#define PPUTLUINT_DECIMAL_0010100101 165
#define PPUTLUINT_DECIMAL_0010100100 164
#define PPUTLUINT_DECIMAL_0010100011 163
#define PPUTLUINT_DECIMAL_0010100010 162
#define PPUTLUINT_DECIMAL_0010100001 161
#define PPUTLUINT_DECIMAL_0010100000 160
#define PPUTLUINT_DECIMAL_0010011111 159
#define PPUTLUINT_DECIMAL_0010011110 158
#define PPUTLUINT_DECIMAL_0010011101 157
#define PPUTLUINT_DECIMAL_0010011100 156
#define PPUTLUINT_DECIMAL_0010011011 155
#define PPUTLUINT_DECIMAL_0010011010 154
#define PPUTLUINT_DECIMAL_0010011001 153
#define PPUTLUINT_DECIMAL_0010011000 152
#define PPUTLUINT_DECIMAL_0010010111 151
#define PPUTLUINT_DECIMAL_0010010110 150
#define PPUTLUINT_DECIMAL_0010010101 149
#define PPUTLUINT_DECIMAL_0010010100 148
#define PPUTLUINT_DECIMAL_0010010011 147
#define PPUTLUINT_DECIMAL_0010010010 146
#define PPUTLUINT_DECIMAL_0010010001 145
#define PPUTLUINT_DECIMAL_0010010000 144
#define PPUTLUINT_DECIMAL_0010001111 143
#define PPUTLUINT_DECIMAL_0010001110 142
#define PPUTLUINT_DECIMAL_0010001101 141
#define PPUTLUINT_DECIMAL_0010001100 140
#define PPUTLUINT_DECIMAL_0010001011 139
#define PPUTLUINT_DECIMAL_0010001010 138
#define PPUTLUINT_DECIMAL_0010001001 137
#define PPUTLUINT_DECIMAL_0010001000 136
#define PPUTLUINT_DECIMAL_0010000111 135
#define PPUTLUINT_DECIMAL_0010000110 134
#define PPUTLUINT_DECIMAL_0010000101 133
#define PPUTLUINT_DECIMAL_0010000100 132
#define PPUTLUINT_DECIMAL_0010000011 131
#define PPUTLUINT_DECIMAL_0010000010 130
#define PPUTLUINT_DECIMAL_0010000001 129
#define PPUTLUINT_DECIMAL_0010000000 128
#define PPUTLUINT_DECIMAL_0001111111 127
#define PPUTLUINT_DECIMAL_0001111110 126
#define PPUTLUINT_DECIMAL_0001111101 125
#define PPUTLUINT_DECIMAL_0001111100 124
#define PPUTLUINT_DECIMAL_0001111011 123
#define PPUTLUINT_DECIMAL_0001111010 122
#define PPUTLUINT_DECIMAL_0001111001 121
#define PPUTLUINT_DECIMAL_0001111000 120
#define PPUTLUINT_DECIMAL_0001110111 119
#define PPUTLUINT_DECIMAL_0001110110 118
#define PPUTLUINT_DECIMAL_0001110101 117
#define PPUTLUINT_DECIMAL_0001110100 116
#define PPUTLUINT_DECIMAL_0001110011 115
#define PPUTLUINT_DECIMAL_0001110010 114
#define PPUTLUINT_DECIMAL_0001110001 113
#define PPUTLUINT_DECIMAL_0001110000 112
#define PPUTLUINT_DECIMAL_0001101111 111
#define PPUTLUINT_DECIMAL_0001101110 110
#define PPUTLUINT_DECIMAL_0001101101 109
#define PPUTLUINT_DECIMAL_0001101100 108
#define PPUTLUINT_DECIMAL_0001101011 107
#define PPUTLUINT_DECIMAL_0001101010 106
#define PPUTLUINT_DECIMAL_0001101001 105
#define PPUTLUINT_DECIMAL_0001101000 104
#define PPUTLUINT_DECIMAL_0001100111 103
#define PPUTLUINT_DECIMAL_0001100110 102
#define PPUTLUINT_DECIMAL_0001100101 101
#define PPUTLUINT_DECIMAL_0001100100 100
#define PPUTLUINT_DECIMAL_0001100011 99
#define PPUTLUINT_DECIMAL_0001100010 98
#define PPUTLUINT_DECIMAL_0001100001 97
#define PPUTLUINT_DECIMAL_0001100000 96
#define PPUTLUINT_DECIMAL_0001011111 95
#define PPUTLUINT_DECIMAL_0001011110 94
#define PPUTLUINT_DECIMAL_0001011101 93
#define PPUTLUINT_DECIMAL_0001011100 92
#define PPUTLUINT_DECIMAL_0001011011 91
#define PPUTLUINT_DECIMAL_0001011010 90
#define PPUTLUINT_DECIMAL_0001011001 89
#define PPUTLUINT_DECIMAL_0001011000 88
#define PPUTLUINT_DECIMAL_0001010111 87
#define PPUTLUINT_DECIMAL_0001010110 86
#define PPUTLUINT_DECIMAL_0001010101 85
#define PPUTLUINT_DECIMAL_0001010100 84
#define PPUTLUINT_DECIMAL_0001010011 83
#define PPUTLUINT_DECIMAL_0001010010 82
#define PPUTLUINT_DECIMAL_0001010001 81
#define PPUTLUINT_DECIMAL_0001010000 80
#define PPUTLUINT_DECIMAL_0001001111 79
#define PPUTLUINT_DECIMAL_0001001110 78
#define PPUTLUINT_DECIMAL_0001001101 77
#define PPUTLUINT_DECIMAL_0001001100 76
#define PPUTLUINT_DECIMAL_0001001011 75
#define PPUTLUINT_DECIMAL_0001001010 74
#define PPUTLUINT_DECIMAL_0001001001 73
#define PPUTLUINT_DECIMAL_0001001000 72
#define PPUTLUINT_DECIMAL_0001000111 71
#define PPUTLUINT_DECIMAL_0001000110 70
#define PPUTLUINT_DECIMAL_0001000101 69
#define PPUTLUINT_DECIMAL_0001000100 68
#define PPUTLUINT_DECIMAL_0001000011 67
#define PPUTLUINT_DECIMAL_0001000010 66
#define PPUTLUINT_DECIMAL_0001000001 65
#define PPUTLUINT_DECIMAL_0001000000 64
#define PPUTLUINT_DECIMAL_0000111111 63
#define PPUTLUINT_DECIMAL_0000111110 62
#define PPUTLUINT_DECIMAL_0000111101 61
#define PPUTLUINT_DECIMAL_0000111100 60
#define PPUTLUINT_DECIMAL_0000111011 59
#define PPUTLUINT_DECIMAL_0000111010 58
#define PPUTLUINT_DECIMAL_0000111001 57
#define PPUTLUINT_DECIMAL_0000111000 56
#define PPUTLUINT_DECIMAL_0000110111 55
#define PPUTLUINT_DECIMAL_0000110110 54
#define PPUTLUINT_DECIMAL_0000110101 53
#define PPUTLUINT_DECIMAL_0000110100 52
#define PPUTLUINT_DECIMAL_0000110011 51
#define PPUTLUINT_DECIMAL_0000110010 50
#define PPUTLUINT_DECIMAL_0000110001 49
#define PPUTLUINT_DECIMAL_0000110000 48
#define PPUTLUINT_DECIMAL_0000101111 47
#define PPUTLUINT_DECIMAL_0000101110 46
#define PPUTLUINT_DECIMAL_0000101101 45
#define PPUTLUINT_DECIMAL_0000101100 44
#define PPUTLUINT_DECIMAL_0000101011 43
#define PPUTLUINT_DECIMAL_0000101010 42
#define PPUTLUINT_DECIMAL_0000101001 41
#define PPUTLUINT_DECIMAL_0000101000 40
#define PPUTLUINT_DECIMAL_0000100111 39
#define PPUTLUINT_DECIMAL_0000100110 38
#define PPUTLUINT_DECIMAL_0000100101 37
#define PPUTLUINT_DECIMAL_0000100100 36
#define PPUTLUINT_DECIMAL_0000100011 35
#define PPUTLUINT_DECIMAL_0000100010 34
#define PPUTLUINT_DECIMAL_0000100001 33
#define PPUTLUINT_DECIMAL_0000100000 32
#define PPUTLUINT_DECIMAL_0000011111 31
#define PPUTLUINT_DECIMAL_0000011110 30
#define PPUTLUINT_DECIMAL_0000011101 29
#define PPUTLUINT_DECIMAL_0000011100 28
#define PPUTLUINT_DECIMAL_0000011011 27
#define PPUTLUINT_DECIMAL_0000011010 26
#define PPUTLUINT_DECIMAL_0000011001 25
#define PPUTLUINT_DECIMAL_0000011000 24
#define PPUTLUINT_DECIMAL_0000010111 23
#define PPUTLUINT_DECIMAL_0000010110 22
#define PPUTLUINT_DECIMAL_0000010101 21
#define PPUTLUINT_DECIMAL_0000010100 20
#define PPUTLUINT_DECIMAL_0000010011 19
#define PPUTLUINT_DECIMAL_0000010010 18
#define PPUTLUINT_DECIMAL_0000010001 17
#define PPUTLUINT_DECIMAL_0000010000 16
#define PPUTLUINT_DECIMAL_0000001111 15
#define PPUTLUINT_DECIMAL_0000001110 14
#define PPUTLUINT_DECIMAL_0000001101 13
#define PPUTLUINT_DECIMAL_0000001100 12
#define PPUTLUINT_DECIMAL_0000001011 11
#define PPUTLUINT_DECIMAL_0000001010 10
#define PPUTLUINT_DECIMAL_0000001001 9
#define PPUTLUINT_DECIMAL_0000001000 8
#define PPUTLUINT_DECIMAL_0000000111 7
#define PPUTLUINT_DECIMAL_0000000110 6
#define PPUTLUINT_DECIMAL_0000000101 5
#define PPUTLUINT_DECIMAL_0000000100 4
#define PPUTLUINT_DECIMAL_0000000011 3
#define PPUTLUINT_DECIMAL_0000000010 2
#define PPUTLUINT_DECIMAL_0000000001 1
#define PPUTLUINT_DECIMAL_0000000000 0

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

/// [traits.size]
/// -------------
/// O(1) variadic argument size computation.
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

/// [bitwise.bitnot]
/// ----------------
/// bitwise NOT.
#define PTL_BITNOT(/* v: binary */...) /* -> ~v */ PTL_IF(__VA_ARGS__, (0), (1))

// vim: fdm=marker:fmr={{{,}}}

#endif

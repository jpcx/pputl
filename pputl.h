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
//    represented using an '0b' prefix and 'u' suffix and their bit length    //
//    is always fixed to the configured uint bits.                            //
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
/// binary bit length is fixed at PTL_UINT_BITS (10).
///
/// PTL_UINT(0)             // 0
/// PTL_UINT(1)             // 1
/// PTL_UINT(2)             // 2
/// PTL_UINT(1023)          // 1023
/// PTL_UINT(0b0000000000u) // 0b0000000000u
/// PTL_UINT(0b1111111111u) // 0b1111111111u
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
#define PPUTLUINT_OOO(...)          PPUTLUINT_OOO_RES(PPUTLUTRAITS_##__VA_ARGS__)
#define PPUTLUINT_OOO_RES(...)      PPUTLUINT_OOO_RES_X(__VA_ARGS__)
#define PPUTLUINT_OOO_RES_X(_, ...) PPUTLUINT_OOO_##__VA_OPT__(NO_)##FAIL()
#define PPUTLUINT_OOO_NO_FAIL(...)  PPUTLUINT_PASS
#define PPUTLUINT_OOO_FAIL(...)     PPUTLUINT_FAIL

/// fourth parentheses; returns
#define PPUTLUINT_PASS(err, ...) __VA_ARGS__
#define PPUTLUINT_FAIL(err, ...) PTL_FAIL(err)

/// binary, log2, sqrt, factors
#define PPUTLUTRAITS_1023 0b1111111111u, 9, 31, (3, 11, 31)
#define PPUTLUTRAITS_1022 0b1111111110u, 9, 31, (2, 7, 73)
#define PPUTLUTRAITS_1021 0b1111111101u, 9, 31, ()
#define PPUTLUTRAITS_1020 0b1111111100u, 9, 31, (2, 2, 3, 5, 17)
#define PPUTLUTRAITS_1019 0b1111111011u, 9, 31, ()
#define PPUTLUTRAITS_1018 0b1111111010u, 9, 31, (2, 509)
#define PPUTLUTRAITS_1017 0b1111111001u, 9, 31, (3, 3, 113)
#define PPUTLUTRAITS_1016 0b1111111000u, 9, 31, (2, 2, 2, 127)
#define PPUTLUTRAITS_1015 0b1111110111u, 9, 31, (5, 7, 29)
#define PPUTLUTRAITS_1014 0b1111110110u, 9, 31, (2, 3, 13, 13)
#define PPUTLUTRAITS_1013 0b1111110101u, 9, 31, ()
#define PPUTLUTRAITS_1012 0b1111110100u, 9, 31, (2, 2, 11, 23)
#define PPUTLUTRAITS_1011 0b1111110011u, 9, 31, (3, 337)
#define PPUTLUTRAITS_1010 0b1111110010u, 9, 31, (2, 5, 101)
#define PPUTLUTRAITS_1009 0b1111110001u, 9, 31, ()
#define PPUTLUTRAITS_1008 0b1111110000u, 9, 31, (2, 2, 2, 2, 3, 3, 7)
#define PPUTLUTRAITS_1007 0b1111101111u, 9, 31, (19, 53)
#define PPUTLUTRAITS_1006 0b1111101110u, 9, 31, (2, 503)
#define PPUTLUTRAITS_1005 0b1111101101u, 9, 31, (3, 5, 67)
#define PPUTLUTRAITS_1004 0b1111101100u, 9, 31, (2, 2, 251)
#define PPUTLUTRAITS_1003 0b1111101011u, 9, 31, (17, 59)
#define PPUTLUTRAITS_1002 0b1111101010u, 9, 31, (2, 3, 167)
#define PPUTLUTRAITS_1001 0b1111101001u, 9, 31, (7, 11, 13)
#define PPUTLUTRAITS_1000 0b1111101000u, 9, 31, (2, 2, 2, 5, 5, 5)
#define PPUTLUTRAITS_999  0b1111100111u, 9, 31, (3, 3, 3, 37)
#define PPUTLUTRAITS_998  0b1111100110u, 9, 31, (2, 499)
#define PPUTLUTRAITS_997  0b1111100101u, 9, 31, ()
#define PPUTLUTRAITS_996  0b1111100100u, 9, 31, (2, 2, 3, 83)
#define PPUTLUTRAITS_995  0b1111100011u, 9, 31, (5, 199)
#define PPUTLUTRAITS_994  0b1111100010u, 9, 31, (2, 7, 71)
#define PPUTLUTRAITS_993  0b1111100001u, 9, 31, (3, 331)
#define PPUTLUTRAITS_992  0b1111100000u, 9, 31, (2, 2, 2, 2, 2, 31)
#define PPUTLUTRAITS_991  0b1111011111u, 9, 31, ()
#define PPUTLUTRAITS_990  0b1111011110u, 9, 31, (2, 3, 3, 5, 11)
#define PPUTLUTRAITS_989  0b1111011101u, 9, 31, (23, 43)
#define PPUTLUTRAITS_988  0b1111011100u, 9, 31, (2, 2, 13, 19)
#define PPUTLUTRAITS_987  0b1111011011u, 9, 31, (3, 7, 47)
#define PPUTLUTRAITS_986  0b1111011010u, 9, 31, (2, 17, 29)
#define PPUTLUTRAITS_985  0b1111011001u, 9, 31, (5, 197)
#define PPUTLUTRAITS_984  0b1111011000u, 9, 31, (2, 2, 2, 3, 41)
#define PPUTLUTRAITS_983  0b1111010111u, 9, 31, ()
#define PPUTLUTRAITS_982  0b1111010110u, 9, 31, (2, 491)
#define PPUTLUTRAITS_981  0b1111010101u, 9, 31, (3, 3, 109)
#define PPUTLUTRAITS_980  0b1111010100u, 9, 31, (2, 2, 5, 7, 7)
#define PPUTLUTRAITS_979  0b1111010011u, 9, 31, (11, 89)
#define PPUTLUTRAITS_978  0b1111010010u, 9, 31, (2, 3, 163)
#define PPUTLUTRAITS_977  0b1111010001u, 9, 31, ()
#define PPUTLUTRAITS_976  0b1111010000u, 9, 31, (2, 2, 2, 2, 61)
#define PPUTLUTRAITS_975  0b1111001111u, 9, 31, (3, 5, 5, 13)
#define PPUTLUTRAITS_974  0b1111001110u, 9, 31, (2, 487)
#define PPUTLUTRAITS_973  0b1111001101u, 9, 31, (7, 139)
#define PPUTLUTRAITS_972  0b1111001100u, 9, 31, (2, 2, 3, 3, 3, 3, 3)
#define PPUTLUTRAITS_971  0b1111001011u, 9, 31, ()
#define PPUTLUTRAITS_970  0b1111001010u, 9, 31, (2, 5, 97)
#define PPUTLUTRAITS_969  0b1111001001u, 9, 31, (3, 17, 19)
#define PPUTLUTRAITS_968  0b1111001000u, 9, 31, (2, 2, 2, 11, 11)
#define PPUTLUTRAITS_967  0b1111000111u, 9, 31, ()
#define PPUTLUTRAITS_966  0b1111000110u, 9, 31, (2, 3, 7, 23)
#define PPUTLUTRAITS_965  0b1111000101u, 9, 31, (5, 193)
#define PPUTLUTRAITS_964  0b1111000100u, 9, 31, (2, 2, 241)
#define PPUTLUTRAITS_963  0b1111000011u, 9, 31, (3, 3, 107)
#define PPUTLUTRAITS_962  0b1111000010u, 9, 31, (2, 13, 37)
#define PPUTLUTRAITS_961  0b1111000001u, 9, 31, (31, 31)
#define PPUTLUTRAITS_960  0b1111000000u, 9, 30, (2, 2, 2, 2, 2, 2, 3, 5)
#define PPUTLUTRAITS_959  0b1110111111u, 9, 30, (7, 137)
#define PPUTLUTRAITS_958  0b1110111110u, 9, 30, (2, 479)
#define PPUTLUTRAITS_957  0b1110111101u, 9, 30, (3, 11, 29)
#define PPUTLUTRAITS_956  0b1110111100u, 9, 30, (2, 2, 239)
#define PPUTLUTRAITS_955  0b1110111011u, 9, 30, (5, 191)
#define PPUTLUTRAITS_954  0b1110111010u, 9, 30, (2, 3, 3, 53)
#define PPUTLUTRAITS_953  0b1110111001u, 9, 30, ()
#define PPUTLUTRAITS_952  0b1110111000u, 9, 30, (2, 2, 2, 7, 17)
#define PPUTLUTRAITS_951  0b1110110111u, 9, 30, (3, 317)
#define PPUTLUTRAITS_950  0b1110110110u, 9, 30, (2, 5, 5, 19)
#define PPUTLUTRAITS_949  0b1110110101u, 9, 30, (13, 73)
#define PPUTLUTRAITS_948  0b1110110100u, 9, 30, (2, 2, 3, 79)
#define PPUTLUTRAITS_947  0b1110110011u, 9, 30, ()
#define PPUTLUTRAITS_946  0b1110110010u, 9, 30, (2, 11, 43)
#define PPUTLUTRAITS_945  0b1110110001u, 9, 30, (3, 3, 3, 5, 7)
#define PPUTLUTRAITS_944  0b1110110000u, 9, 30, (2, 2, 2, 2, 59)
#define PPUTLUTRAITS_943  0b1110101111u, 9, 30, (23, 41)
#define PPUTLUTRAITS_942  0b1110101110u, 9, 30, (2, 3, 157)
#define PPUTLUTRAITS_941  0b1110101101u, 9, 30, ()
#define PPUTLUTRAITS_940  0b1110101100u, 9, 30, (2, 2, 5, 47)
#define PPUTLUTRAITS_939  0b1110101011u, 9, 30, (3, 313)
#define PPUTLUTRAITS_938  0b1110101010u, 9, 30, (2, 7, 67)
#define PPUTLUTRAITS_937  0b1110101001u, 9, 30, ()
#define PPUTLUTRAITS_936  0b1110101000u, 9, 30, (2, 2, 2, 3, 3, 13)
#define PPUTLUTRAITS_935  0b1110100111u, 9, 30, (5, 11, 17)
#define PPUTLUTRAITS_934  0b1110100110u, 9, 30, (2, 467)
#define PPUTLUTRAITS_933  0b1110100101u, 9, 30, (3, 311)
#define PPUTLUTRAITS_932  0b1110100100u, 9, 30, (2, 2, 233)
#define PPUTLUTRAITS_931  0b1110100011u, 9, 30, (7, 7, 19)
#define PPUTLUTRAITS_930  0b1110100010u, 9, 30, (2, 3, 5, 31)
#define PPUTLUTRAITS_929  0b1110100001u, 9, 30, ()
#define PPUTLUTRAITS_928  0b1110100000u, 9, 30, (2, 2, 2, 2, 2, 29)
#define PPUTLUTRAITS_927  0b1110011111u, 9, 30, (3, 3, 103)
#define PPUTLUTRAITS_926  0b1110011110u, 9, 30, (2, 463)
#define PPUTLUTRAITS_925  0b1110011101u, 9, 30, (5, 5, 37)
#define PPUTLUTRAITS_924  0b1110011100u, 9, 30, (2, 2, 3, 7, 11)
#define PPUTLUTRAITS_923  0b1110011011u, 9, 30, (13, 71)
#define PPUTLUTRAITS_922  0b1110011010u, 9, 30, (2, 461)
#define PPUTLUTRAITS_921  0b1110011001u, 9, 30, (3, 307)
#define PPUTLUTRAITS_920  0b1110011000u, 9, 30, (2, 2, 2, 5, 23)
#define PPUTLUTRAITS_919  0b1110010111u, 9, 30, ()
#define PPUTLUTRAITS_918  0b1110010110u, 9, 30, (2, 3, 3, 3, 17)
#define PPUTLUTRAITS_917  0b1110010101u, 9, 30, (7, 131)
#define PPUTLUTRAITS_916  0b1110010100u, 9, 30, (2, 2, 229)
#define PPUTLUTRAITS_915  0b1110010011u, 9, 30, (3, 5, 61)
#define PPUTLUTRAITS_914  0b1110010010u, 9, 30, (2, 457)
#define PPUTLUTRAITS_913  0b1110010001u, 9, 30, (11, 83)
#define PPUTLUTRAITS_912  0b1110010000u, 9, 30, (2, 2, 2, 2, 3, 19)
#define PPUTLUTRAITS_911  0b1110001111u, 9, 30, ()
#define PPUTLUTRAITS_910  0b1110001110u, 9, 30, (2, 5, 7, 13)
#define PPUTLUTRAITS_909  0b1110001101u, 9, 30, (3, 3, 101)
#define PPUTLUTRAITS_908  0b1110001100u, 9, 30, (2, 2, 227)
#define PPUTLUTRAITS_907  0b1110001011u, 9, 30, ()
#define PPUTLUTRAITS_906  0b1110001010u, 9, 30, (2, 3, 151)
#define PPUTLUTRAITS_905  0b1110001001u, 9, 30, (5, 181)
#define PPUTLUTRAITS_904  0b1110001000u, 9, 30, (2, 2, 2, 113)
#define PPUTLUTRAITS_903  0b1110000111u, 9, 30, (3, 7, 43)
#define PPUTLUTRAITS_902  0b1110000110u, 9, 30, (2, 11, 41)
#define PPUTLUTRAITS_901  0b1110000101u, 9, 30, (17, 53)
#define PPUTLUTRAITS_900  0b1110000100u, 9, 30, (2, 2, 3, 3, 5, 5)
#define PPUTLUTRAITS_899  0b1110000011u, 9, 29, (29, 31)
#define PPUTLUTRAITS_898  0b1110000010u, 9, 29, (2, 449)
#define PPUTLUTRAITS_897  0b1110000001u, 9, 29, (3, 13, 23)
#define PPUTLUTRAITS_896  0b1110000000u, 9, 29, (2, 2, 2, 2, 2, 2, 2, 7)
#define PPUTLUTRAITS_895  0b1101111111u, 9, 29, (5, 179)
#define PPUTLUTRAITS_894  0b1101111110u, 9, 29, (2, 3, 149)
#define PPUTLUTRAITS_893  0b1101111101u, 9, 29, (19, 47)
#define PPUTLUTRAITS_892  0b1101111100u, 9, 29, (2, 2, 223)
#define PPUTLUTRAITS_891  0b1101111011u, 9, 29, (3, 3, 3, 3, 11)
#define PPUTLUTRAITS_890  0b1101111010u, 9, 29, (2, 5, 89)
#define PPUTLUTRAITS_889  0b1101111001u, 9, 29, (7, 127)
#define PPUTLUTRAITS_888  0b1101111000u, 9, 29, (2, 2, 2, 3, 37)
#define PPUTLUTRAITS_887  0b1101110111u, 9, 29, ()
#define PPUTLUTRAITS_886  0b1101110110u, 9, 29, (2, 443)
#define PPUTLUTRAITS_885  0b1101110101u, 9, 29, (3, 5, 59)
#define PPUTLUTRAITS_884  0b1101110100u, 9, 29, (2, 2, 13, 17)
#define PPUTLUTRAITS_883  0b1101110011u, 9, 29, ()
#define PPUTLUTRAITS_882  0b1101110010u, 9, 29, (2, 3, 3, 7, 7)
#define PPUTLUTRAITS_881  0b1101110001u, 9, 29, ()
#define PPUTLUTRAITS_880  0b1101110000u, 9, 29, (2, 2, 2, 2, 5, 11)
#define PPUTLUTRAITS_879  0b1101101111u, 9, 29, (3, 293)
#define PPUTLUTRAITS_878  0b1101101110u, 9, 29, (2, 439)
#define PPUTLUTRAITS_877  0b1101101101u, 9, 29, ()
#define PPUTLUTRAITS_876  0b1101101100u, 9, 29, (2, 2, 3, 73)
#define PPUTLUTRAITS_875  0b1101101011u, 9, 29, (5, 5, 5, 7)
#define PPUTLUTRAITS_874  0b1101101010u, 9, 29, (2, 19, 23)
#define PPUTLUTRAITS_873  0b1101101001u, 9, 29, (3, 3, 97)
#define PPUTLUTRAITS_872  0b1101101000u, 9, 29, (2, 2, 2, 109)
#define PPUTLUTRAITS_871  0b1101100111u, 9, 29, (13, 67)
#define PPUTLUTRAITS_870  0b1101100110u, 9, 29, (2, 3, 5, 29)
#define PPUTLUTRAITS_869  0b1101100101u, 9, 29, (11, 79)
#define PPUTLUTRAITS_868  0b1101100100u, 9, 29, (2, 2, 7, 31)
#define PPUTLUTRAITS_867  0b1101100011u, 9, 29, (3, 17, 17)
#define PPUTLUTRAITS_866  0b1101100010u, 9, 29, (2, 433)
#define PPUTLUTRAITS_865  0b1101100001u, 9, 29, (5, 173)
#define PPUTLUTRAITS_864  0b1101100000u, 9, 29, (2, 2, 2, 2, 2, 3, 3, 3)
#define PPUTLUTRAITS_863  0b1101011111u, 9, 29, ()
#define PPUTLUTRAITS_862  0b1101011110u, 9, 29, (2, 431)
#define PPUTLUTRAITS_861  0b1101011101u, 9, 29, (3, 7, 41)
#define PPUTLUTRAITS_860  0b1101011100u, 9, 29, (2, 2, 5, 43)
#define PPUTLUTRAITS_859  0b1101011011u, 9, 29, ()
#define PPUTLUTRAITS_858  0b1101011010u, 9, 29, (2, 3, 11, 13)
#define PPUTLUTRAITS_857  0b1101011001u, 9, 29, ()
#define PPUTLUTRAITS_856  0b1101011000u, 9, 29, (2, 2, 2, 107)
#define PPUTLUTRAITS_855  0b1101010111u, 9, 29, (3, 3, 5, 19)
#define PPUTLUTRAITS_854  0b1101010110u, 9, 29, (2, 7, 61)
#define PPUTLUTRAITS_853  0b1101010101u, 9, 29, ()
#define PPUTLUTRAITS_852  0b1101010100u, 9, 29, (2, 2, 3, 71)
#define PPUTLUTRAITS_851  0b1101010011u, 9, 29, (23, 37)
#define PPUTLUTRAITS_850  0b1101010010u, 9, 29, (2, 5, 5, 17)
#define PPUTLUTRAITS_849  0b1101010001u, 9, 29, (3, 283)
#define PPUTLUTRAITS_848  0b1101010000u, 9, 29, (2, 2, 2, 2, 53)
#define PPUTLUTRAITS_847  0b1101001111u, 9, 29, (7, 11, 11)
#define PPUTLUTRAITS_846  0b1101001110u, 9, 29, (2, 3, 3, 47)
#define PPUTLUTRAITS_845  0b1101001101u, 9, 29, (5, 13, 13)
#define PPUTLUTRAITS_844  0b1101001100u, 9, 29, (2, 2, 211)
#define PPUTLUTRAITS_843  0b1101001011u, 9, 29, (3, 281)
#define PPUTLUTRAITS_842  0b1101001010u, 9, 29, (2, 421)
#define PPUTLUTRAITS_841  0b1101001001u, 9, 29, (29, 29)
#define PPUTLUTRAITS_840  0b1101001000u, 9, 28, (2, 2, 2, 3, 5, 7)
#define PPUTLUTRAITS_839  0b1101000111u, 9, 28, ()
#define PPUTLUTRAITS_838  0b1101000110u, 9, 28, (2, 419)
#define PPUTLUTRAITS_837  0b1101000101u, 9, 28, (3, 3, 3, 31)
#define PPUTLUTRAITS_836  0b1101000100u, 9, 28, (2, 2, 11, 19)
#define PPUTLUTRAITS_835  0b1101000011u, 9, 28, (5, 167)
#define PPUTLUTRAITS_834  0b1101000010u, 9, 28, (2, 3, 139)
#define PPUTLUTRAITS_833  0b1101000001u, 9, 28, (7, 7, 17)
#define PPUTLUTRAITS_832  0b1101000000u, 9, 28, (2, 2, 2, 2, 2, 2, 13)
#define PPUTLUTRAITS_831  0b1100111111u, 9, 28, (3, 277)
#define PPUTLUTRAITS_830  0b1100111110u, 9, 28, (2, 5, 83)
#define PPUTLUTRAITS_829  0b1100111101u, 9, 28, ()
#define PPUTLUTRAITS_828  0b1100111100u, 9, 28, (2, 2, 3, 3, 23)
#define PPUTLUTRAITS_827  0b1100111011u, 9, 28, ()
#define PPUTLUTRAITS_826  0b1100111010u, 9, 28, (2, 7, 59)
#define PPUTLUTRAITS_825  0b1100111001u, 9, 28, (3, 5, 5, 11)
#define PPUTLUTRAITS_824  0b1100111000u, 9, 28, (2, 2, 2, 103)
#define PPUTLUTRAITS_823  0b1100110111u, 9, 28, ()
#define PPUTLUTRAITS_822  0b1100110110u, 9, 28, (2, 3, 137)
#define PPUTLUTRAITS_821  0b1100110101u, 9, 28, ()
#define PPUTLUTRAITS_820  0b1100110100u, 9, 28, (2, 2, 5, 41)
#define PPUTLUTRAITS_819  0b1100110011u, 9, 28, (3, 3, 7, 13)
#define PPUTLUTRAITS_818  0b1100110010u, 9, 28, (2, 409)
#define PPUTLUTRAITS_817  0b1100110001u, 9, 28, (19, 43)
#define PPUTLUTRAITS_816  0b1100110000u, 9, 28, (2, 2, 2, 2, 3, 17)
#define PPUTLUTRAITS_815  0b1100101111u, 9, 28, (5, 163)
#define PPUTLUTRAITS_814  0b1100101110u, 9, 28, (2, 11, 37)
#define PPUTLUTRAITS_813  0b1100101101u, 9, 28, (3, 271)
#define PPUTLUTRAITS_812  0b1100101100u, 9, 28, (2, 2, 7, 29)
#define PPUTLUTRAITS_811  0b1100101011u, 9, 28, ()
#define PPUTLUTRAITS_810  0b1100101010u, 9, 28, (2, 3, 3, 3, 3, 5)
#define PPUTLUTRAITS_809  0b1100101001u, 9, 28, ()
#define PPUTLUTRAITS_808  0b1100101000u, 9, 28, (2, 2, 2, 101)
#define PPUTLUTRAITS_807  0b1100100111u, 9, 28, (3, 269)
#define PPUTLUTRAITS_806  0b1100100110u, 9, 28, (2, 13, 31)
#define PPUTLUTRAITS_805  0b1100100101u, 9, 28, (5, 7, 23)
#define PPUTLUTRAITS_804  0b1100100100u, 9, 28, (2, 2, 3, 67)
#define PPUTLUTRAITS_803  0b1100100011u, 9, 28, (11, 73)
#define PPUTLUTRAITS_802  0b1100100010u, 9, 28, (2, 401)
#define PPUTLUTRAITS_801  0b1100100001u, 9, 28, (3, 3, 89)
#define PPUTLUTRAITS_800  0b1100100000u, 9, 28, (2, 2, 2, 2, 2, 5, 5)
#define PPUTLUTRAITS_799  0b1100011111u, 9, 28, (17, 47)
#define PPUTLUTRAITS_798  0b1100011110u, 9, 28, (2, 3, 7, 19)
#define PPUTLUTRAITS_797  0b1100011101u, 9, 28, ()
#define PPUTLUTRAITS_796  0b1100011100u, 9, 28, (2, 2, 199)
#define PPUTLUTRAITS_795  0b1100011011u, 9, 28, (3, 5, 53)
#define PPUTLUTRAITS_794  0b1100011010u, 9, 28, (2, 397)
#define PPUTLUTRAITS_793  0b1100011001u, 9, 28, (13, 61)
#define PPUTLUTRAITS_792  0b1100011000u, 9, 28, (2, 2, 2, 3, 3, 11)
#define PPUTLUTRAITS_791  0b1100010111u, 9, 28, (7, 113)
#define PPUTLUTRAITS_790  0b1100010110u, 9, 28, (2, 5, 79)
#define PPUTLUTRAITS_789  0b1100010101u, 9, 28, (3, 263)
#define PPUTLUTRAITS_788  0b1100010100u, 9, 28, (2, 2, 197)
#define PPUTLUTRAITS_787  0b1100010011u, 9, 28, ()
#define PPUTLUTRAITS_786  0b1100010010u, 9, 28, (2, 3, 131)
#define PPUTLUTRAITS_785  0b1100010001u, 9, 28, (5, 157)
#define PPUTLUTRAITS_784  0b1100010000u, 9, 28, (2, 2, 2, 2, 7, 7)
#define PPUTLUTRAITS_783  0b1100001111u, 9, 27, (3, 3, 3, 29)
#define PPUTLUTRAITS_782  0b1100001110u, 9, 27, (2, 17, 23)
#define PPUTLUTRAITS_781  0b1100001101u, 9, 27, (11, 71)
#define PPUTLUTRAITS_780  0b1100001100u, 9, 27, (2, 2, 3, 5, 13)
#define PPUTLUTRAITS_779  0b1100001011u, 9, 27, (19, 41)
#define PPUTLUTRAITS_778  0b1100001010u, 9, 27, (2, 389)
#define PPUTLUTRAITS_777  0b1100001001u, 9, 27, (3, 7, 37)
#define PPUTLUTRAITS_776  0b1100001000u, 9, 27, (2, 2, 2, 97)
#define PPUTLUTRAITS_775  0b1100000111u, 9, 27, (5, 5, 31)
#define PPUTLUTRAITS_774  0b1100000110u, 9, 27, (2, 3, 3, 43)
#define PPUTLUTRAITS_773  0b1100000101u, 9, 27, ()
#define PPUTLUTRAITS_772  0b1100000100u, 9, 27, (2, 2, 193)
#define PPUTLUTRAITS_771  0b1100000011u, 9, 27, (3, 257)
#define PPUTLUTRAITS_770  0b1100000010u, 9, 27, (2, 5, 7, 11)
#define PPUTLUTRAITS_769  0b1100000001u, 9, 27, ()
#define PPUTLUTRAITS_768  0b1100000000u, 9, 27, (2, 2, 2, 2, 2, 2, 2, 2, 3)
#define PPUTLUTRAITS_767  0b1011111111u, 9, 27, (13, 59)
#define PPUTLUTRAITS_766  0b1011111110u, 9, 27, (2, 383)
#define PPUTLUTRAITS_765  0b1011111101u, 9, 27, (3, 3, 5, 17)
#define PPUTLUTRAITS_764  0b1011111100u, 9, 27, (2, 2, 191)
#define PPUTLUTRAITS_763  0b1011111011u, 9, 27, (7, 109)
#define PPUTLUTRAITS_762  0b1011111010u, 9, 27, (2, 3, 127)
#define PPUTLUTRAITS_761  0b1011111001u, 9, 27, ()
#define PPUTLUTRAITS_760  0b1011111000u, 9, 27, (2, 2, 2, 5, 19)
#define PPUTLUTRAITS_759  0b1011110111u, 9, 27, (3, 11, 23)
#define PPUTLUTRAITS_758  0b1011110110u, 9, 27, (2, 379)
#define PPUTLUTRAITS_757  0b1011110101u, 9, 27, ()
#define PPUTLUTRAITS_756  0b1011110100u, 9, 27, (2, 2, 3, 3, 3, 7)
#define PPUTLUTRAITS_755  0b1011110011u, 9, 27, (5, 151)
#define PPUTLUTRAITS_754  0b1011110010u, 9, 27, (2, 13, 29)
#define PPUTLUTRAITS_753  0b1011110001u, 9, 27, (3, 251)
#define PPUTLUTRAITS_752  0b1011110000u, 9, 27, (2, 2, 2, 2, 47)
#define PPUTLUTRAITS_751  0b1011101111u, 9, 27, ()
#define PPUTLUTRAITS_750  0b1011101110u, 9, 27, (2, 3, 5, 5, 5)
#define PPUTLUTRAITS_749  0b1011101101u, 9, 27, (7, 107)
#define PPUTLUTRAITS_748  0b1011101100u, 9, 27, (2, 2, 11, 17)
#define PPUTLUTRAITS_747  0b1011101011u, 9, 27, (3, 3, 83)
#define PPUTLUTRAITS_746  0b1011101010u, 9, 27, (2, 373)
#define PPUTLUTRAITS_745  0b1011101001u, 9, 27, (5, 149)
#define PPUTLUTRAITS_744  0b1011101000u, 9, 27, (2, 2, 2, 3, 31)
#define PPUTLUTRAITS_743  0b1011100111u, 9, 27, ()
#define PPUTLUTRAITS_742  0b1011100110u, 9, 27, (2, 7, 53)
#define PPUTLUTRAITS_741  0b1011100101u, 9, 27, (3, 13, 19)
#define PPUTLUTRAITS_740  0b1011100100u, 9, 27, (2, 2, 5, 37)
#define PPUTLUTRAITS_739  0b1011100011u, 9, 27, ()
#define PPUTLUTRAITS_738  0b1011100010u, 9, 27, (2, 3, 3, 41)
#define PPUTLUTRAITS_737  0b1011100001u, 9, 27, (11, 67)
#define PPUTLUTRAITS_736  0b1011100000u, 9, 27, (2, 2, 2, 2, 2, 23)
#define PPUTLUTRAITS_735  0b1011011111u, 9, 27, (3, 5, 7, 7)
#define PPUTLUTRAITS_734  0b1011011110u, 9, 27, (2, 367)
#define PPUTLUTRAITS_733  0b1011011101u, 9, 27, ()
#define PPUTLUTRAITS_732  0b1011011100u, 9, 27, (2, 2, 3, 61)
#define PPUTLUTRAITS_731  0b1011011011u, 9, 27, (17, 43)
#define PPUTLUTRAITS_730  0b1011011010u, 9, 27, (2, 5, 73)
#define PPUTLUTRAITS_729  0b1011011001u, 9, 27, (3, 3, 3, 3, 3, 3)
#define PPUTLUTRAITS_728  0b1011011000u, 9, 26, (2, 2, 2, 7, 13)
#define PPUTLUTRAITS_727  0b1011010111u, 9, 26, ()
#define PPUTLUTRAITS_726  0b1011010110u, 9, 26, (2, 3, 11, 11)
#define PPUTLUTRAITS_725  0b1011010101u, 9, 26, (5, 5, 29)
#define PPUTLUTRAITS_724  0b1011010100u, 9, 26, (2, 2, 181)
#define PPUTLUTRAITS_723  0b1011010011u, 9, 26, (3, 241)
#define PPUTLUTRAITS_722  0b1011010010u, 9, 26, (2, 19, 19)
#define PPUTLUTRAITS_721  0b1011010001u, 9, 26, (7, 103)
#define PPUTLUTRAITS_720  0b1011010000u, 9, 26, (2, 2, 2, 2, 3, 3, 5)
#define PPUTLUTRAITS_719  0b1011001111u, 9, 26, ()
#define PPUTLUTRAITS_718  0b1011001110u, 9, 26, (2, 359)
#define PPUTLUTRAITS_717  0b1011001101u, 9, 26, (3, 239)
#define PPUTLUTRAITS_716  0b1011001100u, 9, 26, (2, 2, 179)
#define PPUTLUTRAITS_715  0b1011001011u, 9, 26, (5, 11, 13)
#define PPUTLUTRAITS_714  0b1011001010u, 9, 26, (2, 3, 7, 17)
#define PPUTLUTRAITS_713  0b1011001001u, 9, 26, (23, 31)
#define PPUTLUTRAITS_712  0b1011001000u, 9, 26, (2, 2, 2, 89)
#define PPUTLUTRAITS_711  0b1011000111u, 9, 26, (3, 3, 79)
#define PPUTLUTRAITS_710  0b1011000110u, 9, 26, (2, 5, 71)
#define PPUTLUTRAITS_709  0b1011000101u, 9, 26, ()
#define PPUTLUTRAITS_708  0b1011000100u, 9, 26, (2, 2, 3, 59)
#define PPUTLUTRAITS_707  0b1011000011u, 9, 26, (7, 101)
#define PPUTLUTRAITS_706  0b1011000010u, 9, 26, (2, 353)
#define PPUTLUTRAITS_705  0b1011000001u, 9, 26, (3, 5, 47)
#define PPUTLUTRAITS_704  0b1011000000u, 9, 26, (2, 2, 2, 2, 2, 2, 11)
#define PPUTLUTRAITS_703  0b1010111111u, 9, 26, (19, 37)
#define PPUTLUTRAITS_702  0b1010111110u, 9, 26, (2, 3, 3, 3, 13)
#define PPUTLUTRAITS_701  0b1010111101u, 9, 26, ()
#define PPUTLUTRAITS_700  0b1010111100u, 9, 26, (2, 2, 5, 5, 7)
#define PPUTLUTRAITS_699  0b1010111011u, 9, 26, (3, 233)
#define PPUTLUTRAITS_698  0b1010111010u, 9, 26, (2, 349)
#define PPUTLUTRAITS_697  0b1010111001u, 9, 26, (17, 41)
#define PPUTLUTRAITS_696  0b1010111000u, 9, 26, (2, 2, 2, 3, 29)
#define PPUTLUTRAITS_695  0b1010110111u, 9, 26, (5, 139)
#define PPUTLUTRAITS_694  0b1010110110u, 9, 26, (2, 347)
#define PPUTLUTRAITS_693  0b1010110101u, 9, 26, (3, 3, 7, 11)
#define PPUTLUTRAITS_692  0b1010110100u, 9, 26, (2, 2, 173)
#define PPUTLUTRAITS_691  0b1010110011u, 9, 26, ()
#define PPUTLUTRAITS_690  0b1010110010u, 9, 26, (2, 3, 5, 23)
#define PPUTLUTRAITS_689  0b1010110001u, 9, 26, (13, 53)
#define PPUTLUTRAITS_688  0b1010110000u, 9, 26, (2, 2, 2, 2, 43)
#define PPUTLUTRAITS_687  0b1010101111u, 9, 26, (3, 229)
#define PPUTLUTRAITS_686  0b1010101110u, 9, 26, (2, 7, 7, 7)
#define PPUTLUTRAITS_685  0b1010101101u, 9, 26, (5, 137)
#define PPUTLUTRAITS_684  0b1010101100u, 9, 26, (2, 2, 3, 3, 19)
#define PPUTLUTRAITS_683  0b1010101011u, 9, 26, ()
#define PPUTLUTRAITS_682  0b1010101010u, 9, 26, (2, 11, 31)
#define PPUTLUTRAITS_681  0b1010101001u, 9, 26, (3, 227)
#define PPUTLUTRAITS_680  0b1010101000u, 9, 26, (2, 2, 2, 5, 17)
#define PPUTLUTRAITS_679  0b1010100111u, 9, 26, (7, 97)
#define PPUTLUTRAITS_678  0b1010100110u, 9, 26, (2, 3, 113)
#define PPUTLUTRAITS_677  0b1010100101u, 9, 26, ()
#define PPUTLUTRAITS_676  0b1010100100u, 9, 26, (2, 2, 13, 13)
#define PPUTLUTRAITS_675  0b1010100011u, 9, 25, (3, 3, 3, 5, 5)
#define PPUTLUTRAITS_674  0b1010100010u, 9, 25, (2, 337)
#define PPUTLUTRAITS_673  0b1010100001u, 9, 25, ()
#define PPUTLUTRAITS_672  0b1010100000u, 9, 25, (2, 2, 2, 2, 2, 3, 7)
#define PPUTLUTRAITS_671  0b1010011111u, 9, 25, (11, 61)
#define PPUTLUTRAITS_670  0b1010011110u, 9, 25, (2, 5, 67)
#define PPUTLUTRAITS_669  0b1010011101u, 9, 25, (3, 223)
#define PPUTLUTRAITS_668  0b1010011100u, 9, 25, (2, 2, 167)
#define PPUTLUTRAITS_667  0b1010011011u, 9, 25, (23, 29)
#define PPUTLUTRAITS_666  0b1010011010u, 9, 25, (2, 3, 3, 37)
#define PPUTLUTRAITS_665  0b1010011001u, 9, 25, (5, 7, 19)
#define PPUTLUTRAITS_664  0b1010011000u, 9, 25, (2, 2, 2, 83)
#define PPUTLUTRAITS_663  0b1010010111u, 9, 25, (3, 13, 17)
#define PPUTLUTRAITS_662  0b1010010110u, 9, 25, (2, 331)
#define PPUTLUTRAITS_661  0b1010010101u, 9, 25, ()
#define PPUTLUTRAITS_660  0b1010010100u, 9, 25, (2, 2, 3, 5, 11)
#define PPUTLUTRAITS_659  0b1010010011u, 9, 25, ()
#define PPUTLUTRAITS_658  0b1010010010u, 9, 25, (2, 7, 47)
#define PPUTLUTRAITS_657  0b1010010001u, 9, 25, (3, 3, 73)
#define PPUTLUTRAITS_656  0b1010010000u, 9, 25, (2, 2, 2, 2, 41)
#define PPUTLUTRAITS_655  0b1010001111u, 9, 25, (5, 131)
#define PPUTLUTRAITS_654  0b1010001110u, 9, 25, (2, 3, 109)
#define PPUTLUTRAITS_653  0b1010001101u, 9, 25, ()
#define PPUTLUTRAITS_652  0b1010001100u, 9, 25, (2, 2, 163)
#define PPUTLUTRAITS_651  0b1010001011u, 9, 25, (3, 7, 31)
#define PPUTLUTRAITS_650  0b1010001010u, 9, 25, (2, 5, 5, 13)
#define PPUTLUTRAITS_649  0b1010001001u, 9, 25, (11, 59)
#define PPUTLUTRAITS_648  0b1010001000u, 9, 25, (2, 2, 2, 3, 3, 3, 3)
#define PPUTLUTRAITS_647  0b1010000111u, 9, 25, ()
#define PPUTLUTRAITS_646  0b1010000110u, 9, 25, (2, 17, 19)
#define PPUTLUTRAITS_645  0b1010000101u, 9, 25, (3, 5, 43)
#define PPUTLUTRAITS_644  0b1010000100u, 9, 25, (2, 2, 7, 23)
#define PPUTLUTRAITS_643  0b1010000011u, 9, 25, ()
#define PPUTLUTRAITS_642  0b1010000010u, 9, 25, (2, 3, 107)
#define PPUTLUTRAITS_641  0b1010000001u, 9, 25, ()
#define PPUTLUTRAITS_640  0b1010000000u, 9, 25, (2, 2, 2, 2, 2, 2, 2, 5)
#define PPUTLUTRAITS_639  0b1001111111u, 9, 25, (3, 3, 71)
#define PPUTLUTRAITS_638  0b1001111110u, 9, 25, (2, 11, 29)
#define PPUTLUTRAITS_637  0b1001111101u, 9, 25, (7, 7, 13)
#define PPUTLUTRAITS_636  0b1001111100u, 9, 25, (2, 2, 3, 53)
#define PPUTLUTRAITS_635  0b1001111011u, 9, 25, (5, 127)
#define PPUTLUTRAITS_634  0b1001111010u, 9, 25, (2, 317)
#define PPUTLUTRAITS_633  0b1001111001u, 9, 25, (3, 211)
#define PPUTLUTRAITS_632  0b1001111000u, 9, 25, (2, 2, 2, 79)
#define PPUTLUTRAITS_631  0b1001110111u, 9, 25, ()
#define PPUTLUTRAITS_630  0b1001110110u, 9, 25, (2, 3, 3, 5, 7)
#define PPUTLUTRAITS_629  0b1001110101u, 9, 25, (17, 37)
#define PPUTLUTRAITS_628  0b1001110100u, 9, 25, (2, 2, 157)
#define PPUTLUTRAITS_627  0b1001110011u, 9, 25, (3, 11, 19)
#define PPUTLUTRAITS_626  0b1001110010u, 9, 25, (2, 313)
#define PPUTLUTRAITS_625  0b1001110001u, 9, 25, (5, 5, 5, 5)
#define PPUTLUTRAITS_624  0b1001110000u, 9, 24, (2, 2, 2, 2, 3, 13)
#define PPUTLUTRAITS_623  0b1001101111u, 9, 24, (7, 89)
#define PPUTLUTRAITS_622  0b1001101110u, 9, 24, (2, 311)
#define PPUTLUTRAITS_621  0b1001101101u, 9, 24, (3, 3, 3, 23)
#define PPUTLUTRAITS_620  0b1001101100u, 9, 24, (2, 2, 5, 31)
#define PPUTLUTRAITS_619  0b1001101011u, 9, 24, ()
#define PPUTLUTRAITS_618  0b1001101010u, 9, 24, (2, 3, 103)
#define PPUTLUTRAITS_617  0b1001101001u, 9, 24, ()
#define PPUTLUTRAITS_616  0b1001101000u, 9, 24, (2, 2, 2, 7, 11)
#define PPUTLUTRAITS_615  0b1001100111u, 9, 24, (3, 5, 41)
#define PPUTLUTRAITS_614  0b1001100110u, 9, 24, (2, 307)
#define PPUTLUTRAITS_613  0b1001100101u, 9, 24, ()
#define PPUTLUTRAITS_612  0b1001100100u, 9, 24, (2, 2, 3, 3, 17)
#define PPUTLUTRAITS_611  0b1001100011u, 9, 24, (13, 47)
#define PPUTLUTRAITS_610  0b1001100010u, 9, 24, (2, 5, 61)
#define PPUTLUTRAITS_609  0b1001100001u, 9, 24, (3, 7, 29)
#define PPUTLUTRAITS_608  0b1001100000u, 9, 24, (2, 2, 2, 2, 2, 19)
#define PPUTLUTRAITS_607  0b1001011111u, 9, 24, ()
#define PPUTLUTRAITS_606  0b1001011110u, 9, 24, (2, 3, 101)
#define PPUTLUTRAITS_605  0b1001011101u, 9, 24, (5, 11, 11)
#define PPUTLUTRAITS_604  0b1001011100u, 9, 24, (2, 2, 151)
#define PPUTLUTRAITS_603  0b1001011011u, 9, 24, (3, 3, 67)
#define PPUTLUTRAITS_602  0b1001011010u, 9, 24, (2, 7, 43)
#define PPUTLUTRAITS_601  0b1001011001u, 9, 24, ()
#define PPUTLUTRAITS_600  0b1001011000u, 9, 24, (2, 2, 2, 3, 5, 5)
#define PPUTLUTRAITS_599  0b1001010111u, 9, 24, ()
#define PPUTLUTRAITS_598  0b1001010110u, 9, 24, (2, 13, 23)
#define PPUTLUTRAITS_597  0b1001010101u, 9, 24, (3, 199)
#define PPUTLUTRAITS_596  0b1001010100u, 9, 24, (2, 2, 149)
#define PPUTLUTRAITS_595  0b1001010011u, 9, 24, (5, 7, 17)
#define PPUTLUTRAITS_594  0b1001010010u, 9, 24, (2, 3, 3, 3, 11)
#define PPUTLUTRAITS_593  0b1001010001u, 9, 24, ()
#define PPUTLUTRAITS_592  0b1001010000u, 9, 24, (2, 2, 2, 2, 37)
#define PPUTLUTRAITS_591  0b1001001111u, 9, 24, (3, 197)
#define PPUTLUTRAITS_590  0b1001001110u, 9, 24, (2, 5, 59)
#define PPUTLUTRAITS_589  0b1001001101u, 9, 24, (19, 31)
#define PPUTLUTRAITS_588  0b1001001100u, 9, 24, (2, 2, 3, 7, 7)
#define PPUTLUTRAITS_587  0b1001001011u, 9, 24, ()
#define PPUTLUTRAITS_586  0b1001001010u, 9, 24, (2, 293)
#define PPUTLUTRAITS_585  0b1001001001u, 9, 24, (3, 3, 5, 13)
#define PPUTLUTRAITS_584  0b1001001000u, 9, 24, (2, 2, 2, 73)
#define PPUTLUTRAITS_583  0b1001000111u, 9, 24, (11, 53)
#define PPUTLUTRAITS_582  0b1001000110u, 9, 24, (2, 3, 97)
#define PPUTLUTRAITS_581  0b1001000101u, 9, 24, (7, 83)
#define PPUTLUTRAITS_580  0b1001000100u, 9, 24, (2, 2, 5, 29)
#define PPUTLUTRAITS_579  0b1001000011u, 9, 24, (3, 193)
#define PPUTLUTRAITS_578  0b1001000010u, 9, 24, (2, 17, 17)
#define PPUTLUTRAITS_577  0b1001000001u, 9, 24, ()
#define PPUTLUTRAITS_576  0b1001000000u, 9, 24, (2, 2, 2, 2, 2, 2, 3, 3)
#define PPUTLUTRAITS_575  0b1000111111u, 9, 23, (5, 5, 23)
#define PPUTLUTRAITS_574  0b1000111110u, 9, 23, (2, 7, 41)
#define PPUTLUTRAITS_573  0b1000111101u, 9, 23, (3, 191)
#define PPUTLUTRAITS_572  0b1000111100u, 9, 23, (2, 2, 11, 13)
#define PPUTLUTRAITS_571  0b1000111011u, 9, 23, ()
#define PPUTLUTRAITS_570  0b1000111010u, 9, 23, (2, 3, 5, 19)
#define PPUTLUTRAITS_569  0b1000111001u, 9, 23, ()
#define PPUTLUTRAITS_568  0b1000111000u, 9, 23, (2, 2, 2, 71)
#define PPUTLUTRAITS_567  0b1000110111u, 9, 23, (3, 3, 3, 3, 7)
#define PPUTLUTRAITS_566  0b1000110110u, 9, 23, (2, 283)
#define PPUTLUTRAITS_565  0b1000110101u, 9, 23, (5, 113)
#define PPUTLUTRAITS_564  0b1000110100u, 9, 23, (2, 2, 3, 47)
#define PPUTLUTRAITS_563  0b1000110011u, 9, 23, ()
#define PPUTLUTRAITS_562  0b1000110010u, 9, 23, (2, 281)
#define PPUTLUTRAITS_561  0b1000110001u, 9, 23, (3, 11, 17)
#define PPUTLUTRAITS_560  0b1000110000u, 9, 23, (2, 2, 2, 2, 5, 7)
#define PPUTLUTRAITS_559  0b1000101111u, 9, 23, (13, 43)
#define PPUTLUTRAITS_558  0b1000101110u, 9, 23, (2, 3, 3, 31)
#define PPUTLUTRAITS_557  0b1000101101u, 9, 23, ()
#define PPUTLUTRAITS_556  0b1000101100u, 9, 23, (2, 2, 139)
#define PPUTLUTRAITS_555  0b1000101011u, 9, 23, (3, 5, 37)
#define PPUTLUTRAITS_554  0b1000101010u, 9, 23, (2, 277)
#define PPUTLUTRAITS_553  0b1000101001u, 9, 23, (7, 79)
#define PPUTLUTRAITS_552  0b1000101000u, 9, 23, (2, 2, 2, 3, 23)
#define PPUTLUTRAITS_551  0b1000100111u, 9, 23, (19, 29)
#define PPUTLUTRAITS_550  0b1000100110u, 9, 23, (2, 5, 5, 11)
#define PPUTLUTRAITS_549  0b1000100101u, 9, 23, (3, 3, 61)
#define PPUTLUTRAITS_548  0b1000100100u, 9, 23, (2, 2, 137)
#define PPUTLUTRAITS_547  0b1000100011u, 9, 23, ()
#define PPUTLUTRAITS_546  0b1000100010u, 9, 23, (2, 3, 7, 13)
#define PPUTLUTRAITS_545  0b1000100001u, 9, 23, (5, 109)
#define PPUTLUTRAITS_544  0b1000100000u, 9, 23, (2, 2, 2, 2, 2, 17)
#define PPUTLUTRAITS_543  0b1000011111u, 9, 23, (3, 181)
#define PPUTLUTRAITS_542  0b1000011110u, 9, 23, (2, 271)
#define PPUTLUTRAITS_541  0b1000011101u, 9, 23, ()
#define PPUTLUTRAITS_540  0b1000011100u, 9, 23, (2, 2, 3, 3, 3, 5)
#define PPUTLUTRAITS_539  0b1000011011u, 9, 23, (7, 7, 11)
#define PPUTLUTRAITS_538  0b1000011010u, 9, 23, (2, 269)
#define PPUTLUTRAITS_537  0b1000011001u, 9, 23, (3, 179)
#define PPUTLUTRAITS_536  0b1000011000u, 9, 23, (2, 2, 2, 67)
#define PPUTLUTRAITS_535  0b1000010111u, 9, 23, (5, 107)
#define PPUTLUTRAITS_534  0b1000010110u, 9, 23, (2, 3, 89)
#define PPUTLUTRAITS_533  0b1000010101u, 9, 23, (13, 41)
#define PPUTLUTRAITS_532  0b1000010100u, 9, 23, (2, 2, 7, 19)
#define PPUTLUTRAITS_531  0b1000010011u, 9, 23, (3, 3, 59)
#define PPUTLUTRAITS_530  0b1000010010u, 9, 23, (2, 5, 53)
#define PPUTLUTRAITS_529  0b1000010001u, 9, 23, (23, 23)
#define PPUTLUTRAITS_528  0b1000010000u, 9, 22, (2, 2, 2, 2, 3, 11)
#define PPUTLUTRAITS_527  0b1000001111u, 9, 22, (17, 31)
#define PPUTLUTRAITS_526  0b1000001110u, 9, 22, (2, 263)
#define PPUTLUTRAITS_525  0b1000001101u, 9, 22, (3, 5, 5, 7)
#define PPUTLUTRAITS_524  0b1000001100u, 9, 22, (2, 2, 131)
#define PPUTLUTRAITS_523  0b1000001011u, 9, 22, ()
#define PPUTLUTRAITS_522  0b1000001010u, 9, 22, (2, 3, 3, 29)
#define PPUTLUTRAITS_521  0b1000001001u, 9, 22, ()
#define PPUTLUTRAITS_520  0b1000001000u, 9, 22, (2, 2, 2, 5, 13)
#define PPUTLUTRAITS_519  0b1000000111u, 9, 22, (3, 173)
#define PPUTLUTRAITS_518  0b1000000110u, 9, 22, (2, 7, 37)
#define PPUTLUTRAITS_517  0b1000000101u, 9, 22, (11, 47)
#define PPUTLUTRAITS_516  0b1000000100u, 9, 22, (2, 2, 3, 43)
#define PPUTLUTRAITS_515  0b1000000011u, 9, 22, (5, 103)
#define PPUTLUTRAITS_514  0b1000000010u, 9, 22, (2, 257)
#define PPUTLUTRAITS_513  0b1000000001u, 9, 22, (3, 3, 3, 19)
#define PPUTLUTRAITS_512  0b1000000000u, 9, 22, (2, 2, 2, 2, 2, 2, 2, 2, 2)
#define PPUTLUTRAITS_511  0b0111111111u, 8, 22, (7, 73)
#define PPUTLUTRAITS_510  0b0111111110u, 8, 22, (2, 3, 5, 17)
#define PPUTLUTRAITS_509  0b0111111101u, 8, 22, ()
#define PPUTLUTRAITS_508  0b0111111100u, 8, 22, (2, 2, 127)
#define PPUTLUTRAITS_507  0b0111111011u, 8, 22, (3, 13, 13)
#define PPUTLUTRAITS_506  0b0111111010u, 8, 22, (2, 11, 23)
#define PPUTLUTRAITS_505  0b0111111001u, 8, 22, (5, 101)
#define PPUTLUTRAITS_504  0b0111111000u, 8, 22, (2, 2, 2, 3, 3, 7)
#define PPUTLUTRAITS_503  0b0111110111u, 8, 22, ()
#define PPUTLUTRAITS_502  0b0111110110u, 8, 22, (2, 251)
#define PPUTLUTRAITS_501  0b0111110101u, 8, 22, (3, 167)
#define PPUTLUTRAITS_500  0b0111110100u, 8, 22, (2, 2, 5, 5, 5)
#define PPUTLUTRAITS_499  0b0111110011u, 8, 22, ()
#define PPUTLUTRAITS_498  0b0111110010u, 8, 22, (2, 3, 83)
#define PPUTLUTRAITS_497  0b0111110001u, 8, 22, (7, 71)
#define PPUTLUTRAITS_496  0b0111110000u, 8, 22, (2, 2, 2, 2, 31)
#define PPUTLUTRAITS_495  0b0111101111u, 8, 22, (3, 3, 5, 11)
#define PPUTLUTRAITS_494  0b0111101110u, 8, 22, (2, 13, 19)
#define PPUTLUTRAITS_493  0b0111101101u, 8, 22, (17, 29)
#define PPUTLUTRAITS_492  0b0111101100u, 8, 22, (2, 2, 3, 41)
#define PPUTLUTRAITS_491  0b0111101011u, 8, 22, ()
#define PPUTLUTRAITS_490  0b0111101010u, 8, 22, (2, 5, 7, 7)
#define PPUTLUTRAITS_489  0b0111101001u, 8, 22, (3, 163)
#define PPUTLUTRAITS_488  0b0111101000u, 8, 22, (2, 2, 2, 61)
#define PPUTLUTRAITS_487  0b0111100111u, 8, 22, ()
#define PPUTLUTRAITS_486  0b0111100110u, 8, 22, (2, 3, 3, 3, 3, 3)
#define PPUTLUTRAITS_485  0b0111100101u, 8, 22, (5, 97)
#define PPUTLUTRAITS_484  0b0111100100u, 8, 22, (2, 2, 11, 11)
#define PPUTLUTRAITS_483  0b0111100011u, 8, 21, (3, 7, 23)
#define PPUTLUTRAITS_482  0b0111100010u, 8, 21, (2, 241)
#define PPUTLUTRAITS_481  0b0111100001u, 8, 21, (13, 37)
#define PPUTLUTRAITS_480  0b0111100000u, 8, 21, (2, 2, 2, 2, 2, 3, 5)
#define PPUTLUTRAITS_479  0b0111011111u, 8, 21, ()
#define PPUTLUTRAITS_478  0b0111011110u, 8, 21, (2, 239)
#define PPUTLUTRAITS_477  0b0111011101u, 8, 21, (3, 3, 53)
#define PPUTLUTRAITS_476  0b0111011100u, 8, 21, (2, 2, 7, 17)
#define PPUTLUTRAITS_475  0b0111011011u, 8, 21, (5, 5, 19)
#define PPUTLUTRAITS_474  0b0111011010u, 8, 21, (2, 3, 79)
#define PPUTLUTRAITS_473  0b0111011001u, 8, 21, (11, 43)
#define PPUTLUTRAITS_472  0b0111011000u, 8, 21, (2, 2, 2, 59)
#define PPUTLUTRAITS_471  0b0111010111u, 8, 21, (3, 157)
#define PPUTLUTRAITS_470  0b0111010110u, 8, 21, (2, 5, 47)
#define PPUTLUTRAITS_469  0b0111010101u, 8, 21, (7, 67)
#define PPUTLUTRAITS_468  0b0111010100u, 8, 21, (2, 2, 3, 3, 13)
#define PPUTLUTRAITS_467  0b0111010011u, 8, 21, ()
#define PPUTLUTRAITS_466  0b0111010010u, 8, 21, (2, 233)
#define PPUTLUTRAITS_465  0b0111010001u, 8, 21, (3, 5, 31)
#define PPUTLUTRAITS_464  0b0111010000u, 8, 21, (2, 2, 2, 2, 29)
#define PPUTLUTRAITS_463  0b0111001111u, 8, 21, ()
#define PPUTLUTRAITS_462  0b0111001110u, 8, 21, (2, 3, 7, 11)
#define PPUTLUTRAITS_461  0b0111001101u, 8, 21, ()
#define PPUTLUTRAITS_460  0b0111001100u, 8, 21, (2, 2, 5, 23)
#define PPUTLUTRAITS_459  0b0111001011u, 8, 21, (3, 3, 3, 17)
#define PPUTLUTRAITS_458  0b0111001010u, 8, 21, (2, 229)
#define PPUTLUTRAITS_457  0b0111001001u, 8, 21, ()
#define PPUTLUTRAITS_456  0b0111001000u, 8, 21, (2, 2, 2, 3, 19)
#define PPUTLUTRAITS_455  0b0111000111u, 8, 21, (5, 7, 13)
#define PPUTLUTRAITS_454  0b0111000110u, 8, 21, (2, 227)
#define PPUTLUTRAITS_453  0b0111000101u, 8, 21, (3, 151)
#define PPUTLUTRAITS_452  0b0111000100u, 8, 21, (2, 2, 113)
#define PPUTLUTRAITS_451  0b0111000011u, 8, 21, (11, 41)
#define PPUTLUTRAITS_450  0b0111000010u, 8, 21, (2, 3, 3, 5, 5)
#define PPUTLUTRAITS_449  0b0111000001u, 8, 21, ()
#define PPUTLUTRAITS_448  0b0111000000u, 8, 21, (2, 2, 2, 2, 2, 2, 7)
#define PPUTLUTRAITS_447  0b0110111111u, 8, 21, (3, 149)
#define PPUTLUTRAITS_446  0b0110111110u, 8, 21, (2, 223)
#define PPUTLUTRAITS_445  0b0110111101u, 8, 21, (5, 89)
#define PPUTLUTRAITS_444  0b0110111100u, 8, 21, (2, 2, 3, 37)
#define PPUTLUTRAITS_443  0b0110111011u, 8, 21, ()
#define PPUTLUTRAITS_442  0b0110111010u, 8, 21, (2, 13, 17)
#define PPUTLUTRAITS_441  0b0110111001u, 8, 21, (3, 3, 7, 7)
#define PPUTLUTRAITS_440  0b0110111000u, 8, 20, (2, 2, 2, 5, 11)
#define PPUTLUTRAITS_439  0b0110110111u, 8, 20, ()
#define PPUTLUTRAITS_438  0b0110110110u, 8, 20, (2, 3, 73)
#define PPUTLUTRAITS_437  0b0110110101u, 8, 20, (19, 23)
#define PPUTLUTRAITS_436  0b0110110100u, 8, 20, (2, 2, 109)
#define PPUTLUTRAITS_435  0b0110110011u, 8, 20, (3, 5, 29)
#define PPUTLUTRAITS_434  0b0110110010u, 8, 20, (2, 7, 31)
#define PPUTLUTRAITS_433  0b0110110001u, 8, 20, ()
#define PPUTLUTRAITS_432  0b0110110000u, 8, 20, (2, 2, 2, 2, 3, 3, 3)
#define PPUTLUTRAITS_431  0b0110101111u, 8, 20, ()
#define PPUTLUTRAITS_430  0b0110101110u, 8, 20, (2, 5, 43)
#define PPUTLUTRAITS_429  0b0110101101u, 8, 20, (3, 11, 13)
#define PPUTLUTRAITS_428  0b0110101100u, 8, 20, (2, 2, 107)
#define PPUTLUTRAITS_427  0b0110101011u, 8, 20, (7, 61)
#define PPUTLUTRAITS_426  0b0110101010u, 8, 20, (2, 3, 71)
#define PPUTLUTRAITS_425  0b0110101001u, 8, 20, (5, 5, 17)
#define PPUTLUTRAITS_424  0b0110101000u, 8, 20, (2, 2, 2, 53)
#define PPUTLUTRAITS_423  0b0110100111u, 8, 20, (3, 3, 47)
#define PPUTLUTRAITS_422  0b0110100110u, 8, 20, (2, 211)
#define PPUTLUTRAITS_421  0b0110100101u, 8, 20, ()
#define PPUTLUTRAITS_420  0b0110100100u, 8, 20, (2, 2, 3, 5, 7)
#define PPUTLUTRAITS_419  0b0110100011u, 8, 20, ()
#define PPUTLUTRAITS_418  0b0110100010u, 8, 20, (2, 11, 19)
#define PPUTLUTRAITS_417  0b0110100001u, 8, 20, (3, 139)
#define PPUTLUTRAITS_416  0b0110100000u, 8, 20, (2, 2, 2, 2, 2, 13)
#define PPUTLUTRAITS_415  0b0110011111u, 8, 20, (5, 83)
#define PPUTLUTRAITS_414  0b0110011110u, 8, 20, (2, 3, 3, 23)
#define PPUTLUTRAITS_413  0b0110011101u, 8, 20, (7, 59)
#define PPUTLUTRAITS_412  0b0110011100u, 8, 20, (2, 2, 103)
#define PPUTLUTRAITS_411  0b0110011011u, 8, 20, (3, 137)
#define PPUTLUTRAITS_410  0b0110011010u, 8, 20, (2, 5, 41)
#define PPUTLUTRAITS_409  0b0110011001u, 8, 20, ()
#define PPUTLUTRAITS_408  0b0110011000u, 8, 20, (2, 2, 2, 3, 17)
#define PPUTLUTRAITS_407  0b0110010111u, 8, 20, (11, 37)
#define PPUTLUTRAITS_406  0b0110010110u, 8, 20, (2, 7, 29)
#define PPUTLUTRAITS_405  0b0110010101u, 8, 20, (3, 3, 3, 3, 5)
#define PPUTLUTRAITS_404  0b0110010100u, 8, 20, (2, 2, 101)
#define PPUTLUTRAITS_403  0b0110010011u, 8, 20, (13, 31)
#define PPUTLUTRAITS_402  0b0110010010u, 8, 20, (2, 3, 67)
#define PPUTLUTRAITS_401  0b0110010001u, 8, 20, ()
#define PPUTLUTRAITS_400  0b0110010000u, 8, 20, (2, 2, 2, 2, 5, 5)
#define PPUTLUTRAITS_399  0b0110001111u, 8, 19, (3, 7, 19)
#define PPUTLUTRAITS_398  0b0110001110u, 8, 19, (2, 199)
#define PPUTLUTRAITS_397  0b0110001101u, 8, 19, ()
#define PPUTLUTRAITS_396  0b0110001100u, 8, 19, (2, 2, 3, 3, 11)
#define PPUTLUTRAITS_395  0b0110001011u, 8, 19, (5, 79)
#define PPUTLUTRAITS_394  0b0110001010u, 8, 19, (2, 197)
#define PPUTLUTRAITS_393  0b0110001001u, 8, 19, (3, 131)
#define PPUTLUTRAITS_392  0b0110001000u, 8, 19, (2, 2, 2, 7, 7)
#define PPUTLUTRAITS_391  0b0110000111u, 8, 19, (17, 23)
#define PPUTLUTRAITS_390  0b0110000110u, 8, 19, (2, 3, 5, 13)
#define PPUTLUTRAITS_389  0b0110000101u, 8, 19, ()
#define PPUTLUTRAITS_388  0b0110000100u, 8, 19, (2, 2, 97)
#define PPUTLUTRAITS_387  0b0110000011u, 8, 19, (3, 3, 43)
#define PPUTLUTRAITS_386  0b0110000010u, 8, 19, (2, 193)
#define PPUTLUTRAITS_385  0b0110000001u, 8, 19, (5, 7, 11)
#define PPUTLUTRAITS_384  0b0110000000u, 8, 19, (2, 2, 2, 2, 2, 2, 2, 3)
#define PPUTLUTRAITS_383  0b0101111111u, 8, 19, ()
#define PPUTLUTRAITS_382  0b0101111110u, 8, 19, (2, 191)
#define PPUTLUTRAITS_381  0b0101111101u, 8, 19, (3, 127)
#define PPUTLUTRAITS_380  0b0101111100u, 8, 19, (2, 2, 5, 19)
#define PPUTLUTRAITS_379  0b0101111011u, 8, 19, ()
#define PPUTLUTRAITS_378  0b0101111010u, 8, 19, (2, 3, 3, 3, 7)
#define PPUTLUTRAITS_377  0b0101111001u, 8, 19, (13, 29)
#define PPUTLUTRAITS_376  0b0101111000u, 8, 19, (2, 2, 2, 47)
#define PPUTLUTRAITS_375  0b0101110111u, 8, 19, (3, 5, 5, 5)
#define PPUTLUTRAITS_374  0b0101110110u, 8, 19, (2, 11, 17)
#define PPUTLUTRAITS_373  0b0101110101u, 8, 19, ()
#define PPUTLUTRAITS_372  0b0101110100u, 8, 19, (2, 2, 3, 31)
#define PPUTLUTRAITS_371  0b0101110011u, 8, 19, (7, 53)
#define PPUTLUTRAITS_370  0b0101110010u, 8, 19, (2, 5, 37)
#define PPUTLUTRAITS_369  0b0101110001u, 8, 19, (3, 3, 41)
#define PPUTLUTRAITS_368  0b0101110000u, 8, 19, (2, 2, 2, 2, 23)
#define PPUTLUTRAITS_367  0b0101101111u, 8, 19, ()
#define PPUTLUTRAITS_366  0b0101101110u, 8, 19, (2, 3, 61)
#define PPUTLUTRAITS_365  0b0101101101u, 8, 19, (5, 73)
#define PPUTLUTRAITS_364  0b0101101100u, 8, 19, (2, 2, 7, 13)
#define PPUTLUTRAITS_363  0b0101101011u, 8, 19, (3, 11, 11)
#define PPUTLUTRAITS_362  0b0101101010u, 8, 19, (2, 181)
#define PPUTLUTRAITS_361  0b0101101001u, 8, 19, (19, 19)
#define PPUTLUTRAITS_360  0b0101101000u, 8, 18, (2, 2, 2, 3, 3, 5)
#define PPUTLUTRAITS_359  0b0101100111u, 8, 18, ()
#define PPUTLUTRAITS_358  0b0101100110u, 8, 18, (2, 179)
#define PPUTLUTRAITS_357  0b0101100101u, 8, 18, (3, 7, 17)
#define PPUTLUTRAITS_356  0b0101100100u, 8, 18, (2, 2, 89)
#define PPUTLUTRAITS_355  0b0101100011u, 8, 18, (5, 71)
#define PPUTLUTRAITS_354  0b0101100010u, 8, 18, (2, 3, 59)
#define PPUTLUTRAITS_353  0b0101100001u, 8, 18, ()
#define PPUTLUTRAITS_352  0b0101100000u, 8, 18, (2, 2, 2, 2, 2, 11)
#define PPUTLUTRAITS_351  0b0101011111u, 8, 18, (3, 3, 3, 13)
#define PPUTLUTRAITS_350  0b0101011110u, 8, 18, (2, 5, 5, 7)
#define PPUTLUTRAITS_349  0b0101011101u, 8, 18, ()
#define PPUTLUTRAITS_348  0b0101011100u, 8, 18, (2, 2, 3, 29)
#define PPUTLUTRAITS_347  0b0101011011u, 8, 18, ()
#define PPUTLUTRAITS_346  0b0101011010u, 8, 18, (2, 173)
#define PPUTLUTRAITS_345  0b0101011001u, 8, 18, (3, 5, 23)
#define PPUTLUTRAITS_344  0b0101011000u, 8, 18, (2, 2, 2, 43)
#define PPUTLUTRAITS_343  0b0101010111u, 8, 18, (7, 7, 7)
#define PPUTLUTRAITS_342  0b0101010110u, 8, 18, (2, 3, 3, 19)
#define PPUTLUTRAITS_341  0b0101010101u, 8, 18, (11, 31)
#define PPUTLUTRAITS_340  0b0101010100u, 8, 18, (2, 2, 5, 17)
#define PPUTLUTRAITS_339  0b0101010011u, 8, 18, (3, 113)
#define PPUTLUTRAITS_338  0b0101010010u, 8, 18, (2, 13, 13)
#define PPUTLUTRAITS_337  0b0101010001u, 8, 18, ()
#define PPUTLUTRAITS_336  0b0101010000u, 8, 18, (2, 2, 2, 2, 3, 7)
#define PPUTLUTRAITS_335  0b0101001111u, 8, 18, (5, 67)
#define PPUTLUTRAITS_334  0b0101001110u, 8, 18, (2, 167)
#define PPUTLUTRAITS_333  0b0101001101u, 8, 18, (3, 3, 37)
#define PPUTLUTRAITS_332  0b0101001100u, 8, 18, (2, 2, 83)
#define PPUTLUTRAITS_331  0b0101001011u, 8, 18, ()
#define PPUTLUTRAITS_330  0b0101001010u, 8, 18, (2, 3, 5, 11)
#define PPUTLUTRAITS_329  0b0101001001u, 8, 18, (7, 47)
#define PPUTLUTRAITS_328  0b0101001000u, 8, 18, (2, 2, 2, 41)
#define PPUTLUTRAITS_327  0b0101000111u, 8, 18, (3, 109)
#define PPUTLUTRAITS_326  0b0101000110u, 8, 18, (2, 163)
#define PPUTLUTRAITS_325  0b0101000101u, 8, 18, (5, 5, 13)
#define PPUTLUTRAITS_324  0b0101000100u, 8, 18, (2, 2, 3, 3, 3, 3)
#define PPUTLUTRAITS_323  0b0101000011u, 8, 17, (17, 19)
#define PPUTLUTRAITS_322  0b0101000010u, 8, 17, (2, 7, 23)
#define PPUTLUTRAITS_321  0b0101000001u, 8, 17, (3, 107)
#define PPUTLUTRAITS_320  0b0101000000u, 8, 17, (2, 2, 2, 2, 2, 2, 5)
#define PPUTLUTRAITS_319  0b0100111111u, 8, 17, (11, 29)
#define PPUTLUTRAITS_318  0b0100111110u, 8, 17, (2, 3, 53)
#define PPUTLUTRAITS_317  0b0100111101u, 8, 17, ()
#define PPUTLUTRAITS_316  0b0100111100u, 8, 17, (2, 2, 79)
#define PPUTLUTRAITS_315  0b0100111011u, 8, 17, (3, 3, 5, 7)
#define PPUTLUTRAITS_314  0b0100111010u, 8, 17, (2, 157)
#define PPUTLUTRAITS_313  0b0100111001u, 8, 17, ()
#define PPUTLUTRAITS_312  0b0100111000u, 8, 17, (2, 2, 2, 3, 13)
#define PPUTLUTRAITS_311  0b0100110111u, 8, 17, ()
#define PPUTLUTRAITS_310  0b0100110110u, 8, 17, (2, 5, 31)
#define PPUTLUTRAITS_309  0b0100110101u, 8, 17, (3, 103)
#define PPUTLUTRAITS_308  0b0100110100u, 8, 17, (2, 2, 7, 11)
#define PPUTLUTRAITS_307  0b0100110011u, 8, 17, ()
#define PPUTLUTRAITS_306  0b0100110010u, 8, 17, (2, 3, 3, 17)
#define PPUTLUTRAITS_305  0b0100110001u, 8, 17, (5, 61)
#define PPUTLUTRAITS_304  0b0100110000u, 8, 17, (2, 2, 2, 2, 19)
#define PPUTLUTRAITS_303  0b0100101111u, 8, 17, (3, 101)
#define PPUTLUTRAITS_302  0b0100101110u, 8, 17, (2, 151)
#define PPUTLUTRAITS_301  0b0100101101u, 8, 17, (7, 43)
#define PPUTLUTRAITS_300  0b0100101100u, 8, 17, (2, 2, 3, 5, 5)
#define PPUTLUTRAITS_299  0b0100101011u, 8, 17, (13, 23)
#define PPUTLUTRAITS_298  0b0100101010u, 8, 17, (2, 149)
#define PPUTLUTRAITS_297  0b0100101001u, 8, 17, (3, 3, 3, 11)
#define PPUTLUTRAITS_296  0b0100101000u, 8, 17, (2, 2, 2, 37)
#define PPUTLUTRAITS_295  0b0100100111u, 8, 17, (5, 59)
#define PPUTLUTRAITS_294  0b0100100110u, 8, 17, (2, 3, 7, 7)
#define PPUTLUTRAITS_293  0b0100100101u, 8, 17, ()
#define PPUTLUTRAITS_292  0b0100100100u, 8, 17, (2, 2, 73)
#define PPUTLUTRAITS_291  0b0100100011u, 8, 17, (3, 97)
#define PPUTLUTRAITS_290  0b0100100010u, 8, 17, (2, 5, 29)
#define PPUTLUTRAITS_289  0b0100100001u, 8, 17, (17, 17)
#define PPUTLUTRAITS_288  0b0100100000u, 8, 16, (2, 2, 2, 2, 2, 3, 3)
#define PPUTLUTRAITS_287  0b0100011111u, 8, 16, (7, 41)
#define PPUTLUTRAITS_286  0b0100011110u, 8, 16, (2, 11, 13)
#define PPUTLUTRAITS_285  0b0100011101u, 8, 16, (3, 5, 19)
#define PPUTLUTRAITS_284  0b0100011100u, 8, 16, (2, 2, 71)
#define PPUTLUTRAITS_283  0b0100011011u, 8, 16, ()
#define PPUTLUTRAITS_282  0b0100011010u, 8, 16, (2, 3, 47)
#define PPUTLUTRAITS_281  0b0100011001u, 8, 16, ()
#define PPUTLUTRAITS_280  0b0100011000u, 8, 16, (2, 2, 2, 5, 7)
#define PPUTLUTRAITS_279  0b0100010111u, 8, 16, (3, 3, 31)
#define PPUTLUTRAITS_278  0b0100010110u, 8, 16, (2, 139)
#define PPUTLUTRAITS_277  0b0100010101u, 8, 16, ()
#define PPUTLUTRAITS_276  0b0100010100u, 8, 16, (2, 2, 3, 23)
#define PPUTLUTRAITS_275  0b0100010011u, 8, 16, (5, 5, 11)
#define PPUTLUTRAITS_274  0b0100010010u, 8, 16, (2, 137)
#define PPUTLUTRAITS_273  0b0100010001u, 8, 16, (3, 7, 13)
#define PPUTLUTRAITS_272  0b0100010000u, 8, 16, (2, 2, 2, 2, 17)
#define PPUTLUTRAITS_271  0b0100001111u, 8, 16, ()
#define PPUTLUTRAITS_270  0b0100001110u, 8, 16, (2, 3, 3, 3, 5)
#define PPUTLUTRAITS_269  0b0100001101u, 8, 16, ()
#define PPUTLUTRAITS_268  0b0100001100u, 8, 16, (2, 2, 67)
#define PPUTLUTRAITS_267  0b0100001011u, 8, 16, (3, 89)
#define PPUTLUTRAITS_266  0b0100001010u, 8, 16, (2, 7, 19)
#define PPUTLUTRAITS_265  0b0100001001u, 8, 16, (5, 53)
#define PPUTLUTRAITS_264  0b0100001000u, 8, 16, (2, 2, 2, 3, 11)
#define PPUTLUTRAITS_263  0b0100000111u, 8, 16, ()
#define PPUTLUTRAITS_262  0b0100000110u, 8, 16, (2, 131)
#define PPUTLUTRAITS_261  0b0100000101u, 8, 16, (3, 3, 29)
#define PPUTLUTRAITS_260  0b0100000100u, 8, 16, (2, 2, 5, 13)
#define PPUTLUTRAITS_259  0b0100000011u, 8, 16, (7, 37)
#define PPUTLUTRAITS_258  0b0100000010u, 8, 16, (2, 3, 43)
#define PPUTLUTRAITS_257  0b0100000001u, 8, 16, ()
#define PPUTLUTRAITS_256  0b0100000000u, 8, 16, (2, 2, 2, 2, 2, 2, 2, 2)
#define PPUTLUTRAITS_255  0b0011111111u, 7, 15, (3, 5, 17)
#define PPUTLUTRAITS_254  0b0011111110u, 7, 15, (2, 127)
#define PPUTLUTRAITS_253  0b0011111101u, 7, 15, (11, 23)
#define PPUTLUTRAITS_252  0b0011111100u, 7, 15, (2, 2, 3, 3, 7)
#define PPUTLUTRAITS_251  0b0011111011u, 7, 15, ()
#define PPUTLUTRAITS_250  0b0011111010u, 7, 15, (2, 5, 5, 5)
#define PPUTLUTRAITS_249  0b0011111001u, 7, 15, (3, 83)
#define PPUTLUTRAITS_248  0b0011111000u, 7, 15, (2, 2, 2, 31)
#define PPUTLUTRAITS_247  0b0011110111u, 7, 15, (13, 19)
#define PPUTLUTRAITS_246  0b0011110110u, 7, 15, (2, 3, 41)
#define PPUTLUTRAITS_245  0b0011110101u, 7, 15, (5, 7, 7)
#define PPUTLUTRAITS_244  0b0011110100u, 7, 15, (2, 2, 61)
#define PPUTLUTRAITS_243  0b0011110011u, 7, 15, (3, 3, 3, 3, 3)
#define PPUTLUTRAITS_242  0b0011110010u, 7, 15, (2, 11, 11)
#define PPUTLUTRAITS_241  0b0011110001u, 7, 15, ()
#define PPUTLUTRAITS_240  0b0011110000u, 7, 15, (2, 2, 2, 2, 3, 5)
#define PPUTLUTRAITS_239  0b0011101111u, 7, 15, ()
#define PPUTLUTRAITS_238  0b0011101110u, 7, 15, (2, 7, 17)
#define PPUTLUTRAITS_237  0b0011101101u, 7, 15, (3, 79)
#define PPUTLUTRAITS_236  0b0011101100u, 7, 15, (2, 2, 59)
#define PPUTLUTRAITS_235  0b0011101011u, 7, 15, (5, 47)
#define PPUTLUTRAITS_234  0b0011101010u, 7, 15, (2, 3, 3, 13)
#define PPUTLUTRAITS_233  0b0011101001u, 7, 15, ()
#define PPUTLUTRAITS_232  0b0011101000u, 7, 15, (2, 2, 2, 29)
#define PPUTLUTRAITS_231  0b0011100111u, 7, 15, (3, 7, 11)
#define PPUTLUTRAITS_230  0b0011100110u, 7, 15, (2, 5, 23)
#define PPUTLUTRAITS_229  0b0011100101u, 7, 15, ()
#define PPUTLUTRAITS_228  0b0011100100u, 7, 15, (2, 2, 3, 19)
#define PPUTLUTRAITS_227  0b0011100011u, 7, 15, ()
#define PPUTLUTRAITS_226  0b0011100010u, 7, 15, (2, 113)
#define PPUTLUTRAITS_225  0b0011100001u, 7, 15, (3, 3, 5, 5)
#define PPUTLUTRAITS_224  0b0011100000u, 7, 14, (2, 2, 2, 2, 2, 7)
#define PPUTLUTRAITS_223  0b0011011111u, 7, 14, ()
#define PPUTLUTRAITS_222  0b0011011110u, 7, 14, (2, 3, 37)
#define PPUTLUTRAITS_221  0b0011011101u, 7, 14, (13, 17)
#define PPUTLUTRAITS_220  0b0011011100u, 7, 14, (2, 2, 5, 11)
#define PPUTLUTRAITS_219  0b0011011011u, 7, 14, (3, 73)
#define PPUTLUTRAITS_218  0b0011011010u, 7, 14, (2, 109)
#define PPUTLUTRAITS_217  0b0011011001u, 7, 14, (7, 31)
#define PPUTLUTRAITS_216  0b0011011000u, 7, 14, (2, 2, 2, 3, 3, 3)
#define PPUTLUTRAITS_215  0b0011010111u, 7, 14, (5, 43)
#define PPUTLUTRAITS_214  0b0011010110u, 7, 14, (2, 107)
#define PPUTLUTRAITS_213  0b0011010101u, 7, 14, (3, 71)
#define PPUTLUTRAITS_212  0b0011010100u, 7, 14, (2, 2, 53)
#define PPUTLUTRAITS_211  0b0011010011u, 7, 14, ()
#define PPUTLUTRAITS_210  0b0011010010u, 7, 14, (2, 3, 5, 7)
#define PPUTLUTRAITS_209  0b0011010001u, 7, 14, (11, 19)
#define PPUTLUTRAITS_208  0b0011010000u, 7, 14, (2, 2, 2, 2, 13)
#define PPUTLUTRAITS_207  0b0011001111u, 7, 14, (3, 3, 23)
#define PPUTLUTRAITS_206  0b0011001110u, 7, 14, (2, 103)
#define PPUTLUTRAITS_205  0b0011001101u, 7, 14, (5, 41)
#define PPUTLUTRAITS_204  0b0011001100u, 7, 14, (2, 2, 3, 17)
#define PPUTLUTRAITS_203  0b0011001011u, 7, 14, (7, 29)
#define PPUTLUTRAITS_202  0b0011001010u, 7, 14, (2, 101)
#define PPUTLUTRAITS_201  0b0011001001u, 7, 14, (3, 67)
#define PPUTLUTRAITS_200  0b0011001000u, 7, 14, (2, 2, 2, 5, 5)
#define PPUTLUTRAITS_199  0b0011000111u, 7, 14, ()
#define PPUTLUTRAITS_198  0b0011000110u, 7, 14, (2, 3, 3, 11)
#define PPUTLUTRAITS_197  0b0011000101u, 7, 14, ()
#define PPUTLUTRAITS_196  0b0011000100u, 7, 14, (2, 2, 7, 7)
#define PPUTLUTRAITS_195  0b0011000011u, 7, 13, (3, 5, 13)
#define PPUTLUTRAITS_194  0b0011000010u, 7, 13, (2, 97)
#define PPUTLUTRAITS_193  0b0011000001u, 7, 13, ()
#define PPUTLUTRAITS_192  0b0011000000u, 7, 13, (2, 2, 2, 2, 2, 2, 3)
#define PPUTLUTRAITS_191  0b0010111111u, 7, 13, ()
#define PPUTLUTRAITS_190  0b0010111110u, 7, 13, (2, 5, 19)
#define PPUTLUTRAITS_189  0b0010111101u, 7, 13, (3, 3, 3, 7)
#define PPUTLUTRAITS_188  0b0010111100u, 7, 13, (2, 2, 47)
#define PPUTLUTRAITS_187  0b0010111011u, 7, 13, (11, 17)
#define PPUTLUTRAITS_186  0b0010111010u, 7, 13, (2, 3, 31)
#define PPUTLUTRAITS_185  0b0010111001u, 7, 13, (5, 37)
#define PPUTLUTRAITS_184  0b0010111000u, 7, 13, (2, 2, 2, 23)
#define PPUTLUTRAITS_183  0b0010110111u, 7, 13, (3, 61)
#define PPUTLUTRAITS_182  0b0010110110u, 7, 13, (2, 7, 13)
#define PPUTLUTRAITS_181  0b0010110101u, 7, 13, ()
#define PPUTLUTRAITS_180  0b0010110100u, 7, 13, (2, 2, 3, 3, 5)
#define PPUTLUTRAITS_179  0b0010110011u, 7, 13, ()
#define PPUTLUTRAITS_178  0b0010110010u, 7, 13, (2, 89)
#define PPUTLUTRAITS_177  0b0010110001u, 7, 13, (3, 59)
#define PPUTLUTRAITS_176  0b0010110000u, 7, 13, (2, 2, 2, 2, 11)
#define PPUTLUTRAITS_175  0b0010101111u, 7, 13, (5, 5, 7)
#define PPUTLUTRAITS_174  0b0010101110u, 7, 13, (2, 3, 29)
#define PPUTLUTRAITS_173  0b0010101101u, 7, 13, ()
#define PPUTLUTRAITS_172  0b0010101100u, 7, 13, (2, 2, 43)
#define PPUTLUTRAITS_171  0b0010101011u, 7, 13, (3, 3, 19)
#define PPUTLUTRAITS_170  0b0010101010u, 7, 13, (2, 5, 17)
#define PPUTLUTRAITS_169  0b0010101001u, 7, 13, (13, 13)
#define PPUTLUTRAITS_168  0b0010101000u, 7, 12, (2, 2, 2, 3, 7)
#define PPUTLUTRAITS_167  0b0010100111u, 7, 12, ()
#define PPUTLUTRAITS_166  0b0010100110u, 7, 12, (2, 83)
#define PPUTLUTRAITS_165  0b0010100101u, 7, 12, (3, 5, 11)
#define PPUTLUTRAITS_164  0b0010100100u, 7, 12, (2, 2, 41)
#define PPUTLUTRAITS_163  0b0010100011u, 7, 12, ()
#define PPUTLUTRAITS_162  0b0010100010u, 7, 12, (2, 3, 3, 3, 3)
#define PPUTLUTRAITS_161  0b0010100001u, 7, 12, (7, 23)
#define PPUTLUTRAITS_160  0b0010100000u, 7, 12, (2, 2, 2, 2, 2, 5)
#define PPUTLUTRAITS_159  0b0010011111u, 7, 12, (3, 53)
#define PPUTLUTRAITS_158  0b0010011110u, 7, 12, (2, 79)
#define PPUTLUTRAITS_157  0b0010011101u, 7, 12, ()
#define PPUTLUTRAITS_156  0b0010011100u, 7, 12, (2, 2, 3, 13)
#define PPUTLUTRAITS_155  0b0010011011u, 7, 12, (5, 31)
#define PPUTLUTRAITS_154  0b0010011010u, 7, 12, (2, 7, 11)
#define PPUTLUTRAITS_153  0b0010011001u, 7, 12, (3, 3, 17)
#define PPUTLUTRAITS_152  0b0010011000u, 7, 12, (2, 2, 2, 19)
#define PPUTLUTRAITS_151  0b0010010111u, 7, 12, ()
#define PPUTLUTRAITS_150  0b0010010110u, 7, 12, (2, 3, 5, 5)
#define PPUTLUTRAITS_149  0b0010010101u, 7, 12, ()
#define PPUTLUTRAITS_148  0b0010010100u, 7, 12, (2, 2, 37)
#define PPUTLUTRAITS_147  0b0010010011u, 7, 12, (3, 7, 7)
#define PPUTLUTRAITS_146  0b0010010010u, 7, 12, (2, 73)
#define PPUTLUTRAITS_145  0b0010010001u, 7, 12, (5, 29)
#define PPUTLUTRAITS_144  0b0010010000u, 7, 12, (2, 2, 2, 2, 3, 3)
#define PPUTLUTRAITS_143  0b0010001111u, 7, 11, (11, 13)
#define PPUTLUTRAITS_142  0b0010001110u, 7, 11, (2, 71)
#define PPUTLUTRAITS_141  0b0010001101u, 7, 11, (3, 47)
#define PPUTLUTRAITS_140  0b0010001100u, 7, 11, (2, 2, 5, 7)
#define PPUTLUTRAITS_139  0b0010001011u, 7, 11, ()
#define PPUTLUTRAITS_138  0b0010001010u, 7, 11, (2, 3, 23)
#define PPUTLUTRAITS_137  0b0010001001u, 7, 11, ()
#define PPUTLUTRAITS_136  0b0010001000u, 7, 11, (2, 2, 2, 17)
#define PPUTLUTRAITS_135  0b0010000111u, 7, 11, (3, 3, 3, 5)
#define PPUTLUTRAITS_134  0b0010000110u, 7, 11, (2, 67)
#define PPUTLUTRAITS_133  0b0010000101u, 7, 11, (7, 19)
#define PPUTLUTRAITS_132  0b0010000100u, 7, 11, (2, 2, 3, 11)
#define PPUTLUTRAITS_131  0b0010000011u, 7, 11, ()
#define PPUTLUTRAITS_130  0b0010000010u, 7, 11, (2, 5, 13)
#define PPUTLUTRAITS_129  0b0010000001u, 7, 11, (3, 43)
#define PPUTLUTRAITS_128  0b0010000000u, 7, 11, (2, 2, 2, 2, 2, 2, 2)
#define PPUTLUTRAITS_127  0b0001111111u, 6, 11, ()
#define PPUTLUTRAITS_126  0b0001111110u, 6, 11, (2, 3, 3, 7)
#define PPUTLUTRAITS_125  0b0001111101u, 6, 11, (5, 5, 5)
#define PPUTLUTRAITS_124  0b0001111100u, 6, 11, (2, 2, 31)
#define PPUTLUTRAITS_123  0b0001111011u, 6, 11, (3, 41)
#define PPUTLUTRAITS_122  0b0001111010u, 6, 11, (2, 61)
#define PPUTLUTRAITS_121  0b0001111001u, 6, 11, (11, 11)
#define PPUTLUTRAITS_120  0b0001111000u, 6, 10, (2, 2, 2, 3, 5)
#define PPUTLUTRAITS_119  0b0001110111u, 6, 10, (7, 17)
#define PPUTLUTRAITS_118  0b0001110110u, 6, 10, (2, 59)
#define PPUTLUTRAITS_117  0b0001110101u, 6, 10, (3, 3, 13)
#define PPUTLUTRAITS_116  0b0001110100u, 6, 10, (2, 2, 29)
#define PPUTLUTRAITS_115  0b0001110011u, 6, 10, (5, 23)
#define PPUTLUTRAITS_114  0b0001110010u, 6, 10, (2, 3, 19)
#define PPUTLUTRAITS_113  0b0001110001u, 6, 10, ()
#define PPUTLUTRAITS_112  0b0001110000u, 6, 10, (2, 2, 2, 2, 7)
#define PPUTLUTRAITS_111  0b0001101111u, 6, 10, (3, 37)
#define PPUTLUTRAITS_110  0b0001101110u, 6, 10, (2, 5, 11)
#define PPUTLUTRAITS_109  0b0001101101u, 6, 10, ()
#define PPUTLUTRAITS_108  0b0001101100u, 6, 10, (2, 2, 3, 3, 3)
#define PPUTLUTRAITS_107  0b0001101011u, 6, 10, ()
#define PPUTLUTRAITS_106  0b0001101010u, 6, 10, (2, 53)
#define PPUTLUTRAITS_105  0b0001101001u, 6, 10, (3, 5, 7)
#define PPUTLUTRAITS_104  0b0001101000u, 6, 10, (2, 2, 2, 13)
#define PPUTLUTRAITS_103  0b0001100111u, 6, 10, ()
#define PPUTLUTRAITS_102  0b0001100110u, 6, 10, (2, 3, 17)
#define PPUTLUTRAITS_101  0b0001100101u, 6, 10, ()
#define PPUTLUTRAITS_100  0b0001100100u, 6, 10, (2, 2, 5, 5)
#define PPUTLUTRAITS_99   0b0001100011u, 6, 9, (3, 3, 11)
#define PPUTLUTRAITS_98   0b0001100010u, 6, 9, (2, 7, 7)
#define PPUTLUTRAITS_97   0b0001100001u, 6, 9, ()
#define PPUTLUTRAITS_96   0b0001100000u, 6, 9, (2, 2, 2, 2, 2, 3)
#define PPUTLUTRAITS_95   0b0001011111u, 6, 9, (5, 19)
#define PPUTLUTRAITS_94   0b0001011110u, 6, 9, (2, 47)
#define PPUTLUTRAITS_93   0b0001011101u, 6, 9, (3, 31)
#define PPUTLUTRAITS_92   0b0001011100u, 6, 9, (2, 2, 23)
#define PPUTLUTRAITS_91   0b0001011011u, 6, 9, (7, 13)
#define PPUTLUTRAITS_90   0b0001011010u, 6, 9, (2, 3, 3, 5)
#define PPUTLUTRAITS_89   0b0001011001u, 6, 9, ()
#define PPUTLUTRAITS_88   0b0001011000u, 6, 9, (2, 2, 2, 11)
#define PPUTLUTRAITS_87   0b0001010111u, 6, 9, (3, 29)
#define PPUTLUTRAITS_86   0b0001010110u, 6, 9, (2, 43)
#define PPUTLUTRAITS_85   0b0001010101u, 6, 9, (5, 17)
#define PPUTLUTRAITS_84   0b0001010100u, 6, 9, (2, 2, 3, 7)
#define PPUTLUTRAITS_83   0b0001010011u, 6, 9, ()
#define PPUTLUTRAITS_82   0b0001010010u, 6, 9, (2, 41)
#define PPUTLUTRAITS_81   0b0001010001u, 6, 9, (3, 3, 3, 3)
#define PPUTLUTRAITS_80   0b0001010000u, 6, 8, (2, 2, 2, 2, 5)
#define PPUTLUTRAITS_79   0b0001001111u, 6, 8, ()
#define PPUTLUTRAITS_78   0b0001001110u, 6, 8, (2, 3, 13)
#define PPUTLUTRAITS_77   0b0001001101u, 6, 8, (7, 11)
#define PPUTLUTRAITS_76   0b0001001100u, 6, 8, (2, 2, 19)
#define PPUTLUTRAITS_75   0b0001001011u, 6, 8, (3, 5, 5)
#define PPUTLUTRAITS_74   0b0001001010u, 6, 8, (2, 37)
#define PPUTLUTRAITS_73   0b0001001001u, 6, 8, ()
#define PPUTLUTRAITS_72   0b0001001000u, 6, 8, (2, 2, 2, 3, 3)
#define PPUTLUTRAITS_71   0b0001000111u, 6, 8, ()
#define PPUTLUTRAITS_70   0b0001000110u, 6, 8, (2, 5, 7)
#define PPUTLUTRAITS_69   0b0001000101u, 6, 8, (3, 23)
#define PPUTLUTRAITS_68   0b0001000100u, 6, 8, (2, 2, 17)
#define PPUTLUTRAITS_67   0b0001000011u, 6, 8, ()
#define PPUTLUTRAITS_66   0b0001000010u, 6, 8, (2, 3, 11)
#define PPUTLUTRAITS_65   0b0001000001u, 6, 8, (5, 13)
#define PPUTLUTRAITS_64   0b0001000000u, 6, 8, (2, 2, 2, 2, 2, 2)
#define PPUTLUTRAITS_63   0b0000111111u, 5, 7, (3, 3, 7)
#define PPUTLUTRAITS_62   0b0000111110u, 5, 7, (2, 31)
#define PPUTLUTRAITS_61   0b0000111101u, 5, 7, ()
#define PPUTLUTRAITS_60   0b0000111100u, 5, 7, (2, 2, 3, 5)
#define PPUTLUTRAITS_59   0b0000111011u, 5, 7, ()
#define PPUTLUTRAITS_58   0b0000111010u, 5, 7, (2, 29)
#define PPUTLUTRAITS_57   0b0000111001u, 5, 7, (3, 19)
#define PPUTLUTRAITS_56   0b0000111000u, 5, 7, (2, 2, 2, 7)
#define PPUTLUTRAITS_55   0b0000110111u, 5, 7, (5, 11)
#define PPUTLUTRAITS_54   0b0000110110u, 5, 7, (2, 3, 3, 3)
#define PPUTLUTRAITS_53   0b0000110101u, 5, 7, ()
#define PPUTLUTRAITS_52   0b0000110100u, 5, 7, (2, 2, 13)
#define PPUTLUTRAITS_51   0b0000110011u, 5, 7, (3, 17)
#define PPUTLUTRAITS_50   0b0000110010u, 5, 7, (2, 5, 5)
#define PPUTLUTRAITS_49   0b0000110001u, 5, 7, (7, 7)
#define PPUTLUTRAITS_48   0b0000110000u, 5, 6, (2, 2, 2, 2, 3)
#define PPUTLUTRAITS_47   0b0000101111u, 5, 6, ()
#define PPUTLUTRAITS_46   0b0000101110u, 5, 6, (2, 23)
#define PPUTLUTRAITS_45   0b0000101101u, 5, 6, (3, 3, 5)
#define PPUTLUTRAITS_44   0b0000101100u, 5, 6, (2, 2, 11)
#define PPUTLUTRAITS_43   0b0000101011u, 5, 6, ()
#define PPUTLUTRAITS_42   0b0000101010u, 5, 6, (2, 3, 7)
#define PPUTLUTRAITS_41   0b0000101001u, 5, 6, ()
#define PPUTLUTRAITS_40   0b0000101000u, 5, 6, (2, 2, 2, 5)
#define PPUTLUTRAITS_39   0b0000100111u, 5, 6, (3, 13)
#define PPUTLUTRAITS_38   0b0000100110u, 5, 6, (2, 19)
#define PPUTLUTRAITS_37   0b0000100101u, 5, 6, ()
#define PPUTLUTRAITS_36   0b0000100100u, 5, 6, (2, 2, 3, 3)
#define PPUTLUTRAITS_35   0b0000100011u, 5, 5, (5, 7)
#define PPUTLUTRAITS_34   0b0000100010u, 5, 5, (2, 17)
#define PPUTLUTRAITS_33   0b0000100001u, 5, 5, (3, 11)
#define PPUTLUTRAITS_32   0b0000100000u, 5, 5, (2, 2, 2, 2, 2)
#define PPUTLUTRAITS_31   0b0000011111u, 4, 5, ()
#define PPUTLUTRAITS_30   0b0000011110u, 4, 5, (2, 3, 5)
#define PPUTLUTRAITS_29   0b0000011101u, 4, 5, ()
#define PPUTLUTRAITS_28   0b0000011100u, 4, 5, (2, 2, 7)
#define PPUTLUTRAITS_27   0b0000011011u, 4, 5, (3, 3, 3)
#define PPUTLUTRAITS_26   0b0000011010u, 4, 5, (2, 13)
#define PPUTLUTRAITS_25   0b0000011001u, 4, 5, (5, 5)
#define PPUTLUTRAITS_24   0b0000011000u, 4, 4, (2, 2, 2, 3)
#define PPUTLUTRAITS_23   0b0000010111u, 4, 4, ()
#define PPUTLUTRAITS_22   0b0000010110u, 4, 4, (2, 11)
#define PPUTLUTRAITS_21   0b0000010101u, 4, 4, (3, 7)
#define PPUTLUTRAITS_20   0b0000010100u, 4, 4, (2, 2, 5)
#define PPUTLUTRAITS_19   0b0000010011u, 4, 4, ()
#define PPUTLUTRAITS_18   0b0000010010u, 4, 4, (2, 3, 3)
#define PPUTLUTRAITS_17   0b0000010001u, 4, 4, ()
#define PPUTLUTRAITS_16   0b0000010000u, 4, 4, (2, 2, 2, 2)
#define PPUTLUTRAITS_15   0b0000001111u, 3, 3, (3, 5)
#define PPUTLUTRAITS_14   0b0000001110u, 3, 3, (2, 7)
#define PPUTLUTRAITS_13   0b0000001101u, 3, 3, ()
#define PPUTLUTRAITS_12   0b0000001100u, 3, 3, (2, 2, 3)
#define PPUTLUTRAITS_11   0b0000001011u, 3, 3, ()
#define PPUTLUTRAITS_10   0b0000001010u, 3, 3, (2, 5)
#define PPUTLUTRAITS_9    0b0000001001u, 3, 3, (3, 3)
#define PPUTLUTRAITS_8    0b0000001000u, 3, 2, (2, 2, 2)
#define PPUTLUTRAITS_7    0b0000000111u, 2, 2, ()
#define PPUTLUTRAITS_6    0b0000000110u, 2, 2, (2, 3)
#define PPUTLUTRAITS_5    0b0000000101u, 2, 2, ()
#define PPUTLUTRAITS_4    0b0000000100u, 2, 2, (2, 2)
#define PPUTLUTRAITS_3    0b0000000011u, 1, 1, ()
#define PPUTLUTRAITS_2    0b0000000010u, 1, 1, ()
#define PPUTLUTRAITS_1    0b0000000001u, 0, 1, ()
#define PPUTLUTRAITS_0    0b0000000000u, , 0, ()

/// decimal, bits, bitnot
#define PPUTLUTRAITS_0b1111111111u 1023, (1, 1, 1, 1, 1, 1, 1, 1, 1, 1), 0b0000000000u
#define PPUTLUTRAITS_0b1111111110u 1022, (1, 1, 1, 1, 1, 1, 1, 1, 1, 0), 0b0000000001u
#define PPUTLUTRAITS_0b1111111101u 1021, (1, 1, 1, 1, 1, 1, 1, 1, 0, 1), 0b0000000010u
#define PPUTLUTRAITS_0b1111111100u 1020, (1, 1, 1, 1, 1, 1, 1, 1, 0, 0), 0b0000000011u
#define PPUTLUTRAITS_0b1111111011u 1019, (1, 1, 1, 1, 1, 1, 1, 0, 1, 1), 0b0000000100u
#define PPUTLUTRAITS_0b1111111010u 1018, (1, 1, 1, 1, 1, 1, 1, 0, 1, 0), 0b0000000101u
#define PPUTLUTRAITS_0b1111111001u 1017, (1, 1, 1, 1, 1, 1, 1, 0, 0, 1), 0b0000000110u
#define PPUTLUTRAITS_0b1111111000u 1016, (1, 1, 1, 1, 1, 1, 1, 0, 0, 0), 0b0000000111u
#define PPUTLUTRAITS_0b1111110111u 1015, (1, 1, 1, 1, 1, 1, 0, 1, 1, 1), 0b0000001000u
#define PPUTLUTRAITS_0b1111110110u 1014, (1, 1, 1, 1, 1, 1, 0, 1, 1, 0), 0b0000001001u
#define PPUTLUTRAITS_0b1111110101u 1013, (1, 1, 1, 1, 1, 1, 0, 1, 0, 1), 0b0000001010u
#define PPUTLUTRAITS_0b1111110100u 1012, (1, 1, 1, 1, 1, 1, 0, 1, 0, 0), 0b0000001011u
#define PPUTLUTRAITS_0b1111110011u 1011, (1, 1, 1, 1, 1, 1, 0, 0, 1, 1), 0b0000001100u
#define PPUTLUTRAITS_0b1111110010u 1010, (1, 1, 1, 1, 1, 1, 0, 0, 1, 0), 0b0000001101u
#define PPUTLUTRAITS_0b1111110001u 1009, (1, 1, 1, 1, 1, 1, 0, 0, 0, 1), 0b0000001110u
#define PPUTLUTRAITS_0b1111110000u 1008, (1, 1, 1, 1, 1, 1, 0, 0, 0, 0), 0b0000001111u
#define PPUTLUTRAITS_0b1111101111u 1007, (1, 1, 1, 1, 1, 0, 1, 1, 1, 1), 0b0000010000u
#define PPUTLUTRAITS_0b1111101110u 1006, (1, 1, 1, 1, 1, 0, 1, 1, 1, 0), 0b0000010001u
#define PPUTLUTRAITS_0b1111101101u 1005, (1, 1, 1, 1, 1, 0, 1, 1, 0, 1), 0b0000010010u
#define PPUTLUTRAITS_0b1111101100u 1004, (1, 1, 1, 1, 1, 0, 1, 1, 0, 0), 0b0000010011u
#define PPUTLUTRAITS_0b1111101011u 1003, (1, 1, 1, 1, 1, 0, 1, 0, 1, 1), 0b0000010100u
#define PPUTLUTRAITS_0b1111101010u 1002, (1, 1, 1, 1, 1, 0, 1, 0, 1, 0), 0b0000010101u
#define PPUTLUTRAITS_0b1111101001u 1001, (1, 1, 1, 1, 1, 0, 1, 0, 0, 1), 0b0000010110u
#define PPUTLUTRAITS_0b1111101000u 1000, (1, 1, 1, 1, 1, 0, 1, 0, 0, 0), 0b0000010111u
#define PPUTLUTRAITS_0b1111100111u 999, (1, 1, 1, 1, 1, 0, 0, 1, 1, 1), 0b0000011000u
#define PPUTLUTRAITS_0b1111100110u 998, (1, 1, 1, 1, 1, 0, 0, 1, 1, 0), 0b0000011001u
#define PPUTLUTRAITS_0b1111100101u 997, (1, 1, 1, 1, 1, 0, 0, 1, 0, 1), 0b0000011010u
#define PPUTLUTRAITS_0b1111100100u 996, (1, 1, 1, 1, 1, 0, 0, 1, 0, 0), 0b0000011011u
#define PPUTLUTRAITS_0b1111100011u 995, (1, 1, 1, 1, 1, 0, 0, 0, 1, 1), 0b0000011100u
#define PPUTLUTRAITS_0b1111100010u 994, (1, 1, 1, 1, 1, 0, 0, 0, 1, 0), 0b0000011101u
#define PPUTLUTRAITS_0b1111100001u 993, (1, 1, 1, 1, 1, 0, 0, 0, 0, 1), 0b0000011110u
#define PPUTLUTRAITS_0b1111100000u 992, (1, 1, 1, 1, 1, 0, 0, 0, 0, 0), 0b0000011111u
#define PPUTLUTRAITS_0b1111011111u 991, (1, 1, 1, 1, 0, 1, 1, 1, 1, 1), 0b0000100000u
#define PPUTLUTRAITS_0b1111011110u 990, (1, 1, 1, 1, 0, 1, 1, 1, 1, 0), 0b0000100001u
#define PPUTLUTRAITS_0b1111011101u 989, (1, 1, 1, 1, 0, 1, 1, 1, 0, 1), 0b0000100010u
#define PPUTLUTRAITS_0b1111011100u 988, (1, 1, 1, 1, 0, 1, 1, 1, 0, 0), 0b0000100011u
#define PPUTLUTRAITS_0b1111011011u 987, (1, 1, 1, 1, 0, 1, 1, 0, 1, 1), 0b0000100100u
#define PPUTLUTRAITS_0b1111011010u 986, (1, 1, 1, 1, 0, 1, 1, 0, 1, 0), 0b0000100101u
#define PPUTLUTRAITS_0b1111011001u 985, (1, 1, 1, 1, 0, 1, 1, 0, 0, 1), 0b0000100110u
#define PPUTLUTRAITS_0b1111011000u 984, (1, 1, 1, 1, 0, 1, 1, 0, 0, 0), 0b0000100111u
#define PPUTLUTRAITS_0b1111010111u 983, (1, 1, 1, 1, 0, 1, 0, 1, 1, 1), 0b0000101000u
#define PPUTLUTRAITS_0b1111010110u 982, (1, 1, 1, 1, 0, 1, 0, 1, 1, 0), 0b0000101001u
#define PPUTLUTRAITS_0b1111010101u 981, (1, 1, 1, 1, 0, 1, 0, 1, 0, 1), 0b0000101010u
#define PPUTLUTRAITS_0b1111010100u 980, (1, 1, 1, 1, 0, 1, 0, 1, 0, 0), 0b0000101011u
#define PPUTLUTRAITS_0b1111010011u 979, (1, 1, 1, 1, 0, 1, 0, 0, 1, 1), 0b0000101100u
#define PPUTLUTRAITS_0b1111010010u 978, (1, 1, 1, 1, 0, 1, 0, 0, 1, 0), 0b0000101101u
#define PPUTLUTRAITS_0b1111010001u 977, (1, 1, 1, 1, 0, 1, 0, 0, 0, 1), 0b0000101110u
#define PPUTLUTRAITS_0b1111010000u 976, (1, 1, 1, 1, 0, 1, 0, 0, 0, 0), 0b0000101111u
#define PPUTLUTRAITS_0b1111001111u 975, (1, 1, 1, 1, 0, 0, 1, 1, 1, 1), 0b0000110000u
#define PPUTLUTRAITS_0b1111001110u 974, (1, 1, 1, 1, 0, 0, 1, 1, 1, 0), 0b0000110001u
#define PPUTLUTRAITS_0b1111001101u 973, (1, 1, 1, 1, 0, 0, 1, 1, 0, 1), 0b0000110010u
#define PPUTLUTRAITS_0b1111001100u 972, (1, 1, 1, 1, 0, 0, 1, 1, 0, 0), 0b0000110011u
#define PPUTLUTRAITS_0b1111001011u 971, (1, 1, 1, 1, 0, 0, 1, 0, 1, 1), 0b0000110100u
#define PPUTLUTRAITS_0b1111001010u 970, (1, 1, 1, 1, 0, 0, 1, 0, 1, 0), 0b0000110101u
#define PPUTLUTRAITS_0b1111001001u 969, (1, 1, 1, 1, 0, 0, 1, 0, 0, 1), 0b0000110110u
#define PPUTLUTRAITS_0b1111001000u 968, (1, 1, 1, 1, 0, 0, 1, 0, 0, 0), 0b0000110111u
#define PPUTLUTRAITS_0b1111000111u 967, (1, 1, 1, 1, 0, 0, 0, 1, 1, 1), 0b0000111000u
#define PPUTLUTRAITS_0b1111000110u 966, (1, 1, 1, 1, 0, 0, 0, 1, 1, 0), 0b0000111001u
#define PPUTLUTRAITS_0b1111000101u 965, (1, 1, 1, 1, 0, 0, 0, 1, 0, 1), 0b0000111010u
#define PPUTLUTRAITS_0b1111000100u 964, (1, 1, 1, 1, 0, 0, 0, 1, 0, 0), 0b0000111011u
#define PPUTLUTRAITS_0b1111000011u 963, (1, 1, 1, 1, 0, 0, 0, 0, 1, 1), 0b0000111100u
#define PPUTLUTRAITS_0b1111000010u 962, (1, 1, 1, 1, 0, 0, 0, 0, 1, 0), 0b0000111101u
#define PPUTLUTRAITS_0b1111000001u 961, (1, 1, 1, 1, 0, 0, 0, 0, 0, 1), 0b0000111110u
#define PPUTLUTRAITS_0b1111000000u 960, (1, 1, 1, 1, 0, 0, 0, 0, 0, 0), 0b0000111111u
#define PPUTLUTRAITS_0b1110111111u 959, (1, 1, 1, 0, 1, 1, 1, 1, 1, 1), 0b0001000000u
#define PPUTLUTRAITS_0b1110111110u 958, (1, 1, 1, 0, 1, 1, 1, 1, 1, 0), 0b0001000001u
#define PPUTLUTRAITS_0b1110111101u 957, (1, 1, 1, 0, 1, 1, 1, 1, 0, 1), 0b0001000010u
#define PPUTLUTRAITS_0b1110111100u 956, (1, 1, 1, 0, 1, 1, 1, 1, 0, 0), 0b0001000011u
#define PPUTLUTRAITS_0b1110111011u 955, (1, 1, 1, 0, 1, 1, 1, 0, 1, 1), 0b0001000100u
#define PPUTLUTRAITS_0b1110111010u 954, (1, 1, 1, 0, 1, 1, 1, 0, 1, 0), 0b0001000101u
#define PPUTLUTRAITS_0b1110111001u 953, (1, 1, 1, 0, 1, 1, 1, 0, 0, 1), 0b0001000110u
#define PPUTLUTRAITS_0b1110111000u 952, (1, 1, 1, 0, 1, 1, 1, 0, 0, 0), 0b0001000111u
#define PPUTLUTRAITS_0b1110110111u 951, (1, 1, 1, 0, 1, 1, 0, 1, 1, 1), 0b0001001000u
#define PPUTLUTRAITS_0b1110110110u 950, (1, 1, 1, 0, 1, 1, 0, 1, 1, 0), 0b0001001001u
#define PPUTLUTRAITS_0b1110110101u 949, (1, 1, 1, 0, 1, 1, 0, 1, 0, 1), 0b0001001010u
#define PPUTLUTRAITS_0b1110110100u 948, (1, 1, 1, 0, 1, 1, 0, 1, 0, 0), 0b0001001011u
#define PPUTLUTRAITS_0b1110110011u 947, (1, 1, 1, 0, 1, 1, 0, 0, 1, 1), 0b0001001100u
#define PPUTLUTRAITS_0b1110110010u 946, (1, 1, 1, 0, 1, 1, 0, 0, 1, 0), 0b0001001101u
#define PPUTLUTRAITS_0b1110110001u 945, (1, 1, 1, 0, 1, 1, 0, 0, 0, 1), 0b0001001110u
#define PPUTLUTRAITS_0b1110110000u 944, (1, 1, 1, 0, 1, 1, 0, 0, 0, 0), 0b0001001111u
#define PPUTLUTRAITS_0b1110101111u 943, (1, 1, 1, 0, 1, 0, 1, 1, 1, 1), 0b0001010000u
#define PPUTLUTRAITS_0b1110101110u 942, (1, 1, 1, 0, 1, 0, 1, 1, 1, 0), 0b0001010001u
#define PPUTLUTRAITS_0b1110101101u 941, (1, 1, 1, 0, 1, 0, 1, 1, 0, 1), 0b0001010010u
#define PPUTLUTRAITS_0b1110101100u 940, (1, 1, 1, 0, 1, 0, 1, 1, 0, 0), 0b0001010011u
#define PPUTLUTRAITS_0b1110101011u 939, (1, 1, 1, 0, 1, 0, 1, 0, 1, 1), 0b0001010100u
#define PPUTLUTRAITS_0b1110101010u 938, (1, 1, 1, 0, 1, 0, 1, 0, 1, 0), 0b0001010101u
#define PPUTLUTRAITS_0b1110101001u 937, (1, 1, 1, 0, 1, 0, 1, 0, 0, 1), 0b0001010110u
#define PPUTLUTRAITS_0b1110101000u 936, (1, 1, 1, 0, 1, 0, 1, 0, 0, 0), 0b0001010111u
#define PPUTLUTRAITS_0b1110100111u 935, (1, 1, 1, 0, 1, 0, 0, 1, 1, 1), 0b0001011000u
#define PPUTLUTRAITS_0b1110100110u 934, (1, 1, 1, 0, 1, 0, 0, 1, 1, 0), 0b0001011001u
#define PPUTLUTRAITS_0b1110100101u 933, (1, 1, 1, 0, 1, 0, 0, 1, 0, 1), 0b0001011010u
#define PPUTLUTRAITS_0b1110100100u 932, (1, 1, 1, 0, 1, 0, 0, 1, 0, 0), 0b0001011011u
#define PPUTLUTRAITS_0b1110100011u 931, (1, 1, 1, 0, 1, 0, 0, 0, 1, 1), 0b0001011100u
#define PPUTLUTRAITS_0b1110100010u 930, (1, 1, 1, 0, 1, 0, 0, 0, 1, 0), 0b0001011101u
#define PPUTLUTRAITS_0b1110100001u 929, (1, 1, 1, 0, 1, 0, 0, 0, 0, 1), 0b0001011110u
#define PPUTLUTRAITS_0b1110100000u 928, (1, 1, 1, 0, 1, 0, 0, 0, 0, 0), 0b0001011111u
#define PPUTLUTRAITS_0b1110011111u 927, (1, 1, 1, 0, 0, 1, 1, 1, 1, 1), 0b0001100000u
#define PPUTLUTRAITS_0b1110011110u 926, (1, 1, 1, 0, 0, 1, 1, 1, 1, 0), 0b0001100001u
#define PPUTLUTRAITS_0b1110011101u 925, (1, 1, 1, 0, 0, 1, 1, 1, 0, 1), 0b0001100010u
#define PPUTLUTRAITS_0b1110011100u 924, (1, 1, 1, 0, 0, 1, 1, 1, 0, 0), 0b0001100011u
#define PPUTLUTRAITS_0b1110011011u 923, (1, 1, 1, 0, 0, 1, 1, 0, 1, 1), 0b0001100100u
#define PPUTLUTRAITS_0b1110011010u 922, (1, 1, 1, 0, 0, 1, 1, 0, 1, 0), 0b0001100101u
#define PPUTLUTRAITS_0b1110011001u 921, (1, 1, 1, 0, 0, 1, 1, 0, 0, 1), 0b0001100110u
#define PPUTLUTRAITS_0b1110011000u 920, (1, 1, 1, 0, 0, 1, 1, 0, 0, 0), 0b0001100111u
#define PPUTLUTRAITS_0b1110010111u 919, (1, 1, 1, 0, 0, 1, 0, 1, 1, 1), 0b0001101000u
#define PPUTLUTRAITS_0b1110010110u 918, (1, 1, 1, 0, 0, 1, 0, 1, 1, 0), 0b0001101001u
#define PPUTLUTRAITS_0b1110010101u 917, (1, 1, 1, 0, 0, 1, 0, 1, 0, 1), 0b0001101010u
#define PPUTLUTRAITS_0b1110010100u 916, (1, 1, 1, 0, 0, 1, 0, 1, 0, 0), 0b0001101011u
#define PPUTLUTRAITS_0b1110010011u 915, (1, 1, 1, 0, 0, 1, 0, 0, 1, 1), 0b0001101100u
#define PPUTLUTRAITS_0b1110010010u 914, (1, 1, 1, 0, 0, 1, 0, 0, 1, 0), 0b0001101101u
#define PPUTLUTRAITS_0b1110010001u 913, (1, 1, 1, 0, 0, 1, 0, 0, 0, 1), 0b0001101110u
#define PPUTLUTRAITS_0b1110010000u 912, (1, 1, 1, 0, 0, 1, 0, 0, 0, 0), 0b0001101111u
#define PPUTLUTRAITS_0b1110001111u 911, (1, 1, 1, 0, 0, 0, 1, 1, 1, 1), 0b0001110000u
#define PPUTLUTRAITS_0b1110001110u 910, (1, 1, 1, 0, 0, 0, 1, 1, 1, 0), 0b0001110001u
#define PPUTLUTRAITS_0b1110001101u 909, (1, 1, 1, 0, 0, 0, 1, 1, 0, 1), 0b0001110010u
#define PPUTLUTRAITS_0b1110001100u 908, (1, 1, 1, 0, 0, 0, 1, 1, 0, 0), 0b0001110011u
#define PPUTLUTRAITS_0b1110001011u 907, (1, 1, 1, 0, 0, 0, 1, 0, 1, 1), 0b0001110100u
#define PPUTLUTRAITS_0b1110001010u 906, (1, 1, 1, 0, 0, 0, 1, 0, 1, 0), 0b0001110101u
#define PPUTLUTRAITS_0b1110001001u 905, (1, 1, 1, 0, 0, 0, 1, 0, 0, 1), 0b0001110110u
#define PPUTLUTRAITS_0b1110001000u 904, (1, 1, 1, 0, 0, 0, 1, 0, 0, 0), 0b0001110111u
#define PPUTLUTRAITS_0b1110000111u 903, (1, 1, 1, 0, 0, 0, 0, 1, 1, 1), 0b0001111000u
#define PPUTLUTRAITS_0b1110000110u 902, (1, 1, 1, 0, 0, 0, 0, 1, 1, 0), 0b0001111001u
#define PPUTLUTRAITS_0b1110000101u 901, (1, 1, 1, 0, 0, 0, 0, 1, 0, 1), 0b0001111010u
#define PPUTLUTRAITS_0b1110000100u 900, (1, 1, 1, 0, 0, 0, 0, 1, 0, 0), 0b0001111011u
#define PPUTLUTRAITS_0b1110000011u 899, (1, 1, 1, 0, 0, 0, 0, 0, 1, 1), 0b0001111100u
#define PPUTLUTRAITS_0b1110000010u 898, (1, 1, 1, 0, 0, 0, 0, 0, 1, 0), 0b0001111101u
#define PPUTLUTRAITS_0b1110000001u 897, (1, 1, 1, 0, 0, 0, 0, 0, 0, 1), 0b0001111110u
#define PPUTLUTRAITS_0b1110000000u 896, (1, 1, 1, 0, 0, 0, 0, 0, 0, 0), 0b0001111111u
#define PPUTLUTRAITS_0b1101111111u 895, (1, 1, 0, 1, 1, 1, 1, 1, 1, 1), 0b0010000000u
#define PPUTLUTRAITS_0b1101111110u 894, (1, 1, 0, 1, 1, 1, 1, 1, 1, 0), 0b0010000001u
#define PPUTLUTRAITS_0b1101111101u 893, (1, 1, 0, 1, 1, 1, 1, 1, 0, 1), 0b0010000010u
#define PPUTLUTRAITS_0b1101111100u 892, (1, 1, 0, 1, 1, 1, 1, 1, 0, 0), 0b0010000011u
#define PPUTLUTRAITS_0b1101111011u 891, (1, 1, 0, 1, 1, 1, 1, 0, 1, 1), 0b0010000100u
#define PPUTLUTRAITS_0b1101111010u 890, (1, 1, 0, 1, 1, 1, 1, 0, 1, 0), 0b0010000101u
#define PPUTLUTRAITS_0b1101111001u 889, (1, 1, 0, 1, 1, 1, 1, 0, 0, 1), 0b0010000110u
#define PPUTLUTRAITS_0b1101111000u 888, (1, 1, 0, 1, 1, 1, 1, 0, 0, 0), 0b0010000111u
#define PPUTLUTRAITS_0b1101110111u 887, (1, 1, 0, 1, 1, 1, 0, 1, 1, 1), 0b0010001000u
#define PPUTLUTRAITS_0b1101110110u 886, (1, 1, 0, 1, 1, 1, 0, 1, 1, 0), 0b0010001001u
#define PPUTLUTRAITS_0b1101110101u 885, (1, 1, 0, 1, 1, 1, 0, 1, 0, 1), 0b0010001010u
#define PPUTLUTRAITS_0b1101110100u 884, (1, 1, 0, 1, 1, 1, 0, 1, 0, 0), 0b0010001011u
#define PPUTLUTRAITS_0b1101110011u 883, (1, 1, 0, 1, 1, 1, 0, 0, 1, 1), 0b0010001100u
#define PPUTLUTRAITS_0b1101110010u 882, (1, 1, 0, 1, 1, 1, 0, 0, 1, 0), 0b0010001101u
#define PPUTLUTRAITS_0b1101110001u 881, (1, 1, 0, 1, 1, 1, 0, 0, 0, 1), 0b0010001110u
#define PPUTLUTRAITS_0b1101110000u 880, (1, 1, 0, 1, 1, 1, 0, 0, 0, 0), 0b0010001111u
#define PPUTLUTRAITS_0b1101101111u 879, (1, 1, 0, 1, 1, 0, 1, 1, 1, 1), 0b0010010000u
#define PPUTLUTRAITS_0b1101101110u 878, (1, 1, 0, 1, 1, 0, 1, 1, 1, 0), 0b0010010001u
#define PPUTLUTRAITS_0b1101101101u 877, (1, 1, 0, 1, 1, 0, 1, 1, 0, 1), 0b0010010010u
#define PPUTLUTRAITS_0b1101101100u 876, (1, 1, 0, 1, 1, 0, 1, 1, 0, 0), 0b0010010011u
#define PPUTLUTRAITS_0b1101101011u 875, (1, 1, 0, 1, 1, 0, 1, 0, 1, 1), 0b0010010100u
#define PPUTLUTRAITS_0b1101101010u 874, (1, 1, 0, 1, 1, 0, 1, 0, 1, 0), 0b0010010101u
#define PPUTLUTRAITS_0b1101101001u 873, (1, 1, 0, 1, 1, 0, 1, 0, 0, 1), 0b0010010110u
#define PPUTLUTRAITS_0b1101101000u 872, (1, 1, 0, 1, 1, 0, 1, 0, 0, 0), 0b0010010111u
#define PPUTLUTRAITS_0b1101100111u 871, (1, 1, 0, 1, 1, 0, 0, 1, 1, 1), 0b0010011000u
#define PPUTLUTRAITS_0b1101100110u 870, (1, 1, 0, 1, 1, 0, 0, 1, 1, 0), 0b0010011001u
#define PPUTLUTRAITS_0b1101100101u 869, (1, 1, 0, 1, 1, 0, 0, 1, 0, 1), 0b0010011010u
#define PPUTLUTRAITS_0b1101100100u 868, (1, 1, 0, 1, 1, 0, 0, 1, 0, 0), 0b0010011011u
#define PPUTLUTRAITS_0b1101100011u 867, (1, 1, 0, 1, 1, 0, 0, 0, 1, 1), 0b0010011100u
#define PPUTLUTRAITS_0b1101100010u 866, (1, 1, 0, 1, 1, 0, 0, 0, 1, 0), 0b0010011101u
#define PPUTLUTRAITS_0b1101100001u 865, (1, 1, 0, 1, 1, 0, 0, 0, 0, 1), 0b0010011110u
#define PPUTLUTRAITS_0b1101100000u 864, (1, 1, 0, 1, 1, 0, 0, 0, 0, 0), 0b0010011111u
#define PPUTLUTRAITS_0b1101011111u 863, (1, 1, 0, 1, 0, 1, 1, 1, 1, 1), 0b0010100000u
#define PPUTLUTRAITS_0b1101011110u 862, (1, 1, 0, 1, 0, 1, 1, 1, 1, 0), 0b0010100001u
#define PPUTLUTRAITS_0b1101011101u 861, (1, 1, 0, 1, 0, 1, 1, 1, 0, 1), 0b0010100010u
#define PPUTLUTRAITS_0b1101011100u 860, (1, 1, 0, 1, 0, 1, 1, 1, 0, 0), 0b0010100011u
#define PPUTLUTRAITS_0b1101011011u 859, (1, 1, 0, 1, 0, 1, 1, 0, 1, 1), 0b0010100100u
#define PPUTLUTRAITS_0b1101011010u 858, (1, 1, 0, 1, 0, 1, 1, 0, 1, 0), 0b0010100101u
#define PPUTLUTRAITS_0b1101011001u 857, (1, 1, 0, 1, 0, 1, 1, 0, 0, 1), 0b0010100110u
#define PPUTLUTRAITS_0b1101011000u 856, (1, 1, 0, 1, 0, 1, 1, 0, 0, 0), 0b0010100111u
#define PPUTLUTRAITS_0b1101010111u 855, (1, 1, 0, 1, 0, 1, 0, 1, 1, 1), 0b0010101000u
#define PPUTLUTRAITS_0b1101010110u 854, (1, 1, 0, 1, 0, 1, 0, 1, 1, 0), 0b0010101001u
#define PPUTLUTRAITS_0b1101010101u 853, (1, 1, 0, 1, 0, 1, 0, 1, 0, 1), 0b0010101010u
#define PPUTLUTRAITS_0b1101010100u 852, (1, 1, 0, 1, 0, 1, 0, 1, 0, 0), 0b0010101011u
#define PPUTLUTRAITS_0b1101010011u 851, (1, 1, 0, 1, 0, 1, 0, 0, 1, 1), 0b0010101100u
#define PPUTLUTRAITS_0b1101010010u 850, (1, 1, 0, 1, 0, 1, 0, 0, 1, 0), 0b0010101101u
#define PPUTLUTRAITS_0b1101010001u 849, (1, 1, 0, 1, 0, 1, 0, 0, 0, 1), 0b0010101110u
#define PPUTLUTRAITS_0b1101010000u 848, (1, 1, 0, 1, 0, 1, 0, 0, 0, 0), 0b0010101111u
#define PPUTLUTRAITS_0b1101001111u 847, (1, 1, 0, 1, 0, 0, 1, 1, 1, 1), 0b0010110000u
#define PPUTLUTRAITS_0b1101001110u 846, (1, 1, 0, 1, 0, 0, 1, 1, 1, 0), 0b0010110001u
#define PPUTLUTRAITS_0b1101001101u 845, (1, 1, 0, 1, 0, 0, 1, 1, 0, 1), 0b0010110010u
#define PPUTLUTRAITS_0b1101001100u 844, (1, 1, 0, 1, 0, 0, 1, 1, 0, 0), 0b0010110011u
#define PPUTLUTRAITS_0b1101001011u 843, (1, 1, 0, 1, 0, 0, 1, 0, 1, 1), 0b0010110100u
#define PPUTLUTRAITS_0b1101001010u 842, (1, 1, 0, 1, 0, 0, 1, 0, 1, 0), 0b0010110101u
#define PPUTLUTRAITS_0b1101001001u 841, (1, 1, 0, 1, 0, 0, 1, 0, 0, 1), 0b0010110110u
#define PPUTLUTRAITS_0b1101001000u 840, (1, 1, 0, 1, 0, 0, 1, 0, 0, 0), 0b0010110111u
#define PPUTLUTRAITS_0b1101000111u 839, (1, 1, 0, 1, 0, 0, 0, 1, 1, 1), 0b0010111000u
#define PPUTLUTRAITS_0b1101000110u 838, (1, 1, 0, 1, 0, 0, 0, 1, 1, 0), 0b0010111001u
#define PPUTLUTRAITS_0b1101000101u 837, (1, 1, 0, 1, 0, 0, 0, 1, 0, 1), 0b0010111010u
#define PPUTLUTRAITS_0b1101000100u 836, (1, 1, 0, 1, 0, 0, 0, 1, 0, 0), 0b0010111011u
#define PPUTLUTRAITS_0b1101000011u 835, (1, 1, 0, 1, 0, 0, 0, 0, 1, 1), 0b0010111100u
#define PPUTLUTRAITS_0b1101000010u 834, (1, 1, 0, 1, 0, 0, 0, 0, 1, 0), 0b0010111101u
#define PPUTLUTRAITS_0b1101000001u 833, (1, 1, 0, 1, 0, 0, 0, 0, 0, 1), 0b0010111110u
#define PPUTLUTRAITS_0b1101000000u 832, (1, 1, 0, 1, 0, 0, 0, 0, 0, 0), 0b0010111111u
#define PPUTLUTRAITS_0b1100111111u 831, (1, 1, 0, 0, 1, 1, 1, 1, 1, 1), 0b0011000000u
#define PPUTLUTRAITS_0b1100111110u 830, (1, 1, 0, 0, 1, 1, 1, 1, 1, 0), 0b0011000001u
#define PPUTLUTRAITS_0b1100111101u 829, (1, 1, 0, 0, 1, 1, 1, 1, 0, 1), 0b0011000010u
#define PPUTLUTRAITS_0b1100111100u 828, (1, 1, 0, 0, 1, 1, 1, 1, 0, 0), 0b0011000011u
#define PPUTLUTRAITS_0b1100111011u 827, (1, 1, 0, 0, 1, 1, 1, 0, 1, 1), 0b0011000100u
#define PPUTLUTRAITS_0b1100111010u 826, (1, 1, 0, 0, 1, 1, 1, 0, 1, 0), 0b0011000101u
#define PPUTLUTRAITS_0b1100111001u 825, (1, 1, 0, 0, 1, 1, 1, 0, 0, 1), 0b0011000110u
#define PPUTLUTRAITS_0b1100111000u 824, (1, 1, 0, 0, 1, 1, 1, 0, 0, 0), 0b0011000111u
#define PPUTLUTRAITS_0b1100110111u 823, (1, 1, 0, 0, 1, 1, 0, 1, 1, 1), 0b0011001000u
#define PPUTLUTRAITS_0b1100110110u 822, (1, 1, 0, 0, 1, 1, 0, 1, 1, 0), 0b0011001001u
#define PPUTLUTRAITS_0b1100110101u 821, (1, 1, 0, 0, 1, 1, 0, 1, 0, 1), 0b0011001010u
#define PPUTLUTRAITS_0b1100110100u 820, (1, 1, 0, 0, 1, 1, 0, 1, 0, 0), 0b0011001011u
#define PPUTLUTRAITS_0b1100110011u 819, (1, 1, 0, 0, 1, 1, 0, 0, 1, 1), 0b0011001100u
#define PPUTLUTRAITS_0b1100110010u 818, (1, 1, 0, 0, 1, 1, 0, 0, 1, 0), 0b0011001101u
#define PPUTLUTRAITS_0b1100110001u 817, (1, 1, 0, 0, 1, 1, 0, 0, 0, 1), 0b0011001110u
#define PPUTLUTRAITS_0b1100110000u 816, (1, 1, 0, 0, 1, 1, 0, 0, 0, 0), 0b0011001111u
#define PPUTLUTRAITS_0b1100101111u 815, (1, 1, 0, 0, 1, 0, 1, 1, 1, 1), 0b0011010000u
#define PPUTLUTRAITS_0b1100101110u 814, (1, 1, 0, 0, 1, 0, 1, 1, 1, 0), 0b0011010001u
#define PPUTLUTRAITS_0b1100101101u 813, (1, 1, 0, 0, 1, 0, 1, 1, 0, 1), 0b0011010010u
#define PPUTLUTRAITS_0b1100101100u 812, (1, 1, 0, 0, 1, 0, 1, 1, 0, 0), 0b0011010011u
#define PPUTLUTRAITS_0b1100101011u 811, (1, 1, 0, 0, 1, 0, 1, 0, 1, 1), 0b0011010100u
#define PPUTLUTRAITS_0b1100101010u 810, (1, 1, 0, 0, 1, 0, 1, 0, 1, 0), 0b0011010101u
#define PPUTLUTRAITS_0b1100101001u 809, (1, 1, 0, 0, 1, 0, 1, 0, 0, 1), 0b0011010110u
#define PPUTLUTRAITS_0b1100101000u 808, (1, 1, 0, 0, 1, 0, 1, 0, 0, 0), 0b0011010111u
#define PPUTLUTRAITS_0b1100100111u 807, (1, 1, 0, 0, 1, 0, 0, 1, 1, 1), 0b0011011000u
#define PPUTLUTRAITS_0b1100100110u 806, (1, 1, 0, 0, 1, 0, 0, 1, 1, 0), 0b0011011001u
#define PPUTLUTRAITS_0b1100100101u 805, (1, 1, 0, 0, 1, 0, 0, 1, 0, 1), 0b0011011010u
#define PPUTLUTRAITS_0b1100100100u 804, (1, 1, 0, 0, 1, 0, 0, 1, 0, 0), 0b0011011011u
#define PPUTLUTRAITS_0b1100100011u 803, (1, 1, 0, 0, 1, 0, 0, 0, 1, 1), 0b0011011100u
#define PPUTLUTRAITS_0b1100100010u 802, (1, 1, 0, 0, 1, 0, 0, 0, 1, 0), 0b0011011101u
#define PPUTLUTRAITS_0b1100100001u 801, (1, 1, 0, 0, 1, 0, 0, 0, 0, 1), 0b0011011110u
#define PPUTLUTRAITS_0b1100100000u 800, (1, 1, 0, 0, 1, 0, 0, 0, 0, 0), 0b0011011111u
#define PPUTLUTRAITS_0b1100011111u 799, (1, 1, 0, 0, 0, 1, 1, 1, 1, 1), 0b0011100000u
#define PPUTLUTRAITS_0b1100011110u 798, (1, 1, 0, 0, 0, 1, 1, 1, 1, 0), 0b0011100001u
#define PPUTLUTRAITS_0b1100011101u 797, (1, 1, 0, 0, 0, 1, 1, 1, 0, 1), 0b0011100010u
#define PPUTLUTRAITS_0b1100011100u 796, (1, 1, 0, 0, 0, 1, 1, 1, 0, 0), 0b0011100011u
#define PPUTLUTRAITS_0b1100011011u 795, (1, 1, 0, 0, 0, 1, 1, 0, 1, 1), 0b0011100100u
#define PPUTLUTRAITS_0b1100011010u 794, (1, 1, 0, 0, 0, 1, 1, 0, 1, 0), 0b0011100101u
#define PPUTLUTRAITS_0b1100011001u 793, (1, 1, 0, 0, 0, 1, 1, 0, 0, 1), 0b0011100110u
#define PPUTLUTRAITS_0b1100011000u 792, (1, 1, 0, 0, 0, 1, 1, 0, 0, 0), 0b0011100111u
#define PPUTLUTRAITS_0b1100010111u 791, (1, 1, 0, 0, 0, 1, 0, 1, 1, 1), 0b0011101000u
#define PPUTLUTRAITS_0b1100010110u 790, (1, 1, 0, 0, 0, 1, 0, 1, 1, 0), 0b0011101001u
#define PPUTLUTRAITS_0b1100010101u 789, (1, 1, 0, 0, 0, 1, 0, 1, 0, 1), 0b0011101010u
#define PPUTLUTRAITS_0b1100010100u 788, (1, 1, 0, 0, 0, 1, 0, 1, 0, 0), 0b0011101011u
#define PPUTLUTRAITS_0b1100010011u 787, (1, 1, 0, 0, 0, 1, 0, 0, 1, 1), 0b0011101100u
#define PPUTLUTRAITS_0b1100010010u 786, (1, 1, 0, 0, 0, 1, 0, 0, 1, 0), 0b0011101101u
#define PPUTLUTRAITS_0b1100010001u 785, (1, 1, 0, 0, 0, 1, 0, 0, 0, 1), 0b0011101110u
#define PPUTLUTRAITS_0b1100010000u 784, (1, 1, 0, 0, 0, 1, 0, 0, 0, 0), 0b0011101111u
#define PPUTLUTRAITS_0b1100001111u 783, (1, 1, 0, 0, 0, 0, 1, 1, 1, 1), 0b0011110000u
#define PPUTLUTRAITS_0b1100001110u 782, (1, 1, 0, 0, 0, 0, 1, 1, 1, 0), 0b0011110001u
#define PPUTLUTRAITS_0b1100001101u 781, (1, 1, 0, 0, 0, 0, 1, 1, 0, 1), 0b0011110010u
#define PPUTLUTRAITS_0b1100001100u 780, (1, 1, 0, 0, 0, 0, 1, 1, 0, 0), 0b0011110011u
#define PPUTLUTRAITS_0b1100001011u 779, (1, 1, 0, 0, 0, 0, 1, 0, 1, 1), 0b0011110100u
#define PPUTLUTRAITS_0b1100001010u 778, (1, 1, 0, 0, 0, 0, 1, 0, 1, 0), 0b0011110101u
#define PPUTLUTRAITS_0b1100001001u 777, (1, 1, 0, 0, 0, 0, 1, 0, 0, 1), 0b0011110110u
#define PPUTLUTRAITS_0b1100001000u 776, (1, 1, 0, 0, 0, 0, 1, 0, 0, 0), 0b0011110111u
#define PPUTLUTRAITS_0b1100000111u 775, (1, 1, 0, 0, 0, 0, 0, 1, 1, 1), 0b0011111000u
#define PPUTLUTRAITS_0b1100000110u 774, (1, 1, 0, 0, 0, 0, 0, 1, 1, 0), 0b0011111001u
#define PPUTLUTRAITS_0b1100000101u 773, (1, 1, 0, 0, 0, 0, 0, 1, 0, 1), 0b0011111010u
#define PPUTLUTRAITS_0b1100000100u 772, (1, 1, 0, 0, 0, 0, 0, 1, 0, 0), 0b0011111011u
#define PPUTLUTRAITS_0b1100000011u 771, (1, 1, 0, 0, 0, 0, 0, 0, 1, 1), 0b0011111100u
#define PPUTLUTRAITS_0b1100000010u 770, (1, 1, 0, 0, 0, 0, 0, 0, 1, 0), 0b0011111101u
#define PPUTLUTRAITS_0b1100000001u 769, (1, 1, 0, 0, 0, 0, 0, 0, 0, 1), 0b0011111110u
#define PPUTLUTRAITS_0b1100000000u 768, (1, 1, 0, 0, 0, 0, 0, 0, 0, 0), 0b0011111111u
#define PPUTLUTRAITS_0b1011111111u 767, (1, 0, 1, 1, 1, 1, 1, 1, 1, 1), 0b0100000000u
#define PPUTLUTRAITS_0b1011111110u 766, (1, 0, 1, 1, 1, 1, 1, 1, 1, 0), 0b0100000001u
#define PPUTLUTRAITS_0b1011111101u 765, (1, 0, 1, 1, 1, 1, 1, 1, 0, 1), 0b0100000010u
#define PPUTLUTRAITS_0b1011111100u 764, (1, 0, 1, 1, 1, 1, 1, 1, 0, 0), 0b0100000011u
#define PPUTLUTRAITS_0b1011111011u 763, (1, 0, 1, 1, 1, 1, 1, 0, 1, 1), 0b0100000100u
#define PPUTLUTRAITS_0b1011111010u 762, (1, 0, 1, 1, 1, 1, 1, 0, 1, 0), 0b0100000101u
#define PPUTLUTRAITS_0b1011111001u 761, (1, 0, 1, 1, 1, 1, 1, 0, 0, 1), 0b0100000110u
#define PPUTLUTRAITS_0b1011111000u 760, (1, 0, 1, 1, 1, 1, 1, 0, 0, 0), 0b0100000111u
#define PPUTLUTRAITS_0b1011110111u 759, (1, 0, 1, 1, 1, 1, 0, 1, 1, 1), 0b0100001000u
#define PPUTLUTRAITS_0b1011110110u 758, (1, 0, 1, 1, 1, 1, 0, 1, 1, 0), 0b0100001001u
#define PPUTLUTRAITS_0b1011110101u 757, (1, 0, 1, 1, 1, 1, 0, 1, 0, 1), 0b0100001010u
#define PPUTLUTRAITS_0b1011110100u 756, (1, 0, 1, 1, 1, 1, 0, 1, 0, 0), 0b0100001011u
#define PPUTLUTRAITS_0b1011110011u 755, (1, 0, 1, 1, 1, 1, 0, 0, 1, 1), 0b0100001100u
#define PPUTLUTRAITS_0b1011110010u 754, (1, 0, 1, 1, 1, 1, 0, 0, 1, 0), 0b0100001101u
#define PPUTLUTRAITS_0b1011110001u 753, (1, 0, 1, 1, 1, 1, 0, 0, 0, 1), 0b0100001110u
#define PPUTLUTRAITS_0b1011110000u 752, (1, 0, 1, 1, 1, 1, 0, 0, 0, 0), 0b0100001111u
#define PPUTLUTRAITS_0b1011101111u 751, (1, 0, 1, 1, 1, 0, 1, 1, 1, 1), 0b0100010000u
#define PPUTLUTRAITS_0b1011101110u 750, (1, 0, 1, 1, 1, 0, 1, 1, 1, 0), 0b0100010001u
#define PPUTLUTRAITS_0b1011101101u 749, (1, 0, 1, 1, 1, 0, 1, 1, 0, 1), 0b0100010010u
#define PPUTLUTRAITS_0b1011101100u 748, (1, 0, 1, 1, 1, 0, 1, 1, 0, 0), 0b0100010011u
#define PPUTLUTRAITS_0b1011101011u 747, (1, 0, 1, 1, 1, 0, 1, 0, 1, 1), 0b0100010100u
#define PPUTLUTRAITS_0b1011101010u 746, (1, 0, 1, 1, 1, 0, 1, 0, 1, 0), 0b0100010101u
#define PPUTLUTRAITS_0b1011101001u 745, (1, 0, 1, 1, 1, 0, 1, 0, 0, 1), 0b0100010110u
#define PPUTLUTRAITS_0b1011101000u 744, (1, 0, 1, 1, 1, 0, 1, 0, 0, 0), 0b0100010111u
#define PPUTLUTRAITS_0b1011100111u 743, (1, 0, 1, 1, 1, 0, 0, 1, 1, 1), 0b0100011000u
#define PPUTLUTRAITS_0b1011100110u 742, (1, 0, 1, 1, 1, 0, 0, 1, 1, 0), 0b0100011001u
#define PPUTLUTRAITS_0b1011100101u 741, (1, 0, 1, 1, 1, 0, 0, 1, 0, 1), 0b0100011010u
#define PPUTLUTRAITS_0b1011100100u 740, (1, 0, 1, 1, 1, 0, 0, 1, 0, 0), 0b0100011011u
#define PPUTLUTRAITS_0b1011100011u 739, (1, 0, 1, 1, 1, 0, 0, 0, 1, 1), 0b0100011100u
#define PPUTLUTRAITS_0b1011100010u 738, (1, 0, 1, 1, 1, 0, 0, 0, 1, 0), 0b0100011101u
#define PPUTLUTRAITS_0b1011100001u 737, (1, 0, 1, 1, 1, 0, 0, 0, 0, 1), 0b0100011110u
#define PPUTLUTRAITS_0b1011100000u 736, (1, 0, 1, 1, 1, 0, 0, 0, 0, 0), 0b0100011111u
#define PPUTLUTRAITS_0b1011011111u 735, (1, 0, 1, 1, 0, 1, 1, 1, 1, 1), 0b0100100000u
#define PPUTLUTRAITS_0b1011011110u 734, (1, 0, 1, 1, 0, 1, 1, 1, 1, 0), 0b0100100001u
#define PPUTLUTRAITS_0b1011011101u 733, (1, 0, 1, 1, 0, 1, 1, 1, 0, 1), 0b0100100010u
#define PPUTLUTRAITS_0b1011011100u 732, (1, 0, 1, 1, 0, 1, 1, 1, 0, 0), 0b0100100011u
#define PPUTLUTRAITS_0b1011011011u 731, (1, 0, 1, 1, 0, 1, 1, 0, 1, 1), 0b0100100100u
#define PPUTLUTRAITS_0b1011011010u 730, (1, 0, 1, 1, 0, 1, 1, 0, 1, 0), 0b0100100101u
#define PPUTLUTRAITS_0b1011011001u 729, (1, 0, 1, 1, 0, 1, 1, 0, 0, 1), 0b0100100110u
#define PPUTLUTRAITS_0b1011011000u 728, (1, 0, 1, 1, 0, 1, 1, 0, 0, 0), 0b0100100111u
#define PPUTLUTRAITS_0b1011010111u 727, (1, 0, 1, 1, 0, 1, 0, 1, 1, 1), 0b0100101000u
#define PPUTLUTRAITS_0b1011010110u 726, (1, 0, 1, 1, 0, 1, 0, 1, 1, 0), 0b0100101001u
#define PPUTLUTRAITS_0b1011010101u 725, (1, 0, 1, 1, 0, 1, 0, 1, 0, 1), 0b0100101010u
#define PPUTLUTRAITS_0b1011010100u 724, (1, 0, 1, 1, 0, 1, 0, 1, 0, 0), 0b0100101011u
#define PPUTLUTRAITS_0b1011010011u 723, (1, 0, 1, 1, 0, 1, 0, 0, 1, 1), 0b0100101100u
#define PPUTLUTRAITS_0b1011010010u 722, (1, 0, 1, 1, 0, 1, 0, 0, 1, 0), 0b0100101101u
#define PPUTLUTRAITS_0b1011010001u 721, (1, 0, 1, 1, 0, 1, 0, 0, 0, 1), 0b0100101110u
#define PPUTLUTRAITS_0b1011010000u 720, (1, 0, 1, 1, 0, 1, 0, 0, 0, 0), 0b0100101111u
#define PPUTLUTRAITS_0b1011001111u 719, (1, 0, 1, 1, 0, 0, 1, 1, 1, 1), 0b0100110000u
#define PPUTLUTRAITS_0b1011001110u 718, (1, 0, 1, 1, 0, 0, 1, 1, 1, 0), 0b0100110001u
#define PPUTLUTRAITS_0b1011001101u 717, (1, 0, 1, 1, 0, 0, 1, 1, 0, 1), 0b0100110010u
#define PPUTLUTRAITS_0b1011001100u 716, (1, 0, 1, 1, 0, 0, 1, 1, 0, 0), 0b0100110011u
#define PPUTLUTRAITS_0b1011001011u 715, (1, 0, 1, 1, 0, 0, 1, 0, 1, 1), 0b0100110100u
#define PPUTLUTRAITS_0b1011001010u 714, (1, 0, 1, 1, 0, 0, 1, 0, 1, 0), 0b0100110101u
#define PPUTLUTRAITS_0b1011001001u 713, (1, 0, 1, 1, 0, 0, 1, 0, 0, 1), 0b0100110110u
#define PPUTLUTRAITS_0b1011001000u 712, (1, 0, 1, 1, 0, 0, 1, 0, 0, 0), 0b0100110111u
#define PPUTLUTRAITS_0b1011000111u 711, (1, 0, 1, 1, 0, 0, 0, 1, 1, 1), 0b0100111000u
#define PPUTLUTRAITS_0b1011000110u 710, (1, 0, 1, 1, 0, 0, 0, 1, 1, 0), 0b0100111001u
#define PPUTLUTRAITS_0b1011000101u 709, (1, 0, 1, 1, 0, 0, 0, 1, 0, 1), 0b0100111010u
#define PPUTLUTRAITS_0b1011000100u 708, (1, 0, 1, 1, 0, 0, 0, 1, 0, 0), 0b0100111011u
#define PPUTLUTRAITS_0b1011000011u 707, (1, 0, 1, 1, 0, 0, 0, 0, 1, 1), 0b0100111100u
#define PPUTLUTRAITS_0b1011000010u 706, (1, 0, 1, 1, 0, 0, 0, 0, 1, 0), 0b0100111101u
#define PPUTLUTRAITS_0b1011000001u 705, (1, 0, 1, 1, 0, 0, 0, 0, 0, 1), 0b0100111110u
#define PPUTLUTRAITS_0b1011000000u 704, (1, 0, 1, 1, 0, 0, 0, 0, 0, 0), 0b0100111111u
#define PPUTLUTRAITS_0b1010111111u 703, (1, 0, 1, 0, 1, 1, 1, 1, 1, 1), 0b0101000000u
#define PPUTLUTRAITS_0b1010111110u 702, (1, 0, 1, 0, 1, 1, 1, 1, 1, 0), 0b0101000001u
#define PPUTLUTRAITS_0b1010111101u 701, (1, 0, 1, 0, 1, 1, 1, 1, 0, 1), 0b0101000010u
#define PPUTLUTRAITS_0b1010111100u 700, (1, 0, 1, 0, 1, 1, 1, 1, 0, 0), 0b0101000011u
#define PPUTLUTRAITS_0b1010111011u 699, (1, 0, 1, 0, 1, 1, 1, 0, 1, 1), 0b0101000100u
#define PPUTLUTRAITS_0b1010111010u 698, (1, 0, 1, 0, 1, 1, 1, 0, 1, 0), 0b0101000101u
#define PPUTLUTRAITS_0b1010111001u 697, (1, 0, 1, 0, 1, 1, 1, 0, 0, 1), 0b0101000110u
#define PPUTLUTRAITS_0b1010111000u 696, (1, 0, 1, 0, 1, 1, 1, 0, 0, 0), 0b0101000111u
#define PPUTLUTRAITS_0b1010110111u 695, (1, 0, 1, 0, 1, 1, 0, 1, 1, 1), 0b0101001000u
#define PPUTLUTRAITS_0b1010110110u 694, (1, 0, 1, 0, 1, 1, 0, 1, 1, 0), 0b0101001001u
#define PPUTLUTRAITS_0b1010110101u 693, (1, 0, 1, 0, 1, 1, 0, 1, 0, 1), 0b0101001010u
#define PPUTLUTRAITS_0b1010110100u 692, (1, 0, 1, 0, 1, 1, 0, 1, 0, 0), 0b0101001011u
#define PPUTLUTRAITS_0b1010110011u 691, (1, 0, 1, 0, 1, 1, 0, 0, 1, 1), 0b0101001100u
#define PPUTLUTRAITS_0b1010110010u 690, (1, 0, 1, 0, 1, 1, 0, 0, 1, 0), 0b0101001101u
#define PPUTLUTRAITS_0b1010110001u 689, (1, 0, 1, 0, 1, 1, 0, 0, 0, 1), 0b0101001110u
#define PPUTLUTRAITS_0b1010110000u 688, (1, 0, 1, 0, 1, 1, 0, 0, 0, 0), 0b0101001111u
#define PPUTLUTRAITS_0b1010101111u 687, (1, 0, 1, 0, 1, 0, 1, 1, 1, 1), 0b0101010000u
#define PPUTLUTRAITS_0b1010101110u 686, (1, 0, 1, 0, 1, 0, 1, 1, 1, 0), 0b0101010001u
#define PPUTLUTRAITS_0b1010101101u 685, (1, 0, 1, 0, 1, 0, 1, 1, 0, 1), 0b0101010010u
#define PPUTLUTRAITS_0b1010101100u 684, (1, 0, 1, 0, 1, 0, 1, 1, 0, 0), 0b0101010011u
#define PPUTLUTRAITS_0b1010101011u 683, (1, 0, 1, 0, 1, 0, 1, 0, 1, 1), 0b0101010100u
#define PPUTLUTRAITS_0b1010101010u 682, (1, 0, 1, 0, 1, 0, 1, 0, 1, 0), 0b0101010101u
#define PPUTLUTRAITS_0b1010101001u 681, (1, 0, 1, 0, 1, 0, 1, 0, 0, 1), 0b0101010110u
#define PPUTLUTRAITS_0b1010101000u 680, (1, 0, 1, 0, 1, 0, 1, 0, 0, 0), 0b0101010111u
#define PPUTLUTRAITS_0b1010100111u 679, (1, 0, 1, 0, 1, 0, 0, 1, 1, 1), 0b0101011000u
#define PPUTLUTRAITS_0b1010100110u 678, (1, 0, 1, 0, 1, 0, 0, 1, 1, 0), 0b0101011001u
#define PPUTLUTRAITS_0b1010100101u 677, (1, 0, 1, 0, 1, 0, 0, 1, 0, 1), 0b0101011010u
#define PPUTLUTRAITS_0b1010100100u 676, (1, 0, 1, 0, 1, 0, 0, 1, 0, 0), 0b0101011011u
#define PPUTLUTRAITS_0b1010100011u 675, (1, 0, 1, 0, 1, 0, 0, 0, 1, 1), 0b0101011100u
#define PPUTLUTRAITS_0b1010100010u 674, (1, 0, 1, 0, 1, 0, 0, 0, 1, 0), 0b0101011101u
#define PPUTLUTRAITS_0b1010100001u 673, (1, 0, 1, 0, 1, 0, 0, 0, 0, 1), 0b0101011110u
#define PPUTLUTRAITS_0b1010100000u 672, (1, 0, 1, 0, 1, 0, 0, 0, 0, 0), 0b0101011111u
#define PPUTLUTRAITS_0b1010011111u 671, (1, 0, 1, 0, 0, 1, 1, 1, 1, 1), 0b0101100000u
#define PPUTLUTRAITS_0b1010011110u 670, (1, 0, 1, 0, 0, 1, 1, 1, 1, 0), 0b0101100001u
#define PPUTLUTRAITS_0b1010011101u 669, (1, 0, 1, 0, 0, 1, 1, 1, 0, 1), 0b0101100010u
#define PPUTLUTRAITS_0b1010011100u 668, (1, 0, 1, 0, 0, 1, 1, 1, 0, 0), 0b0101100011u
#define PPUTLUTRAITS_0b1010011011u 667, (1, 0, 1, 0, 0, 1, 1, 0, 1, 1), 0b0101100100u
#define PPUTLUTRAITS_0b1010011010u 666, (1, 0, 1, 0, 0, 1, 1, 0, 1, 0), 0b0101100101u
#define PPUTLUTRAITS_0b1010011001u 665, (1, 0, 1, 0, 0, 1, 1, 0, 0, 1), 0b0101100110u
#define PPUTLUTRAITS_0b1010011000u 664, (1, 0, 1, 0, 0, 1, 1, 0, 0, 0), 0b0101100111u
#define PPUTLUTRAITS_0b1010010111u 663, (1, 0, 1, 0, 0, 1, 0, 1, 1, 1), 0b0101101000u
#define PPUTLUTRAITS_0b1010010110u 662, (1, 0, 1, 0, 0, 1, 0, 1, 1, 0), 0b0101101001u
#define PPUTLUTRAITS_0b1010010101u 661, (1, 0, 1, 0, 0, 1, 0, 1, 0, 1), 0b0101101010u
#define PPUTLUTRAITS_0b1010010100u 660, (1, 0, 1, 0, 0, 1, 0, 1, 0, 0), 0b0101101011u
#define PPUTLUTRAITS_0b1010010011u 659, (1, 0, 1, 0, 0, 1, 0, 0, 1, 1), 0b0101101100u
#define PPUTLUTRAITS_0b1010010010u 658, (1, 0, 1, 0, 0, 1, 0, 0, 1, 0), 0b0101101101u
#define PPUTLUTRAITS_0b1010010001u 657, (1, 0, 1, 0, 0, 1, 0, 0, 0, 1), 0b0101101110u
#define PPUTLUTRAITS_0b1010010000u 656, (1, 0, 1, 0, 0, 1, 0, 0, 0, 0), 0b0101101111u
#define PPUTLUTRAITS_0b1010001111u 655, (1, 0, 1, 0, 0, 0, 1, 1, 1, 1), 0b0101110000u
#define PPUTLUTRAITS_0b1010001110u 654, (1, 0, 1, 0, 0, 0, 1, 1, 1, 0), 0b0101110001u
#define PPUTLUTRAITS_0b1010001101u 653, (1, 0, 1, 0, 0, 0, 1, 1, 0, 1), 0b0101110010u
#define PPUTLUTRAITS_0b1010001100u 652, (1, 0, 1, 0, 0, 0, 1, 1, 0, 0), 0b0101110011u
#define PPUTLUTRAITS_0b1010001011u 651, (1, 0, 1, 0, 0, 0, 1, 0, 1, 1), 0b0101110100u
#define PPUTLUTRAITS_0b1010001010u 650, (1, 0, 1, 0, 0, 0, 1, 0, 1, 0), 0b0101110101u
#define PPUTLUTRAITS_0b1010001001u 649, (1, 0, 1, 0, 0, 0, 1, 0, 0, 1), 0b0101110110u
#define PPUTLUTRAITS_0b1010001000u 648, (1, 0, 1, 0, 0, 0, 1, 0, 0, 0), 0b0101110111u
#define PPUTLUTRAITS_0b1010000111u 647, (1, 0, 1, 0, 0, 0, 0, 1, 1, 1), 0b0101111000u
#define PPUTLUTRAITS_0b1010000110u 646, (1, 0, 1, 0, 0, 0, 0, 1, 1, 0), 0b0101111001u
#define PPUTLUTRAITS_0b1010000101u 645, (1, 0, 1, 0, 0, 0, 0, 1, 0, 1), 0b0101111010u
#define PPUTLUTRAITS_0b1010000100u 644, (1, 0, 1, 0, 0, 0, 0, 1, 0, 0), 0b0101111011u
#define PPUTLUTRAITS_0b1010000011u 643, (1, 0, 1, 0, 0, 0, 0, 0, 1, 1), 0b0101111100u
#define PPUTLUTRAITS_0b1010000010u 642, (1, 0, 1, 0, 0, 0, 0, 0, 1, 0), 0b0101111101u
#define PPUTLUTRAITS_0b1010000001u 641, (1, 0, 1, 0, 0, 0, 0, 0, 0, 1), 0b0101111110u
#define PPUTLUTRAITS_0b1010000000u 640, (1, 0, 1, 0, 0, 0, 0, 0, 0, 0), 0b0101111111u
#define PPUTLUTRAITS_0b1001111111u 639, (1, 0, 0, 1, 1, 1, 1, 1, 1, 1), 0b0110000000u
#define PPUTLUTRAITS_0b1001111110u 638, (1, 0, 0, 1, 1, 1, 1, 1, 1, 0), 0b0110000001u
#define PPUTLUTRAITS_0b1001111101u 637, (1, 0, 0, 1, 1, 1, 1, 1, 0, 1), 0b0110000010u
#define PPUTLUTRAITS_0b1001111100u 636, (1, 0, 0, 1, 1, 1, 1, 1, 0, 0), 0b0110000011u
#define PPUTLUTRAITS_0b1001111011u 635, (1, 0, 0, 1, 1, 1, 1, 0, 1, 1), 0b0110000100u
#define PPUTLUTRAITS_0b1001111010u 634, (1, 0, 0, 1, 1, 1, 1, 0, 1, 0), 0b0110000101u
#define PPUTLUTRAITS_0b1001111001u 633, (1, 0, 0, 1, 1, 1, 1, 0, 0, 1), 0b0110000110u
#define PPUTLUTRAITS_0b1001111000u 632, (1, 0, 0, 1, 1, 1, 1, 0, 0, 0), 0b0110000111u
#define PPUTLUTRAITS_0b1001110111u 631, (1, 0, 0, 1, 1, 1, 0, 1, 1, 1), 0b0110001000u
#define PPUTLUTRAITS_0b1001110110u 630, (1, 0, 0, 1, 1, 1, 0, 1, 1, 0), 0b0110001001u
#define PPUTLUTRAITS_0b1001110101u 629, (1, 0, 0, 1, 1, 1, 0, 1, 0, 1), 0b0110001010u
#define PPUTLUTRAITS_0b1001110100u 628, (1, 0, 0, 1, 1, 1, 0, 1, 0, 0), 0b0110001011u
#define PPUTLUTRAITS_0b1001110011u 627, (1, 0, 0, 1, 1, 1, 0, 0, 1, 1), 0b0110001100u
#define PPUTLUTRAITS_0b1001110010u 626, (1, 0, 0, 1, 1, 1, 0, 0, 1, 0), 0b0110001101u
#define PPUTLUTRAITS_0b1001110001u 625, (1, 0, 0, 1, 1, 1, 0, 0, 0, 1), 0b0110001110u
#define PPUTLUTRAITS_0b1001110000u 624, (1, 0, 0, 1, 1, 1, 0, 0, 0, 0), 0b0110001111u
#define PPUTLUTRAITS_0b1001101111u 623, (1, 0, 0, 1, 1, 0, 1, 1, 1, 1), 0b0110010000u
#define PPUTLUTRAITS_0b1001101110u 622, (1, 0, 0, 1, 1, 0, 1, 1, 1, 0), 0b0110010001u
#define PPUTLUTRAITS_0b1001101101u 621, (1, 0, 0, 1, 1, 0, 1, 1, 0, 1), 0b0110010010u
#define PPUTLUTRAITS_0b1001101100u 620, (1, 0, 0, 1, 1, 0, 1, 1, 0, 0), 0b0110010011u
#define PPUTLUTRAITS_0b1001101011u 619, (1, 0, 0, 1, 1, 0, 1, 0, 1, 1), 0b0110010100u
#define PPUTLUTRAITS_0b1001101010u 618, (1, 0, 0, 1, 1, 0, 1, 0, 1, 0), 0b0110010101u
#define PPUTLUTRAITS_0b1001101001u 617, (1, 0, 0, 1, 1, 0, 1, 0, 0, 1), 0b0110010110u
#define PPUTLUTRAITS_0b1001101000u 616, (1, 0, 0, 1, 1, 0, 1, 0, 0, 0), 0b0110010111u
#define PPUTLUTRAITS_0b1001100111u 615, (1, 0, 0, 1, 1, 0, 0, 1, 1, 1), 0b0110011000u
#define PPUTLUTRAITS_0b1001100110u 614, (1, 0, 0, 1, 1, 0, 0, 1, 1, 0), 0b0110011001u
#define PPUTLUTRAITS_0b1001100101u 613, (1, 0, 0, 1, 1, 0, 0, 1, 0, 1), 0b0110011010u
#define PPUTLUTRAITS_0b1001100100u 612, (1, 0, 0, 1, 1, 0, 0, 1, 0, 0), 0b0110011011u
#define PPUTLUTRAITS_0b1001100011u 611, (1, 0, 0, 1, 1, 0, 0, 0, 1, 1), 0b0110011100u
#define PPUTLUTRAITS_0b1001100010u 610, (1, 0, 0, 1, 1, 0, 0, 0, 1, 0), 0b0110011101u
#define PPUTLUTRAITS_0b1001100001u 609, (1, 0, 0, 1, 1, 0, 0, 0, 0, 1), 0b0110011110u
#define PPUTLUTRAITS_0b1001100000u 608, (1, 0, 0, 1, 1, 0, 0, 0, 0, 0), 0b0110011111u
#define PPUTLUTRAITS_0b1001011111u 607, (1, 0, 0, 1, 0, 1, 1, 1, 1, 1), 0b0110100000u
#define PPUTLUTRAITS_0b1001011110u 606, (1, 0, 0, 1, 0, 1, 1, 1, 1, 0), 0b0110100001u
#define PPUTLUTRAITS_0b1001011101u 605, (1, 0, 0, 1, 0, 1, 1, 1, 0, 1), 0b0110100010u
#define PPUTLUTRAITS_0b1001011100u 604, (1, 0, 0, 1, 0, 1, 1, 1, 0, 0), 0b0110100011u
#define PPUTLUTRAITS_0b1001011011u 603, (1, 0, 0, 1, 0, 1, 1, 0, 1, 1), 0b0110100100u
#define PPUTLUTRAITS_0b1001011010u 602, (1, 0, 0, 1, 0, 1, 1, 0, 1, 0), 0b0110100101u
#define PPUTLUTRAITS_0b1001011001u 601, (1, 0, 0, 1, 0, 1, 1, 0, 0, 1), 0b0110100110u
#define PPUTLUTRAITS_0b1001011000u 600, (1, 0, 0, 1, 0, 1, 1, 0, 0, 0), 0b0110100111u
#define PPUTLUTRAITS_0b1001010111u 599, (1, 0, 0, 1, 0, 1, 0, 1, 1, 1), 0b0110101000u
#define PPUTLUTRAITS_0b1001010110u 598, (1, 0, 0, 1, 0, 1, 0, 1, 1, 0), 0b0110101001u
#define PPUTLUTRAITS_0b1001010101u 597, (1, 0, 0, 1, 0, 1, 0, 1, 0, 1), 0b0110101010u
#define PPUTLUTRAITS_0b1001010100u 596, (1, 0, 0, 1, 0, 1, 0, 1, 0, 0), 0b0110101011u
#define PPUTLUTRAITS_0b1001010011u 595, (1, 0, 0, 1, 0, 1, 0, 0, 1, 1), 0b0110101100u
#define PPUTLUTRAITS_0b1001010010u 594, (1, 0, 0, 1, 0, 1, 0, 0, 1, 0), 0b0110101101u
#define PPUTLUTRAITS_0b1001010001u 593, (1, 0, 0, 1, 0, 1, 0, 0, 0, 1), 0b0110101110u
#define PPUTLUTRAITS_0b1001010000u 592, (1, 0, 0, 1, 0, 1, 0, 0, 0, 0), 0b0110101111u
#define PPUTLUTRAITS_0b1001001111u 591, (1, 0, 0, 1, 0, 0, 1, 1, 1, 1), 0b0110110000u
#define PPUTLUTRAITS_0b1001001110u 590, (1, 0, 0, 1, 0, 0, 1, 1, 1, 0), 0b0110110001u
#define PPUTLUTRAITS_0b1001001101u 589, (1, 0, 0, 1, 0, 0, 1, 1, 0, 1), 0b0110110010u
#define PPUTLUTRAITS_0b1001001100u 588, (1, 0, 0, 1, 0, 0, 1, 1, 0, 0), 0b0110110011u
#define PPUTLUTRAITS_0b1001001011u 587, (1, 0, 0, 1, 0, 0, 1, 0, 1, 1), 0b0110110100u
#define PPUTLUTRAITS_0b1001001010u 586, (1, 0, 0, 1, 0, 0, 1, 0, 1, 0), 0b0110110101u
#define PPUTLUTRAITS_0b1001001001u 585, (1, 0, 0, 1, 0, 0, 1, 0, 0, 1), 0b0110110110u
#define PPUTLUTRAITS_0b1001001000u 584, (1, 0, 0, 1, 0, 0, 1, 0, 0, 0), 0b0110110111u
#define PPUTLUTRAITS_0b1001000111u 583, (1, 0, 0, 1, 0, 0, 0, 1, 1, 1), 0b0110111000u
#define PPUTLUTRAITS_0b1001000110u 582, (1, 0, 0, 1, 0, 0, 0, 1, 1, 0), 0b0110111001u
#define PPUTLUTRAITS_0b1001000101u 581, (1, 0, 0, 1, 0, 0, 0, 1, 0, 1), 0b0110111010u
#define PPUTLUTRAITS_0b1001000100u 580, (1, 0, 0, 1, 0, 0, 0, 1, 0, 0), 0b0110111011u
#define PPUTLUTRAITS_0b1001000011u 579, (1, 0, 0, 1, 0, 0, 0, 0, 1, 1), 0b0110111100u
#define PPUTLUTRAITS_0b1001000010u 578, (1, 0, 0, 1, 0, 0, 0, 0, 1, 0), 0b0110111101u
#define PPUTLUTRAITS_0b1001000001u 577, (1, 0, 0, 1, 0, 0, 0, 0, 0, 1), 0b0110111110u
#define PPUTLUTRAITS_0b1001000000u 576, (1, 0, 0, 1, 0, 0, 0, 0, 0, 0), 0b0110111111u
#define PPUTLUTRAITS_0b1000111111u 575, (1, 0, 0, 0, 1, 1, 1, 1, 1, 1), 0b0111000000u
#define PPUTLUTRAITS_0b1000111110u 574, (1, 0, 0, 0, 1, 1, 1, 1, 1, 0), 0b0111000001u
#define PPUTLUTRAITS_0b1000111101u 573, (1, 0, 0, 0, 1, 1, 1, 1, 0, 1), 0b0111000010u
#define PPUTLUTRAITS_0b1000111100u 572, (1, 0, 0, 0, 1, 1, 1, 1, 0, 0), 0b0111000011u
#define PPUTLUTRAITS_0b1000111011u 571, (1, 0, 0, 0, 1, 1, 1, 0, 1, 1), 0b0111000100u
#define PPUTLUTRAITS_0b1000111010u 570, (1, 0, 0, 0, 1, 1, 1, 0, 1, 0), 0b0111000101u
#define PPUTLUTRAITS_0b1000111001u 569, (1, 0, 0, 0, 1, 1, 1, 0, 0, 1), 0b0111000110u
#define PPUTLUTRAITS_0b1000111000u 568, (1, 0, 0, 0, 1, 1, 1, 0, 0, 0), 0b0111000111u
#define PPUTLUTRAITS_0b1000110111u 567, (1, 0, 0, 0, 1, 1, 0, 1, 1, 1), 0b0111001000u
#define PPUTLUTRAITS_0b1000110110u 566, (1, 0, 0, 0, 1, 1, 0, 1, 1, 0), 0b0111001001u
#define PPUTLUTRAITS_0b1000110101u 565, (1, 0, 0, 0, 1, 1, 0, 1, 0, 1), 0b0111001010u
#define PPUTLUTRAITS_0b1000110100u 564, (1, 0, 0, 0, 1, 1, 0, 1, 0, 0), 0b0111001011u
#define PPUTLUTRAITS_0b1000110011u 563, (1, 0, 0, 0, 1, 1, 0, 0, 1, 1), 0b0111001100u
#define PPUTLUTRAITS_0b1000110010u 562, (1, 0, 0, 0, 1, 1, 0, 0, 1, 0), 0b0111001101u
#define PPUTLUTRAITS_0b1000110001u 561, (1, 0, 0, 0, 1, 1, 0, 0, 0, 1), 0b0111001110u
#define PPUTLUTRAITS_0b1000110000u 560, (1, 0, 0, 0, 1, 1, 0, 0, 0, 0), 0b0111001111u
#define PPUTLUTRAITS_0b1000101111u 559, (1, 0, 0, 0, 1, 0, 1, 1, 1, 1), 0b0111010000u
#define PPUTLUTRAITS_0b1000101110u 558, (1, 0, 0, 0, 1, 0, 1, 1, 1, 0), 0b0111010001u
#define PPUTLUTRAITS_0b1000101101u 557, (1, 0, 0, 0, 1, 0, 1, 1, 0, 1), 0b0111010010u
#define PPUTLUTRAITS_0b1000101100u 556, (1, 0, 0, 0, 1, 0, 1, 1, 0, 0), 0b0111010011u
#define PPUTLUTRAITS_0b1000101011u 555, (1, 0, 0, 0, 1, 0, 1, 0, 1, 1), 0b0111010100u
#define PPUTLUTRAITS_0b1000101010u 554, (1, 0, 0, 0, 1, 0, 1, 0, 1, 0), 0b0111010101u
#define PPUTLUTRAITS_0b1000101001u 553, (1, 0, 0, 0, 1, 0, 1, 0, 0, 1), 0b0111010110u
#define PPUTLUTRAITS_0b1000101000u 552, (1, 0, 0, 0, 1, 0, 1, 0, 0, 0), 0b0111010111u
#define PPUTLUTRAITS_0b1000100111u 551, (1, 0, 0, 0, 1, 0, 0, 1, 1, 1), 0b0111011000u
#define PPUTLUTRAITS_0b1000100110u 550, (1, 0, 0, 0, 1, 0, 0, 1, 1, 0), 0b0111011001u
#define PPUTLUTRAITS_0b1000100101u 549, (1, 0, 0, 0, 1, 0, 0, 1, 0, 1), 0b0111011010u
#define PPUTLUTRAITS_0b1000100100u 548, (1, 0, 0, 0, 1, 0, 0, 1, 0, 0), 0b0111011011u
#define PPUTLUTRAITS_0b1000100011u 547, (1, 0, 0, 0, 1, 0, 0, 0, 1, 1), 0b0111011100u
#define PPUTLUTRAITS_0b1000100010u 546, (1, 0, 0, 0, 1, 0, 0, 0, 1, 0), 0b0111011101u
#define PPUTLUTRAITS_0b1000100001u 545, (1, 0, 0, 0, 1, 0, 0, 0, 0, 1), 0b0111011110u
#define PPUTLUTRAITS_0b1000100000u 544, (1, 0, 0, 0, 1, 0, 0, 0, 0, 0), 0b0111011111u
#define PPUTLUTRAITS_0b1000011111u 543, (1, 0, 0, 0, 0, 1, 1, 1, 1, 1), 0b0111100000u
#define PPUTLUTRAITS_0b1000011110u 542, (1, 0, 0, 0, 0, 1, 1, 1, 1, 0), 0b0111100001u
#define PPUTLUTRAITS_0b1000011101u 541, (1, 0, 0, 0, 0, 1, 1, 1, 0, 1), 0b0111100010u
#define PPUTLUTRAITS_0b1000011100u 540, (1, 0, 0, 0, 0, 1, 1, 1, 0, 0), 0b0111100011u
#define PPUTLUTRAITS_0b1000011011u 539, (1, 0, 0, 0, 0, 1, 1, 0, 1, 1), 0b0111100100u
#define PPUTLUTRAITS_0b1000011010u 538, (1, 0, 0, 0, 0, 1, 1, 0, 1, 0), 0b0111100101u
#define PPUTLUTRAITS_0b1000011001u 537, (1, 0, 0, 0, 0, 1, 1, 0, 0, 1), 0b0111100110u
#define PPUTLUTRAITS_0b1000011000u 536, (1, 0, 0, 0, 0, 1, 1, 0, 0, 0), 0b0111100111u
#define PPUTLUTRAITS_0b1000010111u 535, (1, 0, 0, 0, 0, 1, 0, 1, 1, 1), 0b0111101000u
#define PPUTLUTRAITS_0b1000010110u 534, (1, 0, 0, 0, 0, 1, 0, 1, 1, 0), 0b0111101001u
#define PPUTLUTRAITS_0b1000010101u 533, (1, 0, 0, 0, 0, 1, 0, 1, 0, 1), 0b0111101010u
#define PPUTLUTRAITS_0b1000010100u 532, (1, 0, 0, 0, 0, 1, 0, 1, 0, 0), 0b0111101011u
#define PPUTLUTRAITS_0b1000010011u 531, (1, 0, 0, 0, 0, 1, 0, 0, 1, 1), 0b0111101100u
#define PPUTLUTRAITS_0b1000010010u 530, (1, 0, 0, 0, 0, 1, 0, 0, 1, 0), 0b0111101101u
#define PPUTLUTRAITS_0b1000010001u 529, (1, 0, 0, 0, 0, 1, 0, 0, 0, 1), 0b0111101110u
#define PPUTLUTRAITS_0b1000010000u 528, (1, 0, 0, 0, 0, 1, 0, 0, 0, 0), 0b0111101111u
#define PPUTLUTRAITS_0b1000001111u 527, (1, 0, 0, 0, 0, 0, 1, 1, 1, 1), 0b0111110000u
#define PPUTLUTRAITS_0b1000001110u 526, (1, 0, 0, 0, 0, 0, 1, 1, 1, 0), 0b0111110001u
#define PPUTLUTRAITS_0b1000001101u 525, (1, 0, 0, 0, 0, 0, 1, 1, 0, 1), 0b0111110010u
#define PPUTLUTRAITS_0b1000001100u 524, (1, 0, 0, 0, 0, 0, 1, 1, 0, 0), 0b0111110011u
#define PPUTLUTRAITS_0b1000001011u 523, (1, 0, 0, 0, 0, 0, 1, 0, 1, 1), 0b0111110100u
#define PPUTLUTRAITS_0b1000001010u 522, (1, 0, 0, 0, 0, 0, 1, 0, 1, 0), 0b0111110101u
#define PPUTLUTRAITS_0b1000001001u 521, (1, 0, 0, 0, 0, 0, 1, 0, 0, 1), 0b0111110110u
#define PPUTLUTRAITS_0b1000001000u 520, (1, 0, 0, 0, 0, 0, 1, 0, 0, 0), 0b0111110111u
#define PPUTLUTRAITS_0b1000000111u 519, (1, 0, 0, 0, 0, 0, 0, 1, 1, 1), 0b0111111000u
#define PPUTLUTRAITS_0b1000000110u 518, (1, 0, 0, 0, 0, 0, 0, 1, 1, 0), 0b0111111001u
#define PPUTLUTRAITS_0b1000000101u 517, (1, 0, 0, 0, 0, 0, 0, 1, 0, 1), 0b0111111010u
#define PPUTLUTRAITS_0b1000000100u 516, (1, 0, 0, 0, 0, 0, 0, 1, 0, 0), 0b0111111011u
#define PPUTLUTRAITS_0b1000000011u 515, (1, 0, 0, 0, 0, 0, 0, 0, 1, 1), 0b0111111100u
#define PPUTLUTRAITS_0b1000000010u 514, (1, 0, 0, 0, 0, 0, 0, 0, 1, 0), 0b0111111101u
#define PPUTLUTRAITS_0b1000000001u 513, (1, 0, 0, 0, 0, 0, 0, 0, 0, 1), 0b0111111110u
#define PPUTLUTRAITS_0b1000000000u 512, (1, 0, 0, 0, 0, 0, 0, 0, 0, 0), 0b0111111111u
#define PPUTLUTRAITS_0b0111111111u 511, (0, 1, 1, 1, 1, 1, 1, 1, 1, 1), 0b1000000000u
#define PPUTLUTRAITS_0b0111111110u 510, (0, 1, 1, 1, 1, 1, 1, 1, 1, 0), 0b1000000001u
#define PPUTLUTRAITS_0b0111111101u 509, (0, 1, 1, 1, 1, 1, 1, 1, 0, 1), 0b1000000010u
#define PPUTLUTRAITS_0b0111111100u 508, (0, 1, 1, 1, 1, 1, 1, 1, 0, 0), 0b1000000011u
#define PPUTLUTRAITS_0b0111111011u 507, (0, 1, 1, 1, 1, 1, 1, 0, 1, 1), 0b1000000100u
#define PPUTLUTRAITS_0b0111111010u 506, (0, 1, 1, 1, 1, 1, 1, 0, 1, 0), 0b1000000101u
#define PPUTLUTRAITS_0b0111111001u 505, (0, 1, 1, 1, 1, 1, 1, 0, 0, 1), 0b1000000110u
#define PPUTLUTRAITS_0b0111111000u 504, (0, 1, 1, 1, 1, 1, 1, 0, 0, 0), 0b1000000111u
#define PPUTLUTRAITS_0b0111110111u 503, (0, 1, 1, 1, 1, 1, 0, 1, 1, 1), 0b1000001000u
#define PPUTLUTRAITS_0b0111110110u 502, (0, 1, 1, 1, 1, 1, 0, 1, 1, 0), 0b1000001001u
#define PPUTLUTRAITS_0b0111110101u 501, (0, 1, 1, 1, 1, 1, 0, 1, 0, 1), 0b1000001010u
#define PPUTLUTRAITS_0b0111110100u 500, (0, 1, 1, 1, 1, 1, 0, 1, 0, 0), 0b1000001011u
#define PPUTLUTRAITS_0b0111110011u 499, (0, 1, 1, 1, 1, 1, 0, 0, 1, 1), 0b1000001100u
#define PPUTLUTRAITS_0b0111110010u 498, (0, 1, 1, 1, 1, 1, 0, 0, 1, 0), 0b1000001101u
#define PPUTLUTRAITS_0b0111110001u 497, (0, 1, 1, 1, 1, 1, 0, 0, 0, 1), 0b1000001110u
#define PPUTLUTRAITS_0b0111110000u 496, (0, 1, 1, 1, 1, 1, 0, 0, 0, 0), 0b1000001111u
#define PPUTLUTRAITS_0b0111101111u 495, (0, 1, 1, 1, 1, 0, 1, 1, 1, 1), 0b1000010000u
#define PPUTLUTRAITS_0b0111101110u 494, (0, 1, 1, 1, 1, 0, 1, 1, 1, 0), 0b1000010001u
#define PPUTLUTRAITS_0b0111101101u 493, (0, 1, 1, 1, 1, 0, 1, 1, 0, 1), 0b1000010010u
#define PPUTLUTRAITS_0b0111101100u 492, (0, 1, 1, 1, 1, 0, 1, 1, 0, 0), 0b1000010011u
#define PPUTLUTRAITS_0b0111101011u 491, (0, 1, 1, 1, 1, 0, 1, 0, 1, 1), 0b1000010100u
#define PPUTLUTRAITS_0b0111101010u 490, (0, 1, 1, 1, 1, 0, 1, 0, 1, 0), 0b1000010101u
#define PPUTLUTRAITS_0b0111101001u 489, (0, 1, 1, 1, 1, 0, 1, 0, 0, 1), 0b1000010110u
#define PPUTLUTRAITS_0b0111101000u 488, (0, 1, 1, 1, 1, 0, 1, 0, 0, 0), 0b1000010111u
#define PPUTLUTRAITS_0b0111100111u 487, (0, 1, 1, 1, 1, 0, 0, 1, 1, 1), 0b1000011000u
#define PPUTLUTRAITS_0b0111100110u 486, (0, 1, 1, 1, 1, 0, 0, 1, 1, 0), 0b1000011001u
#define PPUTLUTRAITS_0b0111100101u 485, (0, 1, 1, 1, 1, 0, 0, 1, 0, 1), 0b1000011010u
#define PPUTLUTRAITS_0b0111100100u 484, (0, 1, 1, 1, 1, 0, 0, 1, 0, 0), 0b1000011011u
#define PPUTLUTRAITS_0b0111100011u 483, (0, 1, 1, 1, 1, 0, 0, 0, 1, 1), 0b1000011100u
#define PPUTLUTRAITS_0b0111100010u 482, (0, 1, 1, 1, 1, 0, 0, 0, 1, 0), 0b1000011101u
#define PPUTLUTRAITS_0b0111100001u 481, (0, 1, 1, 1, 1, 0, 0, 0, 0, 1), 0b1000011110u
#define PPUTLUTRAITS_0b0111100000u 480, (0, 1, 1, 1, 1, 0, 0, 0, 0, 0), 0b1000011111u
#define PPUTLUTRAITS_0b0111011111u 479, (0, 1, 1, 1, 0, 1, 1, 1, 1, 1), 0b1000100000u
#define PPUTLUTRAITS_0b0111011110u 478, (0, 1, 1, 1, 0, 1, 1, 1, 1, 0), 0b1000100001u
#define PPUTLUTRAITS_0b0111011101u 477, (0, 1, 1, 1, 0, 1, 1, 1, 0, 1), 0b1000100010u
#define PPUTLUTRAITS_0b0111011100u 476, (0, 1, 1, 1, 0, 1, 1, 1, 0, 0), 0b1000100011u
#define PPUTLUTRAITS_0b0111011011u 475, (0, 1, 1, 1, 0, 1, 1, 0, 1, 1), 0b1000100100u
#define PPUTLUTRAITS_0b0111011010u 474, (0, 1, 1, 1, 0, 1, 1, 0, 1, 0), 0b1000100101u
#define PPUTLUTRAITS_0b0111011001u 473, (0, 1, 1, 1, 0, 1, 1, 0, 0, 1), 0b1000100110u
#define PPUTLUTRAITS_0b0111011000u 472, (0, 1, 1, 1, 0, 1, 1, 0, 0, 0), 0b1000100111u
#define PPUTLUTRAITS_0b0111010111u 471, (0, 1, 1, 1, 0, 1, 0, 1, 1, 1), 0b1000101000u
#define PPUTLUTRAITS_0b0111010110u 470, (0, 1, 1, 1, 0, 1, 0, 1, 1, 0), 0b1000101001u
#define PPUTLUTRAITS_0b0111010101u 469, (0, 1, 1, 1, 0, 1, 0, 1, 0, 1), 0b1000101010u
#define PPUTLUTRAITS_0b0111010100u 468, (0, 1, 1, 1, 0, 1, 0, 1, 0, 0), 0b1000101011u
#define PPUTLUTRAITS_0b0111010011u 467, (0, 1, 1, 1, 0, 1, 0, 0, 1, 1), 0b1000101100u
#define PPUTLUTRAITS_0b0111010010u 466, (0, 1, 1, 1, 0, 1, 0, 0, 1, 0), 0b1000101101u
#define PPUTLUTRAITS_0b0111010001u 465, (0, 1, 1, 1, 0, 1, 0, 0, 0, 1), 0b1000101110u
#define PPUTLUTRAITS_0b0111010000u 464, (0, 1, 1, 1, 0, 1, 0, 0, 0, 0), 0b1000101111u
#define PPUTLUTRAITS_0b0111001111u 463, (0, 1, 1, 1, 0, 0, 1, 1, 1, 1), 0b1000110000u
#define PPUTLUTRAITS_0b0111001110u 462, (0, 1, 1, 1, 0, 0, 1, 1, 1, 0), 0b1000110001u
#define PPUTLUTRAITS_0b0111001101u 461, (0, 1, 1, 1, 0, 0, 1, 1, 0, 1), 0b1000110010u
#define PPUTLUTRAITS_0b0111001100u 460, (0, 1, 1, 1, 0, 0, 1, 1, 0, 0), 0b1000110011u
#define PPUTLUTRAITS_0b0111001011u 459, (0, 1, 1, 1, 0, 0, 1, 0, 1, 1), 0b1000110100u
#define PPUTLUTRAITS_0b0111001010u 458, (0, 1, 1, 1, 0, 0, 1, 0, 1, 0), 0b1000110101u
#define PPUTLUTRAITS_0b0111001001u 457, (0, 1, 1, 1, 0, 0, 1, 0, 0, 1), 0b1000110110u
#define PPUTLUTRAITS_0b0111001000u 456, (0, 1, 1, 1, 0, 0, 1, 0, 0, 0), 0b1000110111u
#define PPUTLUTRAITS_0b0111000111u 455, (0, 1, 1, 1, 0, 0, 0, 1, 1, 1), 0b1000111000u
#define PPUTLUTRAITS_0b0111000110u 454, (0, 1, 1, 1, 0, 0, 0, 1, 1, 0), 0b1000111001u
#define PPUTLUTRAITS_0b0111000101u 453, (0, 1, 1, 1, 0, 0, 0, 1, 0, 1), 0b1000111010u
#define PPUTLUTRAITS_0b0111000100u 452, (0, 1, 1, 1, 0, 0, 0, 1, 0, 0), 0b1000111011u
#define PPUTLUTRAITS_0b0111000011u 451, (0, 1, 1, 1, 0, 0, 0, 0, 1, 1), 0b1000111100u
#define PPUTLUTRAITS_0b0111000010u 450, (0, 1, 1, 1, 0, 0, 0, 0, 1, 0), 0b1000111101u
#define PPUTLUTRAITS_0b0111000001u 449, (0, 1, 1, 1, 0, 0, 0, 0, 0, 1), 0b1000111110u
#define PPUTLUTRAITS_0b0111000000u 448, (0, 1, 1, 1, 0, 0, 0, 0, 0, 0), 0b1000111111u
#define PPUTLUTRAITS_0b0110111111u 447, (0, 1, 1, 0, 1, 1, 1, 1, 1, 1), 0b1001000000u
#define PPUTLUTRAITS_0b0110111110u 446, (0, 1, 1, 0, 1, 1, 1, 1, 1, 0), 0b1001000001u
#define PPUTLUTRAITS_0b0110111101u 445, (0, 1, 1, 0, 1, 1, 1, 1, 0, 1), 0b1001000010u
#define PPUTLUTRAITS_0b0110111100u 444, (0, 1, 1, 0, 1, 1, 1, 1, 0, 0), 0b1001000011u
#define PPUTLUTRAITS_0b0110111011u 443, (0, 1, 1, 0, 1, 1, 1, 0, 1, 1), 0b1001000100u
#define PPUTLUTRAITS_0b0110111010u 442, (0, 1, 1, 0, 1, 1, 1, 0, 1, 0), 0b1001000101u
#define PPUTLUTRAITS_0b0110111001u 441, (0, 1, 1, 0, 1, 1, 1, 0, 0, 1), 0b1001000110u
#define PPUTLUTRAITS_0b0110111000u 440, (0, 1, 1, 0, 1, 1, 1, 0, 0, 0), 0b1001000111u
#define PPUTLUTRAITS_0b0110110111u 439, (0, 1, 1, 0, 1, 1, 0, 1, 1, 1), 0b1001001000u
#define PPUTLUTRAITS_0b0110110110u 438, (0, 1, 1, 0, 1, 1, 0, 1, 1, 0), 0b1001001001u
#define PPUTLUTRAITS_0b0110110101u 437, (0, 1, 1, 0, 1, 1, 0, 1, 0, 1), 0b1001001010u
#define PPUTLUTRAITS_0b0110110100u 436, (0, 1, 1, 0, 1, 1, 0, 1, 0, 0), 0b1001001011u
#define PPUTLUTRAITS_0b0110110011u 435, (0, 1, 1, 0, 1, 1, 0, 0, 1, 1), 0b1001001100u
#define PPUTLUTRAITS_0b0110110010u 434, (0, 1, 1, 0, 1, 1, 0, 0, 1, 0), 0b1001001101u
#define PPUTLUTRAITS_0b0110110001u 433, (0, 1, 1, 0, 1, 1, 0, 0, 0, 1), 0b1001001110u
#define PPUTLUTRAITS_0b0110110000u 432, (0, 1, 1, 0, 1, 1, 0, 0, 0, 0), 0b1001001111u
#define PPUTLUTRAITS_0b0110101111u 431, (0, 1, 1, 0, 1, 0, 1, 1, 1, 1), 0b1001010000u
#define PPUTLUTRAITS_0b0110101110u 430, (0, 1, 1, 0, 1, 0, 1, 1, 1, 0), 0b1001010001u
#define PPUTLUTRAITS_0b0110101101u 429, (0, 1, 1, 0, 1, 0, 1, 1, 0, 1), 0b1001010010u
#define PPUTLUTRAITS_0b0110101100u 428, (0, 1, 1, 0, 1, 0, 1, 1, 0, 0), 0b1001010011u
#define PPUTLUTRAITS_0b0110101011u 427, (0, 1, 1, 0, 1, 0, 1, 0, 1, 1), 0b1001010100u
#define PPUTLUTRAITS_0b0110101010u 426, (0, 1, 1, 0, 1, 0, 1, 0, 1, 0), 0b1001010101u
#define PPUTLUTRAITS_0b0110101001u 425, (0, 1, 1, 0, 1, 0, 1, 0, 0, 1), 0b1001010110u
#define PPUTLUTRAITS_0b0110101000u 424, (0, 1, 1, 0, 1, 0, 1, 0, 0, 0), 0b1001010111u
#define PPUTLUTRAITS_0b0110100111u 423, (0, 1, 1, 0, 1, 0, 0, 1, 1, 1), 0b1001011000u
#define PPUTLUTRAITS_0b0110100110u 422, (0, 1, 1, 0, 1, 0, 0, 1, 1, 0), 0b1001011001u
#define PPUTLUTRAITS_0b0110100101u 421, (0, 1, 1, 0, 1, 0, 0, 1, 0, 1), 0b1001011010u
#define PPUTLUTRAITS_0b0110100100u 420, (0, 1, 1, 0, 1, 0, 0, 1, 0, 0), 0b1001011011u
#define PPUTLUTRAITS_0b0110100011u 419, (0, 1, 1, 0, 1, 0, 0, 0, 1, 1), 0b1001011100u
#define PPUTLUTRAITS_0b0110100010u 418, (0, 1, 1, 0, 1, 0, 0, 0, 1, 0), 0b1001011101u
#define PPUTLUTRAITS_0b0110100001u 417, (0, 1, 1, 0, 1, 0, 0, 0, 0, 1), 0b1001011110u
#define PPUTLUTRAITS_0b0110100000u 416, (0, 1, 1, 0, 1, 0, 0, 0, 0, 0), 0b1001011111u
#define PPUTLUTRAITS_0b0110011111u 415, (0, 1, 1, 0, 0, 1, 1, 1, 1, 1), 0b1001100000u
#define PPUTLUTRAITS_0b0110011110u 414, (0, 1, 1, 0, 0, 1, 1, 1, 1, 0), 0b1001100001u
#define PPUTLUTRAITS_0b0110011101u 413, (0, 1, 1, 0, 0, 1, 1, 1, 0, 1), 0b1001100010u
#define PPUTLUTRAITS_0b0110011100u 412, (0, 1, 1, 0, 0, 1, 1, 1, 0, 0), 0b1001100011u
#define PPUTLUTRAITS_0b0110011011u 411, (0, 1, 1, 0, 0, 1, 1, 0, 1, 1), 0b1001100100u
#define PPUTLUTRAITS_0b0110011010u 410, (0, 1, 1, 0, 0, 1, 1, 0, 1, 0), 0b1001100101u
#define PPUTLUTRAITS_0b0110011001u 409, (0, 1, 1, 0, 0, 1, 1, 0, 0, 1), 0b1001100110u
#define PPUTLUTRAITS_0b0110011000u 408, (0, 1, 1, 0, 0, 1, 1, 0, 0, 0), 0b1001100111u
#define PPUTLUTRAITS_0b0110010111u 407, (0, 1, 1, 0, 0, 1, 0, 1, 1, 1), 0b1001101000u
#define PPUTLUTRAITS_0b0110010110u 406, (0, 1, 1, 0, 0, 1, 0, 1, 1, 0), 0b1001101001u
#define PPUTLUTRAITS_0b0110010101u 405, (0, 1, 1, 0, 0, 1, 0, 1, 0, 1), 0b1001101010u
#define PPUTLUTRAITS_0b0110010100u 404, (0, 1, 1, 0, 0, 1, 0, 1, 0, 0), 0b1001101011u
#define PPUTLUTRAITS_0b0110010011u 403, (0, 1, 1, 0, 0, 1, 0, 0, 1, 1), 0b1001101100u
#define PPUTLUTRAITS_0b0110010010u 402, (0, 1, 1, 0, 0, 1, 0, 0, 1, 0), 0b1001101101u
#define PPUTLUTRAITS_0b0110010001u 401, (0, 1, 1, 0, 0, 1, 0, 0, 0, 1), 0b1001101110u
#define PPUTLUTRAITS_0b0110010000u 400, (0, 1, 1, 0, 0, 1, 0, 0, 0, 0), 0b1001101111u
#define PPUTLUTRAITS_0b0110001111u 399, (0, 1, 1, 0, 0, 0, 1, 1, 1, 1), 0b1001110000u
#define PPUTLUTRAITS_0b0110001110u 398, (0, 1, 1, 0, 0, 0, 1, 1, 1, 0), 0b1001110001u
#define PPUTLUTRAITS_0b0110001101u 397, (0, 1, 1, 0, 0, 0, 1, 1, 0, 1), 0b1001110010u
#define PPUTLUTRAITS_0b0110001100u 396, (0, 1, 1, 0, 0, 0, 1, 1, 0, 0), 0b1001110011u
#define PPUTLUTRAITS_0b0110001011u 395, (0, 1, 1, 0, 0, 0, 1, 0, 1, 1), 0b1001110100u
#define PPUTLUTRAITS_0b0110001010u 394, (0, 1, 1, 0, 0, 0, 1, 0, 1, 0), 0b1001110101u
#define PPUTLUTRAITS_0b0110001001u 393, (0, 1, 1, 0, 0, 0, 1, 0, 0, 1), 0b1001110110u
#define PPUTLUTRAITS_0b0110001000u 392, (0, 1, 1, 0, 0, 0, 1, 0, 0, 0), 0b1001110111u
#define PPUTLUTRAITS_0b0110000111u 391, (0, 1, 1, 0, 0, 0, 0, 1, 1, 1), 0b1001111000u
#define PPUTLUTRAITS_0b0110000110u 390, (0, 1, 1, 0, 0, 0, 0, 1, 1, 0), 0b1001111001u
#define PPUTLUTRAITS_0b0110000101u 389, (0, 1, 1, 0, 0, 0, 0, 1, 0, 1), 0b1001111010u
#define PPUTLUTRAITS_0b0110000100u 388, (0, 1, 1, 0, 0, 0, 0, 1, 0, 0), 0b1001111011u
#define PPUTLUTRAITS_0b0110000011u 387, (0, 1, 1, 0, 0, 0, 0, 0, 1, 1), 0b1001111100u
#define PPUTLUTRAITS_0b0110000010u 386, (0, 1, 1, 0, 0, 0, 0, 0, 1, 0), 0b1001111101u
#define PPUTLUTRAITS_0b0110000001u 385, (0, 1, 1, 0, 0, 0, 0, 0, 0, 1), 0b1001111110u
#define PPUTLUTRAITS_0b0110000000u 384, (0, 1, 1, 0, 0, 0, 0, 0, 0, 0), 0b1001111111u
#define PPUTLUTRAITS_0b0101111111u 383, (0, 1, 0, 1, 1, 1, 1, 1, 1, 1), 0b1010000000u
#define PPUTLUTRAITS_0b0101111110u 382, (0, 1, 0, 1, 1, 1, 1, 1, 1, 0), 0b1010000001u
#define PPUTLUTRAITS_0b0101111101u 381, (0, 1, 0, 1, 1, 1, 1, 1, 0, 1), 0b1010000010u
#define PPUTLUTRAITS_0b0101111100u 380, (0, 1, 0, 1, 1, 1, 1, 1, 0, 0), 0b1010000011u
#define PPUTLUTRAITS_0b0101111011u 379, (0, 1, 0, 1, 1, 1, 1, 0, 1, 1), 0b1010000100u
#define PPUTLUTRAITS_0b0101111010u 378, (0, 1, 0, 1, 1, 1, 1, 0, 1, 0), 0b1010000101u
#define PPUTLUTRAITS_0b0101111001u 377, (0, 1, 0, 1, 1, 1, 1, 0, 0, 1), 0b1010000110u
#define PPUTLUTRAITS_0b0101111000u 376, (0, 1, 0, 1, 1, 1, 1, 0, 0, 0), 0b1010000111u
#define PPUTLUTRAITS_0b0101110111u 375, (0, 1, 0, 1, 1, 1, 0, 1, 1, 1), 0b1010001000u
#define PPUTLUTRAITS_0b0101110110u 374, (0, 1, 0, 1, 1, 1, 0, 1, 1, 0), 0b1010001001u
#define PPUTLUTRAITS_0b0101110101u 373, (0, 1, 0, 1, 1, 1, 0, 1, 0, 1), 0b1010001010u
#define PPUTLUTRAITS_0b0101110100u 372, (0, 1, 0, 1, 1, 1, 0, 1, 0, 0), 0b1010001011u
#define PPUTLUTRAITS_0b0101110011u 371, (0, 1, 0, 1, 1, 1, 0, 0, 1, 1), 0b1010001100u
#define PPUTLUTRAITS_0b0101110010u 370, (0, 1, 0, 1, 1, 1, 0, 0, 1, 0), 0b1010001101u
#define PPUTLUTRAITS_0b0101110001u 369, (0, 1, 0, 1, 1, 1, 0, 0, 0, 1), 0b1010001110u
#define PPUTLUTRAITS_0b0101110000u 368, (0, 1, 0, 1, 1, 1, 0, 0, 0, 0), 0b1010001111u
#define PPUTLUTRAITS_0b0101101111u 367, (0, 1, 0, 1, 1, 0, 1, 1, 1, 1), 0b1010010000u
#define PPUTLUTRAITS_0b0101101110u 366, (0, 1, 0, 1, 1, 0, 1, 1, 1, 0), 0b1010010001u
#define PPUTLUTRAITS_0b0101101101u 365, (0, 1, 0, 1, 1, 0, 1, 1, 0, 1), 0b1010010010u
#define PPUTLUTRAITS_0b0101101100u 364, (0, 1, 0, 1, 1, 0, 1, 1, 0, 0), 0b1010010011u
#define PPUTLUTRAITS_0b0101101011u 363, (0, 1, 0, 1, 1, 0, 1, 0, 1, 1), 0b1010010100u
#define PPUTLUTRAITS_0b0101101010u 362, (0, 1, 0, 1, 1, 0, 1, 0, 1, 0), 0b1010010101u
#define PPUTLUTRAITS_0b0101101001u 361, (0, 1, 0, 1, 1, 0, 1, 0, 0, 1), 0b1010010110u
#define PPUTLUTRAITS_0b0101101000u 360, (0, 1, 0, 1, 1, 0, 1, 0, 0, 0), 0b1010010111u
#define PPUTLUTRAITS_0b0101100111u 359, (0, 1, 0, 1, 1, 0, 0, 1, 1, 1), 0b1010011000u
#define PPUTLUTRAITS_0b0101100110u 358, (0, 1, 0, 1, 1, 0, 0, 1, 1, 0), 0b1010011001u
#define PPUTLUTRAITS_0b0101100101u 357, (0, 1, 0, 1, 1, 0, 0, 1, 0, 1), 0b1010011010u
#define PPUTLUTRAITS_0b0101100100u 356, (0, 1, 0, 1, 1, 0, 0, 1, 0, 0), 0b1010011011u
#define PPUTLUTRAITS_0b0101100011u 355, (0, 1, 0, 1, 1, 0, 0, 0, 1, 1), 0b1010011100u
#define PPUTLUTRAITS_0b0101100010u 354, (0, 1, 0, 1, 1, 0, 0, 0, 1, 0), 0b1010011101u
#define PPUTLUTRAITS_0b0101100001u 353, (0, 1, 0, 1, 1, 0, 0, 0, 0, 1), 0b1010011110u
#define PPUTLUTRAITS_0b0101100000u 352, (0, 1, 0, 1, 1, 0, 0, 0, 0, 0), 0b1010011111u
#define PPUTLUTRAITS_0b0101011111u 351, (0, 1, 0, 1, 0, 1, 1, 1, 1, 1), 0b1010100000u
#define PPUTLUTRAITS_0b0101011110u 350, (0, 1, 0, 1, 0, 1, 1, 1, 1, 0), 0b1010100001u
#define PPUTLUTRAITS_0b0101011101u 349, (0, 1, 0, 1, 0, 1, 1, 1, 0, 1), 0b1010100010u
#define PPUTLUTRAITS_0b0101011100u 348, (0, 1, 0, 1, 0, 1, 1, 1, 0, 0), 0b1010100011u
#define PPUTLUTRAITS_0b0101011011u 347, (0, 1, 0, 1, 0, 1, 1, 0, 1, 1), 0b1010100100u
#define PPUTLUTRAITS_0b0101011010u 346, (0, 1, 0, 1, 0, 1, 1, 0, 1, 0), 0b1010100101u
#define PPUTLUTRAITS_0b0101011001u 345, (0, 1, 0, 1, 0, 1, 1, 0, 0, 1), 0b1010100110u
#define PPUTLUTRAITS_0b0101011000u 344, (0, 1, 0, 1, 0, 1, 1, 0, 0, 0), 0b1010100111u
#define PPUTLUTRAITS_0b0101010111u 343, (0, 1, 0, 1, 0, 1, 0, 1, 1, 1), 0b1010101000u
#define PPUTLUTRAITS_0b0101010110u 342, (0, 1, 0, 1, 0, 1, 0, 1, 1, 0), 0b1010101001u
#define PPUTLUTRAITS_0b0101010101u 341, (0, 1, 0, 1, 0, 1, 0, 1, 0, 1), 0b1010101010u
#define PPUTLUTRAITS_0b0101010100u 340, (0, 1, 0, 1, 0, 1, 0, 1, 0, 0), 0b1010101011u
#define PPUTLUTRAITS_0b0101010011u 339, (0, 1, 0, 1, 0, 1, 0, 0, 1, 1), 0b1010101100u
#define PPUTLUTRAITS_0b0101010010u 338, (0, 1, 0, 1, 0, 1, 0, 0, 1, 0), 0b1010101101u
#define PPUTLUTRAITS_0b0101010001u 337, (0, 1, 0, 1, 0, 1, 0, 0, 0, 1), 0b1010101110u
#define PPUTLUTRAITS_0b0101010000u 336, (0, 1, 0, 1, 0, 1, 0, 0, 0, 0), 0b1010101111u
#define PPUTLUTRAITS_0b0101001111u 335, (0, 1, 0, 1, 0, 0, 1, 1, 1, 1), 0b1010110000u
#define PPUTLUTRAITS_0b0101001110u 334, (0, 1, 0, 1, 0, 0, 1, 1, 1, 0), 0b1010110001u
#define PPUTLUTRAITS_0b0101001101u 333, (0, 1, 0, 1, 0, 0, 1, 1, 0, 1), 0b1010110010u
#define PPUTLUTRAITS_0b0101001100u 332, (0, 1, 0, 1, 0, 0, 1, 1, 0, 0), 0b1010110011u
#define PPUTLUTRAITS_0b0101001011u 331, (0, 1, 0, 1, 0, 0, 1, 0, 1, 1), 0b1010110100u
#define PPUTLUTRAITS_0b0101001010u 330, (0, 1, 0, 1, 0, 0, 1, 0, 1, 0), 0b1010110101u
#define PPUTLUTRAITS_0b0101001001u 329, (0, 1, 0, 1, 0, 0, 1, 0, 0, 1), 0b1010110110u
#define PPUTLUTRAITS_0b0101001000u 328, (0, 1, 0, 1, 0, 0, 1, 0, 0, 0), 0b1010110111u
#define PPUTLUTRAITS_0b0101000111u 327, (0, 1, 0, 1, 0, 0, 0, 1, 1, 1), 0b1010111000u
#define PPUTLUTRAITS_0b0101000110u 326, (0, 1, 0, 1, 0, 0, 0, 1, 1, 0), 0b1010111001u
#define PPUTLUTRAITS_0b0101000101u 325, (0, 1, 0, 1, 0, 0, 0, 1, 0, 1), 0b1010111010u
#define PPUTLUTRAITS_0b0101000100u 324, (0, 1, 0, 1, 0, 0, 0, 1, 0, 0), 0b1010111011u
#define PPUTLUTRAITS_0b0101000011u 323, (0, 1, 0, 1, 0, 0, 0, 0, 1, 1), 0b1010111100u
#define PPUTLUTRAITS_0b0101000010u 322, (0, 1, 0, 1, 0, 0, 0, 0, 1, 0), 0b1010111101u
#define PPUTLUTRAITS_0b0101000001u 321, (0, 1, 0, 1, 0, 0, 0, 0, 0, 1), 0b1010111110u
#define PPUTLUTRAITS_0b0101000000u 320, (0, 1, 0, 1, 0, 0, 0, 0, 0, 0), 0b1010111111u
#define PPUTLUTRAITS_0b0100111111u 319, (0, 1, 0, 0, 1, 1, 1, 1, 1, 1), 0b1011000000u
#define PPUTLUTRAITS_0b0100111110u 318, (0, 1, 0, 0, 1, 1, 1, 1, 1, 0), 0b1011000001u
#define PPUTLUTRAITS_0b0100111101u 317, (0, 1, 0, 0, 1, 1, 1, 1, 0, 1), 0b1011000010u
#define PPUTLUTRAITS_0b0100111100u 316, (0, 1, 0, 0, 1, 1, 1, 1, 0, 0), 0b1011000011u
#define PPUTLUTRAITS_0b0100111011u 315, (0, 1, 0, 0, 1, 1, 1, 0, 1, 1), 0b1011000100u
#define PPUTLUTRAITS_0b0100111010u 314, (0, 1, 0, 0, 1, 1, 1, 0, 1, 0), 0b1011000101u
#define PPUTLUTRAITS_0b0100111001u 313, (0, 1, 0, 0, 1, 1, 1, 0, 0, 1), 0b1011000110u
#define PPUTLUTRAITS_0b0100111000u 312, (0, 1, 0, 0, 1, 1, 1, 0, 0, 0), 0b1011000111u
#define PPUTLUTRAITS_0b0100110111u 311, (0, 1, 0, 0, 1, 1, 0, 1, 1, 1), 0b1011001000u
#define PPUTLUTRAITS_0b0100110110u 310, (0, 1, 0, 0, 1, 1, 0, 1, 1, 0), 0b1011001001u
#define PPUTLUTRAITS_0b0100110101u 309, (0, 1, 0, 0, 1, 1, 0, 1, 0, 1), 0b1011001010u
#define PPUTLUTRAITS_0b0100110100u 308, (0, 1, 0, 0, 1, 1, 0, 1, 0, 0), 0b1011001011u
#define PPUTLUTRAITS_0b0100110011u 307, (0, 1, 0, 0, 1, 1, 0, 0, 1, 1), 0b1011001100u
#define PPUTLUTRAITS_0b0100110010u 306, (0, 1, 0, 0, 1, 1, 0, 0, 1, 0), 0b1011001101u
#define PPUTLUTRAITS_0b0100110001u 305, (0, 1, 0, 0, 1, 1, 0, 0, 0, 1), 0b1011001110u
#define PPUTLUTRAITS_0b0100110000u 304, (0, 1, 0, 0, 1, 1, 0, 0, 0, 0), 0b1011001111u
#define PPUTLUTRAITS_0b0100101111u 303, (0, 1, 0, 0, 1, 0, 1, 1, 1, 1), 0b1011010000u
#define PPUTLUTRAITS_0b0100101110u 302, (0, 1, 0, 0, 1, 0, 1, 1, 1, 0), 0b1011010001u
#define PPUTLUTRAITS_0b0100101101u 301, (0, 1, 0, 0, 1, 0, 1, 1, 0, 1), 0b1011010010u
#define PPUTLUTRAITS_0b0100101100u 300, (0, 1, 0, 0, 1, 0, 1, 1, 0, 0), 0b1011010011u
#define PPUTLUTRAITS_0b0100101011u 299, (0, 1, 0, 0, 1, 0, 1, 0, 1, 1), 0b1011010100u
#define PPUTLUTRAITS_0b0100101010u 298, (0, 1, 0, 0, 1, 0, 1, 0, 1, 0), 0b1011010101u
#define PPUTLUTRAITS_0b0100101001u 297, (0, 1, 0, 0, 1, 0, 1, 0, 0, 1), 0b1011010110u
#define PPUTLUTRAITS_0b0100101000u 296, (0, 1, 0, 0, 1, 0, 1, 0, 0, 0), 0b1011010111u
#define PPUTLUTRAITS_0b0100100111u 295, (0, 1, 0, 0, 1, 0, 0, 1, 1, 1), 0b1011011000u
#define PPUTLUTRAITS_0b0100100110u 294, (0, 1, 0, 0, 1, 0, 0, 1, 1, 0), 0b1011011001u
#define PPUTLUTRAITS_0b0100100101u 293, (0, 1, 0, 0, 1, 0, 0, 1, 0, 1), 0b1011011010u
#define PPUTLUTRAITS_0b0100100100u 292, (0, 1, 0, 0, 1, 0, 0, 1, 0, 0), 0b1011011011u
#define PPUTLUTRAITS_0b0100100011u 291, (0, 1, 0, 0, 1, 0, 0, 0, 1, 1), 0b1011011100u
#define PPUTLUTRAITS_0b0100100010u 290, (0, 1, 0, 0, 1, 0, 0, 0, 1, 0), 0b1011011101u
#define PPUTLUTRAITS_0b0100100001u 289, (0, 1, 0, 0, 1, 0, 0, 0, 0, 1), 0b1011011110u
#define PPUTLUTRAITS_0b0100100000u 288, (0, 1, 0, 0, 1, 0, 0, 0, 0, 0), 0b1011011111u
#define PPUTLUTRAITS_0b0100011111u 287, (0, 1, 0, 0, 0, 1, 1, 1, 1, 1), 0b1011100000u
#define PPUTLUTRAITS_0b0100011110u 286, (0, 1, 0, 0, 0, 1, 1, 1, 1, 0), 0b1011100001u
#define PPUTLUTRAITS_0b0100011101u 285, (0, 1, 0, 0, 0, 1, 1, 1, 0, 1), 0b1011100010u
#define PPUTLUTRAITS_0b0100011100u 284, (0, 1, 0, 0, 0, 1, 1, 1, 0, 0), 0b1011100011u
#define PPUTLUTRAITS_0b0100011011u 283, (0, 1, 0, 0, 0, 1, 1, 0, 1, 1), 0b1011100100u
#define PPUTLUTRAITS_0b0100011010u 282, (0, 1, 0, 0, 0, 1, 1, 0, 1, 0), 0b1011100101u
#define PPUTLUTRAITS_0b0100011001u 281, (0, 1, 0, 0, 0, 1, 1, 0, 0, 1), 0b1011100110u
#define PPUTLUTRAITS_0b0100011000u 280, (0, 1, 0, 0, 0, 1, 1, 0, 0, 0), 0b1011100111u
#define PPUTLUTRAITS_0b0100010111u 279, (0, 1, 0, 0, 0, 1, 0, 1, 1, 1), 0b1011101000u
#define PPUTLUTRAITS_0b0100010110u 278, (0, 1, 0, 0, 0, 1, 0, 1, 1, 0), 0b1011101001u
#define PPUTLUTRAITS_0b0100010101u 277, (0, 1, 0, 0, 0, 1, 0, 1, 0, 1), 0b1011101010u
#define PPUTLUTRAITS_0b0100010100u 276, (0, 1, 0, 0, 0, 1, 0, 1, 0, 0), 0b1011101011u
#define PPUTLUTRAITS_0b0100010011u 275, (0, 1, 0, 0, 0, 1, 0, 0, 1, 1), 0b1011101100u
#define PPUTLUTRAITS_0b0100010010u 274, (0, 1, 0, 0, 0, 1, 0, 0, 1, 0), 0b1011101101u
#define PPUTLUTRAITS_0b0100010001u 273, (0, 1, 0, 0, 0, 1, 0, 0, 0, 1), 0b1011101110u
#define PPUTLUTRAITS_0b0100010000u 272, (0, 1, 0, 0, 0, 1, 0, 0, 0, 0), 0b1011101111u
#define PPUTLUTRAITS_0b0100001111u 271, (0, 1, 0, 0, 0, 0, 1, 1, 1, 1), 0b1011110000u
#define PPUTLUTRAITS_0b0100001110u 270, (0, 1, 0, 0, 0, 0, 1, 1, 1, 0), 0b1011110001u
#define PPUTLUTRAITS_0b0100001101u 269, (0, 1, 0, 0, 0, 0, 1, 1, 0, 1), 0b1011110010u
#define PPUTLUTRAITS_0b0100001100u 268, (0, 1, 0, 0, 0, 0, 1, 1, 0, 0), 0b1011110011u
#define PPUTLUTRAITS_0b0100001011u 267, (0, 1, 0, 0, 0, 0, 1, 0, 1, 1), 0b1011110100u
#define PPUTLUTRAITS_0b0100001010u 266, (0, 1, 0, 0, 0, 0, 1, 0, 1, 0), 0b1011110101u
#define PPUTLUTRAITS_0b0100001001u 265, (0, 1, 0, 0, 0, 0, 1, 0, 0, 1), 0b1011110110u
#define PPUTLUTRAITS_0b0100001000u 264, (0, 1, 0, 0, 0, 0, 1, 0, 0, 0), 0b1011110111u
#define PPUTLUTRAITS_0b0100000111u 263, (0, 1, 0, 0, 0, 0, 0, 1, 1, 1), 0b1011111000u
#define PPUTLUTRAITS_0b0100000110u 262, (0, 1, 0, 0, 0, 0, 0, 1, 1, 0), 0b1011111001u
#define PPUTLUTRAITS_0b0100000101u 261, (0, 1, 0, 0, 0, 0, 0, 1, 0, 1), 0b1011111010u
#define PPUTLUTRAITS_0b0100000100u 260, (0, 1, 0, 0, 0, 0, 0, 1, 0, 0), 0b1011111011u
#define PPUTLUTRAITS_0b0100000011u 259, (0, 1, 0, 0, 0, 0, 0, 0, 1, 1), 0b1011111100u
#define PPUTLUTRAITS_0b0100000010u 258, (0, 1, 0, 0, 0, 0, 0, 0, 1, 0), 0b1011111101u
#define PPUTLUTRAITS_0b0100000001u 257, (0, 1, 0, 0, 0, 0, 0, 0, 0, 1), 0b1011111110u
#define PPUTLUTRAITS_0b0100000000u 256, (0, 1, 0, 0, 0, 0, 0, 0, 0, 0), 0b1011111111u
#define PPUTLUTRAITS_0b0011111111u 255, (0, 0, 1, 1, 1, 1, 1, 1, 1, 1), 0b1100000000u
#define PPUTLUTRAITS_0b0011111110u 254, (0, 0, 1, 1, 1, 1, 1, 1, 1, 0), 0b1100000001u
#define PPUTLUTRAITS_0b0011111101u 253, (0, 0, 1, 1, 1, 1, 1, 1, 0, 1), 0b1100000010u
#define PPUTLUTRAITS_0b0011111100u 252, (0, 0, 1, 1, 1, 1, 1, 1, 0, 0), 0b1100000011u
#define PPUTLUTRAITS_0b0011111011u 251, (0, 0, 1, 1, 1, 1, 1, 0, 1, 1), 0b1100000100u
#define PPUTLUTRAITS_0b0011111010u 250, (0, 0, 1, 1, 1, 1, 1, 0, 1, 0), 0b1100000101u
#define PPUTLUTRAITS_0b0011111001u 249, (0, 0, 1, 1, 1, 1, 1, 0, 0, 1), 0b1100000110u
#define PPUTLUTRAITS_0b0011111000u 248, (0, 0, 1, 1, 1, 1, 1, 0, 0, 0), 0b1100000111u
#define PPUTLUTRAITS_0b0011110111u 247, (0, 0, 1, 1, 1, 1, 0, 1, 1, 1), 0b1100001000u
#define PPUTLUTRAITS_0b0011110110u 246, (0, 0, 1, 1, 1, 1, 0, 1, 1, 0), 0b1100001001u
#define PPUTLUTRAITS_0b0011110101u 245, (0, 0, 1, 1, 1, 1, 0, 1, 0, 1), 0b1100001010u
#define PPUTLUTRAITS_0b0011110100u 244, (0, 0, 1, 1, 1, 1, 0, 1, 0, 0), 0b1100001011u
#define PPUTLUTRAITS_0b0011110011u 243, (0, 0, 1, 1, 1, 1, 0, 0, 1, 1), 0b1100001100u
#define PPUTLUTRAITS_0b0011110010u 242, (0, 0, 1, 1, 1, 1, 0, 0, 1, 0), 0b1100001101u
#define PPUTLUTRAITS_0b0011110001u 241, (0, 0, 1, 1, 1, 1, 0, 0, 0, 1), 0b1100001110u
#define PPUTLUTRAITS_0b0011110000u 240, (0, 0, 1, 1, 1, 1, 0, 0, 0, 0), 0b1100001111u
#define PPUTLUTRAITS_0b0011101111u 239, (0, 0, 1, 1, 1, 0, 1, 1, 1, 1), 0b1100010000u
#define PPUTLUTRAITS_0b0011101110u 238, (0, 0, 1, 1, 1, 0, 1, 1, 1, 0), 0b1100010001u
#define PPUTLUTRAITS_0b0011101101u 237, (0, 0, 1, 1, 1, 0, 1, 1, 0, 1), 0b1100010010u
#define PPUTLUTRAITS_0b0011101100u 236, (0, 0, 1, 1, 1, 0, 1, 1, 0, 0), 0b1100010011u
#define PPUTLUTRAITS_0b0011101011u 235, (0, 0, 1, 1, 1, 0, 1, 0, 1, 1), 0b1100010100u
#define PPUTLUTRAITS_0b0011101010u 234, (0, 0, 1, 1, 1, 0, 1, 0, 1, 0), 0b1100010101u
#define PPUTLUTRAITS_0b0011101001u 233, (0, 0, 1, 1, 1, 0, 1, 0, 0, 1), 0b1100010110u
#define PPUTLUTRAITS_0b0011101000u 232, (0, 0, 1, 1, 1, 0, 1, 0, 0, 0), 0b1100010111u
#define PPUTLUTRAITS_0b0011100111u 231, (0, 0, 1, 1, 1, 0, 0, 1, 1, 1), 0b1100011000u
#define PPUTLUTRAITS_0b0011100110u 230, (0, 0, 1, 1, 1, 0, 0, 1, 1, 0), 0b1100011001u
#define PPUTLUTRAITS_0b0011100101u 229, (0, 0, 1, 1, 1, 0, 0, 1, 0, 1), 0b1100011010u
#define PPUTLUTRAITS_0b0011100100u 228, (0, 0, 1, 1, 1, 0, 0, 1, 0, 0), 0b1100011011u
#define PPUTLUTRAITS_0b0011100011u 227, (0, 0, 1, 1, 1, 0, 0, 0, 1, 1), 0b1100011100u
#define PPUTLUTRAITS_0b0011100010u 226, (0, 0, 1, 1, 1, 0, 0, 0, 1, 0), 0b1100011101u
#define PPUTLUTRAITS_0b0011100001u 225, (0, 0, 1, 1, 1, 0, 0, 0, 0, 1), 0b1100011110u
#define PPUTLUTRAITS_0b0011100000u 224, (0, 0, 1, 1, 1, 0, 0, 0, 0, 0), 0b1100011111u
#define PPUTLUTRAITS_0b0011011111u 223, (0, 0, 1, 1, 0, 1, 1, 1, 1, 1), 0b1100100000u
#define PPUTLUTRAITS_0b0011011110u 222, (0, 0, 1, 1, 0, 1, 1, 1, 1, 0), 0b1100100001u
#define PPUTLUTRAITS_0b0011011101u 221, (0, 0, 1, 1, 0, 1, 1, 1, 0, 1), 0b1100100010u
#define PPUTLUTRAITS_0b0011011100u 220, (0, 0, 1, 1, 0, 1, 1, 1, 0, 0), 0b1100100011u
#define PPUTLUTRAITS_0b0011011011u 219, (0, 0, 1, 1, 0, 1, 1, 0, 1, 1), 0b1100100100u
#define PPUTLUTRAITS_0b0011011010u 218, (0, 0, 1, 1, 0, 1, 1, 0, 1, 0), 0b1100100101u
#define PPUTLUTRAITS_0b0011011001u 217, (0, 0, 1, 1, 0, 1, 1, 0, 0, 1), 0b1100100110u
#define PPUTLUTRAITS_0b0011011000u 216, (0, 0, 1, 1, 0, 1, 1, 0, 0, 0), 0b1100100111u
#define PPUTLUTRAITS_0b0011010111u 215, (0, 0, 1, 1, 0, 1, 0, 1, 1, 1), 0b1100101000u
#define PPUTLUTRAITS_0b0011010110u 214, (0, 0, 1, 1, 0, 1, 0, 1, 1, 0), 0b1100101001u
#define PPUTLUTRAITS_0b0011010101u 213, (0, 0, 1, 1, 0, 1, 0, 1, 0, 1), 0b1100101010u
#define PPUTLUTRAITS_0b0011010100u 212, (0, 0, 1, 1, 0, 1, 0, 1, 0, 0), 0b1100101011u
#define PPUTLUTRAITS_0b0011010011u 211, (0, 0, 1, 1, 0, 1, 0, 0, 1, 1), 0b1100101100u
#define PPUTLUTRAITS_0b0011010010u 210, (0, 0, 1, 1, 0, 1, 0, 0, 1, 0), 0b1100101101u
#define PPUTLUTRAITS_0b0011010001u 209, (0, 0, 1, 1, 0, 1, 0, 0, 0, 1), 0b1100101110u
#define PPUTLUTRAITS_0b0011010000u 208, (0, 0, 1, 1, 0, 1, 0, 0, 0, 0), 0b1100101111u
#define PPUTLUTRAITS_0b0011001111u 207, (0, 0, 1, 1, 0, 0, 1, 1, 1, 1), 0b1100110000u
#define PPUTLUTRAITS_0b0011001110u 206, (0, 0, 1, 1, 0, 0, 1, 1, 1, 0), 0b1100110001u
#define PPUTLUTRAITS_0b0011001101u 205, (0, 0, 1, 1, 0, 0, 1, 1, 0, 1), 0b1100110010u
#define PPUTLUTRAITS_0b0011001100u 204, (0, 0, 1, 1, 0, 0, 1, 1, 0, 0), 0b1100110011u
#define PPUTLUTRAITS_0b0011001011u 203, (0, 0, 1, 1, 0, 0, 1, 0, 1, 1), 0b1100110100u
#define PPUTLUTRAITS_0b0011001010u 202, (0, 0, 1, 1, 0, 0, 1, 0, 1, 0), 0b1100110101u
#define PPUTLUTRAITS_0b0011001001u 201, (0, 0, 1, 1, 0, 0, 1, 0, 0, 1), 0b1100110110u
#define PPUTLUTRAITS_0b0011001000u 200, (0, 0, 1, 1, 0, 0, 1, 0, 0, 0), 0b1100110111u
#define PPUTLUTRAITS_0b0011000111u 199, (0, 0, 1, 1, 0, 0, 0, 1, 1, 1), 0b1100111000u
#define PPUTLUTRAITS_0b0011000110u 198, (0, 0, 1, 1, 0, 0, 0, 1, 1, 0), 0b1100111001u
#define PPUTLUTRAITS_0b0011000101u 197, (0, 0, 1, 1, 0, 0, 0, 1, 0, 1), 0b1100111010u
#define PPUTLUTRAITS_0b0011000100u 196, (0, 0, 1, 1, 0, 0, 0, 1, 0, 0), 0b1100111011u
#define PPUTLUTRAITS_0b0011000011u 195, (0, 0, 1, 1, 0, 0, 0, 0, 1, 1), 0b1100111100u
#define PPUTLUTRAITS_0b0011000010u 194, (0, 0, 1, 1, 0, 0, 0, 0, 1, 0), 0b1100111101u
#define PPUTLUTRAITS_0b0011000001u 193, (0, 0, 1, 1, 0, 0, 0, 0, 0, 1), 0b1100111110u
#define PPUTLUTRAITS_0b0011000000u 192, (0, 0, 1, 1, 0, 0, 0, 0, 0, 0), 0b1100111111u
#define PPUTLUTRAITS_0b0010111111u 191, (0, 0, 1, 0, 1, 1, 1, 1, 1, 1), 0b1101000000u
#define PPUTLUTRAITS_0b0010111110u 190, (0, 0, 1, 0, 1, 1, 1, 1, 1, 0), 0b1101000001u
#define PPUTLUTRAITS_0b0010111101u 189, (0, 0, 1, 0, 1, 1, 1, 1, 0, 1), 0b1101000010u
#define PPUTLUTRAITS_0b0010111100u 188, (0, 0, 1, 0, 1, 1, 1, 1, 0, 0), 0b1101000011u
#define PPUTLUTRAITS_0b0010111011u 187, (0, 0, 1, 0, 1, 1, 1, 0, 1, 1), 0b1101000100u
#define PPUTLUTRAITS_0b0010111010u 186, (0, 0, 1, 0, 1, 1, 1, 0, 1, 0), 0b1101000101u
#define PPUTLUTRAITS_0b0010111001u 185, (0, 0, 1, 0, 1, 1, 1, 0, 0, 1), 0b1101000110u
#define PPUTLUTRAITS_0b0010111000u 184, (0, 0, 1, 0, 1, 1, 1, 0, 0, 0), 0b1101000111u
#define PPUTLUTRAITS_0b0010110111u 183, (0, 0, 1, 0, 1, 1, 0, 1, 1, 1), 0b1101001000u
#define PPUTLUTRAITS_0b0010110110u 182, (0, 0, 1, 0, 1, 1, 0, 1, 1, 0), 0b1101001001u
#define PPUTLUTRAITS_0b0010110101u 181, (0, 0, 1, 0, 1, 1, 0, 1, 0, 1), 0b1101001010u
#define PPUTLUTRAITS_0b0010110100u 180, (0, 0, 1, 0, 1, 1, 0, 1, 0, 0), 0b1101001011u
#define PPUTLUTRAITS_0b0010110011u 179, (0, 0, 1, 0, 1, 1, 0, 0, 1, 1), 0b1101001100u
#define PPUTLUTRAITS_0b0010110010u 178, (0, 0, 1, 0, 1, 1, 0, 0, 1, 0), 0b1101001101u
#define PPUTLUTRAITS_0b0010110001u 177, (0, 0, 1, 0, 1, 1, 0, 0, 0, 1), 0b1101001110u
#define PPUTLUTRAITS_0b0010110000u 176, (0, 0, 1, 0, 1, 1, 0, 0, 0, 0), 0b1101001111u
#define PPUTLUTRAITS_0b0010101111u 175, (0, 0, 1, 0, 1, 0, 1, 1, 1, 1), 0b1101010000u
#define PPUTLUTRAITS_0b0010101110u 174, (0, 0, 1, 0, 1, 0, 1, 1, 1, 0), 0b1101010001u
#define PPUTLUTRAITS_0b0010101101u 173, (0, 0, 1, 0, 1, 0, 1, 1, 0, 1), 0b1101010010u
#define PPUTLUTRAITS_0b0010101100u 172, (0, 0, 1, 0, 1, 0, 1, 1, 0, 0), 0b1101010011u
#define PPUTLUTRAITS_0b0010101011u 171, (0, 0, 1, 0, 1, 0, 1, 0, 1, 1), 0b1101010100u
#define PPUTLUTRAITS_0b0010101010u 170, (0, 0, 1, 0, 1, 0, 1, 0, 1, 0), 0b1101010101u
#define PPUTLUTRAITS_0b0010101001u 169, (0, 0, 1, 0, 1, 0, 1, 0, 0, 1), 0b1101010110u
#define PPUTLUTRAITS_0b0010101000u 168, (0, 0, 1, 0, 1, 0, 1, 0, 0, 0), 0b1101010111u
#define PPUTLUTRAITS_0b0010100111u 167, (0, 0, 1, 0, 1, 0, 0, 1, 1, 1), 0b1101011000u
#define PPUTLUTRAITS_0b0010100110u 166, (0, 0, 1, 0, 1, 0, 0, 1, 1, 0), 0b1101011001u
#define PPUTLUTRAITS_0b0010100101u 165, (0, 0, 1, 0, 1, 0, 0, 1, 0, 1), 0b1101011010u
#define PPUTLUTRAITS_0b0010100100u 164, (0, 0, 1, 0, 1, 0, 0, 1, 0, 0), 0b1101011011u
#define PPUTLUTRAITS_0b0010100011u 163, (0, 0, 1, 0, 1, 0, 0, 0, 1, 1), 0b1101011100u
#define PPUTLUTRAITS_0b0010100010u 162, (0, 0, 1, 0, 1, 0, 0, 0, 1, 0), 0b1101011101u
#define PPUTLUTRAITS_0b0010100001u 161, (0, 0, 1, 0, 1, 0, 0, 0, 0, 1), 0b1101011110u
#define PPUTLUTRAITS_0b0010100000u 160, (0, 0, 1, 0, 1, 0, 0, 0, 0, 0), 0b1101011111u
#define PPUTLUTRAITS_0b0010011111u 159, (0, 0, 1, 0, 0, 1, 1, 1, 1, 1), 0b1101100000u
#define PPUTLUTRAITS_0b0010011110u 158, (0, 0, 1, 0, 0, 1, 1, 1, 1, 0), 0b1101100001u
#define PPUTLUTRAITS_0b0010011101u 157, (0, 0, 1, 0, 0, 1, 1, 1, 0, 1), 0b1101100010u
#define PPUTLUTRAITS_0b0010011100u 156, (0, 0, 1, 0, 0, 1, 1, 1, 0, 0), 0b1101100011u
#define PPUTLUTRAITS_0b0010011011u 155, (0, 0, 1, 0, 0, 1, 1, 0, 1, 1), 0b1101100100u
#define PPUTLUTRAITS_0b0010011010u 154, (0, 0, 1, 0, 0, 1, 1, 0, 1, 0), 0b1101100101u
#define PPUTLUTRAITS_0b0010011001u 153, (0, 0, 1, 0, 0, 1, 1, 0, 0, 1), 0b1101100110u
#define PPUTLUTRAITS_0b0010011000u 152, (0, 0, 1, 0, 0, 1, 1, 0, 0, 0), 0b1101100111u
#define PPUTLUTRAITS_0b0010010111u 151, (0, 0, 1, 0, 0, 1, 0, 1, 1, 1), 0b1101101000u
#define PPUTLUTRAITS_0b0010010110u 150, (0, 0, 1, 0, 0, 1, 0, 1, 1, 0), 0b1101101001u
#define PPUTLUTRAITS_0b0010010101u 149, (0, 0, 1, 0, 0, 1, 0, 1, 0, 1), 0b1101101010u
#define PPUTLUTRAITS_0b0010010100u 148, (0, 0, 1, 0, 0, 1, 0, 1, 0, 0), 0b1101101011u
#define PPUTLUTRAITS_0b0010010011u 147, (0, 0, 1, 0, 0, 1, 0, 0, 1, 1), 0b1101101100u
#define PPUTLUTRAITS_0b0010010010u 146, (0, 0, 1, 0, 0, 1, 0, 0, 1, 0), 0b1101101101u
#define PPUTLUTRAITS_0b0010010001u 145, (0, 0, 1, 0, 0, 1, 0, 0, 0, 1), 0b1101101110u
#define PPUTLUTRAITS_0b0010010000u 144, (0, 0, 1, 0, 0, 1, 0, 0, 0, 0), 0b1101101111u
#define PPUTLUTRAITS_0b0010001111u 143, (0, 0, 1, 0, 0, 0, 1, 1, 1, 1), 0b1101110000u
#define PPUTLUTRAITS_0b0010001110u 142, (0, 0, 1, 0, 0, 0, 1, 1, 1, 0), 0b1101110001u
#define PPUTLUTRAITS_0b0010001101u 141, (0, 0, 1, 0, 0, 0, 1, 1, 0, 1), 0b1101110010u
#define PPUTLUTRAITS_0b0010001100u 140, (0, 0, 1, 0, 0, 0, 1, 1, 0, 0), 0b1101110011u
#define PPUTLUTRAITS_0b0010001011u 139, (0, 0, 1, 0, 0, 0, 1, 0, 1, 1), 0b1101110100u
#define PPUTLUTRAITS_0b0010001010u 138, (0, 0, 1, 0, 0, 0, 1, 0, 1, 0), 0b1101110101u
#define PPUTLUTRAITS_0b0010001001u 137, (0, 0, 1, 0, 0, 0, 1, 0, 0, 1), 0b1101110110u
#define PPUTLUTRAITS_0b0010001000u 136, (0, 0, 1, 0, 0, 0, 1, 0, 0, 0), 0b1101110111u
#define PPUTLUTRAITS_0b0010000111u 135, (0, 0, 1, 0, 0, 0, 0, 1, 1, 1), 0b1101111000u
#define PPUTLUTRAITS_0b0010000110u 134, (0, 0, 1, 0, 0, 0, 0, 1, 1, 0), 0b1101111001u
#define PPUTLUTRAITS_0b0010000101u 133, (0, 0, 1, 0, 0, 0, 0, 1, 0, 1), 0b1101111010u
#define PPUTLUTRAITS_0b0010000100u 132, (0, 0, 1, 0, 0, 0, 0, 1, 0, 0), 0b1101111011u
#define PPUTLUTRAITS_0b0010000011u 131, (0, 0, 1, 0, 0, 0, 0, 0, 1, 1), 0b1101111100u
#define PPUTLUTRAITS_0b0010000010u 130, (0, 0, 1, 0, 0, 0, 0, 0, 1, 0), 0b1101111101u
#define PPUTLUTRAITS_0b0010000001u 129, (0, 0, 1, 0, 0, 0, 0, 0, 0, 1), 0b1101111110u
#define PPUTLUTRAITS_0b0010000000u 128, (0, 0, 1, 0, 0, 0, 0, 0, 0, 0), 0b1101111111u
#define PPUTLUTRAITS_0b0001111111u 127, (0, 0, 0, 1, 1, 1, 1, 1, 1, 1), 0b1110000000u
#define PPUTLUTRAITS_0b0001111110u 126, (0, 0, 0, 1, 1, 1, 1, 1, 1, 0), 0b1110000001u
#define PPUTLUTRAITS_0b0001111101u 125, (0, 0, 0, 1, 1, 1, 1, 1, 0, 1), 0b1110000010u
#define PPUTLUTRAITS_0b0001111100u 124, (0, 0, 0, 1, 1, 1, 1, 1, 0, 0), 0b1110000011u
#define PPUTLUTRAITS_0b0001111011u 123, (0, 0, 0, 1, 1, 1, 1, 0, 1, 1), 0b1110000100u
#define PPUTLUTRAITS_0b0001111010u 122, (0, 0, 0, 1, 1, 1, 1, 0, 1, 0), 0b1110000101u
#define PPUTLUTRAITS_0b0001111001u 121, (0, 0, 0, 1, 1, 1, 1, 0, 0, 1), 0b1110000110u
#define PPUTLUTRAITS_0b0001111000u 120, (0, 0, 0, 1, 1, 1, 1, 0, 0, 0), 0b1110000111u
#define PPUTLUTRAITS_0b0001110111u 119, (0, 0, 0, 1, 1, 1, 0, 1, 1, 1), 0b1110001000u
#define PPUTLUTRAITS_0b0001110110u 118, (0, 0, 0, 1, 1, 1, 0, 1, 1, 0), 0b1110001001u
#define PPUTLUTRAITS_0b0001110101u 117, (0, 0, 0, 1, 1, 1, 0, 1, 0, 1), 0b1110001010u
#define PPUTLUTRAITS_0b0001110100u 116, (0, 0, 0, 1, 1, 1, 0, 1, 0, 0), 0b1110001011u
#define PPUTLUTRAITS_0b0001110011u 115, (0, 0, 0, 1, 1, 1, 0, 0, 1, 1), 0b1110001100u
#define PPUTLUTRAITS_0b0001110010u 114, (0, 0, 0, 1, 1, 1, 0, 0, 1, 0), 0b1110001101u
#define PPUTLUTRAITS_0b0001110001u 113, (0, 0, 0, 1, 1, 1, 0, 0, 0, 1), 0b1110001110u
#define PPUTLUTRAITS_0b0001110000u 112, (0, 0, 0, 1, 1, 1, 0, 0, 0, 0), 0b1110001111u
#define PPUTLUTRAITS_0b0001101111u 111, (0, 0, 0, 1, 1, 0, 1, 1, 1, 1), 0b1110010000u
#define PPUTLUTRAITS_0b0001101110u 110, (0, 0, 0, 1, 1, 0, 1, 1, 1, 0), 0b1110010001u
#define PPUTLUTRAITS_0b0001101101u 109, (0, 0, 0, 1, 1, 0, 1, 1, 0, 1), 0b1110010010u
#define PPUTLUTRAITS_0b0001101100u 108, (0, 0, 0, 1, 1, 0, 1, 1, 0, 0), 0b1110010011u
#define PPUTLUTRAITS_0b0001101011u 107, (0, 0, 0, 1, 1, 0, 1, 0, 1, 1), 0b1110010100u
#define PPUTLUTRAITS_0b0001101010u 106, (0, 0, 0, 1, 1, 0, 1, 0, 1, 0), 0b1110010101u
#define PPUTLUTRAITS_0b0001101001u 105, (0, 0, 0, 1, 1, 0, 1, 0, 0, 1), 0b1110010110u
#define PPUTLUTRAITS_0b0001101000u 104, (0, 0, 0, 1, 1, 0, 1, 0, 0, 0), 0b1110010111u
#define PPUTLUTRAITS_0b0001100111u 103, (0, 0, 0, 1, 1, 0, 0, 1, 1, 1), 0b1110011000u
#define PPUTLUTRAITS_0b0001100110u 102, (0, 0, 0, 1, 1, 0, 0, 1, 1, 0), 0b1110011001u
#define PPUTLUTRAITS_0b0001100101u 101, (0, 0, 0, 1, 1, 0, 0, 1, 0, 1), 0b1110011010u
#define PPUTLUTRAITS_0b0001100100u 100, (0, 0, 0, 1, 1, 0, 0, 1, 0, 0), 0b1110011011u
#define PPUTLUTRAITS_0b0001100011u 99, (0, 0, 0, 1, 1, 0, 0, 0, 1, 1), 0b1110011100u
#define PPUTLUTRAITS_0b0001100010u 98, (0, 0, 0, 1, 1, 0, 0, 0, 1, 0), 0b1110011101u
#define PPUTLUTRAITS_0b0001100001u 97, (0, 0, 0, 1, 1, 0, 0, 0, 0, 1), 0b1110011110u
#define PPUTLUTRAITS_0b0001100000u 96, (0, 0, 0, 1, 1, 0, 0, 0, 0, 0), 0b1110011111u
#define PPUTLUTRAITS_0b0001011111u 95, (0, 0, 0, 1, 0, 1, 1, 1, 1, 1), 0b1110100000u
#define PPUTLUTRAITS_0b0001011110u 94, (0, 0, 0, 1, 0, 1, 1, 1, 1, 0), 0b1110100001u
#define PPUTLUTRAITS_0b0001011101u 93, (0, 0, 0, 1, 0, 1, 1, 1, 0, 1), 0b1110100010u
#define PPUTLUTRAITS_0b0001011100u 92, (0, 0, 0, 1, 0, 1, 1, 1, 0, 0), 0b1110100011u
#define PPUTLUTRAITS_0b0001011011u 91, (0, 0, 0, 1, 0, 1, 1, 0, 1, 1), 0b1110100100u
#define PPUTLUTRAITS_0b0001011010u 90, (0, 0, 0, 1, 0, 1, 1, 0, 1, 0), 0b1110100101u
#define PPUTLUTRAITS_0b0001011001u 89, (0, 0, 0, 1, 0, 1, 1, 0, 0, 1), 0b1110100110u
#define PPUTLUTRAITS_0b0001011000u 88, (0, 0, 0, 1, 0, 1, 1, 0, 0, 0), 0b1110100111u
#define PPUTLUTRAITS_0b0001010111u 87, (0, 0, 0, 1, 0, 1, 0, 1, 1, 1), 0b1110101000u
#define PPUTLUTRAITS_0b0001010110u 86, (0, 0, 0, 1, 0, 1, 0, 1, 1, 0), 0b1110101001u
#define PPUTLUTRAITS_0b0001010101u 85, (0, 0, 0, 1, 0, 1, 0, 1, 0, 1), 0b1110101010u
#define PPUTLUTRAITS_0b0001010100u 84, (0, 0, 0, 1, 0, 1, 0, 1, 0, 0), 0b1110101011u
#define PPUTLUTRAITS_0b0001010011u 83, (0, 0, 0, 1, 0, 1, 0, 0, 1, 1), 0b1110101100u
#define PPUTLUTRAITS_0b0001010010u 82, (0, 0, 0, 1, 0, 1, 0, 0, 1, 0), 0b1110101101u
#define PPUTLUTRAITS_0b0001010001u 81, (0, 0, 0, 1, 0, 1, 0, 0, 0, 1), 0b1110101110u
#define PPUTLUTRAITS_0b0001010000u 80, (0, 0, 0, 1, 0, 1, 0, 0, 0, 0), 0b1110101111u
#define PPUTLUTRAITS_0b0001001111u 79, (0, 0, 0, 1, 0, 0, 1, 1, 1, 1), 0b1110110000u
#define PPUTLUTRAITS_0b0001001110u 78, (0, 0, 0, 1, 0, 0, 1, 1, 1, 0), 0b1110110001u
#define PPUTLUTRAITS_0b0001001101u 77, (0, 0, 0, 1, 0, 0, 1, 1, 0, 1), 0b1110110010u
#define PPUTLUTRAITS_0b0001001100u 76, (0, 0, 0, 1, 0, 0, 1, 1, 0, 0), 0b1110110011u
#define PPUTLUTRAITS_0b0001001011u 75, (0, 0, 0, 1, 0, 0, 1, 0, 1, 1), 0b1110110100u
#define PPUTLUTRAITS_0b0001001010u 74, (0, 0, 0, 1, 0, 0, 1, 0, 1, 0), 0b1110110101u
#define PPUTLUTRAITS_0b0001001001u 73, (0, 0, 0, 1, 0, 0, 1, 0, 0, 1), 0b1110110110u
#define PPUTLUTRAITS_0b0001001000u 72, (0, 0, 0, 1, 0, 0, 1, 0, 0, 0), 0b1110110111u
#define PPUTLUTRAITS_0b0001000111u 71, (0, 0, 0, 1, 0, 0, 0, 1, 1, 1), 0b1110111000u
#define PPUTLUTRAITS_0b0001000110u 70, (0, 0, 0, 1, 0, 0, 0, 1, 1, 0), 0b1110111001u
#define PPUTLUTRAITS_0b0001000101u 69, (0, 0, 0, 1, 0, 0, 0, 1, 0, 1), 0b1110111010u
#define PPUTLUTRAITS_0b0001000100u 68, (0, 0, 0, 1, 0, 0, 0, 1, 0, 0), 0b1110111011u
#define PPUTLUTRAITS_0b0001000011u 67, (0, 0, 0, 1, 0, 0, 0, 0, 1, 1), 0b1110111100u
#define PPUTLUTRAITS_0b0001000010u 66, (0, 0, 0, 1, 0, 0, 0, 0, 1, 0), 0b1110111101u
#define PPUTLUTRAITS_0b0001000001u 65, (0, 0, 0, 1, 0, 0, 0, 0, 0, 1), 0b1110111110u
#define PPUTLUTRAITS_0b0001000000u 64, (0, 0, 0, 1, 0, 0, 0, 0, 0, 0), 0b1110111111u
#define PPUTLUTRAITS_0b0000111111u 63, (0, 0, 0, 0, 1, 1, 1, 1, 1, 1), 0b1111000000u
#define PPUTLUTRAITS_0b0000111110u 62, (0, 0, 0, 0, 1, 1, 1, 1, 1, 0), 0b1111000001u
#define PPUTLUTRAITS_0b0000111101u 61, (0, 0, 0, 0, 1, 1, 1, 1, 0, 1), 0b1111000010u
#define PPUTLUTRAITS_0b0000111100u 60, (0, 0, 0, 0, 1, 1, 1, 1, 0, 0), 0b1111000011u
#define PPUTLUTRAITS_0b0000111011u 59, (0, 0, 0, 0, 1, 1, 1, 0, 1, 1), 0b1111000100u
#define PPUTLUTRAITS_0b0000111010u 58, (0, 0, 0, 0, 1, 1, 1, 0, 1, 0), 0b1111000101u
#define PPUTLUTRAITS_0b0000111001u 57, (0, 0, 0, 0, 1, 1, 1, 0, 0, 1), 0b1111000110u
#define PPUTLUTRAITS_0b0000111000u 56, (0, 0, 0, 0, 1, 1, 1, 0, 0, 0), 0b1111000111u
#define PPUTLUTRAITS_0b0000110111u 55, (0, 0, 0, 0, 1, 1, 0, 1, 1, 1), 0b1111001000u
#define PPUTLUTRAITS_0b0000110110u 54, (0, 0, 0, 0, 1, 1, 0, 1, 1, 0), 0b1111001001u
#define PPUTLUTRAITS_0b0000110101u 53, (0, 0, 0, 0, 1, 1, 0, 1, 0, 1), 0b1111001010u
#define PPUTLUTRAITS_0b0000110100u 52, (0, 0, 0, 0, 1, 1, 0, 1, 0, 0), 0b1111001011u
#define PPUTLUTRAITS_0b0000110011u 51, (0, 0, 0, 0, 1, 1, 0, 0, 1, 1), 0b1111001100u
#define PPUTLUTRAITS_0b0000110010u 50, (0, 0, 0, 0, 1, 1, 0, 0, 1, 0), 0b1111001101u
#define PPUTLUTRAITS_0b0000110001u 49, (0, 0, 0, 0, 1, 1, 0, 0, 0, 1), 0b1111001110u
#define PPUTLUTRAITS_0b0000110000u 48, (0, 0, 0, 0, 1, 1, 0, 0, 0, 0), 0b1111001111u
#define PPUTLUTRAITS_0b0000101111u 47, (0, 0, 0, 0, 1, 0, 1, 1, 1, 1), 0b1111010000u
#define PPUTLUTRAITS_0b0000101110u 46, (0, 0, 0, 0, 1, 0, 1, 1, 1, 0), 0b1111010001u
#define PPUTLUTRAITS_0b0000101101u 45, (0, 0, 0, 0, 1, 0, 1, 1, 0, 1), 0b1111010010u
#define PPUTLUTRAITS_0b0000101100u 44, (0, 0, 0, 0, 1, 0, 1, 1, 0, 0), 0b1111010011u
#define PPUTLUTRAITS_0b0000101011u 43, (0, 0, 0, 0, 1, 0, 1, 0, 1, 1), 0b1111010100u
#define PPUTLUTRAITS_0b0000101010u 42, (0, 0, 0, 0, 1, 0, 1, 0, 1, 0), 0b1111010101u
#define PPUTLUTRAITS_0b0000101001u 41, (0, 0, 0, 0, 1, 0, 1, 0, 0, 1), 0b1111010110u
#define PPUTLUTRAITS_0b0000101000u 40, (0, 0, 0, 0, 1, 0, 1, 0, 0, 0), 0b1111010111u
#define PPUTLUTRAITS_0b0000100111u 39, (0, 0, 0, 0, 1, 0, 0, 1, 1, 1), 0b1111011000u
#define PPUTLUTRAITS_0b0000100110u 38, (0, 0, 0, 0, 1, 0, 0, 1, 1, 0), 0b1111011001u
#define PPUTLUTRAITS_0b0000100101u 37, (0, 0, 0, 0, 1, 0, 0, 1, 0, 1), 0b1111011010u
#define PPUTLUTRAITS_0b0000100100u 36, (0, 0, 0, 0, 1, 0, 0, 1, 0, 0), 0b1111011011u
#define PPUTLUTRAITS_0b0000100011u 35, (0, 0, 0, 0, 1, 0, 0, 0, 1, 1), 0b1111011100u
#define PPUTLUTRAITS_0b0000100010u 34, (0, 0, 0, 0, 1, 0, 0, 0, 1, 0), 0b1111011101u
#define PPUTLUTRAITS_0b0000100001u 33, (0, 0, 0, 0, 1, 0, 0, 0, 0, 1), 0b1111011110u
#define PPUTLUTRAITS_0b0000100000u 32, (0, 0, 0, 0, 1, 0, 0, 0, 0, 0), 0b1111011111u
#define PPUTLUTRAITS_0b0000011111u 31, (0, 0, 0, 0, 0, 1, 1, 1, 1, 1), 0b1111100000u
#define PPUTLUTRAITS_0b0000011110u 30, (0, 0, 0, 0, 0, 1, 1, 1, 1, 0), 0b1111100001u
#define PPUTLUTRAITS_0b0000011101u 29, (0, 0, 0, 0, 0, 1, 1, 1, 0, 1), 0b1111100010u
#define PPUTLUTRAITS_0b0000011100u 28, (0, 0, 0, 0, 0, 1, 1, 1, 0, 0), 0b1111100011u
#define PPUTLUTRAITS_0b0000011011u 27, (0, 0, 0, 0, 0, 1, 1, 0, 1, 1), 0b1111100100u
#define PPUTLUTRAITS_0b0000011010u 26, (0, 0, 0, 0, 0, 1, 1, 0, 1, 0), 0b1111100101u
#define PPUTLUTRAITS_0b0000011001u 25, (0, 0, 0, 0, 0, 1, 1, 0, 0, 1), 0b1111100110u
#define PPUTLUTRAITS_0b0000011000u 24, (0, 0, 0, 0, 0, 1, 1, 0, 0, 0), 0b1111100111u
#define PPUTLUTRAITS_0b0000010111u 23, (0, 0, 0, 0, 0, 1, 0, 1, 1, 1), 0b1111101000u
#define PPUTLUTRAITS_0b0000010110u 22, (0, 0, 0, 0, 0, 1, 0, 1, 1, 0), 0b1111101001u
#define PPUTLUTRAITS_0b0000010101u 21, (0, 0, 0, 0, 0, 1, 0, 1, 0, 1), 0b1111101010u
#define PPUTLUTRAITS_0b0000010100u 20, (0, 0, 0, 0, 0, 1, 0, 1, 0, 0), 0b1111101011u
#define PPUTLUTRAITS_0b0000010011u 19, (0, 0, 0, 0, 0, 1, 0, 0, 1, 1), 0b1111101100u
#define PPUTLUTRAITS_0b0000010010u 18, (0, 0, 0, 0, 0, 1, 0, 0, 1, 0), 0b1111101101u
#define PPUTLUTRAITS_0b0000010001u 17, (0, 0, 0, 0, 0, 1, 0, 0, 0, 1), 0b1111101110u
#define PPUTLUTRAITS_0b0000010000u 16, (0, 0, 0, 0, 0, 1, 0, 0, 0, 0), 0b1111101111u
#define PPUTLUTRAITS_0b0000001111u 15, (0, 0, 0, 0, 0, 0, 1, 1, 1, 1), 0b1111110000u
#define PPUTLUTRAITS_0b0000001110u 14, (0, 0, 0, 0, 0, 0, 1, 1, 1, 0), 0b1111110001u
#define PPUTLUTRAITS_0b0000001101u 13, (0, 0, 0, 0, 0, 0, 1, 1, 0, 1), 0b1111110010u
#define PPUTLUTRAITS_0b0000001100u 12, (0, 0, 0, 0, 0, 0, 1, 1, 0, 0), 0b1111110011u
#define PPUTLUTRAITS_0b0000001011u 11, (0, 0, 0, 0, 0, 0, 1, 0, 1, 1), 0b1111110100u
#define PPUTLUTRAITS_0b0000001010u 10, (0, 0, 0, 0, 0, 0, 1, 0, 1, 0), 0b1111110101u
#define PPUTLUTRAITS_0b0000001001u 9, (0, 0, 0, 0, 0, 0, 1, 0, 0, 1), 0b1111110110u
#define PPUTLUTRAITS_0b0000001000u 8, (0, 0, 0, 0, 0, 0, 1, 0, 0, 0), 0b1111110111u
#define PPUTLUTRAITS_0b0000000111u 7, (0, 0, 0, 0, 0, 0, 0, 1, 1, 1), 0b1111111000u
#define PPUTLUTRAITS_0b0000000110u 6, (0, 0, 0, 0, 0, 0, 0, 1, 1, 0), 0b1111111001u
#define PPUTLUTRAITS_0b0000000101u 5, (0, 0, 0, 0, 0, 0, 0, 1, 0, 1), 0b1111111010u
#define PPUTLUTRAITS_0b0000000100u 4, (0, 0, 0, 0, 0, 0, 0, 1, 0, 0), 0b1111111011u
#define PPUTLUTRAITS_0b0000000011u 3, (0, 0, 0, 0, 0, 0, 0, 0, 1, 1), 0b1111111100u
#define PPUTLUTRAITS_0b0000000010u 2, (0, 0, 0, 0, 0, 0, 0, 0, 1, 0), 0b1111111101u
#define PPUTLUTRAITS_0b0000000001u 1, (0, 0, 0, 0, 0, 0, 0, 0, 0, 1), 0b1111111110u
#define PPUTLUTRAITS_0b0000000000u 0, (0, 0, 0, 0, 0, 0, 0, 0, 0, 0), 0b1111111111u

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
/// binary bit length is fixed at PTL_UINT_BITS (10).
///
/// PTL_IS_UINT()              // 0
/// PTL_IS_UINT(foo)           // 0
/// PTL_IS_UINT(0)             // 1
/// PTL_IS_UINT(())            // 0
/// PTL_IS_UINT((), ())        // 0
/// PTL_IS_UINT(0, 1)          // 0
/// PTL_IS_UINT(1023)          // 1
/// PTL_IS_UINT(0b0000000000u) // 1
/// PTL_IS_UINT(0b1111111111u) // 1
/// PTL_IS_UINT(0b1111111111)  // 0
/// PTL_IS_UINT(0b110u)        // 0
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

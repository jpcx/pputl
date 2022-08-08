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
#define PTL_BUILD /* -> <c++ int> */ 20220808

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

/// type, decimal, bits, bitnot
#define PPUTLUTRAITS_0b1111111111u BIN, 1023, (1, 1, 1, 1, 1, 1, 1, 1, 1, 1), 0b0000000000u
#define PPUTLUTRAITS_0b1111111110u BIN, 1022, (1, 1, 1, 1, 1, 1, 1, 1, 1, 0), 0b0000000001u
#define PPUTLUTRAITS_0b1111111101u BIN, 1021, (1, 1, 1, 1, 1, 1, 1, 1, 0, 1), 0b0000000010u
#define PPUTLUTRAITS_0b1111111100u BIN, 1020, (1, 1, 1, 1, 1, 1, 1, 1, 0, 0), 0b0000000011u
#define PPUTLUTRAITS_0b1111111011u BIN, 1019, (1, 1, 1, 1, 1, 1, 1, 0, 1, 1), 0b0000000100u
#define PPUTLUTRAITS_0b1111111010u BIN, 1018, (1, 1, 1, 1, 1, 1, 1, 0, 1, 0), 0b0000000101u
#define PPUTLUTRAITS_0b1111111001u BIN, 1017, (1, 1, 1, 1, 1, 1, 1, 0, 0, 1), 0b0000000110u
#define PPUTLUTRAITS_0b1111111000u BIN, 1016, (1, 1, 1, 1, 1, 1, 1, 0, 0, 0), 0b0000000111u
#define PPUTLUTRAITS_0b1111110111u BIN, 1015, (1, 1, 1, 1, 1, 1, 0, 1, 1, 1), 0b0000001000u
#define PPUTLUTRAITS_0b1111110110u BIN, 1014, (1, 1, 1, 1, 1, 1, 0, 1, 1, 0), 0b0000001001u
#define PPUTLUTRAITS_0b1111110101u BIN, 1013, (1, 1, 1, 1, 1, 1, 0, 1, 0, 1), 0b0000001010u
#define PPUTLUTRAITS_0b1111110100u BIN, 1012, (1, 1, 1, 1, 1, 1, 0, 1, 0, 0), 0b0000001011u
#define PPUTLUTRAITS_0b1111110011u BIN, 1011, (1, 1, 1, 1, 1, 1, 0, 0, 1, 1), 0b0000001100u
#define PPUTLUTRAITS_0b1111110010u BIN, 1010, (1, 1, 1, 1, 1, 1, 0, 0, 1, 0), 0b0000001101u
#define PPUTLUTRAITS_0b1111110001u BIN, 1009, (1, 1, 1, 1, 1, 1, 0, 0, 0, 1), 0b0000001110u
#define PPUTLUTRAITS_0b1111110000u BIN, 1008, (1, 1, 1, 1, 1, 1, 0, 0, 0, 0), 0b0000001111u
#define PPUTLUTRAITS_0b1111101111u BIN, 1007, (1, 1, 1, 1, 1, 0, 1, 1, 1, 1), 0b0000010000u
#define PPUTLUTRAITS_0b1111101110u BIN, 1006, (1, 1, 1, 1, 1, 0, 1, 1, 1, 0), 0b0000010001u
#define PPUTLUTRAITS_0b1111101101u BIN, 1005, (1, 1, 1, 1, 1, 0, 1, 1, 0, 1), 0b0000010010u
#define PPUTLUTRAITS_0b1111101100u BIN, 1004, (1, 1, 1, 1, 1, 0, 1, 1, 0, 0), 0b0000010011u
#define PPUTLUTRAITS_0b1111101011u BIN, 1003, (1, 1, 1, 1, 1, 0, 1, 0, 1, 1), 0b0000010100u
#define PPUTLUTRAITS_0b1111101010u BIN, 1002, (1, 1, 1, 1, 1, 0, 1, 0, 1, 0), 0b0000010101u
#define PPUTLUTRAITS_0b1111101001u BIN, 1001, (1, 1, 1, 1, 1, 0, 1, 0, 0, 1), 0b0000010110u
#define PPUTLUTRAITS_0b1111101000u BIN, 1000, (1, 1, 1, 1, 1, 0, 1, 0, 0, 0), 0b0000010111u
#define PPUTLUTRAITS_0b1111100111u BIN, 999, (1, 1, 1, 1, 1, 0, 0, 1, 1, 1), 0b0000011000u
#define PPUTLUTRAITS_0b1111100110u BIN, 998, (1, 1, 1, 1, 1, 0, 0, 1, 1, 0), 0b0000011001u
#define PPUTLUTRAITS_0b1111100101u BIN, 997, (1, 1, 1, 1, 1, 0, 0, 1, 0, 1), 0b0000011010u
#define PPUTLUTRAITS_0b1111100100u BIN, 996, (1, 1, 1, 1, 1, 0, 0, 1, 0, 0), 0b0000011011u
#define PPUTLUTRAITS_0b1111100011u BIN, 995, (1, 1, 1, 1, 1, 0, 0, 0, 1, 1), 0b0000011100u
#define PPUTLUTRAITS_0b1111100010u BIN, 994, (1, 1, 1, 1, 1, 0, 0, 0, 1, 0), 0b0000011101u
#define PPUTLUTRAITS_0b1111100001u BIN, 993, (1, 1, 1, 1, 1, 0, 0, 0, 0, 1), 0b0000011110u
#define PPUTLUTRAITS_0b1111100000u BIN, 992, (1, 1, 1, 1, 1, 0, 0, 0, 0, 0), 0b0000011111u
#define PPUTLUTRAITS_0b1111011111u BIN, 991, (1, 1, 1, 1, 0, 1, 1, 1, 1, 1), 0b0000100000u
#define PPUTLUTRAITS_0b1111011110u BIN, 990, (1, 1, 1, 1, 0, 1, 1, 1, 1, 0), 0b0000100001u
#define PPUTLUTRAITS_0b1111011101u BIN, 989, (1, 1, 1, 1, 0, 1, 1, 1, 0, 1), 0b0000100010u
#define PPUTLUTRAITS_0b1111011100u BIN, 988, (1, 1, 1, 1, 0, 1, 1, 1, 0, 0), 0b0000100011u
#define PPUTLUTRAITS_0b1111011011u BIN, 987, (1, 1, 1, 1, 0, 1, 1, 0, 1, 1), 0b0000100100u
#define PPUTLUTRAITS_0b1111011010u BIN, 986, (1, 1, 1, 1, 0, 1, 1, 0, 1, 0), 0b0000100101u
#define PPUTLUTRAITS_0b1111011001u BIN, 985, (1, 1, 1, 1, 0, 1, 1, 0, 0, 1), 0b0000100110u
#define PPUTLUTRAITS_0b1111011000u BIN, 984, (1, 1, 1, 1, 0, 1, 1, 0, 0, 0), 0b0000100111u
#define PPUTLUTRAITS_0b1111010111u BIN, 983, (1, 1, 1, 1, 0, 1, 0, 1, 1, 1), 0b0000101000u
#define PPUTLUTRAITS_0b1111010110u BIN, 982, (1, 1, 1, 1, 0, 1, 0, 1, 1, 0), 0b0000101001u
#define PPUTLUTRAITS_0b1111010101u BIN, 981, (1, 1, 1, 1, 0, 1, 0, 1, 0, 1), 0b0000101010u
#define PPUTLUTRAITS_0b1111010100u BIN, 980, (1, 1, 1, 1, 0, 1, 0, 1, 0, 0), 0b0000101011u
#define PPUTLUTRAITS_0b1111010011u BIN, 979, (1, 1, 1, 1, 0, 1, 0, 0, 1, 1), 0b0000101100u
#define PPUTLUTRAITS_0b1111010010u BIN, 978, (1, 1, 1, 1, 0, 1, 0, 0, 1, 0), 0b0000101101u
#define PPUTLUTRAITS_0b1111010001u BIN, 977, (1, 1, 1, 1, 0, 1, 0, 0, 0, 1), 0b0000101110u
#define PPUTLUTRAITS_0b1111010000u BIN, 976, (1, 1, 1, 1, 0, 1, 0, 0, 0, 0), 0b0000101111u
#define PPUTLUTRAITS_0b1111001111u BIN, 975, (1, 1, 1, 1, 0, 0, 1, 1, 1, 1), 0b0000110000u
#define PPUTLUTRAITS_0b1111001110u BIN, 974, (1, 1, 1, 1, 0, 0, 1, 1, 1, 0), 0b0000110001u
#define PPUTLUTRAITS_0b1111001101u BIN, 973, (1, 1, 1, 1, 0, 0, 1, 1, 0, 1), 0b0000110010u
#define PPUTLUTRAITS_0b1111001100u BIN, 972, (1, 1, 1, 1, 0, 0, 1, 1, 0, 0), 0b0000110011u
#define PPUTLUTRAITS_0b1111001011u BIN, 971, (1, 1, 1, 1, 0, 0, 1, 0, 1, 1), 0b0000110100u
#define PPUTLUTRAITS_0b1111001010u BIN, 970, (1, 1, 1, 1, 0, 0, 1, 0, 1, 0), 0b0000110101u
#define PPUTLUTRAITS_0b1111001001u BIN, 969, (1, 1, 1, 1, 0, 0, 1, 0, 0, 1), 0b0000110110u
#define PPUTLUTRAITS_0b1111001000u BIN, 968, (1, 1, 1, 1, 0, 0, 1, 0, 0, 0), 0b0000110111u
#define PPUTLUTRAITS_0b1111000111u BIN, 967, (1, 1, 1, 1, 0, 0, 0, 1, 1, 1), 0b0000111000u
#define PPUTLUTRAITS_0b1111000110u BIN, 966, (1, 1, 1, 1, 0, 0, 0, 1, 1, 0), 0b0000111001u
#define PPUTLUTRAITS_0b1111000101u BIN, 965, (1, 1, 1, 1, 0, 0, 0, 1, 0, 1), 0b0000111010u
#define PPUTLUTRAITS_0b1111000100u BIN, 964, (1, 1, 1, 1, 0, 0, 0, 1, 0, 0), 0b0000111011u
#define PPUTLUTRAITS_0b1111000011u BIN, 963, (1, 1, 1, 1, 0, 0, 0, 0, 1, 1), 0b0000111100u
#define PPUTLUTRAITS_0b1111000010u BIN, 962, (1, 1, 1, 1, 0, 0, 0, 0, 1, 0), 0b0000111101u
#define PPUTLUTRAITS_0b1111000001u BIN, 961, (1, 1, 1, 1, 0, 0, 0, 0, 0, 1), 0b0000111110u
#define PPUTLUTRAITS_0b1111000000u BIN, 960, (1, 1, 1, 1, 0, 0, 0, 0, 0, 0), 0b0000111111u
#define PPUTLUTRAITS_0b1110111111u BIN, 959, (1, 1, 1, 0, 1, 1, 1, 1, 1, 1), 0b0001000000u
#define PPUTLUTRAITS_0b1110111110u BIN, 958, (1, 1, 1, 0, 1, 1, 1, 1, 1, 0), 0b0001000001u
#define PPUTLUTRAITS_0b1110111101u BIN, 957, (1, 1, 1, 0, 1, 1, 1, 1, 0, 1), 0b0001000010u
#define PPUTLUTRAITS_0b1110111100u BIN, 956, (1, 1, 1, 0, 1, 1, 1, 1, 0, 0), 0b0001000011u
#define PPUTLUTRAITS_0b1110111011u BIN, 955, (1, 1, 1, 0, 1, 1, 1, 0, 1, 1), 0b0001000100u
#define PPUTLUTRAITS_0b1110111010u BIN, 954, (1, 1, 1, 0, 1, 1, 1, 0, 1, 0), 0b0001000101u
#define PPUTLUTRAITS_0b1110111001u BIN, 953, (1, 1, 1, 0, 1, 1, 1, 0, 0, 1), 0b0001000110u
#define PPUTLUTRAITS_0b1110111000u BIN, 952, (1, 1, 1, 0, 1, 1, 1, 0, 0, 0), 0b0001000111u
#define PPUTLUTRAITS_0b1110110111u BIN, 951, (1, 1, 1, 0, 1, 1, 0, 1, 1, 1), 0b0001001000u
#define PPUTLUTRAITS_0b1110110110u BIN, 950, (1, 1, 1, 0, 1, 1, 0, 1, 1, 0), 0b0001001001u
#define PPUTLUTRAITS_0b1110110101u BIN, 949, (1, 1, 1, 0, 1, 1, 0, 1, 0, 1), 0b0001001010u
#define PPUTLUTRAITS_0b1110110100u BIN, 948, (1, 1, 1, 0, 1, 1, 0, 1, 0, 0), 0b0001001011u
#define PPUTLUTRAITS_0b1110110011u BIN, 947, (1, 1, 1, 0, 1, 1, 0, 0, 1, 1), 0b0001001100u
#define PPUTLUTRAITS_0b1110110010u BIN, 946, (1, 1, 1, 0, 1, 1, 0, 0, 1, 0), 0b0001001101u
#define PPUTLUTRAITS_0b1110110001u BIN, 945, (1, 1, 1, 0, 1, 1, 0, 0, 0, 1), 0b0001001110u
#define PPUTLUTRAITS_0b1110110000u BIN, 944, (1, 1, 1, 0, 1, 1, 0, 0, 0, 0), 0b0001001111u
#define PPUTLUTRAITS_0b1110101111u BIN, 943, (1, 1, 1, 0, 1, 0, 1, 1, 1, 1), 0b0001010000u
#define PPUTLUTRAITS_0b1110101110u BIN, 942, (1, 1, 1, 0, 1, 0, 1, 1, 1, 0), 0b0001010001u
#define PPUTLUTRAITS_0b1110101101u BIN, 941, (1, 1, 1, 0, 1, 0, 1, 1, 0, 1), 0b0001010010u
#define PPUTLUTRAITS_0b1110101100u BIN, 940, (1, 1, 1, 0, 1, 0, 1, 1, 0, 0), 0b0001010011u
#define PPUTLUTRAITS_0b1110101011u BIN, 939, (1, 1, 1, 0, 1, 0, 1, 0, 1, 1), 0b0001010100u
#define PPUTLUTRAITS_0b1110101010u BIN, 938, (1, 1, 1, 0, 1, 0, 1, 0, 1, 0), 0b0001010101u
#define PPUTLUTRAITS_0b1110101001u BIN, 937, (1, 1, 1, 0, 1, 0, 1, 0, 0, 1), 0b0001010110u
#define PPUTLUTRAITS_0b1110101000u BIN, 936, (1, 1, 1, 0, 1, 0, 1, 0, 0, 0), 0b0001010111u
#define PPUTLUTRAITS_0b1110100111u BIN, 935, (1, 1, 1, 0, 1, 0, 0, 1, 1, 1), 0b0001011000u
#define PPUTLUTRAITS_0b1110100110u BIN, 934, (1, 1, 1, 0, 1, 0, 0, 1, 1, 0), 0b0001011001u
#define PPUTLUTRAITS_0b1110100101u BIN, 933, (1, 1, 1, 0, 1, 0, 0, 1, 0, 1), 0b0001011010u
#define PPUTLUTRAITS_0b1110100100u BIN, 932, (1, 1, 1, 0, 1, 0, 0, 1, 0, 0), 0b0001011011u
#define PPUTLUTRAITS_0b1110100011u BIN, 931, (1, 1, 1, 0, 1, 0, 0, 0, 1, 1), 0b0001011100u
#define PPUTLUTRAITS_0b1110100010u BIN, 930, (1, 1, 1, 0, 1, 0, 0, 0, 1, 0), 0b0001011101u
#define PPUTLUTRAITS_0b1110100001u BIN, 929, (1, 1, 1, 0, 1, 0, 0, 0, 0, 1), 0b0001011110u
#define PPUTLUTRAITS_0b1110100000u BIN, 928, (1, 1, 1, 0, 1, 0, 0, 0, 0, 0), 0b0001011111u
#define PPUTLUTRAITS_0b1110011111u BIN, 927, (1, 1, 1, 0, 0, 1, 1, 1, 1, 1), 0b0001100000u
#define PPUTLUTRAITS_0b1110011110u BIN, 926, (1, 1, 1, 0, 0, 1, 1, 1, 1, 0), 0b0001100001u
#define PPUTLUTRAITS_0b1110011101u BIN, 925, (1, 1, 1, 0, 0, 1, 1, 1, 0, 1), 0b0001100010u
#define PPUTLUTRAITS_0b1110011100u BIN, 924, (1, 1, 1, 0, 0, 1, 1, 1, 0, 0), 0b0001100011u
#define PPUTLUTRAITS_0b1110011011u BIN, 923, (1, 1, 1, 0, 0, 1, 1, 0, 1, 1), 0b0001100100u
#define PPUTLUTRAITS_0b1110011010u BIN, 922, (1, 1, 1, 0, 0, 1, 1, 0, 1, 0), 0b0001100101u
#define PPUTLUTRAITS_0b1110011001u BIN, 921, (1, 1, 1, 0, 0, 1, 1, 0, 0, 1), 0b0001100110u
#define PPUTLUTRAITS_0b1110011000u BIN, 920, (1, 1, 1, 0, 0, 1, 1, 0, 0, 0), 0b0001100111u
#define PPUTLUTRAITS_0b1110010111u BIN, 919, (1, 1, 1, 0, 0, 1, 0, 1, 1, 1), 0b0001101000u
#define PPUTLUTRAITS_0b1110010110u BIN, 918, (1, 1, 1, 0, 0, 1, 0, 1, 1, 0), 0b0001101001u
#define PPUTLUTRAITS_0b1110010101u BIN, 917, (1, 1, 1, 0, 0, 1, 0, 1, 0, 1), 0b0001101010u
#define PPUTLUTRAITS_0b1110010100u BIN, 916, (1, 1, 1, 0, 0, 1, 0, 1, 0, 0), 0b0001101011u
#define PPUTLUTRAITS_0b1110010011u BIN, 915, (1, 1, 1, 0, 0, 1, 0, 0, 1, 1), 0b0001101100u
#define PPUTLUTRAITS_0b1110010010u BIN, 914, (1, 1, 1, 0, 0, 1, 0, 0, 1, 0), 0b0001101101u
#define PPUTLUTRAITS_0b1110010001u BIN, 913, (1, 1, 1, 0, 0, 1, 0, 0, 0, 1), 0b0001101110u
#define PPUTLUTRAITS_0b1110010000u BIN, 912, (1, 1, 1, 0, 0, 1, 0, 0, 0, 0), 0b0001101111u
#define PPUTLUTRAITS_0b1110001111u BIN, 911, (1, 1, 1, 0, 0, 0, 1, 1, 1, 1), 0b0001110000u
#define PPUTLUTRAITS_0b1110001110u BIN, 910, (1, 1, 1, 0, 0, 0, 1, 1, 1, 0), 0b0001110001u
#define PPUTLUTRAITS_0b1110001101u BIN, 909, (1, 1, 1, 0, 0, 0, 1, 1, 0, 1), 0b0001110010u
#define PPUTLUTRAITS_0b1110001100u BIN, 908, (1, 1, 1, 0, 0, 0, 1, 1, 0, 0), 0b0001110011u
#define PPUTLUTRAITS_0b1110001011u BIN, 907, (1, 1, 1, 0, 0, 0, 1, 0, 1, 1), 0b0001110100u
#define PPUTLUTRAITS_0b1110001010u BIN, 906, (1, 1, 1, 0, 0, 0, 1, 0, 1, 0), 0b0001110101u
#define PPUTLUTRAITS_0b1110001001u BIN, 905, (1, 1, 1, 0, 0, 0, 1, 0, 0, 1), 0b0001110110u
#define PPUTLUTRAITS_0b1110001000u BIN, 904, (1, 1, 1, 0, 0, 0, 1, 0, 0, 0), 0b0001110111u
#define PPUTLUTRAITS_0b1110000111u BIN, 903, (1, 1, 1, 0, 0, 0, 0, 1, 1, 1), 0b0001111000u
#define PPUTLUTRAITS_0b1110000110u BIN, 902, (1, 1, 1, 0, 0, 0, 0, 1, 1, 0), 0b0001111001u
#define PPUTLUTRAITS_0b1110000101u BIN, 901, (1, 1, 1, 0, 0, 0, 0, 1, 0, 1), 0b0001111010u
#define PPUTLUTRAITS_0b1110000100u BIN, 900, (1, 1, 1, 0, 0, 0, 0, 1, 0, 0), 0b0001111011u
#define PPUTLUTRAITS_0b1110000011u BIN, 899, (1, 1, 1, 0, 0, 0, 0, 0, 1, 1), 0b0001111100u
#define PPUTLUTRAITS_0b1110000010u BIN, 898, (1, 1, 1, 0, 0, 0, 0, 0, 1, 0), 0b0001111101u
#define PPUTLUTRAITS_0b1110000001u BIN, 897, (1, 1, 1, 0, 0, 0, 0, 0, 0, 1), 0b0001111110u
#define PPUTLUTRAITS_0b1110000000u BIN, 896, (1, 1, 1, 0, 0, 0, 0, 0, 0, 0), 0b0001111111u
#define PPUTLUTRAITS_0b1101111111u BIN, 895, (1, 1, 0, 1, 1, 1, 1, 1, 1, 1), 0b0010000000u
#define PPUTLUTRAITS_0b1101111110u BIN, 894, (1, 1, 0, 1, 1, 1, 1, 1, 1, 0), 0b0010000001u
#define PPUTLUTRAITS_0b1101111101u BIN, 893, (1, 1, 0, 1, 1, 1, 1, 1, 0, 1), 0b0010000010u
#define PPUTLUTRAITS_0b1101111100u BIN, 892, (1, 1, 0, 1, 1, 1, 1, 1, 0, 0), 0b0010000011u
#define PPUTLUTRAITS_0b1101111011u BIN, 891, (1, 1, 0, 1, 1, 1, 1, 0, 1, 1), 0b0010000100u
#define PPUTLUTRAITS_0b1101111010u BIN, 890, (1, 1, 0, 1, 1, 1, 1, 0, 1, 0), 0b0010000101u
#define PPUTLUTRAITS_0b1101111001u BIN, 889, (1, 1, 0, 1, 1, 1, 1, 0, 0, 1), 0b0010000110u
#define PPUTLUTRAITS_0b1101111000u BIN, 888, (1, 1, 0, 1, 1, 1, 1, 0, 0, 0), 0b0010000111u
#define PPUTLUTRAITS_0b1101110111u BIN, 887, (1, 1, 0, 1, 1, 1, 0, 1, 1, 1), 0b0010001000u
#define PPUTLUTRAITS_0b1101110110u BIN, 886, (1, 1, 0, 1, 1, 1, 0, 1, 1, 0), 0b0010001001u
#define PPUTLUTRAITS_0b1101110101u BIN, 885, (1, 1, 0, 1, 1, 1, 0, 1, 0, 1), 0b0010001010u
#define PPUTLUTRAITS_0b1101110100u BIN, 884, (1, 1, 0, 1, 1, 1, 0, 1, 0, 0), 0b0010001011u
#define PPUTLUTRAITS_0b1101110011u BIN, 883, (1, 1, 0, 1, 1, 1, 0, 0, 1, 1), 0b0010001100u
#define PPUTLUTRAITS_0b1101110010u BIN, 882, (1, 1, 0, 1, 1, 1, 0, 0, 1, 0), 0b0010001101u
#define PPUTLUTRAITS_0b1101110001u BIN, 881, (1, 1, 0, 1, 1, 1, 0, 0, 0, 1), 0b0010001110u
#define PPUTLUTRAITS_0b1101110000u BIN, 880, (1, 1, 0, 1, 1, 1, 0, 0, 0, 0), 0b0010001111u
#define PPUTLUTRAITS_0b1101101111u BIN, 879, (1, 1, 0, 1, 1, 0, 1, 1, 1, 1), 0b0010010000u
#define PPUTLUTRAITS_0b1101101110u BIN, 878, (1, 1, 0, 1, 1, 0, 1, 1, 1, 0), 0b0010010001u
#define PPUTLUTRAITS_0b1101101101u BIN, 877, (1, 1, 0, 1, 1, 0, 1, 1, 0, 1), 0b0010010010u
#define PPUTLUTRAITS_0b1101101100u BIN, 876, (1, 1, 0, 1, 1, 0, 1, 1, 0, 0), 0b0010010011u
#define PPUTLUTRAITS_0b1101101011u BIN, 875, (1, 1, 0, 1, 1, 0, 1, 0, 1, 1), 0b0010010100u
#define PPUTLUTRAITS_0b1101101010u BIN, 874, (1, 1, 0, 1, 1, 0, 1, 0, 1, 0), 0b0010010101u
#define PPUTLUTRAITS_0b1101101001u BIN, 873, (1, 1, 0, 1, 1, 0, 1, 0, 0, 1), 0b0010010110u
#define PPUTLUTRAITS_0b1101101000u BIN, 872, (1, 1, 0, 1, 1, 0, 1, 0, 0, 0), 0b0010010111u
#define PPUTLUTRAITS_0b1101100111u BIN, 871, (1, 1, 0, 1, 1, 0, 0, 1, 1, 1), 0b0010011000u
#define PPUTLUTRAITS_0b1101100110u BIN, 870, (1, 1, 0, 1, 1, 0, 0, 1, 1, 0), 0b0010011001u
#define PPUTLUTRAITS_0b1101100101u BIN, 869, (1, 1, 0, 1, 1, 0, 0, 1, 0, 1), 0b0010011010u
#define PPUTLUTRAITS_0b1101100100u BIN, 868, (1, 1, 0, 1, 1, 0, 0, 1, 0, 0), 0b0010011011u
#define PPUTLUTRAITS_0b1101100011u BIN, 867, (1, 1, 0, 1, 1, 0, 0, 0, 1, 1), 0b0010011100u
#define PPUTLUTRAITS_0b1101100010u BIN, 866, (1, 1, 0, 1, 1, 0, 0, 0, 1, 0), 0b0010011101u
#define PPUTLUTRAITS_0b1101100001u BIN, 865, (1, 1, 0, 1, 1, 0, 0, 0, 0, 1), 0b0010011110u
#define PPUTLUTRAITS_0b1101100000u BIN, 864, (1, 1, 0, 1, 1, 0, 0, 0, 0, 0), 0b0010011111u
#define PPUTLUTRAITS_0b1101011111u BIN, 863, (1, 1, 0, 1, 0, 1, 1, 1, 1, 1), 0b0010100000u
#define PPUTLUTRAITS_0b1101011110u BIN, 862, (1, 1, 0, 1, 0, 1, 1, 1, 1, 0), 0b0010100001u
#define PPUTLUTRAITS_0b1101011101u BIN, 861, (1, 1, 0, 1, 0, 1, 1, 1, 0, 1), 0b0010100010u
#define PPUTLUTRAITS_0b1101011100u BIN, 860, (1, 1, 0, 1, 0, 1, 1, 1, 0, 0), 0b0010100011u
#define PPUTLUTRAITS_0b1101011011u BIN, 859, (1, 1, 0, 1, 0, 1, 1, 0, 1, 1), 0b0010100100u
#define PPUTLUTRAITS_0b1101011010u BIN, 858, (1, 1, 0, 1, 0, 1, 1, 0, 1, 0), 0b0010100101u
#define PPUTLUTRAITS_0b1101011001u BIN, 857, (1, 1, 0, 1, 0, 1, 1, 0, 0, 1), 0b0010100110u
#define PPUTLUTRAITS_0b1101011000u BIN, 856, (1, 1, 0, 1, 0, 1, 1, 0, 0, 0), 0b0010100111u
#define PPUTLUTRAITS_0b1101010111u BIN, 855, (1, 1, 0, 1, 0, 1, 0, 1, 1, 1), 0b0010101000u
#define PPUTLUTRAITS_0b1101010110u BIN, 854, (1, 1, 0, 1, 0, 1, 0, 1, 1, 0), 0b0010101001u
#define PPUTLUTRAITS_0b1101010101u BIN, 853, (1, 1, 0, 1, 0, 1, 0, 1, 0, 1), 0b0010101010u
#define PPUTLUTRAITS_0b1101010100u BIN, 852, (1, 1, 0, 1, 0, 1, 0, 1, 0, 0), 0b0010101011u
#define PPUTLUTRAITS_0b1101010011u BIN, 851, (1, 1, 0, 1, 0, 1, 0, 0, 1, 1), 0b0010101100u
#define PPUTLUTRAITS_0b1101010010u BIN, 850, (1, 1, 0, 1, 0, 1, 0, 0, 1, 0), 0b0010101101u
#define PPUTLUTRAITS_0b1101010001u BIN, 849, (1, 1, 0, 1, 0, 1, 0, 0, 0, 1), 0b0010101110u
#define PPUTLUTRAITS_0b1101010000u BIN, 848, (1, 1, 0, 1, 0, 1, 0, 0, 0, 0), 0b0010101111u
#define PPUTLUTRAITS_0b1101001111u BIN, 847, (1, 1, 0, 1, 0, 0, 1, 1, 1, 1), 0b0010110000u
#define PPUTLUTRAITS_0b1101001110u BIN, 846, (1, 1, 0, 1, 0, 0, 1, 1, 1, 0), 0b0010110001u
#define PPUTLUTRAITS_0b1101001101u BIN, 845, (1, 1, 0, 1, 0, 0, 1, 1, 0, 1), 0b0010110010u
#define PPUTLUTRAITS_0b1101001100u BIN, 844, (1, 1, 0, 1, 0, 0, 1, 1, 0, 0), 0b0010110011u
#define PPUTLUTRAITS_0b1101001011u BIN, 843, (1, 1, 0, 1, 0, 0, 1, 0, 1, 1), 0b0010110100u
#define PPUTLUTRAITS_0b1101001010u BIN, 842, (1, 1, 0, 1, 0, 0, 1, 0, 1, 0), 0b0010110101u
#define PPUTLUTRAITS_0b1101001001u BIN, 841, (1, 1, 0, 1, 0, 0, 1, 0, 0, 1), 0b0010110110u
#define PPUTLUTRAITS_0b1101001000u BIN, 840, (1, 1, 0, 1, 0, 0, 1, 0, 0, 0), 0b0010110111u
#define PPUTLUTRAITS_0b1101000111u BIN, 839, (1, 1, 0, 1, 0, 0, 0, 1, 1, 1), 0b0010111000u
#define PPUTLUTRAITS_0b1101000110u BIN, 838, (1, 1, 0, 1, 0, 0, 0, 1, 1, 0), 0b0010111001u
#define PPUTLUTRAITS_0b1101000101u BIN, 837, (1, 1, 0, 1, 0, 0, 0, 1, 0, 1), 0b0010111010u
#define PPUTLUTRAITS_0b1101000100u BIN, 836, (1, 1, 0, 1, 0, 0, 0, 1, 0, 0), 0b0010111011u
#define PPUTLUTRAITS_0b1101000011u BIN, 835, (1, 1, 0, 1, 0, 0, 0, 0, 1, 1), 0b0010111100u
#define PPUTLUTRAITS_0b1101000010u BIN, 834, (1, 1, 0, 1, 0, 0, 0, 0, 1, 0), 0b0010111101u
#define PPUTLUTRAITS_0b1101000001u BIN, 833, (1, 1, 0, 1, 0, 0, 0, 0, 0, 1), 0b0010111110u
#define PPUTLUTRAITS_0b1101000000u BIN, 832, (1, 1, 0, 1, 0, 0, 0, 0, 0, 0), 0b0010111111u
#define PPUTLUTRAITS_0b1100111111u BIN, 831, (1, 1, 0, 0, 1, 1, 1, 1, 1, 1), 0b0011000000u
#define PPUTLUTRAITS_0b1100111110u BIN, 830, (1, 1, 0, 0, 1, 1, 1, 1, 1, 0), 0b0011000001u
#define PPUTLUTRAITS_0b1100111101u BIN, 829, (1, 1, 0, 0, 1, 1, 1, 1, 0, 1), 0b0011000010u
#define PPUTLUTRAITS_0b1100111100u BIN, 828, (1, 1, 0, 0, 1, 1, 1, 1, 0, 0), 0b0011000011u
#define PPUTLUTRAITS_0b1100111011u BIN, 827, (1, 1, 0, 0, 1, 1, 1, 0, 1, 1), 0b0011000100u
#define PPUTLUTRAITS_0b1100111010u BIN, 826, (1, 1, 0, 0, 1, 1, 1, 0, 1, 0), 0b0011000101u
#define PPUTLUTRAITS_0b1100111001u BIN, 825, (1, 1, 0, 0, 1, 1, 1, 0, 0, 1), 0b0011000110u
#define PPUTLUTRAITS_0b1100111000u BIN, 824, (1, 1, 0, 0, 1, 1, 1, 0, 0, 0), 0b0011000111u
#define PPUTLUTRAITS_0b1100110111u BIN, 823, (1, 1, 0, 0, 1, 1, 0, 1, 1, 1), 0b0011001000u
#define PPUTLUTRAITS_0b1100110110u BIN, 822, (1, 1, 0, 0, 1, 1, 0, 1, 1, 0), 0b0011001001u
#define PPUTLUTRAITS_0b1100110101u BIN, 821, (1, 1, 0, 0, 1, 1, 0, 1, 0, 1), 0b0011001010u
#define PPUTLUTRAITS_0b1100110100u BIN, 820, (1, 1, 0, 0, 1, 1, 0, 1, 0, 0), 0b0011001011u
#define PPUTLUTRAITS_0b1100110011u BIN, 819, (1, 1, 0, 0, 1, 1, 0, 0, 1, 1), 0b0011001100u
#define PPUTLUTRAITS_0b1100110010u BIN, 818, (1, 1, 0, 0, 1, 1, 0, 0, 1, 0), 0b0011001101u
#define PPUTLUTRAITS_0b1100110001u BIN, 817, (1, 1, 0, 0, 1, 1, 0, 0, 0, 1), 0b0011001110u
#define PPUTLUTRAITS_0b1100110000u BIN, 816, (1, 1, 0, 0, 1, 1, 0, 0, 0, 0), 0b0011001111u
#define PPUTLUTRAITS_0b1100101111u BIN, 815, (1, 1, 0, 0, 1, 0, 1, 1, 1, 1), 0b0011010000u
#define PPUTLUTRAITS_0b1100101110u BIN, 814, (1, 1, 0, 0, 1, 0, 1, 1, 1, 0), 0b0011010001u
#define PPUTLUTRAITS_0b1100101101u BIN, 813, (1, 1, 0, 0, 1, 0, 1, 1, 0, 1), 0b0011010010u
#define PPUTLUTRAITS_0b1100101100u BIN, 812, (1, 1, 0, 0, 1, 0, 1, 1, 0, 0), 0b0011010011u
#define PPUTLUTRAITS_0b1100101011u BIN, 811, (1, 1, 0, 0, 1, 0, 1, 0, 1, 1), 0b0011010100u
#define PPUTLUTRAITS_0b1100101010u BIN, 810, (1, 1, 0, 0, 1, 0, 1, 0, 1, 0), 0b0011010101u
#define PPUTLUTRAITS_0b1100101001u BIN, 809, (1, 1, 0, 0, 1, 0, 1, 0, 0, 1), 0b0011010110u
#define PPUTLUTRAITS_0b1100101000u BIN, 808, (1, 1, 0, 0, 1, 0, 1, 0, 0, 0), 0b0011010111u
#define PPUTLUTRAITS_0b1100100111u BIN, 807, (1, 1, 0, 0, 1, 0, 0, 1, 1, 1), 0b0011011000u
#define PPUTLUTRAITS_0b1100100110u BIN, 806, (1, 1, 0, 0, 1, 0, 0, 1, 1, 0), 0b0011011001u
#define PPUTLUTRAITS_0b1100100101u BIN, 805, (1, 1, 0, 0, 1, 0, 0, 1, 0, 1), 0b0011011010u
#define PPUTLUTRAITS_0b1100100100u BIN, 804, (1, 1, 0, 0, 1, 0, 0, 1, 0, 0), 0b0011011011u
#define PPUTLUTRAITS_0b1100100011u BIN, 803, (1, 1, 0, 0, 1, 0, 0, 0, 1, 1), 0b0011011100u
#define PPUTLUTRAITS_0b1100100010u BIN, 802, (1, 1, 0, 0, 1, 0, 0, 0, 1, 0), 0b0011011101u
#define PPUTLUTRAITS_0b1100100001u BIN, 801, (1, 1, 0, 0, 1, 0, 0, 0, 0, 1), 0b0011011110u
#define PPUTLUTRAITS_0b1100100000u BIN, 800, (1, 1, 0, 0, 1, 0, 0, 0, 0, 0), 0b0011011111u
#define PPUTLUTRAITS_0b1100011111u BIN, 799, (1, 1, 0, 0, 0, 1, 1, 1, 1, 1), 0b0011100000u
#define PPUTLUTRAITS_0b1100011110u BIN, 798, (1, 1, 0, 0, 0, 1, 1, 1, 1, 0), 0b0011100001u
#define PPUTLUTRAITS_0b1100011101u BIN, 797, (1, 1, 0, 0, 0, 1, 1, 1, 0, 1), 0b0011100010u
#define PPUTLUTRAITS_0b1100011100u BIN, 796, (1, 1, 0, 0, 0, 1, 1, 1, 0, 0), 0b0011100011u
#define PPUTLUTRAITS_0b1100011011u BIN, 795, (1, 1, 0, 0, 0, 1, 1, 0, 1, 1), 0b0011100100u
#define PPUTLUTRAITS_0b1100011010u BIN, 794, (1, 1, 0, 0, 0, 1, 1, 0, 1, 0), 0b0011100101u
#define PPUTLUTRAITS_0b1100011001u BIN, 793, (1, 1, 0, 0, 0, 1, 1, 0, 0, 1), 0b0011100110u
#define PPUTLUTRAITS_0b1100011000u BIN, 792, (1, 1, 0, 0, 0, 1, 1, 0, 0, 0), 0b0011100111u
#define PPUTLUTRAITS_0b1100010111u BIN, 791, (1, 1, 0, 0, 0, 1, 0, 1, 1, 1), 0b0011101000u
#define PPUTLUTRAITS_0b1100010110u BIN, 790, (1, 1, 0, 0, 0, 1, 0, 1, 1, 0), 0b0011101001u
#define PPUTLUTRAITS_0b1100010101u BIN, 789, (1, 1, 0, 0, 0, 1, 0, 1, 0, 1), 0b0011101010u
#define PPUTLUTRAITS_0b1100010100u BIN, 788, (1, 1, 0, 0, 0, 1, 0, 1, 0, 0), 0b0011101011u
#define PPUTLUTRAITS_0b1100010011u BIN, 787, (1, 1, 0, 0, 0, 1, 0, 0, 1, 1), 0b0011101100u
#define PPUTLUTRAITS_0b1100010010u BIN, 786, (1, 1, 0, 0, 0, 1, 0, 0, 1, 0), 0b0011101101u
#define PPUTLUTRAITS_0b1100010001u BIN, 785, (1, 1, 0, 0, 0, 1, 0, 0, 0, 1), 0b0011101110u
#define PPUTLUTRAITS_0b1100010000u BIN, 784, (1, 1, 0, 0, 0, 1, 0, 0, 0, 0), 0b0011101111u
#define PPUTLUTRAITS_0b1100001111u BIN, 783, (1, 1, 0, 0, 0, 0, 1, 1, 1, 1), 0b0011110000u
#define PPUTLUTRAITS_0b1100001110u BIN, 782, (1, 1, 0, 0, 0, 0, 1, 1, 1, 0), 0b0011110001u
#define PPUTLUTRAITS_0b1100001101u BIN, 781, (1, 1, 0, 0, 0, 0, 1, 1, 0, 1), 0b0011110010u
#define PPUTLUTRAITS_0b1100001100u BIN, 780, (1, 1, 0, 0, 0, 0, 1, 1, 0, 0), 0b0011110011u
#define PPUTLUTRAITS_0b1100001011u BIN, 779, (1, 1, 0, 0, 0, 0, 1, 0, 1, 1), 0b0011110100u
#define PPUTLUTRAITS_0b1100001010u BIN, 778, (1, 1, 0, 0, 0, 0, 1, 0, 1, 0), 0b0011110101u
#define PPUTLUTRAITS_0b1100001001u BIN, 777, (1, 1, 0, 0, 0, 0, 1, 0, 0, 1), 0b0011110110u
#define PPUTLUTRAITS_0b1100001000u BIN, 776, (1, 1, 0, 0, 0, 0, 1, 0, 0, 0), 0b0011110111u
#define PPUTLUTRAITS_0b1100000111u BIN, 775, (1, 1, 0, 0, 0, 0, 0, 1, 1, 1), 0b0011111000u
#define PPUTLUTRAITS_0b1100000110u BIN, 774, (1, 1, 0, 0, 0, 0, 0, 1, 1, 0), 0b0011111001u
#define PPUTLUTRAITS_0b1100000101u BIN, 773, (1, 1, 0, 0, 0, 0, 0, 1, 0, 1), 0b0011111010u
#define PPUTLUTRAITS_0b1100000100u BIN, 772, (1, 1, 0, 0, 0, 0, 0, 1, 0, 0), 0b0011111011u
#define PPUTLUTRAITS_0b1100000011u BIN, 771, (1, 1, 0, 0, 0, 0, 0, 0, 1, 1), 0b0011111100u
#define PPUTLUTRAITS_0b1100000010u BIN, 770, (1, 1, 0, 0, 0, 0, 0, 0, 1, 0), 0b0011111101u
#define PPUTLUTRAITS_0b1100000001u BIN, 769, (1, 1, 0, 0, 0, 0, 0, 0, 0, 1), 0b0011111110u
#define PPUTLUTRAITS_0b1100000000u BIN, 768, (1, 1, 0, 0, 0, 0, 0, 0, 0, 0), 0b0011111111u
#define PPUTLUTRAITS_0b1011111111u BIN, 767, (1, 0, 1, 1, 1, 1, 1, 1, 1, 1), 0b0100000000u
#define PPUTLUTRAITS_0b1011111110u BIN, 766, (1, 0, 1, 1, 1, 1, 1, 1, 1, 0), 0b0100000001u
#define PPUTLUTRAITS_0b1011111101u BIN, 765, (1, 0, 1, 1, 1, 1, 1, 1, 0, 1), 0b0100000010u
#define PPUTLUTRAITS_0b1011111100u BIN, 764, (1, 0, 1, 1, 1, 1, 1, 1, 0, 0), 0b0100000011u
#define PPUTLUTRAITS_0b1011111011u BIN, 763, (1, 0, 1, 1, 1, 1, 1, 0, 1, 1), 0b0100000100u
#define PPUTLUTRAITS_0b1011111010u BIN, 762, (1, 0, 1, 1, 1, 1, 1, 0, 1, 0), 0b0100000101u
#define PPUTLUTRAITS_0b1011111001u BIN, 761, (1, 0, 1, 1, 1, 1, 1, 0, 0, 1), 0b0100000110u
#define PPUTLUTRAITS_0b1011111000u BIN, 760, (1, 0, 1, 1, 1, 1, 1, 0, 0, 0), 0b0100000111u
#define PPUTLUTRAITS_0b1011110111u BIN, 759, (1, 0, 1, 1, 1, 1, 0, 1, 1, 1), 0b0100001000u
#define PPUTLUTRAITS_0b1011110110u BIN, 758, (1, 0, 1, 1, 1, 1, 0, 1, 1, 0), 0b0100001001u
#define PPUTLUTRAITS_0b1011110101u BIN, 757, (1, 0, 1, 1, 1, 1, 0, 1, 0, 1), 0b0100001010u
#define PPUTLUTRAITS_0b1011110100u BIN, 756, (1, 0, 1, 1, 1, 1, 0, 1, 0, 0), 0b0100001011u
#define PPUTLUTRAITS_0b1011110011u BIN, 755, (1, 0, 1, 1, 1, 1, 0, 0, 1, 1), 0b0100001100u
#define PPUTLUTRAITS_0b1011110010u BIN, 754, (1, 0, 1, 1, 1, 1, 0, 0, 1, 0), 0b0100001101u
#define PPUTLUTRAITS_0b1011110001u BIN, 753, (1, 0, 1, 1, 1, 1, 0, 0, 0, 1), 0b0100001110u
#define PPUTLUTRAITS_0b1011110000u BIN, 752, (1, 0, 1, 1, 1, 1, 0, 0, 0, 0), 0b0100001111u
#define PPUTLUTRAITS_0b1011101111u BIN, 751, (1, 0, 1, 1, 1, 0, 1, 1, 1, 1), 0b0100010000u
#define PPUTLUTRAITS_0b1011101110u BIN, 750, (1, 0, 1, 1, 1, 0, 1, 1, 1, 0), 0b0100010001u
#define PPUTLUTRAITS_0b1011101101u BIN, 749, (1, 0, 1, 1, 1, 0, 1, 1, 0, 1), 0b0100010010u
#define PPUTLUTRAITS_0b1011101100u BIN, 748, (1, 0, 1, 1, 1, 0, 1, 1, 0, 0), 0b0100010011u
#define PPUTLUTRAITS_0b1011101011u BIN, 747, (1, 0, 1, 1, 1, 0, 1, 0, 1, 1), 0b0100010100u
#define PPUTLUTRAITS_0b1011101010u BIN, 746, (1, 0, 1, 1, 1, 0, 1, 0, 1, 0), 0b0100010101u
#define PPUTLUTRAITS_0b1011101001u BIN, 745, (1, 0, 1, 1, 1, 0, 1, 0, 0, 1), 0b0100010110u
#define PPUTLUTRAITS_0b1011101000u BIN, 744, (1, 0, 1, 1, 1, 0, 1, 0, 0, 0), 0b0100010111u
#define PPUTLUTRAITS_0b1011100111u BIN, 743, (1, 0, 1, 1, 1, 0, 0, 1, 1, 1), 0b0100011000u
#define PPUTLUTRAITS_0b1011100110u BIN, 742, (1, 0, 1, 1, 1, 0, 0, 1, 1, 0), 0b0100011001u
#define PPUTLUTRAITS_0b1011100101u BIN, 741, (1, 0, 1, 1, 1, 0, 0, 1, 0, 1), 0b0100011010u
#define PPUTLUTRAITS_0b1011100100u BIN, 740, (1, 0, 1, 1, 1, 0, 0, 1, 0, 0), 0b0100011011u
#define PPUTLUTRAITS_0b1011100011u BIN, 739, (1, 0, 1, 1, 1, 0, 0, 0, 1, 1), 0b0100011100u
#define PPUTLUTRAITS_0b1011100010u BIN, 738, (1, 0, 1, 1, 1, 0, 0, 0, 1, 0), 0b0100011101u
#define PPUTLUTRAITS_0b1011100001u BIN, 737, (1, 0, 1, 1, 1, 0, 0, 0, 0, 1), 0b0100011110u
#define PPUTLUTRAITS_0b1011100000u BIN, 736, (1, 0, 1, 1, 1, 0, 0, 0, 0, 0), 0b0100011111u
#define PPUTLUTRAITS_0b1011011111u BIN, 735, (1, 0, 1, 1, 0, 1, 1, 1, 1, 1), 0b0100100000u
#define PPUTLUTRAITS_0b1011011110u BIN, 734, (1, 0, 1, 1, 0, 1, 1, 1, 1, 0), 0b0100100001u
#define PPUTLUTRAITS_0b1011011101u BIN, 733, (1, 0, 1, 1, 0, 1, 1, 1, 0, 1), 0b0100100010u
#define PPUTLUTRAITS_0b1011011100u BIN, 732, (1, 0, 1, 1, 0, 1, 1, 1, 0, 0), 0b0100100011u
#define PPUTLUTRAITS_0b1011011011u BIN, 731, (1, 0, 1, 1, 0, 1, 1, 0, 1, 1), 0b0100100100u
#define PPUTLUTRAITS_0b1011011010u BIN, 730, (1, 0, 1, 1, 0, 1, 1, 0, 1, 0), 0b0100100101u
#define PPUTLUTRAITS_0b1011011001u BIN, 729, (1, 0, 1, 1, 0, 1, 1, 0, 0, 1), 0b0100100110u
#define PPUTLUTRAITS_0b1011011000u BIN, 728, (1, 0, 1, 1, 0, 1, 1, 0, 0, 0), 0b0100100111u
#define PPUTLUTRAITS_0b1011010111u BIN, 727, (1, 0, 1, 1, 0, 1, 0, 1, 1, 1), 0b0100101000u
#define PPUTLUTRAITS_0b1011010110u BIN, 726, (1, 0, 1, 1, 0, 1, 0, 1, 1, 0), 0b0100101001u
#define PPUTLUTRAITS_0b1011010101u BIN, 725, (1, 0, 1, 1, 0, 1, 0, 1, 0, 1), 0b0100101010u
#define PPUTLUTRAITS_0b1011010100u BIN, 724, (1, 0, 1, 1, 0, 1, 0, 1, 0, 0), 0b0100101011u
#define PPUTLUTRAITS_0b1011010011u BIN, 723, (1, 0, 1, 1, 0, 1, 0, 0, 1, 1), 0b0100101100u
#define PPUTLUTRAITS_0b1011010010u BIN, 722, (1, 0, 1, 1, 0, 1, 0, 0, 1, 0), 0b0100101101u
#define PPUTLUTRAITS_0b1011010001u BIN, 721, (1, 0, 1, 1, 0, 1, 0, 0, 0, 1), 0b0100101110u
#define PPUTLUTRAITS_0b1011010000u BIN, 720, (1, 0, 1, 1, 0, 1, 0, 0, 0, 0), 0b0100101111u
#define PPUTLUTRAITS_0b1011001111u BIN, 719, (1, 0, 1, 1, 0, 0, 1, 1, 1, 1), 0b0100110000u
#define PPUTLUTRAITS_0b1011001110u BIN, 718, (1, 0, 1, 1, 0, 0, 1, 1, 1, 0), 0b0100110001u
#define PPUTLUTRAITS_0b1011001101u BIN, 717, (1, 0, 1, 1, 0, 0, 1, 1, 0, 1), 0b0100110010u
#define PPUTLUTRAITS_0b1011001100u BIN, 716, (1, 0, 1, 1, 0, 0, 1, 1, 0, 0), 0b0100110011u
#define PPUTLUTRAITS_0b1011001011u BIN, 715, (1, 0, 1, 1, 0, 0, 1, 0, 1, 1), 0b0100110100u
#define PPUTLUTRAITS_0b1011001010u BIN, 714, (1, 0, 1, 1, 0, 0, 1, 0, 1, 0), 0b0100110101u
#define PPUTLUTRAITS_0b1011001001u BIN, 713, (1, 0, 1, 1, 0, 0, 1, 0, 0, 1), 0b0100110110u
#define PPUTLUTRAITS_0b1011001000u BIN, 712, (1, 0, 1, 1, 0, 0, 1, 0, 0, 0), 0b0100110111u
#define PPUTLUTRAITS_0b1011000111u BIN, 711, (1, 0, 1, 1, 0, 0, 0, 1, 1, 1), 0b0100111000u
#define PPUTLUTRAITS_0b1011000110u BIN, 710, (1, 0, 1, 1, 0, 0, 0, 1, 1, 0), 0b0100111001u
#define PPUTLUTRAITS_0b1011000101u BIN, 709, (1, 0, 1, 1, 0, 0, 0, 1, 0, 1), 0b0100111010u
#define PPUTLUTRAITS_0b1011000100u BIN, 708, (1, 0, 1, 1, 0, 0, 0, 1, 0, 0), 0b0100111011u
#define PPUTLUTRAITS_0b1011000011u BIN, 707, (1, 0, 1, 1, 0, 0, 0, 0, 1, 1), 0b0100111100u
#define PPUTLUTRAITS_0b1011000010u BIN, 706, (1, 0, 1, 1, 0, 0, 0, 0, 1, 0), 0b0100111101u
#define PPUTLUTRAITS_0b1011000001u BIN, 705, (1, 0, 1, 1, 0, 0, 0, 0, 0, 1), 0b0100111110u
#define PPUTLUTRAITS_0b1011000000u BIN, 704, (1, 0, 1, 1, 0, 0, 0, 0, 0, 0), 0b0100111111u
#define PPUTLUTRAITS_0b1010111111u BIN, 703, (1, 0, 1, 0, 1, 1, 1, 1, 1, 1), 0b0101000000u
#define PPUTLUTRAITS_0b1010111110u BIN, 702, (1, 0, 1, 0, 1, 1, 1, 1, 1, 0), 0b0101000001u
#define PPUTLUTRAITS_0b1010111101u BIN, 701, (1, 0, 1, 0, 1, 1, 1, 1, 0, 1), 0b0101000010u
#define PPUTLUTRAITS_0b1010111100u BIN, 700, (1, 0, 1, 0, 1, 1, 1, 1, 0, 0), 0b0101000011u
#define PPUTLUTRAITS_0b1010111011u BIN, 699, (1, 0, 1, 0, 1, 1, 1, 0, 1, 1), 0b0101000100u
#define PPUTLUTRAITS_0b1010111010u BIN, 698, (1, 0, 1, 0, 1, 1, 1, 0, 1, 0), 0b0101000101u
#define PPUTLUTRAITS_0b1010111001u BIN, 697, (1, 0, 1, 0, 1, 1, 1, 0, 0, 1), 0b0101000110u
#define PPUTLUTRAITS_0b1010111000u BIN, 696, (1, 0, 1, 0, 1, 1, 1, 0, 0, 0), 0b0101000111u
#define PPUTLUTRAITS_0b1010110111u BIN, 695, (1, 0, 1, 0, 1, 1, 0, 1, 1, 1), 0b0101001000u
#define PPUTLUTRAITS_0b1010110110u BIN, 694, (1, 0, 1, 0, 1, 1, 0, 1, 1, 0), 0b0101001001u
#define PPUTLUTRAITS_0b1010110101u BIN, 693, (1, 0, 1, 0, 1, 1, 0, 1, 0, 1), 0b0101001010u
#define PPUTLUTRAITS_0b1010110100u BIN, 692, (1, 0, 1, 0, 1, 1, 0, 1, 0, 0), 0b0101001011u
#define PPUTLUTRAITS_0b1010110011u BIN, 691, (1, 0, 1, 0, 1, 1, 0, 0, 1, 1), 0b0101001100u
#define PPUTLUTRAITS_0b1010110010u BIN, 690, (1, 0, 1, 0, 1, 1, 0, 0, 1, 0), 0b0101001101u
#define PPUTLUTRAITS_0b1010110001u BIN, 689, (1, 0, 1, 0, 1, 1, 0, 0, 0, 1), 0b0101001110u
#define PPUTLUTRAITS_0b1010110000u BIN, 688, (1, 0, 1, 0, 1, 1, 0, 0, 0, 0), 0b0101001111u
#define PPUTLUTRAITS_0b1010101111u BIN, 687, (1, 0, 1, 0, 1, 0, 1, 1, 1, 1), 0b0101010000u
#define PPUTLUTRAITS_0b1010101110u BIN, 686, (1, 0, 1, 0, 1, 0, 1, 1, 1, 0), 0b0101010001u
#define PPUTLUTRAITS_0b1010101101u BIN, 685, (1, 0, 1, 0, 1, 0, 1, 1, 0, 1), 0b0101010010u
#define PPUTLUTRAITS_0b1010101100u BIN, 684, (1, 0, 1, 0, 1, 0, 1, 1, 0, 0), 0b0101010011u
#define PPUTLUTRAITS_0b1010101011u BIN, 683, (1, 0, 1, 0, 1, 0, 1, 0, 1, 1), 0b0101010100u
#define PPUTLUTRAITS_0b1010101010u BIN, 682, (1, 0, 1, 0, 1, 0, 1, 0, 1, 0), 0b0101010101u
#define PPUTLUTRAITS_0b1010101001u BIN, 681, (1, 0, 1, 0, 1, 0, 1, 0, 0, 1), 0b0101010110u
#define PPUTLUTRAITS_0b1010101000u BIN, 680, (1, 0, 1, 0, 1, 0, 1, 0, 0, 0), 0b0101010111u
#define PPUTLUTRAITS_0b1010100111u BIN, 679, (1, 0, 1, 0, 1, 0, 0, 1, 1, 1), 0b0101011000u
#define PPUTLUTRAITS_0b1010100110u BIN, 678, (1, 0, 1, 0, 1, 0, 0, 1, 1, 0), 0b0101011001u
#define PPUTLUTRAITS_0b1010100101u BIN, 677, (1, 0, 1, 0, 1, 0, 0, 1, 0, 1), 0b0101011010u
#define PPUTLUTRAITS_0b1010100100u BIN, 676, (1, 0, 1, 0, 1, 0, 0, 1, 0, 0), 0b0101011011u
#define PPUTLUTRAITS_0b1010100011u BIN, 675, (1, 0, 1, 0, 1, 0, 0, 0, 1, 1), 0b0101011100u
#define PPUTLUTRAITS_0b1010100010u BIN, 674, (1, 0, 1, 0, 1, 0, 0, 0, 1, 0), 0b0101011101u
#define PPUTLUTRAITS_0b1010100001u BIN, 673, (1, 0, 1, 0, 1, 0, 0, 0, 0, 1), 0b0101011110u
#define PPUTLUTRAITS_0b1010100000u BIN, 672, (1, 0, 1, 0, 1, 0, 0, 0, 0, 0), 0b0101011111u
#define PPUTLUTRAITS_0b1010011111u BIN, 671, (1, 0, 1, 0, 0, 1, 1, 1, 1, 1), 0b0101100000u
#define PPUTLUTRAITS_0b1010011110u BIN, 670, (1, 0, 1, 0, 0, 1, 1, 1, 1, 0), 0b0101100001u
#define PPUTLUTRAITS_0b1010011101u BIN, 669, (1, 0, 1, 0, 0, 1, 1, 1, 0, 1), 0b0101100010u
#define PPUTLUTRAITS_0b1010011100u BIN, 668, (1, 0, 1, 0, 0, 1, 1, 1, 0, 0), 0b0101100011u
#define PPUTLUTRAITS_0b1010011011u BIN, 667, (1, 0, 1, 0, 0, 1, 1, 0, 1, 1), 0b0101100100u
#define PPUTLUTRAITS_0b1010011010u BIN, 666, (1, 0, 1, 0, 0, 1, 1, 0, 1, 0), 0b0101100101u
#define PPUTLUTRAITS_0b1010011001u BIN, 665, (1, 0, 1, 0, 0, 1, 1, 0, 0, 1), 0b0101100110u
#define PPUTLUTRAITS_0b1010011000u BIN, 664, (1, 0, 1, 0, 0, 1, 1, 0, 0, 0), 0b0101100111u
#define PPUTLUTRAITS_0b1010010111u BIN, 663, (1, 0, 1, 0, 0, 1, 0, 1, 1, 1), 0b0101101000u
#define PPUTLUTRAITS_0b1010010110u BIN, 662, (1, 0, 1, 0, 0, 1, 0, 1, 1, 0), 0b0101101001u
#define PPUTLUTRAITS_0b1010010101u BIN, 661, (1, 0, 1, 0, 0, 1, 0, 1, 0, 1), 0b0101101010u
#define PPUTLUTRAITS_0b1010010100u BIN, 660, (1, 0, 1, 0, 0, 1, 0, 1, 0, 0), 0b0101101011u
#define PPUTLUTRAITS_0b1010010011u BIN, 659, (1, 0, 1, 0, 0, 1, 0, 0, 1, 1), 0b0101101100u
#define PPUTLUTRAITS_0b1010010010u BIN, 658, (1, 0, 1, 0, 0, 1, 0, 0, 1, 0), 0b0101101101u
#define PPUTLUTRAITS_0b1010010001u BIN, 657, (1, 0, 1, 0, 0, 1, 0, 0, 0, 1), 0b0101101110u
#define PPUTLUTRAITS_0b1010010000u BIN, 656, (1, 0, 1, 0, 0, 1, 0, 0, 0, 0), 0b0101101111u
#define PPUTLUTRAITS_0b1010001111u BIN, 655, (1, 0, 1, 0, 0, 0, 1, 1, 1, 1), 0b0101110000u
#define PPUTLUTRAITS_0b1010001110u BIN, 654, (1, 0, 1, 0, 0, 0, 1, 1, 1, 0), 0b0101110001u
#define PPUTLUTRAITS_0b1010001101u BIN, 653, (1, 0, 1, 0, 0, 0, 1, 1, 0, 1), 0b0101110010u
#define PPUTLUTRAITS_0b1010001100u BIN, 652, (1, 0, 1, 0, 0, 0, 1, 1, 0, 0), 0b0101110011u
#define PPUTLUTRAITS_0b1010001011u BIN, 651, (1, 0, 1, 0, 0, 0, 1, 0, 1, 1), 0b0101110100u
#define PPUTLUTRAITS_0b1010001010u BIN, 650, (1, 0, 1, 0, 0, 0, 1, 0, 1, 0), 0b0101110101u
#define PPUTLUTRAITS_0b1010001001u BIN, 649, (1, 0, 1, 0, 0, 0, 1, 0, 0, 1), 0b0101110110u
#define PPUTLUTRAITS_0b1010001000u BIN, 648, (1, 0, 1, 0, 0, 0, 1, 0, 0, 0), 0b0101110111u
#define PPUTLUTRAITS_0b1010000111u BIN, 647, (1, 0, 1, 0, 0, 0, 0, 1, 1, 1), 0b0101111000u
#define PPUTLUTRAITS_0b1010000110u BIN, 646, (1, 0, 1, 0, 0, 0, 0, 1, 1, 0), 0b0101111001u
#define PPUTLUTRAITS_0b1010000101u BIN, 645, (1, 0, 1, 0, 0, 0, 0, 1, 0, 1), 0b0101111010u
#define PPUTLUTRAITS_0b1010000100u BIN, 644, (1, 0, 1, 0, 0, 0, 0, 1, 0, 0), 0b0101111011u
#define PPUTLUTRAITS_0b1010000011u BIN, 643, (1, 0, 1, 0, 0, 0, 0, 0, 1, 1), 0b0101111100u
#define PPUTLUTRAITS_0b1010000010u BIN, 642, (1, 0, 1, 0, 0, 0, 0, 0, 1, 0), 0b0101111101u
#define PPUTLUTRAITS_0b1010000001u BIN, 641, (1, 0, 1, 0, 0, 0, 0, 0, 0, 1), 0b0101111110u
#define PPUTLUTRAITS_0b1010000000u BIN, 640, (1, 0, 1, 0, 0, 0, 0, 0, 0, 0), 0b0101111111u
#define PPUTLUTRAITS_0b1001111111u BIN, 639, (1, 0, 0, 1, 1, 1, 1, 1, 1, 1), 0b0110000000u
#define PPUTLUTRAITS_0b1001111110u BIN, 638, (1, 0, 0, 1, 1, 1, 1, 1, 1, 0), 0b0110000001u
#define PPUTLUTRAITS_0b1001111101u BIN, 637, (1, 0, 0, 1, 1, 1, 1, 1, 0, 1), 0b0110000010u
#define PPUTLUTRAITS_0b1001111100u BIN, 636, (1, 0, 0, 1, 1, 1, 1, 1, 0, 0), 0b0110000011u
#define PPUTLUTRAITS_0b1001111011u BIN, 635, (1, 0, 0, 1, 1, 1, 1, 0, 1, 1), 0b0110000100u
#define PPUTLUTRAITS_0b1001111010u BIN, 634, (1, 0, 0, 1, 1, 1, 1, 0, 1, 0), 0b0110000101u
#define PPUTLUTRAITS_0b1001111001u BIN, 633, (1, 0, 0, 1, 1, 1, 1, 0, 0, 1), 0b0110000110u
#define PPUTLUTRAITS_0b1001111000u BIN, 632, (1, 0, 0, 1, 1, 1, 1, 0, 0, 0), 0b0110000111u
#define PPUTLUTRAITS_0b1001110111u BIN, 631, (1, 0, 0, 1, 1, 1, 0, 1, 1, 1), 0b0110001000u
#define PPUTLUTRAITS_0b1001110110u BIN, 630, (1, 0, 0, 1, 1, 1, 0, 1, 1, 0), 0b0110001001u
#define PPUTLUTRAITS_0b1001110101u BIN, 629, (1, 0, 0, 1, 1, 1, 0, 1, 0, 1), 0b0110001010u
#define PPUTLUTRAITS_0b1001110100u BIN, 628, (1, 0, 0, 1, 1, 1, 0, 1, 0, 0), 0b0110001011u
#define PPUTLUTRAITS_0b1001110011u BIN, 627, (1, 0, 0, 1, 1, 1, 0, 0, 1, 1), 0b0110001100u
#define PPUTLUTRAITS_0b1001110010u BIN, 626, (1, 0, 0, 1, 1, 1, 0, 0, 1, 0), 0b0110001101u
#define PPUTLUTRAITS_0b1001110001u BIN, 625, (1, 0, 0, 1, 1, 1, 0, 0, 0, 1), 0b0110001110u
#define PPUTLUTRAITS_0b1001110000u BIN, 624, (1, 0, 0, 1, 1, 1, 0, 0, 0, 0), 0b0110001111u
#define PPUTLUTRAITS_0b1001101111u BIN, 623, (1, 0, 0, 1, 1, 0, 1, 1, 1, 1), 0b0110010000u
#define PPUTLUTRAITS_0b1001101110u BIN, 622, (1, 0, 0, 1, 1, 0, 1, 1, 1, 0), 0b0110010001u
#define PPUTLUTRAITS_0b1001101101u BIN, 621, (1, 0, 0, 1, 1, 0, 1, 1, 0, 1), 0b0110010010u
#define PPUTLUTRAITS_0b1001101100u BIN, 620, (1, 0, 0, 1, 1, 0, 1, 1, 0, 0), 0b0110010011u
#define PPUTLUTRAITS_0b1001101011u BIN, 619, (1, 0, 0, 1, 1, 0, 1, 0, 1, 1), 0b0110010100u
#define PPUTLUTRAITS_0b1001101010u BIN, 618, (1, 0, 0, 1, 1, 0, 1, 0, 1, 0), 0b0110010101u
#define PPUTLUTRAITS_0b1001101001u BIN, 617, (1, 0, 0, 1, 1, 0, 1, 0, 0, 1), 0b0110010110u
#define PPUTLUTRAITS_0b1001101000u BIN, 616, (1, 0, 0, 1, 1, 0, 1, 0, 0, 0), 0b0110010111u
#define PPUTLUTRAITS_0b1001100111u BIN, 615, (1, 0, 0, 1, 1, 0, 0, 1, 1, 1), 0b0110011000u
#define PPUTLUTRAITS_0b1001100110u BIN, 614, (1, 0, 0, 1, 1, 0, 0, 1, 1, 0), 0b0110011001u
#define PPUTLUTRAITS_0b1001100101u BIN, 613, (1, 0, 0, 1, 1, 0, 0, 1, 0, 1), 0b0110011010u
#define PPUTLUTRAITS_0b1001100100u BIN, 612, (1, 0, 0, 1, 1, 0, 0, 1, 0, 0), 0b0110011011u
#define PPUTLUTRAITS_0b1001100011u BIN, 611, (1, 0, 0, 1, 1, 0, 0, 0, 1, 1), 0b0110011100u
#define PPUTLUTRAITS_0b1001100010u BIN, 610, (1, 0, 0, 1, 1, 0, 0, 0, 1, 0), 0b0110011101u
#define PPUTLUTRAITS_0b1001100001u BIN, 609, (1, 0, 0, 1, 1, 0, 0, 0, 0, 1), 0b0110011110u
#define PPUTLUTRAITS_0b1001100000u BIN, 608, (1, 0, 0, 1, 1, 0, 0, 0, 0, 0), 0b0110011111u
#define PPUTLUTRAITS_0b1001011111u BIN, 607, (1, 0, 0, 1, 0, 1, 1, 1, 1, 1), 0b0110100000u
#define PPUTLUTRAITS_0b1001011110u BIN, 606, (1, 0, 0, 1, 0, 1, 1, 1, 1, 0), 0b0110100001u
#define PPUTLUTRAITS_0b1001011101u BIN, 605, (1, 0, 0, 1, 0, 1, 1, 1, 0, 1), 0b0110100010u
#define PPUTLUTRAITS_0b1001011100u BIN, 604, (1, 0, 0, 1, 0, 1, 1, 1, 0, 0), 0b0110100011u
#define PPUTLUTRAITS_0b1001011011u BIN, 603, (1, 0, 0, 1, 0, 1, 1, 0, 1, 1), 0b0110100100u
#define PPUTLUTRAITS_0b1001011010u BIN, 602, (1, 0, 0, 1, 0, 1, 1, 0, 1, 0), 0b0110100101u
#define PPUTLUTRAITS_0b1001011001u BIN, 601, (1, 0, 0, 1, 0, 1, 1, 0, 0, 1), 0b0110100110u
#define PPUTLUTRAITS_0b1001011000u BIN, 600, (1, 0, 0, 1, 0, 1, 1, 0, 0, 0), 0b0110100111u
#define PPUTLUTRAITS_0b1001010111u BIN, 599, (1, 0, 0, 1, 0, 1, 0, 1, 1, 1), 0b0110101000u
#define PPUTLUTRAITS_0b1001010110u BIN, 598, (1, 0, 0, 1, 0, 1, 0, 1, 1, 0), 0b0110101001u
#define PPUTLUTRAITS_0b1001010101u BIN, 597, (1, 0, 0, 1, 0, 1, 0, 1, 0, 1), 0b0110101010u
#define PPUTLUTRAITS_0b1001010100u BIN, 596, (1, 0, 0, 1, 0, 1, 0, 1, 0, 0), 0b0110101011u
#define PPUTLUTRAITS_0b1001010011u BIN, 595, (1, 0, 0, 1, 0, 1, 0, 0, 1, 1), 0b0110101100u
#define PPUTLUTRAITS_0b1001010010u BIN, 594, (1, 0, 0, 1, 0, 1, 0, 0, 1, 0), 0b0110101101u
#define PPUTLUTRAITS_0b1001010001u BIN, 593, (1, 0, 0, 1, 0, 1, 0, 0, 0, 1), 0b0110101110u
#define PPUTLUTRAITS_0b1001010000u BIN, 592, (1, 0, 0, 1, 0, 1, 0, 0, 0, 0), 0b0110101111u
#define PPUTLUTRAITS_0b1001001111u BIN, 591, (1, 0, 0, 1, 0, 0, 1, 1, 1, 1), 0b0110110000u
#define PPUTLUTRAITS_0b1001001110u BIN, 590, (1, 0, 0, 1, 0, 0, 1, 1, 1, 0), 0b0110110001u
#define PPUTLUTRAITS_0b1001001101u BIN, 589, (1, 0, 0, 1, 0, 0, 1, 1, 0, 1), 0b0110110010u
#define PPUTLUTRAITS_0b1001001100u BIN, 588, (1, 0, 0, 1, 0, 0, 1, 1, 0, 0), 0b0110110011u
#define PPUTLUTRAITS_0b1001001011u BIN, 587, (1, 0, 0, 1, 0, 0, 1, 0, 1, 1), 0b0110110100u
#define PPUTLUTRAITS_0b1001001010u BIN, 586, (1, 0, 0, 1, 0, 0, 1, 0, 1, 0), 0b0110110101u
#define PPUTLUTRAITS_0b1001001001u BIN, 585, (1, 0, 0, 1, 0, 0, 1, 0, 0, 1), 0b0110110110u
#define PPUTLUTRAITS_0b1001001000u BIN, 584, (1, 0, 0, 1, 0, 0, 1, 0, 0, 0), 0b0110110111u
#define PPUTLUTRAITS_0b1001000111u BIN, 583, (1, 0, 0, 1, 0, 0, 0, 1, 1, 1), 0b0110111000u
#define PPUTLUTRAITS_0b1001000110u BIN, 582, (1, 0, 0, 1, 0, 0, 0, 1, 1, 0), 0b0110111001u
#define PPUTLUTRAITS_0b1001000101u BIN, 581, (1, 0, 0, 1, 0, 0, 0, 1, 0, 1), 0b0110111010u
#define PPUTLUTRAITS_0b1001000100u BIN, 580, (1, 0, 0, 1, 0, 0, 0, 1, 0, 0), 0b0110111011u
#define PPUTLUTRAITS_0b1001000011u BIN, 579, (1, 0, 0, 1, 0, 0, 0, 0, 1, 1), 0b0110111100u
#define PPUTLUTRAITS_0b1001000010u BIN, 578, (1, 0, 0, 1, 0, 0, 0, 0, 1, 0), 0b0110111101u
#define PPUTLUTRAITS_0b1001000001u BIN, 577, (1, 0, 0, 1, 0, 0, 0, 0, 0, 1), 0b0110111110u
#define PPUTLUTRAITS_0b1001000000u BIN, 576, (1, 0, 0, 1, 0, 0, 0, 0, 0, 0), 0b0110111111u
#define PPUTLUTRAITS_0b1000111111u BIN, 575, (1, 0, 0, 0, 1, 1, 1, 1, 1, 1), 0b0111000000u
#define PPUTLUTRAITS_0b1000111110u BIN, 574, (1, 0, 0, 0, 1, 1, 1, 1, 1, 0), 0b0111000001u
#define PPUTLUTRAITS_0b1000111101u BIN, 573, (1, 0, 0, 0, 1, 1, 1, 1, 0, 1), 0b0111000010u
#define PPUTLUTRAITS_0b1000111100u BIN, 572, (1, 0, 0, 0, 1, 1, 1, 1, 0, 0), 0b0111000011u
#define PPUTLUTRAITS_0b1000111011u BIN, 571, (1, 0, 0, 0, 1, 1, 1, 0, 1, 1), 0b0111000100u
#define PPUTLUTRAITS_0b1000111010u BIN, 570, (1, 0, 0, 0, 1, 1, 1, 0, 1, 0), 0b0111000101u
#define PPUTLUTRAITS_0b1000111001u BIN, 569, (1, 0, 0, 0, 1, 1, 1, 0, 0, 1), 0b0111000110u
#define PPUTLUTRAITS_0b1000111000u BIN, 568, (1, 0, 0, 0, 1, 1, 1, 0, 0, 0), 0b0111000111u
#define PPUTLUTRAITS_0b1000110111u BIN, 567, (1, 0, 0, 0, 1, 1, 0, 1, 1, 1), 0b0111001000u
#define PPUTLUTRAITS_0b1000110110u BIN, 566, (1, 0, 0, 0, 1, 1, 0, 1, 1, 0), 0b0111001001u
#define PPUTLUTRAITS_0b1000110101u BIN, 565, (1, 0, 0, 0, 1, 1, 0, 1, 0, 1), 0b0111001010u
#define PPUTLUTRAITS_0b1000110100u BIN, 564, (1, 0, 0, 0, 1, 1, 0, 1, 0, 0), 0b0111001011u
#define PPUTLUTRAITS_0b1000110011u BIN, 563, (1, 0, 0, 0, 1, 1, 0, 0, 1, 1), 0b0111001100u
#define PPUTLUTRAITS_0b1000110010u BIN, 562, (1, 0, 0, 0, 1, 1, 0, 0, 1, 0), 0b0111001101u
#define PPUTLUTRAITS_0b1000110001u BIN, 561, (1, 0, 0, 0, 1, 1, 0, 0, 0, 1), 0b0111001110u
#define PPUTLUTRAITS_0b1000110000u BIN, 560, (1, 0, 0, 0, 1, 1, 0, 0, 0, 0), 0b0111001111u
#define PPUTLUTRAITS_0b1000101111u BIN, 559, (1, 0, 0, 0, 1, 0, 1, 1, 1, 1), 0b0111010000u
#define PPUTLUTRAITS_0b1000101110u BIN, 558, (1, 0, 0, 0, 1, 0, 1, 1, 1, 0), 0b0111010001u
#define PPUTLUTRAITS_0b1000101101u BIN, 557, (1, 0, 0, 0, 1, 0, 1, 1, 0, 1), 0b0111010010u
#define PPUTLUTRAITS_0b1000101100u BIN, 556, (1, 0, 0, 0, 1, 0, 1, 1, 0, 0), 0b0111010011u
#define PPUTLUTRAITS_0b1000101011u BIN, 555, (1, 0, 0, 0, 1, 0, 1, 0, 1, 1), 0b0111010100u
#define PPUTLUTRAITS_0b1000101010u BIN, 554, (1, 0, 0, 0, 1, 0, 1, 0, 1, 0), 0b0111010101u
#define PPUTLUTRAITS_0b1000101001u BIN, 553, (1, 0, 0, 0, 1, 0, 1, 0, 0, 1), 0b0111010110u
#define PPUTLUTRAITS_0b1000101000u BIN, 552, (1, 0, 0, 0, 1, 0, 1, 0, 0, 0), 0b0111010111u
#define PPUTLUTRAITS_0b1000100111u BIN, 551, (1, 0, 0, 0, 1, 0, 0, 1, 1, 1), 0b0111011000u
#define PPUTLUTRAITS_0b1000100110u BIN, 550, (1, 0, 0, 0, 1, 0, 0, 1, 1, 0), 0b0111011001u
#define PPUTLUTRAITS_0b1000100101u BIN, 549, (1, 0, 0, 0, 1, 0, 0, 1, 0, 1), 0b0111011010u
#define PPUTLUTRAITS_0b1000100100u BIN, 548, (1, 0, 0, 0, 1, 0, 0, 1, 0, 0), 0b0111011011u
#define PPUTLUTRAITS_0b1000100011u BIN, 547, (1, 0, 0, 0, 1, 0, 0, 0, 1, 1), 0b0111011100u
#define PPUTLUTRAITS_0b1000100010u BIN, 546, (1, 0, 0, 0, 1, 0, 0, 0, 1, 0), 0b0111011101u
#define PPUTLUTRAITS_0b1000100001u BIN, 545, (1, 0, 0, 0, 1, 0, 0, 0, 0, 1), 0b0111011110u
#define PPUTLUTRAITS_0b1000100000u BIN, 544, (1, 0, 0, 0, 1, 0, 0, 0, 0, 0), 0b0111011111u
#define PPUTLUTRAITS_0b1000011111u BIN, 543, (1, 0, 0, 0, 0, 1, 1, 1, 1, 1), 0b0111100000u
#define PPUTLUTRAITS_0b1000011110u BIN, 542, (1, 0, 0, 0, 0, 1, 1, 1, 1, 0), 0b0111100001u
#define PPUTLUTRAITS_0b1000011101u BIN, 541, (1, 0, 0, 0, 0, 1, 1, 1, 0, 1), 0b0111100010u
#define PPUTLUTRAITS_0b1000011100u BIN, 540, (1, 0, 0, 0, 0, 1, 1, 1, 0, 0), 0b0111100011u
#define PPUTLUTRAITS_0b1000011011u BIN, 539, (1, 0, 0, 0, 0, 1, 1, 0, 1, 1), 0b0111100100u
#define PPUTLUTRAITS_0b1000011010u BIN, 538, (1, 0, 0, 0, 0, 1, 1, 0, 1, 0), 0b0111100101u
#define PPUTLUTRAITS_0b1000011001u BIN, 537, (1, 0, 0, 0, 0, 1, 1, 0, 0, 1), 0b0111100110u
#define PPUTLUTRAITS_0b1000011000u BIN, 536, (1, 0, 0, 0, 0, 1, 1, 0, 0, 0), 0b0111100111u
#define PPUTLUTRAITS_0b1000010111u BIN, 535, (1, 0, 0, 0, 0, 1, 0, 1, 1, 1), 0b0111101000u
#define PPUTLUTRAITS_0b1000010110u BIN, 534, (1, 0, 0, 0, 0, 1, 0, 1, 1, 0), 0b0111101001u
#define PPUTLUTRAITS_0b1000010101u BIN, 533, (1, 0, 0, 0, 0, 1, 0, 1, 0, 1), 0b0111101010u
#define PPUTLUTRAITS_0b1000010100u BIN, 532, (1, 0, 0, 0, 0, 1, 0, 1, 0, 0), 0b0111101011u
#define PPUTLUTRAITS_0b1000010011u BIN, 531, (1, 0, 0, 0, 0, 1, 0, 0, 1, 1), 0b0111101100u
#define PPUTLUTRAITS_0b1000010010u BIN, 530, (1, 0, 0, 0, 0, 1, 0, 0, 1, 0), 0b0111101101u
#define PPUTLUTRAITS_0b1000010001u BIN, 529, (1, 0, 0, 0, 0, 1, 0, 0, 0, 1), 0b0111101110u
#define PPUTLUTRAITS_0b1000010000u BIN, 528, (1, 0, 0, 0, 0, 1, 0, 0, 0, 0), 0b0111101111u
#define PPUTLUTRAITS_0b1000001111u BIN, 527, (1, 0, 0, 0, 0, 0, 1, 1, 1, 1), 0b0111110000u
#define PPUTLUTRAITS_0b1000001110u BIN, 526, (1, 0, 0, 0, 0, 0, 1, 1, 1, 0), 0b0111110001u
#define PPUTLUTRAITS_0b1000001101u BIN, 525, (1, 0, 0, 0, 0, 0, 1, 1, 0, 1), 0b0111110010u
#define PPUTLUTRAITS_0b1000001100u BIN, 524, (1, 0, 0, 0, 0, 0, 1, 1, 0, 0), 0b0111110011u
#define PPUTLUTRAITS_0b1000001011u BIN, 523, (1, 0, 0, 0, 0, 0, 1, 0, 1, 1), 0b0111110100u
#define PPUTLUTRAITS_0b1000001010u BIN, 522, (1, 0, 0, 0, 0, 0, 1, 0, 1, 0), 0b0111110101u
#define PPUTLUTRAITS_0b1000001001u BIN, 521, (1, 0, 0, 0, 0, 0, 1, 0, 0, 1), 0b0111110110u
#define PPUTLUTRAITS_0b1000001000u BIN, 520, (1, 0, 0, 0, 0, 0, 1, 0, 0, 0), 0b0111110111u
#define PPUTLUTRAITS_0b1000000111u BIN, 519, (1, 0, 0, 0, 0, 0, 0, 1, 1, 1), 0b0111111000u
#define PPUTLUTRAITS_0b1000000110u BIN, 518, (1, 0, 0, 0, 0, 0, 0, 1, 1, 0), 0b0111111001u
#define PPUTLUTRAITS_0b1000000101u BIN, 517, (1, 0, 0, 0, 0, 0, 0, 1, 0, 1), 0b0111111010u
#define PPUTLUTRAITS_0b1000000100u BIN, 516, (1, 0, 0, 0, 0, 0, 0, 1, 0, 0), 0b0111111011u
#define PPUTLUTRAITS_0b1000000011u BIN, 515, (1, 0, 0, 0, 0, 0, 0, 0, 1, 1), 0b0111111100u
#define PPUTLUTRAITS_0b1000000010u BIN, 514, (1, 0, 0, 0, 0, 0, 0, 0, 1, 0), 0b0111111101u
#define PPUTLUTRAITS_0b1000000001u BIN, 513, (1, 0, 0, 0, 0, 0, 0, 0, 0, 1), 0b0111111110u
#define PPUTLUTRAITS_0b1000000000u BIN, 512, (1, 0, 0, 0, 0, 0, 0, 0, 0, 0), 0b0111111111u
#define PPUTLUTRAITS_0b0111111111u BIN, 511, (0, 1, 1, 1, 1, 1, 1, 1, 1, 1), 0b1000000000u
#define PPUTLUTRAITS_0b0111111110u BIN, 510, (0, 1, 1, 1, 1, 1, 1, 1, 1, 0), 0b1000000001u
#define PPUTLUTRAITS_0b0111111101u BIN, 509, (0, 1, 1, 1, 1, 1, 1, 1, 0, 1), 0b1000000010u
#define PPUTLUTRAITS_0b0111111100u BIN, 508, (0, 1, 1, 1, 1, 1, 1, 1, 0, 0), 0b1000000011u
#define PPUTLUTRAITS_0b0111111011u BIN, 507, (0, 1, 1, 1, 1, 1, 1, 0, 1, 1), 0b1000000100u
#define PPUTLUTRAITS_0b0111111010u BIN, 506, (0, 1, 1, 1, 1, 1, 1, 0, 1, 0), 0b1000000101u
#define PPUTLUTRAITS_0b0111111001u BIN, 505, (0, 1, 1, 1, 1, 1, 1, 0, 0, 1), 0b1000000110u
#define PPUTLUTRAITS_0b0111111000u BIN, 504, (0, 1, 1, 1, 1, 1, 1, 0, 0, 0), 0b1000000111u
#define PPUTLUTRAITS_0b0111110111u BIN, 503, (0, 1, 1, 1, 1, 1, 0, 1, 1, 1), 0b1000001000u
#define PPUTLUTRAITS_0b0111110110u BIN, 502, (0, 1, 1, 1, 1, 1, 0, 1, 1, 0), 0b1000001001u
#define PPUTLUTRAITS_0b0111110101u BIN, 501, (0, 1, 1, 1, 1, 1, 0, 1, 0, 1), 0b1000001010u
#define PPUTLUTRAITS_0b0111110100u BIN, 500, (0, 1, 1, 1, 1, 1, 0, 1, 0, 0), 0b1000001011u
#define PPUTLUTRAITS_0b0111110011u BIN, 499, (0, 1, 1, 1, 1, 1, 0, 0, 1, 1), 0b1000001100u
#define PPUTLUTRAITS_0b0111110010u BIN, 498, (0, 1, 1, 1, 1, 1, 0, 0, 1, 0), 0b1000001101u
#define PPUTLUTRAITS_0b0111110001u BIN, 497, (0, 1, 1, 1, 1, 1, 0, 0, 0, 1), 0b1000001110u
#define PPUTLUTRAITS_0b0111110000u BIN, 496, (0, 1, 1, 1, 1, 1, 0, 0, 0, 0), 0b1000001111u
#define PPUTLUTRAITS_0b0111101111u BIN, 495, (0, 1, 1, 1, 1, 0, 1, 1, 1, 1), 0b1000010000u
#define PPUTLUTRAITS_0b0111101110u BIN, 494, (0, 1, 1, 1, 1, 0, 1, 1, 1, 0), 0b1000010001u
#define PPUTLUTRAITS_0b0111101101u BIN, 493, (0, 1, 1, 1, 1, 0, 1, 1, 0, 1), 0b1000010010u
#define PPUTLUTRAITS_0b0111101100u BIN, 492, (0, 1, 1, 1, 1, 0, 1, 1, 0, 0), 0b1000010011u
#define PPUTLUTRAITS_0b0111101011u BIN, 491, (0, 1, 1, 1, 1, 0, 1, 0, 1, 1), 0b1000010100u
#define PPUTLUTRAITS_0b0111101010u BIN, 490, (0, 1, 1, 1, 1, 0, 1, 0, 1, 0), 0b1000010101u
#define PPUTLUTRAITS_0b0111101001u BIN, 489, (0, 1, 1, 1, 1, 0, 1, 0, 0, 1), 0b1000010110u
#define PPUTLUTRAITS_0b0111101000u BIN, 488, (0, 1, 1, 1, 1, 0, 1, 0, 0, 0), 0b1000010111u
#define PPUTLUTRAITS_0b0111100111u BIN, 487, (0, 1, 1, 1, 1, 0, 0, 1, 1, 1), 0b1000011000u
#define PPUTLUTRAITS_0b0111100110u BIN, 486, (0, 1, 1, 1, 1, 0, 0, 1, 1, 0), 0b1000011001u
#define PPUTLUTRAITS_0b0111100101u BIN, 485, (0, 1, 1, 1, 1, 0, 0, 1, 0, 1), 0b1000011010u
#define PPUTLUTRAITS_0b0111100100u BIN, 484, (0, 1, 1, 1, 1, 0, 0, 1, 0, 0), 0b1000011011u
#define PPUTLUTRAITS_0b0111100011u BIN, 483, (0, 1, 1, 1, 1, 0, 0, 0, 1, 1), 0b1000011100u
#define PPUTLUTRAITS_0b0111100010u BIN, 482, (0, 1, 1, 1, 1, 0, 0, 0, 1, 0), 0b1000011101u
#define PPUTLUTRAITS_0b0111100001u BIN, 481, (0, 1, 1, 1, 1, 0, 0, 0, 0, 1), 0b1000011110u
#define PPUTLUTRAITS_0b0111100000u BIN, 480, (0, 1, 1, 1, 1, 0, 0, 0, 0, 0), 0b1000011111u
#define PPUTLUTRAITS_0b0111011111u BIN, 479, (0, 1, 1, 1, 0, 1, 1, 1, 1, 1), 0b1000100000u
#define PPUTLUTRAITS_0b0111011110u BIN, 478, (0, 1, 1, 1, 0, 1, 1, 1, 1, 0), 0b1000100001u
#define PPUTLUTRAITS_0b0111011101u BIN, 477, (0, 1, 1, 1, 0, 1, 1, 1, 0, 1), 0b1000100010u
#define PPUTLUTRAITS_0b0111011100u BIN, 476, (0, 1, 1, 1, 0, 1, 1, 1, 0, 0), 0b1000100011u
#define PPUTLUTRAITS_0b0111011011u BIN, 475, (0, 1, 1, 1, 0, 1, 1, 0, 1, 1), 0b1000100100u
#define PPUTLUTRAITS_0b0111011010u BIN, 474, (0, 1, 1, 1, 0, 1, 1, 0, 1, 0), 0b1000100101u
#define PPUTLUTRAITS_0b0111011001u BIN, 473, (0, 1, 1, 1, 0, 1, 1, 0, 0, 1), 0b1000100110u
#define PPUTLUTRAITS_0b0111011000u BIN, 472, (0, 1, 1, 1, 0, 1, 1, 0, 0, 0), 0b1000100111u
#define PPUTLUTRAITS_0b0111010111u BIN, 471, (0, 1, 1, 1, 0, 1, 0, 1, 1, 1), 0b1000101000u
#define PPUTLUTRAITS_0b0111010110u BIN, 470, (0, 1, 1, 1, 0, 1, 0, 1, 1, 0), 0b1000101001u
#define PPUTLUTRAITS_0b0111010101u BIN, 469, (0, 1, 1, 1, 0, 1, 0, 1, 0, 1), 0b1000101010u
#define PPUTLUTRAITS_0b0111010100u BIN, 468, (0, 1, 1, 1, 0, 1, 0, 1, 0, 0), 0b1000101011u
#define PPUTLUTRAITS_0b0111010011u BIN, 467, (0, 1, 1, 1, 0, 1, 0, 0, 1, 1), 0b1000101100u
#define PPUTLUTRAITS_0b0111010010u BIN, 466, (0, 1, 1, 1, 0, 1, 0, 0, 1, 0), 0b1000101101u
#define PPUTLUTRAITS_0b0111010001u BIN, 465, (0, 1, 1, 1, 0, 1, 0, 0, 0, 1), 0b1000101110u
#define PPUTLUTRAITS_0b0111010000u BIN, 464, (0, 1, 1, 1, 0, 1, 0, 0, 0, 0), 0b1000101111u
#define PPUTLUTRAITS_0b0111001111u BIN, 463, (0, 1, 1, 1, 0, 0, 1, 1, 1, 1), 0b1000110000u
#define PPUTLUTRAITS_0b0111001110u BIN, 462, (0, 1, 1, 1, 0, 0, 1, 1, 1, 0), 0b1000110001u
#define PPUTLUTRAITS_0b0111001101u BIN, 461, (0, 1, 1, 1, 0, 0, 1, 1, 0, 1), 0b1000110010u
#define PPUTLUTRAITS_0b0111001100u BIN, 460, (0, 1, 1, 1, 0, 0, 1, 1, 0, 0), 0b1000110011u
#define PPUTLUTRAITS_0b0111001011u BIN, 459, (0, 1, 1, 1, 0, 0, 1, 0, 1, 1), 0b1000110100u
#define PPUTLUTRAITS_0b0111001010u BIN, 458, (0, 1, 1, 1, 0, 0, 1, 0, 1, 0), 0b1000110101u
#define PPUTLUTRAITS_0b0111001001u BIN, 457, (0, 1, 1, 1, 0, 0, 1, 0, 0, 1), 0b1000110110u
#define PPUTLUTRAITS_0b0111001000u BIN, 456, (0, 1, 1, 1, 0, 0, 1, 0, 0, 0), 0b1000110111u
#define PPUTLUTRAITS_0b0111000111u BIN, 455, (0, 1, 1, 1, 0, 0, 0, 1, 1, 1), 0b1000111000u
#define PPUTLUTRAITS_0b0111000110u BIN, 454, (0, 1, 1, 1, 0, 0, 0, 1, 1, 0), 0b1000111001u
#define PPUTLUTRAITS_0b0111000101u BIN, 453, (0, 1, 1, 1, 0, 0, 0, 1, 0, 1), 0b1000111010u
#define PPUTLUTRAITS_0b0111000100u BIN, 452, (0, 1, 1, 1, 0, 0, 0, 1, 0, 0), 0b1000111011u
#define PPUTLUTRAITS_0b0111000011u BIN, 451, (0, 1, 1, 1, 0, 0, 0, 0, 1, 1), 0b1000111100u
#define PPUTLUTRAITS_0b0111000010u BIN, 450, (0, 1, 1, 1, 0, 0, 0, 0, 1, 0), 0b1000111101u
#define PPUTLUTRAITS_0b0111000001u BIN, 449, (0, 1, 1, 1, 0, 0, 0, 0, 0, 1), 0b1000111110u
#define PPUTLUTRAITS_0b0111000000u BIN, 448, (0, 1, 1, 1, 0, 0, 0, 0, 0, 0), 0b1000111111u
#define PPUTLUTRAITS_0b0110111111u BIN, 447, (0, 1, 1, 0, 1, 1, 1, 1, 1, 1), 0b1001000000u
#define PPUTLUTRAITS_0b0110111110u BIN, 446, (0, 1, 1, 0, 1, 1, 1, 1, 1, 0), 0b1001000001u
#define PPUTLUTRAITS_0b0110111101u BIN, 445, (0, 1, 1, 0, 1, 1, 1, 1, 0, 1), 0b1001000010u
#define PPUTLUTRAITS_0b0110111100u BIN, 444, (0, 1, 1, 0, 1, 1, 1, 1, 0, 0), 0b1001000011u
#define PPUTLUTRAITS_0b0110111011u BIN, 443, (0, 1, 1, 0, 1, 1, 1, 0, 1, 1), 0b1001000100u
#define PPUTLUTRAITS_0b0110111010u BIN, 442, (0, 1, 1, 0, 1, 1, 1, 0, 1, 0), 0b1001000101u
#define PPUTLUTRAITS_0b0110111001u BIN, 441, (0, 1, 1, 0, 1, 1, 1, 0, 0, 1), 0b1001000110u
#define PPUTLUTRAITS_0b0110111000u BIN, 440, (0, 1, 1, 0, 1, 1, 1, 0, 0, 0), 0b1001000111u
#define PPUTLUTRAITS_0b0110110111u BIN, 439, (0, 1, 1, 0, 1, 1, 0, 1, 1, 1), 0b1001001000u
#define PPUTLUTRAITS_0b0110110110u BIN, 438, (0, 1, 1, 0, 1, 1, 0, 1, 1, 0), 0b1001001001u
#define PPUTLUTRAITS_0b0110110101u BIN, 437, (0, 1, 1, 0, 1, 1, 0, 1, 0, 1), 0b1001001010u
#define PPUTLUTRAITS_0b0110110100u BIN, 436, (0, 1, 1, 0, 1, 1, 0, 1, 0, 0), 0b1001001011u
#define PPUTLUTRAITS_0b0110110011u BIN, 435, (0, 1, 1, 0, 1, 1, 0, 0, 1, 1), 0b1001001100u
#define PPUTLUTRAITS_0b0110110010u BIN, 434, (0, 1, 1, 0, 1, 1, 0, 0, 1, 0), 0b1001001101u
#define PPUTLUTRAITS_0b0110110001u BIN, 433, (0, 1, 1, 0, 1, 1, 0, 0, 0, 1), 0b1001001110u
#define PPUTLUTRAITS_0b0110110000u BIN, 432, (0, 1, 1, 0, 1, 1, 0, 0, 0, 0), 0b1001001111u
#define PPUTLUTRAITS_0b0110101111u BIN, 431, (0, 1, 1, 0, 1, 0, 1, 1, 1, 1), 0b1001010000u
#define PPUTLUTRAITS_0b0110101110u BIN, 430, (0, 1, 1, 0, 1, 0, 1, 1, 1, 0), 0b1001010001u
#define PPUTLUTRAITS_0b0110101101u BIN, 429, (0, 1, 1, 0, 1, 0, 1, 1, 0, 1), 0b1001010010u
#define PPUTLUTRAITS_0b0110101100u BIN, 428, (0, 1, 1, 0, 1, 0, 1, 1, 0, 0), 0b1001010011u
#define PPUTLUTRAITS_0b0110101011u BIN, 427, (0, 1, 1, 0, 1, 0, 1, 0, 1, 1), 0b1001010100u
#define PPUTLUTRAITS_0b0110101010u BIN, 426, (0, 1, 1, 0, 1, 0, 1, 0, 1, 0), 0b1001010101u
#define PPUTLUTRAITS_0b0110101001u BIN, 425, (0, 1, 1, 0, 1, 0, 1, 0, 0, 1), 0b1001010110u
#define PPUTLUTRAITS_0b0110101000u BIN, 424, (0, 1, 1, 0, 1, 0, 1, 0, 0, 0), 0b1001010111u
#define PPUTLUTRAITS_0b0110100111u BIN, 423, (0, 1, 1, 0, 1, 0, 0, 1, 1, 1), 0b1001011000u
#define PPUTLUTRAITS_0b0110100110u BIN, 422, (0, 1, 1, 0, 1, 0, 0, 1, 1, 0), 0b1001011001u
#define PPUTLUTRAITS_0b0110100101u BIN, 421, (0, 1, 1, 0, 1, 0, 0, 1, 0, 1), 0b1001011010u
#define PPUTLUTRAITS_0b0110100100u BIN, 420, (0, 1, 1, 0, 1, 0, 0, 1, 0, 0), 0b1001011011u
#define PPUTLUTRAITS_0b0110100011u BIN, 419, (0, 1, 1, 0, 1, 0, 0, 0, 1, 1), 0b1001011100u
#define PPUTLUTRAITS_0b0110100010u BIN, 418, (0, 1, 1, 0, 1, 0, 0, 0, 1, 0), 0b1001011101u
#define PPUTLUTRAITS_0b0110100001u BIN, 417, (0, 1, 1, 0, 1, 0, 0, 0, 0, 1), 0b1001011110u
#define PPUTLUTRAITS_0b0110100000u BIN, 416, (0, 1, 1, 0, 1, 0, 0, 0, 0, 0), 0b1001011111u
#define PPUTLUTRAITS_0b0110011111u BIN, 415, (0, 1, 1, 0, 0, 1, 1, 1, 1, 1), 0b1001100000u
#define PPUTLUTRAITS_0b0110011110u BIN, 414, (0, 1, 1, 0, 0, 1, 1, 1, 1, 0), 0b1001100001u
#define PPUTLUTRAITS_0b0110011101u BIN, 413, (0, 1, 1, 0, 0, 1, 1, 1, 0, 1), 0b1001100010u
#define PPUTLUTRAITS_0b0110011100u BIN, 412, (0, 1, 1, 0, 0, 1, 1, 1, 0, 0), 0b1001100011u
#define PPUTLUTRAITS_0b0110011011u BIN, 411, (0, 1, 1, 0, 0, 1, 1, 0, 1, 1), 0b1001100100u
#define PPUTLUTRAITS_0b0110011010u BIN, 410, (0, 1, 1, 0, 0, 1, 1, 0, 1, 0), 0b1001100101u
#define PPUTLUTRAITS_0b0110011001u BIN, 409, (0, 1, 1, 0, 0, 1, 1, 0, 0, 1), 0b1001100110u
#define PPUTLUTRAITS_0b0110011000u BIN, 408, (0, 1, 1, 0, 0, 1, 1, 0, 0, 0), 0b1001100111u
#define PPUTLUTRAITS_0b0110010111u BIN, 407, (0, 1, 1, 0, 0, 1, 0, 1, 1, 1), 0b1001101000u
#define PPUTLUTRAITS_0b0110010110u BIN, 406, (0, 1, 1, 0, 0, 1, 0, 1, 1, 0), 0b1001101001u
#define PPUTLUTRAITS_0b0110010101u BIN, 405, (0, 1, 1, 0, 0, 1, 0, 1, 0, 1), 0b1001101010u
#define PPUTLUTRAITS_0b0110010100u BIN, 404, (0, 1, 1, 0, 0, 1, 0, 1, 0, 0), 0b1001101011u
#define PPUTLUTRAITS_0b0110010011u BIN, 403, (0, 1, 1, 0, 0, 1, 0, 0, 1, 1), 0b1001101100u
#define PPUTLUTRAITS_0b0110010010u BIN, 402, (0, 1, 1, 0, 0, 1, 0, 0, 1, 0), 0b1001101101u
#define PPUTLUTRAITS_0b0110010001u BIN, 401, (0, 1, 1, 0, 0, 1, 0, 0, 0, 1), 0b1001101110u
#define PPUTLUTRAITS_0b0110010000u BIN, 400, (0, 1, 1, 0, 0, 1, 0, 0, 0, 0), 0b1001101111u
#define PPUTLUTRAITS_0b0110001111u BIN, 399, (0, 1, 1, 0, 0, 0, 1, 1, 1, 1), 0b1001110000u
#define PPUTLUTRAITS_0b0110001110u BIN, 398, (0, 1, 1, 0, 0, 0, 1, 1, 1, 0), 0b1001110001u
#define PPUTLUTRAITS_0b0110001101u BIN, 397, (0, 1, 1, 0, 0, 0, 1, 1, 0, 1), 0b1001110010u
#define PPUTLUTRAITS_0b0110001100u BIN, 396, (0, 1, 1, 0, 0, 0, 1, 1, 0, 0), 0b1001110011u
#define PPUTLUTRAITS_0b0110001011u BIN, 395, (0, 1, 1, 0, 0, 0, 1, 0, 1, 1), 0b1001110100u
#define PPUTLUTRAITS_0b0110001010u BIN, 394, (0, 1, 1, 0, 0, 0, 1, 0, 1, 0), 0b1001110101u
#define PPUTLUTRAITS_0b0110001001u BIN, 393, (0, 1, 1, 0, 0, 0, 1, 0, 0, 1), 0b1001110110u
#define PPUTLUTRAITS_0b0110001000u BIN, 392, (0, 1, 1, 0, 0, 0, 1, 0, 0, 0), 0b1001110111u
#define PPUTLUTRAITS_0b0110000111u BIN, 391, (0, 1, 1, 0, 0, 0, 0, 1, 1, 1), 0b1001111000u
#define PPUTLUTRAITS_0b0110000110u BIN, 390, (0, 1, 1, 0, 0, 0, 0, 1, 1, 0), 0b1001111001u
#define PPUTLUTRAITS_0b0110000101u BIN, 389, (0, 1, 1, 0, 0, 0, 0, 1, 0, 1), 0b1001111010u
#define PPUTLUTRAITS_0b0110000100u BIN, 388, (0, 1, 1, 0, 0, 0, 0, 1, 0, 0), 0b1001111011u
#define PPUTLUTRAITS_0b0110000011u BIN, 387, (0, 1, 1, 0, 0, 0, 0, 0, 1, 1), 0b1001111100u
#define PPUTLUTRAITS_0b0110000010u BIN, 386, (0, 1, 1, 0, 0, 0, 0, 0, 1, 0), 0b1001111101u
#define PPUTLUTRAITS_0b0110000001u BIN, 385, (0, 1, 1, 0, 0, 0, 0, 0, 0, 1), 0b1001111110u
#define PPUTLUTRAITS_0b0110000000u BIN, 384, (0, 1, 1, 0, 0, 0, 0, 0, 0, 0), 0b1001111111u
#define PPUTLUTRAITS_0b0101111111u BIN, 383, (0, 1, 0, 1, 1, 1, 1, 1, 1, 1), 0b1010000000u
#define PPUTLUTRAITS_0b0101111110u BIN, 382, (0, 1, 0, 1, 1, 1, 1, 1, 1, 0), 0b1010000001u
#define PPUTLUTRAITS_0b0101111101u BIN, 381, (0, 1, 0, 1, 1, 1, 1, 1, 0, 1), 0b1010000010u
#define PPUTLUTRAITS_0b0101111100u BIN, 380, (0, 1, 0, 1, 1, 1, 1, 1, 0, 0), 0b1010000011u
#define PPUTLUTRAITS_0b0101111011u BIN, 379, (0, 1, 0, 1, 1, 1, 1, 0, 1, 1), 0b1010000100u
#define PPUTLUTRAITS_0b0101111010u BIN, 378, (0, 1, 0, 1, 1, 1, 1, 0, 1, 0), 0b1010000101u
#define PPUTLUTRAITS_0b0101111001u BIN, 377, (0, 1, 0, 1, 1, 1, 1, 0, 0, 1), 0b1010000110u
#define PPUTLUTRAITS_0b0101111000u BIN, 376, (0, 1, 0, 1, 1, 1, 1, 0, 0, 0), 0b1010000111u
#define PPUTLUTRAITS_0b0101110111u BIN, 375, (0, 1, 0, 1, 1, 1, 0, 1, 1, 1), 0b1010001000u
#define PPUTLUTRAITS_0b0101110110u BIN, 374, (0, 1, 0, 1, 1, 1, 0, 1, 1, 0), 0b1010001001u
#define PPUTLUTRAITS_0b0101110101u BIN, 373, (0, 1, 0, 1, 1, 1, 0, 1, 0, 1), 0b1010001010u
#define PPUTLUTRAITS_0b0101110100u BIN, 372, (0, 1, 0, 1, 1, 1, 0, 1, 0, 0), 0b1010001011u
#define PPUTLUTRAITS_0b0101110011u BIN, 371, (0, 1, 0, 1, 1, 1, 0, 0, 1, 1), 0b1010001100u
#define PPUTLUTRAITS_0b0101110010u BIN, 370, (0, 1, 0, 1, 1, 1, 0, 0, 1, 0), 0b1010001101u
#define PPUTLUTRAITS_0b0101110001u BIN, 369, (0, 1, 0, 1, 1, 1, 0, 0, 0, 1), 0b1010001110u
#define PPUTLUTRAITS_0b0101110000u BIN, 368, (0, 1, 0, 1, 1, 1, 0, 0, 0, 0), 0b1010001111u
#define PPUTLUTRAITS_0b0101101111u BIN, 367, (0, 1, 0, 1, 1, 0, 1, 1, 1, 1), 0b1010010000u
#define PPUTLUTRAITS_0b0101101110u BIN, 366, (0, 1, 0, 1, 1, 0, 1, 1, 1, 0), 0b1010010001u
#define PPUTLUTRAITS_0b0101101101u BIN, 365, (0, 1, 0, 1, 1, 0, 1, 1, 0, 1), 0b1010010010u
#define PPUTLUTRAITS_0b0101101100u BIN, 364, (0, 1, 0, 1, 1, 0, 1, 1, 0, 0), 0b1010010011u
#define PPUTLUTRAITS_0b0101101011u BIN, 363, (0, 1, 0, 1, 1, 0, 1, 0, 1, 1), 0b1010010100u
#define PPUTLUTRAITS_0b0101101010u BIN, 362, (0, 1, 0, 1, 1, 0, 1, 0, 1, 0), 0b1010010101u
#define PPUTLUTRAITS_0b0101101001u BIN, 361, (0, 1, 0, 1, 1, 0, 1, 0, 0, 1), 0b1010010110u
#define PPUTLUTRAITS_0b0101101000u BIN, 360, (0, 1, 0, 1, 1, 0, 1, 0, 0, 0), 0b1010010111u
#define PPUTLUTRAITS_0b0101100111u BIN, 359, (0, 1, 0, 1, 1, 0, 0, 1, 1, 1), 0b1010011000u
#define PPUTLUTRAITS_0b0101100110u BIN, 358, (0, 1, 0, 1, 1, 0, 0, 1, 1, 0), 0b1010011001u
#define PPUTLUTRAITS_0b0101100101u BIN, 357, (0, 1, 0, 1, 1, 0, 0, 1, 0, 1), 0b1010011010u
#define PPUTLUTRAITS_0b0101100100u BIN, 356, (0, 1, 0, 1, 1, 0, 0, 1, 0, 0), 0b1010011011u
#define PPUTLUTRAITS_0b0101100011u BIN, 355, (0, 1, 0, 1, 1, 0, 0, 0, 1, 1), 0b1010011100u
#define PPUTLUTRAITS_0b0101100010u BIN, 354, (0, 1, 0, 1, 1, 0, 0, 0, 1, 0), 0b1010011101u
#define PPUTLUTRAITS_0b0101100001u BIN, 353, (0, 1, 0, 1, 1, 0, 0, 0, 0, 1), 0b1010011110u
#define PPUTLUTRAITS_0b0101100000u BIN, 352, (0, 1, 0, 1, 1, 0, 0, 0, 0, 0), 0b1010011111u
#define PPUTLUTRAITS_0b0101011111u BIN, 351, (0, 1, 0, 1, 0, 1, 1, 1, 1, 1), 0b1010100000u
#define PPUTLUTRAITS_0b0101011110u BIN, 350, (0, 1, 0, 1, 0, 1, 1, 1, 1, 0), 0b1010100001u
#define PPUTLUTRAITS_0b0101011101u BIN, 349, (0, 1, 0, 1, 0, 1, 1, 1, 0, 1), 0b1010100010u
#define PPUTLUTRAITS_0b0101011100u BIN, 348, (0, 1, 0, 1, 0, 1, 1, 1, 0, 0), 0b1010100011u
#define PPUTLUTRAITS_0b0101011011u BIN, 347, (0, 1, 0, 1, 0, 1, 1, 0, 1, 1), 0b1010100100u
#define PPUTLUTRAITS_0b0101011010u BIN, 346, (0, 1, 0, 1, 0, 1, 1, 0, 1, 0), 0b1010100101u
#define PPUTLUTRAITS_0b0101011001u BIN, 345, (0, 1, 0, 1, 0, 1, 1, 0, 0, 1), 0b1010100110u
#define PPUTLUTRAITS_0b0101011000u BIN, 344, (0, 1, 0, 1, 0, 1, 1, 0, 0, 0), 0b1010100111u
#define PPUTLUTRAITS_0b0101010111u BIN, 343, (0, 1, 0, 1, 0, 1, 0, 1, 1, 1), 0b1010101000u
#define PPUTLUTRAITS_0b0101010110u BIN, 342, (0, 1, 0, 1, 0, 1, 0, 1, 1, 0), 0b1010101001u
#define PPUTLUTRAITS_0b0101010101u BIN, 341, (0, 1, 0, 1, 0, 1, 0, 1, 0, 1), 0b1010101010u
#define PPUTLUTRAITS_0b0101010100u BIN, 340, (0, 1, 0, 1, 0, 1, 0, 1, 0, 0), 0b1010101011u
#define PPUTLUTRAITS_0b0101010011u BIN, 339, (0, 1, 0, 1, 0, 1, 0, 0, 1, 1), 0b1010101100u
#define PPUTLUTRAITS_0b0101010010u BIN, 338, (0, 1, 0, 1, 0, 1, 0, 0, 1, 0), 0b1010101101u
#define PPUTLUTRAITS_0b0101010001u BIN, 337, (0, 1, 0, 1, 0, 1, 0, 0, 0, 1), 0b1010101110u
#define PPUTLUTRAITS_0b0101010000u BIN, 336, (0, 1, 0, 1, 0, 1, 0, 0, 0, 0), 0b1010101111u
#define PPUTLUTRAITS_0b0101001111u BIN, 335, (0, 1, 0, 1, 0, 0, 1, 1, 1, 1), 0b1010110000u
#define PPUTLUTRAITS_0b0101001110u BIN, 334, (0, 1, 0, 1, 0, 0, 1, 1, 1, 0), 0b1010110001u
#define PPUTLUTRAITS_0b0101001101u BIN, 333, (0, 1, 0, 1, 0, 0, 1, 1, 0, 1), 0b1010110010u
#define PPUTLUTRAITS_0b0101001100u BIN, 332, (0, 1, 0, 1, 0, 0, 1, 1, 0, 0), 0b1010110011u
#define PPUTLUTRAITS_0b0101001011u BIN, 331, (0, 1, 0, 1, 0, 0, 1, 0, 1, 1), 0b1010110100u
#define PPUTLUTRAITS_0b0101001010u BIN, 330, (0, 1, 0, 1, 0, 0, 1, 0, 1, 0), 0b1010110101u
#define PPUTLUTRAITS_0b0101001001u BIN, 329, (0, 1, 0, 1, 0, 0, 1, 0, 0, 1), 0b1010110110u
#define PPUTLUTRAITS_0b0101001000u BIN, 328, (0, 1, 0, 1, 0, 0, 1, 0, 0, 0), 0b1010110111u
#define PPUTLUTRAITS_0b0101000111u BIN, 327, (0, 1, 0, 1, 0, 0, 0, 1, 1, 1), 0b1010111000u
#define PPUTLUTRAITS_0b0101000110u BIN, 326, (0, 1, 0, 1, 0, 0, 0, 1, 1, 0), 0b1010111001u
#define PPUTLUTRAITS_0b0101000101u BIN, 325, (0, 1, 0, 1, 0, 0, 0, 1, 0, 1), 0b1010111010u
#define PPUTLUTRAITS_0b0101000100u BIN, 324, (0, 1, 0, 1, 0, 0, 0, 1, 0, 0), 0b1010111011u
#define PPUTLUTRAITS_0b0101000011u BIN, 323, (0, 1, 0, 1, 0, 0, 0, 0, 1, 1), 0b1010111100u
#define PPUTLUTRAITS_0b0101000010u BIN, 322, (0, 1, 0, 1, 0, 0, 0, 0, 1, 0), 0b1010111101u
#define PPUTLUTRAITS_0b0101000001u BIN, 321, (0, 1, 0, 1, 0, 0, 0, 0, 0, 1), 0b1010111110u
#define PPUTLUTRAITS_0b0101000000u BIN, 320, (0, 1, 0, 1, 0, 0, 0, 0, 0, 0), 0b1010111111u
#define PPUTLUTRAITS_0b0100111111u BIN, 319, (0, 1, 0, 0, 1, 1, 1, 1, 1, 1), 0b1011000000u
#define PPUTLUTRAITS_0b0100111110u BIN, 318, (0, 1, 0, 0, 1, 1, 1, 1, 1, 0), 0b1011000001u
#define PPUTLUTRAITS_0b0100111101u BIN, 317, (0, 1, 0, 0, 1, 1, 1, 1, 0, 1), 0b1011000010u
#define PPUTLUTRAITS_0b0100111100u BIN, 316, (0, 1, 0, 0, 1, 1, 1, 1, 0, 0), 0b1011000011u
#define PPUTLUTRAITS_0b0100111011u BIN, 315, (0, 1, 0, 0, 1, 1, 1, 0, 1, 1), 0b1011000100u
#define PPUTLUTRAITS_0b0100111010u BIN, 314, (0, 1, 0, 0, 1, 1, 1, 0, 1, 0), 0b1011000101u
#define PPUTLUTRAITS_0b0100111001u BIN, 313, (0, 1, 0, 0, 1, 1, 1, 0, 0, 1), 0b1011000110u
#define PPUTLUTRAITS_0b0100111000u BIN, 312, (0, 1, 0, 0, 1, 1, 1, 0, 0, 0), 0b1011000111u
#define PPUTLUTRAITS_0b0100110111u BIN, 311, (0, 1, 0, 0, 1, 1, 0, 1, 1, 1), 0b1011001000u
#define PPUTLUTRAITS_0b0100110110u BIN, 310, (0, 1, 0, 0, 1, 1, 0, 1, 1, 0), 0b1011001001u
#define PPUTLUTRAITS_0b0100110101u BIN, 309, (0, 1, 0, 0, 1, 1, 0, 1, 0, 1), 0b1011001010u
#define PPUTLUTRAITS_0b0100110100u BIN, 308, (0, 1, 0, 0, 1, 1, 0, 1, 0, 0), 0b1011001011u
#define PPUTLUTRAITS_0b0100110011u BIN, 307, (0, 1, 0, 0, 1, 1, 0, 0, 1, 1), 0b1011001100u
#define PPUTLUTRAITS_0b0100110010u BIN, 306, (0, 1, 0, 0, 1, 1, 0, 0, 1, 0), 0b1011001101u
#define PPUTLUTRAITS_0b0100110001u BIN, 305, (0, 1, 0, 0, 1, 1, 0, 0, 0, 1), 0b1011001110u
#define PPUTLUTRAITS_0b0100110000u BIN, 304, (0, 1, 0, 0, 1, 1, 0, 0, 0, 0), 0b1011001111u
#define PPUTLUTRAITS_0b0100101111u BIN, 303, (0, 1, 0, 0, 1, 0, 1, 1, 1, 1), 0b1011010000u
#define PPUTLUTRAITS_0b0100101110u BIN, 302, (0, 1, 0, 0, 1, 0, 1, 1, 1, 0), 0b1011010001u
#define PPUTLUTRAITS_0b0100101101u BIN, 301, (0, 1, 0, 0, 1, 0, 1, 1, 0, 1), 0b1011010010u
#define PPUTLUTRAITS_0b0100101100u BIN, 300, (0, 1, 0, 0, 1, 0, 1, 1, 0, 0), 0b1011010011u
#define PPUTLUTRAITS_0b0100101011u BIN, 299, (0, 1, 0, 0, 1, 0, 1, 0, 1, 1), 0b1011010100u
#define PPUTLUTRAITS_0b0100101010u BIN, 298, (0, 1, 0, 0, 1, 0, 1, 0, 1, 0), 0b1011010101u
#define PPUTLUTRAITS_0b0100101001u BIN, 297, (0, 1, 0, 0, 1, 0, 1, 0, 0, 1), 0b1011010110u
#define PPUTLUTRAITS_0b0100101000u BIN, 296, (0, 1, 0, 0, 1, 0, 1, 0, 0, 0), 0b1011010111u
#define PPUTLUTRAITS_0b0100100111u BIN, 295, (0, 1, 0, 0, 1, 0, 0, 1, 1, 1), 0b1011011000u
#define PPUTLUTRAITS_0b0100100110u BIN, 294, (0, 1, 0, 0, 1, 0, 0, 1, 1, 0), 0b1011011001u
#define PPUTLUTRAITS_0b0100100101u BIN, 293, (0, 1, 0, 0, 1, 0, 0, 1, 0, 1), 0b1011011010u
#define PPUTLUTRAITS_0b0100100100u BIN, 292, (0, 1, 0, 0, 1, 0, 0, 1, 0, 0), 0b1011011011u
#define PPUTLUTRAITS_0b0100100011u BIN, 291, (0, 1, 0, 0, 1, 0, 0, 0, 1, 1), 0b1011011100u
#define PPUTLUTRAITS_0b0100100010u BIN, 290, (0, 1, 0, 0, 1, 0, 0, 0, 1, 0), 0b1011011101u
#define PPUTLUTRAITS_0b0100100001u BIN, 289, (0, 1, 0, 0, 1, 0, 0, 0, 0, 1), 0b1011011110u
#define PPUTLUTRAITS_0b0100100000u BIN, 288, (0, 1, 0, 0, 1, 0, 0, 0, 0, 0), 0b1011011111u
#define PPUTLUTRAITS_0b0100011111u BIN, 287, (0, 1, 0, 0, 0, 1, 1, 1, 1, 1), 0b1011100000u
#define PPUTLUTRAITS_0b0100011110u BIN, 286, (0, 1, 0, 0, 0, 1, 1, 1, 1, 0), 0b1011100001u
#define PPUTLUTRAITS_0b0100011101u BIN, 285, (0, 1, 0, 0, 0, 1, 1, 1, 0, 1), 0b1011100010u
#define PPUTLUTRAITS_0b0100011100u BIN, 284, (0, 1, 0, 0, 0, 1, 1, 1, 0, 0), 0b1011100011u
#define PPUTLUTRAITS_0b0100011011u BIN, 283, (0, 1, 0, 0, 0, 1, 1, 0, 1, 1), 0b1011100100u
#define PPUTLUTRAITS_0b0100011010u BIN, 282, (0, 1, 0, 0, 0, 1, 1, 0, 1, 0), 0b1011100101u
#define PPUTLUTRAITS_0b0100011001u BIN, 281, (0, 1, 0, 0, 0, 1, 1, 0, 0, 1), 0b1011100110u
#define PPUTLUTRAITS_0b0100011000u BIN, 280, (0, 1, 0, 0, 0, 1, 1, 0, 0, 0), 0b1011100111u
#define PPUTLUTRAITS_0b0100010111u BIN, 279, (0, 1, 0, 0, 0, 1, 0, 1, 1, 1), 0b1011101000u
#define PPUTLUTRAITS_0b0100010110u BIN, 278, (0, 1, 0, 0, 0, 1, 0, 1, 1, 0), 0b1011101001u
#define PPUTLUTRAITS_0b0100010101u BIN, 277, (0, 1, 0, 0, 0, 1, 0, 1, 0, 1), 0b1011101010u
#define PPUTLUTRAITS_0b0100010100u BIN, 276, (0, 1, 0, 0, 0, 1, 0, 1, 0, 0), 0b1011101011u
#define PPUTLUTRAITS_0b0100010011u BIN, 275, (0, 1, 0, 0, 0, 1, 0, 0, 1, 1), 0b1011101100u
#define PPUTLUTRAITS_0b0100010010u BIN, 274, (0, 1, 0, 0, 0, 1, 0, 0, 1, 0), 0b1011101101u
#define PPUTLUTRAITS_0b0100010001u BIN, 273, (0, 1, 0, 0, 0, 1, 0, 0, 0, 1), 0b1011101110u
#define PPUTLUTRAITS_0b0100010000u BIN, 272, (0, 1, 0, 0, 0, 1, 0, 0, 0, 0), 0b1011101111u
#define PPUTLUTRAITS_0b0100001111u BIN, 271, (0, 1, 0, 0, 0, 0, 1, 1, 1, 1), 0b1011110000u
#define PPUTLUTRAITS_0b0100001110u BIN, 270, (0, 1, 0, 0, 0, 0, 1, 1, 1, 0), 0b1011110001u
#define PPUTLUTRAITS_0b0100001101u BIN, 269, (0, 1, 0, 0, 0, 0, 1, 1, 0, 1), 0b1011110010u
#define PPUTLUTRAITS_0b0100001100u BIN, 268, (0, 1, 0, 0, 0, 0, 1, 1, 0, 0), 0b1011110011u
#define PPUTLUTRAITS_0b0100001011u BIN, 267, (0, 1, 0, 0, 0, 0, 1, 0, 1, 1), 0b1011110100u
#define PPUTLUTRAITS_0b0100001010u BIN, 266, (0, 1, 0, 0, 0, 0, 1, 0, 1, 0), 0b1011110101u
#define PPUTLUTRAITS_0b0100001001u BIN, 265, (0, 1, 0, 0, 0, 0, 1, 0, 0, 1), 0b1011110110u
#define PPUTLUTRAITS_0b0100001000u BIN, 264, (0, 1, 0, 0, 0, 0, 1, 0, 0, 0), 0b1011110111u
#define PPUTLUTRAITS_0b0100000111u BIN, 263, (0, 1, 0, 0, 0, 0, 0, 1, 1, 1), 0b1011111000u
#define PPUTLUTRAITS_0b0100000110u BIN, 262, (0, 1, 0, 0, 0, 0, 0, 1, 1, 0), 0b1011111001u
#define PPUTLUTRAITS_0b0100000101u BIN, 261, (0, 1, 0, 0, 0, 0, 0, 1, 0, 1), 0b1011111010u
#define PPUTLUTRAITS_0b0100000100u BIN, 260, (0, 1, 0, 0, 0, 0, 0, 1, 0, 0), 0b1011111011u
#define PPUTLUTRAITS_0b0100000011u BIN, 259, (0, 1, 0, 0, 0, 0, 0, 0, 1, 1), 0b1011111100u
#define PPUTLUTRAITS_0b0100000010u BIN, 258, (0, 1, 0, 0, 0, 0, 0, 0, 1, 0), 0b1011111101u
#define PPUTLUTRAITS_0b0100000001u BIN, 257, (0, 1, 0, 0, 0, 0, 0, 0, 0, 1), 0b1011111110u
#define PPUTLUTRAITS_0b0100000000u BIN, 256, (0, 1, 0, 0, 0, 0, 0, 0, 0, 0), 0b1011111111u
#define PPUTLUTRAITS_0b0011111111u BIN, 255, (0, 0, 1, 1, 1, 1, 1, 1, 1, 1), 0b1100000000u
#define PPUTLUTRAITS_0b0011111110u BIN, 254, (0, 0, 1, 1, 1, 1, 1, 1, 1, 0), 0b1100000001u
#define PPUTLUTRAITS_0b0011111101u BIN, 253, (0, 0, 1, 1, 1, 1, 1, 1, 0, 1), 0b1100000010u
#define PPUTLUTRAITS_0b0011111100u BIN, 252, (0, 0, 1, 1, 1, 1, 1, 1, 0, 0), 0b1100000011u
#define PPUTLUTRAITS_0b0011111011u BIN, 251, (0, 0, 1, 1, 1, 1, 1, 0, 1, 1), 0b1100000100u
#define PPUTLUTRAITS_0b0011111010u BIN, 250, (0, 0, 1, 1, 1, 1, 1, 0, 1, 0), 0b1100000101u
#define PPUTLUTRAITS_0b0011111001u BIN, 249, (0, 0, 1, 1, 1, 1, 1, 0, 0, 1), 0b1100000110u
#define PPUTLUTRAITS_0b0011111000u BIN, 248, (0, 0, 1, 1, 1, 1, 1, 0, 0, 0), 0b1100000111u
#define PPUTLUTRAITS_0b0011110111u BIN, 247, (0, 0, 1, 1, 1, 1, 0, 1, 1, 1), 0b1100001000u
#define PPUTLUTRAITS_0b0011110110u BIN, 246, (0, 0, 1, 1, 1, 1, 0, 1, 1, 0), 0b1100001001u
#define PPUTLUTRAITS_0b0011110101u BIN, 245, (0, 0, 1, 1, 1, 1, 0, 1, 0, 1), 0b1100001010u
#define PPUTLUTRAITS_0b0011110100u BIN, 244, (0, 0, 1, 1, 1, 1, 0, 1, 0, 0), 0b1100001011u
#define PPUTLUTRAITS_0b0011110011u BIN, 243, (0, 0, 1, 1, 1, 1, 0, 0, 1, 1), 0b1100001100u
#define PPUTLUTRAITS_0b0011110010u BIN, 242, (0, 0, 1, 1, 1, 1, 0, 0, 1, 0), 0b1100001101u
#define PPUTLUTRAITS_0b0011110001u BIN, 241, (0, 0, 1, 1, 1, 1, 0, 0, 0, 1), 0b1100001110u
#define PPUTLUTRAITS_0b0011110000u BIN, 240, (0, 0, 1, 1, 1, 1, 0, 0, 0, 0), 0b1100001111u
#define PPUTLUTRAITS_0b0011101111u BIN, 239, (0, 0, 1, 1, 1, 0, 1, 1, 1, 1), 0b1100010000u
#define PPUTLUTRAITS_0b0011101110u BIN, 238, (0, 0, 1, 1, 1, 0, 1, 1, 1, 0), 0b1100010001u
#define PPUTLUTRAITS_0b0011101101u BIN, 237, (0, 0, 1, 1, 1, 0, 1, 1, 0, 1), 0b1100010010u
#define PPUTLUTRAITS_0b0011101100u BIN, 236, (0, 0, 1, 1, 1, 0, 1, 1, 0, 0), 0b1100010011u
#define PPUTLUTRAITS_0b0011101011u BIN, 235, (0, 0, 1, 1, 1, 0, 1, 0, 1, 1), 0b1100010100u
#define PPUTLUTRAITS_0b0011101010u BIN, 234, (0, 0, 1, 1, 1, 0, 1, 0, 1, 0), 0b1100010101u
#define PPUTLUTRAITS_0b0011101001u BIN, 233, (0, 0, 1, 1, 1, 0, 1, 0, 0, 1), 0b1100010110u
#define PPUTLUTRAITS_0b0011101000u BIN, 232, (0, 0, 1, 1, 1, 0, 1, 0, 0, 0), 0b1100010111u
#define PPUTLUTRAITS_0b0011100111u BIN, 231, (0, 0, 1, 1, 1, 0, 0, 1, 1, 1), 0b1100011000u
#define PPUTLUTRAITS_0b0011100110u BIN, 230, (0, 0, 1, 1, 1, 0, 0, 1, 1, 0), 0b1100011001u
#define PPUTLUTRAITS_0b0011100101u BIN, 229, (0, 0, 1, 1, 1, 0, 0, 1, 0, 1), 0b1100011010u
#define PPUTLUTRAITS_0b0011100100u BIN, 228, (0, 0, 1, 1, 1, 0, 0, 1, 0, 0), 0b1100011011u
#define PPUTLUTRAITS_0b0011100011u BIN, 227, (0, 0, 1, 1, 1, 0, 0, 0, 1, 1), 0b1100011100u
#define PPUTLUTRAITS_0b0011100010u BIN, 226, (0, 0, 1, 1, 1, 0, 0, 0, 1, 0), 0b1100011101u
#define PPUTLUTRAITS_0b0011100001u BIN, 225, (0, 0, 1, 1, 1, 0, 0, 0, 0, 1), 0b1100011110u
#define PPUTLUTRAITS_0b0011100000u BIN, 224, (0, 0, 1, 1, 1, 0, 0, 0, 0, 0), 0b1100011111u
#define PPUTLUTRAITS_0b0011011111u BIN, 223, (0, 0, 1, 1, 0, 1, 1, 1, 1, 1), 0b1100100000u
#define PPUTLUTRAITS_0b0011011110u BIN, 222, (0, 0, 1, 1, 0, 1, 1, 1, 1, 0), 0b1100100001u
#define PPUTLUTRAITS_0b0011011101u BIN, 221, (0, 0, 1, 1, 0, 1, 1, 1, 0, 1), 0b1100100010u
#define PPUTLUTRAITS_0b0011011100u BIN, 220, (0, 0, 1, 1, 0, 1, 1, 1, 0, 0), 0b1100100011u
#define PPUTLUTRAITS_0b0011011011u BIN, 219, (0, 0, 1, 1, 0, 1, 1, 0, 1, 1), 0b1100100100u
#define PPUTLUTRAITS_0b0011011010u BIN, 218, (0, 0, 1, 1, 0, 1, 1, 0, 1, 0), 0b1100100101u
#define PPUTLUTRAITS_0b0011011001u BIN, 217, (0, 0, 1, 1, 0, 1, 1, 0, 0, 1), 0b1100100110u
#define PPUTLUTRAITS_0b0011011000u BIN, 216, (0, 0, 1, 1, 0, 1, 1, 0, 0, 0), 0b1100100111u
#define PPUTLUTRAITS_0b0011010111u BIN, 215, (0, 0, 1, 1, 0, 1, 0, 1, 1, 1), 0b1100101000u
#define PPUTLUTRAITS_0b0011010110u BIN, 214, (0, 0, 1, 1, 0, 1, 0, 1, 1, 0), 0b1100101001u
#define PPUTLUTRAITS_0b0011010101u BIN, 213, (0, 0, 1, 1, 0, 1, 0, 1, 0, 1), 0b1100101010u
#define PPUTLUTRAITS_0b0011010100u BIN, 212, (0, 0, 1, 1, 0, 1, 0, 1, 0, 0), 0b1100101011u
#define PPUTLUTRAITS_0b0011010011u BIN, 211, (0, 0, 1, 1, 0, 1, 0, 0, 1, 1), 0b1100101100u
#define PPUTLUTRAITS_0b0011010010u BIN, 210, (0, 0, 1, 1, 0, 1, 0, 0, 1, 0), 0b1100101101u
#define PPUTLUTRAITS_0b0011010001u BIN, 209, (0, 0, 1, 1, 0, 1, 0, 0, 0, 1), 0b1100101110u
#define PPUTLUTRAITS_0b0011010000u BIN, 208, (0, 0, 1, 1, 0, 1, 0, 0, 0, 0), 0b1100101111u
#define PPUTLUTRAITS_0b0011001111u BIN, 207, (0, 0, 1, 1, 0, 0, 1, 1, 1, 1), 0b1100110000u
#define PPUTLUTRAITS_0b0011001110u BIN, 206, (0, 0, 1, 1, 0, 0, 1, 1, 1, 0), 0b1100110001u
#define PPUTLUTRAITS_0b0011001101u BIN, 205, (0, 0, 1, 1, 0, 0, 1, 1, 0, 1), 0b1100110010u
#define PPUTLUTRAITS_0b0011001100u BIN, 204, (0, 0, 1, 1, 0, 0, 1, 1, 0, 0), 0b1100110011u
#define PPUTLUTRAITS_0b0011001011u BIN, 203, (0, 0, 1, 1, 0, 0, 1, 0, 1, 1), 0b1100110100u
#define PPUTLUTRAITS_0b0011001010u BIN, 202, (0, 0, 1, 1, 0, 0, 1, 0, 1, 0), 0b1100110101u
#define PPUTLUTRAITS_0b0011001001u BIN, 201, (0, 0, 1, 1, 0, 0, 1, 0, 0, 1), 0b1100110110u
#define PPUTLUTRAITS_0b0011001000u BIN, 200, (0, 0, 1, 1, 0, 0, 1, 0, 0, 0), 0b1100110111u
#define PPUTLUTRAITS_0b0011000111u BIN, 199, (0, 0, 1, 1, 0, 0, 0, 1, 1, 1), 0b1100111000u
#define PPUTLUTRAITS_0b0011000110u BIN, 198, (0, 0, 1, 1, 0, 0, 0, 1, 1, 0), 0b1100111001u
#define PPUTLUTRAITS_0b0011000101u BIN, 197, (0, 0, 1, 1, 0, 0, 0, 1, 0, 1), 0b1100111010u
#define PPUTLUTRAITS_0b0011000100u BIN, 196, (0, 0, 1, 1, 0, 0, 0, 1, 0, 0), 0b1100111011u
#define PPUTLUTRAITS_0b0011000011u BIN, 195, (0, 0, 1, 1, 0, 0, 0, 0, 1, 1), 0b1100111100u
#define PPUTLUTRAITS_0b0011000010u BIN, 194, (0, 0, 1, 1, 0, 0, 0, 0, 1, 0), 0b1100111101u
#define PPUTLUTRAITS_0b0011000001u BIN, 193, (0, 0, 1, 1, 0, 0, 0, 0, 0, 1), 0b1100111110u
#define PPUTLUTRAITS_0b0011000000u BIN, 192, (0, 0, 1, 1, 0, 0, 0, 0, 0, 0), 0b1100111111u
#define PPUTLUTRAITS_0b0010111111u BIN, 191, (0, 0, 1, 0, 1, 1, 1, 1, 1, 1), 0b1101000000u
#define PPUTLUTRAITS_0b0010111110u BIN, 190, (0, 0, 1, 0, 1, 1, 1, 1, 1, 0), 0b1101000001u
#define PPUTLUTRAITS_0b0010111101u BIN, 189, (0, 0, 1, 0, 1, 1, 1, 1, 0, 1), 0b1101000010u
#define PPUTLUTRAITS_0b0010111100u BIN, 188, (0, 0, 1, 0, 1, 1, 1, 1, 0, 0), 0b1101000011u
#define PPUTLUTRAITS_0b0010111011u BIN, 187, (0, 0, 1, 0, 1, 1, 1, 0, 1, 1), 0b1101000100u
#define PPUTLUTRAITS_0b0010111010u BIN, 186, (0, 0, 1, 0, 1, 1, 1, 0, 1, 0), 0b1101000101u
#define PPUTLUTRAITS_0b0010111001u BIN, 185, (0, 0, 1, 0, 1, 1, 1, 0, 0, 1), 0b1101000110u
#define PPUTLUTRAITS_0b0010111000u BIN, 184, (0, 0, 1, 0, 1, 1, 1, 0, 0, 0), 0b1101000111u
#define PPUTLUTRAITS_0b0010110111u BIN, 183, (0, 0, 1, 0, 1, 1, 0, 1, 1, 1), 0b1101001000u
#define PPUTLUTRAITS_0b0010110110u BIN, 182, (0, 0, 1, 0, 1, 1, 0, 1, 1, 0), 0b1101001001u
#define PPUTLUTRAITS_0b0010110101u BIN, 181, (0, 0, 1, 0, 1, 1, 0, 1, 0, 1), 0b1101001010u
#define PPUTLUTRAITS_0b0010110100u BIN, 180, (0, 0, 1, 0, 1, 1, 0, 1, 0, 0), 0b1101001011u
#define PPUTLUTRAITS_0b0010110011u BIN, 179, (0, 0, 1, 0, 1, 1, 0, 0, 1, 1), 0b1101001100u
#define PPUTLUTRAITS_0b0010110010u BIN, 178, (0, 0, 1, 0, 1, 1, 0, 0, 1, 0), 0b1101001101u
#define PPUTLUTRAITS_0b0010110001u BIN, 177, (0, 0, 1, 0, 1, 1, 0, 0, 0, 1), 0b1101001110u
#define PPUTLUTRAITS_0b0010110000u BIN, 176, (0, 0, 1, 0, 1, 1, 0, 0, 0, 0), 0b1101001111u
#define PPUTLUTRAITS_0b0010101111u BIN, 175, (0, 0, 1, 0, 1, 0, 1, 1, 1, 1), 0b1101010000u
#define PPUTLUTRAITS_0b0010101110u BIN, 174, (0, 0, 1, 0, 1, 0, 1, 1, 1, 0), 0b1101010001u
#define PPUTLUTRAITS_0b0010101101u BIN, 173, (0, 0, 1, 0, 1, 0, 1, 1, 0, 1), 0b1101010010u
#define PPUTLUTRAITS_0b0010101100u BIN, 172, (0, 0, 1, 0, 1, 0, 1, 1, 0, 0), 0b1101010011u
#define PPUTLUTRAITS_0b0010101011u BIN, 171, (0, 0, 1, 0, 1, 0, 1, 0, 1, 1), 0b1101010100u
#define PPUTLUTRAITS_0b0010101010u BIN, 170, (0, 0, 1, 0, 1, 0, 1, 0, 1, 0), 0b1101010101u
#define PPUTLUTRAITS_0b0010101001u BIN, 169, (0, 0, 1, 0, 1, 0, 1, 0, 0, 1), 0b1101010110u
#define PPUTLUTRAITS_0b0010101000u BIN, 168, (0, 0, 1, 0, 1, 0, 1, 0, 0, 0), 0b1101010111u
#define PPUTLUTRAITS_0b0010100111u BIN, 167, (0, 0, 1, 0, 1, 0, 0, 1, 1, 1), 0b1101011000u
#define PPUTLUTRAITS_0b0010100110u BIN, 166, (0, 0, 1, 0, 1, 0, 0, 1, 1, 0), 0b1101011001u
#define PPUTLUTRAITS_0b0010100101u BIN, 165, (0, 0, 1, 0, 1, 0, 0, 1, 0, 1), 0b1101011010u
#define PPUTLUTRAITS_0b0010100100u BIN, 164, (0, 0, 1, 0, 1, 0, 0, 1, 0, 0), 0b1101011011u
#define PPUTLUTRAITS_0b0010100011u BIN, 163, (0, 0, 1, 0, 1, 0, 0, 0, 1, 1), 0b1101011100u
#define PPUTLUTRAITS_0b0010100010u BIN, 162, (0, 0, 1, 0, 1, 0, 0, 0, 1, 0), 0b1101011101u
#define PPUTLUTRAITS_0b0010100001u BIN, 161, (0, 0, 1, 0, 1, 0, 0, 0, 0, 1), 0b1101011110u
#define PPUTLUTRAITS_0b0010100000u BIN, 160, (0, 0, 1, 0, 1, 0, 0, 0, 0, 0), 0b1101011111u
#define PPUTLUTRAITS_0b0010011111u BIN, 159, (0, 0, 1, 0, 0, 1, 1, 1, 1, 1), 0b1101100000u
#define PPUTLUTRAITS_0b0010011110u BIN, 158, (0, 0, 1, 0, 0, 1, 1, 1, 1, 0), 0b1101100001u
#define PPUTLUTRAITS_0b0010011101u BIN, 157, (0, 0, 1, 0, 0, 1, 1, 1, 0, 1), 0b1101100010u
#define PPUTLUTRAITS_0b0010011100u BIN, 156, (0, 0, 1, 0, 0, 1, 1, 1, 0, 0), 0b1101100011u
#define PPUTLUTRAITS_0b0010011011u BIN, 155, (0, 0, 1, 0, 0, 1, 1, 0, 1, 1), 0b1101100100u
#define PPUTLUTRAITS_0b0010011010u BIN, 154, (0, 0, 1, 0, 0, 1, 1, 0, 1, 0), 0b1101100101u
#define PPUTLUTRAITS_0b0010011001u BIN, 153, (0, 0, 1, 0, 0, 1, 1, 0, 0, 1), 0b1101100110u
#define PPUTLUTRAITS_0b0010011000u BIN, 152, (0, 0, 1, 0, 0, 1, 1, 0, 0, 0), 0b1101100111u
#define PPUTLUTRAITS_0b0010010111u BIN, 151, (0, 0, 1, 0, 0, 1, 0, 1, 1, 1), 0b1101101000u
#define PPUTLUTRAITS_0b0010010110u BIN, 150, (0, 0, 1, 0, 0, 1, 0, 1, 1, 0), 0b1101101001u
#define PPUTLUTRAITS_0b0010010101u BIN, 149, (0, 0, 1, 0, 0, 1, 0, 1, 0, 1), 0b1101101010u
#define PPUTLUTRAITS_0b0010010100u BIN, 148, (0, 0, 1, 0, 0, 1, 0, 1, 0, 0), 0b1101101011u
#define PPUTLUTRAITS_0b0010010011u BIN, 147, (0, 0, 1, 0, 0, 1, 0, 0, 1, 1), 0b1101101100u
#define PPUTLUTRAITS_0b0010010010u BIN, 146, (0, 0, 1, 0, 0, 1, 0, 0, 1, 0), 0b1101101101u
#define PPUTLUTRAITS_0b0010010001u BIN, 145, (0, 0, 1, 0, 0, 1, 0, 0, 0, 1), 0b1101101110u
#define PPUTLUTRAITS_0b0010010000u BIN, 144, (0, 0, 1, 0, 0, 1, 0, 0, 0, 0), 0b1101101111u
#define PPUTLUTRAITS_0b0010001111u BIN, 143, (0, 0, 1, 0, 0, 0, 1, 1, 1, 1), 0b1101110000u
#define PPUTLUTRAITS_0b0010001110u BIN, 142, (0, 0, 1, 0, 0, 0, 1, 1, 1, 0), 0b1101110001u
#define PPUTLUTRAITS_0b0010001101u BIN, 141, (0, 0, 1, 0, 0, 0, 1, 1, 0, 1), 0b1101110010u
#define PPUTLUTRAITS_0b0010001100u BIN, 140, (0, 0, 1, 0, 0, 0, 1, 1, 0, 0), 0b1101110011u
#define PPUTLUTRAITS_0b0010001011u BIN, 139, (0, 0, 1, 0, 0, 0, 1, 0, 1, 1), 0b1101110100u
#define PPUTLUTRAITS_0b0010001010u BIN, 138, (0, 0, 1, 0, 0, 0, 1, 0, 1, 0), 0b1101110101u
#define PPUTLUTRAITS_0b0010001001u BIN, 137, (0, 0, 1, 0, 0, 0, 1, 0, 0, 1), 0b1101110110u
#define PPUTLUTRAITS_0b0010001000u BIN, 136, (0, 0, 1, 0, 0, 0, 1, 0, 0, 0), 0b1101110111u
#define PPUTLUTRAITS_0b0010000111u BIN, 135, (0, 0, 1, 0, 0, 0, 0, 1, 1, 1), 0b1101111000u
#define PPUTLUTRAITS_0b0010000110u BIN, 134, (0, 0, 1, 0, 0, 0, 0, 1, 1, 0), 0b1101111001u
#define PPUTLUTRAITS_0b0010000101u BIN, 133, (0, 0, 1, 0, 0, 0, 0, 1, 0, 1), 0b1101111010u
#define PPUTLUTRAITS_0b0010000100u BIN, 132, (0, 0, 1, 0, 0, 0, 0, 1, 0, 0), 0b1101111011u
#define PPUTLUTRAITS_0b0010000011u BIN, 131, (0, 0, 1, 0, 0, 0, 0, 0, 1, 1), 0b1101111100u
#define PPUTLUTRAITS_0b0010000010u BIN, 130, (0, 0, 1, 0, 0, 0, 0, 0, 1, 0), 0b1101111101u
#define PPUTLUTRAITS_0b0010000001u BIN, 129, (0, 0, 1, 0, 0, 0, 0, 0, 0, 1), 0b1101111110u
#define PPUTLUTRAITS_0b0010000000u BIN, 128, (0, 0, 1, 0, 0, 0, 0, 0, 0, 0), 0b1101111111u
#define PPUTLUTRAITS_0b0001111111u BIN, 127, (0, 0, 0, 1, 1, 1, 1, 1, 1, 1), 0b1110000000u
#define PPUTLUTRAITS_0b0001111110u BIN, 126, (0, 0, 0, 1, 1, 1, 1, 1, 1, 0), 0b1110000001u
#define PPUTLUTRAITS_0b0001111101u BIN, 125, (0, 0, 0, 1, 1, 1, 1, 1, 0, 1), 0b1110000010u
#define PPUTLUTRAITS_0b0001111100u BIN, 124, (0, 0, 0, 1, 1, 1, 1, 1, 0, 0), 0b1110000011u
#define PPUTLUTRAITS_0b0001111011u BIN, 123, (0, 0, 0, 1, 1, 1, 1, 0, 1, 1), 0b1110000100u
#define PPUTLUTRAITS_0b0001111010u BIN, 122, (0, 0, 0, 1, 1, 1, 1, 0, 1, 0), 0b1110000101u
#define PPUTLUTRAITS_0b0001111001u BIN, 121, (0, 0, 0, 1, 1, 1, 1, 0, 0, 1), 0b1110000110u
#define PPUTLUTRAITS_0b0001111000u BIN, 120, (0, 0, 0, 1, 1, 1, 1, 0, 0, 0), 0b1110000111u
#define PPUTLUTRAITS_0b0001110111u BIN, 119, (0, 0, 0, 1, 1, 1, 0, 1, 1, 1), 0b1110001000u
#define PPUTLUTRAITS_0b0001110110u BIN, 118, (0, 0, 0, 1, 1, 1, 0, 1, 1, 0), 0b1110001001u
#define PPUTLUTRAITS_0b0001110101u BIN, 117, (0, 0, 0, 1, 1, 1, 0, 1, 0, 1), 0b1110001010u
#define PPUTLUTRAITS_0b0001110100u BIN, 116, (0, 0, 0, 1, 1, 1, 0, 1, 0, 0), 0b1110001011u
#define PPUTLUTRAITS_0b0001110011u BIN, 115, (0, 0, 0, 1, 1, 1, 0, 0, 1, 1), 0b1110001100u
#define PPUTLUTRAITS_0b0001110010u BIN, 114, (0, 0, 0, 1, 1, 1, 0, 0, 1, 0), 0b1110001101u
#define PPUTLUTRAITS_0b0001110001u BIN, 113, (0, 0, 0, 1, 1, 1, 0, 0, 0, 1), 0b1110001110u
#define PPUTLUTRAITS_0b0001110000u BIN, 112, (0, 0, 0, 1, 1, 1, 0, 0, 0, 0), 0b1110001111u
#define PPUTLUTRAITS_0b0001101111u BIN, 111, (0, 0, 0, 1, 1, 0, 1, 1, 1, 1), 0b1110010000u
#define PPUTLUTRAITS_0b0001101110u BIN, 110, (0, 0, 0, 1, 1, 0, 1, 1, 1, 0), 0b1110010001u
#define PPUTLUTRAITS_0b0001101101u BIN, 109, (0, 0, 0, 1, 1, 0, 1, 1, 0, 1), 0b1110010010u
#define PPUTLUTRAITS_0b0001101100u BIN, 108, (0, 0, 0, 1, 1, 0, 1, 1, 0, 0), 0b1110010011u
#define PPUTLUTRAITS_0b0001101011u BIN, 107, (0, 0, 0, 1, 1, 0, 1, 0, 1, 1), 0b1110010100u
#define PPUTLUTRAITS_0b0001101010u BIN, 106, (0, 0, 0, 1, 1, 0, 1, 0, 1, 0), 0b1110010101u
#define PPUTLUTRAITS_0b0001101001u BIN, 105, (0, 0, 0, 1, 1, 0, 1, 0, 0, 1), 0b1110010110u
#define PPUTLUTRAITS_0b0001101000u BIN, 104, (0, 0, 0, 1, 1, 0, 1, 0, 0, 0), 0b1110010111u
#define PPUTLUTRAITS_0b0001100111u BIN, 103, (0, 0, 0, 1, 1, 0, 0, 1, 1, 1), 0b1110011000u
#define PPUTLUTRAITS_0b0001100110u BIN, 102, (0, 0, 0, 1, 1, 0, 0, 1, 1, 0), 0b1110011001u
#define PPUTLUTRAITS_0b0001100101u BIN, 101, (0, 0, 0, 1, 1, 0, 0, 1, 0, 1), 0b1110011010u
#define PPUTLUTRAITS_0b0001100100u BIN, 100, (0, 0, 0, 1, 1, 0, 0, 1, 0, 0), 0b1110011011u
#define PPUTLUTRAITS_0b0001100011u BIN, 99, (0, 0, 0, 1, 1, 0, 0, 0, 1, 1), 0b1110011100u
#define PPUTLUTRAITS_0b0001100010u BIN, 98, (0, 0, 0, 1, 1, 0, 0, 0, 1, 0), 0b1110011101u
#define PPUTLUTRAITS_0b0001100001u BIN, 97, (0, 0, 0, 1, 1, 0, 0, 0, 0, 1), 0b1110011110u
#define PPUTLUTRAITS_0b0001100000u BIN, 96, (0, 0, 0, 1, 1, 0, 0, 0, 0, 0), 0b1110011111u
#define PPUTLUTRAITS_0b0001011111u BIN, 95, (0, 0, 0, 1, 0, 1, 1, 1, 1, 1), 0b1110100000u
#define PPUTLUTRAITS_0b0001011110u BIN, 94, (0, 0, 0, 1, 0, 1, 1, 1, 1, 0), 0b1110100001u
#define PPUTLUTRAITS_0b0001011101u BIN, 93, (0, 0, 0, 1, 0, 1, 1, 1, 0, 1), 0b1110100010u
#define PPUTLUTRAITS_0b0001011100u BIN, 92, (0, 0, 0, 1, 0, 1, 1, 1, 0, 0), 0b1110100011u
#define PPUTLUTRAITS_0b0001011011u BIN, 91, (0, 0, 0, 1, 0, 1, 1, 0, 1, 1), 0b1110100100u
#define PPUTLUTRAITS_0b0001011010u BIN, 90, (0, 0, 0, 1, 0, 1, 1, 0, 1, 0), 0b1110100101u
#define PPUTLUTRAITS_0b0001011001u BIN, 89, (0, 0, 0, 1, 0, 1, 1, 0, 0, 1), 0b1110100110u
#define PPUTLUTRAITS_0b0001011000u BIN, 88, (0, 0, 0, 1, 0, 1, 1, 0, 0, 0), 0b1110100111u
#define PPUTLUTRAITS_0b0001010111u BIN, 87, (0, 0, 0, 1, 0, 1, 0, 1, 1, 1), 0b1110101000u
#define PPUTLUTRAITS_0b0001010110u BIN, 86, (0, 0, 0, 1, 0, 1, 0, 1, 1, 0), 0b1110101001u
#define PPUTLUTRAITS_0b0001010101u BIN, 85, (0, 0, 0, 1, 0, 1, 0, 1, 0, 1), 0b1110101010u
#define PPUTLUTRAITS_0b0001010100u BIN, 84, (0, 0, 0, 1, 0, 1, 0, 1, 0, 0), 0b1110101011u
#define PPUTLUTRAITS_0b0001010011u BIN, 83, (0, 0, 0, 1, 0, 1, 0, 0, 1, 1), 0b1110101100u
#define PPUTLUTRAITS_0b0001010010u BIN, 82, (0, 0, 0, 1, 0, 1, 0, 0, 1, 0), 0b1110101101u
#define PPUTLUTRAITS_0b0001010001u BIN, 81, (0, 0, 0, 1, 0, 1, 0, 0, 0, 1), 0b1110101110u
#define PPUTLUTRAITS_0b0001010000u BIN, 80, (0, 0, 0, 1, 0, 1, 0, 0, 0, 0), 0b1110101111u
#define PPUTLUTRAITS_0b0001001111u BIN, 79, (0, 0, 0, 1, 0, 0, 1, 1, 1, 1), 0b1110110000u
#define PPUTLUTRAITS_0b0001001110u BIN, 78, (0, 0, 0, 1, 0, 0, 1, 1, 1, 0), 0b1110110001u
#define PPUTLUTRAITS_0b0001001101u BIN, 77, (0, 0, 0, 1, 0, 0, 1, 1, 0, 1), 0b1110110010u
#define PPUTLUTRAITS_0b0001001100u BIN, 76, (0, 0, 0, 1, 0, 0, 1, 1, 0, 0), 0b1110110011u
#define PPUTLUTRAITS_0b0001001011u BIN, 75, (0, 0, 0, 1, 0, 0, 1, 0, 1, 1), 0b1110110100u
#define PPUTLUTRAITS_0b0001001010u BIN, 74, (0, 0, 0, 1, 0, 0, 1, 0, 1, 0), 0b1110110101u
#define PPUTLUTRAITS_0b0001001001u BIN, 73, (0, 0, 0, 1, 0, 0, 1, 0, 0, 1), 0b1110110110u
#define PPUTLUTRAITS_0b0001001000u BIN, 72, (0, 0, 0, 1, 0, 0, 1, 0, 0, 0), 0b1110110111u
#define PPUTLUTRAITS_0b0001000111u BIN, 71, (0, 0, 0, 1, 0, 0, 0, 1, 1, 1), 0b1110111000u
#define PPUTLUTRAITS_0b0001000110u BIN, 70, (0, 0, 0, 1, 0, 0, 0, 1, 1, 0), 0b1110111001u
#define PPUTLUTRAITS_0b0001000101u BIN, 69, (0, 0, 0, 1, 0, 0, 0, 1, 0, 1), 0b1110111010u
#define PPUTLUTRAITS_0b0001000100u BIN, 68, (0, 0, 0, 1, 0, 0, 0, 1, 0, 0), 0b1110111011u
#define PPUTLUTRAITS_0b0001000011u BIN, 67, (0, 0, 0, 1, 0, 0, 0, 0, 1, 1), 0b1110111100u
#define PPUTLUTRAITS_0b0001000010u BIN, 66, (0, 0, 0, 1, 0, 0, 0, 0, 1, 0), 0b1110111101u
#define PPUTLUTRAITS_0b0001000001u BIN, 65, (0, 0, 0, 1, 0, 0, 0, 0, 0, 1), 0b1110111110u
#define PPUTLUTRAITS_0b0001000000u BIN, 64, (0, 0, 0, 1, 0, 0, 0, 0, 0, 0), 0b1110111111u
#define PPUTLUTRAITS_0b0000111111u BIN, 63, (0, 0, 0, 0, 1, 1, 1, 1, 1, 1), 0b1111000000u
#define PPUTLUTRAITS_0b0000111110u BIN, 62, (0, 0, 0, 0, 1, 1, 1, 1, 1, 0), 0b1111000001u
#define PPUTLUTRAITS_0b0000111101u BIN, 61, (0, 0, 0, 0, 1, 1, 1, 1, 0, 1), 0b1111000010u
#define PPUTLUTRAITS_0b0000111100u BIN, 60, (0, 0, 0, 0, 1, 1, 1, 1, 0, 0), 0b1111000011u
#define PPUTLUTRAITS_0b0000111011u BIN, 59, (0, 0, 0, 0, 1, 1, 1, 0, 1, 1), 0b1111000100u
#define PPUTLUTRAITS_0b0000111010u BIN, 58, (0, 0, 0, 0, 1, 1, 1, 0, 1, 0), 0b1111000101u
#define PPUTLUTRAITS_0b0000111001u BIN, 57, (0, 0, 0, 0, 1, 1, 1, 0, 0, 1), 0b1111000110u
#define PPUTLUTRAITS_0b0000111000u BIN, 56, (0, 0, 0, 0, 1, 1, 1, 0, 0, 0), 0b1111000111u
#define PPUTLUTRAITS_0b0000110111u BIN, 55, (0, 0, 0, 0, 1, 1, 0, 1, 1, 1), 0b1111001000u
#define PPUTLUTRAITS_0b0000110110u BIN, 54, (0, 0, 0, 0, 1, 1, 0, 1, 1, 0), 0b1111001001u
#define PPUTLUTRAITS_0b0000110101u BIN, 53, (0, 0, 0, 0, 1, 1, 0, 1, 0, 1), 0b1111001010u
#define PPUTLUTRAITS_0b0000110100u BIN, 52, (0, 0, 0, 0, 1, 1, 0, 1, 0, 0), 0b1111001011u
#define PPUTLUTRAITS_0b0000110011u BIN, 51, (0, 0, 0, 0, 1, 1, 0, 0, 1, 1), 0b1111001100u
#define PPUTLUTRAITS_0b0000110010u BIN, 50, (0, 0, 0, 0, 1, 1, 0, 0, 1, 0), 0b1111001101u
#define PPUTLUTRAITS_0b0000110001u BIN, 49, (0, 0, 0, 0, 1, 1, 0, 0, 0, 1), 0b1111001110u
#define PPUTLUTRAITS_0b0000110000u BIN, 48, (0, 0, 0, 0, 1, 1, 0, 0, 0, 0), 0b1111001111u
#define PPUTLUTRAITS_0b0000101111u BIN, 47, (0, 0, 0, 0, 1, 0, 1, 1, 1, 1), 0b1111010000u
#define PPUTLUTRAITS_0b0000101110u BIN, 46, (0, 0, 0, 0, 1, 0, 1, 1, 1, 0), 0b1111010001u
#define PPUTLUTRAITS_0b0000101101u BIN, 45, (0, 0, 0, 0, 1, 0, 1, 1, 0, 1), 0b1111010010u
#define PPUTLUTRAITS_0b0000101100u BIN, 44, (0, 0, 0, 0, 1, 0, 1, 1, 0, 0), 0b1111010011u
#define PPUTLUTRAITS_0b0000101011u BIN, 43, (0, 0, 0, 0, 1, 0, 1, 0, 1, 1), 0b1111010100u
#define PPUTLUTRAITS_0b0000101010u BIN, 42, (0, 0, 0, 0, 1, 0, 1, 0, 1, 0), 0b1111010101u
#define PPUTLUTRAITS_0b0000101001u BIN, 41, (0, 0, 0, 0, 1, 0, 1, 0, 0, 1), 0b1111010110u
#define PPUTLUTRAITS_0b0000101000u BIN, 40, (0, 0, 0, 0, 1, 0, 1, 0, 0, 0), 0b1111010111u
#define PPUTLUTRAITS_0b0000100111u BIN, 39, (0, 0, 0, 0, 1, 0, 0, 1, 1, 1), 0b1111011000u
#define PPUTLUTRAITS_0b0000100110u BIN, 38, (0, 0, 0, 0, 1, 0, 0, 1, 1, 0), 0b1111011001u
#define PPUTLUTRAITS_0b0000100101u BIN, 37, (0, 0, 0, 0, 1, 0, 0, 1, 0, 1), 0b1111011010u
#define PPUTLUTRAITS_0b0000100100u BIN, 36, (0, 0, 0, 0, 1, 0, 0, 1, 0, 0), 0b1111011011u
#define PPUTLUTRAITS_0b0000100011u BIN, 35, (0, 0, 0, 0, 1, 0, 0, 0, 1, 1), 0b1111011100u
#define PPUTLUTRAITS_0b0000100010u BIN, 34, (0, 0, 0, 0, 1, 0, 0, 0, 1, 0), 0b1111011101u
#define PPUTLUTRAITS_0b0000100001u BIN, 33, (0, 0, 0, 0, 1, 0, 0, 0, 0, 1), 0b1111011110u
#define PPUTLUTRAITS_0b0000100000u BIN, 32, (0, 0, 0, 0, 1, 0, 0, 0, 0, 0), 0b1111011111u
#define PPUTLUTRAITS_0b0000011111u BIN, 31, (0, 0, 0, 0, 0, 1, 1, 1, 1, 1), 0b1111100000u
#define PPUTLUTRAITS_0b0000011110u BIN, 30, (0, 0, 0, 0, 0, 1, 1, 1, 1, 0), 0b1111100001u
#define PPUTLUTRAITS_0b0000011101u BIN, 29, (0, 0, 0, 0, 0, 1, 1, 1, 0, 1), 0b1111100010u
#define PPUTLUTRAITS_0b0000011100u BIN, 28, (0, 0, 0, 0, 0, 1, 1, 1, 0, 0), 0b1111100011u
#define PPUTLUTRAITS_0b0000011011u BIN, 27, (0, 0, 0, 0, 0, 1, 1, 0, 1, 1), 0b1111100100u
#define PPUTLUTRAITS_0b0000011010u BIN, 26, (0, 0, 0, 0, 0, 1, 1, 0, 1, 0), 0b1111100101u
#define PPUTLUTRAITS_0b0000011001u BIN, 25, (0, 0, 0, 0, 0, 1, 1, 0, 0, 1), 0b1111100110u
#define PPUTLUTRAITS_0b0000011000u BIN, 24, (0, 0, 0, 0, 0, 1, 1, 0, 0, 0), 0b1111100111u
#define PPUTLUTRAITS_0b0000010111u BIN, 23, (0, 0, 0, 0, 0, 1, 0, 1, 1, 1), 0b1111101000u
#define PPUTLUTRAITS_0b0000010110u BIN, 22, (0, 0, 0, 0, 0, 1, 0, 1, 1, 0), 0b1111101001u
#define PPUTLUTRAITS_0b0000010101u BIN, 21, (0, 0, 0, 0, 0, 1, 0, 1, 0, 1), 0b1111101010u
#define PPUTLUTRAITS_0b0000010100u BIN, 20, (0, 0, 0, 0, 0, 1, 0, 1, 0, 0), 0b1111101011u
#define PPUTLUTRAITS_0b0000010011u BIN, 19, (0, 0, 0, 0, 0, 1, 0, 0, 1, 1), 0b1111101100u
#define PPUTLUTRAITS_0b0000010010u BIN, 18, (0, 0, 0, 0, 0, 1, 0, 0, 1, 0), 0b1111101101u
#define PPUTLUTRAITS_0b0000010001u BIN, 17, (0, 0, 0, 0, 0, 1, 0, 0, 0, 1), 0b1111101110u
#define PPUTLUTRAITS_0b0000010000u BIN, 16, (0, 0, 0, 0, 0, 1, 0, 0, 0, 0), 0b1111101111u
#define PPUTLUTRAITS_0b0000001111u BIN, 15, (0, 0, 0, 0, 0, 0, 1, 1, 1, 1), 0b1111110000u
#define PPUTLUTRAITS_0b0000001110u BIN, 14, (0, 0, 0, 0, 0, 0, 1, 1, 1, 0), 0b1111110001u
#define PPUTLUTRAITS_0b0000001101u BIN, 13, (0, 0, 0, 0, 0, 0, 1, 1, 0, 1), 0b1111110010u
#define PPUTLUTRAITS_0b0000001100u BIN, 12, (0, 0, 0, 0, 0, 0, 1, 1, 0, 0), 0b1111110011u
#define PPUTLUTRAITS_0b0000001011u BIN, 11, (0, 0, 0, 0, 0, 0, 1, 0, 1, 1), 0b1111110100u
#define PPUTLUTRAITS_0b0000001010u BIN, 10, (0, 0, 0, 0, 0, 0, 1, 0, 1, 0), 0b1111110101u
#define PPUTLUTRAITS_0b0000001001u BIN, 9, (0, 0, 0, 0, 0, 0, 1, 0, 0, 1), 0b1111110110u
#define PPUTLUTRAITS_0b0000001000u BIN, 8, (0, 0, 0, 0, 0, 0, 1, 0, 0, 0), 0b1111110111u
#define PPUTLUTRAITS_0b0000000111u BIN, 7, (0, 0, 0, 0, 0, 0, 0, 1, 1, 1), 0b1111111000u
#define PPUTLUTRAITS_0b0000000110u BIN, 6, (0, 0, 0, 0, 0, 0, 0, 1, 1, 0), 0b1111111001u
#define PPUTLUTRAITS_0b0000000101u BIN, 5, (0, 0, 0, 0, 0, 0, 0, 1, 0, 1), 0b1111111010u
#define PPUTLUTRAITS_0b0000000100u BIN, 4, (0, 0, 0, 0, 0, 0, 0, 1, 0, 0), 0b1111111011u
#define PPUTLUTRAITS_0b0000000011u BIN, 3, (0, 0, 0, 0, 0, 0, 0, 0, 1, 1), 0b1111111100u
#define PPUTLUTRAITS_0b0000000010u BIN, 2, (0, 0, 0, 0, 0, 0, 0, 0, 1, 0), 0b1111111101u
#define PPUTLUTRAITS_0b0000000001u BIN, 1, (0, 0, 0, 0, 0, 0, 0, 0, 0, 1), 0b1111111110u
#define PPUTLUTRAITS_0b0000000000u BIN, 0, (0, 0, 0, 0, 0, 0, 0, 0, 0, 0), 0b1111111111u

/// type, binary, log2, sqrt, factors
#define PPUTLUTRAITS_1023 DEC, 0b1111111111u, 9, 31, (3, 11, 31)
#define PPUTLUTRAITS_1022 DEC, 0b1111111110u, 9, 31, (2, 7, 73)
#define PPUTLUTRAITS_1021 DEC, 0b1111111101u, 9, 31, ()
#define PPUTLUTRAITS_1020 DEC, 0b1111111100u, 9, 31, (2, 2, 3, 5, 17)
#define PPUTLUTRAITS_1019 DEC, 0b1111111011u, 9, 31, ()
#define PPUTLUTRAITS_1018 DEC, 0b1111111010u, 9, 31, (2, 509)
#define PPUTLUTRAITS_1017 DEC, 0b1111111001u, 9, 31, (3, 3, 113)
#define PPUTLUTRAITS_1016 DEC, 0b1111111000u, 9, 31, (2, 2, 2, 127)
#define PPUTLUTRAITS_1015 DEC, 0b1111110111u, 9, 31, (5, 7, 29)
#define PPUTLUTRAITS_1014 DEC, 0b1111110110u, 9, 31, (2, 3, 13, 13)
#define PPUTLUTRAITS_1013 DEC, 0b1111110101u, 9, 31, ()
#define PPUTLUTRAITS_1012 DEC, 0b1111110100u, 9, 31, (2, 2, 11, 23)
#define PPUTLUTRAITS_1011 DEC, 0b1111110011u, 9, 31, (3, 337)
#define PPUTLUTRAITS_1010 DEC, 0b1111110010u, 9, 31, (2, 5, 101)
#define PPUTLUTRAITS_1009 DEC, 0b1111110001u, 9, 31, ()
#define PPUTLUTRAITS_1008 DEC, 0b1111110000u, 9, 31, (2, 2, 2, 2, 3, 3, 7)
#define PPUTLUTRAITS_1007 DEC, 0b1111101111u, 9, 31, (19, 53)
#define PPUTLUTRAITS_1006 DEC, 0b1111101110u, 9, 31, (2, 503)
#define PPUTLUTRAITS_1005 DEC, 0b1111101101u, 9, 31, (3, 5, 67)
#define PPUTLUTRAITS_1004 DEC, 0b1111101100u, 9, 31, (2, 2, 251)
#define PPUTLUTRAITS_1003 DEC, 0b1111101011u, 9, 31, (17, 59)
#define PPUTLUTRAITS_1002 DEC, 0b1111101010u, 9, 31, (2, 3, 167)
#define PPUTLUTRAITS_1001 DEC, 0b1111101001u, 9, 31, (7, 11, 13)
#define PPUTLUTRAITS_1000 DEC, 0b1111101000u, 9, 31, (2, 2, 2, 5, 5, 5)
#define PPUTLUTRAITS_999  DEC, 0b1111100111u, 9, 31, (3, 3, 3, 37)
#define PPUTLUTRAITS_998  DEC, 0b1111100110u, 9, 31, (2, 499)
#define PPUTLUTRAITS_997  DEC, 0b1111100101u, 9, 31, ()
#define PPUTLUTRAITS_996  DEC, 0b1111100100u, 9, 31, (2, 2, 3, 83)
#define PPUTLUTRAITS_995  DEC, 0b1111100011u, 9, 31, (5, 199)
#define PPUTLUTRAITS_994  DEC, 0b1111100010u, 9, 31, (2, 7, 71)
#define PPUTLUTRAITS_993  DEC, 0b1111100001u, 9, 31, (3, 331)
#define PPUTLUTRAITS_992  DEC, 0b1111100000u, 9, 31, (2, 2, 2, 2, 2, 31)
#define PPUTLUTRAITS_991  DEC, 0b1111011111u, 9, 31, ()
#define PPUTLUTRAITS_990  DEC, 0b1111011110u, 9, 31, (2, 3, 3, 5, 11)
#define PPUTLUTRAITS_989  DEC, 0b1111011101u, 9, 31, (23, 43)
#define PPUTLUTRAITS_988  DEC, 0b1111011100u, 9, 31, (2, 2, 13, 19)
#define PPUTLUTRAITS_987  DEC, 0b1111011011u, 9, 31, (3, 7, 47)
#define PPUTLUTRAITS_986  DEC, 0b1111011010u, 9, 31, (2, 17, 29)
#define PPUTLUTRAITS_985  DEC, 0b1111011001u, 9, 31, (5, 197)
#define PPUTLUTRAITS_984  DEC, 0b1111011000u, 9, 31, (2, 2, 2, 3, 41)
#define PPUTLUTRAITS_983  DEC, 0b1111010111u, 9, 31, ()
#define PPUTLUTRAITS_982  DEC, 0b1111010110u, 9, 31, (2, 491)
#define PPUTLUTRAITS_981  DEC, 0b1111010101u, 9, 31, (3, 3, 109)
#define PPUTLUTRAITS_980  DEC, 0b1111010100u, 9, 31, (2, 2, 5, 7, 7)
#define PPUTLUTRAITS_979  DEC, 0b1111010011u, 9, 31, (11, 89)
#define PPUTLUTRAITS_978  DEC, 0b1111010010u, 9, 31, (2, 3, 163)
#define PPUTLUTRAITS_977  DEC, 0b1111010001u, 9, 31, ()
#define PPUTLUTRAITS_976  DEC, 0b1111010000u, 9, 31, (2, 2, 2, 2, 61)
#define PPUTLUTRAITS_975  DEC, 0b1111001111u, 9, 31, (3, 5, 5, 13)
#define PPUTLUTRAITS_974  DEC, 0b1111001110u, 9, 31, (2, 487)
#define PPUTLUTRAITS_973  DEC, 0b1111001101u, 9, 31, (7, 139)
#define PPUTLUTRAITS_972  DEC, 0b1111001100u, 9, 31, (2, 2, 3, 3, 3, 3, 3)
#define PPUTLUTRAITS_971  DEC, 0b1111001011u, 9, 31, ()
#define PPUTLUTRAITS_970  DEC, 0b1111001010u, 9, 31, (2, 5, 97)
#define PPUTLUTRAITS_969  DEC, 0b1111001001u, 9, 31, (3, 17, 19)
#define PPUTLUTRAITS_968  DEC, 0b1111001000u, 9, 31, (2, 2, 2, 11, 11)
#define PPUTLUTRAITS_967  DEC, 0b1111000111u, 9, 31, ()
#define PPUTLUTRAITS_966  DEC, 0b1111000110u, 9, 31, (2, 3, 7, 23)
#define PPUTLUTRAITS_965  DEC, 0b1111000101u, 9, 31, (5, 193)
#define PPUTLUTRAITS_964  DEC, 0b1111000100u, 9, 31, (2, 2, 241)
#define PPUTLUTRAITS_963  DEC, 0b1111000011u, 9, 31, (3, 3, 107)
#define PPUTLUTRAITS_962  DEC, 0b1111000010u, 9, 31, (2, 13, 37)
#define PPUTLUTRAITS_961  DEC, 0b1111000001u, 9, 31, (31, 31)
#define PPUTLUTRAITS_960  DEC, 0b1111000000u, 9, 30, (2, 2, 2, 2, 2, 2, 3, 5)
#define PPUTLUTRAITS_959  DEC, 0b1110111111u, 9, 30, (7, 137)
#define PPUTLUTRAITS_958  DEC, 0b1110111110u, 9, 30, (2, 479)
#define PPUTLUTRAITS_957  DEC, 0b1110111101u, 9, 30, (3, 11, 29)
#define PPUTLUTRAITS_956  DEC, 0b1110111100u, 9, 30, (2, 2, 239)
#define PPUTLUTRAITS_955  DEC, 0b1110111011u, 9, 30, (5, 191)
#define PPUTLUTRAITS_954  DEC, 0b1110111010u, 9, 30, (2, 3, 3, 53)
#define PPUTLUTRAITS_953  DEC, 0b1110111001u, 9, 30, ()
#define PPUTLUTRAITS_952  DEC, 0b1110111000u, 9, 30, (2, 2, 2, 7, 17)
#define PPUTLUTRAITS_951  DEC, 0b1110110111u, 9, 30, (3, 317)
#define PPUTLUTRAITS_950  DEC, 0b1110110110u, 9, 30, (2, 5, 5, 19)
#define PPUTLUTRAITS_949  DEC, 0b1110110101u, 9, 30, (13, 73)
#define PPUTLUTRAITS_948  DEC, 0b1110110100u, 9, 30, (2, 2, 3, 79)
#define PPUTLUTRAITS_947  DEC, 0b1110110011u, 9, 30, ()
#define PPUTLUTRAITS_946  DEC, 0b1110110010u, 9, 30, (2, 11, 43)
#define PPUTLUTRAITS_945  DEC, 0b1110110001u, 9, 30, (3, 3, 3, 5, 7)
#define PPUTLUTRAITS_944  DEC, 0b1110110000u, 9, 30, (2, 2, 2, 2, 59)
#define PPUTLUTRAITS_943  DEC, 0b1110101111u, 9, 30, (23, 41)
#define PPUTLUTRAITS_942  DEC, 0b1110101110u, 9, 30, (2, 3, 157)
#define PPUTLUTRAITS_941  DEC, 0b1110101101u, 9, 30, ()
#define PPUTLUTRAITS_940  DEC, 0b1110101100u, 9, 30, (2, 2, 5, 47)
#define PPUTLUTRAITS_939  DEC, 0b1110101011u, 9, 30, (3, 313)
#define PPUTLUTRAITS_938  DEC, 0b1110101010u, 9, 30, (2, 7, 67)
#define PPUTLUTRAITS_937  DEC, 0b1110101001u, 9, 30, ()
#define PPUTLUTRAITS_936  DEC, 0b1110101000u, 9, 30, (2, 2, 2, 3, 3, 13)
#define PPUTLUTRAITS_935  DEC, 0b1110100111u, 9, 30, (5, 11, 17)
#define PPUTLUTRAITS_934  DEC, 0b1110100110u, 9, 30, (2, 467)
#define PPUTLUTRAITS_933  DEC, 0b1110100101u, 9, 30, (3, 311)
#define PPUTLUTRAITS_932  DEC, 0b1110100100u, 9, 30, (2, 2, 233)
#define PPUTLUTRAITS_931  DEC, 0b1110100011u, 9, 30, (7, 7, 19)
#define PPUTLUTRAITS_930  DEC, 0b1110100010u, 9, 30, (2, 3, 5, 31)
#define PPUTLUTRAITS_929  DEC, 0b1110100001u, 9, 30, ()
#define PPUTLUTRAITS_928  DEC, 0b1110100000u, 9, 30, (2, 2, 2, 2, 2, 29)
#define PPUTLUTRAITS_927  DEC, 0b1110011111u, 9, 30, (3, 3, 103)
#define PPUTLUTRAITS_926  DEC, 0b1110011110u, 9, 30, (2, 463)
#define PPUTLUTRAITS_925  DEC, 0b1110011101u, 9, 30, (5, 5, 37)
#define PPUTLUTRAITS_924  DEC, 0b1110011100u, 9, 30, (2, 2, 3, 7, 11)
#define PPUTLUTRAITS_923  DEC, 0b1110011011u, 9, 30, (13, 71)
#define PPUTLUTRAITS_922  DEC, 0b1110011010u, 9, 30, (2, 461)
#define PPUTLUTRAITS_921  DEC, 0b1110011001u, 9, 30, (3, 307)
#define PPUTLUTRAITS_920  DEC, 0b1110011000u, 9, 30, (2, 2, 2, 5, 23)
#define PPUTLUTRAITS_919  DEC, 0b1110010111u, 9, 30, ()
#define PPUTLUTRAITS_918  DEC, 0b1110010110u, 9, 30, (2, 3, 3, 3, 17)
#define PPUTLUTRAITS_917  DEC, 0b1110010101u, 9, 30, (7, 131)
#define PPUTLUTRAITS_916  DEC, 0b1110010100u, 9, 30, (2, 2, 229)
#define PPUTLUTRAITS_915  DEC, 0b1110010011u, 9, 30, (3, 5, 61)
#define PPUTLUTRAITS_914  DEC, 0b1110010010u, 9, 30, (2, 457)
#define PPUTLUTRAITS_913  DEC, 0b1110010001u, 9, 30, (11, 83)
#define PPUTLUTRAITS_912  DEC, 0b1110010000u, 9, 30, (2, 2, 2, 2, 3, 19)
#define PPUTLUTRAITS_911  DEC, 0b1110001111u, 9, 30, ()
#define PPUTLUTRAITS_910  DEC, 0b1110001110u, 9, 30, (2, 5, 7, 13)
#define PPUTLUTRAITS_909  DEC, 0b1110001101u, 9, 30, (3, 3, 101)
#define PPUTLUTRAITS_908  DEC, 0b1110001100u, 9, 30, (2, 2, 227)
#define PPUTLUTRAITS_907  DEC, 0b1110001011u, 9, 30, ()
#define PPUTLUTRAITS_906  DEC, 0b1110001010u, 9, 30, (2, 3, 151)
#define PPUTLUTRAITS_905  DEC, 0b1110001001u, 9, 30, (5, 181)
#define PPUTLUTRAITS_904  DEC, 0b1110001000u, 9, 30, (2, 2, 2, 113)
#define PPUTLUTRAITS_903  DEC, 0b1110000111u, 9, 30, (3, 7, 43)
#define PPUTLUTRAITS_902  DEC, 0b1110000110u, 9, 30, (2, 11, 41)
#define PPUTLUTRAITS_901  DEC, 0b1110000101u, 9, 30, (17, 53)
#define PPUTLUTRAITS_900  DEC, 0b1110000100u, 9, 30, (2, 2, 3, 3, 5, 5)
#define PPUTLUTRAITS_899  DEC, 0b1110000011u, 9, 29, (29, 31)
#define PPUTLUTRAITS_898  DEC, 0b1110000010u, 9, 29, (2, 449)
#define PPUTLUTRAITS_897  DEC, 0b1110000001u, 9, 29, (3, 13, 23)
#define PPUTLUTRAITS_896  DEC, 0b1110000000u, 9, 29, (2, 2, 2, 2, 2, 2, 2, 7)
#define PPUTLUTRAITS_895  DEC, 0b1101111111u, 9, 29, (5, 179)
#define PPUTLUTRAITS_894  DEC, 0b1101111110u, 9, 29, (2, 3, 149)
#define PPUTLUTRAITS_893  DEC, 0b1101111101u, 9, 29, (19, 47)
#define PPUTLUTRAITS_892  DEC, 0b1101111100u, 9, 29, (2, 2, 223)
#define PPUTLUTRAITS_891  DEC, 0b1101111011u, 9, 29, (3, 3, 3, 3, 11)
#define PPUTLUTRAITS_890  DEC, 0b1101111010u, 9, 29, (2, 5, 89)
#define PPUTLUTRAITS_889  DEC, 0b1101111001u, 9, 29, (7, 127)
#define PPUTLUTRAITS_888  DEC, 0b1101111000u, 9, 29, (2, 2, 2, 3, 37)
#define PPUTLUTRAITS_887  DEC, 0b1101110111u, 9, 29, ()
#define PPUTLUTRAITS_886  DEC, 0b1101110110u, 9, 29, (2, 443)
#define PPUTLUTRAITS_885  DEC, 0b1101110101u, 9, 29, (3, 5, 59)
#define PPUTLUTRAITS_884  DEC, 0b1101110100u, 9, 29, (2, 2, 13, 17)
#define PPUTLUTRAITS_883  DEC, 0b1101110011u, 9, 29, ()
#define PPUTLUTRAITS_882  DEC, 0b1101110010u, 9, 29, (2, 3, 3, 7, 7)
#define PPUTLUTRAITS_881  DEC, 0b1101110001u, 9, 29, ()
#define PPUTLUTRAITS_880  DEC, 0b1101110000u, 9, 29, (2, 2, 2, 2, 5, 11)
#define PPUTLUTRAITS_879  DEC, 0b1101101111u, 9, 29, (3, 293)
#define PPUTLUTRAITS_878  DEC, 0b1101101110u, 9, 29, (2, 439)
#define PPUTLUTRAITS_877  DEC, 0b1101101101u, 9, 29, ()
#define PPUTLUTRAITS_876  DEC, 0b1101101100u, 9, 29, (2, 2, 3, 73)
#define PPUTLUTRAITS_875  DEC, 0b1101101011u, 9, 29, (5, 5, 5, 7)
#define PPUTLUTRAITS_874  DEC, 0b1101101010u, 9, 29, (2, 19, 23)
#define PPUTLUTRAITS_873  DEC, 0b1101101001u, 9, 29, (3, 3, 97)
#define PPUTLUTRAITS_872  DEC, 0b1101101000u, 9, 29, (2, 2, 2, 109)
#define PPUTLUTRAITS_871  DEC, 0b1101100111u, 9, 29, (13, 67)
#define PPUTLUTRAITS_870  DEC, 0b1101100110u, 9, 29, (2, 3, 5, 29)
#define PPUTLUTRAITS_869  DEC, 0b1101100101u, 9, 29, (11, 79)
#define PPUTLUTRAITS_868  DEC, 0b1101100100u, 9, 29, (2, 2, 7, 31)
#define PPUTLUTRAITS_867  DEC, 0b1101100011u, 9, 29, (3, 17, 17)
#define PPUTLUTRAITS_866  DEC, 0b1101100010u, 9, 29, (2, 433)
#define PPUTLUTRAITS_865  DEC, 0b1101100001u, 9, 29, (5, 173)
#define PPUTLUTRAITS_864  DEC, 0b1101100000u, 9, 29, (2, 2, 2, 2, 2, 3, 3, 3)
#define PPUTLUTRAITS_863  DEC, 0b1101011111u, 9, 29, ()
#define PPUTLUTRAITS_862  DEC, 0b1101011110u, 9, 29, (2, 431)
#define PPUTLUTRAITS_861  DEC, 0b1101011101u, 9, 29, (3, 7, 41)
#define PPUTLUTRAITS_860  DEC, 0b1101011100u, 9, 29, (2, 2, 5, 43)
#define PPUTLUTRAITS_859  DEC, 0b1101011011u, 9, 29, ()
#define PPUTLUTRAITS_858  DEC, 0b1101011010u, 9, 29, (2, 3, 11, 13)
#define PPUTLUTRAITS_857  DEC, 0b1101011001u, 9, 29, ()
#define PPUTLUTRAITS_856  DEC, 0b1101011000u, 9, 29, (2, 2, 2, 107)
#define PPUTLUTRAITS_855  DEC, 0b1101010111u, 9, 29, (3, 3, 5, 19)
#define PPUTLUTRAITS_854  DEC, 0b1101010110u, 9, 29, (2, 7, 61)
#define PPUTLUTRAITS_853  DEC, 0b1101010101u, 9, 29, ()
#define PPUTLUTRAITS_852  DEC, 0b1101010100u, 9, 29, (2, 2, 3, 71)
#define PPUTLUTRAITS_851  DEC, 0b1101010011u, 9, 29, (23, 37)
#define PPUTLUTRAITS_850  DEC, 0b1101010010u, 9, 29, (2, 5, 5, 17)
#define PPUTLUTRAITS_849  DEC, 0b1101010001u, 9, 29, (3, 283)
#define PPUTLUTRAITS_848  DEC, 0b1101010000u, 9, 29, (2, 2, 2, 2, 53)
#define PPUTLUTRAITS_847  DEC, 0b1101001111u, 9, 29, (7, 11, 11)
#define PPUTLUTRAITS_846  DEC, 0b1101001110u, 9, 29, (2, 3, 3, 47)
#define PPUTLUTRAITS_845  DEC, 0b1101001101u, 9, 29, (5, 13, 13)
#define PPUTLUTRAITS_844  DEC, 0b1101001100u, 9, 29, (2, 2, 211)
#define PPUTLUTRAITS_843  DEC, 0b1101001011u, 9, 29, (3, 281)
#define PPUTLUTRAITS_842  DEC, 0b1101001010u, 9, 29, (2, 421)
#define PPUTLUTRAITS_841  DEC, 0b1101001001u, 9, 29, (29, 29)
#define PPUTLUTRAITS_840  DEC, 0b1101001000u, 9, 28, (2, 2, 2, 3, 5, 7)
#define PPUTLUTRAITS_839  DEC, 0b1101000111u, 9, 28, ()
#define PPUTLUTRAITS_838  DEC, 0b1101000110u, 9, 28, (2, 419)
#define PPUTLUTRAITS_837  DEC, 0b1101000101u, 9, 28, (3, 3, 3, 31)
#define PPUTLUTRAITS_836  DEC, 0b1101000100u, 9, 28, (2, 2, 11, 19)
#define PPUTLUTRAITS_835  DEC, 0b1101000011u, 9, 28, (5, 167)
#define PPUTLUTRAITS_834  DEC, 0b1101000010u, 9, 28, (2, 3, 139)
#define PPUTLUTRAITS_833  DEC, 0b1101000001u, 9, 28, (7, 7, 17)
#define PPUTLUTRAITS_832  DEC, 0b1101000000u, 9, 28, (2, 2, 2, 2, 2, 2, 13)
#define PPUTLUTRAITS_831  DEC, 0b1100111111u, 9, 28, (3, 277)
#define PPUTLUTRAITS_830  DEC, 0b1100111110u, 9, 28, (2, 5, 83)
#define PPUTLUTRAITS_829  DEC, 0b1100111101u, 9, 28, ()
#define PPUTLUTRAITS_828  DEC, 0b1100111100u, 9, 28, (2, 2, 3, 3, 23)
#define PPUTLUTRAITS_827  DEC, 0b1100111011u, 9, 28, ()
#define PPUTLUTRAITS_826  DEC, 0b1100111010u, 9, 28, (2, 7, 59)
#define PPUTLUTRAITS_825  DEC, 0b1100111001u, 9, 28, (3, 5, 5, 11)
#define PPUTLUTRAITS_824  DEC, 0b1100111000u, 9, 28, (2, 2, 2, 103)
#define PPUTLUTRAITS_823  DEC, 0b1100110111u, 9, 28, ()
#define PPUTLUTRAITS_822  DEC, 0b1100110110u, 9, 28, (2, 3, 137)
#define PPUTLUTRAITS_821  DEC, 0b1100110101u, 9, 28, ()
#define PPUTLUTRAITS_820  DEC, 0b1100110100u, 9, 28, (2, 2, 5, 41)
#define PPUTLUTRAITS_819  DEC, 0b1100110011u, 9, 28, (3, 3, 7, 13)
#define PPUTLUTRAITS_818  DEC, 0b1100110010u, 9, 28, (2, 409)
#define PPUTLUTRAITS_817  DEC, 0b1100110001u, 9, 28, (19, 43)
#define PPUTLUTRAITS_816  DEC, 0b1100110000u, 9, 28, (2, 2, 2, 2, 3, 17)
#define PPUTLUTRAITS_815  DEC, 0b1100101111u, 9, 28, (5, 163)
#define PPUTLUTRAITS_814  DEC, 0b1100101110u, 9, 28, (2, 11, 37)
#define PPUTLUTRAITS_813  DEC, 0b1100101101u, 9, 28, (3, 271)
#define PPUTLUTRAITS_812  DEC, 0b1100101100u, 9, 28, (2, 2, 7, 29)
#define PPUTLUTRAITS_811  DEC, 0b1100101011u, 9, 28, ()
#define PPUTLUTRAITS_810  DEC, 0b1100101010u, 9, 28, (2, 3, 3, 3, 3, 5)
#define PPUTLUTRAITS_809  DEC, 0b1100101001u, 9, 28, ()
#define PPUTLUTRAITS_808  DEC, 0b1100101000u, 9, 28, (2, 2, 2, 101)
#define PPUTLUTRAITS_807  DEC, 0b1100100111u, 9, 28, (3, 269)
#define PPUTLUTRAITS_806  DEC, 0b1100100110u, 9, 28, (2, 13, 31)
#define PPUTLUTRAITS_805  DEC, 0b1100100101u, 9, 28, (5, 7, 23)
#define PPUTLUTRAITS_804  DEC, 0b1100100100u, 9, 28, (2, 2, 3, 67)
#define PPUTLUTRAITS_803  DEC, 0b1100100011u, 9, 28, (11, 73)
#define PPUTLUTRAITS_802  DEC, 0b1100100010u, 9, 28, (2, 401)
#define PPUTLUTRAITS_801  DEC, 0b1100100001u, 9, 28, (3, 3, 89)
#define PPUTLUTRAITS_800  DEC, 0b1100100000u, 9, 28, (2, 2, 2, 2, 2, 5, 5)
#define PPUTLUTRAITS_799  DEC, 0b1100011111u, 9, 28, (17, 47)
#define PPUTLUTRAITS_798  DEC, 0b1100011110u, 9, 28, (2, 3, 7, 19)
#define PPUTLUTRAITS_797  DEC, 0b1100011101u, 9, 28, ()
#define PPUTLUTRAITS_796  DEC, 0b1100011100u, 9, 28, (2, 2, 199)
#define PPUTLUTRAITS_795  DEC, 0b1100011011u, 9, 28, (3, 5, 53)
#define PPUTLUTRAITS_794  DEC, 0b1100011010u, 9, 28, (2, 397)
#define PPUTLUTRAITS_793  DEC, 0b1100011001u, 9, 28, (13, 61)
#define PPUTLUTRAITS_792  DEC, 0b1100011000u, 9, 28, (2, 2, 2, 3, 3, 11)
#define PPUTLUTRAITS_791  DEC, 0b1100010111u, 9, 28, (7, 113)
#define PPUTLUTRAITS_790  DEC, 0b1100010110u, 9, 28, (2, 5, 79)
#define PPUTLUTRAITS_789  DEC, 0b1100010101u, 9, 28, (3, 263)
#define PPUTLUTRAITS_788  DEC, 0b1100010100u, 9, 28, (2, 2, 197)
#define PPUTLUTRAITS_787  DEC, 0b1100010011u, 9, 28, ()
#define PPUTLUTRAITS_786  DEC, 0b1100010010u, 9, 28, (2, 3, 131)
#define PPUTLUTRAITS_785  DEC, 0b1100010001u, 9, 28, (5, 157)
#define PPUTLUTRAITS_784  DEC, 0b1100010000u, 9, 28, (2, 2, 2, 2, 7, 7)
#define PPUTLUTRAITS_783  DEC, 0b1100001111u, 9, 27, (3, 3, 3, 29)
#define PPUTLUTRAITS_782  DEC, 0b1100001110u, 9, 27, (2, 17, 23)
#define PPUTLUTRAITS_781  DEC, 0b1100001101u, 9, 27, (11, 71)
#define PPUTLUTRAITS_780  DEC, 0b1100001100u, 9, 27, (2, 2, 3, 5, 13)
#define PPUTLUTRAITS_779  DEC, 0b1100001011u, 9, 27, (19, 41)
#define PPUTLUTRAITS_778  DEC, 0b1100001010u, 9, 27, (2, 389)
#define PPUTLUTRAITS_777  DEC, 0b1100001001u, 9, 27, (3, 7, 37)
#define PPUTLUTRAITS_776  DEC, 0b1100001000u, 9, 27, (2, 2, 2, 97)
#define PPUTLUTRAITS_775  DEC, 0b1100000111u, 9, 27, (5, 5, 31)
#define PPUTLUTRAITS_774  DEC, 0b1100000110u, 9, 27, (2, 3, 3, 43)
#define PPUTLUTRAITS_773  DEC, 0b1100000101u, 9, 27, ()
#define PPUTLUTRAITS_772  DEC, 0b1100000100u, 9, 27, (2, 2, 193)
#define PPUTLUTRAITS_771  DEC, 0b1100000011u, 9, 27, (3, 257)
#define PPUTLUTRAITS_770  DEC, 0b1100000010u, 9, 27, (2, 5, 7, 11)
#define PPUTLUTRAITS_769  DEC, 0b1100000001u, 9, 27, ()
#define PPUTLUTRAITS_768  DEC, 0b1100000000u, 9, 27, (2, 2, 2, 2, 2, 2, 2, 2, 3)
#define PPUTLUTRAITS_767  DEC, 0b1011111111u, 9, 27, (13, 59)
#define PPUTLUTRAITS_766  DEC, 0b1011111110u, 9, 27, (2, 383)
#define PPUTLUTRAITS_765  DEC, 0b1011111101u, 9, 27, (3, 3, 5, 17)
#define PPUTLUTRAITS_764  DEC, 0b1011111100u, 9, 27, (2, 2, 191)
#define PPUTLUTRAITS_763  DEC, 0b1011111011u, 9, 27, (7, 109)
#define PPUTLUTRAITS_762  DEC, 0b1011111010u, 9, 27, (2, 3, 127)
#define PPUTLUTRAITS_761  DEC, 0b1011111001u, 9, 27, ()
#define PPUTLUTRAITS_760  DEC, 0b1011111000u, 9, 27, (2, 2, 2, 5, 19)
#define PPUTLUTRAITS_759  DEC, 0b1011110111u, 9, 27, (3, 11, 23)
#define PPUTLUTRAITS_758  DEC, 0b1011110110u, 9, 27, (2, 379)
#define PPUTLUTRAITS_757  DEC, 0b1011110101u, 9, 27, ()
#define PPUTLUTRAITS_756  DEC, 0b1011110100u, 9, 27, (2, 2, 3, 3, 3, 7)
#define PPUTLUTRAITS_755  DEC, 0b1011110011u, 9, 27, (5, 151)
#define PPUTLUTRAITS_754  DEC, 0b1011110010u, 9, 27, (2, 13, 29)
#define PPUTLUTRAITS_753  DEC, 0b1011110001u, 9, 27, (3, 251)
#define PPUTLUTRAITS_752  DEC, 0b1011110000u, 9, 27, (2, 2, 2, 2, 47)
#define PPUTLUTRAITS_751  DEC, 0b1011101111u, 9, 27, ()
#define PPUTLUTRAITS_750  DEC, 0b1011101110u, 9, 27, (2, 3, 5, 5, 5)
#define PPUTLUTRAITS_749  DEC, 0b1011101101u, 9, 27, (7, 107)
#define PPUTLUTRAITS_748  DEC, 0b1011101100u, 9, 27, (2, 2, 11, 17)
#define PPUTLUTRAITS_747  DEC, 0b1011101011u, 9, 27, (3, 3, 83)
#define PPUTLUTRAITS_746  DEC, 0b1011101010u, 9, 27, (2, 373)
#define PPUTLUTRAITS_745  DEC, 0b1011101001u, 9, 27, (5, 149)
#define PPUTLUTRAITS_744  DEC, 0b1011101000u, 9, 27, (2, 2, 2, 3, 31)
#define PPUTLUTRAITS_743  DEC, 0b1011100111u, 9, 27, ()
#define PPUTLUTRAITS_742  DEC, 0b1011100110u, 9, 27, (2, 7, 53)
#define PPUTLUTRAITS_741  DEC, 0b1011100101u, 9, 27, (3, 13, 19)
#define PPUTLUTRAITS_740  DEC, 0b1011100100u, 9, 27, (2, 2, 5, 37)
#define PPUTLUTRAITS_739  DEC, 0b1011100011u, 9, 27, ()
#define PPUTLUTRAITS_738  DEC, 0b1011100010u, 9, 27, (2, 3, 3, 41)
#define PPUTLUTRAITS_737  DEC, 0b1011100001u, 9, 27, (11, 67)
#define PPUTLUTRAITS_736  DEC, 0b1011100000u, 9, 27, (2, 2, 2, 2, 2, 23)
#define PPUTLUTRAITS_735  DEC, 0b1011011111u, 9, 27, (3, 5, 7, 7)
#define PPUTLUTRAITS_734  DEC, 0b1011011110u, 9, 27, (2, 367)
#define PPUTLUTRAITS_733  DEC, 0b1011011101u, 9, 27, ()
#define PPUTLUTRAITS_732  DEC, 0b1011011100u, 9, 27, (2, 2, 3, 61)
#define PPUTLUTRAITS_731  DEC, 0b1011011011u, 9, 27, (17, 43)
#define PPUTLUTRAITS_730  DEC, 0b1011011010u, 9, 27, (2, 5, 73)
#define PPUTLUTRAITS_729  DEC, 0b1011011001u, 9, 27, (3, 3, 3, 3, 3, 3)
#define PPUTLUTRAITS_728  DEC, 0b1011011000u, 9, 26, (2, 2, 2, 7, 13)
#define PPUTLUTRAITS_727  DEC, 0b1011010111u, 9, 26, ()
#define PPUTLUTRAITS_726  DEC, 0b1011010110u, 9, 26, (2, 3, 11, 11)
#define PPUTLUTRAITS_725  DEC, 0b1011010101u, 9, 26, (5, 5, 29)
#define PPUTLUTRAITS_724  DEC, 0b1011010100u, 9, 26, (2, 2, 181)
#define PPUTLUTRAITS_723  DEC, 0b1011010011u, 9, 26, (3, 241)
#define PPUTLUTRAITS_722  DEC, 0b1011010010u, 9, 26, (2, 19, 19)
#define PPUTLUTRAITS_721  DEC, 0b1011010001u, 9, 26, (7, 103)
#define PPUTLUTRAITS_720  DEC, 0b1011010000u, 9, 26, (2, 2, 2, 2, 3, 3, 5)
#define PPUTLUTRAITS_719  DEC, 0b1011001111u, 9, 26, ()
#define PPUTLUTRAITS_718  DEC, 0b1011001110u, 9, 26, (2, 359)
#define PPUTLUTRAITS_717  DEC, 0b1011001101u, 9, 26, (3, 239)
#define PPUTLUTRAITS_716  DEC, 0b1011001100u, 9, 26, (2, 2, 179)
#define PPUTLUTRAITS_715  DEC, 0b1011001011u, 9, 26, (5, 11, 13)
#define PPUTLUTRAITS_714  DEC, 0b1011001010u, 9, 26, (2, 3, 7, 17)
#define PPUTLUTRAITS_713  DEC, 0b1011001001u, 9, 26, (23, 31)
#define PPUTLUTRAITS_712  DEC, 0b1011001000u, 9, 26, (2, 2, 2, 89)
#define PPUTLUTRAITS_711  DEC, 0b1011000111u, 9, 26, (3, 3, 79)
#define PPUTLUTRAITS_710  DEC, 0b1011000110u, 9, 26, (2, 5, 71)
#define PPUTLUTRAITS_709  DEC, 0b1011000101u, 9, 26, ()
#define PPUTLUTRAITS_708  DEC, 0b1011000100u, 9, 26, (2, 2, 3, 59)
#define PPUTLUTRAITS_707  DEC, 0b1011000011u, 9, 26, (7, 101)
#define PPUTLUTRAITS_706  DEC, 0b1011000010u, 9, 26, (2, 353)
#define PPUTLUTRAITS_705  DEC, 0b1011000001u, 9, 26, (3, 5, 47)
#define PPUTLUTRAITS_704  DEC, 0b1011000000u, 9, 26, (2, 2, 2, 2, 2, 2, 11)
#define PPUTLUTRAITS_703  DEC, 0b1010111111u, 9, 26, (19, 37)
#define PPUTLUTRAITS_702  DEC, 0b1010111110u, 9, 26, (2, 3, 3, 3, 13)
#define PPUTLUTRAITS_701  DEC, 0b1010111101u, 9, 26, ()
#define PPUTLUTRAITS_700  DEC, 0b1010111100u, 9, 26, (2, 2, 5, 5, 7)
#define PPUTLUTRAITS_699  DEC, 0b1010111011u, 9, 26, (3, 233)
#define PPUTLUTRAITS_698  DEC, 0b1010111010u, 9, 26, (2, 349)
#define PPUTLUTRAITS_697  DEC, 0b1010111001u, 9, 26, (17, 41)
#define PPUTLUTRAITS_696  DEC, 0b1010111000u, 9, 26, (2, 2, 2, 3, 29)
#define PPUTLUTRAITS_695  DEC, 0b1010110111u, 9, 26, (5, 139)
#define PPUTLUTRAITS_694  DEC, 0b1010110110u, 9, 26, (2, 347)
#define PPUTLUTRAITS_693  DEC, 0b1010110101u, 9, 26, (3, 3, 7, 11)
#define PPUTLUTRAITS_692  DEC, 0b1010110100u, 9, 26, (2, 2, 173)
#define PPUTLUTRAITS_691  DEC, 0b1010110011u, 9, 26, ()
#define PPUTLUTRAITS_690  DEC, 0b1010110010u, 9, 26, (2, 3, 5, 23)
#define PPUTLUTRAITS_689  DEC, 0b1010110001u, 9, 26, (13, 53)
#define PPUTLUTRAITS_688  DEC, 0b1010110000u, 9, 26, (2, 2, 2, 2, 43)
#define PPUTLUTRAITS_687  DEC, 0b1010101111u, 9, 26, (3, 229)
#define PPUTLUTRAITS_686  DEC, 0b1010101110u, 9, 26, (2, 7, 7, 7)
#define PPUTLUTRAITS_685  DEC, 0b1010101101u, 9, 26, (5, 137)
#define PPUTLUTRAITS_684  DEC, 0b1010101100u, 9, 26, (2, 2, 3, 3, 19)
#define PPUTLUTRAITS_683  DEC, 0b1010101011u, 9, 26, ()
#define PPUTLUTRAITS_682  DEC, 0b1010101010u, 9, 26, (2, 11, 31)
#define PPUTLUTRAITS_681  DEC, 0b1010101001u, 9, 26, (3, 227)
#define PPUTLUTRAITS_680  DEC, 0b1010101000u, 9, 26, (2, 2, 2, 5, 17)
#define PPUTLUTRAITS_679  DEC, 0b1010100111u, 9, 26, (7, 97)
#define PPUTLUTRAITS_678  DEC, 0b1010100110u, 9, 26, (2, 3, 113)
#define PPUTLUTRAITS_677  DEC, 0b1010100101u, 9, 26, ()
#define PPUTLUTRAITS_676  DEC, 0b1010100100u, 9, 26, (2, 2, 13, 13)
#define PPUTLUTRAITS_675  DEC, 0b1010100011u, 9, 25, (3, 3, 3, 5, 5)
#define PPUTLUTRAITS_674  DEC, 0b1010100010u, 9, 25, (2, 337)
#define PPUTLUTRAITS_673  DEC, 0b1010100001u, 9, 25, ()
#define PPUTLUTRAITS_672  DEC, 0b1010100000u, 9, 25, (2, 2, 2, 2, 2, 3, 7)
#define PPUTLUTRAITS_671  DEC, 0b1010011111u, 9, 25, (11, 61)
#define PPUTLUTRAITS_670  DEC, 0b1010011110u, 9, 25, (2, 5, 67)
#define PPUTLUTRAITS_669  DEC, 0b1010011101u, 9, 25, (3, 223)
#define PPUTLUTRAITS_668  DEC, 0b1010011100u, 9, 25, (2, 2, 167)
#define PPUTLUTRAITS_667  DEC, 0b1010011011u, 9, 25, (23, 29)
#define PPUTLUTRAITS_666  DEC, 0b1010011010u, 9, 25, (2, 3, 3, 37)
#define PPUTLUTRAITS_665  DEC, 0b1010011001u, 9, 25, (5, 7, 19)
#define PPUTLUTRAITS_664  DEC, 0b1010011000u, 9, 25, (2, 2, 2, 83)
#define PPUTLUTRAITS_663  DEC, 0b1010010111u, 9, 25, (3, 13, 17)
#define PPUTLUTRAITS_662  DEC, 0b1010010110u, 9, 25, (2, 331)
#define PPUTLUTRAITS_661  DEC, 0b1010010101u, 9, 25, ()
#define PPUTLUTRAITS_660  DEC, 0b1010010100u, 9, 25, (2, 2, 3, 5, 11)
#define PPUTLUTRAITS_659  DEC, 0b1010010011u, 9, 25, ()
#define PPUTLUTRAITS_658  DEC, 0b1010010010u, 9, 25, (2, 7, 47)
#define PPUTLUTRAITS_657  DEC, 0b1010010001u, 9, 25, (3, 3, 73)
#define PPUTLUTRAITS_656  DEC, 0b1010010000u, 9, 25, (2, 2, 2, 2, 41)
#define PPUTLUTRAITS_655  DEC, 0b1010001111u, 9, 25, (5, 131)
#define PPUTLUTRAITS_654  DEC, 0b1010001110u, 9, 25, (2, 3, 109)
#define PPUTLUTRAITS_653  DEC, 0b1010001101u, 9, 25, ()
#define PPUTLUTRAITS_652  DEC, 0b1010001100u, 9, 25, (2, 2, 163)
#define PPUTLUTRAITS_651  DEC, 0b1010001011u, 9, 25, (3, 7, 31)
#define PPUTLUTRAITS_650  DEC, 0b1010001010u, 9, 25, (2, 5, 5, 13)
#define PPUTLUTRAITS_649  DEC, 0b1010001001u, 9, 25, (11, 59)
#define PPUTLUTRAITS_648  DEC, 0b1010001000u, 9, 25, (2, 2, 2, 3, 3, 3, 3)
#define PPUTLUTRAITS_647  DEC, 0b1010000111u, 9, 25, ()
#define PPUTLUTRAITS_646  DEC, 0b1010000110u, 9, 25, (2, 17, 19)
#define PPUTLUTRAITS_645  DEC, 0b1010000101u, 9, 25, (3, 5, 43)
#define PPUTLUTRAITS_644  DEC, 0b1010000100u, 9, 25, (2, 2, 7, 23)
#define PPUTLUTRAITS_643  DEC, 0b1010000011u, 9, 25, ()
#define PPUTLUTRAITS_642  DEC, 0b1010000010u, 9, 25, (2, 3, 107)
#define PPUTLUTRAITS_641  DEC, 0b1010000001u, 9, 25, ()
#define PPUTLUTRAITS_640  DEC, 0b1010000000u, 9, 25, (2, 2, 2, 2, 2, 2, 2, 5)
#define PPUTLUTRAITS_639  DEC, 0b1001111111u, 9, 25, (3, 3, 71)
#define PPUTLUTRAITS_638  DEC, 0b1001111110u, 9, 25, (2, 11, 29)
#define PPUTLUTRAITS_637  DEC, 0b1001111101u, 9, 25, (7, 7, 13)
#define PPUTLUTRAITS_636  DEC, 0b1001111100u, 9, 25, (2, 2, 3, 53)
#define PPUTLUTRAITS_635  DEC, 0b1001111011u, 9, 25, (5, 127)
#define PPUTLUTRAITS_634  DEC, 0b1001111010u, 9, 25, (2, 317)
#define PPUTLUTRAITS_633  DEC, 0b1001111001u, 9, 25, (3, 211)
#define PPUTLUTRAITS_632  DEC, 0b1001111000u, 9, 25, (2, 2, 2, 79)
#define PPUTLUTRAITS_631  DEC, 0b1001110111u, 9, 25, ()
#define PPUTLUTRAITS_630  DEC, 0b1001110110u, 9, 25, (2, 3, 3, 5, 7)
#define PPUTLUTRAITS_629  DEC, 0b1001110101u, 9, 25, (17, 37)
#define PPUTLUTRAITS_628  DEC, 0b1001110100u, 9, 25, (2, 2, 157)
#define PPUTLUTRAITS_627  DEC, 0b1001110011u, 9, 25, (3, 11, 19)
#define PPUTLUTRAITS_626  DEC, 0b1001110010u, 9, 25, (2, 313)
#define PPUTLUTRAITS_625  DEC, 0b1001110001u, 9, 25, (5, 5, 5, 5)
#define PPUTLUTRAITS_624  DEC, 0b1001110000u, 9, 24, (2, 2, 2, 2, 3, 13)
#define PPUTLUTRAITS_623  DEC, 0b1001101111u, 9, 24, (7, 89)
#define PPUTLUTRAITS_622  DEC, 0b1001101110u, 9, 24, (2, 311)
#define PPUTLUTRAITS_621  DEC, 0b1001101101u, 9, 24, (3, 3, 3, 23)
#define PPUTLUTRAITS_620  DEC, 0b1001101100u, 9, 24, (2, 2, 5, 31)
#define PPUTLUTRAITS_619  DEC, 0b1001101011u, 9, 24, ()
#define PPUTLUTRAITS_618  DEC, 0b1001101010u, 9, 24, (2, 3, 103)
#define PPUTLUTRAITS_617  DEC, 0b1001101001u, 9, 24, ()
#define PPUTLUTRAITS_616  DEC, 0b1001101000u, 9, 24, (2, 2, 2, 7, 11)
#define PPUTLUTRAITS_615  DEC, 0b1001100111u, 9, 24, (3, 5, 41)
#define PPUTLUTRAITS_614  DEC, 0b1001100110u, 9, 24, (2, 307)
#define PPUTLUTRAITS_613  DEC, 0b1001100101u, 9, 24, ()
#define PPUTLUTRAITS_612  DEC, 0b1001100100u, 9, 24, (2, 2, 3, 3, 17)
#define PPUTLUTRAITS_611  DEC, 0b1001100011u, 9, 24, (13, 47)
#define PPUTLUTRAITS_610  DEC, 0b1001100010u, 9, 24, (2, 5, 61)
#define PPUTLUTRAITS_609  DEC, 0b1001100001u, 9, 24, (3, 7, 29)
#define PPUTLUTRAITS_608  DEC, 0b1001100000u, 9, 24, (2, 2, 2, 2, 2, 19)
#define PPUTLUTRAITS_607  DEC, 0b1001011111u, 9, 24, ()
#define PPUTLUTRAITS_606  DEC, 0b1001011110u, 9, 24, (2, 3, 101)
#define PPUTLUTRAITS_605  DEC, 0b1001011101u, 9, 24, (5, 11, 11)
#define PPUTLUTRAITS_604  DEC, 0b1001011100u, 9, 24, (2, 2, 151)
#define PPUTLUTRAITS_603  DEC, 0b1001011011u, 9, 24, (3, 3, 67)
#define PPUTLUTRAITS_602  DEC, 0b1001011010u, 9, 24, (2, 7, 43)
#define PPUTLUTRAITS_601  DEC, 0b1001011001u, 9, 24, ()
#define PPUTLUTRAITS_600  DEC, 0b1001011000u, 9, 24, (2, 2, 2, 3, 5, 5)
#define PPUTLUTRAITS_599  DEC, 0b1001010111u, 9, 24, ()
#define PPUTLUTRAITS_598  DEC, 0b1001010110u, 9, 24, (2, 13, 23)
#define PPUTLUTRAITS_597  DEC, 0b1001010101u, 9, 24, (3, 199)
#define PPUTLUTRAITS_596  DEC, 0b1001010100u, 9, 24, (2, 2, 149)
#define PPUTLUTRAITS_595  DEC, 0b1001010011u, 9, 24, (5, 7, 17)
#define PPUTLUTRAITS_594  DEC, 0b1001010010u, 9, 24, (2, 3, 3, 3, 11)
#define PPUTLUTRAITS_593  DEC, 0b1001010001u, 9, 24, ()
#define PPUTLUTRAITS_592  DEC, 0b1001010000u, 9, 24, (2, 2, 2, 2, 37)
#define PPUTLUTRAITS_591  DEC, 0b1001001111u, 9, 24, (3, 197)
#define PPUTLUTRAITS_590  DEC, 0b1001001110u, 9, 24, (2, 5, 59)
#define PPUTLUTRAITS_589  DEC, 0b1001001101u, 9, 24, (19, 31)
#define PPUTLUTRAITS_588  DEC, 0b1001001100u, 9, 24, (2, 2, 3, 7, 7)
#define PPUTLUTRAITS_587  DEC, 0b1001001011u, 9, 24, ()
#define PPUTLUTRAITS_586  DEC, 0b1001001010u, 9, 24, (2, 293)
#define PPUTLUTRAITS_585  DEC, 0b1001001001u, 9, 24, (3, 3, 5, 13)
#define PPUTLUTRAITS_584  DEC, 0b1001001000u, 9, 24, (2, 2, 2, 73)
#define PPUTLUTRAITS_583  DEC, 0b1001000111u, 9, 24, (11, 53)
#define PPUTLUTRAITS_582  DEC, 0b1001000110u, 9, 24, (2, 3, 97)
#define PPUTLUTRAITS_581  DEC, 0b1001000101u, 9, 24, (7, 83)
#define PPUTLUTRAITS_580  DEC, 0b1001000100u, 9, 24, (2, 2, 5, 29)
#define PPUTLUTRAITS_579  DEC, 0b1001000011u, 9, 24, (3, 193)
#define PPUTLUTRAITS_578  DEC, 0b1001000010u, 9, 24, (2, 17, 17)
#define PPUTLUTRAITS_577  DEC, 0b1001000001u, 9, 24, ()
#define PPUTLUTRAITS_576  DEC, 0b1001000000u, 9, 24, (2, 2, 2, 2, 2, 2, 3, 3)
#define PPUTLUTRAITS_575  DEC, 0b1000111111u, 9, 23, (5, 5, 23)
#define PPUTLUTRAITS_574  DEC, 0b1000111110u, 9, 23, (2, 7, 41)
#define PPUTLUTRAITS_573  DEC, 0b1000111101u, 9, 23, (3, 191)
#define PPUTLUTRAITS_572  DEC, 0b1000111100u, 9, 23, (2, 2, 11, 13)
#define PPUTLUTRAITS_571  DEC, 0b1000111011u, 9, 23, ()
#define PPUTLUTRAITS_570  DEC, 0b1000111010u, 9, 23, (2, 3, 5, 19)
#define PPUTLUTRAITS_569  DEC, 0b1000111001u, 9, 23, ()
#define PPUTLUTRAITS_568  DEC, 0b1000111000u, 9, 23, (2, 2, 2, 71)
#define PPUTLUTRAITS_567  DEC, 0b1000110111u, 9, 23, (3, 3, 3, 3, 7)
#define PPUTLUTRAITS_566  DEC, 0b1000110110u, 9, 23, (2, 283)
#define PPUTLUTRAITS_565  DEC, 0b1000110101u, 9, 23, (5, 113)
#define PPUTLUTRAITS_564  DEC, 0b1000110100u, 9, 23, (2, 2, 3, 47)
#define PPUTLUTRAITS_563  DEC, 0b1000110011u, 9, 23, ()
#define PPUTLUTRAITS_562  DEC, 0b1000110010u, 9, 23, (2, 281)
#define PPUTLUTRAITS_561  DEC, 0b1000110001u, 9, 23, (3, 11, 17)
#define PPUTLUTRAITS_560  DEC, 0b1000110000u, 9, 23, (2, 2, 2, 2, 5, 7)
#define PPUTLUTRAITS_559  DEC, 0b1000101111u, 9, 23, (13, 43)
#define PPUTLUTRAITS_558  DEC, 0b1000101110u, 9, 23, (2, 3, 3, 31)
#define PPUTLUTRAITS_557  DEC, 0b1000101101u, 9, 23, ()
#define PPUTLUTRAITS_556  DEC, 0b1000101100u, 9, 23, (2, 2, 139)
#define PPUTLUTRAITS_555  DEC, 0b1000101011u, 9, 23, (3, 5, 37)
#define PPUTLUTRAITS_554  DEC, 0b1000101010u, 9, 23, (2, 277)
#define PPUTLUTRAITS_553  DEC, 0b1000101001u, 9, 23, (7, 79)
#define PPUTLUTRAITS_552  DEC, 0b1000101000u, 9, 23, (2, 2, 2, 3, 23)
#define PPUTLUTRAITS_551  DEC, 0b1000100111u, 9, 23, (19, 29)
#define PPUTLUTRAITS_550  DEC, 0b1000100110u, 9, 23, (2, 5, 5, 11)
#define PPUTLUTRAITS_549  DEC, 0b1000100101u, 9, 23, (3, 3, 61)
#define PPUTLUTRAITS_548  DEC, 0b1000100100u, 9, 23, (2, 2, 137)
#define PPUTLUTRAITS_547  DEC, 0b1000100011u, 9, 23, ()
#define PPUTLUTRAITS_546  DEC, 0b1000100010u, 9, 23, (2, 3, 7, 13)
#define PPUTLUTRAITS_545  DEC, 0b1000100001u, 9, 23, (5, 109)
#define PPUTLUTRAITS_544  DEC, 0b1000100000u, 9, 23, (2, 2, 2, 2, 2, 17)
#define PPUTLUTRAITS_543  DEC, 0b1000011111u, 9, 23, (3, 181)
#define PPUTLUTRAITS_542  DEC, 0b1000011110u, 9, 23, (2, 271)
#define PPUTLUTRAITS_541  DEC, 0b1000011101u, 9, 23, ()
#define PPUTLUTRAITS_540  DEC, 0b1000011100u, 9, 23, (2, 2, 3, 3, 3, 5)
#define PPUTLUTRAITS_539  DEC, 0b1000011011u, 9, 23, (7, 7, 11)
#define PPUTLUTRAITS_538  DEC, 0b1000011010u, 9, 23, (2, 269)
#define PPUTLUTRAITS_537  DEC, 0b1000011001u, 9, 23, (3, 179)
#define PPUTLUTRAITS_536  DEC, 0b1000011000u, 9, 23, (2, 2, 2, 67)
#define PPUTLUTRAITS_535  DEC, 0b1000010111u, 9, 23, (5, 107)
#define PPUTLUTRAITS_534  DEC, 0b1000010110u, 9, 23, (2, 3, 89)
#define PPUTLUTRAITS_533  DEC, 0b1000010101u, 9, 23, (13, 41)
#define PPUTLUTRAITS_532  DEC, 0b1000010100u, 9, 23, (2, 2, 7, 19)
#define PPUTLUTRAITS_531  DEC, 0b1000010011u, 9, 23, (3, 3, 59)
#define PPUTLUTRAITS_530  DEC, 0b1000010010u, 9, 23, (2, 5, 53)
#define PPUTLUTRAITS_529  DEC, 0b1000010001u, 9, 23, (23, 23)
#define PPUTLUTRAITS_528  DEC, 0b1000010000u, 9, 22, (2, 2, 2, 2, 3, 11)
#define PPUTLUTRAITS_527  DEC, 0b1000001111u, 9, 22, (17, 31)
#define PPUTLUTRAITS_526  DEC, 0b1000001110u, 9, 22, (2, 263)
#define PPUTLUTRAITS_525  DEC, 0b1000001101u, 9, 22, (3, 5, 5, 7)
#define PPUTLUTRAITS_524  DEC, 0b1000001100u, 9, 22, (2, 2, 131)
#define PPUTLUTRAITS_523  DEC, 0b1000001011u, 9, 22, ()
#define PPUTLUTRAITS_522  DEC, 0b1000001010u, 9, 22, (2, 3, 3, 29)
#define PPUTLUTRAITS_521  DEC, 0b1000001001u, 9, 22, ()
#define PPUTLUTRAITS_520  DEC, 0b1000001000u, 9, 22, (2, 2, 2, 5, 13)
#define PPUTLUTRAITS_519  DEC, 0b1000000111u, 9, 22, (3, 173)
#define PPUTLUTRAITS_518  DEC, 0b1000000110u, 9, 22, (2, 7, 37)
#define PPUTLUTRAITS_517  DEC, 0b1000000101u, 9, 22, (11, 47)
#define PPUTLUTRAITS_516  DEC, 0b1000000100u, 9, 22, (2, 2, 3, 43)
#define PPUTLUTRAITS_515  DEC, 0b1000000011u, 9, 22, (5, 103)
#define PPUTLUTRAITS_514  DEC, 0b1000000010u, 9, 22, (2, 257)
#define PPUTLUTRAITS_513  DEC, 0b1000000001u, 9, 22, (3, 3, 3, 19)
#define PPUTLUTRAITS_512  DEC, 0b1000000000u, 9, 22, (2, 2, 2, 2, 2, 2, 2, 2, 2)
#define PPUTLUTRAITS_511  DEC, 0b0111111111u, 8, 22, (7, 73)
#define PPUTLUTRAITS_510  DEC, 0b0111111110u, 8, 22, (2, 3, 5, 17)
#define PPUTLUTRAITS_509  DEC, 0b0111111101u, 8, 22, ()
#define PPUTLUTRAITS_508  DEC, 0b0111111100u, 8, 22, (2, 2, 127)
#define PPUTLUTRAITS_507  DEC, 0b0111111011u, 8, 22, (3, 13, 13)
#define PPUTLUTRAITS_506  DEC, 0b0111111010u, 8, 22, (2, 11, 23)
#define PPUTLUTRAITS_505  DEC, 0b0111111001u, 8, 22, (5, 101)
#define PPUTLUTRAITS_504  DEC, 0b0111111000u, 8, 22, (2, 2, 2, 3, 3, 7)
#define PPUTLUTRAITS_503  DEC, 0b0111110111u, 8, 22, ()
#define PPUTLUTRAITS_502  DEC, 0b0111110110u, 8, 22, (2, 251)
#define PPUTLUTRAITS_501  DEC, 0b0111110101u, 8, 22, (3, 167)
#define PPUTLUTRAITS_500  DEC, 0b0111110100u, 8, 22, (2, 2, 5, 5, 5)
#define PPUTLUTRAITS_499  DEC, 0b0111110011u, 8, 22, ()
#define PPUTLUTRAITS_498  DEC, 0b0111110010u, 8, 22, (2, 3, 83)
#define PPUTLUTRAITS_497  DEC, 0b0111110001u, 8, 22, (7, 71)
#define PPUTLUTRAITS_496  DEC, 0b0111110000u, 8, 22, (2, 2, 2, 2, 31)
#define PPUTLUTRAITS_495  DEC, 0b0111101111u, 8, 22, (3, 3, 5, 11)
#define PPUTLUTRAITS_494  DEC, 0b0111101110u, 8, 22, (2, 13, 19)
#define PPUTLUTRAITS_493  DEC, 0b0111101101u, 8, 22, (17, 29)
#define PPUTLUTRAITS_492  DEC, 0b0111101100u, 8, 22, (2, 2, 3, 41)
#define PPUTLUTRAITS_491  DEC, 0b0111101011u, 8, 22, ()
#define PPUTLUTRAITS_490  DEC, 0b0111101010u, 8, 22, (2, 5, 7, 7)
#define PPUTLUTRAITS_489  DEC, 0b0111101001u, 8, 22, (3, 163)
#define PPUTLUTRAITS_488  DEC, 0b0111101000u, 8, 22, (2, 2, 2, 61)
#define PPUTLUTRAITS_487  DEC, 0b0111100111u, 8, 22, ()
#define PPUTLUTRAITS_486  DEC, 0b0111100110u, 8, 22, (2, 3, 3, 3, 3, 3)
#define PPUTLUTRAITS_485  DEC, 0b0111100101u, 8, 22, (5, 97)
#define PPUTLUTRAITS_484  DEC, 0b0111100100u, 8, 22, (2, 2, 11, 11)
#define PPUTLUTRAITS_483  DEC, 0b0111100011u, 8, 21, (3, 7, 23)
#define PPUTLUTRAITS_482  DEC, 0b0111100010u, 8, 21, (2, 241)
#define PPUTLUTRAITS_481  DEC, 0b0111100001u, 8, 21, (13, 37)
#define PPUTLUTRAITS_480  DEC, 0b0111100000u, 8, 21, (2, 2, 2, 2, 2, 3, 5)
#define PPUTLUTRAITS_479  DEC, 0b0111011111u, 8, 21, ()
#define PPUTLUTRAITS_478  DEC, 0b0111011110u, 8, 21, (2, 239)
#define PPUTLUTRAITS_477  DEC, 0b0111011101u, 8, 21, (3, 3, 53)
#define PPUTLUTRAITS_476  DEC, 0b0111011100u, 8, 21, (2, 2, 7, 17)
#define PPUTLUTRAITS_475  DEC, 0b0111011011u, 8, 21, (5, 5, 19)
#define PPUTLUTRAITS_474  DEC, 0b0111011010u, 8, 21, (2, 3, 79)
#define PPUTLUTRAITS_473  DEC, 0b0111011001u, 8, 21, (11, 43)
#define PPUTLUTRAITS_472  DEC, 0b0111011000u, 8, 21, (2, 2, 2, 59)
#define PPUTLUTRAITS_471  DEC, 0b0111010111u, 8, 21, (3, 157)
#define PPUTLUTRAITS_470  DEC, 0b0111010110u, 8, 21, (2, 5, 47)
#define PPUTLUTRAITS_469  DEC, 0b0111010101u, 8, 21, (7, 67)
#define PPUTLUTRAITS_468  DEC, 0b0111010100u, 8, 21, (2, 2, 3, 3, 13)
#define PPUTLUTRAITS_467  DEC, 0b0111010011u, 8, 21, ()
#define PPUTLUTRAITS_466  DEC, 0b0111010010u, 8, 21, (2, 233)
#define PPUTLUTRAITS_465  DEC, 0b0111010001u, 8, 21, (3, 5, 31)
#define PPUTLUTRAITS_464  DEC, 0b0111010000u, 8, 21, (2, 2, 2, 2, 29)
#define PPUTLUTRAITS_463  DEC, 0b0111001111u, 8, 21, ()
#define PPUTLUTRAITS_462  DEC, 0b0111001110u, 8, 21, (2, 3, 7, 11)
#define PPUTLUTRAITS_461  DEC, 0b0111001101u, 8, 21, ()
#define PPUTLUTRAITS_460  DEC, 0b0111001100u, 8, 21, (2, 2, 5, 23)
#define PPUTLUTRAITS_459  DEC, 0b0111001011u, 8, 21, (3, 3, 3, 17)
#define PPUTLUTRAITS_458  DEC, 0b0111001010u, 8, 21, (2, 229)
#define PPUTLUTRAITS_457  DEC, 0b0111001001u, 8, 21, ()
#define PPUTLUTRAITS_456  DEC, 0b0111001000u, 8, 21, (2, 2, 2, 3, 19)
#define PPUTLUTRAITS_455  DEC, 0b0111000111u, 8, 21, (5, 7, 13)
#define PPUTLUTRAITS_454  DEC, 0b0111000110u, 8, 21, (2, 227)
#define PPUTLUTRAITS_453  DEC, 0b0111000101u, 8, 21, (3, 151)
#define PPUTLUTRAITS_452  DEC, 0b0111000100u, 8, 21, (2, 2, 113)
#define PPUTLUTRAITS_451  DEC, 0b0111000011u, 8, 21, (11, 41)
#define PPUTLUTRAITS_450  DEC, 0b0111000010u, 8, 21, (2, 3, 3, 5, 5)
#define PPUTLUTRAITS_449  DEC, 0b0111000001u, 8, 21, ()
#define PPUTLUTRAITS_448  DEC, 0b0111000000u, 8, 21, (2, 2, 2, 2, 2, 2, 7)
#define PPUTLUTRAITS_447  DEC, 0b0110111111u, 8, 21, (3, 149)
#define PPUTLUTRAITS_446  DEC, 0b0110111110u, 8, 21, (2, 223)
#define PPUTLUTRAITS_445  DEC, 0b0110111101u, 8, 21, (5, 89)
#define PPUTLUTRAITS_444  DEC, 0b0110111100u, 8, 21, (2, 2, 3, 37)
#define PPUTLUTRAITS_443  DEC, 0b0110111011u, 8, 21, ()
#define PPUTLUTRAITS_442  DEC, 0b0110111010u, 8, 21, (2, 13, 17)
#define PPUTLUTRAITS_441  DEC, 0b0110111001u, 8, 21, (3, 3, 7, 7)
#define PPUTLUTRAITS_440  DEC, 0b0110111000u, 8, 20, (2, 2, 2, 5, 11)
#define PPUTLUTRAITS_439  DEC, 0b0110110111u, 8, 20, ()
#define PPUTLUTRAITS_438  DEC, 0b0110110110u, 8, 20, (2, 3, 73)
#define PPUTLUTRAITS_437  DEC, 0b0110110101u, 8, 20, (19, 23)
#define PPUTLUTRAITS_436  DEC, 0b0110110100u, 8, 20, (2, 2, 109)
#define PPUTLUTRAITS_435  DEC, 0b0110110011u, 8, 20, (3, 5, 29)
#define PPUTLUTRAITS_434  DEC, 0b0110110010u, 8, 20, (2, 7, 31)
#define PPUTLUTRAITS_433  DEC, 0b0110110001u, 8, 20, ()
#define PPUTLUTRAITS_432  DEC, 0b0110110000u, 8, 20, (2, 2, 2, 2, 3, 3, 3)
#define PPUTLUTRAITS_431  DEC, 0b0110101111u, 8, 20, ()
#define PPUTLUTRAITS_430  DEC, 0b0110101110u, 8, 20, (2, 5, 43)
#define PPUTLUTRAITS_429  DEC, 0b0110101101u, 8, 20, (3, 11, 13)
#define PPUTLUTRAITS_428  DEC, 0b0110101100u, 8, 20, (2, 2, 107)
#define PPUTLUTRAITS_427  DEC, 0b0110101011u, 8, 20, (7, 61)
#define PPUTLUTRAITS_426  DEC, 0b0110101010u, 8, 20, (2, 3, 71)
#define PPUTLUTRAITS_425  DEC, 0b0110101001u, 8, 20, (5, 5, 17)
#define PPUTLUTRAITS_424  DEC, 0b0110101000u, 8, 20, (2, 2, 2, 53)
#define PPUTLUTRAITS_423  DEC, 0b0110100111u, 8, 20, (3, 3, 47)
#define PPUTLUTRAITS_422  DEC, 0b0110100110u, 8, 20, (2, 211)
#define PPUTLUTRAITS_421  DEC, 0b0110100101u, 8, 20, ()
#define PPUTLUTRAITS_420  DEC, 0b0110100100u, 8, 20, (2, 2, 3, 5, 7)
#define PPUTLUTRAITS_419  DEC, 0b0110100011u, 8, 20, ()
#define PPUTLUTRAITS_418  DEC, 0b0110100010u, 8, 20, (2, 11, 19)
#define PPUTLUTRAITS_417  DEC, 0b0110100001u, 8, 20, (3, 139)
#define PPUTLUTRAITS_416  DEC, 0b0110100000u, 8, 20, (2, 2, 2, 2, 2, 13)
#define PPUTLUTRAITS_415  DEC, 0b0110011111u, 8, 20, (5, 83)
#define PPUTLUTRAITS_414  DEC, 0b0110011110u, 8, 20, (2, 3, 3, 23)
#define PPUTLUTRAITS_413  DEC, 0b0110011101u, 8, 20, (7, 59)
#define PPUTLUTRAITS_412  DEC, 0b0110011100u, 8, 20, (2, 2, 103)
#define PPUTLUTRAITS_411  DEC, 0b0110011011u, 8, 20, (3, 137)
#define PPUTLUTRAITS_410  DEC, 0b0110011010u, 8, 20, (2, 5, 41)
#define PPUTLUTRAITS_409  DEC, 0b0110011001u, 8, 20, ()
#define PPUTLUTRAITS_408  DEC, 0b0110011000u, 8, 20, (2, 2, 2, 3, 17)
#define PPUTLUTRAITS_407  DEC, 0b0110010111u, 8, 20, (11, 37)
#define PPUTLUTRAITS_406  DEC, 0b0110010110u, 8, 20, (2, 7, 29)
#define PPUTLUTRAITS_405  DEC, 0b0110010101u, 8, 20, (3, 3, 3, 3, 5)
#define PPUTLUTRAITS_404  DEC, 0b0110010100u, 8, 20, (2, 2, 101)
#define PPUTLUTRAITS_403  DEC, 0b0110010011u, 8, 20, (13, 31)
#define PPUTLUTRAITS_402  DEC, 0b0110010010u, 8, 20, (2, 3, 67)
#define PPUTLUTRAITS_401  DEC, 0b0110010001u, 8, 20, ()
#define PPUTLUTRAITS_400  DEC, 0b0110010000u, 8, 20, (2, 2, 2, 2, 5, 5)
#define PPUTLUTRAITS_399  DEC, 0b0110001111u, 8, 19, (3, 7, 19)
#define PPUTLUTRAITS_398  DEC, 0b0110001110u, 8, 19, (2, 199)
#define PPUTLUTRAITS_397  DEC, 0b0110001101u, 8, 19, ()
#define PPUTLUTRAITS_396  DEC, 0b0110001100u, 8, 19, (2, 2, 3, 3, 11)
#define PPUTLUTRAITS_395  DEC, 0b0110001011u, 8, 19, (5, 79)
#define PPUTLUTRAITS_394  DEC, 0b0110001010u, 8, 19, (2, 197)
#define PPUTLUTRAITS_393  DEC, 0b0110001001u, 8, 19, (3, 131)
#define PPUTLUTRAITS_392  DEC, 0b0110001000u, 8, 19, (2, 2, 2, 7, 7)
#define PPUTLUTRAITS_391  DEC, 0b0110000111u, 8, 19, (17, 23)
#define PPUTLUTRAITS_390  DEC, 0b0110000110u, 8, 19, (2, 3, 5, 13)
#define PPUTLUTRAITS_389  DEC, 0b0110000101u, 8, 19, ()
#define PPUTLUTRAITS_388  DEC, 0b0110000100u, 8, 19, (2, 2, 97)
#define PPUTLUTRAITS_387  DEC, 0b0110000011u, 8, 19, (3, 3, 43)
#define PPUTLUTRAITS_386  DEC, 0b0110000010u, 8, 19, (2, 193)
#define PPUTLUTRAITS_385  DEC, 0b0110000001u, 8, 19, (5, 7, 11)
#define PPUTLUTRAITS_384  DEC, 0b0110000000u, 8, 19, (2, 2, 2, 2, 2, 2, 2, 3)
#define PPUTLUTRAITS_383  DEC, 0b0101111111u, 8, 19, ()
#define PPUTLUTRAITS_382  DEC, 0b0101111110u, 8, 19, (2, 191)
#define PPUTLUTRAITS_381  DEC, 0b0101111101u, 8, 19, (3, 127)
#define PPUTLUTRAITS_380  DEC, 0b0101111100u, 8, 19, (2, 2, 5, 19)
#define PPUTLUTRAITS_379  DEC, 0b0101111011u, 8, 19, ()
#define PPUTLUTRAITS_378  DEC, 0b0101111010u, 8, 19, (2, 3, 3, 3, 7)
#define PPUTLUTRAITS_377  DEC, 0b0101111001u, 8, 19, (13, 29)
#define PPUTLUTRAITS_376  DEC, 0b0101111000u, 8, 19, (2, 2, 2, 47)
#define PPUTLUTRAITS_375  DEC, 0b0101110111u, 8, 19, (3, 5, 5, 5)
#define PPUTLUTRAITS_374  DEC, 0b0101110110u, 8, 19, (2, 11, 17)
#define PPUTLUTRAITS_373  DEC, 0b0101110101u, 8, 19, ()
#define PPUTLUTRAITS_372  DEC, 0b0101110100u, 8, 19, (2, 2, 3, 31)
#define PPUTLUTRAITS_371  DEC, 0b0101110011u, 8, 19, (7, 53)
#define PPUTLUTRAITS_370  DEC, 0b0101110010u, 8, 19, (2, 5, 37)
#define PPUTLUTRAITS_369  DEC, 0b0101110001u, 8, 19, (3, 3, 41)
#define PPUTLUTRAITS_368  DEC, 0b0101110000u, 8, 19, (2, 2, 2, 2, 23)
#define PPUTLUTRAITS_367  DEC, 0b0101101111u, 8, 19, ()
#define PPUTLUTRAITS_366  DEC, 0b0101101110u, 8, 19, (2, 3, 61)
#define PPUTLUTRAITS_365  DEC, 0b0101101101u, 8, 19, (5, 73)
#define PPUTLUTRAITS_364  DEC, 0b0101101100u, 8, 19, (2, 2, 7, 13)
#define PPUTLUTRAITS_363  DEC, 0b0101101011u, 8, 19, (3, 11, 11)
#define PPUTLUTRAITS_362  DEC, 0b0101101010u, 8, 19, (2, 181)
#define PPUTLUTRAITS_361  DEC, 0b0101101001u, 8, 19, (19, 19)
#define PPUTLUTRAITS_360  DEC, 0b0101101000u, 8, 18, (2, 2, 2, 3, 3, 5)
#define PPUTLUTRAITS_359  DEC, 0b0101100111u, 8, 18, ()
#define PPUTLUTRAITS_358  DEC, 0b0101100110u, 8, 18, (2, 179)
#define PPUTLUTRAITS_357  DEC, 0b0101100101u, 8, 18, (3, 7, 17)
#define PPUTLUTRAITS_356  DEC, 0b0101100100u, 8, 18, (2, 2, 89)
#define PPUTLUTRAITS_355  DEC, 0b0101100011u, 8, 18, (5, 71)
#define PPUTLUTRAITS_354  DEC, 0b0101100010u, 8, 18, (2, 3, 59)
#define PPUTLUTRAITS_353  DEC, 0b0101100001u, 8, 18, ()
#define PPUTLUTRAITS_352  DEC, 0b0101100000u, 8, 18, (2, 2, 2, 2, 2, 11)
#define PPUTLUTRAITS_351  DEC, 0b0101011111u, 8, 18, (3, 3, 3, 13)
#define PPUTLUTRAITS_350  DEC, 0b0101011110u, 8, 18, (2, 5, 5, 7)
#define PPUTLUTRAITS_349  DEC, 0b0101011101u, 8, 18, ()
#define PPUTLUTRAITS_348  DEC, 0b0101011100u, 8, 18, (2, 2, 3, 29)
#define PPUTLUTRAITS_347  DEC, 0b0101011011u, 8, 18, ()
#define PPUTLUTRAITS_346  DEC, 0b0101011010u, 8, 18, (2, 173)
#define PPUTLUTRAITS_345  DEC, 0b0101011001u, 8, 18, (3, 5, 23)
#define PPUTLUTRAITS_344  DEC, 0b0101011000u, 8, 18, (2, 2, 2, 43)
#define PPUTLUTRAITS_343  DEC, 0b0101010111u, 8, 18, (7, 7, 7)
#define PPUTLUTRAITS_342  DEC, 0b0101010110u, 8, 18, (2, 3, 3, 19)
#define PPUTLUTRAITS_341  DEC, 0b0101010101u, 8, 18, (11, 31)
#define PPUTLUTRAITS_340  DEC, 0b0101010100u, 8, 18, (2, 2, 5, 17)
#define PPUTLUTRAITS_339  DEC, 0b0101010011u, 8, 18, (3, 113)
#define PPUTLUTRAITS_338  DEC, 0b0101010010u, 8, 18, (2, 13, 13)
#define PPUTLUTRAITS_337  DEC, 0b0101010001u, 8, 18, ()
#define PPUTLUTRAITS_336  DEC, 0b0101010000u, 8, 18, (2, 2, 2, 2, 3, 7)
#define PPUTLUTRAITS_335  DEC, 0b0101001111u, 8, 18, (5, 67)
#define PPUTLUTRAITS_334  DEC, 0b0101001110u, 8, 18, (2, 167)
#define PPUTLUTRAITS_333  DEC, 0b0101001101u, 8, 18, (3, 3, 37)
#define PPUTLUTRAITS_332  DEC, 0b0101001100u, 8, 18, (2, 2, 83)
#define PPUTLUTRAITS_331  DEC, 0b0101001011u, 8, 18, ()
#define PPUTLUTRAITS_330  DEC, 0b0101001010u, 8, 18, (2, 3, 5, 11)
#define PPUTLUTRAITS_329  DEC, 0b0101001001u, 8, 18, (7, 47)
#define PPUTLUTRAITS_328  DEC, 0b0101001000u, 8, 18, (2, 2, 2, 41)
#define PPUTLUTRAITS_327  DEC, 0b0101000111u, 8, 18, (3, 109)
#define PPUTLUTRAITS_326  DEC, 0b0101000110u, 8, 18, (2, 163)
#define PPUTLUTRAITS_325  DEC, 0b0101000101u, 8, 18, (5, 5, 13)
#define PPUTLUTRAITS_324  DEC, 0b0101000100u, 8, 18, (2, 2, 3, 3, 3, 3)
#define PPUTLUTRAITS_323  DEC, 0b0101000011u, 8, 17, (17, 19)
#define PPUTLUTRAITS_322  DEC, 0b0101000010u, 8, 17, (2, 7, 23)
#define PPUTLUTRAITS_321  DEC, 0b0101000001u, 8, 17, (3, 107)
#define PPUTLUTRAITS_320  DEC, 0b0101000000u, 8, 17, (2, 2, 2, 2, 2, 2, 5)
#define PPUTLUTRAITS_319  DEC, 0b0100111111u, 8, 17, (11, 29)
#define PPUTLUTRAITS_318  DEC, 0b0100111110u, 8, 17, (2, 3, 53)
#define PPUTLUTRAITS_317  DEC, 0b0100111101u, 8, 17, ()
#define PPUTLUTRAITS_316  DEC, 0b0100111100u, 8, 17, (2, 2, 79)
#define PPUTLUTRAITS_315  DEC, 0b0100111011u, 8, 17, (3, 3, 5, 7)
#define PPUTLUTRAITS_314  DEC, 0b0100111010u, 8, 17, (2, 157)
#define PPUTLUTRAITS_313  DEC, 0b0100111001u, 8, 17, ()
#define PPUTLUTRAITS_312  DEC, 0b0100111000u, 8, 17, (2, 2, 2, 3, 13)
#define PPUTLUTRAITS_311  DEC, 0b0100110111u, 8, 17, ()
#define PPUTLUTRAITS_310  DEC, 0b0100110110u, 8, 17, (2, 5, 31)
#define PPUTLUTRAITS_309  DEC, 0b0100110101u, 8, 17, (3, 103)
#define PPUTLUTRAITS_308  DEC, 0b0100110100u, 8, 17, (2, 2, 7, 11)
#define PPUTLUTRAITS_307  DEC, 0b0100110011u, 8, 17, ()
#define PPUTLUTRAITS_306  DEC, 0b0100110010u, 8, 17, (2, 3, 3, 17)
#define PPUTLUTRAITS_305  DEC, 0b0100110001u, 8, 17, (5, 61)
#define PPUTLUTRAITS_304  DEC, 0b0100110000u, 8, 17, (2, 2, 2, 2, 19)
#define PPUTLUTRAITS_303  DEC, 0b0100101111u, 8, 17, (3, 101)
#define PPUTLUTRAITS_302  DEC, 0b0100101110u, 8, 17, (2, 151)
#define PPUTLUTRAITS_301  DEC, 0b0100101101u, 8, 17, (7, 43)
#define PPUTLUTRAITS_300  DEC, 0b0100101100u, 8, 17, (2, 2, 3, 5, 5)
#define PPUTLUTRAITS_299  DEC, 0b0100101011u, 8, 17, (13, 23)
#define PPUTLUTRAITS_298  DEC, 0b0100101010u, 8, 17, (2, 149)
#define PPUTLUTRAITS_297  DEC, 0b0100101001u, 8, 17, (3, 3, 3, 11)
#define PPUTLUTRAITS_296  DEC, 0b0100101000u, 8, 17, (2, 2, 2, 37)
#define PPUTLUTRAITS_295  DEC, 0b0100100111u, 8, 17, (5, 59)
#define PPUTLUTRAITS_294  DEC, 0b0100100110u, 8, 17, (2, 3, 7, 7)
#define PPUTLUTRAITS_293  DEC, 0b0100100101u, 8, 17, ()
#define PPUTLUTRAITS_292  DEC, 0b0100100100u, 8, 17, (2, 2, 73)
#define PPUTLUTRAITS_291  DEC, 0b0100100011u, 8, 17, (3, 97)
#define PPUTLUTRAITS_290  DEC, 0b0100100010u, 8, 17, (2, 5, 29)
#define PPUTLUTRAITS_289  DEC, 0b0100100001u, 8, 17, (17, 17)
#define PPUTLUTRAITS_288  DEC, 0b0100100000u, 8, 16, (2, 2, 2, 2, 2, 3, 3)
#define PPUTLUTRAITS_287  DEC, 0b0100011111u, 8, 16, (7, 41)
#define PPUTLUTRAITS_286  DEC, 0b0100011110u, 8, 16, (2, 11, 13)
#define PPUTLUTRAITS_285  DEC, 0b0100011101u, 8, 16, (3, 5, 19)
#define PPUTLUTRAITS_284  DEC, 0b0100011100u, 8, 16, (2, 2, 71)
#define PPUTLUTRAITS_283  DEC, 0b0100011011u, 8, 16, ()
#define PPUTLUTRAITS_282  DEC, 0b0100011010u, 8, 16, (2, 3, 47)
#define PPUTLUTRAITS_281  DEC, 0b0100011001u, 8, 16, ()
#define PPUTLUTRAITS_280  DEC, 0b0100011000u, 8, 16, (2, 2, 2, 5, 7)
#define PPUTLUTRAITS_279  DEC, 0b0100010111u, 8, 16, (3, 3, 31)
#define PPUTLUTRAITS_278  DEC, 0b0100010110u, 8, 16, (2, 139)
#define PPUTLUTRAITS_277  DEC, 0b0100010101u, 8, 16, ()
#define PPUTLUTRAITS_276  DEC, 0b0100010100u, 8, 16, (2, 2, 3, 23)
#define PPUTLUTRAITS_275  DEC, 0b0100010011u, 8, 16, (5, 5, 11)
#define PPUTLUTRAITS_274  DEC, 0b0100010010u, 8, 16, (2, 137)
#define PPUTLUTRAITS_273  DEC, 0b0100010001u, 8, 16, (3, 7, 13)
#define PPUTLUTRAITS_272  DEC, 0b0100010000u, 8, 16, (2, 2, 2, 2, 17)
#define PPUTLUTRAITS_271  DEC, 0b0100001111u, 8, 16, ()
#define PPUTLUTRAITS_270  DEC, 0b0100001110u, 8, 16, (2, 3, 3, 3, 5)
#define PPUTLUTRAITS_269  DEC, 0b0100001101u, 8, 16, ()
#define PPUTLUTRAITS_268  DEC, 0b0100001100u, 8, 16, (2, 2, 67)
#define PPUTLUTRAITS_267  DEC, 0b0100001011u, 8, 16, (3, 89)
#define PPUTLUTRAITS_266  DEC, 0b0100001010u, 8, 16, (2, 7, 19)
#define PPUTLUTRAITS_265  DEC, 0b0100001001u, 8, 16, (5, 53)
#define PPUTLUTRAITS_264  DEC, 0b0100001000u, 8, 16, (2, 2, 2, 3, 11)
#define PPUTLUTRAITS_263  DEC, 0b0100000111u, 8, 16, ()
#define PPUTLUTRAITS_262  DEC, 0b0100000110u, 8, 16, (2, 131)
#define PPUTLUTRAITS_261  DEC, 0b0100000101u, 8, 16, (3, 3, 29)
#define PPUTLUTRAITS_260  DEC, 0b0100000100u, 8, 16, (2, 2, 5, 13)
#define PPUTLUTRAITS_259  DEC, 0b0100000011u, 8, 16, (7, 37)
#define PPUTLUTRAITS_258  DEC, 0b0100000010u, 8, 16, (2, 3, 43)
#define PPUTLUTRAITS_257  DEC, 0b0100000001u, 8, 16, ()
#define PPUTLUTRAITS_256  DEC, 0b0100000000u, 8, 16, (2, 2, 2, 2, 2, 2, 2, 2)
#define PPUTLUTRAITS_255  DEC, 0b0011111111u, 7, 15, (3, 5, 17)
#define PPUTLUTRAITS_254  DEC, 0b0011111110u, 7, 15, (2, 127)
#define PPUTLUTRAITS_253  DEC, 0b0011111101u, 7, 15, (11, 23)
#define PPUTLUTRAITS_252  DEC, 0b0011111100u, 7, 15, (2, 2, 3, 3, 7)
#define PPUTLUTRAITS_251  DEC, 0b0011111011u, 7, 15, ()
#define PPUTLUTRAITS_250  DEC, 0b0011111010u, 7, 15, (2, 5, 5, 5)
#define PPUTLUTRAITS_249  DEC, 0b0011111001u, 7, 15, (3, 83)
#define PPUTLUTRAITS_248  DEC, 0b0011111000u, 7, 15, (2, 2, 2, 31)
#define PPUTLUTRAITS_247  DEC, 0b0011110111u, 7, 15, (13, 19)
#define PPUTLUTRAITS_246  DEC, 0b0011110110u, 7, 15, (2, 3, 41)
#define PPUTLUTRAITS_245  DEC, 0b0011110101u, 7, 15, (5, 7, 7)
#define PPUTLUTRAITS_244  DEC, 0b0011110100u, 7, 15, (2, 2, 61)
#define PPUTLUTRAITS_243  DEC, 0b0011110011u, 7, 15, (3, 3, 3, 3, 3)
#define PPUTLUTRAITS_242  DEC, 0b0011110010u, 7, 15, (2, 11, 11)
#define PPUTLUTRAITS_241  DEC, 0b0011110001u, 7, 15, ()
#define PPUTLUTRAITS_240  DEC, 0b0011110000u, 7, 15, (2, 2, 2, 2, 3, 5)
#define PPUTLUTRAITS_239  DEC, 0b0011101111u, 7, 15, ()
#define PPUTLUTRAITS_238  DEC, 0b0011101110u, 7, 15, (2, 7, 17)
#define PPUTLUTRAITS_237  DEC, 0b0011101101u, 7, 15, (3, 79)
#define PPUTLUTRAITS_236  DEC, 0b0011101100u, 7, 15, (2, 2, 59)
#define PPUTLUTRAITS_235  DEC, 0b0011101011u, 7, 15, (5, 47)
#define PPUTLUTRAITS_234  DEC, 0b0011101010u, 7, 15, (2, 3, 3, 13)
#define PPUTLUTRAITS_233  DEC, 0b0011101001u, 7, 15, ()
#define PPUTLUTRAITS_232  DEC, 0b0011101000u, 7, 15, (2, 2, 2, 29)
#define PPUTLUTRAITS_231  DEC, 0b0011100111u, 7, 15, (3, 7, 11)
#define PPUTLUTRAITS_230  DEC, 0b0011100110u, 7, 15, (2, 5, 23)
#define PPUTLUTRAITS_229  DEC, 0b0011100101u, 7, 15, ()
#define PPUTLUTRAITS_228  DEC, 0b0011100100u, 7, 15, (2, 2, 3, 19)
#define PPUTLUTRAITS_227  DEC, 0b0011100011u, 7, 15, ()
#define PPUTLUTRAITS_226  DEC, 0b0011100010u, 7, 15, (2, 113)
#define PPUTLUTRAITS_225  DEC, 0b0011100001u, 7, 15, (3, 3, 5, 5)
#define PPUTLUTRAITS_224  DEC, 0b0011100000u, 7, 14, (2, 2, 2, 2, 2, 7)
#define PPUTLUTRAITS_223  DEC, 0b0011011111u, 7, 14, ()
#define PPUTLUTRAITS_222  DEC, 0b0011011110u, 7, 14, (2, 3, 37)
#define PPUTLUTRAITS_221  DEC, 0b0011011101u, 7, 14, (13, 17)
#define PPUTLUTRAITS_220  DEC, 0b0011011100u, 7, 14, (2, 2, 5, 11)
#define PPUTLUTRAITS_219  DEC, 0b0011011011u, 7, 14, (3, 73)
#define PPUTLUTRAITS_218  DEC, 0b0011011010u, 7, 14, (2, 109)
#define PPUTLUTRAITS_217  DEC, 0b0011011001u, 7, 14, (7, 31)
#define PPUTLUTRAITS_216  DEC, 0b0011011000u, 7, 14, (2, 2, 2, 3, 3, 3)
#define PPUTLUTRAITS_215  DEC, 0b0011010111u, 7, 14, (5, 43)
#define PPUTLUTRAITS_214  DEC, 0b0011010110u, 7, 14, (2, 107)
#define PPUTLUTRAITS_213  DEC, 0b0011010101u, 7, 14, (3, 71)
#define PPUTLUTRAITS_212  DEC, 0b0011010100u, 7, 14, (2, 2, 53)
#define PPUTLUTRAITS_211  DEC, 0b0011010011u, 7, 14, ()
#define PPUTLUTRAITS_210  DEC, 0b0011010010u, 7, 14, (2, 3, 5, 7)
#define PPUTLUTRAITS_209  DEC, 0b0011010001u, 7, 14, (11, 19)
#define PPUTLUTRAITS_208  DEC, 0b0011010000u, 7, 14, (2, 2, 2, 2, 13)
#define PPUTLUTRAITS_207  DEC, 0b0011001111u, 7, 14, (3, 3, 23)
#define PPUTLUTRAITS_206  DEC, 0b0011001110u, 7, 14, (2, 103)
#define PPUTLUTRAITS_205  DEC, 0b0011001101u, 7, 14, (5, 41)
#define PPUTLUTRAITS_204  DEC, 0b0011001100u, 7, 14, (2, 2, 3, 17)
#define PPUTLUTRAITS_203  DEC, 0b0011001011u, 7, 14, (7, 29)
#define PPUTLUTRAITS_202  DEC, 0b0011001010u, 7, 14, (2, 101)
#define PPUTLUTRAITS_201  DEC, 0b0011001001u, 7, 14, (3, 67)
#define PPUTLUTRAITS_200  DEC, 0b0011001000u, 7, 14, (2, 2, 2, 5, 5)
#define PPUTLUTRAITS_199  DEC, 0b0011000111u, 7, 14, ()
#define PPUTLUTRAITS_198  DEC, 0b0011000110u, 7, 14, (2, 3, 3, 11)
#define PPUTLUTRAITS_197  DEC, 0b0011000101u, 7, 14, ()
#define PPUTLUTRAITS_196  DEC, 0b0011000100u, 7, 14, (2, 2, 7, 7)
#define PPUTLUTRAITS_195  DEC, 0b0011000011u, 7, 13, (3, 5, 13)
#define PPUTLUTRAITS_194  DEC, 0b0011000010u, 7, 13, (2, 97)
#define PPUTLUTRAITS_193  DEC, 0b0011000001u, 7, 13, ()
#define PPUTLUTRAITS_192  DEC, 0b0011000000u, 7, 13, (2, 2, 2, 2, 2, 2, 3)
#define PPUTLUTRAITS_191  DEC, 0b0010111111u, 7, 13, ()
#define PPUTLUTRAITS_190  DEC, 0b0010111110u, 7, 13, (2, 5, 19)
#define PPUTLUTRAITS_189  DEC, 0b0010111101u, 7, 13, (3, 3, 3, 7)
#define PPUTLUTRAITS_188  DEC, 0b0010111100u, 7, 13, (2, 2, 47)
#define PPUTLUTRAITS_187  DEC, 0b0010111011u, 7, 13, (11, 17)
#define PPUTLUTRAITS_186  DEC, 0b0010111010u, 7, 13, (2, 3, 31)
#define PPUTLUTRAITS_185  DEC, 0b0010111001u, 7, 13, (5, 37)
#define PPUTLUTRAITS_184  DEC, 0b0010111000u, 7, 13, (2, 2, 2, 23)
#define PPUTLUTRAITS_183  DEC, 0b0010110111u, 7, 13, (3, 61)
#define PPUTLUTRAITS_182  DEC, 0b0010110110u, 7, 13, (2, 7, 13)
#define PPUTLUTRAITS_181  DEC, 0b0010110101u, 7, 13, ()
#define PPUTLUTRAITS_180  DEC, 0b0010110100u, 7, 13, (2, 2, 3, 3, 5)
#define PPUTLUTRAITS_179  DEC, 0b0010110011u, 7, 13, ()
#define PPUTLUTRAITS_178  DEC, 0b0010110010u, 7, 13, (2, 89)
#define PPUTLUTRAITS_177  DEC, 0b0010110001u, 7, 13, (3, 59)
#define PPUTLUTRAITS_176  DEC, 0b0010110000u, 7, 13, (2, 2, 2, 2, 11)
#define PPUTLUTRAITS_175  DEC, 0b0010101111u, 7, 13, (5, 5, 7)
#define PPUTLUTRAITS_174  DEC, 0b0010101110u, 7, 13, (2, 3, 29)
#define PPUTLUTRAITS_173  DEC, 0b0010101101u, 7, 13, ()
#define PPUTLUTRAITS_172  DEC, 0b0010101100u, 7, 13, (2, 2, 43)
#define PPUTLUTRAITS_171  DEC, 0b0010101011u, 7, 13, (3, 3, 19)
#define PPUTLUTRAITS_170  DEC, 0b0010101010u, 7, 13, (2, 5, 17)
#define PPUTLUTRAITS_169  DEC, 0b0010101001u, 7, 13, (13, 13)
#define PPUTLUTRAITS_168  DEC, 0b0010101000u, 7, 12, (2, 2, 2, 3, 7)
#define PPUTLUTRAITS_167  DEC, 0b0010100111u, 7, 12, ()
#define PPUTLUTRAITS_166  DEC, 0b0010100110u, 7, 12, (2, 83)
#define PPUTLUTRAITS_165  DEC, 0b0010100101u, 7, 12, (3, 5, 11)
#define PPUTLUTRAITS_164  DEC, 0b0010100100u, 7, 12, (2, 2, 41)
#define PPUTLUTRAITS_163  DEC, 0b0010100011u, 7, 12, ()
#define PPUTLUTRAITS_162  DEC, 0b0010100010u, 7, 12, (2, 3, 3, 3, 3)
#define PPUTLUTRAITS_161  DEC, 0b0010100001u, 7, 12, (7, 23)
#define PPUTLUTRAITS_160  DEC, 0b0010100000u, 7, 12, (2, 2, 2, 2, 2, 5)
#define PPUTLUTRAITS_159  DEC, 0b0010011111u, 7, 12, (3, 53)
#define PPUTLUTRAITS_158  DEC, 0b0010011110u, 7, 12, (2, 79)
#define PPUTLUTRAITS_157  DEC, 0b0010011101u, 7, 12, ()
#define PPUTLUTRAITS_156  DEC, 0b0010011100u, 7, 12, (2, 2, 3, 13)
#define PPUTLUTRAITS_155  DEC, 0b0010011011u, 7, 12, (5, 31)
#define PPUTLUTRAITS_154  DEC, 0b0010011010u, 7, 12, (2, 7, 11)
#define PPUTLUTRAITS_153  DEC, 0b0010011001u, 7, 12, (3, 3, 17)
#define PPUTLUTRAITS_152  DEC, 0b0010011000u, 7, 12, (2, 2, 2, 19)
#define PPUTLUTRAITS_151  DEC, 0b0010010111u, 7, 12, ()
#define PPUTLUTRAITS_150  DEC, 0b0010010110u, 7, 12, (2, 3, 5, 5)
#define PPUTLUTRAITS_149  DEC, 0b0010010101u, 7, 12, ()
#define PPUTLUTRAITS_148  DEC, 0b0010010100u, 7, 12, (2, 2, 37)
#define PPUTLUTRAITS_147  DEC, 0b0010010011u, 7, 12, (3, 7, 7)
#define PPUTLUTRAITS_146  DEC, 0b0010010010u, 7, 12, (2, 73)
#define PPUTLUTRAITS_145  DEC, 0b0010010001u, 7, 12, (5, 29)
#define PPUTLUTRAITS_144  DEC, 0b0010010000u, 7, 12, (2, 2, 2, 2, 3, 3)
#define PPUTLUTRAITS_143  DEC, 0b0010001111u, 7, 11, (11, 13)
#define PPUTLUTRAITS_142  DEC, 0b0010001110u, 7, 11, (2, 71)
#define PPUTLUTRAITS_141  DEC, 0b0010001101u, 7, 11, (3, 47)
#define PPUTLUTRAITS_140  DEC, 0b0010001100u, 7, 11, (2, 2, 5, 7)
#define PPUTLUTRAITS_139  DEC, 0b0010001011u, 7, 11, ()
#define PPUTLUTRAITS_138  DEC, 0b0010001010u, 7, 11, (2, 3, 23)
#define PPUTLUTRAITS_137  DEC, 0b0010001001u, 7, 11, ()
#define PPUTLUTRAITS_136  DEC, 0b0010001000u, 7, 11, (2, 2, 2, 17)
#define PPUTLUTRAITS_135  DEC, 0b0010000111u, 7, 11, (3, 3, 3, 5)
#define PPUTLUTRAITS_134  DEC, 0b0010000110u, 7, 11, (2, 67)
#define PPUTLUTRAITS_133  DEC, 0b0010000101u, 7, 11, (7, 19)
#define PPUTLUTRAITS_132  DEC, 0b0010000100u, 7, 11, (2, 2, 3, 11)
#define PPUTLUTRAITS_131  DEC, 0b0010000011u, 7, 11, ()
#define PPUTLUTRAITS_130  DEC, 0b0010000010u, 7, 11, (2, 5, 13)
#define PPUTLUTRAITS_129  DEC, 0b0010000001u, 7, 11, (3, 43)
#define PPUTLUTRAITS_128  DEC, 0b0010000000u, 7, 11, (2, 2, 2, 2, 2, 2, 2)
#define PPUTLUTRAITS_127  DEC, 0b0001111111u, 6, 11, ()
#define PPUTLUTRAITS_126  DEC, 0b0001111110u, 6, 11, (2, 3, 3, 7)
#define PPUTLUTRAITS_125  DEC, 0b0001111101u, 6, 11, (5, 5, 5)
#define PPUTLUTRAITS_124  DEC, 0b0001111100u, 6, 11, (2, 2, 31)
#define PPUTLUTRAITS_123  DEC, 0b0001111011u, 6, 11, (3, 41)
#define PPUTLUTRAITS_122  DEC, 0b0001111010u, 6, 11, (2, 61)
#define PPUTLUTRAITS_121  DEC, 0b0001111001u, 6, 11, (11, 11)
#define PPUTLUTRAITS_120  DEC, 0b0001111000u, 6, 10, (2, 2, 2, 3, 5)
#define PPUTLUTRAITS_119  DEC, 0b0001110111u, 6, 10, (7, 17)
#define PPUTLUTRAITS_118  DEC, 0b0001110110u, 6, 10, (2, 59)
#define PPUTLUTRAITS_117  DEC, 0b0001110101u, 6, 10, (3, 3, 13)
#define PPUTLUTRAITS_116  DEC, 0b0001110100u, 6, 10, (2, 2, 29)
#define PPUTLUTRAITS_115  DEC, 0b0001110011u, 6, 10, (5, 23)
#define PPUTLUTRAITS_114  DEC, 0b0001110010u, 6, 10, (2, 3, 19)
#define PPUTLUTRAITS_113  DEC, 0b0001110001u, 6, 10, ()
#define PPUTLUTRAITS_112  DEC, 0b0001110000u, 6, 10, (2, 2, 2, 2, 7)
#define PPUTLUTRAITS_111  DEC, 0b0001101111u, 6, 10, (3, 37)
#define PPUTLUTRAITS_110  DEC, 0b0001101110u, 6, 10, (2, 5, 11)
#define PPUTLUTRAITS_109  DEC, 0b0001101101u, 6, 10, ()
#define PPUTLUTRAITS_108  DEC, 0b0001101100u, 6, 10, (2, 2, 3, 3, 3)
#define PPUTLUTRAITS_107  DEC, 0b0001101011u, 6, 10, ()
#define PPUTLUTRAITS_106  DEC, 0b0001101010u, 6, 10, (2, 53)
#define PPUTLUTRAITS_105  DEC, 0b0001101001u, 6, 10, (3, 5, 7)
#define PPUTLUTRAITS_104  DEC, 0b0001101000u, 6, 10, (2, 2, 2, 13)
#define PPUTLUTRAITS_103  DEC, 0b0001100111u, 6, 10, ()
#define PPUTLUTRAITS_102  DEC, 0b0001100110u, 6, 10, (2, 3, 17)
#define PPUTLUTRAITS_101  DEC, 0b0001100101u, 6, 10, ()
#define PPUTLUTRAITS_100  DEC, 0b0001100100u, 6, 10, (2, 2, 5, 5)
#define PPUTLUTRAITS_99   DEC, 0b0001100011u, 6, 9, (3, 3, 11)
#define PPUTLUTRAITS_98   DEC, 0b0001100010u, 6, 9, (2, 7, 7)
#define PPUTLUTRAITS_97   DEC, 0b0001100001u, 6, 9, ()
#define PPUTLUTRAITS_96   DEC, 0b0001100000u, 6, 9, (2, 2, 2, 2, 2, 3)
#define PPUTLUTRAITS_95   DEC, 0b0001011111u, 6, 9, (5, 19)
#define PPUTLUTRAITS_94   DEC, 0b0001011110u, 6, 9, (2, 47)
#define PPUTLUTRAITS_93   DEC, 0b0001011101u, 6, 9, (3, 31)
#define PPUTLUTRAITS_92   DEC, 0b0001011100u, 6, 9, (2, 2, 23)
#define PPUTLUTRAITS_91   DEC, 0b0001011011u, 6, 9, (7, 13)
#define PPUTLUTRAITS_90   DEC, 0b0001011010u, 6, 9, (2, 3, 3, 5)
#define PPUTLUTRAITS_89   DEC, 0b0001011001u, 6, 9, ()
#define PPUTLUTRAITS_88   DEC, 0b0001011000u, 6, 9, (2, 2, 2, 11)
#define PPUTLUTRAITS_87   DEC, 0b0001010111u, 6, 9, (3, 29)
#define PPUTLUTRAITS_86   DEC, 0b0001010110u, 6, 9, (2, 43)
#define PPUTLUTRAITS_85   DEC, 0b0001010101u, 6, 9, (5, 17)
#define PPUTLUTRAITS_84   DEC, 0b0001010100u, 6, 9, (2, 2, 3, 7)
#define PPUTLUTRAITS_83   DEC, 0b0001010011u, 6, 9, ()
#define PPUTLUTRAITS_82   DEC, 0b0001010010u, 6, 9, (2, 41)
#define PPUTLUTRAITS_81   DEC, 0b0001010001u, 6, 9, (3, 3, 3, 3)
#define PPUTLUTRAITS_80   DEC, 0b0001010000u, 6, 8, (2, 2, 2, 2, 5)
#define PPUTLUTRAITS_79   DEC, 0b0001001111u, 6, 8, ()
#define PPUTLUTRAITS_78   DEC, 0b0001001110u, 6, 8, (2, 3, 13)
#define PPUTLUTRAITS_77   DEC, 0b0001001101u, 6, 8, (7, 11)
#define PPUTLUTRAITS_76   DEC, 0b0001001100u, 6, 8, (2, 2, 19)
#define PPUTLUTRAITS_75   DEC, 0b0001001011u, 6, 8, (3, 5, 5)
#define PPUTLUTRAITS_74   DEC, 0b0001001010u, 6, 8, (2, 37)
#define PPUTLUTRAITS_73   DEC, 0b0001001001u, 6, 8, ()
#define PPUTLUTRAITS_72   DEC, 0b0001001000u, 6, 8, (2, 2, 2, 3, 3)
#define PPUTLUTRAITS_71   DEC, 0b0001000111u, 6, 8, ()
#define PPUTLUTRAITS_70   DEC, 0b0001000110u, 6, 8, (2, 5, 7)
#define PPUTLUTRAITS_69   DEC, 0b0001000101u, 6, 8, (3, 23)
#define PPUTLUTRAITS_68   DEC, 0b0001000100u, 6, 8, (2, 2, 17)
#define PPUTLUTRAITS_67   DEC, 0b0001000011u, 6, 8, ()
#define PPUTLUTRAITS_66   DEC, 0b0001000010u, 6, 8, (2, 3, 11)
#define PPUTLUTRAITS_65   DEC, 0b0001000001u, 6, 8, (5, 13)
#define PPUTLUTRAITS_64   DEC, 0b0001000000u, 6, 8, (2, 2, 2, 2, 2, 2)
#define PPUTLUTRAITS_63   DEC, 0b0000111111u, 5, 7, (3, 3, 7)
#define PPUTLUTRAITS_62   DEC, 0b0000111110u, 5, 7, (2, 31)
#define PPUTLUTRAITS_61   DEC, 0b0000111101u, 5, 7, ()
#define PPUTLUTRAITS_60   DEC, 0b0000111100u, 5, 7, (2, 2, 3, 5)
#define PPUTLUTRAITS_59   DEC, 0b0000111011u, 5, 7, ()
#define PPUTLUTRAITS_58   DEC, 0b0000111010u, 5, 7, (2, 29)
#define PPUTLUTRAITS_57   DEC, 0b0000111001u, 5, 7, (3, 19)
#define PPUTLUTRAITS_56   DEC, 0b0000111000u, 5, 7, (2, 2, 2, 7)
#define PPUTLUTRAITS_55   DEC, 0b0000110111u, 5, 7, (5, 11)
#define PPUTLUTRAITS_54   DEC, 0b0000110110u, 5, 7, (2, 3, 3, 3)
#define PPUTLUTRAITS_53   DEC, 0b0000110101u, 5, 7, ()
#define PPUTLUTRAITS_52   DEC, 0b0000110100u, 5, 7, (2, 2, 13)
#define PPUTLUTRAITS_51   DEC, 0b0000110011u, 5, 7, (3, 17)
#define PPUTLUTRAITS_50   DEC, 0b0000110010u, 5, 7, (2, 5, 5)
#define PPUTLUTRAITS_49   DEC, 0b0000110001u, 5, 7, (7, 7)
#define PPUTLUTRAITS_48   DEC, 0b0000110000u, 5, 6, (2, 2, 2, 2, 3)
#define PPUTLUTRAITS_47   DEC, 0b0000101111u, 5, 6, ()
#define PPUTLUTRAITS_46   DEC, 0b0000101110u, 5, 6, (2, 23)
#define PPUTLUTRAITS_45   DEC, 0b0000101101u, 5, 6, (3, 3, 5)
#define PPUTLUTRAITS_44   DEC, 0b0000101100u, 5, 6, (2, 2, 11)
#define PPUTLUTRAITS_43   DEC, 0b0000101011u, 5, 6, ()
#define PPUTLUTRAITS_42   DEC, 0b0000101010u, 5, 6, (2, 3, 7)
#define PPUTLUTRAITS_41   DEC, 0b0000101001u, 5, 6, ()
#define PPUTLUTRAITS_40   DEC, 0b0000101000u, 5, 6, (2, 2, 2, 5)
#define PPUTLUTRAITS_39   DEC, 0b0000100111u, 5, 6, (3, 13)
#define PPUTLUTRAITS_38   DEC, 0b0000100110u, 5, 6, (2, 19)
#define PPUTLUTRAITS_37   DEC, 0b0000100101u, 5, 6, ()
#define PPUTLUTRAITS_36   DEC, 0b0000100100u, 5, 6, (2, 2, 3, 3)
#define PPUTLUTRAITS_35   DEC, 0b0000100011u, 5, 5, (5, 7)
#define PPUTLUTRAITS_34   DEC, 0b0000100010u, 5, 5, (2, 17)
#define PPUTLUTRAITS_33   DEC, 0b0000100001u, 5, 5, (3, 11)
#define PPUTLUTRAITS_32   DEC, 0b0000100000u, 5, 5, (2, 2, 2, 2, 2)
#define PPUTLUTRAITS_31   DEC, 0b0000011111u, 4, 5, ()
#define PPUTLUTRAITS_30   DEC, 0b0000011110u, 4, 5, (2, 3, 5)
#define PPUTLUTRAITS_29   DEC, 0b0000011101u, 4, 5, ()
#define PPUTLUTRAITS_28   DEC, 0b0000011100u, 4, 5, (2, 2, 7)
#define PPUTLUTRAITS_27   DEC, 0b0000011011u, 4, 5, (3, 3, 3)
#define PPUTLUTRAITS_26   DEC, 0b0000011010u, 4, 5, (2, 13)
#define PPUTLUTRAITS_25   DEC, 0b0000011001u, 4, 5, (5, 5)
#define PPUTLUTRAITS_24   DEC, 0b0000011000u, 4, 4, (2, 2, 2, 3)
#define PPUTLUTRAITS_23   DEC, 0b0000010111u, 4, 4, ()
#define PPUTLUTRAITS_22   DEC, 0b0000010110u, 4, 4, (2, 11)
#define PPUTLUTRAITS_21   DEC, 0b0000010101u, 4, 4, (3, 7)
#define PPUTLUTRAITS_20   DEC, 0b0000010100u, 4, 4, (2, 2, 5)
#define PPUTLUTRAITS_19   DEC, 0b0000010011u, 4, 4, ()
#define PPUTLUTRAITS_18   DEC, 0b0000010010u, 4, 4, (2, 3, 3)
#define PPUTLUTRAITS_17   DEC, 0b0000010001u, 4, 4, ()
#define PPUTLUTRAITS_16   DEC, 0b0000010000u, 4, 4, (2, 2, 2, 2)
#define PPUTLUTRAITS_15   DEC, 0b0000001111u, 3, 3, (3, 5)
#define PPUTLUTRAITS_14   DEC, 0b0000001110u, 3, 3, (2, 7)
#define PPUTLUTRAITS_13   DEC, 0b0000001101u, 3, 3, ()
#define PPUTLUTRAITS_12   DEC, 0b0000001100u, 3, 3, (2, 2, 3)
#define PPUTLUTRAITS_11   DEC, 0b0000001011u, 3, 3, ()
#define PPUTLUTRAITS_10   DEC, 0b0000001010u, 3, 3, (2, 5)
#define PPUTLUTRAITS_9    DEC, 0b0000001001u, 3, 3, (3, 3)
#define PPUTLUTRAITS_8    DEC, 0b0000001000u, 3, 2, (2, 2, 2)
#define PPUTLUTRAITS_7    DEC, 0b0000000111u, 2, 2, ()
#define PPUTLUTRAITS_6    DEC, 0b0000000110u, 2, 2, (2, 3)
#define PPUTLUTRAITS_5    DEC, 0b0000000101u, 2, 2, ()
#define PPUTLUTRAITS_4    DEC, 0b0000000100u, 2, 2, (2, 2)
#define PPUTLUTRAITS_3    DEC, 0b0000000011u, 1, 1, ()
#define PPUTLUTRAITS_2    DEC, 0b0000000010u, 1, 1, ()
#define PPUTLUTRAITS_1    DEC, 0b0000000001u, 0, 1, ()
#define PPUTLUTRAITS_0    DEC, 0b0000000000u, , 0, ()

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

/// [type.binary]
/// -------------
/// casts a uint to its binary subtype.
///
/// PTL_BINARY(0)             // 0b0000000000u
/// PTL_BINARY(1)             // 0b0000000001u
/// PTL_BINARY(1023)          // 0b1111111111u
/// PTL_BINARY(0b0000000000u) // 0b0000000000u
/// PTL_BINARY(0b0000000001u) // 0b0000000001u
/// PTL_BINARY(0b1111111111u) // 0b1111111111u
#define PTL_BINARY(/* n: uint */...) /* -> binary */ PPUTLBINARY_O(PTL_UINT(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBINARY_O(n)             PPUTLBINARY_OO(n, PTL_CAT(PPUTLUTRAITS_, n))
#define PPUTLBINARY_OO(...)          PPUTLBINARY_OO_X(__VA_ARGS__)
#define PPUTLBINARY_OO_X(n, t, ...)  PTL_CAT(PPUTLBINARY_, t)(n, __VA_ARGS__)
#define PPUTLBINARY_BIN(n, dec, ...) n
#define PPUTLBINARY_DEC(n, bin, ...) bin

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.decimal]
/// --------------
/// casts a uint to its decimal subtype.
///
/// PTL_DECIMAL(0)             // 0
/// PTL_DECIMAL(1)             // 1
/// PTL_DECIMAL(1023)          // 1023
/// PTL_DECIMAL(0b0000000000u) // 0
/// PTL_DECIMAL(0b0000000001u) // 1
/// PTL_DECIMAL(0b1111111111u) // 1023
#define PTL_DECIMAL(/* n: uint */...) /* -> decimal */ PPUTLDECIMAL_O(PTL_UINT(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLDECIMAL_O(n)             PPUTLDECIMAL_OO(n, PTL_CAT(PPUTLUTRAITS_, n))
#define PPUTLDECIMAL_OO(...)          PPUTLDECIMAL_OO_X(__VA_ARGS__)
#define PPUTLDECIMAL_OO_X(n, t, ...)  PTL_CAT(PPUTLDECIMAL_, t)(n, __VA_ARGS__)
#define PPUTLDECIMAL_BIN(n, dec, ...) dec
#define PPUTLDECIMAL_DEC(n, dec, ...) n

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.typeof]
/// -------------
/// detects the value type. will fail if the type is not tuple or uint.
/// literal 0 and 1 are considered decimal rather than bool.
///
/// returns one of:
/// - PTL_TUPLE
/// - PTL_BINARY
/// - PTL_DECIMAL
///
/// PTL_TYPEOF((foo))         // PTL_TUPLE
/// PTL_TYPEOF(0)             // PTL_DECIMAL
/// PTL_TYPEOF(0b1111111111u) // PTL_BINARY
#define PTL_TYPEOF(/* v: tuple|uint */...) /* -> <tuple|binary|decimal ctor> */ \
  PPUTLTYPEOF_O(PTL_EAT __VA_ARGS__)(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLTYPEOF_O(...) PPUTLTYPEOF_OO##__VA_OPT__(_NO)##_TUPLE
#define PPUTLTYPEOF_OO_NO_TUPLE(...) \
  PTL_CAT(PPUTLTYPEOF_OO_NO_TUPLE_, PTL_FIRST(PTL_CAT(PPUTLUTRAITS_, PTL_UINT(__VA_ARGS__))))
#define PPUTLTYPEOF_OO_NO_TUPLE_BIN PTL_BINARY
#define PPUTLTYPEOF_OO_NO_TUPLE_DEC PTL_DECIMAL
#define PPUTLTYPEOF_OO_TUPLE(...)   PTL_TUPLE

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

/// [traits.is_binary]
/// ------------------
/// detects if args is a uint represented as binary.
///
/// PTL_IS_BINARY()              // 0
/// PTL_IS_BINARY(48)            // 0
/// PTL_IS_BINARY(1023)          // 0
/// PTL_IS_BINARY(foo)           // 0
/// PTL_IS_BINARY(0b0000000000u) // 1
/// PTL_IS_BINARY(0b1111111111u) // 1
#define PTL_IS_BINARY(...) /* -> bool */ \
  PTL_CAT(PPUTLIS_BINARY_O_, PTL_IS_UINT(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_BINARY_O_1(u)   PTL_CAT(PPUTLIS_BINARY_O_1_, PTL_FIRST(PTL_CAT(PPUTLUTRAITS_, u)))
#define PPUTLIS_BINARY_O_1_BIN  1
#define PPUTLIS_BINARY_O_1_DEC  0
#define PPUTLIS_BINARY_O_0(...) 0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_decimal]
/// -------------------
/// detects if args is a uint represented as decimal.
///
/// PTL_IS_DECIMAL()              // 0
/// PTL_IS_DECIMAL(48)            // 1
/// PTL_IS_DECIMAL(1023)          // 1
/// PTL_IS_DECIMAL(foo)           // 0
/// PTL_IS_DECIMAL(0b0000000000u) // 0
/// PTL_IS_DECIMAL(0b1111111111u) // 0
#define PTL_IS_DECIMAL(...) /* -> bool */ \
  PTL_CAT(PPUTLIS_DECIMAL_O_, PTL_IS_UINT(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_DECIMAL_O_1(u)   PTL_CAT(PPUTLIS_DECIMAL_O_1_, PTL_FIRST(PTL_CAT(PPUTLUTRAITS_, u)))
#define PPUTLIS_DECIMAL_O_1_BIN  0
#define PPUTLIS_DECIMAL_O_1_DEC  1
#define PPUTLIS_DECIMAL_O_0(...) 0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [bitwise.bitget]
/// ----------------
/// gets the ith bit from the uint.
/// i must be less than PTL_UINT_BITS (10).
///
/// PTL_BITGET(2, 7)             // 0
/// PTL_BITGET(2, 8)             // 1
/// PTL_BITGET(2, 9)             // 0
/// PTL_BITGET(0b1111111110u, 9) // 0
#define PTL_BITGET(/* v: uint, i: uint */...) /* -> v[i]: bool */ \
  PPUTLBITGET_O(                                                  \
      PTL_CAT(PPUTLBITGET_, PTL_DECIMAL(PTL_REST(__VA_ARGS__))),  \
      PTL_FIRST(PTL_REST(PTL_REST(PTL_CAT(PPUTLUTRAITS_, PTL_BINARY(PTL_FIRST(__VA_ARGS__)))))))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBITGET_O(op, bits)                     op bits
#define PPUTLBITGET_9(a, b, c, d, e, f, g, h, i, j) j
#define PPUTLBITGET_8(a, b, c, d, e, f, g, h, i, j) i
#define PPUTLBITGET_7(a, b, c, d, e, f, g, h, i, j) h
#define PPUTLBITGET_6(a, b, c, d, e, f, g, h, i, j) g
#define PPUTLBITGET_5(a, b, c, d, e, f, g, h, i, j) f
#define PPUTLBITGET_4(a, b, c, d, e, f, g, h, i, j) e
#define PPUTLBITGET_3(a, b, c, d, e, f, g, h, i, j) d
#define PPUTLBITGET_2(a, b, c, d, e, f, g, h, i, j) c
#define PPUTLBITGET_1(a, b, c, d, e, f, g, h, i, j) b
#define PPUTLBITGET_0(a, b, c, d, e, f, g, h, i, j) a

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [bitwise.bitset]
/// ----------------
/// sets the ith bit in the uint to b.
/// i must be less than PTL_UINT_BITS (10).
///
/// PTL_BITSET(0, 8, 1)             // 2
/// PTL_BITSET(1, 7, 1)             // 5
/// PTL_BITSET(0b1111111111u, 9, 0) // 0b1111111110u
#define PTL_BITSET(/* v: uint, i: uint, b: bool */...) /* -> (v[i] = b): typeof(v) */ \
  PPUTLBITSET_O(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBITSET_O(v, i, b)                              \
  PTL_TYPEOF(v)                                             \
  (PPUTLBITSET_OO(b, PTL_CAT(PPUTLBITSET_, PTL_DECIMAL(i)), \
                  PTL_FIRST(PTL_REST(PTL_REST(PTL_CAT(PPUTLUTRAITS_, PTL_BINARY(v)))))))
#define PPUTLBITSET_OO(b, op, bits)                    PPUTLBITSET_OO_X(b, op, PTL_ITEMS(bits))
#define PPUTLBITSET_OO_X(b, op, ...)                   op(b, __VA_ARGS__)
#define PPUTLBITSET_9(_, a, B, c, d, e, f, g, h, i, j) 0b##a##B##c##d##e##f##g##h##i##_##u
#define PPUTLBITSET_8(_, a, B, c, d, e, f, g, h, i, j) 0b##a##B##c##d##e##f##g##h##_##j##u
#define PPUTLBITSET_7(_, a, B, c, d, e, f, g, h, i, j) 0b##a##B##c##d##e##f##g##_##i##j##u
#define PPUTLBITSET_6(_, a, B, c, d, e, f, g, h, i, j) 0b##a##B##c##d##e##f##_##h##i##j##u
#define PPUTLBITSET_5(_, a, B, c, d, e, f, g, h, i, j) 0b##a##B##c##d##e##_##g##h##i##j##u
#define PPUTLBITSET_4(_, a, B, c, d, e, f, g, h, i, j) 0b##a##B##c##d##_##f##g##h##i##j##u
#define PPUTLBITSET_3(_, a, B, c, d, e, f, g, h, i, j) 0b##a##B##c##_##e##f##g##h##i##j##u
#define PPUTLBITSET_2(_, a, B, c, d, e, f, g, h, i, j) 0b##a##B##_##d##e##f##g##h##i##j##u
#define PPUTLBITSET_1(_, a, B, c, d, e, f, g, h, i, j) 0b##a##_##c##d##e##f##g##h##i##j##u
#define PPUTLBITSET_0(_, a, B, c, d, e, f, g, h, i, j) 0b##_##B##c##d##e##f##g##h##i##j##u

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [bitwise.bitnot]
/// ----------------
/// bitwise NOT.
/// returns the same uint representation as its input.
///
/// PTL_BITNOT(0)             // 1023
/// PTL_BITNOT(1)             // 1022
/// PTL_BITNOT(0b0000000000u) // 0b1111111111u
/// PTL_BITNOT(0b0000000001u) // 0b1111111110u
#define PTL_BITNOT(/* v: uint */...) /* -> ~v: typeof(v) */ \
  PTL_TYPEOF(__VA_ARGS__)                                   \
  (PTL_REST(PTL_REST(PTL_REST(PTL_CAT(PPUTLUTRAITS_, PTL_BINARY(__VA_ARGS__))))))

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

// vim: fdm=marker:fmr={{{,}}}

#endif

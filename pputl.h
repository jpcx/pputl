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
//    Macro functions are generally not advisable in production code. They    //
//    are difficult to reason about, pollute the global namespace, and can    //
//    hinder debugging and refactoring efforts.  C++ has evolved to enable    //
//    countless metaprogramming techniques that are preferable.               //
//                                                                            //
//    This library is built to provide a strong, safe set of functionality    //
//    for edge cases that uniquely benefit from text replacement and would   //
//    would otherwise utilize a separate code generation script, including    //
//    test case generation and template specialization minimizations.         //
//                                                                            //
//    At its core, pputl is primarily a research project that explores new    //
//    language possibilities  regarding the reduction of terse syntax  and    //
//    the implementation of various forms of reflection.                      //
//                                                                            //
//    ABOUT                                                                   //
//    -----                                                                   //
//                                                                            //
//    pputl  is a powerful, typed C++ preprocessor utilities library  that    //
//    implements many high-level programming constructs,  including 12-bit    //
//    signed and unsigned integers with arithmetic and comparison support.    //
//                                                                            //
//    pputl implements  recursive algorithms  by manipulating preprocessor    //
//    syntax to construct inline call stacks that execute much faster than    //
//    mutually-recursive patterns and can be nested indefinitely.             //
//                                                                            //
//    pputl requires __VA_ARGS__, __VA_OPT__, and empty variadic arguments    //
//    support (which are guaranteed by C++20) but has no dependencies.        //
//                                                                            //
//    pputl is completely generated and tested  by a custom C++ framework.    //
//    See the codegen/ folder for the full source.                            //
//                                                                            //
//    USAGE                                                                   //
//    -----                                                                   //
//    Copy pputl.h and include. The distribution is single-header.            //
//                                                                            //
//    Configuration of the word size and naming scheme  can be achieved by    //
//    modifying the head of codegen/codegen.h and running `make`.             //
//                                                                            //
//    Run `make test` to validate the library on your system.                 //
//                                                                            //
//    TERMINOLOGY                                                             //
//    -----------                                                             //
//                                                                            //
//    pputl defines several types  to describe different kinds of variadic    //
//    arguments  (potentially empty,  comma-delimited tokens of any kind).    //
//    Type identification and conversion is used extensively.                 //
//                                                                            //
//    Each type  is represented by  two functions:  a predicate for traits    //
//    testing, and a constructor that validates, converts, or fails.   All    //
//    functions that use these types in their parameter docs  assert their    //
//    argument sanity by calling the appropriate constructor functions.       //
//                                                                            //
//      none: nothing                                                         //
//      some: <abstract> something; not nothing                               //
//       |- any: exactly one generic value                                    //
//          |- atom: a generic value not surrounded by parentheses            //
//          |   |- bool: a literal '1' or '0'                                 //
//          |   |- nybl: a (4-bit) literal uppercase hex digit [e.g. B]       //
//          |   |- int: <abstract> a word-sized signed integer                //
//          |   |   |- idec: a positive 2s-complement decimal int [e.g. 3]    //
//          |   |   |- ihex: a signed hex integer [e.g. 0x861]                //
//          |   |- uint: <abstract> a word-sized unsigned integer             //
//          |       |- udec: an unsigned decimal integer [e.g. 42u]           //
//          |       |- uhex: an unsigned hex integer [e.g. 0x02Au]            //
//          |- tup: parenthesised items [typedocs: tup, (T...), (T, U), etc.] //
//              |- word: a word-sized tup of nybls [e.g. (6, D, 2)]           //
//                                                                            //
//    FUNDAMENTALS                                                            //
//    ------------                                                            //
//                                                                            //
//    pputl errors execute  an invalid preprocessor operation by using the    //
//    concatenation operator (incorrectly) on a string error message.  All    //
//    errors  triggered by  pputl functions  will include  the macro name,    //
//    a textual description, and its primary expansion arguments.             //
//                                                                            //
//    Non-nullary API functions are fully variadic and chainable such that    //
//    the outputs of one may be used as inputs to another. Parameters must    //
//    be fully expanded and distinguishable after the primary expansion.      //
//                                                                            //
//    Hexadecimal integers are always represented by fixed-length strings.    //
//    Negative ints cannot be represented in decimal  due to concatenation    //
//    restrictions. Arithmetic and bitwise functions attempt to cast their    //
//    results in the same form as their input, but will always return ihex    //
//    when an idec input becomes negative.  Decimal representations can be   ///
//    generated for pasting using fmt.paste.                                ////
//                                                                         /////
///////////////////////////////////////////////////////////////////////////// */

/// [config.build]
/// --------------
/// the build number of this pputl release (ISO8601).
#define PTL_BUILD /* -> <c++ int> */ 20220813

/// [config.word_size]
/// ------------------
/// the number of nybls used to represent pputl integers.
/// hex representations of integers are fixed at this length.
/// see the readme code generation section to configure.
#define PTL_WORD_SIZE /* -> uint */ 3

/// [config.int_min]
/// ----------------
/// the minimum value of a pputl signed int.
/// only representable as hex. see type.int for details.
#define PTL_INT_MIN /* -> binary */ 0x800

/// [config.int_max]
/// ----------------
/// the maximum value of a pputl signed int.
#define PTL_INT_MAX /* -> int */ 2047

/// [config.uint_max]
/// -----------------
/// the maximum value of a pputl unsigned int.
/// upper bound on the number of args for many pputl functions.
#define PTL_UINT_MAX /* -> uint */ 4095u

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

/// [lang.ifirst]
/// -------------
/// immediately returns the first argument.
/// must have at least one argument.
///
/// useful for operating directly on __VA_ARGS__ or
/// for quickly retrieving the first tuple element
/// using an identity function such as PTL_ESC.
///
/// e.g. PTL_IFIRST(__VA_ARGS__)
///      PTL_ESC(PTL_IFIRST tup)
#define PTL_IFIRST(/* first: any, rest: any... */ _, ...) /* -> first */ _

/// [lang.first]
/// ------------
/// returns the first argument.
///
/// PTL_FIRST()     // <nothing>
/// PTL_FIRST(, )   // <nothing>
/// PTL_FIRST(a)    // a
/// PTL_FIRST(a, b) // a
#define PTL_FIRST(/* v: any... */...) /* -> v[0] */ __VA_OPT__(PTL_IFIRST(__VA_ARGS__))

/// [lang.irest]
/// ------------
/// immediately returns all args except for the first.
/// must have at least one argument.
///
/// useful for operating directly on __VA_ARGS__ or
/// for quickly retrieving the rest tuple elements
/// using an identity function such as PTL_ESC.
///
/// e.g. PTL_IREST(__VA_ARGS__)
///      PTL_ESC(PTL_IREST tup)
#define PTL_IREST(/* first: any, rest: any... */ _, ...) /* -> ...rest */ __VA_ARGS__

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
#define PTL_REST(/* v: any... */...) /* -> ...v */ __VA_OPT__(PTL_IREST(__VA_ARGS__))

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

/// [lang.nibble]
/// -------------
/// translates four bits to a hexadecimal digit.
///
/// PTL_NIBBLE(0, 0, 0, 0) // 0
/// PTL_NIBBLE(0, 0, 1, 0) // 2
/// PTL_NIBBLE(1, 0, 0, 1) // 9
/// PTL_NIBBLE(1, 1, 0, 1) // D
/// PTL_NIBBLE(1, 1, 1, 1) // F
#define PTL_NIBBLE(/* b0: bool, b1: bool, b2: bool, b3: bool */...) /* -> <0-F> */ \
  PPUTLNIBBLE_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLNIBBLE_o(b0, b1, b2, b3) PPUTLNIBBLE_##b0##b1##b2##b3
#define PPUTLNIBBLE_1111              F
#define PPUTLNIBBLE_1110              E
#define PPUTLNIBBLE_1101              D
#define PPUTLNIBBLE_1100              C
#define PPUTLNIBBLE_1011              B
#define PPUTLNIBBLE_1010              A
#define PPUTLNIBBLE_1001              9
#define PPUTLNIBBLE_1000              8
#define PPUTLNIBBLE_0111              7
#define PPUTLNIBBLE_0110              6
#define PPUTLNIBBLE_0101              5
#define PPUTLNIBBLE_0100              4
#define PPUTLNIBBLE_0011              3
#define PPUTLNIBBLE_0010              2
#define PPUTLNIBBLE_0001              1
#define PPUTLNIBBLE_0000              0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_none]
/// ----------------
/// detects if args is nothing.
///
/// PTL_IS_NONE()          // 1
/// PTL_IS_NONE(foo)       // 0
/// PTL_IS_NONE(foo, bar)  // 0
/// PTL_IS_NONE(PTL_ESC()) // 1
#define PTL_IS_NONE(/* <unknown> */...) /* -> bool */ PPUTLIS_NONE_0##__VA_OPT__(1)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_NONE_01 0
#define PPUTLIS_NONE_0  1

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_some]
/// ----------------
/// detects if args is something.
///
/// PTL_IS_SOME()          // 0
/// PTL_IS_SOME(foo)       // 1
/// PTL_IS_SOME(foo, bar)  // 1
/// PTL_IS_SOME(PTL_ESC()) // 0
#define PTL_IS_SOME(/* <unknown> */...) /* -> bool */ PPUTLIS_SOME_0##__VA_OPT__(1)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_SOME_01 1
#define PPUTLIS_SOME_0  0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_any]
/// ---------------
/// [extends PTL_IS_SOME] detects if args is exactly one generic value.
///
/// PTL_IS_ANY()         // 0
/// PTL_IS_ANY(,)        // 0
/// PTL_IS_ANY(foo,)     // 0
/// PTL_IS_ANY(foo, bar) // 0
/// PTL_IS_ANY(foo)      // 1
/// PTL_IS_ANY((42))     // 1
#define PTL_IS_ANY(/* <unknown> */...) /* -> bool */ \
  PTL_CAT(PPUTLIS_ANY_, PTL_IS_SOME(__VA_ARGS__))(__VA_ARGS__.)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_ANY_1           PPUTLIS_ANY_o
#define PPUTLIS_ANY_0           PPUTLIS_ANY_0_FAIL
#define PPUTLIS_ANY_0_FAIL(...) 0

/// must be called with an tokens after __VA_ARGS__
#define PPUTLIS_ANY_o(/* <some + token; e.g. __VA_ARGS__.foo> */ _, ...) \
  PTL_CAT(PPUTLIS_ANY_o##__VA_OPT__(_NOT)##_OK, PTL_IS_SOME(_))
#define PPUTLIS_ANY_o_NOT_OK   PPUTLIS_ANY_o_NOT_OK_
#define PPUTLIS_ANY_o_NOT_OK_1 0
#define PPUTLIS_ANY_o_NOT_OK_0 0
#define PPUTLIS_ANY_o_OK       PPUTLIS_ANY_o_OK_
#define PPUTLIS_ANY_o_OK_1     1
#define PPUTLIS_ANY_o_OK_0     0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_atom]
/// ----------------
/// [extends PTL_IS_ANY] detects if args is a generic, non-tuple, singular value.
///
/// PTL_IS_ATOM()       // 0
/// PTL_IS_ATOM(foo)    // 1
/// PTL_IS_ATOM(0)      // 1
/// PTL_IS_ATOM(1, 2)   // 0
/// PTL_IS_ATOM(())     // 0
/// PTL_IS_ATOM((1, 2)) // 0
#define PTL_IS_ATOM(/* <unknown> */...) /* -> bool */ \
  PTL_CAT(PPUTLIS_ATOM_, PTL_IS_ANY(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_ATOM_1           PPUTLIS_ATOM_o
#define PPUTLIS_ATOM_0           PPUTLIS_ATOM_0_FAIL
#define PPUTLIS_ATOM_0_FAIL(...) 0
#define PPUTLIS_ATOM_o(any)      PTL_CAT(PPUTLIS_ATOM_o_, PTL_IS_NONE(PTL_EAT any))
#define PPUTLIS_ATOM_o_1         0
#define PPUTLIS_ATOM_o_0         1

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_bool]
/// ----------------
/// [extends PTL_IS_ATOM] detects if args is a bool (literal '1' or '0').
///
/// PTL_IS_BOOL()      // 0
/// PTL_IS_BOOL(0)     // 1
/// PTL_IS_BOOL(1)     // 1
/// PTL_IS_BOOL(1u)    // 0
/// PTL_IS_BOOL(0x000) // 0
/// PTL_IS_BOOL(0, 1)  // 0
/// PTL_IS_BOOL((0))   // 0
#define PTL_IS_BOOL(/* <unknown> */...) /* -> bool */ \
  PTL_CAT(PPUTLIS_BOOL_, PTL_IS_ATOM(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_BOOL_1           PPUTLIS_BOOL_o
#define PPUTLIS_BOOL_0           PPUTLIS_BOOL_0_FAIL
#define PPUTLIS_BOOL_0_FAIL(...) 0
#define PPUTLIS_BOOL_o(atom)     PTL_IS_NONE(PTL_CAT(PPUTLIS_BOOL_o_, atom))
#define PPUTLIS_BOOL_o_1
#define PPUTLIS_BOOL_o_0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_nybl]
/// ----------------
/// [extends PTL_IS_ATOM] detects if args is a nybl (capital hex digit).
///
/// PTL_IS_NYBL()    // 0
/// PTL_IS_NYBL(0)   // 1
/// PTL_IS_NYBL(Q)   // 0
/// PTL_IS_NYBL(foo) // 0
/// PTL_IS_NYBL(B)   // 1
/// PTL_IS_NYBL(b)   // 0
/// PTL_IS_NYBL(F)   // 1
#define PTL_IS_NYBL(/* <unknown> */...) /* -> bool */ \
  PTL_CAT(PPUTLIS_NYBL_, PTL_IS_ATOM(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_NYBL_1           PPUTLIS_NYBL_o
#define PPUTLIS_NYBL_0           PPUTLIS_NYBL_0_FAIL
#define PPUTLIS_NYBL_0_FAIL(...) 0
#define PPUTLIS_NYBL_o(atom)     PPUTLIMPL_NYBL_TRAIT(atom, IS)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_int]
/// ---------------
/// [extends PTL_IS_ATOM] detects if args is a signed integer.
/// hex length is fixed at PTL_WORD_SIZE (3).
/// decimals must be smaller than PTL_INT_MAX (2047).
/// negative values must be hex; concatenation with '-' is not supported.
///
/// PTL_IS_INT()       // 0
/// PTL_IS_INT(foo)    // 0
/// PTL_IS_INT(0)      // 1
/// PTL_IS_INT(0u)     // 0
/// PTL_IS_INT(4095)   // 0
/// PTL_IS_INT(0x000u) // 0
/// PTL_IS_INT(0xFFF)  // 1
/// PTL_IS_INT(0b110u) // 0
/// PTL_IS_INT((), ()) // 0
#define PTL_IS_INT(/* <unknown> */...) /* -> bool */ \
  PTL_CAT(PPUTLIS_INT_, PTL_IS_ATOM(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_INT_1             PPUTLIS_INT_o
#define PPUTLIS_INT_0             PPUTLIS_INT_0_FAIL
#define PPUTLIS_INT_0_FAIL(...)   0
#define PPUTLIS_INT_o(atom)       PTL_CAT(PPUTLIS_INT_o_, PPUTLIMPL_UINT_TRAIT(atom##u, IS))(atom)(atom)
#define PPUTLIS_INT_o_1(i)        PTL_CAT(PPUTLIS_INT_o_1_, PPUTLIMPL_UINT_TRAIT(i##u, TYPE))
#define PPUTLIS_INT_o_1_DEC(i)    PTL_CAT(PPUTLIS_INT_o_1_DEC_, PPUTLIMPL_UINT_TRAIT(i##u, DINEG))
#define PPUTLIS_INT_o_1_DEC_1     0
#define PPUTLIS_INT_o_1_DEC_0     1
#define PPUTLIS_INT_o_1_HEX(i)    1
#define PPUTLIS_INT_o_0(...)      PPUTLIS_INT_o_0_FAIL
#define PPUTLIS_INT_o_0_FAIL(...) 0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_idec]
/// ----------------
/// [extends PTL_IS_INT] detects if args is a signed int in decimal form.
/// decimals must be smaller than PTL_INT_MAX (2047).
/// negative values must be hex; concatenation with '-' is not supported.
///
/// PTL_IS_IDEC(1)      // 1
/// PTL_IS_IDEC(1u)     // 0
/// PTL_IS_IDEC(2047)   // 1
/// PTL_IS_IDEC(4095)   // 0
/// PTL_IS_IDEC(0x000u) // 0
/// PTL_IS_IDEC(0xFFF)  // 0
/// PTL_IS_IDEC((), ()) // 0
#define PTL_IS_IDEC(/* <unknown> */...) /* -> bool */ \
  PTL_CAT(PPUTLIS_IDEC_, PTL_IS_INT(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_IDEC_1           PPUTLIS_IDEC_o
#define PPUTLIS_IDEC_0           PPUTLIS_IDEC_0_FAIL
#define PPUTLIS_IDEC_0_FAIL(...) 0
#define PPUTLIS_IDEC_o(int) \
  PTL_IS_NONE(PTL_CAT(PPUTLIS_IDEC_o_, PPUTLIMPL_UINT_TRAIT(int##u, TYPE)))
#define PPUTLIS_IDEC_o_DEC

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_ihex]
/// ----------------
/// [extends PTL_IS_INT] detects if args is a signed int in hex form.
/// hex length is fixed at PTL_WORD_SIZE (3).
///
/// PTL_IS_IHEX(1)      // 0
/// PTL_IS_IHEX(1u)     // 0
/// PTL_IS_IHEX(0x000)  // 1
/// PTL_IS_IHEX(0xFFF)  // 1
/// PTL_IS_IHEX(0xFFFu) // 0
/// PTL_IS_IHEX((), ()) // 0
#define PTL_IS_IHEX(/* <unknown> */...) /* -> bool */ \
  PTL_CAT(PPUTLIS_IHEX_, PTL_IS_INT(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_IHEX_1           PPUTLIS_IHEX_o
#define PPUTLIS_IHEX_0           PPUTLIS_IHEX_0_FAIL
#define PPUTLIS_IHEX_0_FAIL(...) 0
#define PPUTLIS_IHEX_o(int) \
  PTL_IS_NONE(PTL_CAT(PPUTLIS_IHEX_o_, PPUTLIMPL_UINT_TRAIT(int##u, TYPE)))
#define PPUTLIS_IHEX_o_HEX

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_uint]
/// ----------------
/// [extends PTL_IS_ATOM] detects if args is an unsigned integer.
/// hex length is fixed at PTL_WORD_SIZE (3).
///
/// PTL_IS_UINT()       // 0
/// PTL_IS_UINT(foo)    // 0
/// PTL_IS_UINT(0)      // 0
/// PTL_IS_UINT(0u)     // 1
/// PTL_IS_UINT(4095)   // 0
/// PTL_IS_UINT(4095u)  // 1
/// PTL_IS_UINT(0x000u) // 1
/// PTL_IS_UINT(0xFFF)  // 0
/// PTL_IS_UINT(0b110u) // 0
/// PTL_IS_UINT((), ()) // 0
#define PTL_IS_UINT(/* <unknown> */...) /* -> bool */ \
  PTL_CAT(PPUTLIS_UINT_, PTL_IS_ATOM(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_UINT_1           PPUTLIS_UINT_o
#define PPUTLIS_UINT_0           PPUTLIS_UINT_0_FAIL
#define PPUTLIS_UINT_0_FAIL(...) 0
#define PPUTLIS_UINT_o(atom)     PPUTLIMPL_UINT_TRAIT(atom, IS)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_udec]
/// ----------------
/// [extends PTL_IS_UINT] detects if args is an unsigned int in deicmal form (requires 'u' suffix).
///
/// PTL_IS_UDEC(1)      // 0
/// PTL_IS_UDEC(1u)     // 1
/// PTL_IS_UDEC(4095)   // 0
/// PTL_IS_UDEC(4095u)  // 1
/// PTL_IS_UDEC(0x000u) // 0
/// PTL_IS_UDEC(0xFFF)  // 0
/// PTL_IS_UDEC((), ()) // 0
#define PTL_IS_UDEC(/* <unknown> */...) /* -> bool */ \
  PTL_CAT(PPUTLIS_UDEC_, PTL_IS_UINT(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_UDEC_1           PPUTLIS_UDEC_o
#define PPUTLIS_UDEC_0           PPUTLIS_UDEC_0_FAIL
#define PPUTLIS_UDEC_0_FAIL(...) 0
#define PPUTLIS_UDEC_o(uint)     PTL_IS_NONE(PTL_CAT(PPUTLIS_UDEC_o_, PPUTLIMPL_UINT_TRAIT(uint, TYPE)))
#define PPUTLIS_UDEC_o_DEC

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_uhex]
/// ----------------
/// [extends PTL_IS_UINT] detects if args is an unsigned int in hex form (requires 'u' suffix).
/// hex length is fixed at PTL_WORD_SIZE (3).
///
/// PTL_IS_UHEX(1)      // 0
/// PTL_IS_UHEX(1u)     // 0
/// PTL_IS_UHEX(0x000u) // 1
/// PTL_IS_UHEX(0xFFF)  // 0
/// PTL_IS_UHEX((), ()) // 0
#define PTL_IS_UHEX(/* <unknown> */...) /* -> bool */ \
  PTL_CAT(PPUTLIS_UHEX_, PTL_IS_UINT(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_UHEX_1           PPUTLIS_UHEX_o
#define PPUTLIS_UHEX_0           PPUTLIS_UHEX_0_FAIL
#define PPUTLIS_UHEX_0_FAIL(...) 0
#define PPUTLIS_UHEX_o(uint)     PTL_IS_NONE(PTL_CAT(PPUTLIS_UHEX_o_, PPUTLIMPL_UINT_TRAIT(uint, TYPE)))
#define PPUTLIS_UHEX_o_HEX

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_tup]
/// ---------------
/// [extends PTL_IS_ANY] detects if args is a tuple (any...).
///
/// PTL_IS_TUP()       // 0
/// PTL_IS_TUP(1, 2)   // 0
/// PTL_IS_TUP(())     // 1
/// PTL_IS_TUP((1, 2)) // 1
#define PTL_IS_TUP(/* <unknown> */...) /* -> bool */ \
  PTL_CAT(PPUTLIS_TUP_, PTL_IS_ANY(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_TUP_1           PPUTLIS_TUP_o
#define PPUTLIS_TUP_0           PPUTLIS_TUP_0_FAIL
#define PPUTLIS_TUP_0_FAIL(...) 0
#define PPUTLIS_TUP_o(any)      PTL_IS_NONE(PTL_EAT any)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_word]
/// ----------------
/// [extends PTL_IS_TUP] detects if args a tup of exactly PTL_WORD_SIZE (3) nybls.
///
/// PTL_IS_WORD()             // 0
/// PTL_IS_WORD(foo)          // 0
/// PTL_IS_WORD(0)            // 0
/// PTL_IS_WORD(9, B, C)      // 0
/// PTL_IS_WORD((9, B, C))    // 1
/// PTL_IS_WORD((9, B, C,))   // 0
/// PTL_IS_WORD((9, B, C, E)) // 0
#define PTL_IS_WORD(/* <unknown> */...) /* -> bool */ \
  PTL_CAT(PPUTLIS_WORD_, PTL_IS_TUP(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_WORD_1                 PPUTLIS_WORD_o
#define PPUTLIS_WORD_0                 PPUTLIS_WORD_0_FAIL
#define PPUTLIS_WORD_0_FAIL(...)       0
#define PPUTLIS_WORD_o(tup)            PPUTLIS_WORD_RES(PPUTLIS_WORD_R(PPUTLIS_WORD_R((), PTL_ESC tup)))
#define PPUTLIS_WORD_R(...)            PPUTLIS_WORD_R_o(__VA_ARGS__)
#define PPUTLIS_WORD_R_o(ctup, _, ...) (PTL_IS_NYBL(_), PTL_ESC ctup), __VA_ARGS__
#define PPUTLIS_WORD_RES(...)          PPUTLIS_WORD_RES_o(__VA_ARGS__)
#define PPUTLIS_WORD_RES_o(ctup, ...) \
  PPUTLIS_WORD_RES_oo(PTL_IS_NONE(PPUTLIS_WORD_CHK ctup), PTL_IS_NYBL(__VA_ARGS__))
#define PPUTLIS_WORD_RES_oo(...)    PPUTLIS_WORD_RES_ooo(__VA_ARGS__)
#define PPUTLIS_WORD_RES_ooo(c, n)  PPUTLIS_WORD_##c##n
#define PPUTLIS_WORD_CHK(a, b, ...) PPUTLIS_WORD_CHK_##a##b
#define PPUTLIS_WORD_CHK_11
#define PPUTLIS_WORD_11 1
#define PPUTLIS_WORD_10 0
#define PPUTLIS_WORD_01 0
#define PPUTLIS_WORD_00 0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.none]
/// -----------
/// nothing. fails if something.
///
/// PTL_NONE() // <nothing>
#define PTL_NONE(/* <nothing> */...) /* -> none */ \
  PTL_CAT(PPUTLNONE_, PTL_IS_NONE(__VA_ARGS__))    \
  (PTL_ISTR([PTL_NONE] none cannot describe something : __VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLNONE_1(e)
#define PPUTLNONE_0(e) PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.some]
/// -----------
/// something. fails if nothing.
///
/// PTL_SOME(foo)             // foo
/// PTL_SOME(foo, bar)        // foo, bar
/// PTL_SOME(foo, 42, (, , )) // foo, 42, (, , )
/// PTL_SOME(, )              // ,
#define PTL_SOME(/* args: <something>... */...) /* -> some{...args} */ \
  PTL_CAT(PPUTLSOME_, PTL_IS_SOME(__VA_ARGS__))                        \
  (PTL_ISTR([PTL_SOME] some cannot describe nothing : __VA_ARGS__), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLSOME_1(e, ...) __VA_ARGS__
#define PPUTLSOME_0(e, ...) PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.any]
/// ----------
/// [inherits from PTL_SOME] any type (generic data). returns arg.
/// describes exactly one generic value.
///
/// fails if not exactly one arg.
///
/// PTL_ANY(foo) // foo
#define PTL_ANY(/* v: any */...) /* -> any{v} */                  \
  PPUTLANY_o(PTL_ISTR([PTL_ANY] any cannot describe multiple args \
                      : __VA_ARGS__),                             \
             PTL_SOME(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLANY_o(e, ...) PTL_CAT(PPUTLANY_, PPUTLIS_ANY_o(__VA_ARGS__.))(e, __VA_ARGS__)
#define PPUTLANY_1(e, any) any
#define PPUTLANY_0(e, ...) PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.atom]
/// -----------
/// [inherits from PTL_ANY] a generic, non-tuple, singular value.
///
/// PTL_ATOM(foo) // foo
#define PTL_ATOM(/* v: any */...) /* -> v */ \
  PPUTLATOM_o(PTL_ISTR([PTL_ATOM] atom cannot describe tuples : __VA_ARGS__), PTL_ANY(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLATOM_o(e, any)  PTL_CAT(PPUTLATOM_, PPUTLIS_ATOM_o(any))(e, any)
#define PPUTLATOM_1(e, atom) atom
#define PPUTLATOM_0(e, ...)  PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.bool]
/// -----------
/// [inherits from PTL_ATOM] bool type (0 or 1).
/// expands to b if valid, else fails.
///
/// PTL_BOOL(0) // 0
/// PTL_BOOL(1) // 1
#define PTL_BOOL(/* b: 0|1 */...) /* -> b */                                                \
  PPUTLBOOL_o(PTL_ISTR([PTL_BOOL] bool cannot describe anything but the literal '1' and '0' \
                       : __VA_ARGS__),                                                      \
              PTL_ATOM(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBOOL_o(e, atom) PTL_CAT(PPUTLBOOL_, PPUTLIS_BOOL_o(atom))(e, atom)
#define PPUTLBOOL_1(e, bool) bool
#define PPUTLBOOL_0(e, ...)  PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.nybl]
/// -----------
/// [inherits from PTL_ATOM] nybl type (capital hex digit).
/// expands to n if valid, else fails.
///
/// PTL_NYBL(0) // 0
/// PTL_NYBL(1) // 1
/// PTL_NYBL(B) // B
/// PTL_NYBL(F) // F
#define PTL_NYBL(/* n: 0-F */...) /* -> nybl{n}: <0-F> */                                     \
  PPUTLNYBL_o(                                                                                \
      PTL_ISTR([PTL_NYBL] nybl cannot describe anything but literal, capital hex digits 0 - F \
               : __VA_ARGS__),                                                                \
      PTL_ATOM(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLNYBL_o(e, atom) PTL_CAT(PPUTLNYBL_, PPUTLIS_NYBL_o(atom))(e, atom)
#define PPUTLNYBL_1(e, nybl) nybl
#define PPUTLNYBL_0(e, ...)  PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.int]
/// ----------
/// [inherits from PTL_ATOM] 12-bit signed integer type.
/// constructible from any integer or word. word construction returns ihex.
/// cannot parse negative decimals; use math.neg instead.
///
/// hex length is fixed. cannot parse shorter hex lengths.
///
/// attempts to preserve hex/decimal representation, but will
/// output hex if casting the input yields a negative number
///
/// cast from unsigned reinterprets bits as signed two's complement.
///
/// value must be a valid signed int; implicit interpretation
/// as unsigned is not allowed (e.g. 4095 is not a valid integer).
///
/// PTL_INT(0)         // 0
/// PTL_INT(1u)        // 1
/// PTL_INT(0x000)     // 0x000
/// PTL_INT(2047)      // 2047
/// PTL_INT(0xFFFu)    // 0xFFF
/// PTL_INT(4095u)     // 0xFFF
/// PTL_INT((8, 0, 0)) // 0x800
/// PTL_INT((7, F, F)) // 0x7FF
#define PTL_INT(/* n: int|uint|word */...) /* -> int{n} */ \
  PPUTLINT_o(PTL_ISTR([PTL_INT] invalid integer : __VA_ARGS__), PTL_SOME(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLINT_o(e, ...)      PTL_CAT(PPUTLINT_, PPUTLIS_WORD_o(__VA_ARGS__))(e, __VA_ARGS__)
#define PPUTLINT_1(e, word)     PTL_ESC(PPUTLINT_1_CAT word)
#define PPUTLINT_1_CAT(a, b, c) 0x##a##b##c
#define PPUTLINT_0(e, ...)      PPUTLINT_0_o(e, PTL_ATOM(__VA_ARGS__))
#define PPUTLINT_0_o(e, atom) \
  PTL_CAT(PPUTLINT_o_, PTL_CAT(PPUTLIS_INT_o(atom), PPUTLIS_UINT_o(atom)))(e, atom)
#define PPUTLINT_o_00(e, ...) PTL_FAIL(e)

/// cast from int
#define PPUTLINT_o_10(e, i) i

/// cast from uint
#define PPUTLINT_o_01(e, uint) PTL_CAT(PPUTLINT_, PPUTLIMPL_UINT_TRAIT(uint, TYPE))(uint)
#define PPUTLINT_DEC(uint)     PTL_CAT(PPUTLINT_DEC_, PPUTLIMPL_UINT_TRAIT(uint, DINEG))(uint)
#define PPUTLINT_DEC_1(uint)   PPUTLIMPL_UINT_TRAIT(uint, DIHEX)
#define PPUTLINT_DEC_0(uint) \
  PPUTLIMPL_UINT_TRAIT(PTL_CAT(PPUTLIMPL_UINT_TRAIT(uint, DIHEX), u), HIDEC)
#define PPUTLINT_HEX(uint) PPUTLIMPL_UINT_TRAIT(PPUTLIMPL_UINT_TRAIT(uint, HUDEC), DIHEX)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.idec]
/// -----------
/// [inherits from PTL_INT] casts to the signed int decimal subtype.
/// positive values only! fails on negative ints.
///
/// use fmt.paste with ihex to get negative decimals.
///
/// PTL_IDEC(0x000)  // 0
/// PTL_IDEC(0x001)  // 1
/// PTL_IDEC(0x005u) // 5
/// PTL_IDEC(0x7FF)  // 2047
/// PTL_IDEC(2047)   // 2047
#define PTL_IDEC(/* n: int|uint|word */...) /* -> idec{n} */          \
  PPUTLIDEC_o(PTL_ISTR([PTL_IDEC] cannot represent negative in base10 \
                       : __VA_ARGS__),                                \
              PTL_INT(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIDEC_o(e, int)  PTL_CAT(PPUTLIDEC_, PPUTLIS_IDEC_o(int))(e, int)
#define PPUTLIDEC_1(e, idec) idec
#define PPUTLIDEC_0(e, ihex) PPUTLIDEC_0_o(e, ihex##u)
#define PPUTLIDEC_0_o(e, uhex) \
  PTL_CAT(PPUTLIDEC_0_o_, PPUTLIMPL_UINT_TRAIT(PPUTLIMPL_UINT_TRAIT(uhex, HUDEC), DINEG))(e, uhex)
#define PPUTLIDEC_0_o_1(e, uhex) PTL_FAIL(e)
#define PPUTLIDEC_0_o_0(e, uhex) PPUTLIMPL_UINT_TRAIT(uhex, HIDEC)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.ihex]
/// -----------
/// [inherits from PTL_INT] casts to the signed int binary subtype.
///
/// PTL_IHEX(0)     // 0x000
/// PTL_IHEX(1)     // 0x001
/// PTL_IHEX(5)     // 0x005
/// PTL_IHEX(4095u) // 0xFFF
/// PTL_IHEX(2047u) // 0x7FF
#define PTL_IHEX(/* n: int|uint|word */...) /* -> ihex{n} */ PPUTLIHEX_o(PTL_INT(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIHEX_o(int)    PTL_CAT(PPUTLIHEX_, PPUTLIS_IHEX_o(int))(int)
#define PPUTLIHEX_1(ihex)   ihex
#define PPUTLIHEX_0(idec)   PPUTLIHEX_0_o(idec##u)
#define PPUTLIHEX_0_o(udec) PPUTLIMPL_UINT_TRAIT(udec, DIHEX)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.uint]
/// -----------
/// [inherits from PTL_ATOM] 12-bit unsigned integer type.
/// constructible from any integer or word. word construction returns uhex.
/// cannot parse negative decimals; use math.neg instead.
///
/// hex length is fixed. cannot parse shorter hex lengths.
///
/// preserves hex/decimal representation.
///
/// uses a 'u' suffix for both representations.
/// see fmt.paste_uint to remove suffix before pasting.
///
/// cast from signed reinterprets bits as unsigned.
/// value must be a valid signed int; implicit interpretation
/// as unsigned is not allowed (e.g. 4095 is not a valid integer).
///
/// PTL_UINT(0)         // 0u
/// PTL_UINT(1)         // 1u
/// PTL_UINT(2u)        // 2u
/// PTL_UINT(4095u)     // 4095u
/// PTL_UINT(0x000u)    // 0x000u
/// PTL_UINT(0xFFF)     // 0xFFFu
/// PTL_UINT((0, 0, 0)) // 0x000u
/// PTL_UINT((F, F, F)) // 0xFFFu
#define PTL_UINT(/* n: int|uint|word */...) /* -> uint{n} */ \
  PPUTLUINT_o(PTL_ISTR([PTL_UINT] invalid integer : __VA_ARGS__), PTL_SOME(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLUINT_o(e, ...)      PTL_CAT(PPUTLUINT_, PPUTLIS_WORD_o(__VA_ARGS__))(e, __VA_ARGS__)
#define PPUTLUINT_1(e, word)     PTL_ESC(PPUTLUINT_1_CAT word)
#define PPUTLUINT_1_CAT(a, b, c) 0x##a##b##c##u
#define PPUTLUINT_0(e, ...)      PPUTLUINT_0_o(e, PTL_ATOM(__VA_ARGS__))
#define PPUTLUINT_0_o(e, atom) \
  PTL_CAT(PPUTLUINT_, PTL_CAT(PPUTLIS_UINT_o(atom), PPUTLIS_INT_o(atom)))(e, atom)
#define PPUTLUINT_10(e, u)   u
#define PPUTLUINT_01(e, i)   i##u
#define PPUTLUINT_00(e, ...) PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.udec]
/// -----------
/// [inherits from PTL_UINT] casts to the unsigned int decimal subtype.
///
/// PTL_UDEC(0x000u) // 0u
/// PTL_UDEC(1)      // 1u
/// PTL_UDEC(5)      // 5u
/// PTL_UDEC(0x005u) // 5u
/// PTL_UDEC(0xFFFu) // 4095u
/// PTL_UDEC(0xFFF)  // 4095u
#define PTL_UDEC(/* n: int|uint|word */...) /* -> udec{n} */ PPUTLUDEC_o(PTL_UINT(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLUDEC_o(uint) PTL_CAT(PPUTLUDEC_, PPUTLIS_UDEC_o(uint))(uint)
#define PPUTLUDEC_1(udec) udec
#define PPUTLUDEC_0(uhex) PPUTLIMPL_UINT_TRAIT(uhex, HUDEC)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.uhex]
/// -----------
/// [inherits from PTL_UINT] casts to the unsigned int hexadecimal subtype.
///
/// PTL_UHEX(0)      // 0x000u
/// PTL_UHEX(1)      // 0x001u
/// PTL_UHEX(5)      // 0x005u
/// PTL_UHEX(4095u)  // 0xFFFu
/// PTL_UHEX(0x000u) // 0x000u
/// PTL_UHEX(0x001u) // 0x001u
/// PTL_UHEX(0xFFF)  // 0xFFFu
#define PTL_UHEX(/* n: int|uint|word */...) /* -> uhex{n} */ PPUTLUHEX_o(PTL_UINT(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLUHEX_o(uint) PTL_CAT(PPUTLUHEX_, PPUTLIS_UHEX_o(uint))(uint)
#define PPUTLUHEX_1(uhex) uhex
#define PPUTLUHEX_0(udec) PTL_CAT(PPUTLIMPL_UINT_TRAIT(udec, DIHEX), u)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.tup]
/// ----------
/// [inherits from PTL_ANY] tuple type (any...).
/// expands to t if valid, else fails.
///
/// PTL_TUP(())     // ()
/// PTL_TUP((1, 2)) // (1, 2)
#define PTL_TUP(/* t: tup */...) /* -> t */                          \
  PPUTLTUP_o(PTL_ISTR([PTL_TUP] tuple must be wrapped in parentheses \
                      : __VA_ARGS__),                                \
             PTL_ANY(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLTUP_o(e, any) PTL_CAT(PPUTLTUP_, PPUTLIS_TUP_o(any))(e, any)
#define PPUTLTUP_1(e, tup) tup
#define PPUTLTUP_0(e, ...) PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.word]
/// -----------
/// [inherits from PTL_SOME] a tuple of exactly PTL_WORD_SIZE (3) nybls.
///
/// fails if args is not a word, an int, or a uint.
///
/// PTL_WORD(0)         // (0, 0, 0)
/// PTL_WORD(4095u)     // (F, F, F)
/// PTL_WORD(0x800)     // (8, 0, 0)
/// PTL_WORD(2047)      // (7, F, F)
/// PTL_WORD((1, 0, 0)) // (1, 0, 0)
#define PTL_WORD(/* n: int|uint|word */...) /* -> word{n} */ \
  PPUTLWORD_o(PTL_ISTR([PTL_WORD] invalid integer or word : __VA_ARGS__), PTL_SOME(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLWORD_o(e, ...)     PTL_CAT(PPUTLWORD_, PPUTLIS_WORD_o(__VA_ARGS__))(e, __VA_ARGS__)
#define PPUTLWORD_1(e, ...)     __VA_ARGS__
#define PPUTLWORD_0(e, ...)     PTL_CAT(PPUTLWORD_0, PTL_IS_ATOM(__VA_ARGS__))(e, __VA_ARGS__)
#define PPUTLWORD_01(e, atom)   PTL_CAT(PPUTLWORD_01, PPUTLIS_INT_o(atom))(e, atom)
#define PPUTLWORD_011(e, int)   PPUTLIMPL_UINT_TRAIT(PTL_UHEX(int), HWORD)
#define PPUTLWORD_010(e, atom)  PTL_CAT(PPUTLWORD_010, PPUTLIS_UINT_o(atom))(e, atom)
#define PPUTLWORD_0101(e, uint) PPUTLIMPL_UINT_TRAIT(PTL_UHEX(uint), HWORD)
#define PPUTLWORD_0100(e, atom) PTL_FAIL(e)
#define PPUTLWORD_00(e, ...)    PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.typeof]
/// -------------
/// detects the value type. must be compatible with the ## operator.
/// literal 0 through 9 are considered ibase10 rather than bool or nybl.
///
/// returns one of:
/// - PTL_NONE
/// - PTL_SOME
/// - PTL_WORD
/// - PTL_TUP
/// - PTL_IDEC
/// - PTL_IHEX
/// - PTL_UDEC
/// - PTL_UHEX
/// - PTL_NYBL
/// - PTL_ATOM
///
/// PTL_TYPEOF((foo))     // PTL_TUP
/// PTL_TYPEOF(0)         // PTL_IDEC
/// PTL_TYPEOF(0u)        // PTL_UDEC
/// PTL_TYPEOF(D)         // PTL_NYBL
/// PTL_TYPEOF(4095)      // PTL_ATOM
/// PTL_TYPEOF(4095u)     // PTL_UDEC
/// PTL_TYPEOF(0xFFF)     // PTL_IHEX
/// PTL_TYPEOF(0xFFFu)    // PTL_UHEX
/// PTL_TYPEOF(foo)       // PTL_ATOM
/// PTL_TYPEOF(foo, bar)  // PTL_SOME
/// PTL_TYPEOF((A))       // PTL_TUP
/// PTL_TYPEOF((0, 0, 0)) // PTL_WORD
/// PTL_TYPEOF((F, F, F)) // PTL_WORD
/// PTL_TYPEOF()          // PTL_NONE
#define PTL_TYPEOF(                                                                     \
    /* <unknown> */...) /* -> ctor<none|some|word|tup|idec|ihex|udec|uhex|nybl|atom> */ \
  PTL_CAT(PPUTLTYPEOF_, PTL_IS_NONE(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

/// none
#define PPUTLTYPEOF_1(...)       PTL_NONE
/// !none
#define PPUTLTYPEOF_0(...)       PTL_CAT(PPUTLTYPEOF_0, PPUTLIS_ANY_o(__VA_ARGS__.))(__VA_ARGS__)
/// !none → any
#define PPUTLTYPEOF_01(any)      PTL_CAT(PPUTLTYPEOF_01, PPUTLIS_TUP_o(any))(any)
/// !none → any → tup
#define PPUTLTYPEOF_011(tup)     PTL_CAT(PPUTLTYPEOF_011, PPUTLIS_WORD_o(tup))(tup)
/// !none → any → tup → word
#define PPUTLTYPEOF_0111(word)   PTL_WORD
/// !none → any → tup → !word
#define PPUTLTYPEOF_0110(tup)    PTL_TUP
/// !none → any → !tup
#define PPUTLTYPEOF_010(atom)    PTL_CAT(PPUTLTYPEOF_010, PPUTLIS_INT_o(atom))(atom)
/// !none → any → !tup → int
#define PPUTLTYPEOF_0101(int)    PTL_CAT(PPUTLTYPEOF_0101, PPUTLIS_IDEC_o(int))(int)
/// !none → any → !tup → int → idec
#define PPUTLTYPEOF_01011(idec)  PTL_IDEC
/// !none → any → !tup → int → !idec
#define PPUTLTYPEOF_01010(ihex)  PTL_IHEX
/// !none → any → !tup → !int
#define PPUTLTYPEOF_0100(atom)   PTL_CAT(PPUTLTYPEOF_0100, PPUTLIS_UINT_o(atom))(atom)
/// !none → any → !tup → !int → uint
#define PPUTLTYPEOF_01001(uint)  PTL_CAT(PPUTLTYPEOF_01001, PPUTLIS_UDEC_o(uint))(uint)
/// !none → any → !tup → !int → uint → udec
#define PPUTLTYPEOF_010011(udec) PTL_UDEC
/// !none → any → !tup → !int → uint → !udec
#define PPUTLTYPEOF_010010(uhex) PTL_UHEX
/// !none → any → !tup → !int → !uint
#define PPUTLTYPEOF_01000(atom)  PTL_CAT(PPUTLTYPEOF_01000, PPUTLIS_NYBL_o(atom))(atom)
/// !none → any → !tup → !int → !uint → nybl
#define PPUTLTYPEOF_010001(nybl) PTL_NYBL
/// !none → any → !tup → !int → !uint → !nybl
#define PPUTLTYPEOF_010000(atom) PTL_ATOM
/// !none → !any
#define PPUTLTYPEOF_00(...)      PTL_SOME

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [logic.not]
/// -----------
/// logical NOT.
///
/// PTL_NOT(0) // 1
/// PTL_NOT(1) // 0
#define PTL_NOT(/* v: bool */...) /* -> bool{!v} */ PTL_CAT(PPUTLNOT_, PTL_BOOL(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLNOT_1 0
#define PPUTLNOT_0 1

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [logic.and]
/// -----------
/// logical AND.
///
/// PTL_AND(0, 0) // 0
/// PTL_AND(0, 1) // 0
/// PTL_AND(1, 0) // 0
/// PTL_AND(1, 1) // 1
#define PTL_AND(/* a: bool, b: bool */...) /* -> bool{a and b} */ PPUTLAND_X(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLAND_X(a, b) PTL_CAT(PPUTLAND_, PTL_CAT(PTL_BOOL(a), PTL_BOOL(b)))
#define PPUTLAND_11      1
#define PPUTLAND_10      0
#define PPUTLAND_01      0
#define PPUTLAND_00      0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [logic.or]
/// ----------
/// logical OR.
///
/// PTL_OR(0, 0) // 0
/// PTL_OR(0, 1) // 1
/// PTL_OR(1, 0) // 1
/// PTL_OR(1, 1) // 1
#define PTL_OR(/* a: bool, b: bool */...) /* -> bool{a or b} */ PPUTLOR_X(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLOR_X(a, b) PTL_CAT(PPUTLOR_, PTL_CAT(PTL_BOOL(a), PTL_BOOL(b)))
#define PPUTLOR_11      1
#define PPUTLOR_10      1
#define PPUTLOR_01      1
#define PPUTLOR_00      0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [logic.nand]
/// ------------
/// logical NAND.
///
/// PTL_NAND(0, 0) // 1
/// PTL_NAND(0, 1) // 1
/// PTL_NAND(1, 0) // 1
/// PTL_NAND(1, 1) // 0
#define PTL_NAND(/* a: bool, b: bool */...) /* -> bool{!(a and b)} */ PPUTLNAND_X(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLNAND_X(a, b) PTL_CAT(PPUTLNAND_, PTL_CAT(PTL_BOOL(a), PTL_BOOL(b)))
#define PPUTLNAND_11      0
#define PPUTLNAND_10      1
#define PPUTLNAND_01      1
#define PPUTLNAND_00      1

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [logic.nor]
/// -----------
/// logical NOR.
///
/// PTL_NOR(0, 0) // 1
/// PTL_NOR(0, 1) // 0
/// PTL_NOR(1, 0) // 0
/// PTL_NOR(1, 1) // 0
#define PTL_NOR(/* a: bool, b: bool */...) /* -> bool{!(a or b)} */ PPUTLNOR_X(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLNOR_X(a, b) PTL_CAT(PPUTLNOR_, PTL_CAT(PTL_BOOL(a), PTL_BOOL(b)))
#define PPUTLNOR_11      0
#define PPUTLNOR_10      0
#define PPUTLNOR_01      0
#define PPUTLNOR_00      1

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [logic.xor]
/// -----------
/// logical XOR.
///
/// PTL_XOR(0, 0) // 0
/// PTL_XOR(0, 1) // 1
/// PTL_XOR(1, 0) // 1
/// PTL_XOR(1, 1) // 0
#define PTL_XOR(/* a: bool, b: bool */...) /* -> bool{a xor b} */ PPUTLXOR_X(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLXOR_X(a, b) PTL_CAT(PPUTLXOR_, PTL_CAT(PTL_BOOL(a), PTL_BOOL(b)))
#define PPUTLXOR_11      0
#define PPUTLXOR_10      1
#define PPUTLXOR_01      1
#define PPUTLXOR_00      0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [logic.xnor]
/// ------------
/// logical XNOR.
///
/// PTL_XNOR(0, 0) // 1
/// PTL_XNOR(0, 1) // 0
/// PTL_XNOR(1, 0) // 0
/// PTL_XNOR(1, 1) // 1
#define PTL_XNOR(/* a: bool, b: bool */...) /* -> bool{!(a xor b)} */ PPUTLXNOR_X(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLXNOR_X(a, b) PTL_CAT(PPUTLXNOR_, PTL_CAT(PTL_BOOL(a), PTL_BOOL(b)))
#define PPUTLXNOR_11      1
#define PPUTLXNOR_10      0
#define PPUTLXNOR_01      0
#define PPUTLXNOR_00      1

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
/// uses mutual recursion; can track any number of expansions.
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

/// [util.size]
/// -----------
/// O(1) variadic argument size computation.
/// fails if too many args passed.
///
/// PTL_SIZE()     // 0u
/// PTL_SIZE(a)    // 1u
/// PTL_SIZE(a, b) // 2u
/// PTL_SIZE(, )   // 2u
#define PTL_SIZE(/* v: any... */...) /* -> udec */ \
  PPUTLSIZE_X(PTL_ISTR([PTL_SIZE] too many args : __VA_ARGS__), PPUTLSIZE, __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLSIZE_X(_err, _, ...)                                                                  \
  PPUTLSIZE_READ(                                                                                  \
      _err, __VA_ARGS__ __VA_OPT__(, ) _(4095u), _(4094u), _(4093u), _(4092u), _(4091u), _(4090u), \
      _(4089u), _(4088u), _(4087u), _(4086u), _(4085u), _(4084u), _(4083u), _(4082u), _(4081u),    \
      _(4080u), _(4079u), _(4078u), _(4077u), _(4076u), _(4075u), _(4074u), _(4073u), _(4072u),    \
      _(4071u), _(4070u), _(4069u), _(4068u), _(4067u), _(4066u), _(4065u), _(4064u), _(4063u),    \
      _(4062u), _(4061u), _(4060u), _(4059u), _(4058u), _(4057u), _(4056u), _(4055u), _(4054u),    \
      _(4053u), _(4052u), _(4051u), _(4050u), _(4049u), _(4048u), _(4047u), _(4046u), _(4045u),    \
      _(4044u), _(4043u), _(4042u), _(4041u), _(4040u), _(4039u), _(4038u), _(4037u), _(4036u),    \
      _(4035u), _(4034u), _(4033u), _(4032u), _(4031u), _(4030u), _(4029u), _(4028u), _(4027u),    \
      _(4026u), _(4025u), _(4024u), _(4023u), _(4022u), _(4021u), _(4020u), _(4019u), _(4018u),    \
      _(4017u), _(4016u), _(4015u), _(4014u), _(4013u), _(4012u), _(4011u), _(4010u), _(4009u),    \
      _(4008u), _(4007u), _(4006u), _(4005u), _(4004u), _(4003u), _(4002u), _(4001u), _(4000u),    \
      _(3999u), _(3998u), _(3997u), _(3996u), _(3995u), _(3994u), _(3993u), _(3992u), _(3991u),    \
      _(3990u), _(3989u), _(3988u), _(3987u), _(3986u), _(3985u), _(3984u), _(3983u), _(3982u),    \
      _(3981u), _(3980u), _(3979u), _(3978u), _(3977u), _(3976u), _(3975u), _(3974u), _(3973u),    \
      _(3972u), _(3971u), _(3970u), _(3969u), _(3968u), _(3967u), _(3966u), _(3965u), _(3964u),    \
      _(3963u), _(3962u), _(3961u), _(3960u), _(3959u), _(3958u), _(3957u), _(3956u), _(3955u),    \
      _(3954u), _(3953u), _(3952u), _(3951u), _(3950u), _(3949u), _(3948u), _(3947u), _(3946u),    \
      _(3945u), _(3944u), _(3943u), _(3942u), _(3941u), _(3940u), _(3939u), _(3938u), _(3937u),    \
      _(3936u), _(3935u), _(3934u), _(3933u), _(3932u), _(3931u), _(3930u), _(3929u), _(3928u),    \
      _(3927u), _(3926u), _(3925u), _(3924u), _(3923u), _(3922u), _(3921u), _(3920u), _(3919u),    \
      _(3918u), _(3917u), _(3916u), _(3915u), _(3914u), _(3913u), _(3912u), _(3911u), _(3910u),    \
      _(3909u), _(3908u), _(3907u), _(3906u), _(3905u), _(3904u), _(3903u), _(3902u), _(3901u),    \
      _(3900u), _(3899u), _(3898u), _(3897u), _(3896u), _(3895u), _(3894u), _(3893u), _(3892u),    \
      _(3891u), _(3890u), _(3889u), _(3888u), _(3887u), _(3886u), _(3885u), _(3884u), _(3883u),    \
      _(3882u), _(3881u), _(3880u), _(3879u), _(3878u), _(3877u), _(3876u), _(3875u), _(3874u),    \
      _(3873u), _(3872u), _(3871u), _(3870u), _(3869u), _(3868u), _(3867u), _(3866u), _(3865u),    \
      _(3864u), _(3863u), _(3862u), _(3861u), _(3860u), _(3859u), _(3858u), _(3857u), _(3856u),    \
      _(3855u), _(3854u), _(3853u), _(3852u), _(3851u), _(3850u), _(3849u), _(3848u), _(3847u),    \
      _(3846u), _(3845u), _(3844u), _(3843u), _(3842u), _(3841u), _(3840u), _(3839u), _(3838u),    \
      _(3837u), _(3836u), _(3835u), _(3834u), _(3833u), _(3832u), _(3831u), _(3830u), _(3829u),    \
      _(3828u), _(3827u), _(3826u), _(3825u), _(3824u), _(3823u), _(3822u), _(3821u), _(3820u),    \
      _(3819u), _(3818u), _(3817u), _(3816u), _(3815u), _(3814u), _(3813u), _(3812u), _(3811u),    \
      _(3810u), _(3809u), _(3808u), _(3807u), _(3806u), _(3805u), _(3804u), _(3803u), _(3802u),    \
      _(3801u), _(3800u), _(3799u), _(3798u), _(3797u), _(3796u), _(3795u), _(3794u), _(3793u),    \
      _(3792u), _(3791u), _(3790u), _(3789u), _(3788u), _(3787u), _(3786u), _(3785u), _(3784u),    \
      _(3783u), _(3782u), _(3781u), _(3780u), _(3779u), _(3778u), _(3777u), _(3776u), _(3775u),    \
      _(3774u), _(3773u), _(3772u), _(3771u), _(3770u), _(3769u), _(3768u), _(3767u), _(3766u),    \
      _(3765u), _(3764u), _(3763u), _(3762u), _(3761u), _(3760u), _(3759u), _(3758u), _(3757u),    \
      _(3756u), _(3755u), _(3754u), _(3753u), _(3752u), _(3751u), _(3750u), _(3749u), _(3748u),    \
      _(3747u), _(3746u), _(3745u), _(3744u), _(3743u), _(3742u), _(3741u), _(3740u), _(3739u),    \
      _(3738u), _(3737u), _(3736u), _(3735u), _(3734u), _(3733u), _(3732u), _(3731u), _(3730u),    \
      _(3729u), _(3728u), _(3727u), _(3726u), _(3725u), _(3724u), _(3723u), _(3722u), _(3721u),    \
      _(3720u), _(3719u), _(3718u), _(3717u), _(3716u), _(3715u), _(3714u), _(3713u), _(3712u),    \
      _(3711u), _(3710u), _(3709u), _(3708u), _(3707u), _(3706u), _(3705u), _(3704u), _(3703u),    \
      _(3702u), _(3701u), _(3700u), _(3699u), _(3698u), _(3697u), _(3696u), _(3695u), _(3694u),    \
      _(3693u), _(3692u), _(3691u), _(3690u), _(3689u), _(3688u), _(3687u), _(3686u), _(3685u),    \
      _(3684u), _(3683u), _(3682u), _(3681u), _(3680u), _(3679u), _(3678u), _(3677u), _(3676u),    \
      _(3675u), _(3674u), _(3673u), _(3672u), _(3671u), _(3670u), _(3669u), _(3668u), _(3667u),    \
      _(3666u), _(3665u), _(3664u), _(3663u), _(3662u), _(3661u), _(3660u), _(3659u), _(3658u),    \
      _(3657u), _(3656u), _(3655u), _(3654u), _(3653u), _(3652u), _(3651u), _(3650u), _(3649u),    \
      _(3648u), _(3647u), _(3646u), _(3645u), _(3644u), _(3643u), _(3642u), _(3641u), _(3640u),    \
      _(3639u), _(3638u), _(3637u), _(3636u), _(3635u), _(3634u), _(3633u), _(3632u), _(3631u),    \
      _(3630u), _(3629u), _(3628u), _(3627u), _(3626u), _(3625u), _(3624u), _(3623u), _(3622u),    \
      _(3621u), _(3620u), _(3619u), _(3618u), _(3617u), _(3616u), _(3615u), _(3614u), _(3613u),    \
      _(3612u), _(3611u), _(3610u), _(3609u), _(3608u), _(3607u), _(3606u), _(3605u), _(3604u),    \
      _(3603u), _(3602u), _(3601u), _(3600u), _(3599u), _(3598u), _(3597u), _(3596u), _(3595u),    \
      _(3594u), _(3593u), _(3592u), _(3591u), _(3590u), _(3589u), _(3588u), _(3587u), _(3586u),    \
      _(3585u), _(3584u), _(3583u), _(3582u), _(3581u), _(3580u), _(3579u), _(3578u), _(3577u),    \
      _(3576u), _(3575u), _(3574u), _(3573u), _(3572u), _(3571u), _(3570u), _(3569u), _(3568u),    \
      _(3567u), _(3566u), _(3565u), _(3564u), _(3563u), _(3562u), _(3561u), _(3560u), _(3559u),    \
      _(3558u), _(3557u), _(3556u), _(3555u), _(3554u), _(3553u), _(3552u), _(3551u), _(3550u),    \
      _(3549u), _(3548u), _(3547u), _(3546u), _(3545u), _(3544u), _(3543u), _(3542u), _(3541u),    \
      _(3540u), _(3539u), _(3538u), _(3537u), _(3536u), _(3535u), _(3534u), _(3533u), _(3532u),    \
      _(3531u), _(3530u), _(3529u), _(3528u), _(3527u), _(3526u), _(3525u), _(3524u), _(3523u),    \
      _(3522u), _(3521u), _(3520u), _(3519u), _(3518u), _(3517u), _(3516u), _(3515u), _(3514u),    \
      _(3513u), _(3512u), _(3511u), _(3510u), _(3509u), _(3508u), _(3507u), _(3506u), _(3505u),    \
      _(3504u), _(3503u), _(3502u), _(3501u), _(3500u), _(3499u), _(3498u), _(3497u), _(3496u),    \
      _(3495u), _(3494u), _(3493u), _(3492u), _(3491u), _(3490u), _(3489u), _(3488u), _(3487u),    \
      _(3486u), _(3485u), _(3484u), _(3483u), _(3482u), _(3481u), _(3480u), _(3479u), _(3478u),    \
      _(3477u), _(3476u), _(3475u), _(3474u), _(3473u), _(3472u), _(3471u), _(3470u), _(3469u),    \
      _(3468u), _(3467u), _(3466u), _(3465u), _(3464u), _(3463u), _(3462u), _(3461u), _(3460u),    \
      _(3459u), _(3458u), _(3457u), _(3456u), _(3455u), _(3454u), _(3453u), _(3452u), _(3451u),    \
      _(3450u), _(3449u), _(3448u), _(3447u), _(3446u), _(3445u), _(3444u), _(3443u), _(3442u),    \
      _(3441u), _(3440u), _(3439u), _(3438u), _(3437u), _(3436u), _(3435u), _(3434u), _(3433u),    \
      _(3432u), _(3431u), _(3430u), _(3429u), _(3428u), _(3427u), _(3426u), _(3425u), _(3424u),    \
      _(3423u), _(3422u), _(3421u), _(3420u), _(3419u), _(3418u), _(3417u), _(3416u), _(3415u),    \
      _(3414u), _(3413u), _(3412u), _(3411u), _(3410u), _(3409u), _(3408u), _(3407u), _(3406u),    \
      _(3405u), _(3404u), _(3403u), _(3402u), _(3401u), _(3400u), _(3399u), _(3398u), _(3397u),    \
      _(3396u), _(3395u), _(3394u), _(3393u), _(3392u), _(3391u), _(3390u), _(3389u), _(3388u),    \
      _(3387u), _(3386u), _(3385u), _(3384u), _(3383u), _(3382u), _(3381u), _(3380u), _(3379u),    \
      _(3378u), _(3377u), _(3376u), _(3375u), _(3374u), _(3373u), _(3372u), _(3371u), _(3370u),    \
      _(3369u), _(3368u), _(3367u), _(3366u), _(3365u), _(3364u), _(3363u), _(3362u), _(3361u),    \
      _(3360u), _(3359u), _(3358u), _(3357u), _(3356u), _(3355u), _(3354u), _(3353u), _(3352u),    \
      _(3351u), _(3350u), _(3349u), _(3348u), _(3347u), _(3346u), _(3345u), _(3344u), _(3343u),    \
      _(3342u), _(3341u), _(3340u), _(3339u), _(3338u), _(3337u), _(3336u), _(3335u), _(3334u),    \
      _(3333u), _(3332u), _(3331u), _(3330u), _(3329u), _(3328u), _(3327u), _(3326u), _(3325u),    \
      _(3324u), _(3323u), _(3322u), _(3321u), _(3320u), _(3319u), _(3318u), _(3317u), _(3316u),    \
      _(3315u), _(3314u), _(3313u), _(3312u), _(3311u), _(3310u), _(3309u), _(3308u), _(3307u),    \
      _(3306u), _(3305u), _(3304u), _(3303u), _(3302u), _(3301u), _(3300u), _(3299u), _(3298u),    \
      _(3297u), _(3296u), _(3295u), _(3294u), _(3293u), _(3292u), _(3291u), _(3290u), _(3289u),    \
      _(3288u), _(3287u), _(3286u), _(3285u), _(3284u), _(3283u), _(3282u), _(3281u), _(3280u),    \
      _(3279u), _(3278u), _(3277u), _(3276u), _(3275u), _(3274u), _(3273u), _(3272u), _(3271u),    \
      _(3270u), _(3269u), _(3268u), _(3267u), _(3266u), _(3265u), _(3264u), _(3263u), _(3262u),    \
      _(3261u), _(3260u), _(3259u), _(3258u), _(3257u), _(3256u), _(3255u), _(3254u), _(3253u),    \
      _(3252u), _(3251u), _(3250u), _(3249u), _(3248u), _(3247u), _(3246u), _(3245u), _(3244u),    \
      _(3243u), _(3242u), _(3241u), _(3240u), _(3239u), _(3238u), _(3237u), _(3236u), _(3235u),    \
      _(3234u), _(3233u), _(3232u), _(3231u), _(3230u), _(3229u), _(3228u), _(3227u), _(3226u),    \
      _(3225u), _(3224u), _(3223u), _(3222u), _(3221u), _(3220u), _(3219u), _(3218u), _(3217u),    \
      _(3216u), _(3215u), _(3214u), _(3213u), _(3212u), _(3211u), _(3210u), _(3209u), _(3208u),    \
      _(3207u), _(3206u), _(3205u), _(3204u), _(3203u), _(3202u), _(3201u), _(3200u), _(3199u),    \
      _(3198u), _(3197u), _(3196u), _(3195u), _(3194u), _(3193u), _(3192u), _(3191u), _(3190u),    \
      _(3189u), _(3188u), _(3187u), _(3186u), _(3185u), _(3184u), _(3183u), _(3182u), _(3181u),    \
      _(3180u), _(3179u), _(3178u), _(3177u), _(3176u), _(3175u), _(3174u), _(3173u), _(3172u),    \
      _(3171u), _(3170u), _(3169u), _(3168u), _(3167u), _(3166u), _(3165u), _(3164u), _(3163u),    \
      _(3162u), _(3161u), _(3160u), _(3159u), _(3158u), _(3157u), _(3156u), _(3155u), _(3154u),    \
      _(3153u), _(3152u), _(3151u), _(3150u), _(3149u), _(3148u), _(3147u), _(3146u), _(3145u),    \
      _(3144u), _(3143u), _(3142u), _(3141u), _(3140u), _(3139u), _(3138u), _(3137u), _(3136u),    \
      _(3135u), _(3134u), _(3133u), _(3132u), _(3131u), _(3130u), _(3129u), _(3128u), _(3127u),    \
      _(3126u), _(3125u), _(3124u), _(3123u), _(3122u), _(3121u), _(3120u), _(3119u), _(3118u),    \
      _(3117u), _(3116u), _(3115u), _(3114u), _(3113u), _(3112u), _(3111u), _(3110u), _(3109u),    \
      _(3108u), _(3107u), _(3106u), _(3105u), _(3104u), _(3103u), _(3102u), _(3101u), _(3100u),    \
      _(3099u), _(3098u), _(3097u), _(3096u), _(3095u), _(3094u), _(3093u), _(3092u), _(3091u),    \
      _(3090u), _(3089u), _(3088u), _(3087u), _(3086u), _(3085u), _(3084u), _(3083u), _(3082u),    \
      _(3081u), _(3080u), _(3079u), _(3078u), _(3077u), _(3076u), _(3075u), _(3074u), _(3073u),    \
      _(3072u), _(3071u), _(3070u), _(3069u), _(3068u), _(3067u), _(3066u), _(3065u), _(3064u),    \
      _(3063u), _(3062u), _(3061u), _(3060u), _(3059u), _(3058u), _(3057u), _(3056u), _(3055u),    \
      _(3054u), _(3053u), _(3052u), _(3051u), _(3050u), _(3049u), _(3048u), _(3047u), _(3046u),    \
      _(3045u), _(3044u), _(3043u), _(3042u), _(3041u), _(3040u), _(3039u), _(3038u), _(3037u),    \
      _(3036u), _(3035u), _(3034u), _(3033u), _(3032u), _(3031u), _(3030u), _(3029u), _(3028u),    \
      _(3027u), _(3026u), _(3025u), _(3024u), _(3023u), _(3022u), _(3021u), _(3020u), _(3019u),    \
      _(3018u), _(3017u), _(3016u), _(3015u), _(3014u), _(3013u), _(3012u), _(3011u), _(3010u),    \
      _(3009u), _(3008u), _(3007u), _(3006u), _(3005u), _(3004u), _(3003u), _(3002u), _(3001u),    \
      _(3000u), _(2999u), _(2998u), _(2997u), _(2996u), _(2995u), _(2994u), _(2993u), _(2992u),    \
      _(2991u), _(2990u), _(2989u), _(2988u), _(2987u), _(2986u), _(2985u), _(2984u), _(2983u),    \
      _(2982u), _(2981u), _(2980u), _(2979u), _(2978u), _(2977u), _(2976u), _(2975u), _(2974u),    \
      _(2973u), _(2972u), _(2971u), _(2970u), _(2969u), _(2968u), _(2967u), _(2966u), _(2965u),    \
      _(2964u), _(2963u), _(2962u), _(2961u), _(2960u), _(2959u), _(2958u), _(2957u), _(2956u),    \
      _(2955u), _(2954u), _(2953u), _(2952u), _(2951u), _(2950u), _(2949u), _(2948u), _(2947u),    \
      _(2946u), _(2945u), _(2944u), _(2943u), _(2942u), _(2941u), _(2940u), _(2939u), _(2938u),    \
      _(2937u), _(2936u), _(2935u), _(2934u), _(2933u), _(2932u), _(2931u), _(2930u), _(2929u),    \
      _(2928u), _(2927u), _(2926u), _(2925u), _(2924u), _(2923u), _(2922u), _(2921u), _(2920u),    \
      _(2919u), _(2918u), _(2917u), _(2916u), _(2915u), _(2914u), _(2913u), _(2912u), _(2911u),    \
      _(2910u), _(2909u), _(2908u), _(2907u), _(2906u), _(2905u), _(2904u), _(2903u), _(2902u),    \
      _(2901u), _(2900u), _(2899u), _(2898u), _(2897u), _(2896u), _(2895u), _(2894u), _(2893u),    \
      _(2892u), _(2891u), _(2890u), _(2889u), _(2888u), _(2887u), _(2886u), _(2885u), _(2884u),    \
      _(2883u), _(2882u), _(2881u), _(2880u), _(2879u), _(2878u), _(2877u), _(2876u), _(2875u),    \
      _(2874u), _(2873u), _(2872u), _(2871u), _(2870u), _(2869u), _(2868u), _(2867u), _(2866u),    \
      _(2865u), _(2864u), _(2863u), _(2862u), _(2861u), _(2860u), _(2859u), _(2858u), _(2857u),    \
      _(2856u), _(2855u), _(2854u), _(2853u), _(2852u), _(2851u), _(2850u), _(2849u), _(2848u),    \
      _(2847u), _(2846u), _(2845u), _(2844u), _(2843u), _(2842u), _(2841u), _(2840u), _(2839u),    \
      _(2838u), _(2837u), _(2836u), _(2835u), _(2834u), _(2833u), _(2832u), _(2831u), _(2830u),    \
      _(2829u), _(2828u), _(2827u), _(2826u), _(2825u), _(2824u), _(2823u), _(2822u), _(2821u),    \
      _(2820u), _(2819u), _(2818u), _(2817u), _(2816u), _(2815u), _(2814u), _(2813u), _(2812u),    \
      _(2811u), _(2810u), _(2809u), _(2808u), _(2807u), _(2806u), _(2805u), _(2804u), _(2803u),    \
      _(2802u), _(2801u), _(2800u), _(2799u), _(2798u), _(2797u), _(2796u), _(2795u), _(2794u),    \
      _(2793u), _(2792u), _(2791u), _(2790u), _(2789u), _(2788u), _(2787u), _(2786u), _(2785u),    \
      _(2784u), _(2783u), _(2782u), _(2781u), _(2780u), _(2779u), _(2778u), _(2777u), _(2776u),    \
      _(2775u), _(2774u), _(2773u), _(2772u), _(2771u), _(2770u), _(2769u), _(2768u), _(2767u),    \
      _(2766u), _(2765u), _(2764u), _(2763u), _(2762u), _(2761u), _(2760u), _(2759u), _(2758u),    \
      _(2757u), _(2756u), _(2755u), _(2754u), _(2753u), _(2752u), _(2751u), _(2750u), _(2749u),    \
      _(2748u), _(2747u), _(2746u), _(2745u), _(2744u), _(2743u), _(2742u), _(2741u), _(2740u),    \
      _(2739u), _(2738u), _(2737u), _(2736u), _(2735u), _(2734u), _(2733u), _(2732u), _(2731u),    \
      _(2730u), _(2729u), _(2728u), _(2727u), _(2726u), _(2725u), _(2724u), _(2723u), _(2722u),    \
      _(2721u), _(2720u), _(2719u), _(2718u), _(2717u), _(2716u), _(2715u), _(2714u), _(2713u),    \
      _(2712u), _(2711u), _(2710u), _(2709u), _(2708u), _(2707u), _(2706u), _(2705u), _(2704u),    \
      _(2703u), _(2702u), _(2701u), _(2700u), _(2699u), _(2698u), _(2697u), _(2696u), _(2695u),    \
      _(2694u), _(2693u), _(2692u), _(2691u), _(2690u), _(2689u), _(2688u), _(2687u), _(2686u),    \
      _(2685u), _(2684u), _(2683u), _(2682u), _(2681u), _(2680u), _(2679u), _(2678u), _(2677u),    \
      _(2676u), _(2675u), _(2674u), _(2673u), _(2672u), _(2671u), _(2670u), _(2669u), _(2668u),    \
      _(2667u), _(2666u), _(2665u), _(2664u), _(2663u), _(2662u), _(2661u), _(2660u), _(2659u),    \
      _(2658u), _(2657u), _(2656u), _(2655u), _(2654u), _(2653u), _(2652u), _(2651u), _(2650u),    \
      _(2649u), _(2648u), _(2647u), _(2646u), _(2645u), _(2644u), _(2643u), _(2642u), _(2641u),    \
      _(2640u), _(2639u), _(2638u), _(2637u), _(2636u), _(2635u), _(2634u), _(2633u), _(2632u),    \
      _(2631u), _(2630u), _(2629u), _(2628u), _(2627u), _(2626u), _(2625u), _(2624u), _(2623u),    \
      _(2622u), _(2621u), _(2620u), _(2619u), _(2618u), _(2617u), _(2616u), _(2615u), _(2614u),    \
      _(2613u), _(2612u), _(2611u), _(2610u), _(2609u), _(2608u), _(2607u), _(2606u), _(2605u),    \
      _(2604u), _(2603u), _(2602u), _(2601u), _(2600u), _(2599u), _(2598u), _(2597u), _(2596u),    \
      _(2595u), _(2594u), _(2593u), _(2592u), _(2591u), _(2590u), _(2589u), _(2588u), _(2587u),    \
      _(2586u), _(2585u), _(2584u), _(2583u), _(2582u), _(2581u), _(2580u), _(2579u), _(2578u),    \
      _(2577u), _(2576u), _(2575u), _(2574u), _(2573u), _(2572u), _(2571u), _(2570u), _(2569u),    \
      _(2568u), _(2567u), _(2566u), _(2565u), _(2564u), _(2563u), _(2562u), _(2561u), _(2560u),    \
      _(2559u), _(2558u), _(2557u), _(2556u), _(2555u), _(2554u), _(2553u), _(2552u), _(2551u),    \
      _(2550u), _(2549u), _(2548u), _(2547u), _(2546u), _(2545u), _(2544u), _(2543u), _(2542u),    \
      _(2541u), _(2540u), _(2539u), _(2538u), _(2537u), _(2536u), _(2535u), _(2534u), _(2533u),    \
      _(2532u), _(2531u), _(2530u), _(2529u), _(2528u), _(2527u), _(2526u), _(2525u), _(2524u),    \
      _(2523u), _(2522u), _(2521u), _(2520u), _(2519u), _(2518u), _(2517u), _(2516u), _(2515u),    \
      _(2514u), _(2513u), _(2512u), _(2511u), _(2510u), _(2509u), _(2508u), _(2507u), _(2506u),    \
      _(2505u), _(2504u), _(2503u), _(2502u), _(2501u), _(2500u), _(2499u), _(2498u), _(2497u),    \
      _(2496u), _(2495u), _(2494u), _(2493u), _(2492u), _(2491u), _(2490u), _(2489u), _(2488u),    \
      _(2487u), _(2486u), _(2485u), _(2484u), _(2483u), _(2482u), _(2481u), _(2480u), _(2479u),    \
      _(2478u), _(2477u), _(2476u), _(2475u), _(2474u), _(2473u), _(2472u), _(2471u), _(2470u),    \
      _(2469u), _(2468u), _(2467u), _(2466u), _(2465u), _(2464u), _(2463u), _(2462u), _(2461u),    \
      _(2460u), _(2459u), _(2458u), _(2457u), _(2456u), _(2455u), _(2454u), _(2453u), _(2452u),    \
      _(2451u), _(2450u), _(2449u), _(2448u), _(2447u), _(2446u), _(2445u), _(2444u), _(2443u),    \
      _(2442u), _(2441u), _(2440u), _(2439u), _(2438u), _(2437u), _(2436u), _(2435u), _(2434u),    \
      _(2433u), _(2432u), _(2431u), _(2430u), _(2429u), _(2428u), _(2427u), _(2426u), _(2425u),    \
      _(2424u), _(2423u), _(2422u), _(2421u), _(2420u), _(2419u), _(2418u), _(2417u), _(2416u),    \
      _(2415u), _(2414u), _(2413u), _(2412u), _(2411u), _(2410u), _(2409u), _(2408u), _(2407u),    \
      _(2406u), _(2405u), _(2404u), _(2403u), _(2402u), _(2401u), _(2400u), _(2399u), _(2398u),    \
      _(2397u), _(2396u), _(2395u), _(2394u), _(2393u), _(2392u), _(2391u), _(2390u), _(2389u),    \
      _(2388u), _(2387u), _(2386u), _(2385u), _(2384u), _(2383u), _(2382u), _(2381u), _(2380u),    \
      _(2379u), _(2378u), _(2377u), _(2376u), _(2375u), _(2374u), _(2373u), _(2372u), _(2371u),    \
      _(2370u), _(2369u), _(2368u), _(2367u), _(2366u), _(2365u), _(2364u), _(2363u), _(2362u),    \
      _(2361u), _(2360u), _(2359u), _(2358u), _(2357u), _(2356u), _(2355u), _(2354u), _(2353u),    \
      _(2352u), _(2351u), _(2350u), _(2349u), _(2348u), _(2347u), _(2346u), _(2345u), _(2344u),    \
      _(2343u), _(2342u), _(2341u), _(2340u), _(2339u), _(2338u), _(2337u), _(2336u), _(2335u),    \
      _(2334u), _(2333u), _(2332u), _(2331u), _(2330u), _(2329u), _(2328u), _(2327u), _(2326u),    \
      _(2325u), _(2324u), _(2323u), _(2322u), _(2321u), _(2320u), _(2319u), _(2318u), _(2317u),    \
      _(2316u), _(2315u), _(2314u), _(2313u), _(2312u), _(2311u), _(2310u), _(2309u), _(2308u),    \
      _(2307u), _(2306u), _(2305u), _(2304u), _(2303u), _(2302u), _(2301u), _(2300u), _(2299u),    \
      _(2298u), _(2297u), _(2296u), _(2295u), _(2294u), _(2293u), _(2292u), _(2291u), _(2290u),    \
      _(2289u), _(2288u), _(2287u), _(2286u), _(2285u), _(2284u), _(2283u), _(2282u), _(2281u),    \
      _(2280u), _(2279u), _(2278u), _(2277u), _(2276u), _(2275u), _(2274u), _(2273u), _(2272u),    \
      _(2271u), _(2270u), _(2269u), _(2268u), _(2267u), _(2266u), _(2265u), _(2264u), _(2263u),    \
      _(2262u), _(2261u), _(2260u), _(2259u), _(2258u), _(2257u), _(2256u), _(2255u), _(2254u),    \
      _(2253u), _(2252u), _(2251u), _(2250u), _(2249u), _(2248u), _(2247u), _(2246u), _(2245u),    \
      _(2244u), _(2243u), _(2242u), _(2241u), _(2240u), _(2239u), _(2238u), _(2237u), _(2236u),    \
      _(2235u), _(2234u), _(2233u), _(2232u), _(2231u), _(2230u), _(2229u), _(2228u), _(2227u),    \
      _(2226u), _(2225u), _(2224u), _(2223u), _(2222u), _(2221u), _(2220u), _(2219u), _(2218u),    \
      _(2217u), _(2216u), _(2215u), _(2214u), _(2213u), _(2212u), _(2211u), _(2210u), _(2209u),    \
      _(2208u), _(2207u), _(2206u), _(2205u), _(2204u), _(2203u), _(2202u), _(2201u), _(2200u),    \
      _(2199u), _(2198u), _(2197u), _(2196u), _(2195u), _(2194u), _(2193u), _(2192u), _(2191u),    \
      _(2190u), _(2189u), _(2188u), _(2187u), _(2186u), _(2185u), _(2184u), _(2183u), _(2182u),    \
      _(2181u), _(2180u), _(2179u), _(2178u), _(2177u), _(2176u), _(2175u), _(2174u), _(2173u),    \
      _(2172u), _(2171u), _(2170u), _(2169u), _(2168u), _(2167u), _(2166u), _(2165u), _(2164u),    \
      _(2163u), _(2162u), _(2161u), _(2160u), _(2159u), _(2158u), _(2157u), _(2156u), _(2155u),    \
      _(2154u), _(2153u), _(2152u), _(2151u), _(2150u), _(2149u), _(2148u), _(2147u), _(2146u),    \
      _(2145u), _(2144u), _(2143u), _(2142u), _(2141u), _(2140u), _(2139u), _(2138u), _(2137u),    \
      _(2136u), _(2135u), _(2134u), _(2133u), _(2132u), _(2131u), _(2130u), _(2129u), _(2128u),    \
      _(2127u), _(2126u), _(2125u), _(2124u), _(2123u), _(2122u), _(2121u), _(2120u), _(2119u),    \
      _(2118u), _(2117u), _(2116u), _(2115u), _(2114u), _(2113u), _(2112u), _(2111u), _(2110u),    \
      _(2109u), _(2108u), _(2107u), _(2106u), _(2105u), _(2104u), _(2103u), _(2102u), _(2101u),    \
      _(2100u), _(2099u), _(2098u), _(2097u), _(2096u), _(2095u), _(2094u), _(2093u), _(2092u),    \
      _(2091u), _(2090u), _(2089u), _(2088u), _(2087u), _(2086u), _(2085u), _(2084u), _(2083u),    \
      _(2082u), _(2081u), _(2080u), _(2079u), _(2078u), _(2077u), _(2076u), _(2075u), _(2074u),    \
      _(2073u), _(2072u), _(2071u), _(2070u), _(2069u), _(2068u), _(2067u), _(2066u), _(2065u),    \
      _(2064u), _(2063u), _(2062u), _(2061u), _(2060u), _(2059u), _(2058u), _(2057u), _(2056u),    \
      _(2055u), _(2054u), _(2053u), _(2052u), _(2051u), _(2050u), _(2049u), _(2048u), _(2047u),    \
      _(2046u), _(2045u), _(2044u), _(2043u), _(2042u), _(2041u), _(2040u), _(2039u), _(2038u),    \
      _(2037u), _(2036u), _(2035u), _(2034u), _(2033u), _(2032u), _(2031u), _(2030u), _(2029u),    \
      _(2028u), _(2027u), _(2026u), _(2025u), _(2024u), _(2023u), _(2022u), _(2021u), _(2020u),    \
      _(2019u), _(2018u), _(2017u), _(2016u), _(2015u), _(2014u), _(2013u), _(2012u), _(2011u),    \
      _(2010u), _(2009u), _(2008u), _(2007u), _(2006u), _(2005u), _(2004u), _(2003u), _(2002u),    \
      _(2001u), _(2000u), _(1999u), _(1998u), _(1997u), _(1996u), _(1995u), _(1994u), _(1993u),    \
      _(1992u), _(1991u), _(1990u), _(1989u), _(1988u), _(1987u), _(1986u), _(1985u), _(1984u),    \
      _(1983u), _(1982u), _(1981u), _(1980u), _(1979u), _(1978u), _(1977u), _(1976u), _(1975u),    \
      _(1974u), _(1973u), _(1972u), _(1971u), _(1970u), _(1969u), _(1968u), _(1967u), _(1966u),    \
      _(1965u), _(1964u), _(1963u), _(1962u), _(1961u), _(1960u), _(1959u), _(1958u), _(1957u),    \
      _(1956u), _(1955u), _(1954u), _(1953u), _(1952u), _(1951u), _(1950u), _(1949u), _(1948u),    \
      _(1947u), _(1946u), _(1945u), _(1944u), _(1943u), _(1942u), _(1941u), _(1940u), _(1939u),    \
      _(1938u), _(1937u), _(1936u), _(1935u), _(1934u), _(1933u), _(1932u), _(1931u), _(1930u),    \
      _(1929u), _(1928u), _(1927u), _(1926u), _(1925u), _(1924u), _(1923u), _(1922u), _(1921u),    \
      _(1920u), _(1919u), _(1918u), _(1917u), _(1916u), _(1915u), _(1914u), _(1913u), _(1912u),    \
      _(1911u), _(1910u), _(1909u), _(1908u), _(1907u), _(1906u), _(1905u), _(1904u), _(1903u),    \
      _(1902u), _(1901u), _(1900u), _(1899u), _(1898u), _(1897u), _(1896u), _(1895u), _(1894u),    \
      _(1893u), _(1892u), _(1891u), _(1890u), _(1889u), _(1888u), _(1887u), _(1886u), _(1885u),    \
      _(1884u), _(1883u), _(1882u), _(1881u), _(1880u), _(1879u), _(1878u), _(1877u), _(1876u),    \
      _(1875u), _(1874u), _(1873u), _(1872u), _(1871u), _(1870u), _(1869u), _(1868u), _(1867u),    \
      _(1866u), _(1865u), _(1864u), _(1863u), _(1862u), _(1861u), _(1860u), _(1859u), _(1858u),    \
      _(1857u), _(1856u), _(1855u), _(1854u), _(1853u), _(1852u), _(1851u), _(1850u), _(1849u),    \
      _(1848u), _(1847u), _(1846u), _(1845u), _(1844u), _(1843u), _(1842u), _(1841u), _(1840u),    \
      _(1839u), _(1838u), _(1837u), _(1836u), _(1835u), _(1834u), _(1833u), _(1832u), _(1831u),    \
      _(1830u), _(1829u), _(1828u), _(1827u), _(1826u), _(1825u), _(1824u), _(1823u), _(1822u),    \
      _(1821u), _(1820u), _(1819u), _(1818u), _(1817u), _(1816u), _(1815u), _(1814u), _(1813u),    \
      _(1812u), _(1811u), _(1810u), _(1809u), _(1808u), _(1807u), _(1806u), _(1805u), _(1804u),    \
      _(1803u), _(1802u), _(1801u), _(1800u), _(1799u), _(1798u), _(1797u), _(1796u), _(1795u),    \
      _(1794u), _(1793u), _(1792u), _(1791u), _(1790u), _(1789u), _(1788u), _(1787u), _(1786u),    \
      _(1785u), _(1784u), _(1783u), _(1782u), _(1781u), _(1780u), _(1779u), _(1778u), _(1777u),    \
      _(1776u), _(1775u), _(1774u), _(1773u), _(1772u), _(1771u), _(1770u), _(1769u), _(1768u),    \
      _(1767u), _(1766u), _(1765u), _(1764u), _(1763u), _(1762u), _(1761u), _(1760u), _(1759u),    \
      _(1758u), _(1757u), _(1756u), _(1755u), _(1754u), _(1753u), _(1752u), _(1751u), _(1750u),    \
      _(1749u), _(1748u), _(1747u), _(1746u), _(1745u), _(1744u), _(1743u), _(1742u), _(1741u),    \
      _(1740u), _(1739u), _(1738u), _(1737u), _(1736u), _(1735u), _(1734u), _(1733u), _(1732u),    \
      _(1731u), _(1730u), _(1729u), _(1728u), _(1727u), _(1726u), _(1725u), _(1724u), _(1723u),    \
      _(1722u), _(1721u), _(1720u), _(1719u), _(1718u), _(1717u), _(1716u), _(1715u), _(1714u),    \
      _(1713u), _(1712u), _(1711u), _(1710u), _(1709u), _(1708u), _(1707u), _(1706u), _(1705u),    \
      _(1704u), _(1703u), _(1702u), _(1701u), _(1700u), _(1699u), _(1698u), _(1697u), _(1696u),    \
      _(1695u), _(1694u), _(1693u), _(1692u), _(1691u), _(1690u), _(1689u), _(1688u), _(1687u),    \
      _(1686u), _(1685u), _(1684u), _(1683u), _(1682u), _(1681u), _(1680u), _(1679u), _(1678u),    \
      _(1677u), _(1676u), _(1675u), _(1674u), _(1673u), _(1672u), _(1671u), _(1670u), _(1669u),    \
      _(1668u), _(1667u), _(1666u), _(1665u), _(1664u), _(1663u), _(1662u), _(1661u), _(1660u),    \
      _(1659u), _(1658u), _(1657u), _(1656u), _(1655u), _(1654u), _(1653u), _(1652u), _(1651u),    \
      _(1650u), _(1649u), _(1648u), _(1647u), _(1646u), _(1645u), _(1644u), _(1643u), _(1642u),    \
      _(1641u), _(1640u), _(1639u), _(1638u), _(1637u), _(1636u), _(1635u), _(1634u), _(1633u),    \
      _(1632u), _(1631u), _(1630u), _(1629u), _(1628u), _(1627u), _(1626u), _(1625u), _(1624u),    \
      _(1623u), _(1622u), _(1621u), _(1620u), _(1619u), _(1618u), _(1617u), _(1616u), _(1615u),    \
      _(1614u), _(1613u), _(1612u), _(1611u), _(1610u), _(1609u), _(1608u), _(1607u), _(1606u),    \
      _(1605u), _(1604u), _(1603u), _(1602u), _(1601u), _(1600u), _(1599u), _(1598u), _(1597u),    \
      _(1596u), _(1595u), _(1594u), _(1593u), _(1592u), _(1591u), _(1590u), _(1589u), _(1588u),    \
      _(1587u), _(1586u), _(1585u), _(1584u), _(1583u), _(1582u), _(1581u), _(1580u), _(1579u),    \
      _(1578u), _(1577u), _(1576u), _(1575u), _(1574u), _(1573u), _(1572u), _(1571u), _(1570u),    \
      _(1569u), _(1568u), _(1567u), _(1566u), _(1565u), _(1564u), _(1563u), _(1562u), _(1561u),    \
      _(1560u), _(1559u), _(1558u), _(1557u), _(1556u), _(1555u), _(1554u), _(1553u), _(1552u),    \
      _(1551u), _(1550u), _(1549u), _(1548u), _(1547u), _(1546u), _(1545u), _(1544u), _(1543u),    \
      _(1542u), _(1541u), _(1540u), _(1539u), _(1538u), _(1537u), _(1536u), _(1535u), _(1534u),    \
      _(1533u), _(1532u), _(1531u), _(1530u), _(1529u), _(1528u), _(1527u), _(1526u), _(1525u),    \
      _(1524u), _(1523u), _(1522u), _(1521u), _(1520u), _(1519u), _(1518u), _(1517u), _(1516u),    \
      _(1515u), _(1514u), _(1513u), _(1512u), _(1511u), _(1510u), _(1509u), _(1508u), _(1507u),    \
      _(1506u), _(1505u), _(1504u), _(1503u), _(1502u), _(1501u), _(1500u), _(1499u), _(1498u),    \
      _(1497u), _(1496u), _(1495u), _(1494u), _(1493u), _(1492u), _(1491u), _(1490u), _(1489u),    \
      _(1488u), _(1487u), _(1486u), _(1485u), _(1484u), _(1483u), _(1482u), _(1481u), _(1480u),    \
      _(1479u), _(1478u), _(1477u), _(1476u), _(1475u), _(1474u), _(1473u), _(1472u), _(1471u),    \
      _(1470u), _(1469u), _(1468u), _(1467u), _(1466u), _(1465u), _(1464u), _(1463u), _(1462u),    \
      _(1461u), _(1460u), _(1459u), _(1458u), _(1457u), _(1456u), _(1455u), _(1454u), _(1453u),    \
      _(1452u), _(1451u), _(1450u), _(1449u), _(1448u), _(1447u), _(1446u), _(1445u), _(1444u),    \
      _(1443u), _(1442u), _(1441u), _(1440u), _(1439u), _(1438u), _(1437u), _(1436u), _(1435u),    \
      _(1434u), _(1433u), _(1432u), _(1431u), _(1430u), _(1429u), _(1428u), _(1427u), _(1426u),    \
      _(1425u), _(1424u), _(1423u), _(1422u), _(1421u), _(1420u), _(1419u), _(1418u), _(1417u),    \
      _(1416u), _(1415u), _(1414u), _(1413u), _(1412u), _(1411u), _(1410u), _(1409u), _(1408u),    \
      _(1407u), _(1406u), _(1405u), _(1404u), _(1403u), _(1402u), _(1401u), _(1400u), _(1399u),    \
      _(1398u), _(1397u), _(1396u), _(1395u), _(1394u), _(1393u), _(1392u), _(1391u), _(1390u),    \
      _(1389u), _(1388u), _(1387u), _(1386u), _(1385u), _(1384u), _(1383u), _(1382u), _(1381u),    \
      _(1380u), _(1379u), _(1378u), _(1377u), _(1376u), _(1375u), _(1374u), _(1373u), _(1372u),    \
      _(1371u), _(1370u), _(1369u), _(1368u), _(1367u), _(1366u), _(1365u), _(1364u), _(1363u),    \
      _(1362u), _(1361u), _(1360u), _(1359u), _(1358u), _(1357u), _(1356u), _(1355u), _(1354u),    \
      _(1353u), _(1352u), _(1351u), _(1350u), _(1349u), _(1348u), _(1347u), _(1346u), _(1345u),    \
      _(1344u), _(1343u), _(1342u), _(1341u), _(1340u), _(1339u), _(1338u), _(1337u), _(1336u),    \
      _(1335u), _(1334u), _(1333u), _(1332u), _(1331u), _(1330u), _(1329u), _(1328u), _(1327u),    \
      _(1326u), _(1325u), _(1324u), _(1323u), _(1322u), _(1321u), _(1320u), _(1319u), _(1318u),    \
      _(1317u), _(1316u), _(1315u), _(1314u), _(1313u), _(1312u), _(1311u), _(1310u), _(1309u),    \
      _(1308u), _(1307u), _(1306u), _(1305u), _(1304u), _(1303u), _(1302u), _(1301u), _(1300u),    \
      _(1299u), _(1298u), _(1297u), _(1296u), _(1295u), _(1294u), _(1293u), _(1292u), _(1291u),    \
      _(1290u), _(1289u), _(1288u), _(1287u), _(1286u), _(1285u), _(1284u), _(1283u), _(1282u),    \
      _(1281u), _(1280u), _(1279u), _(1278u), _(1277u), _(1276u), _(1275u), _(1274u), _(1273u),    \
      _(1272u), _(1271u), _(1270u), _(1269u), _(1268u), _(1267u), _(1266u), _(1265u), _(1264u),    \
      _(1263u), _(1262u), _(1261u), _(1260u), _(1259u), _(1258u), _(1257u), _(1256u), _(1255u),    \
      _(1254u), _(1253u), _(1252u), _(1251u), _(1250u), _(1249u), _(1248u), _(1247u), _(1246u),    \
      _(1245u), _(1244u), _(1243u), _(1242u), _(1241u), _(1240u), _(1239u), _(1238u), _(1237u),    \
      _(1236u), _(1235u), _(1234u), _(1233u), _(1232u), _(1231u), _(1230u), _(1229u), _(1228u),    \
      _(1227u), _(1226u), _(1225u), _(1224u), _(1223u), _(1222u), _(1221u), _(1220u), _(1219u),    \
      _(1218u), _(1217u), _(1216u), _(1215u), _(1214u), _(1213u), _(1212u), _(1211u), _(1210u),    \
      _(1209u), _(1208u), _(1207u), _(1206u), _(1205u), _(1204u), _(1203u), _(1202u), _(1201u),    \
      _(1200u), _(1199u), _(1198u), _(1197u), _(1196u), _(1195u), _(1194u), _(1193u), _(1192u),    \
      _(1191u), _(1190u), _(1189u), _(1188u), _(1187u), _(1186u), _(1185u), _(1184u), _(1183u),    \
      _(1182u), _(1181u), _(1180u), _(1179u), _(1178u), _(1177u), _(1176u), _(1175u), _(1174u),    \
      _(1173u), _(1172u), _(1171u), _(1170u), _(1169u), _(1168u), _(1167u), _(1166u), _(1165u),    \
      _(1164u), _(1163u), _(1162u), _(1161u), _(1160u), _(1159u), _(1158u), _(1157u), _(1156u),    \
      _(1155u), _(1154u), _(1153u), _(1152u), _(1151u), _(1150u), _(1149u), _(1148u), _(1147u),    \
      _(1146u), _(1145u), _(1144u), _(1143u), _(1142u), _(1141u), _(1140u), _(1139u), _(1138u),    \
      _(1137u), _(1136u), _(1135u), _(1134u), _(1133u), _(1132u), _(1131u), _(1130u), _(1129u),    \
      _(1128u), _(1127u), _(1126u), _(1125u), _(1124u), _(1123u), _(1122u), _(1121u), _(1120u),    \
      _(1119u), _(1118u), _(1117u), _(1116u), _(1115u), _(1114u), _(1113u), _(1112u), _(1111u),    \
      _(1110u), _(1109u), _(1108u), _(1107u), _(1106u), _(1105u), _(1104u), _(1103u), _(1102u),    \
      _(1101u), _(1100u), _(1099u), _(1098u), _(1097u), _(1096u), _(1095u), _(1094u), _(1093u),    \
      _(1092u), _(1091u), _(1090u), _(1089u), _(1088u), _(1087u), _(1086u), _(1085u), _(1084u),    \
      _(1083u), _(1082u), _(1081u), _(1080u), _(1079u), _(1078u), _(1077u), _(1076u), _(1075u),    \
      _(1074u), _(1073u), _(1072u), _(1071u), _(1070u), _(1069u), _(1068u), _(1067u), _(1066u),    \
      _(1065u), _(1064u), _(1063u), _(1062u), _(1061u), _(1060u), _(1059u), _(1058u), _(1057u),    \
      _(1056u), _(1055u), _(1054u), _(1053u), _(1052u), _(1051u), _(1050u), _(1049u), _(1048u),    \
      _(1047u), _(1046u), _(1045u), _(1044u), _(1043u), _(1042u), _(1041u), _(1040u), _(1039u),    \
      _(1038u), _(1037u), _(1036u), _(1035u), _(1034u), _(1033u), _(1032u), _(1031u), _(1030u),    \
      _(1029u), _(1028u), _(1027u), _(1026u), _(1025u), _(1024u), _(1023u), _(1022u), _(1021u),    \
      _(1020u), _(1019u), _(1018u), _(1017u), _(1016u), _(1015u), _(1014u), _(1013u), _(1012u),    \
      _(1011u), _(1010u), _(1009u), _(1008u), _(1007u), _(1006u), _(1005u), _(1004u), _(1003u),    \
      _(1002u), _(1001u), _(1000u), _(999u), _(998u), _(997u), _(996u), _(995u), _(994u), _(993u), \
      _(992u), _(991u), _(990u), _(989u), _(988u), _(987u), _(986u), _(985u), _(984u), _(983u),    \
      _(982u), _(981u), _(980u), _(979u), _(978u), _(977u), _(976u), _(975u), _(974u), _(973u),    \
      _(972u), _(971u), _(970u), _(969u), _(968u), _(967u), _(966u), _(965u), _(964u), _(963u),    \
      _(962u), _(961u), _(960u), _(959u), _(958u), _(957u), _(956u), _(955u), _(954u), _(953u),    \
      _(952u), _(951u), _(950u), _(949u), _(948u), _(947u), _(946u), _(945u), _(944u), _(943u),    \
      _(942u), _(941u), _(940u), _(939u), _(938u), _(937u), _(936u), _(935u), _(934u), _(933u),    \
      _(932u), _(931u), _(930u), _(929u), _(928u), _(927u), _(926u), _(925u), _(924u), _(923u),    \
      _(922u), _(921u), _(920u), _(919u), _(918u), _(917u), _(916u), _(915u), _(914u), _(913u),    \
      _(912u), _(911u), _(910u), _(909u), _(908u), _(907u), _(906u), _(905u), _(904u), _(903u),    \
      _(902u), _(901u), _(900u), _(899u), _(898u), _(897u), _(896u), _(895u), _(894u), _(893u),    \
      _(892u), _(891u), _(890u), _(889u), _(888u), _(887u), _(886u), _(885u), _(884u), _(883u),    \
      _(882u), _(881u), _(880u), _(879u), _(878u), _(877u), _(876u), _(875u), _(874u), _(873u),    \
      _(872u), _(871u), _(870u), _(869u), _(868u), _(867u), _(866u), _(865u), _(864u), _(863u),    \
      _(862u), _(861u), _(860u), _(859u), _(858u), _(857u), _(856u), _(855u), _(854u), _(853u),    \
      _(852u), _(851u), _(850u), _(849u), _(848u), _(847u), _(846u), _(845u), _(844u), _(843u),    \
      _(842u), _(841u), _(840u), _(839u), _(838u), _(837u), _(836u), _(835u), _(834u), _(833u),    \
      _(832u), _(831u), _(830u), _(829u), _(828u), _(827u), _(826u), _(825u), _(824u), _(823u),    \
      _(822u), _(821u), _(820u), _(819u), _(818u), _(817u), _(816u), _(815u), _(814u), _(813u),    \
      _(812u), _(811u), _(810u), _(809u), _(808u), _(807u), _(806u), _(805u), _(804u), _(803u),    \
      _(802u), _(801u), _(800u), _(799u), _(798u), _(797u), _(796u), _(795u), _(794u), _(793u),    \
      _(792u), _(791u), _(790u), _(789u), _(788u), _(787u), _(786u), _(785u), _(784u), _(783u),    \
      _(782u), _(781u), _(780u), _(779u), _(778u), _(777u), _(776u), _(775u), _(774u), _(773u),    \
      _(772u), _(771u), _(770u), _(769u), _(768u), _(767u), _(766u), _(765u), _(764u), _(763u),    \
      _(762u), _(761u), _(760u), _(759u), _(758u), _(757u), _(756u), _(755u), _(754u), _(753u),    \
      _(752u), _(751u), _(750u), _(749u), _(748u), _(747u), _(746u), _(745u), _(744u), _(743u),    \
      _(742u), _(741u), _(740u), _(739u), _(738u), _(737u), _(736u), _(735u), _(734u), _(733u),    \
      _(732u), _(731u), _(730u), _(729u), _(728u), _(727u), _(726u), _(725u), _(724u), _(723u),    \
      _(722u), _(721u), _(720u), _(719u), _(718u), _(717u), _(716u), _(715u), _(714u), _(713u),    \
      _(712u), _(711u), _(710u), _(709u), _(708u), _(707u), _(706u), _(705u), _(704u), _(703u),    \
      _(702u), _(701u), _(700u), _(699u), _(698u), _(697u), _(696u), _(695u), _(694u), _(693u),    \
      _(692u), _(691u), _(690u), _(689u), _(688u), _(687u), _(686u), _(685u), _(684u), _(683u),    \
      _(682u), _(681u), _(680u), _(679u), _(678u), _(677u), _(676u), _(675u), _(674u), _(673u),    \
      _(672u), _(671u), _(670u), _(669u), _(668u), _(667u), _(666u), _(665u), _(664u), _(663u),    \
      _(662u), _(661u), _(660u), _(659u), _(658u), _(657u), _(656u), _(655u), _(654u), _(653u),    \
      _(652u), _(651u), _(650u), _(649u), _(648u), _(647u), _(646u), _(645u), _(644u), _(643u),    \
      _(642u), _(641u), _(640u), _(639u), _(638u), _(637u), _(636u), _(635u), _(634u), _(633u),    \
      _(632u), _(631u), _(630u), _(629u), _(628u), _(627u), _(626u), _(625u), _(624u), _(623u),    \
      _(622u), _(621u), _(620u), _(619u), _(618u), _(617u), _(616u), _(615u), _(614u), _(613u),    \
      _(612u), _(611u), _(610u), _(609u), _(608u), _(607u), _(606u), _(605u), _(604u), _(603u),    \
      _(602u), _(601u), _(600u), _(599u), _(598u), _(597u), _(596u), _(595u), _(594u), _(593u),    \
      _(592u), _(591u), _(590u), _(589u), _(588u), _(587u), _(586u), _(585u), _(584u), _(583u),    \
      _(582u), _(581u), _(580u), _(579u), _(578u), _(577u), _(576u), _(575u), _(574u), _(573u),    \
      _(572u), _(571u), _(570u), _(569u), _(568u), _(567u), _(566u), _(565u), _(564u), _(563u),    \
      _(562u), _(561u), _(560u), _(559u), _(558u), _(557u), _(556u), _(555u), _(554u), _(553u),    \
      _(552u), _(551u), _(550u), _(549u), _(548u), _(547u), _(546u), _(545u), _(544u), _(543u),    \
      _(542u), _(541u), _(540u), _(539u), _(538u), _(537u), _(536u), _(535u), _(534u), _(533u),    \
      _(532u), _(531u), _(530u), _(529u), _(528u), _(527u), _(526u), _(525u), _(524u), _(523u),    \
      _(522u), _(521u), _(520u), _(519u), _(518u), _(517u), _(516u), _(515u), _(514u), _(513u),    \
      _(512u), _(511u), _(510u), _(509u), _(508u), _(507u), _(506u), _(505u), _(504u), _(503u),    \
      _(502u), _(501u), _(500u), _(499u), _(498u), _(497u), _(496u), _(495u), _(494u), _(493u),    \
      _(492u), _(491u), _(490u), _(489u), _(488u), _(487u), _(486u), _(485u), _(484u), _(483u),    \
      _(482u), _(481u), _(480u), _(479u), _(478u), _(477u), _(476u), _(475u), _(474u), _(473u),    \
      _(472u), _(471u), _(470u), _(469u), _(468u), _(467u), _(466u), _(465u), _(464u), _(463u),    \
      _(462u), _(461u), _(460u), _(459u), _(458u), _(457u), _(456u), _(455u), _(454u), _(453u),    \
      _(452u), _(451u), _(450u), _(449u), _(448u), _(447u), _(446u), _(445u), _(444u), _(443u),    \
      _(442u), _(441u), _(440u), _(439u), _(438u), _(437u), _(436u), _(435u), _(434u), _(433u),    \
      _(432u), _(431u), _(430u), _(429u), _(428u), _(427u), _(426u), _(425u), _(424u), _(423u),    \
      _(422u), _(421u), _(420u), _(419u), _(418u), _(417u), _(416u), _(415u), _(414u), _(413u),    \
      _(412u), _(411u), _(410u), _(409u), _(408u), _(407u), _(406u), _(405u), _(404u), _(403u),    \
      _(402u), _(401u), _(400u), _(399u), _(398u), _(397u), _(396u), _(395u), _(394u), _(393u),    \
      _(392u), _(391u), _(390u), _(389u), _(388u), _(387u), _(386u), _(385u), _(384u), _(383u),    \
      _(382u), _(381u), _(380u), _(379u), _(378u), _(377u), _(376u), _(375u), _(374u), _(373u),    \
      _(372u), _(371u), _(370u), _(369u), _(368u), _(367u), _(366u), _(365u), _(364u), _(363u),    \
      _(362u), _(361u), _(360u), _(359u), _(358u), _(357u), _(356u), _(355u), _(354u), _(353u),    \
      _(352u), _(351u), _(350u), _(349u), _(348u), _(347u), _(346u), _(345u), _(344u), _(343u),    \
      _(342u), _(341u), _(340u), _(339u), _(338u), _(337u), _(336u), _(335u), _(334u), _(333u),    \
      _(332u), _(331u), _(330u), _(329u), _(328u), _(327u), _(326u), _(325u), _(324u), _(323u),    \
      _(322u), _(321u), _(320u), _(319u), _(318u), _(317u), _(316u), _(315u), _(314u), _(313u),    \
      _(312u), _(311u), _(310u), _(309u), _(308u), _(307u), _(306u), _(305u), _(304u), _(303u),    \
      _(302u), _(301u), _(300u), _(299u), _(298u), _(297u), _(296u), _(295u), _(294u), _(293u),    \
      _(292u), _(291u), _(290u), _(289u), _(288u), _(287u), _(286u), _(285u), _(284u), _(283u),    \
      _(282u), _(281u), _(280u), _(279u), _(278u), _(277u), _(276u), _(275u), _(274u), _(273u),    \
      _(272u), _(271u), _(270u), _(269u), _(268u), _(267u), _(266u), _(265u), _(264u), _(263u),    \
      _(262u), _(261u), _(260u), _(259u), _(258u), _(257u), _(256u), _(255u), _(254u), _(253u),    \
      _(252u), _(251u), _(250u), _(249u), _(248u), _(247u), _(246u), _(245u), _(244u), _(243u),    \
      _(242u), _(241u), _(240u), _(239u), _(238u), _(237u), _(236u), _(235u), _(234u), _(233u),    \
      _(232u), _(231u), _(230u), _(229u), _(228u), _(227u), _(226u), _(225u), _(224u), _(223u),    \
      _(222u), _(221u), _(220u), _(219u), _(218u), _(217u), _(216u), _(215u), _(214u), _(213u),    \
      _(212u), _(211u), _(210u), _(209u), _(208u), _(207u), _(206u), _(205u), _(204u), _(203u),    \
      _(202u), _(201u), _(200u), _(199u), _(198u), _(197u), _(196u), _(195u), _(194u), _(193u),    \
      _(192u), _(191u), _(190u), _(189u), _(188u), _(187u), _(186u), _(185u), _(184u), _(183u),    \
      _(182u), _(181u), _(180u), _(179u), _(178u), _(177u), _(176u), _(175u), _(174u), _(173u),    \
      _(172u), _(171u), _(170u), _(169u), _(168u), _(167u), _(166u), _(165u), _(164u), _(163u),    \
      _(162u), _(161u), _(160u), _(159u), _(158u), _(157u), _(156u), _(155u), _(154u), _(153u),    \
      _(152u), _(151u), _(150u), _(149u), _(148u), _(147u), _(146u), _(145u), _(144u), _(143u),    \
      _(142u), _(141u), _(140u), _(139u), _(138u), _(137u), _(136u), _(135u), _(134u), _(133u),    \
      _(132u), _(131u), _(130u), _(129u), _(128u), _(127u), _(126u), _(125u), _(124u), _(123u),    \
      _(122u), _(121u), _(120u), _(119u), _(118u), _(117u), _(116u), _(115u), _(114u), _(113u),    \
      _(112u), _(111u), _(110u), _(109u), _(108u), _(107u), _(106u), _(105u), _(104u), _(103u),    \
      _(102u), _(101u), _(100u), _(99u), _(98u), _(97u), _(96u), _(95u), _(94u), _(93u), _(92u),   \
      _(91u), _(90u), _(89u), _(88u), _(87u), _(86u), _(85u), _(84u), _(83u), _(82u), _(81u),      \
      _(80u), _(79u), _(78u), _(77u), _(76u), _(75u), _(74u), _(73u), _(72u), _(71u), _(70u),      \
      _(69u), _(68u), _(67u), _(66u), _(65u), _(64u), _(63u), _(62u), _(61u), _(60u), _(59u),      \
      _(58u), _(57u), _(56u), _(55u), _(54u), _(53u), _(52u), _(51u), _(50u), _(49u), _(48u),      \
      _(47u), _(46u), _(45u), _(44u), _(43u), _(42u), _(41u), _(40u), _(39u), _(38u), _(37u),      \
      _(36u), _(35u), _(34u), _(33u), _(32u), _(31u), _(30u), _(29u), _(28u), _(27u), _(26u),      \
      _(25u), _(24u), _(23u), _(22u), _(21u), _(20u), _(19u), _(18u), _(17u), _(16u), _(15u),      \
      _(14u), _(13u), _(12u), _(11u), _(10u), _(9u), _(8u), _(7u), _(6u), _(5u), _(4u), _(3u),     \
      _(2u), _(1u), _(0u))
#define PPUTLSIZE_READ(                                                                            \
    _err, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, A, B, C,   \
    D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, ba, bb, bc, bd, be, bf,   \
    bg, bh, bi, bj, bk, bl, bm, bn, bo, bp, bq, br, bs, bt, bu, bv, bw, bx, by, bz, bA, bB, bC,    \
    bD, bE, bF, bG, bH, bI, bJ, bK, bL, bM, bN, bO, bP, bQ, bR, bS, bT, bU, bV, bW, bX, bY, bZ,    \
    ca, cb, cc, cd, ce, cf, cg, ch, ci, cj, ck, cl, cm, cn, co, cp, cq, cr, cs, ct, cu, cv, cw,    \
    cx, cy, cz, cA, cB, cC, cD, cE, cF, cG, cH, cI, cJ, cK, cL, cM, cN, cO, cP, cQ, cR, cS, cT,    \
    cU, cV, cW, cX, cY, cZ, da, db, dc, dd, de, df, dg, dh, di, dj, dk, dl, dm, dn, do, dp, dq,    \
    dr, ds, dt, du, dv, dw, dx, dy, dz, dA, dB, dC, dD, dE, dF, dG, dH, dI, dJ, dK, dL, dM, dN,    \
    dO, dP, dQ, dR, dS, dT, dU, dV, dW, dX, dY, dZ, ea, eb, ec, ed, ee, ef, eg, eh, ei, ej, ek,    \
    el, em, en, eo, ep, eq, er, es, et, eu, ev, ew, ex, ey, ez, eA, eB, eC, eD, eE, eF, eG, eH,    \
    eI, eJ, eK, eL, eM, eN, eO, eP, eQ, eR, eS, eT, eU, eV, eW, eX, eY, eZ, fa, fb, fc, fd, fe,    \
    ff, fg, fh, fi, fj, fk, fl, fm, fn, fo, fp, fq, fr, fs, ft, fu, fv, fw, fx, fy, fz, fA, fB,    \
    fC, fD, fE, fF, fG, fH, fI, fJ, fK, fL, fM, fN, fO, fP, fQ, fR, fS, fT, fU, fV, fW, fX, fY,    \
    fZ, ga, gb, gc, gd, ge, gf, gg, gh, gi, gj, gk, gl, gm, gn, go, gp, gq, gr, gs, gt, gu, gv,    \
    gw, gx, gy, gz, gA, gB, gC, gD, gE, gF, gG, gH, gI, gJ, gK, gL, gM, gN, gO, gP, gQ, gR, gS,    \
    gT, gU, gV, gW, gX, gY, gZ, ha, hb, hc, hd, he, hf, hg, hh, hi, hj, hk, hl, hm, hn, ho, hp,    \
    hq, hr, hs, ht, hu, hv, hw, hx, hy, hz, hA, hB, hC, hD, hE, hF, hG, hH, hI, hJ, hK, hL, hM,    \
    hN, hO, hP, hQ, hR, hS, hT, hU, hV, hW, hX, hY, hZ, ia, ib, ic, id, ie, if, ig, ih, ii, ij,    \
    ik, il, im, in, io, ip, iq, ir, is, it, iu, iv, iw, ix, iy, iz, iA, iB, iC, iD, iE, iF, iG,    \
    iH, iI, iJ, iK, iL, iM, iN, iO, iP, iQ, iR, iS, iT, iU, iV, iW, iX, iY, iZ, ja, jb, jc, jd,    \
    je, jf, jg, jh, ji, jj, jk, jl, jm, jn, jo, jp, jq, jr, js, jt, ju, jv, jw, jx, jy, jz, jA,    \
    jB, jC, jD, jE, jF, jG, jH, jI, jJ, jK, jL, jM, jN, jO, jP, jQ, jR, jS, jT, jU, jV, jW, jX,    \
    jY, jZ, ka, kb, kc, kd, ke, kf, kg, kh, ki, kj, kk, kl, km, kn, ko, kp, kq, kr, ks, kt, ku,    \
    kv, kw, kx, ky, kz, kA, kB, kC, kD, kE, kF, kG, kH, kI, kJ, kK, kL, kM, kN, kO, kP, kQ, kR,    \
    kS, kT, kU, kV, kW, kX, kY, kZ, la, lb, lc, ld, le, lf, lg, lh, li, lj, lk, ll, lm, ln, lo,    \
    lp, lq, lr, ls, lt, lu, lv, lw, lx, ly, lz, lA, lB, lC, lD, lE, lF, lG, lH, lI, lJ, lK, lL,    \
    lM, lN, lO, lP, lQ, lR, lS, lT, lU, lV, lW, lX, lY, lZ, ma, mb, mc, md, me, mf, mg, mh, mi,    \
    mj, mk, ml, mm, mn, mo, mp, mq, mr, ms, mt, mu, mv, mw, mx, my, mz, mA, mB, mC, mD, mE, mF,    \
    mG, mH, mI, mJ, mK, mL, mM, mN, mO, mP, mQ, mR, mS, mT, mU, mV, mW, mX, mY, mZ, na, nb, nc,    \
    nd, ne, nf, ng, nh, ni, nj, nk, nl, nm, nn, no, np, nq, nr, ns, nt, nu, nv, nw, nx, ny, nz,    \
    nA, nB, nC, nD, nE, nF, nG, nH, nI, nJ, nK, nL, nM, nN, nO, nP, nQ, nR, nS, nT, nU, nV, nW,    \
    nX, nY, nZ, oa, ob, oc, od, oe, of, og, oh, oi, oj, ok, ol, om, on, oo, op, oq, _or, os, ot,   \
    ou, ov, ow, ox, oy, oz, oA, oB, oC, oD, oE, oF, oG, oH, oI, oJ, oK, oL, oM, oN, oO, oP, oQ,    \
    oR, oS, oT, oU, oV, oW, oX, oY, oZ, pa, pb, pc, pd, pe, pf, pg, ph, pi, pj, pk, pl, pm, pn,    \
    po, pp, pq, pr, ps, pt, pu, pv, pw, px, py, pz, pA, pB, pC, pD, pE, pF, pG, pH, pI, pJ, pK,    \
    pL, pM, pN, pO, pP, pQ, pR, pS, pT, pU, pV, pW, pX, pY, pZ, qa, qb, qc, qd, qe, qf, qg, qh,    \
    qi, qj, qk, ql, qm, qn, qo, qp, qq, qr, qs, qt, qu, qv, qw, qx, qy, qz, qA, qB, qC, qD, qE,    \
    qF, qG, qH, qI, qJ, qK, qL, qM, qN, qO, qP, qQ, qR, qS, qT, qU, qV, qW, qX, qY, qZ, ra, rb,    \
    rc, rd, re, rf, rg, rh, ri, rj, rk, rl, rm, rn, ro, rp, rq, rr, rs, rt, ru, rv, rw, rx, ry,    \
    rz, rA, rB, rC, rD, rE, rF, rG, rH, rI, rJ, rK, rL, rM, rN, rO, rP, rQ, rR, rS, rT, rU, rV,    \
    rW, rX, rY, rZ, sa, sb, sc, sd, se, sf, sg, sh, si, sj, sk, sl, sm, sn, so, sp, sq, sr, ss,    \
    st, su, sv, sw, sx, sy, sz, sA, sB, sC, sD, sE, sF, sG, sH, sI, sJ, sK, sL, sM, sN, sO, sP,    \
    sQ, sR, sS, sT, sU, sV, sW, sX, sY, sZ, ta, tb, tc, td, te, tf, tg, th, ti, tj, tk, tl, tm,    \
    tn, to, tp, tq, tr, ts, tt, tu, tv, tw, tx, ty, tz, tA, tB, tC, tD, tE, tF, tG, tH, tI, tJ,    \
    tK, tL, tM, tN, tO, tP, tQ, tR, tS, tT, tU, tV, tW, tX, tY, tZ, ua, ub, uc, ud, ue, uf, ug,    \
    uh, ui, uj, uk, ul, um, un, uo, up, uq, ur, us, ut, uu, uv, uw, ux, uy, uz, uA, uB, uC, uD,    \
    uE, uF, uG, uH, uI, uJ, uK, uL, uM, uN, uO, uP, uQ, uR, uS, uT, uU, uV, uW, uX, uY, uZ, va,    \
    vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl, vm, vn, vo, vp, vq, vr, vs, vt, vu, vv, vw, vx,    \
    vy, vz, vA, vB, vC, vD, vE, vF, vG, vH, vI, vJ, vK, vL, vM, vN, vO, vP, vQ, vR, vS, vT, vU,    \
    vV, vW, vX, vY, vZ, wa, wb, wc, wd, we, wf, wg, wh, wi, wj, wk, wl, wm, wn, wo, wp, wq, wr,    \
    ws, wt, wu, wv, ww, wx, wy, wz, wA, wB, wC, wD, wE, wF, wG, wH, wI, wJ, wK, wL, wM, wN, wO,    \
    wP, wQ, wR, wS, wT, wU, wV, wW, wX, wY, wZ, xa, xb, xc, xd, xe, xf, xg, xh, xi, xj, xk, xl,    \
    xm, xn, xo, xp, xq, xr, xs, xt, xu, xv, xw, xx, xy, xz, xA, xB, xC, xD, xE, xF, xG, xH, xI,    \
    xJ, xK, xL, xM, xN, xO, xP, xQ, xR, xS, xT, xU, xV, xW, xX, xY, xZ, ya, yb, yc, yd, ye, yf,    \
    yg, yh, yi, yj, yk, yl, ym, yn, yo, yp, yq, yr, ys, yt, yu, yv, yw, yx, yy, yz, yA, yB, yC,    \
    yD, yE, yF, yG, yH, yI, yJ, yK, yL, yM, yN, yO, yP, yQ, yR, yS, yT, yU, yV, yW, yX, yY, yZ,    \
    za, zb, zc, zd, ze, zf, zg, zh, zi, zj, zk, zl, zm, zn, zo, zp, zq, zr, zs, zt, zu, zv, zw,    \
    zx, zy, zz, zA, zB, zC, zD, zE, zF, zG, zH, zI, zJ, zK, zL, zM, zN, zO, zP, zQ, zR, zS, zT,    \
    zU, zV, zW, zX, zY, zZ, Aa, Ab, Ac, Ad, Ae, Af, Ag, Ah, Ai, Aj, Ak, Al, Am, An, Ao, Ap, Aq,    \
    Ar, As, At, Au, Av, Aw, Ax, Ay, Az, AA, AB, AC, AD, AE, AF, AG, AH, AI, AJ, AK, AL, AM, AN,    \
    AO, AP, AQ, AR, AS, AT, AU, AV, AW, AX, AY, AZ, Ba, Bb, Bc, Bd, Be, Bf, Bg, Bh, Bi, Bj, Bk,    \
    Bl, Bm, Bn, Bo, Bp, Bq, Br, Bs, Bt, Bu, Bv, Bw, Bx, By, Bz, BA, BB, BC, BD, BE, BF, BG, BH,    \
    BI, BJ, BK, BL, BM, BN, BO, BP, BQ, BR, BS, BT, BU, BV, BW, BX, BY, BZ, Ca, Cb, Cc, Cd, Ce,    \
    Cf, Cg, Ch, Ci, Cj, Ck, Cl, Cm, Cn, Co, Cp, Cq, Cr, Cs, Ct, Cu, Cv, Cw, Cx, Cy, Cz, CA, CB,    \
    CC, CD, CE, CF, CG, CH, CI, CJ, CK, CL, CM, CN, CO, CP, CQ, CR, CS, CT, CU, CV, CW, CX, CY,    \
    CZ, Da, Db, Dc, Dd, De, Df, Dg, Dh, Di, Dj, Dk, Dl, Dm, Dn, Do, Dp, Dq, Dr, Ds, Dt, Du, Dv,    \
    Dw, Dx, Dy, Dz, DA, DB, DC, DD, DE, DF, DG, DH, DI, DJ, DK, DL, DM, DN, DO, DP, DQ, DR, DS,    \
    DT, DU, DV, DW, DX, DY, DZ, Ea, Eb, Ec, Ed, Ee, Ef, Eg, Eh, Ei, Ej, Ek, El, Em, En, Eo, Ep,    \
    Eq, Er, Es, Et, Eu, Ev, Ew, Ex, Ey, Ez, EA, EB, EC, ED, EE, EF, EG, EH, EI, EJ, EK, EL, EM,    \
    EN, EO, EP, EQ, ER, ES, ET, EU, EV, EW, EX, EY, EZ, Fa, Fb, Fc, Fd, Fe, Ff, Fg, Fh, Fi, Fj,    \
    Fk, Fl, Fm, Fn, Fo, Fp, Fq, Fr, Fs, Ft, Fu, Fv, Fw, Fx, Fy, Fz, FA, FB, FC, FD, FE, FF, FG,    \
    FH, FI, FJ, FK, FL, FM, FN, FO, FP, FQ, FR, FS, FT, FU, FV, FW, FX, FY, FZ, Ga, Gb, Gc, Gd,    \
    Ge, Gf, Gg, Gh, Gi, Gj, Gk, Gl, Gm, Gn, Go, Gp, Gq, Gr, Gs, Gt, Gu, Gv, Gw, Gx, Gy, Gz, GA,    \
    GB, GC, GD, GE, GF, GG, GH, GI, GJ, GK, GL, GM, GN, GO, GP, GQ, GR, GS, GT, GU, GV, GW, GX,    \
    GY, GZ, Ha, Hb, Hc, Hd, He, Hf, Hg, Hh, Hi, Hj, Hk, Hl, Hm, Hn, Ho, Hp, Hq, Hr, Hs, Ht, Hu,    \
    Hv, Hw, Hx, Hy, Hz, HA, HB, HC, HD, HE, HF, HG, HH, HI, HJ, HK, HL, HM, HN, HO, HP, HQ, HR,    \
    HS, HT, HU, HV, HW, HX, HY, HZ, Ia, Ib, Ic, Id, Ie, If, Ig, Ih, Ii, Ij, Ik, Il, Im, In, Io,    \
    Ip, Iq, Ir, Is, It, Iu, Iv, Iw, Ix, Iy, Iz, IA, IB, IC, ID, IE, IF, IG, IH, II, IJ, IK, IL,    \
    IM, IN, IO, IP, IQ, IR, IS, IT, IU, IV, IW, IX, IY, IZ, Ja, Jb, Jc, Jd, Je, Jf, Jg, Jh, Ji,    \
    Jj, Jk, Jl, Jm, Jn, Jo, Jp, Jq, Jr, Js, Jt, Ju, Jv, Jw, Jx, Jy, Jz, JA, JB, JC, JD, JE, JF,    \
    JG, JH, JI, JJ, JK, JL, JM, JN, JO, JP, JQ, JR, JS, JT, JU, JV, JW, JX, JY, JZ, Ka, Kb, Kc,    \
    Kd, Ke, Kf, Kg, Kh, Ki, Kj, Kk, Kl, Km, Kn, Ko, Kp, Kq, Kr, Ks, Kt, Ku, Kv, Kw, Kx, Ky, Kz,    \
    KA, KB, KC, KD, KE, KF, KG, KH, KI, KJ, KK, KL, KM, KN, KO, KP, KQ, KR, KS, KT, KU, KV, KW,    \
    KX, KY, KZ, La, Lb, Lc, Ld, Le, Lf, Lg, Lh, Li, Lj, Lk, Ll, Lm, Ln, Lo, Lp, Lq, Lr, Ls, Lt,    \
    Lu, Lv, Lw, Lx, Ly, Lz, LA, LB, LC, LD, LE, LF, LG, LH, LI, LJ, LK, LL, LM, LN, LO, LP, LQ,    \
    LR, LS, LT, LU, LV, LW, LX, LY, LZ, Ma, Mb, Mc, Md, Me, Mf, Mg, Mh, Mi, Mj, Mk, Ml, Mm, Mn,    \
    Mo, Mp, Mq, Mr, Ms, Mt, Mu, Mv, Mw, Mx, My, Mz, MA, MB, MC, MD, ME, MF, MG, MH, MI, MJ, MK,    \
    ML, MM, MN, MO, MP, MQ, MR, MS, MT, MU, MV, MW, MX, MY, MZ, Na, Nb, Nc, Nd, Ne, Nf, Ng, Nh,    \
    Ni, Nj, Nk, Nl, Nm, Nn, No, Np, Nq, Nr, Ns, Nt, Nu, Nv, Nw, Nx, Ny, Nz, NA, NB, NC, ND, NE,    \
    NF, NG, NH, NI, NJ, NK, NL, NM, NN, NO, NP, NQ, NR, NS, NT, NU, NV, NW, NX, NY, NZ, Oa, Ob,    \
    Oc, Od, Oe, Of, Og, Oh, Oi, Oj, Ok, Ol, Om, On, Oo, Op, Oq, Or, Os, Ot, Ou, Ov, Ow, Ox, Oy,    \
    Oz, OA, OB, OC, OD, OE, OF, OG, OH, OI, OJ, OK, OL, OM, ON, OO, OP, OQ, OR, OS, OT, OU, OV,    \
    OW, OX, OY, OZ, Pa, Pb, Pc, Pd, Pe, Pf, Pg, Ph, Pi, Pj, Pk, Pl, Pm, Pn, Po, Pp, Pq, Pr, Ps,    \
    Pt, Pu, Pv, Pw, Px, Py, Pz, PA, PB, PC, PD, PE, PF, PG, PH, PI, PJ, PK, PL, PM, PN, PO, PP,    \
    PQ, PR, PS, PT, PU, PV, PW, PX, PY, PZ, Qa, Qb, Qc, Qd, Qe, Qf, Qg, Qh, Qi, Qj, Qk, Ql, Qm,    \
    Qn, Qo, Qp, Qq, Qr, Qs, Qt, Qu, Qv, Qw, Qx, Qy, Qz, QA, QB, QC, QD, QE, QF, QG, QH, QI, QJ,    \
    QK, QL, QM, QN, QO, QP, QQ, QR, QS, QT, QU, QV, QW, QX, QY, QZ, Ra, Rb, Rc, Rd, Re, Rf, Rg,    \
    Rh, Ri, Rj, Rk, Rl, Rm, Rn, Ro, Rp, Rq, Rr, Rs, Rt, Ru, Rv, Rw, Rx, Ry, Rz, RA, RB, RC, RD,    \
    RE, RF, RG, RH, RI, RJ, RK, RL, RM, RN, RO, RP, RQ, RR, RS, RT, RU, RV, RW, RX, RY, RZ, Sa,    \
    Sb, Sc, Sd, Se, Sf, Sg, Sh, Si, Sj, Sk, Sl, Sm, Sn, So, Sp, Sq, Sr, Ss, St, Su, Sv, Sw, Sx,    \
    Sy, Sz, SA, SB, SC, SD, SE, SF, SG, SH, SI, SJ, SK, SL, SM, SN, SO, SP, SQ, SR, SS, ST, SU,    \
    SV, SW, SX, SY, SZ, Ta, Tb, Tc, Td, Te, Tf, Tg, Th, Ti, Tj, Tk, Tl, Tm, Tn, To, Tp, Tq, Tr,    \
    Ts, Tt, Tu, Tv, Tw, Tx, Ty, Tz, TA, TB, TC, TD, TE, TF, TG, TH, TI, TJ, TK, TL, TM, TN, TO,    \
    TP, TQ, TR, TS, TT, TU, TV, TW, TX, TY, TZ, Ua, Ub, Uc, Ud, Ue, Uf, Ug, Uh, Ui, Uj, Uk, Ul,    \
    Um, Un, Uo, Up, Uq, Ur, Us, Ut, Uu, Uv, Uw, Ux, Uy, Uz, UA, UB, UC, UD, UE, UF, UG, UH, UI,    \
    UJ, UK, UL, UM, UN, UO, UP, UQ, UR, US, UT, UU, UV, UW, UX, UY, UZ, Va, Vb, Vc, Vd, Ve, Vf,    \
    Vg, Vh, Vi, Vj, Vk, Vl, Vm, Vn, Vo, Vp, Vq, Vr, Vs, Vt, Vu, Vv, Vw, Vx, Vy, Vz, VA, VB, VC,    \
    VD, VE, VF, VG, VH, VI, VJ, VK, VL, VM, VN, VO, VP, VQ, VR, VS, VT, VU, VV, VW, VX, VY, VZ,    \
    Wa, Wb, Wc, Wd, We, Wf, Wg, Wh, Wi, Wj, Wk, Wl, Wm, Wn, Wo, Wp, Wq, Wr, Ws, Wt, Wu, Wv, Ww,    \
    Wx, Wy, Wz, WA, WB, WC, WD, WE, WF, WG, WH, WI, WJ, WK, WL, WM, WN, WO, WP, WQ, WR, WS, WT,    \
    WU, WV, WW, WX, WY, WZ, Xa, Xb, Xc, Xd, Xe, Xf, Xg, Xh, Xi, Xj, Xk, Xl, Xm, Xn, Xo, Xp, Xq,    \
    Xr, Xs, Xt, Xu, Xv, Xw, Xx, Xy, Xz, XA, XB, XC, XD, XE, XF, XG, XH, XI, XJ, XK, XL, XM, XN,    \
    XO, XP, XQ, XR, XS, XT, XU, XV, XW, XX, XY, XZ, Ya, Yb, Yc, Yd, Ye, Yf, Yg, Yh, Yi, Yj, Yk,    \
    Yl, Ym, Yn, Yo, Yp, Yq, Yr, Ys, Yt, Yu, Yv, Yw, Yx, Yy, Yz, YA, YB, YC, YD, YE, YF, YG, YH,    \
    YI, YJ, YK, YL, YM, YN, YO, YP, YQ, YR, YS, YT, YU, YV, YW, YX, YY, YZ, Za, Zb, Zc, Zd, Ze,    \
    Zf, Zg, Zh, Zi, Zj, Zk, Zl, Zm, Zn, Zo, Zp, Zq, Zr, Zs, Zt, Zu, Zv, Zw, Zx, Zy, Zz, ZA, ZB,    \
    ZC, ZD, ZE, ZF, ZG, ZH, ZI, ZJ, ZK, ZL, ZM, ZN, ZO, ZP, ZQ, ZR, ZS, ZT, ZU, ZV, ZW, ZX, ZY,    \
    ZZ, baa, bab, bac, bad, bae, baf, bag, bah, bai, baj, bak, bal, bam, ban, bao, bap, baq, bar,  \
    bas, bat, bau, bav, baw, bax, bay, baz, baA, baB, baC, baD, baE, baF, baG, baH, baI, baJ, baK, \
    baL, baM, baN, baO, baP, baQ, baR, baS, baT, baU, baV, baW, baX, baY, baZ, bba, bbb, bbc, bbd, \
    bbe, bbf, bbg, bbh, bbi, bbj, bbk, bbl, bbm, bbn, bbo, bbp, bbq, bbr, bbs, bbt, bbu, bbv, bbw, \
    bbx, bby, bbz, bbA, bbB, bbC, bbD, bbE, bbF, bbG, bbH, bbI, bbJ, bbK, bbL, bbM, bbN, bbO, bbP, \
    bbQ, bbR, bbS, bbT, bbU, bbV, bbW, bbX, bbY, bbZ, bca, bcb, bcc, bcd, bce, bcf, bcg, bch, bci, \
    bcj, bck, bcl, bcm, bcn, bco, bcp, bcq, bcr, bcs, bct, bcu, bcv, bcw, bcx, bcy, bcz, bcA, bcB, \
    bcC, bcD, bcE, bcF, bcG, bcH, bcI, bcJ, bcK, bcL, bcM, bcN, bcO, bcP, bcQ, bcR, bcS, bcT, bcU, \
    bcV, bcW, bcX, bcY, bcZ, bda, bdb, bdc, bdd, bde, bdf, bdg, bdh, bdi, bdj, bdk, bdl, bdm, bdn, \
    bdo, bdp, bdq, bdr, bds, bdt, bdu, bdv, bdw, bdx, bdy, bdz, bdA, bdB, bdC, bdD, bdE, bdF, bdG, \
    bdH, bdI, bdJ, bdK, bdL, bdM, bdN, bdO, bdP, bdQ, bdR, bdS, bdT, bdU, bdV, bdW, bdX, bdY, bdZ, \
    bea, beb, bec, bed, bee, bef, beg, beh, bei, bej, bek, bel, bem, ben, beo, bep, beq, ber, bes, \
    bet, beu, bev, bew, bex, bey, bez, beA, beB, beC, beD, beE, beF, beG, beH, beI, beJ, beK, beL, \
    beM, beN, beO, beP, beQ, beR, beS, beT, beU, beV, beW, beX, beY, beZ, bfa, bfb, bfc, bfd, bfe, \
    bff, bfg, bfh, bfi, bfj, bfk, bfl, bfm, bfn, bfo, bfp, bfq, bfr, bfs, bft, bfu, bfv, bfw, bfx, \
    bfy, bfz, bfA, bfB, bfC, bfD, bfE, bfF, bfG, bfH, bfI, bfJ, bfK, bfL, bfM, bfN, bfO, bfP, bfQ, \
    bfR, bfS, bfT, bfU, bfV, bfW, bfX, bfY, bfZ, bga, bgb, bgc, bgd, bge, bgf, bgg, bgh, bgi, bgj, \
    bgk, bgl, bgm, bgn, bgo, bgp, bgq, bgr, bgs, bgt, bgu, bgv, bgw, bgx, bgy, bgz, bgA, bgB, bgC, \
    bgD, bgE, bgF, bgG, bgH, bgI, bgJ, bgK, bgL, bgM, bgN, bgO, bgP, bgQ, bgR, bgS, bgT, bgU, bgV, \
    bgW, bgX, bgY, bgZ, bha, bhb, bhc, bhd, bhe, bhf, bhg, bhh, bhi, bhj, bhk, bhl, bhm, bhn, bho, \
    bhp, bhq, bhr, bhs, bht, bhu, bhv, bhw, bhx, bhy, bhz, bhA, bhB, bhC, bhD, bhE, bhF, bhG, bhH, \
    bhI, bhJ, bhK, bhL, bhM, bhN, bhO, bhP, bhQ, bhR, bhS, bhT, bhU, bhV, bhW, bhX, bhY, bhZ, bia, \
    bib, bic, bid, bie, bif, big, bih, bii, bij, bik, bil, bim, bin, bio, bip, biq, bir, bis, bit, \
    biu, biv, biw, bix, biy, biz, biA, biB, biC, biD, biE, biF, biG, biH, biI, biJ, biK, biL, biM, \
    biN, biO, biP, biQ, biR, biS, biT, biU, biV, biW, biX, biY, biZ, bja, bjb, bjc, bjd, bje, bjf, \
    bjg, bjh, bji, bjj, bjk, bjl, bjm, bjn, bjo, bjp, bjq, bjr, bjs, bjt, bju, bjv, bjw, bjx, bjy, \
    bjz, bjA, bjB, bjC, bjD, bjE, bjF, bjG, bjH, bjI, bjJ, bjK, bjL, bjM, bjN, bjO, bjP, bjQ, bjR, \
    bjS, bjT, bjU, bjV, bjW, bjX, bjY, bjZ, bka, bkb, bkc, bkd, bke, bkf, bkg, bkh, bki, bkj, bkk, \
    bkl, bkm, bkn, bko, bkp, bkq, bkr, bks, bkt, bku, bkv, bkw, bkx, bky, bkz, bkA, bkB, bkC, bkD, \
    bkE, bkF, bkG, bkH, bkI, bkJ, bkK, bkL, bkM, bkN, bkO, bkP, bkQ, bkR, bkS, bkT, bkU, bkV, bkW, \
    bkX, bkY, bkZ, bla, blb, blc, bld, ble, blf, blg, blh, bli, blj, blk, bll, blm, bln, blo, blp, \
    blq, blr, bls, blt, blu, blv, blw, blx, bly, blz, blA, blB, blC, blD, blE, blF, blG, blH, blI, \
    blJ, blK, blL, blM, blN, blO, blP, blQ, blR, blS, blT, blU, blV, blW, blX, blY, blZ, bma, bmb, \
    bmc, bmd, bme, bmf, bmg, bmh, bmi, bmj, bmk, bml, bmm, bmn, bmo, bmp, bmq, bmr, bms, bmt, bmu, \
    bmv, bmw, bmx, bmy, bmz, bmA, bmB, bmC, bmD, bmE, bmF, bmG, bmH, bmI, bmJ, bmK, bmL, bmM, bmN, \
    bmO, bmP, bmQ, bmR, bmS, bmT, bmU, bmV, bmW, bmX, bmY, bmZ, bna, bnb, bnc, bnd, bne, bnf, bng, \
    bnh, bni, bnj, bnk, bnl, bnm, bnn, bno, bnp, bnq, bnr, bns, bnt, bnu, bnv, bnw, bnx, bny, bnz, \
    bnA, bnB, bnC, bnD, bnE, bnF, bnG, bnH, bnI, bnJ, bnK, bnL, bnM, bnN, bnO, bnP, bnQ, bnR, bnS, \
    bnT, bnU, bnV, bnW, bnX, bnY, bnZ, boa, bob, boc, bod, boe, bof, bog, boh, boi, boj, bok, bol, \
    bom, bon, boo, bop, boq, bor, bos, bot, bou, bov, bow, box, boy, boz, boA, boB, boC, boD, boE, \
    boF, boG, boH, boI, boJ, boK, boL, boM, boN, boO, boP, boQ, boR, boS, boT, boU, boV, boW, boX, \
    boY, boZ, bpa, bpb, bpc, bpd, bpe, bpf, bpg, bph, bpi, bpj, bpk, bpl, bpm, bpn, bpo, bpp, bpq, \
    bpr, bps, bpt, bpu, bpv, bpw, bpx, bpy, bpz, bpA, bpB, bpC, bpD, bpE, bpF, bpG, bpH, bpI, bpJ, \
    bpK, bpL, bpM, bpN, bpO, bpP, bpQ, bpR, bpS, bpT, bpU, bpV, bpW, bpX, bpY, bpZ, bqa, bqb, bqc, \
    bqd, bqe, bqf, bqg, bqh, bqi, bqj, bqk, bql, bqm, bqn, bqo, bqp, bqq, bqr, bqs, bqt, bqu, bqv, \
    bqw, bqx, bqy, bqz, bqA, bqB, bqC, bqD, bqE, bqF, bqG, bqH, bqI, bqJ, bqK, bqL, bqM, bqN, bqO, \
    bqP, bqQ, bqR, bqS, bqT, bqU, bqV, bqW, bqX, bqY, bqZ, bra, brb, brc, brd, bre, brf, brg, brh, \
    bri, brj, brk, brl, brm, brn, bro, brp, brq, brr, brs, brt, bru, brv, brw, brx, bry, brz, brA, \
    brB, brC, brD, brE, brF, brG, brH, brI, brJ, brK, brL, brM, brN, brO, brP, brQ, brR, brS, brT, \
    brU, brV, brW, brX, brY, brZ, bsa, bsb, bsc, bsd, bse, bsf, bsg, bsh, bsi, bsj, bsk, bsl, bsm, \
    bsn, bso, bsp, bsq, bsr, bss, bst, bsu, bsv, bsw, bsx, bsy, bsz, bsA, bsB, bsC, bsD, bsE, bsF, \
    bsG, bsH, bsI, bsJ, bsK, bsL, bsM, bsN, bsO, bsP, bsQ, bsR, bsS, bsT, bsU, bsV, bsW, bsX, bsY, \
    bsZ, bta, btb, btc, btd, bte, btf, btg, bth, bti, btj, btk, btl, btm, btn, bto, btp, btq, btr, \
    bts, btt, btu, btv, btw, btx, bty, btz, btA, btB, btC, btD, btE, btF, btG, btH, btI, btJ, btK, \
    btL, btM, btN, btO, btP, btQ, btR, btS, btT, btU, btV, btW, btX, btY, btZ, bua, bub, buc, bud, \
    bue, buf, bug, buh, bui, buj, buk, bul, bum, bun, buo, bup, buq, bur, bus, but, buu, buv, buw, \
    bux, buy, buz, buA, buB, buC, buD, buE, buF, buG, buH, buI, buJ, buK, buL, buM, buN, buO, buP, \
    buQ, buR, buS, buT, buU, buV, buW, buX, buY, buZ, bva, bvb, bvc, bvd, bve, bvf, bvg, bvh, bvi, \
    bvj, bvk, bvl, bvm, bvn, bvo, bvp, bvq, bvr, bvs, bvt, bvu, bvv, bvw, bvx, bvy, bvz, bvA, bvB, \
    bvC, bvD, bvE, bvF, bvG, bvH, bvI, bvJ, bvK, bvL, bvM, bvN, bvO, bvP, bvQ, bvR, bvS, bvT, bvU, \
    bvV, bvW, bvX, bvY, bvZ, bwa, bwb, bwc, bwd, bwe, bwf, bwg, bwh, bwi, bwj, bwk, bwl, bwm, bwn, \
    bwo, bwp, bwq, bwr, bws, bwt, bwu, bwv, bww, bwx, bwy, bwz, bwA, bwB, bwC, bwD, bwE, bwF, bwG, \
    bwH, bwI, bwJ, bwK, bwL, bwM, bwN, bwO, bwP, bwQ, bwR, bwS, bwT, bwU, bwV, bwW, bwX, bwY, bwZ, \
    bxa, bxb, bxc, bxd, bxe, bxf, bxg, bxh, bxi, bxj, bxk, bxl, bxm, bxn, bxo, bxp, bxq, bxr, bxs, \
    bxt, bxu, bxv, bxw, bxx, bxy, bxz, bxA, bxB, bxC, bxD, bxE, bxF, bxG, bxH, bxI, bxJ, bxK, bxL, \
    bxM, bxN, bxO, bxP, bxQ, bxR, bxS, bxT, bxU, bxV, bxW, bxX, bxY, bxZ, bya, byb, byc, byd, bye, \
    byf, byg, byh, byi, byj, byk, byl, bym, byn, byo, byp, byq, byr, bys, byt, byu, byv, byw, byx, \
    byy, byz, byA, byB, byC, byD, byE, byF, byG, byH, byI, byJ, byK, byL, byM, byN, byO, byP, byQ, \
    byR, byS, byT, byU, byV, byW, byX, byY, byZ, bza, bzb, bzc, bzd, bze, bzf, bzg, bzh, bzi, bzj, \
    bzk, bzl, bzm, bzn, bzo, bzp, bzq, bzr, bzs, bzt, bzu, bzv, bzw, bzx, bzy, bzz, bzA, bzB, bzC, \
    bzD, bzE, bzF, bzG, bzH, bzI, bzJ, bzK, bzL, bzM, bzN, bzO, bzP, bzQ, bzR, bzS, bzT, bzU, bzV, \
    bzW, bzX, bzY, bzZ, bAa, bAb, bAc, bAd, bAe, bAf, bAg, bAh, bAi, bAj, bAk, bAl, bAm, bAn, bAo, \
    bAp, bAq, bAr, bAs, bAt, bAu, bAv, bAw, bAx, bAy, bAz, bAA, bAB, bAC, bAD, bAE, bAF, bAG, bAH, \
    bAI, bAJ, bAK, bAL, bAM, _sz, ...)                                                             \
  PTL_CAT(PPUTLSIZE_READ_, PTL_IS_NONE(PPUTLSIZE_##_sz))(_err, _sz)
#define PPUTLSIZE_READ_1(_err, _sz)   PPUTLSIZE_READ_##_sz
#define PPUTLSIZE_READ_0(_err, _sz)   PTL_FAIL(_err)
#define PPUTLSIZE_READ_PPUTLSIZE(...) __VA_ARGS__
#define PPUTLSIZE_PPUTLSIZE(...)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [util.items]
/// ------------
/// extracts tuple items.
///
/// PTL_ITEMS(())        // <nothing>
/// PTL_ITEMS((a))       // a
/// PTL_ITEMS((a, b))    // a, b
/// PTL_ITEMS((a, b, c)) // a, b, c
#define PTL_ITEMS(/* v: tup */...) /* -> ...v */ PPUTLITEMS_X(PTL_TUP(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLITEMS_X(...) PTL_ESC __VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [impl]
/// ------

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

/// [impl.nybl.trait]
/// -----------------
/// [internal] get a nybl digit trait.
/// argument must be atom for IS. all other traits require nybl.
#define PPUTLIMPL_NYBL_TRAIT(/* v: <atom|nybl>, trait: IS|NOT|DEC|INC|SL|SR|BIN */ v, t) \
  PPUTLIMPL_NYBL_TRAIT_o(t, PTL_CAT(PPUTLIMPL_NYBL_TRAIT_, v))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_NYBL_TRAIT_o(trait, ...)             PPUTLIMPL_NYBL_TRAIT_##trait(__VA_ARGS__)
#define PPUTLIMPL_NYBL_TRAIT_BIN(n, d, i, sl, sr, bin) bin
#define PPUTLIMPL_NYBL_TRAIT_SR(n, d, i, sl, sr, ...)  sr
#define PPUTLIMPL_NYBL_TRAIT_SL(n, d, i, sl, ...)      sl
#define PPUTLIMPL_NYBL_TRAIT_INC(n, d, i, ...)         i
#define PPUTLIMPL_NYBL_TRAIT_DEC(n, d, ...)            d
#define PPUTLIMPL_NYBL_TRAIT_NOT(n, ...)               n
#define PPUTLIMPL_NYBL_TRAIT_IS(_, ...)                PPUTLIMPL_NYBL_TRAIT_IS_0##__VA_OPT__(1)
#define PPUTLIMPL_NYBL_TRAIT_IS_01                     1
#define PPUTLIMPL_NYBL_TRAIT_IS_0                      0

/// not, (dec carry, dec), (inc carry, inc), (sl carry, sl), (sr mod, sr), (...bin)
#define PPUTLIMPL_NYBL_TRAIT_F 0, (0, E), (1, 0), (1, E), (1, 7), (1, 1, 1, 1)
#define PPUTLIMPL_NYBL_TRAIT_E 1, (0, D), (0, F), (1, C), (0, 7), (1, 1, 1, 0)
#define PPUTLIMPL_NYBL_TRAIT_D 2, (0, C), (0, E), (1, A), (1, 6), (1, 1, 0, 1)
#define PPUTLIMPL_NYBL_TRAIT_C 3, (0, B), (0, D), (1, 8), (0, 6), (1, 1, 0, 0)
#define PPUTLIMPL_NYBL_TRAIT_B 4, (0, A), (0, C), (1, 6), (1, 5), (1, 0, 1, 1)
#define PPUTLIMPL_NYBL_TRAIT_A 5, (0, 9), (0, B), (1, 4), (0, 5), (1, 0, 1, 0)
#define PPUTLIMPL_NYBL_TRAIT_9 6, (0, 8), (0, A), (1, 2), (1, 4), (1, 0, 0, 1)
#define PPUTLIMPL_NYBL_TRAIT_8 7, (0, 7), (0, 9), (1, 0), (0, 4), (1, 0, 0, 0)
#define PPUTLIMPL_NYBL_TRAIT_7 8, (0, 6), (0, 8), (0, E), (1, 3), (0, 1, 1, 1)
#define PPUTLIMPL_NYBL_TRAIT_6 9, (0, 5), (0, 7), (0, C), (0, 3), (0, 1, 1, 0)
#define PPUTLIMPL_NYBL_TRAIT_5 A, (0, 4), (0, 6), (0, A), (1, 2), (0, 1, 0, 1)
#define PPUTLIMPL_NYBL_TRAIT_4 B, (0, 3), (0, 5), (0, 8), (0, 2), (0, 1, 0, 0)
#define PPUTLIMPL_NYBL_TRAIT_3 C, (0, 2), (0, 4), (0, 6), (1, 1), (0, 0, 1, 1)
#define PPUTLIMPL_NYBL_TRAIT_2 D, (0, 1), (0, 3), (0, 4), (0, 1), (0, 0, 1, 0)
#define PPUTLIMPL_NYBL_TRAIT_1 E, (0, 0), (0, 2), (0, 2), (1, 0), (0, 0, 0, 1)
#define PPUTLIMPL_NYBL_TRAIT_0 F, (1, F), (0, 1), (0, 0), (0, 0), (0, 0, 0, 0)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [impl.nybl.pair_trait]
/// ----------------------
/// [internal] get a nybl pair trait.
/// p must be a nybl pair; e.g. E6.
#define PPUTLIMPL_NYBL_PAIR_TRAIT(/* p: <nybl pair>, trait: LT|AND|OR|XOR|SUB|ADD */ p, t) \
  PPUTLIMPL_NYBL_PAIR_TRAIT_o(t, PTL_CAT(PPUTLIMPL_NYBL_PAIR_TRAIT_, p))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_NYBL_PAIR_TRAIT_o(trait, ...) PPUTLIMPL_NYBL_PAIR_TRAIT_##trait(__VA_ARGS__)

#define PPUTLIMPL_NYBL_PAIR_TRAIT_ADD(l, a, o, x, s, ad, ...) ad
#define PPUTLIMPL_NYBL_PAIR_TRAIT_SUB(l, a, o, x, s, ...)     s
#define PPUTLIMPL_NYBL_PAIR_TRAIT_XOR(l, a, o, x, ...)        x
#define PPUTLIMPL_NYBL_PAIR_TRAIT_OR(l, a, o, ...)            o
#define PPUTLIMPL_NYBL_PAIR_TRAIT_AND(l, a, ...)              a
#define PPUTLIMPL_NYBL_PAIR_TRAIT_LT(l, ...)                  l

/// lt, and, or, xor, (sub carry, sub), (add carry, add)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_FF 0, F, F, 0, (0, 0), (1, E)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_FE 0, E, F, 1, (0, 1), (1, D)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_FD 0, D, F, 2, (0, 2), (1, C)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_FC 0, C, F, 3, (0, 3), (1, B)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_FB 0, B, F, 4, (0, 4), (1, A)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_FA 0, A, F, 5, (0, 5), (1, 9)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_F9 0, 9, F, 6, (0, 6), (1, 8)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_F8 0, 8, F, 7, (0, 7), (1, 7)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_F7 0, 7, F, 8, (0, 8), (1, 6)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_F6 0, 6, F, 9, (0, 9), (1, 5)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_F5 0, 5, F, A, (0, A), (1, 4)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_F4 0, 4, F, B, (0, B), (1, 3)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_F3 0, 3, F, C, (0, C), (1, 2)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_F2 0, 2, F, D, (0, D), (1, 1)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_F1 0, 1, F, E, (0, E), (1, 0)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_F0 0, 0, F, F, (0, F), (0, F)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_EF 1, E, F, 1, (1, F), (1, D)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_EE 0, E, E, 0, (0, 0), (1, C)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_ED 0, C, F, 3, (0, 1), (1, B)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_EC 0, C, E, 2, (0, 2), (1, A)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_EB 0, A, F, 5, (0, 3), (1, 9)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_EA 0, A, E, 4, (0, 4), (1, 8)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_E9 0, 8, F, 7, (0, 5), (1, 7)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_E8 0, 8, E, 6, (0, 6), (1, 6)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_E7 0, 6, F, 9, (0, 7), (1, 5)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_E6 0, 6, E, 8, (0, 8), (1, 4)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_E5 0, 4, F, B, (0, 9), (1, 3)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_E4 0, 4, E, A, (0, A), (1, 2)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_E3 0, 2, F, D, (0, B), (1, 1)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_E2 0, 2, E, C, (0, C), (1, 0)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_E1 0, 0, F, F, (0, D), (0, F)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_E0 0, 0, E, E, (0, E), (0, E)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_DF 1, D, F, 2, (1, E), (1, C)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_DE 1, C, F, 3, (1, F), (1, B)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_DD 0, D, D, 0, (0, 0), (1, A)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_DC 0, C, D, 1, (0, 1), (1, 9)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_DB 0, 9, F, 6, (0, 2), (1, 8)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_DA 0, 8, F, 7, (0, 3), (1, 7)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_D9 0, 9, D, 4, (0, 4), (1, 6)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_D8 0, 8, D, 5, (0, 5), (1, 5)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_D7 0, 5, F, A, (0, 6), (1, 4)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_D6 0, 4, F, B, (0, 7), (1, 3)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_D5 0, 5, D, 8, (0, 8), (1, 2)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_D4 0, 4, D, 9, (0, 9), (1, 1)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_D3 0, 1, F, E, (0, A), (1, 0)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_D2 0, 0, F, F, (0, B), (0, F)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_D1 0, 1, D, C, (0, C), (0, E)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_D0 0, 0, D, D, (0, D), (0, D)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_CF 1, C, F, 3, (1, D), (1, B)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_CE 1, C, E, 2, (1, E), (1, A)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_CD 1, C, D, 1, (1, F), (1, 9)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_CC 0, C, C, 0, (0, 0), (1, 8)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_CB 0, 8, F, 7, (0, 1), (1, 7)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_CA 0, 8, E, 6, (0, 2), (1, 6)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_C9 0, 8, D, 5, (0, 3), (1, 5)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_C8 0, 8, C, 4, (0, 4), (1, 4)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_C7 0, 4, F, B, (0, 5), (1, 3)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_C6 0, 4, E, A, (0, 6), (1, 2)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_C5 0, 4, D, 9, (0, 7), (1, 1)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_C4 0, 4, C, 8, (0, 8), (1, 0)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_C3 0, 0, F, F, (0, 9), (0, F)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_C2 0, 0, E, E, (0, A), (0, E)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_C1 0, 0, D, D, (0, B), (0, D)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_C0 0, 0, C, C, (0, C), (0, C)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_BF 1, B, F, 4, (1, C), (1, A)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_BE 1, A, F, 5, (1, D), (1, 9)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_BD 1, 9, F, 6, (1, E), (1, 8)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_BC 1, 8, F, 7, (1, F), (1, 7)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_BB 0, B, B, 0, (0, 0), (1, 6)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_BA 0, A, B, 1, (0, 1), (1, 5)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_B9 0, 9, B, 2, (0, 2), (1, 4)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_B8 0, 8, B, 3, (0, 3), (1, 3)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_B7 0, 3, F, C, (0, 4), (1, 2)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_B6 0, 2, F, D, (0, 5), (1, 1)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_B5 0, 1, F, E, (0, 6), (1, 0)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_B4 0, 0, F, F, (0, 7), (0, F)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_B3 0, 3, B, 8, (0, 8), (0, E)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_B2 0, 2, B, 9, (0, 9), (0, D)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_B1 0, 1, B, A, (0, A), (0, C)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_B0 0, 0, B, B, (0, B), (0, B)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_AF 1, A, F, 5, (1, B), (1, 9)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_AE 1, A, E, 4, (1, C), (1, 8)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_AD 1, 8, F, 7, (1, D), (1, 7)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_AC 1, 8, E, 6, (1, E), (1, 6)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_AB 1, A, B, 1, (1, F), (1, 5)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_AA 0, A, A, 0, (0, 0), (1, 4)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_A9 0, 8, B, 3, (0, 1), (1, 3)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_A8 0, 8, A, 2, (0, 2), (1, 2)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_A7 0, 2, F, D, (0, 3), (1, 1)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_A6 0, 2, E, C, (0, 4), (1, 0)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_A5 0, 0, F, F, (0, 5), (0, F)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_A4 0, 0, E, E, (0, 6), (0, E)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_A3 0, 2, B, 9, (0, 7), (0, D)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_A2 0, 2, A, 8, (0, 8), (0, C)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_A1 0, 0, B, B, (0, 9), (0, B)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_A0 0, 0, A, A, (0, A), (0, A)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_9F 1, 9, F, 6, (1, A), (1, 8)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_9E 1, 8, F, 7, (1, B), (1, 7)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_9D 1, 9, D, 4, (1, C), (1, 6)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_9C 1, 8, D, 5, (1, D), (1, 5)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_9B 1, 9, B, 2, (1, E), (1, 4)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_9A 1, 8, B, 3, (1, F), (1, 3)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_99 0, 9, 9, 0, (0, 0), (1, 2)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_98 0, 8, 9, 1, (0, 1), (1, 1)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_97 0, 1, F, E, (0, 2), (1, 0)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_96 0, 0, F, F, (0, 3), (0, F)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_95 0, 1, D, C, (0, 4), (0, E)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_94 0, 0, D, D, (0, 5), (0, D)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_93 0, 1, B, A, (0, 6), (0, C)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_92 0, 0, B, B, (0, 7), (0, B)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_91 0, 1, 9, 8, (0, 8), (0, A)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_90 0, 0, 9, 9, (0, 9), (0, 9)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_8F 1, 8, F, 7, (1, 9), (1, 7)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_8E 1, 8, E, 6, (1, A), (1, 6)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_8D 1, 8, D, 5, (1, B), (1, 5)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_8C 1, 8, C, 4, (1, C), (1, 4)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_8B 1, 8, B, 3, (1, D), (1, 3)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_8A 1, 8, A, 2, (1, E), (1, 2)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_89 1, 8, 9, 1, (1, F), (1, 1)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_88 0, 8, 8, 0, (0, 0), (1, 0)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_87 0, 0, F, F, (0, 1), (0, F)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_86 0, 0, E, E, (0, 2), (0, E)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_85 0, 0, D, D, (0, 3), (0, D)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_84 0, 0, C, C, (0, 4), (0, C)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_83 0, 0, B, B, (0, 5), (0, B)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_82 0, 0, A, A, (0, 6), (0, A)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_81 0, 0, 9, 9, (0, 7), (0, 9)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_80 0, 0, 8, 8, (0, 8), (0, 8)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_7F 1, 7, F, 8, (1, 8), (1, 6)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_7E 1, 6, F, 9, (1, 9), (1, 5)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_7D 1, 5, F, A, (1, A), (1, 4)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_7C 1, 4, F, B, (1, B), (1, 3)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_7B 1, 3, F, C, (1, C), (1, 2)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_7A 1, 2, F, D, (1, D), (1, 1)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_79 1, 1, F, E, (1, E), (1, 0)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_78 1, 0, F, F, (1, F), (0, F)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_77 0, 7, 7, 0, (0, 0), (0, E)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_76 0, 6, 7, 1, (0, 1), (0, D)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_75 0, 5, 7, 2, (0, 2), (0, C)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_74 0, 4, 7, 3, (0, 3), (0, B)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_73 0, 3, 7, 4, (0, 4), (0, A)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_72 0, 2, 7, 5, (0, 5), (0, 9)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_71 0, 1, 7, 6, (0, 6), (0, 8)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_70 0, 0, 7, 7, (0, 7), (0, 7)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_6F 1, 6, F, 9, (1, 7), (1, 5)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_6E 1, 6, E, 8, (1, 8), (1, 4)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_6D 1, 4, F, B, (1, 9), (1, 3)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_6C 1, 4, E, A, (1, A), (1, 2)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_6B 1, 2, F, D, (1, B), (1, 1)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_6A 1, 2, E, C, (1, C), (1, 0)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_69 1, 0, F, F, (1, D), (0, F)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_68 1, 0, E, E, (1, E), (0, E)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_67 1, 6, 7, 1, (1, F), (0, D)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_66 0, 6, 6, 0, (0, 0), (0, C)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_65 0, 4, 7, 3, (0, 1), (0, B)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_64 0, 4, 6, 2, (0, 2), (0, A)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_63 0, 2, 7, 5, (0, 3), (0, 9)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_62 0, 2, 6, 4, (0, 4), (0, 8)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_61 0, 0, 7, 7, (0, 5), (0, 7)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_60 0, 0, 6, 6, (0, 6), (0, 6)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_5F 1, 5, F, A, (1, 6), (1, 4)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_5E 1, 4, F, B, (1, 7), (1, 3)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_5D 1, 5, D, 8, (1, 8), (1, 2)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_5C 1, 4, D, 9, (1, 9), (1, 1)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_5B 1, 1, F, E, (1, A), (1, 0)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_5A 1, 0, F, F, (1, B), (0, F)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_59 1, 1, D, C, (1, C), (0, E)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_58 1, 0, D, D, (1, D), (0, D)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_57 1, 5, 7, 2, (1, E), (0, C)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_56 1, 4, 7, 3, (1, F), (0, B)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_55 0, 5, 5, 0, (0, 0), (0, A)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_54 0, 4, 5, 1, (0, 1), (0, 9)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_53 0, 1, 7, 6, (0, 2), (0, 8)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_52 0, 0, 7, 7, (0, 3), (0, 7)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_51 0, 1, 5, 4, (0, 4), (0, 6)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_50 0, 0, 5, 5, (0, 5), (0, 5)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_4F 1, 4, F, B, (1, 5), (1, 3)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_4E 1, 4, E, A, (1, 6), (1, 2)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_4D 1, 4, D, 9, (1, 7), (1, 1)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_4C 1, 4, C, 8, (1, 8), (1, 0)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_4B 1, 0, F, F, (1, 9), (0, F)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_4A 1, 0, E, E, (1, A), (0, E)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_49 1, 0, D, D, (1, B), (0, D)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_48 1, 0, C, C, (1, C), (0, C)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_47 1, 4, 7, 3, (1, D), (0, B)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_46 1, 4, 6, 2, (1, E), (0, A)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_45 1, 4, 5, 1, (1, F), (0, 9)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_44 0, 4, 4, 0, (0, 0), (0, 8)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_43 0, 0, 7, 7, (0, 1), (0, 7)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_42 0, 0, 6, 6, (0, 2), (0, 6)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_41 0, 0, 5, 5, (0, 3), (0, 5)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_40 0, 0, 4, 4, (0, 4), (0, 4)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_3F 1, 3, F, C, (1, 4), (1, 2)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_3E 1, 2, F, D, (1, 5), (1, 1)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_3D 1, 1, F, E, (1, 6), (1, 0)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_3C 1, 0, F, F, (1, 7), (0, F)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_3B 1, 3, B, 8, (1, 8), (0, E)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_3A 1, 2, B, 9, (1, 9), (0, D)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_39 1, 1, B, A, (1, A), (0, C)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_38 1, 0, B, B, (1, B), (0, B)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_37 1, 3, 7, 4, (1, C), (0, A)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_36 1, 2, 7, 5, (1, D), (0, 9)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_35 1, 1, 7, 6, (1, E), (0, 8)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_34 1, 0, 7, 7, (1, F), (0, 7)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_33 0, 3, 3, 0, (0, 0), (0, 6)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_32 0, 2, 3, 1, (0, 1), (0, 5)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_31 0, 1, 3, 2, (0, 2), (0, 4)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_30 0, 0, 3, 3, (0, 3), (0, 3)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_2F 1, 2, F, D, (1, 3), (1, 1)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_2E 1, 2, E, C, (1, 4), (1, 0)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_2D 1, 0, F, F, (1, 5), (0, F)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_2C 1, 0, E, E, (1, 6), (0, E)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_2B 1, 2, B, 9, (1, 7), (0, D)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_2A 1, 2, A, 8, (1, 8), (0, C)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_29 1, 0, B, B, (1, 9), (0, B)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_28 1, 0, A, A, (1, A), (0, A)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_27 1, 2, 7, 5, (1, B), (0, 9)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_26 1, 2, 6, 4, (1, C), (0, 8)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_25 1, 0, 7, 7, (1, D), (0, 7)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_24 1, 0, 6, 6, (1, E), (0, 6)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_23 1, 2, 3, 1, (1, F), (0, 5)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_22 0, 2, 2, 0, (0, 0), (0, 4)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_21 0, 0, 3, 3, (0, 1), (0, 3)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_20 0, 0, 2, 2, (0, 2), (0, 2)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_1F 1, 1, F, E, (1, 2), (1, 0)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_1E 1, 0, F, F, (1, 3), (0, F)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_1D 1, 1, D, C, (1, 4), (0, E)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_1C 1, 0, D, D, (1, 5), (0, D)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_1B 1, 1, B, A, (1, 6), (0, C)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_1A 1, 0, B, B, (1, 7), (0, B)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_19 1, 1, 9, 8, (1, 8), (0, A)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_18 1, 0, 9, 9, (1, 9), (0, 9)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_17 1, 1, 7, 6, (1, A), (0, 8)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_16 1, 0, 7, 7, (1, B), (0, 7)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_15 1, 1, 5, 4, (1, C), (0, 6)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_14 1, 0, 5, 5, (1, D), (0, 5)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_13 1, 1, 3, 2, (1, E), (0, 4)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_12 1, 0, 3, 3, (1, F), (0, 3)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_11 0, 1, 1, 0, (0, 0), (0, 2)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_10 0, 0, 1, 1, (0, 1), (0, 1)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_0F 1, 0, F, F, (1, 1), (0, F)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_0E 1, 0, E, E, (1, 2), (0, E)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_0D 1, 0, D, D, (1, 3), (0, D)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_0C 1, 0, C, C, (1, 4), (0, C)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_0B 1, 0, B, B, (1, 5), (0, B)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_0A 1, 0, A, A, (1, 6), (0, A)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_09 1, 0, 9, 9, (1, 7), (0, 9)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_08 1, 0, 8, 8, (1, 8), (0, 8)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_07 1, 0, 7, 7, (1, 9), (0, 7)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_06 1, 0, 6, 6, (1, A), (0, 6)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_05 1, 0, 5, 5, (1, B), (0, 5)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_04 1, 0, 4, 4, (1, C), (0, 4)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_03 1, 0, 3, 3, (1, D), (0, 3)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_02 1, 0, 2, 2, (1, E), (0, 2)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_01 1, 0, 1, 1, (1, F), (0, 1)
#define PPUTLIMPL_NYBL_PAIR_TRAIT_00 0, 0, 0, 0, (0, 0), (0, 0)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [impl.uint.trait]
/// -----------------
/// [internal] get a uint trait.
/// argument must be atom for IS. all other traits require uint.
/// traits with an 'H' prefix require hex representation,
/// and traits with a 'D' prefix require decimal representation.
#define PPUTLIMPL_UINT_TRAIT(                                                                    \
    /* v: <atom|uint>, trait: IS|TYPE|HUDEC|HIDEC|HNYBS|HBNOT|DHEX|DINEG|DLOG2|DSQRT|DFACT */ v, \
    t)                                                                                           \
  PPUTLIMPL_UINT_TRAIT_o(t, PTL_CAT(PPUTLIMPL_UINT_TRAIT_, v))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_UINT_TRAIT_o(trait, ...)                 PPUTLIMPL_UINT_TRAIT_##trait(__VA_ARGS__)
#define PPUTLIMPL_UINT_TRAIT_DFACT(t, ih, in, l2, sq, f)   /* -> typle<ubase10{factor}...> */ f
#define PPUTLIMPL_UINT_TRAIT_DSQRT(t, ih, in, l2, sq, ...) /* -> ubase10{sqrt(n)} */ sq
#define PPUTLIMPL_UINT_TRAIT_DLOG2(t, ih, in, l2, ...)     /* -> ubase10{log2(n)} */ l2
#define PPUTLIMPL_UINT_TRAIT_DINEG(t, ih, in, ...)         /* -> bool{signed<0} */ in
#define PPUTLIMPL_UINT_TRAIT_DIHEX(t, ih, ...)             /* -> ibase16 */ ih
#define PPUTLIMPL_UINT_TRAIT_HBNOT(t, ud, id, n, bn)       /* -> ubase16 */ bn
#define PPUTLIMPL_UINT_TRAIT_HWORD(t, ud, id, n, ...)      /* -> word */ n
#define PPUTLIMPL_UINT_TRAIT_HIDEC(t, ud, id, ...)         /* -> ibase10 */ id
#define PPUTLIMPL_UINT_TRAIT_HUDEC(t, ud, ...)             /* -> ubase10 */ ud
#define PPUTLIMPL_UINT_TRAIT_TYPE(t, ...)                  /* -> HEX|DEC */ t

#define PPUTLIMPL_UINT_TRAIT_IS(_, ...) /* -> bool */ PPUTLIMPL_UINT_TRAIT_IS_0##__VA_OPT__(1)

#define PPUTLIMPL_UINT_TRAIT_IS_01 1
#define PPUTLIMPL_UINT_TRAIT_IS_0  0

/// type, unsigned decimal, signed decimal, word, bitnot
#define PPUTLIMPL_UINT_TRAIT_0xFFFu HEX, 4095u, -1, (F, F, F), 0x000u
#define PPUTLIMPL_UINT_TRAIT_0xFFEu HEX, 4094u, -2, (F, F, E), 0x001u
#define PPUTLIMPL_UINT_TRAIT_0xFFDu HEX, 4093u, -3, (F, F, D), 0x002u
#define PPUTLIMPL_UINT_TRAIT_0xFFCu HEX, 4092u, -4, (F, F, C), 0x003u
#define PPUTLIMPL_UINT_TRAIT_0xFFBu HEX, 4091u, -5, (F, F, B), 0x004u
#define PPUTLIMPL_UINT_TRAIT_0xFFAu HEX, 4090u, -6, (F, F, A), 0x005u
#define PPUTLIMPL_UINT_TRAIT_0xFF9u HEX, 4089u, -7, (F, F, 9), 0x006u
#define PPUTLIMPL_UINT_TRAIT_0xFF8u HEX, 4088u, -8, (F, F, 8), 0x007u
#define PPUTLIMPL_UINT_TRAIT_0xFF7u HEX, 4087u, -9, (F, F, 7), 0x008u
#define PPUTLIMPL_UINT_TRAIT_0xFF6u HEX, 4086u, -10, (F, F, 6), 0x009u
#define PPUTLIMPL_UINT_TRAIT_0xFF5u HEX, 4085u, -11, (F, F, 5), 0x00Au
#define PPUTLIMPL_UINT_TRAIT_0xFF4u HEX, 4084u, -12, (F, F, 4), 0x00Bu
#define PPUTLIMPL_UINT_TRAIT_0xFF3u HEX, 4083u, -13, (F, F, 3), 0x00Cu
#define PPUTLIMPL_UINT_TRAIT_0xFF2u HEX, 4082u, -14, (F, F, 2), 0x00Du
#define PPUTLIMPL_UINT_TRAIT_0xFF1u HEX, 4081u, -15, (F, F, 1), 0x00Eu
#define PPUTLIMPL_UINT_TRAIT_0xFF0u HEX, 4080u, -16, (F, F, 0), 0x00Fu
#define PPUTLIMPL_UINT_TRAIT_0xFEFu HEX, 4079u, -17, (F, E, F), 0x010u
#define PPUTLIMPL_UINT_TRAIT_0xFEEu HEX, 4078u, -18, (F, E, E), 0x011u
#define PPUTLIMPL_UINT_TRAIT_0xFEDu HEX, 4077u, -19, (F, E, D), 0x012u
#define PPUTLIMPL_UINT_TRAIT_0xFECu HEX, 4076u, -20, (F, E, C), 0x013u
#define PPUTLIMPL_UINT_TRAIT_0xFEBu HEX, 4075u, -21, (F, E, B), 0x014u
#define PPUTLIMPL_UINT_TRAIT_0xFEAu HEX, 4074u, -22, (F, E, A), 0x015u
#define PPUTLIMPL_UINT_TRAIT_0xFE9u HEX, 4073u, -23, (F, E, 9), 0x016u
#define PPUTLIMPL_UINT_TRAIT_0xFE8u HEX, 4072u, -24, (F, E, 8), 0x017u
#define PPUTLIMPL_UINT_TRAIT_0xFE7u HEX, 4071u, -25, (F, E, 7), 0x018u
#define PPUTLIMPL_UINT_TRAIT_0xFE6u HEX, 4070u, -26, (F, E, 6), 0x019u
#define PPUTLIMPL_UINT_TRAIT_0xFE5u HEX, 4069u, -27, (F, E, 5), 0x01Au
#define PPUTLIMPL_UINT_TRAIT_0xFE4u HEX, 4068u, -28, (F, E, 4), 0x01Bu
#define PPUTLIMPL_UINT_TRAIT_0xFE3u HEX, 4067u, -29, (F, E, 3), 0x01Cu
#define PPUTLIMPL_UINT_TRAIT_0xFE2u HEX, 4066u, -30, (F, E, 2), 0x01Du
#define PPUTLIMPL_UINT_TRAIT_0xFE1u HEX, 4065u, -31, (F, E, 1), 0x01Eu
#define PPUTLIMPL_UINT_TRAIT_0xFE0u HEX, 4064u, -32, (F, E, 0), 0x01Fu
#define PPUTLIMPL_UINT_TRAIT_0xFDFu HEX, 4063u, -33, (F, D, F), 0x020u
#define PPUTLIMPL_UINT_TRAIT_0xFDEu HEX, 4062u, -34, (F, D, E), 0x021u
#define PPUTLIMPL_UINT_TRAIT_0xFDDu HEX, 4061u, -35, (F, D, D), 0x022u
#define PPUTLIMPL_UINT_TRAIT_0xFDCu HEX, 4060u, -36, (F, D, C), 0x023u
#define PPUTLIMPL_UINT_TRAIT_0xFDBu HEX, 4059u, -37, (F, D, B), 0x024u
#define PPUTLIMPL_UINT_TRAIT_0xFDAu HEX, 4058u, -38, (F, D, A), 0x025u
#define PPUTLIMPL_UINT_TRAIT_0xFD9u HEX, 4057u, -39, (F, D, 9), 0x026u
#define PPUTLIMPL_UINT_TRAIT_0xFD8u HEX, 4056u, -40, (F, D, 8), 0x027u
#define PPUTLIMPL_UINT_TRAIT_0xFD7u HEX, 4055u, -41, (F, D, 7), 0x028u
#define PPUTLIMPL_UINT_TRAIT_0xFD6u HEX, 4054u, -42, (F, D, 6), 0x029u
#define PPUTLIMPL_UINT_TRAIT_0xFD5u HEX, 4053u, -43, (F, D, 5), 0x02Au
#define PPUTLIMPL_UINT_TRAIT_0xFD4u HEX, 4052u, -44, (F, D, 4), 0x02Bu
#define PPUTLIMPL_UINT_TRAIT_0xFD3u HEX, 4051u, -45, (F, D, 3), 0x02Cu
#define PPUTLIMPL_UINT_TRAIT_0xFD2u HEX, 4050u, -46, (F, D, 2), 0x02Du
#define PPUTLIMPL_UINT_TRAIT_0xFD1u HEX, 4049u, -47, (F, D, 1), 0x02Eu
#define PPUTLIMPL_UINT_TRAIT_0xFD0u HEX, 4048u, -48, (F, D, 0), 0x02Fu
#define PPUTLIMPL_UINT_TRAIT_0xFCFu HEX, 4047u, -49, (F, C, F), 0x030u
#define PPUTLIMPL_UINT_TRAIT_0xFCEu HEX, 4046u, -50, (F, C, E), 0x031u
#define PPUTLIMPL_UINT_TRAIT_0xFCDu HEX, 4045u, -51, (F, C, D), 0x032u
#define PPUTLIMPL_UINT_TRAIT_0xFCCu HEX, 4044u, -52, (F, C, C), 0x033u
#define PPUTLIMPL_UINT_TRAIT_0xFCBu HEX, 4043u, -53, (F, C, B), 0x034u
#define PPUTLIMPL_UINT_TRAIT_0xFCAu HEX, 4042u, -54, (F, C, A), 0x035u
#define PPUTLIMPL_UINT_TRAIT_0xFC9u HEX, 4041u, -55, (F, C, 9), 0x036u
#define PPUTLIMPL_UINT_TRAIT_0xFC8u HEX, 4040u, -56, (F, C, 8), 0x037u
#define PPUTLIMPL_UINT_TRAIT_0xFC7u HEX, 4039u, -57, (F, C, 7), 0x038u
#define PPUTLIMPL_UINT_TRAIT_0xFC6u HEX, 4038u, -58, (F, C, 6), 0x039u
#define PPUTLIMPL_UINT_TRAIT_0xFC5u HEX, 4037u, -59, (F, C, 5), 0x03Au
#define PPUTLIMPL_UINT_TRAIT_0xFC4u HEX, 4036u, -60, (F, C, 4), 0x03Bu
#define PPUTLIMPL_UINT_TRAIT_0xFC3u HEX, 4035u, -61, (F, C, 3), 0x03Cu
#define PPUTLIMPL_UINT_TRAIT_0xFC2u HEX, 4034u, -62, (F, C, 2), 0x03Du
#define PPUTLIMPL_UINT_TRAIT_0xFC1u HEX, 4033u, -63, (F, C, 1), 0x03Eu
#define PPUTLIMPL_UINT_TRAIT_0xFC0u HEX, 4032u, -64, (F, C, 0), 0x03Fu
#define PPUTLIMPL_UINT_TRAIT_0xFBFu HEX, 4031u, -65, (F, B, F), 0x040u
#define PPUTLIMPL_UINT_TRAIT_0xFBEu HEX, 4030u, -66, (F, B, E), 0x041u
#define PPUTLIMPL_UINT_TRAIT_0xFBDu HEX, 4029u, -67, (F, B, D), 0x042u
#define PPUTLIMPL_UINT_TRAIT_0xFBCu HEX, 4028u, -68, (F, B, C), 0x043u
#define PPUTLIMPL_UINT_TRAIT_0xFBBu HEX, 4027u, -69, (F, B, B), 0x044u
#define PPUTLIMPL_UINT_TRAIT_0xFBAu HEX, 4026u, -70, (F, B, A), 0x045u
#define PPUTLIMPL_UINT_TRAIT_0xFB9u HEX, 4025u, -71, (F, B, 9), 0x046u
#define PPUTLIMPL_UINT_TRAIT_0xFB8u HEX, 4024u, -72, (F, B, 8), 0x047u
#define PPUTLIMPL_UINT_TRAIT_0xFB7u HEX, 4023u, -73, (F, B, 7), 0x048u
#define PPUTLIMPL_UINT_TRAIT_0xFB6u HEX, 4022u, -74, (F, B, 6), 0x049u
#define PPUTLIMPL_UINT_TRAIT_0xFB5u HEX, 4021u, -75, (F, B, 5), 0x04Au
#define PPUTLIMPL_UINT_TRAIT_0xFB4u HEX, 4020u, -76, (F, B, 4), 0x04Bu
#define PPUTLIMPL_UINT_TRAIT_0xFB3u HEX, 4019u, -77, (F, B, 3), 0x04Cu
#define PPUTLIMPL_UINT_TRAIT_0xFB2u HEX, 4018u, -78, (F, B, 2), 0x04Du
#define PPUTLIMPL_UINT_TRAIT_0xFB1u HEX, 4017u, -79, (F, B, 1), 0x04Eu
#define PPUTLIMPL_UINT_TRAIT_0xFB0u HEX, 4016u, -80, (F, B, 0), 0x04Fu
#define PPUTLIMPL_UINT_TRAIT_0xFAFu HEX, 4015u, -81, (F, A, F), 0x050u
#define PPUTLIMPL_UINT_TRAIT_0xFAEu HEX, 4014u, -82, (F, A, E), 0x051u
#define PPUTLIMPL_UINT_TRAIT_0xFADu HEX, 4013u, -83, (F, A, D), 0x052u
#define PPUTLIMPL_UINT_TRAIT_0xFACu HEX, 4012u, -84, (F, A, C), 0x053u
#define PPUTLIMPL_UINT_TRAIT_0xFABu HEX, 4011u, -85, (F, A, B), 0x054u
#define PPUTLIMPL_UINT_TRAIT_0xFAAu HEX, 4010u, -86, (F, A, A), 0x055u
#define PPUTLIMPL_UINT_TRAIT_0xFA9u HEX, 4009u, -87, (F, A, 9), 0x056u
#define PPUTLIMPL_UINT_TRAIT_0xFA8u HEX, 4008u, -88, (F, A, 8), 0x057u
#define PPUTLIMPL_UINT_TRAIT_0xFA7u HEX, 4007u, -89, (F, A, 7), 0x058u
#define PPUTLIMPL_UINT_TRAIT_0xFA6u HEX, 4006u, -90, (F, A, 6), 0x059u
#define PPUTLIMPL_UINT_TRAIT_0xFA5u HEX, 4005u, -91, (F, A, 5), 0x05Au
#define PPUTLIMPL_UINT_TRAIT_0xFA4u HEX, 4004u, -92, (F, A, 4), 0x05Bu
#define PPUTLIMPL_UINT_TRAIT_0xFA3u HEX, 4003u, -93, (F, A, 3), 0x05Cu
#define PPUTLIMPL_UINT_TRAIT_0xFA2u HEX, 4002u, -94, (F, A, 2), 0x05Du
#define PPUTLIMPL_UINT_TRAIT_0xFA1u HEX, 4001u, -95, (F, A, 1), 0x05Eu
#define PPUTLIMPL_UINT_TRAIT_0xFA0u HEX, 4000u, -96, (F, A, 0), 0x05Fu
#define PPUTLIMPL_UINT_TRAIT_0xF9Fu HEX, 3999u, -97, (F, 9, F), 0x060u
#define PPUTLIMPL_UINT_TRAIT_0xF9Eu HEX, 3998u, -98, (F, 9, E), 0x061u
#define PPUTLIMPL_UINT_TRAIT_0xF9Du HEX, 3997u, -99, (F, 9, D), 0x062u
#define PPUTLIMPL_UINT_TRAIT_0xF9Cu HEX, 3996u, -100, (F, 9, C), 0x063u
#define PPUTLIMPL_UINT_TRAIT_0xF9Bu HEX, 3995u, -101, (F, 9, B), 0x064u
#define PPUTLIMPL_UINT_TRAIT_0xF9Au HEX, 3994u, -102, (F, 9, A), 0x065u
#define PPUTLIMPL_UINT_TRAIT_0xF99u HEX, 3993u, -103, (F, 9, 9), 0x066u
#define PPUTLIMPL_UINT_TRAIT_0xF98u HEX, 3992u, -104, (F, 9, 8), 0x067u
#define PPUTLIMPL_UINT_TRAIT_0xF97u HEX, 3991u, -105, (F, 9, 7), 0x068u
#define PPUTLIMPL_UINT_TRAIT_0xF96u HEX, 3990u, -106, (F, 9, 6), 0x069u
#define PPUTLIMPL_UINT_TRAIT_0xF95u HEX, 3989u, -107, (F, 9, 5), 0x06Au
#define PPUTLIMPL_UINT_TRAIT_0xF94u HEX, 3988u, -108, (F, 9, 4), 0x06Bu
#define PPUTLIMPL_UINT_TRAIT_0xF93u HEX, 3987u, -109, (F, 9, 3), 0x06Cu
#define PPUTLIMPL_UINT_TRAIT_0xF92u HEX, 3986u, -110, (F, 9, 2), 0x06Du
#define PPUTLIMPL_UINT_TRAIT_0xF91u HEX, 3985u, -111, (F, 9, 1), 0x06Eu
#define PPUTLIMPL_UINT_TRAIT_0xF90u HEX, 3984u, -112, (F, 9, 0), 0x06Fu
#define PPUTLIMPL_UINT_TRAIT_0xF8Fu HEX, 3983u, -113, (F, 8, F), 0x070u
#define PPUTLIMPL_UINT_TRAIT_0xF8Eu HEX, 3982u, -114, (F, 8, E), 0x071u
#define PPUTLIMPL_UINT_TRAIT_0xF8Du HEX, 3981u, -115, (F, 8, D), 0x072u
#define PPUTLIMPL_UINT_TRAIT_0xF8Cu HEX, 3980u, -116, (F, 8, C), 0x073u
#define PPUTLIMPL_UINT_TRAIT_0xF8Bu HEX, 3979u, -117, (F, 8, B), 0x074u
#define PPUTLIMPL_UINT_TRAIT_0xF8Au HEX, 3978u, -118, (F, 8, A), 0x075u
#define PPUTLIMPL_UINT_TRAIT_0xF89u HEX, 3977u, -119, (F, 8, 9), 0x076u
#define PPUTLIMPL_UINT_TRAIT_0xF88u HEX, 3976u, -120, (F, 8, 8), 0x077u
#define PPUTLIMPL_UINT_TRAIT_0xF87u HEX, 3975u, -121, (F, 8, 7), 0x078u
#define PPUTLIMPL_UINT_TRAIT_0xF86u HEX, 3974u, -122, (F, 8, 6), 0x079u
#define PPUTLIMPL_UINT_TRAIT_0xF85u HEX, 3973u, -123, (F, 8, 5), 0x07Au
#define PPUTLIMPL_UINT_TRAIT_0xF84u HEX, 3972u, -124, (F, 8, 4), 0x07Bu
#define PPUTLIMPL_UINT_TRAIT_0xF83u HEX, 3971u, -125, (F, 8, 3), 0x07Cu
#define PPUTLIMPL_UINT_TRAIT_0xF82u HEX, 3970u, -126, (F, 8, 2), 0x07Du
#define PPUTLIMPL_UINT_TRAIT_0xF81u HEX, 3969u, -127, (F, 8, 1), 0x07Eu
#define PPUTLIMPL_UINT_TRAIT_0xF80u HEX, 3968u, -128, (F, 8, 0), 0x07Fu
#define PPUTLIMPL_UINT_TRAIT_0xF7Fu HEX, 3967u, -129, (F, 7, F), 0x080u
#define PPUTLIMPL_UINT_TRAIT_0xF7Eu HEX, 3966u, -130, (F, 7, E), 0x081u
#define PPUTLIMPL_UINT_TRAIT_0xF7Du HEX, 3965u, -131, (F, 7, D), 0x082u
#define PPUTLIMPL_UINT_TRAIT_0xF7Cu HEX, 3964u, -132, (F, 7, C), 0x083u
#define PPUTLIMPL_UINT_TRAIT_0xF7Bu HEX, 3963u, -133, (F, 7, B), 0x084u
#define PPUTLIMPL_UINT_TRAIT_0xF7Au HEX, 3962u, -134, (F, 7, A), 0x085u
#define PPUTLIMPL_UINT_TRAIT_0xF79u HEX, 3961u, -135, (F, 7, 9), 0x086u
#define PPUTLIMPL_UINT_TRAIT_0xF78u HEX, 3960u, -136, (F, 7, 8), 0x087u
#define PPUTLIMPL_UINT_TRAIT_0xF77u HEX, 3959u, -137, (F, 7, 7), 0x088u
#define PPUTLIMPL_UINT_TRAIT_0xF76u HEX, 3958u, -138, (F, 7, 6), 0x089u
#define PPUTLIMPL_UINT_TRAIT_0xF75u HEX, 3957u, -139, (F, 7, 5), 0x08Au
#define PPUTLIMPL_UINT_TRAIT_0xF74u HEX, 3956u, -140, (F, 7, 4), 0x08Bu
#define PPUTLIMPL_UINT_TRAIT_0xF73u HEX, 3955u, -141, (F, 7, 3), 0x08Cu
#define PPUTLIMPL_UINT_TRAIT_0xF72u HEX, 3954u, -142, (F, 7, 2), 0x08Du
#define PPUTLIMPL_UINT_TRAIT_0xF71u HEX, 3953u, -143, (F, 7, 1), 0x08Eu
#define PPUTLIMPL_UINT_TRAIT_0xF70u HEX, 3952u, -144, (F, 7, 0), 0x08Fu
#define PPUTLIMPL_UINT_TRAIT_0xF6Fu HEX, 3951u, -145, (F, 6, F), 0x090u
#define PPUTLIMPL_UINT_TRAIT_0xF6Eu HEX, 3950u, -146, (F, 6, E), 0x091u
#define PPUTLIMPL_UINT_TRAIT_0xF6Du HEX, 3949u, -147, (F, 6, D), 0x092u
#define PPUTLIMPL_UINT_TRAIT_0xF6Cu HEX, 3948u, -148, (F, 6, C), 0x093u
#define PPUTLIMPL_UINT_TRAIT_0xF6Bu HEX, 3947u, -149, (F, 6, B), 0x094u
#define PPUTLIMPL_UINT_TRAIT_0xF6Au HEX, 3946u, -150, (F, 6, A), 0x095u
#define PPUTLIMPL_UINT_TRAIT_0xF69u HEX, 3945u, -151, (F, 6, 9), 0x096u
#define PPUTLIMPL_UINT_TRAIT_0xF68u HEX, 3944u, -152, (F, 6, 8), 0x097u
#define PPUTLIMPL_UINT_TRAIT_0xF67u HEX, 3943u, -153, (F, 6, 7), 0x098u
#define PPUTLIMPL_UINT_TRAIT_0xF66u HEX, 3942u, -154, (F, 6, 6), 0x099u
#define PPUTLIMPL_UINT_TRAIT_0xF65u HEX, 3941u, -155, (F, 6, 5), 0x09Au
#define PPUTLIMPL_UINT_TRAIT_0xF64u HEX, 3940u, -156, (F, 6, 4), 0x09Bu
#define PPUTLIMPL_UINT_TRAIT_0xF63u HEX, 3939u, -157, (F, 6, 3), 0x09Cu
#define PPUTLIMPL_UINT_TRAIT_0xF62u HEX, 3938u, -158, (F, 6, 2), 0x09Du
#define PPUTLIMPL_UINT_TRAIT_0xF61u HEX, 3937u, -159, (F, 6, 1), 0x09Eu
#define PPUTLIMPL_UINT_TRAIT_0xF60u HEX, 3936u, -160, (F, 6, 0), 0x09Fu
#define PPUTLIMPL_UINT_TRAIT_0xF5Fu HEX, 3935u, -161, (F, 5, F), 0x0A0u
#define PPUTLIMPL_UINT_TRAIT_0xF5Eu HEX, 3934u, -162, (F, 5, E), 0x0A1u
#define PPUTLIMPL_UINT_TRAIT_0xF5Du HEX, 3933u, -163, (F, 5, D), 0x0A2u
#define PPUTLIMPL_UINT_TRAIT_0xF5Cu HEX, 3932u, -164, (F, 5, C), 0x0A3u
#define PPUTLIMPL_UINT_TRAIT_0xF5Bu HEX, 3931u, -165, (F, 5, B), 0x0A4u
#define PPUTLIMPL_UINT_TRAIT_0xF5Au HEX, 3930u, -166, (F, 5, A), 0x0A5u
#define PPUTLIMPL_UINT_TRAIT_0xF59u HEX, 3929u, -167, (F, 5, 9), 0x0A6u
#define PPUTLIMPL_UINT_TRAIT_0xF58u HEX, 3928u, -168, (F, 5, 8), 0x0A7u
#define PPUTLIMPL_UINT_TRAIT_0xF57u HEX, 3927u, -169, (F, 5, 7), 0x0A8u
#define PPUTLIMPL_UINT_TRAIT_0xF56u HEX, 3926u, -170, (F, 5, 6), 0x0A9u
#define PPUTLIMPL_UINT_TRAIT_0xF55u HEX, 3925u, -171, (F, 5, 5), 0x0AAu
#define PPUTLIMPL_UINT_TRAIT_0xF54u HEX, 3924u, -172, (F, 5, 4), 0x0ABu
#define PPUTLIMPL_UINT_TRAIT_0xF53u HEX, 3923u, -173, (F, 5, 3), 0x0ACu
#define PPUTLIMPL_UINT_TRAIT_0xF52u HEX, 3922u, -174, (F, 5, 2), 0x0ADu
#define PPUTLIMPL_UINT_TRAIT_0xF51u HEX, 3921u, -175, (F, 5, 1), 0x0AEu
#define PPUTLIMPL_UINT_TRAIT_0xF50u HEX, 3920u, -176, (F, 5, 0), 0x0AFu
#define PPUTLIMPL_UINT_TRAIT_0xF4Fu HEX, 3919u, -177, (F, 4, F), 0x0B0u
#define PPUTLIMPL_UINT_TRAIT_0xF4Eu HEX, 3918u, -178, (F, 4, E), 0x0B1u
#define PPUTLIMPL_UINT_TRAIT_0xF4Du HEX, 3917u, -179, (F, 4, D), 0x0B2u
#define PPUTLIMPL_UINT_TRAIT_0xF4Cu HEX, 3916u, -180, (F, 4, C), 0x0B3u
#define PPUTLIMPL_UINT_TRAIT_0xF4Bu HEX, 3915u, -181, (F, 4, B), 0x0B4u
#define PPUTLIMPL_UINT_TRAIT_0xF4Au HEX, 3914u, -182, (F, 4, A), 0x0B5u
#define PPUTLIMPL_UINT_TRAIT_0xF49u HEX, 3913u, -183, (F, 4, 9), 0x0B6u
#define PPUTLIMPL_UINT_TRAIT_0xF48u HEX, 3912u, -184, (F, 4, 8), 0x0B7u
#define PPUTLIMPL_UINT_TRAIT_0xF47u HEX, 3911u, -185, (F, 4, 7), 0x0B8u
#define PPUTLIMPL_UINT_TRAIT_0xF46u HEX, 3910u, -186, (F, 4, 6), 0x0B9u
#define PPUTLIMPL_UINT_TRAIT_0xF45u HEX, 3909u, -187, (F, 4, 5), 0x0BAu
#define PPUTLIMPL_UINT_TRAIT_0xF44u HEX, 3908u, -188, (F, 4, 4), 0x0BBu
#define PPUTLIMPL_UINT_TRAIT_0xF43u HEX, 3907u, -189, (F, 4, 3), 0x0BCu
#define PPUTLIMPL_UINT_TRAIT_0xF42u HEX, 3906u, -190, (F, 4, 2), 0x0BDu
#define PPUTLIMPL_UINT_TRAIT_0xF41u HEX, 3905u, -191, (F, 4, 1), 0x0BEu
#define PPUTLIMPL_UINT_TRAIT_0xF40u HEX, 3904u, -192, (F, 4, 0), 0x0BFu
#define PPUTLIMPL_UINT_TRAIT_0xF3Fu HEX, 3903u, -193, (F, 3, F), 0x0C0u
#define PPUTLIMPL_UINT_TRAIT_0xF3Eu HEX, 3902u, -194, (F, 3, E), 0x0C1u
#define PPUTLIMPL_UINT_TRAIT_0xF3Du HEX, 3901u, -195, (F, 3, D), 0x0C2u
#define PPUTLIMPL_UINT_TRAIT_0xF3Cu HEX, 3900u, -196, (F, 3, C), 0x0C3u
#define PPUTLIMPL_UINT_TRAIT_0xF3Bu HEX, 3899u, -197, (F, 3, B), 0x0C4u
#define PPUTLIMPL_UINT_TRAIT_0xF3Au HEX, 3898u, -198, (F, 3, A), 0x0C5u
#define PPUTLIMPL_UINT_TRAIT_0xF39u HEX, 3897u, -199, (F, 3, 9), 0x0C6u
#define PPUTLIMPL_UINT_TRAIT_0xF38u HEX, 3896u, -200, (F, 3, 8), 0x0C7u
#define PPUTLIMPL_UINT_TRAIT_0xF37u HEX, 3895u, -201, (F, 3, 7), 0x0C8u
#define PPUTLIMPL_UINT_TRAIT_0xF36u HEX, 3894u, -202, (F, 3, 6), 0x0C9u
#define PPUTLIMPL_UINT_TRAIT_0xF35u HEX, 3893u, -203, (F, 3, 5), 0x0CAu
#define PPUTLIMPL_UINT_TRAIT_0xF34u HEX, 3892u, -204, (F, 3, 4), 0x0CBu
#define PPUTLIMPL_UINT_TRAIT_0xF33u HEX, 3891u, -205, (F, 3, 3), 0x0CCu
#define PPUTLIMPL_UINT_TRAIT_0xF32u HEX, 3890u, -206, (F, 3, 2), 0x0CDu
#define PPUTLIMPL_UINT_TRAIT_0xF31u HEX, 3889u, -207, (F, 3, 1), 0x0CEu
#define PPUTLIMPL_UINT_TRAIT_0xF30u HEX, 3888u, -208, (F, 3, 0), 0x0CFu
#define PPUTLIMPL_UINT_TRAIT_0xF2Fu HEX, 3887u, -209, (F, 2, F), 0x0D0u
#define PPUTLIMPL_UINT_TRAIT_0xF2Eu HEX, 3886u, -210, (F, 2, E), 0x0D1u
#define PPUTLIMPL_UINT_TRAIT_0xF2Du HEX, 3885u, -211, (F, 2, D), 0x0D2u
#define PPUTLIMPL_UINT_TRAIT_0xF2Cu HEX, 3884u, -212, (F, 2, C), 0x0D3u
#define PPUTLIMPL_UINT_TRAIT_0xF2Bu HEX, 3883u, -213, (F, 2, B), 0x0D4u
#define PPUTLIMPL_UINT_TRAIT_0xF2Au HEX, 3882u, -214, (F, 2, A), 0x0D5u
#define PPUTLIMPL_UINT_TRAIT_0xF29u HEX, 3881u, -215, (F, 2, 9), 0x0D6u
#define PPUTLIMPL_UINT_TRAIT_0xF28u HEX, 3880u, -216, (F, 2, 8), 0x0D7u
#define PPUTLIMPL_UINT_TRAIT_0xF27u HEX, 3879u, -217, (F, 2, 7), 0x0D8u
#define PPUTLIMPL_UINT_TRAIT_0xF26u HEX, 3878u, -218, (F, 2, 6), 0x0D9u
#define PPUTLIMPL_UINT_TRAIT_0xF25u HEX, 3877u, -219, (F, 2, 5), 0x0DAu
#define PPUTLIMPL_UINT_TRAIT_0xF24u HEX, 3876u, -220, (F, 2, 4), 0x0DBu
#define PPUTLIMPL_UINT_TRAIT_0xF23u HEX, 3875u, -221, (F, 2, 3), 0x0DCu
#define PPUTLIMPL_UINT_TRAIT_0xF22u HEX, 3874u, -222, (F, 2, 2), 0x0DDu
#define PPUTLIMPL_UINT_TRAIT_0xF21u HEX, 3873u, -223, (F, 2, 1), 0x0DEu
#define PPUTLIMPL_UINT_TRAIT_0xF20u HEX, 3872u, -224, (F, 2, 0), 0x0DFu
#define PPUTLIMPL_UINT_TRAIT_0xF1Fu HEX, 3871u, -225, (F, 1, F), 0x0E0u
#define PPUTLIMPL_UINT_TRAIT_0xF1Eu HEX, 3870u, -226, (F, 1, E), 0x0E1u
#define PPUTLIMPL_UINT_TRAIT_0xF1Du HEX, 3869u, -227, (F, 1, D), 0x0E2u
#define PPUTLIMPL_UINT_TRAIT_0xF1Cu HEX, 3868u, -228, (F, 1, C), 0x0E3u
#define PPUTLIMPL_UINT_TRAIT_0xF1Bu HEX, 3867u, -229, (F, 1, B), 0x0E4u
#define PPUTLIMPL_UINT_TRAIT_0xF1Au HEX, 3866u, -230, (F, 1, A), 0x0E5u
#define PPUTLIMPL_UINT_TRAIT_0xF19u HEX, 3865u, -231, (F, 1, 9), 0x0E6u
#define PPUTLIMPL_UINT_TRAIT_0xF18u HEX, 3864u, -232, (F, 1, 8), 0x0E7u
#define PPUTLIMPL_UINT_TRAIT_0xF17u HEX, 3863u, -233, (F, 1, 7), 0x0E8u
#define PPUTLIMPL_UINT_TRAIT_0xF16u HEX, 3862u, -234, (F, 1, 6), 0x0E9u
#define PPUTLIMPL_UINT_TRAIT_0xF15u HEX, 3861u, -235, (F, 1, 5), 0x0EAu
#define PPUTLIMPL_UINT_TRAIT_0xF14u HEX, 3860u, -236, (F, 1, 4), 0x0EBu
#define PPUTLIMPL_UINT_TRAIT_0xF13u HEX, 3859u, -237, (F, 1, 3), 0x0ECu
#define PPUTLIMPL_UINT_TRAIT_0xF12u HEX, 3858u, -238, (F, 1, 2), 0x0EDu
#define PPUTLIMPL_UINT_TRAIT_0xF11u HEX, 3857u, -239, (F, 1, 1), 0x0EEu
#define PPUTLIMPL_UINT_TRAIT_0xF10u HEX, 3856u, -240, (F, 1, 0), 0x0EFu
#define PPUTLIMPL_UINT_TRAIT_0xF0Fu HEX, 3855u, -241, (F, 0, F), 0x0F0u
#define PPUTLIMPL_UINT_TRAIT_0xF0Eu HEX, 3854u, -242, (F, 0, E), 0x0F1u
#define PPUTLIMPL_UINT_TRAIT_0xF0Du HEX, 3853u, -243, (F, 0, D), 0x0F2u
#define PPUTLIMPL_UINT_TRAIT_0xF0Cu HEX, 3852u, -244, (F, 0, C), 0x0F3u
#define PPUTLIMPL_UINT_TRAIT_0xF0Bu HEX, 3851u, -245, (F, 0, B), 0x0F4u
#define PPUTLIMPL_UINT_TRAIT_0xF0Au HEX, 3850u, -246, (F, 0, A), 0x0F5u
#define PPUTLIMPL_UINT_TRAIT_0xF09u HEX, 3849u, -247, (F, 0, 9), 0x0F6u
#define PPUTLIMPL_UINT_TRAIT_0xF08u HEX, 3848u, -248, (F, 0, 8), 0x0F7u
#define PPUTLIMPL_UINT_TRAIT_0xF07u HEX, 3847u, -249, (F, 0, 7), 0x0F8u
#define PPUTLIMPL_UINT_TRAIT_0xF06u HEX, 3846u, -250, (F, 0, 6), 0x0F9u
#define PPUTLIMPL_UINT_TRAIT_0xF05u HEX, 3845u, -251, (F, 0, 5), 0x0FAu
#define PPUTLIMPL_UINT_TRAIT_0xF04u HEX, 3844u, -252, (F, 0, 4), 0x0FBu
#define PPUTLIMPL_UINT_TRAIT_0xF03u HEX, 3843u, -253, (F, 0, 3), 0x0FCu
#define PPUTLIMPL_UINT_TRAIT_0xF02u HEX, 3842u, -254, (F, 0, 2), 0x0FDu
#define PPUTLIMPL_UINT_TRAIT_0xF01u HEX, 3841u, -255, (F, 0, 1), 0x0FEu
#define PPUTLIMPL_UINT_TRAIT_0xF00u HEX, 3840u, -256, (F, 0, 0), 0x0FFu
#define PPUTLIMPL_UINT_TRAIT_0xEFFu HEX, 3839u, -257, (E, F, F), 0x100u
#define PPUTLIMPL_UINT_TRAIT_0xEFEu HEX, 3838u, -258, (E, F, E), 0x101u
#define PPUTLIMPL_UINT_TRAIT_0xEFDu HEX, 3837u, -259, (E, F, D), 0x102u
#define PPUTLIMPL_UINT_TRAIT_0xEFCu HEX, 3836u, -260, (E, F, C), 0x103u
#define PPUTLIMPL_UINT_TRAIT_0xEFBu HEX, 3835u, -261, (E, F, B), 0x104u
#define PPUTLIMPL_UINT_TRAIT_0xEFAu HEX, 3834u, -262, (E, F, A), 0x105u
#define PPUTLIMPL_UINT_TRAIT_0xEF9u HEX, 3833u, -263, (E, F, 9), 0x106u
#define PPUTLIMPL_UINT_TRAIT_0xEF8u HEX, 3832u, -264, (E, F, 8), 0x107u
#define PPUTLIMPL_UINT_TRAIT_0xEF7u HEX, 3831u, -265, (E, F, 7), 0x108u
#define PPUTLIMPL_UINT_TRAIT_0xEF6u HEX, 3830u, -266, (E, F, 6), 0x109u
#define PPUTLIMPL_UINT_TRAIT_0xEF5u HEX, 3829u, -267, (E, F, 5), 0x10Au
#define PPUTLIMPL_UINT_TRAIT_0xEF4u HEX, 3828u, -268, (E, F, 4), 0x10Bu
#define PPUTLIMPL_UINT_TRAIT_0xEF3u HEX, 3827u, -269, (E, F, 3), 0x10Cu
#define PPUTLIMPL_UINT_TRAIT_0xEF2u HEX, 3826u, -270, (E, F, 2), 0x10Du
#define PPUTLIMPL_UINT_TRAIT_0xEF1u HEX, 3825u, -271, (E, F, 1), 0x10Eu
#define PPUTLIMPL_UINT_TRAIT_0xEF0u HEX, 3824u, -272, (E, F, 0), 0x10Fu
#define PPUTLIMPL_UINT_TRAIT_0xEEFu HEX, 3823u, -273, (E, E, F), 0x110u
#define PPUTLIMPL_UINT_TRAIT_0xEEEu HEX, 3822u, -274, (E, E, E), 0x111u
#define PPUTLIMPL_UINT_TRAIT_0xEEDu HEX, 3821u, -275, (E, E, D), 0x112u
#define PPUTLIMPL_UINT_TRAIT_0xEECu HEX, 3820u, -276, (E, E, C), 0x113u
#define PPUTLIMPL_UINT_TRAIT_0xEEBu HEX, 3819u, -277, (E, E, B), 0x114u
#define PPUTLIMPL_UINT_TRAIT_0xEEAu HEX, 3818u, -278, (E, E, A), 0x115u
#define PPUTLIMPL_UINT_TRAIT_0xEE9u HEX, 3817u, -279, (E, E, 9), 0x116u
#define PPUTLIMPL_UINT_TRAIT_0xEE8u HEX, 3816u, -280, (E, E, 8), 0x117u
#define PPUTLIMPL_UINT_TRAIT_0xEE7u HEX, 3815u, -281, (E, E, 7), 0x118u
#define PPUTLIMPL_UINT_TRAIT_0xEE6u HEX, 3814u, -282, (E, E, 6), 0x119u
#define PPUTLIMPL_UINT_TRAIT_0xEE5u HEX, 3813u, -283, (E, E, 5), 0x11Au
#define PPUTLIMPL_UINT_TRAIT_0xEE4u HEX, 3812u, -284, (E, E, 4), 0x11Bu
#define PPUTLIMPL_UINT_TRAIT_0xEE3u HEX, 3811u, -285, (E, E, 3), 0x11Cu
#define PPUTLIMPL_UINT_TRAIT_0xEE2u HEX, 3810u, -286, (E, E, 2), 0x11Du
#define PPUTLIMPL_UINT_TRAIT_0xEE1u HEX, 3809u, -287, (E, E, 1), 0x11Eu
#define PPUTLIMPL_UINT_TRAIT_0xEE0u HEX, 3808u, -288, (E, E, 0), 0x11Fu
#define PPUTLIMPL_UINT_TRAIT_0xEDFu HEX, 3807u, -289, (E, D, F), 0x120u
#define PPUTLIMPL_UINT_TRAIT_0xEDEu HEX, 3806u, -290, (E, D, E), 0x121u
#define PPUTLIMPL_UINT_TRAIT_0xEDDu HEX, 3805u, -291, (E, D, D), 0x122u
#define PPUTLIMPL_UINT_TRAIT_0xEDCu HEX, 3804u, -292, (E, D, C), 0x123u
#define PPUTLIMPL_UINT_TRAIT_0xEDBu HEX, 3803u, -293, (E, D, B), 0x124u
#define PPUTLIMPL_UINT_TRAIT_0xEDAu HEX, 3802u, -294, (E, D, A), 0x125u
#define PPUTLIMPL_UINT_TRAIT_0xED9u HEX, 3801u, -295, (E, D, 9), 0x126u
#define PPUTLIMPL_UINT_TRAIT_0xED8u HEX, 3800u, -296, (E, D, 8), 0x127u
#define PPUTLIMPL_UINT_TRAIT_0xED7u HEX, 3799u, -297, (E, D, 7), 0x128u
#define PPUTLIMPL_UINT_TRAIT_0xED6u HEX, 3798u, -298, (E, D, 6), 0x129u
#define PPUTLIMPL_UINT_TRAIT_0xED5u HEX, 3797u, -299, (E, D, 5), 0x12Au
#define PPUTLIMPL_UINT_TRAIT_0xED4u HEX, 3796u, -300, (E, D, 4), 0x12Bu
#define PPUTLIMPL_UINT_TRAIT_0xED3u HEX, 3795u, -301, (E, D, 3), 0x12Cu
#define PPUTLIMPL_UINT_TRAIT_0xED2u HEX, 3794u, -302, (E, D, 2), 0x12Du
#define PPUTLIMPL_UINT_TRAIT_0xED1u HEX, 3793u, -303, (E, D, 1), 0x12Eu
#define PPUTLIMPL_UINT_TRAIT_0xED0u HEX, 3792u, -304, (E, D, 0), 0x12Fu
#define PPUTLIMPL_UINT_TRAIT_0xECFu HEX, 3791u, -305, (E, C, F), 0x130u
#define PPUTLIMPL_UINT_TRAIT_0xECEu HEX, 3790u, -306, (E, C, E), 0x131u
#define PPUTLIMPL_UINT_TRAIT_0xECDu HEX, 3789u, -307, (E, C, D), 0x132u
#define PPUTLIMPL_UINT_TRAIT_0xECCu HEX, 3788u, -308, (E, C, C), 0x133u
#define PPUTLIMPL_UINT_TRAIT_0xECBu HEX, 3787u, -309, (E, C, B), 0x134u
#define PPUTLIMPL_UINT_TRAIT_0xECAu HEX, 3786u, -310, (E, C, A), 0x135u
#define PPUTLIMPL_UINT_TRAIT_0xEC9u HEX, 3785u, -311, (E, C, 9), 0x136u
#define PPUTLIMPL_UINT_TRAIT_0xEC8u HEX, 3784u, -312, (E, C, 8), 0x137u
#define PPUTLIMPL_UINT_TRAIT_0xEC7u HEX, 3783u, -313, (E, C, 7), 0x138u
#define PPUTLIMPL_UINT_TRAIT_0xEC6u HEX, 3782u, -314, (E, C, 6), 0x139u
#define PPUTLIMPL_UINT_TRAIT_0xEC5u HEX, 3781u, -315, (E, C, 5), 0x13Au
#define PPUTLIMPL_UINT_TRAIT_0xEC4u HEX, 3780u, -316, (E, C, 4), 0x13Bu
#define PPUTLIMPL_UINT_TRAIT_0xEC3u HEX, 3779u, -317, (E, C, 3), 0x13Cu
#define PPUTLIMPL_UINT_TRAIT_0xEC2u HEX, 3778u, -318, (E, C, 2), 0x13Du
#define PPUTLIMPL_UINT_TRAIT_0xEC1u HEX, 3777u, -319, (E, C, 1), 0x13Eu
#define PPUTLIMPL_UINT_TRAIT_0xEC0u HEX, 3776u, -320, (E, C, 0), 0x13Fu
#define PPUTLIMPL_UINT_TRAIT_0xEBFu HEX, 3775u, -321, (E, B, F), 0x140u
#define PPUTLIMPL_UINT_TRAIT_0xEBEu HEX, 3774u, -322, (E, B, E), 0x141u
#define PPUTLIMPL_UINT_TRAIT_0xEBDu HEX, 3773u, -323, (E, B, D), 0x142u
#define PPUTLIMPL_UINT_TRAIT_0xEBCu HEX, 3772u, -324, (E, B, C), 0x143u
#define PPUTLIMPL_UINT_TRAIT_0xEBBu HEX, 3771u, -325, (E, B, B), 0x144u
#define PPUTLIMPL_UINT_TRAIT_0xEBAu HEX, 3770u, -326, (E, B, A), 0x145u
#define PPUTLIMPL_UINT_TRAIT_0xEB9u HEX, 3769u, -327, (E, B, 9), 0x146u
#define PPUTLIMPL_UINT_TRAIT_0xEB8u HEX, 3768u, -328, (E, B, 8), 0x147u
#define PPUTLIMPL_UINT_TRAIT_0xEB7u HEX, 3767u, -329, (E, B, 7), 0x148u
#define PPUTLIMPL_UINT_TRAIT_0xEB6u HEX, 3766u, -330, (E, B, 6), 0x149u
#define PPUTLIMPL_UINT_TRAIT_0xEB5u HEX, 3765u, -331, (E, B, 5), 0x14Au
#define PPUTLIMPL_UINT_TRAIT_0xEB4u HEX, 3764u, -332, (E, B, 4), 0x14Bu
#define PPUTLIMPL_UINT_TRAIT_0xEB3u HEX, 3763u, -333, (E, B, 3), 0x14Cu
#define PPUTLIMPL_UINT_TRAIT_0xEB2u HEX, 3762u, -334, (E, B, 2), 0x14Du
#define PPUTLIMPL_UINT_TRAIT_0xEB1u HEX, 3761u, -335, (E, B, 1), 0x14Eu
#define PPUTLIMPL_UINT_TRAIT_0xEB0u HEX, 3760u, -336, (E, B, 0), 0x14Fu
#define PPUTLIMPL_UINT_TRAIT_0xEAFu HEX, 3759u, -337, (E, A, F), 0x150u
#define PPUTLIMPL_UINT_TRAIT_0xEAEu HEX, 3758u, -338, (E, A, E), 0x151u
#define PPUTLIMPL_UINT_TRAIT_0xEADu HEX, 3757u, -339, (E, A, D), 0x152u
#define PPUTLIMPL_UINT_TRAIT_0xEACu HEX, 3756u, -340, (E, A, C), 0x153u
#define PPUTLIMPL_UINT_TRAIT_0xEABu HEX, 3755u, -341, (E, A, B), 0x154u
#define PPUTLIMPL_UINT_TRAIT_0xEAAu HEX, 3754u, -342, (E, A, A), 0x155u
#define PPUTLIMPL_UINT_TRAIT_0xEA9u HEX, 3753u, -343, (E, A, 9), 0x156u
#define PPUTLIMPL_UINT_TRAIT_0xEA8u HEX, 3752u, -344, (E, A, 8), 0x157u
#define PPUTLIMPL_UINT_TRAIT_0xEA7u HEX, 3751u, -345, (E, A, 7), 0x158u
#define PPUTLIMPL_UINT_TRAIT_0xEA6u HEX, 3750u, -346, (E, A, 6), 0x159u
#define PPUTLIMPL_UINT_TRAIT_0xEA5u HEX, 3749u, -347, (E, A, 5), 0x15Au
#define PPUTLIMPL_UINT_TRAIT_0xEA4u HEX, 3748u, -348, (E, A, 4), 0x15Bu
#define PPUTLIMPL_UINT_TRAIT_0xEA3u HEX, 3747u, -349, (E, A, 3), 0x15Cu
#define PPUTLIMPL_UINT_TRAIT_0xEA2u HEX, 3746u, -350, (E, A, 2), 0x15Du
#define PPUTLIMPL_UINT_TRAIT_0xEA1u HEX, 3745u, -351, (E, A, 1), 0x15Eu
#define PPUTLIMPL_UINT_TRAIT_0xEA0u HEX, 3744u, -352, (E, A, 0), 0x15Fu
#define PPUTLIMPL_UINT_TRAIT_0xE9Fu HEX, 3743u, -353, (E, 9, F), 0x160u
#define PPUTLIMPL_UINT_TRAIT_0xE9Eu HEX, 3742u, -354, (E, 9, E), 0x161u
#define PPUTLIMPL_UINT_TRAIT_0xE9Du HEX, 3741u, -355, (E, 9, D), 0x162u
#define PPUTLIMPL_UINT_TRAIT_0xE9Cu HEX, 3740u, -356, (E, 9, C), 0x163u
#define PPUTLIMPL_UINT_TRAIT_0xE9Bu HEX, 3739u, -357, (E, 9, B), 0x164u
#define PPUTLIMPL_UINT_TRAIT_0xE9Au HEX, 3738u, -358, (E, 9, A), 0x165u
#define PPUTLIMPL_UINT_TRAIT_0xE99u HEX, 3737u, -359, (E, 9, 9), 0x166u
#define PPUTLIMPL_UINT_TRAIT_0xE98u HEX, 3736u, -360, (E, 9, 8), 0x167u
#define PPUTLIMPL_UINT_TRAIT_0xE97u HEX, 3735u, -361, (E, 9, 7), 0x168u
#define PPUTLIMPL_UINT_TRAIT_0xE96u HEX, 3734u, -362, (E, 9, 6), 0x169u
#define PPUTLIMPL_UINT_TRAIT_0xE95u HEX, 3733u, -363, (E, 9, 5), 0x16Au
#define PPUTLIMPL_UINT_TRAIT_0xE94u HEX, 3732u, -364, (E, 9, 4), 0x16Bu
#define PPUTLIMPL_UINT_TRAIT_0xE93u HEX, 3731u, -365, (E, 9, 3), 0x16Cu
#define PPUTLIMPL_UINT_TRAIT_0xE92u HEX, 3730u, -366, (E, 9, 2), 0x16Du
#define PPUTLIMPL_UINT_TRAIT_0xE91u HEX, 3729u, -367, (E, 9, 1), 0x16Eu
#define PPUTLIMPL_UINT_TRAIT_0xE90u HEX, 3728u, -368, (E, 9, 0), 0x16Fu
#define PPUTLIMPL_UINT_TRAIT_0xE8Fu HEX, 3727u, -369, (E, 8, F), 0x170u
#define PPUTLIMPL_UINT_TRAIT_0xE8Eu HEX, 3726u, -370, (E, 8, E), 0x171u
#define PPUTLIMPL_UINT_TRAIT_0xE8Du HEX, 3725u, -371, (E, 8, D), 0x172u
#define PPUTLIMPL_UINT_TRAIT_0xE8Cu HEX, 3724u, -372, (E, 8, C), 0x173u
#define PPUTLIMPL_UINT_TRAIT_0xE8Bu HEX, 3723u, -373, (E, 8, B), 0x174u
#define PPUTLIMPL_UINT_TRAIT_0xE8Au HEX, 3722u, -374, (E, 8, A), 0x175u
#define PPUTLIMPL_UINT_TRAIT_0xE89u HEX, 3721u, -375, (E, 8, 9), 0x176u
#define PPUTLIMPL_UINT_TRAIT_0xE88u HEX, 3720u, -376, (E, 8, 8), 0x177u
#define PPUTLIMPL_UINT_TRAIT_0xE87u HEX, 3719u, -377, (E, 8, 7), 0x178u
#define PPUTLIMPL_UINT_TRAIT_0xE86u HEX, 3718u, -378, (E, 8, 6), 0x179u
#define PPUTLIMPL_UINT_TRAIT_0xE85u HEX, 3717u, -379, (E, 8, 5), 0x17Au
#define PPUTLIMPL_UINT_TRAIT_0xE84u HEX, 3716u, -380, (E, 8, 4), 0x17Bu
#define PPUTLIMPL_UINT_TRAIT_0xE83u HEX, 3715u, -381, (E, 8, 3), 0x17Cu
#define PPUTLIMPL_UINT_TRAIT_0xE82u HEX, 3714u, -382, (E, 8, 2), 0x17Du
#define PPUTLIMPL_UINT_TRAIT_0xE81u HEX, 3713u, -383, (E, 8, 1), 0x17Eu
#define PPUTLIMPL_UINT_TRAIT_0xE80u HEX, 3712u, -384, (E, 8, 0), 0x17Fu
#define PPUTLIMPL_UINT_TRAIT_0xE7Fu HEX, 3711u, -385, (E, 7, F), 0x180u
#define PPUTLIMPL_UINT_TRAIT_0xE7Eu HEX, 3710u, -386, (E, 7, E), 0x181u
#define PPUTLIMPL_UINT_TRAIT_0xE7Du HEX, 3709u, -387, (E, 7, D), 0x182u
#define PPUTLIMPL_UINT_TRAIT_0xE7Cu HEX, 3708u, -388, (E, 7, C), 0x183u
#define PPUTLIMPL_UINT_TRAIT_0xE7Bu HEX, 3707u, -389, (E, 7, B), 0x184u
#define PPUTLIMPL_UINT_TRAIT_0xE7Au HEX, 3706u, -390, (E, 7, A), 0x185u
#define PPUTLIMPL_UINT_TRAIT_0xE79u HEX, 3705u, -391, (E, 7, 9), 0x186u
#define PPUTLIMPL_UINT_TRAIT_0xE78u HEX, 3704u, -392, (E, 7, 8), 0x187u
#define PPUTLIMPL_UINT_TRAIT_0xE77u HEX, 3703u, -393, (E, 7, 7), 0x188u
#define PPUTLIMPL_UINT_TRAIT_0xE76u HEX, 3702u, -394, (E, 7, 6), 0x189u
#define PPUTLIMPL_UINT_TRAIT_0xE75u HEX, 3701u, -395, (E, 7, 5), 0x18Au
#define PPUTLIMPL_UINT_TRAIT_0xE74u HEX, 3700u, -396, (E, 7, 4), 0x18Bu
#define PPUTLIMPL_UINT_TRAIT_0xE73u HEX, 3699u, -397, (E, 7, 3), 0x18Cu
#define PPUTLIMPL_UINT_TRAIT_0xE72u HEX, 3698u, -398, (E, 7, 2), 0x18Du
#define PPUTLIMPL_UINT_TRAIT_0xE71u HEX, 3697u, -399, (E, 7, 1), 0x18Eu
#define PPUTLIMPL_UINT_TRAIT_0xE70u HEX, 3696u, -400, (E, 7, 0), 0x18Fu
#define PPUTLIMPL_UINT_TRAIT_0xE6Fu HEX, 3695u, -401, (E, 6, F), 0x190u
#define PPUTLIMPL_UINT_TRAIT_0xE6Eu HEX, 3694u, -402, (E, 6, E), 0x191u
#define PPUTLIMPL_UINT_TRAIT_0xE6Du HEX, 3693u, -403, (E, 6, D), 0x192u
#define PPUTLIMPL_UINT_TRAIT_0xE6Cu HEX, 3692u, -404, (E, 6, C), 0x193u
#define PPUTLIMPL_UINT_TRAIT_0xE6Bu HEX, 3691u, -405, (E, 6, B), 0x194u
#define PPUTLIMPL_UINT_TRAIT_0xE6Au HEX, 3690u, -406, (E, 6, A), 0x195u
#define PPUTLIMPL_UINT_TRAIT_0xE69u HEX, 3689u, -407, (E, 6, 9), 0x196u
#define PPUTLIMPL_UINT_TRAIT_0xE68u HEX, 3688u, -408, (E, 6, 8), 0x197u
#define PPUTLIMPL_UINT_TRAIT_0xE67u HEX, 3687u, -409, (E, 6, 7), 0x198u
#define PPUTLIMPL_UINT_TRAIT_0xE66u HEX, 3686u, -410, (E, 6, 6), 0x199u
#define PPUTLIMPL_UINT_TRAIT_0xE65u HEX, 3685u, -411, (E, 6, 5), 0x19Au
#define PPUTLIMPL_UINT_TRAIT_0xE64u HEX, 3684u, -412, (E, 6, 4), 0x19Bu
#define PPUTLIMPL_UINT_TRAIT_0xE63u HEX, 3683u, -413, (E, 6, 3), 0x19Cu
#define PPUTLIMPL_UINT_TRAIT_0xE62u HEX, 3682u, -414, (E, 6, 2), 0x19Du
#define PPUTLIMPL_UINT_TRAIT_0xE61u HEX, 3681u, -415, (E, 6, 1), 0x19Eu
#define PPUTLIMPL_UINT_TRAIT_0xE60u HEX, 3680u, -416, (E, 6, 0), 0x19Fu
#define PPUTLIMPL_UINT_TRAIT_0xE5Fu HEX, 3679u, -417, (E, 5, F), 0x1A0u
#define PPUTLIMPL_UINT_TRAIT_0xE5Eu HEX, 3678u, -418, (E, 5, E), 0x1A1u
#define PPUTLIMPL_UINT_TRAIT_0xE5Du HEX, 3677u, -419, (E, 5, D), 0x1A2u
#define PPUTLIMPL_UINT_TRAIT_0xE5Cu HEX, 3676u, -420, (E, 5, C), 0x1A3u
#define PPUTLIMPL_UINT_TRAIT_0xE5Bu HEX, 3675u, -421, (E, 5, B), 0x1A4u
#define PPUTLIMPL_UINT_TRAIT_0xE5Au HEX, 3674u, -422, (E, 5, A), 0x1A5u
#define PPUTLIMPL_UINT_TRAIT_0xE59u HEX, 3673u, -423, (E, 5, 9), 0x1A6u
#define PPUTLIMPL_UINT_TRAIT_0xE58u HEX, 3672u, -424, (E, 5, 8), 0x1A7u
#define PPUTLIMPL_UINT_TRAIT_0xE57u HEX, 3671u, -425, (E, 5, 7), 0x1A8u
#define PPUTLIMPL_UINT_TRAIT_0xE56u HEX, 3670u, -426, (E, 5, 6), 0x1A9u
#define PPUTLIMPL_UINT_TRAIT_0xE55u HEX, 3669u, -427, (E, 5, 5), 0x1AAu
#define PPUTLIMPL_UINT_TRAIT_0xE54u HEX, 3668u, -428, (E, 5, 4), 0x1ABu
#define PPUTLIMPL_UINT_TRAIT_0xE53u HEX, 3667u, -429, (E, 5, 3), 0x1ACu
#define PPUTLIMPL_UINT_TRAIT_0xE52u HEX, 3666u, -430, (E, 5, 2), 0x1ADu
#define PPUTLIMPL_UINT_TRAIT_0xE51u HEX, 3665u, -431, (E, 5, 1), 0x1AEu
#define PPUTLIMPL_UINT_TRAIT_0xE50u HEX, 3664u, -432, (E, 5, 0), 0x1AFu
#define PPUTLIMPL_UINT_TRAIT_0xE4Fu HEX, 3663u, -433, (E, 4, F), 0x1B0u
#define PPUTLIMPL_UINT_TRAIT_0xE4Eu HEX, 3662u, -434, (E, 4, E), 0x1B1u
#define PPUTLIMPL_UINT_TRAIT_0xE4Du HEX, 3661u, -435, (E, 4, D), 0x1B2u
#define PPUTLIMPL_UINT_TRAIT_0xE4Cu HEX, 3660u, -436, (E, 4, C), 0x1B3u
#define PPUTLIMPL_UINT_TRAIT_0xE4Bu HEX, 3659u, -437, (E, 4, B), 0x1B4u
#define PPUTLIMPL_UINT_TRAIT_0xE4Au HEX, 3658u, -438, (E, 4, A), 0x1B5u
#define PPUTLIMPL_UINT_TRAIT_0xE49u HEX, 3657u, -439, (E, 4, 9), 0x1B6u
#define PPUTLIMPL_UINT_TRAIT_0xE48u HEX, 3656u, -440, (E, 4, 8), 0x1B7u
#define PPUTLIMPL_UINT_TRAIT_0xE47u HEX, 3655u, -441, (E, 4, 7), 0x1B8u
#define PPUTLIMPL_UINT_TRAIT_0xE46u HEX, 3654u, -442, (E, 4, 6), 0x1B9u
#define PPUTLIMPL_UINT_TRAIT_0xE45u HEX, 3653u, -443, (E, 4, 5), 0x1BAu
#define PPUTLIMPL_UINT_TRAIT_0xE44u HEX, 3652u, -444, (E, 4, 4), 0x1BBu
#define PPUTLIMPL_UINT_TRAIT_0xE43u HEX, 3651u, -445, (E, 4, 3), 0x1BCu
#define PPUTLIMPL_UINT_TRAIT_0xE42u HEX, 3650u, -446, (E, 4, 2), 0x1BDu
#define PPUTLIMPL_UINT_TRAIT_0xE41u HEX, 3649u, -447, (E, 4, 1), 0x1BEu
#define PPUTLIMPL_UINT_TRAIT_0xE40u HEX, 3648u, -448, (E, 4, 0), 0x1BFu
#define PPUTLIMPL_UINT_TRAIT_0xE3Fu HEX, 3647u, -449, (E, 3, F), 0x1C0u
#define PPUTLIMPL_UINT_TRAIT_0xE3Eu HEX, 3646u, -450, (E, 3, E), 0x1C1u
#define PPUTLIMPL_UINT_TRAIT_0xE3Du HEX, 3645u, -451, (E, 3, D), 0x1C2u
#define PPUTLIMPL_UINT_TRAIT_0xE3Cu HEX, 3644u, -452, (E, 3, C), 0x1C3u
#define PPUTLIMPL_UINT_TRAIT_0xE3Bu HEX, 3643u, -453, (E, 3, B), 0x1C4u
#define PPUTLIMPL_UINT_TRAIT_0xE3Au HEX, 3642u, -454, (E, 3, A), 0x1C5u
#define PPUTLIMPL_UINT_TRAIT_0xE39u HEX, 3641u, -455, (E, 3, 9), 0x1C6u
#define PPUTLIMPL_UINT_TRAIT_0xE38u HEX, 3640u, -456, (E, 3, 8), 0x1C7u
#define PPUTLIMPL_UINT_TRAIT_0xE37u HEX, 3639u, -457, (E, 3, 7), 0x1C8u
#define PPUTLIMPL_UINT_TRAIT_0xE36u HEX, 3638u, -458, (E, 3, 6), 0x1C9u
#define PPUTLIMPL_UINT_TRAIT_0xE35u HEX, 3637u, -459, (E, 3, 5), 0x1CAu
#define PPUTLIMPL_UINT_TRAIT_0xE34u HEX, 3636u, -460, (E, 3, 4), 0x1CBu
#define PPUTLIMPL_UINT_TRAIT_0xE33u HEX, 3635u, -461, (E, 3, 3), 0x1CCu
#define PPUTLIMPL_UINT_TRAIT_0xE32u HEX, 3634u, -462, (E, 3, 2), 0x1CDu
#define PPUTLIMPL_UINT_TRAIT_0xE31u HEX, 3633u, -463, (E, 3, 1), 0x1CEu
#define PPUTLIMPL_UINT_TRAIT_0xE30u HEX, 3632u, -464, (E, 3, 0), 0x1CFu
#define PPUTLIMPL_UINT_TRAIT_0xE2Fu HEX, 3631u, -465, (E, 2, F), 0x1D0u
#define PPUTLIMPL_UINT_TRAIT_0xE2Eu HEX, 3630u, -466, (E, 2, E), 0x1D1u
#define PPUTLIMPL_UINT_TRAIT_0xE2Du HEX, 3629u, -467, (E, 2, D), 0x1D2u
#define PPUTLIMPL_UINT_TRAIT_0xE2Cu HEX, 3628u, -468, (E, 2, C), 0x1D3u
#define PPUTLIMPL_UINT_TRAIT_0xE2Bu HEX, 3627u, -469, (E, 2, B), 0x1D4u
#define PPUTLIMPL_UINT_TRAIT_0xE2Au HEX, 3626u, -470, (E, 2, A), 0x1D5u
#define PPUTLIMPL_UINT_TRAIT_0xE29u HEX, 3625u, -471, (E, 2, 9), 0x1D6u
#define PPUTLIMPL_UINT_TRAIT_0xE28u HEX, 3624u, -472, (E, 2, 8), 0x1D7u
#define PPUTLIMPL_UINT_TRAIT_0xE27u HEX, 3623u, -473, (E, 2, 7), 0x1D8u
#define PPUTLIMPL_UINT_TRAIT_0xE26u HEX, 3622u, -474, (E, 2, 6), 0x1D9u
#define PPUTLIMPL_UINT_TRAIT_0xE25u HEX, 3621u, -475, (E, 2, 5), 0x1DAu
#define PPUTLIMPL_UINT_TRAIT_0xE24u HEX, 3620u, -476, (E, 2, 4), 0x1DBu
#define PPUTLIMPL_UINT_TRAIT_0xE23u HEX, 3619u, -477, (E, 2, 3), 0x1DCu
#define PPUTLIMPL_UINT_TRAIT_0xE22u HEX, 3618u, -478, (E, 2, 2), 0x1DDu
#define PPUTLIMPL_UINT_TRAIT_0xE21u HEX, 3617u, -479, (E, 2, 1), 0x1DEu
#define PPUTLIMPL_UINT_TRAIT_0xE20u HEX, 3616u, -480, (E, 2, 0), 0x1DFu
#define PPUTLIMPL_UINT_TRAIT_0xE1Fu HEX, 3615u, -481, (E, 1, F), 0x1E0u
#define PPUTLIMPL_UINT_TRAIT_0xE1Eu HEX, 3614u, -482, (E, 1, E), 0x1E1u
#define PPUTLIMPL_UINT_TRAIT_0xE1Du HEX, 3613u, -483, (E, 1, D), 0x1E2u
#define PPUTLIMPL_UINT_TRAIT_0xE1Cu HEX, 3612u, -484, (E, 1, C), 0x1E3u
#define PPUTLIMPL_UINT_TRAIT_0xE1Bu HEX, 3611u, -485, (E, 1, B), 0x1E4u
#define PPUTLIMPL_UINT_TRAIT_0xE1Au HEX, 3610u, -486, (E, 1, A), 0x1E5u
#define PPUTLIMPL_UINT_TRAIT_0xE19u HEX, 3609u, -487, (E, 1, 9), 0x1E6u
#define PPUTLIMPL_UINT_TRAIT_0xE18u HEX, 3608u, -488, (E, 1, 8), 0x1E7u
#define PPUTLIMPL_UINT_TRAIT_0xE17u HEX, 3607u, -489, (E, 1, 7), 0x1E8u
#define PPUTLIMPL_UINT_TRAIT_0xE16u HEX, 3606u, -490, (E, 1, 6), 0x1E9u
#define PPUTLIMPL_UINT_TRAIT_0xE15u HEX, 3605u, -491, (E, 1, 5), 0x1EAu
#define PPUTLIMPL_UINT_TRAIT_0xE14u HEX, 3604u, -492, (E, 1, 4), 0x1EBu
#define PPUTLIMPL_UINT_TRAIT_0xE13u HEX, 3603u, -493, (E, 1, 3), 0x1ECu
#define PPUTLIMPL_UINT_TRAIT_0xE12u HEX, 3602u, -494, (E, 1, 2), 0x1EDu
#define PPUTLIMPL_UINT_TRAIT_0xE11u HEX, 3601u, -495, (E, 1, 1), 0x1EEu
#define PPUTLIMPL_UINT_TRAIT_0xE10u HEX, 3600u, -496, (E, 1, 0), 0x1EFu
#define PPUTLIMPL_UINT_TRAIT_0xE0Fu HEX, 3599u, -497, (E, 0, F), 0x1F0u
#define PPUTLIMPL_UINT_TRAIT_0xE0Eu HEX, 3598u, -498, (E, 0, E), 0x1F1u
#define PPUTLIMPL_UINT_TRAIT_0xE0Du HEX, 3597u, -499, (E, 0, D), 0x1F2u
#define PPUTLIMPL_UINT_TRAIT_0xE0Cu HEX, 3596u, -500, (E, 0, C), 0x1F3u
#define PPUTLIMPL_UINT_TRAIT_0xE0Bu HEX, 3595u, -501, (E, 0, B), 0x1F4u
#define PPUTLIMPL_UINT_TRAIT_0xE0Au HEX, 3594u, -502, (E, 0, A), 0x1F5u
#define PPUTLIMPL_UINT_TRAIT_0xE09u HEX, 3593u, -503, (E, 0, 9), 0x1F6u
#define PPUTLIMPL_UINT_TRAIT_0xE08u HEX, 3592u, -504, (E, 0, 8), 0x1F7u
#define PPUTLIMPL_UINT_TRAIT_0xE07u HEX, 3591u, -505, (E, 0, 7), 0x1F8u
#define PPUTLIMPL_UINT_TRAIT_0xE06u HEX, 3590u, -506, (E, 0, 6), 0x1F9u
#define PPUTLIMPL_UINT_TRAIT_0xE05u HEX, 3589u, -507, (E, 0, 5), 0x1FAu
#define PPUTLIMPL_UINT_TRAIT_0xE04u HEX, 3588u, -508, (E, 0, 4), 0x1FBu
#define PPUTLIMPL_UINT_TRAIT_0xE03u HEX, 3587u, -509, (E, 0, 3), 0x1FCu
#define PPUTLIMPL_UINT_TRAIT_0xE02u HEX, 3586u, -510, (E, 0, 2), 0x1FDu
#define PPUTLIMPL_UINT_TRAIT_0xE01u HEX, 3585u, -511, (E, 0, 1), 0x1FEu
#define PPUTLIMPL_UINT_TRAIT_0xE00u HEX, 3584u, -512, (E, 0, 0), 0x1FFu
#define PPUTLIMPL_UINT_TRAIT_0xDFFu HEX, 3583u, -513, (D, F, F), 0x200u
#define PPUTLIMPL_UINT_TRAIT_0xDFEu HEX, 3582u, -514, (D, F, E), 0x201u
#define PPUTLIMPL_UINT_TRAIT_0xDFDu HEX, 3581u, -515, (D, F, D), 0x202u
#define PPUTLIMPL_UINT_TRAIT_0xDFCu HEX, 3580u, -516, (D, F, C), 0x203u
#define PPUTLIMPL_UINT_TRAIT_0xDFBu HEX, 3579u, -517, (D, F, B), 0x204u
#define PPUTLIMPL_UINT_TRAIT_0xDFAu HEX, 3578u, -518, (D, F, A), 0x205u
#define PPUTLIMPL_UINT_TRAIT_0xDF9u HEX, 3577u, -519, (D, F, 9), 0x206u
#define PPUTLIMPL_UINT_TRAIT_0xDF8u HEX, 3576u, -520, (D, F, 8), 0x207u
#define PPUTLIMPL_UINT_TRAIT_0xDF7u HEX, 3575u, -521, (D, F, 7), 0x208u
#define PPUTLIMPL_UINT_TRAIT_0xDF6u HEX, 3574u, -522, (D, F, 6), 0x209u
#define PPUTLIMPL_UINT_TRAIT_0xDF5u HEX, 3573u, -523, (D, F, 5), 0x20Au
#define PPUTLIMPL_UINT_TRAIT_0xDF4u HEX, 3572u, -524, (D, F, 4), 0x20Bu
#define PPUTLIMPL_UINT_TRAIT_0xDF3u HEX, 3571u, -525, (D, F, 3), 0x20Cu
#define PPUTLIMPL_UINT_TRAIT_0xDF2u HEX, 3570u, -526, (D, F, 2), 0x20Du
#define PPUTLIMPL_UINT_TRAIT_0xDF1u HEX, 3569u, -527, (D, F, 1), 0x20Eu
#define PPUTLIMPL_UINT_TRAIT_0xDF0u HEX, 3568u, -528, (D, F, 0), 0x20Fu
#define PPUTLIMPL_UINT_TRAIT_0xDEFu HEX, 3567u, -529, (D, E, F), 0x210u
#define PPUTLIMPL_UINT_TRAIT_0xDEEu HEX, 3566u, -530, (D, E, E), 0x211u
#define PPUTLIMPL_UINT_TRAIT_0xDEDu HEX, 3565u, -531, (D, E, D), 0x212u
#define PPUTLIMPL_UINT_TRAIT_0xDECu HEX, 3564u, -532, (D, E, C), 0x213u
#define PPUTLIMPL_UINT_TRAIT_0xDEBu HEX, 3563u, -533, (D, E, B), 0x214u
#define PPUTLIMPL_UINT_TRAIT_0xDEAu HEX, 3562u, -534, (D, E, A), 0x215u
#define PPUTLIMPL_UINT_TRAIT_0xDE9u HEX, 3561u, -535, (D, E, 9), 0x216u
#define PPUTLIMPL_UINT_TRAIT_0xDE8u HEX, 3560u, -536, (D, E, 8), 0x217u
#define PPUTLIMPL_UINT_TRAIT_0xDE7u HEX, 3559u, -537, (D, E, 7), 0x218u
#define PPUTLIMPL_UINT_TRAIT_0xDE6u HEX, 3558u, -538, (D, E, 6), 0x219u
#define PPUTLIMPL_UINT_TRAIT_0xDE5u HEX, 3557u, -539, (D, E, 5), 0x21Au
#define PPUTLIMPL_UINT_TRAIT_0xDE4u HEX, 3556u, -540, (D, E, 4), 0x21Bu
#define PPUTLIMPL_UINT_TRAIT_0xDE3u HEX, 3555u, -541, (D, E, 3), 0x21Cu
#define PPUTLIMPL_UINT_TRAIT_0xDE2u HEX, 3554u, -542, (D, E, 2), 0x21Du
#define PPUTLIMPL_UINT_TRAIT_0xDE1u HEX, 3553u, -543, (D, E, 1), 0x21Eu
#define PPUTLIMPL_UINT_TRAIT_0xDE0u HEX, 3552u, -544, (D, E, 0), 0x21Fu
#define PPUTLIMPL_UINT_TRAIT_0xDDFu HEX, 3551u, -545, (D, D, F), 0x220u
#define PPUTLIMPL_UINT_TRAIT_0xDDEu HEX, 3550u, -546, (D, D, E), 0x221u
#define PPUTLIMPL_UINT_TRAIT_0xDDDu HEX, 3549u, -547, (D, D, D), 0x222u
#define PPUTLIMPL_UINT_TRAIT_0xDDCu HEX, 3548u, -548, (D, D, C), 0x223u
#define PPUTLIMPL_UINT_TRAIT_0xDDBu HEX, 3547u, -549, (D, D, B), 0x224u
#define PPUTLIMPL_UINT_TRAIT_0xDDAu HEX, 3546u, -550, (D, D, A), 0x225u
#define PPUTLIMPL_UINT_TRAIT_0xDD9u HEX, 3545u, -551, (D, D, 9), 0x226u
#define PPUTLIMPL_UINT_TRAIT_0xDD8u HEX, 3544u, -552, (D, D, 8), 0x227u
#define PPUTLIMPL_UINT_TRAIT_0xDD7u HEX, 3543u, -553, (D, D, 7), 0x228u
#define PPUTLIMPL_UINT_TRAIT_0xDD6u HEX, 3542u, -554, (D, D, 6), 0x229u
#define PPUTLIMPL_UINT_TRAIT_0xDD5u HEX, 3541u, -555, (D, D, 5), 0x22Au
#define PPUTLIMPL_UINT_TRAIT_0xDD4u HEX, 3540u, -556, (D, D, 4), 0x22Bu
#define PPUTLIMPL_UINT_TRAIT_0xDD3u HEX, 3539u, -557, (D, D, 3), 0x22Cu
#define PPUTLIMPL_UINT_TRAIT_0xDD2u HEX, 3538u, -558, (D, D, 2), 0x22Du
#define PPUTLIMPL_UINT_TRAIT_0xDD1u HEX, 3537u, -559, (D, D, 1), 0x22Eu
#define PPUTLIMPL_UINT_TRAIT_0xDD0u HEX, 3536u, -560, (D, D, 0), 0x22Fu
#define PPUTLIMPL_UINT_TRAIT_0xDCFu HEX, 3535u, -561, (D, C, F), 0x230u
#define PPUTLIMPL_UINT_TRAIT_0xDCEu HEX, 3534u, -562, (D, C, E), 0x231u
#define PPUTLIMPL_UINT_TRAIT_0xDCDu HEX, 3533u, -563, (D, C, D), 0x232u
#define PPUTLIMPL_UINT_TRAIT_0xDCCu HEX, 3532u, -564, (D, C, C), 0x233u
#define PPUTLIMPL_UINT_TRAIT_0xDCBu HEX, 3531u, -565, (D, C, B), 0x234u
#define PPUTLIMPL_UINT_TRAIT_0xDCAu HEX, 3530u, -566, (D, C, A), 0x235u
#define PPUTLIMPL_UINT_TRAIT_0xDC9u HEX, 3529u, -567, (D, C, 9), 0x236u
#define PPUTLIMPL_UINT_TRAIT_0xDC8u HEX, 3528u, -568, (D, C, 8), 0x237u
#define PPUTLIMPL_UINT_TRAIT_0xDC7u HEX, 3527u, -569, (D, C, 7), 0x238u
#define PPUTLIMPL_UINT_TRAIT_0xDC6u HEX, 3526u, -570, (D, C, 6), 0x239u
#define PPUTLIMPL_UINT_TRAIT_0xDC5u HEX, 3525u, -571, (D, C, 5), 0x23Au
#define PPUTLIMPL_UINT_TRAIT_0xDC4u HEX, 3524u, -572, (D, C, 4), 0x23Bu
#define PPUTLIMPL_UINT_TRAIT_0xDC3u HEX, 3523u, -573, (D, C, 3), 0x23Cu
#define PPUTLIMPL_UINT_TRAIT_0xDC2u HEX, 3522u, -574, (D, C, 2), 0x23Du
#define PPUTLIMPL_UINT_TRAIT_0xDC1u HEX, 3521u, -575, (D, C, 1), 0x23Eu
#define PPUTLIMPL_UINT_TRAIT_0xDC0u HEX, 3520u, -576, (D, C, 0), 0x23Fu
#define PPUTLIMPL_UINT_TRAIT_0xDBFu HEX, 3519u, -577, (D, B, F), 0x240u
#define PPUTLIMPL_UINT_TRAIT_0xDBEu HEX, 3518u, -578, (D, B, E), 0x241u
#define PPUTLIMPL_UINT_TRAIT_0xDBDu HEX, 3517u, -579, (D, B, D), 0x242u
#define PPUTLIMPL_UINT_TRAIT_0xDBCu HEX, 3516u, -580, (D, B, C), 0x243u
#define PPUTLIMPL_UINT_TRAIT_0xDBBu HEX, 3515u, -581, (D, B, B), 0x244u
#define PPUTLIMPL_UINT_TRAIT_0xDBAu HEX, 3514u, -582, (D, B, A), 0x245u
#define PPUTLIMPL_UINT_TRAIT_0xDB9u HEX, 3513u, -583, (D, B, 9), 0x246u
#define PPUTLIMPL_UINT_TRAIT_0xDB8u HEX, 3512u, -584, (D, B, 8), 0x247u
#define PPUTLIMPL_UINT_TRAIT_0xDB7u HEX, 3511u, -585, (D, B, 7), 0x248u
#define PPUTLIMPL_UINT_TRAIT_0xDB6u HEX, 3510u, -586, (D, B, 6), 0x249u
#define PPUTLIMPL_UINT_TRAIT_0xDB5u HEX, 3509u, -587, (D, B, 5), 0x24Au
#define PPUTLIMPL_UINT_TRAIT_0xDB4u HEX, 3508u, -588, (D, B, 4), 0x24Bu
#define PPUTLIMPL_UINT_TRAIT_0xDB3u HEX, 3507u, -589, (D, B, 3), 0x24Cu
#define PPUTLIMPL_UINT_TRAIT_0xDB2u HEX, 3506u, -590, (D, B, 2), 0x24Du
#define PPUTLIMPL_UINT_TRAIT_0xDB1u HEX, 3505u, -591, (D, B, 1), 0x24Eu
#define PPUTLIMPL_UINT_TRAIT_0xDB0u HEX, 3504u, -592, (D, B, 0), 0x24Fu
#define PPUTLIMPL_UINT_TRAIT_0xDAFu HEX, 3503u, -593, (D, A, F), 0x250u
#define PPUTLIMPL_UINT_TRAIT_0xDAEu HEX, 3502u, -594, (D, A, E), 0x251u
#define PPUTLIMPL_UINT_TRAIT_0xDADu HEX, 3501u, -595, (D, A, D), 0x252u
#define PPUTLIMPL_UINT_TRAIT_0xDACu HEX, 3500u, -596, (D, A, C), 0x253u
#define PPUTLIMPL_UINT_TRAIT_0xDABu HEX, 3499u, -597, (D, A, B), 0x254u
#define PPUTLIMPL_UINT_TRAIT_0xDAAu HEX, 3498u, -598, (D, A, A), 0x255u
#define PPUTLIMPL_UINT_TRAIT_0xDA9u HEX, 3497u, -599, (D, A, 9), 0x256u
#define PPUTLIMPL_UINT_TRAIT_0xDA8u HEX, 3496u, -600, (D, A, 8), 0x257u
#define PPUTLIMPL_UINT_TRAIT_0xDA7u HEX, 3495u, -601, (D, A, 7), 0x258u
#define PPUTLIMPL_UINT_TRAIT_0xDA6u HEX, 3494u, -602, (D, A, 6), 0x259u
#define PPUTLIMPL_UINT_TRAIT_0xDA5u HEX, 3493u, -603, (D, A, 5), 0x25Au
#define PPUTLIMPL_UINT_TRAIT_0xDA4u HEX, 3492u, -604, (D, A, 4), 0x25Bu
#define PPUTLIMPL_UINT_TRAIT_0xDA3u HEX, 3491u, -605, (D, A, 3), 0x25Cu
#define PPUTLIMPL_UINT_TRAIT_0xDA2u HEX, 3490u, -606, (D, A, 2), 0x25Du
#define PPUTLIMPL_UINT_TRAIT_0xDA1u HEX, 3489u, -607, (D, A, 1), 0x25Eu
#define PPUTLIMPL_UINT_TRAIT_0xDA0u HEX, 3488u, -608, (D, A, 0), 0x25Fu
#define PPUTLIMPL_UINT_TRAIT_0xD9Fu HEX, 3487u, -609, (D, 9, F), 0x260u
#define PPUTLIMPL_UINT_TRAIT_0xD9Eu HEX, 3486u, -610, (D, 9, E), 0x261u
#define PPUTLIMPL_UINT_TRAIT_0xD9Du HEX, 3485u, -611, (D, 9, D), 0x262u
#define PPUTLIMPL_UINT_TRAIT_0xD9Cu HEX, 3484u, -612, (D, 9, C), 0x263u
#define PPUTLIMPL_UINT_TRAIT_0xD9Bu HEX, 3483u, -613, (D, 9, B), 0x264u
#define PPUTLIMPL_UINT_TRAIT_0xD9Au HEX, 3482u, -614, (D, 9, A), 0x265u
#define PPUTLIMPL_UINT_TRAIT_0xD99u HEX, 3481u, -615, (D, 9, 9), 0x266u
#define PPUTLIMPL_UINT_TRAIT_0xD98u HEX, 3480u, -616, (D, 9, 8), 0x267u
#define PPUTLIMPL_UINT_TRAIT_0xD97u HEX, 3479u, -617, (D, 9, 7), 0x268u
#define PPUTLIMPL_UINT_TRAIT_0xD96u HEX, 3478u, -618, (D, 9, 6), 0x269u
#define PPUTLIMPL_UINT_TRAIT_0xD95u HEX, 3477u, -619, (D, 9, 5), 0x26Au
#define PPUTLIMPL_UINT_TRAIT_0xD94u HEX, 3476u, -620, (D, 9, 4), 0x26Bu
#define PPUTLIMPL_UINT_TRAIT_0xD93u HEX, 3475u, -621, (D, 9, 3), 0x26Cu
#define PPUTLIMPL_UINT_TRAIT_0xD92u HEX, 3474u, -622, (D, 9, 2), 0x26Du
#define PPUTLIMPL_UINT_TRAIT_0xD91u HEX, 3473u, -623, (D, 9, 1), 0x26Eu
#define PPUTLIMPL_UINT_TRAIT_0xD90u HEX, 3472u, -624, (D, 9, 0), 0x26Fu
#define PPUTLIMPL_UINT_TRAIT_0xD8Fu HEX, 3471u, -625, (D, 8, F), 0x270u
#define PPUTLIMPL_UINT_TRAIT_0xD8Eu HEX, 3470u, -626, (D, 8, E), 0x271u
#define PPUTLIMPL_UINT_TRAIT_0xD8Du HEX, 3469u, -627, (D, 8, D), 0x272u
#define PPUTLIMPL_UINT_TRAIT_0xD8Cu HEX, 3468u, -628, (D, 8, C), 0x273u
#define PPUTLIMPL_UINT_TRAIT_0xD8Bu HEX, 3467u, -629, (D, 8, B), 0x274u
#define PPUTLIMPL_UINT_TRAIT_0xD8Au HEX, 3466u, -630, (D, 8, A), 0x275u
#define PPUTLIMPL_UINT_TRAIT_0xD89u HEX, 3465u, -631, (D, 8, 9), 0x276u
#define PPUTLIMPL_UINT_TRAIT_0xD88u HEX, 3464u, -632, (D, 8, 8), 0x277u
#define PPUTLIMPL_UINT_TRAIT_0xD87u HEX, 3463u, -633, (D, 8, 7), 0x278u
#define PPUTLIMPL_UINT_TRAIT_0xD86u HEX, 3462u, -634, (D, 8, 6), 0x279u
#define PPUTLIMPL_UINT_TRAIT_0xD85u HEX, 3461u, -635, (D, 8, 5), 0x27Au
#define PPUTLIMPL_UINT_TRAIT_0xD84u HEX, 3460u, -636, (D, 8, 4), 0x27Bu
#define PPUTLIMPL_UINT_TRAIT_0xD83u HEX, 3459u, -637, (D, 8, 3), 0x27Cu
#define PPUTLIMPL_UINT_TRAIT_0xD82u HEX, 3458u, -638, (D, 8, 2), 0x27Du
#define PPUTLIMPL_UINT_TRAIT_0xD81u HEX, 3457u, -639, (D, 8, 1), 0x27Eu
#define PPUTLIMPL_UINT_TRAIT_0xD80u HEX, 3456u, -640, (D, 8, 0), 0x27Fu
#define PPUTLIMPL_UINT_TRAIT_0xD7Fu HEX, 3455u, -641, (D, 7, F), 0x280u
#define PPUTLIMPL_UINT_TRAIT_0xD7Eu HEX, 3454u, -642, (D, 7, E), 0x281u
#define PPUTLIMPL_UINT_TRAIT_0xD7Du HEX, 3453u, -643, (D, 7, D), 0x282u
#define PPUTLIMPL_UINT_TRAIT_0xD7Cu HEX, 3452u, -644, (D, 7, C), 0x283u
#define PPUTLIMPL_UINT_TRAIT_0xD7Bu HEX, 3451u, -645, (D, 7, B), 0x284u
#define PPUTLIMPL_UINT_TRAIT_0xD7Au HEX, 3450u, -646, (D, 7, A), 0x285u
#define PPUTLIMPL_UINT_TRAIT_0xD79u HEX, 3449u, -647, (D, 7, 9), 0x286u
#define PPUTLIMPL_UINT_TRAIT_0xD78u HEX, 3448u, -648, (D, 7, 8), 0x287u
#define PPUTLIMPL_UINT_TRAIT_0xD77u HEX, 3447u, -649, (D, 7, 7), 0x288u
#define PPUTLIMPL_UINT_TRAIT_0xD76u HEX, 3446u, -650, (D, 7, 6), 0x289u
#define PPUTLIMPL_UINT_TRAIT_0xD75u HEX, 3445u, -651, (D, 7, 5), 0x28Au
#define PPUTLIMPL_UINT_TRAIT_0xD74u HEX, 3444u, -652, (D, 7, 4), 0x28Bu
#define PPUTLIMPL_UINT_TRAIT_0xD73u HEX, 3443u, -653, (D, 7, 3), 0x28Cu
#define PPUTLIMPL_UINT_TRAIT_0xD72u HEX, 3442u, -654, (D, 7, 2), 0x28Du
#define PPUTLIMPL_UINT_TRAIT_0xD71u HEX, 3441u, -655, (D, 7, 1), 0x28Eu
#define PPUTLIMPL_UINT_TRAIT_0xD70u HEX, 3440u, -656, (D, 7, 0), 0x28Fu
#define PPUTLIMPL_UINT_TRAIT_0xD6Fu HEX, 3439u, -657, (D, 6, F), 0x290u
#define PPUTLIMPL_UINT_TRAIT_0xD6Eu HEX, 3438u, -658, (D, 6, E), 0x291u
#define PPUTLIMPL_UINT_TRAIT_0xD6Du HEX, 3437u, -659, (D, 6, D), 0x292u
#define PPUTLIMPL_UINT_TRAIT_0xD6Cu HEX, 3436u, -660, (D, 6, C), 0x293u
#define PPUTLIMPL_UINT_TRAIT_0xD6Bu HEX, 3435u, -661, (D, 6, B), 0x294u
#define PPUTLIMPL_UINT_TRAIT_0xD6Au HEX, 3434u, -662, (D, 6, A), 0x295u
#define PPUTLIMPL_UINT_TRAIT_0xD69u HEX, 3433u, -663, (D, 6, 9), 0x296u
#define PPUTLIMPL_UINT_TRAIT_0xD68u HEX, 3432u, -664, (D, 6, 8), 0x297u
#define PPUTLIMPL_UINT_TRAIT_0xD67u HEX, 3431u, -665, (D, 6, 7), 0x298u
#define PPUTLIMPL_UINT_TRAIT_0xD66u HEX, 3430u, -666, (D, 6, 6), 0x299u
#define PPUTLIMPL_UINT_TRAIT_0xD65u HEX, 3429u, -667, (D, 6, 5), 0x29Au
#define PPUTLIMPL_UINT_TRAIT_0xD64u HEX, 3428u, -668, (D, 6, 4), 0x29Bu
#define PPUTLIMPL_UINT_TRAIT_0xD63u HEX, 3427u, -669, (D, 6, 3), 0x29Cu
#define PPUTLIMPL_UINT_TRAIT_0xD62u HEX, 3426u, -670, (D, 6, 2), 0x29Du
#define PPUTLIMPL_UINT_TRAIT_0xD61u HEX, 3425u, -671, (D, 6, 1), 0x29Eu
#define PPUTLIMPL_UINT_TRAIT_0xD60u HEX, 3424u, -672, (D, 6, 0), 0x29Fu
#define PPUTLIMPL_UINT_TRAIT_0xD5Fu HEX, 3423u, -673, (D, 5, F), 0x2A0u
#define PPUTLIMPL_UINT_TRAIT_0xD5Eu HEX, 3422u, -674, (D, 5, E), 0x2A1u
#define PPUTLIMPL_UINT_TRAIT_0xD5Du HEX, 3421u, -675, (D, 5, D), 0x2A2u
#define PPUTLIMPL_UINT_TRAIT_0xD5Cu HEX, 3420u, -676, (D, 5, C), 0x2A3u
#define PPUTLIMPL_UINT_TRAIT_0xD5Bu HEX, 3419u, -677, (D, 5, B), 0x2A4u
#define PPUTLIMPL_UINT_TRAIT_0xD5Au HEX, 3418u, -678, (D, 5, A), 0x2A5u
#define PPUTLIMPL_UINT_TRAIT_0xD59u HEX, 3417u, -679, (D, 5, 9), 0x2A6u
#define PPUTLIMPL_UINT_TRAIT_0xD58u HEX, 3416u, -680, (D, 5, 8), 0x2A7u
#define PPUTLIMPL_UINT_TRAIT_0xD57u HEX, 3415u, -681, (D, 5, 7), 0x2A8u
#define PPUTLIMPL_UINT_TRAIT_0xD56u HEX, 3414u, -682, (D, 5, 6), 0x2A9u
#define PPUTLIMPL_UINT_TRAIT_0xD55u HEX, 3413u, -683, (D, 5, 5), 0x2AAu
#define PPUTLIMPL_UINT_TRAIT_0xD54u HEX, 3412u, -684, (D, 5, 4), 0x2ABu
#define PPUTLIMPL_UINT_TRAIT_0xD53u HEX, 3411u, -685, (D, 5, 3), 0x2ACu
#define PPUTLIMPL_UINT_TRAIT_0xD52u HEX, 3410u, -686, (D, 5, 2), 0x2ADu
#define PPUTLIMPL_UINT_TRAIT_0xD51u HEX, 3409u, -687, (D, 5, 1), 0x2AEu
#define PPUTLIMPL_UINT_TRAIT_0xD50u HEX, 3408u, -688, (D, 5, 0), 0x2AFu
#define PPUTLIMPL_UINT_TRAIT_0xD4Fu HEX, 3407u, -689, (D, 4, F), 0x2B0u
#define PPUTLIMPL_UINT_TRAIT_0xD4Eu HEX, 3406u, -690, (D, 4, E), 0x2B1u
#define PPUTLIMPL_UINT_TRAIT_0xD4Du HEX, 3405u, -691, (D, 4, D), 0x2B2u
#define PPUTLIMPL_UINT_TRAIT_0xD4Cu HEX, 3404u, -692, (D, 4, C), 0x2B3u
#define PPUTLIMPL_UINT_TRAIT_0xD4Bu HEX, 3403u, -693, (D, 4, B), 0x2B4u
#define PPUTLIMPL_UINT_TRAIT_0xD4Au HEX, 3402u, -694, (D, 4, A), 0x2B5u
#define PPUTLIMPL_UINT_TRAIT_0xD49u HEX, 3401u, -695, (D, 4, 9), 0x2B6u
#define PPUTLIMPL_UINT_TRAIT_0xD48u HEX, 3400u, -696, (D, 4, 8), 0x2B7u
#define PPUTLIMPL_UINT_TRAIT_0xD47u HEX, 3399u, -697, (D, 4, 7), 0x2B8u
#define PPUTLIMPL_UINT_TRAIT_0xD46u HEX, 3398u, -698, (D, 4, 6), 0x2B9u
#define PPUTLIMPL_UINT_TRAIT_0xD45u HEX, 3397u, -699, (D, 4, 5), 0x2BAu
#define PPUTLIMPL_UINT_TRAIT_0xD44u HEX, 3396u, -700, (D, 4, 4), 0x2BBu
#define PPUTLIMPL_UINT_TRAIT_0xD43u HEX, 3395u, -701, (D, 4, 3), 0x2BCu
#define PPUTLIMPL_UINT_TRAIT_0xD42u HEX, 3394u, -702, (D, 4, 2), 0x2BDu
#define PPUTLIMPL_UINT_TRAIT_0xD41u HEX, 3393u, -703, (D, 4, 1), 0x2BEu
#define PPUTLIMPL_UINT_TRAIT_0xD40u HEX, 3392u, -704, (D, 4, 0), 0x2BFu
#define PPUTLIMPL_UINT_TRAIT_0xD3Fu HEX, 3391u, -705, (D, 3, F), 0x2C0u
#define PPUTLIMPL_UINT_TRAIT_0xD3Eu HEX, 3390u, -706, (D, 3, E), 0x2C1u
#define PPUTLIMPL_UINT_TRAIT_0xD3Du HEX, 3389u, -707, (D, 3, D), 0x2C2u
#define PPUTLIMPL_UINT_TRAIT_0xD3Cu HEX, 3388u, -708, (D, 3, C), 0x2C3u
#define PPUTLIMPL_UINT_TRAIT_0xD3Bu HEX, 3387u, -709, (D, 3, B), 0x2C4u
#define PPUTLIMPL_UINT_TRAIT_0xD3Au HEX, 3386u, -710, (D, 3, A), 0x2C5u
#define PPUTLIMPL_UINT_TRAIT_0xD39u HEX, 3385u, -711, (D, 3, 9), 0x2C6u
#define PPUTLIMPL_UINT_TRAIT_0xD38u HEX, 3384u, -712, (D, 3, 8), 0x2C7u
#define PPUTLIMPL_UINT_TRAIT_0xD37u HEX, 3383u, -713, (D, 3, 7), 0x2C8u
#define PPUTLIMPL_UINT_TRAIT_0xD36u HEX, 3382u, -714, (D, 3, 6), 0x2C9u
#define PPUTLIMPL_UINT_TRAIT_0xD35u HEX, 3381u, -715, (D, 3, 5), 0x2CAu
#define PPUTLIMPL_UINT_TRAIT_0xD34u HEX, 3380u, -716, (D, 3, 4), 0x2CBu
#define PPUTLIMPL_UINT_TRAIT_0xD33u HEX, 3379u, -717, (D, 3, 3), 0x2CCu
#define PPUTLIMPL_UINT_TRAIT_0xD32u HEX, 3378u, -718, (D, 3, 2), 0x2CDu
#define PPUTLIMPL_UINT_TRAIT_0xD31u HEX, 3377u, -719, (D, 3, 1), 0x2CEu
#define PPUTLIMPL_UINT_TRAIT_0xD30u HEX, 3376u, -720, (D, 3, 0), 0x2CFu
#define PPUTLIMPL_UINT_TRAIT_0xD2Fu HEX, 3375u, -721, (D, 2, F), 0x2D0u
#define PPUTLIMPL_UINT_TRAIT_0xD2Eu HEX, 3374u, -722, (D, 2, E), 0x2D1u
#define PPUTLIMPL_UINT_TRAIT_0xD2Du HEX, 3373u, -723, (D, 2, D), 0x2D2u
#define PPUTLIMPL_UINT_TRAIT_0xD2Cu HEX, 3372u, -724, (D, 2, C), 0x2D3u
#define PPUTLIMPL_UINT_TRAIT_0xD2Bu HEX, 3371u, -725, (D, 2, B), 0x2D4u
#define PPUTLIMPL_UINT_TRAIT_0xD2Au HEX, 3370u, -726, (D, 2, A), 0x2D5u
#define PPUTLIMPL_UINT_TRAIT_0xD29u HEX, 3369u, -727, (D, 2, 9), 0x2D6u
#define PPUTLIMPL_UINT_TRAIT_0xD28u HEX, 3368u, -728, (D, 2, 8), 0x2D7u
#define PPUTLIMPL_UINT_TRAIT_0xD27u HEX, 3367u, -729, (D, 2, 7), 0x2D8u
#define PPUTLIMPL_UINT_TRAIT_0xD26u HEX, 3366u, -730, (D, 2, 6), 0x2D9u
#define PPUTLIMPL_UINT_TRAIT_0xD25u HEX, 3365u, -731, (D, 2, 5), 0x2DAu
#define PPUTLIMPL_UINT_TRAIT_0xD24u HEX, 3364u, -732, (D, 2, 4), 0x2DBu
#define PPUTLIMPL_UINT_TRAIT_0xD23u HEX, 3363u, -733, (D, 2, 3), 0x2DCu
#define PPUTLIMPL_UINT_TRAIT_0xD22u HEX, 3362u, -734, (D, 2, 2), 0x2DDu
#define PPUTLIMPL_UINT_TRAIT_0xD21u HEX, 3361u, -735, (D, 2, 1), 0x2DEu
#define PPUTLIMPL_UINT_TRAIT_0xD20u HEX, 3360u, -736, (D, 2, 0), 0x2DFu
#define PPUTLIMPL_UINT_TRAIT_0xD1Fu HEX, 3359u, -737, (D, 1, F), 0x2E0u
#define PPUTLIMPL_UINT_TRAIT_0xD1Eu HEX, 3358u, -738, (D, 1, E), 0x2E1u
#define PPUTLIMPL_UINT_TRAIT_0xD1Du HEX, 3357u, -739, (D, 1, D), 0x2E2u
#define PPUTLIMPL_UINT_TRAIT_0xD1Cu HEX, 3356u, -740, (D, 1, C), 0x2E3u
#define PPUTLIMPL_UINT_TRAIT_0xD1Bu HEX, 3355u, -741, (D, 1, B), 0x2E4u
#define PPUTLIMPL_UINT_TRAIT_0xD1Au HEX, 3354u, -742, (D, 1, A), 0x2E5u
#define PPUTLIMPL_UINT_TRAIT_0xD19u HEX, 3353u, -743, (D, 1, 9), 0x2E6u
#define PPUTLIMPL_UINT_TRAIT_0xD18u HEX, 3352u, -744, (D, 1, 8), 0x2E7u
#define PPUTLIMPL_UINT_TRAIT_0xD17u HEX, 3351u, -745, (D, 1, 7), 0x2E8u
#define PPUTLIMPL_UINT_TRAIT_0xD16u HEX, 3350u, -746, (D, 1, 6), 0x2E9u
#define PPUTLIMPL_UINT_TRAIT_0xD15u HEX, 3349u, -747, (D, 1, 5), 0x2EAu
#define PPUTLIMPL_UINT_TRAIT_0xD14u HEX, 3348u, -748, (D, 1, 4), 0x2EBu
#define PPUTLIMPL_UINT_TRAIT_0xD13u HEX, 3347u, -749, (D, 1, 3), 0x2ECu
#define PPUTLIMPL_UINT_TRAIT_0xD12u HEX, 3346u, -750, (D, 1, 2), 0x2EDu
#define PPUTLIMPL_UINT_TRAIT_0xD11u HEX, 3345u, -751, (D, 1, 1), 0x2EEu
#define PPUTLIMPL_UINT_TRAIT_0xD10u HEX, 3344u, -752, (D, 1, 0), 0x2EFu
#define PPUTLIMPL_UINT_TRAIT_0xD0Fu HEX, 3343u, -753, (D, 0, F), 0x2F0u
#define PPUTLIMPL_UINT_TRAIT_0xD0Eu HEX, 3342u, -754, (D, 0, E), 0x2F1u
#define PPUTLIMPL_UINT_TRAIT_0xD0Du HEX, 3341u, -755, (D, 0, D), 0x2F2u
#define PPUTLIMPL_UINT_TRAIT_0xD0Cu HEX, 3340u, -756, (D, 0, C), 0x2F3u
#define PPUTLIMPL_UINT_TRAIT_0xD0Bu HEX, 3339u, -757, (D, 0, B), 0x2F4u
#define PPUTLIMPL_UINT_TRAIT_0xD0Au HEX, 3338u, -758, (D, 0, A), 0x2F5u
#define PPUTLIMPL_UINT_TRAIT_0xD09u HEX, 3337u, -759, (D, 0, 9), 0x2F6u
#define PPUTLIMPL_UINT_TRAIT_0xD08u HEX, 3336u, -760, (D, 0, 8), 0x2F7u
#define PPUTLIMPL_UINT_TRAIT_0xD07u HEX, 3335u, -761, (D, 0, 7), 0x2F8u
#define PPUTLIMPL_UINT_TRAIT_0xD06u HEX, 3334u, -762, (D, 0, 6), 0x2F9u
#define PPUTLIMPL_UINT_TRAIT_0xD05u HEX, 3333u, -763, (D, 0, 5), 0x2FAu
#define PPUTLIMPL_UINT_TRAIT_0xD04u HEX, 3332u, -764, (D, 0, 4), 0x2FBu
#define PPUTLIMPL_UINT_TRAIT_0xD03u HEX, 3331u, -765, (D, 0, 3), 0x2FCu
#define PPUTLIMPL_UINT_TRAIT_0xD02u HEX, 3330u, -766, (D, 0, 2), 0x2FDu
#define PPUTLIMPL_UINT_TRAIT_0xD01u HEX, 3329u, -767, (D, 0, 1), 0x2FEu
#define PPUTLIMPL_UINT_TRAIT_0xD00u HEX, 3328u, -768, (D, 0, 0), 0x2FFu
#define PPUTLIMPL_UINT_TRAIT_0xCFFu HEX, 3327u, -769, (C, F, F), 0x300u
#define PPUTLIMPL_UINT_TRAIT_0xCFEu HEX, 3326u, -770, (C, F, E), 0x301u
#define PPUTLIMPL_UINT_TRAIT_0xCFDu HEX, 3325u, -771, (C, F, D), 0x302u
#define PPUTLIMPL_UINT_TRAIT_0xCFCu HEX, 3324u, -772, (C, F, C), 0x303u
#define PPUTLIMPL_UINT_TRAIT_0xCFBu HEX, 3323u, -773, (C, F, B), 0x304u
#define PPUTLIMPL_UINT_TRAIT_0xCFAu HEX, 3322u, -774, (C, F, A), 0x305u
#define PPUTLIMPL_UINT_TRAIT_0xCF9u HEX, 3321u, -775, (C, F, 9), 0x306u
#define PPUTLIMPL_UINT_TRAIT_0xCF8u HEX, 3320u, -776, (C, F, 8), 0x307u
#define PPUTLIMPL_UINT_TRAIT_0xCF7u HEX, 3319u, -777, (C, F, 7), 0x308u
#define PPUTLIMPL_UINT_TRAIT_0xCF6u HEX, 3318u, -778, (C, F, 6), 0x309u
#define PPUTLIMPL_UINT_TRAIT_0xCF5u HEX, 3317u, -779, (C, F, 5), 0x30Au
#define PPUTLIMPL_UINT_TRAIT_0xCF4u HEX, 3316u, -780, (C, F, 4), 0x30Bu
#define PPUTLIMPL_UINT_TRAIT_0xCF3u HEX, 3315u, -781, (C, F, 3), 0x30Cu
#define PPUTLIMPL_UINT_TRAIT_0xCF2u HEX, 3314u, -782, (C, F, 2), 0x30Du
#define PPUTLIMPL_UINT_TRAIT_0xCF1u HEX, 3313u, -783, (C, F, 1), 0x30Eu
#define PPUTLIMPL_UINT_TRAIT_0xCF0u HEX, 3312u, -784, (C, F, 0), 0x30Fu
#define PPUTLIMPL_UINT_TRAIT_0xCEFu HEX, 3311u, -785, (C, E, F), 0x310u
#define PPUTLIMPL_UINT_TRAIT_0xCEEu HEX, 3310u, -786, (C, E, E), 0x311u
#define PPUTLIMPL_UINT_TRAIT_0xCEDu HEX, 3309u, -787, (C, E, D), 0x312u
#define PPUTLIMPL_UINT_TRAIT_0xCECu HEX, 3308u, -788, (C, E, C), 0x313u
#define PPUTLIMPL_UINT_TRAIT_0xCEBu HEX, 3307u, -789, (C, E, B), 0x314u
#define PPUTLIMPL_UINT_TRAIT_0xCEAu HEX, 3306u, -790, (C, E, A), 0x315u
#define PPUTLIMPL_UINT_TRAIT_0xCE9u HEX, 3305u, -791, (C, E, 9), 0x316u
#define PPUTLIMPL_UINT_TRAIT_0xCE8u HEX, 3304u, -792, (C, E, 8), 0x317u
#define PPUTLIMPL_UINT_TRAIT_0xCE7u HEX, 3303u, -793, (C, E, 7), 0x318u
#define PPUTLIMPL_UINT_TRAIT_0xCE6u HEX, 3302u, -794, (C, E, 6), 0x319u
#define PPUTLIMPL_UINT_TRAIT_0xCE5u HEX, 3301u, -795, (C, E, 5), 0x31Au
#define PPUTLIMPL_UINT_TRAIT_0xCE4u HEX, 3300u, -796, (C, E, 4), 0x31Bu
#define PPUTLIMPL_UINT_TRAIT_0xCE3u HEX, 3299u, -797, (C, E, 3), 0x31Cu
#define PPUTLIMPL_UINT_TRAIT_0xCE2u HEX, 3298u, -798, (C, E, 2), 0x31Du
#define PPUTLIMPL_UINT_TRAIT_0xCE1u HEX, 3297u, -799, (C, E, 1), 0x31Eu
#define PPUTLIMPL_UINT_TRAIT_0xCE0u HEX, 3296u, -800, (C, E, 0), 0x31Fu
#define PPUTLIMPL_UINT_TRAIT_0xCDFu HEX, 3295u, -801, (C, D, F), 0x320u
#define PPUTLIMPL_UINT_TRAIT_0xCDEu HEX, 3294u, -802, (C, D, E), 0x321u
#define PPUTLIMPL_UINT_TRAIT_0xCDDu HEX, 3293u, -803, (C, D, D), 0x322u
#define PPUTLIMPL_UINT_TRAIT_0xCDCu HEX, 3292u, -804, (C, D, C), 0x323u
#define PPUTLIMPL_UINT_TRAIT_0xCDBu HEX, 3291u, -805, (C, D, B), 0x324u
#define PPUTLIMPL_UINT_TRAIT_0xCDAu HEX, 3290u, -806, (C, D, A), 0x325u
#define PPUTLIMPL_UINT_TRAIT_0xCD9u HEX, 3289u, -807, (C, D, 9), 0x326u
#define PPUTLIMPL_UINT_TRAIT_0xCD8u HEX, 3288u, -808, (C, D, 8), 0x327u
#define PPUTLIMPL_UINT_TRAIT_0xCD7u HEX, 3287u, -809, (C, D, 7), 0x328u
#define PPUTLIMPL_UINT_TRAIT_0xCD6u HEX, 3286u, -810, (C, D, 6), 0x329u
#define PPUTLIMPL_UINT_TRAIT_0xCD5u HEX, 3285u, -811, (C, D, 5), 0x32Au
#define PPUTLIMPL_UINT_TRAIT_0xCD4u HEX, 3284u, -812, (C, D, 4), 0x32Bu
#define PPUTLIMPL_UINT_TRAIT_0xCD3u HEX, 3283u, -813, (C, D, 3), 0x32Cu
#define PPUTLIMPL_UINT_TRAIT_0xCD2u HEX, 3282u, -814, (C, D, 2), 0x32Du
#define PPUTLIMPL_UINT_TRAIT_0xCD1u HEX, 3281u, -815, (C, D, 1), 0x32Eu
#define PPUTLIMPL_UINT_TRAIT_0xCD0u HEX, 3280u, -816, (C, D, 0), 0x32Fu
#define PPUTLIMPL_UINT_TRAIT_0xCCFu HEX, 3279u, -817, (C, C, F), 0x330u
#define PPUTLIMPL_UINT_TRAIT_0xCCEu HEX, 3278u, -818, (C, C, E), 0x331u
#define PPUTLIMPL_UINT_TRAIT_0xCCDu HEX, 3277u, -819, (C, C, D), 0x332u
#define PPUTLIMPL_UINT_TRAIT_0xCCCu HEX, 3276u, -820, (C, C, C), 0x333u
#define PPUTLIMPL_UINT_TRAIT_0xCCBu HEX, 3275u, -821, (C, C, B), 0x334u
#define PPUTLIMPL_UINT_TRAIT_0xCCAu HEX, 3274u, -822, (C, C, A), 0x335u
#define PPUTLIMPL_UINT_TRAIT_0xCC9u HEX, 3273u, -823, (C, C, 9), 0x336u
#define PPUTLIMPL_UINT_TRAIT_0xCC8u HEX, 3272u, -824, (C, C, 8), 0x337u
#define PPUTLIMPL_UINT_TRAIT_0xCC7u HEX, 3271u, -825, (C, C, 7), 0x338u
#define PPUTLIMPL_UINT_TRAIT_0xCC6u HEX, 3270u, -826, (C, C, 6), 0x339u
#define PPUTLIMPL_UINT_TRAIT_0xCC5u HEX, 3269u, -827, (C, C, 5), 0x33Au
#define PPUTLIMPL_UINT_TRAIT_0xCC4u HEX, 3268u, -828, (C, C, 4), 0x33Bu
#define PPUTLIMPL_UINT_TRAIT_0xCC3u HEX, 3267u, -829, (C, C, 3), 0x33Cu
#define PPUTLIMPL_UINT_TRAIT_0xCC2u HEX, 3266u, -830, (C, C, 2), 0x33Du
#define PPUTLIMPL_UINT_TRAIT_0xCC1u HEX, 3265u, -831, (C, C, 1), 0x33Eu
#define PPUTLIMPL_UINT_TRAIT_0xCC0u HEX, 3264u, -832, (C, C, 0), 0x33Fu
#define PPUTLIMPL_UINT_TRAIT_0xCBFu HEX, 3263u, -833, (C, B, F), 0x340u
#define PPUTLIMPL_UINT_TRAIT_0xCBEu HEX, 3262u, -834, (C, B, E), 0x341u
#define PPUTLIMPL_UINT_TRAIT_0xCBDu HEX, 3261u, -835, (C, B, D), 0x342u
#define PPUTLIMPL_UINT_TRAIT_0xCBCu HEX, 3260u, -836, (C, B, C), 0x343u
#define PPUTLIMPL_UINT_TRAIT_0xCBBu HEX, 3259u, -837, (C, B, B), 0x344u
#define PPUTLIMPL_UINT_TRAIT_0xCBAu HEX, 3258u, -838, (C, B, A), 0x345u
#define PPUTLIMPL_UINT_TRAIT_0xCB9u HEX, 3257u, -839, (C, B, 9), 0x346u
#define PPUTLIMPL_UINT_TRAIT_0xCB8u HEX, 3256u, -840, (C, B, 8), 0x347u
#define PPUTLIMPL_UINT_TRAIT_0xCB7u HEX, 3255u, -841, (C, B, 7), 0x348u
#define PPUTLIMPL_UINT_TRAIT_0xCB6u HEX, 3254u, -842, (C, B, 6), 0x349u
#define PPUTLIMPL_UINT_TRAIT_0xCB5u HEX, 3253u, -843, (C, B, 5), 0x34Au
#define PPUTLIMPL_UINT_TRAIT_0xCB4u HEX, 3252u, -844, (C, B, 4), 0x34Bu
#define PPUTLIMPL_UINT_TRAIT_0xCB3u HEX, 3251u, -845, (C, B, 3), 0x34Cu
#define PPUTLIMPL_UINT_TRAIT_0xCB2u HEX, 3250u, -846, (C, B, 2), 0x34Du
#define PPUTLIMPL_UINT_TRAIT_0xCB1u HEX, 3249u, -847, (C, B, 1), 0x34Eu
#define PPUTLIMPL_UINT_TRAIT_0xCB0u HEX, 3248u, -848, (C, B, 0), 0x34Fu
#define PPUTLIMPL_UINT_TRAIT_0xCAFu HEX, 3247u, -849, (C, A, F), 0x350u
#define PPUTLIMPL_UINT_TRAIT_0xCAEu HEX, 3246u, -850, (C, A, E), 0x351u
#define PPUTLIMPL_UINT_TRAIT_0xCADu HEX, 3245u, -851, (C, A, D), 0x352u
#define PPUTLIMPL_UINT_TRAIT_0xCACu HEX, 3244u, -852, (C, A, C), 0x353u
#define PPUTLIMPL_UINT_TRAIT_0xCABu HEX, 3243u, -853, (C, A, B), 0x354u
#define PPUTLIMPL_UINT_TRAIT_0xCAAu HEX, 3242u, -854, (C, A, A), 0x355u
#define PPUTLIMPL_UINT_TRAIT_0xCA9u HEX, 3241u, -855, (C, A, 9), 0x356u
#define PPUTLIMPL_UINT_TRAIT_0xCA8u HEX, 3240u, -856, (C, A, 8), 0x357u
#define PPUTLIMPL_UINT_TRAIT_0xCA7u HEX, 3239u, -857, (C, A, 7), 0x358u
#define PPUTLIMPL_UINT_TRAIT_0xCA6u HEX, 3238u, -858, (C, A, 6), 0x359u
#define PPUTLIMPL_UINT_TRAIT_0xCA5u HEX, 3237u, -859, (C, A, 5), 0x35Au
#define PPUTLIMPL_UINT_TRAIT_0xCA4u HEX, 3236u, -860, (C, A, 4), 0x35Bu
#define PPUTLIMPL_UINT_TRAIT_0xCA3u HEX, 3235u, -861, (C, A, 3), 0x35Cu
#define PPUTLIMPL_UINT_TRAIT_0xCA2u HEX, 3234u, -862, (C, A, 2), 0x35Du
#define PPUTLIMPL_UINT_TRAIT_0xCA1u HEX, 3233u, -863, (C, A, 1), 0x35Eu
#define PPUTLIMPL_UINT_TRAIT_0xCA0u HEX, 3232u, -864, (C, A, 0), 0x35Fu
#define PPUTLIMPL_UINT_TRAIT_0xC9Fu HEX, 3231u, -865, (C, 9, F), 0x360u
#define PPUTLIMPL_UINT_TRAIT_0xC9Eu HEX, 3230u, -866, (C, 9, E), 0x361u
#define PPUTLIMPL_UINT_TRAIT_0xC9Du HEX, 3229u, -867, (C, 9, D), 0x362u
#define PPUTLIMPL_UINT_TRAIT_0xC9Cu HEX, 3228u, -868, (C, 9, C), 0x363u
#define PPUTLIMPL_UINT_TRAIT_0xC9Bu HEX, 3227u, -869, (C, 9, B), 0x364u
#define PPUTLIMPL_UINT_TRAIT_0xC9Au HEX, 3226u, -870, (C, 9, A), 0x365u
#define PPUTLIMPL_UINT_TRAIT_0xC99u HEX, 3225u, -871, (C, 9, 9), 0x366u
#define PPUTLIMPL_UINT_TRAIT_0xC98u HEX, 3224u, -872, (C, 9, 8), 0x367u
#define PPUTLIMPL_UINT_TRAIT_0xC97u HEX, 3223u, -873, (C, 9, 7), 0x368u
#define PPUTLIMPL_UINT_TRAIT_0xC96u HEX, 3222u, -874, (C, 9, 6), 0x369u
#define PPUTLIMPL_UINT_TRAIT_0xC95u HEX, 3221u, -875, (C, 9, 5), 0x36Au
#define PPUTLIMPL_UINT_TRAIT_0xC94u HEX, 3220u, -876, (C, 9, 4), 0x36Bu
#define PPUTLIMPL_UINT_TRAIT_0xC93u HEX, 3219u, -877, (C, 9, 3), 0x36Cu
#define PPUTLIMPL_UINT_TRAIT_0xC92u HEX, 3218u, -878, (C, 9, 2), 0x36Du
#define PPUTLIMPL_UINT_TRAIT_0xC91u HEX, 3217u, -879, (C, 9, 1), 0x36Eu
#define PPUTLIMPL_UINT_TRAIT_0xC90u HEX, 3216u, -880, (C, 9, 0), 0x36Fu
#define PPUTLIMPL_UINT_TRAIT_0xC8Fu HEX, 3215u, -881, (C, 8, F), 0x370u
#define PPUTLIMPL_UINT_TRAIT_0xC8Eu HEX, 3214u, -882, (C, 8, E), 0x371u
#define PPUTLIMPL_UINT_TRAIT_0xC8Du HEX, 3213u, -883, (C, 8, D), 0x372u
#define PPUTLIMPL_UINT_TRAIT_0xC8Cu HEX, 3212u, -884, (C, 8, C), 0x373u
#define PPUTLIMPL_UINT_TRAIT_0xC8Bu HEX, 3211u, -885, (C, 8, B), 0x374u
#define PPUTLIMPL_UINT_TRAIT_0xC8Au HEX, 3210u, -886, (C, 8, A), 0x375u
#define PPUTLIMPL_UINT_TRAIT_0xC89u HEX, 3209u, -887, (C, 8, 9), 0x376u
#define PPUTLIMPL_UINT_TRAIT_0xC88u HEX, 3208u, -888, (C, 8, 8), 0x377u
#define PPUTLIMPL_UINT_TRAIT_0xC87u HEX, 3207u, -889, (C, 8, 7), 0x378u
#define PPUTLIMPL_UINT_TRAIT_0xC86u HEX, 3206u, -890, (C, 8, 6), 0x379u
#define PPUTLIMPL_UINT_TRAIT_0xC85u HEX, 3205u, -891, (C, 8, 5), 0x37Au
#define PPUTLIMPL_UINT_TRAIT_0xC84u HEX, 3204u, -892, (C, 8, 4), 0x37Bu
#define PPUTLIMPL_UINT_TRAIT_0xC83u HEX, 3203u, -893, (C, 8, 3), 0x37Cu
#define PPUTLIMPL_UINT_TRAIT_0xC82u HEX, 3202u, -894, (C, 8, 2), 0x37Du
#define PPUTLIMPL_UINT_TRAIT_0xC81u HEX, 3201u, -895, (C, 8, 1), 0x37Eu
#define PPUTLIMPL_UINT_TRAIT_0xC80u HEX, 3200u, -896, (C, 8, 0), 0x37Fu
#define PPUTLIMPL_UINT_TRAIT_0xC7Fu HEX, 3199u, -897, (C, 7, F), 0x380u
#define PPUTLIMPL_UINT_TRAIT_0xC7Eu HEX, 3198u, -898, (C, 7, E), 0x381u
#define PPUTLIMPL_UINT_TRAIT_0xC7Du HEX, 3197u, -899, (C, 7, D), 0x382u
#define PPUTLIMPL_UINT_TRAIT_0xC7Cu HEX, 3196u, -900, (C, 7, C), 0x383u
#define PPUTLIMPL_UINT_TRAIT_0xC7Bu HEX, 3195u, -901, (C, 7, B), 0x384u
#define PPUTLIMPL_UINT_TRAIT_0xC7Au HEX, 3194u, -902, (C, 7, A), 0x385u
#define PPUTLIMPL_UINT_TRAIT_0xC79u HEX, 3193u, -903, (C, 7, 9), 0x386u
#define PPUTLIMPL_UINT_TRAIT_0xC78u HEX, 3192u, -904, (C, 7, 8), 0x387u
#define PPUTLIMPL_UINT_TRAIT_0xC77u HEX, 3191u, -905, (C, 7, 7), 0x388u
#define PPUTLIMPL_UINT_TRAIT_0xC76u HEX, 3190u, -906, (C, 7, 6), 0x389u
#define PPUTLIMPL_UINT_TRAIT_0xC75u HEX, 3189u, -907, (C, 7, 5), 0x38Au
#define PPUTLIMPL_UINT_TRAIT_0xC74u HEX, 3188u, -908, (C, 7, 4), 0x38Bu
#define PPUTLIMPL_UINT_TRAIT_0xC73u HEX, 3187u, -909, (C, 7, 3), 0x38Cu
#define PPUTLIMPL_UINT_TRAIT_0xC72u HEX, 3186u, -910, (C, 7, 2), 0x38Du
#define PPUTLIMPL_UINT_TRAIT_0xC71u HEX, 3185u, -911, (C, 7, 1), 0x38Eu
#define PPUTLIMPL_UINT_TRAIT_0xC70u HEX, 3184u, -912, (C, 7, 0), 0x38Fu
#define PPUTLIMPL_UINT_TRAIT_0xC6Fu HEX, 3183u, -913, (C, 6, F), 0x390u
#define PPUTLIMPL_UINT_TRAIT_0xC6Eu HEX, 3182u, -914, (C, 6, E), 0x391u
#define PPUTLIMPL_UINT_TRAIT_0xC6Du HEX, 3181u, -915, (C, 6, D), 0x392u
#define PPUTLIMPL_UINT_TRAIT_0xC6Cu HEX, 3180u, -916, (C, 6, C), 0x393u
#define PPUTLIMPL_UINT_TRAIT_0xC6Bu HEX, 3179u, -917, (C, 6, B), 0x394u
#define PPUTLIMPL_UINT_TRAIT_0xC6Au HEX, 3178u, -918, (C, 6, A), 0x395u
#define PPUTLIMPL_UINT_TRAIT_0xC69u HEX, 3177u, -919, (C, 6, 9), 0x396u
#define PPUTLIMPL_UINT_TRAIT_0xC68u HEX, 3176u, -920, (C, 6, 8), 0x397u
#define PPUTLIMPL_UINT_TRAIT_0xC67u HEX, 3175u, -921, (C, 6, 7), 0x398u
#define PPUTLIMPL_UINT_TRAIT_0xC66u HEX, 3174u, -922, (C, 6, 6), 0x399u
#define PPUTLIMPL_UINT_TRAIT_0xC65u HEX, 3173u, -923, (C, 6, 5), 0x39Au
#define PPUTLIMPL_UINT_TRAIT_0xC64u HEX, 3172u, -924, (C, 6, 4), 0x39Bu
#define PPUTLIMPL_UINT_TRAIT_0xC63u HEX, 3171u, -925, (C, 6, 3), 0x39Cu
#define PPUTLIMPL_UINT_TRAIT_0xC62u HEX, 3170u, -926, (C, 6, 2), 0x39Du
#define PPUTLIMPL_UINT_TRAIT_0xC61u HEX, 3169u, -927, (C, 6, 1), 0x39Eu
#define PPUTLIMPL_UINT_TRAIT_0xC60u HEX, 3168u, -928, (C, 6, 0), 0x39Fu
#define PPUTLIMPL_UINT_TRAIT_0xC5Fu HEX, 3167u, -929, (C, 5, F), 0x3A0u
#define PPUTLIMPL_UINT_TRAIT_0xC5Eu HEX, 3166u, -930, (C, 5, E), 0x3A1u
#define PPUTLIMPL_UINT_TRAIT_0xC5Du HEX, 3165u, -931, (C, 5, D), 0x3A2u
#define PPUTLIMPL_UINT_TRAIT_0xC5Cu HEX, 3164u, -932, (C, 5, C), 0x3A3u
#define PPUTLIMPL_UINT_TRAIT_0xC5Bu HEX, 3163u, -933, (C, 5, B), 0x3A4u
#define PPUTLIMPL_UINT_TRAIT_0xC5Au HEX, 3162u, -934, (C, 5, A), 0x3A5u
#define PPUTLIMPL_UINT_TRAIT_0xC59u HEX, 3161u, -935, (C, 5, 9), 0x3A6u
#define PPUTLIMPL_UINT_TRAIT_0xC58u HEX, 3160u, -936, (C, 5, 8), 0x3A7u
#define PPUTLIMPL_UINT_TRAIT_0xC57u HEX, 3159u, -937, (C, 5, 7), 0x3A8u
#define PPUTLIMPL_UINT_TRAIT_0xC56u HEX, 3158u, -938, (C, 5, 6), 0x3A9u
#define PPUTLIMPL_UINT_TRAIT_0xC55u HEX, 3157u, -939, (C, 5, 5), 0x3AAu
#define PPUTLIMPL_UINT_TRAIT_0xC54u HEX, 3156u, -940, (C, 5, 4), 0x3ABu
#define PPUTLIMPL_UINT_TRAIT_0xC53u HEX, 3155u, -941, (C, 5, 3), 0x3ACu
#define PPUTLIMPL_UINT_TRAIT_0xC52u HEX, 3154u, -942, (C, 5, 2), 0x3ADu
#define PPUTLIMPL_UINT_TRAIT_0xC51u HEX, 3153u, -943, (C, 5, 1), 0x3AEu
#define PPUTLIMPL_UINT_TRAIT_0xC50u HEX, 3152u, -944, (C, 5, 0), 0x3AFu
#define PPUTLIMPL_UINT_TRAIT_0xC4Fu HEX, 3151u, -945, (C, 4, F), 0x3B0u
#define PPUTLIMPL_UINT_TRAIT_0xC4Eu HEX, 3150u, -946, (C, 4, E), 0x3B1u
#define PPUTLIMPL_UINT_TRAIT_0xC4Du HEX, 3149u, -947, (C, 4, D), 0x3B2u
#define PPUTLIMPL_UINT_TRAIT_0xC4Cu HEX, 3148u, -948, (C, 4, C), 0x3B3u
#define PPUTLIMPL_UINT_TRAIT_0xC4Bu HEX, 3147u, -949, (C, 4, B), 0x3B4u
#define PPUTLIMPL_UINT_TRAIT_0xC4Au HEX, 3146u, -950, (C, 4, A), 0x3B5u
#define PPUTLIMPL_UINT_TRAIT_0xC49u HEX, 3145u, -951, (C, 4, 9), 0x3B6u
#define PPUTLIMPL_UINT_TRAIT_0xC48u HEX, 3144u, -952, (C, 4, 8), 0x3B7u
#define PPUTLIMPL_UINT_TRAIT_0xC47u HEX, 3143u, -953, (C, 4, 7), 0x3B8u
#define PPUTLIMPL_UINT_TRAIT_0xC46u HEX, 3142u, -954, (C, 4, 6), 0x3B9u
#define PPUTLIMPL_UINT_TRAIT_0xC45u HEX, 3141u, -955, (C, 4, 5), 0x3BAu
#define PPUTLIMPL_UINT_TRAIT_0xC44u HEX, 3140u, -956, (C, 4, 4), 0x3BBu
#define PPUTLIMPL_UINT_TRAIT_0xC43u HEX, 3139u, -957, (C, 4, 3), 0x3BCu
#define PPUTLIMPL_UINT_TRAIT_0xC42u HEX, 3138u, -958, (C, 4, 2), 0x3BDu
#define PPUTLIMPL_UINT_TRAIT_0xC41u HEX, 3137u, -959, (C, 4, 1), 0x3BEu
#define PPUTLIMPL_UINT_TRAIT_0xC40u HEX, 3136u, -960, (C, 4, 0), 0x3BFu
#define PPUTLIMPL_UINT_TRAIT_0xC3Fu HEX, 3135u, -961, (C, 3, F), 0x3C0u
#define PPUTLIMPL_UINT_TRAIT_0xC3Eu HEX, 3134u, -962, (C, 3, E), 0x3C1u
#define PPUTLIMPL_UINT_TRAIT_0xC3Du HEX, 3133u, -963, (C, 3, D), 0x3C2u
#define PPUTLIMPL_UINT_TRAIT_0xC3Cu HEX, 3132u, -964, (C, 3, C), 0x3C3u
#define PPUTLIMPL_UINT_TRAIT_0xC3Bu HEX, 3131u, -965, (C, 3, B), 0x3C4u
#define PPUTLIMPL_UINT_TRAIT_0xC3Au HEX, 3130u, -966, (C, 3, A), 0x3C5u
#define PPUTLIMPL_UINT_TRAIT_0xC39u HEX, 3129u, -967, (C, 3, 9), 0x3C6u
#define PPUTLIMPL_UINT_TRAIT_0xC38u HEX, 3128u, -968, (C, 3, 8), 0x3C7u
#define PPUTLIMPL_UINT_TRAIT_0xC37u HEX, 3127u, -969, (C, 3, 7), 0x3C8u
#define PPUTLIMPL_UINT_TRAIT_0xC36u HEX, 3126u, -970, (C, 3, 6), 0x3C9u
#define PPUTLIMPL_UINT_TRAIT_0xC35u HEX, 3125u, -971, (C, 3, 5), 0x3CAu
#define PPUTLIMPL_UINT_TRAIT_0xC34u HEX, 3124u, -972, (C, 3, 4), 0x3CBu
#define PPUTLIMPL_UINT_TRAIT_0xC33u HEX, 3123u, -973, (C, 3, 3), 0x3CCu
#define PPUTLIMPL_UINT_TRAIT_0xC32u HEX, 3122u, -974, (C, 3, 2), 0x3CDu
#define PPUTLIMPL_UINT_TRAIT_0xC31u HEX, 3121u, -975, (C, 3, 1), 0x3CEu
#define PPUTLIMPL_UINT_TRAIT_0xC30u HEX, 3120u, -976, (C, 3, 0), 0x3CFu
#define PPUTLIMPL_UINT_TRAIT_0xC2Fu HEX, 3119u, -977, (C, 2, F), 0x3D0u
#define PPUTLIMPL_UINT_TRAIT_0xC2Eu HEX, 3118u, -978, (C, 2, E), 0x3D1u
#define PPUTLIMPL_UINT_TRAIT_0xC2Du HEX, 3117u, -979, (C, 2, D), 0x3D2u
#define PPUTLIMPL_UINT_TRAIT_0xC2Cu HEX, 3116u, -980, (C, 2, C), 0x3D3u
#define PPUTLIMPL_UINT_TRAIT_0xC2Bu HEX, 3115u, -981, (C, 2, B), 0x3D4u
#define PPUTLIMPL_UINT_TRAIT_0xC2Au HEX, 3114u, -982, (C, 2, A), 0x3D5u
#define PPUTLIMPL_UINT_TRAIT_0xC29u HEX, 3113u, -983, (C, 2, 9), 0x3D6u
#define PPUTLIMPL_UINT_TRAIT_0xC28u HEX, 3112u, -984, (C, 2, 8), 0x3D7u
#define PPUTLIMPL_UINT_TRAIT_0xC27u HEX, 3111u, -985, (C, 2, 7), 0x3D8u
#define PPUTLIMPL_UINT_TRAIT_0xC26u HEX, 3110u, -986, (C, 2, 6), 0x3D9u
#define PPUTLIMPL_UINT_TRAIT_0xC25u HEX, 3109u, -987, (C, 2, 5), 0x3DAu
#define PPUTLIMPL_UINT_TRAIT_0xC24u HEX, 3108u, -988, (C, 2, 4), 0x3DBu
#define PPUTLIMPL_UINT_TRAIT_0xC23u HEX, 3107u, -989, (C, 2, 3), 0x3DCu
#define PPUTLIMPL_UINT_TRAIT_0xC22u HEX, 3106u, -990, (C, 2, 2), 0x3DDu
#define PPUTLIMPL_UINT_TRAIT_0xC21u HEX, 3105u, -991, (C, 2, 1), 0x3DEu
#define PPUTLIMPL_UINT_TRAIT_0xC20u HEX, 3104u, -992, (C, 2, 0), 0x3DFu
#define PPUTLIMPL_UINT_TRAIT_0xC1Fu HEX, 3103u, -993, (C, 1, F), 0x3E0u
#define PPUTLIMPL_UINT_TRAIT_0xC1Eu HEX, 3102u, -994, (C, 1, E), 0x3E1u
#define PPUTLIMPL_UINT_TRAIT_0xC1Du HEX, 3101u, -995, (C, 1, D), 0x3E2u
#define PPUTLIMPL_UINT_TRAIT_0xC1Cu HEX, 3100u, -996, (C, 1, C), 0x3E3u
#define PPUTLIMPL_UINT_TRAIT_0xC1Bu HEX, 3099u, -997, (C, 1, B), 0x3E4u
#define PPUTLIMPL_UINT_TRAIT_0xC1Au HEX, 3098u, -998, (C, 1, A), 0x3E5u
#define PPUTLIMPL_UINT_TRAIT_0xC19u HEX, 3097u, -999, (C, 1, 9), 0x3E6u
#define PPUTLIMPL_UINT_TRAIT_0xC18u HEX, 3096u, -1000, (C, 1, 8), 0x3E7u
#define PPUTLIMPL_UINT_TRAIT_0xC17u HEX, 3095u, -1001, (C, 1, 7), 0x3E8u
#define PPUTLIMPL_UINT_TRAIT_0xC16u HEX, 3094u, -1002, (C, 1, 6), 0x3E9u
#define PPUTLIMPL_UINT_TRAIT_0xC15u HEX, 3093u, -1003, (C, 1, 5), 0x3EAu
#define PPUTLIMPL_UINT_TRAIT_0xC14u HEX, 3092u, -1004, (C, 1, 4), 0x3EBu
#define PPUTLIMPL_UINT_TRAIT_0xC13u HEX, 3091u, -1005, (C, 1, 3), 0x3ECu
#define PPUTLIMPL_UINT_TRAIT_0xC12u HEX, 3090u, -1006, (C, 1, 2), 0x3EDu
#define PPUTLIMPL_UINT_TRAIT_0xC11u HEX, 3089u, -1007, (C, 1, 1), 0x3EEu
#define PPUTLIMPL_UINT_TRAIT_0xC10u HEX, 3088u, -1008, (C, 1, 0), 0x3EFu
#define PPUTLIMPL_UINT_TRAIT_0xC0Fu HEX, 3087u, -1009, (C, 0, F), 0x3F0u
#define PPUTLIMPL_UINT_TRAIT_0xC0Eu HEX, 3086u, -1010, (C, 0, E), 0x3F1u
#define PPUTLIMPL_UINT_TRAIT_0xC0Du HEX, 3085u, -1011, (C, 0, D), 0x3F2u
#define PPUTLIMPL_UINT_TRAIT_0xC0Cu HEX, 3084u, -1012, (C, 0, C), 0x3F3u
#define PPUTLIMPL_UINT_TRAIT_0xC0Bu HEX, 3083u, -1013, (C, 0, B), 0x3F4u
#define PPUTLIMPL_UINT_TRAIT_0xC0Au HEX, 3082u, -1014, (C, 0, A), 0x3F5u
#define PPUTLIMPL_UINT_TRAIT_0xC09u HEX, 3081u, -1015, (C, 0, 9), 0x3F6u
#define PPUTLIMPL_UINT_TRAIT_0xC08u HEX, 3080u, -1016, (C, 0, 8), 0x3F7u
#define PPUTLIMPL_UINT_TRAIT_0xC07u HEX, 3079u, -1017, (C, 0, 7), 0x3F8u
#define PPUTLIMPL_UINT_TRAIT_0xC06u HEX, 3078u, -1018, (C, 0, 6), 0x3F9u
#define PPUTLIMPL_UINT_TRAIT_0xC05u HEX, 3077u, -1019, (C, 0, 5), 0x3FAu
#define PPUTLIMPL_UINT_TRAIT_0xC04u HEX, 3076u, -1020, (C, 0, 4), 0x3FBu
#define PPUTLIMPL_UINT_TRAIT_0xC03u HEX, 3075u, -1021, (C, 0, 3), 0x3FCu
#define PPUTLIMPL_UINT_TRAIT_0xC02u HEX, 3074u, -1022, (C, 0, 2), 0x3FDu
#define PPUTLIMPL_UINT_TRAIT_0xC01u HEX, 3073u, -1023, (C, 0, 1), 0x3FEu
#define PPUTLIMPL_UINT_TRAIT_0xC00u HEX, 3072u, -1024, (C, 0, 0), 0x3FFu
#define PPUTLIMPL_UINT_TRAIT_0xBFFu HEX, 3071u, -1025, (B, F, F), 0x400u
#define PPUTLIMPL_UINT_TRAIT_0xBFEu HEX, 3070u, -1026, (B, F, E), 0x401u
#define PPUTLIMPL_UINT_TRAIT_0xBFDu HEX, 3069u, -1027, (B, F, D), 0x402u
#define PPUTLIMPL_UINT_TRAIT_0xBFCu HEX, 3068u, -1028, (B, F, C), 0x403u
#define PPUTLIMPL_UINT_TRAIT_0xBFBu HEX, 3067u, -1029, (B, F, B), 0x404u
#define PPUTLIMPL_UINT_TRAIT_0xBFAu HEX, 3066u, -1030, (B, F, A), 0x405u
#define PPUTLIMPL_UINT_TRAIT_0xBF9u HEX, 3065u, -1031, (B, F, 9), 0x406u
#define PPUTLIMPL_UINT_TRAIT_0xBF8u HEX, 3064u, -1032, (B, F, 8), 0x407u
#define PPUTLIMPL_UINT_TRAIT_0xBF7u HEX, 3063u, -1033, (B, F, 7), 0x408u
#define PPUTLIMPL_UINT_TRAIT_0xBF6u HEX, 3062u, -1034, (B, F, 6), 0x409u
#define PPUTLIMPL_UINT_TRAIT_0xBF5u HEX, 3061u, -1035, (B, F, 5), 0x40Au
#define PPUTLIMPL_UINT_TRAIT_0xBF4u HEX, 3060u, -1036, (B, F, 4), 0x40Bu
#define PPUTLIMPL_UINT_TRAIT_0xBF3u HEX, 3059u, -1037, (B, F, 3), 0x40Cu
#define PPUTLIMPL_UINT_TRAIT_0xBF2u HEX, 3058u, -1038, (B, F, 2), 0x40Du
#define PPUTLIMPL_UINT_TRAIT_0xBF1u HEX, 3057u, -1039, (B, F, 1), 0x40Eu
#define PPUTLIMPL_UINT_TRAIT_0xBF0u HEX, 3056u, -1040, (B, F, 0), 0x40Fu
#define PPUTLIMPL_UINT_TRAIT_0xBEFu HEX, 3055u, -1041, (B, E, F), 0x410u
#define PPUTLIMPL_UINT_TRAIT_0xBEEu HEX, 3054u, -1042, (B, E, E), 0x411u
#define PPUTLIMPL_UINT_TRAIT_0xBEDu HEX, 3053u, -1043, (B, E, D), 0x412u
#define PPUTLIMPL_UINT_TRAIT_0xBECu HEX, 3052u, -1044, (B, E, C), 0x413u
#define PPUTLIMPL_UINT_TRAIT_0xBEBu HEX, 3051u, -1045, (B, E, B), 0x414u
#define PPUTLIMPL_UINT_TRAIT_0xBEAu HEX, 3050u, -1046, (B, E, A), 0x415u
#define PPUTLIMPL_UINT_TRAIT_0xBE9u HEX, 3049u, -1047, (B, E, 9), 0x416u
#define PPUTLIMPL_UINT_TRAIT_0xBE8u HEX, 3048u, -1048, (B, E, 8), 0x417u
#define PPUTLIMPL_UINT_TRAIT_0xBE7u HEX, 3047u, -1049, (B, E, 7), 0x418u
#define PPUTLIMPL_UINT_TRAIT_0xBE6u HEX, 3046u, -1050, (B, E, 6), 0x419u
#define PPUTLIMPL_UINT_TRAIT_0xBE5u HEX, 3045u, -1051, (B, E, 5), 0x41Au
#define PPUTLIMPL_UINT_TRAIT_0xBE4u HEX, 3044u, -1052, (B, E, 4), 0x41Bu
#define PPUTLIMPL_UINT_TRAIT_0xBE3u HEX, 3043u, -1053, (B, E, 3), 0x41Cu
#define PPUTLIMPL_UINT_TRAIT_0xBE2u HEX, 3042u, -1054, (B, E, 2), 0x41Du
#define PPUTLIMPL_UINT_TRAIT_0xBE1u HEX, 3041u, -1055, (B, E, 1), 0x41Eu
#define PPUTLIMPL_UINT_TRAIT_0xBE0u HEX, 3040u, -1056, (B, E, 0), 0x41Fu
#define PPUTLIMPL_UINT_TRAIT_0xBDFu HEX, 3039u, -1057, (B, D, F), 0x420u
#define PPUTLIMPL_UINT_TRAIT_0xBDEu HEX, 3038u, -1058, (B, D, E), 0x421u
#define PPUTLIMPL_UINT_TRAIT_0xBDDu HEX, 3037u, -1059, (B, D, D), 0x422u
#define PPUTLIMPL_UINT_TRAIT_0xBDCu HEX, 3036u, -1060, (B, D, C), 0x423u
#define PPUTLIMPL_UINT_TRAIT_0xBDBu HEX, 3035u, -1061, (B, D, B), 0x424u
#define PPUTLIMPL_UINT_TRAIT_0xBDAu HEX, 3034u, -1062, (B, D, A), 0x425u
#define PPUTLIMPL_UINT_TRAIT_0xBD9u HEX, 3033u, -1063, (B, D, 9), 0x426u
#define PPUTLIMPL_UINT_TRAIT_0xBD8u HEX, 3032u, -1064, (B, D, 8), 0x427u
#define PPUTLIMPL_UINT_TRAIT_0xBD7u HEX, 3031u, -1065, (B, D, 7), 0x428u
#define PPUTLIMPL_UINT_TRAIT_0xBD6u HEX, 3030u, -1066, (B, D, 6), 0x429u
#define PPUTLIMPL_UINT_TRAIT_0xBD5u HEX, 3029u, -1067, (B, D, 5), 0x42Au
#define PPUTLIMPL_UINT_TRAIT_0xBD4u HEX, 3028u, -1068, (B, D, 4), 0x42Bu
#define PPUTLIMPL_UINT_TRAIT_0xBD3u HEX, 3027u, -1069, (B, D, 3), 0x42Cu
#define PPUTLIMPL_UINT_TRAIT_0xBD2u HEX, 3026u, -1070, (B, D, 2), 0x42Du
#define PPUTLIMPL_UINT_TRAIT_0xBD1u HEX, 3025u, -1071, (B, D, 1), 0x42Eu
#define PPUTLIMPL_UINT_TRAIT_0xBD0u HEX, 3024u, -1072, (B, D, 0), 0x42Fu
#define PPUTLIMPL_UINT_TRAIT_0xBCFu HEX, 3023u, -1073, (B, C, F), 0x430u
#define PPUTLIMPL_UINT_TRAIT_0xBCEu HEX, 3022u, -1074, (B, C, E), 0x431u
#define PPUTLIMPL_UINT_TRAIT_0xBCDu HEX, 3021u, -1075, (B, C, D), 0x432u
#define PPUTLIMPL_UINT_TRAIT_0xBCCu HEX, 3020u, -1076, (B, C, C), 0x433u
#define PPUTLIMPL_UINT_TRAIT_0xBCBu HEX, 3019u, -1077, (B, C, B), 0x434u
#define PPUTLIMPL_UINT_TRAIT_0xBCAu HEX, 3018u, -1078, (B, C, A), 0x435u
#define PPUTLIMPL_UINT_TRAIT_0xBC9u HEX, 3017u, -1079, (B, C, 9), 0x436u
#define PPUTLIMPL_UINT_TRAIT_0xBC8u HEX, 3016u, -1080, (B, C, 8), 0x437u
#define PPUTLIMPL_UINT_TRAIT_0xBC7u HEX, 3015u, -1081, (B, C, 7), 0x438u
#define PPUTLIMPL_UINT_TRAIT_0xBC6u HEX, 3014u, -1082, (B, C, 6), 0x439u
#define PPUTLIMPL_UINT_TRAIT_0xBC5u HEX, 3013u, -1083, (B, C, 5), 0x43Au
#define PPUTLIMPL_UINT_TRAIT_0xBC4u HEX, 3012u, -1084, (B, C, 4), 0x43Bu
#define PPUTLIMPL_UINT_TRAIT_0xBC3u HEX, 3011u, -1085, (B, C, 3), 0x43Cu
#define PPUTLIMPL_UINT_TRAIT_0xBC2u HEX, 3010u, -1086, (B, C, 2), 0x43Du
#define PPUTLIMPL_UINT_TRAIT_0xBC1u HEX, 3009u, -1087, (B, C, 1), 0x43Eu
#define PPUTLIMPL_UINT_TRAIT_0xBC0u HEX, 3008u, -1088, (B, C, 0), 0x43Fu
#define PPUTLIMPL_UINT_TRAIT_0xBBFu HEX, 3007u, -1089, (B, B, F), 0x440u
#define PPUTLIMPL_UINT_TRAIT_0xBBEu HEX, 3006u, -1090, (B, B, E), 0x441u
#define PPUTLIMPL_UINT_TRAIT_0xBBDu HEX, 3005u, -1091, (B, B, D), 0x442u
#define PPUTLIMPL_UINT_TRAIT_0xBBCu HEX, 3004u, -1092, (B, B, C), 0x443u
#define PPUTLIMPL_UINT_TRAIT_0xBBBu HEX, 3003u, -1093, (B, B, B), 0x444u
#define PPUTLIMPL_UINT_TRAIT_0xBBAu HEX, 3002u, -1094, (B, B, A), 0x445u
#define PPUTLIMPL_UINT_TRAIT_0xBB9u HEX, 3001u, -1095, (B, B, 9), 0x446u
#define PPUTLIMPL_UINT_TRAIT_0xBB8u HEX, 3000u, -1096, (B, B, 8), 0x447u
#define PPUTLIMPL_UINT_TRAIT_0xBB7u HEX, 2999u, -1097, (B, B, 7), 0x448u
#define PPUTLIMPL_UINT_TRAIT_0xBB6u HEX, 2998u, -1098, (B, B, 6), 0x449u
#define PPUTLIMPL_UINT_TRAIT_0xBB5u HEX, 2997u, -1099, (B, B, 5), 0x44Au
#define PPUTLIMPL_UINT_TRAIT_0xBB4u HEX, 2996u, -1100, (B, B, 4), 0x44Bu
#define PPUTLIMPL_UINT_TRAIT_0xBB3u HEX, 2995u, -1101, (B, B, 3), 0x44Cu
#define PPUTLIMPL_UINT_TRAIT_0xBB2u HEX, 2994u, -1102, (B, B, 2), 0x44Du
#define PPUTLIMPL_UINT_TRAIT_0xBB1u HEX, 2993u, -1103, (B, B, 1), 0x44Eu
#define PPUTLIMPL_UINT_TRAIT_0xBB0u HEX, 2992u, -1104, (B, B, 0), 0x44Fu
#define PPUTLIMPL_UINT_TRAIT_0xBAFu HEX, 2991u, -1105, (B, A, F), 0x450u
#define PPUTLIMPL_UINT_TRAIT_0xBAEu HEX, 2990u, -1106, (B, A, E), 0x451u
#define PPUTLIMPL_UINT_TRAIT_0xBADu HEX, 2989u, -1107, (B, A, D), 0x452u
#define PPUTLIMPL_UINT_TRAIT_0xBACu HEX, 2988u, -1108, (B, A, C), 0x453u
#define PPUTLIMPL_UINT_TRAIT_0xBABu HEX, 2987u, -1109, (B, A, B), 0x454u
#define PPUTLIMPL_UINT_TRAIT_0xBAAu HEX, 2986u, -1110, (B, A, A), 0x455u
#define PPUTLIMPL_UINT_TRAIT_0xBA9u HEX, 2985u, -1111, (B, A, 9), 0x456u
#define PPUTLIMPL_UINT_TRAIT_0xBA8u HEX, 2984u, -1112, (B, A, 8), 0x457u
#define PPUTLIMPL_UINT_TRAIT_0xBA7u HEX, 2983u, -1113, (B, A, 7), 0x458u
#define PPUTLIMPL_UINT_TRAIT_0xBA6u HEX, 2982u, -1114, (B, A, 6), 0x459u
#define PPUTLIMPL_UINT_TRAIT_0xBA5u HEX, 2981u, -1115, (B, A, 5), 0x45Au
#define PPUTLIMPL_UINT_TRAIT_0xBA4u HEX, 2980u, -1116, (B, A, 4), 0x45Bu
#define PPUTLIMPL_UINT_TRAIT_0xBA3u HEX, 2979u, -1117, (B, A, 3), 0x45Cu
#define PPUTLIMPL_UINT_TRAIT_0xBA2u HEX, 2978u, -1118, (B, A, 2), 0x45Du
#define PPUTLIMPL_UINT_TRAIT_0xBA1u HEX, 2977u, -1119, (B, A, 1), 0x45Eu
#define PPUTLIMPL_UINT_TRAIT_0xBA0u HEX, 2976u, -1120, (B, A, 0), 0x45Fu
#define PPUTLIMPL_UINT_TRAIT_0xB9Fu HEX, 2975u, -1121, (B, 9, F), 0x460u
#define PPUTLIMPL_UINT_TRAIT_0xB9Eu HEX, 2974u, -1122, (B, 9, E), 0x461u
#define PPUTLIMPL_UINT_TRAIT_0xB9Du HEX, 2973u, -1123, (B, 9, D), 0x462u
#define PPUTLIMPL_UINT_TRAIT_0xB9Cu HEX, 2972u, -1124, (B, 9, C), 0x463u
#define PPUTLIMPL_UINT_TRAIT_0xB9Bu HEX, 2971u, -1125, (B, 9, B), 0x464u
#define PPUTLIMPL_UINT_TRAIT_0xB9Au HEX, 2970u, -1126, (B, 9, A), 0x465u
#define PPUTLIMPL_UINT_TRAIT_0xB99u HEX, 2969u, -1127, (B, 9, 9), 0x466u
#define PPUTLIMPL_UINT_TRAIT_0xB98u HEX, 2968u, -1128, (B, 9, 8), 0x467u
#define PPUTLIMPL_UINT_TRAIT_0xB97u HEX, 2967u, -1129, (B, 9, 7), 0x468u
#define PPUTLIMPL_UINT_TRAIT_0xB96u HEX, 2966u, -1130, (B, 9, 6), 0x469u
#define PPUTLIMPL_UINT_TRAIT_0xB95u HEX, 2965u, -1131, (B, 9, 5), 0x46Au
#define PPUTLIMPL_UINT_TRAIT_0xB94u HEX, 2964u, -1132, (B, 9, 4), 0x46Bu
#define PPUTLIMPL_UINT_TRAIT_0xB93u HEX, 2963u, -1133, (B, 9, 3), 0x46Cu
#define PPUTLIMPL_UINT_TRAIT_0xB92u HEX, 2962u, -1134, (B, 9, 2), 0x46Du
#define PPUTLIMPL_UINT_TRAIT_0xB91u HEX, 2961u, -1135, (B, 9, 1), 0x46Eu
#define PPUTLIMPL_UINT_TRAIT_0xB90u HEX, 2960u, -1136, (B, 9, 0), 0x46Fu
#define PPUTLIMPL_UINT_TRAIT_0xB8Fu HEX, 2959u, -1137, (B, 8, F), 0x470u
#define PPUTLIMPL_UINT_TRAIT_0xB8Eu HEX, 2958u, -1138, (B, 8, E), 0x471u
#define PPUTLIMPL_UINT_TRAIT_0xB8Du HEX, 2957u, -1139, (B, 8, D), 0x472u
#define PPUTLIMPL_UINT_TRAIT_0xB8Cu HEX, 2956u, -1140, (B, 8, C), 0x473u
#define PPUTLIMPL_UINT_TRAIT_0xB8Bu HEX, 2955u, -1141, (B, 8, B), 0x474u
#define PPUTLIMPL_UINT_TRAIT_0xB8Au HEX, 2954u, -1142, (B, 8, A), 0x475u
#define PPUTLIMPL_UINT_TRAIT_0xB89u HEX, 2953u, -1143, (B, 8, 9), 0x476u
#define PPUTLIMPL_UINT_TRAIT_0xB88u HEX, 2952u, -1144, (B, 8, 8), 0x477u
#define PPUTLIMPL_UINT_TRAIT_0xB87u HEX, 2951u, -1145, (B, 8, 7), 0x478u
#define PPUTLIMPL_UINT_TRAIT_0xB86u HEX, 2950u, -1146, (B, 8, 6), 0x479u
#define PPUTLIMPL_UINT_TRAIT_0xB85u HEX, 2949u, -1147, (B, 8, 5), 0x47Au
#define PPUTLIMPL_UINT_TRAIT_0xB84u HEX, 2948u, -1148, (B, 8, 4), 0x47Bu
#define PPUTLIMPL_UINT_TRAIT_0xB83u HEX, 2947u, -1149, (B, 8, 3), 0x47Cu
#define PPUTLIMPL_UINT_TRAIT_0xB82u HEX, 2946u, -1150, (B, 8, 2), 0x47Du
#define PPUTLIMPL_UINT_TRAIT_0xB81u HEX, 2945u, -1151, (B, 8, 1), 0x47Eu
#define PPUTLIMPL_UINT_TRAIT_0xB80u HEX, 2944u, -1152, (B, 8, 0), 0x47Fu
#define PPUTLIMPL_UINT_TRAIT_0xB7Fu HEX, 2943u, -1153, (B, 7, F), 0x480u
#define PPUTLIMPL_UINT_TRAIT_0xB7Eu HEX, 2942u, -1154, (B, 7, E), 0x481u
#define PPUTLIMPL_UINT_TRAIT_0xB7Du HEX, 2941u, -1155, (B, 7, D), 0x482u
#define PPUTLIMPL_UINT_TRAIT_0xB7Cu HEX, 2940u, -1156, (B, 7, C), 0x483u
#define PPUTLIMPL_UINT_TRAIT_0xB7Bu HEX, 2939u, -1157, (B, 7, B), 0x484u
#define PPUTLIMPL_UINT_TRAIT_0xB7Au HEX, 2938u, -1158, (B, 7, A), 0x485u
#define PPUTLIMPL_UINT_TRAIT_0xB79u HEX, 2937u, -1159, (B, 7, 9), 0x486u
#define PPUTLIMPL_UINT_TRAIT_0xB78u HEX, 2936u, -1160, (B, 7, 8), 0x487u
#define PPUTLIMPL_UINT_TRAIT_0xB77u HEX, 2935u, -1161, (B, 7, 7), 0x488u
#define PPUTLIMPL_UINT_TRAIT_0xB76u HEX, 2934u, -1162, (B, 7, 6), 0x489u
#define PPUTLIMPL_UINT_TRAIT_0xB75u HEX, 2933u, -1163, (B, 7, 5), 0x48Au
#define PPUTLIMPL_UINT_TRAIT_0xB74u HEX, 2932u, -1164, (B, 7, 4), 0x48Bu
#define PPUTLIMPL_UINT_TRAIT_0xB73u HEX, 2931u, -1165, (B, 7, 3), 0x48Cu
#define PPUTLIMPL_UINT_TRAIT_0xB72u HEX, 2930u, -1166, (B, 7, 2), 0x48Du
#define PPUTLIMPL_UINT_TRAIT_0xB71u HEX, 2929u, -1167, (B, 7, 1), 0x48Eu
#define PPUTLIMPL_UINT_TRAIT_0xB70u HEX, 2928u, -1168, (B, 7, 0), 0x48Fu
#define PPUTLIMPL_UINT_TRAIT_0xB6Fu HEX, 2927u, -1169, (B, 6, F), 0x490u
#define PPUTLIMPL_UINT_TRAIT_0xB6Eu HEX, 2926u, -1170, (B, 6, E), 0x491u
#define PPUTLIMPL_UINT_TRAIT_0xB6Du HEX, 2925u, -1171, (B, 6, D), 0x492u
#define PPUTLIMPL_UINT_TRAIT_0xB6Cu HEX, 2924u, -1172, (B, 6, C), 0x493u
#define PPUTLIMPL_UINT_TRAIT_0xB6Bu HEX, 2923u, -1173, (B, 6, B), 0x494u
#define PPUTLIMPL_UINT_TRAIT_0xB6Au HEX, 2922u, -1174, (B, 6, A), 0x495u
#define PPUTLIMPL_UINT_TRAIT_0xB69u HEX, 2921u, -1175, (B, 6, 9), 0x496u
#define PPUTLIMPL_UINT_TRAIT_0xB68u HEX, 2920u, -1176, (B, 6, 8), 0x497u
#define PPUTLIMPL_UINT_TRAIT_0xB67u HEX, 2919u, -1177, (B, 6, 7), 0x498u
#define PPUTLIMPL_UINT_TRAIT_0xB66u HEX, 2918u, -1178, (B, 6, 6), 0x499u
#define PPUTLIMPL_UINT_TRAIT_0xB65u HEX, 2917u, -1179, (B, 6, 5), 0x49Au
#define PPUTLIMPL_UINT_TRAIT_0xB64u HEX, 2916u, -1180, (B, 6, 4), 0x49Bu
#define PPUTLIMPL_UINT_TRAIT_0xB63u HEX, 2915u, -1181, (B, 6, 3), 0x49Cu
#define PPUTLIMPL_UINT_TRAIT_0xB62u HEX, 2914u, -1182, (B, 6, 2), 0x49Du
#define PPUTLIMPL_UINT_TRAIT_0xB61u HEX, 2913u, -1183, (B, 6, 1), 0x49Eu
#define PPUTLIMPL_UINT_TRAIT_0xB60u HEX, 2912u, -1184, (B, 6, 0), 0x49Fu
#define PPUTLIMPL_UINT_TRAIT_0xB5Fu HEX, 2911u, -1185, (B, 5, F), 0x4A0u
#define PPUTLIMPL_UINT_TRAIT_0xB5Eu HEX, 2910u, -1186, (B, 5, E), 0x4A1u
#define PPUTLIMPL_UINT_TRAIT_0xB5Du HEX, 2909u, -1187, (B, 5, D), 0x4A2u
#define PPUTLIMPL_UINT_TRAIT_0xB5Cu HEX, 2908u, -1188, (B, 5, C), 0x4A3u
#define PPUTLIMPL_UINT_TRAIT_0xB5Bu HEX, 2907u, -1189, (B, 5, B), 0x4A4u
#define PPUTLIMPL_UINT_TRAIT_0xB5Au HEX, 2906u, -1190, (B, 5, A), 0x4A5u
#define PPUTLIMPL_UINT_TRAIT_0xB59u HEX, 2905u, -1191, (B, 5, 9), 0x4A6u
#define PPUTLIMPL_UINT_TRAIT_0xB58u HEX, 2904u, -1192, (B, 5, 8), 0x4A7u
#define PPUTLIMPL_UINT_TRAIT_0xB57u HEX, 2903u, -1193, (B, 5, 7), 0x4A8u
#define PPUTLIMPL_UINT_TRAIT_0xB56u HEX, 2902u, -1194, (B, 5, 6), 0x4A9u
#define PPUTLIMPL_UINT_TRAIT_0xB55u HEX, 2901u, -1195, (B, 5, 5), 0x4AAu
#define PPUTLIMPL_UINT_TRAIT_0xB54u HEX, 2900u, -1196, (B, 5, 4), 0x4ABu
#define PPUTLIMPL_UINT_TRAIT_0xB53u HEX, 2899u, -1197, (B, 5, 3), 0x4ACu
#define PPUTLIMPL_UINT_TRAIT_0xB52u HEX, 2898u, -1198, (B, 5, 2), 0x4ADu
#define PPUTLIMPL_UINT_TRAIT_0xB51u HEX, 2897u, -1199, (B, 5, 1), 0x4AEu
#define PPUTLIMPL_UINT_TRAIT_0xB50u HEX, 2896u, -1200, (B, 5, 0), 0x4AFu
#define PPUTLIMPL_UINT_TRAIT_0xB4Fu HEX, 2895u, -1201, (B, 4, F), 0x4B0u
#define PPUTLIMPL_UINT_TRAIT_0xB4Eu HEX, 2894u, -1202, (B, 4, E), 0x4B1u
#define PPUTLIMPL_UINT_TRAIT_0xB4Du HEX, 2893u, -1203, (B, 4, D), 0x4B2u
#define PPUTLIMPL_UINT_TRAIT_0xB4Cu HEX, 2892u, -1204, (B, 4, C), 0x4B3u
#define PPUTLIMPL_UINT_TRAIT_0xB4Bu HEX, 2891u, -1205, (B, 4, B), 0x4B4u
#define PPUTLIMPL_UINT_TRAIT_0xB4Au HEX, 2890u, -1206, (B, 4, A), 0x4B5u
#define PPUTLIMPL_UINT_TRAIT_0xB49u HEX, 2889u, -1207, (B, 4, 9), 0x4B6u
#define PPUTLIMPL_UINT_TRAIT_0xB48u HEX, 2888u, -1208, (B, 4, 8), 0x4B7u
#define PPUTLIMPL_UINT_TRAIT_0xB47u HEX, 2887u, -1209, (B, 4, 7), 0x4B8u
#define PPUTLIMPL_UINT_TRAIT_0xB46u HEX, 2886u, -1210, (B, 4, 6), 0x4B9u
#define PPUTLIMPL_UINT_TRAIT_0xB45u HEX, 2885u, -1211, (B, 4, 5), 0x4BAu
#define PPUTLIMPL_UINT_TRAIT_0xB44u HEX, 2884u, -1212, (B, 4, 4), 0x4BBu
#define PPUTLIMPL_UINT_TRAIT_0xB43u HEX, 2883u, -1213, (B, 4, 3), 0x4BCu
#define PPUTLIMPL_UINT_TRAIT_0xB42u HEX, 2882u, -1214, (B, 4, 2), 0x4BDu
#define PPUTLIMPL_UINT_TRAIT_0xB41u HEX, 2881u, -1215, (B, 4, 1), 0x4BEu
#define PPUTLIMPL_UINT_TRAIT_0xB40u HEX, 2880u, -1216, (B, 4, 0), 0x4BFu
#define PPUTLIMPL_UINT_TRAIT_0xB3Fu HEX, 2879u, -1217, (B, 3, F), 0x4C0u
#define PPUTLIMPL_UINT_TRAIT_0xB3Eu HEX, 2878u, -1218, (B, 3, E), 0x4C1u
#define PPUTLIMPL_UINT_TRAIT_0xB3Du HEX, 2877u, -1219, (B, 3, D), 0x4C2u
#define PPUTLIMPL_UINT_TRAIT_0xB3Cu HEX, 2876u, -1220, (B, 3, C), 0x4C3u
#define PPUTLIMPL_UINT_TRAIT_0xB3Bu HEX, 2875u, -1221, (B, 3, B), 0x4C4u
#define PPUTLIMPL_UINT_TRAIT_0xB3Au HEX, 2874u, -1222, (B, 3, A), 0x4C5u
#define PPUTLIMPL_UINT_TRAIT_0xB39u HEX, 2873u, -1223, (B, 3, 9), 0x4C6u
#define PPUTLIMPL_UINT_TRAIT_0xB38u HEX, 2872u, -1224, (B, 3, 8), 0x4C7u
#define PPUTLIMPL_UINT_TRAIT_0xB37u HEX, 2871u, -1225, (B, 3, 7), 0x4C8u
#define PPUTLIMPL_UINT_TRAIT_0xB36u HEX, 2870u, -1226, (B, 3, 6), 0x4C9u
#define PPUTLIMPL_UINT_TRAIT_0xB35u HEX, 2869u, -1227, (B, 3, 5), 0x4CAu
#define PPUTLIMPL_UINT_TRAIT_0xB34u HEX, 2868u, -1228, (B, 3, 4), 0x4CBu
#define PPUTLIMPL_UINT_TRAIT_0xB33u HEX, 2867u, -1229, (B, 3, 3), 0x4CCu
#define PPUTLIMPL_UINT_TRAIT_0xB32u HEX, 2866u, -1230, (B, 3, 2), 0x4CDu
#define PPUTLIMPL_UINT_TRAIT_0xB31u HEX, 2865u, -1231, (B, 3, 1), 0x4CEu
#define PPUTLIMPL_UINT_TRAIT_0xB30u HEX, 2864u, -1232, (B, 3, 0), 0x4CFu
#define PPUTLIMPL_UINT_TRAIT_0xB2Fu HEX, 2863u, -1233, (B, 2, F), 0x4D0u
#define PPUTLIMPL_UINT_TRAIT_0xB2Eu HEX, 2862u, -1234, (B, 2, E), 0x4D1u
#define PPUTLIMPL_UINT_TRAIT_0xB2Du HEX, 2861u, -1235, (B, 2, D), 0x4D2u
#define PPUTLIMPL_UINT_TRAIT_0xB2Cu HEX, 2860u, -1236, (B, 2, C), 0x4D3u
#define PPUTLIMPL_UINT_TRAIT_0xB2Bu HEX, 2859u, -1237, (B, 2, B), 0x4D4u
#define PPUTLIMPL_UINT_TRAIT_0xB2Au HEX, 2858u, -1238, (B, 2, A), 0x4D5u
#define PPUTLIMPL_UINT_TRAIT_0xB29u HEX, 2857u, -1239, (B, 2, 9), 0x4D6u
#define PPUTLIMPL_UINT_TRAIT_0xB28u HEX, 2856u, -1240, (B, 2, 8), 0x4D7u
#define PPUTLIMPL_UINT_TRAIT_0xB27u HEX, 2855u, -1241, (B, 2, 7), 0x4D8u
#define PPUTLIMPL_UINT_TRAIT_0xB26u HEX, 2854u, -1242, (B, 2, 6), 0x4D9u
#define PPUTLIMPL_UINT_TRAIT_0xB25u HEX, 2853u, -1243, (B, 2, 5), 0x4DAu
#define PPUTLIMPL_UINT_TRAIT_0xB24u HEX, 2852u, -1244, (B, 2, 4), 0x4DBu
#define PPUTLIMPL_UINT_TRAIT_0xB23u HEX, 2851u, -1245, (B, 2, 3), 0x4DCu
#define PPUTLIMPL_UINT_TRAIT_0xB22u HEX, 2850u, -1246, (B, 2, 2), 0x4DDu
#define PPUTLIMPL_UINT_TRAIT_0xB21u HEX, 2849u, -1247, (B, 2, 1), 0x4DEu
#define PPUTLIMPL_UINT_TRAIT_0xB20u HEX, 2848u, -1248, (B, 2, 0), 0x4DFu
#define PPUTLIMPL_UINT_TRAIT_0xB1Fu HEX, 2847u, -1249, (B, 1, F), 0x4E0u
#define PPUTLIMPL_UINT_TRAIT_0xB1Eu HEX, 2846u, -1250, (B, 1, E), 0x4E1u
#define PPUTLIMPL_UINT_TRAIT_0xB1Du HEX, 2845u, -1251, (B, 1, D), 0x4E2u
#define PPUTLIMPL_UINT_TRAIT_0xB1Cu HEX, 2844u, -1252, (B, 1, C), 0x4E3u
#define PPUTLIMPL_UINT_TRAIT_0xB1Bu HEX, 2843u, -1253, (B, 1, B), 0x4E4u
#define PPUTLIMPL_UINT_TRAIT_0xB1Au HEX, 2842u, -1254, (B, 1, A), 0x4E5u
#define PPUTLIMPL_UINT_TRAIT_0xB19u HEX, 2841u, -1255, (B, 1, 9), 0x4E6u
#define PPUTLIMPL_UINT_TRAIT_0xB18u HEX, 2840u, -1256, (B, 1, 8), 0x4E7u
#define PPUTLIMPL_UINT_TRAIT_0xB17u HEX, 2839u, -1257, (B, 1, 7), 0x4E8u
#define PPUTLIMPL_UINT_TRAIT_0xB16u HEX, 2838u, -1258, (B, 1, 6), 0x4E9u
#define PPUTLIMPL_UINT_TRAIT_0xB15u HEX, 2837u, -1259, (B, 1, 5), 0x4EAu
#define PPUTLIMPL_UINT_TRAIT_0xB14u HEX, 2836u, -1260, (B, 1, 4), 0x4EBu
#define PPUTLIMPL_UINT_TRAIT_0xB13u HEX, 2835u, -1261, (B, 1, 3), 0x4ECu
#define PPUTLIMPL_UINT_TRAIT_0xB12u HEX, 2834u, -1262, (B, 1, 2), 0x4EDu
#define PPUTLIMPL_UINT_TRAIT_0xB11u HEX, 2833u, -1263, (B, 1, 1), 0x4EEu
#define PPUTLIMPL_UINT_TRAIT_0xB10u HEX, 2832u, -1264, (B, 1, 0), 0x4EFu
#define PPUTLIMPL_UINT_TRAIT_0xB0Fu HEX, 2831u, -1265, (B, 0, F), 0x4F0u
#define PPUTLIMPL_UINT_TRAIT_0xB0Eu HEX, 2830u, -1266, (B, 0, E), 0x4F1u
#define PPUTLIMPL_UINT_TRAIT_0xB0Du HEX, 2829u, -1267, (B, 0, D), 0x4F2u
#define PPUTLIMPL_UINT_TRAIT_0xB0Cu HEX, 2828u, -1268, (B, 0, C), 0x4F3u
#define PPUTLIMPL_UINT_TRAIT_0xB0Bu HEX, 2827u, -1269, (B, 0, B), 0x4F4u
#define PPUTLIMPL_UINT_TRAIT_0xB0Au HEX, 2826u, -1270, (B, 0, A), 0x4F5u
#define PPUTLIMPL_UINT_TRAIT_0xB09u HEX, 2825u, -1271, (B, 0, 9), 0x4F6u
#define PPUTLIMPL_UINT_TRAIT_0xB08u HEX, 2824u, -1272, (B, 0, 8), 0x4F7u
#define PPUTLIMPL_UINT_TRAIT_0xB07u HEX, 2823u, -1273, (B, 0, 7), 0x4F8u
#define PPUTLIMPL_UINT_TRAIT_0xB06u HEX, 2822u, -1274, (B, 0, 6), 0x4F9u
#define PPUTLIMPL_UINT_TRAIT_0xB05u HEX, 2821u, -1275, (B, 0, 5), 0x4FAu
#define PPUTLIMPL_UINT_TRAIT_0xB04u HEX, 2820u, -1276, (B, 0, 4), 0x4FBu
#define PPUTLIMPL_UINT_TRAIT_0xB03u HEX, 2819u, -1277, (B, 0, 3), 0x4FCu
#define PPUTLIMPL_UINT_TRAIT_0xB02u HEX, 2818u, -1278, (B, 0, 2), 0x4FDu
#define PPUTLIMPL_UINT_TRAIT_0xB01u HEX, 2817u, -1279, (B, 0, 1), 0x4FEu
#define PPUTLIMPL_UINT_TRAIT_0xB00u HEX, 2816u, -1280, (B, 0, 0), 0x4FFu
#define PPUTLIMPL_UINT_TRAIT_0xAFFu HEX, 2815u, -1281, (A, F, F), 0x500u
#define PPUTLIMPL_UINT_TRAIT_0xAFEu HEX, 2814u, -1282, (A, F, E), 0x501u
#define PPUTLIMPL_UINT_TRAIT_0xAFDu HEX, 2813u, -1283, (A, F, D), 0x502u
#define PPUTLIMPL_UINT_TRAIT_0xAFCu HEX, 2812u, -1284, (A, F, C), 0x503u
#define PPUTLIMPL_UINT_TRAIT_0xAFBu HEX, 2811u, -1285, (A, F, B), 0x504u
#define PPUTLIMPL_UINT_TRAIT_0xAFAu HEX, 2810u, -1286, (A, F, A), 0x505u
#define PPUTLIMPL_UINT_TRAIT_0xAF9u HEX, 2809u, -1287, (A, F, 9), 0x506u
#define PPUTLIMPL_UINT_TRAIT_0xAF8u HEX, 2808u, -1288, (A, F, 8), 0x507u
#define PPUTLIMPL_UINT_TRAIT_0xAF7u HEX, 2807u, -1289, (A, F, 7), 0x508u
#define PPUTLIMPL_UINT_TRAIT_0xAF6u HEX, 2806u, -1290, (A, F, 6), 0x509u
#define PPUTLIMPL_UINT_TRAIT_0xAF5u HEX, 2805u, -1291, (A, F, 5), 0x50Au
#define PPUTLIMPL_UINT_TRAIT_0xAF4u HEX, 2804u, -1292, (A, F, 4), 0x50Bu
#define PPUTLIMPL_UINT_TRAIT_0xAF3u HEX, 2803u, -1293, (A, F, 3), 0x50Cu
#define PPUTLIMPL_UINT_TRAIT_0xAF2u HEX, 2802u, -1294, (A, F, 2), 0x50Du
#define PPUTLIMPL_UINT_TRAIT_0xAF1u HEX, 2801u, -1295, (A, F, 1), 0x50Eu
#define PPUTLIMPL_UINT_TRAIT_0xAF0u HEX, 2800u, -1296, (A, F, 0), 0x50Fu
#define PPUTLIMPL_UINT_TRAIT_0xAEFu HEX, 2799u, -1297, (A, E, F), 0x510u
#define PPUTLIMPL_UINT_TRAIT_0xAEEu HEX, 2798u, -1298, (A, E, E), 0x511u
#define PPUTLIMPL_UINT_TRAIT_0xAEDu HEX, 2797u, -1299, (A, E, D), 0x512u
#define PPUTLIMPL_UINT_TRAIT_0xAECu HEX, 2796u, -1300, (A, E, C), 0x513u
#define PPUTLIMPL_UINT_TRAIT_0xAEBu HEX, 2795u, -1301, (A, E, B), 0x514u
#define PPUTLIMPL_UINT_TRAIT_0xAEAu HEX, 2794u, -1302, (A, E, A), 0x515u
#define PPUTLIMPL_UINT_TRAIT_0xAE9u HEX, 2793u, -1303, (A, E, 9), 0x516u
#define PPUTLIMPL_UINT_TRAIT_0xAE8u HEX, 2792u, -1304, (A, E, 8), 0x517u
#define PPUTLIMPL_UINT_TRAIT_0xAE7u HEX, 2791u, -1305, (A, E, 7), 0x518u
#define PPUTLIMPL_UINT_TRAIT_0xAE6u HEX, 2790u, -1306, (A, E, 6), 0x519u
#define PPUTLIMPL_UINT_TRAIT_0xAE5u HEX, 2789u, -1307, (A, E, 5), 0x51Au
#define PPUTLIMPL_UINT_TRAIT_0xAE4u HEX, 2788u, -1308, (A, E, 4), 0x51Bu
#define PPUTLIMPL_UINT_TRAIT_0xAE3u HEX, 2787u, -1309, (A, E, 3), 0x51Cu
#define PPUTLIMPL_UINT_TRAIT_0xAE2u HEX, 2786u, -1310, (A, E, 2), 0x51Du
#define PPUTLIMPL_UINT_TRAIT_0xAE1u HEX, 2785u, -1311, (A, E, 1), 0x51Eu
#define PPUTLIMPL_UINT_TRAIT_0xAE0u HEX, 2784u, -1312, (A, E, 0), 0x51Fu
#define PPUTLIMPL_UINT_TRAIT_0xADFu HEX, 2783u, -1313, (A, D, F), 0x520u
#define PPUTLIMPL_UINT_TRAIT_0xADEu HEX, 2782u, -1314, (A, D, E), 0x521u
#define PPUTLIMPL_UINT_TRAIT_0xADDu HEX, 2781u, -1315, (A, D, D), 0x522u
#define PPUTLIMPL_UINT_TRAIT_0xADCu HEX, 2780u, -1316, (A, D, C), 0x523u
#define PPUTLIMPL_UINT_TRAIT_0xADBu HEX, 2779u, -1317, (A, D, B), 0x524u
#define PPUTLIMPL_UINT_TRAIT_0xADAu HEX, 2778u, -1318, (A, D, A), 0x525u
#define PPUTLIMPL_UINT_TRAIT_0xAD9u HEX, 2777u, -1319, (A, D, 9), 0x526u
#define PPUTLIMPL_UINT_TRAIT_0xAD8u HEX, 2776u, -1320, (A, D, 8), 0x527u
#define PPUTLIMPL_UINT_TRAIT_0xAD7u HEX, 2775u, -1321, (A, D, 7), 0x528u
#define PPUTLIMPL_UINT_TRAIT_0xAD6u HEX, 2774u, -1322, (A, D, 6), 0x529u
#define PPUTLIMPL_UINT_TRAIT_0xAD5u HEX, 2773u, -1323, (A, D, 5), 0x52Au
#define PPUTLIMPL_UINT_TRAIT_0xAD4u HEX, 2772u, -1324, (A, D, 4), 0x52Bu
#define PPUTLIMPL_UINT_TRAIT_0xAD3u HEX, 2771u, -1325, (A, D, 3), 0x52Cu
#define PPUTLIMPL_UINT_TRAIT_0xAD2u HEX, 2770u, -1326, (A, D, 2), 0x52Du
#define PPUTLIMPL_UINT_TRAIT_0xAD1u HEX, 2769u, -1327, (A, D, 1), 0x52Eu
#define PPUTLIMPL_UINT_TRAIT_0xAD0u HEX, 2768u, -1328, (A, D, 0), 0x52Fu
#define PPUTLIMPL_UINT_TRAIT_0xACFu HEX, 2767u, -1329, (A, C, F), 0x530u
#define PPUTLIMPL_UINT_TRAIT_0xACEu HEX, 2766u, -1330, (A, C, E), 0x531u
#define PPUTLIMPL_UINT_TRAIT_0xACDu HEX, 2765u, -1331, (A, C, D), 0x532u
#define PPUTLIMPL_UINT_TRAIT_0xACCu HEX, 2764u, -1332, (A, C, C), 0x533u
#define PPUTLIMPL_UINT_TRAIT_0xACBu HEX, 2763u, -1333, (A, C, B), 0x534u
#define PPUTLIMPL_UINT_TRAIT_0xACAu HEX, 2762u, -1334, (A, C, A), 0x535u
#define PPUTLIMPL_UINT_TRAIT_0xAC9u HEX, 2761u, -1335, (A, C, 9), 0x536u
#define PPUTLIMPL_UINT_TRAIT_0xAC8u HEX, 2760u, -1336, (A, C, 8), 0x537u
#define PPUTLIMPL_UINT_TRAIT_0xAC7u HEX, 2759u, -1337, (A, C, 7), 0x538u
#define PPUTLIMPL_UINT_TRAIT_0xAC6u HEX, 2758u, -1338, (A, C, 6), 0x539u
#define PPUTLIMPL_UINT_TRAIT_0xAC5u HEX, 2757u, -1339, (A, C, 5), 0x53Au
#define PPUTLIMPL_UINT_TRAIT_0xAC4u HEX, 2756u, -1340, (A, C, 4), 0x53Bu
#define PPUTLIMPL_UINT_TRAIT_0xAC3u HEX, 2755u, -1341, (A, C, 3), 0x53Cu
#define PPUTLIMPL_UINT_TRAIT_0xAC2u HEX, 2754u, -1342, (A, C, 2), 0x53Du
#define PPUTLIMPL_UINT_TRAIT_0xAC1u HEX, 2753u, -1343, (A, C, 1), 0x53Eu
#define PPUTLIMPL_UINT_TRAIT_0xAC0u HEX, 2752u, -1344, (A, C, 0), 0x53Fu
#define PPUTLIMPL_UINT_TRAIT_0xABFu HEX, 2751u, -1345, (A, B, F), 0x540u
#define PPUTLIMPL_UINT_TRAIT_0xABEu HEX, 2750u, -1346, (A, B, E), 0x541u
#define PPUTLIMPL_UINT_TRAIT_0xABDu HEX, 2749u, -1347, (A, B, D), 0x542u
#define PPUTLIMPL_UINT_TRAIT_0xABCu HEX, 2748u, -1348, (A, B, C), 0x543u
#define PPUTLIMPL_UINT_TRAIT_0xABBu HEX, 2747u, -1349, (A, B, B), 0x544u
#define PPUTLIMPL_UINT_TRAIT_0xABAu HEX, 2746u, -1350, (A, B, A), 0x545u
#define PPUTLIMPL_UINT_TRAIT_0xAB9u HEX, 2745u, -1351, (A, B, 9), 0x546u
#define PPUTLIMPL_UINT_TRAIT_0xAB8u HEX, 2744u, -1352, (A, B, 8), 0x547u
#define PPUTLIMPL_UINT_TRAIT_0xAB7u HEX, 2743u, -1353, (A, B, 7), 0x548u
#define PPUTLIMPL_UINT_TRAIT_0xAB6u HEX, 2742u, -1354, (A, B, 6), 0x549u
#define PPUTLIMPL_UINT_TRAIT_0xAB5u HEX, 2741u, -1355, (A, B, 5), 0x54Au
#define PPUTLIMPL_UINT_TRAIT_0xAB4u HEX, 2740u, -1356, (A, B, 4), 0x54Bu
#define PPUTLIMPL_UINT_TRAIT_0xAB3u HEX, 2739u, -1357, (A, B, 3), 0x54Cu
#define PPUTLIMPL_UINT_TRAIT_0xAB2u HEX, 2738u, -1358, (A, B, 2), 0x54Du
#define PPUTLIMPL_UINT_TRAIT_0xAB1u HEX, 2737u, -1359, (A, B, 1), 0x54Eu
#define PPUTLIMPL_UINT_TRAIT_0xAB0u HEX, 2736u, -1360, (A, B, 0), 0x54Fu
#define PPUTLIMPL_UINT_TRAIT_0xAAFu HEX, 2735u, -1361, (A, A, F), 0x550u
#define PPUTLIMPL_UINT_TRAIT_0xAAEu HEX, 2734u, -1362, (A, A, E), 0x551u
#define PPUTLIMPL_UINT_TRAIT_0xAADu HEX, 2733u, -1363, (A, A, D), 0x552u
#define PPUTLIMPL_UINT_TRAIT_0xAACu HEX, 2732u, -1364, (A, A, C), 0x553u
#define PPUTLIMPL_UINT_TRAIT_0xAABu HEX, 2731u, -1365, (A, A, B), 0x554u
#define PPUTLIMPL_UINT_TRAIT_0xAAAu HEX, 2730u, -1366, (A, A, A), 0x555u
#define PPUTLIMPL_UINT_TRAIT_0xAA9u HEX, 2729u, -1367, (A, A, 9), 0x556u
#define PPUTLIMPL_UINT_TRAIT_0xAA8u HEX, 2728u, -1368, (A, A, 8), 0x557u
#define PPUTLIMPL_UINT_TRAIT_0xAA7u HEX, 2727u, -1369, (A, A, 7), 0x558u
#define PPUTLIMPL_UINT_TRAIT_0xAA6u HEX, 2726u, -1370, (A, A, 6), 0x559u
#define PPUTLIMPL_UINT_TRAIT_0xAA5u HEX, 2725u, -1371, (A, A, 5), 0x55Au
#define PPUTLIMPL_UINT_TRAIT_0xAA4u HEX, 2724u, -1372, (A, A, 4), 0x55Bu
#define PPUTLIMPL_UINT_TRAIT_0xAA3u HEX, 2723u, -1373, (A, A, 3), 0x55Cu
#define PPUTLIMPL_UINT_TRAIT_0xAA2u HEX, 2722u, -1374, (A, A, 2), 0x55Du
#define PPUTLIMPL_UINT_TRAIT_0xAA1u HEX, 2721u, -1375, (A, A, 1), 0x55Eu
#define PPUTLIMPL_UINT_TRAIT_0xAA0u HEX, 2720u, -1376, (A, A, 0), 0x55Fu
#define PPUTLIMPL_UINT_TRAIT_0xA9Fu HEX, 2719u, -1377, (A, 9, F), 0x560u
#define PPUTLIMPL_UINT_TRAIT_0xA9Eu HEX, 2718u, -1378, (A, 9, E), 0x561u
#define PPUTLIMPL_UINT_TRAIT_0xA9Du HEX, 2717u, -1379, (A, 9, D), 0x562u
#define PPUTLIMPL_UINT_TRAIT_0xA9Cu HEX, 2716u, -1380, (A, 9, C), 0x563u
#define PPUTLIMPL_UINT_TRAIT_0xA9Bu HEX, 2715u, -1381, (A, 9, B), 0x564u
#define PPUTLIMPL_UINT_TRAIT_0xA9Au HEX, 2714u, -1382, (A, 9, A), 0x565u
#define PPUTLIMPL_UINT_TRAIT_0xA99u HEX, 2713u, -1383, (A, 9, 9), 0x566u
#define PPUTLIMPL_UINT_TRAIT_0xA98u HEX, 2712u, -1384, (A, 9, 8), 0x567u
#define PPUTLIMPL_UINT_TRAIT_0xA97u HEX, 2711u, -1385, (A, 9, 7), 0x568u
#define PPUTLIMPL_UINT_TRAIT_0xA96u HEX, 2710u, -1386, (A, 9, 6), 0x569u
#define PPUTLIMPL_UINT_TRAIT_0xA95u HEX, 2709u, -1387, (A, 9, 5), 0x56Au
#define PPUTLIMPL_UINT_TRAIT_0xA94u HEX, 2708u, -1388, (A, 9, 4), 0x56Bu
#define PPUTLIMPL_UINT_TRAIT_0xA93u HEX, 2707u, -1389, (A, 9, 3), 0x56Cu
#define PPUTLIMPL_UINT_TRAIT_0xA92u HEX, 2706u, -1390, (A, 9, 2), 0x56Du
#define PPUTLIMPL_UINT_TRAIT_0xA91u HEX, 2705u, -1391, (A, 9, 1), 0x56Eu
#define PPUTLIMPL_UINT_TRAIT_0xA90u HEX, 2704u, -1392, (A, 9, 0), 0x56Fu
#define PPUTLIMPL_UINT_TRAIT_0xA8Fu HEX, 2703u, -1393, (A, 8, F), 0x570u
#define PPUTLIMPL_UINT_TRAIT_0xA8Eu HEX, 2702u, -1394, (A, 8, E), 0x571u
#define PPUTLIMPL_UINT_TRAIT_0xA8Du HEX, 2701u, -1395, (A, 8, D), 0x572u
#define PPUTLIMPL_UINT_TRAIT_0xA8Cu HEX, 2700u, -1396, (A, 8, C), 0x573u
#define PPUTLIMPL_UINT_TRAIT_0xA8Bu HEX, 2699u, -1397, (A, 8, B), 0x574u
#define PPUTLIMPL_UINT_TRAIT_0xA8Au HEX, 2698u, -1398, (A, 8, A), 0x575u
#define PPUTLIMPL_UINT_TRAIT_0xA89u HEX, 2697u, -1399, (A, 8, 9), 0x576u
#define PPUTLIMPL_UINT_TRAIT_0xA88u HEX, 2696u, -1400, (A, 8, 8), 0x577u
#define PPUTLIMPL_UINT_TRAIT_0xA87u HEX, 2695u, -1401, (A, 8, 7), 0x578u
#define PPUTLIMPL_UINT_TRAIT_0xA86u HEX, 2694u, -1402, (A, 8, 6), 0x579u
#define PPUTLIMPL_UINT_TRAIT_0xA85u HEX, 2693u, -1403, (A, 8, 5), 0x57Au
#define PPUTLIMPL_UINT_TRAIT_0xA84u HEX, 2692u, -1404, (A, 8, 4), 0x57Bu
#define PPUTLIMPL_UINT_TRAIT_0xA83u HEX, 2691u, -1405, (A, 8, 3), 0x57Cu
#define PPUTLIMPL_UINT_TRAIT_0xA82u HEX, 2690u, -1406, (A, 8, 2), 0x57Du
#define PPUTLIMPL_UINT_TRAIT_0xA81u HEX, 2689u, -1407, (A, 8, 1), 0x57Eu
#define PPUTLIMPL_UINT_TRAIT_0xA80u HEX, 2688u, -1408, (A, 8, 0), 0x57Fu
#define PPUTLIMPL_UINT_TRAIT_0xA7Fu HEX, 2687u, -1409, (A, 7, F), 0x580u
#define PPUTLIMPL_UINT_TRAIT_0xA7Eu HEX, 2686u, -1410, (A, 7, E), 0x581u
#define PPUTLIMPL_UINT_TRAIT_0xA7Du HEX, 2685u, -1411, (A, 7, D), 0x582u
#define PPUTLIMPL_UINT_TRAIT_0xA7Cu HEX, 2684u, -1412, (A, 7, C), 0x583u
#define PPUTLIMPL_UINT_TRAIT_0xA7Bu HEX, 2683u, -1413, (A, 7, B), 0x584u
#define PPUTLIMPL_UINT_TRAIT_0xA7Au HEX, 2682u, -1414, (A, 7, A), 0x585u
#define PPUTLIMPL_UINT_TRAIT_0xA79u HEX, 2681u, -1415, (A, 7, 9), 0x586u
#define PPUTLIMPL_UINT_TRAIT_0xA78u HEX, 2680u, -1416, (A, 7, 8), 0x587u
#define PPUTLIMPL_UINT_TRAIT_0xA77u HEX, 2679u, -1417, (A, 7, 7), 0x588u
#define PPUTLIMPL_UINT_TRAIT_0xA76u HEX, 2678u, -1418, (A, 7, 6), 0x589u
#define PPUTLIMPL_UINT_TRAIT_0xA75u HEX, 2677u, -1419, (A, 7, 5), 0x58Au
#define PPUTLIMPL_UINT_TRAIT_0xA74u HEX, 2676u, -1420, (A, 7, 4), 0x58Bu
#define PPUTLIMPL_UINT_TRAIT_0xA73u HEX, 2675u, -1421, (A, 7, 3), 0x58Cu
#define PPUTLIMPL_UINT_TRAIT_0xA72u HEX, 2674u, -1422, (A, 7, 2), 0x58Du
#define PPUTLIMPL_UINT_TRAIT_0xA71u HEX, 2673u, -1423, (A, 7, 1), 0x58Eu
#define PPUTLIMPL_UINT_TRAIT_0xA70u HEX, 2672u, -1424, (A, 7, 0), 0x58Fu
#define PPUTLIMPL_UINT_TRAIT_0xA6Fu HEX, 2671u, -1425, (A, 6, F), 0x590u
#define PPUTLIMPL_UINT_TRAIT_0xA6Eu HEX, 2670u, -1426, (A, 6, E), 0x591u
#define PPUTLIMPL_UINT_TRAIT_0xA6Du HEX, 2669u, -1427, (A, 6, D), 0x592u
#define PPUTLIMPL_UINT_TRAIT_0xA6Cu HEX, 2668u, -1428, (A, 6, C), 0x593u
#define PPUTLIMPL_UINT_TRAIT_0xA6Bu HEX, 2667u, -1429, (A, 6, B), 0x594u
#define PPUTLIMPL_UINT_TRAIT_0xA6Au HEX, 2666u, -1430, (A, 6, A), 0x595u
#define PPUTLIMPL_UINT_TRAIT_0xA69u HEX, 2665u, -1431, (A, 6, 9), 0x596u
#define PPUTLIMPL_UINT_TRAIT_0xA68u HEX, 2664u, -1432, (A, 6, 8), 0x597u
#define PPUTLIMPL_UINT_TRAIT_0xA67u HEX, 2663u, -1433, (A, 6, 7), 0x598u
#define PPUTLIMPL_UINT_TRAIT_0xA66u HEX, 2662u, -1434, (A, 6, 6), 0x599u
#define PPUTLIMPL_UINT_TRAIT_0xA65u HEX, 2661u, -1435, (A, 6, 5), 0x59Au
#define PPUTLIMPL_UINT_TRAIT_0xA64u HEX, 2660u, -1436, (A, 6, 4), 0x59Bu
#define PPUTLIMPL_UINT_TRAIT_0xA63u HEX, 2659u, -1437, (A, 6, 3), 0x59Cu
#define PPUTLIMPL_UINT_TRAIT_0xA62u HEX, 2658u, -1438, (A, 6, 2), 0x59Du
#define PPUTLIMPL_UINT_TRAIT_0xA61u HEX, 2657u, -1439, (A, 6, 1), 0x59Eu
#define PPUTLIMPL_UINT_TRAIT_0xA60u HEX, 2656u, -1440, (A, 6, 0), 0x59Fu
#define PPUTLIMPL_UINT_TRAIT_0xA5Fu HEX, 2655u, -1441, (A, 5, F), 0x5A0u
#define PPUTLIMPL_UINT_TRAIT_0xA5Eu HEX, 2654u, -1442, (A, 5, E), 0x5A1u
#define PPUTLIMPL_UINT_TRAIT_0xA5Du HEX, 2653u, -1443, (A, 5, D), 0x5A2u
#define PPUTLIMPL_UINT_TRAIT_0xA5Cu HEX, 2652u, -1444, (A, 5, C), 0x5A3u
#define PPUTLIMPL_UINT_TRAIT_0xA5Bu HEX, 2651u, -1445, (A, 5, B), 0x5A4u
#define PPUTLIMPL_UINT_TRAIT_0xA5Au HEX, 2650u, -1446, (A, 5, A), 0x5A5u
#define PPUTLIMPL_UINT_TRAIT_0xA59u HEX, 2649u, -1447, (A, 5, 9), 0x5A6u
#define PPUTLIMPL_UINT_TRAIT_0xA58u HEX, 2648u, -1448, (A, 5, 8), 0x5A7u
#define PPUTLIMPL_UINT_TRAIT_0xA57u HEX, 2647u, -1449, (A, 5, 7), 0x5A8u
#define PPUTLIMPL_UINT_TRAIT_0xA56u HEX, 2646u, -1450, (A, 5, 6), 0x5A9u
#define PPUTLIMPL_UINT_TRAIT_0xA55u HEX, 2645u, -1451, (A, 5, 5), 0x5AAu
#define PPUTLIMPL_UINT_TRAIT_0xA54u HEX, 2644u, -1452, (A, 5, 4), 0x5ABu
#define PPUTLIMPL_UINT_TRAIT_0xA53u HEX, 2643u, -1453, (A, 5, 3), 0x5ACu
#define PPUTLIMPL_UINT_TRAIT_0xA52u HEX, 2642u, -1454, (A, 5, 2), 0x5ADu
#define PPUTLIMPL_UINT_TRAIT_0xA51u HEX, 2641u, -1455, (A, 5, 1), 0x5AEu
#define PPUTLIMPL_UINT_TRAIT_0xA50u HEX, 2640u, -1456, (A, 5, 0), 0x5AFu
#define PPUTLIMPL_UINT_TRAIT_0xA4Fu HEX, 2639u, -1457, (A, 4, F), 0x5B0u
#define PPUTLIMPL_UINT_TRAIT_0xA4Eu HEX, 2638u, -1458, (A, 4, E), 0x5B1u
#define PPUTLIMPL_UINT_TRAIT_0xA4Du HEX, 2637u, -1459, (A, 4, D), 0x5B2u
#define PPUTLIMPL_UINT_TRAIT_0xA4Cu HEX, 2636u, -1460, (A, 4, C), 0x5B3u
#define PPUTLIMPL_UINT_TRAIT_0xA4Bu HEX, 2635u, -1461, (A, 4, B), 0x5B4u
#define PPUTLIMPL_UINT_TRAIT_0xA4Au HEX, 2634u, -1462, (A, 4, A), 0x5B5u
#define PPUTLIMPL_UINT_TRAIT_0xA49u HEX, 2633u, -1463, (A, 4, 9), 0x5B6u
#define PPUTLIMPL_UINT_TRAIT_0xA48u HEX, 2632u, -1464, (A, 4, 8), 0x5B7u
#define PPUTLIMPL_UINT_TRAIT_0xA47u HEX, 2631u, -1465, (A, 4, 7), 0x5B8u
#define PPUTLIMPL_UINT_TRAIT_0xA46u HEX, 2630u, -1466, (A, 4, 6), 0x5B9u
#define PPUTLIMPL_UINT_TRAIT_0xA45u HEX, 2629u, -1467, (A, 4, 5), 0x5BAu
#define PPUTLIMPL_UINT_TRAIT_0xA44u HEX, 2628u, -1468, (A, 4, 4), 0x5BBu
#define PPUTLIMPL_UINT_TRAIT_0xA43u HEX, 2627u, -1469, (A, 4, 3), 0x5BCu
#define PPUTLIMPL_UINT_TRAIT_0xA42u HEX, 2626u, -1470, (A, 4, 2), 0x5BDu
#define PPUTLIMPL_UINT_TRAIT_0xA41u HEX, 2625u, -1471, (A, 4, 1), 0x5BEu
#define PPUTLIMPL_UINT_TRAIT_0xA40u HEX, 2624u, -1472, (A, 4, 0), 0x5BFu
#define PPUTLIMPL_UINT_TRAIT_0xA3Fu HEX, 2623u, -1473, (A, 3, F), 0x5C0u
#define PPUTLIMPL_UINT_TRAIT_0xA3Eu HEX, 2622u, -1474, (A, 3, E), 0x5C1u
#define PPUTLIMPL_UINT_TRAIT_0xA3Du HEX, 2621u, -1475, (A, 3, D), 0x5C2u
#define PPUTLIMPL_UINT_TRAIT_0xA3Cu HEX, 2620u, -1476, (A, 3, C), 0x5C3u
#define PPUTLIMPL_UINT_TRAIT_0xA3Bu HEX, 2619u, -1477, (A, 3, B), 0x5C4u
#define PPUTLIMPL_UINT_TRAIT_0xA3Au HEX, 2618u, -1478, (A, 3, A), 0x5C5u
#define PPUTLIMPL_UINT_TRAIT_0xA39u HEX, 2617u, -1479, (A, 3, 9), 0x5C6u
#define PPUTLIMPL_UINT_TRAIT_0xA38u HEX, 2616u, -1480, (A, 3, 8), 0x5C7u
#define PPUTLIMPL_UINT_TRAIT_0xA37u HEX, 2615u, -1481, (A, 3, 7), 0x5C8u
#define PPUTLIMPL_UINT_TRAIT_0xA36u HEX, 2614u, -1482, (A, 3, 6), 0x5C9u
#define PPUTLIMPL_UINT_TRAIT_0xA35u HEX, 2613u, -1483, (A, 3, 5), 0x5CAu
#define PPUTLIMPL_UINT_TRAIT_0xA34u HEX, 2612u, -1484, (A, 3, 4), 0x5CBu
#define PPUTLIMPL_UINT_TRAIT_0xA33u HEX, 2611u, -1485, (A, 3, 3), 0x5CCu
#define PPUTLIMPL_UINT_TRAIT_0xA32u HEX, 2610u, -1486, (A, 3, 2), 0x5CDu
#define PPUTLIMPL_UINT_TRAIT_0xA31u HEX, 2609u, -1487, (A, 3, 1), 0x5CEu
#define PPUTLIMPL_UINT_TRAIT_0xA30u HEX, 2608u, -1488, (A, 3, 0), 0x5CFu
#define PPUTLIMPL_UINT_TRAIT_0xA2Fu HEX, 2607u, -1489, (A, 2, F), 0x5D0u
#define PPUTLIMPL_UINT_TRAIT_0xA2Eu HEX, 2606u, -1490, (A, 2, E), 0x5D1u
#define PPUTLIMPL_UINT_TRAIT_0xA2Du HEX, 2605u, -1491, (A, 2, D), 0x5D2u
#define PPUTLIMPL_UINT_TRAIT_0xA2Cu HEX, 2604u, -1492, (A, 2, C), 0x5D3u
#define PPUTLIMPL_UINT_TRAIT_0xA2Bu HEX, 2603u, -1493, (A, 2, B), 0x5D4u
#define PPUTLIMPL_UINT_TRAIT_0xA2Au HEX, 2602u, -1494, (A, 2, A), 0x5D5u
#define PPUTLIMPL_UINT_TRAIT_0xA29u HEX, 2601u, -1495, (A, 2, 9), 0x5D6u
#define PPUTLIMPL_UINT_TRAIT_0xA28u HEX, 2600u, -1496, (A, 2, 8), 0x5D7u
#define PPUTLIMPL_UINT_TRAIT_0xA27u HEX, 2599u, -1497, (A, 2, 7), 0x5D8u
#define PPUTLIMPL_UINT_TRAIT_0xA26u HEX, 2598u, -1498, (A, 2, 6), 0x5D9u
#define PPUTLIMPL_UINT_TRAIT_0xA25u HEX, 2597u, -1499, (A, 2, 5), 0x5DAu
#define PPUTLIMPL_UINT_TRAIT_0xA24u HEX, 2596u, -1500, (A, 2, 4), 0x5DBu
#define PPUTLIMPL_UINT_TRAIT_0xA23u HEX, 2595u, -1501, (A, 2, 3), 0x5DCu
#define PPUTLIMPL_UINT_TRAIT_0xA22u HEX, 2594u, -1502, (A, 2, 2), 0x5DDu
#define PPUTLIMPL_UINT_TRAIT_0xA21u HEX, 2593u, -1503, (A, 2, 1), 0x5DEu
#define PPUTLIMPL_UINT_TRAIT_0xA20u HEX, 2592u, -1504, (A, 2, 0), 0x5DFu
#define PPUTLIMPL_UINT_TRAIT_0xA1Fu HEX, 2591u, -1505, (A, 1, F), 0x5E0u
#define PPUTLIMPL_UINT_TRAIT_0xA1Eu HEX, 2590u, -1506, (A, 1, E), 0x5E1u
#define PPUTLIMPL_UINT_TRAIT_0xA1Du HEX, 2589u, -1507, (A, 1, D), 0x5E2u
#define PPUTLIMPL_UINT_TRAIT_0xA1Cu HEX, 2588u, -1508, (A, 1, C), 0x5E3u
#define PPUTLIMPL_UINT_TRAIT_0xA1Bu HEX, 2587u, -1509, (A, 1, B), 0x5E4u
#define PPUTLIMPL_UINT_TRAIT_0xA1Au HEX, 2586u, -1510, (A, 1, A), 0x5E5u
#define PPUTLIMPL_UINT_TRAIT_0xA19u HEX, 2585u, -1511, (A, 1, 9), 0x5E6u
#define PPUTLIMPL_UINT_TRAIT_0xA18u HEX, 2584u, -1512, (A, 1, 8), 0x5E7u
#define PPUTLIMPL_UINT_TRAIT_0xA17u HEX, 2583u, -1513, (A, 1, 7), 0x5E8u
#define PPUTLIMPL_UINT_TRAIT_0xA16u HEX, 2582u, -1514, (A, 1, 6), 0x5E9u
#define PPUTLIMPL_UINT_TRAIT_0xA15u HEX, 2581u, -1515, (A, 1, 5), 0x5EAu
#define PPUTLIMPL_UINT_TRAIT_0xA14u HEX, 2580u, -1516, (A, 1, 4), 0x5EBu
#define PPUTLIMPL_UINT_TRAIT_0xA13u HEX, 2579u, -1517, (A, 1, 3), 0x5ECu
#define PPUTLIMPL_UINT_TRAIT_0xA12u HEX, 2578u, -1518, (A, 1, 2), 0x5EDu
#define PPUTLIMPL_UINT_TRAIT_0xA11u HEX, 2577u, -1519, (A, 1, 1), 0x5EEu
#define PPUTLIMPL_UINT_TRAIT_0xA10u HEX, 2576u, -1520, (A, 1, 0), 0x5EFu
#define PPUTLIMPL_UINT_TRAIT_0xA0Fu HEX, 2575u, -1521, (A, 0, F), 0x5F0u
#define PPUTLIMPL_UINT_TRAIT_0xA0Eu HEX, 2574u, -1522, (A, 0, E), 0x5F1u
#define PPUTLIMPL_UINT_TRAIT_0xA0Du HEX, 2573u, -1523, (A, 0, D), 0x5F2u
#define PPUTLIMPL_UINT_TRAIT_0xA0Cu HEX, 2572u, -1524, (A, 0, C), 0x5F3u
#define PPUTLIMPL_UINT_TRAIT_0xA0Bu HEX, 2571u, -1525, (A, 0, B), 0x5F4u
#define PPUTLIMPL_UINT_TRAIT_0xA0Au HEX, 2570u, -1526, (A, 0, A), 0x5F5u
#define PPUTLIMPL_UINT_TRAIT_0xA09u HEX, 2569u, -1527, (A, 0, 9), 0x5F6u
#define PPUTLIMPL_UINT_TRAIT_0xA08u HEX, 2568u, -1528, (A, 0, 8), 0x5F7u
#define PPUTLIMPL_UINT_TRAIT_0xA07u HEX, 2567u, -1529, (A, 0, 7), 0x5F8u
#define PPUTLIMPL_UINT_TRAIT_0xA06u HEX, 2566u, -1530, (A, 0, 6), 0x5F9u
#define PPUTLIMPL_UINT_TRAIT_0xA05u HEX, 2565u, -1531, (A, 0, 5), 0x5FAu
#define PPUTLIMPL_UINT_TRAIT_0xA04u HEX, 2564u, -1532, (A, 0, 4), 0x5FBu
#define PPUTLIMPL_UINT_TRAIT_0xA03u HEX, 2563u, -1533, (A, 0, 3), 0x5FCu
#define PPUTLIMPL_UINT_TRAIT_0xA02u HEX, 2562u, -1534, (A, 0, 2), 0x5FDu
#define PPUTLIMPL_UINT_TRAIT_0xA01u HEX, 2561u, -1535, (A, 0, 1), 0x5FEu
#define PPUTLIMPL_UINT_TRAIT_0xA00u HEX, 2560u, -1536, (A, 0, 0), 0x5FFu
#define PPUTLIMPL_UINT_TRAIT_0x9FFu HEX, 2559u, -1537, (9, F, F), 0x600u
#define PPUTLIMPL_UINT_TRAIT_0x9FEu HEX, 2558u, -1538, (9, F, E), 0x601u
#define PPUTLIMPL_UINT_TRAIT_0x9FDu HEX, 2557u, -1539, (9, F, D), 0x602u
#define PPUTLIMPL_UINT_TRAIT_0x9FCu HEX, 2556u, -1540, (9, F, C), 0x603u
#define PPUTLIMPL_UINT_TRAIT_0x9FBu HEX, 2555u, -1541, (9, F, B), 0x604u
#define PPUTLIMPL_UINT_TRAIT_0x9FAu HEX, 2554u, -1542, (9, F, A), 0x605u
#define PPUTLIMPL_UINT_TRAIT_0x9F9u HEX, 2553u, -1543, (9, F, 9), 0x606u
#define PPUTLIMPL_UINT_TRAIT_0x9F8u HEX, 2552u, -1544, (9, F, 8), 0x607u
#define PPUTLIMPL_UINT_TRAIT_0x9F7u HEX, 2551u, -1545, (9, F, 7), 0x608u
#define PPUTLIMPL_UINT_TRAIT_0x9F6u HEX, 2550u, -1546, (9, F, 6), 0x609u
#define PPUTLIMPL_UINT_TRAIT_0x9F5u HEX, 2549u, -1547, (9, F, 5), 0x60Au
#define PPUTLIMPL_UINT_TRAIT_0x9F4u HEX, 2548u, -1548, (9, F, 4), 0x60Bu
#define PPUTLIMPL_UINT_TRAIT_0x9F3u HEX, 2547u, -1549, (9, F, 3), 0x60Cu
#define PPUTLIMPL_UINT_TRAIT_0x9F2u HEX, 2546u, -1550, (9, F, 2), 0x60Du
#define PPUTLIMPL_UINT_TRAIT_0x9F1u HEX, 2545u, -1551, (9, F, 1), 0x60Eu
#define PPUTLIMPL_UINT_TRAIT_0x9F0u HEX, 2544u, -1552, (9, F, 0), 0x60Fu
#define PPUTLIMPL_UINT_TRAIT_0x9EFu HEX, 2543u, -1553, (9, E, F), 0x610u
#define PPUTLIMPL_UINT_TRAIT_0x9EEu HEX, 2542u, -1554, (9, E, E), 0x611u
#define PPUTLIMPL_UINT_TRAIT_0x9EDu HEX, 2541u, -1555, (9, E, D), 0x612u
#define PPUTLIMPL_UINT_TRAIT_0x9ECu HEX, 2540u, -1556, (9, E, C), 0x613u
#define PPUTLIMPL_UINT_TRAIT_0x9EBu HEX, 2539u, -1557, (9, E, B), 0x614u
#define PPUTLIMPL_UINT_TRAIT_0x9EAu HEX, 2538u, -1558, (9, E, A), 0x615u
#define PPUTLIMPL_UINT_TRAIT_0x9E9u HEX, 2537u, -1559, (9, E, 9), 0x616u
#define PPUTLIMPL_UINT_TRAIT_0x9E8u HEX, 2536u, -1560, (9, E, 8), 0x617u
#define PPUTLIMPL_UINT_TRAIT_0x9E7u HEX, 2535u, -1561, (9, E, 7), 0x618u
#define PPUTLIMPL_UINT_TRAIT_0x9E6u HEX, 2534u, -1562, (9, E, 6), 0x619u
#define PPUTLIMPL_UINT_TRAIT_0x9E5u HEX, 2533u, -1563, (9, E, 5), 0x61Au
#define PPUTLIMPL_UINT_TRAIT_0x9E4u HEX, 2532u, -1564, (9, E, 4), 0x61Bu
#define PPUTLIMPL_UINT_TRAIT_0x9E3u HEX, 2531u, -1565, (9, E, 3), 0x61Cu
#define PPUTLIMPL_UINT_TRAIT_0x9E2u HEX, 2530u, -1566, (9, E, 2), 0x61Du
#define PPUTLIMPL_UINT_TRAIT_0x9E1u HEX, 2529u, -1567, (9, E, 1), 0x61Eu
#define PPUTLIMPL_UINT_TRAIT_0x9E0u HEX, 2528u, -1568, (9, E, 0), 0x61Fu
#define PPUTLIMPL_UINT_TRAIT_0x9DFu HEX, 2527u, -1569, (9, D, F), 0x620u
#define PPUTLIMPL_UINT_TRAIT_0x9DEu HEX, 2526u, -1570, (9, D, E), 0x621u
#define PPUTLIMPL_UINT_TRAIT_0x9DDu HEX, 2525u, -1571, (9, D, D), 0x622u
#define PPUTLIMPL_UINT_TRAIT_0x9DCu HEX, 2524u, -1572, (9, D, C), 0x623u
#define PPUTLIMPL_UINT_TRAIT_0x9DBu HEX, 2523u, -1573, (9, D, B), 0x624u
#define PPUTLIMPL_UINT_TRAIT_0x9DAu HEX, 2522u, -1574, (9, D, A), 0x625u
#define PPUTLIMPL_UINT_TRAIT_0x9D9u HEX, 2521u, -1575, (9, D, 9), 0x626u
#define PPUTLIMPL_UINT_TRAIT_0x9D8u HEX, 2520u, -1576, (9, D, 8), 0x627u
#define PPUTLIMPL_UINT_TRAIT_0x9D7u HEX, 2519u, -1577, (9, D, 7), 0x628u
#define PPUTLIMPL_UINT_TRAIT_0x9D6u HEX, 2518u, -1578, (9, D, 6), 0x629u
#define PPUTLIMPL_UINT_TRAIT_0x9D5u HEX, 2517u, -1579, (9, D, 5), 0x62Au
#define PPUTLIMPL_UINT_TRAIT_0x9D4u HEX, 2516u, -1580, (9, D, 4), 0x62Bu
#define PPUTLIMPL_UINT_TRAIT_0x9D3u HEX, 2515u, -1581, (9, D, 3), 0x62Cu
#define PPUTLIMPL_UINT_TRAIT_0x9D2u HEX, 2514u, -1582, (9, D, 2), 0x62Du
#define PPUTLIMPL_UINT_TRAIT_0x9D1u HEX, 2513u, -1583, (9, D, 1), 0x62Eu
#define PPUTLIMPL_UINT_TRAIT_0x9D0u HEX, 2512u, -1584, (9, D, 0), 0x62Fu
#define PPUTLIMPL_UINT_TRAIT_0x9CFu HEX, 2511u, -1585, (9, C, F), 0x630u
#define PPUTLIMPL_UINT_TRAIT_0x9CEu HEX, 2510u, -1586, (9, C, E), 0x631u
#define PPUTLIMPL_UINT_TRAIT_0x9CDu HEX, 2509u, -1587, (9, C, D), 0x632u
#define PPUTLIMPL_UINT_TRAIT_0x9CCu HEX, 2508u, -1588, (9, C, C), 0x633u
#define PPUTLIMPL_UINT_TRAIT_0x9CBu HEX, 2507u, -1589, (9, C, B), 0x634u
#define PPUTLIMPL_UINT_TRAIT_0x9CAu HEX, 2506u, -1590, (9, C, A), 0x635u
#define PPUTLIMPL_UINT_TRAIT_0x9C9u HEX, 2505u, -1591, (9, C, 9), 0x636u
#define PPUTLIMPL_UINT_TRAIT_0x9C8u HEX, 2504u, -1592, (9, C, 8), 0x637u
#define PPUTLIMPL_UINT_TRAIT_0x9C7u HEX, 2503u, -1593, (9, C, 7), 0x638u
#define PPUTLIMPL_UINT_TRAIT_0x9C6u HEX, 2502u, -1594, (9, C, 6), 0x639u
#define PPUTLIMPL_UINT_TRAIT_0x9C5u HEX, 2501u, -1595, (9, C, 5), 0x63Au
#define PPUTLIMPL_UINT_TRAIT_0x9C4u HEX, 2500u, -1596, (9, C, 4), 0x63Bu
#define PPUTLIMPL_UINT_TRAIT_0x9C3u HEX, 2499u, -1597, (9, C, 3), 0x63Cu
#define PPUTLIMPL_UINT_TRAIT_0x9C2u HEX, 2498u, -1598, (9, C, 2), 0x63Du
#define PPUTLIMPL_UINT_TRAIT_0x9C1u HEX, 2497u, -1599, (9, C, 1), 0x63Eu
#define PPUTLIMPL_UINT_TRAIT_0x9C0u HEX, 2496u, -1600, (9, C, 0), 0x63Fu
#define PPUTLIMPL_UINT_TRAIT_0x9BFu HEX, 2495u, -1601, (9, B, F), 0x640u
#define PPUTLIMPL_UINT_TRAIT_0x9BEu HEX, 2494u, -1602, (9, B, E), 0x641u
#define PPUTLIMPL_UINT_TRAIT_0x9BDu HEX, 2493u, -1603, (9, B, D), 0x642u
#define PPUTLIMPL_UINT_TRAIT_0x9BCu HEX, 2492u, -1604, (9, B, C), 0x643u
#define PPUTLIMPL_UINT_TRAIT_0x9BBu HEX, 2491u, -1605, (9, B, B), 0x644u
#define PPUTLIMPL_UINT_TRAIT_0x9BAu HEX, 2490u, -1606, (9, B, A), 0x645u
#define PPUTLIMPL_UINT_TRAIT_0x9B9u HEX, 2489u, -1607, (9, B, 9), 0x646u
#define PPUTLIMPL_UINT_TRAIT_0x9B8u HEX, 2488u, -1608, (9, B, 8), 0x647u
#define PPUTLIMPL_UINT_TRAIT_0x9B7u HEX, 2487u, -1609, (9, B, 7), 0x648u
#define PPUTLIMPL_UINT_TRAIT_0x9B6u HEX, 2486u, -1610, (9, B, 6), 0x649u
#define PPUTLIMPL_UINT_TRAIT_0x9B5u HEX, 2485u, -1611, (9, B, 5), 0x64Au
#define PPUTLIMPL_UINT_TRAIT_0x9B4u HEX, 2484u, -1612, (9, B, 4), 0x64Bu
#define PPUTLIMPL_UINT_TRAIT_0x9B3u HEX, 2483u, -1613, (9, B, 3), 0x64Cu
#define PPUTLIMPL_UINT_TRAIT_0x9B2u HEX, 2482u, -1614, (9, B, 2), 0x64Du
#define PPUTLIMPL_UINT_TRAIT_0x9B1u HEX, 2481u, -1615, (9, B, 1), 0x64Eu
#define PPUTLIMPL_UINT_TRAIT_0x9B0u HEX, 2480u, -1616, (9, B, 0), 0x64Fu
#define PPUTLIMPL_UINT_TRAIT_0x9AFu HEX, 2479u, -1617, (9, A, F), 0x650u
#define PPUTLIMPL_UINT_TRAIT_0x9AEu HEX, 2478u, -1618, (9, A, E), 0x651u
#define PPUTLIMPL_UINT_TRAIT_0x9ADu HEX, 2477u, -1619, (9, A, D), 0x652u
#define PPUTLIMPL_UINT_TRAIT_0x9ACu HEX, 2476u, -1620, (9, A, C), 0x653u
#define PPUTLIMPL_UINT_TRAIT_0x9ABu HEX, 2475u, -1621, (9, A, B), 0x654u
#define PPUTLIMPL_UINT_TRAIT_0x9AAu HEX, 2474u, -1622, (9, A, A), 0x655u
#define PPUTLIMPL_UINT_TRAIT_0x9A9u HEX, 2473u, -1623, (9, A, 9), 0x656u
#define PPUTLIMPL_UINT_TRAIT_0x9A8u HEX, 2472u, -1624, (9, A, 8), 0x657u
#define PPUTLIMPL_UINT_TRAIT_0x9A7u HEX, 2471u, -1625, (9, A, 7), 0x658u
#define PPUTLIMPL_UINT_TRAIT_0x9A6u HEX, 2470u, -1626, (9, A, 6), 0x659u
#define PPUTLIMPL_UINT_TRAIT_0x9A5u HEX, 2469u, -1627, (9, A, 5), 0x65Au
#define PPUTLIMPL_UINT_TRAIT_0x9A4u HEX, 2468u, -1628, (9, A, 4), 0x65Bu
#define PPUTLIMPL_UINT_TRAIT_0x9A3u HEX, 2467u, -1629, (9, A, 3), 0x65Cu
#define PPUTLIMPL_UINT_TRAIT_0x9A2u HEX, 2466u, -1630, (9, A, 2), 0x65Du
#define PPUTLIMPL_UINT_TRAIT_0x9A1u HEX, 2465u, -1631, (9, A, 1), 0x65Eu
#define PPUTLIMPL_UINT_TRAIT_0x9A0u HEX, 2464u, -1632, (9, A, 0), 0x65Fu
#define PPUTLIMPL_UINT_TRAIT_0x99Fu HEX, 2463u, -1633, (9, 9, F), 0x660u
#define PPUTLIMPL_UINT_TRAIT_0x99Eu HEX, 2462u, -1634, (9, 9, E), 0x661u
#define PPUTLIMPL_UINT_TRAIT_0x99Du HEX, 2461u, -1635, (9, 9, D), 0x662u
#define PPUTLIMPL_UINT_TRAIT_0x99Cu HEX, 2460u, -1636, (9, 9, C), 0x663u
#define PPUTLIMPL_UINT_TRAIT_0x99Bu HEX, 2459u, -1637, (9, 9, B), 0x664u
#define PPUTLIMPL_UINT_TRAIT_0x99Au HEX, 2458u, -1638, (9, 9, A), 0x665u
#define PPUTLIMPL_UINT_TRAIT_0x999u HEX, 2457u, -1639, (9, 9, 9), 0x666u
#define PPUTLIMPL_UINT_TRAIT_0x998u HEX, 2456u, -1640, (9, 9, 8), 0x667u
#define PPUTLIMPL_UINT_TRAIT_0x997u HEX, 2455u, -1641, (9, 9, 7), 0x668u
#define PPUTLIMPL_UINT_TRAIT_0x996u HEX, 2454u, -1642, (9, 9, 6), 0x669u
#define PPUTLIMPL_UINT_TRAIT_0x995u HEX, 2453u, -1643, (9, 9, 5), 0x66Au
#define PPUTLIMPL_UINT_TRAIT_0x994u HEX, 2452u, -1644, (9, 9, 4), 0x66Bu
#define PPUTLIMPL_UINT_TRAIT_0x993u HEX, 2451u, -1645, (9, 9, 3), 0x66Cu
#define PPUTLIMPL_UINT_TRAIT_0x992u HEX, 2450u, -1646, (9, 9, 2), 0x66Du
#define PPUTLIMPL_UINT_TRAIT_0x991u HEX, 2449u, -1647, (9, 9, 1), 0x66Eu
#define PPUTLIMPL_UINT_TRAIT_0x990u HEX, 2448u, -1648, (9, 9, 0), 0x66Fu
#define PPUTLIMPL_UINT_TRAIT_0x98Fu HEX, 2447u, -1649, (9, 8, F), 0x670u
#define PPUTLIMPL_UINT_TRAIT_0x98Eu HEX, 2446u, -1650, (9, 8, E), 0x671u
#define PPUTLIMPL_UINT_TRAIT_0x98Du HEX, 2445u, -1651, (9, 8, D), 0x672u
#define PPUTLIMPL_UINT_TRAIT_0x98Cu HEX, 2444u, -1652, (9, 8, C), 0x673u
#define PPUTLIMPL_UINT_TRAIT_0x98Bu HEX, 2443u, -1653, (9, 8, B), 0x674u
#define PPUTLIMPL_UINT_TRAIT_0x98Au HEX, 2442u, -1654, (9, 8, A), 0x675u
#define PPUTLIMPL_UINT_TRAIT_0x989u HEX, 2441u, -1655, (9, 8, 9), 0x676u
#define PPUTLIMPL_UINT_TRAIT_0x988u HEX, 2440u, -1656, (9, 8, 8), 0x677u
#define PPUTLIMPL_UINT_TRAIT_0x987u HEX, 2439u, -1657, (9, 8, 7), 0x678u
#define PPUTLIMPL_UINT_TRAIT_0x986u HEX, 2438u, -1658, (9, 8, 6), 0x679u
#define PPUTLIMPL_UINT_TRAIT_0x985u HEX, 2437u, -1659, (9, 8, 5), 0x67Au
#define PPUTLIMPL_UINT_TRAIT_0x984u HEX, 2436u, -1660, (9, 8, 4), 0x67Bu
#define PPUTLIMPL_UINT_TRAIT_0x983u HEX, 2435u, -1661, (9, 8, 3), 0x67Cu
#define PPUTLIMPL_UINT_TRAIT_0x982u HEX, 2434u, -1662, (9, 8, 2), 0x67Du
#define PPUTLIMPL_UINT_TRAIT_0x981u HEX, 2433u, -1663, (9, 8, 1), 0x67Eu
#define PPUTLIMPL_UINT_TRAIT_0x980u HEX, 2432u, -1664, (9, 8, 0), 0x67Fu
#define PPUTLIMPL_UINT_TRAIT_0x97Fu HEX, 2431u, -1665, (9, 7, F), 0x680u
#define PPUTLIMPL_UINT_TRAIT_0x97Eu HEX, 2430u, -1666, (9, 7, E), 0x681u
#define PPUTLIMPL_UINT_TRAIT_0x97Du HEX, 2429u, -1667, (9, 7, D), 0x682u
#define PPUTLIMPL_UINT_TRAIT_0x97Cu HEX, 2428u, -1668, (9, 7, C), 0x683u
#define PPUTLIMPL_UINT_TRAIT_0x97Bu HEX, 2427u, -1669, (9, 7, B), 0x684u
#define PPUTLIMPL_UINT_TRAIT_0x97Au HEX, 2426u, -1670, (9, 7, A), 0x685u
#define PPUTLIMPL_UINT_TRAIT_0x979u HEX, 2425u, -1671, (9, 7, 9), 0x686u
#define PPUTLIMPL_UINT_TRAIT_0x978u HEX, 2424u, -1672, (9, 7, 8), 0x687u
#define PPUTLIMPL_UINT_TRAIT_0x977u HEX, 2423u, -1673, (9, 7, 7), 0x688u
#define PPUTLIMPL_UINT_TRAIT_0x976u HEX, 2422u, -1674, (9, 7, 6), 0x689u
#define PPUTLIMPL_UINT_TRAIT_0x975u HEX, 2421u, -1675, (9, 7, 5), 0x68Au
#define PPUTLIMPL_UINT_TRAIT_0x974u HEX, 2420u, -1676, (9, 7, 4), 0x68Bu
#define PPUTLIMPL_UINT_TRAIT_0x973u HEX, 2419u, -1677, (9, 7, 3), 0x68Cu
#define PPUTLIMPL_UINT_TRAIT_0x972u HEX, 2418u, -1678, (9, 7, 2), 0x68Du
#define PPUTLIMPL_UINT_TRAIT_0x971u HEX, 2417u, -1679, (9, 7, 1), 0x68Eu
#define PPUTLIMPL_UINT_TRAIT_0x970u HEX, 2416u, -1680, (9, 7, 0), 0x68Fu
#define PPUTLIMPL_UINT_TRAIT_0x96Fu HEX, 2415u, -1681, (9, 6, F), 0x690u
#define PPUTLIMPL_UINT_TRAIT_0x96Eu HEX, 2414u, -1682, (9, 6, E), 0x691u
#define PPUTLIMPL_UINT_TRAIT_0x96Du HEX, 2413u, -1683, (9, 6, D), 0x692u
#define PPUTLIMPL_UINT_TRAIT_0x96Cu HEX, 2412u, -1684, (9, 6, C), 0x693u
#define PPUTLIMPL_UINT_TRAIT_0x96Bu HEX, 2411u, -1685, (9, 6, B), 0x694u
#define PPUTLIMPL_UINT_TRAIT_0x96Au HEX, 2410u, -1686, (9, 6, A), 0x695u
#define PPUTLIMPL_UINT_TRAIT_0x969u HEX, 2409u, -1687, (9, 6, 9), 0x696u
#define PPUTLIMPL_UINT_TRAIT_0x968u HEX, 2408u, -1688, (9, 6, 8), 0x697u
#define PPUTLIMPL_UINT_TRAIT_0x967u HEX, 2407u, -1689, (9, 6, 7), 0x698u
#define PPUTLIMPL_UINT_TRAIT_0x966u HEX, 2406u, -1690, (9, 6, 6), 0x699u
#define PPUTLIMPL_UINT_TRAIT_0x965u HEX, 2405u, -1691, (9, 6, 5), 0x69Au
#define PPUTLIMPL_UINT_TRAIT_0x964u HEX, 2404u, -1692, (9, 6, 4), 0x69Bu
#define PPUTLIMPL_UINT_TRAIT_0x963u HEX, 2403u, -1693, (9, 6, 3), 0x69Cu
#define PPUTLIMPL_UINT_TRAIT_0x962u HEX, 2402u, -1694, (9, 6, 2), 0x69Du
#define PPUTLIMPL_UINT_TRAIT_0x961u HEX, 2401u, -1695, (9, 6, 1), 0x69Eu
#define PPUTLIMPL_UINT_TRAIT_0x960u HEX, 2400u, -1696, (9, 6, 0), 0x69Fu
#define PPUTLIMPL_UINT_TRAIT_0x95Fu HEX, 2399u, -1697, (9, 5, F), 0x6A0u
#define PPUTLIMPL_UINT_TRAIT_0x95Eu HEX, 2398u, -1698, (9, 5, E), 0x6A1u
#define PPUTLIMPL_UINT_TRAIT_0x95Du HEX, 2397u, -1699, (9, 5, D), 0x6A2u
#define PPUTLIMPL_UINT_TRAIT_0x95Cu HEX, 2396u, -1700, (9, 5, C), 0x6A3u
#define PPUTLIMPL_UINT_TRAIT_0x95Bu HEX, 2395u, -1701, (9, 5, B), 0x6A4u
#define PPUTLIMPL_UINT_TRAIT_0x95Au HEX, 2394u, -1702, (9, 5, A), 0x6A5u
#define PPUTLIMPL_UINT_TRAIT_0x959u HEX, 2393u, -1703, (9, 5, 9), 0x6A6u
#define PPUTLIMPL_UINT_TRAIT_0x958u HEX, 2392u, -1704, (9, 5, 8), 0x6A7u
#define PPUTLIMPL_UINT_TRAIT_0x957u HEX, 2391u, -1705, (9, 5, 7), 0x6A8u
#define PPUTLIMPL_UINT_TRAIT_0x956u HEX, 2390u, -1706, (9, 5, 6), 0x6A9u
#define PPUTLIMPL_UINT_TRAIT_0x955u HEX, 2389u, -1707, (9, 5, 5), 0x6AAu
#define PPUTLIMPL_UINT_TRAIT_0x954u HEX, 2388u, -1708, (9, 5, 4), 0x6ABu
#define PPUTLIMPL_UINT_TRAIT_0x953u HEX, 2387u, -1709, (9, 5, 3), 0x6ACu
#define PPUTLIMPL_UINT_TRAIT_0x952u HEX, 2386u, -1710, (9, 5, 2), 0x6ADu
#define PPUTLIMPL_UINT_TRAIT_0x951u HEX, 2385u, -1711, (9, 5, 1), 0x6AEu
#define PPUTLIMPL_UINT_TRAIT_0x950u HEX, 2384u, -1712, (9, 5, 0), 0x6AFu
#define PPUTLIMPL_UINT_TRAIT_0x94Fu HEX, 2383u, -1713, (9, 4, F), 0x6B0u
#define PPUTLIMPL_UINT_TRAIT_0x94Eu HEX, 2382u, -1714, (9, 4, E), 0x6B1u
#define PPUTLIMPL_UINT_TRAIT_0x94Du HEX, 2381u, -1715, (9, 4, D), 0x6B2u
#define PPUTLIMPL_UINT_TRAIT_0x94Cu HEX, 2380u, -1716, (9, 4, C), 0x6B3u
#define PPUTLIMPL_UINT_TRAIT_0x94Bu HEX, 2379u, -1717, (9, 4, B), 0x6B4u
#define PPUTLIMPL_UINT_TRAIT_0x94Au HEX, 2378u, -1718, (9, 4, A), 0x6B5u
#define PPUTLIMPL_UINT_TRAIT_0x949u HEX, 2377u, -1719, (9, 4, 9), 0x6B6u
#define PPUTLIMPL_UINT_TRAIT_0x948u HEX, 2376u, -1720, (9, 4, 8), 0x6B7u
#define PPUTLIMPL_UINT_TRAIT_0x947u HEX, 2375u, -1721, (9, 4, 7), 0x6B8u
#define PPUTLIMPL_UINT_TRAIT_0x946u HEX, 2374u, -1722, (9, 4, 6), 0x6B9u
#define PPUTLIMPL_UINT_TRAIT_0x945u HEX, 2373u, -1723, (9, 4, 5), 0x6BAu
#define PPUTLIMPL_UINT_TRAIT_0x944u HEX, 2372u, -1724, (9, 4, 4), 0x6BBu
#define PPUTLIMPL_UINT_TRAIT_0x943u HEX, 2371u, -1725, (9, 4, 3), 0x6BCu
#define PPUTLIMPL_UINT_TRAIT_0x942u HEX, 2370u, -1726, (9, 4, 2), 0x6BDu
#define PPUTLIMPL_UINT_TRAIT_0x941u HEX, 2369u, -1727, (9, 4, 1), 0x6BEu
#define PPUTLIMPL_UINT_TRAIT_0x940u HEX, 2368u, -1728, (9, 4, 0), 0x6BFu
#define PPUTLIMPL_UINT_TRAIT_0x93Fu HEX, 2367u, -1729, (9, 3, F), 0x6C0u
#define PPUTLIMPL_UINT_TRAIT_0x93Eu HEX, 2366u, -1730, (9, 3, E), 0x6C1u
#define PPUTLIMPL_UINT_TRAIT_0x93Du HEX, 2365u, -1731, (9, 3, D), 0x6C2u
#define PPUTLIMPL_UINT_TRAIT_0x93Cu HEX, 2364u, -1732, (9, 3, C), 0x6C3u
#define PPUTLIMPL_UINT_TRAIT_0x93Bu HEX, 2363u, -1733, (9, 3, B), 0x6C4u
#define PPUTLIMPL_UINT_TRAIT_0x93Au HEX, 2362u, -1734, (9, 3, A), 0x6C5u
#define PPUTLIMPL_UINT_TRAIT_0x939u HEX, 2361u, -1735, (9, 3, 9), 0x6C6u
#define PPUTLIMPL_UINT_TRAIT_0x938u HEX, 2360u, -1736, (9, 3, 8), 0x6C7u
#define PPUTLIMPL_UINT_TRAIT_0x937u HEX, 2359u, -1737, (9, 3, 7), 0x6C8u
#define PPUTLIMPL_UINT_TRAIT_0x936u HEX, 2358u, -1738, (9, 3, 6), 0x6C9u
#define PPUTLIMPL_UINT_TRAIT_0x935u HEX, 2357u, -1739, (9, 3, 5), 0x6CAu
#define PPUTLIMPL_UINT_TRAIT_0x934u HEX, 2356u, -1740, (9, 3, 4), 0x6CBu
#define PPUTLIMPL_UINT_TRAIT_0x933u HEX, 2355u, -1741, (9, 3, 3), 0x6CCu
#define PPUTLIMPL_UINT_TRAIT_0x932u HEX, 2354u, -1742, (9, 3, 2), 0x6CDu
#define PPUTLIMPL_UINT_TRAIT_0x931u HEX, 2353u, -1743, (9, 3, 1), 0x6CEu
#define PPUTLIMPL_UINT_TRAIT_0x930u HEX, 2352u, -1744, (9, 3, 0), 0x6CFu
#define PPUTLIMPL_UINT_TRAIT_0x92Fu HEX, 2351u, -1745, (9, 2, F), 0x6D0u
#define PPUTLIMPL_UINT_TRAIT_0x92Eu HEX, 2350u, -1746, (9, 2, E), 0x6D1u
#define PPUTLIMPL_UINT_TRAIT_0x92Du HEX, 2349u, -1747, (9, 2, D), 0x6D2u
#define PPUTLIMPL_UINT_TRAIT_0x92Cu HEX, 2348u, -1748, (9, 2, C), 0x6D3u
#define PPUTLIMPL_UINT_TRAIT_0x92Bu HEX, 2347u, -1749, (9, 2, B), 0x6D4u
#define PPUTLIMPL_UINT_TRAIT_0x92Au HEX, 2346u, -1750, (9, 2, A), 0x6D5u
#define PPUTLIMPL_UINT_TRAIT_0x929u HEX, 2345u, -1751, (9, 2, 9), 0x6D6u
#define PPUTLIMPL_UINT_TRAIT_0x928u HEX, 2344u, -1752, (9, 2, 8), 0x6D7u
#define PPUTLIMPL_UINT_TRAIT_0x927u HEX, 2343u, -1753, (9, 2, 7), 0x6D8u
#define PPUTLIMPL_UINT_TRAIT_0x926u HEX, 2342u, -1754, (9, 2, 6), 0x6D9u
#define PPUTLIMPL_UINT_TRAIT_0x925u HEX, 2341u, -1755, (9, 2, 5), 0x6DAu
#define PPUTLIMPL_UINT_TRAIT_0x924u HEX, 2340u, -1756, (9, 2, 4), 0x6DBu
#define PPUTLIMPL_UINT_TRAIT_0x923u HEX, 2339u, -1757, (9, 2, 3), 0x6DCu
#define PPUTLIMPL_UINT_TRAIT_0x922u HEX, 2338u, -1758, (9, 2, 2), 0x6DDu
#define PPUTLIMPL_UINT_TRAIT_0x921u HEX, 2337u, -1759, (9, 2, 1), 0x6DEu
#define PPUTLIMPL_UINT_TRAIT_0x920u HEX, 2336u, -1760, (9, 2, 0), 0x6DFu
#define PPUTLIMPL_UINT_TRAIT_0x91Fu HEX, 2335u, -1761, (9, 1, F), 0x6E0u
#define PPUTLIMPL_UINT_TRAIT_0x91Eu HEX, 2334u, -1762, (9, 1, E), 0x6E1u
#define PPUTLIMPL_UINT_TRAIT_0x91Du HEX, 2333u, -1763, (9, 1, D), 0x6E2u
#define PPUTLIMPL_UINT_TRAIT_0x91Cu HEX, 2332u, -1764, (9, 1, C), 0x6E3u
#define PPUTLIMPL_UINT_TRAIT_0x91Bu HEX, 2331u, -1765, (9, 1, B), 0x6E4u
#define PPUTLIMPL_UINT_TRAIT_0x91Au HEX, 2330u, -1766, (9, 1, A), 0x6E5u
#define PPUTLIMPL_UINT_TRAIT_0x919u HEX, 2329u, -1767, (9, 1, 9), 0x6E6u
#define PPUTLIMPL_UINT_TRAIT_0x918u HEX, 2328u, -1768, (9, 1, 8), 0x6E7u
#define PPUTLIMPL_UINT_TRAIT_0x917u HEX, 2327u, -1769, (9, 1, 7), 0x6E8u
#define PPUTLIMPL_UINT_TRAIT_0x916u HEX, 2326u, -1770, (9, 1, 6), 0x6E9u
#define PPUTLIMPL_UINT_TRAIT_0x915u HEX, 2325u, -1771, (9, 1, 5), 0x6EAu
#define PPUTLIMPL_UINT_TRAIT_0x914u HEX, 2324u, -1772, (9, 1, 4), 0x6EBu
#define PPUTLIMPL_UINT_TRAIT_0x913u HEX, 2323u, -1773, (9, 1, 3), 0x6ECu
#define PPUTLIMPL_UINT_TRAIT_0x912u HEX, 2322u, -1774, (9, 1, 2), 0x6EDu
#define PPUTLIMPL_UINT_TRAIT_0x911u HEX, 2321u, -1775, (9, 1, 1), 0x6EEu
#define PPUTLIMPL_UINT_TRAIT_0x910u HEX, 2320u, -1776, (9, 1, 0), 0x6EFu
#define PPUTLIMPL_UINT_TRAIT_0x90Fu HEX, 2319u, -1777, (9, 0, F), 0x6F0u
#define PPUTLIMPL_UINT_TRAIT_0x90Eu HEX, 2318u, -1778, (9, 0, E), 0x6F1u
#define PPUTLIMPL_UINT_TRAIT_0x90Du HEX, 2317u, -1779, (9, 0, D), 0x6F2u
#define PPUTLIMPL_UINT_TRAIT_0x90Cu HEX, 2316u, -1780, (9, 0, C), 0x6F3u
#define PPUTLIMPL_UINT_TRAIT_0x90Bu HEX, 2315u, -1781, (9, 0, B), 0x6F4u
#define PPUTLIMPL_UINT_TRAIT_0x90Au HEX, 2314u, -1782, (9, 0, A), 0x6F5u
#define PPUTLIMPL_UINT_TRAIT_0x909u HEX, 2313u, -1783, (9, 0, 9), 0x6F6u
#define PPUTLIMPL_UINT_TRAIT_0x908u HEX, 2312u, -1784, (9, 0, 8), 0x6F7u
#define PPUTLIMPL_UINT_TRAIT_0x907u HEX, 2311u, -1785, (9, 0, 7), 0x6F8u
#define PPUTLIMPL_UINT_TRAIT_0x906u HEX, 2310u, -1786, (9, 0, 6), 0x6F9u
#define PPUTLIMPL_UINT_TRAIT_0x905u HEX, 2309u, -1787, (9, 0, 5), 0x6FAu
#define PPUTLIMPL_UINT_TRAIT_0x904u HEX, 2308u, -1788, (9, 0, 4), 0x6FBu
#define PPUTLIMPL_UINT_TRAIT_0x903u HEX, 2307u, -1789, (9, 0, 3), 0x6FCu
#define PPUTLIMPL_UINT_TRAIT_0x902u HEX, 2306u, -1790, (9, 0, 2), 0x6FDu
#define PPUTLIMPL_UINT_TRAIT_0x901u HEX, 2305u, -1791, (9, 0, 1), 0x6FEu
#define PPUTLIMPL_UINT_TRAIT_0x900u HEX, 2304u, -1792, (9, 0, 0), 0x6FFu
#define PPUTLIMPL_UINT_TRAIT_0x8FFu HEX, 2303u, -1793, (8, F, F), 0x700u
#define PPUTLIMPL_UINT_TRAIT_0x8FEu HEX, 2302u, -1794, (8, F, E), 0x701u
#define PPUTLIMPL_UINT_TRAIT_0x8FDu HEX, 2301u, -1795, (8, F, D), 0x702u
#define PPUTLIMPL_UINT_TRAIT_0x8FCu HEX, 2300u, -1796, (8, F, C), 0x703u
#define PPUTLIMPL_UINT_TRAIT_0x8FBu HEX, 2299u, -1797, (8, F, B), 0x704u
#define PPUTLIMPL_UINT_TRAIT_0x8FAu HEX, 2298u, -1798, (8, F, A), 0x705u
#define PPUTLIMPL_UINT_TRAIT_0x8F9u HEX, 2297u, -1799, (8, F, 9), 0x706u
#define PPUTLIMPL_UINT_TRAIT_0x8F8u HEX, 2296u, -1800, (8, F, 8), 0x707u
#define PPUTLIMPL_UINT_TRAIT_0x8F7u HEX, 2295u, -1801, (8, F, 7), 0x708u
#define PPUTLIMPL_UINT_TRAIT_0x8F6u HEX, 2294u, -1802, (8, F, 6), 0x709u
#define PPUTLIMPL_UINT_TRAIT_0x8F5u HEX, 2293u, -1803, (8, F, 5), 0x70Au
#define PPUTLIMPL_UINT_TRAIT_0x8F4u HEX, 2292u, -1804, (8, F, 4), 0x70Bu
#define PPUTLIMPL_UINT_TRAIT_0x8F3u HEX, 2291u, -1805, (8, F, 3), 0x70Cu
#define PPUTLIMPL_UINT_TRAIT_0x8F2u HEX, 2290u, -1806, (8, F, 2), 0x70Du
#define PPUTLIMPL_UINT_TRAIT_0x8F1u HEX, 2289u, -1807, (8, F, 1), 0x70Eu
#define PPUTLIMPL_UINT_TRAIT_0x8F0u HEX, 2288u, -1808, (8, F, 0), 0x70Fu
#define PPUTLIMPL_UINT_TRAIT_0x8EFu HEX, 2287u, -1809, (8, E, F), 0x710u
#define PPUTLIMPL_UINT_TRAIT_0x8EEu HEX, 2286u, -1810, (8, E, E), 0x711u
#define PPUTLIMPL_UINT_TRAIT_0x8EDu HEX, 2285u, -1811, (8, E, D), 0x712u
#define PPUTLIMPL_UINT_TRAIT_0x8ECu HEX, 2284u, -1812, (8, E, C), 0x713u
#define PPUTLIMPL_UINT_TRAIT_0x8EBu HEX, 2283u, -1813, (8, E, B), 0x714u
#define PPUTLIMPL_UINT_TRAIT_0x8EAu HEX, 2282u, -1814, (8, E, A), 0x715u
#define PPUTLIMPL_UINT_TRAIT_0x8E9u HEX, 2281u, -1815, (8, E, 9), 0x716u
#define PPUTLIMPL_UINT_TRAIT_0x8E8u HEX, 2280u, -1816, (8, E, 8), 0x717u
#define PPUTLIMPL_UINT_TRAIT_0x8E7u HEX, 2279u, -1817, (8, E, 7), 0x718u
#define PPUTLIMPL_UINT_TRAIT_0x8E6u HEX, 2278u, -1818, (8, E, 6), 0x719u
#define PPUTLIMPL_UINT_TRAIT_0x8E5u HEX, 2277u, -1819, (8, E, 5), 0x71Au
#define PPUTLIMPL_UINT_TRAIT_0x8E4u HEX, 2276u, -1820, (8, E, 4), 0x71Bu
#define PPUTLIMPL_UINT_TRAIT_0x8E3u HEX, 2275u, -1821, (8, E, 3), 0x71Cu
#define PPUTLIMPL_UINT_TRAIT_0x8E2u HEX, 2274u, -1822, (8, E, 2), 0x71Du
#define PPUTLIMPL_UINT_TRAIT_0x8E1u HEX, 2273u, -1823, (8, E, 1), 0x71Eu
#define PPUTLIMPL_UINT_TRAIT_0x8E0u HEX, 2272u, -1824, (8, E, 0), 0x71Fu
#define PPUTLIMPL_UINT_TRAIT_0x8DFu HEX, 2271u, -1825, (8, D, F), 0x720u
#define PPUTLIMPL_UINT_TRAIT_0x8DEu HEX, 2270u, -1826, (8, D, E), 0x721u
#define PPUTLIMPL_UINT_TRAIT_0x8DDu HEX, 2269u, -1827, (8, D, D), 0x722u
#define PPUTLIMPL_UINT_TRAIT_0x8DCu HEX, 2268u, -1828, (8, D, C), 0x723u
#define PPUTLIMPL_UINT_TRAIT_0x8DBu HEX, 2267u, -1829, (8, D, B), 0x724u
#define PPUTLIMPL_UINT_TRAIT_0x8DAu HEX, 2266u, -1830, (8, D, A), 0x725u
#define PPUTLIMPL_UINT_TRAIT_0x8D9u HEX, 2265u, -1831, (8, D, 9), 0x726u
#define PPUTLIMPL_UINT_TRAIT_0x8D8u HEX, 2264u, -1832, (8, D, 8), 0x727u
#define PPUTLIMPL_UINT_TRAIT_0x8D7u HEX, 2263u, -1833, (8, D, 7), 0x728u
#define PPUTLIMPL_UINT_TRAIT_0x8D6u HEX, 2262u, -1834, (8, D, 6), 0x729u
#define PPUTLIMPL_UINT_TRAIT_0x8D5u HEX, 2261u, -1835, (8, D, 5), 0x72Au
#define PPUTLIMPL_UINT_TRAIT_0x8D4u HEX, 2260u, -1836, (8, D, 4), 0x72Bu
#define PPUTLIMPL_UINT_TRAIT_0x8D3u HEX, 2259u, -1837, (8, D, 3), 0x72Cu
#define PPUTLIMPL_UINT_TRAIT_0x8D2u HEX, 2258u, -1838, (8, D, 2), 0x72Du
#define PPUTLIMPL_UINT_TRAIT_0x8D1u HEX, 2257u, -1839, (8, D, 1), 0x72Eu
#define PPUTLIMPL_UINT_TRAIT_0x8D0u HEX, 2256u, -1840, (8, D, 0), 0x72Fu
#define PPUTLIMPL_UINT_TRAIT_0x8CFu HEX, 2255u, -1841, (8, C, F), 0x730u
#define PPUTLIMPL_UINT_TRAIT_0x8CEu HEX, 2254u, -1842, (8, C, E), 0x731u
#define PPUTLIMPL_UINT_TRAIT_0x8CDu HEX, 2253u, -1843, (8, C, D), 0x732u
#define PPUTLIMPL_UINT_TRAIT_0x8CCu HEX, 2252u, -1844, (8, C, C), 0x733u
#define PPUTLIMPL_UINT_TRAIT_0x8CBu HEX, 2251u, -1845, (8, C, B), 0x734u
#define PPUTLIMPL_UINT_TRAIT_0x8CAu HEX, 2250u, -1846, (8, C, A), 0x735u
#define PPUTLIMPL_UINT_TRAIT_0x8C9u HEX, 2249u, -1847, (8, C, 9), 0x736u
#define PPUTLIMPL_UINT_TRAIT_0x8C8u HEX, 2248u, -1848, (8, C, 8), 0x737u
#define PPUTLIMPL_UINT_TRAIT_0x8C7u HEX, 2247u, -1849, (8, C, 7), 0x738u
#define PPUTLIMPL_UINT_TRAIT_0x8C6u HEX, 2246u, -1850, (8, C, 6), 0x739u
#define PPUTLIMPL_UINT_TRAIT_0x8C5u HEX, 2245u, -1851, (8, C, 5), 0x73Au
#define PPUTLIMPL_UINT_TRAIT_0x8C4u HEX, 2244u, -1852, (8, C, 4), 0x73Bu
#define PPUTLIMPL_UINT_TRAIT_0x8C3u HEX, 2243u, -1853, (8, C, 3), 0x73Cu
#define PPUTLIMPL_UINT_TRAIT_0x8C2u HEX, 2242u, -1854, (8, C, 2), 0x73Du
#define PPUTLIMPL_UINT_TRAIT_0x8C1u HEX, 2241u, -1855, (8, C, 1), 0x73Eu
#define PPUTLIMPL_UINT_TRAIT_0x8C0u HEX, 2240u, -1856, (8, C, 0), 0x73Fu
#define PPUTLIMPL_UINT_TRAIT_0x8BFu HEX, 2239u, -1857, (8, B, F), 0x740u
#define PPUTLIMPL_UINT_TRAIT_0x8BEu HEX, 2238u, -1858, (8, B, E), 0x741u
#define PPUTLIMPL_UINT_TRAIT_0x8BDu HEX, 2237u, -1859, (8, B, D), 0x742u
#define PPUTLIMPL_UINT_TRAIT_0x8BCu HEX, 2236u, -1860, (8, B, C), 0x743u
#define PPUTLIMPL_UINT_TRAIT_0x8BBu HEX, 2235u, -1861, (8, B, B), 0x744u
#define PPUTLIMPL_UINT_TRAIT_0x8BAu HEX, 2234u, -1862, (8, B, A), 0x745u
#define PPUTLIMPL_UINT_TRAIT_0x8B9u HEX, 2233u, -1863, (8, B, 9), 0x746u
#define PPUTLIMPL_UINT_TRAIT_0x8B8u HEX, 2232u, -1864, (8, B, 8), 0x747u
#define PPUTLIMPL_UINT_TRAIT_0x8B7u HEX, 2231u, -1865, (8, B, 7), 0x748u
#define PPUTLIMPL_UINT_TRAIT_0x8B6u HEX, 2230u, -1866, (8, B, 6), 0x749u
#define PPUTLIMPL_UINT_TRAIT_0x8B5u HEX, 2229u, -1867, (8, B, 5), 0x74Au
#define PPUTLIMPL_UINT_TRAIT_0x8B4u HEX, 2228u, -1868, (8, B, 4), 0x74Bu
#define PPUTLIMPL_UINT_TRAIT_0x8B3u HEX, 2227u, -1869, (8, B, 3), 0x74Cu
#define PPUTLIMPL_UINT_TRAIT_0x8B2u HEX, 2226u, -1870, (8, B, 2), 0x74Du
#define PPUTLIMPL_UINT_TRAIT_0x8B1u HEX, 2225u, -1871, (8, B, 1), 0x74Eu
#define PPUTLIMPL_UINT_TRAIT_0x8B0u HEX, 2224u, -1872, (8, B, 0), 0x74Fu
#define PPUTLIMPL_UINT_TRAIT_0x8AFu HEX, 2223u, -1873, (8, A, F), 0x750u
#define PPUTLIMPL_UINT_TRAIT_0x8AEu HEX, 2222u, -1874, (8, A, E), 0x751u
#define PPUTLIMPL_UINT_TRAIT_0x8ADu HEX, 2221u, -1875, (8, A, D), 0x752u
#define PPUTLIMPL_UINT_TRAIT_0x8ACu HEX, 2220u, -1876, (8, A, C), 0x753u
#define PPUTLIMPL_UINT_TRAIT_0x8ABu HEX, 2219u, -1877, (8, A, B), 0x754u
#define PPUTLIMPL_UINT_TRAIT_0x8AAu HEX, 2218u, -1878, (8, A, A), 0x755u
#define PPUTLIMPL_UINT_TRAIT_0x8A9u HEX, 2217u, -1879, (8, A, 9), 0x756u
#define PPUTLIMPL_UINT_TRAIT_0x8A8u HEX, 2216u, -1880, (8, A, 8), 0x757u
#define PPUTLIMPL_UINT_TRAIT_0x8A7u HEX, 2215u, -1881, (8, A, 7), 0x758u
#define PPUTLIMPL_UINT_TRAIT_0x8A6u HEX, 2214u, -1882, (8, A, 6), 0x759u
#define PPUTLIMPL_UINT_TRAIT_0x8A5u HEX, 2213u, -1883, (8, A, 5), 0x75Au
#define PPUTLIMPL_UINT_TRAIT_0x8A4u HEX, 2212u, -1884, (8, A, 4), 0x75Bu
#define PPUTLIMPL_UINT_TRAIT_0x8A3u HEX, 2211u, -1885, (8, A, 3), 0x75Cu
#define PPUTLIMPL_UINT_TRAIT_0x8A2u HEX, 2210u, -1886, (8, A, 2), 0x75Du
#define PPUTLIMPL_UINT_TRAIT_0x8A1u HEX, 2209u, -1887, (8, A, 1), 0x75Eu
#define PPUTLIMPL_UINT_TRAIT_0x8A0u HEX, 2208u, -1888, (8, A, 0), 0x75Fu
#define PPUTLIMPL_UINT_TRAIT_0x89Fu HEX, 2207u, -1889, (8, 9, F), 0x760u
#define PPUTLIMPL_UINT_TRAIT_0x89Eu HEX, 2206u, -1890, (8, 9, E), 0x761u
#define PPUTLIMPL_UINT_TRAIT_0x89Du HEX, 2205u, -1891, (8, 9, D), 0x762u
#define PPUTLIMPL_UINT_TRAIT_0x89Cu HEX, 2204u, -1892, (8, 9, C), 0x763u
#define PPUTLIMPL_UINT_TRAIT_0x89Bu HEX, 2203u, -1893, (8, 9, B), 0x764u
#define PPUTLIMPL_UINT_TRAIT_0x89Au HEX, 2202u, -1894, (8, 9, A), 0x765u
#define PPUTLIMPL_UINT_TRAIT_0x899u HEX, 2201u, -1895, (8, 9, 9), 0x766u
#define PPUTLIMPL_UINT_TRAIT_0x898u HEX, 2200u, -1896, (8, 9, 8), 0x767u
#define PPUTLIMPL_UINT_TRAIT_0x897u HEX, 2199u, -1897, (8, 9, 7), 0x768u
#define PPUTLIMPL_UINT_TRAIT_0x896u HEX, 2198u, -1898, (8, 9, 6), 0x769u
#define PPUTLIMPL_UINT_TRAIT_0x895u HEX, 2197u, -1899, (8, 9, 5), 0x76Au
#define PPUTLIMPL_UINT_TRAIT_0x894u HEX, 2196u, -1900, (8, 9, 4), 0x76Bu
#define PPUTLIMPL_UINT_TRAIT_0x893u HEX, 2195u, -1901, (8, 9, 3), 0x76Cu
#define PPUTLIMPL_UINT_TRAIT_0x892u HEX, 2194u, -1902, (8, 9, 2), 0x76Du
#define PPUTLIMPL_UINT_TRAIT_0x891u HEX, 2193u, -1903, (8, 9, 1), 0x76Eu
#define PPUTLIMPL_UINT_TRAIT_0x890u HEX, 2192u, -1904, (8, 9, 0), 0x76Fu
#define PPUTLIMPL_UINT_TRAIT_0x88Fu HEX, 2191u, -1905, (8, 8, F), 0x770u
#define PPUTLIMPL_UINT_TRAIT_0x88Eu HEX, 2190u, -1906, (8, 8, E), 0x771u
#define PPUTLIMPL_UINT_TRAIT_0x88Du HEX, 2189u, -1907, (8, 8, D), 0x772u
#define PPUTLIMPL_UINT_TRAIT_0x88Cu HEX, 2188u, -1908, (8, 8, C), 0x773u
#define PPUTLIMPL_UINT_TRAIT_0x88Bu HEX, 2187u, -1909, (8, 8, B), 0x774u
#define PPUTLIMPL_UINT_TRAIT_0x88Au HEX, 2186u, -1910, (8, 8, A), 0x775u
#define PPUTLIMPL_UINT_TRAIT_0x889u HEX, 2185u, -1911, (8, 8, 9), 0x776u
#define PPUTLIMPL_UINT_TRAIT_0x888u HEX, 2184u, -1912, (8, 8, 8), 0x777u
#define PPUTLIMPL_UINT_TRAIT_0x887u HEX, 2183u, -1913, (8, 8, 7), 0x778u
#define PPUTLIMPL_UINT_TRAIT_0x886u HEX, 2182u, -1914, (8, 8, 6), 0x779u
#define PPUTLIMPL_UINT_TRAIT_0x885u HEX, 2181u, -1915, (8, 8, 5), 0x77Au
#define PPUTLIMPL_UINT_TRAIT_0x884u HEX, 2180u, -1916, (8, 8, 4), 0x77Bu
#define PPUTLIMPL_UINT_TRAIT_0x883u HEX, 2179u, -1917, (8, 8, 3), 0x77Cu
#define PPUTLIMPL_UINT_TRAIT_0x882u HEX, 2178u, -1918, (8, 8, 2), 0x77Du
#define PPUTLIMPL_UINT_TRAIT_0x881u HEX, 2177u, -1919, (8, 8, 1), 0x77Eu
#define PPUTLIMPL_UINT_TRAIT_0x880u HEX, 2176u, -1920, (8, 8, 0), 0x77Fu
#define PPUTLIMPL_UINT_TRAIT_0x87Fu HEX, 2175u, -1921, (8, 7, F), 0x780u
#define PPUTLIMPL_UINT_TRAIT_0x87Eu HEX, 2174u, -1922, (8, 7, E), 0x781u
#define PPUTLIMPL_UINT_TRAIT_0x87Du HEX, 2173u, -1923, (8, 7, D), 0x782u
#define PPUTLIMPL_UINT_TRAIT_0x87Cu HEX, 2172u, -1924, (8, 7, C), 0x783u
#define PPUTLIMPL_UINT_TRAIT_0x87Bu HEX, 2171u, -1925, (8, 7, B), 0x784u
#define PPUTLIMPL_UINT_TRAIT_0x87Au HEX, 2170u, -1926, (8, 7, A), 0x785u
#define PPUTLIMPL_UINT_TRAIT_0x879u HEX, 2169u, -1927, (8, 7, 9), 0x786u
#define PPUTLIMPL_UINT_TRAIT_0x878u HEX, 2168u, -1928, (8, 7, 8), 0x787u
#define PPUTLIMPL_UINT_TRAIT_0x877u HEX, 2167u, -1929, (8, 7, 7), 0x788u
#define PPUTLIMPL_UINT_TRAIT_0x876u HEX, 2166u, -1930, (8, 7, 6), 0x789u
#define PPUTLIMPL_UINT_TRAIT_0x875u HEX, 2165u, -1931, (8, 7, 5), 0x78Au
#define PPUTLIMPL_UINT_TRAIT_0x874u HEX, 2164u, -1932, (8, 7, 4), 0x78Bu
#define PPUTLIMPL_UINT_TRAIT_0x873u HEX, 2163u, -1933, (8, 7, 3), 0x78Cu
#define PPUTLIMPL_UINT_TRAIT_0x872u HEX, 2162u, -1934, (8, 7, 2), 0x78Du
#define PPUTLIMPL_UINT_TRAIT_0x871u HEX, 2161u, -1935, (8, 7, 1), 0x78Eu
#define PPUTLIMPL_UINT_TRAIT_0x870u HEX, 2160u, -1936, (8, 7, 0), 0x78Fu
#define PPUTLIMPL_UINT_TRAIT_0x86Fu HEX, 2159u, -1937, (8, 6, F), 0x790u
#define PPUTLIMPL_UINT_TRAIT_0x86Eu HEX, 2158u, -1938, (8, 6, E), 0x791u
#define PPUTLIMPL_UINT_TRAIT_0x86Du HEX, 2157u, -1939, (8, 6, D), 0x792u
#define PPUTLIMPL_UINT_TRAIT_0x86Cu HEX, 2156u, -1940, (8, 6, C), 0x793u
#define PPUTLIMPL_UINT_TRAIT_0x86Bu HEX, 2155u, -1941, (8, 6, B), 0x794u
#define PPUTLIMPL_UINT_TRAIT_0x86Au HEX, 2154u, -1942, (8, 6, A), 0x795u
#define PPUTLIMPL_UINT_TRAIT_0x869u HEX, 2153u, -1943, (8, 6, 9), 0x796u
#define PPUTLIMPL_UINT_TRAIT_0x868u HEX, 2152u, -1944, (8, 6, 8), 0x797u
#define PPUTLIMPL_UINT_TRAIT_0x867u HEX, 2151u, -1945, (8, 6, 7), 0x798u
#define PPUTLIMPL_UINT_TRAIT_0x866u HEX, 2150u, -1946, (8, 6, 6), 0x799u
#define PPUTLIMPL_UINT_TRAIT_0x865u HEX, 2149u, -1947, (8, 6, 5), 0x79Au
#define PPUTLIMPL_UINT_TRAIT_0x864u HEX, 2148u, -1948, (8, 6, 4), 0x79Bu
#define PPUTLIMPL_UINT_TRAIT_0x863u HEX, 2147u, -1949, (8, 6, 3), 0x79Cu
#define PPUTLIMPL_UINT_TRAIT_0x862u HEX, 2146u, -1950, (8, 6, 2), 0x79Du
#define PPUTLIMPL_UINT_TRAIT_0x861u HEX, 2145u, -1951, (8, 6, 1), 0x79Eu
#define PPUTLIMPL_UINT_TRAIT_0x860u HEX, 2144u, -1952, (8, 6, 0), 0x79Fu
#define PPUTLIMPL_UINT_TRAIT_0x85Fu HEX, 2143u, -1953, (8, 5, F), 0x7A0u
#define PPUTLIMPL_UINT_TRAIT_0x85Eu HEX, 2142u, -1954, (8, 5, E), 0x7A1u
#define PPUTLIMPL_UINT_TRAIT_0x85Du HEX, 2141u, -1955, (8, 5, D), 0x7A2u
#define PPUTLIMPL_UINT_TRAIT_0x85Cu HEX, 2140u, -1956, (8, 5, C), 0x7A3u
#define PPUTLIMPL_UINT_TRAIT_0x85Bu HEX, 2139u, -1957, (8, 5, B), 0x7A4u
#define PPUTLIMPL_UINT_TRAIT_0x85Au HEX, 2138u, -1958, (8, 5, A), 0x7A5u
#define PPUTLIMPL_UINT_TRAIT_0x859u HEX, 2137u, -1959, (8, 5, 9), 0x7A6u
#define PPUTLIMPL_UINT_TRAIT_0x858u HEX, 2136u, -1960, (8, 5, 8), 0x7A7u
#define PPUTLIMPL_UINT_TRAIT_0x857u HEX, 2135u, -1961, (8, 5, 7), 0x7A8u
#define PPUTLIMPL_UINT_TRAIT_0x856u HEX, 2134u, -1962, (8, 5, 6), 0x7A9u
#define PPUTLIMPL_UINT_TRAIT_0x855u HEX, 2133u, -1963, (8, 5, 5), 0x7AAu
#define PPUTLIMPL_UINT_TRAIT_0x854u HEX, 2132u, -1964, (8, 5, 4), 0x7ABu
#define PPUTLIMPL_UINT_TRAIT_0x853u HEX, 2131u, -1965, (8, 5, 3), 0x7ACu
#define PPUTLIMPL_UINT_TRAIT_0x852u HEX, 2130u, -1966, (8, 5, 2), 0x7ADu
#define PPUTLIMPL_UINT_TRAIT_0x851u HEX, 2129u, -1967, (8, 5, 1), 0x7AEu
#define PPUTLIMPL_UINT_TRAIT_0x850u HEX, 2128u, -1968, (8, 5, 0), 0x7AFu
#define PPUTLIMPL_UINT_TRAIT_0x84Fu HEX, 2127u, -1969, (8, 4, F), 0x7B0u
#define PPUTLIMPL_UINT_TRAIT_0x84Eu HEX, 2126u, -1970, (8, 4, E), 0x7B1u
#define PPUTLIMPL_UINT_TRAIT_0x84Du HEX, 2125u, -1971, (8, 4, D), 0x7B2u
#define PPUTLIMPL_UINT_TRAIT_0x84Cu HEX, 2124u, -1972, (8, 4, C), 0x7B3u
#define PPUTLIMPL_UINT_TRAIT_0x84Bu HEX, 2123u, -1973, (8, 4, B), 0x7B4u
#define PPUTLIMPL_UINT_TRAIT_0x84Au HEX, 2122u, -1974, (8, 4, A), 0x7B5u
#define PPUTLIMPL_UINT_TRAIT_0x849u HEX, 2121u, -1975, (8, 4, 9), 0x7B6u
#define PPUTLIMPL_UINT_TRAIT_0x848u HEX, 2120u, -1976, (8, 4, 8), 0x7B7u
#define PPUTLIMPL_UINT_TRAIT_0x847u HEX, 2119u, -1977, (8, 4, 7), 0x7B8u
#define PPUTLIMPL_UINT_TRAIT_0x846u HEX, 2118u, -1978, (8, 4, 6), 0x7B9u
#define PPUTLIMPL_UINT_TRAIT_0x845u HEX, 2117u, -1979, (8, 4, 5), 0x7BAu
#define PPUTLIMPL_UINT_TRAIT_0x844u HEX, 2116u, -1980, (8, 4, 4), 0x7BBu
#define PPUTLIMPL_UINT_TRAIT_0x843u HEX, 2115u, -1981, (8, 4, 3), 0x7BCu
#define PPUTLIMPL_UINT_TRAIT_0x842u HEX, 2114u, -1982, (8, 4, 2), 0x7BDu
#define PPUTLIMPL_UINT_TRAIT_0x841u HEX, 2113u, -1983, (8, 4, 1), 0x7BEu
#define PPUTLIMPL_UINT_TRAIT_0x840u HEX, 2112u, -1984, (8, 4, 0), 0x7BFu
#define PPUTLIMPL_UINT_TRAIT_0x83Fu HEX, 2111u, -1985, (8, 3, F), 0x7C0u
#define PPUTLIMPL_UINT_TRAIT_0x83Eu HEX, 2110u, -1986, (8, 3, E), 0x7C1u
#define PPUTLIMPL_UINT_TRAIT_0x83Du HEX, 2109u, -1987, (8, 3, D), 0x7C2u
#define PPUTLIMPL_UINT_TRAIT_0x83Cu HEX, 2108u, -1988, (8, 3, C), 0x7C3u
#define PPUTLIMPL_UINT_TRAIT_0x83Bu HEX, 2107u, -1989, (8, 3, B), 0x7C4u
#define PPUTLIMPL_UINT_TRAIT_0x83Au HEX, 2106u, -1990, (8, 3, A), 0x7C5u
#define PPUTLIMPL_UINT_TRAIT_0x839u HEX, 2105u, -1991, (8, 3, 9), 0x7C6u
#define PPUTLIMPL_UINT_TRAIT_0x838u HEX, 2104u, -1992, (8, 3, 8), 0x7C7u
#define PPUTLIMPL_UINT_TRAIT_0x837u HEX, 2103u, -1993, (8, 3, 7), 0x7C8u
#define PPUTLIMPL_UINT_TRAIT_0x836u HEX, 2102u, -1994, (8, 3, 6), 0x7C9u
#define PPUTLIMPL_UINT_TRAIT_0x835u HEX, 2101u, -1995, (8, 3, 5), 0x7CAu
#define PPUTLIMPL_UINT_TRAIT_0x834u HEX, 2100u, -1996, (8, 3, 4), 0x7CBu
#define PPUTLIMPL_UINT_TRAIT_0x833u HEX, 2099u, -1997, (8, 3, 3), 0x7CCu
#define PPUTLIMPL_UINT_TRAIT_0x832u HEX, 2098u, -1998, (8, 3, 2), 0x7CDu
#define PPUTLIMPL_UINT_TRAIT_0x831u HEX, 2097u, -1999, (8, 3, 1), 0x7CEu
#define PPUTLIMPL_UINT_TRAIT_0x830u HEX, 2096u, -2000, (8, 3, 0), 0x7CFu
#define PPUTLIMPL_UINT_TRAIT_0x82Fu HEX, 2095u, -2001, (8, 2, F), 0x7D0u
#define PPUTLIMPL_UINT_TRAIT_0x82Eu HEX, 2094u, -2002, (8, 2, E), 0x7D1u
#define PPUTLIMPL_UINT_TRAIT_0x82Du HEX, 2093u, -2003, (8, 2, D), 0x7D2u
#define PPUTLIMPL_UINT_TRAIT_0x82Cu HEX, 2092u, -2004, (8, 2, C), 0x7D3u
#define PPUTLIMPL_UINT_TRAIT_0x82Bu HEX, 2091u, -2005, (8, 2, B), 0x7D4u
#define PPUTLIMPL_UINT_TRAIT_0x82Au HEX, 2090u, -2006, (8, 2, A), 0x7D5u
#define PPUTLIMPL_UINT_TRAIT_0x829u HEX, 2089u, -2007, (8, 2, 9), 0x7D6u
#define PPUTLIMPL_UINT_TRAIT_0x828u HEX, 2088u, -2008, (8, 2, 8), 0x7D7u
#define PPUTLIMPL_UINT_TRAIT_0x827u HEX, 2087u, -2009, (8, 2, 7), 0x7D8u
#define PPUTLIMPL_UINT_TRAIT_0x826u HEX, 2086u, -2010, (8, 2, 6), 0x7D9u
#define PPUTLIMPL_UINT_TRAIT_0x825u HEX, 2085u, -2011, (8, 2, 5), 0x7DAu
#define PPUTLIMPL_UINT_TRAIT_0x824u HEX, 2084u, -2012, (8, 2, 4), 0x7DBu
#define PPUTLIMPL_UINT_TRAIT_0x823u HEX, 2083u, -2013, (8, 2, 3), 0x7DCu
#define PPUTLIMPL_UINT_TRAIT_0x822u HEX, 2082u, -2014, (8, 2, 2), 0x7DDu
#define PPUTLIMPL_UINT_TRAIT_0x821u HEX, 2081u, -2015, (8, 2, 1), 0x7DEu
#define PPUTLIMPL_UINT_TRAIT_0x820u HEX, 2080u, -2016, (8, 2, 0), 0x7DFu
#define PPUTLIMPL_UINT_TRAIT_0x81Fu HEX, 2079u, -2017, (8, 1, F), 0x7E0u
#define PPUTLIMPL_UINT_TRAIT_0x81Eu HEX, 2078u, -2018, (8, 1, E), 0x7E1u
#define PPUTLIMPL_UINT_TRAIT_0x81Du HEX, 2077u, -2019, (8, 1, D), 0x7E2u
#define PPUTLIMPL_UINT_TRAIT_0x81Cu HEX, 2076u, -2020, (8, 1, C), 0x7E3u
#define PPUTLIMPL_UINT_TRAIT_0x81Bu HEX, 2075u, -2021, (8, 1, B), 0x7E4u
#define PPUTLIMPL_UINT_TRAIT_0x81Au HEX, 2074u, -2022, (8, 1, A), 0x7E5u
#define PPUTLIMPL_UINT_TRAIT_0x819u HEX, 2073u, -2023, (8, 1, 9), 0x7E6u
#define PPUTLIMPL_UINT_TRAIT_0x818u HEX, 2072u, -2024, (8, 1, 8), 0x7E7u
#define PPUTLIMPL_UINT_TRAIT_0x817u HEX, 2071u, -2025, (8, 1, 7), 0x7E8u
#define PPUTLIMPL_UINT_TRAIT_0x816u HEX, 2070u, -2026, (8, 1, 6), 0x7E9u
#define PPUTLIMPL_UINT_TRAIT_0x815u HEX, 2069u, -2027, (8, 1, 5), 0x7EAu
#define PPUTLIMPL_UINT_TRAIT_0x814u HEX, 2068u, -2028, (8, 1, 4), 0x7EBu
#define PPUTLIMPL_UINT_TRAIT_0x813u HEX, 2067u, -2029, (8, 1, 3), 0x7ECu
#define PPUTLIMPL_UINT_TRAIT_0x812u HEX, 2066u, -2030, (8, 1, 2), 0x7EDu
#define PPUTLIMPL_UINT_TRAIT_0x811u HEX, 2065u, -2031, (8, 1, 1), 0x7EEu
#define PPUTLIMPL_UINT_TRAIT_0x810u HEX, 2064u, -2032, (8, 1, 0), 0x7EFu
#define PPUTLIMPL_UINT_TRAIT_0x80Fu HEX, 2063u, -2033, (8, 0, F), 0x7F0u
#define PPUTLIMPL_UINT_TRAIT_0x80Eu HEX, 2062u, -2034, (8, 0, E), 0x7F1u
#define PPUTLIMPL_UINT_TRAIT_0x80Du HEX, 2061u, -2035, (8, 0, D), 0x7F2u
#define PPUTLIMPL_UINT_TRAIT_0x80Cu HEX, 2060u, -2036, (8, 0, C), 0x7F3u
#define PPUTLIMPL_UINT_TRAIT_0x80Bu HEX, 2059u, -2037, (8, 0, B), 0x7F4u
#define PPUTLIMPL_UINT_TRAIT_0x80Au HEX, 2058u, -2038, (8, 0, A), 0x7F5u
#define PPUTLIMPL_UINT_TRAIT_0x809u HEX, 2057u, -2039, (8, 0, 9), 0x7F6u
#define PPUTLIMPL_UINT_TRAIT_0x808u HEX, 2056u, -2040, (8, 0, 8), 0x7F7u
#define PPUTLIMPL_UINT_TRAIT_0x807u HEX, 2055u, -2041, (8, 0, 7), 0x7F8u
#define PPUTLIMPL_UINT_TRAIT_0x806u HEX, 2054u, -2042, (8, 0, 6), 0x7F9u
#define PPUTLIMPL_UINT_TRAIT_0x805u HEX, 2053u, -2043, (8, 0, 5), 0x7FAu
#define PPUTLIMPL_UINT_TRAIT_0x804u HEX, 2052u, -2044, (8, 0, 4), 0x7FBu
#define PPUTLIMPL_UINT_TRAIT_0x803u HEX, 2051u, -2045, (8, 0, 3), 0x7FCu
#define PPUTLIMPL_UINT_TRAIT_0x802u HEX, 2050u, -2046, (8, 0, 2), 0x7FDu
#define PPUTLIMPL_UINT_TRAIT_0x801u HEX, 2049u, -2047, (8, 0, 1), 0x7FEu
#define PPUTLIMPL_UINT_TRAIT_0x800u HEX, 2048u, -2048, (8, 0, 0), 0x7FFu
#define PPUTLIMPL_UINT_TRAIT_0x7FFu HEX, 2047u, 2047, (7, F, F), 0x800u
#define PPUTLIMPL_UINT_TRAIT_0x7FEu HEX, 2046u, 2046, (7, F, E), 0x801u
#define PPUTLIMPL_UINT_TRAIT_0x7FDu HEX, 2045u, 2045, (7, F, D), 0x802u
#define PPUTLIMPL_UINT_TRAIT_0x7FCu HEX, 2044u, 2044, (7, F, C), 0x803u
#define PPUTLIMPL_UINT_TRAIT_0x7FBu HEX, 2043u, 2043, (7, F, B), 0x804u
#define PPUTLIMPL_UINT_TRAIT_0x7FAu HEX, 2042u, 2042, (7, F, A), 0x805u
#define PPUTLIMPL_UINT_TRAIT_0x7F9u HEX, 2041u, 2041, (7, F, 9), 0x806u
#define PPUTLIMPL_UINT_TRAIT_0x7F8u HEX, 2040u, 2040, (7, F, 8), 0x807u
#define PPUTLIMPL_UINT_TRAIT_0x7F7u HEX, 2039u, 2039, (7, F, 7), 0x808u
#define PPUTLIMPL_UINT_TRAIT_0x7F6u HEX, 2038u, 2038, (7, F, 6), 0x809u
#define PPUTLIMPL_UINT_TRAIT_0x7F5u HEX, 2037u, 2037, (7, F, 5), 0x80Au
#define PPUTLIMPL_UINT_TRAIT_0x7F4u HEX, 2036u, 2036, (7, F, 4), 0x80Bu
#define PPUTLIMPL_UINT_TRAIT_0x7F3u HEX, 2035u, 2035, (7, F, 3), 0x80Cu
#define PPUTLIMPL_UINT_TRAIT_0x7F2u HEX, 2034u, 2034, (7, F, 2), 0x80Du
#define PPUTLIMPL_UINT_TRAIT_0x7F1u HEX, 2033u, 2033, (7, F, 1), 0x80Eu
#define PPUTLIMPL_UINT_TRAIT_0x7F0u HEX, 2032u, 2032, (7, F, 0), 0x80Fu
#define PPUTLIMPL_UINT_TRAIT_0x7EFu HEX, 2031u, 2031, (7, E, F), 0x810u
#define PPUTLIMPL_UINT_TRAIT_0x7EEu HEX, 2030u, 2030, (7, E, E), 0x811u
#define PPUTLIMPL_UINT_TRAIT_0x7EDu HEX, 2029u, 2029, (7, E, D), 0x812u
#define PPUTLIMPL_UINT_TRAIT_0x7ECu HEX, 2028u, 2028, (7, E, C), 0x813u
#define PPUTLIMPL_UINT_TRAIT_0x7EBu HEX, 2027u, 2027, (7, E, B), 0x814u
#define PPUTLIMPL_UINT_TRAIT_0x7EAu HEX, 2026u, 2026, (7, E, A), 0x815u
#define PPUTLIMPL_UINT_TRAIT_0x7E9u HEX, 2025u, 2025, (7, E, 9), 0x816u
#define PPUTLIMPL_UINT_TRAIT_0x7E8u HEX, 2024u, 2024, (7, E, 8), 0x817u
#define PPUTLIMPL_UINT_TRAIT_0x7E7u HEX, 2023u, 2023, (7, E, 7), 0x818u
#define PPUTLIMPL_UINT_TRAIT_0x7E6u HEX, 2022u, 2022, (7, E, 6), 0x819u
#define PPUTLIMPL_UINT_TRAIT_0x7E5u HEX, 2021u, 2021, (7, E, 5), 0x81Au
#define PPUTLIMPL_UINT_TRAIT_0x7E4u HEX, 2020u, 2020, (7, E, 4), 0x81Bu
#define PPUTLIMPL_UINT_TRAIT_0x7E3u HEX, 2019u, 2019, (7, E, 3), 0x81Cu
#define PPUTLIMPL_UINT_TRAIT_0x7E2u HEX, 2018u, 2018, (7, E, 2), 0x81Du
#define PPUTLIMPL_UINT_TRAIT_0x7E1u HEX, 2017u, 2017, (7, E, 1), 0x81Eu
#define PPUTLIMPL_UINT_TRAIT_0x7E0u HEX, 2016u, 2016, (7, E, 0), 0x81Fu
#define PPUTLIMPL_UINT_TRAIT_0x7DFu HEX, 2015u, 2015, (7, D, F), 0x820u
#define PPUTLIMPL_UINT_TRAIT_0x7DEu HEX, 2014u, 2014, (7, D, E), 0x821u
#define PPUTLIMPL_UINT_TRAIT_0x7DDu HEX, 2013u, 2013, (7, D, D), 0x822u
#define PPUTLIMPL_UINT_TRAIT_0x7DCu HEX, 2012u, 2012, (7, D, C), 0x823u
#define PPUTLIMPL_UINT_TRAIT_0x7DBu HEX, 2011u, 2011, (7, D, B), 0x824u
#define PPUTLIMPL_UINT_TRAIT_0x7DAu HEX, 2010u, 2010, (7, D, A), 0x825u
#define PPUTLIMPL_UINT_TRAIT_0x7D9u HEX, 2009u, 2009, (7, D, 9), 0x826u
#define PPUTLIMPL_UINT_TRAIT_0x7D8u HEX, 2008u, 2008, (7, D, 8), 0x827u
#define PPUTLIMPL_UINT_TRAIT_0x7D7u HEX, 2007u, 2007, (7, D, 7), 0x828u
#define PPUTLIMPL_UINT_TRAIT_0x7D6u HEX, 2006u, 2006, (7, D, 6), 0x829u
#define PPUTLIMPL_UINT_TRAIT_0x7D5u HEX, 2005u, 2005, (7, D, 5), 0x82Au
#define PPUTLIMPL_UINT_TRAIT_0x7D4u HEX, 2004u, 2004, (7, D, 4), 0x82Bu
#define PPUTLIMPL_UINT_TRAIT_0x7D3u HEX, 2003u, 2003, (7, D, 3), 0x82Cu
#define PPUTLIMPL_UINT_TRAIT_0x7D2u HEX, 2002u, 2002, (7, D, 2), 0x82Du
#define PPUTLIMPL_UINT_TRAIT_0x7D1u HEX, 2001u, 2001, (7, D, 1), 0x82Eu
#define PPUTLIMPL_UINT_TRAIT_0x7D0u HEX, 2000u, 2000, (7, D, 0), 0x82Fu
#define PPUTLIMPL_UINT_TRAIT_0x7CFu HEX, 1999u, 1999, (7, C, F), 0x830u
#define PPUTLIMPL_UINT_TRAIT_0x7CEu HEX, 1998u, 1998, (7, C, E), 0x831u
#define PPUTLIMPL_UINT_TRAIT_0x7CDu HEX, 1997u, 1997, (7, C, D), 0x832u
#define PPUTLIMPL_UINT_TRAIT_0x7CCu HEX, 1996u, 1996, (7, C, C), 0x833u
#define PPUTLIMPL_UINT_TRAIT_0x7CBu HEX, 1995u, 1995, (7, C, B), 0x834u
#define PPUTLIMPL_UINT_TRAIT_0x7CAu HEX, 1994u, 1994, (7, C, A), 0x835u
#define PPUTLIMPL_UINT_TRAIT_0x7C9u HEX, 1993u, 1993, (7, C, 9), 0x836u
#define PPUTLIMPL_UINT_TRAIT_0x7C8u HEX, 1992u, 1992, (7, C, 8), 0x837u
#define PPUTLIMPL_UINT_TRAIT_0x7C7u HEX, 1991u, 1991, (7, C, 7), 0x838u
#define PPUTLIMPL_UINT_TRAIT_0x7C6u HEX, 1990u, 1990, (7, C, 6), 0x839u
#define PPUTLIMPL_UINT_TRAIT_0x7C5u HEX, 1989u, 1989, (7, C, 5), 0x83Au
#define PPUTLIMPL_UINT_TRAIT_0x7C4u HEX, 1988u, 1988, (7, C, 4), 0x83Bu
#define PPUTLIMPL_UINT_TRAIT_0x7C3u HEX, 1987u, 1987, (7, C, 3), 0x83Cu
#define PPUTLIMPL_UINT_TRAIT_0x7C2u HEX, 1986u, 1986, (7, C, 2), 0x83Du
#define PPUTLIMPL_UINT_TRAIT_0x7C1u HEX, 1985u, 1985, (7, C, 1), 0x83Eu
#define PPUTLIMPL_UINT_TRAIT_0x7C0u HEX, 1984u, 1984, (7, C, 0), 0x83Fu
#define PPUTLIMPL_UINT_TRAIT_0x7BFu HEX, 1983u, 1983, (7, B, F), 0x840u
#define PPUTLIMPL_UINT_TRAIT_0x7BEu HEX, 1982u, 1982, (7, B, E), 0x841u
#define PPUTLIMPL_UINT_TRAIT_0x7BDu HEX, 1981u, 1981, (7, B, D), 0x842u
#define PPUTLIMPL_UINT_TRAIT_0x7BCu HEX, 1980u, 1980, (7, B, C), 0x843u
#define PPUTLIMPL_UINT_TRAIT_0x7BBu HEX, 1979u, 1979, (7, B, B), 0x844u
#define PPUTLIMPL_UINT_TRAIT_0x7BAu HEX, 1978u, 1978, (7, B, A), 0x845u
#define PPUTLIMPL_UINT_TRAIT_0x7B9u HEX, 1977u, 1977, (7, B, 9), 0x846u
#define PPUTLIMPL_UINT_TRAIT_0x7B8u HEX, 1976u, 1976, (7, B, 8), 0x847u
#define PPUTLIMPL_UINT_TRAIT_0x7B7u HEX, 1975u, 1975, (7, B, 7), 0x848u
#define PPUTLIMPL_UINT_TRAIT_0x7B6u HEX, 1974u, 1974, (7, B, 6), 0x849u
#define PPUTLIMPL_UINT_TRAIT_0x7B5u HEX, 1973u, 1973, (7, B, 5), 0x84Au
#define PPUTLIMPL_UINT_TRAIT_0x7B4u HEX, 1972u, 1972, (7, B, 4), 0x84Bu
#define PPUTLIMPL_UINT_TRAIT_0x7B3u HEX, 1971u, 1971, (7, B, 3), 0x84Cu
#define PPUTLIMPL_UINT_TRAIT_0x7B2u HEX, 1970u, 1970, (7, B, 2), 0x84Du
#define PPUTLIMPL_UINT_TRAIT_0x7B1u HEX, 1969u, 1969, (7, B, 1), 0x84Eu
#define PPUTLIMPL_UINT_TRAIT_0x7B0u HEX, 1968u, 1968, (7, B, 0), 0x84Fu
#define PPUTLIMPL_UINT_TRAIT_0x7AFu HEX, 1967u, 1967, (7, A, F), 0x850u
#define PPUTLIMPL_UINT_TRAIT_0x7AEu HEX, 1966u, 1966, (7, A, E), 0x851u
#define PPUTLIMPL_UINT_TRAIT_0x7ADu HEX, 1965u, 1965, (7, A, D), 0x852u
#define PPUTLIMPL_UINT_TRAIT_0x7ACu HEX, 1964u, 1964, (7, A, C), 0x853u
#define PPUTLIMPL_UINT_TRAIT_0x7ABu HEX, 1963u, 1963, (7, A, B), 0x854u
#define PPUTLIMPL_UINT_TRAIT_0x7AAu HEX, 1962u, 1962, (7, A, A), 0x855u
#define PPUTLIMPL_UINT_TRAIT_0x7A9u HEX, 1961u, 1961, (7, A, 9), 0x856u
#define PPUTLIMPL_UINT_TRAIT_0x7A8u HEX, 1960u, 1960, (7, A, 8), 0x857u
#define PPUTLIMPL_UINT_TRAIT_0x7A7u HEX, 1959u, 1959, (7, A, 7), 0x858u
#define PPUTLIMPL_UINT_TRAIT_0x7A6u HEX, 1958u, 1958, (7, A, 6), 0x859u
#define PPUTLIMPL_UINT_TRAIT_0x7A5u HEX, 1957u, 1957, (7, A, 5), 0x85Au
#define PPUTLIMPL_UINT_TRAIT_0x7A4u HEX, 1956u, 1956, (7, A, 4), 0x85Bu
#define PPUTLIMPL_UINT_TRAIT_0x7A3u HEX, 1955u, 1955, (7, A, 3), 0x85Cu
#define PPUTLIMPL_UINT_TRAIT_0x7A2u HEX, 1954u, 1954, (7, A, 2), 0x85Du
#define PPUTLIMPL_UINT_TRAIT_0x7A1u HEX, 1953u, 1953, (7, A, 1), 0x85Eu
#define PPUTLIMPL_UINT_TRAIT_0x7A0u HEX, 1952u, 1952, (7, A, 0), 0x85Fu
#define PPUTLIMPL_UINT_TRAIT_0x79Fu HEX, 1951u, 1951, (7, 9, F), 0x860u
#define PPUTLIMPL_UINT_TRAIT_0x79Eu HEX, 1950u, 1950, (7, 9, E), 0x861u
#define PPUTLIMPL_UINT_TRAIT_0x79Du HEX, 1949u, 1949, (7, 9, D), 0x862u
#define PPUTLIMPL_UINT_TRAIT_0x79Cu HEX, 1948u, 1948, (7, 9, C), 0x863u
#define PPUTLIMPL_UINT_TRAIT_0x79Bu HEX, 1947u, 1947, (7, 9, B), 0x864u
#define PPUTLIMPL_UINT_TRAIT_0x79Au HEX, 1946u, 1946, (7, 9, A), 0x865u
#define PPUTLIMPL_UINT_TRAIT_0x799u HEX, 1945u, 1945, (7, 9, 9), 0x866u
#define PPUTLIMPL_UINT_TRAIT_0x798u HEX, 1944u, 1944, (7, 9, 8), 0x867u
#define PPUTLIMPL_UINT_TRAIT_0x797u HEX, 1943u, 1943, (7, 9, 7), 0x868u
#define PPUTLIMPL_UINT_TRAIT_0x796u HEX, 1942u, 1942, (7, 9, 6), 0x869u
#define PPUTLIMPL_UINT_TRAIT_0x795u HEX, 1941u, 1941, (7, 9, 5), 0x86Au
#define PPUTLIMPL_UINT_TRAIT_0x794u HEX, 1940u, 1940, (7, 9, 4), 0x86Bu
#define PPUTLIMPL_UINT_TRAIT_0x793u HEX, 1939u, 1939, (7, 9, 3), 0x86Cu
#define PPUTLIMPL_UINT_TRAIT_0x792u HEX, 1938u, 1938, (7, 9, 2), 0x86Du
#define PPUTLIMPL_UINT_TRAIT_0x791u HEX, 1937u, 1937, (7, 9, 1), 0x86Eu
#define PPUTLIMPL_UINT_TRAIT_0x790u HEX, 1936u, 1936, (7, 9, 0), 0x86Fu
#define PPUTLIMPL_UINT_TRAIT_0x78Fu HEX, 1935u, 1935, (7, 8, F), 0x870u
#define PPUTLIMPL_UINT_TRAIT_0x78Eu HEX, 1934u, 1934, (7, 8, E), 0x871u
#define PPUTLIMPL_UINT_TRAIT_0x78Du HEX, 1933u, 1933, (7, 8, D), 0x872u
#define PPUTLIMPL_UINT_TRAIT_0x78Cu HEX, 1932u, 1932, (7, 8, C), 0x873u
#define PPUTLIMPL_UINT_TRAIT_0x78Bu HEX, 1931u, 1931, (7, 8, B), 0x874u
#define PPUTLIMPL_UINT_TRAIT_0x78Au HEX, 1930u, 1930, (7, 8, A), 0x875u
#define PPUTLIMPL_UINT_TRAIT_0x789u HEX, 1929u, 1929, (7, 8, 9), 0x876u
#define PPUTLIMPL_UINT_TRAIT_0x788u HEX, 1928u, 1928, (7, 8, 8), 0x877u
#define PPUTLIMPL_UINT_TRAIT_0x787u HEX, 1927u, 1927, (7, 8, 7), 0x878u
#define PPUTLIMPL_UINT_TRAIT_0x786u HEX, 1926u, 1926, (7, 8, 6), 0x879u
#define PPUTLIMPL_UINT_TRAIT_0x785u HEX, 1925u, 1925, (7, 8, 5), 0x87Au
#define PPUTLIMPL_UINT_TRAIT_0x784u HEX, 1924u, 1924, (7, 8, 4), 0x87Bu
#define PPUTLIMPL_UINT_TRAIT_0x783u HEX, 1923u, 1923, (7, 8, 3), 0x87Cu
#define PPUTLIMPL_UINT_TRAIT_0x782u HEX, 1922u, 1922, (7, 8, 2), 0x87Du
#define PPUTLIMPL_UINT_TRAIT_0x781u HEX, 1921u, 1921, (7, 8, 1), 0x87Eu
#define PPUTLIMPL_UINT_TRAIT_0x780u HEX, 1920u, 1920, (7, 8, 0), 0x87Fu
#define PPUTLIMPL_UINT_TRAIT_0x77Fu HEX, 1919u, 1919, (7, 7, F), 0x880u
#define PPUTLIMPL_UINT_TRAIT_0x77Eu HEX, 1918u, 1918, (7, 7, E), 0x881u
#define PPUTLIMPL_UINT_TRAIT_0x77Du HEX, 1917u, 1917, (7, 7, D), 0x882u
#define PPUTLIMPL_UINT_TRAIT_0x77Cu HEX, 1916u, 1916, (7, 7, C), 0x883u
#define PPUTLIMPL_UINT_TRAIT_0x77Bu HEX, 1915u, 1915, (7, 7, B), 0x884u
#define PPUTLIMPL_UINT_TRAIT_0x77Au HEX, 1914u, 1914, (7, 7, A), 0x885u
#define PPUTLIMPL_UINT_TRAIT_0x779u HEX, 1913u, 1913, (7, 7, 9), 0x886u
#define PPUTLIMPL_UINT_TRAIT_0x778u HEX, 1912u, 1912, (7, 7, 8), 0x887u
#define PPUTLIMPL_UINT_TRAIT_0x777u HEX, 1911u, 1911, (7, 7, 7), 0x888u
#define PPUTLIMPL_UINT_TRAIT_0x776u HEX, 1910u, 1910, (7, 7, 6), 0x889u
#define PPUTLIMPL_UINT_TRAIT_0x775u HEX, 1909u, 1909, (7, 7, 5), 0x88Au
#define PPUTLIMPL_UINT_TRAIT_0x774u HEX, 1908u, 1908, (7, 7, 4), 0x88Bu
#define PPUTLIMPL_UINT_TRAIT_0x773u HEX, 1907u, 1907, (7, 7, 3), 0x88Cu
#define PPUTLIMPL_UINT_TRAIT_0x772u HEX, 1906u, 1906, (7, 7, 2), 0x88Du
#define PPUTLIMPL_UINT_TRAIT_0x771u HEX, 1905u, 1905, (7, 7, 1), 0x88Eu
#define PPUTLIMPL_UINT_TRAIT_0x770u HEX, 1904u, 1904, (7, 7, 0), 0x88Fu
#define PPUTLIMPL_UINT_TRAIT_0x76Fu HEX, 1903u, 1903, (7, 6, F), 0x890u
#define PPUTLIMPL_UINT_TRAIT_0x76Eu HEX, 1902u, 1902, (7, 6, E), 0x891u
#define PPUTLIMPL_UINT_TRAIT_0x76Du HEX, 1901u, 1901, (7, 6, D), 0x892u
#define PPUTLIMPL_UINT_TRAIT_0x76Cu HEX, 1900u, 1900, (7, 6, C), 0x893u
#define PPUTLIMPL_UINT_TRAIT_0x76Bu HEX, 1899u, 1899, (7, 6, B), 0x894u
#define PPUTLIMPL_UINT_TRAIT_0x76Au HEX, 1898u, 1898, (7, 6, A), 0x895u
#define PPUTLIMPL_UINT_TRAIT_0x769u HEX, 1897u, 1897, (7, 6, 9), 0x896u
#define PPUTLIMPL_UINT_TRAIT_0x768u HEX, 1896u, 1896, (7, 6, 8), 0x897u
#define PPUTLIMPL_UINT_TRAIT_0x767u HEX, 1895u, 1895, (7, 6, 7), 0x898u
#define PPUTLIMPL_UINT_TRAIT_0x766u HEX, 1894u, 1894, (7, 6, 6), 0x899u
#define PPUTLIMPL_UINT_TRAIT_0x765u HEX, 1893u, 1893, (7, 6, 5), 0x89Au
#define PPUTLIMPL_UINT_TRAIT_0x764u HEX, 1892u, 1892, (7, 6, 4), 0x89Bu
#define PPUTLIMPL_UINT_TRAIT_0x763u HEX, 1891u, 1891, (7, 6, 3), 0x89Cu
#define PPUTLIMPL_UINT_TRAIT_0x762u HEX, 1890u, 1890, (7, 6, 2), 0x89Du
#define PPUTLIMPL_UINT_TRAIT_0x761u HEX, 1889u, 1889, (7, 6, 1), 0x89Eu
#define PPUTLIMPL_UINT_TRAIT_0x760u HEX, 1888u, 1888, (7, 6, 0), 0x89Fu
#define PPUTLIMPL_UINT_TRAIT_0x75Fu HEX, 1887u, 1887, (7, 5, F), 0x8A0u
#define PPUTLIMPL_UINT_TRAIT_0x75Eu HEX, 1886u, 1886, (7, 5, E), 0x8A1u
#define PPUTLIMPL_UINT_TRAIT_0x75Du HEX, 1885u, 1885, (7, 5, D), 0x8A2u
#define PPUTLIMPL_UINT_TRAIT_0x75Cu HEX, 1884u, 1884, (7, 5, C), 0x8A3u
#define PPUTLIMPL_UINT_TRAIT_0x75Bu HEX, 1883u, 1883, (7, 5, B), 0x8A4u
#define PPUTLIMPL_UINT_TRAIT_0x75Au HEX, 1882u, 1882, (7, 5, A), 0x8A5u
#define PPUTLIMPL_UINT_TRAIT_0x759u HEX, 1881u, 1881, (7, 5, 9), 0x8A6u
#define PPUTLIMPL_UINT_TRAIT_0x758u HEX, 1880u, 1880, (7, 5, 8), 0x8A7u
#define PPUTLIMPL_UINT_TRAIT_0x757u HEX, 1879u, 1879, (7, 5, 7), 0x8A8u
#define PPUTLIMPL_UINT_TRAIT_0x756u HEX, 1878u, 1878, (7, 5, 6), 0x8A9u
#define PPUTLIMPL_UINT_TRAIT_0x755u HEX, 1877u, 1877, (7, 5, 5), 0x8AAu
#define PPUTLIMPL_UINT_TRAIT_0x754u HEX, 1876u, 1876, (7, 5, 4), 0x8ABu
#define PPUTLIMPL_UINT_TRAIT_0x753u HEX, 1875u, 1875, (7, 5, 3), 0x8ACu
#define PPUTLIMPL_UINT_TRAIT_0x752u HEX, 1874u, 1874, (7, 5, 2), 0x8ADu
#define PPUTLIMPL_UINT_TRAIT_0x751u HEX, 1873u, 1873, (7, 5, 1), 0x8AEu
#define PPUTLIMPL_UINT_TRAIT_0x750u HEX, 1872u, 1872, (7, 5, 0), 0x8AFu
#define PPUTLIMPL_UINT_TRAIT_0x74Fu HEX, 1871u, 1871, (7, 4, F), 0x8B0u
#define PPUTLIMPL_UINT_TRAIT_0x74Eu HEX, 1870u, 1870, (7, 4, E), 0x8B1u
#define PPUTLIMPL_UINT_TRAIT_0x74Du HEX, 1869u, 1869, (7, 4, D), 0x8B2u
#define PPUTLIMPL_UINT_TRAIT_0x74Cu HEX, 1868u, 1868, (7, 4, C), 0x8B3u
#define PPUTLIMPL_UINT_TRAIT_0x74Bu HEX, 1867u, 1867, (7, 4, B), 0x8B4u
#define PPUTLIMPL_UINT_TRAIT_0x74Au HEX, 1866u, 1866, (7, 4, A), 0x8B5u
#define PPUTLIMPL_UINT_TRAIT_0x749u HEX, 1865u, 1865, (7, 4, 9), 0x8B6u
#define PPUTLIMPL_UINT_TRAIT_0x748u HEX, 1864u, 1864, (7, 4, 8), 0x8B7u
#define PPUTLIMPL_UINT_TRAIT_0x747u HEX, 1863u, 1863, (7, 4, 7), 0x8B8u
#define PPUTLIMPL_UINT_TRAIT_0x746u HEX, 1862u, 1862, (7, 4, 6), 0x8B9u
#define PPUTLIMPL_UINT_TRAIT_0x745u HEX, 1861u, 1861, (7, 4, 5), 0x8BAu
#define PPUTLIMPL_UINT_TRAIT_0x744u HEX, 1860u, 1860, (7, 4, 4), 0x8BBu
#define PPUTLIMPL_UINT_TRAIT_0x743u HEX, 1859u, 1859, (7, 4, 3), 0x8BCu
#define PPUTLIMPL_UINT_TRAIT_0x742u HEX, 1858u, 1858, (7, 4, 2), 0x8BDu
#define PPUTLIMPL_UINT_TRAIT_0x741u HEX, 1857u, 1857, (7, 4, 1), 0x8BEu
#define PPUTLIMPL_UINT_TRAIT_0x740u HEX, 1856u, 1856, (7, 4, 0), 0x8BFu
#define PPUTLIMPL_UINT_TRAIT_0x73Fu HEX, 1855u, 1855, (7, 3, F), 0x8C0u
#define PPUTLIMPL_UINT_TRAIT_0x73Eu HEX, 1854u, 1854, (7, 3, E), 0x8C1u
#define PPUTLIMPL_UINT_TRAIT_0x73Du HEX, 1853u, 1853, (7, 3, D), 0x8C2u
#define PPUTLIMPL_UINT_TRAIT_0x73Cu HEX, 1852u, 1852, (7, 3, C), 0x8C3u
#define PPUTLIMPL_UINT_TRAIT_0x73Bu HEX, 1851u, 1851, (7, 3, B), 0x8C4u
#define PPUTLIMPL_UINT_TRAIT_0x73Au HEX, 1850u, 1850, (7, 3, A), 0x8C5u
#define PPUTLIMPL_UINT_TRAIT_0x739u HEX, 1849u, 1849, (7, 3, 9), 0x8C6u
#define PPUTLIMPL_UINT_TRAIT_0x738u HEX, 1848u, 1848, (7, 3, 8), 0x8C7u
#define PPUTLIMPL_UINT_TRAIT_0x737u HEX, 1847u, 1847, (7, 3, 7), 0x8C8u
#define PPUTLIMPL_UINT_TRAIT_0x736u HEX, 1846u, 1846, (7, 3, 6), 0x8C9u
#define PPUTLIMPL_UINT_TRAIT_0x735u HEX, 1845u, 1845, (7, 3, 5), 0x8CAu
#define PPUTLIMPL_UINT_TRAIT_0x734u HEX, 1844u, 1844, (7, 3, 4), 0x8CBu
#define PPUTLIMPL_UINT_TRAIT_0x733u HEX, 1843u, 1843, (7, 3, 3), 0x8CCu
#define PPUTLIMPL_UINT_TRAIT_0x732u HEX, 1842u, 1842, (7, 3, 2), 0x8CDu
#define PPUTLIMPL_UINT_TRAIT_0x731u HEX, 1841u, 1841, (7, 3, 1), 0x8CEu
#define PPUTLIMPL_UINT_TRAIT_0x730u HEX, 1840u, 1840, (7, 3, 0), 0x8CFu
#define PPUTLIMPL_UINT_TRAIT_0x72Fu HEX, 1839u, 1839, (7, 2, F), 0x8D0u
#define PPUTLIMPL_UINT_TRAIT_0x72Eu HEX, 1838u, 1838, (7, 2, E), 0x8D1u
#define PPUTLIMPL_UINT_TRAIT_0x72Du HEX, 1837u, 1837, (7, 2, D), 0x8D2u
#define PPUTLIMPL_UINT_TRAIT_0x72Cu HEX, 1836u, 1836, (7, 2, C), 0x8D3u
#define PPUTLIMPL_UINT_TRAIT_0x72Bu HEX, 1835u, 1835, (7, 2, B), 0x8D4u
#define PPUTLIMPL_UINT_TRAIT_0x72Au HEX, 1834u, 1834, (7, 2, A), 0x8D5u
#define PPUTLIMPL_UINT_TRAIT_0x729u HEX, 1833u, 1833, (7, 2, 9), 0x8D6u
#define PPUTLIMPL_UINT_TRAIT_0x728u HEX, 1832u, 1832, (7, 2, 8), 0x8D7u
#define PPUTLIMPL_UINT_TRAIT_0x727u HEX, 1831u, 1831, (7, 2, 7), 0x8D8u
#define PPUTLIMPL_UINT_TRAIT_0x726u HEX, 1830u, 1830, (7, 2, 6), 0x8D9u
#define PPUTLIMPL_UINT_TRAIT_0x725u HEX, 1829u, 1829, (7, 2, 5), 0x8DAu
#define PPUTLIMPL_UINT_TRAIT_0x724u HEX, 1828u, 1828, (7, 2, 4), 0x8DBu
#define PPUTLIMPL_UINT_TRAIT_0x723u HEX, 1827u, 1827, (7, 2, 3), 0x8DCu
#define PPUTLIMPL_UINT_TRAIT_0x722u HEX, 1826u, 1826, (7, 2, 2), 0x8DDu
#define PPUTLIMPL_UINT_TRAIT_0x721u HEX, 1825u, 1825, (7, 2, 1), 0x8DEu
#define PPUTLIMPL_UINT_TRAIT_0x720u HEX, 1824u, 1824, (7, 2, 0), 0x8DFu
#define PPUTLIMPL_UINT_TRAIT_0x71Fu HEX, 1823u, 1823, (7, 1, F), 0x8E0u
#define PPUTLIMPL_UINT_TRAIT_0x71Eu HEX, 1822u, 1822, (7, 1, E), 0x8E1u
#define PPUTLIMPL_UINT_TRAIT_0x71Du HEX, 1821u, 1821, (7, 1, D), 0x8E2u
#define PPUTLIMPL_UINT_TRAIT_0x71Cu HEX, 1820u, 1820, (7, 1, C), 0x8E3u
#define PPUTLIMPL_UINT_TRAIT_0x71Bu HEX, 1819u, 1819, (7, 1, B), 0x8E4u
#define PPUTLIMPL_UINT_TRAIT_0x71Au HEX, 1818u, 1818, (7, 1, A), 0x8E5u
#define PPUTLIMPL_UINT_TRAIT_0x719u HEX, 1817u, 1817, (7, 1, 9), 0x8E6u
#define PPUTLIMPL_UINT_TRAIT_0x718u HEX, 1816u, 1816, (7, 1, 8), 0x8E7u
#define PPUTLIMPL_UINT_TRAIT_0x717u HEX, 1815u, 1815, (7, 1, 7), 0x8E8u
#define PPUTLIMPL_UINT_TRAIT_0x716u HEX, 1814u, 1814, (7, 1, 6), 0x8E9u
#define PPUTLIMPL_UINT_TRAIT_0x715u HEX, 1813u, 1813, (7, 1, 5), 0x8EAu
#define PPUTLIMPL_UINT_TRAIT_0x714u HEX, 1812u, 1812, (7, 1, 4), 0x8EBu
#define PPUTLIMPL_UINT_TRAIT_0x713u HEX, 1811u, 1811, (7, 1, 3), 0x8ECu
#define PPUTLIMPL_UINT_TRAIT_0x712u HEX, 1810u, 1810, (7, 1, 2), 0x8EDu
#define PPUTLIMPL_UINT_TRAIT_0x711u HEX, 1809u, 1809, (7, 1, 1), 0x8EEu
#define PPUTLIMPL_UINT_TRAIT_0x710u HEX, 1808u, 1808, (7, 1, 0), 0x8EFu
#define PPUTLIMPL_UINT_TRAIT_0x70Fu HEX, 1807u, 1807, (7, 0, F), 0x8F0u
#define PPUTLIMPL_UINT_TRAIT_0x70Eu HEX, 1806u, 1806, (7, 0, E), 0x8F1u
#define PPUTLIMPL_UINT_TRAIT_0x70Du HEX, 1805u, 1805, (7, 0, D), 0x8F2u
#define PPUTLIMPL_UINT_TRAIT_0x70Cu HEX, 1804u, 1804, (7, 0, C), 0x8F3u
#define PPUTLIMPL_UINT_TRAIT_0x70Bu HEX, 1803u, 1803, (7, 0, B), 0x8F4u
#define PPUTLIMPL_UINT_TRAIT_0x70Au HEX, 1802u, 1802, (7, 0, A), 0x8F5u
#define PPUTLIMPL_UINT_TRAIT_0x709u HEX, 1801u, 1801, (7, 0, 9), 0x8F6u
#define PPUTLIMPL_UINT_TRAIT_0x708u HEX, 1800u, 1800, (7, 0, 8), 0x8F7u
#define PPUTLIMPL_UINT_TRAIT_0x707u HEX, 1799u, 1799, (7, 0, 7), 0x8F8u
#define PPUTLIMPL_UINT_TRAIT_0x706u HEX, 1798u, 1798, (7, 0, 6), 0x8F9u
#define PPUTLIMPL_UINT_TRAIT_0x705u HEX, 1797u, 1797, (7, 0, 5), 0x8FAu
#define PPUTLIMPL_UINT_TRAIT_0x704u HEX, 1796u, 1796, (7, 0, 4), 0x8FBu
#define PPUTLIMPL_UINT_TRAIT_0x703u HEX, 1795u, 1795, (7, 0, 3), 0x8FCu
#define PPUTLIMPL_UINT_TRAIT_0x702u HEX, 1794u, 1794, (7, 0, 2), 0x8FDu
#define PPUTLIMPL_UINT_TRAIT_0x701u HEX, 1793u, 1793, (7, 0, 1), 0x8FEu
#define PPUTLIMPL_UINT_TRAIT_0x700u HEX, 1792u, 1792, (7, 0, 0), 0x8FFu
#define PPUTLIMPL_UINT_TRAIT_0x6FFu HEX, 1791u, 1791, (6, F, F), 0x900u
#define PPUTLIMPL_UINT_TRAIT_0x6FEu HEX, 1790u, 1790, (6, F, E), 0x901u
#define PPUTLIMPL_UINT_TRAIT_0x6FDu HEX, 1789u, 1789, (6, F, D), 0x902u
#define PPUTLIMPL_UINT_TRAIT_0x6FCu HEX, 1788u, 1788, (6, F, C), 0x903u
#define PPUTLIMPL_UINT_TRAIT_0x6FBu HEX, 1787u, 1787, (6, F, B), 0x904u
#define PPUTLIMPL_UINT_TRAIT_0x6FAu HEX, 1786u, 1786, (6, F, A), 0x905u
#define PPUTLIMPL_UINT_TRAIT_0x6F9u HEX, 1785u, 1785, (6, F, 9), 0x906u
#define PPUTLIMPL_UINT_TRAIT_0x6F8u HEX, 1784u, 1784, (6, F, 8), 0x907u
#define PPUTLIMPL_UINT_TRAIT_0x6F7u HEX, 1783u, 1783, (6, F, 7), 0x908u
#define PPUTLIMPL_UINT_TRAIT_0x6F6u HEX, 1782u, 1782, (6, F, 6), 0x909u
#define PPUTLIMPL_UINT_TRAIT_0x6F5u HEX, 1781u, 1781, (6, F, 5), 0x90Au
#define PPUTLIMPL_UINT_TRAIT_0x6F4u HEX, 1780u, 1780, (6, F, 4), 0x90Bu
#define PPUTLIMPL_UINT_TRAIT_0x6F3u HEX, 1779u, 1779, (6, F, 3), 0x90Cu
#define PPUTLIMPL_UINT_TRAIT_0x6F2u HEX, 1778u, 1778, (6, F, 2), 0x90Du
#define PPUTLIMPL_UINT_TRAIT_0x6F1u HEX, 1777u, 1777, (6, F, 1), 0x90Eu
#define PPUTLIMPL_UINT_TRAIT_0x6F0u HEX, 1776u, 1776, (6, F, 0), 0x90Fu
#define PPUTLIMPL_UINT_TRAIT_0x6EFu HEX, 1775u, 1775, (6, E, F), 0x910u
#define PPUTLIMPL_UINT_TRAIT_0x6EEu HEX, 1774u, 1774, (6, E, E), 0x911u
#define PPUTLIMPL_UINT_TRAIT_0x6EDu HEX, 1773u, 1773, (6, E, D), 0x912u
#define PPUTLIMPL_UINT_TRAIT_0x6ECu HEX, 1772u, 1772, (6, E, C), 0x913u
#define PPUTLIMPL_UINT_TRAIT_0x6EBu HEX, 1771u, 1771, (6, E, B), 0x914u
#define PPUTLIMPL_UINT_TRAIT_0x6EAu HEX, 1770u, 1770, (6, E, A), 0x915u
#define PPUTLIMPL_UINT_TRAIT_0x6E9u HEX, 1769u, 1769, (6, E, 9), 0x916u
#define PPUTLIMPL_UINT_TRAIT_0x6E8u HEX, 1768u, 1768, (6, E, 8), 0x917u
#define PPUTLIMPL_UINT_TRAIT_0x6E7u HEX, 1767u, 1767, (6, E, 7), 0x918u
#define PPUTLIMPL_UINT_TRAIT_0x6E6u HEX, 1766u, 1766, (6, E, 6), 0x919u
#define PPUTLIMPL_UINT_TRAIT_0x6E5u HEX, 1765u, 1765, (6, E, 5), 0x91Au
#define PPUTLIMPL_UINT_TRAIT_0x6E4u HEX, 1764u, 1764, (6, E, 4), 0x91Bu
#define PPUTLIMPL_UINT_TRAIT_0x6E3u HEX, 1763u, 1763, (6, E, 3), 0x91Cu
#define PPUTLIMPL_UINT_TRAIT_0x6E2u HEX, 1762u, 1762, (6, E, 2), 0x91Du
#define PPUTLIMPL_UINT_TRAIT_0x6E1u HEX, 1761u, 1761, (6, E, 1), 0x91Eu
#define PPUTLIMPL_UINT_TRAIT_0x6E0u HEX, 1760u, 1760, (6, E, 0), 0x91Fu
#define PPUTLIMPL_UINT_TRAIT_0x6DFu HEX, 1759u, 1759, (6, D, F), 0x920u
#define PPUTLIMPL_UINT_TRAIT_0x6DEu HEX, 1758u, 1758, (6, D, E), 0x921u
#define PPUTLIMPL_UINT_TRAIT_0x6DDu HEX, 1757u, 1757, (6, D, D), 0x922u
#define PPUTLIMPL_UINT_TRAIT_0x6DCu HEX, 1756u, 1756, (6, D, C), 0x923u
#define PPUTLIMPL_UINT_TRAIT_0x6DBu HEX, 1755u, 1755, (6, D, B), 0x924u
#define PPUTLIMPL_UINT_TRAIT_0x6DAu HEX, 1754u, 1754, (6, D, A), 0x925u
#define PPUTLIMPL_UINT_TRAIT_0x6D9u HEX, 1753u, 1753, (6, D, 9), 0x926u
#define PPUTLIMPL_UINT_TRAIT_0x6D8u HEX, 1752u, 1752, (6, D, 8), 0x927u
#define PPUTLIMPL_UINT_TRAIT_0x6D7u HEX, 1751u, 1751, (6, D, 7), 0x928u
#define PPUTLIMPL_UINT_TRAIT_0x6D6u HEX, 1750u, 1750, (6, D, 6), 0x929u
#define PPUTLIMPL_UINT_TRAIT_0x6D5u HEX, 1749u, 1749, (6, D, 5), 0x92Au
#define PPUTLIMPL_UINT_TRAIT_0x6D4u HEX, 1748u, 1748, (6, D, 4), 0x92Bu
#define PPUTLIMPL_UINT_TRAIT_0x6D3u HEX, 1747u, 1747, (6, D, 3), 0x92Cu
#define PPUTLIMPL_UINT_TRAIT_0x6D2u HEX, 1746u, 1746, (6, D, 2), 0x92Du
#define PPUTLIMPL_UINT_TRAIT_0x6D1u HEX, 1745u, 1745, (6, D, 1), 0x92Eu
#define PPUTLIMPL_UINT_TRAIT_0x6D0u HEX, 1744u, 1744, (6, D, 0), 0x92Fu
#define PPUTLIMPL_UINT_TRAIT_0x6CFu HEX, 1743u, 1743, (6, C, F), 0x930u
#define PPUTLIMPL_UINT_TRAIT_0x6CEu HEX, 1742u, 1742, (6, C, E), 0x931u
#define PPUTLIMPL_UINT_TRAIT_0x6CDu HEX, 1741u, 1741, (6, C, D), 0x932u
#define PPUTLIMPL_UINT_TRAIT_0x6CCu HEX, 1740u, 1740, (6, C, C), 0x933u
#define PPUTLIMPL_UINT_TRAIT_0x6CBu HEX, 1739u, 1739, (6, C, B), 0x934u
#define PPUTLIMPL_UINT_TRAIT_0x6CAu HEX, 1738u, 1738, (6, C, A), 0x935u
#define PPUTLIMPL_UINT_TRAIT_0x6C9u HEX, 1737u, 1737, (6, C, 9), 0x936u
#define PPUTLIMPL_UINT_TRAIT_0x6C8u HEX, 1736u, 1736, (6, C, 8), 0x937u
#define PPUTLIMPL_UINT_TRAIT_0x6C7u HEX, 1735u, 1735, (6, C, 7), 0x938u
#define PPUTLIMPL_UINT_TRAIT_0x6C6u HEX, 1734u, 1734, (6, C, 6), 0x939u
#define PPUTLIMPL_UINT_TRAIT_0x6C5u HEX, 1733u, 1733, (6, C, 5), 0x93Au
#define PPUTLIMPL_UINT_TRAIT_0x6C4u HEX, 1732u, 1732, (6, C, 4), 0x93Bu
#define PPUTLIMPL_UINT_TRAIT_0x6C3u HEX, 1731u, 1731, (6, C, 3), 0x93Cu
#define PPUTLIMPL_UINT_TRAIT_0x6C2u HEX, 1730u, 1730, (6, C, 2), 0x93Du
#define PPUTLIMPL_UINT_TRAIT_0x6C1u HEX, 1729u, 1729, (6, C, 1), 0x93Eu
#define PPUTLIMPL_UINT_TRAIT_0x6C0u HEX, 1728u, 1728, (6, C, 0), 0x93Fu
#define PPUTLIMPL_UINT_TRAIT_0x6BFu HEX, 1727u, 1727, (6, B, F), 0x940u
#define PPUTLIMPL_UINT_TRAIT_0x6BEu HEX, 1726u, 1726, (6, B, E), 0x941u
#define PPUTLIMPL_UINT_TRAIT_0x6BDu HEX, 1725u, 1725, (6, B, D), 0x942u
#define PPUTLIMPL_UINT_TRAIT_0x6BCu HEX, 1724u, 1724, (6, B, C), 0x943u
#define PPUTLIMPL_UINT_TRAIT_0x6BBu HEX, 1723u, 1723, (6, B, B), 0x944u
#define PPUTLIMPL_UINT_TRAIT_0x6BAu HEX, 1722u, 1722, (6, B, A), 0x945u
#define PPUTLIMPL_UINT_TRAIT_0x6B9u HEX, 1721u, 1721, (6, B, 9), 0x946u
#define PPUTLIMPL_UINT_TRAIT_0x6B8u HEX, 1720u, 1720, (6, B, 8), 0x947u
#define PPUTLIMPL_UINT_TRAIT_0x6B7u HEX, 1719u, 1719, (6, B, 7), 0x948u
#define PPUTLIMPL_UINT_TRAIT_0x6B6u HEX, 1718u, 1718, (6, B, 6), 0x949u
#define PPUTLIMPL_UINT_TRAIT_0x6B5u HEX, 1717u, 1717, (6, B, 5), 0x94Au
#define PPUTLIMPL_UINT_TRAIT_0x6B4u HEX, 1716u, 1716, (6, B, 4), 0x94Bu
#define PPUTLIMPL_UINT_TRAIT_0x6B3u HEX, 1715u, 1715, (6, B, 3), 0x94Cu
#define PPUTLIMPL_UINT_TRAIT_0x6B2u HEX, 1714u, 1714, (6, B, 2), 0x94Du
#define PPUTLIMPL_UINT_TRAIT_0x6B1u HEX, 1713u, 1713, (6, B, 1), 0x94Eu
#define PPUTLIMPL_UINT_TRAIT_0x6B0u HEX, 1712u, 1712, (6, B, 0), 0x94Fu
#define PPUTLIMPL_UINT_TRAIT_0x6AFu HEX, 1711u, 1711, (6, A, F), 0x950u
#define PPUTLIMPL_UINT_TRAIT_0x6AEu HEX, 1710u, 1710, (6, A, E), 0x951u
#define PPUTLIMPL_UINT_TRAIT_0x6ADu HEX, 1709u, 1709, (6, A, D), 0x952u
#define PPUTLIMPL_UINT_TRAIT_0x6ACu HEX, 1708u, 1708, (6, A, C), 0x953u
#define PPUTLIMPL_UINT_TRAIT_0x6ABu HEX, 1707u, 1707, (6, A, B), 0x954u
#define PPUTLIMPL_UINT_TRAIT_0x6AAu HEX, 1706u, 1706, (6, A, A), 0x955u
#define PPUTLIMPL_UINT_TRAIT_0x6A9u HEX, 1705u, 1705, (6, A, 9), 0x956u
#define PPUTLIMPL_UINT_TRAIT_0x6A8u HEX, 1704u, 1704, (6, A, 8), 0x957u
#define PPUTLIMPL_UINT_TRAIT_0x6A7u HEX, 1703u, 1703, (6, A, 7), 0x958u
#define PPUTLIMPL_UINT_TRAIT_0x6A6u HEX, 1702u, 1702, (6, A, 6), 0x959u
#define PPUTLIMPL_UINT_TRAIT_0x6A5u HEX, 1701u, 1701, (6, A, 5), 0x95Au
#define PPUTLIMPL_UINT_TRAIT_0x6A4u HEX, 1700u, 1700, (6, A, 4), 0x95Bu
#define PPUTLIMPL_UINT_TRAIT_0x6A3u HEX, 1699u, 1699, (6, A, 3), 0x95Cu
#define PPUTLIMPL_UINT_TRAIT_0x6A2u HEX, 1698u, 1698, (6, A, 2), 0x95Du
#define PPUTLIMPL_UINT_TRAIT_0x6A1u HEX, 1697u, 1697, (6, A, 1), 0x95Eu
#define PPUTLIMPL_UINT_TRAIT_0x6A0u HEX, 1696u, 1696, (6, A, 0), 0x95Fu
#define PPUTLIMPL_UINT_TRAIT_0x69Fu HEX, 1695u, 1695, (6, 9, F), 0x960u
#define PPUTLIMPL_UINT_TRAIT_0x69Eu HEX, 1694u, 1694, (6, 9, E), 0x961u
#define PPUTLIMPL_UINT_TRAIT_0x69Du HEX, 1693u, 1693, (6, 9, D), 0x962u
#define PPUTLIMPL_UINT_TRAIT_0x69Cu HEX, 1692u, 1692, (6, 9, C), 0x963u
#define PPUTLIMPL_UINT_TRAIT_0x69Bu HEX, 1691u, 1691, (6, 9, B), 0x964u
#define PPUTLIMPL_UINT_TRAIT_0x69Au HEX, 1690u, 1690, (6, 9, A), 0x965u
#define PPUTLIMPL_UINT_TRAIT_0x699u HEX, 1689u, 1689, (6, 9, 9), 0x966u
#define PPUTLIMPL_UINT_TRAIT_0x698u HEX, 1688u, 1688, (6, 9, 8), 0x967u
#define PPUTLIMPL_UINT_TRAIT_0x697u HEX, 1687u, 1687, (6, 9, 7), 0x968u
#define PPUTLIMPL_UINT_TRAIT_0x696u HEX, 1686u, 1686, (6, 9, 6), 0x969u
#define PPUTLIMPL_UINT_TRAIT_0x695u HEX, 1685u, 1685, (6, 9, 5), 0x96Au
#define PPUTLIMPL_UINT_TRAIT_0x694u HEX, 1684u, 1684, (6, 9, 4), 0x96Bu
#define PPUTLIMPL_UINT_TRAIT_0x693u HEX, 1683u, 1683, (6, 9, 3), 0x96Cu
#define PPUTLIMPL_UINT_TRAIT_0x692u HEX, 1682u, 1682, (6, 9, 2), 0x96Du
#define PPUTLIMPL_UINT_TRAIT_0x691u HEX, 1681u, 1681, (6, 9, 1), 0x96Eu
#define PPUTLIMPL_UINT_TRAIT_0x690u HEX, 1680u, 1680, (6, 9, 0), 0x96Fu
#define PPUTLIMPL_UINT_TRAIT_0x68Fu HEX, 1679u, 1679, (6, 8, F), 0x970u
#define PPUTLIMPL_UINT_TRAIT_0x68Eu HEX, 1678u, 1678, (6, 8, E), 0x971u
#define PPUTLIMPL_UINT_TRAIT_0x68Du HEX, 1677u, 1677, (6, 8, D), 0x972u
#define PPUTLIMPL_UINT_TRAIT_0x68Cu HEX, 1676u, 1676, (6, 8, C), 0x973u
#define PPUTLIMPL_UINT_TRAIT_0x68Bu HEX, 1675u, 1675, (6, 8, B), 0x974u
#define PPUTLIMPL_UINT_TRAIT_0x68Au HEX, 1674u, 1674, (6, 8, A), 0x975u
#define PPUTLIMPL_UINT_TRAIT_0x689u HEX, 1673u, 1673, (6, 8, 9), 0x976u
#define PPUTLIMPL_UINT_TRAIT_0x688u HEX, 1672u, 1672, (6, 8, 8), 0x977u
#define PPUTLIMPL_UINT_TRAIT_0x687u HEX, 1671u, 1671, (6, 8, 7), 0x978u
#define PPUTLIMPL_UINT_TRAIT_0x686u HEX, 1670u, 1670, (6, 8, 6), 0x979u
#define PPUTLIMPL_UINT_TRAIT_0x685u HEX, 1669u, 1669, (6, 8, 5), 0x97Au
#define PPUTLIMPL_UINT_TRAIT_0x684u HEX, 1668u, 1668, (6, 8, 4), 0x97Bu
#define PPUTLIMPL_UINT_TRAIT_0x683u HEX, 1667u, 1667, (6, 8, 3), 0x97Cu
#define PPUTLIMPL_UINT_TRAIT_0x682u HEX, 1666u, 1666, (6, 8, 2), 0x97Du
#define PPUTLIMPL_UINT_TRAIT_0x681u HEX, 1665u, 1665, (6, 8, 1), 0x97Eu
#define PPUTLIMPL_UINT_TRAIT_0x680u HEX, 1664u, 1664, (6, 8, 0), 0x97Fu
#define PPUTLIMPL_UINT_TRAIT_0x67Fu HEX, 1663u, 1663, (6, 7, F), 0x980u
#define PPUTLIMPL_UINT_TRAIT_0x67Eu HEX, 1662u, 1662, (6, 7, E), 0x981u
#define PPUTLIMPL_UINT_TRAIT_0x67Du HEX, 1661u, 1661, (6, 7, D), 0x982u
#define PPUTLIMPL_UINT_TRAIT_0x67Cu HEX, 1660u, 1660, (6, 7, C), 0x983u
#define PPUTLIMPL_UINT_TRAIT_0x67Bu HEX, 1659u, 1659, (6, 7, B), 0x984u
#define PPUTLIMPL_UINT_TRAIT_0x67Au HEX, 1658u, 1658, (6, 7, A), 0x985u
#define PPUTLIMPL_UINT_TRAIT_0x679u HEX, 1657u, 1657, (6, 7, 9), 0x986u
#define PPUTLIMPL_UINT_TRAIT_0x678u HEX, 1656u, 1656, (6, 7, 8), 0x987u
#define PPUTLIMPL_UINT_TRAIT_0x677u HEX, 1655u, 1655, (6, 7, 7), 0x988u
#define PPUTLIMPL_UINT_TRAIT_0x676u HEX, 1654u, 1654, (6, 7, 6), 0x989u
#define PPUTLIMPL_UINT_TRAIT_0x675u HEX, 1653u, 1653, (6, 7, 5), 0x98Au
#define PPUTLIMPL_UINT_TRAIT_0x674u HEX, 1652u, 1652, (6, 7, 4), 0x98Bu
#define PPUTLIMPL_UINT_TRAIT_0x673u HEX, 1651u, 1651, (6, 7, 3), 0x98Cu
#define PPUTLIMPL_UINT_TRAIT_0x672u HEX, 1650u, 1650, (6, 7, 2), 0x98Du
#define PPUTLIMPL_UINT_TRAIT_0x671u HEX, 1649u, 1649, (6, 7, 1), 0x98Eu
#define PPUTLIMPL_UINT_TRAIT_0x670u HEX, 1648u, 1648, (6, 7, 0), 0x98Fu
#define PPUTLIMPL_UINT_TRAIT_0x66Fu HEX, 1647u, 1647, (6, 6, F), 0x990u
#define PPUTLIMPL_UINT_TRAIT_0x66Eu HEX, 1646u, 1646, (6, 6, E), 0x991u
#define PPUTLIMPL_UINT_TRAIT_0x66Du HEX, 1645u, 1645, (6, 6, D), 0x992u
#define PPUTLIMPL_UINT_TRAIT_0x66Cu HEX, 1644u, 1644, (6, 6, C), 0x993u
#define PPUTLIMPL_UINT_TRAIT_0x66Bu HEX, 1643u, 1643, (6, 6, B), 0x994u
#define PPUTLIMPL_UINT_TRAIT_0x66Au HEX, 1642u, 1642, (6, 6, A), 0x995u
#define PPUTLIMPL_UINT_TRAIT_0x669u HEX, 1641u, 1641, (6, 6, 9), 0x996u
#define PPUTLIMPL_UINT_TRAIT_0x668u HEX, 1640u, 1640, (6, 6, 8), 0x997u
#define PPUTLIMPL_UINT_TRAIT_0x667u HEX, 1639u, 1639, (6, 6, 7), 0x998u
#define PPUTLIMPL_UINT_TRAIT_0x666u HEX, 1638u, 1638, (6, 6, 6), 0x999u
#define PPUTLIMPL_UINT_TRAIT_0x665u HEX, 1637u, 1637, (6, 6, 5), 0x99Au
#define PPUTLIMPL_UINT_TRAIT_0x664u HEX, 1636u, 1636, (6, 6, 4), 0x99Bu
#define PPUTLIMPL_UINT_TRAIT_0x663u HEX, 1635u, 1635, (6, 6, 3), 0x99Cu
#define PPUTLIMPL_UINT_TRAIT_0x662u HEX, 1634u, 1634, (6, 6, 2), 0x99Du
#define PPUTLIMPL_UINT_TRAIT_0x661u HEX, 1633u, 1633, (6, 6, 1), 0x99Eu
#define PPUTLIMPL_UINT_TRAIT_0x660u HEX, 1632u, 1632, (6, 6, 0), 0x99Fu
#define PPUTLIMPL_UINT_TRAIT_0x65Fu HEX, 1631u, 1631, (6, 5, F), 0x9A0u
#define PPUTLIMPL_UINT_TRAIT_0x65Eu HEX, 1630u, 1630, (6, 5, E), 0x9A1u
#define PPUTLIMPL_UINT_TRAIT_0x65Du HEX, 1629u, 1629, (6, 5, D), 0x9A2u
#define PPUTLIMPL_UINT_TRAIT_0x65Cu HEX, 1628u, 1628, (6, 5, C), 0x9A3u
#define PPUTLIMPL_UINT_TRAIT_0x65Bu HEX, 1627u, 1627, (6, 5, B), 0x9A4u
#define PPUTLIMPL_UINT_TRAIT_0x65Au HEX, 1626u, 1626, (6, 5, A), 0x9A5u
#define PPUTLIMPL_UINT_TRAIT_0x659u HEX, 1625u, 1625, (6, 5, 9), 0x9A6u
#define PPUTLIMPL_UINT_TRAIT_0x658u HEX, 1624u, 1624, (6, 5, 8), 0x9A7u
#define PPUTLIMPL_UINT_TRAIT_0x657u HEX, 1623u, 1623, (6, 5, 7), 0x9A8u
#define PPUTLIMPL_UINT_TRAIT_0x656u HEX, 1622u, 1622, (6, 5, 6), 0x9A9u
#define PPUTLIMPL_UINT_TRAIT_0x655u HEX, 1621u, 1621, (6, 5, 5), 0x9AAu
#define PPUTLIMPL_UINT_TRAIT_0x654u HEX, 1620u, 1620, (6, 5, 4), 0x9ABu
#define PPUTLIMPL_UINT_TRAIT_0x653u HEX, 1619u, 1619, (6, 5, 3), 0x9ACu
#define PPUTLIMPL_UINT_TRAIT_0x652u HEX, 1618u, 1618, (6, 5, 2), 0x9ADu
#define PPUTLIMPL_UINT_TRAIT_0x651u HEX, 1617u, 1617, (6, 5, 1), 0x9AEu
#define PPUTLIMPL_UINT_TRAIT_0x650u HEX, 1616u, 1616, (6, 5, 0), 0x9AFu
#define PPUTLIMPL_UINT_TRAIT_0x64Fu HEX, 1615u, 1615, (6, 4, F), 0x9B0u
#define PPUTLIMPL_UINT_TRAIT_0x64Eu HEX, 1614u, 1614, (6, 4, E), 0x9B1u
#define PPUTLIMPL_UINT_TRAIT_0x64Du HEX, 1613u, 1613, (6, 4, D), 0x9B2u
#define PPUTLIMPL_UINT_TRAIT_0x64Cu HEX, 1612u, 1612, (6, 4, C), 0x9B3u
#define PPUTLIMPL_UINT_TRAIT_0x64Bu HEX, 1611u, 1611, (6, 4, B), 0x9B4u
#define PPUTLIMPL_UINT_TRAIT_0x64Au HEX, 1610u, 1610, (6, 4, A), 0x9B5u
#define PPUTLIMPL_UINT_TRAIT_0x649u HEX, 1609u, 1609, (6, 4, 9), 0x9B6u
#define PPUTLIMPL_UINT_TRAIT_0x648u HEX, 1608u, 1608, (6, 4, 8), 0x9B7u
#define PPUTLIMPL_UINT_TRAIT_0x647u HEX, 1607u, 1607, (6, 4, 7), 0x9B8u
#define PPUTLIMPL_UINT_TRAIT_0x646u HEX, 1606u, 1606, (6, 4, 6), 0x9B9u
#define PPUTLIMPL_UINT_TRAIT_0x645u HEX, 1605u, 1605, (6, 4, 5), 0x9BAu
#define PPUTLIMPL_UINT_TRAIT_0x644u HEX, 1604u, 1604, (6, 4, 4), 0x9BBu
#define PPUTLIMPL_UINT_TRAIT_0x643u HEX, 1603u, 1603, (6, 4, 3), 0x9BCu
#define PPUTLIMPL_UINT_TRAIT_0x642u HEX, 1602u, 1602, (6, 4, 2), 0x9BDu
#define PPUTLIMPL_UINT_TRAIT_0x641u HEX, 1601u, 1601, (6, 4, 1), 0x9BEu
#define PPUTLIMPL_UINT_TRAIT_0x640u HEX, 1600u, 1600, (6, 4, 0), 0x9BFu
#define PPUTLIMPL_UINT_TRAIT_0x63Fu HEX, 1599u, 1599, (6, 3, F), 0x9C0u
#define PPUTLIMPL_UINT_TRAIT_0x63Eu HEX, 1598u, 1598, (6, 3, E), 0x9C1u
#define PPUTLIMPL_UINT_TRAIT_0x63Du HEX, 1597u, 1597, (6, 3, D), 0x9C2u
#define PPUTLIMPL_UINT_TRAIT_0x63Cu HEX, 1596u, 1596, (6, 3, C), 0x9C3u
#define PPUTLIMPL_UINT_TRAIT_0x63Bu HEX, 1595u, 1595, (6, 3, B), 0x9C4u
#define PPUTLIMPL_UINT_TRAIT_0x63Au HEX, 1594u, 1594, (6, 3, A), 0x9C5u
#define PPUTLIMPL_UINT_TRAIT_0x639u HEX, 1593u, 1593, (6, 3, 9), 0x9C6u
#define PPUTLIMPL_UINT_TRAIT_0x638u HEX, 1592u, 1592, (6, 3, 8), 0x9C7u
#define PPUTLIMPL_UINT_TRAIT_0x637u HEX, 1591u, 1591, (6, 3, 7), 0x9C8u
#define PPUTLIMPL_UINT_TRAIT_0x636u HEX, 1590u, 1590, (6, 3, 6), 0x9C9u
#define PPUTLIMPL_UINT_TRAIT_0x635u HEX, 1589u, 1589, (6, 3, 5), 0x9CAu
#define PPUTLIMPL_UINT_TRAIT_0x634u HEX, 1588u, 1588, (6, 3, 4), 0x9CBu
#define PPUTLIMPL_UINT_TRAIT_0x633u HEX, 1587u, 1587, (6, 3, 3), 0x9CCu
#define PPUTLIMPL_UINT_TRAIT_0x632u HEX, 1586u, 1586, (6, 3, 2), 0x9CDu
#define PPUTLIMPL_UINT_TRAIT_0x631u HEX, 1585u, 1585, (6, 3, 1), 0x9CEu
#define PPUTLIMPL_UINT_TRAIT_0x630u HEX, 1584u, 1584, (6, 3, 0), 0x9CFu
#define PPUTLIMPL_UINT_TRAIT_0x62Fu HEX, 1583u, 1583, (6, 2, F), 0x9D0u
#define PPUTLIMPL_UINT_TRAIT_0x62Eu HEX, 1582u, 1582, (6, 2, E), 0x9D1u
#define PPUTLIMPL_UINT_TRAIT_0x62Du HEX, 1581u, 1581, (6, 2, D), 0x9D2u
#define PPUTLIMPL_UINT_TRAIT_0x62Cu HEX, 1580u, 1580, (6, 2, C), 0x9D3u
#define PPUTLIMPL_UINT_TRAIT_0x62Bu HEX, 1579u, 1579, (6, 2, B), 0x9D4u
#define PPUTLIMPL_UINT_TRAIT_0x62Au HEX, 1578u, 1578, (6, 2, A), 0x9D5u
#define PPUTLIMPL_UINT_TRAIT_0x629u HEX, 1577u, 1577, (6, 2, 9), 0x9D6u
#define PPUTLIMPL_UINT_TRAIT_0x628u HEX, 1576u, 1576, (6, 2, 8), 0x9D7u
#define PPUTLIMPL_UINT_TRAIT_0x627u HEX, 1575u, 1575, (6, 2, 7), 0x9D8u
#define PPUTLIMPL_UINT_TRAIT_0x626u HEX, 1574u, 1574, (6, 2, 6), 0x9D9u
#define PPUTLIMPL_UINT_TRAIT_0x625u HEX, 1573u, 1573, (6, 2, 5), 0x9DAu
#define PPUTLIMPL_UINT_TRAIT_0x624u HEX, 1572u, 1572, (6, 2, 4), 0x9DBu
#define PPUTLIMPL_UINT_TRAIT_0x623u HEX, 1571u, 1571, (6, 2, 3), 0x9DCu
#define PPUTLIMPL_UINT_TRAIT_0x622u HEX, 1570u, 1570, (6, 2, 2), 0x9DDu
#define PPUTLIMPL_UINT_TRAIT_0x621u HEX, 1569u, 1569, (6, 2, 1), 0x9DEu
#define PPUTLIMPL_UINT_TRAIT_0x620u HEX, 1568u, 1568, (6, 2, 0), 0x9DFu
#define PPUTLIMPL_UINT_TRAIT_0x61Fu HEX, 1567u, 1567, (6, 1, F), 0x9E0u
#define PPUTLIMPL_UINT_TRAIT_0x61Eu HEX, 1566u, 1566, (6, 1, E), 0x9E1u
#define PPUTLIMPL_UINT_TRAIT_0x61Du HEX, 1565u, 1565, (6, 1, D), 0x9E2u
#define PPUTLIMPL_UINT_TRAIT_0x61Cu HEX, 1564u, 1564, (6, 1, C), 0x9E3u
#define PPUTLIMPL_UINT_TRAIT_0x61Bu HEX, 1563u, 1563, (6, 1, B), 0x9E4u
#define PPUTLIMPL_UINT_TRAIT_0x61Au HEX, 1562u, 1562, (6, 1, A), 0x9E5u
#define PPUTLIMPL_UINT_TRAIT_0x619u HEX, 1561u, 1561, (6, 1, 9), 0x9E6u
#define PPUTLIMPL_UINT_TRAIT_0x618u HEX, 1560u, 1560, (6, 1, 8), 0x9E7u
#define PPUTLIMPL_UINT_TRAIT_0x617u HEX, 1559u, 1559, (6, 1, 7), 0x9E8u
#define PPUTLIMPL_UINT_TRAIT_0x616u HEX, 1558u, 1558, (6, 1, 6), 0x9E9u
#define PPUTLIMPL_UINT_TRAIT_0x615u HEX, 1557u, 1557, (6, 1, 5), 0x9EAu
#define PPUTLIMPL_UINT_TRAIT_0x614u HEX, 1556u, 1556, (6, 1, 4), 0x9EBu
#define PPUTLIMPL_UINT_TRAIT_0x613u HEX, 1555u, 1555, (6, 1, 3), 0x9ECu
#define PPUTLIMPL_UINT_TRAIT_0x612u HEX, 1554u, 1554, (6, 1, 2), 0x9EDu
#define PPUTLIMPL_UINT_TRAIT_0x611u HEX, 1553u, 1553, (6, 1, 1), 0x9EEu
#define PPUTLIMPL_UINT_TRAIT_0x610u HEX, 1552u, 1552, (6, 1, 0), 0x9EFu
#define PPUTLIMPL_UINT_TRAIT_0x60Fu HEX, 1551u, 1551, (6, 0, F), 0x9F0u
#define PPUTLIMPL_UINT_TRAIT_0x60Eu HEX, 1550u, 1550, (6, 0, E), 0x9F1u
#define PPUTLIMPL_UINT_TRAIT_0x60Du HEX, 1549u, 1549, (6, 0, D), 0x9F2u
#define PPUTLIMPL_UINT_TRAIT_0x60Cu HEX, 1548u, 1548, (6, 0, C), 0x9F3u
#define PPUTLIMPL_UINT_TRAIT_0x60Bu HEX, 1547u, 1547, (6, 0, B), 0x9F4u
#define PPUTLIMPL_UINT_TRAIT_0x60Au HEX, 1546u, 1546, (6, 0, A), 0x9F5u
#define PPUTLIMPL_UINT_TRAIT_0x609u HEX, 1545u, 1545, (6, 0, 9), 0x9F6u
#define PPUTLIMPL_UINT_TRAIT_0x608u HEX, 1544u, 1544, (6, 0, 8), 0x9F7u
#define PPUTLIMPL_UINT_TRAIT_0x607u HEX, 1543u, 1543, (6, 0, 7), 0x9F8u
#define PPUTLIMPL_UINT_TRAIT_0x606u HEX, 1542u, 1542, (6, 0, 6), 0x9F9u
#define PPUTLIMPL_UINT_TRAIT_0x605u HEX, 1541u, 1541, (6, 0, 5), 0x9FAu
#define PPUTLIMPL_UINT_TRAIT_0x604u HEX, 1540u, 1540, (6, 0, 4), 0x9FBu
#define PPUTLIMPL_UINT_TRAIT_0x603u HEX, 1539u, 1539, (6, 0, 3), 0x9FCu
#define PPUTLIMPL_UINT_TRAIT_0x602u HEX, 1538u, 1538, (6, 0, 2), 0x9FDu
#define PPUTLIMPL_UINT_TRAIT_0x601u HEX, 1537u, 1537, (6, 0, 1), 0x9FEu
#define PPUTLIMPL_UINT_TRAIT_0x600u HEX, 1536u, 1536, (6, 0, 0), 0x9FFu
#define PPUTLIMPL_UINT_TRAIT_0x5FFu HEX, 1535u, 1535, (5, F, F), 0xA00u
#define PPUTLIMPL_UINT_TRAIT_0x5FEu HEX, 1534u, 1534, (5, F, E), 0xA01u
#define PPUTLIMPL_UINT_TRAIT_0x5FDu HEX, 1533u, 1533, (5, F, D), 0xA02u
#define PPUTLIMPL_UINT_TRAIT_0x5FCu HEX, 1532u, 1532, (5, F, C), 0xA03u
#define PPUTLIMPL_UINT_TRAIT_0x5FBu HEX, 1531u, 1531, (5, F, B), 0xA04u
#define PPUTLIMPL_UINT_TRAIT_0x5FAu HEX, 1530u, 1530, (5, F, A), 0xA05u
#define PPUTLIMPL_UINT_TRAIT_0x5F9u HEX, 1529u, 1529, (5, F, 9), 0xA06u
#define PPUTLIMPL_UINT_TRAIT_0x5F8u HEX, 1528u, 1528, (5, F, 8), 0xA07u
#define PPUTLIMPL_UINT_TRAIT_0x5F7u HEX, 1527u, 1527, (5, F, 7), 0xA08u
#define PPUTLIMPL_UINT_TRAIT_0x5F6u HEX, 1526u, 1526, (5, F, 6), 0xA09u
#define PPUTLIMPL_UINT_TRAIT_0x5F5u HEX, 1525u, 1525, (5, F, 5), 0xA0Au
#define PPUTLIMPL_UINT_TRAIT_0x5F4u HEX, 1524u, 1524, (5, F, 4), 0xA0Bu
#define PPUTLIMPL_UINT_TRAIT_0x5F3u HEX, 1523u, 1523, (5, F, 3), 0xA0Cu
#define PPUTLIMPL_UINT_TRAIT_0x5F2u HEX, 1522u, 1522, (5, F, 2), 0xA0Du
#define PPUTLIMPL_UINT_TRAIT_0x5F1u HEX, 1521u, 1521, (5, F, 1), 0xA0Eu
#define PPUTLIMPL_UINT_TRAIT_0x5F0u HEX, 1520u, 1520, (5, F, 0), 0xA0Fu
#define PPUTLIMPL_UINT_TRAIT_0x5EFu HEX, 1519u, 1519, (5, E, F), 0xA10u
#define PPUTLIMPL_UINT_TRAIT_0x5EEu HEX, 1518u, 1518, (5, E, E), 0xA11u
#define PPUTLIMPL_UINT_TRAIT_0x5EDu HEX, 1517u, 1517, (5, E, D), 0xA12u
#define PPUTLIMPL_UINT_TRAIT_0x5ECu HEX, 1516u, 1516, (5, E, C), 0xA13u
#define PPUTLIMPL_UINT_TRAIT_0x5EBu HEX, 1515u, 1515, (5, E, B), 0xA14u
#define PPUTLIMPL_UINT_TRAIT_0x5EAu HEX, 1514u, 1514, (5, E, A), 0xA15u
#define PPUTLIMPL_UINT_TRAIT_0x5E9u HEX, 1513u, 1513, (5, E, 9), 0xA16u
#define PPUTLIMPL_UINT_TRAIT_0x5E8u HEX, 1512u, 1512, (5, E, 8), 0xA17u
#define PPUTLIMPL_UINT_TRAIT_0x5E7u HEX, 1511u, 1511, (5, E, 7), 0xA18u
#define PPUTLIMPL_UINT_TRAIT_0x5E6u HEX, 1510u, 1510, (5, E, 6), 0xA19u
#define PPUTLIMPL_UINT_TRAIT_0x5E5u HEX, 1509u, 1509, (5, E, 5), 0xA1Au
#define PPUTLIMPL_UINT_TRAIT_0x5E4u HEX, 1508u, 1508, (5, E, 4), 0xA1Bu
#define PPUTLIMPL_UINT_TRAIT_0x5E3u HEX, 1507u, 1507, (5, E, 3), 0xA1Cu
#define PPUTLIMPL_UINT_TRAIT_0x5E2u HEX, 1506u, 1506, (5, E, 2), 0xA1Du
#define PPUTLIMPL_UINT_TRAIT_0x5E1u HEX, 1505u, 1505, (5, E, 1), 0xA1Eu
#define PPUTLIMPL_UINT_TRAIT_0x5E0u HEX, 1504u, 1504, (5, E, 0), 0xA1Fu
#define PPUTLIMPL_UINT_TRAIT_0x5DFu HEX, 1503u, 1503, (5, D, F), 0xA20u
#define PPUTLIMPL_UINT_TRAIT_0x5DEu HEX, 1502u, 1502, (5, D, E), 0xA21u
#define PPUTLIMPL_UINT_TRAIT_0x5DDu HEX, 1501u, 1501, (5, D, D), 0xA22u
#define PPUTLIMPL_UINT_TRAIT_0x5DCu HEX, 1500u, 1500, (5, D, C), 0xA23u
#define PPUTLIMPL_UINT_TRAIT_0x5DBu HEX, 1499u, 1499, (5, D, B), 0xA24u
#define PPUTLIMPL_UINT_TRAIT_0x5DAu HEX, 1498u, 1498, (5, D, A), 0xA25u
#define PPUTLIMPL_UINT_TRAIT_0x5D9u HEX, 1497u, 1497, (5, D, 9), 0xA26u
#define PPUTLIMPL_UINT_TRAIT_0x5D8u HEX, 1496u, 1496, (5, D, 8), 0xA27u
#define PPUTLIMPL_UINT_TRAIT_0x5D7u HEX, 1495u, 1495, (5, D, 7), 0xA28u
#define PPUTLIMPL_UINT_TRAIT_0x5D6u HEX, 1494u, 1494, (5, D, 6), 0xA29u
#define PPUTLIMPL_UINT_TRAIT_0x5D5u HEX, 1493u, 1493, (5, D, 5), 0xA2Au
#define PPUTLIMPL_UINT_TRAIT_0x5D4u HEX, 1492u, 1492, (5, D, 4), 0xA2Bu
#define PPUTLIMPL_UINT_TRAIT_0x5D3u HEX, 1491u, 1491, (5, D, 3), 0xA2Cu
#define PPUTLIMPL_UINT_TRAIT_0x5D2u HEX, 1490u, 1490, (5, D, 2), 0xA2Du
#define PPUTLIMPL_UINT_TRAIT_0x5D1u HEX, 1489u, 1489, (5, D, 1), 0xA2Eu
#define PPUTLIMPL_UINT_TRAIT_0x5D0u HEX, 1488u, 1488, (5, D, 0), 0xA2Fu
#define PPUTLIMPL_UINT_TRAIT_0x5CFu HEX, 1487u, 1487, (5, C, F), 0xA30u
#define PPUTLIMPL_UINT_TRAIT_0x5CEu HEX, 1486u, 1486, (5, C, E), 0xA31u
#define PPUTLIMPL_UINT_TRAIT_0x5CDu HEX, 1485u, 1485, (5, C, D), 0xA32u
#define PPUTLIMPL_UINT_TRAIT_0x5CCu HEX, 1484u, 1484, (5, C, C), 0xA33u
#define PPUTLIMPL_UINT_TRAIT_0x5CBu HEX, 1483u, 1483, (5, C, B), 0xA34u
#define PPUTLIMPL_UINT_TRAIT_0x5CAu HEX, 1482u, 1482, (5, C, A), 0xA35u
#define PPUTLIMPL_UINT_TRAIT_0x5C9u HEX, 1481u, 1481, (5, C, 9), 0xA36u
#define PPUTLIMPL_UINT_TRAIT_0x5C8u HEX, 1480u, 1480, (5, C, 8), 0xA37u
#define PPUTLIMPL_UINT_TRAIT_0x5C7u HEX, 1479u, 1479, (5, C, 7), 0xA38u
#define PPUTLIMPL_UINT_TRAIT_0x5C6u HEX, 1478u, 1478, (5, C, 6), 0xA39u
#define PPUTLIMPL_UINT_TRAIT_0x5C5u HEX, 1477u, 1477, (5, C, 5), 0xA3Au
#define PPUTLIMPL_UINT_TRAIT_0x5C4u HEX, 1476u, 1476, (5, C, 4), 0xA3Bu
#define PPUTLIMPL_UINT_TRAIT_0x5C3u HEX, 1475u, 1475, (5, C, 3), 0xA3Cu
#define PPUTLIMPL_UINT_TRAIT_0x5C2u HEX, 1474u, 1474, (5, C, 2), 0xA3Du
#define PPUTLIMPL_UINT_TRAIT_0x5C1u HEX, 1473u, 1473, (5, C, 1), 0xA3Eu
#define PPUTLIMPL_UINT_TRAIT_0x5C0u HEX, 1472u, 1472, (5, C, 0), 0xA3Fu
#define PPUTLIMPL_UINT_TRAIT_0x5BFu HEX, 1471u, 1471, (5, B, F), 0xA40u
#define PPUTLIMPL_UINT_TRAIT_0x5BEu HEX, 1470u, 1470, (5, B, E), 0xA41u
#define PPUTLIMPL_UINT_TRAIT_0x5BDu HEX, 1469u, 1469, (5, B, D), 0xA42u
#define PPUTLIMPL_UINT_TRAIT_0x5BCu HEX, 1468u, 1468, (5, B, C), 0xA43u
#define PPUTLIMPL_UINT_TRAIT_0x5BBu HEX, 1467u, 1467, (5, B, B), 0xA44u
#define PPUTLIMPL_UINT_TRAIT_0x5BAu HEX, 1466u, 1466, (5, B, A), 0xA45u
#define PPUTLIMPL_UINT_TRAIT_0x5B9u HEX, 1465u, 1465, (5, B, 9), 0xA46u
#define PPUTLIMPL_UINT_TRAIT_0x5B8u HEX, 1464u, 1464, (5, B, 8), 0xA47u
#define PPUTLIMPL_UINT_TRAIT_0x5B7u HEX, 1463u, 1463, (5, B, 7), 0xA48u
#define PPUTLIMPL_UINT_TRAIT_0x5B6u HEX, 1462u, 1462, (5, B, 6), 0xA49u
#define PPUTLIMPL_UINT_TRAIT_0x5B5u HEX, 1461u, 1461, (5, B, 5), 0xA4Au
#define PPUTLIMPL_UINT_TRAIT_0x5B4u HEX, 1460u, 1460, (5, B, 4), 0xA4Bu
#define PPUTLIMPL_UINT_TRAIT_0x5B3u HEX, 1459u, 1459, (5, B, 3), 0xA4Cu
#define PPUTLIMPL_UINT_TRAIT_0x5B2u HEX, 1458u, 1458, (5, B, 2), 0xA4Du
#define PPUTLIMPL_UINT_TRAIT_0x5B1u HEX, 1457u, 1457, (5, B, 1), 0xA4Eu
#define PPUTLIMPL_UINT_TRAIT_0x5B0u HEX, 1456u, 1456, (5, B, 0), 0xA4Fu
#define PPUTLIMPL_UINT_TRAIT_0x5AFu HEX, 1455u, 1455, (5, A, F), 0xA50u
#define PPUTLIMPL_UINT_TRAIT_0x5AEu HEX, 1454u, 1454, (5, A, E), 0xA51u
#define PPUTLIMPL_UINT_TRAIT_0x5ADu HEX, 1453u, 1453, (5, A, D), 0xA52u
#define PPUTLIMPL_UINT_TRAIT_0x5ACu HEX, 1452u, 1452, (5, A, C), 0xA53u
#define PPUTLIMPL_UINT_TRAIT_0x5ABu HEX, 1451u, 1451, (5, A, B), 0xA54u
#define PPUTLIMPL_UINT_TRAIT_0x5AAu HEX, 1450u, 1450, (5, A, A), 0xA55u
#define PPUTLIMPL_UINT_TRAIT_0x5A9u HEX, 1449u, 1449, (5, A, 9), 0xA56u
#define PPUTLIMPL_UINT_TRAIT_0x5A8u HEX, 1448u, 1448, (5, A, 8), 0xA57u
#define PPUTLIMPL_UINT_TRAIT_0x5A7u HEX, 1447u, 1447, (5, A, 7), 0xA58u
#define PPUTLIMPL_UINT_TRAIT_0x5A6u HEX, 1446u, 1446, (5, A, 6), 0xA59u
#define PPUTLIMPL_UINT_TRAIT_0x5A5u HEX, 1445u, 1445, (5, A, 5), 0xA5Au
#define PPUTLIMPL_UINT_TRAIT_0x5A4u HEX, 1444u, 1444, (5, A, 4), 0xA5Bu
#define PPUTLIMPL_UINT_TRAIT_0x5A3u HEX, 1443u, 1443, (5, A, 3), 0xA5Cu
#define PPUTLIMPL_UINT_TRAIT_0x5A2u HEX, 1442u, 1442, (5, A, 2), 0xA5Du
#define PPUTLIMPL_UINT_TRAIT_0x5A1u HEX, 1441u, 1441, (5, A, 1), 0xA5Eu
#define PPUTLIMPL_UINT_TRAIT_0x5A0u HEX, 1440u, 1440, (5, A, 0), 0xA5Fu
#define PPUTLIMPL_UINT_TRAIT_0x59Fu HEX, 1439u, 1439, (5, 9, F), 0xA60u
#define PPUTLIMPL_UINT_TRAIT_0x59Eu HEX, 1438u, 1438, (5, 9, E), 0xA61u
#define PPUTLIMPL_UINT_TRAIT_0x59Du HEX, 1437u, 1437, (5, 9, D), 0xA62u
#define PPUTLIMPL_UINT_TRAIT_0x59Cu HEX, 1436u, 1436, (5, 9, C), 0xA63u
#define PPUTLIMPL_UINT_TRAIT_0x59Bu HEX, 1435u, 1435, (5, 9, B), 0xA64u
#define PPUTLIMPL_UINT_TRAIT_0x59Au HEX, 1434u, 1434, (5, 9, A), 0xA65u
#define PPUTLIMPL_UINT_TRAIT_0x599u HEX, 1433u, 1433, (5, 9, 9), 0xA66u
#define PPUTLIMPL_UINT_TRAIT_0x598u HEX, 1432u, 1432, (5, 9, 8), 0xA67u
#define PPUTLIMPL_UINT_TRAIT_0x597u HEX, 1431u, 1431, (5, 9, 7), 0xA68u
#define PPUTLIMPL_UINT_TRAIT_0x596u HEX, 1430u, 1430, (5, 9, 6), 0xA69u
#define PPUTLIMPL_UINT_TRAIT_0x595u HEX, 1429u, 1429, (5, 9, 5), 0xA6Au
#define PPUTLIMPL_UINT_TRAIT_0x594u HEX, 1428u, 1428, (5, 9, 4), 0xA6Bu
#define PPUTLIMPL_UINT_TRAIT_0x593u HEX, 1427u, 1427, (5, 9, 3), 0xA6Cu
#define PPUTLIMPL_UINT_TRAIT_0x592u HEX, 1426u, 1426, (5, 9, 2), 0xA6Du
#define PPUTLIMPL_UINT_TRAIT_0x591u HEX, 1425u, 1425, (5, 9, 1), 0xA6Eu
#define PPUTLIMPL_UINT_TRAIT_0x590u HEX, 1424u, 1424, (5, 9, 0), 0xA6Fu
#define PPUTLIMPL_UINT_TRAIT_0x58Fu HEX, 1423u, 1423, (5, 8, F), 0xA70u
#define PPUTLIMPL_UINT_TRAIT_0x58Eu HEX, 1422u, 1422, (5, 8, E), 0xA71u
#define PPUTLIMPL_UINT_TRAIT_0x58Du HEX, 1421u, 1421, (5, 8, D), 0xA72u
#define PPUTLIMPL_UINT_TRAIT_0x58Cu HEX, 1420u, 1420, (5, 8, C), 0xA73u
#define PPUTLIMPL_UINT_TRAIT_0x58Bu HEX, 1419u, 1419, (5, 8, B), 0xA74u
#define PPUTLIMPL_UINT_TRAIT_0x58Au HEX, 1418u, 1418, (5, 8, A), 0xA75u
#define PPUTLIMPL_UINT_TRAIT_0x589u HEX, 1417u, 1417, (5, 8, 9), 0xA76u
#define PPUTLIMPL_UINT_TRAIT_0x588u HEX, 1416u, 1416, (5, 8, 8), 0xA77u
#define PPUTLIMPL_UINT_TRAIT_0x587u HEX, 1415u, 1415, (5, 8, 7), 0xA78u
#define PPUTLIMPL_UINT_TRAIT_0x586u HEX, 1414u, 1414, (5, 8, 6), 0xA79u
#define PPUTLIMPL_UINT_TRAIT_0x585u HEX, 1413u, 1413, (5, 8, 5), 0xA7Au
#define PPUTLIMPL_UINT_TRAIT_0x584u HEX, 1412u, 1412, (5, 8, 4), 0xA7Bu
#define PPUTLIMPL_UINT_TRAIT_0x583u HEX, 1411u, 1411, (5, 8, 3), 0xA7Cu
#define PPUTLIMPL_UINT_TRAIT_0x582u HEX, 1410u, 1410, (5, 8, 2), 0xA7Du
#define PPUTLIMPL_UINT_TRAIT_0x581u HEX, 1409u, 1409, (5, 8, 1), 0xA7Eu
#define PPUTLIMPL_UINT_TRAIT_0x580u HEX, 1408u, 1408, (5, 8, 0), 0xA7Fu
#define PPUTLIMPL_UINT_TRAIT_0x57Fu HEX, 1407u, 1407, (5, 7, F), 0xA80u
#define PPUTLIMPL_UINT_TRAIT_0x57Eu HEX, 1406u, 1406, (5, 7, E), 0xA81u
#define PPUTLIMPL_UINT_TRAIT_0x57Du HEX, 1405u, 1405, (5, 7, D), 0xA82u
#define PPUTLIMPL_UINT_TRAIT_0x57Cu HEX, 1404u, 1404, (5, 7, C), 0xA83u
#define PPUTLIMPL_UINT_TRAIT_0x57Bu HEX, 1403u, 1403, (5, 7, B), 0xA84u
#define PPUTLIMPL_UINT_TRAIT_0x57Au HEX, 1402u, 1402, (5, 7, A), 0xA85u
#define PPUTLIMPL_UINT_TRAIT_0x579u HEX, 1401u, 1401, (5, 7, 9), 0xA86u
#define PPUTLIMPL_UINT_TRAIT_0x578u HEX, 1400u, 1400, (5, 7, 8), 0xA87u
#define PPUTLIMPL_UINT_TRAIT_0x577u HEX, 1399u, 1399, (5, 7, 7), 0xA88u
#define PPUTLIMPL_UINT_TRAIT_0x576u HEX, 1398u, 1398, (5, 7, 6), 0xA89u
#define PPUTLIMPL_UINT_TRAIT_0x575u HEX, 1397u, 1397, (5, 7, 5), 0xA8Au
#define PPUTLIMPL_UINT_TRAIT_0x574u HEX, 1396u, 1396, (5, 7, 4), 0xA8Bu
#define PPUTLIMPL_UINT_TRAIT_0x573u HEX, 1395u, 1395, (5, 7, 3), 0xA8Cu
#define PPUTLIMPL_UINT_TRAIT_0x572u HEX, 1394u, 1394, (5, 7, 2), 0xA8Du
#define PPUTLIMPL_UINT_TRAIT_0x571u HEX, 1393u, 1393, (5, 7, 1), 0xA8Eu
#define PPUTLIMPL_UINT_TRAIT_0x570u HEX, 1392u, 1392, (5, 7, 0), 0xA8Fu
#define PPUTLIMPL_UINT_TRAIT_0x56Fu HEX, 1391u, 1391, (5, 6, F), 0xA90u
#define PPUTLIMPL_UINT_TRAIT_0x56Eu HEX, 1390u, 1390, (5, 6, E), 0xA91u
#define PPUTLIMPL_UINT_TRAIT_0x56Du HEX, 1389u, 1389, (5, 6, D), 0xA92u
#define PPUTLIMPL_UINT_TRAIT_0x56Cu HEX, 1388u, 1388, (5, 6, C), 0xA93u
#define PPUTLIMPL_UINT_TRAIT_0x56Bu HEX, 1387u, 1387, (5, 6, B), 0xA94u
#define PPUTLIMPL_UINT_TRAIT_0x56Au HEX, 1386u, 1386, (5, 6, A), 0xA95u
#define PPUTLIMPL_UINT_TRAIT_0x569u HEX, 1385u, 1385, (5, 6, 9), 0xA96u
#define PPUTLIMPL_UINT_TRAIT_0x568u HEX, 1384u, 1384, (5, 6, 8), 0xA97u
#define PPUTLIMPL_UINT_TRAIT_0x567u HEX, 1383u, 1383, (5, 6, 7), 0xA98u
#define PPUTLIMPL_UINT_TRAIT_0x566u HEX, 1382u, 1382, (5, 6, 6), 0xA99u
#define PPUTLIMPL_UINT_TRAIT_0x565u HEX, 1381u, 1381, (5, 6, 5), 0xA9Au
#define PPUTLIMPL_UINT_TRAIT_0x564u HEX, 1380u, 1380, (5, 6, 4), 0xA9Bu
#define PPUTLIMPL_UINT_TRAIT_0x563u HEX, 1379u, 1379, (5, 6, 3), 0xA9Cu
#define PPUTLIMPL_UINT_TRAIT_0x562u HEX, 1378u, 1378, (5, 6, 2), 0xA9Du
#define PPUTLIMPL_UINT_TRAIT_0x561u HEX, 1377u, 1377, (5, 6, 1), 0xA9Eu
#define PPUTLIMPL_UINT_TRAIT_0x560u HEX, 1376u, 1376, (5, 6, 0), 0xA9Fu
#define PPUTLIMPL_UINT_TRAIT_0x55Fu HEX, 1375u, 1375, (5, 5, F), 0xAA0u
#define PPUTLIMPL_UINT_TRAIT_0x55Eu HEX, 1374u, 1374, (5, 5, E), 0xAA1u
#define PPUTLIMPL_UINT_TRAIT_0x55Du HEX, 1373u, 1373, (5, 5, D), 0xAA2u
#define PPUTLIMPL_UINT_TRAIT_0x55Cu HEX, 1372u, 1372, (5, 5, C), 0xAA3u
#define PPUTLIMPL_UINT_TRAIT_0x55Bu HEX, 1371u, 1371, (5, 5, B), 0xAA4u
#define PPUTLIMPL_UINT_TRAIT_0x55Au HEX, 1370u, 1370, (5, 5, A), 0xAA5u
#define PPUTLIMPL_UINT_TRAIT_0x559u HEX, 1369u, 1369, (5, 5, 9), 0xAA6u
#define PPUTLIMPL_UINT_TRAIT_0x558u HEX, 1368u, 1368, (5, 5, 8), 0xAA7u
#define PPUTLIMPL_UINT_TRAIT_0x557u HEX, 1367u, 1367, (5, 5, 7), 0xAA8u
#define PPUTLIMPL_UINT_TRAIT_0x556u HEX, 1366u, 1366, (5, 5, 6), 0xAA9u
#define PPUTLIMPL_UINT_TRAIT_0x555u HEX, 1365u, 1365, (5, 5, 5), 0xAAAu
#define PPUTLIMPL_UINT_TRAIT_0x554u HEX, 1364u, 1364, (5, 5, 4), 0xAABu
#define PPUTLIMPL_UINT_TRAIT_0x553u HEX, 1363u, 1363, (5, 5, 3), 0xAACu
#define PPUTLIMPL_UINT_TRAIT_0x552u HEX, 1362u, 1362, (5, 5, 2), 0xAADu
#define PPUTLIMPL_UINT_TRAIT_0x551u HEX, 1361u, 1361, (5, 5, 1), 0xAAEu
#define PPUTLIMPL_UINT_TRAIT_0x550u HEX, 1360u, 1360, (5, 5, 0), 0xAAFu
#define PPUTLIMPL_UINT_TRAIT_0x54Fu HEX, 1359u, 1359, (5, 4, F), 0xAB0u
#define PPUTLIMPL_UINT_TRAIT_0x54Eu HEX, 1358u, 1358, (5, 4, E), 0xAB1u
#define PPUTLIMPL_UINT_TRAIT_0x54Du HEX, 1357u, 1357, (5, 4, D), 0xAB2u
#define PPUTLIMPL_UINT_TRAIT_0x54Cu HEX, 1356u, 1356, (5, 4, C), 0xAB3u
#define PPUTLIMPL_UINT_TRAIT_0x54Bu HEX, 1355u, 1355, (5, 4, B), 0xAB4u
#define PPUTLIMPL_UINT_TRAIT_0x54Au HEX, 1354u, 1354, (5, 4, A), 0xAB5u
#define PPUTLIMPL_UINT_TRAIT_0x549u HEX, 1353u, 1353, (5, 4, 9), 0xAB6u
#define PPUTLIMPL_UINT_TRAIT_0x548u HEX, 1352u, 1352, (5, 4, 8), 0xAB7u
#define PPUTLIMPL_UINT_TRAIT_0x547u HEX, 1351u, 1351, (5, 4, 7), 0xAB8u
#define PPUTLIMPL_UINT_TRAIT_0x546u HEX, 1350u, 1350, (5, 4, 6), 0xAB9u
#define PPUTLIMPL_UINT_TRAIT_0x545u HEX, 1349u, 1349, (5, 4, 5), 0xABAu
#define PPUTLIMPL_UINT_TRAIT_0x544u HEX, 1348u, 1348, (5, 4, 4), 0xABBu
#define PPUTLIMPL_UINT_TRAIT_0x543u HEX, 1347u, 1347, (5, 4, 3), 0xABCu
#define PPUTLIMPL_UINT_TRAIT_0x542u HEX, 1346u, 1346, (5, 4, 2), 0xABDu
#define PPUTLIMPL_UINT_TRAIT_0x541u HEX, 1345u, 1345, (5, 4, 1), 0xABEu
#define PPUTLIMPL_UINT_TRAIT_0x540u HEX, 1344u, 1344, (5, 4, 0), 0xABFu
#define PPUTLIMPL_UINT_TRAIT_0x53Fu HEX, 1343u, 1343, (5, 3, F), 0xAC0u
#define PPUTLIMPL_UINT_TRAIT_0x53Eu HEX, 1342u, 1342, (5, 3, E), 0xAC1u
#define PPUTLIMPL_UINT_TRAIT_0x53Du HEX, 1341u, 1341, (5, 3, D), 0xAC2u
#define PPUTLIMPL_UINT_TRAIT_0x53Cu HEX, 1340u, 1340, (5, 3, C), 0xAC3u
#define PPUTLIMPL_UINT_TRAIT_0x53Bu HEX, 1339u, 1339, (5, 3, B), 0xAC4u
#define PPUTLIMPL_UINT_TRAIT_0x53Au HEX, 1338u, 1338, (5, 3, A), 0xAC5u
#define PPUTLIMPL_UINT_TRAIT_0x539u HEX, 1337u, 1337, (5, 3, 9), 0xAC6u
#define PPUTLIMPL_UINT_TRAIT_0x538u HEX, 1336u, 1336, (5, 3, 8), 0xAC7u
#define PPUTLIMPL_UINT_TRAIT_0x537u HEX, 1335u, 1335, (5, 3, 7), 0xAC8u
#define PPUTLIMPL_UINT_TRAIT_0x536u HEX, 1334u, 1334, (5, 3, 6), 0xAC9u
#define PPUTLIMPL_UINT_TRAIT_0x535u HEX, 1333u, 1333, (5, 3, 5), 0xACAu
#define PPUTLIMPL_UINT_TRAIT_0x534u HEX, 1332u, 1332, (5, 3, 4), 0xACBu
#define PPUTLIMPL_UINT_TRAIT_0x533u HEX, 1331u, 1331, (5, 3, 3), 0xACCu
#define PPUTLIMPL_UINT_TRAIT_0x532u HEX, 1330u, 1330, (5, 3, 2), 0xACDu
#define PPUTLIMPL_UINT_TRAIT_0x531u HEX, 1329u, 1329, (5, 3, 1), 0xACEu
#define PPUTLIMPL_UINT_TRAIT_0x530u HEX, 1328u, 1328, (5, 3, 0), 0xACFu
#define PPUTLIMPL_UINT_TRAIT_0x52Fu HEX, 1327u, 1327, (5, 2, F), 0xAD0u
#define PPUTLIMPL_UINT_TRAIT_0x52Eu HEX, 1326u, 1326, (5, 2, E), 0xAD1u
#define PPUTLIMPL_UINT_TRAIT_0x52Du HEX, 1325u, 1325, (5, 2, D), 0xAD2u
#define PPUTLIMPL_UINT_TRAIT_0x52Cu HEX, 1324u, 1324, (5, 2, C), 0xAD3u
#define PPUTLIMPL_UINT_TRAIT_0x52Bu HEX, 1323u, 1323, (5, 2, B), 0xAD4u
#define PPUTLIMPL_UINT_TRAIT_0x52Au HEX, 1322u, 1322, (5, 2, A), 0xAD5u
#define PPUTLIMPL_UINT_TRAIT_0x529u HEX, 1321u, 1321, (5, 2, 9), 0xAD6u
#define PPUTLIMPL_UINT_TRAIT_0x528u HEX, 1320u, 1320, (5, 2, 8), 0xAD7u
#define PPUTLIMPL_UINT_TRAIT_0x527u HEX, 1319u, 1319, (5, 2, 7), 0xAD8u
#define PPUTLIMPL_UINT_TRAIT_0x526u HEX, 1318u, 1318, (5, 2, 6), 0xAD9u
#define PPUTLIMPL_UINT_TRAIT_0x525u HEX, 1317u, 1317, (5, 2, 5), 0xADAu
#define PPUTLIMPL_UINT_TRAIT_0x524u HEX, 1316u, 1316, (5, 2, 4), 0xADBu
#define PPUTLIMPL_UINT_TRAIT_0x523u HEX, 1315u, 1315, (5, 2, 3), 0xADCu
#define PPUTLIMPL_UINT_TRAIT_0x522u HEX, 1314u, 1314, (5, 2, 2), 0xADDu
#define PPUTLIMPL_UINT_TRAIT_0x521u HEX, 1313u, 1313, (5, 2, 1), 0xADEu
#define PPUTLIMPL_UINT_TRAIT_0x520u HEX, 1312u, 1312, (5, 2, 0), 0xADFu
#define PPUTLIMPL_UINT_TRAIT_0x51Fu HEX, 1311u, 1311, (5, 1, F), 0xAE0u
#define PPUTLIMPL_UINT_TRAIT_0x51Eu HEX, 1310u, 1310, (5, 1, E), 0xAE1u
#define PPUTLIMPL_UINT_TRAIT_0x51Du HEX, 1309u, 1309, (5, 1, D), 0xAE2u
#define PPUTLIMPL_UINT_TRAIT_0x51Cu HEX, 1308u, 1308, (5, 1, C), 0xAE3u
#define PPUTLIMPL_UINT_TRAIT_0x51Bu HEX, 1307u, 1307, (5, 1, B), 0xAE4u
#define PPUTLIMPL_UINT_TRAIT_0x51Au HEX, 1306u, 1306, (5, 1, A), 0xAE5u
#define PPUTLIMPL_UINT_TRAIT_0x519u HEX, 1305u, 1305, (5, 1, 9), 0xAE6u
#define PPUTLIMPL_UINT_TRAIT_0x518u HEX, 1304u, 1304, (5, 1, 8), 0xAE7u
#define PPUTLIMPL_UINT_TRAIT_0x517u HEX, 1303u, 1303, (5, 1, 7), 0xAE8u
#define PPUTLIMPL_UINT_TRAIT_0x516u HEX, 1302u, 1302, (5, 1, 6), 0xAE9u
#define PPUTLIMPL_UINT_TRAIT_0x515u HEX, 1301u, 1301, (5, 1, 5), 0xAEAu
#define PPUTLIMPL_UINT_TRAIT_0x514u HEX, 1300u, 1300, (5, 1, 4), 0xAEBu
#define PPUTLIMPL_UINT_TRAIT_0x513u HEX, 1299u, 1299, (5, 1, 3), 0xAECu
#define PPUTLIMPL_UINT_TRAIT_0x512u HEX, 1298u, 1298, (5, 1, 2), 0xAEDu
#define PPUTLIMPL_UINT_TRAIT_0x511u HEX, 1297u, 1297, (5, 1, 1), 0xAEEu
#define PPUTLIMPL_UINT_TRAIT_0x510u HEX, 1296u, 1296, (5, 1, 0), 0xAEFu
#define PPUTLIMPL_UINT_TRAIT_0x50Fu HEX, 1295u, 1295, (5, 0, F), 0xAF0u
#define PPUTLIMPL_UINT_TRAIT_0x50Eu HEX, 1294u, 1294, (5, 0, E), 0xAF1u
#define PPUTLIMPL_UINT_TRAIT_0x50Du HEX, 1293u, 1293, (5, 0, D), 0xAF2u
#define PPUTLIMPL_UINT_TRAIT_0x50Cu HEX, 1292u, 1292, (5, 0, C), 0xAF3u
#define PPUTLIMPL_UINT_TRAIT_0x50Bu HEX, 1291u, 1291, (5, 0, B), 0xAF4u
#define PPUTLIMPL_UINT_TRAIT_0x50Au HEX, 1290u, 1290, (5, 0, A), 0xAF5u
#define PPUTLIMPL_UINT_TRAIT_0x509u HEX, 1289u, 1289, (5, 0, 9), 0xAF6u
#define PPUTLIMPL_UINT_TRAIT_0x508u HEX, 1288u, 1288, (5, 0, 8), 0xAF7u
#define PPUTLIMPL_UINT_TRAIT_0x507u HEX, 1287u, 1287, (5, 0, 7), 0xAF8u
#define PPUTLIMPL_UINT_TRAIT_0x506u HEX, 1286u, 1286, (5, 0, 6), 0xAF9u
#define PPUTLIMPL_UINT_TRAIT_0x505u HEX, 1285u, 1285, (5, 0, 5), 0xAFAu
#define PPUTLIMPL_UINT_TRAIT_0x504u HEX, 1284u, 1284, (5, 0, 4), 0xAFBu
#define PPUTLIMPL_UINT_TRAIT_0x503u HEX, 1283u, 1283, (5, 0, 3), 0xAFCu
#define PPUTLIMPL_UINT_TRAIT_0x502u HEX, 1282u, 1282, (5, 0, 2), 0xAFDu
#define PPUTLIMPL_UINT_TRAIT_0x501u HEX, 1281u, 1281, (5, 0, 1), 0xAFEu
#define PPUTLIMPL_UINT_TRAIT_0x500u HEX, 1280u, 1280, (5, 0, 0), 0xAFFu
#define PPUTLIMPL_UINT_TRAIT_0x4FFu HEX, 1279u, 1279, (4, F, F), 0xB00u
#define PPUTLIMPL_UINT_TRAIT_0x4FEu HEX, 1278u, 1278, (4, F, E), 0xB01u
#define PPUTLIMPL_UINT_TRAIT_0x4FDu HEX, 1277u, 1277, (4, F, D), 0xB02u
#define PPUTLIMPL_UINT_TRAIT_0x4FCu HEX, 1276u, 1276, (4, F, C), 0xB03u
#define PPUTLIMPL_UINT_TRAIT_0x4FBu HEX, 1275u, 1275, (4, F, B), 0xB04u
#define PPUTLIMPL_UINT_TRAIT_0x4FAu HEX, 1274u, 1274, (4, F, A), 0xB05u
#define PPUTLIMPL_UINT_TRAIT_0x4F9u HEX, 1273u, 1273, (4, F, 9), 0xB06u
#define PPUTLIMPL_UINT_TRAIT_0x4F8u HEX, 1272u, 1272, (4, F, 8), 0xB07u
#define PPUTLIMPL_UINT_TRAIT_0x4F7u HEX, 1271u, 1271, (4, F, 7), 0xB08u
#define PPUTLIMPL_UINT_TRAIT_0x4F6u HEX, 1270u, 1270, (4, F, 6), 0xB09u
#define PPUTLIMPL_UINT_TRAIT_0x4F5u HEX, 1269u, 1269, (4, F, 5), 0xB0Au
#define PPUTLIMPL_UINT_TRAIT_0x4F4u HEX, 1268u, 1268, (4, F, 4), 0xB0Bu
#define PPUTLIMPL_UINT_TRAIT_0x4F3u HEX, 1267u, 1267, (4, F, 3), 0xB0Cu
#define PPUTLIMPL_UINT_TRAIT_0x4F2u HEX, 1266u, 1266, (4, F, 2), 0xB0Du
#define PPUTLIMPL_UINT_TRAIT_0x4F1u HEX, 1265u, 1265, (4, F, 1), 0xB0Eu
#define PPUTLIMPL_UINT_TRAIT_0x4F0u HEX, 1264u, 1264, (4, F, 0), 0xB0Fu
#define PPUTLIMPL_UINT_TRAIT_0x4EFu HEX, 1263u, 1263, (4, E, F), 0xB10u
#define PPUTLIMPL_UINT_TRAIT_0x4EEu HEX, 1262u, 1262, (4, E, E), 0xB11u
#define PPUTLIMPL_UINT_TRAIT_0x4EDu HEX, 1261u, 1261, (4, E, D), 0xB12u
#define PPUTLIMPL_UINT_TRAIT_0x4ECu HEX, 1260u, 1260, (4, E, C), 0xB13u
#define PPUTLIMPL_UINT_TRAIT_0x4EBu HEX, 1259u, 1259, (4, E, B), 0xB14u
#define PPUTLIMPL_UINT_TRAIT_0x4EAu HEX, 1258u, 1258, (4, E, A), 0xB15u
#define PPUTLIMPL_UINT_TRAIT_0x4E9u HEX, 1257u, 1257, (4, E, 9), 0xB16u
#define PPUTLIMPL_UINT_TRAIT_0x4E8u HEX, 1256u, 1256, (4, E, 8), 0xB17u
#define PPUTLIMPL_UINT_TRAIT_0x4E7u HEX, 1255u, 1255, (4, E, 7), 0xB18u
#define PPUTLIMPL_UINT_TRAIT_0x4E6u HEX, 1254u, 1254, (4, E, 6), 0xB19u
#define PPUTLIMPL_UINT_TRAIT_0x4E5u HEX, 1253u, 1253, (4, E, 5), 0xB1Au
#define PPUTLIMPL_UINT_TRAIT_0x4E4u HEX, 1252u, 1252, (4, E, 4), 0xB1Bu
#define PPUTLIMPL_UINT_TRAIT_0x4E3u HEX, 1251u, 1251, (4, E, 3), 0xB1Cu
#define PPUTLIMPL_UINT_TRAIT_0x4E2u HEX, 1250u, 1250, (4, E, 2), 0xB1Du
#define PPUTLIMPL_UINT_TRAIT_0x4E1u HEX, 1249u, 1249, (4, E, 1), 0xB1Eu
#define PPUTLIMPL_UINT_TRAIT_0x4E0u HEX, 1248u, 1248, (4, E, 0), 0xB1Fu
#define PPUTLIMPL_UINT_TRAIT_0x4DFu HEX, 1247u, 1247, (4, D, F), 0xB20u
#define PPUTLIMPL_UINT_TRAIT_0x4DEu HEX, 1246u, 1246, (4, D, E), 0xB21u
#define PPUTLIMPL_UINT_TRAIT_0x4DDu HEX, 1245u, 1245, (4, D, D), 0xB22u
#define PPUTLIMPL_UINT_TRAIT_0x4DCu HEX, 1244u, 1244, (4, D, C), 0xB23u
#define PPUTLIMPL_UINT_TRAIT_0x4DBu HEX, 1243u, 1243, (4, D, B), 0xB24u
#define PPUTLIMPL_UINT_TRAIT_0x4DAu HEX, 1242u, 1242, (4, D, A), 0xB25u
#define PPUTLIMPL_UINT_TRAIT_0x4D9u HEX, 1241u, 1241, (4, D, 9), 0xB26u
#define PPUTLIMPL_UINT_TRAIT_0x4D8u HEX, 1240u, 1240, (4, D, 8), 0xB27u
#define PPUTLIMPL_UINT_TRAIT_0x4D7u HEX, 1239u, 1239, (4, D, 7), 0xB28u
#define PPUTLIMPL_UINT_TRAIT_0x4D6u HEX, 1238u, 1238, (4, D, 6), 0xB29u
#define PPUTLIMPL_UINT_TRAIT_0x4D5u HEX, 1237u, 1237, (4, D, 5), 0xB2Au
#define PPUTLIMPL_UINT_TRAIT_0x4D4u HEX, 1236u, 1236, (4, D, 4), 0xB2Bu
#define PPUTLIMPL_UINT_TRAIT_0x4D3u HEX, 1235u, 1235, (4, D, 3), 0xB2Cu
#define PPUTLIMPL_UINT_TRAIT_0x4D2u HEX, 1234u, 1234, (4, D, 2), 0xB2Du
#define PPUTLIMPL_UINT_TRAIT_0x4D1u HEX, 1233u, 1233, (4, D, 1), 0xB2Eu
#define PPUTLIMPL_UINT_TRAIT_0x4D0u HEX, 1232u, 1232, (4, D, 0), 0xB2Fu
#define PPUTLIMPL_UINT_TRAIT_0x4CFu HEX, 1231u, 1231, (4, C, F), 0xB30u
#define PPUTLIMPL_UINT_TRAIT_0x4CEu HEX, 1230u, 1230, (4, C, E), 0xB31u
#define PPUTLIMPL_UINT_TRAIT_0x4CDu HEX, 1229u, 1229, (4, C, D), 0xB32u
#define PPUTLIMPL_UINT_TRAIT_0x4CCu HEX, 1228u, 1228, (4, C, C), 0xB33u
#define PPUTLIMPL_UINT_TRAIT_0x4CBu HEX, 1227u, 1227, (4, C, B), 0xB34u
#define PPUTLIMPL_UINT_TRAIT_0x4CAu HEX, 1226u, 1226, (4, C, A), 0xB35u
#define PPUTLIMPL_UINT_TRAIT_0x4C9u HEX, 1225u, 1225, (4, C, 9), 0xB36u
#define PPUTLIMPL_UINT_TRAIT_0x4C8u HEX, 1224u, 1224, (4, C, 8), 0xB37u
#define PPUTLIMPL_UINT_TRAIT_0x4C7u HEX, 1223u, 1223, (4, C, 7), 0xB38u
#define PPUTLIMPL_UINT_TRAIT_0x4C6u HEX, 1222u, 1222, (4, C, 6), 0xB39u
#define PPUTLIMPL_UINT_TRAIT_0x4C5u HEX, 1221u, 1221, (4, C, 5), 0xB3Au
#define PPUTLIMPL_UINT_TRAIT_0x4C4u HEX, 1220u, 1220, (4, C, 4), 0xB3Bu
#define PPUTLIMPL_UINT_TRAIT_0x4C3u HEX, 1219u, 1219, (4, C, 3), 0xB3Cu
#define PPUTLIMPL_UINT_TRAIT_0x4C2u HEX, 1218u, 1218, (4, C, 2), 0xB3Du
#define PPUTLIMPL_UINT_TRAIT_0x4C1u HEX, 1217u, 1217, (4, C, 1), 0xB3Eu
#define PPUTLIMPL_UINT_TRAIT_0x4C0u HEX, 1216u, 1216, (4, C, 0), 0xB3Fu
#define PPUTLIMPL_UINT_TRAIT_0x4BFu HEX, 1215u, 1215, (4, B, F), 0xB40u
#define PPUTLIMPL_UINT_TRAIT_0x4BEu HEX, 1214u, 1214, (4, B, E), 0xB41u
#define PPUTLIMPL_UINT_TRAIT_0x4BDu HEX, 1213u, 1213, (4, B, D), 0xB42u
#define PPUTLIMPL_UINT_TRAIT_0x4BCu HEX, 1212u, 1212, (4, B, C), 0xB43u
#define PPUTLIMPL_UINT_TRAIT_0x4BBu HEX, 1211u, 1211, (4, B, B), 0xB44u
#define PPUTLIMPL_UINT_TRAIT_0x4BAu HEX, 1210u, 1210, (4, B, A), 0xB45u
#define PPUTLIMPL_UINT_TRAIT_0x4B9u HEX, 1209u, 1209, (4, B, 9), 0xB46u
#define PPUTLIMPL_UINT_TRAIT_0x4B8u HEX, 1208u, 1208, (4, B, 8), 0xB47u
#define PPUTLIMPL_UINT_TRAIT_0x4B7u HEX, 1207u, 1207, (4, B, 7), 0xB48u
#define PPUTLIMPL_UINT_TRAIT_0x4B6u HEX, 1206u, 1206, (4, B, 6), 0xB49u
#define PPUTLIMPL_UINT_TRAIT_0x4B5u HEX, 1205u, 1205, (4, B, 5), 0xB4Au
#define PPUTLIMPL_UINT_TRAIT_0x4B4u HEX, 1204u, 1204, (4, B, 4), 0xB4Bu
#define PPUTLIMPL_UINT_TRAIT_0x4B3u HEX, 1203u, 1203, (4, B, 3), 0xB4Cu
#define PPUTLIMPL_UINT_TRAIT_0x4B2u HEX, 1202u, 1202, (4, B, 2), 0xB4Du
#define PPUTLIMPL_UINT_TRAIT_0x4B1u HEX, 1201u, 1201, (4, B, 1), 0xB4Eu
#define PPUTLIMPL_UINT_TRAIT_0x4B0u HEX, 1200u, 1200, (4, B, 0), 0xB4Fu
#define PPUTLIMPL_UINT_TRAIT_0x4AFu HEX, 1199u, 1199, (4, A, F), 0xB50u
#define PPUTLIMPL_UINT_TRAIT_0x4AEu HEX, 1198u, 1198, (4, A, E), 0xB51u
#define PPUTLIMPL_UINT_TRAIT_0x4ADu HEX, 1197u, 1197, (4, A, D), 0xB52u
#define PPUTLIMPL_UINT_TRAIT_0x4ACu HEX, 1196u, 1196, (4, A, C), 0xB53u
#define PPUTLIMPL_UINT_TRAIT_0x4ABu HEX, 1195u, 1195, (4, A, B), 0xB54u
#define PPUTLIMPL_UINT_TRAIT_0x4AAu HEX, 1194u, 1194, (4, A, A), 0xB55u
#define PPUTLIMPL_UINT_TRAIT_0x4A9u HEX, 1193u, 1193, (4, A, 9), 0xB56u
#define PPUTLIMPL_UINT_TRAIT_0x4A8u HEX, 1192u, 1192, (4, A, 8), 0xB57u
#define PPUTLIMPL_UINT_TRAIT_0x4A7u HEX, 1191u, 1191, (4, A, 7), 0xB58u
#define PPUTLIMPL_UINT_TRAIT_0x4A6u HEX, 1190u, 1190, (4, A, 6), 0xB59u
#define PPUTLIMPL_UINT_TRAIT_0x4A5u HEX, 1189u, 1189, (4, A, 5), 0xB5Au
#define PPUTLIMPL_UINT_TRAIT_0x4A4u HEX, 1188u, 1188, (4, A, 4), 0xB5Bu
#define PPUTLIMPL_UINT_TRAIT_0x4A3u HEX, 1187u, 1187, (4, A, 3), 0xB5Cu
#define PPUTLIMPL_UINT_TRAIT_0x4A2u HEX, 1186u, 1186, (4, A, 2), 0xB5Du
#define PPUTLIMPL_UINT_TRAIT_0x4A1u HEX, 1185u, 1185, (4, A, 1), 0xB5Eu
#define PPUTLIMPL_UINT_TRAIT_0x4A0u HEX, 1184u, 1184, (4, A, 0), 0xB5Fu
#define PPUTLIMPL_UINT_TRAIT_0x49Fu HEX, 1183u, 1183, (4, 9, F), 0xB60u
#define PPUTLIMPL_UINT_TRAIT_0x49Eu HEX, 1182u, 1182, (4, 9, E), 0xB61u
#define PPUTLIMPL_UINT_TRAIT_0x49Du HEX, 1181u, 1181, (4, 9, D), 0xB62u
#define PPUTLIMPL_UINT_TRAIT_0x49Cu HEX, 1180u, 1180, (4, 9, C), 0xB63u
#define PPUTLIMPL_UINT_TRAIT_0x49Bu HEX, 1179u, 1179, (4, 9, B), 0xB64u
#define PPUTLIMPL_UINT_TRAIT_0x49Au HEX, 1178u, 1178, (4, 9, A), 0xB65u
#define PPUTLIMPL_UINT_TRAIT_0x499u HEX, 1177u, 1177, (4, 9, 9), 0xB66u
#define PPUTLIMPL_UINT_TRAIT_0x498u HEX, 1176u, 1176, (4, 9, 8), 0xB67u
#define PPUTLIMPL_UINT_TRAIT_0x497u HEX, 1175u, 1175, (4, 9, 7), 0xB68u
#define PPUTLIMPL_UINT_TRAIT_0x496u HEX, 1174u, 1174, (4, 9, 6), 0xB69u
#define PPUTLIMPL_UINT_TRAIT_0x495u HEX, 1173u, 1173, (4, 9, 5), 0xB6Au
#define PPUTLIMPL_UINT_TRAIT_0x494u HEX, 1172u, 1172, (4, 9, 4), 0xB6Bu
#define PPUTLIMPL_UINT_TRAIT_0x493u HEX, 1171u, 1171, (4, 9, 3), 0xB6Cu
#define PPUTLIMPL_UINT_TRAIT_0x492u HEX, 1170u, 1170, (4, 9, 2), 0xB6Du
#define PPUTLIMPL_UINT_TRAIT_0x491u HEX, 1169u, 1169, (4, 9, 1), 0xB6Eu
#define PPUTLIMPL_UINT_TRAIT_0x490u HEX, 1168u, 1168, (4, 9, 0), 0xB6Fu
#define PPUTLIMPL_UINT_TRAIT_0x48Fu HEX, 1167u, 1167, (4, 8, F), 0xB70u
#define PPUTLIMPL_UINT_TRAIT_0x48Eu HEX, 1166u, 1166, (4, 8, E), 0xB71u
#define PPUTLIMPL_UINT_TRAIT_0x48Du HEX, 1165u, 1165, (4, 8, D), 0xB72u
#define PPUTLIMPL_UINT_TRAIT_0x48Cu HEX, 1164u, 1164, (4, 8, C), 0xB73u
#define PPUTLIMPL_UINT_TRAIT_0x48Bu HEX, 1163u, 1163, (4, 8, B), 0xB74u
#define PPUTLIMPL_UINT_TRAIT_0x48Au HEX, 1162u, 1162, (4, 8, A), 0xB75u
#define PPUTLIMPL_UINT_TRAIT_0x489u HEX, 1161u, 1161, (4, 8, 9), 0xB76u
#define PPUTLIMPL_UINT_TRAIT_0x488u HEX, 1160u, 1160, (4, 8, 8), 0xB77u
#define PPUTLIMPL_UINT_TRAIT_0x487u HEX, 1159u, 1159, (4, 8, 7), 0xB78u
#define PPUTLIMPL_UINT_TRAIT_0x486u HEX, 1158u, 1158, (4, 8, 6), 0xB79u
#define PPUTLIMPL_UINT_TRAIT_0x485u HEX, 1157u, 1157, (4, 8, 5), 0xB7Au
#define PPUTLIMPL_UINT_TRAIT_0x484u HEX, 1156u, 1156, (4, 8, 4), 0xB7Bu
#define PPUTLIMPL_UINT_TRAIT_0x483u HEX, 1155u, 1155, (4, 8, 3), 0xB7Cu
#define PPUTLIMPL_UINT_TRAIT_0x482u HEX, 1154u, 1154, (4, 8, 2), 0xB7Du
#define PPUTLIMPL_UINT_TRAIT_0x481u HEX, 1153u, 1153, (4, 8, 1), 0xB7Eu
#define PPUTLIMPL_UINT_TRAIT_0x480u HEX, 1152u, 1152, (4, 8, 0), 0xB7Fu
#define PPUTLIMPL_UINT_TRAIT_0x47Fu HEX, 1151u, 1151, (4, 7, F), 0xB80u
#define PPUTLIMPL_UINT_TRAIT_0x47Eu HEX, 1150u, 1150, (4, 7, E), 0xB81u
#define PPUTLIMPL_UINT_TRAIT_0x47Du HEX, 1149u, 1149, (4, 7, D), 0xB82u
#define PPUTLIMPL_UINT_TRAIT_0x47Cu HEX, 1148u, 1148, (4, 7, C), 0xB83u
#define PPUTLIMPL_UINT_TRAIT_0x47Bu HEX, 1147u, 1147, (4, 7, B), 0xB84u
#define PPUTLIMPL_UINT_TRAIT_0x47Au HEX, 1146u, 1146, (4, 7, A), 0xB85u
#define PPUTLIMPL_UINT_TRAIT_0x479u HEX, 1145u, 1145, (4, 7, 9), 0xB86u
#define PPUTLIMPL_UINT_TRAIT_0x478u HEX, 1144u, 1144, (4, 7, 8), 0xB87u
#define PPUTLIMPL_UINT_TRAIT_0x477u HEX, 1143u, 1143, (4, 7, 7), 0xB88u
#define PPUTLIMPL_UINT_TRAIT_0x476u HEX, 1142u, 1142, (4, 7, 6), 0xB89u
#define PPUTLIMPL_UINT_TRAIT_0x475u HEX, 1141u, 1141, (4, 7, 5), 0xB8Au
#define PPUTLIMPL_UINT_TRAIT_0x474u HEX, 1140u, 1140, (4, 7, 4), 0xB8Bu
#define PPUTLIMPL_UINT_TRAIT_0x473u HEX, 1139u, 1139, (4, 7, 3), 0xB8Cu
#define PPUTLIMPL_UINT_TRAIT_0x472u HEX, 1138u, 1138, (4, 7, 2), 0xB8Du
#define PPUTLIMPL_UINT_TRAIT_0x471u HEX, 1137u, 1137, (4, 7, 1), 0xB8Eu
#define PPUTLIMPL_UINT_TRAIT_0x470u HEX, 1136u, 1136, (4, 7, 0), 0xB8Fu
#define PPUTLIMPL_UINT_TRAIT_0x46Fu HEX, 1135u, 1135, (4, 6, F), 0xB90u
#define PPUTLIMPL_UINT_TRAIT_0x46Eu HEX, 1134u, 1134, (4, 6, E), 0xB91u
#define PPUTLIMPL_UINT_TRAIT_0x46Du HEX, 1133u, 1133, (4, 6, D), 0xB92u
#define PPUTLIMPL_UINT_TRAIT_0x46Cu HEX, 1132u, 1132, (4, 6, C), 0xB93u
#define PPUTLIMPL_UINT_TRAIT_0x46Bu HEX, 1131u, 1131, (4, 6, B), 0xB94u
#define PPUTLIMPL_UINT_TRAIT_0x46Au HEX, 1130u, 1130, (4, 6, A), 0xB95u
#define PPUTLIMPL_UINT_TRAIT_0x469u HEX, 1129u, 1129, (4, 6, 9), 0xB96u
#define PPUTLIMPL_UINT_TRAIT_0x468u HEX, 1128u, 1128, (4, 6, 8), 0xB97u
#define PPUTLIMPL_UINT_TRAIT_0x467u HEX, 1127u, 1127, (4, 6, 7), 0xB98u
#define PPUTLIMPL_UINT_TRAIT_0x466u HEX, 1126u, 1126, (4, 6, 6), 0xB99u
#define PPUTLIMPL_UINT_TRAIT_0x465u HEX, 1125u, 1125, (4, 6, 5), 0xB9Au
#define PPUTLIMPL_UINT_TRAIT_0x464u HEX, 1124u, 1124, (4, 6, 4), 0xB9Bu
#define PPUTLIMPL_UINT_TRAIT_0x463u HEX, 1123u, 1123, (4, 6, 3), 0xB9Cu
#define PPUTLIMPL_UINT_TRAIT_0x462u HEX, 1122u, 1122, (4, 6, 2), 0xB9Du
#define PPUTLIMPL_UINT_TRAIT_0x461u HEX, 1121u, 1121, (4, 6, 1), 0xB9Eu
#define PPUTLIMPL_UINT_TRAIT_0x460u HEX, 1120u, 1120, (4, 6, 0), 0xB9Fu
#define PPUTLIMPL_UINT_TRAIT_0x45Fu HEX, 1119u, 1119, (4, 5, F), 0xBA0u
#define PPUTLIMPL_UINT_TRAIT_0x45Eu HEX, 1118u, 1118, (4, 5, E), 0xBA1u
#define PPUTLIMPL_UINT_TRAIT_0x45Du HEX, 1117u, 1117, (4, 5, D), 0xBA2u
#define PPUTLIMPL_UINT_TRAIT_0x45Cu HEX, 1116u, 1116, (4, 5, C), 0xBA3u
#define PPUTLIMPL_UINT_TRAIT_0x45Bu HEX, 1115u, 1115, (4, 5, B), 0xBA4u
#define PPUTLIMPL_UINT_TRAIT_0x45Au HEX, 1114u, 1114, (4, 5, A), 0xBA5u
#define PPUTLIMPL_UINT_TRAIT_0x459u HEX, 1113u, 1113, (4, 5, 9), 0xBA6u
#define PPUTLIMPL_UINT_TRAIT_0x458u HEX, 1112u, 1112, (4, 5, 8), 0xBA7u
#define PPUTLIMPL_UINT_TRAIT_0x457u HEX, 1111u, 1111, (4, 5, 7), 0xBA8u
#define PPUTLIMPL_UINT_TRAIT_0x456u HEX, 1110u, 1110, (4, 5, 6), 0xBA9u
#define PPUTLIMPL_UINT_TRAIT_0x455u HEX, 1109u, 1109, (4, 5, 5), 0xBAAu
#define PPUTLIMPL_UINT_TRAIT_0x454u HEX, 1108u, 1108, (4, 5, 4), 0xBABu
#define PPUTLIMPL_UINT_TRAIT_0x453u HEX, 1107u, 1107, (4, 5, 3), 0xBACu
#define PPUTLIMPL_UINT_TRAIT_0x452u HEX, 1106u, 1106, (4, 5, 2), 0xBADu
#define PPUTLIMPL_UINT_TRAIT_0x451u HEX, 1105u, 1105, (4, 5, 1), 0xBAEu
#define PPUTLIMPL_UINT_TRAIT_0x450u HEX, 1104u, 1104, (4, 5, 0), 0xBAFu
#define PPUTLIMPL_UINT_TRAIT_0x44Fu HEX, 1103u, 1103, (4, 4, F), 0xBB0u
#define PPUTLIMPL_UINT_TRAIT_0x44Eu HEX, 1102u, 1102, (4, 4, E), 0xBB1u
#define PPUTLIMPL_UINT_TRAIT_0x44Du HEX, 1101u, 1101, (4, 4, D), 0xBB2u
#define PPUTLIMPL_UINT_TRAIT_0x44Cu HEX, 1100u, 1100, (4, 4, C), 0xBB3u
#define PPUTLIMPL_UINT_TRAIT_0x44Bu HEX, 1099u, 1099, (4, 4, B), 0xBB4u
#define PPUTLIMPL_UINT_TRAIT_0x44Au HEX, 1098u, 1098, (4, 4, A), 0xBB5u
#define PPUTLIMPL_UINT_TRAIT_0x449u HEX, 1097u, 1097, (4, 4, 9), 0xBB6u
#define PPUTLIMPL_UINT_TRAIT_0x448u HEX, 1096u, 1096, (4, 4, 8), 0xBB7u
#define PPUTLIMPL_UINT_TRAIT_0x447u HEX, 1095u, 1095, (4, 4, 7), 0xBB8u
#define PPUTLIMPL_UINT_TRAIT_0x446u HEX, 1094u, 1094, (4, 4, 6), 0xBB9u
#define PPUTLIMPL_UINT_TRAIT_0x445u HEX, 1093u, 1093, (4, 4, 5), 0xBBAu
#define PPUTLIMPL_UINT_TRAIT_0x444u HEX, 1092u, 1092, (4, 4, 4), 0xBBBu
#define PPUTLIMPL_UINT_TRAIT_0x443u HEX, 1091u, 1091, (4, 4, 3), 0xBBCu
#define PPUTLIMPL_UINT_TRAIT_0x442u HEX, 1090u, 1090, (4, 4, 2), 0xBBDu
#define PPUTLIMPL_UINT_TRAIT_0x441u HEX, 1089u, 1089, (4, 4, 1), 0xBBEu
#define PPUTLIMPL_UINT_TRAIT_0x440u HEX, 1088u, 1088, (4, 4, 0), 0xBBFu
#define PPUTLIMPL_UINT_TRAIT_0x43Fu HEX, 1087u, 1087, (4, 3, F), 0xBC0u
#define PPUTLIMPL_UINT_TRAIT_0x43Eu HEX, 1086u, 1086, (4, 3, E), 0xBC1u
#define PPUTLIMPL_UINT_TRAIT_0x43Du HEX, 1085u, 1085, (4, 3, D), 0xBC2u
#define PPUTLIMPL_UINT_TRAIT_0x43Cu HEX, 1084u, 1084, (4, 3, C), 0xBC3u
#define PPUTLIMPL_UINT_TRAIT_0x43Bu HEX, 1083u, 1083, (4, 3, B), 0xBC4u
#define PPUTLIMPL_UINT_TRAIT_0x43Au HEX, 1082u, 1082, (4, 3, A), 0xBC5u
#define PPUTLIMPL_UINT_TRAIT_0x439u HEX, 1081u, 1081, (4, 3, 9), 0xBC6u
#define PPUTLIMPL_UINT_TRAIT_0x438u HEX, 1080u, 1080, (4, 3, 8), 0xBC7u
#define PPUTLIMPL_UINT_TRAIT_0x437u HEX, 1079u, 1079, (4, 3, 7), 0xBC8u
#define PPUTLIMPL_UINT_TRAIT_0x436u HEX, 1078u, 1078, (4, 3, 6), 0xBC9u
#define PPUTLIMPL_UINT_TRAIT_0x435u HEX, 1077u, 1077, (4, 3, 5), 0xBCAu
#define PPUTLIMPL_UINT_TRAIT_0x434u HEX, 1076u, 1076, (4, 3, 4), 0xBCBu
#define PPUTLIMPL_UINT_TRAIT_0x433u HEX, 1075u, 1075, (4, 3, 3), 0xBCCu
#define PPUTLIMPL_UINT_TRAIT_0x432u HEX, 1074u, 1074, (4, 3, 2), 0xBCDu
#define PPUTLIMPL_UINT_TRAIT_0x431u HEX, 1073u, 1073, (4, 3, 1), 0xBCEu
#define PPUTLIMPL_UINT_TRAIT_0x430u HEX, 1072u, 1072, (4, 3, 0), 0xBCFu
#define PPUTLIMPL_UINT_TRAIT_0x42Fu HEX, 1071u, 1071, (4, 2, F), 0xBD0u
#define PPUTLIMPL_UINT_TRAIT_0x42Eu HEX, 1070u, 1070, (4, 2, E), 0xBD1u
#define PPUTLIMPL_UINT_TRAIT_0x42Du HEX, 1069u, 1069, (4, 2, D), 0xBD2u
#define PPUTLIMPL_UINT_TRAIT_0x42Cu HEX, 1068u, 1068, (4, 2, C), 0xBD3u
#define PPUTLIMPL_UINT_TRAIT_0x42Bu HEX, 1067u, 1067, (4, 2, B), 0xBD4u
#define PPUTLIMPL_UINT_TRAIT_0x42Au HEX, 1066u, 1066, (4, 2, A), 0xBD5u
#define PPUTLIMPL_UINT_TRAIT_0x429u HEX, 1065u, 1065, (4, 2, 9), 0xBD6u
#define PPUTLIMPL_UINT_TRAIT_0x428u HEX, 1064u, 1064, (4, 2, 8), 0xBD7u
#define PPUTLIMPL_UINT_TRAIT_0x427u HEX, 1063u, 1063, (4, 2, 7), 0xBD8u
#define PPUTLIMPL_UINT_TRAIT_0x426u HEX, 1062u, 1062, (4, 2, 6), 0xBD9u
#define PPUTLIMPL_UINT_TRAIT_0x425u HEX, 1061u, 1061, (4, 2, 5), 0xBDAu
#define PPUTLIMPL_UINT_TRAIT_0x424u HEX, 1060u, 1060, (4, 2, 4), 0xBDBu
#define PPUTLIMPL_UINT_TRAIT_0x423u HEX, 1059u, 1059, (4, 2, 3), 0xBDCu
#define PPUTLIMPL_UINT_TRAIT_0x422u HEX, 1058u, 1058, (4, 2, 2), 0xBDDu
#define PPUTLIMPL_UINT_TRAIT_0x421u HEX, 1057u, 1057, (4, 2, 1), 0xBDEu
#define PPUTLIMPL_UINT_TRAIT_0x420u HEX, 1056u, 1056, (4, 2, 0), 0xBDFu
#define PPUTLIMPL_UINT_TRAIT_0x41Fu HEX, 1055u, 1055, (4, 1, F), 0xBE0u
#define PPUTLIMPL_UINT_TRAIT_0x41Eu HEX, 1054u, 1054, (4, 1, E), 0xBE1u
#define PPUTLIMPL_UINT_TRAIT_0x41Du HEX, 1053u, 1053, (4, 1, D), 0xBE2u
#define PPUTLIMPL_UINT_TRAIT_0x41Cu HEX, 1052u, 1052, (4, 1, C), 0xBE3u
#define PPUTLIMPL_UINT_TRAIT_0x41Bu HEX, 1051u, 1051, (4, 1, B), 0xBE4u
#define PPUTLIMPL_UINT_TRAIT_0x41Au HEX, 1050u, 1050, (4, 1, A), 0xBE5u
#define PPUTLIMPL_UINT_TRAIT_0x419u HEX, 1049u, 1049, (4, 1, 9), 0xBE6u
#define PPUTLIMPL_UINT_TRAIT_0x418u HEX, 1048u, 1048, (4, 1, 8), 0xBE7u
#define PPUTLIMPL_UINT_TRAIT_0x417u HEX, 1047u, 1047, (4, 1, 7), 0xBE8u
#define PPUTLIMPL_UINT_TRAIT_0x416u HEX, 1046u, 1046, (4, 1, 6), 0xBE9u
#define PPUTLIMPL_UINT_TRAIT_0x415u HEX, 1045u, 1045, (4, 1, 5), 0xBEAu
#define PPUTLIMPL_UINT_TRAIT_0x414u HEX, 1044u, 1044, (4, 1, 4), 0xBEBu
#define PPUTLIMPL_UINT_TRAIT_0x413u HEX, 1043u, 1043, (4, 1, 3), 0xBECu
#define PPUTLIMPL_UINT_TRAIT_0x412u HEX, 1042u, 1042, (4, 1, 2), 0xBEDu
#define PPUTLIMPL_UINT_TRAIT_0x411u HEX, 1041u, 1041, (4, 1, 1), 0xBEEu
#define PPUTLIMPL_UINT_TRAIT_0x410u HEX, 1040u, 1040, (4, 1, 0), 0xBEFu
#define PPUTLIMPL_UINT_TRAIT_0x40Fu HEX, 1039u, 1039, (4, 0, F), 0xBF0u
#define PPUTLIMPL_UINT_TRAIT_0x40Eu HEX, 1038u, 1038, (4, 0, E), 0xBF1u
#define PPUTLIMPL_UINT_TRAIT_0x40Du HEX, 1037u, 1037, (4, 0, D), 0xBF2u
#define PPUTLIMPL_UINT_TRAIT_0x40Cu HEX, 1036u, 1036, (4, 0, C), 0xBF3u
#define PPUTLIMPL_UINT_TRAIT_0x40Bu HEX, 1035u, 1035, (4, 0, B), 0xBF4u
#define PPUTLIMPL_UINT_TRAIT_0x40Au HEX, 1034u, 1034, (4, 0, A), 0xBF5u
#define PPUTLIMPL_UINT_TRAIT_0x409u HEX, 1033u, 1033, (4, 0, 9), 0xBF6u
#define PPUTLIMPL_UINT_TRAIT_0x408u HEX, 1032u, 1032, (4, 0, 8), 0xBF7u
#define PPUTLIMPL_UINT_TRAIT_0x407u HEX, 1031u, 1031, (4, 0, 7), 0xBF8u
#define PPUTLIMPL_UINT_TRAIT_0x406u HEX, 1030u, 1030, (4, 0, 6), 0xBF9u
#define PPUTLIMPL_UINT_TRAIT_0x405u HEX, 1029u, 1029, (4, 0, 5), 0xBFAu
#define PPUTLIMPL_UINT_TRAIT_0x404u HEX, 1028u, 1028, (4, 0, 4), 0xBFBu
#define PPUTLIMPL_UINT_TRAIT_0x403u HEX, 1027u, 1027, (4, 0, 3), 0xBFCu
#define PPUTLIMPL_UINT_TRAIT_0x402u HEX, 1026u, 1026, (4, 0, 2), 0xBFDu
#define PPUTLIMPL_UINT_TRAIT_0x401u HEX, 1025u, 1025, (4, 0, 1), 0xBFEu
#define PPUTLIMPL_UINT_TRAIT_0x400u HEX, 1024u, 1024, (4, 0, 0), 0xBFFu
#define PPUTLIMPL_UINT_TRAIT_0x3FFu HEX, 1023u, 1023, (3, F, F), 0xC00u
#define PPUTLIMPL_UINT_TRAIT_0x3FEu HEX, 1022u, 1022, (3, F, E), 0xC01u
#define PPUTLIMPL_UINT_TRAIT_0x3FDu HEX, 1021u, 1021, (3, F, D), 0xC02u
#define PPUTLIMPL_UINT_TRAIT_0x3FCu HEX, 1020u, 1020, (3, F, C), 0xC03u
#define PPUTLIMPL_UINT_TRAIT_0x3FBu HEX, 1019u, 1019, (3, F, B), 0xC04u
#define PPUTLIMPL_UINT_TRAIT_0x3FAu HEX, 1018u, 1018, (3, F, A), 0xC05u
#define PPUTLIMPL_UINT_TRAIT_0x3F9u HEX, 1017u, 1017, (3, F, 9), 0xC06u
#define PPUTLIMPL_UINT_TRAIT_0x3F8u HEX, 1016u, 1016, (3, F, 8), 0xC07u
#define PPUTLIMPL_UINT_TRAIT_0x3F7u HEX, 1015u, 1015, (3, F, 7), 0xC08u
#define PPUTLIMPL_UINT_TRAIT_0x3F6u HEX, 1014u, 1014, (3, F, 6), 0xC09u
#define PPUTLIMPL_UINT_TRAIT_0x3F5u HEX, 1013u, 1013, (3, F, 5), 0xC0Au
#define PPUTLIMPL_UINT_TRAIT_0x3F4u HEX, 1012u, 1012, (3, F, 4), 0xC0Bu
#define PPUTLIMPL_UINT_TRAIT_0x3F3u HEX, 1011u, 1011, (3, F, 3), 0xC0Cu
#define PPUTLIMPL_UINT_TRAIT_0x3F2u HEX, 1010u, 1010, (3, F, 2), 0xC0Du
#define PPUTLIMPL_UINT_TRAIT_0x3F1u HEX, 1009u, 1009, (3, F, 1), 0xC0Eu
#define PPUTLIMPL_UINT_TRAIT_0x3F0u HEX, 1008u, 1008, (3, F, 0), 0xC0Fu
#define PPUTLIMPL_UINT_TRAIT_0x3EFu HEX, 1007u, 1007, (3, E, F), 0xC10u
#define PPUTLIMPL_UINT_TRAIT_0x3EEu HEX, 1006u, 1006, (3, E, E), 0xC11u
#define PPUTLIMPL_UINT_TRAIT_0x3EDu HEX, 1005u, 1005, (3, E, D), 0xC12u
#define PPUTLIMPL_UINT_TRAIT_0x3ECu HEX, 1004u, 1004, (3, E, C), 0xC13u
#define PPUTLIMPL_UINT_TRAIT_0x3EBu HEX, 1003u, 1003, (3, E, B), 0xC14u
#define PPUTLIMPL_UINT_TRAIT_0x3EAu HEX, 1002u, 1002, (3, E, A), 0xC15u
#define PPUTLIMPL_UINT_TRAIT_0x3E9u HEX, 1001u, 1001, (3, E, 9), 0xC16u
#define PPUTLIMPL_UINT_TRAIT_0x3E8u HEX, 1000u, 1000, (3, E, 8), 0xC17u
#define PPUTLIMPL_UINT_TRAIT_0x3E7u HEX, 999u, 999, (3, E, 7), 0xC18u
#define PPUTLIMPL_UINT_TRAIT_0x3E6u HEX, 998u, 998, (3, E, 6), 0xC19u
#define PPUTLIMPL_UINT_TRAIT_0x3E5u HEX, 997u, 997, (3, E, 5), 0xC1Au
#define PPUTLIMPL_UINT_TRAIT_0x3E4u HEX, 996u, 996, (3, E, 4), 0xC1Bu
#define PPUTLIMPL_UINT_TRAIT_0x3E3u HEX, 995u, 995, (3, E, 3), 0xC1Cu
#define PPUTLIMPL_UINT_TRAIT_0x3E2u HEX, 994u, 994, (3, E, 2), 0xC1Du
#define PPUTLIMPL_UINT_TRAIT_0x3E1u HEX, 993u, 993, (3, E, 1), 0xC1Eu
#define PPUTLIMPL_UINT_TRAIT_0x3E0u HEX, 992u, 992, (3, E, 0), 0xC1Fu
#define PPUTLIMPL_UINT_TRAIT_0x3DFu HEX, 991u, 991, (3, D, F), 0xC20u
#define PPUTLIMPL_UINT_TRAIT_0x3DEu HEX, 990u, 990, (3, D, E), 0xC21u
#define PPUTLIMPL_UINT_TRAIT_0x3DDu HEX, 989u, 989, (3, D, D), 0xC22u
#define PPUTLIMPL_UINT_TRAIT_0x3DCu HEX, 988u, 988, (3, D, C), 0xC23u
#define PPUTLIMPL_UINT_TRAIT_0x3DBu HEX, 987u, 987, (3, D, B), 0xC24u
#define PPUTLIMPL_UINT_TRAIT_0x3DAu HEX, 986u, 986, (3, D, A), 0xC25u
#define PPUTLIMPL_UINT_TRAIT_0x3D9u HEX, 985u, 985, (3, D, 9), 0xC26u
#define PPUTLIMPL_UINT_TRAIT_0x3D8u HEX, 984u, 984, (3, D, 8), 0xC27u
#define PPUTLIMPL_UINT_TRAIT_0x3D7u HEX, 983u, 983, (3, D, 7), 0xC28u
#define PPUTLIMPL_UINT_TRAIT_0x3D6u HEX, 982u, 982, (3, D, 6), 0xC29u
#define PPUTLIMPL_UINT_TRAIT_0x3D5u HEX, 981u, 981, (3, D, 5), 0xC2Au
#define PPUTLIMPL_UINT_TRAIT_0x3D4u HEX, 980u, 980, (3, D, 4), 0xC2Bu
#define PPUTLIMPL_UINT_TRAIT_0x3D3u HEX, 979u, 979, (3, D, 3), 0xC2Cu
#define PPUTLIMPL_UINT_TRAIT_0x3D2u HEX, 978u, 978, (3, D, 2), 0xC2Du
#define PPUTLIMPL_UINT_TRAIT_0x3D1u HEX, 977u, 977, (3, D, 1), 0xC2Eu
#define PPUTLIMPL_UINT_TRAIT_0x3D0u HEX, 976u, 976, (3, D, 0), 0xC2Fu
#define PPUTLIMPL_UINT_TRAIT_0x3CFu HEX, 975u, 975, (3, C, F), 0xC30u
#define PPUTLIMPL_UINT_TRAIT_0x3CEu HEX, 974u, 974, (3, C, E), 0xC31u
#define PPUTLIMPL_UINT_TRAIT_0x3CDu HEX, 973u, 973, (3, C, D), 0xC32u
#define PPUTLIMPL_UINT_TRAIT_0x3CCu HEX, 972u, 972, (3, C, C), 0xC33u
#define PPUTLIMPL_UINT_TRAIT_0x3CBu HEX, 971u, 971, (3, C, B), 0xC34u
#define PPUTLIMPL_UINT_TRAIT_0x3CAu HEX, 970u, 970, (3, C, A), 0xC35u
#define PPUTLIMPL_UINT_TRAIT_0x3C9u HEX, 969u, 969, (3, C, 9), 0xC36u
#define PPUTLIMPL_UINT_TRAIT_0x3C8u HEX, 968u, 968, (3, C, 8), 0xC37u
#define PPUTLIMPL_UINT_TRAIT_0x3C7u HEX, 967u, 967, (3, C, 7), 0xC38u
#define PPUTLIMPL_UINT_TRAIT_0x3C6u HEX, 966u, 966, (3, C, 6), 0xC39u
#define PPUTLIMPL_UINT_TRAIT_0x3C5u HEX, 965u, 965, (3, C, 5), 0xC3Au
#define PPUTLIMPL_UINT_TRAIT_0x3C4u HEX, 964u, 964, (3, C, 4), 0xC3Bu
#define PPUTLIMPL_UINT_TRAIT_0x3C3u HEX, 963u, 963, (3, C, 3), 0xC3Cu
#define PPUTLIMPL_UINT_TRAIT_0x3C2u HEX, 962u, 962, (3, C, 2), 0xC3Du
#define PPUTLIMPL_UINT_TRAIT_0x3C1u HEX, 961u, 961, (3, C, 1), 0xC3Eu
#define PPUTLIMPL_UINT_TRAIT_0x3C0u HEX, 960u, 960, (3, C, 0), 0xC3Fu
#define PPUTLIMPL_UINT_TRAIT_0x3BFu HEX, 959u, 959, (3, B, F), 0xC40u
#define PPUTLIMPL_UINT_TRAIT_0x3BEu HEX, 958u, 958, (3, B, E), 0xC41u
#define PPUTLIMPL_UINT_TRAIT_0x3BDu HEX, 957u, 957, (3, B, D), 0xC42u
#define PPUTLIMPL_UINT_TRAIT_0x3BCu HEX, 956u, 956, (3, B, C), 0xC43u
#define PPUTLIMPL_UINT_TRAIT_0x3BBu HEX, 955u, 955, (3, B, B), 0xC44u
#define PPUTLIMPL_UINT_TRAIT_0x3BAu HEX, 954u, 954, (3, B, A), 0xC45u
#define PPUTLIMPL_UINT_TRAIT_0x3B9u HEX, 953u, 953, (3, B, 9), 0xC46u
#define PPUTLIMPL_UINT_TRAIT_0x3B8u HEX, 952u, 952, (3, B, 8), 0xC47u
#define PPUTLIMPL_UINT_TRAIT_0x3B7u HEX, 951u, 951, (3, B, 7), 0xC48u
#define PPUTLIMPL_UINT_TRAIT_0x3B6u HEX, 950u, 950, (3, B, 6), 0xC49u
#define PPUTLIMPL_UINT_TRAIT_0x3B5u HEX, 949u, 949, (3, B, 5), 0xC4Au
#define PPUTLIMPL_UINT_TRAIT_0x3B4u HEX, 948u, 948, (3, B, 4), 0xC4Bu
#define PPUTLIMPL_UINT_TRAIT_0x3B3u HEX, 947u, 947, (3, B, 3), 0xC4Cu
#define PPUTLIMPL_UINT_TRAIT_0x3B2u HEX, 946u, 946, (3, B, 2), 0xC4Du
#define PPUTLIMPL_UINT_TRAIT_0x3B1u HEX, 945u, 945, (3, B, 1), 0xC4Eu
#define PPUTLIMPL_UINT_TRAIT_0x3B0u HEX, 944u, 944, (3, B, 0), 0xC4Fu
#define PPUTLIMPL_UINT_TRAIT_0x3AFu HEX, 943u, 943, (3, A, F), 0xC50u
#define PPUTLIMPL_UINT_TRAIT_0x3AEu HEX, 942u, 942, (3, A, E), 0xC51u
#define PPUTLIMPL_UINT_TRAIT_0x3ADu HEX, 941u, 941, (3, A, D), 0xC52u
#define PPUTLIMPL_UINT_TRAIT_0x3ACu HEX, 940u, 940, (3, A, C), 0xC53u
#define PPUTLIMPL_UINT_TRAIT_0x3ABu HEX, 939u, 939, (3, A, B), 0xC54u
#define PPUTLIMPL_UINT_TRAIT_0x3AAu HEX, 938u, 938, (3, A, A), 0xC55u
#define PPUTLIMPL_UINT_TRAIT_0x3A9u HEX, 937u, 937, (3, A, 9), 0xC56u
#define PPUTLIMPL_UINT_TRAIT_0x3A8u HEX, 936u, 936, (3, A, 8), 0xC57u
#define PPUTLIMPL_UINT_TRAIT_0x3A7u HEX, 935u, 935, (3, A, 7), 0xC58u
#define PPUTLIMPL_UINT_TRAIT_0x3A6u HEX, 934u, 934, (3, A, 6), 0xC59u
#define PPUTLIMPL_UINT_TRAIT_0x3A5u HEX, 933u, 933, (3, A, 5), 0xC5Au
#define PPUTLIMPL_UINT_TRAIT_0x3A4u HEX, 932u, 932, (3, A, 4), 0xC5Bu
#define PPUTLIMPL_UINT_TRAIT_0x3A3u HEX, 931u, 931, (3, A, 3), 0xC5Cu
#define PPUTLIMPL_UINT_TRAIT_0x3A2u HEX, 930u, 930, (3, A, 2), 0xC5Du
#define PPUTLIMPL_UINT_TRAIT_0x3A1u HEX, 929u, 929, (3, A, 1), 0xC5Eu
#define PPUTLIMPL_UINT_TRAIT_0x3A0u HEX, 928u, 928, (3, A, 0), 0xC5Fu
#define PPUTLIMPL_UINT_TRAIT_0x39Fu HEX, 927u, 927, (3, 9, F), 0xC60u
#define PPUTLIMPL_UINT_TRAIT_0x39Eu HEX, 926u, 926, (3, 9, E), 0xC61u
#define PPUTLIMPL_UINT_TRAIT_0x39Du HEX, 925u, 925, (3, 9, D), 0xC62u
#define PPUTLIMPL_UINT_TRAIT_0x39Cu HEX, 924u, 924, (3, 9, C), 0xC63u
#define PPUTLIMPL_UINT_TRAIT_0x39Bu HEX, 923u, 923, (3, 9, B), 0xC64u
#define PPUTLIMPL_UINT_TRAIT_0x39Au HEX, 922u, 922, (3, 9, A), 0xC65u
#define PPUTLIMPL_UINT_TRAIT_0x399u HEX, 921u, 921, (3, 9, 9), 0xC66u
#define PPUTLIMPL_UINT_TRAIT_0x398u HEX, 920u, 920, (3, 9, 8), 0xC67u
#define PPUTLIMPL_UINT_TRAIT_0x397u HEX, 919u, 919, (3, 9, 7), 0xC68u
#define PPUTLIMPL_UINT_TRAIT_0x396u HEX, 918u, 918, (3, 9, 6), 0xC69u
#define PPUTLIMPL_UINT_TRAIT_0x395u HEX, 917u, 917, (3, 9, 5), 0xC6Au
#define PPUTLIMPL_UINT_TRAIT_0x394u HEX, 916u, 916, (3, 9, 4), 0xC6Bu
#define PPUTLIMPL_UINT_TRAIT_0x393u HEX, 915u, 915, (3, 9, 3), 0xC6Cu
#define PPUTLIMPL_UINT_TRAIT_0x392u HEX, 914u, 914, (3, 9, 2), 0xC6Du
#define PPUTLIMPL_UINT_TRAIT_0x391u HEX, 913u, 913, (3, 9, 1), 0xC6Eu
#define PPUTLIMPL_UINT_TRAIT_0x390u HEX, 912u, 912, (3, 9, 0), 0xC6Fu
#define PPUTLIMPL_UINT_TRAIT_0x38Fu HEX, 911u, 911, (3, 8, F), 0xC70u
#define PPUTLIMPL_UINT_TRAIT_0x38Eu HEX, 910u, 910, (3, 8, E), 0xC71u
#define PPUTLIMPL_UINT_TRAIT_0x38Du HEX, 909u, 909, (3, 8, D), 0xC72u
#define PPUTLIMPL_UINT_TRAIT_0x38Cu HEX, 908u, 908, (3, 8, C), 0xC73u
#define PPUTLIMPL_UINT_TRAIT_0x38Bu HEX, 907u, 907, (3, 8, B), 0xC74u
#define PPUTLIMPL_UINT_TRAIT_0x38Au HEX, 906u, 906, (3, 8, A), 0xC75u
#define PPUTLIMPL_UINT_TRAIT_0x389u HEX, 905u, 905, (3, 8, 9), 0xC76u
#define PPUTLIMPL_UINT_TRAIT_0x388u HEX, 904u, 904, (3, 8, 8), 0xC77u
#define PPUTLIMPL_UINT_TRAIT_0x387u HEX, 903u, 903, (3, 8, 7), 0xC78u
#define PPUTLIMPL_UINT_TRAIT_0x386u HEX, 902u, 902, (3, 8, 6), 0xC79u
#define PPUTLIMPL_UINT_TRAIT_0x385u HEX, 901u, 901, (3, 8, 5), 0xC7Au
#define PPUTLIMPL_UINT_TRAIT_0x384u HEX, 900u, 900, (3, 8, 4), 0xC7Bu
#define PPUTLIMPL_UINT_TRAIT_0x383u HEX, 899u, 899, (3, 8, 3), 0xC7Cu
#define PPUTLIMPL_UINT_TRAIT_0x382u HEX, 898u, 898, (3, 8, 2), 0xC7Du
#define PPUTLIMPL_UINT_TRAIT_0x381u HEX, 897u, 897, (3, 8, 1), 0xC7Eu
#define PPUTLIMPL_UINT_TRAIT_0x380u HEX, 896u, 896, (3, 8, 0), 0xC7Fu
#define PPUTLIMPL_UINT_TRAIT_0x37Fu HEX, 895u, 895, (3, 7, F), 0xC80u
#define PPUTLIMPL_UINT_TRAIT_0x37Eu HEX, 894u, 894, (3, 7, E), 0xC81u
#define PPUTLIMPL_UINT_TRAIT_0x37Du HEX, 893u, 893, (3, 7, D), 0xC82u
#define PPUTLIMPL_UINT_TRAIT_0x37Cu HEX, 892u, 892, (3, 7, C), 0xC83u
#define PPUTLIMPL_UINT_TRAIT_0x37Bu HEX, 891u, 891, (3, 7, B), 0xC84u
#define PPUTLIMPL_UINT_TRAIT_0x37Au HEX, 890u, 890, (3, 7, A), 0xC85u
#define PPUTLIMPL_UINT_TRAIT_0x379u HEX, 889u, 889, (3, 7, 9), 0xC86u
#define PPUTLIMPL_UINT_TRAIT_0x378u HEX, 888u, 888, (3, 7, 8), 0xC87u
#define PPUTLIMPL_UINT_TRAIT_0x377u HEX, 887u, 887, (3, 7, 7), 0xC88u
#define PPUTLIMPL_UINT_TRAIT_0x376u HEX, 886u, 886, (3, 7, 6), 0xC89u
#define PPUTLIMPL_UINT_TRAIT_0x375u HEX, 885u, 885, (3, 7, 5), 0xC8Au
#define PPUTLIMPL_UINT_TRAIT_0x374u HEX, 884u, 884, (3, 7, 4), 0xC8Bu
#define PPUTLIMPL_UINT_TRAIT_0x373u HEX, 883u, 883, (3, 7, 3), 0xC8Cu
#define PPUTLIMPL_UINT_TRAIT_0x372u HEX, 882u, 882, (3, 7, 2), 0xC8Du
#define PPUTLIMPL_UINT_TRAIT_0x371u HEX, 881u, 881, (3, 7, 1), 0xC8Eu
#define PPUTLIMPL_UINT_TRAIT_0x370u HEX, 880u, 880, (3, 7, 0), 0xC8Fu
#define PPUTLIMPL_UINT_TRAIT_0x36Fu HEX, 879u, 879, (3, 6, F), 0xC90u
#define PPUTLIMPL_UINT_TRAIT_0x36Eu HEX, 878u, 878, (3, 6, E), 0xC91u
#define PPUTLIMPL_UINT_TRAIT_0x36Du HEX, 877u, 877, (3, 6, D), 0xC92u
#define PPUTLIMPL_UINT_TRAIT_0x36Cu HEX, 876u, 876, (3, 6, C), 0xC93u
#define PPUTLIMPL_UINT_TRAIT_0x36Bu HEX, 875u, 875, (3, 6, B), 0xC94u
#define PPUTLIMPL_UINT_TRAIT_0x36Au HEX, 874u, 874, (3, 6, A), 0xC95u
#define PPUTLIMPL_UINT_TRAIT_0x369u HEX, 873u, 873, (3, 6, 9), 0xC96u
#define PPUTLIMPL_UINT_TRAIT_0x368u HEX, 872u, 872, (3, 6, 8), 0xC97u
#define PPUTLIMPL_UINT_TRAIT_0x367u HEX, 871u, 871, (3, 6, 7), 0xC98u
#define PPUTLIMPL_UINT_TRAIT_0x366u HEX, 870u, 870, (3, 6, 6), 0xC99u
#define PPUTLIMPL_UINT_TRAIT_0x365u HEX, 869u, 869, (3, 6, 5), 0xC9Au
#define PPUTLIMPL_UINT_TRAIT_0x364u HEX, 868u, 868, (3, 6, 4), 0xC9Bu
#define PPUTLIMPL_UINT_TRAIT_0x363u HEX, 867u, 867, (3, 6, 3), 0xC9Cu
#define PPUTLIMPL_UINT_TRAIT_0x362u HEX, 866u, 866, (3, 6, 2), 0xC9Du
#define PPUTLIMPL_UINT_TRAIT_0x361u HEX, 865u, 865, (3, 6, 1), 0xC9Eu
#define PPUTLIMPL_UINT_TRAIT_0x360u HEX, 864u, 864, (3, 6, 0), 0xC9Fu
#define PPUTLIMPL_UINT_TRAIT_0x35Fu HEX, 863u, 863, (3, 5, F), 0xCA0u
#define PPUTLIMPL_UINT_TRAIT_0x35Eu HEX, 862u, 862, (3, 5, E), 0xCA1u
#define PPUTLIMPL_UINT_TRAIT_0x35Du HEX, 861u, 861, (3, 5, D), 0xCA2u
#define PPUTLIMPL_UINT_TRAIT_0x35Cu HEX, 860u, 860, (3, 5, C), 0xCA3u
#define PPUTLIMPL_UINT_TRAIT_0x35Bu HEX, 859u, 859, (3, 5, B), 0xCA4u
#define PPUTLIMPL_UINT_TRAIT_0x35Au HEX, 858u, 858, (3, 5, A), 0xCA5u
#define PPUTLIMPL_UINT_TRAIT_0x359u HEX, 857u, 857, (3, 5, 9), 0xCA6u
#define PPUTLIMPL_UINT_TRAIT_0x358u HEX, 856u, 856, (3, 5, 8), 0xCA7u
#define PPUTLIMPL_UINT_TRAIT_0x357u HEX, 855u, 855, (3, 5, 7), 0xCA8u
#define PPUTLIMPL_UINT_TRAIT_0x356u HEX, 854u, 854, (3, 5, 6), 0xCA9u
#define PPUTLIMPL_UINT_TRAIT_0x355u HEX, 853u, 853, (3, 5, 5), 0xCAAu
#define PPUTLIMPL_UINT_TRAIT_0x354u HEX, 852u, 852, (3, 5, 4), 0xCABu
#define PPUTLIMPL_UINT_TRAIT_0x353u HEX, 851u, 851, (3, 5, 3), 0xCACu
#define PPUTLIMPL_UINT_TRAIT_0x352u HEX, 850u, 850, (3, 5, 2), 0xCADu
#define PPUTLIMPL_UINT_TRAIT_0x351u HEX, 849u, 849, (3, 5, 1), 0xCAEu
#define PPUTLIMPL_UINT_TRAIT_0x350u HEX, 848u, 848, (3, 5, 0), 0xCAFu
#define PPUTLIMPL_UINT_TRAIT_0x34Fu HEX, 847u, 847, (3, 4, F), 0xCB0u
#define PPUTLIMPL_UINT_TRAIT_0x34Eu HEX, 846u, 846, (3, 4, E), 0xCB1u
#define PPUTLIMPL_UINT_TRAIT_0x34Du HEX, 845u, 845, (3, 4, D), 0xCB2u
#define PPUTLIMPL_UINT_TRAIT_0x34Cu HEX, 844u, 844, (3, 4, C), 0xCB3u
#define PPUTLIMPL_UINT_TRAIT_0x34Bu HEX, 843u, 843, (3, 4, B), 0xCB4u
#define PPUTLIMPL_UINT_TRAIT_0x34Au HEX, 842u, 842, (3, 4, A), 0xCB5u
#define PPUTLIMPL_UINT_TRAIT_0x349u HEX, 841u, 841, (3, 4, 9), 0xCB6u
#define PPUTLIMPL_UINT_TRAIT_0x348u HEX, 840u, 840, (3, 4, 8), 0xCB7u
#define PPUTLIMPL_UINT_TRAIT_0x347u HEX, 839u, 839, (3, 4, 7), 0xCB8u
#define PPUTLIMPL_UINT_TRAIT_0x346u HEX, 838u, 838, (3, 4, 6), 0xCB9u
#define PPUTLIMPL_UINT_TRAIT_0x345u HEX, 837u, 837, (3, 4, 5), 0xCBAu
#define PPUTLIMPL_UINT_TRAIT_0x344u HEX, 836u, 836, (3, 4, 4), 0xCBBu
#define PPUTLIMPL_UINT_TRAIT_0x343u HEX, 835u, 835, (3, 4, 3), 0xCBCu
#define PPUTLIMPL_UINT_TRAIT_0x342u HEX, 834u, 834, (3, 4, 2), 0xCBDu
#define PPUTLIMPL_UINT_TRAIT_0x341u HEX, 833u, 833, (3, 4, 1), 0xCBEu
#define PPUTLIMPL_UINT_TRAIT_0x340u HEX, 832u, 832, (3, 4, 0), 0xCBFu
#define PPUTLIMPL_UINT_TRAIT_0x33Fu HEX, 831u, 831, (3, 3, F), 0xCC0u
#define PPUTLIMPL_UINT_TRAIT_0x33Eu HEX, 830u, 830, (3, 3, E), 0xCC1u
#define PPUTLIMPL_UINT_TRAIT_0x33Du HEX, 829u, 829, (3, 3, D), 0xCC2u
#define PPUTLIMPL_UINT_TRAIT_0x33Cu HEX, 828u, 828, (3, 3, C), 0xCC3u
#define PPUTLIMPL_UINT_TRAIT_0x33Bu HEX, 827u, 827, (3, 3, B), 0xCC4u
#define PPUTLIMPL_UINT_TRAIT_0x33Au HEX, 826u, 826, (3, 3, A), 0xCC5u
#define PPUTLIMPL_UINT_TRAIT_0x339u HEX, 825u, 825, (3, 3, 9), 0xCC6u
#define PPUTLIMPL_UINT_TRAIT_0x338u HEX, 824u, 824, (3, 3, 8), 0xCC7u
#define PPUTLIMPL_UINT_TRAIT_0x337u HEX, 823u, 823, (3, 3, 7), 0xCC8u
#define PPUTLIMPL_UINT_TRAIT_0x336u HEX, 822u, 822, (3, 3, 6), 0xCC9u
#define PPUTLIMPL_UINT_TRAIT_0x335u HEX, 821u, 821, (3, 3, 5), 0xCCAu
#define PPUTLIMPL_UINT_TRAIT_0x334u HEX, 820u, 820, (3, 3, 4), 0xCCBu
#define PPUTLIMPL_UINT_TRAIT_0x333u HEX, 819u, 819, (3, 3, 3), 0xCCCu
#define PPUTLIMPL_UINT_TRAIT_0x332u HEX, 818u, 818, (3, 3, 2), 0xCCDu
#define PPUTLIMPL_UINT_TRAIT_0x331u HEX, 817u, 817, (3, 3, 1), 0xCCEu
#define PPUTLIMPL_UINT_TRAIT_0x330u HEX, 816u, 816, (3, 3, 0), 0xCCFu
#define PPUTLIMPL_UINT_TRAIT_0x32Fu HEX, 815u, 815, (3, 2, F), 0xCD0u
#define PPUTLIMPL_UINT_TRAIT_0x32Eu HEX, 814u, 814, (3, 2, E), 0xCD1u
#define PPUTLIMPL_UINT_TRAIT_0x32Du HEX, 813u, 813, (3, 2, D), 0xCD2u
#define PPUTLIMPL_UINT_TRAIT_0x32Cu HEX, 812u, 812, (3, 2, C), 0xCD3u
#define PPUTLIMPL_UINT_TRAIT_0x32Bu HEX, 811u, 811, (3, 2, B), 0xCD4u
#define PPUTLIMPL_UINT_TRAIT_0x32Au HEX, 810u, 810, (3, 2, A), 0xCD5u
#define PPUTLIMPL_UINT_TRAIT_0x329u HEX, 809u, 809, (3, 2, 9), 0xCD6u
#define PPUTLIMPL_UINT_TRAIT_0x328u HEX, 808u, 808, (3, 2, 8), 0xCD7u
#define PPUTLIMPL_UINT_TRAIT_0x327u HEX, 807u, 807, (3, 2, 7), 0xCD8u
#define PPUTLIMPL_UINT_TRAIT_0x326u HEX, 806u, 806, (3, 2, 6), 0xCD9u
#define PPUTLIMPL_UINT_TRAIT_0x325u HEX, 805u, 805, (3, 2, 5), 0xCDAu
#define PPUTLIMPL_UINT_TRAIT_0x324u HEX, 804u, 804, (3, 2, 4), 0xCDBu
#define PPUTLIMPL_UINT_TRAIT_0x323u HEX, 803u, 803, (3, 2, 3), 0xCDCu
#define PPUTLIMPL_UINT_TRAIT_0x322u HEX, 802u, 802, (3, 2, 2), 0xCDDu
#define PPUTLIMPL_UINT_TRAIT_0x321u HEX, 801u, 801, (3, 2, 1), 0xCDEu
#define PPUTLIMPL_UINT_TRAIT_0x320u HEX, 800u, 800, (3, 2, 0), 0xCDFu
#define PPUTLIMPL_UINT_TRAIT_0x31Fu HEX, 799u, 799, (3, 1, F), 0xCE0u
#define PPUTLIMPL_UINT_TRAIT_0x31Eu HEX, 798u, 798, (3, 1, E), 0xCE1u
#define PPUTLIMPL_UINT_TRAIT_0x31Du HEX, 797u, 797, (3, 1, D), 0xCE2u
#define PPUTLIMPL_UINT_TRAIT_0x31Cu HEX, 796u, 796, (3, 1, C), 0xCE3u
#define PPUTLIMPL_UINT_TRAIT_0x31Bu HEX, 795u, 795, (3, 1, B), 0xCE4u
#define PPUTLIMPL_UINT_TRAIT_0x31Au HEX, 794u, 794, (3, 1, A), 0xCE5u
#define PPUTLIMPL_UINT_TRAIT_0x319u HEX, 793u, 793, (3, 1, 9), 0xCE6u
#define PPUTLIMPL_UINT_TRAIT_0x318u HEX, 792u, 792, (3, 1, 8), 0xCE7u
#define PPUTLIMPL_UINT_TRAIT_0x317u HEX, 791u, 791, (3, 1, 7), 0xCE8u
#define PPUTLIMPL_UINT_TRAIT_0x316u HEX, 790u, 790, (3, 1, 6), 0xCE9u
#define PPUTLIMPL_UINT_TRAIT_0x315u HEX, 789u, 789, (3, 1, 5), 0xCEAu
#define PPUTLIMPL_UINT_TRAIT_0x314u HEX, 788u, 788, (3, 1, 4), 0xCEBu
#define PPUTLIMPL_UINT_TRAIT_0x313u HEX, 787u, 787, (3, 1, 3), 0xCECu
#define PPUTLIMPL_UINT_TRAIT_0x312u HEX, 786u, 786, (3, 1, 2), 0xCEDu
#define PPUTLIMPL_UINT_TRAIT_0x311u HEX, 785u, 785, (3, 1, 1), 0xCEEu
#define PPUTLIMPL_UINT_TRAIT_0x310u HEX, 784u, 784, (3, 1, 0), 0xCEFu
#define PPUTLIMPL_UINT_TRAIT_0x30Fu HEX, 783u, 783, (3, 0, F), 0xCF0u
#define PPUTLIMPL_UINT_TRAIT_0x30Eu HEX, 782u, 782, (3, 0, E), 0xCF1u
#define PPUTLIMPL_UINT_TRAIT_0x30Du HEX, 781u, 781, (3, 0, D), 0xCF2u
#define PPUTLIMPL_UINT_TRAIT_0x30Cu HEX, 780u, 780, (3, 0, C), 0xCF3u
#define PPUTLIMPL_UINT_TRAIT_0x30Bu HEX, 779u, 779, (3, 0, B), 0xCF4u
#define PPUTLIMPL_UINT_TRAIT_0x30Au HEX, 778u, 778, (3, 0, A), 0xCF5u
#define PPUTLIMPL_UINT_TRAIT_0x309u HEX, 777u, 777, (3, 0, 9), 0xCF6u
#define PPUTLIMPL_UINT_TRAIT_0x308u HEX, 776u, 776, (3, 0, 8), 0xCF7u
#define PPUTLIMPL_UINT_TRAIT_0x307u HEX, 775u, 775, (3, 0, 7), 0xCF8u
#define PPUTLIMPL_UINT_TRAIT_0x306u HEX, 774u, 774, (3, 0, 6), 0xCF9u
#define PPUTLIMPL_UINT_TRAIT_0x305u HEX, 773u, 773, (3, 0, 5), 0xCFAu
#define PPUTLIMPL_UINT_TRAIT_0x304u HEX, 772u, 772, (3, 0, 4), 0xCFBu
#define PPUTLIMPL_UINT_TRAIT_0x303u HEX, 771u, 771, (3, 0, 3), 0xCFCu
#define PPUTLIMPL_UINT_TRAIT_0x302u HEX, 770u, 770, (3, 0, 2), 0xCFDu
#define PPUTLIMPL_UINT_TRAIT_0x301u HEX, 769u, 769, (3, 0, 1), 0xCFEu
#define PPUTLIMPL_UINT_TRAIT_0x300u HEX, 768u, 768, (3, 0, 0), 0xCFFu
#define PPUTLIMPL_UINT_TRAIT_0x2FFu HEX, 767u, 767, (2, F, F), 0xD00u
#define PPUTLIMPL_UINT_TRAIT_0x2FEu HEX, 766u, 766, (2, F, E), 0xD01u
#define PPUTLIMPL_UINT_TRAIT_0x2FDu HEX, 765u, 765, (2, F, D), 0xD02u
#define PPUTLIMPL_UINT_TRAIT_0x2FCu HEX, 764u, 764, (2, F, C), 0xD03u
#define PPUTLIMPL_UINT_TRAIT_0x2FBu HEX, 763u, 763, (2, F, B), 0xD04u
#define PPUTLIMPL_UINT_TRAIT_0x2FAu HEX, 762u, 762, (2, F, A), 0xD05u
#define PPUTLIMPL_UINT_TRAIT_0x2F9u HEX, 761u, 761, (2, F, 9), 0xD06u
#define PPUTLIMPL_UINT_TRAIT_0x2F8u HEX, 760u, 760, (2, F, 8), 0xD07u
#define PPUTLIMPL_UINT_TRAIT_0x2F7u HEX, 759u, 759, (2, F, 7), 0xD08u
#define PPUTLIMPL_UINT_TRAIT_0x2F6u HEX, 758u, 758, (2, F, 6), 0xD09u
#define PPUTLIMPL_UINT_TRAIT_0x2F5u HEX, 757u, 757, (2, F, 5), 0xD0Au
#define PPUTLIMPL_UINT_TRAIT_0x2F4u HEX, 756u, 756, (2, F, 4), 0xD0Bu
#define PPUTLIMPL_UINT_TRAIT_0x2F3u HEX, 755u, 755, (2, F, 3), 0xD0Cu
#define PPUTLIMPL_UINT_TRAIT_0x2F2u HEX, 754u, 754, (2, F, 2), 0xD0Du
#define PPUTLIMPL_UINT_TRAIT_0x2F1u HEX, 753u, 753, (2, F, 1), 0xD0Eu
#define PPUTLIMPL_UINT_TRAIT_0x2F0u HEX, 752u, 752, (2, F, 0), 0xD0Fu
#define PPUTLIMPL_UINT_TRAIT_0x2EFu HEX, 751u, 751, (2, E, F), 0xD10u
#define PPUTLIMPL_UINT_TRAIT_0x2EEu HEX, 750u, 750, (2, E, E), 0xD11u
#define PPUTLIMPL_UINT_TRAIT_0x2EDu HEX, 749u, 749, (2, E, D), 0xD12u
#define PPUTLIMPL_UINT_TRAIT_0x2ECu HEX, 748u, 748, (2, E, C), 0xD13u
#define PPUTLIMPL_UINT_TRAIT_0x2EBu HEX, 747u, 747, (2, E, B), 0xD14u
#define PPUTLIMPL_UINT_TRAIT_0x2EAu HEX, 746u, 746, (2, E, A), 0xD15u
#define PPUTLIMPL_UINT_TRAIT_0x2E9u HEX, 745u, 745, (2, E, 9), 0xD16u
#define PPUTLIMPL_UINT_TRAIT_0x2E8u HEX, 744u, 744, (2, E, 8), 0xD17u
#define PPUTLIMPL_UINT_TRAIT_0x2E7u HEX, 743u, 743, (2, E, 7), 0xD18u
#define PPUTLIMPL_UINT_TRAIT_0x2E6u HEX, 742u, 742, (2, E, 6), 0xD19u
#define PPUTLIMPL_UINT_TRAIT_0x2E5u HEX, 741u, 741, (2, E, 5), 0xD1Au
#define PPUTLIMPL_UINT_TRAIT_0x2E4u HEX, 740u, 740, (2, E, 4), 0xD1Bu
#define PPUTLIMPL_UINT_TRAIT_0x2E3u HEX, 739u, 739, (2, E, 3), 0xD1Cu
#define PPUTLIMPL_UINT_TRAIT_0x2E2u HEX, 738u, 738, (2, E, 2), 0xD1Du
#define PPUTLIMPL_UINT_TRAIT_0x2E1u HEX, 737u, 737, (2, E, 1), 0xD1Eu
#define PPUTLIMPL_UINT_TRAIT_0x2E0u HEX, 736u, 736, (2, E, 0), 0xD1Fu
#define PPUTLIMPL_UINT_TRAIT_0x2DFu HEX, 735u, 735, (2, D, F), 0xD20u
#define PPUTLIMPL_UINT_TRAIT_0x2DEu HEX, 734u, 734, (2, D, E), 0xD21u
#define PPUTLIMPL_UINT_TRAIT_0x2DDu HEX, 733u, 733, (2, D, D), 0xD22u
#define PPUTLIMPL_UINT_TRAIT_0x2DCu HEX, 732u, 732, (2, D, C), 0xD23u
#define PPUTLIMPL_UINT_TRAIT_0x2DBu HEX, 731u, 731, (2, D, B), 0xD24u
#define PPUTLIMPL_UINT_TRAIT_0x2DAu HEX, 730u, 730, (2, D, A), 0xD25u
#define PPUTLIMPL_UINT_TRAIT_0x2D9u HEX, 729u, 729, (2, D, 9), 0xD26u
#define PPUTLIMPL_UINT_TRAIT_0x2D8u HEX, 728u, 728, (2, D, 8), 0xD27u
#define PPUTLIMPL_UINT_TRAIT_0x2D7u HEX, 727u, 727, (2, D, 7), 0xD28u
#define PPUTLIMPL_UINT_TRAIT_0x2D6u HEX, 726u, 726, (2, D, 6), 0xD29u
#define PPUTLIMPL_UINT_TRAIT_0x2D5u HEX, 725u, 725, (2, D, 5), 0xD2Au
#define PPUTLIMPL_UINT_TRAIT_0x2D4u HEX, 724u, 724, (2, D, 4), 0xD2Bu
#define PPUTLIMPL_UINT_TRAIT_0x2D3u HEX, 723u, 723, (2, D, 3), 0xD2Cu
#define PPUTLIMPL_UINT_TRAIT_0x2D2u HEX, 722u, 722, (2, D, 2), 0xD2Du
#define PPUTLIMPL_UINT_TRAIT_0x2D1u HEX, 721u, 721, (2, D, 1), 0xD2Eu
#define PPUTLIMPL_UINT_TRAIT_0x2D0u HEX, 720u, 720, (2, D, 0), 0xD2Fu
#define PPUTLIMPL_UINT_TRAIT_0x2CFu HEX, 719u, 719, (2, C, F), 0xD30u
#define PPUTLIMPL_UINT_TRAIT_0x2CEu HEX, 718u, 718, (2, C, E), 0xD31u
#define PPUTLIMPL_UINT_TRAIT_0x2CDu HEX, 717u, 717, (2, C, D), 0xD32u
#define PPUTLIMPL_UINT_TRAIT_0x2CCu HEX, 716u, 716, (2, C, C), 0xD33u
#define PPUTLIMPL_UINT_TRAIT_0x2CBu HEX, 715u, 715, (2, C, B), 0xD34u
#define PPUTLIMPL_UINT_TRAIT_0x2CAu HEX, 714u, 714, (2, C, A), 0xD35u
#define PPUTLIMPL_UINT_TRAIT_0x2C9u HEX, 713u, 713, (2, C, 9), 0xD36u
#define PPUTLIMPL_UINT_TRAIT_0x2C8u HEX, 712u, 712, (2, C, 8), 0xD37u
#define PPUTLIMPL_UINT_TRAIT_0x2C7u HEX, 711u, 711, (2, C, 7), 0xD38u
#define PPUTLIMPL_UINT_TRAIT_0x2C6u HEX, 710u, 710, (2, C, 6), 0xD39u
#define PPUTLIMPL_UINT_TRAIT_0x2C5u HEX, 709u, 709, (2, C, 5), 0xD3Au
#define PPUTLIMPL_UINT_TRAIT_0x2C4u HEX, 708u, 708, (2, C, 4), 0xD3Bu
#define PPUTLIMPL_UINT_TRAIT_0x2C3u HEX, 707u, 707, (2, C, 3), 0xD3Cu
#define PPUTLIMPL_UINT_TRAIT_0x2C2u HEX, 706u, 706, (2, C, 2), 0xD3Du
#define PPUTLIMPL_UINT_TRAIT_0x2C1u HEX, 705u, 705, (2, C, 1), 0xD3Eu
#define PPUTLIMPL_UINT_TRAIT_0x2C0u HEX, 704u, 704, (2, C, 0), 0xD3Fu
#define PPUTLIMPL_UINT_TRAIT_0x2BFu HEX, 703u, 703, (2, B, F), 0xD40u
#define PPUTLIMPL_UINT_TRAIT_0x2BEu HEX, 702u, 702, (2, B, E), 0xD41u
#define PPUTLIMPL_UINT_TRAIT_0x2BDu HEX, 701u, 701, (2, B, D), 0xD42u
#define PPUTLIMPL_UINT_TRAIT_0x2BCu HEX, 700u, 700, (2, B, C), 0xD43u
#define PPUTLIMPL_UINT_TRAIT_0x2BBu HEX, 699u, 699, (2, B, B), 0xD44u
#define PPUTLIMPL_UINT_TRAIT_0x2BAu HEX, 698u, 698, (2, B, A), 0xD45u
#define PPUTLIMPL_UINT_TRAIT_0x2B9u HEX, 697u, 697, (2, B, 9), 0xD46u
#define PPUTLIMPL_UINT_TRAIT_0x2B8u HEX, 696u, 696, (2, B, 8), 0xD47u
#define PPUTLIMPL_UINT_TRAIT_0x2B7u HEX, 695u, 695, (2, B, 7), 0xD48u
#define PPUTLIMPL_UINT_TRAIT_0x2B6u HEX, 694u, 694, (2, B, 6), 0xD49u
#define PPUTLIMPL_UINT_TRAIT_0x2B5u HEX, 693u, 693, (2, B, 5), 0xD4Au
#define PPUTLIMPL_UINT_TRAIT_0x2B4u HEX, 692u, 692, (2, B, 4), 0xD4Bu
#define PPUTLIMPL_UINT_TRAIT_0x2B3u HEX, 691u, 691, (2, B, 3), 0xD4Cu
#define PPUTLIMPL_UINT_TRAIT_0x2B2u HEX, 690u, 690, (2, B, 2), 0xD4Du
#define PPUTLIMPL_UINT_TRAIT_0x2B1u HEX, 689u, 689, (2, B, 1), 0xD4Eu
#define PPUTLIMPL_UINT_TRAIT_0x2B0u HEX, 688u, 688, (2, B, 0), 0xD4Fu
#define PPUTLIMPL_UINT_TRAIT_0x2AFu HEX, 687u, 687, (2, A, F), 0xD50u
#define PPUTLIMPL_UINT_TRAIT_0x2AEu HEX, 686u, 686, (2, A, E), 0xD51u
#define PPUTLIMPL_UINT_TRAIT_0x2ADu HEX, 685u, 685, (2, A, D), 0xD52u
#define PPUTLIMPL_UINT_TRAIT_0x2ACu HEX, 684u, 684, (2, A, C), 0xD53u
#define PPUTLIMPL_UINT_TRAIT_0x2ABu HEX, 683u, 683, (2, A, B), 0xD54u
#define PPUTLIMPL_UINT_TRAIT_0x2AAu HEX, 682u, 682, (2, A, A), 0xD55u
#define PPUTLIMPL_UINT_TRAIT_0x2A9u HEX, 681u, 681, (2, A, 9), 0xD56u
#define PPUTLIMPL_UINT_TRAIT_0x2A8u HEX, 680u, 680, (2, A, 8), 0xD57u
#define PPUTLIMPL_UINT_TRAIT_0x2A7u HEX, 679u, 679, (2, A, 7), 0xD58u
#define PPUTLIMPL_UINT_TRAIT_0x2A6u HEX, 678u, 678, (2, A, 6), 0xD59u
#define PPUTLIMPL_UINT_TRAIT_0x2A5u HEX, 677u, 677, (2, A, 5), 0xD5Au
#define PPUTLIMPL_UINT_TRAIT_0x2A4u HEX, 676u, 676, (2, A, 4), 0xD5Bu
#define PPUTLIMPL_UINT_TRAIT_0x2A3u HEX, 675u, 675, (2, A, 3), 0xD5Cu
#define PPUTLIMPL_UINT_TRAIT_0x2A2u HEX, 674u, 674, (2, A, 2), 0xD5Du
#define PPUTLIMPL_UINT_TRAIT_0x2A1u HEX, 673u, 673, (2, A, 1), 0xD5Eu
#define PPUTLIMPL_UINT_TRAIT_0x2A0u HEX, 672u, 672, (2, A, 0), 0xD5Fu
#define PPUTLIMPL_UINT_TRAIT_0x29Fu HEX, 671u, 671, (2, 9, F), 0xD60u
#define PPUTLIMPL_UINT_TRAIT_0x29Eu HEX, 670u, 670, (2, 9, E), 0xD61u
#define PPUTLIMPL_UINT_TRAIT_0x29Du HEX, 669u, 669, (2, 9, D), 0xD62u
#define PPUTLIMPL_UINT_TRAIT_0x29Cu HEX, 668u, 668, (2, 9, C), 0xD63u
#define PPUTLIMPL_UINT_TRAIT_0x29Bu HEX, 667u, 667, (2, 9, B), 0xD64u
#define PPUTLIMPL_UINT_TRAIT_0x29Au HEX, 666u, 666, (2, 9, A), 0xD65u
#define PPUTLIMPL_UINT_TRAIT_0x299u HEX, 665u, 665, (2, 9, 9), 0xD66u
#define PPUTLIMPL_UINT_TRAIT_0x298u HEX, 664u, 664, (2, 9, 8), 0xD67u
#define PPUTLIMPL_UINT_TRAIT_0x297u HEX, 663u, 663, (2, 9, 7), 0xD68u
#define PPUTLIMPL_UINT_TRAIT_0x296u HEX, 662u, 662, (2, 9, 6), 0xD69u
#define PPUTLIMPL_UINT_TRAIT_0x295u HEX, 661u, 661, (2, 9, 5), 0xD6Au
#define PPUTLIMPL_UINT_TRAIT_0x294u HEX, 660u, 660, (2, 9, 4), 0xD6Bu
#define PPUTLIMPL_UINT_TRAIT_0x293u HEX, 659u, 659, (2, 9, 3), 0xD6Cu
#define PPUTLIMPL_UINT_TRAIT_0x292u HEX, 658u, 658, (2, 9, 2), 0xD6Du
#define PPUTLIMPL_UINT_TRAIT_0x291u HEX, 657u, 657, (2, 9, 1), 0xD6Eu
#define PPUTLIMPL_UINT_TRAIT_0x290u HEX, 656u, 656, (2, 9, 0), 0xD6Fu
#define PPUTLIMPL_UINT_TRAIT_0x28Fu HEX, 655u, 655, (2, 8, F), 0xD70u
#define PPUTLIMPL_UINT_TRAIT_0x28Eu HEX, 654u, 654, (2, 8, E), 0xD71u
#define PPUTLIMPL_UINT_TRAIT_0x28Du HEX, 653u, 653, (2, 8, D), 0xD72u
#define PPUTLIMPL_UINT_TRAIT_0x28Cu HEX, 652u, 652, (2, 8, C), 0xD73u
#define PPUTLIMPL_UINT_TRAIT_0x28Bu HEX, 651u, 651, (2, 8, B), 0xD74u
#define PPUTLIMPL_UINT_TRAIT_0x28Au HEX, 650u, 650, (2, 8, A), 0xD75u
#define PPUTLIMPL_UINT_TRAIT_0x289u HEX, 649u, 649, (2, 8, 9), 0xD76u
#define PPUTLIMPL_UINT_TRAIT_0x288u HEX, 648u, 648, (2, 8, 8), 0xD77u
#define PPUTLIMPL_UINT_TRAIT_0x287u HEX, 647u, 647, (2, 8, 7), 0xD78u
#define PPUTLIMPL_UINT_TRAIT_0x286u HEX, 646u, 646, (2, 8, 6), 0xD79u
#define PPUTLIMPL_UINT_TRAIT_0x285u HEX, 645u, 645, (2, 8, 5), 0xD7Au
#define PPUTLIMPL_UINT_TRAIT_0x284u HEX, 644u, 644, (2, 8, 4), 0xD7Bu
#define PPUTLIMPL_UINT_TRAIT_0x283u HEX, 643u, 643, (2, 8, 3), 0xD7Cu
#define PPUTLIMPL_UINT_TRAIT_0x282u HEX, 642u, 642, (2, 8, 2), 0xD7Du
#define PPUTLIMPL_UINT_TRAIT_0x281u HEX, 641u, 641, (2, 8, 1), 0xD7Eu
#define PPUTLIMPL_UINT_TRAIT_0x280u HEX, 640u, 640, (2, 8, 0), 0xD7Fu
#define PPUTLIMPL_UINT_TRAIT_0x27Fu HEX, 639u, 639, (2, 7, F), 0xD80u
#define PPUTLIMPL_UINT_TRAIT_0x27Eu HEX, 638u, 638, (2, 7, E), 0xD81u
#define PPUTLIMPL_UINT_TRAIT_0x27Du HEX, 637u, 637, (2, 7, D), 0xD82u
#define PPUTLIMPL_UINT_TRAIT_0x27Cu HEX, 636u, 636, (2, 7, C), 0xD83u
#define PPUTLIMPL_UINT_TRAIT_0x27Bu HEX, 635u, 635, (2, 7, B), 0xD84u
#define PPUTLIMPL_UINT_TRAIT_0x27Au HEX, 634u, 634, (2, 7, A), 0xD85u
#define PPUTLIMPL_UINT_TRAIT_0x279u HEX, 633u, 633, (2, 7, 9), 0xD86u
#define PPUTLIMPL_UINT_TRAIT_0x278u HEX, 632u, 632, (2, 7, 8), 0xD87u
#define PPUTLIMPL_UINT_TRAIT_0x277u HEX, 631u, 631, (2, 7, 7), 0xD88u
#define PPUTLIMPL_UINT_TRAIT_0x276u HEX, 630u, 630, (2, 7, 6), 0xD89u
#define PPUTLIMPL_UINT_TRAIT_0x275u HEX, 629u, 629, (2, 7, 5), 0xD8Au
#define PPUTLIMPL_UINT_TRAIT_0x274u HEX, 628u, 628, (2, 7, 4), 0xD8Bu
#define PPUTLIMPL_UINT_TRAIT_0x273u HEX, 627u, 627, (2, 7, 3), 0xD8Cu
#define PPUTLIMPL_UINT_TRAIT_0x272u HEX, 626u, 626, (2, 7, 2), 0xD8Du
#define PPUTLIMPL_UINT_TRAIT_0x271u HEX, 625u, 625, (2, 7, 1), 0xD8Eu
#define PPUTLIMPL_UINT_TRAIT_0x270u HEX, 624u, 624, (2, 7, 0), 0xD8Fu
#define PPUTLIMPL_UINT_TRAIT_0x26Fu HEX, 623u, 623, (2, 6, F), 0xD90u
#define PPUTLIMPL_UINT_TRAIT_0x26Eu HEX, 622u, 622, (2, 6, E), 0xD91u
#define PPUTLIMPL_UINT_TRAIT_0x26Du HEX, 621u, 621, (2, 6, D), 0xD92u
#define PPUTLIMPL_UINT_TRAIT_0x26Cu HEX, 620u, 620, (2, 6, C), 0xD93u
#define PPUTLIMPL_UINT_TRAIT_0x26Bu HEX, 619u, 619, (2, 6, B), 0xD94u
#define PPUTLIMPL_UINT_TRAIT_0x26Au HEX, 618u, 618, (2, 6, A), 0xD95u
#define PPUTLIMPL_UINT_TRAIT_0x269u HEX, 617u, 617, (2, 6, 9), 0xD96u
#define PPUTLIMPL_UINT_TRAIT_0x268u HEX, 616u, 616, (2, 6, 8), 0xD97u
#define PPUTLIMPL_UINT_TRAIT_0x267u HEX, 615u, 615, (2, 6, 7), 0xD98u
#define PPUTLIMPL_UINT_TRAIT_0x266u HEX, 614u, 614, (2, 6, 6), 0xD99u
#define PPUTLIMPL_UINT_TRAIT_0x265u HEX, 613u, 613, (2, 6, 5), 0xD9Au
#define PPUTLIMPL_UINT_TRAIT_0x264u HEX, 612u, 612, (2, 6, 4), 0xD9Bu
#define PPUTLIMPL_UINT_TRAIT_0x263u HEX, 611u, 611, (2, 6, 3), 0xD9Cu
#define PPUTLIMPL_UINT_TRAIT_0x262u HEX, 610u, 610, (2, 6, 2), 0xD9Du
#define PPUTLIMPL_UINT_TRAIT_0x261u HEX, 609u, 609, (2, 6, 1), 0xD9Eu
#define PPUTLIMPL_UINT_TRAIT_0x260u HEX, 608u, 608, (2, 6, 0), 0xD9Fu
#define PPUTLIMPL_UINT_TRAIT_0x25Fu HEX, 607u, 607, (2, 5, F), 0xDA0u
#define PPUTLIMPL_UINT_TRAIT_0x25Eu HEX, 606u, 606, (2, 5, E), 0xDA1u
#define PPUTLIMPL_UINT_TRAIT_0x25Du HEX, 605u, 605, (2, 5, D), 0xDA2u
#define PPUTLIMPL_UINT_TRAIT_0x25Cu HEX, 604u, 604, (2, 5, C), 0xDA3u
#define PPUTLIMPL_UINT_TRAIT_0x25Bu HEX, 603u, 603, (2, 5, B), 0xDA4u
#define PPUTLIMPL_UINT_TRAIT_0x25Au HEX, 602u, 602, (2, 5, A), 0xDA5u
#define PPUTLIMPL_UINT_TRAIT_0x259u HEX, 601u, 601, (2, 5, 9), 0xDA6u
#define PPUTLIMPL_UINT_TRAIT_0x258u HEX, 600u, 600, (2, 5, 8), 0xDA7u
#define PPUTLIMPL_UINT_TRAIT_0x257u HEX, 599u, 599, (2, 5, 7), 0xDA8u
#define PPUTLIMPL_UINT_TRAIT_0x256u HEX, 598u, 598, (2, 5, 6), 0xDA9u
#define PPUTLIMPL_UINT_TRAIT_0x255u HEX, 597u, 597, (2, 5, 5), 0xDAAu
#define PPUTLIMPL_UINT_TRAIT_0x254u HEX, 596u, 596, (2, 5, 4), 0xDABu
#define PPUTLIMPL_UINT_TRAIT_0x253u HEX, 595u, 595, (2, 5, 3), 0xDACu
#define PPUTLIMPL_UINT_TRAIT_0x252u HEX, 594u, 594, (2, 5, 2), 0xDADu
#define PPUTLIMPL_UINT_TRAIT_0x251u HEX, 593u, 593, (2, 5, 1), 0xDAEu
#define PPUTLIMPL_UINT_TRAIT_0x250u HEX, 592u, 592, (2, 5, 0), 0xDAFu
#define PPUTLIMPL_UINT_TRAIT_0x24Fu HEX, 591u, 591, (2, 4, F), 0xDB0u
#define PPUTLIMPL_UINT_TRAIT_0x24Eu HEX, 590u, 590, (2, 4, E), 0xDB1u
#define PPUTLIMPL_UINT_TRAIT_0x24Du HEX, 589u, 589, (2, 4, D), 0xDB2u
#define PPUTLIMPL_UINT_TRAIT_0x24Cu HEX, 588u, 588, (2, 4, C), 0xDB3u
#define PPUTLIMPL_UINT_TRAIT_0x24Bu HEX, 587u, 587, (2, 4, B), 0xDB4u
#define PPUTLIMPL_UINT_TRAIT_0x24Au HEX, 586u, 586, (2, 4, A), 0xDB5u
#define PPUTLIMPL_UINT_TRAIT_0x249u HEX, 585u, 585, (2, 4, 9), 0xDB6u
#define PPUTLIMPL_UINT_TRAIT_0x248u HEX, 584u, 584, (2, 4, 8), 0xDB7u
#define PPUTLIMPL_UINT_TRAIT_0x247u HEX, 583u, 583, (2, 4, 7), 0xDB8u
#define PPUTLIMPL_UINT_TRAIT_0x246u HEX, 582u, 582, (2, 4, 6), 0xDB9u
#define PPUTLIMPL_UINT_TRAIT_0x245u HEX, 581u, 581, (2, 4, 5), 0xDBAu
#define PPUTLIMPL_UINT_TRAIT_0x244u HEX, 580u, 580, (2, 4, 4), 0xDBBu
#define PPUTLIMPL_UINT_TRAIT_0x243u HEX, 579u, 579, (2, 4, 3), 0xDBCu
#define PPUTLIMPL_UINT_TRAIT_0x242u HEX, 578u, 578, (2, 4, 2), 0xDBDu
#define PPUTLIMPL_UINT_TRAIT_0x241u HEX, 577u, 577, (2, 4, 1), 0xDBEu
#define PPUTLIMPL_UINT_TRAIT_0x240u HEX, 576u, 576, (2, 4, 0), 0xDBFu
#define PPUTLIMPL_UINT_TRAIT_0x23Fu HEX, 575u, 575, (2, 3, F), 0xDC0u
#define PPUTLIMPL_UINT_TRAIT_0x23Eu HEX, 574u, 574, (2, 3, E), 0xDC1u
#define PPUTLIMPL_UINT_TRAIT_0x23Du HEX, 573u, 573, (2, 3, D), 0xDC2u
#define PPUTLIMPL_UINT_TRAIT_0x23Cu HEX, 572u, 572, (2, 3, C), 0xDC3u
#define PPUTLIMPL_UINT_TRAIT_0x23Bu HEX, 571u, 571, (2, 3, B), 0xDC4u
#define PPUTLIMPL_UINT_TRAIT_0x23Au HEX, 570u, 570, (2, 3, A), 0xDC5u
#define PPUTLIMPL_UINT_TRAIT_0x239u HEX, 569u, 569, (2, 3, 9), 0xDC6u
#define PPUTLIMPL_UINT_TRAIT_0x238u HEX, 568u, 568, (2, 3, 8), 0xDC7u
#define PPUTLIMPL_UINT_TRAIT_0x237u HEX, 567u, 567, (2, 3, 7), 0xDC8u
#define PPUTLIMPL_UINT_TRAIT_0x236u HEX, 566u, 566, (2, 3, 6), 0xDC9u
#define PPUTLIMPL_UINT_TRAIT_0x235u HEX, 565u, 565, (2, 3, 5), 0xDCAu
#define PPUTLIMPL_UINT_TRAIT_0x234u HEX, 564u, 564, (2, 3, 4), 0xDCBu
#define PPUTLIMPL_UINT_TRAIT_0x233u HEX, 563u, 563, (2, 3, 3), 0xDCCu
#define PPUTLIMPL_UINT_TRAIT_0x232u HEX, 562u, 562, (2, 3, 2), 0xDCDu
#define PPUTLIMPL_UINT_TRAIT_0x231u HEX, 561u, 561, (2, 3, 1), 0xDCEu
#define PPUTLIMPL_UINT_TRAIT_0x230u HEX, 560u, 560, (2, 3, 0), 0xDCFu
#define PPUTLIMPL_UINT_TRAIT_0x22Fu HEX, 559u, 559, (2, 2, F), 0xDD0u
#define PPUTLIMPL_UINT_TRAIT_0x22Eu HEX, 558u, 558, (2, 2, E), 0xDD1u
#define PPUTLIMPL_UINT_TRAIT_0x22Du HEX, 557u, 557, (2, 2, D), 0xDD2u
#define PPUTLIMPL_UINT_TRAIT_0x22Cu HEX, 556u, 556, (2, 2, C), 0xDD3u
#define PPUTLIMPL_UINT_TRAIT_0x22Bu HEX, 555u, 555, (2, 2, B), 0xDD4u
#define PPUTLIMPL_UINT_TRAIT_0x22Au HEX, 554u, 554, (2, 2, A), 0xDD5u
#define PPUTLIMPL_UINT_TRAIT_0x229u HEX, 553u, 553, (2, 2, 9), 0xDD6u
#define PPUTLIMPL_UINT_TRAIT_0x228u HEX, 552u, 552, (2, 2, 8), 0xDD7u
#define PPUTLIMPL_UINT_TRAIT_0x227u HEX, 551u, 551, (2, 2, 7), 0xDD8u
#define PPUTLIMPL_UINT_TRAIT_0x226u HEX, 550u, 550, (2, 2, 6), 0xDD9u
#define PPUTLIMPL_UINT_TRAIT_0x225u HEX, 549u, 549, (2, 2, 5), 0xDDAu
#define PPUTLIMPL_UINT_TRAIT_0x224u HEX, 548u, 548, (2, 2, 4), 0xDDBu
#define PPUTLIMPL_UINT_TRAIT_0x223u HEX, 547u, 547, (2, 2, 3), 0xDDCu
#define PPUTLIMPL_UINT_TRAIT_0x222u HEX, 546u, 546, (2, 2, 2), 0xDDDu
#define PPUTLIMPL_UINT_TRAIT_0x221u HEX, 545u, 545, (2, 2, 1), 0xDDEu
#define PPUTLIMPL_UINT_TRAIT_0x220u HEX, 544u, 544, (2, 2, 0), 0xDDFu
#define PPUTLIMPL_UINT_TRAIT_0x21Fu HEX, 543u, 543, (2, 1, F), 0xDE0u
#define PPUTLIMPL_UINT_TRAIT_0x21Eu HEX, 542u, 542, (2, 1, E), 0xDE1u
#define PPUTLIMPL_UINT_TRAIT_0x21Du HEX, 541u, 541, (2, 1, D), 0xDE2u
#define PPUTLIMPL_UINT_TRAIT_0x21Cu HEX, 540u, 540, (2, 1, C), 0xDE3u
#define PPUTLIMPL_UINT_TRAIT_0x21Bu HEX, 539u, 539, (2, 1, B), 0xDE4u
#define PPUTLIMPL_UINT_TRAIT_0x21Au HEX, 538u, 538, (2, 1, A), 0xDE5u
#define PPUTLIMPL_UINT_TRAIT_0x219u HEX, 537u, 537, (2, 1, 9), 0xDE6u
#define PPUTLIMPL_UINT_TRAIT_0x218u HEX, 536u, 536, (2, 1, 8), 0xDE7u
#define PPUTLIMPL_UINT_TRAIT_0x217u HEX, 535u, 535, (2, 1, 7), 0xDE8u
#define PPUTLIMPL_UINT_TRAIT_0x216u HEX, 534u, 534, (2, 1, 6), 0xDE9u
#define PPUTLIMPL_UINT_TRAIT_0x215u HEX, 533u, 533, (2, 1, 5), 0xDEAu
#define PPUTLIMPL_UINT_TRAIT_0x214u HEX, 532u, 532, (2, 1, 4), 0xDEBu
#define PPUTLIMPL_UINT_TRAIT_0x213u HEX, 531u, 531, (2, 1, 3), 0xDECu
#define PPUTLIMPL_UINT_TRAIT_0x212u HEX, 530u, 530, (2, 1, 2), 0xDEDu
#define PPUTLIMPL_UINT_TRAIT_0x211u HEX, 529u, 529, (2, 1, 1), 0xDEEu
#define PPUTLIMPL_UINT_TRAIT_0x210u HEX, 528u, 528, (2, 1, 0), 0xDEFu
#define PPUTLIMPL_UINT_TRAIT_0x20Fu HEX, 527u, 527, (2, 0, F), 0xDF0u
#define PPUTLIMPL_UINT_TRAIT_0x20Eu HEX, 526u, 526, (2, 0, E), 0xDF1u
#define PPUTLIMPL_UINT_TRAIT_0x20Du HEX, 525u, 525, (2, 0, D), 0xDF2u
#define PPUTLIMPL_UINT_TRAIT_0x20Cu HEX, 524u, 524, (2, 0, C), 0xDF3u
#define PPUTLIMPL_UINT_TRAIT_0x20Bu HEX, 523u, 523, (2, 0, B), 0xDF4u
#define PPUTLIMPL_UINT_TRAIT_0x20Au HEX, 522u, 522, (2, 0, A), 0xDF5u
#define PPUTLIMPL_UINT_TRAIT_0x209u HEX, 521u, 521, (2, 0, 9), 0xDF6u
#define PPUTLIMPL_UINT_TRAIT_0x208u HEX, 520u, 520, (2, 0, 8), 0xDF7u
#define PPUTLIMPL_UINT_TRAIT_0x207u HEX, 519u, 519, (2, 0, 7), 0xDF8u
#define PPUTLIMPL_UINT_TRAIT_0x206u HEX, 518u, 518, (2, 0, 6), 0xDF9u
#define PPUTLIMPL_UINT_TRAIT_0x205u HEX, 517u, 517, (2, 0, 5), 0xDFAu
#define PPUTLIMPL_UINT_TRAIT_0x204u HEX, 516u, 516, (2, 0, 4), 0xDFBu
#define PPUTLIMPL_UINT_TRAIT_0x203u HEX, 515u, 515, (2, 0, 3), 0xDFCu
#define PPUTLIMPL_UINT_TRAIT_0x202u HEX, 514u, 514, (2, 0, 2), 0xDFDu
#define PPUTLIMPL_UINT_TRAIT_0x201u HEX, 513u, 513, (2, 0, 1), 0xDFEu
#define PPUTLIMPL_UINT_TRAIT_0x200u HEX, 512u, 512, (2, 0, 0), 0xDFFu
#define PPUTLIMPL_UINT_TRAIT_0x1FFu HEX, 511u, 511, (1, F, F), 0xE00u
#define PPUTLIMPL_UINT_TRAIT_0x1FEu HEX, 510u, 510, (1, F, E), 0xE01u
#define PPUTLIMPL_UINT_TRAIT_0x1FDu HEX, 509u, 509, (1, F, D), 0xE02u
#define PPUTLIMPL_UINT_TRAIT_0x1FCu HEX, 508u, 508, (1, F, C), 0xE03u
#define PPUTLIMPL_UINT_TRAIT_0x1FBu HEX, 507u, 507, (1, F, B), 0xE04u
#define PPUTLIMPL_UINT_TRAIT_0x1FAu HEX, 506u, 506, (1, F, A), 0xE05u
#define PPUTLIMPL_UINT_TRAIT_0x1F9u HEX, 505u, 505, (1, F, 9), 0xE06u
#define PPUTLIMPL_UINT_TRAIT_0x1F8u HEX, 504u, 504, (1, F, 8), 0xE07u
#define PPUTLIMPL_UINT_TRAIT_0x1F7u HEX, 503u, 503, (1, F, 7), 0xE08u
#define PPUTLIMPL_UINT_TRAIT_0x1F6u HEX, 502u, 502, (1, F, 6), 0xE09u
#define PPUTLIMPL_UINT_TRAIT_0x1F5u HEX, 501u, 501, (1, F, 5), 0xE0Au
#define PPUTLIMPL_UINT_TRAIT_0x1F4u HEX, 500u, 500, (1, F, 4), 0xE0Bu
#define PPUTLIMPL_UINT_TRAIT_0x1F3u HEX, 499u, 499, (1, F, 3), 0xE0Cu
#define PPUTLIMPL_UINT_TRAIT_0x1F2u HEX, 498u, 498, (1, F, 2), 0xE0Du
#define PPUTLIMPL_UINT_TRAIT_0x1F1u HEX, 497u, 497, (1, F, 1), 0xE0Eu
#define PPUTLIMPL_UINT_TRAIT_0x1F0u HEX, 496u, 496, (1, F, 0), 0xE0Fu
#define PPUTLIMPL_UINT_TRAIT_0x1EFu HEX, 495u, 495, (1, E, F), 0xE10u
#define PPUTLIMPL_UINT_TRAIT_0x1EEu HEX, 494u, 494, (1, E, E), 0xE11u
#define PPUTLIMPL_UINT_TRAIT_0x1EDu HEX, 493u, 493, (1, E, D), 0xE12u
#define PPUTLIMPL_UINT_TRAIT_0x1ECu HEX, 492u, 492, (1, E, C), 0xE13u
#define PPUTLIMPL_UINT_TRAIT_0x1EBu HEX, 491u, 491, (1, E, B), 0xE14u
#define PPUTLIMPL_UINT_TRAIT_0x1EAu HEX, 490u, 490, (1, E, A), 0xE15u
#define PPUTLIMPL_UINT_TRAIT_0x1E9u HEX, 489u, 489, (1, E, 9), 0xE16u
#define PPUTLIMPL_UINT_TRAIT_0x1E8u HEX, 488u, 488, (1, E, 8), 0xE17u
#define PPUTLIMPL_UINT_TRAIT_0x1E7u HEX, 487u, 487, (1, E, 7), 0xE18u
#define PPUTLIMPL_UINT_TRAIT_0x1E6u HEX, 486u, 486, (1, E, 6), 0xE19u
#define PPUTLIMPL_UINT_TRAIT_0x1E5u HEX, 485u, 485, (1, E, 5), 0xE1Au
#define PPUTLIMPL_UINT_TRAIT_0x1E4u HEX, 484u, 484, (1, E, 4), 0xE1Bu
#define PPUTLIMPL_UINT_TRAIT_0x1E3u HEX, 483u, 483, (1, E, 3), 0xE1Cu
#define PPUTLIMPL_UINT_TRAIT_0x1E2u HEX, 482u, 482, (1, E, 2), 0xE1Du
#define PPUTLIMPL_UINT_TRAIT_0x1E1u HEX, 481u, 481, (1, E, 1), 0xE1Eu
#define PPUTLIMPL_UINT_TRAIT_0x1E0u HEX, 480u, 480, (1, E, 0), 0xE1Fu
#define PPUTLIMPL_UINT_TRAIT_0x1DFu HEX, 479u, 479, (1, D, F), 0xE20u
#define PPUTLIMPL_UINT_TRAIT_0x1DEu HEX, 478u, 478, (1, D, E), 0xE21u
#define PPUTLIMPL_UINT_TRAIT_0x1DDu HEX, 477u, 477, (1, D, D), 0xE22u
#define PPUTLIMPL_UINT_TRAIT_0x1DCu HEX, 476u, 476, (1, D, C), 0xE23u
#define PPUTLIMPL_UINT_TRAIT_0x1DBu HEX, 475u, 475, (1, D, B), 0xE24u
#define PPUTLIMPL_UINT_TRAIT_0x1DAu HEX, 474u, 474, (1, D, A), 0xE25u
#define PPUTLIMPL_UINT_TRAIT_0x1D9u HEX, 473u, 473, (1, D, 9), 0xE26u
#define PPUTLIMPL_UINT_TRAIT_0x1D8u HEX, 472u, 472, (1, D, 8), 0xE27u
#define PPUTLIMPL_UINT_TRAIT_0x1D7u HEX, 471u, 471, (1, D, 7), 0xE28u
#define PPUTLIMPL_UINT_TRAIT_0x1D6u HEX, 470u, 470, (1, D, 6), 0xE29u
#define PPUTLIMPL_UINT_TRAIT_0x1D5u HEX, 469u, 469, (1, D, 5), 0xE2Au
#define PPUTLIMPL_UINT_TRAIT_0x1D4u HEX, 468u, 468, (1, D, 4), 0xE2Bu
#define PPUTLIMPL_UINT_TRAIT_0x1D3u HEX, 467u, 467, (1, D, 3), 0xE2Cu
#define PPUTLIMPL_UINT_TRAIT_0x1D2u HEX, 466u, 466, (1, D, 2), 0xE2Du
#define PPUTLIMPL_UINT_TRAIT_0x1D1u HEX, 465u, 465, (1, D, 1), 0xE2Eu
#define PPUTLIMPL_UINT_TRAIT_0x1D0u HEX, 464u, 464, (1, D, 0), 0xE2Fu
#define PPUTLIMPL_UINT_TRAIT_0x1CFu HEX, 463u, 463, (1, C, F), 0xE30u
#define PPUTLIMPL_UINT_TRAIT_0x1CEu HEX, 462u, 462, (1, C, E), 0xE31u
#define PPUTLIMPL_UINT_TRAIT_0x1CDu HEX, 461u, 461, (1, C, D), 0xE32u
#define PPUTLIMPL_UINT_TRAIT_0x1CCu HEX, 460u, 460, (1, C, C), 0xE33u
#define PPUTLIMPL_UINT_TRAIT_0x1CBu HEX, 459u, 459, (1, C, B), 0xE34u
#define PPUTLIMPL_UINT_TRAIT_0x1CAu HEX, 458u, 458, (1, C, A), 0xE35u
#define PPUTLIMPL_UINT_TRAIT_0x1C9u HEX, 457u, 457, (1, C, 9), 0xE36u
#define PPUTLIMPL_UINT_TRAIT_0x1C8u HEX, 456u, 456, (1, C, 8), 0xE37u
#define PPUTLIMPL_UINT_TRAIT_0x1C7u HEX, 455u, 455, (1, C, 7), 0xE38u
#define PPUTLIMPL_UINT_TRAIT_0x1C6u HEX, 454u, 454, (1, C, 6), 0xE39u
#define PPUTLIMPL_UINT_TRAIT_0x1C5u HEX, 453u, 453, (1, C, 5), 0xE3Au
#define PPUTLIMPL_UINT_TRAIT_0x1C4u HEX, 452u, 452, (1, C, 4), 0xE3Bu
#define PPUTLIMPL_UINT_TRAIT_0x1C3u HEX, 451u, 451, (1, C, 3), 0xE3Cu
#define PPUTLIMPL_UINT_TRAIT_0x1C2u HEX, 450u, 450, (1, C, 2), 0xE3Du
#define PPUTLIMPL_UINT_TRAIT_0x1C1u HEX, 449u, 449, (1, C, 1), 0xE3Eu
#define PPUTLIMPL_UINT_TRAIT_0x1C0u HEX, 448u, 448, (1, C, 0), 0xE3Fu
#define PPUTLIMPL_UINT_TRAIT_0x1BFu HEX, 447u, 447, (1, B, F), 0xE40u
#define PPUTLIMPL_UINT_TRAIT_0x1BEu HEX, 446u, 446, (1, B, E), 0xE41u
#define PPUTLIMPL_UINT_TRAIT_0x1BDu HEX, 445u, 445, (1, B, D), 0xE42u
#define PPUTLIMPL_UINT_TRAIT_0x1BCu HEX, 444u, 444, (1, B, C), 0xE43u
#define PPUTLIMPL_UINT_TRAIT_0x1BBu HEX, 443u, 443, (1, B, B), 0xE44u
#define PPUTLIMPL_UINT_TRAIT_0x1BAu HEX, 442u, 442, (1, B, A), 0xE45u
#define PPUTLIMPL_UINT_TRAIT_0x1B9u HEX, 441u, 441, (1, B, 9), 0xE46u
#define PPUTLIMPL_UINT_TRAIT_0x1B8u HEX, 440u, 440, (1, B, 8), 0xE47u
#define PPUTLIMPL_UINT_TRAIT_0x1B7u HEX, 439u, 439, (1, B, 7), 0xE48u
#define PPUTLIMPL_UINT_TRAIT_0x1B6u HEX, 438u, 438, (1, B, 6), 0xE49u
#define PPUTLIMPL_UINT_TRAIT_0x1B5u HEX, 437u, 437, (1, B, 5), 0xE4Au
#define PPUTLIMPL_UINT_TRAIT_0x1B4u HEX, 436u, 436, (1, B, 4), 0xE4Bu
#define PPUTLIMPL_UINT_TRAIT_0x1B3u HEX, 435u, 435, (1, B, 3), 0xE4Cu
#define PPUTLIMPL_UINT_TRAIT_0x1B2u HEX, 434u, 434, (1, B, 2), 0xE4Du
#define PPUTLIMPL_UINT_TRAIT_0x1B1u HEX, 433u, 433, (1, B, 1), 0xE4Eu
#define PPUTLIMPL_UINT_TRAIT_0x1B0u HEX, 432u, 432, (1, B, 0), 0xE4Fu
#define PPUTLIMPL_UINT_TRAIT_0x1AFu HEX, 431u, 431, (1, A, F), 0xE50u
#define PPUTLIMPL_UINT_TRAIT_0x1AEu HEX, 430u, 430, (1, A, E), 0xE51u
#define PPUTLIMPL_UINT_TRAIT_0x1ADu HEX, 429u, 429, (1, A, D), 0xE52u
#define PPUTLIMPL_UINT_TRAIT_0x1ACu HEX, 428u, 428, (1, A, C), 0xE53u
#define PPUTLIMPL_UINT_TRAIT_0x1ABu HEX, 427u, 427, (1, A, B), 0xE54u
#define PPUTLIMPL_UINT_TRAIT_0x1AAu HEX, 426u, 426, (1, A, A), 0xE55u
#define PPUTLIMPL_UINT_TRAIT_0x1A9u HEX, 425u, 425, (1, A, 9), 0xE56u
#define PPUTLIMPL_UINT_TRAIT_0x1A8u HEX, 424u, 424, (1, A, 8), 0xE57u
#define PPUTLIMPL_UINT_TRAIT_0x1A7u HEX, 423u, 423, (1, A, 7), 0xE58u
#define PPUTLIMPL_UINT_TRAIT_0x1A6u HEX, 422u, 422, (1, A, 6), 0xE59u
#define PPUTLIMPL_UINT_TRAIT_0x1A5u HEX, 421u, 421, (1, A, 5), 0xE5Au
#define PPUTLIMPL_UINT_TRAIT_0x1A4u HEX, 420u, 420, (1, A, 4), 0xE5Bu
#define PPUTLIMPL_UINT_TRAIT_0x1A3u HEX, 419u, 419, (1, A, 3), 0xE5Cu
#define PPUTLIMPL_UINT_TRAIT_0x1A2u HEX, 418u, 418, (1, A, 2), 0xE5Du
#define PPUTLIMPL_UINT_TRAIT_0x1A1u HEX, 417u, 417, (1, A, 1), 0xE5Eu
#define PPUTLIMPL_UINT_TRAIT_0x1A0u HEX, 416u, 416, (1, A, 0), 0xE5Fu
#define PPUTLIMPL_UINT_TRAIT_0x19Fu HEX, 415u, 415, (1, 9, F), 0xE60u
#define PPUTLIMPL_UINT_TRAIT_0x19Eu HEX, 414u, 414, (1, 9, E), 0xE61u
#define PPUTLIMPL_UINT_TRAIT_0x19Du HEX, 413u, 413, (1, 9, D), 0xE62u
#define PPUTLIMPL_UINT_TRAIT_0x19Cu HEX, 412u, 412, (1, 9, C), 0xE63u
#define PPUTLIMPL_UINT_TRAIT_0x19Bu HEX, 411u, 411, (1, 9, B), 0xE64u
#define PPUTLIMPL_UINT_TRAIT_0x19Au HEX, 410u, 410, (1, 9, A), 0xE65u
#define PPUTLIMPL_UINT_TRAIT_0x199u HEX, 409u, 409, (1, 9, 9), 0xE66u
#define PPUTLIMPL_UINT_TRAIT_0x198u HEX, 408u, 408, (1, 9, 8), 0xE67u
#define PPUTLIMPL_UINT_TRAIT_0x197u HEX, 407u, 407, (1, 9, 7), 0xE68u
#define PPUTLIMPL_UINT_TRAIT_0x196u HEX, 406u, 406, (1, 9, 6), 0xE69u
#define PPUTLIMPL_UINT_TRAIT_0x195u HEX, 405u, 405, (1, 9, 5), 0xE6Au
#define PPUTLIMPL_UINT_TRAIT_0x194u HEX, 404u, 404, (1, 9, 4), 0xE6Bu
#define PPUTLIMPL_UINT_TRAIT_0x193u HEX, 403u, 403, (1, 9, 3), 0xE6Cu
#define PPUTLIMPL_UINT_TRAIT_0x192u HEX, 402u, 402, (1, 9, 2), 0xE6Du
#define PPUTLIMPL_UINT_TRAIT_0x191u HEX, 401u, 401, (1, 9, 1), 0xE6Eu
#define PPUTLIMPL_UINT_TRAIT_0x190u HEX, 400u, 400, (1, 9, 0), 0xE6Fu
#define PPUTLIMPL_UINT_TRAIT_0x18Fu HEX, 399u, 399, (1, 8, F), 0xE70u
#define PPUTLIMPL_UINT_TRAIT_0x18Eu HEX, 398u, 398, (1, 8, E), 0xE71u
#define PPUTLIMPL_UINT_TRAIT_0x18Du HEX, 397u, 397, (1, 8, D), 0xE72u
#define PPUTLIMPL_UINT_TRAIT_0x18Cu HEX, 396u, 396, (1, 8, C), 0xE73u
#define PPUTLIMPL_UINT_TRAIT_0x18Bu HEX, 395u, 395, (1, 8, B), 0xE74u
#define PPUTLIMPL_UINT_TRAIT_0x18Au HEX, 394u, 394, (1, 8, A), 0xE75u
#define PPUTLIMPL_UINT_TRAIT_0x189u HEX, 393u, 393, (1, 8, 9), 0xE76u
#define PPUTLIMPL_UINT_TRAIT_0x188u HEX, 392u, 392, (1, 8, 8), 0xE77u
#define PPUTLIMPL_UINT_TRAIT_0x187u HEX, 391u, 391, (1, 8, 7), 0xE78u
#define PPUTLIMPL_UINT_TRAIT_0x186u HEX, 390u, 390, (1, 8, 6), 0xE79u
#define PPUTLIMPL_UINT_TRAIT_0x185u HEX, 389u, 389, (1, 8, 5), 0xE7Au
#define PPUTLIMPL_UINT_TRAIT_0x184u HEX, 388u, 388, (1, 8, 4), 0xE7Bu
#define PPUTLIMPL_UINT_TRAIT_0x183u HEX, 387u, 387, (1, 8, 3), 0xE7Cu
#define PPUTLIMPL_UINT_TRAIT_0x182u HEX, 386u, 386, (1, 8, 2), 0xE7Du
#define PPUTLIMPL_UINT_TRAIT_0x181u HEX, 385u, 385, (1, 8, 1), 0xE7Eu
#define PPUTLIMPL_UINT_TRAIT_0x180u HEX, 384u, 384, (1, 8, 0), 0xE7Fu
#define PPUTLIMPL_UINT_TRAIT_0x17Fu HEX, 383u, 383, (1, 7, F), 0xE80u
#define PPUTLIMPL_UINT_TRAIT_0x17Eu HEX, 382u, 382, (1, 7, E), 0xE81u
#define PPUTLIMPL_UINT_TRAIT_0x17Du HEX, 381u, 381, (1, 7, D), 0xE82u
#define PPUTLIMPL_UINT_TRAIT_0x17Cu HEX, 380u, 380, (1, 7, C), 0xE83u
#define PPUTLIMPL_UINT_TRAIT_0x17Bu HEX, 379u, 379, (1, 7, B), 0xE84u
#define PPUTLIMPL_UINT_TRAIT_0x17Au HEX, 378u, 378, (1, 7, A), 0xE85u
#define PPUTLIMPL_UINT_TRAIT_0x179u HEX, 377u, 377, (1, 7, 9), 0xE86u
#define PPUTLIMPL_UINT_TRAIT_0x178u HEX, 376u, 376, (1, 7, 8), 0xE87u
#define PPUTLIMPL_UINT_TRAIT_0x177u HEX, 375u, 375, (1, 7, 7), 0xE88u
#define PPUTLIMPL_UINT_TRAIT_0x176u HEX, 374u, 374, (1, 7, 6), 0xE89u
#define PPUTLIMPL_UINT_TRAIT_0x175u HEX, 373u, 373, (1, 7, 5), 0xE8Au
#define PPUTLIMPL_UINT_TRAIT_0x174u HEX, 372u, 372, (1, 7, 4), 0xE8Bu
#define PPUTLIMPL_UINT_TRAIT_0x173u HEX, 371u, 371, (1, 7, 3), 0xE8Cu
#define PPUTLIMPL_UINT_TRAIT_0x172u HEX, 370u, 370, (1, 7, 2), 0xE8Du
#define PPUTLIMPL_UINT_TRAIT_0x171u HEX, 369u, 369, (1, 7, 1), 0xE8Eu
#define PPUTLIMPL_UINT_TRAIT_0x170u HEX, 368u, 368, (1, 7, 0), 0xE8Fu
#define PPUTLIMPL_UINT_TRAIT_0x16Fu HEX, 367u, 367, (1, 6, F), 0xE90u
#define PPUTLIMPL_UINT_TRAIT_0x16Eu HEX, 366u, 366, (1, 6, E), 0xE91u
#define PPUTLIMPL_UINT_TRAIT_0x16Du HEX, 365u, 365, (1, 6, D), 0xE92u
#define PPUTLIMPL_UINT_TRAIT_0x16Cu HEX, 364u, 364, (1, 6, C), 0xE93u
#define PPUTLIMPL_UINT_TRAIT_0x16Bu HEX, 363u, 363, (1, 6, B), 0xE94u
#define PPUTLIMPL_UINT_TRAIT_0x16Au HEX, 362u, 362, (1, 6, A), 0xE95u
#define PPUTLIMPL_UINT_TRAIT_0x169u HEX, 361u, 361, (1, 6, 9), 0xE96u
#define PPUTLIMPL_UINT_TRAIT_0x168u HEX, 360u, 360, (1, 6, 8), 0xE97u
#define PPUTLIMPL_UINT_TRAIT_0x167u HEX, 359u, 359, (1, 6, 7), 0xE98u
#define PPUTLIMPL_UINT_TRAIT_0x166u HEX, 358u, 358, (1, 6, 6), 0xE99u
#define PPUTLIMPL_UINT_TRAIT_0x165u HEX, 357u, 357, (1, 6, 5), 0xE9Au
#define PPUTLIMPL_UINT_TRAIT_0x164u HEX, 356u, 356, (1, 6, 4), 0xE9Bu
#define PPUTLIMPL_UINT_TRAIT_0x163u HEX, 355u, 355, (1, 6, 3), 0xE9Cu
#define PPUTLIMPL_UINT_TRAIT_0x162u HEX, 354u, 354, (1, 6, 2), 0xE9Du
#define PPUTLIMPL_UINT_TRAIT_0x161u HEX, 353u, 353, (1, 6, 1), 0xE9Eu
#define PPUTLIMPL_UINT_TRAIT_0x160u HEX, 352u, 352, (1, 6, 0), 0xE9Fu
#define PPUTLIMPL_UINT_TRAIT_0x15Fu HEX, 351u, 351, (1, 5, F), 0xEA0u
#define PPUTLIMPL_UINT_TRAIT_0x15Eu HEX, 350u, 350, (1, 5, E), 0xEA1u
#define PPUTLIMPL_UINT_TRAIT_0x15Du HEX, 349u, 349, (1, 5, D), 0xEA2u
#define PPUTLIMPL_UINT_TRAIT_0x15Cu HEX, 348u, 348, (1, 5, C), 0xEA3u
#define PPUTLIMPL_UINT_TRAIT_0x15Bu HEX, 347u, 347, (1, 5, B), 0xEA4u
#define PPUTLIMPL_UINT_TRAIT_0x15Au HEX, 346u, 346, (1, 5, A), 0xEA5u
#define PPUTLIMPL_UINT_TRAIT_0x159u HEX, 345u, 345, (1, 5, 9), 0xEA6u
#define PPUTLIMPL_UINT_TRAIT_0x158u HEX, 344u, 344, (1, 5, 8), 0xEA7u
#define PPUTLIMPL_UINT_TRAIT_0x157u HEX, 343u, 343, (1, 5, 7), 0xEA8u
#define PPUTLIMPL_UINT_TRAIT_0x156u HEX, 342u, 342, (1, 5, 6), 0xEA9u
#define PPUTLIMPL_UINT_TRAIT_0x155u HEX, 341u, 341, (1, 5, 5), 0xEAAu
#define PPUTLIMPL_UINT_TRAIT_0x154u HEX, 340u, 340, (1, 5, 4), 0xEABu
#define PPUTLIMPL_UINT_TRAIT_0x153u HEX, 339u, 339, (1, 5, 3), 0xEACu
#define PPUTLIMPL_UINT_TRAIT_0x152u HEX, 338u, 338, (1, 5, 2), 0xEADu
#define PPUTLIMPL_UINT_TRAIT_0x151u HEX, 337u, 337, (1, 5, 1), 0xEAEu
#define PPUTLIMPL_UINT_TRAIT_0x150u HEX, 336u, 336, (1, 5, 0), 0xEAFu
#define PPUTLIMPL_UINT_TRAIT_0x14Fu HEX, 335u, 335, (1, 4, F), 0xEB0u
#define PPUTLIMPL_UINT_TRAIT_0x14Eu HEX, 334u, 334, (1, 4, E), 0xEB1u
#define PPUTLIMPL_UINT_TRAIT_0x14Du HEX, 333u, 333, (1, 4, D), 0xEB2u
#define PPUTLIMPL_UINT_TRAIT_0x14Cu HEX, 332u, 332, (1, 4, C), 0xEB3u
#define PPUTLIMPL_UINT_TRAIT_0x14Bu HEX, 331u, 331, (1, 4, B), 0xEB4u
#define PPUTLIMPL_UINT_TRAIT_0x14Au HEX, 330u, 330, (1, 4, A), 0xEB5u
#define PPUTLIMPL_UINT_TRAIT_0x149u HEX, 329u, 329, (1, 4, 9), 0xEB6u
#define PPUTLIMPL_UINT_TRAIT_0x148u HEX, 328u, 328, (1, 4, 8), 0xEB7u
#define PPUTLIMPL_UINT_TRAIT_0x147u HEX, 327u, 327, (1, 4, 7), 0xEB8u
#define PPUTLIMPL_UINT_TRAIT_0x146u HEX, 326u, 326, (1, 4, 6), 0xEB9u
#define PPUTLIMPL_UINT_TRAIT_0x145u HEX, 325u, 325, (1, 4, 5), 0xEBAu
#define PPUTLIMPL_UINT_TRAIT_0x144u HEX, 324u, 324, (1, 4, 4), 0xEBBu
#define PPUTLIMPL_UINT_TRAIT_0x143u HEX, 323u, 323, (1, 4, 3), 0xEBCu
#define PPUTLIMPL_UINT_TRAIT_0x142u HEX, 322u, 322, (1, 4, 2), 0xEBDu
#define PPUTLIMPL_UINT_TRAIT_0x141u HEX, 321u, 321, (1, 4, 1), 0xEBEu
#define PPUTLIMPL_UINT_TRAIT_0x140u HEX, 320u, 320, (1, 4, 0), 0xEBFu
#define PPUTLIMPL_UINT_TRAIT_0x13Fu HEX, 319u, 319, (1, 3, F), 0xEC0u
#define PPUTLIMPL_UINT_TRAIT_0x13Eu HEX, 318u, 318, (1, 3, E), 0xEC1u
#define PPUTLIMPL_UINT_TRAIT_0x13Du HEX, 317u, 317, (1, 3, D), 0xEC2u
#define PPUTLIMPL_UINT_TRAIT_0x13Cu HEX, 316u, 316, (1, 3, C), 0xEC3u
#define PPUTLIMPL_UINT_TRAIT_0x13Bu HEX, 315u, 315, (1, 3, B), 0xEC4u
#define PPUTLIMPL_UINT_TRAIT_0x13Au HEX, 314u, 314, (1, 3, A), 0xEC5u
#define PPUTLIMPL_UINT_TRAIT_0x139u HEX, 313u, 313, (1, 3, 9), 0xEC6u
#define PPUTLIMPL_UINT_TRAIT_0x138u HEX, 312u, 312, (1, 3, 8), 0xEC7u
#define PPUTLIMPL_UINT_TRAIT_0x137u HEX, 311u, 311, (1, 3, 7), 0xEC8u
#define PPUTLIMPL_UINT_TRAIT_0x136u HEX, 310u, 310, (1, 3, 6), 0xEC9u
#define PPUTLIMPL_UINT_TRAIT_0x135u HEX, 309u, 309, (1, 3, 5), 0xECAu
#define PPUTLIMPL_UINT_TRAIT_0x134u HEX, 308u, 308, (1, 3, 4), 0xECBu
#define PPUTLIMPL_UINT_TRAIT_0x133u HEX, 307u, 307, (1, 3, 3), 0xECCu
#define PPUTLIMPL_UINT_TRAIT_0x132u HEX, 306u, 306, (1, 3, 2), 0xECDu
#define PPUTLIMPL_UINT_TRAIT_0x131u HEX, 305u, 305, (1, 3, 1), 0xECEu
#define PPUTLIMPL_UINT_TRAIT_0x130u HEX, 304u, 304, (1, 3, 0), 0xECFu
#define PPUTLIMPL_UINT_TRAIT_0x12Fu HEX, 303u, 303, (1, 2, F), 0xED0u
#define PPUTLIMPL_UINT_TRAIT_0x12Eu HEX, 302u, 302, (1, 2, E), 0xED1u
#define PPUTLIMPL_UINT_TRAIT_0x12Du HEX, 301u, 301, (1, 2, D), 0xED2u
#define PPUTLIMPL_UINT_TRAIT_0x12Cu HEX, 300u, 300, (1, 2, C), 0xED3u
#define PPUTLIMPL_UINT_TRAIT_0x12Bu HEX, 299u, 299, (1, 2, B), 0xED4u
#define PPUTLIMPL_UINT_TRAIT_0x12Au HEX, 298u, 298, (1, 2, A), 0xED5u
#define PPUTLIMPL_UINT_TRAIT_0x129u HEX, 297u, 297, (1, 2, 9), 0xED6u
#define PPUTLIMPL_UINT_TRAIT_0x128u HEX, 296u, 296, (1, 2, 8), 0xED7u
#define PPUTLIMPL_UINT_TRAIT_0x127u HEX, 295u, 295, (1, 2, 7), 0xED8u
#define PPUTLIMPL_UINT_TRAIT_0x126u HEX, 294u, 294, (1, 2, 6), 0xED9u
#define PPUTLIMPL_UINT_TRAIT_0x125u HEX, 293u, 293, (1, 2, 5), 0xEDAu
#define PPUTLIMPL_UINT_TRAIT_0x124u HEX, 292u, 292, (1, 2, 4), 0xEDBu
#define PPUTLIMPL_UINT_TRAIT_0x123u HEX, 291u, 291, (1, 2, 3), 0xEDCu
#define PPUTLIMPL_UINT_TRAIT_0x122u HEX, 290u, 290, (1, 2, 2), 0xEDDu
#define PPUTLIMPL_UINT_TRAIT_0x121u HEX, 289u, 289, (1, 2, 1), 0xEDEu
#define PPUTLIMPL_UINT_TRAIT_0x120u HEX, 288u, 288, (1, 2, 0), 0xEDFu
#define PPUTLIMPL_UINT_TRAIT_0x11Fu HEX, 287u, 287, (1, 1, F), 0xEE0u
#define PPUTLIMPL_UINT_TRAIT_0x11Eu HEX, 286u, 286, (1, 1, E), 0xEE1u
#define PPUTLIMPL_UINT_TRAIT_0x11Du HEX, 285u, 285, (1, 1, D), 0xEE2u
#define PPUTLIMPL_UINT_TRAIT_0x11Cu HEX, 284u, 284, (1, 1, C), 0xEE3u
#define PPUTLIMPL_UINT_TRAIT_0x11Bu HEX, 283u, 283, (1, 1, B), 0xEE4u
#define PPUTLIMPL_UINT_TRAIT_0x11Au HEX, 282u, 282, (1, 1, A), 0xEE5u
#define PPUTLIMPL_UINT_TRAIT_0x119u HEX, 281u, 281, (1, 1, 9), 0xEE6u
#define PPUTLIMPL_UINT_TRAIT_0x118u HEX, 280u, 280, (1, 1, 8), 0xEE7u
#define PPUTLIMPL_UINT_TRAIT_0x117u HEX, 279u, 279, (1, 1, 7), 0xEE8u
#define PPUTLIMPL_UINT_TRAIT_0x116u HEX, 278u, 278, (1, 1, 6), 0xEE9u
#define PPUTLIMPL_UINT_TRAIT_0x115u HEX, 277u, 277, (1, 1, 5), 0xEEAu
#define PPUTLIMPL_UINT_TRAIT_0x114u HEX, 276u, 276, (1, 1, 4), 0xEEBu
#define PPUTLIMPL_UINT_TRAIT_0x113u HEX, 275u, 275, (1, 1, 3), 0xEECu
#define PPUTLIMPL_UINT_TRAIT_0x112u HEX, 274u, 274, (1, 1, 2), 0xEEDu
#define PPUTLIMPL_UINT_TRAIT_0x111u HEX, 273u, 273, (1, 1, 1), 0xEEEu
#define PPUTLIMPL_UINT_TRAIT_0x110u HEX, 272u, 272, (1, 1, 0), 0xEEFu
#define PPUTLIMPL_UINT_TRAIT_0x10Fu HEX, 271u, 271, (1, 0, F), 0xEF0u
#define PPUTLIMPL_UINT_TRAIT_0x10Eu HEX, 270u, 270, (1, 0, E), 0xEF1u
#define PPUTLIMPL_UINT_TRAIT_0x10Du HEX, 269u, 269, (1, 0, D), 0xEF2u
#define PPUTLIMPL_UINT_TRAIT_0x10Cu HEX, 268u, 268, (1, 0, C), 0xEF3u
#define PPUTLIMPL_UINT_TRAIT_0x10Bu HEX, 267u, 267, (1, 0, B), 0xEF4u
#define PPUTLIMPL_UINT_TRAIT_0x10Au HEX, 266u, 266, (1, 0, A), 0xEF5u
#define PPUTLIMPL_UINT_TRAIT_0x109u HEX, 265u, 265, (1, 0, 9), 0xEF6u
#define PPUTLIMPL_UINT_TRAIT_0x108u HEX, 264u, 264, (1, 0, 8), 0xEF7u
#define PPUTLIMPL_UINT_TRAIT_0x107u HEX, 263u, 263, (1, 0, 7), 0xEF8u
#define PPUTLIMPL_UINT_TRAIT_0x106u HEX, 262u, 262, (1, 0, 6), 0xEF9u
#define PPUTLIMPL_UINT_TRAIT_0x105u HEX, 261u, 261, (1, 0, 5), 0xEFAu
#define PPUTLIMPL_UINT_TRAIT_0x104u HEX, 260u, 260, (1, 0, 4), 0xEFBu
#define PPUTLIMPL_UINT_TRAIT_0x103u HEX, 259u, 259, (1, 0, 3), 0xEFCu
#define PPUTLIMPL_UINT_TRAIT_0x102u HEX, 258u, 258, (1, 0, 2), 0xEFDu
#define PPUTLIMPL_UINT_TRAIT_0x101u HEX, 257u, 257, (1, 0, 1), 0xEFEu
#define PPUTLIMPL_UINT_TRAIT_0x100u HEX, 256u, 256, (1, 0, 0), 0xEFFu
#define PPUTLIMPL_UINT_TRAIT_0x0FFu HEX, 255u, 255, (0, F, F), 0xF00u
#define PPUTLIMPL_UINT_TRAIT_0x0FEu HEX, 254u, 254, (0, F, E), 0xF01u
#define PPUTLIMPL_UINT_TRAIT_0x0FDu HEX, 253u, 253, (0, F, D), 0xF02u
#define PPUTLIMPL_UINT_TRAIT_0x0FCu HEX, 252u, 252, (0, F, C), 0xF03u
#define PPUTLIMPL_UINT_TRAIT_0x0FBu HEX, 251u, 251, (0, F, B), 0xF04u
#define PPUTLIMPL_UINT_TRAIT_0x0FAu HEX, 250u, 250, (0, F, A), 0xF05u
#define PPUTLIMPL_UINT_TRAIT_0x0F9u HEX, 249u, 249, (0, F, 9), 0xF06u
#define PPUTLIMPL_UINT_TRAIT_0x0F8u HEX, 248u, 248, (0, F, 8), 0xF07u
#define PPUTLIMPL_UINT_TRAIT_0x0F7u HEX, 247u, 247, (0, F, 7), 0xF08u
#define PPUTLIMPL_UINT_TRAIT_0x0F6u HEX, 246u, 246, (0, F, 6), 0xF09u
#define PPUTLIMPL_UINT_TRAIT_0x0F5u HEX, 245u, 245, (0, F, 5), 0xF0Au
#define PPUTLIMPL_UINT_TRAIT_0x0F4u HEX, 244u, 244, (0, F, 4), 0xF0Bu
#define PPUTLIMPL_UINT_TRAIT_0x0F3u HEX, 243u, 243, (0, F, 3), 0xF0Cu
#define PPUTLIMPL_UINT_TRAIT_0x0F2u HEX, 242u, 242, (0, F, 2), 0xF0Du
#define PPUTLIMPL_UINT_TRAIT_0x0F1u HEX, 241u, 241, (0, F, 1), 0xF0Eu
#define PPUTLIMPL_UINT_TRAIT_0x0F0u HEX, 240u, 240, (0, F, 0), 0xF0Fu
#define PPUTLIMPL_UINT_TRAIT_0x0EFu HEX, 239u, 239, (0, E, F), 0xF10u
#define PPUTLIMPL_UINT_TRAIT_0x0EEu HEX, 238u, 238, (0, E, E), 0xF11u
#define PPUTLIMPL_UINT_TRAIT_0x0EDu HEX, 237u, 237, (0, E, D), 0xF12u
#define PPUTLIMPL_UINT_TRAIT_0x0ECu HEX, 236u, 236, (0, E, C), 0xF13u
#define PPUTLIMPL_UINT_TRAIT_0x0EBu HEX, 235u, 235, (0, E, B), 0xF14u
#define PPUTLIMPL_UINT_TRAIT_0x0EAu HEX, 234u, 234, (0, E, A), 0xF15u
#define PPUTLIMPL_UINT_TRAIT_0x0E9u HEX, 233u, 233, (0, E, 9), 0xF16u
#define PPUTLIMPL_UINT_TRAIT_0x0E8u HEX, 232u, 232, (0, E, 8), 0xF17u
#define PPUTLIMPL_UINT_TRAIT_0x0E7u HEX, 231u, 231, (0, E, 7), 0xF18u
#define PPUTLIMPL_UINT_TRAIT_0x0E6u HEX, 230u, 230, (0, E, 6), 0xF19u
#define PPUTLIMPL_UINT_TRAIT_0x0E5u HEX, 229u, 229, (0, E, 5), 0xF1Au
#define PPUTLIMPL_UINT_TRAIT_0x0E4u HEX, 228u, 228, (0, E, 4), 0xF1Bu
#define PPUTLIMPL_UINT_TRAIT_0x0E3u HEX, 227u, 227, (0, E, 3), 0xF1Cu
#define PPUTLIMPL_UINT_TRAIT_0x0E2u HEX, 226u, 226, (0, E, 2), 0xF1Du
#define PPUTLIMPL_UINT_TRAIT_0x0E1u HEX, 225u, 225, (0, E, 1), 0xF1Eu
#define PPUTLIMPL_UINT_TRAIT_0x0E0u HEX, 224u, 224, (0, E, 0), 0xF1Fu
#define PPUTLIMPL_UINT_TRAIT_0x0DFu HEX, 223u, 223, (0, D, F), 0xF20u
#define PPUTLIMPL_UINT_TRAIT_0x0DEu HEX, 222u, 222, (0, D, E), 0xF21u
#define PPUTLIMPL_UINT_TRAIT_0x0DDu HEX, 221u, 221, (0, D, D), 0xF22u
#define PPUTLIMPL_UINT_TRAIT_0x0DCu HEX, 220u, 220, (0, D, C), 0xF23u
#define PPUTLIMPL_UINT_TRAIT_0x0DBu HEX, 219u, 219, (0, D, B), 0xF24u
#define PPUTLIMPL_UINT_TRAIT_0x0DAu HEX, 218u, 218, (0, D, A), 0xF25u
#define PPUTLIMPL_UINT_TRAIT_0x0D9u HEX, 217u, 217, (0, D, 9), 0xF26u
#define PPUTLIMPL_UINT_TRAIT_0x0D8u HEX, 216u, 216, (0, D, 8), 0xF27u
#define PPUTLIMPL_UINT_TRAIT_0x0D7u HEX, 215u, 215, (0, D, 7), 0xF28u
#define PPUTLIMPL_UINT_TRAIT_0x0D6u HEX, 214u, 214, (0, D, 6), 0xF29u
#define PPUTLIMPL_UINT_TRAIT_0x0D5u HEX, 213u, 213, (0, D, 5), 0xF2Au
#define PPUTLIMPL_UINT_TRAIT_0x0D4u HEX, 212u, 212, (0, D, 4), 0xF2Bu
#define PPUTLIMPL_UINT_TRAIT_0x0D3u HEX, 211u, 211, (0, D, 3), 0xF2Cu
#define PPUTLIMPL_UINT_TRAIT_0x0D2u HEX, 210u, 210, (0, D, 2), 0xF2Du
#define PPUTLIMPL_UINT_TRAIT_0x0D1u HEX, 209u, 209, (0, D, 1), 0xF2Eu
#define PPUTLIMPL_UINT_TRAIT_0x0D0u HEX, 208u, 208, (0, D, 0), 0xF2Fu
#define PPUTLIMPL_UINT_TRAIT_0x0CFu HEX, 207u, 207, (0, C, F), 0xF30u
#define PPUTLIMPL_UINT_TRAIT_0x0CEu HEX, 206u, 206, (0, C, E), 0xF31u
#define PPUTLIMPL_UINT_TRAIT_0x0CDu HEX, 205u, 205, (0, C, D), 0xF32u
#define PPUTLIMPL_UINT_TRAIT_0x0CCu HEX, 204u, 204, (0, C, C), 0xF33u
#define PPUTLIMPL_UINT_TRAIT_0x0CBu HEX, 203u, 203, (0, C, B), 0xF34u
#define PPUTLIMPL_UINT_TRAIT_0x0CAu HEX, 202u, 202, (0, C, A), 0xF35u
#define PPUTLIMPL_UINT_TRAIT_0x0C9u HEX, 201u, 201, (0, C, 9), 0xF36u
#define PPUTLIMPL_UINT_TRAIT_0x0C8u HEX, 200u, 200, (0, C, 8), 0xF37u
#define PPUTLIMPL_UINT_TRAIT_0x0C7u HEX, 199u, 199, (0, C, 7), 0xF38u
#define PPUTLIMPL_UINT_TRAIT_0x0C6u HEX, 198u, 198, (0, C, 6), 0xF39u
#define PPUTLIMPL_UINT_TRAIT_0x0C5u HEX, 197u, 197, (0, C, 5), 0xF3Au
#define PPUTLIMPL_UINT_TRAIT_0x0C4u HEX, 196u, 196, (0, C, 4), 0xF3Bu
#define PPUTLIMPL_UINT_TRAIT_0x0C3u HEX, 195u, 195, (0, C, 3), 0xF3Cu
#define PPUTLIMPL_UINT_TRAIT_0x0C2u HEX, 194u, 194, (0, C, 2), 0xF3Du
#define PPUTLIMPL_UINT_TRAIT_0x0C1u HEX, 193u, 193, (0, C, 1), 0xF3Eu
#define PPUTLIMPL_UINT_TRAIT_0x0C0u HEX, 192u, 192, (0, C, 0), 0xF3Fu
#define PPUTLIMPL_UINT_TRAIT_0x0BFu HEX, 191u, 191, (0, B, F), 0xF40u
#define PPUTLIMPL_UINT_TRAIT_0x0BEu HEX, 190u, 190, (0, B, E), 0xF41u
#define PPUTLIMPL_UINT_TRAIT_0x0BDu HEX, 189u, 189, (0, B, D), 0xF42u
#define PPUTLIMPL_UINT_TRAIT_0x0BCu HEX, 188u, 188, (0, B, C), 0xF43u
#define PPUTLIMPL_UINT_TRAIT_0x0BBu HEX, 187u, 187, (0, B, B), 0xF44u
#define PPUTLIMPL_UINT_TRAIT_0x0BAu HEX, 186u, 186, (0, B, A), 0xF45u
#define PPUTLIMPL_UINT_TRAIT_0x0B9u HEX, 185u, 185, (0, B, 9), 0xF46u
#define PPUTLIMPL_UINT_TRAIT_0x0B8u HEX, 184u, 184, (0, B, 8), 0xF47u
#define PPUTLIMPL_UINT_TRAIT_0x0B7u HEX, 183u, 183, (0, B, 7), 0xF48u
#define PPUTLIMPL_UINT_TRAIT_0x0B6u HEX, 182u, 182, (0, B, 6), 0xF49u
#define PPUTLIMPL_UINT_TRAIT_0x0B5u HEX, 181u, 181, (0, B, 5), 0xF4Au
#define PPUTLIMPL_UINT_TRAIT_0x0B4u HEX, 180u, 180, (0, B, 4), 0xF4Bu
#define PPUTLIMPL_UINT_TRAIT_0x0B3u HEX, 179u, 179, (0, B, 3), 0xF4Cu
#define PPUTLIMPL_UINT_TRAIT_0x0B2u HEX, 178u, 178, (0, B, 2), 0xF4Du
#define PPUTLIMPL_UINT_TRAIT_0x0B1u HEX, 177u, 177, (0, B, 1), 0xF4Eu
#define PPUTLIMPL_UINT_TRAIT_0x0B0u HEX, 176u, 176, (0, B, 0), 0xF4Fu
#define PPUTLIMPL_UINT_TRAIT_0x0AFu HEX, 175u, 175, (0, A, F), 0xF50u
#define PPUTLIMPL_UINT_TRAIT_0x0AEu HEX, 174u, 174, (0, A, E), 0xF51u
#define PPUTLIMPL_UINT_TRAIT_0x0ADu HEX, 173u, 173, (0, A, D), 0xF52u
#define PPUTLIMPL_UINT_TRAIT_0x0ACu HEX, 172u, 172, (0, A, C), 0xF53u
#define PPUTLIMPL_UINT_TRAIT_0x0ABu HEX, 171u, 171, (0, A, B), 0xF54u
#define PPUTLIMPL_UINT_TRAIT_0x0AAu HEX, 170u, 170, (0, A, A), 0xF55u
#define PPUTLIMPL_UINT_TRAIT_0x0A9u HEX, 169u, 169, (0, A, 9), 0xF56u
#define PPUTLIMPL_UINT_TRAIT_0x0A8u HEX, 168u, 168, (0, A, 8), 0xF57u
#define PPUTLIMPL_UINT_TRAIT_0x0A7u HEX, 167u, 167, (0, A, 7), 0xF58u
#define PPUTLIMPL_UINT_TRAIT_0x0A6u HEX, 166u, 166, (0, A, 6), 0xF59u
#define PPUTLIMPL_UINT_TRAIT_0x0A5u HEX, 165u, 165, (0, A, 5), 0xF5Au
#define PPUTLIMPL_UINT_TRAIT_0x0A4u HEX, 164u, 164, (0, A, 4), 0xF5Bu
#define PPUTLIMPL_UINT_TRAIT_0x0A3u HEX, 163u, 163, (0, A, 3), 0xF5Cu
#define PPUTLIMPL_UINT_TRAIT_0x0A2u HEX, 162u, 162, (0, A, 2), 0xF5Du
#define PPUTLIMPL_UINT_TRAIT_0x0A1u HEX, 161u, 161, (0, A, 1), 0xF5Eu
#define PPUTLIMPL_UINT_TRAIT_0x0A0u HEX, 160u, 160, (0, A, 0), 0xF5Fu
#define PPUTLIMPL_UINT_TRAIT_0x09Fu HEX, 159u, 159, (0, 9, F), 0xF60u
#define PPUTLIMPL_UINT_TRAIT_0x09Eu HEX, 158u, 158, (0, 9, E), 0xF61u
#define PPUTLIMPL_UINT_TRAIT_0x09Du HEX, 157u, 157, (0, 9, D), 0xF62u
#define PPUTLIMPL_UINT_TRAIT_0x09Cu HEX, 156u, 156, (0, 9, C), 0xF63u
#define PPUTLIMPL_UINT_TRAIT_0x09Bu HEX, 155u, 155, (0, 9, B), 0xF64u
#define PPUTLIMPL_UINT_TRAIT_0x09Au HEX, 154u, 154, (0, 9, A), 0xF65u
#define PPUTLIMPL_UINT_TRAIT_0x099u HEX, 153u, 153, (0, 9, 9), 0xF66u
#define PPUTLIMPL_UINT_TRAIT_0x098u HEX, 152u, 152, (0, 9, 8), 0xF67u
#define PPUTLIMPL_UINT_TRAIT_0x097u HEX, 151u, 151, (0, 9, 7), 0xF68u
#define PPUTLIMPL_UINT_TRAIT_0x096u HEX, 150u, 150, (0, 9, 6), 0xF69u
#define PPUTLIMPL_UINT_TRAIT_0x095u HEX, 149u, 149, (0, 9, 5), 0xF6Au
#define PPUTLIMPL_UINT_TRAIT_0x094u HEX, 148u, 148, (0, 9, 4), 0xF6Bu
#define PPUTLIMPL_UINT_TRAIT_0x093u HEX, 147u, 147, (0, 9, 3), 0xF6Cu
#define PPUTLIMPL_UINT_TRAIT_0x092u HEX, 146u, 146, (0, 9, 2), 0xF6Du
#define PPUTLIMPL_UINT_TRAIT_0x091u HEX, 145u, 145, (0, 9, 1), 0xF6Eu
#define PPUTLIMPL_UINT_TRAIT_0x090u HEX, 144u, 144, (0, 9, 0), 0xF6Fu
#define PPUTLIMPL_UINT_TRAIT_0x08Fu HEX, 143u, 143, (0, 8, F), 0xF70u
#define PPUTLIMPL_UINT_TRAIT_0x08Eu HEX, 142u, 142, (0, 8, E), 0xF71u
#define PPUTLIMPL_UINT_TRAIT_0x08Du HEX, 141u, 141, (0, 8, D), 0xF72u
#define PPUTLIMPL_UINT_TRAIT_0x08Cu HEX, 140u, 140, (0, 8, C), 0xF73u
#define PPUTLIMPL_UINT_TRAIT_0x08Bu HEX, 139u, 139, (0, 8, B), 0xF74u
#define PPUTLIMPL_UINT_TRAIT_0x08Au HEX, 138u, 138, (0, 8, A), 0xF75u
#define PPUTLIMPL_UINT_TRAIT_0x089u HEX, 137u, 137, (0, 8, 9), 0xF76u
#define PPUTLIMPL_UINT_TRAIT_0x088u HEX, 136u, 136, (0, 8, 8), 0xF77u
#define PPUTLIMPL_UINT_TRAIT_0x087u HEX, 135u, 135, (0, 8, 7), 0xF78u
#define PPUTLIMPL_UINT_TRAIT_0x086u HEX, 134u, 134, (0, 8, 6), 0xF79u
#define PPUTLIMPL_UINT_TRAIT_0x085u HEX, 133u, 133, (0, 8, 5), 0xF7Au
#define PPUTLIMPL_UINT_TRAIT_0x084u HEX, 132u, 132, (0, 8, 4), 0xF7Bu
#define PPUTLIMPL_UINT_TRAIT_0x083u HEX, 131u, 131, (0, 8, 3), 0xF7Cu
#define PPUTLIMPL_UINT_TRAIT_0x082u HEX, 130u, 130, (0, 8, 2), 0xF7Du
#define PPUTLIMPL_UINT_TRAIT_0x081u HEX, 129u, 129, (0, 8, 1), 0xF7Eu
#define PPUTLIMPL_UINT_TRAIT_0x080u HEX, 128u, 128, (0, 8, 0), 0xF7Fu
#define PPUTLIMPL_UINT_TRAIT_0x07Fu HEX, 127u, 127, (0, 7, F), 0xF80u
#define PPUTLIMPL_UINT_TRAIT_0x07Eu HEX, 126u, 126, (0, 7, E), 0xF81u
#define PPUTLIMPL_UINT_TRAIT_0x07Du HEX, 125u, 125, (0, 7, D), 0xF82u
#define PPUTLIMPL_UINT_TRAIT_0x07Cu HEX, 124u, 124, (0, 7, C), 0xF83u
#define PPUTLIMPL_UINT_TRAIT_0x07Bu HEX, 123u, 123, (0, 7, B), 0xF84u
#define PPUTLIMPL_UINT_TRAIT_0x07Au HEX, 122u, 122, (0, 7, A), 0xF85u
#define PPUTLIMPL_UINT_TRAIT_0x079u HEX, 121u, 121, (0, 7, 9), 0xF86u
#define PPUTLIMPL_UINT_TRAIT_0x078u HEX, 120u, 120, (0, 7, 8), 0xF87u
#define PPUTLIMPL_UINT_TRAIT_0x077u HEX, 119u, 119, (0, 7, 7), 0xF88u
#define PPUTLIMPL_UINT_TRAIT_0x076u HEX, 118u, 118, (0, 7, 6), 0xF89u
#define PPUTLIMPL_UINT_TRAIT_0x075u HEX, 117u, 117, (0, 7, 5), 0xF8Au
#define PPUTLIMPL_UINT_TRAIT_0x074u HEX, 116u, 116, (0, 7, 4), 0xF8Bu
#define PPUTLIMPL_UINT_TRAIT_0x073u HEX, 115u, 115, (0, 7, 3), 0xF8Cu
#define PPUTLIMPL_UINT_TRAIT_0x072u HEX, 114u, 114, (0, 7, 2), 0xF8Du
#define PPUTLIMPL_UINT_TRAIT_0x071u HEX, 113u, 113, (0, 7, 1), 0xF8Eu
#define PPUTLIMPL_UINT_TRAIT_0x070u HEX, 112u, 112, (0, 7, 0), 0xF8Fu
#define PPUTLIMPL_UINT_TRAIT_0x06Fu HEX, 111u, 111, (0, 6, F), 0xF90u
#define PPUTLIMPL_UINT_TRAIT_0x06Eu HEX, 110u, 110, (0, 6, E), 0xF91u
#define PPUTLIMPL_UINT_TRAIT_0x06Du HEX, 109u, 109, (0, 6, D), 0xF92u
#define PPUTLIMPL_UINT_TRAIT_0x06Cu HEX, 108u, 108, (0, 6, C), 0xF93u
#define PPUTLIMPL_UINT_TRAIT_0x06Bu HEX, 107u, 107, (0, 6, B), 0xF94u
#define PPUTLIMPL_UINT_TRAIT_0x06Au HEX, 106u, 106, (0, 6, A), 0xF95u
#define PPUTLIMPL_UINT_TRAIT_0x069u HEX, 105u, 105, (0, 6, 9), 0xF96u
#define PPUTLIMPL_UINT_TRAIT_0x068u HEX, 104u, 104, (0, 6, 8), 0xF97u
#define PPUTLIMPL_UINT_TRAIT_0x067u HEX, 103u, 103, (0, 6, 7), 0xF98u
#define PPUTLIMPL_UINT_TRAIT_0x066u HEX, 102u, 102, (0, 6, 6), 0xF99u
#define PPUTLIMPL_UINT_TRAIT_0x065u HEX, 101u, 101, (0, 6, 5), 0xF9Au
#define PPUTLIMPL_UINT_TRAIT_0x064u HEX, 100u, 100, (0, 6, 4), 0xF9Bu
#define PPUTLIMPL_UINT_TRAIT_0x063u HEX, 99u, 99, (0, 6, 3), 0xF9Cu
#define PPUTLIMPL_UINT_TRAIT_0x062u HEX, 98u, 98, (0, 6, 2), 0xF9Du
#define PPUTLIMPL_UINT_TRAIT_0x061u HEX, 97u, 97, (0, 6, 1), 0xF9Eu
#define PPUTLIMPL_UINT_TRAIT_0x060u HEX, 96u, 96, (0, 6, 0), 0xF9Fu
#define PPUTLIMPL_UINT_TRAIT_0x05Fu HEX, 95u, 95, (0, 5, F), 0xFA0u
#define PPUTLIMPL_UINT_TRAIT_0x05Eu HEX, 94u, 94, (0, 5, E), 0xFA1u
#define PPUTLIMPL_UINT_TRAIT_0x05Du HEX, 93u, 93, (0, 5, D), 0xFA2u
#define PPUTLIMPL_UINT_TRAIT_0x05Cu HEX, 92u, 92, (0, 5, C), 0xFA3u
#define PPUTLIMPL_UINT_TRAIT_0x05Bu HEX, 91u, 91, (0, 5, B), 0xFA4u
#define PPUTLIMPL_UINT_TRAIT_0x05Au HEX, 90u, 90, (0, 5, A), 0xFA5u
#define PPUTLIMPL_UINT_TRAIT_0x059u HEX, 89u, 89, (0, 5, 9), 0xFA6u
#define PPUTLIMPL_UINT_TRAIT_0x058u HEX, 88u, 88, (0, 5, 8), 0xFA7u
#define PPUTLIMPL_UINT_TRAIT_0x057u HEX, 87u, 87, (0, 5, 7), 0xFA8u
#define PPUTLIMPL_UINT_TRAIT_0x056u HEX, 86u, 86, (0, 5, 6), 0xFA9u
#define PPUTLIMPL_UINT_TRAIT_0x055u HEX, 85u, 85, (0, 5, 5), 0xFAAu
#define PPUTLIMPL_UINT_TRAIT_0x054u HEX, 84u, 84, (0, 5, 4), 0xFABu
#define PPUTLIMPL_UINT_TRAIT_0x053u HEX, 83u, 83, (0, 5, 3), 0xFACu
#define PPUTLIMPL_UINT_TRAIT_0x052u HEX, 82u, 82, (0, 5, 2), 0xFADu
#define PPUTLIMPL_UINT_TRAIT_0x051u HEX, 81u, 81, (0, 5, 1), 0xFAEu
#define PPUTLIMPL_UINT_TRAIT_0x050u HEX, 80u, 80, (0, 5, 0), 0xFAFu
#define PPUTLIMPL_UINT_TRAIT_0x04Fu HEX, 79u, 79, (0, 4, F), 0xFB0u
#define PPUTLIMPL_UINT_TRAIT_0x04Eu HEX, 78u, 78, (0, 4, E), 0xFB1u
#define PPUTLIMPL_UINT_TRAIT_0x04Du HEX, 77u, 77, (0, 4, D), 0xFB2u
#define PPUTLIMPL_UINT_TRAIT_0x04Cu HEX, 76u, 76, (0, 4, C), 0xFB3u
#define PPUTLIMPL_UINT_TRAIT_0x04Bu HEX, 75u, 75, (0, 4, B), 0xFB4u
#define PPUTLIMPL_UINT_TRAIT_0x04Au HEX, 74u, 74, (0, 4, A), 0xFB5u
#define PPUTLIMPL_UINT_TRAIT_0x049u HEX, 73u, 73, (0, 4, 9), 0xFB6u
#define PPUTLIMPL_UINT_TRAIT_0x048u HEX, 72u, 72, (0, 4, 8), 0xFB7u
#define PPUTLIMPL_UINT_TRAIT_0x047u HEX, 71u, 71, (0, 4, 7), 0xFB8u
#define PPUTLIMPL_UINT_TRAIT_0x046u HEX, 70u, 70, (0, 4, 6), 0xFB9u
#define PPUTLIMPL_UINT_TRAIT_0x045u HEX, 69u, 69, (0, 4, 5), 0xFBAu
#define PPUTLIMPL_UINT_TRAIT_0x044u HEX, 68u, 68, (0, 4, 4), 0xFBBu
#define PPUTLIMPL_UINT_TRAIT_0x043u HEX, 67u, 67, (0, 4, 3), 0xFBCu
#define PPUTLIMPL_UINT_TRAIT_0x042u HEX, 66u, 66, (0, 4, 2), 0xFBDu
#define PPUTLIMPL_UINT_TRAIT_0x041u HEX, 65u, 65, (0, 4, 1), 0xFBEu
#define PPUTLIMPL_UINT_TRAIT_0x040u HEX, 64u, 64, (0, 4, 0), 0xFBFu
#define PPUTLIMPL_UINT_TRAIT_0x03Fu HEX, 63u, 63, (0, 3, F), 0xFC0u
#define PPUTLIMPL_UINT_TRAIT_0x03Eu HEX, 62u, 62, (0, 3, E), 0xFC1u
#define PPUTLIMPL_UINT_TRAIT_0x03Du HEX, 61u, 61, (0, 3, D), 0xFC2u
#define PPUTLIMPL_UINT_TRAIT_0x03Cu HEX, 60u, 60, (0, 3, C), 0xFC3u
#define PPUTLIMPL_UINT_TRAIT_0x03Bu HEX, 59u, 59, (0, 3, B), 0xFC4u
#define PPUTLIMPL_UINT_TRAIT_0x03Au HEX, 58u, 58, (0, 3, A), 0xFC5u
#define PPUTLIMPL_UINT_TRAIT_0x039u HEX, 57u, 57, (0, 3, 9), 0xFC6u
#define PPUTLIMPL_UINT_TRAIT_0x038u HEX, 56u, 56, (0, 3, 8), 0xFC7u
#define PPUTLIMPL_UINT_TRAIT_0x037u HEX, 55u, 55, (0, 3, 7), 0xFC8u
#define PPUTLIMPL_UINT_TRAIT_0x036u HEX, 54u, 54, (0, 3, 6), 0xFC9u
#define PPUTLIMPL_UINT_TRAIT_0x035u HEX, 53u, 53, (0, 3, 5), 0xFCAu
#define PPUTLIMPL_UINT_TRAIT_0x034u HEX, 52u, 52, (0, 3, 4), 0xFCBu
#define PPUTLIMPL_UINT_TRAIT_0x033u HEX, 51u, 51, (0, 3, 3), 0xFCCu
#define PPUTLIMPL_UINT_TRAIT_0x032u HEX, 50u, 50, (0, 3, 2), 0xFCDu
#define PPUTLIMPL_UINT_TRAIT_0x031u HEX, 49u, 49, (0, 3, 1), 0xFCEu
#define PPUTLIMPL_UINT_TRAIT_0x030u HEX, 48u, 48, (0, 3, 0), 0xFCFu
#define PPUTLIMPL_UINT_TRAIT_0x02Fu HEX, 47u, 47, (0, 2, F), 0xFD0u
#define PPUTLIMPL_UINT_TRAIT_0x02Eu HEX, 46u, 46, (0, 2, E), 0xFD1u
#define PPUTLIMPL_UINT_TRAIT_0x02Du HEX, 45u, 45, (0, 2, D), 0xFD2u
#define PPUTLIMPL_UINT_TRAIT_0x02Cu HEX, 44u, 44, (0, 2, C), 0xFD3u
#define PPUTLIMPL_UINT_TRAIT_0x02Bu HEX, 43u, 43, (0, 2, B), 0xFD4u
#define PPUTLIMPL_UINT_TRAIT_0x02Au HEX, 42u, 42, (0, 2, A), 0xFD5u
#define PPUTLIMPL_UINT_TRAIT_0x029u HEX, 41u, 41, (0, 2, 9), 0xFD6u
#define PPUTLIMPL_UINT_TRAIT_0x028u HEX, 40u, 40, (0, 2, 8), 0xFD7u
#define PPUTLIMPL_UINT_TRAIT_0x027u HEX, 39u, 39, (0, 2, 7), 0xFD8u
#define PPUTLIMPL_UINT_TRAIT_0x026u HEX, 38u, 38, (0, 2, 6), 0xFD9u
#define PPUTLIMPL_UINT_TRAIT_0x025u HEX, 37u, 37, (0, 2, 5), 0xFDAu
#define PPUTLIMPL_UINT_TRAIT_0x024u HEX, 36u, 36, (0, 2, 4), 0xFDBu
#define PPUTLIMPL_UINT_TRAIT_0x023u HEX, 35u, 35, (0, 2, 3), 0xFDCu
#define PPUTLIMPL_UINT_TRAIT_0x022u HEX, 34u, 34, (0, 2, 2), 0xFDDu
#define PPUTLIMPL_UINT_TRAIT_0x021u HEX, 33u, 33, (0, 2, 1), 0xFDEu
#define PPUTLIMPL_UINT_TRAIT_0x020u HEX, 32u, 32, (0, 2, 0), 0xFDFu
#define PPUTLIMPL_UINT_TRAIT_0x01Fu HEX, 31u, 31, (0, 1, F), 0xFE0u
#define PPUTLIMPL_UINT_TRAIT_0x01Eu HEX, 30u, 30, (0, 1, E), 0xFE1u
#define PPUTLIMPL_UINT_TRAIT_0x01Du HEX, 29u, 29, (0, 1, D), 0xFE2u
#define PPUTLIMPL_UINT_TRAIT_0x01Cu HEX, 28u, 28, (0, 1, C), 0xFE3u
#define PPUTLIMPL_UINT_TRAIT_0x01Bu HEX, 27u, 27, (0, 1, B), 0xFE4u
#define PPUTLIMPL_UINT_TRAIT_0x01Au HEX, 26u, 26, (0, 1, A), 0xFE5u
#define PPUTLIMPL_UINT_TRAIT_0x019u HEX, 25u, 25, (0, 1, 9), 0xFE6u
#define PPUTLIMPL_UINT_TRAIT_0x018u HEX, 24u, 24, (0, 1, 8), 0xFE7u
#define PPUTLIMPL_UINT_TRAIT_0x017u HEX, 23u, 23, (0, 1, 7), 0xFE8u
#define PPUTLIMPL_UINT_TRAIT_0x016u HEX, 22u, 22, (0, 1, 6), 0xFE9u
#define PPUTLIMPL_UINT_TRAIT_0x015u HEX, 21u, 21, (0, 1, 5), 0xFEAu
#define PPUTLIMPL_UINT_TRAIT_0x014u HEX, 20u, 20, (0, 1, 4), 0xFEBu
#define PPUTLIMPL_UINT_TRAIT_0x013u HEX, 19u, 19, (0, 1, 3), 0xFECu
#define PPUTLIMPL_UINT_TRAIT_0x012u HEX, 18u, 18, (0, 1, 2), 0xFEDu
#define PPUTLIMPL_UINT_TRAIT_0x011u HEX, 17u, 17, (0, 1, 1), 0xFEEu
#define PPUTLIMPL_UINT_TRAIT_0x010u HEX, 16u, 16, (0, 1, 0), 0xFEFu
#define PPUTLIMPL_UINT_TRAIT_0x00Fu HEX, 15u, 15, (0, 0, F), 0xFF0u
#define PPUTLIMPL_UINT_TRAIT_0x00Eu HEX, 14u, 14, (0, 0, E), 0xFF1u
#define PPUTLIMPL_UINT_TRAIT_0x00Du HEX, 13u, 13, (0, 0, D), 0xFF2u
#define PPUTLIMPL_UINT_TRAIT_0x00Cu HEX, 12u, 12, (0, 0, C), 0xFF3u
#define PPUTLIMPL_UINT_TRAIT_0x00Bu HEX, 11u, 11, (0, 0, B), 0xFF4u
#define PPUTLIMPL_UINT_TRAIT_0x00Au HEX, 10u, 10, (0, 0, A), 0xFF5u
#define PPUTLIMPL_UINT_TRAIT_0x009u HEX, 9u, 9, (0, 0, 9), 0xFF6u
#define PPUTLIMPL_UINT_TRAIT_0x008u HEX, 8u, 8, (0, 0, 8), 0xFF7u
#define PPUTLIMPL_UINT_TRAIT_0x007u HEX, 7u, 7, (0, 0, 7), 0xFF8u
#define PPUTLIMPL_UINT_TRAIT_0x006u HEX, 6u, 6, (0, 0, 6), 0xFF9u
#define PPUTLIMPL_UINT_TRAIT_0x005u HEX, 5u, 5, (0, 0, 5), 0xFFAu
#define PPUTLIMPL_UINT_TRAIT_0x004u HEX, 4u, 4, (0, 0, 4), 0xFFBu
#define PPUTLIMPL_UINT_TRAIT_0x003u HEX, 3u, 3, (0, 0, 3), 0xFFCu
#define PPUTLIMPL_UINT_TRAIT_0x002u HEX, 2u, 2, (0, 0, 2), 0xFFDu
#define PPUTLIMPL_UINT_TRAIT_0x001u HEX, 1u, 1, (0, 0, 1), 0xFFEu
#define PPUTLIMPL_UINT_TRAIT_0x000u HEX, 0u, 0, (0, 0, 0), 0xFFFu

/// type, signed hex, signed is negative, log2, sqrt, factors
#define PPUTLIMPL_UINT_TRAIT_4095u DEC, 0xFFF, 1, 11u, 63u, (3u, 3u, 5u, 7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_4094u DEC, 0xFFE, 1, 11u, 63u, (2u, 23u, 89u)
#define PPUTLIMPL_UINT_TRAIT_4093u DEC, 0xFFD, 1, 11u, 63u, ()
#define PPUTLIMPL_UINT_TRAIT_4092u DEC, 0xFFC, 1, 11u, 63u, (2u, 2u, 3u, 11u, 31u)
#define PPUTLIMPL_UINT_TRAIT_4091u DEC, 0xFFB, 1, 11u, 63u, ()
#define PPUTLIMPL_UINT_TRAIT_4090u DEC, 0xFFA, 1, 11u, 63u, (2u, 5u, 409u)
#define PPUTLIMPL_UINT_TRAIT_4089u DEC, 0xFF9, 1, 11u, 63u, (3u, 29u, 47u)
#define PPUTLIMPL_UINT_TRAIT_4088u DEC, 0xFF8, 1, 11u, 63u, (2u, 2u, 2u, 7u, 73u)
#define PPUTLIMPL_UINT_TRAIT_4087u DEC, 0xFF7, 1, 11u, 63u, (61u, 67u)
#define PPUTLIMPL_UINT_TRAIT_4086u DEC, 0xFF6, 1, 11u, 63u, (2u, 3u, 3u, 227u)
#define PPUTLIMPL_UINT_TRAIT_4085u DEC, 0xFF5, 1, 11u, 63u, (5u, 19u, 43u)
#define PPUTLIMPL_UINT_TRAIT_4084u DEC, 0xFF4, 1, 11u, 63u, (2u, 2u, 1021u)
#define PPUTLIMPL_UINT_TRAIT_4083u DEC, 0xFF3, 1, 11u, 63u, (3u, 1361u)
#define PPUTLIMPL_UINT_TRAIT_4082u DEC, 0xFF2, 1, 11u, 63u, (2u, 13u, 157u)
#define PPUTLIMPL_UINT_TRAIT_4081u DEC, 0xFF1, 1, 11u, 63u, (7u, 11u, 53u)
#define PPUTLIMPL_UINT_TRAIT_4080u DEC, 0xFF0, 1, 11u, 63u, (2u, 2u, 2u, 2u, 3u, 5u, 17u)
#define PPUTLIMPL_UINT_TRAIT_4079u DEC, 0xFEF, 1, 11u, 63u, ()
#define PPUTLIMPL_UINT_TRAIT_4078u DEC, 0xFEE, 1, 11u, 63u, (2u, 2039u)
#define PPUTLIMPL_UINT_TRAIT_4077u DEC, 0xFED, 1, 11u, 63u, (3u, 3u, 3u, 151u)
#define PPUTLIMPL_UINT_TRAIT_4076u DEC, 0xFEC, 1, 11u, 63u, (2u, 2u, 1019u)
#define PPUTLIMPL_UINT_TRAIT_4075u DEC, 0xFEB, 1, 11u, 63u, (5u, 5u, 163u)
#define PPUTLIMPL_UINT_TRAIT_4074u DEC, 0xFEA, 1, 11u, 63u, (2u, 3u, 7u, 97u)
#define PPUTLIMPL_UINT_TRAIT_4073u DEC, 0xFE9, 1, 11u, 63u, ()
#define PPUTLIMPL_UINT_TRAIT_4072u DEC, 0xFE8, 1, 11u, 63u, (2u, 2u, 2u, 509u)
#define PPUTLIMPL_UINT_TRAIT_4071u DEC, 0xFE7, 1, 11u, 63u, (3u, 23u, 59u)
#define PPUTLIMPL_UINT_TRAIT_4070u DEC, 0xFE6, 1, 11u, 63u, (2u, 5u, 11u, 37u)
#define PPUTLIMPL_UINT_TRAIT_4069u DEC, 0xFE5, 1, 11u, 63u, (13u, 313u)
#define PPUTLIMPL_UINT_TRAIT_4068u DEC, 0xFE4, 1, 11u, 63u, (2u, 2u, 3u, 3u, 113u)
#define PPUTLIMPL_UINT_TRAIT_4067u DEC, 0xFE3, 1, 11u, 63u, (7u, 7u, 83u)
#define PPUTLIMPL_UINT_TRAIT_4066u DEC, 0xFE2, 1, 11u, 63u, (2u, 19u, 107u)
#define PPUTLIMPL_UINT_TRAIT_4065u DEC, 0xFE1, 1, 11u, 63u, (3u, 5u, 271u)
#define PPUTLIMPL_UINT_TRAIT_4064u DEC, 0xFE0, 1, 11u, 63u, (2u, 2u, 2u, 2u, 2u, 127u)
#define PPUTLIMPL_UINT_TRAIT_4063u DEC, 0xFDF, 1, 11u, 63u, (17u, 239u)
#define PPUTLIMPL_UINT_TRAIT_4062u DEC, 0xFDE, 1, 11u, 63u, (2u, 3u, 677u)
#define PPUTLIMPL_UINT_TRAIT_4061u DEC, 0xFDD, 1, 11u, 63u, (31u, 131u)
#define PPUTLIMPL_UINT_TRAIT_4060u DEC, 0xFDC, 1, 11u, 63u, (2u, 2u, 5u, 7u, 29u)
#define PPUTLIMPL_UINT_TRAIT_4059u DEC, 0xFDB, 1, 11u, 63u, (3u, 3u, 11u, 41u)
#define PPUTLIMPL_UINT_TRAIT_4058u DEC, 0xFDA, 1, 11u, 63u, (2u, 2029u)
#define PPUTLIMPL_UINT_TRAIT_4057u DEC, 0xFD9, 1, 11u, 63u, ()
#define PPUTLIMPL_UINT_TRAIT_4056u DEC, 0xFD8, 1, 11u, 63u, (2u, 2u, 2u, 3u, 13u, 13u)
#define PPUTLIMPL_UINT_TRAIT_4055u DEC, 0xFD7, 1, 11u, 63u, (5u, 811u)
#define PPUTLIMPL_UINT_TRAIT_4054u DEC, 0xFD6, 1, 11u, 63u, (2u, 2027u)
#define PPUTLIMPL_UINT_TRAIT_4053u DEC, 0xFD5, 1, 11u, 63u, (3u, 7u, 193u)
#define PPUTLIMPL_UINT_TRAIT_4052u DEC, 0xFD4, 1, 11u, 63u, (2u, 2u, 1013u)
#define PPUTLIMPL_UINT_TRAIT_4051u DEC, 0xFD3, 1, 11u, 63u, ()
#define PPUTLIMPL_UINT_TRAIT_4050u DEC, 0xFD2, 1, 11u, 63u, (2u, 3u, 3u, 3u, 3u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_4049u DEC, 0xFD1, 1, 11u, 63u, ()
#define PPUTLIMPL_UINT_TRAIT_4048u DEC, 0xFD0, 1, 11u, 63u, (2u, 2u, 2u, 2u, 11u, 23u)
#define PPUTLIMPL_UINT_TRAIT_4047u DEC, 0xFCF, 1, 11u, 63u, (3u, 19u, 71u)
#define PPUTLIMPL_UINT_TRAIT_4046u DEC, 0xFCE, 1, 11u, 63u, (2u, 7u, 17u, 17u)
#define PPUTLIMPL_UINT_TRAIT_4045u DEC, 0xFCD, 1, 11u, 63u, (5u, 809u)
#define PPUTLIMPL_UINT_TRAIT_4044u DEC, 0xFCC, 1, 11u, 63u, (2u, 2u, 3u, 337u)
#define PPUTLIMPL_UINT_TRAIT_4043u DEC, 0xFCB, 1, 11u, 63u, (13u, 311u)
#define PPUTLIMPL_UINT_TRAIT_4042u DEC, 0xFCA, 1, 11u, 63u, (2u, 43u, 47u)
#define PPUTLIMPL_UINT_TRAIT_4041u DEC, 0xFC9, 1, 11u, 63u, (3u, 3u, 449u)
#define PPUTLIMPL_UINT_TRAIT_4040u DEC, 0xFC8, 1, 11u, 63u, (2u, 2u, 2u, 5u, 101u)
#define PPUTLIMPL_UINT_TRAIT_4039u DEC, 0xFC7, 1, 11u, 63u, (7u, 577u)
#define PPUTLIMPL_UINT_TRAIT_4038u DEC, 0xFC6, 1, 11u, 63u, (2u, 3u, 673u)
#define PPUTLIMPL_UINT_TRAIT_4037u DEC, 0xFC5, 1, 11u, 63u, (11u, 367u)
#define PPUTLIMPL_UINT_TRAIT_4036u DEC, 0xFC4, 1, 11u, 63u, (2u, 2u, 1009u)
#define PPUTLIMPL_UINT_TRAIT_4035u DEC, 0xFC3, 1, 11u, 63u, (3u, 5u, 269u)
#define PPUTLIMPL_UINT_TRAIT_4034u DEC, 0xFC2, 1, 11u, 63u, (2u, 2017u)
#define PPUTLIMPL_UINT_TRAIT_4033u DEC, 0xFC1, 1, 11u, 63u, (37u, 109u)
#define PPUTLIMPL_UINT_TRAIT_4032u DEC, 0xFC0, 1, 11u, 63u, (2u, 2u, 2u, 2u, 2u, 2u, 3u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_4031u DEC, 0xFBF, 1, 11u, 63u, (29u, 139u)
#define PPUTLIMPL_UINT_TRAIT_4030u DEC, 0xFBE, 1, 11u, 63u, (2u, 5u, 13u, 31u)
#define PPUTLIMPL_UINT_TRAIT_4029u DEC, 0xFBD, 1, 11u, 63u, (3u, 17u, 79u)
#define PPUTLIMPL_UINT_TRAIT_4028u DEC, 0xFBC, 1, 11u, 63u, (2u, 2u, 19u, 53u)
#define PPUTLIMPL_UINT_TRAIT_4027u DEC, 0xFBB, 1, 11u, 63u, ()
#define PPUTLIMPL_UINT_TRAIT_4026u DEC, 0xFBA, 1, 11u, 63u, (2u, 3u, 11u, 61u)
#define PPUTLIMPL_UINT_TRAIT_4025u DEC, 0xFB9, 1, 11u, 63u, (5u, 5u, 7u, 23u)
#define PPUTLIMPL_UINT_TRAIT_4024u DEC, 0xFB8, 1, 11u, 63u, (2u, 2u, 2u, 503u)
#define PPUTLIMPL_UINT_TRAIT_4023u DEC, 0xFB7, 1, 11u, 63u, (3u, 3u, 3u, 149u)
#define PPUTLIMPL_UINT_TRAIT_4022u DEC, 0xFB6, 1, 11u, 63u, (2u, 2011u)
#define PPUTLIMPL_UINT_TRAIT_4021u DEC, 0xFB5, 1, 11u, 63u, ()
#define PPUTLIMPL_UINT_TRAIT_4020u DEC, 0xFB4, 1, 11u, 63u, (2u, 2u, 3u, 5u, 67u)
#define PPUTLIMPL_UINT_TRAIT_4019u DEC, 0xFB3, 1, 11u, 63u, ()
#define PPUTLIMPL_UINT_TRAIT_4018u DEC, 0xFB2, 1, 11u, 63u, (2u, 7u, 7u, 41u)
#define PPUTLIMPL_UINT_TRAIT_4017u DEC, 0xFB1, 1, 11u, 63u, (3u, 13u, 103u)
#define PPUTLIMPL_UINT_TRAIT_4016u DEC, 0xFB0, 1, 11u, 63u, (2u, 2u, 2u, 2u, 251u)
#define PPUTLIMPL_UINT_TRAIT_4015u DEC, 0xFAF, 1, 11u, 63u, (5u, 11u, 73u)
#define PPUTLIMPL_UINT_TRAIT_4014u DEC, 0xFAE, 1, 11u, 63u, (2u, 3u, 3u, 223u)
#define PPUTLIMPL_UINT_TRAIT_4013u DEC, 0xFAD, 1, 11u, 63u, ()
#define PPUTLIMPL_UINT_TRAIT_4012u DEC, 0xFAC, 1, 11u, 63u, (2u, 2u, 17u, 59u)
#define PPUTLIMPL_UINT_TRAIT_4011u DEC, 0xFAB, 1, 11u, 63u, (3u, 7u, 191u)
#define PPUTLIMPL_UINT_TRAIT_4010u DEC, 0xFAA, 1, 11u, 63u, (2u, 5u, 401u)
#define PPUTLIMPL_UINT_TRAIT_4009u DEC, 0xFA9, 1, 11u, 63u, (19u, 211u)
#define PPUTLIMPL_UINT_TRAIT_4008u DEC, 0xFA8, 1, 11u, 63u, (2u, 2u, 2u, 3u, 167u)
#define PPUTLIMPL_UINT_TRAIT_4007u DEC, 0xFA7, 1, 11u, 63u, ()
#define PPUTLIMPL_UINT_TRAIT_4006u DEC, 0xFA6, 1, 11u, 63u, (2u, 2003u)
#define PPUTLIMPL_UINT_TRAIT_4005u DEC, 0xFA5, 1, 11u, 63u, (3u, 3u, 5u, 89u)
#define PPUTLIMPL_UINT_TRAIT_4004u DEC, 0xFA4, 1, 11u, 63u, (2u, 2u, 7u, 11u, 13u)
#define PPUTLIMPL_UINT_TRAIT_4003u DEC, 0xFA3, 1, 11u, 63u, ()
#define PPUTLIMPL_UINT_TRAIT_4002u DEC, 0xFA2, 1, 11u, 63u, (2u, 3u, 23u, 29u)
#define PPUTLIMPL_UINT_TRAIT_4001u DEC, 0xFA1, 1, 11u, 63u, ()
#define PPUTLIMPL_UINT_TRAIT_4000u DEC, 0xFA0, 1, 11u, 63u, (2u, 2u, 2u, 2u, 2u, 5u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_3999u DEC, 0xF9F, 1, 11u, 63u, (3u, 31u, 43u)
#define PPUTLIMPL_UINT_TRAIT_3998u DEC, 0xF9E, 1, 11u, 63u, (2u, 1999u)
#define PPUTLIMPL_UINT_TRAIT_3997u DEC, 0xF9D, 1, 11u, 63u, (7u, 571u)
#define PPUTLIMPL_UINT_TRAIT_3996u DEC, 0xF9C, 1, 11u, 63u, (2u, 2u, 3u, 3u, 3u, 37u)
#define PPUTLIMPL_UINT_TRAIT_3995u DEC, 0xF9B, 1, 11u, 63u, (5u, 17u, 47u)
#define PPUTLIMPL_UINT_TRAIT_3994u DEC, 0xF9A, 1, 11u, 63u, (2u, 1997u)
#define PPUTLIMPL_UINT_TRAIT_3993u DEC, 0xF99, 1, 11u, 63u, (3u, 11u, 11u, 11u)
#define PPUTLIMPL_UINT_TRAIT_3992u DEC, 0xF98, 1, 11u, 63u, (2u, 2u, 2u, 499u)
#define PPUTLIMPL_UINT_TRAIT_3991u DEC, 0xF97, 1, 11u, 63u, (13u, 307u)
#define PPUTLIMPL_UINT_TRAIT_3990u DEC, 0xF96, 1, 11u, 63u, (2u, 3u, 5u, 7u, 19u)
#define PPUTLIMPL_UINT_TRAIT_3989u DEC, 0xF95, 1, 11u, 63u, ()
#define PPUTLIMPL_UINT_TRAIT_3988u DEC, 0xF94, 1, 11u, 63u, (2u, 2u, 997u)
#define PPUTLIMPL_UINT_TRAIT_3987u DEC, 0xF93, 1, 11u, 63u, (3u, 3u, 443u)
#define PPUTLIMPL_UINT_TRAIT_3986u DEC, 0xF92, 1, 11u, 63u, (2u, 1993u)
#define PPUTLIMPL_UINT_TRAIT_3985u DEC, 0xF91, 1, 11u, 63u, (5u, 797u)
#define PPUTLIMPL_UINT_TRAIT_3984u DEC, 0xF90, 1, 11u, 63u, (2u, 2u, 2u, 2u, 3u, 83u)
#define PPUTLIMPL_UINT_TRAIT_3983u DEC, 0xF8F, 1, 11u, 63u, (7u, 569u)
#define PPUTLIMPL_UINT_TRAIT_3982u DEC, 0xF8E, 1, 11u, 63u, (2u, 11u, 181u)
#define PPUTLIMPL_UINT_TRAIT_3981u DEC, 0xF8D, 1, 11u, 63u, (3u, 1327u)
#define PPUTLIMPL_UINT_TRAIT_3980u DEC, 0xF8C, 1, 11u, 63u, (2u, 2u, 5u, 199u)
#define PPUTLIMPL_UINT_TRAIT_3979u DEC, 0xF8B, 1, 11u, 63u, (23u, 173u)
#define PPUTLIMPL_UINT_TRAIT_3978u DEC, 0xF8A, 1, 11u, 63u, (2u, 3u, 3u, 13u, 17u)
#define PPUTLIMPL_UINT_TRAIT_3977u DEC, 0xF89, 1, 11u, 63u, (41u, 97u)
#define PPUTLIMPL_UINT_TRAIT_3976u DEC, 0xF88, 1, 11u, 63u, (2u, 2u, 2u, 7u, 71u)
#define PPUTLIMPL_UINT_TRAIT_3975u DEC, 0xF87, 1, 11u, 63u, (3u, 5u, 5u, 53u)
#define PPUTLIMPL_UINT_TRAIT_3974u DEC, 0xF86, 1, 11u, 63u, (2u, 1987u)
#define PPUTLIMPL_UINT_TRAIT_3973u DEC, 0xF85, 1, 11u, 63u, (29u, 137u)
#define PPUTLIMPL_UINT_TRAIT_3972u DEC, 0xF84, 1, 11u, 63u, (2u, 2u, 3u, 331u)
#define PPUTLIMPL_UINT_TRAIT_3971u DEC, 0xF83, 1, 11u, 63u, (11u, 19u, 19u)
#define PPUTLIMPL_UINT_TRAIT_3970u DEC, 0xF82, 1, 11u, 63u, (2u, 5u, 397u)
#define PPUTLIMPL_UINT_TRAIT_3969u DEC, 0xF81, 1, 11u, 63u, (3u, 3u, 3u, 3u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_3968u DEC, 0xF80, 1, 11u, 62u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 31u)
#define PPUTLIMPL_UINT_TRAIT_3967u DEC, 0xF7F, 1, 11u, 62u, ()
#define PPUTLIMPL_UINT_TRAIT_3966u DEC, 0xF7E, 1, 11u, 62u, (2u, 3u, 661u)
#define PPUTLIMPL_UINT_TRAIT_3965u DEC, 0xF7D, 1, 11u, 62u, (5u, 13u, 61u)
#define PPUTLIMPL_UINT_TRAIT_3964u DEC, 0xF7C, 1, 11u, 62u, (2u, 2u, 991u)
#define PPUTLIMPL_UINT_TRAIT_3963u DEC, 0xF7B, 1, 11u, 62u, (3u, 1321u)
#define PPUTLIMPL_UINT_TRAIT_3962u DEC, 0xF7A, 1, 11u, 62u, (2u, 7u, 283u)
#define PPUTLIMPL_UINT_TRAIT_3961u DEC, 0xF79, 1, 11u, 62u, (17u, 233u)
#define PPUTLIMPL_UINT_TRAIT_3960u DEC, 0xF78, 1, 11u, 62u, (2u, 2u, 2u, 3u, 3u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_3959u DEC, 0xF77, 1, 11u, 62u, (37u, 107u)
#define PPUTLIMPL_UINT_TRAIT_3958u DEC, 0xF76, 1, 11u, 62u, (2u, 1979u)
#define PPUTLIMPL_UINT_TRAIT_3957u DEC, 0xF75, 1, 11u, 62u, (3u, 1319u)
#define PPUTLIMPL_UINT_TRAIT_3956u DEC, 0xF74, 1, 11u, 62u, (2u, 2u, 23u, 43u)
#define PPUTLIMPL_UINT_TRAIT_3955u DEC, 0xF73, 1, 11u, 62u, (5u, 7u, 113u)
#define PPUTLIMPL_UINT_TRAIT_3954u DEC, 0xF72, 1, 11u, 62u, (2u, 3u, 659u)
#define PPUTLIMPL_UINT_TRAIT_3953u DEC, 0xF71, 1, 11u, 62u, (59u, 67u)
#define PPUTLIMPL_UINT_TRAIT_3952u DEC, 0xF70, 1, 11u, 62u, (2u, 2u, 2u, 2u, 13u, 19u)
#define PPUTLIMPL_UINT_TRAIT_3951u DEC, 0xF6F, 1, 11u, 62u, (3u, 3u, 439u)
#define PPUTLIMPL_UINT_TRAIT_3950u DEC, 0xF6E, 1, 11u, 62u, (2u, 5u, 5u, 79u)
#define PPUTLIMPL_UINT_TRAIT_3949u DEC, 0xF6D, 1, 11u, 62u, (11u, 359u)
#define PPUTLIMPL_UINT_TRAIT_3948u DEC, 0xF6C, 1, 11u, 62u, (2u, 2u, 3u, 7u, 47u)
#define PPUTLIMPL_UINT_TRAIT_3947u DEC, 0xF6B, 1, 11u, 62u, ()
#define PPUTLIMPL_UINT_TRAIT_3946u DEC, 0xF6A, 1, 11u, 62u, (2u, 1973u)
#define PPUTLIMPL_UINT_TRAIT_3945u DEC, 0xF69, 1, 11u, 62u, (3u, 5u, 263u)
#define PPUTLIMPL_UINT_TRAIT_3944u DEC, 0xF68, 1, 11u, 62u, (2u, 2u, 2u, 17u, 29u)
#define PPUTLIMPL_UINT_TRAIT_3943u DEC, 0xF67, 1, 11u, 62u, ()
#define PPUTLIMPL_UINT_TRAIT_3942u DEC, 0xF66, 1, 11u, 62u, (2u, 3u, 3u, 3u, 73u)
#define PPUTLIMPL_UINT_TRAIT_3941u DEC, 0xF65, 1, 11u, 62u, (7u, 563u)
#define PPUTLIMPL_UINT_TRAIT_3940u DEC, 0xF64, 1, 11u, 62u, (2u, 2u, 5u, 197u)
#define PPUTLIMPL_UINT_TRAIT_3939u DEC, 0xF63, 1, 11u, 62u, (3u, 13u, 101u)
#define PPUTLIMPL_UINT_TRAIT_3938u DEC, 0xF62, 1, 11u, 62u, (2u, 11u, 179u)
#define PPUTLIMPL_UINT_TRAIT_3937u DEC, 0xF61, 1, 11u, 62u, (31u, 127u)
#define PPUTLIMPL_UINT_TRAIT_3936u DEC, 0xF60, 1, 11u, 62u, (2u, 2u, 2u, 2u, 2u, 3u, 41u)
#define PPUTLIMPL_UINT_TRAIT_3935u DEC, 0xF5F, 1, 11u, 62u, (5u, 787u)
#define PPUTLIMPL_UINT_TRAIT_3934u DEC, 0xF5E, 1, 11u, 62u, (2u, 7u, 281u)
#define PPUTLIMPL_UINT_TRAIT_3933u DEC, 0xF5D, 1, 11u, 62u, (3u, 3u, 19u, 23u)
#define PPUTLIMPL_UINT_TRAIT_3932u DEC, 0xF5C, 1, 11u, 62u, (2u, 2u, 983u)
#define PPUTLIMPL_UINT_TRAIT_3931u DEC, 0xF5B, 1, 11u, 62u, ()
#define PPUTLIMPL_UINT_TRAIT_3930u DEC, 0xF5A, 1, 11u, 62u, (2u, 3u, 5u, 131u)
#define PPUTLIMPL_UINT_TRAIT_3929u DEC, 0xF59, 1, 11u, 62u, ()
#define PPUTLIMPL_UINT_TRAIT_3928u DEC, 0xF58, 1, 11u, 62u, (2u, 2u, 2u, 491u)
#define PPUTLIMPL_UINT_TRAIT_3927u DEC, 0xF57, 1, 11u, 62u, (3u, 7u, 11u, 17u)
#define PPUTLIMPL_UINT_TRAIT_3926u DEC, 0xF56, 1, 11u, 62u, (2u, 13u, 151u)
#define PPUTLIMPL_UINT_TRAIT_3925u DEC, 0xF55, 1, 11u, 62u, (5u, 5u, 157u)
#define PPUTLIMPL_UINT_TRAIT_3924u DEC, 0xF54, 1, 11u, 62u, (2u, 2u, 3u, 3u, 109u)
#define PPUTLIMPL_UINT_TRAIT_3923u DEC, 0xF53, 1, 11u, 62u, ()
#define PPUTLIMPL_UINT_TRAIT_3922u DEC, 0xF52, 1, 11u, 62u, (2u, 37u, 53u)
#define PPUTLIMPL_UINT_TRAIT_3921u DEC, 0xF51, 1, 11u, 62u, (3u, 1307u)
#define PPUTLIMPL_UINT_TRAIT_3920u DEC, 0xF50, 1, 11u, 62u, (2u, 2u, 2u, 2u, 5u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_3919u DEC, 0xF4F, 1, 11u, 62u, ()
#define PPUTLIMPL_UINT_TRAIT_3918u DEC, 0xF4E, 1, 11u, 62u, (2u, 3u, 653u)
#define PPUTLIMPL_UINT_TRAIT_3917u DEC, 0xF4D, 1, 11u, 62u, ()
#define PPUTLIMPL_UINT_TRAIT_3916u DEC, 0xF4C, 1, 11u, 62u, (2u, 2u, 11u, 89u)
#define PPUTLIMPL_UINT_TRAIT_3915u DEC, 0xF4B, 1, 11u, 62u, (3u, 3u, 3u, 5u, 29u)
#define PPUTLIMPL_UINT_TRAIT_3914u DEC, 0xF4A, 1, 11u, 62u, (2u, 19u, 103u)
#define PPUTLIMPL_UINT_TRAIT_3913u DEC, 0xF49, 1, 11u, 62u, (7u, 13u, 43u)
#define PPUTLIMPL_UINT_TRAIT_3912u DEC, 0xF48, 1, 11u, 62u, (2u, 2u, 2u, 3u, 163u)
#define PPUTLIMPL_UINT_TRAIT_3911u DEC, 0xF47, 1, 11u, 62u, ()
#define PPUTLIMPL_UINT_TRAIT_3910u DEC, 0xF46, 1, 11u, 62u, (2u, 5u, 17u, 23u)
#define PPUTLIMPL_UINT_TRAIT_3909u DEC, 0xF45, 1, 11u, 62u, (3u, 1303u)
#define PPUTLIMPL_UINT_TRAIT_3908u DEC, 0xF44, 1, 11u, 62u, (2u, 2u, 977u)
#define PPUTLIMPL_UINT_TRAIT_3907u DEC, 0xF43, 1, 11u, 62u, ()
#define PPUTLIMPL_UINT_TRAIT_3906u DEC, 0xF42, 1, 11u, 62u, (2u, 3u, 3u, 7u, 31u)
#define PPUTLIMPL_UINT_TRAIT_3905u DEC, 0xF41, 1, 11u, 62u, (5u, 11u, 71u)
#define PPUTLIMPL_UINT_TRAIT_3904u DEC, 0xF40, 1, 11u, 62u, (2u, 2u, 2u, 2u, 2u, 2u, 61u)
#define PPUTLIMPL_UINT_TRAIT_3903u DEC, 0xF3F, 1, 11u, 62u, (3u, 1301u)
#define PPUTLIMPL_UINT_TRAIT_3902u DEC, 0xF3E, 1, 11u, 62u, (2u, 1951u)
#define PPUTLIMPL_UINT_TRAIT_3901u DEC, 0xF3D, 1, 11u, 62u, (47u, 83u)
#define PPUTLIMPL_UINT_TRAIT_3900u DEC, 0xF3C, 1, 11u, 62u, (2u, 2u, 3u, 5u, 5u, 13u)
#define PPUTLIMPL_UINT_TRAIT_3899u DEC, 0xF3B, 1, 11u, 62u, (7u, 557u)
#define PPUTLIMPL_UINT_TRAIT_3898u DEC, 0xF3A, 1, 11u, 62u, (2u, 1949u)
#define PPUTLIMPL_UINT_TRAIT_3897u DEC, 0xF39, 1, 11u, 62u, (3u, 3u, 433u)
#define PPUTLIMPL_UINT_TRAIT_3896u DEC, 0xF38, 1, 11u, 62u, (2u, 2u, 2u, 487u)
#define PPUTLIMPL_UINT_TRAIT_3895u DEC, 0xF37, 1, 11u, 62u, (5u, 19u, 41u)
#define PPUTLIMPL_UINT_TRAIT_3894u DEC, 0xF36, 1, 11u, 62u, (2u, 3u, 11u, 59u)
#define PPUTLIMPL_UINT_TRAIT_3893u DEC, 0xF35, 1, 11u, 62u, (17u, 229u)
#define PPUTLIMPL_UINT_TRAIT_3892u DEC, 0xF34, 1, 11u, 62u, (2u, 2u, 7u, 139u)
#define PPUTLIMPL_UINT_TRAIT_3891u DEC, 0xF33, 1, 11u, 62u, (3u, 1297u)
#define PPUTLIMPL_UINT_TRAIT_3890u DEC, 0xF32, 1, 11u, 62u, (2u, 5u, 389u)
#define PPUTLIMPL_UINT_TRAIT_3889u DEC, 0xF31, 1, 11u, 62u, ()
#define PPUTLIMPL_UINT_TRAIT_3888u DEC, 0xF30, 1, 11u, 62u, (2u, 2u, 2u, 2u, 3u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_3887u DEC, 0xF2F, 1, 11u, 62u, (13u, 13u, 23u)
#define PPUTLIMPL_UINT_TRAIT_3886u DEC, 0xF2E, 1, 11u, 62u, (2u, 29u, 67u)
#define PPUTLIMPL_UINT_TRAIT_3885u DEC, 0xF2D, 1, 11u, 62u, (3u, 5u, 7u, 37u)
#define PPUTLIMPL_UINT_TRAIT_3884u DEC, 0xF2C, 1, 11u, 62u, (2u, 2u, 971u)
#define PPUTLIMPL_UINT_TRAIT_3883u DEC, 0xF2B, 1, 11u, 62u, (11u, 353u)
#define PPUTLIMPL_UINT_TRAIT_3882u DEC, 0xF2A, 1, 11u, 62u, (2u, 3u, 647u)
#define PPUTLIMPL_UINT_TRAIT_3881u DEC, 0xF29, 1, 11u, 62u, ()
#define PPUTLIMPL_UINT_TRAIT_3880u DEC, 0xF28, 1, 11u, 62u, (2u, 2u, 2u, 5u, 97u)
#define PPUTLIMPL_UINT_TRAIT_3879u DEC, 0xF27, 1, 11u, 62u, (3u, 3u, 431u)
#define PPUTLIMPL_UINT_TRAIT_3878u DEC, 0xF26, 1, 11u, 62u, (2u, 7u, 277u)
#define PPUTLIMPL_UINT_TRAIT_3877u DEC, 0xF25, 1, 11u, 62u, ()
#define PPUTLIMPL_UINT_TRAIT_3876u DEC, 0xF24, 1, 11u, 62u, (2u, 2u, 3u, 17u, 19u)
#define PPUTLIMPL_UINT_TRAIT_3875u DEC, 0xF23, 1, 11u, 62u, (5u, 5u, 5u, 31u)
#define PPUTLIMPL_UINT_TRAIT_3874u DEC, 0xF22, 1, 11u, 62u, (2u, 13u, 149u)
#define PPUTLIMPL_UINT_TRAIT_3873u DEC, 0xF21, 1, 11u, 62u, (3u, 1291u)
#define PPUTLIMPL_UINT_TRAIT_3872u DEC, 0xF20, 1, 11u, 62u, (2u, 2u, 2u, 2u, 2u, 11u, 11u)
#define PPUTLIMPL_UINT_TRAIT_3871u DEC, 0xF1F, 1, 11u, 62u, (7u, 7u, 79u)
#define PPUTLIMPL_UINT_TRAIT_3870u DEC, 0xF1E, 1, 11u, 62u, (2u, 3u, 3u, 5u, 43u)
#define PPUTLIMPL_UINT_TRAIT_3869u DEC, 0xF1D, 1, 11u, 62u, (53u, 73u)
#define PPUTLIMPL_UINT_TRAIT_3868u DEC, 0xF1C, 1, 11u, 62u, (2u, 2u, 967u)
#define PPUTLIMPL_UINT_TRAIT_3867u DEC, 0xF1B, 1, 11u, 62u, (3u, 1289u)
#define PPUTLIMPL_UINT_TRAIT_3866u DEC, 0xF1A, 1, 11u, 62u, (2u, 1933u)
#define PPUTLIMPL_UINT_TRAIT_3865u DEC, 0xF19, 1, 11u, 62u, (5u, 773u)
#define PPUTLIMPL_UINT_TRAIT_3864u DEC, 0xF18, 1, 11u, 62u, (2u, 2u, 2u, 3u, 7u, 23u)
#define PPUTLIMPL_UINT_TRAIT_3863u DEC, 0xF17, 1, 11u, 62u, ()
#define PPUTLIMPL_UINT_TRAIT_3862u DEC, 0xF16, 1, 11u, 62u, (2u, 1931u)
#define PPUTLIMPL_UINT_TRAIT_3861u DEC, 0xF15, 1, 11u, 62u, (3u, 3u, 3u, 11u, 13u)
#define PPUTLIMPL_UINT_TRAIT_3860u DEC, 0xF14, 1, 11u, 62u, (2u, 2u, 5u, 193u)
#define PPUTLIMPL_UINT_TRAIT_3859u DEC, 0xF13, 1, 11u, 62u, (17u, 227u)
#define PPUTLIMPL_UINT_TRAIT_3858u DEC, 0xF12, 1, 11u, 62u, (2u, 3u, 643u)
#define PPUTLIMPL_UINT_TRAIT_3857u DEC, 0xF11, 1, 11u, 62u, (7u, 19u, 29u)
#define PPUTLIMPL_UINT_TRAIT_3856u DEC, 0xF10, 1, 11u, 62u, (2u, 2u, 2u, 2u, 241u)
#define PPUTLIMPL_UINT_TRAIT_3855u DEC, 0xF0F, 1, 11u, 62u, (3u, 5u, 257u)
#define PPUTLIMPL_UINT_TRAIT_3854u DEC, 0xF0E, 1, 11u, 62u, (2u, 41u, 47u)
#define PPUTLIMPL_UINT_TRAIT_3853u DEC, 0xF0D, 1, 11u, 62u, ()
#define PPUTLIMPL_UINT_TRAIT_3852u DEC, 0xF0C, 1, 11u, 62u, (2u, 2u, 3u, 3u, 107u)
#define PPUTLIMPL_UINT_TRAIT_3851u DEC, 0xF0B, 1, 11u, 62u, ()
#define PPUTLIMPL_UINT_TRAIT_3850u DEC, 0xF0A, 1, 11u, 62u, (2u, 5u, 5u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_3849u DEC, 0xF09, 1, 11u, 62u, (3u, 1283u)
#define PPUTLIMPL_UINT_TRAIT_3848u DEC, 0xF08, 1, 11u, 62u, (2u, 2u, 2u, 13u, 37u)
#define PPUTLIMPL_UINT_TRAIT_3847u DEC, 0xF07, 1, 11u, 62u, ()
#define PPUTLIMPL_UINT_TRAIT_3846u DEC, 0xF06, 1, 11u, 62u, (2u, 3u, 641u)
#define PPUTLIMPL_UINT_TRAIT_3845u DEC, 0xF05, 1, 11u, 62u, (5u, 769u)
#define PPUTLIMPL_UINT_TRAIT_3844u DEC, 0xF04, 1, 11u, 62u, (2u, 2u, 31u, 31u)
#define PPUTLIMPL_UINT_TRAIT_3843u DEC, 0xF03, 1, 11u, 61u, (3u, 3u, 7u, 61u)
#define PPUTLIMPL_UINT_TRAIT_3842u DEC, 0xF02, 1, 11u, 61u, (2u, 17u, 113u)
#define PPUTLIMPL_UINT_TRAIT_3841u DEC, 0xF01, 1, 11u, 61u, (23u, 167u)
#define PPUTLIMPL_UINT_TRAIT_3840u DEC, 0xF00, 1, 11u, 61u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_3839u DEC, 0xEFF, 1, 11u, 61u, (11u, 349u)
#define PPUTLIMPL_UINT_TRAIT_3838u DEC, 0xEFE, 1, 11u, 61u, (2u, 19u, 101u)
#define PPUTLIMPL_UINT_TRAIT_3837u DEC, 0xEFD, 1, 11u, 61u, (3u, 1279u)
#define PPUTLIMPL_UINT_TRAIT_3836u DEC, 0xEFC, 1, 11u, 61u, (2u, 2u, 7u, 137u)
#define PPUTLIMPL_UINT_TRAIT_3835u DEC, 0xEFB, 1, 11u, 61u, (5u, 13u, 59u)
#define PPUTLIMPL_UINT_TRAIT_3834u DEC, 0xEFA, 1, 11u, 61u, (2u, 3u, 3u, 3u, 71u)
#define PPUTLIMPL_UINT_TRAIT_3833u DEC, 0xEF9, 1, 11u, 61u, ()
#define PPUTLIMPL_UINT_TRAIT_3832u DEC, 0xEF8, 1, 11u, 61u, (2u, 2u, 2u, 479u)
#define PPUTLIMPL_UINT_TRAIT_3831u DEC, 0xEF7, 1, 11u, 61u, (3u, 1277u)
#define PPUTLIMPL_UINT_TRAIT_3830u DEC, 0xEF6, 1, 11u, 61u, (2u, 5u, 383u)
#define PPUTLIMPL_UINT_TRAIT_3829u DEC, 0xEF5, 1, 11u, 61u, (7u, 547u)
#define PPUTLIMPL_UINT_TRAIT_3828u DEC, 0xEF4, 1, 11u, 61u, (2u, 2u, 3u, 11u, 29u)
#define PPUTLIMPL_UINT_TRAIT_3827u DEC, 0xEF3, 1, 11u, 61u, (43u, 89u)
#define PPUTLIMPL_UINT_TRAIT_3826u DEC, 0xEF2, 1, 11u, 61u, (2u, 1913u)
#define PPUTLIMPL_UINT_TRAIT_3825u DEC, 0xEF1, 1, 11u, 61u, (3u, 3u, 5u, 5u, 17u)
#define PPUTLIMPL_UINT_TRAIT_3824u DEC, 0xEF0, 1, 11u, 61u, (2u, 2u, 2u, 2u, 239u)
#define PPUTLIMPL_UINT_TRAIT_3823u DEC, 0xEEF, 1, 11u, 61u, ()
#define PPUTLIMPL_UINT_TRAIT_3822u DEC, 0xEEE, 1, 11u, 61u, (2u, 3u, 7u, 7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_3821u DEC, 0xEED, 1, 11u, 61u, ()
#define PPUTLIMPL_UINT_TRAIT_3820u DEC, 0xEEC, 1, 11u, 61u, (2u, 2u, 5u, 191u)
#define PPUTLIMPL_UINT_TRAIT_3819u DEC, 0xEEB, 1, 11u, 61u, (3u, 19u, 67u)
#define PPUTLIMPL_UINT_TRAIT_3818u DEC, 0xEEA, 1, 11u, 61u, (2u, 23u, 83u)
#define PPUTLIMPL_UINT_TRAIT_3817u DEC, 0xEE9, 1, 11u, 61u, (11u, 347u)
#define PPUTLIMPL_UINT_TRAIT_3816u DEC, 0xEE8, 1, 11u, 61u, (2u, 2u, 2u, 3u, 3u, 53u)
#define PPUTLIMPL_UINT_TRAIT_3815u DEC, 0xEE7, 1, 11u, 61u, (5u, 7u, 109u)
#define PPUTLIMPL_UINT_TRAIT_3814u DEC, 0xEE6, 1, 11u, 61u, (2u, 1907u)
#define PPUTLIMPL_UINT_TRAIT_3813u DEC, 0xEE5, 1, 11u, 61u, (3u, 31u, 41u)
#define PPUTLIMPL_UINT_TRAIT_3812u DEC, 0xEE4, 1, 11u, 61u, (2u, 2u, 953u)
#define PPUTLIMPL_UINT_TRAIT_3811u DEC, 0xEE3, 1, 11u, 61u, (37u, 103u)
#define PPUTLIMPL_UINT_TRAIT_3810u DEC, 0xEE2, 1, 11u, 61u, (2u, 3u, 5u, 127u)
#define PPUTLIMPL_UINT_TRAIT_3809u DEC, 0xEE1, 1, 11u, 61u, (13u, 293u)
#define PPUTLIMPL_UINT_TRAIT_3808u DEC, 0xEE0, 1, 11u, 61u, (2u, 2u, 2u, 2u, 2u, 7u, 17u)
#define PPUTLIMPL_UINT_TRAIT_3807u DEC, 0xEDF, 1, 11u, 61u, (3u, 3u, 3u, 3u, 47u)
#define PPUTLIMPL_UINT_TRAIT_3806u DEC, 0xEDE, 1, 11u, 61u, (2u, 11u, 173u)
#define PPUTLIMPL_UINT_TRAIT_3805u DEC, 0xEDD, 1, 11u, 61u, (5u, 761u)
#define PPUTLIMPL_UINT_TRAIT_3804u DEC, 0xEDC, 1, 11u, 61u, (2u, 2u, 3u, 317u)
#define PPUTLIMPL_UINT_TRAIT_3803u DEC, 0xEDB, 1, 11u, 61u, ()
#define PPUTLIMPL_UINT_TRAIT_3802u DEC, 0xEDA, 1, 11u, 61u, (2u, 1901u)
#define PPUTLIMPL_UINT_TRAIT_3801u DEC, 0xED9, 1, 11u, 61u, (3u, 7u, 181u)
#define PPUTLIMPL_UINT_TRAIT_3800u DEC, 0xED8, 1, 11u, 61u, (2u, 2u, 2u, 5u, 5u, 19u)
#define PPUTLIMPL_UINT_TRAIT_3799u DEC, 0xED7, 1, 11u, 61u, (29u, 131u)
#define PPUTLIMPL_UINT_TRAIT_3798u DEC, 0xED6, 1, 11u, 61u, (2u, 3u, 3u, 211u)
#define PPUTLIMPL_UINT_TRAIT_3797u DEC, 0xED5, 1, 11u, 61u, ()
#define PPUTLIMPL_UINT_TRAIT_3796u DEC, 0xED4, 1, 11u, 61u, (2u, 2u, 13u, 73u)
#define PPUTLIMPL_UINT_TRAIT_3795u DEC, 0xED3, 1, 11u, 61u, (3u, 5u, 11u, 23u)
#define PPUTLIMPL_UINT_TRAIT_3794u DEC, 0xED2, 1, 11u, 61u, (2u, 7u, 271u)
#define PPUTLIMPL_UINT_TRAIT_3793u DEC, 0xED1, 1, 11u, 61u, ()
#define PPUTLIMPL_UINT_TRAIT_3792u DEC, 0xED0, 1, 11u, 61u, (2u, 2u, 2u, 2u, 3u, 79u)
#define PPUTLIMPL_UINT_TRAIT_3791u DEC, 0xECF, 1, 11u, 61u, (17u, 223u)
#define PPUTLIMPL_UINT_TRAIT_3790u DEC, 0xECE, 1, 11u, 61u, (2u, 5u, 379u)
#define PPUTLIMPL_UINT_TRAIT_3789u DEC, 0xECD, 1, 11u, 61u, (3u, 3u, 421u)
#define PPUTLIMPL_UINT_TRAIT_3788u DEC, 0xECC, 1, 11u, 61u, (2u, 2u, 947u)
#define PPUTLIMPL_UINT_TRAIT_3787u DEC, 0xECB, 1, 11u, 61u, (7u, 541u)
#define PPUTLIMPL_UINT_TRAIT_3786u DEC, 0xECA, 1, 11u, 61u, (2u, 3u, 631u)
#define PPUTLIMPL_UINT_TRAIT_3785u DEC, 0xEC9, 1, 11u, 61u, (5u, 757u)
#define PPUTLIMPL_UINT_TRAIT_3784u DEC, 0xEC8, 1, 11u, 61u, (2u, 2u, 2u, 11u, 43u)
#define PPUTLIMPL_UINT_TRAIT_3783u DEC, 0xEC7, 1, 11u, 61u, (3u, 13u, 97u)
#define PPUTLIMPL_UINT_TRAIT_3782u DEC, 0xEC6, 1, 11u, 61u, (2u, 31u, 61u)
#define PPUTLIMPL_UINT_TRAIT_3781u DEC, 0xEC5, 1, 11u, 61u, (19u, 199u)
#define PPUTLIMPL_UINT_TRAIT_3780u DEC, 0xEC4, 1, 11u, 61u, (2u, 2u, 3u, 3u, 3u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_3779u DEC, 0xEC3, 1, 11u, 61u, ()
#define PPUTLIMPL_UINT_TRAIT_3778u DEC, 0xEC2, 1, 11u, 61u, (2u, 1889u)
#define PPUTLIMPL_UINT_TRAIT_3777u DEC, 0xEC1, 1, 11u, 61u, (3u, 1259u)
#define PPUTLIMPL_UINT_TRAIT_3776u DEC, 0xEC0, 1, 11u, 61u, (2u, 2u, 2u, 2u, 2u, 2u, 59u)
#define PPUTLIMPL_UINT_TRAIT_3775u DEC, 0xEBF, 1, 11u, 61u, (5u, 5u, 151u)
#define PPUTLIMPL_UINT_TRAIT_3774u DEC, 0xEBE, 1, 11u, 61u, (2u, 3u, 17u, 37u)
#define PPUTLIMPL_UINT_TRAIT_3773u DEC, 0xEBD, 1, 11u, 61u, (7u, 7u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_3772u DEC, 0xEBC, 1, 11u, 61u, (2u, 2u, 23u, 41u)
#define PPUTLIMPL_UINT_TRAIT_3771u DEC, 0xEBB, 1, 11u, 61u, (3u, 3u, 419u)
#define PPUTLIMPL_UINT_TRAIT_3770u DEC, 0xEBA, 1, 11u, 61u, (2u, 5u, 13u, 29u)
#define PPUTLIMPL_UINT_TRAIT_3769u DEC, 0xEB9, 1, 11u, 61u, ()
#define PPUTLIMPL_UINT_TRAIT_3768u DEC, 0xEB8, 1, 11u, 61u, (2u, 2u, 2u, 3u, 157u)
#define PPUTLIMPL_UINT_TRAIT_3767u DEC, 0xEB7, 1, 11u, 61u, ()
#define PPUTLIMPL_UINT_TRAIT_3766u DEC, 0xEB6, 1, 11u, 61u, (2u, 7u, 269u)
#define PPUTLIMPL_UINT_TRAIT_3765u DEC, 0xEB5, 1, 11u, 61u, (3u, 5u, 251u)
#define PPUTLIMPL_UINT_TRAIT_3764u DEC, 0xEB4, 1, 11u, 61u, (2u, 2u, 941u)
#define PPUTLIMPL_UINT_TRAIT_3763u DEC, 0xEB3, 1, 11u, 61u, (53u, 71u)
#define PPUTLIMPL_UINT_TRAIT_3762u DEC, 0xEB2, 1, 11u, 61u, (2u, 3u, 3u, 11u, 19u)
#define PPUTLIMPL_UINT_TRAIT_3761u DEC, 0xEB1, 1, 11u, 61u, ()
#define PPUTLIMPL_UINT_TRAIT_3760u DEC, 0xEB0, 1, 11u, 61u, (2u, 2u, 2u, 2u, 5u, 47u)
#define PPUTLIMPL_UINT_TRAIT_3759u DEC, 0xEAF, 1, 11u, 61u, (3u, 7u, 179u)
#define PPUTLIMPL_UINT_TRAIT_3758u DEC, 0xEAE, 1, 11u, 61u, (2u, 1879u)
#define PPUTLIMPL_UINT_TRAIT_3757u DEC, 0xEAD, 1, 11u, 61u, (13u, 17u, 17u)
#define PPUTLIMPL_UINT_TRAIT_3756u DEC, 0xEAC, 1, 11u, 61u, (2u, 2u, 3u, 313u)
#define PPUTLIMPL_UINT_TRAIT_3755u DEC, 0xEAB, 1, 11u, 61u, (5u, 751u)
#define PPUTLIMPL_UINT_TRAIT_3754u DEC, 0xEAA, 1, 11u, 61u, (2u, 1877u)
#define PPUTLIMPL_UINT_TRAIT_3753u DEC, 0xEA9, 1, 11u, 61u, (3u, 3u, 3u, 139u)
#define PPUTLIMPL_UINT_TRAIT_3752u DEC, 0xEA8, 1, 11u, 61u, (2u, 2u, 2u, 7u, 67u)
#define PPUTLIMPL_UINT_TRAIT_3751u DEC, 0xEA7, 1, 11u, 61u, (11u, 11u, 31u)
#define PPUTLIMPL_UINT_TRAIT_3750u DEC, 0xEA6, 1, 11u, 61u, (2u, 3u, 5u, 5u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_3749u DEC, 0xEA5, 1, 11u, 61u, (23u, 163u)
#define PPUTLIMPL_UINT_TRAIT_3748u DEC, 0xEA4, 1, 11u, 61u, (2u, 2u, 937u)
#define PPUTLIMPL_UINT_TRAIT_3747u DEC, 0xEA3, 1, 11u, 61u, (3u, 1249u)
#define PPUTLIMPL_UINT_TRAIT_3746u DEC, 0xEA2, 1, 11u, 61u, (2u, 1873u)
#define PPUTLIMPL_UINT_TRAIT_3745u DEC, 0xEA1, 1, 11u, 61u, (5u, 7u, 107u)
#define PPUTLIMPL_UINT_TRAIT_3744u DEC, 0xEA0, 1, 11u, 61u, (2u, 2u, 2u, 2u, 2u, 3u, 3u, 13u)
#define PPUTLIMPL_UINT_TRAIT_3743u DEC, 0xE9F, 1, 11u, 61u, (19u, 197u)
#define PPUTLIMPL_UINT_TRAIT_3742u DEC, 0xE9E, 1, 11u, 61u, (2u, 1871u)
#define PPUTLIMPL_UINT_TRAIT_3741u DEC, 0xE9D, 1, 11u, 61u, (3u, 29u, 43u)
#define PPUTLIMPL_UINT_TRAIT_3740u DEC, 0xE9C, 1, 11u, 61u, (2u, 2u, 5u, 11u, 17u)
#define PPUTLIMPL_UINT_TRAIT_3739u DEC, 0xE9B, 1, 11u, 61u, ()
#define PPUTLIMPL_UINT_TRAIT_3738u DEC, 0xE9A, 1, 11u, 61u, (2u, 3u, 7u, 89u)
#define PPUTLIMPL_UINT_TRAIT_3737u DEC, 0xE99, 1, 11u, 61u, (37u, 101u)
#define PPUTLIMPL_UINT_TRAIT_3736u DEC, 0xE98, 1, 11u, 61u, (2u, 2u, 2u, 467u)
#define PPUTLIMPL_UINT_TRAIT_3735u DEC, 0xE97, 1, 11u, 61u, (3u, 3u, 5u, 83u)
#define PPUTLIMPL_UINT_TRAIT_3734u DEC, 0xE96, 1, 11u, 61u, (2u, 1867u)
#define PPUTLIMPL_UINT_TRAIT_3733u DEC, 0xE95, 1, 11u, 61u, ()
#define PPUTLIMPL_UINT_TRAIT_3732u DEC, 0xE94, 1, 11u, 61u, (2u, 2u, 3u, 311u)
#define PPUTLIMPL_UINT_TRAIT_3731u DEC, 0xE93, 1, 11u, 61u, (7u, 13u, 41u)
#define PPUTLIMPL_UINT_TRAIT_3730u DEC, 0xE92, 1, 11u, 61u, (2u, 5u, 373u)
#define PPUTLIMPL_UINT_TRAIT_3729u DEC, 0xE91, 1, 11u, 61u, (3u, 11u, 113u)
#define PPUTLIMPL_UINT_TRAIT_3728u DEC, 0xE90, 1, 11u, 61u, (2u, 2u, 2u, 2u, 233u)
#define PPUTLIMPL_UINT_TRAIT_3727u DEC, 0xE8F, 1, 11u, 61u, ()
#define PPUTLIMPL_UINT_TRAIT_3726u DEC, 0xE8E, 1, 11u, 61u, (2u, 3u, 3u, 3u, 3u, 23u)
#define PPUTLIMPL_UINT_TRAIT_3725u DEC, 0xE8D, 1, 11u, 61u, (5u, 5u, 149u)
#define PPUTLIMPL_UINT_TRAIT_3724u DEC, 0xE8C, 1, 11u, 61u, (2u, 2u, 7u, 7u, 19u)
#define PPUTLIMPL_UINT_TRAIT_3723u DEC, 0xE8B, 1, 11u, 61u, (3u, 17u, 73u)
#define PPUTLIMPL_UINT_TRAIT_3722u DEC, 0xE8A, 1, 11u, 61u, (2u, 1861u)
#define PPUTLIMPL_UINT_TRAIT_3721u DEC, 0xE89, 1, 11u, 61u, (61u, 61u)
#define PPUTLIMPL_UINT_TRAIT_3720u DEC, 0xE88, 1, 11u, 60u, (2u, 2u, 2u, 3u, 5u, 31u)
#define PPUTLIMPL_UINT_TRAIT_3719u DEC, 0xE87, 1, 11u, 60u, ()
#define PPUTLIMPL_UINT_TRAIT_3718u DEC, 0xE86, 1, 11u, 60u, (2u, 11u, 13u, 13u)
#define PPUTLIMPL_UINT_TRAIT_3717u DEC, 0xE85, 1, 11u, 60u, (3u, 3u, 7u, 59u)
#define PPUTLIMPL_UINT_TRAIT_3716u DEC, 0xE84, 1, 11u, 60u, (2u, 2u, 929u)
#define PPUTLIMPL_UINT_TRAIT_3715u DEC, 0xE83, 1, 11u, 60u, (5u, 743u)
#define PPUTLIMPL_UINT_TRAIT_3714u DEC, 0xE82, 1, 11u, 60u, (2u, 3u, 619u)
#define PPUTLIMPL_UINT_TRAIT_3713u DEC, 0xE81, 1, 11u, 60u, (47u, 79u)
#define PPUTLIMPL_UINT_TRAIT_3712u DEC, 0xE80, 1, 11u, 60u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 29u)
#define PPUTLIMPL_UINT_TRAIT_3711u DEC, 0xE7F, 1, 11u, 60u, (3u, 1237u)
#define PPUTLIMPL_UINT_TRAIT_3710u DEC, 0xE7E, 1, 11u, 60u, (2u, 5u, 7u, 53u)
#define PPUTLIMPL_UINT_TRAIT_3709u DEC, 0xE7D, 1, 11u, 60u, ()
#define PPUTLIMPL_UINT_TRAIT_3708u DEC, 0xE7C, 1, 11u, 60u, (2u, 2u, 3u, 3u, 103u)
#define PPUTLIMPL_UINT_TRAIT_3707u DEC, 0xE7B, 1, 11u, 60u, (11u, 337u)
#define PPUTLIMPL_UINT_TRAIT_3706u DEC, 0xE7A, 1, 11u, 60u, (2u, 17u, 109u)
#define PPUTLIMPL_UINT_TRAIT_3705u DEC, 0xE79, 1, 11u, 60u, (3u, 5u, 13u, 19u)
#define PPUTLIMPL_UINT_TRAIT_3704u DEC, 0xE78, 1, 11u, 60u, (2u, 2u, 2u, 463u)
#define PPUTLIMPL_UINT_TRAIT_3703u DEC, 0xE77, 1, 11u, 60u, (7u, 23u, 23u)
#define PPUTLIMPL_UINT_TRAIT_3702u DEC, 0xE76, 1, 11u, 60u, (2u, 3u, 617u)
#define PPUTLIMPL_UINT_TRAIT_3701u DEC, 0xE75, 1, 11u, 60u, ()
#define PPUTLIMPL_UINT_TRAIT_3700u DEC, 0xE74, 1, 11u, 60u, (2u, 2u, 5u, 5u, 37u)
#define PPUTLIMPL_UINT_TRAIT_3699u DEC, 0xE73, 1, 11u, 60u, (3u, 3u, 3u, 137u)
#define PPUTLIMPL_UINT_TRAIT_3698u DEC, 0xE72, 1, 11u, 60u, (2u, 43u, 43u)
#define PPUTLIMPL_UINT_TRAIT_3697u DEC, 0xE71, 1, 11u, 60u, ()
#define PPUTLIMPL_UINT_TRAIT_3696u DEC, 0xE70, 1, 11u, 60u, (2u, 2u, 2u, 2u, 3u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_3695u DEC, 0xE6F, 1, 11u, 60u, (5u, 739u)
#define PPUTLIMPL_UINT_TRAIT_3694u DEC, 0xE6E, 1, 11u, 60u, (2u, 1847u)
#define PPUTLIMPL_UINT_TRAIT_3693u DEC, 0xE6D, 1, 11u, 60u, (3u, 1231u)
#define PPUTLIMPL_UINT_TRAIT_3692u DEC, 0xE6C, 1, 11u, 60u, (2u, 2u, 13u, 71u)
#define PPUTLIMPL_UINT_TRAIT_3691u DEC, 0xE6B, 1, 11u, 60u, ()
#define PPUTLIMPL_UINT_TRAIT_3690u DEC, 0xE6A, 1, 11u, 60u, (2u, 3u, 3u, 5u, 41u)
#define PPUTLIMPL_UINT_TRAIT_3689u DEC, 0xE69, 1, 11u, 60u, (7u, 17u, 31u)
#define PPUTLIMPL_UINT_TRAIT_3688u DEC, 0xE68, 1, 11u, 60u, (2u, 2u, 2u, 461u)
#define PPUTLIMPL_UINT_TRAIT_3687u DEC, 0xE67, 1, 11u, 60u, (3u, 1229u)
#define PPUTLIMPL_UINT_TRAIT_3686u DEC, 0xE66, 1, 11u, 60u, (2u, 19u, 97u)
#define PPUTLIMPL_UINT_TRAIT_3685u DEC, 0xE65, 1, 11u, 60u, (5u, 11u, 67u)
#define PPUTLIMPL_UINT_TRAIT_3684u DEC, 0xE64, 1, 11u, 60u, (2u, 2u, 3u, 307u)
#define PPUTLIMPL_UINT_TRAIT_3683u DEC, 0xE63, 1, 11u, 60u, (29u, 127u)
#define PPUTLIMPL_UINT_TRAIT_3682u DEC, 0xE62, 1, 11u, 60u, (2u, 7u, 263u)
#define PPUTLIMPL_UINT_TRAIT_3681u DEC, 0xE61, 1, 11u, 60u, (3u, 3u, 409u)
#define PPUTLIMPL_UINT_TRAIT_3680u DEC, 0xE60, 1, 11u, 60u, (2u, 2u, 2u, 2u, 2u, 5u, 23u)
#define PPUTLIMPL_UINT_TRAIT_3679u DEC, 0xE5F, 1, 11u, 60u, (13u, 283u)
#define PPUTLIMPL_UINT_TRAIT_3678u DEC, 0xE5E, 1, 11u, 60u, (2u, 3u, 613u)
#define PPUTLIMPL_UINT_TRAIT_3677u DEC, 0xE5D, 1, 11u, 60u, ()
#define PPUTLIMPL_UINT_TRAIT_3676u DEC, 0xE5C, 1, 11u, 60u, (2u, 2u, 919u)
#define PPUTLIMPL_UINT_TRAIT_3675u DEC, 0xE5B, 1, 11u, 60u, (3u, 5u, 5u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_3674u DEC, 0xE5A, 1, 11u, 60u, (2u, 11u, 167u)
#define PPUTLIMPL_UINT_TRAIT_3673u DEC, 0xE59, 1, 11u, 60u, ()
#define PPUTLIMPL_UINT_TRAIT_3672u DEC, 0xE58, 1, 11u, 60u, (2u, 2u, 2u, 3u, 3u, 3u, 17u)
#define PPUTLIMPL_UINT_TRAIT_3671u DEC, 0xE57, 1, 11u, 60u, ()
#define PPUTLIMPL_UINT_TRAIT_3670u DEC, 0xE56, 1, 11u, 60u, (2u, 5u, 367u)
#define PPUTLIMPL_UINT_TRAIT_3669u DEC, 0xE55, 1, 11u, 60u, (3u, 1223u)
#define PPUTLIMPL_UINT_TRAIT_3668u DEC, 0xE54, 1, 11u, 60u, (2u, 2u, 7u, 131u)
#define PPUTLIMPL_UINT_TRAIT_3667u DEC, 0xE53, 1, 11u, 60u, (19u, 193u)
#define PPUTLIMPL_UINT_TRAIT_3666u DEC, 0xE52, 1, 11u, 60u, (2u, 3u, 13u, 47u)
#define PPUTLIMPL_UINT_TRAIT_3665u DEC, 0xE51, 1, 11u, 60u, (5u, 733u)
#define PPUTLIMPL_UINT_TRAIT_3664u DEC, 0xE50, 1, 11u, 60u, (2u, 2u, 2u, 2u, 229u)
#define PPUTLIMPL_UINT_TRAIT_3663u DEC, 0xE4F, 1, 11u, 60u, (3u, 3u, 11u, 37u)
#define PPUTLIMPL_UINT_TRAIT_3662u DEC, 0xE4E, 1, 11u, 60u, (2u, 1831u)
#define PPUTLIMPL_UINT_TRAIT_3661u DEC, 0xE4D, 1, 11u, 60u, (7u, 523u)
#define PPUTLIMPL_UINT_TRAIT_3660u DEC, 0xE4C, 1, 11u, 60u, (2u, 2u, 3u, 5u, 61u)
#define PPUTLIMPL_UINT_TRAIT_3659u DEC, 0xE4B, 1, 11u, 60u, ()
#define PPUTLIMPL_UINT_TRAIT_3658u DEC, 0xE4A, 1, 11u, 60u, (2u, 31u, 59u)
#define PPUTLIMPL_UINT_TRAIT_3657u DEC, 0xE49, 1, 11u, 60u, (3u, 23u, 53u)
#define PPUTLIMPL_UINT_TRAIT_3656u DEC, 0xE48, 1, 11u, 60u, (2u, 2u, 2u, 457u)
#define PPUTLIMPL_UINT_TRAIT_3655u DEC, 0xE47, 1, 11u, 60u, (5u, 17u, 43u)
#define PPUTLIMPL_UINT_TRAIT_3654u DEC, 0xE46, 1, 11u, 60u, (2u, 3u, 3u, 7u, 29u)
#define PPUTLIMPL_UINT_TRAIT_3653u DEC, 0xE45, 1, 11u, 60u, (13u, 281u)
#define PPUTLIMPL_UINT_TRAIT_3652u DEC, 0xE44, 1, 11u, 60u, (2u, 2u, 11u, 83u)
#define PPUTLIMPL_UINT_TRAIT_3651u DEC, 0xE43, 1, 11u, 60u, (3u, 1217u)
#define PPUTLIMPL_UINT_TRAIT_3650u DEC, 0xE42, 1, 11u, 60u, (2u, 5u, 5u, 73u)
#define PPUTLIMPL_UINT_TRAIT_3649u DEC, 0xE41, 1, 11u, 60u, (41u, 89u)
#define PPUTLIMPL_UINT_TRAIT_3648u DEC, 0xE40, 1, 11u, 60u, (2u, 2u, 2u, 2u, 2u, 2u, 3u, 19u)
#define PPUTLIMPL_UINT_TRAIT_3647u DEC, 0xE3F, 1, 11u, 60u, (7u, 521u)
#define PPUTLIMPL_UINT_TRAIT_3646u DEC, 0xE3E, 1, 11u, 60u, (2u, 1823u)
#define PPUTLIMPL_UINT_TRAIT_3645u DEC, 0xE3D, 1, 11u, 60u, (3u, 3u, 3u, 3u, 3u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_3644u DEC, 0xE3C, 1, 11u, 60u, (2u, 2u, 911u)
#define PPUTLIMPL_UINT_TRAIT_3643u DEC, 0xE3B, 1, 11u, 60u, ()
#define PPUTLIMPL_UINT_TRAIT_3642u DEC, 0xE3A, 1, 11u, 60u, (2u, 3u, 607u)
#define PPUTLIMPL_UINT_TRAIT_3641u DEC, 0xE39, 1, 11u, 60u, (11u, 331u)
#define PPUTLIMPL_UINT_TRAIT_3640u DEC, 0xE38, 1, 11u, 60u, (2u, 2u, 2u, 5u, 7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_3639u DEC, 0xE37, 1, 11u, 60u, (3u, 1213u)
#define PPUTLIMPL_UINT_TRAIT_3638u DEC, 0xE36, 1, 11u, 60u, (2u, 17u, 107u)
#define PPUTLIMPL_UINT_TRAIT_3637u DEC, 0xE35, 1, 11u, 60u, ()
#define PPUTLIMPL_UINT_TRAIT_3636u DEC, 0xE34, 1, 11u, 60u, (2u, 2u, 3u, 3u, 101u)
#define PPUTLIMPL_UINT_TRAIT_3635u DEC, 0xE33, 1, 11u, 60u, (5u, 727u)
#define PPUTLIMPL_UINT_TRAIT_3634u DEC, 0xE32, 1, 11u, 60u, (2u, 23u, 79u)
#define PPUTLIMPL_UINT_TRAIT_3633u DEC, 0xE31, 1, 11u, 60u, (3u, 7u, 173u)
#define PPUTLIMPL_UINT_TRAIT_3632u DEC, 0xE30, 1, 11u, 60u, (2u, 2u, 2u, 2u, 227u)
#define PPUTLIMPL_UINT_TRAIT_3631u DEC, 0xE2F, 1, 11u, 60u, ()
#define PPUTLIMPL_UINT_TRAIT_3630u DEC, 0xE2E, 1, 11u, 60u, (2u, 3u, 5u, 11u, 11u)
#define PPUTLIMPL_UINT_TRAIT_3629u DEC, 0xE2D, 1, 11u, 60u, (19u, 191u)
#define PPUTLIMPL_UINT_TRAIT_3628u DEC, 0xE2C, 1, 11u, 60u, (2u, 2u, 907u)
#define PPUTLIMPL_UINT_TRAIT_3627u DEC, 0xE2B, 1, 11u, 60u, (3u, 3u, 13u, 31u)
#define PPUTLIMPL_UINT_TRAIT_3626u DEC, 0xE2A, 1, 11u, 60u, (2u, 7u, 7u, 37u)
#define PPUTLIMPL_UINT_TRAIT_3625u DEC, 0xE29, 1, 11u, 60u, (5u, 5u, 5u, 29u)
#define PPUTLIMPL_UINT_TRAIT_3624u DEC, 0xE28, 1, 11u, 60u, (2u, 2u, 2u, 3u, 151u)
#define PPUTLIMPL_UINT_TRAIT_3623u DEC, 0xE27, 1, 11u, 60u, ()
#define PPUTLIMPL_UINT_TRAIT_3622u DEC, 0xE26, 1, 11u, 60u, (2u, 1811u)
#define PPUTLIMPL_UINT_TRAIT_3621u DEC, 0xE25, 1, 11u, 60u, (3u, 17u, 71u)
#define PPUTLIMPL_UINT_TRAIT_3620u DEC, 0xE24, 1, 11u, 60u, (2u, 2u, 5u, 181u)
#define PPUTLIMPL_UINT_TRAIT_3619u DEC, 0xE23, 1, 11u, 60u, (7u, 11u, 47u)
#define PPUTLIMPL_UINT_TRAIT_3618u DEC, 0xE22, 1, 11u, 60u, (2u, 3u, 3u, 3u, 67u)
#define PPUTLIMPL_UINT_TRAIT_3617u DEC, 0xE21, 1, 11u, 60u, ()
#define PPUTLIMPL_UINT_TRAIT_3616u DEC, 0xE20, 1, 11u, 60u, (2u, 2u, 2u, 2u, 2u, 113u)
#define PPUTLIMPL_UINT_TRAIT_3615u DEC, 0xE1F, 1, 11u, 60u, (3u, 5u, 241u)
#define PPUTLIMPL_UINT_TRAIT_3614u DEC, 0xE1E, 1, 11u, 60u, (2u, 13u, 139u)
#define PPUTLIMPL_UINT_TRAIT_3613u DEC, 0xE1D, 1, 11u, 60u, ()
#define PPUTLIMPL_UINT_TRAIT_3612u DEC, 0xE1C, 1, 11u, 60u, (2u, 2u, 3u, 7u, 43u)
#define PPUTLIMPL_UINT_TRAIT_3611u DEC, 0xE1B, 1, 11u, 60u, (23u, 157u)
#define PPUTLIMPL_UINT_TRAIT_3610u DEC, 0xE1A, 1, 11u, 60u, (2u, 5u, 19u, 19u)
#define PPUTLIMPL_UINT_TRAIT_3609u DEC, 0xE19, 1, 11u, 60u, (3u, 3u, 401u)
#define PPUTLIMPL_UINT_TRAIT_3608u DEC, 0xE18, 1, 11u, 60u, (2u, 2u, 2u, 11u, 41u)
#define PPUTLIMPL_UINT_TRAIT_3607u DEC, 0xE17, 1, 11u, 60u, ()
#define PPUTLIMPL_UINT_TRAIT_3606u DEC, 0xE16, 1, 11u, 60u, (2u, 3u, 601u)
#define PPUTLIMPL_UINT_TRAIT_3605u DEC, 0xE15, 1, 11u, 60u, (5u, 7u, 103u)
#define PPUTLIMPL_UINT_TRAIT_3604u DEC, 0xE14, 1, 11u, 60u, (2u, 2u, 17u, 53u)
#define PPUTLIMPL_UINT_TRAIT_3603u DEC, 0xE13, 1, 11u, 60u, (3u, 1201u)
#define PPUTLIMPL_UINT_TRAIT_3602u DEC, 0xE12, 1, 11u, 60u, (2u, 1801u)
#define PPUTLIMPL_UINT_TRAIT_3601u DEC, 0xE11, 1, 11u, 60u, (13u, 277u)
#define PPUTLIMPL_UINT_TRAIT_3600u DEC, 0xE10, 1, 11u, 60u, (2u, 2u, 2u, 2u, 3u, 3u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_3599u DEC, 0xE0F, 1, 11u, 59u, (59u, 61u)
#define PPUTLIMPL_UINT_TRAIT_3598u DEC, 0xE0E, 1, 11u, 59u, (2u, 7u, 257u)
#define PPUTLIMPL_UINT_TRAIT_3597u DEC, 0xE0D, 1, 11u, 59u, (3u, 11u, 109u)
#define PPUTLIMPL_UINT_TRAIT_3596u DEC, 0xE0C, 1, 11u, 59u, (2u, 2u, 29u, 31u)
#define PPUTLIMPL_UINT_TRAIT_3595u DEC, 0xE0B, 1, 11u, 59u, (5u, 719u)
#define PPUTLIMPL_UINT_TRAIT_3594u DEC, 0xE0A, 1, 11u, 59u, (2u, 3u, 599u)
#define PPUTLIMPL_UINT_TRAIT_3593u DEC, 0xE09, 1, 11u, 59u, ()
#define PPUTLIMPL_UINT_TRAIT_3592u DEC, 0xE08, 1, 11u, 59u, (2u, 2u, 2u, 449u)
#define PPUTLIMPL_UINT_TRAIT_3591u DEC, 0xE07, 1, 11u, 59u, (3u, 3u, 3u, 7u, 19u)
#define PPUTLIMPL_UINT_TRAIT_3590u DEC, 0xE06, 1, 11u, 59u, (2u, 5u, 359u)
#define PPUTLIMPL_UINT_TRAIT_3589u DEC, 0xE05, 1, 11u, 59u, (37u, 97u)
#define PPUTLIMPL_UINT_TRAIT_3588u DEC, 0xE04, 1, 11u, 59u, (2u, 2u, 3u, 13u, 23u)
#define PPUTLIMPL_UINT_TRAIT_3587u DEC, 0xE03, 1, 11u, 59u, (17u, 211u)
#define PPUTLIMPL_UINT_TRAIT_3586u DEC, 0xE02, 1, 11u, 59u, (2u, 11u, 163u)
#define PPUTLIMPL_UINT_TRAIT_3585u DEC, 0xE01, 1, 11u, 59u, (3u, 5u, 239u)
#define PPUTLIMPL_UINT_TRAIT_3584u DEC, 0xE00, 1, 11u, 59u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 7u)
#define PPUTLIMPL_UINT_TRAIT_3583u DEC, 0xDFF, 1, 11u, 59u, ()
#define PPUTLIMPL_UINT_TRAIT_3582u DEC, 0xDFE, 1, 11u, 59u, (2u, 3u, 3u, 199u)
#define PPUTLIMPL_UINT_TRAIT_3581u DEC, 0xDFD, 1, 11u, 59u, ()
#define PPUTLIMPL_UINT_TRAIT_3580u DEC, 0xDFC, 1, 11u, 59u, (2u, 2u, 5u, 179u)
#define PPUTLIMPL_UINT_TRAIT_3579u DEC, 0xDFB, 1, 11u, 59u, (3u, 1193u)
#define PPUTLIMPL_UINT_TRAIT_3578u DEC, 0xDFA, 1, 11u, 59u, (2u, 1789u)
#define PPUTLIMPL_UINT_TRAIT_3577u DEC, 0xDF9, 1, 11u, 59u, (7u, 7u, 73u)
#define PPUTLIMPL_UINT_TRAIT_3576u DEC, 0xDF8, 1, 11u, 59u, (2u, 2u, 2u, 3u, 149u)
#define PPUTLIMPL_UINT_TRAIT_3575u DEC, 0xDF7, 1, 11u, 59u, (5u, 5u, 11u, 13u)
#define PPUTLIMPL_UINT_TRAIT_3574u DEC, 0xDF6, 1, 11u, 59u, (2u, 1787u)
#define PPUTLIMPL_UINT_TRAIT_3573u DEC, 0xDF5, 1, 11u, 59u, (3u, 3u, 397u)
#define PPUTLIMPL_UINT_TRAIT_3572u DEC, 0xDF4, 1, 11u, 59u, (2u, 2u, 19u, 47u)
#define PPUTLIMPL_UINT_TRAIT_3571u DEC, 0xDF3, 1, 11u, 59u, ()
#define PPUTLIMPL_UINT_TRAIT_3570u DEC, 0xDF2, 1, 11u, 59u, (2u, 3u, 5u, 7u, 17u)
#define PPUTLIMPL_UINT_TRAIT_3569u DEC, 0xDF1, 1, 11u, 59u, (43u, 83u)
#define PPUTLIMPL_UINT_TRAIT_3568u DEC, 0xDF0, 1, 11u, 59u, (2u, 2u, 2u, 2u, 223u)
#define PPUTLIMPL_UINT_TRAIT_3567u DEC, 0xDEF, 1, 11u, 59u, (3u, 29u, 41u)
#define PPUTLIMPL_UINT_TRAIT_3566u DEC, 0xDEE, 1, 11u, 59u, (2u, 1783u)
#define PPUTLIMPL_UINT_TRAIT_3565u DEC, 0xDED, 1, 11u, 59u, (5u, 23u, 31u)
#define PPUTLIMPL_UINT_TRAIT_3564u DEC, 0xDEC, 1, 11u, 59u, (2u, 2u, 3u, 3u, 3u, 3u, 11u)
#define PPUTLIMPL_UINT_TRAIT_3563u DEC, 0xDEB, 1, 11u, 59u, (7u, 509u)
#define PPUTLIMPL_UINT_TRAIT_3562u DEC, 0xDEA, 1, 11u, 59u, (2u, 13u, 137u)
#define PPUTLIMPL_UINT_TRAIT_3561u DEC, 0xDE9, 1, 11u, 59u, (3u, 1187u)
#define PPUTLIMPL_UINT_TRAIT_3560u DEC, 0xDE8, 1, 11u, 59u, (2u, 2u, 2u, 5u, 89u)
#define PPUTLIMPL_UINT_TRAIT_3559u DEC, 0xDE7, 1, 11u, 59u, ()
#define PPUTLIMPL_UINT_TRAIT_3558u DEC, 0xDE6, 1, 11u, 59u, (2u, 3u, 593u)
#define PPUTLIMPL_UINT_TRAIT_3557u DEC, 0xDE5, 1, 11u, 59u, ()
#define PPUTLIMPL_UINT_TRAIT_3556u DEC, 0xDE4, 1, 11u, 59u, (2u, 2u, 7u, 127u)
#define PPUTLIMPL_UINT_TRAIT_3555u DEC, 0xDE3, 1, 11u, 59u, (3u, 3u, 5u, 79u)
#define PPUTLIMPL_UINT_TRAIT_3554u DEC, 0xDE2, 1, 11u, 59u, (2u, 1777u)
#define PPUTLIMPL_UINT_TRAIT_3553u DEC, 0xDE1, 1, 11u, 59u, (11u, 17u, 19u)
#define PPUTLIMPL_UINT_TRAIT_3552u DEC, 0xDE0, 1, 11u, 59u, (2u, 2u, 2u, 2u, 2u, 3u, 37u)
#define PPUTLIMPL_UINT_TRAIT_3551u DEC, 0xDDF, 1, 11u, 59u, (53u, 67u)
#define PPUTLIMPL_UINT_TRAIT_3550u DEC, 0xDDE, 1, 11u, 59u, (2u, 5u, 5u, 71u)
#define PPUTLIMPL_UINT_TRAIT_3549u DEC, 0xDDD, 1, 11u, 59u, (3u, 7u, 13u, 13u)
#define PPUTLIMPL_UINT_TRAIT_3548u DEC, 0xDDC, 1, 11u, 59u, (2u, 2u, 887u)
#define PPUTLIMPL_UINT_TRAIT_3547u DEC, 0xDDB, 1, 11u, 59u, ()
#define PPUTLIMPL_UINT_TRAIT_3546u DEC, 0xDDA, 1, 11u, 59u, (2u, 3u, 3u, 197u)
#define PPUTLIMPL_UINT_TRAIT_3545u DEC, 0xDD9, 1, 11u, 59u, (5u, 709u)
#define PPUTLIMPL_UINT_TRAIT_3544u DEC, 0xDD8, 1, 11u, 59u, (2u, 2u, 2u, 443u)
#define PPUTLIMPL_UINT_TRAIT_3543u DEC, 0xDD7, 1, 11u, 59u, (3u, 1181u)
#define PPUTLIMPL_UINT_TRAIT_3542u DEC, 0xDD6, 1, 11u, 59u, (2u, 7u, 11u, 23u)
#define PPUTLIMPL_UINT_TRAIT_3541u DEC, 0xDD5, 1, 11u, 59u, ()
#define PPUTLIMPL_UINT_TRAIT_3540u DEC, 0xDD4, 1, 11u, 59u, (2u, 2u, 3u, 5u, 59u)
#define PPUTLIMPL_UINT_TRAIT_3539u DEC, 0xDD3, 1, 11u, 59u, ()
#define PPUTLIMPL_UINT_TRAIT_3538u DEC, 0xDD2, 1, 11u, 59u, (2u, 29u, 61u)
#define PPUTLIMPL_UINT_TRAIT_3537u DEC, 0xDD1, 1, 11u, 59u, (3u, 3u, 3u, 131u)
#define PPUTLIMPL_UINT_TRAIT_3536u DEC, 0xDD0, 1, 11u, 59u, (2u, 2u, 2u, 2u, 13u, 17u)
#define PPUTLIMPL_UINT_TRAIT_3535u DEC, 0xDCF, 1, 11u, 59u, (5u, 7u, 101u)
#define PPUTLIMPL_UINT_TRAIT_3534u DEC, 0xDCE, 1, 11u, 59u, (2u, 3u, 19u, 31u)
#define PPUTLIMPL_UINT_TRAIT_3533u DEC, 0xDCD, 1, 11u, 59u, ()
#define PPUTLIMPL_UINT_TRAIT_3532u DEC, 0xDCC, 1, 11u, 59u, (2u, 2u, 883u)
#define PPUTLIMPL_UINT_TRAIT_3531u DEC, 0xDCB, 1, 11u, 59u, (3u, 11u, 107u)
#define PPUTLIMPL_UINT_TRAIT_3530u DEC, 0xDCA, 1, 11u, 59u, (2u, 5u, 353u)
#define PPUTLIMPL_UINT_TRAIT_3529u DEC, 0xDC9, 1, 11u, 59u, ()
#define PPUTLIMPL_UINT_TRAIT_3528u DEC, 0xDC8, 1, 11u, 59u, (2u, 2u, 2u, 3u, 3u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_3527u DEC, 0xDC7, 1, 11u, 59u, ()
#define PPUTLIMPL_UINT_TRAIT_3526u DEC, 0xDC6, 1, 11u, 59u, (2u, 41u, 43u)
#define PPUTLIMPL_UINT_TRAIT_3525u DEC, 0xDC5, 1, 11u, 59u, (3u, 5u, 5u, 47u)
#define PPUTLIMPL_UINT_TRAIT_3524u DEC, 0xDC4, 1, 11u, 59u, (2u, 2u, 881u)
#define PPUTLIMPL_UINT_TRAIT_3523u DEC, 0xDC3, 1, 11u, 59u, (13u, 271u)
#define PPUTLIMPL_UINT_TRAIT_3522u DEC, 0xDC2, 1, 11u, 59u, (2u, 3u, 587u)
#define PPUTLIMPL_UINT_TRAIT_3521u DEC, 0xDC1, 1, 11u, 59u, (7u, 503u)
#define PPUTLIMPL_UINT_TRAIT_3520u DEC, 0xDC0, 1, 11u, 59u, (2u, 2u, 2u, 2u, 2u, 2u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_3519u DEC, 0xDBF, 1, 11u, 59u, (3u, 3u, 17u, 23u)
#define PPUTLIMPL_UINT_TRAIT_3518u DEC, 0xDBE, 1, 11u, 59u, (2u, 1759u)
#define PPUTLIMPL_UINT_TRAIT_3517u DEC, 0xDBD, 1, 11u, 59u, ()
#define PPUTLIMPL_UINT_TRAIT_3516u DEC, 0xDBC, 1, 11u, 59u, (2u, 2u, 3u, 293u)
#define PPUTLIMPL_UINT_TRAIT_3515u DEC, 0xDBB, 1, 11u, 59u, (5u, 19u, 37u)
#define PPUTLIMPL_UINT_TRAIT_3514u DEC, 0xDBA, 1, 11u, 59u, (2u, 7u, 251u)
#define PPUTLIMPL_UINT_TRAIT_3513u DEC, 0xDB9, 1, 11u, 59u, (3u, 1171u)
#define PPUTLIMPL_UINT_TRAIT_3512u DEC, 0xDB8, 1, 11u, 59u, (2u, 2u, 2u, 439u)
#define PPUTLIMPL_UINT_TRAIT_3511u DEC, 0xDB7, 1, 11u, 59u, ()
#define PPUTLIMPL_UINT_TRAIT_3510u DEC, 0xDB6, 1, 11u, 59u, (2u, 3u, 3u, 3u, 5u, 13u)
#define PPUTLIMPL_UINT_TRAIT_3509u DEC, 0xDB5, 1, 11u, 59u, (11u, 11u, 29u)
#define PPUTLIMPL_UINT_TRAIT_3508u DEC, 0xDB4, 1, 11u, 59u, (2u, 2u, 877u)
#define PPUTLIMPL_UINT_TRAIT_3507u DEC, 0xDB3, 1, 11u, 59u, (3u, 7u, 167u)
#define PPUTLIMPL_UINT_TRAIT_3506u DEC, 0xDB2, 1, 11u, 59u, (2u, 1753u)
#define PPUTLIMPL_UINT_TRAIT_3505u DEC, 0xDB1, 1, 11u, 59u, (5u, 701u)
#define PPUTLIMPL_UINT_TRAIT_3504u DEC, 0xDB0, 1, 11u, 59u, (2u, 2u, 2u, 2u, 3u, 73u)
#define PPUTLIMPL_UINT_TRAIT_3503u DEC, 0xDAF, 1, 11u, 59u, (31u, 113u)
#define PPUTLIMPL_UINT_TRAIT_3502u DEC, 0xDAE, 1, 11u, 59u, (2u, 17u, 103u)
#define PPUTLIMPL_UINT_TRAIT_3501u DEC, 0xDAD, 1, 11u, 59u, (3u, 3u, 389u)
#define PPUTLIMPL_UINT_TRAIT_3500u DEC, 0xDAC, 1, 11u, 59u, (2u, 2u, 5u, 5u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_3499u DEC, 0xDAB, 1, 11u, 59u, ()
#define PPUTLIMPL_UINT_TRAIT_3498u DEC, 0xDAA, 1, 11u, 59u, (2u, 3u, 11u, 53u)
#define PPUTLIMPL_UINT_TRAIT_3497u DEC, 0xDA9, 1, 11u, 59u, (13u, 269u)
#define PPUTLIMPL_UINT_TRAIT_3496u DEC, 0xDA8, 1, 11u, 59u, (2u, 2u, 2u, 19u, 23u)
#define PPUTLIMPL_UINT_TRAIT_3495u DEC, 0xDA7, 1, 11u, 59u, (3u, 5u, 233u)
#define PPUTLIMPL_UINT_TRAIT_3494u DEC, 0xDA6, 1, 11u, 59u, (2u, 1747u)
#define PPUTLIMPL_UINT_TRAIT_3493u DEC, 0xDA5, 1, 11u, 59u, (7u, 499u)
#define PPUTLIMPL_UINT_TRAIT_3492u DEC, 0xDA4, 1, 11u, 59u, (2u, 2u, 3u, 3u, 97u)
#define PPUTLIMPL_UINT_TRAIT_3491u DEC, 0xDA3, 1, 11u, 59u, ()
#define PPUTLIMPL_UINT_TRAIT_3490u DEC, 0xDA2, 1, 11u, 59u, (2u, 5u, 349u)
#define PPUTLIMPL_UINT_TRAIT_3489u DEC, 0xDA1, 1, 11u, 59u, (3u, 1163u)
#define PPUTLIMPL_UINT_TRAIT_3488u DEC, 0xDA0, 1, 11u, 59u, (2u, 2u, 2u, 2u, 2u, 109u)
#define PPUTLIMPL_UINT_TRAIT_3487u DEC, 0xD9F, 1, 11u, 59u, (11u, 317u)
#define PPUTLIMPL_UINT_TRAIT_3486u DEC, 0xD9E, 1, 11u, 59u, (2u, 3u, 7u, 83u)
#define PPUTLIMPL_UINT_TRAIT_3485u DEC, 0xD9D, 1, 11u, 59u, (5u, 17u, 41u)
#define PPUTLIMPL_UINT_TRAIT_3484u DEC, 0xD9C, 1, 11u, 59u, (2u, 2u, 13u, 67u)
#define PPUTLIMPL_UINT_TRAIT_3483u DEC, 0xD9B, 1, 11u, 59u, (3u, 3u, 3u, 3u, 43u)
#define PPUTLIMPL_UINT_TRAIT_3482u DEC, 0xD9A, 1, 11u, 59u, (2u, 1741u)
#define PPUTLIMPL_UINT_TRAIT_3481u DEC, 0xD99, 1, 11u, 59u, (59u, 59u)
#define PPUTLIMPL_UINT_TRAIT_3480u DEC, 0xD98, 1, 11u, 58u, (2u, 2u, 2u, 3u, 5u, 29u)
#define PPUTLIMPL_UINT_TRAIT_3479u DEC, 0xD97, 1, 11u, 58u, (7u, 7u, 71u)
#define PPUTLIMPL_UINT_TRAIT_3478u DEC, 0xD96, 1, 11u, 58u, (2u, 37u, 47u)
#define PPUTLIMPL_UINT_TRAIT_3477u DEC, 0xD95, 1, 11u, 58u, (3u, 19u, 61u)
#define PPUTLIMPL_UINT_TRAIT_3476u DEC, 0xD94, 1, 11u, 58u, (2u, 2u, 11u, 79u)
#define PPUTLIMPL_UINT_TRAIT_3475u DEC, 0xD93, 1, 11u, 58u, (5u, 5u, 139u)
#define PPUTLIMPL_UINT_TRAIT_3474u DEC, 0xD92, 1, 11u, 58u, (2u, 3u, 3u, 193u)
#define PPUTLIMPL_UINT_TRAIT_3473u DEC, 0xD91, 1, 11u, 58u, (23u, 151u)
#define PPUTLIMPL_UINT_TRAIT_3472u DEC, 0xD90, 1, 11u, 58u, (2u, 2u, 2u, 2u, 7u, 31u)
#define PPUTLIMPL_UINT_TRAIT_3471u DEC, 0xD8F, 1, 11u, 58u, (3u, 13u, 89u)
#define PPUTLIMPL_UINT_TRAIT_3470u DEC, 0xD8E, 1, 11u, 58u, (2u, 5u, 347u)
#define PPUTLIMPL_UINT_TRAIT_3469u DEC, 0xD8D, 1, 11u, 58u, ()
#define PPUTLIMPL_UINT_TRAIT_3468u DEC, 0xD8C, 1, 11u, 58u, (2u, 2u, 3u, 17u, 17u)
#define PPUTLIMPL_UINT_TRAIT_3467u DEC, 0xD8B, 1, 11u, 58u, ()
#define PPUTLIMPL_UINT_TRAIT_3466u DEC, 0xD8A, 1, 11u, 58u, (2u, 1733u)
#define PPUTLIMPL_UINT_TRAIT_3465u DEC, 0xD89, 1, 11u, 58u, (3u, 3u, 5u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_3464u DEC, 0xD88, 1, 11u, 58u, (2u, 2u, 2u, 433u)
#define PPUTLIMPL_UINT_TRAIT_3463u DEC, 0xD87, 1, 11u, 58u, ()
#define PPUTLIMPL_UINT_TRAIT_3462u DEC, 0xD86, 1, 11u, 58u, (2u, 3u, 577u)
#define PPUTLIMPL_UINT_TRAIT_3461u DEC, 0xD85, 1, 11u, 58u, ()
#define PPUTLIMPL_UINT_TRAIT_3460u DEC, 0xD84, 1, 11u, 58u, (2u, 2u, 5u, 173u)
#define PPUTLIMPL_UINT_TRAIT_3459u DEC, 0xD83, 1, 11u, 58u, (3u, 1153u)
#define PPUTLIMPL_UINT_TRAIT_3458u DEC, 0xD82, 1, 11u, 58u, (2u, 7u, 13u, 19u)
#define PPUTLIMPL_UINT_TRAIT_3457u DEC, 0xD81, 1, 11u, 58u, ()
#define PPUTLIMPL_UINT_TRAIT_3456u DEC, 0xD80, 1, 11u, 58u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_3455u DEC, 0xD7F, 1, 11u, 58u, (5u, 691u)
#define PPUTLIMPL_UINT_TRAIT_3454u DEC, 0xD7E, 1, 11u, 58u, (2u, 11u, 157u)
#define PPUTLIMPL_UINT_TRAIT_3453u DEC, 0xD7D, 1, 11u, 58u, (3u, 1151u)
#define PPUTLIMPL_UINT_TRAIT_3452u DEC, 0xD7C, 1, 11u, 58u, (2u, 2u, 863u)
#define PPUTLIMPL_UINT_TRAIT_3451u DEC, 0xD7B, 1, 11u, 58u, (7u, 17u, 29u)
#define PPUTLIMPL_UINT_TRAIT_3450u DEC, 0xD7A, 1, 11u, 58u, (2u, 3u, 5u, 5u, 23u)
#define PPUTLIMPL_UINT_TRAIT_3449u DEC, 0xD79, 1, 11u, 58u, ()
#define PPUTLIMPL_UINT_TRAIT_3448u DEC, 0xD78, 1, 11u, 58u, (2u, 2u, 2u, 431u)
#define PPUTLIMPL_UINT_TRAIT_3447u DEC, 0xD77, 1, 11u, 58u, (3u, 3u, 383u)
#define PPUTLIMPL_UINT_TRAIT_3446u DEC, 0xD76, 1, 11u, 58u, (2u, 1723u)
#define PPUTLIMPL_UINT_TRAIT_3445u DEC, 0xD75, 1, 11u, 58u, (5u, 13u, 53u)
#define PPUTLIMPL_UINT_TRAIT_3444u DEC, 0xD74, 1, 11u, 58u, (2u, 2u, 3u, 7u, 41u)
#define PPUTLIMPL_UINT_TRAIT_3443u DEC, 0xD73, 1, 11u, 58u, (11u, 313u)
#define PPUTLIMPL_UINT_TRAIT_3442u DEC, 0xD72, 1, 11u, 58u, (2u, 1721u)
#define PPUTLIMPL_UINT_TRAIT_3441u DEC, 0xD71, 1, 11u, 58u, (3u, 31u, 37u)
#define PPUTLIMPL_UINT_TRAIT_3440u DEC, 0xD70, 1, 11u, 58u, (2u, 2u, 2u, 2u, 5u, 43u)
#define PPUTLIMPL_UINT_TRAIT_3439u DEC, 0xD6F, 1, 11u, 58u, (19u, 181u)
#define PPUTLIMPL_UINT_TRAIT_3438u DEC, 0xD6E, 1, 11u, 58u, (2u, 3u, 3u, 191u)
#define PPUTLIMPL_UINT_TRAIT_3437u DEC, 0xD6D, 1, 11u, 58u, (7u, 491u)
#define PPUTLIMPL_UINT_TRAIT_3436u DEC, 0xD6C, 1, 11u, 58u, (2u, 2u, 859u)
#define PPUTLIMPL_UINT_TRAIT_3435u DEC, 0xD6B, 1, 11u, 58u, (3u, 5u, 229u)
#define PPUTLIMPL_UINT_TRAIT_3434u DEC, 0xD6A, 1, 11u, 58u, (2u, 17u, 101u)
#define PPUTLIMPL_UINT_TRAIT_3433u DEC, 0xD69, 1, 11u, 58u, ()
#define PPUTLIMPL_UINT_TRAIT_3432u DEC, 0xD68, 1, 11u, 58u, (2u, 2u, 2u, 3u, 11u, 13u)
#define PPUTLIMPL_UINT_TRAIT_3431u DEC, 0xD67, 1, 11u, 58u, (47u, 73u)
#define PPUTLIMPL_UINT_TRAIT_3430u DEC, 0xD66, 1, 11u, 58u, (2u, 5u, 7u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_3429u DEC, 0xD65, 1, 11u, 58u, (3u, 3u, 3u, 127u)
#define PPUTLIMPL_UINT_TRAIT_3428u DEC, 0xD64, 1, 11u, 58u, (2u, 2u, 857u)
#define PPUTLIMPL_UINT_TRAIT_3427u DEC, 0xD63, 1, 11u, 58u, (23u, 149u)
#define PPUTLIMPL_UINT_TRAIT_3426u DEC, 0xD62, 1, 11u, 58u, (2u, 3u, 571u)
#define PPUTLIMPL_UINT_TRAIT_3425u DEC, 0xD61, 1, 11u, 58u, (5u, 5u, 137u)
#define PPUTLIMPL_UINT_TRAIT_3424u DEC, 0xD60, 1, 11u, 58u, (2u, 2u, 2u, 2u, 2u, 107u)
#define PPUTLIMPL_UINT_TRAIT_3423u DEC, 0xD5F, 1, 11u, 58u, (3u, 7u, 163u)
#define PPUTLIMPL_UINT_TRAIT_3422u DEC, 0xD5E, 1, 11u, 58u, (2u, 29u, 59u)
#define PPUTLIMPL_UINT_TRAIT_3421u DEC, 0xD5D, 1, 11u, 58u, (11u, 311u)
#define PPUTLIMPL_UINT_TRAIT_3420u DEC, 0xD5C, 1, 11u, 58u, (2u, 2u, 3u, 3u, 5u, 19u)
#define PPUTLIMPL_UINT_TRAIT_3419u DEC, 0xD5B, 1, 11u, 58u, (13u, 263u)
#define PPUTLIMPL_UINT_TRAIT_3418u DEC, 0xD5A, 1, 11u, 58u, (2u, 1709u)
#define PPUTLIMPL_UINT_TRAIT_3417u DEC, 0xD59, 1, 11u, 58u, (3u, 17u, 67u)
#define PPUTLIMPL_UINT_TRAIT_3416u DEC, 0xD58, 1, 11u, 58u, (2u, 2u, 2u, 7u, 61u)
#define PPUTLIMPL_UINT_TRAIT_3415u DEC, 0xD57, 1, 11u, 58u, (5u, 683u)
#define PPUTLIMPL_UINT_TRAIT_3414u DEC, 0xD56, 1, 11u, 58u, (2u, 3u, 569u)
#define PPUTLIMPL_UINT_TRAIT_3413u DEC, 0xD55, 1, 11u, 58u, ()
#define PPUTLIMPL_UINT_TRAIT_3412u DEC, 0xD54, 1, 11u, 58u, (2u, 2u, 853u)
#define PPUTLIMPL_UINT_TRAIT_3411u DEC, 0xD53, 1, 11u, 58u, (3u, 3u, 379u)
#define PPUTLIMPL_UINT_TRAIT_3410u DEC, 0xD52, 1, 11u, 58u, (2u, 5u, 11u, 31u)
#define PPUTLIMPL_UINT_TRAIT_3409u DEC, 0xD51, 1, 11u, 58u, (7u, 487u)
#define PPUTLIMPL_UINT_TRAIT_3408u DEC, 0xD50, 1, 11u, 58u, (2u, 2u, 2u, 2u, 3u, 71u)
#define PPUTLIMPL_UINT_TRAIT_3407u DEC, 0xD4F, 1, 11u, 58u, ()
#define PPUTLIMPL_UINT_TRAIT_3406u DEC, 0xD4E, 1, 11u, 58u, (2u, 13u, 131u)
#define PPUTLIMPL_UINT_TRAIT_3405u DEC, 0xD4D, 1, 11u, 58u, (3u, 5u, 227u)
#define PPUTLIMPL_UINT_TRAIT_3404u DEC, 0xD4C, 1, 11u, 58u, (2u, 2u, 23u, 37u)
#define PPUTLIMPL_UINT_TRAIT_3403u DEC, 0xD4B, 1, 11u, 58u, (41u, 83u)
#define PPUTLIMPL_UINT_TRAIT_3402u DEC, 0xD4A, 1, 11u, 58u, (2u, 3u, 3u, 3u, 3u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_3401u DEC, 0xD49, 1, 11u, 58u, (19u, 179u)
#define PPUTLIMPL_UINT_TRAIT_3400u DEC, 0xD48, 1, 11u, 58u, (2u, 2u, 2u, 5u, 5u, 17u)
#define PPUTLIMPL_UINT_TRAIT_3399u DEC, 0xD47, 1, 11u, 58u, (3u, 11u, 103u)
#define PPUTLIMPL_UINT_TRAIT_3398u DEC, 0xD46, 1, 11u, 58u, (2u, 1699u)
#define PPUTLIMPL_UINT_TRAIT_3397u DEC, 0xD45, 1, 11u, 58u, (43u, 79u)
#define PPUTLIMPL_UINT_TRAIT_3396u DEC, 0xD44, 1, 11u, 58u, (2u, 2u, 3u, 283u)
#define PPUTLIMPL_UINT_TRAIT_3395u DEC, 0xD43, 1, 11u, 58u, (5u, 7u, 97u)
#define PPUTLIMPL_UINT_TRAIT_3394u DEC, 0xD42, 1, 11u, 58u, (2u, 1697u)
#define PPUTLIMPL_UINT_TRAIT_3393u DEC, 0xD41, 1, 11u, 58u, (3u, 3u, 13u, 29u)
#define PPUTLIMPL_UINT_TRAIT_3392u DEC, 0xD40, 1, 11u, 58u, (2u, 2u, 2u, 2u, 2u, 2u, 53u)
#define PPUTLIMPL_UINT_TRAIT_3391u DEC, 0xD3F, 1, 11u, 58u, ()
#define PPUTLIMPL_UINT_TRAIT_3390u DEC, 0xD3E, 1, 11u, 58u, (2u, 3u, 5u, 113u)
#define PPUTLIMPL_UINT_TRAIT_3389u DEC, 0xD3D, 1, 11u, 58u, ()
#define PPUTLIMPL_UINT_TRAIT_3388u DEC, 0xD3C, 1, 11u, 58u, (2u, 2u, 7u, 11u, 11u)
#define PPUTLIMPL_UINT_TRAIT_3387u DEC, 0xD3B, 1, 11u, 58u, (3u, 1129u)
#define PPUTLIMPL_UINT_TRAIT_3386u DEC, 0xD3A, 1, 11u, 58u, (2u, 1693u)
#define PPUTLIMPL_UINT_TRAIT_3385u DEC, 0xD39, 1, 11u, 58u, (5u, 677u)
#define PPUTLIMPL_UINT_TRAIT_3384u DEC, 0xD38, 1, 11u, 58u, (2u, 2u, 2u, 3u, 3u, 47u)
#define PPUTLIMPL_UINT_TRAIT_3383u DEC, 0xD37, 1, 11u, 58u, (17u, 199u)
#define PPUTLIMPL_UINT_TRAIT_3382u DEC, 0xD36, 1, 11u, 58u, (2u, 19u, 89u)
#define PPUTLIMPL_UINT_TRAIT_3381u DEC, 0xD35, 1, 11u, 58u, (3u, 7u, 7u, 23u)
#define PPUTLIMPL_UINT_TRAIT_3380u DEC, 0xD34, 1, 11u, 58u, (2u, 2u, 5u, 13u, 13u)
#define PPUTLIMPL_UINT_TRAIT_3379u DEC, 0xD33, 1, 11u, 58u, (31u, 109u)
#define PPUTLIMPL_UINT_TRAIT_3378u DEC, 0xD32, 1, 11u, 58u, (2u, 3u, 563u)
#define PPUTLIMPL_UINT_TRAIT_3377u DEC, 0xD31, 1, 11u, 58u, (11u, 307u)
#define PPUTLIMPL_UINT_TRAIT_3376u DEC, 0xD30, 1, 11u, 58u, (2u, 2u, 2u, 2u, 211u)
#define PPUTLIMPL_UINT_TRAIT_3375u DEC, 0xD2F, 1, 11u, 58u, (3u, 3u, 3u, 5u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_3374u DEC, 0xD2E, 1, 11u, 58u, (2u, 7u, 241u)
#define PPUTLIMPL_UINT_TRAIT_3373u DEC, 0xD2D, 1, 11u, 58u, ()
#define PPUTLIMPL_UINT_TRAIT_3372u DEC, 0xD2C, 1, 11u, 58u, (2u, 2u, 3u, 281u)
#define PPUTLIMPL_UINT_TRAIT_3371u DEC, 0xD2B, 1, 11u, 58u, ()
#define PPUTLIMPL_UINT_TRAIT_3370u DEC, 0xD2A, 1, 11u, 58u, (2u, 5u, 337u)
#define PPUTLIMPL_UINT_TRAIT_3369u DEC, 0xD29, 1, 11u, 58u, (3u, 1123u)
#define PPUTLIMPL_UINT_TRAIT_3368u DEC, 0xD28, 1, 11u, 58u, (2u, 2u, 2u, 421u)
#define PPUTLIMPL_UINT_TRAIT_3367u DEC, 0xD27, 1, 11u, 58u, (7u, 13u, 37u)
#define PPUTLIMPL_UINT_TRAIT_3366u DEC, 0xD26, 1, 11u, 58u, (2u, 3u, 3u, 11u, 17u)
#define PPUTLIMPL_UINT_TRAIT_3365u DEC, 0xD25, 1, 11u, 58u, (5u, 673u)
#define PPUTLIMPL_UINT_TRAIT_3364u DEC, 0xD24, 1, 11u, 58u, (2u, 2u, 29u, 29u)
#define PPUTLIMPL_UINT_TRAIT_3363u DEC, 0xD23, 1, 11u, 57u, (3u, 19u, 59u)
#define PPUTLIMPL_UINT_TRAIT_3362u DEC, 0xD22, 1, 11u, 57u, (2u, 41u, 41u)
#define PPUTLIMPL_UINT_TRAIT_3361u DEC, 0xD21, 1, 11u, 57u, ()
#define PPUTLIMPL_UINT_TRAIT_3360u DEC, 0xD20, 1, 11u, 57u, (2u, 2u, 2u, 2u, 2u, 3u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_3359u DEC, 0xD1F, 1, 11u, 57u, ()
#define PPUTLIMPL_UINT_TRAIT_3358u DEC, 0xD1E, 1, 11u, 57u, (2u, 23u, 73u)
#define PPUTLIMPL_UINT_TRAIT_3357u DEC, 0xD1D, 1, 11u, 57u, (3u, 3u, 373u)
#define PPUTLIMPL_UINT_TRAIT_3356u DEC, 0xD1C, 1, 11u, 57u, (2u, 2u, 839u)
#define PPUTLIMPL_UINT_TRAIT_3355u DEC, 0xD1B, 1, 11u, 57u, (5u, 11u, 61u)
#define PPUTLIMPL_UINT_TRAIT_3354u DEC, 0xD1A, 1, 11u, 57u, (2u, 3u, 13u, 43u)
#define PPUTLIMPL_UINT_TRAIT_3353u DEC, 0xD19, 1, 11u, 57u, (7u, 479u)
#define PPUTLIMPL_UINT_TRAIT_3352u DEC, 0xD18, 1, 11u, 57u, (2u, 2u, 2u, 419u)
#define PPUTLIMPL_UINT_TRAIT_3351u DEC, 0xD17, 1, 11u, 57u, (3u, 1117u)
#define PPUTLIMPL_UINT_TRAIT_3350u DEC, 0xD16, 1, 11u, 57u, (2u, 5u, 5u, 67u)
#define PPUTLIMPL_UINT_TRAIT_3349u DEC, 0xD15, 1, 11u, 57u, (17u, 197u)
#define PPUTLIMPL_UINT_TRAIT_3348u DEC, 0xD14, 1, 11u, 57u, (2u, 2u, 3u, 3u, 3u, 31u)
#define PPUTLIMPL_UINT_TRAIT_3347u DEC, 0xD13, 1, 11u, 57u, ()
#define PPUTLIMPL_UINT_TRAIT_3346u DEC, 0xD12, 1, 11u, 57u, (2u, 7u, 239u)
#define PPUTLIMPL_UINT_TRAIT_3345u DEC, 0xD11, 1, 11u, 57u, (3u, 5u, 223u)
#define PPUTLIMPL_UINT_TRAIT_3344u DEC, 0xD10, 1, 11u, 57u, (2u, 2u, 2u, 2u, 11u, 19u)
#define PPUTLIMPL_UINT_TRAIT_3343u DEC, 0xD0F, 1, 11u, 57u, ()
#define PPUTLIMPL_UINT_TRAIT_3342u DEC, 0xD0E, 1, 11u, 57u, (2u, 3u, 557u)
#define PPUTLIMPL_UINT_TRAIT_3341u DEC, 0xD0D, 1, 11u, 57u, (13u, 257u)
#define PPUTLIMPL_UINT_TRAIT_3340u DEC, 0xD0C, 1, 11u, 57u, (2u, 2u, 5u, 167u)
#define PPUTLIMPL_UINT_TRAIT_3339u DEC, 0xD0B, 1, 11u, 57u, (3u, 3u, 7u, 53u)
#define PPUTLIMPL_UINT_TRAIT_3338u DEC, 0xD0A, 1, 11u, 57u, (2u, 1669u)
#define PPUTLIMPL_UINT_TRAIT_3337u DEC, 0xD09, 1, 11u, 57u, (47u, 71u)
#define PPUTLIMPL_UINT_TRAIT_3336u DEC, 0xD08, 1, 11u, 57u, (2u, 2u, 2u, 3u, 139u)
#define PPUTLIMPL_UINT_TRAIT_3335u DEC, 0xD07, 1, 11u, 57u, (5u, 23u, 29u)
#define PPUTLIMPL_UINT_TRAIT_3334u DEC, 0xD06, 1, 11u, 57u, (2u, 1667u)
#define PPUTLIMPL_UINT_TRAIT_3333u DEC, 0xD05, 1, 11u, 57u, (3u, 11u, 101u)
#define PPUTLIMPL_UINT_TRAIT_3332u DEC, 0xD04, 1, 11u, 57u, (2u, 2u, 7u, 7u, 17u)
#define PPUTLIMPL_UINT_TRAIT_3331u DEC, 0xD03, 1, 11u, 57u, ()
#define PPUTLIMPL_UINT_TRAIT_3330u DEC, 0xD02, 1, 11u, 57u, (2u, 3u, 3u, 5u, 37u)
#define PPUTLIMPL_UINT_TRAIT_3329u DEC, 0xD01, 1, 11u, 57u, ()
#define PPUTLIMPL_UINT_TRAIT_3328u DEC, 0xD00, 1, 11u, 57u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 13u)
#define PPUTLIMPL_UINT_TRAIT_3327u DEC, 0xCFF, 1, 11u, 57u, (3u, 1109u)
#define PPUTLIMPL_UINT_TRAIT_3326u DEC, 0xCFE, 1, 11u, 57u, (2u, 1663u)
#define PPUTLIMPL_UINT_TRAIT_3325u DEC, 0xCFD, 1, 11u, 57u, (5u, 5u, 7u, 19u)
#define PPUTLIMPL_UINT_TRAIT_3324u DEC, 0xCFC, 1, 11u, 57u, (2u, 2u, 3u, 277u)
#define PPUTLIMPL_UINT_TRAIT_3323u DEC, 0xCFB, 1, 11u, 57u, ()
#define PPUTLIMPL_UINT_TRAIT_3322u DEC, 0xCFA, 1, 11u, 57u, (2u, 11u, 151u)
#define PPUTLIMPL_UINT_TRAIT_3321u DEC, 0xCF9, 1, 11u, 57u, (3u, 3u, 3u, 3u, 41u)
#define PPUTLIMPL_UINT_TRAIT_3320u DEC, 0xCF8, 1, 11u, 57u, (2u, 2u, 2u, 5u, 83u)
#define PPUTLIMPL_UINT_TRAIT_3319u DEC, 0xCF7, 1, 11u, 57u, ()
#define PPUTLIMPL_UINT_TRAIT_3318u DEC, 0xCF6, 1, 11u, 57u, (2u, 3u, 7u, 79u)
#define PPUTLIMPL_UINT_TRAIT_3317u DEC, 0xCF5, 1, 11u, 57u, (31u, 107u)
#define PPUTLIMPL_UINT_TRAIT_3316u DEC, 0xCF4, 1, 11u, 57u, (2u, 2u, 829u)
#define PPUTLIMPL_UINT_TRAIT_3315u DEC, 0xCF3, 1, 11u, 57u, (3u, 5u, 13u, 17u)
#define PPUTLIMPL_UINT_TRAIT_3314u DEC, 0xCF2, 1, 11u, 57u, (2u, 1657u)
#define PPUTLIMPL_UINT_TRAIT_3313u DEC, 0xCF1, 1, 11u, 57u, ()
#define PPUTLIMPL_UINT_TRAIT_3312u DEC, 0xCF0, 1, 11u, 57u, (2u, 2u, 2u, 2u, 3u, 3u, 23u)
#define PPUTLIMPL_UINT_TRAIT_3311u DEC, 0xCEF, 1, 11u, 57u, (7u, 11u, 43u)
#define PPUTLIMPL_UINT_TRAIT_3310u DEC, 0xCEE, 1, 11u, 57u, (2u, 5u, 331u)
#define PPUTLIMPL_UINT_TRAIT_3309u DEC, 0xCED, 1, 11u, 57u, (3u, 1103u)
#define PPUTLIMPL_UINT_TRAIT_3308u DEC, 0xCEC, 1, 11u, 57u, (2u, 2u, 827u)
#define PPUTLIMPL_UINT_TRAIT_3307u DEC, 0xCEB, 1, 11u, 57u, ()
#define PPUTLIMPL_UINT_TRAIT_3306u DEC, 0xCEA, 1, 11u, 57u, (2u, 3u, 19u, 29u)
#define PPUTLIMPL_UINT_TRAIT_3305u DEC, 0xCE9, 1, 11u, 57u, (5u, 661u)
#define PPUTLIMPL_UINT_TRAIT_3304u DEC, 0xCE8, 1, 11u, 57u, (2u, 2u, 2u, 7u, 59u)
#define PPUTLIMPL_UINT_TRAIT_3303u DEC, 0xCE7, 1, 11u, 57u, (3u, 3u, 367u)
#define PPUTLIMPL_UINT_TRAIT_3302u DEC, 0xCE6, 1, 11u, 57u, (2u, 13u, 127u)
#define PPUTLIMPL_UINT_TRAIT_3301u DEC, 0xCE5, 1, 11u, 57u, ()
#define PPUTLIMPL_UINT_TRAIT_3300u DEC, 0xCE4, 1, 11u, 57u, (2u, 2u, 3u, 5u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_3299u DEC, 0xCE3, 1, 11u, 57u, ()
#define PPUTLIMPL_UINT_TRAIT_3298u DEC, 0xCE2, 1, 11u, 57u, (2u, 17u, 97u)
#define PPUTLIMPL_UINT_TRAIT_3297u DEC, 0xCE1, 1, 11u, 57u, (3u, 7u, 157u)
#define PPUTLIMPL_UINT_TRAIT_3296u DEC, 0xCE0, 1, 11u, 57u, (2u, 2u, 2u, 2u, 2u, 103u)
#define PPUTLIMPL_UINT_TRAIT_3295u DEC, 0xCDF, 1, 11u, 57u, (5u, 659u)
#define PPUTLIMPL_UINT_TRAIT_3294u DEC, 0xCDE, 1, 11u, 57u, (2u, 3u, 3u, 3u, 61u)
#define PPUTLIMPL_UINT_TRAIT_3293u DEC, 0xCDD, 1, 11u, 57u, (37u, 89u)
#define PPUTLIMPL_UINT_TRAIT_3292u DEC, 0xCDC, 1, 11u, 57u, (2u, 2u, 823u)
#define PPUTLIMPL_UINT_TRAIT_3291u DEC, 0xCDB, 1, 11u, 57u, (3u, 1097u)
#define PPUTLIMPL_UINT_TRAIT_3290u DEC, 0xCDA, 1, 11u, 57u, (2u, 5u, 7u, 47u)
#define PPUTLIMPL_UINT_TRAIT_3289u DEC, 0xCD9, 1, 11u, 57u, (11u, 13u, 23u)
#define PPUTLIMPL_UINT_TRAIT_3288u DEC, 0xCD8, 1, 11u, 57u, (2u, 2u, 2u, 3u, 137u)
#define PPUTLIMPL_UINT_TRAIT_3287u DEC, 0xCD7, 1, 11u, 57u, (19u, 173u)
#define PPUTLIMPL_UINT_TRAIT_3286u DEC, 0xCD6, 1, 11u, 57u, (2u, 31u, 53u)
#define PPUTLIMPL_UINT_TRAIT_3285u DEC, 0xCD5, 1, 11u, 57u, (3u, 3u, 5u, 73u)
#define PPUTLIMPL_UINT_TRAIT_3284u DEC, 0xCD4, 1, 11u, 57u, (2u, 2u, 821u)
#define PPUTLIMPL_UINT_TRAIT_3283u DEC, 0xCD3, 1, 11u, 57u, (7u, 7u, 67u)
#define PPUTLIMPL_UINT_TRAIT_3282u DEC, 0xCD2, 1, 11u, 57u, (2u, 3u, 547u)
#define PPUTLIMPL_UINT_TRAIT_3281u DEC, 0xCD1, 1, 11u, 57u, (17u, 193u)
#define PPUTLIMPL_UINT_TRAIT_3280u DEC, 0xCD0, 1, 11u, 57u, (2u, 2u, 2u, 2u, 5u, 41u)
#define PPUTLIMPL_UINT_TRAIT_3279u DEC, 0xCCF, 1, 11u, 57u, (3u, 1093u)
#define PPUTLIMPL_UINT_TRAIT_3278u DEC, 0xCCE, 1, 11u, 57u, (2u, 11u, 149u)
#define PPUTLIMPL_UINT_TRAIT_3277u DEC, 0xCCD, 1, 11u, 57u, (29u, 113u)
#define PPUTLIMPL_UINT_TRAIT_3276u DEC, 0xCCC, 1, 11u, 57u, (2u, 2u, 3u, 3u, 7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_3275u DEC, 0xCCB, 1, 11u, 57u, (5u, 5u, 131u)
#define PPUTLIMPL_UINT_TRAIT_3274u DEC, 0xCCA, 1, 11u, 57u, (2u, 1637u)
#define PPUTLIMPL_UINT_TRAIT_3273u DEC, 0xCC9, 1, 11u, 57u, (3u, 1091u)
#define PPUTLIMPL_UINT_TRAIT_3272u DEC, 0xCC8, 1, 11u, 57u, (2u, 2u, 2u, 409u)
#define PPUTLIMPL_UINT_TRAIT_3271u DEC, 0xCC7, 1, 11u, 57u, ()
#define PPUTLIMPL_UINT_TRAIT_3270u DEC, 0xCC6, 1, 11u, 57u, (2u, 3u, 5u, 109u)
#define PPUTLIMPL_UINT_TRAIT_3269u DEC, 0xCC5, 1, 11u, 57u, (7u, 467u)
#define PPUTLIMPL_UINT_TRAIT_3268u DEC, 0xCC4, 1, 11u, 57u, (2u, 2u, 19u, 43u)
#define PPUTLIMPL_UINT_TRAIT_3267u DEC, 0xCC3, 1, 11u, 57u, (3u, 3u, 3u, 11u, 11u)
#define PPUTLIMPL_UINT_TRAIT_3266u DEC, 0xCC2, 1, 11u, 57u, (2u, 23u, 71u)
#define PPUTLIMPL_UINT_TRAIT_3265u DEC, 0xCC1, 1, 11u, 57u, (5u, 653u)
#define PPUTLIMPL_UINT_TRAIT_3264u DEC, 0xCC0, 1, 11u, 57u, (2u, 2u, 2u, 2u, 2u, 2u, 3u, 17u)
#define PPUTLIMPL_UINT_TRAIT_3263u DEC, 0xCBF, 1, 11u, 57u, (13u, 251u)
#define PPUTLIMPL_UINT_TRAIT_3262u DEC, 0xCBE, 1, 11u, 57u, (2u, 7u, 233u)
#define PPUTLIMPL_UINT_TRAIT_3261u DEC, 0xCBD, 1, 11u, 57u, (3u, 1087u)
#define PPUTLIMPL_UINT_TRAIT_3260u DEC, 0xCBC, 1, 11u, 57u, (2u, 2u, 5u, 163u)
#define PPUTLIMPL_UINT_TRAIT_3259u DEC, 0xCBB, 1, 11u, 57u, ()
#define PPUTLIMPL_UINT_TRAIT_3258u DEC, 0xCBA, 1, 11u, 57u, (2u, 3u, 3u, 181u)
#define PPUTLIMPL_UINT_TRAIT_3257u DEC, 0xCB9, 1, 11u, 57u, ()
#define PPUTLIMPL_UINT_TRAIT_3256u DEC, 0xCB8, 1, 11u, 57u, (2u, 2u, 2u, 11u, 37u)
#define PPUTLIMPL_UINT_TRAIT_3255u DEC, 0xCB7, 1, 11u, 57u, (3u, 5u, 7u, 31u)
#define PPUTLIMPL_UINT_TRAIT_3254u DEC, 0xCB6, 1, 11u, 57u, (2u, 1627u)
#define PPUTLIMPL_UINT_TRAIT_3253u DEC, 0xCB5, 1, 11u, 57u, ()
#define PPUTLIMPL_UINT_TRAIT_3252u DEC, 0xCB4, 1, 11u, 57u, (2u, 2u, 3u, 271u)
#define PPUTLIMPL_UINT_TRAIT_3251u DEC, 0xCB3, 1, 11u, 57u, ()
#define PPUTLIMPL_UINT_TRAIT_3250u DEC, 0xCB2, 1, 11u, 57u, (2u, 5u, 5u, 5u, 13u)
#define PPUTLIMPL_UINT_TRAIT_3249u DEC, 0xCB1, 1, 11u, 57u, (3u, 3u, 19u, 19u)
#define PPUTLIMPL_UINT_TRAIT_3248u DEC, 0xCB0, 1, 11u, 56u, (2u, 2u, 2u, 2u, 7u, 29u)
#define PPUTLIMPL_UINT_TRAIT_3247u DEC, 0xCAF, 1, 11u, 56u, (17u, 191u)
#define PPUTLIMPL_UINT_TRAIT_3246u DEC, 0xCAE, 1, 11u, 56u, (2u, 3u, 541u)
#define PPUTLIMPL_UINT_TRAIT_3245u DEC, 0xCAD, 1, 11u, 56u, (5u, 11u, 59u)
#define PPUTLIMPL_UINT_TRAIT_3244u DEC, 0xCAC, 1, 11u, 56u, (2u, 2u, 811u)
#define PPUTLIMPL_UINT_TRAIT_3243u DEC, 0xCAB, 1, 11u, 56u, (3u, 23u, 47u)
#define PPUTLIMPL_UINT_TRAIT_3242u DEC, 0xCAA, 1, 11u, 56u, (2u, 1621u)
#define PPUTLIMPL_UINT_TRAIT_3241u DEC, 0xCA9, 1, 11u, 56u, (7u, 463u)
#define PPUTLIMPL_UINT_TRAIT_3240u DEC, 0xCA8, 1, 11u, 56u, (2u, 2u, 2u, 3u, 3u, 3u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_3239u DEC, 0xCA7, 1, 11u, 56u, (41u, 79u)
#define PPUTLIMPL_UINT_TRAIT_3238u DEC, 0xCA6, 1, 11u, 56u, (2u, 1619u)
#define PPUTLIMPL_UINT_TRAIT_3237u DEC, 0xCA5, 1, 11u, 56u, (3u, 13u, 83u)
#define PPUTLIMPL_UINT_TRAIT_3236u DEC, 0xCA4, 1, 11u, 56u, (2u, 2u, 809u)
#define PPUTLIMPL_UINT_TRAIT_3235u DEC, 0xCA3, 1, 11u, 56u, (5u, 647u)
#define PPUTLIMPL_UINT_TRAIT_3234u DEC, 0xCA2, 1, 11u, 56u, (2u, 3u, 7u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_3233u DEC, 0xCA1, 1, 11u, 56u, (53u, 61u)
#define PPUTLIMPL_UINT_TRAIT_3232u DEC, 0xCA0, 1, 11u, 56u, (2u, 2u, 2u, 2u, 2u, 101u)
#define PPUTLIMPL_UINT_TRAIT_3231u DEC, 0xC9F, 1, 11u, 56u, (3u, 3u, 359u)
#define PPUTLIMPL_UINT_TRAIT_3230u DEC, 0xC9E, 1, 11u, 56u, (2u, 5u, 17u, 19u)
#define PPUTLIMPL_UINT_TRAIT_3229u DEC, 0xC9D, 1, 11u, 56u, ()
#define PPUTLIMPL_UINT_TRAIT_3228u DEC, 0xC9C, 1, 11u, 56u, (2u, 2u, 3u, 269u)
#define PPUTLIMPL_UINT_TRAIT_3227u DEC, 0xC9B, 1, 11u, 56u, (7u, 461u)
#define PPUTLIMPL_UINT_TRAIT_3226u DEC, 0xC9A, 1, 11u, 56u, (2u, 1613u)
#define PPUTLIMPL_UINT_TRAIT_3225u DEC, 0xC99, 1, 11u, 56u, (3u, 5u, 5u, 43u)
#define PPUTLIMPL_UINT_TRAIT_3224u DEC, 0xC98, 1, 11u, 56u, (2u, 2u, 2u, 13u, 31u)
#define PPUTLIMPL_UINT_TRAIT_3223u DEC, 0xC97, 1, 11u, 56u, (11u, 293u)
#define PPUTLIMPL_UINT_TRAIT_3222u DEC, 0xC96, 1, 11u, 56u, (2u, 3u, 3u, 179u)
#define PPUTLIMPL_UINT_TRAIT_3221u DEC, 0xC95, 1, 11u, 56u, ()
#define PPUTLIMPL_UINT_TRAIT_3220u DEC, 0xC94, 1, 11u, 56u, (2u, 2u, 5u, 7u, 23u)
#define PPUTLIMPL_UINT_TRAIT_3219u DEC, 0xC93, 1, 11u, 56u, (3u, 29u, 37u)
#define PPUTLIMPL_UINT_TRAIT_3218u DEC, 0xC92, 1, 11u, 56u, (2u, 1609u)
#define PPUTLIMPL_UINT_TRAIT_3217u DEC, 0xC91, 1, 11u, 56u, ()
#define PPUTLIMPL_UINT_TRAIT_3216u DEC, 0xC90, 1, 11u, 56u, (2u, 2u, 2u, 2u, 3u, 67u)
#define PPUTLIMPL_UINT_TRAIT_3215u DEC, 0xC8F, 1, 11u, 56u, (5u, 643u)
#define PPUTLIMPL_UINT_TRAIT_3214u DEC, 0xC8E, 1, 11u, 56u, (2u, 1607u)
#define PPUTLIMPL_UINT_TRAIT_3213u DEC, 0xC8D, 1, 11u, 56u, (3u, 3u, 3u, 7u, 17u)
#define PPUTLIMPL_UINT_TRAIT_3212u DEC, 0xC8C, 1, 11u, 56u, (2u, 2u, 11u, 73u)
#define PPUTLIMPL_UINT_TRAIT_3211u DEC, 0xC8B, 1, 11u, 56u, (13u, 13u, 19u)
#define PPUTLIMPL_UINT_TRAIT_3210u DEC, 0xC8A, 1, 11u, 56u, (2u, 3u, 5u, 107u)
#define PPUTLIMPL_UINT_TRAIT_3209u DEC, 0xC89, 1, 11u, 56u, ()
#define PPUTLIMPL_UINT_TRAIT_3208u DEC, 0xC88, 1, 11u, 56u, (2u, 2u, 2u, 401u)
#define PPUTLIMPL_UINT_TRAIT_3207u DEC, 0xC87, 1, 11u, 56u, (3u, 1069u)
#define PPUTLIMPL_UINT_TRAIT_3206u DEC, 0xC86, 1, 11u, 56u, (2u, 7u, 229u)
#define PPUTLIMPL_UINT_TRAIT_3205u DEC, 0xC85, 1, 11u, 56u, (5u, 641u)
#define PPUTLIMPL_UINT_TRAIT_3204u DEC, 0xC84, 1, 11u, 56u, (2u, 2u, 3u, 3u, 89u)
#define PPUTLIMPL_UINT_TRAIT_3203u DEC, 0xC83, 1, 11u, 56u, ()
#define PPUTLIMPL_UINT_TRAIT_3202u DEC, 0xC82, 1, 11u, 56u, (2u, 1601u)
#define PPUTLIMPL_UINT_TRAIT_3201u DEC, 0xC81, 1, 11u, 56u, (3u, 11u, 97u)
#define PPUTLIMPL_UINT_TRAIT_3200u DEC, 0xC80, 1, 11u, 56u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_3199u DEC, 0xC7F, 1, 11u, 56u, (7u, 457u)
#define PPUTLIMPL_UINT_TRAIT_3198u DEC, 0xC7E, 1, 11u, 56u, (2u, 3u, 13u, 41u)
#define PPUTLIMPL_UINT_TRAIT_3197u DEC, 0xC7D, 1, 11u, 56u, (23u, 139u)
#define PPUTLIMPL_UINT_TRAIT_3196u DEC, 0xC7C, 1, 11u, 56u, (2u, 2u, 17u, 47u)
#define PPUTLIMPL_UINT_TRAIT_3195u DEC, 0xC7B, 1, 11u, 56u, (3u, 3u, 5u, 71u)
#define PPUTLIMPL_UINT_TRAIT_3194u DEC, 0xC7A, 1, 11u, 56u, (2u, 1597u)
#define PPUTLIMPL_UINT_TRAIT_3193u DEC, 0xC79, 1, 11u, 56u, (31u, 103u)
#define PPUTLIMPL_UINT_TRAIT_3192u DEC, 0xC78, 1, 11u, 56u, (2u, 2u, 2u, 3u, 7u, 19u)
#define PPUTLIMPL_UINT_TRAIT_3191u DEC, 0xC77, 1, 11u, 56u, ()
#define PPUTLIMPL_UINT_TRAIT_3190u DEC, 0xC76, 1, 11u, 56u, (2u, 5u, 11u, 29u)
#define PPUTLIMPL_UINT_TRAIT_3189u DEC, 0xC75, 1, 11u, 56u, (3u, 1063u)
#define PPUTLIMPL_UINT_TRAIT_3188u DEC, 0xC74, 1, 11u, 56u, (2u, 2u, 797u)
#define PPUTLIMPL_UINT_TRAIT_3187u DEC, 0xC73, 1, 11u, 56u, ()
#define PPUTLIMPL_UINT_TRAIT_3186u DEC, 0xC72, 1, 11u, 56u, (2u, 3u, 3u, 3u, 59u)
#define PPUTLIMPL_UINT_TRAIT_3185u DEC, 0xC71, 1, 11u, 56u, (5u, 7u, 7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_3184u DEC, 0xC70, 1, 11u, 56u, (2u, 2u, 2u, 2u, 199u)
#define PPUTLIMPL_UINT_TRAIT_3183u DEC, 0xC6F, 1, 11u, 56u, (3u, 1061u)
#define PPUTLIMPL_UINT_TRAIT_3182u DEC, 0xC6E, 1, 11u, 56u, (2u, 37u, 43u)
#define PPUTLIMPL_UINT_TRAIT_3181u DEC, 0xC6D, 1, 11u, 56u, ()
#define PPUTLIMPL_UINT_TRAIT_3180u DEC, 0xC6C, 1, 11u, 56u, (2u, 2u, 3u, 5u, 53u)
#define PPUTLIMPL_UINT_TRAIT_3179u DEC, 0xC6B, 1, 11u, 56u, (11u, 17u, 17u)
#define PPUTLIMPL_UINT_TRAIT_3178u DEC, 0xC6A, 1, 11u, 56u, (2u, 7u, 227u)
#define PPUTLIMPL_UINT_TRAIT_3177u DEC, 0xC69, 1, 11u, 56u, (3u, 3u, 353u)
#define PPUTLIMPL_UINT_TRAIT_3176u DEC, 0xC68, 1, 11u, 56u, (2u, 2u, 2u, 397u)
#define PPUTLIMPL_UINT_TRAIT_3175u DEC, 0xC67, 1, 11u, 56u, (5u, 5u, 127u)
#define PPUTLIMPL_UINT_TRAIT_3174u DEC, 0xC66, 1, 11u, 56u, (2u, 3u, 23u, 23u)
#define PPUTLIMPL_UINT_TRAIT_3173u DEC, 0xC65, 1, 11u, 56u, (19u, 167u)
#define PPUTLIMPL_UINT_TRAIT_3172u DEC, 0xC64, 1, 11u, 56u, (2u, 2u, 13u, 61u)
#define PPUTLIMPL_UINT_TRAIT_3171u DEC, 0xC63, 1, 11u, 56u, (3u, 7u, 151u)
#define PPUTLIMPL_UINT_TRAIT_3170u DEC, 0xC62, 1, 11u, 56u, (2u, 5u, 317u)
#define PPUTLIMPL_UINT_TRAIT_3169u DEC, 0xC61, 1, 11u, 56u, ()
#define PPUTLIMPL_UINT_TRAIT_3168u DEC, 0xC60, 1, 11u, 56u, (2u, 2u, 2u, 2u, 2u, 3u, 3u, 11u)
#define PPUTLIMPL_UINT_TRAIT_3167u DEC, 0xC5F, 1, 11u, 56u, ()
#define PPUTLIMPL_UINT_TRAIT_3166u DEC, 0xC5E, 1, 11u, 56u, (2u, 1583u)
#define PPUTLIMPL_UINT_TRAIT_3165u DEC, 0xC5D, 1, 11u, 56u, (3u, 5u, 211u)
#define PPUTLIMPL_UINT_TRAIT_3164u DEC, 0xC5C, 1, 11u, 56u, (2u, 2u, 7u, 113u)
#define PPUTLIMPL_UINT_TRAIT_3163u DEC, 0xC5B, 1, 11u, 56u, ()
#define PPUTLIMPL_UINT_TRAIT_3162u DEC, 0xC5A, 1, 11u, 56u, (2u, 3u, 17u, 31u)
#define PPUTLIMPL_UINT_TRAIT_3161u DEC, 0xC59, 1, 11u, 56u, (29u, 109u)
#define PPUTLIMPL_UINT_TRAIT_3160u DEC, 0xC58, 1, 11u, 56u, (2u, 2u, 2u, 5u, 79u)
#define PPUTLIMPL_UINT_TRAIT_3159u DEC, 0xC57, 1, 11u, 56u, (3u, 3u, 3u, 3u, 3u, 13u)
#define PPUTLIMPL_UINT_TRAIT_3158u DEC, 0xC56, 1, 11u, 56u, (2u, 1579u)
#define PPUTLIMPL_UINT_TRAIT_3157u DEC, 0xC55, 1, 11u, 56u, (7u, 11u, 41u)
#define PPUTLIMPL_UINT_TRAIT_3156u DEC, 0xC54, 1, 11u, 56u, (2u, 2u, 3u, 263u)
#define PPUTLIMPL_UINT_TRAIT_3155u DEC, 0xC53, 1, 11u, 56u, (5u, 631u)
#define PPUTLIMPL_UINT_TRAIT_3154u DEC, 0xC52, 1, 11u, 56u, (2u, 19u, 83u)
#define PPUTLIMPL_UINT_TRAIT_3153u DEC, 0xC51, 1, 11u, 56u, (3u, 1051u)
#define PPUTLIMPL_UINT_TRAIT_3152u DEC, 0xC50, 1, 11u, 56u, (2u, 2u, 2u, 2u, 197u)
#define PPUTLIMPL_UINT_TRAIT_3151u DEC, 0xC4F, 1, 11u, 56u, (23u, 137u)
#define PPUTLIMPL_UINT_TRAIT_3150u DEC, 0xC4E, 1, 11u, 56u, (2u, 3u, 3u, 5u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_3149u DEC, 0xC4D, 1, 11u, 56u, (47u, 67u)
#define PPUTLIMPL_UINT_TRAIT_3148u DEC, 0xC4C, 1, 11u, 56u, (2u, 2u, 787u)
#define PPUTLIMPL_UINT_TRAIT_3147u DEC, 0xC4B, 1, 11u, 56u, (3u, 1049u)
#define PPUTLIMPL_UINT_TRAIT_3146u DEC, 0xC4A, 1, 11u, 56u, (2u, 11u, 11u, 13u)
#define PPUTLIMPL_UINT_TRAIT_3145u DEC, 0xC49, 1, 11u, 56u, (5u, 17u, 37u)
#define PPUTLIMPL_UINT_TRAIT_3144u DEC, 0xC48, 1, 11u, 56u, (2u, 2u, 2u, 3u, 131u)
#define PPUTLIMPL_UINT_TRAIT_3143u DEC, 0xC47, 1, 11u, 56u, (7u, 449u)
#define PPUTLIMPL_UINT_TRAIT_3142u DEC, 0xC46, 1, 11u, 56u, (2u, 1571u)
#define PPUTLIMPL_UINT_TRAIT_3141u DEC, 0xC45, 1, 11u, 56u, (3u, 3u, 349u)
#define PPUTLIMPL_UINT_TRAIT_3140u DEC, 0xC44, 1, 11u, 56u, (2u, 2u, 5u, 157u)
#define PPUTLIMPL_UINT_TRAIT_3139u DEC, 0xC43, 1, 11u, 56u, (43u, 73u)
#define PPUTLIMPL_UINT_TRAIT_3138u DEC, 0xC42, 1, 11u, 56u, (2u, 3u, 523u)
#define PPUTLIMPL_UINT_TRAIT_3137u DEC, 0xC41, 1, 11u, 56u, ()
#define PPUTLIMPL_UINT_TRAIT_3136u DEC, 0xC40, 1, 11u, 56u, (2u, 2u, 2u, 2u, 2u, 2u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_3135u DEC, 0xC3F, 1, 11u, 55u, (3u, 5u, 11u, 19u)
#define PPUTLIMPL_UINT_TRAIT_3134u DEC, 0xC3E, 1, 11u, 55u, (2u, 1567u)
#define PPUTLIMPL_UINT_TRAIT_3133u DEC, 0xC3D, 1, 11u, 55u, (13u, 241u)
#define PPUTLIMPL_UINT_TRAIT_3132u DEC, 0xC3C, 1, 11u, 55u, (2u, 2u, 3u, 3u, 3u, 29u)
#define PPUTLIMPL_UINT_TRAIT_3131u DEC, 0xC3B, 1, 11u, 55u, (31u, 101u)
#define PPUTLIMPL_UINT_TRAIT_3130u DEC, 0xC3A, 1, 11u, 55u, (2u, 5u, 313u)
#define PPUTLIMPL_UINT_TRAIT_3129u DEC, 0xC39, 1, 11u, 55u, (3u, 7u, 149u)
#define PPUTLIMPL_UINT_TRAIT_3128u DEC, 0xC38, 1, 11u, 55u, (2u, 2u, 2u, 17u, 23u)
#define PPUTLIMPL_UINT_TRAIT_3127u DEC, 0xC37, 1, 11u, 55u, (53u, 59u)
#define PPUTLIMPL_UINT_TRAIT_3126u DEC, 0xC36, 1, 11u, 55u, (2u, 3u, 521u)
#define PPUTLIMPL_UINT_TRAIT_3125u DEC, 0xC35, 1, 11u, 55u, (5u, 5u, 5u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_3124u DEC, 0xC34, 1, 11u, 55u, (2u, 2u, 11u, 71u)
#define PPUTLIMPL_UINT_TRAIT_3123u DEC, 0xC33, 1, 11u, 55u, (3u, 3u, 347u)
#define PPUTLIMPL_UINT_TRAIT_3122u DEC, 0xC32, 1, 11u, 55u, (2u, 7u, 223u)
#define PPUTLIMPL_UINT_TRAIT_3121u DEC, 0xC31, 1, 11u, 55u, ()
#define PPUTLIMPL_UINT_TRAIT_3120u DEC, 0xC30, 1, 11u, 55u, (2u, 2u, 2u, 2u, 3u, 5u, 13u)
#define PPUTLIMPL_UINT_TRAIT_3119u DEC, 0xC2F, 1, 11u, 55u, ()
#define PPUTLIMPL_UINT_TRAIT_3118u DEC, 0xC2E, 1, 11u, 55u, (2u, 1559u)
#define PPUTLIMPL_UINT_TRAIT_3117u DEC, 0xC2D, 1, 11u, 55u, (3u, 1039u)
#define PPUTLIMPL_UINT_TRAIT_3116u DEC, 0xC2C, 1, 11u, 55u, (2u, 2u, 19u, 41u)
#define PPUTLIMPL_UINT_TRAIT_3115u DEC, 0xC2B, 1, 11u, 55u, (5u, 7u, 89u)
#define PPUTLIMPL_UINT_TRAIT_3114u DEC, 0xC2A, 1, 11u, 55u, (2u, 3u, 3u, 173u)
#define PPUTLIMPL_UINT_TRAIT_3113u DEC, 0xC29, 1, 11u, 55u, (11u, 283u)
#define PPUTLIMPL_UINT_TRAIT_3112u DEC, 0xC28, 1, 11u, 55u, (2u, 2u, 2u, 389u)
#define PPUTLIMPL_UINT_TRAIT_3111u DEC, 0xC27, 1, 11u, 55u, (3u, 17u, 61u)
#define PPUTLIMPL_UINT_TRAIT_3110u DEC, 0xC26, 1, 11u, 55u, (2u, 5u, 311u)
#define PPUTLIMPL_UINT_TRAIT_3109u DEC, 0xC25, 1, 11u, 55u, ()
#define PPUTLIMPL_UINT_TRAIT_3108u DEC, 0xC24, 1, 11u, 55u, (2u, 2u, 3u, 7u, 37u)
#define PPUTLIMPL_UINT_TRAIT_3107u DEC, 0xC23, 1, 11u, 55u, (13u, 239u)
#define PPUTLIMPL_UINT_TRAIT_3106u DEC, 0xC22, 1, 11u, 55u, (2u, 1553u)
#define PPUTLIMPL_UINT_TRAIT_3105u DEC, 0xC21, 1, 11u, 55u, (3u, 3u, 3u, 5u, 23u)
#define PPUTLIMPL_UINT_TRAIT_3104u DEC, 0xC20, 1, 11u, 55u, (2u, 2u, 2u, 2u, 2u, 97u)
#define PPUTLIMPL_UINT_TRAIT_3103u DEC, 0xC1F, 1, 11u, 55u, (29u, 107u)
#define PPUTLIMPL_UINT_TRAIT_3102u DEC, 0xC1E, 1, 11u, 55u, (2u, 3u, 11u, 47u)
#define PPUTLIMPL_UINT_TRAIT_3101u DEC, 0xC1D, 1, 11u, 55u, (7u, 443u)
#define PPUTLIMPL_UINT_TRAIT_3100u DEC, 0xC1C, 1, 11u, 55u, (2u, 2u, 5u, 5u, 31u)
#define PPUTLIMPL_UINT_TRAIT_3099u DEC, 0xC1B, 1, 11u, 55u, (3u, 1033u)
#define PPUTLIMPL_UINT_TRAIT_3098u DEC, 0xC1A, 1, 11u, 55u, (2u, 1549u)
#define PPUTLIMPL_UINT_TRAIT_3097u DEC, 0xC19, 1, 11u, 55u, (19u, 163u)
#define PPUTLIMPL_UINT_TRAIT_3096u DEC, 0xC18, 1, 11u, 55u, (2u, 2u, 2u, 3u, 3u, 43u)
#define PPUTLIMPL_UINT_TRAIT_3095u DEC, 0xC17, 1, 11u, 55u, (5u, 619u)
#define PPUTLIMPL_UINT_TRAIT_3094u DEC, 0xC16, 1, 11u, 55u, (2u, 7u, 13u, 17u)
#define PPUTLIMPL_UINT_TRAIT_3093u DEC, 0xC15, 1, 11u, 55u, (3u, 1031u)
#define PPUTLIMPL_UINT_TRAIT_3092u DEC, 0xC14, 1, 11u, 55u, (2u, 2u, 773u)
#define PPUTLIMPL_UINT_TRAIT_3091u DEC, 0xC13, 1, 11u, 55u, (11u, 281u)
#define PPUTLIMPL_UINT_TRAIT_3090u DEC, 0xC12, 1, 11u, 55u, (2u, 3u, 5u, 103u)
#define PPUTLIMPL_UINT_TRAIT_3089u DEC, 0xC11, 1, 11u, 55u, ()
#define PPUTLIMPL_UINT_TRAIT_3088u DEC, 0xC10, 1, 11u, 55u, (2u, 2u, 2u, 2u, 193u)
#define PPUTLIMPL_UINT_TRAIT_3087u DEC, 0xC0F, 1, 11u, 55u, (3u, 3u, 7u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_3086u DEC, 0xC0E, 1, 11u, 55u, (2u, 1543u)
#define PPUTLIMPL_UINT_TRAIT_3085u DEC, 0xC0D, 1, 11u, 55u, (5u, 617u)
#define PPUTLIMPL_UINT_TRAIT_3084u DEC, 0xC0C, 1, 11u, 55u, (2u, 2u, 3u, 257u)
#define PPUTLIMPL_UINT_TRAIT_3083u DEC, 0xC0B, 1, 11u, 55u, ()
#define PPUTLIMPL_UINT_TRAIT_3082u DEC, 0xC0A, 1, 11u, 55u, (2u, 23u, 67u)
#define PPUTLIMPL_UINT_TRAIT_3081u DEC, 0xC09, 1, 11u, 55u, (3u, 13u, 79u)
#define PPUTLIMPL_UINT_TRAIT_3080u DEC, 0xC08, 1, 11u, 55u, (2u, 2u, 2u, 5u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_3079u DEC, 0xC07, 1, 11u, 55u, ()
#define PPUTLIMPL_UINT_TRAIT_3078u DEC, 0xC06, 1, 11u, 55u, (2u, 3u, 3u, 3u, 3u, 19u)
#define PPUTLIMPL_UINT_TRAIT_3077u DEC, 0xC05, 1, 11u, 55u, (17u, 181u)
#define PPUTLIMPL_UINT_TRAIT_3076u DEC, 0xC04, 1, 11u, 55u, (2u, 2u, 769u)
#define PPUTLIMPL_UINT_TRAIT_3075u DEC, 0xC03, 1, 11u, 55u, (3u, 5u, 5u, 41u)
#define PPUTLIMPL_UINT_TRAIT_3074u DEC, 0xC02, 1, 11u, 55u, (2u, 29u, 53u)
#define PPUTLIMPL_UINT_TRAIT_3073u DEC, 0xC01, 1, 11u, 55u, (7u, 439u)
#define PPUTLIMPL_UINT_TRAIT_3072u \
  DEC, 0xC00, 1, 11u, 55u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 3u)
#define PPUTLIMPL_UINT_TRAIT_3071u DEC, 0xBFF, 1, 11u, 55u, (37u, 83u)
#define PPUTLIMPL_UINT_TRAIT_3070u DEC, 0xBFE, 1, 11u, 55u, (2u, 5u, 307u)
#define PPUTLIMPL_UINT_TRAIT_3069u DEC, 0xBFD, 1, 11u, 55u, (3u, 3u, 11u, 31u)
#define PPUTLIMPL_UINT_TRAIT_3068u DEC, 0xBFC, 1, 11u, 55u, (2u, 2u, 13u, 59u)
#define PPUTLIMPL_UINT_TRAIT_3067u DEC, 0xBFB, 1, 11u, 55u, ()
#define PPUTLIMPL_UINT_TRAIT_3066u DEC, 0xBFA, 1, 11u, 55u, (2u, 3u, 7u, 73u)
#define PPUTLIMPL_UINT_TRAIT_3065u DEC, 0xBF9, 1, 11u, 55u, (5u, 613u)
#define PPUTLIMPL_UINT_TRAIT_3064u DEC, 0xBF8, 1, 11u, 55u, (2u, 2u, 2u, 383u)
#define PPUTLIMPL_UINT_TRAIT_3063u DEC, 0xBF7, 1, 11u, 55u, (3u, 1021u)
#define PPUTLIMPL_UINT_TRAIT_3062u DEC, 0xBF6, 1, 11u, 55u, (2u, 1531u)
#define PPUTLIMPL_UINT_TRAIT_3061u DEC, 0xBF5, 1, 11u, 55u, ()
#define PPUTLIMPL_UINT_TRAIT_3060u DEC, 0xBF4, 1, 11u, 55u, (2u, 2u, 3u, 3u, 5u, 17u)
#define PPUTLIMPL_UINT_TRAIT_3059u DEC, 0xBF3, 1, 11u, 55u, (7u, 19u, 23u)
#define PPUTLIMPL_UINT_TRAIT_3058u DEC, 0xBF2, 1, 11u, 55u, (2u, 11u, 139u)
#define PPUTLIMPL_UINT_TRAIT_3057u DEC, 0xBF1, 1, 11u, 55u, (3u, 1019u)
#define PPUTLIMPL_UINT_TRAIT_3056u DEC, 0xBF0, 1, 11u, 55u, (2u, 2u, 2u, 2u, 191u)
#define PPUTLIMPL_UINT_TRAIT_3055u DEC, 0xBEF, 1, 11u, 55u, (5u, 13u, 47u)
#define PPUTLIMPL_UINT_TRAIT_3054u DEC, 0xBEE, 1, 11u, 55u, (2u, 3u, 509u)
#define PPUTLIMPL_UINT_TRAIT_3053u DEC, 0xBED, 1, 11u, 55u, (43u, 71u)
#define PPUTLIMPL_UINT_TRAIT_3052u DEC, 0xBEC, 1, 11u, 55u, (2u, 2u, 7u, 109u)
#define PPUTLIMPL_UINT_TRAIT_3051u DEC, 0xBEB, 1, 11u, 55u, (3u, 3u, 3u, 113u)
#define PPUTLIMPL_UINT_TRAIT_3050u DEC, 0xBEA, 1, 11u, 55u, (2u, 5u, 5u, 61u)
#define PPUTLIMPL_UINT_TRAIT_3049u DEC, 0xBE9, 1, 11u, 55u, ()
#define PPUTLIMPL_UINT_TRAIT_3048u DEC, 0xBE8, 1, 11u, 55u, (2u, 2u, 2u, 3u, 127u)
#define PPUTLIMPL_UINT_TRAIT_3047u DEC, 0xBE7, 1, 11u, 55u, (11u, 277u)
#define PPUTLIMPL_UINT_TRAIT_3046u DEC, 0xBE6, 1, 11u, 55u, (2u, 1523u)
#define PPUTLIMPL_UINT_TRAIT_3045u DEC, 0xBE5, 1, 11u, 55u, (3u, 5u, 7u, 29u)
#define PPUTLIMPL_UINT_TRAIT_3044u DEC, 0xBE4, 1, 11u, 55u, (2u, 2u, 761u)
#define PPUTLIMPL_UINT_TRAIT_3043u DEC, 0xBE3, 1, 11u, 55u, (17u, 179u)
#define PPUTLIMPL_UINT_TRAIT_3042u DEC, 0xBE2, 1, 11u, 55u, (2u, 3u, 3u, 13u, 13u)
#define PPUTLIMPL_UINT_TRAIT_3041u DEC, 0xBE1, 1, 11u, 55u, ()
#define PPUTLIMPL_UINT_TRAIT_3040u DEC, 0xBE0, 1, 11u, 55u, (2u, 2u, 2u, 2u, 2u, 5u, 19u)
#define PPUTLIMPL_UINT_TRAIT_3039u DEC, 0xBDF, 1, 11u, 55u, (3u, 1013u)
#define PPUTLIMPL_UINT_TRAIT_3038u DEC, 0xBDE, 1, 11u, 55u, (2u, 7u, 7u, 31u)
#define PPUTLIMPL_UINT_TRAIT_3037u DEC, 0xBDD, 1, 11u, 55u, ()
#define PPUTLIMPL_UINT_TRAIT_3036u DEC, 0xBDC, 1, 11u, 55u, (2u, 2u, 3u, 11u, 23u)
#define PPUTLIMPL_UINT_TRAIT_3035u DEC, 0xBDB, 1, 11u, 55u, (5u, 607u)
#define PPUTLIMPL_UINT_TRAIT_3034u DEC, 0xBDA, 1, 11u, 55u, (2u, 37u, 41u)
#define PPUTLIMPL_UINT_TRAIT_3033u DEC, 0xBD9, 1, 11u, 55u, (3u, 3u, 337u)
#define PPUTLIMPL_UINT_TRAIT_3032u DEC, 0xBD8, 1, 11u, 55u, (2u, 2u, 2u, 379u)
#define PPUTLIMPL_UINT_TRAIT_3031u DEC, 0xBD7, 1, 11u, 55u, (7u, 433u)
#define PPUTLIMPL_UINT_TRAIT_3030u DEC, 0xBD6, 1, 11u, 55u, (2u, 3u, 5u, 101u)
#define PPUTLIMPL_UINT_TRAIT_3029u DEC, 0xBD5, 1, 11u, 55u, (13u, 233u)
#define PPUTLIMPL_UINT_TRAIT_3028u DEC, 0xBD4, 1, 11u, 55u, (2u, 2u, 757u)
#define PPUTLIMPL_UINT_TRAIT_3027u DEC, 0xBD3, 1, 11u, 55u, (3u, 1009u)
#define PPUTLIMPL_UINT_TRAIT_3026u DEC, 0xBD2, 1, 11u, 55u, (2u, 17u, 89u)
#define PPUTLIMPL_UINT_TRAIT_3025u DEC, 0xBD1, 1, 11u, 55u, (5u, 5u, 11u, 11u)
#define PPUTLIMPL_UINT_TRAIT_3024u DEC, 0xBD0, 1, 11u, 54u, (2u, 2u, 2u, 2u, 3u, 3u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_3023u DEC, 0xBCF, 1, 11u, 54u, ()
#define PPUTLIMPL_UINT_TRAIT_3022u DEC, 0xBCE, 1, 11u, 54u, (2u, 1511u)
#define PPUTLIMPL_UINT_TRAIT_3021u DEC, 0xBCD, 1, 11u, 54u, (3u, 19u, 53u)
#define PPUTLIMPL_UINT_TRAIT_3020u DEC, 0xBCC, 1, 11u, 54u, (2u, 2u, 5u, 151u)
#define PPUTLIMPL_UINT_TRAIT_3019u DEC, 0xBCB, 1, 11u, 54u, ()
#define PPUTLIMPL_UINT_TRAIT_3018u DEC, 0xBCA, 1, 11u, 54u, (2u, 3u, 503u)
#define PPUTLIMPL_UINT_TRAIT_3017u DEC, 0xBC9, 1, 11u, 54u, (7u, 431u)
#define PPUTLIMPL_UINT_TRAIT_3016u DEC, 0xBC8, 1, 11u, 54u, (2u, 2u, 2u, 13u, 29u)
#define PPUTLIMPL_UINT_TRAIT_3015u DEC, 0xBC7, 1, 11u, 54u, (3u, 3u, 5u, 67u)
#define PPUTLIMPL_UINT_TRAIT_3014u DEC, 0xBC6, 1, 11u, 54u, (2u, 11u, 137u)
#define PPUTLIMPL_UINT_TRAIT_3013u DEC, 0xBC5, 1, 11u, 54u, (23u, 131u)
#define PPUTLIMPL_UINT_TRAIT_3012u DEC, 0xBC4, 1, 11u, 54u, (2u, 2u, 3u, 251u)
#define PPUTLIMPL_UINT_TRAIT_3011u DEC, 0xBC3, 1, 11u, 54u, ()
#define PPUTLIMPL_UINT_TRAIT_3010u DEC, 0xBC2, 1, 11u, 54u, (2u, 5u, 7u, 43u)
#define PPUTLIMPL_UINT_TRAIT_3009u DEC, 0xBC1, 1, 11u, 54u, (3u, 17u, 59u)
#define PPUTLIMPL_UINT_TRAIT_3008u DEC, 0xBC0, 1, 11u, 54u, (2u, 2u, 2u, 2u, 2u, 2u, 47u)
#define PPUTLIMPL_UINT_TRAIT_3007u DEC, 0xBBF, 1, 11u, 54u, (31u, 97u)
#define PPUTLIMPL_UINT_TRAIT_3006u DEC, 0xBBE, 1, 11u, 54u, (2u, 3u, 3u, 167u)
#define PPUTLIMPL_UINT_TRAIT_3005u DEC, 0xBBD, 1, 11u, 54u, (5u, 601u)
#define PPUTLIMPL_UINT_TRAIT_3004u DEC, 0xBBC, 1, 11u, 54u, (2u, 2u, 751u)
#define PPUTLIMPL_UINT_TRAIT_3003u DEC, 0xBBB, 1, 11u, 54u, (3u, 7u, 11u, 13u)
#define PPUTLIMPL_UINT_TRAIT_3002u DEC, 0xBBA, 1, 11u, 54u, (2u, 19u, 79u)
#define PPUTLIMPL_UINT_TRAIT_3001u DEC, 0xBB9, 1, 11u, 54u, ()
#define PPUTLIMPL_UINT_TRAIT_3000u DEC, 0xBB8, 1, 11u, 54u, (2u, 2u, 2u, 3u, 5u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_2999u DEC, 0xBB7, 1, 11u, 54u, ()
#define PPUTLIMPL_UINT_TRAIT_2998u DEC, 0xBB6, 1, 11u, 54u, (2u, 1499u)
#define PPUTLIMPL_UINT_TRAIT_2997u DEC, 0xBB5, 1, 11u, 54u, (3u, 3u, 3u, 3u, 37u)
#define PPUTLIMPL_UINT_TRAIT_2996u DEC, 0xBB4, 1, 11u, 54u, (2u, 2u, 7u, 107u)
#define PPUTLIMPL_UINT_TRAIT_2995u DEC, 0xBB3, 1, 11u, 54u, (5u, 599u)
#define PPUTLIMPL_UINT_TRAIT_2994u DEC, 0xBB2, 1, 11u, 54u, (2u, 3u, 499u)
#define PPUTLIMPL_UINT_TRAIT_2993u DEC, 0xBB1, 1, 11u, 54u, (41u, 73u)
#define PPUTLIMPL_UINT_TRAIT_2992u DEC, 0xBB0, 1, 11u, 54u, (2u, 2u, 2u, 2u, 11u, 17u)
#define PPUTLIMPL_UINT_TRAIT_2991u DEC, 0xBAF, 1, 11u, 54u, (3u, 997u)
#define PPUTLIMPL_UINT_TRAIT_2990u DEC, 0xBAE, 1, 11u, 54u, (2u, 5u, 13u, 23u)
#define PPUTLIMPL_UINT_TRAIT_2989u DEC, 0xBAD, 1, 11u, 54u, (7u, 7u, 61u)
#define PPUTLIMPL_UINT_TRAIT_2988u DEC, 0xBAC, 1, 11u, 54u, (2u, 2u, 3u, 3u, 83u)
#define PPUTLIMPL_UINT_TRAIT_2987u DEC, 0xBAB, 1, 11u, 54u, (29u, 103u)
#define PPUTLIMPL_UINT_TRAIT_2986u DEC, 0xBAA, 1, 11u, 54u, (2u, 1493u)
#define PPUTLIMPL_UINT_TRAIT_2985u DEC, 0xBA9, 1, 11u, 54u, (3u, 5u, 199u)
#define PPUTLIMPL_UINT_TRAIT_2984u DEC, 0xBA8, 1, 11u, 54u, (2u, 2u, 2u, 373u)
#define PPUTLIMPL_UINT_TRAIT_2983u DEC, 0xBA7, 1, 11u, 54u, (19u, 157u)
#define PPUTLIMPL_UINT_TRAIT_2982u DEC, 0xBA6, 1, 11u, 54u, (2u, 3u, 7u, 71u)
#define PPUTLIMPL_UINT_TRAIT_2981u DEC, 0xBA5, 1, 11u, 54u, (11u, 271u)
#define PPUTLIMPL_UINT_TRAIT_2980u DEC, 0xBA4, 1, 11u, 54u, (2u, 2u, 5u, 149u)
#define PPUTLIMPL_UINT_TRAIT_2979u DEC, 0xBA3, 1, 11u, 54u, (3u, 3u, 331u)
#define PPUTLIMPL_UINT_TRAIT_2978u DEC, 0xBA2, 1, 11u, 54u, (2u, 1489u)
#define PPUTLIMPL_UINT_TRAIT_2977u DEC, 0xBA1, 1, 11u, 54u, (13u, 229u)
#define PPUTLIMPL_UINT_TRAIT_2976u DEC, 0xBA0, 1, 11u, 54u, (2u, 2u, 2u, 2u, 2u, 3u, 31u)
#define PPUTLIMPL_UINT_TRAIT_2975u DEC, 0xB9F, 1, 11u, 54u, (5u, 5u, 7u, 17u)
#define PPUTLIMPL_UINT_TRAIT_2974u DEC, 0xB9E, 1, 11u, 54u, (2u, 1487u)
#define PPUTLIMPL_UINT_TRAIT_2973u DEC, 0xB9D, 1, 11u, 54u, (3u, 991u)
#define PPUTLIMPL_UINT_TRAIT_2972u DEC, 0xB9C, 1, 11u, 54u, (2u, 2u, 743u)
#define PPUTLIMPL_UINT_TRAIT_2971u DEC, 0xB9B, 1, 11u, 54u, ()
#define PPUTLIMPL_UINT_TRAIT_2970u DEC, 0xB9A, 1, 11u, 54u, (2u, 3u, 3u, 3u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_2969u DEC, 0xB99, 1, 11u, 54u, ()
#define PPUTLIMPL_UINT_TRAIT_2968u DEC, 0xB98, 1, 11u, 54u, (2u, 2u, 2u, 7u, 53u)
#define PPUTLIMPL_UINT_TRAIT_2967u DEC, 0xB97, 1, 11u, 54u, (3u, 23u, 43u)
#define PPUTLIMPL_UINT_TRAIT_2966u DEC, 0xB96, 1, 11u, 54u, (2u, 1483u)
#define PPUTLIMPL_UINT_TRAIT_2965u DEC, 0xB95, 1, 11u, 54u, (5u, 593u)
#define PPUTLIMPL_UINT_TRAIT_2964u DEC, 0xB94, 1, 11u, 54u, (2u, 2u, 3u, 13u, 19u)
#define PPUTLIMPL_UINT_TRAIT_2963u DEC, 0xB93, 1, 11u, 54u, ()
#define PPUTLIMPL_UINT_TRAIT_2962u DEC, 0xB92, 1, 11u, 54u, (2u, 1481u)
#define PPUTLIMPL_UINT_TRAIT_2961u DEC, 0xB91, 1, 11u, 54u, (3u, 3u, 7u, 47u)
#define PPUTLIMPL_UINT_TRAIT_2960u DEC, 0xB90, 1, 11u, 54u, (2u, 2u, 2u, 2u, 5u, 37u)
#define PPUTLIMPL_UINT_TRAIT_2959u DEC, 0xB8F, 1, 11u, 54u, (11u, 269u)
#define PPUTLIMPL_UINT_TRAIT_2958u DEC, 0xB8E, 1, 11u, 54u, (2u, 3u, 17u, 29u)
#define PPUTLIMPL_UINT_TRAIT_2957u DEC, 0xB8D, 1, 11u, 54u, ()
#define PPUTLIMPL_UINT_TRAIT_2956u DEC, 0xB8C, 1, 11u, 54u, (2u, 2u, 739u)
#define PPUTLIMPL_UINT_TRAIT_2955u DEC, 0xB8B, 1, 11u, 54u, (3u, 5u, 197u)
#define PPUTLIMPL_UINT_TRAIT_2954u DEC, 0xB8A, 1, 11u, 54u, (2u, 7u, 211u)
#define PPUTLIMPL_UINT_TRAIT_2953u DEC, 0xB89, 1, 11u, 54u, ()
#define PPUTLIMPL_UINT_TRAIT_2952u DEC, 0xB88, 1, 11u, 54u, (2u, 2u, 2u, 3u, 3u, 41u)
#define PPUTLIMPL_UINT_TRAIT_2951u DEC, 0xB87, 1, 11u, 54u, (13u, 227u)
#define PPUTLIMPL_UINT_TRAIT_2950u DEC, 0xB86, 1, 11u, 54u, (2u, 5u, 5u, 59u)
#define PPUTLIMPL_UINT_TRAIT_2949u DEC, 0xB85, 1, 11u, 54u, (3u, 983u)
#define PPUTLIMPL_UINT_TRAIT_2948u DEC, 0xB84, 1, 11u, 54u, (2u, 2u, 11u, 67u)
#define PPUTLIMPL_UINT_TRAIT_2947u DEC, 0xB83, 1, 11u, 54u, (7u, 421u)
#define PPUTLIMPL_UINT_TRAIT_2946u DEC, 0xB82, 1, 11u, 54u, (2u, 3u, 491u)
#define PPUTLIMPL_UINT_TRAIT_2945u DEC, 0xB81, 1, 11u, 54u, (5u, 19u, 31u)
#define PPUTLIMPL_UINT_TRAIT_2944u DEC, 0xB80, 1, 11u, 54u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 23u)
#define PPUTLIMPL_UINT_TRAIT_2943u DEC, 0xB7F, 1, 11u, 54u, (3u, 3u, 3u, 109u)
#define PPUTLIMPL_UINT_TRAIT_2942u DEC, 0xB7E, 1, 11u, 54u, (2u, 1471u)
#define PPUTLIMPL_UINT_TRAIT_2941u DEC, 0xB7D, 1, 11u, 54u, (17u, 173u)
#define PPUTLIMPL_UINT_TRAIT_2940u DEC, 0xB7C, 1, 11u, 54u, (2u, 2u, 3u, 5u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_2939u DEC, 0xB7B, 1, 11u, 54u, ()
#define PPUTLIMPL_UINT_TRAIT_2938u DEC, 0xB7A, 1, 11u, 54u, (2u, 13u, 113u)
#define PPUTLIMPL_UINT_TRAIT_2937u DEC, 0xB79, 1, 11u, 54u, (3u, 11u, 89u)
#define PPUTLIMPL_UINT_TRAIT_2936u DEC, 0xB78, 1, 11u, 54u, (2u, 2u, 2u, 367u)
#define PPUTLIMPL_UINT_TRAIT_2935u DEC, 0xB77, 1, 11u, 54u, (5u, 587u)
#define PPUTLIMPL_UINT_TRAIT_2934u DEC, 0xB76, 1, 11u, 54u, (2u, 3u, 3u, 163u)
#define PPUTLIMPL_UINT_TRAIT_2933u DEC, 0xB75, 1, 11u, 54u, (7u, 419u)
#define PPUTLIMPL_UINT_TRAIT_2932u DEC, 0xB74, 1, 11u, 54u, (2u, 2u, 733u)
#define PPUTLIMPL_UINT_TRAIT_2931u DEC, 0xB73, 1, 11u, 54u, (3u, 977u)
#define PPUTLIMPL_UINT_TRAIT_2930u DEC, 0xB72, 1, 11u, 54u, (2u, 5u, 293u)
#define PPUTLIMPL_UINT_TRAIT_2929u DEC, 0xB71, 1, 11u, 54u, (29u, 101u)
#define PPUTLIMPL_UINT_TRAIT_2928u DEC, 0xB70, 1, 11u, 54u, (2u, 2u, 2u, 2u, 3u, 61u)
#define PPUTLIMPL_UINT_TRAIT_2927u DEC, 0xB6F, 1, 11u, 54u, ()
#define PPUTLIMPL_UINT_TRAIT_2926u DEC, 0xB6E, 1, 11u, 54u, (2u, 7u, 11u, 19u)
#define PPUTLIMPL_UINT_TRAIT_2925u DEC, 0xB6D, 1, 11u, 54u, (3u, 3u, 5u, 5u, 13u)
#define PPUTLIMPL_UINT_TRAIT_2924u DEC, 0xB6C, 1, 11u, 54u, (2u, 2u, 17u, 43u)
#define PPUTLIMPL_UINT_TRAIT_2923u DEC, 0xB6B, 1, 11u, 54u, (37u, 79u)
#define PPUTLIMPL_UINT_TRAIT_2922u DEC, 0xB6A, 1, 11u, 54u, (2u, 3u, 487u)
#define PPUTLIMPL_UINT_TRAIT_2921u DEC, 0xB69, 1, 11u, 54u, (23u, 127u)
#define PPUTLIMPL_UINT_TRAIT_2920u DEC, 0xB68, 1, 11u, 54u, (2u, 2u, 2u, 5u, 73u)
#define PPUTLIMPL_UINT_TRAIT_2919u DEC, 0xB67, 1, 11u, 54u, (3u, 7u, 139u)
#define PPUTLIMPL_UINT_TRAIT_2918u DEC, 0xB66, 1, 11u, 54u, (2u, 1459u)
#define PPUTLIMPL_UINT_TRAIT_2917u DEC, 0xB65, 1, 11u, 54u, ()
#define PPUTLIMPL_UINT_TRAIT_2916u DEC, 0xB64, 1, 11u, 54u, (2u, 2u, 3u, 3u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_2915u DEC, 0xB63, 1, 11u, 53u, (5u, 11u, 53u)
#define PPUTLIMPL_UINT_TRAIT_2914u DEC, 0xB62, 1, 11u, 53u, (2u, 31u, 47u)
#define PPUTLIMPL_UINT_TRAIT_2913u DEC, 0xB61, 1, 11u, 53u, (3u, 971u)
#define PPUTLIMPL_UINT_TRAIT_2912u DEC, 0xB60, 1, 11u, 53u, (2u, 2u, 2u, 2u, 2u, 7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_2911u DEC, 0xB5F, 1, 11u, 53u, (41u, 71u)
#define PPUTLIMPL_UINT_TRAIT_2910u DEC, 0xB5E, 1, 11u, 53u, (2u, 3u, 5u, 97u)
#define PPUTLIMPL_UINT_TRAIT_2909u DEC, 0xB5D, 1, 11u, 53u, ()
#define PPUTLIMPL_UINT_TRAIT_2908u DEC, 0xB5C, 1, 11u, 53u, (2u, 2u, 727u)
#define PPUTLIMPL_UINT_TRAIT_2907u DEC, 0xB5B, 1, 11u, 53u, (3u, 3u, 17u, 19u)
#define PPUTLIMPL_UINT_TRAIT_2906u DEC, 0xB5A, 1, 11u, 53u, (2u, 1453u)
#define PPUTLIMPL_UINT_TRAIT_2905u DEC, 0xB59, 1, 11u, 53u, (5u, 7u, 83u)
#define PPUTLIMPL_UINT_TRAIT_2904u DEC, 0xB58, 1, 11u, 53u, (2u, 2u, 2u, 3u, 11u, 11u)
#define PPUTLIMPL_UINT_TRAIT_2903u DEC, 0xB57, 1, 11u, 53u, ()
#define PPUTLIMPL_UINT_TRAIT_2902u DEC, 0xB56, 1, 11u, 53u, (2u, 1451u)
#define PPUTLIMPL_UINT_TRAIT_2901u DEC, 0xB55, 1, 11u, 53u, (3u, 967u)
#define PPUTLIMPL_UINT_TRAIT_2900u DEC, 0xB54, 1, 11u, 53u, (2u, 2u, 5u, 5u, 29u)
#define PPUTLIMPL_UINT_TRAIT_2899u DEC, 0xB53, 1, 11u, 53u, (13u, 223u)
#define PPUTLIMPL_UINT_TRAIT_2898u DEC, 0xB52, 1, 11u, 53u, (2u, 3u, 3u, 7u, 23u)
#define PPUTLIMPL_UINT_TRAIT_2897u DEC, 0xB51, 1, 11u, 53u, ()
#define PPUTLIMPL_UINT_TRAIT_2896u DEC, 0xB50, 1, 11u, 53u, (2u, 2u, 2u, 2u, 181u)
#define PPUTLIMPL_UINT_TRAIT_2895u DEC, 0xB4F, 1, 11u, 53u, (3u, 5u, 193u)
#define PPUTLIMPL_UINT_TRAIT_2894u DEC, 0xB4E, 1, 11u, 53u, (2u, 1447u)
#define PPUTLIMPL_UINT_TRAIT_2893u DEC, 0xB4D, 1, 11u, 53u, (11u, 263u)
#define PPUTLIMPL_UINT_TRAIT_2892u DEC, 0xB4C, 1, 11u, 53u, (2u, 2u, 3u, 241u)
#define PPUTLIMPL_UINT_TRAIT_2891u DEC, 0xB4B, 1, 11u, 53u, (7u, 7u, 59u)
#define PPUTLIMPL_UINT_TRAIT_2890u DEC, 0xB4A, 1, 11u, 53u, (2u, 5u, 17u, 17u)
#define PPUTLIMPL_UINT_TRAIT_2889u DEC, 0xB49, 1, 11u, 53u, (3u, 3u, 3u, 107u)
#define PPUTLIMPL_UINT_TRAIT_2888u DEC, 0xB48, 1, 11u, 53u, (2u, 2u, 2u, 19u, 19u)
#define PPUTLIMPL_UINT_TRAIT_2887u DEC, 0xB47, 1, 11u, 53u, ()
#define PPUTLIMPL_UINT_TRAIT_2886u DEC, 0xB46, 1, 11u, 53u, (2u, 3u, 13u, 37u)
#define PPUTLIMPL_UINT_TRAIT_2885u DEC, 0xB45, 1, 11u, 53u, (5u, 577u)
#define PPUTLIMPL_UINT_TRAIT_2884u DEC, 0xB44, 1, 11u, 53u, (2u, 2u, 7u, 103u)
#define PPUTLIMPL_UINT_TRAIT_2883u DEC, 0xB43, 1, 11u, 53u, (3u, 31u, 31u)
#define PPUTLIMPL_UINT_TRAIT_2882u DEC, 0xB42, 1, 11u, 53u, (2u, 11u, 131u)
#define PPUTLIMPL_UINT_TRAIT_2881u DEC, 0xB41, 1, 11u, 53u, (43u, 67u)
#define PPUTLIMPL_UINT_TRAIT_2880u DEC, 0xB40, 1, 11u, 53u, (2u, 2u, 2u, 2u, 2u, 2u, 3u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_2879u DEC, 0xB3F, 1, 11u, 53u, ()
#define PPUTLIMPL_UINT_TRAIT_2878u DEC, 0xB3E, 1, 11u, 53u, (2u, 1439u)
#define PPUTLIMPL_UINT_TRAIT_2877u DEC, 0xB3D, 1, 11u, 53u, (3u, 7u, 137u)
#define PPUTLIMPL_UINT_TRAIT_2876u DEC, 0xB3C, 1, 11u, 53u, (2u, 2u, 719u)
#define PPUTLIMPL_UINT_TRAIT_2875u DEC, 0xB3B, 1, 11u, 53u, (5u, 5u, 5u, 23u)
#define PPUTLIMPL_UINT_TRAIT_2874u DEC, 0xB3A, 1, 11u, 53u, (2u, 3u, 479u)
#define PPUTLIMPL_UINT_TRAIT_2873u DEC, 0xB39, 1, 11u, 53u, (13u, 13u, 17u)
#define PPUTLIMPL_UINT_TRAIT_2872u DEC, 0xB38, 1, 11u, 53u, (2u, 2u, 2u, 359u)
#define PPUTLIMPL_UINT_TRAIT_2871u DEC, 0xB37, 1, 11u, 53u, (3u, 3u, 11u, 29u)
#define PPUTLIMPL_UINT_TRAIT_2870u DEC, 0xB36, 1, 11u, 53u, (2u, 5u, 7u, 41u)
#define PPUTLIMPL_UINT_TRAIT_2869u DEC, 0xB35, 1, 11u, 53u, (19u, 151u)
#define PPUTLIMPL_UINT_TRAIT_2868u DEC, 0xB34, 1, 11u, 53u, (2u, 2u, 3u, 239u)
#define PPUTLIMPL_UINT_TRAIT_2867u DEC, 0xB33, 1, 11u, 53u, (47u, 61u)
#define PPUTLIMPL_UINT_TRAIT_2866u DEC, 0xB32, 1, 11u, 53u, (2u, 1433u)
#define PPUTLIMPL_UINT_TRAIT_2865u DEC, 0xB31, 1, 11u, 53u, (3u, 5u, 191u)
#define PPUTLIMPL_UINT_TRAIT_2864u DEC, 0xB30, 1, 11u, 53u, (2u, 2u, 2u, 2u, 179u)
#define PPUTLIMPL_UINT_TRAIT_2863u DEC, 0xB2F, 1, 11u, 53u, (7u, 409u)
#define PPUTLIMPL_UINT_TRAIT_2862u DEC, 0xB2E, 1, 11u, 53u, (2u, 3u, 3u, 3u, 53u)
#define PPUTLIMPL_UINT_TRAIT_2861u DEC, 0xB2D, 1, 11u, 53u, ()
#define PPUTLIMPL_UINT_TRAIT_2860u DEC, 0xB2C, 1, 11u, 53u, (2u, 2u, 5u, 11u, 13u)
#define PPUTLIMPL_UINT_TRAIT_2859u DEC, 0xB2B, 1, 11u, 53u, (3u, 953u)
#define PPUTLIMPL_UINT_TRAIT_2858u DEC, 0xB2A, 1, 11u, 53u, (2u, 1429u)
#define PPUTLIMPL_UINT_TRAIT_2857u DEC, 0xB29, 1, 11u, 53u, ()
#define PPUTLIMPL_UINT_TRAIT_2856u DEC, 0xB28, 1, 11u, 53u, (2u, 2u, 2u, 3u, 7u, 17u)
#define PPUTLIMPL_UINT_TRAIT_2855u DEC, 0xB27, 1, 11u, 53u, (5u, 571u)
#define PPUTLIMPL_UINT_TRAIT_2854u DEC, 0xB26, 1, 11u, 53u, (2u, 1427u)
#define PPUTLIMPL_UINT_TRAIT_2853u DEC, 0xB25, 1, 11u, 53u, (3u, 3u, 317u)
#define PPUTLIMPL_UINT_TRAIT_2852u DEC, 0xB24, 1, 11u, 53u, (2u, 2u, 23u, 31u)
#define PPUTLIMPL_UINT_TRAIT_2851u DEC, 0xB23, 1, 11u, 53u, ()
#define PPUTLIMPL_UINT_TRAIT_2850u DEC, 0xB22, 1, 11u, 53u, (2u, 3u, 5u, 5u, 19u)
#define PPUTLIMPL_UINT_TRAIT_2849u DEC, 0xB21, 1, 11u, 53u, (7u, 11u, 37u)
#define PPUTLIMPL_UINT_TRAIT_2848u DEC, 0xB20, 1, 11u, 53u, (2u, 2u, 2u, 2u, 2u, 89u)
#define PPUTLIMPL_UINT_TRAIT_2847u DEC, 0xB1F, 1, 11u, 53u, (3u, 13u, 73u)
#define PPUTLIMPL_UINT_TRAIT_2846u DEC, 0xB1E, 1, 11u, 53u, (2u, 1423u)
#define PPUTLIMPL_UINT_TRAIT_2845u DEC, 0xB1D, 1, 11u, 53u, (5u, 569u)
#define PPUTLIMPL_UINT_TRAIT_2844u DEC, 0xB1C, 1, 11u, 53u, (2u, 2u, 3u, 3u, 79u)
#define PPUTLIMPL_UINT_TRAIT_2843u DEC, 0xB1B, 1, 11u, 53u, ()
#define PPUTLIMPL_UINT_TRAIT_2842u DEC, 0xB1A, 1, 11u, 53u, (2u, 7u, 7u, 29u)
#define PPUTLIMPL_UINT_TRAIT_2841u DEC, 0xB19, 1, 11u, 53u, (3u, 947u)
#define PPUTLIMPL_UINT_TRAIT_2840u DEC, 0xB18, 1, 11u, 53u, (2u, 2u, 2u, 5u, 71u)
#define PPUTLIMPL_UINT_TRAIT_2839u DEC, 0xB17, 1, 11u, 53u, (17u, 167u)
#define PPUTLIMPL_UINT_TRAIT_2838u DEC, 0xB16, 1, 11u, 53u, (2u, 3u, 11u, 43u)
#define PPUTLIMPL_UINT_TRAIT_2837u DEC, 0xB15, 1, 11u, 53u, ()
#define PPUTLIMPL_UINT_TRAIT_2836u DEC, 0xB14, 1, 11u, 53u, (2u, 2u, 709u)
#define PPUTLIMPL_UINT_TRAIT_2835u DEC, 0xB13, 1, 11u, 53u, (3u, 3u, 3u, 3u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_2834u DEC, 0xB12, 1, 11u, 53u, (2u, 13u, 109u)
#define PPUTLIMPL_UINT_TRAIT_2833u DEC, 0xB11, 1, 11u, 53u, ()
#define PPUTLIMPL_UINT_TRAIT_2832u DEC, 0xB10, 1, 11u, 53u, (2u, 2u, 2u, 2u, 3u, 59u)
#define PPUTLIMPL_UINT_TRAIT_2831u DEC, 0xB0F, 1, 11u, 53u, (19u, 149u)
#define PPUTLIMPL_UINT_TRAIT_2830u DEC, 0xB0E, 1, 11u, 53u, (2u, 5u, 283u)
#define PPUTLIMPL_UINT_TRAIT_2829u DEC, 0xB0D, 1, 11u, 53u, (3u, 23u, 41u)
#define PPUTLIMPL_UINT_TRAIT_2828u DEC, 0xB0C, 1, 11u, 53u, (2u, 2u, 7u, 101u)
#define PPUTLIMPL_UINT_TRAIT_2827u DEC, 0xB0B, 1, 11u, 53u, (11u, 257u)
#define PPUTLIMPL_UINT_TRAIT_2826u DEC, 0xB0A, 1, 11u, 53u, (2u, 3u, 3u, 157u)
#define PPUTLIMPL_UINT_TRAIT_2825u DEC, 0xB09, 1, 11u, 53u, (5u, 5u, 113u)
#define PPUTLIMPL_UINT_TRAIT_2824u DEC, 0xB08, 1, 11u, 53u, (2u, 2u, 2u, 353u)
#define PPUTLIMPL_UINT_TRAIT_2823u DEC, 0xB07, 1, 11u, 53u, (3u, 941u)
#define PPUTLIMPL_UINT_TRAIT_2822u DEC, 0xB06, 1, 11u, 53u, (2u, 17u, 83u)
#define PPUTLIMPL_UINT_TRAIT_2821u DEC, 0xB05, 1, 11u, 53u, (7u, 13u, 31u)
#define PPUTLIMPL_UINT_TRAIT_2820u DEC, 0xB04, 1, 11u, 53u, (2u, 2u, 3u, 5u, 47u)
#define PPUTLIMPL_UINT_TRAIT_2819u DEC, 0xB03, 1, 11u, 53u, ()
#define PPUTLIMPL_UINT_TRAIT_2818u DEC, 0xB02, 1, 11u, 53u, (2u, 1409u)
#define PPUTLIMPL_UINT_TRAIT_2817u DEC, 0xB01, 1, 11u, 53u, (3u, 3u, 313u)
#define PPUTLIMPL_UINT_TRAIT_2816u DEC, 0xB00, 1, 11u, 53u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 11u)
#define PPUTLIMPL_UINT_TRAIT_2815u DEC, 0xAFF, 1, 11u, 53u, (5u, 563u)
#define PPUTLIMPL_UINT_TRAIT_2814u DEC, 0xAFE, 1, 11u, 53u, (2u, 3u, 7u, 67u)
#define PPUTLIMPL_UINT_TRAIT_2813u DEC, 0xAFD, 1, 11u, 53u, (29u, 97u)
#define PPUTLIMPL_UINT_TRAIT_2812u DEC, 0xAFC, 1, 11u, 53u, (2u, 2u, 19u, 37u)
#define PPUTLIMPL_UINT_TRAIT_2811u DEC, 0xAFB, 1, 11u, 53u, (3u, 937u)
#define PPUTLIMPL_UINT_TRAIT_2810u DEC, 0xAFA, 1, 11u, 53u, (2u, 5u, 281u)
#define PPUTLIMPL_UINT_TRAIT_2809u DEC, 0xAF9, 1, 11u, 53u, (53u, 53u)
#define PPUTLIMPL_UINT_TRAIT_2808u DEC, 0xAF8, 1, 11u, 52u, (2u, 2u, 2u, 3u, 3u, 3u, 13u)
#define PPUTLIMPL_UINT_TRAIT_2807u DEC, 0xAF7, 1, 11u, 52u, (7u, 401u)
#define PPUTLIMPL_UINT_TRAIT_2806u DEC, 0xAF6, 1, 11u, 52u, (2u, 23u, 61u)
#define PPUTLIMPL_UINT_TRAIT_2805u DEC, 0xAF5, 1, 11u, 52u, (3u, 5u, 11u, 17u)
#define PPUTLIMPL_UINT_TRAIT_2804u DEC, 0xAF4, 1, 11u, 52u, (2u, 2u, 701u)
#define PPUTLIMPL_UINT_TRAIT_2803u DEC, 0xAF3, 1, 11u, 52u, ()
#define PPUTLIMPL_UINT_TRAIT_2802u DEC, 0xAF2, 1, 11u, 52u, (2u, 3u, 467u)
#define PPUTLIMPL_UINT_TRAIT_2801u DEC, 0xAF1, 1, 11u, 52u, ()
#define PPUTLIMPL_UINT_TRAIT_2800u DEC, 0xAF0, 1, 11u, 52u, (2u, 2u, 2u, 2u, 5u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_2799u DEC, 0xAEF, 1, 11u, 52u, (3u, 3u, 311u)
#define PPUTLIMPL_UINT_TRAIT_2798u DEC, 0xAEE, 1, 11u, 52u, (2u, 1399u)
#define PPUTLIMPL_UINT_TRAIT_2797u DEC, 0xAED, 1, 11u, 52u, ()
#define PPUTLIMPL_UINT_TRAIT_2796u DEC, 0xAEC, 1, 11u, 52u, (2u, 2u, 3u, 233u)
#define PPUTLIMPL_UINT_TRAIT_2795u DEC, 0xAEB, 1, 11u, 52u, (5u, 13u, 43u)
#define PPUTLIMPL_UINT_TRAIT_2794u DEC, 0xAEA, 1, 11u, 52u, (2u, 11u, 127u)
#define PPUTLIMPL_UINT_TRAIT_2793u DEC, 0xAE9, 1, 11u, 52u, (3u, 7u, 7u, 19u)
#define PPUTLIMPL_UINT_TRAIT_2792u DEC, 0xAE8, 1, 11u, 52u, (2u, 2u, 2u, 349u)
#define PPUTLIMPL_UINT_TRAIT_2791u DEC, 0xAE7, 1, 11u, 52u, ()
#define PPUTLIMPL_UINT_TRAIT_2790u DEC, 0xAE6, 1, 11u, 52u, (2u, 3u, 3u, 5u, 31u)
#define PPUTLIMPL_UINT_TRAIT_2789u DEC, 0xAE5, 1, 11u, 52u, ()
#define PPUTLIMPL_UINT_TRAIT_2788u DEC, 0xAE4, 1, 11u, 52u, (2u, 2u, 17u, 41u)
#define PPUTLIMPL_UINT_TRAIT_2787u DEC, 0xAE3, 1, 11u, 52u, (3u, 929u)
#define PPUTLIMPL_UINT_TRAIT_2786u DEC, 0xAE2, 1, 11u, 52u, (2u, 7u, 199u)
#define PPUTLIMPL_UINT_TRAIT_2785u DEC, 0xAE1, 1, 11u, 52u, (5u, 557u)
#define PPUTLIMPL_UINT_TRAIT_2784u DEC, 0xAE0, 1, 11u, 52u, (2u, 2u, 2u, 2u, 2u, 3u, 29u)
#define PPUTLIMPL_UINT_TRAIT_2783u DEC, 0xADF, 1, 11u, 52u, (11u, 11u, 23u)
#define PPUTLIMPL_UINT_TRAIT_2782u DEC, 0xADE, 1, 11u, 52u, (2u, 13u, 107u)
#define PPUTLIMPL_UINT_TRAIT_2781u DEC, 0xADD, 1, 11u, 52u, (3u, 3u, 3u, 103u)
#define PPUTLIMPL_UINT_TRAIT_2780u DEC, 0xADC, 1, 11u, 52u, (2u, 2u, 5u, 139u)
#define PPUTLIMPL_UINT_TRAIT_2779u DEC, 0xADB, 1, 11u, 52u, (7u, 397u)
#define PPUTLIMPL_UINT_TRAIT_2778u DEC, 0xADA, 1, 11u, 52u, (2u, 3u, 463u)
#define PPUTLIMPL_UINT_TRAIT_2777u DEC, 0xAD9, 1, 11u, 52u, ()
#define PPUTLIMPL_UINT_TRAIT_2776u DEC, 0xAD8, 1, 11u, 52u, (2u, 2u, 2u, 347u)
#define PPUTLIMPL_UINT_TRAIT_2775u DEC, 0xAD7, 1, 11u, 52u, (3u, 5u, 5u, 37u)
#define PPUTLIMPL_UINT_TRAIT_2774u DEC, 0xAD6, 1, 11u, 52u, (2u, 19u, 73u)
#define PPUTLIMPL_UINT_TRAIT_2773u DEC, 0xAD5, 1, 11u, 52u, (47u, 59u)
#define PPUTLIMPL_UINT_TRAIT_2772u DEC, 0xAD4, 1, 11u, 52u, (2u, 2u, 3u, 3u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_2771u DEC, 0xAD3, 1, 11u, 52u, (17u, 163u)
#define PPUTLIMPL_UINT_TRAIT_2770u DEC, 0xAD2, 1, 11u, 52u, (2u, 5u, 277u)
#define PPUTLIMPL_UINT_TRAIT_2769u DEC, 0xAD1, 1, 11u, 52u, (3u, 13u, 71u)
#define PPUTLIMPL_UINT_TRAIT_2768u DEC, 0xAD0, 1, 11u, 52u, (2u, 2u, 2u, 2u, 173u)
#define PPUTLIMPL_UINT_TRAIT_2767u DEC, 0xACF, 1, 11u, 52u, ()
#define PPUTLIMPL_UINT_TRAIT_2766u DEC, 0xACE, 1, 11u, 52u, (2u, 3u, 461u)
#define PPUTLIMPL_UINT_TRAIT_2765u DEC, 0xACD, 1, 11u, 52u, (5u, 7u, 79u)
#define PPUTLIMPL_UINT_TRAIT_2764u DEC, 0xACC, 1, 11u, 52u, (2u, 2u, 691u)
#define PPUTLIMPL_UINT_TRAIT_2763u DEC, 0xACB, 1, 11u, 52u, (3u, 3u, 307u)
#define PPUTLIMPL_UINT_TRAIT_2762u DEC, 0xACA, 1, 11u, 52u, (2u, 1381u)
#define PPUTLIMPL_UINT_TRAIT_2761u DEC, 0xAC9, 1, 11u, 52u, (11u, 251u)
#define PPUTLIMPL_UINT_TRAIT_2760u DEC, 0xAC8, 1, 11u, 52u, (2u, 2u, 2u, 3u, 5u, 23u)
#define PPUTLIMPL_UINT_TRAIT_2759u DEC, 0xAC7, 1, 11u, 52u, (31u, 89u)
#define PPUTLIMPL_UINT_TRAIT_2758u DEC, 0xAC6, 1, 11u, 52u, (2u, 7u, 197u)
#define PPUTLIMPL_UINT_TRAIT_2757u DEC, 0xAC5, 1, 11u, 52u, (3u, 919u)
#define PPUTLIMPL_UINT_TRAIT_2756u DEC, 0xAC4, 1, 11u, 52u, (2u, 2u, 13u, 53u)
#define PPUTLIMPL_UINT_TRAIT_2755u DEC, 0xAC3, 1, 11u, 52u, (5u, 19u, 29u)
#define PPUTLIMPL_UINT_TRAIT_2754u DEC, 0xAC2, 1, 11u, 52u, (2u, 3u, 3u, 3u, 3u, 17u)
#define PPUTLIMPL_UINT_TRAIT_2753u DEC, 0xAC1, 1, 11u, 52u, ()
#define PPUTLIMPL_UINT_TRAIT_2752u DEC, 0xAC0, 1, 11u, 52u, (2u, 2u, 2u, 2u, 2u, 2u, 43u)
#define PPUTLIMPL_UINT_TRAIT_2751u DEC, 0xABF, 1, 11u, 52u, (3u, 7u, 131u)
#define PPUTLIMPL_UINT_TRAIT_2750u DEC, 0xABE, 1, 11u, 52u, (2u, 5u, 5u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_2749u DEC, 0xABD, 1, 11u, 52u, ()
#define PPUTLIMPL_UINT_TRAIT_2748u DEC, 0xABC, 1, 11u, 52u, (2u, 2u, 3u, 229u)
#define PPUTLIMPL_UINT_TRAIT_2747u DEC, 0xABB, 1, 11u, 52u, (41u, 67u)
#define PPUTLIMPL_UINT_TRAIT_2746u DEC, 0xABA, 1, 11u, 52u, (2u, 1373u)
#define PPUTLIMPL_UINT_TRAIT_2745u DEC, 0xAB9, 1, 11u, 52u, (3u, 3u, 5u, 61u)
#define PPUTLIMPL_UINT_TRAIT_2744u DEC, 0xAB8, 1, 11u, 52u, (2u, 2u, 2u, 7u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_2743u DEC, 0xAB7, 1, 11u, 52u, (13u, 211u)
#define PPUTLIMPL_UINT_TRAIT_2742u DEC, 0xAB6, 1, 11u, 52u, (2u, 3u, 457u)
#define PPUTLIMPL_UINT_TRAIT_2741u DEC, 0xAB5, 1, 11u, 52u, ()
#define PPUTLIMPL_UINT_TRAIT_2740u DEC, 0xAB4, 1, 11u, 52u, (2u, 2u, 5u, 137u)
#define PPUTLIMPL_UINT_TRAIT_2739u DEC, 0xAB3, 1, 11u, 52u, (3u, 11u, 83u)
#define PPUTLIMPL_UINT_TRAIT_2738u DEC, 0xAB2, 1, 11u, 52u, (2u, 37u, 37u)
#define PPUTLIMPL_UINT_TRAIT_2737u DEC, 0xAB1, 1, 11u, 52u, (7u, 17u, 23u)
#define PPUTLIMPL_UINT_TRAIT_2736u DEC, 0xAB0, 1, 11u, 52u, (2u, 2u, 2u, 2u, 3u, 3u, 19u)
#define PPUTLIMPL_UINT_TRAIT_2735u DEC, 0xAAF, 1, 11u, 52u, (5u, 547u)
#define PPUTLIMPL_UINT_TRAIT_2734u DEC, 0xAAE, 1, 11u, 52u, (2u, 1367u)
#define PPUTLIMPL_UINT_TRAIT_2733u DEC, 0xAAD, 1, 11u, 52u, (3u, 911u)
#define PPUTLIMPL_UINT_TRAIT_2732u DEC, 0xAAC, 1, 11u, 52u, (2u, 2u, 683u)
#define PPUTLIMPL_UINT_TRAIT_2731u DEC, 0xAAB, 1, 11u, 52u, ()
#define PPUTLIMPL_UINT_TRAIT_2730u DEC, 0xAAA, 1, 11u, 52u, (2u, 3u, 5u, 7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_2729u DEC, 0xAA9, 1, 11u, 52u, ()
#define PPUTLIMPL_UINT_TRAIT_2728u DEC, 0xAA8, 1, 11u, 52u, (2u, 2u, 2u, 11u, 31u)
#define PPUTLIMPL_UINT_TRAIT_2727u DEC, 0xAA7, 1, 11u, 52u, (3u, 3u, 3u, 101u)
#define PPUTLIMPL_UINT_TRAIT_2726u DEC, 0xAA6, 1, 11u, 52u, (2u, 29u, 47u)
#define PPUTLIMPL_UINT_TRAIT_2725u DEC, 0xAA5, 1, 11u, 52u, (5u, 5u, 109u)
#define PPUTLIMPL_UINT_TRAIT_2724u DEC, 0xAA4, 1, 11u, 52u, (2u, 2u, 3u, 227u)
#define PPUTLIMPL_UINT_TRAIT_2723u DEC, 0xAA3, 1, 11u, 52u, (7u, 389u)
#define PPUTLIMPL_UINT_TRAIT_2722u DEC, 0xAA2, 1, 11u, 52u, (2u, 1361u)
#define PPUTLIMPL_UINT_TRAIT_2721u DEC, 0xAA1, 1, 11u, 52u, (3u, 907u)
#define PPUTLIMPL_UINT_TRAIT_2720u DEC, 0xAA0, 1, 11u, 52u, (2u, 2u, 2u, 2u, 2u, 5u, 17u)
#define PPUTLIMPL_UINT_TRAIT_2719u DEC, 0xA9F, 1, 11u, 52u, ()
#define PPUTLIMPL_UINT_TRAIT_2718u DEC, 0xA9E, 1, 11u, 52u, (2u, 3u, 3u, 151u)
#define PPUTLIMPL_UINT_TRAIT_2717u DEC, 0xA9D, 1, 11u, 52u, (11u, 13u, 19u)
#define PPUTLIMPL_UINT_TRAIT_2716u DEC, 0xA9C, 1, 11u, 52u, (2u, 2u, 7u, 97u)
#define PPUTLIMPL_UINT_TRAIT_2715u DEC, 0xA9B, 1, 11u, 52u, (3u, 5u, 181u)
#define PPUTLIMPL_UINT_TRAIT_2714u DEC, 0xA9A, 1, 11u, 52u, (2u, 23u, 59u)
#define PPUTLIMPL_UINT_TRAIT_2713u DEC, 0xA99, 1, 11u, 52u, ()
#define PPUTLIMPL_UINT_TRAIT_2712u DEC, 0xA98, 1, 11u, 52u, (2u, 2u, 2u, 3u, 113u)
#define PPUTLIMPL_UINT_TRAIT_2711u DEC, 0xA97, 1, 11u, 52u, ()
#define PPUTLIMPL_UINT_TRAIT_2710u DEC, 0xA96, 1, 11u, 52u, (2u, 5u, 271u)
#define PPUTLIMPL_UINT_TRAIT_2709u DEC, 0xA95, 1, 11u, 52u, (3u, 3u, 7u, 43u)
#define PPUTLIMPL_UINT_TRAIT_2708u DEC, 0xA94, 1, 11u, 52u, (2u, 2u, 677u)
#define PPUTLIMPL_UINT_TRAIT_2707u DEC, 0xA93, 1, 11u, 52u, ()
#define PPUTLIMPL_UINT_TRAIT_2706u DEC, 0xA92, 1, 11u, 52u, (2u, 3u, 11u, 41u)
#define PPUTLIMPL_UINT_TRAIT_2705u DEC, 0xA91, 1, 11u, 52u, (5u, 541u)
#define PPUTLIMPL_UINT_TRAIT_2704u DEC, 0xA90, 1, 11u, 52u, (2u, 2u, 2u, 2u, 13u, 13u)
#define PPUTLIMPL_UINT_TRAIT_2703u DEC, 0xA8F, 1, 11u, 51u, (3u, 17u, 53u)
#define PPUTLIMPL_UINT_TRAIT_2702u DEC, 0xA8E, 1, 11u, 51u, (2u, 7u, 193u)
#define PPUTLIMPL_UINT_TRAIT_2701u DEC, 0xA8D, 1, 11u, 51u, (37u, 73u)
#define PPUTLIMPL_UINT_TRAIT_2700u DEC, 0xA8C, 1, 11u, 51u, (2u, 2u, 3u, 3u, 3u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_2699u DEC, 0xA8B, 1, 11u, 51u, ()
#define PPUTLIMPL_UINT_TRAIT_2698u DEC, 0xA8A, 1, 11u, 51u, (2u, 19u, 71u)
#define PPUTLIMPL_UINT_TRAIT_2697u DEC, 0xA89, 1, 11u, 51u, (3u, 29u, 31u)
#define PPUTLIMPL_UINT_TRAIT_2696u DEC, 0xA88, 1, 11u, 51u, (2u, 2u, 2u, 337u)
#define PPUTLIMPL_UINT_TRAIT_2695u DEC, 0xA87, 1, 11u, 51u, (5u, 7u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_2694u DEC, 0xA86, 1, 11u, 51u, (2u, 3u, 449u)
#define PPUTLIMPL_UINT_TRAIT_2693u DEC, 0xA85, 1, 11u, 51u, ()
#define PPUTLIMPL_UINT_TRAIT_2692u DEC, 0xA84, 1, 11u, 51u, (2u, 2u, 673u)
#define PPUTLIMPL_UINT_TRAIT_2691u DEC, 0xA83, 1, 11u, 51u, (3u, 3u, 13u, 23u)
#define PPUTLIMPL_UINT_TRAIT_2690u DEC, 0xA82, 1, 11u, 51u, (2u, 5u, 269u)
#define PPUTLIMPL_UINT_TRAIT_2689u DEC, 0xA81, 1, 11u, 51u, ()
#define PPUTLIMPL_UINT_TRAIT_2688u DEC, 0xA80, 1, 11u, 51u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_2687u DEC, 0xA7F, 1, 11u, 51u, ()
#define PPUTLIMPL_UINT_TRAIT_2686u DEC, 0xA7E, 1, 11u, 51u, (2u, 17u, 79u)
#define PPUTLIMPL_UINT_TRAIT_2685u DEC, 0xA7D, 1, 11u, 51u, (3u, 5u, 179u)
#define PPUTLIMPL_UINT_TRAIT_2684u DEC, 0xA7C, 1, 11u, 51u, (2u, 2u, 11u, 61u)
#define PPUTLIMPL_UINT_TRAIT_2683u DEC, 0xA7B, 1, 11u, 51u, ()
#define PPUTLIMPL_UINT_TRAIT_2682u DEC, 0xA7A, 1, 11u, 51u, (2u, 3u, 3u, 149u)
#define PPUTLIMPL_UINT_TRAIT_2681u DEC, 0xA79, 1, 11u, 51u, (7u, 383u)
#define PPUTLIMPL_UINT_TRAIT_2680u DEC, 0xA78, 1, 11u, 51u, (2u, 2u, 2u, 5u, 67u)
#define PPUTLIMPL_UINT_TRAIT_2679u DEC, 0xA77, 1, 11u, 51u, (3u, 19u, 47u)
#define PPUTLIMPL_UINT_TRAIT_2678u DEC, 0xA76, 1, 11u, 51u, (2u, 13u, 103u)
#define PPUTLIMPL_UINT_TRAIT_2677u DEC, 0xA75, 1, 11u, 51u, ()
#define PPUTLIMPL_UINT_TRAIT_2676u DEC, 0xA74, 1, 11u, 51u, (2u, 2u, 3u, 223u)
#define PPUTLIMPL_UINT_TRAIT_2675u DEC, 0xA73, 1, 11u, 51u, (5u, 5u, 107u)
#define PPUTLIMPL_UINT_TRAIT_2674u DEC, 0xA72, 1, 11u, 51u, (2u, 7u, 191u)
#define PPUTLIMPL_UINT_TRAIT_2673u DEC, 0xA71, 1, 11u, 51u, (3u, 3u, 3u, 3u, 3u, 11u)
#define PPUTLIMPL_UINT_TRAIT_2672u DEC, 0xA70, 1, 11u, 51u, (2u, 2u, 2u, 2u, 167u)
#define PPUTLIMPL_UINT_TRAIT_2671u DEC, 0xA6F, 1, 11u, 51u, ()
#define PPUTLIMPL_UINT_TRAIT_2670u DEC, 0xA6E, 1, 11u, 51u, (2u, 3u, 5u, 89u)
#define PPUTLIMPL_UINT_TRAIT_2669u DEC, 0xA6D, 1, 11u, 51u, (17u, 157u)
#define PPUTLIMPL_UINT_TRAIT_2668u DEC, 0xA6C, 1, 11u, 51u, (2u, 2u, 23u, 29u)
#define PPUTLIMPL_UINT_TRAIT_2667u DEC, 0xA6B, 1, 11u, 51u, (3u, 7u, 127u)
#define PPUTLIMPL_UINT_TRAIT_2666u DEC, 0xA6A, 1, 11u, 51u, (2u, 31u, 43u)
#define PPUTLIMPL_UINT_TRAIT_2665u DEC, 0xA69, 1, 11u, 51u, (5u, 13u, 41u)
#define PPUTLIMPL_UINT_TRAIT_2664u DEC, 0xA68, 1, 11u, 51u, (2u, 2u, 2u, 3u, 3u, 37u)
#define PPUTLIMPL_UINT_TRAIT_2663u DEC, 0xA67, 1, 11u, 51u, ()
#define PPUTLIMPL_UINT_TRAIT_2662u DEC, 0xA66, 1, 11u, 51u, (2u, 11u, 11u, 11u)
#define PPUTLIMPL_UINT_TRAIT_2661u DEC, 0xA65, 1, 11u, 51u, (3u, 887u)
#define PPUTLIMPL_UINT_TRAIT_2660u DEC, 0xA64, 1, 11u, 51u, (2u, 2u, 5u, 7u, 19u)
#define PPUTLIMPL_UINT_TRAIT_2659u DEC, 0xA63, 1, 11u, 51u, ()
#define PPUTLIMPL_UINT_TRAIT_2658u DEC, 0xA62, 1, 11u, 51u, (2u, 3u, 443u)
#define PPUTLIMPL_UINT_TRAIT_2657u DEC, 0xA61, 1, 11u, 51u, ()
#define PPUTLIMPL_UINT_TRAIT_2656u DEC, 0xA60, 1, 11u, 51u, (2u, 2u, 2u, 2u, 2u, 83u)
#define PPUTLIMPL_UINT_TRAIT_2655u DEC, 0xA5F, 1, 11u, 51u, (3u, 3u, 5u, 59u)
#define PPUTLIMPL_UINT_TRAIT_2654u DEC, 0xA5E, 1, 11u, 51u, (2u, 1327u)
#define PPUTLIMPL_UINT_TRAIT_2653u DEC, 0xA5D, 1, 11u, 51u, (7u, 379u)
#define PPUTLIMPL_UINT_TRAIT_2652u DEC, 0xA5C, 1, 11u, 51u, (2u, 2u, 3u, 13u, 17u)
#define PPUTLIMPL_UINT_TRAIT_2651u DEC, 0xA5B, 1, 11u, 51u, (11u, 241u)
#define PPUTLIMPL_UINT_TRAIT_2650u DEC, 0xA5A, 1, 11u, 51u, (2u, 5u, 5u, 53u)
#define PPUTLIMPL_UINT_TRAIT_2649u DEC, 0xA59, 1, 11u, 51u, (3u, 883u)
#define PPUTLIMPL_UINT_TRAIT_2648u DEC, 0xA58, 1, 11u, 51u, (2u, 2u, 2u, 331u)
#define PPUTLIMPL_UINT_TRAIT_2647u DEC, 0xA57, 1, 11u, 51u, ()
#define PPUTLIMPL_UINT_TRAIT_2646u DEC, 0xA56, 1, 11u, 51u, (2u, 3u, 3u, 3u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_2645u DEC, 0xA55, 1, 11u, 51u, (5u, 23u, 23u)
#define PPUTLIMPL_UINT_TRAIT_2644u DEC, 0xA54, 1, 11u, 51u, (2u, 2u, 661u)
#define PPUTLIMPL_UINT_TRAIT_2643u DEC, 0xA53, 1, 11u, 51u, (3u, 881u)
#define PPUTLIMPL_UINT_TRAIT_2642u DEC, 0xA52, 1, 11u, 51u, (2u, 1321u)
#define PPUTLIMPL_UINT_TRAIT_2641u DEC, 0xA51, 1, 11u, 51u, (19u, 139u)
#define PPUTLIMPL_UINT_TRAIT_2640u DEC, 0xA50, 1, 11u, 51u, (2u, 2u, 2u, 2u, 3u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_2639u DEC, 0xA4F, 1, 11u, 51u, (7u, 13u, 29u)
#define PPUTLIMPL_UINT_TRAIT_2638u DEC, 0xA4E, 1, 11u, 51u, (2u, 1319u)
#define PPUTLIMPL_UINT_TRAIT_2637u DEC, 0xA4D, 1, 11u, 51u, (3u, 3u, 293u)
#define PPUTLIMPL_UINT_TRAIT_2636u DEC, 0xA4C, 1, 11u, 51u, (2u, 2u, 659u)
#define PPUTLIMPL_UINT_TRAIT_2635u DEC, 0xA4B, 1, 11u, 51u, (5u, 17u, 31u)
#define PPUTLIMPL_UINT_TRAIT_2634u DEC, 0xA4A, 1, 11u, 51u, (2u, 3u, 439u)
#define PPUTLIMPL_UINT_TRAIT_2633u DEC, 0xA49, 1, 11u, 51u, ()
#define PPUTLIMPL_UINT_TRAIT_2632u DEC, 0xA48, 1, 11u, 51u, (2u, 2u, 2u, 7u, 47u)
#define PPUTLIMPL_UINT_TRAIT_2631u DEC, 0xA47, 1, 11u, 51u, (3u, 877u)
#define PPUTLIMPL_UINT_TRAIT_2630u DEC, 0xA46, 1, 11u, 51u, (2u, 5u, 263u)
#define PPUTLIMPL_UINT_TRAIT_2629u DEC, 0xA45, 1, 11u, 51u, (11u, 239u)
#define PPUTLIMPL_UINT_TRAIT_2628u DEC, 0xA44, 1, 11u, 51u, (2u, 2u, 3u, 3u, 73u)
#define PPUTLIMPL_UINT_TRAIT_2627u DEC, 0xA43, 1, 11u, 51u, (37u, 71u)
#define PPUTLIMPL_UINT_TRAIT_2626u DEC, 0xA42, 1, 11u, 51u, (2u, 13u, 101u)
#define PPUTLIMPL_UINT_TRAIT_2625u DEC, 0xA41, 1, 11u, 51u, (3u, 5u, 5u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_2624u DEC, 0xA40, 1, 11u, 51u, (2u, 2u, 2u, 2u, 2u, 2u, 41u)
#define PPUTLIMPL_UINT_TRAIT_2623u DEC, 0xA3F, 1, 11u, 51u, (43u, 61u)
#define PPUTLIMPL_UINT_TRAIT_2622u DEC, 0xA3E, 1, 11u, 51u, (2u, 3u, 19u, 23u)
#define PPUTLIMPL_UINT_TRAIT_2621u DEC, 0xA3D, 1, 11u, 51u, ()
#define PPUTLIMPL_UINT_TRAIT_2620u DEC, 0xA3C, 1, 11u, 51u, (2u, 2u, 5u, 131u)
#define PPUTLIMPL_UINT_TRAIT_2619u DEC, 0xA3B, 1, 11u, 51u, (3u, 3u, 3u, 97u)
#define PPUTLIMPL_UINT_TRAIT_2618u DEC, 0xA3A, 1, 11u, 51u, (2u, 7u, 11u, 17u)
#define PPUTLIMPL_UINT_TRAIT_2617u DEC, 0xA39, 1, 11u, 51u, ()
#define PPUTLIMPL_UINT_TRAIT_2616u DEC, 0xA38, 1, 11u, 51u, (2u, 2u, 2u, 3u, 109u)
#define PPUTLIMPL_UINT_TRAIT_2615u DEC, 0xA37, 1, 11u, 51u, (5u, 523u)
#define PPUTLIMPL_UINT_TRAIT_2614u DEC, 0xA36, 1, 11u, 51u, (2u, 1307u)
#define PPUTLIMPL_UINT_TRAIT_2613u DEC, 0xA35, 1, 11u, 51u, (3u, 13u, 67u)
#define PPUTLIMPL_UINT_TRAIT_2612u DEC, 0xA34, 1, 11u, 51u, (2u, 2u, 653u)
#define PPUTLIMPL_UINT_TRAIT_2611u DEC, 0xA33, 1, 11u, 51u, (7u, 373u)
#define PPUTLIMPL_UINT_TRAIT_2610u DEC, 0xA32, 1, 11u, 51u, (2u, 3u, 3u, 5u, 29u)
#define PPUTLIMPL_UINT_TRAIT_2609u DEC, 0xA31, 1, 11u, 51u, ()
#define PPUTLIMPL_UINT_TRAIT_2608u DEC, 0xA30, 1, 11u, 51u, (2u, 2u, 2u, 2u, 163u)
#define PPUTLIMPL_UINT_TRAIT_2607u DEC, 0xA2F, 1, 11u, 51u, (3u, 11u, 79u)
#define PPUTLIMPL_UINT_TRAIT_2606u DEC, 0xA2E, 1, 11u, 51u, (2u, 1303u)
#define PPUTLIMPL_UINT_TRAIT_2605u DEC, 0xA2D, 1, 11u, 51u, (5u, 521u)
#define PPUTLIMPL_UINT_TRAIT_2604u DEC, 0xA2C, 1, 11u, 51u, (2u, 2u, 3u, 7u, 31u)
#define PPUTLIMPL_UINT_TRAIT_2603u DEC, 0xA2B, 1, 11u, 51u, (19u, 137u)
#define PPUTLIMPL_UINT_TRAIT_2602u DEC, 0xA2A, 1, 11u, 51u, (2u, 1301u)
#define PPUTLIMPL_UINT_TRAIT_2601u DEC, 0xA29, 1, 11u, 51u, (3u, 3u, 17u, 17u)
#define PPUTLIMPL_UINT_TRAIT_2600u DEC, 0xA28, 1, 11u, 50u, (2u, 2u, 2u, 5u, 5u, 13u)
#define PPUTLIMPL_UINT_TRAIT_2599u DEC, 0xA27, 1, 11u, 50u, (23u, 113u)
#define PPUTLIMPL_UINT_TRAIT_2598u DEC, 0xA26, 1, 11u, 50u, (2u, 3u, 433u)
#define PPUTLIMPL_UINT_TRAIT_2597u DEC, 0xA25, 1, 11u, 50u, (7u, 7u, 53u)
#define PPUTLIMPL_UINT_TRAIT_2596u DEC, 0xA24, 1, 11u, 50u, (2u, 2u, 11u, 59u)
#define PPUTLIMPL_UINT_TRAIT_2595u DEC, 0xA23, 1, 11u, 50u, (3u, 5u, 173u)
#define PPUTLIMPL_UINT_TRAIT_2594u DEC, 0xA22, 1, 11u, 50u, (2u, 1297u)
#define PPUTLIMPL_UINT_TRAIT_2593u DEC, 0xA21, 1, 11u, 50u, ()
#define PPUTLIMPL_UINT_TRAIT_2592u DEC, 0xA20, 1, 11u, 50u, (2u, 2u, 2u, 2u, 2u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_2591u DEC, 0xA1F, 1, 11u, 50u, ()
#define PPUTLIMPL_UINT_TRAIT_2590u DEC, 0xA1E, 1, 11u, 50u, (2u, 5u, 7u, 37u)
#define PPUTLIMPL_UINT_TRAIT_2589u DEC, 0xA1D, 1, 11u, 50u, (3u, 863u)
#define PPUTLIMPL_UINT_TRAIT_2588u DEC, 0xA1C, 1, 11u, 50u, (2u, 2u, 647u)
#define PPUTLIMPL_UINT_TRAIT_2587u DEC, 0xA1B, 1, 11u, 50u, (13u, 199u)
#define PPUTLIMPL_UINT_TRAIT_2586u DEC, 0xA1A, 1, 11u, 50u, (2u, 3u, 431u)
#define PPUTLIMPL_UINT_TRAIT_2585u DEC, 0xA19, 1, 11u, 50u, (5u, 11u, 47u)
#define PPUTLIMPL_UINT_TRAIT_2584u DEC, 0xA18, 1, 11u, 50u, (2u, 2u, 2u, 17u, 19u)
#define PPUTLIMPL_UINT_TRAIT_2583u DEC, 0xA17, 1, 11u, 50u, (3u, 3u, 7u, 41u)
#define PPUTLIMPL_UINT_TRAIT_2582u DEC, 0xA16, 1, 11u, 50u, (2u, 1291u)
#define PPUTLIMPL_UINT_TRAIT_2581u DEC, 0xA15, 1, 11u, 50u, (29u, 89u)
#define PPUTLIMPL_UINT_TRAIT_2580u DEC, 0xA14, 1, 11u, 50u, (2u, 2u, 3u, 5u, 43u)
#define PPUTLIMPL_UINT_TRAIT_2579u DEC, 0xA13, 1, 11u, 50u, ()
#define PPUTLIMPL_UINT_TRAIT_2578u DEC, 0xA12, 1, 11u, 50u, (2u, 1289u)
#define PPUTLIMPL_UINT_TRAIT_2577u DEC, 0xA11, 1, 11u, 50u, (3u, 859u)
#define PPUTLIMPL_UINT_TRAIT_2576u DEC, 0xA10, 1, 11u, 50u, (2u, 2u, 2u, 2u, 7u, 23u)
#define PPUTLIMPL_UINT_TRAIT_2575u DEC, 0xA0F, 1, 11u, 50u, (5u, 5u, 103u)
#define PPUTLIMPL_UINT_TRAIT_2574u DEC, 0xA0E, 1, 11u, 50u, (2u, 3u, 3u, 11u, 13u)
#define PPUTLIMPL_UINT_TRAIT_2573u DEC, 0xA0D, 1, 11u, 50u, (31u, 83u)
#define PPUTLIMPL_UINT_TRAIT_2572u DEC, 0xA0C, 1, 11u, 50u, (2u, 2u, 643u)
#define PPUTLIMPL_UINT_TRAIT_2571u DEC, 0xA0B, 1, 11u, 50u, (3u, 857u)
#define PPUTLIMPL_UINT_TRAIT_2570u DEC, 0xA0A, 1, 11u, 50u, (2u, 5u, 257u)
#define PPUTLIMPL_UINT_TRAIT_2569u DEC, 0xA09, 1, 11u, 50u, (7u, 367u)
#define PPUTLIMPL_UINT_TRAIT_2568u DEC, 0xA08, 1, 11u, 50u, (2u, 2u, 2u, 3u, 107u)
#define PPUTLIMPL_UINT_TRAIT_2567u DEC, 0xA07, 1, 11u, 50u, (17u, 151u)
#define PPUTLIMPL_UINT_TRAIT_2566u DEC, 0xA06, 1, 11u, 50u, (2u, 1283u)
#define PPUTLIMPL_UINT_TRAIT_2565u DEC, 0xA05, 1, 11u, 50u, (3u, 3u, 3u, 5u, 19u)
#define PPUTLIMPL_UINT_TRAIT_2564u DEC, 0xA04, 1, 11u, 50u, (2u, 2u, 641u)
#define PPUTLIMPL_UINT_TRAIT_2563u DEC, 0xA03, 1, 11u, 50u, (11u, 233u)
#define PPUTLIMPL_UINT_TRAIT_2562u DEC, 0xA02, 1, 11u, 50u, (2u, 3u, 7u, 61u)
#define PPUTLIMPL_UINT_TRAIT_2561u DEC, 0xA01, 1, 11u, 50u, (13u, 197u)
#define PPUTLIMPL_UINT_TRAIT_2560u DEC, 0xA00, 1, 11u, 50u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 5u)
#define PPUTLIMPL_UINT_TRAIT_2559u DEC, 0x9FF, 1, 11u, 50u, (3u, 853u)
#define PPUTLIMPL_UINT_TRAIT_2558u DEC, 0x9FE, 1, 11u, 50u, (2u, 1279u)
#define PPUTLIMPL_UINT_TRAIT_2557u DEC, 0x9FD, 1, 11u, 50u, ()
#define PPUTLIMPL_UINT_TRAIT_2556u DEC, 0x9FC, 1, 11u, 50u, (2u, 2u, 3u, 3u, 71u)
#define PPUTLIMPL_UINT_TRAIT_2555u DEC, 0x9FB, 1, 11u, 50u, (5u, 7u, 73u)
#define PPUTLIMPL_UINT_TRAIT_2554u DEC, 0x9FA, 1, 11u, 50u, (2u, 1277u)
#define PPUTLIMPL_UINT_TRAIT_2553u DEC, 0x9F9, 1, 11u, 50u, (3u, 23u, 37u)
#define PPUTLIMPL_UINT_TRAIT_2552u DEC, 0x9F8, 1, 11u, 50u, (2u, 2u, 2u, 11u, 29u)
#define PPUTLIMPL_UINT_TRAIT_2551u DEC, 0x9F7, 1, 11u, 50u, ()
#define PPUTLIMPL_UINT_TRAIT_2550u DEC, 0x9F6, 1, 11u, 50u, (2u, 3u, 5u, 5u, 17u)
#define PPUTLIMPL_UINT_TRAIT_2549u DEC, 0x9F5, 1, 11u, 50u, ()
#define PPUTLIMPL_UINT_TRAIT_2548u DEC, 0x9F4, 1, 11u, 50u, (2u, 2u, 7u, 7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_2547u DEC, 0x9F3, 1, 11u, 50u, (3u, 3u, 283u)
#define PPUTLIMPL_UINT_TRAIT_2546u DEC, 0x9F2, 1, 11u, 50u, (2u, 19u, 67u)
#define PPUTLIMPL_UINT_TRAIT_2545u DEC, 0x9F1, 1, 11u, 50u, (5u, 509u)
#define PPUTLIMPL_UINT_TRAIT_2544u DEC, 0x9F0, 1, 11u, 50u, (2u, 2u, 2u, 2u, 3u, 53u)
#define PPUTLIMPL_UINT_TRAIT_2543u DEC, 0x9EF, 1, 11u, 50u, ()
#define PPUTLIMPL_UINT_TRAIT_2542u DEC, 0x9EE, 1, 11u, 50u, (2u, 31u, 41u)
#define PPUTLIMPL_UINT_TRAIT_2541u DEC, 0x9ED, 1, 11u, 50u, (3u, 7u, 11u, 11u)
#define PPUTLIMPL_UINT_TRAIT_2540u DEC, 0x9EC, 1, 11u, 50u, (2u, 2u, 5u, 127u)
#define PPUTLIMPL_UINT_TRAIT_2539u DEC, 0x9EB, 1, 11u, 50u, ()
#define PPUTLIMPL_UINT_TRAIT_2538u DEC, 0x9EA, 1, 11u, 50u, (2u, 3u, 3u, 3u, 47u)
#define PPUTLIMPL_UINT_TRAIT_2537u DEC, 0x9E9, 1, 11u, 50u, (43u, 59u)
#define PPUTLIMPL_UINT_TRAIT_2536u DEC, 0x9E8, 1, 11u, 50u, (2u, 2u, 2u, 317u)
#define PPUTLIMPL_UINT_TRAIT_2535u DEC, 0x9E7, 1, 11u, 50u, (3u, 5u, 13u, 13u)
#define PPUTLIMPL_UINT_TRAIT_2534u DEC, 0x9E6, 1, 11u, 50u, (2u, 7u, 181u)
#define PPUTLIMPL_UINT_TRAIT_2533u DEC, 0x9E5, 1, 11u, 50u, (17u, 149u)
#define PPUTLIMPL_UINT_TRAIT_2532u DEC, 0x9E4, 1, 11u, 50u, (2u, 2u, 3u, 211u)
#define PPUTLIMPL_UINT_TRAIT_2531u DEC, 0x9E3, 1, 11u, 50u, ()
#define PPUTLIMPL_UINT_TRAIT_2530u DEC, 0x9E2, 1, 11u, 50u, (2u, 5u, 11u, 23u)
#define PPUTLIMPL_UINT_TRAIT_2529u DEC, 0x9E1, 1, 11u, 50u, (3u, 3u, 281u)
#define PPUTLIMPL_UINT_TRAIT_2528u DEC, 0x9E0, 1, 11u, 50u, (2u, 2u, 2u, 2u, 2u, 79u)
#define PPUTLIMPL_UINT_TRAIT_2527u DEC, 0x9DF, 1, 11u, 50u, (7u, 19u, 19u)
#define PPUTLIMPL_UINT_TRAIT_2526u DEC, 0x9DE, 1, 11u, 50u, (2u, 3u, 421u)
#define PPUTLIMPL_UINT_TRAIT_2525u DEC, 0x9DD, 1, 11u, 50u, (5u, 5u, 101u)
#define PPUTLIMPL_UINT_TRAIT_2524u DEC, 0x9DC, 1, 11u, 50u, (2u, 2u, 631u)
#define PPUTLIMPL_UINT_TRAIT_2523u DEC, 0x9DB, 1, 11u, 50u, (3u, 29u, 29u)
#define PPUTLIMPL_UINT_TRAIT_2522u DEC, 0x9DA, 1, 11u, 50u, (2u, 13u, 97u)
#define PPUTLIMPL_UINT_TRAIT_2521u DEC, 0x9D9, 1, 11u, 50u, ()
#define PPUTLIMPL_UINT_TRAIT_2520u DEC, 0x9D8, 1, 11u, 50u, (2u, 2u, 2u, 3u, 3u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_2519u DEC, 0x9D7, 1, 11u, 50u, (11u, 229u)
#define PPUTLIMPL_UINT_TRAIT_2518u DEC, 0x9D6, 1, 11u, 50u, (2u, 1259u)
#define PPUTLIMPL_UINT_TRAIT_2517u DEC, 0x9D5, 1, 11u, 50u, (3u, 839u)
#define PPUTLIMPL_UINT_TRAIT_2516u DEC, 0x9D4, 1, 11u, 50u, (2u, 2u, 17u, 37u)
#define PPUTLIMPL_UINT_TRAIT_2515u DEC, 0x9D3, 1, 11u, 50u, (5u, 503u)
#define PPUTLIMPL_UINT_TRAIT_2514u DEC, 0x9D2, 1, 11u, 50u, (2u, 3u, 419u)
#define PPUTLIMPL_UINT_TRAIT_2513u DEC, 0x9D1, 1, 11u, 50u, (7u, 359u)
#define PPUTLIMPL_UINT_TRAIT_2512u DEC, 0x9D0, 1, 11u, 50u, (2u, 2u, 2u, 2u, 157u)
#define PPUTLIMPL_UINT_TRAIT_2511u DEC, 0x9CF, 1, 11u, 50u, (3u, 3u, 3u, 3u, 31u)
#define PPUTLIMPL_UINT_TRAIT_2510u DEC, 0x9CE, 1, 11u, 50u, (2u, 5u, 251u)
#define PPUTLIMPL_UINT_TRAIT_2509u DEC, 0x9CD, 1, 11u, 50u, (13u, 193u)
#define PPUTLIMPL_UINT_TRAIT_2508u DEC, 0x9CC, 1, 11u, 50u, (2u, 2u, 3u, 11u, 19u)
#define PPUTLIMPL_UINT_TRAIT_2507u DEC, 0x9CB, 1, 11u, 50u, (23u, 109u)
#define PPUTLIMPL_UINT_TRAIT_2506u DEC, 0x9CA, 1, 11u, 50u, (2u, 7u, 179u)
#define PPUTLIMPL_UINT_TRAIT_2505u DEC, 0x9C9, 1, 11u, 50u, (3u, 5u, 167u)
#define PPUTLIMPL_UINT_TRAIT_2504u DEC, 0x9C8, 1, 11u, 50u, (2u, 2u, 2u, 313u)
#define PPUTLIMPL_UINT_TRAIT_2503u DEC, 0x9C7, 1, 11u, 50u, ()
#define PPUTLIMPL_UINT_TRAIT_2502u DEC, 0x9C6, 1, 11u, 50u, (2u, 3u, 3u, 139u)
#define PPUTLIMPL_UINT_TRAIT_2501u DEC, 0x9C5, 1, 11u, 50u, (41u, 61u)
#define PPUTLIMPL_UINT_TRAIT_2500u DEC, 0x9C4, 1, 11u, 50u, (2u, 2u, 5u, 5u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_2499u DEC, 0x9C3, 1, 11u, 49u, (3u, 7u, 7u, 17u)
#define PPUTLIMPL_UINT_TRAIT_2498u DEC, 0x9C2, 1, 11u, 49u, (2u, 1249u)
#define PPUTLIMPL_UINT_TRAIT_2497u DEC, 0x9C1, 1, 11u, 49u, (11u, 227u)
#define PPUTLIMPL_UINT_TRAIT_2496u DEC, 0x9C0, 1, 11u, 49u, (2u, 2u, 2u, 2u, 2u, 2u, 3u, 13u)
#define PPUTLIMPL_UINT_TRAIT_2495u DEC, 0x9BF, 1, 11u, 49u, (5u, 499u)
#define PPUTLIMPL_UINT_TRAIT_2494u DEC, 0x9BE, 1, 11u, 49u, (2u, 29u, 43u)
#define PPUTLIMPL_UINT_TRAIT_2493u DEC, 0x9BD, 1, 11u, 49u, (3u, 3u, 277u)
#define PPUTLIMPL_UINT_TRAIT_2492u DEC, 0x9BC, 1, 11u, 49u, (2u, 2u, 7u, 89u)
#define PPUTLIMPL_UINT_TRAIT_2491u DEC, 0x9BB, 1, 11u, 49u, (47u, 53u)
#define PPUTLIMPL_UINT_TRAIT_2490u DEC, 0x9BA, 1, 11u, 49u, (2u, 3u, 5u, 83u)
#define PPUTLIMPL_UINT_TRAIT_2489u DEC, 0x9B9, 1, 11u, 49u, (19u, 131u)
#define PPUTLIMPL_UINT_TRAIT_2488u DEC, 0x9B8, 1, 11u, 49u, (2u, 2u, 2u, 311u)
#define PPUTLIMPL_UINT_TRAIT_2487u DEC, 0x9B7, 1, 11u, 49u, (3u, 829u)
#define PPUTLIMPL_UINT_TRAIT_2486u DEC, 0x9B6, 1, 11u, 49u, (2u, 11u, 113u)
#define PPUTLIMPL_UINT_TRAIT_2485u DEC, 0x9B5, 1, 11u, 49u, (5u, 7u, 71u)
#define PPUTLIMPL_UINT_TRAIT_2484u DEC, 0x9B4, 1, 11u, 49u, (2u, 2u, 3u, 3u, 3u, 23u)
#define PPUTLIMPL_UINT_TRAIT_2483u DEC, 0x9B3, 1, 11u, 49u, (13u, 191u)
#define PPUTLIMPL_UINT_TRAIT_2482u DEC, 0x9B2, 1, 11u, 49u, (2u, 17u, 73u)
#define PPUTLIMPL_UINT_TRAIT_2481u DEC, 0x9B1, 1, 11u, 49u, (3u, 827u)
#define PPUTLIMPL_UINT_TRAIT_2480u DEC, 0x9B0, 1, 11u, 49u, (2u, 2u, 2u, 2u, 5u, 31u)
#define PPUTLIMPL_UINT_TRAIT_2479u DEC, 0x9AF, 1, 11u, 49u, (37u, 67u)
#define PPUTLIMPL_UINT_TRAIT_2478u DEC, 0x9AE, 1, 11u, 49u, (2u, 3u, 7u, 59u)
#define PPUTLIMPL_UINT_TRAIT_2477u DEC, 0x9AD, 1, 11u, 49u, ()
#define PPUTLIMPL_UINT_TRAIT_2476u DEC, 0x9AC, 1, 11u, 49u, (2u, 2u, 619u)
#define PPUTLIMPL_UINT_TRAIT_2475u DEC, 0x9AB, 1, 11u, 49u, (3u, 3u, 5u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_2474u DEC, 0x9AA, 1, 11u, 49u, (2u, 1237u)
#define PPUTLIMPL_UINT_TRAIT_2473u DEC, 0x9A9, 1, 11u, 49u, ()
#define PPUTLIMPL_UINT_TRAIT_2472u DEC, 0x9A8, 1, 11u, 49u, (2u, 2u, 2u, 3u, 103u)
#define PPUTLIMPL_UINT_TRAIT_2471u DEC, 0x9A7, 1, 11u, 49u, (7u, 353u)
#define PPUTLIMPL_UINT_TRAIT_2470u DEC, 0x9A6, 1, 11u, 49u, (2u, 5u, 13u, 19u)
#define PPUTLIMPL_UINT_TRAIT_2469u DEC, 0x9A5, 1, 11u, 49u, (3u, 823u)
#define PPUTLIMPL_UINT_TRAIT_2468u DEC, 0x9A4, 1, 11u, 49u, (2u, 2u, 617u)
#define PPUTLIMPL_UINT_TRAIT_2467u DEC, 0x9A3, 1, 11u, 49u, ()
#define PPUTLIMPL_UINT_TRAIT_2466u DEC, 0x9A2, 1, 11u, 49u, (2u, 3u, 3u, 137u)
#define PPUTLIMPL_UINT_TRAIT_2465u DEC, 0x9A1, 1, 11u, 49u, (5u, 17u, 29u)
#define PPUTLIMPL_UINT_TRAIT_2464u DEC, 0x9A0, 1, 11u, 49u, (2u, 2u, 2u, 2u, 2u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_2463u DEC, 0x99F, 1, 11u, 49u, (3u, 821u)
#define PPUTLIMPL_UINT_TRAIT_2462u DEC, 0x99E, 1, 11u, 49u, (2u, 1231u)
#define PPUTLIMPL_UINT_TRAIT_2461u DEC, 0x99D, 1, 11u, 49u, (23u, 107u)
#define PPUTLIMPL_UINT_TRAIT_2460u DEC, 0x99C, 1, 11u, 49u, (2u, 2u, 3u, 5u, 41u)
#define PPUTLIMPL_UINT_TRAIT_2459u DEC, 0x99B, 1, 11u, 49u, ()
#define PPUTLIMPL_UINT_TRAIT_2458u DEC, 0x99A, 1, 11u, 49u, (2u, 1229u)
#define PPUTLIMPL_UINT_TRAIT_2457u DEC, 0x999, 1, 11u, 49u, (3u, 3u, 3u, 7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_2456u DEC, 0x998, 1, 11u, 49u, (2u, 2u, 2u, 307u)
#define PPUTLIMPL_UINT_TRAIT_2455u DEC, 0x997, 1, 11u, 49u, (5u, 491u)
#define PPUTLIMPL_UINT_TRAIT_2454u DEC, 0x996, 1, 11u, 49u, (2u, 3u, 409u)
#define PPUTLIMPL_UINT_TRAIT_2453u DEC, 0x995, 1, 11u, 49u, (11u, 223u)
#define PPUTLIMPL_UINT_TRAIT_2452u DEC, 0x994, 1, 11u, 49u, (2u, 2u, 613u)
#define PPUTLIMPL_UINT_TRAIT_2451u DEC, 0x993, 1, 11u, 49u, (3u, 19u, 43u)
#define PPUTLIMPL_UINT_TRAIT_2450u DEC, 0x992, 1, 11u, 49u, (2u, 5u, 5u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_2449u DEC, 0x991, 1, 11u, 49u, (31u, 79u)
#define PPUTLIMPL_UINT_TRAIT_2448u DEC, 0x990, 1, 11u, 49u, (2u, 2u, 2u, 2u, 3u, 3u, 17u)
#define PPUTLIMPL_UINT_TRAIT_2447u DEC, 0x98F, 1, 11u, 49u, ()
#define PPUTLIMPL_UINT_TRAIT_2446u DEC, 0x98E, 1, 11u, 49u, (2u, 1223u)
#define PPUTLIMPL_UINT_TRAIT_2445u DEC, 0x98D, 1, 11u, 49u, (3u, 5u, 163u)
#define PPUTLIMPL_UINT_TRAIT_2444u DEC, 0x98C, 1, 11u, 49u, (2u, 2u, 13u, 47u)
#define PPUTLIMPL_UINT_TRAIT_2443u DEC, 0x98B, 1, 11u, 49u, (7u, 349u)
#define PPUTLIMPL_UINT_TRAIT_2442u DEC, 0x98A, 1, 11u, 49u, (2u, 3u, 11u, 37u)
#define PPUTLIMPL_UINT_TRAIT_2441u DEC, 0x989, 1, 11u, 49u, ()
#define PPUTLIMPL_UINT_TRAIT_2440u DEC, 0x988, 1, 11u, 49u, (2u, 2u, 2u, 5u, 61u)
#define PPUTLIMPL_UINT_TRAIT_2439u DEC, 0x987, 1, 11u, 49u, (3u, 3u, 271u)
#define PPUTLIMPL_UINT_TRAIT_2438u DEC, 0x986, 1, 11u, 49u, (2u, 23u, 53u)
#define PPUTLIMPL_UINT_TRAIT_2437u DEC, 0x985, 1, 11u, 49u, ()
#define PPUTLIMPL_UINT_TRAIT_2436u DEC, 0x984, 1, 11u, 49u, (2u, 2u, 3u, 7u, 29u)
#define PPUTLIMPL_UINT_TRAIT_2435u DEC, 0x983, 1, 11u, 49u, (5u, 487u)
#define PPUTLIMPL_UINT_TRAIT_2434u DEC, 0x982, 1, 11u, 49u, (2u, 1217u)
#define PPUTLIMPL_UINT_TRAIT_2433u DEC, 0x981, 1, 11u, 49u, (3u, 811u)
#define PPUTLIMPL_UINT_TRAIT_2432u DEC, 0x980, 1, 11u, 49u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 19u)
#define PPUTLIMPL_UINT_TRAIT_2431u DEC, 0x97F, 1, 11u, 49u, (11u, 13u, 17u)
#define PPUTLIMPL_UINT_TRAIT_2430u DEC, 0x97E, 1, 11u, 49u, (2u, 3u, 3u, 3u, 3u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_2429u DEC, 0x97D, 1, 11u, 49u, (7u, 347u)
#define PPUTLIMPL_UINT_TRAIT_2428u DEC, 0x97C, 1, 11u, 49u, (2u, 2u, 607u)
#define PPUTLIMPL_UINT_TRAIT_2427u DEC, 0x97B, 1, 11u, 49u, (3u, 809u)
#define PPUTLIMPL_UINT_TRAIT_2426u DEC, 0x97A, 1, 11u, 49u, (2u, 1213u)
#define PPUTLIMPL_UINT_TRAIT_2425u DEC, 0x979, 1, 11u, 49u, (5u, 5u, 97u)
#define PPUTLIMPL_UINT_TRAIT_2424u DEC, 0x978, 1, 11u, 49u, (2u, 2u, 2u, 3u, 101u)
#define PPUTLIMPL_UINT_TRAIT_2423u DEC, 0x977, 1, 11u, 49u, ()
#define PPUTLIMPL_UINT_TRAIT_2422u DEC, 0x976, 1, 11u, 49u, (2u, 7u, 173u)
#define PPUTLIMPL_UINT_TRAIT_2421u DEC, 0x975, 1, 11u, 49u, (3u, 3u, 269u)
#define PPUTLIMPL_UINT_TRAIT_2420u DEC, 0x974, 1, 11u, 49u, (2u, 2u, 5u, 11u, 11u)
#define PPUTLIMPL_UINT_TRAIT_2419u DEC, 0x973, 1, 11u, 49u, (41u, 59u)
#define PPUTLIMPL_UINT_TRAIT_2418u DEC, 0x972, 1, 11u, 49u, (2u, 3u, 13u, 31u)
#define PPUTLIMPL_UINT_TRAIT_2417u DEC, 0x971, 1, 11u, 49u, ()
#define PPUTLIMPL_UINT_TRAIT_2416u DEC, 0x970, 1, 11u, 49u, (2u, 2u, 2u, 2u, 151u)
#define PPUTLIMPL_UINT_TRAIT_2415u DEC, 0x96F, 1, 11u, 49u, (3u, 5u, 7u, 23u)
#define PPUTLIMPL_UINT_TRAIT_2414u DEC, 0x96E, 1, 11u, 49u, (2u, 17u, 71u)
#define PPUTLIMPL_UINT_TRAIT_2413u DEC, 0x96D, 1, 11u, 49u, (19u, 127u)
#define PPUTLIMPL_UINT_TRAIT_2412u DEC, 0x96C, 1, 11u, 49u, (2u, 2u, 3u, 3u, 67u)
#define PPUTLIMPL_UINT_TRAIT_2411u DEC, 0x96B, 1, 11u, 49u, ()
#define PPUTLIMPL_UINT_TRAIT_2410u DEC, 0x96A, 1, 11u, 49u, (2u, 5u, 241u)
#define PPUTLIMPL_UINT_TRAIT_2409u DEC, 0x969, 1, 11u, 49u, (3u, 11u, 73u)
#define PPUTLIMPL_UINT_TRAIT_2408u DEC, 0x968, 1, 11u, 49u, (2u, 2u, 2u, 7u, 43u)
#define PPUTLIMPL_UINT_TRAIT_2407u DEC, 0x967, 1, 11u, 49u, (29u, 83u)
#define PPUTLIMPL_UINT_TRAIT_2406u DEC, 0x966, 1, 11u, 49u, (2u, 3u, 401u)
#define PPUTLIMPL_UINT_TRAIT_2405u DEC, 0x965, 1, 11u, 49u, (5u, 13u, 37u)
#define PPUTLIMPL_UINT_TRAIT_2404u DEC, 0x964, 1, 11u, 49u, (2u, 2u, 601u)
#define PPUTLIMPL_UINT_TRAIT_2403u DEC, 0x963, 1, 11u, 49u, (3u, 3u, 3u, 89u)
#define PPUTLIMPL_UINT_TRAIT_2402u DEC, 0x962, 1, 11u, 49u, (2u, 1201u)
#define PPUTLIMPL_UINT_TRAIT_2401u DEC, 0x961, 1, 11u, 49u, (7u, 7u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_2400u DEC, 0x960, 1, 11u, 48u, (2u, 2u, 2u, 2u, 2u, 3u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_2399u DEC, 0x95F, 1, 11u, 48u, ()
#define PPUTLIMPL_UINT_TRAIT_2398u DEC, 0x95E, 1, 11u, 48u, (2u, 11u, 109u)
#define PPUTLIMPL_UINT_TRAIT_2397u DEC, 0x95D, 1, 11u, 48u, (3u, 17u, 47u)
#define PPUTLIMPL_UINT_TRAIT_2396u DEC, 0x95C, 1, 11u, 48u, (2u, 2u, 599u)
#define PPUTLIMPL_UINT_TRAIT_2395u DEC, 0x95B, 1, 11u, 48u, (5u, 479u)
#define PPUTLIMPL_UINT_TRAIT_2394u DEC, 0x95A, 1, 11u, 48u, (2u, 3u, 3u, 7u, 19u)
#define PPUTLIMPL_UINT_TRAIT_2393u DEC, 0x959, 1, 11u, 48u, ()
#define PPUTLIMPL_UINT_TRAIT_2392u DEC, 0x958, 1, 11u, 48u, (2u, 2u, 2u, 13u, 23u)
#define PPUTLIMPL_UINT_TRAIT_2391u DEC, 0x957, 1, 11u, 48u, (3u, 797u)
#define PPUTLIMPL_UINT_TRAIT_2390u DEC, 0x956, 1, 11u, 48u, (2u, 5u, 239u)
#define PPUTLIMPL_UINT_TRAIT_2389u DEC, 0x955, 1, 11u, 48u, ()
#define PPUTLIMPL_UINT_TRAIT_2388u DEC, 0x954, 1, 11u, 48u, (2u, 2u, 3u, 199u)
#define PPUTLIMPL_UINT_TRAIT_2387u DEC, 0x953, 1, 11u, 48u, (7u, 11u, 31u)
#define PPUTLIMPL_UINT_TRAIT_2386u DEC, 0x952, 1, 11u, 48u, (2u, 1193u)
#define PPUTLIMPL_UINT_TRAIT_2385u DEC, 0x951, 1, 11u, 48u, (3u, 3u, 5u, 53u)
#define PPUTLIMPL_UINT_TRAIT_2384u DEC, 0x950, 1, 11u, 48u, (2u, 2u, 2u, 2u, 149u)
#define PPUTLIMPL_UINT_TRAIT_2383u DEC, 0x94F, 1, 11u, 48u, ()
#define PPUTLIMPL_UINT_TRAIT_2382u DEC, 0x94E, 1, 11u, 48u, (2u, 3u, 397u)
#define PPUTLIMPL_UINT_TRAIT_2381u DEC, 0x94D, 1, 11u, 48u, ()
#define PPUTLIMPL_UINT_TRAIT_2380u DEC, 0x94C, 1, 11u, 48u, (2u, 2u, 5u, 7u, 17u)
#define PPUTLIMPL_UINT_TRAIT_2379u DEC, 0x94B, 1, 11u, 48u, (3u, 13u, 61u)
#define PPUTLIMPL_UINT_TRAIT_2378u DEC, 0x94A, 1, 11u, 48u, (2u, 29u, 41u)
#define PPUTLIMPL_UINT_TRAIT_2377u DEC, 0x949, 1, 11u, 48u, ()
#define PPUTLIMPL_UINT_TRAIT_2376u DEC, 0x948, 1, 11u, 48u, (2u, 2u, 2u, 3u, 3u, 3u, 11u)
#define PPUTLIMPL_UINT_TRAIT_2375u DEC, 0x947, 1, 11u, 48u, (5u, 5u, 5u, 19u)
#define PPUTLIMPL_UINT_TRAIT_2374u DEC, 0x946, 1, 11u, 48u, (2u, 1187u)
#define PPUTLIMPL_UINT_TRAIT_2373u DEC, 0x945, 1, 11u, 48u, (3u, 7u, 113u)
#define PPUTLIMPL_UINT_TRAIT_2372u DEC, 0x944, 1, 11u, 48u, (2u, 2u, 593u)
#define PPUTLIMPL_UINT_TRAIT_2371u DEC, 0x943, 1, 11u, 48u, ()
#define PPUTLIMPL_UINT_TRAIT_2370u DEC, 0x942, 1, 11u, 48u, (2u, 3u, 5u, 79u)
#define PPUTLIMPL_UINT_TRAIT_2369u DEC, 0x941, 1, 11u, 48u, (23u, 103u)
#define PPUTLIMPL_UINT_TRAIT_2368u DEC, 0x940, 1, 11u, 48u, (2u, 2u, 2u, 2u, 2u, 2u, 37u)
#define PPUTLIMPL_UINT_TRAIT_2367u DEC, 0x93F, 1, 11u, 48u, (3u, 3u, 263u)
#define PPUTLIMPL_UINT_TRAIT_2366u DEC, 0x93E, 1, 11u, 48u, (2u, 7u, 13u, 13u)
#define PPUTLIMPL_UINT_TRAIT_2365u DEC, 0x93D, 1, 11u, 48u, (5u, 11u, 43u)
#define PPUTLIMPL_UINT_TRAIT_2364u DEC, 0x93C, 1, 11u, 48u, (2u, 2u, 3u, 197u)
#define PPUTLIMPL_UINT_TRAIT_2363u DEC, 0x93B, 1, 11u, 48u, (17u, 139u)
#define PPUTLIMPL_UINT_TRAIT_2362u DEC, 0x93A, 1, 11u, 48u, (2u, 1181u)
#define PPUTLIMPL_UINT_TRAIT_2361u DEC, 0x939, 1, 11u, 48u, (3u, 787u)
#define PPUTLIMPL_UINT_TRAIT_2360u DEC, 0x938, 1, 11u, 48u, (2u, 2u, 2u, 5u, 59u)
#define PPUTLIMPL_UINT_TRAIT_2359u DEC, 0x937, 1, 11u, 48u, (7u, 337u)
#define PPUTLIMPL_UINT_TRAIT_2358u DEC, 0x936, 1, 11u, 48u, (2u, 3u, 3u, 131u)
#define PPUTLIMPL_UINT_TRAIT_2357u DEC, 0x935, 1, 11u, 48u, ()
#define PPUTLIMPL_UINT_TRAIT_2356u DEC, 0x934, 1, 11u, 48u, (2u, 2u, 19u, 31u)
#define PPUTLIMPL_UINT_TRAIT_2355u DEC, 0x933, 1, 11u, 48u, (3u, 5u, 157u)
#define PPUTLIMPL_UINT_TRAIT_2354u DEC, 0x932, 1, 11u, 48u, (2u, 11u, 107u)
#define PPUTLIMPL_UINT_TRAIT_2353u DEC, 0x931, 1, 11u, 48u, (13u, 181u)
#define PPUTLIMPL_UINT_TRAIT_2352u DEC, 0x930, 1, 11u, 48u, (2u, 2u, 2u, 2u, 3u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_2351u DEC, 0x92F, 1, 11u, 48u, ()
#define PPUTLIMPL_UINT_TRAIT_2350u DEC, 0x92E, 1, 11u, 48u, (2u, 5u, 5u, 47u)
#define PPUTLIMPL_UINT_TRAIT_2349u DEC, 0x92D, 1, 11u, 48u, (3u, 3u, 3u, 3u, 29u)
#define PPUTLIMPL_UINT_TRAIT_2348u DEC, 0x92C, 1, 11u, 48u, (2u, 2u, 587u)
#define PPUTLIMPL_UINT_TRAIT_2347u DEC, 0x92B, 1, 11u, 48u, ()
#define PPUTLIMPL_UINT_TRAIT_2346u DEC, 0x92A, 1, 11u, 48u, (2u, 3u, 17u, 23u)
#define PPUTLIMPL_UINT_TRAIT_2345u DEC, 0x929, 1, 11u, 48u, (5u, 7u, 67u)
#define PPUTLIMPL_UINT_TRAIT_2344u DEC, 0x928, 1, 11u, 48u, (2u, 2u, 2u, 293u)
#define PPUTLIMPL_UINT_TRAIT_2343u DEC, 0x927, 1, 11u, 48u, (3u, 11u, 71u)
#define PPUTLIMPL_UINT_TRAIT_2342u DEC, 0x926, 1, 11u, 48u, (2u, 1171u)
#define PPUTLIMPL_UINT_TRAIT_2341u DEC, 0x925, 1, 11u, 48u, ()
#define PPUTLIMPL_UINT_TRAIT_2340u DEC, 0x924, 1, 11u, 48u, (2u, 2u, 3u, 3u, 5u, 13u)
#define PPUTLIMPL_UINT_TRAIT_2339u DEC, 0x923, 1, 11u, 48u, ()
#define PPUTLIMPL_UINT_TRAIT_2338u DEC, 0x922, 1, 11u, 48u, (2u, 7u, 167u)
#define PPUTLIMPL_UINT_TRAIT_2337u DEC, 0x921, 1, 11u, 48u, (3u, 19u, 41u)
#define PPUTLIMPL_UINT_TRAIT_2336u DEC, 0x920, 1, 11u, 48u, (2u, 2u, 2u, 2u, 2u, 73u)
#define PPUTLIMPL_UINT_TRAIT_2335u DEC, 0x91F, 1, 11u, 48u, (5u, 467u)
#define PPUTLIMPL_UINT_TRAIT_2334u DEC, 0x91E, 1, 11u, 48u, (2u, 3u, 389u)
#define PPUTLIMPL_UINT_TRAIT_2333u DEC, 0x91D, 1, 11u, 48u, ()
#define PPUTLIMPL_UINT_TRAIT_2332u DEC, 0x91C, 1, 11u, 48u, (2u, 2u, 11u, 53u)
#define PPUTLIMPL_UINT_TRAIT_2331u DEC, 0x91B, 1, 11u, 48u, (3u, 3u, 7u, 37u)
#define PPUTLIMPL_UINT_TRAIT_2330u DEC, 0x91A, 1, 11u, 48u, (2u, 5u, 233u)
#define PPUTLIMPL_UINT_TRAIT_2329u DEC, 0x919, 1, 11u, 48u, (17u, 137u)
#define PPUTLIMPL_UINT_TRAIT_2328u DEC, 0x918, 1, 11u, 48u, (2u, 2u, 2u, 3u, 97u)
#define PPUTLIMPL_UINT_TRAIT_2327u DEC, 0x917, 1, 11u, 48u, (13u, 179u)
#define PPUTLIMPL_UINT_TRAIT_2326u DEC, 0x916, 1, 11u, 48u, (2u, 1163u)
#define PPUTLIMPL_UINT_TRAIT_2325u DEC, 0x915, 1, 11u, 48u, (3u, 5u, 5u, 31u)
#define PPUTLIMPL_UINT_TRAIT_2324u DEC, 0x914, 1, 11u, 48u, (2u, 2u, 7u, 83u)
#define PPUTLIMPL_UINT_TRAIT_2323u DEC, 0x913, 1, 11u, 48u, (23u, 101u)
#define PPUTLIMPL_UINT_TRAIT_2322u DEC, 0x912, 1, 11u, 48u, (2u, 3u, 3u, 3u, 43u)
#define PPUTLIMPL_UINT_TRAIT_2321u DEC, 0x911, 1, 11u, 48u, (11u, 211u)
#define PPUTLIMPL_UINT_TRAIT_2320u DEC, 0x910, 1, 11u, 48u, (2u, 2u, 2u, 2u, 5u, 29u)
#define PPUTLIMPL_UINT_TRAIT_2319u DEC, 0x90F, 1, 11u, 48u, (3u, 773u)
#define PPUTLIMPL_UINT_TRAIT_2318u DEC, 0x90E, 1, 11u, 48u, (2u, 19u, 61u)
#define PPUTLIMPL_UINT_TRAIT_2317u DEC, 0x90D, 1, 11u, 48u, (7u, 331u)
#define PPUTLIMPL_UINT_TRAIT_2316u DEC, 0x90C, 1, 11u, 48u, (2u, 2u, 3u, 193u)
#define PPUTLIMPL_UINT_TRAIT_2315u DEC, 0x90B, 1, 11u, 48u, (5u, 463u)
#define PPUTLIMPL_UINT_TRAIT_2314u DEC, 0x90A, 1, 11u, 48u, (2u, 13u, 89u)
#define PPUTLIMPL_UINT_TRAIT_2313u DEC, 0x909, 1, 11u, 48u, (3u, 3u, 257u)
#define PPUTLIMPL_UINT_TRAIT_2312u DEC, 0x908, 1, 11u, 48u, (2u, 2u, 2u, 17u, 17u)
#define PPUTLIMPL_UINT_TRAIT_2311u DEC, 0x907, 1, 11u, 48u, ()
#define PPUTLIMPL_UINT_TRAIT_2310u DEC, 0x906, 1, 11u, 48u, (2u, 3u, 5u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_2309u DEC, 0x905, 1, 11u, 48u, ()
#define PPUTLIMPL_UINT_TRAIT_2308u DEC, 0x904, 1, 11u, 48u, (2u, 2u, 577u)
#define PPUTLIMPL_UINT_TRAIT_2307u DEC, 0x903, 1, 11u, 48u, (3u, 769u)
#define PPUTLIMPL_UINT_TRAIT_2306u DEC, 0x902, 1, 11u, 48u, (2u, 1153u)
#define PPUTLIMPL_UINT_TRAIT_2305u DEC, 0x901, 1, 11u, 48u, (5u, 461u)
#define PPUTLIMPL_UINT_TRAIT_2304u DEC, 0x900, 1, 11u, 48u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_2303u DEC, 0x8FF, 1, 11u, 47u, (7u, 7u, 47u)
#define PPUTLIMPL_UINT_TRAIT_2302u DEC, 0x8FE, 1, 11u, 47u, (2u, 1151u)
#define PPUTLIMPL_UINT_TRAIT_2301u DEC, 0x8FD, 1, 11u, 47u, (3u, 13u, 59u)
#define PPUTLIMPL_UINT_TRAIT_2300u DEC, 0x8FC, 1, 11u, 47u, (2u, 2u, 5u, 5u, 23u)
#define PPUTLIMPL_UINT_TRAIT_2299u DEC, 0x8FB, 1, 11u, 47u, (11u, 11u, 19u)
#define PPUTLIMPL_UINT_TRAIT_2298u DEC, 0x8FA, 1, 11u, 47u, (2u, 3u, 383u)
#define PPUTLIMPL_UINT_TRAIT_2297u DEC, 0x8F9, 1, 11u, 47u, ()
#define PPUTLIMPL_UINT_TRAIT_2296u DEC, 0x8F8, 1, 11u, 47u, (2u, 2u, 2u, 7u, 41u)
#define PPUTLIMPL_UINT_TRAIT_2295u DEC, 0x8F7, 1, 11u, 47u, (3u, 3u, 3u, 5u, 17u)
#define PPUTLIMPL_UINT_TRAIT_2294u DEC, 0x8F6, 1, 11u, 47u, (2u, 31u, 37u)
#define PPUTLIMPL_UINT_TRAIT_2293u DEC, 0x8F5, 1, 11u, 47u, ()
#define PPUTLIMPL_UINT_TRAIT_2292u DEC, 0x8F4, 1, 11u, 47u, (2u, 2u, 3u, 191u)
#define PPUTLIMPL_UINT_TRAIT_2291u DEC, 0x8F3, 1, 11u, 47u, (29u, 79u)
#define PPUTLIMPL_UINT_TRAIT_2290u DEC, 0x8F2, 1, 11u, 47u, (2u, 5u, 229u)
#define PPUTLIMPL_UINT_TRAIT_2289u DEC, 0x8F1, 1, 11u, 47u, (3u, 7u, 109u)
#define PPUTLIMPL_UINT_TRAIT_2288u DEC, 0x8F0, 1, 11u, 47u, (2u, 2u, 2u, 2u, 11u, 13u)
#define PPUTLIMPL_UINT_TRAIT_2287u DEC, 0x8EF, 1, 11u, 47u, ()
#define PPUTLIMPL_UINT_TRAIT_2286u DEC, 0x8EE, 1, 11u, 47u, (2u, 3u, 3u, 127u)
#define PPUTLIMPL_UINT_TRAIT_2285u DEC, 0x8ED, 1, 11u, 47u, (5u, 457u)
#define PPUTLIMPL_UINT_TRAIT_2284u DEC, 0x8EC, 1, 11u, 47u, (2u, 2u, 571u)
#define PPUTLIMPL_UINT_TRAIT_2283u DEC, 0x8EB, 1, 11u, 47u, (3u, 761u)
#define PPUTLIMPL_UINT_TRAIT_2282u DEC, 0x8EA, 1, 11u, 47u, (2u, 7u, 163u)
#define PPUTLIMPL_UINT_TRAIT_2281u DEC, 0x8E9, 1, 11u, 47u, ()
#define PPUTLIMPL_UINT_TRAIT_2280u DEC, 0x8E8, 1, 11u, 47u, (2u, 2u, 2u, 3u, 5u, 19u)
#define PPUTLIMPL_UINT_TRAIT_2279u DEC, 0x8E7, 1, 11u, 47u, (43u, 53u)
#define PPUTLIMPL_UINT_TRAIT_2278u DEC, 0x8E6, 1, 11u, 47u, (2u, 17u, 67u)
#define PPUTLIMPL_UINT_TRAIT_2277u DEC, 0x8E5, 1, 11u, 47u, (3u, 3u, 11u, 23u)
#define PPUTLIMPL_UINT_TRAIT_2276u DEC, 0x8E4, 1, 11u, 47u, (2u, 2u, 569u)
#define PPUTLIMPL_UINT_TRAIT_2275u DEC, 0x8E3, 1, 11u, 47u, (5u, 5u, 7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_2274u DEC, 0x8E2, 1, 11u, 47u, (2u, 3u, 379u)
#define PPUTLIMPL_UINT_TRAIT_2273u DEC, 0x8E1, 1, 11u, 47u, ()
#define PPUTLIMPL_UINT_TRAIT_2272u DEC, 0x8E0, 1, 11u, 47u, (2u, 2u, 2u, 2u, 2u, 71u)
#define PPUTLIMPL_UINT_TRAIT_2271u DEC, 0x8DF, 1, 11u, 47u, (3u, 757u)
#define PPUTLIMPL_UINT_TRAIT_2270u DEC, 0x8DE, 1, 11u, 47u, (2u, 5u, 227u)
#define PPUTLIMPL_UINT_TRAIT_2269u DEC, 0x8DD, 1, 11u, 47u, ()
#define PPUTLIMPL_UINT_TRAIT_2268u DEC, 0x8DC, 1, 11u, 47u, (2u, 2u, 3u, 3u, 3u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_2267u DEC, 0x8DB, 1, 11u, 47u, ()
#define PPUTLIMPL_UINT_TRAIT_2266u DEC, 0x8DA, 1, 11u, 47u, (2u, 11u, 103u)
#define PPUTLIMPL_UINT_TRAIT_2265u DEC, 0x8D9, 1, 11u, 47u, (3u, 5u, 151u)
#define PPUTLIMPL_UINT_TRAIT_2264u DEC, 0x8D8, 1, 11u, 47u, (2u, 2u, 2u, 283u)
#define PPUTLIMPL_UINT_TRAIT_2263u DEC, 0x8D7, 1, 11u, 47u, (31u, 73u)
#define PPUTLIMPL_UINT_TRAIT_2262u DEC, 0x8D6, 1, 11u, 47u, (2u, 3u, 13u, 29u)
#define PPUTLIMPL_UINT_TRAIT_2261u DEC, 0x8D5, 1, 11u, 47u, (7u, 17u, 19u)
#define PPUTLIMPL_UINT_TRAIT_2260u DEC, 0x8D4, 1, 11u, 47u, (2u, 2u, 5u, 113u)
#define PPUTLIMPL_UINT_TRAIT_2259u DEC, 0x8D3, 1, 11u, 47u, (3u, 3u, 251u)
#define PPUTLIMPL_UINT_TRAIT_2258u DEC, 0x8D2, 1, 11u, 47u, (2u, 1129u)
#define PPUTLIMPL_UINT_TRAIT_2257u DEC, 0x8D1, 1, 11u, 47u, (37u, 61u)
#define PPUTLIMPL_UINT_TRAIT_2256u DEC, 0x8D0, 1, 11u, 47u, (2u, 2u, 2u, 2u, 3u, 47u)
#define PPUTLIMPL_UINT_TRAIT_2255u DEC, 0x8CF, 1, 11u, 47u, (5u, 11u, 41u)
#define PPUTLIMPL_UINT_TRAIT_2254u DEC, 0x8CE, 1, 11u, 47u, (2u, 7u, 7u, 23u)
#define PPUTLIMPL_UINT_TRAIT_2253u DEC, 0x8CD, 1, 11u, 47u, (3u, 751u)
#define PPUTLIMPL_UINT_TRAIT_2252u DEC, 0x8CC, 1, 11u, 47u, (2u, 2u, 563u)
#define PPUTLIMPL_UINT_TRAIT_2251u DEC, 0x8CB, 1, 11u, 47u, ()
#define PPUTLIMPL_UINT_TRAIT_2250u DEC, 0x8CA, 1, 11u, 47u, (2u, 3u, 3u, 5u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_2249u DEC, 0x8C9, 1, 11u, 47u, (13u, 173u)
#define PPUTLIMPL_UINT_TRAIT_2248u DEC, 0x8C8, 1, 11u, 47u, (2u, 2u, 2u, 281u)
#define PPUTLIMPL_UINT_TRAIT_2247u DEC, 0x8C7, 1, 11u, 47u, (3u, 7u, 107u)
#define PPUTLIMPL_UINT_TRAIT_2246u DEC, 0x8C6, 1, 11u, 47u, (2u, 1123u)
#define PPUTLIMPL_UINT_TRAIT_2245u DEC, 0x8C5, 1, 11u, 47u, (5u, 449u)
#define PPUTLIMPL_UINT_TRAIT_2244u DEC, 0x8C4, 1, 11u, 47u, (2u, 2u, 3u, 11u, 17u)
#define PPUTLIMPL_UINT_TRAIT_2243u DEC, 0x8C3, 1, 11u, 47u, ()
#define PPUTLIMPL_UINT_TRAIT_2242u DEC, 0x8C2, 1, 11u, 47u, (2u, 19u, 59u)
#define PPUTLIMPL_UINT_TRAIT_2241u DEC, 0x8C1, 1, 11u, 47u, (3u, 3u, 3u, 83u)
#define PPUTLIMPL_UINT_TRAIT_2240u DEC, 0x8C0, 1, 11u, 47u, (2u, 2u, 2u, 2u, 2u, 2u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_2239u DEC, 0x8BF, 1, 11u, 47u, ()
#define PPUTLIMPL_UINT_TRAIT_2238u DEC, 0x8BE, 1, 11u, 47u, (2u, 3u, 373u)
#define PPUTLIMPL_UINT_TRAIT_2237u DEC, 0x8BD, 1, 11u, 47u, ()
#define PPUTLIMPL_UINT_TRAIT_2236u DEC, 0x8BC, 1, 11u, 47u, (2u, 2u, 13u, 43u)
#define PPUTLIMPL_UINT_TRAIT_2235u DEC, 0x8BB, 1, 11u, 47u, (3u, 5u, 149u)
#define PPUTLIMPL_UINT_TRAIT_2234u DEC, 0x8BA, 1, 11u, 47u, (2u, 1117u)
#define PPUTLIMPL_UINT_TRAIT_2233u DEC, 0x8B9, 1, 11u, 47u, (7u, 11u, 29u)
#define PPUTLIMPL_UINT_TRAIT_2232u DEC, 0x8B8, 1, 11u, 47u, (2u, 2u, 2u, 3u, 3u, 31u)
#define PPUTLIMPL_UINT_TRAIT_2231u DEC, 0x8B7, 1, 11u, 47u, (23u, 97u)
#define PPUTLIMPL_UINT_TRAIT_2230u DEC, 0x8B6, 1, 11u, 47u, (2u, 5u, 223u)
#define PPUTLIMPL_UINT_TRAIT_2229u DEC, 0x8B5, 1, 11u, 47u, (3u, 743u)
#define PPUTLIMPL_UINT_TRAIT_2228u DEC, 0x8B4, 1, 11u, 47u, (2u, 2u, 557u)
#define PPUTLIMPL_UINT_TRAIT_2227u DEC, 0x8B3, 1, 11u, 47u, (17u, 131u)
#define PPUTLIMPL_UINT_TRAIT_2226u DEC, 0x8B2, 1, 11u, 47u, (2u, 3u, 7u, 53u)
#define PPUTLIMPL_UINT_TRAIT_2225u DEC, 0x8B1, 1, 11u, 47u, (5u, 5u, 89u)
#define PPUTLIMPL_UINT_TRAIT_2224u DEC, 0x8B0, 1, 11u, 47u, (2u, 2u, 2u, 2u, 139u)
#define PPUTLIMPL_UINT_TRAIT_2223u DEC, 0x8AF, 1, 11u, 47u, (3u, 3u, 13u, 19u)
#define PPUTLIMPL_UINT_TRAIT_2222u DEC, 0x8AE, 1, 11u, 47u, (2u, 11u, 101u)
#define PPUTLIMPL_UINT_TRAIT_2221u DEC, 0x8AD, 1, 11u, 47u, ()
#define PPUTLIMPL_UINT_TRAIT_2220u DEC, 0x8AC, 1, 11u, 47u, (2u, 2u, 3u, 5u, 37u)
#define PPUTLIMPL_UINT_TRAIT_2219u DEC, 0x8AB, 1, 11u, 47u, (7u, 317u)
#define PPUTLIMPL_UINT_TRAIT_2218u DEC, 0x8AA, 1, 11u, 47u, (2u, 1109u)
#define PPUTLIMPL_UINT_TRAIT_2217u DEC, 0x8A9, 1, 11u, 47u, (3u, 739u)
#define PPUTLIMPL_UINT_TRAIT_2216u DEC, 0x8A8, 1, 11u, 47u, (2u, 2u, 2u, 277u)
#define PPUTLIMPL_UINT_TRAIT_2215u DEC, 0x8A7, 1, 11u, 47u, (5u, 443u)
#define PPUTLIMPL_UINT_TRAIT_2214u DEC, 0x8A6, 1, 11u, 47u, (2u, 3u, 3u, 3u, 41u)
#define PPUTLIMPL_UINT_TRAIT_2213u DEC, 0x8A5, 1, 11u, 47u, ()
#define PPUTLIMPL_UINT_TRAIT_2212u DEC, 0x8A4, 1, 11u, 47u, (2u, 2u, 7u, 79u)
#define PPUTLIMPL_UINT_TRAIT_2211u DEC, 0x8A3, 1, 11u, 47u, (3u, 11u, 67u)
#define PPUTLIMPL_UINT_TRAIT_2210u DEC, 0x8A2, 1, 11u, 47u, (2u, 5u, 13u, 17u)
#define PPUTLIMPL_UINT_TRAIT_2209u DEC, 0x8A1, 1, 11u, 47u, (47u, 47u)
#define PPUTLIMPL_UINT_TRAIT_2208u DEC, 0x8A0, 1, 11u, 46u, (2u, 2u, 2u, 2u, 2u, 3u, 23u)
#define PPUTLIMPL_UINT_TRAIT_2207u DEC, 0x89F, 1, 11u, 46u, ()
#define PPUTLIMPL_UINT_TRAIT_2206u DEC, 0x89E, 1, 11u, 46u, (2u, 1103u)
#define PPUTLIMPL_UINT_TRAIT_2205u DEC, 0x89D, 1, 11u, 46u, (3u, 3u, 5u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_2204u DEC, 0x89C, 1, 11u, 46u, (2u, 2u, 19u, 29u)
#define PPUTLIMPL_UINT_TRAIT_2203u DEC, 0x89B, 1, 11u, 46u, ()
#define PPUTLIMPL_UINT_TRAIT_2202u DEC, 0x89A, 1, 11u, 46u, (2u, 3u, 367u)
#define PPUTLIMPL_UINT_TRAIT_2201u DEC, 0x899, 1, 11u, 46u, (31u, 71u)
#define PPUTLIMPL_UINT_TRAIT_2200u DEC, 0x898, 1, 11u, 46u, (2u, 2u, 2u, 5u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_2199u DEC, 0x897, 1, 11u, 46u, (3u, 733u)
#define PPUTLIMPL_UINT_TRAIT_2198u DEC, 0x896, 1, 11u, 46u, (2u, 7u, 157u)
#define PPUTLIMPL_UINT_TRAIT_2197u DEC, 0x895, 1, 11u, 46u, (13u, 13u, 13u)
#define PPUTLIMPL_UINT_TRAIT_2196u DEC, 0x894, 1, 11u, 46u, (2u, 2u, 3u, 3u, 61u)
#define PPUTLIMPL_UINT_TRAIT_2195u DEC, 0x893, 1, 11u, 46u, (5u, 439u)
#define PPUTLIMPL_UINT_TRAIT_2194u DEC, 0x892, 1, 11u, 46u, (2u, 1097u)
#define PPUTLIMPL_UINT_TRAIT_2193u DEC, 0x891, 1, 11u, 46u, (3u, 17u, 43u)
#define PPUTLIMPL_UINT_TRAIT_2192u DEC, 0x890, 1, 11u, 46u, (2u, 2u, 2u, 2u, 137u)
#define PPUTLIMPL_UINT_TRAIT_2191u DEC, 0x88F, 1, 11u, 46u, (7u, 313u)
#define PPUTLIMPL_UINT_TRAIT_2190u DEC, 0x88E, 1, 11u, 46u, (2u, 3u, 5u, 73u)
#define PPUTLIMPL_UINT_TRAIT_2189u DEC, 0x88D, 1, 11u, 46u, (11u, 199u)
#define PPUTLIMPL_UINT_TRAIT_2188u DEC, 0x88C, 1, 11u, 46u, (2u, 2u, 547u)
#define PPUTLIMPL_UINT_TRAIT_2187u DEC, 0x88B, 1, 11u, 46u, (3u, 3u, 3u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_2186u DEC, 0x88A, 1, 11u, 46u, (2u, 1093u)
#define PPUTLIMPL_UINT_TRAIT_2185u DEC, 0x889, 1, 11u, 46u, (5u, 19u, 23u)
#define PPUTLIMPL_UINT_TRAIT_2184u DEC, 0x888, 1, 11u, 46u, (2u, 2u, 2u, 3u, 7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_2183u DEC, 0x887, 1, 11u, 46u, (37u, 59u)
#define PPUTLIMPL_UINT_TRAIT_2182u DEC, 0x886, 1, 11u, 46u, (2u, 1091u)
#define PPUTLIMPL_UINT_TRAIT_2181u DEC, 0x885, 1, 11u, 46u, (3u, 727u)
#define PPUTLIMPL_UINT_TRAIT_2180u DEC, 0x884, 1, 11u, 46u, (2u, 2u, 5u, 109u)
#define PPUTLIMPL_UINT_TRAIT_2179u DEC, 0x883, 1, 11u, 46u, ()
#define PPUTLIMPL_UINT_TRAIT_2178u DEC, 0x882, 1, 11u, 46u, (2u, 3u, 3u, 11u, 11u)
#define PPUTLIMPL_UINT_TRAIT_2177u DEC, 0x881, 1, 11u, 46u, (7u, 311u)
#define PPUTLIMPL_UINT_TRAIT_2176u DEC, 0x880, 1, 11u, 46u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 17u)
#define PPUTLIMPL_UINT_TRAIT_2175u DEC, 0x87F, 1, 11u, 46u, (3u, 5u, 5u, 29u)
#define PPUTLIMPL_UINT_TRAIT_2174u DEC, 0x87E, 1, 11u, 46u, (2u, 1087u)
#define PPUTLIMPL_UINT_TRAIT_2173u DEC, 0x87D, 1, 11u, 46u, (41u, 53u)
#define PPUTLIMPL_UINT_TRAIT_2172u DEC, 0x87C, 1, 11u, 46u, (2u, 2u, 3u, 181u)
#define PPUTLIMPL_UINT_TRAIT_2171u DEC, 0x87B, 1, 11u, 46u, (13u, 167u)
#define PPUTLIMPL_UINT_TRAIT_2170u DEC, 0x87A, 1, 11u, 46u, (2u, 5u, 7u, 31u)
#define PPUTLIMPL_UINT_TRAIT_2169u DEC, 0x879, 1, 11u, 46u, (3u, 3u, 241u)
#define PPUTLIMPL_UINT_TRAIT_2168u DEC, 0x878, 1, 11u, 46u, (2u, 2u, 2u, 271u)
#define PPUTLIMPL_UINT_TRAIT_2167u DEC, 0x877, 1, 11u, 46u, (11u, 197u)
#define PPUTLIMPL_UINT_TRAIT_2166u DEC, 0x876, 1, 11u, 46u, (2u, 3u, 19u, 19u)
#define PPUTLIMPL_UINT_TRAIT_2165u DEC, 0x875, 1, 11u, 46u, (5u, 433u)
#define PPUTLIMPL_UINT_TRAIT_2164u DEC, 0x874, 1, 11u, 46u, (2u, 2u, 541u)
#define PPUTLIMPL_UINT_TRAIT_2163u DEC, 0x873, 1, 11u, 46u, (3u, 7u, 103u)
#define PPUTLIMPL_UINT_TRAIT_2162u DEC, 0x872, 1, 11u, 46u, (2u, 23u, 47u)
#define PPUTLIMPL_UINT_TRAIT_2161u DEC, 0x871, 1, 11u, 46u, ()
#define PPUTLIMPL_UINT_TRAIT_2160u DEC, 0x870, 1, 11u, 46u, (2u, 2u, 2u, 2u, 3u, 3u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_2159u DEC, 0x86F, 1, 11u, 46u, (17u, 127u)
#define PPUTLIMPL_UINT_TRAIT_2158u DEC, 0x86E, 1, 11u, 46u, (2u, 13u, 83u)
#define PPUTLIMPL_UINT_TRAIT_2157u DEC, 0x86D, 1, 11u, 46u, (3u, 719u)
#define PPUTLIMPL_UINT_TRAIT_2156u DEC, 0x86C, 1, 11u, 46u, (2u, 2u, 7u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_2155u DEC, 0x86B, 1, 11u, 46u, (5u, 431u)
#define PPUTLIMPL_UINT_TRAIT_2154u DEC, 0x86A, 1, 11u, 46u, (2u, 3u, 359u)
#define PPUTLIMPL_UINT_TRAIT_2153u DEC, 0x869, 1, 11u, 46u, ()
#define PPUTLIMPL_UINT_TRAIT_2152u DEC, 0x868, 1, 11u, 46u, (2u, 2u, 2u, 269u)
#define PPUTLIMPL_UINT_TRAIT_2151u DEC, 0x867, 1, 11u, 46u, (3u, 3u, 239u)
#define PPUTLIMPL_UINT_TRAIT_2150u DEC, 0x866, 1, 11u, 46u, (2u, 5u, 5u, 43u)
#define PPUTLIMPL_UINT_TRAIT_2149u DEC, 0x865, 1, 11u, 46u, (7u, 307u)
#define PPUTLIMPL_UINT_TRAIT_2148u DEC, 0x864, 1, 11u, 46u, (2u, 2u, 3u, 179u)
#define PPUTLIMPL_UINT_TRAIT_2147u DEC, 0x863, 1, 11u, 46u, (19u, 113u)
#define PPUTLIMPL_UINT_TRAIT_2146u DEC, 0x862, 1, 11u, 46u, (2u, 29u, 37u)
#define PPUTLIMPL_UINT_TRAIT_2145u DEC, 0x861, 1, 11u, 46u, (3u, 5u, 11u, 13u)
#define PPUTLIMPL_UINT_TRAIT_2144u DEC, 0x860, 1, 11u, 46u, (2u, 2u, 2u, 2u, 2u, 67u)
#define PPUTLIMPL_UINT_TRAIT_2143u DEC, 0x85F, 1, 11u, 46u, ()
#define PPUTLIMPL_UINT_TRAIT_2142u DEC, 0x85E, 1, 11u, 46u, (2u, 3u, 3u, 7u, 17u)
#define PPUTLIMPL_UINT_TRAIT_2141u DEC, 0x85D, 1, 11u, 46u, ()
#define PPUTLIMPL_UINT_TRAIT_2140u DEC, 0x85C, 1, 11u, 46u, (2u, 2u, 5u, 107u)
#define PPUTLIMPL_UINT_TRAIT_2139u DEC, 0x85B, 1, 11u, 46u, (3u, 23u, 31u)
#define PPUTLIMPL_UINT_TRAIT_2138u DEC, 0x85A, 1, 11u, 46u, (2u, 1069u)
#define PPUTLIMPL_UINT_TRAIT_2137u DEC, 0x859, 1, 11u, 46u, ()
#define PPUTLIMPL_UINT_TRAIT_2136u DEC, 0x858, 1, 11u, 46u, (2u, 2u, 2u, 3u, 89u)
#define PPUTLIMPL_UINT_TRAIT_2135u DEC, 0x857, 1, 11u, 46u, (5u, 7u, 61u)
#define PPUTLIMPL_UINT_TRAIT_2134u DEC, 0x856, 1, 11u, 46u, (2u, 11u, 97u)
#define PPUTLIMPL_UINT_TRAIT_2133u DEC, 0x855, 1, 11u, 46u, (3u, 3u, 3u, 79u)
#define PPUTLIMPL_UINT_TRAIT_2132u DEC, 0x854, 1, 11u, 46u, (2u, 2u, 13u, 41u)
#define PPUTLIMPL_UINT_TRAIT_2131u DEC, 0x853, 1, 11u, 46u, ()
#define PPUTLIMPL_UINT_TRAIT_2130u DEC, 0x852, 1, 11u, 46u, (2u, 3u, 5u, 71u)
#define PPUTLIMPL_UINT_TRAIT_2129u DEC, 0x851, 1, 11u, 46u, ()
#define PPUTLIMPL_UINT_TRAIT_2128u DEC, 0x850, 1, 11u, 46u, (2u, 2u, 2u, 2u, 7u, 19u)
#define PPUTLIMPL_UINT_TRAIT_2127u DEC, 0x84F, 1, 11u, 46u, (3u, 709u)
#define PPUTLIMPL_UINT_TRAIT_2126u DEC, 0x84E, 1, 11u, 46u, (2u, 1063u)
#define PPUTLIMPL_UINT_TRAIT_2125u DEC, 0x84D, 1, 11u, 46u, (5u, 5u, 5u, 17u)
#define PPUTLIMPL_UINT_TRAIT_2124u DEC, 0x84C, 1, 11u, 46u, (2u, 2u, 3u, 3u, 59u)
#define PPUTLIMPL_UINT_TRAIT_2123u DEC, 0x84B, 1, 11u, 46u, (11u, 193u)
#define PPUTLIMPL_UINT_TRAIT_2122u DEC, 0x84A, 1, 11u, 46u, (2u, 1061u)
#define PPUTLIMPL_UINT_TRAIT_2121u DEC, 0x849, 1, 11u, 46u, (3u, 7u, 101u)
#define PPUTLIMPL_UINT_TRAIT_2120u DEC, 0x848, 1, 11u, 46u, (2u, 2u, 2u, 5u, 53u)
#define PPUTLIMPL_UINT_TRAIT_2119u DEC, 0x847, 1, 11u, 46u, (13u, 163u)
#define PPUTLIMPL_UINT_TRAIT_2118u DEC, 0x846, 1, 11u, 46u, (2u, 3u, 353u)
#define PPUTLIMPL_UINT_TRAIT_2117u DEC, 0x845, 1, 11u, 46u, (29u, 73u)
#define PPUTLIMPL_UINT_TRAIT_2116u DEC, 0x844, 1, 11u, 46u, (2u, 2u, 23u, 23u)
#define PPUTLIMPL_UINT_TRAIT_2115u DEC, 0x843, 1, 11u, 45u, (3u, 3u, 5u, 47u)
#define PPUTLIMPL_UINT_TRAIT_2114u DEC, 0x842, 1, 11u, 45u, (2u, 7u, 151u)
#define PPUTLIMPL_UINT_TRAIT_2113u DEC, 0x841, 1, 11u, 45u, ()
#define PPUTLIMPL_UINT_TRAIT_2112u DEC, 0x840, 1, 11u, 45u, (2u, 2u, 2u, 2u, 2u, 2u, 3u, 11u)
#define PPUTLIMPL_UINT_TRAIT_2111u DEC, 0x83F, 1, 11u, 45u, ()
#define PPUTLIMPL_UINT_TRAIT_2110u DEC, 0x83E, 1, 11u, 45u, (2u, 5u, 211u)
#define PPUTLIMPL_UINT_TRAIT_2109u DEC, 0x83D, 1, 11u, 45u, (3u, 19u, 37u)
#define PPUTLIMPL_UINT_TRAIT_2108u DEC, 0x83C, 1, 11u, 45u, (2u, 2u, 17u, 31u)
#define PPUTLIMPL_UINT_TRAIT_2107u DEC, 0x83B, 1, 11u, 45u, (7u, 7u, 43u)
#define PPUTLIMPL_UINT_TRAIT_2106u DEC, 0x83A, 1, 11u, 45u, (2u, 3u, 3u, 3u, 3u, 13u)
#define PPUTLIMPL_UINT_TRAIT_2105u DEC, 0x839, 1, 11u, 45u, (5u, 421u)
#define PPUTLIMPL_UINT_TRAIT_2104u DEC, 0x838, 1, 11u, 45u, (2u, 2u, 2u, 263u)
#define PPUTLIMPL_UINT_TRAIT_2103u DEC, 0x837, 1, 11u, 45u, (3u, 701u)
#define PPUTLIMPL_UINT_TRAIT_2102u DEC, 0x836, 1, 11u, 45u, (2u, 1051u)
#define PPUTLIMPL_UINT_TRAIT_2101u DEC, 0x835, 1, 11u, 45u, (11u, 191u)
#define PPUTLIMPL_UINT_TRAIT_2100u DEC, 0x834, 1, 11u, 45u, (2u, 2u, 3u, 5u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_2099u DEC, 0x833, 1, 11u, 45u, ()
#define PPUTLIMPL_UINT_TRAIT_2098u DEC, 0x832, 1, 11u, 45u, (2u, 1049u)
#define PPUTLIMPL_UINT_TRAIT_2097u DEC, 0x831, 1, 11u, 45u, (3u, 3u, 233u)
#define PPUTLIMPL_UINT_TRAIT_2096u DEC, 0x830, 1, 11u, 45u, (2u, 2u, 2u, 2u, 131u)
#define PPUTLIMPL_UINT_TRAIT_2095u DEC, 0x82F, 1, 11u, 45u, (5u, 419u)
#define PPUTLIMPL_UINT_TRAIT_2094u DEC, 0x82E, 1, 11u, 45u, (2u, 3u, 349u)
#define PPUTLIMPL_UINT_TRAIT_2093u DEC, 0x82D, 1, 11u, 45u, (7u, 13u, 23u)
#define PPUTLIMPL_UINT_TRAIT_2092u DEC, 0x82C, 1, 11u, 45u, (2u, 2u, 523u)
#define PPUTLIMPL_UINT_TRAIT_2091u DEC, 0x82B, 1, 11u, 45u, (3u, 17u, 41u)
#define PPUTLIMPL_UINT_TRAIT_2090u DEC, 0x82A, 1, 11u, 45u, (2u, 5u, 11u, 19u)
#define PPUTLIMPL_UINT_TRAIT_2089u DEC, 0x829, 1, 11u, 45u, ()
#define PPUTLIMPL_UINT_TRAIT_2088u DEC, 0x828, 1, 11u, 45u, (2u, 2u, 2u, 3u, 3u, 29u)
#define PPUTLIMPL_UINT_TRAIT_2087u DEC, 0x827, 1, 11u, 45u, ()
#define PPUTLIMPL_UINT_TRAIT_2086u DEC, 0x826, 1, 11u, 45u, (2u, 7u, 149u)
#define PPUTLIMPL_UINT_TRAIT_2085u DEC, 0x825, 1, 11u, 45u, (3u, 5u, 139u)
#define PPUTLIMPL_UINT_TRAIT_2084u DEC, 0x824, 1, 11u, 45u, (2u, 2u, 521u)
#define PPUTLIMPL_UINT_TRAIT_2083u DEC, 0x823, 1, 11u, 45u, ()
#define PPUTLIMPL_UINT_TRAIT_2082u DEC, 0x822, 1, 11u, 45u, (2u, 3u, 347u)
#define PPUTLIMPL_UINT_TRAIT_2081u DEC, 0x821, 1, 11u, 45u, ()
#define PPUTLIMPL_UINT_TRAIT_2080u DEC, 0x820, 1, 11u, 45u, (2u, 2u, 2u, 2u, 2u, 5u, 13u)
#define PPUTLIMPL_UINT_TRAIT_2079u DEC, 0x81F, 1, 11u, 45u, (3u, 3u, 3u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_2078u DEC, 0x81E, 1, 11u, 45u, (2u, 1039u)
#define PPUTLIMPL_UINT_TRAIT_2077u DEC, 0x81D, 1, 11u, 45u, (31u, 67u)
#define PPUTLIMPL_UINT_TRAIT_2076u DEC, 0x81C, 1, 11u, 45u, (2u, 2u, 3u, 173u)
#define PPUTLIMPL_UINT_TRAIT_2075u DEC, 0x81B, 1, 11u, 45u, (5u, 5u, 83u)
#define PPUTLIMPL_UINT_TRAIT_2074u DEC, 0x81A, 1, 11u, 45u, (2u, 17u, 61u)
#define PPUTLIMPL_UINT_TRAIT_2073u DEC, 0x819, 1, 11u, 45u, (3u, 691u)
#define PPUTLIMPL_UINT_TRAIT_2072u DEC, 0x818, 1, 11u, 45u, (2u, 2u, 2u, 7u, 37u)
#define PPUTLIMPL_UINT_TRAIT_2071u DEC, 0x817, 1, 11u, 45u, (19u, 109u)
#define PPUTLIMPL_UINT_TRAIT_2070u DEC, 0x816, 1, 11u, 45u, (2u, 3u, 3u, 5u, 23u)
#define PPUTLIMPL_UINT_TRAIT_2069u DEC, 0x815, 1, 11u, 45u, ()
#define PPUTLIMPL_UINT_TRAIT_2068u DEC, 0x814, 1, 11u, 45u, (2u, 2u, 11u, 47u)
#define PPUTLIMPL_UINT_TRAIT_2067u DEC, 0x813, 1, 11u, 45u, (3u, 13u, 53u)
#define PPUTLIMPL_UINT_TRAIT_2066u DEC, 0x812, 1, 11u, 45u, (2u, 1033u)
#define PPUTLIMPL_UINT_TRAIT_2065u DEC, 0x811, 1, 11u, 45u, (5u, 7u, 59u)
#define PPUTLIMPL_UINT_TRAIT_2064u DEC, 0x810, 1, 11u, 45u, (2u, 2u, 2u, 2u, 3u, 43u)
#define PPUTLIMPL_UINT_TRAIT_2063u DEC, 0x80F, 1, 11u, 45u, ()
#define PPUTLIMPL_UINT_TRAIT_2062u DEC, 0x80E, 1, 11u, 45u, (2u, 1031u)
#define PPUTLIMPL_UINT_TRAIT_2061u DEC, 0x80D, 1, 11u, 45u, (3u, 3u, 229u)
#define PPUTLIMPL_UINT_TRAIT_2060u DEC, 0x80C, 1, 11u, 45u, (2u, 2u, 5u, 103u)
#define PPUTLIMPL_UINT_TRAIT_2059u DEC, 0x80B, 1, 11u, 45u, (29u, 71u)
#define PPUTLIMPL_UINT_TRAIT_2058u DEC, 0x80A, 1, 11u, 45u, (2u, 3u, 7u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_2057u DEC, 0x809, 1, 11u, 45u, (11u, 11u, 17u)
#define PPUTLIMPL_UINT_TRAIT_2056u DEC, 0x808, 1, 11u, 45u, (2u, 2u, 2u, 257u)
#define PPUTLIMPL_UINT_TRAIT_2055u DEC, 0x807, 1, 11u, 45u, (3u, 5u, 137u)
#define PPUTLIMPL_UINT_TRAIT_2054u DEC, 0x806, 1, 11u, 45u, (2u, 13u, 79u)
#define PPUTLIMPL_UINT_TRAIT_2053u DEC, 0x805, 1, 11u, 45u, ()
#define PPUTLIMPL_UINT_TRAIT_2052u DEC, 0x804, 1, 11u, 45u, (2u, 2u, 3u, 3u, 3u, 19u)
#define PPUTLIMPL_UINT_TRAIT_2051u DEC, 0x803, 1, 11u, 45u, (7u, 293u)
#define PPUTLIMPL_UINT_TRAIT_2050u DEC, 0x802, 1, 11u, 45u, (2u, 5u, 5u, 41u)
#define PPUTLIMPL_UINT_TRAIT_2049u DEC, 0x801, 1, 11u, 45u, (3u, 683u)
#define PPUTLIMPL_UINT_TRAIT_2048u \
  DEC, 0x800, 1, 11u, 45u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u)
#define PPUTLIMPL_UINT_TRAIT_2047u DEC, 0x7FF, 0, 10u, 45u, (23u, 89u)
#define PPUTLIMPL_UINT_TRAIT_2046u DEC, 0x7FE, 0, 10u, 45u, (2u, 3u, 11u, 31u)
#define PPUTLIMPL_UINT_TRAIT_2045u DEC, 0x7FD, 0, 10u, 45u, (5u, 409u)
#define PPUTLIMPL_UINT_TRAIT_2044u DEC, 0x7FC, 0, 10u, 45u, (2u, 2u, 7u, 73u)
#define PPUTLIMPL_UINT_TRAIT_2043u DEC, 0x7FB, 0, 10u, 45u, (3u, 3u, 227u)
#define PPUTLIMPL_UINT_TRAIT_2042u DEC, 0x7FA, 0, 10u, 45u, (2u, 1021u)
#define PPUTLIMPL_UINT_TRAIT_2041u DEC, 0x7F9, 0, 10u, 45u, (13u, 157u)
#define PPUTLIMPL_UINT_TRAIT_2040u DEC, 0x7F8, 0, 10u, 45u, (2u, 2u, 2u, 3u, 5u, 17u)
#define PPUTLIMPL_UINT_TRAIT_2039u DEC, 0x7F7, 0, 10u, 45u, ()
#define PPUTLIMPL_UINT_TRAIT_2038u DEC, 0x7F6, 0, 10u, 45u, (2u, 1019u)
#define PPUTLIMPL_UINT_TRAIT_2037u DEC, 0x7F5, 0, 10u, 45u, (3u, 7u, 97u)
#define PPUTLIMPL_UINT_TRAIT_2036u DEC, 0x7F4, 0, 10u, 45u, (2u, 2u, 509u)
#define PPUTLIMPL_UINT_TRAIT_2035u DEC, 0x7F3, 0, 10u, 45u, (5u, 11u, 37u)
#define PPUTLIMPL_UINT_TRAIT_2034u DEC, 0x7F2, 0, 10u, 45u, (2u, 3u, 3u, 113u)
#define PPUTLIMPL_UINT_TRAIT_2033u DEC, 0x7F1, 0, 10u, 45u, (19u, 107u)
#define PPUTLIMPL_UINT_TRAIT_2032u DEC, 0x7F0, 0, 10u, 45u, (2u, 2u, 2u, 2u, 127u)
#define PPUTLIMPL_UINT_TRAIT_2031u DEC, 0x7EF, 0, 10u, 45u, (3u, 677u)
#define PPUTLIMPL_UINT_TRAIT_2030u DEC, 0x7EE, 0, 10u, 45u, (2u, 5u, 7u, 29u)
#define PPUTLIMPL_UINT_TRAIT_2029u DEC, 0x7ED, 0, 10u, 45u, ()
#define PPUTLIMPL_UINT_TRAIT_2028u DEC, 0x7EC, 0, 10u, 45u, (2u, 2u, 3u, 13u, 13u)
#define PPUTLIMPL_UINT_TRAIT_2027u DEC, 0x7EB, 0, 10u, 45u, ()
#define PPUTLIMPL_UINT_TRAIT_2026u DEC, 0x7EA, 0, 10u, 45u, (2u, 1013u)
#define PPUTLIMPL_UINT_TRAIT_2025u DEC, 0x7E9, 0, 10u, 45u, (3u, 3u, 3u, 3u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_2024u DEC, 0x7E8, 0, 10u, 44u, (2u, 2u, 2u, 11u, 23u)
#define PPUTLIMPL_UINT_TRAIT_2023u DEC, 0x7E7, 0, 10u, 44u, (7u, 17u, 17u)
#define PPUTLIMPL_UINT_TRAIT_2022u DEC, 0x7E6, 0, 10u, 44u, (2u, 3u, 337u)
#define PPUTLIMPL_UINT_TRAIT_2021u DEC, 0x7E5, 0, 10u, 44u, (43u, 47u)
#define PPUTLIMPL_UINT_TRAIT_2020u DEC, 0x7E4, 0, 10u, 44u, (2u, 2u, 5u, 101u)
#define PPUTLIMPL_UINT_TRAIT_2019u DEC, 0x7E3, 0, 10u, 44u, (3u, 673u)
#define PPUTLIMPL_UINT_TRAIT_2018u DEC, 0x7E2, 0, 10u, 44u, (2u, 1009u)
#define PPUTLIMPL_UINT_TRAIT_2017u DEC, 0x7E1, 0, 10u, 44u, ()
#define PPUTLIMPL_UINT_TRAIT_2016u DEC, 0x7E0, 0, 10u, 44u, (2u, 2u, 2u, 2u, 2u, 3u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_2015u DEC, 0x7DF, 0, 10u, 44u, (5u, 13u, 31u)
#define PPUTLIMPL_UINT_TRAIT_2014u DEC, 0x7DE, 0, 10u, 44u, (2u, 19u, 53u)
#define PPUTLIMPL_UINT_TRAIT_2013u DEC, 0x7DD, 0, 10u, 44u, (3u, 11u, 61u)
#define PPUTLIMPL_UINT_TRAIT_2012u DEC, 0x7DC, 0, 10u, 44u, (2u, 2u, 503u)
#define PPUTLIMPL_UINT_TRAIT_2011u DEC, 0x7DB, 0, 10u, 44u, ()
#define PPUTLIMPL_UINT_TRAIT_2010u DEC, 0x7DA, 0, 10u, 44u, (2u, 3u, 5u, 67u)
#define PPUTLIMPL_UINT_TRAIT_2009u DEC, 0x7D9, 0, 10u, 44u, (7u, 7u, 41u)
#define PPUTLIMPL_UINT_TRAIT_2008u DEC, 0x7D8, 0, 10u, 44u, (2u, 2u, 2u, 251u)
#define PPUTLIMPL_UINT_TRAIT_2007u DEC, 0x7D7, 0, 10u, 44u, (3u, 3u, 223u)
#define PPUTLIMPL_UINT_TRAIT_2006u DEC, 0x7D6, 0, 10u, 44u, (2u, 17u, 59u)
#define PPUTLIMPL_UINT_TRAIT_2005u DEC, 0x7D5, 0, 10u, 44u, (5u, 401u)
#define PPUTLIMPL_UINT_TRAIT_2004u DEC, 0x7D4, 0, 10u, 44u, (2u, 2u, 3u, 167u)
#define PPUTLIMPL_UINT_TRAIT_2003u DEC, 0x7D3, 0, 10u, 44u, ()
#define PPUTLIMPL_UINT_TRAIT_2002u DEC, 0x7D2, 0, 10u, 44u, (2u, 7u, 11u, 13u)
#define PPUTLIMPL_UINT_TRAIT_2001u DEC, 0x7D1, 0, 10u, 44u, (3u, 23u, 29u)
#define PPUTLIMPL_UINT_TRAIT_2000u DEC, 0x7D0, 0, 10u, 44u, (2u, 2u, 2u, 2u, 5u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_1999u DEC, 0x7CF, 0, 10u, 44u, ()
#define PPUTLIMPL_UINT_TRAIT_1998u DEC, 0x7CE, 0, 10u, 44u, (2u, 3u, 3u, 3u, 37u)
#define PPUTLIMPL_UINT_TRAIT_1997u DEC, 0x7CD, 0, 10u, 44u, ()
#define PPUTLIMPL_UINT_TRAIT_1996u DEC, 0x7CC, 0, 10u, 44u, (2u, 2u, 499u)
#define PPUTLIMPL_UINT_TRAIT_1995u DEC, 0x7CB, 0, 10u, 44u, (3u, 5u, 7u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1994u DEC, 0x7CA, 0, 10u, 44u, (2u, 997u)
#define PPUTLIMPL_UINT_TRAIT_1993u DEC, 0x7C9, 0, 10u, 44u, ()
#define PPUTLIMPL_UINT_TRAIT_1992u DEC, 0x7C8, 0, 10u, 44u, (2u, 2u, 2u, 3u, 83u)
#define PPUTLIMPL_UINT_TRAIT_1991u DEC, 0x7C7, 0, 10u, 44u, (11u, 181u)
#define PPUTLIMPL_UINT_TRAIT_1990u DEC, 0x7C6, 0, 10u, 44u, (2u, 5u, 199u)
#define PPUTLIMPL_UINT_TRAIT_1989u DEC, 0x7C5, 0, 10u, 44u, (3u, 3u, 13u, 17u)
#define PPUTLIMPL_UINT_TRAIT_1988u DEC, 0x7C4, 0, 10u, 44u, (2u, 2u, 7u, 71u)
#define PPUTLIMPL_UINT_TRAIT_1987u DEC, 0x7C3, 0, 10u, 44u, ()
#define PPUTLIMPL_UINT_TRAIT_1986u DEC, 0x7C2, 0, 10u, 44u, (2u, 3u, 331u)
#define PPUTLIMPL_UINT_TRAIT_1985u DEC, 0x7C1, 0, 10u, 44u, (5u, 397u)
#define PPUTLIMPL_UINT_TRAIT_1984u DEC, 0x7C0, 0, 10u, 44u, (2u, 2u, 2u, 2u, 2u, 2u, 31u)
#define PPUTLIMPL_UINT_TRAIT_1983u DEC, 0x7BF, 0, 10u, 44u, (3u, 661u)
#define PPUTLIMPL_UINT_TRAIT_1982u DEC, 0x7BE, 0, 10u, 44u, (2u, 991u)
#define PPUTLIMPL_UINT_TRAIT_1981u DEC, 0x7BD, 0, 10u, 44u, (7u, 283u)
#define PPUTLIMPL_UINT_TRAIT_1980u DEC, 0x7BC, 0, 10u, 44u, (2u, 2u, 3u, 3u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_1979u DEC, 0x7BB, 0, 10u, 44u, ()
#define PPUTLIMPL_UINT_TRAIT_1978u DEC, 0x7BA, 0, 10u, 44u, (2u, 23u, 43u)
#define PPUTLIMPL_UINT_TRAIT_1977u DEC, 0x7B9, 0, 10u, 44u, (3u, 659u)
#define PPUTLIMPL_UINT_TRAIT_1976u DEC, 0x7B8, 0, 10u, 44u, (2u, 2u, 2u, 13u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1975u DEC, 0x7B7, 0, 10u, 44u, (5u, 5u, 79u)
#define PPUTLIMPL_UINT_TRAIT_1974u DEC, 0x7B6, 0, 10u, 44u, (2u, 3u, 7u, 47u)
#define PPUTLIMPL_UINT_TRAIT_1973u DEC, 0x7B5, 0, 10u, 44u, ()
#define PPUTLIMPL_UINT_TRAIT_1972u DEC, 0x7B4, 0, 10u, 44u, (2u, 2u, 17u, 29u)
#define PPUTLIMPL_UINT_TRAIT_1971u DEC, 0x7B3, 0, 10u, 44u, (3u, 3u, 3u, 73u)
#define PPUTLIMPL_UINT_TRAIT_1970u DEC, 0x7B2, 0, 10u, 44u, (2u, 5u, 197u)
#define PPUTLIMPL_UINT_TRAIT_1969u DEC, 0x7B1, 0, 10u, 44u, (11u, 179u)
#define PPUTLIMPL_UINT_TRAIT_1968u DEC, 0x7B0, 0, 10u, 44u, (2u, 2u, 2u, 2u, 3u, 41u)
#define PPUTLIMPL_UINT_TRAIT_1967u DEC, 0x7AF, 0, 10u, 44u, (7u, 281u)
#define PPUTLIMPL_UINT_TRAIT_1966u DEC, 0x7AE, 0, 10u, 44u, (2u, 983u)
#define PPUTLIMPL_UINT_TRAIT_1965u DEC, 0x7AD, 0, 10u, 44u, (3u, 5u, 131u)
#define PPUTLIMPL_UINT_TRAIT_1964u DEC, 0x7AC, 0, 10u, 44u, (2u, 2u, 491u)
#define PPUTLIMPL_UINT_TRAIT_1963u DEC, 0x7AB, 0, 10u, 44u, (13u, 151u)
#define PPUTLIMPL_UINT_TRAIT_1962u DEC, 0x7AA, 0, 10u, 44u, (2u, 3u, 3u, 109u)
#define PPUTLIMPL_UINT_TRAIT_1961u DEC, 0x7A9, 0, 10u, 44u, (37u, 53u)
#define PPUTLIMPL_UINT_TRAIT_1960u DEC, 0x7A8, 0, 10u, 44u, (2u, 2u, 2u, 5u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_1959u DEC, 0x7A7, 0, 10u, 44u, (3u, 653u)
#define PPUTLIMPL_UINT_TRAIT_1958u DEC, 0x7A6, 0, 10u, 44u, (2u, 11u, 89u)
#define PPUTLIMPL_UINT_TRAIT_1957u DEC, 0x7A5, 0, 10u, 44u, (19u, 103u)
#define PPUTLIMPL_UINT_TRAIT_1956u DEC, 0x7A4, 0, 10u, 44u, (2u, 2u, 3u, 163u)
#define PPUTLIMPL_UINT_TRAIT_1955u DEC, 0x7A3, 0, 10u, 44u, (5u, 17u, 23u)
#define PPUTLIMPL_UINT_TRAIT_1954u DEC, 0x7A2, 0, 10u, 44u, (2u, 977u)
#define PPUTLIMPL_UINT_TRAIT_1953u DEC, 0x7A1, 0, 10u, 44u, (3u, 3u, 7u, 31u)
#define PPUTLIMPL_UINT_TRAIT_1952u DEC, 0x7A0, 0, 10u, 44u, (2u, 2u, 2u, 2u, 2u, 61u)
#define PPUTLIMPL_UINT_TRAIT_1951u DEC, 0x79F, 0, 10u, 44u, ()
#define PPUTLIMPL_UINT_TRAIT_1950u DEC, 0x79E, 0, 10u, 44u, (2u, 3u, 5u, 5u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1949u DEC, 0x79D, 0, 10u, 44u, ()
#define PPUTLIMPL_UINT_TRAIT_1948u DEC, 0x79C, 0, 10u, 44u, (2u, 2u, 487u)
#define PPUTLIMPL_UINT_TRAIT_1947u DEC, 0x79B, 0, 10u, 44u, (3u, 11u, 59u)
#define PPUTLIMPL_UINT_TRAIT_1946u DEC, 0x79A, 0, 10u, 44u, (2u, 7u, 139u)
#define PPUTLIMPL_UINT_TRAIT_1945u DEC, 0x799, 0, 10u, 44u, (5u, 389u)
#define PPUTLIMPL_UINT_TRAIT_1944u DEC, 0x798, 0, 10u, 44u, (2u, 2u, 2u, 3u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_1943u DEC, 0x797, 0, 10u, 44u, (29u, 67u)
#define PPUTLIMPL_UINT_TRAIT_1942u DEC, 0x796, 0, 10u, 44u, (2u, 971u)
#define PPUTLIMPL_UINT_TRAIT_1941u DEC, 0x795, 0, 10u, 44u, (3u, 647u)
#define PPUTLIMPL_UINT_TRAIT_1940u DEC, 0x794, 0, 10u, 44u, (2u, 2u, 5u, 97u)
#define PPUTLIMPL_UINT_TRAIT_1939u DEC, 0x793, 0, 10u, 44u, (7u, 277u)
#define PPUTLIMPL_UINT_TRAIT_1938u DEC, 0x792, 0, 10u, 44u, (2u, 3u, 17u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1937u DEC, 0x791, 0, 10u, 44u, (13u, 149u)
#define PPUTLIMPL_UINT_TRAIT_1936u DEC, 0x790, 0, 10u, 44u, (2u, 2u, 2u, 2u, 11u, 11u)
#define PPUTLIMPL_UINT_TRAIT_1935u DEC, 0x78F, 0, 10u, 43u, (3u, 3u, 5u, 43u)
#define PPUTLIMPL_UINT_TRAIT_1934u DEC, 0x78E, 0, 10u, 43u, (2u, 967u)
#define PPUTLIMPL_UINT_TRAIT_1933u DEC, 0x78D, 0, 10u, 43u, ()
#define PPUTLIMPL_UINT_TRAIT_1932u DEC, 0x78C, 0, 10u, 43u, (2u, 2u, 3u, 7u, 23u)
#define PPUTLIMPL_UINT_TRAIT_1931u DEC, 0x78B, 0, 10u, 43u, ()
#define PPUTLIMPL_UINT_TRAIT_1930u DEC, 0x78A, 0, 10u, 43u, (2u, 5u, 193u)
#define PPUTLIMPL_UINT_TRAIT_1929u DEC, 0x789, 0, 10u, 43u, (3u, 643u)
#define PPUTLIMPL_UINT_TRAIT_1928u DEC, 0x788, 0, 10u, 43u, (2u, 2u, 2u, 241u)
#define PPUTLIMPL_UINT_TRAIT_1927u DEC, 0x787, 0, 10u, 43u, (41u, 47u)
#define PPUTLIMPL_UINT_TRAIT_1926u DEC, 0x786, 0, 10u, 43u, (2u, 3u, 3u, 107u)
#define PPUTLIMPL_UINT_TRAIT_1925u DEC, 0x785, 0, 10u, 43u, (5u, 5u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_1924u DEC, 0x784, 0, 10u, 43u, (2u, 2u, 13u, 37u)
#define PPUTLIMPL_UINT_TRAIT_1923u DEC, 0x783, 0, 10u, 43u, (3u, 641u)
#define PPUTLIMPL_UINT_TRAIT_1922u DEC, 0x782, 0, 10u, 43u, (2u, 31u, 31u)
#define PPUTLIMPL_UINT_TRAIT_1921u DEC, 0x781, 0, 10u, 43u, (17u, 113u)
#define PPUTLIMPL_UINT_TRAIT_1920u DEC, 0x780, 0, 10u, 43u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_1919u DEC, 0x77F, 0, 10u, 43u, (19u, 101u)
#define PPUTLIMPL_UINT_TRAIT_1918u DEC, 0x77E, 0, 10u, 43u, (2u, 7u, 137u)
#define PPUTLIMPL_UINT_TRAIT_1917u DEC, 0x77D, 0, 10u, 43u, (3u, 3u, 3u, 71u)
#define PPUTLIMPL_UINT_TRAIT_1916u DEC, 0x77C, 0, 10u, 43u, (2u, 2u, 479u)
#define PPUTLIMPL_UINT_TRAIT_1915u DEC, 0x77B, 0, 10u, 43u, (5u, 383u)
#define PPUTLIMPL_UINT_TRAIT_1914u DEC, 0x77A, 0, 10u, 43u, (2u, 3u, 11u, 29u)
#define PPUTLIMPL_UINT_TRAIT_1913u DEC, 0x779, 0, 10u, 43u, ()
#define PPUTLIMPL_UINT_TRAIT_1912u DEC, 0x778, 0, 10u, 43u, (2u, 2u, 2u, 239u)
#define PPUTLIMPL_UINT_TRAIT_1911u DEC, 0x777, 0, 10u, 43u, (3u, 7u, 7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1910u DEC, 0x776, 0, 10u, 43u, (2u, 5u, 191u)
#define PPUTLIMPL_UINT_TRAIT_1909u DEC, 0x775, 0, 10u, 43u, (23u, 83u)
#define PPUTLIMPL_UINT_TRAIT_1908u DEC, 0x774, 0, 10u, 43u, (2u, 2u, 3u, 3u, 53u)
#define PPUTLIMPL_UINT_TRAIT_1907u DEC, 0x773, 0, 10u, 43u, ()
#define PPUTLIMPL_UINT_TRAIT_1906u DEC, 0x772, 0, 10u, 43u, (2u, 953u)
#define PPUTLIMPL_UINT_TRAIT_1905u DEC, 0x771, 0, 10u, 43u, (3u, 5u, 127u)
#define PPUTLIMPL_UINT_TRAIT_1904u DEC, 0x770, 0, 10u, 43u, (2u, 2u, 2u, 2u, 7u, 17u)
#define PPUTLIMPL_UINT_TRAIT_1903u DEC, 0x76F, 0, 10u, 43u, (11u, 173u)
#define PPUTLIMPL_UINT_TRAIT_1902u DEC, 0x76E, 0, 10u, 43u, (2u, 3u, 317u)
#define PPUTLIMPL_UINT_TRAIT_1901u DEC, 0x76D, 0, 10u, 43u, ()
#define PPUTLIMPL_UINT_TRAIT_1900u DEC, 0x76C, 0, 10u, 43u, (2u, 2u, 5u, 5u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1899u DEC, 0x76B, 0, 10u, 43u, (3u, 3u, 211u)
#define PPUTLIMPL_UINT_TRAIT_1898u DEC, 0x76A, 0, 10u, 43u, (2u, 13u, 73u)
#define PPUTLIMPL_UINT_TRAIT_1897u DEC, 0x769, 0, 10u, 43u, (7u, 271u)
#define PPUTLIMPL_UINT_TRAIT_1896u DEC, 0x768, 0, 10u, 43u, (2u, 2u, 2u, 3u, 79u)
#define PPUTLIMPL_UINT_TRAIT_1895u DEC, 0x767, 0, 10u, 43u, (5u, 379u)
#define PPUTLIMPL_UINT_TRAIT_1894u DEC, 0x766, 0, 10u, 43u, (2u, 947u)
#define PPUTLIMPL_UINT_TRAIT_1893u DEC, 0x765, 0, 10u, 43u, (3u, 631u)
#define PPUTLIMPL_UINT_TRAIT_1892u DEC, 0x764, 0, 10u, 43u, (2u, 2u, 11u, 43u)
#define PPUTLIMPL_UINT_TRAIT_1891u DEC, 0x763, 0, 10u, 43u, (31u, 61u)
#define PPUTLIMPL_UINT_TRAIT_1890u DEC, 0x762, 0, 10u, 43u, (2u, 3u, 3u, 3u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_1889u DEC, 0x761, 0, 10u, 43u, ()
#define PPUTLIMPL_UINT_TRAIT_1888u DEC, 0x760, 0, 10u, 43u, (2u, 2u, 2u, 2u, 2u, 59u)
#define PPUTLIMPL_UINT_TRAIT_1887u DEC, 0x75F, 0, 10u, 43u, (3u, 17u, 37u)
#define PPUTLIMPL_UINT_TRAIT_1886u DEC, 0x75E, 0, 10u, 43u, (2u, 23u, 41u)
#define PPUTLIMPL_UINT_TRAIT_1885u DEC, 0x75D, 0, 10u, 43u, (5u, 13u, 29u)
#define PPUTLIMPL_UINT_TRAIT_1884u DEC, 0x75C, 0, 10u, 43u, (2u, 2u, 3u, 157u)
#define PPUTLIMPL_UINT_TRAIT_1883u DEC, 0x75B, 0, 10u, 43u, (7u, 269u)
#define PPUTLIMPL_UINT_TRAIT_1882u DEC, 0x75A, 0, 10u, 43u, (2u, 941u)
#define PPUTLIMPL_UINT_TRAIT_1881u DEC, 0x759, 0, 10u, 43u, (3u, 3u, 11u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1880u DEC, 0x758, 0, 10u, 43u, (2u, 2u, 2u, 5u, 47u)
#define PPUTLIMPL_UINT_TRAIT_1879u DEC, 0x757, 0, 10u, 43u, ()
#define PPUTLIMPL_UINT_TRAIT_1878u DEC, 0x756, 0, 10u, 43u, (2u, 3u, 313u)
#define PPUTLIMPL_UINT_TRAIT_1877u DEC, 0x755, 0, 10u, 43u, ()
#define PPUTLIMPL_UINT_TRAIT_1876u DEC, 0x754, 0, 10u, 43u, (2u, 2u, 7u, 67u)
#define PPUTLIMPL_UINT_TRAIT_1875u DEC, 0x753, 0, 10u, 43u, (3u, 5u, 5u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_1874u DEC, 0x752, 0, 10u, 43u, (2u, 937u)
#define PPUTLIMPL_UINT_TRAIT_1873u DEC, 0x751, 0, 10u, 43u, ()
#define PPUTLIMPL_UINT_TRAIT_1872u DEC, 0x750, 0, 10u, 43u, (2u, 2u, 2u, 2u, 3u, 3u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1871u DEC, 0x74F, 0, 10u, 43u, ()
#define PPUTLIMPL_UINT_TRAIT_1870u DEC, 0x74E, 0, 10u, 43u, (2u, 5u, 11u, 17u)
#define PPUTLIMPL_UINT_TRAIT_1869u DEC, 0x74D, 0, 10u, 43u, (3u, 7u, 89u)
#define PPUTLIMPL_UINT_TRAIT_1868u DEC, 0x74C, 0, 10u, 43u, (2u, 2u, 467u)
#define PPUTLIMPL_UINT_TRAIT_1867u DEC, 0x74B, 0, 10u, 43u, ()
#define PPUTLIMPL_UINT_TRAIT_1866u DEC, 0x74A, 0, 10u, 43u, (2u, 3u, 311u)
#define PPUTLIMPL_UINT_TRAIT_1865u DEC, 0x749, 0, 10u, 43u, (5u, 373u)
#define PPUTLIMPL_UINT_TRAIT_1864u DEC, 0x748, 0, 10u, 43u, (2u, 2u, 2u, 233u)
#define PPUTLIMPL_UINT_TRAIT_1863u DEC, 0x747, 0, 10u, 43u, (3u, 3u, 3u, 3u, 23u)
#define PPUTLIMPL_UINT_TRAIT_1862u DEC, 0x746, 0, 10u, 43u, (2u, 7u, 7u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1861u DEC, 0x745, 0, 10u, 43u, ()
#define PPUTLIMPL_UINT_TRAIT_1860u DEC, 0x744, 0, 10u, 43u, (2u, 2u, 3u, 5u, 31u)
#define PPUTLIMPL_UINT_TRAIT_1859u DEC, 0x743, 0, 10u, 43u, (11u, 13u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1858u DEC, 0x742, 0, 10u, 43u, (2u, 929u)
#define PPUTLIMPL_UINT_TRAIT_1857u DEC, 0x741, 0, 10u, 43u, (3u, 619u)
#define PPUTLIMPL_UINT_TRAIT_1856u DEC, 0x740, 0, 10u, 43u, (2u, 2u, 2u, 2u, 2u, 2u, 29u)
#define PPUTLIMPL_UINT_TRAIT_1855u DEC, 0x73F, 0, 10u, 43u, (5u, 7u, 53u)
#define PPUTLIMPL_UINT_TRAIT_1854u DEC, 0x73E, 0, 10u, 43u, (2u, 3u, 3u, 103u)
#define PPUTLIMPL_UINT_TRAIT_1853u DEC, 0x73D, 0, 10u, 43u, (17u, 109u)
#define PPUTLIMPL_UINT_TRAIT_1852u DEC, 0x73C, 0, 10u, 43u, (2u, 2u, 463u)
#define PPUTLIMPL_UINT_TRAIT_1851u DEC, 0x73B, 0, 10u, 43u, (3u, 617u)
#define PPUTLIMPL_UINT_TRAIT_1850u DEC, 0x73A, 0, 10u, 43u, (2u, 5u, 5u, 37u)
#define PPUTLIMPL_UINT_TRAIT_1849u DEC, 0x739, 0, 10u, 43u, (43u, 43u)
#define PPUTLIMPL_UINT_TRAIT_1848u DEC, 0x738, 0, 10u, 42u, (2u, 2u, 2u, 3u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_1847u DEC, 0x737, 0, 10u, 42u, ()
#define PPUTLIMPL_UINT_TRAIT_1846u DEC, 0x736, 0, 10u, 42u, (2u, 13u, 71u)
#define PPUTLIMPL_UINT_TRAIT_1845u DEC, 0x735, 0, 10u, 42u, (3u, 3u, 5u, 41u)
#define PPUTLIMPL_UINT_TRAIT_1844u DEC, 0x734, 0, 10u, 42u, (2u, 2u, 461u)
#define PPUTLIMPL_UINT_TRAIT_1843u DEC, 0x733, 0, 10u, 42u, (19u, 97u)
#define PPUTLIMPL_UINT_TRAIT_1842u DEC, 0x732, 0, 10u, 42u, (2u, 3u, 307u)
#define PPUTLIMPL_UINT_TRAIT_1841u DEC, 0x731, 0, 10u, 42u, (7u, 263u)
#define PPUTLIMPL_UINT_TRAIT_1840u DEC, 0x730, 0, 10u, 42u, (2u, 2u, 2u, 2u, 5u, 23u)
#define PPUTLIMPL_UINT_TRAIT_1839u DEC, 0x72F, 0, 10u, 42u, (3u, 613u)
#define PPUTLIMPL_UINT_TRAIT_1838u DEC, 0x72E, 0, 10u, 42u, (2u, 919u)
#define PPUTLIMPL_UINT_TRAIT_1837u DEC, 0x72D, 0, 10u, 42u, (11u, 167u)
#define PPUTLIMPL_UINT_TRAIT_1836u DEC, 0x72C, 0, 10u, 42u, (2u, 2u, 3u, 3u, 3u, 17u)
#define PPUTLIMPL_UINT_TRAIT_1835u DEC, 0x72B, 0, 10u, 42u, (5u, 367u)
#define PPUTLIMPL_UINT_TRAIT_1834u DEC, 0x72A, 0, 10u, 42u, (2u, 7u, 131u)
#define PPUTLIMPL_UINT_TRAIT_1833u DEC, 0x729, 0, 10u, 42u, (3u, 13u, 47u)
#define PPUTLIMPL_UINT_TRAIT_1832u DEC, 0x728, 0, 10u, 42u, (2u, 2u, 2u, 229u)
#define PPUTLIMPL_UINT_TRAIT_1831u DEC, 0x727, 0, 10u, 42u, ()
#define PPUTLIMPL_UINT_TRAIT_1830u DEC, 0x726, 0, 10u, 42u, (2u, 3u, 5u, 61u)
#define PPUTLIMPL_UINT_TRAIT_1829u DEC, 0x725, 0, 10u, 42u, (31u, 59u)
#define PPUTLIMPL_UINT_TRAIT_1828u DEC, 0x724, 0, 10u, 42u, (2u, 2u, 457u)
#define PPUTLIMPL_UINT_TRAIT_1827u DEC, 0x723, 0, 10u, 42u, (3u, 3u, 7u, 29u)
#define PPUTLIMPL_UINT_TRAIT_1826u DEC, 0x722, 0, 10u, 42u, (2u, 11u, 83u)
#define PPUTLIMPL_UINT_TRAIT_1825u DEC, 0x721, 0, 10u, 42u, (5u, 5u, 73u)
#define PPUTLIMPL_UINT_TRAIT_1824u DEC, 0x720, 0, 10u, 42u, (2u, 2u, 2u, 2u, 2u, 3u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1823u DEC, 0x71F, 0, 10u, 42u, ()
#define PPUTLIMPL_UINT_TRAIT_1822u DEC, 0x71E, 0, 10u, 42u, (2u, 911u)
#define PPUTLIMPL_UINT_TRAIT_1821u DEC, 0x71D, 0, 10u, 42u, (3u, 607u)
#define PPUTLIMPL_UINT_TRAIT_1820u DEC, 0x71C, 0, 10u, 42u, (2u, 2u, 5u, 7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1819u DEC, 0x71B, 0, 10u, 42u, (17u, 107u)
#define PPUTLIMPL_UINT_TRAIT_1818u DEC, 0x71A, 0, 10u, 42u, (2u, 3u, 3u, 101u)
#define PPUTLIMPL_UINT_TRAIT_1817u DEC, 0x719, 0, 10u, 42u, (23u, 79u)
#define PPUTLIMPL_UINT_TRAIT_1816u DEC, 0x718, 0, 10u, 42u, (2u, 2u, 2u, 227u)
#define PPUTLIMPL_UINT_TRAIT_1815u DEC, 0x717, 0, 10u, 42u, (3u, 5u, 11u, 11u)
#define PPUTLIMPL_UINT_TRAIT_1814u DEC, 0x716, 0, 10u, 42u, (2u, 907u)
#define PPUTLIMPL_UINT_TRAIT_1813u DEC, 0x715, 0, 10u, 42u, (7u, 7u, 37u)
#define PPUTLIMPL_UINT_TRAIT_1812u DEC, 0x714, 0, 10u, 42u, (2u, 2u, 3u, 151u)
#define PPUTLIMPL_UINT_TRAIT_1811u DEC, 0x713, 0, 10u, 42u, ()
#define PPUTLIMPL_UINT_TRAIT_1810u DEC, 0x712, 0, 10u, 42u, (2u, 5u, 181u)
#define PPUTLIMPL_UINT_TRAIT_1809u DEC, 0x711, 0, 10u, 42u, (3u, 3u, 3u, 67u)
#define PPUTLIMPL_UINT_TRAIT_1808u DEC, 0x710, 0, 10u, 42u, (2u, 2u, 2u, 2u, 113u)
#define PPUTLIMPL_UINT_TRAIT_1807u DEC, 0x70F, 0, 10u, 42u, (13u, 139u)
#define PPUTLIMPL_UINT_TRAIT_1806u DEC, 0x70E, 0, 10u, 42u, (2u, 3u, 7u, 43u)
#define PPUTLIMPL_UINT_TRAIT_1805u DEC, 0x70D, 0, 10u, 42u, (5u, 19u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1804u DEC, 0x70C, 0, 10u, 42u, (2u, 2u, 11u, 41u)
#define PPUTLIMPL_UINT_TRAIT_1803u DEC, 0x70B, 0, 10u, 42u, (3u, 601u)
#define PPUTLIMPL_UINT_TRAIT_1802u DEC, 0x70A, 0, 10u, 42u, (2u, 17u, 53u)
#define PPUTLIMPL_UINT_TRAIT_1801u DEC, 0x709, 0, 10u, 42u, ()
#define PPUTLIMPL_UINT_TRAIT_1800u DEC, 0x708, 0, 10u, 42u, (2u, 2u, 2u, 3u, 3u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_1799u DEC, 0x707, 0, 10u, 42u, (7u, 257u)
#define PPUTLIMPL_UINT_TRAIT_1798u DEC, 0x706, 0, 10u, 42u, (2u, 29u, 31u)
#define PPUTLIMPL_UINT_TRAIT_1797u DEC, 0x705, 0, 10u, 42u, (3u, 599u)
#define PPUTLIMPL_UINT_TRAIT_1796u DEC, 0x704, 0, 10u, 42u, (2u, 2u, 449u)
#define PPUTLIMPL_UINT_TRAIT_1795u DEC, 0x703, 0, 10u, 42u, (5u, 359u)
#define PPUTLIMPL_UINT_TRAIT_1794u DEC, 0x702, 0, 10u, 42u, (2u, 3u, 13u, 23u)
#define PPUTLIMPL_UINT_TRAIT_1793u DEC, 0x701, 0, 10u, 42u, (11u, 163u)
#define PPUTLIMPL_UINT_TRAIT_1792u DEC, 0x700, 0, 10u, 42u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 7u)
#define PPUTLIMPL_UINT_TRAIT_1791u DEC, 0x6FF, 0, 10u, 42u, (3u, 3u, 199u)
#define PPUTLIMPL_UINT_TRAIT_1790u DEC, 0x6FE, 0, 10u, 42u, (2u, 5u, 179u)
#define PPUTLIMPL_UINT_TRAIT_1789u DEC, 0x6FD, 0, 10u, 42u, ()
#define PPUTLIMPL_UINT_TRAIT_1788u DEC, 0x6FC, 0, 10u, 42u, (2u, 2u, 3u, 149u)
#define PPUTLIMPL_UINT_TRAIT_1787u DEC, 0x6FB, 0, 10u, 42u, ()
#define PPUTLIMPL_UINT_TRAIT_1786u DEC, 0x6FA, 0, 10u, 42u, (2u, 19u, 47u)
#define PPUTLIMPL_UINT_TRAIT_1785u DEC, 0x6F9, 0, 10u, 42u, (3u, 5u, 7u, 17u)
#define PPUTLIMPL_UINT_TRAIT_1784u DEC, 0x6F8, 0, 10u, 42u, (2u, 2u, 2u, 223u)
#define PPUTLIMPL_UINT_TRAIT_1783u DEC, 0x6F7, 0, 10u, 42u, ()
#define PPUTLIMPL_UINT_TRAIT_1782u DEC, 0x6F6, 0, 10u, 42u, (2u, 3u, 3u, 3u, 3u, 11u)
#define PPUTLIMPL_UINT_TRAIT_1781u DEC, 0x6F5, 0, 10u, 42u, (13u, 137u)
#define PPUTLIMPL_UINT_TRAIT_1780u DEC, 0x6F4, 0, 10u, 42u, (2u, 2u, 5u, 89u)
#define PPUTLIMPL_UINT_TRAIT_1779u DEC, 0x6F3, 0, 10u, 42u, (3u, 593u)
#define PPUTLIMPL_UINT_TRAIT_1778u DEC, 0x6F2, 0, 10u, 42u, (2u, 7u, 127u)
#define PPUTLIMPL_UINT_TRAIT_1777u DEC, 0x6F1, 0, 10u, 42u, ()
#define PPUTLIMPL_UINT_TRAIT_1776u DEC, 0x6F0, 0, 10u, 42u, (2u, 2u, 2u, 2u, 3u, 37u)
#define PPUTLIMPL_UINT_TRAIT_1775u DEC, 0x6EF, 0, 10u, 42u, (5u, 5u, 71u)
#define PPUTLIMPL_UINT_TRAIT_1774u DEC, 0x6EE, 0, 10u, 42u, (2u, 887u)
#define PPUTLIMPL_UINT_TRAIT_1773u DEC, 0x6ED, 0, 10u, 42u, (3u, 3u, 197u)
#define PPUTLIMPL_UINT_TRAIT_1772u DEC, 0x6EC, 0, 10u, 42u, (2u, 2u, 443u)
#define PPUTLIMPL_UINT_TRAIT_1771u DEC, 0x6EB, 0, 10u, 42u, (7u, 11u, 23u)
#define PPUTLIMPL_UINT_TRAIT_1770u DEC, 0x6EA, 0, 10u, 42u, (2u, 3u, 5u, 59u)
#define PPUTLIMPL_UINT_TRAIT_1769u DEC, 0x6E9, 0, 10u, 42u, (29u, 61u)
#define PPUTLIMPL_UINT_TRAIT_1768u DEC, 0x6E8, 0, 10u, 42u, (2u, 2u, 2u, 13u, 17u)
#define PPUTLIMPL_UINT_TRAIT_1767u DEC, 0x6E7, 0, 10u, 42u, (3u, 19u, 31u)
#define PPUTLIMPL_UINT_TRAIT_1766u DEC, 0x6E6, 0, 10u, 42u, (2u, 883u)
#define PPUTLIMPL_UINT_TRAIT_1765u DEC, 0x6E5, 0, 10u, 42u, (5u, 353u)
#define PPUTLIMPL_UINT_TRAIT_1764u DEC, 0x6E4, 0, 10u, 42u, (2u, 2u, 3u, 3u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_1763u DEC, 0x6E3, 0, 10u, 41u, (41u, 43u)
#define PPUTLIMPL_UINT_TRAIT_1762u DEC, 0x6E2, 0, 10u, 41u, (2u, 881u)
#define PPUTLIMPL_UINT_TRAIT_1761u DEC, 0x6E1, 0, 10u, 41u, (3u, 587u)
#define PPUTLIMPL_UINT_TRAIT_1760u DEC, 0x6E0, 0, 10u, 41u, (2u, 2u, 2u, 2u, 2u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_1759u DEC, 0x6DF, 0, 10u, 41u, ()
#define PPUTLIMPL_UINT_TRAIT_1758u DEC, 0x6DE, 0, 10u, 41u, (2u, 3u, 293u)
#define PPUTLIMPL_UINT_TRAIT_1757u DEC, 0x6DD, 0, 10u, 41u, (7u, 251u)
#define PPUTLIMPL_UINT_TRAIT_1756u DEC, 0x6DC, 0, 10u, 41u, (2u, 2u, 439u)
#define PPUTLIMPL_UINT_TRAIT_1755u DEC, 0x6DB, 0, 10u, 41u, (3u, 3u, 3u, 5u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1754u DEC, 0x6DA, 0, 10u, 41u, (2u, 877u)
#define PPUTLIMPL_UINT_TRAIT_1753u DEC, 0x6D9, 0, 10u, 41u, ()
#define PPUTLIMPL_UINT_TRAIT_1752u DEC, 0x6D8, 0, 10u, 41u, (2u, 2u, 2u, 3u, 73u)
#define PPUTLIMPL_UINT_TRAIT_1751u DEC, 0x6D7, 0, 10u, 41u, (17u, 103u)
#define PPUTLIMPL_UINT_TRAIT_1750u DEC, 0x6D6, 0, 10u, 41u, (2u, 5u, 5u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_1749u DEC, 0x6D5, 0, 10u, 41u, (3u, 11u, 53u)
#define PPUTLIMPL_UINT_TRAIT_1748u DEC, 0x6D4, 0, 10u, 41u, (2u, 2u, 19u, 23u)
#define PPUTLIMPL_UINT_TRAIT_1747u DEC, 0x6D3, 0, 10u, 41u, ()
#define PPUTLIMPL_UINT_TRAIT_1746u DEC, 0x6D2, 0, 10u, 41u, (2u, 3u, 3u, 97u)
#define PPUTLIMPL_UINT_TRAIT_1745u DEC, 0x6D1, 0, 10u, 41u, (5u, 349u)
#define PPUTLIMPL_UINT_TRAIT_1744u DEC, 0x6D0, 0, 10u, 41u, (2u, 2u, 2u, 2u, 109u)
#define PPUTLIMPL_UINT_TRAIT_1743u DEC, 0x6CF, 0, 10u, 41u, (3u, 7u, 83u)
#define PPUTLIMPL_UINT_TRAIT_1742u DEC, 0x6CE, 0, 10u, 41u, (2u, 13u, 67u)
#define PPUTLIMPL_UINT_TRAIT_1741u DEC, 0x6CD, 0, 10u, 41u, ()
#define PPUTLIMPL_UINT_TRAIT_1740u DEC, 0x6CC, 0, 10u, 41u, (2u, 2u, 3u, 5u, 29u)
#define PPUTLIMPL_UINT_TRAIT_1739u DEC, 0x6CB, 0, 10u, 41u, (37u, 47u)
#define PPUTLIMPL_UINT_TRAIT_1738u DEC, 0x6CA, 0, 10u, 41u, (2u, 11u, 79u)
#define PPUTLIMPL_UINT_TRAIT_1737u DEC, 0x6C9, 0, 10u, 41u, (3u, 3u, 193u)
#define PPUTLIMPL_UINT_TRAIT_1736u DEC, 0x6C8, 0, 10u, 41u, (2u, 2u, 2u, 7u, 31u)
#define PPUTLIMPL_UINT_TRAIT_1735u DEC, 0x6C7, 0, 10u, 41u, (5u, 347u)
#define PPUTLIMPL_UINT_TRAIT_1734u DEC, 0x6C6, 0, 10u, 41u, (2u, 3u, 17u, 17u)
#define PPUTLIMPL_UINT_TRAIT_1733u DEC, 0x6C5, 0, 10u, 41u, ()
#define PPUTLIMPL_UINT_TRAIT_1732u DEC, 0x6C4, 0, 10u, 41u, (2u, 2u, 433u)
#define PPUTLIMPL_UINT_TRAIT_1731u DEC, 0x6C3, 0, 10u, 41u, (3u, 577u)
#define PPUTLIMPL_UINT_TRAIT_1730u DEC, 0x6C2, 0, 10u, 41u, (2u, 5u, 173u)
#define PPUTLIMPL_UINT_TRAIT_1729u DEC, 0x6C1, 0, 10u, 41u, (7u, 13u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1728u DEC, 0x6C0, 0, 10u, 41u, (2u, 2u, 2u, 2u, 2u, 2u, 3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_1727u DEC, 0x6BF, 0, 10u, 41u, (11u, 157u)
#define PPUTLIMPL_UINT_TRAIT_1726u DEC, 0x6BE, 0, 10u, 41u, (2u, 863u)
#define PPUTLIMPL_UINT_TRAIT_1725u DEC, 0x6BD, 0, 10u, 41u, (3u, 5u, 5u, 23u)
#define PPUTLIMPL_UINT_TRAIT_1724u DEC, 0x6BC, 0, 10u, 41u, (2u, 2u, 431u)
#define PPUTLIMPL_UINT_TRAIT_1723u DEC, 0x6BB, 0, 10u, 41u, ()
#define PPUTLIMPL_UINT_TRAIT_1722u DEC, 0x6BA, 0, 10u, 41u, (2u, 3u, 7u, 41u)
#define PPUTLIMPL_UINT_TRAIT_1721u DEC, 0x6B9, 0, 10u, 41u, ()
#define PPUTLIMPL_UINT_TRAIT_1720u DEC, 0x6B8, 0, 10u, 41u, (2u, 2u, 2u, 5u, 43u)
#define PPUTLIMPL_UINT_TRAIT_1719u DEC, 0x6B7, 0, 10u, 41u, (3u, 3u, 191u)
#define PPUTLIMPL_UINT_TRAIT_1718u DEC, 0x6B6, 0, 10u, 41u, (2u, 859u)
#define PPUTLIMPL_UINT_TRAIT_1717u DEC, 0x6B5, 0, 10u, 41u, (17u, 101u)
#define PPUTLIMPL_UINT_TRAIT_1716u DEC, 0x6B4, 0, 10u, 41u, (2u, 2u, 3u, 11u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1715u DEC, 0x6B3, 0, 10u, 41u, (5u, 7u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_1714u DEC, 0x6B2, 0, 10u, 41u, (2u, 857u)
#define PPUTLIMPL_UINT_TRAIT_1713u DEC, 0x6B1, 0, 10u, 41u, (3u, 571u)
#define PPUTLIMPL_UINT_TRAIT_1712u DEC, 0x6B0, 0, 10u, 41u, (2u, 2u, 2u, 2u, 107u)
#define PPUTLIMPL_UINT_TRAIT_1711u DEC, 0x6AF, 0, 10u, 41u, (29u, 59u)
#define PPUTLIMPL_UINT_TRAIT_1710u DEC, 0x6AE, 0, 10u, 41u, (2u, 3u, 3u, 5u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1709u DEC, 0x6AD, 0, 10u, 41u, ()
#define PPUTLIMPL_UINT_TRAIT_1708u DEC, 0x6AC, 0, 10u, 41u, (2u, 2u, 7u, 61u)
#define PPUTLIMPL_UINT_TRAIT_1707u DEC, 0x6AB, 0, 10u, 41u, (3u, 569u)
#define PPUTLIMPL_UINT_TRAIT_1706u DEC, 0x6AA, 0, 10u, 41u, (2u, 853u)
#define PPUTLIMPL_UINT_TRAIT_1705u DEC, 0x6A9, 0, 10u, 41u, (5u, 11u, 31u)
#define PPUTLIMPL_UINT_TRAIT_1704u DEC, 0x6A8, 0, 10u, 41u, (2u, 2u, 2u, 3u, 71u)
#define PPUTLIMPL_UINT_TRAIT_1703u DEC, 0x6A7, 0, 10u, 41u, (13u, 131u)
#define PPUTLIMPL_UINT_TRAIT_1702u DEC, 0x6A6, 0, 10u, 41u, (2u, 23u, 37u)
#define PPUTLIMPL_UINT_TRAIT_1701u DEC, 0x6A5, 0, 10u, 41u, (3u, 3u, 3u, 3u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_1700u DEC, 0x6A4, 0, 10u, 41u, (2u, 2u, 5u, 5u, 17u)
#define PPUTLIMPL_UINT_TRAIT_1699u DEC, 0x6A3, 0, 10u, 41u, ()
#define PPUTLIMPL_UINT_TRAIT_1698u DEC, 0x6A2, 0, 10u, 41u, (2u, 3u, 283u)
#define PPUTLIMPL_UINT_TRAIT_1697u DEC, 0x6A1, 0, 10u, 41u, ()
#define PPUTLIMPL_UINT_TRAIT_1696u DEC, 0x6A0, 0, 10u, 41u, (2u, 2u, 2u, 2u, 2u, 53u)
#define PPUTLIMPL_UINT_TRAIT_1695u DEC, 0x69F, 0, 10u, 41u, (3u, 5u, 113u)
#define PPUTLIMPL_UINT_TRAIT_1694u DEC, 0x69E, 0, 10u, 41u, (2u, 7u, 11u, 11u)
#define PPUTLIMPL_UINT_TRAIT_1693u DEC, 0x69D, 0, 10u, 41u, ()
#define PPUTLIMPL_UINT_TRAIT_1692u DEC, 0x69C, 0, 10u, 41u, (2u, 2u, 3u, 3u, 47u)
#define PPUTLIMPL_UINT_TRAIT_1691u DEC, 0x69B, 0, 10u, 41u, (19u, 89u)
#define PPUTLIMPL_UINT_TRAIT_1690u DEC, 0x69A, 0, 10u, 41u, (2u, 5u, 13u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1689u DEC, 0x699, 0, 10u, 41u, (3u, 563u)
#define PPUTLIMPL_UINT_TRAIT_1688u DEC, 0x698, 0, 10u, 41u, (2u, 2u, 2u, 211u)
#define PPUTLIMPL_UINT_TRAIT_1687u DEC, 0x697, 0, 10u, 41u, (7u, 241u)
#define PPUTLIMPL_UINT_TRAIT_1686u DEC, 0x696, 0, 10u, 41u, (2u, 3u, 281u)
#define PPUTLIMPL_UINT_TRAIT_1685u DEC, 0x695, 0, 10u, 41u, (5u, 337u)
#define PPUTLIMPL_UINT_TRAIT_1684u DEC, 0x694, 0, 10u, 41u, (2u, 2u, 421u)
#define PPUTLIMPL_UINT_TRAIT_1683u DEC, 0x693, 0, 10u, 41u, (3u, 3u, 11u, 17u)
#define PPUTLIMPL_UINT_TRAIT_1682u DEC, 0x692, 0, 10u, 41u, (2u, 29u, 29u)
#define PPUTLIMPL_UINT_TRAIT_1681u DEC, 0x691, 0, 10u, 41u, (41u, 41u)
#define PPUTLIMPL_UINT_TRAIT_1680u DEC, 0x690, 0, 10u, 40u, (2u, 2u, 2u, 2u, 3u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_1679u DEC, 0x68F, 0, 10u, 40u, (23u, 73u)
#define PPUTLIMPL_UINT_TRAIT_1678u DEC, 0x68E, 0, 10u, 40u, (2u, 839u)
#define PPUTLIMPL_UINT_TRAIT_1677u DEC, 0x68D, 0, 10u, 40u, (3u, 13u, 43u)
#define PPUTLIMPL_UINT_TRAIT_1676u DEC, 0x68C, 0, 10u, 40u, (2u, 2u, 419u)
#define PPUTLIMPL_UINT_TRAIT_1675u DEC, 0x68B, 0, 10u, 40u, (5u, 5u, 67u)
#define PPUTLIMPL_UINT_TRAIT_1674u DEC, 0x68A, 0, 10u, 40u, (2u, 3u, 3u, 3u, 31u)
#define PPUTLIMPL_UINT_TRAIT_1673u DEC, 0x689, 0, 10u, 40u, (7u, 239u)
#define PPUTLIMPL_UINT_TRAIT_1672u DEC, 0x688, 0, 10u, 40u, (2u, 2u, 2u, 11u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1671u DEC, 0x687, 0, 10u, 40u, (3u, 557u)
#define PPUTLIMPL_UINT_TRAIT_1670u DEC, 0x686, 0, 10u, 40u, (2u, 5u, 167u)
#define PPUTLIMPL_UINT_TRAIT_1669u DEC, 0x685, 0, 10u, 40u, ()
#define PPUTLIMPL_UINT_TRAIT_1668u DEC, 0x684, 0, 10u, 40u, (2u, 2u, 3u, 139u)
#define PPUTLIMPL_UINT_TRAIT_1667u DEC, 0x683, 0, 10u, 40u, ()
#define PPUTLIMPL_UINT_TRAIT_1666u DEC, 0x682, 0, 10u, 40u, (2u, 7u, 7u, 17u)
#define PPUTLIMPL_UINT_TRAIT_1665u DEC, 0x681, 0, 10u, 40u, (3u, 3u, 5u, 37u)
#define PPUTLIMPL_UINT_TRAIT_1664u DEC, 0x680, 0, 10u, 40u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1663u DEC, 0x67F, 0, 10u, 40u, ()
#define PPUTLIMPL_UINT_TRAIT_1662u DEC, 0x67E, 0, 10u, 40u, (2u, 3u, 277u)
#define PPUTLIMPL_UINT_TRAIT_1661u DEC, 0x67D, 0, 10u, 40u, (11u, 151u)
#define PPUTLIMPL_UINT_TRAIT_1660u DEC, 0x67C, 0, 10u, 40u, (2u, 2u, 5u, 83u)
#define PPUTLIMPL_UINT_TRAIT_1659u DEC, 0x67B, 0, 10u, 40u, (3u, 7u, 79u)
#define PPUTLIMPL_UINT_TRAIT_1658u DEC, 0x67A, 0, 10u, 40u, (2u, 829u)
#define PPUTLIMPL_UINT_TRAIT_1657u DEC, 0x679, 0, 10u, 40u, ()
#define PPUTLIMPL_UINT_TRAIT_1656u DEC, 0x678, 0, 10u, 40u, (2u, 2u, 2u, 3u, 3u, 23u)
#define PPUTLIMPL_UINT_TRAIT_1655u DEC, 0x677, 0, 10u, 40u, (5u, 331u)
#define PPUTLIMPL_UINT_TRAIT_1654u DEC, 0x676, 0, 10u, 40u, (2u, 827u)
#define PPUTLIMPL_UINT_TRAIT_1653u DEC, 0x675, 0, 10u, 40u, (3u, 19u, 29u)
#define PPUTLIMPL_UINT_TRAIT_1652u DEC, 0x674, 0, 10u, 40u, (2u, 2u, 7u, 59u)
#define PPUTLIMPL_UINT_TRAIT_1651u DEC, 0x673, 0, 10u, 40u, (13u, 127u)
#define PPUTLIMPL_UINT_TRAIT_1650u DEC, 0x672, 0, 10u, 40u, (2u, 3u, 5u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_1649u DEC, 0x671, 0, 10u, 40u, (17u, 97u)
#define PPUTLIMPL_UINT_TRAIT_1648u DEC, 0x670, 0, 10u, 40u, (2u, 2u, 2u, 2u, 103u)
#define PPUTLIMPL_UINT_TRAIT_1647u DEC, 0x66F, 0, 10u, 40u, (3u, 3u, 3u, 61u)
#define PPUTLIMPL_UINT_TRAIT_1646u DEC, 0x66E, 0, 10u, 40u, (2u, 823u)
#define PPUTLIMPL_UINT_TRAIT_1645u DEC, 0x66D, 0, 10u, 40u, (5u, 7u, 47u)
#define PPUTLIMPL_UINT_TRAIT_1644u DEC, 0x66C, 0, 10u, 40u, (2u, 2u, 3u, 137u)
#define PPUTLIMPL_UINT_TRAIT_1643u DEC, 0x66B, 0, 10u, 40u, (31u, 53u)
#define PPUTLIMPL_UINT_TRAIT_1642u DEC, 0x66A, 0, 10u, 40u, (2u, 821u)
#define PPUTLIMPL_UINT_TRAIT_1641u DEC, 0x669, 0, 10u, 40u, (3u, 547u)
#define PPUTLIMPL_UINT_TRAIT_1640u DEC, 0x668, 0, 10u, 40u, (2u, 2u, 2u, 5u, 41u)
#define PPUTLIMPL_UINT_TRAIT_1639u DEC, 0x667, 0, 10u, 40u, (11u, 149u)
#define PPUTLIMPL_UINT_TRAIT_1638u DEC, 0x666, 0, 10u, 40u, (2u, 3u, 3u, 7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1637u DEC, 0x665, 0, 10u, 40u, ()
#define PPUTLIMPL_UINT_TRAIT_1636u DEC, 0x664, 0, 10u, 40u, (2u, 2u, 409u)
#define PPUTLIMPL_UINT_TRAIT_1635u DEC, 0x663, 0, 10u, 40u, (3u, 5u, 109u)
#define PPUTLIMPL_UINT_TRAIT_1634u DEC, 0x662, 0, 10u, 40u, (2u, 19u, 43u)
#define PPUTLIMPL_UINT_TRAIT_1633u DEC, 0x661, 0, 10u, 40u, (23u, 71u)
#define PPUTLIMPL_UINT_TRAIT_1632u DEC, 0x660, 0, 10u, 40u, (2u, 2u, 2u, 2u, 2u, 3u, 17u)
#define PPUTLIMPL_UINT_TRAIT_1631u DEC, 0x65F, 0, 10u, 40u, (7u, 233u)
#define PPUTLIMPL_UINT_TRAIT_1630u DEC, 0x65E, 0, 10u, 40u, (2u, 5u, 163u)
#define PPUTLIMPL_UINT_TRAIT_1629u DEC, 0x65D, 0, 10u, 40u, (3u, 3u, 181u)
#define PPUTLIMPL_UINT_TRAIT_1628u DEC, 0x65C, 0, 10u, 40u, (2u, 2u, 11u, 37u)
#define PPUTLIMPL_UINT_TRAIT_1627u DEC, 0x65B, 0, 10u, 40u, ()
#define PPUTLIMPL_UINT_TRAIT_1626u DEC, 0x65A, 0, 10u, 40u, (2u, 3u, 271u)
#define PPUTLIMPL_UINT_TRAIT_1625u DEC, 0x659, 0, 10u, 40u, (5u, 5u, 5u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1624u DEC, 0x658, 0, 10u, 40u, (2u, 2u, 2u, 7u, 29u)
#define PPUTLIMPL_UINT_TRAIT_1623u DEC, 0x657, 0, 10u, 40u, (3u, 541u)
#define PPUTLIMPL_UINT_TRAIT_1622u DEC, 0x656, 0, 10u, 40u, (2u, 811u)
#define PPUTLIMPL_UINT_TRAIT_1621u DEC, 0x655, 0, 10u, 40u, ()
#define PPUTLIMPL_UINT_TRAIT_1620u DEC, 0x654, 0, 10u, 40u, (2u, 2u, 3u, 3u, 3u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_1619u DEC, 0x653, 0, 10u, 40u, ()
#define PPUTLIMPL_UINT_TRAIT_1618u DEC, 0x652, 0, 10u, 40u, (2u, 809u)
#define PPUTLIMPL_UINT_TRAIT_1617u DEC, 0x651, 0, 10u, 40u, (3u, 7u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_1616u DEC, 0x650, 0, 10u, 40u, (2u, 2u, 2u, 2u, 101u)
#define PPUTLIMPL_UINT_TRAIT_1615u DEC, 0x64F, 0, 10u, 40u, (5u, 17u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1614u DEC, 0x64E, 0, 10u, 40u, (2u, 3u, 269u)
#define PPUTLIMPL_UINT_TRAIT_1613u DEC, 0x64D, 0, 10u, 40u, ()
#define PPUTLIMPL_UINT_TRAIT_1612u DEC, 0x64C, 0, 10u, 40u, (2u, 2u, 13u, 31u)
#define PPUTLIMPL_UINT_TRAIT_1611u DEC, 0x64B, 0, 10u, 40u, (3u, 3u, 179u)
#define PPUTLIMPL_UINT_TRAIT_1610u DEC, 0x64A, 0, 10u, 40u, (2u, 5u, 7u, 23u)
#define PPUTLIMPL_UINT_TRAIT_1609u DEC, 0x649, 0, 10u, 40u, ()
#define PPUTLIMPL_UINT_TRAIT_1608u DEC, 0x648, 0, 10u, 40u, (2u, 2u, 2u, 3u, 67u)
#define PPUTLIMPL_UINT_TRAIT_1607u DEC, 0x647, 0, 10u, 40u, ()
#define PPUTLIMPL_UINT_TRAIT_1606u DEC, 0x646, 0, 10u, 40u, (2u, 11u, 73u)
#define PPUTLIMPL_UINT_TRAIT_1605u DEC, 0x645, 0, 10u, 40u, (3u, 5u, 107u)
#define PPUTLIMPL_UINT_TRAIT_1604u DEC, 0x644, 0, 10u, 40u, (2u, 2u, 401u)
#define PPUTLIMPL_UINT_TRAIT_1603u DEC, 0x643, 0, 10u, 40u, (7u, 229u)
#define PPUTLIMPL_UINT_TRAIT_1602u DEC, 0x642, 0, 10u, 40u, (2u, 3u, 3u, 89u)
#define PPUTLIMPL_UINT_TRAIT_1601u DEC, 0x641, 0, 10u, 40u, ()
#define PPUTLIMPL_UINT_TRAIT_1600u DEC, 0x640, 0, 10u, 40u, (2u, 2u, 2u, 2u, 2u, 2u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_1599u DEC, 0x63F, 0, 10u, 39u, (3u, 13u, 41u)
#define PPUTLIMPL_UINT_TRAIT_1598u DEC, 0x63E, 0, 10u, 39u, (2u, 17u, 47u)
#define PPUTLIMPL_UINT_TRAIT_1597u DEC, 0x63D, 0, 10u, 39u, ()
#define PPUTLIMPL_UINT_TRAIT_1596u DEC, 0x63C, 0, 10u, 39u, (2u, 2u, 3u, 7u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1595u DEC, 0x63B, 0, 10u, 39u, (5u, 11u, 29u)
#define PPUTLIMPL_UINT_TRAIT_1594u DEC, 0x63A, 0, 10u, 39u, (2u, 797u)
#define PPUTLIMPL_UINT_TRAIT_1593u DEC, 0x639, 0, 10u, 39u, (3u, 3u, 3u, 59u)
#define PPUTLIMPL_UINT_TRAIT_1592u DEC, 0x638, 0, 10u, 39u, (2u, 2u, 2u, 199u)
#define PPUTLIMPL_UINT_TRAIT_1591u DEC, 0x637, 0, 10u, 39u, (37u, 43u)
#define PPUTLIMPL_UINT_TRAIT_1590u DEC, 0x636, 0, 10u, 39u, (2u, 3u, 5u, 53u)
#define PPUTLIMPL_UINT_TRAIT_1589u DEC, 0x635, 0, 10u, 39u, (7u, 227u)
#define PPUTLIMPL_UINT_TRAIT_1588u DEC, 0x634, 0, 10u, 39u, (2u, 2u, 397u)
#define PPUTLIMPL_UINT_TRAIT_1587u DEC, 0x633, 0, 10u, 39u, (3u, 23u, 23u)
#define PPUTLIMPL_UINT_TRAIT_1586u DEC, 0x632, 0, 10u, 39u, (2u, 13u, 61u)
#define PPUTLIMPL_UINT_TRAIT_1585u DEC, 0x631, 0, 10u, 39u, (5u, 317u)
#define PPUTLIMPL_UINT_TRAIT_1584u DEC, 0x630, 0, 10u, 39u, (2u, 2u, 2u, 2u, 3u, 3u, 11u)
#define PPUTLIMPL_UINT_TRAIT_1583u DEC, 0x62F, 0, 10u, 39u, ()
#define PPUTLIMPL_UINT_TRAIT_1582u DEC, 0x62E, 0, 10u, 39u, (2u, 7u, 113u)
#define PPUTLIMPL_UINT_TRAIT_1581u DEC, 0x62D, 0, 10u, 39u, (3u, 17u, 31u)
#define PPUTLIMPL_UINT_TRAIT_1580u DEC, 0x62C, 0, 10u, 39u, (2u, 2u, 5u, 79u)
#define PPUTLIMPL_UINT_TRAIT_1579u DEC, 0x62B, 0, 10u, 39u, ()
#define PPUTLIMPL_UINT_TRAIT_1578u DEC, 0x62A, 0, 10u, 39u, (2u, 3u, 263u)
#define PPUTLIMPL_UINT_TRAIT_1577u DEC, 0x629, 0, 10u, 39u, (19u, 83u)
#define PPUTLIMPL_UINT_TRAIT_1576u DEC, 0x628, 0, 10u, 39u, (2u, 2u, 2u, 197u)
#define PPUTLIMPL_UINT_TRAIT_1575u DEC, 0x627, 0, 10u, 39u, (3u, 3u, 5u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_1574u DEC, 0x626, 0, 10u, 39u, (2u, 787u)
#define PPUTLIMPL_UINT_TRAIT_1573u DEC, 0x625, 0, 10u, 39u, (11u, 11u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1572u DEC, 0x624, 0, 10u, 39u, (2u, 2u, 3u, 131u)
#define PPUTLIMPL_UINT_TRAIT_1571u DEC, 0x623, 0, 10u, 39u, ()
#define PPUTLIMPL_UINT_TRAIT_1570u DEC, 0x622, 0, 10u, 39u, (2u, 5u, 157u)
#define PPUTLIMPL_UINT_TRAIT_1569u DEC, 0x621, 0, 10u, 39u, (3u, 523u)
#define PPUTLIMPL_UINT_TRAIT_1568u DEC, 0x620, 0, 10u, 39u, (2u, 2u, 2u, 2u, 2u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_1567u DEC, 0x61F, 0, 10u, 39u, ()
#define PPUTLIMPL_UINT_TRAIT_1566u DEC, 0x61E, 0, 10u, 39u, (2u, 3u, 3u, 3u, 29u)
#define PPUTLIMPL_UINT_TRAIT_1565u DEC, 0x61D, 0, 10u, 39u, (5u, 313u)
#define PPUTLIMPL_UINT_TRAIT_1564u DEC, 0x61C, 0, 10u, 39u, (2u, 2u, 17u, 23u)
#define PPUTLIMPL_UINT_TRAIT_1563u DEC, 0x61B, 0, 10u, 39u, (3u, 521u)
#define PPUTLIMPL_UINT_TRAIT_1562u DEC, 0x61A, 0, 10u, 39u, (2u, 11u, 71u)
#define PPUTLIMPL_UINT_TRAIT_1561u DEC, 0x619, 0, 10u, 39u, (7u, 223u)
#define PPUTLIMPL_UINT_TRAIT_1560u DEC, 0x618, 0, 10u, 39u, (2u, 2u, 2u, 3u, 5u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1559u DEC, 0x617, 0, 10u, 39u, ()
#define PPUTLIMPL_UINT_TRAIT_1558u DEC, 0x616, 0, 10u, 39u, (2u, 19u, 41u)
#define PPUTLIMPL_UINT_TRAIT_1557u DEC, 0x615, 0, 10u, 39u, (3u, 3u, 173u)
#define PPUTLIMPL_UINT_TRAIT_1556u DEC, 0x614, 0, 10u, 39u, (2u, 2u, 389u)
#define PPUTLIMPL_UINT_TRAIT_1555u DEC, 0x613, 0, 10u, 39u, (5u, 311u)
#define PPUTLIMPL_UINT_TRAIT_1554u DEC, 0x612, 0, 10u, 39u, (2u, 3u, 7u, 37u)
#define PPUTLIMPL_UINT_TRAIT_1553u DEC, 0x611, 0, 10u, 39u, ()
#define PPUTLIMPL_UINT_TRAIT_1552u DEC, 0x610, 0, 10u, 39u, (2u, 2u, 2u, 2u, 97u)
#define PPUTLIMPL_UINT_TRAIT_1551u DEC, 0x60F, 0, 10u, 39u, (3u, 11u, 47u)
#define PPUTLIMPL_UINT_TRAIT_1550u DEC, 0x60E, 0, 10u, 39u, (2u, 5u, 5u, 31u)
#define PPUTLIMPL_UINT_TRAIT_1549u DEC, 0x60D, 0, 10u, 39u, ()
#define PPUTLIMPL_UINT_TRAIT_1548u DEC, 0x60C, 0, 10u, 39u, (2u, 2u, 3u, 3u, 43u)
#define PPUTLIMPL_UINT_TRAIT_1547u DEC, 0x60B, 0, 10u, 39u, (7u, 13u, 17u)
#define PPUTLIMPL_UINT_TRAIT_1546u DEC, 0x60A, 0, 10u, 39u, (2u, 773u)
#define PPUTLIMPL_UINT_TRAIT_1545u DEC, 0x609, 0, 10u, 39u, (3u, 5u, 103u)
#define PPUTLIMPL_UINT_TRAIT_1544u DEC, 0x608, 0, 10u, 39u, (2u, 2u, 2u, 193u)
#define PPUTLIMPL_UINT_TRAIT_1543u DEC, 0x607, 0, 10u, 39u, ()
#define PPUTLIMPL_UINT_TRAIT_1542u DEC, 0x606, 0, 10u, 39u, (2u, 3u, 257u)
#define PPUTLIMPL_UINT_TRAIT_1541u DEC, 0x605, 0, 10u, 39u, (23u, 67u)
#define PPUTLIMPL_UINT_TRAIT_1540u DEC, 0x604, 0, 10u, 39u, (2u, 2u, 5u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_1539u DEC, 0x603, 0, 10u, 39u, (3u, 3u, 3u, 3u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1538u DEC, 0x602, 0, 10u, 39u, (2u, 769u)
#define PPUTLIMPL_UINT_TRAIT_1537u DEC, 0x601, 0, 10u, 39u, (29u, 53u)
#define PPUTLIMPL_UINT_TRAIT_1536u DEC, 0x600, 0, 10u, 39u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 3u)
#define PPUTLIMPL_UINT_TRAIT_1535u DEC, 0x5FF, 0, 10u, 39u, (5u, 307u)
#define PPUTLIMPL_UINT_TRAIT_1534u DEC, 0x5FE, 0, 10u, 39u, (2u, 13u, 59u)
#define PPUTLIMPL_UINT_TRAIT_1533u DEC, 0x5FD, 0, 10u, 39u, (3u, 7u, 73u)
#define PPUTLIMPL_UINT_TRAIT_1532u DEC, 0x5FC, 0, 10u, 39u, (2u, 2u, 383u)
#define PPUTLIMPL_UINT_TRAIT_1531u DEC, 0x5FB, 0, 10u, 39u, ()
#define PPUTLIMPL_UINT_TRAIT_1530u DEC, 0x5FA, 0, 10u, 39u, (2u, 3u, 3u, 5u, 17u)
#define PPUTLIMPL_UINT_TRAIT_1529u DEC, 0x5F9, 0, 10u, 39u, (11u, 139u)
#define PPUTLIMPL_UINT_TRAIT_1528u DEC, 0x5F8, 0, 10u, 39u, (2u, 2u, 2u, 191u)
#define PPUTLIMPL_UINT_TRAIT_1527u DEC, 0x5F7, 0, 10u, 39u, (3u, 509u)
#define PPUTLIMPL_UINT_TRAIT_1526u DEC, 0x5F6, 0, 10u, 39u, (2u, 7u, 109u)
#define PPUTLIMPL_UINT_TRAIT_1525u DEC, 0x5F5, 0, 10u, 39u, (5u, 5u, 61u)
#define PPUTLIMPL_UINT_TRAIT_1524u DEC, 0x5F4, 0, 10u, 39u, (2u, 2u, 3u, 127u)
#define PPUTLIMPL_UINT_TRAIT_1523u DEC, 0x5F3, 0, 10u, 39u, ()
#define PPUTLIMPL_UINT_TRAIT_1522u DEC, 0x5F2, 0, 10u, 39u, (2u, 761u)
#define PPUTLIMPL_UINT_TRAIT_1521u DEC, 0x5F1, 0, 10u, 39u, (3u, 3u, 13u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1520u DEC, 0x5F0, 0, 10u, 38u, (2u, 2u, 2u, 2u, 5u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1519u DEC, 0x5EF, 0, 10u, 38u, (7u, 7u, 31u)
#define PPUTLIMPL_UINT_TRAIT_1518u DEC, 0x5EE, 0, 10u, 38u, (2u, 3u, 11u, 23u)
#define PPUTLIMPL_UINT_TRAIT_1517u DEC, 0x5ED, 0, 10u, 38u, (37u, 41u)
#define PPUTLIMPL_UINT_TRAIT_1516u DEC, 0x5EC, 0, 10u, 38u, (2u, 2u, 379u)
#define PPUTLIMPL_UINT_TRAIT_1515u DEC, 0x5EB, 0, 10u, 38u, (3u, 5u, 101u)
#define PPUTLIMPL_UINT_TRAIT_1514u DEC, 0x5EA, 0, 10u, 38u, (2u, 757u)
#define PPUTLIMPL_UINT_TRAIT_1513u DEC, 0x5E9, 0, 10u, 38u, (17u, 89u)
#define PPUTLIMPL_UINT_TRAIT_1512u DEC, 0x5E8, 0, 10u, 38u, (2u, 2u, 2u, 3u, 3u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_1511u DEC, 0x5E7, 0, 10u, 38u, ()
#define PPUTLIMPL_UINT_TRAIT_1510u DEC, 0x5E6, 0, 10u, 38u, (2u, 5u, 151u)
#define PPUTLIMPL_UINT_TRAIT_1509u DEC, 0x5E5, 0, 10u, 38u, (3u, 503u)
#define PPUTLIMPL_UINT_TRAIT_1508u DEC, 0x5E4, 0, 10u, 38u, (2u, 2u, 13u, 29u)
#define PPUTLIMPL_UINT_TRAIT_1507u DEC, 0x5E3, 0, 10u, 38u, (11u, 137u)
#define PPUTLIMPL_UINT_TRAIT_1506u DEC, 0x5E2, 0, 10u, 38u, (2u, 3u, 251u)
#define PPUTLIMPL_UINT_TRAIT_1505u DEC, 0x5E1, 0, 10u, 38u, (5u, 7u, 43u)
#define PPUTLIMPL_UINT_TRAIT_1504u DEC, 0x5E0, 0, 10u, 38u, (2u, 2u, 2u, 2u, 2u, 47u)
#define PPUTLIMPL_UINT_TRAIT_1503u DEC, 0x5DF, 0, 10u, 38u, (3u, 3u, 167u)
#define PPUTLIMPL_UINT_TRAIT_1502u DEC, 0x5DE, 0, 10u, 38u, (2u, 751u)
#define PPUTLIMPL_UINT_TRAIT_1501u DEC, 0x5DD, 0, 10u, 38u, (19u, 79u)
#define PPUTLIMPL_UINT_TRAIT_1500u DEC, 0x5DC, 0, 10u, 38u, (2u, 2u, 3u, 5u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_1499u DEC, 0x5DB, 0, 10u, 38u, ()
#define PPUTLIMPL_UINT_TRAIT_1498u DEC, 0x5DA, 0, 10u, 38u, (2u, 7u, 107u)
#define PPUTLIMPL_UINT_TRAIT_1497u DEC, 0x5D9, 0, 10u, 38u, (3u, 499u)
#define PPUTLIMPL_UINT_TRAIT_1496u DEC, 0x5D8, 0, 10u, 38u, (2u, 2u, 2u, 11u, 17u)
#define PPUTLIMPL_UINT_TRAIT_1495u DEC, 0x5D7, 0, 10u, 38u, (5u, 13u, 23u)
#define PPUTLIMPL_UINT_TRAIT_1494u DEC, 0x5D6, 0, 10u, 38u, (2u, 3u, 3u, 83u)
#define PPUTLIMPL_UINT_TRAIT_1493u DEC, 0x5D5, 0, 10u, 38u, ()
#define PPUTLIMPL_UINT_TRAIT_1492u DEC, 0x5D4, 0, 10u, 38u, (2u, 2u, 373u)
#define PPUTLIMPL_UINT_TRAIT_1491u DEC, 0x5D3, 0, 10u, 38u, (3u, 7u, 71u)
#define PPUTLIMPL_UINT_TRAIT_1490u DEC, 0x5D2, 0, 10u, 38u, (2u, 5u, 149u)
#define PPUTLIMPL_UINT_TRAIT_1489u DEC, 0x5D1, 0, 10u, 38u, ()
#define PPUTLIMPL_UINT_TRAIT_1488u DEC, 0x5D0, 0, 10u, 38u, (2u, 2u, 2u, 2u, 3u, 31u)
#define PPUTLIMPL_UINT_TRAIT_1487u DEC, 0x5CF, 0, 10u, 38u, ()
#define PPUTLIMPL_UINT_TRAIT_1486u DEC, 0x5CE, 0, 10u, 38u, (2u, 743u)
#define PPUTLIMPL_UINT_TRAIT_1485u DEC, 0x5CD, 0, 10u, 38u, (3u, 3u, 3u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_1484u DEC, 0x5CC, 0, 10u, 38u, (2u, 2u, 7u, 53u)
#define PPUTLIMPL_UINT_TRAIT_1483u DEC, 0x5CB, 0, 10u, 38u, ()
#define PPUTLIMPL_UINT_TRAIT_1482u DEC, 0x5CA, 0, 10u, 38u, (2u, 3u, 13u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1481u DEC, 0x5C9, 0, 10u, 38u, ()
#define PPUTLIMPL_UINT_TRAIT_1480u DEC, 0x5C8, 0, 10u, 38u, (2u, 2u, 2u, 5u, 37u)
#define PPUTLIMPL_UINT_TRAIT_1479u DEC, 0x5C7, 0, 10u, 38u, (3u, 17u, 29u)
#define PPUTLIMPL_UINT_TRAIT_1478u DEC, 0x5C6, 0, 10u, 38u, (2u, 739u)
#define PPUTLIMPL_UINT_TRAIT_1477u DEC, 0x5C5, 0, 10u, 38u, (7u, 211u)
#define PPUTLIMPL_UINT_TRAIT_1476u DEC, 0x5C4, 0, 10u, 38u, (2u, 2u, 3u, 3u, 41u)
#define PPUTLIMPL_UINT_TRAIT_1475u DEC, 0x5C3, 0, 10u, 38u, (5u, 5u, 59u)
#define PPUTLIMPL_UINT_TRAIT_1474u DEC, 0x5C2, 0, 10u, 38u, (2u, 11u, 67u)
#define PPUTLIMPL_UINT_TRAIT_1473u DEC, 0x5C1, 0, 10u, 38u, (3u, 491u)
#define PPUTLIMPL_UINT_TRAIT_1472u DEC, 0x5C0, 0, 10u, 38u, (2u, 2u, 2u, 2u, 2u, 2u, 23u)
#define PPUTLIMPL_UINT_TRAIT_1471u DEC, 0x5BF, 0, 10u, 38u, ()
#define PPUTLIMPL_UINT_TRAIT_1470u DEC, 0x5BE, 0, 10u, 38u, (2u, 3u, 5u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_1469u DEC, 0x5BD, 0, 10u, 38u, (13u, 113u)
#define PPUTLIMPL_UINT_TRAIT_1468u DEC, 0x5BC, 0, 10u, 38u, (2u, 2u, 367u)
#define PPUTLIMPL_UINT_TRAIT_1467u DEC, 0x5BB, 0, 10u, 38u, (3u, 3u, 163u)
#define PPUTLIMPL_UINT_TRAIT_1466u DEC, 0x5BA, 0, 10u, 38u, (2u, 733u)
#define PPUTLIMPL_UINT_TRAIT_1465u DEC, 0x5B9, 0, 10u, 38u, (5u, 293u)
#define PPUTLIMPL_UINT_TRAIT_1464u DEC, 0x5B8, 0, 10u, 38u, (2u, 2u, 2u, 3u, 61u)
#define PPUTLIMPL_UINT_TRAIT_1463u DEC, 0x5B7, 0, 10u, 38u, (7u, 11u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1462u DEC, 0x5B6, 0, 10u, 38u, (2u, 17u, 43u)
#define PPUTLIMPL_UINT_TRAIT_1461u DEC, 0x5B5, 0, 10u, 38u, (3u, 487u)
#define PPUTLIMPL_UINT_TRAIT_1460u DEC, 0x5B4, 0, 10u, 38u, (2u, 2u, 5u, 73u)
#define PPUTLIMPL_UINT_TRAIT_1459u DEC, 0x5B3, 0, 10u, 38u, ()
#define PPUTLIMPL_UINT_TRAIT_1458u DEC, 0x5B2, 0, 10u, 38u, (2u, 3u, 3u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_1457u DEC, 0x5B1, 0, 10u, 38u, (31u, 47u)
#define PPUTLIMPL_UINT_TRAIT_1456u DEC, 0x5B0, 0, 10u, 38u, (2u, 2u, 2u, 2u, 7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1455u DEC, 0x5AF, 0, 10u, 38u, (3u, 5u, 97u)
#define PPUTLIMPL_UINT_TRAIT_1454u DEC, 0x5AE, 0, 10u, 38u, (2u, 727u)
#define PPUTLIMPL_UINT_TRAIT_1453u DEC, 0x5AD, 0, 10u, 38u, ()
#define PPUTLIMPL_UINT_TRAIT_1452u DEC, 0x5AC, 0, 10u, 38u, (2u, 2u, 3u, 11u, 11u)
#define PPUTLIMPL_UINT_TRAIT_1451u DEC, 0x5AB, 0, 10u, 38u, ()
#define PPUTLIMPL_UINT_TRAIT_1450u DEC, 0x5AA, 0, 10u, 38u, (2u, 5u, 5u, 29u)
#define PPUTLIMPL_UINT_TRAIT_1449u DEC, 0x5A9, 0, 10u, 38u, (3u, 3u, 7u, 23u)
#define PPUTLIMPL_UINT_TRAIT_1448u DEC, 0x5A8, 0, 10u, 38u, (2u, 2u, 2u, 181u)
#define PPUTLIMPL_UINT_TRAIT_1447u DEC, 0x5A7, 0, 10u, 38u, ()
#define PPUTLIMPL_UINT_TRAIT_1446u DEC, 0x5A6, 0, 10u, 38u, (2u, 3u, 241u)
#define PPUTLIMPL_UINT_TRAIT_1445u DEC, 0x5A5, 0, 10u, 38u, (5u, 17u, 17u)
#define PPUTLIMPL_UINT_TRAIT_1444u DEC, 0x5A4, 0, 10u, 38u, (2u, 2u, 19u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1443u DEC, 0x5A3, 0, 10u, 37u, (3u, 13u, 37u)
#define PPUTLIMPL_UINT_TRAIT_1442u DEC, 0x5A2, 0, 10u, 37u, (2u, 7u, 103u)
#define PPUTLIMPL_UINT_TRAIT_1441u DEC, 0x5A1, 0, 10u, 37u, (11u, 131u)
#define PPUTLIMPL_UINT_TRAIT_1440u DEC, 0x5A0, 0, 10u, 37u, (2u, 2u, 2u, 2u, 2u, 3u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_1439u DEC, 0x59F, 0, 10u, 37u, ()
#define PPUTLIMPL_UINT_TRAIT_1438u DEC, 0x59E, 0, 10u, 37u, (2u, 719u)
#define PPUTLIMPL_UINT_TRAIT_1437u DEC, 0x59D, 0, 10u, 37u, (3u, 479u)
#define PPUTLIMPL_UINT_TRAIT_1436u DEC, 0x59C, 0, 10u, 37u, (2u, 2u, 359u)
#define PPUTLIMPL_UINT_TRAIT_1435u DEC, 0x59B, 0, 10u, 37u, (5u, 7u, 41u)
#define PPUTLIMPL_UINT_TRAIT_1434u DEC, 0x59A, 0, 10u, 37u, (2u, 3u, 239u)
#define PPUTLIMPL_UINT_TRAIT_1433u DEC, 0x599, 0, 10u, 37u, ()
#define PPUTLIMPL_UINT_TRAIT_1432u DEC, 0x598, 0, 10u, 37u, (2u, 2u, 2u, 179u)
#define PPUTLIMPL_UINT_TRAIT_1431u DEC, 0x597, 0, 10u, 37u, (3u, 3u, 3u, 53u)
#define PPUTLIMPL_UINT_TRAIT_1430u DEC, 0x596, 0, 10u, 37u, (2u, 5u, 11u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1429u DEC, 0x595, 0, 10u, 37u, ()
#define PPUTLIMPL_UINT_TRAIT_1428u DEC, 0x594, 0, 10u, 37u, (2u, 2u, 3u, 7u, 17u)
#define PPUTLIMPL_UINT_TRAIT_1427u DEC, 0x593, 0, 10u, 37u, ()
#define PPUTLIMPL_UINT_TRAIT_1426u DEC, 0x592, 0, 10u, 37u, (2u, 23u, 31u)
#define PPUTLIMPL_UINT_TRAIT_1425u DEC, 0x591, 0, 10u, 37u, (3u, 5u, 5u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1424u DEC, 0x590, 0, 10u, 37u, (2u, 2u, 2u, 2u, 89u)
#define PPUTLIMPL_UINT_TRAIT_1423u DEC, 0x58F, 0, 10u, 37u, ()
#define PPUTLIMPL_UINT_TRAIT_1422u DEC, 0x58E, 0, 10u, 37u, (2u, 3u, 3u, 79u)
#define PPUTLIMPL_UINT_TRAIT_1421u DEC, 0x58D, 0, 10u, 37u, (7u, 7u, 29u)
#define PPUTLIMPL_UINT_TRAIT_1420u DEC, 0x58C, 0, 10u, 37u, (2u, 2u, 5u, 71u)
#define PPUTLIMPL_UINT_TRAIT_1419u DEC, 0x58B, 0, 10u, 37u, (3u, 11u, 43u)
#define PPUTLIMPL_UINT_TRAIT_1418u DEC, 0x58A, 0, 10u, 37u, (2u, 709u)
#define PPUTLIMPL_UINT_TRAIT_1417u DEC, 0x589, 0, 10u, 37u, (13u, 109u)
#define PPUTLIMPL_UINT_TRAIT_1416u DEC, 0x588, 0, 10u, 37u, (2u, 2u, 2u, 3u, 59u)
#define PPUTLIMPL_UINT_TRAIT_1415u DEC, 0x587, 0, 10u, 37u, (5u, 283u)
#define PPUTLIMPL_UINT_TRAIT_1414u DEC, 0x586, 0, 10u, 37u, (2u, 7u, 101u)
#define PPUTLIMPL_UINT_TRAIT_1413u DEC, 0x585, 0, 10u, 37u, (3u, 3u, 157u)
#define PPUTLIMPL_UINT_TRAIT_1412u DEC, 0x584, 0, 10u, 37u, (2u, 2u, 353u)
#define PPUTLIMPL_UINT_TRAIT_1411u DEC, 0x583, 0, 10u, 37u, (17u, 83u)
#define PPUTLIMPL_UINT_TRAIT_1410u DEC, 0x582, 0, 10u, 37u, (2u, 3u, 5u, 47u)
#define PPUTLIMPL_UINT_TRAIT_1409u DEC, 0x581, 0, 10u, 37u, ()
#define PPUTLIMPL_UINT_TRAIT_1408u DEC, 0x580, 0, 10u, 37u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 11u)
#define PPUTLIMPL_UINT_TRAIT_1407u DEC, 0x57F, 0, 10u, 37u, (3u, 7u, 67u)
#define PPUTLIMPL_UINT_TRAIT_1406u DEC, 0x57E, 0, 10u, 37u, (2u, 19u, 37u)
#define PPUTLIMPL_UINT_TRAIT_1405u DEC, 0x57D, 0, 10u, 37u, (5u, 281u)
#define PPUTLIMPL_UINT_TRAIT_1404u DEC, 0x57C, 0, 10u, 37u, (2u, 2u, 3u, 3u, 3u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1403u DEC, 0x57B, 0, 10u, 37u, (23u, 61u)
#define PPUTLIMPL_UINT_TRAIT_1402u DEC, 0x57A, 0, 10u, 37u, (2u, 701u)
#define PPUTLIMPL_UINT_TRAIT_1401u DEC, 0x579, 0, 10u, 37u, (3u, 467u)
#define PPUTLIMPL_UINT_TRAIT_1400u DEC, 0x578, 0, 10u, 37u, (2u, 2u, 2u, 5u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_1399u DEC, 0x577, 0, 10u, 37u, ()
#define PPUTLIMPL_UINT_TRAIT_1398u DEC, 0x576, 0, 10u, 37u, (2u, 3u, 233u)
#define PPUTLIMPL_UINT_TRAIT_1397u DEC, 0x575, 0, 10u, 37u, (11u, 127u)
#define PPUTLIMPL_UINT_TRAIT_1396u DEC, 0x574, 0, 10u, 37u, (2u, 2u, 349u)
#define PPUTLIMPL_UINT_TRAIT_1395u DEC, 0x573, 0, 10u, 37u, (3u, 3u, 5u, 31u)
#define PPUTLIMPL_UINT_TRAIT_1394u DEC, 0x572, 0, 10u, 37u, (2u, 17u, 41u)
#define PPUTLIMPL_UINT_TRAIT_1393u DEC, 0x571, 0, 10u, 37u, (7u, 199u)
#define PPUTLIMPL_UINT_TRAIT_1392u DEC, 0x570, 0, 10u, 37u, (2u, 2u, 2u, 2u, 3u, 29u)
#define PPUTLIMPL_UINT_TRAIT_1391u DEC, 0x56F, 0, 10u, 37u, (13u, 107u)
#define PPUTLIMPL_UINT_TRAIT_1390u DEC, 0x56E, 0, 10u, 37u, (2u, 5u, 139u)
#define PPUTLIMPL_UINT_TRAIT_1389u DEC, 0x56D, 0, 10u, 37u, (3u, 463u)
#define PPUTLIMPL_UINT_TRAIT_1388u DEC, 0x56C, 0, 10u, 37u, (2u, 2u, 347u)
#define PPUTLIMPL_UINT_TRAIT_1387u DEC, 0x56B, 0, 10u, 37u, (19u, 73u)
#define PPUTLIMPL_UINT_TRAIT_1386u DEC, 0x56A, 0, 10u, 37u, (2u, 3u, 3u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_1385u DEC, 0x569, 0, 10u, 37u, (5u, 277u)
#define PPUTLIMPL_UINT_TRAIT_1384u DEC, 0x568, 0, 10u, 37u, (2u, 2u, 2u, 173u)
#define PPUTLIMPL_UINT_TRAIT_1383u DEC, 0x567, 0, 10u, 37u, (3u, 461u)
#define PPUTLIMPL_UINT_TRAIT_1382u DEC, 0x566, 0, 10u, 37u, (2u, 691u)
#define PPUTLIMPL_UINT_TRAIT_1381u DEC, 0x565, 0, 10u, 37u, ()
#define PPUTLIMPL_UINT_TRAIT_1380u DEC, 0x564, 0, 10u, 37u, (2u, 2u, 3u, 5u, 23u)
#define PPUTLIMPL_UINT_TRAIT_1379u DEC, 0x563, 0, 10u, 37u, (7u, 197u)
#define PPUTLIMPL_UINT_TRAIT_1378u DEC, 0x562, 0, 10u, 37u, (2u, 13u, 53u)
#define PPUTLIMPL_UINT_TRAIT_1377u DEC, 0x561, 0, 10u, 37u, (3u, 3u, 3u, 3u, 17u)
#define PPUTLIMPL_UINT_TRAIT_1376u DEC, 0x560, 0, 10u, 37u, (2u, 2u, 2u, 2u, 2u, 43u)
#define PPUTLIMPL_UINT_TRAIT_1375u DEC, 0x55F, 0, 10u, 37u, (5u, 5u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_1374u DEC, 0x55E, 0, 10u, 37u, (2u, 3u, 229u)
#define PPUTLIMPL_UINT_TRAIT_1373u DEC, 0x55D, 0, 10u, 37u, ()
#define PPUTLIMPL_UINT_TRAIT_1372u DEC, 0x55C, 0, 10u, 37u, (2u, 2u, 7u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_1371u DEC, 0x55B, 0, 10u, 37u, (3u, 457u)
#define PPUTLIMPL_UINT_TRAIT_1370u DEC, 0x55A, 0, 10u, 37u, (2u, 5u, 137u)
#define PPUTLIMPL_UINT_TRAIT_1369u DEC, 0x559, 0, 10u, 37u, (37u, 37u)
#define PPUTLIMPL_UINT_TRAIT_1368u DEC, 0x558, 0, 10u, 36u, (2u, 2u, 2u, 3u, 3u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1367u DEC, 0x557, 0, 10u, 36u, ()
#define PPUTLIMPL_UINT_TRAIT_1366u DEC, 0x556, 0, 10u, 36u, (2u, 683u)
#define PPUTLIMPL_UINT_TRAIT_1365u DEC, 0x555, 0, 10u, 36u, (3u, 5u, 7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1364u DEC, 0x554, 0, 10u, 36u, (2u, 2u, 11u, 31u)
#define PPUTLIMPL_UINT_TRAIT_1363u DEC, 0x553, 0, 10u, 36u, (29u, 47u)
#define PPUTLIMPL_UINT_TRAIT_1362u DEC, 0x552, 0, 10u, 36u, (2u, 3u, 227u)
#define PPUTLIMPL_UINT_TRAIT_1361u DEC, 0x551, 0, 10u, 36u, ()
#define PPUTLIMPL_UINT_TRAIT_1360u DEC, 0x550, 0, 10u, 36u, (2u, 2u, 2u, 2u, 5u, 17u)
#define PPUTLIMPL_UINT_TRAIT_1359u DEC, 0x54F, 0, 10u, 36u, (3u, 3u, 151u)
#define PPUTLIMPL_UINT_TRAIT_1358u DEC, 0x54E, 0, 10u, 36u, (2u, 7u, 97u)
#define PPUTLIMPL_UINT_TRAIT_1357u DEC, 0x54D, 0, 10u, 36u, (23u, 59u)
#define PPUTLIMPL_UINT_TRAIT_1356u DEC, 0x54C, 0, 10u, 36u, (2u, 2u, 3u, 113u)
#define PPUTLIMPL_UINT_TRAIT_1355u DEC, 0x54B, 0, 10u, 36u, (5u, 271u)
#define PPUTLIMPL_UINT_TRAIT_1354u DEC, 0x54A, 0, 10u, 36u, (2u, 677u)
#define PPUTLIMPL_UINT_TRAIT_1353u DEC, 0x549, 0, 10u, 36u, (3u, 11u, 41u)
#define PPUTLIMPL_UINT_TRAIT_1352u DEC, 0x548, 0, 10u, 36u, (2u, 2u, 2u, 13u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1351u DEC, 0x547, 0, 10u, 36u, (7u, 193u)
#define PPUTLIMPL_UINT_TRAIT_1350u DEC, 0x546, 0, 10u, 36u, (2u, 3u, 3u, 3u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_1349u DEC, 0x545, 0, 10u, 36u, (19u, 71u)
#define PPUTLIMPL_UINT_TRAIT_1348u DEC, 0x544, 0, 10u, 36u, (2u, 2u, 337u)
#define PPUTLIMPL_UINT_TRAIT_1347u DEC, 0x543, 0, 10u, 36u, (3u, 449u)
#define PPUTLIMPL_UINT_TRAIT_1346u DEC, 0x542, 0, 10u, 36u, (2u, 673u)
#define PPUTLIMPL_UINT_TRAIT_1345u DEC, 0x541, 0, 10u, 36u, (5u, 269u)
#define PPUTLIMPL_UINT_TRAIT_1344u DEC, 0x540, 0, 10u, 36u, (2u, 2u, 2u, 2u, 2u, 2u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_1343u DEC, 0x53F, 0, 10u, 36u, (17u, 79u)
#define PPUTLIMPL_UINT_TRAIT_1342u DEC, 0x53E, 0, 10u, 36u, (2u, 11u, 61u)
#define PPUTLIMPL_UINT_TRAIT_1341u DEC, 0x53D, 0, 10u, 36u, (3u, 3u, 149u)
#define PPUTLIMPL_UINT_TRAIT_1340u DEC, 0x53C, 0, 10u, 36u, (2u, 2u, 5u, 67u)
#define PPUTLIMPL_UINT_TRAIT_1339u DEC, 0x53B, 0, 10u, 36u, (13u, 103u)
#define PPUTLIMPL_UINT_TRAIT_1338u DEC, 0x53A, 0, 10u, 36u, (2u, 3u, 223u)
#define PPUTLIMPL_UINT_TRAIT_1337u DEC, 0x539, 0, 10u, 36u, (7u, 191u)
#define PPUTLIMPL_UINT_TRAIT_1336u DEC, 0x538, 0, 10u, 36u, (2u, 2u, 2u, 167u)
#define PPUTLIMPL_UINT_TRAIT_1335u DEC, 0x537, 0, 10u, 36u, (3u, 5u, 89u)
#define PPUTLIMPL_UINT_TRAIT_1334u DEC, 0x536, 0, 10u, 36u, (2u, 23u, 29u)
#define PPUTLIMPL_UINT_TRAIT_1333u DEC, 0x535, 0, 10u, 36u, (31u, 43u)
#define PPUTLIMPL_UINT_TRAIT_1332u DEC, 0x534, 0, 10u, 36u, (2u, 2u, 3u, 3u, 37u)
#define PPUTLIMPL_UINT_TRAIT_1331u DEC, 0x533, 0, 10u, 36u, (11u, 11u, 11u)
#define PPUTLIMPL_UINT_TRAIT_1330u DEC, 0x532, 0, 10u, 36u, (2u, 5u, 7u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1329u DEC, 0x531, 0, 10u, 36u, (3u, 443u)
#define PPUTLIMPL_UINT_TRAIT_1328u DEC, 0x530, 0, 10u, 36u, (2u, 2u, 2u, 2u, 83u)
#define PPUTLIMPL_UINT_TRAIT_1327u DEC, 0x52F, 0, 10u, 36u, ()
#define PPUTLIMPL_UINT_TRAIT_1326u DEC, 0x52E, 0, 10u, 36u, (2u, 3u, 13u, 17u)
#define PPUTLIMPL_UINT_TRAIT_1325u DEC, 0x52D, 0, 10u, 36u, (5u, 5u, 53u)
#define PPUTLIMPL_UINT_TRAIT_1324u DEC, 0x52C, 0, 10u, 36u, (2u, 2u, 331u)
#define PPUTLIMPL_UINT_TRAIT_1323u DEC, 0x52B, 0, 10u, 36u, (3u, 3u, 3u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_1322u DEC, 0x52A, 0, 10u, 36u, (2u, 661u)
#define PPUTLIMPL_UINT_TRAIT_1321u DEC, 0x529, 0, 10u, 36u, ()
#define PPUTLIMPL_UINT_TRAIT_1320u DEC, 0x528, 0, 10u, 36u, (2u, 2u, 2u, 3u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_1319u DEC, 0x527, 0, 10u, 36u, ()
#define PPUTLIMPL_UINT_TRAIT_1318u DEC, 0x526, 0, 10u, 36u, (2u, 659u)
#define PPUTLIMPL_UINT_TRAIT_1317u DEC, 0x525, 0, 10u, 36u, (3u, 439u)
#define PPUTLIMPL_UINT_TRAIT_1316u DEC, 0x524, 0, 10u, 36u, (2u, 2u, 7u, 47u)
#define PPUTLIMPL_UINT_TRAIT_1315u DEC, 0x523, 0, 10u, 36u, (5u, 263u)
#define PPUTLIMPL_UINT_TRAIT_1314u DEC, 0x522, 0, 10u, 36u, (2u, 3u, 3u, 73u)
#define PPUTLIMPL_UINT_TRAIT_1313u DEC, 0x521, 0, 10u, 36u, (13u, 101u)
#define PPUTLIMPL_UINT_TRAIT_1312u DEC, 0x520, 0, 10u, 36u, (2u, 2u, 2u, 2u, 2u, 41u)
#define PPUTLIMPL_UINT_TRAIT_1311u DEC, 0x51F, 0, 10u, 36u, (3u, 19u, 23u)
#define PPUTLIMPL_UINT_TRAIT_1310u DEC, 0x51E, 0, 10u, 36u, (2u, 5u, 131u)
#define PPUTLIMPL_UINT_TRAIT_1309u DEC, 0x51D, 0, 10u, 36u, (7u, 11u, 17u)
#define PPUTLIMPL_UINT_TRAIT_1308u DEC, 0x51C, 0, 10u, 36u, (2u, 2u, 3u, 109u)
#define PPUTLIMPL_UINT_TRAIT_1307u DEC, 0x51B, 0, 10u, 36u, ()
#define PPUTLIMPL_UINT_TRAIT_1306u DEC, 0x51A, 0, 10u, 36u, (2u, 653u)
#define PPUTLIMPL_UINT_TRAIT_1305u DEC, 0x519, 0, 10u, 36u, (3u, 3u, 5u, 29u)
#define PPUTLIMPL_UINT_TRAIT_1304u DEC, 0x518, 0, 10u, 36u, (2u, 2u, 2u, 163u)
#define PPUTLIMPL_UINT_TRAIT_1303u DEC, 0x517, 0, 10u, 36u, ()
#define PPUTLIMPL_UINT_TRAIT_1302u DEC, 0x516, 0, 10u, 36u, (2u, 3u, 7u, 31u)
#define PPUTLIMPL_UINT_TRAIT_1301u DEC, 0x515, 0, 10u, 36u, ()
#define PPUTLIMPL_UINT_TRAIT_1300u DEC, 0x514, 0, 10u, 36u, (2u, 2u, 5u, 5u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1299u DEC, 0x513, 0, 10u, 36u, (3u, 433u)
#define PPUTLIMPL_UINT_TRAIT_1298u DEC, 0x512, 0, 10u, 36u, (2u, 11u, 59u)
#define PPUTLIMPL_UINT_TRAIT_1297u DEC, 0x511, 0, 10u, 36u, ()
#define PPUTLIMPL_UINT_TRAIT_1296u DEC, 0x510, 0, 10u, 36u, (2u, 2u, 2u, 2u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_1295u DEC, 0x50F, 0, 10u, 35u, (5u, 7u, 37u)
#define PPUTLIMPL_UINT_TRAIT_1294u DEC, 0x50E, 0, 10u, 35u, (2u, 647u)
#define PPUTLIMPL_UINT_TRAIT_1293u DEC, 0x50D, 0, 10u, 35u, (3u, 431u)
#define PPUTLIMPL_UINT_TRAIT_1292u DEC, 0x50C, 0, 10u, 35u, (2u, 2u, 17u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1291u DEC, 0x50B, 0, 10u, 35u, ()
#define PPUTLIMPL_UINT_TRAIT_1290u DEC, 0x50A, 0, 10u, 35u, (2u, 3u, 5u, 43u)
#define PPUTLIMPL_UINT_TRAIT_1289u DEC, 0x509, 0, 10u, 35u, ()
#define PPUTLIMPL_UINT_TRAIT_1288u DEC, 0x508, 0, 10u, 35u, (2u, 2u, 2u, 7u, 23u)
#define PPUTLIMPL_UINT_TRAIT_1287u DEC, 0x507, 0, 10u, 35u, (3u, 3u, 11u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1286u DEC, 0x506, 0, 10u, 35u, (2u, 643u)
#define PPUTLIMPL_UINT_TRAIT_1285u DEC, 0x505, 0, 10u, 35u, (5u, 257u)
#define PPUTLIMPL_UINT_TRAIT_1284u DEC, 0x504, 0, 10u, 35u, (2u, 2u, 3u, 107u)
#define PPUTLIMPL_UINT_TRAIT_1283u DEC, 0x503, 0, 10u, 35u, ()
#define PPUTLIMPL_UINT_TRAIT_1282u DEC, 0x502, 0, 10u, 35u, (2u, 641u)
#define PPUTLIMPL_UINT_TRAIT_1281u DEC, 0x501, 0, 10u, 35u, (3u, 7u, 61u)
#define PPUTLIMPL_UINT_TRAIT_1280u DEC, 0x500, 0, 10u, 35u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 5u)
#define PPUTLIMPL_UINT_TRAIT_1279u DEC, 0x4FF, 0, 10u, 35u, ()
#define PPUTLIMPL_UINT_TRAIT_1278u DEC, 0x4FE, 0, 10u, 35u, (2u, 3u, 3u, 71u)
#define PPUTLIMPL_UINT_TRAIT_1277u DEC, 0x4FD, 0, 10u, 35u, ()
#define PPUTLIMPL_UINT_TRAIT_1276u DEC, 0x4FC, 0, 10u, 35u, (2u, 2u, 11u, 29u)
#define PPUTLIMPL_UINT_TRAIT_1275u DEC, 0x4FB, 0, 10u, 35u, (3u, 5u, 5u, 17u)
#define PPUTLIMPL_UINT_TRAIT_1274u DEC, 0x4FA, 0, 10u, 35u, (2u, 7u, 7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1273u DEC, 0x4F9, 0, 10u, 35u, (19u, 67u)
#define PPUTLIMPL_UINT_TRAIT_1272u DEC, 0x4F8, 0, 10u, 35u, (2u, 2u, 2u, 3u, 53u)
#define PPUTLIMPL_UINT_TRAIT_1271u DEC, 0x4F7, 0, 10u, 35u, (31u, 41u)
#define PPUTLIMPL_UINT_TRAIT_1270u DEC, 0x4F6, 0, 10u, 35u, (2u, 5u, 127u)
#define PPUTLIMPL_UINT_TRAIT_1269u DEC, 0x4F5, 0, 10u, 35u, (3u, 3u, 3u, 47u)
#define PPUTLIMPL_UINT_TRAIT_1268u DEC, 0x4F4, 0, 10u, 35u, (2u, 2u, 317u)
#define PPUTLIMPL_UINT_TRAIT_1267u DEC, 0x4F3, 0, 10u, 35u, (7u, 181u)
#define PPUTLIMPL_UINT_TRAIT_1266u DEC, 0x4F2, 0, 10u, 35u, (2u, 3u, 211u)
#define PPUTLIMPL_UINT_TRAIT_1265u DEC, 0x4F1, 0, 10u, 35u, (5u, 11u, 23u)
#define PPUTLIMPL_UINT_TRAIT_1264u DEC, 0x4F0, 0, 10u, 35u, (2u, 2u, 2u, 2u, 79u)
#define PPUTLIMPL_UINT_TRAIT_1263u DEC, 0x4EF, 0, 10u, 35u, (3u, 421u)
#define PPUTLIMPL_UINT_TRAIT_1262u DEC, 0x4EE, 0, 10u, 35u, (2u, 631u)
#define PPUTLIMPL_UINT_TRAIT_1261u DEC, 0x4ED, 0, 10u, 35u, (13u, 97u)
#define PPUTLIMPL_UINT_TRAIT_1260u DEC, 0x4EC, 0, 10u, 35u, (2u, 2u, 3u, 3u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_1259u DEC, 0x4EB, 0, 10u, 35u, ()
#define PPUTLIMPL_UINT_TRAIT_1258u DEC, 0x4EA, 0, 10u, 35u, (2u, 17u, 37u)
#define PPUTLIMPL_UINT_TRAIT_1257u DEC, 0x4E9, 0, 10u, 35u, (3u, 419u)
#define PPUTLIMPL_UINT_TRAIT_1256u DEC, 0x4E8, 0, 10u, 35u, (2u, 2u, 2u, 157u)
#define PPUTLIMPL_UINT_TRAIT_1255u DEC, 0x4E7, 0, 10u, 35u, (5u, 251u)
#define PPUTLIMPL_UINT_TRAIT_1254u DEC, 0x4E6, 0, 10u, 35u, (2u, 3u, 11u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1253u DEC, 0x4E5, 0, 10u, 35u, (7u, 179u)
#define PPUTLIMPL_UINT_TRAIT_1252u DEC, 0x4E4, 0, 10u, 35u, (2u, 2u, 313u)
#define PPUTLIMPL_UINT_TRAIT_1251u DEC, 0x4E3, 0, 10u, 35u, (3u, 3u, 139u)
#define PPUTLIMPL_UINT_TRAIT_1250u DEC, 0x4E2, 0, 10u, 35u, (2u, 5u, 5u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_1249u DEC, 0x4E1, 0, 10u, 35u, ()
#define PPUTLIMPL_UINT_TRAIT_1248u DEC, 0x4E0, 0, 10u, 35u, (2u, 2u, 2u, 2u, 2u, 3u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1247u DEC, 0x4DF, 0, 10u, 35u, (29u, 43u)
#define PPUTLIMPL_UINT_TRAIT_1246u DEC, 0x4DE, 0, 10u, 35u, (2u, 7u, 89u)
#define PPUTLIMPL_UINT_TRAIT_1245u DEC, 0x4DD, 0, 10u, 35u, (3u, 5u, 83u)
#define PPUTLIMPL_UINT_TRAIT_1244u DEC, 0x4DC, 0, 10u, 35u, (2u, 2u, 311u)
#define PPUTLIMPL_UINT_TRAIT_1243u DEC, 0x4DB, 0, 10u, 35u, (11u, 113u)
#define PPUTLIMPL_UINT_TRAIT_1242u DEC, 0x4DA, 0, 10u, 35u, (2u, 3u, 3u, 3u, 23u)
#define PPUTLIMPL_UINT_TRAIT_1241u DEC, 0x4D9, 0, 10u, 35u, (17u, 73u)
#define PPUTLIMPL_UINT_TRAIT_1240u DEC, 0x4D8, 0, 10u, 35u, (2u, 2u, 2u, 5u, 31u)
#define PPUTLIMPL_UINT_TRAIT_1239u DEC, 0x4D7, 0, 10u, 35u, (3u, 7u, 59u)
#define PPUTLIMPL_UINT_TRAIT_1238u DEC, 0x4D6, 0, 10u, 35u, (2u, 619u)
#define PPUTLIMPL_UINT_TRAIT_1237u DEC, 0x4D5, 0, 10u, 35u, ()
#define PPUTLIMPL_UINT_TRAIT_1236u DEC, 0x4D4, 0, 10u, 35u, (2u, 2u, 3u, 103u)
#define PPUTLIMPL_UINT_TRAIT_1235u DEC, 0x4D3, 0, 10u, 35u, (5u, 13u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1234u DEC, 0x4D2, 0, 10u, 35u, (2u, 617u)
#define PPUTLIMPL_UINT_TRAIT_1233u DEC, 0x4D1, 0, 10u, 35u, (3u, 3u, 137u)
#define PPUTLIMPL_UINT_TRAIT_1232u DEC, 0x4D0, 0, 10u, 35u, (2u, 2u, 2u, 2u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_1231u DEC, 0x4CF, 0, 10u, 35u, ()
#define PPUTLIMPL_UINT_TRAIT_1230u DEC, 0x4CE, 0, 10u, 35u, (2u, 3u, 5u, 41u)
#define PPUTLIMPL_UINT_TRAIT_1229u DEC, 0x4CD, 0, 10u, 35u, ()
#define PPUTLIMPL_UINT_TRAIT_1228u DEC, 0x4CC, 0, 10u, 35u, (2u, 2u, 307u)
#define PPUTLIMPL_UINT_TRAIT_1227u DEC, 0x4CB, 0, 10u, 35u, (3u, 409u)
#define PPUTLIMPL_UINT_TRAIT_1226u DEC, 0x4CA, 0, 10u, 35u, (2u, 613u)
#define PPUTLIMPL_UINT_TRAIT_1225u DEC, 0x4C9, 0, 10u, 35u, (5u, 5u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_1224u DEC, 0x4C8, 0, 10u, 34u, (2u, 2u, 2u, 3u, 3u, 17u)
#define PPUTLIMPL_UINT_TRAIT_1223u DEC, 0x4C7, 0, 10u, 34u, ()
#define PPUTLIMPL_UINT_TRAIT_1222u DEC, 0x4C6, 0, 10u, 34u, (2u, 13u, 47u)
#define PPUTLIMPL_UINT_TRAIT_1221u DEC, 0x4C5, 0, 10u, 34u, (3u, 11u, 37u)
#define PPUTLIMPL_UINT_TRAIT_1220u DEC, 0x4C4, 0, 10u, 34u, (2u, 2u, 5u, 61u)
#define PPUTLIMPL_UINT_TRAIT_1219u DEC, 0x4C3, 0, 10u, 34u, (23u, 53u)
#define PPUTLIMPL_UINT_TRAIT_1218u DEC, 0x4C2, 0, 10u, 34u, (2u, 3u, 7u, 29u)
#define PPUTLIMPL_UINT_TRAIT_1217u DEC, 0x4C1, 0, 10u, 34u, ()
#define PPUTLIMPL_UINT_TRAIT_1216u DEC, 0x4C0, 0, 10u, 34u, (2u, 2u, 2u, 2u, 2u, 2u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1215u DEC, 0x4BF, 0, 10u, 34u, (3u, 3u, 3u, 3u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_1214u DEC, 0x4BE, 0, 10u, 34u, (2u, 607u)
#define PPUTLIMPL_UINT_TRAIT_1213u DEC, 0x4BD, 0, 10u, 34u, ()
#define PPUTLIMPL_UINT_TRAIT_1212u DEC, 0x4BC, 0, 10u, 34u, (2u, 2u, 3u, 101u)
#define PPUTLIMPL_UINT_TRAIT_1211u DEC, 0x4BB, 0, 10u, 34u, (7u, 173u)
#define PPUTLIMPL_UINT_TRAIT_1210u DEC, 0x4BA, 0, 10u, 34u, (2u, 5u, 11u, 11u)
#define PPUTLIMPL_UINT_TRAIT_1209u DEC, 0x4B9, 0, 10u, 34u, (3u, 13u, 31u)
#define PPUTLIMPL_UINT_TRAIT_1208u DEC, 0x4B8, 0, 10u, 34u, (2u, 2u, 2u, 151u)
#define PPUTLIMPL_UINT_TRAIT_1207u DEC, 0x4B7, 0, 10u, 34u, (17u, 71u)
#define PPUTLIMPL_UINT_TRAIT_1206u DEC, 0x4B6, 0, 10u, 34u, (2u, 3u, 3u, 67u)
#define PPUTLIMPL_UINT_TRAIT_1205u DEC, 0x4B5, 0, 10u, 34u, (5u, 241u)
#define PPUTLIMPL_UINT_TRAIT_1204u DEC, 0x4B4, 0, 10u, 34u, (2u, 2u, 7u, 43u)
#define PPUTLIMPL_UINT_TRAIT_1203u DEC, 0x4B3, 0, 10u, 34u, (3u, 401u)
#define PPUTLIMPL_UINT_TRAIT_1202u DEC, 0x4B2, 0, 10u, 34u, (2u, 601u)
#define PPUTLIMPL_UINT_TRAIT_1201u DEC, 0x4B1, 0, 10u, 34u, ()
#define PPUTLIMPL_UINT_TRAIT_1200u DEC, 0x4B0, 0, 10u, 34u, (2u, 2u, 2u, 2u, 3u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_1199u DEC, 0x4AF, 0, 10u, 34u, (11u, 109u)
#define PPUTLIMPL_UINT_TRAIT_1198u DEC, 0x4AE, 0, 10u, 34u, (2u, 599u)
#define PPUTLIMPL_UINT_TRAIT_1197u DEC, 0x4AD, 0, 10u, 34u, (3u, 3u, 7u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1196u DEC, 0x4AC, 0, 10u, 34u, (2u, 2u, 13u, 23u)
#define PPUTLIMPL_UINT_TRAIT_1195u DEC, 0x4AB, 0, 10u, 34u, (5u, 239u)
#define PPUTLIMPL_UINT_TRAIT_1194u DEC, 0x4AA, 0, 10u, 34u, (2u, 3u, 199u)
#define PPUTLIMPL_UINT_TRAIT_1193u DEC, 0x4A9, 0, 10u, 34u, ()
#define PPUTLIMPL_UINT_TRAIT_1192u DEC, 0x4A8, 0, 10u, 34u, (2u, 2u, 2u, 149u)
#define PPUTLIMPL_UINT_TRAIT_1191u DEC, 0x4A7, 0, 10u, 34u, (3u, 397u)
#define PPUTLIMPL_UINT_TRAIT_1190u DEC, 0x4A6, 0, 10u, 34u, (2u, 5u, 7u, 17u)
#define PPUTLIMPL_UINT_TRAIT_1189u DEC, 0x4A5, 0, 10u, 34u, (29u, 41u)
#define PPUTLIMPL_UINT_TRAIT_1188u DEC, 0x4A4, 0, 10u, 34u, (2u, 2u, 3u, 3u, 3u, 11u)
#define PPUTLIMPL_UINT_TRAIT_1187u DEC, 0x4A3, 0, 10u, 34u, ()
#define PPUTLIMPL_UINT_TRAIT_1186u DEC, 0x4A2, 0, 10u, 34u, (2u, 593u)
#define PPUTLIMPL_UINT_TRAIT_1185u DEC, 0x4A1, 0, 10u, 34u, (3u, 5u, 79u)
#define PPUTLIMPL_UINT_TRAIT_1184u DEC, 0x4A0, 0, 10u, 34u, (2u, 2u, 2u, 2u, 2u, 37u)
#define PPUTLIMPL_UINT_TRAIT_1183u DEC, 0x49F, 0, 10u, 34u, (7u, 13u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1182u DEC, 0x49E, 0, 10u, 34u, (2u, 3u, 197u)
#define PPUTLIMPL_UINT_TRAIT_1181u DEC, 0x49D, 0, 10u, 34u, ()
#define PPUTLIMPL_UINT_TRAIT_1180u DEC, 0x49C, 0, 10u, 34u, (2u, 2u, 5u, 59u)
#define PPUTLIMPL_UINT_TRAIT_1179u DEC, 0x49B, 0, 10u, 34u, (3u, 3u, 131u)
#define PPUTLIMPL_UINT_TRAIT_1178u DEC, 0x49A, 0, 10u, 34u, (2u, 19u, 31u)
#define PPUTLIMPL_UINT_TRAIT_1177u DEC, 0x499, 0, 10u, 34u, (11u, 107u)
#define PPUTLIMPL_UINT_TRAIT_1176u DEC, 0x498, 0, 10u, 34u, (2u, 2u, 2u, 3u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_1175u DEC, 0x497, 0, 10u, 34u, (5u, 5u, 47u)
#define PPUTLIMPL_UINT_TRAIT_1174u DEC, 0x496, 0, 10u, 34u, (2u, 587u)
#define PPUTLIMPL_UINT_TRAIT_1173u DEC, 0x495, 0, 10u, 34u, (3u, 17u, 23u)
#define PPUTLIMPL_UINT_TRAIT_1172u DEC, 0x494, 0, 10u, 34u, (2u, 2u, 293u)
#define PPUTLIMPL_UINT_TRAIT_1171u DEC, 0x493, 0, 10u, 34u, ()
#define PPUTLIMPL_UINT_TRAIT_1170u DEC, 0x492, 0, 10u, 34u, (2u, 3u, 3u, 5u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1169u DEC, 0x491, 0, 10u, 34u, (7u, 167u)
#define PPUTLIMPL_UINT_TRAIT_1168u DEC, 0x490, 0, 10u, 34u, (2u, 2u, 2u, 2u, 73u)
#define PPUTLIMPL_UINT_TRAIT_1167u DEC, 0x48F, 0, 10u, 34u, (3u, 389u)
#define PPUTLIMPL_UINT_TRAIT_1166u DEC, 0x48E, 0, 10u, 34u, (2u, 11u, 53u)
#define PPUTLIMPL_UINT_TRAIT_1165u DEC, 0x48D, 0, 10u, 34u, (5u, 233u)
#define PPUTLIMPL_UINT_TRAIT_1164u DEC, 0x48C, 0, 10u, 34u, (2u, 2u, 3u, 97u)
#define PPUTLIMPL_UINT_TRAIT_1163u DEC, 0x48B, 0, 10u, 34u, ()
#define PPUTLIMPL_UINT_TRAIT_1162u DEC, 0x48A, 0, 10u, 34u, (2u, 7u, 83u)
#define PPUTLIMPL_UINT_TRAIT_1161u DEC, 0x489, 0, 10u, 34u, (3u, 3u, 3u, 43u)
#define PPUTLIMPL_UINT_TRAIT_1160u DEC, 0x488, 0, 10u, 34u, (2u, 2u, 2u, 5u, 29u)
#define PPUTLIMPL_UINT_TRAIT_1159u DEC, 0x487, 0, 10u, 34u, (19u, 61u)
#define PPUTLIMPL_UINT_TRAIT_1158u DEC, 0x486, 0, 10u, 34u, (2u, 3u, 193u)
#define PPUTLIMPL_UINT_TRAIT_1157u DEC, 0x485, 0, 10u, 34u, (13u, 89u)
#define PPUTLIMPL_UINT_TRAIT_1156u DEC, 0x484, 0, 10u, 34u, (2u, 2u, 17u, 17u)
#define PPUTLIMPL_UINT_TRAIT_1155u DEC, 0x483, 0, 10u, 33u, (3u, 5u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_1154u DEC, 0x482, 0, 10u, 33u, (2u, 577u)
#define PPUTLIMPL_UINT_TRAIT_1153u DEC, 0x481, 0, 10u, 33u, ()
#define PPUTLIMPL_UINT_TRAIT_1152u DEC, 0x480, 0, 10u, 33u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_1151u DEC, 0x47F, 0, 10u, 33u, ()
#define PPUTLIMPL_UINT_TRAIT_1150u DEC, 0x47E, 0, 10u, 33u, (2u, 5u, 5u, 23u)
#define PPUTLIMPL_UINT_TRAIT_1149u DEC, 0x47D, 0, 10u, 33u, (3u, 383u)
#define PPUTLIMPL_UINT_TRAIT_1148u DEC, 0x47C, 0, 10u, 33u, (2u, 2u, 7u, 41u)
#define PPUTLIMPL_UINT_TRAIT_1147u DEC, 0x47B, 0, 10u, 33u, (31u, 37u)
#define PPUTLIMPL_UINT_TRAIT_1146u DEC, 0x47A, 0, 10u, 33u, (2u, 3u, 191u)
#define PPUTLIMPL_UINT_TRAIT_1145u DEC, 0x479, 0, 10u, 33u, (5u, 229u)
#define PPUTLIMPL_UINT_TRAIT_1144u DEC, 0x478, 0, 10u, 33u, (2u, 2u, 2u, 11u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1143u DEC, 0x477, 0, 10u, 33u, (3u, 3u, 127u)
#define PPUTLIMPL_UINT_TRAIT_1142u DEC, 0x476, 0, 10u, 33u, (2u, 571u)
#define PPUTLIMPL_UINT_TRAIT_1141u DEC, 0x475, 0, 10u, 33u, (7u, 163u)
#define PPUTLIMPL_UINT_TRAIT_1140u DEC, 0x474, 0, 10u, 33u, (2u, 2u, 3u, 5u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1139u DEC, 0x473, 0, 10u, 33u, (17u, 67u)
#define PPUTLIMPL_UINT_TRAIT_1138u DEC, 0x472, 0, 10u, 33u, (2u, 569u)
#define PPUTLIMPL_UINT_TRAIT_1137u DEC, 0x471, 0, 10u, 33u, (3u, 379u)
#define PPUTLIMPL_UINT_TRAIT_1136u DEC, 0x470, 0, 10u, 33u, (2u, 2u, 2u, 2u, 71u)
#define PPUTLIMPL_UINT_TRAIT_1135u DEC, 0x46F, 0, 10u, 33u, (5u, 227u)
#define PPUTLIMPL_UINT_TRAIT_1134u DEC, 0x46E, 0, 10u, 33u, (2u, 3u, 3u, 3u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_1133u DEC, 0x46D, 0, 10u, 33u, (11u, 103u)
#define PPUTLIMPL_UINT_TRAIT_1132u DEC, 0x46C, 0, 10u, 33u, (2u, 2u, 283u)
#define PPUTLIMPL_UINT_TRAIT_1131u DEC, 0x46B, 0, 10u, 33u, (3u, 13u, 29u)
#define PPUTLIMPL_UINT_TRAIT_1130u DEC, 0x46A, 0, 10u, 33u, (2u, 5u, 113u)
#define PPUTLIMPL_UINT_TRAIT_1129u DEC, 0x469, 0, 10u, 33u, ()
#define PPUTLIMPL_UINT_TRAIT_1128u DEC, 0x468, 0, 10u, 33u, (2u, 2u, 2u, 3u, 47u)
#define PPUTLIMPL_UINT_TRAIT_1127u DEC, 0x467, 0, 10u, 33u, (7u, 7u, 23u)
#define PPUTLIMPL_UINT_TRAIT_1126u DEC, 0x466, 0, 10u, 33u, (2u, 563u)
#define PPUTLIMPL_UINT_TRAIT_1125u DEC, 0x465, 0, 10u, 33u, (3u, 3u, 5u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_1124u DEC, 0x464, 0, 10u, 33u, (2u, 2u, 281u)
#define PPUTLIMPL_UINT_TRAIT_1123u DEC, 0x463, 0, 10u, 33u, ()
#define PPUTLIMPL_UINT_TRAIT_1122u DEC, 0x462, 0, 10u, 33u, (2u, 3u, 11u, 17u)
#define PPUTLIMPL_UINT_TRAIT_1121u DEC, 0x461, 0, 10u, 33u, (19u, 59u)
#define PPUTLIMPL_UINT_TRAIT_1120u DEC, 0x460, 0, 10u, 33u, (2u, 2u, 2u, 2u, 2u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_1119u DEC, 0x45F, 0, 10u, 33u, (3u, 373u)
#define PPUTLIMPL_UINT_TRAIT_1118u DEC, 0x45E, 0, 10u, 33u, (2u, 13u, 43u)
#define PPUTLIMPL_UINT_TRAIT_1117u DEC, 0x45D, 0, 10u, 33u, ()
#define PPUTLIMPL_UINT_TRAIT_1116u DEC, 0x45C, 0, 10u, 33u, (2u, 2u, 3u, 3u, 31u)
#define PPUTLIMPL_UINT_TRAIT_1115u DEC, 0x45B, 0, 10u, 33u, (5u, 223u)
#define PPUTLIMPL_UINT_TRAIT_1114u DEC, 0x45A, 0, 10u, 33u, (2u, 557u)
#define PPUTLIMPL_UINT_TRAIT_1113u DEC, 0x459, 0, 10u, 33u, (3u, 7u, 53u)
#define PPUTLIMPL_UINT_TRAIT_1112u DEC, 0x458, 0, 10u, 33u, (2u, 2u, 2u, 139u)
#define PPUTLIMPL_UINT_TRAIT_1111u DEC, 0x457, 0, 10u, 33u, (11u, 101u)
#define PPUTLIMPL_UINT_TRAIT_1110u DEC, 0x456, 0, 10u, 33u, (2u, 3u, 5u, 37u)
#define PPUTLIMPL_UINT_TRAIT_1109u DEC, 0x455, 0, 10u, 33u, ()
#define PPUTLIMPL_UINT_TRAIT_1108u DEC, 0x454, 0, 10u, 33u, (2u, 2u, 277u)
#define PPUTLIMPL_UINT_TRAIT_1107u DEC, 0x453, 0, 10u, 33u, (3u, 3u, 3u, 41u)
#define PPUTLIMPL_UINT_TRAIT_1106u DEC, 0x452, 0, 10u, 33u, (2u, 7u, 79u)
#define PPUTLIMPL_UINT_TRAIT_1105u DEC, 0x451, 0, 10u, 33u, (5u, 13u, 17u)
#define PPUTLIMPL_UINT_TRAIT_1104u DEC, 0x450, 0, 10u, 33u, (2u, 2u, 2u, 2u, 3u, 23u)
#define PPUTLIMPL_UINT_TRAIT_1103u DEC, 0x44F, 0, 10u, 33u, ()
#define PPUTLIMPL_UINT_TRAIT_1102u DEC, 0x44E, 0, 10u, 33u, (2u, 19u, 29u)
#define PPUTLIMPL_UINT_TRAIT_1101u DEC, 0x44D, 0, 10u, 33u, (3u, 367u)
#define PPUTLIMPL_UINT_TRAIT_1100u DEC, 0x44C, 0, 10u, 33u, (2u, 2u, 5u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_1099u DEC, 0x44B, 0, 10u, 33u, (7u, 157u)
#define PPUTLIMPL_UINT_TRAIT_1098u DEC, 0x44A, 0, 10u, 33u, (2u, 3u, 3u, 61u)
#define PPUTLIMPL_UINT_TRAIT_1097u DEC, 0x449, 0, 10u, 33u, ()
#define PPUTLIMPL_UINT_TRAIT_1096u DEC, 0x448, 0, 10u, 33u, (2u, 2u, 2u, 137u)
#define PPUTLIMPL_UINT_TRAIT_1095u DEC, 0x447, 0, 10u, 33u, (3u, 5u, 73u)
#define PPUTLIMPL_UINT_TRAIT_1094u DEC, 0x446, 0, 10u, 33u, (2u, 547u)
#define PPUTLIMPL_UINT_TRAIT_1093u DEC, 0x445, 0, 10u, 33u, ()
#define PPUTLIMPL_UINT_TRAIT_1092u DEC, 0x444, 0, 10u, 33u, (2u, 2u, 3u, 7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1091u DEC, 0x443, 0, 10u, 33u, ()
#define PPUTLIMPL_UINT_TRAIT_1090u DEC, 0x442, 0, 10u, 33u, (2u, 5u, 109u)
#define PPUTLIMPL_UINT_TRAIT_1089u DEC, 0x441, 0, 10u, 33u, (3u, 3u, 11u, 11u)
#define PPUTLIMPL_UINT_TRAIT_1088u DEC, 0x440, 0, 10u, 32u, (2u, 2u, 2u, 2u, 2u, 2u, 17u)
#define PPUTLIMPL_UINT_TRAIT_1087u DEC, 0x43F, 0, 10u, 32u, ()
#define PPUTLIMPL_UINT_TRAIT_1086u DEC, 0x43E, 0, 10u, 32u, (2u, 3u, 181u)
#define PPUTLIMPL_UINT_TRAIT_1085u DEC, 0x43D, 0, 10u, 32u, (5u, 7u, 31u)
#define PPUTLIMPL_UINT_TRAIT_1084u DEC, 0x43C, 0, 10u, 32u, (2u, 2u, 271u)
#define PPUTLIMPL_UINT_TRAIT_1083u DEC, 0x43B, 0, 10u, 32u, (3u, 19u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1082u DEC, 0x43A, 0, 10u, 32u, (2u, 541u)
#define PPUTLIMPL_UINT_TRAIT_1081u DEC, 0x439, 0, 10u, 32u, (23u, 47u)
#define PPUTLIMPL_UINT_TRAIT_1080u DEC, 0x438, 0, 10u, 32u, (2u, 2u, 2u, 3u, 3u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_1079u DEC, 0x437, 0, 10u, 32u, (13u, 83u)
#define PPUTLIMPL_UINT_TRAIT_1078u DEC, 0x436, 0, 10u, 32u, (2u, 7u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_1077u DEC, 0x435, 0, 10u, 32u, (3u, 359u)
#define PPUTLIMPL_UINT_TRAIT_1076u DEC, 0x434, 0, 10u, 32u, (2u, 2u, 269u)
#define PPUTLIMPL_UINT_TRAIT_1075u DEC, 0x433, 0, 10u, 32u, (5u, 5u, 43u)
#define PPUTLIMPL_UINT_TRAIT_1074u DEC, 0x432, 0, 10u, 32u, (2u, 3u, 179u)
#define PPUTLIMPL_UINT_TRAIT_1073u DEC, 0x431, 0, 10u, 32u, (29u, 37u)
#define PPUTLIMPL_UINT_TRAIT_1072u DEC, 0x430, 0, 10u, 32u, (2u, 2u, 2u, 2u, 67u)
#define PPUTLIMPL_UINT_TRAIT_1071u DEC, 0x42F, 0, 10u, 32u, (3u, 3u, 7u, 17u)
#define PPUTLIMPL_UINT_TRAIT_1070u DEC, 0x42E, 0, 10u, 32u, (2u, 5u, 107u)
#define PPUTLIMPL_UINT_TRAIT_1069u DEC, 0x42D, 0, 10u, 32u, ()
#define PPUTLIMPL_UINT_TRAIT_1068u DEC, 0x42C, 0, 10u, 32u, (2u, 2u, 3u, 89u)
#define PPUTLIMPL_UINT_TRAIT_1067u DEC, 0x42B, 0, 10u, 32u, (11u, 97u)
#define PPUTLIMPL_UINT_TRAIT_1066u DEC, 0x42A, 0, 10u, 32u, (2u, 13u, 41u)
#define PPUTLIMPL_UINT_TRAIT_1065u DEC, 0x429, 0, 10u, 32u, (3u, 5u, 71u)
#define PPUTLIMPL_UINT_TRAIT_1064u DEC, 0x428, 0, 10u, 32u, (2u, 2u, 2u, 7u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1063u DEC, 0x427, 0, 10u, 32u, ()
#define PPUTLIMPL_UINT_TRAIT_1062u DEC, 0x426, 0, 10u, 32u, (2u, 3u, 3u, 59u)
#define PPUTLIMPL_UINT_TRAIT_1061u DEC, 0x425, 0, 10u, 32u, ()
#define PPUTLIMPL_UINT_TRAIT_1060u DEC, 0x424, 0, 10u, 32u, (2u, 2u, 5u, 53u)
#define PPUTLIMPL_UINT_TRAIT_1059u DEC, 0x423, 0, 10u, 32u, (3u, 353u)
#define PPUTLIMPL_UINT_TRAIT_1058u DEC, 0x422, 0, 10u, 32u, (2u, 23u, 23u)
#define PPUTLIMPL_UINT_TRAIT_1057u DEC, 0x421, 0, 10u, 32u, (7u, 151u)
#define PPUTLIMPL_UINT_TRAIT_1056u DEC, 0x420, 0, 10u, 32u, (2u, 2u, 2u, 2u, 2u, 3u, 11u)
#define PPUTLIMPL_UINT_TRAIT_1055u DEC, 0x41F, 0, 10u, 32u, (5u, 211u)
#define PPUTLIMPL_UINT_TRAIT_1054u DEC, 0x41E, 0, 10u, 32u, (2u, 17u, 31u)
#define PPUTLIMPL_UINT_TRAIT_1053u DEC, 0x41D, 0, 10u, 32u, (3u, 3u, 3u, 3u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1052u DEC, 0x41C, 0, 10u, 32u, (2u, 2u, 263u)
#define PPUTLIMPL_UINT_TRAIT_1051u DEC, 0x41B, 0, 10u, 32u, ()
#define PPUTLIMPL_UINT_TRAIT_1050u DEC, 0x41A, 0, 10u, 32u, (2u, 3u, 5u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_1049u DEC, 0x419, 0, 10u, 32u, ()
#define PPUTLIMPL_UINT_TRAIT_1048u DEC, 0x418, 0, 10u, 32u, (2u, 2u, 2u, 131u)
#define PPUTLIMPL_UINT_TRAIT_1047u DEC, 0x417, 0, 10u, 32u, (3u, 349u)
#define PPUTLIMPL_UINT_TRAIT_1046u DEC, 0x416, 0, 10u, 32u, (2u, 523u)
#define PPUTLIMPL_UINT_TRAIT_1045u DEC, 0x415, 0, 10u, 32u, (5u, 11u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1044u DEC, 0x414, 0, 10u, 32u, (2u, 2u, 3u, 3u, 29u)
#define PPUTLIMPL_UINT_TRAIT_1043u DEC, 0x413, 0, 10u, 32u, (7u, 149u)
#define PPUTLIMPL_UINT_TRAIT_1042u DEC, 0x412, 0, 10u, 32u, (2u, 521u)
#define PPUTLIMPL_UINT_TRAIT_1041u DEC, 0x411, 0, 10u, 32u, (3u, 347u)
#define PPUTLIMPL_UINT_TRAIT_1040u DEC, 0x410, 0, 10u, 32u, (2u, 2u, 2u, 2u, 5u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1039u DEC, 0x40F, 0, 10u, 32u, ()
#define PPUTLIMPL_UINT_TRAIT_1038u DEC, 0x40E, 0, 10u, 32u, (2u, 3u, 173u)
#define PPUTLIMPL_UINT_TRAIT_1037u DEC, 0x40D, 0, 10u, 32u, (17u, 61u)
#define PPUTLIMPL_UINT_TRAIT_1036u DEC, 0x40C, 0, 10u, 32u, (2u, 2u, 7u, 37u)
#define PPUTLIMPL_UINT_TRAIT_1035u DEC, 0x40B, 0, 10u, 32u, (3u, 3u, 5u, 23u)
#define PPUTLIMPL_UINT_TRAIT_1034u DEC, 0x40A, 0, 10u, 32u, (2u, 11u, 47u)
#define PPUTLIMPL_UINT_TRAIT_1033u DEC, 0x409, 0, 10u, 32u, ()
#define PPUTLIMPL_UINT_TRAIT_1032u DEC, 0x408, 0, 10u, 32u, (2u, 2u, 2u, 3u, 43u)
#define PPUTLIMPL_UINT_TRAIT_1031u DEC, 0x407, 0, 10u, 32u, ()
#define PPUTLIMPL_UINT_TRAIT_1030u DEC, 0x406, 0, 10u, 32u, (2u, 5u, 103u)
#define PPUTLIMPL_UINT_TRAIT_1029u DEC, 0x405, 0, 10u, 32u, (3u, 7u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_1028u DEC, 0x404, 0, 10u, 32u, (2u, 2u, 257u)
#define PPUTLIMPL_UINT_TRAIT_1027u DEC, 0x403, 0, 10u, 32u, (13u, 79u)
#define PPUTLIMPL_UINT_TRAIT_1026u DEC, 0x402, 0, 10u, 32u, (2u, 3u, 3u, 3u, 19u)
#define PPUTLIMPL_UINT_TRAIT_1025u DEC, 0x401, 0, 10u, 32u, (5u, 5u, 41u)
#define PPUTLIMPL_UINT_TRAIT_1024u DEC, 0x400, 0, 10u, 32u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u)
#define PPUTLIMPL_UINT_TRAIT_1023u DEC, 0x3FF, 0, 9u, 31u, (3u, 11u, 31u)
#define PPUTLIMPL_UINT_TRAIT_1022u DEC, 0x3FE, 0, 9u, 31u, (2u, 7u, 73u)
#define PPUTLIMPL_UINT_TRAIT_1021u DEC, 0x3FD, 0, 9u, 31u, ()
#define PPUTLIMPL_UINT_TRAIT_1020u DEC, 0x3FC, 0, 9u, 31u, (2u, 2u, 3u, 5u, 17u)
#define PPUTLIMPL_UINT_TRAIT_1019u DEC, 0x3FB, 0, 9u, 31u, ()
#define PPUTLIMPL_UINT_TRAIT_1018u DEC, 0x3FA, 0, 9u, 31u, (2u, 509u)
#define PPUTLIMPL_UINT_TRAIT_1017u DEC, 0x3F9, 0, 9u, 31u, (3u, 3u, 113u)
#define PPUTLIMPL_UINT_TRAIT_1016u DEC, 0x3F8, 0, 9u, 31u, (2u, 2u, 2u, 127u)
#define PPUTLIMPL_UINT_TRAIT_1015u DEC, 0x3F7, 0, 9u, 31u, (5u, 7u, 29u)
#define PPUTLIMPL_UINT_TRAIT_1014u DEC, 0x3F6, 0, 9u, 31u, (2u, 3u, 13u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1013u DEC, 0x3F5, 0, 9u, 31u, ()
#define PPUTLIMPL_UINT_TRAIT_1012u DEC, 0x3F4, 0, 9u, 31u, (2u, 2u, 11u, 23u)
#define PPUTLIMPL_UINT_TRAIT_1011u DEC, 0x3F3, 0, 9u, 31u, (3u, 337u)
#define PPUTLIMPL_UINT_TRAIT_1010u DEC, 0x3F2, 0, 9u, 31u, (2u, 5u, 101u)
#define PPUTLIMPL_UINT_TRAIT_1009u DEC, 0x3F1, 0, 9u, 31u, ()
#define PPUTLIMPL_UINT_TRAIT_1008u DEC, 0x3F0, 0, 9u, 31u, (2u, 2u, 2u, 2u, 3u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_1007u DEC, 0x3EF, 0, 9u, 31u, (19u, 53u)
#define PPUTLIMPL_UINT_TRAIT_1006u DEC, 0x3EE, 0, 9u, 31u, (2u, 503u)
#define PPUTLIMPL_UINT_TRAIT_1005u DEC, 0x3ED, 0, 9u, 31u, (3u, 5u, 67u)
#define PPUTLIMPL_UINT_TRAIT_1004u DEC, 0x3EC, 0, 9u, 31u, (2u, 2u, 251u)
#define PPUTLIMPL_UINT_TRAIT_1003u DEC, 0x3EB, 0, 9u, 31u, (17u, 59u)
#define PPUTLIMPL_UINT_TRAIT_1002u DEC, 0x3EA, 0, 9u, 31u, (2u, 3u, 167u)
#define PPUTLIMPL_UINT_TRAIT_1001u DEC, 0x3E9, 0, 9u, 31u, (7u, 11u, 13u)
#define PPUTLIMPL_UINT_TRAIT_1000u DEC, 0x3E8, 0, 9u, 31u, (2u, 2u, 2u, 5u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_999u  DEC, 0x3E7, 0, 9u, 31u, (3u, 3u, 3u, 37u)
#define PPUTLIMPL_UINT_TRAIT_998u  DEC, 0x3E6, 0, 9u, 31u, (2u, 499u)
#define PPUTLIMPL_UINT_TRAIT_997u  DEC, 0x3E5, 0, 9u, 31u, ()
#define PPUTLIMPL_UINT_TRAIT_996u  DEC, 0x3E4, 0, 9u, 31u, (2u, 2u, 3u, 83u)
#define PPUTLIMPL_UINT_TRAIT_995u  DEC, 0x3E3, 0, 9u, 31u, (5u, 199u)
#define PPUTLIMPL_UINT_TRAIT_994u  DEC, 0x3E2, 0, 9u, 31u, (2u, 7u, 71u)
#define PPUTLIMPL_UINT_TRAIT_993u  DEC, 0x3E1, 0, 9u, 31u, (3u, 331u)
#define PPUTLIMPL_UINT_TRAIT_992u  DEC, 0x3E0, 0, 9u, 31u, (2u, 2u, 2u, 2u, 2u, 31u)
#define PPUTLIMPL_UINT_TRAIT_991u  DEC, 0x3DF, 0, 9u, 31u, ()
#define PPUTLIMPL_UINT_TRAIT_990u  DEC, 0x3DE, 0, 9u, 31u, (2u, 3u, 3u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_989u  DEC, 0x3DD, 0, 9u, 31u, (23u, 43u)
#define PPUTLIMPL_UINT_TRAIT_988u  DEC, 0x3DC, 0, 9u, 31u, (2u, 2u, 13u, 19u)
#define PPUTLIMPL_UINT_TRAIT_987u  DEC, 0x3DB, 0, 9u, 31u, (3u, 7u, 47u)
#define PPUTLIMPL_UINT_TRAIT_986u  DEC, 0x3DA, 0, 9u, 31u, (2u, 17u, 29u)
#define PPUTLIMPL_UINT_TRAIT_985u  DEC, 0x3D9, 0, 9u, 31u, (5u, 197u)
#define PPUTLIMPL_UINT_TRAIT_984u  DEC, 0x3D8, 0, 9u, 31u, (2u, 2u, 2u, 3u, 41u)
#define PPUTLIMPL_UINT_TRAIT_983u  DEC, 0x3D7, 0, 9u, 31u, ()
#define PPUTLIMPL_UINT_TRAIT_982u  DEC, 0x3D6, 0, 9u, 31u, (2u, 491u)
#define PPUTLIMPL_UINT_TRAIT_981u  DEC, 0x3D5, 0, 9u, 31u, (3u, 3u, 109u)
#define PPUTLIMPL_UINT_TRAIT_980u  DEC, 0x3D4, 0, 9u, 31u, (2u, 2u, 5u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_979u  DEC, 0x3D3, 0, 9u, 31u, (11u, 89u)
#define PPUTLIMPL_UINT_TRAIT_978u  DEC, 0x3D2, 0, 9u, 31u, (2u, 3u, 163u)
#define PPUTLIMPL_UINT_TRAIT_977u  DEC, 0x3D1, 0, 9u, 31u, ()
#define PPUTLIMPL_UINT_TRAIT_976u  DEC, 0x3D0, 0, 9u, 31u, (2u, 2u, 2u, 2u, 61u)
#define PPUTLIMPL_UINT_TRAIT_975u  DEC, 0x3CF, 0, 9u, 31u, (3u, 5u, 5u, 13u)
#define PPUTLIMPL_UINT_TRAIT_974u  DEC, 0x3CE, 0, 9u, 31u, (2u, 487u)
#define PPUTLIMPL_UINT_TRAIT_973u  DEC, 0x3CD, 0, 9u, 31u, (7u, 139u)
#define PPUTLIMPL_UINT_TRAIT_972u  DEC, 0x3CC, 0, 9u, 31u, (2u, 2u, 3u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_971u  DEC, 0x3CB, 0, 9u, 31u, ()
#define PPUTLIMPL_UINT_TRAIT_970u  DEC, 0x3CA, 0, 9u, 31u, (2u, 5u, 97u)
#define PPUTLIMPL_UINT_TRAIT_969u  DEC, 0x3C9, 0, 9u, 31u, (3u, 17u, 19u)
#define PPUTLIMPL_UINT_TRAIT_968u  DEC, 0x3C8, 0, 9u, 31u, (2u, 2u, 2u, 11u, 11u)
#define PPUTLIMPL_UINT_TRAIT_967u  DEC, 0x3C7, 0, 9u, 31u, ()
#define PPUTLIMPL_UINT_TRAIT_966u  DEC, 0x3C6, 0, 9u, 31u, (2u, 3u, 7u, 23u)
#define PPUTLIMPL_UINT_TRAIT_965u  DEC, 0x3C5, 0, 9u, 31u, (5u, 193u)
#define PPUTLIMPL_UINT_TRAIT_964u  DEC, 0x3C4, 0, 9u, 31u, (2u, 2u, 241u)
#define PPUTLIMPL_UINT_TRAIT_963u  DEC, 0x3C3, 0, 9u, 31u, (3u, 3u, 107u)
#define PPUTLIMPL_UINT_TRAIT_962u  DEC, 0x3C2, 0, 9u, 31u, (2u, 13u, 37u)
#define PPUTLIMPL_UINT_TRAIT_961u  DEC, 0x3C1, 0, 9u, 31u, (31u, 31u)
#define PPUTLIMPL_UINT_TRAIT_960u  DEC, 0x3C0, 0, 9u, 30u, (2u, 2u, 2u, 2u, 2u, 2u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_959u  DEC, 0x3BF, 0, 9u, 30u, (7u, 137u)
#define PPUTLIMPL_UINT_TRAIT_958u  DEC, 0x3BE, 0, 9u, 30u, (2u, 479u)
#define PPUTLIMPL_UINT_TRAIT_957u  DEC, 0x3BD, 0, 9u, 30u, (3u, 11u, 29u)
#define PPUTLIMPL_UINT_TRAIT_956u  DEC, 0x3BC, 0, 9u, 30u, (2u, 2u, 239u)
#define PPUTLIMPL_UINT_TRAIT_955u  DEC, 0x3BB, 0, 9u, 30u, (5u, 191u)
#define PPUTLIMPL_UINT_TRAIT_954u  DEC, 0x3BA, 0, 9u, 30u, (2u, 3u, 3u, 53u)
#define PPUTLIMPL_UINT_TRAIT_953u  DEC, 0x3B9, 0, 9u, 30u, ()
#define PPUTLIMPL_UINT_TRAIT_952u  DEC, 0x3B8, 0, 9u, 30u, (2u, 2u, 2u, 7u, 17u)
#define PPUTLIMPL_UINT_TRAIT_951u  DEC, 0x3B7, 0, 9u, 30u, (3u, 317u)
#define PPUTLIMPL_UINT_TRAIT_950u  DEC, 0x3B6, 0, 9u, 30u, (2u, 5u, 5u, 19u)
#define PPUTLIMPL_UINT_TRAIT_949u  DEC, 0x3B5, 0, 9u, 30u, (13u, 73u)
#define PPUTLIMPL_UINT_TRAIT_948u  DEC, 0x3B4, 0, 9u, 30u, (2u, 2u, 3u, 79u)
#define PPUTLIMPL_UINT_TRAIT_947u  DEC, 0x3B3, 0, 9u, 30u, ()
#define PPUTLIMPL_UINT_TRAIT_946u  DEC, 0x3B2, 0, 9u, 30u, (2u, 11u, 43u)
#define PPUTLIMPL_UINT_TRAIT_945u  DEC, 0x3B1, 0, 9u, 30u, (3u, 3u, 3u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_944u  DEC, 0x3B0, 0, 9u, 30u, (2u, 2u, 2u, 2u, 59u)
#define PPUTLIMPL_UINT_TRAIT_943u  DEC, 0x3AF, 0, 9u, 30u, (23u, 41u)
#define PPUTLIMPL_UINT_TRAIT_942u  DEC, 0x3AE, 0, 9u, 30u, (2u, 3u, 157u)
#define PPUTLIMPL_UINT_TRAIT_941u  DEC, 0x3AD, 0, 9u, 30u, ()
#define PPUTLIMPL_UINT_TRAIT_940u  DEC, 0x3AC, 0, 9u, 30u, (2u, 2u, 5u, 47u)
#define PPUTLIMPL_UINT_TRAIT_939u  DEC, 0x3AB, 0, 9u, 30u, (3u, 313u)
#define PPUTLIMPL_UINT_TRAIT_938u  DEC, 0x3AA, 0, 9u, 30u, (2u, 7u, 67u)
#define PPUTLIMPL_UINT_TRAIT_937u  DEC, 0x3A9, 0, 9u, 30u, ()
#define PPUTLIMPL_UINT_TRAIT_936u  DEC, 0x3A8, 0, 9u, 30u, (2u, 2u, 2u, 3u, 3u, 13u)
#define PPUTLIMPL_UINT_TRAIT_935u  DEC, 0x3A7, 0, 9u, 30u, (5u, 11u, 17u)
#define PPUTLIMPL_UINT_TRAIT_934u  DEC, 0x3A6, 0, 9u, 30u, (2u, 467u)
#define PPUTLIMPL_UINT_TRAIT_933u  DEC, 0x3A5, 0, 9u, 30u, (3u, 311u)
#define PPUTLIMPL_UINT_TRAIT_932u  DEC, 0x3A4, 0, 9u, 30u, (2u, 2u, 233u)
#define PPUTLIMPL_UINT_TRAIT_931u  DEC, 0x3A3, 0, 9u, 30u, (7u, 7u, 19u)
#define PPUTLIMPL_UINT_TRAIT_930u  DEC, 0x3A2, 0, 9u, 30u, (2u, 3u, 5u, 31u)
#define PPUTLIMPL_UINT_TRAIT_929u  DEC, 0x3A1, 0, 9u, 30u, ()
#define PPUTLIMPL_UINT_TRAIT_928u  DEC, 0x3A0, 0, 9u, 30u, (2u, 2u, 2u, 2u, 2u, 29u)
#define PPUTLIMPL_UINT_TRAIT_927u  DEC, 0x39F, 0, 9u, 30u, (3u, 3u, 103u)
#define PPUTLIMPL_UINT_TRAIT_926u  DEC, 0x39E, 0, 9u, 30u, (2u, 463u)
#define PPUTLIMPL_UINT_TRAIT_925u  DEC, 0x39D, 0, 9u, 30u, (5u, 5u, 37u)
#define PPUTLIMPL_UINT_TRAIT_924u  DEC, 0x39C, 0, 9u, 30u, (2u, 2u, 3u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_923u  DEC, 0x39B, 0, 9u, 30u, (13u, 71u)
#define PPUTLIMPL_UINT_TRAIT_922u  DEC, 0x39A, 0, 9u, 30u, (2u, 461u)
#define PPUTLIMPL_UINT_TRAIT_921u  DEC, 0x399, 0, 9u, 30u, (3u, 307u)
#define PPUTLIMPL_UINT_TRAIT_920u  DEC, 0x398, 0, 9u, 30u, (2u, 2u, 2u, 5u, 23u)
#define PPUTLIMPL_UINT_TRAIT_919u  DEC, 0x397, 0, 9u, 30u, ()
#define PPUTLIMPL_UINT_TRAIT_918u  DEC, 0x396, 0, 9u, 30u, (2u, 3u, 3u, 3u, 17u)
#define PPUTLIMPL_UINT_TRAIT_917u  DEC, 0x395, 0, 9u, 30u, (7u, 131u)
#define PPUTLIMPL_UINT_TRAIT_916u  DEC, 0x394, 0, 9u, 30u, (2u, 2u, 229u)
#define PPUTLIMPL_UINT_TRAIT_915u  DEC, 0x393, 0, 9u, 30u, (3u, 5u, 61u)
#define PPUTLIMPL_UINT_TRAIT_914u  DEC, 0x392, 0, 9u, 30u, (2u, 457u)
#define PPUTLIMPL_UINT_TRAIT_913u  DEC, 0x391, 0, 9u, 30u, (11u, 83u)
#define PPUTLIMPL_UINT_TRAIT_912u  DEC, 0x390, 0, 9u, 30u, (2u, 2u, 2u, 2u, 3u, 19u)
#define PPUTLIMPL_UINT_TRAIT_911u  DEC, 0x38F, 0, 9u, 30u, ()
#define PPUTLIMPL_UINT_TRAIT_910u  DEC, 0x38E, 0, 9u, 30u, (2u, 5u, 7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_909u  DEC, 0x38D, 0, 9u, 30u, (3u, 3u, 101u)
#define PPUTLIMPL_UINT_TRAIT_908u  DEC, 0x38C, 0, 9u, 30u, (2u, 2u, 227u)
#define PPUTLIMPL_UINT_TRAIT_907u  DEC, 0x38B, 0, 9u, 30u, ()
#define PPUTLIMPL_UINT_TRAIT_906u  DEC, 0x38A, 0, 9u, 30u, (2u, 3u, 151u)
#define PPUTLIMPL_UINT_TRAIT_905u  DEC, 0x389, 0, 9u, 30u, (5u, 181u)
#define PPUTLIMPL_UINT_TRAIT_904u  DEC, 0x388, 0, 9u, 30u, (2u, 2u, 2u, 113u)
#define PPUTLIMPL_UINT_TRAIT_903u  DEC, 0x387, 0, 9u, 30u, (3u, 7u, 43u)
#define PPUTLIMPL_UINT_TRAIT_902u  DEC, 0x386, 0, 9u, 30u, (2u, 11u, 41u)
#define PPUTLIMPL_UINT_TRAIT_901u  DEC, 0x385, 0, 9u, 30u, (17u, 53u)
#define PPUTLIMPL_UINT_TRAIT_900u  DEC, 0x384, 0, 9u, 30u, (2u, 2u, 3u, 3u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_899u  DEC, 0x383, 0, 9u, 29u, (29u, 31u)
#define PPUTLIMPL_UINT_TRAIT_898u  DEC, 0x382, 0, 9u, 29u, (2u, 449u)
#define PPUTLIMPL_UINT_TRAIT_897u  DEC, 0x381, 0, 9u, 29u, (3u, 13u, 23u)
#define PPUTLIMPL_UINT_TRAIT_896u  DEC, 0x380, 0, 9u, 29u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 7u)
#define PPUTLIMPL_UINT_TRAIT_895u  DEC, 0x37F, 0, 9u, 29u, (5u, 179u)
#define PPUTLIMPL_UINT_TRAIT_894u  DEC, 0x37E, 0, 9u, 29u, (2u, 3u, 149u)
#define PPUTLIMPL_UINT_TRAIT_893u  DEC, 0x37D, 0, 9u, 29u, (19u, 47u)
#define PPUTLIMPL_UINT_TRAIT_892u  DEC, 0x37C, 0, 9u, 29u, (2u, 2u, 223u)
#define PPUTLIMPL_UINT_TRAIT_891u  DEC, 0x37B, 0, 9u, 29u, (3u, 3u, 3u, 3u, 11u)
#define PPUTLIMPL_UINT_TRAIT_890u  DEC, 0x37A, 0, 9u, 29u, (2u, 5u, 89u)
#define PPUTLIMPL_UINT_TRAIT_889u  DEC, 0x379, 0, 9u, 29u, (7u, 127u)
#define PPUTLIMPL_UINT_TRAIT_888u  DEC, 0x378, 0, 9u, 29u, (2u, 2u, 2u, 3u, 37u)
#define PPUTLIMPL_UINT_TRAIT_887u  DEC, 0x377, 0, 9u, 29u, ()
#define PPUTLIMPL_UINT_TRAIT_886u  DEC, 0x376, 0, 9u, 29u, (2u, 443u)
#define PPUTLIMPL_UINT_TRAIT_885u  DEC, 0x375, 0, 9u, 29u, (3u, 5u, 59u)
#define PPUTLIMPL_UINT_TRAIT_884u  DEC, 0x374, 0, 9u, 29u, (2u, 2u, 13u, 17u)
#define PPUTLIMPL_UINT_TRAIT_883u  DEC, 0x373, 0, 9u, 29u, ()
#define PPUTLIMPL_UINT_TRAIT_882u  DEC, 0x372, 0, 9u, 29u, (2u, 3u, 3u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_881u  DEC, 0x371, 0, 9u, 29u, ()
#define PPUTLIMPL_UINT_TRAIT_880u  DEC, 0x370, 0, 9u, 29u, (2u, 2u, 2u, 2u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_879u  DEC, 0x36F, 0, 9u, 29u, (3u, 293u)
#define PPUTLIMPL_UINT_TRAIT_878u  DEC, 0x36E, 0, 9u, 29u, (2u, 439u)
#define PPUTLIMPL_UINT_TRAIT_877u  DEC, 0x36D, 0, 9u, 29u, ()
#define PPUTLIMPL_UINT_TRAIT_876u  DEC, 0x36C, 0, 9u, 29u, (2u, 2u, 3u, 73u)
#define PPUTLIMPL_UINT_TRAIT_875u  DEC, 0x36B, 0, 9u, 29u, (5u, 5u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_874u  DEC, 0x36A, 0, 9u, 29u, (2u, 19u, 23u)
#define PPUTLIMPL_UINT_TRAIT_873u  DEC, 0x369, 0, 9u, 29u, (3u, 3u, 97u)
#define PPUTLIMPL_UINT_TRAIT_872u  DEC, 0x368, 0, 9u, 29u, (2u, 2u, 2u, 109u)
#define PPUTLIMPL_UINT_TRAIT_871u  DEC, 0x367, 0, 9u, 29u, (13u, 67u)
#define PPUTLIMPL_UINT_TRAIT_870u  DEC, 0x366, 0, 9u, 29u, (2u, 3u, 5u, 29u)
#define PPUTLIMPL_UINT_TRAIT_869u  DEC, 0x365, 0, 9u, 29u, (11u, 79u)
#define PPUTLIMPL_UINT_TRAIT_868u  DEC, 0x364, 0, 9u, 29u, (2u, 2u, 7u, 31u)
#define PPUTLIMPL_UINT_TRAIT_867u  DEC, 0x363, 0, 9u, 29u, (3u, 17u, 17u)
#define PPUTLIMPL_UINT_TRAIT_866u  DEC, 0x362, 0, 9u, 29u, (2u, 433u)
#define PPUTLIMPL_UINT_TRAIT_865u  DEC, 0x361, 0, 9u, 29u, (5u, 173u)
#define PPUTLIMPL_UINT_TRAIT_864u  DEC, 0x360, 0, 9u, 29u, (2u, 2u, 2u, 2u, 2u, 3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_863u  DEC, 0x35F, 0, 9u, 29u, ()
#define PPUTLIMPL_UINT_TRAIT_862u  DEC, 0x35E, 0, 9u, 29u, (2u, 431u)
#define PPUTLIMPL_UINT_TRAIT_861u  DEC, 0x35D, 0, 9u, 29u, (3u, 7u, 41u)
#define PPUTLIMPL_UINT_TRAIT_860u  DEC, 0x35C, 0, 9u, 29u, (2u, 2u, 5u, 43u)
#define PPUTLIMPL_UINT_TRAIT_859u  DEC, 0x35B, 0, 9u, 29u, ()
#define PPUTLIMPL_UINT_TRAIT_858u  DEC, 0x35A, 0, 9u, 29u, (2u, 3u, 11u, 13u)
#define PPUTLIMPL_UINT_TRAIT_857u  DEC, 0x359, 0, 9u, 29u, ()
#define PPUTLIMPL_UINT_TRAIT_856u  DEC, 0x358, 0, 9u, 29u, (2u, 2u, 2u, 107u)
#define PPUTLIMPL_UINT_TRAIT_855u  DEC, 0x357, 0, 9u, 29u, (3u, 3u, 5u, 19u)
#define PPUTLIMPL_UINT_TRAIT_854u  DEC, 0x356, 0, 9u, 29u, (2u, 7u, 61u)
#define PPUTLIMPL_UINT_TRAIT_853u  DEC, 0x355, 0, 9u, 29u, ()
#define PPUTLIMPL_UINT_TRAIT_852u  DEC, 0x354, 0, 9u, 29u, (2u, 2u, 3u, 71u)
#define PPUTLIMPL_UINT_TRAIT_851u  DEC, 0x353, 0, 9u, 29u, (23u, 37u)
#define PPUTLIMPL_UINT_TRAIT_850u  DEC, 0x352, 0, 9u, 29u, (2u, 5u, 5u, 17u)
#define PPUTLIMPL_UINT_TRAIT_849u  DEC, 0x351, 0, 9u, 29u, (3u, 283u)
#define PPUTLIMPL_UINT_TRAIT_848u  DEC, 0x350, 0, 9u, 29u, (2u, 2u, 2u, 2u, 53u)
#define PPUTLIMPL_UINT_TRAIT_847u  DEC, 0x34F, 0, 9u, 29u, (7u, 11u, 11u)
#define PPUTLIMPL_UINT_TRAIT_846u  DEC, 0x34E, 0, 9u, 29u, (2u, 3u, 3u, 47u)
#define PPUTLIMPL_UINT_TRAIT_845u  DEC, 0x34D, 0, 9u, 29u, (5u, 13u, 13u)
#define PPUTLIMPL_UINT_TRAIT_844u  DEC, 0x34C, 0, 9u, 29u, (2u, 2u, 211u)
#define PPUTLIMPL_UINT_TRAIT_843u  DEC, 0x34B, 0, 9u, 29u, (3u, 281u)
#define PPUTLIMPL_UINT_TRAIT_842u  DEC, 0x34A, 0, 9u, 29u, (2u, 421u)
#define PPUTLIMPL_UINT_TRAIT_841u  DEC, 0x349, 0, 9u, 29u, (29u, 29u)
#define PPUTLIMPL_UINT_TRAIT_840u  DEC, 0x348, 0, 9u, 28u, (2u, 2u, 2u, 3u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_839u  DEC, 0x347, 0, 9u, 28u, ()
#define PPUTLIMPL_UINT_TRAIT_838u  DEC, 0x346, 0, 9u, 28u, (2u, 419u)
#define PPUTLIMPL_UINT_TRAIT_837u  DEC, 0x345, 0, 9u, 28u, (3u, 3u, 3u, 31u)
#define PPUTLIMPL_UINT_TRAIT_836u  DEC, 0x344, 0, 9u, 28u, (2u, 2u, 11u, 19u)
#define PPUTLIMPL_UINT_TRAIT_835u  DEC, 0x343, 0, 9u, 28u, (5u, 167u)
#define PPUTLIMPL_UINT_TRAIT_834u  DEC, 0x342, 0, 9u, 28u, (2u, 3u, 139u)
#define PPUTLIMPL_UINT_TRAIT_833u  DEC, 0x341, 0, 9u, 28u, (7u, 7u, 17u)
#define PPUTLIMPL_UINT_TRAIT_832u  DEC, 0x340, 0, 9u, 28u, (2u, 2u, 2u, 2u, 2u, 2u, 13u)
#define PPUTLIMPL_UINT_TRAIT_831u  DEC, 0x33F, 0, 9u, 28u, (3u, 277u)
#define PPUTLIMPL_UINT_TRAIT_830u  DEC, 0x33E, 0, 9u, 28u, (2u, 5u, 83u)
#define PPUTLIMPL_UINT_TRAIT_829u  DEC, 0x33D, 0, 9u, 28u, ()
#define PPUTLIMPL_UINT_TRAIT_828u  DEC, 0x33C, 0, 9u, 28u, (2u, 2u, 3u, 3u, 23u)
#define PPUTLIMPL_UINT_TRAIT_827u  DEC, 0x33B, 0, 9u, 28u, ()
#define PPUTLIMPL_UINT_TRAIT_826u  DEC, 0x33A, 0, 9u, 28u, (2u, 7u, 59u)
#define PPUTLIMPL_UINT_TRAIT_825u  DEC, 0x339, 0, 9u, 28u, (3u, 5u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_824u  DEC, 0x338, 0, 9u, 28u, (2u, 2u, 2u, 103u)
#define PPUTLIMPL_UINT_TRAIT_823u  DEC, 0x337, 0, 9u, 28u, ()
#define PPUTLIMPL_UINT_TRAIT_822u  DEC, 0x336, 0, 9u, 28u, (2u, 3u, 137u)
#define PPUTLIMPL_UINT_TRAIT_821u  DEC, 0x335, 0, 9u, 28u, ()
#define PPUTLIMPL_UINT_TRAIT_820u  DEC, 0x334, 0, 9u, 28u, (2u, 2u, 5u, 41u)
#define PPUTLIMPL_UINT_TRAIT_819u  DEC, 0x333, 0, 9u, 28u, (3u, 3u, 7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_818u  DEC, 0x332, 0, 9u, 28u, (2u, 409u)
#define PPUTLIMPL_UINT_TRAIT_817u  DEC, 0x331, 0, 9u, 28u, (19u, 43u)
#define PPUTLIMPL_UINT_TRAIT_816u  DEC, 0x330, 0, 9u, 28u, (2u, 2u, 2u, 2u, 3u, 17u)
#define PPUTLIMPL_UINT_TRAIT_815u  DEC, 0x32F, 0, 9u, 28u, (5u, 163u)
#define PPUTLIMPL_UINT_TRAIT_814u  DEC, 0x32E, 0, 9u, 28u, (2u, 11u, 37u)
#define PPUTLIMPL_UINT_TRAIT_813u  DEC, 0x32D, 0, 9u, 28u, (3u, 271u)
#define PPUTLIMPL_UINT_TRAIT_812u  DEC, 0x32C, 0, 9u, 28u, (2u, 2u, 7u, 29u)
#define PPUTLIMPL_UINT_TRAIT_811u  DEC, 0x32B, 0, 9u, 28u, ()
#define PPUTLIMPL_UINT_TRAIT_810u  DEC, 0x32A, 0, 9u, 28u, (2u, 3u, 3u, 3u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_809u  DEC, 0x329, 0, 9u, 28u, ()
#define PPUTLIMPL_UINT_TRAIT_808u  DEC, 0x328, 0, 9u, 28u, (2u, 2u, 2u, 101u)
#define PPUTLIMPL_UINT_TRAIT_807u  DEC, 0x327, 0, 9u, 28u, (3u, 269u)
#define PPUTLIMPL_UINT_TRAIT_806u  DEC, 0x326, 0, 9u, 28u, (2u, 13u, 31u)
#define PPUTLIMPL_UINT_TRAIT_805u  DEC, 0x325, 0, 9u, 28u, (5u, 7u, 23u)
#define PPUTLIMPL_UINT_TRAIT_804u  DEC, 0x324, 0, 9u, 28u, (2u, 2u, 3u, 67u)
#define PPUTLIMPL_UINT_TRAIT_803u  DEC, 0x323, 0, 9u, 28u, (11u, 73u)
#define PPUTLIMPL_UINT_TRAIT_802u  DEC, 0x322, 0, 9u, 28u, (2u, 401u)
#define PPUTLIMPL_UINT_TRAIT_801u  DEC, 0x321, 0, 9u, 28u, (3u, 3u, 89u)
#define PPUTLIMPL_UINT_TRAIT_800u  DEC, 0x320, 0, 9u, 28u, (2u, 2u, 2u, 2u, 2u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_799u  DEC, 0x31F, 0, 9u, 28u, (17u, 47u)
#define PPUTLIMPL_UINT_TRAIT_798u  DEC, 0x31E, 0, 9u, 28u, (2u, 3u, 7u, 19u)
#define PPUTLIMPL_UINT_TRAIT_797u  DEC, 0x31D, 0, 9u, 28u, ()
#define PPUTLIMPL_UINT_TRAIT_796u  DEC, 0x31C, 0, 9u, 28u, (2u, 2u, 199u)
#define PPUTLIMPL_UINT_TRAIT_795u  DEC, 0x31B, 0, 9u, 28u, (3u, 5u, 53u)
#define PPUTLIMPL_UINT_TRAIT_794u  DEC, 0x31A, 0, 9u, 28u, (2u, 397u)
#define PPUTLIMPL_UINT_TRAIT_793u  DEC, 0x319, 0, 9u, 28u, (13u, 61u)
#define PPUTLIMPL_UINT_TRAIT_792u  DEC, 0x318, 0, 9u, 28u, (2u, 2u, 2u, 3u, 3u, 11u)
#define PPUTLIMPL_UINT_TRAIT_791u  DEC, 0x317, 0, 9u, 28u, (7u, 113u)
#define PPUTLIMPL_UINT_TRAIT_790u  DEC, 0x316, 0, 9u, 28u, (2u, 5u, 79u)
#define PPUTLIMPL_UINT_TRAIT_789u  DEC, 0x315, 0, 9u, 28u, (3u, 263u)
#define PPUTLIMPL_UINT_TRAIT_788u  DEC, 0x314, 0, 9u, 28u, (2u, 2u, 197u)
#define PPUTLIMPL_UINT_TRAIT_787u  DEC, 0x313, 0, 9u, 28u, ()
#define PPUTLIMPL_UINT_TRAIT_786u  DEC, 0x312, 0, 9u, 28u, (2u, 3u, 131u)
#define PPUTLIMPL_UINT_TRAIT_785u  DEC, 0x311, 0, 9u, 28u, (5u, 157u)
#define PPUTLIMPL_UINT_TRAIT_784u  DEC, 0x310, 0, 9u, 28u, (2u, 2u, 2u, 2u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_783u  DEC, 0x30F, 0, 9u, 27u, (3u, 3u, 3u, 29u)
#define PPUTLIMPL_UINT_TRAIT_782u  DEC, 0x30E, 0, 9u, 27u, (2u, 17u, 23u)
#define PPUTLIMPL_UINT_TRAIT_781u  DEC, 0x30D, 0, 9u, 27u, (11u, 71u)
#define PPUTLIMPL_UINT_TRAIT_780u  DEC, 0x30C, 0, 9u, 27u, (2u, 2u, 3u, 5u, 13u)
#define PPUTLIMPL_UINT_TRAIT_779u  DEC, 0x30B, 0, 9u, 27u, (19u, 41u)
#define PPUTLIMPL_UINT_TRAIT_778u  DEC, 0x30A, 0, 9u, 27u, (2u, 389u)
#define PPUTLIMPL_UINT_TRAIT_777u  DEC, 0x309, 0, 9u, 27u, (3u, 7u, 37u)
#define PPUTLIMPL_UINT_TRAIT_776u  DEC, 0x308, 0, 9u, 27u, (2u, 2u, 2u, 97u)
#define PPUTLIMPL_UINT_TRAIT_775u  DEC, 0x307, 0, 9u, 27u, (5u, 5u, 31u)
#define PPUTLIMPL_UINT_TRAIT_774u  DEC, 0x306, 0, 9u, 27u, (2u, 3u, 3u, 43u)
#define PPUTLIMPL_UINT_TRAIT_773u  DEC, 0x305, 0, 9u, 27u, ()
#define PPUTLIMPL_UINT_TRAIT_772u  DEC, 0x304, 0, 9u, 27u, (2u, 2u, 193u)
#define PPUTLIMPL_UINT_TRAIT_771u  DEC, 0x303, 0, 9u, 27u, (3u, 257u)
#define PPUTLIMPL_UINT_TRAIT_770u  DEC, 0x302, 0, 9u, 27u, (2u, 5u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_769u  DEC, 0x301, 0, 9u, 27u, ()
#define PPUTLIMPL_UINT_TRAIT_768u  DEC, 0x300, 0, 9u, 27u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 3u)
#define PPUTLIMPL_UINT_TRAIT_767u  DEC, 0x2FF, 0, 9u, 27u, (13u, 59u)
#define PPUTLIMPL_UINT_TRAIT_766u  DEC, 0x2FE, 0, 9u, 27u, (2u, 383u)
#define PPUTLIMPL_UINT_TRAIT_765u  DEC, 0x2FD, 0, 9u, 27u, (3u, 3u, 5u, 17u)
#define PPUTLIMPL_UINT_TRAIT_764u  DEC, 0x2FC, 0, 9u, 27u, (2u, 2u, 191u)
#define PPUTLIMPL_UINT_TRAIT_763u  DEC, 0x2FB, 0, 9u, 27u, (7u, 109u)
#define PPUTLIMPL_UINT_TRAIT_762u  DEC, 0x2FA, 0, 9u, 27u, (2u, 3u, 127u)
#define PPUTLIMPL_UINT_TRAIT_761u  DEC, 0x2F9, 0, 9u, 27u, ()
#define PPUTLIMPL_UINT_TRAIT_760u  DEC, 0x2F8, 0, 9u, 27u, (2u, 2u, 2u, 5u, 19u)
#define PPUTLIMPL_UINT_TRAIT_759u  DEC, 0x2F7, 0, 9u, 27u, (3u, 11u, 23u)
#define PPUTLIMPL_UINT_TRAIT_758u  DEC, 0x2F6, 0, 9u, 27u, (2u, 379u)
#define PPUTLIMPL_UINT_TRAIT_757u  DEC, 0x2F5, 0, 9u, 27u, ()
#define PPUTLIMPL_UINT_TRAIT_756u  DEC, 0x2F4, 0, 9u, 27u, (2u, 2u, 3u, 3u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_755u  DEC, 0x2F3, 0, 9u, 27u, (5u, 151u)
#define PPUTLIMPL_UINT_TRAIT_754u  DEC, 0x2F2, 0, 9u, 27u, (2u, 13u, 29u)
#define PPUTLIMPL_UINT_TRAIT_753u  DEC, 0x2F1, 0, 9u, 27u, (3u, 251u)
#define PPUTLIMPL_UINT_TRAIT_752u  DEC, 0x2F0, 0, 9u, 27u, (2u, 2u, 2u, 2u, 47u)
#define PPUTLIMPL_UINT_TRAIT_751u  DEC, 0x2EF, 0, 9u, 27u, ()
#define PPUTLIMPL_UINT_TRAIT_750u  DEC, 0x2EE, 0, 9u, 27u, (2u, 3u, 5u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_749u  DEC, 0x2ED, 0, 9u, 27u, (7u, 107u)
#define PPUTLIMPL_UINT_TRAIT_748u  DEC, 0x2EC, 0, 9u, 27u, (2u, 2u, 11u, 17u)
#define PPUTLIMPL_UINT_TRAIT_747u  DEC, 0x2EB, 0, 9u, 27u, (3u, 3u, 83u)
#define PPUTLIMPL_UINT_TRAIT_746u  DEC, 0x2EA, 0, 9u, 27u, (2u, 373u)
#define PPUTLIMPL_UINT_TRAIT_745u  DEC, 0x2E9, 0, 9u, 27u, (5u, 149u)
#define PPUTLIMPL_UINT_TRAIT_744u  DEC, 0x2E8, 0, 9u, 27u, (2u, 2u, 2u, 3u, 31u)
#define PPUTLIMPL_UINT_TRAIT_743u  DEC, 0x2E7, 0, 9u, 27u, ()
#define PPUTLIMPL_UINT_TRAIT_742u  DEC, 0x2E6, 0, 9u, 27u, (2u, 7u, 53u)
#define PPUTLIMPL_UINT_TRAIT_741u  DEC, 0x2E5, 0, 9u, 27u, (3u, 13u, 19u)
#define PPUTLIMPL_UINT_TRAIT_740u  DEC, 0x2E4, 0, 9u, 27u, (2u, 2u, 5u, 37u)
#define PPUTLIMPL_UINT_TRAIT_739u  DEC, 0x2E3, 0, 9u, 27u, ()
#define PPUTLIMPL_UINT_TRAIT_738u  DEC, 0x2E2, 0, 9u, 27u, (2u, 3u, 3u, 41u)
#define PPUTLIMPL_UINT_TRAIT_737u  DEC, 0x2E1, 0, 9u, 27u, (11u, 67u)
#define PPUTLIMPL_UINT_TRAIT_736u  DEC, 0x2E0, 0, 9u, 27u, (2u, 2u, 2u, 2u, 2u, 23u)
#define PPUTLIMPL_UINT_TRAIT_735u  DEC, 0x2DF, 0, 9u, 27u, (3u, 5u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_734u  DEC, 0x2DE, 0, 9u, 27u, (2u, 367u)
#define PPUTLIMPL_UINT_TRAIT_733u  DEC, 0x2DD, 0, 9u, 27u, ()
#define PPUTLIMPL_UINT_TRAIT_732u  DEC, 0x2DC, 0, 9u, 27u, (2u, 2u, 3u, 61u)
#define PPUTLIMPL_UINT_TRAIT_731u  DEC, 0x2DB, 0, 9u, 27u, (17u, 43u)
#define PPUTLIMPL_UINT_TRAIT_730u  DEC, 0x2DA, 0, 9u, 27u, (2u, 5u, 73u)
#define PPUTLIMPL_UINT_TRAIT_729u  DEC, 0x2D9, 0, 9u, 27u, (3u, 3u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_728u  DEC, 0x2D8, 0, 9u, 26u, (2u, 2u, 2u, 7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_727u  DEC, 0x2D7, 0, 9u, 26u, ()
#define PPUTLIMPL_UINT_TRAIT_726u  DEC, 0x2D6, 0, 9u, 26u, (2u, 3u, 11u, 11u)
#define PPUTLIMPL_UINT_TRAIT_725u  DEC, 0x2D5, 0, 9u, 26u, (5u, 5u, 29u)
#define PPUTLIMPL_UINT_TRAIT_724u  DEC, 0x2D4, 0, 9u, 26u, (2u, 2u, 181u)
#define PPUTLIMPL_UINT_TRAIT_723u  DEC, 0x2D3, 0, 9u, 26u, (3u, 241u)
#define PPUTLIMPL_UINT_TRAIT_722u  DEC, 0x2D2, 0, 9u, 26u, (2u, 19u, 19u)
#define PPUTLIMPL_UINT_TRAIT_721u  DEC, 0x2D1, 0, 9u, 26u, (7u, 103u)
#define PPUTLIMPL_UINT_TRAIT_720u  DEC, 0x2D0, 0, 9u, 26u, (2u, 2u, 2u, 2u, 3u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_719u  DEC, 0x2CF, 0, 9u, 26u, ()
#define PPUTLIMPL_UINT_TRAIT_718u  DEC, 0x2CE, 0, 9u, 26u, (2u, 359u)
#define PPUTLIMPL_UINT_TRAIT_717u  DEC, 0x2CD, 0, 9u, 26u, (3u, 239u)
#define PPUTLIMPL_UINT_TRAIT_716u  DEC, 0x2CC, 0, 9u, 26u, (2u, 2u, 179u)
#define PPUTLIMPL_UINT_TRAIT_715u  DEC, 0x2CB, 0, 9u, 26u, (5u, 11u, 13u)
#define PPUTLIMPL_UINT_TRAIT_714u  DEC, 0x2CA, 0, 9u, 26u, (2u, 3u, 7u, 17u)
#define PPUTLIMPL_UINT_TRAIT_713u  DEC, 0x2C9, 0, 9u, 26u, (23u, 31u)
#define PPUTLIMPL_UINT_TRAIT_712u  DEC, 0x2C8, 0, 9u, 26u, (2u, 2u, 2u, 89u)
#define PPUTLIMPL_UINT_TRAIT_711u  DEC, 0x2C7, 0, 9u, 26u, (3u, 3u, 79u)
#define PPUTLIMPL_UINT_TRAIT_710u  DEC, 0x2C6, 0, 9u, 26u, (2u, 5u, 71u)
#define PPUTLIMPL_UINT_TRAIT_709u  DEC, 0x2C5, 0, 9u, 26u, ()
#define PPUTLIMPL_UINT_TRAIT_708u  DEC, 0x2C4, 0, 9u, 26u, (2u, 2u, 3u, 59u)
#define PPUTLIMPL_UINT_TRAIT_707u  DEC, 0x2C3, 0, 9u, 26u, (7u, 101u)
#define PPUTLIMPL_UINT_TRAIT_706u  DEC, 0x2C2, 0, 9u, 26u, (2u, 353u)
#define PPUTLIMPL_UINT_TRAIT_705u  DEC, 0x2C1, 0, 9u, 26u, (3u, 5u, 47u)
#define PPUTLIMPL_UINT_TRAIT_704u  DEC, 0x2C0, 0, 9u, 26u, (2u, 2u, 2u, 2u, 2u, 2u, 11u)
#define PPUTLIMPL_UINT_TRAIT_703u  DEC, 0x2BF, 0, 9u, 26u, (19u, 37u)
#define PPUTLIMPL_UINT_TRAIT_702u  DEC, 0x2BE, 0, 9u, 26u, (2u, 3u, 3u, 3u, 13u)
#define PPUTLIMPL_UINT_TRAIT_701u  DEC, 0x2BD, 0, 9u, 26u, ()
#define PPUTLIMPL_UINT_TRAIT_700u  DEC, 0x2BC, 0, 9u, 26u, (2u, 2u, 5u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_699u  DEC, 0x2BB, 0, 9u, 26u, (3u, 233u)
#define PPUTLIMPL_UINT_TRAIT_698u  DEC, 0x2BA, 0, 9u, 26u, (2u, 349u)
#define PPUTLIMPL_UINT_TRAIT_697u  DEC, 0x2B9, 0, 9u, 26u, (17u, 41u)
#define PPUTLIMPL_UINT_TRAIT_696u  DEC, 0x2B8, 0, 9u, 26u, (2u, 2u, 2u, 3u, 29u)
#define PPUTLIMPL_UINT_TRAIT_695u  DEC, 0x2B7, 0, 9u, 26u, (5u, 139u)
#define PPUTLIMPL_UINT_TRAIT_694u  DEC, 0x2B6, 0, 9u, 26u, (2u, 347u)
#define PPUTLIMPL_UINT_TRAIT_693u  DEC, 0x2B5, 0, 9u, 26u, (3u, 3u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_692u  DEC, 0x2B4, 0, 9u, 26u, (2u, 2u, 173u)
#define PPUTLIMPL_UINT_TRAIT_691u  DEC, 0x2B3, 0, 9u, 26u, ()
#define PPUTLIMPL_UINT_TRAIT_690u  DEC, 0x2B2, 0, 9u, 26u, (2u, 3u, 5u, 23u)
#define PPUTLIMPL_UINT_TRAIT_689u  DEC, 0x2B1, 0, 9u, 26u, (13u, 53u)
#define PPUTLIMPL_UINT_TRAIT_688u  DEC, 0x2B0, 0, 9u, 26u, (2u, 2u, 2u, 2u, 43u)
#define PPUTLIMPL_UINT_TRAIT_687u  DEC, 0x2AF, 0, 9u, 26u, (3u, 229u)
#define PPUTLIMPL_UINT_TRAIT_686u  DEC, 0x2AE, 0, 9u, 26u, (2u, 7u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_685u  DEC, 0x2AD, 0, 9u, 26u, (5u, 137u)
#define PPUTLIMPL_UINT_TRAIT_684u  DEC, 0x2AC, 0, 9u, 26u, (2u, 2u, 3u, 3u, 19u)
#define PPUTLIMPL_UINT_TRAIT_683u  DEC, 0x2AB, 0, 9u, 26u, ()
#define PPUTLIMPL_UINT_TRAIT_682u  DEC, 0x2AA, 0, 9u, 26u, (2u, 11u, 31u)
#define PPUTLIMPL_UINT_TRAIT_681u  DEC, 0x2A9, 0, 9u, 26u, (3u, 227u)
#define PPUTLIMPL_UINT_TRAIT_680u  DEC, 0x2A8, 0, 9u, 26u, (2u, 2u, 2u, 5u, 17u)
#define PPUTLIMPL_UINT_TRAIT_679u  DEC, 0x2A7, 0, 9u, 26u, (7u, 97u)
#define PPUTLIMPL_UINT_TRAIT_678u  DEC, 0x2A6, 0, 9u, 26u, (2u, 3u, 113u)
#define PPUTLIMPL_UINT_TRAIT_677u  DEC, 0x2A5, 0, 9u, 26u, ()
#define PPUTLIMPL_UINT_TRAIT_676u  DEC, 0x2A4, 0, 9u, 26u, (2u, 2u, 13u, 13u)
#define PPUTLIMPL_UINT_TRAIT_675u  DEC, 0x2A3, 0, 9u, 25u, (3u, 3u, 3u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_674u  DEC, 0x2A2, 0, 9u, 25u, (2u, 337u)
#define PPUTLIMPL_UINT_TRAIT_673u  DEC, 0x2A1, 0, 9u, 25u, ()
#define PPUTLIMPL_UINT_TRAIT_672u  DEC, 0x2A0, 0, 9u, 25u, (2u, 2u, 2u, 2u, 2u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_671u  DEC, 0x29F, 0, 9u, 25u, (11u, 61u)
#define PPUTLIMPL_UINT_TRAIT_670u  DEC, 0x29E, 0, 9u, 25u, (2u, 5u, 67u)
#define PPUTLIMPL_UINT_TRAIT_669u  DEC, 0x29D, 0, 9u, 25u, (3u, 223u)
#define PPUTLIMPL_UINT_TRAIT_668u  DEC, 0x29C, 0, 9u, 25u, (2u, 2u, 167u)
#define PPUTLIMPL_UINT_TRAIT_667u  DEC, 0x29B, 0, 9u, 25u, (23u, 29u)
#define PPUTLIMPL_UINT_TRAIT_666u  DEC, 0x29A, 0, 9u, 25u, (2u, 3u, 3u, 37u)
#define PPUTLIMPL_UINT_TRAIT_665u  DEC, 0x299, 0, 9u, 25u, (5u, 7u, 19u)
#define PPUTLIMPL_UINT_TRAIT_664u  DEC, 0x298, 0, 9u, 25u, (2u, 2u, 2u, 83u)
#define PPUTLIMPL_UINT_TRAIT_663u  DEC, 0x297, 0, 9u, 25u, (3u, 13u, 17u)
#define PPUTLIMPL_UINT_TRAIT_662u  DEC, 0x296, 0, 9u, 25u, (2u, 331u)
#define PPUTLIMPL_UINT_TRAIT_661u  DEC, 0x295, 0, 9u, 25u, ()
#define PPUTLIMPL_UINT_TRAIT_660u  DEC, 0x294, 0, 9u, 25u, (2u, 2u, 3u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_659u  DEC, 0x293, 0, 9u, 25u, ()
#define PPUTLIMPL_UINT_TRAIT_658u  DEC, 0x292, 0, 9u, 25u, (2u, 7u, 47u)
#define PPUTLIMPL_UINT_TRAIT_657u  DEC, 0x291, 0, 9u, 25u, (3u, 3u, 73u)
#define PPUTLIMPL_UINT_TRAIT_656u  DEC, 0x290, 0, 9u, 25u, (2u, 2u, 2u, 2u, 41u)
#define PPUTLIMPL_UINT_TRAIT_655u  DEC, 0x28F, 0, 9u, 25u, (5u, 131u)
#define PPUTLIMPL_UINT_TRAIT_654u  DEC, 0x28E, 0, 9u, 25u, (2u, 3u, 109u)
#define PPUTLIMPL_UINT_TRAIT_653u  DEC, 0x28D, 0, 9u, 25u, ()
#define PPUTLIMPL_UINT_TRAIT_652u  DEC, 0x28C, 0, 9u, 25u, (2u, 2u, 163u)
#define PPUTLIMPL_UINT_TRAIT_651u  DEC, 0x28B, 0, 9u, 25u, (3u, 7u, 31u)
#define PPUTLIMPL_UINT_TRAIT_650u  DEC, 0x28A, 0, 9u, 25u, (2u, 5u, 5u, 13u)
#define PPUTLIMPL_UINT_TRAIT_649u  DEC, 0x289, 0, 9u, 25u, (11u, 59u)
#define PPUTLIMPL_UINT_TRAIT_648u  DEC, 0x288, 0, 9u, 25u, (2u, 2u, 2u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_647u  DEC, 0x287, 0, 9u, 25u, ()
#define PPUTLIMPL_UINT_TRAIT_646u  DEC, 0x286, 0, 9u, 25u, (2u, 17u, 19u)
#define PPUTLIMPL_UINT_TRAIT_645u  DEC, 0x285, 0, 9u, 25u, (3u, 5u, 43u)
#define PPUTLIMPL_UINT_TRAIT_644u  DEC, 0x284, 0, 9u, 25u, (2u, 2u, 7u, 23u)
#define PPUTLIMPL_UINT_TRAIT_643u  DEC, 0x283, 0, 9u, 25u, ()
#define PPUTLIMPL_UINT_TRAIT_642u  DEC, 0x282, 0, 9u, 25u, (2u, 3u, 107u)
#define PPUTLIMPL_UINT_TRAIT_641u  DEC, 0x281, 0, 9u, 25u, ()
#define PPUTLIMPL_UINT_TRAIT_640u  DEC, 0x280, 0, 9u, 25u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 5u)
#define PPUTLIMPL_UINT_TRAIT_639u  DEC, 0x27F, 0, 9u, 25u, (3u, 3u, 71u)
#define PPUTLIMPL_UINT_TRAIT_638u  DEC, 0x27E, 0, 9u, 25u, (2u, 11u, 29u)
#define PPUTLIMPL_UINT_TRAIT_637u  DEC, 0x27D, 0, 9u, 25u, (7u, 7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_636u  DEC, 0x27C, 0, 9u, 25u, (2u, 2u, 3u, 53u)
#define PPUTLIMPL_UINT_TRAIT_635u  DEC, 0x27B, 0, 9u, 25u, (5u, 127u)
#define PPUTLIMPL_UINT_TRAIT_634u  DEC, 0x27A, 0, 9u, 25u, (2u, 317u)
#define PPUTLIMPL_UINT_TRAIT_633u  DEC, 0x279, 0, 9u, 25u, (3u, 211u)
#define PPUTLIMPL_UINT_TRAIT_632u  DEC, 0x278, 0, 9u, 25u, (2u, 2u, 2u, 79u)
#define PPUTLIMPL_UINT_TRAIT_631u  DEC, 0x277, 0, 9u, 25u, ()
#define PPUTLIMPL_UINT_TRAIT_630u  DEC, 0x276, 0, 9u, 25u, (2u, 3u, 3u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_629u  DEC, 0x275, 0, 9u, 25u, (17u, 37u)
#define PPUTLIMPL_UINT_TRAIT_628u  DEC, 0x274, 0, 9u, 25u, (2u, 2u, 157u)
#define PPUTLIMPL_UINT_TRAIT_627u  DEC, 0x273, 0, 9u, 25u, (3u, 11u, 19u)
#define PPUTLIMPL_UINT_TRAIT_626u  DEC, 0x272, 0, 9u, 25u, (2u, 313u)
#define PPUTLIMPL_UINT_TRAIT_625u  DEC, 0x271, 0, 9u, 25u, (5u, 5u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_624u  DEC, 0x270, 0, 9u, 24u, (2u, 2u, 2u, 2u, 3u, 13u)
#define PPUTLIMPL_UINT_TRAIT_623u  DEC, 0x26F, 0, 9u, 24u, (7u, 89u)
#define PPUTLIMPL_UINT_TRAIT_622u  DEC, 0x26E, 0, 9u, 24u, (2u, 311u)
#define PPUTLIMPL_UINT_TRAIT_621u  DEC, 0x26D, 0, 9u, 24u, (3u, 3u, 3u, 23u)
#define PPUTLIMPL_UINT_TRAIT_620u  DEC, 0x26C, 0, 9u, 24u, (2u, 2u, 5u, 31u)
#define PPUTLIMPL_UINT_TRAIT_619u  DEC, 0x26B, 0, 9u, 24u, ()
#define PPUTLIMPL_UINT_TRAIT_618u  DEC, 0x26A, 0, 9u, 24u, (2u, 3u, 103u)
#define PPUTLIMPL_UINT_TRAIT_617u  DEC, 0x269, 0, 9u, 24u, ()
#define PPUTLIMPL_UINT_TRAIT_616u  DEC, 0x268, 0, 9u, 24u, (2u, 2u, 2u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_615u  DEC, 0x267, 0, 9u, 24u, (3u, 5u, 41u)
#define PPUTLIMPL_UINT_TRAIT_614u  DEC, 0x266, 0, 9u, 24u, (2u, 307u)
#define PPUTLIMPL_UINT_TRAIT_613u  DEC, 0x265, 0, 9u, 24u, ()
#define PPUTLIMPL_UINT_TRAIT_612u  DEC, 0x264, 0, 9u, 24u, (2u, 2u, 3u, 3u, 17u)
#define PPUTLIMPL_UINT_TRAIT_611u  DEC, 0x263, 0, 9u, 24u, (13u, 47u)
#define PPUTLIMPL_UINT_TRAIT_610u  DEC, 0x262, 0, 9u, 24u, (2u, 5u, 61u)
#define PPUTLIMPL_UINT_TRAIT_609u  DEC, 0x261, 0, 9u, 24u, (3u, 7u, 29u)
#define PPUTLIMPL_UINT_TRAIT_608u  DEC, 0x260, 0, 9u, 24u, (2u, 2u, 2u, 2u, 2u, 19u)
#define PPUTLIMPL_UINT_TRAIT_607u  DEC, 0x25F, 0, 9u, 24u, ()
#define PPUTLIMPL_UINT_TRAIT_606u  DEC, 0x25E, 0, 9u, 24u, (2u, 3u, 101u)
#define PPUTLIMPL_UINT_TRAIT_605u  DEC, 0x25D, 0, 9u, 24u, (5u, 11u, 11u)
#define PPUTLIMPL_UINT_TRAIT_604u  DEC, 0x25C, 0, 9u, 24u, (2u, 2u, 151u)
#define PPUTLIMPL_UINT_TRAIT_603u  DEC, 0x25B, 0, 9u, 24u, (3u, 3u, 67u)
#define PPUTLIMPL_UINT_TRAIT_602u  DEC, 0x25A, 0, 9u, 24u, (2u, 7u, 43u)
#define PPUTLIMPL_UINT_TRAIT_601u  DEC, 0x259, 0, 9u, 24u, ()
#define PPUTLIMPL_UINT_TRAIT_600u  DEC, 0x258, 0, 9u, 24u, (2u, 2u, 2u, 3u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_599u  DEC, 0x257, 0, 9u, 24u, ()
#define PPUTLIMPL_UINT_TRAIT_598u  DEC, 0x256, 0, 9u, 24u, (2u, 13u, 23u)
#define PPUTLIMPL_UINT_TRAIT_597u  DEC, 0x255, 0, 9u, 24u, (3u, 199u)
#define PPUTLIMPL_UINT_TRAIT_596u  DEC, 0x254, 0, 9u, 24u, (2u, 2u, 149u)
#define PPUTLIMPL_UINT_TRAIT_595u  DEC, 0x253, 0, 9u, 24u, (5u, 7u, 17u)
#define PPUTLIMPL_UINT_TRAIT_594u  DEC, 0x252, 0, 9u, 24u, (2u, 3u, 3u, 3u, 11u)
#define PPUTLIMPL_UINT_TRAIT_593u  DEC, 0x251, 0, 9u, 24u, ()
#define PPUTLIMPL_UINT_TRAIT_592u  DEC, 0x250, 0, 9u, 24u, (2u, 2u, 2u, 2u, 37u)
#define PPUTLIMPL_UINT_TRAIT_591u  DEC, 0x24F, 0, 9u, 24u, (3u, 197u)
#define PPUTLIMPL_UINT_TRAIT_590u  DEC, 0x24E, 0, 9u, 24u, (2u, 5u, 59u)
#define PPUTLIMPL_UINT_TRAIT_589u  DEC, 0x24D, 0, 9u, 24u, (19u, 31u)
#define PPUTLIMPL_UINT_TRAIT_588u  DEC, 0x24C, 0, 9u, 24u, (2u, 2u, 3u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_587u  DEC, 0x24B, 0, 9u, 24u, ()
#define PPUTLIMPL_UINT_TRAIT_586u  DEC, 0x24A, 0, 9u, 24u, (2u, 293u)
#define PPUTLIMPL_UINT_TRAIT_585u  DEC, 0x249, 0, 9u, 24u, (3u, 3u, 5u, 13u)
#define PPUTLIMPL_UINT_TRAIT_584u  DEC, 0x248, 0, 9u, 24u, (2u, 2u, 2u, 73u)
#define PPUTLIMPL_UINT_TRAIT_583u  DEC, 0x247, 0, 9u, 24u, (11u, 53u)
#define PPUTLIMPL_UINT_TRAIT_582u  DEC, 0x246, 0, 9u, 24u, (2u, 3u, 97u)
#define PPUTLIMPL_UINT_TRAIT_581u  DEC, 0x245, 0, 9u, 24u, (7u, 83u)
#define PPUTLIMPL_UINT_TRAIT_580u  DEC, 0x244, 0, 9u, 24u, (2u, 2u, 5u, 29u)
#define PPUTLIMPL_UINT_TRAIT_579u  DEC, 0x243, 0, 9u, 24u, (3u, 193u)
#define PPUTLIMPL_UINT_TRAIT_578u  DEC, 0x242, 0, 9u, 24u, (2u, 17u, 17u)
#define PPUTLIMPL_UINT_TRAIT_577u  DEC, 0x241, 0, 9u, 24u, ()
#define PPUTLIMPL_UINT_TRAIT_576u  DEC, 0x240, 0, 9u, 24u, (2u, 2u, 2u, 2u, 2u, 2u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_575u  DEC, 0x23F, 0, 9u, 23u, (5u, 5u, 23u)
#define PPUTLIMPL_UINT_TRAIT_574u  DEC, 0x23E, 0, 9u, 23u, (2u, 7u, 41u)
#define PPUTLIMPL_UINT_TRAIT_573u  DEC, 0x23D, 0, 9u, 23u, (3u, 191u)
#define PPUTLIMPL_UINT_TRAIT_572u  DEC, 0x23C, 0, 9u, 23u, (2u, 2u, 11u, 13u)
#define PPUTLIMPL_UINT_TRAIT_571u  DEC, 0x23B, 0, 9u, 23u, ()
#define PPUTLIMPL_UINT_TRAIT_570u  DEC, 0x23A, 0, 9u, 23u, (2u, 3u, 5u, 19u)
#define PPUTLIMPL_UINT_TRAIT_569u  DEC, 0x239, 0, 9u, 23u, ()
#define PPUTLIMPL_UINT_TRAIT_568u  DEC, 0x238, 0, 9u, 23u, (2u, 2u, 2u, 71u)
#define PPUTLIMPL_UINT_TRAIT_567u  DEC, 0x237, 0, 9u, 23u, (3u, 3u, 3u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_566u  DEC, 0x236, 0, 9u, 23u, (2u, 283u)
#define PPUTLIMPL_UINT_TRAIT_565u  DEC, 0x235, 0, 9u, 23u, (5u, 113u)
#define PPUTLIMPL_UINT_TRAIT_564u  DEC, 0x234, 0, 9u, 23u, (2u, 2u, 3u, 47u)
#define PPUTLIMPL_UINT_TRAIT_563u  DEC, 0x233, 0, 9u, 23u, ()
#define PPUTLIMPL_UINT_TRAIT_562u  DEC, 0x232, 0, 9u, 23u, (2u, 281u)
#define PPUTLIMPL_UINT_TRAIT_561u  DEC, 0x231, 0, 9u, 23u, (3u, 11u, 17u)
#define PPUTLIMPL_UINT_TRAIT_560u  DEC, 0x230, 0, 9u, 23u, (2u, 2u, 2u, 2u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_559u  DEC, 0x22F, 0, 9u, 23u, (13u, 43u)
#define PPUTLIMPL_UINT_TRAIT_558u  DEC, 0x22E, 0, 9u, 23u, (2u, 3u, 3u, 31u)
#define PPUTLIMPL_UINT_TRAIT_557u  DEC, 0x22D, 0, 9u, 23u, ()
#define PPUTLIMPL_UINT_TRAIT_556u  DEC, 0x22C, 0, 9u, 23u, (2u, 2u, 139u)
#define PPUTLIMPL_UINT_TRAIT_555u  DEC, 0x22B, 0, 9u, 23u, (3u, 5u, 37u)
#define PPUTLIMPL_UINT_TRAIT_554u  DEC, 0x22A, 0, 9u, 23u, (2u, 277u)
#define PPUTLIMPL_UINT_TRAIT_553u  DEC, 0x229, 0, 9u, 23u, (7u, 79u)
#define PPUTLIMPL_UINT_TRAIT_552u  DEC, 0x228, 0, 9u, 23u, (2u, 2u, 2u, 3u, 23u)
#define PPUTLIMPL_UINT_TRAIT_551u  DEC, 0x227, 0, 9u, 23u, (19u, 29u)
#define PPUTLIMPL_UINT_TRAIT_550u  DEC, 0x226, 0, 9u, 23u, (2u, 5u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_549u  DEC, 0x225, 0, 9u, 23u, (3u, 3u, 61u)
#define PPUTLIMPL_UINT_TRAIT_548u  DEC, 0x224, 0, 9u, 23u, (2u, 2u, 137u)
#define PPUTLIMPL_UINT_TRAIT_547u  DEC, 0x223, 0, 9u, 23u, ()
#define PPUTLIMPL_UINT_TRAIT_546u  DEC, 0x222, 0, 9u, 23u, (2u, 3u, 7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_545u  DEC, 0x221, 0, 9u, 23u, (5u, 109u)
#define PPUTLIMPL_UINT_TRAIT_544u  DEC, 0x220, 0, 9u, 23u, (2u, 2u, 2u, 2u, 2u, 17u)
#define PPUTLIMPL_UINT_TRAIT_543u  DEC, 0x21F, 0, 9u, 23u, (3u, 181u)
#define PPUTLIMPL_UINT_TRAIT_542u  DEC, 0x21E, 0, 9u, 23u, (2u, 271u)
#define PPUTLIMPL_UINT_TRAIT_541u  DEC, 0x21D, 0, 9u, 23u, ()
#define PPUTLIMPL_UINT_TRAIT_540u  DEC, 0x21C, 0, 9u, 23u, (2u, 2u, 3u, 3u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_539u  DEC, 0x21B, 0, 9u, 23u, (7u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_538u  DEC, 0x21A, 0, 9u, 23u, (2u, 269u)
#define PPUTLIMPL_UINT_TRAIT_537u  DEC, 0x219, 0, 9u, 23u, (3u, 179u)
#define PPUTLIMPL_UINT_TRAIT_536u  DEC, 0x218, 0, 9u, 23u, (2u, 2u, 2u, 67u)
#define PPUTLIMPL_UINT_TRAIT_535u  DEC, 0x217, 0, 9u, 23u, (5u, 107u)
#define PPUTLIMPL_UINT_TRAIT_534u  DEC, 0x216, 0, 9u, 23u, (2u, 3u, 89u)
#define PPUTLIMPL_UINT_TRAIT_533u  DEC, 0x215, 0, 9u, 23u, (13u, 41u)
#define PPUTLIMPL_UINT_TRAIT_532u  DEC, 0x214, 0, 9u, 23u, (2u, 2u, 7u, 19u)
#define PPUTLIMPL_UINT_TRAIT_531u  DEC, 0x213, 0, 9u, 23u, (3u, 3u, 59u)
#define PPUTLIMPL_UINT_TRAIT_530u  DEC, 0x212, 0, 9u, 23u, (2u, 5u, 53u)
#define PPUTLIMPL_UINT_TRAIT_529u  DEC, 0x211, 0, 9u, 23u, (23u, 23u)
#define PPUTLIMPL_UINT_TRAIT_528u  DEC, 0x210, 0, 9u, 22u, (2u, 2u, 2u, 2u, 3u, 11u)
#define PPUTLIMPL_UINT_TRAIT_527u  DEC, 0x20F, 0, 9u, 22u, (17u, 31u)
#define PPUTLIMPL_UINT_TRAIT_526u  DEC, 0x20E, 0, 9u, 22u, (2u, 263u)
#define PPUTLIMPL_UINT_TRAIT_525u  DEC, 0x20D, 0, 9u, 22u, (3u, 5u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_524u  DEC, 0x20C, 0, 9u, 22u, (2u, 2u, 131u)
#define PPUTLIMPL_UINT_TRAIT_523u  DEC, 0x20B, 0, 9u, 22u, ()
#define PPUTLIMPL_UINT_TRAIT_522u  DEC, 0x20A, 0, 9u, 22u, (2u, 3u, 3u, 29u)
#define PPUTLIMPL_UINT_TRAIT_521u  DEC, 0x209, 0, 9u, 22u, ()
#define PPUTLIMPL_UINT_TRAIT_520u  DEC, 0x208, 0, 9u, 22u, (2u, 2u, 2u, 5u, 13u)
#define PPUTLIMPL_UINT_TRAIT_519u  DEC, 0x207, 0, 9u, 22u, (3u, 173u)
#define PPUTLIMPL_UINT_TRAIT_518u  DEC, 0x206, 0, 9u, 22u, (2u, 7u, 37u)
#define PPUTLIMPL_UINT_TRAIT_517u  DEC, 0x205, 0, 9u, 22u, (11u, 47u)
#define PPUTLIMPL_UINT_TRAIT_516u  DEC, 0x204, 0, 9u, 22u, (2u, 2u, 3u, 43u)
#define PPUTLIMPL_UINT_TRAIT_515u  DEC, 0x203, 0, 9u, 22u, (5u, 103u)
#define PPUTLIMPL_UINT_TRAIT_514u  DEC, 0x202, 0, 9u, 22u, (2u, 257u)
#define PPUTLIMPL_UINT_TRAIT_513u  DEC, 0x201, 0, 9u, 22u, (3u, 3u, 3u, 19u)
#define PPUTLIMPL_UINT_TRAIT_512u  DEC, 0x200, 0, 9u, 22u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u)
#define PPUTLIMPL_UINT_TRAIT_511u  DEC, 0x1FF, 0, 8u, 22u, (7u, 73u)
#define PPUTLIMPL_UINT_TRAIT_510u  DEC, 0x1FE, 0, 8u, 22u, (2u, 3u, 5u, 17u)
#define PPUTLIMPL_UINT_TRAIT_509u  DEC, 0x1FD, 0, 8u, 22u, ()
#define PPUTLIMPL_UINT_TRAIT_508u  DEC, 0x1FC, 0, 8u, 22u, (2u, 2u, 127u)
#define PPUTLIMPL_UINT_TRAIT_507u  DEC, 0x1FB, 0, 8u, 22u, (3u, 13u, 13u)
#define PPUTLIMPL_UINT_TRAIT_506u  DEC, 0x1FA, 0, 8u, 22u, (2u, 11u, 23u)
#define PPUTLIMPL_UINT_TRAIT_505u  DEC, 0x1F9, 0, 8u, 22u, (5u, 101u)
#define PPUTLIMPL_UINT_TRAIT_504u  DEC, 0x1F8, 0, 8u, 22u, (2u, 2u, 2u, 3u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_503u  DEC, 0x1F7, 0, 8u, 22u, ()
#define PPUTLIMPL_UINT_TRAIT_502u  DEC, 0x1F6, 0, 8u, 22u, (2u, 251u)
#define PPUTLIMPL_UINT_TRAIT_501u  DEC, 0x1F5, 0, 8u, 22u, (3u, 167u)
#define PPUTLIMPL_UINT_TRAIT_500u  DEC, 0x1F4, 0, 8u, 22u, (2u, 2u, 5u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_499u  DEC, 0x1F3, 0, 8u, 22u, ()
#define PPUTLIMPL_UINT_TRAIT_498u  DEC, 0x1F2, 0, 8u, 22u, (2u, 3u, 83u)
#define PPUTLIMPL_UINT_TRAIT_497u  DEC, 0x1F1, 0, 8u, 22u, (7u, 71u)
#define PPUTLIMPL_UINT_TRAIT_496u  DEC, 0x1F0, 0, 8u, 22u, (2u, 2u, 2u, 2u, 31u)
#define PPUTLIMPL_UINT_TRAIT_495u  DEC, 0x1EF, 0, 8u, 22u, (3u, 3u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_494u  DEC, 0x1EE, 0, 8u, 22u, (2u, 13u, 19u)
#define PPUTLIMPL_UINT_TRAIT_493u  DEC, 0x1ED, 0, 8u, 22u, (17u, 29u)
#define PPUTLIMPL_UINT_TRAIT_492u  DEC, 0x1EC, 0, 8u, 22u, (2u, 2u, 3u, 41u)
#define PPUTLIMPL_UINT_TRAIT_491u  DEC, 0x1EB, 0, 8u, 22u, ()
#define PPUTLIMPL_UINT_TRAIT_490u  DEC, 0x1EA, 0, 8u, 22u, (2u, 5u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_489u  DEC, 0x1E9, 0, 8u, 22u, (3u, 163u)
#define PPUTLIMPL_UINT_TRAIT_488u  DEC, 0x1E8, 0, 8u, 22u, (2u, 2u, 2u, 61u)
#define PPUTLIMPL_UINT_TRAIT_487u  DEC, 0x1E7, 0, 8u, 22u, ()
#define PPUTLIMPL_UINT_TRAIT_486u  DEC, 0x1E6, 0, 8u, 22u, (2u, 3u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_485u  DEC, 0x1E5, 0, 8u, 22u, (5u, 97u)
#define PPUTLIMPL_UINT_TRAIT_484u  DEC, 0x1E4, 0, 8u, 22u, (2u, 2u, 11u, 11u)
#define PPUTLIMPL_UINT_TRAIT_483u  DEC, 0x1E3, 0, 8u, 21u, (3u, 7u, 23u)
#define PPUTLIMPL_UINT_TRAIT_482u  DEC, 0x1E2, 0, 8u, 21u, (2u, 241u)
#define PPUTLIMPL_UINT_TRAIT_481u  DEC, 0x1E1, 0, 8u, 21u, (13u, 37u)
#define PPUTLIMPL_UINT_TRAIT_480u  DEC, 0x1E0, 0, 8u, 21u, (2u, 2u, 2u, 2u, 2u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_479u  DEC, 0x1DF, 0, 8u, 21u, ()
#define PPUTLIMPL_UINT_TRAIT_478u  DEC, 0x1DE, 0, 8u, 21u, (2u, 239u)
#define PPUTLIMPL_UINT_TRAIT_477u  DEC, 0x1DD, 0, 8u, 21u, (3u, 3u, 53u)
#define PPUTLIMPL_UINT_TRAIT_476u  DEC, 0x1DC, 0, 8u, 21u, (2u, 2u, 7u, 17u)
#define PPUTLIMPL_UINT_TRAIT_475u  DEC, 0x1DB, 0, 8u, 21u, (5u, 5u, 19u)
#define PPUTLIMPL_UINT_TRAIT_474u  DEC, 0x1DA, 0, 8u, 21u, (2u, 3u, 79u)
#define PPUTLIMPL_UINT_TRAIT_473u  DEC, 0x1D9, 0, 8u, 21u, (11u, 43u)
#define PPUTLIMPL_UINT_TRAIT_472u  DEC, 0x1D8, 0, 8u, 21u, (2u, 2u, 2u, 59u)
#define PPUTLIMPL_UINT_TRAIT_471u  DEC, 0x1D7, 0, 8u, 21u, (3u, 157u)
#define PPUTLIMPL_UINT_TRAIT_470u  DEC, 0x1D6, 0, 8u, 21u, (2u, 5u, 47u)
#define PPUTLIMPL_UINT_TRAIT_469u  DEC, 0x1D5, 0, 8u, 21u, (7u, 67u)
#define PPUTLIMPL_UINT_TRAIT_468u  DEC, 0x1D4, 0, 8u, 21u, (2u, 2u, 3u, 3u, 13u)
#define PPUTLIMPL_UINT_TRAIT_467u  DEC, 0x1D3, 0, 8u, 21u, ()
#define PPUTLIMPL_UINT_TRAIT_466u  DEC, 0x1D2, 0, 8u, 21u, (2u, 233u)
#define PPUTLIMPL_UINT_TRAIT_465u  DEC, 0x1D1, 0, 8u, 21u, (3u, 5u, 31u)
#define PPUTLIMPL_UINT_TRAIT_464u  DEC, 0x1D0, 0, 8u, 21u, (2u, 2u, 2u, 2u, 29u)
#define PPUTLIMPL_UINT_TRAIT_463u  DEC, 0x1CF, 0, 8u, 21u, ()
#define PPUTLIMPL_UINT_TRAIT_462u  DEC, 0x1CE, 0, 8u, 21u, (2u, 3u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_461u  DEC, 0x1CD, 0, 8u, 21u, ()
#define PPUTLIMPL_UINT_TRAIT_460u  DEC, 0x1CC, 0, 8u, 21u, (2u, 2u, 5u, 23u)
#define PPUTLIMPL_UINT_TRAIT_459u  DEC, 0x1CB, 0, 8u, 21u, (3u, 3u, 3u, 17u)
#define PPUTLIMPL_UINT_TRAIT_458u  DEC, 0x1CA, 0, 8u, 21u, (2u, 229u)
#define PPUTLIMPL_UINT_TRAIT_457u  DEC, 0x1C9, 0, 8u, 21u, ()
#define PPUTLIMPL_UINT_TRAIT_456u  DEC, 0x1C8, 0, 8u, 21u, (2u, 2u, 2u, 3u, 19u)
#define PPUTLIMPL_UINT_TRAIT_455u  DEC, 0x1C7, 0, 8u, 21u, (5u, 7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_454u  DEC, 0x1C6, 0, 8u, 21u, (2u, 227u)
#define PPUTLIMPL_UINT_TRAIT_453u  DEC, 0x1C5, 0, 8u, 21u, (3u, 151u)
#define PPUTLIMPL_UINT_TRAIT_452u  DEC, 0x1C4, 0, 8u, 21u, (2u, 2u, 113u)
#define PPUTLIMPL_UINT_TRAIT_451u  DEC, 0x1C3, 0, 8u, 21u, (11u, 41u)
#define PPUTLIMPL_UINT_TRAIT_450u  DEC, 0x1C2, 0, 8u, 21u, (2u, 3u, 3u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_449u  DEC, 0x1C1, 0, 8u, 21u, ()
#define PPUTLIMPL_UINT_TRAIT_448u  DEC, 0x1C0, 0, 8u, 21u, (2u, 2u, 2u, 2u, 2u, 2u, 7u)
#define PPUTLIMPL_UINT_TRAIT_447u  DEC, 0x1BF, 0, 8u, 21u, (3u, 149u)
#define PPUTLIMPL_UINT_TRAIT_446u  DEC, 0x1BE, 0, 8u, 21u, (2u, 223u)
#define PPUTLIMPL_UINT_TRAIT_445u  DEC, 0x1BD, 0, 8u, 21u, (5u, 89u)
#define PPUTLIMPL_UINT_TRAIT_444u  DEC, 0x1BC, 0, 8u, 21u, (2u, 2u, 3u, 37u)
#define PPUTLIMPL_UINT_TRAIT_443u  DEC, 0x1BB, 0, 8u, 21u, ()
#define PPUTLIMPL_UINT_TRAIT_442u  DEC, 0x1BA, 0, 8u, 21u, (2u, 13u, 17u)
#define PPUTLIMPL_UINT_TRAIT_441u  DEC, 0x1B9, 0, 8u, 21u, (3u, 3u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_440u  DEC, 0x1B8, 0, 8u, 20u, (2u, 2u, 2u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_439u  DEC, 0x1B7, 0, 8u, 20u, ()
#define PPUTLIMPL_UINT_TRAIT_438u  DEC, 0x1B6, 0, 8u, 20u, (2u, 3u, 73u)
#define PPUTLIMPL_UINT_TRAIT_437u  DEC, 0x1B5, 0, 8u, 20u, (19u, 23u)
#define PPUTLIMPL_UINT_TRAIT_436u  DEC, 0x1B4, 0, 8u, 20u, (2u, 2u, 109u)
#define PPUTLIMPL_UINT_TRAIT_435u  DEC, 0x1B3, 0, 8u, 20u, (3u, 5u, 29u)
#define PPUTLIMPL_UINT_TRAIT_434u  DEC, 0x1B2, 0, 8u, 20u, (2u, 7u, 31u)
#define PPUTLIMPL_UINT_TRAIT_433u  DEC, 0x1B1, 0, 8u, 20u, ()
#define PPUTLIMPL_UINT_TRAIT_432u  DEC, 0x1B0, 0, 8u, 20u, (2u, 2u, 2u, 2u, 3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_431u  DEC, 0x1AF, 0, 8u, 20u, ()
#define PPUTLIMPL_UINT_TRAIT_430u  DEC, 0x1AE, 0, 8u, 20u, (2u, 5u, 43u)
#define PPUTLIMPL_UINT_TRAIT_429u  DEC, 0x1AD, 0, 8u, 20u, (3u, 11u, 13u)
#define PPUTLIMPL_UINT_TRAIT_428u  DEC, 0x1AC, 0, 8u, 20u, (2u, 2u, 107u)
#define PPUTLIMPL_UINT_TRAIT_427u  DEC, 0x1AB, 0, 8u, 20u, (7u, 61u)
#define PPUTLIMPL_UINT_TRAIT_426u  DEC, 0x1AA, 0, 8u, 20u, (2u, 3u, 71u)
#define PPUTLIMPL_UINT_TRAIT_425u  DEC, 0x1A9, 0, 8u, 20u, (5u, 5u, 17u)
#define PPUTLIMPL_UINT_TRAIT_424u  DEC, 0x1A8, 0, 8u, 20u, (2u, 2u, 2u, 53u)
#define PPUTLIMPL_UINT_TRAIT_423u  DEC, 0x1A7, 0, 8u, 20u, (3u, 3u, 47u)
#define PPUTLIMPL_UINT_TRAIT_422u  DEC, 0x1A6, 0, 8u, 20u, (2u, 211u)
#define PPUTLIMPL_UINT_TRAIT_421u  DEC, 0x1A5, 0, 8u, 20u, ()
#define PPUTLIMPL_UINT_TRAIT_420u  DEC, 0x1A4, 0, 8u, 20u, (2u, 2u, 3u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_419u  DEC, 0x1A3, 0, 8u, 20u, ()
#define PPUTLIMPL_UINT_TRAIT_418u  DEC, 0x1A2, 0, 8u, 20u, (2u, 11u, 19u)
#define PPUTLIMPL_UINT_TRAIT_417u  DEC, 0x1A1, 0, 8u, 20u, (3u, 139u)
#define PPUTLIMPL_UINT_TRAIT_416u  DEC, 0x1A0, 0, 8u, 20u, (2u, 2u, 2u, 2u, 2u, 13u)
#define PPUTLIMPL_UINT_TRAIT_415u  DEC, 0x19F, 0, 8u, 20u, (5u, 83u)
#define PPUTLIMPL_UINT_TRAIT_414u  DEC, 0x19E, 0, 8u, 20u, (2u, 3u, 3u, 23u)
#define PPUTLIMPL_UINT_TRAIT_413u  DEC, 0x19D, 0, 8u, 20u, (7u, 59u)
#define PPUTLIMPL_UINT_TRAIT_412u  DEC, 0x19C, 0, 8u, 20u, (2u, 2u, 103u)
#define PPUTLIMPL_UINT_TRAIT_411u  DEC, 0x19B, 0, 8u, 20u, (3u, 137u)
#define PPUTLIMPL_UINT_TRAIT_410u  DEC, 0x19A, 0, 8u, 20u, (2u, 5u, 41u)
#define PPUTLIMPL_UINT_TRAIT_409u  DEC, 0x199, 0, 8u, 20u, ()
#define PPUTLIMPL_UINT_TRAIT_408u  DEC, 0x198, 0, 8u, 20u, (2u, 2u, 2u, 3u, 17u)
#define PPUTLIMPL_UINT_TRAIT_407u  DEC, 0x197, 0, 8u, 20u, (11u, 37u)
#define PPUTLIMPL_UINT_TRAIT_406u  DEC, 0x196, 0, 8u, 20u, (2u, 7u, 29u)
#define PPUTLIMPL_UINT_TRAIT_405u  DEC, 0x195, 0, 8u, 20u, (3u, 3u, 3u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_404u  DEC, 0x194, 0, 8u, 20u, (2u, 2u, 101u)
#define PPUTLIMPL_UINT_TRAIT_403u  DEC, 0x193, 0, 8u, 20u, (13u, 31u)
#define PPUTLIMPL_UINT_TRAIT_402u  DEC, 0x192, 0, 8u, 20u, (2u, 3u, 67u)
#define PPUTLIMPL_UINT_TRAIT_401u  DEC, 0x191, 0, 8u, 20u, ()
#define PPUTLIMPL_UINT_TRAIT_400u  DEC, 0x190, 0, 8u, 20u, (2u, 2u, 2u, 2u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_399u  DEC, 0x18F, 0, 8u, 19u, (3u, 7u, 19u)
#define PPUTLIMPL_UINT_TRAIT_398u  DEC, 0x18E, 0, 8u, 19u, (2u, 199u)
#define PPUTLIMPL_UINT_TRAIT_397u  DEC, 0x18D, 0, 8u, 19u, ()
#define PPUTLIMPL_UINT_TRAIT_396u  DEC, 0x18C, 0, 8u, 19u, (2u, 2u, 3u, 3u, 11u)
#define PPUTLIMPL_UINT_TRAIT_395u  DEC, 0x18B, 0, 8u, 19u, (5u, 79u)
#define PPUTLIMPL_UINT_TRAIT_394u  DEC, 0x18A, 0, 8u, 19u, (2u, 197u)
#define PPUTLIMPL_UINT_TRAIT_393u  DEC, 0x189, 0, 8u, 19u, (3u, 131u)
#define PPUTLIMPL_UINT_TRAIT_392u  DEC, 0x188, 0, 8u, 19u, (2u, 2u, 2u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_391u  DEC, 0x187, 0, 8u, 19u, (17u, 23u)
#define PPUTLIMPL_UINT_TRAIT_390u  DEC, 0x186, 0, 8u, 19u, (2u, 3u, 5u, 13u)
#define PPUTLIMPL_UINT_TRAIT_389u  DEC, 0x185, 0, 8u, 19u, ()
#define PPUTLIMPL_UINT_TRAIT_388u  DEC, 0x184, 0, 8u, 19u, (2u, 2u, 97u)
#define PPUTLIMPL_UINT_TRAIT_387u  DEC, 0x183, 0, 8u, 19u, (3u, 3u, 43u)
#define PPUTLIMPL_UINT_TRAIT_386u  DEC, 0x182, 0, 8u, 19u, (2u, 193u)
#define PPUTLIMPL_UINT_TRAIT_385u  DEC, 0x181, 0, 8u, 19u, (5u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_384u  DEC, 0x180, 0, 8u, 19u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 3u)
#define PPUTLIMPL_UINT_TRAIT_383u  DEC, 0x17F, 0, 8u, 19u, ()
#define PPUTLIMPL_UINT_TRAIT_382u  DEC, 0x17E, 0, 8u, 19u, (2u, 191u)
#define PPUTLIMPL_UINT_TRAIT_381u  DEC, 0x17D, 0, 8u, 19u, (3u, 127u)
#define PPUTLIMPL_UINT_TRAIT_380u  DEC, 0x17C, 0, 8u, 19u, (2u, 2u, 5u, 19u)
#define PPUTLIMPL_UINT_TRAIT_379u  DEC, 0x17B, 0, 8u, 19u, ()
#define PPUTLIMPL_UINT_TRAIT_378u  DEC, 0x17A, 0, 8u, 19u, (2u, 3u, 3u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_377u  DEC, 0x179, 0, 8u, 19u, (13u, 29u)
#define PPUTLIMPL_UINT_TRAIT_376u  DEC, 0x178, 0, 8u, 19u, (2u, 2u, 2u, 47u)
#define PPUTLIMPL_UINT_TRAIT_375u  DEC, 0x177, 0, 8u, 19u, (3u, 5u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_374u  DEC, 0x176, 0, 8u, 19u, (2u, 11u, 17u)
#define PPUTLIMPL_UINT_TRAIT_373u  DEC, 0x175, 0, 8u, 19u, ()
#define PPUTLIMPL_UINT_TRAIT_372u  DEC, 0x174, 0, 8u, 19u, (2u, 2u, 3u, 31u)
#define PPUTLIMPL_UINT_TRAIT_371u  DEC, 0x173, 0, 8u, 19u, (7u, 53u)
#define PPUTLIMPL_UINT_TRAIT_370u  DEC, 0x172, 0, 8u, 19u, (2u, 5u, 37u)
#define PPUTLIMPL_UINT_TRAIT_369u  DEC, 0x171, 0, 8u, 19u, (3u, 3u, 41u)
#define PPUTLIMPL_UINT_TRAIT_368u  DEC, 0x170, 0, 8u, 19u, (2u, 2u, 2u, 2u, 23u)
#define PPUTLIMPL_UINT_TRAIT_367u  DEC, 0x16F, 0, 8u, 19u, ()
#define PPUTLIMPL_UINT_TRAIT_366u  DEC, 0x16E, 0, 8u, 19u, (2u, 3u, 61u)
#define PPUTLIMPL_UINT_TRAIT_365u  DEC, 0x16D, 0, 8u, 19u, (5u, 73u)
#define PPUTLIMPL_UINT_TRAIT_364u  DEC, 0x16C, 0, 8u, 19u, (2u, 2u, 7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_363u  DEC, 0x16B, 0, 8u, 19u, (3u, 11u, 11u)
#define PPUTLIMPL_UINT_TRAIT_362u  DEC, 0x16A, 0, 8u, 19u, (2u, 181u)
#define PPUTLIMPL_UINT_TRAIT_361u  DEC, 0x169, 0, 8u, 19u, (19u, 19u)
#define PPUTLIMPL_UINT_TRAIT_360u  DEC, 0x168, 0, 8u, 18u, (2u, 2u, 2u, 3u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_359u  DEC, 0x167, 0, 8u, 18u, ()
#define PPUTLIMPL_UINT_TRAIT_358u  DEC, 0x166, 0, 8u, 18u, (2u, 179u)
#define PPUTLIMPL_UINT_TRAIT_357u  DEC, 0x165, 0, 8u, 18u, (3u, 7u, 17u)
#define PPUTLIMPL_UINT_TRAIT_356u  DEC, 0x164, 0, 8u, 18u, (2u, 2u, 89u)
#define PPUTLIMPL_UINT_TRAIT_355u  DEC, 0x163, 0, 8u, 18u, (5u, 71u)
#define PPUTLIMPL_UINT_TRAIT_354u  DEC, 0x162, 0, 8u, 18u, (2u, 3u, 59u)
#define PPUTLIMPL_UINT_TRAIT_353u  DEC, 0x161, 0, 8u, 18u, ()
#define PPUTLIMPL_UINT_TRAIT_352u  DEC, 0x160, 0, 8u, 18u, (2u, 2u, 2u, 2u, 2u, 11u)
#define PPUTLIMPL_UINT_TRAIT_351u  DEC, 0x15F, 0, 8u, 18u, (3u, 3u, 3u, 13u)
#define PPUTLIMPL_UINT_TRAIT_350u  DEC, 0x15E, 0, 8u, 18u, (2u, 5u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_349u  DEC, 0x15D, 0, 8u, 18u, ()
#define PPUTLIMPL_UINT_TRAIT_348u  DEC, 0x15C, 0, 8u, 18u, (2u, 2u, 3u, 29u)
#define PPUTLIMPL_UINT_TRAIT_347u  DEC, 0x15B, 0, 8u, 18u, ()
#define PPUTLIMPL_UINT_TRAIT_346u  DEC, 0x15A, 0, 8u, 18u, (2u, 173u)
#define PPUTLIMPL_UINT_TRAIT_345u  DEC, 0x159, 0, 8u, 18u, (3u, 5u, 23u)
#define PPUTLIMPL_UINT_TRAIT_344u  DEC, 0x158, 0, 8u, 18u, (2u, 2u, 2u, 43u)
#define PPUTLIMPL_UINT_TRAIT_343u  DEC, 0x157, 0, 8u, 18u, (7u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_342u  DEC, 0x156, 0, 8u, 18u, (2u, 3u, 3u, 19u)
#define PPUTLIMPL_UINT_TRAIT_341u  DEC, 0x155, 0, 8u, 18u, (11u, 31u)
#define PPUTLIMPL_UINT_TRAIT_340u  DEC, 0x154, 0, 8u, 18u, (2u, 2u, 5u, 17u)
#define PPUTLIMPL_UINT_TRAIT_339u  DEC, 0x153, 0, 8u, 18u, (3u, 113u)
#define PPUTLIMPL_UINT_TRAIT_338u  DEC, 0x152, 0, 8u, 18u, (2u, 13u, 13u)
#define PPUTLIMPL_UINT_TRAIT_337u  DEC, 0x151, 0, 8u, 18u, ()
#define PPUTLIMPL_UINT_TRAIT_336u  DEC, 0x150, 0, 8u, 18u, (2u, 2u, 2u, 2u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_335u  DEC, 0x14F, 0, 8u, 18u, (5u, 67u)
#define PPUTLIMPL_UINT_TRAIT_334u  DEC, 0x14E, 0, 8u, 18u, (2u, 167u)
#define PPUTLIMPL_UINT_TRAIT_333u  DEC, 0x14D, 0, 8u, 18u, (3u, 3u, 37u)
#define PPUTLIMPL_UINT_TRAIT_332u  DEC, 0x14C, 0, 8u, 18u, (2u, 2u, 83u)
#define PPUTLIMPL_UINT_TRAIT_331u  DEC, 0x14B, 0, 8u, 18u, ()
#define PPUTLIMPL_UINT_TRAIT_330u  DEC, 0x14A, 0, 8u, 18u, (2u, 3u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_329u  DEC, 0x149, 0, 8u, 18u, (7u, 47u)
#define PPUTLIMPL_UINT_TRAIT_328u  DEC, 0x148, 0, 8u, 18u, (2u, 2u, 2u, 41u)
#define PPUTLIMPL_UINT_TRAIT_327u  DEC, 0x147, 0, 8u, 18u, (3u, 109u)
#define PPUTLIMPL_UINT_TRAIT_326u  DEC, 0x146, 0, 8u, 18u, (2u, 163u)
#define PPUTLIMPL_UINT_TRAIT_325u  DEC, 0x145, 0, 8u, 18u, (5u, 5u, 13u)
#define PPUTLIMPL_UINT_TRAIT_324u  DEC, 0x144, 0, 8u, 18u, (2u, 2u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_323u  DEC, 0x143, 0, 8u, 17u, (17u, 19u)
#define PPUTLIMPL_UINT_TRAIT_322u  DEC, 0x142, 0, 8u, 17u, (2u, 7u, 23u)
#define PPUTLIMPL_UINT_TRAIT_321u  DEC, 0x141, 0, 8u, 17u, (3u, 107u)
#define PPUTLIMPL_UINT_TRAIT_320u  DEC, 0x140, 0, 8u, 17u, (2u, 2u, 2u, 2u, 2u, 2u, 5u)
#define PPUTLIMPL_UINT_TRAIT_319u  DEC, 0x13F, 0, 8u, 17u, (11u, 29u)
#define PPUTLIMPL_UINT_TRAIT_318u  DEC, 0x13E, 0, 8u, 17u, (2u, 3u, 53u)
#define PPUTLIMPL_UINT_TRAIT_317u  DEC, 0x13D, 0, 8u, 17u, ()
#define PPUTLIMPL_UINT_TRAIT_316u  DEC, 0x13C, 0, 8u, 17u, (2u, 2u, 79u)
#define PPUTLIMPL_UINT_TRAIT_315u  DEC, 0x13B, 0, 8u, 17u, (3u, 3u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_314u  DEC, 0x13A, 0, 8u, 17u, (2u, 157u)
#define PPUTLIMPL_UINT_TRAIT_313u  DEC, 0x139, 0, 8u, 17u, ()
#define PPUTLIMPL_UINT_TRAIT_312u  DEC, 0x138, 0, 8u, 17u, (2u, 2u, 2u, 3u, 13u)
#define PPUTLIMPL_UINT_TRAIT_311u  DEC, 0x137, 0, 8u, 17u, ()
#define PPUTLIMPL_UINT_TRAIT_310u  DEC, 0x136, 0, 8u, 17u, (2u, 5u, 31u)
#define PPUTLIMPL_UINT_TRAIT_309u  DEC, 0x135, 0, 8u, 17u, (3u, 103u)
#define PPUTLIMPL_UINT_TRAIT_308u  DEC, 0x134, 0, 8u, 17u, (2u, 2u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_307u  DEC, 0x133, 0, 8u, 17u, ()
#define PPUTLIMPL_UINT_TRAIT_306u  DEC, 0x132, 0, 8u, 17u, (2u, 3u, 3u, 17u)
#define PPUTLIMPL_UINT_TRAIT_305u  DEC, 0x131, 0, 8u, 17u, (5u, 61u)
#define PPUTLIMPL_UINT_TRAIT_304u  DEC, 0x130, 0, 8u, 17u, (2u, 2u, 2u, 2u, 19u)
#define PPUTLIMPL_UINT_TRAIT_303u  DEC, 0x12F, 0, 8u, 17u, (3u, 101u)
#define PPUTLIMPL_UINT_TRAIT_302u  DEC, 0x12E, 0, 8u, 17u, (2u, 151u)
#define PPUTLIMPL_UINT_TRAIT_301u  DEC, 0x12D, 0, 8u, 17u, (7u, 43u)
#define PPUTLIMPL_UINT_TRAIT_300u  DEC, 0x12C, 0, 8u, 17u, (2u, 2u, 3u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_299u  DEC, 0x12B, 0, 8u, 17u, (13u, 23u)
#define PPUTLIMPL_UINT_TRAIT_298u  DEC, 0x12A, 0, 8u, 17u, (2u, 149u)
#define PPUTLIMPL_UINT_TRAIT_297u  DEC, 0x129, 0, 8u, 17u, (3u, 3u, 3u, 11u)
#define PPUTLIMPL_UINT_TRAIT_296u  DEC, 0x128, 0, 8u, 17u, (2u, 2u, 2u, 37u)
#define PPUTLIMPL_UINT_TRAIT_295u  DEC, 0x127, 0, 8u, 17u, (5u, 59u)
#define PPUTLIMPL_UINT_TRAIT_294u  DEC, 0x126, 0, 8u, 17u, (2u, 3u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_293u  DEC, 0x125, 0, 8u, 17u, ()
#define PPUTLIMPL_UINT_TRAIT_292u  DEC, 0x124, 0, 8u, 17u, (2u, 2u, 73u)
#define PPUTLIMPL_UINT_TRAIT_291u  DEC, 0x123, 0, 8u, 17u, (3u, 97u)
#define PPUTLIMPL_UINT_TRAIT_290u  DEC, 0x122, 0, 8u, 17u, (2u, 5u, 29u)
#define PPUTLIMPL_UINT_TRAIT_289u  DEC, 0x121, 0, 8u, 17u, (17u, 17u)
#define PPUTLIMPL_UINT_TRAIT_288u  DEC, 0x120, 0, 8u, 16u, (2u, 2u, 2u, 2u, 2u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_287u  DEC, 0x11F, 0, 8u, 16u, (7u, 41u)
#define PPUTLIMPL_UINT_TRAIT_286u  DEC, 0x11E, 0, 8u, 16u, (2u, 11u, 13u)
#define PPUTLIMPL_UINT_TRAIT_285u  DEC, 0x11D, 0, 8u, 16u, (3u, 5u, 19u)
#define PPUTLIMPL_UINT_TRAIT_284u  DEC, 0x11C, 0, 8u, 16u, (2u, 2u, 71u)
#define PPUTLIMPL_UINT_TRAIT_283u  DEC, 0x11B, 0, 8u, 16u, ()
#define PPUTLIMPL_UINT_TRAIT_282u  DEC, 0x11A, 0, 8u, 16u, (2u, 3u, 47u)
#define PPUTLIMPL_UINT_TRAIT_281u  DEC, 0x119, 0, 8u, 16u, ()
#define PPUTLIMPL_UINT_TRAIT_280u  DEC, 0x118, 0, 8u, 16u, (2u, 2u, 2u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_279u  DEC, 0x117, 0, 8u, 16u, (3u, 3u, 31u)
#define PPUTLIMPL_UINT_TRAIT_278u  DEC, 0x116, 0, 8u, 16u, (2u, 139u)
#define PPUTLIMPL_UINT_TRAIT_277u  DEC, 0x115, 0, 8u, 16u, ()
#define PPUTLIMPL_UINT_TRAIT_276u  DEC, 0x114, 0, 8u, 16u, (2u, 2u, 3u, 23u)
#define PPUTLIMPL_UINT_TRAIT_275u  DEC, 0x113, 0, 8u, 16u, (5u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_274u  DEC, 0x112, 0, 8u, 16u, (2u, 137u)
#define PPUTLIMPL_UINT_TRAIT_273u  DEC, 0x111, 0, 8u, 16u, (3u, 7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_272u  DEC, 0x110, 0, 8u, 16u, (2u, 2u, 2u, 2u, 17u)
#define PPUTLIMPL_UINT_TRAIT_271u  DEC, 0x10F, 0, 8u, 16u, ()
#define PPUTLIMPL_UINT_TRAIT_270u  DEC, 0x10E, 0, 8u, 16u, (2u, 3u, 3u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_269u  DEC, 0x10D, 0, 8u, 16u, ()
#define PPUTLIMPL_UINT_TRAIT_268u  DEC, 0x10C, 0, 8u, 16u, (2u, 2u, 67u)
#define PPUTLIMPL_UINT_TRAIT_267u  DEC, 0x10B, 0, 8u, 16u, (3u, 89u)
#define PPUTLIMPL_UINT_TRAIT_266u  DEC, 0x10A, 0, 8u, 16u, (2u, 7u, 19u)
#define PPUTLIMPL_UINT_TRAIT_265u  DEC, 0x109, 0, 8u, 16u, (5u, 53u)
#define PPUTLIMPL_UINT_TRAIT_264u  DEC, 0x108, 0, 8u, 16u, (2u, 2u, 2u, 3u, 11u)
#define PPUTLIMPL_UINT_TRAIT_263u  DEC, 0x107, 0, 8u, 16u, ()
#define PPUTLIMPL_UINT_TRAIT_262u  DEC, 0x106, 0, 8u, 16u, (2u, 131u)
#define PPUTLIMPL_UINT_TRAIT_261u  DEC, 0x105, 0, 8u, 16u, (3u, 3u, 29u)
#define PPUTLIMPL_UINT_TRAIT_260u  DEC, 0x104, 0, 8u, 16u, (2u, 2u, 5u, 13u)
#define PPUTLIMPL_UINT_TRAIT_259u  DEC, 0x103, 0, 8u, 16u, (7u, 37u)
#define PPUTLIMPL_UINT_TRAIT_258u  DEC, 0x102, 0, 8u, 16u, (2u, 3u, 43u)
#define PPUTLIMPL_UINT_TRAIT_257u  DEC, 0x101, 0, 8u, 16u, ()
#define PPUTLIMPL_UINT_TRAIT_256u  DEC, 0x100, 0, 8u, 16u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u)
#define PPUTLIMPL_UINT_TRAIT_255u  DEC, 0x0FF, 0, 7u, 15u, (3u, 5u, 17u)
#define PPUTLIMPL_UINT_TRAIT_254u  DEC, 0x0FE, 0, 7u, 15u, (2u, 127u)
#define PPUTLIMPL_UINT_TRAIT_253u  DEC, 0x0FD, 0, 7u, 15u, (11u, 23u)
#define PPUTLIMPL_UINT_TRAIT_252u  DEC, 0x0FC, 0, 7u, 15u, (2u, 2u, 3u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_251u  DEC, 0x0FB, 0, 7u, 15u, ()
#define PPUTLIMPL_UINT_TRAIT_250u  DEC, 0x0FA, 0, 7u, 15u, (2u, 5u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_249u  DEC, 0x0F9, 0, 7u, 15u, (3u, 83u)
#define PPUTLIMPL_UINT_TRAIT_248u  DEC, 0x0F8, 0, 7u, 15u, (2u, 2u, 2u, 31u)
#define PPUTLIMPL_UINT_TRAIT_247u  DEC, 0x0F7, 0, 7u, 15u, (13u, 19u)
#define PPUTLIMPL_UINT_TRAIT_246u  DEC, 0x0F6, 0, 7u, 15u, (2u, 3u, 41u)
#define PPUTLIMPL_UINT_TRAIT_245u  DEC, 0x0F5, 0, 7u, 15u, (5u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_244u  DEC, 0x0F4, 0, 7u, 15u, (2u, 2u, 61u)
#define PPUTLIMPL_UINT_TRAIT_243u  DEC, 0x0F3, 0, 7u, 15u, (3u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_242u  DEC, 0x0F2, 0, 7u, 15u, (2u, 11u, 11u)
#define PPUTLIMPL_UINT_TRAIT_241u  DEC, 0x0F1, 0, 7u, 15u, ()
#define PPUTLIMPL_UINT_TRAIT_240u  DEC, 0x0F0, 0, 7u, 15u, (2u, 2u, 2u, 2u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_239u  DEC, 0x0EF, 0, 7u, 15u, ()
#define PPUTLIMPL_UINT_TRAIT_238u  DEC, 0x0EE, 0, 7u, 15u, (2u, 7u, 17u)
#define PPUTLIMPL_UINT_TRAIT_237u  DEC, 0x0ED, 0, 7u, 15u, (3u, 79u)
#define PPUTLIMPL_UINT_TRAIT_236u  DEC, 0x0EC, 0, 7u, 15u, (2u, 2u, 59u)
#define PPUTLIMPL_UINT_TRAIT_235u  DEC, 0x0EB, 0, 7u, 15u, (5u, 47u)
#define PPUTLIMPL_UINT_TRAIT_234u  DEC, 0x0EA, 0, 7u, 15u, (2u, 3u, 3u, 13u)
#define PPUTLIMPL_UINT_TRAIT_233u  DEC, 0x0E9, 0, 7u, 15u, ()
#define PPUTLIMPL_UINT_TRAIT_232u  DEC, 0x0E8, 0, 7u, 15u, (2u, 2u, 2u, 29u)
#define PPUTLIMPL_UINT_TRAIT_231u  DEC, 0x0E7, 0, 7u, 15u, (3u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_230u  DEC, 0x0E6, 0, 7u, 15u, (2u, 5u, 23u)
#define PPUTLIMPL_UINT_TRAIT_229u  DEC, 0x0E5, 0, 7u, 15u, ()
#define PPUTLIMPL_UINT_TRAIT_228u  DEC, 0x0E4, 0, 7u, 15u, (2u, 2u, 3u, 19u)
#define PPUTLIMPL_UINT_TRAIT_227u  DEC, 0x0E3, 0, 7u, 15u, ()
#define PPUTLIMPL_UINT_TRAIT_226u  DEC, 0x0E2, 0, 7u, 15u, (2u, 113u)
#define PPUTLIMPL_UINT_TRAIT_225u  DEC, 0x0E1, 0, 7u, 15u, (3u, 3u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_224u  DEC, 0x0E0, 0, 7u, 14u, (2u, 2u, 2u, 2u, 2u, 7u)
#define PPUTLIMPL_UINT_TRAIT_223u  DEC, 0x0DF, 0, 7u, 14u, ()
#define PPUTLIMPL_UINT_TRAIT_222u  DEC, 0x0DE, 0, 7u, 14u, (2u, 3u, 37u)
#define PPUTLIMPL_UINT_TRAIT_221u  DEC, 0x0DD, 0, 7u, 14u, (13u, 17u)
#define PPUTLIMPL_UINT_TRAIT_220u  DEC, 0x0DC, 0, 7u, 14u, (2u, 2u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_219u  DEC, 0x0DB, 0, 7u, 14u, (3u, 73u)
#define PPUTLIMPL_UINT_TRAIT_218u  DEC, 0x0DA, 0, 7u, 14u, (2u, 109u)
#define PPUTLIMPL_UINT_TRAIT_217u  DEC, 0x0D9, 0, 7u, 14u, (7u, 31u)
#define PPUTLIMPL_UINT_TRAIT_216u  DEC, 0x0D8, 0, 7u, 14u, (2u, 2u, 2u, 3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_215u  DEC, 0x0D7, 0, 7u, 14u, (5u, 43u)
#define PPUTLIMPL_UINT_TRAIT_214u  DEC, 0x0D6, 0, 7u, 14u, (2u, 107u)
#define PPUTLIMPL_UINT_TRAIT_213u  DEC, 0x0D5, 0, 7u, 14u, (3u, 71u)
#define PPUTLIMPL_UINT_TRAIT_212u  DEC, 0x0D4, 0, 7u, 14u, (2u, 2u, 53u)
#define PPUTLIMPL_UINT_TRAIT_211u  DEC, 0x0D3, 0, 7u, 14u, ()
#define PPUTLIMPL_UINT_TRAIT_210u  DEC, 0x0D2, 0, 7u, 14u, (2u, 3u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_209u  DEC, 0x0D1, 0, 7u, 14u, (11u, 19u)
#define PPUTLIMPL_UINT_TRAIT_208u  DEC, 0x0D0, 0, 7u, 14u, (2u, 2u, 2u, 2u, 13u)
#define PPUTLIMPL_UINT_TRAIT_207u  DEC, 0x0CF, 0, 7u, 14u, (3u, 3u, 23u)
#define PPUTLIMPL_UINT_TRAIT_206u  DEC, 0x0CE, 0, 7u, 14u, (2u, 103u)
#define PPUTLIMPL_UINT_TRAIT_205u  DEC, 0x0CD, 0, 7u, 14u, (5u, 41u)
#define PPUTLIMPL_UINT_TRAIT_204u  DEC, 0x0CC, 0, 7u, 14u, (2u, 2u, 3u, 17u)
#define PPUTLIMPL_UINT_TRAIT_203u  DEC, 0x0CB, 0, 7u, 14u, (7u, 29u)
#define PPUTLIMPL_UINT_TRAIT_202u  DEC, 0x0CA, 0, 7u, 14u, (2u, 101u)
#define PPUTLIMPL_UINT_TRAIT_201u  DEC, 0x0C9, 0, 7u, 14u, (3u, 67u)
#define PPUTLIMPL_UINT_TRAIT_200u  DEC, 0x0C8, 0, 7u, 14u, (2u, 2u, 2u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_199u  DEC, 0x0C7, 0, 7u, 14u, ()
#define PPUTLIMPL_UINT_TRAIT_198u  DEC, 0x0C6, 0, 7u, 14u, (2u, 3u, 3u, 11u)
#define PPUTLIMPL_UINT_TRAIT_197u  DEC, 0x0C5, 0, 7u, 14u, ()
#define PPUTLIMPL_UINT_TRAIT_196u  DEC, 0x0C4, 0, 7u, 14u, (2u, 2u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_195u  DEC, 0x0C3, 0, 7u, 13u, (3u, 5u, 13u)
#define PPUTLIMPL_UINT_TRAIT_194u  DEC, 0x0C2, 0, 7u, 13u, (2u, 97u)
#define PPUTLIMPL_UINT_TRAIT_193u  DEC, 0x0C1, 0, 7u, 13u, ()
#define PPUTLIMPL_UINT_TRAIT_192u  DEC, 0x0C0, 0, 7u, 13u, (2u, 2u, 2u, 2u, 2u, 2u, 3u)
#define PPUTLIMPL_UINT_TRAIT_191u  DEC, 0x0BF, 0, 7u, 13u, ()
#define PPUTLIMPL_UINT_TRAIT_190u  DEC, 0x0BE, 0, 7u, 13u, (2u, 5u, 19u)
#define PPUTLIMPL_UINT_TRAIT_189u  DEC, 0x0BD, 0, 7u, 13u, (3u, 3u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_188u  DEC, 0x0BC, 0, 7u, 13u, (2u, 2u, 47u)
#define PPUTLIMPL_UINT_TRAIT_187u  DEC, 0x0BB, 0, 7u, 13u, (11u, 17u)
#define PPUTLIMPL_UINT_TRAIT_186u  DEC, 0x0BA, 0, 7u, 13u, (2u, 3u, 31u)
#define PPUTLIMPL_UINT_TRAIT_185u  DEC, 0x0B9, 0, 7u, 13u, (5u, 37u)
#define PPUTLIMPL_UINT_TRAIT_184u  DEC, 0x0B8, 0, 7u, 13u, (2u, 2u, 2u, 23u)
#define PPUTLIMPL_UINT_TRAIT_183u  DEC, 0x0B7, 0, 7u, 13u, (3u, 61u)
#define PPUTLIMPL_UINT_TRAIT_182u  DEC, 0x0B6, 0, 7u, 13u, (2u, 7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_181u  DEC, 0x0B5, 0, 7u, 13u, ()
#define PPUTLIMPL_UINT_TRAIT_180u  DEC, 0x0B4, 0, 7u, 13u, (2u, 2u, 3u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_179u  DEC, 0x0B3, 0, 7u, 13u, ()
#define PPUTLIMPL_UINT_TRAIT_178u  DEC, 0x0B2, 0, 7u, 13u, (2u, 89u)
#define PPUTLIMPL_UINT_TRAIT_177u  DEC, 0x0B1, 0, 7u, 13u, (3u, 59u)
#define PPUTLIMPL_UINT_TRAIT_176u  DEC, 0x0B0, 0, 7u, 13u, (2u, 2u, 2u, 2u, 11u)
#define PPUTLIMPL_UINT_TRAIT_175u  DEC, 0x0AF, 0, 7u, 13u, (5u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_174u  DEC, 0x0AE, 0, 7u, 13u, (2u, 3u, 29u)
#define PPUTLIMPL_UINT_TRAIT_173u  DEC, 0x0AD, 0, 7u, 13u, ()
#define PPUTLIMPL_UINT_TRAIT_172u  DEC, 0x0AC, 0, 7u, 13u, (2u, 2u, 43u)
#define PPUTLIMPL_UINT_TRAIT_171u  DEC, 0x0AB, 0, 7u, 13u, (3u, 3u, 19u)
#define PPUTLIMPL_UINT_TRAIT_170u  DEC, 0x0AA, 0, 7u, 13u, (2u, 5u, 17u)
#define PPUTLIMPL_UINT_TRAIT_169u  DEC, 0x0A9, 0, 7u, 13u, (13u, 13u)
#define PPUTLIMPL_UINT_TRAIT_168u  DEC, 0x0A8, 0, 7u, 12u, (2u, 2u, 2u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_167u  DEC, 0x0A7, 0, 7u, 12u, ()
#define PPUTLIMPL_UINT_TRAIT_166u  DEC, 0x0A6, 0, 7u, 12u, (2u, 83u)
#define PPUTLIMPL_UINT_TRAIT_165u  DEC, 0x0A5, 0, 7u, 12u, (3u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_164u  DEC, 0x0A4, 0, 7u, 12u, (2u, 2u, 41u)
#define PPUTLIMPL_UINT_TRAIT_163u  DEC, 0x0A3, 0, 7u, 12u, ()
#define PPUTLIMPL_UINT_TRAIT_162u  DEC, 0x0A2, 0, 7u, 12u, (2u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_161u  DEC, 0x0A1, 0, 7u, 12u, (7u, 23u)
#define PPUTLIMPL_UINT_TRAIT_160u  DEC, 0x0A0, 0, 7u, 12u, (2u, 2u, 2u, 2u, 2u, 5u)
#define PPUTLIMPL_UINT_TRAIT_159u  DEC, 0x09F, 0, 7u, 12u, (3u, 53u)
#define PPUTLIMPL_UINT_TRAIT_158u  DEC, 0x09E, 0, 7u, 12u, (2u, 79u)
#define PPUTLIMPL_UINT_TRAIT_157u  DEC, 0x09D, 0, 7u, 12u, ()
#define PPUTLIMPL_UINT_TRAIT_156u  DEC, 0x09C, 0, 7u, 12u, (2u, 2u, 3u, 13u)
#define PPUTLIMPL_UINT_TRAIT_155u  DEC, 0x09B, 0, 7u, 12u, (5u, 31u)
#define PPUTLIMPL_UINT_TRAIT_154u  DEC, 0x09A, 0, 7u, 12u, (2u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_153u  DEC, 0x099, 0, 7u, 12u, (3u, 3u, 17u)
#define PPUTLIMPL_UINT_TRAIT_152u  DEC, 0x098, 0, 7u, 12u, (2u, 2u, 2u, 19u)
#define PPUTLIMPL_UINT_TRAIT_151u  DEC, 0x097, 0, 7u, 12u, ()
#define PPUTLIMPL_UINT_TRAIT_150u  DEC, 0x096, 0, 7u, 12u, (2u, 3u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_149u  DEC, 0x095, 0, 7u, 12u, ()
#define PPUTLIMPL_UINT_TRAIT_148u  DEC, 0x094, 0, 7u, 12u, (2u, 2u, 37u)
#define PPUTLIMPL_UINT_TRAIT_147u  DEC, 0x093, 0, 7u, 12u, (3u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_146u  DEC, 0x092, 0, 7u, 12u, (2u, 73u)
#define PPUTLIMPL_UINT_TRAIT_145u  DEC, 0x091, 0, 7u, 12u, (5u, 29u)
#define PPUTLIMPL_UINT_TRAIT_144u  DEC, 0x090, 0, 7u, 12u, (2u, 2u, 2u, 2u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_143u  DEC, 0x08F, 0, 7u, 11u, (11u, 13u)
#define PPUTLIMPL_UINT_TRAIT_142u  DEC, 0x08E, 0, 7u, 11u, (2u, 71u)
#define PPUTLIMPL_UINT_TRAIT_141u  DEC, 0x08D, 0, 7u, 11u, (3u, 47u)
#define PPUTLIMPL_UINT_TRAIT_140u  DEC, 0x08C, 0, 7u, 11u, (2u, 2u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_139u  DEC, 0x08B, 0, 7u, 11u, ()
#define PPUTLIMPL_UINT_TRAIT_138u  DEC, 0x08A, 0, 7u, 11u, (2u, 3u, 23u)
#define PPUTLIMPL_UINT_TRAIT_137u  DEC, 0x089, 0, 7u, 11u, ()
#define PPUTLIMPL_UINT_TRAIT_136u  DEC, 0x088, 0, 7u, 11u, (2u, 2u, 2u, 17u)
#define PPUTLIMPL_UINT_TRAIT_135u  DEC, 0x087, 0, 7u, 11u, (3u, 3u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_134u  DEC, 0x086, 0, 7u, 11u, (2u, 67u)
#define PPUTLIMPL_UINT_TRAIT_133u  DEC, 0x085, 0, 7u, 11u, (7u, 19u)
#define PPUTLIMPL_UINT_TRAIT_132u  DEC, 0x084, 0, 7u, 11u, (2u, 2u, 3u, 11u)
#define PPUTLIMPL_UINT_TRAIT_131u  DEC, 0x083, 0, 7u, 11u, ()
#define PPUTLIMPL_UINT_TRAIT_130u  DEC, 0x082, 0, 7u, 11u, (2u, 5u, 13u)
#define PPUTLIMPL_UINT_TRAIT_129u  DEC, 0x081, 0, 7u, 11u, (3u, 43u)
#define PPUTLIMPL_UINT_TRAIT_128u  DEC, 0x080, 0, 7u, 11u, (2u, 2u, 2u, 2u, 2u, 2u, 2u)
#define PPUTLIMPL_UINT_TRAIT_127u  DEC, 0x07F, 0, 6u, 11u, ()
#define PPUTLIMPL_UINT_TRAIT_126u  DEC, 0x07E, 0, 6u, 11u, (2u, 3u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_125u  DEC, 0x07D, 0, 6u, 11u, (5u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_124u  DEC, 0x07C, 0, 6u, 11u, (2u, 2u, 31u)
#define PPUTLIMPL_UINT_TRAIT_123u  DEC, 0x07B, 0, 6u, 11u, (3u, 41u)
#define PPUTLIMPL_UINT_TRAIT_122u  DEC, 0x07A, 0, 6u, 11u, (2u, 61u)
#define PPUTLIMPL_UINT_TRAIT_121u  DEC, 0x079, 0, 6u, 11u, (11u, 11u)
#define PPUTLIMPL_UINT_TRAIT_120u  DEC, 0x078, 0, 6u, 10u, (2u, 2u, 2u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_119u  DEC, 0x077, 0, 6u, 10u, (7u, 17u)
#define PPUTLIMPL_UINT_TRAIT_118u  DEC, 0x076, 0, 6u, 10u, (2u, 59u)
#define PPUTLIMPL_UINT_TRAIT_117u  DEC, 0x075, 0, 6u, 10u, (3u, 3u, 13u)
#define PPUTLIMPL_UINT_TRAIT_116u  DEC, 0x074, 0, 6u, 10u, (2u, 2u, 29u)
#define PPUTLIMPL_UINT_TRAIT_115u  DEC, 0x073, 0, 6u, 10u, (5u, 23u)
#define PPUTLIMPL_UINT_TRAIT_114u  DEC, 0x072, 0, 6u, 10u, (2u, 3u, 19u)
#define PPUTLIMPL_UINT_TRAIT_113u  DEC, 0x071, 0, 6u, 10u, ()
#define PPUTLIMPL_UINT_TRAIT_112u  DEC, 0x070, 0, 6u, 10u, (2u, 2u, 2u, 2u, 7u)
#define PPUTLIMPL_UINT_TRAIT_111u  DEC, 0x06F, 0, 6u, 10u, (3u, 37u)
#define PPUTLIMPL_UINT_TRAIT_110u  DEC, 0x06E, 0, 6u, 10u, (2u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_109u  DEC, 0x06D, 0, 6u, 10u, ()
#define PPUTLIMPL_UINT_TRAIT_108u  DEC, 0x06C, 0, 6u, 10u, (2u, 2u, 3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_107u  DEC, 0x06B, 0, 6u, 10u, ()
#define PPUTLIMPL_UINT_TRAIT_106u  DEC, 0x06A, 0, 6u, 10u, (2u, 53u)
#define PPUTLIMPL_UINT_TRAIT_105u  DEC, 0x069, 0, 6u, 10u, (3u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_104u  DEC, 0x068, 0, 6u, 10u, (2u, 2u, 2u, 13u)
#define PPUTLIMPL_UINT_TRAIT_103u  DEC, 0x067, 0, 6u, 10u, ()
#define PPUTLIMPL_UINT_TRAIT_102u  DEC, 0x066, 0, 6u, 10u, (2u, 3u, 17u)
#define PPUTLIMPL_UINT_TRAIT_101u  DEC, 0x065, 0, 6u, 10u, ()
#define PPUTLIMPL_UINT_TRAIT_100u  DEC, 0x064, 0, 6u, 10u, (2u, 2u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_99u   DEC, 0x063, 0, 6u, 9u, (3u, 3u, 11u)
#define PPUTLIMPL_UINT_TRAIT_98u   DEC, 0x062, 0, 6u, 9u, (2u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_97u   DEC, 0x061, 0, 6u, 9u, ()
#define PPUTLIMPL_UINT_TRAIT_96u   DEC, 0x060, 0, 6u, 9u, (2u, 2u, 2u, 2u, 2u, 3u)
#define PPUTLIMPL_UINT_TRAIT_95u   DEC, 0x05F, 0, 6u, 9u, (5u, 19u)
#define PPUTLIMPL_UINT_TRAIT_94u   DEC, 0x05E, 0, 6u, 9u, (2u, 47u)
#define PPUTLIMPL_UINT_TRAIT_93u   DEC, 0x05D, 0, 6u, 9u, (3u, 31u)
#define PPUTLIMPL_UINT_TRAIT_92u   DEC, 0x05C, 0, 6u, 9u, (2u, 2u, 23u)
#define PPUTLIMPL_UINT_TRAIT_91u   DEC, 0x05B, 0, 6u, 9u, (7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_90u   DEC, 0x05A, 0, 6u, 9u, (2u, 3u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_89u   DEC, 0x059, 0, 6u, 9u, ()
#define PPUTLIMPL_UINT_TRAIT_88u   DEC, 0x058, 0, 6u, 9u, (2u, 2u, 2u, 11u)
#define PPUTLIMPL_UINT_TRAIT_87u   DEC, 0x057, 0, 6u, 9u, (3u, 29u)
#define PPUTLIMPL_UINT_TRAIT_86u   DEC, 0x056, 0, 6u, 9u, (2u, 43u)
#define PPUTLIMPL_UINT_TRAIT_85u   DEC, 0x055, 0, 6u, 9u, (5u, 17u)
#define PPUTLIMPL_UINT_TRAIT_84u   DEC, 0x054, 0, 6u, 9u, (2u, 2u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_83u   DEC, 0x053, 0, 6u, 9u, ()
#define PPUTLIMPL_UINT_TRAIT_82u   DEC, 0x052, 0, 6u, 9u, (2u, 41u)
#define PPUTLIMPL_UINT_TRAIT_81u   DEC, 0x051, 0, 6u, 9u, (3u, 3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_80u   DEC, 0x050, 0, 6u, 8u, (2u, 2u, 2u, 2u, 5u)
#define PPUTLIMPL_UINT_TRAIT_79u   DEC, 0x04F, 0, 6u, 8u, ()
#define PPUTLIMPL_UINT_TRAIT_78u   DEC, 0x04E, 0, 6u, 8u, (2u, 3u, 13u)
#define PPUTLIMPL_UINT_TRAIT_77u   DEC, 0x04D, 0, 6u, 8u, (7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_76u   DEC, 0x04C, 0, 6u, 8u, (2u, 2u, 19u)
#define PPUTLIMPL_UINT_TRAIT_75u   DEC, 0x04B, 0, 6u, 8u, (3u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_74u   DEC, 0x04A, 0, 6u, 8u, (2u, 37u)
#define PPUTLIMPL_UINT_TRAIT_73u   DEC, 0x049, 0, 6u, 8u, ()
#define PPUTLIMPL_UINT_TRAIT_72u   DEC, 0x048, 0, 6u, 8u, (2u, 2u, 2u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_71u   DEC, 0x047, 0, 6u, 8u, ()
#define PPUTLIMPL_UINT_TRAIT_70u   DEC, 0x046, 0, 6u, 8u, (2u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_69u   DEC, 0x045, 0, 6u, 8u, (3u, 23u)
#define PPUTLIMPL_UINT_TRAIT_68u   DEC, 0x044, 0, 6u, 8u, (2u, 2u, 17u)
#define PPUTLIMPL_UINT_TRAIT_67u   DEC, 0x043, 0, 6u, 8u, ()
#define PPUTLIMPL_UINT_TRAIT_66u   DEC, 0x042, 0, 6u, 8u, (2u, 3u, 11u)
#define PPUTLIMPL_UINT_TRAIT_65u   DEC, 0x041, 0, 6u, 8u, (5u, 13u)
#define PPUTLIMPL_UINT_TRAIT_64u   DEC, 0x040, 0, 6u, 8u, (2u, 2u, 2u, 2u, 2u, 2u)
#define PPUTLIMPL_UINT_TRAIT_63u   DEC, 0x03F, 0, 5u, 7u, (3u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_62u   DEC, 0x03E, 0, 5u, 7u, (2u, 31u)
#define PPUTLIMPL_UINT_TRAIT_61u   DEC, 0x03D, 0, 5u, 7u, ()
#define PPUTLIMPL_UINT_TRAIT_60u   DEC, 0x03C, 0, 5u, 7u, (2u, 2u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_59u   DEC, 0x03B, 0, 5u, 7u, ()
#define PPUTLIMPL_UINT_TRAIT_58u   DEC, 0x03A, 0, 5u, 7u, (2u, 29u)
#define PPUTLIMPL_UINT_TRAIT_57u   DEC, 0x039, 0, 5u, 7u, (3u, 19u)
#define PPUTLIMPL_UINT_TRAIT_56u   DEC, 0x038, 0, 5u, 7u, (2u, 2u, 2u, 7u)
#define PPUTLIMPL_UINT_TRAIT_55u   DEC, 0x037, 0, 5u, 7u, (5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_54u   DEC, 0x036, 0, 5u, 7u, (2u, 3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_53u   DEC, 0x035, 0, 5u, 7u, ()
#define PPUTLIMPL_UINT_TRAIT_52u   DEC, 0x034, 0, 5u, 7u, (2u, 2u, 13u)
#define PPUTLIMPL_UINT_TRAIT_51u   DEC, 0x033, 0, 5u, 7u, (3u, 17u)
#define PPUTLIMPL_UINT_TRAIT_50u   DEC, 0x032, 0, 5u, 7u, (2u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_49u   DEC, 0x031, 0, 5u, 7u, (7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_48u   DEC, 0x030, 0, 5u, 6u, (2u, 2u, 2u, 2u, 3u)
#define PPUTLIMPL_UINT_TRAIT_47u   DEC, 0x02F, 0, 5u, 6u, ()
#define PPUTLIMPL_UINT_TRAIT_46u   DEC, 0x02E, 0, 5u, 6u, (2u, 23u)
#define PPUTLIMPL_UINT_TRAIT_45u   DEC, 0x02D, 0, 5u, 6u, (3u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_44u   DEC, 0x02C, 0, 5u, 6u, (2u, 2u, 11u)
#define PPUTLIMPL_UINT_TRAIT_43u   DEC, 0x02B, 0, 5u, 6u, ()
#define PPUTLIMPL_UINT_TRAIT_42u   DEC, 0x02A, 0, 5u, 6u, (2u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_41u   DEC, 0x029, 0, 5u, 6u, ()
#define PPUTLIMPL_UINT_TRAIT_40u   DEC, 0x028, 0, 5u, 6u, (2u, 2u, 2u, 5u)
#define PPUTLIMPL_UINT_TRAIT_39u   DEC, 0x027, 0, 5u, 6u, (3u, 13u)
#define PPUTLIMPL_UINT_TRAIT_38u   DEC, 0x026, 0, 5u, 6u, (2u, 19u)
#define PPUTLIMPL_UINT_TRAIT_37u   DEC, 0x025, 0, 5u, 6u, ()
#define PPUTLIMPL_UINT_TRAIT_36u   DEC, 0x024, 0, 5u, 6u, (2u, 2u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_35u   DEC, 0x023, 0, 5u, 5u, (5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_34u   DEC, 0x022, 0, 5u, 5u, (2u, 17u)
#define PPUTLIMPL_UINT_TRAIT_33u   DEC, 0x021, 0, 5u, 5u, (3u, 11u)
#define PPUTLIMPL_UINT_TRAIT_32u   DEC, 0x020, 0, 5u, 5u, (2u, 2u, 2u, 2u, 2u)
#define PPUTLIMPL_UINT_TRAIT_31u   DEC, 0x01F, 0, 4u, 5u, ()
#define PPUTLIMPL_UINT_TRAIT_30u   DEC, 0x01E, 0, 4u, 5u, (2u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_29u   DEC, 0x01D, 0, 4u, 5u, ()
#define PPUTLIMPL_UINT_TRAIT_28u   DEC, 0x01C, 0, 4u, 5u, (2u, 2u, 7u)
#define PPUTLIMPL_UINT_TRAIT_27u   DEC, 0x01B, 0, 4u, 5u, (3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_26u   DEC, 0x01A, 0, 4u, 5u, (2u, 13u)
#define PPUTLIMPL_UINT_TRAIT_25u   DEC, 0x019, 0, 4u, 5u, (5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_24u   DEC, 0x018, 0, 4u, 4u, (2u, 2u, 2u, 3u)
#define PPUTLIMPL_UINT_TRAIT_23u   DEC, 0x017, 0, 4u, 4u, ()
#define PPUTLIMPL_UINT_TRAIT_22u   DEC, 0x016, 0, 4u, 4u, (2u, 11u)
#define PPUTLIMPL_UINT_TRAIT_21u   DEC, 0x015, 0, 4u, 4u, (3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_20u   DEC, 0x014, 0, 4u, 4u, (2u, 2u, 5u)
#define PPUTLIMPL_UINT_TRAIT_19u   DEC, 0x013, 0, 4u, 4u, ()
#define PPUTLIMPL_UINT_TRAIT_18u   DEC, 0x012, 0, 4u, 4u, (2u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_17u   DEC, 0x011, 0, 4u, 4u, ()
#define PPUTLIMPL_UINT_TRAIT_16u   DEC, 0x010, 0, 4u, 4u, (2u, 2u, 2u, 2u)
#define PPUTLIMPL_UINT_TRAIT_15u   DEC, 0x00F, 0, 3u, 3u, (3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_14u   DEC, 0x00E, 0, 3u, 3u, (2u, 7u)
#define PPUTLIMPL_UINT_TRAIT_13u   DEC, 0x00D, 0, 3u, 3u, ()
#define PPUTLIMPL_UINT_TRAIT_12u   DEC, 0x00C, 0, 3u, 3u, (2u, 2u, 3u)
#define PPUTLIMPL_UINT_TRAIT_11u   DEC, 0x00B, 0, 3u, 3u, ()
#define PPUTLIMPL_UINT_TRAIT_10u   DEC, 0x00A, 0, 3u, 3u, (2u, 5u)
#define PPUTLIMPL_UINT_TRAIT_9u    DEC, 0x009, 0, 3u, 3u, (3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_8u    DEC, 0x008, 0, 3u, 2u, (2u, 2u, 2u)
#define PPUTLIMPL_UINT_TRAIT_7u    DEC, 0x007, 0, 2u, 2u, ()
#define PPUTLIMPL_UINT_TRAIT_6u    DEC, 0x006, 0, 2u, 2u, (2u, 3u)
#define PPUTLIMPL_UINT_TRAIT_5u    DEC, 0x005, 0, 2u, 2u, ()
#define PPUTLIMPL_UINT_TRAIT_4u    DEC, 0x004, 0, 2u, 2u, (2u, 2u)
#define PPUTLIMPL_UINT_TRAIT_3u    DEC, 0x003, 0, 1u, 1u, ()
#define PPUTLIMPL_UINT_TRAIT_2u    DEC, 0x002, 0, 1u, 1u, ()
#define PPUTLIMPL_UINT_TRAIT_1u    DEC, 0x001, 0, 0u, 1u, ()
#define PPUTLIMPL_UINT_TRAIT_0u    DEC, 0x000, 0, , 0u, ()

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

// vim: fdm=marker:fmr={{{,}}}

#endif

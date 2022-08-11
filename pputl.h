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
//    many high-level programming constructs  and 12-bit binary arithmetic    //
//    for both unsigned and signed two's complement integers.                 //
//                                                                            //
//    pputl algorithms  are built using a preprocessor syntax manipulation    //
//    technique for constructing inline recursive call stacks that execute    //
//    much faster than mutually-recursive methods.                            //
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
//    Modify the head of codegen/codegen.h  to configure the bit length or    //
//    naming preferences and run `make` to regenerate.                        //
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
//    pputl defines several types and uses type identification and casting    //
//    for control flow and error reporting. See the [type] section.           //
//                                                                            //
//     none: nothing                                                          //
//     some: <abstract> something; not nothing                                //
//      |-any: exactly one generic value                                      //
//         |-tup: anything in parentheses                                     //
//         |-atom: a generic non-tuple value                                  //
//            |- bool: a literal '1' or '0'                                   //
//            |- nybl: a literal uppercase hexadecimal digit (e.g. B)         //
//            |- uint: <abstract> an unsigned integer                         //
//            |   |- udec: an unsigned decimal integer (e.g. 42u)             //
//            |   |- uhex: an unsigned hex integer (e.g. 0x02Au)              //
//            |- int: <abstract> a signed integer                             //
//                |- idec: a positive 2s-complement decimal int (e.g. 353)    //
//                |- ihex: a signed hex integer (e.g. 0x161)                  //
//                                                                            //
//    Hexadecimal integers are always represented by fixed-length strings.    //
//    Negative ints cannot be represented in decimal  due to concatenation    //
//    restrictions. Arithmetic and bitwise functions attempt to cast their    //
//    results in the same form as their input, but will always return ihex    //
//    when an idec input becomes negative.  Decimal representations can be    //
//    generated for pasting using fmt.paste.                                  //
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
#define PTL_BUILD /* -> <c++ int> */ 20220811

/// [config.hex_length]
/// -------------------
/// the number of hex digits used by pputl integers.
/// hex representations are fixed at this length.
/// see the readme code generation section to configure.
#define PTL_HEX_LENGTH /* -> uint */ 2

/// [config.bit_length]
/// -------------------
/// the number of bits used by pputl integers.
/// based on the configured hex length.
#define PTL_BIT_LENGTH /* -> uint */ 8

/// [config.uint_max]
/// -----------------
/// the maximum value of a pputl unsigned int.
/// upper bound on the number of args for many pputl functions.
#define PTL_UINT_MAX /* -> uint */ 255u

/// [config.int_max]
/// ----------------
/// the maximum value of a pputl signed int.
#define PTL_INT_MAX /* -> int */ 127

/// [config.int_min]
/// ----------------
/// the minimum value of a pputl signed int.
/// only representable as hex. see type.int for details.
#define PTL_INT_MIN /* -> binary */ 0x80

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
#define PTL_IS_NONE(/* v: <unknown>... */...) /* -> 0|1 */ PPUTLIS_NONE_0##__VA_OPT__(1)

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
#define PTL_IS_SOME(/* v: <unknown>... */...) /* -> 0|1 */ PPUTLIS_SOME_0##__VA_OPT__(1)

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
#define PTL_IS_ANY(/* v: <unknown>... */...) /* -> 0|1 */ \
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

/// [traits.is_tup]
/// ---------------
/// [extends PTL_IS_ANY] detects if args is a tuple (any...).
///
/// PTL_IS_TUP()       // 0
/// PTL_IS_TUP(1, 2)   // 0
/// PTL_IS_TUP(())     // 1
/// PTL_IS_TUP((1, 2)) // 1
#define PTL_IS_TUP(/* v: <unknown>... */...) /* -> 0|1 */ \
  PTL_CAT(PPUTLIS_TUP_, PTL_IS_ANY(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_TUP_1           PPUTLIS_TUP_o
#define PPUTLIS_TUP_0           PPUTLIS_TUP_0_FAIL
#define PPUTLIS_TUP_0_FAIL(...) 0
#define PPUTLIS_TUP_o(any)      PTL_IS_NONE(PTL_EAT any)

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
#define PTL_IS_ATOM(/* v: <unknown>... */...) /* -> 0|1 */ \
  PTL_CAT(PPUTLIS_ATOM_, PTL_IS_ANY(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_ATOM_1           PPUTLIS_ATOM_o
#define PPUTLIS_ATOM_0           PPUTLIS_ATOM_0_FAIL
#define PPUTLIS_ATOM_0_FAIL(...) 0
#define PPUTLIS_ATOM_o(any)      PTL_CAT(PPUTLIS_ATOM_o_, PTL_IS_TUP(any))
#define PPUTLIS_ATOM_o_1         0
#define PPUTLIS_ATOM_o_0         1

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_bool]
/// ----------------
/// [extends PTL_IS_ATOM] detects if args is a bool (literal '1' or '0').
///
/// PTL_IS_BOOL()     // 0
/// PTL_IS_BOOL(0)    // 1
/// PTL_IS_BOOL(1)    // 1
/// PTL_IS_BOOL(1u)   // 0
/// PTL_IS_BOOL(0x00) // 0
/// PTL_IS_BOOL(0, 1) // 0
/// PTL_IS_BOOL((0))  // 0
#define PTL_IS_BOOL(/* v: <unknown>... */...) /* -> 0|1 */ \
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
#define PTL_IS_NYBL(/* v: <unknown>... */...) /* -> 0|1 */ \
  PTL_CAT(PPUTLIS_NYBL_, PTL_IS_ATOM(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_NYBL_1           PPUTLIS_NYBL_o
#define PPUTLIS_NYBL_0           PPUTLIS_NYBL_0_FAIL
#define PPUTLIS_NYBL_0_FAIL(...) 0
#define PPUTLIS_NYBL_o(atom)     PPUTLIMPL_NYBL_TRAIT(atom, IS)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_uint]
/// ----------------
/// [extends PTL_IS_ATOM] detects if args is an unsigned integer.
/// hex length is fixed at PTL_HEX_LENGTH (2).
///
/// PTL_IS_UINT()       // 0
/// PTL_IS_UINT(foo)    // 0
/// PTL_IS_UINT(0)      // 0
/// PTL_IS_UINT(0u)     // 1
/// PTL_IS_UINT(255)    // 0
/// PTL_IS_UINT(255u)   // 1
/// PTL_IS_UINT(0x00u)  // 1
/// PTL_IS_UINT(0xFF)   // 0
/// PTL_IS_UINT(0b110u) // 0
/// PTL_IS_UINT((), ()) // 0
#define PTL_IS_UINT(/* v: <unknown>... */...) /* -> 0|1 */ \
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
/// PTL_IS_UDEC(255)    // 0
/// PTL_IS_UDEC(255u)   // 1
/// PTL_IS_UDEC(0x00u)  // 0
/// PTL_IS_UDEC(0xFF)   // 0
/// PTL_IS_UDEC((), ()) // 0
#define PTL_IS_UDEC(/* v: <unknown>... */...) /* -> 0|1 */ \
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
/// hex length is fixed at PTL_HEX_LENGTH (2).
///
/// PTL_IS_UHEX(1)      // 0
/// PTL_IS_UHEX(1u)     // 0
/// PTL_IS_UHEX(0x00u)  // 1
/// PTL_IS_UHEX(0xFF)   // 0
/// PTL_IS_UHEX((), ()) // 0
#define PTL_IS_UHEX(/* v: <unknown>... */...) /* -> 0|1 */ \
  PTL_CAT(PPUTLIS_UHEX_, PTL_IS_UINT(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_UHEX_1           PPUTLIS_UHEX_o
#define PPUTLIS_UHEX_0           PPUTLIS_UHEX_0_FAIL
#define PPUTLIS_UHEX_0_FAIL(...) 0
#define PPUTLIS_UHEX_o(uint)     PTL_IS_NONE(PTL_CAT(PPUTLIS_UHEX_o_, PPUTLIMPL_UINT_TRAIT(uint, TYPE)))
#define PPUTLIS_UHEX_o_HEX

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_int]
/// ---------------
/// [extends PTL_IS_ATOM] detects if args is a signed integer.
/// hex length is fixed at PTL_HEX_LENGTH (2).
/// decimals must be smaller than PTL_INT_MAX (127).
/// negative values must be hex; concatenation with '-' is not supported.
///
/// PTL_IS_INT()       // 0
/// PTL_IS_INT(foo)    // 0
/// PTL_IS_INT(0)      // 1
/// PTL_IS_INT(0u)     // 0
/// PTL_IS_INT(255)    // 0
/// PTL_IS_INT(0x00u)  // 0
/// PTL_IS_INT(0xFF)   // 1
/// PTL_IS_INT(0b110u) // 0
/// PTL_IS_INT((), ()) // 0
#define PTL_IS_INT(/* v: <unknown>... */...) /* -> 0|1 */ \
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
/// decimals must be smaller than PTL_INT_MAX (127).
/// negative values must be hex; concatenation with '-' is not supported.
///
/// PTL_IS_IDEC(1)      // 1
/// PTL_IS_IDEC(1u)     // 0
/// PTL_IS_IDEC(127)    // 1
/// PTL_IS_IDEC(255)    // 0
/// PTL_IS_IDEC(0x00u)  // 0
/// PTL_IS_IDEC(0xFF)   // 0
/// PTL_IS_IDEC((), ()) // 0
#define PTL_IS_IDEC(/* v: <unknown>... */...) /* -> 0|1 */ \
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
/// hex length is fixed at PTL_HEX_LENGTH (2).
///
/// PTL_IS_IHEX(1)      // 0
/// PTL_IS_IHEX(1u)     // 0
/// PTL_IS_IHEX(0x00)   // 1
/// PTL_IS_IHEX(0xFF)   // 1
/// PTL_IS_IHEX(0xFFu)  // 0
/// PTL_IS_IHEX((), ()) // 0
#define PTL_IS_IHEX(/* v: <unknown>... */...) /* -> 0|1 */ \
  PTL_CAT(PPUTLIS_IHEX_, PTL_IS_INT(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_IHEX_1           PPUTLIS_IHEX_o
#define PPUTLIS_IHEX_0           PPUTLIS_IHEX_0_FAIL
#define PPUTLIS_IHEX_0_FAIL(...) 0
#define PPUTLIS_IHEX_o(int) \
  PTL_IS_NONE(PTL_CAT(PPUTLIS_IHEX_o_, PPUTLIMPL_UINT_TRAIT(int##u, TYPE)))
#define PPUTLIS_IHEX_o_HEX

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

/// [type.uint]
/// -----------
/// [inherits from PTL_ATOM] 8-bit unsigned integer type.
/// may be constructed from either unsigned or signed ints.
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
/// as unsigned is not allowed (e.g. 255 is not a valid integer).
///
/// PTL_UINT(0)     // 0u
/// PTL_UINT(1)     // 1u
/// PTL_UINT(2u)    // 2u
/// PTL_UINT(255u)  // 255u
/// PTL_UINT(0x00u) // 0x00u
/// PTL_UINT(0xFF)  // 0xFFu
#define PTL_UINT(/* n: uint|int */...) /* -> uint{n} */ \
  PPUTLUINT_o(PTL_ISTR([PTL_UINT] invalid integer : __VA_ARGS__), PTL_ATOM(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLUINT_o(e, atom) \
  PTL_CAT(PPUTLUINT_, PTL_CAT(PPUTLIS_UINT_o(atom), PPUTLIS_INT_o(atom)))(e, atom)
#define PPUTLUINT_10(e, u)   u
#define PPUTLUINT_01(e, i)   i##u
#define PPUTLUINT_00(e, ...) PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.udec]
/// -----------
/// [inherits from PTL_UINT] casts to the unsigned int decimal subtype.
///
/// PTL_UDEC(0x00u) // 0u
/// PTL_UDEC(1)     // 1u
/// PTL_UDEC(5)     // 5u
/// PTL_UDEC(0x05u) // 5u
/// PTL_UDEC(0xFFu) // 255u
/// PTL_UDEC(0xFF)  // 255u
#define PTL_UDEC(/* n: uint|int */...) /* -> udec{n} */ PPUTLUDEC_o(PTL_UINT(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLUDEC_o(uint) PTL_CAT(PPUTLUDEC_, PPUTLIS_UDEC_o(uint))(uint)
#define PPUTLUDEC_1(udec) udec
#define PPUTLUDEC_0(uhex) PPUTLIMPL_UINT_TRAIT(uhex, HUDEC)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.uhex]
/// -----------
/// [inherits from PTL_UINT] casts to the unsigned int hexadecimal subtype.
///
/// PTL_UHEX(0)     // 0x00u
/// PTL_UHEX(1)     // 0x01u
/// PTL_UHEX(5)     // 0x05u
/// PTL_UHEX(255u)  // 0xFFu
/// PTL_UHEX(0x00u) // 0x00u
/// PTL_UHEX(0x01u) // 0x01u
/// PTL_UHEX(0xFF)  // 0xFFu
#define PTL_UHEX(/* n: uint|int */...) /* -> uhex{n} */ PPUTLUHEX_o(PTL_UINT(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLUHEX_o(uint) PTL_CAT(PPUTLUHEX_, PPUTLIS_UHEX_o(uint))(uint)
#define PPUTLUHEX_1(uhex) uhex
#define PPUTLUHEX_0(udec) PTL_CAT(PPUTLIMPL_UINT_TRAIT(udec, DIHEX), u)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.int]
/// ----------
/// [inherits from PTL_ATOM] 8-bit signed integer type.
/// may be constructed from either unsigned or signed ints.
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
/// as unsigned is not allowed (e.g. 255 is not a valid integer).
///
/// PTL_INT(0)     // 0
/// PTL_INT(1u)    // 1
/// PTL_INT(0x00)  // 0x00
/// PTL_INT(127)   // 127
/// PTL_INT(0xFFu) // 0xFF
/// PTL_INT(255u)  // 0xFF
#define PTL_INT(/* n: uint|int */...) /* -> int{n} */ \
  PPUTLINT_o(PTL_ISTR([PTL_INT] invalid integer : __VA_ARGS__), PTL_ATOM(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLINT_o(e, atom) \
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
/// PTL_IDEC(0x00)  // 0
/// PTL_IDEC(0x01)  // 1
/// PTL_IDEC(0x05u) // 5
/// PTL_IDEC(0x7F)  // 127
/// PTL_IDEC(127)   // 127
#define PTL_IDEC(/* n: uint|int */...) /* -> idec{n} */               \
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
/// PTL_IHEX(0)    // 0x00
/// PTL_IHEX(1)    // 0x01
/// PTL_IHEX(5)    // 0x05
/// PTL_IHEX(255u) // 0xFF
/// PTL_IHEX(127u) // 0x7F
#define PTL_IHEX(/* n: uint|int */...) /* -> ihex{n} */ PPUTLIHEX_o(PTL_INT(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIHEX_o(int)    PTL_CAT(PPUTLIHEX_, PPUTLIS_IHEX_o(int))(int)
#define PPUTLIHEX_1(ihex)   ihex
#define PPUTLIHEX_0(idec)   PPUTLIHEX_0_o(idec##u)
#define PPUTLIHEX_0_o(udec) PPUTLIMPL_UINT_TRAIT(udec, DIHEX)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.typeof]
/// -------------
/// detects the value type. must be compatible with the ## operator.
/// literal 0 through 9 are considered ibase10 rather than bool or nybl.
///
/// returns one of:
/// - PTL_NONE
/// - PTL_SOME
/// - PTL_TUP
/// - PTL_IDEC
/// - PTL_IHEX
/// - PTL_UDEC
/// - PTL_UHEX
/// - PTL_NYBL
/// - PTL_ATOM
///
/// PTL_TYPEOF((foo))    // PTL_TUP
/// PTL_TYPEOF(0)        // PTL_IDEC
/// PTL_TYPEOF(0u)       // PTL_UDEC
/// PTL_TYPEOF(D)        // PTL_NYBL
/// PTL_TYPEOF(255)      // PTL_ATOM
/// PTL_TYPEOF(255u)     // PTL_UDEC
/// PTL_TYPEOF(0xFF)     // PTL_IHEX
/// PTL_TYPEOF(0xFFu)    // PTL_UHEX
/// PTL_TYPEOF(foo)      // PTL_ATOM
/// PTL_TYPEOF(foo, bar) // PTL_SOME
/// PTL_TYPEOF()         // PTL_NONE
#define PTL_TYPEOF(                                                                      \
    /* v: <unknown>... */...) /* -> ctor<none|some|tup|idec|ihex|udec|uhex|nybl|atom> */ \
  PTL_CAT(PPUTLTYPEOF_, PTL_IS_NONE(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

/// none
#define PPUTLTYPEOF_1(...)       PTL_NONE
/// !none
#define PPUTLTYPEOF_0(...)       PTL_CAT(PPUTLTYPEOF_0, PPUTLIS_ANY_o(__VA_ARGS__.))(__VA_ARGS__)
/// !none → any
#define PPUTLTYPEOF_01(any)      PTL_CAT(PPUTLTYPEOF_01, PPUTLIS_TUP_o(any))(any)
/// !none → any → tup
#define PPUTLTYPEOF_011(tup)     PTL_TUP
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
#define PPUTLIMPL_UINT_TRAIT_HNYBS(t, ud, id, n, ...)      /* -> tuple<nybl...> */ n
#define PPUTLIMPL_UINT_TRAIT_HIDEC(t, ud, id, ...)         /* -> ibase10 */ id
#define PPUTLIMPL_UINT_TRAIT_HUDEC(t, ud, ...)             /* -> ubase10 */ ud
#define PPUTLIMPL_UINT_TRAIT_TYPE(t, ...)                  /* -> HEX|DEC */ t

#define PPUTLIMPL_UINT_TRAIT_IS(_, ...) /* -> bool */ PPUTLIMPL_UINT_TRAIT_IS_0##__VA_OPT__(1)

#define PPUTLIMPL_UINT_TRAIT_IS_01 1
#define PPUTLIMPL_UINT_TRAIT_IS_0  0

/// type, unsigned decimal, signed decimal, nybls, bitnot
#define PPUTLIMPL_UINT_TRAIT_0xFFu HEX, 255u, -1, (F, F), 0x00u
#define PPUTLIMPL_UINT_TRAIT_0xFEu HEX, 254u, -2, (F, E), 0x01u
#define PPUTLIMPL_UINT_TRAIT_0xFDu HEX, 253u, -3, (F, D), 0x02u
#define PPUTLIMPL_UINT_TRAIT_0xFCu HEX, 252u, -4, (F, C), 0x03u
#define PPUTLIMPL_UINT_TRAIT_0xFBu HEX, 251u, -5, (F, B), 0x04u
#define PPUTLIMPL_UINT_TRAIT_0xFAu HEX, 250u, -6, (F, A), 0x05u
#define PPUTLIMPL_UINT_TRAIT_0xF9u HEX, 249u, -7, (F, 9), 0x06u
#define PPUTLIMPL_UINT_TRAIT_0xF8u HEX, 248u, -8, (F, 8), 0x07u
#define PPUTLIMPL_UINT_TRAIT_0xF7u HEX, 247u, -9, (F, 7), 0x08u
#define PPUTLIMPL_UINT_TRAIT_0xF6u HEX, 246u, -10, (F, 6), 0x09u
#define PPUTLIMPL_UINT_TRAIT_0xF5u HEX, 245u, -11, (F, 5), 0x0Au
#define PPUTLIMPL_UINT_TRAIT_0xF4u HEX, 244u, -12, (F, 4), 0x0Bu
#define PPUTLIMPL_UINT_TRAIT_0xF3u HEX, 243u, -13, (F, 3), 0x0Cu
#define PPUTLIMPL_UINT_TRAIT_0xF2u HEX, 242u, -14, (F, 2), 0x0Du
#define PPUTLIMPL_UINT_TRAIT_0xF1u HEX, 241u, -15, (F, 1), 0x0Eu
#define PPUTLIMPL_UINT_TRAIT_0xF0u HEX, 240u, -16, (F, 0), 0x0Fu
#define PPUTLIMPL_UINT_TRAIT_0xEFu HEX, 239u, -17, (E, F), 0x10u
#define PPUTLIMPL_UINT_TRAIT_0xEEu HEX, 238u, -18, (E, E), 0x11u
#define PPUTLIMPL_UINT_TRAIT_0xEDu HEX, 237u, -19, (E, D), 0x12u
#define PPUTLIMPL_UINT_TRAIT_0xECu HEX, 236u, -20, (E, C), 0x13u
#define PPUTLIMPL_UINT_TRAIT_0xEBu HEX, 235u, -21, (E, B), 0x14u
#define PPUTLIMPL_UINT_TRAIT_0xEAu HEX, 234u, -22, (E, A), 0x15u
#define PPUTLIMPL_UINT_TRAIT_0xE9u HEX, 233u, -23, (E, 9), 0x16u
#define PPUTLIMPL_UINT_TRAIT_0xE8u HEX, 232u, -24, (E, 8), 0x17u
#define PPUTLIMPL_UINT_TRAIT_0xE7u HEX, 231u, -25, (E, 7), 0x18u
#define PPUTLIMPL_UINT_TRAIT_0xE6u HEX, 230u, -26, (E, 6), 0x19u
#define PPUTLIMPL_UINT_TRAIT_0xE5u HEX, 229u, -27, (E, 5), 0x1Au
#define PPUTLIMPL_UINT_TRAIT_0xE4u HEX, 228u, -28, (E, 4), 0x1Bu
#define PPUTLIMPL_UINT_TRAIT_0xE3u HEX, 227u, -29, (E, 3), 0x1Cu
#define PPUTLIMPL_UINT_TRAIT_0xE2u HEX, 226u, -30, (E, 2), 0x1Du
#define PPUTLIMPL_UINT_TRAIT_0xE1u HEX, 225u, -31, (E, 1), 0x1Eu
#define PPUTLIMPL_UINT_TRAIT_0xE0u HEX, 224u, -32, (E, 0), 0x1Fu
#define PPUTLIMPL_UINT_TRAIT_0xDFu HEX, 223u, -33, (D, F), 0x20u
#define PPUTLIMPL_UINT_TRAIT_0xDEu HEX, 222u, -34, (D, E), 0x21u
#define PPUTLIMPL_UINT_TRAIT_0xDDu HEX, 221u, -35, (D, D), 0x22u
#define PPUTLIMPL_UINT_TRAIT_0xDCu HEX, 220u, -36, (D, C), 0x23u
#define PPUTLIMPL_UINT_TRAIT_0xDBu HEX, 219u, -37, (D, B), 0x24u
#define PPUTLIMPL_UINT_TRAIT_0xDAu HEX, 218u, -38, (D, A), 0x25u
#define PPUTLIMPL_UINT_TRAIT_0xD9u HEX, 217u, -39, (D, 9), 0x26u
#define PPUTLIMPL_UINT_TRAIT_0xD8u HEX, 216u, -40, (D, 8), 0x27u
#define PPUTLIMPL_UINT_TRAIT_0xD7u HEX, 215u, -41, (D, 7), 0x28u
#define PPUTLIMPL_UINT_TRAIT_0xD6u HEX, 214u, -42, (D, 6), 0x29u
#define PPUTLIMPL_UINT_TRAIT_0xD5u HEX, 213u, -43, (D, 5), 0x2Au
#define PPUTLIMPL_UINT_TRAIT_0xD4u HEX, 212u, -44, (D, 4), 0x2Bu
#define PPUTLIMPL_UINT_TRAIT_0xD3u HEX, 211u, -45, (D, 3), 0x2Cu
#define PPUTLIMPL_UINT_TRAIT_0xD2u HEX, 210u, -46, (D, 2), 0x2Du
#define PPUTLIMPL_UINT_TRAIT_0xD1u HEX, 209u, -47, (D, 1), 0x2Eu
#define PPUTLIMPL_UINT_TRAIT_0xD0u HEX, 208u, -48, (D, 0), 0x2Fu
#define PPUTLIMPL_UINT_TRAIT_0xCFu HEX, 207u, -49, (C, F), 0x30u
#define PPUTLIMPL_UINT_TRAIT_0xCEu HEX, 206u, -50, (C, E), 0x31u
#define PPUTLIMPL_UINT_TRAIT_0xCDu HEX, 205u, -51, (C, D), 0x32u
#define PPUTLIMPL_UINT_TRAIT_0xCCu HEX, 204u, -52, (C, C), 0x33u
#define PPUTLIMPL_UINT_TRAIT_0xCBu HEX, 203u, -53, (C, B), 0x34u
#define PPUTLIMPL_UINT_TRAIT_0xCAu HEX, 202u, -54, (C, A), 0x35u
#define PPUTLIMPL_UINT_TRAIT_0xC9u HEX, 201u, -55, (C, 9), 0x36u
#define PPUTLIMPL_UINT_TRAIT_0xC8u HEX, 200u, -56, (C, 8), 0x37u
#define PPUTLIMPL_UINT_TRAIT_0xC7u HEX, 199u, -57, (C, 7), 0x38u
#define PPUTLIMPL_UINT_TRAIT_0xC6u HEX, 198u, -58, (C, 6), 0x39u
#define PPUTLIMPL_UINT_TRAIT_0xC5u HEX, 197u, -59, (C, 5), 0x3Au
#define PPUTLIMPL_UINT_TRAIT_0xC4u HEX, 196u, -60, (C, 4), 0x3Bu
#define PPUTLIMPL_UINT_TRAIT_0xC3u HEX, 195u, -61, (C, 3), 0x3Cu
#define PPUTLIMPL_UINT_TRAIT_0xC2u HEX, 194u, -62, (C, 2), 0x3Du
#define PPUTLIMPL_UINT_TRAIT_0xC1u HEX, 193u, -63, (C, 1), 0x3Eu
#define PPUTLIMPL_UINT_TRAIT_0xC0u HEX, 192u, -64, (C, 0), 0x3Fu
#define PPUTLIMPL_UINT_TRAIT_0xBFu HEX, 191u, -65, (B, F), 0x40u
#define PPUTLIMPL_UINT_TRAIT_0xBEu HEX, 190u, -66, (B, E), 0x41u
#define PPUTLIMPL_UINT_TRAIT_0xBDu HEX, 189u, -67, (B, D), 0x42u
#define PPUTLIMPL_UINT_TRAIT_0xBCu HEX, 188u, -68, (B, C), 0x43u
#define PPUTLIMPL_UINT_TRAIT_0xBBu HEX, 187u, -69, (B, B), 0x44u
#define PPUTLIMPL_UINT_TRAIT_0xBAu HEX, 186u, -70, (B, A), 0x45u
#define PPUTLIMPL_UINT_TRAIT_0xB9u HEX, 185u, -71, (B, 9), 0x46u
#define PPUTLIMPL_UINT_TRAIT_0xB8u HEX, 184u, -72, (B, 8), 0x47u
#define PPUTLIMPL_UINT_TRAIT_0xB7u HEX, 183u, -73, (B, 7), 0x48u
#define PPUTLIMPL_UINT_TRAIT_0xB6u HEX, 182u, -74, (B, 6), 0x49u
#define PPUTLIMPL_UINT_TRAIT_0xB5u HEX, 181u, -75, (B, 5), 0x4Au
#define PPUTLIMPL_UINT_TRAIT_0xB4u HEX, 180u, -76, (B, 4), 0x4Bu
#define PPUTLIMPL_UINT_TRAIT_0xB3u HEX, 179u, -77, (B, 3), 0x4Cu
#define PPUTLIMPL_UINT_TRAIT_0xB2u HEX, 178u, -78, (B, 2), 0x4Du
#define PPUTLIMPL_UINT_TRAIT_0xB1u HEX, 177u, -79, (B, 1), 0x4Eu
#define PPUTLIMPL_UINT_TRAIT_0xB0u HEX, 176u, -80, (B, 0), 0x4Fu
#define PPUTLIMPL_UINT_TRAIT_0xAFu HEX, 175u, -81, (A, F), 0x50u
#define PPUTLIMPL_UINT_TRAIT_0xAEu HEX, 174u, -82, (A, E), 0x51u
#define PPUTLIMPL_UINT_TRAIT_0xADu HEX, 173u, -83, (A, D), 0x52u
#define PPUTLIMPL_UINT_TRAIT_0xACu HEX, 172u, -84, (A, C), 0x53u
#define PPUTLIMPL_UINT_TRAIT_0xABu HEX, 171u, -85, (A, B), 0x54u
#define PPUTLIMPL_UINT_TRAIT_0xAAu HEX, 170u, -86, (A, A), 0x55u
#define PPUTLIMPL_UINT_TRAIT_0xA9u HEX, 169u, -87, (A, 9), 0x56u
#define PPUTLIMPL_UINT_TRAIT_0xA8u HEX, 168u, -88, (A, 8), 0x57u
#define PPUTLIMPL_UINT_TRAIT_0xA7u HEX, 167u, -89, (A, 7), 0x58u
#define PPUTLIMPL_UINT_TRAIT_0xA6u HEX, 166u, -90, (A, 6), 0x59u
#define PPUTLIMPL_UINT_TRAIT_0xA5u HEX, 165u, -91, (A, 5), 0x5Au
#define PPUTLIMPL_UINT_TRAIT_0xA4u HEX, 164u, -92, (A, 4), 0x5Bu
#define PPUTLIMPL_UINT_TRAIT_0xA3u HEX, 163u, -93, (A, 3), 0x5Cu
#define PPUTLIMPL_UINT_TRAIT_0xA2u HEX, 162u, -94, (A, 2), 0x5Du
#define PPUTLIMPL_UINT_TRAIT_0xA1u HEX, 161u, -95, (A, 1), 0x5Eu
#define PPUTLIMPL_UINT_TRAIT_0xA0u HEX, 160u, -96, (A, 0), 0x5Fu
#define PPUTLIMPL_UINT_TRAIT_0x9Fu HEX, 159u, -97, (9, F), 0x60u
#define PPUTLIMPL_UINT_TRAIT_0x9Eu HEX, 158u, -98, (9, E), 0x61u
#define PPUTLIMPL_UINT_TRAIT_0x9Du HEX, 157u, -99, (9, D), 0x62u
#define PPUTLIMPL_UINT_TRAIT_0x9Cu HEX, 156u, -100, (9, C), 0x63u
#define PPUTLIMPL_UINT_TRAIT_0x9Bu HEX, 155u, -101, (9, B), 0x64u
#define PPUTLIMPL_UINT_TRAIT_0x9Au HEX, 154u, -102, (9, A), 0x65u
#define PPUTLIMPL_UINT_TRAIT_0x99u HEX, 153u, -103, (9, 9), 0x66u
#define PPUTLIMPL_UINT_TRAIT_0x98u HEX, 152u, -104, (9, 8), 0x67u
#define PPUTLIMPL_UINT_TRAIT_0x97u HEX, 151u, -105, (9, 7), 0x68u
#define PPUTLIMPL_UINT_TRAIT_0x96u HEX, 150u, -106, (9, 6), 0x69u
#define PPUTLIMPL_UINT_TRAIT_0x95u HEX, 149u, -107, (9, 5), 0x6Au
#define PPUTLIMPL_UINT_TRAIT_0x94u HEX, 148u, -108, (9, 4), 0x6Bu
#define PPUTLIMPL_UINT_TRAIT_0x93u HEX, 147u, -109, (9, 3), 0x6Cu
#define PPUTLIMPL_UINT_TRAIT_0x92u HEX, 146u, -110, (9, 2), 0x6Du
#define PPUTLIMPL_UINT_TRAIT_0x91u HEX, 145u, -111, (9, 1), 0x6Eu
#define PPUTLIMPL_UINT_TRAIT_0x90u HEX, 144u, -112, (9, 0), 0x6Fu
#define PPUTLIMPL_UINT_TRAIT_0x8Fu HEX, 143u, -113, (8, F), 0x70u
#define PPUTLIMPL_UINT_TRAIT_0x8Eu HEX, 142u, -114, (8, E), 0x71u
#define PPUTLIMPL_UINT_TRAIT_0x8Du HEX, 141u, -115, (8, D), 0x72u
#define PPUTLIMPL_UINT_TRAIT_0x8Cu HEX, 140u, -116, (8, C), 0x73u
#define PPUTLIMPL_UINT_TRAIT_0x8Bu HEX, 139u, -117, (8, B), 0x74u
#define PPUTLIMPL_UINT_TRAIT_0x8Au HEX, 138u, -118, (8, A), 0x75u
#define PPUTLIMPL_UINT_TRAIT_0x89u HEX, 137u, -119, (8, 9), 0x76u
#define PPUTLIMPL_UINT_TRAIT_0x88u HEX, 136u, -120, (8, 8), 0x77u
#define PPUTLIMPL_UINT_TRAIT_0x87u HEX, 135u, -121, (8, 7), 0x78u
#define PPUTLIMPL_UINT_TRAIT_0x86u HEX, 134u, -122, (8, 6), 0x79u
#define PPUTLIMPL_UINT_TRAIT_0x85u HEX, 133u, -123, (8, 5), 0x7Au
#define PPUTLIMPL_UINT_TRAIT_0x84u HEX, 132u, -124, (8, 4), 0x7Bu
#define PPUTLIMPL_UINT_TRAIT_0x83u HEX, 131u, -125, (8, 3), 0x7Cu
#define PPUTLIMPL_UINT_TRAIT_0x82u HEX, 130u, -126, (8, 2), 0x7Du
#define PPUTLIMPL_UINT_TRAIT_0x81u HEX, 129u, -127, (8, 1), 0x7Eu
#define PPUTLIMPL_UINT_TRAIT_0x80u HEX, 128u, -128, (8, 0), 0x7Fu
#define PPUTLIMPL_UINT_TRAIT_0x7Fu HEX, 127u, 127, (7, F), 0x80u
#define PPUTLIMPL_UINT_TRAIT_0x7Eu HEX, 126u, 126, (7, E), 0x81u
#define PPUTLIMPL_UINT_TRAIT_0x7Du HEX, 125u, 125, (7, D), 0x82u
#define PPUTLIMPL_UINT_TRAIT_0x7Cu HEX, 124u, 124, (7, C), 0x83u
#define PPUTLIMPL_UINT_TRAIT_0x7Bu HEX, 123u, 123, (7, B), 0x84u
#define PPUTLIMPL_UINT_TRAIT_0x7Au HEX, 122u, 122, (7, A), 0x85u
#define PPUTLIMPL_UINT_TRAIT_0x79u HEX, 121u, 121, (7, 9), 0x86u
#define PPUTLIMPL_UINT_TRAIT_0x78u HEX, 120u, 120, (7, 8), 0x87u
#define PPUTLIMPL_UINT_TRAIT_0x77u HEX, 119u, 119, (7, 7), 0x88u
#define PPUTLIMPL_UINT_TRAIT_0x76u HEX, 118u, 118, (7, 6), 0x89u
#define PPUTLIMPL_UINT_TRAIT_0x75u HEX, 117u, 117, (7, 5), 0x8Au
#define PPUTLIMPL_UINT_TRAIT_0x74u HEX, 116u, 116, (7, 4), 0x8Bu
#define PPUTLIMPL_UINT_TRAIT_0x73u HEX, 115u, 115, (7, 3), 0x8Cu
#define PPUTLIMPL_UINT_TRAIT_0x72u HEX, 114u, 114, (7, 2), 0x8Du
#define PPUTLIMPL_UINT_TRAIT_0x71u HEX, 113u, 113, (7, 1), 0x8Eu
#define PPUTLIMPL_UINT_TRAIT_0x70u HEX, 112u, 112, (7, 0), 0x8Fu
#define PPUTLIMPL_UINT_TRAIT_0x6Fu HEX, 111u, 111, (6, F), 0x90u
#define PPUTLIMPL_UINT_TRAIT_0x6Eu HEX, 110u, 110, (6, E), 0x91u
#define PPUTLIMPL_UINT_TRAIT_0x6Du HEX, 109u, 109, (6, D), 0x92u
#define PPUTLIMPL_UINT_TRAIT_0x6Cu HEX, 108u, 108, (6, C), 0x93u
#define PPUTLIMPL_UINT_TRAIT_0x6Bu HEX, 107u, 107, (6, B), 0x94u
#define PPUTLIMPL_UINT_TRAIT_0x6Au HEX, 106u, 106, (6, A), 0x95u
#define PPUTLIMPL_UINT_TRAIT_0x69u HEX, 105u, 105, (6, 9), 0x96u
#define PPUTLIMPL_UINT_TRAIT_0x68u HEX, 104u, 104, (6, 8), 0x97u
#define PPUTLIMPL_UINT_TRAIT_0x67u HEX, 103u, 103, (6, 7), 0x98u
#define PPUTLIMPL_UINT_TRAIT_0x66u HEX, 102u, 102, (6, 6), 0x99u
#define PPUTLIMPL_UINT_TRAIT_0x65u HEX, 101u, 101, (6, 5), 0x9Au
#define PPUTLIMPL_UINT_TRAIT_0x64u HEX, 100u, 100, (6, 4), 0x9Bu
#define PPUTLIMPL_UINT_TRAIT_0x63u HEX, 99u, 99, (6, 3), 0x9Cu
#define PPUTLIMPL_UINT_TRAIT_0x62u HEX, 98u, 98, (6, 2), 0x9Du
#define PPUTLIMPL_UINT_TRAIT_0x61u HEX, 97u, 97, (6, 1), 0x9Eu
#define PPUTLIMPL_UINT_TRAIT_0x60u HEX, 96u, 96, (6, 0), 0x9Fu
#define PPUTLIMPL_UINT_TRAIT_0x5Fu HEX, 95u, 95, (5, F), 0xA0u
#define PPUTLIMPL_UINT_TRAIT_0x5Eu HEX, 94u, 94, (5, E), 0xA1u
#define PPUTLIMPL_UINT_TRAIT_0x5Du HEX, 93u, 93, (5, D), 0xA2u
#define PPUTLIMPL_UINT_TRAIT_0x5Cu HEX, 92u, 92, (5, C), 0xA3u
#define PPUTLIMPL_UINT_TRAIT_0x5Bu HEX, 91u, 91, (5, B), 0xA4u
#define PPUTLIMPL_UINT_TRAIT_0x5Au HEX, 90u, 90, (5, A), 0xA5u
#define PPUTLIMPL_UINT_TRAIT_0x59u HEX, 89u, 89, (5, 9), 0xA6u
#define PPUTLIMPL_UINT_TRAIT_0x58u HEX, 88u, 88, (5, 8), 0xA7u
#define PPUTLIMPL_UINT_TRAIT_0x57u HEX, 87u, 87, (5, 7), 0xA8u
#define PPUTLIMPL_UINT_TRAIT_0x56u HEX, 86u, 86, (5, 6), 0xA9u
#define PPUTLIMPL_UINT_TRAIT_0x55u HEX, 85u, 85, (5, 5), 0xAAu
#define PPUTLIMPL_UINT_TRAIT_0x54u HEX, 84u, 84, (5, 4), 0xABu
#define PPUTLIMPL_UINT_TRAIT_0x53u HEX, 83u, 83, (5, 3), 0xACu
#define PPUTLIMPL_UINT_TRAIT_0x52u HEX, 82u, 82, (5, 2), 0xADu
#define PPUTLIMPL_UINT_TRAIT_0x51u HEX, 81u, 81, (5, 1), 0xAEu
#define PPUTLIMPL_UINT_TRAIT_0x50u HEX, 80u, 80, (5, 0), 0xAFu
#define PPUTLIMPL_UINT_TRAIT_0x4Fu HEX, 79u, 79, (4, F), 0xB0u
#define PPUTLIMPL_UINT_TRAIT_0x4Eu HEX, 78u, 78, (4, E), 0xB1u
#define PPUTLIMPL_UINT_TRAIT_0x4Du HEX, 77u, 77, (4, D), 0xB2u
#define PPUTLIMPL_UINT_TRAIT_0x4Cu HEX, 76u, 76, (4, C), 0xB3u
#define PPUTLIMPL_UINT_TRAIT_0x4Bu HEX, 75u, 75, (4, B), 0xB4u
#define PPUTLIMPL_UINT_TRAIT_0x4Au HEX, 74u, 74, (4, A), 0xB5u
#define PPUTLIMPL_UINT_TRAIT_0x49u HEX, 73u, 73, (4, 9), 0xB6u
#define PPUTLIMPL_UINT_TRAIT_0x48u HEX, 72u, 72, (4, 8), 0xB7u
#define PPUTLIMPL_UINT_TRAIT_0x47u HEX, 71u, 71, (4, 7), 0xB8u
#define PPUTLIMPL_UINT_TRAIT_0x46u HEX, 70u, 70, (4, 6), 0xB9u
#define PPUTLIMPL_UINT_TRAIT_0x45u HEX, 69u, 69, (4, 5), 0xBAu
#define PPUTLIMPL_UINT_TRAIT_0x44u HEX, 68u, 68, (4, 4), 0xBBu
#define PPUTLIMPL_UINT_TRAIT_0x43u HEX, 67u, 67, (4, 3), 0xBCu
#define PPUTLIMPL_UINT_TRAIT_0x42u HEX, 66u, 66, (4, 2), 0xBDu
#define PPUTLIMPL_UINT_TRAIT_0x41u HEX, 65u, 65, (4, 1), 0xBEu
#define PPUTLIMPL_UINT_TRAIT_0x40u HEX, 64u, 64, (4, 0), 0xBFu
#define PPUTLIMPL_UINT_TRAIT_0x3Fu HEX, 63u, 63, (3, F), 0xC0u
#define PPUTLIMPL_UINT_TRAIT_0x3Eu HEX, 62u, 62, (3, E), 0xC1u
#define PPUTLIMPL_UINT_TRAIT_0x3Du HEX, 61u, 61, (3, D), 0xC2u
#define PPUTLIMPL_UINT_TRAIT_0x3Cu HEX, 60u, 60, (3, C), 0xC3u
#define PPUTLIMPL_UINT_TRAIT_0x3Bu HEX, 59u, 59, (3, B), 0xC4u
#define PPUTLIMPL_UINT_TRAIT_0x3Au HEX, 58u, 58, (3, A), 0xC5u
#define PPUTLIMPL_UINT_TRAIT_0x39u HEX, 57u, 57, (3, 9), 0xC6u
#define PPUTLIMPL_UINT_TRAIT_0x38u HEX, 56u, 56, (3, 8), 0xC7u
#define PPUTLIMPL_UINT_TRAIT_0x37u HEX, 55u, 55, (3, 7), 0xC8u
#define PPUTLIMPL_UINT_TRAIT_0x36u HEX, 54u, 54, (3, 6), 0xC9u
#define PPUTLIMPL_UINT_TRAIT_0x35u HEX, 53u, 53, (3, 5), 0xCAu
#define PPUTLIMPL_UINT_TRAIT_0x34u HEX, 52u, 52, (3, 4), 0xCBu
#define PPUTLIMPL_UINT_TRAIT_0x33u HEX, 51u, 51, (3, 3), 0xCCu
#define PPUTLIMPL_UINT_TRAIT_0x32u HEX, 50u, 50, (3, 2), 0xCDu
#define PPUTLIMPL_UINT_TRAIT_0x31u HEX, 49u, 49, (3, 1), 0xCEu
#define PPUTLIMPL_UINT_TRAIT_0x30u HEX, 48u, 48, (3, 0), 0xCFu
#define PPUTLIMPL_UINT_TRAIT_0x2Fu HEX, 47u, 47, (2, F), 0xD0u
#define PPUTLIMPL_UINT_TRAIT_0x2Eu HEX, 46u, 46, (2, E), 0xD1u
#define PPUTLIMPL_UINT_TRAIT_0x2Du HEX, 45u, 45, (2, D), 0xD2u
#define PPUTLIMPL_UINT_TRAIT_0x2Cu HEX, 44u, 44, (2, C), 0xD3u
#define PPUTLIMPL_UINT_TRAIT_0x2Bu HEX, 43u, 43, (2, B), 0xD4u
#define PPUTLIMPL_UINT_TRAIT_0x2Au HEX, 42u, 42, (2, A), 0xD5u
#define PPUTLIMPL_UINT_TRAIT_0x29u HEX, 41u, 41, (2, 9), 0xD6u
#define PPUTLIMPL_UINT_TRAIT_0x28u HEX, 40u, 40, (2, 8), 0xD7u
#define PPUTLIMPL_UINT_TRAIT_0x27u HEX, 39u, 39, (2, 7), 0xD8u
#define PPUTLIMPL_UINT_TRAIT_0x26u HEX, 38u, 38, (2, 6), 0xD9u
#define PPUTLIMPL_UINT_TRAIT_0x25u HEX, 37u, 37, (2, 5), 0xDAu
#define PPUTLIMPL_UINT_TRAIT_0x24u HEX, 36u, 36, (2, 4), 0xDBu
#define PPUTLIMPL_UINT_TRAIT_0x23u HEX, 35u, 35, (2, 3), 0xDCu
#define PPUTLIMPL_UINT_TRAIT_0x22u HEX, 34u, 34, (2, 2), 0xDDu
#define PPUTLIMPL_UINT_TRAIT_0x21u HEX, 33u, 33, (2, 1), 0xDEu
#define PPUTLIMPL_UINT_TRAIT_0x20u HEX, 32u, 32, (2, 0), 0xDFu
#define PPUTLIMPL_UINT_TRAIT_0x1Fu HEX, 31u, 31, (1, F), 0xE0u
#define PPUTLIMPL_UINT_TRAIT_0x1Eu HEX, 30u, 30, (1, E), 0xE1u
#define PPUTLIMPL_UINT_TRAIT_0x1Du HEX, 29u, 29, (1, D), 0xE2u
#define PPUTLIMPL_UINT_TRAIT_0x1Cu HEX, 28u, 28, (1, C), 0xE3u
#define PPUTLIMPL_UINT_TRAIT_0x1Bu HEX, 27u, 27, (1, B), 0xE4u
#define PPUTLIMPL_UINT_TRAIT_0x1Au HEX, 26u, 26, (1, A), 0xE5u
#define PPUTLIMPL_UINT_TRAIT_0x19u HEX, 25u, 25, (1, 9), 0xE6u
#define PPUTLIMPL_UINT_TRAIT_0x18u HEX, 24u, 24, (1, 8), 0xE7u
#define PPUTLIMPL_UINT_TRAIT_0x17u HEX, 23u, 23, (1, 7), 0xE8u
#define PPUTLIMPL_UINT_TRAIT_0x16u HEX, 22u, 22, (1, 6), 0xE9u
#define PPUTLIMPL_UINT_TRAIT_0x15u HEX, 21u, 21, (1, 5), 0xEAu
#define PPUTLIMPL_UINT_TRAIT_0x14u HEX, 20u, 20, (1, 4), 0xEBu
#define PPUTLIMPL_UINT_TRAIT_0x13u HEX, 19u, 19, (1, 3), 0xECu
#define PPUTLIMPL_UINT_TRAIT_0x12u HEX, 18u, 18, (1, 2), 0xEDu
#define PPUTLIMPL_UINT_TRAIT_0x11u HEX, 17u, 17, (1, 1), 0xEEu
#define PPUTLIMPL_UINT_TRAIT_0x10u HEX, 16u, 16, (1, 0), 0xEFu
#define PPUTLIMPL_UINT_TRAIT_0x0Fu HEX, 15u, 15, (0, F), 0xF0u
#define PPUTLIMPL_UINT_TRAIT_0x0Eu HEX, 14u, 14, (0, E), 0xF1u
#define PPUTLIMPL_UINT_TRAIT_0x0Du HEX, 13u, 13, (0, D), 0xF2u
#define PPUTLIMPL_UINT_TRAIT_0x0Cu HEX, 12u, 12, (0, C), 0xF3u
#define PPUTLIMPL_UINT_TRAIT_0x0Bu HEX, 11u, 11, (0, B), 0xF4u
#define PPUTLIMPL_UINT_TRAIT_0x0Au HEX, 10u, 10, (0, A), 0xF5u
#define PPUTLIMPL_UINT_TRAIT_0x09u HEX, 9u, 9, (0, 9), 0xF6u
#define PPUTLIMPL_UINT_TRAIT_0x08u HEX, 8u, 8, (0, 8), 0xF7u
#define PPUTLIMPL_UINT_TRAIT_0x07u HEX, 7u, 7, (0, 7), 0xF8u
#define PPUTLIMPL_UINT_TRAIT_0x06u HEX, 6u, 6, (0, 6), 0xF9u
#define PPUTLIMPL_UINT_TRAIT_0x05u HEX, 5u, 5, (0, 5), 0xFAu
#define PPUTLIMPL_UINT_TRAIT_0x04u HEX, 4u, 4, (0, 4), 0xFBu
#define PPUTLIMPL_UINT_TRAIT_0x03u HEX, 3u, 3, (0, 3), 0xFCu
#define PPUTLIMPL_UINT_TRAIT_0x02u HEX, 2u, 2, (0, 2), 0xFDu
#define PPUTLIMPL_UINT_TRAIT_0x01u HEX, 1u, 1, (0, 1), 0xFEu
#define PPUTLIMPL_UINT_TRAIT_0x00u HEX, 0u, 0, (0, 0), 0xFFu

/// type, signed hex, signed is negative, log2, sqrt, factors
#define PPUTLIMPL_UINT_TRAIT_255u DEC, 0xFF, 1, 7u, 15u, (3u, 5u, 17u)
#define PPUTLIMPL_UINT_TRAIT_254u DEC, 0xFE, 1, 7u, 15u, (2u, 127u)
#define PPUTLIMPL_UINT_TRAIT_253u DEC, 0xFD, 1, 7u, 15u, (11u, 23u)
#define PPUTLIMPL_UINT_TRAIT_252u DEC, 0xFC, 1, 7u, 15u, (2u, 2u, 3u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_251u DEC, 0xFB, 1, 7u, 15u, ()
#define PPUTLIMPL_UINT_TRAIT_250u DEC, 0xFA, 1, 7u, 15u, (2u, 5u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_249u DEC, 0xF9, 1, 7u, 15u, (3u, 83u)
#define PPUTLIMPL_UINT_TRAIT_248u DEC, 0xF8, 1, 7u, 15u, (2u, 2u, 2u, 31u)
#define PPUTLIMPL_UINT_TRAIT_247u DEC, 0xF7, 1, 7u, 15u, (13u, 19u)
#define PPUTLIMPL_UINT_TRAIT_246u DEC, 0xF6, 1, 7u, 15u, (2u, 3u, 41u)
#define PPUTLIMPL_UINT_TRAIT_245u DEC, 0xF5, 1, 7u, 15u, (5u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_244u DEC, 0xF4, 1, 7u, 15u, (2u, 2u, 61u)
#define PPUTLIMPL_UINT_TRAIT_243u DEC, 0xF3, 1, 7u, 15u, (3u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_242u DEC, 0xF2, 1, 7u, 15u, (2u, 11u, 11u)
#define PPUTLIMPL_UINT_TRAIT_241u DEC, 0xF1, 1, 7u, 15u, ()
#define PPUTLIMPL_UINT_TRAIT_240u DEC, 0xF0, 1, 7u, 15u, (2u, 2u, 2u, 2u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_239u DEC, 0xEF, 1, 7u, 15u, ()
#define PPUTLIMPL_UINT_TRAIT_238u DEC, 0xEE, 1, 7u, 15u, (2u, 7u, 17u)
#define PPUTLIMPL_UINT_TRAIT_237u DEC, 0xED, 1, 7u, 15u, (3u, 79u)
#define PPUTLIMPL_UINT_TRAIT_236u DEC, 0xEC, 1, 7u, 15u, (2u, 2u, 59u)
#define PPUTLIMPL_UINT_TRAIT_235u DEC, 0xEB, 1, 7u, 15u, (5u, 47u)
#define PPUTLIMPL_UINT_TRAIT_234u DEC, 0xEA, 1, 7u, 15u, (2u, 3u, 3u, 13u)
#define PPUTLIMPL_UINT_TRAIT_233u DEC, 0xE9, 1, 7u, 15u, ()
#define PPUTLIMPL_UINT_TRAIT_232u DEC, 0xE8, 1, 7u, 15u, (2u, 2u, 2u, 29u)
#define PPUTLIMPL_UINT_TRAIT_231u DEC, 0xE7, 1, 7u, 15u, (3u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_230u DEC, 0xE6, 1, 7u, 15u, (2u, 5u, 23u)
#define PPUTLIMPL_UINT_TRAIT_229u DEC, 0xE5, 1, 7u, 15u, ()
#define PPUTLIMPL_UINT_TRAIT_228u DEC, 0xE4, 1, 7u, 15u, (2u, 2u, 3u, 19u)
#define PPUTLIMPL_UINT_TRAIT_227u DEC, 0xE3, 1, 7u, 15u, ()
#define PPUTLIMPL_UINT_TRAIT_226u DEC, 0xE2, 1, 7u, 15u, (2u, 113u)
#define PPUTLIMPL_UINT_TRAIT_225u DEC, 0xE1, 1, 7u, 15u, (3u, 3u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_224u DEC, 0xE0, 1, 7u, 14u, (2u, 2u, 2u, 2u, 2u, 7u)
#define PPUTLIMPL_UINT_TRAIT_223u DEC, 0xDF, 1, 7u, 14u, ()
#define PPUTLIMPL_UINT_TRAIT_222u DEC, 0xDE, 1, 7u, 14u, (2u, 3u, 37u)
#define PPUTLIMPL_UINT_TRAIT_221u DEC, 0xDD, 1, 7u, 14u, (13u, 17u)
#define PPUTLIMPL_UINT_TRAIT_220u DEC, 0xDC, 1, 7u, 14u, (2u, 2u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_219u DEC, 0xDB, 1, 7u, 14u, (3u, 73u)
#define PPUTLIMPL_UINT_TRAIT_218u DEC, 0xDA, 1, 7u, 14u, (2u, 109u)
#define PPUTLIMPL_UINT_TRAIT_217u DEC, 0xD9, 1, 7u, 14u, (7u, 31u)
#define PPUTLIMPL_UINT_TRAIT_216u DEC, 0xD8, 1, 7u, 14u, (2u, 2u, 2u, 3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_215u DEC, 0xD7, 1, 7u, 14u, (5u, 43u)
#define PPUTLIMPL_UINT_TRAIT_214u DEC, 0xD6, 1, 7u, 14u, (2u, 107u)
#define PPUTLIMPL_UINT_TRAIT_213u DEC, 0xD5, 1, 7u, 14u, (3u, 71u)
#define PPUTLIMPL_UINT_TRAIT_212u DEC, 0xD4, 1, 7u, 14u, (2u, 2u, 53u)
#define PPUTLIMPL_UINT_TRAIT_211u DEC, 0xD3, 1, 7u, 14u, ()
#define PPUTLIMPL_UINT_TRAIT_210u DEC, 0xD2, 1, 7u, 14u, (2u, 3u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_209u DEC, 0xD1, 1, 7u, 14u, (11u, 19u)
#define PPUTLIMPL_UINT_TRAIT_208u DEC, 0xD0, 1, 7u, 14u, (2u, 2u, 2u, 2u, 13u)
#define PPUTLIMPL_UINT_TRAIT_207u DEC, 0xCF, 1, 7u, 14u, (3u, 3u, 23u)
#define PPUTLIMPL_UINT_TRAIT_206u DEC, 0xCE, 1, 7u, 14u, (2u, 103u)
#define PPUTLIMPL_UINT_TRAIT_205u DEC, 0xCD, 1, 7u, 14u, (5u, 41u)
#define PPUTLIMPL_UINT_TRAIT_204u DEC, 0xCC, 1, 7u, 14u, (2u, 2u, 3u, 17u)
#define PPUTLIMPL_UINT_TRAIT_203u DEC, 0xCB, 1, 7u, 14u, (7u, 29u)
#define PPUTLIMPL_UINT_TRAIT_202u DEC, 0xCA, 1, 7u, 14u, (2u, 101u)
#define PPUTLIMPL_UINT_TRAIT_201u DEC, 0xC9, 1, 7u, 14u, (3u, 67u)
#define PPUTLIMPL_UINT_TRAIT_200u DEC, 0xC8, 1, 7u, 14u, (2u, 2u, 2u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_199u DEC, 0xC7, 1, 7u, 14u, ()
#define PPUTLIMPL_UINT_TRAIT_198u DEC, 0xC6, 1, 7u, 14u, (2u, 3u, 3u, 11u)
#define PPUTLIMPL_UINT_TRAIT_197u DEC, 0xC5, 1, 7u, 14u, ()
#define PPUTLIMPL_UINT_TRAIT_196u DEC, 0xC4, 1, 7u, 14u, (2u, 2u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_195u DEC, 0xC3, 1, 7u, 13u, (3u, 5u, 13u)
#define PPUTLIMPL_UINT_TRAIT_194u DEC, 0xC2, 1, 7u, 13u, (2u, 97u)
#define PPUTLIMPL_UINT_TRAIT_193u DEC, 0xC1, 1, 7u, 13u, ()
#define PPUTLIMPL_UINT_TRAIT_192u DEC, 0xC0, 1, 7u, 13u, (2u, 2u, 2u, 2u, 2u, 2u, 3u)
#define PPUTLIMPL_UINT_TRAIT_191u DEC, 0xBF, 1, 7u, 13u, ()
#define PPUTLIMPL_UINT_TRAIT_190u DEC, 0xBE, 1, 7u, 13u, (2u, 5u, 19u)
#define PPUTLIMPL_UINT_TRAIT_189u DEC, 0xBD, 1, 7u, 13u, (3u, 3u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_188u DEC, 0xBC, 1, 7u, 13u, (2u, 2u, 47u)
#define PPUTLIMPL_UINT_TRAIT_187u DEC, 0xBB, 1, 7u, 13u, (11u, 17u)
#define PPUTLIMPL_UINT_TRAIT_186u DEC, 0xBA, 1, 7u, 13u, (2u, 3u, 31u)
#define PPUTLIMPL_UINT_TRAIT_185u DEC, 0xB9, 1, 7u, 13u, (5u, 37u)
#define PPUTLIMPL_UINT_TRAIT_184u DEC, 0xB8, 1, 7u, 13u, (2u, 2u, 2u, 23u)
#define PPUTLIMPL_UINT_TRAIT_183u DEC, 0xB7, 1, 7u, 13u, (3u, 61u)
#define PPUTLIMPL_UINT_TRAIT_182u DEC, 0xB6, 1, 7u, 13u, (2u, 7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_181u DEC, 0xB5, 1, 7u, 13u, ()
#define PPUTLIMPL_UINT_TRAIT_180u DEC, 0xB4, 1, 7u, 13u, (2u, 2u, 3u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_179u DEC, 0xB3, 1, 7u, 13u, ()
#define PPUTLIMPL_UINT_TRAIT_178u DEC, 0xB2, 1, 7u, 13u, (2u, 89u)
#define PPUTLIMPL_UINT_TRAIT_177u DEC, 0xB1, 1, 7u, 13u, (3u, 59u)
#define PPUTLIMPL_UINT_TRAIT_176u DEC, 0xB0, 1, 7u, 13u, (2u, 2u, 2u, 2u, 11u)
#define PPUTLIMPL_UINT_TRAIT_175u DEC, 0xAF, 1, 7u, 13u, (5u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_174u DEC, 0xAE, 1, 7u, 13u, (2u, 3u, 29u)
#define PPUTLIMPL_UINT_TRAIT_173u DEC, 0xAD, 1, 7u, 13u, ()
#define PPUTLIMPL_UINT_TRAIT_172u DEC, 0xAC, 1, 7u, 13u, (2u, 2u, 43u)
#define PPUTLIMPL_UINT_TRAIT_171u DEC, 0xAB, 1, 7u, 13u, (3u, 3u, 19u)
#define PPUTLIMPL_UINT_TRAIT_170u DEC, 0xAA, 1, 7u, 13u, (2u, 5u, 17u)
#define PPUTLIMPL_UINT_TRAIT_169u DEC, 0xA9, 1, 7u, 13u, (13u, 13u)
#define PPUTLIMPL_UINT_TRAIT_168u DEC, 0xA8, 1, 7u, 12u, (2u, 2u, 2u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_167u DEC, 0xA7, 1, 7u, 12u, ()
#define PPUTLIMPL_UINT_TRAIT_166u DEC, 0xA6, 1, 7u, 12u, (2u, 83u)
#define PPUTLIMPL_UINT_TRAIT_165u DEC, 0xA5, 1, 7u, 12u, (3u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_164u DEC, 0xA4, 1, 7u, 12u, (2u, 2u, 41u)
#define PPUTLIMPL_UINT_TRAIT_163u DEC, 0xA3, 1, 7u, 12u, ()
#define PPUTLIMPL_UINT_TRAIT_162u DEC, 0xA2, 1, 7u, 12u, (2u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_161u DEC, 0xA1, 1, 7u, 12u, (7u, 23u)
#define PPUTLIMPL_UINT_TRAIT_160u DEC, 0xA0, 1, 7u, 12u, (2u, 2u, 2u, 2u, 2u, 5u)
#define PPUTLIMPL_UINT_TRAIT_159u DEC, 0x9F, 1, 7u, 12u, (3u, 53u)
#define PPUTLIMPL_UINT_TRAIT_158u DEC, 0x9E, 1, 7u, 12u, (2u, 79u)
#define PPUTLIMPL_UINT_TRAIT_157u DEC, 0x9D, 1, 7u, 12u, ()
#define PPUTLIMPL_UINT_TRAIT_156u DEC, 0x9C, 1, 7u, 12u, (2u, 2u, 3u, 13u)
#define PPUTLIMPL_UINT_TRAIT_155u DEC, 0x9B, 1, 7u, 12u, (5u, 31u)
#define PPUTLIMPL_UINT_TRAIT_154u DEC, 0x9A, 1, 7u, 12u, (2u, 7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_153u DEC, 0x99, 1, 7u, 12u, (3u, 3u, 17u)
#define PPUTLIMPL_UINT_TRAIT_152u DEC, 0x98, 1, 7u, 12u, (2u, 2u, 2u, 19u)
#define PPUTLIMPL_UINT_TRAIT_151u DEC, 0x97, 1, 7u, 12u, ()
#define PPUTLIMPL_UINT_TRAIT_150u DEC, 0x96, 1, 7u, 12u, (2u, 3u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_149u DEC, 0x95, 1, 7u, 12u, ()
#define PPUTLIMPL_UINT_TRAIT_148u DEC, 0x94, 1, 7u, 12u, (2u, 2u, 37u)
#define PPUTLIMPL_UINT_TRAIT_147u DEC, 0x93, 1, 7u, 12u, (3u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_146u DEC, 0x92, 1, 7u, 12u, (2u, 73u)
#define PPUTLIMPL_UINT_TRAIT_145u DEC, 0x91, 1, 7u, 12u, (5u, 29u)
#define PPUTLIMPL_UINT_TRAIT_144u DEC, 0x90, 1, 7u, 12u, (2u, 2u, 2u, 2u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_143u DEC, 0x8F, 1, 7u, 11u, (11u, 13u)
#define PPUTLIMPL_UINT_TRAIT_142u DEC, 0x8E, 1, 7u, 11u, (2u, 71u)
#define PPUTLIMPL_UINT_TRAIT_141u DEC, 0x8D, 1, 7u, 11u, (3u, 47u)
#define PPUTLIMPL_UINT_TRAIT_140u DEC, 0x8C, 1, 7u, 11u, (2u, 2u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_139u DEC, 0x8B, 1, 7u, 11u, ()
#define PPUTLIMPL_UINT_TRAIT_138u DEC, 0x8A, 1, 7u, 11u, (2u, 3u, 23u)
#define PPUTLIMPL_UINT_TRAIT_137u DEC, 0x89, 1, 7u, 11u, ()
#define PPUTLIMPL_UINT_TRAIT_136u DEC, 0x88, 1, 7u, 11u, (2u, 2u, 2u, 17u)
#define PPUTLIMPL_UINT_TRAIT_135u DEC, 0x87, 1, 7u, 11u, (3u, 3u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_134u DEC, 0x86, 1, 7u, 11u, (2u, 67u)
#define PPUTLIMPL_UINT_TRAIT_133u DEC, 0x85, 1, 7u, 11u, (7u, 19u)
#define PPUTLIMPL_UINT_TRAIT_132u DEC, 0x84, 1, 7u, 11u, (2u, 2u, 3u, 11u)
#define PPUTLIMPL_UINT_TRAIT_131u DEC, 0x83, 1, 7u, 11u, ()
#define PPUTLIMPL_UINT_TRAIT_130u DEC, 0x82, 1, 7u, 11u, (2u, 5u, 13u)
#define PPUTLIMPL_UINT_TRAIT_129u DEC, 0x81, 1, 7u, 11u, (3u, 43u)
#define PPUTLIMPL_UINT_TRAIT_128u DEC, 0x80, 1, 7u, 11u, (2u, 2u, 2u, 2u, 2u, 2u, 2u)
#define PPUTLIMPL_UINT_TRAIT_127u DEC, 0x7F, 0, 6u, 11u, ()
#define PPUTLIMPL_UINT_TRAIT_126u DEC, 0x7E, 0, 6u, 11u, (2u, 3u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_125u DEC, 0x7D, 0, 6u, 11u, (5u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_124u DEC, 0x7C, 0, 6u, 11u, (2u, 2u, 31u)
#define PPUTLIMPL_UINT_TRAIT_123u DEC, 0x7B, 0, 6u, 11u, (3u, 41u)
#define PPUTLIMPL_UINT_TRAIT_122u DEC, 0x7A, 0, 6u, 11u, (2u, 61u)
#define PPUTLIMPL_UINT_TRAIT_121u DEC, 0x79, 0, 6u, 11u, (11u, 11u)
#define PPUTLIMPL_UINT_TRAIT_120u DEC, 0x78, 0, 6u, 10u, (2u, 2u, 2u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_119u DEC, 0x77, 0, 6u, 10u, (7u, 17u)
#define PPUTLIMPL_UINT_TRAIT_118u DEC, 0x76, 0, 6u, 10u, (2u, 59u)
#define PPUTLIMPL_UINT_TRAIT_117u DEC, 0x75, 0, 6u, 10u, (3u, 3u, 13u)
#define PPUTLIMPL_UINT_TRAIT_116u DEC, 0x74, 0, 6u, 10u, (2u, 2u, 29u)
#define PPUTLIMPL_UINT_TRAIT_115u DEC, 0x73, 0, 6u, 10u, (5u, 23u)
#define PPUTLIMPL_UINT_TRAIT_114u DEC, 0x72, 0, 6u, 10u, (2u, 3u, 19u)
#define PPUTLIMPL_UINT_TRAIT_113u DEC, 0x71, 0, 6u, 10u, ()
#define PPUTLIMPL_UINT_TRAIT_112u DEC, 0x70, 0, 6u, 10u, (2u, 2u, 2u, 2u, 7u)
#define PPUTLIMPL_UINT_TRAIT_111u DEC, 0x6F, 0, 6u, 10u, (3u, 37u)
#define PPUTLIMPL_UINT_TRAIT_110u DEC, 0x6E, 0, 6u, 10u, (2u, 5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_109u DEC, 0x6D, 0, 6u, 10u, ()
#define PPUTLIMPL_UINT_TRAIT_108u DEC, 0x6C, 0, 6u, 10u, (2u, 2u, 3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_107u DEC, 0x6B, 0, 6u, 10u, ()
#define PPUTLIMPL_UINT_TRAIT_106u DEC, 0x6A, 0, 6u, 10u, (2u, 53u)
#define PPUTLIMPL_UINT_TRAIT_105u DEC, 0x69, 0, 6u, 10u, (3u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_104u DEC, 0x68, 0, 6u, 10u, (2u, 2u, 2u, 13u)
#define PPUTLIMPL_UINT_TRAIT_103u DEC, 0x67, 0, 6u, 10u, ()
#define PPUTLIMPL_UINT_TRAIT_102u DEC, 0x66, 0, 6u, 10u, (2u, 3u, 17u)
#define PPUTLIMPL_UINT_TRAIT_101u DEC, 0x65, 0, 6u, 10u, ()
#define PPUTLIMPL_UINT_TRAIT_100u DEC, 0x64, 0, 6u, 10u, (2u, 2u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_99u  DEC, 0x63, 0, 6u, 9u, (3u, 3u, 11u)
#define PPUTLIMPL_UINT_TRAIT_98u  DEC, 0x62, 0, 6u, 9u, (2u, 7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_97u  DEC, 0x61, 0, 6u, 9u, ()
#define PPUTLIMPL_UINT_TRAIT_96u  DEC, 0x60, 0, 6u, 9u, (2u, 2u, 2u, 2u, 2u, 3u)
#define PPUTLIMPL_UINT_TRAIT_95u  DEC, 0x5F, 0, 6u, 9u, (5u, 19u)
#define PPUTLIMPL_UINT_TRAIT_94u  DEC, 0x5E, 0, 6u, 9u, (2u, 47u)
#define PPUTLIMPL_UINT_TRAIT_93u  DEC, 0x5D, 0, 6u, 9u, (3u, 31u)
#define PPUTLIMPL_UINT_TRAIT_92u  DEC, 0x5C, 0, 6u, 9u, (2u, 2u, 23u)
#define PPUTLIMPL_UINT_TRAIT_91u  DEC, 0x5B, 0, 6u, 9u, (7u, 13u)
#define PPUTLIMPL_UINT_TRAIT_90u  DEC, 0x5A, 0, 6u, 9u, (2u, 3u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_89u  DEC, 0x59, 0, 6u, 9u, ()
#define PPUTLIMPL_UINT_TRAIT_88u  DEC, 0x58, 0, 6u, 9u, (2u, 2u, 2u, 11u)
#define PPUTLIMPL_UINT_TRAIT_87u  DEC, 0x57, 0, 6u, 9u, (3u, 29u)
#define PPUTLIMPL_UINT_TRAIT_86u  DEC, 0x56, 0, 6u, 9u, (2u, 43u)
#define PPUTLIMPL_UINT_TRAIT_85u  DEC, 0x55, 0, 6u, 9u, (5u, 17u)
#define PPUTLIMPL_UINT_TRAIT_84u  DEC, 0x54, 0, 6u, 9u, (2u, 2u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_83u  DEC, 0x53, 0, 6u, 9u, ()
#define PPUTLIMPL_UINT_TRAIT_82u  DEC, 0x52, 0, 6u, 9u, (2u, 41u)
#define PPUTLIMPL_UINT_TRAIT_81u  DEC, 0x51, 0, 6u, 9u, (3u, 3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_80u  DEC, 0x50, 0, 6u, 8u, (2u, 2u, 2u, 2u, 5u)
#define PPUTLIMPL_UINT_TRAIT_79u  DEC, 0x4F, 0, 6u, 8u, ()
#define PPUTLIMPL_UINT_TRAIT_78u  DEC, 0x4E, 0, 6u, 8u, (2u, 3u, 13u)
#define PPUTLIMPL_UINT_TRAIT_77u  DEC, 0x4D, 0, 6u, 8u, (7u, 11u)
#define PPUTLIMPL_UINT_TRAIT_76u  DEC, 0x4C, 0, 6u, 8u, (2u, 2u, 19u)
#define PPUTLIMPL_UINT_TRAIT_75u  DEC, 0x4B, 0, 6u, 8u, (3u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_74u  DEC, 0x4A, 0, 6u, 8u, (2u, 37u)
#define PPUTLIMPL_UINT_TRAIT_73u  DEC, 0x49, 0, 6u, 8u, ()
#define PPUTLIMPL_UINT_TRAIT_72u  DEC, 0x48, 0, 6u, 8u, (2u, 2u, 2u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_71u  DEC, 0x47, 0, 6u, 8u, ()
#define PPUTLIMPL_UINT_TRAIT_70u  DEC, 0x46, 0, 6u, 8u, (2u, 5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_69u  DEC, 0x45, 0, 6u, 8u, (3u, 23u)
#define PPUTLIMPL_UINT_TRAIT_68u  DEC, 0x44, 0, 6u, 8u, (2u, 2u, 17u)
#define PPUTLIMPL_UINT_TRAIT_67u  DEC, 0x43, 0, 6u, 8u, ()
#define PPUTLIMPL_UINT_TRAIT_66u  DEC, 0x42, 0, 6u, 8u, (2u, 3u, 11u)
#define PPUTLIMPL_UINT_TRAIT_65u  DEC, 0x41, 0, 6u, 8u, (5u, 13u)
#define PPUTLIMPL_UINT_TRAIT_64u  DEC, 0x40, 0, 6u, 8u, (2u, 2u, 2u, 2u, 2u, 2u)
#define PPUTLIMPL_UINT_TRAIT_63u  DEC, 0x3F, 0, 5u, 7u, (3u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_62u  DEC, 0x3E, 0, 5u, 7u, (2u, 31u)
#define PPUTLIMPL_UINT_TRAIT_61u  DEC, 0x3D, 0, 5u, 7u, ()
#define PPUTLIMPL_UINT_TRAIT_60u  DEC, 0x3C, 0, 5u, 7u, (2u, 2u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_59u  DEC, 0x3B, 0, 5u, 7u, ()
#define PPUTLIMPL_UINT_TRAIT_58u  DEC, 0x3A, 0, 5u, 7u, (2u, 29u)
#define PPUTLIMPL_UINT_TRAIT_57u  DEC, 0x39, 0, 5u, 7u, (3u, 19u)
#define PPUTLIMPL_UINT_TRAIT_56u  DEC, 0x38, 0, 5u, 7u, (2u, 2u, 2u, 7u)
#define PPUTLIMPL_UINT_TRAIT_55u  DEC, 0x37, 0, 5u, 7u, (5u, 11u)
#define PPUTLIMPL_UINT_TRAIT_54u  DEC, 0x36, 0, 5u, 7u, (2u, 3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_53u  DEC, 0x35, 0, 5u, 7u, ()
#define PPUTLIMPL_UINT_TRAIT_52u  DEC, 0x34, 0, 5u, 7u, (2u, 2u, 13u)
#define PPUTLIMPL_UINT_TRAIT_51u  DEC, 0x33, 0, 5u, 7u, (3u, 17u)
#define PPUTLIMPL_UINT_TRAIT_50u  DEC, 0x32, 0, 5u, 7u, (2u, 5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_49u  DEC, 0x31, 0, 5u, 7u, (7u, 7u)
#define PPUTLIMPL_UINT_TRAIT_48u  DEC, 0x30, 0, 5u, 6u, (2u, 2u, 2u, 2u, 3u)
#define PPUTLIMPL_UINT_TRAIT_47u  DEC, 0x2F, 0, 5u, 6u, ()
#define PPUTLIMPL_UINT_TRAIT_46u  DEC, 0x2E, 0, 5u, 6u, (2u, 23u)
#define PPUTLIMPL_UINT_TRAIT_45u  DEC, 0x2D, 0, 5u, 6u, (3u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_44u  DEC, 0x2C, 0, 5u, 6u, (2u, 2u, 11u)
#define PPUTLIMPL_UINT_TRAIT_43u  DEC, 0x2B, 0, 5u, 6u, ()
#define PPUTLIMPL_UINT_TRAIT_42u  DEC, 0x2A, 0, 5u, 6u, (2u, 3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_41u  DEC, 0x29, 0, 5u, 6u, ()
#define PPUTLIMPL_UINT_TRAIT_40u  DEC, 0x28, 0, 5u, 6u, (2u, 2u, 2u, 5u)
#define PPUTLIMPL_UINT_TRAIT_39u  DEC, 0x27, 0, 5u, 6u, (3u, 13u)
#define PPUTLIMPL_UINT_TRAIT_38u  DEC, 0x26, 0, 5u, 6u, (2u, 19u)
#define PPUTLIMPL_UINT_TRAIT_37u  DEC, 0x25, 0, 5u, 6u, ()
#define PPUTLIMPL_UINT_TRAIT_36u  DEC, 0x24, 0, 5u, 6u, (2u, 2u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_35u  DEC, 0x23, 0, 5u, 5u, (5u, 7u)
#define PPUTLIMPL_UINT_TRAIT_34u  DEC, 0x22, 0, 5u, 5u, (2u, 17u)
#define PPUTLIMPL_UINT_TRAIT_33u  DEC, 0x21, 0, 5u, 5u, (3u, 11u)
#define PPUTLIMPL_UINT_TRAIT_32u  DEC, 0x20, 0, 5u, 5u, (2u, 2u, 2u, 2u, 2u)
#define PPUTLIMPL_UINT_TRAIT_31u  DEC, 0x1F, 0, 4u, 5u, ()
#define PPUTLIMPL_UINT_TRAIT_30u  DEC, 0x1E, 0, 4u, 5u, (2u, 3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_29u  DEC, 0x1D, 0, 4u, 5u, ()
#define PPUTLIMPL_UINT_TRAIT_28u  DEC, 0x1C, 0, 4u, 5u, (2u, 2u, 7u)
#define PPUTLIMPL_UINT_TRAIT_27u  DEC, 0x1B, 0, 4u, 5u, (3u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_26u  DEC, 0x1A, 0, 4u, 5u, (2u, 13u)
#define PPUTLIMPL_UINT_TRAIT_25u  DEC, 0x19, 0, 4u, 5u, (5u, 5u)
#define PPUTLIMPL_UINT_TRAIT_24u  DEC, 0x18, 0, 4u, 4u, (2u, 2u, 2u, 3u)
#define PPUTLIMPL_UINT_TRAIT_23u  DEC, 0x17, 0, 4u, 4u, ()
#define PPUTLIMPL_UINT_TRAIT_22u  DEC, 0x16, 0, 4u, 4u, (2u, 11u)
#define PPUTLIMPL_UINT_TRAIT_21u  DEC, 0x15, 0, 4u, 4u, (3u, 7u)
#define PPUTLIMPL_UINT_TRAIT_20u  DEC, 0x14, 0, 4u, 4u, (2u, 2u, 5u)
#define PPUTLIMPL_UINT_TRAIT_19u  DEC, 0x13, 0, 4u, 4u, ()
#define PPUTLIMPL_UINT_TRAIT_18u  DEC, 0x12, 0, 4u, 4u, (2u, 3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_17u  DEC, 0x11, 0, 4u, 4u, ()
#define PPUTLIMPL_UINT_TRAIT_16u  DEC, 0x10, 0, 4u, 4u, (2u, 2u, 2u, 2u)
#define PPUTLIMPL_UINT_TRAIT_15u  DEC, 0x0F, 0, 3u, 3u, (3u, 5u)
#define PPUTLIMPL_UINT_TRAIT_14u  DEC, 0x0E, 0, 3u, 3u, (2u, 7u)
#define PPUTLIMPL_UINT_TRAIT_13u  DEC, 0x0D, 0, 3u, 3u, ()
#define PPUTLIMPL_UINT_TRAIT_12u  DEC, 0x0C, 0, 3u, 3u, (2u, 2u, 3u)
#define PPUTLIMPL_UINT_TRAIT_11u  DEC, 0x0B, 0, 3u, 3u, ()
#define PPUTLIMPL_UINT_TRAIT_10u  DEC, 0x0A, 0, 3u, 3u, (2u, 5u)
#define PPUTLIMPL_UINT_TRAIT_9u   DEC, 0x09, 0, 3u, 3u, (3u, 3u)
#define PPUTLIMPL_UINT_TRAIT_8u   DEC, 0x08, 0, 3u, 2u, (2u, 2u, 2u)
#define PPUTLIMPL_UINT_TRAIT_7u   DEC, 0x07, 0, 2u, 2u, ()
#define PPUTLIMPL_UINT_TRAIT_6u   DEC, 0x06, 0, 2u, 2u, (2u, 3u)
#define PPUTLIMPL_UINT_TRAIT_5u   DEC, 0x05, 0, 2u, 2u, ()
#define PPUTLIMPL_UINT_TRAIT_4u   DEC, 0x04, 0, 2u, 2u, (2u, 2u)
#define PPUTLIMPL_UINT_TRAIT_3u   DEC, 0x03, 0, 1u, 1u, ()
#define PPUTLIMPL_UINT_TRAIT_2u   DEC, 0x02, 0, 1u, 1u, ()
#define PPUTLIMPL_UINT_TRAIT_1u   DEC, 0x01, 0, 0u, 1u, ()
#define PPUTLIMPL_UINT_TRAIT_0u   DEC, 0x00, 0, , 0u, ()

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

// vim: fdm=marker:fmr={{{,}}}

#endif

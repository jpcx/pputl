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
//     some: <abstract> anything                                              //
//      |-any: exactly one generic value                                      //
//         |-tup: anything in parentheses                                     //
//         |-atom: a generic non-tuple value                                  //
//            |- bool: a literal '1' or '0'                                   //
//            |- bit: an alias for bool                                       //
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
#define PTL_BUILD /* -> <c++ int> */ 20220810

/// [config.hex_length]
/// -------------------
/// the number of hex digits used by pputl integers.
/// hex representations are fixed at this length.
/// see the readme code generation section to configure.
#define PTL_HEX_LENGTH /* -> uint */ 2

/// [config.bit_length]
/// -------------------
/// the number of bits used by pputl integers.
/// see the readme code generation section to configure.
#define PTL_BIT_LENGTH /* -> uint */ 8

/// [config.uint_max]
/// -----------------
/// the maximum value of a pputl unsigned int.
/// upper bound on the number of args for many pputl functions.
/// see the readme code generation section to configure.
#define PTL_UINT_MAX /* -> uint */ 255u

/// [config.int_max]
/// ----------------
/// the maximum value of a pputl signed int.
/// see the readme code generation section to configure.
#define PTL_INT_MAX /* -> int */ 127

/// [config.int_min]
/// ----------------
/// the minimum value of a pputl signed int.
/// only representable as binary. see type.int for details.
/// see the readme code generation section to configure.
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

/// [type.none]
/// -----------
/// nothing. fails if something.
///
/// PTL_NONE() // <nothing>
#define PTL_NONE(/* <nothing> */...) /* -> none */                                     \
  PPUTLNONE##__VA_OPT__(_NO)##_PASS(PTL_ISTR([PTL_NONE] none cannot describe something \
                                             : __VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLNONE_NO_PASS(e) PTL_FAIL(e)
#define PPUTLNONE_PASS(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.some]
/// -----------
/// something. fails if nothing.
///
/// PTL_SOME(foo)             // foo
/// PTL_SOME(foo, bar)        // foo, bar
/// PTL_SOME(foo, 42, (, , )) // foo, 42, (, , )
/// PTL_SOME(, )              // ,
#define PTL_SOME(/* args: <something>... */...) /* -> some{...args} */               \
  PPUTLSOME##__VA_OPT__(_NO)##_FAIL(PTL_ISTR([PTL_SOME] some cannot describe nothing \
                                             : __VA_ARGS__),                         \
                                    __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLSOME_NO_FAIL(e, ...) __VA_ARGS__
#define PPUTLSOME_FAIL(e, ...)    PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.any]
/// ----------
/// [inherits from PTL_SOME] any type (generic data). returns arg.
/// describes exactly one generic value.
///
/// fails if not exactly one arg.
///
/// PTL_ANY(foo) // foo
#define PTL_ANY(/* v: any */...) /* -> any{v} */                         \
  PPUTLANY_o(__VA_ARGS__.)(__VA_ARGS__)(                                 \
      PTL_ISTR([PTL_ANY] any cannot describe nothing                     \
               : __VA_ARGS__),                                           \
      PTL_ISTR([PTL_ANY] any cannot describe more than one generic value \
               : __VA_ARGS__),                                           \
      __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLANY_o(_, ...)                      PPUTLANY_oo##__VA_OPT__(_NO)##_PASS
#define PPUTLANY_oo_NO_PASS(...)                PPUTLANY_ooo_FAIL_REST
#define PPUTLANY_oo_PASS(...)                   PPUTLANY_ooo_##__VA_OPT__(NO_)##FAIL_FIRST
#define PPUTLANY_ooo_FAIL_REST(e0, e1, ...)     PTL_FAIL(e1)
#define PPUTLANY_ooo_NO_FAIL_FIRST(e0, e1, any) any
#define PPUTLANY_ooo_FAIL_FIRST(e0, e1, ...)    PTL_FAIL(e0)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.tup]
/// ----------
/// [inherits from PTL_ANY] tuple type (any...).
/// expands to t if valid, else fails.
///
/// PTL_TUP(())     // ()
/// PTL_TUP((1, 2)) // (1, 2)
#define PTL_TUP(/* t: tup */...) /* -> t */ \
  PPUTLTUP_o(PTL_EAT __VA_ARGS__)(PTL_ISTR([PTL_TUP] invalid tuple : __VA_ARGS__), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLTUP_o(...)               PPUTLTUP_oo##__VA_OPT__(_NO)##_PASS
#define PPUTLTUP_oo_NO_PASS(err, ...) PTL_FAIL(err)
#define PPUTLTUP_oo_PASS(err, ...)    __VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.atom]
/// -----------
/// [inherits from PTL_ANY] a generic, non-tuple, singular value.
/// must not be a deferred expression.
/// fails if tuple.
///
/// PTL_ATOM(foo) // foo
#define PTL_ATOM(/* v: any */...) /* -> v */           \
  PPUTLATOM_o(PTL_ESC(PTL_EAT PTL_ANY(__VA_ARGS__)))(  \
      PTL_ISTR([PTL_ATOM] atom cannot describe a tuple \
               : __VA_ARGS__),                         \
      __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLATOM_o(...)               PPUTLATOM_oo##__VA_OPT__(_NO)##_FAIL
#define PPUTLATOM_oo_NO_FAIL(err, ...) __VA_ARGS__
#define PPUTLATOM_oo_FAIL(err, ...)    PTL_FAIL(err)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.bool]
/// -----------
/// [inherits from PTL_ATOM] bool type (0 or 1).
/// expands to b if valid, else fails.
///
/// PTL_BOOL(0) // 0
/// PTL_BOOL(1) // 1
#define PTL_BOOL(/* b: 0|1 */...) /* -> b */                                               \
  PPUTLBOOL_o(PTL_CAT(PPUTLBOOL_, PTL_ATOM(__VA_ARGS__)))(PTL_ISTR([PTL_BOOL] invalid bool \
                                                                   : __VA_ARGS__),         \
                                                          __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBOOL_o(...)          PPUTLBOOL##__VA_OPT__(_NO)##_PASS
#define PPUTLBOOL_PASS(e, b)      b
#define PPUTLBOOL_NO_PASS(e, ...) PTL_FAIL(e)
#define PPUTLBOOL_1
#define PPUTLBOOL_0

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
#define PTL_UINT(/* n: uint|int|nybl */...) /* -> uint{n} */                          \
  PPUTLUINT_o(PTL_ATOM(__VA_ARGS__))(__VA_ARGS__)(PTL_ISTR([PTL_UINT] invalid integer \
                                                           : __VA_ARGS__),            \
                                                  __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

/// third parentheses; attempts cast from unsigned.
#define PPUTLUINT_o(...)               PPUTLUINT_o_RES(PTL_CAT(PPUTLUTRAITS_, __VA_ARGS__))
#define PPUTLUINT_o_RES(...)           PPUTLUINT_o_RES_o(__VA_ARGS__)
#define PPUTLUINT_o_RES_o(_, ...)      PPUTLUINT_o_RES_o##__VA_OPT__(_NO)##_FAIL()
#define PPUTLUINT_o_RES_o_NO_FAIL(...) PPUTLUINT_oo_UPASS
#define PPUTLUINT_o_RES_o_FAIL(...)    PPUTLUINT_oo

/// fourth parentheses; attempts cast from signed.
#define PPUTLUINT_oo(...)             PPUTLUINT_oo_RES(__VA_ARGS__##u, PPUTLUTRAITS_##__VA_ARGS__##u)
#define PPUTLUINT_oo_RES(u, ...)      PPUTLUINT_oo_RES_o(u, __VA_ARGS__)
#define PPUTLUINT_oo_RES_o(u, _, ...) PPUTLUINT_oo_##__VA_OPT__(NO_)##FAIL(u)
#define PPUTLUINT_oo_UPASS(...)       PPUTLUINT_UPASS
#define PPUTLUINT_oo_NO_FAIL(u)       PTL_CAT(PPUTLUINT_oo_ICHK_, PPUTLUINT_TRAIT(u, TYPE))(u)
#define PPUTLUINT_oo_FAIL(u)          PPUTLUINT_FAIL
#define PPUTLUINT_oo_ICHK_HEX(u)      PPUTLUINT_IPASS
#define PPUTLUINT_oo_ICHK_DEC(u)      PTL_CAT(PPUTLUINT_oo_ICHK_DEC_, PPUTLUINT_TRAIT(u, DEC_INEG))
#define PPUTLUINT_oo_ICHK_DEC_1       PPUTLUINT_FAIL
#define PPUTLUINT_oo_ICHK_DEC_0       PPUTLUINT_IPASS

/// final parentheses; returns
#define PPUTLUINT_UPASS(e, v)  v
#define PPUTLUINT_IPASS(e, v)  v##u
#define PPUTLUINT_FAIL(e, ...) PPUTLUINT_FAIL(e)

/// internal traits retrieval. uint must be valid and have a suffix.
#define PPUTLUINT_TRAIT(...)           PPUTLUINT_TRAIT_o(__VA_ARGS__)
#define PPUTLUINT_TRAIT_o(uint, trait) PPUTLUINT_TRAIT_oo(trait, PTL_CAT(PPUTLUTRAITS_, uint))
#define PPUTLUINT_TRAIT_oo(trait, ...) PPUTLUINT_TRAIT_##trait(__VA_ARGS__)

/// uint traits. trait name follows the PPUTLUINT_TRAIT_ prefix
#define PPUTLUINT_TRAIT_DEC_FACT(t, ib, in, l2, sq, f) /* -> typle<ubase10{factor}...> */ f
#define PPUTLUINT_TRAIT_DEC_SQRT(t, ib, in, l2, sq, f) /* -> ubase10{sqrt(n)} */ sq
#define PPUTLUINT_TRAIT_DEC_LOG2(t, ib, in, l2, sq, f) /* -> ubase10{log2(n)} */ l2
#define PPUTLUINT_TRAIT_DEC_INEG(t, ib, in, l2, sq, f) /* -> bool{signed<0} */ in
#define PPUTLUINT_TRAIT_DEC_IHEX(t, ib, in, l2, sq, f) /* -> ibase16 */ ib
#define PPUTLUINT_TRAIT_HEX_BNOT(t, ud, id, b, bn)     /* -> ubase16 */ bn
#define PPUTLUINT_TRAIT_HEX_BITS(t, ud, id, b, bn)     /* -> tuple<bool...> */ b
#define PPUTLUINT_TRAIT_HEX_IDEC(t, ud, id, b, bn)     /* -> ibase10 */ id
#define PPUTLUINT_TRAIT_HEX_UDEC(t, ud, id, b, bn)     /* -> ubase10 */ ud
#define PPUTLUINT_TRAIT_TYPE(t, ...)                   /* -> HEX|DEC */ t

// clang-format off

/// type, unsigned decimal, signed decimal, bits, bitnot
#define PPUTLUTRAITS_0xFFu HEX,255u,-1,(1,1,1,1,1,1,1,1),0x00u
#define PPUTLUTRAITS_0xFEu HEX,254u,-2,(1,1,1,1,1,1,1,0),0x01u
#define PPUTLUTRAITS_0xFDu HEX,253u,-3,(1,1,1,1,1,1,0,1),0x02u
#define PPUTLUTRAITS_0xFCu HEX,252u,-4,(1,1,1,1,1,1,0,0),0x03u
#define PPUTLUTRAITS_0xFBu HEX,251u,-5,(1,1,1,1,1,0,1,1),0x04u
#define PPUTLUTRAITS_0xFAu HEX,250u,-6,(1,1,1,1,1,0,1,0),0x05u
#define PPUTLUTRAITS_0xF9u HEX,249u,-7,(1,1,1,1,1,0,0,1),0x06u
#define PPUTLUTRAITS_0xF8u HEX,248u,-8,(1,1,1,1,1,0,0,0),0x07u
#define PPUTLUTRAITS_0xF7u HEX,247u,-9,(1,1,1,1,0,1,1,1),0x08u
#define PPUTLUTRAITS_0xF6u HEX,246u,-10,(1,1,1,1,0,1,1,0),0x09u
#define PPUTLUTRAITS_0xF5u HEX,245u,-11,(1,1,1,1,0,1,0,1),0x0Au
#define PPUTLUTRAITS_0xF4u HEX,244u,-12,(1,1,1,1,0,1,0,0),0x0Bu
#define PPUTLUTRAITS_0xF3u HEX,243u,-13,(1,1,1,1,0,0,1,1),0x0Cu
#define PPUTLUTRAITS_0xF2u HEX,242u,-14,(1,1,1,1,0,0,1,0),0x0Du
#define PPUTLUTRAITS_0xF1u HEX,241u,-15,(1,1,1,1,0,0,0,1),0x0Eu
#define PPUTLUTRAITS_0xF0u HEX,240u,-16,(1,1,1,1,0,0,0,0),0x0Fu
#define PPUTLUTRAITS_0xEFu HEX,239u,-17,(1,1,1,0,1,1,1,1),0x10u
#define PPUTLUTRAITS_0xEEu HEX,238u,-18,(1,1,1,0,1,1,1,0),0x11u
#define PPUTLUTRAITS_0xEDu HEX,237u,-19,(1,1,1,0,1,1,0,1),0x12u
#define PPUTLUTRAITS_0xECu HEX,236u,-20,(1,1,1,0,1,1,0,0),0x13u
#define PPUTLUTRAITS_0xEBu HEX,235u,-21,(1,1,1,0,1,0,1,1),0x14u
#define PPUTLUTRAITS_0xEAu HEX,234u,-22,(1,1,1,0,1,0,1,0),0x15u
#define PPUTLUTRAITS_0xE9u HEX,233u,-23,(1,1,1,0,1,0,0,1),0x16u
#define PPUTLUTRAITS_0xE8u HEX,232u,-24,(1,1,1,0,1,0,0,0),0x17u
#define PPUTLUTRAITS_0xE7u HEX,231u,-25,(1,1,1,0,0,1,1,1),0x18u
#define PPUTLUTRAITS_0xE6u HEX,230u,-26,(1,1,1,0,0,1,1,0),0x19u
#define PPUTLUTRAITS_0xE5u HEX,229u,-27,(1,1,1,0,0,1,0,1),0x1Au
#define PPUTLUTRAITS_0xE4u HEX,228u,-28,(1,1,1,0,0,1,0,0),0x1Bu
#define PPUTLUTRAITS_0xE3u HEX,227u,-29,(1,1,1,0,0,0,1,1),0x1Cu
#define PPUTLUTRAITS_0xE2u HEX,226u,-30,(1,1,1,0,0,0,1,0),0x1Du
#define PPUTLUTRAITS_0xE1u HEX,225u,-31,(1,1,1,0,0,0,0,1),0x1Eu
#define PPUTLUTRAITS_0xE0u HEX,224u,-32,(1,1,1,0,0,0,0,0),0x1Fu
#define PPUTLUTRAITS_0xDFu HEX,223u,-33,(1,1,0,1,1,1,1,1),0x20u
#define PPUTLUTRAITS_0xDEu HEX,222u,-34,(1,1,0,1,1,1,1,0),0x21u
#define PPUTLUTRAITS_0xDDu HEX,221u,-35,(1,1,0,1,1,1,0,1),0x22u
#define PPUTLUTRAITS_0xDCu HEX,220u,-36,(1,1,0,1,1,1,0,0),0x23u
#define PPUTLUTRAITS_0xDBu HEX,219u,-37,(1,1,0,1,1,0,1,1),0x24u
#define PPUTLUTRAITS_0xDAu HEX,218u,-38,(1,1,0,1,1,0,1,0),0x25u
#define PPUTLUTRAITS_0xD9u HEX,217u,-39,(1,1,0,1,1,0,0,1),0x26u
#define PPUTLUTRAITS_0xD8u HEX,216u,-40,(1,1,0,1,1,0,0,0),0x27u
#define PPUTLUTRAITS_0xD7u HEX,215u,-41,(1,1,0,1,0,1,1,1),0x28u
#define PPUTLUTRAITS_0xD6u HEX,214u,-42,(1,1,0,1,0,1,1,0),0x29u
#define PPUTLUTRAITS_0xD5u HEX,213u,-43,(1,1,0,1,0,1,0,1),0x2Au
#define PPUTLUTRAITS_0xD4u HEX,212u,-44,(1,1,0,1,0,1,0,0),0x2Bu
#define PPUTLUTRAITS_0xD3u HEX,211u,-45,(1,1,0,1,0,0,1,1),0x2Cu
#define PPUTLUTRAITS_0xD2u HEX,210u,-46,(1,1,0,1,0,0,1,0),0x2Du
#define PPUTLUTRAITS_0xD1u HEX,209u,-47,(1,1,0,1,0,0,0,1),0x2Eu
#define PPUTLUTRAITS_0xD0u HEX,208u,-48,(1,1,0,1,0,0,0,0),0x2Fu
#define PPUTLUTRAITS_0xCFu HEX,207u,-49,(1,1,0,0,1,1,1,1),0x30u
#define PPUTLUTRAITS_0xCEu HEX,206u,-50,(1,1,0,0,1,1,1,0),0x31u
#define PPUTLUTRAITS_0xCDu HEX,205u,-51,(1,1,0,0,1,1,0,1),0x32u
#define PPUTLUTRAITS_0xCCu HEX,204u,-52,(1,1,0,0,1,1,0,0),0x33u
#define PPUTLUTRAITS_0xCBu HEX,203u,-53,(1,1,0,0,1,0,1,1),0x34u
#define PPUTLUTRAITS_0xCAu HEX,202u,-54,(1,1,0,0,1,0,1,0),0x35u
#define PPUTLUTRAITS_0xC9u HEX,201u,-55,(1,1,0,0,1,0,0,1),0x36u
#define PPUTLUTRAITS_0xC8u HEX,200u,-56,(1,1,0,0,1,0,0,0),0x37u
#define PPUTLUTRAITS_0xC7u HEX,199u,-57,(1,1,0,0,0,1,1,1),0x38u
#define PPUTLUTRAITS_0xC6u HEX,198u,-58,(1,1,0,0,0,1,1,0),0x39u
#define PPUTLUTRAITS_0xC5u HEX,197u,-59,(1,1,0,0,0,1,0,1),0x3Au
#define PPUTLUTRAITS_0xC4u HEX,196u,-60,(1,1,0,0,0,1,0,0),0x3Bu
#define PPUTLUTRAITS_0xC3u HEX,195u,-61,(1,1,0,0,0,0,1,1),0x3Cu
#define PPUTLUTRAITS_0xC2u HEX,194u,-62,(1,1,0,0,0,0,1,0),0x3Du
#define PPUTLUTRAITS_0xC1u HEX,193u,-63,(1,1,0,0,0,0,0,1),0x3Eu
#define PPUTLUTRAITS_0xC0u HEX,192u,-64,(1,1,0,0,0,0,0,0),0x3Fu
#define PPUTLUTRAITS_0xBFu HEX,191u,-65,(1,0,1,1,1,1,1,1),0x40u
#define PPUTLUTRAITS_0xBEu HEX,190u,-66,(1,0,1,1,1,1,1,0),0x41u
#define PPUTLUTRAITS_0xBDu HEX,189u,-67,(1,0,1,1,1,1,0,1),0x42u
#define PPUTLUTRAITS_0xBCu HEX,188u,-68,(1,0,1,1,1,1,0,0),0x43u
#define PPUTLUTRAITS_0xBBu HEX,187u,-69,(1,0,1,1,1,0,1,1),0x44u
#define PPUTLUTRAITS_0xBAu HEX,186u,-70,(1,0,1,1,1,0,1,0),0x45u
#define PPUTLUTRAITS_0xB9u HEX,185u,-71,(1,0,1,1,1,0,0,1),0x46u
#define PPUTLUTRAITS_0xB8u HEX,184u,-72,(1,0,1,1,1,0,0,0),0x47u
#define PPUTLUTRAITS_0xB7u HEX,183u,-73,(1,0,1,1,0,1,1,1),0x48u
#define PPUTLUTRAITS_0xB6u HEX,182u,-74,(1,0,1,1,0,1,1,0),0x49u
#define PPUTLUTRAITS_0xB5u HEX,181u,-75,(1,0,1,1,0,1,0,1),0x4Au
#define PPUTLUTRAITS_0xB4u HEX,180u,-76,(1,0,1,1,0,1,0,0),0x4Bu
#define PPUTLUTRAITS_0xB3u HEX,179u,-77,(1,0,1,1,0,0,1,1),0x4Cu
#define PPUTLUTRAITS_0xB2u HEX,178u,-78,(1,0,1,1,0,0,1,0),0x4Du
#define PPUTLUTRAITS_0xB1u HEX,177u,-79,(1,0,1,1,0,0,0,1),0x4Eu
#define PPUTLUTRAITS_0xB0u HEX,176u,-80,(1,0,1,1,0,0,0,0),0x4Fu
#define PPUTLUTRAITS_0xAFu HEX,175u,-81,(1,0,1,0,1,1,1,1),0x50u
#define PPUTLUTRAITS_0xAEu HEX,174u,-82,(1,0,1,0,1,1,1,0),0x51u
#define PPUTLUTRAITS_0xADu HEX,173u,-83,(1,0,1,0,1,1,0,1),0x52u
#define PPUTLUTRAITS_0xACu HEX,172u,-84,(1,0,1,0,1,1,0,0),0x53u
#define PPUTLUTRAITS_0xABu HEX,171u,-85,(1,0,1,0,1,0,1,1),0x54u
#define PPUTLUTRAITS_0xAAu HEX,170u,-86,(1,0,1,0,1,0,1,0),0x55u
#define PPUTLUTRAITS_0xA9u HEX,169u,-87,(1,0,1,0,1,0,0,1),0x56u
#define PPUTLUTRAITS_0xA8u HEX,168u,-88,(1,0,1,0,1,0,0,0),0x57u
#define PPUTLUTRAITS_0xA7u HEX,167u,-89,(1,0,1,0,0,1,1,1),0x58u
#define PPUTLUTRAITS_0xA6u HEX,166u,-90,(1,0,1,0,0,1,1,0),0x59u
#define PPUTLUTRAITS_0xA5u HEX,165u,-91,(1,0,1,0,0,1,0,1),0x5Au
#define PPUTLUTRAITS_0xA4u HEX,164u,-92,(1,0,1,0,0,1,0,0),0x5Bu
#define PPUTLUTRAITS_0xA3u HEX,163u,-93,(1,0,1,0,0,0,1,1),0x5Cu
#define PPUTLUTRAITS_0xA2u HEX,162u,-94,(1,0,1,0,0,0,1,0),0x5Du
#define PPUTLUTRAITS_0xA1u HEX,161u,-95,(1,0,1,0,0,0,0,1),0x5Eu
#define PPUTLUTRAITS_0xA0u HEX,160u,-96,(1,0,1,0,0,0,0,0),0x5Fu
#define PPUTLUTRAITS_0x9Fu HEX,159u,-97,(1,0,0,1,1,1,1,1),0x60u
#define PPUTLUTRAITS_0x9Eu HEX,158u,-98,(1,0,0,1,1,1,1,0),0x61u
#define PPUTLUTRAITS_0x9Du HEX,157u,-99,(1,0,0,1,1,1,0,1),0x62u
#define PPUTLUTRAITS_0x9Cu HEX,156u,-100,(1,0,0,1,1,1,0,0),0x63u
#define PPUTLUTRAITS_0x9Bu HEX,155u,-101,(1,0,0,1,1,0,1,1),0x64u
#define PPUTLUTRAITS_0x9Au HEX,154u,-102,(1,0,0,1,1,0,1,0),0x65u
#define PPUTLUTRAITS_0x99u HEX,153u,-103,(1,0,0,1,1,0,0,1),0x66u
#define PPUTLUTRAITS_0x98u HEX,152u,-104,(1,0,0,1,1,0,0,0),0x67u
#define PPUTLUTRAITS_0x97u HEX,151u,-105,(1,0,0,1,0,1,1,1),0x68u
#define PPUTLUTRAITS_0x96u HEX,150u,-106,(1,0,0,1,0,1,1,0),0x69u
#define PPUTLUTRAITS_0x95u HEX,149u,-107,(1,0,0,1,0,1,0,1),0x6Au
#define PPUTLUTRAITS_0x94u HEX,148u,-108,(1,0,0,1,0,1,0,0),0x6Bu
#define PPUTLUTRAITS_0x93u HEX,147u,-109,(1,0,0,1,0,0,1,1),0x6Cu
#define PPUTLUTRAITS_0x92u HEX,146u,-110,(1,0,0,1,0,0,1,0),0x6Du
#define PPUTLUTRAITS_0x91u HEX,145u,-111,(1,0,0,1,0,0,0,1),0x6Eu
#define PPUTLUTRAITS_0x90u HEX,144u,-112,(1,0,0,1,0,0,0,0),0x6Fu
#define PPUTLUTRAITS_0x8Fu HEX,143u,-113,(1,0,0,0,1,1,1,1),0x70u
#define PPUTLUTRAITS_0x8Eu HEX,142u,-114,(1,0,0,0,1,1,1,0),0x71u
#define PPUTLUTRAITS_0x8Du HEX,141u,-115,(1,0,0,0,1,1,0,1),0x72u
#define PPUTLUTRAITS_0x8Cu HEX,140u,-116,(1,0,0,0,1,1,0,0),0x73u
#define PPUTLUTRAITS_0x8Bu HEX,139u,-117,(1,0,0,0,1,0,1,1),0x74u
#define PPUTLUTRAITS_0x8Au HEX,138u,-118,(1,0,0,0,1,0,1,0),0x75u
#define PPUTLUTRAITS_0x89u HEX,137u,-119,(1,0,0,0,1,0,0,1),0x76u
#define PPUTLUTRAITS_0x88u HEX,136u,-120,(1,0,0,0,1,0,0,0),0x77u
#define PPUTLUTRAITS_0x87u HEX,135u,-121,(1,0,0,0,0,1,1,1),0x78u
#define PPUTLUTRAITS_0x86u HEX,134u,-122,(1,0,0,0,0,1,1,0),0x79u
#define PPUTLUTRAITS_0x85u HEX,133u,-123,(1,0,0,0,0,1,0,1),0x7Au
#define PPUTLUTRAITS_0x84u HEX,132u,-124,(1,0,0,0,0,1,0,0),0x7Bu
#define PPUTLUTRAITS_0x83u HEX,131u,-125,(1,0,0,0,0,0,1,1),0x7Cu
#define PPUTLUTRAITS_0x82u HEX,130u,-126,(1,0,0,0,0,0,1,0),0x7Du
#define PPUTLUTRAITS_0x81u HEX,129u,-127,(1,0,0,0,0,0,0,1),0x7Eu
#define PPUTLUTRAITS_0x80u HEX,128u,-128,(1,0,0,0,0,0,0,0),0x7Fu
#define PPUTLUTRAITS_0x7Fu HEX,127u,127,(0,1,1,1,1,1,1,1),0x80u
#define PPUTLUTRAITS_0x7Eu HEX,126u,126,(0,1,1,1,1,1,1,0),0x81u
#define PPUTLUTRAITS_0x7Du HEX,125u,125,(0,1,1,1,1,1,0,1),0x82u
#define PPUTLUTRAITS_0x7Cu HEX,124u,124,(0,1,1,1,1,1,0,0),0x83u
#define PPUTLUTRAITS_0x7Bu HEX,123u,123,(0,1,1,1,1,0,1,1),0x84u
#define PPUTLUTRAITS_0x7Au HEX,122u,122,(0,1,1,1,1,0,1,0),0x85u
#define PPUTLUTRAITS_0x79u HEX,121u,121,(0,1,1,1,1,0,0,1),0x86u
#define PPUTLUTRAITS_0x78u HEX,120u,120,(0,1,1,1,1,0,0,0),0x87u
#define PPUTLUTRAITS_0x77u HEX,119u,119,(0,1,1,1,0,1,1,1),0x88u
#define PPUTLUTRAITS_0x76u HEX,118u,118,(0,1,1,1,0,1,1,0),0x89u
#define PPUTLUTRAITS_0x75u HEX,117u,117,(0,1,1,1,0,1,0,1),0x8Au
#define PPUTLUTRAITS_0x74u HEX,116u,116,(0,1,1,1,0,1,0,0),0x8Bu
#define PPUTLUTRAITS_0x73u HEX,115u,115,(0,1,1,1,0,0,1,1),0x8Cu
#define PPUTLUTRAITS_0x72u HEX,114u,114,(0,1,1,1,0,0,1,0),0x8Du
#define PPUTLUTRAITS_0x71u HEX,113u,113,(0,1,1,1,0,0,0,1),0x8Eu
#define PPUTLUTRAITS_0x70u HEX,112u,112,(0,1,1,1,0,0,0,0),0x8Fu
#define PPUTLUTRAITS_0x6Fu HEX,111u,111,(0,1,1,0,1,1,1,1),0x90u
#define PPUTLUTRAITS_0x6Eu HEX,110u,110,(0,1,1,0,1,1,1,0),0x91u
#define PPUTLUTRAITS_0x6Du HEX,109u,109,(0,1,1,0,1,1,0,1),0x92u
#define PPUTLUTRAITS_0x6Cu HEX,108u,108,(0,1,1,0,1,1,0,0),0x93u
#define PPUTLUTRAITS_0x6Bu HEX,107u,107,(0,1,1,0,1,0,1,1),0x94u
#define PPUTLUTRAITS_0x6Au HEX,106u,106,(0,1,1,0,1,0,1,0),0x95u
#define PPUTLUTRAITS_0x69u HEX,105u,105,(0,1,1,0,1,0,0,1),0x96u
#define PPUTLUTRAITS_0x68u HEX,104u,104,(0,1,1,0,1,0,0,0),0x97u
#define PPUTLUTRAITS_0x67u HEX,103u,103,(0,1,1,0,0,1,1,1),0x98u
#define PPUTLUTRAITS_0x66u HEX,102u,102,(0,1,1,0,0,1,1,0),0x99u
#define PPUTLUTRAITS_0x65u HEX,101u,101,(0,1,1,0,0,1,0,1),0x9Au
#define PPUTLUTRAITS_0x64u HEX,100u,100,(0,1,1,0,0,1,0,0),0x9Bu
#define PPUTLUTRAITS_0x63u HEX,99u,99,(0,1,1,0,0,0,1,1),0x9Cu
#define PPUTLUTRAITS_0x62u HEX,98u,98,(0,1,1,0,0,0,1,0),0x9Du
#define PPUTLUTRAITS_0x61u HEX,97u,97,(0,1,1,0,0,0,0,1),0x9Eu
#define PPUTLUTRAITS_0x60u HEX,96u,96,(0,1,1,0,0,0,0,0),0x9Fu
#define PPUTLUTRAITS_0x5Fu HEX,95u,95,(0,1,0,1,1,1,1,1),0xA0u
#define PPUTLUTRAITS_0x5Eu HEX,94u,94,(0,1,0,1,1,1,1,0),0xA1u
#define PPUTLUTRAITS_0x5Du HEX,93u,93,(0,1,0,1,1,1,0,1),0xA2u
#define PPUTLUTRAITS_0x5Cu HEX,92u,92,(0,1,0,1,1,1,0,0),0xA3u
#define PPUTLUTRAITS_0x5Bu HEX,91u,91,(0,1,0,1,1,0,1,1),0xA4u
#define PPUTLUTRAITS_0x5Au HEX,90u,90,(0,1,0,1,1,0,1,0),0xA5u
#define PPUTLUTRAITS_0x59u HEX,89u,89,(0,1,0,1,1,0,0,1),0xA6u
#define PPUTLUTRAITS_0x58u HEX,88u,88,(0,1,0,1,1,0,0,0),0xA7u
#define PPUTLUTRAITS_0x57u HEX,87u,87,(0,1,0,1,0,1,1,1),0xA8u
#define PPUTLUTRAITS_0x56u HEX,86u,86,(0,1,0,1,0,1,1,0),0xA9u
#define PPUTLUTRAITS_0x55u HEX,85u,85,(0,1,0,1,0,1,0,1),0xAAu
#define PPUTLUTRAITS_0x54u HEX,84u,84,(0,1,0,1,0,1,0,0),0xABu
#define PPUTLUTRAITS_0x53u HEX,83u,83,(0,1,0,1,0,0,1,1),0xACu
#define PPUTLUTRAITS_0x52u HEX,82u,82,(0,1,0,1,0,0,1,0),0xADu
#define PPUTLUTRAITS_0x51u HEX,81u,81,(0,1,0,1,0,0,0,1),0xAEu
#define PPUTLUTRAITS_0x50u HEX,80u,80,(0,1,0,1,0,0,0,0),0xAFu
#define PPUTLUTRAITS_0x4Fu HEX,79u,79,(0,1,0,0,1,1,1,1),0xB0u
#define PPUTLUTRAITS_0x4Eu HEX,78u,78,(0,1,0,0,1,1,1,0),0xB1u
#define PPUTLUTRAITS_0x4Du HEX,77u,77,(0,1,0,0,1,1,0,1),0xB2u
#define PPUTLUTRAITS_0x4Cu HEX,76u,76,(0,1,0,0,1,1,0,0),0xB3u
#define PPUTLUTRAITS_0x4Bu HEX,75u,75,(0,1,0,0,1,0,1,1),0xB4u
#define PPUTLUTRAITS_0x4Au HEX,74u,74,(0,1,0,0,1,0,1,0),0xB5u
#define PPUTLUTRAITS_0x49u HEX,73u,73,(0,1,0,0,1,0,0,1),0xB6u
#define PPUTLUTRAITS_0x48u HEX,72u,72,(0,1,0,0,1,0,0,0),0xB7u
#define PPUTLUTRAITS_0x47u HEX,71u,71,(0,1,0,0,0,1,1,1),0xB8u
#define PPUTLUTRAITS_0x46u HEX,70u,70,(0,1,0,0,0,1,1,0),0xB9u
#define PPUTLUTRAITS_0x45u HEX,69u,69,(0,1,0,0,0,1,0,1),0xBAu
#define PPUTLUTRAITS_0x44u HEX,68u,68,(0,1,0,0,0,1,0,0),0xBBu
#define PPUTLUTRAITS_0x43u HEX,67u,67,(0,1,0,0,0,0,1,1),0xBCu
#define PPUTLUTRAITS_0x42u HEX,66u,66,(0,1,0,0,0,0,1,0),0xBDu
#define PPUTLUTRAITS_0x41u HEX,65u,65,(0,1,0,0,0,0,0,1),0xBEu
#define PPUTLUTRAITS_0x40u HEX,64u,64,(0,1,0,0,0,0,0,0),0xBFu
#define PPUTLUTRAITS_0x3Fu HEX,63u,63,(0,0,1,1,1,1,1,1),0xC0u
#define PPUTLUTRAITS_0x3Eu HEX,62u,62,(0,0,1,1,1,1,1,0),0xC1u
#define PPUTLUTRAITS_0x3Du HEX,61u,61,(0,0,1,1,1,1,0,1),0xC2u
#define PPUTLUTRAITS_0x3Cu HEX,60u,60,(0,0,1,1,1,1,0,0),0xC3u
#define PPUTLUTRAITS_0x3Bu HEX,59u,59,(0,0,1,1,1,0,1,1),0xC4u
#define PPUTLUTRAITS_0x3Au HEX,58u,58,(0,0,1,1,1,0,1,0),0xC5u
#define PPUTLUTRAITS_0x39u HEX,57u,57,(0,0,1,1,1,0,0,1),0xC6u
#define PPUTLUTRAITS_0x38u HEX,56u,56,(0,0,1,1,1,0,0,0),0xC7u
#define PPUTLUTRAITS_0x37u HEX,55u,55,(0,0,1,1,0,1,1,1),0xC8u
#define PPUTLUTRAITS_0x36u HEX,54u,54,(0,0,1,1,0,1,1,0),0xC9u
#define PPUTLUTRAITS_0x35u HEX,53u,53,(0,0,1,1,0,1,0,1),0xCAu
#define PPUTLUTRAITS_0x34u HEX,52u,52,(0,0,1,1,0,1,0,0),0xCBu
#define PPUTLUTRAITS_0x33u HEX,51u,51,(0,0,1,1,0,0,1,1),0xCCu
#define PPUTLUTRAITS_0x32u HEX,50u,50,(0,0,1,1,0,0,1,0),0xCDu
#define PPUTLUTRAITS_0x31u HEX,49u,49,(0,0,1,1,0,0,0,1),0xCEu
#define PPUTLUTRAITS_0x30u HEX,48u,48,(0,0,1,1,0,0,0,0),0xCFu
#define PPUTLUTRAITS_0x2Fu HEX,47u,47,(0,0,1,0,1,1,1,1),0xD0u
#define PPUTLUTRAITS_0x2Eu HEX,46u,46,(0,0,1,0,1,1,1,0),0xD1u
#define PPUTLUTRAITS_0x2Du HEX,45u,45,(0,0,1,0,1,1,0,1),0xD2u
#define PPUTLUTRAITS_0x2Cu HEX,44u,44,(0,0,1,0,1,1,0,0),0xD3u
#define PPUTLUTRAITS_0x2Bu HEX,43u,43,(0,0,1,0,1,0,1,1),0xD4u
#define PPUTLUTRAITS_0x2Au HEX,42u,42,(0,0,1,0,1,0,1,0),0xD5u
#define PPUTLUTRAITS_0x29u HEX,41u,41,(0,0,1,0,1,0,0,1),0xD6u
#define PPUTLUTRAITS_0x28u HEX,40u,40,(0,0,1,0,1,0,0,0),0xD7u
#define PPUTLUTRAITS_0x27u HEX,39u,39,(0,0,1,0,0,1,1,1),0xD8u
#define PPUTLUTRAITS_0x26u HEX,38u,38,(0,0,1,0,0,1,1,0),0xD9u
#define PPUTLUTRAITS_0x25u HEX,37u,37,(0,0,1,0,0,1,0,1),0xDAu
#define PPUTLUTRAITS_0x24u HEX,36u,36,(0,0,1,0,0,1,0,0),0xDBu
#define PPUTLUTRAITS_0x23u HEX,35u,35,(0,0,1,0,0,0,1,1),0xDCu
#define PPUTLUTRAITS_0x22u HEX,34u,34,(0,0,1,0,0,0,1,0),0xDDu
#define PPUTLUTRAITS_0x21u HEX,33u,33,(0,0,1,0,0,0,0,1),0xDEu
#define PPUTLUTRAITS_0x20u HEX,32u,32,(0,0,1,0,0,0,0,0),0xDFu
#define PPUTLUTRAITS_0x1Fu HEX,31u,31,(0,0,0,1,1,1,1,1),0xE0u
#define PPUTLUTRAITS_0x1Eu HEX,30u,30,(0,0,0,1,1,1,1,0),0xE1u
#define PPUTLUTRAITS_0x1Du HEX,29u,29,(0,0,0,1,1,1,0,1),0xE2u
#define PPUTLUTRAITS_0x1Cu HEX,28u,28,(0,0,0,1,1,1,0,0),0xE3u
#define PPUTLUTRAITS_0x1Bu HEX,27u,27,(0,0,0,1,1,0,1,1),0xE4u
#define PPUTLUTRAITS_0x1Au HEX,26u,26,(0,0,0,1,1,0,1,0),0xE5u
#define PPUTLUTRAITS_0x19u HEX,25u,25,(0,0,0,1,1,0,0,1),0xE6u
#define PPUTLUTRAITS_0x18u HEX,24u,24,(0,0,0,1,1,0,0,0),0xE7u
#define PPUTLUTRAITS_0x17u HEX,23u,23,(0,0,0,1,0,1,1,1),0xE8u
#define PPUTLUTRAITS_0x16u HEX,22u,22,(0,0,0,1,0,1,1,0),0xE9u
#define PPUTLUTRAITS_0x15u HEX,21u,21,(0,0,0,1,0,1,0,1),0xEAu
#define PPUTLUTRAITS_0x14u HEX,20u,20,(0,0,0,1,0,1,0,0),0xEBu
#define PPUTLUTRAITS_0x13u HEX,19u,19,(0,0,0,1,0,0,1,1),0xECu
#define PPUTLUTRAITS_0x12u HEX,18u,18,(0,0,0,1,0,0,1,0),0xEDu
#define PPUTLUTRAITS_0x11u HEX,17u,17,(0,0,0,1,0,0,0,1),0xEEu
#define PPUTLUTRAITS_0x10u HEX,16u,16,(0,0,0,1,0,0,0,0),0xEFu
#define PPUTLUTRAITS_0x0Fu HEX,15u,15,(0,0,0,0,1,1,1,1),0xF0u
#define PPUTLUTRAITS_0x0Eu HEX,14u,14,(0,0,0,0,1,1,1,0),0xF1u
#define PPUTLUTRAITS_0x0Du HEX,13u,13,(0,0,0,0,1,1,0,1),0xF2u
#define PPUTLUTRAITS_0x0Cu HEX,12u,12,(0,0,0,0,1,1,0,0),0xF3u
#define PPUTLUTRAITS_0x0Bu HEX,11u,11,(0,0,0,0,1,0,1,1),0xF4u
#define PPUTLUTRAITS_0x0Au HEX,10u,10,(0,0,0,0,1,0,1,0),0xF5u
#define PPUTLUTRAITS_0x09u HEX,9u,9,(0,0,0,0,1,0,0,1),0xF6u
#define PPUTLUTRAITS_0x08u HEX,8u,8,(0,0,0,0,1,0,0,0),0xF7u
#define PPUTLUTRAITS_0x07u HEX,7u,7,(0,0,0,0,0,1,1,1),0xF8u
#define PPUTLUTRAITS_0x06u HEX,6u,6,(0,0,0,0,0,1,1,0),0xF9u
#define PPUTLUTRAITS_0x05u HEX,5u,5,(0,0,0,0,0,1,0,1),0xFAu
#define PPUTLUTRAITS_0x04u HEX,4u,4,(0,0,0,0,0,1,0,0),0xFBu
#define PPUTLUTRAITS_0x03u HEX,3u,3,(0,0,0,0,0,0,1,1),0xFCu
#define PPUTLUTRAITS_0x02u HEX,2u,2,(0,0,0,0,0,0,1,0),0xFDu
#define PPUTLUTRAITS_0x01u HEX,1u,1,(0,0,0,0,0,0,0,1),0xFEu
#define PPUTLUTRAITS_0x00u HEX,0u,0,(0,0,0,0,0,0,0,0),0xFFu

/// type, signed hex, signed is negative, log2, sqrt, factors
#define PPUTLUTRAITS_255u DEC,0xFF,1,7u,15u,(3u, 5u, 17u)
#define PPUTLUTRAITS_254u DEC,0xFE,1,7u,15u,(2u, 127u)
#define PPUTLUTRAITS_253u DEC,0xFD,1,7u,15u,(11u, 23u)
#define PPUTLUTRAITS_252u DEC,0xFC,1,7u,15u,(2u, 2u, 3u, 3u, 7u)
#define PPUTLUTRAITS_251u DEC,0xFB,1,7u,15u,()
#define PPUTLUTRAITS_250u DEC,0xFA,1,7u,15u,(2u, 5u, 5u, 5u)
#define PPUTLUTRAITS_249u DEC,0xF9,1,7u,15u,(3u, 83u)
#define PPUTLUTRAITS_248u DEC,0xF8,1,7u,15u,(2u, 2u, 2u, 31u)
#define PPUTLUTRAITS_247u DEC,0xF7,1,7u,15u,(13u, 19u)
#define PPUTLUTRAITS_246u DEC,0xF6,1,7u,15u,(2u, 3u, 41u)
#define PPUTLUTRAITS_245u DEC,0xF5,1,7u,15u,(5u, 7u, 7u)
#define PPUTLUTRAITS_244u DEC,0xF4,1,7u,15u,(2u, 2u, 61u)
#define PPUTLUTRAITS_243u DEC,0xF3,1,7u,15u,(3u, 3u, 3u, 3u, 3u)
#define PPUTLUTRAITS_242u DEC,0xF2,1,7u,15u,(2u, 11u, 11u)
#define PPUTLUTRAITS_241u DEC,0xF1,1,7u,15u,()
#define PPUTLUTRAITS_240u DEC,0xF0,1,7u,15u,(2u, 2u, 2u, 2u, 3u, 5u)
#define PPUTLUTRAITS_239u DEC,0xEF,1,7u,15u,()
#define PPUTLUTRAITS_238u DEC,0xEE,1,7u,15u,(2u, 7u, 17u)
#define PPUTLUTRAITS_237u DEC,0xED,1,7u,15u,(3u, 79u)
#define PPUTLUTRAITS_236u DEC,0xEC,1,7u,15u,(2u, 2u, 59u)
#define PPUTLUTRAITS_235u DEC,0xEB,1,7u,15u,(5u, 47u)
#define PPUTLUTRAITS_234u DEC,0xEA,1,7u,15u,(2u, 3u, 3u, 13u)
#define PPUTLUTRAITS_233u DEC,0xE9,1,7u,15u,()
#define PPUTLUTRAITS_232u DEC,0xE8,1,7u,15u,(2u, 2u, 2u, 29u)
#define PPUTLUTRAITS_231u DEC,0xE7,1,7u,15u,(3u, 7u, 11u)
#define PPUTLUTRAITS_230u DEC,0xE6,1,7u,15u,(2u, 5u, 23u)
#define PPUTLUTRAITS_229u DEC,0xE5,1,7u,15u,()
#define PPUTLUTRAITS_228u DEC,0xE4,1,7u,15u,(2u, 2u, 3u, 19u)
#define PPUTLUTRAITS_227u DEC,0xE3,1,7u,15u,()
#define PPUTLUTRAITS_226u DEC,0xE2,1,7u,15u,(2u, 113u)
#define PPUTLUTRAITS_225u DEC,0xE1,1,7u,15u,(3u, 3u, 5u, 5u)
#define PPUTLUTRAITS_224u DEC,0xE0,1,7u,14u,(2u, 2u, 2u, 2u, 2u, 7u)
#define PPUTLUTRAITS_223u DEC,0xDF,1,7u,14u,()
#define PPUTLUTRAITS_222u DEC,0xDE,1,7u,14u,(2u, 3u, 37u)
#define PPUTLUTRAITS_221u DEC,0xDD,1,7u,14u,(13u, 17u)
#define PPUTLUTRAITS_220u DEC,0xDC,1,7u,14u,(2u, 2u, 5u, 11u)
#define PPUTLUTRAITS_219u DEC,0xDB,1,7u,14u,(3u, 73u)
#define PPUTLUTRAITS_218u DEC,0xDA,1,7u,14u,(2u, 109u)
#define PPUTLUTRAITS_217u DEC,0xD9,1,7u,14u,(7u, 31u)
#define PPUTLUTRAITS_216u DEC,0xD8,1,7u,14u,(2u, 2u, 2u, 3u, 3u, 3u)
#define PPUTLUTRAITS_215u DEC,0xD7,1,7u,14u,(5u, 43u)
#define PPUTLUTRAITS_214u DEC,0xD6,1,7u,14u,(2u, 107u)
#define PPUTLUTRAITS_213u DEC,0xD5,1,7u,14u,(3u, 71u)
#define PPUTLUTRAITS_212u DEC,0xD4,1,7u,14u,(2u, 2u, 53u)
#define PPUTLUTRAITS_211u DEC,0xD3,1,7u,14u,()
#define PPUTLUTRAITS_210u DEC,0xD2,1,7u,14u,(2u, 3u, 5u, 7u)
#define PPUTLUTRAITS_209u DEC,0xD1,1,7u,14u,(11u, 19u)
#define PPUTLUTRAITS_208u DEC,0xD0,1,7u,14u,(2u, 2u, 2u, 2u, 13u)
#define PPUTLUTRAITS_207u DEC,0xCF,1,7u,14u,(3u, 3u, 23u)
#define PPUTLUTRAITS_206u DEC,0xCE,1,7u,14u,(2u, 103u)
#define PPUTLUTRAITS_205u DEC,0xCD,1,7u,14u,(5u, 41u)
#define PPUTLUTRAITS_204u DEC,0xCC,1,7u,14u,(2u, 2u, 3u, 17u)
#define PPUTLUTRAITS_203u DEC,0xCB,1,7u,14u,(7u, 29u)
#define PPUTLUTRAITS_202u DEC,0xCA,1,7u,14u,(2u, 101u)
#define PPUTLUTRAITS_201u DEC,0xC9,1,7u,14u,(3u, 67u)
#define PPUTLUTRAITS_200u DEC,0xC8,1,7u,14u,(2u, 2u, 2u, 5u, 5u)
#define PPUTLUTRAITS_199u DEC,0xC7,1,7u,14u,()
#define PPUTLUTRAITS_198u DEC,0xC6,1,7u,14u,(2u, 3u, 3u, 11u)
#define PPUTLUTRAITS_197u DEC,0xC5,1,7u,14u,()
#define PPUTLUTRAITS_196u DEC,0xC4,1,7u,14u,(2u, 2u, 7u, 7u)
#define PPUTLUTRAITS_195u DEC,0xC3,1,7u,13u,(3u, 5u, 13u)
#define PPUTLUTRAITS_194u DEC,0xC2,1,7u,13u,(2u, 97u)
#define PPUTLUTRAITS_193u DEC,0xC1,1,7u,13u,()
#define PPUTLUTRAITS_192u DEC,0xC0,1,7u,13u,(2u, 2u, 2u, 2u, 2u, 2u, 3u)
#define PPUTLUTRAITS_191u DEC,0xBF,1,7u,13u,()
#define PPUTLUTRAITS_190u DEC,0xBE,1,7u,13u,(2u, 5u, 19u)
#define PPUTLUTRAITS_189u DEC,0xBD,1,7u,13u,(3u, 3u, 3u, 7u)
#define PPUTLUTRAITS_188u DEC,0xBC,1,7u,13u,(2u, 2u, 47u)
#define PPUTLUTRAITS_187u DEC,0xBB,1,7u,13u,(11u, 17u)
#define PPUTLUTRAITS_186u DEC,0xBA,1,7u,13u,(2u, 3u, 31u)
#define PPUTLUTRAITS_185u DEC,0xB9,1,7u,13u,(5u, 37u)
#define PPUTLUTRAITS_184u DEC,0xB8,1,7u,13u,(2u, 2u, 2u, 23u)
#define PPUTLUTRAITS_183u DEC,0xB7,1,7u,13u,(3u, 61u)
#define PPUTLUTRAITS_182u DEC,0xB6,1,7u,13u,(2u, 7u, 13u)
#define PPUTLUTRAITS_181u DEC,0xB5,1,7u,13u,()
#define PPUTLUTRAITS_180u DEC,0xB4,1,7u,13u,(2u, 2u, 3u, 3u, 5u)
#define PPUTLUTRAITS_179u DEC,0xB3,1,7u,13u,()
#define PPUTLUTRAITS_178u DEC,0xB2,1,7u,13u,(2u, 89u)
#define PPUTLUTRAITS_177u DEC,0xB1,1,7u,13u,(3u, 59u)
#define PPUTLUTRAITS_176u DEC,0xB0,1,7u,13u,(2u, 2u, 2u, 2u, 11u)
#define PPUTLUTRAITS_175u DEC,0xAF,1,7u,13u,(5u, 5u, 7u)
#define PPUTLUTRAITS_174u DEC,0xAE,1,7u,13u,(2u, 3u, 29u)
#define PPUTLUTRAITS_173u DEC,0xAD,1,7u,13u,()
#define PPUTLUTRAITS_172u DEC,0xAC,1,7u,13u,(2u, 2u, 43u)
#define PPUTLUTRAITS_171u DEC,0xAB,1,7u,13u,(3u, 3u, 19u)
#define PPUTLUTRAITS_170u DEC,0xAA,1,7u,13u,(2u, 5u, 17u)
#define PPUTLUTRAITS_169u DEC,0xA9,1,7u,13u,(13u, 13u)
#define PPUTLUTRAITS_168u DEC,0xA8,1,7u,12u,(2u, 2u, 2u, 3u, 7u)
#define PPUTLUTRAITS_167u DEC,0xA7,1,7u,12u,()
#define PPUTLUTRAITS_166u DEC,0xA6,1,7u,12u,(2u, 83u)
#define PPUTLUTRAITS_165u DEC,0xA5,1,7u,12u,(3u, 5u, 11u)
#define PPUTLUTRAITS_164u DEC,0xA4,1,7u,12u,(2u, 2u, 41u)
#define PPUTLUTRAITS_163u DEC,0xA3,1,7u,12u,()
#define PPUTLUTRAITS_162u DEC,0xA2,1,7u,12u,(2u, 3u, 3u, 3u, 3u)
#define PPUTLUTRAITS_161u DEC,0xA1,1,7u,12u,(7u, 23u)
#define PPUTLUTRAITS_160u DEC,0xA0,1,7u,12u,(2u, 2u, 2u, 2u, 2u, 5u)
#define PPUTLUTRAITS_159u DEC,0x9F,1,7u,12u,(3u, 53u)
#define PPUTLUTRAITS_158u DEC,0x9E,1,7u,12u,(2u, 79u)
#define PPUTLUTRAITS_157u DEC,0x9D,1,7u,12u,()
#define PPUTLUTRAITS_156u DEC,0x9C,1,7u,12u,(2u, 2u, 3u, 13u)
#define PPUTLUTRAITS_155u DEC,0x9B,1,7u,12u,(5u, 31u)
#define PPUTLUTRAITS_154u DEC,0x9A,1,7u,12u,(2u, 7u, 11u)
#define PPUTLUTRAITS_153u DEC,0x99,1,7u,12u,(3u, 3u, 17u)
#define PPUTLUTRAITS_152u DEC,0x98,1,7u,12u,(2u, 2u, 2u, 19u)
#define PPUTLUTRAITS_151u DEC,0x97,1,7u,12u,()
#define PPUTLUTRAITS_150u DEC,0x96,1,7u,12u,(2u, 3u, 5u, 5u)
#define PPUTLUTRAITS_149u DEC,0x95,1,7u,12u,()
#define PPUTLUTRAITS_148u DEC,0x94,1,7u,12u,(2u, 2u, 37u)
#define PPUTLUTRAITS_147u DEC,0x93,1,7u,12u,(3u, 7u, 7u)
#define PPUTLUTRAITS_146u DEC,0x92,1,7u,12u,(2u, 73u)
#define PPUTLUTRAITS_145u DEC,0x91,1,7u,12u,(5u, 29u)
#define PPUTLUTRAITS_144u DEC,0x90,1,7u,12u,(2u, 2u, 2u, 2u, 3u, 3u)
#define PPUTLUTRAITS_143u DEC,0x8F,1,7u,11u,(11u, 13u)
#define PPUTLUTRAITS_142u DEC,0x8E,1,7u,11u,(2u, 71u)
#define PPUTLUTRAITS_141u DEC,0x8D,1,7u,11u,(3u, 47u)
#define PPUTLUTRAITS_140u DEC,0x8C,1,7u,11u,(2u, 2u, 5u, 7u)
#define PPUTLUTRAITS_139u DEC,0x8B,1,7u,11u,()
#define PPUTLUTRAITS_138u DEC,0x8A,1,7u,11u,(2u, 3u, 23u)
#define PPUTLUTRAITS_137u DEC,0x89,1,7u,11u,()
#define PPUTLUTRAITS_136u DEC,0x88,1,7u,11u,(2u, 2u, 2u, 17u)
#define PPUTLUTRAITS_135u DEC,0x87,1,7u,11u,(3u, 3u, 3u, 5u)
#define PPUTLUTRAITS_134u DEC,0x86,1,7u,11u,(2u, 67u)
#define PPUTLUTRAITS_133u DEC,0x85,1,7u,11u,(7u, 19u)
#define PPUTLUTRAITS_132u DEC,0x84,1,7u,11u,(2u, 2u, 3u, 11u)
#define PPUTLUTRAITS_131u DEC,0x83,1,7u,11u,()
#define PPUTLUTRAITS_130u DEC,0x82,1,7u,11u,(2u, 5u, 13u)
#define PPUTLUTRAITS_129u DEC,0x81,1,7u,11u,(3u, 43u)
#define PPUTLUTRAITS_128u DEC,0x80,1,7u,11u,(2u, 2u, 2u, 2u, 2u, 2u, 2u)
#define PPUTLUTRAITS_127u DEC,0x7F,0,6u,11u,()
#define PPUTLUTRAITS_126u DEC,0x7E,0,6u,11u,(2u, 3u, 3u, 7u)
#define PPUTLUTRAITS_125u DEC,0x7D,0,6u,11u,(5u, 5u, 5u)
#define PPUTLUTRAITS_124u DEC,0x7C,0,6u,11u,(2u, 2u, 31u)
#define PPUTLUTRAITS_123u DEC,0x7B,0,6u,11u,(3u, 41u)
#define PPUTLUTRAITS_122u DEC,0x7A,0,6u,11u,(2u, 61u)
#define PPUTLUTRAITS_121u DEC,0x79,0,6u,11u,(11u, 11u)
#define PPUTLUTRAITS_120u DEC,0x78,0,6u,10u,(2u, 2u, 2u, 3u, 5u)
#define PPUTLUTRAITS_119u DEC,0x77,0,6u,10u,(7u, 17u)
#define PPUTLUTRAITS_118u DEC,0x76,0,6u,10u,(2u, 59u)
#define PPUTLUTRAITS_117u DEC,0x75,0,6u,10u,(3u, 3u, 13u)
#define PPUTLUTRAITS_116u DEC,0x74,0,6u,10u,(2u, 2u, 29u)
#define PPUTLUTRAITS_115u DEC,0x73,0,6u,10u,(5u, 23u)
#define PPUTLUTRAITS_114u DEC,0x72,0,6u,10u,(2u, 3u, 19u)
#define PPUTLUTRAITS_113u DEC,0x71,0,6u,10u,()
#define PPUTLUTRAITS_112u DEC,0x70,0,6u,10u,(2u, 2u, 2u, 2u, 7u)
#define PPUTLUTRAITS_111u DEC,0x6F,0,6u,10u,(3u, 37u)
#define PPUTLUTRAITS_110u DEC,0x6E,0,6u,10u,(2u, 5u, 11u)
#define PPUTLUTRAITS_109u DEC,0x6D,0,6u,10u,()
#define PPUTLUTRAITS_108u DEC,0x6C,0,6u,10u,(2u, 2u, 3u, 3u, 3u)
#define PPUTLUTRAITS_107u DEC,0x6B,0,6u,10u,()
#define PPUTLUTRAITS_106u DEC,0x6A,0,6u,10u,(2u, 53u)
#define PPUTLUTRAITS_105u DEC,0x69,0,6u,10u,(3u, 5u, 7u)
#define PPUTLUTRAITS_104u DEC,0x68,0,6u,10u,(2u, 2u, 2u, 13u)
#define PPUTLUTRAITS_103u DEC,0x67,0,6u,10u,()
#define PPUTLUTRAITS_102u DEC,0x66,0,6u,10u,(2u, 3u, 17u)
#define PPUTLUTRAITS_101u DEC,0x65,0,6u,10u,()
#define PPUTLUTRAITS_100u DEC,0x64,0,6u,10u,(2u, 2u, 5u, 5u)
#define PPUTLUTRAITS_99u DEC,0x63,0,6u,9u,(3u, 3u, 11u)
#define PPUTLUTRAITS_98u DEC,0x62,0,6u,9u,(2u, 7u, 7u)
#define PPUTLUTRAITS_97u DEC,0x61,0,6u,9u,()
#define PPUTLUTRAITS_96u DEC,0x60,0,6u,9u,(2u, 2u, 2u, 2u, 2u, 3u)
#define PPUTLUTRAITS_95u DEC,0x5F,0,6u,9u,(5u, 19u)
#define PPUTLUTRAITS_94u DEC,0x5E,0,6u,9u,(2u, 47u)
#define PPUTLUTRAITS_93u DEC,0x5D,0,6u,9u,(3u, 31u)
#define PPUTLUTRAITS_92u DEC,0x5C,0,6u,9u,(2u, 2u, 23u)
#define PPUTLUTRAITS_91u DEC,0x5B,0,6u,9u,(7u, 13u)
#define PPUTLUTRAITS_90u DEC,0x5A,0,6u,9u,(2u, 3u, 3u, 5u)
#define PPUTLUTRAITS_89u DEC,0x59,0,6u,9u,()
#define PPUTLUTRAITS_88u DEC,0x58,0,6u,9u,(2u, 2u, 2u, 11u)
#define PPUTLUTRAITS_87u DEC,0x57,0,6u,9u,(3u, 29u)
#define PPUTLUTRAITS_86u DEC,0x56,0,6u,9u,(2u, 43u)
#define PPUTLUTRAITS_85u DEC,0x55,0,6u,9u,(5u, 17u)
#define PPUTLUTRAITS_84u DEC,0x54,0,6u,9u,(2u, 2u, 3u, 7u)
#define PPUTLUTRAITS_83u DEC,0x53,0,6u,9u,()
#define PPUTLUTRAITS_82u DEC,0x52,0,6u,9u,(2u, 41u)
#define PPUTLUTRAITS_81u DEC,0x51,0,6u,9u,(3u, 3u, 3u, 3u)
#define PPUTLUTRAITS_80u DEC,0x50,0,6u,8u,(2u, 2u, 2u, 2u, 5u)
#define PPUTLUTRAITS_79u DEC,0x4F,0,6u,8u,()
#define PPUTLUTRAITS_78u DEC,0x4E,0,6u,8u,(2u, 3u, 13u)
#define PPUTLUTRAITS_77u DEC,0x4D,0,6u,8u,(7u, 11u)
#define PPUTLUTRAITS_76u DEC,0x4C,0,6u,8u,(2u, 2u, 19u)
#define PPUTLUTRAITS_75u DEC,0x4B,0,6u,8u,(3u, 5u, 5u)
#define PPUTLUTRAITS_74u DEC,0x4A,0,6u,8u,(2u, 37u)
#define PPUTLUTRAITS_73u DEC,0x49,0,6u,8u,()
#define PPUTLUTRAITS_72u DEC,0x48,0,6u,8u,(2u, 2u, 2u, 3u, 3u)
#define PPUTLUTRAITS_71u DEC,0x47,0,6u,8u,()
#define PPUTLUTRAITS_70u DEC,0x46,0,6u,8u,(2u, 5u, 7u)
#define PPUTLUTRAITS_69u DEC,0x45,0,6u,8u,(3u, 23u)
#define PPUTLUTRAITS_68u DEC,0x44,0,6u,8u,(2u, 2u, 17u)
#define PPUTLUTRAITS_67u DEC,0x43,0,6u,8u,()
#define PPUTLUTRAITS_66u DEC,0x42,0,6u,8u,(2u, 3u, 11u)
#define PPUTLUTRAITS_65u DEC,0x41,0,6u,8u,(5u, 13u)
#define PPUTLUTRAITS_64u DEC,0x40,0,6u,8u,(2u, 2u, 2u, 2u, 2u, 2u)
#define PPUTLUTRAITS_63u DEC,0x3F,0,5u,7u,(3u, 3u, 7u)
#define PPUTLUTRAITS_62u DEC,0x3E,0,5u,7u,(2u, 31u)
#define PPUTLUTRAITS_61u DEC,0x3D,0,5u,7u,()
#define PPUTLUTRAITS_60u DEC,0x3C,0,5u,7u,(2u, 2u, 3u, 5u)
#define PPUTLUTRAITS_59u DEC,0x3B,0,5u,7u,()
#define PPUTLUTRAITS_58u DEC,0x3A,0,5u,7u,(2u, 29u)
#define PPUTLUTRAITS_57u DEC,0x39,0,5u,7u,(3u, 19u)
#define PPUTLUTRAITS_56u DEC,0x38,0,5u,7u,(2u, 2u, 2u, 7u)
#define PPUTLUTRAITS_55u DEC,0x37,0,5u,7u,(5u, 11u)
#define PPUTLUTRAITS_54u DEC,0x36,0,5u,7u,(2u, 3u, 3u, 3u)
#define PPUTLUTRAITS_53u DEC,0x35,0,5u,7u,()
#define PPUTLUTRAITS_52u DEC,0x34,0,5u,7u,(2u, 2u, 13u)
#define PPUTLUTRAITS_51u DEC,0x33,0,5u,7u,(3u, 17u)
#define PPUTLUTRAITS_50u DEC,0x32,0,5u,7u,(2u, 5u, 5u)
#define PPUTLUTRAITS_49u DEC,0x31,0,5u,7u,(7u, 7u)
#define PPUTLUTRAITS_48u DEC,0x30,0,5u,6u,(2u, 2u, 2u, 2u, 3u)
#define PPUTLUTRAITS_47u DEC,0x2F,0,5u,6u,()
#define PPUTLUTRAITS_46u DEC,0x2E,0,5u,6u,(2u, 23u)
#define PPUTLUTRAITS_45u DEC,0x2D,0,5u,6u,(3u, 3u, 5u)
#define PPUTLUTRAITS_44u DEC,0x2C,0,5u,6u,(2u, 2u, 11u)
#define PPUTLUTRAITS_43u DEC,0x2B,0,5u,6u,()
#define PPUTLUTRAITS_42u DEC,0x2A,0,5u,6u,(2u, 3u, 7u)
#define PPUTLUTRAITS_41u DEC,0x29,0,5u,6u,()
#define PPUTLUTRAITS_40u DEC,0x28,0,5u,6u,(2u, 2u, 2u, 5u)
#define PPUTLUTRAITS_39u DEC,0x27,0,5u,6u,(3u, 13u)
#define PPUTLUTRAITS_38u DEC,0x26,0,5u,6u,(2u, 19u)
#define PPUTLUTRAITS_37u DEC,0x25,0,5u,6u,()
#define PPUTLUTRAITS_36u DEC,0x24,0,5u,6u,(2u, 2u, 3u, 3u)
#define PPUTLUTRAITS_35u DEC,0x23,0,5u,5u,(5u, 7u)
#define PPUTLUTRAITS_34u DEC,0x22,0,5u,5u,(2u, 17u)
#define PPUTLUTRAITS_33u DEC,0x21,0,5u,5u,(3u, 11u)
#define PPUTLUTRAITS_32u DEC,0x20,0,5u,5u,(2u, 2u, 2u, 2u, 2u)
#define PPUTLUTRAITS_31u DEC,0x1F,0,4u,5u,()
#define PPUTLUTRAITS_30u DEC,0x1E,0,4u,5u,(2u, 3u, 5u)
#define PPUTLUTRAITS_29u DEC,0x1D,0,4u,5u,()
#define PPUTLUTRAITS_28u DEC,0x1C,0,4u,5u,(2u, 2u, 7u)
#define PPUTLUTRAITS_27u DEC,0x1B,0,4u,5u,(3u, 3u, 3u)
#define PPUTLUTRAITS_26u DEC,0x1A,0,4u,5u,(2u, 13u)
#define PPUTLUTRAITS_25u DEC,0x19,0,4u,5u,(5u, 5u)
#define PPUTLUTRAITS_24u DEC,0x18,0,4u,4u,(2u, 2u, 2u, 3u)
#define PPUTLUTRAITS_23u DEC,0x17,0,4u,4u,()
#define PPUTLUTRAITS_22u DEC,0x16,0,4u,4u,(2u, 11u)
#define PPUTLUTRAITS_21u DEC,0x15,0,4u,4u,(3u, 7u)
#define PPUTLUTRAITS_20u DEC,0x14,0,4u,4u,(2u, 2u, 5u)
#define PPUTLUTRAITS_19u DEC,0x13,0,4u,4u,()
#define PPUTLUTRAITS_18u DEC,0x12,0,4u,4u,(2u, 3u, 3u)
#define PPUTLUTRAITS_17u DEC,0x11,0,4u,4u,()
#define PPUTLUTRAITS_16u DEC,0x10,0,4u,4u,(2u, 2u, 2u, 2u)
#define PPUTLUTRAITS_15u DEC,0x0F,0,3u,3u,(3u, 5u)
#define PPUTLUTRAITS_14u DEC,0x0E,0,3u,3u,(2u, 7u)
#define PPUTLUTRAITS_13u DEC,0x0D,0,3u,3u,()
#define PPUTLUTRAITS_12u DEC,0x0C,0,3u,3u,(2u, 2u, 3u)
#define PPUTLUTRAITS_11u DEC,0x0B,0,3u,3u,()
#define PPUTLUTRAITS_10u DEC,0x0A,0,3u,3u,(2u, 5u)
#define PPUTLUTRAITS_9u DEC,0x09,0,3u,3u,(3u, 3u)
#define PPUTLUTRAITS_8u DEC,0x08,0,3u,2u,(2u, 2u, 2u)
#define PPUTLUTRAITS_7u DEC,0x07,0,2u,2u,()
#define PPUTLUTRAITS_6u DEC,0x06,0,2u,2u,(2u, 3u)
#define PPUTLUTRAITS_5u DEC,0x05,0,2u,2u,()
#define PPUTLUTRAITS_4u DEC,0x04,0,2u,2u,(2u, 2u)
#define PPUTLUTRAITS_3u DEC,0x03,0,1u,1u,()
#define PPUTLUTRAITS_2u DEC,0x02,0,1u,1u,()
#define PPUTLUTRAITS_1u DEC,0x01,0,0u,1u,()
#define PPUTLUTRAITS_0u DEC,0x00,0,,0u,()


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

// clang-format on

/// [type.int]
/// ----------
/// 8-bit signed integer type.
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
#define PTL_INT(/* n: uint|int|nybl */...) /* -> int{n} */                                       \
  PPUTLINT_o(__VA_ARGS__.)(__VA_ARGS__)(__VA_ARGS__)(__VA_ARGS__)(PTL_ISTR([PTL_INT] invalid int \
                                                                           : __VA_ARGS__),       \
                                                                  __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

/// first parentheses; asserts only one arg.
#define PPUTLINT_o(_, ...)      PPUTLINT_o_##__VA_OPT__(NO_)##PASS()
#define PPUTLINT_o_NO_PASS(...) PPUTLINT_oo_FAIL
#define PPUTLINT_o_PASS(...)    PPUTLINT_oo

/// second parentheses; asserts non-tuple.
#define PPUTLINT_oo(_, ...)      PPUTLINT_oo_RES(PTL_EAT _)
#define PPUTLINT_oo_RES(...)     PPUTLINT_oo_##__VA_OPT__(NO_)##FAIL()
#define PPUTLINT_oo_NO_FAIL(...) PPUTLINT_ooo
#define PPUTLINT_oo_FAIL(...)    PPUTLINT_ooo_FAIL

/// third parentheses; attempts cast from signed.
#define PPUTLINT_ooo(...)             PPUTLINT_ooo_RES(__VA_ARGS__##u, PPUTLUTRAITS_##__VA_ARGS__##u)
#define PPUTLINT_ooo_RES(u, ...)      PPUTLINT_ooo_RES_o(u, __VA_ARGS__)
#define PPUTLINT_ooo_RES_o(u, _, ...) PPUTLINT_ooo_##__VA_OPT__(NO_)##FAIL(u)
#define PPUTLINT_ooo_NO_FAIL(u)       PTL_CAT(PPUTLINT_ooo_ICHK_, PPUTLUINT_TRAIT(u, TYPE))(u)
#define PPUTLINT_ooo_FAIL(u)          PPUTLINT_oooo
#define PPUTLINT_ooo_ICHK_HEX(u)      PPUTLINT_oooo_IPASS
#define PPUTLINT_ooo_ICHK_DEC(u)      PTL_CAT(PPUTLINT_ooo_ICHK_DEC_, PPUTLUINT_TRAIT(u, DEC_INEG))
#define PPUTLINT_ooo_ICHK_DEC_1       PPUTLINT_oooo_FAIL
#define PPUTLINT_ooo_ICHK_DEC_0       PPUTLINT_oooo_IPASS

/// fourth parentheses; attempts cast from unsigned.
#define PPUTLINT_oooo(...)          PPUTLINT_oooo_RES(PPUTLUTRAITS_##__VA_ARGS__)
#define PPUTLINT_oooo_RES(...)      PPUTLINT_oooo_RES_o(__VA_ARGS__)
#define PPUTLINT_oooo_RES_o(_, ...) PPUTLINT_oooo_##__VA_OPT__(NO_)##FAIL()
#define PPUTLINT_oooo_IPASS(...)    PPUTLINT_IPASS
#define PPUTLINT_oooo_NO_FAIL(...)  PPUTLINT_UPASS
#define PPUTLINT_oooo_FAIL(...)     PPUTLINT_FAIL

/// final parentheses (cast from signed)
#define PPUTLINT_IPASS(e, i) i

/// final parentheses (cast from unsigned)
#define PPUTLINT_UPASS(e, u)            PTL_CAT(PPUTLINT_UPASS_, PPUTLUINT_TRAIT(u, TYPE))(u)
#define PPUTLINT_UPASS_HEX(u)           PPUTLUINT_TRAIT(PPUTLUINT_TRAIT(u, HEX_UDEC), DEC_IHEX)
#define PPUTLINT_UPASS_DEC(u)           PPUTLINT_UPASS_DEC_o(u, PPUTLUINT_TRAIT(u, DEC_IHEX))
#define PPUTLINT_UPASS_DEC_o(u, ibin)   PPUTLINT_UPASS_DEC_oo(u, ibin)
#define PPUTLINT_UPASS_DEC_oo(u_, ibin) PPUTLINT_UPASS_DEC_ooo(u_, ibin, ibin##u)
#define PPUTLINT_UPASS_DEC_ooo(u, ibin, ubin) \
  PTL_CAT(PPUTLINT_UPASS_DEC_, PTL_ESC(PTL_IFIRST PPUTLUINT_TRAIT(ubin, HEX_BITS)))(u, ibin, ubin)
#define PPUTLINT_UPASS_DEC_1(u, ibin, ubin) ibin
#define PPUTLINT_UPASS_DEC_0(u, ibin, ubin) PPUTLUINT_TRAIT(ubin, HEX_IDEC)

/// final parentheses (fail)
#define PPUTLINT_FAIL(e, ...) PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.uhex]
/// -----------
/// casts to the unsigned int hexadecimal subtype.
///
/// PTL_UHEX(0)     // 0x00u
/// PTL_UHEX(1)     // 0x01u
/// PTL_UHEX(5)     // 0x05u
/// PTL_UHEX(255u)  // 0xFFu
/// PTL_UHEX(0x00u) // 0x00u
/// PTL_UHEX(0x01u) // 0x01u
/// PTL_UHEX(0xFF)  // 0xFFu
#define PTL_UHEX(/* n: uint|int|nybl */...) /* -> uhex{n} */ PPUTLUHEX_o(PTL_UINT(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLUHEX_o(n)   PTL_CAT(PPUTLUHEX_, PPUTLUINT_TRAIT(n, TYPE))(n)
#define PPUTLUHEX_HEX(n) n
#define PPUTLUHEX_DEC(n) PTL_CAT(PPUTLUINT_TRAIT(n, DEC_IHEX), u)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.ihex]
/// -----------
/// casts to the signed int binary subtype.
///
/// PTL_IHEX(0)    // 0x00
/// PTL_IHEX(1)    // 0x01
/// PTL_IHEX(5)    // 0x05
/// PTL_IHEX(255u) // 0xFF
/// PTL_IHEX(127u) // 0x7F
#define PTL_IHEX(/* n: uint|int|nybl */...) /* -> ihex{n} */ PPUTLIHEX_o(PTL_INT(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIHEX_o(n)      PPUTLIHEX_oo(n, PTL_CAT(n, u))
#define PPUTLIHEX_oo(n, u)  PTL_CAT(PPUTLIHEX_, PPUTLUINT_TRAIT(u, TYPE))(n, u)
#define PPUTLIHEX_HEX(n, u) n
#define PPUTLIHEX_DEC(n, u) PPUTLUINT_TRAIT(u, DEC_IHEX)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.udec]
/// -----------
/// casts to the unsigned int decimal subtype.
///
/// PTL_UDEC(0x00u) // 0u
/// PTL_UDEC(1)     // 1u
/// PTL_UDEC(5)     // 5u
/// PTL_UDEC(0x05u) // 5u
/// PTL_UDEC(0xFFu) // 255u
/// PTL_UDEC(0xFF)  // 255u
#define PTL_UDEC(/* n: uint|int|nybl */...) /* -> udec{n} */ PPUTLUDEC_o(PTL_UINT(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLUDEC_o(n)   PTL_CAT(PPUTLUDEC_, PPUTLUINT_TRAIT(n, TYPE))(n)
#define PPUTLUDEC_HEX(n) PPUTLUINT_TRAIT(n, HEX_UDEC)
#define PPUTLUDEC_DEC(n) n

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.idec]
/// -----------
/// [positive-only] casts to the signed int decimal subtype.
/// fails on negative ints.
///
/// use fmt.paste with ihex to get negative decimals.
///
/// PTL_IDEC(0x00)  // 0
/// PTL_IDEC(0x01)  // 1
/// PTL_IDEC(0x05u) // 5
/// PTL_IDEC(0x7F)  // 127
/// PTL_IDEC(127)   // 127
#define PTL_IDEC(/* n: uint|int|nybl */...) /* -> idec{n} */          \
  PPUTLIDEC_o(PTL_ISTR([PTL_IDEC] cannot represent negative in base10 \
                       : __VA_ARGS__),                                \
              PTL_INT(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIDEC_o(e, n)     PPUTLIDEC_oo(e, n, PTL_CAT(n, u))
#define PPUTLIDEC_oo(e, n, u) PTL_CAT(PPUTLIDEC_, PPUTLUINT_TRAIT(u, TYPE))(e, n, u)
#define PPUTLIDEC_HEX(e, n, u) \
  PTL_CAT(PPUTLIDEC_HEX_, PTL_ESC(PTL_IFIRST PPUTLUINT_TRAIT(u, HEX_BITS)))(e, n, u)
#define PPUTLIDEC_HEX_1(e, n, u) PTL_FAIL(e)
#define PPUTLIDEC_HEX_0(e, n, u) PPUTLUINT_TRAIT(u, HEX_IDEC)
#define PPUTLIDEC_DEC(e, n, u)   n

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

// vim: fdm=marker:fmr={{{,}}}

#endif

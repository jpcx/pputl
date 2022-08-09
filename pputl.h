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
//    many high-level programming constructs  and 10-bit binary arithmetic    //
//    for both unsigned and signed two's complement integers.                 //
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
//    Modify  the head of  codegen/codegen.h  to configure the bit size or    //
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
//      - tuple   : anything in parentheses                                   //
//      - uint    : [abstract] unsigned integer                               //
//      - int     : [abstract] signed integer                                 //
//      - ubase2  : [uint] unsigned base 2 integer;  e.g. 0b0000101010u       //
//      - ubase10 : [uint] unsigned base 10 integer; e.g. 42u                 //
//      - bool    : [int]  the literal '1' or '0'                             //
//      - ibase2  : [int]  signed base 2 integer;    e.g. 0b1100100100        //
//      - ibase10 : [int]  signed base 10 integer;   e.g. 353                 //
//      - any     : exactly one generic value                                 //
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
#define PTL_BUILD /* -> <c++ int> */ 20220809

/// [config.bit_length]
/// -------------------
/// the number of bits used by pputl integers.
/// binary representations are fixed at this length.
/// see the readme code generation section to configure.
#define PTL_BIT_LENGTH /* -> uint */ 10

/// [config.uint_max]
/// -----------------
/// the maximum value of a pputl unsigned int.
/// upper bound on the number of args for many pputl functions.
/// see the readme code generation section to configure.
#define PTL_UINT_MAX /* -> uint */ 1023u

/// [config.int_max]
/// ----------------
/// the maximum value of a pputl signed int.
/// see the readme code generation section to configure.
#define PTL_INT_MAX /* -> int */ 511

/// [config.int_min]
/// ----------------
/// the minimum value of a pputl signed int.
/// only representable as binary. see type.int for details.
/// see the readme code generation section to configure.
#define PTL_INT_MIN /* -> binary */ 0b1000000000

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
  PPUTLTUPLE_o(PTL_EAT __VA_ARGS__)(PTL_ISTR([PTL_TUPLE] invalid tuple : __VA_ARGS__), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

/// first parentheses; detects tuple
#define PPUTLTUPLE_o(...) PPUTLTUPLE_oo##__VA_OPT__(_NO)##_PASS

/// second parentheses; returns or fails
#define PPUTLTUPLE_oo_NO_PASS(err, ...) PTL_FAIL(err)
#define PPUTLTUPLE_oo_PASS(err, ...)    __VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.bool]
/// -----------
/// bool type (0 or 1).
/// expands to b if valid, else fails.
///
/// PTL_BOOL(0) // 0
/// PTL_BOOL(1) // 1
#define PTL_BOOL(/* b: bool */...) /* -> b */                                          \
  PPUTLBOOL_o(__VA_ARGS__.)(__VA_ARGS__)(__VA_ARGS__)(PTL_ISTR([PTL_BOOL] invalid bool \
                                                               : __VA_ARGS__),         \
                                                      __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

/// first parentheses; asserts only one arg.
#define PPUTLBOOL_o(_, ...)      PPUTLBOOL_o_##__VA_OPT__(NO_)##PASS()
#define PPUTLBOOL_o_NO_PASS(...) PPUTLBOOL_oo_FAIL
#define PPUTLBOOL_o_PASS(...)    PPUTLBOOL_oo

/// second parentheses; asserts non-tuple.
#define PPUTLBOOL_oo(_, ...)      PPUTLBOOL_oo_RES(PTL_EAT _)
#define PPUTLBOOL_oo_RES(...)     PPUTLBOOL_oo_##__VA_OPT__(NO_)##FAIL()
#define PPUTLBOOL_oo_NO_FAIL(...) PPUTLBOOL_ooo
#define PPUTLBOOL_oo_FAIL(...)    PPUTLBOOL_ooo_NO_PASS

/// third parentheses; asserts either 0 or 1.
#define PPUTLBOOL_ooo(...)         PPUTLBOOL_ooo_RES(PPUTLBOOL_CHK_##__VA_ARGS__)
#define PPUTLBOOL_ooo_RES(...)     PPUTLBOOL_ooo_##__VA_OPT__(NO_)##PASS()
#define PPUTLBOOL_ooo_PASS(...)    PPUTLBOOL_PASS
#define PPUTLBOOL_ooo_NO_PASS(...) PPUTLBOOL_FAIL

/// fourth parentheses; returns
#define PPUTLBOOL_PASS(err, ...) __VA_ARGS__
#define PPUTLBOOL_FAIL(err, ...) PTL_FAIL(err)

/// concat existence checks
#define PPUTLBOOL_CHK_1
#define PPUTLBOOL_CHK_0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.uint]
/// -----------
/// 10-bit unsigned integer type.
/// may be constructed from either unsigned or signed ints.
/// cannot parse negative decimals; use math.neg instead.
///
/// bit length is fixed. cannot parse shorter bit lengths.
///
/// preserves binary/decimal representation.
///
/// uses a 'u' suffix for both representations.
/// see fmt.paste_uint to remove suffix before pasting.
///
/// cast from signed reinterprets bits as unsigned.
///
/// PTL_UINT(0)             // 0u
/// PTL_UINT(1)             // 1u
/// PTL_UINT(2u)            // 2u
/// PTL_UINT(1023)          // 1023u
/// PTL_UINT(1023u)         // 1023u
/// PTL_UINT(0b0000000000u) // 0b0000000000u
/// PTL_UINT(0b1111111111)  // 0b1111111111u
#define PTL_UINT(/* n: uint|int */...) /* -> uint{n} */          \
  PPUTLUINT_o(__VA_ARGS__.)(__VA_ARGS__)(                        \
      __VA_ARGS__)(__VA_ARGS__)(PTL_ISTR([PTL_UINT] invalid uint \
                                         : __VA_ARGS__),         \
                                __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

/// first parentheses; asserts only one arg.
#define PPUTLUINT_o(_, ...)      PPUTLUINT_o_##__VA_OPT__(NO_)##PASS()
#define PPUTLUINT_o_NO_PASS(...) PPUTLUINT_oo_FAIL
#define PPUTLUINT_o_PASS(...)    PPUTLUINT_oo

/// second parentheses; asserts non-tuple.
#define PPUTLUINT_oo(_, ...)      PPUTLUINT_oo_RES(PTL_EAT _)
#define PPUTLUINT_oo_RES(...)     PPUTLUINT_oo_##__VA_OPT__(NO_)##FAIL()
#define PPUTLUINT_oo_NO_FAIL(...) PPUTLUINT_ooo
#define PPUTLUINT_oo_FAIL(...)    PPUTLUINT_ooo_FAIL

/// third parentheses; checks for validity without added 'u' suffix.
#define PPUTLUINT_ooo(...)          PPUTLUINT_ooo_RES(PPUTLUTRAITS_##__VA_ARGS__)
#define PPUTLUINT_ooo_RES(...)      PPUTLUINT_ooo_RES_o(__VA_ARGS__)
#define PPUTLUINT_ooo_RES_o(_, ...) PPUTLUINT_ooo_##__VA_OPT__(NO_)##FAIL()
#define PPUTLUINT_ooo_NO_FAIL(...)  PPUTLUINT_oooo_UPASS
#define PPUTLUINT_ooo_FAIL(...)     PPUTLUINT_oooo

/// fourth parentheses; checks for validity with added 'u' suffix (cast from int).
#define PPUTLUINT_oooo(...)          PPUTLUINT_oooo_RES(PPUTLUTRAITS_##__VA_ARGS__##u)
#define PPUTLUINT_oooo_RES(...)      PPUTLUINT_oooo_RES_o(__VA_ARGS__)
#define PPUTLUINT_oooo_RES_o(_, ...) PPUTLUINT_oooo_##__VA_OPT__(NO_)##FAIL
#define PPUTLUINT_oooo_UPASS(...)    PPUTLUINT_UPASS
#define PPUTLUINT_oooo_NO_FAIL       PPUTLUINT_IPASS
#define PPUTLUINT_oooo_FAIL          PPUTLUINT_FAIL

/// final parentheses; returns
#define PPUTLUINT_UPASS(e, v)  v
#define PPUTLUINT_IPASS(e, v)  v##u
#define PPUTLUINT_FAIL(e, ...) PTL_FAIL(e)

/// internal traits retrieval. uint must be valid and have a suffix.
#define PPUTLUINT_TRAIT(...)           PPUTLUINT_TRAIT_o(__VA_ARGS__)
#define PPUTLUINT_TRAIT_o(uint, trait) PPUTLUINT_TRAIT_oo(trait, PTL_CAT(PPUTLUTRAITS_, uint))
#define PPUTLUINT_TRAIT_oo(trait, ...) PPUTLUINT_TRAIT_##trait(__VA_ARGS__)

/// uint traits. trait name follows the PPUTLUINT_TRAIT_ prefix
#define PPUTLUINT_TRAIT_DEC_FACT(t, ib, l2, sq, f) f
#define PPUTLUINT_TRAIT_DEC_SQRT(t, ib, l2, sq, f) sq
#define PPUTLUINT_TRAIT_DEC_LOG2(t, ib, l2, sq, f) l2
#define PPUTLUINT_TRAIT_DEC_IBIN(t, ib, l2, sq, f) ib
#define PPUTLUINT_TRAIT_BIN_BNOT(t, ud, id, b, bn) bn
#define PPUTLUINT_TRAIT_BIN_BITS(t, ud, id, b, bn) b
#define PPUTLUINT_TRAIT_BIN_IDEC(t, ud, id, b, bn) id
#define PPUTLUINT_TRAIT_BIN_UDEC(t, ud, id, b, bn) ud
#define PPUTLUINT_TRAIT_TYPE(t, ...)               t

// clang-format off

/// type, unsigned decimal, signed decimal, bits, bitnot
#define PPUTLUTRAITS_0b1111111111u BIN,1023u,-1,(1,1,1,1,1,1,1,1,1,1),0b0000000000u
#define PPUTLUTRAITS_0b1111111110u BIN,1022u,-2,(1,1,1,1,1,1,1,1,1,0),0b0000000001u
#define PPUTLUTRAITS_0b1111111101u BIN,1021u,-3,(1,1,1,1,1,1,1,1,0,1),0b0000000010u
#define PPUTLUTRAITS_0b1111111100u BIN,1020u,-4,(1,1,1,1,1,1,1,1,0,0),0b0000000011u
#define PPUTLUTRAITS_0b1111111011u BIN,1019u,-5,(1,1,1,1,1,1,1,0,1,1),0b0000000100u
#define PPUTLUTRAITS_0b1111111010u BIN,1018u,-6,(1,1,1,1,1,1,1,0,1,0),0b0000000101u
#define PPUTLUTRAITS_0b1111111001u BIN,1017u,-7,(1,1,1,1,1,1,1,0,0,1),0b0000000110u
#define PPUTLUTRAITS_0b1111111000u BIN,1016u,-8,(1,1,1,1,1,1,1,0,0,0),0b0000000111u
#define PPUTLUTRAITS_0b1111110111u BIN,1015u,-9,(1,1,1,1,1,1,0,1,1,1),0b0000001000u
#define PPUTLUTRAITS_0b1111110110u BIN,1014u,-10,(1,1,1,1,1,1,0,1,1,0),0b0000001001u
#define PPUTLUTRAITS_0b1111110101u BIN,1013u,-11,(1,1,1,1,1,1,0,1,0,1),0b0000001010u
#define PPUTLUTRAITS_0b1111110100u BIN,1012u,-12,(1,1,1,1,1,1,0,1,0,0),0b0000001011u
#define PPUTLUTRAITS_0b1111110011u BIN,1011u,-13,(1,1,1,1,1,1,0,0,1,1),0b0000001100u
#define PPUTLUTRAITS_0b1111110010u BIN,1010u,-14,(1,1,1,1,1,1,0,0,1,0),0b0000001101u
#define PPUTLUTRAITS_0b1111110001u BIN,1009u,-15,(1,1,1,1,1,1,0,0,0,1),0b0000001110u
#define PPUTLUTRAITS_0b1111110000u BIN,1008u,-16,(1,1,1,1,1,1,0,0,0,0),0b0000001111u
#define PPUTLUTRAITS_0b1111101111u BIN,1007u,-17,(1,1,1,1,1,0,1,1,1,1),0b0000010000u
#define PPUTLUTRAITS_0b1111101110u BIN,1006u,-18,(1,1,1,1,1,0,1,1,1,0),0b0000010001u
#define PPUTLUTRAITS_0b1111101101u BIN,1005u,-19,(1,1,1,1,1,0,1,1,0,1),0b0000010010u
#define PPUTLUTRAITS_0b1111101100u BIN,1004u,-20,(1,1,1,1,1,0,1,1,0,0),0b0000010011u
#define PPUTLUTRAITS_0b1111101011u BIN,1003u,-21,(1,1,1,1,1,0,1,0,1,1),0b0000010100u
#define PPUTLUTRAITS_0b1111101010u BIN,1002u,-22,(1,1,1,1,1,0,1,0,1,0),0b0000010101u
#define PPUTLUTRAITS_0b1111101001u BIN,1001u,-23,(1,1,1,1,1,0,1,0,0,1),0b0000010110u
#define PPUTLUTRAITS_0b1111101000u BIN,1000u,-24,(1,1,1,1,1,0,1,0,0,0),0b0000010111u
#define PPUTLUTRAITS_0b1111100111u BIN,999u,-25,(1,1,1,1,1,0,0,1,1,1),0b0000011000u
#define PPUTLUTRAITS_0b1111100110u BIN,998u,-26,(1,1,1,1,1,0,0,1,1,0),0b0000011001u
#define PPUTLUTRAITS_0b1111100101u BIN,997u,-27,(1,1,1,1,1,0,0,1,0,1),0b0000011010u
#define PPUTLUTRAITS_0b1111100100u BIN,996u,-28,(1,1,1,1,1,0,0,1,0,0),0b0000011011u
#define PPUTLUTRAITS_0b1111100011u BIN,995u,-29,(1,1,1,1,1,0,0,0,1,1),0b0000011100u
#define PPUTLUTRAITS_0b1111100010u BIN,994u,-30,(1,1,1,1,1,0,0,0,1,0),0b0000011101u
#define PPUTLUTRAITS_0b1111100001u BIN,993u,-31,(1,1,1,1,1,0,0,0,0,1),0b0000011110u
#define PPUTLUTRAITS_0b1111100000u BIN,992u,-32,(1,1,1,1,1,0,0,0,0,0),0b0000011111u
#define PPUTLUTRAITS_0b1111011111u BIN,991u,-33,(1,1,1,1,0,1,1,1,1,1),0b0000100000u
#define PPUTLUTRAITS_0b1111011110u BIN,990u,-34,(1,1,1,1,0,1,1,1,1,0),0b0000100001u
#define PPUTLUTRAITS_0b1111011101u BIN,989u,-35,(1,1,1,1,0,1,1,1,0,1),0b0000100010u
#define PPUTLUTRAITS_0b1111011100u BIN,988u,-36,(1,1,1,1,0,1,1,1,0,0),0b0000100011u
#define PPUTLUTRAITS_0b1111011011u BIN,987u,-37,(1,1,1,1,0,1,1,0,1,1),0b0000100100u
#define PPUTLUTRAITS_0b1111011010u BIN,986u,-38,(1,1,1,1,0,1,1,0,1,0),0b0000100101u
#define PPUTLUTRAITS_0b1111011001u BIN,985u,-39,(1,1,1,1,0,1,1,0,0,1),0b0000100110u
#define PPUTLUTRAITS_0b1111011000u BIN,984u,-40,(1,1,1,1,0,1,1,0,0,0),0b0000100111u
#define PPUTLUTRAITS_0b1111010111u BIN,983u,-41,(1,1,1,1,0,1,0,1,1,1),0b0000101000u
#define PPUTLUTRAITS_0b1111010110u BIN,982u,-42,(1,1,1,1,0,1,0,1,1,0),0b0000101001u
#define PPUTLUTRAITS_0b1111010101u BIN,981u,-43,(1,1,1,1,0,1,0,1,0,1),0b0000101010u
#define PPUTLUTRAITS_0b1111010100u BIN,980u,-44,(1,1,1,1,0,1,0,1,0,0),0b0000101011u
#define PPUTLUTRAITS_0b1111010011u BIN,979u,-45,(1,1,1,1,0,1,0,0,1,1),0b0000101100u
#define PPUTLUTRAITS_0b1111010010u BIN,978u,-46,(1,1,1,1,0,1,0,0,1,0),0b0000101101u
#define PPUTLUTRAITS_0b1111010001u BIN,977u,-47,(1,1,1,1,0,1,0,0,0,1),0b0000101110u
#define PPUTLUTRAITS_0b1111010000u BIN,976u,-48,(1,1,1,1,0,1,0,0,0,0),0b0000101111u
#define PPUTLUTRAITS_0b1111001111u BIN,975u,-49,(1,1,1,1,0,0,1,1,1,1),0b0000110000u
#define PPUTLUTRAITS_0b1111001110u BIN,974u,-50,(1,1,1,1,0,0,1,1,1,0),0b0000110001u
#define PPUTLUTRAITS_0b1111001101u BIN,973u,-51,(1,1,1,1,0,0,1,1,0,1),0b0000110010u
#define PPUTLUTRAITS_0b1111001100u BIN,972u,-52,(1,1,1,1,0,0,1,1,0,0),0b0000110011u
#define PPUTLUTRAITS_0b1111001011u BIN,971u,-53,(1,1,1,1,0,0,1,0,1,1),0b0000110100u
#define PPUTLUTRAITS_0b1111001010u BIN,970u,-54,(1,1,1,1,0,0,1,0,1,0),0b0000110101u
#define PPUTLUTRAITS_0b1111001001u BIN,969u,-55,(1,1,1,1,0,0,1,0,0,1),0b0000110110u
#define PPUTLUTRAITS_0b1111001000u BIN,968u,-56,(1,1,1,1,0,0,1,0,0,0),0b0000110111u
#define PPUTLUTRAITS_0b1111000111u BIN,967u,-57,(1,1,1,1,0,0,0,1,1,1),0b0000111000u
#define PPUTLUTRAITS_0b1111000110u BIN,966u,-58,(1,1,1,1,0,0,0,1,1,0),0b0000111001u
#define PPUTLUTRAITS_0b1111000101u BIN,965u,-59,(1,1,1,1,0,0,0,1,0,1),0b0000111010u
#define PPUTLUTRAITS_0b1111000100u BIN,964u,-60,(1,1,1,1,0,0,0,1,0,0),0b0000111011u
#define PPUTLUTRAITS_0b1111000011u BIN,963u,-61,(1,1,1,1,0,0,0,0,1,1),0b0000111100u
#define PPUTLUTRAITS_0b1111000010u BIN,962u,-62,(1,1,1,1,0,0,0,0,1,0),0b0000111101u
#define PPUTLUTRAITS_0b1111000001u BIN,961u,-63,(1,1,1,1,0,0,0,0,0,1),0b0000111110u
#define PPUTLUTRAITS_0b1111000000u BIN,960u,-64,(1,1,1,1,0,0,0,0,0,0),0b0000111111u
#define PPUTLUTRAITS_0b1110111111u BIN,959u,-65,(1,1,1,0,1,1,1,1,1,1),0b0001000000u
#define PPUTLUTRAITS_0b1110111110u BIN,958u,-66,(1,1,1,0,1,1,1,1,1,0),0b0001000001u
#define PPUTLUTRAITS_0b1110111101u BIN,957u,-67,(1,1,1,0,1,1,1,1,0,1),0b0001000010u
#define PPUTLUTRAITS_0b1110111100u BIN,956u,-68,(1,1,1,0,1,1,1,1,0,0),0b0001000011u
#define PPUTLUTRAITS_0b1110111011u BIN,955u,-69,(1,1,1,0,1,1,1,0,1,1),0b0001000100u
#define PPUTLUTRAITS_0b1110111010u BIN,954u,-70,(1,1,1,0,1,1,1,0,1,0),0b0001000101u
#define PPUTLUTRAITS_0b1110111001u BIN,953u,-71,(1,1,1,0,1,1,1,0,0,1),0b0001000110u
#define PPUTLUTRAITS_0b1110111000u BIN,952u,-72,(1,1,1,0,1,1,1,0,0,0),0b0001000111u
#define PPUTLUTRAITS_0b1110110111u BIN,951u,-73,(1,1,1,0,1,1,0,1,1,1),0b0001001000u
#define PPUTLUTRAITS_0b1110110110u BIN,950u,-74,(1,1,1,0,1,1,0,1,1,0),0b0001001001u
#define PPUTLUTRAITS_0b1110110101u BIN,949u,-75,(1,1,1,0,1,1,0,1,0,1),0b0001001010u
#define PPUTLUTRAITS_0b1110110100u BIN,948u,-76,(1,1,1,0,1,1,0,1,0,0),0b0001001011u
#define PPUTLUTRAITS_0b1110110011u BIN,947u,-77,(1,1,1,0,1,1,0,0,1,1),0b0001001100u
#define PPUTLUTRAITS_0b1110110010u BIN,946u,-78,(1,1,1,0,1,1,0,0,1,0),0b0001001101u
#define PPUTLUTRAITS_0b1110110001u BIN,945u,-79,(1,1,1,0,1,1,0,0,0,1),0b0001001110u
#define PPUTLUTRAITS_0b1110110000u BIN,944u,-80,(1,1,1,0,1,1,0,0,0,0),0b0001001111u
#define PPUTLUTRAITS_0b1110101111u BIN,943u,-81,(1,1,1,0,1,0,1,1,1,1),0b0001010000u
#define PPUTLUTRAITS_0b1110101110u BIN,942u,-82,(1,1,1,0,1,0,1,1,1,0),0b0001010001u
#define PPUTLUTRAITS_0b1110101101u BIN,941u,-83,(1,1,1,0,1,0,1,1,0,1),0b0001010010u
#define PPUTLUTRAITS_0b1110101100u BIN,940u,-84,(1,1,1,0,1,0,1,1,0,0),0b0001010011u
#define PPUTLUTRAITS_0b1110101011u BIN,939u,-85,(1,1,1,0,1,0,1,0,1,1),0b0001010100u
#define PPUTLUTRAITS_0b1110101010u BIN,938u,-86,(1,1,1,0,1,0,1,0,1,0),0b0001010101u
#define PPUTLUTRAITS_0b1110101001u BIN,937u,-87,(1,1,1,0,1,0,1,0,0,1),0b0001010110u
#define PPUTLUTRAITS_0b1110101000u BIN,936u,-88,(1,1,1,0,1,0,1,0,0,0),0b0001010111u
#define PPUTLUTRAITS_0b1110100111u BIN,935u,-89,(1,1,1,0,1,0,0,1,1,1),0b0001011000u
#define PPUTLUTRAITS_0b1110100110u BIN,934u,-90,(1,1,1,0,1,0,0,1,1,0),0b0001011001u
#define PPUTLUTRAITS_0b1110100101u BIN,933u,-91,(1,1,1,0,1,0,0,1,0,1),0b0001011010u
#define PPUTLUTRAITS_0b1110100100u BIN,932u,-92,(1,1,1,0,1,0,0,1,0,0),0b0001011011u
#define PPUTLUTRAITS_0b1110100011u BIN,931u,-93,(1,1,1,0,1,0,0,0,1,1),0b0001011100u
#define PPUTLUTRAITS_0b1110100010u BIN,930u,-94,(1,1,1,0,1,0,0,0,1,0),0b0001011101u
#define PPUTLUTRAITS_0b1110100001u BIN,929u,-95,(1,1,1,0,1,0,0,0,0,1),0b0001011110u
#define PPUTLUTRAITS_0b1110100000u BIN,928u,-96,(1,1,1,0,1,0,0,0,0,0),0b0001011111u
#define PPUTLUTRAITS_0b1110011111u BIN,927u,-97,(1,1,1,0,0,1,1,1,1,1),0b0001100000u
#define PPUTLUTRAITS_0b1110011110u BIN,926u,-98,(1,1,1,0,0,1,1,1,1,0),0b0001100001u
#define PPUTLUTRAITS_0b1110011101u BIN,925u,-99,(1,1,1,0,0,1,1,1,0,1),0b0001100010u
#define PPUTLUTRAITS_0b1110011100u BIN,924u,-100,(1,1,1,0,0,1,1,1,0,0),0b0001100011u
#define PPUTLUTRAITS_0b1110011011u BIN,923u,-101,(1,1,1,0,0,1,1,0,1,1),0b0001100100u
#define PPUTLUTRAITS_0b1110011010u BIN,922u,-102,(1,1,1,0,0,1,1,0,1,0),0b0001100101u
#define PPUTLUTRAITS_0b1110011001u BIN,921u,-103,(1,1,1,0,0,1,1,0,0,1),0b0001100110u
#define PPUTLUTRAITS_0b1110011000u BIN,920u,-104,(1,1,1,0,0,1,1,0,0,0),0b0001100111u
#define PPUTLUTRAITS_0b1110010111u BIN,919u,-105,(1,1,1,0,0,1,0,1,1,1),0b0001101000u
#define PPUTLUTRAITS_0b1110010110u BIN,918u,-106,(1,1,1,0,0,1,0,1,1,0),0b0001101001u
#define PPUTLUTRAITS_0b1110010101u BIN,917u,-107,(1,1,1,0,0,1,0,1,0,1),0b0001101010u
#define PPUTLUTRAITS_0b1110010100u BIN,916u,-108,(1,1,1,0,0,1,0,1,0,0),0b0001101011u
#define PPUTLUTRAITS_0b1110010011u BIN,915u,-109,(1,1,1,0,0,1,0,0,1,1),0b0001101100u
#define PPUTLUTRAITS_0b1110010010u BIN,914u,-110,(1,1,1,0,0,1,0,0,1,0),0b0001101101u
#define PPUTLUTRAITS_0b1110010001u BIN,913u,-111,(1,1,1,0,0,1,0,0,0,1),0b0001101110u
#define PPUTLUTRAITS_0b1110010000u BIN,912u,-112,(1,1,1,0,0,1,0,0,0,0),0b0001101111u
#define PPUTLUTRAITS_0b1110001111u BIN,911u,-113,(1,1,1,0,0,0,1,1,1,1),0b0001110000u
#define PPUTLUTRAITS_0b1110001110u BIN,910u,-114,(1,1,1,0,0,0,1,1,1,0),0b0001110001u
#define PPUTLUTRAITS_0b1110001101u BIN,909u,-115,(1,1,1,0,0,0,1,1,0,1),0b0001110010u
#define PPUTLUTRAITS_0b1110001100u BIN,908u,-116,(1,1,1,0,0,0,1,1,0,0),0b0001110011u
#define PPUTLUTRAITS_0b1110001011u BIN,907u,-117,(1,1,1,0,0,0,1,0,1,1),0b0001110100u
#define PPUTLUTRAITS_0b1110001010u BIN,906u,-118,(1,1,1,0,0,0,1,0,1,0),0b0001110101u
#define PPUTLUTRAITS_0b1110001001u BIN,905u,-119,(1,1,1,0,0,0,1,0,0,1),0b0001110110u
#define PPUTLUTRAITS_0b1110001000u BIN,904u,-120,(1,1,1,0,0,0,1,0,0,0),0b0001110111u
#define PPUTLUTRAITS_0b1110000111u BIN,903u,-121,(1,1,1,0,0,0,0,1,1,1),0b0001111000u
#define PPUTLUTRAITS_0b1110000110u BIN,902u,-122,(1,1,1,0,0,0,0,1,1,0),0b0001111001u
#define PPUTLUTRAITS_0b1110000101u BIN,901u,-123,(1,1,1,0,0,0,0,1,0,1),0b0001111010u
#define PPUTLUTRAITS_0b1110000100u BIN,900u,-124,(1,1,1,0,0,0,0,1,0,0),0b0001111011u
#define PPUTLUTRAITS_0b1110000011u BIN,899u,-125,(1,1,1,0,0,0,0,0,1,1),0b0001111100u
#define PPUTLUTRAITS_0b1110000010u BIN,898u,-126,(1,1,1,0,0,0,0,0,1,0),0b0001111101u
#define PPUTLUTRAITS_0b1110000001u BIN,897u,-127,(1,1,1,0,0,0,0,0,0,1),0b0001111110u
#define PPUTLUTRAITS_0b1110000000u BIN,896u,-128,(1,1,1,0,0,0,0,0,0,0),0b0001111111u
#define PPUTLUTRAITS_0b1101111111u BIN,895u,-129,(1,1,0,1,1,1,1,1,1,1),0b0010000000u
#define PPUTLUTRAITS_0b1101111110u BIN,894u,-130,(1,1,0,1,1,1,1,1,1,0),0b0010000001u
#define PPUTLUTRAITS_0b1101111101u BIN,893u,-131,(1,1,0,1,1,1,1,1,0,1),0b0010000010u
#define PPUTLUTRAITS_0b1101111100u BIN,892u,-132,(1,1,0,1,1,1,1,1,0,0),0b0010000011u
#define PPUTLUTRAITS_0b1101111011u BIN,891u,-133,(1,1,0,1,1,1,1,0,1,1),0b0010000100u
#define PPUTLUTRAITS_0b1101111010u BIN,890u,-134,(1,1,0,1,1,1,1,0,1,0),0b0010000101u
#define PPUTLUTRAITS_0b1101111001u BIN,889u,-135,(1,1,0,1,1,1,1,0,0,1),0b0010000110u
#define PPUTLUTRAITS_0b1101111000u BIN,888u,-136,(1,1,0,1,1,1,1,0,0,0),0b0010000111u
#define PPUTLUTRAITS_0b1101110111u BIN,887u,-137,(1,1,0,1,1,1,0,1,1,1),0b0010001000u
#define PPUTLUTRAITS_0b1101110110u BIN,886u,-138,(1,1,0,1,1,1,0,1,1,0),0b0010001001u
#define PPUTLUTRAITS_0b1101110101u BIN,885u,-139,(1,1,0,1,1,1,0,1,0,1),0b0010001010u
#define PPUTLUTRAITS_0b1101110100u BIN,884u,-140,(1,1,0,1,1,1,0,1,0,0),0b0010001011u
#define PPUTLUTRAITS_0b1101110011u BIN,883u,-141,(1,1,0,1,1,1,0,0,1,1),0b0010001100u
#define PPUTLUTRAITS_0b1101110010u BIN,882u,-142,(1,1,0,1,1,1,0,0,1,0),0b0010001101u
#define PPUTLUTRAITS_0b1101110001u BIN,881u,-143,(1,1,0,1,1,1,0,0,0,1),0b0010001110u
#define PPUTLUTRAITS_0b1101110000u BIN,880u,-144,(1,1,0,1,1,1,0,0,0,0),0b0010001111u
#define PPUTLUTRAITS_0b1101101111u BIN,879u,-145,(1,1,0,1,1,0,1,1,1,1),0b0010010000u
#define PPUTLUTRAITS_0b1101101110u BIN,878u,-146,(1,1,0,1,1,0,1,1,1,0),0b0010010001u
#define PPUTLUTRAITS_0b1101101101u BIN,877u,-147,(1,1,0,1,1,0,1,1,0,1),0b0010010010u
#define PPUTLUTRAITS_0b1101101100u BIN,876u,-148,(1,1,0,1,1,0,1,1,0,0),0b0010010011u
#define PPUTLUTRAITS_0b1101101011u BIN,875u,-149,(1,1,0,1,1,0,1,0,1,1),0b0010010100u
#define PPUTLUTRAITS_0b1101101010u BIN,874u,-150,(1,1,0,1,1,0,1,0,1,0),0b0010010101u
#define PPUTLUTRAITS_0b1101101001u BIN,873u,-151,(1,1,0,1,1,0,1,0,0,1),0b0010010110u
#define PPUTLUTRAITS_0b1101101000u BIN,872u,-152,(1,1,0,1,1,0,1,0,0,0),0b0010010111u
#define PPUTLUTRAITS_0b1101100111u BIN,871u,-153,(1,1,0,1,1,0,0,1,1,1),0b0010011000u
#define PPUTLUTRAITS_0b1101100110u BIN,870u,-154,(1,1,0,1,1,0,0,1,1,0),0b0010011001u
#define PPUTLUTRAITS_0b1101100101u BIN,869u,-155,(1,1,0,1,1,0,0,1,0,1),0b0010011010u
#define PPUTLUTRAITS_0b1101100100u BIN,868u,-156,(1,1,0,1,1,0,0,1,0,0),0b0010011011u
#define PPUTLUTRAITS_0b1101100011u BIN,867u,-157,(1,1,0,1,1,0,0,0,1,1),0b0010011100u
#define PPUTLUTRAITS_0b1101100010u BIN,866u,-158,(1,1,0,1,1,0,0,0,1,0),0b0010011101u
#define PPUTLUTRAITS_0b1101100001u BIN,865u,-159,(1,1,0,1,1,0,0,0,0,1),0b0010011110u
#define PPUTLUTRAITS_0b1101100000u BIN,864u,-160,(1,1,0,1,1,0,0,0,0,0),0b0010011111u
#define PPUTLUTRAITS_0b1101011111u BIN,863u,-161,(1,1,0,1,0,1,1,1,1,1),0b0010100000u
#define PPUTLUTRAITS_0b1101011110u BIN,862u,-162,(1,1,0,1,0,1,1,1,1,0),0b0010100001u
#define PPUTLUTRAITS_0b1101011101u BIN,861u,-163,(1,1,0,1,0,1,1,1,0,1),0b0010100010u
#define PPUTLUTRAITS_0b1101011100u BIN,860u,-164,(1,1,0,1,0,1,1,1,0,0),0b0010100011u
#define PPUTLUTRAITS_0b1101011011u BIN,859u,-165,(1,1,0,1,0,1,1,0,1,1),0b0010100100u
#define PPUTLUTRAITS_0b1101011010u BIN,858u,-166,(1,1,0,1,0,1,1,0,1,0),0b0010100101u
#define PPUTLUTRAITS_0b1101011001u BIN,857u,-167,(1,1,0,1,0,1,1,0,0,1),0b0010100110u
#define PPUTLUTRAITS_0b1101011000u BIN,856u,-168,(1,1,0,1,0,1,1,0,0,0),0b0010100111u
#define PPUTLUTRAITS_0b1101010111u BIN,855u,-169,(1,1,0,1,0,1,0,1,1,1),0b0010101000u
#define PPUTLUTRAITS_0b1101010110u BIN,854u,-170,(1,1,0,1,0,1,0,1,1,0),0b0010101001u
#define PPUTLUTRAITS_0b1101010101u BIN,853u,-171,(1,1,0,1,0,1,0,1,0,1),0b0010101010u
#define PPUTLUTRAITS_0b1101010100u BIN,852u,-172,(1,1,0,1,0,1,0,1,0,0),0b0010101011u
#define PPUTLUTRAITS_0b1101010011u BIN,851u,-173,(1,1,0,1,0,1,0,0,1,1),0b0010101100u
#define PPUTLUTRAITS_0b1101010010u BIN,850u,-174,(1,1,0,1,0,1,0,0,1,0),0b0010101101u
#define PPUTLUTRAITS_0b1101010001u BIN,849u,-175,(1,1,0,1,0,1,0,0,0,1),0b0010101110u
#define PPUTLUTRAITS_0b1101010000u BIN,848u,-176,(1,1,0,1,0,1,0,0,0,0),0b0010101111u
#define PPUTLUTRAITS_0b1101001111u BIN,847u,-177,(1,1,0,1,0,0,1,1,1,1),0b0010110000u
#define PPUTLUTRAITS_0b1101001110u BIN,846u,-178,(1,1,0,1,0,0,1,1,1,0),0b0010110001u
#define PPUTLUTRAITS_0b1101001101u BIN,845u,-179,(1,1,0,1,0,0,1,1,0,1),0b0010110010u
#define PPUTLUTRAITS_0b1101001100u BIN,844u,-180,(1,1,0,1,0,0,1,1,0,0),0b0010110011u
#define PPUTLUTRAITS_0b1101001011u BIN,843u,-181,(1,1,0,1,0,0,1,0,1,1),0b0010110100u
#define PPUTLUTRAITS_0b1101001010u BIN,842u,-182,(1,1,0,1,0,0,1,0,1,0),0b0010110101u
#define PPUTLUTRAITS_0b1101001001u BIN,841u,-183,(1,1,0,1,0,0,1,0,0,1),0b0010110110u
#define PPUTLUTRAITS_0b1101001000u BIN,840u,-184,(1,1,0,1,0,0,1,0,0,0),0b0010110111u
#define PPUTLUTRAITS_0b1101000111u BIN,839u,-185,(1,1,0,1,0,0,0,1,1,1),0b0010111000u
#define PPUTLUTRAITS_0b1101000110u BIN,838u,-186,(1,1,0,1,0,0,0,1,1,0),0b0010111001u
#define PPUTLUTRAITS_0b1101000101u BIN,837u,-187,(1,1,0,1,0,0,0,1,0,1),0b0010111010u
#define PPUTLUTRAITS_0b1101000100u BIN,836u,-188,(1,1,0,1,0,0,0,1,0,0),0b0010111011u
#define PPUTLUTRAITS_0b1101000011u BIN,835u,-189,(1,1,0,1,0,0,0,0,1,1),0b0010111100u
#define PPUTLUTRAITS_0b1101000010u BIN,834u,-190,(1,1,0,1,0,0,0,0,1,0),0b0010111101u
#define PPUTLUTRAITS_0b1101000001u BIN,833u,-191,(1,1,0,1,0,0,0,0,0,1),0b0010111110u
#define PPUTLUTRAITS_0b1101000000u BIN,832u,-192,(1,1,0,1,0,0,0,0,0,0),0b0010111111u
#define PPUTLUTRAITS_0b1100111111u BIN,831u,-193,(1,1,0,0,1,1,1,1,1,1),0b0011000000u
#define PPUTLUTRAITS_0b1100111110u BIN,830u,-194,(1,1,0,0,1,1,1,1,1,0),0b0011000001u
#define PPUTLUTRAITS_0b1100111101u BIN,829u,-195,(1,1,0,0,1,1,1,1,0,1),0b0011000010u
#define PPUTLUTRAITS_0b1100111100u BIN,828u,-196,(1,1,0,0,1,1,1,1,0,0),0b0011000011u
#define PPUTLUTRAITS_0b1100111011u BIN,827u,-197,(1,1,0,0,1,1,1,0,1,1),0b0011000100u
#define PPUTLUTRAITS_0b1100111010u BIN,826u,-198,(1,1,0,0,1,1,1,0,1,0),0b0011000101u
#define PPUTLUTRAITS_0b1100111001u BIN,825u,-199,(1,1,0,0,1,1,1,0,0,1),0b0011000110u
#define PPUTLUTRAITS_0b1100111000u BIN,824u,-200,(1,1,0,0,1,1,1,0,0,0),0b0011000111u
#define PPUTLUTRAITS_0b1100110111u BIN,823u,-201,(1,1,0,0,1,1,0,1,1,1),0b0011001000u
#define PPUTLUTRAITS_0b1100110110u BIN,822u,-202,(1,1,0,0,1,1,0,1,1,0),0b0011001001u
#define PPUTLUTRAITS_0b1100110101u BIN,821u,-203,(1,1,0,0,1,1,0,1,0,1),0b0011001010u
#define PPUTLUTRAITS_0b1100110100u BIN,820u,-204,(1,1,0,0,1,1,0,1,0,0),0b0011001011u
#define PPUTLUTRAITS_0b1100110011u BIN,819u,-205,(1,1,0,0,1,1,0,0,1,1),0b0011001100u
#define PPUTLUTRAITS_0b1100110010u BIN,818u,-206,(1,1,0,0,1,1,0,0,1,0),0b0011001101u
#define PPUTLUTRAITS_0b1100110001u BIN,817u,-207,(1,1,0,0,1,1,0,0,0,1),0b0011001110u
#define PPUTLUTRAITS_0b1100110000u BIN,816u,-208,(1,1,0,0,1,1,0,0,0,0),0b0011001111u
#define PPUTLUTRAITS_0b1100101111u BIN,815u,-209,(1,1,0,0,1,0,1,1,1,1),0b0011010000u
#define PPUTLUTRAITS_0b1100101110u BIN,814u,-210,(1,1,0,0,1,0,1,1,1,0),0b0011010001u
#define PPUTLUTRAITS_0b1100101101u BIN,813u,-211,(1,1,0,0,1,0,1,1,0,1),0b0011010010u
#define PPUTLUTRAITS_0b1100101100u BIN,812u,-212,(1,1,0,0,1,0,1,1,0,0),0b0011010011u
#define PPUTLUTRAITS_0b1100101011u BIN,811u,-213,(1,1,0,0,1,0,1,0,1,1),0b0011010100u
#define PPUTLUTRAITS_0b1100101010u BIN,810u,-214,(1,1,0,0,1,0,1,0,1,0),0b0011010101u
#define PPUTLUTRAITS_0b1100101001u BIN,809u,-215,(1,1,0,0,1,0,1,0,0,1),0b0011010110u
#define PPUTLUTRAITS_0b1100101000u BIN,808u,-216,(1,1,0,0,1,0,1,0,0,0),0b0011010111u
#define PPUTLUTRAITS_0b1100100111u BIN,807u,-217,(1,1,0,0,1,0,0,1,1,1),0b0011011000u
#define PPUTLUTRAITS_0b1100100110u BIN,806u,-218,(1,1,0,0,1,0,0,1,1,0),0b0011011001u
#define PPUTLUTRAITS_0b1100100101u BIN,805u,-219,(1,1,0,0,1,0,0,1,0,1),0b0011011010u
#define PPUTLUTRAITS_0b1100100100u BIN,804u,-220,(1,1,0,0,1,0,0,1,0,0),0b0011011011u
#define PPUTLUTRAITS_0b1100100011u BIN,803u,-221,(1,1,0,0,1,0,0,0,1,1),0b0011011100u
#define PPUTLUTRAITS_0b1100100010u BIN,802u,-222,(1,1,0,0,1,0,0,0,1,0),0b0011011101u
#define PPUTLUTRAITS_0b1100100001u BIN,801u,-223,(1,1,0,0,1,0,0,0,0,1),0b0011011110u
#define PPUTLUTRAITS_0b1100100000u BIN,800u,-224,(1,1,0,0,1,0,0,0,0,0),0b0011011111u
#define PPUTLUTRAITS_0b1100011111u BIN,799u,-225,(1,1,0,0,0,1,1,1,1,1),0b0011100000u
#define PPUTLUTRAITS_0b1100011110u BIN,798u,-226,(1,1,0,0,0,1,1,1,1,0),0b0011100001u
#define PPUTLUTRAITS_0b1100011101u BIN,797u,-227,(1,1,0,0,0,1,1,1,0,1),0b0011100010u
#define PPUTLUTRAITS_0b1100011100u BIN,796u,-228,(1,1,0,0,0,1,1,1,0,0),0b0011100011u
#define PPUTLUTRAITS_0b1100011011u BIN,795u,-229,(1,1,0,0,0,1,1,0,1,1),0b0011100100u
#define PPUTLUTRAITS_0b1100011010u BIN,794u,-230,(1,1,0,0,0,1,1,0,1,0),0b0011100101u
#define PPUTLUTRAITS_0b1100011001u BIN,793u,-231,(1,1,0,0,0,1,1,0,0,1),0b0011100110u
#define PPUTLUTRAITS_0b1100011000u BIN,792u,-232,(1,1,0,0,0,1,1,0,0,0),0b0011100111u
#define PPUTLUTRAITS_0b1100010111u BIN,791u,-233,(1,1,0,0,0,1,0,1,1,1),0b0011101000u
#define PPUTLUTRAITS_0b1100010110u BIN,790u,-234,(1,1,0,0,0,1,0,1,1,0),0b0011101001u
#define PPUTLUTRAITS_0b1100010101u BIN,789u,-235,(1,1,0,0,0,1,0,1,0,1),0b0011101010u
#define PPUTLUTRAITS_0b1100010100u BIN,788u,-236,(1,1,0,0,0,1,0,1,0,0),0b0011101011u
#define PPUTLUTRAITS_0b1100010011u BIN,787u,-237,(1,1,0,0,0,1,0,0,1,1),0b0011101100u
#define PPUTLUTRAITS_0b1100010010u BIN,786u,-238,(1,1,0,0,0,1,0,0,1,0),0b0011101101u
#define PPUTLUTRAITS_0b1100010001u BIN,785u,-239,(1,1,0,0,0,1,0,0,0,1),0b0011101110u
#define PPUTLUTRAITS_0b1100010000u BIN,784u,-240,(1,1,0,0,0,1,0,0,0,0),0b0011101111u
#define PPUTLUTRAITS_0b1100001111u BIN,783u,-241,(1,1,0,0,0,0,1,1,1,1),0b0011110000u
#define PPUTLUTRAITS_0b1100001110u BIN,782u,-242,(1,1,0,0,0,0,1,1,1,0),0b0011110001u
#define PPUTLUTRAITS_0b1100001101u BIN,781u,-243,(1,1,0,0,0,0,1,1,0,1),0b0011110010u
#define PPUTLUTRAITS_0b1100001100u BIN,780u,-244,(1,1,0,0,0,0,1,1,0,0),0b0011110011u
#define PPUTLUTRAITS_0b1100001011u BIN,779u,-245,(1,1,0,0,0,0,1,0,1,1),0b0011110100u
#define PPUTLUTRAITS_0b1100001010u BIN,778u,-246,(1,1,0,0,0,0,1,0,1,0),0b0011110101u
#define PPUTLUTRAITS_0b1100001001u BIN,777u,-247,(1,1,0,0,0,0,1,0,0,1),0b0011110110u
#define PPUTLUTRAITS_0b1100001000u BIN,776u,-248,(1,1,0,0,0,0,1,0,0,0),0b0011110111u
#define PPUTLUTRAITS_0b1100000111u BIN,775u,-249,(1,1,0,0,0,0,0,1,1,1),0b0011111000u
#define PPUTLUTRAITS_0b1100000110u BIN,774u,-250,(1,1,0,0,0,0,0,1,1,0),0b0011111001u
#define PPUTLUTRAITS_0b1100000101u BIN,773u,-251,(1,1,0,0,0,0,0,1,0,1),0b0011111010u
#define PPUTLUTRAITS_0b1100000100u BIN,772u,-252,(1,1,0,0,0,0,0,1,0,0),0b0011111011u
#define PPUTLUTRAITS_0b1100000011u BIN,771u,-253,(1,1,0,0,0,0,0,0,1,1),0b0011111100u
#define PPUTLUTRAITS_0b1100000010u BIN,770u,-254,(1,1,0,0,0,0,0,0,1,0),0b0011111101u
#define PPUTLUTRAITS_0b1100000001u BIN,769u,-255,(1,1,0,0,0,0,0,0,0,1),0b0011111110u
#define PPUTLUTRAITS_0b1100000000u BIN,768u,-256,(1,1,0,0,0,0,0,0,0,0),0b0011111111u
#define PPUTLUTRAITS_0b1011111111u BIN,767u,-257,(1,0,1,1,1,1,1,1,1,1),0b0100000000u
#define PPUTLUTRAITS_0b1011111110u BIN,766u,-258,(1,0,1,1,1,1,1,1,1,0),0b0100000001u
#define PPUTLUTRAITS_0b1011111101u BIN,765u,-259,(1,0,1,1,1,1,1,1,0,1),0b0100000010u
#define PPUTLUTRAITS_0b1011111100u BIN,764u,-260,(1,0,1,1,1,1,1,1,0,0),0b0100000011u
#define PPUTLUTRAITS_0b1011111011u BIN,763u,-261,(1,0,1,1,1,1,1,0,1,1),0b0100000100u
#define PPUTLUTRAITS_0b1011111010u BIN,762u,-262,(1,0,1,1,1,1,1,0,1,0),0b0100000101u
#define PPUTLUTRAITS_0b1011111001u BIN,761u,-263,(1,0,1,1,1,1,1,0,0,1),0b0100000110u
#define PPUTLUTRAITS_0b1011111000u BIN,760u,-264,(1,0,1,1,1,1,1,0,0,0),0b0100000111u
#define PPUTLUTRAITS_0b1011110111u BIN,759u,-265,(1,0,1,1,1,1,0,1,1,1),0b0100001000u
#define PPUTLUTRAITS_0b1011110110u BIN,758u,-266,(1,0,1,1,1,1,0,1,1,0),0b0100001001u
#define PPUTLUTRAITS_0b1011110101u BIN,757u,-267,(1,0,1,1,1,1,0,1,0,1),0b0100001010u
#define PPUTLUTRAITS_0b1011110100u BIN,756u,-268,(1,0,1,1,1,1,0,1,0,0),0b0100001011u
#define PPUTLUTRAITS_0b1011110011u BIN,755u,-269,(1,0,1,1,1,1,0,0,1,1),0b0100001100u
#define PPUTLUTRAITS_0b1011110010u BIN,754u,-270,(1,0,1,1,1,1,0,0,1,0),0b0100001101u
#define PPUTLUTRAITS_0b1011110001u BIN,753u,-271,(1,0,1,1,1,1,0,0,0,1),0b0100001110u
#define PPUTLUTRAITS_0b1011110000u BIN,752u,-272,(1,0,1,1,1,1,0,0,0,0),0b0100001111u
#define PPUTLUTRAITS_0b1011101111u BIN,751u,-273,(1,0,1,1,1,0,1,1,1,1),0b0100010000u
#define PPUTLUTRAITS_0b1011101110u BIN,750u,-274,(1,0,1,1,1,0,1,1,1,0),0b0100010001u
#define PPUTLUTRAITS_0b1011101101u BIN,749u,-275,(1,0,1,1,1,0,1,1,0,1),0b0100010010u
#define PPUTLUTRAITS_0b1011101100u BIN,748u,-276,(1,0,1,1,1,0,1,1,0,0),0b0100010011u
#define PPUTLUTRAITS_0b1011101011u BIN,747u,-277,(1,0,1,1,1,0,1,0,1,1),0b0100010100u
#define PPUTLUTRAITS_0b1011101010u BIN,746u,-278,(1,0,1,1,1,0,1,0,1,0),0b0100010101u
#define PPUTLUTRAITS_0b1011101001u BIN,745u,-279,(1,0,1,1,1,0,1,0,0,1),0b0100010110u
#define PPUTLUTRAITS_0b1011101000u BIN,744u,-280,(1,0,1,1,1,0,1,0,0,0),0b0100010111u
#define PPUTLUTRAITS_0b1011100111u BIN,743u,-281,(1,0,1,1,1,0,0,1,1,1),0b0100011000u
#define PPUTLUTRAITS_0b1011100110u BIN,742u,-282,(1,0,1,1,1,0,0,1,1,0),0b0100011001u
#define PPUTLUTRAITS_0b1011100101u BIN,741u,-283,(1,0,1,1,1,0,0,1,0,1),0b0100011010u
#define PPUTLUTRAITS_0b1011100100u BIN,740u,-284,(1,0,1,1,1,0,0,1,0,0),0b0100011011u
#define PPUTLUTRAITS_0b1011100011u BIN,739u,-285,(1,0,1,1,1,0,0,0,1,1),0b0100011100u
#define PPUTLUTRAITS_0b1011100010u BIN,738u,-286,(1,0,1,1,1,0,0,0,1,0),0b0100011101u
#define PPUTLUTRAITS_0b1011100001u BIN,737u,-287,(1,0,1,1,1,0,0,0,0,1),0b0100011110u
#define PPUTLUTRAITS_0b1011100000u BIN,736u,-288,(1,0,1,1,1,0,0,0,0,0),0b0100011111u
#define PPUTLUTRAITS_0b1011011111u BIN,735u,-289,(1,0,1,1,0,1,1,1,1,1),0b0100100000u
#define PPUTLUTRAITS_0b1011011110u BIN,734u,-290,(1,0,1,1,0,1,1,1,1,0),0b0100100001u
#define PPUTLUTRAITS_0b1011011101u BIN,733u,-291,(1,0,1,1,0,1,1,1,0,1),0b0100100010u
#define PPUTLUTRAITS_0b1011011100u BIN,732u,-292,(1,0,1,1,0,1,1,1,0,0),0b0100100011u
#define PPUTLUTRAITS_0b1011011011u BIN,731u,-293,(1,0,1,1,0,1,1,0,1,1),0b0100100100u
#define PPUTLUTRAITS_0b1011011010u BIN,730u,-294,(1,0,1,1,0,1,1,0,1,0),0b0100100101u
#define PPUTLUTRAITS_0b1011011001u BIN,729u,-295,(1,0,1,1,0,1,1,0,0,1),0b0100100110u
#define PPUTLUTRAITS_0b1011011000u BIN,728u,-296,(1,0,1,1,0,1,1,0,0,0),0b0100100111u
#define PPUTLUTRAITS_0b1011010111u BIN,727u,-297,(1,0,1,1,0,1,0,1,1,1),0b0100101000u
#define PPUTLUTRAITS_0b1011010110u BIN,726u,-298,(1,0,1,1,0,1,0,1,1,0),0b0100101001u
#define PPUTLUTRAITS_0b1011010101u BIN,725u,-299,(1,0,1,1,0,1,0,1,0,1),0b0100101010u
#define PPUTLUTRAITS_0b1011010100u BIN,724u,-300,(1,0,1,1,0,1,0,1,0,0),0b0100101011u
#define PPUTLUTRAITS_0b1011010011u BIN,723u,-301,(1,0,1,1,0,1,0,0,1,1),0b0100101100u
#define PPUTLUTRAITS_0b1011010010u BIN,722u,-302,(1,0,1,1,0,1,0,0,1,0),0b0100101101u
#define PPUTLUTRAITS_0b1011010001u BIN,721u,-303,(1,0,1,1,0,1,0,0,0,1),0b0100101110u
#define PPUTLUTRAITS_0b1011010000u BIN,720u,-304,(1,0,1,1,0,1,0,0,0,0),0b0100101111u
#define PPUTLUTRAITS_0b1011001111u BIN,719u,-305,(1,0,1,1,0,0,1,1,1,1),0b0100110000u
#define PPUTLUTRAITS_0b1011001110u BIN,718u,-306,(1,0,1,1,0,0,1,1,1,0),0b0100110001u
#define PPUTLUTRAITS_0b1011001101u BIN,717u,-307,(1,0,1,1,0,0,1,1,0,1),0b0100110010u
#define PPUTLUTRAITS_0b1011001100u BIN,716u,-308,(1,0,1,1,0,0,1,1,0,0),0b0100110011u
#define PPUTLUTRAITS_0b1011001011u BIN,715u,-309,(1,0,1,1,0,0,1,0,1,1),0b0100110100u
#define PPUTLUTRAITS_0b1011001010u BIN,714u,-310,(1,0,1,1,0,0,1,0,1,0),0b0100110101u
#define PPUTLUTRAITS_0b1011001001u BIN,713u,-311,(1,0,1,1,0,0,1,0,0,1),0b0100110110u
#define PPUTLUTRAITS_0b1011001000u BIN,712u,-312,(1,0,1,1,0,0,1,0,0,0),0b0100110111u
#define PPUTLUTRAITS_0b1011000111u BIN,711u,-313,(1,0,1,1,0,0,0,1,1,1),0b0100111000u
#define PPUTLUTRAITS_0b1011000110u BIN,710u,-314,(1,0,1,1,0,0,0,1,1,0),0b0100111001u
#define PPUTLUTRAITS_0b1011000101u BIN,709u,-315,(1,0,1,1,0,0,0,1,0,1),0b0100111010u
#define PPUTLUTRAITS_0b1011000100u BIN,708u,-316,(1,0,1,1,0,0,0,1,0,0),0b0100111011u
#define PPUTLUTRAITS_0b1011000011u BIN,707u,-317,(1,0,1,1,0,0,0,0,1,1),0b0100111100u
#define PPUTLUTRAITS_0b1011000010u BIN,706u,-318,(1,0,1,1,0,0,0,0,1,0),0b0100111101u
#define PPUTLUTRAITS_0b1011000001u BIN,705u,-319,(1,0,1,1,0,0,0,0,0,1),0b0100111110u
#define PPUTLUTRAITS_0b1011000000u BIN,704u,-320,(1,0,1,1,0,0,0,0,0,0),0b0100111111u
#define PPUTLUTRAITS_0b1010111111u BIN,703u,-321,(1,0,1,0,1,1,1,1,1,1),0b0101000000u
#define PPUTLUTRAITS_0b1010111110u BIN,702u,-322,(1,0,1,0,1,1,1,1,1,0),0b0101000001u
#define PPUTLUTRAITS_0b1010111101u BIN,701u,-323,(1,0,1,0,1,1,1,1,0,1),0b0101000010u
#define PPUTLUTRAITS_0b1010111100u BIN,700u,-324,(1,0,1,0,1,1,1,1,0,0),0b0101000011u
#define PPUTLUTRAITS_0b1010111011u BIN,699u,-325,(1,0,1,0,1,1,1,0,1,1),0b0101000100u
#define PPUTLUTRAITS_0b1010111010u BIN,698u,-326,(1,0,1,0,1,1,1,0,1,0),0b0101000101u
#define PPUTLUTRAITS_0b1010111001u BIN,697u,-327,(1,0,1,0,1,1,1,0,0,1),0b0101000110u
#define PPUTLUTRAITS_0b1010111000u BIN,696u,-328,(1,0,1,0,1,1,1,0,0,0),0b0101000111u
#define PPUTLUTRAITS_0b1010110111u BIN,695u,-329,(1,0,1,0,1,1,0,1,1,1),0b0101001000u
#define PPUTLUTRAITS_0b1010110110u BIN,694u,-330,(1,0,1,0,1,1,0,1,1,0),0b0101001001u
#define PPUTLUTRAITS_0b1010110101u BIN,693u,-331,(1,0,1,0,1,1,0,1,0,1),0b0101001010u
#define PPUTLUTRAITS_0b1010110100u BIN,692u,-332,(1,0,1,0,1,1,0,1,0,0),0b0101001011u
#define PPUTLUTRAITS_0b1010110011u BIN,691u,-333,(1,0,1,0,1,1,0,0,1,1),0b0101001100u
#define PPUTLUTRAITS_0b1010110010u BIN,690u,-334,(1,0,1,0,1,1,0,0,1,0),0b0101001101u
#define PPUTLUTRAITS_0b1010110001u BIN,689u,-335,(1,0,1,0,1,1,0,0,0,1),0b0101001110u
#define PPUTLUTRAITS_0b1010110000u BIN,688u,-336,(1,0,1,0,1,1,0,0,0,0),0b0101001111u
#define PPUTLUTRAITS_0b1010101111u BIN,687u,-337,(1,0,1,0,1,0,1,1,1,1),0b0101010000u
#define PPUTLUTRAITS_0b1010101110u BIN,686u,-338,(1,0,1,0,1,0,1,1,1,0),0b0101010001u
#define PPUTLUTRAITS_0b1010101101u BIN,685u,-339,(1,0,1,0,1,0,1,1,0,1),0b0101010010u
#define PPUTLUTRAITS_0b1010101100u BIN,684u,-340,(1,0,1,0,1,0,1,1,0,0),0b0101010011u
#define PPUTLUTRAITS_0b1010101011u BIN,683u,-341,(1,0,1,0,1,0,1,0,1,1),0b0101010100u
#define PPUTLUTRAITS_0b1010101010u BIN,682u,-342,(1,0,1,0,1,0,1,0,1,0),0b0101010101u
#define PPUTLUTRAITS_0b1010101001u BIN,681u,-343,(1,0,1,0,1,0,1,0,0,1),0b0101010110u
#define PPUTLUTRAITS_0b1010101000u BIN,680u,-344,(1,0,1,0,1,0,1,0,0,0),0b0101010111u
#define PPUTLUTRAITS_0b1010100111u BIN,679u,-345,(1,0,1,0,1,0,0,1,1,1),0b0101011000u
#define PPUTLUTRAITS_0b1010100110u BIN,678u,-346,(1,0,1,0,1,0,0,1,1,0),0b0101011001u
#define PPUTLUTRAITS_0b1010100101u BIN,677u,-347,(1,0,1,0,1,0,0,1,0,1),0b0101011010u
#define PPUTLUTRAITS_0b1010100100u BIN,676u,-348,(1,0,1,0,1,0,0,1,0,0),0b0101011011u
#define PPUTLUTRAITS_0b1010100011u BIN,675u,-349,(1,0,1,0,1,0,0,0,1,1),0b0101011100u
#define PPUTLUTRAITS_0b1010100010u BIN,674u,-350,(1,0,1,0,1,0,0,0,1,0),0b0101011101u
#define PPUTLUTRAITS_0b1010100001u BIN,673u,-351,(1,0,1,0,1,0,0,0,0,1),0b0101011110u
#define PPUTLUTRAITS_0b1010100000u BIN,672u,-352,(1,0,1,0,1,0,0,0,0,0),0b0101011111u
#define PPUTLUTRAITS_0b1010011111u BIN,671u,-353,(1,0,1,0,0,1,1,1,1,1),0b0101100000u
#define PPUTLUTRAITS_0b1010011110u BIN,670u,-354,(1,0,1,0,0,1,1,1,1,0),0b0101100001u
#define PPUTLUTRAITS_0b1010011101u BIN,669u,-355,(1,0,1,0,0,1,1,1,0,1),0b0101100010u
#define PPUTLUTRAITS_0b1010011100u BIN,668u,-356,(1,0,1,0,0,1,1,1,0,0),0b0101100011u
#define PPUTLUTRAITS_0b1010011011u BIN,667u,-357,(1,0,1,0,0,1,1,0,1,1),0b0101100100u
#define PPUTLUTRAITS_0b1010011010u BIN,666u,-358,(1,0,1,0,0,1,1,0,1,0),0b0101100101u
#define PPUTLUTRAITS_0b1010011001u BIN,665u,-359,(1,0,1,0,0,1,1,0,0,1),0b0101100110u
#define PPUTLUTRAITS_0b1010011000u BIN,664u,-360,(1,0,1,0,0,1,1,0,0,0),0b0101100111u
#define PPUTLUTRAITS_0b1010010111u BIN,663u,-361,(1,0,1,0,0,1,0,1,1,1),0b0101101000u
#define PPUTLUTRAITS_0b1010010110u BIN,662u,-362,(1,0,1,0,0,1,0,1,1,0),0b0101101001u
#define PPUTLUTRAITS_0b1010010101u BIN,661u,-363,(1,0,1,0,0,1,0,1,0,1),0b0101101010u
#define PPUTLUTRAITS_0b1010010100u BIN,660u,-364,(1,0,1,0,0,1,0,1,0,0),0b0101101011u
#define PPUTLUTRAITS_0b1010010011u BIN,659u,-365,(1,0,1,0,0,1,0,0,1,1),0b0101101100u
#define PPUTLUTRAITS_0b1010010010u BIN,658u,-366,(1,0,1,0,0,1,0,0,1,0),0b0101101101u
#define PPUTLUTRAITS_0b1010010001u BIN,657u,-367,(1,0,1,0,0,1,0,0,0,1),0b0101101110u
#define PPUTLUTRAITS_0b1010010000u BIN,656u,-368,(1,0,1,0,0,1,0,0,0,0),0b0101101111u
#define PPUTLUTRAITS_0b1010001111u BIN,655u,-369,(1,0,1,0,0,0,1,1,1,1),0b0101110000u
#define PPUTLUTRAITS_0b1010001110u BIN,654u,-370,(1,0,1,0,0,0,1,1,1,0),0b0101110001u
#define PPUTLUTRAITS_0b1010001101u BIN,653u,-371,(1,0,1,0,0,0,1,1,0,1),0b0101110010u
#define PPUTLUTRAITS_0b1010001100u BIN,652u,-372,(1,0,1,0,0,0,1,1,0,0),0b0101110011u
#define PPUTLUTRAITS_0b1010001011u BIN,651u,-373,(1,0,1,0,0,0,1,0,1,1),0b0101110100u
#define PPUTLUTRAITS_0b1010001010u BIN,650u,-374,(1,0,1,0,0,0,1,0,1,0),0b0101110101u
#define PPUTLUTRAITS_0b1010001001u BIN,649u,-375,(1,0,1,0,0,0,1,0,0,1),0b0101110110u
#define PPUTLUTRAITS_0b1010001000u BIN,648u,-376,(1,0,1,0,0,0,1,0,0,0),0b0101110111u
#define PPUTLUTRAITS_0b1010000111u BIN,647u,-377,(1,0,1,0,0,0,0,1,1,1),0b0101111000u
#define PPUTLUTRAITS_0b1010000110u BIN,646u,-378,(1,0,1,0,0,0,0,1,1,0),0b0101111001u
#define PPUTLUTRAITS_0b1010000101u BIN,645u,-379,(1,0,1,0,0,0,0,1,0,1),0b0101111010u
#define PPUTLUTRAITS_0b1010000100u BIN,644u,-380,(1,0,1,0,0,0,0,1,0,0),0b0101111011u
#define PPUTLUTRAITS_0b1010000011u BIN,643u,-381,(1,0,1,0,0,0,0,0,1,1),0b0101111100u
#define PPUTLUTRAITS_0b1010000010u BIN,642u,-382,(1,0,1,0,0,0,0,0,1,0),0b0101111101u
#define PPUTLUTRAITS_0b1010000001u BIN,641u,-383,(1,0,1,0,0,0,0,0,0,1),0b0101111110u
#define PPUTLUTRAITS_0b1010000000u BIN,640u,-384,(1,0,1,0,0,0,0,0,0,0),0b0101111111u
#define PPUTLUTRAITS_0b1001111111u BIN,639u,-385,(1,0,0,1,1,1,1,1,1,1),0b0110000000u
#define PPUTLUTRAITS_0b1001111110u BIN,638u,-386,(1,0,0,1,1,1,1,1,1,0),0b0110000001u
#define PPUTLUTRAITS_0b1001111101u BIN,637u,-387,(1,0,0,1,1,1,1,1,0,1),0b0110000010u
#define PPUTLUTRAITS_0b1001111100u BIN,636u,-388,(1,0,0,1,1,1,1,1,0,0),0b0110000011u
#define PPUTLUTRAITS_0b1001111011u BIN,635u,-389,(1,0,0,1,1,1,1,0,1,1),0b0110000100u
#define PPUTLUTRAITS_0b1001111010u BIN,634u,-390,(1,0,0,1,1,1,1,0,1,0),0b0110000101u
#define PPUTLUTRAITS_0b1001111001u BIN,633u,-391,(1,0,0,1,1,1,1,0,0,1),0b0110000110u
#define PPUTLUTRAITS_0b1001111000u BIN,632u,-392,(1,0,0,1,1,1,1,0,0,0),0b0110000111u
#define PPUTLUTRAITS_0b1001110111u BIN,631u,-393,(1,0,0,1,1,1,0,1,1,1),0b0110001000u
#define PPUTLUTRAITS_0b1001110110u BIN,630u,-394,(1,0,0,1,1,1,0,1,1,0),0b0110001001u
#define PPUTLUTRAITS_0b1001110101u BIN,629u,-395,(1,0,0,1,1,1,0,1,0,1),0b0110001010u
#define PPUTLUTRAITS_0b1001110100u BIN,628u,-396,(1,0,0,1,1,1,0,1,0,0),0b0110001011u
#define PPUTLUTRAITS_0b1001110011u BIN,627u,-397,(1,0,0,1,1,1,0,0,1,1),0b0110001100u
#define PPUTLUTRAITS_0b1001110010u BIN,626u,-398,(1,0,0,1,1,1,0,0,1,0),0b0110001101u
#define PPUTLUTRAITS_0b1001110001u BIN,625u,-399,(1,0,0,1,1,1,0,0,0,1),0b0110001110u
#define PPUTLUTRAITS_0b1001110000u BIN,624u,-400,(1,0,0,1,1,1,0,0,0,0),0b0110001111u
#define PPUTLUTRAITS_0b1001101111u BIN,623u,-401,(1,0,0,1,1,0,1,1,1,1),0b0110010000u
#define PPUTLUTRAITS_0b1001101110u BIN,622u,-402,(1,0,0,1,1,0,1,1,1,0),0b0110010001u
#define PPUTLUTRAITS_0b1001101101u BIN,621u,-403,(1,0,0,1,1,0,1,1,0,1),0b0110010010u
#define PPUTLUTRAITS_0b1001101100u BIN,620u,-404,(1,0,0,1,1,0,1,1,0,0),0b0110010011u
#define PPUTLUTRAITS_0b1001101011u BIN,619u,-405,(1,0,0,1,1,0,1,0,1,1),0b0110010100u
#define PPUTLUTRAITS_0b1001101010u BIN,618u,-406,(1,0,0,1,1,0,1,0,1,0),0b0110010101u
#define PPUTLUTRAITS_0b1001101001u BIN,617u,-407,(1,0,0,1,1,0,1,0,0,1),0b0110010110u
#define PPUTLUTRAITS_0b1001101000u BIN,616u,-408,(1,0,0,1,1,0,1,0,0,0),0b0110010111u
#define PPUTLUTRAITS_0b1001100111u BIN,615u,-409,(1,0,0,1,1,0,0,1,1,1),0b0110011000u
#define PPUTLUTRAITS_0b1001100110u BIN,614u,-410,(1,0,0,1,1,0,0,1,1,0),0b0110011001u
#define PPUTLUTRAITS_0b1001100101u BIN,613u,-411,(1,0,0,1,1,0,0,1,0,1),0b0110011010u
#define PPUTLUTRAITS_0b1001100100u BIN,612u,-412,(1,0,0,1,1,0,0,1,0,0),0b0110011011u
#define PPUTLUTRAITS_0b1001100011u BIN,611u,-413,(1,0,0,1,1,0,0,0,1,1),0b0110011100u
#define PPUTLUTRAITS_0b1001100010u BIN,610u,-414,(1,0,0,1,1,0,0,0,1,0),0b0110011101u
#define PPUTLUTRAITS_0b1001100001u BIN,609u,-415,(1,0,0,1,1,0,0,0,0,1),0b0110011110u
#define PPUTLUTRAITS_0b1001100000u BIN,608u,-416,(1,0,0,1,1,0,0,0,0,0),0b0110011111u
#define PPUTLUTRAITS_0b1001011111u BIN,607u,-417,(1,0,0,1,0,1,1,1,1,1),0b0110100000u
#define PPUTLUTRAITS_0b1001011110u BIN,606u,-418,(1,0,0,1,0,1,1,1,1,0),0b0110100001u
#define PPUTLUTRAITS_0b1001011101u BIN,605u,-419,(1,0,0,1,0,1,1,1,0,1),0b0110100010u
#define PPUTLUTRAITS_0b1001011100u BIN,604u,-420,(1,0,0,1,0,1,1,1,0,0),0b0110100011u
#define PPUTLUTRAITS_0b1001011011u BIN,603u,-421,(1,0,0,1,0,1,1,0,1,1),0b0110100100u
#define PPUTLUTRAITS_0b1001011010u BIN,602u,-422,(1,0,0,1,0,1,1,0,1,0),0b0110100101u
#define PPUTLUTRAITS_0b1001011001u BIN,601u,-423,(1,0,0,1,0,1,1,0,0,1),0b0110100110u
#define PPUTLUTRAITS_0b1001011000u BIN,600u,-424,(1,0,0,1,0,1,1,0,0,0),0b0110100111u
#define PPUTLUTRAITS_0b1001010111u BIN,599u,-425,(1,0,0,1,0,1,0,1,1,1),0b0110101000u
#define PPUTLUTRAITS_0b1001010110u BIN,598u,-426,(1,0,0,1,0,1,0,1,1,0),0b0110101001u
#define PPUTLUTRAITS_0b1001010101u BIN,597u,-427,(1,0,0,1,0,1,0,1,0,1),0b0110101010u
#define PPUTLUTRAITS_0b1001010100u BIN,596u,-428,(1,0,0,1,0,1,0,1,0,0),0b0110101011u
#define PPUTLUTRAITS_0b1001010011u BIN,595u,-429,(1,0,0,1,0,1,0,0,1,1),0b0110101100u
#define PPUTLUTRAITS_0b1001010010u BIN,594u,-430,(1,0,0,1,0,1,0,0,1,0),0b0110101101u
#define PPUTLUTRAITS_0b1001010001u BIN,593u,-431,(1,0,0,1,0,1,0,0,0,1),0b0110101110u
#define PPUTLUTRAITS_0b1001010000u BIN,592u,-432,(1,0,0,1,0,1,0,0,0,0),0b0110101111u
#define PPUTLUTRAITS_0b1001001111u BIN,591u,-433,(1,0,0,1,0,0,1,1,1,1),0b0110110000u
#define PPUTLUTRAITS_0b1001001110u BIN,590u,-434,(1,0,0,1,0,0,1,1,1,0),0b0110110001u
#define PPUTLUTRAITS_0b1001001101u BIN,589u,-435,(1,0,0,1,0,0,1,1,0,1),0b0110110010u
#define PPUTLUTRAITS_0b1001001100u BIN,588u,-436,(1,0,0,1,0,0,1,1,0,0),0b0110110011u
#define PPUTLUTRAITS_0b1001001011u BIN,587u,-437,(1,0,0,1,0,0,1,0,1,1),0b0110110100u
#define PPUTLUTRAITS_0b1001001010u BIN,586u,-438,(1,0,0,1,0,0,1,0,1,0),0b0110110101u
#define PPUTLUTRAITS_0b1001001001u BIN,585u,-439,(1,0,0,1,0,0,1,0,0,1),0b0110110110u
#define PPUTLUTRAITS_0b1001001000u BIN,584u,-440,(1,0,0,1,0,0,1,0,0,0),0b0110110111u
#define PPUTLUTRAITS_0b1001000111u BIN,583u,-441,(1,0,0,1,0,0,0,1,1,1),0b0110111000u
#define PPUTLUTRAITS_0b1001000110u BIN,582u,-442,(1,0,0,1,0,0,0,1,1,0),0b0110111001u
#define PPUTLUTRAITS_0b1001000101u BIN,581u,-443,(1,0,0,1,0,0,0,1,0,1),0b0110111010u
#define PPUTLUTRAITS_0b1001000100u BIN,580u,-444,(1,0,0,1,0,0,0,1,0,0),0b0110111011u
#define PPUTLUTRAITS_0b1001000011u BIN,579u,-445,(1,0,0,1,0,0,0,0,1,1),0b0110111100u
#define PPUTLUTRAITS_0b1001000010u BIN,578u,-446,(1,0,0,1,0,0,0,0,1,0),0b0110111101u
#define PPUTLUTRAITS_0b1001000001u BIN,577u,-447,(1,0,0,1,0,0,0,0,0,1),0b0110111110u
#define PPUTLUTRAITS_0b1001000000u BIN,576u,-448,(1,0,0,1,0,0,0,0,0,0),0b0110111111u
#define PPUTLUTRAITS_0b1000111111u BIN,575u,-449,(1,0,0,0,1,1,1,1,1,1),0b0111000000u
#define PPUTLUTRAITS_0b1000111110u BIN,574u,-450,(1,0,0,0,1,1,1,1,1,0),0b0111000001u
#define PPUTLUTRAITS_0b1000111101u BIN,573u,-451,(1,0,0,0,1,1,1,1,0,1),0b0111000010u
#define PPUTLUTRAITS_0b1000111100u BIN,572u,-452,(1,0,0,0,1,1,1,1,0,0),0b0111000011u
#define PPUTLUTRAITS_0b1000111011u BIN,571u,-453,(1,0,0,0,1,1,1,0,1,1),0b0111000100u
#define PPUTLUTRAITS_0b1000111010u BIN,570u,-454,(1,0,0,0,1,1,1,0,1,0),0b0111000101u
#define PPUTLUTRAITS_0b1000111001u BIN,569u,-455,(1,0,0,0,1,1,1,0,0,1),0b0111000110u
#define PPUTLUTRAITS_0b1000111000u BIN,568u,-456,(1,0,0,0,1,1,1,0,0,0),0b0111000111u
#define PPUTLUTRAITS_0b1000110111u BIN,567u,-457,(1,0,0,0,1,1,0,1,1,1),0b0111001000u
#define PPUTLUTRAITS_0b1000110110u BIN,566u,-458,(1,0,0,0,1,1,0,1,1,0),0b0111001001u
#define PPUTLUTRAITS_0b1000110101u BIN,565u,-459,(1,0,0,0,1,1,0,1,0,1),0b0111001010u
#define PPUTLUTRAITS_0b1000110100u BIN,564u,-460,(1,0,0,0,1,1,0,1,0,0),0b0111001011u
#define PPUTLUTRAITS_0b1000110011u BIN,563u,-461,(1,0,0,0,1,1,0,0,1,1),0b0111001100u
#define PPUTLUTRAITS_0b1000110010u BIN,562u,-462,(1,0,0,0,1,1,0,0,1,0),0b0111001101u
#define PPUTLUTRAITS_0b1000110001u BIN,561u,-463,(1,0,0,0,1,1,0,0,0,1),0b0111001110u
#define PPUTLUTRAITS_0b1000110000u BIN,560u,-464,(1,0,0,0,1,1,0,0,0,0),0b0111001111u
#define PPUTLUTRAITS_0b1000101111u BIN,559u,-465,(1,0,0,0,1,0,1,1,1,1),0b0111010000u
#define PPUTLUTRAITS_0b1000101110u BIN,558u,-466,(1,0,0,0,1,0,1,1,1,0),0b0111010001u
#define PPUTLUTRAITS_0b1000101101u BIN,557u,-467,(1,0,0,0,1,0,1,1,0,1),0b0111010010u
#define PPUTLUTRAITS_0b1000101100u BIN,556u,-468,(1,0,0,0,1,0,1,1,0,0),0b0111010011u
#define PPUTLUTRAITS_0b1000101011u BIN,555u,-469,(1,0,0,0,1,0,1,0,1,1),0b0111010100u
#define PPUTLUTRAITS_0b1000101010u BIN,554u,-470,(1,0,0,0,1,0,1,0,1,0),0b0111010101u
#define PPUTLUTRAITS_0b1000101001u BIN,553u,-471,(1,0,0,0,1,0,1,0,0,1),0b0111010110u
#define PPUTLUTRAITS_0b1000101000u BIN,552u,-472,(1,0,0,0,1,0,1,0,0,0),0b0111010111u
#define PPUTLUTRAITS_0b1000100111u BIN,551u,-473,(1,0,0,0,1,0,0,1,1,1),0b0111011000u
#define PPUTLUTRAITS_0b1000100110u BIN,550u,-474,(1,0,0,0,1,0,0,1,1,0),0b0111011001u
#define PPUTLUTRAITS_0b1000100101u BIN,549u,-475,(1,0,0,0,1,0,0,1,0,1),0b0111011010u
#define PPUTLUTRAITS_0b1000100100u BIN,548u,-476,(1,0,0,0,1,0,0,1,0,0),0b0111011011u
#define PPUTLUTRAITS_0b1000100011u BIN,547u,-477,(1,0,0,0,1,0,0,0,1,1),0b0111011100u
#define PPUTLUTRAITS_0b1000100010u BIN,546u,-478,(1,0,0,0,1,0,0,0,1,0),0b0111011101u
#define PPUTLUTRAITS_0b1000100001u BIN,545u,-479,(1,0,0,0,1,0,0,0,0,1),0b0111011110u
#define PPUTLUTRAITS_0b1000100000u BIN,544u,-480,(1,0,0,0,1,0,0,0,0,0),0b0111011111u
#define PPUTLUTRAITS_0b1000011111u BIN,543u,-481,(1,0,0,0,0,1,1,1,1,1),0b0111100000u
#define PPUTLUTRAITS_0b1000011110u BIN,542u,-482,(1,0,0,0,0,1,1,1,1,0),0b0111100001u
#define PPUTLUTRAITS_0b1000011101u BIN,541u,-483,(1,0,0,0,0,1,1,1,0,1),0b0111100010u
#define PPUTLUTRAITS_0b1000011100u BIN,540u,-484,(1,0,0,0,0,1,1,1,0,0),0b0111100011u
#define PPUTLUTRAITS_0b1000011011u BIN,539u,-485,(1,0,0,0,0,1,1,0,1,1),0b0111100100u
#define PPUTLUTRAITS_0b1000011010u BIN,538u,-486,(1,0,0,0,0,1,1,0,1,0),0b0111100101u
#define PPUTLUTRAITS_0b1000011001u BIN,537u,-487,(1,0,0,0,0,1,1,0,0,1),0b0111100110u
#define PPUTLUTRAITS_0b1000011000u BIN,536u,-488,(1,0,0,0,0,1,1,0,0,0),0b0111100111u
#define PPUTLUTRAITS_0b1000010111u BIN,535u,-489,(1,0,0,0,0,1,0,1,1,1),0b0111101000u
#define PPUTLUTRAITS_0b1000010110u BIN,534u,-490,(1,0,0,0,0,1,0,1,1,0),0b0111101001u
#define PPUTLUTRAITS_0b1000010101u BIN,533u,-491,(1,0,0,0,0,1,0,1,0,1),0b0111101010u
#define PPUTLUTRAITS_0b1000010100u BIN,532u,-492,(1,0,0,0,0,1,0,1,0,0),0b0111101011u
#define PPUTLUTRAITS_0b1000010011u BIN,531u,-493,(1,0,0,0,0,1,0,0,1,1),0b0111101100u
#define PPUTLUTRAITS_0b1000010010u BIN,530u,-494,(1,0,0,0,0,1,0,0,1,0),0b0111101101u
#define PPUTLUTRAITS_0b1000010001u BIN,529u,-495,(1,0,0,0,0,1,0,0,0,1),0b0111101110u
#define PPUTLUTRAITS_0b1000010000u BIN,528u,-496,(1,0,0,0,0,1,0,0,0,0),0b0111101111u
#define PPUTLUTRAITS_0b1000001111u BIN,527u,-497,(1,0,0,0,0,0,1,1,1,1),0b0111110000u
#define PPUTLUTRAITS_0b1000001110u BIN,526u,-498,(1,0,0,0,0,0,1,1,1,0),0b0111110001u
#define PPUTLUTRAITS_0b1000001101u BIN,525u,-499,(1,0,0,0,0,0,1,1,0,1),0b0111110010u
#define PPUTLUTRAITS_0b1000001100u BIN,524u,-500,(1,0,0,0,0,0,1,1,0,0),0b0111110011u
#define PPUTLUTRAITS_0b1000001011u BIN,523u,-501,(1,0,0,0,0,0,1,0,1,1),0b0111110100u
#define PPUTLUTRAITS_0b1000001010u BIN,522u,-502,(1,0,0,0,0,0,1,0,1,0),0b0111110101u
#define PPUTLUTRAITS_0b1000001001u BIN,521u,-503,(1,0,0,0,0,0,1,0,0,1),0b0111110110u
#define PPUTLUTRAITS_0b1000001000u BIN,520u,-504,(1,0,0,0,0,0,1,0,0,0),0b0111110111u
#define PPUTLUTRAITS_0b1000000111u BIN,519u,-505,(1,0,0,0,0,0,0,1,1,1),0b0111111000u
#define PPUTLUTRAITS_0b1000000110u BIN,518u,-506,(1,0,0,0,0,0,0,1,1,0),0b0111111001u
#define PPUTLUTRAITS_0b1000000101u BIN,517u,-507,(1,0,0,0,0,0,0,1,0,1),0b0111111010u
#define PPUTLUTRAITS_0b1000000100u BIN,516u,-508,(1,0,0,0,0,0,0,1,0,0),0b0111111011u
#define PPUTLUTRAITS_0b1000000011u BIN,515u,-509,(1,0,0,0,0,0,0,0,1,1),0b0111111100u
#define PPUTLUTRAITS_0b1000000010u BIN,514u,-510,(1,0,0,0,0,0,0,0,1,0),0b0111111101u
#define PPUTLUTRAITS_0b1000000001u BIN,513u,-511,(1,0,0,0,0,0,0,0,0,1),0b0111111110u
#define PPUTLUTRAITS_0b1000000000u BIN,512u,-512,(1,0,0,0,0,0,0,0,0,0),0b0111111111u
#define PPUTLUTRAITS_0b0111111111u BIN,511u,511,(0,1,1,1,1,1,1,1,1,1),0b1000000000u
#define PPUTLUTRAITS_0b0111111110u BIN,510u,510,(0,1,1,1,1,1,1,1,1,0),0b1000000001u
#define PPUTLUTRAITS_0b0111111101u BIN,509u,509,(0,1,1,1,1,1,1,1,0,1),0b1000000010u
#define PPUTLUTRAITS_0b0111111100u BIN,508u,508,(0,1,1,1,1,1,1,1,0,0),0b1000000011u
#define PPUTLUTRAITS_0b0111111011u BIN,507u,507,(0,1,1,1,1,1,1,0,1,1),0b1000000100u
#define PPUTLUTRAITS_0b0111111010u BIN,506u,506,(0,1,1,1,1,1,1,0,1,0),0b1000000101u
#define PPUTLUTRAITS_0b0111111001u BIN,505u,505,(0,1,1,1,1,1,1,0,0,1),0b1000000110u
#define PPUTLUTRAITS_0b0111111000u BIN,504u,504,(0,1,1,1,1,1,1,0,0,0),0b1000000111u
#define PPUTLUTRAITS_0b0111110111u BIN,503u,503,(0,1,1,1,1,1,0,1,1,1),0b1000001000u
#define PPUTLUTRAITS_0b0111110110u BIN,502u,502,(0,1,1,1,1,1,0,1,1,0),0b1000001001u
#define PPUTLUTRAITS_0b0111110101u BIN,501u,501,(0,1,1,1,1,1,0,1,0,1),0b1000001010u
#define PPUTLUTRAITS_0b0111110100u BIN,500u,500,(0,1,1,1,1,1,0,1,0,0),0b1000001011u
#define PPUTLUTRAITS_0b0111110011u BIN,499u,499,(0,1,1,1,1,1,0,0,1,1),0b1000001100u
#define PPUTLUTRAITS_0b0111110010u BIN,498u,498,(0,1,1,1,1,1,0,0,1,0),0b1000001101u
#define PPUTLUTRAITS_0b0111110001u BIN,497u,497,(0,1,1,1,1,1,0,0,0,1),0b1000001110u
#define PPUTLUTRAITS_0b0111110000u BIN,496u,496,(0,1,1,1,1,1,0,0,0,0),0b1000001111u
#define PPUTLUTRAITS_0b0111101111u BIN,495u,495,(0,1,1,1,1,0,1,1,1,1),0b1000010000u
#define PPUTLUTRAITS_0b0111101110u BIN,494u,494,(0,1,1,1,1,0,1,1,1,0),0b1000010001u
#define PPUTLUTRAITS_0b0111101101u BIN,493u,493,(0,1,1,1,1,0,1,1,0,1),0b1000010010u
#define PPUTLUTRAITS_0b0111101100u BIN,492u,492,(0,1,1,1,1,0,1,1,0,0),0b1000010011u
#define PPUTLUTRAITS_0b0111101011u BIN,491u,491,(0,1,1,1,1,0,1,0,1,1),0b1000010100u
#define PPUTLUTRAITS_0b0111101010u BIN,490u,490,(0,1,1,1,1,0,1,0,1,0),0b1000010101u
#define PPUTLUTRAITS_0b0111101001u BIN,489u,489,(0,1,1,1,1,0,1,0,0,1),0b1000010110u
#define PPUTLUTRAITS_0b0111101000u BIN,488u,488,(0,1,1,1,1,0,1,0,0,0),0b1000010111u
#define PPUTLUTRAITS_0b0111100111u BIN,487u,487,(0,1,1,1,1,0,0,1,1,1),0b1000011000u
#define PPUTLUTRAITS_0b0111100110u BIN,486u,486,(0,1,1,1,1,0,0,1,1,0),0b1000011001u
#define PPUTLUTRAITS_0b0111100101u BIN,485u,485,(0,1,1,1,1,0,0,1,0,1),0b1000011010u
#define PPUTLUTRAITS_0b0111100100u BIN,484u,484,(0,1,1,1,1,0,0,1,0,0),0b1000011011u
#define PPUTLUTRAITS_0b0111100011u BIN,483u,483,(0,1,1,1,1,0,0,0,1,1),0b1000011100u
#define PPUTLUTRAITS_0b0111100010u BIN,482u,482,(0,1,1,1,1,0,0,0,1,0),0b1000011101u
#define PPUTLUTRAITS_0b0111100001u BIN,481u,481,(0,1,1,1,1,0,0,0,0,1),0b1000011110u
#define PPUTLUTRAITS_0b0111100000u BIN,480u,480,(0,1,1,1,1,0,0,0,0,0),0b1000011111u
#define PPUTLUTRAITS_0b0111011111u BIN,479u,479,(0,1,1,1,0,1,1,1,1,1),0b1000100000u
#define PPUTLUTRAITS_0b0111011110u BIN,478u,478,(0,1,1,1,0,1,1,1,1,0),0b1000100001u
#define PPUTLUTRAITS_0b0111011101u BIN,477u,477,(0,1,1,1,0,1,1,1,0,1),0b1000100010u
#define PPUTLUTRAITS_0b0111011100u BIN,476u,476,(0,1,1,1,0,1,1,1,0,0),0b1000100011u
#define PPUTLUTRAITS_0b0111011011u BIN,475u,475,(0,1,1,1,0,1,1,0,1,1),0b1000100100u
#define PPUTLUTRAITS_0b0111011010u BIN,474u,474,(0,1,1,1,0,1,1,0,1,0),0b1000100101u
#define PPUTLUTRAITS_0b0111011001u BIN,473u,473,(0,1,1,1,0,1,1,0,0,1),0b1000100110u
#define PPUTLUTRAITS_0b0111011000u BIN,472u,472,(0,1,1,1,0,1,1,0,0,0),0b1000100111u
#define PPUTLUTRAITS_0b0111010111u BIN,471u,471,(0,1,1,1,0,1,0,1,1,1),0b1000101000u
#define PPUTLUTRAITS_0b0111010110u BIN,470u,470,(0,1,1,1,0,1,0,1,1,0),0b1000101001u
#define PPUTLUTRAITS_0b0111010101u BIN,469u,469,(0,1,1,1,0,1,0,1,0,1),0b1000101010u
#define PPUTLUTRAITS_0b0111010100u BIN,468u,468,(0,1,1,1,0,1,0,1,0,0),0b1000101011u
#define PPUTLUTRAITS_0b0111010011u BIN,467u,467,(0,1,1,1,0,1,0,0,1,1),0b1000101100u
#define PPUTLUTRAITS_0b0111010010u BIN,466u,466,(0,1,1,1,0,1,0,0,1,0),0b1000101101u
#define PPUTLUTRAITS_0b0111010001u BIN,465u,465,(0,1,1,1,0,1,0,0,0,1),0b1000101110u
#define PPUTLUTRAITS_0b0111010000u BIN,464u,464,(0,1,1,1,0,1,0,0,0,0),0b1000101111u
#define PPUTLUTRAITS_0b0111001111u BIN,463u,463,(0,1,1,1,0,0,1,1,1,1),0b1000110000u
#define PPUTLUTRAITS_0b0111001110u BIN,462u,462,(0,1,1,1,0,0,1,1,1,0),0b1000110001u
#define PPUTLUTRAITS_0b0111001101u BIN,461u,461,(0,1,1,1,0,0,1,1,0,1),0b1000110010u
#define PPUTLUTRAITS_0b0111001100u BIN,460u,460,(0,1,1,1,0,0,1,1,0,0),0b1000110011u
#define PPUTLUTRAITS_0b0111001011u BIN,459u,459,(0,1,1,1,0,0,1,0,1,1),0b1000110100u
#define PPUTLUTRAITS_0b0111001010u BIN,458u,458,(0,1,1,1,0,0,1,0,1,0),0b1000110101u
#define PPUTLUTRAITS_0b0111001001u BIN,457u,457,(0,1,1,1,0,0,1,0,0,1),0b1000110110u
#define PPUTLUTRAITS_0b0111001000u BIN,456u,456,(0,1,1,1,0,0,1,0,0,0),0b1000110111u
#define PPUTLUTRAITS_0b0111000111u BIN,455u,455,(0,1,1,1,0,0,0,1,1,1),0b1000111000u
#define PPUTLUTRAITS_0b0111000110u BIN,454u,454,(0,1,1,1,0,0,0,1,1,0),0b1000111001u
#define PPUTLUTRAITS_0b0111000101u BIN,453u,453,(0,1,1,1,0,0,0,1,0,1),0b1000111010u
#define PPUTLUTRAITS_0b0111000100u BIN,452u,452,(0,1,1,1,0,0,0,1,0,0),0b1000111011u
#define PPUTLUTRAITS_0b0111000011u BIN,451u,451,(0,1,1,1,0,0,0,0,1,1),0b1000111100u
#define PPUTLUTRAITS_0b0111000010u BIN,450u,450,(0,1,1,1,0,0,0,0,1,0),0b1000111101u
#define PPUTLUTRAITS_0b0111000001u BIN,449u,449,(0,1,1,1,0,0,0,0,0,1),0b1000111110u
#define PPUTLUTRAITS_0b0111000000u BIN,448u,448,(0,1,1,1,0,0,0,0,0,0),0b1000111111u
#define PPUTLUTRAITS_0b0110111111u BIN,447u,447,(0,1,1,0,1,1,1,1,1,1),0b1001000000u
#define PPUTLUTRAITS_0b0110111110u BIN,446u,446,(0,1,1,0,1,1,1,1,1,0),0b1001000001u
#define PPUTLUTRAITS_0b0110111101u BIN,445u,445,(0,1,1,0,1,1,1,1,0,1),0b1001000010u
#define PPUTLUTRAITS_0b0110111100u BIN,444u,444,(0,1,1,0,1,1,1,1,0,0),0b1001000011u
#define PPUTLUTRAITS_0b0110111011u BIN,443u,443,(0,1,1,0,1,1,1,0,1,1),0b1001000100u
#define PPUTLUTRAITS_0b0110111010u BIN,442u,442,(0,1,1,0,1,1,1,0,1,0),0b1001000101u
#define PPUTLUTRAITS_0b0110111001u BIN,441u,441,(0,1,1,0,1,1,1,0,0,1),0b1001000110u
#define PPUTLUTRAITS_0b0110111000u BIN,440u,440,(0,1,1,0,1,1,1,0,0,0),0b1001000111u
#define PPUTLUTRAITS_0b0110110111u BIN,439u,439,(0,1,1,0,1,1,0,1,1,1),0b1001001000u
#define PPUTLUTRAITS_0b0110110110u BIN,438u,438,(0,1,1,0,1,1,0,1,1,0),0b1001001001u
#define PPUTLUTRAITS_0b0110110101u BIN,437u,437,(0,1,1,0,1,1,0,1,0,1),0b1001001010u
#define PPUTLUTRAITS_0b0110110100u BIN,436u,436,(0,1,1,0,1,1,0,1,0,0),0b1001001011u
#define PPUTLUTRAITS_0b0110110011u BIN,435u,435,(0,1,1,0,1,1,0,0,1,1),0b1001001100u
#define PPUTLUTRAITS_0b0110110010u BIN,434u,434,(0,1,1,0,1,1,0,0,1,0),0b1001001101u
#define PPUTLUTRAITS_0b0110110001u BIN,433u,433,(0,1,1,0,1,1,0,0,0,1),0b1001001110u
#define PPUTLUTRAITS_0b0110110000u BIN,432u,432,(0,1,1,0,1,1,0,0,0,0),0b1001001111u
#define PPUTLUTRAITS_0b0110101111u BIN,431u,431,(0,1,1,0,1,0,1,1,1,1),0b1001010000u
#define PPUTLUTRAITS_0b0110101110u BIN,430u,430,(0,1,1,0,1,0,1,1,1,0),0b1001010001u
#define PPUTLUTRAITS_0b0110101101u BIN,429u,429,(0,1,1,0,1,0,1,1,0,1),0b1001010010u
#define PPUTLUTRAITS_0b0110101100u BIN,428u,428,(0,1,1,0,1,0,1,1,0,0),0b1001010011u
#define PPUTLUTRAITS_0b0110101011u BIN,427u,427,(0,1,1,0,1,0,1,0,1,1),0b1001010100u
#define PPUTLUTRAITS_0b0110101010u BIN,426u,426,(0,1,1,0,1,0,1,0,1,0),0b1001010101u
#define PPUTLUTRAITS_0b0110101001u BIN,425u,425,(0,1,1,0,1,0,1,0,0,1),0b1001010110u
#define PPUTLUTRAITS_0b0110101000u BIN,424u,424,(0,1,1,0,1,0,1,0,0,0),0b1001010111u
#define PPUTLUTRAITS_0b0110100111u BIN,423u,423,(0,1,1,0,1,0,0,1,1,1),0b1001011000u
#define PPUTLUTRAITS_0b0110100110u BIN,422u,422,(0,1,1,0,1,0,0,1,1,0),0b1001011001u
#define PPUTLUTRAITS_0b0110100101u BIN,421u,421,(0,1,1,0,1,0,0,1,0,1),0b1001011010u
#define PPUTLUTRAITS_0b0110100100u BIN,420u,420,(0,1,1,0,1,0,0,1,0,0),0b1001011011u
#define PPUTLUTRAITS_0b0110100011u BIN,419u,419,(0,1,1,0,1,0,0,0,1,1),0b1001011100u
#define PPUTLUTRAITS_0b0110100010u BIN,418u,418,(0,1,1,0,1,0,0,0,1,0),0b1001011101u
#define PPUTLUTRAITS_0b0110100001u BIN,417u,417,(0,1,1,0,1,0,0,0,0,1),0b1001011110u
#define PPUTLUTRAITS_0b0110100000u BIN,416u,416,(0,1,1,0,1,0,0,0,0,0),0b1001011111u
#define PPUTLUTRAITS_0b0110011111u BIN,415u,415,(0,1,1,0,0,1,1,1,1,1),0b1001100000u
#define PPUTLUTRAITS_0b0110011110u BIN,414u,414,(0,1,1,0,0,1,1,1,1,0),0b1001100001u
#define PPUTLUTRAITS_0b0110011101u BIN,413u,413,(0,1,1,0,0,1,1,1,0,1),0b1001100010u
#define PPUTLUTRAITS_0b0110011100u BIN,412u,412,(0,1,1,0,0,1,1,1,0,0),0b1001100011u
#define PPUTLUTRAITS_0b0110011011u BIN,411u,411,(0,1,1,0,0,1,1,0,1,1),0b1001100100u
#define PPUTLUTRAITS_0b0110011010u BIN,410u,410,(0,1,1,0,0,1,1,0,1,0),0b1001100101u
#define PPUTLUTRAITS_0b0110011001u BIN,409u,409,(0,1,1,0,0,1,1,0,0,1),0b1001100110u
#define PPUTLUTRAITS_0b0110011000u BIN,408u,408,(0,1,1,0,0,1,1,0,0,0),0b1001100111u
#define PPUTLUTRAITS_0b0110010111u BIN,407u,407,(0,1,1,0,0,1,0,1,1,1),0b1001101000u
#define PPUTLUTRAITS_0b0110010110u BIN,406u,406,(0,1,1,0,0,1,0,1,1,0),0b1001101001u
#define PPUTLUTRAITS_0b0110010101u BIN,405u,405,(0,1,1,0,0,1,0,1,0,1),0b1001101010u
#define PPUTLUTRAITS_0b0110010100u BIN,404u,404,(0,1,1,0,0,1,0,1,0,0),0b1001101011u
#define PPUTLUTRAITS_0b0110010011u BIN,403u,403,(0,1,1,0,0,1,0,0,1,1),0b1001101100u
#define PPUTLUTRAITS_0b0110010010u BIN,402u,402,(0,1,1,0,0,1,0,0,1,0),0b1001101101u
#define PPUTLUTRAITS_0b0110010001u BIN,401u,401,(0,1,1,0,0,1,0,0,0,1),0b1001101110u
#define PPUTLUTRAITS_0b0110010000u BIN,400u,400,(0,1,1,0,0,1,0,0,0,0),0b1001101111u
#define PPUTLUTRAITS_0b0110001111u BIN,399u,399,(0,1,1,0,0,0,1,1,1,1),0b1001110000u
#define PPUTLUTRAITS_0b0110001110u BIN,398u,398,(0,1,1,0,0,0,1,1,1,0),0b1001110001u
#define PPUTLUTRAITS_0b0110001101u BIN,397u,397,(0,1,1,0,0,0,1,1,0,1),0b1001110010u
#define PPUTLUTRAITS_0b0110001100u BIN,396u,396,(0,1,1,0,0,0,1,1,0,0),0b1001110011u
#define PPUTLUTRAITS_0b0110001011u BIN,395u,395,(0,1,1,0,0,0,1,0,1,1),0b1001110100u
#define PPUTLUTRAITS_0b0110001010u BIN,394u,394,(0,1,1,0,0,0,1,0,1,0),0b1001110101u
#define PPUTLUTRAITS_0b0110001001u BIN,393u,393,(0,1,1,0,0,0,1,0,0,1),0b1001110110u
#define PPUTLUTRAITS_0b0110001000u BIN,392u,392,(0,1,1,0,0,0,1,0,0,0),0b1001110111u
#define PPUTLUTRAITS_0b0110000111u BIN,391u,391,(0,1,1,0,0,0,0,1,1,1),0b1001111000u
#define PPUTLUTRAITS_0b0110000110u BIN,390u,390,(0,1,1,0,0,0,0,1,1,0),0b1001111001u
#define PPUTLUTRAITS_0b0110000101u BIN,389u,389,(0,1,1,0,0,0,0,1,0,1),0b1001111010u
#define PPUTLUTRAITS_0b0110000100u BIN,388u,388,(0,1,1,0,0,0,0,1,0,0),0b1001111011u
#define PPUTLUTRAITS_0b0110000011u BIN,387u,387,(0,1,1,0,0,0,0,0,1,1),0b1001111100u
#define PPUTLUTRAITS_0b0110000010u BIN,386u,386,(0,1,1,0,0,0,0,0,1,0),0b1001111101u
#define PPUTLUTRAITS_0b0110000001u BIN,385u,385,(0,1,1,0,0,0,0,0,0,1),0b1001111110u
#define PPUTLUTRAITS_0b0110000000u BIN,384u,384,(0,1,1,0,0,0,0,0,0,0),0b1001111111u
#define PPUTLUTRAITS_0b0101111111u BIN,383u,383,(0,1,0,1,1,1,1,1,1,1),0b1010000000u
#define PPUTLUTRAITS_0b0101111110u BIN,382u,382,(0,1,0,1,1,1,1,1,1,0),0b1010000001u
#define PPUTLUTRAITS_0b0101111101u BIN,381u,381,(0,1,0,1,1,1,1,1,0,1),0b1010000010u
#define PPUTLUTRAITS_0b0101111100u BIN,380u,380,(0,1,0,1,1,1,1,1,0,0),0b1010000011u
#define PPUTLUTRAITS_0b0101111011u BIN,379u,379,(0,1,0,1,1,1,1,0,1,1),0b1010000100u
#define PPUTLUTRAITS_0b0101111010u BIN,378u,378,(0,1,0,1,1,1,1,0,1,0),0b1010000101u
#define PPUTLUTRAITS_0b0101111001u BIN,377u,377,(0,1,0,1,1,1,1,0,0,1),0b1010000110u
#define PPUTLUTRAITS_0b0101111000u BIN,376u,376,(0,1,0,1,1,1,1,0,0,0),0b1010000111u
#define PPUTLUTRAITS_0b0101110111u BIN,375u,375,(0,1,0,1,1,1,0,1,1,1),0b1010001000u
#define PPUTLUTRAITS_0b0101110110u BIN,374u,374,(0,1,0,1,1,1,0,1,1,0),0b1010001001u
#define PPUTLUTRAITS_0b0101110101u BIN,373u,373,(0,1,0,1,1,1,0,1,0,1),0b1010001010u
#define PPUTLUTRAITS_0b0101110100u BIN,372u,372,(0,1,0,1,1,1,0,1,0,0),0b1010001011u
#define PPUTLUTRAITS_0b0101110011u BIN,371u,371,(0,1,0,1,1,1,0,0,1,1),0b1010001100u
#define PPUTLUTRAITS_0b0101110010u BIN,370u,370,(0,1,0,1,1,1,0,0,1,0),0b1010001101u
#define PPUTLUTRAITS_0b0101110001u BIN,369u,369,(0,1,0,1,1,1,0,0,0,1),0b1010001110u
#define PPUTLUTRAITS_0b0101110000u BIN,368u,368,(0,1,0,1,1,1,0,0,0,0),0b1010001111u
#define PPUTLUTRAITS_0b0101101111u BIN,367u,367,(0,1,0,1,1,0,1,1,1,1),0b1010010000u
#define PPUTLUTRAITS_0b0101101110u BIN,366u,366,(0,1,0,1,1,0,1,1,1,0),0b1010010001u
#define PPUTLUTRAITS_0b0101101101u BIN,365u,365,(0,1,0,1,1,0,1,1,0,1),0b1010010010u
#define PPUTLUTRAITS_0b0101101100u BIN,364u,364,(0,1,0,1,1,0,1,1,0,0),0b1010010011u
#define PPUTLUTRAITS_0b0101101011u BIN,363u,363,(0,1,0,1,1,0,1,0,1,1),0b1010010100u
#define PPUTLUTRAITS_0b0101101010u BIN,362u,362,(0,1,0,1,1,0,1,0,1,0),0b1010010101u
#define PPUTLUTRAITS_0b0101101001u BIN,361u,361,(0,1,0,1,1,0,1,0,0,1),0b1010010110u
#define PPUTLUTRAITS_0b0101101000u BIN,360u,360,(0,1,0,1,1,0,1,0,0,0),0b1010010111u
#define PPUTLUTRAITS_0b0101100111u BIN,359u,359,(0,1,0,1,1,0,0,1,1,1),0b1010011000u
#define PPUTLUTRAITS_0b0101100110u BIN,358u,358,(0,1,0,1,1,0,0,1,1,0),0b1010011001u
#define PPUTLUTRAITS_0b0101100101u BIN,357u,357,(0,1,0,1,1,0,0,1,0,1),0b1010011010u
#define PPUTLUTRAITS_0b0101100100u BIN,356u,356,(0,1,0,1,1,0,0,1,0,0),0b1010011011u
#define PPUTLUTRAITS_0b0101100011u BIN,355u,355,(0,1,0,1,1,0,0,0,1,1),0b1010011100u
#define PPUTLUTRAITS_0b0101100010u BIN,354u,354,(0,1,0,1,1,0,0,0,1,0),0b1010011101u
#define PPUTLUTRAITS_0b0101100001u BIN,353u,353,(0,1,0,1,1,0,0,0,0,1),0b1010011110u
#define PPUTLUTRAITS_0b0101100000u BIN,352u,352,(0,1,0,1,1,0,0,0,0,0),0b1010011111u
#define PPUTLUTRAITS_0b0101011111u BIN,351u,351,(0,1,0,1,0,1,1,1,1,1),0b1010100000u
#define PPUTLUTRAITS_0b0101011110u BIN,350u,350,(0,1,0,1,0,1,1,1,1,0),0b1010100001u
#define PPUTLUTRAITS_0b0101011101u BIN,349u,349,(0,1,0,1,0,1,1,1,0,1),0b1010100010u
#define PPUTLUTRAITS_0b0101011100u BIN,348u,348,(0,1,0,1,0,1,1,1,0,0),0b1010100011u
#define PPUTLUTRAITS_0b0101011011u BIN,347u,347,(0,1,0,1,0,1,1,0,1,1),0b1010100100u
#define PPUTLUTRAITS_0b0101011010u BIN,346u,346,(0,1,0,1,0,1,1,0,1,0),0b1010100101u
#define PPUTLUTRAITS_0b0101011001u BIN,345u,345,(0,1,0,1,0,1,1,0,0,1),0b1010100110u
#define PPUTLUTRAITS_0b0101011000u BIN,344u,344,(0,1,0,1,0,1,1,0,0,0),0b1010100111u
#define PPUTLUTRAITS_0b0101010111u BIN,343u,343,(0,1,0,1,0,1,0,1,1,1),0b1010101000u
#define PPUTLUTRAITS_0b0101010110u BIN,342u,342,(0,1,0,1,0,1,0,1,1,0),0b1010101001u
#define PPUTLUTRAITS_0b0101010101u BIN,341u,341,(0,1,0,1,0,1,0,1,0,1),0b1010101010u
#define PPUTLUTRAITS_0b0101010100u BIN,340u,340,(0,1,0,1,0,1,0,1,0,0),0b1010101011u
#define PPUTLUTRAITS_0b0101010011u BIN,339u,339,(0,1,0,1,0,1,0,0,1,1),0b1010101100u
#define PPUTLUTRAITS_0b0101010010u BIN,338u,338,(0,1,0,1,0,1,0,0,1,0),0b1010101101u
#define PPUTLUTRAITS_0b0101010001u BIN,337u,337,(0,1,0,1,0,1,0,0,0,1),0b1010101110u
#define PPUTLUTRAITS_0b0101010000u BIN,336u,336,(0,1,0,1,0,1,0,0,0,0),0b1010101111u
#define PPUTLUTRAITS_0b0101001111u BIN,335u,335,(0,1,0,1,0,0,1,1,1,1),0b1010110000u
#define PPUTLUTRAITS_0b0101001110u BIN,334u,334,(0,1,0,1,0,0,1,1,1,0),0b1010110001u
#define PPUTLUTRAITS_0b0101001101u BIN,333u,333,(0,1,0,1,0,0,1,1,0,1),0b1010110010u
#define PPUTLUTRAITS_0b0101001100u BIN,332u,332,(0,1,0,1,0,0,1,1,0,0),0b1010110011u
#define PPUTLUTRAITS_0b0101001011u BIN,331u,331,(0,1,0,1,0,0,1,0,1,1),0b1010110100u
#define PPUTLUTRAITS_0b0101001010u BIN,330u,330,(0,1,0,1,0,0,1,0,1,0),0b1010110101u
#define PPUTLUTRAITS_0b0101001001u BIN,329u,329,(0,1,0,1,0,0,1,0,0,1),0b1010110110u
#define PPUTLUTRAITS_0b0101001000u BIN,328u,328,(0,1,0,1,0,0,1,0,0,0),0b1010110111u
#define PPUTLUTRAITS_0b0101000111u BIN,327u,327,(0,1,0,1,0,0,0,1,1,1),0b1010111000u
#define PPUTLUTRAITS_0b0101000110u BIN,326u,326,(0,1,0,1,0,0,0,1,1,0),0b1010111001u
#define PPUTLUTRAITS_0b0101000101u BIN,325u,325,(0,1,0,1,0,0,0,1,0,1),0b1010111010u
#define PPUTLUTRAITS_0b0101000100u BIN,324u,324,(0,1,0,1,0,0,0,1,0,0),0b1010111011u
#define PPUTLUTRAITS_0b0101000011u BIN,323u,323,(0,1,0,1,0,0,0,0,1,1),0b1010111100u
#define PPUTLUTRAITS_0b0101000010u BIN,322u,322,(0,1,0,1,0,0,0,0,1,0),0b1010111101u
#define PPUTLUTRAITS_0b0101000001u BIN,321u,321,(0,1,0,1,0,0,0,0,0,1),0b1010111110u
#define PPUTLUTRAITS_0b0101000000u BIN,320u,320,(0,1,0,1,0,0,0,0,0,0),0b1010111111u
#define PPUTLUTRAITS_0b0100111111u BIN,319u,319,(0,1,0,0,1,1,1,1,1,1),0b1011000000u
#define PPUTLUTRAITS_0b0100111110u BIN,318u,318,(0,1,0,0,1,1,1,1,1,0),0b1011000001u
#define PPUTLUTRAITS_0b0100111101u BIN,317u,317,(0,1,0,0,1,1,1,1,0,1),0b1011000010u
#define PPUTLUTRAITS_0b0100111100u BIN,316u,316,(0,1,0,0,1,1,1,1,0,0),0b1011000011u
#define PPUTLUTRAITS_0b0100111011u BIN,315u,315,(0,1,0,0,1,1,1,0,1,1),0b1011000100u
#define PPUTLUTRAITS_0b0100111010u BIN,314u,314,(0,1,0,0,1,1,1,0,1,0),0b1011000101u
#define PPUTLUTRAITS_0b0100111001u BIN,313u,313,(0,1,0,0,1,1,1,0,0,1),0b1011000110u
#define PPUTLUTRAITS_0b0100111000u BIN,312u,312,(0,1,0,0,1,1,1,0,0,0),0b1011000111u
#define PPUTLUTRAITS_0b0100110111u BIN,311u,311,(0,1,0,0,1,1,0,1,1,1),0b1011001000u
#define PPUTLUTRAITS_0b0100110110u BIN,310u,310,(0,1,0,0,1,1,0,1,1,0),0b1011001001u
#define PPUTLUTRAITS_0b0100110101u BIN,309u,309,(0,1,0,0,1,1,0,1,0,1),0b1011001010u
#define PPUTLUTRAITS_0b0100110100u BIN,308u,308,(0,1,0,0,1,1,0,1,0,0),0b1011001011u
#define PPUTLUTRAITS_0b0100110011u BIN,307u,307,(0,1,0,0,1,1,0,0,1,1),0b1011001100u
#define PPUTLUTRAITS_0b0100110010u BIN,306u,306,(0,1,0,0,1,1,0,0,1,0),0b1011001101u
#define PPUTLUTRAITS_0b0100110001u BIN,305u,305,(0,1,0,0,1,1,0,0,0,1),0b1011001110u
#define PPUTLUTRAITS_0b0100110000u BIN,304u,304,(0,1,0,0,1,1,0,0,0,0),0b1011001111u
#define PPUTLUTRAITS_0b0100101111u BIN,303u,303,(0,1,0,0,1,0,1,1,1,1),0b1011010000u
#define PPUTLUTRAITS_0b0100101110u BIN,302u,302,(0,1,0,0,1,0,1,1,1,0),0b1011010001u
#define PPUTLUTRAITS_0b0100101101u BIN,301u,301,(0,1,0,0,1,0,1,1,0,1),0b1011010010u
#define PPUTLUTRAITS_0b0100101100u BIN,300u,300,(0,1,0,0,1,0,1,1,0,0),0b1011010011u
#define PPUTLUTRAITS_0b0100101011u BIN,299u,299,(0,1,0,0,1,0,1,0,1,1),0b1011010100u
#define PPUTLUTRAITS_0b0100101010u BIN,298u,298,(0,1,0,0,1,0,1,0,1,0),0b1011010101u
#define PPUTLUTRAITS_0b0100101001u BIN,297u,297,(0,1,0,0,1,0,1,0,0,1),0b1011010110u
#define PPUTLUTRAITS_0b0100101000u BIN,296u,296,(0,1,0,0,1,0,1,0,0,0),0b1011010111u
#define PPUTLUTRAITS_0b0100100111u BIN,295u,295,(0,1,0,0,1,0,0,1,1,1),0b1011011000u
#define PPUTLUTRAITS_0b0100100110u BIN,294u,294,(0,1,0,0,1,0,0,1,1,0),0b1011011001u
#define PPUTLUTRAITS_0b0100100101u BIN,293u,293,(0,1,0,0,1,0,0,1,0,1),0b1011011010u
#define PPUTLUTRAITS_0b0100100100u BIN,292u,292,(0,1,0,0,1,0,0,1,0,0),0b1011011011u
#define PPUTLUTRAITS_0b0100100011u BIN,291u,291,(0,1,0,0,1,0,0,0,1,1),0b1011011100u
#define PPUTLUTRAITS_0b0100100010u BIN,290u,290,(0,1,0,0,1,0,0,0,1,0),0b1011011101u
#define PPUTLUTRAITS_0b0100100001u BIN,289u,289,(0,1,0,0,1,0,0,0,0,1),0b1011011110u
#define PPUTLUTRAITS_0b0100100000u BIN,288u,288,(0,1,0,0,1,0,0,0,0,0),0b1011011111u
#define PPUTLUTRAITS_0b0100011111u BIN,287u,287,(0,1,0,0,0,1,1,1,1,1),0b1011100000u
#define PPUTLUTRAITS_0b0100011110u BIN,286u,286,(0,1,0,0,0,1,1,1,1,0),0b1011100001u
#define PPUTLUTRAITS_0b0100011101u BIN,285u,285,(0,1,0,0,0,1,1,1,0,1),0b1011100010u
#define PPUTLUTRAITS_0b0100011100u BIN,284u,284,(0,1,0,0,0,1,1,1,0,0),0b1011100011u
#define PPUTLUTRAITS_0b0100011011u BIN,283u,283,(0,1,0,0,0,1,1,0,1,1),0b1011100100u
#define PPUTLUTRAITS_0b0100011010u BIN,282u,282,(0,1,0,0,0,1,1,0,1,0),0b1011100101u
#define PPUTLUTRAITS_0b0100011001u BIN,281u,281,(0,1,0,0,0,1,1,0,0,1),0b1011100110u
#define PPUTLUTRAITS_0b0100011000u BIN,280u,280,(0,1,0,0,0,1,1,0,0,0),0b1011100111u
#define PPUTLUTRAITS_0b0100010111u BIN,279u,279,(0,1,0,0,0,1,0,1,1,1),0b1011101000u
#define PPUTLUTRAITS_0b0100010110u BIN,278u,278,(0,1,0,0,0,1,0,1,1,0),0b1011101001u
#define PPUTLUTRAITS_0b0100010101u BIN,277u,277,(0,1,0,0,0,1,0,1,0,1),0b1011101010u
#define PPUTLUTRAITS_0b0100010100u BIN,276u,276,(0,1,0,0,0,1,0,1,0,0),0b1011101011u
#define PPUTLUTRAITS_0b0100010011u BIN,275u,275,(0,1,0,0,0,1,0,0,1,1),0b1011101100u
#define PPUTLUTRAITS_0b0100010010u BIN,274u,274,(0,1,0,0,0,1,0,0,1,0),0b1011101101u
#define PPUTLUTRAITS_0b0100010001u BIN,273u,273,(0,1,0,0,0,1,0,0,0,1),0b1011101110u
#define PPUTLUTRAITS_0b0100010000u BIN,272u,272,(0,1,0,0,0,1,0,0,0,0),0b1011101111u
#define PPUTLUTRAITS_0b0100001111u BIN,271u,271,(0,1,0,0,0,0,1,1,1,1),0b1011110000u
#define PPUTLUTRAITS_0b0100001110u BIN,270u,270,(0,1,0,0,0,0,1,1,1,0),0b1011110001u
#define PPUTLUTRAITS_0b0100001101u BIN,269u,269,(0,1,0,0,0,0,1,1,0,1),0b1011110010u
#define PPUTLUTRAITS_0b0100001100u BIN,268u,268,(0,1,0,0,0,0,1,1,0,0),0b1011110011u
#define PPUTLUTRAITS_0b0100001011u BIN,267u,267,(0,1,0,0,0,0,1,0,1,1),0b1011110100u
#define PPUTLUTRAITS_0b0100001010u BIN,266u,266,(0,1,0,0,0,0,1,0,1,0),0b1011110101u
#define PPUTLUTRAITS_0b0100001001u BIN,265u,265,(0,1,0,0,0,0,1,0,0,1),0b1011110110u
#define PPUTLUTRAITS_0b0100001000u BIN,264u,264,(0,1,0,0,0,0,1,0,0,0),0b1011110111u
#define PPUTLUTRAITS_0b0100000111u BIN,263u,263,(0,1,0,0,0,0,0,1,1,1),0b1011111000u
#define PPUTLUTRAITS_0b0100000110u BIN,262u,262,(0,1,0,0,0,0,0,1,1,0),0b1011111001u
#define PPUTLUTRAITS_0b0100000101u BIN,261u,261,(0,1,0,0,0,0,0,1,0,1),0b1011111010u
#define PPUTLUTRAITS_0b0100000100u BIN,260u,260,(0,1,0,0,0,0,0,1,0,0),0b1011111011u
#define PPUTLUTRAITS_0b0100000011u BIN,259u,259,(0,1,0,0,0,0,0,0,1,1),0b1011111100u
#define PPUTLUTRAITS_0b0100000010u BIN,258u,258,(0,1,0,0,0,0,0,0,1,0),0b1011111101u
#define PPUTLUTRAITS_0b0100000001u BIN,257u,257,(0,1,0,0,0,0,0,0,0,1),0b1011111110u
#define PPUTLUTRAITS_0b0100000000u BIN,256u,256,(0,1,0,0,0,0,0,0,0,0),0b1011111111u
#define PPUTLUTRAITS_0b0011111111u BIN,255u,255,(0,0,1,1,1,1,1,1,1,1),0b1100000000u
#define PPUTLUTRAITS_0b0011111110u BIN,254u,254,(0,0,1,1,1,1,1,1,1,0),0b1100000001u
#define PPUTLUTRAITS_0b0011111101u BIN,253u,253,(0,0,1,1,1,1,1,1,0,1),0b1100000010u
#define PPUTLUTRAITS_0b0011111100u BIN,252u,252,(0,0,1,1,1,1,1,1,0,0),0b1100000011u
#define PPUTLUTRAITS_0b0011111011u BIN,251u,251,(0,0,1,1,1,1,1,0,1,1),0b1100000100u
#define PPUTLUTRAITS_0b0011111010u BIN,250u,250,(0,0,1,1,1,1,1,0,1,0),0b1100000101u
#define PPUTLUTRAITS_0b0011111001u BIN,249u,249,(0,0,1,1,1,1,1,0,0,1),0b1100000110u
#define PPUTLUTRAITS_0b0011111000u BIN,248u,248,(0,0,1,1,1,1,1,0,0,0),0b1100000111u
#define PPUTLUTRAITS_0b0011110111u BIN,247u,247,(0,0,1,1,1,1,0,1,1,1),0b1100001000u
#define PPUTLUTRAITS_0b0011110110u BIN,246u,246,(0,0,1,1,1,1,0,1,1,0),0b1100001001u
#define PPUTLUTRAITS_0b0011110101u BIN,245u,245,(0,0,1,1,1,1,0,1,0,1),0b1100001010u
#define PPUTLUTRAITS_0b0011110100u BIN,244u,244,(0,0,1,1,1,1,0,1,0,0),0b1100001011u
#define PPUTLUTRAITS_0b0011110011u BIN,243u,243,(0,0,1,1,1,1,0,0,1,1),0b1100001100u
#define PPUTLUTRAITS_0b0011110010u BIN,242u,242,(0,0,1,1,1,1,0,0,1,0),0b1100001101u
#define PPUTLUTRAITS_0b0011110001u BIN,241u,241,(0,0,1,1,1,1,0,0,0,1),0b1100001110u
#define PPUTLUTRAITS_0b0011110000u BIN,240u,240,(0,0,1,1,1,1,0,0,0,0),0b1100001111u
#define PPUTLUTRAITS_0b0011101111u BIN,239u,239,(0,0,1,1,1,0,1,1,1,1),0b1100010000u
#define PPUTLUTRAITS_0b0011101110u BIN,238u,238,(0,0,1,1,1,0,1,1,1,0),0b1100010001u
#define PPUTLUTRAITS_0b0011101101u BIN,237u,237,(0,0,1,1,1,0,1,1,0,1),0b1100010010u
#define PPUTLUTRAITS_0b0011101100u BIN,236u,236,(0,0,1,1,1,0,1,1,0,0),0b1100010011u
#define PPUTLUTRAITS_0b0011101011u BIN,235u,235,(0,0,1,1,1,0,1,0,1,1),0b1100010100u
#define PPUTLUTRAITS_0b0011101010u BIN,234u,234,(0,0,1,1,1,0,1,0,1,0),0b1100010101u
#define PPUTLUTRAITS_0b0011101001u BIN,233u,233,(0,0,1,1,1,0,1,0,0,1),0b1100010110u
#define PPUTLUTRAITS_0b0011101000u BIN,232u,232,(0,0,1,1,1,0,1,0,0,0),0b1100010111u
#define PPUTLUTRAITS_0b0011100111u BIN,231u,231,(0,0,1,1,1,0,0,1,1,1),0b1100011000u
#define PPUTLUTRAITS_0b0011100110u BIN,230u,230,(0,0,1,1,1,0,0,1,1,0),0b1100011001u
#define PPUTLUTRAITS_0b0011100101u BIN,229u,229,(0,0,1,1,1,0,0,1,0,1),0b1100011010u
#define PPUTLUTRAITS_0b0011100100u BIN,228u,228,(0,0,1,1,1,0,0,1,0,0),0b1100011011u
#define PPUTLUTRAITS_0b0011100011u BIN,227u,227,(0,0,1,1,1,0,0,0,1,1),0b1100011100u
#define PPUTLUTRAITS_0b0011100010u BIN,226u,226,(0,0,1,1,1,0,0,0,1,0),0b1100011101u
#define PPUTLUTRAITS_0b0011100001u BIN,225u,225,(0,0,1,1,1,0,0,0,0,1),0b1100011110u
#define PPUTLUTRAITS_0b0011100000u BIN,224u,224,(0,0,1,1,1,0,0,0,0,0),0b1100011111u
#define PPUTLUTRAITS_0b0011011111u BIN,223u,223,(0,0,1,1,0,1,1,1,1,1),0b1100100000u
#define PPUTLUTRAITS_0b0011011110u BIN,222u,222,(0,0,1,1,0,1,1,1,1,0),0b1100100001u
#define PPUTLUTRAITS_0b0011011101u BIN,221u,221,(0,0,1,1,0,1,1,1,0,1),0b1100100010u
#define PPUTLUTRAITS_0b0011011100u BIN,220u,220,(0,0,1,1,0,1,1,1,0,0),0b1100100011u
#define PPUTLUTRAITS_0b0011011011u BIN,219u,219,(0,0,1,1,0,1,1,0,1,1),0b1100100100u
#define PPUTLUTRAITS_0b0011011010u BIN,218u,218,(0,0,1,1,0,1,1,0,1,0),0b1100100101u
#define PPUTLUTRAITS_0b0011011001u BIN,217u,217,(0,0,1,1,0,1,1,0,0,1),0b1100100110u
#define PPUTLUTRAITS_0b0011011000u BIN,216u,216,(0,0,1,1,0,1,1,0,0,0),0b1100100111u
#define PPUTLUTRAITS_0b0011010111u BIN,215u,215,(0,0,1,1,0,1,0,1,1,1),0b1100101000u
#define PPUTLUTRAITS_0b0011010110u BIN,214u,214,(0,0,1,1,0,1,0,1,1,0),0b1100101001u
#define PPUTLUTRAITS_0b0011010101u BIN,213u,213,(0,0,1,1,0,1,0,1,0,1),0b1100101010u
#define PPUTLUTRAITS_0b0011010100u BIN,212u,212,(0,0,1,1,0,1,0,1,0,0),0b1100101011u
#define PPUTLUTRAITS_0b0011010011u BIN,211u,211,(0,0,1,1,0,1,0,0,1,1),0b1100101100u
#define PPUTLUTRAITS_0b0011010010u BIN,210u,210,(0,0,1,1,0,1,0,0,1,0),0b1100101101u
#define PPUTLUTRAITS_0b0011010001u BIN,209u,209,(0,0,1,1,0,1,0,0,0,1),0b1100101110u
#define PPUTLUTRAITS_0b0011010000u BIN,208u,208,(0,0,1,1,0,1,0,0,0,0),0b1100101111u
#define PPUTLUTRAITS_0b0011001111u BIN,207u,207,(0,0,1,1,0,0,1,1,1,1),0b1100110000u
#define PPUTLUTRAITS_0b0011001110u BIN,206u,206,(0,0,1,1,0,0,1,1,1,0),0b1100110001u
#define PPUTLUTRAITS_0b0011001101u BIN,205u,205,(0,0,1,1,0,0,1,1,0,1),0b1100110010u
#define PPUTLUTRAITS_0b0011001100u BIN,204u,204,(0,0,1,1,0,0,1,1,0,0),0b1100110011u
#define PPUTLUTRAITS_0b0011001011u BIN,203u,203,(0,0,1,1,0,0,1,0,1,1),0b1100110100u
#define PPUTLUTRAITS_0b0011001010u BIN,202u,202,(0,0,1,1,0,0,1,0,1,0),0b1100110101u
#define PPUTLUTRAITS_0b0011001001u BIN,201u,201,(0,0,1,1,0,0,1,0,0,1),0b1100110110u
#define PPUTLUTRAITS_0b0011001000u BIN,200u,200,(0,0,1,1,0,0,1,0,0,0),0b1100110111u
#define PPUTLUTRAITS_0b0011000111u BIN,199u,199,(0,0,1,1,0,0,0,1,1,1),0b1100111000u
#define PPUTLUTRAITS_0b0011000110u BIN,198u,198,(0,0,1,1,0,0,0,1,1,0),0b1100111001u
#define PPUTLUTRAITS_0b0011000101u BIN,197u,197,(0,0,1,1,0,0,0,1,0,1),0b1100111010u
#define PPUTLUTRAITS_0b0011000100u BIN,196u,196,(0,0,1,1,0,0,0,1,0,0),0b1100111011u
#define PPUTLUTRAITS_0b0011000011u BIN,195u,195,(0,0,1,1,0,0,0,0,1,1),0b1100111100u
#define PPUTLUTRAITS_0b0011000010u BIN,194u,194,(0,0,1,1,0,0,0,0,1,0),0b1100111101u
#define PPUTLUTRAITS_0b0011000001u BIN,193u,193,(0,0,1,1,0,0,0,0,0,1),0b1100111110u
#define PPUTLUTRAITS_0b0011000000u BIN,192u,192,(0,0,1,1,0,0,0,0,0,0),0b1100111111u
#define PPUTLUTRAITS_0b0010111111u BIN,191u,191,(0,0,1,0,1,1,1,1,1,1),0b1101000000u
#define PPUTLUTRAITS_0b0010111110u BIN,190u,190,(0,0,1,0,1,1,1,1,1,0),0b1101000001u
#define PPUTLUTRAITS_0b0010111101u BIN,189u,189,(0,0,1,0,1,1,1,1,0,1),0b1101000010u
#define PPUTLUTRAITS_0b0010111100u BIN,188u,188,(0,0,1,0,1,1,1,1,0,0),0b1101000011u
#define PPUTLUTRAITS_0b0010111011u BIN,187u,187,(0,0,1,0,1,1,1,0,1,1),0b1101000100u
#define PPUTLUTRAITS_0b0010111010u BIN,186u,186,(0,0,1,0,1,1,1,0,1,0),0b1101000101u
#define PPUTLUTRAITS_0b0010111001u BIN,185u,185,(0,0,1,0,1,1,1,0,0,1),0b1101000110u
#define PPUTLUTRAITS_0b0010111000u BIN,184u,184,(0,0,1,0,1,1,1,0,0,0),0b1101000111u
#define PPUTLUTRAITS_0b0010110111u BIN,183u,183,(0,0,1,0,1,1,0,1,1,1),0b1101001000u
#define PPUTLUTRAITS_0b0010110110u BIN,182u,182,(0,0,1,0,1,1,0,1,1,0),0b1101001001u
#define PPUTLUTRAITS_0b0010110101u BIN,181u,181,(0,0,1,0,1,1,0,1,0,1),0b1101001010u
#define PPUTLUTRAITS_0b0010110100u BIN,180u,180,(0,0,1,0,1,1,0,1,0,0),0b1101001011u
#define PPUTLUTRAITS_0b0010110011u BIN,179u,179,(0,0,1,0,1,1,0,0,1,1),0b1101001100u
#define PPUTLUTRAITS_0b0010110010u BIN,178u,178,(0,0,1,0,1,1,0,0,1,0),0b1101001101u
#define PPUTLUTRAITS_0b0010110001u BIN,177u,177,(0,0,1,0,1,1,0,0,0,1),0b1101001110u
#define PPUTLUTRAITS_0b0010110000u BIN,176u,176,(0,0,1,0,1,1,0,0,0,0),0b1101001111u
#define PPUTLUTRAITS_0b0010101111u BIN,175u,175,(0,0,1,0,1,0,1,1,1,1),0b1101010000u
#define PPUTLUTRAITS_0b0010101110u BIN,174u,174,(0,0,1,0,1,0,1,1,1,0),0b1101010001u
#define PPUTLUTRAITS_0b0010101101u BIN,173u,173,(0,0,1,0,1,0,1,1,0,1),0b1101010010u
#define PPUTLUTRAITS_0b0010101100u BIN,172u,172,(0,0,1,0,1,0,1,1,0,0),0b1101010011u
#define PPUTLUTRAITS_0b0010101011u BIN,171u,171,(0,0,1,0,1,0,1,0,1,1),0b1101010100u
#define PPUTLUTRAITS_0b0010101010u BIN,170u,170,(0,0,1,0,1,0,1,0,1,0),0b1101010101u
#define PPUTLUTRAITS_0b0010101001u BIN,169u,169,(0,0,1,0,1,0,1,0,0,1),0b1101010110u
#define PPUTLUTRAITS_0b0010101000u BIN,168u,168,(0,0,1,0,1,0,1,0,0,0),0b1101010111u
#define PPUTLUTRAITS_0b0010100111u BIN,167u,167,(0,0,1,0,1,0,0,1,1,1),0b1101011000u
#define PPUTLUTRAITS_0b0010100110u BIN,166u,166,(0,0,1,0,1,0,0,1,1,0),0b1101011001u
#define PPUTLUTRAITS_0b0010100101u BIN,165u,165,(0,0,1,0,1,0,0,1,0,1),0b1101011010u
#define PPUTLUTRAITS_0b0010100100u BIN,164u,164,(0,0,1,0,1,0,0,1,0,0),0b1101011011u
#define PPUTLUTRAITS_0b0010100011u BIN,163u,163,(0,0,1,0,1,0,0,0,1,1),0b1101011100u
#define PPUTLUTRAITS_0b0010100010u BIN,162u,162,(0,0,1,0,1,0,0,0,1,0),0b1101011101u
#define PPUTLUTRAITS_0b0010100001u BIN,161u,161,(0,0,1,0,1,0,0,0,0,1),0b1101011110u
#define PPUTLUTRAITS_0b0010100000u BIN,160u,160,(0,0,1,0,1,0,0,0,0,0),0b1101011111u
#define PPUTLUTRAITS_0b0010011111u BIN,159u,159,(0,0,1,0,0,1,1,1,1,1),0b1101100000u
#define PPUTLUTRAITS_0b0010011110u BIN,158u,158,(0,0,1,0,0,1,1,1,1,0),0b1101100001u
#define PPUTLUTRAITS_0b0010011101u BIN,157u,157,(0,0,1,0,0,1,1,1,0,1),0b1101100010u
#define PPUTLUTRAITS_0b0010011100u BIN,156u,156,(0,0,1,0,0,1,1,1,0,0),0b1101100011u
#define PPUTLUTRAITS_0b0010011011u BIN,155u,155,(0,0,1,0,0,1,1,0,1,1),0b1101100100u
#define PPUTLUTRAITS_0b0010011010u BIN,154u,154,(0,0,1,0,0,1,1,0,1,0),0b1101100101u
#define PPUTLUTRAITS_0b0010011001u BIN,153u,153,(0,0,1,0,0,1,1,0,0,1),0b1101100110u
#define PPUTLUTRAITS_0b0010011000u BIN,152u,152,(0,0,1,0,0,1,1,0,0,0),0b1101100111u
#define PPUTLUTRAITS_0b0010010111u BIN,151u,151,(0,0,1,0,0,1,0,1,1,1),0b1101101000u
#define PPUTLUTRAITS_0b0010010110u BIN,150u,150,(0,0,1,0,0,1,0,1,1,0),0b1101101001u
#define PPUTLUTRAITS_0b0010010101u BIN,149u,149,(0,0,1,0,0,1,0,1,0,1),0b1101101010u
#define PPUTLUTRAITS_0b0010010100u BIN,148u,148,(0,0,1,0,0,1,0,1,0,0),0b1101101011u
#define PPUTLUTRAITS_0b0010010011u BIN,147u,147,(0,0,1,0,0,1,0,0,1,1),0b1101101100u
#define PPUTLUTRAITS_0b0010010010u BIN,146u,146,(0,0,1,0,0,1,0,0,1,0),0b1101101101u
#define PPUTLUTRAITS_0b0010010001u BIN,145u,145,(0,0,1,0,0,1,0,0,0,1),0b1101101110u
#define PPUTLUTRAITS_0b0010010000u BIN,144u,144,(0,0,1,0,0,1,0,0,0,0),0b1101101111u
#define PPUTLUTRAITS_0b0010001111u BIN,143u,143,(0,0,1,0,0,0,1,1,1,1),0b1101110000u
#define PPUTLUTRAITS_0b0010001110u BIN,142u,142,(0,0,1,0,0,0,1,1,1,0),0b1101110001u
#define PPUTLUTRAITS_0b0010001101u BIN,141u,141,(0,0,1,0,0,0,1,1,0,1),0b1101110010u
#define PPUTLUTRAITS_0b0010001100u BIN,140u,140,(0,0,1,0,0,0,1,1,0,0),0b1101110011u
#define PPUTLUTRAITS_0b0010001011u BIN,139u,139,(0,0,1,0,0,0,1,0,1,1),0b1101110100u
#define PPUTLUTRAITS_0b0010001010u BIN,138u,138,(0,0,1,0,0,0,1,0,1,0),0b1101110101u
#define PPUTLUTRAITS_0b0010001001u BIN,137u,137,(0,0,1,0,0,0,1,0,0,1),0b1101110110u
#define PPUTLUTRAITS_0b0010001000u BIN,136u,136,(0,0,1,0,0,0,1,0,0,0),0b1101110111u
#define PPUTLUTRAITS_0b0010000111u BIN,135u,135,(0,0,1,0,0,0,0,1,1,1),0b1101111000u
#define PPUTLUTRAITS_0b0010000110u BIN,134u,134,(0,0,1,0,0,0,0,1,1,0),0b1101111001u
#define PPUTLUTRAITS_0b0010000101u BIN,133u,133,(0,0,1,0,0,0,0,1,0,1),0b1101111010u
#define PPUTLUTRAITS_0b0010000100u BIN,132u,132,(0,0,1,0,0,0,0,1,0,0),0b1101111011u
#define PPUTLUTRAITS_0b0010000011u BIN,131u,131,(0,0,1,0,0,0,0,0,1,1),0b1101111100u
#define PPUTLUTRAITS_0b0010000010u BIN,130u,130,(0,0,1,0,0,0,0,0,1,0),0b1101111101u
#define PPUTLUTRAITS_0b0010000001u BIN,129u,129,(0,0,1,0,0,0,0,0,0,1),0b1101111110u
#define PPUTLUTRAITS_0b0010000000u BIN,128u,128,(0,0,1,0,0,0,0,0,0,0),0b1101111111u
#define PPUTLUTRAITS_0b0001111111u BIN,127u,127,(0,0,0,1,1,1,1,1,1,1),0b1110000000u
#define PPUTLUTRAITS_0b0001111110u BIN,126u,126,(0,0,0,1,1,1,1,1,1,0),0b1110000001u
#define PPUTLUTRAITS_0b0001111101u BIN,125u,125,(0,0,0,1,1,1,1,1,0,1),0b1110000010u
#define PPUTLUTRAITS_0b0001111100u BIN,124u,124,(0,0,0,1,1,1,1,1,0,0),0b1110000011u
#define PPUTLUTRAITS_0b0001111011u BIN,123u,123,(0,0,0,1,1,1,1,0,1,1),0b1110000100u
#define PPUTLUTRAITS_0b0001111010u BIN,122u,122,(0,0,0,1,1,1,1,0,1,0),0b1110000101u
#define PPUTLUTRAITS_0b0001111001u BIN,121u,121,(0,0,0,1,1,1,1,0,0,1),0b1110000110u
#define PPUTLUTRAITS_0b0001111000u BIN,120u,120,(0,0,0,1,1,1,1,0,0,0),0b1110000111u
#define PPUTLUTRAITS_0b0001110111u BIN,119u,119,(0,0,0,1,1,1,0,1,1,1),0b1110001000u
#define PPUTLUTRAITS_0b0001110110u BIN,118u,118,(0,0,0,1,1,1,0,1,1,0),0b1110001001u
#define PPUTLUTRAITS_0b0001110101u BIN,117u,117,(0,0,0,1,1,1,0,1,0,1),0b1110001010u
#define PPUTLUTRAITS_0b0001110100u BIN,116u,116,(0,0,0,1,1,1,0,1,0,0),0b1110001011u
#define PPUTLUTRAITS_0b0001110011u BIN,115u,115,(0,0,0,1,1,1,0,0,1,1),0b1110001100u
#define PPUTLUTRAITS_0b0001110010u BIN,114u,114,(0,0,0,1,1,1,0,0,1,0),0b1110001101u
#define PPUTLUTRAITS_0b0001110001u BIN,113u,113,(0,0,0,1,1,1,0,0,0,1),0b1110001110u
#define PPUTLUTRAITS_0b0001110000u BIN,112u,112,(0,0,0,1,1,1,0,0,0,0),0b1110001111u
#define PPUTLUTRAITS_0b0001101111u BIN,111u,111,(0,0,0,1,1,0,1,1,1,1),0b1110010000u
#define PPUTLUTRAITS_0b0001101110u BIN,110u,110,(0,0,0,1,1,0,1,1,1,0),0b1110010001u
#define PPUTLUTRAITS_0b0001101101u BIN,109u,109,(0,0,0,1,1,0,1,1,0,1),0b1110010010u
#define PPUTLUTRAITS_0b0001101100u BIN,108u,108,(0,0,0,1,1,0,1,1,0,0),0b1110010011u
#define PPUTLUTRAITS_0b0001101011u BIN,107u,107,(0,0,0,1,1,0,1,0,1,1),0b1110010100u
#define PPUTLUTRAITS_0b0001101010u BIN,106u,106,(0,0,0,1,1,0,1,0,1,0),0b1110010101u
#define PPUTLUTRAITS_0b0001101001u BIN,105u,105,(0,0,0,1,1,0,1,0,0,1),0b1110010110u
#define PPUTLUTRAITS_0b0001101000u BIN,104u,104,(0,0,0,1,1,0,1,0,0,0),0b1110010111u
#define PPUTLUTRAITS_0b0001100111u BIN,103u,103,(0,0,0,1,1,0,0,1,1,1),0b1110011000u
#define PPUTLUTRAITS_0b0001100110u BIN,102u,102,(0,0,0,1,1,0,0,1,1,0),0b1110011001u
#define PPUTLUTRAITS_0b0001100101u BIN,101u,101,(0,0,0,1,1,0,0,1,0,1),0b1110011010u
#define PPUTLUTRAITS_0b0001100100u BIN,100u,100,(0,0,0,1,1,0,0,1,0,0),0b1110011011u
#define PPUTLUTRAITS_0b0001100011u BIN,99u,99,(0,0,0,1,1,0,0,0,1,1),0b1110011100u
#define PPUTLUTRAITS_0b0001100010u BIN,98u,98,(0,0,0,1,1,0,0,0,1,0),0b1110011101u
#define PPUTLUTRAITS_0b0001100001u BIN,97u,97,(0,0,0,1,1,0,0,0,0,1),0b1110011110u
#define PPUTLUTRAITS_0b0001100000u BIN,96u,96,(0,0,0,1,1,0,0,0,0,0),0b1110011111u
#define PPUTLUTRAITS_0b0001011111u BIN,95u,95,(0,0,0,1,0,1,1,1,1,1),0b1110100000u
#define PPUTLUTRAITS_0b0001011110u BIN,94u,94,(0,0,0,1,0,1,1,1,1,0),0b1110100001u
#define PPUTLUTRAITS_0b0001011101u BIN,93u,93,(0,0,0,1,0,1,1,1,0,1),0b1110100010u
#define PPUTLUTRAITS_0b0001011100u BIN,92u,92,(0,0,0,1,0,1,1,1,0,0),0b1110100011u
#define PPUTLUTRAITS_0b0001011011u BIN,91u,91,(0,0,0,1,0,1,1,0,1,1),0b1110100100u
#define PPUTLUTRAITS_0b0001011010u BIN,90u,90,(0,0,0,1,0,1,1,0,1,0),0b1110100101u
#define PPUTLUTRAITS_0b0001011001u BIN,89u,89,(0,0,0,1,0,1,1,0,0,1),0b1110100110u
#define PPUTLUTRAITS_0b0001011000u BIN,88u,88,(0,0,0,1,0,1,1,0,0,0),0b1110100111u
#define PPUTLUTRAITS_0b0001010111u BIN,87u,87,(0,0,0,1,0,1,0,1,1,1),0b1110101000u
#define PPUTLUTRAITS_0b0001010110u BIN,86u,86,(0,0,0,1,0,1,0,1,1,0),0b1110101001u
#define PPUTLUTRAITS_0b0001010101u BIN,85u,85,(0,0,0,1,0,1,0,1,0,1),0b1110101010u
#define PPUTLUTRAITS_0b0001010100u BIN,84u,84,(0,0,0,1,0,1,0,1,0,0),0b1110101011u
#define PPUTLUTRAITS_0b0001010011u BIN,83u,83,(0,0,0,1,0,1,0,0,1,1),0b1110101100u
#define PPUTLUTRAITS_0b0001010010u BIN,82u,82,(0,0,0,1,0,1,0,0,1,0),0b1110101101u
#define PPUTLUTRAITS_0b0001010001u BIN,81u,81,(0,0,0,1,0,1,0,0,0,1),0b1110101110u
#define PPUTLUTRAITS_0b0001010000u BIN,80u,80,(0,0,0,1,0,1,0,0,0,0),0b1110101111u
#define PPUTLUTRAITS_0b0001001111u BIN,79u,79,(0,0,0,1,0,0,1,1,1,1),0b1110110000u
#define PPUTLUTRAITS_0b0001001110u BIN,78u,78,(0,0,0,1,0,0,1,1,1,0),0b1110110001u
#define PPUTLUTRAITS_0b0001001101u BIN,77u,77,(0,0,0,1,0,0,1,1,0,1),0b1110110010u
#define PPUTLUTRAITS_0b0001001100u BIN,76u,76,(0,0,0,1,0,0,1,1,0,0),0b1110110011u
#define PPUTLUTRAITS_0b0001001011u BIN,75u,75,(0,0,0,1,0,0,1,0,1,1),0b1110110100u
#define PPUTLUTRAITS_0b0001001010u BIN,74u,74,(0,0,0,1,0,0,1,0,1,0),0b1110110101u
#define PPUTLUTRAITS_0b0001001001u BIN,73u,73,(0,0,0,1,0,0,1,0,0,1),0b1110110110u
#define PPUTLUTRAITS_0b0001001000u BIN,72u,72,(0,0,0,1,0,0,1,0,0,0),0b1110110111u
#define PPUTLUTRAITS_0b0001000111u BIN,71u,71,(0,0,0,1,0,0,0,1,1,1),0b1110111000u
#define PPUTLUTRAITS_0b0001000110u BIN,70u,70,(0,0,0,1,0,0,0,1,1,0),0b1110111001u
#define PPUTLUTRAITS_0b0001000101u BIN,69u,69,(0,0,0,1,0,0,0,1,0,1),0b1110111010u
#define PPUTLUTRAITS_0b0001000100u BIN,68u,68,(0,0,0,1,0,0,0,1,0,0),0b1110111011u
#define PPUTLUTRAITS_0b0001000011u BIN,67u,67,(0,0,0,1,0,0,0,0,1,1),0b1110111100u
#define PPUTLUTRAITS_0b0001000010u BIN,66u,66,(0,0,0,1,0,0,0,0,1,0),0b1110111101u
#define PPUTLUTRAITS_0b0001000001u BIN,65u,65,(0,0,0,1,0,0,0,0,0,1),0b1110111110u
#define PPUTLUTRAITS_0b0001000000u BIN,64u,64,(0,0,0,1,0,0,0,0,0,0),0b1110111111u
#define PPUTLUTRAITS_0b0000111111u BIN,63u,63,(0,0,0,0,1,1,1,1,1,1),0b1111000000u
#define PPUTLUTRAITS_0b0000111110u BIN,62u,62,(0,0,0,0,1,1,1,1,1,0),0b1111000001u
#define PPUTLUTRAITS_0b0000111101u BIN,61u,61,(0,0,0,0,1,1,1,1,0,1),0b1111000010u
#define PPUTLUTRAITS_0b0000111100u BIN,60u,60,(0,0,0,0,1,1,1,1,0,0),0b1111000011u
#define PPUTLUTRAITS_0b0000111011u BIN,59u,59,(0,0,0,0,1,1,1,0,1,1),0b1111000100u
#define PPUTLUTRAITS_0b0000111010u BIN,58u,58,(0,0,0,0,1,1,1,0,1,0),0b1111000101u
#define PPUTLUTRAITS_0b0000111001u BIN,57u,57,(0,0,0,0,1,1,1,0,0,1),0b1111000110u
#define PPUTLUTRAITS_0b0000111000u BIN,56u,56,(0,0,0,0,1,1,1,0,0,0),0b1111000111u
#define PPUTLUTRAITS_0b0000110111u BIN,55u,55,(0,0,0,0,1,1,0,1,1,1),0b1111001000u
#define PPUTLUTRAITS_0b0000110110u BIN,54u,54,(0,0,0,0,1,1,0,1,1,0),0b1111001001u
#define PPUTLUTRAITS_0b0000110101u BIN,53u,53,(0,0,0,0,1,1,0,1,0,1),0b1111001010u
#define PPUTLUTRAITS_0b0000110100u BIN,52u,52,(0,0,0,0,1,1,0,1,0,0),0b1111001011u
#define PPUTLUTRAITS_0b0000110011u BIN,51u,51,(0,0,0,0,1,1,0,0,1,1),0b1111001100u
#define PPUTLUTRAITS_0b0000110010u BIN,50u,50,(0,0,0,0,1,1,0,0,1,0),0b1111001101u
#define PPUTLUTRAITS_0b0000110001u BIN,49u,49,(0,0,0,0,1,1,0,0,0,1),0b1111001110u
#define PPUTLUTRAITS_0b0000110000u BIN,48u,48,(0,0,0,0,1,1,0,0,0,0),0b1111001111u
#define PPUTLUTRAITS_0b0000101111u BIN,47u,47,(0,0,0,0,1,0,1,1,1,1),0b1111010000u
#define PPUTLUTRAITS_0b0000101110u BIN,46u,46,(0,0,0,0,1,0,1,1,1,0),0b1111010001u
#define PPUTLUTRAITS_0b0000101101u BIN,45u,45,(0,0,0,0,1,0,1,1,0,1),0b1111010010u
#define PPUTLUTRAITS_0b0000101100u BIN,44u,44,(0,0,0,0,1,0,1,1,0,0),0b1111010011u
#define PPUTLUTRAITS_0b0000101011u BIN,43u,43,(0,0,0,0,1,0,1,0,1,1),0b1111010100u
#define PPUTLUTRAITS_0b0000101010u BIN,42u,42,(0,0,0,0,1,0,1,0,1,0),0b1111010101u
#define PPUTLUTRAITS_0b0000101001u BIN,41u,41,(0,0,0,0,1,0,1,0,0,1),0b1111010110u
#define PPUTLUTRAITS_0b0000101000u BIN,40u,40,(0,0,0,0,1,0,1,0,0,0),0b1111010111u
#define PPUTLUTRAITS_0b0000100111u BIN,39u,39,(0,0,0,0,1,0,0,1,1,1),0b1111011000u
#define PPUTLUTRAITS_0b0000100110u BIN,38u,38,(0,0,0,0,1,0,0,1,1,0),0b1111011001u
#define PPUTLUTRAITS_0b0000100101u BIN,37u,37,(0,0,0,0,1,0,0,1,0,1),0b1111011010u
#define PPUTLUTRAITS_0b0000100100u BIN,36u,36,(0,0,0,0,1,0,0,1,0,0),0b1111011011u
#define PPUTLUTRAITS_0b0000100011u BIN,35u,35,(0,0,0,0,1,0,0,0,1,1),0b1111011100u
#define PPUTLUTRAITS_0b0000100010u BIN,34u,34,(0,0,0,0,1,0,0,0,1,0),0b1111011101u
#define PPUTLUTRAITS_0b0000100001u BIN,33u,33,(0,0,0,0,1,0,0,0,0,1),0b1111011110u
#define PPUTLUTRAITS_0b0000100000u BIN,32u,32,(0,0,0,0,1,0,0,0,0,0),0b1111011111u
#define PPUTLUTRAITS_0b0000011111u BIN,31u,31,(0,0,0,0,0,1,1,1,1,1),0b1111100000u
#define PPUTLUTRAITS_0b0000011110u BIN,30u,30,(0,0,0,0,0,1,1,1,1,0),0b1111100001u
#define PPUTLUTRAITS_0b0000011101u BIN,29u,29,(0,0,0,0,0,1,1,1,0,1),0b1111100010u
#define PPUTLUTRAITS_0b0000011100u BIN,28u,28,(0,0,0,0,0,1,1,1,0,0),0b1111100011u
#define PPUTLUTRAITS_0b0000011011u BIN,27u,27,(0,0,0,0,0,1,1,0,1,1),0b1111100100u
#define PPUTLUTRAITS_0b0000011010u BIN,26u,26,(0,0,0,0,0,1,1,0,1,0),0b1111100101u
#define PPUTLUTRAITS_0b0000011001u BIN,25u,25,(0,0,0,0,0,1,1,0,0,1),0b1111100110u
#define PPUTLUTRAITS_0b0000011000u BIN,24u,24,(0,0,0,0,0,1,1,0,0,0),0b1111100111u
#define PPUTLUTRAITS_0b0000010111u BIN,23u,23,(0,0,0,0,0,1,0,1,1,1),0b1111101000u
#define PPUTLUTRAITS_0b0000010110u BIN,22u,22,(0,0,0,0,0,1,0,1,1,0),0b1111101001u
#define PPUTLUTRAITS_0b0000010101u BIN,21u,21,(0,0,0,0,0,1,0,1,0,1),0b1111101010u
#define PPUTLUTRAITS_0b0000010100u BIN,20u,20,(0,0,0,0,0,1,0,1,0,0),0b1111101011u
#define PPUTLUTRAITS_0b0000010011u BIN,19u,19,(0,0,0,0,0,1,0,0,1,1),0b1111101100u
#define PPUTLUTRAITS_0b0000010010u BIN,18u,18,(0,0,0,0,0,1,0,0,1,0),0b1111101101u
#define PPUTLUTRAITS_0b0000010001u BIN,17u,17,(0,0,0,0,0,1,0,0,0,1),0b1111101110u
#define PPUTLUTRAITS_0b0000010000u BIN,16u,16,(0,0,0,0,0,1,0,0,0,0),0b1111101111u
#define PPUTLUTRAITS_0b0000001111u BIN,15u,15,(0,0,0,0,0,0,1,1,1,1),0b1111110000u
#define PPUTLUTRAITS_0b0000001110u BIN,14u,14,(0,0,0,0,0,0,1,1,1,0),0b1111110001u
#define PPUTLUTRAITS_0b0000001101u BIN,13u,13,(0,0,0,0,0,0,1,1,0,1),0b1111110010u
#define PPUTLUTRAITS_0b0000001100u BIN,12u,12,(0,0,0,0,0,0,1,1,0,0),0b1111110011u
#define PPUTLUTRAITS_0b0000001011u BIN,11u,11,(0,0,0,0,0,0,1,0,1,1),0b1111110100u
#define PPUTLUTRAITS_0b0000001010u BIN,10u,10,(0,0,0,0,0,0,1,0,1,0),0b1111110101u
#define PPUTLUTRAITS_0b0000001001u BIN,9u,9,(0,0,0,0,0,0,1,0,0,1),0b1111110110u
#define PPUTLUTRAITS_0b0000001000u BIN,8u,8,(0,0,0,0,0,0,1,0,0,0),0b1111110111u
#define PPUTLUTRAITS_0b0000000111u BIN,7u,7,(0,0,0,0,0,0,0,1,1,1),0b1111111000u
#define PPUTLUTRAITS_0b0000000110u BIN,6u,6,(0,0,0,0,0,0,0,1,1,0),0b1111111001u
#define PPUTLUTRAITS_0b0000000101u BIN,5u,5,(0,0,0,0,0,0,0,1,0,1),0b1111111010u
#define PPUTLUTRAITS_0b0000000100u BIN,4u,4,(0,0,0,0,0,0,0,1,0,0),0b1111111011u
#define PPUTLUTRAITS_0b0000000011u BIN,3u,3,(0,0,0,0,0,0,0,0,1,1),0b1111111100u
#define PPUTLUTRAITS_0b0000000010u BIN,2u,2,(0,0,0,0,0,0,0,0,1,0),0b1111111101u
#define PPUTLUTRAITS_0b0000000001u BIN,1u,1,(0,0,0,0,0,0,0,0,0,1),0b1111111110u
#define PPUTLUTRAITS_0b0000000000u BIN,0u,0,(0,0,0,0,0,0,0,0,0,0),0b1111111111u

/// type, signed binary, log2, sqrt, factors
#define PPUTLUTRAITS_1023u DEC,0b1111111111,9u,31u,(3u, 11u, 31u)
#define PPUTLUTRAITS_1022u DEC,0b1111111110,9u,31u,(2u, 7u, 73u)
#define PPUTLUTRAITS_1021u DEC,0b1111111101,9u,31u,()
#define PPUTLUTRAITS_1020u DEC,0b1111111100,9u,31u,(2u, 2u, 3u, 5u, 17u)
#define PPUTLUTRAITS_1019u DEC,0b1111111011,9u,31u,()
#define PPUTLUTRAITS_1018u DEC,0b1111111010,9u,31u,(2u, 509u)
#define PPUTLUTRAITS_1017u DEC,0b1111111001,9u,31u,(3u, 3u, 113u)
#define PPUTLUTRAITS_1016u DEC,0b1111111000,9u,31u,(2u, 2u, 2u, 127u)
#define PPUTLUTRAITS_1015u DEC,0b1111110111,9u,31u,(5u, 7u, 29u)
#define PPUTLUTRAITS_1014u DEC,0b1111110110,9u,31u,(2u, 3u, 13u, 13u)
#define PPUTLUTRAITS_1013u DEC,0b1111110101,9u,31u,()
#define PPUTLUTRAITS_1012u DEC,0b1111110100,9u,31u,(2u, 2u, 11u, 23u)
#define PPUTLUTRAITS_1011u DEC,0b1111110011,9u,31u,(3u, 337u)
#define PPUTLUTRAITS_1010u DEC,0b1111110010,9u,31u,(2u, 5u, 101u)
#define PPUTLUTRAITS_1009u DEC,0b1111110001,9u,31u,()
#define PPUTLUTRAITS_1008u DEC,0b1111110000,9u,31u,(2u, 2u, 2u, 2u, 3u, 3u, 7u)
#define PPUTLUTRAITS_1007u DEC,0b1111101111,9u,31u,(19u, 53u)
#define PPUTLUTRAITS_1006u DEC,0b1111101110,9u,31u,(2u, 503u)
#define PPUTLUTRAITS_1005u DEC,0b1111101101,9u,31u,(3u, 5u, 67u)
#define PPUTLUTRAITS_1004u DEC,0b1111101100,9u,31u,(2u, 2u, 251u)
#define PPUTLUTRAITS_1003u DEC,0b1111101011,9u,31u,(17u, 59u)
#define PPUTLUTRAITS_1002u DEC,0b1111101010,9u,31u,(2u, 3u, 167u)
#define PPUTLUTRAITS_1001u DEC,0b1111101001,9u,31u,(7u, 11u, 13u)
#define PPUTLUTRAITS_1000u DEC,0b1111101000,9u,31u,(2u, 2u, 2u, 5u, 5u, 5u)
#define PPUTLUTRAITS_999u DEC,0b1111100111,9u,31u,(3u, 3u, 3u, 37u)
#define PPUTLUTRAITS_998u DEC,0b1111100110,9u,31u,(2u, 499u)
#define PPUTLUTRAITS_997u DEC,0b1111100101,9u,31u,()
#define PPUTLUTRAITS_996u DEC,0b1111100100,9u,31u,(2u, 2u, 3u, 83u)
#define PPUTLUTRAITS_995u DEC,0b1111100011,9u,31u,(5u, 199u)
#define PPUTLUTRAITS_994u DEC,0b1111100010,9u,31u,(2u, 7u, 71u)
#define PPUTLUTRAITS_993u DEC,0b1111100001,9u,31u,(3u, 331u)
#define PPUTLUTRAITS_992u DEC,0b1111100000,9u,31u,(2u, 2u, 2u, 2u, 2u, 31u)
#define PPUTLUTRAITS_991u DEC,0b1111011111,9u,31u,()
#define PPUTLUTRAITS_990u DEC,0b1111011110,9u,31u,(2u, 3u, 3u, 5u, 11u)
#define PPUTLUTRAITS_989u DEC,0b1111011101,9u,31u,(23u, 43u)
#define PPUTLUTRAITS_988u DEC,0b1111011100,9u,31u,(2u, 2u, 13u, 19u)
#define PPUTLUTRAITS_987u DEC,0b1111011011,9u,31u,(3u, 7u, 47u)
#define PPUTLUTRAITS_986u DEC,0b1111011010,9u,31u,(2u, 17u, 29u)
#define PPUTLUTRAITS_985u DEC,0b1111011001,9u,31u,(5u, 197u)
#define PPUTLUTRAITS_984u DEC,0b1111011000,9u,31u,(2u, 2u, 2u, 3u, 41u)
#define PPUTLUTRAITS_983u DEC,0b1111010111,9u,31u,()
#define PPUTLUTRAITS_982u DEC,0b1111010110,9u,31u,(2u, 491u)
#define PPUTLUTRAITS_981u DEC,0b1111010101,9u,31u,(3u, 3u, 109u)
#define PPUTLUTRAITS_980u DEC,0b1111010100,9u,31u,(2u, 2u, 5u, 7u, 7u)
#define PPUTLUTRAITS_979u DEC,0b1111010011,9u,31u,(11u, 89u)
#define PPUTLUTRAITS_978u DEC,0b1111010010,9u,31u,(2u, 3u, 163u)
#define PPUTLUTRAITS_977u DEC,0b1111010001,9u,31u,()
#define PPUTLUTRAITS_976u DEC,0b1111010000,9u,31u,(2u, 2u, 2u, 2u, 61u)
#define PPUTLUTRAITS_975u DEC,0b1111001111,9u,31u,(3u, 5u, 5u, 13u)
#define PPUTLUTRAITS_974u DEC,0b1111001110,9u,31u,(2u, 487u)
#define PPUTLUTRAITS_973u DEC,0b1111001101,9u,31u,(7u, 139u)
#define PPUTLUTRAITS_972u DEC,0b1111001100,9u,31u,(2u, 2u, 3u, 3u, 3u, 3u, 3u)
#define PPUTLUTRAITS_971u DEC,0b1111001011,9u,31u,()
#define PPUTLUTRAITS_970u DEC,0b1111001010,9u,31u,(2u, 5u, 97u)
#define PPUTLUTRAITS_969u DEC,0b1111001001,9u,31u,(3u, 17u, 19u)
#define PPUTLUTRAITS_968u DEC,0b1111001000,9u,31u,(2u, 2u, 2u, 11u, 11u)
#define PPUTLUTRAITS_967u DEC,0b1111000111,9u,31u,()
#define PPUTLUTRAITS_966u DEC,0b1111000110,9u,31u,(2u, 3u, 7u, 23u)
#define PPUTLUTRAITS_965u DEC,0b1111000101,9u,31u,(5u, 193u)
#define PPUTLUTRAITS_964u DEC,0b1111000100,9u,31u,(2u, 2u, 241u)
#define PPUTLUTRAITS_963u DEC,0b1111000011,9u,31u,(3u, 3u, 107u)
#define PPUTLUTRAITS_962u DEC,0b1111000010,9u,31u,(2u, 13u, 37u)
#define PPUTLUTRAITS_961u DEC,0b1111000001,9u,31u,(31u, 31u)
#define PPUTLUTRAITS_960u DEC,0b1111000000,9u,30u,(2u, 2u, 2u, 2u, 2u, 2u, 3u, 5u)
#define PPUTLUTRAITS_959u DEC,0b1110111111,9u,30u,(7u, 137u)
#define PPUTLUTRAITS_958u DEC,0b1110111110,9u,30u,(2u, 479u)
#define PPUTLUTRAITS_957u DEC,0b1110111101,9u,30u,(3u, 11u, 29u)
#define PPUTLUTRAITS_956u DEC,0b1110111100,9u,30u,(2u, 2u, 239u)
#define PPUTLUTRAITS_955u DEC,0b1110111011,9u,30u,(5u, 191u)
#define PPUTLUTRAITS_954u DEC,0b1110111010,9u,30u,(2u, 3u, 3u, 53u)
#define PPUTLUTRAITS_953u DEC,0b1110111001,9u,30u,()
#define PPUTLUTRAITS_952u DEC,0b1110111000,9u,30u,(2u, 2u, 2u, 7u, 17u)
#define PPUTLUTRAITS_951u DEC,0b1110110111,9u,30u,(3u, 317u)
#define PPUTLUTRAITS_950u DEC,0b1110110110,9u,30u,(2u, 5u, 5u, 19u)
#define PPUTLUTRAITS_949u DEC,0b1110110101,9u,30u,(13u, 73u)
#define PPUTLUTRAITS_948u DEC,0b1110110100,9u,30u,(2u, 2u, 3u, 79u)
#define PPUTLUTRAITS_947u DEC,0b1110110011,9u,30u,()
#define PPUTLUTRAITS_946u DEC,0b1110110010,9u,30u,(2u, 11u, 43u)
#define PPUTLUTRAITS_945u DEC,0b1110110001,9u,30u,(3u, 3u, 3u, 5u, 7u)
#define PPUTLUTRAITS_944u DEC,0b1110110000,9u,30u,(2u, 2u, 2u, 2u, 59u)
#define PPUTLUTRAITS_943u DEC,0b1110101111,9u,30u,(23u, 41u)
#define PPUTLUTRAITS_942u DEC,0b1110101110,9u,30u,(2u, 3u, 157u)
#define PPUTLUTRAITS_941u DEC,0b1110101101,9u,30u,()
#define PPUTLUTRAITS_940u DEC,0b1110101100,9u,30u,(2u, 2u, 5u, 47u)
#define PPUTLUTRAITS_939u DEC,0b1110101011,9u,30u,(3u, 313u)
#define PPUTLUTRAITS_938u DEC,0b1110101010,9u,30u,(2u, 7u, 67u)
#define PPUTLUTRAITS_937u DEC,0b1110101001,9u,30u,()
#define PPUTLUTRAITS_936u DEC,0b1110101000,9u,30u,(2u, 2u, 2u, 3u, 3u, 13u)
#define PPUTLUTRAITS_935u DEC,0b1110100111,9u,30u,(5u, 11u, 17u)
#define PPUTLUTRAITS_934u DEC,0b1110100110,9u,30u,(2u, 467u)
#define PPUTLUTRAITS_933u DEC,0b1110100101,9u,30u,(3u, 311u)
#define PPUTLUTRAITS_932u DEC,0b1110100100,9u,30u,(2u, 2u, 233u)
#define PPUTLUTRAITS_931u DEC,0b1110100011,9u,30u,(7u, 7u, 19u)
#define PPUTLUTRAITS_930u DEC,0b1110100010,9u,30u,(2u, 3u, 5u, 31u)
#define PPUTLUTRAITS_929u DEC,0b1110100001,9u,30u,()
#define PPUTLUTRAITS_928u DEC,0b1110100000,9u,30u,(2u, 2u, 2u, 2u, 2u, 29u)
#define PPUTLUTRAITS_927u DEC,0b1110011111,9u,30u,(3u, 3u, 103u)
#define PPUTLUTRAITS_926u DEC,0b1110011110,9u,30u,(2u, 463u)
#define PPUTLUTRAITS_925u DEC,0b1110011101,9u,30u,(5u, 5u, 37u)
#define PPUTLUTRAITS_924u DEC,0b1110011100,9u,30u,(2u, 2u, 3u, 7u, 11u)
#define PPUTLUTRAITS_923u DEC,0b1110011011,9u,30u,(13u, 71u)
#define PPUTLUTRAITS_922u DEC,0b1110011010,9u,30u,(2u, 461u)
#define PPUTLUTRAITS_921u DEC,0b1110011001,9u,30u,(3u, 307u)
#define PPUTLUTRAITS_920u DEC,0b1110011000,9u,30u,(2u, 2u, 2u, 5u, 23u)
#define PPUTLUTRAITS_919u DEC,0b1110010111,9u,30u,()
#define PPUTLUTRAITS_918u DEC,0b1110010110,9u,30u,(2u, 3u, 3u, 3u, 17u)
#define PPUTLUTRAITS_917u DEC,0b1110010101,9u,30u,(7u, 131u)
#define PPUTLUTRAITS_916u DEC,0b1110010100,9u,30u,(2u, 2u, 229u)
#define PPUTLUTRAITS_915u DEC,0b1110010011,9u,30u,(3u, 5u, 61u)
#define PPUTLUTRAITS_914u DEC,0b1110010010,9u,30u,(2u, 457u)
#define PPUTLUTRAITS_913u DEC,0b1110010001,9u,30u,(11u, 83u)
#define PPUTLUTRAITS_912u DEC,0b1110010000,9u,30u,(2u, 2u, 2u, 2u, 3u, 19u)
#define PPUTLUTRAITS_911u DEC,0b1110001111,9u,30u,()
#define PPUTLUTRAITS_910u DEC,0b1110001110,9u,30u,(2u, 5u, 7u, 13u)
#define PPUTLUTRAITS_909u DEC,0b1110001101,9u,30u,(3u, 3u, 101u)
#define PPUTLUTRAITS_908u DEC,0b1110001100,9u,30u,(2u, 2u, 227u)
#define PPUTLUTRAITS_907u DEC,0b1110001011,9u,30u,()
#define PPUTLUTRAITS_906u DEC,0b1110001010,9u,30u,(2u, 3u, 151u)
#define PPUTLUTRAITS_905u DEC,0b1110001001,9u,30u,(5u, 181u)
#define PPUTLUTRAITS_904u DEC,0b1110001000,9u,30u,(2u, 2u, 2u, 113u)
#define PPUTLUTRAITS_903u DEC,0b1110000111,9u,30u,(3u, 7u, 43u)
#define PPUTLUTRAITS_902u DEC,0b1110000110,9u,30u,(2u, 11u, 41u)
#define PPUTLUTRAITS_901u DEC,0b1110000101,9u,30u,(17u, 53u)
#define PPUTLUTRAITS_900u DEC,0b1110000100,9u,30u,(2u, 2u, 3u, 3u, 5u, 5u)
#define PPUTLUTRAITS_899u DEC,0b1110000011,9u,29u,(29u, 31u)
#define PPUTLUTRAITS_898u DEC,0b1110000010,9u,29u,(2u, 449u)
#define PPUTLUTRAITS_897u DEC,0b1110000001,9u,29u,(3u, 13u, 23u)
#define PPUTLUTRAITS_896u DEC,0b1110000000,9u,29u,(2u, 2u, 2u, 2u, 2u, 2u, 2u, 7u)
#define PPUTLUTRAITS_895u DEC,0b1101111111,9u,29u,(5u, 179u)
#define PPUTLUTRAITS_894u DEC,0b1101111110,9u,29u,(2u, 3u, 149u)
#define PPUTLUTRAITS_893u DEC,0b1101111101,9u,29u,(19u, 47u)
#define PPUTLUTRAITS_892u DEC,0b1101111100,9u,29u,(2u, 2u, 223u)
#define PPUTLUTRAITS_891u DEC,0b1101111011,9u,29u,(3u, 3u, 3u, 3u, 11u)
#define PPUTLUTRAITS_890u DEC,0b1101111010,9u,29u,(2u, 5u, 89u)
#define PPUTLUTRAITS_889u DEC,0b1101111001,9u,29u,(7u, 127u)
#define PPUTLUTRAITS_888u DEC,0b1101111000,9u,29u,(2u, 2u, 2u, 3u, 37u)
#define PPUTLUTRAITS_887u DEC,0b1101110111,9u,29u,()
#define PPUTLUTRAITS_886u DEC,0b1101110110,9u,29u,(2u, 443u)
#define PPUTLUTRAITS_885u DEC,0b1101110101,9u,29u,(3u, 5u, 59u)
#define PPUTLUTRAITS_884u DEC,0b1101110100,9u,29u,(2u, 2u, 13u, 17u)
#define PPUTLUTRAITS_883u DEC,0b1101110011,9u,29u,()
#define PPUTLUTRAITS_882u DEC,0b1101110010,9u,29u,(2u, 3u, 3u, 7u, 7u)
#define PPUTLUTRAITS_881u DEC,0b1101110001,9u,29u,()
#define PPUTLUTRAITS_880u DEC,0b1101110000,9u,29u,(2u, 2u, 2u, 2u, 5u, 11u)
#define PPUTLUTRAITS_879u DEC,0b1101101111,9u,29u,(3u, 293u)
#define PPUTLUTRAITS_878u DEC,0b1101101110,9u,29u,(2u, 439u)
#define PPUTLUTRAITS_877u DEC,0b1101101101,9u,29u,()
#define PPUTLUTRAITS_876u DEC,0b1101101100,9u,29u,(2u, 2u, 3u, 73u)
#define PPUTLUTRAITS_875u DEC,0b1101101011,9u,29u,(5u, 5u, 5u, 7u)
#define PPUTLUTRAITS_874u DEC,0b1101101010,9u,29u,(2u, 19u, 23u)
#define PPUTLUTRAITS_873u DEC,0b1101101001,9u,29u,(3u, 3u, 97u)
#define PPUTLUTRAITS_872u DEC,0b1101101000,9u,29u,(2u, 2u, 2u, 109u)
#define PPUTLUTRAITS_871u DEC,0b1101100111,9u,29u,(13u, 67u)
#define PPUTLUTRAITS_870u DEC,0b1101100110,9u,29u,(2u, 3u, 5u, 29u)
#define PPUTLUTRAITS_869u DEC,0b1101100101,9u,29u,(11u, 79u)
#define PPUTLUTRAITS_868u DEC,0b1101100100,9u,29u,(2u, 2u, 7u, 31u)
#define PPUTLUTRAITS_867u DEC,0b1101100011,9u,29u,(3u, 17u, 17u)
#define PPUTLUTRAITS_866u DEC,0b1101100010,9u,29u,(2u, 433u)
#define PPUTLUTRAITS_865u DEC,0b1101100001,9u,29u,(5u, 173u)
#define PPUTLUTRAITS_864u DEC,0b1101100000,9u,29u,(2u, 2u, 2u, 2u, 2u, 3u, 3u, 3u)
#define PPUTLUTRAITS_863u DEC,0b1101011111,9u,29u,()
#define PPUTLUTRAITS_862u DEC,0b1101011110,9u,29u,(2u, 431u)
#define PPUTLUTRAITS_861u DEC,0b1101011101,9u,29u,(3u, 7u, 41u)
#define PPUTLUTRAITS_860u DEC,0b1101011100,9u,29u,(2u, 2u, 5u, 43u)
#define PPUTLUTRAITS_859u DEC,0b1101011011,9u,29u,()
#define PPUTLUTRAITS_858u DEC,0b1101011010,9u,29u,(2u, 3u, 11u, 13u)
#define PPUTLUTRAITS_857u DEC,0b1101011001,9u,29u,()
#define PPUTLUTRAITS_856u DEC,0b1101011000,9u,29u,(2u, 2u, 2u, 107u)
#define PPUTLUTRAITS_855u DEC,0b1101010111,9u,29u,(3u, 3u, 5u, 19u)
#define PPUTLUTRAITS_854u DEC,0b1101010110,9u,29u,(2u, 7u, 61u)
#define PPUTLUTRAITS_853u DEC,0b1101010101,9u,29u,()
#define PPUTLUTRAITS_852u DEC,0b1101010100,9u,29u,(2u, 2u, 3u, 71u)
#define PPUTLUTRAITS_851u DEC,0b1101010011,9u,29u,(23u, 37u)
#define PPUTLUTRAITS_850u DEC,0b1101010010,9u,29u,(2u, 5u, 5u, 17u)
#define PPUTLUTRAITS_849u DEC,0b1101010001,9u,29u,(3u, 283u)
#define PPUTLUTRAITS_848u DEC,0b1101010000,9u,29u,(2u, 2u, 2u, 2u, 53u)
#define PPUTLUTRAITS_847u DEC,0b1101001111,9u,29u,(7u, 11u, 11u)
#define PPUTLUTRAITS_846u DEC,0b1101001110,9u,29u,(2u, 3u, 3u, 47u)
#define PPUTLUTRAITS_845u DEC,0b1101001101,9u,29u,(5u, 13u, 13u)
#define PPUTLUTRAITS_844u DEC,0b1101001100,9u,29u,(2u, 2u, 211u)
#define PPUTLUTRAITS_843u DEC,0b1101001011,9u,29u,(3u, 281u)
#define PPUTLUTRAITS_842u DEC,0b1101001010,9u,29u,(2u, 421u)
#define PPUTLUTRAITS_841u DEC,0b1101001001,9u,29u,(29u, 29u)
#define PPUTLUTRAITS_840u DEC,0b1101001000,9u,28u,(2u, 2u, 2u, 3u, 5u, 7u)
#define PPUTLUTRAITS_839u DEC,0b1101000111,9u,28u,()
#define PPUTLUTRAITS_838u DEC,0b1101000110,9u,28u,(2u, 419u)
#define PPUTLUTRAITS_837u DEC,0b1101000101,9u,28u,(3u, 3u, 3u, 31u)
#define PPUTLUTRAITS_836u DEC,0b1101000100,9u,28u,(2u, 2u, 11u, 19u)
#define PPUTLUTRAITS_835u DEC,0b1101000011,9u,28u,(5u, 167u)
#define PPUTLUTRAITS_834u DEC,0b1101000010,9u,28u,(2u, 3u, 139u)
#define PPUTLUTRAITS_833u DEC,0b1101000001,9u,28u,(7u, 7u, 17u)
#define PPUTLUTRAITS_832u DEC,0b1101000000,9u,28u,(2u, 2u, 2u, 2u, 2u, 2u, 13u)
#define PPUTLUTRAITS_831u DEC,0b1100111111,9u,28u,(3u, 277u)
#define PPUTLUTRAITS_830u DEC,0b1100111110,9u,28u,(2u, 5u, 83u)
#define PPUTLUTRAITS_829u DEC,0b1100111101,9u,28u,()
#define PPUTLUTRAITS_828u DEC,0b1100111100,9u,28u,(2u, 2u, 3u, 3u, 23u)
#define PPUTLUTRAITS_827u DEC,0b1100111011,9u,28u,()
#define PPUTLUTRAITS_826u DEC,0b1100111010,9u,28u,(2u, 7u, 59u)
#define PPUTLUTRAITS_825u DEC,0b1100111001,9u,28u,(3u, 5u, 5u, 11u)
#define PPUTLUTRAITS_824u DEC,0b1100111000,9u,28u,(2u, 2u, 2u, 103u)
#define PPUTLUTRAITS_823u DEC,0b1100110111,9u,28u,()
#define PPUTLUTRAITS_822u DEC,0b1100110110,9u,28u,(2u, 3u, 137u)
#define PPUTLUTRAITS_821u DEC,0b1100110101,9u,28u,()
#define PPUTLUTRAITS_820u DEC,0b1100110100,9u,28u,(2u, 2u, 5u, 41u)
#define PPUTLUTRAITS_819u DEC,0b1100110011,9u,28u,(3u, 3u, 7u, 13u)
#define PPUTLUTRAITS_818u DEC,0b1100110010,9u,28u,(2u, 409u)
#define PPUTLUTRAITS_817u DEC,0b1100110001,9u,28u,(19u, 43u)
#define PPUTLUTRAITS_816u DEC,0b1100110000,9u,28u,(2u, 2u, 2u, 2u, 3u, 17u)
#define PPUTLUTRAITS_815u DEC,0b1100101111,9u,28u,(5u, 163u)
#define PPUTLUTRAITS_814u DEC,0b1100101110,9u,28u,(2u, 11u, 37u)
#define PPUTLUTRAITS_813u DEC,0b1100101101,9u,28u,(3u, 271u)
#define PPUTLUTRAITS_812u DEC,0b1100101100,9u,28u,(2u, 2u, 7u, 29u)
#define PPUTLUTRAITS_811u DEC,0b1100101011,9u,28u,()
#define PPUTLUTRAITS_810u DEC,0b1100101010,9u,28u,(2u, 3u, 3u, 3u, 3u, 5u)
#define PPUTLUTRAITS_809u DEC,0b1100101001,9u,28u,()
#define PPUTLUTRAITS_808u DEC,0b1100101000,9u,28u,(2u, 2u, 2u, 101u)
#define PPUTLUTRAITS_807u DEC,0b1100100111,9u,28u,(3u, 269u)
#define PPUTLUTRAITS_806u DEC,0b1100100110,9u,28u,(2u, 13u, 31u)
#define PPUTLUTRAITS_805u DEC,0b1100100101,9u,28u,(5u, 7u, 23u)
#define PPUTLUTRAITS_804u DEC,0b1100100100,9u,28u,(2u, 2u, 3u, 67u)
#define PPUTLUTRAITS_803u DEC,0b1100100011,9u,28u,(11u, 73u)
#define PPUTLUTRAITS_802u DEC,0b1100100010,9u,28u,(2u, 401u)
#define PPUTLUTRAITS_801u DEC,0b1100100001,9u,28u,(3u, 3u, 89u)
#define PPUTLUTRAITS_800u DEC,0b1100100000,9u,28u,(2u, 2u, 2u, 2u, 2u, 5u, 5u)
#define PPUTLUTRAITS_799u DEC,0b1100011111,9u,28u,(17u, 47u)
#define PPUTLUTRAITS_798u DEC,0b1100011110,9u,28u,(2u, 3u, 7u, 19u)
#define PPUTLUTRAITS_797u DEC,0b1100011101,9u,28u,()
#define PPUTLUTRAITS_796u DEC,0b1100011100,9u,28u,(2u, 2u, 199u)
#define PPUTLUTRAITS_795u DEC,0b1100011011,9u,28u,(3u, 5u, 53u)
#define PPUTLUTRAITS_794u DEC,0b1100011010,9u,28u,(2u, 397u)
#define PPUTLUTRAITS_793u DEC,0b1100011001,9u,28u,(13u, 61u)
#define PPUTLUTRAITS_792u DEC,0b1100011000,9u,28u,(2u, 2u, 2u, 3u, 3u, 11u)
#define PPUTLUTRAITS_791u DEC,0b1100010111,9u,28u,(7u, 113u)
#define PPUTLUTRAITS_790u DEC,0b1100010110,9u,28u,(2u, 5u, 79u)
#define PPUTLUTRAITS_789u DEC,0b1100010101,9u,28u,(3u, 263u)
#define PPUTLUTRAITS_788u DEC,0b1100010100,9u,28u,(2u, 2u, 197u)
#define PPUTLUTRAITS_787u DEC,0b1100010011,9u,28u,()
#define PPUTLUTRAITS_786u DEC,0b1100010010,9u,28u,(2u, 3u, 131u)
#define PPUTLUTRAITS_785u DEC,0b1100010001,9u,28u,(5u, 157u)
#define PPUTLUTRAITS_784u DEC,0b1100010000,9u,28u,(2u, 2u, 2u, 2u, 7u, 7u)
#define PPUTLUTRAITS_783u DEC,0b1100001111,9u,27u,(3u, 3u, 3u, 29u)
#define PPUTLUTRAITS_782u DEC,0b1100001110,9u,27u,(2u, 17u, 23u)
#define PPUTLUTRAITS_781u DEC,0b1100001101,9u,27u,(11u, 71u)
#define PPUTLUTRAITS_780u DEC,0b1100001100,9u,27u,(2u, 2u, 3u, 5u, 13u)
#define PPUTLUTRAITS_779u DEC,0b1100001011,9u,27u,(19u, 41u)
#define PPUTLUTRAITS_778u DEC,0b1100001010,9u,27u,(2u, 389u)
#define PPUTLUTRAITS_777u DEC,0b1100001001,9u,27u,(3u, 7u, 37u)
#define PPUTLUTRAITS_776u DEC,0b1100001000,9u,27u,(2u, 2u, 2u, 97u)
#define PPUTLUTRAITS_775u DEC,0b1100000111,9u,27u,(5u, 5u, 31u)
#define PPUTLUTRAITS_774u DEC,0b1100000110,9u,27u,(2u, 3u, 3u, 43u)
#define PPUTLUTRAITS_773u DEC,0b1100000101,9u,27u,()
#define PPUTLUTRAITS_772u DEC,0b1100000100,9u,27u,(2u, 2u, 193u)
#define PPUTLUTRAITS_771u DEC,0b1100000011,9u,27u,(3u, 257u)
#define PPUTLUTRAITS_770u DEC,0b1100000010,9u,27u,(2u, 5u, 7u, 11u)
#define PPUTLUTRAITS_769u DEC,0b1100000001,9u,27u,()
#define PPUTLUTRAITS_768u DEC,0b1100000000,9u,27u,(2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 3u)
#define PPUTLUTRAITS_767u DEC,0b1011111111,9u,27u,(13u, 59u)
#define PPUTLUTRAITS_766u DEC,0b1011111110,9u,27u,(2u, 383u)
#define PPUTLUTRAITS_765u DEC,0b1011111101,9u,27u,(3u, 3u, 5u, 17u)
#define PPUTLUTRAITS_764u DEC,0b1011111100,9u,27u,(2u, 2u, 191u)
#define PPUTLUTRAITS_763u DEC,0b1011111011,9u,27u,(7u, 109u)
#define PPUTLUTRAITS_762u DEC,0b1011111010,9u,27u,(2u, 3u, 127u)
#define PPUTLUTRAITS_761u DEC,0b1011111001,9u,27u,()
#define PPUTLUTRAITS_760u DEC,0b1011111000,9u,27u,(2u, 2u, 2u, 5u, 19u)
#define PPUTLUTRAITS_759u DEC,0b1011110111,9u,27u,(3u, 11u, 23u)
#define PPUTLUTRAITS_758u DEC,0b1011110110,9u,27u,(2u, 379u)
#define PPUTLUTRAITS_757u DEC,0b1011110101,9u,27u,()
#define PPUTLUTRAITS_756u DEC,0b1011110100,9u,27u,(2u, 2u, 3u, 3u, 3u, 7u)
#define PPUTLUTRAITS_755u DEC,0b1011110011,9u,27u,(5u, 151u)
#define PPUTLUTRAITS_754u DEC,0b1011110010,9u,27u,(2u, 13u, 29u)
#define PPUTLUTRAITS_753u DEC,0b1011110001,9u,27u,(3u, 251u)
#define PPUTLUTRAITS_752u DEC,0b1011110000,9u,27u,(2u, 2u, 2u, 2u, 47u)
#define PPUTLUTRAITS_751u DEC,0b1011101111,9u,27u,()
#define PPUTLUTRAITS_750u DEC,0b1011101110,9u,27u,(2u, 3u, 5u, 5u, 5u)
#define PPUTLUTRAITS_749u DEC,0b1011101101,9u,27u,(7u, 107u)
#define PPUTLUTRAITS_748u DEC,0b1011101100,9u,27u,(2u, 2u, 11u, 17u)
#define PPUTLUTRAITS_747u DEC,0b1011101011,9u,27u,(3u, 3u, 83u)
#define PPUTLUTRAITS_746u DEC,0b1011101010,9u,27u,(2u, 373u)
#define PPUTLUTRAITS_745u DEC,0b1011101001,9u,27u,(5u, 149u)
#define PPUTLUTRAITS_744u DEC,0b1011101000,9u,27u,(2u, 2u, 2u, 3u, 31u)
#define PPUTLUTRAITS_743u DEC,0b1011100111,9u,27u,()
#define PPUTLUTRAITS_742u DEC,0b1011100110,9u,27u,(2u, 7u, 53u)
#define PPUTLUTRAITS_741u DEC,0b1011100101,9u,27u,(3u, 13u, 19u)
#define PPUTLUTRAITS_740u DEC,0b1011100100,9u,27u,(2u, 2u, 5u, 37u)
#define PPUTLUTRAITS_739u DEC,0b1011100011,9u,27u,()
#define PPUTLUTRAITS_738u DEC,0b1011100010,9u,27u,(2u, 3u, 3u, 41u)
#define PPUTLUTRAITS_737u DEC,0b1011100001,9u,27u,(11u, 67u)
#define PPUTLUTRAITS_736u DEC,0b1011100000,9u,27u,(2u, 2u, 2u, 2u, 2u, 23u)
#define PPUTLUTRAITS_735u DEC,0b1011011111,9u,27u,(3u, 5u, 7u, 7u)
#define PPUTLUTRAITS_734u DEC,0b1011011110,9u,27u,(2u, 367u)
#define PPUTLUTRAITS_733u DEC,0b1011011101,9u,27u,()
#define PPUTLUTRAITS_732u DEC,0b1011011100,9u,27u,(2u, 2u, 3u, 61u)
#define PPUTLUTRAITS_731u DEC,0b1011011011,9u,27u,(17u, 43u)
#define PPUTLUTRAITS_730u DEC,0b1011011010,9u,27u,(2u, 5u, 73u)
#define PPUTLUTRAITS_729u DEC,0b1011011001,9u,27u,(3u, 3u, 3u, 3u, 3u, 3u)
#define PPUTLUTRAITS_728u DEC,0b1011011000,9u,26u,(2u, 2u, 2u, 7u, 13u)
#define PPUTLUTRAITS_727u DEC,0b1011010111,9u,26u,()
#define PPUTLUTRAITS_726u DEC,0b1011010110,9u,26u,(2u, 3u, 11u, 11u)
#define PPUTLUTRAITS_725u DEC,0b1011010101,9u,26u,(5u, 5u, 29u)
#define PPUTLUTRAITS_724u DEC,0b1011010100,9u,26u,(2u, 2u, 181u)
#define PPUTLUTRAITS_723u DEC,0b1011010011,9u,26u,(3u, 241u)
#define PPUTLUTRAITS_722u DEC,0b1011010010,9u,26u,(2u, 19u, 19u)
#define PPUTLUTRAITS_721u DEC,0b1011010001,9u,26u,(7u, 103u)
#define PPUTLUTRAITS_720u DEC,0b1011010000,9u,26u,(2u, 2u, 2u, 2u, 3u, 3u, 5u)
#define PPUTLUTRAITS_719u DEC,0b1011001111,9u,26u,()
#define PPUTLUTRAITS_718u DEC,0b1011001110,9u,26u,(2u, 359u)
#define PPUTLUTRAITS_717u DEC,0b1011001101,9u,26u,(3u, 239u)
#define PPUTLUTRAITS_716u DEC,0b1011001100,9u,26u,(2u, 2u, 179u)
#define PPUTLUTRAITS_715u DEC,0b1011001011,9u,26u,(5u, 11u, 13u)
#define PPUTLUTRAITS_714u DEC,0b1011001010,9u,26u,(2u, 3u, 7u, 17u)
#define PPUTLUTRAITS_713u DEC,0b1011001001,9u,26u,(23u, 31u)
#define PPUTLUTRAITS_712u DEC,0b1011001000,9u,26u,(2u, 2u, 2u, 89u)
#define PPUTLUTRAITS_711u DEC,0b1011000111,9u,26u,(3u, 3u, 79u)
#define PPUTLUTRAITS_710u DEC,0b1011000110,9u,26u,(2u, 5u, 71u)
#define PPUTLUTRAITS_709u DEC,0b1011000101,9u,26u,()
#define PPUTLUTRAITS_708u DEC,0b1011000100,9u,26u,(2u, 2u, 3u, 59u)
#define PPUTLUTRAITS_707u DEC,0b1011000011,9u,26u,(7u, 101u)
#define PPUTLUTRAITS_706u DEC,0b1011000010,9u,26u,(2u, 353u)
#define PPUTLUTRAITS_705u DEC,0b1011000001,9u,26u,(3u, 5u, 47u)
#define PPUTLUTRAITS_704u DEC,0b1011000000,9u,26u,(2u, 2u, 2u, 2u, 2u, 2u, 11u)
#define PPUTLUTRAITS_703u DEC,0b1010111111,9u,26u,(19u, 37u)
#define PPUTLUTRAITS_702u DEC,0b1010111110,9u,26u,(2u, 3u, 3u, 3u, 13u)
#define PPUTLUTRAITS_701u DEC,0b1010111101,9u,26u,()
#define PPUTLUTRAITS_700u DEC,0b1010111100,9u,26u,(2u, 2u, 5u, 5u, 7u)
#define PPUTLUTRAITS_699u DEC,0b1010111011,9u,26u,(3u, 233u)
#define PPUTLUTRAITS_698u DEC,0b1010111010,9u,26u,(2u, 349u)
#define PPUTLUTRAITS_697u DEC,0b1010111001,9u,26u,(17u, 41u)
#define PPUTLUTRAITS_696u DEC,0b1010111000,9u,26u,(2u, 2u, 2u, 3u, 29u)
#define PPUTLUTRAITS_695u DEC,0b1010110111,9u,26u,(5u, 139u)
#define PPUTLUTRAITS_694u DEC,0b1010110110,9u,26u,(2u, 347u)
#define PPUTLUTRAITS_693u DEC,0b1010110101,9u,26u,(3u, 3u, 7u, 11u)
#define PPUTLUTRAITS_692u DEC,0b1010110100,9u,26u,(2u, 2u, 173u)
#define PPUTLUTRAITS_691u DEC,0b1010110011,9u,26u,()
#define PPUTLUTRAITS_690u DEC,0b1010110010,9u,26u,(2u, 3u, 5u, 23u)
#define PPUTLUTRAITS_689u DEC,0b1010110001,9u,26u,(13u, 53u)
#define PPUTLUTRAITS_688u DEC,0b1010110000,9u,26u,(2u, 2u, 2u, 2u, 43u)
#define PPUTLUTRAITS_687u DEC,0b1010101111,9u,26u,(3u, 229u)
#define PPUTLUTRAITS_686u DEC,0b1010101110,9u,26u,(2u, 7u, 7u, 7u)
#define PPUTLUTRAITS_685u DEC,0b1010101101,9u,26u,(5u, 137u)
#define PPUTLUTRAITS_684u DEC,0b1010101100,9u,26u,(2u, 2u, 3u, 3u, 19u)
#define PPUTLUTRAITS_683u DEC,0b1010101011,9u,26u,()
#define PPUTLUTRAITS_682u DEC,0b1010101010,9u,26u,(2u, 11u, 31u)
#define PPUTLUTRAITS_681u DEC,0b1010101001,9u,26u,(3u, 227u)
#define PPUTLUTRAITS_680u DEC,0b1010101000,9u,26u,(2u, 2u, 2u, 5u, 17u)
#define PPUTLUTRAITS_679u DEC,0b1010100111,9u,26u,(7u, 97u)
#define PPUTLUTRAITS_678u DEC,0b1010100110,9u,26u,(2u, 3u, 113u)
#define PPUTLUTRAITS_677u DEC,0b1010100101,9u,26u,()
#define PPUTLUTRAITS_676u DEC,0b1010100100,9u,26u,(2u, 2u, 13u, 13u)
#define PPUTLUTRAITS_675u DEC,0b1010100011,9u,25u,(3u, 3u, 3u, 5u, 5u)
#define PPUTLUTRAITS_674u DEC,0b1010100010,9u,25u,(2u, 337u)
#define PPUTLUTRAITS_673u DEC,0b1010100001,9u,25u,()
#define PPUTLUTRAITS_672u DEC,0b1010100000,9u,25u,(2u, 2u, 2u, 2u, 2u, 3u, 7u)
#define PPUTLUTRAITS_671u DEC,0b1010011111,9u,25u,(11u, 61u)
#define PPUTLUTRAITS_670u DEC,0b1010011110,9u,25u,(2u, 5u, 67u)
#define PPUTLUTRAITS_669u DEC,0b1010011101,9u,25u,(3u, 223u)
#define PPUTLUTRAITS_668u DEC,0b1010011100,9u,25u,(2u, 2u, 167u)
#define PPUTLUTRAITS_667u DEC,0b1010011011,9u,25u,(23u, 29u)
#define PPUTLUTRAITS_666u DEC,0b1010011010,9u,25u,(2u, 3u, 3u, 37u)
#define PPUTLUTRAITS_665u DEC,0b1010011001,9u,25u,(5u, 7u, 19u)
#define PPUTLUTRAITS_664u DEC,0b1010011000,9u,25u,(2u, 2u, 2u, 83u)
#define PPUTLUTRAITS_663u DEC,0b1010010111,9u,25u,(3u, 13u, 17u)
#define PPUTLUTRAITS_662u DEC,0b1010010110,9u,25u,(2u, 331u)
#define PPUTLUTRAITS_661u DEC,0b1010010101,9u,25u,()
#define PPUTLUTRAITS_660u DEC,0b1010010100,9u,25u,(2u, 2u, 3u, 5u, 11u)
#define PPUTLUTRAITS_659u DEC,0b1010010011,9u,25u,()
#define PPUTLUTRAITS_658u DEC,0b1010010010,9u,25u,(2u, 7u, 47u)
#define PPUTLUTRAITS_657u DEC,0b1010010001,9u,25u,(3u, 3u, 73u)
#define PPUTLUTRAITS_656u DEC,0b1010010000,9u,25u,(2u, 2u, 2u, 2u, 41u)
#define PPUTLUTRAITS_655u DEC,0b1010001111,9u,25u,(5u, 131u)
#define PPUTLUTRAITS_654u DEC,0b1010001110,9u,25u,(2u, 3u, 109u)
#define PPUTLUTRAITS_653u DEC,0b1010001101,9u,25u,()
#define PPUTLUTRAITS_652u DEC,0b1010001100,9u,25u,(2u, 2u, 163u)
#define PPUTLUTRAITS_651u DEC,0b1010001011,9u,25u,(3u, 7u, 31u)
#define PPUTLUTRAITS_650u DEC,0b1010001010,9u,25u,(2u, 5u, 5u, 13u)
#define PPUTLUTRAITS_649u DEC,0b1010001001,9u,25u,(11u, 59u)
#define PPUTLUTRAITS_648u DEC,0b1010001000,9u,25u,(2u, 2u, 2u, 3u, 3u, 3u, 3u)
#define PPUTLUTRAITS_647u DEC,0b1010000111,9u,25u,()
#define PPUTLUTRAITS_646u DEC,0b1010000110,9u,25u,(2u, 17u, 19u)
#define PPUTLUTRAITS_645u DEC,0b1010000101,9u,25u,(3u, 5u, 43u)
#define PPUTLUTRAITS_644u DEC,0b1010000100,9u,25u,(2u, 2u, 7u, 23u)
#define PPUTLUTRAITS_643u DEC,0b1010000011,9u,25u,()
#define PPUTLUTRAITS_642u DEC,0b1010000010,9u,25u,(2u, 3u, 107u)
#define PPUTLUTRAITS_641u DEC,0b1010000001,9u,25u,()
#define PPUTLUTRAITS_640u DEC,0b1010000000,9u,25u,(2u, 2u, 2u, 2u, 2u, 2u, 2u, 5u)
#define PPUTLUTRAITS_639u DEC,0b1001111111,9u,25u,(3u, 3u, 71u)
#define PPUTLUTRAITS_638u DEC,0b1001111110,9u,25u,(2u, 11u, 29u)
#define PPUTLUTRAITS_637u DEC,0b1001111101,9u,25u,(7u, 7u, 13u)
#define PPUTLUTRAITS_636u DEC,0b1001111100,9u,25u,(2u, 2u, 3u, 53u)
#define PPUTLUTRAITS_635u DEC,0b1001111011,9u,25u,(5u, 127u)
#define PPUTLUTRAITS_634u DEC,0b1001111010,9u,25u,(2u, 317u)
#define PPUTLUTRAITS_633u DEC,0b1001111001,9u,25u,(3u, 211u)
#define PPUTLUTRAITS_632u DEC,0b1001111000,9u,25u,(2u, 2u, 2u, 79u)
#define PPUTLUTRAITS_631u DEC,0b1001110111,9u,25u,()
#define PPUTLUTRAITS_630u DEC,0b1001110110,9u,25u,(2u, 3u, 3u, 5u, 7u)
#define PPUTLUTRAITS_629u DEC,0b1001110101,9u,25u,(17u, 37u)
#define PPUTLUTRAITS_628u DEC,0b1001110100,9u,25u,(2u, 2u, 157u)
#define PPUTLUTRAITS_627u DEC,0b1001110011,9u,25u,(3u, 11u, 19u)
#define PPUTLUTRAITS_626u DEC,0b1001110010,9u,25u,(2u, 313u)
#define PPUTLUTRAITS_625u DEC,0b1001110001,9u,25u,(5u, 5u, 5u, 5u)
#define PPUTLUTRAITS_624u DEC,0b1001110000,9u,24u,(2u, 2u, 2u, 2u, 3u, 13u)
#define PPUTLUTRAITS_623u DEC,0b1001101111,9u,24u,(7u, 89u)
#define PPUTLUTRAITS_622u DEC,0b1001101110,9u,24u,(2u, 311u)
#define PPUTLUTRAITS_621u DEC,0b1001101101,9u,24u,(3u, 3u, 3u, 23u)
#define PPUTLUTRAITS_620u DEC,0b1001101100,9u,24u,(2u, 2u, 5u, 31u)
#define PPUTLUTRAITS_619u DEC,0b1001101011,9u,24u,()
#define PPUTLUTRAITS_618u DEC,0b1001101010,9u,24u,(2u, 3u, 103u)
#define PPUTLUTRAITS_617u DEC,0b1001101001,9u,24u,()
#define PPUTLUTRAITS_616u DEC,0b1001101000,9u,24u,(2u, 2u, 2u, 7u, 11u)
#define PPUTLUTRAITS_615u DEC,0b1001100111,9u,24u,(3u, 5u, 41u)
#define PPUTLUTRAITS_614u DEC,0b1001100110,9u,24u,(2u, 307u)
#define PPUTLUTRAITS_613u DEC,0b1001100101,9u,24u,()
#define PPUTLUTRAITS_612u DEC,0b1001100100,9u,24u,(2u, 2u, 3u, 3u, 17u)
#define PPUTLUTRAITS_611u DEC,0b1001100011,9u,24u,(13u, 47u)
#define PPUTLUTRAITS_610u DEC,0b1001100010,9u,24u,(2u, 5u, 61u)
#define PPUTLUTRAITS_609u DEC,0b1001100001,9u,24u,(3u, 7u, 29u)
#define PPUTLUTRAITS_608u DEC,0b1001100000,9u,24u,(2u, 2u, 2u, 2u, 2u, 19u)
#define PPUTLUTRAITS_607u DEC,0b1001011111,9u,24u,()
#define PPUTLUTRAITS_606u DEC,0b1001011110,9u,24u,(2u, 3u, 101u)
#define PPUTLUTRAITS_605u DEC,0b1001011101,9u,24u,(5u, 11u, 11u)
#define PPUTLUTRAITS_604u DEC,0b1001011100,9u,24u,(2u, 2u, 151u)
#define PPUTLUTRAITS_603u DEC,0b1001011011,9u,24u,(3u, 3u, 67u)
#define PPUTLUTRAITS_602u DEC,0b1001011010,9u,24u,(2u, 7u, 43u)
#define PPUTLUTRAITS_601u DEC,0b1001011001,9u,24u,()
#define PPUTLUTRAITS_600u DEC,0b1001011000,9u,24u,(2u, 2u, 2u, 3u, 5u, 5u)
#define PPUTLUTRAITS_599u DEC,0b1001010111,9u,24u,()
#define PPUTLUTRAITS_598u DEC,0b1001010110,9u,24u,(2u, 13u, 23u)
#define PPUTLUTRAITS_597u DEC,0b1001010101,9u,24u,(3u, 199u)
#define PPUTLUTRAITS_596u DEC,0b1001010100,9u,24u,(2u, 2u, 149u)
#define PPUTLUTRAITS_595u DEC,0b1001010011,9u,24u,(5u, 7u, 17u)
#define PPUTLUTRAITS_594u DEC,0b1001010010,9u,24u,(2u, 3u, 3u, 3u, 11u)
#define PPUTLUTRAITS_593u DEC,0b1001010001,9u,24u,()
#define PPUTLUTRAITS_592u DEC,0b1001010000,9u,24u,(2u, 2u, 2u, 2u, 37u)
#define PPUTLUTRAITS_591u DEC,0b1001001111,9u,24u,(3u, 197u)
#define PPUTLUTRAITS_590u DEC,0b1001001110,9u,24u,(2u, 5u, 59u)
#define PPUTLUTRAITS_589u DEC,0b1001001101,9u,24u,(19u, 31u)
#define PPUTLUTRAITS_588u DEC,0b1001001100,9u,24u,(2u, 2u, 3u, 7u, 7u)
#define PPUTLUTRAITS_587u DEC,0b1001001011,9u,24u,()
#define PPUTLUTRAITS_586u DEC,0b1001001010,9u,24u,(2u, 293u)
#define PPUTLUTRAITS_585u DEC,0b1001001001,9u,24u,(3u, 3u, 5u, 13u)
#define PPUTLUTRAITS_584u DEC,0b1001001000,9u,24u,(2u, 2u, 2u, 73u)
#define PPUTLUTRAITS_583u DEC,0b1001000111,9u,24u,(11u, 53u)
#define PPUTLUTRAITS_582u DEC,0b1001000110,9u,24u,(2u, 3u, 97u)
#define PPUTLUTRAITS_581u DEC,0b1001000101,9u,24u,(7u, 83u)
#define PPUTLUTRAITS_580u DEC,0b1001000100,9u,24u,(2u, 2u, 5u, 29u)
#define PPUTLUTRAITS_579u DEC,0b1001000011,9u,24u,(3u, 193u)
#define PPUTLUTRAITS_578u DEC,0b1001000010,9u,24u,(2u, 17u, 17u)
#define PPUTLUTRAITS_577u DEC,0b1001000001,9u,24u,()
#define PPUTLUTRAITS_576u DEC,0b1001000000,9u,24u,(2u, 2u, 2u, 2u, 2u, 2u, 3u, 3u)
#define PPUTLUTRAITS_575u DEC,0b1000111111,9u,23u,(5u, 5u, 23u)
#define PPUTLUTRAITS_574u DEC,0b1000111110,9u,23u,(2u, 7u, 41u)
#define PPUTLUTRAITS_573u DEC,0b1000111101,9u,23u,(3u, 191u)
#define PPUTLUTRAITS_572u DEC,0b1000111100,9u,23u,(2u, 2u, 11u, 13u)
#define PPUTLUTRAITS_571u DEC,0b1000111011,9u,23u,()
#define PPUTLUTRAITS_570u DEC,0b1000111010,9u,23u,(2u, 3u, 5u, 19u)
#define PPUTLUTRAITS_569u DEC,0b1000111001,9u,23u,()
#define PPUTLUTRAITS_568u DEC,0b1000111000,9u,23u,(2u, 2u, 2u, 71u)
#define PPUTLUTRAITS_567u DEC,0b1000110111,9u,23u,(3u, 3u, 3u, 3u, 7u)
#define PPUTLUTRAITS_566u DEC,0b1000110110,9u,23u,(2u, 283u)
#define PPUTLUTRAITS_565u DEC,0b1000110101,9u,23u,(5u, 113u)
#define PPUTLUTRAITS_564u DEC,0b1000110100,9u,23u,(2u, 2u, 3u, 47u)
#define PPUTLUTRAITS_563u DEC,0b1000110011,9u,23u,()
#define PPUTLUTRAITS_562u DEC,0b1000110010,9u,23u,(2u, 281u)
#define PPUTLUTRAITS_561u DEC,0b1000110001,9u,23u,(3u, 11u, 17u)
#define PPUTLUTRAITS_560u DEC,0b1000110000,9u,23u,(2u, 2u, 2u, 2u, 5u, 7u)
#define PPUTLUTRAITS_559u DEC,0b1000101111,9u,23u,(13u, 43u)
#define PPUTLUTRAITS_558u DEC,0b1000101110,9u,23u,(2u, 3u, 3u, 31u)
#define PPUTLUTRAITS_557u DEC,0b1000101101,9u,23u,()
#define PPUTLUTRAITS_556u DEC,0b1000101100,9u,23u,(2u, 2u, 139u)
#define PPUTLUTRAITS_555u DEC,0b1000101011,9u,23u,(3u, 5u, 37u)
#define PPUTLUTRAITS_554u DEC,0b1000101010,9u,23u,(2u, 277u)
#define PPUTLUTRAITS_553u DEC,0b1000101001,9u,23u,(7u, 79u)
#define PPUTLUTRAITS_552u DEC,0b1000101000,9u,23u,(2u, 2u, 2u, 3u, 23u)
#define PPUTLUTRAITS_551u DEC,0b1000100111,9u,23u,(19u, 29u)
#define PPUTLUTRAITS_550u DEC,0b1000100110,9u,23u,(2u, 5u, 5u, 11u)
#define PPUTLUTRAITS_549u DEC,0b1000100101,9u,23u,(3u, 3u, 61u)
#define PPUTLUTRAITS_548u DEC,0b1000100100,9u,23u,(2u, 2u, 137u)
#define PPUTLUTRAITS_547u DEC,0b1000100011,9u,23u,()
#define PPUTLUTRAITS_546u DEC,0b1000100010,9u,23u,(2u, 3u, 7u, 13u)
#define PPUTLUTRAITS_545u DEC,0b1000100001,9u,23u,(5u, 109u)
#define PPUTLUTRAITS_544u DEC,0b1000100000,9u,23u,(2u, 2u, 2u, 2u, 2u, 17u)
#define PPUTLUTRAITS_543u DEC,0b1000011111,9u,23u,(3u, 181u)
#define PPUTLUTRAITS_542u DEC,0b1000011110,9u,23u,(2u, 271u)
#define PPUTLUTRAITS_541u DEC,0b1000011101,9u,23u,()
#define PPUTLUTRAITS_540u DEC,0b1000011100,9u,23u,(2u, 2u, 3u, 3u, 3u, 5u)
#define PPUTLUTRAITS_539u DEC,0b1000011011,9u,23u,(7u, 7u, 11u)
#define PPUTLUTRAITS_538u DEC,0b1000011010,9u,23u,(2u, 269u)
#define PPUTLUTRAITS_537u DEC,0b1000011001,9u,23u,(3u, 179u)
#define PPUTLUTRAITS_536u DEC,0b1000011000,9u,23u,(2u, 2u, 2u, 67u)
#define PPUTLUTRAITS_535u DEC,0b1000010111,9u,23u,(5u, 107u)
#define PPUTLUTRAITS_534u DEC,0b1000010110,9u,23u,(2u, 3u, 89u)
#define PPUTLUTRAITS_533u DEC,0b1000010101,9u,23u,(13u, 41u)
#define PPUTLUTRAITS_532u DEC,0b1000010100,9u,23u,(2u, 2u, 7u, 19u)
#define PPUTLUTRAITS_531u DEC,0b1000010011,9u,23u,(3u, 3u, 59u)
#define PPUTLUTRAITS_530u DEC,0b1000010010,9u,23u,(2u, 5u, 53u)
#define PPUTLUTRAITS_529u DEC,0b1000010001,9u,23u,(23u, 23u)
#define PPUTLUTRAITS_528u DEC,0b1000010000,9u,22u,(2u, 2u, 2u, 2u, 3u, 11u)
#define PPUTLUTRAITS_527u DEC,0b1000001111,9u,22u,(17u, 31u)
#define PPUTLUTRAITS_526u DEC,0b1000001110,9u,22u,(2u, 263u)
#define PPUTLUTRAITS_525u DEC,0b1000001101,9u,22u,(3u, 5u, 5u, 7u)
#define PPUTLUTRAITS_524u DEC,0b1000001100,9u,22u,(2u, 2u, 131u)
#define PPUTLUTRAITS_523u DEC,0b1000001011,9u,22u,()
#define PPUTLUTRAITS_522u DEC,0b1000001010,9u,22u,(2u, 3u, 3u, 29u)
#define PPUTLUTRAITS_521u DEC,0b1000001001,9u,22u,()
#define PPUTLUTRAITS_520u DEC,0b1000001000,9u,22u,(2u, 2u, 2u, 5u, 13u)
#define PPUTLUTRAITS_519u DEC,0b1000000111,9u,22u,(3u, 173u)
#define PPUTLUTRAITS_518u DEC,0b1000000110,9u,22u,(2u, 7u, 37u)
#define PPUTLUTRAITS_517u DEC,0b1000000101,9u,22u,(11u, 47u)
#define PPUTLUTRAITS_516u DEC,0b1000000100,9u,22u,(2u, 2u, 3u, 43u)
#define PPUTLUTRAITS_515u DEC,0b1000000011,9u,22u,(5u, 103u)
#define PPUTLUTRAITS_514u DEC,0b1000000010,9u,22u,(2u, 257u)
#define PPUTLUTRAITS_513u DEC,0b1000000001,9u,22u,(3u, 3u, 3u, 19u)
#define PPUTLUTRAITS_512u DEC,0b1000000000,9u,22u,(2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u)
#define PPUTLUTRAITS_511u DEC,0b0111111111,8u,22u,(7u, 73u)
#define PPUTLUTRAITS_510u DEC,0b0111111110,8u,22u,(2u, 3u, 5u, 17u)
#define PPUTLUTRAITS_509u DEC,0b0111111101,8u,22u,()
#define PPUTLUTRAITS_508u DEC,0b0111111100,8u,22u,(2u, 2u, 127u)
#define PPUTLUTRAITS_507u DEC,0b0111111011,8u,22u,(3u, 13u, 13u)
#define PPUTLUTRAITS_506u DEC,0b0111111010,8u,22u,(2u, 11u, 23u)
#define PPUTLUTRAITS_505u DEC,0b0111111001,8u,22u,(5u, 101u)
#define PPUTLUTRAITS_504u DEC,0b0111111000,8u,22u,(2u, 2u, 2u, 3u, 3u, 7u)
#define PPUTLUTRAITS_503u DEC,0b0111110111,8u,22u,()
#define PPUTLUTRAITS_502u DEC,0b0111110110,8u,22u,(2u, 251u)
#define PPUTLUTRAITS_501u DEC,0b0111110101,8u,22u,(3u, 167u)
#define PPUTLUTRAITS_500u DEC,0b0111110100,8u,22u,(2u, 2u, 5u, 5u, 5u)
#define PPUTLUTRAITS_499u DEC,0b0111110011,8u,22u,()
#define PPUTLUTRAITS_498u DEC,0b0111110010,8u,22u,(2u, 3u, 83u)
#define PPUTLUTRAITS_497u DEC,0b0111110001,8u,22u,(7u, 71u)
#define PPUTLUTRAITS_496u DEC,0b0111110000,8u,22u,(2u, 2u, 2u, 2u, 31u)
#define PPUTLUTRAITS_495u DEC,0b0111101111,8u,22u,(3u, 3u, 5u, 11u)
#define PPUTLUTRAITS_494u DEC,0b0111101110,8u,22u,(2u, 13u, 19u)
#define PPUTLUTRAITS_493u DEC,0b0111101101,8u,22u,(17u, 29u)
#define PPUTLUTRAITS_492u DEC,0b0111101100,8u,22u,(2u, 2u, 3u, 41u)
#define PPUTLUTRAITS_491u DEC,0b0111101011,8u,22u,()
#define PPUTLUTRAITS_490u DEC,0b0111101010,8u,22u,(2u, 5u, 7u, 7u)
#define PPUTLUTRAITS_489u DEC,0b0111101001,8u,22u,(3u, 163u)
#define PPUTLUTRAITS_488u DEC,0b0111101000,8u,22u,(2u, 2u, 2u, 61u)
#define PPUTLUTRAITS_487u DEC,0b0111100111,8u,22u,()
#define PPUTLUTRAITS_486u DEC,0b0111100110,8u,22u,(2u, 3u, 3u, 3u, 3u, 3u)
#define PPUTLUTRAITS_485u DEC,0b0111100101,8u,22u,(5u, 97u)
#define PPUTLUTRAITS_484u DEC,0b0111100100,8u,22u,(2u, 2u, 11u, 11u)
#define PPUTLUTRAITS_483u DEC,0b0111100011,8u,21u,(3u, 7u, 23u)
#define PPUTLUTRAITS_482u DEC,0b0111100010,8u,21u,(2u, 241u)
#define PPUTLUTRAITS_481u DEC,0b0111100001,8u,21u,(13u, 37u)
#define PPUTLUTRAITS_480u DEC,0b0111100000,8u,21u,(2u, 2u, 2u, 2u, 2u, 3u, 5u)
#define PPUTLUTRAITS_479u DEC,0b0111011111,8u,21u,()
#define PPUTLUTRAITS_478u DEC,0b0111011110,8u,21u,(2u, 239u)
#define PPUTLUTRAITS_477u DEC,0b0111011101,8u,21u,(3u, 3u, 53u)
#define PPUTLUTRAITS_476u DEC,0b0111011100,8u,21u,(2u, 2u, 7u, 17u)
#define PPUTLUTRAITS_475u DEC,0b0111011011,8u,21u,(5u, 5u, 19u)
#define PPUTLUTRAITS_474u DEC,0b0111011010,8u,21u,(2u, 3u, 79u)
#define PPUTLUTRAITS_473u DEC,0b0111011001,8u,21u,(11u, 43u)
#define PPUTLUTRAITS_472u DEC,0b0111011000,8u,21u,(2u, 2u, 2u, 59u)
#define PPUTLUTRAITS_471u DEC,0b0111010111,8u,21u,(3u, 157u)
#define PPUTLUTRAITS_470u DEC,0b0111010110,8u,21u,(2u, 5u, 47u)
#define PPUTLUTRAITS_469u DEC,0b0111010101,8u,21u,(7u, 67u)
#define PPUTLUTRAITS_468u DEC,0b0111010100,8u,21u,(2u, 2u, 3u, 3u, 13u)
#define PPUTLUTRAITS_467u DEC,0b0111010011,8u,21u,()
#define PPUTLUTRAITS_466u DEC,0b0111010010,8u,21u,(2u, 233u)
#define PPUTLUTRAITS_465u DEC,0b0111010001,8u,21u,(3u, 5u, 31u)
#define PPUTLUTRAITS_464u DEC,0b0111010000,8u,21u,(2u, 2u, 2u, 2u, 29u)
#define PPUTLUTRAITS_463u DEC,0b0111001111,8u,21u,()
#define PPUTLUTRAITS_462u DEC,0b0111001110,8u,21u,(2u, 3u, 7u, 11u)
#define PPUTLUTRAITS_461u DEC,0b0111001101,8u,21u,()
#define PPUTLUTRAITS_460u DEC,0b0111001100,8u,21u,(2u, 2u, 5u, 23u)
#define PPUTLUTRAITS_459u DEC,0b0111001011,8u,21u,(3u, 3u, 3u, 17u)
#define PPUTLUTRAITS_458u DEC,0b0111001010,8u,21u,(2u, 229u)
#define PPUTLUTRAITS_457u DEC,0b0111001001,8u,21u,()
#define PPUTLUTRAITS_456u DEC,0b0111001000,8u,21u,(2u, 2u, 2u, 3u, 19u)
#define PPUTLUTRAITS_455u DEC,0b0111000111,8u,21u,(5u, 7u, 13u)
#define PPUTLUTRAITS_454u DEC,0b0111000110,8u,21u,(2u, 227u)
#define PPUTLUTRAITS_453u DEC,0b0111000101,8u,21u,(3u, 151u)
#define PPUTLUTRAITS_452u DEC,0b0111000100,8u,21u,(2u, 2u, 113u)
#define PPUTLUTRAITS_451u DEC,0b0111000011,8u,21u,(11u, 41u)
#define PPUTLUTRAITS_450u DEC,0b0111000010,8u,21u,(2u, 3u, 3u, 5u, 5u)
#define PPUTLUTRAITS_449u DEC,0b0111000001,8u,21u,()
#define PPUTLUTRAITS_448u DEC,0b0111000000,8u,21u,(2u, 2u, 2u, 2u, 2u, 2u, 7u)
#define PPUTLUTRAITS_447u DEC,0b0110111111,8u,21u,(3u, 149u)
#define PPUTLUTRAITS_446u DEC,0b0110111110,8u,21u,(2u, 223u)
#define PPUTLUTRAITS_445u DEC,0b0110111101,8u,21u,(5u, 89u)
#define PPUTLUTRAITS_444u DEC,0b0110111100,8u,21u,(2u, 2u, 3u, 37u)
#define PPUTLUTRAITS_443u DEC,0b0110111011,8u,21u,()
#define PPUTLUTRAITS_442u DEC,0b0110111010,8u,21u,(2u, 13u, 17u)
#define PPUTLUTRAITS_441u DEC,0b0110111001,8u,21u,(3u, 3u, 7u, 7u)
#define PPUTLUTRAITS_440u DEC,0b0110111000,8u,20u,(2u, 2u, 2u, 5u, 11u)
#define PPUTLUTRAITS_439u DEC,0b0110110111,8u,20u,()
#define PPUTLUTRAITS_438u DEC,0b0110110110,8u,20u,(2u, 3u, 73u)
#define PPUTLUTRAITS_437u DEC,0b0110110101,8u,20u,(19u, 23u)
#define PPUTLUTRAITS_436u DEC,0b0110110100,8u,20u,(2u, 2u, 109u)
#define PPUTLUTRAITS_435u DEC,0b0110110011,8u,20u,(3u, 5u, 29u)
#define PPUTLUTRAITS_434u DEC,0b0110110010,8u,20u,(2u, 7u, 31u)
#define PPUTLUTRAITS_433u DEC,0b0110110001,8u,20u,()
#define PPUTLUTRAITS_432u DEC,0b0110110000,8u,20u,(2u, 2u, 2u, 2u, 3u, 3u, 3u)
#define PPUTLUTRAITS_431u DEC,0b0110101111,8u,20u,()
#define PPUTLUTRAITS_430u DEC,0b0110101110,8u,20u,(2u, 5u, 43u)
#define PPUTLUTRAITS_429u DEC,0b0110101101,8u,20u,(3u, 11u, 13u)
#define PPUTLUTRAITS_428u DEC,0b0110101100,8u,20u,(2u, 2u, 107u)
#define PPUTLUTRAITS_427u DEC,0b0110101011,8u,20u,(7u, 61u)
#define PPUTLUTRAITS_426u DEC,0b0110101010,8u,20u,(2u, 3u, 71u)
#define PPUTLUTRAITS_425u DEC,0b0110101001,8u,20u,(5u, 5u, 17u)
#define PPUTLUTRAITS_424u DEC,0b0110101000,8u,20u,(2u, 2u, 2u, 53u)
#define PPUTLUTRAITS_423u DEC,0b0110100111,8u,20u,(3u, 3u, 47u)
#define PPUTLUTRAITS_422u DEC,0b0110100110,8u,20u,(2u, 211u)
#define PPUTLUTRAITS_421u DEC,0b0110100101,8u,20u,()
#define PPUTLUTRAITS_420u DEC,0b0110100100,8u,20u,(2u, 2u, 3u, 5u, 7u)
#define PPUTLUTRAITS_419u DEC,0b0110100011,8u,20u,()
#define PPUTLUTRAITS_418u DEC,0b0110100010,8u,20u,(2u, 11u, 19u)
#define PPUTLUTRAITS_417u DEC,0b0110100001,8u,20u,(3u, 139u)
#define PPUTLUTRAITS_416u DEC,0b0110100000,8u,20u,(2u, 2u, 2u, 2u, 2u, 13u)
#define PPUTLUTRAITS_415u DEC,0b0110011111,8u,20u,(5u, 83u)
#define PPUTLUTRAITS_414u DEC,0b0110011110,8u,20u,(2u, 3u, 3u, 23u)
#define PPUTLUTRAITS_413u DEC,0b0110011101,8u,20u,(7u, 59u)
#define PPUTLUTRAITS_412u DEC,0b0110011100,8u,20u,(2u, 2u, 103u)
#define PPUTLUTRAITS_411u DEC,0b0110011011,8u,20u,(3u, 137u)
#define PPUTLUTRAITS_410u DEC,0b0110011010,8u,20u,(2u, 5u, 41u)
#define PPUTLUTRAITS_409u DEC,0b0110011001,8u,20u,()
#define PPUTLUTRAITS_408u DEC,0b0110011000,8u,20u,(2u, 2u, 2u, 3u, 17u)
#define PPUTLUTRAITS_407u DEC,0b0110010111,8u,20u,(11u, 37u)
#define PPUTLUTRAITS_406u DEC,0b0110010110,8u,20u,(2u, 7u, 29u)
#define PPUTLUTRAITS_405u DEC,0b0110010101,8u,20u,(3u, 3u, 3u, 3u, 5u)
#define PPUTLUTRAITS_404u DEC,0b0110010100,8u,20u,(2u, 2u, 101u)
#define PPUTLUTRAITS_403u DEC,0b0110010011,8u,20u,(13u, 31u)
#define PPUTLUTRAITS_402u DEC,0b0110010010,8u,20u,(2u, 3u, 67u)
#define PPUTLUTRAITS_401u DEC,0b0110010001,8u,20u,()
#define PPUTLUTRAITS_400u DEC,0b0110010000,8u,20u,(2u, 2u, 2u, 2u, 5u, 5u)
#define PPUTLUTRAITS_399u DEC,0b0110001111,8u,19u,(3u, 7u, 19u)
#define PPUTLUTRAITS_398u DEC,0b0110001110,8u,19u,(2u, 199u)
#define PPUTLUTRAITS_397u DEC,0b0110001101,8u,19u,()
#define PPUTLUTRAITS_396u DEC,0b0110001100,8u,19u,(2u, 2u, 3u, 3u, 11u)
#define PPUTLUTRAITS_395u DEC,0b0110001011,8u,19u,(5u, 79u)
#define PPUTLUTRAITS_394u DEC,0b0110001010,8u,19u,(2u, 197u)
#define PPUTLUTRAITS_393u DEC,0b0110001001,8u,19u,(3u, 131u)
#define PPUTLUTRAITS_392u DEC,0b0110001000,8u,19u,(2u, 2u, 2u, 7u, 7u)
#define PPUTLUTRAITS_391u DEC,0b0110000111,8u,19u,(17u, 23u)
#define PPUTLUTRAITS_390u DEC,0b0110000110,8u,19u,(2u, 3u, 5u, 13u)
#define PPUTLUTRAITS_389u DEC,0b0110000101,8u,19u,()
#define PPUTLUTRAITS_388u DEC,0b0110000100,8u,19u,(2u, 2u, 97u)
#define PPUTLUTRAITS_387u DEC,0b0110000011,8u,19u,(3u, 3u, 43u)
#define PPUTLUTRAITS_386u DEC,0b0110000010,8u,19u,(2u, 193u)
#define PPUTLUTRAITS_385u DEC,0b0110000001,8u,19u,(5u, 7u, 11u)
#define PPUTLUTRAITS_384u DEC,0b0110000000,8u,19u,(2u, 2u, 2u, 2u, 2u, 2u, 2u, 3u)
#define PPUTLUTRAITS_383u DEC,0b0101111111,8u,19u,()
#define PPUTLUTRAITS_382u DEC,0b0101111110,8u,19u,(2u, 191u)
#define PPUTLUTRAITS_381u DEC,0b0101111101,8u,19u,(3u, 127u)
#define PPUTLUTRAITS_380u DEC,0b0101111100,8u,19u,(2u, 2u, 5u, 19u)
#define PPUTLUTRAITS_379u DEC,0b0101111011,8u,19u,()
#define PPUTLUTRAITS_378u DEC,0b0101111010,8u,19u,(2u, 3u, 3u, 3u, 7u)
#define PPUTLUTRAITS_377u DEC,0b0101111001,8u,19u,(13u, 29u)
#define PPUTLUTRAITS_376u DEC,0b0101111000,8u,19u,(2u, 2u, 2u, 47u)
#define PPUTLUTRAITS_375u DEC,0b0101110111,8u,19u,(3u, 5u, 5u, 5u)
#define PPUTLUTRAITS_374u DEC,0b0101110110,8u,19u,(2u, 11u, 17u)
#define PPUTLUTRAITS_373u DEC,0b0101110101,8u,19u,()
#define PPUTLUTRAITS_372u DEC,0b0101110100,8u,19u,(2u, 2u, 3u, 31u)
#define PPUTLUTRAITS_371u DEC,0b0101110011,8u,19u,(7u, 53u)
#define PPUTLUTRAITS_370u DEC,0b0101110010,8u,19u,(2u, 5u, 37u)
#define PPUTLUTRAITS_369u DEC,0b0101110001,8u,19u,(3u, 3u, 41u)
#define PPUTLUTRAITS_368u DEC,0b0101110000,8u,19u,(2u, 2u, 2u, 2u, 23u)
#define PPUTLUTRAITS_367u DEC,0b0101101111,8u,19u,()
#define PPUTLUTRAITS_366u DEC,0b0101101110,8u,19u,(2u, 3u, 61u)
#define PPUTLUTRAITS_365u DEC,0b0101101101,8u,19u,(5u, 73u)
#define PPUTLUTRAITS_364u DEC,0b0101101100,8u,19u,(2u, 2u, 7u, 13u)
#define PPUTLUTRAITS_363u DEC,0b0101101011,8u,19u,(3u, 11u, 11u)
#define PPUTLUTRAITS_362u DEC,0b0101101010,8u,19u,(2u, 181u)
#define PPUTLUTRAITS_361u DEC,0b0101101001,8u,19u,(19u, 19u)
#define PPUTLUTRAITS_360u DEC,0b0101101000,8u,18u,(2u, 2u, 2u, 3u, 3u, 5u)
#define PPUTLUTRAITS_359u DEC,0b0101100111,8u,18u,()
#define PPUTLUTRAITS_358u DEC,0b0101100110,8u,18u,(2u, 179u)
#define PPUTLUTRAITS_357u DEC,0b0101100101,8u,18u,(3u, 7u, 17u)
#define PPUTLUTRAITS_356u DEC,0b0101100100,8u,18u,(2u, 2u, 89u)
#define PPUTLUTRAITS_355u DEC,0b0101100011,8u,18u,(5u, 71u)
#define PPUTLUTRAITS_354u DEC,0b0101100010,8u,18u,(2u, 3u, 59u)
#define PPUTLUTRAITS_353u DEC,0b0101100001,8u,18u,()
#define PPUTLUTRAITS_352u DEC,0b0101100000,8u,18u,(2u, 2u, 2u, 2u, 2u, 11u)
#define PPUTLUTRAITS_351u DEC,0b0101011111,8u,18u,(3u, 3u, 3u, 13u)
#define PPUTLUTRAITS_350u DEC,0b0101011110,8u,18u,(2u, 5u, 5u, 7u)
#define PPUTLUTRAITS_349u DEC,0b0101011101,8u,18u,()
#define PPUTLUTRAITS_348u DEC,0b0101011100,8u,18u,(2u, 2u, 3u, 29u)
#define PPUTLUTRAITS_347u DEC,0b0101011011,8u,18u,()
#define PPUTLUTRAITS_346u DEC,0b0101011010,8u,18u,(2u, 173u)
#define PPUTLUTRAITS_345u DEC,0b0101011001,8u,18u,(3u, 5u, 23u)
#define PPUTLUTRAITS_344u DEC,0b0101011000,8u,18u,(2u, 2u, 2u, 43u)
#define PPUTLUTRAITS_343u DEC,0b0101010111,8u,18u,(7u, 7u, 7u)
#define PPUTLUTRAITS_342u DEC,0b0101010110,8u,18u,(2u, 3u, 3u, 19u)
#define PPUTLUTRAITS_341u DEC,0b0101010101,8u,18u,(11u, 31u)
#define PPUTLUTRAITS_340u DEC,0b0101010100,8u,18u,(2u, 2u, 5u, 17u)
#define PPUTLUTRAITS_339u DEC,0b0101010011,8u,18u,(3u, 113u)
#define PPUTLUTRAITS_338u DEC,0b0101010010,8u,18u,(2u, 13u, 13u)
#define PPUTLUTRAITS_337u DEC,0b0101010001,8u,18u,()
#define PPUTLUTRAITS_336u DEC,0b0101010000,8u,18u,(2u, 2u, 2u, 2u, 3u, 7u)
#define PPUTLUTRAITS_335u DEC,0b0101001111,8u,18u,(5u, 67u)
#define PPUTLUTRAITS_334u DEC,0b0101001110,8u,18u,(2u, 167u)
#define PPUTLUTRAITS_333u DEC,0b0101001101,8u,18u,(3u, 3u, 37u)
#define PPUTLUTRAITS_332u DEC,0b0101001100,8u,18u,(2u, 2u, 83u)
#define PPUTLUTRAITS_331u DEC,0b0101001011,8u,18u,()
#define PPUTLUTRAITS_330u DEC,0b0101001010,8u,18u,(2u, 3u, 5u, 11u)
#define PPUTLUTRAITS_329u DEC,0b0101001001,8u,18u,(7u, 47u)
#define PPUTLUTRAITS_328u DEC,0b0101001000,8u,18u,(2u, 2u, 2u, 41u)
#define PPUTLUTRAITS_327u DEC,0b0101000111,8u,18u,(3u, 109u)
#define PPUTLUTRAITS_326u DEC,0b0101000110,8u,18u,(2u, 163u)
#define PPUTLUTRAITS_325u DEC,0b0101000101,8u,18u,(5u, 5u, 13u)
#define PPUTLUTRAITS_324u DEC,0b0101000100,8u,18u,(2u, 2u, 3u, 3u, 3u, 3u)
#define PPUTLUTRAITS_323u DEC,0b0101000011,8u,17u,(17u, 19u)
#define PPUTLUTRAITS_322u DEC,0b0101000010,8u,17u,(2u, 7u, 23u)
#define PPUTLUTRAITS_321u DEC,0b0101000001,8u,17u,(3u, 107u)
#define PPUTLUTRAITS_320u DEC,0b0101000000,8u,17u,(2u, 2u, 2u, 2u, 2u, 2u, 5u)
#define PPUTLUTRAITS_319u DEC,0b0100111111,8u,17u,(11u, 29u)
#define PPUTLUTRAITS_318u DEC,0b0100111110,8u,17u,(2u, 3u, 53u)
#define PPUTLUTRAITS_317u DEC,0b0100111101,8u,17u,()
#define PPUTLUTRAITS_316u DEC,0b0100111100,8u,17u,(2u, 2u, 79u)
#define PPUTLUTRAITS_315u DEC,0b0100111011,8u,17u,(3u, 3u, 5u, 7u)
#define PPUTLUTRAITS_314u DEC,0b0100111010,8u,17u,(2u, 157u)
#define PPUTLUTRAITS_313u DEC,0b0100111001,8u,17u,()
#define PPUTLUTRAITS_312u DEC,0b0100111000,8u,17u,(2u, 2u, 2u, 3u, 13u)
#define PPUTLUTRAITS_311u DEC,0b0100110111,8u,17u,()
#define PPUTLUTRAITS_310u DEC,0b0100110110,8u,17u,(2u, 5u, 31u)
#define PPUTLUTRAITS_309u DEC,0b0100110101,8u,17u,(3u, 103u)
#define PPUTLUTRAITS_308u DEC,0b0100110100,8u,17u,(2u, 2u, 7u, 11u)
#define PPUTLUTRAITS_307u DEC,0b0100110011,8u,17u,()
#define PPUTLUTRAITS_306u DEC,0b0100110010,8u,17u,(2u, 3u, 3u, 17u)
#define PPUTLUTRAITS_305u DEC,0b0100110001,8u,17u,(5u, 61u)
#define PPUTLUTRAITS_304u DEC,0b0100110000,8u,17u,(2u, 2u, 2u, 2u, 19u)
#define PPUTLUTRAITS_303u DEC,0b0100101111,8u,17u,(3u, 101u)
#define PPUTLUTRAITS_302u DEC,0b0100101110,8u,17u,(2u, 151u)
#define PPUTLUTRAITS_301u DEC,0b0100101101,8u,17u,(7u, 43u)
#define PPUTLUTRAITS_300u DEC,0b0100101100,8u,17u,(2u, 2u, 3u, 5u, 5u)
#define PPUTLUTRAITS_299u DEC,0b0100101011,8u,17u,(13u, 23u)
#define PPUTLUTRAITS_298u DEC,0b0100101010,8u,17u,(2u, 149u)
#define PPUTLUTRAITS_297u DEC,0b0100101001,8u,17u,(3u, 3u, 3u, 11u)
#define PPUTLUTRAITS_296u DEC,0b0100101000,8u,17u,(2u, 2u, 2u, 37u)
#define PPUTLUTRAITS_295u DEC,0b0100100111,8u,17u,(5u, 59u)
#define PPUTLUTRAITS_294u DEC,0b0100100110,8u,17u,(2u, 3u, 7u, 7u)
#define PPUTLUTRAITS_293u DEC,0b0100100101,8u,17u,()
#define PPUTLUTRAITS_292u DEC,0b0100100100,8u,17u,(2u, 2u, 73u)
#define PPUTLUTRAITS_291u DEC,0b0100100011,8u,17u,(3u, 97u)
#define PPUTLUTRAITS_290u DEC,0b0100100010,8u,17u,(2u, 5u, 29u)
#define PPUTLUTRAITS_289u DEC,0b0100100001,8u,17u,(17u, 17u)
#define PPUTLUTRAITS_288u DEC,0b0100100000,8u,16u,(2u, 2u, 2u, 2u, 2u, 3u, 3u)
#define PPUTLUTRAITS_287u DEC,0b0100011111,8u,16u,(7u, 41u)
#define PPUTLUTRAITS_286u DEC,0b0100011110,8u,16u,(2u, 11u, 13u)
#define PPUTLUTRAITS_285u DEC,0b0100011101,8u,16u,(3u, 5u, 19u)
#define PPUTLUTRAITS_284u DEC,0b0100011100,8u,16u,(2u, 2u, 71u)
#define PPUTLUTRAITS_283u DEC,0b0100011011,8u,16u,()
#define PPUTLUTRAITS_282u DEC,0b0100011010,8u,16u,(2u, 3u, 47u)
#define PPUTLUTRAITS_281u DEC,0b0100011001,8u,16u,()
#define PPUTLUTRAITS_280u DEC,0b0100011000,8u,16u,(2u, 2u, 2u, 5u, 7u)
#define PPUTLUTRAITS_279u DEC,0b0100010111,8u,16u,(3u, 3u, 31u)
#define PPUTLUTRAITS_278u DEC,0b0100010110,8u,16u,(2u, 139u)
#define PPUTLUTRAITS_277u DEC,0b0100010101,8u,16u,()
#define PPUTLUTRAITS_276u DEC,0b0100010100,8u,16u,(2u, 2u, 3u, 23u)
#define PPUTLUTRAITS_275u DEC,0b0100010011,8u,16u,(5u, 5u, 11u)
#define PPUTLUTRAITS_274u DEC,0b0100010010,8u,16u,(2u, 137u)
#define PPUTLUTRAITS_273u DEC,0b0100010001,8u,16u,(3u, 7u, 13u)
#define PPUTLUTRAITS_272u DEC,0b0100010000,8u,16u,(2u, 2u, 2u, 2u, 17u)
#define PPUTLUTRAITS_271u DEC,0b0100001111,8u,16u,()
#define PPUTLUTRAITS_270u DEC,0b0100001110,8u,16u,(2u, 3u, 3u, 3u, 5u)
#define PPUTLUTRAITS_269u DEC,0b0100001101,8u,16u,()
#define PPUTLUTRAITS_268u DEC,0b0100001100,8u,16u,(2u, 2u, 67u)
#define PPUTLUTRAITS_267u DEC,0b0100001011,8u,16u,(3u, 89u)
#define PPUTLUTRAITS_266u DEC,0b0100001010,8u,16u,(2u, 7u, 19u)
#define PPUTLUTRAITS_265u DEC,0b0100001001,8u,16u,(5u, 53u)
#define PPUTLUTRAITS_264u DEC,0b0100001000,8u,16u,(2u, 2u, 2u, 3u, 11u)
#define PPUTLUTRAITS_263u DEC,0b0100000111,8u,16u,()
#define PPUTLUTRAITS_262u DEC,0b0100000110,8u,16u,(2u, 131u)
#define PPUTLUTRAITS_261u DEC,0b0100000101,8u,16u,(3u, 3u, 29u)
#define PPUTLUTRAITS_260u DEC,0b0100000100,8u,16u,(2u, 2u, 5u, 13u)
#define PPUTLUTRAITS_259u DEC,0b0100000011,8u,16u,(7u, 37u)
#define PPUTLUTRAITS_258u DEC,0b0100000010,8u,16u,(2u, 3u, 43u)
#define PPUTLUTRAITS_257u DEC,0b0100000001,8u,16u,()
#define PPUTLUTRAITS_256u DEC,0b0100000000,8u,16u,(2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u)
#define PPUTLUTRAITS_255u DEC,0b0011111111,7u,15u,(3u, 5u, 17u)
#define PPUTLUTRAITS_254u DEC,0b0011111110,7u,15u,(2u, 127u)
#define PPUTLUTRAITS_253u DEC,0b0011111101,7u,15u,(11u, 23u)
#define PPUTLUTRAITS_252u DEC,0b0011111100,7u,15u,(2u, 2u, 3u, 3u, 7u)
#define PPUTLUTRAITS_251u DEC,0b0011111011,7u,15u,()
#define PPUTLUTRAITS_250u DEC,0b0011111010,7u,15u,(2u, 5u, 5u, 5u)
#define PPUTLUTRAITS_249u DEC,0b0011111001,7u,15u,(3u, 83u)
#define PPUTLUTRAITS_248u DEC,0b0011111000,7u,15u,(2u, 2u, 2u, 31u)
#define PPUTLUTRAITS_247u DEC,0b0011110111,7u,15u,(13u, 19u)
#define PPUTLUTRAITS_246u DEC,0b0011110110,7u,15u,(2u, 3u, 41u)
#define PPUTLUTRAITS_245u DEC,0b0011110101,7u,15u,(5u, 7u, 7u)
#define PPUTLUTRAITS_244u DEC,0b0011110100,7u,15u,(2u, 2u, 61u)
#define PPUTLUTRAITS_243u DEC,0b0011110011,7u,15u,(3u, 3u, 3u, 3u, 3u)
#define PPUTLUTRAITS_242u DEC,0b0011110010,7u,15u,(2u, 11u, 11u)
#define PPUTLUTRAITS_241u DEC,0b0011110001,7u,15u,()
#define PPUTLUTRAITS_240u DEC,0b0011110000,7u,15u,(2u, 2u, 2u, 2u, 3u, 5u)
#define PPUTLUTRAITS_239u DEC,0b0011101111,7u,15u,()
#define PPUTLUTRAITS_238u DEC,0b0011101110,7u,15u,(2u, 7u, 17u)
#define PPUTLUTRAITS_237u DEC,0b0011101101,7u,15u,(3u, 79u)
#define PPUTLUTRAITS_236u DEC,0b0011101100,7u,15u,(2u, 2u, 59u)
#define PPUTLUTRAITS_235u DEC,0b0011101011,7u,15u,(5u, 47u)
#define PPUTLUTRAITS_234u DEC,0b0011101010,7u,15u,(2u, 3u, 3u, 13u)
#define PPUTLUTRAITS_233u DEC,0b0011101001,7u,15u,()
#define PPUTLUTRAITS_232u DEC,0b0011101000,7u,15u,(2u, 2u, 2u, 29u)
#define PPUTLUTRAITS_231u DEC,0b0011100111,7u,15u,(3u, 7u, 11u)
#define PPUTLUTRAITS_230u DEC,0b0011100110,7u,15u,(2u, 5u, 23u)
#define PPUTLUTRAITS_229u DEC,0b0011100101,7u,15u,()
#define PPUTLUTRAITS_228u DEC,0b0011100100,7u,15u,(2u, 2u, 3u, 19u)
#define PPUTLUTRAITS_227u DEC,0b0011100011,7u,15u,()
#define PPUTLUTRAITS_226u DEC,0b0011100010,7u,15u,(2u, 113u)
#define PPUTLUTRAITS_225u DEC,0b0011100001,7u,15u,(3u, 3u, 5u, 5u)
#define PPUTLUTRAITS_224u DEC,0b0011100000,7u,14u,(2u, 2u, 2u, 2u, 2u, 7u)
#define PPUTLUTRAITS_223u DEC,0b0011011111,7u,14u,()
#define PPUTLUTRAITS_222u DEC,0b0011011110,7u,14u,(2u, 3u, 37u)
#define PPUTLUTRAITS_221u DEC,0b0011011101,7u,14u,(13u, 17u)
#define PPUTLUTRAITS_220u DEC,0b0011011100,7u,14u,(2u, 2u, 5u, 11u)
#define PPUTLUTRAITS_219u DEC,0b0011011011,7u,14u,(3u, 73u)
#define PPUTLUTRAITS_218u DEC,0b0011011010,7u,14u,(2u, 109u)
#define PPUTLUTRAITS_217u DEC,0b0011011001,7u,14u,(7u, 31u)
#define PPUTLUTRAITS_216u DEC,0b0011011000,7u,14u,(2u, 2u, 2u, 3u, 3u, 3u)
#define PPUTLUTRAITS_215u DEC,0b0011010111,7u,14u,(5u, 43u)
#define PPUTLUTRAITS_214u DEC,0b0011010110,7u,14u,(2u, 107u)
#define PPUTLUTRAITS_213u DEC,0b0011010101,7u,14u,(3u, 71u)
#define PPUTLUTRAITS_212u DEC,0b0011010100,7u,14u,(2u, 2u, 53u)
#define PPUTLUTRAITS_211u DEC,0b0011010011,7u,14u,()
#define PPUTLUTRAITS_210u DEC,0b0011010010,7u,14u,(2u, 3u, 5u, 7u)
#define PPUTLUTRAITS_209u DEC,0b0011010001,7u,14u,(11u, 19u)
#define PPUTLUTRAITS_208u DEC,0b0011010000,7u,14u,(2u, 2u, 2u, 2u, 13u)
#define PPUTLUTRAITS_207u DEC,0b0011001111,7u,14u,(3u, 3u, 23u)
#define PPUTLUTRAITS_206u DEC,0b0011001110,7u,14u,(2u, 103u)
#define PPUTLUTRAITS_205u DEC,0b0011001101,7u,14u,(5u, 41u)
#define PPUTLUTRAITS_204u DEC,0b0011001100,7u,14u,(2u, 2u, 3u, 17u)
#define PPUTLUTRAITS_203u DEC,0b0011001011,7u,14u,(7u, 29u)
#define PPUTLUTRAITS_202u DEC,0b0011001010,7u,14u,(2u, 101u)
#define PPUTLUTRAITS_201u DEC,0b0011001001,7u,14u,(3u, 67u)
#define PPUTLUTRAITS_200u DEC,0b0011001000,7u,14u,(2u, 2u, 2u, 5u, 5u)
#define PPUTLUTRAITS_199u DEC,0b0011000111,7u,14u,()
#define PPUTLUTRAITS_198u DEC,0b0011000110,7u,14u,(2u, 3u, 3u, 11u)
#define PPUTLUTRAITS_197u DEC,0b0011000101,7u,14u,()
#define PPUTLUTRAITS_196u DEC,0b0011000100,7u,14u,(2u, 2u, 7u, 7u)
#define PPUTLUTRAITS_195u DEC,0b0011000011,7u,13u,(3u, 5u, 13u)
#define PPUTLUTRAITS_194u DEC,0b0011000010,7u,13u,(2u, 97u)
#define PPUTLUTRAITS_193u DEC,0b0011000001,7u,13u,()
#define PPUTLUTRAITS_192u DEC,0b0011000000,7u,13u,(2u, 2u, 2u, 2u, 2u, 2u, 3u)
#define PPUTLUTRAITS_191u DEC,0b0010111111,7u,13u,()
#define PPUTLUTRAITS_190u DEC,0b0010111110,7u,13u,(2u, 5u, 19u)
#define PPUTLUTRAITS_189u DEC,0b0010111101,7u,13u,(3u, 3u, 3u, 7u)
#define PPUTLUTRAITS_188u DEC,0b0010111100,7u,13u,(2u, 2u, 47u)
#define PPUTLUTRAITS_187u DEC,0b0010111011,7u,13u,(11u, 17u)
#define PPUTLUTRAITS_186u DEC,0b0010111010,7u,13u,(2u, 3u, 31u)
#define PPUTLUTRAITS_185u DEC,0b0010111001,7u,13u,(5u, 37u)
#define PPUTLUTRAITS_184u DEC,0b0010111000,7u,13u,(2u, 2u, 2u, 23u)
#define PPUTLUTRAITS_183u DEC,0b0010110111,7u,13u,(3u, 61u)
#define PPUTLUTRAITS_182u DEC,0b0010110110,7u,13u,(2u, 7u, 13u)
#define PPUTLUTRAITS_181u DEC,0b0010110101,7u,13u,()
#define PPUTLUTRAITS_180u DEC,0b0010110100,7u,13u,(2u, 2u, 3u, 3u, 5u)
#define PPUTLUTRAITS_179u DEC,0b0010110011,7u,13u,()
#define PPUTLUTRAITS_178u DEC,0b0010110010,7u,13u,(2u, 89u)
#define PPUTLUTRAITS_177u DEC,0b0010110001,7u,13u,(3u, 59u)
#define PPUTLUTRAITS_176u DEC,0b0010110000,7u,13u,(2u, 2u, 2u, 2u, 11u)
#define PPUTLUTRAITS_175u DEC,0b0010101111,7u,13u,(5u, 5u, 7u)
#define PPUTLUTRAITS_174u DEC,0b0010101110,7u,13u,(2u, 3u, 29u)
#define PPUTLUTRAITS_173u DEC,0b0010101101,7u,13u,()
#define PPUTLUTRAITS_172u DEC,0b0010101100,7u,13u,(2u, 2u, 43u)
#define PPUTLUTRAITS_171u DEC,0b0010101011,7u,13u,(3u, 3u, 19u)
#define PPUTLUTRAITS_170u DEC,0b0010101010,7u,13u,(2u, 5u, 17u)
#define PPUTLUTRAITS_169u DEC,0b0010101001,7u,13u,(13u, 13u)
#define PPUTLUTRAITS_168u DEC,0b0010101000,7u,12u,(2u, 2u, 2u, 3u, 7u)
#define PPUTLUTRAITS_167u DEC,0b0010100111,7u,12u,()
#define PPUTLUTRAITS_166u DEC,0b0010100110,7u,12u,(2u, 83u)
#define PPUTLUTRAITS_165u DEC,0b0010100101,7u,12u,(3u, 5u, 11u)
#define PPUTLUTRAITS_164u DEC,0b0010100100,7u,12u,(2u, 2u, 41u)
#define PPUTLUTRAITS_163u DEC,0b0010100011,7u,12u,()
#define PPUTLUTRAITS_162u DEC,0b0010100010,7u,12u,(2u, 3u, 3u, 3u, 3u)
#define PPUTLUTRAITS_161u DEC,0b0010100001,7u,12u,(7u, 23u)
#define PPUTLUTRAITS_160u DEC,0b0010100000,7u,12u,(2u, 2u, 2u, 2u, 2u, 5u)
#define PPUTLUTRAITS_159u DEC,0b0010011111,7u,12u,(3u, 53u)
#define PPUTLUTRAITS_158u DEC,0b0010011110,7u,12u,(2u, 79u)
#define PPUTLUTRAITS_157u DEC,0b0010011101,7u,12u,()
#define PPUTLUTRAITS_156u DEC,0b0010011100,7u,12u,(2u, 2u, 3u, 13u)
#define PPUTLUTRAITS_155u DEC,0b0010011011,7u,12u,(5u, 31u)
#define PPUTLUTRAITS_154u DEC,0b0010011010,7u,12u,(2u, 7u, 11u)
#define PPUTLUTRAITS_153u DEC,0b0010011001,7u,12u,(3u, 3u, 17u)
#define PPUTLUTRAITS_152u DEC,0b0010011000,7u,12u,(2u, 2u, 2u, 19u)
#define PPUTLUTRAITS_151u DEC,0b0010010111,7u,12u,()
#define PPUTLUTRAITS_150u DEC,0b0010010110,7u,12u,(2u, 3u, 5u, 5u)
#define PPUTLUTRAITS_149u DEC,0b0010010101,7u,12u,()
#define PPUTLUTRAITS_148u DEC,0b0010010100,7u,12u,(2u, 2u, 37u)
#define PPUTLUTRAITS_147u DEC,0b0010010011,7u,12u,(3u, 7u, 7u)
#define PPUTLUTRAITS_146u DEC,0b0010010010,7u,12u,(2u, 73u)
#define PPUTLUTRAITS_145u DEC,0b0010010001,7u,12u,(5u, 29u)
#define PPUTLUTRAITS_144u DEC,0b0010010000,7u,12u,(2u, 2u, 2u, 2u, 3u, 3u)
#define PPUTLUTRAITS_143u DEC,0b0010001111,7u,11u,(11u, 13u)
#define PPUTLUTRAITS_142u DEC,0b0010001110,7u,11u,(2u, 71u)
#define PPUTLUTRAITS_141u DEC,0b0010001101,7u,11u,(3u, 47u)
#define PPUTLUTRAITS_140u DEC,0b0010001100,7u,11u,(2u, 2u, 5u, 7u)
#define PPUTLUTRAITS_139u DEC,0b0010001011,7u,11u,()
#define PPUTLUTRAITS_138u DEC,0b0010001010,7u,11u,(2u, 3u, 23u)
#define PPUTLUTRAITS_137u DEC,0b0010001001,7u,11u,()
#define PPUTLUTRAITS_136u DEC,0b0010001000,7u,11u,(2u, 2u, 2u, 17u)
#define PPUTLUTRAITS_135u DEC,0b0010000111,7u,11u,(3u, 3u, 3u, 5u)
#define PPUTLUTRAITS_134u DEC,0b0010000110,7u,11u,(2u, 67u)
#define PPUTLUTRAITS_133u DEC,0b0010000101,7u,11u,(7u, 19u)
#define PPUTLUTRAITS_132u DEC,0b0010000100,7u,11u,(2u, 2u, 3u, 11u)
#define PPUTLUTRAITS_131u DEC,0b0010000011,7u,11u,()
#define PPUTLUTRAITS_130u DEC,0b0010000010,7u,11u,(2u, 5u, 13u)
#define PPUTLUTRAITS_129u DEC,0b0010000001,7u,11u,(3u, 43u)
#define PPUTLUTRAITS_128u DEC,0b0010000000,7u,11u,(2u, 2u, 2u, 2u, 2u, 2u, 2u)
#define PPUTLUTRAITS_127u DEC,0b0001111111,6u,11u,()
#define PPUTLUTRAITS_126u DEC,0b0001111110,6u,11u,(2u, 3u, 3u, 7u)
#define PPUTLUTRAITS_125u DEC,0b0001111101,6u,11u,(5u, 5u, 5u)
#define PPUTLUTRAITS_124u DEC,0b0001111100,6u,11u,(2u, 2u, 31u)
#define PPUTLUTRAITS_123u DEC,0b0001111011,6u,11u,(3u, 41u)
#define PPUTLUTRAITS_122u DEC,0b0001111010,6u,11u,(2u, 61u)
#define PPUTLUTRAITS_121u DEC,0b0001111001,6u,11u,(11u, 11u)
#define PPUTLUTRAITS_120u DEC,0b0001111000,6u,10u,(2u, 2u, 2u, 3u, 5u)
#define PPUTLUTRAITS_119u DEC,0b0001110111,6u,10u,(7u, 17u)
#define PPUTLUTRAITS_118u DEC,0b0001110110,6u,10u,(2u, 59u)
#define PPUTLUTRAITS_117u DEC,0b0001110101,6u,10u,(3u, 3u, 13u)
#define PPUTLUTRAITS_116u DEC,0b0001110100,6u,10u,(2u, 2u, 29u)
#define PPUTLUTRAITS_115u DEC,0b0001110011,6u,10u,(5u, 23u)
#define PPUTLUTRAITS_114u DEC,0b0001110010,6u,10u,(2u, 3u, 19u)
#define PPUTLUTRAITS_113u DEC,0b0001110001,6u,10u,()
#define PPUTLUTRAITS_112u DEC,0b0001110000,6u,10u,(2u, 2u, 2u, 2u, 7u)
#define PPUTLUTRAITS_111u DEC,0b0001101111,6u,10u,(3u, 37u)
#define PPUTLUTRAITS_110u DEC,0b0001101110,6u,10u,(2u, 5u, 11u)
#define PPUTLUTRAITS_109u DEC,0b0001101101,6u,10u,()
#define PPUTLUTRAITS_108u DEC,0b0001101100,6u,10u,(2u, 2u, 3u, 3u, 3u)
#define PPUTLUTRAITS_107u DEC,0b0001101011,6u,10u,()
#define PPUTLUTRAITS_106u DEC,0b0001101010,6u,10u,(2u, 53u)
#define PPUTLUTRAITS_105u DEC,0b0001101001,6u,10u,(3u, 5u, 7u)
#define PPUTLUTRAITS_104u DEC,0b0001101000,6u,10u,(2u, 2u, 2u, 13u)
#define PPUTLUTRAITS_103u DEC,0b0001100111,6u,10u,()
#define PPUTLUTRAITS_102u DEC,0b0001100110,6u,10u,(2u, 3u, 17u)
#define PPUTLUTRAITS_101u DEC,0b0001100101,6u,10u,()
#define PPUTLUTRAITS_100u DEC,0b0001100100,6u,10u,(2u, 2u, 5u, 5u)
#define PPUTLUTRAITS_99u DEC,0b0001100011,6u,9u,(3u, 3u, 11u)
#define PPUTLUTRAITS_98u DEC,0b0001100010,6u,9u,(2u, 7u, 7u)
#define PPUTLUTRAITS_97u DEC,0b0001100001,6u,9u,()
#define PPUTLUTRAITS_96u DEC,0b0001100000,6u,9u,(2u, 2u, 2u, 2u, 2u, 3u)
#define PPUTLUTRAITS_95u DEC,0b0001011111,6u,9u,(5u, 19u)
#define PPUTLUTRAITS_94u DEC,0b0001011110,6u,9u,(2u, 47u)
#define PPUTLUTRAITS_93u DEC,0b0001011101,6u,9u,(3u, 31u)
#define PPUTLUTRAITS_92u DEC,0b0001011100,6u,9u,(2u, 2u, 23u)
#define PPUTLUTRAITS_91u DEC,0b0001011011,6u,9u,(7u, 13u)
#define PPUTLUTRAITS_90u DEC,0b0001011010,6u,9u,(2u, 3u, 3u, 5u)
#define PPUTLUTRAITS_89u DEC,0b0001011001,6u,9u,()
#define PPUTLUTRAITS_88u DEC,0b0001011000,6u,9u,(2u, 2u, 2u, 11u)
#define PPUTLUTRAITS_87u DEC,0b0001010111,6u,9u,(3u, 29u)
#define PPUTLUTRAITS_86u DEC,0b0001010110,6u,9u,(2u, 43u)
#define PPUTLUTRAITS_85u DEC,0b0001010101,6u,9u,(5u, 17u)
#define PPUTLUTRAITS_84u DEC,0b0001010100,6u,9u,(2u, 2u, 3u, 7u)
#define PPUTLUTRAITS_83u DEC,0b0001010011,6u,9u,()
#define PPUTLUTRAITS_82u DEC,0b0001010010,6u,9u,(2u, 41u)
#define PPUTLUTRAITS_81u DEC,0b0001010001,6u,9u,(3u, 3u, 3u, 3u)
#define PPUTLUTRAITS_80u DEC,0b0001010000,6u,8u,(2u, 2u, 2u, 2u, 5u)
#define PPUTLUTRAITS_79u DEC,0b0001001111,6u,8u,()
#define PPUTLUTRAITS_78u DEC,0b0001001110,6u,8u,(2u, 3u, 13u)
#define PPUTLUTRAITS_77u DEC,0b0001001101,6u,8u,(7u, 11u)
#define PPUTLUTRAITS_76u DEC,0b0001001100,6u,8u,(2u, 2u, 19u)
#define PPUTLUTRAITS_75u DEC,0b0001001011,6u,8u,(3u, 5u, 5u)
#define PPUTLUTRAITS_74u DEC,0b0001001010,6u,8u,(2u, 37u)
#define PPUTLUTRAITS_73u DEC,0b0001001001,6u,8u,()
#define PPUTLUTRAITS_72u DEC,0b0001001000,6u,8u,(2u, 2u, 2u, 3u, 3u)
#define PPUTLUTRAITS_71u DEC,0b0001000111,6u,8u,()
#define PPUTLUTRAITS_70u DEC,0b0001000110,6u,8u,(2u, 5u, 7u)
#define PPUTLUTRAITS_69u DEC,0b0001000101,6u,8u,(3u, 23u)
#define PPUTLUTRAITS_68u DEC,0b0001000100,6u,8u,(2u, 2u, 17u)
#define PPUTLUTRAITS_67u DEC,0b0001000011,6u,8u,()
#define PPUTLUTRAITS_66u DEC,0b0001000010,6u,8u,(2u, 3u, 11u)
#define PPUTLUTRAITS_65u DEC,0b0001000001,6u,8u,(5u, 13u)
#define PPUTLUTRAITS_64u DEC,0b0001000000,6u,8u,(2u, 2u, 2u, 2u, 2u, 2u)
#define PPUTLUTRAITS_63u DEC,0b0000111111,5u,7u,(3u, 3u, 7u)
#define PPUTLUTRAITS_62u DEC,0b0000111110,5u,7u,(2u, 31u)
#define PPUTLUTRAITS_61u DEC,0b0000111101,5u,7u,()
#define PPUTLUTRAITS_60u DEC,0b0000111100,5u,7u,(2u, 2u, 3u, 5u)
#define PPUTLUTRAITS_59u DEC,0b0000111011,5u,7u,()
#define PPUTLUTRAITS_58u DEC,0b0000111010,5u,7u,(2u, 29u)
#define PPUTLUTRAITS_57u DEC,0b0000111001,5u,7u,(3u, 19u)
#define PPUTLUTRAITS_56u DEC,0b0000111000,5u,7u,(2u, 2u, 2u, 7u)
#define PPUTLUTRAITS_55u DEC,0b0000110111,5u,7u,(5u, 11u)
#define PPUTLUTRAITS_54u DEC,0b0000110110,5u,7u,(2u, 3u, 3u, 3u)
#define PPUTLUTRAITS_53u DEC,0b0000110101,5u,7u,()
#define PPUTLUTRAITS_52u DEC,0b0000110100,5u,7u,(2u, 2u, 13u)
#define PPUTLUTRAITS_51u DEC,0b0000110011,5u,7u,(3u, 17u)
#define PPUTLUTRAITS_50u DEC,0b0000110010,5u,7u,(2u, 5u, 5u)
#define PPUTLUTRAITS_49u DEC,0b0000110001,5u,7u,(7u, 7u)
#define PPUTLUTRAITS_48u DEC,0b0000110000,5u,6u,(2u, 2u, 2u, 2u, 3u)
#define PPUTLUTRAITS_47u DEC,0b0000101111,5u,6u,()
#define PPUTLUTRAITS_46u DEC,0b0000101110,5u,6u,(2u, 23u)
#define PPUTLUTRAITS_45u DEC,0b0000101101,5u,6u,(3u, 3u, 5u)
#define PPUTLUTRAITS_44u DEC,0b0000101100,5u,6u,(2u, 2u, 11u)
#define PPUTLUTRAITS_43u DEC,0b0000101011,5u,6u,()
#define PPUTLUTRAITS_42u DEC,0b0000101010,5u,6u,(2u, 3u, 7u)
#define PPUTLUTRAITS_41u DEC,0b0000101001,5u,6u,()
#define PPUTLUTRAITS_40u DEC,0b0000101000,5u,6u,(2u, 2u, 2u, 5u)
#define PPUTLUTRAITS_39u DEC,0b0000100111,5u,6u,(3u, 13u)
#define PPUTLUTRAITS_38u DEC,0b0000100110,5u,6u,(2u, 19u)
#define PPUTLUTRAITS_37u DEC,0b0000100101,5u,6u,()
#define PPUTLUTRAITS_36u DEC,0b0000100100,5u,6u,(2u, 2u, 3u, 3u)
#define PPUTLUTRAITS_35u DEC,0b0000100011,5u,5u,(5u, 7u)
#define PPUTLUTRAITS_34u DEC,0b0000100010,5u,5u,(2u, 17u)
#define PPUTLUTRAITS_33u DEC,0b0000100001,5u,5u,(3u, 11u)
#define PPUTLUTRAITS_32u DEC,0b0000100000,5u,5u,(2u, 2u, 2u, 2u, 2u)
#define PPUTLUTRAITS_31u DEC,0b0000011111,4u,5u,()
#define PPUTLUTRAITS_30u DEC,0b0000011110,4u,5u,(2u, 3u, 5u)
#define PPUTLUTRAITS_29u DEC,0b0000011101,4u,5u,()
#define PPUTLUTRAITS_28u DEC,0b0000011100,4u,5u,(2u, 2u, 7u)
#define PPUTLUTRAITS_27u DEC,0b0000011011,4u,5u,(3u, 3u, 3u)
#define PPUTLUTRAITS_26u DEC,0b0000011010,4u,5u,(2u, 13u)
#define PPUTLUTRAITS_25u DEC,0b0000011001,4u,5u,(5u, 5u)
#define PPUTLUTRAITS_24u DEC,0b0000011000,4u,4u,(2u, 2u, 2u, 3u)
#define PPUTLUTRAITS_23u DEC,0b0000010111,4u,4u,()
#define PPUTLUTRAITS_22u DEC,0b0000010110,4u,4u,(2u, 11u)
#define PPUTLUTRAITS_21u DEC,0b0000010101,4u,4u,(3u, 7u)
#define PPUTLUTRAITS_20u DEC,0b0000010100,4u,4u,(2u, 2u, 5u)
#define PPUTLUTRAITS_19u DEC,0b0000010011,4u,4u,()
#define PPUTLUTRAITS_18u DEC,0b0000010010,4u,4u,(2u, 3u, 3u)
#define PPUTLUTRAITS_17u DEC,0b0000010001,4u,4u,()
#define PPUTLUTRAITS_16u DEC,0b0000010000,4u,4u,(2u, 2u, 2u, 2u)
#define PPUTLUTRAITS_15u DEC,0b0000001111,3u,3u,(3u, 5u)
#define PPUTLUTRAITS_14u DEC,0b0000001110,3u,3u,(2u, 7u)
#define PPUTLUTRAITS_13u DEC,0b0000001101,3u,3u,()
#define PPUTLUTRAITS_12u DEC,0b0000001100,3u,3u,(2u, 2u, 3u)
#define PPUTLUTRAITS_11u DEC,0b0000001011,3u,3u,()
#define PPUTLUTRAITS_10u DEC,0b0000001010,3u,3u,(2u, 5u)
#define PPUTLUTRAITS_9u DEC,0b0000001001,3u,3u,(3u, 3u)
#define PPUTLUTRAITS_8u DEC,0b0000001000,3u,2u,(2u, 2u, 2u)
#define PPUTLUTRAITS_7u DEC,0b0000000111,2u,2u,()
#define PPUTLUTRAITS_6u DEC,0b0000000110,2u,2u,(2u, 3u)
#define PPUTLUTRAITS_5u DEC,0b0000000101,2u,2u,()
#define PPUTLUTRAITS_4u DEC,0b0000000100,2u,2u,(2u, 2u)
#define PPUTLUTRAITS_3u DEC,0b0000000011,1u,1u,()
#define PPUTLUTRAITS_2u DEC,0b0000000010,1u,1u,()
#define PPUTLUTRAITS_1u DEC,0b0000000001,0u,1u,()
#define PPUTLUTRAITS_0u DEC,0b0000000000,,0u,()


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

// clang-format on

/// [type.int]
/// ----------
/// 10-bit signed integer type.
/// may be constructed from either unsigned or signed ints.
/// cannot parse negative decimals; use math.neg instead.
///
/// bit length is fixed. cannot parse shorter bit lengths.
///
/// attempts to preserve binary/decimal representation, but will
/// output binary if casting the input yields a negative number
///
/// cast from unsigned reinterprets bits as signed two's complement.
///
/// decimals larger than the int maximum that lack a 'u' suffix
/// are interpreted as uints and converted to binary.
///
/// PTL_INT(0)             // 0
/// PTL_INT(1u)            // 1
/// PTL_INT(0b0000000000)  // 0b0000000000
/// PTL_INT(511)           // 511
/// PTL_INT(0b0111111111u) // 0b0111111111
/// PTL_INT(1023u)         // 0b1111111111
/// PTL_INT(1023)          // 0b1111111111
#define PTL_INT(/* n: uint|int */...) /* -> int{n} */                                            \
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

/// third parentheses; checks for validity with added 'u' suffix.
#define PPUTLINT_ooo(...)          PPUTLINT_ooo_RES(PPUTLUTRAITS_##__VA_ARGS__##u)
#define PPUTLINT_ooo_RES(...)      PPUTLINT_ooo_RES_o(__VA_ARGS__)
#define PPUTLINT_ooo_RES_o(_, ...) PPUTLINT_ooo_##__VA_OPT__(NO_)##FAIL()
#define PPUTLINT_ooo_NO_FAIL(...)  PPUTLINT_oooo_IPASS
#define PPUTLINT_ooo_FAIL(...)     PPUTLINT_oooo

/// fourth parentheses; checks for validity without added 'u' suffix (cast from uint).
#define PPUTLINT_oooo(...)          PPUTLINT_oooo_RES(PPUTLUTRAITS_##__VA_ARGS__)
#define PPUTLINT_oooo_RES(...)      PPUTLINT_oooo_RES_o(__VA_ARGS__)
#define PPUTLINT_oooo_RES_o(_, ...) PPUTLINT_oooo_##__VA_OPT__(NO_)##FAIL
#define PPUTLINT_oooo_IPASS(...)    PPUTLINT_IPASS
#define PPUTLINT_oooo_NO_FAIL       PPUTLINT_UPASS
#define PPUTLINT_oooo_FAIL          PPUTLINT_FAIL

/// final parentheses (cast from integer or implicit unsigned)
#define PPUTLINT_IPASS(e, v)           PPUTLINT_IPASS_o(v, v##u)
#define PPUTLINT_IPASS_o(...)          PPUTLINT_IPASS_oo(__VA_ARGS__)
#define PPUTLINT_IPASS_oo(v, u)        PTL_CAT(PPUTLINT_IPASS_, PPUTLUINT_TRAIT(u, TYPE))(v, u)
#define PPUTLINT_IPASS_BIN(v, u)       v
#define PPUTLINT_IPASS_DEC(v, u)       PPUTLINT_IPASS_DEC_o(v, PPUTLUINT_TRAIT(u, DEC_IBIN))
#define PPUTLINT_IPASS_DEC_o(v, ibin)  PPUTLINT_IPASS_DEC_oo(v, ibin)
#define PPUTLINT_IPASS_DEC_oo(v, ibin) PPUTLINT_IPASS_DEC_ooo(v, ibin, ibin##u)
#define PPUTLINT_IPASS_DEC_ooo(v, ibin, ubin) \
  PTL_CAT(PPUTLINT_IPASS_DEC_, PTL_ESC(PPUTLINT_MSB PPUTLUINT_TRAIT(ubin, BIN_BITS)))(v, ibin)
#define PPUTLINT_IPASS_DEC_1(v, ibin) ibin
#define PPUTLINT_IPASS_DEC_0(v, ibin) v

/// final parentheses (cast from explicit unsigned)
#define PPUTLINT_UPASS(e, v)           PTL_CAT(PPUTLINT_UPASS_, PPUTLUINT_TRAIT(v, TYPE))(v)
#define PPUTLINT_UPASS_BIN(v)          PPUTLUINT_TRAIT(PPUTLUINT_TRAIT(v, BIN_UDEC), DEC_IBIN)
#define PPUTLINT_UPASS_DEC(v)          PPUTLINT_UPASS_DEC_o(v, PPUTLUINT_TRAIT(v, DEC_IBIN))
#define PPUTLINT_UPASS_DEC_o(v, ibin)  PPUTLINT_UPASS_DEC_oo(v, ibin)
#define PPUTLINT_UPASS_DEC_oo(v, ibin) PPUTLINT_UPASS_DEC_ooo(v, ibin, ibin##u)
#define PPUTLINT_UPASS_DEC_ooo(v, ibin, ubin) \
  PTL_CAT(PPUTLINT_UPASS_DEC_, PTL_ESC(PPUTLINT_MSB PPUTLUINT_TRAIT(ubin, BIN_BITS)))(v, ibin, ubin)
#define PPUTLINT_UPASS_DEC_1(v, ibin, ubin) ibin
#define PPUTLINT_UPASS_DEC_0(v, ibin, ubin) PPUTLUINT_TRAIT(ubin, BIN_IDEC)
#define PPUTLINT_MSB(a, ...)                a
#define PPUTLINT_FAIL(e, ...)               PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.ubase2]
/// -------------
/// casts to the unsigned int binary subtype.
///
/// PTL_UBASE2(0)             // 0b0000000000u
/// PTL_UBASE2(1)             // 0b0000000001u
/// PTL_UBASE2(5)             // 0b0000000101u
/// PTL_UBASE2(1023u)         // 0b1111111111u
/// PTL_UBASE2(0b0000000000u) // 0b0000000000u
/// PTL_UBASE2(0b0000000001u) // 0b0000000001u
/// PTL_UBASE2(0b1111111111)  // 0b1111111111u
#define PTL_UBASE2(/* n: uint|int */...) /* -> ubase2{n} */ PPUTLUBASE2_o(PTL_UINT(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLUBASE2_o(n)   PTL_CAT(PPUTLUBASE2_, PPUTLUINT_TRAIT(n, TYPE))(n)
#define PPUTLUBASE2_BIN(n) n
#define PPUTLUBASE2_DEC(n) PTL_CAT(PPUTLUINT_TRAIT(n, DEC_IBIN), u)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.ibase2]
/// -------------
/// casts to the signed int binary subtype.
///
/// PTL_IBASE2(0)    // 0b0000000000
/// PTL_IBASE2(5)    // 0b0000000101
/// PTL_IBASE2(1023) // 0b1111111111
/// PTL_IBASE2(511u) // 0b0111111111
#define PTL_IBASE2(/* n: uint|int */...) /* -> ibase2{n} */ PPUTLIBASE2_o(PTL_INT(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIBASE2_o(n)      PPUTLIBASE2_oo(n, PTL_CAT(n, u))
#define PPUTLIBASE2_oo(n, u)  PTL_CAT(PPUTLIBASE2_, PPUTLUINT_TRAIT(u, TYPE))(n, u)
#define PPUTLIBASE2_BIN(n, u) n
#define PPUTLIBASE2_DEC(n, u) PPUTLUINT_TRAIT(u, DEC_IBIN)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.ubase10]
/// --------------
/// casts to the unsigned int decimal subtype.
///
/// PTL_UBASE10(0b0000000000u) // 0u
/// PTL_UBASE10(1)             // 1u
/// PTL_UBASE10(0b0000000101)  // 5u
/// PTL_UBASE10(0b1111111111u) // 1023u
/// PTL_UBASE10(0b1000000000)  // 512u
#define PTL_UBASE10(/* n: uint|int */...) /* -> ubase10{n} */ PPUTLUBASE10_o(PTL_UINT(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLUBASE10_o(n)   PTL_CAT(PPUTLUBASE10_, PPUTLUINT_TRAIT(n, TYPE))(n)
#define PPUTLUBASE10_BIN(n) PPUTLUINT_TRAIT(n, BIN_UDEC)
#define PPUTLUBASE10_DEC(n) n

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.ibase10]
/// --------------
/// [positive values only] casts to the signed int decimal subtype.
///
/// value will remain as binary if negative.
/// use fmt.paste_int to get a negative decimal.
///
/// PTL_IBASE10(0b0000000000)  // 0
/// PTL_IBASE10(0b0000000101u) // 5
/// PTL_IBASE10(0b0111111111)  // 511
/// PTL_IBASE10(0b1000000000)  // 0b1000000000
/// PTL_IBASE10(511)           // 511
/// PTL_IBASE10(1023)          // 0b1111111111
#define PTL_IBASE10(/* n: uint|int */...) /* -> ibase10{n} */ PPUTLIBASE10_o(PTL_INT(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIBASE10_o(n)     PPUTLIBASE10_oo(n, PTL_CAT(n, u))
#define PPUTLIBASE10_oo(n, u) PTL_CAT(PPUTLIBASE10_, PPUTLUINT_TRAIT(u, TYPE))(n, u)
#define PPUTLIBASE10_BIN(n, u) \
  PTL_CAT(PPUTLIBASE10_BIN_, PTL_ESC(PPUTLIBASE10_MSB PPUTLUINT_TRAIT(u, BIN_BITS)))(n, u)
#define PPUTLIBASE10_BIN_1(n, u) n
#define PPUTLIBASE10_BIN_0(n, u) PPUTLUINT_TRAIT(u, BIN_IDEC)
#define PPUTLIBASE10_DEC(n, u)   n
#define PPUTLIBASE10_MSB(a, ...) a

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.any]
/// ----------
/// any type (generic data). returns arg.
/// describes exactly one generic value.
///
/// PTL_ANY(foo) // foo
#define PTL_ANY(/* v: any */...) /* -> any{v} */ PPUTLANY_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLANY_o(v) v

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.typeof]
/// -------------
/// detects the value type. must be compatible with the ## operator.
/// literal 0 and 1 are considered ibase10 rather than bool.
/// defaults to any if a type could not be determined.
///
/// returns one of:
/// - PTL_TUPLE
/// - PTL_UBASE2
/// - PTL_IBASE2
/// - PTL_UBASE10
/// - PTL_IBASE10
/// - PTL_ANY
///
/// PTL_TYPEOF((foo))         // PTL_TUPLE
/// PTL_TYPEOF(0)             // PTL_IBASE10
/// PTL_TYPEOF(0u)            // PTL_UBASE10
/// PTL_TYPEOF(1023u)         // PTL_UBASE10
/// PTL_TYPEOF(0b1111111111)  // PTL_IBASE2
/// PTL_TYPEOF(0b1111111111u) // PTL_UBASE2
/// PTL_TYPEOF(foo)           // PTL_ANY
#define PTL_TYPEOF(/* v: tuple|uint|int|any */...) /* -> <ctor> */ \
  PPUTLTYPEOF_o(PTL_EAT __VA_ARGS__)(__VA_ARGS__)(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLTYPEOF_o(...) PPUTLTYPEOF_oo##__VA_OPT__(_NO)##_TUPLE
#define PPUTLTYPEOF_oo_NO_TUPLE(...) \
  PTL_CAT(PPUTLTYPEOF_oo_NO_TUPLE_,  \
          PPUTLUINT_o(__VA_ARGS__.)(__VA_ARGS__)(__VA_ARGS__)(__VA_ARGS__))
#define PPUTLTYPEOF_oo_NO_TUPLE_PPUTLUINT_IPASS PPUTLTYPEOF_ooo_INT
#define PPUTLTYPEOF_oo_NO_TUPLE_PPUTLUINT_UPASS PPUTLTYPEOF_ooo_UINT
#define PPUTLTYPEOF_oo_NO_TUPLE_PPUTLUINT_FAIL  PPUTLTYPEOF_ooo_ANY
#define PPUTLTYPEOF_oo_TUPLE(...)               PPUTLTYPEOF_ooo_TUPLE
#define PPUTLTYPEOF_ooo_INT(i) \
  PTL_CAT(PPUTLTYPEOF_ooo_INT_, PPUTLUINT_TRAIT(PTL_CAT(PTL_INT(i), u), TYPE))
#define PPUTLTYPEOF_ooo_INT_DEC    PTL_IBASE10
#define PPUTLTYPEOF_ooo_INT_BIN    PTL_IBASE2
#define PPUTLTYPEOF_ooo_UINT(u)    PTL_CAT(PPUTLTYPEOF_ooo_UINT_, PPUTLUINT_TRAIT(PTL_UINT(u), TYPE))
#define PPUTLTYPEOF_ooo_UINT_DEC   PTL_UBASE10
#define PPUTLTYPEOF_ooo_UINT_BIN   PTL_UBASE2
#define PPUTLTYPEOF_ooo_TUPLE(...) PTL_TUPLE
#define PPUTLTYPEOF_ooo_ANY(...)   PTL_ANY

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

/// [traits.is_none]
/// ----------------
/// detects if args is nothing.
///
/// PTL_IS_NONE()          // 1
/// PTL_IS_NONE(foo)       // 0
/// PTL_IS_NONE(foo, bar)  // 0
/// PTL_IS_NONE(PTL_ESC()) // 1
#define PTL_IS_NONE(/* v: any... */...) /* -> bool */ PPUTLIS_NONE_o##__VA_OPT__(0)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_NONE_o0 0
#define PPUTLIS_NONE_o  1

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_some]
/// ----------------
/// detects if args is something.
///
/// PTL_IS_SOME()          // 0
/// PTL_IS_SOME(foo)       // 1
/// PTL_IS_SOME(foo, bar)  // 1
/// PTL_IS_SOME(PTL_ESC()) // 0
#define PTL_IS_SOME(/* v: any... */...) /* -> bool */ PPUTLIS_SOME_o##__VA_OPT__(1)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_SOME_o1 1
#define PPUTLIS_SOME_o  0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_any]
/// ---------------
/// detects if args is exactly one generic value.
///
/// PTL_IS_ANY()         // 0
/// PTL_IS_ANY(,)        // 0
/// PTL_IS_ANY(foo,)     // 0
/// PTL_IS_ANY(foo, bar) // 0
/// PTL_IS_ANY(foo)      // 1
/// PTL_IS_ANY((42))     // 1
#define PTL_IS_ANY(/* v: any... */...) /* -> bool{sizeof v == 1} */ \
  PPUTLIS_ANY_o(__VA_ARGS__ __VA_OPT__(.))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_ANY_o(_, ...) PTL_AND(PTL_IS_SOME(_), PTL_IS_NONE(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.size]
/// -------------
/// O(1) variadic argument size computation.
/// fails if too many args passed.
///
/// PTL_SIZE()     // 0u
/// PTL_SIZE(a)    // 1u
/// PTL_SIZE(a, b) // 2u
/// PTL_SIZE(, )   // 2u
#define PTL_SIZE(/* v: any... */...) /* -> uint */ \
  PPUTLSIZE_X(PTL_ISTR([PTL_SIZE] too many args : __VA_ARGS__), PPUTLSIZE, __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLSIZE_X(_err, _, ...)                                                                  \
  PPUTLSIZE_READ(                                                                                  \
      _err, __VA_ARGS__ __VA_OPT__(, ) _(1023u), _(1022u), _(1021u), _(1020u), _(1019u), _(1018u), \
      _(1017u), _(1016u), _(1015u), _(1014u), _(1013u), _(1012u), _(1011u), _(1010u), _(1009u),    \
      _(1008u), _(1007u), _(1006u), _(1005u), _(1004u), _(1003u), _(1002u), _(1001u), _(1000u),    \
      _(999u), _(998u), _(997u), _(996u), _(995u), _(994u), _(993u), _(992u), _(991u), _(990u),    \
      _(989u), _(988u), _(987u), _(986u), _(985u), _(984u), _(983u), _(982u), _(981u), _(980u),    \
      _(979u), _(978u), _(977u), _(976u), _(975u), _(974u), _(973u), _(972u), _(971u), _(970u),    \
      _(969u), _(968u), _(967u), _(966u), _(965u), _(964u), _(963u), _(962u), _(961u), _(960u),    \
      _(959u), _(958u), _(957u), _(956u), _(955u), _(954u), _(953u), _(952u), _(951u), _(950u),    \
      _(949u), _(948u), _(947u), _(946u), _(945u), _(944u), _(943u), _(942u), _(941u), _(940u),    \
      _(939u), _(938u), _(937u), _(936u), _(935u), _(934u), _(933u), _(932u), _(931u), _(930u),    \
      _(929u), _(928u), _(927u), _(926u), _(925u), _(924u), _(923u), _(922u), _(921u), _(920u),    \
      _(919u), _(918u), _(917u), _(916u), _(915u), _(914u), _(913u), _(912u), _(911u), _(910u),    \
      _(909u), _(908u), _(907u), _(906u), _(905u), _(904u), _(903u), _(902u), _(901u), _(900u),    \
      _(899u), _(898u), _(897u), _(896u), _(895u), _(894u), _(893u), _(892u), _(891u), _(890u),    \
      _(889u), _(888u), _(887u), _(886u), _(885u), _(884u), _(883u), _(882u), _(881u), _(880u),    \
      _(879u), _(878u), _(877u), _(876u), _(875u), _(874u), _(873u), _(872u), _(871u), _(870u),    \
      _(869u), _(868u), _(867u), _(866u), _(865u), _(864u), _(863u), _(862u), _(861u), _(860u),    \
      _(859u), _(858u), _(857u), _(856u), _(855u), _(854u), _(853u), _(852u), _(851u), _(850u),    \
      _(849u), _(848u), _(847u), _(846u), _(845u), _(844u), _(843u), _(842u), _(841u), _(840u),    \
      _(839u), _(838u), _(837u), _(836u), _(835u), _(834u), _(833u), _(832u), _(831u), _(830u),    \
      _(829u), _(828u), _(827u), _(826u), _(825u), _(824u), _(823u), _(822u), _(821u), _(820u),    \
      _(819u), _(818u), _(817u), _(816u), _(815u), _(814u), _(813u), _(812u), _(811u), _(810u),    \
      _(809u), _(808u), _(807u), _(806u), _(805u), _(804u), _(803u), _(802u), _(801u), _(800u),    \
      _(799u), _(798u), _(797u), _(796u), _(795u), _(794u), _(793u), _(792u), _(791u), _(790u),    \
      _(789u), _(788u), _(787u), _(786u), _(785u), _(784u), _(783u), _(782u), _(781u), _(780u),    \
      _(779u), _(778u), _(777u), _(776u), _(775u), _(774u), _(773u), _(772u), _(771u), _(770u),    \
      _(769u), _(768u), _(767u), _(766u), _(765u), _(764u), _(763u), _(762u), _(761u), _(760u),    \
      _(759u), _(758u), _(757u), _(756u), _(755u), _(754u), _(753u), _(752u), _(751u), _(750u),    \
      _(749u), _(748u), _(747u), _(746u), _(745u), _(744u), _(743u), _(742u), _(741u), _(740u),    \
      _(739u), _(738u), _(737u), _(736u), _(735u), _(734u), _(733u), _(732u), _(731u), _(730u),    \
      _(729u), _(728u), _(727u), _(726u), _(725u), _(724u), _(723u), _(722u), _(721u), _(720u),    \
      _(719u), _(718u), _(717u), _(716u), _(715u), _(714u), _(713u), _(712u), _(711u), _(710u),    \
      _(709u), _(708u), _(707u), _(706u), _(705u), _(704u), _(703u), _(702u), _(701u), _(700u),    \
      _(699u), _(698u), _(697u), _(696u), _(695u), _(694u), _(693u), _(692u), _(691u), _(690u),    \
      _(689u), _(688u), _(687u), _(686u), _(685u), _(684u), _(683u), _(682u), _(681u), _(680u),    \
      _(679u), _(678u), _(677u), _(676u), _(675u), _(674u), _(673u), _(672u), _(671u), _(670u),    \
      _(669u), _(668u), _(667u), _(666u), _(665u), _(664u), _(663u), _(662u), _(661u), _(660u),    \
      _(659u), _(658u), _(657u), _(656u), _(655u), _(654u), _(653u), _(652u), _(651u), _(650u),    \
      _(649u), _(648u), _(647u), _(646u), _(645u), _(644u), _(643u), _(642u), _(641u), _(640u),    \
      _(639u), _(638u), _(637u), _(636u), _(635u), _(634u), _(633u), _(632u), _(631u), _(630u),    \
      _(629u), _(628u), _(627u), _(626u), _(625u), _(624u), _(623u), _(622u), _(621u), _(620u),    \
      _(619u), _(618u), _(617u), _(616u), _(615u), _(614u), _(613u), _(612u), _(611u), _(610u),    \
      _(609u), _(608u), _(607u), _(606u), _(605u), _(604u), _(603u), _(602u), _(601u), _(600u),    \
      _(599u), _(598u), _(597u), _(596u), _(595u), _(594u), _(593u), _(592u), _(591u), _(590u),    \
      _(589u), _(588u), _(587u), _(586u), _(585u), _(584u), _(583u), _(582u), _(581u), _(580u),    \
      _(579u), _(578u), _(577u), _(576u), _(575u), _(574u), _(573u), _(572u), _(571u), _(570u),    \
      _(569u), _(568u), _(567u), _(566u), _(565u), _(564u), _(563u), _(562u), _(561u), _(560u),    \
      _(559u), _(558u), _(557u), _(556u), _(555u), _(554u), _(553u), _(552u), _(551u), _(550u),    \
      _(549u), _(548u), _(547u), _(546u), _(545u), _(544u), _(543u), _(542u), _(541u), _(540u),    \
      _(539u), _(538u), _(537u), _(536u), _(535u), _(534u), _(533u), _(532u), _(531u), _(530u),    \
      _(529u), _(528u), _(527u), _(526u), _(525u), _(524u), _(523u), _(522u), _(521u), _(520u),    \
      _(519u), _(518u), _(517u), _(516u), _(515u), _(514u), _(513u), _(512u), _(511u), _(510u),    \
      _(509u), _(508u), _(507u), _(506u), _(505u), _(504u), _(503u), _(502u), _(501u), _(500u),    \
      _(499u), _(498u), _(497u), _(496u), _(495u), _(494u), _(493u), _(492u), _(491u), _(490u),    \
      _(489u), _(488u), _(487u), _(486u), _(485u), _(484u), _(483u), _(482u), _(481u), _(480u),    \
      _(479u), _(478u), _(477u), _(476u), _(475u), _(474u), _(473u), _(472u), _(471u), _(470u),    \
      _(469u), _(468u), _(467u), _(466u), _(465u), _(464u), _(463u), _(462u), _(461u), _(460u),    \
      _(459u), _(458u), _(457u), _(456u), _(455u), _(454u), _(453u), _(452u), _(451u), _(450u),    \
      _(449u), _(448u), _(447u), _(446u), _(445u), _(444u), _(443u), _(442u), _(441u), _(440u),    \
      _(439u), _(438u), _(437u), _(436u), _(435u), _(434u), _(433u), _(432u), _(431u), _(430u),    \
      _(429u), _(428u), _(427u), _(426u), _(425u), _(424u), _(423u), _(422u), _(421u), _(420u),    \
      _(419u), _(418u), _(417u), _(416u), _(415u), _(414u), _(413u), _(412u), _(411u), _(410u),    \
      _(409u), _(408u), _(407u), _(406u), _(405u), _(404u), _(403u), _(402u), _(401u), _(400u),    \
      _(399u), _(398u), _(397u), _(396u), _(395u), _(394u), _(393u), _(392u), _(391u), _(390u),    \
      _(389u), _(388u), _(387u), _(386u), _(385u), _(384u), _(383u), _(382u), _(381u), _(380u),    \
      _(379u), _(378u), _(377u), _(376u), _(375u), _(374u), _(373u), _(372u), _(371u), _(370u),    \
      _(369u), _(368u), _(367u), _(366u), _(365u), _(364u), _(363u), _(362u), _(361u), _(360u),    \
      _(359u), _(358u), _(357u), _(356u), _(355u), _(354u), _(353u), _(352u), _(351u), _(350u),    \
      _(349u), _(348u), _(347u), _(346u), _(345u), _(344u), _(343u), _(342u), _(341u), _(340u),    \
      _(339u), _(338u), _(337u), _(336u), _(335u), _(334u), _(333u), _(332u), _(331u), _(330u),    \
      _(329u), _(328u), _(327u), _(326u), _(325u), _(324u), _(323u), _(322u), _(321u), _(320u),    \
      _(319u), _(318u), _(317u), _(316u), _(315u), _(314u), _(313u), _(312u), _(311u), _(310u),    \
      _(309u), _(308u), _(307u), _(306u), _(305u), _(304u), _(303u), _(302u), _(301u), _(300u),    \
      _(299u), _(298u), _(297u), _(296u), _(295u), _(294u), _(293u), _(292u), _(291u), _(290u),    \
      _(289u), _(288u), _(287u), _(286u), _(285u), _(284u), _(283u), _(282u), _(281u), _(280u),    \
      _(279u), _(278u), _(277u), _(276u), _(275u), _(274u), _(273u), _(272u), _(271u), _(270u),    \
      _(269u), _(268u), _(267u), _(266u), _(265u), _(264u), _(263u), _(262u), _(261u), _(260u),    \
      _(259u), _(258u), _(257u), _(256u), _(255u), _(254u), _(253u), _(252u), _(251u), _(250u),    \
      _(249u), _(248u), _(247u), _(246u), _(245u), _(244u), _(243u), _(242u), _(241u), _(240u),    \
      _(239u), _(238u), _(237u), _(236u), _(235u), _(234u), _(233u), _(232u), _(231u), _(230u),    \
      _(229u), _(228u), _(227u), _(226u), _(225u), _(224u), _(223u), _(222u), _(221u), _(220u),    \
      _(219u), _(218u), _(217u), _(216u), _(215u), _(214u), _(213u), _(212u), _(211u), _(210u),    \
      _(209u), _(208u), _(207u), _(206u), _(205u), _(204u), _(203u), _(202u), _(201u), _(200u),    \
      _(199u), _(198u), _(197u), _(196u), _(195u), _(194u), _(193u), _(192u), _(191u), _(190u),    \
      _(189u), _(188u), _(187u), _(186u), _(185u), _(184u), _(183u), _(182u), _(181u), _(180u),    \
      _(179u), _(178u), _(177u), _(176u), _(175u), _(174u), _(173u), _(172u), _(171u), _(170u),    \
      _(169u), _(168u), _(167u), _(166u), _(165u), _(164u), _(163u), _(162u), _(161u), _(160u),    \
      _(159u), _(158u), _(157u), _(156u), _(155u), _(154u), _(153u), _(152u), _(151u), _(150u),    \
      _(149u), _(148u), _(147u), _(146u), _(145u), _(144u), _(143u), _(142u), _(141u), _(140u),    \
      _(139u), _(138u), _(137u), _(136u), _(135u), _(134u), _(133u), _(132u), _(131u), _(130u),    \
      _(129u), _(128u), _(127u), _(126u), _(125u), _(124u), _(123u), _(122u), _(121u), _(120u),    \
      _(119u), _(118u), _(117u), _(116u), _(115u), _(114u), _(113u), _(112u), _(111u), _(110u),    \
      _(109u), _(108u), _(107u), _(106u), _(105u), _(104u), _(103u), _(102u), _(101u), _(100u),    \
      _(99u), _(98u), _(97u), _(96u), _(95u), _(94u), _(93u), _(92u), _(91u), _(90u), _(89u),      \
      _(88u), _(87u), _(86u), _(85u), _(84u), _(83u), _(82u), _(81u), _(80u), _(79u), _(78u),      \
      _(77u), _(76u), _(75u), _(74u), _(73u), _(72u), _(71u), _(70u), _(69u), _(68u), _(67u),      \
      _(66u), _(65u), _(64u), _(63u), _(62u), _(61u), _(60u), _(59u), _(58u), _(57u), _(56u),      \
      _(55u), _(54u), _(53u), _(52u), _(51u), _(50u), _(49u), _(48u), _(47u), _(46u), _(45u),      \
      _(44u), _(43u), _(42u), _(41u), _(40u), _(39u), _(38u), _(37u), _(36u), _(35u), _(34u),      \
      _(33u), _(32u), _(31u), _(30u), _(29u), _(28u), _(27u), _(26u), _(25u), _(24u), _(23u),      \
      _(22u), _(21u), _(20u), _(19u), _(18u), _(17u), _(16u), _(15u), _(14u), _(13u), _(12u),      \
      _(11u), _(10u), _(9u), _(8u), _(7u), _(6u), _(5u), _(4u), _(3u), _(2u), _(1u), _(0u))
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
#define PTL_IS_TUPLE(/* v: any... */...) /* -> bool */ PTL_IS_NONE(PTL_EAT __VA_ARGS__)

/// [traits.is_bool]
/// ----------------
/// detects if args is a bool.
///
/// PTL_IS_BOOL()             // 0
/// PTL_IS_BOOL(0)            // 1
/// PTL_IS_BOOL(1)            // 1
/// PTL_IS_BOOL(1u)           // 0
/// PTL_IS_BOOL(0b0000000000) // 0
/// PTL_IS_BOOL(0, 1)         // 0
/// PTL_IS_BOOL((0))          // 0
#define PTL_IS_BOOL(/* v: any... */...) /* -> bool */ \
  PTL_CAT(PPUTLIS_BOOL_, PPUTLBOOL_o(__VA_ARGS__.)(__VA_ARGS__)(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_BOOL_PPUTLBOOL_PASS 1
#define PPUTLIS_BOOL_PPUTLBOOL_FAIL 0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_ubase2]
/// ------------------
/// detects if args is an unsigned int in base2 form (requires 'u' suffix).
/// binary bit length is fixed at PTL_BIT_LENGTH (10).
///
/// PTL_IS_UBASE2(1)             // 0
/// PTL_IS_UBASE2(1u)            // 0
/// PTL_IS_UBASE2(0b0000000000u) // 1
/// PTL_IS_UBASE2(0b1111111111)  // 0
/// PTL_IS_UBASE2((), ())        // 0
#define PTL_IS_UBASE2(/* v: any... */...) /* -> bool */ \
  PTL_CAT(PPUTLIS_UBASE2_, PTL_IS_ANY(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_UBASE2_1(...) PTL_IS_NONE(PTL_CAT(PPUTLIS_UBASE2_, PTL_TYPEOF(__VA_ARGS__)))
#define PPUTLIS_UBASE2_0(...) 0
#define PPUTLIS_UBASE2_PTL_UBASE2

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_ubase10]
/// -------------------
/// detects if args is an unsigned int in base10 form (requires 'u' suffix).
///
/// PTL_IS_UBASE10(1)             // 0
/// PTL_IS_UBASE10(1u)            // 1
/// PTL_IS_UBASE10(1023u)         // 1
/// PTL_IS_UBASE10(0b0000000000u) // 0
/// PTL_IS_UBASE10(0b1111111111)  // 0
/// PTL_IS_UBASE10((), ())        // 0
#define PTL_IS_UBASE10(/* v: any... */...) /* -> bool */ \
  PTL_CAT(PPUTLIS_UBASE10_, PTL_IS_ANY(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_UBASE10_1(...) PTL_IS_NONE(PTL_CAT(PPUTLIS_UBASE10_, PTL_TYPEOF(__VA_ARGS__)))
#define PPUTLIS_UBASE10_0(...) 0
#define PPUTLIS_UBASE10_PTL_UBASE10

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_ibase2]
/// ------------------
/// detects if args is a signed int in base2 form.
/// binary bit length is fixed at PTL_BIT_LENGTH (10).
///
/// PTL_IS_IBASE2(1)             // 0
/// PTL_IS_IBASE2(1u)            // 0
/// PTL_IS_IBASE2(0b0000000000)  // 1
/// PTL_IS_IBASE2(0b1111111111)  // 1
/// PTL_IS_IBASE2(0b1111111111u) // 0
/// PTL_IS_IBASE2((), ())        // 0
#define PTL_IS_IBASE2(/* v: any... */...) /* -> bool */ \
  PTL_CAT(PPUTLIS_IBASE2_, PTL_IS_ANY(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_IBASE2_1(...) PTL_IS_NONE(PTL_CAT(PPUTLIS_IBASE2_, PTL_TYPEOF(__VA_ARGS__)))
#define PPUTLIS_IBASE2_0(...) 0
#define PPUTLIS_IBASE2_PTL_IBASE2

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_ibase10]
/// -------------------
/// detects if args is a signed int in base10 form.
///
/// PTL_IS_IBASE10(1)             // 1
/// PTL_IS_IBASE10(1u)            // 0
/// PTL_IS_IBASE10(511)           // 1
/// PTL_IS_IBASE10(0b0000000000u) // 0
/// PTL_IS_IBASE10(0b1111111111)  // 0
/// PTL_IS_IBASE10((), ())        // 0
#define PTL_IS_IBASE10(/* v: any... */...) /* -> bool */ \
  PTL_CAT(PPUTLIS_IBASE10_, PTL_IS_ANY(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_IBASE10_1(...) PTL_IS_NONE(PTL_CAT(PPUTLIS_IBASE10_, PTL_TYPEOF(__VA_ARGS__)))
#define PPUTLIS_IBASE10_0(...) 0
#define PPUTLIS_IBASE10_PTL_IBASE10

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_uint]
/// ----------------
/// detects if args is an unsigned integer.
/// binary bit length is fixed at PTL_BIT_LENGTH (10).
///
/// PTL_IS_UINT()              // 0
/// PTL_IS_UINT(foo)           // 0
/// PTL_IS_UINT(0)             // 0
/// PTL_IS_UINT(0u)            // 1
/// PTL_IS_UINT(0b0000000000u) // 1
/// PTL_IS_UINT(0b1111111111)  // 0
/// PTL_IS_UINT(0b110u)        // 0
/// PTL_IS_UINT((), ())        // 0
#define PTL_IS_UINT(/* v: any... */...) /* -> bool */ \
  PTL_CAT(PPUTLIS_UINT_, PTL_IS_ANY(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_UINT_1(...) PTL_IS_NONE(PTL_CAT(PPUTLIS_UINT_, PTL_TYPEOF(__VA_ARGS__)))
#define PPUTLIS_UINT_0(...) 0
#define PPUTLIS_UINT_PTL_UBASE10
#define PPUTLIS_UINT_PTL_UBASE2

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_int]
/// ---------------
/// detects if args is a signed integer.
/// binary bit length is fixed at PTL_BIT_LENGTH (10).
///
/// PTL_IS_INT()              // 0
/// PTL_IS_INT(foo)           // 0
/// PTL_IS_INT(0)             // 1
/// PTL_IS_INT(0u)            // 0
/// PTL_IS_INT(0b0000000000u) // 0
/// PTL_IS_INT(0b1111111111)  // 1
/// PTL_IS_INT(0b110u)        // 0
/// PTL_IS_INT((), ())        // 0
#define PTL_IS_INT(/* v: any... */...) /* -> bool */ \
  PTL_CAT(PPUTLIS_INT_, PTL_IS_ANY(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_INT_1(...) PTL_IS_NONE(PTL_CAT(PPUTLIS_INT_, PTL_TYPEOF(__VA_ARGS__)))
#define PPUTLIS_INT_0(...) 0
#define PPUTLIS_INT_PTL_IBASE10
#define PPUTLIS_INT_PTL_IBASE2

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

/// [traits.bits]
/// -------------
/// extracts uint bits.
/// size of returned args is exactly PTL_BIT_LENGTH (10).
///
/// PTL_BITS(0)             // 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
/// PTL_BITS(1)             // 0, 0, 0, 0, 0, 0, 0, 0, 0, 1
/// PTL_BITS(0b1111111110u) // 1, 1, 1, 1, 1, 1, 1, 1, 1, 0
#define PTL_BITS(/* v: uint|int */...) /* -> bool... */ \
  PTL_ESC(PPUTLBITS_X PPUTLUINT_TRAIT(PTL_UBASE2(__VA_ARGS__), BIN_BITS))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBITS_X(a, b, c, d, e, f, g, h, i, j) a, b, c, d, e, f, g, h, i, j

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

/// [meta.xct_size]
/// ---------------
/// measures an xct expr to determine the number of expansions it experienced.
/// ignores the expansion required to read the result.
///
/// fails if xct is not a valid xct expression.
/// PTL_SIZE will fail if the xct expression is too large.
///
/// PTL_XCT_SIZE(PTL_XCT)                            // 0u
/// PTL_XCT_SIZE(PTL_ESC(PTL_XCT))                   // 1u
/// PTL_XCT_SIZE(PTL_ESC(PTL_ESC(PTL_XCT)))          // 2u
/// PTL_XCT_SIZE(PTL_ESC(PTL_ESC(PTL_ESC(PTL_XCT)))) // 3u
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

// vim: fdm=marker:fmr={{{,}}}

#endif

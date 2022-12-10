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
//    countless metaprogramming techniques that should be preferred.          //
//                                                                            //
//    This library is built to provide a strong, safe set of functionality    //
//    for edge cases that still uniquely benefit from text replacement and    //
//    would otherwise utilize a separate code generation script or require    //
//    higly verbose/redundant syntax, such as comprehensive test coverage,    //
//    struct reflection, static initialization control, or optimization of    //
//    template specialization algorithms.                                     //
//                                                                            //
//    ABOUT                                                                   //
//    -----                                                                   //
//                                                                            //
//    pputl is a powerful C++ preprocessor utilities library that provides    //
//    many high-level constructs including integers, recursion, and OOP.      //
//                                                                            //
//    Speed, safety, and flexibility are its primary goals.                   //
//                                                                            //
//    Algorithms operate using an inline recursion technique that requires    //
//    fewer expansions than mutually-recursive patterns and can be nested.    //
//    Macros are provided for performing arbitrary, nestable recursion.       //
//                                                                            //
//    Functions verify their arguments and use type casting to ensure that    //
//    useful error messages are generated as soon as an issue manifests.      //
//                                                                            //
//    Features overview:                                                      //
//                                                                            //
//     ◆ language enhancements                                                //
//       ‐ basic argument manipulation                              [lang]    //
//           eat  esc  first  xfirst  rest  xrest  trim                       //
//       ‐ control flow                                    [lang, control]    //
//           default  fail  if  switch                                        //
//       ‐ type casting                                             [type]    //
//           any   none   tup    pair  kv    sym  enum                        //
//           idec  bool   ihex   udec  uhex  int  uint                        //
//           word  size   ofs    obj   null  arr  buf                         //
//           ord   stack  queue  heap  pq    set  map                         //
//       ‐ type traits                                     [traits, range]    //
//           is_any   is_none  is_tup   is_pair  is_kv     is_sym             //
//           is_enum  is_idec  is_bool  is_ihex  is_udec   is_uhex            //
//           is_int   is_uint  is_word  is_size  is_ofs    is_obj             //
//           is_null  is_arr   is_buf   is_ord   is_stack  is_queue           //
//           is_heap  is_pq    is_set   is_map   enum_k    enum_v             //
//           typeof   chainof  countof  sizeof   itemsof   is_empty           //
//       ‐ boolean logic                                           [logic]    //
//           not  and  or  xor  nand  nor  xnor                               //
//       ‐ paste formatting                                    [lang, fmt]    //
//           str  xstr  cat  xcat  c_int  c_bin                               //
//     ◆ signed and unsigned integers                                         //
//       ‐ arithmetic                                      [numeric, math]    //
//           inc  dec  neg  abs   log2  sqrt  fact  prime                     //
//           add  sub  mul  divr  div   rem   index                           //
//       ‐ comparison                                   [numeric, compare]    //
//           eqz  nez  ltz  gtz  lez  gez  lt                                 //
//           gt   le   ge   eq   ne   min  max                                //
//       ‐ bitwise operations                                    [bitwise]    //
//           hexdump  bitdump  bitsll   bitsrl   bitsra   bitnot              //
//           bitand   bitor    bitxor   bitnand  bitnor   bitxnor             //
//           bitget   bitset   bitflip  bitrotl  bitrotr                      //
//     ◆ datastructures and algorithms                                        //
//       ‐ range manipulation                                      [range]    //
//           bisect     unite      head      tail    push_front               //
//           push_back  pop_front  pop_back  get     set                      //
//           ins        del        swap      front   back                     //
//           slice      splice     push      pop                              //
//       ‐ range generation                                         [algo]    //
//           iota  repeat  generate_lp  generate_rp  enumerate                //
//       ‐ range algorithms                                         [algo]    //
//           reverse    transform_lp  transform_rp  shift    rotate           //
//           reduce_lp  reduce_rp     sort_lp       sort_rp  find             //
//           find_not   find_if_lp    find_if_rp                              //
//     ◆ metaprogramming utilities                                            //
//       ‐ expansion control and tracing                            [meta]    //
//           x  xx_lp  xx_rp  lp  rp  xtrace  xtrace_read                     //
//       ‐ inline recursive stack construction                      [meta]    //
//           recur_lp  recur_rp                                               //
//     ◆ configuration details                                    [config]    //
//           version  build     word_size  bit_length  int_min                //
//           int_max  uint_max  size_max   offset_min  offset_max             //
//                                                                            //
//    USAGE                                                                   //
//    -----                                                                   //
//                                                                            //
//    Copy pputl.h and include. The distribution is single-header.            //
//                                                                            //
//    pputl requires a preprocessor that supports the C++20 specifications    //
//    for macro replacement and macro-related implementation limits.          //
//                                                                            //
//    pputl is completely generated and tested by a custom C++ framework.     //
//    See the codegen/ folder for the full source.                            //
//                                                                            //
//    Various settings including word size and naming rules may be changed    //
//    by modifying the head of codegen/codegen.h and running `make`.          //
//                                                                            //
//    The default build uses 12-bit words and 8-bit sizes and is              //
//    fully compliant with the C++20 standard, which defines the              //
//    following implementation limits in [implimits]:                         //
//                                                                            //
//     ‐ Macro identifiers simultaneously                                     //
//       defined in one translation unit: [65536].                            //
//     ‐ Parameters in one macro definition: [256].                           //
//     ‐ Arguments in one macro invocation: [256].                            //
//                                                                            //
//    If you wish to modify the settings to create your own build, keep       //
//    these limitations in mind. Exceeding these limits is possible but       //
//    depends on the preprocessor.                                            //
//                                                                            //
//    pputl has been tested with:                                             //
//                                                                            //
//      gcc   11.2.1                                                          //
//      clang 13.0.0                                                          //
//      MSVC  2022 (requires /Zc:preprocessor)                                //
//                                                                            //
//    Run `make test` to validate on your system.                             //
//                                                                            //
//    TERMINOLOGY                                                             //
//    -----------                                                             //
//                                                                            //
//    pputl makes extensive use of duck-typing  for control flow and error    //
//    management.  pputl types are essentially pairs of functions: one for    //
//    traits identification and another for construction and assertion.       //
//                                                                            //
//    API functions are strictly documented using this type system. Inputs    //
//    are validated by invoking the associated constructor or through some    //
//    other form of inference. An argument is valid if it can be converted    //
//    to or interpreted as its parameter type without losing information.     //
//                                                                            //
//     any: any potentially-empty argument in a __VA_ARGS__ expression        //
//      ├╴none: the literal nothing; an absence of pp-tokens                  //
//      ├╴atom: a non-empty, concatable sequence of pp-tokens                 //
//      │  ├╴int: 0x800-4096|0x801-4096|...|0|...|2046|2047 (2s-compl)        //
//      │  │  └╴bool: 0|1                                                     //
//      │  ├╴uint: 0u|1u|...|4094u|4095u                                      //
//      │  └╴word: int|uint                                                   //
//      │     ├╴size: a non-negative word less than size_max                  //
//      │     └╴ofs:  a word ranged from (-size_max, size_max)                //
//      ├╴tup: a parens-enclosed item sequence [e.g. (a, b, c)]               //
//      │  └╴pair: a two-tuple [e.g. (foo, bar)]                              //
//      └╴obj: an inheritable, atom-addressable state manager                 //
//         ├╴null:  an empty object (the default object)                      //
//         └╴range: a sized, indexable, and iterable item container           //
//            ├╴arr:   a freely-mutable item sequence                         //
//            ├╴stack: a LIFO stack                                           //
//            ├╴queue: a FIFO queue                                           //
//            ├╴map:   a word-keyed map                                       //
//            └╴set:   a set of words                                         //
//                                                                            //
//    FUNDAMENTALS                                                            //
//    ------------                                                            //
//                                                                            //
//    pputl errors  execute an invalid preprocessor operation by using the    //
//    concatenation operator (incorrectly) on a string error message.  All    //
//    errors produced by pputl functions include the macro name, a textual    //
//    description, and the primary expansion arguments.                       //
//                                                                            //
//    With a few exceptions in [lang], non-nullary API functions are fully    //
//    variadic and chainable  such that the outputs of one  may be used as    //
//    inputs to another.  Inputs must be distinguishable after the primary    //
//    expansion; deferred input behavior is undefined.                        //
//                                                                            //
//    Negative ints are represented as valid C++ arithmetic expressions in    //
//    order to avoid post-processing:  pputl arithmetic operations  always    //
//    expand to values  that are usable in both preprocessor and C++ code.    //
//    When constructing preprocessor identifiers,  use lang.cat to convert   ///
//    the expression to its hex prefix before concatenation.                ////
//                                                                         /////
///////////////////////////////////////////////////////////////////////////// */

/// [lang.esc]
/// ----------
/// escapes parentheses. identity function.
///
/// e.g. #define FOO (a, b, c)
///      PTL_ESC FOO // a, b, c
#define PTL_ESC(...) /* -> any... */ __VA_ARGS__

/// [lang.first]
/// ------------
/// immediately returns the first argument.
/// must have at least one argument.
///
/// useful for operating directly on __VA_ARGS__ or
/// for quickly retrieving the first tuple element
/// using an identity function such as PTL_ESC.
///
/// first cannot be used to extract from expression results,
/// as the inputs are evaluated immediately. use xfirst for
/// expressions that should expand before selection.
///
/// e.g. PTL_FIRST(__VA_ARGS__)
///      PTL_ESC(PTL_FIRST tup)
#define PTL_FIRST(/* first: any, ...rest: any */ _, ...) /* -> any */ _

/// [lang.rest]
/// -----------
/// immediately returns all args except for the first.
/// must have at least one argument.
///
/// useful for operating directly on __VA_ARGS__ or
/// for quickly retrieving the rest tuple elements
/// using an identity function such as PTL_ESC.
///
/// rest cannot be used to extract from expression results,
/// as the inputs are evaluated immediately. use xrest for
/// expressions that should expand before selection.
///
/// e.g. PTL_REST(__VA_ARGS__)
///      PTL_ESC(PTL_REST tup)
#define PTL_REST(/* first: any, ...rest: any */ _, ...) /* -> any... */ __VA_ARGS__

/// [lang.eat]
/// ----------
/// eats arguments; return nothing.
///
/// PTL_EAT()    // <nothing>
/// PTL_EAT(foo) // <nothing>
#define PTL_EAT(...) /* -> none */

/// [lang.cat]
/// ----------
/// immediately concatenates a with b.
/// must provide at least one arg.
/// args must be compatible with the ## operator.
///
/// cat cannot be used to concatenate expression results,
/// as the inputs are evaluated immediately. use xcat for
/// expressions that should expand before concatenation.
///
/// PTL_CAT(foo, bar)          // foobar
/// PTL_CAT(foo, PTL_EAT(bar)) // fooPTL_EAT(bar)
#define PTL_CAT(/* a: any, b: any */ a, b) /* -> any */ a##b

/// [lang.str]
/// ----------
/// immediately stringizes args.
///
/// str cannot be used to stringize expression results,
/// as the inputs are evaluated immediately. use xstr for
/// expressions that should expand before stringization.
///
/// PTL_STR()                  // ""
/// PTL_STR(foo, bar)          // "foo, bar"
/// PTL_STR(PTL_CAT(foo, bar)) // "PTL_CAT(foo, bar)"
#define PTL_STR(/* any... */...) /* -> obj */ #__VA_ARGS__

/// [lang.xstr]
/// -----------
/// stringizes args after an expansion.
///
/// PTL_XSTR()                  // ""
/// PTL_XSTR(foo, bar)          // "foo, bar"
/// PTL_XSTR(PTL_CAT(foo, bar)) // "foobar"
#define PTL_XSTR(/* any... */...) /* -> obj */ PTL_STR(__VA_ARGS__)

/// [lang.default]
/// --------------
/// returns the first argument iff the rest of the arguments are nothing.
/// else, returns only the rest of the arguments.
///
/// PTL_DEFAULT(a)       // a
/// PTL_DEFAULT(a,)      // a
/// PTL_DEFAULT(a, b)    // b
/// PTL_DEFAULT(a, b, c) // b, c
#define PTL_DEFAULT(/* default: any, ...args: any */...) /* -> any... */ \
  PPUTLIMPL_DEFAULT_o(__VA_ARGS__)(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_DEFAULT_o(_, ...)  PPUTLIMPL_DEFAULT_0##__VA_OPT__(1)
#define PPUTLIMPL_DEFAULT_01(_, ...) __VA_ARGS__
#define PPUTLIMPL_DEFAULT_0(_, ...)  _

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [lang.xcat]
/// -----------
/// concatenates two args after an expansion.
/// args must be compatible with the ## operator.
///
/// PTL_XCAT(foo, bar)          // foobar
/// PTL_XCAT(foo, PTL_EAT(bar)) // foo
/// PTL_XCAT(,)                 // <nothing>
#define PTL_XCAT(/* a: any, b: any */...) /* -> any */ PTL_CAT(__VA_ARGS__)

/// [lang.fail]
/// -----------
/// executes an invalid preprocessor operation to indicate a failure.
/// must provide a string literal message.
///
/// usage: PTL_FAIL("something bad happened")
///        PTL_FAIL(PTL_STR([myfun] invalid args : __VA_ARGS__))
#define PTL_FAIL(/* msg="unspecified error": atom */...) \
  PTL_XCAT(PTL_FAIL, PTL_DEFAULT("unspecified error", __VA_ARGS__))

/// [lang.xrest]
/// ------------
/// returns all arguments except for the first after an expansion.
///
/// PTL_XREST()                   // <nothing>
/// PTL_XREST(, )                 // <nothing>
/// PTL_XREST(a)                  // <nothing>
/// PTL_XREST(a, b)               // b
/// PTL_XREST(a, b, c)            // b, c
/// PTL_XREST(PTL_XREST(a, b, c)) // c
/// PTL_XREST(a, , )              // ,
/// PTL_XREST(a, b, , )           // b, ,
#define PTL_XREST(/* any... */...) /* -> any... */ __VA_OPT__(PTL_REST(__VA_ARGS__))

/// [lang.xfirst]
/// -------------
/// returns the first argument after an expansion.
///
/// PTL_XFIRST()     // <nothing>
/// PTL_XFIRST(, )   // <nothing>
/// PTL_XFIRST(a)    // a
/// PTL_XFIRST(a, b) // a
#define PTL_XFIRST(/* any... */...) /* -> any */ __VA_OPT__(PTL_FIRST(__VA_ARGS__))

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
#define PTL_TRIM(/* any... */...) /* -> any... */                                 \
  PTL_XCAT(PPUTLIMPL_TRIM_, PTL_XCAT(PPUTLIMPL_TRIM_SEL(PTL_XFIRST(__VA_ARGS__)), \
                                     PPUTLIMPL_TRIM_SEL(PTL_XREST(__VA_ARGS__)))) \
  (__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_TRIM_SEL(...)     0##__VA_OPT__(1)
#define PPUTLIMPL_TRIM_0101(_, ...) _, __VA_ARGS__
#define PPUTLIMPL_TRIM_010(_, ...)  _
#define PPUTLIMPL_TRIM_001(_, ...)  __VA_ARGS__
#define PPUTLIMPL_TRIM_00(...)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [config.bit_length]
/// -------------------
/// the number of bits that can be used to represent pputl integers.
/// see the readme code generation section to configure.
#define PTL_BIT_LENGTH /* -> udec&size */ 8u

/// [config.size_max]
/// -----------------
/// the maximum number of arguments bounded by the C++20 standard.
/// set to min(255, uint_max) unless built with cpp20_arglimit=false
/// (which sets size_max to uint_max).
#define PTL_SIZE_MAX /* -> udec&size */ 255u

/// [config.build]
/// --------------
/// the build number of this pputl release (ISO8601).
#define PTL_BUILD /* -> <c++ int> */ 20221210

/// [config.uint_max]
/// -----------------
/// the maximum value of a pputl unsigned int.
#define PTL_UINT_MAX /* -> udec */ 255u

/// [config.int_min]
/// ----------------
/// the minimum value of a pputl signed int.
/// only representable as hex. see type.int for details.
#define PTL_INT_MIN /* -> ihex */ 0x80

/// [config.word_size]
/// ------------------
/// the number of hex digits used to represent pputl integers.
/// hex representations of integers are fixed at this length.
/// see the readme code generation section to configure.
#define PTL_WORD_SIZE /* -> udec&size */ 2u

/// [config.int_max]
/// ----------------
/// the maximum value of a pputl signed int.
#define PTL_INT_MAX /* -> idec */ 127

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

/// [impl.arithhint]
/// ----------------
/// [internal] two-operand arithmetic cast hint.
///
/// immediately concatenates args.
///
/// returns UDEC|UHEX if either operand is
/// UDEC|UHEX, UDEC|IDEC if either operand
/// is UDEC|IDEC, and UHEX|IHEX otherwise.
#define PPUTLIMPL_ARITHHINT(/* IDEC|IHEX|UDEC|UHEX, IDEC|IHEX|UDEC|UHEX */ a, b) \
  PPUTLIMPL_ARITHHINT_##a##b

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_ARITHHINT_UHEXUHEX UHEX
#define PPUTLIMPL_ARITHHINT_UHEXUDEC UDEC
#define PPUTLIMPL_ARITHHINT_UHEXIHEX UHEX
#define PPUTLIMPL_ARITHHINT_UHEXIDEC UDEC
#define PPUTLIMPL_ARITHHINT_UDECUHEX UDEC
#define PPUTLIMPL_ARITHHINT_UDECUDEC UDEC
#define PPUTLIMPL_ARITHHINT_UDECIHEX UDEC
#define PPUTLIMPL_ARITHHINT_UDECIDEC UDEC
#define PPUTLIMPL_ARITHHINT_IHEXUHEX UHEX
#define PPUTLIMPL_ARITHHINT_IHEXUDEC UDEC
#define PPUTLIMPL_ARITHHINT_IHEXIHEX IHEX
#define PPUTLIMPL_ARITHHINT_IHEXIDEC IDEC
#define PPUTLIMPL_ARITHHINT_IDECUHEX UDEC
#define PPUTLIMPL_ARITHHINT_IDECUDEC UDEC
#define PPUTLIMPL_ARITHHINT_IDECIHEX IDEC
#define PPUTLIMPL_ARITHHINT_IDECIDEC IDEC

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [impl.hexhex_traits]
/// --------------------
/// [internal] hex pair (hex##hex) traits
#define PPUTLIMPL_HEXHEX_TRAITS(                                   \
    /* 00|01|...|FE|FF, LT|AND|OR|XOR|SUB0|SUB1|ADD0|ADD1 */ v, t) \
  PPUTLIMPL_HEXHEX_TRAITS_o(t, PTL_XCAT(PPUTLIMPL_HEXHEX_TRAITS_, v))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_HEXHEX_TRAITS_o(t, ...) PPUTLIMPL_HEXHEX_TRAITS_##t(__VA_ARGS__)

#define PPUTLIMPL_HEXHEX_TRAITS_ADD1(a, b, c, d, e, f, g, h, i, j, k, l, ...) k, l
#define PPUTLIMPL_HEXHEX_TRAITS_ADD0(a, b, c, d, e, f, g, h, i, j, ...)       i, j
#define PPUTLIMPL_HEXHEX_TRAITS_SUB1(a, b, c, d, e, f, g, h, ...)             g, h
#define PPUTLIMPL_HEXHEX_TRAITS_SUB0(a, b, c, d, e, f, ...)                   e, f
#define PPUTLIMPL_HEXHEX_TRAITS_XOR(a, b, c, d, ...)                          d
#define PPUTLIMPL_HEXHEX_TRAITS_OR(a, b, c, ...)                              c
#define PPUTLIMPL_HEXHEX_TRAITS_AND(a, b, ...)                                b
#define PPUTLIMPL_HEXHEX_TRAITS_LT(a, ...)                                    a
#define PPUTLIMPL_HEXHEX_TRAITS_IS(_, ...)                                    /* -> bool */ \
  PPUTLIMPL_HEXHEX_TRAITS_IS_0##__VA_OPT__(1)
#define PPUTLIMPL_HEXHEX_TRAITS_IS_01 1
#define PPUTLIMPL_HEXHEX_TRAITS_IS_0  0

/// lt, and, or, xor, sub0-1 carry, sub0-1, add0-1 carry, add0-1
#define PPUTLIMPL_HEXHEX_TRAITS_FF 0, F, F, 0, 0, 0, 1, F, 1, E, 1, F
#define PPUTLIMPL_HEXHEX_TRAITS_FE 0, E, F, 1, 0, 1, 0, 0, 1, D, 1, E
#define PPUTLIMPL_HEXHEX_TRAITS_FD 0, D, F, 2, 0, 2, 0, 1, 1, C, 1, D
#define PPUTLIMPL_HEXHEX_TRAITS_FC 0, C, F, 3, 0, 3, 0, 2, 1, B, 1, C
#define PPUTLIMPL_HEXHEX_TRAITS_FB 0, B, F, 4, 0, 4, 0, 3, 1, A, 1, B
#define PPUTLIMPL_HEXHEX_TRAITS_FA 0, A, F, 5, 0, 5, 0, 4, 1, 9, 1, A
#define PPUTLIMPL_HEXHEX_TRAITS_F9 0, 9, F, 6, 0, 6, 0, 5, 1, 8, 1, 9
#define PPUTLIMPL_HEXHEX_TRAITS_F8 0, 8, F, 7, 0, 7, 0, 6, 1, 7, 1, 8
#define PPUTLIMPL_HEXHEX_TRAITS_F7 0, 7, F, 8, 0, 8, 0, 7, 1, 6, 1, 7
#define PPUTLIMPL_HEXHEX_TRAITS_F6 0, 6, F, 9, 0, 9, 0, 8, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_TRAITS_F5 0, 5, F, A, 0, A, 0, 9, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_TRAITS_F4 0, 4, F, B, 0, B, 0, A, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_TRAITS_F3 0, 3, F, C, 0, C, 0, B, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_TRAITS_F2 0, 2, F, D, 0, D, 0, C, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_TRAITS_F1 0, 1, F, E, 0, E, 0, D, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_TRAITS_F0 0, 0, F, F, 0, F, 0, E, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_TRAITS_EF 1, E, F, 1, 1, F, 1, E, 1, D, 1, E
#define PPUTLIMPL_HEXHEX_TRAITS_EE 0, E, E, 0, 0, 0, 1, F, 1, C, 1, D
#define PPUTLIMPL_HEXHEX_TRAITS_ED 0, C, F, 3, 0, 1, 0, 0, 1, B, 1, C
#define PPUTLIMPL_HEXHEX_TRAITS_EC 0, C, E, 2, 0, 2, 0, 1, 1, A, 1, B
#define PPUTLIMPL_HEXHEX_TRAITS_EB 0, A, F, 5, 0, 3, 0, 2, 1, 9, 1, A
#define PPUTLIMPL_HEXHEX_TRAITS_EA 0, A, E, 4, 0, 4, 0, 3, 1, 8, 1, 9
#define PPUTLIMPL_HEXHEX_TRAITS_E9 0, 8, F, 7, 0, 5, 0, 4, 1, 7, 1, 8
#define PPUTLIMPL_HEXHEX_TRAITS_E8 0, 8, E, 6, 0, 6, 0, 5, 1, 6, 1, 7
#define PPUTLIMPL_HEXHEX_TRAITS_E7 0, 6, F, 9, 0, 7, 0, 6, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_TRAITS_E6 0, 6, E, 8, 0, 8, 0, 7, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_TRAITS_E5 0, 4, F, B, 0, 9, 0, 8, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_TRAITS_E4 0, 4, E, A, 0, A, 0, 9, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_TRAITS_E3 0, 2, F, D, 0, B, 0, A, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_TRAITS_E2 0, 2, E, C, 0, C, 0, B, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_TRAITS_E1 0, 0, F, F, 0, D, 0, C, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_TRAITS_E0 0, 0, E, E, 0, E, 0, D, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_TRAITS_DF 1, D, F, 2, 1, E, 1, D, 1, C, 1, D
#define PPUTLIMPL_HEXHEX_TRAITS_DE 1, C, F, 3, 1, F, 1, E, 1, B, 1, C
#define PPUTLIMPL_HEXHEX_TRAITS_DD 0, D, D, 0, 0, 0, 1, F, 1, A, 1, B
#define PPUTLIMPL_HEXHEX_TRAITS_DC 0, C, D, 1, 0, 1, 0, 0, 1, 9, 1, A
#define PPUTLIMPL_HEXHEX_TRAITS_DB 0, 9, F, 6, 0, 2, 0, 1, 1, 8, 1, 9
#define PPUTLIMPL_HEXHEX_TRAITS_DA 0, 8, F, 7, 0, 3, 0, 2, 1, 7, 1, 8
#define PPUTLIMPL_HEXHEX_TRAITS_D9 0, 9, D, 4, 0, 4, 0, 3, 1, 6, 1, 7
#define PPUTLIMPL_HEXHEX_TRAITS_D8 0, 8, D, 5, 0, 5, 0, 4, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_TRAITS_D7 0, 5, F, A, 0, 6, 0, 5, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_TRAITS_D6 0, 4, F, B, 0, 7, 0, 6, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_TRAITS_D5 0, 5, D, 8, 0, 8, 0, 7, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_TRAITS_D4 0, 4, D, 9, 0, 9, 0, 8, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_TRAITS_D3 0, 1, F, E, 0, A, 0, 9, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_TRAITS_D2 0, 0, F, F, 0, B, 0, A, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_TRAITS_D1 0, 1, D, C, 0, C, 0, B, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_TRAITS_D0 0, 0, D, D, 0, D, 0, C, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_TRAITS_CF 1, C, F, 3, 1, D, 1, C, 1, B, 1, C
#define PPUTLIMPL_HEXHEX_TRAITS_CE 1, C, E, 2, 1, E, 1, D, 1, A, 1, B
#define PPUTLIMPL_HEXHEX_TRAITS_CD 1, C, D, 1, 1, F, 1, E, 1, 9, 1, A
#define PPUTLIMPL_HEXHEX_TRAITS_CC 0, C, C, 0, 0, 0, 1, F, 1, 8, 1, 9
#define PPUTLIMPL_HEXHEX_TRAITS_CB 0, 8, F, 7, 0, 1, 0, 0, 1, 7, 1, 8
#define PPUTLIMPL_HEXHEX_TRAITS_CA 0, 8, E, 6, 0, 2, 0, 1, 1, 6, 1, 7
#define PPUTLIMPL_HEXHEX_TRAITS_C9 0, 8, D, 5, 0, 3, 0, 2, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_TRAITS_C8 0, 8, C, 4, 0, 4, 0, 3, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_TRAITS_C7 0, 4, F, B, 0, 5, 0, 4, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_TRAITS_C6 0, 4, E, A, 0, 6, 0, 5, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_TRAITS_C5 0, 4, D, 9, 0, 7, 0, 6, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_TRAITS_C4 0, 4, C, 8, 0, 8, 0, 7, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_TRAITS_C3 0, 0, F, F, 0, 9, 0, 8, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_TRAITS_C2 0, 0, E, E, 0, A, 0, 9, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_TRAITS_C1 0, 0, D, D, 0, B, 0, A, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_TRAITS_C0 0, 0, C, C, 0, C, 0, B, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_TRAITS_BF 1, B, F, 4, 1, C, 1, B, 1, A, 1, B
#define PPUTLIMPL_HEXHEX_TRAITS_BE 1, A, F, 5, 1, D, 1, C, 1, 9, 1, A
#define PPUTLIMPL_HEXHEX_TRAITS_BD 1, 9, F, 6, 1, E, 1, D, 1, 8, 1, 9
#define PPUTLIMPL_HEXHEX_TRAITS_BC 1, 8, F, 7, 1, F, 1, E, 1, 7, 1, 8
#define PPUTLIMPL_HEXHEX_TRAITS_BB 0, B, B, 0, 0, 0, 1, F, 1, 6, 1, 7
#define PPUTLIMPL_HEXHEX_TRAITS_BA 0, A, B, 1, 0, 1, 0, 0, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_TRAITS_B9 0, 9, B, 2, 0, 2, 0, 1, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_TRAITS_B8 0, 8, B, 3, 0, 3, 0, 2, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_TRAITS_B7 0, 3, F, C, 0, 4, 0, 3, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_TRAITS_B6 0, 2, F, D, 0, 5, 0, 4, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_TRAITS_B5 0, 1, F, E, 0, 6, 0, 5, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_TRAITS_B4 0, 0, F, F, 0, 7, 0, 6, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_TRAITS_B3 0, 3, B, 8, 0, 8, 0, 7, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_TRAITS_B2 0, 2, B, 9, 0, 9, 0, 8, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_TRAITS_B1 0, 1, B, A, 0, A, 0, 9, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_TRAITS_B0 0, 0, B, B, 0, B, 0, A, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_TRAITS_AF 1, A, F, 5, 1, B, 1, A, 1, 9, 1, A
#define PPUTLIMPL_HEXHEX_TRAITS_AE 1, A, E, 4, 1, C, 1, B, 1, 8, 1, 9
#define PPUTLIMPL_HEXHEX_TRAITS_AD 1, 8, F, 7, 1, D, 1, C, 1, 7, 1, 8
#define PPUTLIMPL_HEXHEX_TRAITS_AC 1, 8, E, 6, 1, E, 1, D, 1, 6, 1, 7
#define PPUTLIMPL_HEXHEX_TRAITS_AB 1, A, B, 1, 1, F, 1, E, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_TRAITS_AA 0, A, A, 0, 0, 0, 1, F, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_TRAITS_A9 0, 8, B, 3, 0, 1, 0, 0, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_TRAITS_A8 0, 8, A, 2, 0, 2, 0, 1, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_TRAITS_A7 0, 2, F, D, 0, 3, 0, 2, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_TRAITS_A6 0, 2, E, C, 0, 4, 0, 3, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_TRAITS_A5 0, 0, F, F, 0, 5, 0, 4, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_TRAITS_A4 0, 0, E, E, 0, 6, 0, 5, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_TRAITS_A3 0, 2, B, 9, 0, 7, 0, 6, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_TRAITS_A2 0, 2, A, 8, 0, 8, 0, 7, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_TRAITS_A1 0, 0, B, B, 0, 9, 0, 8, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_TRAITS_A0 0, 0, A, A, 0, A, 0, 9, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_TRAITS_9F 1, 9, F, 6, 1, A, 1, 9, 1, 8, 1, 9
#define PPUTLIMPL_HEXHEX_TRAITS_9E 1, 8, F, 7, 1, B, 1, A, 1, 7, 1, 8
#define PPUTLIMPL_HEXHEX_TRAITS_9D 1, 9, D, 4, 1, C, 1, B, 1, 6, 1, 7
#define PPUTLIMPL_HEXHEX_TRAITS_9C 1, 8, D, 5, 1, D, 1, C, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_TRAITS_9B 1, 9, B, 2, 1, E, 1, D, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_TRAITS_9A 1, 8, B, 3, 1, F, 1, E, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_TRAITS_99 0, 9, 9, 0, 0, 0, 1, F, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_TRAITS_98 0, 8, 9, 1, 0, 1, 0, 0, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_TRAITS_97 0, 1, F, E, 0, 2, 0, 1, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_TRAITS_96 0, 0, F, F, 0, 3, 0, 2, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_TRAITS_95 0, 1, D, C, 0, 4, 0, 3, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_TRAITS_94 0, 0, D, D, 0, 5, 0, 4, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_TRAITS_93 0, 1, B, A, 0, 6, 0, 5, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_TRAITS_92 0, 0, B, B, 0, 7, 0, 6, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_TRAITS_91 0, 1, 9, 8, 0, 8, 0, 7, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_TRAITS_90 0, 0, 9, 9, 0, 9, 0, 8, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_TRAITS_8F 1, 8, F, 7, 1, 9, 1, 8, 1, 7, 1, 8
#define PPUTLIMPL_HEXHEX_TRAITS_8E 1, 8, E, 6, 1, A, 1, 9, 1, 6, 1, 7
#define PPUTLIMPL_HEXHEX_TRAITS_8D 1, 8, D, 5, 1, B, 1, A, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_TRAITS_8C 1, 8, C, 4, 1, C, 1, B, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_TRAITS_8B 1, 8, B, 3, 1, D, 1, C, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_TRAITS_8A 1, 8, A, 2, 1, E, 1, D, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_TRAITS_89 1, 8, 9, 1, 1, F, 1, E, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_TRAITS_88 0, 8, 8, 0, 0, 0, 1, F, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_TRAITS_87 0, 0, F, F, 0, 1, 0, 0, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_TRAITS_86 0, 0, E, E, 0, 2, 0, 1, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_TRAITS_85 0, 0, D, D, 0, 3, 0, 2, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_TRAITS_84 0, 0, C, C, 0, 4, 0, 3, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_TRAITS_83 0, 0, B, B, 0, 5, 0, 4, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_TRAITS_82 0, 0, A, A, 0, 6, 0, 5, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_TRAITS_81 0, 0, 9, 9, 0, 7, 0, 6, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_TRAITS_80 0, 0, 8, 8, 0, 8, 0, 7, 0, 8, 0, 9
#define PPUTLIMPL_HEXHEX_TRAITS_7F 1, 7, F, 8, 1, 8, 1, 7, 1, 6, 1, 7
#define PPUTLIMPL_HEXHEX_TRAITS_7E 1, 6, F, 9, 1, 9, 1, 8, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_TRAITS_7D 1, 5, F, A, 1, A, 1, 9, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_TRAITS_7C 1, 4, F, B, 1, B, 1, A, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_TRAITS_7B 1, 3, F, C, 1, C, 1, B, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_TRAITS_7A 1, 2, F, D, 1, D, 1, C, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_TRAITS_79 1, 1, F, E, 1, E, 1, D, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_TRAITS_78 1, 0, F, F, 1, F, 1, E, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_TRAITS_77 0, 7, 7, 0, 0, 0, 1, F, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_TRAITS_76 0, 6, 7, 1, 0, 1, 0, 0, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_TRAITS_75 0, 5, 7, 2, 0, 2, 0, 1, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_TRAITS_74 0, 4, 7, 3, 0, 3, 0, 2, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_TRAITS_73 0, 3, 7, 4, 0, 4, 0, 3, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_TRAITS_72 0, 2, 7, 5, 0, 5, 0, 4, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_TRAITS_71 0, 1, 7, 6, 0, 6, 0, 5, 0, 8, 0, 9
#define PPUTLIMPL_HEXHEX_TRAITS_70 0, 0, 7, 7, 0, 7, 0, 6, 0, 7, 0, 8
#define PPUTLIMPL_HEXHEX_TRAITS_6F 1, 6, F, 9, 1, 7, 1, 6, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_TRAITS_6E 1, 6, E, 8, 1, 8, 1, 7, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_TRAITS_6D 1, 4, F, B, 1, 9, 1, 8, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_TRAITS_6C 1, 4, E, A, 1, A, 1, 9, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_TRAITS_6B 1, 2, F, D, 1, B, 1, A, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_TRAITS_6A 1, 2, E, C, 1, C, 1, B, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_TRAITS_69 1, 0, F, F, 1, D, 1, C, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_TRAITS_68 1, 0, E, E, 1, E, 1, D, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_TRAITS_67 1, 6, 7, 1, 1, F, 1, E, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_TRAITS_66 0, 6, 6, 0, 0, 0, 1, F, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_TRAITS_65 0, 4, 7, 3, 0, 1, 0, 0, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_TRAITS_64 0, 4, 6, 2, 0, 2, 0, 1, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_TRAITS_63 0, 2, 7, 5, 0, 3, 0, 2, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_TRAITS_62 0, 2, 6, 4, 0, 4, 0, 3, 0, 8, 0, 9
#define PPUTLIMPL_HEXHEX_TRAITS_61 0, 0, 7, 7, 0, 5, 0, 4, 0, 7, 0, 8
#define PPUTLIMPL_HEXHEX_TRAITS_60 0, 0, 6, 6, 0, 6, 0, 5, 0, 6, 0, 7
#define PPUTLIMPL_HEXHEX_TRAITS_5F 1, 5, F, A, 1, 6, 1, 5, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_TRAITS_5E 1, 4, F, B, 1, 7, 1, 6, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_TRAITS_5D 1, 5, D, 8, 1, 8, 1, 7, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_TRAITS_5C 1, 4, D, 9, 1, 9, 1, 8, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_TRAITS_5B 1, 1, F, E, 1, A, 1, 9, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_TRAITS_5A 1, 0, F, F, 1, B, 1, A, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_TRAITS_59 1, 1, D, C, 1, C, 1, B, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_TRAITS_58 1, 0, D, D, 1, D, 1, C, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_TRAITS_57 1, 5, 7, 2, 1, E, 1, D, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_TRAITS_56 1, 4, 7, 3, 1, F, 1, E, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_TRAITS_55 0, 5, 5, 0, 0, 0, 1, F, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_TRAITS_54 0, 4, 5, 1, 0, 1, 0, 0, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_TRAITS_53 0, 1, 7, 6, 0, 2, 0, 1, 0, 8, 0, 9
#define PPUTLIMPL_HEXHEX_TRAITS_52 0, 0, 7, 7, 0, 3, 0, 2, 0, 7, 0, 8
#define PPUTLIMPL_HEXHEX_TRAITS_51 0, 1, 5, 4, 0, 4, 0, 3, 0, 6, 0, 7
#define PPUTLIMPL_HEXHEX_TRAITS_50 0, 0, 5, 5, 0, 5, 0, 4, 0, 5, 0, 6
#define PPUTLIMPL_HEXHEX_TRAITS_4F 1, 4, F, B, 1, 5, 1, 4, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_TRAITS_4E 1, 4, E, A, 1, 6, 1, 5, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_TRAITS_4D 1, 4, D, 9, 1, 7, 1, 6, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_TRAITS_4C 1, 4, C, 8, 1, 8, 1, 7, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_TRAITS_4B 1, 0, F, F, 1, 9, 1, 8, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_TRAITS_4A 1, 0, E, E, 1, A, 1, 9, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_TRAITS_49 1, 0, D, D, 1, B, 1, A, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_TRAITS_48 1, 0, C, C, 1, C, 1, B, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_TRAITS_47 1, 4, 7, 3, 1, D, 1, C, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_TRAITS_46 1, 4, 6, 2, 1, E, 1, D, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_TRAITS_45 1, 4, 5, 1, 1, F, 1, E, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_TRAITS_44 0, 4, 4, 0, 0, 0, 1, F, 0, 8, 0, 9
#define PPUTLIMPL_HEXHEX_TRAITS_43 0, 0, 7, 7, 0, 1, 0, 0, 0, 7, 0, 8
#define PPUTLIMPL_HEXHEX_TRAITS_42 0, 0, 6, 6, 0, 2, 0, 1, 0, 6, 0, 7
#define PPUTLIMPL_HEXHEX_TRAITS_41 0, 0, 5, 5, 0, 3, 0, 2, 0, 5, 0, 6
#define PPUTLIMPL_HEXHEX_TRAITS_40 0, 0, 4, 4, 0, 4, 0, 3, 0, 4, 0, 5
#define PPUTLIMPL_HEXHEX_TRAITS_3F 1, 3, F, C, 1, 4, 1, 3, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_TRAITS_3E 1, 2, F, D, 1, 5, 1, 4, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_TRAITS_3D 1, 1, F, E, 1, 6, 1, 5, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_TRAITS_3C 1, 0, F, F, 1, 7, 1, 6, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_TRAITS_3B 1, 3, B, 8, 1, 8, 1, 7, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_TRAITS_3A 1, 2, B, 9, 1, 9, 1, 8, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_TRAITS_39 1, 1, B, A, 1, A, 1, 9, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_TRAITS_38 1, 0, B, B, 1, B, 1, A, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_TRAITS_37 1, 3, 7, 4, 1, C, 1, B, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_TRAITS_36 1, 2, 7, 5, 1, D, 1, C, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_TRAITS_35 1, 1, 7, 6, 1, E, 1, D, 0, 8, 0, 9
#define PPUTLIMPL_HEXHEX_TRAITS_34 1, 0, 7, 7, 1, F, 1, E, 0, 7, 0, 8
#define PPUTLIMPL_HEXHEX_TRAITS_33 0, 3, 3, 0, 0, 0, 1, F, 0, 6, 0, 7
#define PPUTLIMPL_HEXHEX_TRAITS_32 0, 2, 3, 1, 0, 1, 0, 0, 0, 5, 0, 6
#define PPUTLIMPL_HEXHEX_TRAITS_31 0, 1, 3, 2, 0, 2, 0, 1, 0, 4, 0, 5
#define PPUTLIMPL_HEXHEX_TRAITS_30 0, 0, 3, 3, 0, 3, 0, 2, 0, 3, 0, 4
#define PPUTLIMPL_HEXHEX_TRAITS_2F 1, 2, F, D, 1, 3, 1, 2, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_TRAITS_2E 1, 2, E, C, 1, 4, 1, 3, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_TRAITS_2D 1, 0, F, F, 1, 5, 1, 4, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_TRAITS_2C 1, 0, E, E, 1, 6, 1, 5, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_TRAITS_2B 1, 2, B, 9, 1, 7, 1, 6, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_TRAITS_2A 1, 2, A, 8, 1, 8, 1, 7, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_TRAITS_29 1, 0, B, B, 1, 9, 1, 8, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_TRAITS_28 1, 0, A, A, 1, A, 1, 9, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_TRAITS_27 1, 2, 7, 5, 1, B, 1, A, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_TRAITS_26 1, 2, 6, 4, 1, C, 1, B, 0, 8, 0, 9
#define PPUTLIMPL_HEXHEX_TRAITS_25 1, 0, 7, 7, 1, D, 1, C, 0, 7, 0, 8
#define PPUTLIMPL_HEXHEX_TRAITS_24 1, 0, 6, 6, 1, E, 1, D, 0, 6, 0, 7
#define PPUTLIMPL_HEXHEX_TRAITS_23 1, 2, 3, 1, 1, F, 1, E, 0, 5, 0, 6
#define PPUTLIMPL_HEXHEX_TRAITS_22 0, 2, 2, 0, 0, 0, 1, F, 0, 4, 0, 5
#define PPUTLIMPL_HEXHEX_TRAITS_21 0, 0, 3, 3, 0, 1, 0, 0, 0, 3, 0, 4
#define PPUTLIMPL_HEXHEX_TRAITS_20 0, 0, 2, 2, 0, 2, 0, 1, 0, 2, 0, 3
#define PPUTLIMPL_HEXHEX_TRAITS_1F 1, 1, F, E, 1, 2, 1, 1, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_TRAITS_1E 1, 0, F, F, 1, 3, 1, 2, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_TRAITS_1D 1, 1, D, C, 1, 4, 1, 3, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_TRAITS_1C 1, 0, D, D, 1, 5, 1, 4, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_TRAITS_1B 1, 1, B, A, 1, 6, 1, 5, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_TRAITS_1A 1, 0, B, B, 1, 7, 1, 6, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_TRAITS_19 1, 1, 9, 8, 1, 8, 1, 7, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_TRAITS_18 1, 0, 9, 9, 1, 9, 1, 8, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_TRAITS_17 1, 1, 7, 6, 1, A, 1, 9, 0, 8, 0, 9
#define PPUTLIMPL_HEXHEX_TRAITS_16 1, 0, 7, 7, 1, B, 1, A, 0, 7, 0, 8
#define PPUTLIMPL_HEXHEX_TRAITS_15 1, 1, 5, 4, 1, C, 1, B, 0, 6, 0, 7
#define PPUTLIMPL_HEXHEX_TRAITS_14 1, 0, 5, 5, 1, D, 1, C, 0, 5, 0, 6
#define PPUTLIMPL_HEXHEX_TRAITS_13 1, 1, 3, 2, 1, E, 1, D, 0, 4, 0, 5
#define PPUTLIMPL_HEXHEX_TRAITS_12 1, 0, 3, 3, 1, F, 1, E, 0, 3, 0, 4
#define PPUTLIMPL_HEXHEX_TRAITS_11 0, 1, 1, 0, 0, 0, 1, F, 0, 2, 0, 3
#define PPUTLIMPL_HEXHEX_TRAITS_10 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 2
#define PPUTLIMPL_HEXHEX_TRAITS_0F 1, 0, F, F, 1, 1, 1, 0, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_TRAITS_0E 1, 0, E, E, 1, 2, 1, 1, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_TRAITS_0D 1, 0, D, D, 1, 3, 1, 2, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_TRAITS_0C 1, 0, C, C, 1, 4, 1, 3, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_TRAITS_0B 1, 0, B, B, 1, 5, 1, 4, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_TRAITS_0A 1, 0, A, A, 1, 6, 1, 5, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_TRAITS_09 1, 0, 9, 9, 1, 7, 1, 6, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_TRAITS_08 1, 0, 8, 8, 1, 8, 1, 7, 0, 8, 0, 9
#define PPUTLIMPL_HEXHEX_TRAITS_07 1, 0, 7, 7, 1, 9, 1, 8, 0, 7, 0, 8
#define PPUTLIMPL_HEXHEX_TRAITS_06 1, 0, 6, 6, 1, A, 1, 9, 0, 6, 0, 7
#define PPUTLIMPL_HEXHEX_TRAITS_05 1, 0, 5, 5, 1, B, 1, A, 0, 5, 0, 6
#define PPUTLIMPL_HEXHEX_TRAITS_04 1, 0, 4, 4, 1, C, 1, B, 0, 4, 0, 5
#define PPUTLIMPL_HEXHEX_TRAITS_03 1, 0, 3, 3, 1, D, 1, C, 0, 3, 0, 4
#define PPUTLIMPL_HEXHEX_TRAITS_02 1, 0, 2, 2, 1, E, 1, D, 0, 2, 0, 3
#define PPUTLIMPL_HEXHEX_TRAITS_01 1, 0, 1, 1, 1, F, 1, E, 0, 1, 0, 2
#define PPUTLIMPL_HEXHEX_TRAITS_00 0, 0, 0, 0, 0, 0, 1, F, 0, 0, 0, 1

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [impl.xarithhint]
/// -----------------
/// [internal] two-operand arithmetic cast hint.
///
/// returns UDEC|UHEX if either operand is
/// UDEC|UHEX, UDEC|IDEC if either operand
/// is UDEC|IDEC, and UHEX|IHEX otherwise.
#define PPUTLIMPL_XARITHHINT(/* IDEC|IHEX|UDEC|UHEX, IDEC|IHEX|UDEC|UHEX */...) \
  PPUTLIMPL_ARITHHINT(__VA_ARGS__)

/// [impl.hex_cat]
/// --------------
/// [internal] concatenates hexadecimal digits to a uhex expression.
#define PPUTLIMPL_HEX_CAT(/* 0|1|...|E|F... */...) PPUTLIMPL_HEX_CAT_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_HEX_CAT_o(a, b) 0x##a##b##u

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [impl.hex_traits]
/// -----------------
/// [internal] hex traits
#define PPUTLIMPL_HEX_TRAITS(/* 0|1|...|E|F, NOT|DEC0|DEC1|INC0|INC1|NYBL|BITS */ v, t) \
  PPUTLIMPL_HEX_TRAITS_o(t, PTL_XCAT(PPUTLIMPL_HEX_TRAITS_, v))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_HEX_TRAITS_o(t, ...) PPUTLIMPL_HEX_TRAITS_##t(__VA_ARGS__)
#define PPUTLIMPL_HEX_TRAITS_BITS(n, d0c, d0, d1c, d1, i0c, i0, i1c, i1, ny, \
                                  ...) /* -> ...bool */                      \
  __VA_ARGS__
#define PPUTLIMPL_HEX_TRAITS_NYBL(n, d0c, d0, d1c, d1, i0c, i0, i1c, i1, ny, \
                                  ...) /* -> nybl */                         \
  ny
#define PPUTLIMPL_HEX_TRAITS_INC1(n, d0c, d0, d1c, d1, i0c, i0, i1c, i1, \
                                  ...) /* -> bool, hex */                \
  i1c, i1
#define PPUTLIMPL_HEX_TRAITS_INC0(n, d0c, d0, d1c, d1, i0c, i0, ...) /* -> bool, hex */ \
  i0c, i0
#define PPUTLIMPL_HEX_TRAITS_DEC1(n, d0c, d0, d1c, d1, ...) /* -> bool, hex */ d1c, d1
#define PPUTLIMPL_HEX_TRAITS_DEC0(n, d0c, d0, ...)          /* -> bool, hex */ d0c, d0
#define PPUTLIMPL_HEX_TRAITS_NOT(n, ...)                    /* -> hex */ n

/// not, dec0carry, dec0, dec1carry, dec1, inc0carry, inc0, inc1carry, inc1, nybl, ...bits
#define PPUTLIMPL_HEX_TRAITS_F 0, 0, F, 0, E, 0, F, 1, 0, 1111, 1, 1, 1, 1
#define PPUTLIMPL_HEX_TRAITS_E 1, 0, E, 0, D, 0, E, 0, F, 1110, 1, 1, 1, 0
#define PPUTLIMPL_HEX_TRAITS_D 2, 0, D, 0, C, 0, D, 0, E, 1101, 1, 1, 0, 1
#define PPUTLIMPL_HEX_TRAITS_C 3, 0, C, 0, B, 0, C, 0, D, 1100, 1, 1, 0, 0
#define PPUTLIMPL_HEX_TRAITS_B 4, 0, B, 0, A, 0, B, 0, C, 1011, 1, 0, 1, 1
#define PPUTLIMPL_HEX_TRAITS_A 5, 0, A, 0, 9, 0, A, 0, B, 1010, 1, 0, 1, 0
#define PPUTLIMPL_HEX_TRAITS_9 6, 0, 9, 0, 8, 0, 9, 0, A, 1001, 1, 0, 0, 1
#define PPUTLIMPL_HEX_TRAITS_8 7, 0, 8, 0, 7, 0, 8, 0, 9, 1000, 1, 0, 0, 0
#define PPUTLIMPL_HEX_TRAITS_7 8, 0, 7, 0, 6, 0, 7, 0, 8, 0111, 0, 1, 1, 1
#define PPUTLIMPL_HEX_TRAITS_6 9, 0, 6, 0, 5, 0, 6, 0, 7, 0110, 0, 1, 1, 0
#define PPUTLIMPL_HEX_TRAITS_5 A, 0, 5, 0, 4, 0, 5, 0, 6, 0101, 0, 1, 0, 1
#define PPUTLIMPL_HEX_TRAITS_4 B, 0, 4, 0, 3, 0, 4, 0, 5, 0100, 0, 1, 0, 0
#define PPUTLIMPL_HEX_TRAITS_3 C, 0, 3, 0, 2, 0, 3, 0, 4, 0011, 0, 0, 1, 1
#define PPUTLIMPL_HEX_TRAITS_2 D, 0, 2, 0, 1, 0, 2, 0, 3, 0010, 0, 0, 1, 0
#define PPUTLIMPL_HEX_TRAITS_1 E, 0, 1, 0, 0, 0, 1, 0, 2, 0001, 0, 0, 0, 1
#define PPUTLIMPL_HEX_TRAITS_0 F, 0, 0, 1, F, 0, 0, 0, 1, 0000, 0, 0, 0, 0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [impl.uhex_traits]
/// ------------------
/// [internal] uhex traits
#define PPUTLIMPL_UHEX_TRAITS(/* uhex, UDEC|IHEX|ICAST|ILTZ|BNOT|HDUMP */ v, t) \
  PPUTLIMPL_UHEX_TRAITS_o(t, PTL_XCAT(PPUTLIMPL_UHEX_TRAITS_, v))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_UHEX_TRAITS_o(t, ...)                 PPUTLIMPL_UHEX_TRAITS_##t(__VA_ARGS__)
#define PPUTLIMPL_UHEX_TRAITS_HDUMP(u, h, i, z, b, ...) /* -> 0|1|...|E|F... */ \
  __VA_ARGS__
#define PPUTLIMPL_UHEX_TRAITS_BNOT(u, h, i, z, b, ...) /* -> uhex */ b
#define PPUTLIMPL_UHEX_TRAITS_ILTZ(u, h, i, z, ...)    /* -> bool */ z
#define PPUTLIMPL_UHEX_TRAITS_ICAST(u, h, i, ...)      /* -> idec|ihex */ i
#define PPUTLIMPL_UHEX_TRAITS_IHEX(u, h, ...)          /* -> ihex */ h
#define PPUTLIMPL_UHEX_TRAITS_UDEC(u, ...)             /* -> udec */ u

/// UDEC, IHEX, ICAST, ILTZ, BNOT, HDUMP...
#define PPUTLIMPL_UHEX_TRAITS_0xFFu 255u, 0xFF, 0xFF, 1, 0x00u, F, F
#define PPUTLIMPL_UHEX_TRAITS_0xFEu 254u, 0xFE, 0xFE, 1, 0x01u, F, E
#define PPUTLIMPL_UHEX_TRAITS_0xFDu 253u, 0xFD, 0xFD, 1, 0x02u, F, D
#define PPUTLIMPL_UHEX_TRAITS_0xFCu 252u, 0xFC, 0xFC, 1, 0x03u, F, C
#define PPUTLIMPL_UHEX_TRAITS_0xFBu 251u, 0xFB, 0xFB, 1, 0x04u, F, B
#define PPUTLIMPL_UHEX_TRAITS_0xFAu 250u, 0xFA, 0xFA, 1, 0x05u, F, A
#define PPUTLIMPL_UHEX_TRAITS_0xF9u 249u, 0xF9, 0xF9, 1, 0x06u, F, 9
#define PPUTLIMPL_UHEX_TRAITS_0xF8u 248u, 0xF8, 0xF8, 1, 0x07u, F, 8
#define PPUTLIMPL_UHEX_TRAITS_0xF7u 247u, 0xF7, 0xF7, 1, 0x08u, F, 7
#define PPUTLIMPL_UHEX_TRAITS_0xF6u 246u, 0xF6, 0xF6, 1, 0x09u, F, 6
#define PPUTLIMPL_UHEX_TRAITS_0xF5u 245u, 0xF5, 0xF5, 1, 0x0Au, F, 5
#define PPUTLIMPL_UHEX_TRAITS_0xF4u 244u, 0xF4, 0xF4, 1, 0x0Bu, F, 4
#define PPUTLIMPL_UHEX_TRAITS_0xF3u 243u, 0xF3, 0xF3, 1, 0x0Cu, F, 3
#define PPUTLIMPL_UHEX_TRAITS_0xF2u 242u, 0xF2, 0xF2, 1, 0x0Du, F, 2
#define PPUTLIMPL_UHEX_TRAITS_0xF1u 241u, 0xF1, 0xF1, 1, 0x0Eu, F, 1
#define PPUTLIMPL_UHEX_TRAITS_0xF0u 240u, 0xF0, 0xF0, 1, 0x0Fu, F, 0
#define PPUTLIMPL_UHEX_TRAITS_0xEFu 239u, 0xEF, 0xEF, 1, 0x10u, E, F
#define PPUTLIMPL_UHEX_TRAITS_0xEEu 238u, 0xEE, 0xEE, 1, 0x11u, E, E
#define PPUTLIMPL_UHEX_TRAITS_0xEDu 237u, 0xED, 0xED, 1, 0x12u, E, D
#define PPUTLIMPL_UHEX_TRAITS_0xECu 236u, 0xEC, 0xEC, 1, 0x13u, E, C
#define PPUTLIMPL_UHEX_TRAITS_0xEBu 235u, 0xEB, 0xEB, 1, 0x14u, E, B
#define PPUTLIMPL_UHEX_TRAITS_0xEAu 234u, 0xEA, 0xEA, 1, 0x15u, E, A
#define PPUTLIMPL_UHEX_TRAITS_0xE9u 233u, 0xE9, 0xE9, 1, 0x16u, E, 9
#define PPUTLIMPL_UHEX_TRAITS_0xE8u 232u, 0xE8, 0xE8, 1, 0x17u, E, 8
#define PPUTLIMPL_UHEX_TRAITS_0xE7u 231u, 0xE7, 0xE7, 1, 0x18u, E, 7
#define PPUTLIMPL_UHEX_TRAITS_0xE6u 230u, 0xE6, 0xE6, 1, 0x19u, E, 6
#define PPUTLIMPL_UHEX_TRAITS_0xE5u 229u, 0xE5, 0xE5, 1, 0x1Au, E, 5
#define PPUTLIMPL_UHEX_TRAITS_0xE4u 228u, 0xE4, 0xE4, 1, 0x1Bu, E, 4
#define PPUTLIMPL_UHEX_TRAITS_0xE3u 227u, 0xE3, 0xE3, 1, 0x1Cu, E, 3
#define PPUTLIMPL_UHEX_TRAITS_0xE2u 226u, 0xE2, 0xE2, 1, 0x1Du, E, 2
#define PPUTLIMPL_UHEX_TRAITS_0xE1u 225u, 0xE1, 0xE1, 1, 0x1Eu, E, 1
#define PPUTLIMPL_UHEX_TRAITS_0xE0u 224u, 0xE0, 0xE0, 1, 0x1Fu, E, 0
#define PPUTLIMPL_UHEX_TRAITS_0xDFu 223u, 0xDF, 0xDF, 1, 0x20u, D, F
#define PPUTLIMPL_UHEX_TRAITS_0xDEu 222u, 0xDE, 0xDE, 1, 0x21u, D, E
#define PPUTLIMPL_UHEX_TRAITS_0xDDu 221u, 0xDD, 0xDD, 1, 0x22u, D, D
#define PPUTLIMPL_UHEX_TRAITS_0xDCu 220u, 0xDC, 0xDC, 1, 0x23u, D, C
#define PPUTLIMPL_UHEX_TRAITS_0xDBu 219u, 0xDB, 0xDB, 1, 0x24u, D, B
#define PPUTLIMPL_UHEX_TRAITS_0xDAu 218u, 0xDA, 0xDA, 1, 0x25u, D, A
#define PPUTLIMPL_UHEX_TRAITS_0xD9u 217u, 0xD9, 0xD9, 1, 0x26u, D, 9
#define PPUTLIMPL_UHEX_TRAITS_0xD8u 216u, 0xD8, 0xD8, 1, 0x27u, D, 8
#define PPUTLIMPL_UHEX_TRAITS_0xD7u 215u, 0xD7, 0xD7, 1, 0x28u, D, 7
#define PPUTLIMPL_UHEX_TRAITS_0xD6u 214u, 0xD6, 0xD6, 1, 0x29u, D, 6
#define PPUTLIMPL_UHEX_TRAITS_0xD5u 213u, 0xD5, 0xD5, 1, 0x2Au, D, 5
#define PPUTLIMPL_UHEX_TRAITS_0xD4u 212u, 0xD4, 0xD4, 1, 0x2Bu, D, 4
#define PPUTLIMPL_UHEX_TRAITS_0xD3u 211u, 0xD3, 0xD3, 1, 0x2Cu, D, 3
#define PPUTLIMPL_UHEX_TRAITS_0xD2u 210u, 0xD2, 0xD2, 1, 0x2Du, D, 2
#define PPUTLIMPL_UHEX_TRAITS_0xD1u 209u, 0xD1, 0xD1, 1, 0x2Eu, D, 1
#define PPUTLIMPL_UHEX_TRAITS_0xD0u 208u, 0xD0, 0xD0, 1, 0x2Fu, D, 0
#define PPUTLIMPL_UHEX_TRAITS_0xCFu 207u, 0xCF, 0xCF, 1, 0x30u, C, F
#define PPUTLIMPL_UHEX_TRAITS_0xCEu 206u, 0xCE, 0xCE, 1, 0x31u, C, E
#define PPUTLIMPL_UHEX_TRAITS_0xCDu 205u, 0xCD, 0xCD, 1, 0x32u, C, D
#define PPUTLIMPL_UHEX_TRAITS_0xCCu 204u, 0xCC, 0xCC, 1, 0x33u, C, C
#define PPUTLIMPL_UHEX_TRAITS_0xCBu 203u, 0xCB, 0xCB, 1, 0x34u, C, B
#define PPUTLIMPL_UHEX_TRAITS_0xCAu 202u, 0xCA, 0xCA, 1, 0x35u, C, A
#define PPUTLIMPL_UHEX_TRAITS_0xC9u 201u, 0xC9, 0xC9, 1, 0x36u, C, 9
#define PPUTLIMPL_UHEX_TRAITS_0xC8u 200u, 0xC8, 0xC8, 1, 0x37u, C, 8
#define PPUTLIMPL_UHEX_TRAITS_0xC7u 199u, 0xC7, 0xC7, 1, 0x38u, C, 7
#define PPUTLIMPL_UHEX_TRAITS_0xC6u 198u, 0xC6, 0xC6, 1, 0x39u, C, 6
#define PPUTLIMPL_UHEX_TRAITS_0xC5u 197u, 0xC5, 0xC5, 1, 0x3Au, C, 5
#define PPUTLIMPL_UHEX_TRAITS_0xC4u 196u, 0xC4, 0xC4, 1, 0x3Bu, C, 4
#define PPUTLIMPL_UHEX_TRAITS_0xC3u 195u, 0xC3, 0xC3, 1, 0x3Cu, C, 3
#define PPUTLIMPL_UHEX_TRAITS_0xC2u 194u, 0xC2, 0xC2, 1, 0x3Du, C, 2
#define PPUTLIMPL_UHEX_TRAITS_0xC1u 193u, 0xC1, 0xC1, 1, 0x3Eu, C, 1
#define PPUTLIMPL_UHEX_TRAITS_0xC0u 192u, 0xC0, 0xC0, 1, 0x3Fu, C, 0
#define PPUTLIMPL_UHEX_TRAITS_0xBFu 191u, 0xBF, 0xBF, 1, 0x40u, B, F
#define PPUTLIMPL_UHEX_TRAITS_0xBEu 190u, 0xBE, 0xBE, 1, 0x41u, B, E
#define PPUTLIMPL_UHEX_TRAITS_0xBDu 189u, 0xBD, 0xBD, 1, 0x42u, B, D
#define PPUTLIMPL_UHEX_TRAITS_0xBCu 188u, 0xBC, 0xBC, 1, 0x43u, B, C
#define PPUTLIMPL_UHEX_TRAITS_0xBBu 187u, 0xBB, 0xBB, 1, 0x44u, B, B
#define PPUTLIMPL_UHEX_TRAITS_0xBAu 186u, 0xBA, 0xBA, 1, 0x45u, B, A
#define PPUTLIMPL_UHEX_TRAITS_0xB9u 185u, 0xB9, 0xB9, 1, 0x46u, B, 9
#define PPUTLIMPL_UHEX_TRAITS_0xB8u 184u, 0xB8, 0xB8, 1, 0x47u, B, 8
#define PPUTLIMPL_UHEX_TRAITS_0xB7u 183u, 0xB7, 0xB7, 1, 0x48u, B, 7
#define PPUTLIMPL_UHEX_TRAITS_0xB6u 182u, 0xB6, 0xB6, 1, 0x49u, B, 6
#define PPUTLIMPL_UHEX_TRAITS_0xB5u 181u, 0xB5, 0xB5, 1, 0x4Au, B, 5
#define PPUTLIMPL_UHEX_TRAITS_0xB4u 180u, 0xB4, 0xB4, 1, 0x4Bu, B, 4
#define PPUTLIMPL_UHEX_TRAITS_0xB3u 179u, 0xB3, 0xB3, 1, 0x4Cu, B, 3
#define PPUTLIMPL_UHEX_TRAITS_0xB2u 178u, 0xB2, 0xB2, 1, 0x4Du, B, 2
#define PPUTLIMPL_UHEX_TRAITS_0xB1u 177u, 0xB1, 0xB1, 1, 0x4Eu, B, 1
#define PPUTLIMPL_UHEX_TRAITS_0xB0u 176u, 0xB0, 0xB0, 1, 0x4Fu, B, 0
#define PPUTLIMPL_UHEX_TRAITS_0xAFu 175u, 0xAF, 0xAF, 1, 0x50u, A, F
#define PPUTLIMPL_UHEX_TRAITS_0xAEu 174u, 0xAE, 0xAE, 1, 0x51u, A, E
#define PPUTLIMPL_UHEX_TRAITS_0xADu 173u, 0xAD, 0xAD, 1, 0x52u, A, D
#define PPUTLIMPL_UHEX_TRAITS_0xACu 172u, 0xAC, 0xAC, 1, 0x53u, A, C
#define PPUTLIMPL_UHEX_TRAITS_0xABu 171u, 0xAB, 0xAB, 1, 0x54u, A, B
#define PPUTLIMPL_UHEX_TRAITS_0xAAu 170u, 0xAA, 0xAA, 1, 0x55u, A, A
#define PPUTLIMPL_UHEX_TRAITS_0xA9u 169u, 0xA9, 0xA9, 1, 0x56u, A, 9
#define PPUTLIMPL_UHEX_TRAITS_0xA8u 168u, 0xA8, 0xA8, 1, 0x57u, A, 8
#define PPUTLIMPL_UHEX_TRAITS_0xA7u 167u, 0xA7, 0xA7, 1, 0x58u, A, 7
#define PPUTLIMPL_UHEX_TRAITS_0xA6u 166u, 0xA6, 0xA6, 1, 0x59u, A, 6
#define PPUTLIMPL_UHEX_TRAITS_0xA5u 165u, 0xA5, 0xA5, 1, 0x5Au, A, 5
#define PPUTLIMPL_UHEX_TRAITS_0xA4u 164u, 0xA4, 0xA4, 1, 0x5Bu, A, 4
#define PPUTLIMPL_UHEX_TRAITS_0xA3u 163u, 0xA3, 0xA3, 1, 0x5Cu, A, 3
#define PPUTLIMPL_UHEX_TRAITS_0xA2u 162u, 0xA2, 0xA2, 1, 0x5Du, A, 2
#define PPUTLIMPL_UHEX_TRAITS_0xA1u 161u, 0xA1, 0xA1, 1, 0x5Eu, A, 1
#define PPUTLIMPL_UHEX_TRAITS_0xA0u 160u, 0xA0, 0xA0, 1, 0x5Fu, A, 0
#define PPUTLIMPL_UHEX_TRAITS_0x9Fu 159u, 0x9F, 0x9F, 1, 0x60u, 9, F
#define PPUTLIMPL_UHEX_TRAITS_0x9Eu 158u, 0x9E, 0x9E, 1, 0x61u, 9, E
#define PPUTLIMPL_UHEX_TRAITS_0x9Du 157u, 0x9D, 0x9D, 1, 0x62u, 9, D
#define PPUTLIMPL_UHEX_TRAITS_0x9Cu 156u, 0x9C, 0x9C, 1, 0x63u, 9, C
#define PPUTLIMPL_UHEX_TRAITS_0x9Bu 155u, 0x9B, 0x9B, 1, 0x64u, 9, B
#define PPUTLIMPL_UHEX_TRAITS_0x9Au 154u, 0x9A, 0x9A, 1, 0x65u, 9, A
#define PPUTLIMPL_UHEX_TRAITS_0x99u 153u, 0x99, 0x99, 1, 0x66u, 9, 9
#define PPUTLIMPL_UHEX_TRAITS_0x98u 152u, 0x98, 0x98, 1, 0x67u, 9, 8
#define PPUTLIMPL_UHEX_TRAITS_0x97u 151u, 0x97, 0x97, 1, 0x68u, 9, 7
#define PPUTLIMPL_UHEX_TRAITS_0x96u 150u, 0x96, 0x96, 1, 0x69u, 9, 6
#define PPUTLIMPL_UHEX_TRAITS_0x95u 149u, 0x95, 0x95, 1, 0x6Au, 9, 5
#define PPUTLIMPL_UHEX_TRAITS_0x94u 148u, 0x94, 0x94, 1, 0x6Bu, 9, 4
#define PPUTLIMPL_UHEX_TRAITS_0x93u 147u, 0x93, 0x93, 1, 0x6Cu, 9, 3
#define PPUTLIMPL_UHEX_TRAITS_0x92u 146u, 0x92, 0x92, 1, 0x6Du, 9, 2
#define PPUTLIMPL_UHEX_TRAITS_0x91u 145u, 0x91, 0x91, 1, 0x6Eu, 9, 1
#define PPUTLIMPL_UHEX_TRAITS_0x90u 144u, 0x90, 0x90, 1, 0x6Fu, 9, 0
#define PPUTLIMPL_UHEX_TRAITS_0x8Fu 143u, 0x8F, 0x8F, 1, 0x70u, 8, F
#define PPUTLIMPL_UHEX_TRAITS_0x8Eu 142u, 0x8E, 0x8E, 1, 0x71u, 8, E
#define PPUTLIMPL_UHEX_TRAITS_0x8Du 141u, 0x8D, 0x8D, 1, 0x72u, 8, D
#define PPUTLIMPL_UHEX_TRAITS_0x8Cu 140u, 0x8C, 0x8C, 1, 0x73u, 8, C
#define PPUTLIMPL_UHEX_TRAITS_0x8Bu 139u, 0x8B, 0x8B, 1, 0x74u, 8, B
#define PPUTLIMPL_UHEX_TRAITS_0x8Au 138u, 0x8A, 0x8A, 1, 0x75u, 8, A
#define PPUTLIMPL_UHEX_TRAITS_0x89u 137u, 0x89, 0x89, 1, 0x76u, 8, 9
#define PPUTLIMPL_UHEX_TRAITS_0x88u 136u, 0x88, 0x88, 1, 0x77u, 8, 8
#define PPUTLIMPL_UHEX_TRAITS_0x87u 135u, 0x87, 0x87, 1, 0x78u, 8, 7
#define PPUTLIMPL_UHEX_TRAITS_0x86u 134u, 0x86, 0x86, 1, 0x79u, 8, 6
#define PPUTLIMPL_UHEX_TRAITS_0x85u 133u, 0x85, 0x85, 1, 0x7Au, 8, 5
#define PPUTLIMPL_UHEX_TRAITS_0x84u 132u, 0x84, 0x84, 1, 0x7Bu, 8, 4
#define PPUTLIMPL_UHEX_TRAITS_0x83u 131u, 0x83, 0x83, 1, 0x7Cu, 8, 3
#define PPUTLIMPL_UHEX_TRAITS_0x82u 130u, 0x82, 0x82, 1, 0x7Du, 8, 2
#define PPUTLIMPL_UHEX_TRAITS_0x81u 129u, 0x81, 0x81, 1, 0x7Eu, 8, 1
#define PPUTLIMPL_UHEX_TRAITS_0x80u 128u, 0x80, 0x80, 1, 0x7Fu, 8, 0
#define PPUTLIMPL_UHEX_TRAITS_0x7Fu 127u, 0x7F, 127, 0, 0x80u, 7, F
#define PPUTLIMPL_UHEX_TRAITS_0x7Eu 126u, 0x7E, 126, 0, 0x81u, 7, E
#define PPUTLIMPL_UHEX_TRAITS_0x7Du 125u, 0x7D, 125, 0, 0x82u, 7, D
#define PPUTLIMPL_UHEX_TRAITS_0x7Cu 124u, 0x7C, 124, 0, 0x83u, 7, C
#define PPUTLIMPL_UHEX_TRAITS_0x7Bu 123u, 0x7B, 123, 0, 0x84u, 7, B
#define PPUTLIMPL_UHEX_TRAITS_0x7Au 122u, 0x7A, 122, 0, 0x85u, 7, A
#define PPUTLIMPL_UHEX_TRAITS_0x79u 121u, 0x79, 121, 0, 0x86u, 7, 9
#define PPUTLIMPL_UHEX_TRAITS_0x78u 120u, 0x78, 120, 0, 0x87u, 7, 8
#define PPUTLIMPL_UHEX_TRAITS_0x77u 119u, 0x77, 119, 0, 0x88u, 7, 7
#define PPUTLIMPL_UHEX_TRAITS_0x76u 118u, 0x76, 118, 0, 0x89u, 7, 6
#define PPUTLIMPL_UHEX_TRAITS_0x75u 117u, 0x75, 117, 0, 0x8Au, 7, 5
#define PPUTLIMPL_UHEX_TRAITS_0x74u 116u, 0x74, 116, 0, 0x8Bu, 7, 4
#define PPUTLIMPL_UHEX_TRAITS_0x73u 115u, 0x73, 115, 0, 0x8Cu, 7, 3
#define PPUTLIMPL_UHEX_TRAITS_0x72u 114u, 0x72, 114, 0, 0x8Du, 7, 2
#define PPUTLIMPL_UHEX_TRAITS_0x71u 113u, 0x71, 113, 0, 0x8Eu, 7, 1
#define PPUTLIMPL_UHEX_TRAITS_0x70u 112u, 0x70, 112, 0, 0x8Fu, 7, 0
#define PPUTLIMPL_UHEX_TRAITS_0x6Fu 111u, 0x6F, 111, 0, 0x90u, 6, F
#define PPUTLIMPL_UHEX_TRAITS_0x6Eu 110u, 0x6E, 110, 0, 0x91u, 6, E
#define PPUTLIMPL_UHEX_TRAITS_0x6Du 109u, 0x6D, 109, 0, 0x92u, 6, D
#define PPUTLIMPL_UHEX_TRAITS_0x6Cu 108u, 0x6C, 108, 0, 0x93u, 6, C
#define PPUTLIMPL_UHEX_TRAITS_0x6Bu 107u, 0x6B, 107, 0, 0x94u, 6, B
#define PPUTLIMPL_UHEX_TRAITS_0x6Au 106u, 0x6A, 106, 0, 0x95u, 6, A
#define PPUTLIMPL_UHEX_TRAITS_0x69u 105u, 0x69, 105, 0, 0x96u, 6, 9
#define PPUTLIMPL_UHEX_TRAITS_0x68u 104u, 0x68, 104, 0, 0x97u, 6, 8
#define PPUTLIMPL_UHEX_TRAITS_0x67u 103u, 0x67, 103, 0, 0x98u, 6, 7
#define PPUTLIMPL_UHEX_TRAITS_0x66u 102u, 0x66, 102, 0, 0x99u, 6, 6
#define PPUTLIMPL_UHEX_TRAITS_0x65u 101u, 0x65, 101, 0, 0x9Au, 6, 5
#define PPUTLIMPL_UHEX_TRAITS_0x64u 100u, 0x64, 100, 0, 0x9Bu, 6, 4
#define PPUTLIMPL_UHEX_TRAITS_0x63u 99u, 0x63, 99, 0, 0x9Cu, 6, 3
#define PPUTLIMPL_UHEX_TRAITS_0x62u 98u, 0x62, 98, 0, 0x9Du, 6, 2
#define PPUTLIMPL_UHEX_TRAITS_0x61u 97u, 0x61, 97, 0, 0x9Eu, 6, 1
#define PPUTLIMPL_UHEX_TRAITS_0x60u 96u, 0x60, 96, 0, 0x9Fu, 6, 0
#define PPUTLIMPL_UHEX_TRAITS_0x5Fu 95u, 0x5F, 95, 0, 0xA0u, 5, F
#define PPUTLIMPL_UHEX_TRAITS_0x5Eu 94u, 0x5E, 94, 0, 0xA1u, 5, E
#define PPUTLIMPL_UHEX_TRAITS_0x5Du 93u, 0x5D, 93, 0, 0xA2u, 5, D
#define PPUTLIMPL_UHEX_TRAITS_0x5Cu 92u, 0x5C, 92, 0, 0xA3u, 5, C
#define PPUTLIMPL_UHEX_TRAITS_0x5Bu 91u, 0x5B, 91, 0, 0xA4u, 5, B
#define PPUTLIMPL_UHEX_TRAITS_0x5Au 90u, 0x5A, 90, 0, 0xA5u, 5, A
#define PPUTLIMPL_UHEX_TRAITS_0x59u 89u, 0x59, 89, 0, 0xA6u, 5, 9
#define PPUTLIMPL_UHEX_TRAITS_0x58u 88u, 0x58, 88, 0, 0xA7u, 5, 8
#define PPUTLIMPL_UHEX_TRAITS_0x57u 87u, 0x57, 87, 0, 0xA8u, 5, 7
#define PPUTLIMPL_UHEX_TRAITS_0x56u 86u, 0x56, 86, 0, 0xA9u, 5, 6
#define PPUTLIMPL_UHEX_TRAITS_0x55u 85u, 0x55, 85, 0, 0xAAu, 5, 5
#define PPUTLIMPL_UHEX_TRAITS_0x54u 84u, 0x54, 84, 0, 0xABu, 5, 4
#define PPUTLIMPL_UHEX_TRAITS_0x53u 83u, 0x53, 83, 0, 0xACu, 5, 3
#define PPUTLIMPL_UHEX_TRAITS_0x52u 82u, 0x52, 82, 0, 0xADu, 5, 2
#define PPUTLIMPL_UHEX_TRAITS_0x51u 81u, 0x51, 81, 0, 0xAEu, 5, 1
#define PPUTLIMPL_UHEX_TRAITS_0x50u 80u, 0x50, 80, 0, 0xAFu, 5, 0
#define PPUTLIMPL_UHEX_TRAITS_0x4Fu 79u, 0x4F, 79, 0, 0xB0u, 4, F
#define PPUTLIMPL_UHEX_TRAITS_0x4Eu 78u, 0x4E, 78, 0, 0xB1u, 4, E
#define PPUTLIMPL_UHEX_TRAITS_0x4Du 77u, 0x4D, 77, 0, 0xB2u, 4, D
#define PPUTLIMPL_UHEX_TRAITS_0x4Cu 76u, 0x4C, 76, 0, 0xB3u, 4, C
#define PPUTLIMPL_UHEX_TRAITS_0x4Bu 75u, 0x4B, 75, 0, 0xB4u, 4, B
#define PPUTLIMPL_UHEX_TRAITS_0x4Au 74u, 0x4A, 74, 0, 0xB5u, 4, A
#define PPUTLIMPL_UHEX_TRAITS_0x49u 73u, 0x49, 73, 0, 0xB6u, 4, 9
#define PPUTLIMPL_UHEX_TRAITS_0x48u 72u, 0x48, 72, 0, 0xB7u, 4, 8
#define PPUTLIMPL_UHEX_TRAITS_0x47u 71u, 0x47, 71, 0, 0xB8u, 4, 7
#define PPUTLIMPL_UHEX_TRAITS_0x46u 70u, 0x46, 70, 0, 0xB9u, 4, 6
#define PPUTLIMPL_UHEX_TRAITS_0x45u 69u, 0x45, 69, 0, 0xBAu, 4, 5
#define PPUTLIMPL_UHEX_TRAITS_0x44u 68u, 0x44, 68, 0, 0xBBu, 4, 4
#define PPUTLIMPL_UHEX_TRAITS_0x43u 67u, 0x43, 67, 0, 0xBCu, 4, 3
#define PPUTLIMPL_UHEX_TRAITS_0x42u 66u, 0x42, 66, 0, 0xBDu, 4, 2
#define PPUTLIMPL_UHEX_TRAITS_0x41u 65u, 0x41, 65, 0, 0xBEu, 4, 1
#define PPUTLIMPL_UHEX_TRAITS_0x40u 64u, 0x40, 64, 0, 0xBFu, 4, 0
#define PPUTLIMPL_UHEX_TRAITS_0x3Fu 63u, 0x3F, 63, 0, 0xC0u, 3, F
#define PPUTLIMPL_UHEX_TRAITS_0x3Eu 62u, 0x3E, 62, 0, 0xC1u, 3, E
#define PPUTLIMPL_UHEX_TRAITS_0x3Du 61u, 0x3D, 61, 0, 0xC2u, 3, D
#define PPUTLIMPL_UHEX_TRAITS_0x3Cu 60u, 0x3C, 60, 0, 0xC3u, 3, C
#define PPUTLIMPL_UHEX_TRAITS_0x3Bu 59u, 0x3B, 59, 0, 0xC4u, 3, B
#define PPUTLIMPL_UHEX_TRAITS_0x3Au 58u, 0x3A, 58, 0, 0xC5u, 3, A
#define PPUTLIMPL_UHEX_TRAITS_0x39u 57u, 0x39, 57, 0, 0xC6u, 3, 9
#define PPUTLIMPL_UHEX_TRAITS_0x38u 56u, 0x38, 56, 0, 0xC7u, 3, 8
#define PPUTLIMPL_UHEX_TRAITS_0x37u 55u, 0x37, 55, 0, 0xC8u, 3, 7
#define PPUTLIMPL_UHEX_TRAITS_0x36u 54u, 0x36, 54, 0, 0xC9u, 3, 6
#define PPUTLIMPL_UHEX_TRAITS_0x35u 53u, 0x35, 53, 0, 0xCAu, 3, 5
#define PPUTLIMPL_UHEX_TRAITS_0x34u 52u, 0x34, 52, 0, 0xCBu, 3, 4
#define PPUTLIMPL_UHEX_TRAITS_0x33u 51u, 0x33, 51, 0, 0xCCu, 3, 3
#define PPUTLIMPL_UHEX_TRAITS_0x32u 50u, 0x32, 50, 0, 0xCDu, 3, 2
#define PPUTLIMPL_UHEX_TRAITS_0x31u 49u, 0x31, 49, 0, 0xCEu, 3, 1
#define PPUTLIMPL_UHEX_TRAITS_0x30u 48u, 0x30, 48, 0, 0xCFu, 3, 0
#define PPUTLIMPL_UHEX_TRAITS_0x2Fu 47u, 0x2F, 47, 0, 0xD0u, 2, F
#define PPUTLIMPL_UHEX_TRAITS_0x2Eu 46u, 0x2E, 46, 0, 0xD1u, 2, E
#define PPUTLIMPL_UHEX_TRAITS_0x2Du 45u, 0x2D, 45, 0, 0xD2u, 2, D
#define PPUTLIMPL_UHEX_TRAITS_0x2Cu 44u, 0x2C, 44, 0, 0xD3u, 2, C
#define PPUTLIMPL_UHEX_TRAITS_0x2Bu 43u, 0x2B, 43, 0, 0xD4u, 2, B
#define PPUTLIMPL_UHEX_TRAITS_0x2Au 42u, 0x2A, 42, 0, 0xD5u, 2, A
#define PPUTLIMPL_UHEX_TRAITS_0x29u 41u, 0x29, 41, 0, 0xD6u, 2, 9
#define PPUTLIMPL_UHEX_TRAITS_0x28u 40u, 0x28, 40, 0, 0xD7u, 2, 8
#define PPUTLIMPL_UHEX_TRAITS_0x27u 39u, 0x27, 39, 0, 0xD8u, 2, 7
#define PPUTLIMPL_UHEX_TRAITS_0x26u 38u, 0x26, 38, 0, 0xD9u, 2, 6
#define PPUTLIMPL_UHEX_TRAITS_0x25u 37u, 0x25, 37, 0, 0xDAu, 2, 5
#define PPUTLIMPL_UHEX_TRAITS_0x24u 36u, 0x24, 36, 0, 0xDBu, 2, 4
#define PPUTLIMPL_UHEX_TRAITS_0x23u 35u, 0x23, 35, 0, 0xDCu, 2, 3
#define PPUTLIMPL_UHEX_TRAITS_0x22u 34u, 0x22, 34, 0, 0xDDu, 2, 2
#define PPUTLIMPL_UHEX_TRAITS_0x21u 33u, 0x21, 33, 0, 0xDEu, 2, 1
#define PPUTLIMPL_UHEX_TRAITS_0x20u 32u, 0x20, 32, 0, 0xDFu, 2, 0
#define PPUTLIMPL_UHEX_TRAITS_0x1Fu 31u, 0x1F, 31, 0, 0xE0u, 1, F
#define PPUTLIMPL_UHEX_TRAITS_0x1Eu 30u, 0x1E, 30, 0, 0xE1u, 1, E
#define PPUTLIMPL_UHEX_TRAITS_0x1Du 29u, 0x1D, 29, 0, 0xE2u, 1, D
#define PPUTLIMPL_UHEX_TRAITS_0x1Cu 28u, 0x1C, 28, 0, 0xE3u, 1, C
#define PPUTLIMPL_UHEX_TRAITS_0x1Bu 27u, 0x1B, 27, 0, 0xE4u, 1, B
#define PPUTLIMPL_UHEX_TRAITS_0x1Au 26u, 0x1A, 26, 0, 0xE5u, 1, A
#define PPUTLIMPL_UHEX_TRAITS_0x19u 25u, 0x19, 25, 0, 0xE6u, 1, 9
#define PPUTLIMPL_UHEX_TRAITS_0x18u 24u, 0x18, 24, 0, 0xE7u, 1, 8
#define PPUTLIMPL_UHEX_TRAITS_0x17u 23u, 0x17, 23, 0, 0xE8u, 1, 7
#define PPUTLIMPL_UHEX_TRAITS_0x16u 22u, 0x16, 22, 0, 0xE9u, 1, 6
#define PPUTLIMPL_UHEX_TRAITS_0x15u 21u, 0x15, 21, 0, 0xEAu, 1, 5
#define PPUTLIMPL_UHEX_TRAITS_0x14u 20u, 0x14, 20, 0, 0xEBu, 1, 4
#define PPUTLIMPL_UHEX_TRAITS_0x13u 19u, 0x13, 19, 0, 0xECu, 1, 3
#define PPUTLIMPL_UHEX_TRAITS_0x12u 18u, 0x12, 18, 0, 0xEDu, 1, 2
#define PPUTLIMPL_UHEX_TRAITS_0x11u 17u, 0x11, 17, 0, 0xEEu, 1, 1
#define PPUTLIMPL_UHEX_TRAITS_0x10u 16u, 0x10, 16, 0, 0xEFu, 1, 0
#define PPUTLIMPL_UHEX_TRAITS_0x0Fu 15u, 0x0F, 15, 0, 0xF0u, 0, F
#define PPUTLIMPL_UHEX_TRAITS_0x0Eu 14u, 0x0E, 14, 0, 0xF1u, 0, E
#define PPUTLIMPL_UHEX_TRAITS_0x0Du 13u, 0x0D, 13, 0, 0xF2u, 0, D
#define PPUTLIMPL_UHEX_TRAITS_0x0Cu 12u, 0x0C, 12, 0, 0xF3u, 0, C
#define PPUTLIMPL_UHEX_TRAITS_0x0Bu 11u, 0x0B, 11, 0, 0xF4u, 0, B
#define PPUTLIMPL_UHEX_TRAITS_0x0Au 10u, 0x0A, 10, 0, 0xF5u, 0, A
#define PPUTLIMPL_UHEX_TRAITS_0x09u 9u, 0x09, 9, 0, 0xF6u, 0, 9
#define PPUTLIMPL_UHEX_TRAITS_0x08u 8u, 0x08, 8, 0, 0xF7u, 0, 8
#define PPUTLIMPL_UHEX_TRAITS_0x07u 7u, 0x07, 7, 0, 0xF8u, 0, 7
#define PPUTLIMPL_UHEX_TRAITS_0x06u 6u, 0x06, 6, 0, 0xF9u, 0, 6
#define PPUTLIMPL_UHEX_TRAITS_0x05u 5u, 0x05, 5, 0, 0xFAu, 0, 5
#define PPUTLIMPL_UHEX_TRAITS_0x04u 4u, 0x04, 4, 0, 0xFBu, 0, 4
#define PPUTLIMPL_UHEX_TRAITS_0x03u 3u, 0x03, 3, 0, 0xFCu, 0, 3
#define PPUTLIMPL_UHEX_TRAITS_0x02u 2u, 0x02, 2, 0, 0xFDu, 0, 2
#define PPUTLIMPL_UHEX_TRAITS_0x01u 1u, 0x01, 1, 0, 0xFEu, 0, 1
#define PPUTLIMPL_UHEX_TRAITS_0x00u 0u, 0x00, 0, 0, 0xFFu, 0, 0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

// vim: fdm=marker:fmr={{{,}}}

#endif

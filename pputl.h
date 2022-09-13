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
//    pputl is a powerful C++ preprocessor utilities library that features    //
//    many high-level constructs including integers and range algorithms.     //
//                                                                            //
//    Speed, safety, and flexibility are its primary goals.                   //
//                                                                            //
//    Algorithms operate using an inline recursion technique that requires    //
//    fewer expansions than mutually-recursive patterns and can be nested.    //
//    Utilities are provided for performing arbitrary, nestable recursion.    //
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
//           list    none  object  tuple   atom   enum                        //
//           idec    bool  ihex    udec    uhex   int                         //
//           uint    word  size    offset  array  map                         //
//           pqueue  set   stack   queue   range  any                         //
//       ‐ type traits                                     [traits, range]    //
//           is_list    is_none   is_object  is_tuple   is_atom   is_enum     //
//           is_idec    is_bool   is_ihex    is_udec    is_uhex   is_int      //
//           is_uint    is_word   is_size    is_offset  is_array  is_map      //
//           is_pqueue  is_set    is_stack   is_queue   is_range  is_any      //
//           typeof     countof   sizeof     itemsof    is_empty              //
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
//           bisect     unite      head      tail      push_front             //
//           push_back  pop_front  pop_back  get_item  set_item               //
//           ins_item   del_item   front     back      slice                  //
//           splice     push       pop                                        //
//       ‐ list generation                                          [algo]    //
//           seq  repeat  gen_lp  gen_rp                                      //
//       ‐ list manipulation                                        [algo]    //
//           rev     transform_lp  transform_rp  shift                        //
//           rotate  reduce_lp     reduce_rp                                  //
//     ◆ metaprogramming utilities                                            //
//       ‐ expansion control and tracing                            [meta]    //
//           x  xx_lp  xx_rp  lp  rp  xtrace  xtrace_read                     //
//       ‐ inline recursive stack construction                      [meta]    //
//           recur_lp  recur_rp                                               //
//     ◆ configuration details                                    [config]    //
//           build    word_size  bit_length  int_min                          //
//           int_max  uint_max   size_max    ofs_max                          //
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
//    Supported integer modes:                                                //
//                                                                            //
//       word_size=1    ⋮   4-bit integers  ⋮  ~? KiB                         //
//       word_size=2    ⋮   8-bit integers  ⋮  ~? KiB                         //
//     [ word_size=3    ⋮  12-bit integers  ⋮  ~? MiB  (default) ]            //
//       word_size=4 †  ⋮  16-bit integers  ⋮  ~? MiB                         //
//                                        ________________________________    //
//                                        †: requires cpp20_deflimit=false    //
//                                                                            //
//    By default, pputl is fully compliant with the C++20 standard,           //
//    which defines the following implementation limits in [implimits]:       //
//                                                                            //
//     ‐ Macro identifiers simultaneously                                     //
//       defined in one translation unit: [65536].                            //
//     ‐ Parameters in one macro definition: [256].                           //
//     ‐ Arguments in one macro invocation: [256].                            //
//                                                                            //
//    Settings are available to ignore these limits, but support for sizes    //
//    and macro definition counts higher than the standard is variable.       //
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
//    to its parameter type; see [type] for constructor documentation.        //
//                                                                            //
//    The following schema depicts the pputl type hierarchy, starting with    //
//    "list" as the most basic description of any  __VA_ARGS__ expression.    //
//    Examples and descriptions represent the default configuration.          //
//                                                                            //
//     list: tokens potentially delimited by non-parenthesized commas         //
//      ├╴none:   nothing; an absence of pp-tokens (an empty list)            //
//      ├╴object: a non-empty list without separators (exactly one thing)     //
//      │  ├╴tuple: a parenthesized list [e.g ()] [e.g. (a, b, , )]           //
//      │  ├╴atom:  a non-parenthesized object                                //
//      │  │  ├╴enum<v0|v1|...>: an atom matching a defined enumeration       //
//      │  │  │  ├╴idec: enum<0|1|2|...|2045|2046|2047>                       //
//      │  │  │  │  └╴bool: enum<0|1>                                         //
//      │  │  │  ├╴ihex: enum<0x000|0x001|...|0xFFE|0xFFF>                    //
//      │  │  │  ├╴udec: enum<0u|1u|2u|...|4093u|4094u|4095u>                 //
//      │  │  │  └╴uhex: enum<0x000u|0x001u|...|0xFFEu|0xFFFu>                //
//      │  │  ├╴int:  <union> idec|ihex; a signed 2s-compl integer            //
//      │  │  ├╴uint: <union> udec|uhex; an unsigned integer                  //
//      │  │  ├╴word: <union> int|uint;  any kind of integer                  //
//      │  │  │  ├╴size:   any non-negative word up to size_max               //
//      │  │  │  └╴offset: any word whose absolute value is a valid size      //
//      │  │  └╴array: an encapsulated, sized sequence of any                 //
//      │  │     ├╴map: a sorted mapping of words or enums to any             //
//      │  │     │  └╴pqueue: a priority queue of any                         //
//      │  │     ├╴set:   a set of words or enums                             //
//      │  │     ├╴stack: a LIFO stack of any                                 //
//      │  │     └╴queue: a FIFO queue of any                                 //
//      │  └╴range: <union> tuple|array; a structured range of any            //
//      └╴any: <union> none|object; nothing or exactly one thing (any arg)    //
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
//    Negative ints  cannot be represented in decimal due to concatenation    //
//    restrictions. Arithmetic and bitwise functions attempt to cast their    //
//    results in the same form as their input, but will always return ihex    //
//    when an idec input becomes negative.  Decimal representations may be   ///
//    generated for pasting using fmt.c_int.                                ////
//                                                                         /////
///////////////////////////////////////////////////////////////////////////// */

/// [config.build]
/// --------------
/// the build number of this pputl release (ISO8601).
#define PTL_BUILD /* -> <c++ int> */ 20220913

/// [config.word_size]
/// ------------------
/// the number of hex digits used to represent pputl integers.
/// hex representations of integers are fixed at this length.
/// see the readme code generation section to configure.
#define PTL_WORD_SIZE /* -> udec&size */ 3u

/// [config.bit_length]
/// -------------------
/// the number of bits that can be used to represent pputl integers.
/// see the readme code generation section to configure.
#define PTL_BIT_LENGTH /* -> udec&size */ 12u

/// [config.int_min]
/// ----------------
/// the minimum value of a pputl signed int.
/// only representable as hex. see type.int for details.
#define PTL_INT_MIN /* -> ihex */ 0x800

/// [config.int_max]
/// ----------------
/// the maximum value of a pputl signed int.
#define PTL_INT_MAX /* -> idec */ 2047

/// [config.uint_max]
/// -----------------
/// the maximum value of a pputl unsigned int.
#define PTL_UINT_MAX /* -> udec */ 4095u

/// [config.size_max]
/// -----------------
/// the maximum number of arguments bounded by the C++20 standard.
/// set to min(255, uint_max) unless built with cpp20_arglimit=false
/// (which sets size_max to uint_max).
#define PTL_SIZE_MAX /* -> udec&size */ 255u

/// [lang.eat]
/// ----------
/// eats arguments; return nothing.
#define PTL_EAT(...) /* -> none */

/// [lang.esc]
/// ----------
/// escapes parentheses. identity function.
///
/// PTL_ESC ()        // <nothing>
/// PTL_ESC (a, b, c) // a, b, c
#define PTL_ESC(...) /* -> any... */ __VA_ARGS__

/// [lang.cat]
/// ----------
/// immediately concatenates a with b.
/// must provide at least one arg.
/// args must be compatible with the ## operator.
///
/// PTL_CAT(foo, bar)          // foobar
/// PTL_CAT(foo, PTL_EAT(bar)) // fooPTL_EAT(bar)
#define PTL_CAT(/* a: any, b: any */ a, b) /* -> any */ a##b

/// [lang.xcat]
/// -----------
/// concatenates two args after an expansion.
/// args must be compatible with the ## operator.
///
/// PTL_XCAT(foo, bar)          // foobar
/// PTL_XCAT(foo, PTL_EAT(bar)) // foo
/// PTL_XCAT(,)                 // <nothing>
#define PTL_XCAT(/* a: any, b: any */...) /* -> any */ PTL_CAT(__VA_ARGS__)

/// [lang.str]
/// ----------
/// immediately stringizes args.
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

/// [lang.first]
/// ------------
/// immediately returns the first argument.
/// must have at least one argument.
///
/// useful for operating directly on __VA_ARGS__ or
/// for quickly retrieving the first tuple element
/// using an identity function such as PTL_ESC.
///
/// e.g. PTL_FIRST(__VA_ARGS__)
///      PTL_ESC(PTL_FIRST tup)
#define PTL_FIRST(/* first: any, ...rest: any */ _, ...) /* -> any */ _

/// [lang.xfirst]
/// -------------
/// returns the first argument.
///
/// PTL_XFIRST()     // <nothing>
/// PTL_XFIRST(, )   // <nothing>
/// PTL_XFIRST(a)    // a
/// PTL_XFIRST(a, b) // a
#define PTL_XFIRST(/* any... */...) /* -> any */ __VA_OPT__(PTL_FIRST(__VA_ARGS__))

/// [lang.rest]
/// -----------
/// immediately returns all args except for the first.
/// must have at least one argument.
///
/// useful for operating directly on __VA_ARGS__ or
/// for quickly retrieving the rest tuple elements
/// using an identity function such as PTL_ESC.
///
/// e.g. PTL_REST(__VA_ARGS__)
///      PTL_ESC(PTL_REST tup)
#define PTL_REST(/* first: any, ...rest: any */ _, ...) /* -> any... */ __VA_ARGS__

/// [lang.xrest]
/// ------------
/// returns all arguments except for the first.
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
#define PTL_TRIM(/* any... */...) /* -> any... */                       \
  PTL_XCAT(PPUTLTRIM_, PTL_XCAT(PPUTLTRIM_SEL(PTL_XFIRST(__VA_ARGS__)), \
                                PPUTLTRIM_SEL(PTL_XREST(__VA_ARGS__)))) \
  (__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLTRIM_SEL(...)     0##__VA_OPT__(1)
#define PPUTLTRIM_0101(_, ...) _, __VA_ARGS__
#define PPUTLTRIM_010(_, ...)  _
#define PPUTLTRIM_001(_, ...)  __VA_ARGS__
#define PPUTLTRIM_00(...)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

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
  PPUTLDEFAULT_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLDEFAULT_o(_, ...)  PPUTLDEFAULT_0##__VA_OPT__(1)(_, __VA_ARGS__)
#define PPUTLDEFAULT_01(_, ...) __VA_ARGS__
#define PPUTLDEFAULT_0(_, ...)  _

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [lang.fail]
/// -----------
/// executes an invalid preprocessor operation to indicate a failure.
/// must provide a string literal message.
///
/// usage: PTL_FAIL("something bad happened")
///        PTL_FAIL(PTL_STR([myfun] invalid args : __VA_ARGS__))
#define PTL_FAIL(/* msg: obj */...) PTL_FAIL##__VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

/// [impl.traits.hex]
/// -----------------
/// [internal] hex traits
#define PPUTLIMPL_HEX(                                                     \
    /* enum<0|1|...|E|F>, enum<NOT|DEC0|DEC1|INC0|INC1|NYBL|BITS> */ v, t) \
  PPUTLIMPL_HEX_o(t, PTL_XCAT(PPUTLIMPL_HEX_, v))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_HEX_o(t, ...) PPUTLIMPL_HEX_##t(__VA_ARGS__)
#define PPUTLIMPL_HEX_BITS(n, d0c, d0, d1c, d1, i0c, i0, i1c, i1, ny, \
                           ...) /* -> ...bool */                      \
  __VA_ARGS__
#define PPUTLIMPL_HEX_NYBL(n, d0c, d0, d1c, d1, i0c, i0, i1c, i1, ny, ...) /* -> nybl */ \
  ny
#define PPUTLIMPL_HEX_INC1(n, d0c, d0, d1c, d1, i0c, i0, i1c, i1, \
                           ...) /* -> bool, hex */                \
  i1c, i1
#define PPUTLIMPL_HEX_INC0(n, d0c, d0, d1c, d1, i0c, i0, ...) /* -> bool, hex */ i0c, i0
#define PPUTLIMPL_HEX_DEC1(n, d0c, d0, d1c, d1, ...)          /* -> bool, hex */ d1c, d1
#define PPUTLIMPL_HEX_DEC0(n, d0c, d0, ...)                   /* -> bool, hex */ d0c, d0
#define PPUTLIMPL_HEX_NOT(n, ...)                             /* -> hex */ n

/// not, dec0carry, dec0, dec1carry, dec1, inc0carry, inc0, inc1carry, inc1, nybl, ...bits
#define PPUTLIMPL_HEX_F 0, 0, F, 0, E, 0, F, 1, 0, 1111, 1, 1, 1, 1
#define PPUTLIMPL_HEX_E 1, 0, E, 0, D, 0, E, 0, F, 1110, 1, 1, 1, 0
#define PPUTLIMPL_HEX_D 2, 0, D, 0, C, 0, D, 0, E, 1101, 1, 1, 0, 1
#define PPUTLIMPL_HEX_C 3, 0, C, 0, B, 0, C, 0, D, 1100, 1, 1, 0, 0
#define PPUTLIMPL_HEX_B 4, 0, B, 0, A, 0, B, 0, C, 1011, 1, 0, 1, 1
#define PPUTLIMPL_HEX_A 5, 0, A, 0, 9, 0, A, 0, B, 1010, 1, 0, 1, 0
#define PPUTLIMPL_HEX_9 6, 0, 9, 0, 8, 0, 9, 0, A, 1001, 1, 0, 0, 1
#define PPUTLIMPL_HEX_8 7, 0, 8, 0, 7, 0, 8, 0, 9, 1000, 1, 0, 0, 0
#define PPUTLIMPL_HEX_7 8, 0, 7, 0, 6, 0, 7, 0, 8, 0111, 0, 1, 1, 1
#define PPUTLIMPL_HEX_6 9, 0, 6, 0, 5, 0, 6, 0, 7, 0110, 0, 1, 1, 0
#define PPUTLIMPL_HEX_5 A, 0, 5, 0, 4, 0, 5, 0, 6, 0101, 0, 1, 0, 1
#define PPUTLIMPL_HEX_4 B, 0, 4, 0, 3, 0, 4, 0, 5, 0100, 0, 1, 0, 0
#define PPUTLIMPL_HEX_3 C, 0, 3, 0, 2, 0, 3, 0, 4, 0011, 0, 0, 1, 1
#define PPUTLIMPL_HEX_2 D, 0, 2, 0, 1, 0, 2, 0, 3, 0010, 0, 0, 1, 0
#define PPUTLIMPL_HEX_1 E, 0, 1, 0, 0, 0, 1, 0, 2, 0001, 0, 0, 0, 1
#define PPUTLIMPL_HEX_0 F, 0, 0, 1, F, 0, 0, 0, 1, 0000, 0, 0, 0, 0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [impl.traits.hexhex]
/// --------------------
/// [internal] hex pair (hex##hex) traits
#define PPUTLIMPL_HEXHEX(                                                      \
    /* enum<00|01|...|FE|FF>, enum<LT|AND|OR|XOR|SUB0|SUB1|ADD0|ADD1> */ v, t) \
  PPUTLIMPL_HEXHEX_o(t, PTL_XCAT(PPUTLIMPL_HEXHEX_, v))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_HEXHEX_o(t, ...) PPUTLIMPL_HEXHEX_##t(__VA_ARGS__)

#define PPUTLIMPL_HEXHEX_ADD1(a, b, c, d, e, f, g, h, i, j, k, l, ...) k, l
#define PPUTLIMPL_HEXHEX_ADD0(a, b, c, d, e, f, g, h, i, j, ...)       i, j
#define PPUTLIMPL_HEXHEX_SUB1(a, b, c, d, e, f, g, h, ...)             g, h
#define PPUTLIMPL_HEXHEX_SUB0(a, b, c, d, e, f, ...)                   e, f
#define PPUTLIMPL_HEXHEX_XOR(a, b, c, d, ...)                          d
#define PPUTLIMPL_HEXHEX_OR(a, b, c, ...)                              c
#define PPUTLIMPL_HEXHEX_AND(a, b, ...)                                b
#define PPUTLIMPL_HEXHEX_LT(a, ...)                                    a

#define PPUTLIMPL_HEXHEX_IS(_, ...) /* -> bool */ PPUTLIMPL_HEXHEX_IS_0##__VA_OPT__(1)

#define PPUTLIMPL_HEXHEX_IS_01 1
#define PPUTLIMPL_HEXHEX_IS_0  0

/// lt, and, or, xor, sub0-1 carry, sub0-1, add0-1 carry, add0-1
#define PPUTLIMPL_HEXHEX_FF 0, F, F, 0, 0, 0, 1, F, 1, E, 1, F
#define PPUTLIMPL_HEXHEX_FE 0, E, F, 1, 0, 1, 0, 0, 1, D, 1, E
#define PPUTLIMPL_HEXHEX_FD 0, D, F, 2, 0, 2, 0, 1, 1, C, 1, D
#define PPUTLIMPL_HEXHEX_FC 0, C, F, 3, 0, 3, 0, 2, 1, B, 1, C
#define PPUTLIMPL_HEXHEX_FB 0, B, F, 4, 0, 4, 0, 3, 1, A, 1, B
#define PPUTLIMPL_HEXHEX_FA 0, A, F, 5, 0, 5, 0, 4, 1, 9, 1, A
#define PPUTLIMPL_HEXHEX_F9 0, 9, F, 6, 0, 6, 0, 5, 1, 8, 1, 9
#define PPUTLIMPL_HEXHEX_F8 0, 8, F, 7, 0, 7, 0, 6, 1, 7, 1, 8
#define PPUTLIMPL_HEXHEX_F7 0, 7, F, 8, 0, 8, 0, 7, 1, 6, 1, 7
#define PPUTLIMPL_HEXHEX_F6 0, 6, F, 9, 0, 9, 0, 8, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_F5 0, 5, F, A, 0, A, 0, 9, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_F4 0, 4, F, B, 0, B, 0, A, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_F3 0, 3, F, C, 0, C, 0, B, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_F2 0, 2, F, D, 0, D, 0, C, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_F1 0, 1, F, E, 0, E, 0, D, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_F0 0, 0, F, F, 0, F, 0, E, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_EF 1, E, F, 1, 1, F, 1, E, 1, D, 1, E
#define PPUTLIMPL_HEXHEX_EE 0, E, E, 0, 0, 0, 1, F, 1, C, 1, D
#define PPUTLIMPL_HEXHEX_ED 0, C, F, 3, 0, 1, 0, 0, 1, B, 1, C
#define PPUTLIMPL_HEXHEX_EC 0, C, E, 2, 0, 2, 0, 1, 1, A, 1, B
#define PPUTLIMPL_HEXHEX_EB 0, A, F, 5, 0, 3, 0, 2, 1, 9, 1, A
#define PPUTLIMPL_HEXHEX_EA 0, A, E, 4, 0, 4, 0, 3, 1, 8, 1, 9
#define PPUTLIMPL_HEXHEX_E9 0, 8, F, 7, 0, 5, 0, 4, 1, 7, 1, 8
#define PPUTLIMPL_HEXHEX_E8 0, 8, E, 6, 0, 6, 0, 5, 1, 6, 1, 7
#define PPUTLIMPL_HEXHEX_E7 0, 6, F, 9, 0, 7, 0, 6, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_E6 0, 6, E, 8, 0, 8, 0, 7, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_E5 0, 4, F, B, 0, 9, 0, 8, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_E4 0, 4, E, A, 0, A, 0, 9, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_E3 0, 2, F, D, 0, B, 0, A, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_E2 0, 2, E, C, 0, C, 0, B, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_E1 0, 0, F, F, 0, D, 0, C, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_E0 0, 0, E, E, 0, E, 0, D, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_DF 1, D, F, 2, 1, E, 1, D, 1, C, 1, D
#define PPUTLIMPL_HEXHEX_DE 1, C, F, 3, 1, F, 1, E, 1, B, 1, C
#define PPUTLIMPL_HEXHEX_DD 0, D, D, 0, 0, 0, 1, F, 1, A, 1, B
#define PPUTLIMPL_HEXHEX_DC 0, C, D, 1, 0, 1, 0, 0, 1, 9, 1, A
#define PPUTLIMPL_HEXHEX_DB 0, 9, F, 6, 0, 2, 0, 1, 1, 8, 1, 9
#define PPUTLIMPL_HEXHEX_DA 0, 8, F, 7, 0, 3, 0, 2, 1, 7, 1, 8
#define PPUTLIMPL_HEXHEX_D9 0, 9, D, 4, 0, 4, 0, 3, 1, 6, 1, 7
#define PPUTLIMPL_HEXHEX_D8 0, 8, D, 5, 0, 5, 0, 4, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_D7 0, 5, F, A, 0, 6, 0, 5, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_D6 0, 4, F, B, 0, 7, 0, 6, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_D5 0, 5, D, 8, 0, 8, 0, 7, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_D4 0, 4, D, 9, 0, 9, 0, 8, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_D3 0, 1, F, E, 0, A, 0, 9, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_D2 0, 0, F, F, 0, B, 0, A, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_D1 0, 1, D, C, 0, C, 0, B, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_D0 0, 0, D, D, 0, D, 0, C, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_CF 1, C, F, 3, 1, D, 1, C, 1, B, 1, C
#define PPUTLIMPL_HEXHEX_CE 1, C, E, 2, 1, E, 1, D, 1, A, 1, B
#define PPUTLIMPL_HEXHEX_CD 1, C, D, 1, 1, F, 1, E, 1, 9, 1, A
#define PPUTLIMPL_HEXHEX_CC 0, C, C, 0, 0, 0, 1, F, 1, 8, 1, 9
#define PPUTLIMPL_HEXHEX_CB 0, 8, F, 7, 0, 1, 0, 0, 1, 7, 1, 8
#define PPUTLIMPL_HEXHEX_CA 0, 8, E, 6, 0, 2, 0, 1, 1, 6, 1, 7
#define PPUTLIMPL_HEXHEX_C9 0, 8, D, 5, 0, 3, 0, 2, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_C8 0, 8, C, 4, 0, 4, 0, 3, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_C7 0, 4, F, B, 0, 5, 0, 4, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_C6 0, 4, E, A, 0, 6, 0, 5, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_C5 0, 4, D, 9, 0, 7, 0, 6, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_C4 0, 4, C, 8, 0, 8, 0, 7, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_C3 0, 0, F, F, 0, 9, 0, 8, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_C2 0, 0, E, E, 0, A, 0, 9, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_C1 0, 0, D, D, 0, B, 0, A, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_C0 0, 0, C, C, 0, C, 0, B, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_BF 1, B, F, 4, 1, C, 1, B, 1, A, 1, B
#define PPUTLIMPL_HEXHEX_BE 1, A, F, 5, 1, D, 1, C, 1, 9, 1, A
#define PPUTLIMPL_HEXHEX_BD 1, 9, F, 6, 1, E, 1, D, 1, 8, 1, 9
#define PPUTLIMPL_HEXHEX_BC 1, 8, F, 7, 1, F, 1, E, 1, 7, 1, 8
#define PPUTLIMPL_HEXHEX_BB 0, B, B, 0, 0, 0, 1, F, 1, 6, 1, 7
#define PPUTLIMPL_HEXHEX_BA 0, A, B, 1, 0, 1, 0, 0, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_B9 0, 9, B, 2, 0, 2, 0, 1, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_B8 0, 8, B, 3, 0, 3, 0, 2, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_B7 0, 3, F, C, 0, 4, 0, 3, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_B6 0, 2, F, D, 0, 5, 0, 4, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_B5 0, 1, F, E, 0, 6, 0, 5, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_B4 0, 0, F, F, 0, 7, 0, 6, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_B3 0, 3, B, 8, 0, 8, 0, 7, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_B2 0, 2, B, 9, 0, 9, 0, 8, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_B1 0, 1, B, A, 0, A, 0, 9, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_B0 0, 0, B, B, 0, B, 0, A, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_AF 1, A, F, 5, 1, B, 1, A, 1, 9, 1, A
#define PPUTLIMPL_HEXHEX_AE 1, A, E, 4, 1, C, 1, B, 1, 8, 1, 9
#define PPUTLIMPL_HEXHEX_AD 1, 8, F, 7, 1, D, 1, C, 1, 7, 1, 8
#define PPUTLIMPL_HEXHEX_AC 1, 8, E, 6, 1, E, 1, D, 1, 6, 1, 7
#define PPUTLIMPL_HEXHEX_AB 1, A, B, 1, 1, F, 1, E, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_AA 0, A, A, 0, 0, 0, 1, F, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_A9 0, 8, B, 3, 0, 1, 0, 0, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_A8 0, 8, A, 2, 0, 2, 0, 1, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_A7 0, 2, F, D, 0, 3, 0, 2, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_A6 0, 2, E, C, 0, 4, 0, 3, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_A5 0, 0, F, F, 0, 5, 0, 4, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_A4 0, 0, E, E, 0, 6, 0, 5, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_A3 0, 2, B, 9, 0, 7, 0, 6, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_A2 0, 2, A, 8, 0, 8, 0, 7, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_A1 0, 0, B, B, 0, 9, 0, 8, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_A0 0, 0, A, A, 0, A, 0, 9, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_9F 1, 9, F, 6, 1, A, 1, 9, 1, 8, 1, 9
#define PPUTLIMPL_HEXHEX_9E 1, 8, F, 7, 1, B, 1, A, 1, 7, 1, 8
#define PPUTLIMPL_HEXHEX_9D 1, 9, D, 4, 1, C, 1, B, 1, 6, 1, 7
#define PPUTLIMPL_HEXHEX_9C 1, 8, D, 5, 1, D, 1, C, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_9B 1, 9, B, 2, 1, E, 1, D, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_9A 1, 8, B, 3, 1, F, 1, E, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_99 0, 9, 9, 0, 0, 0, 1, F, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_98 0, 8, 9, 1, 0, 1, 0, 0, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_97 0, 1, F, E, 0, 2, 0, 1, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_96 0, 0, F, F, 0, 3, 0, 2, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_95 0, 1, D, C, 0, 4, 0, 3, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_94 0, 0, D, D, 0, 5, 0, 4, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_93 0, 1, B, A, 0, 6, 0, 5, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_92 0, 0, B, B, 0, 7, 0, 6, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_91 0, 1, 9, 8, 0, 8, 0, 7, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_90 0, 0, 9, 9, 0, 9, 0, 8, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_8F 1, 8, F, 7, 1, 9, 1, 8, 1, 7, 1, 8
#define PPUTLIMPL_HEXHEX_8E 1, 8, E, 6, 1, A, 1, 9, 1, 6, 1, 7
#define PPUTLIMPL_HEXHEX_8D 1, 8, D, 5, 1, B, 1, A, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_8C 1, 8, C, 4, 1, C, 1, B, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_8B 1, 8, B, 3, 1, D, 1, C, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_8A 1, 8, A, 2, 1, E, 1, D, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_89 1, 8, 9, 1, 1, F, 1, E, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_88 0, 8, 8, 0, 0, 0, 1, F, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_87 0, 0, F, F, 0, 1, 0, 0, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_86 0, 0, E, E, 0, 2, 0, 1, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_85 0, 0, D, D, 0, 3, 0, 2, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_84 0, 0, C, C, 0, 4, 0, 3, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_83 0, 0, B, B, 0, 5, 0, 4, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_82 0, 0, A, A, 0, 6, 0, 5, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_81 0, 0, 9, 9, 0, 7, 0, 6, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_80 0, 0, 8, 8, 0, 8, 0, 7, 0, 8, 0, 9
#define PPUTLIMPL_HEXHEX_7F 1, 7, F, 8, 1, 8, 1, 7, 1, 6, 1, 7
#define PPUTLIMPL_HEXHEX_7E 1, 6, F, 9, 1, 9, 1, 8, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_7D 1, 5, F, A, 1, A, 1, 9, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_7C 1, 4, F, B, 1, B, 1, A, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_7B 1, 3, F, C, 1, C, 1, B, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_7A 1, 2, F, D, 1, D, 1, C, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_79 1, 1, F, E, 1, E, 1, D, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_78 1, 0, F, F, 1, F, 1, E, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_77 0, 7, 7, 0, 0, 0, 1, F, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_76 0, 6, 7, 1, 0, 1, 0, 0, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_75 0, 5, 7, 2, 0, 2, 0, 1, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_74 0, 4, 7, 3, 0, 3, 0, 2, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_73 0, 3, 7, 4, 0, 4, 0, 3, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_72 0, 2, 7, 5, 0, 5, 0, 4, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_71 0, 1, 7, 6, 0, 6, 0, 5, 0, 8, 0, 9
#define PPUTLIMPL_HEXHEX_70 0, 0, 7, 7, 0, 7, 0, 6, 0, 7, 0, 8
#define PPUTLIMPL_HEXHEX_6F 1, 6, F, 9, 1, 7, 1, 6, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_6E 1, 6, E, 8, 1, 8, 1, 7, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_6D 1, 4, F, B, 1, 9, 1, 8, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_6C 1, 4, E, A, 1, A, 1, 9, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_6B 1, 2, F, D, 1, B, 1, A, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_6A 1, 2, E, C, 1, C, 1, B, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_69 1, 0, F, F, 1, D, 1, C, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_68 1, 0, E, E, 1, E, 1, D, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_67 1, 6, 7, 1, 1, F, 1, E, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_66 0, 6, 6, 0, 0, 0, 1, F, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_65 0, 4, 7, 3, 0, 1, 0, 0, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_64 0, 4, 6, 2, 0, 2, 0, 1, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_63 0, 2, 7, 5, 0, 3, 0, 2, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_62 0, 2, 6, 4, 0, 4, 0, 3, 0, 8, 0, 9
#define PPUTLIMPL_HEXHEX_61 0, 0, 7, 7, 0, 5, 0, 4, 0, 7, 0, 8
#define PPUTLIMPL_HEXHEX_60 0, 0, 6, 6, 0, 6, 0, 5, 0, 6, 0, 7
#define PPUTLIMPL_HEXHEX_5F 1, 5, F, A, 1, 6, 1, 5, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_5E 1, 4, F, B, 1, 7, 1, 6, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_5D 1, 5, D, 8, 1, 8, 1, 7, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_5C 1, 4, D, 9, 1, 9, 1, 8, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_5B 1, 1, F, E, 1, A, 1, 9, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_5A 1, 0, F, F, 1, B, 1, A, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_59 1, 1, D, C, 1, C, 1, B, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_58 1, 0, D, D, 1, D, 1, C, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_57 1, 5, 7, 2, 1, E, 1, D, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_56 1, 4, 7, 3, 1, F, 1, E, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_55 0, 5, 5, 0, 0, 0, 1, F, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_54 0, 4, 5, 1, 0, 1, 0, 0, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_53 0, 1, 7, 6, 0, 2, 0, 1, 0, 8, 0, 9
#define PPUTLIMPL_HEXHEX_52 0, 0, 7, 7, 0, 3, 0, 2, 0, 7, 0, 8
#define PPUTLIMPL_HEXHEX_51 0, 1, 5, 4, 0, 4, 0, 3, 0, 6, 0, 7
#define PPUTLIMPL_HEXHEX_50 0, 0, 5, 5, 0, 5, 0, 4, 0, 5, 0, 6
#define PPUTLIMPL_HEXHEX_4F 1, 4, F, B, 1, 5, 1, 4, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_4E 1, 4, E, A, 1, 6, 1, 5, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_4D 1, 4, D, 9, 1, 7, 1, 6, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_4C 1, 4, C, 8, 1, 8, 1, 7, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_4B 1, 0, F, F, 1, 9, 1, 8, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_4A 1, 0, E, E, 1, A, 1, 9, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_49 1, 0, D, D, 1, B, 1, A, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_48 1, 0, C, C, 1, C, 1, B, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_47 1, 4, 7, 3, 1, D, 1, C, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_46 1, 4, 6, 2, 1, E, 1, D, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_45 1, 4, 5, 1, 1, F, 1, E, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_44 0, 4, 4, 0, 0, 0, 1, F, 0, 8, 0, 9
#define PPUTLIMPL_HEXHEX_43 0, 0, 7, 7, 0, 1, 0, 0, 0, 7, 0, 8
#define PPUTLIMPL_HEXHEX_42 0, 0, 6, 6, 0, 2, 0, 1, 0, 6, 0, 7
#define PPUTLIMPL_HEXHEX_41 0, 0, 5, 5, 0, 3, 0, 2, 0, 5, 0, 6
#define PPUTLIMPL_HEXHEX_40 0, 0, 4, 4, 0, 4, 0, 3, 0, 4, 0, 5
#define PPUTLIMPL_HEXHEX_3F 1, 3, F, C, 1, 4, 1, 3, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_3E 1, 2, F, D, 1, 5, 1, 4, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_3D 1, 1, F, E, 1, 6, 1, 5, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_3C 1, 0, F, F, 1, 7, 1, 6, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_3B 1, 3, B, 8, 1, 8, 1, 7, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_3A 1, 2, B, 9, 1, 9, 1, 8, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_39 1, 1, B, A, 1, A, 1, 9, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_38 1, 0, B, B, 1, B, 1, A, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_37 1, 3, 7, 4, 1, C, 1, B, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_36 1, 2, 7, 5, 1, D, 1, C, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_35 1, 1, 7, 6, 1, E, 1, D, 0, 8, 0, 9
#define PPUTLIMPL_HEXHEX_34 1, 0, 7, 7, 1, F, 1, E, 0, 7, 0, 8
#define PPUTLIMPL_HEXHEX_33 0, 3, 3, 0, 0, 0, 1, F, 0, 6, 0, 7
#define PPUTLIMPL_HEXHEX_32 0, 2, 3, 1, 0, 1, 0, 0, 0, 5, 0, 6
#define PPUTLIMPL_HEXHEX_31 0, 1, 3, 2, 0, 2, 0, 1, 0, 4, 0, 5
#define PPUTLIMPL_HEXHEX_30 0, 0, 3, 3, 0, 3, 0, 2, 0, 3, 0, 4
#define PPUTLIMPL_HEXHEX_2F 1, 2, F, D, 1, 3, 1, 2, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_2E 1, 2, E, C, 1, 4, 1, 3, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_2D 1, 0, F, F, 1, 5, 1, 4, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_2C 1, 0, E, E, 1, 6, 1, 5, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_2B 1, 2, B, 9, 1, 7, 1, 6, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_2A 1, 2, A, 8, 1, 8, 1, 7, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_29 1, 0, B, B, 1, 9, 1, 8, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_28 1, 0, A, A, 1, A, 1, 9, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_27 1, 2, 7, 5, 1, B, 1, A, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_26 1, 2, 6, 4, 1, C, 1, B, 0, 8, 0, 9
#define PPUTLIMPL_HEXHEX_25 1, 0, 7, 7, 1, D, 1, C, 0, 7, 0, 8
#define PPUTLIMPL_HEXHEX_24 1, 0, 6, 6, 1, E, 1, D, 0, 6, 0, 7
#define PPUTLIMPL_HEXHEX_23 1, 2, 3, 1, 1, F, 1, E, 0, 5, 0, 6
#define PPUTLIMPL_HEXHEX_22 0, 2, 2, 0, 0, 0, 1, F, 0, 4, 0, 5
#define PPUTLIMPL_HEXHEX_21 0, 0, 3, 3, 0, 1, 0, 0, 0, 3, 0, 4
#define PPUTLIMPL_HEXHEX_20 0, 0, 2, 2, 0, 2, 0, 1, 0, 2, 0, 3
#define PPUTLIMPL_HEXHEX_1F 1, 1, F, E, 1, 2, 1, 1, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_1E 1, 0, F, F, 1, 3, 1, 2, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_1D 1, 1, D, C, 1, 4, 1, 3, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_1C 1, 0, D, D, 1, 5, 1, 4, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_1B 1, 1, B, A, 1, 6, 1, 5, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_1A 1, 0, B, B, 1, 7, 1, 6, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_19 1, 1, 9, 8, 1, 8, 1, 7, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_18 1, 0, 9, 9, 1, 9, 1, 8, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_17 1, 1, 7, 6, 1, A, 1, 9, 0, 8, 0, 9
#define PPUTLIMPL_HEXHEX_16 1, 0, 7, 7, 1, B, 1, A, 0, 7, 0, 8
#define PPUTLIMPL_HEXHEX_15 1, 1, 5, 4, 1, C, 1, B, 0, 6, 0, 7
#define PPUTLIMPL_HEXHEX_14 1, 0, 5, 5, 1, D, 1, C, 0, 5, 0, 6
#define PPUTLIMPL_HEXHEX_13 1, 1, 3, 2, 1, E, 1, D, 0, 4, 0, 5
#define PPUTLIMPL_HEXHEX_12 1, 0, 3, 3, 1, F, 1, E, 0, 3, 0, 4
#define PPUTLIMPL_HEXHEX_11 0, 1, 1, 0, 0, 0, 1, F, 0, 2, 0, 3
#define PPUTLIMPL_HEXHEX_10 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 2
#define PPUTLIMPL_HEXHEX_0F 1, 0, F, F, 1, 1, 1, 0, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_0E 1, 0, E, E, 1, 2, 1, 1, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_0D 1, 0, D, D, 1, 3, 1, 2, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_0C 1, 0, C, C, 1, 4, 1, 3, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_0B 1, 0, B, B, 1, 5, 1, 4, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_0A 1, 0, A, A, 1, 6, 1, 5, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_09 1, 0, 9, 9, 1, 7, 1, 6, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_08 1, 0, 8, 8, 1, 8, 1, 7, 0, 8, 0, 9
#define PPUTLIMPL_HEXHEX_07 1, 0, 7, 7, 1, 9, 1, 8, 0, 7, 0, 8
#define PPUTLIMPL_HEXHEX_06 1, 0, 6, 6, 1, A, 1, 9, 0, 6, 0, 7
#define PPUTLIMPL_HEXHEX_05 1, 0, 5, 5, 1, B, 1, A, 0, 5, 0, 6
#define PPUTLIMPL_HEXHEX_04 1, 0, 4, 4, 1, C, 1, B, 0, 4, 0, 5
#define PPUTLIMPL_HEXHEX_03 1, 0, 3, 3, 1, D, 1, C, 0, 3, 0, 4
#define PPUTLIMPL_HEXHEX_02 1, 0, 2, 2, 1, E, 1, D, 0, 2, 0, 3
#define PPUTLIMPL_HEXHEX_01 1, 0, 1, 1, 1, F, 1, E, 0, 1, 0, 2
#define PPUTLIMPL_HEXHEX_00 0, 0, 0, 0, 0, 0, 1, F, 0, 0, 0, 1

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [impl.traits.nybl]
/// ------------------
/// [internal] nybl traits
#define PPUTLIMPL_NYBL(/* enum<0000|0001|...|1110|1111>, enum<HEX|BITS> */ v, t) \
  PPUTLIMPL_NYBL_o(t, PTL_XCAT(PPUTLIMPL_NYBL_, v))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_NYBL_o(t, ...)      PPUTLIMPL_NYBL_##t(__VA_ARGS__)
#define PPUTLIMPL_NYBL_BITS(hex, ...) /* -> ...bool */ __VA_ARGS__
#define PPUTLIMPL_NYBL_HEX(hex, ...)  /* -> hex */ hex
#define PPUTLIMPL_NYBL_1111           F, 1, 1, 1, 1
#define PPUTLIMPL_NYBL_1110           E, 1, 1, 1, 0
#define PPUTLIMPL_NYBL_1101           D, 1, 1, 0, 1
#define PPUTLIMPL_NYBL_1100           C, 1, 1, 0, 0
#define PPUTLIMPL_NYBL_1011           B, 1, 0, 1, 1
#define PPUTLIMPL_NYBL_1010           A, 1, 0, 1, 0
#define PPUTLIMPL_NYBL_1001           9, 1, 0, 0, 1
#define PPUTLIMPL_NYBL_1000           8, 1, 0, 0, 0
#define PPUTLIMPL_NYBL_0111           7, 0, 1, 1, 1
#define PPUTLIMPL_NYBL_0110           6, 0, 1, 1, 0
#define PPUTLIMPL_NYBL_0101           5, 0, 1, 0, 1
#define PPUTLIMPL_NYBL_0100           4, 0, 1, 0, 0
#define PPUTLIMPL_NYBL_0011           3, 0, 0, 1, 1
#define PPUTLIMPL_NYBL_0010           2, 0, 0, 1, 0
#define PPUTLIMPL_NYBL_0001           1, 0, 0, 0, 1
#define PPUTLIMPL_NYBL_0000           0, 0, 0, 0, 0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [impl.traits.udec]
/// ------------------
/// [internal] udec traits
#define PPUTLIMPL_UDEC(                                                          \
    /* enum<0u|1u|...>, enum<UHEX|ISIZE|USIZE|IOFS|UOFS|LOG2|SQRT|FACT> */ v, t) \
  PPUTLIMPL_UDEC_o(t, PTL_XCAT(PPUTLIMPL_UDEC_, v))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_UDEC_o(t, ...)                           PPUTLIMPL_UDEC_##t(__VA_ARGS__)
#define PPUTLIMPL_UDEC_FACT(u, is, us, ii, ui, l, sq, ...) /* -> idec... */ __VA_ARGS__
#define PPUTLIMPL_UDEC_SQRT(u, is, us, ii, ui, l, sq, ...) /* -> idec */ sq
#define PPUTLIMPL_UDEC_LOG2(u, is, us, ii, ui, l, ...)     /* -> idec */ l
#define PPUTLIMPL_UDEC_UOFS(u, is, us, ii, ui, ...)        /* -> bool */ ui
#define PPUTLIMPL_UDEC_IOFS(u, is, us, ii, ...)            /* -> bool */ ii
#define PPUTLIMPL_UDEC_USIZE(u, is, us, ...)               /* -> bool */ us
#define PPUTLIMPL_UDEC_ISIZE(u, is, ...)                   /* -> bool */ is
#define PPUTLIMPL_UDEC_UHEX(u, ...)                        /* -> uhex */ u

/// UHEX, ISIZE, USIZE, IOFS, UOFS, LOG2, SQRT, FACT
#define PPUTLIMPL_UDEC_4095u 0xFFFu, 0, 0, 1, 0, 11, 63, 3, 3, 5, 7, 13
#define PPUTLIMPL_UDEC_4094u 0xFFEu, 0, 0, 1, 0, 11, 63, 2, 23, 89
#define PPUTLIMPL_UDEC_4093u 0xFFDu, 0, 0, 1, 0, 11, 63,
#define PPUTLIMPL_UDEC_4092u 0xFFCu, 0, 0, 1, 0, 11, 63, 2, 2, 3, 11, 31
#define PPUTLIMPL_UDEC_4091u 0xFFBu, 0, 0, 1, 0, 11, 63,
#define PPUTLIMPL_UDEC_4090u 0xFFAu, 0, 0, 1, 0, 11, 63, 2, 5, 409
#define PPUTLIMPL_UDEC_4089u 0xFF9u, 0, 0, 1, 0, 11, 63, 3, 29, 47
#define PPUTLIMPL_UDEC_4088u 0xFF8u, 0, 0, 1, 0, 11, 63, 2, 2, 2, 7, 73
#define PPUTLIMPL_UDEC_4087u 0xFF7u, 0, 0, 1, 0, 11, 63, 61, 67
#define PPUTLIMPL_UDEC_4086u 0xFF6u, 0, 0, 1, 0, 11, 63, 2, 3, 3, 227
#define PPUTLIMPL_UDEC_4085u 0xFF5u, 0, 0, 1, 0, 11, 63, 5, 19, 43
#define PPUTLIMPL_UDEC_4084u 0xFF4u, 0, 0, 1, 0, 11, 63, 2, 2, 1021
#define PPUTLIMPL_UDEC_4083u 0xFF3u, 0, 0, 1, 0, 11, 63, 3, 1361
#define PPUTLIMPL_UDEC_4082u 0xFF2u, 0, 0, 1, 0, 11, 63, 2, 13, 157
#define PPUTLIMPL_UDEC_4081u 0xFF1u, 0, 0, 1, 0, 11, 63, 7, 11, 53
#define PPUTLIMPL_UDEC_4080u 0xFF0u, 0, 0, 1, 0, 11, 63, 2, 2, 2, 2, 3, 5, 17
#define PPUTLIMPL_UDEC_4079u 0xFEFu, 0, 0, 1, 0, 11, 63,
#define PPUTLIMPL_UDEC_4078u 0xFEEu, 0, 0, 1, 0, 11, 63, 2, 2039
#define PPUTLIMPL_UDEC_4077u 0xFEDu, 0, 0, 1, 0, 11, 63, 3, 3, 3, 151
#define PPUTLIMPL_UDEC_4076u 0xFECu, 0, 0, 1, 0, 11, 63, 2, 2, 1019
#define PPUTLIMPL_UDEC_4075u 0xFEBu, 0, 0, 1, 0, 11, 63, 5, 5, 163
#define PPUTLIMPL_UDEC_4074u 0xFEAu, 0, 0, 1, 0, 11, 63, 2, 3, 7, 97
#define PPUTLIMPL_UDEC_4073u 0xFE9u, 0, 0, 1, 0, 11, 63,
#define PPUTLIMPL_UDEC_4072u 0xFE8u, 0, 0, 1, 0, 11, 63, 2, 2, 2, 509
#define PPUTLIMPL_UDEC_4071u 0xFE7u, 0, 0, 1, 0, 11, 63, 3, 23, 59
#define PPUTLIMPL_UDEC_4070u 0xFE6u, 0, 0, 1, 0, 11, 63, 2, 5, 11, 37
#define PPUTLIMPL_UDEC_4069u 0xFE5u, 0, 0, 1, 0, 11, 63, 13, 313
#define PPUTLIMPL_UDEC_4068u 0xFE4u, 0, 0, 1, 0, 11, 63, 2, 2, 3, 3, 113
#define PPUTLIMPL_UDEC_4067u 0xFE3u, 0, 0, 1, 0, 11, 63, 7, 7, 83
#define PPUTLIMPL_UDEC_4066u 0xFE2u, 0, 0, 1, 0, 11, 63, 2, 19, 107
#define PPUTLIMPL_UDEC_4065u 0xFE1u, 0, 0, 1, 0, 11, 63, 3, 5, 271
#define PPUTLIMPL_UDEC_4064u 0xFE0u, 0, 0, 1, 0, 11, 63, 2, 2, 2, 2, 2, 127
#define PPUTLIMPL_UDEC_4063u 0xFDFu, 0, 0, 1, 0, 11, 63, 17, 239
#define PPUTLIMPL_UDEC_4062u 0xFDEu, 0, 0, 1, 0, 11, 63, 2, 3, 677
#define PPUTLIMPL_UDEC_4061u 0xFDDu, 0, 0, 1, 0, 11, 63, 31, 131
#define PPUTLIMPL_UDEC_4060u 0xFDCu, 0, 0, 1, 0, 11, 63, 2, 2, 5, 7, 29
#define PPUTLIMPL_UDEC_4059u 0xFDBu, 0, 0, 1, 0, 11, 63, 3, 3, 11, 41
#define PPUTLIMPL_UDEC_4058u 0xFDAu, 0, 0, 1, 0, 11, 63, 2, 2029
#define PPUTLIMPL_UDEC_4057u 0xFD9u, 0, 0, 1, 0, 11, 63,
#define PPUTLIMPL_UDEC_4056u 0xFD8u, 0, 0, 1, 0, 11, 63, 2, 2, 2, 3, 13, 13
#define PPUTLIMPL_UDEC_4055u 0xFD7u, 0, 0, 1, 0, 11, 63, 5, 811
#define PPUTLIMPL_UDEC_4054u 0xFD6u, 0, 0, 1, 0, 11, 63, 2, 2027
#define PPUTLIMPL_UDEC_4053u 0xFD5u, 0, 0, 1, 0, 11, 63, 3, 7, 193
#define PPUTLIMPL_UDEC_4052u 0xFD4u, 0, 0, 1, 0, 11, 63, 2, 2, 1013
#define PPUTLIMPL_UDEC_4051u 0xFD3u, 0, 0, 1, 0, 11, 63,
#define PPUTLIMPL_UDEC_4050u 0xFD2u, 0, 0, 1, 0, 11, 63, 2, 3, 3, 3, 3, 5, 5
#define PPUTLIMPL_UDEC_4049u 0xFD1u, 0, 0, 1, 0, 11, 63,
#define PPUTLIMPL_UDEC_4048u 0xFD0u, 0, 0, 1, 0, 11, 63, 2, 2, 2, 2, 11, 23
#define PPUTLIMPL_UDEC_4047u 0xFCFu, 0, 0, 1, 0, 11, 63, 3, 19, 71
#define PPUTLIMPL_UDEC_4046u 0xFCEu, 0, 0, 1, 0, 11, 63, 2, 7, 17, 17
#define PPUTLIMPL_UDEC_4045u 0xFCDu, 0, 0, 1, 0, 11, 63, 5, 809
#define PPUTLIMPL_UDEC_4044u 0xFCCu, 0, 0, 1, 0, 11, 63, 2, 2, 3, 337
#define PPUTLIMPL_UDEC_4043u 0xFCBu, 0, 0, 1, 0, 11, 63, 13, 311
#define PPUTLIMPL_UDEC_4042u 0xFCAu, 0, 0, 1, 0, 11, 63, 2, 43, 47
#define PPUTLIMPL_UDEC_4041u 0xFC9u, 0, 0, 1, 0, 11, 63, 3, 3, 449
#define PPUTLIMPL_UDEC_4040u 0xFC8u, 0, 0, 1, 0, 11, 63, 2, 2, 2, 5, 101
#define PPUTLIMPL_UDEC_4039u 0xFC7u, 0, 0, 1, 0, 11, 63, 7, 577
#define PPUTLIMPL_UDEC_4038u 0xFC6u, 0, 0, 1, 0, 11, 63, 2, 3, 673
#define PPUTLIMPL_UDEC_4037u 0xFC5u, 0, 0, 1, 0, 11, 63, 11, 367
#define PPUTLIMPL_UDEC_4036u 0xFC4u, 0, 0, 1, 0, 11, 63, 2, 2, 1009
#define PPUTLIMPL_UDEC_4035u 0xFC3u, 0, 0, 1, 0, 11, 63, 3, 5, 269
#define PPUTLIMPL_UDEC_4034u 0xFC2u, 0, 0, 1, 0, 11, 63, 2, 2017
#define PPUTLIMPL_UDEC_4033u 0xFC1u, 0, 0, 1, 0, 11, 63, 37, 109
#define PPUTLIMPL_UDEC_4032u 0xFC0u, 0, 0, 1, 0, 11, 63, 2, 2, 2, 2, 2, 2, 3, 3, 7
#define PPUTLIMPL_UDEC_4031u 0xFBFu, 0, 0, 1, 0, 11, 63, 29, 139
#define PPUTLIMPL_UDEC_4030u 0xFBEu, 0, 0, 1, 0, 11, 63, 2, 5, 13, 31
#define PPUTLIMPL_UDEC_4029u 0xFBDu, 0, 0, 1, 0, 11, 63, 3, 17, 79
#define PPUTLIMPL_UDEC_4028u 0xFBCu, 0, 0, 1, 0, 11, 63, 2, 2, 19, 53
#define PPUTLIMPL_UDEC_4027u 0xFBBu, 0, 0, 1, 0, 11, 63,
#define PPUTLIMPL_UDEC_4026u 0xFBAu, 0, 0, 1, 0, 11, 63, 2, 3, 11, 61
#define PPUTLIMPL_UDEC_4025u 0xFB9u, 0, 0, 1, 0, 11, 63, 5, 5, 7, 23
#define PPUTLIMPL_UDEC_4024u 0xFB8u, 0, 0, 1, 0, 11, 63, 2, 2, 2, 503
#define PPUTLIMPL_UDEC_4023u 0xFB7u, 0, 0, 1, 0, 11, 63, 3, 3, 3, 149
#define PPUTLIMPL_UDEC_4022u 0xFB6u, 0, 0, 1, 0, 11, 63, 2, 2011
#define PPUTLIMPL_UDEC_4021u 0xFB5u, 0, 0, 1, 0, 11, 63,
#define PPUTLIMPL_UDEC_4020u 0xFB4u, 0, 0, 1, 0, 11, 63, 2, 2, 3, 5, 67
#define PPUTLIMPL_UDEC_4019u 0xFB3u, 0, 0, 1, 0, 11, 63,
#define PPUTLIMPL_UDEC_4018u 0xFB2u, 0, 0, 1, 0, 11, 63, 2, 7, 7, 41
#define PPUTLIMPL_UDEC_4017u 0xFB1u, 0, 0, 1, 0, 11, 63, 3, 13, 103
#define PPUTLIMPL_UDEC_4016u 0xFB0u, 0, 0, 1, 0, 11, 63, 2, 2, 2, 2, 251
#define PPUTLIMPL_UDEC_4015u 0xFAFu, 0, 0, 1, 0, 11, 63, 5, 11, 73
#define PPUTLIMPL_UDEC_4014u 0xFAEu, 0, 0, 1, 0, 11, 63, 2, 3, 3, 223
#define PPUTLIMPL_UDEC_4013u 0xFADu, 0, 0, 1, 0, 11, 63,
#define PPUTLIMPL_UDEC_4012u 0xFACu, 0, 0, 1, 0, 11, 63, 2, 2, 17, 59
#define PPUTLIMPL_UDEC_4011u 0xFABu, 0, 0, 1, 0, 11, 63, 3, 7, 191
#define PPUTLIMPL_UDEC_4010u 0xFAAu, 0, 0, 1, 0, 11, 63, 2, 5, 401
#define PPUTLIMPL_UDEC_4009u 0xFA9u, 0, 0, 1, 0, 11, 63, 19, 211
#define PPUTLIMPL_UDEC_4008u 0xFA8u, 0, 0, 1, 0, 11, 63, 2, 2, 2, 3, 167
#define PPUTLIMPL_UDEC_4007u 0xFA7u, 0, 0, 1, 0, 11, 63,
#define PPUTLIMPL_UDEC_4006u 0xFA6u, 0, 0, 1, 0, 11, 63, 2, 2003
#define PPUTLIMPL_UDEC_4005u 0xFA5u, 0, 0, 1, 0, 11, 63, 3, 3, 5, 89
#define PPUTLIMPL_UDEC_4004u 0xFA4u, 0, 0, 1, 0, 11, 63, 2, 2, 7, 11, 13
#define PPUTLIMPL_UDEC_4003u 0xFA3u, 0, 0, 1, 0, 11, 63,
#define PPUTLIMPL_UDEC_4002u 0xFA2u, 0, 0, 1, 0, 11, 63, 2, 3, 23, 29
#define PPUTLIMPL_UDEC_4001u 0xFA1u, 0, 0, 1, 0, 11, 63,
#define PPUTLIMPL_UDEC_4000u 0xFA0u, 0, 0, 1, 0, 11, 63, 2, 2, 2, 2, 2, 5, 5, 5
#define PPUTLIMPL_UDEC_3999u 0xF9Fu, 0, 0, 1, 0, 11, 63, 3, 31, 43
#define PPUTLIMPL_UDEC_3998u 0xF9Eu, 0, 0, 1, 0, 11, 63, 2, 1999
#define PPUTLIMPL_UDEC_3997u 0xF9Du, 0, 0, 1, 0, 11, 63, 7, 571
#define PPUTLIMPL_UDEC_3996u 0xF9Cu, 0, 0, 1, 0, 11, 63, 2, 2, 3, 3, 3, 37
#define PPUTLIMPL_UDEC_3995u 0xF9Bu, 0, 0, 1, 0, 11, 63, 5, 17, 47
#define PPUTLIMPL_UDEC_3994u 0xF9Au, 0, 0, 1, 0, 11, 63, 2, 1997
#define PPUTLIMPL_UDEC_3993u 0xF99u, 0, 0, 1, 0, 11, 63, 3, 11, 11, 11
#define PPUTLIMPL_UDEC_3992u 0xF98u, 0, 0, 1, 0, 11, 63, 2, 2, 2, 499
#define PPUTLIMPL_UDEC_3991u 0xF97u, 0, 0, 1, 0, 11, 63, 13, 307
#define PPUTLIMPL_UDEC_3990u 0xF96u, 0, 0, 1, 0, 11, 63, 2, 3, 5, 7, 19
#define PPUTLIMPL_UDEC_3989u 0xF95u, 0, 0, 1, 0, 11, 63,
#define PPUTLIMPL_UDEC_3988u 0xF94u, 0, 0, 1, 0, 11, 63, 2, 2, 997
#define PPUTLIMPL_UDEC_3987u 0xF93u, 0, 0, 1, 0, 11, 63, 3, 3, 443
#define PPUTLIMPL_UDEC_3986u 0xF92u, 0, 0, 1, 0, 11, 63, 2, 1993
#define PPUTLIMPL_UDEC_3985u 0xF91u, 0, 0, 1, 0, 11, 63, 5, 797
#define PPUTLIMPL_UDEC_3984u 0xF90u, 0, 0, 1, 0, 11, 63, 2, 2, 2, 2, 3, 83
#define PPUTLIMPL_UDEC_3983u 0xF8Fu, 0, 0, 1, 0, 11, 63, 7, 569
#define PPUTLIMPL_UDEC_3982u 0xF8Eu, 0, 0, 1, 0, 11, 63, 2, 11, 181
#define PPUTLIMPL_UDEC_3981u 0xF8Du, 0, 0, 1, 0, 11, 63, 3, 1327
#define PPUTLIMPL_UDEC_3980u 0xF8Cu, 0, 0, 1, 0, 11, 63, 2, 2, 5, 199
#define PPUTLIMPL_UDEC_3979u 0xF8Bu, 0, 0, 1, 0, 11, 63, 23, 173
#define PPUTLIMPL_UDEC_3978u 0xF8Au, 0, 0, 1, 0, 11, 63, 2, 3, 3, 13, 17
#define PPUTLIMPL_UDEC_3977u 0xF89u, 0, 0, 1, 0, 11, 63, 41, 97
#define PPUTLIMPL_UDEC_3976u 0xF88u, 0, 0, 1, 0, 11, 63, 2, 2, 2, 7, 71
#define PPUTLIMPL_UDEC_3975u 0xF87u, 0, 0, 1, 0, 11, 63, 3, 5, 5, 53
#define PPUTLIMPL_UDEC_3974u 0xF86u, 0, 0, 1, 0, 11, 63, 2, 1987
#define PPUTLIMPL_UDEC_3973u 0xF85u, 0, 0, 1, 0, 11, 63, 29, 137
#define PPUTLIMPL_UDEC_3972u 0xF84u, 0, 0, 1, 0, 11, 63, 2, 2, 3, 331
#define PPUTLIMPL_UDEC_3971u 0xF83u, 0, 0, 1, 0, 11, 63, 11, 19, 19
#define PPUTLIMPL_UDEC_3970u 0xF82u, 0, 0, 1, 0, 11, 63, 2, 5, 397
#define PPUTLIMPL_UDEC_3969u 0xF81u, 0, 0, 1, 0, 11, 63, 3, 3, 3, 3, 7, 7
#define PPUTLIMPL_UDEC_3968u 0xF80u, 0, 0, 1, 0, 11, 62, 2, 2, 2, 2, 2, 2, 2, 31
#define PPUTLIMPL_UDEC_3967u 0xF7Fu, 0, 0, 1, 0, 11, 62,
#define PPUTLIMPL_UDEC_3966u 0xF7Eu, 0, 0, 1, 0, 11, 62, 2, 3, 661
#define PPUTLIMPL_UDEC_3965u 0xF7Du, 0, 0, 1, 0, 11, 62, 5, 13, 61
#define PPUTLIMPL_UDEC_3964u 0xF7Cu, 0, 0, 1, 0, 11, 62, 2, 2, 991
#define PPUTLIMPL_UDEC_3963u 0xF7Bu, 0, 0, 1, 0, 11, 62, 3, 1321
#define PPUTLIMPL_UDEC_3962u 0xF7Au, 0, 0, 1, 0, 11, 62, 2, 7, 283
#define PPUTLIMPL_UDEC_3961u 0xF79u, 0, 0, 1, 0, 11, 62, 17, 233
#define PPUTLIMPL_UDEC_3960u 0xF78u, 0, 0, 1, 0, 11, 62, 2, 2, 2, 3, 3, 5, 11
#define PPUTLIMPL_UDEC_3959u 0xF77u, 0, 0, 1, 0, 11, 62, 37, 107
#define PPUTLIMPL_UDEC_3958u 0xF76u, 0, 0, 1, 0, 11, 62, 2, 1979
#define PPUTLIMPL_UDEC_3957u 0xF75u, 0, 0, 1, 0, 11, 62, 3, 1319
#define PPUTLIMPL_UDEC_3956u 0xF74u, 0, 0, 1, 0, 11, 62, 2, 2, 23, 43
#define PPUTLIMPL_UDEC_3955u 0xF73u, 0, 0, 1, 0, 11, 62, 5, 7, 113
#define PPUTLIMPL_UDEC_3954u 0xF72u, 0, 0, 1, 0, 11, 62, 2, 3, 659
#define PPUTLIMPL_UDEC_3953u 0xF71u, 0, 0, 1, 0, 11, 62, 59, 67
#define PPUTLIMPL_UDEC_3952u 0xF70u, 0, 0, 1, 0, 11, 62, 2, 2, 2, 2, 13, 19
#define PPUTLIMPL_UDEC_3951u 0xF6Fu, 0, 0, 1, 0, 11, 62, 3, 3, 439
#define PPUTLIMPL_UDEC_3950u 0xF6Eu, 0, 0, 1, 0, 11, 62, 2, 5, 5, 79
#define PPUTLIMPL_UDEC_3949u 0xF6Du, 0, 0, 1, 0, 11, 62, 11, 359
#define PPUTLIMPL_UDEC_3948u 0xF6Cu, 0, 0, 1, 0, 11, 62, 2, 2, 3, 7, 47
#define PPUTLIMPL_UDEC_3947u 0xF6Bu, 0, 0, 1, 0, 11, 62,
#define PPUTLIMPL_UDEC_3946u 0xF6Au, 0, 0, 1, 0, 11, 62, 2, 1973
#define PPUTLIMPL_UDEC_3945u 0xF69u, 0, 0, 1, 0, 11, 62, 3, 5, 263
#define PPUTLIMPL_UDEC_3944u 0xF68u, 0, 0, 1, 0, 11, 62, 2, 2, 2, 17, 29
#define PPUTLIMPL_UDEC_3943u 0xF67u, 0, 0, 1, 0, 11, 62,
#define PPUTLIMPL_UDEC_3942u 0xF66u, 0, 0, 1, 0, 11, 62, 2, 3, 3, 3, 73
#define PPUTLIMPL_UDEC_3941u 0xF65u, 0, 0, 1, 0, 11, 62, 7, 563
#define PPUTLIMPL_UDEC_3940u 0xF64u, 0, 0, 1, 0, 11, 62, 2, 2, 5, 197
#define PPUTLIMPL_UDEC_3939u 0xF63u, 0, 0, 1, 0, 11, 62, 3, 13, 101
#define PPUTLIMPL_UDEC_3938u 0xF62u, 0, 0, 1, 0, 11, 62, 2, 11, 179
#define PPUTLIMPL_UDEC_3937u 0xF61u, 0, 0, 1, 0, 11, 62, 31, 127
#define PPUTLIMPL_UDEC_3936u 0xF60u, 0, 0, 1, 0, 11, 62, 2, 2, 2, 2, 2, 3, 41
#define PPUTLIMPL_UDEC_3935u 0xF5Fu, 0, 0, 1, 0, 11, 62, 5, 787
#define PPUTLIMPL_UDEC_3934u 0xF5Eu, 0, 0, 1, 0, 11, 62, 2, 7, 281
#define PPUTLIMPL_UDEC_3933u 0xF5Du, 0, 0, 1, 0, 11, 62, 3, 3, 19, 23
#define PPUTLIMPL_UDEC_3932u 0xF5Cu, 0, 0, 1, 0, 11, 62, 2, 2, 983
#define PPUTLIMPL_UDEC_3931u 0xF5Bu, 0, 0, 1, 0, 11, 62,
#define PPUTLIMPL_UDEC_3930u 0xF5Au, 0, 0, 1, 0, 11, 62, 2, 3, 5, 131
#define PPUTLIMPL_UDEC_3929u 0xF59u, 0, 0, 1, 0, 11, 62,
#define PPUTLIMPL_UDEC_3928u 0xF58u, 0, 0, 1, 0, 11, 62, 2, 2, 2, 491
#define PPUTLIMPL_UDEC_3927u 0xF57u, 0, 0, 1, 0, 11, 62, 3, 7, 11, 17
#define PPUTLIMPL_UDEC_3926u 0xF56u, 0, 0, 1, 0, 11, 62, 2, 13, 151
#define PPUTLIMPL_UDEC_3925u 0xF55u, 0, 0, 1, 0, 11, 62, 5, 5, 157
#define PPUTLIMPL_UDEC_3924u 0xF54u, 0, 0, 1, 0, 11, 62, 2, 2, 3, 3, 109
#define PPUTLIMPL_UDEC_3923u 0xF53u, 0, 0, 1, 0, 11, 62,
#define PPUTLIMPL_UDEC_3922u 0xF52u, 0, 0, 1, 0, 11, 62, 2, 37, 53
#define PPUTLIMPL_UDEC_3921u 0xF51u, 0, 0, 1, 0, 11, 62, 3, 1307
#define PPUTLIMPL_UDEC_3920u 0xF50u, 0, 0, 1, 0, 11, 62, 2, 2, 2, 2, 5, 7, 7
#define PPUTLIMPL_UDEC_3919u 0xF4Fu, 0, 0, 1, 0, 11, 62,
#define PPUTLIMPL_UDEC_3918u 0xF4Eu, 0, 0, 1, 0, 11, 62, 2, 3, 653
#define PPUTLIMPL_UDEC_3917u 0xF4Du, 0, 0, 1, 0, 11, 62,
#define PPUTLIMPL_UDEC_3916u 0xF4Cu, 0, 0, 1, 0, 11, 62, 2, 2, 11, 89
#define PPUTLIMPL_UDEC_3915u 0xF4Bu, 0, 0, 1, 0, 11, 62, 3, 3, 3, 5, 29
#define PPUTLIMPL_UDEC_3914u 0xF4Au, 0, 0, 1, 0, 11, 62, 2, 19, 103
#define PPUTLIMPL_UDEC_3913u 0xF49u, 0, 0, 1, 0, 11, 62, 7, 13, 43
#define PPUTLIMPL_UDEC_3912u 0xF48u, 0, 0, 1, 0, 11, 62, 2, 2, 2, 3, 163
#define PPUTLIMPL_UDEC_3911u 0xF47u, 0, 0, 1, 0, 11, 62,
#define PPUTLIMPL_UDEC_3910u 0xF46u, 0, 0, 1, 0, 11, 62, 2, 5, 17, 23
#define PPUTLIMPL_UDEC_3909u 0xF45u, 0, 0, 1, 0, 11, 62, 3, 1303
#define PPUTLIMPL_UDEC_3908u 0xF44u, 0, 0, 1, 0, 11, 62, 2, 2, 977
#define PPUTLIMPL_UDEC_3907u 0xF43u, 0, 0, 1, 0, 11, 62,
#define PPUTLIMPL_UDEC_3906u 0xF42u, 0, 0, 1, 0, 11, 62, 2, 3, 3, 7, 31
#define PPUTLIMPL_UDEC_3905u 0xF41u, 0, 0, 1, 0, 11, 62, 5, 11, 71
#define PPUTLIMPL_UDEC_3904u 0xF40u, 0, 0, 1, 0, 11, 62, 2, 2, 2, 2, 2, 2, 61
#define PPUTLIMPL_UDEC_3903u 0xF3Fu, 0, 0, 1, 0, 11, 62, 3, 1301
#define PPUTLIMPL_UDEC_3902u 0xF3Eu, 0, 0, 1, 0, 11, 62, 2, 1951
#define PPUTLIMPL_UDEC_3901u 0xF3Du, 0, 0, 1, 0, 11, 62, 47, 83
#define PPUTLIMPL_UDEC_3900u 0xF3Cu, 0, 0, 1, 0, 11, 62, 2, 2, 3, 5, 5, 13
#define PPUTLIMPL_UDEC_3899u 0xF3Bu, 0, 0, 1, 0, 11, 62, 7, 557
#define PPUTLIMPL_UDEC_3898u 0xF3Au, 0, 0, 1, 0, 11, 62, 2, 1949
#define PPUTLIMPL_UDEC_3897u 0xF39u, 0, 0, 1, 0, 11, 62, 3, 3, 433
#define PPUTLIMPL_UDEC_3896u 0xF38u, 0, 0, 1, 0, 11, 62, 2, 2, 2, 487
#define PPUTLIMPL_UDEC_3895u 0xF37u, 0, 0, 1, 0, 11, 62, 5, 19, 41
#define PPUTLIMPL_UDEC_3894u 0xF36u, 0, 0, 1, 0, 11, 62, 2, 3, 11, 59
#define PPUTLIMPL_UDEC_3893u 0xF35u, 0, 0, 1, 0, 11, 62, 17, 229
#define PPUTLIMPL_UDEC_3892u 0xF34u, 0, 0, 1, 0, 11, 62, 2, 2, 7, 139
#define PPUTLIMPL_UDEC_3891u 0xF33u, 0, 0, 1, 0, 11, 62, 3, 1297
#define PPUTLIMPL_UDEC_3890u 0xF32u, 0, 0, 1, 0, 11, 62, 2, 5, 389
#define PPUTLIMPL_UDEC_3889u 0xF31u, 0, 0, 1, 0, 11, 62,
#define PPUTLIMPL_UDEC_3888u 0xF30u, 0, 0, 1, 0, 11, 62, 2, 2, 2, 2, 3, 3, 3, 3, 3
#define PPUTLIMPL_UDEC_3887u 0xF2Fu, 0, 0, 1, 0, 11, 62, 13, 13, 23
#define PPUTLIMPL_UDEC_3886u 0xF2Eu, 0, 0, 1, 0, 11, 62, 2, 29, 67
#define PPUTLIMPL_UDEC_3885u 0xF2Du, 0, 0, 1, 0, 11, 62, 3, 5, 7, 37
#define PPUTLIMPL_UDEC_3884u 0xF2Cu, 0, 0, 1, 0, 11, 62, 2, 2, 971
#define PPUTLIMPL_UDEC_3883u 0xF2Bu, 0, 0, 1, 0, 11, 62, 11, 353
#define PPUTLIMPL_UDEC_3882u 0xF2Au, 0, 0, 1, 0, 11, 62, 2, 3, 647
#define PPUTLIMPL_UDEC_3881u 0xF29u, 0, 0, 1, 0, 11, 62,
#define PPUTLIMPL_UDEC_3880u 0xF28u, 0, 0, 1, 0, 11, 62, 2, 2, 2, 5, 97
#define PPUTLIMPL_UDEC_3879u 0xF27u, 0, 0, 1, 0, 11, 62, 3, 3, 431
#define PPUTLIMPL_UDEC_3878u 0xF26u, 0, 0, 1, 0, 11, 62, 2, 7, 277
#define PPUTLIMPL_UDEC_3877u 0xF25u, 0, 0, 1, 0, 11, 62,
#define PPUTLIMPL_UDEC_3876u 0xF24u, 0, 0, 1, 0, 11, 62, 2, 2, 3, 17, 19
#define PPUTLIMPL_UDEC_3875u 0xF23u, 0, 0, 1, 0, 11, 62, 5, 5, 5, 31
#define PPUTLIMPL_UDEC_3874u 0xF22u, 0, 0, 1, 0, 11, 62, 2, 13, 149
#define PPUTLIMPL_UDEC_3873u 0xF21u, 0, 0, 1, 0, 11, 62, 3, 1291
#define PPUTLIMPL_UDEC_3872u 0xF20u, 0, 0, 1, 0, 11, 62, 2, 2, 2, 2, 2, 11, 11
#define PPUTLIMPL_UDEC_3871u 0xF1Fu, 0, 0, 1, 0, 11, 62, 7, 7, 79
#define PPUTLIMPL_UDEC_3870u 0xF1Eu, 0, 0, 1, 0, 11, 62, 2, 3, 3, 5, 43
#define PPUTLIMPL_UDEC_3869u 0xF1Du, 0, 0, 1, 0, 11, 62, 53, 73
#define PPUTLIMPL_UDEC_3868u 0xF1Cu, 0, 0, 1, 0, 11, 62, 2, 2, 967
#define PPUTLIMPL_UDEC_3867u 0xF1Bu, 0, 0, 1, 0, 11, 62, 3, 1289
#define PPUTLIMPL_UDEC_3866u 0xF1Au, 0, 0, 1, 0, 11, 62, 2, 1933
#define PPUTLIMPL_UDEC_3865u 0xF19u, 0, 0, 1, 0, 11, 62, 5, 773
#define PPUTLIMPL_UDEC_3864u 0xF18u, 0, 0, 1, 0, 11, 62, 2, 2, 2, 3, 7, 23
#define PPUTLIMPL_UDEC_3863u 0xF17u, 0, 0, 1, 0, 11, 62,
#define PPUTLIMPL_UDEC_3862u 0xF16u, 0, 0, 1, 0, 11, 62, 2, 1931
#define PPUTLIMPL_UDEC_3861u 0xF15u, 0, 0, 1, 0, 11, 62, 3, 3, 3, 11, 13
#define PPUTLIMPL_UDEC_3860u 0xF14u, 0, 0, 1, 0, 11, 62, 2, 2, 5, 193
#define PPUTLIMPL_UDEC_3859u 0xF13u, 0, 0, 1, 0, 11, 62, 17, 227
#define PPUTLIMPL_UDEC_3858u 0xF12u, 0, 0, 1, 0, 11, 62, 2, 3, 643
#define PPUTLIMPL_UDEC_3857u 0xF11u, 0, 0, 1, 0, 11, 62, 7, 19, 29
#define PPUTLIMPL_UDEC_3856u 0xF10u, 0, 0, 1, 0, 11, 62, 2, 2, 2, 2, 241
#define PPUTLIMPL_UDEC_3855u 0xF0Fu, 0, 0, 1, 0, 11, 62, 3, 5, 257
#define PPUTLIMPL_UDEC_3854u 0xF0Eu, 0, 0, 1, 0, 11, 62, 2, 41, 47
#define PPUTLIMPL_UDEC_3853u 0xF0Du, 0, 0, 1, 0, 11, 62,
#define PPUTLIMPL_UDEC_3852u 0xF0Cu, 0, 0, 1, 0, 11, 62, 2, 2, 3, 3, 107
#define PPUTLIMPL_UDEC_3851u 0xF0Bu, 0, 0, 1, 0, 11, 62,
#define PPUTLIMPL_UDEC_3850u 0xF0Au, 0, 0, 1, 0, 11, 62, 2, 5, 5, 7, 11
#define PPUTLIMPL_UDEC_3849u 0xF09u, 0, 0, 1, 0, 11, 62, 3, 1283
#define PPUTLIMPL_UDEC_3848u 0xF08u, 0, 0, 1, 0, 11, 62, 2, 2, 2, 13, 37
#define PPUTLIMPL_UDEC_3847u 0xF07u, 0, 0, 1, 0, 11, 62,
#define PPUTLIMPL_UDEC_3846u 0xF06u, 0, 0, 1, 0, 11, 62, 2, 3, 641
#define PPUTLIMPL_UDEC_3845u 0xF05u, 0, 0, 1, 0, 11, 62, 5, 769
#define PPUTLIMPL_UDEC_3844u 0xF04u, 0, 0, 1, 0, 11, 62, 2, 2, 31, 31
#define PPUTLIMPL_UDEC_3843u 0xF03u, 0, 0, 1, 0, 11, 61, 3, 3, 7, 61
#define PPUTLIMPL_UDEC_3842u 0xF02u, 0, 0, 1, 0, 11, 61, 2, 17, 113
#define PPUTLIMPL_UDEC_3841u 0xF01u, 0, 0, 1, 0, 11, 61, 23, 167
#define PPUTLIMPL_UDEC_3840u 0xF00u, 0, 0, 0, 0, 11, 61, 2, 2, 2, 2, 2, 2, 2, 2, 3, 5
#define PPUTLIMPL_UDEC_3839u 0xEFFu, 0, 0, 0, 0, 11, 61, 11, 349
#define PPUTLIMPL_UDEC_3838u 0xEFEu, 0, 0, 0, 0, 11, 61, 2, 19, 101
#define PPUTLIMPL_UDEC_3837u 0xEFDu, 0, 0, 0, 0, 11, 61, 3, 1279
#define PPUTLIMPL_UDEC_3836u 0xEFCu, 0, 0, 0, 0, 11, 61, 2, 2, 7, 137
#define PPUTLIMPL_UDEC_3835u 0xEFBu, 0, 0, 0, 0, 11, 61, 5, 13, 59
#define PPUTLIMPL_UDEC_3834u 0xEFAu, 0, 0, 0, 0, 11, 61, 2, 3, 3, 3, 71
#define PPUTLIMPL_UDEC_3833u 0xEF9u, 0, 0, 0, 0, 11, 61,
#define PPUTLIMPL_UDEC_3832u 0xEF8u, 0, 0, 0, 0, 11, 61, 2, 2, 2, 479
#define PPUTLIMPL_UDEC_3831u 0xEF7u, 0, 0, 0, 0, 11, 61, 3, 1277
#define PPUTLIMPL_UDEC_3830u 0xEF6u, 0, 0, 0, 0, 11, 61, 2, 5, 383
#define PPUTLIMPL_UDEC_3829u 0xEF5u, 0, 0, 0, 0, 11, 61, 7, 547
#define PPUTLIMPL_UDEC_3828u 0xEF4u, 0, 0, 0, 0, 11, 61, 2, 2, 3, 11, 29
#define PPUTLIMPL_UDEC_3827u 0xEF3u, 0, 0, 0, 0, 11, 61, 43, 89
#define PPUTLIMPL_UDEC_3826u 0xEF2u, 0, 0, 0, 0, 11, 61, 2, 1913
#define PPUTLIMPL_UDEC_3825u 0xEF1u, 0, 0, 0, 0, 11, 61, 3, 3, 5, 5, 17
#define PPUTLIMPL_UDEC_3824u 0xEF0u, 0, 0, 0, 0, 11, 61, 2, 2, 2, 2, 239
#define PPUTLIMPL_UDEC_3823u 0xEEFu, 0, 0, 0, 0, 11, 61,
#define PPUTLIMPL_UDEC_3822u 0xEEEu, 0, 0, 0, 0, 11, 61, 2, 3, 7, 7, 13
#define PPUTLIMPL_UDEC_3821u 0xEEDu, 0, 0, 0, 0, 11, 61,
#define PPUTLIMPL_UDEC_3820u 0xEECu, 0, 0, 0, 0, 11, 61, 2, 2, 5, 191
#define PPUTLIMPL_UDEC_3819u 0xEEBu, 0, 0, 0, 0, 11, 61, 3, 19, 67
#define PPUTLIMPL_UDEC_3818u 0xEEAu, 0, 0, 0, 0, 11, 61, 2, 23, 83
#define PPUTLIMPL_UDEC_3817u 0xEE9u, 0, 0, 0, 0, 11, 61, 11, 347
#define PPUTLIMPL_UDEC_3816u 0xEE8u, 0, 0, 0, 0, 11, 61, 2, 2, 2, 3, 3, 53
#define PPUTLIMPL_UDEC_3815u 0xEE7u, 0, 0, 0, 0, 11, 61, 5, 7, 109
#define PPUTLIMPL_UDEC_3814u 0xEE6u, 0, 0, 0, 0, 11, 61, 2, 1907
#define PPUTLIMPL_UDEC_3813u 0xEE5u, 0, 0, 0, 0, 11, 61, 3, 31, 41
#define PPUTLIMPL_UDEC_3812u 0xEE4u, 0, 0, 0, 0, 11, 61, 2, 2, 953
#define PPUTLIMPL_UDEC_3811u 0xEE3u, 0, 0, 0, 0, 11, 61, 37, 103
#define PPUTLIMPL_UDEC_3810u 0xEE2u, 0, 0, 0, 0, 11, 61, 2, 3, 5, 127
#define PPUTLIMPL_UDEC_3809u 0xEE1u, 0, 0, 0, 0, 11, 61, 13, 293
#define PPUTLIMPL_UDEC_3808u 0xEE0u, 0, 0, 0, 0, 11, 61, 2, 2, 2, 2, 2, 7, 17
#define PPUTLIMPL_UDEC_3807u 0xEDFu, 0, 0, 0, 0, 11, 61, 3, 3, 3, 3, 47
#define PPUTLIMPL_UDEC_3806u 0xEDEu, 0, 0, 0, 0, 11, 61, 2, 11, 173
#define PPUTLIMPL_UDEC_3805u 0xEDDu, 0, 0, 0, 0, 11, 61, 5, 761
#define PPUTLIMPL_UDEC_3804u 0xEDCu, 0, 0, 0, 0, 11, 61, 2, 2, 3, 317
#define PPUTLIMPL_UDEC_3803u 0xEDBu, 0, 0, 0, 0, 11, 61,
#define PPUTLIMPL_UDEC_3802u 0xEDAu, 0, 0, 0, 0, 11, 61, 2, 1901
#define PPUTLIMPL_UDEC_3801u 0xED9u, 0, 0, 0, 0, 11, 61, 3, 7, 181
#define PPUTLIMPL_UDEC_3800u 0xED8u, 0, 0, 0, 0, 11, 61, 2, 2, 2, 5, 5, 19
#define PPUTLIMPL_UDEC_3799u 0xED7u, 0, 0, 0, 0, 11, 61, 29, 131
#define PPUTLIMPL_UDEC_3798u 0xED6u, 0, 0, 0, 0, 11, 61, 2, 3, 3, 211
#define PPUTLIMPL_UDEC_3797u 0xED5u, 0, 0, 0, 0, 11, 61,
#define PPUTLIMPL_UDEC_3796u 0xED4u, 0, 0, 0, 0, 11, 61, 2, 2, 13, 73
#define PPUTLIMPL_UDEC_3795u 0xED3u, 0, 0, 0, 0, 11, 61, 3, 5, 11, 23
#define PPUTLIMPL_UDEC_3794u 0xED2u, 0, 0, 0, 0, 11, 61, 2, 7, 271
#define PPUTLIMPL_UDEC_3793u 0xED1u, 0, 0, 0, 0, 11, 61,
#define PPUTLIMPL_UDEC_3792u 0xED0u, 0, 0, 0, 0, 11, 61, 2, 2, 2, 2, 3, 79
#define PPUTLIMPL_UDEC_3791u 0xECFu, 0, 0, 0, 0, 11, 61, 17, 223
#define PPUTLIMPL_UDEC_3790u 0xECEu, 0, 0, 0, 0, 11, 61, 2, 5, 379
#define PPUTLIMPL_UDEC_3789u 0xECDu, 0, 0, 0, 0, 11, 61, 3, 3, 421
#define PPUTLIMPL_UDEC_3788u 0xECCu, 0, 0, 0, 0, 11, 61, 2, 2, 947
#define PPUTLIMPL_UDEC_3787u 0xECBu, 0, 0, 0, 0, 11, 61, 7, 541
#define PPUTLIMPL_UDEC_3786u 0xECAu, 0, 0, 0, 0, 11, 61, 2, 3, 631
#define PPUTLIMPL_UDEC_3785u 0xEC9u, 0, 0, 0, 0, 11, 61, 5, 757
#define PPUTLIMPL_UDEC_3784u 0xEC8u, 0, 0, 0, 0, 11, 61, 2, 2, 2, 11, 43
#define PPUTLIMPL_UDEC_3783u 0xEC7u, 0, 0, 0, 0, 11, 61, 3, 13, 97
#define PPUTLIMPL_UDEC_3782u 0xEC6u, 0, 0, 0, 0, 11, 61, 2, 31, 61
#define PPUTLIMPL_UDEC_3781u 0xEC5u, 0, 0, 0, 0, 11, 61, 19, 199
#define PPUTLIMPL_UDEC_3780u 0xEC4u, 0, 0, 0, 0, 11, 61, 2, 2, 3, 3, 3, 5, 7
#define PPUTLIMPL_UDEC_3779u 0xEC3u, 0, 0, 0, 0, 11, 61,
#define PPUTLIMPL_UDEC_3778u 0xEC2u, 0, 0, 0, 0, 11, 61, 2, 1889
#define PPUTLIMPL_UDEC_3777u 0xEC1u, 0, 0, 0, 0, 11, 61, 3, 1259
#define PPUTLIMPL_UDEC_3776u 0xEC0u, 0, 0, 0, 0, 11, 61, 2, 2, 2, 2, 2, 2, 59
#define PPUTLIMPL_UDEC_3775u 0xEBFu, 0, 0, 0, 0, 11, 61, 5, 5, 151
#define PPUTLIMPL_UDEC_3774u 0xEBEu, 0, 0, 0, 0, 11, 61, 2, 3, 17, 37
#define PPUTLIMPL_UDEC_3773u 0xEBDu, 0, 0, 0, 0, 11, 61, 7, 7, 7, 11
#define PPUTLIMPL_UDEC_3772u 0xEBCu, 0, 0, 0, 0, 11, 61, 2, 2, 23, 41
#define PPUTLIMPL_UDEC_3771u 0xEBBu, 0, 0, 0, 0, 11, 61, 3, 3, 419
#define PPUTLIMPL_UDEC_3770u 0xEBAu, 0, 0, 0, 0, 11, 61, 2, 5, 13, 29
#define PPUTLIMPL_UDEC_3769u 0xEB9u, 0, 0, 0, 0, 11, 61,
#define PPUTLIMPL_UDEC_3768u 0xEB8u, 0, 0, 0, 0, 11, 61, 2, 2, 2, 3, 157
#define PPUTLIMPL_UDEC_3767u 0xEB7u, 0, 0, 0, 0, 11, 61,
#define PPUTLIMPL_UDEC_3766u 0xEB6u, 0, 0, 0, 0, 11, 61, 2, 7, 269
#define PPUTLIMPL_UDEC_3765u 0xEB5u, 0, 0, 0, 0, 11, 61, 3, 5, 251
#define PPUTLIMPL_UDEC_3764u 0xEB4u, 0, 0, 0, 0, 11, 61, 2, 2, 941
#define PPUTLIMPL_UDEC_3763u 0xEB3u, 0, 0, 0, 0, 11, 61, 53, 71
#define PPUTLIMPL_UDEC_3762u 0xEB2u, 0, 0, 0, 0, 11, 61, 2, 3, 3, 11, 19
#define PPUTLIMPL_UDEC_3761u 0xEB1u, 0, 0, 0, 0, 11, 61,
#define PPUTLIMPL_UDEC_3760u 0xEB0u, 0, 0, 0, 0, 11, 61, 2, 2, 2, 2, 5, 47
#define PPUTLIMPL_UDEC_3759u 0xEAFu, 0, 0, 0, 0, 11, 61, 3, 7, 179
#define PPUTLIMPL_UDEC_3758u 0xEAEu, 0, 0, 0, 0, 11, 61, 2, 1879
#define PPUTLIMPL_UDEC_3757u 0xEADu, 0, 0, 0, 0, 11, 61, 13, 17, 17
#define PPUTLIMPL_UDEC_3756u 0xEACu, 0, 0, 0, 0, 11, 61, 2, 2, 3, 313
#define PPUTLIMPL_UDEC_3755u 0xEABu, 0, 0, 0, 0, 11, 61, 5, 751
#define PPUTLIMPL_UDEC_3754u 0xEAAu, 0, 0, 0, 0, 11, 61, 2, 1877
#define PPUTLIMPL_UDEC_3753u 0xEA9u, 0, 0, 0, 0, 11, 61, 3, 3, 3, 139
#define PPUTLIMPL_UDEC_3752u 0xEA8u, 0, 0, 0, 0, 11, 61, 2, 2, 2, 7, 67
#define PPUTLIMPL_UDEC_3751u 0xEA7u, 0, 0, 0, 0, 11, 61, 11, 11, 31
#define PPUTLIMPL_UDEC_3750u 0xEA6u, 0, 0, 0, 0, 11, 61, 2, 3, 5, 5, 5, 5
#define PPUTLIMPL_UDEC_3749u 0xEA5u, 0, 0, 0, 0, 11, 61, 23, 163
#define PPUTLIMPL_UDEC_3748u 0xEA4u, 0, 0, 0, 0, 11, 61, 2, 2, 937
#define PPUTLIMPL_UDEC_3747u 0xEA3u, 0, 0, 0, 0, 11, 61, 3, 1249
#define PPUTLIMPL_UDEC_3746u 0xEA2u, 0, 0, 0, 0, 11, 61, 2, 1873
#define PPUTLIMPL_UDEC_3745u 0xEA1u, 0, 0, 0, 0, 11, 61, 5, 7, 107
#define PPUTLIMPL_UDEC_3744u 0xEA0u, 0, 0, 0, 0, 11, 61, 2, 2, 2, 2, 2, 3, 3, 13
#define PPUTLIMPL_UDEC_3743u 0xE9Fu, 0, 0, 0, 0, 11, 61, 19, 197
#define PPUTLIMPL_UDEC_3742u 0xE9Eu, 0, 0, 0, 0, 11, 61, 2, 1871
#define PPUTLIMPL_UDEC_3741u 0xE9Du, 0, 0, 0, 0, 11, 61, 3, 29, 43
#define PPUTLIMPL_UDEC_3740u 0xE9Cu, 0, 0, 0, 0, 11, 61, 2, 2, 5, 11, 17
#define PPUTLIMPL_UDEC_3739u 0xE9Bu, 0, 0, 0, 0, 11, 61,
#define PPUTLIMPL_UDEC_3738u 0xE9Au, 0, 0, 0, 0, 11, 61, 2, 3, 7, 89
#define PPUTLIMPL_UDEC_3737u 0xE99u, 0, 0, 0, 0, 11, 61, 37, 101
#define PPUTLIMPL_UDEC_3736u 0xE98u, 0, 0, 0, 0, 11, 61, 2, 2, 2, 467
#define PPUTLIMPL_UDEC_3735u 0xE97u, 0, 0, 0, 0, 11, 61, 3, 3, 5, 83
#define PPUTLIMPL_UDEC_3734u 0xE96u, 0, 0, 0, 0, 11, 61, 2, 1867
#define PPUTLIMPL_UDEC_3733u 0xE95u, 0, 0, 0, 0, 11, 61,
#define PPUTLIMPL_UDEC_3732u 0xE94u, 0, 0, 0, 0, 11, 61, 2, 2, 3, 311
#define PPUTLIMPL_UDEC_3731u 0xE93u, 0, 0, 0, 0, 11, 61, 7, 13, 41
#define PPUTLIMPL_UDEC_3730u 0xE92u, 0, 0, 0, 0, 11, 61, 2, 5, 373
#define PPUTLIMPL_UDEC_3729u 0xE91u, 0, 0, 0, 0, 11, 61, 3, 11, 113
#define PPUTLIMPL_UDEC_3728u 0xE90u, 0, 0, 0, 0, 11, 61, 2, 2, 2, 2, 233
#define PPUTLIMPL_UDEC_3727u 0xE8Fu, 0, 0, 0, 0, 11, 61,
#define PPUTLIMPL_UDEC_3726u 0xE8Eu, 0, 0, 0, 0, 11, 61, 2, 3, 3, 3, 3, 23
#define PPUTLIMPL_UDEC_3725u 0xE8Du, 0, 0, 0, 0, 11, 61, 5, 5, 149
#define PPUTLIMPL_UDEC_3724u 0xE8Cu, 0, 0, 0, 0, 11, 61, 2, 2, 7, 7, 19
#define PPUTLIMPL_UDEC_3723u 0xE8Bu, 0, 0, 0, 0, 11, 61, 3, 17, 73
#define PPUTLIMPL_UDEC_3722u 0xE8Au, 0, 0, 0, 0, 11, 61, 2, 1861
#define PPUTLIMPL_UDEC_3721u 0xE89u, 0, 0, 0, 0, 11, 61, 61, 61
#define PPUTLIMPL_UDEC_3720u 0xE88u, 0, 0, 0, 0, 11, 60, 2, 2, 2, 3, 5, 31
#define PPUTLIMPL_UDEC_3719u 0xE87u, 0, 0, 0, 0, 11, 60,
#define PPUTLIMPL_UDEC_3718u 0xE86u, 0, 0, 0, 0, 11, 60, 2, 11, 13, 13
#define PPUTLIMPL_UDEC_3717u 0xE85u, 0, 0, 0, 0, 11, 60, 3, 3, 7, 59
#define PPUTLIMPL_UDEC_3716u 0xE84u, 0, 0, 0, 0, 11, 60, 2, 2, 929
#define PPUTLIMPL_UDEC_3715u 0xE83u, 0, 0, 0, 0, 11, 60, 5, 743
#define PPUTLIMPL_UDEC_3714u 0xE82u, 0, 0, 0, 0, 11, 60, 2, 3, 619
#define PPUTLIMPL_UDEC_3713u 0xE81u, 0, 0, 0, 0, 11, 60, 47, 79
#define PPUTLIMPL_UDEC_3712u 0xE80u, 0, 0, 0, 0, 11, 60, 2, 2, 2, 2, 2, 2, 2, 29
#define PPUTLIMPL_UDEC_3711u 0xE7Fu, 0, 0, 0, 0, 11, 60, 3, 1237
#define PPUTLIMPL_UDEC_3710u 0xE7Eu, 0, 0, 0, 0, 11, 60, 2, 5, 7, 53
#define PPUTLIMPL_UDEC_3709u 0xE7Du, 0, 0, 0, 0, 11, 60,
#define PPUTLIMPL_UDEC_3708u 0xE7Cu, 0, 0, 0, 0, 11, 60, 2, 2, 3, 3, 103
#define PPUTLIMPL_UDEC_3707u 0xE7Bu, 0, 0, 0, 0, 11, 60, 11, 337
#define PPUTLIMPL_UDEC_3706u 0xE7Au, 0, 0, 0, 0, 11, 60, 2, 17, 109
#define PPUTLIMPL_UDEC_3705u 0xE79u, 0, 0, 0, 0, 11, 60, 3, 5, 13, 19
#define PPUTLIMPL_UDEC_3704u 0xE78u, 0, 0, 0, 0, 11, 60, 2, 2, 2, 463
#define PPUTLIMPL_UDEC_3703u 0xE77u, 0, 0, 0, 0, 11, 60, 7, 23, 23
#define PPUTLIMPL_UDEC_3702u 0xE76u, 0, 0, 0, 0, 11, 60, 2, 3, 617
#define PPUTLIMPL_UDEC_3701u 0xE75u, 0, 0, 0, 0, 11, 60,
#define PPUTLIMPL_UDEC_3700u 0xE74u, 0, 0, 0, 0, 11, 60, 2, 2, 5, 5, 37
#define PPUTLIMPL_UDEC_3699u 0xE73u, 0, 0, 0, 0, 11, 60, 3, 3, 3, 137
#define PPUTLIMPL_UDEC_3698u 0xE72u, 0, 0, 0, 0, 11, 60, 2, 43, 43
#define PPUTLIMPL_UDEC_3697u 0xE71u, 0, 0, 0, 0, 11, 60,
#define PPUTLIMPL_UDEC_3696u 0xE70u, 0, 0, 0, 0, 11, 60, 2, 2, 2, 2, 3, 7, 11
#define PPUTLIMPL_UDEC_3695u 0xE6Fu, 0, 0, 0, 0, 11, 60, 5, 739
#define PPUTLIMPL_UDEC_3694u 0xE6Eu, 0, 0, 0, 0, 11, 60, 2, 1847
#define PPUTLIMPL_UDEC_3693u 0xE6Du, 0, 0, 0, 0, 11, 60, 3, 1231
#define PPUTLIMPL_UDEC_3692u 0xE6Cu, 0, 0, 0, 0, 11, 60, 2, 2, 13, 71
#define PPUTLIMPL_UDEC_3691u 0xE6Bu, 0, 0, 0, 0, 11, 60,
#define PPUTLIMPL_UDEC_3690u 0xE6Au, 0, 0, 0, 0, 11, 60, 2, 3, 3, 5, 41
#define PPUTLIMPL_UDEC_3689u 0xE69u, 0, 0, 0, 0, 11, 60, 7, 17, 31
#define PPUTLIMPL_UDEC_3688u 0xE68u, 0, 0, 0, 0, 11, 60, 2, 2, 2, 461
#define PPUTLIMPL_UDEC_3687u 0xE67u, 0, 0, 0, 0, 11, 60, 3, 1229
#define PPUTLIMPL_UDEC_3686u 0xE66u, 0, 0, 0, 0, 11, 60, 2, 19, 97
#define PPUTLIMPL_UDEC_3685u 0xE65u, 0, 0, 0, 0, 11, 60, 5, 11, 67
#define PPUTLIMPL_UDEC_3684u 0xE64u, 0, 0, 0, 0, 11, 60, 2, 2, 3, 307
#define PPUTLIMPL_UDEC_3683u 0xE63u, 0, 0, 0, 0, 11, 60, 29, 127
#define PPUTLIMPL_UDEC_3682u 0xE62u, 0, 0, 0, 0, 11, 60, 2, 7, 263
#define PPUTLIMPL_UDEC_3681u 0xE61u, 0, 0, 0, 0, 11, 60, 3, 3, 409
#define PPUTLIMPL_UDEC_3680u 0xE60u, 0, 0, 0, 0, 11, 60, 2, 2, 2, 2, 2, 5, 23
#define PPUTLIMPL_UDEC_3679u 0xE5Fu, 0, 0, 0, 0, 11, 60, 13, 283
#define PPUTLIMPL_UDEC_3678u 0xE5Eu, 0, 0, 0, 0, 11, 60, 2, 3, 613
#define PPUTLIMPL_UDEC_3677u 0xE5Du, 0, 0, 0, 0, 11, 60,
#define PPUTLIMPL_UDEC_3676u 0xE5Cu, 0, 0, 0, 0, 11, 60, 2, 2, 919
#define PPUTLIMPL_UDEC_3675u 0xE5Bu, 0, 0, 0, 0, 11, 60, 3, 5, 5, 7, 7
#define PPUTLIMPL_UDEC_3674u 0xE5Au, 0, 0, 0, 0, 11, 60, 2, 11, 167
#define PPUTLIMPL_UDEC_3673u 0xE59u, 0, 0, 0, 0, 11, 60,
#define PPUTLIMPL_UDEC_3672u 0xE58u, 0, 0, 0, 0, 11, 60, 2, 2, 2, 3, 3, 3, 17
#define PPUTLIMPL_UDEC_3671u 0xE57u, 0, 0, 0, 0, 11, 60,
#define PPUTLIMPL_UDEC_3670u 0xE56u, 0, 0, 0, 0, 11, 60, 2, 5, 367
#define PPUTLIMPL_UDEC_3669u 0xE55u, 0, 0, 0, 0, 11, 60, 3, 1223
#define PPUTLIMPL_UDEC_3668u 0xE54u, 0, 0, 0, 0, 11, 60, 2, 2, 7, 131
#define PPUTLIMPL_UDEC_3667u 0xE53u, 0, 0, 0, 0, 11, 60, 19, 193
#define PPUTLIMPL_UDEC_3666u 0xE52u, 0, 0, 0, 0, 11, 60, 2, 3, 13, 47
#define PPUTLIMPL_UDEC_3665u 0xE51u, 0, 0, 0, 0, 11, 60, 5, 733
#define PPUTLIMPL_UDEC_3664u 0xE50u, 0, 0, 0, 0, 11, 60, 2, 2, 2, 2, 229
#define PPUTLIMPL_UDEC_3663u 0xE4Fu, 0, 0, 0, 0, 11, 60, 3, 3, 11, 37
#define PPUTLIMPL_UDEC_3662u 0xE4Eu, 0, 0, 0, 0, 11, 60, 2, 1831
#define PPUTLIMPL_UDEC_3661u 0xE4Du, 0, 0, 0, 0, 11, 60, 7, 523
#define PPUTLIMPL_UDEC_3660u 0xE4Cu, 0, 0, 0, 0, 11, 60, 2, 2, 3, 5, 61
#define PPUTLIMPL_UDEC_3659u 0xE4Bu, 0, 0, 0, 0, 11, 60,
#define PPUTLIMPL_UDEC_3658u 0xE4Au, 0, 0, 0, 0, 11, 60, 2, 31, 59
#define PPUTLIMPL_UDEC_3657u 0xE49u, 0, 0, 0, 0, 11, 60, 3, 23, 53
#define PPUTLIMPL_UDEC_3656u 0xE48u, 0, 0, 0, 0, 11, 60, 2, 2, 2, 457
#define PPUTLIMPL_UDEC_3655u 0xE47u, 0, 0, 0, 0, 11, 60, 5, 17, 43
#define PPUTLIMPL_UDEC_3654u 0xE46u, 0, 0, 0, 0, 11, 60, 2, 3, 3, 7, 29
#define PPUTLIMPL_UDEC_3653u 0xE45u, 0, 0, 0, 0, 11, 60, 13, 281
#define PPUTLIMPL_UDEC_3652u 0xE44u, 0, 0, 0, 0, 11, 60, 2, 2, 11, 83
#define PPUTLIMPL_UDEC_3651u 0xE43u, 0, 0, 0, 0, 11, 60, 3, 1217
#define PPUTLIMPL_UDEC_3650u 0xE42u, 0, 0, 0, 0, 11, 60, 2, 5, 5, 73
#define PPUTLIMPL_UDEC_3649u 0xE41u, 0, 0, 0, 0, 11, 60, 41, 89
#define PPUTLIMPL_UDEC_3648u 0xE40u, 0, 0, 0, 0, 11, 60, 2, 2, 2, 2, 2, 2, 3, 19
#define PPUTLIMPL_UDEC_3647u 0xE3Fu, 0, 0, 0, 0, 11, 60, 7, 521
#define PPUTLIMPL_UDEC_3646u 0xE3Eu, 0, 0, 0, 0, 11, 60, 2, 1823
#define PPUTLIMPL_UDEC_3645u 0xE3Du, 0, 0, 0, 0, 11, 60, 3, 3, 3, 3, 3, 3, 5
#define PPUTLIMPL_UDEC_3644u 0xE3Cu, 0, 0, 0, 0, 11, 60, 2, 2, 911
#define PPUTLIMPL_UDEC_3643u 0xE3Bu, 0, 0, 0, 0, 11, 60,
#define PPUTLIMPL_UDEC_3642u 0xE3Au, 0, 0, 0, 0, 11, 60, 2, 3, 607
#define PPUTLIMPL_UDEC_3641u 0xE39u, 0, 0, 0, 0, 11, 60, 11, 331
#define PPUTLIMPL_UDEC_3640u 0xE38u, 0, 0, 0, 0, 11, 60, 2, 2, 2, 5, 7, 13
#define PPUTLIMPL_UDEC_3639u 0xE37u, 0, 0, 0, 0, 11, 60, 3, 1213
#define PPUTLIMPL_UDEC_3638u 0xE36u, 0, 0, 0, 0, 11, 60, 2, 17, 107
#define PPUTLIMPL_UDEC_3637u 0xE35u, 0, 0, 0, 0, 11, 60,
#define PPUTLIMPL_UDEC_3636u 0xE34u, 0, 0, 0, 0, 11, 60, 2, 2, 3, 3, 101
#define PPUTLIMPL_UDEC_3635u 0xE33u, 0, 0, 0, 0, 11, 60, 5, 727
#define PPUTLIMPL_UDEC_3634u 0xE32u, 0, 0, 0, 0, 11, 60, 2, 23, 79
#define PPUTLIMPL_UDEC_3633u 0xE31u, 0, 0, 0, 0, 11, 60, 3, 7, 173
#define PPUTLIMPL_UDEC_3632u 0xE30u, 0, 0, 0, 0, 11, 60, 2, 2, 2, 2, 227
#define PPUTLIMPL_UDEC_3631u 0xE2Fu, 0, 0, 0, 0, 11, 60,
#define PPUTLIMPL_UDEC_3630u 0xE2Eu, 0, 0, 0, 0, 11, 60, 2, 3, 5, 11, 11
#define PPUTLIMPL_UDEC_3629u 0xE2Du, 0, 0, 0, 0, 11, 60, 19, 191
#define PPUTLIMPL_UDEC_3628u 0xE2Cu, 0, 0, 0, 0, 11, 60, 2, 2, 907
#define PPUTLIMPL_UDEC_3627u 0xE2Bu, 0, 0, 0, 0, 11, 60, 3, 3, 13, 31
#define PPUTLIMPL_UDEC_3626u 0xE2Au, 0, 0, 0, 0, 11, 60, 2, 7, 7, 37
#define PPUTLIMPL_UDEC_3625u 0xE29u, 0, 0, 0, 0, 11, 60, 5, 5, 5, 29
#define PPUTLIMPL_UDEC_3624u 0xE28u, 0, 0, 0, 0, 11, 60, 2, 2, 2, 3, 151
#define PPUTLIMPL_UDEC_3623u 0xE27u, 0, 0, 0, 0, 11, 60,
#define PPUTLIMPL_UDEC_3622u 0xE26u, 0, 0, 0, 0, 11, 60, 2, 1811
#define PPUTLIMPL_UDEC_3621u 0xE25u, 0, 0, 0, 0, 11, 60, 3, 17, 71
#define PPUTLIMPL_UDEC_3620u 0xE24u, 0, 0, 0, 0, 11, 60, 2, 2, 5, 181
#define PPUTLIMPL_UDEC_3619u 0xE23u, 0, 0, 0, 0, 11, 60, 7, 11, 47
#define PPUTLIMPL_UDEC_3618u 0xE22u, 0, 0, 0, 0, 11, 60, 2, 3, 3, 3, 67
#define PPUTLIMPL_UDEC_3617u 0xE21u, 0, 0, 0, 0, 11, 60,
#define PPUTLIMPL_UDEC_3616u 0xE20u, 0, 0, 0, 0, 11, 60, 2, 2, 2, 2, 2, 113
#define PPUTLIMPL_UDEC_3615u 0xE1Fu, 0, 0, 0, 0, 11, 60, 3, 5, 241
#define PPUTLIMPL_UDEC_3614u 0xE1Eu, 0, 0, 0, 0, 11, 60, 2, 13, 139
#define PPUTLIMPL_UDEC_3613u 0xE1Du, 0, 0, 0, 0, 11, 60,
#define PPUTLIMPL_UDEC_3612u 0xE1Cu, 0, 0, 0, 0, 11, 60, 2, 2, 3, 7, 43
#define PPUTLIMPL_UDEC_3611u 0xE1Bu, 0, 0, 0, 0, 11, 60, 23, 157
#define PPUTLIMPL_UDEC_3610u 0xE1Au, 0, 0, 0, 0, 11, 60, 2, 5, 19, 19
#define PPUTLIMPL_UDEC_3609u 0xE19u, 0, 0, 0, 0, 11, 60, 3, 3, 401
#define PPUTLIMPL_UDEC_3608u 0xE18u, 0, 0, 0, 0, 11, 60, 2, 2, 2, 11, 41
#define PPUTLIMPL_UDEC_3607u 0xE17u, 0, 0, 0, 0, 11, 60,
#define PPUTLIMPL_UDEC_3606u 0xE16u, 0, 0, 0, 0, 11, 60, 2, 3, 601
#define PPUTLIMPL_UDEC_3605u 0xE15u, 0, 0, 0, 0, 11, 60, 5, 7, 103
#define PPUTLIMPL_UDEC_3604u 0xE14u, 0, 0, 0, 0, 11, 60, 2, 2, 17, 53
#define PPUTLIMPL_UDEC_3603u 0xE13u, 0, 0, 0, 0, 11, 60, 3, 1201
#define PPUTLIMPL_UDEC_3602u 0xE12u, 0, 0, 0, 0, 11, 60, 2, 1801
#define PPUTLIMPL_UDEC_3601u 0xE11u, 0, 0, 0, 0, 11, 60, 13, 277
#define PPUTLIMPL_UDEC_3600u 0xE10u, 0, 0, 0, 0, 11, 60, 2, 2, 2, 2, 3, 3, 5, 5
#define PPUTLIMPL_UDEC_3599u 0xE0Fu, 0, 0, 0, 0, 11, 59, 59, 61
#define PPUTLIMPL_UDEC_3598u 0xE0Eu, 0, 0, 0, 0, 11, 59, 2, 7, 257
#define PPUTLIMPL_UDEC_3597u 0xE0Du, 0, 0, 0, 0, 11, 59, 3, 11, 109
#define PPUTLIMPL_UDEC_3596u 0xE0Cu, 0, 0, 0, 0, 11, 59, 2, 2, 29, 31
#define PPUTLIMPL_UDEC_3595u 0xE0Bu, 0, 0, 0, 0, 11, 59, 5, 719
#define PPUTLIMPL_UDEC_3594u 0xE0Au, 0, 0, 0, 0, 11, 59, 2, 3, 599
#define PPUTLIMPL_UDEC_3593u 0xE09u, 0, 0, 0, 0, 11, 59,
#define PPUTLIMPL_UDEC_3592u 0xE08u, 0, 0, 0, 0, 11, 59, 2, 2, 2, 449
#define PPUTLIMPL_UDEC_3591u 0xE07u, 0, 0, 0, 0, 11, 59, 3, 3, 3, 7, 19
#define PPUTLIMPL_UDEC_3590u 0xE06u, 0, 0, 0, 0, 11, 59, 2, 5, 359
#define PPUTLIMPL_UDEC_3589u 0xE05u, 0, 0, 0, 0, 11, 59, 37, 97
#define PPUTLIMPL_UDEC_3588u 0xE04u, 0, 0, 0, 0, 11, 59, 2, 2, 3, 13, 23
#define PPUTLIMPL_UDEC_3587u 0xE03u, 0, 0, 0, 0, 11, 59, 17, 211
#define PPUTLIMPL_UDEC_3586u 0xE02u, 0, 0, 0, 0, 11, 59, 2, 11, 163
#define PPUTLIMPL_UDEC_3585u 0xE01u, 0, 0, 0, 0, 11, 59, 3, 5, 239
#define PPUTLIMPL_UDEC_3584u 0xE00u, 0, 0, 0, 0, 11, 59, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7
#define PPUTLIMPL_UDEC_3583u 0xDFFu, 0, 0, 0, 0, 11, 59,
#define PPUTLIMPL_UDEC_3582u 0xDFEu, 0, 0, 0, 0, 11, 59, 2, 3, 3, 199
#define PPUTLIMPL_UDEC_3581u 0xDFDu, 0, 0, 0, 0, 11, 59,
#define PPUTLIMPL_UDEC_3580u 0xDFCu, 0, 0, 0, 0, 11, 59, 2, 2, 5, 179
#define PPUTLIMPL_UDEC_3579u 0xDFBu, 0, 0, 0, 0, 11, 59, 3, 1193
#define PPUTLIMPL_UDEC_3578u 0xDFAu, 0, 0, 0, 0, 11, 59, 2, 1789
#define PPUTLIMPL_UDEC_3577u 0xDF9u, 0, 0, 0, 0, 11, 59, 7, 7, 73
#define PPUTLIMPL_UDEC_3576u 0xDF8u, 0, 0, 0, 0, 11, 59, 2, 2, 2, 3, 149
#define PPUTLIMPL_UDEC_3575u 0xDF7u, 0, 0, 0, 0, 11, 59, 5, 5, 11, 13
#define PPUTLIMPL_UDEC_3574u 0xDF6u, 0, 0, 0, 0, 11, 59, 2, 1787
#define PPUTLIMPL_UDEC_3573u 0xDF5u, 0, 0, 0, 0, 11, 59, 3, 3, 397
#define PPUTLIMPL_UDEC_3572u 0xDF4u, 0, 0, 0, 0, 11, 59, 2, 2, 19, 47
#define PPUTLIMPL_UDEC_3571u 0xDF3u, 0, 0, 0, 0, 11, 59,
#define PPUTLIMPL_UDEC_3570u 0xDF2u, 0, 0, 0, 0, 11, 59, 2, 3, 5, 7, 17
#define PPUTLIMPL_UDEC_3569u 0xDF1u, 0, 0, 0, 0, 11, 59, 43, 83
#define PPUTLIMPL_UDEC_3568u 0xDF0u, 0, 0, 0, 0, 11, 59, 2, 2, 2, 2, 223
#define PPUTLIMPL_UDEC_3567u 0xDEFu, 0, 0, 0, 0, 11, 59, 3, 29, 41
#define PPUTLIMPL_UDEC_3566u 0xDEEu, 0, 0, 0, 0, 11, 59, 2, 1783
#define PPUTLIMPL_UDEC_3565u 0xDEDu, 0, 0, 0, 0, 11, 59, 5, 23, 31
#define PPUTLIMPL_UDEC_3564u 0xDECu, 0, 0, 0, 0, 11, 59, 2, 2, 3, 3, 3, 3, 11
#define PPUTLIMPL_UDEC_3563u 0xDEBu, 0, 0, 0, 0, 11, 59, 7, 509
#define PPUTLIMPL_UDEC_3562u 0xDEAu, 0, 0, 0, 0, 11, 59, 2, 13, 137
#define PPUTLIMPL_UDEC_3561u 0xDE9u, 0, 0, 0, 0, 11, 59, 3, 1187
#define PPUTLIMPL_UDEC_3560u 0xDE8u, 0, 0, 0, 0, 11, 59, 2, 2, 2, 5, 89
#define PPUTLIMPL_UDEC_3559u 0xDE7u, 0, 0, 0, 0, 11, 59,
#define PPUTLIMPL_UDEC_3558u 0xDE6u, 0, 0, 0, 0, 11, 59, 2, 3, 593
#define PPUTLIMPL_UDEC_3557u 0xDE5u, 0, 0, 0, 0, 11, 59,
#define PPUTLIMPL_UDEC_3556u 0xDE4u, 0, 0, 0, 0, 11, 59, 2, 2, 7, 127
#define PPUTLIMPL_UDEC_3555u 0xDE3u, 0, 0, 0, 0, 11, 59, 3, 3, 5, 79
#define PPUTLIMPL_UDEC_3554u 0xDE2u, 0, 0, 0, 0, 11, 59, 2, 1777
#define PPUTLIMPL_UDEC_3553u 0xDE1u, 0, 0, 0, 0, 11, 59, 11, 17, 19
#define PPUTLIMPL_UDEC_3552u 0xDE0u, 0, 0, 0, 0, 11, 59, 2, 2, 2, 2, 2, 3, 37
#define PPUTLIMPL_UDEC_3551u 0xDDFu, 0, 0, 0, 0, 11, 59, 53, 67
#define PPUTLIMPL_UDEC_3550u 0xDDEu, 0, 0, 0, 0, 11, 59, 2, 5, 5, 71
#define PPUTLIMPL_UDEC_3549u 0xDDDu, 0, 0, 0, 0, 11, 59, 3, 7, 13, 13
#define PPUTLIMPL_UDEC_3548u 0xDDCu, 0, 0, 0, 0, 11, 59, 2, 2, 887
#define PPUTLIMPL_UDEC_3547u 0xDDBu, 0, 0, 0, 0, 11, 59,
#define PPUTLIMPL_UDEC_3546u 0xDDAu, 0, 0, 0, 0, 11, 59, 2, 3, 3, 197
#define PPUTLIMPL_UDEC_3545u 0xDD9u, 0, 0, 0, 0, 11, 59, 5, 709
#define PPUTLIMPL_UDEC_3544u 0xDD8u, 0, 0, 0, 0, 11, 59, 2, 2, 2, 443
#define PPUTLIMPL_UDEC_3543u 0xDD7u, 0, 0, 0, 0, 11, 59, 3, 1181
#define PPUTLIMPL_UDEC_3542u 0xDD6u, 0, 0, 0, 0, 11, 59, 2, 7, 11, 23
#define PPUTLIMPL_UDEC_3541u 0xDD5u, 0, 0, 0, 0, 11, 59,
#define PPUTLIMPL_UDEC_3540u 0xDD4u, 0, 0, 0, 0, 11, 59, 2, 2, 3, 5, 59
#define PPUTLIMPL_UDEC_3539u 0xDD3u, 0, 0, 0, 0, 11, 59,
#define PPUTLIMPL_UDEC_3538u 0xDD2u, 0, 0, 0, 0, 11, 59, 2, 29, 61
#define PPUTLIMPL_UDEC_3537u 0xDD1u, 0, 0, 0, 0, 11, 59, 3, 3, 3, 131
#define PPUTLIMPL_UDEC_3536u 0xDD0u, 0, 0, 0, 0, 11, 59, 2, 2, 2, 2, 13, 17
#define PPUTLIMPL_UDEC_3535u 0xDCFu, 0, 0, 0, 0, 11, 59, 5, 7, 101
#define PPUTLIMPL_UDEC_3534u 0xDCEu, 0, 0, 0, 0, 11, 59, 2, 3, 19, 31
#define PPUTLIMPL_UDEC_3533u 0xDCDu, 0, 0, 0, 0, 11, 59,
#define PPUTLIMPL_UDEC_3532u 0xDCCu, 0, 0, 0, 0, 11, 59, 2, 2, 883
#define PPUTLIMPL_UDEC_3531u 0xDCBu, 0, 0, 0, 0, 11, 59, 3, 11, 107
#define PPUTLIMPL_UDEC_3530u 0xDCAu, 0, 0, 0, 0, 11, 59, 2, 5, 353
#define PPUTLIMPL_UDEC_3529u 0xDC9u, 0, 0, 0, 0, 11, 59,
#define PPUTLIMPL_UDEC_3528u 0xDC8u, 0, 0, 0, 0, 11, 59, 2, 2, 2, 3, 3, 7, 7
#define PPUTLIMPL_UDEC_3527u 0xDC7u, 0, 0, 0, 0, 11, 59,
#define PPUTLIMPL_UDEC_3526u 0xDC6u, 0, 0, 0, 0, 11, 59, 2, 41, 43
#define PPUTLIMPL_UDEC_3525u 0xDC5u, 0, 0, 0, 0, 11, 59, 3, 5, 5, 47
#define PPUTLIMPL_UDEC_3524u 0xDC4u, 0, 0, 0, 0, 11, 59, 2, 2, 881
#define PPUTLIMPL_UDEC_3523u 0xDC3u, 0, 0, 0, 0, 11, 59, 13, 271
#define PPUTLIMPL_UDEC_3522u 0xDC2u, 0, 0, 0, 0, 11, 59, 2, 3, 587
#define PPUTLIMPL_UDEC_3521u 0xDC1u, 0, 0, 0, 0, 11, 59, 7, 503
#define PPUTLIMPL_UDEC_3520u 0xDC0u, 0, 0, 0, 0, 11, 59, 2, 2, 2, 2, 2, 2, 5, 11
#define PPUTLIMPL_UDEC_3519u 0xDBFu, 0, 0, 0, 0, 11, 59, 3, 3, 17, 23
#define PPUTLIMPL_UDEC_3518u 0xDBEu, 0, 0, 0, 0, 11, 59, 2, 1759
#define PPUTLIMPL_UDEC_3517u 0xDBDu, 0, 0, 0, 0, 11, 59,
#define PPUTLIMPL_UDEC_3516u 0xDBCu, 0, 0, 0, 0, 11, 59, 2, 2, 3, 293
#define PPUTLIMPL_UDEC_3515u 0xDBBu, 0, 0, 0, 0, 11, 59, 5, 19, 37
#define PPUTLIMPL_UDEC_3514u 0xDBAu, 0, 0, 0, 0, 11, 59, 2, 7, 251
#define PPUTLIMPL_UDEC_3513u 0xDB9u, 0, 0, 0, 0, 11, 59, 3, 1171
#define PPUTLIMPL_UDEC_3512u 0xDB8u, 0, 0, 0, 0, 11, 59, 2, 2, 2, 439
#define PPUTLIMPL_UDEC_3511u 0xDB7u, 0, 0, 0, 0, 11, 59,
#define PPUTLIMPL_UDEC_3510u 0xDB6u, 0, 0, 0, 0, 11, 59, 2, 3, 3, 3, 5, 13
#define PPUTLIMPL_UDEC_3509u 0xDB5u, 0, 0, 0, 0, 11, 59, 11, 11, 29
#define PPUTLIMPL_UDEC_3508u 0xDB4u, 0, 0, 0, 0, 11, 59, 2, 2, 877
#define PPUTLIMPL_UDEC_3507u 0xDB3u, 0, 0, 0, 0, 11, 59, 3, 7, 167
#define PPUTLIMPL_UDEC_3506u 0xDB2u, 0, 0, 0, 0, 11, 59, 2, 1753
#define PPUTLIMPL_UDEC_3505u 0xDB1u, 0, 0, 0, 0, 11, 59, 5, 701
#define PPUTLIMPL_UDEC_3504u 0xDB0u, 0, 0, 0, 0, 11, 59, 2, 2, 2, 2, 3, 73
#define PPUTLIMPL_UDEC_3503u 0xDAFu, 0, 0, 0, 0, 11, 59, 31, 113
#define PPUTLIMPL_UDEC_3502u 0xDAEu, 0, 0, 0, 0, 11, 59, 2, 17, 103
#define PPUTLIMPL_UDEC_3501u 0xDADu, 0, 0, 0, 0, 11, 59, 3, 3, 389
#define PPUTLIMPL_UDEC_3500u 0xDACu, 0, 0, 0, 0, 11, 59, 2, 2, 5, 5, 5, 7
#define PPUTLIMPL_UDEC_3499u 0xDABu, 0, 0, 0, 0, 11, 59,
#define PPUTLIMPL_UDEC_3498u 0xDAAu, 0, 0, 0, 0, 11, 59, 2, 3, 11, 53
#define PPUTLIMPL_UDEC_3497u 0xDA9u, 0, 0, 0, 0, 11, 59, 13, 269
#define PPUTLIMPL_UDEC_3496u 0xDA8u, 0, 0, 0, 0, 11, 59, 2, 2, 2, 19, 23
#define PPUTLIMPL_UDEC_3495u 0xDA7u, 0, 0, 0, 0, 11, 59, 3, 5, 233
#define PPUTLIMPL_UDEC_3494u 0xDA6u, 0, 0, 0, 0, 11, 59, 2, 1747
#define PPUTLIMPL_UDEC_3493u 0xDA5u, 0, 0, 0, 0, 11, 59, 7, 499
#define PPUTLIMPL_UDEC_3492u 0xDA4u, 0, 0, 0, 0, 11, 59, 2, 2, 3, 3, 97
#define PPUTLIMPL_UDEC_3491u 0xDA3u, 0, 0, 0, 0, 11, 59,
#define PPUTLIMPL_UDEC_3490u 0xDA2u, 0, 0, 0, 0, 11, 59, 2, 5, 349
#define PPUTLIMPL_UDEC_3489u 0xDA1u, 0, 0, 0, 0, 11, 59, 3, 1163
#define PPUTLIMPL_UDEC_3488u 0xDA0u, 0, 0, 0, 0, 11, 59, 2, 2, 2, 2, 2, 109
#define PPUTLIMPL_UDEC_3487u 0xD9Fu, 0, 0, 0, 0, 11, 59, 11, 317
#define PPUTLIMPL_UDEC_3486u 0xD9Eu, 0, 0, 0, 0, 11, 59, 2, 3, 7, 83
#define PPUTLIMPL_UDEC_3485u 0xD9Du, 0, 0, 0, 0, 11, 59, 5, 17, 41
#define PPUTLIMPL_UDEC_3484u 0xD9Cu, 0, 0, 0, 0, 11, 59, 2, 2, 13, 67
#define PPUTLIMPL_UDEC_3483u 0xD9Bu, 0, 0, 0, 0, 11, 59, 3, 3, 3, 3, 43
#define PPUTLIMPL_UDEC_3482u 0xD9Au, 0, 0, 0, 0, 11, 59, 2, 1741
#define PPUTLIMPL_UDEC_3481u 0xD99u, 0, 0, 0, 0, 11, 59, 59, 59
#define PPUTLIMPL_UDEC_3480u 0xD98u, 0, 0, 0, 0, 11, 58, 2, 2, 2, 3, 5, 29
#define PPUTLIMPL_UDEC_3479u 0xD97u, 0, 0, 0, 0, 11, 58, 7, 7, 71
#define PPUTLIMPL_UDEC_3478u 0xD96u, 0, 0, 0, 0, 11, 58, 2, 37, 47
#define PPUTLIMPL_UDEC_3477u 0xD95u, 0, 0, 0, 0, 11, 58, 3, 19, 61
#define PPUTLIMPL_UDEC_3476u 0xD94u, 0, 0, 0, 0, 11, 58, 2, 2, 11, 79
#define PPUTLIMPL_UDEC_3475u 0xD93u, 0, 0, 0, 0, 11, 58, 5, 5, 139
#define PPUTLIMPL_UDEC_3474u 0xD92u, 0, 0, 0, 0, 11, 58, 2, 3, 3, 193
#define PPUTLIMPL_UDEC_3473u 0xD91u, 0, 0, 0, 0, 11, 58, 23, 151
#define PPUTLIMPL_UDEC_3472u 0xD90u, 0, 0, 0, 0, 11, 58, 2, 2, 2, 2, 7, 31
#define PPUTLIMPL_UDEC_3471u 0xD8Fu, 0, 0, 0, 0, 11, 58, 3, 13, 89
#define PPUTLIMPL_UDEC_3470u 0xD8Eu, 0, 0, 0, 0, 11, 58, 2, 5, 347
#define PPUTLIMPL_UDEC_3469u 0xD8Du, 0, 0, 0, 0, 11, 58,
#define PPUTLIMPL_UDEC_3468u 0xD8Cu, 0, 0, 0, 0, 11, 58, 2, 2, 3, 17, 17
#define PPUTLIMPL_UDEC_3467u 0xD8Bu, 0, 0, 0, 0, 11, 58,
#define PPUTLIMPL_UDEC_3466u 0xD8Au, 0, 0, 0, 0, 11, 58, 2, 1733
#define PPUTLIMPL_UDEC_3465u 0xD89u, 0, 0, 0, 0, 11, 58, 3, 3, 5, 7, 11
#define PPUTLIMPL_UDEC_3464u 0xD88u, 0, 0, 0, 0, 11, 58, 2, 2, 2, 433
#define PPUTLIMPL_UDEC_3463u 0xD87u, 0, 0, 0, 0, 11, 58,
#define PPUTLIMPL_UDEC_3462u 0xD86u, 0, 0, 0, 0, 11, 58, 2, 3, 577
#define PPUTLIMPL_UDEC_3461u 0xD85u, 0, 0, 0, 0, 11, 58,
#define PPUTLIMPL_UDEC_3460u 0xD84u, 0, 0, 0, 0, 11, 58, 2, 2, 5, 173
#define PPUTLIMPL_UDEC_3459u 0xD83u, 0, 0, 0, 0, 11, 58, 3, 1153
#define PPUTLIMPL_UDEC_3458u 0xD82u, 0, 0, 0, 0, 11, 58, 2, 7, 13, 19
#define PPUTLIMPL_UDEC_3457u 0xD81u, 0, 0, 0, 0, 11, 58,
#define PPUTLIMPL_UDEC_3456u 0xD80u, 0, 0, 0, 0, 11, 58, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3
#define PPUTLIMPL_UDEC_3455u 0xD7Fu, 0, 0, 0, 0, 11, 58, 5, 691
#define PPUTLIMPL_UDEC_3454u 0xD7Eu, 0, 0, 0, 0, 11, 58, 2, 11, 157
#define PPUTLIMPL_UDEC_3453u 0xD7Du, 0, 0, 0, 0, 11, 58, 3, 1151
#define PPUTLIMPL_UDEC_3452u 0xD7Cu, 0, 0, 0, 0, 11, 58, 2, 2, 863
#define PPUTLIMPL_UDEC_3451u 0xD7Bu, 0, 0, 0, 0, 11, 58, 7, 17, 29
#define PPUTLIMPL_UDEC_3450u 0xD7Au, 0, 0, 0, 0, 11, 58, 2, 3, 5, 5, 23
#define PPUTLIMPL_UDEC_3449u 0xD79u, 0, 0, 0, 0, 11, 58,
#define PPUTLIMPL_UDEC_3448u 0xD78u, 0, 0, 0, 0, 11, 58, 2, 2, 2, 431
#define PPUTLIMPL_UDEC_3447u 0xD77u, 0, 0, 0, 0, 11, 58, 3, 3, 383
#define PPUTLIMPL_UDEC_3446u 0xD76u, 0, 0, 0, 0, 11, 58, 2, 1723
#define PPUTLIMPL_UDEC_3445u 0xD75u, 0, 0, 0, 0, 11, 58, 5, 13, 53
#define PPUTLIMPL_UDEC_3444u 0xD74u, 0, 0, 0, 0, 11, 58, 2, 2, 3, 7, 41
#define PPUTLIMPL_UDEC_3443u 0xD73u, 0, 0, 0, 0, 11, 58, 11, 313
#define PPUTLIMPL_UDEC_3442u 0xD72u, 0, 0, 0, 0, 11, 58, 2, 1721
#define PPUTLIMPL_UDEC_3441u 0xD71u, 0, 0, 0, 0, 11, 58, 3, 31, 37
#define PPUTLIMPL_UDEC_3440u 0xD70u, 0, 0, 0, 0, 11, 58, 2, 2, 2, 2, 5, 43
#define PPUTLIMPL_UDEC_3439u 0xD6Fu, 0, 0, 0, 0, 11, 58, 19, 181
#define PPUTLIMPL_UDEC_3438u 0xD6Eu, 0, 0, 0, 0, 11, 58, 2, 3, 3, 191
#define PPUTLIMPL_UDEC_3437u 0xD6Du, 0, 0, 0, 0, 11, 58, 7, 491
#define PPUTLIMPL_UDEC_3436u 0xD6Cu, 0, 0, 0, 0, 11, 58, 2, 2, 859
#define PPUTLIMPL_UDEC_3435u 0xD6Bu, 0, 0, 0, 0, 11, 58, 3, 5, 229
#define PPUTLIMPL_UDEC_3434u 0xD6Au, 0, 0, 0, 0, 11, 58, 2, 17, 101
#define PPUTLIMPL_UDEC_3433u 0xD69u, 0, 0, 0, 0, 11, 58,
#define PPUTLIMPL_UDEC_3432u 0xD68u, 0, 0, 0, 0, 11, 58, 2, 2, 2, 3, 11, 13
#define PPUTLIMPL_UDEC_3431u 0xD67u, 0, 0, 0, 0, 11, 58, 47, 73
#define PPUTLIMPL_UDEC_3430u 0xD66u, 0, 0, 0, 0, 11, 58, 2, 5, 7, 7, 7
#define PPUTLIMPL_UDEC_3429u 0xD65u, 0, 0, 0, 0, 11, 58, 3, 3, 3, 127
#define PPUTLIMPL_UDEC_3428u 0xD64u, 0, 0, 0, 0, 11, 58, 2, 2, 857
#define PPUTLIMPL_UDEC_3427u 0xD63u, 0, 0, 0, 0, 11, 58, 23, 149
#define PPUTLIMPL_UDEC_3426u 0xD62u, 0, 0, 0, 0, 11, 58, 2, 3, 571
#define PPUTLIMPL_UDEC_3425u 0xD61u, 0, 0, 0, 0, 11, 58, 5, 5, 137
#define PPUTLIMPL_UDEC_3424u 0xD60u, 0, 0, 0, 0, 11, 58, 2, 2, 2, 2, 2, 107
#define PPUTLIMPL_UDEC_3423u 0xD5Fu, 0, 0, 0, 0, 11, 58, 3, 7, 163
#define PPUTLIMPL_UDEC_3422u 0xD5Eu, 0, 0, 0, 0, 11, 58, 2, 29, 59
#define PPUTLIMPL_UDEC_3421u 0xD5Du, 0, 0, 0, 0, 11, 58, 11, 311
#define PPUTLIMPL_UDEC_3420u 0xD5Cu, 0, 0, 0, 0, 11, 58, 2, 2, 3, 3, 5, 19
#define PPUTLIMPL_UDEC_3419u 0xD5Bu, 0, 0, 0, 0, 11, 58, 13, 263
#define PPUTLIMPL_UDEC_3418u 0xD5Au, 0, 0, 0, 0, 11, 58, 2, 1709
#define PPUTLIMPL_UDEC_3417u 0xD59u, 0, 0, 0, 0, 11, 58, 3, 17, 67
#define PPUTLIMPL_UDEC_3416u 0xD58u, 0, 0, 0, 0, 11, 58, 2, 2, 2, 7, 61
#define PPUTLIMPL_UDEC_3415u 0xD57u, 0, 0, 0, 0, 11, 58, 5, 683
#define PPUTLIMPL_UDEC_3414u 0xD56u, 0, 0, 0, 0, 11, 58, 2, 3, 569
#define PPUTLIMPL_UDEC_3413u 0xD55u, 0, 0, 0, 0, 11, 58,
#define PPUTLIMPL_UDEC_3412u 0xD54u, 0, 0, 0, 0, 11, 58, 2, 2, 853
#define PPUTLIMPL_UDEC_3411u 0xD53u, 0, 0, 0, 0, 11, 58, 3, 3, 379
#define PPUTLIMPL_UDEC_3410u 0xD52u, 0, 0, 0, 0, 11, 58, 2, 5, 11, 31
#define PPUTLIMPL_UDEC_3409u 0xD51u, 0, 0, 0, 0, 11, 58, 7, 487
#define PPUTLIMPL_UDEC_3408u 0xD50u, 0, 0, 0, 0, 11, 58, 2, 2, 2, 2, 3, 71
#define PPUTLIMPL_UDEC_3407u 0xD4Fu, 0, 0, 0, 0, 11, 58,
#define PPUTLIMPL_UDEC_3406u 0xD4Eu, 0, 0, 0, 0, 11, 58, 2, 13, 131
#define PPUTLIMPL_UDEC_3405u 0xD4Du, 0, 0, 0, 0, 11, 58, 3, 5, 227
#define PPUTLIMPL_UDEC_3404u 0xD4Cu, 0, 0, 0, 0, 11, 58, 2, 2, 23, 37
#define PPUTLIMPL_UDEC_3403u 0xD4Bu, 0, 0, 0, 0, 11, 58, 41, 83
#define PPUTLIMPL_UDEC_3402u 0xD4Au, 0, 0, 0, 0, 11, 58, 2, 3, 3, 3, 3, 3, 7
#define PPUTLIMPL_UDEC_3401u 0xD49u, 0, 0, 0, 0, 11, 58, 19, 179
#define PPUTLIMPL_UDEC_3400u 0xD48u, 0, 0, 0, 0, 11, 58, 2, 2, 2, 5, 5, 17
#define PPUTLIMPL_UDEC_3399u 0xD47u, 0, 0, 0, 0, 11, 58, 3, 11, 103
#define PPUTLIMPL_UDEC_3398u 0xD46u, 0, 0, 0, 0, 11, 58, 2, 1699
#define PPUTLIMPL_UDEC_3397u 0xD45u, 0, 0, 0, 0, 11, 58, 43, 79
#define PPUTLIMPL_UDEC_3396u 0xD44u, 0, 0, 0, 0, 11, 58, 2, 2, 3, 283
#define PPUTLIMPL_UDEC_3395u 0xD43u, 0, 0, 0, 0, 11, 58, 5, 7, 97
#define PPUTLIMPL_UDEC_3394u 0xD42u, 0, 0, 0, 0, 11, 58, 2, 1697
#define PPUTLIMPL_UDEC_3393u 0xD41u, 0, 0, 0, 0, 11, 58, 3, 3, 13, 29
#define PPUTLIMPL_UDEC_3392u 0xD40u, 0, 0, 0, 0, 11, 58, 2, 2, 2, 2, 2, 2, 53
#define PPUTLIMPL_UDEC_3391u 0xD3Fu, 0, 0, 0, 0, 11, 58,
#define PPUTLIMPL_UDEC_3390u 0xD3Eu, 0, 0, 0, 0, 11, 58, 2, 3, 5, 113
#define PPUTLIMPL_UDEC_3389u 0xD3Du, 0, 0, 0, 0, 11, 58,
#define PPUTLIMPL_UDEC_3388u 0xD3Cu, 0, 0, 0, 0, 11, 58, 2, 2, 7, 11, 11
#define PPUTLIMPL_UDEC_3387u 0xD3Bu, 0, 0, 0, 0, 11, 58, 3, 1129
#define PPUTLIMPL_UDEC_3386u 0xD3Au, 0, 0, 0, 0, 11, 58, 2, 1693
#define PPUTLIMPL_UDEC_3385u 0xD39u, 0, 0, 0, 0, 11, 58, 5, 677
#define PPUTLIMPL_UDEC_3384u 0xD38u, 0, 0, 0, 0, 11, 58, 2, 2, 2, 3, 3, 47
#define PPUTLIMPL_UDEC_3383u 0xD37u, 0, 0, 0, 0, 11, 58, 17, 199
#define PPUTLIMPL_UDEC_3382u 0xD36u, 0, 0, 0, 0, 11, 58, 2, 19, 89
#define PPUTLIMPL_UDEC_3381u 0xD35u, 0, 0, 0, 0, 11, 58, 3, 7, 7, 23
#define PPUTLIMPL_UDEC_3380u 0xD34u, 0, 0, 0, 0, 11, 58, 2, 2, 5, 13, 13
#define PPUTLIMPL_UDEC_3379u 0xD33u, 0, 0, 0, 0, 11, 58, 31, 109
#define PPUTLIMPL_UDEC_3378u 0xD32u, 0, 0, 0, 0, 11, 58, 2, 3, 563
#define PPUTLIMPL_UDEC_3377u 0xD31u, 0, 0, 0, 0, 11, 58, 11, 307
#define PPUTLIMPL_UDEC_3376u 0xD30u, 0, 0, 0, 0, 11, 58, 2, 2, 2, 2, 211
#define PPUTLIMPL_UDEC_3375u 0xD2Fu, 0, 0, 0, 0, 11, 58, 3, 3, 3, 5, 5, 5
#define PPUTLIMPL_UDEC_3374u 0xD2Eu, 0, 0, 0, 0, 11, 58, 2, 7, 241
#define PPUTLIMPL_UDEC_3373u 0xD2Du, 0, 0, 0, 0, 11, 58,
#define PPUTLIMPL_UDEC_3372u 0xD2Cu, 0, 0, 0, 0, 11, 58, 2, 2, 3, 281
#define PPUTLIMPL_UDEC_3371u 0xD2Bu, 0, 0, 0, 0, 11, 58,
#define PPUTLIMPL_UDEC_3370u 0xD2Au, 0, 0, 0, 0, 11, 58, 2, 5, 337
#define PPUTLIMPL_UDEC_3369u 0xD29u, 0, 0, 0, 0, 11, 58, 3, 1123
#define PPUTLIMPL_UDEC_3368u 0xD28u, 0, 0, 0, 0, 11, 58, 2, 2, 2, 421
#define PPUTLIMPL_UDEC_3367u 0xD27u, 0, 0, 0, 0, 11, 58, 7, 13, 37
#define PPUTLIMPL_UDEC_3366u 0xD26u, 0, 0, 0, 0, 11, 58, 2, 3, 3, 11, 17
#define PPUTLIMPL_UDEC_3365u 0xD25u, 0, 0, 0, 0, 11, 58, 5, 673
#define PPUTLIMPL_UDEC_3364u 0xD24u, 0, 0, 0, 0, 11, 58, 2, 2, 29, 29
#define PPUTLIMPL_UDEC_3363u 0xD23u, 0, 0, 0, 0, 11, 57, 3, 19, 59
#define PPUTLIMPL_UDEC_3362u 0xD22u, 0, 0, 0, 0, 11, 57, 2, 41, 41
#define PPUTLIMPL_UDEC_3361u 0xD21u, 0, 0, 0, 0, 11, 57,
#define PPUTLIMPL_UDEC_3360u 0xD20u, 0, 0, 0, 0, 11, 57, 2, 2, 2, 2, 2, 3, 5, 7
#define PPUTLIMPL_UDEC_3359u 0xD1Fu, 0, 0, 0, 0, 11, 57,
#define PPUTLIMPL_UDEC_3358u 0xD1Eu, 0, 0, 0, 0, 11, 57, 2, 23, 73
#define PPUTLIMPL_UDEC_3357u 0xD1Du, 0, 0, 0, 0, 11, 57, 3, 3, 373
#define PPUTLIMPL_UDEC_3356u 0xD1Cu, 0, 0, 0, 0, 11, 57, 2, 2, 839
#define PPUTLIMPL_UDEC_3355u 0xD1Bu, 0, 0, 0, 0, 11, 57, 5, 11, 61
#define PPUTLIMPL_UDEC_3354u 0xD1Au, 0, 0, 0, 0, 11, 57, 2, 3, 13, 43
#define PPUTLIMPL_UDEC_3353u 0xD19u, 0, 0, 0, 0, 11, 57, 7, 479
#define PPUTLIMPL_UDEC_3352u 0xD18u, 0, 0, 0, 0, 11, 57, 2, 2, 2, 419
#define PPUTLIMPL_UDEC_3351u 0xD17u, 0, 0, 0, 0, 11, 57, 3, 1117
#define PPUTLIMPL_UDEC_3350u 0xD16u, 0, 0, 0, 0, 11, 57, 2, 5, 5, 67
#define PPUTLIMPL_UDEC_3349u 0xD15u, 0, 0, 0, 0, 11, 57, 17, 197
#define PPUTLIMPL_UDEC_3348u 0xD14u, 0, 0, 0, 0, 11, 57, 2, 2, 3, 3, 3, 31
#define PPUTLIMPL_UDEC_3347u 0xD13u, 0, 0, 0, 0, 11, 57,
#define PPUTLIMPL_UDEC_3346u 0xD12u, 0, 0, 0, 0, 11, 57, 2, 7, 239
#define PPUTLIMPL_UDEC_3345u 0xD11u, 0, 0, 0, 0, 11, 57, 3, 5, 223
#define PPUTLIMPL_UDEC_3344u 0xD10u, 0, 0, 0, 0, 11, 57, 2, 2, 2, 2, 11, 19
#define PPUTLIMPL_UDEC_3343u 0xD0Fu, 0, 0, 0, 0, 11, 57,
#define PPUTLIMPL_UDEC_3342u 0xD0Eu, 0, 0, 0, 0, 11, 57, 2, 3, 557
#define PPUTLIMPL_UDEC_3341u 0xD0Du, 0, 0, 0, 0, 11, 57, 13, 257
#define PPUTLIMPL_UDEC_3340u 0xD0Cu, 0, 0, 0, 0, 11, 57, 2, 2, 5, 167
#define PPUTLIMPL_UDEC_3339u 0xD0Bu, 0, 0, 0, 0, 11, 57, 3, 3, 7, 53
#define PPUTLIMPL_UDEC_3338u 0xD0Au, 0, 0, 0, 0, 11, 57, 2, 1669
#define PPUTLIMPL_UDEC_3337u 0xD09u, 0, 0, 0, 0, 11, 57, 47, 71
#define PPUTLIMPL_UDEC_3336u 0xD08u, 0, 0, 0, 0, 11, 57, 2, 2, 2, 3, 139
#define PPUTLIMPL_UDEC_3335u 0xD07u, 0, 0, 0, 0, 11, 57, 5, 23, 29
#define PPUTLIMPL_UDEC_3334u 0xD06u, 0, 0, 0, 0, 11, 57, 2, 1667
#define PPUTLIMPL_UDEC_3333u 0xD05u, 0, 0, 0, 0, 11, 57, 3, 11, 101
#define PPUTLIMPL_UDEC_3332u 0xD04u, 0, 0, 0, 0, 11, 57, 2, 2, 7, 7, 17
#define PPUTLIMPL_UDEC_3331u 0xD03u, 0, 0, 0, 0, 11, 57,
#define PPUTLIMPL_UDEC_3330u 0xD02u, 0, 0, 0, 0, 11, 57, 2, 3, 3, 5, 37
#define PPUTLIMPL_UDEC_3329u 0xD01u, 0, 0, 0, 0, 11, 57,
#define PPUTLIMPL_UDEC_3328u 0xD00u, 0, 0, 0, 0, 11, 57, 2, 2, 2, 2, 2, 2, 2, 2, 13
#define PPUTLIMPL_UDEC_3327u 0xCFFu, 0, 0, 0, 0, 11, 57, 3, 1109
#define PPUTLIMPL_UDEC_3326u 0xCFEu, 0, 0, 0, 0, 11, 57, 2, 1663
#define PPUTLIMPL_UDEC_3325u 0xCFDu, 0, 0, 0, 0, 11, 57, 5, 5, 7, 19
#define PPUTLIMPL_UDEC_3324u 0xCFCu, 0, 0, 0, 0, 11, 57, 2, 2, 3, 277
#define PPUTLIMPL_UDEC_3323u 0xCFBu, 0, 0, 0, 0, 11, 57,
#define PPUTLIMPL_UDEC_3322u 0xCFAu, 0, 0, 0, 0, 11, 57, 2, 11, 151
#define PPUTLIMPL_UDEC_3321u 0xCF9u, 0, 0, 0, 0, 11, 57, 3, 3, 3, 3, 41
#define PPUTLIMPL_UDEC_3320u 0xCF8u, 0, 0, 0, 0, 11, 57, 2, 2, 2, 5, 83
#define PPUTLIMPL_UDEC_3319u 0xCF7u, 0, 0, 0, 0, 11, 57,
#define PPUTLIMPL_UDEC_3318u 0xCF6u, 0, 0, 0, 0, 11, 57, 2, 3, 7, 79
#define PPUTLIMPL_UDEC_3317u 0xCF5u, 0, 0, 0, 0, 11, 57, 31, 107
#define PPUTLIMPL_UDEC_3316u 0xCF4u, 0, 0, 0, 0, 11, 57, 2, 2, 829
#define PPUTLIMPL_UDEC_3315u 0xCF3u, 0, 0, 0, 0, 11, 57, 3, 5, 13, 17
#define PPUTLIMPL_UDEC_3314u 0xCF2u, 0, 0, 0, 0, 11, 57, 2, 1657
#define PPUTLIMPL_UDEC_3313u 0xCF1u, 0, 0, 0, 0, 11, 57,
#define PPUTLIMPL_UDEC_3312u 0xCF0u, 0, 0, 0, 0, 11, 57, 2, 2, 2, 2, 3, 3, 23
#define PPUTLIMPL_UDEC_3311u 0xCEFu, 0, 0, 0, 0, 11, 57, 7, 11, 43
#define PPUTLIMPL_UDEC_3310u 0xCEEu, 0, 0, 0, 0, 11, 57, 2, 5, 331
#define PPUTLIMPL_UDEC_3309u 0xCEDu, 0, 0, 0, 0, 11, 57, 3, 1103
#define PPUTLIMPL_UDEC_3308u 0xCECu, 0, 0, 0, 0, 11, 57, 2, 2, 827
#define PPUTLIMPL_UDEC_3307u 0xCEBu, 0, 0, 0, 0, 11, 57,
#define PPUTLIMPL_UDEC_3306u 0xCEAu, 0, 0, 0, 0, 11, 57, 2, 3, 19, 29
#define PPUTLIMPL_UDEC_3305u 0xCE9u, 0, 0, 0, 0, 11, 57, 5, 661
#define PPUTLIMPL_UDEC_3304u 0xCE8u, 0, 0, 0, 0, 11, 57, 2, 2, 2, 7, 59
#define PPUTLIMPL_UDEC_3303u 0xCE7u, 0, 0, 0, 0, 11, 57, 3, 3, 367
#define PPUTLIMPL_UDEC_3302u 0xCE6u, 0, 0, 0, 0, 11, 57, 2, 13, 127
#define PPUTLIMPL_UDEC_3301u 0xCE5u, 0, 0, 0, 0, 11, 57,
#define PPUTLIMPL_UDEC_3300u 0xCE4u, 0, 0, 0, 0, 11, 57, 2, 2, 3, 5, 5, 11
#define PPUTLIMPL_UDEC_3299u 0xCE3u, 0, 0, 0, 0, 11, 57,
#define PPUTLIMPL_UDEC_3298u 0xCE2u, 0, 0, 0, 0, 11, 57, 2, 17, 97
#define PPUTLIMPL_UDEC_3297u 0xCE1u, 0, 0, 0, 0, 11, 57, 3, 7, 157
#define PPUTLIMPL_UDEC_3296u 0xCE0u, 0, 0, 0, 0, 11, 57, 2, 2, 2, 2, 2, 103
#define PPUTLIMPL_UDEC_3295u 0xCDFu, 0, 0, 0, 0, 11, 57, 5, 659
#define PPUTLIMPL_UDEC_3294u 0xCDEu, 0, 0, 0, 0, 11, 57, 2, 3, 3, 3, 61
#define PPUTLIMPL_UDEC_3293u 0xCDDu, 0, 0, 0, 0, 11, 57, 37, 89
#define PPUTLIMPL_UDEC_3292u 0xCDCu, 0, 0, 0, 0, 11, 57, 2, 2, 823
#define PPUTLIMPL_UDEC_3291u 0xCDBu, 0, 0, 0, 0, 11, 57, 3, 1097
#define PPUTLIMPL_UDEC_3290u 0xCDAu, 0, 0, 0, 0, 11, 57, 2, 5, 7, 47
#define PPUTLIMPL_UDEC_3289u 0xCD9u, 0, 0, 0, 0, 11, 57, 11, 13, 23
#define PPUTLIMPL_UDEC_3288u 0xCD8u, 0, 0, 0, 0, 11, 57, 2, 2, 2, 3, 137
#define PPUTLIMPL_UDEC_3287u 0xCD7u, 0, 0, 0, 0, 11, 57, 19, 173
#define PPUTLIMPL_UDEC_3286u 0xCD6u, 0, 0, 0, 0, 11, 57, 2, 31, 53
#define PPUTLIMPL_UDEC_3285u 0xCD5u, 0, 0, 0, 0, 11, 57, 3, 3, 5, 73
#define PPUTLIMPL_UDEC_3284u 0xCD4u, 0, 0, 0, 0, 11, 57, 2, 2, 821
#define PPUTLIMPL_UDEC_3283u 0xCD3u, 0, 0, 0, 0, 11, 57, 7, 7, 67
#define PPUTLIMPL_UDEC_3282u 0xCD2u, 0, 0, 0, 0, 11, 57, 2, 3, 547
#define PPUTLIMPL_UDEC_3281u 0xCD1u, 0, 0, 0, 0, 11, 57, 17, 193
#define PPUTLIMPL_UDEC_3280u 0xCD0u, 0, 0, 0, 0, 11, 57, 2, 2, 2, 2, 5, 41
#define PPUTLIMPL_UDEC_3279u 0xCCFu, 0, 0, 0, 0, 11, 57, 3, 1093
#define PPUTLIMPL_UDEC_3278u 0xCCEu, 0, 0, 0, 0, 11, 57, 2, 11, 149
#define PPUTLIMPL_UDEC_3277u 0xCCDu, 0, 0, 0, 0, 11, 57, 29, 113
#define PPUTLIMPL_UDEC_3276u 0xCCCu, 0, 0, 0, 0, 11, 57, 2, 2, 3, 3, 7, 13
#define PPUTLIMPL_UDEC_3275u 0xCCBu, 0, 0, 0, 0, 11, 57, 5, 5, 131
#define PPUTLIMPL_UDEC_3274u 0xCCAu, 0, 0, 0, 0, 11, 57, 2, 1637
#define PPUTLIMPL_UDEC_3273u 0xCC9u, 0, 0, 0, 0, 11, 57, 3, 1091
#define PPUTLIMPL_UDEC_3272u 0xCC8u, 0, 0, 0, 0, 11, 57, 2, 2, 2, 409
#define PPUTLIMPL_UDEC_3271u 0xCC7u, 0, 0, 0, 0, 11, 57,
#define PPUTLIMPL_UDEC_3270u 0xCC6u, 0, 0, 0, 0, 11, 57, 2, 3, 5, 109
#define PPUTLIMPL_UDEC_3269u 0xCC5u, 0, 0, 0, 0, 11, 57, 7, 467
#define PPUTLIMPL_UDEC_3268u 0xCC4u, 0, 0, 0, 0, 11, 57, 2, 2, 19, 43
#define PPUTLIMPL_UDEC_3267u 0xCC3u, 0, 0, 0, 0, 11, 57, 3, 3, 3, 11, 11
#define PPUTLIMPL_UDEC_3266u 0xCC2u, 0, 0, 0, 0, 11, 57, 2, 23, 71
#define PPUTLIMPL_UDEC_3265u 0xCC1u, 0, 0, 0, 0, 11, 57, 5, 653
#define PPUTLIMPL_UDEC_3264u 0xCC0u, 0, 0, 0, 0, 11, 57, 2, 2, 2, 2, 2, 2, 3, 17
#define PPUTLIMPL_UDEC_3263u 0xCBFu, 0, 0, 0, 0, 11, 57, 13, 251
#define PPUTLIMPL_UDEC_3262u 0xCBEu, 0, 0, 0, 0, 11, 57, 2, 7, 233
#define PPUTLIMPL_UDEC_3261u 0xCBDu, 0, 0, 0, 0, 11, 57, 3, 1087
#define PPUTLIMPL_UDEC_3260u 0xCBCu, 0, 0, 0, 0, 11, 57, 2, 2, 5, 163
#define PPUTLIMPL_UDEC_3259u 0xCBBu, 0, 0, 0, 0, 11, 57,
#define PPUTLIMPL_UDEC_3258u 0xCBAu, 0, 0, 0, 0, 11, 57, 2, 3, 3, 181
#define PPUTLIMPL_UDEC_3257u 0xCB9u, 0, 0, 0, 0, 11, 57,
#define PPUTLIMPL_UDEC_3256u 0xCB8u, 0, 0, 0, 0, 11, 57, 2, 2, 2, 11, 37
#define PPUTLIMPL_UDEC_3255u 0xCB7u, 0, 0, 0, 0, 11, 57, 3, 5, 7, 31
#define PPUTLIMPL_UDEC_3254u 0xCB6u, 0, 0, 0, 0, 11, 57, 2, 1627
#define PPUTLIMPL_UDEC_3253u 0xCB5u, 0, 0, 0, 0, 11, 57,
#define PPUTLIMPL_UDEC_3252u 0xCB4u, 0, 0, 0, 0, 11, 57, 2, 2, 3, 271
#define PPUTLIMPL_UDEC_3251u 0xCB3u, 0, 0, 0, 0, 11, 57,
#define PPUTLIMPL_UDEC_3250u 0xCB2u, 0, 0, 0, 0, 11, 57, 2, 5, 5, 5, 13
#define PPUTLIMPL_UDEC_3249u 0xCB1u, 0, 0, 0, 0, 11, 57, 3, 3, 19, 19
#define PPUTLIMPL_UDEC_3248u 0xCB0u, 0, 0, 0, 0, 11, 56, 2, 2, 2, 2, 7, 29
#define PPUTLIMPL_UDEC_3247u 0xCAFu, 0, 0, 0, 0, 11, 56, 17, 191
#define PPUTLIMPL_UDEC_3246u 0xCAEu, 0, 0, 0, 0, 11, 56, 2, 3, 541
#define PPUTLIMPL_UDEC_3245u 0xCADu, 0, 0, 0, 0, 11, 56, 5, 11, 59
#define PPUTLIMPL_UDEC_3244u 0xCACu, 0, 0, 0, 0, 11, 56, 2, 2, 811
#define PPUTLIMPL_UDEC_3243u 0xCABu, 0, 0, 0, 0, 11, 56, 3, 23, 47
#define PPUTLIMPL_UDEC_3242u 0xCAAu, 0, 0, 0, 0, 11, 56, 2, 1621
#define PPUTLIMPL_UDEC_3241u 0xCA9u, 0, 0, 0, 0, 11, 56, 7, 463
#define PPUTLIMPL_UDEC_3240u 0xCA8u, 0, 0, 0, 0, 11, 56, 2, 2, 2, 3, 3, 3, 3, 5
#define PPUTLIMPL_UDEC_3239u 0xCA7u, 0, 0, 0, 0, 11, 56, 41, 79
#define PPUTLIMPL_UDEC_3238u 0xCA6u, 0, 0, 0, 0, 11, 56, 2, 1619
#define PPUTLIMPL_UDEC_3237u 0xCA5u, 0, 0, 0, 0, 11, 56, 3, 13, 83
#define PPUTLIMPL_UDEC_3236u 0xCA4u, 0, 0, 0, 0, 11, 56, 2, 2, 809
#define PPUTLIMPL_UDEC_3235u 0xCA3u, 0, 0, 0, 0, 11, 56, 5, 647
#define PPUTLIMPL_UDEC_3234u 0xCA2u, 0, 0, 0, 0, 11, 56, 2, 3, 7, 7, 11
#define PPUTLIMPL_UDEC_3233u 0xCA1u, 0, 0, 0, 0, 11, 56, 53, 61
#define PPUTLIMPL_UDEC_3232u 0xCA0u, 0, 0, 0, 0, 11, 56, 2, 2, 2, 2, 2, 101
#define PPUTLIMPL_UDEC_3231u 0xC9Fu, 0, 0, 0, 0, 11, 56, 3, 3, 359
#define PPUTLIMPL_UDEC_3230u 0xC9Eu, 0, 0, 0, 0, 11, 56, 2, 5, 17, 19
#define PPUTLIMPL_UDEC_3229u 0xC9Du, 0, 0, 0, 0, 11, 56,
#define PPUTLIMPL_UDEC_3228u 0xC9Cu, 0, 0, 0, 0, 11, 56, 2, 2, 3, 269
#define PPUTLIMPL_UDEC_3227u 0xC9Bu, 0, 0, 0, 0, 11, 56, 7, 461
#define PPUTLIMPL_UDEC_3226u 0xC9Au, 0, 0, 0, 0, 11, 56, 2, 1613
#define PPUTLIMPL_UDEC_3225u 0xC99u, 0, 0, 0, 0, 11, 56, 3, 5, 5, 43
#define PPUTLIMPL_UDEC_3224u 0xC98u, 0, 0, 0, 0, 11, 56, 2, 2, 2, 13, 31
#define PPUTLIMPL_UDEC_3223u 0xC97u, 0, 0, 0, 0, 11, 56, 11, 293
#define PPUTLIMPL_UDEC_3222u 0xC96u, 0, 0, 0, 0, 11, 56, 2, 3, 3, 179
#define PPUTLIMPL_UDEC_3221u 0xC95u, 0, 0, 0, 0, 11, 56,
#define PPUTLIMPL_UDEC_3220u 0xC94u, 0, 0, 0, 0, 11, 56, 2, 2, 5, 7, 23
#define PPUTLIMPL_UDEC_3219u 0xC93u, 0, 0, 0, 0, 11, 56, 3, 29, 37
#define PPUTLIMPL_UDEC_3218u 0xC92u, 0, 0, 0, 0, 11, 56, 2, 1609
#define PPUTLIMPL_UDEC_3217u 0xC91u, 0, 0, 0, 0, 11, 56,
#define PPUTLIMPL_UDEC_3216u 0xC90u, 0, 0, 0, 0, 11, 56, 2, 2, 2, 2, 3, 67
#define PPUTLIMPL_UDEC_3215u 0xC8Fu, 0, 0, 0, 0, 11, 56, 5, 643
#define PPUTLIMPL_UDEC_3214u 0xC8Eu, 0, 0, 0, 0, 11, 56, 2, 1607
#define PPUTLIMPL_UDEC_3213u 0xC8Du, 0, 0, 0, 0, 11, 56, 3, 3, 3, 7, 17
#define PPUTLIMPL_UDEC_3212u 0xC8Cu, 0, 0, 0, 0, 11, 56, 2, 2, 11, 73
#define PPUTLIMPL_UDEC_3211u 0xC8Bu, 0, 0, 0, 0, 11, 56, 13, 13, 19
#define PPUTLIMPL_UDEC_3210u 0xC8Au, 0, 0, 0, 0, 11, 56, 2, 3, 5, 107
#define PPUTLIMPL_UDEC_3209u 0xC89u, 0, 0, 0, 0, 11, 56,
#define PPUTLIMPL_UDEC_3208u 0xC88u, 0, 0, 0, 0, 11, 56, 2, 2, 2, 401
#define PPUTLIMPL_UDEC_3207u 0xC87u, 0, 0, 0, 0, 11, 56, 3, 1069
#define PPUTLIMPL_UDEC_3206u 0xC86u, 0, 0, 0, 0, 11, 56, 2, 7, 229
#define PPUTLIMPL_UDEC_3205u 0xC85u, 0, 0, 0, 0, 11, 56, 5, 641
#define PPUTLIMPL_UDEC_3204u 0xC84u, 0, 0, 0, 0, 11, 56, 2, 2, 3, 3, 89
#define PPUTLIMPL_UDEC_3203u 0xC83u, 0, 0, 0, 0, 11, 56,
#define PPUTLIMPL_UDEC_3202u 0xC82u, 0, 0, 0, 0, 11, 56, 2, 1601
#define PPUTLIMPL_UDEC_3201u 0xC81u, 0, 0, 0, 0, 11, 56, 3, 11, 97
#define PPUTLIMPL_UDEC_3200u 0xC80u, 0, 0, 0, 0, 11, 56, 2, 2, 2, 2, 2, 2, 2, 5, 5
#define PPUTLIMPL_UDEC_3199u 0xC7Fu, 0, 0, 0, 0, 11, 56, 7, 457
#define PPUTLIMPL_UDEC_3198u 0xC7Eu, 0, 0, 0, 0, 11, 56, 2, 3, 13, 41
#define PPUTLIMPL_UDEC_3197u 0xC7Du, 0, 0, 0, 0, 11, 56, 23, 139
#define PPUTLIMPL_UDEC_3196u 0xC7Cu, 0, 0, 0, 0, 11, 56, 2, 2, 17, 47
#define PPUTLIMPL_UDEC_3195u 0xC7Bu, 0, 0, 0, 0, 11, 56, 3, 3, 5, 71
#define PPUTLIMPL_UDEC_3194u 0xC7Au, 0, 0, 0, 0, 11, 56, 2, 1597
#define PPUTLIMPL_UDEC_3193u 0xC79u, 0, 0, 0, 0, 11, 56, 31, 103
#define PPUTLIMPL_UDEC_3192u 0xC78u, 0, 0, 0, 0, 11, 56, 2, 2, 2, 3, 7, 19
#define PPUTLIMPL_UDEC_3191u 0xC77u, 0, 0, 0, 0, 11, 56,
#define PPUTLIMPL_UDEC_3190u 0xC76u, 0, 0, 0, 0, 11, 56, 2, 5, 11, 29
#define PPUTLIMPL_UDEC_3189u 0xC75u, 0, 0, 0, 0, 11, 56, 3, 1063
#define PPUTLIMPL_UDEC_3188u 0xC74u, 0, 0, 0, 0, 11, 56, 2, 2, 797
#define PPUTLIMPL_UDEC_3187u 0xC73u, 0, 0, 0, 0, 11, 56,
#define PPUTLIMPL_UDEC_3186u 0xC72u, 0, 0, 0, 0, 11, 56, 2, 3, 3, 3, 59
#define PPUTLIMPL_UDEC_3185u 0xC71u, 0, 0, 0, 0, 11, 56, 5, 7, 7, 13
#define PPUTLIMPL_UDEC_3184u 0xC70u, 0, 0, 0, 0, 11, 56, 2, 2, 2, 2, 199
#define PPUTLIMPL_UDEC_3183u 0xC6Fu, 0, 0, 0, 0, 11, 56, 3, 1061
#define PPUTLIMPL_UDEC_3182u 0xC6Eu, 0, 0, 0, 0, 11, 56, 2, 37, 43
#define PPUTLIMPL_UDEC_3181u 0xC6Du, 0, 0, 0, 0, 11, 56,
#define PPUTLIMPL_UDEC_3180u 0xC6Cu, 0, 0, 0, 0, 11, 56, 2, 2, 3, 5, 53
#define PPUTLIMPL_UDEC_3179u 0xC6Bu, 0, 0, 0, 0, 11, 56, 11, 17, 17
#define PPUTLIMPL_UDEC_3178u 0xC6Au, 0, 0, 0, 0, 11, 56, 2, 7, 227
#define PPUTLIMPL_UDEC_3177u 0xC69u, 0, 0, 0, 0, 11, 56, 3, 3, 353
#define PPUTLIMPL_UDEC_3176u 0xC68u, 0, 0, 0, 0, 11, 56, 2, 2, 2, 397
#define PPUTLIMPL_UDEC_3175u 0xC67u, 0, 0, 0, 0, 11, 56, 5, 5, 127
#define PPUTLIMPL_UDEC_3174u 0xC66u, 0, 0, 0, 0, 11, 56, 2, 3, 23, 23
#define PPUTLIMPL_UDEC_3173u 0xC65u, 0, 0, 0, 0, 11, 56, 19, 167
#define PPUTLIMPL_UDEC_3172u 0xC64u, 0, 0, 0, 0, 11, 56, 2, 2, 13, 61
#define PPUTLIMPL_UDEC_3171u 0xC63u, 0, 0, 0, 0, 11, 56, 3, 7, 151
#define PPUTLIMPL_UDEC_3170u 0xC62u, 0, 0, 0, 0, 11, 56, 2, 5, 317
#define PPUTLIMPL_UDEC_3169u 0xC61u, 0, 0, 0, 0, 11, 56,
#define PPUTLIMPL_UDEC_3168u 0xC60u, 0, 0, 0, 0, 11, 56, 2, 2, 2, 2, 2, 3, 3, 11
#define PPUTLIMPL_UDEC_3167u 0xC5Fu, 0, 0, 0, 0, 11, 56,
#define PPUTLIMPL_UDEC_3166u 0xC5Eu, 0, 0, 0, 0, 11, 56, 2, 1583
#define PPUTLIMPL_UDEC_3165u 0xC5Du, 0, 0, 0, 0, 11, 56, 3, 5, 211
#define PPUTLIMPL_UDEC_3164u 0xC5Cu, 0, 0, 0, 0, 11, 56, 2, 2, 7, 113
#define PPUTLIMPL_UDEC_3163u 0xC5Bu, 0, 0, 0, 0, 11, 56,
#define PPUTLIMPL_UDEC_3162u 0xC5Au, 0, 0, 0, 0, 11, 56, 2, 3, 17, 31
#define PPUTLIMPL_UDEC_3161u 0xC59u, 0, 0, 0, 0, 11, 56, 29, 109
#define PPUTLIMPL_UDEC_3160u 0xC58u, 0, 0, 0, 0, 11, 56, 2, 2, 2, 5, 79
#define PPUTLIMPL_UDEC_3159u 0xC57u, 0, 0, 0, 0, 11, 56, 3, 3, 3, 3, 3, 13
#define PPUTLIMPL_UDEC_3158u 0xC56u, 0, 0, 0, 0, 11, 56, 2, 1579
#define PPUTLIMPL_UDEC_3157u 0xC55u, 0, 0, 0, 0, 11, 56, 7, 11, 41
#define PPUTLIMPL_UDEC_3156u 0xC54u, 0, 0, 0, 0, 11, 56, 2, 2, 3, 263
#define PPUTLIMPL_UDEC_3155u 0xC53u, 0, 0, 0, 0, 11, 56, 5, 631
#define PPUTLIMPL_UDEC_3154u 0xC52u, 0, 0, 0, 0, 11, 56, 2, 19, 83
#define PPUTLIMPL_UDEC_3153u 0xC51u, 0, 0, 0, 0, 11, 56, 3, 1051
#define PPUTLIMPL_UDEC_3152u 0xC50u, 0, 0, 0, 0, 11, 56, 2, 2, 2, 2, 197
#define PPUTLIMPL_UDEC_3151u 0xC4Fu, 0, 0, 0, 0, 11, 56, 23, 137
#define PPUTLIMPL_UDEC_3150u 0xC4Eu, 0, 0, 0, 0, 11, 56, 2, 3, 3, 5, 5, 7
#define PPUTLIMPL_UDEC_3149u 0xC4Du, 0, 0, 0, 0, 11, 56, 47, 67
#define PPUTLIMPL_UDEC_3148u 0xC4Cu, 0, 0, 0, 0, 11, 56, 2, 2, 787
#define PPUTLIMPL_UDEC_3147u 0xC4Bu, 0, 0, 0, 0, 11, 56, 3, 1049
#define PPUTLIMPL_UDEC_3146u 0xC4Au, 0, 0, 0, 0, 11, 56, 2, 11, 11, 13
#define PPUTLIMPL_UDEC_3145u 0xC49u, 0, 0, 0, 0, 11, 56, 5, 17, 37
#define PPUTLIMPL_UDEC_3144u 0xC48u, 0, 0, 0, 0, 11, 56, 2, 2, 2, 3, 131
#define PPUTLIMPL_UDEC_3143u 0xC47u, 0, 0, 0, 0, 11, 56, 7, 449
#define PPUTLIMPL_UDEC_3142u 0xC46u, 0, 0, 0, 0, 11, 56, 2, 1571
#define PPUTLIMPL_UDEC_3141u 0xC45u, 0, 0, 0, 0, 11, 56, 3, 3, 349
#define PPUTLIMPL_UDEC_3140u 0xC44u, 0, 0, 0, 0, 11, 56, 2, 2, 5, 157
#define PPUTLIMPL_UDEC_3139u 0xC43u, 0, 0, 0, 0, 11, 56, 43, 73
#define PPUTLIMPL_UDEC_3138u 0xC42u, 0, 0, 0, 0, 11, 56, 2, 3, 523
#define PPUTLIMPL_UDEC_3137u 0xC41u, 0, 0, 0, 0, 11, 56,
#define PPUTLIMPL_UDEC_3136u 0xC40u, 0, 0, 0, 0, 11, 56, 2, 2, 2, 2, 2, 2, 7, 7
#define PPUTLIMPL_UDEC_3135u 0xC3Fu, 0, 0, 0, 0, 11, 55, 3, 5, 11, 19
#define PPUTLIMPL_UDEC_3134u 0xC3Eu, 0, 0, 0, 0, 11, 55, 2, 1567
#define PPUTLIMPL_UDEC_3133u 0xC3Du, 0, 0, 0, 0, 11, 55, 13, 241
#define PPUTLIMPL_UDEC_3132u 0xC3Cu, 0, 0, 0, 0, 11, 55, 2, 2, 3, 3, 3, 29
#define PPUTLIMPL_UDEC_3131u 0xC3Bu, 0, 0, 0, 0, 11, 55, 31, 101
#define PPUTLIMPL_UDEC_3130u 0xC3Au, 0, 0, 0, 0, 11, 55, 2, 5, 313
#define PPUTLIMPL_UDEC_3129u 0xC39u, 0, 0, 0, 0, 11, 55, 3, 7, 149
#define PPUTLIMPL_UDEC_3128u 0xC38u, 0, 0, 0, 0, 11, 55, 2, 2, 2, 17, 23
#define PPUTLIMPL_UDEC_3127u 0xC37u, 0, 0, 0, 0, 11, 55, 53, 59
#define PPUTLIMPL_UDEC_3126u 0xC36u, 0, 0, 0, 0, 11, 55, 2, 3, 521
#define PPUTLIMPL_UDEC_3125u 0xC35u, 0, 0, 0, 0, 11, 55, 5, 5, 5, 5, 5
#define PPUTLIMPL_UDEC_3124u 0xC34u, 0, 0, 0, 0, 11, 55, 2, 2, 11, 71
#define PPUTLIMPL_UDEC_3123u 0xC33u, 0, 0, 0, 0, 11, 55, 3, 3, 347
#define PPUTLIMPL_UDEC_3122u 0xC32u, 0, 0, 0, 0, 11, 55, 2, 7, 223
#define PPUTLIMPL_UDEC_3121u 0xC31u, 0, 0, 0, 0, 11, 55,
#define PPUTLIMPL_UDEC_3120u 0xC30u, 0, 0, 0, 0, 11, 55, 2, 2, 2, 2, 3, 5, 13
#define PPUTLIMPL_UDEC_3119u 0xC2Fu, 0, 0, 0, 0, 11, 55,
#define PPUTLIMPL_UDEC_3118u 0xC2Eu, 0, 0, 0, 0, 11, 55, 2, 1559
#define PPUTLIMPL_UDEC_3117u 0xC2Du, 0, 0, 0, 0, 11, 55, 3, 1039
#define PPUTLIMPL_UDEC_3116u 0xC2Cu, 0, 0, 0, 0, 11, 55, 2, 2, 19, 41
#define PPUTLIMPL_UDEC_3115u 0xC2Bu, 0, 0, 0, 0, 11, 55, 5, 7, 89
#define PPUTLIMPL_UDEC_3114u 0xC2Au, 0, 0, 0, 0, 11, 55, 2, 3, 3, 173
#define PPUTLIMPL_UDEC_3113u 0xC29u, 0, 0, 0, 0, 11, 55, 11, 283
#define PPUTLIMPL_UDEC_3112u 0xC28u, 0, 0, 0, 0, 11, 55, 2, 2, 2, 389
#define PPUTLIMPL_UDEC_3111u 0xC27u, 0, 0, 0, 0, 11, 55, 3, 17, 61
#define PPUTLIMPL_UDEC_3110u 0xC26u, 0, 0, 0, 0, 11, 55, 2, 5, 311
#define PPUTLIMPL_UDEC_3109u 0xC25u, 0, 0, 0, 0, 11, 55,
#define PPUTLIMPL_UDEC_3108u 0xC24u, 0, 0, 0, 0, 11, 55, 2, 2, 3, 7, 37
#define PPUTLIMPL_UDEC_3107u 0xC23u, 0, 0, 0, 0, 11, 55, 13, 239
#define PPUTLIMPL_UDEC_3106u 0xC22u, 0, 0, 0, 0, 11, 55, 2, 1553
#define PPUTLIMPL_UDEC_3105u 0xC21u, 0, 0, 0, 0, 11, 55, 3, 3, 3, 5, 23
#define PPUTLIMPL_UDEC_3104u 0xC20u, 0, 0, 0, 0, 11, 55, 2, 2, 2, 2, 2, 97
#define PPUTLIMPL_UDEC_3103u 0xC1Fu, 0, 0, 0, 0, 11, 55, 29, 107
#define PPUTLIMPL_UDEC_3102u 0xC1Eu, 0, 0, 0, 0, 11, 55, 2, 3, 11, 47
#define PPUTLIMPL_UDEC_3101u 0xC1Du, 0, 0, 0, 0, 11, 55, 7, 443
#define PPUTLIMPL_UDEC_3100u 0xC1Cu, 0, 0, 0, 0, 11, 55, 2, 2, 5, 5, 31
#define PPUTLIMPL_UDEC_3099u 0xC1Bu, 0, 0, 0, 0, 11, 55, 3, 1033
#define PPUTLIMPL_UDEC_3098u 0xC1Au, 0, 0, 0, 0, 11, 55, 2, 1549
#define PPUTLIMPL_UDEC_3097u 0xC19u, 0, 0, 0, 0, 11, 55, 19, 163
#define PPUTLIMPL_UDEC_3096u 0xC18u, 0, 0, 0, 0, 11, 55, 2, 2, 2, 3, 3, 43
#define PPUTLIMPL_UDEC_3095u 0xC17u, 0, 0, 0, 0, 11, 55, 5, 619
#define PPUTLIMPL_UDEC_3094u 0xC16u, 0, 0, 0, 0, 11, 55, 2, 7, 13, 17
#define PPUTLIMPL_UDEC_3093u 0xC15u, 0, 0, 0, 0, 11, 55, 3, 1031
#define PPUTLIMPL_UDEC_3092u 0xC14u, 0, 0, 0, 0, 11, 55, 2, 2, 773
#define PPUTLIMPL_UDEC_3091u 0xC13u, 0, 0, 0, 0, 11, 55, 11, 281
#define PPUTLIMPL_UDEC_3090u 0xC12u, 0, 0, 0, 0, 11, 55, 2, 3, 5, 103
#define PPUTLIMPL_UDEC_3089u 0xC11u, 0, 0, 0, 0, 11, 55,
#define PPUTLIMPL_UDEC_3088u 0xC10u, 0, 0, 0, 0, 11, 55, 2, 2, 2, 2, 193
#define PPUTLIMPL_UDEC_3087u 0xC0Fu, 0, 0, 0, 0, 11, 55, 3, 3, 7, 7, 7
#define PPUTLIMPL_UDEC_3086u 0xC0Eu, 0, 0, 0, 0, 11, 55, 2, 1543
#define PPUTLIMPL_UDEC_3085u 0xC0Du, 0, 0, 0, 0, 11, 55, 5, 617
#define PPUTLIMPL_UDEC_3084u 0xC0Cu, 0, 0, 0, 0, 11, 55, 2, 2, 3, 257
#define PPUTLIMPL_UDEC_3083u 0xC0Bu, 0, 0, 0, 0, 11, 55,
#define PPUTLIMPL_UDEC_3082u 0xC0Au, 0, 0, 0, 0, 11, 55, 2, 23, 67
#define PPUTLIMPL_UDEC_3081u 0xC09u, 0, 0, 0, 0, 11, 55, 3, 13, 79
#define PPUTLIMPL_UDEC_3080u 0xC08u, 0, 0, 0, 0, 11, 55, 2, 2, 2, 5, 7, 11
#define PPUTLIMPL_UDEC_3079u 0xC07u, 0, 0, 0, 0, 11, 55,
#define PPUTLIMPL_UDEC_3078u 0xC06u, 0, 0, 0, 0, 11, 55, 2, 3, 3, 3, 3, 19
#define PPUTLIMPL_UDEC_3077u 0xC05u, 0, 0, 0, 0, 11, 55, 17, 181
#define PPUTLIMPL_UDEC_3076u 0xC04u, 0, 0, 0, 0, 11, 55, 2, 2, 769
#define PPUTLIMPL_UDEC_3075u 0xC03u, 0, 0, 0, 0, 11, 55, 3, 5, 5, 41
#define PPUTLIMPL_UDEC_3074u 0xC02u, 0, 0, 0, 0, 11, 55, 2, 29, 53
#define PPUTLIMPL_UDEC_3073u 0xC01u, 0, 0, 0, 0, 11, 55, 7, 439
#define PPUTLIMPL_UDEC_3072u 0xC00u, 0, 0, 0, 0, 11, 55, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3
#define PPUTLIMPL_UDEC_3071u 0xBFFu, 0, 0, 0, 0, 11, 55, 37, 83
#define PPUTLIMPL_UDEC_3070u 0xBFEu, 0, 0, 0, 0, 11, 55, 2, 5, 307
#define PPUTLIMPL_UDEC_3069u 0xBFDu, 0, 0, 0, 0, 11, 55, 3, 3, 11, 31
#define PPUTLIMPL_UDEC_3068u 0xBFCu, 0, 0, 0, 0, 11, 55, 2, 2, 13, 59
#define PPUTLIMPL_UDEC_3067u 0xBFBu, 0, 0, 0, 0, 11, 55,
#define PPUTLIMPL_UDEC_3066u 0xBFAu, 0, 0, 0, 0, 11, 55, 2, 3, 7, 73
#define PPUTLIMPL_UDEC_3065u 0xBF9u, 0, 0, 0, 0, 11, 55, 5, 613
#define PPUTLIMPL_UDEC_3064u 0xBF8u, 0, 0, 0, 0, 11, 55, 2, 2, 2, 383
#define PPUTLIMPL_UDEC_3063u 0xBF7u, 0, 0, 0, 0, 11, 55, 3, 1021
#define PPUTLIMPL_UDEC_3062u 0xBF6u, 0, 0, 0, 0, 11, 55, 2, 1531
#define PPUTLIMPL_UDEC_3061u 0xBF5u, 0, 0, 0, 0, 11, 55,
#define PPUTLIMPL_UDEC_3060u 0xBF4u, 0, 0, 0, 0, 11, 55, 2, 2, 3, 3, 5, 17
#define PPUTLIMPL_UDEC_3059u 0xBF3u, 0, 0, 0, 0, 11, 55, 7, 19, 23
#define PPUTLIMPL_UDEC_3058u 0xBF2u, 0, 0, 0, 0, 11, 55, 2, 11, 139
#define PPUTLIMPL_UDEC_3057u 0xBF1u, 0, 0, 0, 0, 11, 55, 3, 1019
#define PPUTLIMPL_UDEC_3056u 0xBF0u, 0, 0, 0, 0, 11, 55, 2, 2, 2, 2, 191
#define PPUTLIMPL_UDEC_3055u 0xBEFu, 0, 0, 0, 0, 11, 55, 5, 13, 47
#define PPUTLIMPL_UDEC_3054u 0xBEEu, 0, 0, 0, 0, 11, 55, 2, 3, 509
#define PPUTLIMPL_UDEC_3053u 0xBEDu, 0, 0, 0, 0, 11, 55, 43, 71
#define PPUTLIMPL_UDEC_3052u 0xBECu, 0, 0, 0, 0, 11, 55, 2, 2, 7, 109
#define PPUTLIMPL_UDEC_3051u 0xBEBu, 0, 0, 0, 0, 11, 55, 3, 3, 3, 113
#define PPUTLIMPL_UDEC_3050u 0xBEAu, 0, 0, 0, 0, 11, 55, 2, 5, 5, 61
#define PPUTLIMPL_UDEC_3049u 0xBE9u, 0, 0, 0, 0, 11, 55,
#define PPUTLIMPL_UDEC_3048u 0xBE8u, 0, 0, 0, 0, 11, 55, 2, 2, 2, 3, 127
#define PPUTLIMPL_UDEC_3047u 0xBE7u, 0, 0, 0, 0, 11, 55, 11, 277
#define PPUTLIMPL_UDEC_3046u 0xBE6u, 0, 0, 0, 0, 11, 55, 2, 1523
#define PPUTLIMPL_UDEC_3045u 0xBE5u, 0, 0, 0, 0, 11, 55, 3, 5, 7, 29
#define PPUTLIMPL_UDEC_3044u 0xBE4u, 0, 0, 0, 0, 11, 55, 2, 2, 761
#define PPUTLIMPL_UDEC_3043u 0xBE3u, 0, 0, 0, 0, 11, 55, 17, 179
#define PPUTLIMPL_UDEC_3042u 0xBE2u, 0, 0, 0, 0, 11, 55, 2, 3, 3, 13, 13
#define PPUTLIMPL_UDEC_3041u 0xBE1u, 0, 0, 0, 0, 11, 55,
#define PPUTLIMPL_UDEC_3040u 0xBE0u, 0, 0, 0, 0, 11, 55, 2, 2, 2, 2, 2, 5, 19
#define PPUTLIMPL_UDEC_3039u 0xBDFu, 0, 0, 0, 0, 11, 55, 3, 1013
#define PPUTLIMPL_UDEC_3038u 0xBDEu, 0, 0, 0, 0, 11, 55, 2, 7, 7, 31
#define PPUTLIMPL_UDEC_3037u 0xBDDu, 0, 0, 0, 0, 11, 55,
#define PPUTLIMPL_UDEC_3036u 0xBDCu, 0, 0, 0, 0, 11, 55, 2, 2, 3, 11, 23
#define PPUTLIMPL_UDEC_3035u 0xBDBu, 0, 0, 0, 0, 11, 55, 5, 607
#define PPUTLIMPL_UDEC_3034u 0xBDAu, 0, 0, 0, 0, 11, 55, 2, 37, 41
#define PPUTLIMPL_UDEC_3033u 0xBD9u, 0, 0, 0, 0, 11, 55, 3, 3, 337
#define PPUTLIMPL_UDEC_3032u 0xBD8u, 0, 0, 0, 0, 11, 55, 2, 2, 2, 379
#define PPUTLIMPL_UDEC_3031u 0xBD7u, 0, 0, 0, 0, 11, 55, 7, 433
#define PPUTLIMPL_UDEC_3030u 0xBD6u, 0, 0, 0, 0, 11, 55, 2, 3, 5, 101
#define PPUTLIMPL_UDEC_3029u 0xBD5u, 0, 0, 0, 0, 11, 55, 13, 233
#define PPUTLIMPL_UDEC_3028u 0xBD4u, 0, 0, 0, 0, 11, 55, 2, 2, 757
#define PPUTLIMPL_UDEC_3027u 0xBD3u, 0, 0, 0, 0, 11, 55, 3, 1009
#define PPUTLIMPL_UDEC_3026u 0xBD2u, 0, 0, 0, 0, 11, 55, 2, 17, 89
#define PPUTLIMPL_UDEC_3025u 0xBD1u, 0, 0, 0, 0, 11, 55, 5, 5, 11, 11
#define PPUTLIMPL_UDEC_3024u 0xBD0u, 0, 0, 0, 0, 11, 54, 2, 2, 2, 2, 3, 3, 3, 7
#define PPUTLIMPL_UDEC_3023u 0xBCFu, 0, 0, 0, 0, 11, 54,
#define PPUTLIMPL_UDEC_3022u 0xBCEu, 0, 0, 0, 0, 11, 54, 2, 1511
#define PPUTLIMPL_UDEC_3021u 0xBCDu, 0, 0, 0, 0, 11, 54, 3, 19, 53
#define PPUTLIMPL_UDEC_3020u 0xBCCu, 0, 0, 0, 0, 11, 54, 2, 2, 5, 151
#define PPUTLIMPL_UDEC_3019u 0xBCBu, 0, 0, 0, 0, 11, 54,
#define PPUTLIMPL_UDEC_3018u 0xBCAu, 0, 0, 0, 0, 11, 54, 2, 3, 503
#define PPUTLIMPL_UDEC_3017u 0xBC9u, 0, 0, 0, 0, 11, 54, 7, 431
#define PPUTLIMPL_UDEC_3016u 0xBC8u, 0, 0, 0, 0, 11, 54, 2, 2, 2, 13, 29
#define PPUTLIMPL_UDEC_3015u 0xBC7u, 0, 0, 0, 0, 11, 54, 3, 3, 5, 67
#define PPUTLIMPL_UDEC_3014u 0xBC6u, 0, 0, 0, 0, 11, 54, 2, 11, 137
#define PPUTLIMPL_UDEC_3013u 0xBC5u, 0, 0, 0, 0, 11, 54, 23, 131
#define PPUTLIMPL_UDEC_3012u 0xBC4u, 0, 0, 0, 0, 11, 54, 2, 2, 3, 251
#define PPUTLIMPL_UDEC_3011u 0xBC3u, 0, 0, 0, 0, 11, 54,
#define PPUTLIMPL_UDEC_3010u 0xBC2u, 0, 0, 0, 0, 11, 54, 2, 5, 7, 43
#define PPUTLIMPL_UDEC_3009u 0xBC1u, 0, 0, 0, 0, 11, 54, 3, 17, 59
#define PPUTLIMPL_UDEC_3008u 0xBC0u, 0, 0, 0, 0, 11, 54, 2, 2, 2, 2, 2, 2, 47
#define PPUTLIMPL_UDEC_3007u 0xBBFu, 0, 0, 0, 0, 11, 54, 31, 97
#define PPUTLIMPL_UDEC_3006u 0xBBEu, 0, 0, 0, 0, 11, 54, 2, 3, 3, 167
#define PPUTLIMPL_UDEC_3005u 0xBBDu, 0, 0, 0, 0, 11, 54, 5, 601
#define PPUTLIMPL_UDEC_3004u 0xBBCu, 0, 0, 0, 0, 11, 54, 2, 2, 751
#define PPUTLIMPL_UDEC_3003u 0xBBBu, 0, 0, 0, 0, 11, 54, 3, 7, 11, 13
#define PPUTLIMPL_UDEC_3002u 0xBBAu, 0, 0, 0, 0, 11, 54, 2, 19, 79
#define PPUTLIMPL_UDEC_3001u 0xBB9u, 0, 0, 0, 0, 11, 54,
#define PPUTLIMPL_UDEC_3000u 0xBB8u, 0, 0, 0, 0, 11, 54, 2, 2, 2, 3, 5, 5, 5
#define PPUTLIMPL_UDEC_2999u 0xBB7u, 0, 0, 0, 0, 11, 54,
#define PPUTLIMPL_UDEC_2998u 0xBB6u, 0, 0, 0, 0, 11, 54, 2, 1499
#define PPUTLIMPL_UDEC_2997u 0xBB5u, 0, 0, 0, 0, 11, 54, 3, 3, 3, 3, 37
#define PPUTLIMPL_UDEC_2996u 0xBB4u, 0, 0, 0, 0, 11, 54, 2, 2, 7, 107
#define PPUTLIMPL_UDEC_2995u 0xBB3u, 0, 0, 0, 0, 11, 54, 5, 599
#define PPUTLIMPL_UDEC_2994u 0xBB2u, 0, 0, 0, 0, 11, 54, 2, 3, 499
#define PPUTLIMPL_UDEC_2993u 0xBB1u, 0, 0, 0, 0, 11, 54, 41, 73
#define PPUTLIMPL_UDEC_2992u 0xBB0u, 0, 0, 0, 0, 11, 54, 2, 2, 2, 2, 11, 17
#define PPUTLIMPL_UDEC_2991u 0xBAFu, 0, 0, 0, 0, 11, 54, 3, 997
#define PPUTLIMPL_UDEC_2990u 0xBAEu, 0, 0, 0, 0, 11, 54, 2, 5, 13, 23
#define PPUTLIMPL_UDEC_2989u 0xBADu, 0, 0, 0, 0, 11, 54, 7, 7, 61
#define PPUTLIMPL_UDEC_2988u 0xBACu, 0, 0, 0, 0, 11, 54, 2, 2, 3, 3, 83
#define PPUTLIMPL_UDEC_2987u 0xBABu, 0, 0, 0, 0, 11, 54, 29, 103
#define PPUTLIMPL_UDEC_2986u 0xBAAu, 0, 0, 0, 0, 11, 54, 2, 1493
#define PPUTLIMPL_UDEC_2985u 0xBA9u, 0, 0, 0, 0, 11, 54, 3, 5, 199
#define PPUTLIMPL_UDEC_2984u 0xBA8u, 0, 0, 0, 0, 11, 54, 2, 2, 2, 373
#define PPUTLIMPL_UDEC_2983u 0xBA7u, 0, 0, 0, 0, 11, 54, 19, 157
#define PPUTLIMPL_UDEC_2982u 0xBA6u, 0, 0, 0, 0, 11, 54, 2, 3, 7, 71
#define PPUTLIMPL_UDEC_2981u 0xBA5u, 0, 0, 0, 0, 11, 54, 11, 271
#define PPUTLIMPL_UDEC_2980u 0xBA4u, 0, 0, 0, 0, 11, 54, 2, 2, 5, 149
#define PPUTLIMPL_UDEC_2979u 0xBA3u, 0, 0, 0, 0, 11, 54, 3, 3, 331
#define PPUTLIMPL_UDEC_2978u 0xBA2u, 0, 0, 0, 0, 11, 54, 2, 1489
#define PPUTLIMPL_UDEC_2977u 0xBA1u, 0, 0, 0, 0, 11, 54, 13, 229
#define PPUTLIMPL_UDEC_2976u 0xBA0u, 0, 0, 0, 0, 11, 54, 2, 2, 2, 2, 2, 3, 31
#define PPUTLIMPL_UDEC_2975u 0xB9Fu, 0, 0, 0, 0, 11, 54, 5, 5, 7, 17
#define PPUTLIMPL_UDEC_2974u 0xB9Eu, 0, 0, 0, 0, 11, 54, 2, 1487
#define PPUTLIMPL_UDEC_2973u 0xB9Du, 0, 0, 0, 0, 11, 54, 3, 991
#define PPUTLIMPL_UDEC_2972u 0xB9Cu, 0, 0, 0, 0, 11, 54, 2, 2, 743
#define PPUTLIMPL_UDEC_2971u 0xB9Bu, 0, 0, 0, 0, 11, 54,
#define PPUTLIMPL_UDEC_2970u 0xB9Au, 0, 0, 0, 0, 11, 54, 2, 3, 3, 3, 5, 11
#define PPUTLIMPL_UDEC_2969u 0xB99u, 0, 0, 0, 0, 11, 54,
#define PPUTLIMPL_UDEC_2968u 0xB98u, 0, 0, 0, 0, 11, 54, 2, 2, 2, 7, 53
#define PPUTLIMPL_UDEC_2967u 0xB97u, 0, 0, 0, 0, 11, 54, 3, 23, 43
#define PPUTLIMPL_UDEC_2966u 0xB96u, 0, 0, 0, 0, 11, 54, 2, 1483
#define PPUTLIMPL_UDEC_2965u 0xB95u, 0, 0, 0, 0, 11, 54, 5, 593
#define PPUTLIMPL_UDEC_2964u 0xB94u, 0, 0, 0, 0, 11, 54, 2, 2, 3, 13, 19
#define PPUTLIMPL_UDEC_2963u 0xB93u, 0, 0, 0, 0, 11, 54,
#define PPUTLIMPL_UDEC_2962u 0xB92u, 0, 0, 0, 0, 11, 54, 2, 1481
#define PPUTLIMPL_UDEC_2961u 0xB91u, 0, 0, 0, 0, 11, 54, 3, 3, 7, 47
#define PPUTLIMPL_UDEC_2960u 0xB90u, 0, 0, 0, 0, 11, 54, 2, 2, 2, 2, 5, 37
#define PPUTLIMPL_UDEC_2959u 0xB8Fu, 0, 0, 0, 0, 11, 54, 11, 269
#define PPUTLIMPL_UDEC_2958u 0xB8Eu, 0, 0, 0, 0, 11, 54, 2, 3, 17, 29
#define PPUTLIMPL_UDEC_2957u 0xB8Du, 0, 0, 0, 0, 11, 54,
#define PPUTLIMPL_UDEC_2956u 0xB8Cu, 0, 0, 0, 0, 11, 54, 2, 2, 739
#define PPUTLIMPL_UDEC_2955u 0xB8Bu, 0, 0, 0, 0, 11, 54, 3, 5, 197
#define PPUTLIMPL_UDEC_2954u 0xB8Au, 0, 0, 0, 0, 11, 54, 2, 7, 211
#define PPUTLIMPL_UDEC_2953u 0xB89u, 0, 0, 0, 0, 11, 54,
#define PPUTLIMPL_UDEC_2952u 0xB88u, 0, 0, 0, 0, 11, 54, 2, 2, 2, 3, 3, 41
#define PPUTLIMPL_UDEC_2951u 0xB87u, 0, 0, 0, 0, 11, 54, 13, 227
#define PPUTLIMPL_UDEC_2950u 0xB86u, 0, 0, 0, 0, 11, 54, 2, 5, 5, 59
#define PPUTLIMPL_UDEC_2949u 0xB85u, 0, 0, 0, 0, 11, 54, 3, 983
#define PPUTLIMPL_UDEC_2948u 0xB84u, 0, 0, 0, 0, 11, 54, 2, 2, 11, 67
#define PPUTLIMPL_UDEC_2947u 0xB83u, 0, 0, 0, 0, 11, 54, 7, 421
#define PPUTLIMPL_UDEC_2946u 0xB82u, 0, 0, 0, 0, 11, 54, 2, 3, 491
#define PPUTLIMPL_UDEC_2945u 0xB81u, 0, 0, 0, 0, 11, 54, 5, 19, 31
#define PPUTLIMPL_UDEC_2944u 0xB80u, 0, 0, 0, 0, 11, 54, 2, 2, 2, 2, 2, 2, 2, 23
#define PPUTLIMPL_UDEC_2943u 0xB7Fu, 0, 0, 0, 0, 11, 54, 3, 3, 3, 109
#define PPUTLIMPL_UDEC_2942u 0xB7Eu, 0, 0, 0, 0, 11, 54, 2, 1471
#define PPUTLIMPL_UDEC_2941u 0xB7Du, 0, 0, 0, 0, 11, 54, 17, 173
#define PPUTLIMPL_UDEC_2940u 0xB7Cu, 0, 0, 0, 0, 11, 54, 2, 2, 3, 5, 7, 7
#define PPUTLIMPL_UDEC_2939u 0xB7Bu, 0, 0, 0, 0, 11, 54,
#define PPUTLIMPL_UDEC_2938u 0xB7Au, 0, 0, 0, 0, 11, 54, 2, 13, 113
#define PPUTLIMPL_UDEC_2937u 0xB79u, 0, 0, 0, 0, 11, 54, 3, 11, 89
#define PPUTLIMPL_UDEC_2936u 0xB78u, 0, 0, 0, 0, 11, 54, 2, 2, 2, 367
#define PPUTLIMPL_UDEC_2935u 0xB77u, 0, 0, 0, 0, 11, 54, 5, 587
#define PPUTLIMPL_UDEC_2934u 0xB76u, 0, 0, 0, 0, 11, 54, 2, 3, 3, 163
#define PPUTLIMPL_UDEC_2933u 0xB75u, 0, 0, 0, 0, 11, 54, 7, 419
#define PPUTLIMPL_UDEC_2932u 0xB74u, 0, 0, 0, 0, 11, 54, 2, 2, 733
#define PPUTLIMPL_UDEC_2931u 0xB73u, 0, 0, 0, 0, 11, 54, 3, 977
#define PPUTLIMPL_UDEC_2930u 0xB72u, 0, 0, 0, 0, 11, 54, 2, 5, 293
#define PPUTLIMPL_UDEC_2929u 0xB71u, 0, 0, 0, 0, 11, 54, 29, 101
#define PPUTLIMPL_UDEC_2928u 0xB70u, 0, 0, 0, 0, 11, 54, 2, 2, 2, 2, 3, 61
#define PPUTLIMPL_UDEC_2927u 0xB6Fu, 0, 0, 0, 0, 11, 54,
#define PPUTLIMPL_UDEC_2926u 0xB6Eu, 0, 0, 0, 0, 11, 54, 2, 7, 11, 19
#define PPUTLIMPL_UDEC_2925u 0xB6Du, 0, 0, 0, 0, 11, 54, 3, 3, 5, 5, 13
#define PPUTLIMPL_UDEC_2924u 0xB6Cu, 0, 0, 0, 0, 11, 54, 2, 2, 17, 43
#define PPUTLIMPL_UDEC_2923u 0xB6Bu, 0, 0, 0, 0, 11, 54, 37, 79
#define PPUTLIMPL_UDEC_2922u 0xB6Au, 0, 0, 0, 0, 11, 54, 2, 3, 487
#define PPUTLIMPL_UDEC_2921u 0xB69u, 0, 0, 0, 0, 11, 54, 23, 127
#define PPUTLIMPL_UDEC_2920u 0xB68u, 0, 0, 0, 0, 11, 54, 2, 2, 2, 5, 73
#define PPUTLIMPL_UDEC_2919u 0xB67u, 0, 0, 0, 0, 11, 54, 3, 7, 139
#define PPUTLIMPL_UDEC_2918u 0xB66u, 0, 0, 0, 0, 11, 54, 2, 1459
#define PPUTLIMPL_UDEC_2917u 0xB65u, 0, 0, 0, 0, 11, 54,
#define PPUTLIMPL_UDEC_2916u 0xB64u, 0, 0, 0, 0, 11, 54, 2, 2, 3, 3, 3, 3, 3, 3
#define PPUTLIMPL_UDEC_2915u 0xB63u, 0, 0, 0, 0, 11, 53, 5, 11, 53
#define PPUTLIMPL_UDEC_2914u 0xB62u, 0, 0, 0, 0, 11, 53, 2, 31, 47
#define PPUTLIMPL_UDEC_2913u 0xB61u, 0, 0, 0, 0, 11, 53, 3, 971
#define PPUTLIMPL_UDEC_2912u 0xB60u, 0, 0, 0, 0, 11, 53, 2, 2, 2, 2, 2, 7, 13
#define PPUTLIMPL_UDEC_2911u 0xB5Fu, 0, 0, 0, 0, 11, 53, 41, 71
#define PPUTLIMPL_UDEC_2910u 0xB5Eu, 0, 0, 0, 0, 11, 53, 2, 3, 5, 97
#define PPUTLIMPL_UDEC_2909u 0xB5Du, 0, 0, 0, 0, 11, 53,
#define PPUTLIMPL_UDEC_2908u 0xB5Cu, 0, 0, 0, 0, 11, 53, 2, 2, 727
#define PPUTLIMPL_UDEC_2907u 0xB5Bu, 0, 0, 0, 0, 11, 53, 3, 3, 17, 19
#define PPUTLIMPL_UDEC_2906u 0xB5Au, 0, 0, 0, 0, 11, 53, 2, 1453
#define PPUTLIMPL_UDEC_2905u 0xB59u, 0, 0, 0, 0, 11, 53, 5, 7, 83
#define PPUTLIMPL_UDEC_2904u 0xB58u, 0, 0, 0, 0, 11, 53, 2, 2, 2, 3, 11, 11
#define PPUTLIMPL_UDEC_2903u 0xB57u, 0, 0, 0, 0, 11, 53,
#define PPUTLIMPL_UDEC_2902u 0xB56u, 0, 0, 0, 0, 11, 53, 2, 1451
#define PPUTLIMPL_UDEC_2901u 0xB55u, 0, 0, 0, 0, 11, 53, 3, 967
#define PPUTLIMPL_UDEC_2900u 0xB54u, 0, 0, 0, 0, 11, 53, 2, 2, 5, 5, 29
#define PPUTLIMPL_UDEC_2899u 0xB53u, 0, 0, 0, 0, 11, 53, 13, 223
#define PPUTLIMPL_UDEC_2898u 0xB52u, 0, 0, 0, 0, 11, 53, 2, 3, 3, 7, 23
#define PPUTLIMPL_UDEC_2897u 0xB51u, 0, 0, 0, 0, 11, 53,
#define PPUTLIMPL_UDEC_2896u 0xB50u, 0, 0, 0, 0, 11, 53, 2, 2, 2, 2, 181
#define PPUTLIMPL_UDEC_2895u 0xB4Fu, 0, 0, 0, 0, 11, 53, 3, 5, 193
#define PPUTLIMPL_UDEC_2894u 0xB4Eu, 0, 0, 0, 0, 11, 53, 2, 1447
#define PPUTLIMPL_UDEC_2893u 0xB4Du, 0, 0, 0, 0, 11, 53, 11, 263
#define PPUTLIMPL_UDEC_2892u 0xB4Cu, 0, 0, 0, 0, 11, 53, 2, 2, 3, 241
#define PPUTLIMPL_UDEC_2891u 0xB4Bu, 0, 0, 0, 0, 11, 53, 7, 7, 59
#define PPUTLIMPL_UDEC_2890u 0xB4Au, 0, 0, 0, 0, 11, 53, 2, 5, 17, 17
#define PPUTLIMPL_UDEC_2889u 0xB49u, 0, 0, 0, 0, 11, 53, 3, 3, 3, 107
#define PPUTLIMPL_UDEC_2888u 0xB48u, 0, 0, 0, 0, 11, 53, 2, 2, 2, 19, 19
#define PPUTLIMPL_UDEC_2887u 0xB47u, 0, 0, 0, 0, 11, 53,
#define PPUTLIMPL_UDEC_2886u 0xB46u, 0, 0, 0, 0, 11, 53, 2, 3, 13, 37
#define PPUTLIMPL_UDEC_2885u 0xB45u, 0, 0, 0, 0, 11, 53, 5, 577
#define PPUTLIMPL_UDEC_2884u 0xB44u, 0, 0, 0, 0, 11, 53, 2, 2, 7, 103
#define PPUTLIMPL_UDEC_2883u 0xB43u, 0, 0, 0, 0, 11, 53, 3, 31, 31
#define PPUTLIMPL_UDEC_2882u 0xB42u, 0, 0, 0, 0, 11, 53, 2, 11, 131
#define PPUTLIMPL_UDEC_2881u 0xB41u, 0, 0, 0, 0, 11, 53, 43, 67
#define PPUTLIMPL_UDEC_2880u 0xB40u, 0, 0, 0, 0, 11, 53, 2, 2, 2, 2, 2, 2, 3, 3, 5
#define PPUTLIMPL_UDEC_2879u 0xB3Fu, 0, 0, 0, 0, 11, 53,
#define PPUTLIMPL_UDEC_2878u 0xB3Eu, 0, 0, 0, 0, 11, 53, 2, 1439
#define PPUTLIMPL_UDEC_2877u 0xB3Du, 0, 0, 0, 0, 11, 53, 3, 7, 137
#define PPUTLIMPL_UDEC_2876u 0xB3Cu, 0, 0, 0, 0, 11, 53, 2, 2, 719
#define PPUTLIMPL_UDEC_2875u 0xB3Bu, 0, 0, 0, 0, 11, 53, 5, 5, 5, 23
#define PPUTLIMPL_UDEC_2874u 0xB3Au, 0, 0, 0, 0, 11, 53, 2, 3, 479
#define PPUTLIMPL_UDEC_2873u 0xB39u, 0, 0, 0, 0, 11, 53, 13, 13, 17
#define PPUTLIMPL_UDEC_2872u 0xB38u, 0, 0, 0, 0, 11, 53, 2, 2, 2, 359
#define PPUTLIMPL_UDEC_2871u 0xB37u, 0, 0, 0, 0, 11, 53, 3, 3, 11, 29
#define PPUTLIMPL_UDEC_2870u 0xB36u, 0, 0, 0, 0, 11, 53, 2, 5, 7, 41
#define PPUTLIMPL_UDEC_2869u 0xB35u, 0, 0, 0, 0, 11, 53, 19, 151
#define PPUTLIMPL_UDEC_2868u 0xB34u, 0, 0, 0, 0, 11, 53, 2, 2, 3, 239
#define PPUTLIMPL_UDEC_2867u 0xB33u, 0, 0, 0, 0, 11, 53, 47, 61
#define PPUTLIMPL_UDEC_2866u 0xB32u, 0, 0, 0, 0, 11, 53, 2, 1433
#define PPUTLIMPL_UDEC_2865u 0xB31u, 0, 0, 0, 0, 11, 53, 3, 5, 191
#define PPUTLIMPL_UDEC_2864u 0xB30u, 0, 0, 0, 0, 11, 53, 2, 2, 2, 2, 179
#define PPUTLIMPL_UDEC_2863u 0xB2Fu, 0, 0, 0, 0, 11, 53, 7, 409
#define PPUTLIMPL_UDEC_2862u 0xB2Eu, 0, 0, 0, 0, 11, 53, 2, 3, 3, 3, 53
#define PPUTLIMPL_UDEC_2861u 0xB2Du, 0, 0, 0, 0, 11, 53,
#define PPUTLIMPL_UDEC_2860u 0xB2Cu, 0, 0, 0, 0, 11, 53, 2, 2, 5, 11, 13
#define PPUTLIMPL_UDEC_2859u 0xB2Bu, 0, 0, 0, 0, 11, 53, 3, 953
#define PPUTLIMPL_UDEC_2858u 0xB2Au, 0, 0, 0, 0, 11, 53, 2, 1429
#define PPUTLIMPL_UDEC_2857u 0xB29u, 0, 0, 0, 0, 11, 53,
#define PPUTLIMPL_UDEC_2856u 0xB28u, 0, 0, 0, 0, 11, 53, 2, 2, 2, 3, 7, 17
#define PPUTLIMPL_UDEC_2855u 0xB27u, 0, 0, 0, 0, 11, 53, 5, 571
#define PPUTLIMPL_UDEC_2854u 0xB26u, 0, 0, 0, 0, 11, 53, 2, 1427
#define PPUTLIMPL_UDEC_2853u 0xB25u, 0, 0, 0, 0, 11, 53, 3, 3, 317
#define PPUTLIMPL_UDEC_2852u 0xB24u, 0, 0, 0, 0, 11, 53, 2, 2, 23, 31
#define PPUTLIMPL_UDEC_2851u 0xB23u, 0, 0, 0, 0, 11, 53,
#define PPUTLIMPL_UDEC_2850u 0xB22u, 0, 0, 0, 0, 11, 53, 2, 3, 5, 5, 19
#define PPUTLIMPL_UDEC_2849u 0xB21u, 0, 0, 0, 0, 11, 53, 7, 11, 37
#define PPUTLIMPL_UDEC_2848u 0xB20u, 0, 0, 0, 0, 11, 53, 2, 2, 2, 2, 2, 89
#define PPUTLIMPL_UDEC_2847u 0xB1Fu, 0, 0, 0, 0, 11, 53, 3, 13, 73
#define PPUTLIMPL_UDEC_2846u 0xB1Eu, 0, 0, 0, 0, 11, 53, 2, 1423
#define PPUTLIMPL_UDEC_2845u 0xB1Du, 0, 0, 0, 0, 11, 53, 5, 569
#define PPUTLIMPL_UDEC_2844u 0xB1Cu, 0, 0, 0, 0, 11, 53, 2, 2, 3, 3, 79
#define PPUTLIMPL_UDEC_2843u 0xB1Bu, 0, 0, 0, 0, 11, 53,
#define PPUTLIMPL_UDEC_2842u 0xB1Au, 0, 0, 0, 0, 11, 53, 2, 7, 7, 29
#define PPUTLIMPL_UDEC_2841u 0xB19u, 0, 0, 0, 0, 11, 53, 3, 947
#define PPUTLIMPL_UDEC_2840u 0xB18u, 0, 0, 0, 0, 11, 53, 2, 2, 2, 5, 71
#define PPUTLIMPL_UDEC_2839u 0xB17u, 0, 0, 0, 0, 11, 53, 17, 167
#define PPUTLIMPL_UDEC_2838u 0xB16u, 0, 0, 0, 0, 11, 53, 2, 3, 11, 43
#define PPUTLIMPL_UDEC_2837u 0xB15u, 0, 0, 0, 0, 11, 53,
#define PPUTLIMPL_UDEC_2836u 0xB14u, 0, 0, 0, 0, 11, 53, 2, 2, 709
#define PPUTLIMPL_UDEC_2835u 0xB13u, 0, 0, 0, 0, 11, 53, 3, 3, 3, 3, 5, 7
#define PPUTLIMPL_UDEC_2834u 0xB12u, 0, 0, 0, 0, 11, 53, 2, 13, 109
#define PPUTLIMPL_UDEC_2833u 0xB11u, 0, 0, 0, 0, 11, 53,
#define PPUTLIMPL_UDEC_2832u 0xB10u, 0, 0, 0, 0, 11, 53, 2, 2, 2, 2, 3, 59
#define PPUTLIMPL_UDEC_2831u 0xB0Fu, 0, 0, 0, 0, 11, 53, 19, 149
#define PPUTLIMPL_UDEC_2830u 0xB0Eu, 0, 0, 0, 0, 11, 53, 2, 5, 283
#define PPUTLIMPL_UDEC_2829u 0xB0Du, 0, 0, 0, 0, 11, 53, 3, 23, 41
#define PPUTLIMPL_UDEC_2828u 0xB0Cu, 0, 0, 0, 0, 11, 53, 2, 2, 7, 101
#define PPUTLIMPL_UDEC_2827u 0xB0Bu, 0, 0, 0, 0, 11, 53, 11, 257
#define PPUTLIMPL_UDEC_2826u 0xB0Au, 0, 0, 0, 0, 11, 53, 2, 3, 3, 157
#define PPUTLIMPL_UDEC_2825u 0xB09u, 0, 0, 0, 0, 11, 53, 5, 5, 113
#define PPUTLIMPL_UDEC_2824u 0xB08u, 0, 0, 0, 0, 11, 53, 2, 2, 2, 353
#define PPUTLIMPL_UDEC_2823u 0xB07u, 0, 0, 0, 0, 11, 53, 3, 941
#define PPUTLIMPL_UDEC_2822u 0xB06u, 0, 0, 0, 0, 11, 53, 2, 17, 83
#define PPUTLIMPL_UDEC_2821u 0xB05u, 0, 0, 0, 0, 11, 53, 7, 13, 31
#define PPUTLIMPL_UDEC_2820u 0xB04u, 0, 0, 0, 0, 11, 53, 2, 2, 3, 5, 47
#define PPUTLIMPL_UDEC_2819u 0xB03u, 0, 0, 0, 0, 11, 53,
#define PPUTLIMPL_UDEC_2818u 0xB02u, 0, 0, 0, 0, 11, 53, 2, 1409
#define PPUTLIMPL_UDEC_2817u 0xB01u, 0, 0, 0, 0, 11, 53, 3, 3, 313
#define PPUTLIMPL_UDEC_2816u 0xB00u, 0, 0, 0, 0, 11, 53, 2, 2, 2, 2, 2, 2, 2, 2, 11
#define PPUTLIMPL_UDEC_2815u 0xAFFu, 0, 0, 0, 0, 11, 53, 5, 563
#define PPUTLIMPL_UDEC_2814u 0xAFEu, 0, 0, 0, 0, 11, 53, 2, 3, 7, 67
#define PPUTLIMPL_UDEC_2813u 0xAFDu, 0, 0, 0, 0, 11, 53, 29, 97
#define PPUTLIMPL_UDEC_2812u 0xAFCu, 0, 0, 0, 0, 11, 53, 2, 2, 19, 37
#define PPUTLIMPL_UDEC_2811u 0xAFBu, 0, 0, 0, 0, 11, 53, 3, 937
#define PPUTLIMPL_UDEC_2810u 0xAFAu, 0, 0, 0, 0, 11, 53, 2, 5, 281
#define PPUTLIMPL_UDEC_2809u 0xAF9u, 0, 0, 0, 0, 11, 53, 53, 53
#define PPUTLIMPL_UDEC_2808u 0xAF8u, 0, 0, 0, 0, 11, 52, 2, 2, 2, 3, 3, 3, 13
#define PPUTLIMPL_UDEC_2807u 0xAF7u, 0, 0, 0, 0, 11, 52, 7, 401
#define PPUTLIMPL_UDEC_2806u 0xAF6u, 0, 0, 0, 0, 11, 52, 2, 23, 61
#define PPUTLIMPL_UDEC_2805u 0xAF5u, 0, 0, 0, 0, 11, 52, 3, 5, 11, 17
#define PPUTLIMPL_UDEC_2804u 0xAF4u, 0, 0, 0, 0, 11, 52, 2, 2, 701
#define PPUTLIMPL_UDEC_2803u 0xAF3u, 0, 0, 0, 0, 11, 52,
#define PPUTLIMPL_UDEC_2802u 0xAF2u, 0, 0, 0, 0, 11, 52, 2, 3, 467
#define PPUTLIMPL_UDEC_2801u 0xAF1u, 0, 0, 0, 0, 11, 52,
#define PPUTLIMPL_UDEC_2800u 0xAF0u, 0, 0, 0, 0, 11, 52, 2, 2, 2, 2, 5, 5, 7
#define PPUTLIMPL_UDEC_2799u 0xAEFu, 0, 0, 0, 0, 11, 52, 3, 3, 311
#define PPUTLIMPL_UDEC_2798u 0xAEEu, 0, 0, 0, 0, 11, 52, 2, 1399
#define PPUTLIMPL_UDEC_2797u 0xAEDu, 0, 0, 0, 0, 11, 52,
#define PPUTLIMPL_UDEC_2796u 0xAECu, 0, 0, 0, 0, 11, 52, 2, 2, 3, 233
#define PPUTLIMPL_UDEC_2795u 0xAEBu, 0, 0, 0, 0, 11, 52, 5, 13, 43
#define PPUTLIMPL_UDEC_2794u 0xAEAu, 0, 0, 0, 0, 11, 52, 2, 11, 127
#define PPUTLIMPL_UDEC_2793u 0xAE9u, 0, 0, 0, 0, 11, 52, 3, 7, 7, 19
#define PPUTLIMPL_UDEC_2792u 0xAE8u, 0, 0, 0, 0, 11, 52, 2, 2, 2, 349
#define PPUTLIMPL_UDEC_2791u 0xAE7u, 0, 0, 0, 0, 11, 52,
#define PPUTLIMPL_UDEC_2790u 0xAE6u, 0, 0, 0, 0, 11, 52, 2, 3, 3, 5, 31
#define PPUTLIMPL_UDEC_2789u 0xAE5u, 0, 0, 0, 0, 11, 52,
#define PPUTLIMPL_UDEC_2788u 0xAE4u, 0, 0, 0, 0, 11, 52, 2, 2, 17, 41
#define PPUTLIMPL_UDEC_2787u 0xAE3u, 0, 0, 0, 0, 11, 52, 3, 929
#define PPUTLIMPL_UDEC_2786u 0xAE2u, 0, 0, 0, 0, 11, 52, 2, 7, 199
#define PPUTLIMPL_UDEC_2785u 0xAE1u, 0, 0, 0, 0, 11, 52, 5, 557
#define PPUTLIMPL_UDEC_2784u 0xAE0u, 0, 0, 0, 0, 11, 52, 2, 2, 2, 2, 2, 3, 29
#define PPUTLIMPL_UDEC_2783u 0xADFu, 0, 0, 0, 0, 11, 52, 11, 11, 23
#define PPUTLIMPL_UDEC_2782u 0xADEu, 0, 0, 0, 0, 11, 52, 2, 13, 107
#define PPUTLIMPL_UDEC_2781u 0xADDu, 0, 0, 0, 0, 11, 52, 3, 3, 3, 103
#define PPUTLIMPL_UDEC_2780u 0xADCu, 0, 0, 0, 0, 11, 52, 2, 2, 5, 139
#define PPUTLIMPL_UDEC_2779u 0xADBu, 0, 0, 0, 0, 11, 52, 7, 397
#define PPUTLIMPL_UDEC_2778u 0xADAu, 0, 0, 0, 0, 11, 52, 2, 3, 463
#define PPUTLIMPL_UDEC_2777u 0xAD9u, 0, 0, 0, 0, 11, 52,
#define PPUTLIMPL_UDEC_2776u 0xAD8u, 0, 0, 0, 0, 11, 52, 2, 2, 2, 347
#define PPUTLIMPL_UDEC_2775u 0xAD7u, 0, 0, 0, 0, 11, 52, 3, 5, 5, 37
#define PPUTLIMPL_UDEC_2774u 0xAD6u, 0, 0, 0, 0, 11, 52, 2, 19, 73
#define PPUTLIMPL_UDEC_2773u 0xAD5u, 0, 0, 0, 0, 11, 52, 47, 59
#define PPUTLIMPL_UDEC_2772u 0xAD4u, 0, 0, 0, 0, 11, 52, 2, 2, 3, 3, 7, 11
#define PPUTLIMPL_UDEC_2771u 0xAD3u, 0, 0, 0, 0, 11, 52, 17, 163
#define PPUTLIMPL_UDEC_2770u 0xAD2u, 0, 0, 0, 0, 11, 52, 2, 5, 277
#define PPUTLIMPL_UDEC_2769u 0xAD1u, 0, 0, 0, 0, 11, 52, 3, 13, 71
#define PPUTLIMPL_UDEC_2768u 0xAD0u, 0, 0, 0, 0, 11, 52, 2, 2, 2, 2, 173
#define PPUTLIMPL_UDEC_2767u 0xACFu, 0, 0, 0, 0, 11, 52,
#define PPUTLIMPL_UDEC_2766u 0xACEu, 0, 0, 0, 0, 11, 52, 2, 3, 461
#define PPUTLIMPL_UDEC_2765u 0xACDu, 0, 0, 0, 0, 11, 52, 5, 7, 79
#define PPUTLIMPL_UDEC_2764u 0xACCu, 0, 0, 0, 0, 11, 52, 2, 2, 691
#define PPUTLIMPL_UDEC_2763u 0xACBu, 0, 0, 0, 0, 11, 52, 3, 3, 307
#define PPUTLIMPL_UDEC_2762u 0xACAu, 0, 0, 0, 0, 11, 52, 2, 1381
#define PPUTLIMPL_UDEC_2761u 0xAC9u, 0, 0, 0, 0, 11, 52, 11, 251
#define PPUTLIMPL_UDEC_2760u 0xAC8u, 0, 0, 0, 0, 11, 52, 2, 2, 2, 3, 5, 23
#define PPUTLIMPL_UDEC_2759u 0xAC7u, 0, 0, 0, 0, 11, 52, 31, 89
#define PPUTLIMPL_UDEC_2758u 0xAC6u, 0, 0, 0, 0, 11, 52, 2, 7, 197
#define PPUTLIMPL_UDEC_2757u 0xAC5u, 0, 0, 0, 0, 11, 52, 3, 919
#define PPUTLIMPL_UDEC_2756u 0xAC4u, 0, 0, 0, 0, 11, 52, 2, 2, 13, 53
#define PPUTLIMPL_UDEC_2755u 0xAC3u, 0, 0, 0, 0, 11, 52, 5, 19, 29
#define PPUTLIMPL_UDEC_2754u 0xAC2u, 0, 0, 0, 0, 11, 52, 2, 3, 3, 3, 3, 17
#define PPUTLIMPL_UDEC_2753u 0xAC1u, 0, 0, 0, 0, 11, 52,
#define PPUTLIMPL_UDEC_2752u 0xAC0u, 0, 0, 0, 0, 11, 52, 2, 2, 2, 2, 2, 2, 43
#define PPUTLIMPL_UDEC_2751u 0xABFu, 0, 0, 0, 0, 11, 52, 3, 7, 131
#define PPUTLIMPL_UDEC_2750u 0xABEu, 0, 0, 0, 0, 11, 52, 2, 5, 5, 5, 11
#define PPUTLIMPL_UDEC_2749u 0xABDu, 0, 0, 0, 0, 11, 52,
#define PPUTLIMPL_UDEC_2748u 0xABCu, 0, 0, 0, 0, 11, 52, 2, 2, 3, 229
#define PPUTLIMPL_UDEC_2747u 0xABBu, 0, 0, 0, 0, 11, 52, 41, 67
#define PPUTLIMPL_UDEC_2746u 0xABAu, 0, 0, 0, 0, 11, 52, 2, 1373
#define PPUTLIMPL_UDEC_2745u 0xAB9u, 0, 0, 0, 0, 11, 52, 3, 3, 5, 61
#define PPUTLIMPL_UDEC_2744u 0xAB8u, 0, 0, 0, 0, 11, 52, 2, 2, 2, 7, 7, 7
#define PPUTLIMPL_UDEC_2743u 0xAB7u, 0, 0, 0, 0, 11, 52, 13, 211
#define PPUTLIMPL_UDEC_2742u 0xAB6u, 0, 0, 0, 0, 11, 52, 2, 3, 457
#define PPUTLIMPL_UDEC_2741u 0xAB5u, 0, 0, 0, 0, 11, 52,
#define PPUTLIMPL_UDEC_2740u 0xAB4u, 0, 0, 0, 0, 11, 52, 2, 2, 5, 137
#define PPUTLIMPL_UDEC_2739u 0xAB3u, 0, 0, 0, 0, 11, 52, 3, 11, 83
#define PPUTLIMPL_UDEC_2738u 0xAB2u, 0, 0, 0, 0, 11, 52, 2, 37, 37
#define PPUTLIMPL_UDEC_2737u 0xAB1u, 0, 0, 0, 0, 11, 52, 7, 17, 23
#define PPUTLIMPL_UDEC_2736u 0xAB0u, 0, 0, 0, 0, 11, 52, 2, 2, 2, 2, 3, 3, 19
#define PPUTLIMPL_UDEC_2735u 0xAAFu, 0, 0, 0, 0, 11, 52, 5, 547
#define PPUTLIMPL_UDEC_2734u 0xAAEu, 0, 0, 0, 0, 11, 52, 2, 1367
#define PPUTLIMPL_UDEC_2733u 0xAADu, 0, 0, 0, 0, 11, 52, 3, 911
#define PPUTLIMPL_UDEC_2732u 0xAACu, 0, 0, 0, 0, 11, 52, 2, 2, 683
#define PPUTLIMPL_UDEC_2731u 0xAABu, 0, 0, 0, 0, 11, 52,
#define PPUTLIMPL_UDEC_2730u 0xAAAu, 0, 0, 0, 0, 11, 52, 2, 3, 5, 7, 13
#define PPUTLIMPL_UDEC_2729u 0xAA9u, 0, 0, 0, 0, 11, 52,
#define PPUTLIMPL_UDEC_2728u 0xAA8u, 0, 0, 0, 0, 11, 52, 2, 2, 2, 11, 31
#define PPUTLIMPL_UDEC_2727u 0xAA7u, 0, 0, 0, 0, 11, 52, 3, 3, 3, 101
#define PPUTLIMPL_UDEC_2726u 0xAA6u, 0, 0, 0, 0, 11, 52, 2, 29, 47
#define PPUTLIMPL_UDEC_2725u 0xAA5u, 0, 0, 0, 0, 11, 52, 5, 5, 109
#define PPUTLIMPL_UDEC_2724u 0xAA4u, 0, 0, 0, 0, 11, 52, 2, 2, 3, 227
#define PPUTLIMPL_UDEC_2723u 0xAA3u, 0, 0, 0, 0, 11, 52, 7, 389
#define PPUTLIMPL_UDEC_2722u 0xAA2u, 0, 0, 0, 0, 11, 52, 2, 1361
#define PPUTLIMPL_UDEC_2721u 0xAA1u, 0, 0, 0, 0, 11, 52, 3, 907
#define PPUTLIMPL_UDEC_2720u 0xAA0u, 0, 0, 0, 0, 11, 52, 2, 2, 2, 2, 2, 5, 17
#define PPUTLIMPL_UDEC_2719u 0xA9Fu, 0, 0, 0, 0, 11, 52,
#define PPUTLIMPL_UDEC_2718u 0xA9Eu, 0, 0, 0, 0, 11, 52, 2, 3, 3, 151
#define PPUTLIMPL_UDEC_2717u 0xA9Du, 0, 0, 0, 0, 11, 52, 11, 13, 19
#define PPUTLIMPL_UDEC_2716u 0xA9Cu, 0, 0, 0, 0, 11, 52, 2, 2, 7, 97
#define PPUTLIMPL_UDEC_2715u 0xA9Bu, 0, 0, 0, 0, 11, 52, 3, 5, 181
#define PPUTLIMPL_UDEC_2714u 0xA9Au, 0, 0, 0, 0, 11, 52, 2, 23, 59
#define PPUTLIMPL_UDEC_2713u 0xA99u, 0, 0, 0, 0, 11, 52,
#define PPUTLIMPL_UDEC_2712u 0xA98u, 0, 0, 0, 0, 11, 52, 2, 2, 2, 3, 113
#define PPUTLIMPL_UDEC_2711u 0xA97u, 0, 0, 0, 0, 11, 52,
#define PPUTLIMPL_UDEC_2710u 0xA96u, 0, 0, 0, 0, 11, 52, 2, 5, 271
#define PPUTLIMPL_UDEC_2709u 0xA95u, 0, 0, 0, 0, 11, 52, 3, 3, 7, 43
#define PPUTLIMPL_UDEC_2708u 0xA94u, 0, 0, 0, 0, 11, 52, 2, 2, 677
#define PPUTLIMPL_UDEC_2707u 0xA93u, 0, 0, 0, 0, 11, 52,
#define PPUTLIMPL_UDEC_2706u 0xA92u, 0, 0, 0, 0, 11, 52, 2, 3, 11, 41
#define PPUTLIMPL_UDEC_2705u 0xA91u, 0, 0, 0, 0, 11, 52, 5, 541
#define PPUTLIMPL_UDEC_2704u 0xA90u, 0, 0, 0, 0, 11, 52, 2, 2, 2, 2, 13, 13
#define PPUTLIMPL_UDEC_2703u 0xA8Fu, 0, 0, 0, 0, 11, 51, 3, 17, 53
#define PPUTLIMPL_UDEC_2702u 0xA8Eu, 0, 0, 0, 0, 11, 51, 2, 7, 193
#define PPUTLIMPL_UDEC_2701u 0xA8Du, 0, 0, 0, 0, 11, 51, 37, 73
#define PPUTLIMPL_UDEC_2700u 0xA8Cu, 0, 0, 0, 0, 11, 51, 2, 2, 3, 3, 3, 5, 5
#define PPUTLIMPL_UDEC_2699u 0xA8Bu, 0, 0, 0, 0, 11, 51,
#define PPUTLIMPL_UDEC_2698u 0xA8Au, 0, 0, 0, 0, 11, 51, 2, 19, 71
#define PPUTLIMPL_UDEC_2697u 0xA89u, 0, 0, 0, 0, 11, 51, 3, 29, 31
#define PPUTLIMPL_UDEC_2696u 0xA88u, 0, 0, 0, 0, 11, 51, 2, 2, 2, 337
#define PPUTLIMPL_UDEC_2695u 0xA87u, 0, 0, 0, 0, 11, 51, 5, 7, 7, 11
#define PPUTLIMPL_UDEC_2694u 0xA86u, 0, 0, 0, 0, 11, 51, 2, 3, 449
#define PPUTLIMPL_UDEC_2693u 0xA85u, 0, 0, 0, 0, 11, 51,
#define PPUTLIMPL_UDEC_2692u 0xA84u, 0, 0, 0, 0, 11, 51, 2, 2, 673
#define PPUTLIMPL_UDEC_2691u 0xA83u, 0, 0, 0, 0, 11, 51, 3, 3, 13, 23
#define PPUTLIMPL_UDEC_2690u 0xA82u, 0, 0, 0, 0, 11, 51, 2, 5, 269
#define PPUTLIMPL_UDEC_2689u 0xA81u, 0, 0, 0, 0, 11, 51,
#define PPUTLIMPL_UDEC_2688u 0xA80u, 0, 0, 0, 0, 11, 51, 2, 2, 2, 2, 2, 2, 2, 3, 7
#define PPUTLIMPL_UDEC_2687u 0xA7Fu, 0, 0, 0, 0, 11, 51,
#define PPUTLIMPL_UDEC_2686u 0xA7Eu, 0, 0, 0, 0, 11, 51, 2, 17, 79
#define PPUTLIMPL_UDEC_2685u 0xA7Du, 0, 0, 0, 0, 11, 51, 3, 5, 179
#define PPUTLIMPL_UDEC_2684u 0xA7Cu, 0, 0, 0, 0, 11, 51, 2, 2, 11, 61
#define PPUTLIMPL_UDEC_2683u 0xA7Bu, 0, 0, 0, 0, 11, 51,
#define PPUTLIMPL_UDEC_2682u 0xA7Au, 0, 0, 0, 0, 11, 51, 2, 3, 3, 149
#define PPUTLIMPL_UDEC_2681u 0xA79u, 0, 0, 0, 0, 11, 51, 7, 383
#define PPUTLIMPL_UDEC_2680u 0xA78u, 0, 0, 0, 0, 11, 51, 2, 2, 2, 5, 67
#define PPUTLIMPL_UDEC_2679u 0xA77u, 0, 0, 0, 0, 11, 51, 3, 19, 47
#define PPUTLIMPL_UDEC_2678u 0xA76u, 0, 0, 0, 0, 11, 51, 2, 13, 103
#define PPUTLIMPL_UDEC_2677u 0xA75u, 0, 0, 0, 0, 11, 51,
#define PPUTLIMPL_UDEC_2676u 0xA74u, 0, 0, 0, 0, 11, 51, 2, 2, 3, 223
#define PPUTLIMPL_UDEC_2675u 0xA73u, 0, 0, 0, 0, 11, 51, 5, 5, 107
#define PPUTLIMPL_UDEC_2674u 0xA72u, 0, 0, 0, 0, 11, 51, 2, 7, 191
#define PPUTLIMPL_UDEC_2673u 0xA71u, 0, 0, 0, 0, 11, 51, 3, 3, 3, 3, 3, 11
#define PPUTLIMPL_UDEC_2672u 0xA70u, 0, 0, 0, 0, 11, 51, 2, 2, 2, 2, 167
#define PPUTLIMPL_UDEC_2671u 0xA6Fu, 0, 0, 0, 0, 11, 51,
#define PPUTLIMPL_UDEC_2670u 0xA6Eu, 0, 0, 0, 0, 11, 51, 2, 3, 5, 89
#define PPUTLIMPL_UDEC_2669u 0xA6Du, 0, 0, 0, 0, 11, 51, 17, 157
#define PPUTLIMPL_UDEC_2668u 0xA6Cu, 0, 0, 0, 0, 11, 51, 2, 2, 23, 29
#define PPUTLIMPL_UDEC_2667u 0xA6Bu, 0, 0, 0, 0, 11, 51, 3, 7, 127
#define PPUTLIMPL_UDEC_2666u 0xA6Au, 0, 0, 0, 0, 11, 51, 2, 31, 43
#define PPUTLIMPL_UDEC_2665u 0xA69u, 0, 0, 0, 0, 11, 51, 5, 13, 41
#define PPUTLIMPL_UDEC_2664u 0xA68u, 0, 0, 0, 0, 11, 51, 2, 2, 2, 3, 3, 37
#define PPUTLIMPL_UDEC_2663u 0xA67u, 0, 0, 0, 0, 11, 51,
#define PPUTLIMPL_UDEC_2662u 0xA66u, 0, 0, 0, 0, 11, 51, 2, 11, 11, 11
#define PPUTLIMPL_UDEC_2661u 0xA65u, 0, 0, 0, 0, 11, 51, 3, 887
#define PPUTLIMPL_UDEC_2660u 0xA64u, 0, 0, 0, 0, 11, 51, 2, 2, 5, 7, 19
#define PPUTLIMPL_UDEC_2659u 0xA63u, 0, 0, 0, 0, 11, 51,
#define PPUTLIMPL_UDEC_2658u 0xA62u, 0, 0, 0, 0, 11, 51, 2, 3, 443
#define PPUTLIMPL_UDEC_2657u 0xA61u, 0, 0, 0, 0, 11, 51,
#define PPUTLIMPL_UDEC_2656u 0xA60u, 0, 0, 0, 0, 11, 51, 2, 2, 2, 2, 2, 83
#define PPUTLIMPL_UDEC_2655u 0xA5Fu, 0, 0, 0, 0, 11, 51, 3, 3, 5, 59
#define PPUTLIMPL_UDEC_2654u 0xA5Eu, 0, 0, 0, 0, 11, 51, 2, 1327
#define PPUTLIMPL_UDEC_2653u 0xA5Du, 0, 0, 0, 0, 11, 51, 7, 379
#define PPUTLIMPL_UDEC_2652u 0xA5Cu, 0, 0, 0, 0, 11, 51, 2, 2, 3, 13, 17
#define PPUTLIMPL_UDEC_2651u 0xA5Bu, 0, 0, 0, 0, 11, 51, 11, 241
#define PPUTLIMPL_UDEC_2650u 0xA5Au, 0, 0, 0, 0, 11, 51, 2, 5, 5, 53
#define PPUTLIMPL_UDEC_2649u 0xA59u, 0, 0, 0, 0, 11, 51, 3, 883
#define PPUTLIMPL_UDEC_2648u 0xA58u, 0, 0, 0, 0, 11, 51, 2, 2, 2, 331
#define PPUTLIMPL_UDEC_2647u 0xA57u, 0, 0, 0, 0, 11, 51,
#define PPUTLIMPL_UDEC_2646u 0xA56u, 0, 0, 0, 0, 11, 51, 2, 3, 3, 3, 7, 7
#define PPUTLIMPL_UDEC_2645u 0xA55u, 0, 0, 0, 0, 11, 51, 5, 23, 23
#define PPUTLIMPL_UDEC_2644u 0xA54u, 0, 0, 0, 0, 11, 51, 2, 2, 661
#define PPUTLIMPL_UDEC_2643u 0xA53u, 0, 0, 0, 0, 11, 51, 3, 881
#define PPUTLIMPL_UDEC_2642u 0xA52u, 0, 0, 0, 0, 11, 51, 2, 1321
#define PPUTLIMPL_UDEC_2641u 0xA51u, 0, 0, 0, 0, 11, 51, 19, 139
#define PPUTLIMPL_UDEC_2640u 0xA50u, 0, 0, 0, 0, 11, 51, 2, 2, 2, 2, 3, 5, 11
#define PPUTLIMPL_UDEC_2639u 0xA4Fu, 0, 0, 0, 0, 11, 51, 7, 13, 29
#define PPUTLIMPL_UDEC_2638u 0xA4Eu, 0, 0, 0, 0, 11, 51, 2, 1319
#define PPUTLIMPL_UDEC_2637u 0xA4Du, 0, 0, 0, 0, 11, 51, 3, 3, 293
#define PPUTLIMPL_UDEC_2636u 0xA4Cu, 0, 0, 0, 0, 11, 51, 2, 2, 659
#define PPUTLIMPL_UDEC_2635u 0xA4Bu, 0, 0, 0, 0, 11, 51, 5, 17, 31
#define PPUTLIMPL_UDEC_2634u 0xA4Au, 0, 0, 0, 0, 11, 51, 2, 3, 439
#define PPUTLIMPL_UDEC_2633u 0xA49u, 0, 0, 0, 0, 11, 51,
#define PPUTLIMPL_UDEC_2632u 0xA48u, 0, 0, 0, 0, 11, 51, 2, 2, 2, 7, 47
#define PPUTLIMPL_UDEC_2631u 0xA47u, 0, 0, 0, 0, 11, 51, 3, 877
#define PPUTLIMPL_UDEC_2630u 0xA46u, 0, 0, 0, 0, 11, 51, 2, 5, 263
#define PPUTLIMPL_UDEC_2629u 0xA45u, 0, 0, 0, 0, 11, 51, 11, 239
#define PPUTLIMPL_UDEC_2628u 0xA44u, 0, 0, 0, 0, 11, 51, 2, 2, 3, 3, 73
#define PPUTLIMPL_UDEC_2627u 0xA43u, 0, 0, 0, 0, 11, 51, 37, 71
#define PPUTLIMPL_UDEC_2626u 0xA42u, 0, 0, 0, 0, 11, 51, 2, 13, 101
#define PPUTLIMPL_UDEC_2625u 0xA41u, 0, 0, 0, 0, 11, 51, 3, 5, 5, 5, 7
#define PPUTLIMPL_UDEC_2624u 0xA40u, 0, 0, 0, 0, 11, 51, 2, 2, 2, 2, 2, 2, 41
#define PPUTLIMPL_UDEC_2623u 0xA3Fu, 0, 0, 0, 0, 11, 51, 43, 61
#define PPUTLIMPL_UDEC_2622u 0xA3Eu, 0, 0, 0, 0, 11, 51, 2, 3, 19, 23
#define PPUTLIMPL_UDEC_2621u 0xA3Du, 0, 0, 0, 0, 11, 51,
#define PPUTLIMPL_UDEC_2620u 0xA3Cu, 0, 0, 0, 0, 11, 51, 2, 2, 5, 131
#define PPUTLIMPL_UDEC_2619u 0xA3Bu, 0, 0, 0, 0, 11, 51, 3, 3, 3, 97
#define PPUTLIMPL_UDEC_2618u 0xA3Au, 0, 0, 0, 0, 11, 51, 2, 7, 11, 17
#define PPUTLIMPL_UDEC_2617u 0xA39u, 0, 0, 0, 0, 11, 51,
#define PPUTLIMPL_UDEC_2616u 0xA38u, 0, 0, 0, 0, 11, 51, 2, 2, 2, 3, 109
#define PPUTLIMPL_UDEC_2615u 0xA37u, 0, 0, 0, 0, 11, 51, 5, 523
#define PPUTLIMPL_UDEC_2614u 0xA36u, 0, 0, 0, 0, 11, 51, 2, 1307
#define PPUTLIMPL_UDEC_2613u 0xA35u, 0, 0, 0, 0, 11, 51, 3, 13, 67
#define PPUTLIMPL_UDEC_2612u 0xA34u, 0, 0, 0, 0, 11, 51, 2, 2, 653
#define PPUTLIMPL_UDEC_2611u 0xA33u, 0, 0, 0, 0, 11, 51, 7, 373
#define PPUTLIMPL_UDEC_2610u 0xA32u, 0, 0, 0, 0, 11, 51, 2, 3, 3, 5, 29
#define PPUTLIMPL_UDEC_2609u 0xA31u, 0, 0, 0, 0, 11, 51,
#define PPUTLIMPL_UDEC_2608u 0xA30u, 0, 0, 0, 0, 11, 51, 2, 2, 2, 2, 163
#define PPUTLIMPL_UDEC_2607u 0xA2Fu, 0, 0, 0, 0, 11, 51, 3, 11, 79
#define PPUTLIMPL_UDEC_2606u 0xA2Eu, 0, 0, 0, 0, 11, 51, 2, 1303
#define PPUTLIMPL_UDEC_2605u 0xA2Du, 0, 0, 0, 0, 11, 51, 5, 521
#define PPUTLIMPL_UDEC_2604u 0xA2Cu, 0, 0, 0, 0, 11, 51, 2, 2, 3, 7, 31
#define PPUTLIMPL_UDEC_2603u 0xA2Bu, 0, 0, 0, 0, 11, 51, 19, 137
#define PPUTLIMPL_UDEC_2602u 0xA2Au, 0, 0, 0, 0, 11, 51, 2, 1301
#define PPUTLIMPL_UDEC_2601u 0xA29u, 0, 0, 0, 0, 11, 51, 3, 3, 17, 17
#define PPUTLIMPL_UDEC_2600u 0xA28u, 0, 0, 0, 0, 11, 50, 2, 2, 2, 5, 5, 13
#define PPUTLIMPL_UDEC_2599u 0xA27u, 0, 0, 0, 0, 11, 50, 23, 113
#define PPUTLIMPL_UDEC_2598u 0xA26u, 0, 0, 0, 0, 11, 50, 2, 3, 433
#define PPUTLIMPL_UDEC_2597u 0xA25u, 0, 0, 0, 0, 11, 50, 7, 7, 53
#define PPUTLIMPL_UDEC_2596u 0xA24u, 0, 0, 0, 0, 11, 50, 2, 2, 11, 59
#define PPUTLIMPL_UDEC_2595u 0xA23u, 0, 0, 0, 0, 11, 50, 3, 5, 173
#define PPUTLIMPL_UDEC_2594u 0xA22u, 0, 0, 0, 0, 11, 50, 2, 1297
#define PPUTLIMPL_UDEC_2593u 0xA21u, 0, 0, 0, 0, 11, 50,
#define PPUTLIMPL_UDEC_2592u 0xA20u, 0, 0, 0, 0, 11, 50, 2, 2, 2, 2, 2, 3, 3, 3, 3
#define PPUTLIMPL_UDEC_2591u 0xA1Fu, 0, 0, 0, 0, 11, 50,
#define PPUTLIMPL_UDEC_2590u 0xA1Eu, 0, 0, 0, 0, 11, 50, 2, 5, 7, 37
#define PPUTLIMPL_UDEC_2589u 0xA1Du, 0, 0, 0, 0, 11, 50, 3, 863
#define PPUTLIMPL_UDEC_2588u 0xA1Cu, 0, 0, 0, 0, 11, 50, 2, 2, 647
#define PPUTLIMPL_UDEC_2587u 0xA1Bu, 0, 0, 0, 0, 11, 50, 13, 199
#define PPUTLIMPL_UDEC_2586u 0xA1Au, 0, 0, 0, 0, 11, 50, 2, 3, 431
#define PPUTLIMPL_UDEC_2585u 0xA19u, 0, 0, 0, 0, 11, 50, 5, 11, 47
#define PPUTLIMPL_UDEC_2584u 0xA18u, 0, 0, 0, 0, 11, 50, 2, 2, 2, 17, 19
#define PPUTLIMPL_UDEC_2583u 0xA17u, 0, 0, 0, 0, 11, 50, 3, 3, 7, 41
#define PPUTLIMPL_UDEC_2582u 0xA16u, 0, 0, 0, 0, 11, 50, 2, 1291
#define PPUTLIMPL_UDEC_2581u 0xA15u, 0, 0, 0, 0, 11, 50, 29, 89
#define PPUTLIMPL_UDEC_2580u 0xA14u, 0, 0, 0, 0, 11, 50, 2, 2, 3, 5, 43
#define PPUTLIMPL_UDEC_2579u 0xA13u, 0, 0, 0, 0, 11, 50,
#define PPUTLIMPL_UDEC_2578u 0xA12u, 0, 0, 0, 0, 11, 50, 2, 1289
#define PPUTLIMPL_UDEC_2577u 0xA11u, 0, 0, 0, 0, 11, 50, 3, 859
#define PPUTLIMPL_UDEC_2576u 0xA10u, 0, 0, 0, 0, 11, 50, 2, 2, 2, 2, 7, 23
#define PPUTLIMPL_UDEC_2575u 0xA0Fu, 0, 0, 0, 0, 11, 50, 5, 5, 103
#define PPUTLIMPL_UDEC_2574u 0xA0Eu, 0, 0, 0, 0, 11, 50, 2, 3, 3, 11, 13
#define PPUTLIMPL_UDEC_2573u 0xA0Du, 0, 0, 0, 0, 11, 50, 31, 83
#define PPUTLIMPL_UDEC_2572u 0xA0Cu, 0, 0, 0, 0, 11, 50, 2, 2, 643
#define PPUTLIMPL_UDEC_2571u 0xA0Bu, 0, 0, 0, 0, 11, 50, 3, 857
#define PPUTLIMPL_UDEC_2570u 0xA0Au, 0, 0, 0, 0, 11, 50, 2, 5, 257
#define PPUTLIMPL_UDEC_2569u 0xA09u, 0, 0, 0, 0, 11, 50, 7, 367
#define PPUTLIMPL_UDEC_2568u 0xA08u, 0, 0, 0, 0, 11, 50, 2, 2, 2, 3, 107
#define PPUTLIMPL_UDEC_2567u 0xA07u, 0, 0, 0, 0, 11, 50, 17, 151
#define PPUTLIMPL_UDEC_2566u 0xA06u, 0, 0, 0, 0, 11, 50, 2, 1283
#define PPUTLIMPL_UDEC_2565u 0xA05u, 0, 0, 0, 0, 11, 50, 3, 3, 3, 5, 19
#define PPUTLIMPL_UDEC_2564u 0xA04u, 0, 0, 0, 0, 11, 50, 2, 2, 641
#define PPUTLIMPL_UDEC_2563u 0xA03u, 0, 0, 0, 0, 11, 50, 11, 233
#define PPUTLIMPL_UDEC_2562u 0xA02u, 0, 0, 0, 0, 11, 50, 2, 3, 7, 61
#define PPUTLIMPL_UDEC_2561u 0xA01u, 0, 0, 0, 0, 11, 50, 13, 197
#define PPUTLIMPL_UDEC_2560u 0xA00u, 0, 0, 0, 0, 11, 50, 2, 2, 2, 2, 2, 2, 2, 2, 2, 5
#define PPUTLIMPL_UDEC_2559u 0x9FFu, 0, 0, 0, 0, 11, 50, 3, 853
#define PPUTLIMPL_UDEC_2558u 0x9FEu, 0, 0, 0, 0, 11, 50, 2, 1279
#define PPUTLIMPL_UDEC_2557u 0x9FDu, 0, 0, 0, 0, 11, 50,
#define PPUTLIMPL_UDEC_2556u 0x9FCu, 0, 0, 0, 0, 11, 50, 2, 2, 3, 3, 71
#define PPUTLIMPL_UDEC_2555u 0x9FBu, 0, 0, 0, 0, 11, 50, 5, 7, 73
#define PPUTLIMPL_UDEC_2554u 0x9FAu, 0, 0, 0, 0, 11, 50, 2, 1277
#define PPUTLIMPL_UDEC_2553u 0x9F9u, 0, 0, 0, 0, 11, 50, 3, 23, 37
#define PPUTLIMPL_UDEC_2552u 0x9F8u, 0, 0, 0, 0, 11, 50, 2, 2, 2, 11, 29
#define PPUTLIMPL_UDEC_2551u 0x9F7u, 0, 0, 0, 0, 11, 50,
#define PPUTLIMPL_UDEC_2550u 0x9F6u, 0, 0, 0, 0, 11, 50, 2, 3, 5, 5, 17
#define PPUTLIMPL_UDEC_2549u 0x9F5u, 0, 0, 0, 0, 11, 50,
#define PPUTLIMPL_UDEC_2548u 0x9F4u, 0, 0, 0, 0, 11, 50, 2, 2, 7, 7, 13
#define PPUTLIMPL_UDEC_2547u 0x9F3u, 0, 0, 0, 0, 11, 50, 3, 3, 283
#define PPUTLIMPL_UDEC_2546u 0x9F2u, 0, 0, 0, 0, 11, 50, 2, 19, 67
#define PPUTLIMPL_UDEC_2545u 0x9F1u, 0, 0, 0, 0, 11, 50, 5, 509
#define PPUTLIMPL_UDEC_2544u 0x9F0u, 0, 0, 0, 0, 11, 50, 2, 2, 2, 2, 3, 53
#define PPUTLIMPL_UDEC_2543u 0x9EFu, 0, 0, 0, 0, 11, 50,
#define PPUTLIMPL_UDEC_2542u 0x9EEu, 0, 0, 0, 0, 11, 50, 2, 31, 41
#define PPUTLIMPL_UDEC_2541u 0x9EDu, 0, 0, 0, 0, 11, 50, 3, 7, 11, 11
#define PPUTLIMPL_UDEC_2540u 0x9ECu, 0, 0, 0, 0, 11, 50, 2, 2, 5, 127
#define PPUTLIMPL_UDEC_2539u 0x9EBu, 0, 0, 0, 0, 11, 50,
#define PPUTLIMPL_UDEC_2538u 0x9EAu, 0, 0, 0, 0, 11, 50, 2, 3, 3, 3, 47
#define PPUTLIMPL_UDEC_2537u 0x9E9u, 0, 0, 0, 0, 11, 50, 43, 59
#define PPUTLIMPL_UDEC_2536u 0x9E8u, 0, 0, 0, 0, 11, 50, 2, 2, 2, 317
#define PPUTLIMPL_UDEC_2535u 0x9E7u, 0, 0, 0, 0, 11, 50, 3, 5, 13, 13
#define PPUTLIMPL_UDEC_2534u 0x9E6u, 0, 0, 0, 0, 11, 50, 2, 7, 181
#define PPUTLIMPL_UDEC_2533u 0x9E5u, 0, 0, 0, 0, 11, 50, 17, 149
#define PPUTLIMPL_UDEC_2532u 0x9E4u, 0, 0, 0, 0, 11, 50, 2, 2, 3, 211
#define PPUTLIMPL_UDEC_2531u 0x9E3u, 0, 0, 0, 0, 11, 50,
#define PPUTLIMPL_UDEC_2530u 0x9E2u, 0, 0, 0, 0, 11, 50, 2, 5, 11, 23
#define PPUTLIMPL_UDEC_2529u 0x9E1u, 0, 0, 0, 0, 11, 50, 3, 3, 281
#define PPUTLIMPL_UDEC_2528u 0x9E0u, 0, 0, 0, 0, 11, 50, 2, 2, 2, 2, 2, 79
#define PPUTLIMPL_UDEC_2527u 0x9DFu, 0, 0, 0, 0, 11, 50, 7, 19, 19
#define PPUTLIMPL_UDEC_2526u 0x9DEu, 0, 0, 0, 0, 11, 50, 2, 3, 421
#define PPUTLIMPL_UDEC_2525u 0x9DDu, 0, 0, 0, 0, 11, 50, 5, 5, 101
#define PPUTLIMPL_UDEC_2524u 0x9DCu, 0, 0, 0, 0, 11, 50, 2, 2, 631
#define PPUTLIMPL_UDEC_2523u 0x9DBu, 0, 0, 0, 0, 11, 50, 3, 29, 29
#define PPUTLIMPL_UDEC_2522u 0x9DAu, 0, 0, 0, 0, 11, 50, 2, 13, 97
#define PPUTLIMPL_UDEC_2521u 0x9D9u, 0, 0, 0, 0, 11, 50,
#define PPUTLIMPL_UDEC_2520u 0x9D8u, 0, 0, 0, 0, 11, 50, 2, 2, 2, 3, 3, 5, 7
#define PPUTLIMPL_UDEC_2519u 0x9D7u, 0, 0, 0, 0, 11, 50, 11, 229
#define PPUTLIMPL_UDEC_2518u 0x9D6u, 0, 0, 0, 0, 11, 50, 2, 1259
#define PPUTLIMPL_UDEC_2517u 0x9D5u, 0, 0, 0, 0, 11, 50, 3, 839
#define PPUTLIMPL_UDEC_2516u 0x9D4u, 0, 0, 0, 0, 11, 50, 2, 2, 17, 37
#define PPUTLIMPL_UDEC_2515u 0x9D3u, 0, 0, 0, 0, 11, 50, 5, 503
#define PPUTLIMPL_UDEC_2514u 0x9D2u, 0, 0, 0, 0, 11, 50, 2, 3, 419
#define PPUTLIMPL_UDEC_2513u 0x9D1u, 0, 0, 0, 0, 11, 50, 7, 359
#define PPUTLIMPL_UDEC_2512u 0x9D0u, 0, 0, 0, 0, 11, 50, 2, 2, 2, 2, 157
#define PPUTLIMPL_UDEC_2511u 0x9CFu, 0, 0, 0, 0, 11, 50, 3, 3, 3, 3, 31
#define PPUTLIMPL_UDEC_2510u 0x9CEu, 0, 0, 0, 0, 11, 50, 2, 5, 251
#define PPUTLIMPL_UDEC_2509u 0x9CDu, 0, 0, 0, 0, 11, 50, 13, 193
#define PPUTLIMPL_UDEC_2508u 0x9CCu, 0, 0, 0, 0, 11, 50, 2, 2, 3, 11, 19
#define PPUTLIMPL_UDEC_2507u 0x9CBu, 0, 0, 0, 0, 11, 50, 23, 109
#define PPUTLIMPL_UDEC_2506u 0x9CAu, 0, 0, 0, 0, 11, 50, 2, 7, 179
#define PPUTLIMPL_UDEC_2505u 0x9C9u, 0, 0, 0, 0, 11, 50, 3, 5, 167
#define PPUTLIMPL_UDEC_2504u 0x9C8u, 0, 0, 0, 0, 11, 50, 2, 2, 2, 313
#define PPUTLIMPL_UDEC_2503u 0x9C7u, 0, 0, 0, 0, 11, 50,
#define PPUTLIMPL_UDEC_2502u 0x9C6u, 0, 0, 0, 0, 11, 50, 2, 3, 3, 139
#define PPUTLIMPL_UDEC_2501u 0x9C5u, 0, 0, 0, 0, 11, 50, 41, 61
#define PPUTLIMPL_UDEC_2500u 0x9C4u, 0, 0, 0, 0, 11, 50, 2, 2, 5, 5, 5, 5
#define PPUTLIMPL_UDEC_2499u 0x9C3u, 0, 0, 0, 0, 11, 49, 3, 7, 7, 17
#define PPUTLIMPL_UDEC_2498u 0x9C2u, 0, 0, 0, 0, 11, 49, 2, 1249
#define PPUTLIMPL_UDEC_2497u 0x9C1u, 0, 0, 0, 0, 11, 49, 11, 227
#define PPUTLIMPL_UDEC_2496u 0x9C0u, 0, 0, 0, 0, 11, 49, 2, 2, 2, 2, 2, 2, 3, 13
#define PPUTLIMPL_UDEC_2495u 0x9BFu, 0, 0, 0, 0, 11, 49, 5, 499
#define PPUTLIMPL_UDEC_2494u 0x9BEu, 0, 0, 0, 0, 11, 49, 2, 29, 43
#define PPUTLIMPL_UDEC_2493u 0x9BDu, 0, 0, 0, 0, 11, 49, 3, 3, 277
#define PPUTLIMPL_UDEC_2492u 0x9BCu, 0, 0, 0, 0, 11, 49, 2, 2, 7, 89
#define PPUTLIMPL_UDEC_2491u 0x9BBu, 0, 0, 0, 0, 11, 49, 47, 53
#define PPUTLIMPL_UDEC_2490u 0x9BAu, 0, 0, 0, 0, 11, 49, 2, 3, 5, 83
#define PPUTLIMPL_UDEC_2489u 0x9B9u, 0, 0, 0, 0, 11, 49, 19, 131
#define PPUTLIMPL_UDEC_2488u 0x9B8u, 0, 0, 0, 0, 11, 49, 2, 2, 2, 311
#define PPUTLIMPL_UDEC_2487u 0x9B7u, 0, 0, 0, 0, 11, 49, 3, 829
#define PPUTLIMPL_UDEC_2486u 0x9B6u, 0, 0, 0, 0, 11, 49, 2, 11, 113
#define PPUTLIMPL_UDEC_2485u 0x9B5u, 0, 0, 0, 0, 11, 49, 5, 7, 71
#define PPUTLIMPL_UDEC_2484u 0x9B4u, 0, 0, 0, 0, 11, 49, 2, 2, 3, 3, 3, 23
#define PPUTLIMPL_UDEC_2483u 0x9B3u, 0, 0, 0, 0, 11, 49, 13, 191
#define PPUTLIMPL_UDEC_2482u 0x9B2u, 0, 0, 0, 0, 11, 49, 2, 17, 73
#define PPUTLIMPL_UDEC_2481u 0x9B1u, 0, 0, 0, 0, 11, 49, 3, 827
#define PPUTLIMPL_UDEC_2480u 0x9B0u, 0, 0, 0, 0, 11, 49, 2, 2, 2, 2, 5, 31
#define PPUTLIMPL_UDEC_2479u 0x9AFu, 0, 0, 0, 0, 11, 49, 37, 67
#define PPUTLIMPL_UDEC_2478u 0x9AEu, 0, 0, 0, 0, 11, 49, 2, 3, 7, 59
#define PPUTLIMPL_UDEC_2477u 0x9ADu, 0, 0, 0, 0, 11, 49,
#define PPUTLIMPL_UDEC_2476u 0x9ACu, 0, 0, 0, 0, 11, 49, 2, 2, 619
#define PPUTLIMPL_UDEC_2475u 0x9ABu, 0, 0, 0, 0, 11, 49, 3, 3, 5, 5, 11
#define PPUTLIMPL_UDEC_2474u 0x9AAu, 0, 0, 0, 0, 11, 49, 2, 1237
#define PPUTLIMPL_UDEC_2473u 0x9A9u, 0, 0, 0, 0, 11, 49,
#define PPUTLIMPL_UDEC_2472u 0x9A8u, 0, 0, 0, 0, 11, 49, 2, 2, 2, 3, 103
#define PPUTLIMPL_UDEC_2471u 0x9A7u, 0, 0, 0, 0, 11, 49, 7, 353
#define PPUTLIMPL_UDEC_2470u 0x9A6u, 0, 0, 0, 0, 11, 49, 2, 5, 13, 19
#define PPUTLIMPL_UDEC_2469u 0x9A5u, 0, 0, 0, 0, 11, 49, 3, 823
#define PPUTLIMPL_UDEC_2468u 0x9A4u, 0, 0, 0, 0, 11, 49, 2, 2, 617
#define PPUTLIMPL_UDEC_2467u 0x9A3u, 0, 0, 0, 0, 11, 49,
#define PPUTLIMPL_UDEC_2466u 0x9A2u, 0, 0, 0, 0, 11, 49, 2, 3, 3, 137
#define PPUTLIMPL_UDEC_2465u 0x9A1u, 0, 0, 0, 0, 11, 49, 5, 17, 29
#define PPUTLIMPL_UDEC_2464u 0x9A0u, 0, 0, 0, 0, 11, 49, 2, 2, 2, 2, 2, 7, 11
#define PPUTLIMPL_UDEC_2463u 0x99Fu, 0, 0, 0, 0, 11, 49, 3, 821
#define PPUTLIMPL_UDEC_2462u 0x99Eu, 0, 0, 0, 0, 11, 49, 2, 1231
#define PPUTLIMPL_UDEC_2461u 0x99Du, 0, 0, 0, 0, 11, 49, 23, 107
#define PPUTLIMPL_UDEC_2460u 0x99Cu, 0, 0, 0, 0, 11, 49, 2, 2, 3, 5, 41
#define PPUTLIMPL_UDEC_2459u 0x99Bu, 0, 0, 0, 0, 11, 49,
#define PPUTLIMPL_UDEC_2458u 0x99Au, 0, 0, 0, 0, 11, 49, 2, 1229
#define PPUTLIMPL_UDEC_2457u 0x999u, 0, 0, 0, 0, 11, 49, 3, 3, 3, 7, 13
#define PPUTLIMPL_UDEC_2456u 0x998u, 0, 0, 0, 0, 11, 49, 2, 2, 2, 307
#define PPUTLIMPL_UDEC_2455u 0x997u, 0, 0, 0, 0, 11, 49, 5, 491
#define PPUTLIMPL_UDEC_2454u 0x996u, 0, 0, 0, 0, 11, 49, 2, 3, 409
#define PPUTLIMPL_UDEC_2453u 0x995u, 0, 0, 0, 0, 11, 49, 11, 223
#define PPUTLIMPL_UDEC_2452u 0x994u, 0, 0, 0, 0, 11, 49, 2, 2, 613
#define PPUTLIMPL_UDEC_2451u 0x993u, 0, 0, 0, 0, 11, 49, 3, 19, 43
#define PPUTLIMPL_UDEC_2450u 0x992u, 0, 0, 0, 0, 11, 49, 2, 5, 5, 7, 7
#define PPUTLIMPL_UDEC_2449u 0x991u, 0, 0, 0, 0, 11, 49, 31, 79
#define PPUTLIMPL_UDEC_2448u 0x990u, 0, 0, 0, 0, 11, 49, 2, 2, 2, 2, 3, 3, 17
#define PPUTLIMPL_UDEC_2447u 0x98Fu, 0, 0, 0, 0, 11, 49,
#define PPUTLIMPL_UDEC_2446u 0x98Eu, 0, 0, 0, 0, 11, 49, 2, 1223
#define PPUTLIMPL_UDEC_2445u 0x98Du, 0, 0, 0, 0, 11, 49, 3, 5, 163
#define PPUTLIMPL_UDEC_2444u 0x98Cu, 0, 0, 0, 0, 11, 49, 2, 2, 13, 47
#define PPUTLIMPL_UDEC_2443u 0x98Bu, 0, 0, 0, 0, 11, 49, 7, 349
#define PPUTLIMPL_UDEC_2442u 0x98Au, 0, 0, 0, 0, 11, 49, 2, 3, 11, 37
#define PPUTLIMPL_UDEC_2441u 0x989u, 0, 0, 0, 0, 11, 49,
#define PPUTLIMPL_UDEC_2440u 0x988u, 0, 0, 0, 0, 11, 49, 2, 2, 2, 5, 61
#define PPUTLIMPL_UDEC_2439u 0x987u, 0, 0, 0, 0, 11, 49, 3, 3, 271
#define PPUTLIMPL_UDEC_2438u 0x986u, 0, 0, 0, 0, 11, 49, 2, 23, 53
#define PPUTLIMPL_UDEC_2437u 0x985u, 0, 0, 0, 0, 11, 49,
#define PPUTLIMPL_UDEC_2436u 0x984u, 0, 0, 0, 0, 11, 49, 2, 2, 3, 7, 29
#define PPUTLIMPL_UDEC_2435u 0x983u, 0, 0, 0, 0, 11, 49, 5, 487
#define PPUTLIMPL_UDEC_2434u 0x982u, 0, 0, 0, 0, 11, 49, 2, 1217
#define PPUTLIMPL_UDEC_2433u 0x981u, 0, 0, 0, 0, 11, 49, 3, 811
#define PPUTLIMPL_UDEC_2432u 0x980u, 0, 0, 0, 0, 11, 49, 2, 2, 2, 2, 2, 2, 2, 19
#define PPUTLIMPL_UDEC_2431u 0x97Fu, 0, 0, 0, 0, 11, 49, 11, 13, 17
#define PPUTLIMPL_UDEC_2430u 0x97Eu, 0, 0, 0, 0, 11, 49, 2, 3, 3, 3, 3, 3, 5
#define PPUTLIMPL_UDEC_2429u 0x97Du, 0, 0, 0, 0, 11, 49, 7, 347
#define PPUTLIMPL_UDEC_2428u 0x97Cu, 0, 0, 0, 0, 11, 49, 2, 2, 607
#define PPUTLIMPL_UDEC_2427u 0x97Bu, 0, 0, 0, 0, 11, 49, 3, 809
#define PPUTLIMPL_UDEC_2426u 0x97Au, 0, 0, 0, 0, 11, 49, 2, 1213
#define PPUTLIMPL_UDEC_2425u 0x979u, 0, 0, 0, 0, 11, 49, 5, 5, 97
#define PPUTLIMPL_UDEC_2424u 0x978u, 0, 0, 0, 0, 11, 49, 2, 2, 2, 3, 101
#define PPUTLIMPL_UDEC_2423u 0x977u, 0, 0, 0, 0, 11, 49,
#define PPUTLIMPL_UDEC_2422u 0x976u, 0, 0, 0, 0, 11, 49, 2, 7, 173
#define PPUTLIMPL_UDEC_2421u 0x975u, 0, 0, 0, 0, 11, 49, 3, 3, 269
#define PPUTLIMPL_UDEC_2420u 0x974u, 0, 0, 0, 0, 11, 49, 2, 2, 5, 11, 11
#define PPUTLIMPL_UDEC_2419u 0x973u, 0, 0, 0, 0, 11, 49, 41, 59
#define PPUTLIMPL_UDEC_2418u 0x972u, 0, 0, 0, 0, 11, 49, 2, 3, 13, 31
#define PPUTLIMPL_UDEC_2417u 0x971u, 0, 0, 0, 0, 11, 49,
#define PPUTLIMPL_UDEC_2416u 0x970u, 0, 0, 0, 0, 11, 49, 2, 2, 2, 2, 151
#define PPUTLIMPL_UDEC_2415u 0x96Fu, 0, 0, 0, 0, 11, 49, 3, 5, 7, 23
#define PPUTLIMPL_UDEC_2414u 0x96Eu, 0, 0, 0, 0, 11, 49, 2, 17, 71
#define PPUTLIMPL_UDEC_2413u 0x96Du, 0, 0, 0, 0, 11, 49, 19, 127
#define PPUTLIMPL_UDEC_2412u 0x96Cu, 0, 0, 0, 0, 11, 49, 2, 2, 3, 3, 67
#define PPUTLIMPL_UDEC_2411u 0x96Bu, 0, 0, 0, 0, 11, 49,
#define PPUTLIMPL_UDEC_2410u 0x96Au, 0, 0, 0, 0, 11, 49, 2, 5, 241
#define PPUTLIMPL_UDEC_2409u 0x969u, 0, 0, 0, 0, 11, 49, 3, 11, 73
#define PPUTLIMPL_UDEC_2408u 0x968u, 0, 0, 0, 0, 11, 49, 2, 2, 2, 7, 43
#define PPUTLIMPL_UDEC_2407u 0x967u, 0, 0, 0, 0, 11, 49, 29, 83
#define PPUTLIMPL_UDEC_2406u 0x966u, 0, 0, 0, 0, 11, 49, 2, 3, 401
#define PPUTLIMPL_UDEC_2405u 0x965u, 0, 0, 0, 0, 11, 49, 5, 13, 37
#define PPUTLIMPL_UDEC_2404u 0x964u, 0, 0, 0, 0, 11, 49, 2, 2, 601
#define PPUTLIMPL_UDEC_2403u 0x963u, 0, 0, 0, 0, 11, 49, 3, 3, 3, 89
#define PPUTLIMPL_UDEC_2402u 0x962u, 0, 0, 0, 0, 11, 49, 2, 1201
#define PPUTLIMPL_UDEC_2401u 0x961u, 0, 0, 0, 0, 11, 49, 7, 7, 7, 7
#define PPUTLIMPL_UDEC_2400u 0x960u, 0, 0, 0, 0, 11, 48, 2, 2, 2, 2, 2, 3, 5, 5
#define PPUTLIMPL_UDEC_2399u 0x95Fu, 0, 0, 0, 0, 11, 48,
#define PPUTLIMPL_UDEC_2398u 0x95Eu, 0, 0, 0, 0, 11, 48, 2, 11, 109
#define PPUTLIMPL_UDEC_2397u 0x95Du, 0, 0, 0, 0, 11, 48, 3, 17, 47
#define PPUTLIMPL_UDEC_2396u 0x95Cu, 0, 0, 0, 0, 11, 48, 2, 2, 599
#define PPUTLIMPL_UDEC_2395u 0x95Bu, 0, 0, 0, 0, 11, 48, 5, 479
#define PPUTLIMPL_UDEC_2394u 0x95Au, 0, 0, 0, 0, 11, 48, 2, 3, 3, 7, 19
#define PPUTLIMPL_UDEC_2393u 0x959u, 0, 0, 0, 0, 11, 48,
#define PPUTLIMPL_UDEC_2392u 0x958u, 0, 0, 0, 0, 11, 48, 2, 2, 2, 13, 23
#define PPUTLIMPL_UDEC_2391u 0x957u, 0, 0, 0, 0, 11, 48, 3, 797
#define PPUTLIMPL_UDEC_2390u 0x956u, 0, 0, 0, 0, 11, 48, 2, 5, 239
#define PPUTLIMPL_UDEC_2389u 0x955u, 0, 0, 0, 0, 11, 48,
#define PPUTLIMPL_UDEC_2388u 0x954u, 0, 0, 0, 0, 11, 48, 2, 2, 3, 199
#define PPUTLIMPL_UDEC_2387u 0x953u, 0, 0, 0, 0, 11, 48, 7, 11, 31
#define PPUTLIMPL_UDEC_2386u 0x952u, 0, 0, 0, 0, 11, 48, 2, 1193
#define PPUTLIMPL_UDEC_2385u 0x951u, 0, 0, 0, 0, 11, 48, 3, 3, 5, 53
#define PPUTLIMPL_UDEC_2384u 0x950u, 0, 0, 0, 0, 11, 48, 2, 2, 2, 2, 149
#define PPUTLIMPL_UDEC_2383u 0x94Fu, 0, 0, 0, 0, 11, 48,
#define PPUTLIMPL_UDEC_2382u 0x94Eu, 0, 0, 0, 0, 11, 48, 2, 3, 397
#define PPUTLIMPL_UDEC_2381u 0x94Du, 0, 0, 0, 0, 11, 48,
#define PPUTLIMPL_UDEC_2380u 0x94Cu, 0, 0, 0, 0, 11, 48, 2, 2, 5, 7, 17
#define PPUTLIMPL_UDEC_2379u 0x94Bu, 0, 0, 0, 0, 11, 48, 3, 13, 61
#define PPUTLIMPL_UDEC_2378u 0x94Au, 0, 0, 0, 0, 11, 48, 2, 29, 41
#define PPUTLIMPL_UDEC_2377u 0x949u, 0, 0, 0, 0, 11, 48,
#define PPUTLIMPL_UDEC_2376u 0x948u, 0, 0, 0, 0, 11, 48, 2, 2, 2, 3, 3, 3, 11
#define PPUTLIMPL_UDEC_2375u 0x947u, 0, 0, 0, 0, 11, 48, 5, 5, 5, 19
#define PPUTLIMPL_UDEC_2374u 0x946u, 0, 0, 0, 0, 11, 48, 2, 1187
#define PPUTLIMPL_UDEC_2373u 0x945u, 0, 0, 0, 0, 11, 48, 3, 7, 113
#define PPUTLIMPL_UDEC_2372u 0x944u, 0, 0, 0, 0, 11, 48, 2, 2, 593
#define PPUTLIMPL_UDEC_2371u 0x943u, 0, 0, 0, 0, 11, 48,
#define PPUTLIMPL_UDEC_2370u 0x942u, 0, 0, 0, 0, 11, 48, 2, 3, 5, 79
#define PPUTLIMPL_UDEC_2369u 0x941u, 0, 0, 0, 0, 11, 48, 23, 103
#define PPUTLIMPL_UDEC_2368u 0x940u, 0, 0, 0, 0, 11, 48, 2, 2, 2, 2, 2, 2, 37
#define PPUTLIMPL_UDEC_2367u 0x93Fu, 0, 0, 0, 0, 11, 48, 3, 3, 263
#define PPUTLIMPL_UDEC_2366u 0x93Eu, 0, 0, 0, 0, 11, 48, 2, 7, 13, 13
#define PPUTLIMPL_UDEC_2365u 0x93Du, 0, 0, 0, 0, 11, 48, 5, 11, 43
#define PPUTLIMPL_UDEC_2364u 0x93Cu, 0, 0, 0, 0, 11, 48, 2, 2, 3, 197
#define PPUTLIMPL_UDEC_2363u 0x93Bu, 0, 0, 0, 0, 11, 48, 17, 139
#define PPUTLIMPL_UDEC_2362u 0x93Au, 0, 0, 0, 0, 11, 48, 2, 1181
#define PPUTLIMPL_UDEC_2361u 0x939u, 0, 0, 0, 0, 11, 48, 3, 787
#define PPUTLIMPL_UDEC_2360u 0x938u, 0, 0, 0, 0, 11, 48, 2, 2, 2, 5, 59
#define PPUTLIMPL_UDEC_2359u 0x937u, 0, 0, 0, 0, 11, 48, 7, 337
#define PPUTLIMPL_UDEC_2358u 0x936u, 0, 0, 0, 0, 11, 48, 2, 3, 3, 131
#define PPUTLIMPL_UDEC_2357u 0x935u, 0, 0, 0, 0, 11, 48,
#define PPUTLIMPL_UDEC_2356u 0x934u, 0, 0, 0, 0, 11, 48, 2, 2, 19, 31
#define PPUTLIMPL_UDEC_2355u 0x933u, 0, 0, 0, 0, 11, 48, 3, 5, 157
#define PPUTLIMPL_UDEC_2354u 0x932u, 0, 0, 0, 0, 11, 48, 2, 11, 107
#define PPUTLIMPL_UDEC_2353u 0x931u, 0, 0, 0, 0, 11, 48, 13, 181
#define PPUTLIMPL_UDEC_2352u 0x930u, 0, 0, 0, 0, 11, 48, 2, 2, 2, 2, 3, 7, 7
#define PPUTLIMPL_UDEC_2351u 0x92Fu, 0, 0, 0, 0, 11, 48,
#define PPUTLIMPL_UDEC_2350u 0x92Eu, 0, 0, 0, 0, 11, 48, 2, 5, 5, 47
#define PPUTLIMPL_UDEC_2349u 0x92Du, 0, 0, 0, 0, 11, 48, 3, 3, 3, 3, 29
#define PPUTLIMPL_UDEC_2348u 0x92Cu, 0, 0, 0, 0, 11, 48, 2, 2, 587
#define PPUTLIMPL_UDEC_2347u 0x92Bu, 0, 0, 0, 0, 11, 48,
#define PPUTLIMPL_UDEC_2346u 0x92Au, 0, 0, 0, 0, 11, 48, 2, 3, 17, 23
#define PPUTLIMPL_UDEC_2345u 0x929u, 0, 0, 0, 0, 11, 48, 5, 7, 67
#define PPUTLIMPL_UDEC_2344u 0x928u, 0, 0, 0, 0, 11, 48, 2, 2, 2, 293
#define PPUTLIMPL_UDEC_2343u 0x927u, 0, 0, 0, 0, 11, 48, 3, 11, 71
#define PPUTLIMPL_UDEC_2342u 0x926u, 0, 0, 0, 0, 11, 48, 2, 1171
#define PPUTLIMPL_UDEC_2341u 0x925u, 0, 0, 0, 0, 11, 48,
#define PPUTLIMPL_UDEC_2340u 0x924u, 0, 0, 0, 0, 11, 48, 2, 2, 3, 3, 5, 13
#define PPUTLIMPL_UDEC_2339u 0x923u, 0, 0, 0, 0, 11, 48,
#define PPUTLIMPL_UDEC_2338u 0x922u, 0, 0, 0, 0, 11, 48, 2, 7, 167
#define PPUTLIMPL_UDEC_2337u 0x921u, 0, 0, 0, 0, 11, 48, 3, 19, 41
#define PPUTLIMPL_UDEC_2336u 0x920u, 0, 0, 0, 0, 11, 48, 2, 2, 2, 2, 2, 73
#define PPUTLIMPL_UDEC_2335u 0x91Fu, 0, 0, 0, 0, 11, 48, 5, 467
#define PPUTLIMPL_UDEC_2334u 0x91Eu, 0, 0, 0, 0, 11, 48, 2, 3, 389
#define PPUTLIMPL_UDEC_2333u 0x91Du, 0, 0, 0, 0, 11, 48,
#define PPUTLIMPL_UDEC_2332u 0x91Cu, 0, 0, 0, 0, 11, 48, 2, 2, 11, 53
#define PPUTLIMPL_UDEC_2331u 0x91Bu, 0, 0, 0, 0, 11, 48, 3, 3, 7, 37
#define PPUTLIMPL_UDEC_2330u 0x91Au, 0, 0, 0, 0, 11, 48, 2, 5, 233
#define PPUTLIMPL_UDEC_2329u 0x919u, 0, 0, 0, 0, 11, 48, 17, 137
#define PPUTLIMPL_UDEC_2328u 0x918u, 0, 0, 0, 0, 11, 48, 2, 2, 2, 3, 97
#define PPUTLIMPL_UDEC_2327u 0x917u, 0, 0, 0, 0, 11, 48, 13, 179
#define PPUTLIMPL_UDEC_2326u 0x916u, 0, 0, 0, 0, 11, 48, 2, 1163
#define PPUTLIMPL_UDEC_2325u 0x915u, 0, 0, 0, 0, 11, 48, 3, 5, 5, 31
#define PPUTLIMPL_UDEC_2324u 0x914u, 0, 0, 0, 0, 11, 48, 2, 2, 7, 83
#define PPUTLIMPL_UDEC_2323u 0x913u, 0, 0, 0, 0, 11, 48, 23, 101
#define PPUTLIMPL_UDEC_2322u 0x912u, 0, 0, 0, 0, 11, 48, 2, 3, 3, 3, 43
#define PPUTLIMPL_UDEC_2321u 0x911u, 0, 0, 0, 0, 11, 48, 11, 211
#define PPUTLIMPL_UDEC_2320u 0x910u, 0, 0, 0, 0, 11, 48, 2, 2, 2, 2, 5, 29
#define PPUTLIMPL_UDEC_2319u 0x90Fu, 0, 0, 0, 0, 11, 48, 3, 773
#define PPUTLIMPL_UDEC_2318u 0x90Eu, 0, 0, 0, 0, 11, 48, 2, 19, 61
#define PPUTLIMPL_UDEC_2317u 0x90Du, 0, 0, 0, 0, 11, 48, 7, 331
#define PPUTLIMPL_UDEC_2316u 0x90Cu, 0, 0, 0, 0, 11, 48, 2, 2, 3, 193
#define PPUTLIMPL_UDEC_2315u 0x90Bu, 0, 0, 0, 0, 11, 48, 5, 463
#define PPUTLIMPL_UDEC_2314u 0x90Au, 0, 0, 0, 0, 11, 48, 2, 13, 89
#define PPUTLIMPL_UDEC_2313u 0x909u, 0, 0, 0, 0, 11, 48, 3, 3, 257
#define PPUTLIMPL_UDEC_2312u 0x908u, 0, 0, 0, 0, 11, 48, 2, 2, 2, 17, 17
#define PPUTLIMPL_UDEC_2311u 0x907u, 0, 0, 0, 0, 11, 48,
#define PPUTLIMPL_UDEC_2310u 0x906u, 0, 0, 0, 0, 11, 48, 2, 3, 5, 7, 11
#define PPUTLIMPL_UDEC_2309u 0x905u, 0, 0, 0, 0, 11, 48,
#define PPUTLIMPL_UDEC_2308u 0x904u, 0, 0, 0, 0, 11, 48, 2, 2, 577
#define PPUTLIMPL_UDEC_2307u 0x903u, 0, 0, 0, 0, 11, 48, 3, 769
#define PPUTLIMPL_UDEC_2306u 0x902u, 0, 0, 0, 0, 11, 48, 2, 1153
#define PPUTLIMPL_UDEC_2305u 0x901u, 0, 0, 0, 0, 11, 48, 5, 461
#define PPUTLIMPL_UDEC_2304u 0x900u, 0, 0, 0, 0, 11, 48, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3
#define PPUTLIMPL_UDEC_2303u 0x8FFu, 0, 0, 0, 0, 11, 47, 7, 7, 47
#define PPUTLIMPL_UDEC_2302u 0x8FEu, 0, 0, 0, 0, 11, 47, 2, 1151
#define PPUTLIMPL_UDEC_2301u 0x8FDu, 0, 0, 0, 0, 11, 47, 3, 13, 59
#define PPUTLIMPL_UDEC_2300u 0x8FCu, 0, 0, 0, 0, 11, 47, 2, 2, 5, 5, 23
#define PPUTLIMPL_UDEC_2299u 0x8FBu, 0, 0, 0, 0, 11, 47, 11, 11, 19
#define PPUTLIMPL_UDEC_2298u 0x8FAu, 0, 0, 0, 0, 11, 47, 2, 3, 383
#define PPUTLIMPL_UDEC_2297u 0x8F9u, 0, 0, 0, 0, 11, 47,
#define PPUTLIMPL_UDEC_2296u 0x8F8u, 0, 0, 0, 0, 11, 47, 2, 2, 2, 7, 41
#define PPUTLIMPL_UDEC_2295u 0x8F7u, 0, 0, 0, 0, 11, 47, 3, 3, 3, 5, 17
#define PPUTLIMPL_UDEC_2294u 0x8F6u, 0, 0, 0, 0, 11, 47, 2, 31, 37
#define PPUTLIMPL_UDEC_2293u 0x8F5u, 0, 0, 0, 0, 11, 47,
#define PPUTLIMPL_UDEC_2292u 0x8F4u, 0, 0, 0, 0, 11, 47, 2, 2, 3, 191
#define PPUTLIMPL_UDEC_2291u 0x8F3u, 0, 0, 0, 0, 11, 47, 29, 79
#define PPUTLIMPL_UDEC_2290u 0x8F2u, 0, 0, 0, 0, 11, 47, 2, 5, 229
#define PPUTLIMPL_UDEC_2289u 0x8F1u, 0, 0, 0, 0, 11, 47, 3, 7, 109
#define PPUTLIMPL_UDEC_2288u 0x8F0u, 0, 0, 0, 0, 11, 47, 2, 2, 2, 2, 11, 13
#define PPUTLIMPL_UDEC_2287u 0x8EFu, 0, 0, 0, 0, 11, 47,
#define PPUTLIMPL_UDEC_2286u 0x8EEu, 0, 0, 0, 0, 11, 47, 2, 3, 3, 127
#define PPUTLIMPL_UDEC_2285u 0x8EDu, 0, 0, 0, 0, 11, 47, 5, 457
#define PPUTLIMPL_UDEC_2284u 0x8ECu, 0, 0, 0, 0, 11, 47, 2, 2, 571
#define PPUTLIMPL_UDEC_2283u 0x8EBu, 0, 0, 0, 0, 11, 47, 3, 761
#define PPUTLIMPL_UDEC_2282u 0x8EAu, 0, 0, 0, 0, 11, 47, 2, 7, 163
#define PPUTLIMPL_UDEC_2281u 0x8E9u, 0, 0, 0, 0, 11, 47,
#define PPUTLIMPL_UDEC_2280u 0x8E8u, 0, 0, 0, 0, 11, 47, 2, 2, 2, 3, 5, 19
#define PPUTLIMPL_UDEC_2279u 0x8E7u, 0, 0, 0, 0, 11, 47, 43, 53
#define PPUTLIMPL_UDEC_2278u 0x8E6u, 0, 0, 0, 0, 11, 47, 2, 17, 67
#define PPUTLIMPL_UDEC_2277u 0x8E5u, 0, 0, 0, 0, 11, 47, 3, 3, 11, 23
#define PPUTLIMPL_UDEC_2276u 0x8E4u, 0, 0, 0, 0, 11, 47, 2, 2, 569
#define PPUTLIMPL_UDEC_2275u 0x8E3u, 0, 0, 0, 0, 11, 47, 5, 5, 7, 13
#define PPUTLIMPL_UDEC_2274u 0x8E2u, 0, 0, 0, 0, 11, 47, 2, 3, 379
#define PPUTLIMPL_UDEC_2273u 0x8E1u, 0, 0, 0, 0, 11, 47,
#define PPUTLIMPL_UDEC_2272u 0x8E0u, 0, 0, 0, 0, 11, 47, 2, 2, 2, 2, 2, 71
#define PPUTLIMPL_UDEC_2271u 0x8DFu, 0, 0, 0, 0, 11, 47, 3, 757
#define PPUTLIMPL_UDEC_2270u 0x8DEu, 0, 0, 0, 0, 11, 47, 2, 5, 227
#define PPUTLIMPL_UDEC_2269u 0x8DDu, 0, 0, 0, 0, 11, 47,
#define PPUTLIMPL_UDEC_2268u 0x8DCu, 0, 0, 0, 0, 11, 47, 2, 2, 3, 3, 3, 3, 7
#define PPUTLIMPL_UDEC_2267u 0x8DBu, 0, 0, 0, 0, 11, 47,
#define PPUTLIMPL_UDEC_2266u 0x8DAu, 0, 0, 0, 0, 11, 47, 2, 11, 103
#define PPUTLIMPL_UDEC_2265u 0x8D9u, 0, 0, 0, 0, 11, 47, 3, 5, 151
#define PPUTLIMPL_UDEC_2264u 0x8D8u, 0, 0, 0, 0, 11, 47, 2, 2, 2, 283
#define PPUTLIMPL_UDEC_2263u 0x8D7u, 0, 0, 0, 0, 11, 47, 31, 73
#define PPUTLIMPL_UDEC_2262u 0x8D6u, 0, 0, 0, 0, 11, 47, 2, 3, 13, 29
#define PPUTLIMPL_UDEC_2261u 0x8D5u, 0, 0, 0, 0, 11, 47, 7, 17, 19
#define PPUTLIMPL_UDEC_2260u 0x8D4u, 0, 0, 0, 0, 11, 47, 2, 2, 5, 113
#define PPUTLIMPL_UDEC_2259u 0x8D3u, 0, 0, 0, 0, 11, 47, 3, 3, 251
#define PPUTLIMPL_UDEC_2258u 0x8D2u, 0, 0, 0, 0, 11, 47, 2, 1129
#define PPUTLIMPL_UDEC_2257u 0x8D1u, 0, 0, 0, 0, 11, 47, 37, 61
#define PPUTLIMPL_UDEC_2256u 0x8D0u, 0, 0, 0, 0, 11, 47, 2, 2, 2, 2, 3, 47
#define PPUTLIMPL_UDEC_2255u 0x8CFu, 0, 0, 0, 0, 11, 47, 5, 11, 41
#define PPUTLIMPL_UDEC_2254u 0x8CEu, 0, 0, 0, 0, 11, 47, 2, 7, 7, 23
#define PPUTLIMPL_UDEC_2253u 0x8CDu, 0, 0, 0, 0, 11, 47, 3, 751
#define PPUTLIMPL_UDEC_2252u 0x8CCu, 0, 0, 0, 0, 11, 47, 2, 2, 563
#define PPUTLIMPL_UDEC_2251u 0x8CBu, 0, 0, 0, 0, 11, 47,
#define PPUTLIMPL_UDEC_2250u 0x8CAu, 0, 0, 0, 0, 11, 47, 2, 3, 3, 5, 5, 5
#define PPUTLIMPL_UDEC_2249u 0x8C9u, 0, 0, 0, 0, 11, 47, 13, 173
#define PPUTLIMPL_UDEC_2248u 0x8C8u, 0, 0, 0, 0, 11, 47, 2, 2, 2, 281
#define PPUTLIMPL_UDEC_2247u 0x8C7u, 0, 0, 0, 0, 11, 47, 3, 7, 107
#define PPUTLIMPL_UDEC_2246u 0x8C6u, 0, 0, 0, 0, 11, 47, 2, 1123
#define PPUTLIMPL_UDEC_2245u 0x8C5u, 0, 0, 0, 0, 11, 47, 5, 449
#define PPUTLIMPL_UDEC_2244u 0x8C4u, 0, 0, 0, 0, 11, 47, 2, 2, 3, 11, 17
#define PPUTLIMPL_UDEC_2243u 0x8C3u, 0, 0, 0, 0, 11, 47,
#define PPUTLIMPL_UDEC_2242u 0x8C2u, 0, 0, 0, 0, 11, 47, 2, 19, 59
#define PPUTLIMPL_UDEC_2241u 0x8C1u, 0, 0, 0, 0, 11, 47, 3, 3, 3, 83
#define PPUTLIMPL_UDEC_2240u 0x8C0u, 0, 0, 0, 0, 11, 47, 2, 2, 2, 2, 2, 2, 5, 7
#define PPUTLIMPL_UDEC_2239u 0x8BFu, 0, 0, 0, 0, 11, 47,
#define PPUTLIMPL_UDEC_2238u 0x8BEu, 0, 0, 0, 0, 11, 47, 2, 3, 373
#define PPUTLIMPL_UDEC_2237u 0x8BDu, 0, 0, 0, 0, 11, 47,
#define PPUTLIMPL_UDEC_2236u 0x8BCu, 0, 0, 0, 0, 11, 47, 2, 2, 13, 43
#define PPUTLIMPL_UDEC_2235u 0x8BBu, 0, 0, 0, 0, 11, 47, 3, 5, 149
#define PPUTLIMPL_UDEC_2234u 0x8BAu, 0, 0, 0, 0, 11, 47, 2, 1117
#define PPUTLIMPL_UDEC_2233u 0x8B9u, 0, 0, 0, 0, 11, 47, 7, 11, 29
#define PPUTLIMPL_UDEC_2232u 0x8B8u, 0, 0, 0, 0, 11, 47, 2, 2, 2, 3, 3, 31
#define PPUTLIMPL_UDEC_2231u 0x8B7u, 0, 0, 0, 0, 11, 47, 23, 97
#define PPUTLIMPL_UDEC_2230u 0x8B6u, 0, 0, 0, 0, 11, 47, 2, 5, 223
#define PPUTLIMPL_UDEC_2229u 0x8B5u, 0, 0, 0, 0, 11, 47, 3, 743
#define PPUTLIMPL_UDEC_2228u 0x8B4u, 0, 0, 0, 0, 11, 47, 2, 2, 557
#define PPUTLIMPL_UDEC_2227u 0x8B3u, 0, 0, 0, 0, 11, 47, 17, 131
#define PPUTLIMPL_UDEC_2226u 0x8B2u, 0, 0, 0, 0, 11, 47, 2, 3, 7, 53
#define PPUTLIMPL_UDEC_2225u 0x8B1u, 0, 0, 0, 0, 11, 47, 5, 5, 89
#define PPUTLIMPL_UDEC_2224u 0x8B0u, 0, 0, 0, 0, 11, 47, 2, 2, 2, 2, 139
#define PPUTLIMPL_UDEC_2223u 0x8AFu, 0, 0, 0, 0, 11, 47, 3, 3, 13, 19
#define PPUTLIMPL_UDEC_2222u 0x8AEu, 0, 0, 0, 0, 11, 47, 2, 11, 101
#define PPUTLIMPL_UDEC_2221u 0x8ADu, 0, 0, 0, 0, 11, 47,
#define PPUTLIMPL_UDEC_2220u 0x8ACu, 0, 0, 0, 0, 11, 47, 2, 2, 3, 5, 37
#define PPUTLIMPL_UDEC_2219u 0x8ABu, 0, 0, 0, 0, 11, 47, 7, 317
#define PPUTLIMPL_UDEC_2218u 0x8AAu, 0, 0, 0, 0, 11, 47, 2, 1109
#define PPUTLIMPL_UDEC_2217u 0x8A9u, 0, 0, 0, 0, 11, 47, 3, 739
#define PPUTLIMPL_UDEC_2216u 0x8A8u, 0, 0, 0, 0, 11, 47, 2, 2, 2, 277
#define PPUTLIMPL_UDEC_2215u 0x8A7u, 0, 0, 0, 0, 11, 47, 5, 443
#define PPUTLIMPL_UDEC_2214u 0x8A6u, 0, 0, 0, 0, 11, 47, 2, 3, 3, 3, 41
#define PPUTLIMPL_UDEC_2213u 0x8A5u, 0, 0, 0, 0, 11, 47,
#define PPUTLIMPL_UDEC_2212u 0x8A4u, 0, 0, 0, 0, 11, 47, 2, 2, 7, 79
#define PPUTLIMPL_UDEC_2211u 0x8A3u, 0, 0, 0, 0, 11, 47, 3, 11, 67
#define PPUTLIMPL_UDEC_2210u 0x8A2u, 0, 0, 0, 0, 11, 47, 2, 5, 13, 17
#define PPUTLIMPL_UDEC_2209u 0x8A1u, 0, 0, 0, 0, 11, 47, 47, 47
#define PPUTLIMPL_UDEC_2208u 0x8A0u, 0, 0, 0, 0, 11, 46, 2, 2, 2, 2, 2, 3, 23
#define PPUTLIMPL_UDEC_2207u 0x89Fu, 0, 0, 0, 0, 11, 46,
#define PPUTLIMPL_UDEC_2206u 0x89Eu, 0, 0, 0, 0, 11, 46, 2, 1103
#define PPUTLIMPL_UDEC_2205u 0x89Du, 0, 0, 0, 0, 11, 46, 3, 3, 5, 7, 7
#define PPUTLIMPL_UDEC_2204u 0x89Cu, 0, 0, 0, 0, 11, 46, 2, 2, 19, 29
#define PPUTLIMPL_UDEC_2203u 0x89Bu, 0, 0, 0, 0, 11, 46,
#define PPUTLIMPL_UDEC_2202u 0x89Au, 0, 0, 0, 0, 11, 46, 2, 3, 367
#define PPUTLIMPL_UDEC_2201u 0x899u, 0, 0, 0, 0, 11, 46, 31, 71
#define PPUTLIMPL_UDEC_2200u 0x898u, 0, 0, 0, 0, 11, 46, 2, 2, 2, 5, 5, 11
#define PPUTLIMPL_UDEC_2199u 0x897u, 0, 0, 0, 0, 11, 46, 3, 733
#define PPUTLIMPL_UDEC_2198u 0x896u, 0, 0, 0, 0, 11, 46, 2, 7, 157
#define PPUTLIMPL_UDEC_2197u 0x895u, 0, 0, 0, 0, 11, 46, 13, 13, 13
#define PPUTLIMPL_UDEC_2196u 0x894u, 0, 0, 0, 0, 11, 46, 2, 2, 3, 3, 61
#define PPUTLIMPL_UDEC_2195u 0x893u, 0, 0, 0, 0, 11, 46, 5, 439
#define PPUTLIMPL_UDEC_2194u 0x892u, 0, 0, 0, 0, 11, 46, 2, 1097
#define PPUTLIMPL_UDEC_2193u 0x891u, 0, 0, 0, 0, 11, 46, 3, 17, 43
#define PPUTLIMPL_UDEC_2192u 0x890u, 0, 0, 0, 0, 11, 46, 2, 2, 2, 2, 137
#define PPUTLIMPL_UDEC_2191u 0x88Fu, 0, 0, 0, 0, 11, 46, 7, 313
#define PPUTLIMPL_UDEC_2190u 0x88Eu, 0, 0, 0, 0, 11, 46, 2, 3, 5, 73
#define PPUTLIMPL_UDEC_2189u 0x88Du, 0, 0, 0, 0, 11, 46, 11, 199
#define PPUTLIMPL_UDEC_2188u 0x88Cu, 0, 0, 0, 0, 11, 46, 2, 2, 547
#define PPUTLIMPL_UDEC_2187u 0x88Bu, 0, 0, 0, 0, 11, 46, 3, 3, 3, 3, 3, 3, 3
#define PPUTLIMPL_UDEC_2186u 0x88Au, 0, 0, 0, 0, 11, 46, 2, 1093
#define PPUTLIMPL_UDEC_2185u 0x889u, 0, 0, 0, 0, 11, 46, 5, 19, 23
#define PPUTLIMPL_UDEC_2184u 0x888u, 0, 0, 0, 0, 11, 46, 2, 2, 2, 3, 7, 13
#define PPUTLIMPL_UDEC_2183u 0x887u, 0, 0, 0, 0, 11, 46, 37, 59
#define PPUTLIMPL_UDEC_2182u 0x886u, 0, 0, 0, 0, 11, 46, 2, 1091
#define PPUTLIMPL_UDEC_2181u 0x885u, 0, 0, 0, 0, 11, 46, 3, 727
#define PPUTLIMPL_UDEC_2180u 0x884u, 0, 0, 0, 0, 11, 46, 2, 2, 5, 109
#define PPUTLIMPL_UDEC_2179u 0x883u, 0, 0, 0, 0, 11, 46,
#define PPUTLIMPL_UDEC_2178u 0x882u, 0, 0, 0, 0, 11, 46, 2, 3, 3, 11, 11
#define PPUTLIMPL_UDEC_2177u 0x881u, 0, 0, 0, 0, 11, 46, 7, 311
#define PPUTLIMPL_UDEC_2176u 0x880u, 0, 0, 0, 0, 11, 46, 2, 2, 2, 2, 2, 2, 2, 17
#define PPUTLIMPL_UDEC_2175u 0x87Fu, 0, 0, 0, 0, 11, 46, 3, 5, 5, 29
#define PPUTLIMPL_UDEC_2174u 0x87Eu, 0, 0, 0, 0, 11, 46, 2, 1087
#define PPUTLIMPL_UDEC_2173u 0x87Du, 0, 0, 0, 0, 11, 46, 41, 53
#define PPUTLIMPL_UDEC_2172u 0x87Cu, 0, 0, 0, 0, 11, 46, 2, 2, 3, 181
#define PPUTLIMPL_UDEC_2171u 0x87Bu, 0, 0, 0, 0, 11, 46, 13, 167
#define PPUTLIMPL_UDEC_2170u 0x87Au, 0, 0, 0, 0, 11, 46, 2, 5, 7, 31
#define PPUTLIMPL_UDEC_2169u 0x879u, 0, 0, 0, 0, 11, 46, 3, 3, 241
#define PPUTLIMPL_UDEC_2168u 0x878u, 0, 0, 0, 0, 11, 46, 2, 2, 2, 271
#define PPUTLIMPL_UDEC_2167u 0x877u, 0, 0, 0, 0, 11, 46, 11, 197
#define PPUTLIMPL_UDEC_2166u 0x876u, 0, 0, 0, 0, 11, 46, 2, 3, 19, 19
#define PPUTLIMPL_UDEC_2165u 0x875u, 0, 0, 0, 0, 11, 46, 5, 433
#define PPUTLIMPL_UDEC_2164u 0x874u, 0, 0, 0, 0, 11, 46, 2, 2, 541
#define PPUTLIMPL_UDEC_2163u 0x873u, 0, 0, 0, 0, 11, 46, 3, 7, 103
#define PPUTLIMPL_UDEC_2162u 0x872u, 0, 0, 0, 0, 11, 46, 2, 23, 47
#define PPUTLIMPL_UDEC_2161u 0x871u, 0, 0, 0, 0, 11, 46,
#define PPUTLIMPL_UDEC_2160u 0x870u, 0, 0, 0, 0, 11, 46, 2, 2, 2, 2, 3, 3, 3, 5
#define PPUTLIMPL_UDEC_2159u 0x86Fu, 0, 0, 0, 0, 11, 46, 17, 127
#define PPUTLIMPL_UDEC_2158u 0x86Eu, 0, 0, 0, 0, 11, 46, 2, 13, 83
#define PPUTLIMPL_UDEC_2157u 0x86Du, 0, 0, 0, 0, 11, 46, 3, 719
#define PPUTLIMPL_UDEC_2156u 0x86Cu, 0, 0, 0, 0, 11, 46, 2, 2, 7, 7, 11
#define PPUTLIMPL_UDEC_2155u 0x86Bu, 0, 0, 0, 0, 11, 46, 5, 431
#define PPUTLIMPL_UDEC_2154u 0x86Au, 0, 0, 0, 0, 11, 46, 2, 3, 359
#define PPUTLIMPL_UDEC_2153u 0x869u, 0, 0, 0, 0, 11, 46,
#define PPUTLIMPL_UDEC_2152u 0x868u, 0, 0, 0, 0, 11, 46, 2, 2, 2, 269
#define PPUTLIMPL_UDEC_2151u 0x867u, 0, 0, 0, 0, 11, 46, 3, 3, 239
#define PPUTLIMPL_UDEC_2150u 0x866u, 0, 0, 0, 0, 11, 46, 2, 5, 5, 43
#define PPUTLIMPL_UDEC_2149u 0x865u, 0, 0, 0, 0, 11, 46, 7, 307
#define PPUTLIMPL_UDEC_2148u 0x864u, 0, 0, 0, 0, 11, 46, 2, 2, 3, 179
#define PPUTLIMPL_UDEC_2147u 0x863u, 0, 0, 0, 0, 11, 46, 19, 113
#define PPUTLIMPL_UDEC_2146u 0x862u, 0, 0, 0, 0, 11, 46, 2, 29, 37
#define PPUTLIMPL_UDEC_2145u 0x861u, 0, 0, 0, 0, 11, 46, 3, 5, 11, 13
#define PPUTLIMPL_UDEC_2144u 0x860u, 0, 0, 0, 0, 11, 46, 2, 2, 2, 2, 2, 67
#define PPUTLIMPL_UDEC_2143u 0x85Fu, 0, 0, 0, 0, 11, 46,
#define PPUTLIMPL_UDEC_2142u 0x85Eu, 0, 0, 0, 0, 11, 46, 2, 3, 3, 7, 17
#define PPUTLIMPL_UDEC_2141u 0x85Du, 0, 0, 0, 0, 11, 46,
#define PPUTLIMPL_UDEC_2140u 0x85Cu, 0, 0, 0, 0, 11, 46, 2, 2, 5, 107
#define PPUTLIMPL_UDEC_2139u 0x85Bu, 0, 0, 0, 0, 11, 46, 3, 23, 31
#define PPUTLIMPL_UDEC_2138u 0x85Au, 0, 0, 0, 0, 11, 46, 2, 1069
#define PPUTLIMPL_UDEC_2137u 0x859u, 0, 0, 0, 0, 11, 46,
#define PPUTLIMPL_UDEC_2136u 0x858u, 0, 0, 0, 0, 11, 46, 2, 2, 2, 3, 89
#define PPUTLIMPL_UDEC_2135u 0x857u, 0, 0, 0, 0, 11, 46, 5, 7, 61
#define PPUTLIMPL_UDEC_2134u 0x856u, 0, 0, 0, 0, 11, 46, 2, 11, 97
#define PPUTLIMPL_UDEC_2133u 0x855u, 0, 0, 0, 0, 11, 46, 3, 3, 3, 79
#define PPUTLIMPL_UDEC_2132u 0x854u, 0, 0, 0, 0, 11, 46, 2, 2, 13, 41
#define PPUTLIMPL_UDEC_2131u 0x853u, 0, 0, 0, 0, 11, 46,
#define PPUTLIMPL_UDEC_2130u 0x852u, 0, 0, 0, 0, 11, 46, 2, 3, 5, 71
#define PPUTLIMPL_UDEC_2129u 0x851u, 0, 0, 0, 0, 11, 46,
#define PPUTLIMPL_UDEC_2128u 0x850u, 0, 0, 0, 0, 11, 46, 2, 2, 2, 2, 7, 19
#define PPUTLIMPL_UDEC_2127u 0x84Fu, 0, 0, 0, 0, 11, 46, 3, 709
#define PPUTLIMPL_UDEC_2126u 0x84Eu, 0, 0, 0, 0, 11, 46, 2, 1063
#define PPUTLIMPL_UDEC_2125u 0x84Du, 0, 0, 0, 0, 11, 46, 5, 5, 5, 17
#define PPUTLIMPL_UDEC_2124u 0x84Cu, 0, 0, 0, 0, 11, 46, 2, 2, 3, 3, 59
#define PPUTLIMPL_UDEC_2123u 0x84Bu, 0, 0, 0, 0, 11, 46, 11, 193
#define PPUTLIMPL_UDEC_2122u 0x84Au, 0, 0, 0, 0, 11, 46, 2, 1061
#define PPUTLIMPL_UDEC_2121u 0x849u, 0, 0, 0, 0, 11, 46, 3, 7, 101
#define PPUTLIMPL_UDEC_2120u 0x848u, 0, 0, 0, 0, 11, 46, 2, 2, 2, 5, 53
#define PPUTLIMPL_UDEC_2119u 0x847u, 0, 0, 0, 0, 11, 46, 13, 163
#define PPUTLIMPL_UDEC_2118u 0x846u, 0, 0, 0, 0, 11, 46, 2, 3, 353
#define PPUTLIMPL_UDEC_2117u 0x845u, 0, 0, 0, 0, 11, 46, 29, 73
#define PPUTLIMPL_UDEC_2116u 0x844u, 0, 0, 0, 0, 11, 46, 2, 2, 23, 23
#define PPUTLIMPL_UDEC_2115u 0x843u, 0, 0, 0, 0, 11, 45, 3, 3, 5, 47
#define PPUTLIMPL_UDEC_2114u 0x842u, 0, 0, 0, 0, 11, 45, 2, 7, 151
#define PPUTLIMPL_UDEC_2113u 0x841u, 0, 0, 0, 0, 11, 45,
#define PPUTLIMPL_UDEC_2112u 0x840u, 0, 0, 0, 0, 11, 45, 2, 2, 2, 2, 2, 2, 3, 11
#define PPUTLIMPL_UDEC_2111u 0x83Fu, 0, 0, 0, 0, 11, 45,
#define PPUTLIMPL_UDEC_2110u 0x83Eu, 0, 0, 0, 0, 11, 45, 2, 5, 211
#define PPUTLIMPL_UDEC_2109u 0x83Du, 0, 0, 0, 0, 11, 45, 3, 19, 37
#define PPUTLIMPL_UDEC_2108u 0x83Cu, 0, 0, 0, 0, 11, 45, 2, 2, 17, 31
#define PPUTLIMPL_UDEC_2107u 0x83Bu, 0, 0, 0, 0, 11, 45, 7, 7, 43
#define PPUTLIMPL_UDEC_2106u 0x83Au, 0, 0, 0, 0, 11, 45, 2, 3, 3, 3, 3, 13
#define PPUTLIMPL_UDEC_2105u 0x839u, 0, 0, 0, 0, 11, 45, 5, 421
#define PPUTLIMPL_UDEC_2104u 0x838u, 0, 0, 0, 0, 11, 45, 2, 2, 2, 263
#define PPUTLIMPL_UDEC_2103u 0x837u, 0, 0, 0, 0, 11, 45, 3, 701
#define PPUTLIMPL_UDEC_2102u 0x836u, 0, 0, 0, 0, 11, 45, 2, 1051
#define PPUTLIMPL_UDEC_2101u 0x835u, 0, 0, 0, 0, 11, 45, 11, 191
#define PPUTLIMPL_UDEC_2100u 0x834u, 0, 0, 0, 0, 11, 45, 2, 2, 3, 5, 5, 7
#define PPUTLIMPL_UDEC_2099u 0x833u, 0, 0, 0, 0, 11, 45,
#define PPUTLIMPL_UDEC_2098u 0x832u, 0, 0, 0, 0, 11, 45, 2, 1049
#define PPUTLIMPL_UDEC_2097u 0x831u, 0, 0, 0, 0, 11, 45, 3, 3, 233
#define PPUTLIMPL_UDEC_2096u 0x830u, 0, 0, 0, 0, 11, 45, 2, 2, 2, 2, 131
#define PPUTLIMPL_UDEC_2095u 0x82Fu, 0, 0, 0, 0, 11, 45, 5, 419
#define PPUTLIMPL_UDEC_2094u 0x82Eu, 0, 0, 0, 0, 11, 45, 2, 3, 349
#define PPUTLIMPL_UDEC_2093u 0x82Du, 0, 0, 0, 0, 11, 45, 7, 13, 23
#define PPUTLIMPL_UDEC_2092u 0x82Cu, 0, 0, 0, 0, 11, 45, 2, 2, 523
#define PPUTLIMPL_UDEC_2091u 0x82Bu, 0, 0, 0, 0, 11, 45, 3, 17, 41
#define PPUTLIMPL_UDEC_2090u 0x82Au, 0, 0, 0, 0, 11, 45, 2, 5, 11, 19
#define PPUTLIMPL_UDEC_2089u 0x829u, 0, 0, 0, 0, 11, 45,
#define PPUTLIMPL_UDEC_2088u 0x828u, 0, 0, 0, 0, 11, 45, 2, 2, 2, 3, 3, 29
#define PPUTLIMPL_UDEC_2087u 0x827u, 0, 0, 0, 0, 11, 45,
#define PPUTLIMPL_UDEC_2086u 0x826u, 0, 0, 0, 0, 11, 45, 2, 7, 149
#define PPUTLIMPL_UDEC_2085u 0x825u, 0, 0, 0, 0, 11, 45, 3, 5, 139
#define PPUTLIMPL_UDEC_2084u 0x824u, 0, 0, 0, 0, 11, 45, 2, 2, 521
#define PPUTLIMPL_UDEC_2083u 0x823u, 0, 0, 0, 0, 11, 45,
#define PPUTLIMPL_UDEC_2082u 0x822u, 0, 0, 0, 0, 11, 45, 2, 3, 347
#define PPUTLIMPL_UDEC_2081u 0x821u, 0, 0, 0, 0, 11, 45,
#define PPUTLIMPL_UDEC_2080u 0x820u, 0, 0, 0, 0, 11, 45, 2, 2, 2, 2, 2, 5, 13
#define PPUTLIMPL_UDEC_2079u 0x81Fu, 0, 0, 0, 0, 11, 45, 3, 3, 3, 7, 11
#define PPUTLIMPL_UDEC_2078u 0x81Eu, 0, 0, 0, 0, 11, 45, 2, 1039
#define PPUTLIMPL_UDEC_2077u 0x81Du, 0, 0, 0, 0, 11, 45, 31, 67
#define PPUTLIMPL_UDEC_2076u 0x81Cu, 0, 0, 0, 0, 11, 45, 2, 2, 3, 173
#define PPUTLIMPL_UDEC_2075u 0x81Bu, 0, 0, 0, 0, 11, 45, 5, 5, 83
#define PPUTLIMPL_UDEC_2074u 0x81Au, 0, 0, 0, 0, 11, 45, 2, 17, 61
#define PPUTLIMPL_UDEC_2073u 0x819u, 0, 0, 0, 0, 11, 45, 3, 691
#define PPUTLIMPL_UDEC_2072u 0x818u, 0, 0, 0, 0, 11, 45, 2, 2, 2, 7, 37
#define PPUTLIMPL_UDEC_2071u 0x817u, 0, 0, 0, 0, 11, 45, 19, 109
#define PPUTLIMPL_UDEC_2070u 0x816u, 0, 0, 0, 0, 11, 45, 2, 3, 3, 5, 23
#define PPUTLIMPL_UDEC_2069u 0x815u, 0, 0, 0, 0, 11, 45,
#define PPUTLIMPL_UDEC_2068u 0x814u, 0, 0, 0, 0, 11, 45, 2, 2, 11, 47
#define PPUTLIMPL_UDEC_2067u 0x813u, 0, 0, 0, 0, 11, 45, 3, 13, 53
#define PPUTLIMPL_UDEC_2066u 0x812u, 0, 0, 0, 0, 11, 45, 2, 1033
#define PPUTLIMPL_UDEC_2065u 0x811u, 0, 0, 0, 0, 11, 45, 5, 7, 59
#define PPUTLIMPL_UDEC_2064u 0x810u, 0, 0, 0, 0, 11, 45, 2, 2, 2, 2, 3, 43
#define PPUTLIMPL_UDEC_2063u 0x80Fu, 0, 0, 0, 0, 11, 45,
#define PPUTLIMPL_UDEC_2062u 0x80Eu, 0, 0, 0, 0, 11, 45, 2, 1031
#define PPUTLIMPL_UDEC_2061u 0x80Du, 0, 0, 0, 0, 11, 45, 3, 3, 229
#define PPUTLIMPL_UDEC_2060u 0x80Cu, 0, 0, 0, 0, 11, 45, 2, 2, 5, 103
#define PPUTLIMPL_UDEC_2059u 0x80Bu, 0, 0, 0, 0, 11, 45, 29, 71
#define PPUTLIMPL_UDEC_2058u 0x80Au, 0, 0, 0, 0, 11, 45, 2, 3, 7, 7, 7
#define PPUTLIMPL_UDEC_2057u 0x809u, 0, 0, 0, 0, 11, 45, 11, 11, 17
#define PPUTLIMPL_UDEC_2056u 0x808u, 0, 0, 0, 0, 11, 45, 2, 2, 2, 257
#define PPUTLIMPL_UDEC_2055u 0x807u, 0, 0, 0, 0, 11, 45, 3, 5, 137
#define PPUTLIMPL_UDEC_2054u 0x806u, 0, 0, 0, 0, 11, 45, 2, 13, 79
#define PPUTLIMPL_UDEC_2053u 0x805u, 0, 0, 0, 0, 11, 45,
#define PPUTLIMPL_UDEC_2052u 0x804u, 0, 0, 0, 0, 11, 45, 2, 2, 3, 3, 3, 19
#define PPUTLIMPL_UDEC_2051u 0x803u, 0, 0, 0, 0, 11, 45, 7, 293
#define PPUTLIMPL_UDEC_2050u 0x802u, 0, 0, 0, 0, 11, 45, 2, 5, 5, 41
#define PPUTLIMPL_UDEC_2049u 0x801u, 0, 0, 0, 0, 11, 45, 3, 683
#define PPUTLIMPL_UDEC_2048u 0x800u, 0, 0, 0, 0, 11, 45, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
#define PPUTLIMPL_UDEC_2047u 0x7FFu, 0, 0, 0, 0, 10, 45, 23, 89
#define PPUTLIMPL_UDEC_2046u 0x7FEu, 0, 0, 0, 0, 10, 45, 2, 3, 11, 31
#define PPUTLIMPL_UDEC_2045u 0x7FDu, 0, 0, 0, 0, 10, 45, 5, 409
#define PPUTLIMPL_UDEC_2044u 0x7FCu, 0, 0, 0, 0, 10, 45, 2, 2, 7, 73
#define PPUTLIMPL_UDEC_2043u 0x7FBu, 0, 0, 0, 0, 10, 45, 3, 3, 227
#define PPUTLIMPL_UDEC_2042u 0x7FAu, 0, 0, 0, 0, 10, 45, 2, 1021
#define PPUTLIMPL_UDEC_2041u 0x7F9u, 0, 0, 0, 0, 10, 45, 13, 157
#define PPUTLIMPL_UDEC_2040u 0x7F8u, 0, 0, 0, 0, 10, 45, 2, 2, 2, 3, 5, 17
#define PPUTLIMPL_UDEC_2039u 0x7F7u, 0, 0, 0, 0, 10, 45,
#define PPUTLIMPL_UDEC_2038u 0x7F6u, 0, 0, 0, 0, 10, 45, 2, 1019
#define PPUTLIMPL_UDEC_2037u 0x7F5u, 0, 0, 0, 0, 10, 45, 3, 7, 97
#define PPUTLIMPL_UDEC_2036u 0x7F4u, 0, 0, 0, 0, 10, 45, 2, 2, 509
#define PPUTLIMPL_UDEC_2035u 0x7F3u, 0, 0, 0, 0, 10, 45, 5, 11, 37
#define PPUTLIMPL_UDEC_2034u 0x7F2u, 0, 0, 0, 0, 10, 45, 2, 3, 3, 113
#define PPUTLIMPL_UDEC_2033u 0x7F1u, 0, 0, 0, 0, 10, 45, 19, 107
#define PPUTLIMPL_UDEC_2032u 0x7F0u, 0, 0, 0, 0, 10, 45, 2, 2, 2, 2, 127
#define PPUTLIMPL_UDEC_2031u 0x7EFu, 0, 0, 0, 0, 10, 45, 3, 677
#define PPUTLIMPL_UDEC_2030u 0x7EEu, 0, 0, 0, 0, 10, 45, 2, 5, 7, 29
#define PPUTLIMPL_UDEC_2029u 0x7EDu, 0, 0, 0, 0, 10, 45,
#define PPUTLIMPL_UDEC_2028u 0x7ECu, 0, 0, 0, 0, 10, 45, 2, 2, 3, 13, 13
#define PPUTLIMPL_UDEC_2027u 0x7EBu, 0, 0, 0, 0, 10, 45,
#define PPUTLIMPL_UDEC_2026u 0x7EAu, 0, 0, 0, 0, 10, 45, 2, 1013
#define PPUTLIMPL_UDEC_2025u 0x7E9u, 0, 0, 0, 0, 10, 45, 3, 3, 3, 3, 5, 5
#define PPUTLIMPL_UDEC_2024u 0x7E8u, 0, 0, 0, 0, 10, 44, 2, 2, 2, 11, 23
#define PPUTLIMPL_UDEC_2023u 0x7E7u, 0, 0, 0, 0, 10, 44, 7, 17, 17
#define PPUTLIMPL_UDEC_2022u 0x7E6u, 0, 0, 0, 0, 10, 44, 2, 3, 337
#define PPUTLIMPL_UDEC_2021u 0x7E5u, 0, 0, 0, 0, 10, 44, 43, 47
#define PPUTLIMPL_UDEC_2020u 0x7E4u, 0, 0, 0, 0, 10, 44, 2, 2, 5, 101
#define PPUTLIMPL_UDEC_2019u 0x7E3u, 0, 0, 0, 0, 10, 44, 3, 673
#define PPUTLIMPL_UDEC_2018u 0x7E2u, 0, 0, 0, 0, 10, 44, 2, 1009
#define PPUTLIMPL_UDEC_2017u 0x7E1u, 0, 0, 0, 0, 10, 44,
#define PPUTLIMPL_UDEC_2016u 0x7E0u, 0, 0, 0, 0, 10, 44, 2, 2, 2, 2, 2, 3, 3, 7
#define PPUTLIMPL_UDEC_2015u 0x7DFu, 0, 0, 0, 0, 10, 44, 5, 13, 31
#define PPUTLIMPL_UDEC_2014u 0x7DEu, 0, 0, 0, 0, 10, 44, 2, 19, 53
#define PPUTLIMPL_UDEC_2013u 0x7DDu, 0, 0, 0, 0, 10, 44, 3, 11, 61
#define PPUTLIMPL_UDEC_2012u 0x7DCu, 0, 0, 0, 0, 10, 44, 2, 2, 503
#define PPUTLIMPL_UDEC_2011u 0x7DBu, 0, 0, 0, 0, 10, 44,
#define PPUTLIMPL_UDEC_2010u 0x7DAu, 0, 0, 0, 0, 10, 44, 2, 3, 5, 67
#define PPUTLIMPL_UDEC_2009u 0x7D9u, 0, 0, 0, 0, 10, 44, 7, 7, 41
#define PPUTLIMPL_UDEC_2008u 0x7D8u, 0, 0, 0, 0, 10, 44, 2, 2, 2, 251
#define PPUTLIMPL_UDEC_2007u 0x7D7u, 0, 0, 0, 0, 10, 44, 3, 3, 223
#define PPUTLIMPL_UDEC_2006u 0x7D6u, 0, 0, 0, 0, 10, 44, 2, 17, 59
#define PPUTLIMPL_UDEC_2005u 0x7D5u, 0, 0, 0, 0, 10, 44, 5, 401
#define PPUTLIMPL_UDEC_2004u 0x7D4u, 0, 0, 0, 0, 10, 44, 2, 2, 3, 167
#define PPUTLIMPL_UDEC_2003u 0x7D3u, 0, 0, 0, 0, 10, 44,
#define PPUTLIMPL_UDEC_2002u 0x7D2u, 0, 0, 0, 0, 10, 44, 2, 7, 11, 13
#define PPUTLIMPL_UDEC_2001u 0x7D1u, 0, 0, 0, 0, 10, 44, 3, 23, 29
#define PPUTLIMPL_UDEC_2000u 0x7D0u, 0, 0, 0, 0, 10, 44, 2, 2, 2, 2, 5, 5, 5
#define PPUTLIMPL_UDEC_1999u 0x7CFu, 0, 0, 0, 0, 10, 44,
#define PPUTLIMPL_UDEC_1998u 0x7CEu, 0, 0, 0, 0, 10, 44, 2, 3, 3, 3, 37
#define PPUTLIMPL_UDEC_1997u 0x7CDu, 0, 0, 0, 0, 10, 44,
#define PPUTLIMPL_UDEC_1996u 0x7CCu, 0, 0, 0, 0, 10, 44, 2, 2, 499
#define PPUTLIMPL_UDEC_1995u 0x7CBu, 0, 0, 0, 0, 10, 44, 3, 5, 7, 19
#define PPUTLIMPL_UDEC_1994u 0x7CAu, 0, 0, 0, 0, 10, 44, 2, 997
#define PPUTLIMPL_UDEC_1993u 0x7C9u, 0, 0, 0, 0, 10, 44,
#define PPUTLIMPL_UDEC_1992u 0x7C8u, 0, 0, 0, 0, 10, 44, 2, 2, 2, 3, 83
#define PPUTLIMPL_UDEC_1991u 0x7C7u, 0, 0, 0, 0, 10, 44, 11, 181
#define PPUTLIMPL_UDEC_1990u 0x7C6u, 0, 0, 0, 0, 10, 44, 2, 5, 199
#define PPUTLIMPL_UDEC_1989u 0x7C5u, 0, 0, 0, 0, 10, 44, 3, 3, 13, 17
#define PPUTLIMPL_UDEC_1988u 0x7C4u, 0, 0, 0, 0, 10, 44, 2, 2, 7, 71
#define PPUTLIMPL_UDEC_1987u 0x7C3u, 0, 0, 0, 0, 10, 44,
#define PPUTLIMPL_UDEC_1986u 0x7C2u, 0, 0, 0, 0, 10, 44, 2, 3, 331
#define PPUTLIMPL_UDEC_1985u 0x7C1u, 0, 0, 0, 0, 10, 44, 5, 397
#define PPUTLIMPL_UDEC_1984u 0x7C0u, 0, 0, 0, 0, 10, 44, 2, 2, 2, 2, 2, 2, 31
#define PPUTLIMPL_UDEC_1983u 0x7BFu, 0, 0, 0, 0, 10, 44, 3, 661
#define PPUTLIMPL_UDEC_1982u 0x7BEu, 0, 0, 0, 0, 10, 44, 2, 991
#define PPUTLIMPL_UDEC_1981u 0x7BDu, 0, 0, 0, 0, 10, 44, 7, 283
#define PPUTLIMPL_UDEC_1980u 0x7BCu, 0, 0, 0, 0, 10, 44, 2, 2, 3, 3, 5, 11
#define PPUTLIMPL_UDEC_1979u 0x7BBu, 0, 0, 0, 0, 10, 44,
#define PPUTLIMPL_UDEC_1978u 0x7BAu, 0, 0, 0, 0, 10, 44, 2, 23, 43
#define PPUTLIMPL_UDEC_1977u 0x7B9u, 0, 0, 0, 0, 10, 44, 3, 659
#define PPUTLIMPL_UDEC_1976u 0x7B8u, 0, 0, 0, 0, 10, 44, 2, 2, 2, 13, 19
#define PPUTLIMPL_UDEC_1975u 0x7B7u, 0, 0, 0, 0, 10, 44, 5, 5, 79
#define PPUTLIMPL_UDEC_1974u 0x7B6u, 0, 0, 0, 0, 10, 44, 2, 3, 7, 47
#define PPUTLIMPL_UDEC_1973u 0x7B5u, 0, 0, 0, 0, 10, 44,
#define PPUTLIMPL_UDEC_1972u 0x7B4u, 0, 0, 0, 0, 10, 44, 2, 2, 17, 29
#define PPUTLIMPL_UDEC_1971u 0x7B3u, 0, 0, 0, 0, 10, 44, 3, 3, 3, 73
#define PPUTLIMPL_UDEC_1970u 0x7B2u, 0, 0, 0, 0, 10, 44, 2, 5, 197
#define PPUTLIMPL_UDEC_1969u 0x7B1u, 0, 0, 0, 0, 10, 44, 11, 179
#define PPUTLIMPL_UDEC_1968u 0x7B0u, 0, 0, 0, 0, 10, 44, 2, 2, 2, 2, 3, 41
#define PPUTLIMPL_UDEC_1967u 0x7AFu, 0, 0, 0, 0, 10, 44, 7, 281
#define PPUTLIMPL_UDEC_1966u 0x7AEu, 0, 0, 0, 0, 10, 44, 2, 983
#define PPUTLIMPL_UDEC_1965u 0x7ADu, 0, 0, 0, 0, 10, 44, 3, 5, 131
#define PPUTLIMPL_UDEC_1964u 0x7ACu, 0, 0, 0, 0, 10, 44, 2, 2, 491
#define PPUTLIMPL_UDEC_1963u 0x7ABu, 0, 0, 0, 0, 10, 44, 13, 151
#define PPUTLIMPL_UDEC_1962u 0x7AAu, 0, 0, 0, 0, 10, 44, 2, 3, 3, 109
#define PPUTLIMPL_UDEC_1961u 0x7A9u, 0, 0, 0, 0, 10, 44, 37, 53
#define PPUTLIMPL_UDEC_1960u 0x7A8u, 0, 0, 0, 0, 10, 44, 2, 2, 2, 5, 7, 7
#define PPUTLIMPL_UDEC_1959u 0x7A7u, 0, 0, 0, 0, 10, 44, 3, 653
#define PPUTLIMPL_UDEC_1958u 0x7A6u, 0, 0, 0, 0, 10, 44, 2, 11, 89
#define PPUTLIMPL_UDEC_1957u 0x7A5u, 0, 0, 0, 0, 10, 44, 19, 103
#define PPUTLIMPL_UDEC_1956u 0x7A4u, 0, 0, 0, 0, 10, 44, 2, 2, 3, 163
#define PPUTLIMPL_UDEC_1955u 0x7A3u, 0, 0, 0, 0, 10, 44, 5, 17, 23
#define PPUTLIMPL_UDEC_1954u 0x7A2u, 0, 0, 0, 0, 10, 44, 2, 977
#define PPUTLIMPL_UDEC_1953u 0x7A1u, 0, 0, 0, 0, 10, 44, 3, 3, 7, 31
#define PPUTLIMPL_UDEC_1952u 0x7A0u, 0, 0, 0, 0, 10, 44, 2, 2, 2, 2, 2, 61
#define PPUTLIMPL_UDEC_1951u 0x79Fu, 0, 0, 0, 0, 10, 44,
#define PPUTLIMPL_UDEC_1950u 0x79Eu, 0, 0, 0, 0, 10, 44, 2, 3, 5, 5, 13
#define PPUTLIMPL_UDEC_1949u 0x79Du, 0, 0, 0, 0, 10, 44,
#define PPUTLIMPL_UDEC_1948u 0x79Cu, 0, 0, 0, 0, 10, 44, 2, 2, 487
#define PPUTLIMPL_UDEC_1947u 0x79Bu, 0, 0, 0, 0, 10, 44, 3, 11, 59
#define PPUTLIMPL_UDEC_1946u 0x79Au, 0, 0, 0, 0, 10, 44, 2, 7, 139
#define PPUTLIMPL_UDEC_1945u 0x799u, 0, 0, 0, 0, 10, 44, 5, 389
#define PPUTLIMPL_UDEC_1944u 0x798u, 0, 0, 0, 0, 10, 44, 2, 2, 2, 3, 3, 3, 3, 3
#define PPUTLIMPL_UDEC_1943u 0x797u, 0, 0, 0, 0, 10, 44, 29, 67
#define PPUTLIMPL_UDEC_1942u 0x796u, 0, 0, 0, 0, 10, 44, 2, 971
#define PPUTLIMPL_UDEC_1941u 0x795u, 0, 0, 0, 0, 10, 44, 3, 647
#define PPUTLIMPL_UDEC_1940u 0x794u, 0, 0, 0, 0, 10, 44, 2, 2, 5, 97
#define PPUTLIMPL_UDEC_1939u 0x793u, 0, 0, 0, 0, 10, 44, 7, 277
#define PPUTLIMPL_UDEC_1938u 0x792u, 0, 0, 0, 0, 10, 44, 2, 3, 17, 19
#define PPUTLIMPL_UDEC_1937u 0x791u, 0, 0, 0, 0, 10, 44, 13, 149
#define PPUTLIMPL_UDEC_1936u 0x790u, 0, 0, 0, 0, 10, 44, 2, 2, 2, 2, 11, 11
#define PPUTLIMPL_UDEC_1935u 0x78Fu, 0, 0, 0, 0, 10, 43, 3, 3, 5, 43
#define PPUTLIMPL_UDEC_1934u 0x78Eu, 0, 0, 0, 0, 10, 43, 2, 967
#define PPUTLIMPL_UDEC_1933u 0x78Du, 0, 0, 0, 0, 10, 43,
#define PPUTLIMPL_UDEC_1932u 0x78Cu, 0, 0, 0, 0, 10, 43, 2, 2, 3, 7, 23
#define PPUTLIMPL_UDEC_1931u 0x78Bu, 0, 0, 0, 0, 10, 43,
#define PPUTLIMPL_UDEC_1930u 0x78Au, 0, 0, 0, 0, 10, 43, 2, 5, 193
#define PPUTLIMPL_UDEC_1929u 0x789u, 0, 0, 0, 0, 10, 43, 3, 643
#define PPUTLIMPL_UDEC_1928u 0x788u, 0, 0, 0, 0, 10, 43, 2, 2, 2, 241
#define PPUTLIMPL_UDEC_1927u 0x787u, 0, 0, 0, 0, 10, 43, 41, 47
#define PPUTLIMPL_UDEC_1926u 0x786u, 0, 0, 0, 0, 10, 43, 2, 3, 3, 107
#define PPUTLIMPL_UDEC_1925u 0x785u, 0, 0, 0, 0, 10, 43, 5, 5, 7, 11
#define PPUTLIMPL_UDEC_1924u 0x784u, 0, 0, 0, 0, 10, 43, 2, 2, 13, 37
#define PPUTLIMPL_UDEC_1923u 0x783u, 0, 0, 0, 0, 10, 43, 3, 641
#define PPUTLIMPL_UDEC_1922u 0x782u, 0, 0, 0, 0, 10, 43, 2, 31, 31
#define PPUTLIMPL_UDEC_1921u 0x781u, 0, 0, 0, 0, 10, 43, 17, 113
#define PPUTLIMPL_UDEC_1920u 0x780u, 0, 0, 0, 0, 10, 43, 2, 2, 2, 2, 2, 2, 2, 3, 5
#define PPUTLIMPL_UDEC_1919u 0x77Fu, 0, 0, 0, 0, 10, 43, 19, 101
#define PPUTLIMPL_UDEC_1918u 0x77Eu, 0, 0, 0, 0, 10, 43, 2, 7, 137
#define PPUTLIMPL_UDEC_1917u 0x77Du, 0, 0, 0, 0, 10, 43, 3, 3, 3, 71
#define PPUTLIMPL_UDEC_1916u 0x77Cu, 0, 0, 0, 0, 10, 43, 2, 2, 479
#define PPUTLIMPL_UDEC_1915u 0x77Bu, 0, 0, 0, 0, 10, 43, 5, 383
#define PPUTLIMPL_UDEC_1914u 0x77Au, 0, 0, 0, 0, 10, 43, 2, 3, 11, 29
#define PPUTLIMPL_UDEC_1913u 0x779u, 0, 0, 0, 0, 10, 43,
#define PPUTLIMPL_UDEC_1912u 0x778u, 0, 0, 0, 0, 10, 43, 2, 2, 2, 239
#define PPUTLIMPL_UDEC_1911u 0x777u, 0, 0, 0, 0, 10, 43, 3, 7, 7, 13
#define PPUTLIMPL_UDEC_1910u 0x776u, 0, 0, 0, 0, 10, 43, 2, 5, 191
#define PPUTLIMPL_UDEC_1909u 0x775u, 0, 0, 0, 0, 10, 43, 23, 83
#define PPUTLIMPL_UDEC_1908u 0x774u, 0, 0, 0, 0, 10, 43, 2, 2, 3, 3, 53
#define PPUTLIMPL_UDEC_1907u 0x773u, 0, 0, 0, 0, 10, 43,
#define PPUTLIMPL_UDEC_1906u 0x772u, 0, 0, 0, 0, 10, 43, 2, 953
#define PPUTLIMPL_UDEC_1905u 0x771u, 0, 0, 0, 0, 10, 43, 3, 5, 127
#define PPUTLIMPL_UDEC_1904u 0x770u, 0, 0, 0, 0, 10, 43, 2, 2, 2, 2, 7, 17
#define PPUTLIMPL_UDEC_1903u 0x76Fu, 0, 0, 0, 0, 10, 43, 11, 173
#define PPUTLIMPL_UDEC_1902u 0x76Eu, 0, 0, 0, 0, 10, 43, 2, 3, 317
#define PPUTLIMPL_UDEC_1901u 0x76Du, 0, 0, 0, 0, 10, 43,
#define PPUTLIMPL_UDEC_1900u 0x76Cu, 0, 0, 0, 0, 10, 43, 2, 2, 5, 5, 19
#define PPUTLIMPL_UDEC_1899u 0x76Bu, 0, 0, 0, 0, 10, 43, 3, 3, 211
#define PPUTLIMPL_UDEC_1898u 0x76Au, 0, 0, 0, 0, 10, 43, 2, 13, 73
#define PPUTLIMPL_UDEC_1897u 0x769u, 0, 0, 0, 0, 10, 43, 7, 271
#define PPUTLIMPL_UDEC_1896u 0x768u, 0, 0, 0, 0, 10, 43, 2, 2, 2, 3, 79
#define PPUTLIMPL_UDEC_1895u 0x767u, 0, 0, 0, 0, 10, 43, 5, 379
#define PPUTLIMPL_UDEC_1894u 0x766u, 0, 0, 0, 0, 10, 43, 2, 947
#define PPUTLIMPL_UDEC_1893u 0x765u, 0, 0, 0, 0, 10, 43, 3, 631
#define PPUTLIMPL_UDEC_1892u 0x764u, 0, 0, 0, 0, 10, 43, 2, 2, 11, 43
#define PPUTLIMPL_UDEC_1891u 0x763u, 0, 0, 0, 0, 10, 43, 31, 61
#define PPUTLIMPL_UDEC_1890u 0x762u, 0, 0, 0, 0, 10, 43, 2, 3, 3, 3, 5, 7
#define PPUTLIMPL_UDEC_1889u 0x761u, 0, 0, 0, 0, 10, 43,
#define PPUTLIMPL_UDEC_1888u 0x760u, 0, 0, 0, 0, 10, 43, 2, 2, 2, 2, 2, 59
#define PPUTLIMPL_UDEC_1887u 0x75Fu, 0, 0, 0, 0, 10, 43, 3, 17, 37
#define PPUTLIMPL_UDEC_1886u 0x75Eu, 0, 0, 0, 0, 10, 43, 2, 23, 41
#define PPUTLIMPL_UDEC_1885u 0x75Du, 0, 0, 0, 0, 10, 43, 5, 13, 29
#define PPUTLIMPL_UDEC_1884u 0x75Cu, 0, 0, 0, 0, 10, 43, 2, 2, 3, 157
#define PPUTLIMPL_UDEC_1883u 0x75Bu, 0, 0, 0, 0, 10, 43, 7, 269
#define PPUTLIMPL_UDEC_1882u 0x75Au, 0, 0, 0, 0, 10, 43, 2, 941
#define PPUTLIMPL_UDEC_1881u 0x759u, 0, 0, 0, 0, 10, 43, 3, 3, 11, 19
#define PPUTLIMPL_UDEC_1880u 0x758u, 0, 0, 0, 0, 10, 43, 2, 2, 2, 5, 47
#define PPUTLIMPL_UDEC_1879u 0x757u, 0, 0, 0, 0, 10, 43,
#define PPUTLIMPL_UDEC_1878u 0x756u, 0, 0, 0, 0, 10, 43, 2, 3, 313
#define PPUTLIMPL_UDEC_1877u 0x755u, 0, 0, 0, 0, 10, 43,
#define PPUTLIMPL_UDEC_1876u 0x754u, 0, 0, 0, 0, 10, 43, 2, 2, 7, 67
#define PPUTLIMPL_UDEC_1875u 0x753u, 0, 0, 0, 0, 10, 43, 3, 5, 5, 5, 5
#define PPUTLIMPL_UDEC_1874u 0x752u, 0, 0, 0, 0, 10, 43, 2, 937
#define PPUTLIMPL_UDEC_1873u 0x751u, 0, 0, 0, 0, 10, 43,
#define PPUTLIMPL_UDEC_1872u 0x750u, 0, 0, 0, 0, 10, 43, 2, 2, 2, 2, 3, 3, 13
#define PPUTLIMPL_UDEC_1871u 0x74Fu, 0, 0, 0, 0, 10, 43,
#define PPUTLIMPL_UDEC_1870u 0x74Eu, 0, 0, 0, 0, 10, 43, 2, 5, 11, 17
#define PPUTLIMPL_UDEC_1869u 0x74Du, 0, 0, 0, 0, 10, 43, 3, 7, 89
#define PPUTLIMPL_UDEC_1868u 0x74Cu, 0, 0, 0, 0, 10, 43, 2, 2, 467
#define PPUTLIMPL_UDEC_1867u 0x74Bu, 0, 0, 0, 0, 10, 43,
#define PPUTLIMPL_UDEC_1866u 0x74Au, 0, 0, 0, 0, 10, 43, 2, 3, 311
#define PPUTLIMPL_UDEC_1865u 0x749u, 0, 0, 0, 0, 10, 43, 5, 373
#define PPUTLIMPL_UDEC_1864u 0x748u, 0, 0, 0, 0, 10, 43, 2, 2, 2, 233
#define PPUTLIMPL_UDEC_1863u 0x747u, 0, 0, 0, 0, 10, 43, 3, 3, 3, 3, 23
#define PPUTLIMPL_UDEC_1862u 0x746u, 0, 0, 0, 0, 10, 43, 2, 7, 7, 19
#define PPUTLIMPL_UDEC_1861u 0x745u, 0, 0, 0, 0, 10, 43,
#define PPUTLIMPL_UDEC_1860u 0x744u, 0, 0, 0, 0, 10, 43, 2, 2, 3, 5, 31
#define PPUTLIMPL_UDEC_1859u 0x743u, 0, 0, 0, 0, 10, 43, 11, 13, 13
#define PPUTLIMPL_UDEC_1858u 0x742u, 0, 0, 0, 0, 10, 43, 2, 929
#define PPUTLIMPL_UDEC_1857u 0x741u, 0, 0, 0, 0, 10, 43, 3, 619
#define PPUTLIMPL_UDEC_1856u 0x740u, 0, 0, 0, 0, 10, 43, 2, 2, 2, 2, 2, 2, 29
#define PPUTLIMPL_UDEC_1855u 0x73Fu, 0, 0, 0, 0, 10, 43, 5, 7, 53
#define PPUTLIMPL_UDEC_1854u 0x73Eu, 0, 0, 0, 0, 10, 43, 2, 3, 3, 103
#define PPUTLIMPL_UDEC_1853u 0x73Du, 0, 0, 0, 0, 10, 43, 17, 109
#define PPUTLIMPL_UDEC_1852u 0x73Cu, 0, 0, 0, 0, 10, 43, 2, 2, 463
#define PPUTLIMPL_UDEC_1851u 0x73Bu, 0, 0, 0, 0, 10, 43, 3, 617
#define PPUTLIMPL_UDEC_1850u 0x73Au, 0, 0, 0, 0, 10, 43, 2, 5, 5, 37
#define PPUTLIMPL_UDEC_1849u 0x739u, 0, 0, 0, 0, 10, 43, 43, 43
#define PPUTLIMPL_UDEC_1848u 0x738u, 0, 0, 0, 0, 10, 42, 2, 2, 2, 3, 7, 11
#define PPUTLIMPL_UDEC_1847u 0x737u, 0, 0, 0, 0, 10, 42,
#define PPUTLIMPL_UDEC_1846u 0x736u, 0, 0, 0, 0, 10, 42, 2, 13, 71
#define PPUTLIMPL_UDEC_1845u 0x735u, 0, 0, 0, 0, 10, 42, 3, 3, 5, 41
#define PPUTLIMPL_UDEC_1844u 0x734u, 0, 0, 0, 0, 10, 42, 2, 2, 461
#define PPUTLIMPL_UDEC_1843u 0x733u, 0, 0, 0, 0, 10, 42, 19, 97
#define PPUTLIMPL_UDEC_1842u 0x732u, 0, 0, 0, 0, 10, 42, 2, 3, 307
#define PPUTLIMPL_UDEC_1841u 0x731u, 0, 0, 0, 0, 10, 42, 7, 263
#define PPUTLIMPL_UDEC_1840u 0x730u, 0, 0, 0, 0, 10, 42, 2, 2, 2, 2, 5, 23
#define PPUTLIMPL_UDEC_1839u 0x72Fu, 0, 0, 0, 0, 10, 42, 3, 613
#define PPUTLIMPL_UDEC_1838u 0x72Eu, 0, 0, 0, 0, 10, 42, 2, 919
#define PPUTLIMPL_UDEC_1837u 0x72Du, 0, 0, 0, 0, 10, 42, 11, 167
#define PPUTLIMPL_UDEC_1836u 0x72Cu, 0, 0, 0, 0, 10, 42, 2, 2, 3, 3, 3, 17
#define PPUTLIMPL_UDEC_1835u 0x72Bu, 0, 0, 0, 0, 10, 42, 5, 367
#define PPUTLIMPL_UDEC_1834u 0x72Au, 0, 0, 0, 0, 10, 42, 2, 7, 131
#define PPUTLIMPL_UDEC_1833u 0x729u, 0, 0, 0, 0, 10, 42, 3, 13, 47
#define PPUTLIMPL_UDEC_1832u 0x728u, 0, 0, 0, 0, 10, 42, 2, 2, 2, 229
#define PPUTLIMPL_UDEC_1831u 0x727u, 0, 0, 0, 0, 10, 42,
#define PPUTLIMPL_UDEC_1830u 0x726u, 0, 0, 0, 0, 10, 42, 2, 3, 5, 61
#define PPUTLIMPL_UDEC_1829u 0x725u, 0, 0, 0, 0, 10, 42, 31, 59
#define PPUTLIMPL_UDEC_1828u 0x724u, 0, 0, 0, 0, 10, 42, 2, 2, 457
#define PPUTLIMPL_UDEC_1827u 0x723u, 0, 0, 0, 0, 10, 42, 3, 3, 7, 29
#define PPUTLIMPL_UDEC_1826u 0x722u, 0, 0, 0, 0, 10, 42, 2, 11, 83
#define PPUTLIMPL_UDEC_1825u 0x721u, 0, 0, 0, 0, 10, 42, 5, 5, 73
#define PPUTLIMPL_UDEC_1824u 0x720u, 0, 0, 0, 0, 10, 42, 2, 2, 2, 2, 2, 3, 19
#define PPUTLIMPL_UDEC_1823u 0x71Fu, 0, 0, 0, 0, 10, 42,
#define PPUTLIMPL_UDEC_1822u 0x71Eu, 0, 0, 0, 0, 10, 42, 2, 911
#define PPUTLIMPL_UDEC_1821u 0x71Du, 0, 0, 0, 0, 10, 42, 3, 607
#define PPUTLIMPL_UDEC_1820u 0x71Cu, 0, 0, 0, 0, 10, 42, 2, 2, 5, 7, 13
#define PPUTLIMPL_UDEC_1819u 0x71Bu, 0, 0, 0, 0, 10, 42, 17, 107
#define PPUTLIMPL_UDEC_1818u 0x71Au, 0, 0, 0, 0, 10, 42, 2, 3, 3, 101
#define PPUTLIMPL_UDEC_1817u 0x719u, 0, 0, 0, 0, 10, 42, 23, 79
#define PPUTLIMPL_UDEC_1816u 0x718u, 0, 0, 0, 0, 10, 42, 2, 2, 2, 227
#define PPUTLIMPL_UDEC_1815u 0x717u, 0, 0, 0, 0, 10, 42, 3, 5, 11, 11
#define PPUTLIMPL_UDEC_1814u 0x716u, 0, 0, 0, 0, 10, 42, 2, 907
#define PPUTLIMPL_UDEC_1813u 0x715u, 0, 0, 0, 0, 10, 42, 7, 7, 37
#define PPUTLIMPL_UDEC_1812u 0x714u, 0, 0, 0, 0, 10, 42, 2, 2, 3, 151
#define PPUTLIMPL_UDEC_1811u 0x713u, 0, 0, 0, 0, 10, 42,
#define PPUTLIMPL_UDEC_1810u 0x712u, 0, 0, 0, 0, 10, 42, 2, 5, 181
#define PPUTLIMPL_UDEC_1809u 0x711u, 0, 0, 0, 0, 10, 42, 3, 3, 3, 67
#define PPUTLIMPL_UDEC_1808u 0x710u, 0, 0, 0, 0, 10, 42, 2, 2, 2, 2, 113
#define PPUTLIMPL_UDEC_1807u 0x70Fu, 0, 0, 0, 0, 10, 42, 13, 139
#define PPUTLIMPL_UDEC_1806u 0x70Eu, 0, 0, 0, 0, 10, 42, 2, 3, 7, 43
#define PPUTLIMPL_UDEC_1805u 0x70Du, 0, 0, 0, 0, 10, 42, 5, 19, 19
#define PPUTLIMPL_UDEC_1804u 0x70Cu, 0, 0, 0, 0, 10, 42, 2, 2, 11, 41
#define PPUTLIMPL_UDEC_1803u 0x70Bu, 0, 0, 0, 0, 10, 42, 3, 601
#define PPUTLIMPL_UDEC_1802u 0x70Au, 0, 0, 0, 0, 10, 42, 2, 17, 53
#define PPUTLIMPL_UDEC_1801u 0x709u, 0, 0, 0, 0, 10, 42,
#define PPUTLIMPL_UDEC_1800u 0x708u, 0, 0, 0, 0, 10, 42, 2, 2, 2, 3, 3, 5, 5
#define PPUTLIMPL_UDEC_1799u 0x707u, 0, 0, 0, 0, 10, 42, 7, 257
#define PPUTLIMPL_UDEC_1798u 0x706u, 0, 0, 0, 0, 10, 42, 2, 29, 31
#define PPUTLIMPL_UDEC_1797u 0x705u, 0, 0, 0, 0, 10, 42, 3, 599
#define PPUTLIMPL_UDEC_1796u 0x704u, 0, 0, 0, 0, 10, 42, 2, 2, 449
#define PPUTLIMPL_UDEC_1795u 0x703u, 0, 0, 0, 0, 10, 42, 5, 359
#define PPUTLIMPL_UDEC_1794u 0x702u, 0, 0, 0, 0, 10, 42, 2, 3, 13, 23
#define PPUTLIMPL_UDEC_1793u 0x701u, 0, 0, 0, 0, 10, 42, 11, 163
#define PPUTLIMPL_UDEC_1792u 0x700u, 0, 0, 0, 0, 10, 42, 2, 2, 2, 2, 2, 2, 2, 2, 7
#define PPUTLIMPL_UDEC_1791u 0x6FFu, 0, 0, 0, 0, 10, 42, 3, 3, 199
#define PPUTLIMPL_UDEC_1790u 0x6FEu, 0, 0, 0, 0, 10, 42, 2, 5, 179
#define PPUTLIMPL_UDEC_1789u 0x6FDu, 0, 0, 0, 0, 10, 42,
#define PPUTLIMPL_UDEC_1788u 0x6FCu, 0, 0, 0, 0, 10, 42, 2, 2, 3, 149
#define PPUTLIMPL_UDEC_1787u 0x6FBu, 0, 0, 0, 0, 10, 42,
#define PPUTLIMPL_UDEC_1786u 0x6FAu, 0, 0, 0, 0, 10, 42, 2, 19, 47
#define PPUTLIMPL_UDEC_1785u 0x6F9u, 0, 0, 0, 0, 10, 42, 3, 5, 7, 17
#define PPUTLIMPL_UDEC_1784u 0x6F8u, 0, 0, 0, 0, 10, 42, 2, 2, 2, 223
#define PPUTLIMPL_UDEC_1783u 0x6F7u, 0, 0, 0, 0, 10, 42,
#define PPUTLIMPL_UDEC_1782u 0x6F6u, 0, 0, 0, 0, 10, 42, 2, 3, 3, 3, 3, 11
#define PPUTLIMPL_UDEC_1781u 0x6F5u, 0, 0, 0, 0, 10, 42, 13, 137
#define PPUTLIMPL_UDEC_1780u 0x6F4u, 0, 0, 0, 0, 10, 42, 2, 2, 5, 89
#define PPUTLIMPL_UDEC_1779u 0x6F3u, 0, 0, 0, 0, 10, 42, 3, 593
#define PPUTLIMPL_UDEC_1778u 0x6F2u, 0, 0, 0, 0, 10, 42, 2, 7, 127
#define PPUTLIMPL_UDEC_1777u 0x6F1u, 0, 0, 0, 0, 10, 42,
#define PPUTLIMPL_UDEC_1776u 0x6F0u, 0, 0, 0, 0, 10, 42, 2, 2, 2, 2, 3, 37
#define PPUTLIMPL_UDEC_1775u 0x6EFu, 0, 0, 0, 0, 10, 42, 5, 5, 71
#define PPUTLIMPL_UDEC_1774u 0x6EEu, 0, 0, 0, 0, 10, 42, 2, 887
#define PPUTLIMPL_UDEC_1773u 0x6EDu, 0, 0, 0, 0, 10, 42, 3, 3, 197
#define PPUTLIMPL_UDEC_1772u 0x6ECu, 0, 0, 0, 0, 10, 42, 2, 2, 443
#define PPUTLIMPL_UDEC_1771u 0x6EBu, 0, 0, 0, 0, 10, 42, 7, 11, 23
#define PPUTLIMPL_UDEC_1770u 0x6EAu, 0, 0, 0, 0, 10, 42, 2, 3, 5, 59
#define PPUTLIMPL_UDEC_1769u 0x6E9u, 0, 0, 0, 0, 10, 42, 29, 61
#define PPUTLIMPL_UDEC_1768u 0x6E8u, 0, 0, 0, 0, 10, 42, 2, 2, 2, 13, 17
#define PPUTLIMPL_UDEC_1767u 0x6E7u, 0, 0, 0, 0, 10, 42, 3, 19, 31
#define PPUTLIMPL_UDEC_1766u 0x6E6u, 0, 0, 0, 0, 10, 42, 2, 883
#define PPUTLIMPL_UDEC_1765u 0x6E5u, 0, 0, 0, 0, 10, 42, 5, 353
#define PPUTLIMPL_UDEC_1764u 0x6E4u, 0, 0, 0, 0, 10, 42, 2, 2, 3, 3, 7, 7
#define PPUTLIMPL_UDEC_1763u 0x6E3u, 0, 0, 0, 0, 10, 41, 41, 43
#define PPUTLIMPL_UDEC_1762u 0x6E2u, 0, 0, 0, 0, 10, 41, 2, 881
#define PPUTLIMPL_UDEC_1761u 0x6E1u, 0, 0, 0, 0, 10, 41, 3, 587
#define PPUTLIMPL_UDEC_1760u 0x6E0u, 0, 0, 0, 0, 10, 41, 2, 2, 2, 2, 2, 5, 11
#define PPUTLIMPL_UDEC_1759u 0x6DFu, 0, 0, 0, 0, 10, 41,
#define PPUTLIMPL_UDEC_1758u 0x6DEu, 0, 0, 0, 0, 10, 41, 2, 3, 293
#define PPUTLIMPL_UDEC_1757u 0x6DDu, 0, 0, 0, 0, 10, 41, 7, 251
#define PPUTLIMPL_UDEC_1756u 0x6DCu, 0, 0, 0, 0, 10, 41, 2, 2, 439
#define PPUTLIMPL_UDEC_1755u 0x6DBu, 0, 0, 0, 0, 10, 41, 3, 3, 3, 5, 13
#define PPUTLIMPL_UDEC_1754u 0x6DAu, 0, 0, 0, 0, 10, 41, 2, 877
#define PPUTLIMPL_UDEC_1753u 0x6D9u, 0, 0, 0, 0, 10, 41,
#define PPUTLIMPL_UDEC_1752u 0x6D8u, 0, 0, 0, 0, 10, 41, 2, 2, 2, 3, 73
#define PPUTLIMPL_UDEC_1751u 0x6D7u, 0, 0, 0, 0, 10, 41, 17, 103
#define PPUTLIMPL_UDEC_1750u 0x6D6u, 0, 0, 0, 0, 10, 41, 2, 5, 5, 5, 7
#define PPUTLIMPL_UDEC_1749u 0x6D5u, 0, 0, 0, 0, 10, 41, 3, 11, 53
#define PPUTLIMPL_UDEC_1748u 0x6D4u, 0, 0, 0, 0, 10, 41, 2, 2, 19, 23
#define PPUTLIMPL_UDEC_1747u 0x6D3u, 0, 0, 0, 0, 10, 41,
#define PPUTLIMPL_UDEC_1746u 0x6D2u, 0, 0, 0, 0, 10, 41, 2, 3, 3, 97
#define PPUTLIMPL_UDEC_1745u 0x6D1u, 0, 0, 0, 0, 10, 41, 5, 349
#define PPUTLIMPL_UDEC_1744u 0x6D0u, 0, 0, 0, 0, 10, 41, 2, 2, 2, 2, 109
#define PPUTLIMPL_UDEC_1743u 0x6CFu, 0, 0, 0, 0, 10, 41, 3, 7, 83
#define PPUTLIMPL_UDEC_1742u 0x6CEu, 0, 0, 0, 0, 10, 41, 2, 13, 67
#define PPUTLIMPL_UDEC_1741u 0x6CDu, 0, 0, 0, 0, 10, 41,
#define PPUTLIMPL_UDEC_1740u 0x6CCu, 0, 0, 0, 0, 10, 41, 2, 2, 3, 5, 29
#define PPUTLIMPL_UDEC_1739u 0x6CBu, 0, 0, 0, 0, 10, 41, 37, 47
#define PPUTLIMPL_UDEC_1738u 0x6CAu, 0, 0, 0, 0, 10, 41, 2, 11, 79
#define PPUTLIMPL_UDEC_1737u 0x6C9u, 0, 0, 0, 0, 10, 41, 3, 3, 193
#define PPUTLIMPL_UDEC_1736u 0x6C8u, 0, 0, 0, 0, 10, 41, 2, 2, 2, 7, 31
#define PPUTLIMPL_UDEC_1735u 0x6C7u, 0, 0, 0, 0, 10, 41, 5, 347
#define PPUTLIMPL_UDEC_1734u 0x6C6u, 0, 0, 0, 0, 10, 41, 2, 3, 17, 17
#define PPUTLIMPL_UDEC_1733u 0x6C5u, 0, 0, 0, 0, 10, 41,
#define PPUTLIMPL_UDEC_1732u 0x6C4u, 0, 0, 0, 0, 10, 41, 2, 2, 433
#define PPUTLIMPL_UDEC_1731u 0x6C3u, 0, 0, 0, 0, 10, 41, 3, 577
#define PPUTLIMPL_UDEC_1730u 0x6C2u, 0, 0, 0, 0, 10, 41, 2, 5, 173
#define PPUTLIMPL_UDEC_1729u 0x6C1u, 0, 0, 0, 0, 10, 41, 7, 13, 19
#define PPUTLIMPL_UDEC_1728u 0x6C0u, 0, 0, 0, 0, 10, 41, 2, 2, 2, 2, 2, 2, 3, 3, 3
#define PPUTLIMPL_UDEC_1727u 0x6BFu, 0, 0, 0, 0, 10, 41, 11, 157
#define PPUTLIMPL_UDEC_1726u 0x6BEu, 0, 0, 0, 0, 10, 41, 2, 863
#define PPUTLIMPL_UDEC_1725u 0x6BDu, 0, 0, 0, 0, 10, 41, 3, 5, 5, 23
#define PPUTLIMPL_UDEC_1724u 0x6BCu, 0, 0, 0, 0, 10, 41, 2, 2, 431
#define PPUTLIMPL_UDEC_1723u 0x6BBu, 0, 0, 0, 0, 10, 41,
#define PPUTLIMPL_UDEC_1722u 0x6BAu, 0, 0, 0, 0, 10, 41, 2, 3, 7, 41
#define PPUTLIMPL_UDEC_1721u 0x6B9u, 0, 0, 0, 0, 10, 41,
#define PPUTLIMPL_UDEC_1720u 0x6B8u, 0, 0, 0, 0, 10, 41, 2, 2, 2, 5, 43
#define PPUTLIMPL_UDEC_1719u 0x6B7u, 0, 0, 0, 0, 10, 41, 3, 3, 191
#define PPUTLIMPL_UDEC_1718u 0x6B6u, 0, 0, 0, 0, 10, 41, 2, 859
#define PPUTLIMPL_UDEC_1717u 0x6B5u, 0, 0, 0, 0, 10, 41, 17, 101
#define PPUTLIMPL_UDEC_1716u 0x6B4u, 0, 0, 0, 0, 10, 41, 2, 2, 3, 11, 13
#define PPUTLIMPL_UDEC_1715u 0x6B3u, 0, 0, 0, 0, 10, 41, 5, 7, 7, 7
#define PPUTLIMPL_UDEC_1714u 0x6B2u, 0, 0, 0, 0, 10, 41, 2, 857
#define PPUTLIMPL_UDEC_1713u 0x6B1u, 0, 0, 0, 0, 10, 41, 3, 571
#define PPUTLIMPL_UDEC_1712u 0x6B0u, 0, 0, 0, 0, 10, 41, 2, 2, 2, 2, 107
#define PPUTLIMPL_UDEC_1711u 0x6AFu, 0, 0, 0, 0, 10, 41, 29, 59
#define PPUTLIMPL_UDEC_1710u 0x6AEu, 0, 0, 0, 0, 10, 41, 2, 3, 3, 5, 19
#define PPUTLIMPL_UDEC_1709u 0x6ADu, 0, 0, 0, 0, 10, 41,
#define PPUTLIMPL_UDEC_1708u 0x6ACu, 0, 0, 0, 0, 10, 41, 2, 2, 7, 61
#define PPUTLIMPL_UDEC_1707u 0x6ABu, 0, 0, 0, 0, 10, 41, 3, 569
#define PPUTLIMPL_UDEC_1706u 0x6AAu, 0, 0, 0, 0, 10, 41, 2, 853
#define PPUTLIMPL_UDEC_1705u 0x6A9u, 0, 0, 0, 0, 10, 41, 5, 11, 31
#define PPUTLIMPL_UDEC_1704u 0x6A8u, 0, 0, 0, 0, 10, 41, 2, 2, 2, 3, 71
#define PPUTLIMPL_UDEC_1703u 0x6A7u, 0, 0, 0, 0, 10, 41, 13, 131
#define PPUTLIMPL_UDEC_1702u 0x6A6u, 0, 0, 0, 0, 10, 41, 2, 23, 37
#define PPUTLIMPL_UDEC_1701u 0x6A5u, 0, 0, 0, 0, 10, 41, 3, 3, 3, 3, 3, 7
#define PPUTLIMPL_UDEC_1700u 0x6A4u, 0, 0, 0, 0, 10, 41, 2, 2, 5, 5, 17
#define PPUTLIMPL_UDEC_1699u 0x6A3u, 0, 0, 0, 0, 10, 41,
#define PPUTLIMPL_UDEC_1698u 0x6A2u, 0, 0, 0, 0, 10, 41, 2, 3, 283
#define PPUTLIMPL_UDEC_1697u 0x6A1u, 0, 0, 0, 0, 10, 41,
#define PPUTLIMPL_UDEC_1696u 0x6A0u, 0, 0, 0, 0, 10, 41, 2, 2, 2, 2, 2, 53
#define PPUTLIMPL_UDEC_1695u 0x69Fu, 0, 0, 0, 0, 10, 41, 3, 5, 113
#define PPUTLIMPL_UDEC_1694u 0x69Eu, 0, 0, 0, 0, 10, 41, 2, 7, 11, 11
#define PPUTLIMPL_UDEC_1693u 0x69Du, 0, 0, 0, 0, 10, 41,
#define PPUTLIMPL_UDEC_1692u 0x69Cu, 0, 0, 0, 0, 10, 41, 2, 2, 3, 3, 47
#define PPUTLIMPL_UDEC_1691u 0x69Bu, 0, 0, 0, 0, 10, 41, 19, 89
#define PPUTLIMPL_UDEC_1690u 0x69Au, 0, 0, 0, 0, 10, 41, 2, 5, 13, 13
#define PPUTLIMPL_UDEC_1689u 0x699u, 0, 0, 0, 0, 10, 41, 3, 563
#define PPUTLIMPL_UDEC_1688u 0x698u, 0, 0, 0, 0, 10, 41, 2, 2, 2, 211
#define PPUTLIMPL_UDEC_1687u 0x697u, 0, 0, 0, 0, 10, 41, 7, 241
#define PPUTLIMPL_UDEC_1686u 0x696u, 0, 0, 0, 0, 10, 41, 2, 3, 281
#define PPUTLIMPL_UDEC_1685u 0x695u, 0, 0, 0, 0, 10, 41, 5, 337
#define PPUTLIMPL_UDEC_1684u 0x694u, 0, 0, 0, 0, 10, 41, 2, 2, 421
#define PPUTLIMPL_UDEC_1683u 0x693u, 0, 0, 0, 0, 10, 41, 3, 3, 11, 17
#define PPUTLIMPL_UDEC_1682u 0x692u, 0, 0, 0, 0, 10, 41, 2, 29, 29
#define PPUTLIMPL_UDEC_1681u 0x691u, 0, 0, 0, 0, 10, 41, 41, 41
#define PPUTLIMPL_UDEC_1680u 0x690u, 0, 0, 0, 0, 10, 40, 2, 2, 2, 2, 3, 5, 7
#define PPUTLIMPL_UDEC_1679u 0x68Fu, 0, 0, 0, 0, 10, 40, 23, 73
#define PPUTLIMPL_UDEC_1678u 0x68Eu, 0, 0, 0, 0, 10, 40, 2, 839
#define PPUTLIMPL_UDEC_1677u 0x68Du, 0, 0, 0, 0, 10, 40, 3, 13, 43
#define PPUTLIMPL_UDEC_1676u 0x68Cu, 0, 0, 0, 0, 10, 40, 2, 2, 419
#define PPUTLIMPL_UDEC_1675u 0x68Bu, 0, 0, 0, 0, 10, 40, 5, 5, 67
#define PPUTLIMPL_UDEC_1674u 0x68Au, 0, 0, 0, 0, 10, 40, 2, 3, 3, 3, 31
#define PPUTLIMPL_UDEC_1673u 0x689u, 0, 0, 0, 0, 10, 40, 7, 239
#define PPUTLIMPL_UDEC_1672u 0x688u, 0, 0, 0, 0, 10, 40, 2, 2, 2, 11, 19
#define PPUTLIMPL_UDEC_1671u 0x687u, 0, 0, 0, 0, 10, 40, 3, 557
#define PPUTLIMPL_UDEC_1670u 0x686u, 0, 0, 0, 0, 10, 40, 2, 5, 167
#define PPUTLIMPL_UDEC_1669u 0x685u, 0, 0, 0, 0, 10, 40,
#define PPUTLIMPL_UDEC_1668u 0x684u, 0, 0, 0, 0, 10, 40, 2, 2, 3, 139
#define PPUTLIMPL_UDEC_1667u 0x683u, 0, 0, 0, 0, 10, 40,
#define PPUTLIMPL_UDEC_1666u 0x682u, 0, 0, 0, 0, 10, 40, 2, 7, 7, 17
#define PPUTLIMPL_UDEC_1665u 0x681u, 0, 0, 0, 0, 10, 40, 3, 3, 5, 37
#define PPUTLIMPL_UDEC_1664u 0x680u, 0, 0, 0, 0, 10, 40, 2, 2, 2, 2, 2, 2, 2, 13
#define PPUTLIMPL_UDEC_1663u 0x67Fu, 0, 0, 0, 0, 10, 40,
#define PPUTLIMPL_UDEC_1662u 0x67Eu, 0, 0, 0, 0, 10, 40, 2, 3, 277
#define PPUTLIMPL_UDEC_1661u 0x67Du, 0, 0, 0, 0, 10, 40, 11, 151
#define PPUTLIMPL_UDEC_1660u 0x67Cu, 0, 0, 0, 0, 10, 40, 2, 2, 5, 83
#define PPUTLIMPL_UDEC_1659u 0x67Bu, 0, 0, 0, 0, 10, 40, 3, 7, 79
#define PPUTLIMPL_UDEC_1658u 0x67Au, 0, 0, 0, 0, 10, 40, 2, 829
#define PPUTLIMPL_UDEC_1657u 0x679u, 0, 0, 0, 0, 10, 40,
#define PPUTLIMPL_UDEC_1656u 0x678u, 0, 0, 0, 0, 10, 40, 2, 2, 2, 3, 3, 23
#define PPUTLIMPL_UDEC_1655u 0x677u, 0, 0, 0, 0, 10, 40, 5, 331
#define PPUTLIMPL_UDEC_1654u 0x676u, 0, 0, 0, 0, 10, 40, 2, 827
#define PPUTLIMPL_UDEC_1653u 0x675u, 0, 0, 0, 0, 10, 40, 3, 19, 29
#define PPUTLIMPL_UDEC_1652u 0x674u, 0, 0, 0, 0, 10, 40, 2, 2, 7, 59
#define PPUTLIMPL_UDEC_1651u 0x673u, 0, 0, 0, 0, 10, 40, 13, 127
#define PPUTLIMPL_UDEC_1650u 0x672u, 0, 0, 0, 0, 10, 40, 2, 3, 5, 5, 11
#define PPUTLIMPL_UDEC_1649u 0x671u, 0, 0, 0, 0, 10, 40, 17, 97
#define PPUTLIMPL_UDEC_1648u 0x670u, 0, 0, 0, 0, 10, 40, 2, 2, 2, 2, 103
#define PPUTLIMPL_UDEC_1647u 0x66Fu, 0, 0, 0, 0, 10, 40, 3, 3, 3, 61
#define PPUTLIMPL_UDEC_1646u 0x66Eu, 0, 0, 0, 0, 10, 40, 2, 823
#define PPUTLIMPL_UDEC_1645u 0x66Du, 0, 0, 0, 0, 10, 40, 5, 7, 47
#define PPUTLIMPL_UDEC_1644u 0x66Cu, 0, 0, 0, 0, 10, 40, 2, 2, 3, 137
#define PPUTLIMPL_UDEC_1643u 0x66Bu, 0, 0, 0, 0, 10, 40, 31, 53
#define PPUTLIMPL_UDEC_1642u 0x66Au, 0, 0, 0, 0, 10, 40, 2, 821
#define PPUTLIMPL_UDEC_1641u 0x669u, 0, 0, 0, 0, 10, 40, 3, 547
#define PPUTLIMPL_UDEC_1640u 0x668u, 0, 0, 0, 0, 10, 40, 2, 2, 2, 5, 41
#define PPUTLIMPL_UDEC_1639u 0x667u, 0, 0, 0, 0, 10, 40, 11, 149
#define PPUTLIMPL_UDEC_1638u 0x666u, 0, 0, 0, 0, 10, 40, 2, 3, 3, 7, 13
#define PPUTLIMPL_UDEC_1637u 0x665u, 0, 0, 0, 0, 10, 40,
#define PPUTLIMPL_UDEC_1636u 0x664u, 0, 0, 0, 0, 10, 40, 2, 2, 409
#define PPUTLIMPL_UDEC_1635u 0x663u, 0, 0, 0, 0, 10, 40, 3, 5, 109
#define PPUTLIMPL_UDEC_1634u 0x662u, 0, 0, 0, 0, 10, 40, 2, 19, 43
#define PPUTLIMPL_UDEC_1633u 0x661u, 0, 0, 0, 0, 10, 40, 23, 71
#define PPUTLIMPL_UDEC_1632u 0x660u, 0, 0, 0, 0, 10, 40, 2, 2, 2, 2, 2, 3, 17
#define PPUTLIMPL_UDEC_1631u 0x65Fu, 0, 0, 0, 0, 10, 40, 7, 233
#define PPUTLIMPL_UDEC_1630u 0x65Eu, 0, 0, 0, 0, 10, 40, 2, 5, 163
#define PPUTLIMPL_UDEC_1629u 0x65Du, 0, 0, 0, 0, 10, 40, 3, 3, 181
#define PPUTLIMPL_UDEC_1628u 0x65Cu, 0, 0, 0, 0, 10, 40, 2, 2, 11, 37
#define PPUTLIMPL_UDEC_1627u 0x65Bu, 0, 0, 0, 0, 10, 40,
#define PPUTLIMPL_UDEC_1626u 0x65Au, 0, 0, 0, 0, 10, 40, 2, 3, 271
#define PPUTLIMPL_UDEC_1625u 0x659u, 0, 0, 0, 0, 10, 40, 5, 5, 5, 13
#define PPUTLIMPL_UDEC_1624u 0x658u, 0, 0, 0, 0, 10, 40, 2, 2, 2, 7, 29
#define PPUTLIMPL_UDEC_1623u 0x657u, 0, 0, 0, 0, 10, 40, 3, 541
#define PPUTLIMPL_UDEC_1622u 0x656u, 0, 0, 0, 0, 10, 40, 2, 811
#define PPUTLIMPL_UDEC_1621u 0x655u, 0, 0, 0, 0, 10, 40,
#define PPUTLIMPL_UDEC_1620u 0x654u, 0, 0, 0, 0, 10, 40, 2, 2, 3, 3, 3, 3, 5
#define PPUTLIMPL_UDEC_1619u 0x653u, 0, 0, 0, 0, 10, 40,
#define PPUTLIMPL_UDEC_1618u 0x652u, 0, 0, 0, 0, 10, 40, 2, 809
#define PPUTLIMPL_UDEC_1617u 0x651u, 0, 0, 0, 0, 10, 40, 3, 7, 7, 11
#define PPUTLIMPL_UDEC_1616u 0x650u, 0, 0, 0, 0, 10, 40, 2, 2, 2, 2, 101
#define PPUTLIMPL_UDEC_1615u 0x64Fu, 0, 0, 0, 0, 10, 40, 5, 17, 19
#define PPUTLIMPL_UDEC_1614u 0x64Eu, 0, 0, 0, 0, 10, 40, 2, 3, 269
#define PPUTLIMPL_UDEC_1613u 0x64Du, 0, 0, 0, 0, 10, 40,
#define PPUTLIMPL_UDEC_1612u 0x64Cu, 0, 0, 0, 0, 10, 40, 2, 2, 13, 31
#define PPUTLIMPL_UDEC_1611u 0x64Bu, 0, 0, 0, 0, 10, 40, 3, 3, 179
#define PPUTLIMPL_UDEC_1610u 0x64Au, 0, 0, 0, 0, 10, 40, 2, 5, 7, 23
#define PPUTLIMPL_UDEC_1609u 0x649u, 0, 0, 0, 0, 10, 40,
#define PPUTLIMPL_UDEC_1608u 0x648u, 0, 0, 0, 0, 10, 40, 2, 2, 2, 3, 67
#define PPUTLIMPL_UDEC_1607u 0x647u, 0, 0, 0, 0, 10, 40,
#define PPUTLIMPL_UDEC_1606u 0x646u, 0, 0, 0, 0, 10, 40, 2, 11, 73
#define PPUTLIMPL_UDEC_1605u 0x645u, 0, 0, 0, 0, 10, 40, 3, 5, 107
#define PPUTLIMPL_UDEC_1604u 0x644u, 0, 0, 0, 0, 10, 40, 2, 2, 401
#define PPUTLIMPL_UDEC_1603u 0x643u, 0, 0, 0, 0, 10, 40, 7, 229
#define PPUTLIMPL_UDEC_1602u 0x642u, 0, 0, 0, 0, 10, 40, 2, 3, 3, 89
#define PPUTLIMPL_UDEC_1601u 0x641u, 0, 0, 0, 0, 10, 40,
#define PPUTLIMPL_UDEC_1600u 0x640u, 0, 0, 0, 0, 10, 40, 2, 2, 2, 2, 2, 2, 5, 5
#define PPUTLIMPL_UDEC_1599u 0x63Fu, 0, 0, 0, 0, 10, 39, 3, 13, 41
#define PPUTLIMPL_UDEC_1598u 0x63Eu, 0, 0, 0, 0, 10, 39, 2, 17, 47
#define PPUTLIMPL_UDEC_1597u 0x63Du, 0, 0, 0, 0, 10, 39,
#define PPUTLIMPL_UDEC_1596u 0x63Cu, 0, 0, 0, 0, 10, 39, 2, 2, 3, 7, 19
#define PPUTLIMPL_UDEC_1595u 0x63Bu, 0, 0, 0, 0, 10, 39, 5, 11, 29
#define PPUTLIMPL_UDEC_1594u 0x63Au, 0, 0, 0, 0, 10, 39, 2, 797
#define PPUTLIMPL_UDEC_1593u 0x639u, 0, 0, 0, 0, 10, 39, 3, 3, 3, 59
#define PPUTLIMPL_UDEC_1592u 0x638u, 0, 0, 0, 0, 10, 39, 2, 2, 2, 199
#define PPUTLIMPL_UDEC_1591u 0x637u, 0, 0, 0, 0, 10, 39, 37, 43
#define PPUTLIMPL_UDEC_1590u 0x636u, 0, 0, 0, 0, 10, 39, 2, 3, 5, 53
#define PPUTLIMPL_UDEC_1589u 0x635u, 0, 0, 0, 0, 10, 39, 7, 227
#define PPUTLIMPL_UDEC_1588u 0x634u, 0, 0, 0, 0, 10, 39, 2, 2, 397
#define PPUTLIMPL_UDEC_1587u 0x633u, 0, 0, 0, 0, 10, 39, 3, 23, 23
#define PPUTLIMPL_UDEC_1586u 0x632u, 0, 0, 0, 0, 10, 39, 2, 13, 61
#define PPUTLIMPL_UDEC_1585u 0x631u, 0, 0, 0, 0, 10, 39, 5, 317
#define PPUTLIMPL_UDEC_1584u 0x630u, 0, 0, 0, 0, 10, 39, 2, 2, 2, 2, 3, 3, 11
#define PPUTLIMPL_UDEC_1583u 0x62Fu, 0, 0, 0, 0, 10, 39,
#define PPUTLIMPL_UDEC_1582u 0x62Eu, 0, 0, 0, 0, 10, 39, 2, 7, 113
#define PPUTLIMPL_UDEC_1581u 0x62Du, 0, 0, 0, 0, 10, 39, 3, 17, 31
#define PPUTLIMPL_UDEC_1580u 0x62Cu, 0, 0, 0, 0, 10, 39, 2, 2, 5, 79
#define PPUTLIMPL_UDEC_1579u 0x62Bu, 0, 0, 0, 0, 10, 39,
#define PPUTLIMPL_UDEC_1578u 0x62Au, 0, 0, 0, 0, 10, 39, 2, 3, 263
#define PPUTLIMPL_UDEC_1577u 0x629u, 0, 0, 0, 0, 10, 39, 19, 83
#define PPUTLIMPL_UDEC_1576u 0x628u, 0, 0, 0, 0, 10, 39, 2, 2, 2, 197
#define PPUTLIMPL_UDEC_1575u 0x627u, 0, 0, 0, 0, 10, 39, 3, 3, 5, 5, 7
#define PPUTLIMPL_UDEC_1574u 0x626u, 0, 0, 0, 0, 10, 39, 2, 787
#define PPUTLIMPL_UDEC_1573u 0x625u, 0, 0, 0, 0, 10, 39, 11, 11, 13
#define PPUTLIMPL_UDEC_1572u 0x624u, 0, 0, 0, 0, 10, 39, 2, 2, 3, 131
#define PPUTLIMPL_UDEC_1571u 0x623u, 0, 0, 0, 0, 10, 39,
#define PPUTLIMPL_UDEC_1570u 0x622u, 0, 0, 0, 0, 10, 39, 2, 5, 157
#define PPUTLIMPL_UDEC_1569u 0x621u, 0, 0, 0, 0, 10, 39, 3, 523
#define PPUTLIMPL_UDEC_1568u 0x620u, 0, 0, 0, 0, 10, 39, 2, 2, 2, 2, 2, 7, 7
#define PPUTLIMPL_UDEC_1567u 0x61Fu, 0, 0, 0, 0, 10, 39,
#define PPUTLIMPL_UDEC_1566u 0x61Eu, 0, 0, 0, 0, 10, 39, 2, 3, 3, 3, 29
#define PPUTLIMPL_UDEC_1565u 0x61Du, 0, 0, 0, 0, 10, 39, 5, 313
#define PPUTLIMPL_UDEC_1564u 0x61Cu, 0, 0, 0, 0, 10, 39, 2, 2, 17, 23
#define PPUTLIMPL_UDEC_1563u 0x61Bu, 0, 0, 0, 0, 10, 39, 3, 521
#define PPUTLIMPL_UDEC_1562u 0x61Au, 0, 0, 0, 0, 10, 39, 2, 11, 71
#define PPUTLIMPL_UDEC_1561u 0x619u, 0, 0, 0, 0, 10, 39, 7, 223
#define PPUTLIMPL_UDEC_1560u 0x618u, 0, 0, 0, 0, 10, 39, 2, 2, 2, 3, 5, 13
#define PPUTLIMPL_UDEC_1559u 0x617u, 0, 0, 0, 0, 10, 39,
#define PPUTLIMPL_UDEC_1558u 0x616u, 0, 0, 0, 0, 10, 39, 2, 19, 41
#define PPUTLIMPL_UDEC_1557u 0x615u, 0, 0, 0, 0, 10, 39, 3, 3, 173
#define PPUTLIMPL_UDEC_1556u 0x614u, 0, 0, 0, 0, 10, 39, 2, 2, 389
#define PPUTLIMPL_UDEC_1555u 0x613u, 0, 0, 0, 0, 10, 39, 5, 311
#define PPUTLIMPL_UDEC_1554u 0x612u, 0, 0, 0, 0, 10, 39, 2, 3, 7, 37
#define PPUTLIMPL_UDEC_1553u 0x611u, 0, 0, 0, 0, 10, 39,
#define PPUTLIMPL_UDEC_1552u 0x610u, 0, 0, 0, 0, 10, 39, 2, 2, 2, 2, 97
#define PPUTLIMPL_UDEC_1551u 0x60Fu, 0, 0, 0, 0, 10, 39, 3, 11, 47
#define PPUTLIMPL_UDEC_1550u 0x60Eu, 0, 0, 0, 0, 10, 39, 2, 5, 5, 31
#define PPUTLIMPL_UDEC_1549u 0x60Du, 0, 0, 0, 0, 10, 39,
#define PPUTLIMPL_UDEC_1548u 0x60Cu, 0, 0, 0, 0, 10, 39, 2, 2, 3, 3, 43
#define PPUTLIMPL_UDEC_1547u 0x60Bu, 0, 0, 0, 0, 10, 39, 7, 13, 17
#define PPUTLIMPL_UDEC_1546u 0x60Au, 0, 0, 0, 0, 10, 39, 2, 773
#define PPUTLIMPL_UDEC_1545u 0x609u, 0, 0, 0, 0, 10, 39, 3, 5, 103
#define PPUTLIMPL_UDEC_1544u 0x608u, 0, 0, 0, 0, 10, 39, 2, 2, 2, 193
#define PPUTLIMPL_UDEC_1543u 0x607u, 0, 0, 0, 0, 10, 39,
#define PPUTLIMPL_UDEC_1542u 0x606u, 0, 0, 0, 0, 10, 39, 2, 3, 257
#define PPUTLIMPL_UDEC_1541u 0x605u, 0, 0, 0, 0, 10, 39, 23, 67
#define PPUTLIMPL_UDEC_1540u 0x604u, 0, 0, 0, 0, 10, 39, 2, 2, 5, 7, 11
#define PPUTLIMPL_UDEC_1539u 0x603u, 0, 0, 0, 0, 10, 39, 3, 3, 3, 3, 19
#define PPUTLIMPL_UDEC_1538u 0x602u, 0, 0, 0, 0, 10, 39, 2, 769
#define PPUTLIMPL_UDEC_1537u 0x601u, 0, 0, 0, 0, 10, 39, 29, 53
#define PPUTLIMPL_UDEC_1536u 0x600u, 0, 0, 0, 0, 10, 39, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3
#define PPUTLIMPL_UDEC_1535u 0x5FFu, 0, 0, 0, 0, 10, 39, 5, 307
#define PPUTLIMPL_UDEC_1534u 0x5FEu, 0, 0, 0, 0, 10, 39, 2, 13, 59
#define PPUTLIMPL_UDEC_1533u 0x5FDu, 0, 0, 0, 0, 10, 39, 3, 7, 73
#define PPUTLIMPL_UDEC_1532u 0x5FCu, 0, 0, 0, 0, 10, 39, 2, 2, 383
#define PPUTLIMPL_UDEC_1531u 0x5FBu, 0, 0, 0, 0, 10, 39,
#define PPUTLIMPL_UDEC_1530u 0x5FAu, 0, 0, 0, 0, 10, 39, 2, 3, 3, 5, 17
#define PPUTLIMPL_UDEC_1529u 0x5F9u, 0, 0, 0, 0, 10, 39, 11, 139
#define PPUTLIMPL_UDEC_1528u 0x5F8u, 0, 0, 0, 0, 10, 39, 2, 2, 2, 191
#define PPUTLIMPL_UDEC_1527u 0x5F7u, 0, 0, 0, 0, 10, 39, 3, 509
#define PPUTLIMPL_UDEC_1526u 0x5F6u, 0, 0, 0, 0, 10, 39, 2, 7, 109
#define PPUTLIMPL_UDEC_1525u 0x5F5u, 0, 0, 0, 0, 10, 39, 5, 5, 61
#define PPUTLIMPL_UDEC_1524u 0x5F4u, 0, 0, 0, 0, 10, 39, 2, 2, 3, 127
#define PPUTLIMPL_UDEC_1523u 0x5F3u, 0, 0, 0, 0, 10, 39,
#define PPUTLIMPL_UDEC_1522u 0x5F2u, 0, 0, 0, 0, 10, 39, 2, 761
#define PPUTLIMPL_UDEC_1521u 0x5F1u, 0, 0, 0, 0, 10, 39, 3, 3, 13, 13
#define PPUTLIMPL_UDEC_1520u 0x5F0u, 0, 0, 0, 0, 10, 38, 2, 2, 2, 2, 5, 19
#define PPUTLIMPL_UDEC_1519u 0x5EFu, 0, 0, 0, 0, 10, 38, 7, 7, 31
#define PPUTLIMPL_UDEC_1518u 0x5EEu, 0, 0, 0, 0, 10, 38, 2, 3, 11, 23
#define PPUTLIMPL_UDEC_1517u 0x5EDu, 0, 0, 0, 0, 10, 38, 37, 41
#define PPUTLIMPL_UDEC_1516u 0x5ECu, 0, 0, 0, 0, 10, 38, 2, 2, 379
#define PPUTLIMPL_UDEC_1515u 0x5EBu, 0, 0, 0, 0, 10, 38, 3, 5, 101
#define PPUTLIMPL_UDEC_1514u 0x5EAu, 0, 0, 0, 0, 10, 38, 2, 757
#define PPUTLIMPL_UDEC_1513u 0x5E9u, 0, 0, 0, 0, 10, 38, 17, 89
#define PPUTLIMPL_UDEC_1512u 0x5E8u, 0, 0, 0, 0, 10, 38, 2, 2, 2, 3, 3, 3, 7
#define PPUTLIMPL_UDEC_1511u 0x5E7u, 0, 0, 0, 0, 10, 38,
#define PPUTLIMPL_UDEC_1510u 0x5E6u, 0, 0, 0, 0, 10, 38, 2, 5, 151
#define PPUTLIMPL_UDEC_1509u 0x5E5u, 0, 0, 0, 0, 10, 38, 3, 503
#define PPUTLIMPL_UDEC_1508u 0x5E4u, 0, 0, 0, 0, 10, 38, 2, 2, 13, 29
#define PPUTLIMPL_UDEC_1507u 0x5E3u, 0, 0, 0, 0, 10, 38, 11, 137
#define PPUTLIMPL_UDEC_1506u 0x5E2u, 0, 0, 0, 0, 10, 38, 2, 3, 251
#define PPUTLIMPL_UDEC_1505u 0x5E1u, 0, 0, 0, 0, 10, 38, 5, 7, 43
#define PPUTLIMPL_UDEC_1504u 0x5E0u, 0, 0, 0, 0, 10, 38, 2, 2, 2, 2, 2, 47
#define PPUTLIMPL_UDEC_1503u 0x5DFu, 0, 0, 0, 0, 10, 38, 3, 3, 167
#define PPUTLIMPL_UDEC_1502u 0x5DEu, 0, 0, 0, 0, 10, 38, 2, 751
#define PPUTLIMPL_UDEC_1501u 0x5DDu, 0, 0, 0, 0, 10, 38, 19, 79
#define PPUTLIMPL_UDEC_1500u 0x5DCu, 0, 0, 0, 0, 10, 38, 2, 2, 3, 5, 5, 5
#define PPUTLIMPL_UDEC_1499u 0x5DBu, 0, 0, 0, 0, 10, 38,
#define PPUTLIMPL_UDEC_1498u 0x5DAu, 0, 0, 0, 0, 10, 38, 2, 7, 107
#define PPUTLIMPL_UDEC_1497u 0x5D9u, 0, 0, 0, 0, 10, 38, 3, 499
#define PPUTLIMPL_UDEC_1496u 0x5D8u, 0, 0, 0, 0, 10, 38, 2, 2, 2, 11, 17
#define PPUTLIMPL_UDEC_1495u 0x5D7u, 0, 0, 0, 0, 10, 38, 5, 13, 23
#define PPUTLIMPL_UDEC_1494u 0x5D6u, 0, 0, 0, 0, 10, 38, 2, 3, 3, 83
#define PPUTLIMPL_UDEC_1493u 0x5D5u, 0, 0, 0, 0, 10, 38,
#define PPUTLIMPL_UDEC_1492u 0x5D4u, 0, 0, 0, 0, 10, 38, 2, 2, 373
#define PPUTLIMPL_UDEC_1491u 0x5D3u, 0, 0, 0, 0, 10, 38, 3, 7, 71
#define PPUTLIMPL_UDEC_1490u 0x5D2u, 0, 0, 0, 0, 10, 38, 2, 5, 149
#define PPUTLIMPL_UDEC_1489u 0x5D1u, 0, 0, 0, 0, 10, 38,
#define PPUTLIMPL_UDEC_1488u 0x5D0u, 0, 0, 0, 0, 10, 38, 2, 2, 2, 2, 3, 31
#define PPUTLIMPL_UDEC_1487u 0x5CFu, 0, 0, 0, 0, 10, 38,
#define PPUTLIMPL_UDEC_1486u 0x5CEu, 0, 0, 0, 0, 10, 38, 2, 743
#define PPUTLIMPL_UDEC_1485u 0x5CDu, 0, 0, 0, 0, 10, 38, 3, 3, 3, 5, 11
#define PPUTLIMPL_UDEC_1484u 0x5CCu, 0, 0, 0, 0, 10, 38, 2, 2, 7, 53
#define PPUTLIMPL_UDEC_1483u 0x5CBu, 0, 0, 0, 0, 10, 38,
#define PPUTLIMPL_UDEC_1482u 0x5CAu, 0, 0, 0, 0, 10, 38, 2, 3, 13, 19
#define PPUTLIMPL_UDEC_1481u 0x5C9u, 0, 0, 0, 0, 10, 38,
#define PPUTLIMPL_UDEC_1480u 0x5C8u, 0, 0, 0, 0, 10, 38, 2, 2, 2, 5, 37
#define PPUTLIMPL_UDEC_1479u 0x5C7u, 0, 0, 0, 0, 10, 38, 3, 17, 29
#define PPUTLIMPL_UDEC_1478u 0x5C6u, 0, 0, 0, 0, 10, 38, 2, 739
#define PPUTLIMPL_UDEC_1477u 0x5C5u, 0, 0, 0, 0, 10, 38, 7, 211
#define PPUTLIMPL_UDEC_1476u 0x5C4u, 0, 0, 0, 0, 10, 38, 2, 2, 3, 3, 41
#define PPUTLIMPL_UDEC_1475u 0x5C3u, 0, 0, 0, 0, 10, 38, 5, 5, 59
#define PPUTLIMPL_UDEC_1474u 0x5C2u, 0, 0, 0, 0, 10, 38, 2, 11, 67
#define PPUTLIMPL_UDEC_1473u 0x5C1u, 0, 0, 0, 0, 10, 38, 3, 491
#define PPUTLIMPL_UDEC_1472u 0x5C0u, 0, 0, 0, 0, 10, 38, 2, 2, 2, 2, 2, 2, 23
#define PPUTLIMPL_UDEC_1471u 0x5BFu, 0, 0, 0, 0, 10, 38,
#define PPUTLIMPL_UDEC_1470u 0x5BEu, 0, 0, 0, 0, 10, 38, 2, 3, 5, 7, 7
#define PPUTLIMPL_UDEC_1469u 0x5BDu, 0, 0, 0, 0, 10, 38, 13, 113
#define PPUTLIMPL_UDEC_1468u 0x5BCu, 0, 0, 0, 0, 10, 38, 2, 2, 367
#define PPUTLIMPL_UDEC_1467u 0x5BBu, 0, 0, 0, 0, 10, 38, 3, 3, 163
#define PPUTLIMPL_UDEC_1466u 0x5BAu, 0, 0, 0, 0, 10, 38, 2, 733
#define PPUTLIMPL_UDEC_1465u 0x5B9u, 0, 0, 0, 0, 10, 38, 5, 293
#define PPUTLIMPL_UDEC_1464u 0x5B8u, 0, 0, 0, 0, 10, 38, 2, 2, 2, 3, 61
#define PPUTLIMPL_UDEC_1463u 0x5B7u, 0, 0, 0, 0, 10, 38, 7, 11, 19
#define PPUTLIMPL_UDEC_1462u 0x5B6u, 0, 0, 0, 0, 10, 38, 2, 17, 43
#define PPUTLIMPL_UDEC_1461u 0x5B5u, 0, 0, 0, 0, 10, 38, 3, 487
#define PPUTLIMPL_UDEC_1460u 0x5B4u, 0, 0, 0, 0, 10, 38, 2, 2, 5, 73
#define PPUTLIMPL_UDEC_1459u 0x5B3u, 0, 0, 0, 0, 10, 38,
#define PPUTLIMPL_UDEC_1458u 0x5B2u, 0, 0, 0, 0, 10, 38, 2, 3, 3, 3, 3, 3, 3
#define PPUTLIMPL_UDEC_1457u 0x5B1u, 0, 0, 0, 0, 10, 38, 31, 47
#define PPUTLIMPL_UDEC_1456u 0x5B0u, 0, 0, 0, 0, 10, 38, 2, 2, 2, 2, 7, 13
#define PPUTLIMPL_UDEC_1455u 0x5AFu, 0, 0, 0, 0, 10, 38, 3, 5, 97
#define PPUTLIMPL_UDEC_1454u 0x5AEu, 0, 0, 0, 0, 10, 38, 2, 727
#define PPUTLIMPL_UDEC_1453u 0x5ADu, 0, 0, 0, 0, 10, 38,
#define PPUTLIMPL_UDEC_1452u 0x5ACu, 0, 0, 0, 0, 10, 38, 2, 2, 3, 11, 11
#define PPUTLIMPL_UDEC_1451u 0x5ABu, 0, 0, 0, 0, 10, 38,
#define PPUTLIMPL_UDEC_1450u 0x5AAu, 0, 0, 0, 0, 10, 38, 2, 5, 5, 29
#define PPUTLIMPL_UDEC_1449u 0x5A9u, 0, 0, 0, 0, 10, 38, 3, 3, 7, 23
#define PPUTLIMPL_UDEC_1448u 0x5A8u, 0, 0, 0, 0, 10, 38, 2, 2, 2, 181
#define PPUTLIMPL_UDEC_1447u 0x5A7u, 0, 0, 0, 0, 10, 38,
#define PPUTLIMPL_UDEC_1446u 0x5A6u, 0, 0, 0, 0, 10, 38, 2, 3, 241
#define PPUTLIMPL_UDEC_1445u 0x5A5u, 0, 0, 0, 0, 10, 38, 5, 17, 17
#define PPUTLIMPL_UDEC_1444u 0x5A4u, 0, 0, 0, 0, 10, 38, 2, 2, 19, 19
#define PPUTLIMPL_UDEC_1443u 0x5A3u, 0, 0, 0, 0, 10, 37, 3, 13, 37
#define PPUTLIMPL_UDEC_1442u 0x5A2u, 0, 0, 0, 0, 10, 37, 2, 7, 103
#define PPUTLIMPL_UDEC_1441u 0x5A1u, 0, 0, 0, 0, 10, 37, 11, 131
#define PPUTLIMPL_UDEC_1440u 0x5A0u, 0, 0, 0, 0, 10, 37, 2, 2, 2, 2, 2, 3, 3, 5
#define PPUTLIMPL_UDEC_1439u 0x59Fu, 0, 0, 0, 0, 10, 37,
#define PPUTLIMPL_UDEC_1438u 0x59Eu, 0, 0, 0, 0, 10, 37, 2, 719
#define PPUTLIMPL_UDEC_1437u 0x59Du, 0, 0, 0, 0, 10, 37, 3, 479
#define PPUTLIMPL_UDEC_1436u 0x59Cu, 0, 0, 0, 0, 10, 37, 2, 2, 359
#define PPUTLIMPL_UDEC_1435u 0x59Bu, 0, 0, 0, 0, 10, 37, 5, 7, 41
#define PPUTLIMPL_UDEC_1434u 0x59Au, 0, 0, 0, 0, 10, 37, 2, 3, 239
#define PPUTLIMPL_UDEC_1433u 0x599u, 0, 0, 0, 0, 10, 37,
#define PPUTLIMPL_UDEC_1432u 0x598u, 0, 0, 0, 0, 10, 37, 2, 2, 2, 179
#define PPUTLIMPL_UDEC_1431u 0x597u, 0, 0, 0, 0, 10, 37, 3, 3, 3, 53
#define PPUTLIMPL_UDEC_1430u 0x596u, 0, 0, 0, 0, 10, 37, 2, 5, 11, 13
#define PPUTLIMPL_UDEC_1429u 0x595u, 0, 0, 0, 0, 10, 37,
#define PPUTLIMPL_UDEC_1428u 0x594u, 0, 0, 0, 0, 10, 37, 2, 2, 3, 7, 17
#define PPUTLIMPL_UDEC_1427u 0x593u, 0, 0, 0, 0, 10, 37,
#define PPUTLIMPL_UDEC_1426u 0x592u, 0, 0, 0, 0, 10, 37, 2, 23, 31
#define PPUTLIMPL_UDEC_1425u 0x591u, 0, 0, 0, 0, 10, 37, 3, 5, 5, 19
#define PPUTLIMPL_UDEC_1424u 0x590u, 0, 0, 0, 0, 10, 37, 2, 2, 2, 2, 89
#define PPUTLIMPL_UDEC_1423u 0x58Fu, 0, 0, 0, 0, 10, 37,
#define PPUTLIMPL_UDEC_1422u 0x58Eu, 0, 0, 0, 0, 10, 37, 2, 3, 3, 79
#define PPUTLIMPL_UDEC_1421u 0x58Du, 0, 0, 0, 0, 10, 37, 7, 7, 29
#define PPUTLIMPL_UDEC_1420u 0x58Cu, 0, 0, 0, 0, 10, 37, 2, 2, 5, 71
#define PPUTLIMPL_UDEC_1419u 0x58Bu, 0, 0, 0, 0, 10, 37, 3, 11, 43
#define PPUTLIMPL_UDEC_1418u 0x58Au, 0, 0, 0, 0, 10, 37, 2, 709
#define PPUTLIMPL_UDEC_1417u 0x589u, 0, 0, 0, 0, 10, 37, 13, 109
#define PPUTLIMPL_UDEC_1416u 0x588u, 0, 0, 0, 0, 10, 37, 2, 2, 2, 3, 59
#define PPUTLIMPL_UDEC_1415u 0x587u, 0, 0, 0, 0, 10, 37, 5, 283
#define PPUTLIMPL_UDEC_1414u 0x586u, 0, 0, 0, 0, 10, 37, 2, 7, 101
#define PPUTLIMPL_UDEC_1413u 0x585u, 0, 0, 0, 0, 10, 37, 3, 3, 157
#define PPUTLIMPL_UDEC_1412u 0x584u, 0, 0, 0, 0, 10, 37, 2, 2, 353
#define PPUTLIMPL_UDEC_1411u 0x583u, 0, 0, 0, 0, 10, 37, 17, 83
#define PPUTLIMPL_UDEC_1410u 0x582u, 0, 0, 0, 0, 10, 37, 2, 3, 5, 47
#define PPUTLIMPL_UDEC_1409u 0x581u, 0, 0, 0, 0, 10, 37,
#define PPUTLIMPL_UDEC_1408u 0x580u, 0, 0, 0, 0, 10, 37, 2, 2, 2, 2, 2, 2, 2, 11
#define PPUTLIMPL_UDEC_1407u 0x57Fu, 0, 0, 0, 0, 10, 37, 3, 7, 67
#define PPUTLIMPL_UDEC_1406u 0x57Eu, 0, 0, 0, 0, 10, 37, 2, 19, 37
#define PPUTLIMPL_UDEC_1405u 0x57Du, 0, 0, 0, 0, 10, 37, 5, 281
#define PPUTLIMPL_UDEC_1404u 0x57Cu, 0, 0, 0, 0, 10, 37, 2, 2, 3, 3, 3, 13
#define PPUTLIMPL_UDEC_1403u 0x57Bu, 0, 0, 0, 0, 10, 37, 23, 61
#define PPUTLIMPL_UDEC_1402u 0x57Au, 0, 0, 0, 0, 10, 37, 2, 701
#define PPUTLIMPL_UDEC_1401u 0x579u, 0, 0, 0, 0, 10, 37, 3, 467
#define PPUTLIMPL_UDEC_1400u 0x578u, 0, 0, 0, 0, 10, 37, 2, 2, 2, 5, 5, 7
#define PPUTLIMPL_UDEC_1399u 0x577u, 0, 0, 0, 0, 10, 37,
#define PPUTLIMPL_UDEC_1398u 0x576u, 0, 0, 0, 0, 10, 37, 2, 3, 233
#define PPUTLIMPL_UDEC_1397u 0x575u, 0, 0, 0, 0, 10, 37, 11, 127
#define PPUTLIMPL_UDEC_1396u 0x574u, 0, 0, 0, 0, 10, 37, 2, 2, 349
#define PPUTLIMPL_UDEC_1395u 0x573u, 0, 0, 0, 0, 10, 37, 3, 3, 5, 31
#define PPUTLIMPL_UDEC_1394u 0x572u, 0, 0, 0, 0, 10, 37, 2, 17, 41
#define PPUTLIMPL_UDEC_1393u 0x571u, 0, 0, 0, 0, 10, 37, 7, 199
#define PPUTLIMPL_UDEC_1392u 0x570u, 0, 0, 0, 0, 10, 37, 2, 2, 2, 2, 3, 29
#define PPUTLIMPL_UDEC_1391u 0x56Fu, 0, 0, 0, 0, 10, 37, 13, 107
#define PPUTLIMPL_UDEC_1390u 0x56Eu, 0, 0, 0, 0, 10, 37, 2, 5, 139
#define PPUTLIMPL_UDEC_1389u 0x56Du, 0, 0, 0, 0, 10, 37, 3, 463
#define PPUTLIMPL_UDEC_1388u 0x56Cu, 0, 0, 0, 0, 10, 37, 2, 2, 347
#define PPUTLIMPL_UDEC_1387u 0x56Bu, 0, 0, 0, 0, 10, 37, 19, 73
#define PPUTLIMPL_UDEC_1386u 0x56Au, 0, 0, 0, 0, 10, 37, 2, 3, 3, 7, 11
#define PPUTLIMPL_UDEC_1385u 0x569u, 0, 0, 0, 0, 10, 37, 5, 277
#define PPUTLIMPL_UDEC_1384u 0x568u, 0, 0, 0, 0, 10, 37, 2, 2, 2, 173
#define PPUTLIMPL_UDEC_1383u 0x567u, 0, 0, 0, 0, 10, 37, 3, 461
#define PPUTLIMPL_UDEC_1382u 0x566u, 0, 0, 0, 0, 10, 37, 2, 691
#define PPUTLIMPL_UDEC_1381u 0x565u, 0, 0, 0, 0, 10, 37,
#define PPUTLIMPL_UDEC_1380u 0x564u, 0, 0, 0, 0, 10, 37, 2, 2, 3, 5, 23
#define PPUTLIMPL_UDEC_1379u 0x563u, 0, 0, 0, 0, 10, 37, 7, 197
#define PPUTLIMPL_UDEC_1378u 0x562u, 0, 0, 0, 0, 10, 37, 2, 13, 53
#define PPUTLIMPL_UDEC_1377u 0x561u, 0, 0, 0, 0, 10, 37, 3, 3, 3, 3, 17
#define PPUTLIMPL_UDEC_1376u 0x560u, 0, 0, 0, 0, 10, 37, 2, 2, 2, 2, 2, 43
#define PPUTLIMPL_UDEC_1375u 0x55Fu, 0, 0, 0, 0, 10, 37, 5, 5, 5, 11
#define PPUTLIMPL_UDEC_1374u 0x55Eu, 0, 0, 0, 0, 10, 37, 2, 3, 229
#define PPUTLIMPL_UDEC_1373u 0x55Du, 0, 0, 0, 0, 10, 37,
#define PPUTLIMPL_UDEC_1372u 0x55Cu, 0, 0, 0, 0, 10, 37, 2, 2, 7, 7, 7
#define PPUTLIMPL_UDEC_1371u 0x55Bu, 0, 0, 0, 0, 10, 37, 3, 457
#define PPUTLIMPL_UDEC_1370u 0x55Au, 0, 0, 0, 0, 10, 37, 2, 5, 137
#define PPUTLIMPL_UDEC_1369u 0x559u, 0, 0, 0, 0, 10, 37, 37, 37
#define PPUTLIMPL_UDEC_1368u 0x558u, 0, 0, 0, 0, 10, 36, 2, 2, 2, 3, 3, 19
#define PPUTLIMPL_UDEC_1367u 0x557u, 0, 0, 0, 0, 10, 36,
#define PPUTLIMPL_UDEC_1366u 0x556u, 0, 0, 0, 0, 10, 36, 2, 683
#define PPUTLIMPL_UDEC_1365u 0x555u, 0, 0, 0, 0, 10, 36, 3, 5, 7, 13
#define PPUTLIMPL_UDEC_1364u 0x554u, 0, 0, 0, 0, 10, 36, 2, 2, 11, 31
#define PPUTLIMPL_UDEC_1363u 0x553u, 0, 0, 0, 0, 10, 36, 29, 47
#define PPUTLIMPL_UDEC_1362u 0x552u, 0, 0, 0, 0, 10, 36, 2, 3, 227
#define PPUTLIMPL_UDEC_1361u 0x551u, 0, 0, 0, 0, 10, 36,
#define PPUTLIMPL_UDEC_1360u 0x550u, 0, 0, 0, 0, 10, 36, 2, 2, 2, 2, 5, 17
#define PPUTLIMPL_UDEC_1359u 0x54Fu, 0, 0, 0, 0, 10, 36, 3, 3, 151
#define PPUTLIMPL_UDEC_1358u 0x54Eu, 0, 0, 0, 0, 10, 36, 2, 7, 97
#define PPUTLIMPL_UDEC_1357u 0x54Du, 0, 0, 0, 0, 10, 36, 23, 59
#define PPUTLIMPL_UDEC_1356u 0x54Cu, 0, 0, 0, 0, 10, 36, 2, 2, 3, 113
#define PPUTLIMPL_UDEC_1355u 0x54Bu, 0, 0, 0, 0, 10, 36, 5, 271
#define PPUTLIMPL_UDEC_1354u 0x54Au, 0, 0, 0, 0, 10, 36, 2, 677
#define PPUTLIMPL_UDEC_1353u 0x549u, 0, 0, 0, 0, 10, 36, 3, 11, 41
#define PPUTLIMPL_UDEC_1352u 0x548u, 0, 0, 0, 0, 10, 36, 2, 2, 2, 13, 13
#define PPUTLIMPL_UDEC_1351u 0x547u, 0, 0, 0, 0, 10, 36, 7, 193
#define PPUTLIMPL_UDEC_1350u 0x546u, 0, 0, 0, 0, 10, 36, 2, 3, 3, 3, 5, 5
#define PPUTLIMPL_UDEC_1349u 0x545u, 0, 0, 0, 0, 10, 36, 19, 71
#define PPUTLIMPL_UDEC_1348u 0x544u, 0, 0, 0, 0, 10, 36, 2, 2, 337
#define PPUTLIMPL_UDEC_1347u 0x543u, 0, 0, 0, 0, 10, 36, 3, 449
#define PPUTLIMPL_UDEC_1346u 0x542u, 0, 0, 0, 0, 10, 36, 2, 673
#define PPUTLIMPL_UDEC_1345u 0x541u, 0, 0, 0, 0, 10, 36, 5, 269
#define PPUTLIMPL_UDEC_1344u 0x540u, 0, 0, 0, 0, 10, 36, 2, 2, 2, 2, 2, 2, 3, 7
#define PPUTLIMPL_UDEC_1343u 0x53Fu, 0, 0, 0, 0, 10, 36, 17, 79
#define PPUTLIMPL_UDEC_1342u 0x53Eu, 0, 0, 0, 0, 10, 36, 2, 11, 61
#define PPUTLIMPL_UDEC_1341u 0x53Du, 0, 0, 0, 0, 10, 36, 3, 3, 149
#define PPUTLIMPL_UDEC_1340u 0x53Cu, 0, 0, 0, 0, 10, 36, 2, 2, 5, 67
#define PPUTLIMPL_UDEC_1339u 0x53Bu, 0, 0, 0, 0, 10, 36, 13, 103
#define PPUTLIMPL_UDEC_1338u 0x53Au, 0, 0, 0, 0, 10, 36, 2, 3, 223
#define PPUTLIMPL_UDEC_1337u 0x539u, 0, 0, 0, 0, 10, 36, 7, 191
#define PPUTLIMPL_UDEC_1336u 0x538u, 0, 0, 0, 0, 10, 36, 2, 2, 2, 167
#define PPUTLIMPL_UDEC_1335u 0x537u, 0, 0, 0, 0, 10, 36, 3, 5, 89
#define PPUTLIMPL_UDEC_1334u 0x536u, 0, 0, 0, 0, 10, 36, 2, 23, 29
#define PPUTLIMPL_UDEC_1333u 0x535u, 0, 0, 0, 0, 10, 36, 31, 43
#define PPUTLIMPL_UDEC_1332u 0x534u, 0, 0, 0, 0, 10, 36, 2, 2, 3, 3, 37
#define PPUTLIMPL_UDEC_1331u 0x533u, 0, 0, 0, 0, 10, 36, 11, 11, 11
#define PPUTLIMPL_UDEC_1330u 0x532u, 0, 0, 0, 0, 10, 36, 2, 5, 7, 19
#define PPUTLIMPL_UDEC_1329u 0x531u, 0, 0, 0, 0, 10, 36, 3, 443
#define PPUTLIMPL_UDEC_1328u 0x530u, 0, 0, 0, 0, 10, 36, 2, 2, 2, 2, 83
#define PPUTLIMPL_UDEC_1327u 0x52Fu, 0, 0, 0, 0, 10, 36,
#define PPUTLIMPL_UDEC_1326u 0x52Eu, 0, 0, 0, 0, 10, 36, 2, 3, 13, 17
#define PPUTLIMPL_UDEC_1325u 0x52Du, 0, 0, 0, 0, 10, 36, 5, 5, 53
#define PPUTLIMPL_UDEC_1324u 0x52Cu, 0, 0, 0, 0, 10, 36, 2, 2, 331
#define PPUTLIMPL_UDEC_1323u 0x52Bu, 0, 0, 0, 0, 10, 36, 3, 3, 3, 7, 7
#define PPUTLIMPL_UDEC_1322u 0x52Au, 0, 0, 0, 0, 10, 36, 2, 661
#define PPUTLIMPL_UDEC_1321u 0x529u, 0, 0, 0, 0, 10, 36,
#define PPUTLIMPL_UDEC_1320u 0x528u, 0, 0, 0, 0, 10, 36, 2, 2, 2, 3, 5, 11
#define PPUTLIMPL_UDEC_1319u 0x527u, 0, 0, 0, 0, 10, 36,
#define PPUTLIMPL_UDEC_1318u 0x526u, 0, 0, 0, 0, 10, 36, 2, 659
#define PPUTLIMPL_UDEC_1317u 0x525u, 0, 0, 0, 0, 10, 36, 3, 439
#define PPUTLIMPL_UDEC_1316u 0x524u, 0, 0, 0, 0, 10, 36, 2, 2, 7, 47
#define PPUTLIMPL_UDEC_1315u 0x523u, 0, 0, 0, 0, 10, 36, 5, 263
#define PPUTLIMPL_UDEC_1314u 0x522u, 0, 0, 0, 0, 10, 36, 2, 3, 3, 73
#define PPUTLIMPL_UDEC_1313u 0x521u, 0, 0, 0, 0, 10, 36, 13, 101
#define PPUTLIMPL_UDEC_1312u 0x520u, 0, 0, 0, 0, 10, 36, 2, 2, 2, 2, 2, 41
#define PPUTLIMPL_UDEC_1311u 0x51Fu, 0, 0, 0, 0, 10, 36, 3, 19, 23
#define PPUTLIMPL_UDEC_1310u 0x51Eu, 0, 0, 0, 0, 10, 36, 2, 5, 131
#define PPUTLIMPL_UDEC_1309u 0x51Du, 0, 0, 0, 0, 10, 36, 7, 11, 17
#define PPUTLIMPL_UDEC_1308u 0x51Cu, 0, 0, 0, 0, 10, 36, 2, 2, 3, 109
#define PPUTLIMPL_UDEC_1307u 0x51Bu, 0, 0, 0, 0, 10, 36,
#define PPUTLIMPL_UDEC_1306u 0x51Au, 0, 0, 0, 0, 10, 36, 2, 653
#define PPUTLIMPL_UDEC_1305u 0x519u, 0, 0, 0, 0, 10, 36, 3, 3, 5, 29
#define PPUTLIMPL_UDEC_1304u 0x518u, 0, 0, 0, 0, 10, 36, 2, 2, 2, 163
#define PPUTLIMPL_UDEC_1303u 0x517u, 0, 0, 0, 0, 10, 36,
#define PPUTLIMPL_UDEC_1302u 0x516u, 0, 0, 0, 0, 10, 36, 2, 3, 7, 31
#define PPUTLIMPL_UDEC_1301u 0x515u, 0, 0, 0, 0, 10, 36,
#define PPUTLIMPL_UDEC_1300u 0x514u, 0, 0, 0, 0, 10, 36, 2, 2, 5, 5, 13
#define PPUTLIMPL_UDEC_1299u 0x513u, 0, 0, 0, 0, 10, 36, 3, 433
#define PPUTLIMPL_UDEC_1298u 0x512u, 0, 0, 0, 0, 10, 36, 2, 11, 59
#define PPUTLIMPL_UDEC_1297u 0x511u, 0, 0, 0, 0, 10, 36,
#define PPUTLIMPL_UDEC_1296u 0x510u, 0, 0, 0, 0, 10, 36, 2, 2, 2, 2, 3, 3, 3, 3
#define PPUTLIMPL_UDEC_1295u 0x50Fu, 0, 0, 0, 0, 10, 35, 5, 7, 37
#define PPUTLIMPL_UDEC_1294u 0x50Eu, 0, 0, 0, 0, 10, 35, 2, 647
#define PPUTLIMPL_UDEC_1293u 0x50Du, 0, 0, 0, 0, 10, 35, 3, 431
#define PPUTLIMPL_UDEC_1292u 0x50Cu, 0, 0, 0, 0, 10, 35, 2, 2, 17, 19
#define PPUTLIMPL_UDEC_1291u 0x50Bu, 0, 0, 0, 0, 10, 35,
#define PPUTLIMPL_UDEC_1290u 0x50Au, 0, 0, 0, 0, 10, 35, 2, 3, 5, 43
#define PPUTLIMPL_UDEC_1289u 0x509u, 0, 0, 0, 0, 10, 35,
#define PPUTLIMPL_UDEC_1288u 0x508u, 0, 0, 0, 0, 10, 35, 2, 2, 2, 7, 23
#define PPUTLIMPL_UDEC_1287u 0x507u, 0, 0, 0, 0, 10, 35, 3, 3, 11, 13
#define PPUTLIMPL_UDEC_1286u 0x506u, 0, 0, 0, 0, 10, 35, 2, 643
#define PPUTLIMPL_UDEC_1285u 0x505u, 0, 0, 0, 0, 10, 35, 5, 257
#define PPUTLIMPL_UDEC_1284u 0x504u, 0, 0, 0, 0, 10, 35, 2, 2, 3, 107
#define PPUTLIMPL_UDEC_1283u 0x503u, 0, 0, 0, 0, 10, 35,
#define PPUTLIMPL_UDEC_1282u 0x502u, 0, 0, 0, 0, 10, 35, 2, 641
#define PPUTLIMPL_UDEC_1281u 0x501u, 0, 0, 0, 0, 10, 35, 3, 7, 61
#define PPUTLIMPL_UDEC_1280u 0x500u, 0, 0, 0, 0, 10, 35, 2, 2, 2, 2, 2, 2, 2, 2, 5
#define PPUTLIMPL_UDEC_1279u 0x4FFu, 0, 0, 0, 0, 10, 35,
#define PPUTLIMPL_UDEC_1278u 0x4FEu, 0, 0, 0, 0, 10, 35, 2, 3, 3, 71
#define PPUTLIMPL_UDEC_1277u 0x4FDu, 0, 0, 0, 0, 10, 35,
#define PPUTLIMPL_UDEC_1276u 0x4FCu, 0, 0, 0, 0, 10, 35, 2, 2, 11, 29
#define PPUTLIMPL_UDEC_1275u 0x4FBu, 0, 0, 0, 0, 10, 35, 3, 5, 5, 17
#define PPUTLIMPL_UDEC_1274u 0x4FAu, 0, 0, 0, 0, 10, 35, 2, 7, 7, 13
#define PPUTLIMPL_UDEC_1273u 0x4F9u, 0, 0, 0, 0, 10, 35, 19, 67
#define PPUTLIMPL_UDEC_1272u 0x4F8u, 0, 0, 0, 0, 10, 35, 2, 2, 2, 3, 53
#define PPUTLIMPL_UDEC_1271u 0x4F7u, 0, 0, 0, 0, 10, 35, 31, 41
#define PPUTLIMPL_UDEC_1270u 0x4F6u, 0, 0, 0, 0, 10, 35, 2, 5, 127
#define PPUTLIMPL_UDEC_1269u 0x4F5u, 0, 0, 0, 0, 10, 35, 3, 3, 3, 47
#define PPUTLIMPL_UDEC_1268u 0x4F4u, 0, 0, 0, 0, 10, 35, 2, 2, 317
#define PPUTLIMPL_UDEC_1267u 0x4F3u, 0, 0, 0, 0, 10, 35, 7, 181
#define PPUTLIMPL_UDEC_1266u 0x4F2u, 0, 0, 0, 0, 10, 35, 2, 3, 211
#define PPUTLIMPL_UDEC_1265u 0x4F1u, 0, 0, 0, 0, 10, 35, 5, 11, 23
#define PPUTLIMPL_UDEC_1264u 0x4F0u, 0, 0, 0, 0, 10, 35, 2, 2, 2, 2, 79
#define PPUTLIMPL_UDEC_1263u 0x4EFu, 0, 0, 0, 0, 10, 35, 3, 421
#define PPUTLIMPL_UDEC_1262u 0x4EEu, 0, 0, 0, 0, 10, 35, 2, 631
#define PPUTLIMPL_UDEC_1261u 0x4EDu, 0, 0, 0, 0, 10, 35, 13, 97
#define PPUTLIMPL_UDEC_1260u 0x4ECu, 0, 0, 0, 0, 10, 35, 2, 2, 3, 3, 5, 7
#define PPUTLIMPL_UDEC_1259u 0x4EBu, 0, 0, 0, 0, 10, 35,
#define PPUTLIMPL_UDEC_1258u 0x4EAu, 0, 0, 0, 0, 10, 35, 2, 17, 37
#define PPUTLIMPL_UDEC_1257u 0x4E9u, 0, 0, 0, 0, 10, 35, 3, 419
#define PPUTLIMPL_UDEC_1256u 0x4E8u, 0, 0, 0, 0, 10, 35, 2, 2, 2, 157
#define PPUTLIMPL_UDEC_1255u 0x4E7u, 0, 0, 0, 0, 10, 35, 5, 251
#define PPUTLIMPL_UDEC_1254u 0x4E6u, 0, 0, 0, 0, 10, 35, 2, 3, 11, 19
#define PPUTLIMPL_UDEC_1253u 0x4E5u, 0, 0, 0, 0, 10, 35, 7, 179
#define PPUTLIMPL_UDEC_1252u 0x4E4u, 0, 0, 0, 0, 10, 35, 2, 2, 313
#define PPUTLIMPL_UDEC_1251u 0x4E3u, 0, 0, 0, 0, 10, 35, 3, 3, 139
#define PPUTLIMPL_UDEC_1250u 0x4E2u, 0, 0, 0, 0, 10, 35, 2, 5, 5, 5, 5
#define PPUTLIMPL_UDEC_1249u 0x4E1u, 0, 0, 0, 0, 10, 35,
#define PPUTLIMPL_UDEC_1248u 0x4E0u, 0, 0, 0, 0, 10, 35, 2, 2, 2, 2, 2, 3, 13
#define PPUTLIMPL_UDEC_1247u 0x4DFu, 0, 0, 0, 0, 10, 35, 29, 43
#define PPUTLIMPL_UDEC_1246u 0x4DEu, 0, 0, 0, 0, 10, 35, 2, 7, 89
#define PPUTLIMPL_UDEC_1245u 0x4DDu, 0, 0, 0, 0, 10, 35, 3, 5, 83
#define PPUTLIMPL_UDEC_1244u 0x4DCu, 0, 0, 0, 0, 10, 35, 2, 2, 311
#define PPUTLIMPL_UDEC_1243u 0x4DBu, 0, 0, 0, 0, 10, 35, 11, 113
#define PPUTLIMPL_UDEC_1242u 0x4DAu, 0, 0, 0, 0, 10, 35, 2, 3, 3, 3, 23
#define PPUTLIMPL_UDEC_1241u 0x4D9u, 0, 0, 0, 0, 10, 35, 17, 73
#define PPUTLIMPL_UDEC_1240u 0x4D8u, 0, 0, 0, 0, 10, 35, 2, 2, 2, 5, 31
#define PPUTLIMPL_UDEC_1239u 0x4D7u, 0, 0, 0, 0, 10, 35, 3, 7, 59
#define PPUTLIMPL_UDEC_1238u 0x4D6u, 0, 0, 0, 0, 10, 35, 2, 619
#define PPUTLIMPL_UDEC_1237u 0x4D5u, 0, 0, 0, 0, 10, 35,
#define PPUTLIMPL_UDEC_1236u 0x4D4u, 0, 0, 0, 0, 10, 35, 2, 2, 3, 103
#define PPUTLIMPL_UDEC_1235u 0x4D3u, 0, 0, 0, 0, 10, 35, 5, 13, 19
#define PPUTLIMPL_UDEC_1234u 0x4D2u, 0, 0, 0, 0, 10, 35, 2, 617
#define PPUTLIMPL_UDEC_1233u 0x4D1u, 0, 0, 0, 0, 10, 35, 3, 3, 137
#define PPUTLIMPL_UDEC_1232u 0x4D0u, 0, 0, 0, 0, 10, 35, 2, 2, 2, 2, 7, 11
#define PPUTLIMPL_UDEC_1231u 0x4CFu, 0, 0, 0, 0, 10, 35,
#define PPUTLIMPL_UDEC_1230u 0x4CEu, 0, 0, 0, 0, 10, 35, 2, 3, 5, 41
#define PPUTLIMPL_UDEC_1229u 0x4CDu, 0, 0, 0, 0, 10, 35,
#define PPUTLIMPL_UDEC_1228u 0x4CCu, 0, 0, 0, 0, 10, 35, 2, 2, 307
#define PPUTLIMPL_UDEC_1227u 0x4CBu, 0, 0, 0, 0, 10, 35, 3, 409
#define PPUTLIMPL_UDEC_1226u 0x4CAu, 0, 0, 0, 0, 10, 35, 2, 613
#define PPUTLIMPL_UDEC_1225u 0x4C9u, 0, 0, 0, 0, 10, 35, 5, 5, 7, 7
#define PPUTLIMPL_UDEC_1224u 0x4C8u, 0, 0, 0, 0, 10, 34, 2, 2, 2, 3, 3, 17
#define PPUTLIMPL_UDEC_1223u 0x4C7u, 0, 0, 0, 0, 10, 34,
#define PPUTLIMPL_UDEC_1222u 0x4C6u, 0, 0, 0, 0, 10, 34, 2, 13, 47
#define PPUTLIMPL_UDEC_1221u 0x4C5u, 0, 0, 0, 0, 10, 34, 3, 11, 37
#define PPUTLIMPL_UDEC_1220u 0x4C4u, 0, 0, 0, 0, 10, 34, 2, 2, 5, 61
#define PPUTLIMPL_UDEC_1219u 0x4C3u, 0, 0, 0, 0, 10, 34, 23, 53
#define PPUTLIMPL_UDEC_1218u 0x4C2u, 0, 0, 0, 0, 10, 34, 2, 3, 7, 29
#define PPUTLIMPL_UDEC_1217u 0x4C1u, 0, 0, 0, 0, 10, 34,
#define PPUTLIMPL_UDEC_1216u 0x4C0u, 0, 0, 0, 0, 10, 34, 2, 2, 2, 2, 2, 2, 19
#define PPUTLIMPL_UDEC_1215u 0x4BFu, 0, 0, 0, 0, 10, 34, 3, 3, 3, 3, 3, 5
#define PPUTLIMPL_UDEC_1214u 0x4BEu, 0, 0, 0, 0, 10, 34, 2, 607
#define PPUTLIMPL_UDEC_1213u 0x4BDu, 0, 0, 0, 0, 10, 34,
#define PPUTLIMPL_UDEC_1212u 0x4BCu, 0, 0, 0, 0, 10, 34, 2, 2, 3, 101
#define PPUTLIMPL_UDEC_1211u 0x4BBu, 0, 0, 0, 0, 10, 34, 7, 173
#define PPUTLIMPL_UDEC_1210u 0x4BAu, 0, 0, 0, 0, 10, 34, 2, 5, 11, 11
#define PPUTLIMPL_UDEC_1209u 0x4B9u, 0, 0, 0, 0, 10, 34, 3, 13, 31
#define PPUTLIMPL_UDEC_1208u 0x4B8u, 0, 0, 0, 0, 10, 34, 2, 2, 2, 151
#define PPUTLIMPL_UDEC_1207u 0x4B7u, 0, 0, 0, 0, 10, 34, 17, 71
#define PPUTLIMPL_UDEC_1206u 0x4B6u, 0, 0, 0, 0, 10, 34, 2, 3, 3, 67
#define PPUTLIMPL_UDEC_1205u 0x4B5u, 0, 0, 0, 0, 10, 34, 5, 241
#define PPUTLIMPL_UDEC_1204u 0x4B4u, 0, 0, 0, 0, 10, 34, 2, 2, 7, 43
#define PPUTLIMPL_UDEC_1203u 0x4B3u, 0, 0, 0, 0, 10, 34, 3, 401
#define PPUTLIMPL_UDEC_1202u 0x4B2u, 0, 0, 0, 0, 10, 34, 2, 601
#define PPUTLIMPL_UDEC_1201u 0x4B1u, 0, 0, 0, 0, 10, 34,
#define PPUTLIMPL_UDEC_1200u 0x4B0u, 0, 0, 0, 0, 10, 34, 2, 2, 2, 2, 3, 5, 5
#define PPUTLIMPL_UDEC_1199u 0x4AFu, 0, 0, 0, 0, 10, 34, 11, 109
#define PPUTLIMPL_UDEC_1198u 0x4AEu, 0, 0, 0, 0, 10, 34, 2, 599
#define PPUTLIMPL_UDEC_1197u 0x4ADu, 0, 0, 0, 0, 10, 34, 3, 3, 7, 19
#define PPUTLIMPL_UDEC_1196u 0x4ACu, 0, 0, 0, 0, 10, 34, 2, 2, 13, 23
#define PPUTLIMPL_UDEC_1195u 0x4ABu, 0, 0, 0, 0, 10, 34, 5, 239
#define PPUTLIMPL_UDEC_1194u 0x4AAu, 0, 0, 0, 0, 10, 34, 2, 3, 199
#define PPUTLIMPL_UDEC_1193u 0x4A9u, 0, 0, 0, 0, 10, 34,
#define PPUTLIMPL_UDEC_1192u 0x4A8u, 0, 0, 0, 0, 10, 34, 2, 2, 2, 149
#define PPUTLIMPL_UDEC_1191u 0x4A7u, 0, 0, 0, 0, 10, 34, 3, 397
#define PPUTLIMPL_UDEC_1190u 0x4A6u, 0, 0, 0, 0, 10, 34, 2, 5, 7, 17
#define PPUTLIMPL_UDEC_1189u 0x4A5u, 0, 0, 0, 0, 10, 34, 29, 41
#define PPUTLIMPL_UDEC_1188u 0x4A4u, 0, 0, 0, 0, 10, 34, 2, 2, 3, 3, 3, 11
#define PPUTLIMPL_UDEC_1187u 0x4A3u, 0, 0, 0, 0, 10, 34,
#define PPUTLIMPL_UDEC_1186u 0x4A2u, 0, 0, 0, 0, 10, 34, 2, 593
#define PPUTLIMPL_UDEC_1185u 0x4A1u, 0, 0, 0, 0, 10, 34, 3, 5, 79
#define PPUTLIMPL_UDEC_1184u 0x4A0u, 0, 0, 0, 0, 10, 34, 2, 2, 2, 2, 2, 37
#define PPUTLIMPL_UDEC_1183u 0x49Fu, 0, 0, 0, 0, 10, 34, 7, 13, 13
#define PPUTLIMPL_UDEC_1182u 0x49Eu, 0, 0, 0, 0, 10, 34, 2, 3, 197
#define PPUTLIMPL_UDEC_1181u 0x49Du, 0, 0, 0, 0, 10, 34,
#define PPUTLIMPL_UDEC_1180u 0x49Cu, 0, 0, 0, 0, 10, 34, 2, 2, 5, 59
#define PPUTLIMPL_UDEC_1179u 0x49Bu, 0, 0, 0, 0, 10, 34, 3, 3, 131
#define PPUTLIMPL_UDEC_1178u 0x49Au, 0, 0, 0, 0, 10, 34, 2, 19, 31
#define PPUTLIMPL_UDEC_1177u 0x499u, 0, 0, 0, 0, 10, 34, 11, 107
#define PPUTLIMPL_UDEC_1176u 0x498u, 0, 0, 0, 0, 10, 34, 2, 2, 2, 3, 7, 7
#define PPUTLIMPL_UDEC_1175u 0x497u, 0, 0, 0, 0, 10, 34, 5, 5, 47
#define PPUTLIMPL_UDEC_1174u 0x496u, 0, 0, 0, 0, 10, 34, 2, 587
#define PPUTLIMPL_UDEC_1173u 0x495u, 0, 0, 0, 0, 10, 34, 3, 17, 23
#define PPUTLIMPL_UDEC_1172u 0x494u, 0, 0, 0, 0, 10, 34, 2, 2, 293
#define PPUTLIMPL_UDEC_1171u 0x493u, 0, 0, 0, 0, 10, 34,
#define PPUTLIMPL_UDEC_1170u 0x492u, 0, 0, 0, 0, 10, 34, 2, 3, 3, 5, 13
#define PPUTLIMPL_UDEC_1169u 0x491u, 0, 0, 0, 0, 10, 34, 7, 167
#define PPUTLIMPL_UDEC_1168u 0x490u, 0, 0, 0, 0, 10, 34, 2, 2, 2, 2, 73
#define PPUTLIMPL_UDEC_1167u 0x48Fu, 0, 0, 0, 0, 10, 34, 3, 389
#define PPUTLIMPL_UDEC_1166u 0x48Eu, 0, 0, 0, 0, 10, 34, 2, 11, 53
#define PPUTLIMPL_UDEC_1165u 0x48Du, 0, 0, 0, 0, 10, 34, 5, 233
#define PPUTLIMPL_UDEC_1164u 0x48Cu, 0, 0, 0, 0, 10, 34, 2, 2, 3, 97
#define PPUTLIMPL_UDEC_1163u 0x48Bu, 0, 0, 0, 0, 10, 34,
#define PPUTLIMPL_UDEC_1162u 0x48Au, 0, 0, 0, 0, 10, 34, 2, 7, 83
#define PPUTLIMPL_UDEC_1161u 0x489u, 0, 0, 0, 0, 10, 34, 3, 3, 3, 43
#define PPUTLIMPL_UDEC_1160u 0x488u, 0, 0, 0, 0, 10, 34, 2, 2, 2, 5, 29
#define PPUTLIMPL_UDEC_1159u 0x487u, 0, 0, 0, 0, 10, 34, 19, 61
#define PPUTLIMPL_UDEC_1158u 0x486u, 0, 0, 0, 0, 10, 34, 2, 3, 193
#define PPUTLIMPL_UDEC_1157u 0x485u, 0, 0, 0, 0, 10, 34, 13, 89
#define PPUTLIMPL_UDEC_1156u 0x484u, 0, 0, 0, 0, 10, 34, 2, 2, 17, 17
#define PPUTLIMPL_UDEC_1155u 0x483u, 0, 0, 0, 0, 10, 33, 3, 5, 7, 11
#define PPUTLIMPL_UDEC_1154u 0x482u, 0, 0, 0, 0, 10, 33, 2, 577
#define PPUTLIMPL_UDEC_1153u 0x481u, 0, 0, 0, 0, 10, 33,
#define PPUTLIMPL_UDEC_1152u 0x480u, 0, 0, 0, 0, 10, 33, 2, 2, 2, 2, 2, 2, 2, 3, 3
#define PPUTLIMPL_UDEC_1151u 0x47Fu, 0, 0, 0, 0, 10, 33,
#define PPUTLIMPL_UDEC_1150u 0x47Eu, 0, 0, 0, 0, 10, 33, 2, 5, 5, 23
#define PPUTLIMPL_UDEC_1149u 0x47Du, 0, 0, 0, 0, 10, 33, 3, 383
#define PPUTLIMPL_UDEC_1148u 0x47Cu, 0, 0, 0, 0, 10, 33, 2, 2, 7, 41
#define PPUTLIMPL_UDEC_1147u 0x47Bu, 0, 0, 0, 0, 10, 33, 31, 37
#define PPUTLIMPL_UDEC_1146u 0x47Au, 0, 0, 0, 0, 10, 33, 2, 3, 191
#define PPUTLIMPL_UDEC_1145u 0x479u, 0, 0, 0, 0, 10, 33, 5, 229
#define PPUTLIMPL_UDEC_1144u 0x478u, 0, 0, 0, 0, 10, 33, 2, 2, 2, 11, 13
#define PPUTLIMPL_UDEC_1143u 0x477u, 0, 0, 0, 0, 10, 33, 3, 3, 127
#define PPUTLIMPL_UDEC_1142u 0x476u, 0, 0, 0, 0, 10, 33, 2, 571
#define PPUTLIMPL_UDEC_1141u 0x475u, 0, 0, 0, 0, 10, 33, 7, 163
#define PPUTLIMPL_UDEC_1140u 0x474u, 0, 0, 0, 0, 10, 33, 2, 2, 3, 5, 19
#define PPUTLIMPL_UDEC_1139u 0x473u, 0, 0, 0, 0, 10, 33, 17, 67
#define PPUTLIMPL_UDEC_1138u 0x472u, 0, 0, 0, 0, 10, 33, 2, 569
#define PPUTLIMPL_UDEC_1137u 0x471u, 0, 0, 0, 0, 10, 33, 3, 379
#define PPUTLIMPL_UDEC_1136u 0x470u, 0, 0, 0, 0, 10, 33, 2, 2, 2, 2, 71
#define PPUTLIMPL_UDEC_1135u 0x46Fu, 0, 0, 0, 0, 10, 33, 5, 227
#define PPUTLIMPL_UDEC_1134u 0x46Eu, 0, 0, 0, 0, 10, 33, 2, 3, 3, 3, 3, 7
#define PPUTLIMPL_UDEC_1133u 0x46Du, 0, 0, 0, 0, 10, 33, 11, 103
#define PPUTLIMPL_UDEC_1132u 0x46Cu, 0, 0, 0, 0, 10, 33, 2, 2, 283
#define PPUTLIMPL_UDEC_1131u 0x46Bu, 0, 0, 0, 0, 10, 33, 3, 13, 29
#define PPUTLIMPL_UDEC_1130u 0x46Au, 0, 0, 0, 0, 10, 33, 2, 5, 113
#define PPUTLIMPL_UDEC_1129u 0x469u, 0, 0, 0, 0, 10, 33,
#define PPUTLIMPL_UDEC_1128u 0x468u, 0, 0, 0, 0, 10, 33, 2, 2, 2, 3, 47
#define PPUTLIMPL_UDEC_1127u 0x467u, 0, 0, 0, 0, 10, 33, 7, 7, 23
#define PPUTLIMPL_UDEC_1126u 0x466u, 0, 0, 0, 0, 10, 33, 2, 563
#define PPUTLIMPL_UDEC_1125u 0x465u, 0, 0, 0, 0, 10, 33, 3, 3, 5, 5, 5
#define PPUTLIMPL_UDEC_1124u 0x464u, 0, 0, 0, 0, 10, 33, 2, 2, 281
#define PPUTLIMPL_UDEC_1123u 0x463u, 0, 0, 0, 0, 10, 33,
#define PPUTLIMPL_UDEC_1122u 0x462u, 0, 0, 0, 0, 10, 33, 2, 3, 11, 17
#define PPUTLIMPL_UDEC_1121u 0x461u, 0, 0, 0, 0, 10, 33, 19, 59
#define PPUTLIMPL_UDEC_1120u 0x460u, 0, 0, 0, 0, 10, 33, 2, 2, 2, 2, 2, 5, 7
#define PPUTLIMPL_UDEC_1119u 0x45Fu, 0, 0, 0, 0, 10, 33, 3, 373
#define PPUTLIMPL_UDEC_1118u 0x45Eu, 0, 0, 0, 0, 10, 33, 2, 13, 43
#define PPUTLIMPL_UDEC_1117u 0x45Du, 0, 0, 0, 0, 10, 33,
#define PPUTLIMPL_UDEC_1116u 0x45Cu, 0, 0, 0, 0, 10, 33, 2, 2, 3, 3, 31
#define PPUTLIMPL_UDEC_1115u 0x45Bu, 0, 0, 0, 0, 10, 33, 5, 223
#define PPUTLIMPL_UDEC_1114u 0x45Au, 0, 0, 0, 0, 10, 33, 2, 557
#define PPUTLIMPL_UDEC_1113u 0x459u, 0, 0, 0, 0, 10, 33, 3, 7, 53
#define PPUTLIMPL_UDEC_1112u 0x458u, 0, 0, 0, 0, 10, 33, 2, 2, 2, 139
#define PPUTLIMPL_UDEC_1111u 0x457u, 0, 0, 0, 0, 10, 33, 11, 101
#define PPUTLIMPL_UDEC_1110u 0x456u, 0, 0, 0, 0, 10, 33, 2, 3, 5, 37
#define PPUTLIMPL_UDEC_1109u 0x455u, 0, 0, 0, 0, 10, 33,
#define PPUTLIMPL_UDEC_1108u 0x454u, 0, 0, 0, 0, 10, 33, 2, 2, 277
#define PPUTLIMPL_UDEC_1107u 0x453u, 0, 0, 0, 0, 10, 33, 3, 3, 3, 41
#define PPUTLIMPL_UDEC_1106u 0x452u, 0, 0, 0, 0, 10, 33, 2, 7, 79
#define PPUTLIMPL_UDEC_1105u 0x451u, 0, 0, 0, 0, 10, 33, 5, 13, 17
#define PPUTLIMPL_UDEC_1104u 0x450u, 0, 0, 0, 0, 10, 33, 2, 2, 2, 2, 3, 23
#define PPUTLIMPL_UDEC_1103u 0x44Fu, 0, 0, 0, 0, 10, 33,
#define PPUTLIMPL_UDEC_1102u 0x44Eu, 0, 0, 0, 0, 10, 33, 2, 19, 29
#define PPUTLIMPL_UDEC_1101u 0x44Du, 0, 0, 0, 0, 10, 33, 3, 367
#define PPUTLIMPL_UDEC_1100u 0x44Cu, 0, 0, 0, 0, 10, 33, 2, 2, 5, 5, 11
#define PPUTLIMPL_UDEC_1099u 0x44Bu, 0, 0, 0, 0, 10, 33, 7, 157
#define PPUTLIMPL_UDEC_1098u 0x44Au, 0, 0, 0, 0, 10, 33, 2, 3, 3, 61
#define PPUTLIMPL_UDEC_1097u 0x449u, 0, 0, 0, 0, 10, 33,
#define PPUTLIMPL_UDEC_1096u 0x448u, 0, 0, 0, 0, 10, 33, 2, 2, 2, 137
#define PPUTLIMPL_UDEC_1095u 0x447u, 0, 0, 0, 0, 10, 33, 3, 5, 73
#define PPUTLIMPL_UDEC_1094u 0x446u, 0, 0, 0, 0, 10, 33, 2, 547
#define PPUTLIMPL_UDEC_1093u 0x445u, 0, 0, 0, 0, 10, 33,
#define PPUTLIMPL_UDEC_1092u 0x444u, 0, 0, 0, 0, 10, 33, 2, 2, 3, 7, 13
#define PPUTLIMPL_UDEC_1091u 0x443u, 0, 0, 0, 0, 10, 33,
#define PPUTLIMPL_UDEC_1090u 0x442u, 0, 0, 0, 0, 10, 33, 2, 5, 109
#define PPUTLIMPL_UDEC_1089u 0x441u, 0, 0, 0, 0, 10, 33, 3, 3, 11, 11
#define PPUTLIMPL_UDEC_1088u 0x440u, 0, 0, 0, 0, 10, 32, 2, 2, 2, 2, 2, 2, 17
#define PPUTLIMPL_UDEC_1087u 0x43Fu, 0, 0, 0, 0, 10, 32,
#define PPUTLIMPL_UDEC_1086u 0x43Eu, 0, 0, 0, 0, 10, 32, 2, 3, 181
#define PPUTLIMPL_UDEC_1085u 0x43Du, 0, 0, 0, 0, 10, 32, 5, 7, 31
#define PPUTLIMPL_UDEC_1084u 0x43Cu, 0, 0, 0, 0, 10, 32, 2, 2, 271
#define PPUTLIMPL_UDEC_1083u 0x43Bu, 0, 0, 0, 0, 10, 32, 3, 19, 19
#define PPUTLIMPL_UDEC_1082u 0x43Au, 0, 0, 0, 0, 10, 32, 2, 541
#define PPUTLIMPL_UDEC_1081u 0x439u, 0, 0, 0, 0, 10, 32, 23, 47
#define PPUTLIMPL_UDEC_1080u 0x438u, 0, 0, 0, 0, 10, 32, 2, 2, 2, 3, 3, 3, 5
#define PPUTLIMPL_UDEC_1079u 0x437u, 0, 0, 0, 0, 10, 32, 13, 83
#define PPUTLIMPL_UDEC_1078u 0x436u, 0, 0, 0, 0, 10, 32, 2, 7, 7, 11
#define PPUTLIMPL_UDEC_1077u 0x435u, 0, 0, 0, 0, 10, 32, 3, 359
#define PPUTLIMPL_UDEC_1076u 0x434u, 0, 0, 0, 0, 10, 32, 2, 2, 269
#define PPUTLIMPL_UDEC_1075u 0x433u, 0, 0, 0, 0, 10, 32, 5, 5, 43
#define PPUTLIMPL_UDEC_1074u 0x432u, 0, 0, 0, 0, 10, 32, 2, 3, 179
#define PPUTLIMPL_UDEC_1073u 0x431u, 0, 0, 0, 0, 10, 32, 29, 37
#define PPUTLIMPL_UDEC_1072u 0x430u, 0, 0, 0, 0, 10, 32, 2, 2, 2, 2, 67
#define PPUTLIMPL_UDEC_1071u 0x42Fu, 0, 0, 0, 0, 10, 32, 3, 3, 7, 17
#define PPUTLIMPL_UDEC_1070u 0x42Eu, 0, 0, 0, 0, 10, 32, 2, 5, 107
#define PPUTLIMPL_UDEC_1069u 0x42Du, 0, 0, 0, 0, 10, 32,
#define PPUTLIMPL_UDEC_1068u 0x42Cu, 0, 0, 0, 0, 10, 32, 2, 2, 3, 89
#define PPUTLIMPL_UDEC_1067u 0x42Bu, 0, 0, 0, 0, 10, 32, 11, 97
#define PPUTLIMPL_UDEC_1066u 0x42Au, 0, 0, 0, 0, 10, 32, 2, 13, 41
#define PPUTLIMPL_UDEC_1065u 0x429u, 0, 0, 0, 0, 10, 32, 3, 5, 71
#define PPUTLIMPL_UDEC_1064u 0x428u, 0, 0, 0, 0, 10, 32, 2, 2, 2, 7, 19
#define PPUTLIMPL_UDEC_1063u 0x427u, 0, 0, 0, 0, 10, 32,
#define PPUTLIMPL_UDEC_1062u 0x426u, 0, 0, 0, 0, 10, 32, 2, 3, 3, 59
#define PPUTLIMPL_UDEC_1061u 0x425u, 0, 0, 0, 0, 10, 32,
#define PPUTLIMPL_UDEC_1060u 0x424u, 0, 0, 0, 0, 10, 32, 2, 2, 5, 53
#define PPUTLIMPL_UDEC_1059u 0x423u, 0, 0, 0, 0, 10, 32, 3, 353
#define PPUTLIMPL_UDEC_1058u 0x422u, 0, 0, 0, 0, 10, 32, 2, 23, 23
#define PPUTLIMPL_UDEC_1057u 0x421u, 0, 0, 0, 0, 10, 32, 7, 151
#define PPUTLIMPL_UDEC_1056u 0x420u, 0, 0, 0, 0, 10, 32, 2, 2, 2, 2, 2, 3, 11
#define PPUTLIMPL_UDEC_1055u 0x41Fu, 0, 0, 0, 0, 10, 32, 5, 211
#define PPUTLIMPL_UDEC_1054u 0x41Eu, 0, 0, 0, 0, 10, 32, 2, 17, 31
#define PPUTLIMPL_UDEC_1053u 0x41Du, 0, 0, 0, 0, 10, 32, 3, 3, 3, 3, 13
#define PPUTLIMPL_UDEC_1052u 0x41Cu, 0, 0, 0, 0, 10, 32, 2, 2, 263
#define PPUTLIMPL_UDEC_1051u 0x41Bu, 0, 0, 0, 0, 10, 32,
#define PPUTLIMPL_UDEC_1050u 0x41Au, 0, 0, 0, 0, 10, 32, 2, 3, 5, 5, 7
#define PPUTLIMPL_UDEC_1049u 0x419u, 0, 0, 0, 0, 10, 32,
#define PPUTLIMPL_UDEC_1048u 0x418u, 0, 0, 0, 0, 10, 32, 2, 2, 2, 131
#define PPUTLIMPL_UDEC_1047u 0x417u, 0, 0, 0, 0, 10, 32, 3, 349
#define PPUTLIMPL_UDEC_1046u 0x416u, 0, 0, 0, 0, 10, 32, 2, 523
#define PPUTLIMPL_UDEC_1045u 0x415u, 0, 0, 0, 0, 10, 32, 5, 11, 19
#define PPUTLIMPL_UDEC_1044u 0x414u, 0, 0, 0, 0, 10, 32, 2, 2, 3, 3, 29
#define PPUTLIMPL_UDEC_1043u 0x413u, 0, 0, 0, 0, 10, 32, 7, 149
#define PPUTLIMPL_UDEC_1042u 0x412u, 0, 0, 0, 0, 10, 32, 2, 521
#define PPUTLIMPL_UDEC_1041u 0x411u, 0, 0, 0, 0, 10, 32, 3, 347
#define PPUTLIMPL_UDEC_1040u 0x410u, 0, 0, 0, 0, 10, 32, 2, 2, 2, 2, 5, 13
#define PPUTLIMPL_UDEC_1039u 0x40Fu, 0, 0, 0, 0, 10, 32,
#define PPUTLIMPL_UDEC_1038u 0x40Eu, 0, 0, 0, 0, 10, 32, 2, 3, 173
#define PPUTLIMPL_UDEC_1037u 0x40Du, 0, 0, 0, 0, 10, 32, 17, 61
#define PPUTLIMPL_UDEC_1036u 0x40Cu, 0, 0, 0, 0, 10, 32, 2, 2, 7, 37
#define PPUTLIMPL_UDEC_1035u 0x40Bu, 0, 0, 0, 0, 10, 32, 3, 3, 5, 23
#define PPUTLIMPL_UDEC_1034u 0x40Au, 0, 0, 0, 0, 10, 32, 2, 11, 47
#define PPUTLIMPL_UDEC_1033u 0x409u, 0, 0, 0, 0, 10, 32,
#define PPUTLIMPL_UDEC_1032u 0x408u, 0, 0, 0, 0, 10, 32, 2, 2, 2, 3, 43
#define PPUTLIMPL_UDEC_1031u 0x407u, 0, 0, 0, 0, 10, 32,
#define PPUTLIMPL_UDEC_1030u 0x406u, 0, 0, 0, 0, 10, 32, 2, 5, 103
#define PPUTLIMPL_UDEC_1029u 0x405u, 0, 0, 0, 0, 10, 32, 3, 7, 7, 7
#define PPUTLIMPL_UDEC_1028u 0x404u, 0, 0, 0, 0, 10, 32, 2, 2, 257
#define PPUTLIMPL_UDEC_1027u 0x403u, 0, 0, 0, 0, 10, 32, 13, 79
#define PPUTLIMPL_UDEC_1026u 0x402u, 0, 0, 0, 0, 10, 32, 2, 3, 3, 3, 19
#define PPUTLIMPL_UDEC_1025u 0x401u, 0, 0, 0, 0, 10, 32, 5, 5, 41
#define PPUTLIMPL_UDEC_1024u 0x400u, 0, 0, 0, 0, 10, 32, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
#define PPUTLIMPL_UDEC_1023u 0x3FFu, 0, 0, 0, 0, 9, 31, 3, 11, 31
#define PPUTLIMPL_UDEC_1022u 0x3FEu, 0, 0, 0, 0, 9, 31, 2, 7, 73
#define PPUTLIMPL_UDEC_1021u 0x3FDu, 0, 0, 0, 0, 9, 31,
#define PPUTLIMPL_UDEC_1020u 0x3FCu, 0, 0, 0, 0, 9, 31, 2, 2, 3, 5, 17
#define PPUTLIMPL_UDEC_1019u 0x3FBu, 0, 0, 0, 0, 9, 31,
#define PPUTLIMPL_UDEC_1018u 0x3FAu, 0, 0, 0, 0, 9, 31, 2, 509
#define PPUTLIMPL_UDEC_1017u 0x3F9u, 0, 0, 0, 0, 9, 31, 3, 3, 113
#define PPUTLIMPL_UDEC_1016u 0x3F8u, 0, 0, 0, 0, 9, 31, 2, 2, 2, 127
#define PPUTLIMPL_UDEC_1015u 0x3F7u, 0, 0, 0, 0, 9, 31, 5, 7, 29
#define PPUTLIMPL_UDEC_1014u 0x3F6u, 0, 0, 0, 0, 9, 31, 2, 3, 13, 13
#define PPUTLIMPL_UDEC_1013u 0x3F5u, 0, 0, 0, 0, 9, 31,
#define PPUTLIMPL_UDEC_1012u 0x3F4u, 0, 0, 0, 0, 9, 31, 2, 2, 11, 23
#define PPUTLIMPL_UDEC_1011u 0x3F3u, 0, 0, 0, 0, 9, 31, 3, 337
#define PPUTLIMPL_UDEC_1010u 0x3F2u, 0, 0, 0, 0, 9, 31, 2, 5, 101
#define PPUTLIMPL_UDEC_1009u 0x3F1u, 0, 0, 0, 0, 9, 31,
#define PPUTLIMPL_UDEC_1008u 0x3F0u, 0, 0, 0, 0, 9, 31, 2, 2, 2, 2, 3, 3, 7
#define PPUTLIMPL_UDEC_1007u 0x3EFu, 0, 0, 0, 0, 9, 31, 19, 53
#define PPUTLIMPL_UDEC_1006u 0x3EEu, 0, 0, 0, 0, 9, 31, 2, 503
#define PPUTLIMPL_UDEC_1005u 0x3EDu, 0, 0, 0, 0, 9, 31, 3, 5, 67
#define PPUTLIMPL_UDEC_1004u 0x3ECu, 0, 0, 0, 0, 9, 31, 2, 2, 251
#define PPUTLIMPL_UDEC_1003u 0x3EBu, 0, 0, 0, 0, 9, 31, 17, 59
#define PPUTLIMPL_UDEC_1002u 0x3EAu, 0, 0, 0, 0, 9, 31, 2, 3, 167
#define PPUTLIMPL_UDEC_1001u 0x3E9u, 0, 0, 0, 0, 9, 31, 7, 11, 13
#define PPUTLIMPL_UDEC_1000u 0x3E8u, 0, 0, 0, 0, 9, 31, 2, 2, 2, 5, 5, 5
#define PPUTLIMPL_UDEC_999u  0x3E7u, 0, 0, 0, 0, 9, 31, 3, 3, 3, 37
#define PPUTLIMPL_UDEC_998u  0x3E6u, 0, 0, 0, 0, 9, 31, 2, 499
#define PPUTLIMPL_UDEC_997u  0x3E5u, 0, 0, 0, 0, 9, 31,
#define PPUTLIMPL_UDEC_996u  0x3E4u, 0, 0, 0, 0, 9, 31, 2, 2, 3, 83
#define PPUTLIMPL_UDEC_995u  0x3E3u, 0, 0, 0, 0, 9, 31, 5, 199
#define PPUTLIMPL_UDEC_994u  0x3E2u, 0, 0, 0, 0, 9, 31, 2, 7, 71
#define PPUTLIMPL_UDEC_993u  0x3E1u, 0, 0, 0, 0, 9, 31, 3, 331
#define PPUTLIMPL_UDEC_992u  0x3E0u, 0, 0, 0, 0, 9, 31, 2, 2, 2, 2, 2, 31
#define PPUTLIMPL_UDEC_991u  0x3DFu, 0, 0, 0, 0, 9, 31,
#define PPUTLIMPL_UDEC_990u  0x3DEu, 0, 0, 0, 0, 9, 31, 2, 3, 3, 5, 11
#define PPUTLIMPL_UDEC_989u  0x3DDu, 0, 0, 0, 0, 9, 31, 23, 43
#define PPUTLIMPL_UDEC_988u  0x3DCu, 0, 0, 0, 0, 9, 31, 2, 2, 13, 19
#define PPUTLIMPL_UDEC_987u  0x3DBu, 0, 0, 0, 0, 9, 31, 3, 7, 47
#define PPUTLIMPL_UDEC_986u  0x3DAu, 0, 0, 0, 0, 9, 31, 2, 17, 29
#define PPUTLIMPL_UDEC_985u  0x3D9u, 0, 0, 0, 0, 9, 31, 5, 197
#define PPUTLIMPL_UDEC_984u  0x3D8u, 0, 0, 0, 0, 9, 31, 2, 2, 2, 3, 41
#define PPUTLIMPL_UDEC_983u  0x3D7u, 0, 0, 0, 0, 9, 31,
#define PPUTLIMPL_UDEC_982u  0x3D6u, 0, 0, 0, 0, 9, 31, 2, 491
#define PPUTLIMPL_UDEC_981u  0x3D5u, 0, 0, 0, 0, 9, 31, 3, 3, 109
#define PPUTLIMPL_UDEC_980u  0x3D4u, 0, 0, 0, 0, 9, 31, 2, 2, 5, 7, 7
#define PPUTLIMPL_UDEC_979u  0x3D3u, 0, 0, 0, 0, 9, 31, 11, 89
#define PPUTLIMPL_UDEC_978u  0x3D2u, 0, 0, 0, 0, 9, 31, 2, 3, 163
#define PPUTLIMPL_UDEC_977u  0x3D1u, 0, 0, 0, 0, 9, 31,
#define PPUTLIMPL_UDEC_976u  0x3D0u, 0, 0, 0, 0, 9, 31, 2, 2, 2, 2, 61
#define PPUTLIMPL_UDEC_975u  0x3CFu, 0, 0, 0, 0, 9, 31, 3, 5, 5, 13
#define PPUTLIMPL_UDEC_974u  0x3CEu, 0, 0, 0, 0, 9, 31, 2, 487
#define PPUTLIMPL_UDEC_973u  0x3CDu, 0, 0, 0, 0, 9, 31, 7, 139
#define PPUTLIMPL_UDEC_972u  0x3CCu, 0, 0, 0, 0, 9, 31, 2, 2, 3, 3, 3, 3, 3
#define PPUTLIMPL_UDEC_971u  0x3CBu, 0, 0, 0, 0, 9, 31,
#define PPUTLIMPL_UDEC_970u  0x3CAu, 0, 0, 0, 0, 9, 31, 2, 5, 97
#define PPUTLIMPL_UDEC_969u  0x3C9u, 0, 0, 0, 0, 9, 31, 3, 17, 19
#define PPUTLIMPL_UDEC_968u  0x3C8u, 0, 0, 0, 0, 9, 31, 2, 2, 2, 11, 11
#define PPUTLIMPL_UDEC_967u  0x3C7u, 0, 0, 0, 0, 9, 31,
#define PPUTLIMPL_UDEC_966u  0x3C6u, 0, 0, 0, 0, 9, 31, 2, 3, 7, 23
#define PPUTLIMPL_UDEC_965u  0x3C5u, 0, 0, 0, 0, 9, 31, 5, 193
#define PPUTLIMPL_UDEC_964u  0x3C4u, 0, 0, 0, 0, 9, 31, 2, 2, 241
#define PPUTLIMPL_UDEC_963u  0x3C3u, 0, 0, 0, 0, 9, 31, 3, 3, 107
#define PPUTLIMPL_UDEC_962u  0x3C2u, 0, 0, 0, 0, 9, 31, 2, 13, 37
#define PPUTLIMPL_UDEC_961u  0x3C1u, 0, 0, 0, 0, 9, 31, 31, 31
#define PPUTLIMPL_UDEC_960u  0x3C0u, 0, 0, 0, 0, 9, 30, 2, 2, 2, 2, 2, 2, 3, 5
#define PPUTLIMPL_UDEC_959u  0x3BFu, 0, 0, 0, 0, 9, 30, 7, 137
#define PPUTLIMPL_UDEC_958u  0x3BEu, 0, 0, 0, 0, 9, 30, 2, 479
#define PPUTLIMPL_UDEC_957u  0x3BDu, 0, 0, 0, 0, 9, 30, 3, 11, 29
#define PPUTLIMPL_UDEC_956u  0x3BCu, 0, 0, 0, 0, 9, 30, 2, 2, 239
#define PPUTLIMPL_UDEC_955u  0x3BBu, 0, 0, 0, 0, 9, 30, 5, 191
#define PPUTLIMPL_UDEC_954u  0x3BAu, 0, 0, 0, 0, 9, 30, 2, 3, 3, 53
#define PPUTLIMPL_UDEC_953u  0x3B9u, 0, 0, 0, 0, 9, 30,
#define PPUTLIMPL_UDEC_952u  0x3B8u, 0, 0, 0, 0, 9, 30, 2, 2, 2, 7, 17
#define PPUTLIMPL_UDEC_951u  0x3B7u, 0, 0, 0, 0, 9, 30, 3, 317
#define PPUTLIMPL_UDEC_950u  0x3B6u, 0, 0, 0, 0, 9, 30, 2, 5, 5, 19
#define PPUTLIMPL_UDEC_949u  0x3B5u, 0, 0, 0, 0, 9, 30, 13, 73
#define PPUTLIMPL_UDEC_948u  0x3B4u, 0, 0, 0, 0, 9, 30, 2, 2, 3, 79
#define PPUTLIMPL_UDEC_947u  0x3B3u, 0, 0, 0, 0, 9, 30,
#define PPUTLIMPL_UDEC_946u  0x3B2u, 0, 0, 0, 0, 9, 30, 2, 11, 43
#define PPUTLIMPL_UDEC_945u  0x3B1u, 0, 0, 0, 0, 9, 30, 3, 3, 3, 5, 7
#define PPUTLIMPL_UDEC_944u  0x3B0u, 0, 0, 0, 0, 9, 30, 2, 2, 2, 2, 59
#define PPUTLIMPL_UDEC_943u  0x3AFu, 0, 0, 0, 0, 9, 30, 23, 41
#define PPUTLIMPL_UDEC_942u  0x3AEu, 0, 0, 0, 0, 9, 30, 2, 3, 157
#define PPUTLIMPL_UDEC_941u  0x3ADu, 0, 0, 0, 0, 9, 30,
#define PPUTLIMPL_UDEC_940u  0x3ACu, 0, 0, 0, 0, 9, 30, 2, 2, 5, 47
#define PPUTLIMPL_UDEC_939u  0x3ABu, 0, 0, 0, 0, 9, 30, 3, 313
#define PPUTLIMPL_UDEC_938u  0x3AAu, 0, 0, 0, 0, 9, 30, 2, 7, 67
#define PPUTLIMPL_UDEC_937u  0x3A9u, 0, 0, 0, 0, 9, 30,
#define PPUTLIMPL_UDEC_936u  0x3A8u, 0, 0, 0, 0, 9, 30, 2, 2, 2, 3, 3, 13
#define PPUTLIMPL_UDEC_935u  0x3A7u, 0, 0, 0, 0, 9, 30, 5, 11, 17
#define PPUTLIMPL_UDEC_934u  0x3A6u, 0, 0, 0, 0, 9, 30, 2, 467
#define PPUTLIMPL_UDEC_933u  0x3A5u, 0, 0, 0, 0, 9, 30, 3, 311
#define PPUTLIMPL_UDEC_932u  0x3A4u, 0, 0, 0, 0, 9, 30, 2, 2, 233
#define PPUTLIMPL_UDEC_931u  0x3A3u, 0, 0, 0, 0, 9, 30, 7, 7, 19
#define PPUTLIMPL_UDEC_930u  0x3A2u, 0, 0, 0, 0, 9, 30, 2, 3, 5, 31
#define PPUTLIMPL_UDEC_929u  0x3A1u, 0, 0, 0, 0, 9, 30,
#define PPUTLIMPL_UDEC_928u  0x3A0u, 0, 0, 0, 0, 9, 30, 2, 2, 2, 2, 2, 29
#define PPUTLIMPL_UDEC_927u  0x39Fu, 0, 0, 0, 0, 9, 30, 3, 3, 103
#define PPUTLIMPL_UDEC_926u  0x39Eu, 0, 0, 0, 0, 9, 30, 2, 463
#define PPUTLIMPL_UDEC_925u  0x39Du, 0, 0, 0, 0, 9, 30, 5, 5, 37
#define PPUTLIMPL_UDEC_924u  0x39Cu, 0, 0, 0, 0, 9, 30, 2, 2, 3, 7, 11
#define PPUTLIMPL_UDEC_923u  0x39Bu, 0, 0, 0, 0, 9, 30, 13, 71
#define PPUTLIMPL_UDEC_922u  0x39Au, 0, 0, 0, 0, 9, 30, 2, 461
#define PPUTLIMPL_UDEC_921u  0x399u, 0, 0, 0, 0, 9, 30, 3, 307
#define PPUTLIMPL_UDEC_920u  0x398u, 0, 0, 0, 0, 9, 30, 2, 2, 2, 5, 23
#define PPUTLIMPL_UDEC_919u  0x397u, 0, 0, 0, 0, 9, 30,
#define PPUTLIMPL_UDEC_918u  0x396u, 0, 0, 0, 0, 9, 30, 2, 3, 3, 3, 17
#define PPUTLIMPL_UDEC_917u  0x395u, 0, 0, 0, 0, 9, 30, 7, 131
#define PPUTLIMPL_UDEC_916u  0x394u, 0, 0, 0, 0, 9, 30, 2, 2, 229
#define PPUTLIMPL_UDEC_915u  0x393u, 0, 0, 0, 0, 9, 30, 3, 5, 61
#define PPUTLIMPL_UDEC_914u  0x392u, 0, 0, 0, 0, 9, 30, 2, 457
#define PPUTLIMPL_UDEC_913u  0x391u, 0, 0, 0, 0, 9, 30, 11, 83
#define PPUTLIMPL_UDEC_912u  0x390u, 0, 0, 0, 0, 9, 30, 2, 2, 2, 2, 3, 19
#define PPUTLIMPL_UDEC_911u  0x38Fu, 0, 0, 0, 0, 9, 30,
#define PPUTLIMPL_UDEC_910u  0x38Eu, 0, 0, 0, 0, 9, 30, 2, 5, 7, 13
#define PPUTLIMPL_UDEC_909u  0x38Du, 0, 0, 0, 0, 9, 30, 3, 3, 101
#define PPUTLIMPL_UDEC_908u  0x38Cu, 0, 0, 0, 0, 9, 30, 2, 2, 227
#define PPUTLIMPL_UDEC_907u  0x38Bu, 0, 0, 0, 0, 9, 30,
#define PPUTLIMPL_UDEC_906u  0x38Au, 0, 0, 0, 0, 9, 30, 2, 3, 151
#define PPUTLIMPL_UDEC_905u  0x389u, 0, 0, 0, 0, 9, 30, 5, 181
#define PPUTLIMPL_UDEC_904u  0x388u, 0, 0, 0, 0, 9, 30, 2, 2, 2, 113
#define PPUTLIMPL_UDEC_903u  0x387u, 0, 0, 0, 0, 9, 30, 3, 7, 43
#define PPUTLIMPL_UDEC_902u  0x386u, 0, 0, 0, 0, 9, 30, 2, 11, 41
#define PPUTLIMPL_UDEC_901u  0x385u, 0, 0, 0, 0, 9, 30, 17, 53
#define PPUTLIMPL_UDEC_900u  0x384u, 0, 0, 0, 0, 9, 30, 2, 2, 3, 3, 5, 5
#define PPUTLIMPL_UDEC_899u  0x383u, 0, 0, 0, 0, 9, 29, 29, 31
#define PPUTLIMPL_UDEC_898u  0x382u, 0, 0, 0, 0, 9, 29, 2, 449
#define PPUTLIMPL_UDEC_897u  0x381u, 0, 0, 0, 0, 9, 29, 3, 13, 23
#define PPUTLIMPL_UDEC_896u  0x380u, 0, 0, 0, 0, 9, 29, 2, 2, 2, 2, 2, 2, 2, 7
#define PPUTLIMPL_UDEC_895u  0x37Fu, 0, 0, 0, 0, 9, 29, 5, 179
#define PPUTLIMPL_UDEC_894u  0x37Eu, 0, 0, 0, 0, 9, 29, 2, 3, 149
#define PPUTLIMPL_UDEC_893u  0x37Du, 0, 0, 0, 0, 9, 29, 19, 47
#define PPUTLIMPL_UDEC_892u  0x37Cu, 0, 0, 0, 0, 9, 29, 2, 2, 223
#define PPUTLIMPL_UDEC_891u  0x37Bu, 0, 0, 0, 0, 9, 29, 3, 3, 3, 3, 11
#define PPUTLIMPL_UDEC_890u  0x37Au, 0, 0, 0, 0, 9, 29, 2, 5, 89
#define PPUTLIMPL_UDEC_889u  0x379u, 0, 0, 0, 0, 9, 29, 7, 127
#define PPUTLIMPL_UDEC_888u  0x378u, 0, 0, 0, 0, 9, 29, 2, 2, 2, 3, 37
#define PPUTLIMPL_UDEC_887u  0x377u, 0, 0, 0, 0, 9, 29,
#define PPUTLIMPL_UDEC_886u  0x376u, 0, 0, 0, 0, 9, 29, 2, 443
#define PPUTLIMPL_UDEC_885u  0x375u, 0, 0, 0, 0, 9, 29, 3, 5, 59
#define PPUTLIMPL_UDEC_884u  0x374u, 0, 0, 0, 0, 9, 29, 2, 2, 13, 17
#define PPUTLIMPL_UDEC_883u  0x373u, 0, 0, 0, 0, 9, 29,
#define PPUTLIMPL_UDEC_882u  0x372u, 0, 0, 0, 0, 9, 29, 2, 3, 3, 7, 7
#define PPUTLIMPL_UDEC_881u  0x371u, 0, 0, 0, 0, 9, 29,
#define PPUTLIMPL_UDEC_880u  0x370u, 0, 0, 0, 0, 9, 29, 2, 2, 2, 2, 5, 11
#define PPUTLIMPL_UDEC_879u  0x36Fu, 0, 0, 0, 0, 9, 29, 3, 293
#define PPUTLIMPL_UDEC_878u  0x36Eu, 0, 0, 0, 0, 9, 29, 2, 439
#define PPUTLIMPL_UDEC_877u  0x36Du, 0, 0, 0, 0, 9, 29,
#define PPUTLIMPL_UDEC_876u  0x36Cu, 0, 0, 0, 0, 9, 29, 2, 2, 3, 73
#define PPUTLIMPL_UDEC_875u  0x36Bu, 0, 0, 0, 0, 9, 29, 5, 5, 5, 7
#define PPUTLIMPL_UDEC_874u  0x36Au, 0, 0, 0, 0, 9, 29, 2, 19, 23
#define PPUTLIMPL_UDEC_873u  0x369u, 0, 0, 0, 0, 9, 29, 3, 3, 97
#define PPUTLIMPL_UDEC_872u  0x368u, 0, 0, 0, 0, 9, 29, 2, 2, 2, 109
#define PPUTLIMPL_UDEC_871u  0x367u, 0, 0, 0, 0, 9, 29, 13, 67
#define PPUTLIMPL_UDEC_870u  0x366u, 0, 0, 0, 0, 9, 29, 2, 3, 5, 29
#define PPUTLIMPL_UDEC_869u  0x365u, 0, 0, 0, 0, 9, 29, 11, 79
#define PPUTLIMPL_UDEC_868u  0x364u, 0, 0, 0, 0, 9, 29, 2, 2, 7, 31
#define PPUTLIMPL_UDEC_867u  0x363u, 0, 0, 0, 0, 9, 29, 3, 17, 17
#define PPUTLIMPL_UDEC_866u  0x362u, 0, 0, 0, 0, 9, 29, 2, 433
#define PPUTLIMPL_UDEC_865u  0x361u, 0, 0, 0, 0, 9, 29, 5, 173
#define PPUTLIMPL_UDEC_864u  0x360u, 0, 0, 0, 0, 9, 29, 2, 2, 2, 2, 2, 3, 3, 3
#define PPUTLIMPL_UDEC_863u  0x35Fu, 0, 0, 0, 0, 9, 29,
#define PPUTLIMPL_UDEC_862u  0x35Eu, 0, 0, 0, 0, 9, 29, 2, 431
#define PPUTLIMPL_UDEC_861u  0x35Du, 0, 0, 0, 0, 9, 29, 3, 7, 41
#define PPUTLIMPL_UDEC_860u  0x35Cu, 0, 0, 0, 0, 9, 29, 2, 2, 5, 43
#define PPUTLIMPL_UDEC_859u  0x35Bu, 0, 0, 0, 0, 9, 29,
#define PPUTLIMPL_UDEC_858u  0x35Au, 0, 0, 0, 0, 9, 29, 2, 3, 11, 13
#define PPUTLIMPL_UDEC_857u  0x359u, 0, 0, 0, 0, 9, 29,
#define PPUTLIMPL_UDEC_856u  0x358u, 0, 0, 0, 0, 9, 29, 2, 2, 2, 107
#define PPUTLIMPL_UDEC_855u  0x357u, 0, 0, 0, 0, 9, 29, 3, 3, 5, 19
#define PPUTLIMPL_UDEC_854u  0x356u, 0, 0, 0, 0, 9, 29, 2, 7, 61
#define PPUTLIMPL_UDEC_853u  0x355u, 0, 0, 0, 0, 9, 29,
#define PPUTLIMPL_UDEC_852u  0x354u, 0, 0, 0, 0, 9, 29, 2, 2, 3, 71
#define PPUTLIMPL_UDEC_851u  0x353u, 0, 0, 0, 0, 9, 29, 23, 37
#define PPUTLIMPL_UDEC_850u  0x352u, 0, 0, 0, 0, 9, 29, 2, 5, 5, 17
#define PPUTLIMPL_UDEC_849u  0x351u, 0, 0, 0, 0, 9, 29, 3, 283
#define PPUTLIMPL_UDEC_848u  0x350u, 0, 0, 0, 0, 9, 29, 2, 2, 2, 2, 53
#define PPUTLIMPL_UDEC_847u  0x34Fu, 0, 0, 0, 0, 9, 29, 7, 11, 11
#define PPUTLIMPL_UDEC_846u  0x34Eu, 0, 0, 0, 0, 9, 29, 2, 3, 3, 47
#define PPUTLIMPL_UDEC_845u  0x34Du, 0, 0, 0, 0, 9, 29, 5, 13, 13
#define PPUTLIMPL_UDEC_844u  0x34Cu, 0, 0, 0, 0, 9, 29, 2, 2, 211
#define PPUTLIMPL_UDEC_843u  0x34Bu, 0, 0, 0, 0, 9, 29, 3, 281
#define PPUTLIMPL_UDEC_842u  0x34Au, 0, 0, 0, 0, 9, 29, 2, 421
#define PPUTLIMPL_UDEC_841u  0x349u, 0, 0, 0, 0, 9, 29, 29, 29
#define PPUTLIMPL_UDEC_840u  0x348u, 0, 0, 0, 0, 9, 28, 2, 2, 2, 3, 5, 7
#define PPUTLIMPL_UDEC_839u  0x347u, 0, 0, 0, 0, 9, 28,
#define PPUTLIMPL_UDEC_838u  0x346u, 0, 0, 0, 0, 9, 28, 2, 419
#define PPUTLIMPL_UDEC_837u  0x345u, 0, 0, 0, 0, 9, 28, 3, 3, 3, 31
#define PPUTLIMPL_UDEC_836u  0x344u, 0, 0, 0, 0, 9, 28, 2, 2, 11, 19
#define PPUTLIMPL_UDEC_835u  0x343u, 0, 0, 0, 0, 9, 28, 5, 167
#define PPUTLIMPL_UDEC_834u  0x342u, 0, 0, 0, 0, 9, 28, 2, 3, 139
#define PPUTLIMPL_UDEC_833u  0x341u, 0, 0, 0, 0, 9, 28, 7, 7, 17
#define PPUTLIMPL_UDEC_832u  0x340u, 0, 0, 0, 0, 9, 28, 2, 2, 2, 2, 2, 2, 13
#define PPUTLIMPL_UDEC_831u  0x33Fu, 0, 0, 0, 0, 9, 28, 3, 277
#define PPUTLIMPL_UDEC_830u  0x33Eu, 0, 0, 0, 0, 9, 28, 2, 5, 83
#define PPUTLIMPL_UDEC_829u  0x33Du, 0, 0, 0, 0, 9, 28,
#define PPUTLIMPL_UDEC_828u  0x33Cu, 0, 0, 0, 0, 9, 28, 2, 2, 3, 3, 23
#define PPUTLIMPL_UDEC_827u  0x33Bu, 0, 0, 0, 0, 9, 28,
#define PPUTLIMPL_UDEC_826u  0x33Au, 0, 0, 0, 0, 9, 28, 2, 7, 59
#define PPUTLIMPL_UDEC_825u  0x339u, 0, 0, 0, 0, 9, 28, 3, 5, 5, 11
#define PPUTLIMPL_UDEC_824u  0x338u, 0, 0, 0, 0, 9, 28, 2, 2, 2, 103
#define PPUTLIMPL_UDEC_823u  0x337u, 0, 0, 0, 0, 9, 28,
#define PPUTLIMPL_UDEC_822u  0x336u, 0, 0, 0, 0, 9, 28, 2, 3, 137
#define PPUTLIMPL_UDEC_821u  0x335u, 0, 0, 0, 0, 9, 28,
#define PPUTLIMPL_UDEC_820u  0x334u, 0, 0, 0, 0, 9, 28, 2, 2, 5, 41
#define PPUTLIMPL_UDEC_819u  0x333u, 0, 0, 0, 0, 9, 28, 3, 3, 7, 13
#define PPUTLIMPL_UDEC_818u  0x332u, 0, 0, 0, 0, 9, 28, 2, 409
#define PPUTLIMPL_UDEC_817u  0x331u, 0, 0, 0, 0, 9, 28, 19, 43
#define PPUTLIMPL_UDEC_816u  0x330u, 0, 0, 0, 0, 9, 28, 2, 2, 2, 2, 3, 17
#define PPUTLIMPL_UDEC_815u  0x32Fu, 0, 0, 0, 0, 9, 28, 5, 163
#define PPUTLIMPL_UDEC_814u  0x32Eu, 0, 0, 0, 0, 9, 28, 2, 11, 37
#define PPUTLIMPL_UDEC_813u  0x32Du, 0, 0, 0, 0, 9, 28, 3, 271
#define PPUTLIMPL_UDEC_812u  0x32Cu, 0, 0, 0, 0, 9, 28, 2, 2, 7, 29
#define PPUTLIMPL_UDEC_811u  0x32Bu, 0, 0, 0, 0, 9, 28,
#define PPUTLIMPL_UDEC_810u  0x32Au, 0, 0, 0, 0, 9, 28, 2, 3, 3, 3, 3, 5
#define PPUTLIMPL_UDEC_809u  0x329u, 0, 0, 0, 0, 9, 28,
#define PPUTLIMPL_UDEC_808u  0x328u, 0, 0, 0, 0, 9, 28, 2, 2, 2, 101
#define PPUTLIMPL_UDEC_807u  0x327u, 0, 0, 0, 0, 9, 28, 3, 269
#define PPUTLIMPL_UDEC_806u  0x326u, 0, 0, 0, 0, 9, 28, 2, 13, 31
#define PPUTLIMPL_UDEC_805u  0x325u, 0, 0, 0, 0, 9, 28, 5, 7, 23
#define PPUTLIMPL_UDEC_804u  0x324u, 0, 0, 0, 0, 9, 28, 2, 2, 3, 67
#define PPUTLIMPL_UDEC_803u  0x323u, 0, 0, 0, 0, 9, 28, 11, 73
#define PPUTLIMPL_UDEC_802u  0x322u, 0, 0, 0, 0, 9, 28, 2, 401
#define PPUTLIMPL_UDEC_801u  0x321u, 0, 0, 0, 0, 9, 28, 3, 3, 89
#define PPUTLIMPL_UDEC_800u  0x320u, 0, 0, 0, 0, 9, 28, 2, 2, 2, 2, 2, 5, 5
#define PPUTLIMPL_UDEC_799u  0x31Fu, 0, 0, 0, 0, 9, 28, 17, 47
#define PPUTLIMPL_UDEC_798u  0x31Eu, 0, 0, 0, 0, 9, 28, 2, 3, 7, 19
#define PPUTLIMPL_UDEC_797u  0x31Du, 0, 0, 0, 0, 9, 28,
#define PPUTLIMPL_UDEC_796u  0x31Cu, 0, 0, 0, 0, 9, 28, 2, 2, 199
#define PPUTLIMPL_UDEC_795u  0x31Bu, 0, 0, 0, 0, 9, 28, 3, 5, 53
#define PPUTLIMPL_UDEC_794u  0x31Au, 0, 0, 0, 0, 9, 28, 2, 397
#define PPUTLIMPL_UDEC_793u  0x319u, 0, 0, 0, 0, 9, 28, 13, 61
#define PPUTLIMPL_UDEC_792u  0x318u, 0, 0, 0, 0, 9, 28, 2, 2, 2, 3, 3, 11
#define PPUTLIMPL_UDEC_791u  0x317u, 0, 0, 0, 0, 9, 28, 7, 113
#define PPUTLIMPL_UDEC_790u  0x316u, 0, 0, 0, 0, 9, 28, 2, 5, 79
#define PPUTLIMPL_UDEC_789u  0x315u, 0, 0, 0, 0, 9, 28, 3, 263
#define PPUTLIMPL_UDEC_788u  0x314u, 0, 0, 0, 0, 9, 28, 2, 2, 197
#define PPUTLIMPL_UDEC_787u  0x313u, 0, 0, 0, 0, 9, 28,
#define PPUTLIMPL_UDEC_786u  0x312u, 0, 0, 0, 0, 9, 28, 2, 3, 131
#define PPUTLIMPL_UDEC_785u  0x311u, 0, 0, 0, 0, 9, 28, 5, 157
#define PPUTLIMPL_UDEC_784u  0x310u, 0, 0, 0, 0, 9, 28, 2, 2, 2, 2, 7, 7
#define PPUTLIMPL_UDEC_783u  0x30Fu, 0, 0, 0, 0, 9, 27, 3, 3, 3, 29
#define PPUTLIMPL_UDEC_782u  0x30Eu, 0, 0, 0, 0, 9, 27, 2, 17, 23
#define PPUTLIMPL_UDEC_781u  0x30Du, 0, 0, 0, 0, 9, 27, 11, 71
#define PPUTLIMPL_UDEC_780u  0x30Cu, 0, 0, 0, 0, 9, 27, 2, 2, 3, 5, 13
#define PPUTLIMPL_UDEC_779u  0x30Bu, 0, 0, 0, 0, 9, 27, 19, 41
#define PPUTLIMPL_UDEC_778u  0x30Au, 0, 0, 0, 0, 9, 27, 2, 389
#define PPUTLIMPL_UDEC_777u  0x309u, 0, 0, 0, 0, 9, 27, 3, 7, 37
#define PPUTLIMPL_UDEC_776u  0x308u, 0, 0, 0, 0, 9, 27, 2, 2, 2, 97
#define PPUTLIMPL_UDEC_775u  0x307u, 0, 0, 0, 0, 9, 27, 5, 5, 31
#define PPUTLIMPL_UDEC_774u  0x306u, 0, 0, 0, 0, 9, 27, 2, 3, 3, 43
#define PPUTLIMPL_UDEC_773u  0x305u, 0, 0, 0, 0, 9, 27,
#define PPUTLIMPL_UDEC_772u  0x304u, 0, 0, 0, 0, 9, 27, 2, 2, 193
#define PPUTLIMPL_UDEC_771u  0x303u, 0, 0, 0, 0, 9, 27, 3, 257
#define PPUTLIMPL_UDEC_770u  0x302u, 0, 0, 0, 0, 9, 27, 2, 5, 7, 11
#define PPUTLIMPL_UDEC_769u  0x301u, 0, 0, 0, 0, 9, 27,
#define PPUTLIMPL_UDEC_768u  0x300u, 0, 0, 0, 0, 9, 27, 2, 2, 2, 2, 2, 2, 2, 2, 3
#define PPUTLIMPL_UDEC_767u  0x2FFu, 0, 0, 0, 0, 9, 27, 13, 59
#define PPUTLIMPL_UDEC_766u  0x2FEu, 0, 0, 0, 0, 9, 27, 2, 383
#define PPUTLIMPL_UDEC_765u  0x2FDu, 0, 0, 0, 0, 9, 27, 3, 3, 5, 17
#define PPUTLIMPL_UDEC_764u  0x2FCu, 0, 0, 0, 0, 9, 27, 2, 2, 191
#define PPUTLIMPL_UDEC_763u  0x2FBu, 0, 0, 0, 0, 9, 27, 7, 109
#define PPUTLIMPL_UDEC_762u  0x2FAu, 0, 0, 0, 0, 9, 27, 2, 3, 127
#define PPUTLIMPL_UDEC_761u  0x2F9u, 0, 0, 0, 0, 9, 27,
#define PPUTLIMPL_UDEC_760u  0x2F8u, 0, 0, 0, 0, 9, 27, 2, 2, 2, 5, 19
#define PPUTLIMPL_UDEC_759u  0x2F7u, 0, 0, 0, 0, 9, 27, 3, 11, 23
#define PPUTLIMPL_UDEC_758u  0x2F6u, 0, 0, 0, 0, 9, 27, 2, 379
#define PPUTLIMPL_UDEC_757u  0x2F5u, 0, 0, 0, 0, 9, 27,
#define PPUTLIMPL_UDEC_756u  0x2F4u, 0, 0, 0, 0, 9, 27, 2, 2, 3, 3, 3, 7
#define PPUTLIMPL_UDEC_755u  0x2F3u, 0, 0, 0, 0, 9, 27, 5, 151
#define PPUTLIMPL_UDEC_754u  0x2F2u, 0, 0, 0, 0, 9, 27, 2, 13, 29
#define PPUTLIMPL_UDEC_753u  0x2F1u, 0, 0, 0, 0, 9, 27, 3, 251
#define PPUTLIMPL_UDEC_752u  0x2F0u, 0, 0, 0, 0, 9, 27, 2, 2, 2, 2, 47
#define PPUTLIMPL_UDEC_751u  0x2EFu, 0, 0, 0, 0, 9, 27,
#define PPUTLIMPL_UDEC_750u  0x2EEu, 0, 0, 0, 0, 9, 27, 2, 3, 5, 5, 5
#define PPUTLIMPL_UDEC_749u  0x2EDu, 0, 0, 0, 0, 9, 27, 7, 107
#define PPUTLIMPL_UDEC_748u  0x2ECu, 0, 0, 0, 0, 9, 27, 2, 2, 11, 17
#define PPUTLIMPL_UDEC_747u  0x2EBu, 0, 0, 0, 0, 9, 27, 3, 3, 83
#define PPUTLIMPL_UDEC_746u  0x2EAu, 0, 0, 0, 0, 9, 27, 2, 373
#define PPUTLIMPL_UDEC_745u  0x2E9u, 0, 0, 0, 0, 9, 27, 5, 149
#define PPUTLIMPL_UDEC_744u  0x2E8u, 0, 0, 0, 0, 9, 27, 2, 2, 2, 3, 31
#define PPUTLIMPL_UDEC_743u  0x2E7u, 0, 0, 0, 0, 9, 27,
#define PPUTLIMPL_UDEC_742u  0x2E6u, 0, 0, 0, 0, 9, 27, 2, 7, 53
#define PPUTLIMPL_UDEC_741u  0x2E5u, 0, 0, 0, 0, 9, 27, 3, 13, 19
#define PPUTLIMPL_UDEC_740u  0x2E4u, 0, 0, 0, 0, 9, 27, 2, 2, 5, 37
#define PPUTLIMPL_UDEC_739u  0x2E3u, 0, 0, 0, 0, 9, 27,
#define PPUTLIMPL_UDEC_738u  0x2E2u, 0, 0, 0, 0, 9, 27, 2, 3, 3, 41
#define PPUTLIMPL_UDEC_737u  0x2E1u, 0, 0, 0, 0, 9, 27, 11, 67
#define PPUTLIMPL_UDEC_736u  0x2E0u, 0, 0, 0, 0, 9, 27, 2, 2, 2, 2, 2, 23
#define PPUTLIMPL_UDEC_735u  0x2DFu, 0, 0, 0, 0, 9, 27, 3, 5, 7, 7
#define PPUTLIMPL_UDEC_734u  0x2DEu, 0, 0, 0, 0, 9, 27, 2, 367
#define PPUTLIMPL_UDEC_733u  0x2DDu, 0, 0, 0, 0, 9, 27,
#define PPUTLIMPL_UDEC_732u  0x2DCu, 0, 0, 0, 0, 9, 27, 2, 2, 3, 61
#define PPUTLIMPL_UDEC_731u  0x2DBu, 0, 0, 0, 0, 9, 27, 17, 43
#define PPUTLIMPL_UDEC_730u  0x2DAu, 0, 0, 0, 0, 9, 27, 2, 5, 73
#define PPUTLIMPL_UDEC_729u  0x2D9u, 0, 0, 0, 0, 9, 27, 3, 3, 3, 3, 3, 3
#define PPUTLIMPL_UDEC_728u  0x2D8u, 0, 0, 0, 0, 9, 26, 2, 2, 2, 7, 13
#define PPUTLIMPL_UDEC_727u  0x2D7u, 0, 0, 0, 0, 9, 26,
#define PPUTLIMPL_UDEC_726u  0x2D6u, 0, 0, 0, 0, 9, 26, 2, 3, 11, 11
#define PPUTLIMPL_UDEC_725u  0x2D5u, 0, 0, 0, 0, 9, 26, 5, 5, 29
#define PPUTLIMPL_UDEC_724u  0x2D4u, 0, 0, 0, 0, 9, 26, 2, 2, 181
#define PPUTLIMPL_UDEC_723u  0x2D3u, 0, 0, 0, 0, 9, 26, 3, 241
#define PPUTLIMPL_UDEC_722u  0x2D2u, 0, 0, 0, 0, 9, 26, 2, 19, 19
#define PPUTLIMPL_UDEC_721u  0x2D1u, 0, 0, 0, 0, 9, 26, 7, 103
#define PPUTLIMPL_UDEC_720u  0x2D0u, 0, 0, 0, 0, 9, 26, 2, 2, 2, 2, 3, 3, 5
#define PPUTLIMPL_UDEC_719u  0x2CFu, 0, 0, 0, 0, 9, 26,
#define PPUTLIMPL_UDEC_718u  0x2CEu, 0, 0, 0, 0, 9, 26, 2, 359
#define PPUTLIMPL_UDEC_717u  0x2CDu, 0, 0, 0, 0, 9, 26, 3, 239
#define PPUTLIMPL_UDEC_716u  0x2CCu, 0, 0, 0, 0, 9, 26, 2, 2, 179
#define PPUTLIMPL_UDEC_715u  0x2CBu, 0, 0, 0, 0, 9, 26, 5, 11, 13
#define PPUTLIMPL_UDEC_714u  0x2CAu, 0, 0, 0, 0, 9, 26, 2, 3, 7, 17
#define PPUTLIMPL_UDEC_713u  0x2C9u, 0, 0, 0, 0, 9, 26, 23, 31
#define PPUTLIMPL_UDEC_712u  0x2C8u, 0, 0, 0, 0, 9, 26, 2, 2, 2, 89
#define PPUTLIMPL_UDEC_711u  0x2C7u, 0, 0, 0, 0, 9, 26, 3, 3, 79
#define PPUTLIMPL_UDEC_710u  0x2C6u, 0, 0, 0, 0, 9, 26, 2, 5, 71
#define PPUTLIMPL_UDEC_709u  0x2C5u, 0, 0, 0, 0, 9, 26,
#define PPUTLIMPL_UDEC_708u  0x2C4u, 0, 0, 0, 0, 9, 26, 2, 2, 3, 59
#define PPUTLIMPL_UDEC_707u  0x2C3u, 0, 0, 0, 0, 9, 26, 7, 101
#define PPUTLIMPL_UDEC_706u  0x2C2u, 0, 0, 0, 0, 9, 26, 2, 353
#define PPUTLIMPL_UDEC_705u  0x2C1u, 0, 0, 0, 0, 9, 26, 3, 5, 47
#define PPUTLIMPL_UDEC_704u  0x2C0u, 0, 0, 0, 0, 9, 26, 2, 2, 2, 2, 2, 2, 11
#define PPUTLIMPL_UDEC_703u  0x2BFu, 0, 0, 0, 0, 9, 26, 19, 37
#define PPUTLIMPL_UDEC_702u  0x2BEu, 0, 0, 0, 0, 9, 26, 2, 3, 3, 3, 13
#define PPUTLIMPL_UDEC_701u  0x2BDu, 0, 0, 0, 0, 9, 26,
#define PPUTLIMPL_UDEC_700u  0x2BCu, 0, 0, 0, 0, 9, 26, 2, 2, 5, 5, 7
#define PPUTLIMPL_UDEC_699u  0x2BBu, 0, 0, 0, 0, 9, 26, 3, 233
#define PPUTLIMPL_UDEC_698u  0x2BAu, 0, 0, 0, 0, 9, 26, 2, 349
#define PPUTLIMPL_UDEC_697u  0x2B9u, 0, 0, 0, 0, 9, 26, 17, 41
#define PPUTLIMPL_UDEC_696u  0x2B8u, 0, 0, 0, 0, 9, 26, 2, 2, 2, 3, 29
#define PPUTLIMPL_UDEC_695u  0x2B7u, 0, 0, 0, 0, 9, 26, 5, 139
#define PPUTLIMPL_UDEC_694u  0x2B6u, 0, 0, 0, 0, 9, 26, 2, 347
#define PPUTLIMPL_UDEC_693u  0x2B5u, 0, 0, 0, 0, 9, 26, 3, 3, 7, 11
#define PPUTLIMPL_UDEC_692u  0x2B4u, 0, 0, 0, 0, 9, 26, 2, 2, 173
#define PPUTLIMPL_UDEC_691u  0x2B3u, 0, 0, 0, 0, 9, 26,
#define PPUTLIMPL_UDEC_690u  0x2B2u, 0, 0, 0, 0, 9, 26, 2, 3, 5, 23
#define PPUTLIMPL_UDEC_689u  0x2B1u, 0, 0, 0, 0, 9, 26, 13, 53
#define PPUTLIMPL_UDEC_688u  0x2B0u, 0, 0, 0, 0, 9, 26, 2, 2, 2, 2, 43
#define PPUTLIMPL_UDEC_687u  0x2AFu, 0, 0, 0, 0, 9, 26, 3, 229
#define PPUTLIMPL_UDEC_686u  0x2AEu, 0, 0, 0, 0, 9, 26, 2, 7, 7, 7
#define PPUTLIMPL_UDEC_685u  0x2ADu, 0, 0, 0, 0, 9, 26, 5, 137
#define PPUTLIMPL_UDEC_684u  0x2ACu, 0, 0, 0, 0, 9, 26, 2, 2, 3, 3, 19
#define PPUTLIMPL_UDEC_683u  0x2ABu, 0, 0, 0, 0, 9, 26,
#define PPUTLIMPL_UDEC_682u  0x2AAu, 0, 0, 0, 0, 9, 26, 2, 11, 31
#define PPUTLIMPL_UDEC_681u  0x2A9u, 0, 0, 0, 0, 9, 26, 3, 227
#define PPUTLIMPL_UDEC_680u  0x2A8u, 0, 0, 0, 0, 9, 26, 2, 2, 2, 5, 17
#define PPUTLIMPL_UDEC_679u  0x2A7u, 0, 0, 0, 0, 9, 26, 7, 97
#define PPUTLIMPL_UDEC_678u  0x2A6u, 0, 0, 0, 0, 9, 26, 2, 3, 113
#define PPUTLIMPL_UDEC_677u  0x2A5u, 0, 0, 0, 0, 9, 26,
#define PPUTLIMPL_UDEC_676u  0x2A4u, 0, 0, 0, 0, 9, 26, 2, 2, 13, 13
#define PPUTLIMPL_UDEC_675u  0x2A3u, 0, 0, 0, 0, 9, 25, 3, 3, 3, 5, 5
#define PPUTLIMPL_UDEC_674u  0x2A2u, 0, 0, 0, 0, 9, 25, 2, 337
#define PPUTLIMPL_UDEC_673u  0x2A1u, 0, 0, 0, 0, 9, 25,
#define PPUTLIMPL_UDEC_672u  0x2A0u, 0, 0, 0, 0, 9, 25, 2, 2, 2, 2, 2, 3, 7
#define PPUTLIMPL_UDEC_671u  0x29Fu, 0, 0, 0, 0, 9, 25, 11, 61
#define PPUTLIMPL_UDEC_670u  0x29Eu, 0, 0, 0, 0, 9, 25, 2, 5, 67
#define PPUTLIMPL_UDEC_669u  0x29Du, 0, 0, 0, 0, 9, 25, 3, 223
#define PPUTLIMPL_UDEC_668u  0x29Cu, 0, 0, 0, 0, 9, 25, 2, 2, 167
#define PPUTLIMPL_UDEC_667u  0x29Bu, 0, 0, 0, 0, 9, 25, 23, 29
#define PPUTLIMPL_UDEC_666u  0x29Au, 0, 0, 0, 0, 9, 25, 2, 3, 3, 37
#define PPUTLIMPL_UDEC_665u  0x299u, 0, 0, 0, 0, 9, 25, 5, 7, 19
#define PPUTLIMPL_UDEC_664u  0x298u, 0, 0, 0, 0, 9, 25, 2, 2, 2, 83
#define PPUTLIMPL_UDEC_663u  0x297u, 0, 0, 0, 0, 9, 25, 3, 13, 17
#define PPUTLIMPL_UDEC_662u  0x296u, 0, 0, 0, 0, 9, 25, 2, 331
#define PPUTLIMPL_UDEC_661u  0x295u, 0, 0, 0, 0, 9, 25,
#define PPUTLIMPL_UDEC_660u  0x294u, 0, 0, 0, 0, 9, 25, 2, 2, 3, 5, 11
#define PPUTLIMPL_UDEC_659u  0x293u, 0, 0, 0, 0, 9, 25,
#define PPUTLIMPL_UDEC_658u  0x292u, 0, 0, 0, 0, 9, 25, 2, 7, 47
#define PPUTLIMPL_UDEC_657u  0x291u, 0, 0, 0, 0, 9, 25, 3, 3, 73
#define PPUTLIMPL_UDEC_656u  0x290u, 0, 0, 0, 0, 9, 25, 2, 2, 2, 2, 41
#define PPUTLIMPL_UDEC_655u  0x28Fu, 0, 0, 0, 0, 9, 25, 5, 131
#define PPUTLIMPL_UDEC_654u  0x28Eu, 0, 0, 0, 0, 9, 25, 2, 3, 109
#define PPUTLIMPL_UDEC_653u  0x28Du, 0, 0, 0, 0, 9, 25,
#define PPUTLIMPL_UDEC_652u  0x28Cu, 0, 0, 0, 0, 9, 25, 2, 2, 163
#define PPUTLIMPL_UDEC_651u  0x28Bu, 0, 0, 0, 0, 9, 25, 3, 7, 31
#define PPUTLIMPL_UDEC_650u  0x28Au, 0, 0, 0, 0, 9, 25, 2, 5, 5, 13
#define PPUTLIMPL_UDEC_649u  0x289u, 0, 0, 0, 0, 9, 25, 11, 59
#define PPUTLIMPL_UDEC_648u  0x288u, 0, 0, 0, 0, 9, 25, 2, 2, 2, 3, 3, 3, 3
#define PPUTLIMPL_UDEC_647u  0x287u, 0, 0, 0, 0, 9, 25,
#define PPUTLIMPL_UDEC_646u  0x286u, 0, 0, 0, 0, 9, 25, 2, 17, 19
#define PPUTLIMPL_UDEC_645u  0x285u, 0, 0, 0, 0, 9, 25, 3, 5, 43
#define PPUTLIMPL_UDEC_644u  0x284u, 0, 0, 0, 0, 9, 25, 2, 2, 7, 23
#define PPUTLIMPL_UDEC_643u  0x283u, 0, 0, 0, 0, 9, 25,
#define PPUTLIMPL_UDEC_642u  0x282u, 0, 0, 0, 0, 9, 25, 2, 3, 107
#define PPUTLIMPL_UDEC_641u  0x281u, 0, 0, 0, 0, 9, 25,
#define PPUTLIMPL_UDEC_640u  0x280u, 0, 0, 0, 0, 9, 25, 2, 2, 2, 2, 2, 2, 2, 5
#define PPUTLIMPL_UDEC_639u  0x27Fu, 0, 0, 0, 0, 9, 25, 3, 3, 71
#define PPUTLIMPL_UDEC_638u  0x27Eu, 0, 0, 0, 0, 9, 25, 2, 11, 29
#define PPUTLIMPL_UDEC_637u  0x27Du, 0, 0, 0, 0, 9, 25, 7, 7, 13
#define PPUTLIMPL_UDEC_636u  0x27Cu, 0, 0, 0, 0, 9, 25, 2, 2, 3, 53
#define PPUTLIMPL_UDEC_635u  0x27Bu, 0, 0, 0, 0, 9, 25, 5, 127
#define PPUTLIMPL_UDEC_634u  0x27Au, 0, 0, 0, 0, 9, 25, 2, 317
#define PPUTLIMPL_UDEC_633u  0x279u, 0, 0, 0, 0, 9, 25, 3, 211
#define PPUTLIMPL_UDEC_632u  0x278u, 0, 0, 0, 0, 9, 25, 2, 2, 2, 79
#define PPUTLIMPL_UDEC_631u  0x277u, 0, 0, 0, 0, 9, 25,
#define PPUTLIMPL_UDEC_630u  0x276u, 0, 0, 0, 0, 9, 25, 2, 3, 3, 5, 7
#define PPUTLIMPL_UDEC_629u  0x275u, 0, 0, 0, 0, 9, 25, 17, 37
#define PPUTLIMPL_UDEC_628u  0x274u, 0, 0, 0, 0, 9, 25, 2, 2, 157
#define PPUTLIMPL_UDEC_627u  0x273u, 0, 0, 0, 0, 9, 25, 3, 11, 19
#define PPUTLIMPL_UDEC_626u  0x272u, 0, 0, 0, 0, 9, 25, 2, 313
#define PPUTLIMPL_UDEC_625u  0x271u, 0, 0, 0, 0, 9, 25, 5, 5, 5, 5
#define PPUTLIMPL_UDEC_624u  0x270u, 0, 0, 0, 0, 9, 24, 2, 2, 2, 2, 3, 13
#define PPUTLIMPL_UDEC_623u  0x26Fu, 0, 0, 0, 0, 9, 24, 7, 89
#define PPUTLIMPL_UDEC_622u  0x26Eu, 0, 0, 0, 0, 9, 24, 2, 311
#define PPUTLIMPL_UDEC_621u  0x26Du, 0, 0, 0, 0, 9, 24, 3, 3, 3, 23
#define PPUTLIMPL_UDEC_620u  0x26Cu, 0, 0, 0, 0, 9, 24, 2, 2, 5, 31
#define PPUTLIMPL_UDEC_619u  0x26Bu, 0, 0, 0, 0, 9, 24,
#define PPUTLIMPL_UDEC_618u  0x26Au, 0, 0, 0, 0, 9, 24, 2, 3, 103
#define PPUTLIMPL_UDEC_617u  0x269u, 0, 0, 0, 0, 9, 24,
#define PPUTLIMPL_UDEC_616u  0x268u, 0, 0, 0, 0, 9, 24, 2, 2, 2, 7, 11
#define PPUTLIMPL_UDEC_615u  0x267u, 0, 0, 0, 0, 9, 24, 3, 5, 41
#define PPUTLIMPL_UDEC_614u  0x266u, 0, 0, 0, 0, 9, 24, 2, 307
#define PPUTLIMPL_UDEC_613u  0x265u, 0, 0, 0, 0, 9, 24,
#define PPUTLIMPL_UDEC_612u  0x264u, 0, 0, 0, 0, 9, 24, 2, 2, 3, 3, 17
#define PPUTLIMPL_UDEC_611u  0x263u, 0, 0, 0, 0, 9, 24, 13, 47
#define PPUTLIMPL_UDEC_610u  0x262u, 0, 0, 0, 0, 9, 24, 2, 5, 61
#define PPUTLIMPL_UDEC_609u  0x261u, 0, 0, 0, 0, 9, 24, 3, 7, 29
#define PPUTLIMPL_UDEC_608u  0x260u, 0, 0, 0, 0, 9, 24, 2, 2, 2, 2, 2, 19
#define PPUTLIMPL_UDEC_607u  0x25Fu, 0, 0, 0, 0, 9, 24,
#define PPUTLIMPL_UDEC_606u  0x25Eu, 0, 0, 0, 0, 9, 24, 2, 3, 101
#define PPUTLIMPL_UDEC_605u  0x25Du, 0, 0, 0, 0, 9, 24, 5, 11, 11
#define PPUTLIMPL_UDEC_604u  0x25Cu, 0, 0, 0, 0, 9, 24, 2, 2, 151
#define PPUTLIMPL_UDEC_603u  0x25Bu, 0, 0, 0, 0, 9, 24, 3, 3, 67
#define PPUTLIMPL_UDEC_602u  0x25Au, 0, 0, 0, 0, 9, 24, 2, 7, 43
#define PPUTLIMPL_UDEC_601u  0x259u, 0, 0, 0, 0, 9, 24,
#define PPUTLIMPL_UDEC_600u  0x258u, 0, 0, 0, 0, 9, 24, 2, 2, 2, 3, 5, 5
#define PPUTLIMPL_UDEC_599u  0x257u, 0, 0, 0, 0, 9, 24,
#define PPUTLIMPL_UDEC_598u  0x256u, 0, 0, 0, 0, 9, 24, 2, 13, 23
#define PPUTLIMPL_UDEC_597u  0x255u, 0, 0, 0, 0, 9, 24, 3, 199
#define PPUTLIMPL_UDEC_596u  0x254u, 0, 0, 0, 0, 9, 24, 2, 2, 149
#define PPUTLIMPL_UDEC_595u  0x253u, 0, 0, 0, 0, 9, 24, 5, 7, 17
#define PPUTLIMPL_UDEC_594u  0x252u, 0, 0, 0, 0, 9, 24, 2, 3, 3, 3, 11
#define PPUTLIMPL_UDEC_593u  0x251u, 0, 0, 0, 0, 9, 24,
#define PPUTLIMPL_UDEC_592u  0x250u, 0, 0, 0, 0, 9, 24, 2, 2, 2, 2, 37
#define PPUTLIMPL_UDEC_591u  0x24Fu, 0, 0, 0, 0, 9, 24, 3, 197
#define PPUTLIMPL_UDEC_590u  0x24Eu, 0, 0, 0, 0, 9, 24, 2, 5, 59
#define PPUTLIMPL_UDEC_589u  0x24Du, 0, 0, 0, 0, 9, 24, 19, 31
#define PPUTLIMPL_UDEC_588u  0x24Cu, 0, 0, 0, 0, 9, 24, 2, 2, 3, 7, 7
#define PPUTLIMPL_UDEC_587u  0x24Bu, 0, 0, 0, 0, 9, 24,
#define PPUTLIMPL_UDEC_586u  0x24Au, 0, 0, 0, 0, 9, 24, 2, 293
#define PPUTLIMPL_UDEC_585u  0x249u, 0, 0, 0, 0, 9, 24, 3, 3, 5, 13
#define PPUTLIMPL_UDEC_584u  0x248u, 0, 0, 0, 0, 9, 24, 2, 2, 2, 73
#define PPUTLIMPL_UDEC_583u  0x247u, 0, 0, 0, 0, 9, 24, 11, 53
#define PPUTLIMPL_UDEC_582u  0x246u, 0, 0, 0, 0, 9, 24, 2, 3, 97
#define PPUTLIMPL_UDEC_581u  0x245u, 0, 0, 0, 0, 9, 24, 7, 83
#define PPUTLIMPL_UDEC_580u  0x244u, 0, 0, 0, 0, 9, 24, 2, 2, 5, 29
#define PPUTLIMPL_UDEC_579u  0x243u, 0, 0, 0, 0, 9, 24, 3, 193
#define PPUTLIMPL_UDEC_578u  0x242u, 0, 0, 0, 0, 9, 24, 2, 17, 17
#define PPUTLIMPL_UDEC_577u  0x241u, 0, 0, 0, 0, 9, 24,
#define PPUTLIMPL_UDEC_576u  0x240u, 0, 0, 0, 0, 9, 24, 2, 2, 2, 2, 2, 2, 3, 3
#define PPUTLIMPL_UDEC_575u  0x23Fu, 0, 0, 0, 0, 9, 23, 5, 5, 23
#define PPUTLIMPL_UDEC_574u  0x23Eu, 0, 0, 0, 0, 9, 23, 2, 7, 41
#define PPUTLIMPL_UDEC_573u  0x23Du, 0, 0, 0, 0, 9, 23, 3, 191
#define PPUTLIMPL_UDEC_572u  0x23Cu, 0, 0, 0, 0, 9, 23, 2, 2, 11, 13
#define PPUTLIMPL_UDEC_571u  0x23Bu, 0, 0, 0, 0, 9, 23,
#define PPUTLIMPL_UDEC_570u  0x23Au, 0, 0, 0, 0, 9, 23, 2, 3, 5, 19
#define PPUTLIMPL_UDEC_569u  0x239u, 0, 0, 0, 0, 9, 23,
#define PPUTLIMPL_UDEC_568u  0x238u, 0, 0, 0, 0, 9, 23, 2, 2, 2, 71
#define PPUTLIMPL_UDEC_567u  0x237u, 0, 0, 0, 0, 9, 23, 3, 3, 3, 3, 7
#define PPUTLIMPL_UDEC_566u  0x236u, 0, 0, 0, 0, 9, 23, 2, 283
#define PPUTLIMPL_UDEC_565u  0x235u, 0, 0, 0, 0, 9, 23, 5, 113
#define PPUTLIMPL_UDEC_564u  0x234u, 0, 0, 0, 0, 9, 23, 2, 2, 3, 47
#define PPUTLIMPL_UDEC_563u  0x233u, 0, 0, 0, 0, 9, 23,
#define PPUTLIMPL_UDEC_562u  0x232u, 0, 0, 0, 0, 9, 23, 2, 281
#define PPUTLIMPL_UDEC_561u  0x231u, 0, 0, 0, 0, 9, 23, 3, 11, 17
#define PPUTLIMPL_UDEC_560u  0x230u, 0, 0, 0, 0, 9, 23, 2, 2, 2, 2, 5, 7
#define PPUTLIMPL_UDEC_559u  0x22Fu, 0, 0, 0, 0, 9, 23, 13, 43
#define PPUTLIMPL_UDEC_558u  0x22Eu, 0, 0, 0, 0, 9, 23, 2, 3, 3, 31
#define PPUTLIMPL_UDEC_557u  0x22Du, 0, 0, 0, 0, 9, 23,
#define PPUTLIMPL_UDEC_556u  0x22Cu, 0, 0, 0, 0, 9, 23, 2, 2, 139
#define PPUTLIMPL_UDEC_555u  0x22Bu, 0, 0, 0, 0, 9, 23, 3, 5, 37
#define PPUTLIMPL_UDEC_554u  0x22Au, 0, 0, 0, 0, 9, 23, 2, 277
#define PPUTLIMPL_UDEC_553u  0x229u, 0, 0, 0, 0, 9, 23, 7, 79
#define PPUTLIMPL_UDEC_552u  0x228u, 0, 0, 0, 0, 9, 23, 2, 2, 2, 3, 23
#define PPUTLIMPL_UDEC_551u  0x227u, 0, 0, 0, 0, 9, 23, 19, 29
#define PPUTLIMPL_UDEC_550u  0x226u, 0, 0, 0, 0, 9, 23, 2, 5, 5, 11
#define PPUTLIMPL_UDEC_549u  0x225u, 0, 0, 0, 0, 9, 23, 3, 3, 61
#define PPUTLIMPL_UDEC_548u  0x224u, 0, 0, 0, 0, 9, 23, 2, 2, 137
#define PPUTLIMPL_UDEC_547u  0x223u, 0, 0, 0, 0, 9, 23,
#define PPUTLIMPL_UDEC_546u  0x222u, 0, 0, 0, 0, 9, 23, 2, 3, 7, 13
#define PPUTLIMPL_UDEC_545u  0x221u, 0, 0, 0, 0, 9, 23, 5, 109
#define PPUTLIMPL_UDEC_544u  0x220u, 0, 0, 0, 0, 9, 23, 2, 2, 2, 2, 2, 17
#define PPUTLIMPL_UDEC_543u  0x21Fu, 0, 0, 0, 0, 9, 23, 3, 181
#define PPUTLIMPL_UDEC_542u  0x21Eu, 0, 0, 0, 0, 9, 23, 2, 271
#define PPUTLIMPL_UDEC_541u  0x21Du, 0, 0, 0, 0, 9, 23,
#define PPUTLIMPL_UDEC_540u  0x21Cu, 0, 0, 0, 0, 9, 23, 2, 2, 3, 3, 3, 5
#define PPUTLIMPL_UDEC_539u  0x21Bu, 0, 0, 0, 0, 9, 23, 7, 7, 11
#define PPUTLIMPL_UDEC_538u  0x21Au, 0, 0, 0, 0, 9, 23, 2, 269
#define PPUTLIMPL_UDEC_537u  0x219u, 0, 0, 0, 0, 9, 23, 3, 179
#define PPUTLIMPL_UDEC_536u  0x218u, 0, 0, 0, 0, 9, 23, 2, 2, 2, 67
#define PPUTLIMPL_UDEC_535u  0x217u, 0, 0, 0, 0, 9, 23, 5, 107
#define PPUTLIMPL_UDEC_534u  0x216u, 0, 0, 0, 0, 9, 23, 2, 3, 89
#define PPUTLIMPL_UDEC_533u  0x215u, 0, 0, 0, 0, 9, 23, 13, 41
#define PPUTLIMPL_UDEC_532u  0x214u, 0, 0, 0, 0, 9, 23, 2, 2, 7, 19
#define PPUTLIMPL_UDEC_531u  0x213u, 0, 0, 0, 0, 9, 23, 3, 3, 59
#define PPUTLIMPL_UDEC_530u  0x212u, 0, 0, 0, 0, 9, 23, 2, 5, 53
#define PPUTLIMPL_UDEC_529u  0x211u, 0, 0, 0, 0, 9, 23, 23, 23
#define PPUTLIMPL_UDEC_528u  0x210u, 0, 0, 0, 0, 9, 22, 2, 2, 2, 2, 3, 11
#define PPUTLIMPL_UDEC_527u  0x20Fu, 0, 0, 0, 0, 9, 22, 17, 31
#define PPUTLIMPL_UDEC_526u  0x20Eu, 0, 0, 0, 0, 9, 22, 2, 263
#define PPUTLIMPL_UDEC_525u  0x20Du, 0, 0, 0, 0, 9, 22, 3, 5, 5, 7
#define PPUTLIMPL_UDEC_524u  0x20Cu, 0, 0, 0, 0, 9, 22, 2, 2, 131
#define PPUTLIMPL_UDEC_523u  0x20Bu, 0, 0, 0, 0, 9, 22,
#define PPUTLIMPL_UDEC_522u  0x20Au, 0, 0, 0, 0, 9, 22, 2, 3, 3, 29
#define PPUTLIMPL_UDEC_521u  0x209u, 0, 0, 0, 0, 9, 22,
#define PPUTLIMPL_UDEC_520u  0x208u, 0, 0, 0, 0, 9, 22, 2, 2, 2, 5, 13
#define PPUTLIMPL_UDEC_519u  0x207u, 0, 0, 0, 0, 9, 22, 3, 173
#define PPUTLIMPL_UDEC_518u  0x206u, 0, 0, 0, 0, 9, 22, 2, 7, 37
#define PPUTLIMPL_UDEC_517u  0x205u, 0, 0, 0, 0, 9, 22, 11, 47
#define PPUTLIMPL_UDEC_516u  0x204u, 0, 0, 0, 0, 9, 22, 2, 2, 3, 43
#define PPUTLIMPL_UDEC_515u  0x203u, 0, 0, 0, 0, 9, 22, 5, 103
#define PPUTLIMPL_UDEC_514u  0x202u, 0, 0, 0, 0, 9, 22, 2, 257
#define PPUTLIMPL_UDEC_513u  0x201u, 0, 0, 0, 0, 9, 22, 3, 3, 3, 19
#define PPUTLIMPL_UDEC_512u  0x200u, 0, 0, 0, 0, 9, 22, 2, 2, 2, 2, 2, 2, 2, 2, 2
#define PPUTLIMPL_UDEC_511u  0x1FFu, 0, 0, 0, 0, 8, 22, 7, 73
#define PPUTLIMPL_UDEC_510u  0x1FEu, 0, 0, 0, 0, 8, 22, 2, 3, 5, 17
#define PPUTLIMPL_UDEC_509u  0x1FDu, 0, 0, 0, 0, 8, 22,
#define PPUTLIMPL_UDEC_508u  0x1FCu, 0, 0, 0, 0, 8, 22, 2, 2, 127
#define PPUTLIMPL_UDEC_507u  0x1FBu, 0, 0, 0, 0, 8, 22, 3, 13, 13
#define PPUTLIMPL_UDEC_506u  0x1FAu, 0, 0, 0, 0, 8, 22, 2, 11, 23
#define PPUTLIMPL_UDEC_505u  0x1F9u, 0, 0, 0, 0, 8, 22, 5, 101
#define PPUTLIMPL_UDEC_504u  0x1F8u, 0, 0, 0, 0, 8, 22, 2, 2, 2, 3, 3, 7
#define PPUTLIMPL_UDEC_503u  0x1F7u, 0, 0, 0, 0, 8, 22,
#define PPUTLIMPL_UDEC_502u  0x1F6u, 0, 0, 0, 0, 8, 22, 2, 251
#define PPUTLIMPL_UDEC_501u  0x1F5u, 0, 0, 0, 0, 8, 22, 3, 167
#define PPUTLIMPL_UDEC_500u  0x1F4u, 0, 0, 0, 0, 8, 22, 2, 2, 5, 5, 5
#define PPUTLIMPL_UDEC_499u  0x1F3u, 0, 0, 0, 0, 8, 22,
#define PPUTLIMPL_UDEC_498u  0x1F2u, 0, 0, 0, 0, 8, 22, 2, 3, 83
#define PPUTLIMPL_UDEC_497u  0x1F1u, 0, 0, 0, 0, 8, 22, 7, 71
#define PPUTLIMPL_UDEC_496u  0x1F0u, 0, 0, 0, 0, 8, 22, 2, 2, 2, 2, 31
#define PPUTLIMPL_UDEC_495u  0x1EFu, 0, 0, 0, 0, 8, 22, 3, 3, 5, 11
#define PPUTLIMPL_UDEC_494u  0x1EEu, 0, 0, 0, 0, 8, 22, 2, 13, 19
#define PPUTLIMPL_UDEC_493u  0x1EDu, 0, 0, 0, 0, 8, 22, 17, 29
#define PPUTLIMPL_UDEC_492u  0x1ECu, 0, 0, 0, 0, 8, 22, 2, 2, 3, 41
#define PPUTLIMPL_UDEC_491u  0x1EBu, 0, 0, 0, 0, 8, 22,
#define PPUTLIMPL_UDEC_490u  0x1EAu, 0, 0, 0, 0, 8, 22, 2, 5, 7, 7
#define PPUTLIMPL_UDEC_489u  0x1E9u, 0, 0, 0, 0, 8, 22, 3, 163
#define PPUTLIMPL_UDEC_488u  0x1E8u, 0, 0, 0, 0, 8, 22, 2, 2, 2, 61
#define PPUTLIMPL_UDEC_487u  0x1E7u, 0, 0, 0, 0, 8, 22,
#define PPUTLIMPL_UDEC_486u  0x1E6u, 0, 0, 0, 0, 8, 22, 2, 3, 3, 3, 3, 3
#define PPUTLIMPL_UDEC_485u  0x1E5u, 0, 0, 0, 0, 8, 22, 5, 97
#define PPUTLIMPL_UDEC_484u  0x1E4u, 0, 0, 0, 0, 8, 22, 2, 2, 11, 11
#define PPUTLIMPL_UDEC_483u  0x1E3u, 0, 0, 0, 0, 8, 21, 3, 7, 23
#define PPUTLIMPL_UDEC_482u  0x1E2u, 0, 0, 0, 0, 8, 21, 2, 241
#define PPUTLIMPL_UDEC_481u  0x1E1u, 0, 0, 0, 0, 8, 21, 13, 37
#define PPUTLIMPL_UDEC_480u  0x1E0u, 0, 0, 0, 0, 8, 21, 2, 2, 2, 2, 2, 3, 5
#define PPUTLIMPL_UDEC_479u  0x1DFu, 0, 0, 0, 0, 8, 21,
#define PPUTLIMPL_UDEC_478u  0x1DEu, 0, 0, 0, 0, 8, 21, 2, 239
#define PPUTLIMPL_UDEC_477u  0x1DDu, 0, 0, 0, 0, 8, 21, 3, 3, 53
#define PPUTLIMPL_UDEC_476u  0x1DCu, 0, 0, 0, 0, 8, 21, 2, 2, 7, 17
#define PPUTLIMPL_UDEC_475u  0x1DBu, 0, 0, 0, 0, 8, 21, 5, 5, 19
#define PPUTLIMPL_UDEC_474u  0x1DAu, 0, 0, 0, 0, 8, 21, 2, 3, 79
#define PPUTLIMPL_UDEC_473u  0x1D9u, 0, 0, 0, 0, 8, 21, 11, 43
#define PPUTLIMPL_UDEC_472u  0x1D8u, 0, 0, 0, 0, 8, 21, 2, 2, 2, 59
#define PPUTLIMPL_UDEC_471u  0x1D7u, 0, 0, 0, 0, 8, 21, 3, 157
#define PPUTLIMPL_UDEC_470u  0x1D6u, 0, 0, 0, 0, 8, 21, 2, 5, 47
#define PPUTLIMPL_UDEC_469u  0x1D5u, 0, 0, 0, 0, 8, 21, 7, 67
#define PPUTLIMPL_UDEC_468u  0x1D4u, 0, 0, 0, 0, 8, 21, 2, 2, 3, 3, 13
#define PPUTLIMPL_UDEC_467u  0x1D3u, 0, 0, 0, 0, 8, 21,
#define PPUTLIMPL_UDEC_466u  0x1D2u, 0, 0, 0, 0, 8, 21, 2, 233
#define PPUTLIMPL_UDEC_465u  0x1D1u, 0, 0, 0, 0, 8, 21, 3, 5, 31
#define PPUTLIMPL_UDEC_464u  0x1D0u, 0, 0, 0, 0, 8, 21, 2, 2, 2, 2, 29
#define PPUTLIMPL_UDEC_463u  0x1CFu, 0, 0, 0, 0, 8, 21,
#define PPUTLIMPL_UDEC_462u  0x1CEu, 0, 0, 0, 0, 8, 21, 2, 3, 7, 11
#define PPUTLIMPL_UDEC_461u  0x1CDu, 0, 0, 0, 0, 8, 21,
#define PPUTLIMPL_UDEC_460u  0x1CCu, 0, 0, 0, 0, 8, 21, 2, 2, 5, 23
#define PPUTLIMPL_UDEC_459u  0x1CBu, 0, 0, 0, 0, 8, 21, 3, 3, 3, 17
#define PPUTLIMPL_UDEC_458u  0x1CAu, 0, 0, 0, 0, 8, 21, 2, 229
#define PPUTLIMPL_UDEC_457u  0x1C9u, 0, 0, 0, 0, 8, 21,
#define PPUTLIMPL_UDEC_456u  0x1C8u, 0, 0, 0, 0, 8, 21, 2, 2, 2, 3, 19
#define PPUTLIMPL_UDEC_455u  0x1C7u, 0, 0, 0, 0, 8, 21, 5, 7, 13
#define PPUTLIMPL_UDEC_454u  0x1C6u, 0, 0, 0, 0, 8, 21, 2, 227
#define PPUTLIMPL_UDEC_453u  0x1C5u, 0, 0, 0, 0, 8, 21, 3, 151
#define PPUTLIMPL_UDEC_452u  0x1C4u, 0, 0, 0, 0, 8, 21, 2, 2, 113
#define PPUTLIMPL_UDEC_451u  0x1C3u, 0, 0, 0, 0, 8, 21, 11, 41
#define PPUTLIMPL_UDEC_450u  0x1C2u, 0, 0, 0, 0, 8, 21, 2, 3, 3, 5, 5
#define PPUTLIMPL_UDEC_449u  0x1C1u, 0, 0, 0, 0, 8, 21,
#define PPUTLIMPL_UDEC_448u  0x1C0u, 0, 0, 0, 0, 8, 21, 2, 2, 2, 2, 2, 2, 7
#define PPUTLIMPL_UDEC_447u  0x1BFu, 0, 0, 0, 0, 8, 21, 3, 149
#define PPUTLIMPL_UDEC_446u  0x1BEu, 0, 0, 0, 0, 8, 21, 2, 223
#define PPUTLIMPL_UDEC_445u  0x1BDu, 0, 0, 0, 0, 8, 21, 5, 89
#define PPUTLIMPL_UDEC_444u  0x1BCu, 0, 0, 0, 0, 8, 21, 2, 2, 3, 37
#define PPUTLIMPL_UDEC_443u  0x1BBu, 0, 0, 0, 0, 8, 21,
#define PPUTLIMPL_UDEC_442u  0x1BAu, 0, 0, 0, 0, 8, 21, 2, 13, 17
#define PPUTLIMPL_UDEC_441u  0x1B9u, 0, 0, 0, 0, 8, 21, 3, 3, 7, 7
#define PPUTLIMPL_UDEC_440u  0x1B8u, 0, 0, 0, 0, 8, 20, 2, 2, 2, 5, 11
#define PPUTLIMPL_UDEC_439u  0x1B7u, 0, 0, 0, 0, 8, 20,
#define PPUTLIMPL_UDEC_438u  0x1B6u, 0, 0, 0, 0, 8, 20, 2, 3, 73
#define PPUTLIMPL_UDEC_437u  0x1B5u, 0, 0, 0, 0, 8, 20, 19, 23
#define PPUTLIMPL_UDEC_436u  0x1B4u, 0, 0, 0, 0, 8, 20, 2, 2, 109
#define PPUTLIMPL_UDEC_435u  0x1B3u, 0, 0, 0, 0, 8, 20, 3, 5, 29
#define PPUTLIMPL_UDEC_434u  0x1B2u, 0, 0, 0, 0, 8, 20, 2, 7, 31
#define PPUTLIMPL_UDEC_433u  0x1B1u, 0, 0, 0, 0, 8, 20,
#define PPUTLIMPL_UDEC_432u  0x1B0u, 0, 0, 0, 0, 8, 20, 2, 2, 2, 2, 3, 3, 3
#define PPUTLIMPL_UDEC_431u  0x1AFu, 0, 0, 0, 0, 8, 20,
#define PPUTLIMPL_UDEC_430u  0x1AEu, 0, 0, 0, 0, 8, 20, 2, 5, 43
#define PPUTLIMPL_UDEC_429u  0x1ADu, 0, 0, 0, 0, 8, 20, 3, 11, 13
#define PPUTLIMPL_UDEC_428u  0x1ACu, 0, 0, 0, 0, 8, 20, 2, 2, 107
#define PPUTLIMPL_UDEC_427u  0x1ABu, 0, 0, 0, 0, 8, 20, 7, 61
#define PPUTLIMPL_UDEC_426u  0x1AAu, 0, 0, 0, 0, 8, 20, 2, 3, 71
#define PPUTLIMPL_UDEC_425u  0x1A9u, 0, 0, 0, 0, 8, 20, 5, 5, 17
#define PPUTLIMPL_UDEC_424u  0x1A8u, 0, 0, 0, 0, 8, 20, 2, 2, 2, 53
#define PPUTLIMPL_UDEC_423u  0x1A7u, 0, 0, 0, 0, 8, 20, 3, 3, 47
#define PPUTLIMPL_UDEC_422u  0x1A6u, 0, 0, 0, 0, 8, 20, 2, 211
#define PPUTLIMPL_UDEC_421u  0x1A5u, 0, 0, 0, 0, 8, 20,
#define PPUTLIMPL_UDEC_420u  0x1A4u, 0, 0, 0, 0, 8, 20, 2, 2, 3, 5, 7
#define PPUTLIMPL_UDEC_419u  0x1A3u, 0, 0, 0, 0, 8, 20,
#define PPUTLIMPL_UDEC_418u  0x1A2u, 0, 0, 0, 0, 8, 20, 2, 11, 19
#define PPUTLIMPL_UDEC_417u  0x1A1u, 0, 0, 0, 0, 8, 20, 3, 139
#define PPUTLIMPL_UDEC_416u  0x1A0u, 0, 0, 0, 0, 8, 20, 2, 2, 2, 2, 2, 13
#define PPUTLIMPL_UDEC_415u  0x19Fu, 0, 0, 0, 0, 8, 20, 5, 83
#define PPUTLIMPL_UDEC_414u  0x19Eu, 0, 0, 0, 0, 8, 20, 2, 3, 3, 23
#define PPUTLIMPL_UDEC_413u  0x19Du, 0, 0, 0, 0, 8, 20, 7, 59
#define PPUTLIMPL_UDEC_412u  0x19Cu, 0, 0, 0, 0, 8, 20, 2, 2, 103
#define PPUTLIMPL_UDEC_411u  0x19Bu, 0, 0, 0, 0, 8, 20, 3, 137
#define PPUTLIMPL_UDEC_410u  0x19Au, 0, 0, 0, 0, 8, 20, 2, 5, 41
#define PPUTLIMPL_UDEC_409u  0x199u, 0, 0, 0, 0, 8, 20,
#define PPUTLIMPL_UDEC_408u  0x198u, 0, 0, 0, 0, 8, 20, 2, 2, 2, 3, 17
#define PPUTLIMPL_UDEC_407u  0x197u, 0, 0, 0, 0, 8, 20, 11, 37
#define PPUTLIMPL_UDEC_406u  0x196u, 0, 0, 0, 0, 8, 20, 2, 7, 29
#define PPUTLIMPL_UDEC_405u  0x195u, 0, 0, 0, 0, 8, 20, 3, 3, 3, 3, 5
#define PPUTLIMPL_UDEC_404u  0x194u, 0, 0, 0, 0, 8, 20, 2, 2, 101
#define PPUTLIMPL_UDEC_403u  0x193u, 0, 0, 0, 0, 8, 20, 13, 31
#define PPUTLIMPL_UDEC_402u  0x192u, 0, 0, 0, 0, 8, 20, 2, 3, 67
#define PPUTLIMPL_UDEC_401u  0x191u, 0, 0, 0, 0, 8, 20,
#define PPUTLIMPL_UDEC_400u  0x190u, 0, 0, 0, 0, 8, 20, 2, 2, 2, 2, 5, 5
#define PPUTLIMPL_UDEC_399u  0x18Fu, 0, 0, 0, 0, 8, 19, 3, 7, 19
#define PPUTLIMPL_UDEC_398u  0x18Eu, 0, 0, 0, 0, 8, 19, 2, 199
#define PPUTLIMPL_UDEC_397u  0x18Du, 0, 0, 0, 0, 8, 19,
#define PPUTLIMPL_UDEC_396u  0x18Cu, 0, 0, 0, 0, 8, 19, 2, 2, 3, 3, 11
#define PPUTLIMPL_UDEC_395u  0x18Bu, 0, 0, 0, 0, 8, 19, 5, 79
#define PPUTLIMPL_UDEC_394u  0x18Au, 0, 0, 0, 0, 8, 19, 2, 197
#define PPUTLIMPL_UDEC_393u  0x189u, 0, 0, 0, 0, 8, 19, 3, 131
#define PPUTLIMPL_UDEC_392u  0x188u, 0, 0, 0, 0, 8, 19, 2, 2, 2, 7, 7
#define PPUTLIMPL_UDEC_391u  0x187u, 0, 0, 0, 0, 8, 19, 17, 23
#define PPUTLIMPL_UDEC_390u  0x186u, 0, 0, 0, 0, 8, 19, 2, 3, 5, 13
#define PPUTLIMPL_UDEC_389u  0x185u, 0, 0, 0, 0, 8, 19,
#define PPUTLIMPL_UDEC_388u  0x184u, 0, 0, 0, 0, 8, 19, 2, 2, 97
#define PPUTLIMPL_UDEC_387u  0x183u, 0, 0, 0, 0, 8, 19, 3, 3, 43
#define PPUTLIMPL_UDEC_386u  0x182u, 0, 0, 0, 0, 8, 19, 2, 193
#define PPUTLIMPL_UDEC_385u  0x181u, 0, 0, 0, 0, 8, 19, 5, 7, 11
#define PPUTLIMPL_UDEC_384u  0x180u, 0, 0, 0, 0, 8, 19, 2, 2, 2, 2, 2, 2, 2, 3
#define PPUTLIMPL_UDEC_383u  0x17Fu, 0, 0, 0, 0, 8, 19,
#define PPUTLIMPL_UDEC_382u  0x17Eu, 0, 0, 0, 0, 8, 19, 2, 191
#define PPUTLIMPL_UDEC_381u  0x17Du, 0, 0, 0, 0, 8, 19, 3, 127
#define PPUTLIMPL_UDEC_380u  0x17Cu, 0, 0, 0, 0, 8, 19, 2, 2, 5, 19
#define PPUTLIMPL_UDEC_379u  0x17Bu, 0, 0, 0, 0, 8, 19,
#define PPUTLIMPL_UDEC_378u  0x17Au, 0, 0, 0, 0, 8, 19, 2, 3, 3, 3, 7
#define PPUTLIMPL_UDEC_377u  0x179u, 0, 0, 0, 0, 8, 19, 13, 29
#define PPUTLIMPL_UDEC_376u  0x178u, 0, 0, 0, 0, 8, 19, 2, 2, 2, 47
#define PPUTLIMPL_UDEC_375u  0x177u, 0, 0, 0, 0, 8, 19, 3, 5, 5, 5
#define PPUTLIMPL_UDEC_374u  0x176u, 0, 0, 0, 0, 8, 19, 2, 11, 17
#define PPUTLIMPL_UDEC_373u  0x175u, 0, 0, 0, 0, 8, 19,
#define PPUTLIMPL_UDEC_372u  0x174u, 0, 0, 0, 0, 8, 19, 2, 2, 3, 31
#define PPUTLIMPL_UDEC_371u  0x173u, 0, 0, 0, 0, 8, 19, 7, 53
#define PPUTLIMPL_UDEC_370u  0x172u, 0, 0, 0, 0, 8, 19, 2, 5, 37
#define PPUTLIMPL_UDEC_369u  0x171u, 0, 0, 0, 0, 8, 19, 3, 3, 41
#define PPUTLIMPL_UDEC_368u  0x170u, 0, 0, 0, 0, 8, 19, 2, 2, 2, 2, 23
#define PPUTLIMPL_UDEC_367u  0x16Fu, 0, 0, 0, 0, 8, 19,
#define PPUTLIMPL_UDEC_366u  0x16Eu, 0, 0, 0, 0, 8, 19, 2, 3, 61
#define PPUTLIMPL_UDEC_365u  0x16Du, 0, 0, 0, 0, 8, 19, 5, 73
#define PPUTLIMPL_UDEC_364u  0x16Cu, 0, 0, 0, 0, 8, 19, 2, 2, 7, 13
#define PPUTLIMPL_UDEC_363u  0x16Bu, 0, 0, 0, 0, 8, 19, 3, 11, 11
#define PPUTLIMPL_UDEC_362u  0x16Au, 0, 0, 0, 0, 8, 19, 2, 181
#define PPUTLIMPL_UDEC_361u  0x169u, 0, 0, 0, 0, 8, 19, 19, 19
#define PPUTLIMPL_UDEC_360u  0x168u, 0, 0, 0, 0, 8, 18, 2, 2, 2, 3, 3, 5
#define PPUTLIMPL_UDEC_359u  0x167u, 0, 0, 0, 0, 8, 18,
#define PPUTLIMPL_UDEC_358u  0x166u, 0, 0, 0, 0, 8, 18, 2, 179
#define PPUTLIMPL_UDEC_357u  0x165u, 0, 0, 0, 0, 8, 18, 3, 7, 17
#define PPUTLIMPL_UDEC_356u  0x164u, 0, 0, 0, 0, 8, 18, 2, 2, 89
#define PPUTLIMPL_UDEC_355u  0x163u, 0, 0, 0, 0, 8, 18, 5, 71
#define PPUTLIMPL_UDEC_354u  0x162u, 0, 0, 0, 0, 8, 18, 2, 3, 59
#define PPUTLIMPL_UDEC_353u  0x161u, 0, 0, 0, 0, 8, 18,
#define PPUTLIMPL_UDEC_352u  0x160u, 0, 0, 0, 0, 8, 18, 2, 2, 2, 2, 2, 11
#define PPUTLIMPL_UDEC_351u  0x15Fu, 0, 0, 0, 0, 8, 18, 3, 3, 3, 13
#define PPUTLIMPL_UDEC_350u  0x15Eu, 0, 0, 0, 0, 8, 18, 2, 5, 5, 7
#define PPUTLIMPL_UDEC_349u  0x15Du, 0, 0, 0, 0, 8, 18,
#define PPUTLIMPL_UDEC_348u  0x15Cu, 0, 0, 0, 0, 8, 18, 2, 2, 3, 29
#define PPUTLIMPL_UDEC_347u  0x15Bu, 0, 0, 0, 0, 8, 18,
#define PPUTLIMPL_UDEC_346u  0x15Au, 0, 0, 0, 0, 8, 18, 2, 173
#define PPUTLIMPL_UDEC_345u  0x159u, 0, 0, 0, 0, 8, 18, 3, 5, 23
#define PPUTLIMPL_UDEC_344u  0x158u, 0, 0, 0, 0, 8, 18, 2, 2, 2, 43
#define PPUTLIMPL_UDEC_343u  0x157u, 0, 0, 0, 0, 8, 18, 7, 7, 7
#define PPUTLIMPL_UDEC_342u  0x156u, 0, 0, 0, 0, 8, 18, 2, 3, 3, 19
#define PPUTLIMPL_UDEC_341u  0x155u, 0, 0, 0, 0, 8, 18, 11, 31
#define PPUTLIMPL_UDEC_340u  0x154u, 0, 0, 0, 0, 8, 18, 2, 2, 5, 17
#define PPUTLIMPL_UDEC_339u  0x153u, 0, 0, 0, 0, 8, 18, 3, 113
#define PPUTLIMPL_UDEC_338u  0x152u, 0, 0, 0, 0, 8, 18, 2, 13, 13
#define PPUTLIMPL_UDEC_337u  0x151u, 0, 0, 0, 0, 8, 18,
#define PPUTLIMPL_UDEC_336u  0x150u, 0, 0, 0, 0, 8, 18, 2, 2, 2, 2, 3, 7
#define PPUTLIMPL_UDEC_335u  0x14Fu, 0, 0, 0, 0, 8, 18, 5, 67
#define PPUTLIMPL_UDEC_334u  0x14Eu, 0, 0, 0, 0, 8, 18, 2, 167
#define PPUTLIMPL_UDEC_333u  0x14Du, 0, 0, 0, 0, 8, 18, 3, 3, 37
#define PPUTLIMPL_UDEC_332u  0x14Cu, 0, 0, 0, 0, 8, 18, 2, 2, 83
#define PPUTLIMPL_UDEC_331u  0x14Bu, 0, 0, 0, 0, 8, 18,
#define PPUTLIMPL_UDEC_330u  0x14Au, 0, 0, 0, 0, 8, 18, 2, 3, 5, 11
#define PPUTLIMPL_UDEC_329u  0x149u, 0, 0, 0, 0, 8, 18, 7, 47
#define PPUTLIMPL_UDEC_328u  0x148u, 0, 0, 0, 0, 8, 18, 2, 2, 2, 41
#define PPUTLIMPL_UDEC_327u  0x147u, 0, 0, 0, 0, 8, 18, 3, 109
#define PPUTLIMPL_UDEC_326u  0x146u, 0, 0, 0, 0, 8, 18, 2, 163
#define PPUTLIMPL_UDEC_325u  0x145u, 0, 0, 0, 0, 8, 18, 5, 5, 13
#define PPUTLIMPL_UDEC_324u  0x144u, 0, 0, 0, 0, 8, 18, 2, 2, 3, 3, 3, 3
#define PPUTLIMPL_UDEC_323u  0x143u, 0, 0, 0, 0, 8, 17, 17, 19
#define PPUTLIMPL_UDEC_322u  0x142u, 0, 0, 0, 0, 8, 17, 2, 7, 23
#define PPUTLIMPL_UDEC_321u  0x141u, 0, 0, 0, 0, 8, 17, 3, 107
#define PPUTLIMPL_UDEC_320u  0x140u, 0, 0, 0, 0, 8, 17, 2, 2, 2, 2, 2, 2, 5
#define PPUTLIMPL_UDEC_319u  0x13Fu, 0, 0, 0, 0, 8, 17, 11, 29
#define PPUTLIMPL_UDEC_318u  0x13Eu, 0, 0, 0, 0, 8, 17, 2, 3, 53
#define PPUTLIMPL_UDEC_317u  0x13Du, 0, 0, 0, 0, 8, 17,
#define PPUTLIMPL_UDEC_316u  0x13Cu, 0, 0, 0, 0, 8, 17, 2, 2, 79
#define PPUTLIMPL_UDEC_315u  0x13Bu, 0, 0, 0, 0, 8, 17, 3, 3, 5, 7
#define PPUTLIMPL_UDEC_314u  0x13Au, 0, 0, 0, 0, 8, 17, 2, 157
#define PPUTLIMPL_UDEC_313u  0x139u, 0, 0, 0, 0, 8, 17,
#define PPUTLIMPL_UDEC_312u  0x138u, 0, 0, 0, 0, 8, 17, 2, 2, 2, 3, 13
#define PPUTLIMPL_UDEC_311u  0x137u, 0, 0, 0, 0, 8, 17,
#define PPUTLIMPL_UDEC_310u  0x136u, 0, 0, 0, 0, 8, 17, 2, 5, 31
#define PPUTLIMPL_UDEC_309u  0x135u, 0, 0, 0, 0, 8, 17, 3, 103
#define PPUTLIMPL_UDEC_308u  0x134u, 0, 0, 0, 0, 8, 17, 2, 2, 7, 11
#define PPUTLIMPL_UDEC_307u  0x133u, 0, 0, 0, 0, 8, 17,
#define PPUTLIMPL_UDEC_306u  0x132u, 0, 0, 0, 0, 8, 17, 2, 3, 3, 17
#define PPUTLIMPL_UDEC_305u  0x131u, 0, 0, 0, 0, 8, 17, 5, 61
#define PPUTLIMPL_UDEC_304u  0x130u, 0, 0, 0, 0, 8, 17, 2, 2, 2, 2, 19
#define PPUTLIMPL_UDEC_303u  0x12Fu, 0, 0, 0, 0, 8, 17, 3, 101
#define PPUTLIMPL_UDEC_302u  0x12Eu, 0, 0, 0, 0, 8, 17, 2, 151
#define PPUTLIMPL_UDEC_301u  0x12Du, 0, 0, 0, 0, 8, 17, 7, 43
#define PPUTLIMPL_UDEC_300u  0x12Cu, 0, 0, 0, 0, 8, 17, 2, 2, 3, 5, 5
#define PPUTLIMPL_UDEC_299u  0x12Bu, 0, 0, 0, 0, 8, 17, 13, 23
#define PPUTLIMPL_UDEC_298u  0x12Au, 0, 0, 0, 0, 8, 17, 2, 149
#define PPUTLIMPL_UDEC_297u  0x129u, 0, 0, 0, 0, 8, 17, 3, 3, 3, 11
#define PPUTLIMPL_UDEC_296u  0x128u, 0, 0, 0, 0, 8, 17, 2, 2, 2, 37
#define PPUTLIMPL_UDEC_295u  0x127u, 0, 0, 0, 0, 8, 17, 5, 59
#define PPUTLIMPL_UDEC_294u  0x126u, 0, 0, 0, 0, 8, 17, 2, 3, 7, 7
#define PPUTLIMPL_UDEC_293u  0x125u, 0, 0, 0, 0, 8, 17,
#define PPUTLIMPL_UDEC_292u  0x124u, 0, 0, 0, 0, 8, 17, 2, 2, 73
#define PPUTLIMPL_UDEC_291u  0x123u, 0, 0, 0, 0, 8, 17, 3, 97
#define PPUTLIMPL_UDEC_290u  0x122u, 0, 0, 0, 0, 8, 17, 2, 5, 29
#define PPUTLIMPL_UDEC_289u  0x121u, 0, 0, 0, 0, 8, 17, 17, 17
#define PPUTLIMPL_UDEC_288u  0x120u, 0, 0, 0, 0, 8, 16, 2, 2, 2, 2, 2, 3, 3
#define PPUTLIMPL_UDEC_287u  0x11Fu, 0, 0, 0, 0, 8, 16, 7, 41
#define PPUTLIMPL_UDEC_286u  0x11Eu, 0, 0, 0, 0, 8, 16, 2, 11, 13
#define PPUTLIMPL_UDEC_285u  0x11Du, 0, 0, 0, 0, 8, 16, 3, 5, 19
#define PPUTLIMPL_UDEC_284u  0x11Cu, 0, 0, 0, 0, 8, 16, 2, 2, 71
#define PPUTLIMPL_UDEC_283u  0x11Bu, 0, 0, 0, 0, 8, 16,
#define PPUTLIMPL_UDEC_282u  0x11Au, 0, 0, 0, 0, 8, 16, 2, 3, 47
#define PPUTLIMPL_UDEC_281u  0x119u, 0, 0, 0, 0, 8, 16,
#define PPUTLIMPL_UDEC_280u  0x118u, 0, 0, 0, 0, 8, 16, 2, 2, 2, 5, 7
#define PPUTLIMPL_UDEC_279u  0x117u, 0, 0, 0, 0, 8, 16, 3, 3, 31
#define PPUTLIMPL_UDEC_278u  0x116u, 0, 0, 0, 0, 8, 16, 2, 139
#define PPUTLIMPL_UDEC_277u  0x115u, 0, 0, 0, 0, 8, 16,
#define PPUTLIMPL_UDEC_276u  0x114u, 0, 0, 0, 0, 8, 16, 2, 2, 3, 23
#define PPUTLIMPL_UDEC_275u  0x113u, 0, 0, 0, 0, 8, 16, 5, 5, 11
#define PPUTLIMPL_UDEC_274u  0x112u, 0, 0, 0, 0, 8, 16, 2, 137
#define PPUTLIMPL_UDEC_273u  0x111u, 0, 0, 0, 0, 8, 16, 3, 7, 13
#define PPUTLIMPL_UDEC_272u  0x110u, 0, 0, 0, 0, 8, 16, 2, 2, 2, 2, 17
#define PPUTLIMPL_UDEC_271u  0x10Fu, 0, 0, 0, 0, 8, 16,
#define PPUTLIMPL_UDEC_270u  0x10Eu, 0, 0, 0, 0, 8, 16, 2, 3, 3, 3, 5
#define PPUTLIMPL_UDEC_269u  0x10Du, 0, 0, 0, 0, 8, 16,
#define PPUTLIMPL_UDEC_268u  0x10Cu, 0, 0, 0, 0, 8, 16, 2, 2, 67
#define PPUTLIMPL_UDEC_267u  0x10Bu, 0, 0, 0, 0, 8, 16, 3, 89
#define PPUTLIMPL_UDEC_266u  0x10Au, 0, 0, 0, 0, 8, 16, 2, 7, 19
#define PPUTLIMPL_UDEC_265u  0x109u, 0, 0, 0, 0, 8, 16, 5, 53
#define PPUTLIMPL_UDEC_264u  0x108u, 0, 0, 0, 0, 8, 16, 2, 2, 2, 3, 11
#define PPUTLIMPL_UDEC_263u  0x107u, 0, 0, 0, 0, 8, 16,
#define PPUTLIMPL_UDEC_262u  0x106u, 0, 0, 0, 0, 8, 16, 2, 131
#define PPUTLIMPL_UDEC_261u  0x105u, 0, 0, 0, 0, 8, 16, 3, 3, 29
#define PPUTLIMPL_UDEC_260u  0x104u, 0, 0, 0, 0, 8, 16, 2, 2, 5, 13
#define PPUTLIMPL_UDEC_259u  0x103u, 0, 0, 0, 0, 8, 16, 7, 37
#define PPUTLIMPL_UDEC_258u  0x102u, 0, 0, 0, 0, 8, 16, 2, 3, 43
#define PPUTLIMPL_UDEC_257u  0x101u, 0, 0, 0, 0, 8, 16,
#define PPUTLIMPL_UDEC_256u  0x100u, 0, 0, 0, 0, 8, 16, 2, 2, 2, 2, 2, 2, 2, 2
#define PPUTLIMPL_UDEC_255u  0x0FFu, 1, 1, 1, 1, 7, 15, 3, 5, 17
#define PPUTLIMPL_UDEC_254u  0x0FEu, 1, 1, 1, 1, 7, 15, 2, 127
#define PPUTLIMPL_UDEC_253u  0x0FDu, 1, 1, 1, 1, 7, 15, 11, 23
#define PPUTLIMPL_UDEC_252u  0x0FCu, 1, 1, 1, 1, 7, 15, 2, 2, 3, 3, 7
#define PPUTLIMPL_UDEC_251u  0x0FBu, 1, 1, 1, 1, 7, 15,
#define PPUTLIMPL_UDEC_250u  0x0FAu, 1, 1, 1, 1, 7, 15, 2, 5, 5, 5
#define PPUTLIMPL_UDEC_249u  0x0F9u, 1, 1, 1, 1, 7, 15, 3, 83
#define PPUTLIMPL_UDEC_248u  0x0F8u, 1, 1, 1, 1, 7, 15, 2, 2, 2, 31
#define PPUTLIMPL_UDEC_247u  0x0F7u, 1, 1, 1, 1, 7, 15, 13, 19
#define PPUTLIMPL_UDEC_246u  0x0F6u, 1, 1, 1, 1, 7, 15, 2, 3, 41
#define PPUTLIMPL_UDEC_245u  0x0F5u, 1, 1, 1, 1, 7, 15, 5, 7, 7
#define PPUTLIMPL_UDEC_244u  0x0F4u, 1, 1, 1, 1, 7, 15, 2, 2, 61
#define PPUTLIMPL_UDEC_243u  0x0F3u, 1, 1, 1, 1, 7, 15, 3, 3, 3, 3, 3
#define PPUTLIMPL_UDEC_242u  0x0F2u, 1, 1, 1, 1, 7, 15, 2, 11, 11
#define PPUTLIMPL_UDEC_241u  0x0F1u, 1, 1, 1, 1, 7, 15,
#define PPUTLIMPL_UDEC_240u  0x0F0u, 1, 1, 1, 1, 7, 15, 2, 2, 2, 2, 3, 5
#define PPUTLIMPL_UDEC_239u  0x0EFu, 1, 1, 1, 1, 7, 15,
#define PPUTLIMPL_UDEC_238u  0x0EEu, 1, 1, 1, 1, 7, 15, 2, 7, 17
#define PPUTLIMPL_UDEC_237u  0x0EDu, 1, 1, 1, 1, 7, 15, 3, 79
#define PPUTLIMPL_UDEC_236u  0x0ECu, 1, 1, 1, 1, 7, 15, 2, 2, 59
#define PPUTLIMPL_UDEC_235u  0x0EBu, 1, 1, 1, 1, 7, 15, 5, 47
#define PPUTLIMPL_UDEC_234u  0x0EAu, 1, 1, 1, 1, 7, 15, 2, 3, 3, 13
#define PPUTLIMPL_UDEC_233u  0x0E9u, 1, 1, 1, 1, 7, 15,
#define PPUTLIMPL_UDEC_232u  0x0E8u, 1, 1, 1, 1, 7, 15, 2, 2, 2, 29
#define PPUTLIMPL_UDEC_231u  0x0E7u, 1, 1, 1, 1, 7, 15, 3, 7, 11
#define PPUTLIMPL_UDEC_230u  0x0E6u, 1, 1, 1, 1, 7, 15, 2, 5, 23
#define PPUTLIMPL_UDEC_229u  0x0E5u, 1, 1, 1, 1, 7, 15,
#define PPUTLIMPL_UDEC_228u  0x0E4u, 1, 1, 1, 1, 7, 15, 2, 2, 3, 19
#define PPUTLIMPL_UDEC_227u  0x0E3u, 1, 1, 1, 1, 7, 15,
#define PPUTLIMPL_UDEC_226u  0x0E2u, 1, 1, 1, 1, 7, 15, 2, 113
#define PPUTLIMPL_UDEC_225u  0x0E1u, 1, 1, 1, 1, 7, 15, 3, 3, 5, 5
#define PPUTLIMPL_UDEC_224u  0x0E0u, 1, 1, 1, 1, 7, 14, 2, 2, 2, 2, 2, 7
#define PPUTLIMPL_UDEC_223u  0x0DFu, 1, 1, 1, 1, 7, 14,
#define PPUTLIMPL_UDEC_222u  0x0DEu, 1, 1, 1, 1, 7, 14, 2, 3, 37
#define PPUTLIMPL_UDEC_221u  0x0DDu, 1, 1, 1, 1, 7, 14, 13, 17
#define PPUTLIMPL_UDEC_220u  0x0DCu, 1, 1, 1, 1, 7, 14, 2, 2, 5, 11
#define PPUTLIMPL_UDEC_219u  0x0DBu, 1, 1, 1, 1, 7, 14, 3, 73
#define PPUTLIMPL_UDEC_218u  0x0DAu, 1, 1, 1, 1, 7, 14, 2, 109
#define PPUTLIMPL_UDEC_217u  0x0D9u, 1, 1, 1, 1, 7, 14, 7, 31
#define PPUTLIMPL_UDEC_216u  0x0D8u, 1, 1, 1, 1, 7, 14, 2, 2, 2, 3, 3, 3
#define PPUTLIMPL_UDEC_215u  0x0D7u, 1, 1, 1, 1, 7, 14, 5, 43
#define PPUTLIMPL_UDEC_214u  0x0D6u, 1, 1, 1, 1, 7, 14, 2, 107
#define PPUTLIMPL_UDEC_213u  0x0D5u, 1, 1, 1, 1, 7, 14, 3, 71
#define PPUTLIMPL_UDEC_212u  0x0D4u, 1, 1, 1, 1, 7, 14, 2, 2, 53
#define PPUTLIMPL_UDEC_211u  0x0D3u, 1, 1, 1, 1, 7, 14,
#define PPUTLIMPL_UDEC_210u  0x0D2u, 1, 1, 1, 1, 7, 14, 2, 3, 5, 7
#define PPUTLIMPL_UDEC_209u  0x0D1u, 1, 1, 1, 1, 7, 14, 11, 19
#define PPUTLIMPL_UDEC_208u  0x0D0u, 1, 1, 1, 1, 7, 14, 2, 2, 2, 2, 13
#define PPUTLIMPL_UDEC_207u  0x0CFu, 1, 1, 1, 1, 7, 14, 3, 3, 23
#define PPUTLIMPL_UDEC_206u  0x0CEu, 1, 1, 1, 1, 7, 14, 2, 103
#define PPUTLIMPL_UDEC_205u  0x0CDu, 1, 1, 1, 1, 7, 14, 5, 41
#define PPUTLIMPL_UDEC_204u  0x0CCu, 1, 1, 1, 1, 7, 14, 2, 2, 3, 17
#define PPUTLIMPL_UDEC_203u  0x0CBu, 1, 1, 1, 1, 7, 14, 7, 29
#define PPUTLIMPL_UDEC_202u  0x0CAu, 1, 1, 1, 1, 7, 14, 2, 101
#define PPUTLIMPL_UDEC_201u  0x0C9u, 1, 1, 1, 1, 7, 14, 3, 67
#define PPUTLIMPL_UDEC_200u  0x0C8u, 1, 1, 1, 1, 7, 14, 2, 2, 2, 5, 5
#define PPUTLIMPL_UDEC_199u  0x0C7u, 1, 1, 1, 1, 7, 14,
#define PPUTLIMPL_UDEC_198u  0x0C6u, 1, 1, 1, 1, 7, 14, 2, 3, 3, 11
#define PPUTLIMPL_UDEC_197u  0x0C5u, 1, 1, 1, 1, 7, 14,
#define PPUTLIMPL_UDEC_196u  0x0C4u, 1, 1, 1, 1, 7, 14, 2, 2, 7, 7
#define PPUTLIMPL_UDEC_195u  0x0C3u, 1, 1, 1, 1, 7, 13, 3, 5, 13
#define PPUTLIMPL_UDEC_194u  0x0C2u, 1, 1, 1, 1, 7, 13, 2, 97
#define PPUTLIMPL_UDEC_193u  0x0C1u, 1, 1, 1, 1, 7, 13,
#define PPUTLIMPL_UDEC_192u  0x0C0u, 1, 1, 1, 1, 7, 13, 2, 2, 2, 2, 2, 2, 3
#define PPUTLIMPL_UDEC_191u  0x0BFu, 1, 1, 1, 1, 7, 13,
#define PPUTLIMPL_UDEC_190u  0x0BEu, 1, 1, 1, 1, 7, 13, 2, 5, 19
#define PPUTLIMPL_UDEC_189u  0x0BDu, 1, 1, 1, 1, 7, 13, 3, 3, 3, 7
#define PPUTLIMPL_UDEC_188u  0x0BCu, 1, 1, 1, 1, 7, 13, 2, 2, 47
#define PPUTLIMPL_UDEC_187u  0x0BBu, 1, 1, 1, 1, 7, 13, 11, 17
#define PPUTLIMPL_UDEC_186u  0x0BAu, 1, 1, 1, 1, 7, 13, 2, 3, 31
#define PPUTLIMPL_UDEC_185u  0x0B9u, 1, 1, 1, 1, 7, 13, 5, 37
#define PPUTLIMPL_UDEC_184u  0x0B8u, 1, 1, 1, 1, 7, 13, 2, 2, 2, 23
#define PPUTLIMPL_UDEC_183u  0x0B7u, 1, 1, 1, 1, 7, 13, 3, 61
#define PPUTLIMPL_UDEC_182u  0x0B6u, 1, 1, 1, 1, 7, 13, 2, 7, 13
#define PPUTLIMPL_UDEC_181u  0x0B5u, 1, 1, 1, 1, 7, 13,
#define PPUTLIMPL_UDEC_180u  0x0B4u, 1, 1, 1, 1, 7, 13, 2, 2, 3, 3, 5
#define PPUTLIMPL_UDEC_179u  0x0B3u, 1, 1, 1, 1, 7, 13,
#define PPUTLIMPL_UDEC_178u  0x0B2u, 1, 1, 1, 1, 7, 13, 2, 89
#define PPUTLIMPL_UDEC_177u  0x0B1u, 1, 1, 1, 1, 7, 13, 3, 59
#define PPUTLIMPL_UDEC_176u  0x0B0u, 1, 1, 1, 1, 7, 13, 2, 2, 2, 2, 11
#define PPUTLIMPL_UDEC_175u  0x0AFu, 1, 1, 1, 1, 7, 13, 5, 5, 7
#define PPUTLIMPL_UDEC_174u  0x0AEu, 1, 1, 1, 1, 7, 13, 2, 3, 29
#define PPUTLIMPL_UDEC_173u  0x0ADu, 1, 1, 1, 1, 7, 13,
#define PPUTLIMPL_UDEC_172u  0x0ACu, 1, 1, 1, 1, 7, 13, 2, 2, 43
#define PPUTLIMPL_UDEC_171u  0x0ABu, 1, 1, 1, 1, 7, 13, 3, 3, 19
#define PPUTLIMPL_UDEC_170u  0x0AAu, 1, 1, 1, 1, 7, 13, 2, 5, 17
#define PPUTLIMPL_UDEC_169u  0x0A9u, 1, 1, 1, 1, 7, 13, 13, 13
#define PPUTLIMPL_UDEC_168u  0x0A8u, 1, 1, 1, 1, 7, 12, 2, 2, 2, 3, 7
#define PPUTLIMPL_UDEC_167u  0x0A7u, 1, 1, 1, 1, 7, 12,
#define PPUTLIMPL_UDEC_166u  0x0A6u, 1, 1, 1, 1, 7, 12, 2, 83
#define PPUTLIMPL_UDEC_165u  0x0A5u, 1, 1, 1, 1, 7, 12, 3, 5, 11
#define PPUTLIMPL_UDEC_164u  0x0A4u, 1, 1, 1, 1, 7, 12, 2, 2, 41
#define PPUTLIMPL_UDEC_163u  0x0A3u, 1, 1, 1, 1, 7, 12,
#define PPUTLIMPL_UDEC_162u  0x0A2u, 1, 1, 1, 1, 7, 12, 2, 3, 3, 3, 3
#define PPUTLIMPL_UDEC_161u  0x0A1u, 1, 1, 1, 1, 7, 12, 7, 23
#define PPUTLIMPL_UDEC_160u  0x0A0u, 1, 1, 1, 1, 7, 12, 2, 2, 2, 2, 2, 5
#define PPUTLIMPL_UDEC_159u  0x09Fu, 1, 1, 1, 1, 7, 12, 3, 53
#define PPUTLIMPL_UDEC_158u  0x09Eu, 1, 1, 1, 1, 7, 12, 2, 79
#define PPUTLIMPL_UDEC_157u  0x09Du, 1, 1, 1, 1, 7, 12,
#define PPUTLIMPL_UDEC_156u  0x09Cu, 1, 1, 1, 1, 7, 12, 2, 2, 3, 13
#define PPUTLIMPL_UDEC_155u  0x09Bu, 1, 1, 1, 1, 7, 12, 5, 31
#define PPUTLIMPL_UDEC_154u  0x09Au, 1, 1, 1, 1, 7, 12, 2, 7, 11
#define PPUTLIMPL_UDEC_153u  0x099u, 1, 1, 1, 1, 7, 12, 3, 3, 17
#define PPUTLIMPL_UDEC_152u  0x098u, 1, 1, 1, 1, 7, 12, 2, 2, 2, 19
#define PPUTLIMPL_UDEC_151u  0x097u, 1, 1, 1, 1, 7, 12,
#define PPUTLIMPL_UDEC_150u  0x096u, 1, 1, 1, 1, 7, 12, 2, 3, 5, 5
#define PPUTLIMPL_UDEC_149u  0x095u, 1, 1, 1, 1, 7, 12,
#define PPUTLIMPL_UDEC_148u  0x094u, 1, 1, 1, 1, 7, 12, 2, 2, 37
#define PPUTLIMPL_UDEC_147u  0x093u, 1, 1, 1, 1, 7, 12, 3, 7, 7
#define PPUTLIMPL_UDEC_146u  0x092u, 1, 1, 1, 1, 7, 12, 2, 73
#define PPUTLIMPL_UDEC_145u  0x091u, 1, 1, 1, 1, 7, 12, 5, 29
#define PPUTLIMPL_UDEC_144u  0x090u, 1, 1, 1, 1, 7, 12, 2, 2, 2, 2, 3, 3
#define PPUTLIMPL_UDEC_143u  0x08Fu, 1, 1, 1, 1, 7, 11, 11, 13
#define PPUTLIMPL_UDEC_142u  0x08Eu, 1, 1, 1, 1, 7, 11, 2, 71
#define PPUTLIMPL_UDEC_141u  0x08Du, 1, 1, 1, 1, 7, 11, 3, 47
#define PPUTLIMPL_UDEC_140u  0x08Cu, 1, 1, 1, 1, 7, 11, 2, 2, 5, 7
#define PPUTLIMPL_UDEC_139u  0x08Bu, 1, 1, 1, 1, 7, 11,
#define PPUTLIMPL_UDEC_138u  0x08Au, 1, 1, 1, 1, 7, 11, 2, 3, 23
#define PPUTLIMPL_UDEC_137u  0x089u, 1, 1, 1, 1, 7, 11,
#define PPUTLIMPL_UDEC_136u  0x088u, 1, 1, 1, 1, 7, 11, 2, 2, 2, 17
#define PPUTLIMPL_UDEC_135u  0x087u, 1, 1, 1, 1, 7, 11, 3, 3, 3, 5
#define PPUTLIMPL_UDEC_134u  0x086u, 1, 1, 1, 1, 7, 11, 2, 67
#define PPUTLIMPL_UDEC_133u  0x085u, 1, 1, 1, 1, 7, 11, 7, 19
#define PPUTLIMPL_UDEC_132u  0x084u, 1, 1, 1, 1, 7, 11, 2, 2, 3, 11
#define PPUTLIMPL_UDEC_131u  0x083u, 1, 1, 1, 1, 7, 11,
#define PPUTLIMPL_UDEC_130u  0x082u, 1, 1, 1, 1, 7, 11, 2, 5, 13
#define PPUTLIMPL_UDEC_129u  0x081u, 1, 1, 1, 1, 7, 11, 3, 43
#define PPUTLIMPL_UDEC_128u  0x080u, 1, 1, 1, 1, 7, 11, 2, 2, 2, 2, 2, 2, 2
#define PPUTLIMPL_UDEC_127u  0x07Fu, 1, 1, 1, 1, 6, 11,
#define PPUTLIMPL_UDEC_126u  0x07Eu, 1, 1, 1, 1, 6, 11, 2, 3, 3, 7
#define PPUTLIMPL_UDEC_125u  0x07Du, 1, 1, 1, 1, 6, 11, 5, 5, 5
#define PPUTLIMPL_UDEC_124u  0x07Cu, 1, 1, 1, 1, 6, 11, 2, 2, 31
#define PPUTLIMPL_UDEC_123u  0x07Bu, 1, 1, 1, 1, 6, 11, 3, 41
#define PPUTLIMPL_UDEC_122u  0x07Au, 1, 1, 1, 1, 6, 11, 2, 61
#define PPUTLIMPL_UDEC_121u  0x079u, 1, 1, 1, 1, 6, 11, 11, 11
#define PPUTLIMPL_UDEC_120u  0x078u, 1, 1, 1, 1, 6, 10, 2, 2, 2, 3, 5
#define PPUTLIMPL_UDEC_119u  0x077u, 1, 1, 1, 1, 6, 10, 7, 17
#define PPUTLIMPL_UDEC_118u  0x076u, 1, 1, 1, 1, 6, 10, 2, 59
#define PPUTLIMPL_UDEC_117u  0x075u, 1, 1, 1, 1, 6, 10, 3, 3, 13
#define PPUTLIMPL_UDEC_116u  0x074u, 1, 1, 1, 1, 6, 10, 2, 2, 29
#define PPUTLIMPL_UDEC_115u  0x073u, 1, 1, 1, 1, 6, 10, 5, 23
#define PPUTLIMPL_UDEC_114u  0x072u, 1, 1, 1, 1, 6, 10, 2, 3, 19
#define PPUTLIMPL_UDEC_113u  0x071u, 1, 1, 1, 1, 6, 10,
#define PPUTLIMPL_UDEC_112u  0x070u, 1, 1, 1, 1, 6, 10, 2, 2, 2, 2, 7
#define PPUTLIMPL_UDEC_111u  0x06Fu, 1, 1, 1, 1, 6, 10, 3, 37
#define PPUTLIMPL_UDEC_110u  0x06Eu, 1, 1, 1, 1, 6, 10, 2, 5, 11
#define PPUTLIMPL_UDEC_109u  0x06Du, 1, 1, 1, 1, 6, 10,
#define PPUTLIMPL_UDEC_108u  0x06Cu, 1, 1, 1, 1, 6, 10, 2, 2, 3, 3, 3
#define PPUTLIMPL_UDEC_107u  0x06Bu, 1, 1, 1, 1, 6, 10,
#define PPUTLIMPL_UDEC_106u  0x06Au, 1, 1, 1, 1, 6, 10, 2, 53
#define PPUTLIMPL_UDEC_105u  0x069u, 1, 1, 1, 1, 6, 10, 3, 5, 7
#define PPUTLIMPL_UDEC_104u  0x068u, 1, 1, 1, 1, 6, 10, 2, 2, 2, 13
#define PPUTLIMPL_UDEC_103u  0x067u, 1, 1, 1, 1, 6, 10,
#define PPUTLIMPL_UDEC_102u  0x066u, 1, 1, 1, 1, 6, 10, 2, 3, 17
#define PPUTLIMPL_UDEC_101u  0x065u, 1, 1, 1, 1, 6, 10,
#define PPUTLIMPL_UDEC_100u  0x064u, 1, 1, 1, 1, 6, 10, 2, 2, 5, 5
#define PPUTLIMPL_UDEC_99u   0x063u, 1, 1, 1, 1, 6, 9, 3, 3, 11
#define PPUTLIMPL_UDEC_98u   0x062u, 1, 1, 1, 1, 6, 9, 2, 7, 7
#define PPUTLIMPL_UDEC_97u   0x061u, 1, 1, 1, 1, 6, 9,
#define PPUTLIMPL_UDEC_96u   0x060u, 1, 1, 1, 1, 6, 9, 2, 2, 2, 2, 2, 3
#define PPUTLIMPL_UDEC_95u   0x05Fu, 1, 1, 1, 1, 6, 9, 5, 19
#define PPUTLIMPL_UDEC_94u   0x05Eu, 1, 1, 1, 1, 6, 9, 2, 47
#define PPUTLIMPL_UDEC_93u   0x05Du, 1, 1, 1, 1, 6, 9, 3, 31
#define PPUTLIMPL_UDEC_92u   0x05Cu, 1, 1, 1, 1, 6, 9, 2, 2, 23
#define PPUTLIMPL_UDEC_91u   0x05Bu, 1, 1, 1, 1, 6, 9, 7, 13
#define PPUTLIMPL_UDEC_90u   0x05Au, 1, 1, 1, 1, 6, 9, 2, 3, 3, 5
#define PPUTLIMPL_UDEC_89u   0x059u, 1, 1, 1, 1, 6, 9,
#define PPUTLIMPL_UDEC_88u   0x058u, 1, 1, 1, 1, 6, 9, 2, 2, 2, 11
#define PPUTLIMPL_UDEC_87u   0x057u, 1, 1, 1, 1, 6, 9, 3, 29
#define PPUTLIMPL_UDEC_86u   0x056u, 1, 1, 1, 1, 6, 9, 2, 43
#define PPUTLIMPL_UDEC_85u   0x055u, 1, 1, 1, 1, 6, 9, 5, 17
#define PPUTLIMPL_UDEC_84u   0x054u, 1, 1, 1, 1, 6, 9, 2, 2, 3, 7
#define PPUTLIMPL_UDEC_83u   0x053u, 1, 1, 1, 1, 6, 9,
#define PPUTLIMPL_UDEC_82u   0x052u, 1, 1, 1, 1, 6, 9, 2, 41
#define PPUTLIMPL_UDEC_81u   0x051u, 1, 1, 1, 1, 6, 9, 3, 3, 3, 3
#define PPUTLIMPL_UDEC_80u   0x050u, 1, 1, 1, 1, 6, 8, 2, 2, 2, 2, 5
#define PPUTLIMPL_UDEC_79u   0x04Fu, 1, 1, 1, 1, 6, 8,
#define PPUTLIMPL_UDEC_78u   0x04Eu, 1, 1, 1, 1, 6, 8, 2, 3, 13
#define PPUTLIMPL_UDEC_77u   0x04Du, 1, 1, 1, 1, 6, 8, 7, 11
#define PPUTLIMPL_UDEC_76u   0x04Cu, 1, 1, 1, 1, 6, 8, 2, 2, 19
#define PPUTLIMPL_UDEC_75u   0x04Bu, 1, 1, 1, 1, 6, 8, 3, 5, 5
#define PPUTLIMPL_UDEC_74u   0x04Au, 1, 1, 1, 1, 6, 8, 2, 37
#define PPUTLIMPL_UDEC_73u   0x049u, 1, 1, 1, 1, 6, 8,
#define PPUTLIMPL_UDEC_72u   0x048u, 1, 1, 1, 1, 6, 8, 2, 2, 2, 3, 3
#define PPUTLIMPL_UDEC_71u   0x047u, 1, 1, 1, 1, 6, 8,
#define PPUTLIMPL_UDEC_70u   0x046u, 1, 1, 1, 1, 6, 8, 2, 5, 7
#define PPUTLIMPL_UDEC_69u   0x045u, 1, 1, 1, 1, 6, 8, 3, 23
#define PPUTLIMPL_UDEC_68u   0x044u, 1, 1, 1, 1, 6, 8, 2, 2, 17
#define PPUTLIMPL_UDEC_67u   0x043u, 1, 1, 1, 1, 6, 8,
#define PPUTLIMPL_UDEC_66u   0x042u, 1, 1, 1, 1, 6, 8, 2, 3, 11
#define PPUTLIMPL_UDEC_65u   0x041u, 1, 1, 1, 1, 6, 8, 5, 13
#define PPUTLIMPL_UDEC_64u   0x040u, 1, 1, 1, 1, 6, 8, 2, 2, 2, 2, 2, 2
#define PPUTLIMPL_UDEC_63u   0x03Fu, 1, 1, 1, 1, 5, 7, 3, 3, 7
#define PPUTLIMPL_UDEC_62u   0x03Eu, 1, 1, 1, 1, 5, 7, 2, 31
#define PPUTLIMPL_UDEC_61u   0x03Du, 1, 1, 1, 1, 5, 7,
#define PPUTLIMPL_UDEC_60u   0x03Cu, 1, 1, 1, 1, 5, 7, 2, 2, 3, 5
#define PPUTLIMPL_UDEC_59u   0x03Bu, 1, 1, 1, 1, 5, 7,
#define PPUTLIMPL_UDEC_58u   0x03Au, 1, 1, 1, 1, 5, 7, 2, 29
#define PPUTLIMPL_UDEC_57u   0x039u, 1, 1, 1, 1, 5, 7, 3, 19
#define PPUTLIMPL_UDEC_56u   0x038u, 1, 1, 1, 1, 5, 7, 2, 2, 2, 7
#define PPUTLIMPL_UDEC_55u   0x037u, 1, 1, 1, 1, 5, 7, 5, 11
#define PPUTLIMPL_UDEC_54u   0x036u, 1, 1, 1, 1, 5, 7, 2, 3, 3, 3
#define PPUTLIMPL_UDEC_53u   0x035u, 1, 1, 1, 1, 5, 7,
#define PPUTLIMPL_UDEC_52u   0x034u, 1, 1, 1, 1, 5, 7, 2, 2, 13
#define PPUTLIMPL_UDEC_51u   0x033u, 1, 1, 1, 1, 5, 7, 3, 17
#define PPUTLIMPL_UDEC_50u   0x032u, 1, 1, 1, 1, 5, 7, 2, 5, 5
#define PPUTLIMPL_UDEC_49u   0x031u, 1, 1, 1, 1, 5, 7, 7, 7
#define PPUTLIMPL_UDEC_48u   0x030u, 1, 1, 1, 1, 5, 6, 2, 2, 2, 2, 3
#define PPUTLIMPL_UDEC_47u   0x02Fu, 1, 1, 1, 1, 5, 6,
#define PPUTLIMPL_UDEC_46u   0x02Eu, 1, 1, 1, 1, 5, 6, 2, 23
#define PPUTLIMPL_UDEC_45u   0x02Du, 1, 1, 1, 1, 5, 6, 3, 3, 5
#define PPUTLIMPL_UDEC_44u   0x02Cu, 1, 1, 1, 1, 5, 6, 2, 2, 11
#define PPUTLIMPL_UDEC_43u   0x02Bu, 1, 1, 1, 1, 5, 6,
#define PPUTLIMPL_UDEC_42u   0x02Au, 1, 1, 1, 1, 5, 6, 2, 3, 7
#define PPUTLIMPL_UDEC_41u   0x029u, 1, 1, 1, 1, 5, 6,
#define PPUTLIMPL_UDEC_40u   0x028u, 1, 1, 1, 1, 5, 6, 2, 2, 2, 5
#define PPUTLIMPL_UDEC_39u   0x027u, 1, 1, 1, 1, 5, 6, 3, 13
#define PPUTLIMPL_UDEC_38u   0x026u, 1, 1, 1, 1, 5, 6, 2, 19
#define PPUTLIMPL_UDEC_37u   0x025u, 1, 1, 1, 1, 5, 6,
#define PPUTLIMPL_UDEC_36u   0x024u, 1, 1, 1, 1, 5, 6, 2, 2, 3, 3
#define PPUTLIMPL_UDEC_35u   0x023u, 1, 1, 1, 1, 5, 5, 5, 7
#define PPUTLIMPL_UDEC_34u   0x022u, 1, 1, 1, 1, 5, 5, 2, 17
#define PPUTLIMPL_UDEC_33u   0x021u, 1, 1, 1, 1, 5, 5, 3, 11
#define PPUTLIMPL_UDEC_32u   0x020u, 1, 1, 1, 1, 5, 5, 2, 2, 2, 2, 2
#define PPUTLIMPL_UDEC_31u   0x01Fu, 1, 1, 1, 1, 4, 5,
#define PPUTLIMPL_UDEC_30u   0x01Eu, 1, 1, 1, 1, 4, 5, 2, 3, 5
#define PPUTLIMPL_UDEC_29u   0x01Du, 1, 1, 1, 1, 4, 5,
#define PPUTLIMPL_UDEC_28u   0x01Cu, 1, 1, 1, 1, 4, 5, 2, 2, 7
#define PPUTLIMPL_UDEC_27u   0x01Bu, 1, 1, 1, 1, 4, 5, 3, 3, 3
#define PPUTLIMPL_UDEC_26u   0x01Au, 1, 1, 1, 1, 4, 5, 2, 13
#define PPUTLIMPL_UDEC_25u   0x019u, 1, 1, 1, 1, 4, 5, 5, 5
#define PPUTLIMPL_UDEC_24u   0x018u, 1, 1, 1, 1, 4, 4, 2, 2, 2, 3
#define PPUTLIMPL_UDEC_23u   0x017u, 1, 1, 1, 1, 4, 4,
#define PPUTLIMPL_UDEC_22u   0x016u, 1, 1, 1, 1, 4, 4, 2, 11
#define PPUTLIMPL_UDEC_21u   0x015u, 1, 1, 1, 1, 4, 4, 3, 7
#define PPUTLIMPL_UDEC_20u   0x014u, 1, 1, 1, 1, 4, 4, 2, 2, 5
#define PPUTLIMPL_UDEC_19u   0x013u, 1, 1, 1, 1, 4, 4,
#define PPUTLIMPL_UDEC_18u   0x012u, 1, 1, 1, 1, 4, 4, 2, 3, 3
#define PPUTLIMPL_UDEC_17u   0x011u, 1, 1, 1, 1, 4, 4,
#define PPUTLIMPL_UDEC_16u   0x010u, 1, 1, 1, 1, 4, 4, 2, 2, 2, 2
#define PPUTLIMPL_UDEC_15u   0x00Fu, 1, 1, 1, 1, 3, 3, 3, 5
#define PPUTLIMPL_UDEC_14u   0x00Eu, 1, 1, 1, 1, 3, 3, 2, 7
#define PPUTLIMPL_UDEC_13u   0x00Du, 1, 1, 1, 1, 3, 3,
#define PPUTLIMPL_UDEC_12u   0x00Cu, 1, 1, 1, 1, 3, 3, 2, 2, 3
#define PPUTLIMPL_UDEC_11u   0x00Bu, 1, 1, 1, 1, 3, 3,
#define PPUTLIMPL_UDEC_10u   0x00Au, 1, 1, 1, 1, 3, 3, 2, 5
#define PPUTLIMPL_UDEC_9u    0x009u, 1, 1, 1, 1, 3, 3, 3, 3
#define PPUTLIMPL_UDEC_8u    0x008u, 1, 1, 1, 1, 3, 2, 2, 2, 2
#define PPUTLIMPL_UDEC_7u    0x007u, 1, 1, 1, 1, 2, 2,
#define PPUTLIMPL_UDEC_6u    0x006u, 1, 1, 1, 1, 2, 2, 2, 3
#define PPUTLIMPL_UDEC_5u    0x005u, 1, 1, 1, 1, 2, 2,
#define PPUTLIMPL_UDEC_4u    0x004u, 1, 1, 1, 1, 2, 2, 2, 2
#define PPUTLIMPL_UDEC_3u    0x003u, 1, 1, 1, 1, 1, 1,
#define PPUTLIMPL_UDEC_2u    0x002u, 1, 1, 1, 1, 1, 1,
#define PPUTLIMPL_UDEC_1u    0x001u, 1, 1, 1, 1, 0, 1,
#define PPUTLIMPL_UDEC_0u    0x000u, 1, 1, 1, 1, , 0,

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [impl.traits.uhex]
/// ------------------
/// [internal] uhex traits
#define PPUTLIMPL_UHEX(/* enum<...>, enum<UDEC|IHEX|ICAST|ILTZ|BNOT|HDUMP> */ v, t) \
  PPUTLIMPL_UHEX_o(t, PTL_XCAT(PPUTLIMPL_UHEX_, v))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_UHEX_o(t, ...)                 PPUTLIMPL_UHEX_##t(__VA_ARGS__)
#define PPUTLIMPL_UHEX_HDUMP(u, h, i, z, b, ...) /* -> enum<0|1|...|E|F>... */ __VA_ARGS__
#define PPUTLIMPL_UHEX_BNOT(u, h, i, z, b, ...)  /* -> uhex */ b
#define PPUTLIMPL_UHEX_ILTZ(u, h, i, z, ...)     /* -> bool */ z
#define PPUTLIMPL_UHEX_ICAST(u, h, i, ...)       /* -> idec|ihex */ i
#define PPUTLIMPL_UHEX_IHEX(u, h, ...)           /* -> ihex */ h
#define PPUTLIMPL_UHEX_UDEC(u, ...)              /* -> udec */ u

/// UDEC, IHEX, ICAST, ILTZ, BNOT, HDUMP...
#define PPUTLIMPL_UHEX_0xFFFu 4095u, 0xFFF, 0xFFF, 1, 0x000u, F, F, F
#define PPUTLIMPL_UHEX_0xFFEu 4094u, 0xFFE, 0xFFE, 1, 0x001u, F, F, E
#define PPUTLIMPL_UHEX_0xFFDu 4093u, 0xFFD, 0xFFD, 1, 0x002u, F, F, D
#define PPUTLIMPL_UHEX_0xFFCu 4092u, 0xFFC, 0xFFC, 1, 0x003u, F, F, C
#define PPUTLIMPL_UHEX_0xFFBu 4091u, 0xFFB, 0xFFB, 1, 0x004u, F, F, B
#define PPUTLIMPL_UHEX_0xFFAu 4090u, 0xFFA, 0xFFA, 1, 0x005u, F, F, A
#define PPUTLIMPL_UHEX_0xFF9u 4089u, 0xFF9, 0xFF9, 1, 0x006u, F, F, 9
#define PPUTLIMPL_UHEX_0xFF8u 4088u, 0xFF8, 0xFF8, 1, 0x007u, F, F, 8
#define PPUTLIMPL_UHEX_0xFF7u 4087u, 0xFF7, 0xFF7, 1, 0x008u, F, F, 7
#define PPUTLIMPL_UHEX_0xFF6u 4086u, 0xFF6, 0xFF6, 1, 0x009u, F, F, 6
#define PPUTLIMPL_UHEX_0xFF5u 4085u, 0xFF5, 0xFF5, 1, 0x00Au, F, F, 5
#define PPUTLIMPL_UHEX_0xFF4u 4084u, 0xFF4, 0xFF4, 1, 0x00Bu, F, F, 4
#define PPUTLIMPL_UHEX_0xFF3u 4083u, 0xFF3, 0xFF3, 1, 0x00Cu, F, F, 3
#define PPUTLIMPL_UHEX_0xFF2u 4082u, 0xFF2, 0xFF2, 1, 0x00Du, F, F, 2
#define PPUTLIMPL_UHEX_0xFF1u 4081u, 0xFF1, 0xFF1, 1, 0x00Eu, F, F, 1
#define PPUTLIMPL_UHEX_0xFF0u 4080u, 0xFF0, 0xFF0, 1, 0x00Fu, F, F, 0
#define PPUTLIMPL_UHEX_0xFEFu 4079u, 0xFEF, 0xFEF, 1, 0x010u, F, E, F
#define PPUTLIMPL_UHEX_0xFEEu 4078u, 0xFEE, 0xFEE, 1, 0x011u, F, E, E
#define PPUTLIMPL_UHEX_0xFEDu 4077u, 0xFED, 0xFED, 1, 0x012u, F, E, D
#define PPUTLIMPL_UHEX_0xFECu 4076u, 0xFEC, 0xFEC, 1, 0x013u, F, E, C
#define PPUTLIMPL_UHEX_0xFEBu 4075u, 0xFEB, 0xFEB, 1, 0x014u, F, E, B
#define PPUTLIMPL_UHEX_0xFEAu 4074u, 0xFEA, 0xFEA, 1, 0x015u, F, E, A
#define PPUTLIMPL_UHEX_0xFE9u 4073u, 0xFE9, 0xFE9, 1, 0x016u, F, E, 9
#define PPUTLIMPL_UHEX_0xFE8u 4072u, 0xFE8, 0xFE8, 1, 0x017u, F, E, 8
#define PPUTLIMPL_UHEX_0xFE7u 4071u, 0xFE7, 0xFE7, 1, 0x018u, F, E, 7
#define PPUTLIMPL_UHEX_0xFE6u 4070u, 0xFE6, 0xFE6, 1, 0x019u, F, E, 6
#define PPUTLIMPL_UHEX_0xFE5u 4069u, 0xFE5, 0xFE5, 1, 0x01Au, F, E, 5
#define PPUTLIMPL_UHEX_0xFE4u 4068u, 0xFE4, 0xFE4, 1, 0x01Bu, F, E, 4
#define PPUTLIMPL_UHEX_0xFE3u 4067u, 0xFE3, 0xFE3, 1, 0x01Cu, F, E, 3
#define PPUTLIMPL_UHEX_0xFE2u 4066u, 0xFE2, 0xFE2, 1, 0x01Du, F, E, 2
#define PPUTLIMPL_UHEX_0xFE1u 4065u, 0xFE1, 0xFE1, 1, 0x01Eu, F, E, 1
#define PPUTLIMPL_UHEX_0xFE0u 4064u, 0xFE0, 0xFE0, 1, 0x01Fu, F, E, 0
#define PPUTLIMPL_UHEX_0xFDFu 4063u, 0xFDF, 0xFDF, 1, 0x020u, F, D, F
#define PPUTLIMPL_UHEX_0xFDEu 4062u, 0xFDE, 0xFDE, 1, 0x021u, F, D, E
#define PPUTLIMPL_UHEX_0xFDDu 4061u, 0xFDD, 0xFDD, 1, 0x022u, F, D, D
#define PPUTLIMPL_UHEX_0xFDCu 4060u, 0xFDC, 0xFDC, 1, 0x023u, F, D, C
#define PPUTLIMPL_UHEX_0xFDBu 4059u, 0xFDB, 0xFDB, 1, 0x024u, F, D, B
#define PPUTLIMPL_UHEX_0xFDAu 4058u, 0xFDA, 0xFDA, 1, 0x025u, F, D, A
#define PPUTLIMPL_UHEX_0xFD9u 4057u, 0xFD9, 0xFD9, 1, 0x026u, F, D, 9
#define PPUTLIMPL_UHEX_0xFD8u 4056u, 0xFD8, 0xFD8, 1, 0x027u, F, D, 8
#define PPUTLIMPL_UHEX_0xFD7u 4055u, 0xFD7, 0xFD7, 1, 0x028u, F, D, 7
#define PPUTLIMPL_UHEX_0xFD6u 4054u, 0xFD6, 0xFD6, 1, 0x029u, F, D, 6
#define PPUTLIMPL_UHEX_0xFD5u 4053u, 0xFD5, 0xFD5, 1, 0x02Au, F, D, 5
#define PPUTLIMPL_UHEX_0xFD4u 4052u, 0xFD4, 0xFD4, 1, 0x02Bu, F, D, 4
#define PPUTLIMPL_UHEX_0xFD3u 4051u, 0xFD3, 0xFD3, 1, 0x02Cu, F, D, 3
#define PPUTLIMPL_UHEX_0xFD2u 4050u, 0xFD2, 0xFD2, 1, 0x02Du, F, D, 2
#define PPUTLIMPL_UHEX_0xFD1u 4049u, 0xFD1, 0xFD1, 1, 0x02Eu, F, D, 1
#define PPUTLIMPL_UHEX_0xFD0u 4048u, 0xFD0, 0xFD0, 1, 0x02Fu, F, D, 0
#define PPUTLIMPL_UHEX_0xFCFu 4047u, 0xFCF, 0xFCF, 1, 0x030u, F, C, F
#define PPUTLIMPL_UHEX_0xFCEu 4046u, 0xFCE, 0xFCE, 1, 0x031u, F, C, E
#define PPUTLIMPL_UHEX_0xFCDu 4045u, 0xFCD, 0xFCD, 1, 0x032u, F, C, D
#define PPUTLIMPL_UHEX_0xFCCu 4044u, 0xFCC, 0xFCC, 1, 0x033u, F, C, C
#define PPUTLIMPL_UHEX_0xFCBu 4043u, 0xFCB, 0xFCB, 1, 0x034u, F, C, B
#define PPUTLIMPL_UHEX_0xFCAu 4042u, 0xFCA, 0xFCA, 1, 0x035u, F, C, A
#define PPUTLIMPL_UHEX_0xFC9u 4041u, 0xFC9, 0xFC9, 1, 0x036u, F, C, 9
#define PPUTLIMPL_UHEX_0xFC8u 4040u, 0xFC8, 0xFC8, 1, 0x037u, F, C, 8
#define PPUTLIMPL_UHEX_0xFC7u 4039u, 0xFC7, 0xFC7, 1, 0x038u, F, C, 7
#define PPUTLIMPL_UHEX_0xFC6u 4038u, 0xFC6, 0xFC6, 1, 0x039u, F, C, 6
#define PPUTLIMPL_UHEX_0xFC5u 4037u, 0xFC5, 0xFC5, 1, 0x03Au, F, C, 5
#define PPUTLIMPL_UHEX_0xFC4u 4036u, 0xFC4, 0xFC4, 1, 0x03Bu, F, C, 4
#define PPUTLIMPL_UHEX_0xFC3u 4035u, 0xFC3, 0xFC3, 1, 0x03Cu, F, C, 3
#define PPUTLIMPL_UHEX_0xFC2u 4034u, 0xFC2, 0xFC2, 1, 0x03Du, F, C, 2
#define PPUTLIMPL_UHEX_0xFC1u 4033u, 0xFC1, 0xFC1, 1, 0x03Eu, F, C, 1
#define PPUTLIMPL_UHEX_0xFC0u 4032u, 0xFC0, 0xFC0, 1, 0x03Fu, F, C, 0
#define PPUTLIMPL_UHEX_0xFBFu 4031u, 0xFBF, 0xFBF, 1, 0x040u, F, B, F
#define PPUTLIMPL_UHEX_0xFBEu 4030u, 0xFBE, 0xFBE, 1, 0x041u, F, B, E
#define PPUTLIMPL_UHEX_0xFBDu 4029u, 0xFBD, 0xFBD, 1, 0x042u, F, B, D
#define PPUTLIMPL_UHEX_0xFBCu 4028u, 0xFBC, 0xFBC, 1, 0x043u, F, B, C
#define PPUTLIMPL_UHEX_0xFBBu 4027u, 0xFBB, 0xFBB, 1, 0x044u, F, B, B
#define PPUTLIMPL_UHEX_0xFBAu 4026u, 0xFBA, 0xFBA, 1, 0x045u, F, B, A
#define PPUTLIMPL_UHEX_0xFB9u 4025u, 0xFB9, 0xFB9, 1, 0x046u, F, B, 9
#define PPUTLIMPL_UHEX_0xFB8u 4024u, 0xFB8, 0xFB8, 1, 0x047u, F, B, 8
#define PPUTLIMPL_UHEX_0xFB7u 4023u, 0xFB7, 0xFB7, 1, 0x048u, F, B, 7
#define PPUTLIMPL_UHEX_0xFB6u 4022u, 0xFB6, 0xFB6, 1, 0x049u, F, B, 6
#define PPUTLIMPL_UHEX_0xFB5u 4021u, 0xFB5, 0xFB5, 1, 0x04Au, F, B, 5
#define PPUTLIMPL_UHEX_0xFB4u 4020u, 0xFB4, 0xFB4, 1, 0x04Bu, F, B, 4
#define PPUTLIMPL_UHEX_0xFB3u 4019u, 0xFB3, 0xFB3, 1, 0x04Cu, F, B, 3
#define PPUTLIMPL_UHEX_0xFB2u 4018u, 0xFB2, 0xFB2, 1, 0x04Du, F, B, 2
#define PPUTLIMPL_UHEX_0xFB1u 4017u, 0xFB1, 0xFB1, 1, 0x04Eu, F, B, 1
#define PPUTLIMPL_UHEX_0xFB0u 4016u, 0xFB0, 0xFB0, 1, 0x04Fu, F, B, 0
#define PPUTLIMPL_UHEX_0xFAFu 4015u, 0xFAF, 0xFAF, 1, 0x050u, F, A, F
#define PPUTLIMPL_UHEX_0xFAEu 4014u, 0xFAE, 0xFAE, 1, 0x051u, F, A, E
#define PPUTLIMPL_UHEX_0xFADu 4013u, 0xFAD, 0xFAD, 1, 0x052u, F, A, D
#define PPUTLIMPL_UHEX_0xFACu 4012u, 0xFAC, 0xFAC, 1, 0x053u, F, A, C
#define PPUTLIMPL_UHEX_0xFABu 4011u, 0xFAB, 0xFAB, 1, 0x054u, F, A, B
#define PPUTLIMPL_UHEX_0xFAAu 4010u, 0xFAA, 0xFAA, 1, 0x055u, F, A, A
#define PPUTLIMPL_UHEX_0xFA9u 4009u, 0xFA9, 0xFA9, 1, 0x056u, F, A, 9
#define PPUTLIMPL_UHEX_0xFA8u 4008u, 0xFA8, 0xFA8, 1, 0x057u, F, A, 8
#define PPUTLIMPL_UHEX_0xFA7u 4007u, 0xFA7, 0xFA7, 1, 0x058u, F, A, 7
#define PPUTLIMPL_UHEX_0xFA6u 4006u, 0xFA6, 0xFA6, 1, 0x059u, F, A, 6
#define PPUTLIMPL_UHEX_0xFA5u 4005u, 0xFA5, 0xFA5, 1, 0x05Au, F, A, 5
#define PPUTLIMPL_UHEX_0xFA4u 4004u, 0xFA4, 0xFA4, 1, 0x05Bu, F, A, 4
#define PPUTLIMPL_UHEX_0xFA3u 4003u, 0xFA3, 0xFA3, 1, 0x05Cu, F, A, 3
#define PPUTLIMPL_UHEX_0xFA2u 4002u, 0xFA2, 0xFA2, 1, 0x05Du, F, A, 2
#define PPUTLIMPL_UHEX_0xFA1u 4001u, 0xFA1, 0xFA1, 1, 0x05Eu, F, A, 1
#define PPUTLIMPL_UHEX_0xFA0u 4000u, 0xFA0, 0xFA0, 1, 0x05Fu, F, A, 0
#define PPUTLIMPL_UHEX_0xF9Fu 3999u, 0xF9F, 0xF9F, 1, 0x060u, F, 9, F
#define PPUTLIMPL_UHEX_0xF9Eu 3998u, 0xF9E, 0xF9E, 1, 0x061u, F, 9, E
#define PPUTLIMPL_UHEX_0xF9Du 3997u, 0xF9D, 0xF9D, 1, 0x062u, F, 9, D
#define PPUTLIMPL_UHEX_0xF9Cu 3996u, 0xF9C, 0xF9C, 1, 0x063u, F, 9, C
#define PPUTLIMPL_UHEX_0xF9Bu 3995u, 0xF9B, 0xF9B, 1, 0x064u, F, 9, B
#define PPUTLIMPL_UHEX_0xF9Au 3994u, 0xF9A, 0xF9A, 1, 0x065u, F, 9, A
#define PPUTLIMPL_UHEX_0xF99u 3993u, 0xF99, 0xF99, 1, 0x066u, F, 9, 9
#define PPUTLIMPL_UHEX_0xF98u 3992u, 0xF98, 0xF98, 1, 0x067u, F, 9, 8
#define PPUTLIMPL_UHEX_0xF97u 3991u, 0xF97, 0xF97, 1, 0x068u, F, 9, 7
#define PPUTLIMPL_UHEX_0xF96u 3990u, 0xF96, 0xF96, 1, 0x069u, F, 9, 6
#define PPUTLIMPL_UHEX_0xF95u 3989u, 0xF95, 0xF95, 1, 0x06Au, F, 9, 5
#define PPUTLIMPL_UHEX_0xF94u 3988u, 0xF94, 0xF94, 1, 0x06Bu, F, 9, 4
#define PPUTLIMPL_UHEX_0xF93u 3987u, 0xF93, 0xF93, 1, 0x06Cu, F, 9, 3
#define PPUTLIMPL_UHEX_0xF92u 3986u, 0xF92, 0xF92, 1, 0x06Du, F, 9, 2
#define PPUTLIMPL_UHEX_0xF91u 3985u, 0xF91, 0xF91, 1, 0x06Eu, F, 9, 1
#define PPUTLIMPL_UHEX_0xF90u 3984u, 0xF90, 0xF90, 1, 0x06Fu, F, 9, 0
#define PPUTLIMPL_UHEX_0xF8Fu 3983u, 0xF8F, 0xF8F, 1, 0x070u, F, 8, F
#define PPUTLIMPL_UHEX_0xF8Eu 3982u, 0xF8E, 0xF8E, 1, 0x071u, F, 8, E
#define PPUTLIMPL_UHEX_0xF8Du 3981u, 0xF8D, 0xF8D, 1, 0x072u, F, 8, D
#define PPUTLIMPL_UHEX_0xF8Cu 3980u, 0xF8C, 0xF8C, 1, 0x073u, F, 8, C
#define PPUTLIMPL_UHEX_0xF8Bu 3979u, 0xF8B, 0xF8B, 1, 0x074u, F, 8, B
#define PPUTLIMPL_UHEX_0xF8Au 3978u, 0xF8A, 0xF8A, 1, 0x075u, F, 8, A
#define PPUTLIMPL_UHEX_0xF89u 3977u, 0xF89, 0xF89, 1, 0x076u, F, 8, 9
#define PPUTLIMPL_UHEX_0xF88u 3976u, 0xF88, 0xF88, 1, 0x077u, F, 8, 8
#define PPUTLIMPL_UHEX_0xF87u 3975u, 0xF87, 0xF87, 1, 0x078u, F, 8, 7
#define PPUTLIMPL_UHEX_0xF86u 3974u, 0xF86, 0xF86, 1, 0x079u, F, 8, 6
#define PPUTLIMPL_UHEX_0xF85u 3973u, 0xF85, 0xF85, 1, 0x07Au, F, 8, 5
#define PPUTLIMPL_UHEX_0xF84u 3972u, 0xF84, 0xF84, 1, 0x07Bu, F, 8, 4
#define PPUTLIMPL_UHEX_0xF83u 3971u, 0xF83, 0xF83, 1, 0x07Cu, F, 8, 3
#define PPUTLIMPL_UHEX_0xF82u 3970u, 0xF82, 0xF82, 1, 0x07Du, F, 8, 2
#define PPUTLIMPL_UHEX_0xF81u 3969u, 0xF81, 0xF81, 1, 0x07Eu, F, 8, 1
#define PPUTLIMPL_UHEX_0xF80u 3968u, 0xF80, 0xF80, 1, 0x07Fu, F, 8, 0
#define PPUTLIMPL_UHEX_0xF7Fu 3967u, 0xF7F, 0xF7F, 1, 0x080u, F, 7, F
#define PPUTLIMPL_UHEX_0xF7Eu 3966u, 0xF7E, 0xF7E, 1, 0x081u, F, 7, E
#define PPUTLIMPL_UHEX_0xF7Du 3965u, 0xF7D, 0xF7D, 1, 0x082u, F, 7, D
#define PPUTLIMPL_UHEX_0xF7Cu 3964u, 0xF7C, 0xF7C, 1, 0x083u, F, 7, C
#define PPUTLIMPL_UHEX_0xF7Bu 3963u, 0xF7B, 0xF7B, 1, 0x084u, F, 7, B
#define PPUTLIMPL_UHEX_0xF7Au 3962u, 0xF7A, 0xF7A, 1, 0x085u, F, 7, A
#define PPUTLIMPL_UHEX_0xF79u 3961u, 0xF79, 0xF79, 1, 0x086u, F, 7, 9
#define PPUTLIMPL_UHEX_0xF78u 3960u, 0xF78, 0xF78, 1, 0x087u, F, 7, 8
#define PPUTLIMPL_UHEX_0xF77u 3959u, 0xF77, 0xF77, 1, 0x088u, F, 7, 7
#define PPUTLIMPL_UHEX_0xF76u 3958u, 0xF76, 0xF76, 1, 0x089u, F, 7, 6
#define PPUTLIMPL_UHEX_0xF75u 3957u, 0xF75, 0xF75, 1, 0x08Au, F, 7, 5
#define PPUTLIMPL_UHEX_0xF74u 3956u, 0xF74, 0xF74, 1, 0x08Bu, F, 7, 4
#define PPUTLIMPL_UHEX_0xF73u 3955u, 0xF73, 0xF73, 1, 0x08Cu, F, 7, 3
#define PPUTLIMPL_UHEX_0xF72u 3954u, 0xF72, 0xF72, 1, 0x08Du, F, 7, 2
#define PPUTLIMPL_UHEX_0xF71u 3953u, 0xF71, 0xF71, 1, 0x08Eu, F, 7, 1
#define PPUTLIMPL_UHEX_0xF70u 3952u, 0xF70, 0xF70, 1, 0x08Fu, F, 7, 0
#define PPUTLIMPL_UHEX_0xF6Fu 3951u, 0xF6F, 0xF6F, 1, 0x090u, F, 6, F
#define PPUTLIMPL_UHEX_0xF6Eu 3950u, 0xF6E, 0xF6E, 1, 0x091u, F, 6, E
#define PPUTLIMPL_UHEX_0xF6Du 3949u, 0xF6D, 0xF6D, 1, 0x092u, F, 6, D
#define PPUTLIMPL_UHEX_0xF6Cu 3948u, 0xF6C, 0xF6C, 1, 0x093u, F, 6, C
#define PPUTLIMPL_UHEX_0xF6Bu 3947u, 0xF6B, 0xF6B, 1, 0x094u, F, 6, B
#define PPUTLIMPL_UHEX_0xF6Au 3946u, 0xF6A, 0xF6A, 1, 0x095u, F, 6, A
#define PPUTLIMPL_UHEX_0xF69u 3945u, 0xF69, 0xF69, 1, 0x096u, F, 6, 9
#define PPUTLIMPL_UHEX_0xF68u 3944u, 0xF68, 0xF68, 1, 0x097u, F, 6, 8
#define PPUTLIMPL_UHEX_0xF67u 3943u, 0xF67, 0xF67, 1, 0x098u, F, 6, 7
#define PPUTLIMPL_UHEX_0xF66u 3942u, 0xF66, 0xF66, 1, 0x099u, F, 6, 6
#define PPUTLIMPL_UHEX_0xF65u 3941u, 0xF65, 0xF65, 1, 0x09Au, F, 6, 5
#define PPUTLIMPL_UHEX_0xF64u 3940u, 0xF64, 0xF64, 1, 0x09Bu, F, 6, 4
#define PPUTLIMPL_UHEX_0xF63u 3939u, 0xF63, 0xF63, 1, 0x09Cu, F, 6, 3
#define PPUTLIMPL_UHEX_0xF62u 3938u, 0xF62, 0xF62, 1, 0x09Du, F, 6, 2
#define PPUTLIMPL_UHEX_0xF61u 3937u, 0xF61, 0xF61, 1, 0x09Eu, F, 6, 1
#define PPUTLIMPL_UHEX_0xF60u 3936u, 0xF60, 0xF60, 1, 0x09Fu, F, 6, 0
#define PPUTLIMPL_UHEX_0xF5Fu 3935u, 0xF5F, 0xF5F, 1, 0x0A0u, F, 5, F
#define PPUTLIMPL_UHEX_0xF5Eu 3934u, 0xF5E, 0xF5E, 1, 0x0A1u, F, 5, E
#define PPUTLIMPL_UHEX_0xF5Du 3933u, 0xF5D, 0xF5D, 1, 0x0A2u, F, 5, D
#define PPUTLIMPL_UHEX_0xF5Cu 3932u, 0xF5C, 0xF5C, 1, 0x0A3u, F, 5, C
#define PPUTLIMPL_UHEX_0xF5Bu 3931u, 0xF5B, 0xF5B, 1, 0x0A4u, F, 5, B
#define PPUTLIMPL_UHEX_0xF5Au 3930u, 0xF5A, 0xF5A, 1, 0x0A5u, F, 5, A
#define PPUTLIMPL_UHEX_0xF59u 3929u, 0xF59, 0xF59, 1, 0x0A6u, F, 5, 9
#define PPUTLIMPL_UHEX_0xF58u 3928u, 0xF58, 0xF58, 1, 0x0A7u, F, 5, 8
#define PPUTLIMPL_UHEX_0xF57u 3927u, 0xF57, 0xF57, 1, 0x0A8u, F, 5, 7
#define PPUTLIMPL_UHEX_0xF56u 3926u, 0xF56, 0xF56, 1, 0x0A9u, F, 5, 6
#define PPUTLIMPL_UHEX_0xF55u 3925u, 0xF55, 0xF55, 1, 0x0AAu, F, 5, 5
#define PPUTLIMPL_UHEX_0xF54u 3924u, 0xF54, 0xF54, 1, 0x0ABu, F, 5, 4
#define PPUTLIMPL_UHEX_0xF53u 3923u, 0xF53, 0xF53, 1, 0x0ACu, F, 5, 3
#define PPUTLIMPL_UHEX_0xF52u 3922u, 0xF52, 0xF52, 1, 0x0ADu, F, 5, 2
#define PPUTLIMPL_UHEX_0xF51u 3921u, 0xF51, 0xF51, 1, 0x0AEu, F, 5, 1
#define PPUTLIMPL_UHEX_0xF50u 3920u, 0xF50, 0xF50, 1, 0x0AFu, F, 5, 0
#define PPUTLIMPL_UHEX_0xF4Fu 3919u, 0xF4F, 0xF4F, 1, 0x0B0u, F, 4, F
#define PPUTLIMPL_UHEX_0xF4Eu 3918u, 0xF4E, 0xF4E, 1, 0x0B1u, F, 4, E
#define PPUTLIMPL_UHEX_0xF4Du 3917u, 0xF4D, 0xF4D, 1, 0x0B2u, F, 4, D
#define PPUTLIMPL_UHEX_0xF4Cu 3916u, 0xF4C, 0xF4C, 1, 0x0B3u, F, 4, C
#define PPUTLIMPL_UHEX_0xF4Bu 3915u, 0xF4B, 0xF4B, 1, 0x0B4u, F, 4, B
#define PPUTLIMPL_UHEX_0xF4Au 3914u, 0xF4A, 0xF4A, 1, 0x0B5u, F, 4, A
#define PPUTLIMPL_UHEX_0xF49u 3913u, 0xF49, 0xF49, 1, 0x0B6u, F, 4, 9
#define PPUTLIMPL_UHEX_0xF48u 3912u, 0xF48, 0xF48, 1, 0x0B7u, F, 4, 8
#define PPUTLIMPL_UHEX_0xF47u 3911u, 0xF47, 0xF47, 1, 0x0B8u, F, 4, 7
#define PPUTLIMPL_UHEX_0xF46u 3910u, 0xF46, 0xF46, 1, 0x0B9u, F, 4, 6
#define PPUTLIMPL_UHEX_0xF45u 3909u, 0xF45, 0xF45, 1, 0x0BAu, F, 4, 5
#define PPUTLIMPL_UHEX_0xF44u 3908u, 0xF44, 0xF44, 1, 0x0BBu, F, 4, 4
#define PPUTLIMPL_UHEX_0xF43u 3907u, 0xF43, 0xF43, 1, 0x0BCu, F, 4, 3
#define PPUTLIMPL_UHEX_0xF42u 3906u, 0xF42, 0xF42, 1, 0x0BDu, F, 4, 2
#define PPUTLIMPL_UHEX_0xF41u 3905u, 0xF41, 0xF41, 1, 0x0BEu, F, 4, 1
#define PPUTLIMPL_UHEX_0xF40u 3904u, 0xF40, 0xF40, 1, 0x0BFu, F, 4, 0
#define PPUTLIMPL_UHEX_0xF3Fu 3903u, 0xF3F, 0xF3F, 1, 0x0C0u, F, 3, F
#define PPUTLIMPL_UHEX_0xF3Eu 3902u, 0xF3E, 0xF3E, 1, 0x0C1u, F, 3, E
#define PPUTLIMPL_UHEX_0xF3Du 3901u, 0xF3D, 0xF3D, 1, 0x0C2u, F, 3, D
#define PPUTLIMPL_UHEX_0xF3Cu 3900u, 0xF3C, 0xF3C, 1, 0x0C3u, F, 3, C
#define PPUTLIMPL_UHEX_0xF3Bu 3899u, 0xF3B, 0xF3B, 1, 0x0C4u, F, 3, B
#define PPUTLIMPL_UHEX_0xF3Au 3898u, 0xF3A, 0xF3A, 1, 0x0C5u, F, 3, A
#define PPUTLIMPL_UHEX_0xF39u 3897u, 0xF39, 0xF39, 1, 0x0C6u, F, 3, 9
#define PPUTLIMPL_UHEX_0xF38u 3896u, 0xF38, 0xF38, 1, 0x0C7u, F, 3, 8
#define PPUTLIMPL_UHEX_0xF37u 3895u, 0xF37, 0xF37, 1, 0x0C8u, F, 3, 7
#define PPUTLIMPL_UHEX_0xF36u 3894u, 0xF36, 0xF36, 1, 0x0C9u, F, 3, 6
#define PPUTLIMPL_UHEX_0xF35u 3893u, 0xF35, 0xF35, 1, 0x0CAu, F, 3, 5
#define PPUTLIMPL_UHEX_0xF34u 3892u, 0xF34, 0xF34, 1, 0x0CBu, F, 3, 4
#define PPUTLIMPL_UHEX_0xF33u 3891u, 0xF33, 0xF33, 1, 0x0CCu, F, 3, 3
#define PPUTLIMPL_UHEX_0xF32u 3890u, 0xF32, 0xF32, 1, 0x0CDu, F, 3, 2
#define PPUTLIMPL_UHEX_0xF31u 3889u, 0xF31, 0xF31, 1, 0x0CEu, F, 3, 1
#define PPUTLIMPL_UHEX_0xF30u 3888u, 0xF30, 0xF30, 1, 0x0CFu, F, 3, 0
#define PPUTLIMPL_UHEX_0xF2Fu 3887u, 0xF2F, 0xF2F, 1, 0x0D0u, F, 2, F
#define PPUTLIMPL_UHEX_0xF2Eu 3886u, 0xF2E, 0xF2E, 1, 0x0D1u, F, 2, E
#define PPUTLIMPL_UHEX_0xF2Du 3885u, 0xF2D, 0xF2D, 1, 0x0D2u, F, 2, D
#define PPUTLIMPL_UHEX_0xF2Cu 3884u, 0xF2C, 0xF2C, 1, 0x0D3u, F, 2, C
#define PPUTLIMPL_UHEX_0xF2Bu 3883u, 0xF2B, 0xF2B, 1, 0x0D4u, F, 2, B
#define PPUTLIMPL_UHEX_0xF2Au 3882u, 0xF2A, 0xF2A, 1, 0x0D5u, F, 2, A
#define PPUTLIMPL_UHEX_0xF29u 3881u, 0xF29, 0xF29, 1, 0x0D6u, F, 2, 9
#define PPUTLIMPL_UHEX_0xF28u 3880u, 0xF28, 0xF28, 1, 0x0D7u, F, 2, 8
#define PPUTLIMPL_UHEX_0xF27u 3879u, 0xF27, 0xF27, 1, 0x0D8u, F, 2, 7
#define PPUTLIMPL_UHEX_0xF26u 3878u, 0xF26, 0xF26, 1, 0x0D9u, F, 2, 6
#define PPUTLIMPL_UHEX_0xF25u 3877u, 0xF25, 0xF25, 1, 0x0DAu, F, 2, 5
#define PPUTLIMPL_UHEX_0xF24u 3876u, 0xF24, 0xF24, 1, 0x0DBu, F, 2, 4
#define PPUTLIMPL_UHEX_0xF23u 3875u, 0xF23, 0xF23, 1, 0x0DCu, F, 2, 3
#define PPUTLIMPL_UHEX_0xF22u 3874u, 0xF22, 0xF22, 1, 0x0DDu, F, 2, 2
#define PPUTLIMPL_UHEX_0xF21u 3873u, 0xF21, 0xF21, 1, 0x0DEu, F, 2, 1
#define PPUTLIMPL_UHEX_0xF20u 3872u, 0xF20, 0xF20, 1, 0x0DFu, F, 2, 0
#define PPUTLIMPL_UHEX_0xF1Fu 3871u, 0xF1F, 0xF1F, 1, 0x0E0u, F, 1, F
#define PPUTLIMPL_UHEX_0xF1Eu 3870u, 0xF1E, 0xF1E, 1, 0x0E1u, F, 1, E
#define PPUTLIMPL_UHEX_0xF1Du 3869u, 0xF1D, 0xF1D, 1, 0x0E2u, F, 1, D
#define PPUTLIMPL_UHEX_0xF1Cu 3868u, 0xF1C, 0xF1C, 1, 0x0E3u, F, 1, C
#define PPUTLIMPL_UHEX_0xF1Bu 3867u, 0xF1B, 0xF1B, 1, 0x0E4u, F, 1, B
#define PPUTLIMPL_UHEX_0xF1Au 3866u, 0xF1A, 0xF1A, 1, 0x0E5u, F, 1, A
#define PPUTLIMPL_UHEX_0xF19u 3865u, 0xF19, 0xF19, 1, 0x0E6u, F, 1, 9
#define PPUTLIMPL_UHEX_0xF18u 3864u, 0xF18, 0xF18, 1, 0x0E7u, F, 1, 8
#define PPUTLIMPL_UHEX_0xF17u 3863u, 0xF17, 0xF17, 1, 0x0E8u, F, 1, 7
#define PPUTLIMPL_UHEX_0xF16u 3862u, 0xF16, 0xF16, 1, 0x0E9u, F, 1, 6
#define PPUTLIMPL_UHEX_0xF15u 3861u, 0xF15, 0xF15, 1, 0x0EAu, F, 1, 5
#define PPUTLIMPL_UHEX_0xF14u 3860u, 0xF14, 0xF14, 1, 0x0EBu, F, 1, 4
#define PPUTLIMPL_UHEX_0xF13u 3859u, 0xF13, 0xF13, 1, 0x0ECu, F, 1, 3
#define PPUTLIMPL_UHEX_0xF12u 3858u, 0xF12, 0xF12, 1, 0x0EDu, F, 1, 2
#define PPUTLIMPL_UHEX_0xF11u 3857u, 0xF11, 0xF11, 1, 0x0EEu, F, 1, 1
#define PPUTLIMPL_UHEX_0xF10u 3856u, 0xF10, 0xF10, 1, 0x0EFu, F, 1, 0
#define PPUTLIMPL_UHEX_0xF0Fu 3855u, 0xF0F, 0xF0F, 1, 0x0F0u, F, 0, F
#define PPUTLIMPL_UHEX_0xF0Eu 3854u, 0xF0E, 0xF0E, 1, 0x0F1u, F, 0, E
#define PPUTLIMPL_UHEX_0xF0Du 3853u, 0xF0D, 0xF0D, 1, 0x0F2u, F, 0, D
#define PPUTLIMPL_UHEX_0xF0Cu 3852u, 0xF0C, 0xF0C, 1, 0x0F3u, F, 0, C
#define PPUTLIMPL_UHEX_0xF0Bu 3851u, 0xF0B, 0xF0B, 1, 0x0F4u, F, 0, B
#define PPUTLIMPL_UHEX_0xF0Au 3850u, 0xF0A, 0xF0A, 1, 0x0F5u, F, 0, A
#define PPUTLIMPL_UHEX_0xF09u 3849u, 0xF09, 0xF09, 1, 0x0F6u, F, 0, 9
#define PPUTLIMPL_UHEX_0xF08u 3848u, 0xF08, 0xF08, 1, 0x0F7u, F, 0, 8
#define PPUTLIMPL_UHEX_0xF07u 3847u, 0xF07, 0xF07, 1, 0x0F8u, F, 0, 7
#define PPUTLIMPL_UHEX_0xF06u 3846u, 0xF06, 0xF06, 1, 0x0F9u, F, 0, 6
#define PPUTLIMPL_UHEX_0xF05u 3845u, 0xF05, 0xF05, 1, 0x0FAu, F, 0, 5
#define PPUTLIMPL_UHEX_0xF04u 3844u, 0xF04, 0xF04, 1, 0x0FBu, F, 0, 4
#define PPUTLIMPL_UHEX_0xF03u 3843u, 0xF03, 0xF03, 1, 0x0FCu, F, 0, 3
#define PPUTLIMPL_UHEX_0xF02u 3842u, 0xF02, 0xF02, 1, 0x0FDu, F, 0, 2
#define PPUTLIMPL_UHEX_0xF01u 3841u, 0xF01, 0xF01, 1, 0x0FEu, F, 0, 1
#define PPUTLIMPL_UHEX_0xF00u 3840u, 0xF00, 0xF00, 1, 0x0FFu, F, 0, 0
#define PPUTLIMPL_UHEX_0xEFFu 3839u, 0xEFF, 0xEFF, 1, 0x100u, E, F, F
#define PPUTLIMPL_UHEX_0xEFEu 3838u, 0xEFE, 0xEFE, 1, 0x101u, E, F, E
#define PPUTLIMPL_UHEX_0xEFDu 3837u, 0xEFD, 0xEFD, 1, 0x102u, E, F, D
#define PPUTLIMPL_UHEX_0xEFCu 3836u, 0xEFC, 0xEFC, 1, 0x103u, E, F, C
#define PPUTLIMPL_UHEX_0xEFBu 3835u, 0xEFB, 0xEFB, 1, 0x104u, E, F, B
#define PPUTLIMPL_UHEX_0xEFAu 3834u, 0xEFA, 0xEFA, 1, 0x105u, E, F, A
#define PPUTLIMPL_UHEX_0xEF9u 3833u, 0xEF9, 0xEF9, 1, 0x106u, E, F, 9
#define PPUTLIMPL_UHEX_0xEF8u 3832u, 0xEF8, 0xEF8, 1, 0x107u, E, F, 8
#define PPUTLIMPL_UHEX_0xEF7u 3831u, 0xEF7, 0xEF7, 1, 0x108u, E, F, 7
#define PPUTLIMPL_UHEX_0xEF6u 3830u, 0xEF6, 0xEF6, 1, 0x109u, E, F, 6
#define PPUTLIMPL_UHEX_0xEF5u 3829u, 0xEF5, 0xEF5, 1, 0x10Au, E, F, 5
#define PPUTLIMPL_UHEX_0xEF4u 3828u, 0xEF4, 0xEF4, 1, 0x10Bu, E, F, 4
#define PPUTLIMPL_UHEX_0xEF3u 3827u, 0xEF3, 0xEF3, 1, 0x10Cu, E, F, 3
#define PPUTLIMPL_UHEX_0xEF2u 3826u, 0xEF2, 0xEF2, 1, 0x10Du, E, F, 2
#define PPUTLIMPL_UHEX_0xEF1u 3825u, 0xEF1, 0xEF1, 1, 0x10Eu, E, F, 1
#define PPUTLIMPL_UHEX_0xEF0u 3824u, 0xEF0, 0xEF0, 1, 0x10Fu, E, F, 0
#define PPUTLIMPL_UHEX_0xEEFu 3823u, 0xEEF, 0xEEF, 1, 0x110u, E, E, F
#define PPUTLIMPL_UHEX_0xEEEu 3822u, 0xEEE, 0xEEE, 1, 0x111u, E, E, E
#define PPUTLIMPL_UHEX_0xEEDu 3821u, 0xEED, 0xEED, 1, 0x112u, E, E, D
#define PPUTLIMPL_UHEX_0xEECu 3820u, 0xEEC, 0xEEC, 1, 0x113u, E, E, C
#define PPUTLIMPL_UHEX_0xEEBu 3819u, 0xEEB, 0xEEB, 1, 0x114u, E, E, B
#define PPUTLIMPL_UHEX_0xEEAu 3818u, 0xEEA, 0xEEA, 1, 0x115u, E, E, A
#define PPUTLIMPL_UHEX_0xEE9u 3817u, 0xEE9, 0xEE9, 1, 0x116u, E, E, 9
#define PPUTLIMPL_UHEX_0xEE8u 3816u, 0xEE8, 0xEE8, 1, 0x117u, E, E, 8
#define PPUTLIMPL_UHEX_0xEE7u 3815u, 0xEE7, 0xEE7, 1, 0x118u, E, E, 7
#define PPUTLIMPL_UHEX_0xEE6u 3814u, 0xEE6, 0xEE6, 1, 0x119u, E, E, 6
#define PPUTLIMPL_UHEX_0xEE5u 3813u, 0xEE5, 0xEE5, 1, 0x11Au, E, E, 5
#define PPUTLIMPL_UHEX_0xEE4u 3812u, 0xEE4, 0xEE4, 1, 0x11Bu, E, E, 4
#define PPUTLIMPL_UHEX_0xEE3u 3811u, 0xEE3, 0xEE3, 1, 0x11Cu, E, E, 3
#define PPUTLIMPL_UHEX_0xEE2u 3810u, 0xEE2, 0xEE2, 1, 0x11Du, E, E, 2
#define PPUTLIMPL_UHEX_0xEE1u 3809u, 0xEE1, 0xEE1, 1, 0x11Eu, E, E, 1
#define PPUTLIMPL_UHEX_0xEE0u 3808u, 0xEE0, 0xEE0, 1, 0x11Fu, E, E, 0
#define PPUTLIMPL_UHEX_0xEDFu 3807u, 0xEDF, 0xEDF, 1, 0x120u, E, D, F
#define PPUTLIMPL_UHEX_0xEDEu 3806u, 0xEDE, 0xEDE, 1, 0x121u, E, D, E
#define PPUTLIMPL_UHEX_0xEDDu 3805u, 0xEDD, 0xEDD, 1, 0x122u, E, D, D
#define PPUTLIMPL_UHEX_0xEDCu 3804u, 0xEDC, 0xEDC, 1, 0x123u, E, D, C
#define PPUTLIMPL_UHEX_0xEDBu 3803u, 0xEDB, 0xEDB, 1, 0x124u, E, D, B
#define PPUTLIMPL_UHEX_0xEDAu 3802u, 0xEDA, 0xEDA, 1, 0x125u, E, D, A
#define PPUTLIMPL_UHEX_0xED9u 3801u, 0xED9, 0xED9, 1, 0x126u, E, D, 9
#define PPUTLIMPL_UHEX_0xED8u 3800u, 0xED8, 0xED8, 1, 0x127u, E, D, 8
#define PPUTLIMPL_UHEX_0xED7u 3799u, 0xED7, 0xED7, 1, 0x128u, E, D, 7
#define PPUTLIMPL_UHEX_0xED6u 3798u, 0xED6, 0xED6, 1, 0x129u, E, D, 6
#define PPUTLIMPL_UHEX_0xED5u 3797u, 0xED5, 0xED5, 1, 0x12Au, E, D, 5
#define PPUTLIMPL_UHEX_0xED4u 3796u, 0xED4, 0xED4, 1, 0x12Bu, E, D, 4
#define PPUTLIMPL_UHEX_0xED3u 3795u, 0xED3, 0xED3, 1, 0x12Cu, E, D, 3
#define PPUTLIMPL_UHEX_0xED2u 3794u, 0xED2, 0xED2, 1, 0x12Du, E, D, 2
#define PPUTLIMPL_UHEX_0xED1u 3793u, 0xED1, 0xED1, 1, 0x12Eu, E, D, 1
#define PPUTLIMPL_UHEX_0xED0u 3792u, 0xED0, 0xED0, 1, 0x12Fu, E, D, 0
#define PPUTLIMPL_UHEX_0xECFu 3791u, 0xECF, 0xECF, 1, 0x130u, E, C, F
#define PPUTLIMPL_UHEX_0xECEu 3790u, 0xECE, 0xECE, 1, 0x131u, E, C, E
#define PPUTLIMPL_UHEX_0xECDu 3789u, 0xECD, 0xECD, 1, 0x132u, E, C, D
#define PPUTLIMPL_UHEX_0xECCu 3788u, 0xECC, 0xECC, 1, 0x133u, E, C, C
#define PPUTLIMPL_UHEX_0xECBu 3787u, 0xECB, 0xECB, 1, 0x134u, E, C, B
#define PPUTLIMPL_UHEX_0xECAu 3786u, 0xECA, 0xECA, 1, 0x135u, E, C, A
#define PPUTLIMPL_UHEX_0xEC9u 3785u, 0xEC9, 0xEC9, 1, 0x136u, E, C, 9
#define PPUTLIMPL_UHEX_0xEC8u 3784u, 0xEC8, 0xEC8, 1, 0x137u, E, C, 8
#define PPUTLIMPL_UHEX_0xEC7u 3783u, 0xEC7, 0xEC7, 1, 0x138u, E, C, 7
#define PPUTLIMPL_UHEX_0xEC6u 3782u, 0xEC6, 0xEC6, 1, 0x139u, E, C, 6
#define PPUTLIMPL_UHEX_0xEC5u 3781u, 0xEC5, 0xEC5, 1, 0x13Au, E, C, 5
#define PPUTLIMPL_UHEX_0xEC4u 3780u, 0xEC4, 0xEC4, 1, 0x13Bu, E, C, 4
#define PPUTLIMPL_UHEX_0xEC3u 3779u, 0xEC3, 0xEC3, 1, 0x13Cu, E, C, 3
#define PPUTLIMPL_UHEX_0xEC2u 3778u, 0xEC2, 0xEC2, 1, 0x13Du, E, C, 2
#define PPUTLIMPL_UHEX_0xEC1u 3777u, 0xEC1, 0xEC1, 1, 0x13Eu, E, C, 1
#define PPUTLIMPL_UHEX_0xEC0u 3776u, 0xEC0, 0xEC0, 1, 0x13Fu, E, C, 0
#define PPUTLIMPL_UHEX_0xEBFu 3775u, 0xEBF, 0xEBF, 1, 0x140u, E, B, F
#define PPUTLIMPL_UHEX_0xEBEu 3774u, 0xEBE, 0xEBE, 1, 0x141u, E, B, E
#define PPUTLIMPL_UHEX_0xEBDu 3773u, 0xEBD, 0xEBD, 1, 0x142u, E, B, D
#define PPUTLIMPL_UHEX_0xEBCu 3772u, 0xEBC, 0xEBC, 1, 0x143u, E, B, C
#define PPUTLIMPL_UHEX_0xEBBu 3771u, 0xEBB, 0xEBB, 1, 0x144u, E, B, B
#define PPUTLIMPL_UHEX_0xEBAu 3770u, 0xEBA, 0xEBA, 1, 0x145u, E, B, A
#define PPUTLIMPL_UHEX_0xEB9u 3769u, 0xEB9, 0xEB9, 1, 0x146u, E, B, 9
#define PPUTLIMPL_UHEX_0xEB8u 3768u, 0xEB8, 0xEB8, 1, 0x147u, E, B, 8
#define PPUTLIMPL_UHEX_0xEB7u 3767u, 0xEB7, 0xEB7, 1, 0x148u, E, B, 7
#define PPUTLIMPL_UHEX_0xEB6u 3766u, 0xEB6, 0xEB6, 1, 0x149u, E, B, 6
#define PPUTLIMPL_UHEX_0xEB5u 3765u, 0xEB5, 0xEB5, 1, 0x14Au, E, B, 5
#define PPUTLIMPL_UHEX_0xEB4u 3764u, 0xEB4, 0xEB4, 1, 0x14Bu, E, B, 4
#define PPUTLIMPL_UHEX_0xEB3u 3763u, 0xEB3, 0xEB3, 1, 0x14Cu, E, B, 3
#define PPUTLIMPL_UHEX_0xEB2u 3762u, 0xEB2, 0xEB2, 1, 0x14Du, E, B, 2
#define PPUTLIMPL_UHEX_0xEB1u 3761u, 0xEB1, 0xEB1, 1, 0x14Eu, E, B, 1
#define PPUTLIMPL_UHEX_0xEB0u 3760u, 0xEB0, 0xEB0, 1, 0x14Fu, E, B, 0
#define PPUTLIMPL_UHEX_0xEAFu 3759u, 0xEAF, 0xEAF, 1, 0x150u, E, A, F
#define PPUTLIMPL_UHEX_0xEAEu 3758u, 0xEAE, 0xEAE, 1, 0x151u, E, A, E
#define PPUTLIMPL_UHEX_0xEADu 3757u, 0xEAD, 0xEAD, 1, 0x152u, E, A, D
#define PPUTLIMPL_UHEX_0xEACu 3756u, 0xEAC, 0xEAC, 1, 0x153u, E, A, C
#define PPUTLIMPL_UHEX_0xEABu 3755u, 0xEAB, 0xEAB, 1, 0x154u, E, A, B
#define PPUTLIMPL_UHEX_0xEAAu 3754u, 0xEAA, 0xEAA, 1, 0x155u, E, A, A
#define PPUTLIMPL_UHEX_0xEA9u 3753u, 0xEA9, 0xEA9, 1, 0x156u, E, A, 9
#define PPUTLIMPL_UHEX_0xEA8u 3752u, 0xEA8, 0xEA8, 1, 0x157u, E, A, 8
#define PPUTLIMPL_UHEX_0xEA7u 3751u, 0xEA7, 0xEA7, 1, 0x158u, E, A, 7
#define PPUTLIMPL_UHEX_0xEA6u 3750u, 0xEA6, 0xEA6, 1, 0x159u, E, A, 6
#define PPUTLIMPL_UHEX_0xEA5u 3749u, 0xEA5, 0xEA5, 1, 0x15Au, E, A, 5
#define PPUTLIMPL_UHEX_0xEA4u 3748u, 0xEA4, 0xEA4, 1, 0x15Bu, E, A, 4
#define PPUTLIMPL_UHEX_0xEA3u 3747u, 0xEA3, 0xEA3, 1, 0x15Cu, E, A, 3
#define PPUTLIMPL_UHEX_0xEA2u 3746u, 0xEA2, 0xEA2, 1, 0x15Du, E, A, 2
#define PPUTLIMPL_UHEX_0xEA1u 3745u, 0xEA1, 0xEA1, 1, 0x15Eu, E, A, 1
#define PPUTLIMPL_UHEX_0xEA0u 3744u, 0xEA0, 0xEA0, 1, 0x15Fu, E, A, 0
#define PPUTLIMPL_UHEX_0xE9Fu 3743u, 0xE9F, 0xE9F, 1, 0x160u, E, 9, F
#define PPUTLIMPL_UHEX_0xE9Eu 3742u, 0xE9E, 0xE9E, 1, 0x161u, E, 9, E
#define PPUTLIMPL_UHEX_0xE9Du 3741u, 0xE9D, 0xE9D, 1, 0x162u, E, 9, D
#define PPUTLIMPL_UHEX_0xE9Cu 3740u, 0xE9C, 0xE9C, 1, 0x163u, E, 9, C
#define PPUTLIMPL_UHEX_0xE9Bu 3739u, 0xE9B, 0xE9B, 1, 0x164u, E, 9, B
#define PPUTLIMPL_UHEX_0xE9Au 3738u, 0xE9A, 0xE9A, 1, 0x165u, E, 9, A
#define PPUTLIMPL_UHEX_0xE99u 3737u, 0xE99, 0xE99, 1, 0x166u, E, 9, 9
#define PPUTLIMPL_UHEX_0xE98u 3736u, 0xE98, 0xE98, 1, 0x167u, E, 9, 8
#define PPUTLIMPL_UHEX_0xE97u 3735u, 0xE97, 0xE97, 1, 0x168u, E, 9, 7
#define PPUTLIMPL_UHEX_0xE96u 3734u, 0xE96, 0xE96, 1, 0x169u, E, 9, 6
#define PPUTLIMPL_UHEX_0xE95u 3733u, 0xE95, 0xE95, 1, 0x16Au, E, 9, 5
#define PPUTLIMPL_UHEX_0xE94u 3732u, 0xE94, 0xE94, 1, 0x16Bu, E, 9, 4
#define PPUTLIMPL_UHEX_0xE93u 3731u, 0xE93, 0xE93, 1, 0x16Cu, E, 9, 3
#define PPUTLIMPL_UHEX_0xE92u 3730u, 0xE92, 0xE92, 1, 0x16Du, E, 9, 2
#define PPUTLIMPL_UHEX_0xE91u 3729u, 0xE91, 0xE91, 1, 0x16Eu, E, 9, 1
#define PPUTLIMPL_UHEX_0xE90u 3728u, 0xE90, 0xE90, 1, 0x16Fu, E, 9, 0
#define PPUTLIMPL_UHEX_0xE8Fu 3727u, 0xE8F, 0xE8F, 1, 0x170u, E, 8, F
#define PPUTLIMPL_UHEX_0xE8Eu 3726u, 0xE8E, 0xE8E, 1, 0x171u, E, 8, E
#define PPUTLIMPL_UHEX_0xE8Du 3725u, 0xE8D, 0xE8D, 1, 0x172u, E, 8, D
#define PPUTLIMPL_UHEX_0xE8Cu 3724u, 0xE8C, 0xE8C, 1, 0x173u, E, 8, C
#define PPUTLIMPL_UHEX_0xE8Bu 3723u, 0xE8B, 0xE8B, 1, 0x174u, E, 8, B
#define PPUTLIMPL_UHEX_0xE8Au 3722u, 0xE8A, 0xE8A, 1, 0x175u, E, 8, A
#define PPUTLIMPL_UHEX_0xE89u 3721u, 0xE89, 0xE89, 1, 0x176u, E, 8, 9
#define PPUTLIMPL_UHEX_0xE88u 3720u, 0xE88, 0xE88, 1, 0x177u, E, 8, 8
#define PPUTLIMPL_UHEX_0xE87u 3719u, 0xE87, 0xE87, 1, 0x178u, E, 8, 7
#define PPUTLIMPL_UHEX_0xE86u 3718u, 0xE86, 0xE86, 1, 0x179u, E, 8, 6
#define PPUTLIMPL_UHEX_0xE85u 3717u, 0xE85, 0xE85, 1, 0x17Au, E, 8, 5
#define PPUTLIMPL_UHEX_0xE84u 3716u, 0xE84, 0xE84, 1, 0x17Bu, E, 8, 4
#define PPUTLIMPL_UHEX_0xE83u 3715u, 0xE83, 0xE83, 1, 0x17Cu, E, 8, 3
#define PPUTLIMPL_UHEX_0xE82u 3714u, 0xE82, 0xE82, 1, 0x17Du, E, 8, 2
#define PPUTLIMPL_UHEX_0xE81u 3713u, 0xE81, 0xE81, 1, 0x17Eu, E, 8, 1
#define PPUTLIMPL_UHEX_0xE80u 3712u, 0xE80, 0xE80, 1, 0x17Fu, E, 8, 0
#define PPUTLIMPL_UHEX_0xE7Fu 3711u, 0xE7F, 0xE7F, 1, 0x180u, E, 7, F
#define PPUTLIMPL_UHEX_0xE7Eu 3710u, 0xE7E, 0xE7E, 1, 0x181u, E, 7, E
#define PPUTLIMPL_UHEX_0xE7Du 3709u, 0xE7D, 0xE7D, 1, 0x182u, E, 7, D
#define PPUTLIMPL_UHEX_0xE7Cu 3708u, 0xE7C, 0xE7C, 1, 0x183u, E, 7, C
#define PPUTLIMPL_UHEX_0xE7Bu 3707u, 0xE7B, 0xE7B, 1, 0x184u, E, 7, B
#define PPUTLIMPL_UHEX_0xE7Au 3706u, 0xE7A, 0xE7A, 1, 0x185u, E, 7, A
#define PPUTLIMPL_UHEX_0xE79u 3705u, 0xE79, 0xE79, 1, 0x186u, E, 7, 9
#define PPUTLIMPL_UHEX_0xE78u 3704u, 0xE78, 0xE78, 1, 0x187u, E, 7, 8
#define PPUTLIMPL_UHEX_0xE77u 3703u, 0xE77, 0xE77, 1, 0x188u, E, 7, 7
#define PPUTLIMPL_UHEX_0xE76u 3702u, 0xE76, 0xE76, 1, 0x189u, E, 7, 6
#define PPUTLIMPL_UHEX_0xE75u 3701u, 0xE75, 0xE75, 1, 0x18Au, E, 7, 5
#define PPUTLIMPL_UHEX_0xE74u 3700u, 0xE74, 0xE74, 1, 0x18Bu, E, 7, 4
#define PPUTLIMPL_UHEX_0xE73u 3699u, 0xE73, 0xE73, 1, 0x18Cu, E, 7, 3
#define PPUTLIMPL_UHEX_0xE72u 3698u, 0xE72, 0xE72, 1, 0x18Du, E, 7, 2
#define PPUTLIMPL_UHEX_0xE71u 3697u, 0xE71, 0xE71, 1, 0x18Eu, E, 7, 1
#define PPUTLIMPL_UHEX_0xE70u 3696u, 0xE70, 0xE70, 1, 0x18Fu, E, 7, 0
#define PPUTLIMPL_UHEX_0xE6Fu 3695u, 0xE6F, 0xE6F, 1, 0x190u, E, 6, F
#define PPUTLIMPL_UHEX_0xE6Eu 3694u, 0xE6E, 0xE6E, 1, 0x191u, E, 6, E
#define PPUTLIMPL_UHEX_0xE6Du 3693u, 0xE6D, 0xE6D, 1, 0x192u, E, 6, D
#define PPUTLIMPL_UHEX_0xE6Cu 3692u, 0xE6C, 0xE6C, 1, 0x193u, E, 6, C
#define PPUTLIMPL_UHEX_0xE6Bu 3691u, 0xE6B, 0xE6B, 1, 0x194u, E, 6, B
#define PPUTLIMPL_UHEX_0xE6Au 3690u, 0xE6A, 0xE6A, 1, 0x195u, E, 6, A
#define PPUTLIMPL_UHEX_0xE69u 3689u, 0xE69, 0xE69, 1, 0x196u, E, 6, 9
#define PPUTLIMPL_UHEX_0xE68u 3688u, 0xE68, 0xE68, 1, 0x197u, E, 6, 8
#define PPUTLIMPL_UHEX_0xE67u 3687u, 0xE67, 0xE67, 1, 0x198u, E, 6, 7
#define PPUTLIMPL_UHEX_0xE66u 3686u, 0xE66, 0xE66, 1, 0x199u, E, 6, 6
#define PPUTLIMPL_UHEX_0xE65u 3685u, 0xE65, 0xE65, 1, 0x19Au, E, 6, 5
#define PPUTLIMPL_UHEX_0xE64u 3684u, 0xE64, 0xE64, 1, 0x19Bu, E, 6, 4
#define PPUTLIMPL_UHEX_0xE63u 3683u, 0xE63, 0xE63, 1, 0x19Cu, E, 6, 3
#define PPUTLIMPL_UHEX_0xE62u 3682u, 0xE62, 0xE62, 1, 0x19Du, E, 6, 2
#define PPUTLIMPL_UHEX_0xE61u 3681u, 0xE61, 0xE61, 1, 0x19Eu, E, 6, 1
#define PPUTLIMPL_UHEX_0xE60u 3680u, 0xE60, 0xE60, 1, 0x19Fu, E, 6, 0
#define PPUTLIMPL_UHEX_0xE5Fu 3679u, 0xE5F, 0xE5F, 1, 0x1A0u, E, 5, F
#define PPUTLIMPL_UHEX_0xE5Eu 3678u, 0xE5E, 0xE5E, 1, 0x1A1u, E, 5, E
#define PPUTLIMPL_UHEX_0xE5Du 3677u, 0xE5D, 0xE5D, 1, 0x1A2u, E, 5, D
#define PPUTLIMPL_UHEX_0xE5Cu 3676u, 0xE5C, 0xE5C, 1, 0x1A3u, E, 5, C
#define PPUTLIMPL_UHEX_0xE5Bu 3675u, 0xE5B, 0xE5B, 1, 0x1A4u, E, 5, B
#define PPUTLIMPL_UHEX_0xE5Au 3674u, 0xE5A, 0xE5A, 1, 0x1A5u, E, 5, A
#define PPUTLIMPL_UHEX_0xE59u 3673u, 0xE59, 0xE59, 1, 0x1A6u, E, 5, 9
#define PPUTLIMPL_UHEX_0xE58u 3672u, 0xE58, 0xE58, 1, 0x1A7u, E, 5, 8
#define PPUTLIMPL_UHEX_0xE57u 3671u, 0xE57, 0xE57, 1, 0x1A8u, E, 5, 7
#define PPUTLIMPL_UHEX_0xE56u 3670u, 0xE56, 0xE56, 1, 0x1A9u, E, 5, 6
#define PPUTLIMPL_UHEX_0xE55u 3669u, 0xE55, 0xE55, 1, 0x1AAu, E, 5, 5
#define PPUTLIMPL_UHEX_0xE54u 3668u, 0xE54, 0xE54, 1, 0x1ABu, E, 5, 4
#define PPUTLIMPL_UHEX_0xE53u 3667u, 0xE53, 0xE53, 1, 0x1ACu, E, 5, 3
#define PPUTLIMPL_UHEX_0xE52u 3666u, 0xE52, 0xE52, 1, 0x1ADu, E, 5, 2
#define PPUTLIMPL_UHEX_0xE51u 3665u, 0xE51, 0xE51, 1, 0x1AEu, E, 5, 1
#define PPUTLIMPL_UHEX_0xE50u 3664u, 0xE50, 0xE50, 1, 0x1AFu, E, 5, 0
#define PPUTLIMPL_UHEX_0xE4Fu 3663u, 0xE4F, 0xE4F, 1, 0x1B0u, E, 4, F
#define PPUTLIMPL_UHEX_0xE4Eu 3662u, 0xE4E, 0xE4E, 1, 0x1B1u, E, 4, E
#define PPUTLIMPL_UHEX_0xE4Du 3661u, 0xE4D, 0xE4D, 1, 0x1B2u, E, 4, D
#define PPUTLIMPL_UHEX_0xE4Cu 3660u, 0xE4C, 0xE4C, 1, 0x1B3u, E, 4, C
#define PPUTLIMPL_UHEX_0xE4Bu 3659u, 0xE4B, 0xE4B, 1, 0x1B4u, E, 4, B
#define PPUTLIMPL_UHEX_0xE4Au 3658u, 0xE4A, 0xE4A, 1, 0x1B5u, E, 4, A
#define PPUTLIMPL_UHEX_0xE49u 3657u, 0xE49, 0xE49, 1, 0x1B6u, E, 4, 9
#define PPUTLIMPL_UHEX_0xE48u 3656u, 0xE48, 0xE48, 1, 0x1B7u, E, 4, 8
#define PPUTLIMPL_UHEX_0xE47u 3655u, 0xE47, 0xE47, 1, 0x1B8u, E, 4, 7
#define PPUTLIMPL_UHEX_0xE46u 3654u, 0xE46, 0xE46, 1, 0x1B9u, E, 4, 6
#define PPUTLIMPL_UHEX_0xE45u 3653u, 0xE45, 0xE45, 1, 0x1BAu, E, 4, 5
#define PPUTLIMPL_UHEX_0xE44u 3652u, 0xE44, 0xE44, 1, 0x1BBu, E, 4, 4
#define PPUTLIMPL_UHEX_0xE43u 3651u, 0xE43, 0xE43, 1, 0x1BCu, E, 4, 3
#define PPUTLIMPL_UHEX_0xE42u 3650u, 0xE42, 0xE42, 1, 0x1BDu, E, 4, 2
#define PPUTLIMPL_UHEX_0xE41u 3649u, 0xE41, 0xE41, 1, 0x1BEu, E, 4, 1
#define PPUTLIMPL_UHEX_0xE40u 3648u, 0xE40, 0xE40, 1, 0x1BFu, E, 4, 0
#define PPUTLIMPL_UHEX_0xE3Fu 3647u, 0xE3F, 0xE3F, 1, 0x1C0u, E, 3, F
#define PPUTLIMPL_UHEX_0xE3Eu 3646u, 0xE3E, 0xE3E, 1, 0x1C1u, E, 3, E
#define PPUTLIMPL_UHEX_0xE3Du 3645u, 0xE3D, 0xE3D, 1, 0x1C2u, E, 3, D
#define PPUTLIMPL_UHEX_0xE3Cu 3644u, 0xE3C, 0xE3C, 1, 0x1C3u, E, 3, C
#define PPUTLIMPL_UHEX_0xE3Bu 3643u, 0xE3B, 0xE3B, 1, 0x1C4u, E, 3, B
#define PPUTLIMPL_UHEX_0xE3Au 3642u, 0xE3A, 0xE3A, 1, 0x1C5u, E, 3, A
#define PPUTLIMPL_UHEX_0xE39u 3641u, 0xE39, 0xE39, 1, 0x1C6u, E, 3, 9
#define PPUTLIMPL_UHEX_0xE38u 3640u, 0xE38, 0xE38, 1, 0x1C7u, E, 3, 8
#define PPUTLIMPL_UHEX_0xE37u 3639u, 0xE37, 0xE37, 1, 0x1C8u, E, 3, 7
#define PPUTLIMPL_UHEX_0xE36u 3638u, 0xE36, 0xE36, 1, 0x1C9u, E, 3, 6
#define PPUTLIMPL_UHEX_0xE35u 3637u, 0xE35, 0xE35, 1, 0x1CAu, E, 3, 5
#define PPUTLIMPL_UHEX_0xE34u 3636u, 0xE34, 0xE34, 1, 0x1CBu, E, 3, 4
#define PPUTLIMPL_UHEX_0xE33u 3635u, 0xE33, 0xE33, 1, 0x1CCu, E, 3, 3
#define PPUTLIMPL_UHEX_0xE32u 3634u, 0xE32, 0xE32, 1, 0x1CDu, E, 3, 2
#define PPUTLIMPL_UHEX_0xE31u 3633u, 0xE31, 0xE31, 1, 0x1CEu, E, 3, 1
#define PPUTLIMPL_UHEX_0xE30u 3632u, 0xE30, 0xE30, 1, 0x1CFu, E, 3, 0
#define PPUTLIMPL_UHEX_0xE2Fu 3631u, 0xE2F, 0xE2F, 1, 0x1D0u, E, 2, F
#define PPUTLIMPL_UHEX_0xE2Eu 3630u, 0xE2E, 0xE2E, 1, 0x1D1u, E, 2, E
#define PPUTLIMPL_UHEX_0xE2Du 3629u, 0xE2D, 0xE2D, 1, 0x1D2u, E, 2, D
#define PPUTLIMPL_UHEX_0xE2Cu 3628u, 0xE2C, 0xE2C, 1, 0x1D3u, E, 2, C
#define PPUTLIMPL_UHEX_0xE2Bu 3627u, 0xE2B, 0xE2B, 1, 0x1D4u, E, 2, B
#define PPUTLIMPL_UHEX_0xE2Au 3626u, 0xE2A, 0xE2A, 1, 0x1D5u, E, 2, A
#define PPUTLIMPL_UHEX_0xE29u 3625u, 0xE29, 0xE29, 1, 0x1D6u, E, 2, 9
#define PPUTLIMPL_UHEX_0xE28u 3624u, 0xE28, 0xE28, 1, 0x1D7u, E, 2, 8
#define PPUTLIMPL_UHEX_0xE27u 3623u, 0xE27, 0xE27, 1, 0x1D8u, E, 2, 7
#define PPUTLIMPL_UHEX_0xE26u 3622u, 0xE26, 0xE26, 1, 0x1D9u, E, 2, 6
#define PPUTLIMPL_UHEX_0xE25u 3621u, 0xE25, 0xE25, 1, 0x1DAu, E, 2, 5
#define PPUTLIMPL_UHEX_0xE24u 3620u, 0xE24, 0xE24, 1, 0x1DBu, E, 2, 4
#define PPUTLIMPL_UHEX_0xE23u 3619u, 0xE23, 0xE23, 1, 0x1DCu, E, 2, 3
#define PPUTLIMPL_UHEX_0xE22u 3618u, 0xE22, 0xE22, 1, 0x1DDu, E, 2, 2
#define PPUTLIMPL_UHEX_0xE21u 3617u, 0xE21, 0xE21, 1, 0x1DEu, E, 2, 1
#define PPUTLIMPL_UHEX_0xE20u 3616u, 0xE20, 0xE20, 1, 0x1DFu, E, 2, 0
#define PPUTLIMPL_UHEX_0xE1Fu 3615u, 0xE1F, 0xE1F, 1, 0x1E0u, E, 1, F
#define PPUTLIMPL_UHEX_0xE1Eu 3614u, 0xE1E, 0xE1E, 1, 0x1E1u, E, 1, E
#define PPUTLIMPL_UHEX_0xE1Du 3613u, 0xE1D, 0xE1D, 1, 0x1E2u, E, 1, D
#define PPUTLIMPL_UHEX_0xE1Cu 3612u, 0xE1C, 0xE1C, 1, 0x1E3u, E, 1, C
#define PPUTLIMPL_UHEX_0xE1Bu 3611u, 0xE1B, 0xE1B, 1, 0x1E4u, E, 1, B
#define PPUTLIMPL_UHEX_0xE1Au 3610u, 0xE1A, 0xE1A, 1, 0x1E5u, E, 1, A
#define PPUTLIMPL_UHEX_0xE19u 3609u, 0xE19, 0xE19, 1, 0x1E6u, E, 1, 9
#define PPUTLIMPL_UHEX_0xE18u 3608u, 0xE18, 0xE18, 1, 0x1E7u, E, 1, 8
#define PPUTLIMPL_UHEX_0xE17u 3607u, 0xE17, 0xE17, 1, 0x1E8u, E, 1, 7
#define PPUTLIMPL_UHEX_0xE16u 3606u, 0xE16, 0xE16, 1, 0x1E9u, E, 1, 6
#define PPUTLIMPL_UHEX_0xE15u 3605u, 0xE15, 0xE15, 1, 0x1EAu, E, 1, 5
#define PPUTLIMPL_UHEX_0xE14u 3604u, 0xE14, 0xE14, 1, 0x1EBu, E, 1, 4
#define PPUTLIMPL_UHEX_0xE13u 3603u, 0xE13, 0xE13, 1, 0x1ECu, E, 1, 3
#define PPUTLIMPL_UHEX_0xE12u 3602u, 0xE12, 0xE12, 1, 0x1EDu, E, 1, 2
#define PPUTLIMPL_UHEX_0xE11u 3601u, 0xE11, 0xE11, 1, 0x1EEu, E, 1, 1
#define PPUTLIMPL_UHEX_0xE10u 3600u, 0xE10, 0xE10, 1, 0x1EFu, E, 1, 0
#define PPUTLIMPL_UHEX_0xE0Fu 3599u, 0xE0F, 0xE0F, 1, 0x1F0u, E, 0, F
#define PPUTLIMPL_UHEX_0xE0Eu 3598u, 0xE0E, 0xE0E, 1, 0x1F1u, E, 0, E
#define PPUTLIMPL_UHEX_0xE0Du 3597u, 0xE0D, 0xE0D, 1, 0x1F2u, E, 0, D
#define PPUTLIMPL_UHEX_0xE0Cu 3596u, 0xE0C, 0xE0C, 1, 0x1F3u, E, 0, C
#define PPUTLIMPL_UHEX_0xE0Bu 3595u, 0xE0B, 0xE0B, 1, 0x1F4u, E, 0, B
#define PPUTLIMPL_UHEX_0xE0Au 3594u, 0xE0A, 0xE0A, 1, 0x1F5u, E, 0, A
#define PPUTLIMPL_UHEX_0xE09u 3593u, 0xE09, 0xE09, 1, 0x1F6u, E, 0, 9
#define PPUTLIMPL_UHEX_0xE08u 3592u, 0xE08, 0xE08, 1, 0x1F7u, E, 0, 8
#define PPUTLIMPL_UHEX_0xE07u 3591u, 0xE07, 0xE07, 1, 0x1F8u, E, 0, 7
#define PPUTLIMPL_UHEX_0xE06u 3590u, 0xE06, 0xE06, 1, 0x1F9u, E, 0, 6
#define PPUTLIMPL_UHEX_0xE05u 3589u, 0xE05, 0xE05, 1, 0x1FAu, E, 0, 5
#define PPUTLIMPL_UHEX_0xE04u 3588u, 0xE04, 0xE04, 1, 0x1FBu, E, 0, 4
#define PPUTLIMPL_UHEX_0xE03u 3587u, 0xE03, 0xE03, 1, 0x1FCu, E, 0, 3
#define PPUTLIMPL_UHEX_0xE02u 3586u, 0xE02, 0xE02, 1, 0x1FDu, E, 0, 2
#define PPUTLIMPL_UHEX_0xE01u 3585u, 0xE01, 0xE01, 1, 0x1FEu, E, 0, 1
#define PPUTLIMPL_UHEX_0xE00u 3584u, 0xE00, 0xE00, 1, 0x1FFu, E, 0, 0
#define PPUTLIMPL_UHEX_0xDFFu 3583u, 0xDFF, 0xDFF, 1, 0x200u, D, F, F
#define PPUTLIMPL_UHEX_0xDFEu 3582u, 0xDFE, 0xDFE, 1, 0x201u, D, F, E
#define PPUTLIMPL_UHEX_0xDFDu 3581u, 0xDFD, 0xDFD, 1, 0x202u, D, F, D
#define PPUTLIMPL_UHEX_0xDFCu 3580u, 0xDFC, 0xDFC, 1, 0x203u, D, F, C
#define PPUTLIMPL_UHEX_0xDFBu 3579u, 0xDFB, 0xDFB, 1, 0x204u, D, F, B
#define PPUTLIMPL_UHEX_0xDFAu 3578u, 0xDFA, 0xDFA, 1, 0x205u, D, F, A
#define PPUTLIMPL_UHEX_0xDF9u 3577u, 0xDF9, 0xDF9, 1, 0x206u, D, F, 9
#define PPUTLIMPL_UHEX_0xDF8u 3576u, 0xDF8, 0xDF8, 1, 0x207u, D, F, 8
#define PPUTLIMPL_UHEX_0xDF7u 3575u, 0xDF7, 0xDF7, 1, 0x208u, D, F, 7
#define PPUTLIMPL_UHEX_0xDF6u 3574u, 0xDF6, 0xDF6, 1, 0x209u, D, F, 6
#define PPUTLIMPL_UHEX_0xDF5u 3573u, 0xDF5, 0xDF5, 1, 0x20Au, D, F, 5
#define PPUTLIMPL_UHEX_0xDF4u 3572u, 0xDF4, 0xDF4, 1, 0x20Bu, D, F, 4
#define PPUTLIMPL_UHEX_0xDF3u 3571u, 0xDF3, 0xDF3, 1, 0x20Cu, D, F, 3
#define PPUTLIMPL_UHEX_0xDF2u 3570u, 0xDF2, 0xDF2, 1, 0x20Du, D, F, 2
#define PPUTLIMPL_UHEX_0xDF1u 3569u, 0xDF1, 0xDF1, 1, 0x20Eu, D, F, 1
#define PPUTLIMPL_UHEX_0xDF0u 3568u, 0xDF0, 0xDF0, 1, 0x20Fu, D, F, 0
#define PPUTLIMPL_UHEX_0xDEFu 3567u, 0xDEF, 0xDEF, 1, 0x210u, D, E, F
#define PPUTLIMPL_UHEX_0xDEEu 3566u, 0xDEE, 0xDEE, 1, 0x211u, D, E, E
#define PPUTLIMPL_UHEX_0xDEDu 3565u, 0xDED, 0xDED, 1, 0x212u, D, E, D
#define PPUTLIMPL_UHEX_0xDECu 3564u, 0xDEC, 0xDEC, 1, 0x213u, D, E, C
#define PPUTLIMPL_UHEX_0xDEBu 3563u, 0xDEB, 0xDEB, 1, 0x214u, D, E, B
#define PPUTLIMPL_UHEX_0xDEAu 3562u, 0xDEA, 0xDEA, 1, 0x215u, D, E, A
#define PPUTLIMPL_UHEX_0xDE9u 3561u, 0xDE9, 0xDE9, 1, 0x216u, D, E, 9
#define PPUTLIMPL_UHEX_0xDE8u 3560u, 0xDE8, 0xDE8, 1, 0x217u, D, E, 8
#define PPUTLIMPL_UHEX_0xDE7u 3559u, 0xDE7, 0xDE7, 1, 0x218u, D, E, 7
#define PPUTLIMPL_UHEX_0xDE6u 3558u, 0xDE6, 0xDE6, 1, 0x219u, D, E, 6
#define PPUTLIMPL_UHEX_0xDE5u 3557u, 0xDE5, 0xDE5, 1, 0x21Au, D, E, 5
#define PPUTLIMPL_UHEX_0xDE4u 3556u, 0xDE4, 0xDE4, 1, 0x21Bu, D, E, 4
#define PPUTLIMPL_UHEX_0xDE3u 3555u, 0xDE3, 0xDE3, 1, 0x21Cu, D, E, 3
#define PPUTLIMPL_UHEX_0xDE2u 3554u, 0xDE2, 0xDE2, 1, 0x21Du, D, E, 2
#define PPUTLIMPL_UHEX_0xDE1u 3553u, 0xDE1, 0xDE1, 1, 0x21Eu, D, E, 1
#define PPUTLIMPL_UHEX_0xDE0u 3552u, 0xDE0, 0xDE0, 1, 0x21Fu, D, E, 0
#define PPUTLIMPL_UHEX_0xDDFu 3551u, 0xDDF, 0xDDF, 1, 0x220u, D, D, F
#define PPUTLIMPL_UHEX_0xDDEu 3550u, 0xDDE, 0xDDE, 1, 0x221u, D, D, E
#define PPUTLIMPL_UHEX_0xDDDu 3549u, 0xDDD, 0xDDD, 1, 0x222u, D, D, D
#define PPUTLIMPL_UHEX_0xDDCu 3548u, 0xDDC, 0xDDC, 1, 0x223u, D, D, C
#define PPUTLIMPL_UHEX_0xDDBu 3547u, 0xDDB, 0xDDB, 1, 0x224u, D, D, B
#define PPUTLIMPL_UHEX_0xDDAu 3546u, 0xDDA, 0xDDA, 1, 0x225u, D, D, A
#define PPUTLIMPL_UHEX_0xDD9u 3545u, 0xDD9, 0xDD9, 1, 0x226u, D, D, 9
#define PPUTLIMPL_UHEX_0xDD8u 3544u, 0xDD8, 0xDD8, 1, 0x227u, D, D, 8
#define PPUTLIMPL_UHEX_0xDD7u 3543u, 0xDD7, 0xDD7, 1, 0x228u, D, D, 7
#define PPUTLIMPL_UHEX_0xDD6u 3542u, 0xDD6, 0xDD6, 1, 0x229u, D, D, 6
#define PPUTLIMPL_UHEX_0xDD5u 3541u, 0xDD5, 0xDD5, 1, 0x22Au, D, D, 5
#define PPUTLIMPL_UHEX_0xDD4u 3540u, 0xDD4, 0xDD4, 1, 0x22Bu, D, D, 4
#define PPUTLIMPL_UHEX_0xDD3u 3539u, 0xDD3, 0xDD3, 1, 0x22Cu, D, D, 3
#define PPUTLIMPL_UHEX_0xDD2u 3538u, 0xDD2, 0xDD2, 1, 0x22Du, D, D, 2
#define PPUTLIMPL_UHEX_0xDD1u 3537u, 0xDD1, 0xDD1, 1, 0x22Eu, D, D, 1
#define PPUTLIMPL_UHEX_0xDD0u 3536u, 0xDD0, 0xDD0, 1, 0x22Fu, D, D, 0
#define PPUTLIMPL_UHEX_0xDCFu 3535u, 0xDCF, 0xDCF, 1, 0x230u, D, C, F
#define PPUTLIMPL_UHEX_0xDCEu 3534u, 0xDCE, 0xDCE, 1, 0x231u, D, C, E
#define PPUTLIMPL_UHEX_0xDCDu 3533u, 0xDCD, 0xDCD, 1, 0x232u, D, C, D
#define PPUTLIMPL_UHEX_0xDCCu 3532u, 0xDCC, 0xDCC, 1, 0x233u, D, C, C
#define PPUTLIMPL_UHEX_0xDCBu 3531u, 0xDCB, 0xDCB, 1, 0x234u, D, C, B
#define PPUTLIMPL_UHEX_0xDCAu 3530u, 0xDCA, 0xDCA, 1, 0x235u, D, C, A
#define PPUTLIMPL_UHEX_0xDC9u 3529u, 0xDC9, 0xDC9, 1, 0x236u, D, C, 9
#define PPUTLIMPL_UHEX_0xDC8u 3528u, 0xDC8, 0xDC8, 1, 0x237u, D, C, 8
#define PPUTLIMPL_UHEX_0xDC7u 3527u, 0xDC7, 0xDC7, 1, 0x238u, D, C, 7
#define PPUTLIMPL_UHEX_0xDC6u 3526u, 0xDC6, 0xDC6, 1, 0x239u, D, C, 6
#define PPUTLIMPL_UHEX_0xDC5u 3525u, 0xDC5, 0xDC5, 1, 0x23Au, D, C, 5
#define PPUTLIMPL_UHEX_0xDC4u 3524u, 0xDC4, 0xDC4, 1, 0x23Bu, D, C, 4
#define PPUTLIMPL_UHEX_0xDC3u 3523u, 0xDC3, 0xDC3, 1, 0x23Cu, D, C, 3
#define PPUTLIMPL_UHEX_0xDC2u 3522u, 0xDC2, 0xDC2, 1, 0x23Du, D, C, 2
#define PPUTLIMPL_UHEX_0xDC1u 3521u, 0xDC1, 0xDC1, 1, 0x23Eu, D, C, 1
#define PPUTLIMPL_UHEX_0xDC0u 3520u, 0xDC0, 0xDC0, 1, 0x23Fu, D, C, 0
#define PPUTLIMPL_UHEX_0xDBFu 3519u, 0xDBF, 0xDBF, 1, 0x240u, D, B, F
#define PPUTLIMPL_UHEX_0xDBEu 3518u, 0xDBE, 0xDBE, 1, 0x241u, D, B, E
#define PPUTLIMPL_UHEX_0xDBDu 3517u, 0xDBD, 0xDBD, 1, 0x242u, D, B, D
#define PPUTLIMPL_UHEX_0xDBCu 3516u, 0xDBC, 0xDBC, 1, 0x243u, D, B, C
#define PPUTLIMPL_UHEX_0xDBBu 3515u, 0xDBB, 0xDBB, 1, 0x244u, D, B, B
#define PPUTLIMPL_UHEX_0xDBAu 3514u, 0xDBA, 0xDBA, 1, 0x245u, D, B, A
#define PPUTLIMPL_UHEX_0xDB9u 3513u, 0xDB9, 0xDB9, 1, 0x246u, D, B, 9
#define PPUTLIMPL_UHEX_0xDB8u 3512u, 0xDB8, 0xDB8, 1, 0x247u, D, B, 8
#define PPUTLIMPL_UHEX_0xDB7u 3511u, 0xDB7, 0xDB7, 1, 0x248u, D, B, 7
#define PPUTLIMPL_UHEX_0xDB6u 3510u, 0xDB6, 0xDB6, 1, 0x249u, D, B, 6
#define PPUTLIMPL_UHEX_0xDB5u 3509u, 0xDB5, 0xDB5, 1, 0x24Au, D, B, 5
#define PPUTLIMPL_UHEX_0xDB4u 3508u, 0xDB4, 0xDB4, 1, 0x24Bu, D, B, 4
#define PPUTLIMPL_UHEX_0xDB3u 3507u, 0xDB3, 0xDB3, 1, 0x24Cu, D, B, 3
#define PPUTLIMPL_UHEX_0xDB2u 3506u, 0xDB2, 0xDB2, 1, 0x24Du, D, B, 2
#define PPUTLIMPL_UHEX_0xDB1u 3505u, 0xDB1, 0xDB1, 1, 0x24Eu, D, B, 1
#define PPUTLIMPL_UHEX_0xDB0u 3504u, 0xDB0, 0xDB0, 1, 0x24Fu, D, B, 0
#define PPUTLIMPL_UHEX_0xDAFu 3503u, 0xDAF, 0xDAF, 1, 0x250u, D, A, F
#define PPUTLIMPL_UHEX_0xDAEu 3502u, 0xDAE, 0xDAE, 1, 0x251u, D, A, E
#define PPUTLIMPL_UHEX_0xDADu 3501u, 0xDAD, 0xDAD, 1, 0x252u, D, A, D
#define PPUTLIMPL_UHEX_0xDACu 3500u, 0xDAC, 0xDAC, 1, 0x253u, D, A, C
#define PPUTLIMPL_UHEX_0xDABu 3499u, 0xDAB, 0xDAB, 1, 0x254u, D, A, B
#define PPUTLIMPL_UHEX_0xDAAu 3498u, 0xDAA, 0xDAA, 1, 0x255u, D, A, A
#define PPUTLIMPL_UHEX_0xDA9u 3497u, 0xDA9, 0xDA9, 1, 0x256u, D, A, 9
#define PPUTLIMPL_UHEX_0xDA8u 3496u, 0xDA8, 0xDA8, 1, 0x257u, D, A, 8
#define PPUTLIMPL_UHEX_0xDA7u 3495u, 0xDA7, 0xDA7, 1, 0x258u, D, A, 7
#define PPUTLIMPL_UHEX_0xDA6u 3494u, 0xDA6, 0xDA6, 1, 0x259u, D, A, 6
#define PPUTLIMPL_UHEX_0xDA5u 3493u, 0xDA5, 0xDA5, 1, 0x25Au, D, A, 5
#define PPUTLIMPL_UHEX_0xDA4u 3492u, 0xDA4, 0xDA4, 1, 0x25Bu, D, A, 4
#define PPUTLIMPL_UHEX_0xDA3u 3491u, 0xDA3, 0xDA3, 1, 0x25Cu, D, A, 3
#define PPUTLIMPL_UHEX_0xDA2u 3490u, 0xDA2, 0xDA2, 1, 0x25Du, D, A, 2
#define PPUTLIMPL_UHEX_0xDA1u 3489u, 0xDA1, 0xDA1, 1, 0x25Eu, D, A, 1
#define PPUTLIMPL_UHEX_0xDA0u 3488u, 0xDA0, 0xDA0, 1, 0x25Fu, D, A, 0
#define PPUTLIMPL_UHEX_0xD9Fu 3487u, 0xD9F, 0xD9F, 1, 0x260u, D, 9, F
#define PPUTLIMPL_UHEX_0xD9Eu 3486u, 0xD9E, 0xD9E, 1, 0x261u, D, 9, E
#define PPUTLIMPL_UHEX_0xD9Du 3485u, 0xD9D, 0xD9D, 1, 0x262u, D, 9, D
#define PPUTLIMPL_UHEX_0xD9Cu 3484u, 0xD9C, 0xD9C, 1, 0x263u, D, 9, C
#define PPUTLIMPL_UHEX_0xD9Bu 3483u, 0xD9B, 0xD9B, 1, 0x264u, D, 9, B
#define PPUTLIMPL_UHEX_0xD9Au 3482u, 0xD9A, 0xD9A, 1, 0x265u, D, 9, A
#define PPUTLIMPL_UHEX_0xD99u 3481u, 0xD99, 0xD99, 1, 0x266u, D, 9, 9
#define PPUTLIMPL_UHEX_0xD98u 3480u, 0xD98, 0xD98, 1, 0x267u, D, 9, 8
#define PPUTLIMPL_UHEX_0xD97u 3479u, 0xD97, 0xD97, 1, 0x268u, D, 9, 7
#define PPUTLIMPL_UHEX_0xD96u 3478u, 0xD96, 0xD96, 1, 0x269u, D, 9, 6
#define PPUTLIMPL_UHEX_0xD95u 3477u, 0xD95, 0xD95, 1, 0x26Au, D, 9, 5
#define PPUTLIMPL_UHEX_0xD94u 3476u, 0xD94, 0xD94, 1, 0x26Bu, D, 9, 4
#define PPUTLIMPL_UHEX_0xD93u 3475u, 0xD93, 0xD93, 1, 0x26Cu, D, 9, 3
#define PPUTLIMPL_UHEX_0xD92u 3474u, 0xD92, 0xD92, 1, 0x26Du, D, 9, 2
#define PPUTLIMPL_UHEX_0xD91u 3473u, 0xD91, 0xD91, 1, 0x26Eu, D, 9, 1
#define PPUTLIMPL_UHEX_0xD90u 3472u, 0xD90, 0xD90, 1, 0x26Fu, D, 9, 0
#define PPUTLIMPL_UHEX_0xD8Fu 3471u, 0xD8F, 0xD8F, 1, 0x270u, D, 8, F
#define PPUTLIMPL_UHEX_0xD8Eu 3470u, 0xD8E, 0xD8E, 1, 0x271u, D, 8, E
#define PPUTLIMPL_UHEX_0xD8Du 3469u, 0xD8D, 0xD8D, 1, 0x272u, D, 8, D
#define PPUTLIMPL_UHEX_0xD8Cu 3468u, 0xD8C, 0xD8C, 1, 0x273u, D, 8, C
#define PPUTLIMPL_UHEX_0xD8Bu 3467u, 0xD8B, 0xD8B, 1, 0x274u, D, 8, B
#define PPUTLIMPL_UHEX_0xD8Au 3466u, 0xD8A, 0xD8A, 1, 0x275u, D, 8, A
#define PPUTLIMPL_UHEX_0xD89u 3465u, 0xD89, 0xD89, 1, 0x276u, D, 8, 9
#define PPUTLIMPL_UHEX_0xD88u 3464u, 0xD88, 0xD88, 1, 0x277u, D, 8, 8
#define PPUTLIMPL_UHEX_0xD87u 3463u, 0xD87, 0xD87, 1, 0x278u, D, 8, 7
#define PPUTLIMPL_UHEX_0xD86u 3462u, 0xD86, 0xD86, 1, 0x279u, D, 8, 6
#define PPUTLIMPL_UHEX_0xD85u 3461u, 0xD85, 0xD85, 1, 0x27Au, D, 8, 5
#define PPUTLIMPL_UHEX_0xD84u 3460u, 0xD84, 0xD84, 1, 0x27Bu, D, 8, 4
#define PPUTLIMPL_UHEX_0xD83u 3459u, 0xD83, 0xD83, 1, 0x27Cu, D, 8, 3
#define PPUTLIMPL_UHEX_0xD82u 3458u, 0xD82, 0xD82, 1, 0x27Du, D, 8, 2
#define PPUTLIMPL_UHEX_0xD81u 3457u, 0xD81, 0xD81, 1, 0x27Eu, D, 8, 1
#define PPUTLIMPL_UHEX_0xD80u 3456u, 0xD80, 0xD80, 1, 0x27Fu, D, 8, 0
#define PPUTLIMPL_UHEX_0xD7Fu 3455u, 0xD7F, 0xD7F, 1, 0x280u, D, 7, F
#define PPUTLIMPL_UHEX_0xD7Eu 3454u, 0xD7E, 0xD7E, 1, 0x281u, D, 7, E
#define PPUTLIMPL_UHEX_0xD7Du 3453u, 0xD7D, 0xD7D, 1, 0x282u, D, 7, D
#define PPUTLIMPL_UHEX_0xD7Cu 3452u, 0xD7C, 0xD7C, 1, 0x283u, D, 7, C
#define PPUTLIMPL_UHEX_0xD7Bu 3451u, 0xD7B, 0xD7B, 1, 0x284u, D, 7, B
#define PPUTLIMPL_UHEX_0xD7Au 3450u, 0xD7A, 0xD7A, 1, 0x285u, D, 7, A
#define PPUTLIMPL_UHEX_0xD79u 3449u, 0xD79, 0xD79, 1, 0x286u, D, 7, 9
#define PPUTLIMPL_UHEX_0xD78u 3448u, 0xD78, 0xD78, 1, 0x287u, D, 7, 8
#define PPUTLIMPL_UHEX_0xD77u 3447u, 0xD77, 0xD77, 1, 0x288u, D, 7, 7
#define PPUTLIMPL_UHEX_0xD76u 3446u, 0xD76, 0xD76, 1, 0x289u, D, 7, 6
#define PPUTLIMPL_UHEX_0xD75u 3445u, 0xD75, 0xD75, 1, 0x28Au, D, 7, 5
#define PPUTLIMPL_UHEX_0xD74u 3444u, 0xD74, 0xD74, 1, 0x28Bu, D, 7, 4
#define PPUTLIMPL_UHEX_0xD73u 3443u, 0xD73, 0xD73, 1, 0x28Cu, D, 7, 3
#define PPUTLIMPL_UHEX_0xD72u 3442u, 0xD72, 0xD72, 1, 0x28Du, D, 7, 2
#define PPUTLIMPL_UHEX_0xD71u 3441u, 0xD71, 0xD71, 1, 0x28Eu, D, 7, 1
#define PPUTLIMPL_UHEX_0xD70u 3440u, 0xD70, 0xD70, 1, 0x28Fu, D, 7, 0
#define PPUTLIMPL_UHEX_0xD6Fu 3439u, 0xD6F, 0xD6F, 1, 0x290u, D, 6, F
#define PPUTLIMPL_UHEX_0xD6Eu 3438u, 0xD6E, 0xD6E, 1, 0x291u, D, 6, E
#define PPUTLIMPL_UHEX_0xD6Du 3437u, 0xD6D, 0xD6D, 1, 0x292u, D, 6, D
#define PPUTLIMPL_UHEX_0xD6Cu 3436u, 0xD6C, 0xD6C, 1, 0x293u, D, 6, C
#define PPUTLIMPL_UHEX_0xD6Bu 3435u, 0xD6B, 0xD6B, 1, 0x294u, D, 6, B
#define PPUTLIMPL_UHEX_0xD6Au 3434u, 0xD6A, 0xD6A, 1, 0x295u, D, 6, A
#define PPUTLIMPL_UHEX_0xD69u 3433u, 0xD69, 0xD69, 1, 0x296u, D, 6, 9
#define PPUTLIMPL_UHEX_0xD68u 3432u, 0xD68, 0xD68, 1, 0x297u, D, 6, 8
#define PPUTLIMPL_UHEX_0xD67u 3431u, 0xD67, 0xD67, 1, 0x298u, D, 6, 7
#define PPUTLIMPL_UHEX_0xD66u 3430u, 0xD66, 0xD66, 1, 0x299u, D, 6, 6
#define PPUTLIMPL_UHEX_0xD65u 3429u, 0xD65, 0xD65, 1, 0x29Au, D, 6, 5
#define PPUTLIMPL_UHEX_0xD64u 3428u, 0xD64, 0xD64, 1, 0x29Bu, D, 6, 4
#define PPUTLIMPL_UHEX_0xD63u 3427u, 0xD63, 0xD63, 1, 0x29Cu, D, 6, 3
#define PPUTLIMPL_UHEX_0xD62u 3426u, 0xD62, 0xD62, 1, 0x29Du, D, 6, 2
#define PPUTLIMPL_UHEX_0xD61u 3425u, 0xD61, 0xD61, 1, 0x29Eu, D, 6, 1
#define PPUTLIMPL_UHEX_0xD60u 3424u, 0xD60, 0xD60, 1, 0x29Fu, D, 6, 0
#define PPUTLIMPL_UHEX_0xD5Fu 3423u, 0xD5F, 0xD5F, 1, 0x2A0u, D, 5, F
#define PPUTLIMPL_UHEX_0xD5Eu 3422u, 0xD5E, 0xD5E, 1, 0x2A1u, D, 5, E
#define PPUTLIMPL_UHEX_0xD5Du 3421u, 0xD5D, 0xD5D, 1, 0x2A2u, D, 5, D
#define PPUTLIMPL_UHEX_0xD5Cu 3420u, 0xD5C, 0xD5C, 1, 0x2A3u, D, 5, C
#define PPUTLIMPL_UHEX_0xD5Bu 3419u, 0xD5B, 0xD5B, 1, 0x2A4u, D, 5, B
#define PPUTLIMPL_UHEX_0xD5Au 3418u, 0xD5A, 0xD5A, 1, 0x2A5u, D, 5, A
#define PPUTLIMPL_UHEX_0xD59u 3417u, 0xD59, 0xD59, 1, 0x2A6u, D, 5, 9
#define PPUTLIMPL_UHEX_0xD58u 3416u, 0xD58, 0xD58, 1, 0x2A7u, D, 5, 8
#define PPUTLIMPL_UHEX_0xD57u 3415u, 0xD57, 0xD57, 1, 0x2A8u, D, 5, 7
#define PPUTLIMPL_UHEX_0xD56u 3414u, 0xD56, 0xD56, 1, 0x2A9u, D, 5, 6
#define PPUTLIMPL_UHEX_0xD55u 3413u, 0xD55, 0xD55, 1, 0x2AAu, D, 5, 5
#define PPUTLIMPL_UHEX_0xD54u 3412u, 0xD54, 0xD54, 1, 0x2ABu, D, 5, 4
#define PPUTLIMPL_UHEX_0xD53u 3411u, 0xD53, 0xD53, 1, 0x2ACu, D, 5, 3
#define PPUTLIMPL_UHEX_0xD52u 3410u, 0xD52, 0xD52, 1, 0x2ADu, D, 5, 2
#define PPUTLIMPL_UHEX_0xD51u 3409u, 0xD51, 0xD51, 1, 0x2AEu, D, 5, 1
#define PPUTLIMPL_UHEX_0xD50u 3408u, 0xD50, 0xD50, 1, 0x2AFu, D, 5, 0
#define PPUTLIMPL_UHEX_0xD4Fu 3407u, 0xD4F, 0xD4F, 1, 0x2B0u, D, 4, F
#define PPUTLIMPL_UHEX_0xD4Eu 3406u, 0xD4E, 0xD4E, 1, 0x2B1u, D, 4, E
#define PPUTLIMPL_UHEX_0xD4Du 3405u, 0xD4D, 0xD4D, 1, 0x2B2u, D, 4, D
#define PPUTLIMPL_UHEX_0xD4Cu 3404u, 0xD4C, 0xD4C, 1, 0x2B3u, D, 4, C
#define PPUTLIMPL_UHEX_0xD4Bu 3403u, 0xD4B, 0xD4B, 1, 0x2B4u, D, 4, B
#define PPUTLIMPL_UHEX_0xD4Au 3402u, 0xD4A, 0xD4A, 1, 0x2B5u, D, 4, A
#define PPUTLIMPL_UHEX_0xD49u 3401u, 0xD49, 0xD49, 1, 0x2B6u, D, 4, 9
#define PPUTLIMPL_UHEX_0xD48u 3400u, 0xD48, 0xD48, 1, 0x2B7u, D, 4, 8
#define PPUTLIMPL_UHEX_0xD47u 3399u, 0xD47, 0xD47, 1, 0x2B8u, D, 4, 7
#define PPUTLIMPL_UHEX_0xD46u 3398u, 0xD46, 0xD46, 1, 0x2B9u, D, 4, 6
#define PPUTLIMPL_UHEX_0xD45u 3397u, 0xD45, 0xD45, 1, 0x2BAu, D, 4, 5
#define PPUTLIMPL_UHEX_0xD44u 3396u, 0xD44, 0xD44, 1, 0x2BBu, D, 4, 4
#define PPUTLIMPL_UHEX_0xD43u 3395u, 0xD43, 0xD43, 1, 0x2BCu, D, 4, 3
#define PPUTLIMPL_UHEX_0xD42u 3394u, 0xD42, 0xD42, 1, 0x2BDu, D, 4, 2
#define PPUTLIMPL_UHEX_0xD41u 3393u, 0xD41, 0xD41, 1, 0x2BEu, D, 4, 1
#define PPUTLIMPL_UHEX_0xD40u 3392u, 0xD40, 0xD40, 1, 0x2BFu, D, 4, 0
#define PPUTLIMPL_UHEX_0xD3Fu 3391u, 0xD3F, 0xD3F, 1, 0x2C0u, D, 3, F
#define PPUTLIMPL_UHEX_0xD3Eu 3390u, 0xD3E, 0xD3E, 1, 0x2C1u, D, 3, E
#define PPUTLIMPL_UHEX_0xD3Du 3389u, 0xD3D, 0xD3D, 1, 0x2C2u, D, 3, D
#define PPUTLIMPL_UHEX_0xD3Cu 3388u, 0xD3C, 0xD3C, 1, 0x2C3u, D, 3, C
#define PPUTLIMPL_UHEX_0xD3Bu 3387u, 0xD3B, 0xD3B, 1, 0x2C4u, D, 3, B
#define PPUTLIMPL_UHEX_0xD3Au 3386u, 0xD3A, 0xD3A, 1, 0x2C5u, D, 3, A
#define PPUTLIMPL_UHEX_0xD39u 3385u, 0xD39, 0xD39, 1, 0x2C6u, D, 3, 9
#define PPUTLIMPL_UHEX_0xD38u 3384u, 0xD38, 0xD38, 1, 0x2C7u, D, 3, 8
#define PPUTLIMPL_UHEX_0xD37u 3383u, 0xD37, 0xD37, 1, 0x2C8u, D, 3, 7
#define PPUTLIMPL_UHEX_0xD36u 3382u, 0xD36, 0xD36, 1, 0x2C9u, D, 3, 6
#define PPUTLIMPL_UHEX_0xD35u 3381u, 0xD35, 0xD35, 1, 0x2CAu, D, 3, 5
#define PPUTLIMPL_UHEX_0xD34u 3380u, 0xD34, 0xD34, 1, 0x2CBu, D, 3, 4
#define PPUTLIMPL_UHEX_0xD33u 3379u, 0xD33, 0xD33, 1, 0x2CCu, D, 3, 3
#define PPUTLIMPL_UHEX_0xD32u 3378u, 0xD32, 0xD32, 1, 0x2CDu, D, 3, 2
#define PPUTLIMPL_UHEX_0xD31u 3377u, 0xD31, 0xD31, 1, 0x2CEu, D, 3, 1
#define PPUTLIMPL_UHEX_0xD30u 3376u, 0xD30, 0xD30, 1, 0x2CFu, D, 3, 0
#define PPUTLIMPL_UHEX_0xD2Fu 3375u, 0xD2F, 0xD2F, 1, 0x2D0u, D, 2, F
#define PPUTLIMPL_UHEX_0xD2Eu 3374u, 0xD2E, 0xD2E, 1, 0x2D1u, D, 2, E
#define PPUTLIMPL_UHEX_0xD2Du 3373u, 0xD2D, 0xD2D, 1, 0x2D2u, D, 2, D
#define PPUTLIMPL_UHEX_0xD2Cu 3372u, 0xD2C, 0xD2C, 1, 0x2D3u, D, 2, C
#define PPUTLIMPL_UHEX_0xD2Bu 3371u, 0xD2B, 0xD2B, 1, 0x2D4u, D, 2, B
#define PPUTLIMPL_UHEX_0xD2Au 3370u, 0xD2A, 0xD2A, 1, 0x2D5u, D, 2, A
#define PPUTLIMPL_UHEX_0xD29u 3369u, 0xD29, 0xD29, 1, 0x2D6u, D, 2, 9
#define PPUTLIMPL_UHEX_0xD28u 3368u, 0xD28, 0xD28, 1, 0x2D7u, D, 2, 8
#define PPUTLIMPL_UHEX_0xD27u 3367u, 0xD27, 0xD27, 1, 0x2D8u, D, 2, 7
#define PPUTLIMPL_UHEX_0xD26u 3366u, 0xD26, 0xD26, 1, 0x2D9u, D, 2, 6
#define PPUTLIMPL_UHEX_0xD25u 3365u, 0xD25, 0xD25, 1, 0x2DAu, D, 2, 5
#define PPUTLIMPL_UHEX_0xD24u 3364u, 0xD24, 0xD24, 1, 0x2DBu, D, 2, 4
#define PPUTLIMPL_UHEX_0xD23u 3363u, 0xD23, 0xD23, 1, 0x2DCu, D, 2, 3
#define PPUTLIMPL_UHEX_0xD22u 3362u, 0xD22, 0xD22, 1, 0x2DDu, D, 2, 2
#define PPUTLIMPL_UHEX_0xD21u 3361u, 0xD21, 0xD21, 1, 0x2DEu, D, 2, 1
#define PPUTLIMPL_UHEX_0xD20u 3360u, 0xD20, 0xD20, 1, 0x2DFu, D, 2, 0
#define PPUTLIMPL_UHEX_0xD1Fu 3359u, 0xD1F, 0xD1F, 1, 0x2E0u, D, 1, F
#define PPUTLIMPL_UHEX_0xD1Eu 3358u, 0xD1E, 0xD1E, 1, 0x2E1u, D, 1, E
#define PPUTLIMPL_UHEX_0xD1Du 3357u, 0xD1D, 0xD1D, 1, 0x2E2u, D, 1, D
#define PPUTLIMPL_UHEX_0xD1Cu 3356u, 0xD1C, 0xD1C, 1, 0x2E3u, D, 1, C
#define PPUTLIMPL_UHEX_0xD1Bu 3355u, 0xD1B, 0xD1B, 1, 0x2E4u, D, 1, B
#define PPUTLIMPL_UHEX_0xD1Au 3354u, 0xD1A, 0xD1A, 1, 0x2E5u, D, 1, A
#define PPUTLIMPL_UHEX_0xD19u 3353u, 0xD19, 0xD19, 1, 0x2E6u, D, 1, 9
#define PPUTLIMPL_UHEX_0xD18u 3352u, 0xD18, 0xD18, 1, 0x2E7u, D, 1, 8
#define PPUTLIMPL_UHEX_0xD17u 3351u, 0xD17, 0xD17, 1, 0x2E8u, D, 1, 7
#define PPUTLIMPL_UHEX_0xD16u 3350u, 0xD16, 0xD16, 1, 0x2E9u, D, 1, 6
#define PPUTLIMPL_UHEX_0xD15u 3349u, 0xD15, 0xD15, 1, 0x2EAu, D, 1, 5
#define PPUTLIMPL_UHEX_0xD14u 3348u, 0xD14, 0xD14, 1, 0x2EBu, D, 1, 4
#define PPUTLIMPL_UHEX_0xD13u 3347u, 0xD13, 0xD13, 1, 0x2ECu, D, 1, 3
#define PPUTLIMPL_UHEX_0xD12u 3346u, 0xD12, 0xD12, 1, 0x2EDu, D, 1, 2
#define PPUTLIMPL_UHEX_0xD11u 3345u, 0xD11, 0xD11, 1, 0x2EEu, D, 1, 1
#define PPUTLIMPL_UHEX_0xD10u 3344u, 0xD10, 0xD10, 1, 0x2EFu, D, 1, 0
#define PPUTLIMPL_UHEX_0xD0Fu 3343u, 0xD0F, 0xD0F, 1, 0x2F0u, D, 0, F
#define PPUTLIMPL_UHEX_0xD0Eu 3342u, 0xD0E, 0xD0E, 1, 0x2F1u, D, 0, E
#define PPUTLIMPL_UHEX_0xD0Du 3341u, 0xD0D, 0xD0D, 1, 0x2F2u, D, 0, D
#define PPUTLIMPL_UHEX_0xD0Cu 3340u, 0xD0C, 0xD0C, 1, 0x2F3u, D, 0, C
#define PPUTLIMPL_UHEX_0xD0Bu 3339u, 0xD0B, 0xD0B, 1, 0x2F4u, D, 0, B
#define PPUTLIMPL_UHEX_0xD0Au 3338u, 0xD0A, 0xD0A, 1, 0x2F5u, D, 0, A
#define PPUTLIMPL_UHEX_0xD09u 3337u, 0xD09, 0xD09, 1, 0x2F6u, D, 0, 9
#define PPUTLIMPL_UHEX_0xD08u 3336u, 0xD08, 0xD08, 1, 0x2F7u, D, 0, 8
#define PPUTLIMPL_UHEX_0xD07u 3335u, 0xD07, 0xD07, 1, 0x2F8u, D, 0, 7
#define PPUTLIMPL_UHEX_0xD06u 3334u, 0xD06, 0xD06, 1, 0x2F9u, D, 0, 6
#define PPUTLIMPL_UHEX_0xD05u 3333u, 0xD05, 0xD05, 1, 0x2FAu, D, 0, 5
#define PPUTLIMPL_UHEX_0xD04u 3332u, 0xD04, 0xD04, 1, 0x2FBu, D, 0, 4
#define PPUTLIMPL_UHEX_0xD03u 3331u, 0xD03, 0xD03, 1, 0x2FCu, D, 0, 3
#define PPUTLIMPL_UHEX_0xD02u 3330u, 0xD02, 0xD02, 1, 0x2FDu, D, 0, 2
#define PPUTLIMPL_UHEX_0xD01u 3329u, 0xD01, 0xD01, 1, 0x2FEu, D, 0, 1
#define PPUTLIMPL_UHEX_0xD00u 3328u, 0xD00, 0xD00, 1, 0x2FFu, D, 0, 0
#define PPUTLIMPL_UHEX_0xCFFu 3327u, 0xCFF, 0xCFF, 1, 0x300u, C, F, F
#define PPUTLIMPL_UHEX_0xCFEu 3326u, 0xCFE, 0xCFE, 1, 0x301u, C, F, E
#define PPUTLIMPL_UHEX_0xCFDu 3325u, 0xCFD, 0xCFD, 1, 0x302u, C, F, D
#define PPUTLIMPL_UHEX_0xCFCu 3324u, 0xCFC, 0xCFC, 1, 0x303u, C, F, C
#define PPUTLIMPL_UHEX_0xCFBu 3323u, 0xCFB, 0xCFB, 1, 0x304u, C, F, B
#define PPUTLIMPL_UHEX_0xCFAu 3322u, 0xCFA, 0xCFA, 1, 0x305u, C, F, A
#define PPUTLIMPL_UHEX_0xCF9u 3321u, 0xCF9, 0xCF9, 1, 0x306u, C, F, 9
#define PPUTLIMPL_UHEX_0xCF8u 3320u, 0xCF8, 0xCF8, 1, 0x307u, C, F, 8
#define PPUTLIMPL_UHEX_0xCF7u 3319u, 0xCF7, 0xCF7, 1, 0x308u, C, F, 7
#define PPUTLIMPL_UHEX_0xCF6u 3318u, 0xCF6, 0xCF6, 1, 0x309u, C, F, 6
#define PPUTLIMPL_UHEX_0xCF5u 3317u, 0xCF5, 0xCF5, 1, 0x30Au, C, F, 5
#define PPUTLIMPL_UHEX_0xCF4u 3316u, 0xCF4, 0xCF4, 1, 0x30Bu, C, F, 4
#define PPUTLIMPL_UHEX_0xCF3u 3315u, 0xCF3, 0xCF3, 1, 0x30Cu, C, F, 3
#define PPUTLIMPL_UHEX_0xCF2u 3314u, 0xCF2, 0xCF2, 1, 0x30Du, C, F, 2
#define PPUTLIMPL_UHEX_0xCF1u 3313u, 0xCF1, 0xCF1, 1, 0x30Eu, C, F, 1
#define PPUTLIMPL_UHEX_0xCF0u 3312u, 0xCF0, 0xCF0, 1, 0x30Fu, C, F, 0
#define PPUTLIMPL_UHEX_0xCEFu 3311u, 0xCEF, 0xCEF, 1, 0x310u, C, E, F
#define PPUTLIMPL_UHEX_0xCEEu 3310u, 0xCEE, 0xCEE, 1, 0x311u, C, E, E
#define PPUTLIMPL_UHEX_0xCEDu 3309u, 0xCED, 0xCED, 1, 0x312u, C, E, D
#define PPUTLIMPL_UHEX_0xCECu 3308u, 0xCEC, 0xCEC, 1, 0x313u, C, E, C
#define PPUTLIMPL_UHEX_0xCEBu 3307u, 0xCEB, 0xCEB, 1, 0x314u, C, E, B
#define PPUTLIMPL_UHEX_0xCEAu 3306u, 0xCEA, 0xCEA, 1, 0x315u, C, E, A
#define PPUTLIMPL_UHEX_0xCE9u 3305u, 0xCE9, 0xCE9, 1, 0x316u, C, E, 9
#define PPUTLIMPL_UHEX_0xCE8u 3304u, 0xCE8, 0xCE8, 1, 0x317u, C, E, 8
#define PPUTLIMPL_UHEX_0xCE7u 3303u, 0xCE7, 0xCE7, 1, 0x318u, C, E, 7
#define PPUTLIMPL_UHEX_0xCE6u 3302u, 0xCE6, 0xCE6, 1, 0x319u, C, E, 6
#define PPUTLIMPL_UHEX_0xCE5u 3301u, 0xCE5, 0xCE5, 1, 0x31Au, C, E, 5
#define PPUTLIMPL_UHEX_0xCE4u 3300u, 0xCE4, 0xCE4, 1, 0x31Bu, C, E, 4
#define PPUTLIMPL_UHEX_0xCE3u 3299u, 0xCE3, 0xCE3, 1, 0x31Cu, C, E, 3
#define PPUTLIMPL_UHEX_0xCE2u 3298u, 0xCE2, 0xCE2, 1, 0x31Du, C, E, 2
#define PPUTLIMPL_UHEX_0xCE1u 3297u, 0xCE1, 0xCE1, 1, 0x31Eu, C, E, 1
#define PPUTLIMPL_UHEX_0xCE0u 3296u, 0xCE0, 0xCE0, 1, 0x31Fu, C, E, 0
#define PPUTLIMPL_UHEX_0xCDFu 3295u, 0xCDF, 0xCDF, 1, 0x320u, C, D, F
#define PPUTLIMPL_UHEX_0xCDEu 3294u, 0xCDE, 0xCDE, 1, 0x321u, C, D, E
#define PPUTLIMPL_UHEX_0xCDDu 3293u, 0xCDD, 0xCDD, 1, 0x322u, C, D, D
#define PPUTLIMPL_UHEX_0xCDCu 3292u, 0xCDC, 0xCDC, 1, 0x323u, C, D, C
#define PPUTLIMPL_UHEX_0xCDBu 3291u, 0xCDB, 0xCDB, 1, 0x324u, C, D, B
#define PPUTLIMPL_UHEX_0xCDAu 3290u, 0xCDA, 0xCDA, 1, 0x325u, C, D, A
#define PPUTLIMPL_UHEX_0xCD9u 3289u, 0xCD9, 0xCD9, 1, 0x326u, C, D, 9
#define PPUTLIMPL_UHEX_0xCD8u 3288u, 0xCD8, 0xCD8, 1, 0x327u, C, D, 8
#define PPUTLIMPL_UHEX_0xCD7u 3287u, 0xCD7, 0xCD7, 1, 0x328u, C, D, 7
#define PPUTLIMPL_UHEX_0xCD6u 3286u, 0xCD6, 0xCD6, 1, 0x329u, C, D, 6
#define PPUTLIMPL_UHEX_0xCD5u 3285u, 0xCD5, 0xCD5, 1, 0x32Au, C, D, 5
#define PPUTLIMPL_UHEX_0xCD4u 3284u, 0xCD4, 0xCD4, 1, 0x32Bu, C, D, 4
#define PPUTLIMPL_UHEX_0xCD3u 3283u, 0xCD3, 0xCD3, 1, 0x32Cu, C, D, 3
#define PPUTLIMPL_UHEX_0xCD2u 3282u, 0xCD2, 0xCD2, 1, 0x32Du, C, D, 2
#define PPUTLIMPL_UHEX_0xCD1u 3281u, 0xCD1, 0xCD1, 1, 0x32Eu, C, D, 1
#define PPUTLIMPL_UHEX_0xCD0u 3280u, 0xCD0, 0xCD0, 1, 0x32Fu, C, D, 0
#define PPUTLIMPL_UHEX_0xCCFu 3279u, 0xCCF, 0xCCF, 1, 0x330u, C, C, F
#define PPUTLIMPL_UHEX_0xCCEu 3278u, 0xCCE, 0xCCE, 1, 0x331u, C, C, E
#define PPUTLIMPL_UHEX_0xCCDu 3277u, 0xCCD, 0xCCD, 1, 0x332u, C, C, D
#define PPUTLIMPL_UHEX_0xCCCu 3276u, 0xCCC, 0xCCC, 1, 0x333u, C, C, C
#define PPUTLIMPL_UHEX_0xCCBu 3275u, 0xCCB, 0xCCB, 1, 0x334u, C, C, B
#define PPUTLIMPL_UHEX_0xCCAu 3274u, 0xCCA, 0xCCA, 1, 0x335u, C, C, A
#define PPUTLIMPL_UHEX_0xCC9u 3273u, 0xCC9, 0xCC9, 1, 0x336u, C, C, 9
#define PPUTLIMPL_UHEX_0xCC8u 3272u, 0xCC8, 0xCC8, 1, 0x337u, C, C, 8
#define PPUTLIMPL_UHEX_0xCC7u 3271u, 0xCC7, 0xCC7, 1, 0x338u, C, C, 7
#define PPUTLIMPL_UHEX_0xCC6u 3270u, 0xCC6, 0xCC6, 1, 0x339u, C, C, 6
#define PPUTLIMPL_UHEX_0xCC5u 3269u, 0xCC5, 0xCC5, 1, 0x33Au, C, C, 5
#define PPUTLIMPL_UHEX_0xCC4u 3268u, 0xCC4, 0xCC4, 1, 0x33Bu, C, C, 4
#define PPUTLIMPL_UHEX_0xCC3u 3267u, 0xCC3, 0xCC3, 1, 0x33Cu, C, C, 3
#define PPUTLIMPL_UHEX_0xCC2u 3266u, 0xCC2, 0xCC2, 1, 0x33Du, C, C, 2
#define PPUTLIMPL_UHEX_0xCC1u 3265u, 0xCC1, 0xCC1, 1, 0x33Eu, C, C, 1
#define PPUTLIMPL_UHEX_0xCC0u 3264u, 0xCC0, 0xCC0, 1, 0x33Fu, C, C, 0
#define PPUTLIMPL_UHEX_0xCBFu 3263u, 0xCBF, 0xCBF, 1, 0x340u, C, B, F
#define PPUTLIMPL_UHEX_0xCBEu 3262u, 0xCBE, 0xCBE, 1, 0x341u, C, B, E
#define PPUTLIMPL_UHEX_0xCBDu 3261u, 0xCBD, 0xCBD, 1, 0x342u, C, B, D
#define PPUTLIMPL_UHEX_0xCBCu 3260u, 0xCBC, 0xCBC, 1, 0x343u, C, B, C
#define PPUTLIMPL_UHEX_0xCBBu 3259u, 0xCBB, 0xCBB, 1, 0x344u, C, B, B
#define PPUTLIMPL_UHEX_0xCBAu 3258u, 0xCBA, 0xCBA, 1, 0x345u, C, B, A
#define PPUTLIMPL_UHEX_0xCB9u 3257u, 0xCB9, 0xCB9, 1, 0x346u, C, B, 9
#define PPUTLIMPL_UHEX_0xCB8u 3256u, 0xCB8, 0xCB8, 1, 0x347u, C, B, 8
#define PPUTLIMPL_UHEX_0xCB7u 3255u, 0xCB7, 0xCB7, 1, 0x348u, C, B, 7
#define PPUTLIMPL_UHEX_0xCB6u 3254u, 0xCB6, 0xCB6, 1, 0x349u, C, B, 6
#define PPUTLIMPL_UHEX_0xCB5u 3253u, 0xCB5, 0xCB5, 1, 0x34Au, C, B, 5
#define PPUTLIMPL_UHEX_0xCB4u 3252u, 0xCB4, 0xCB4, 1, 0x34Bu, C, B, 4
#define PPUTLIMPL_UHEX_0xCB3u 3251u, 0xCB3, 0xCB3, 1, 0x34Cu, C, B, 3
#define PPUTLIMPL_UHEX_0xCB2u 3250u, 0xCB2, 0xCB2, 1, 0x34Du, C, B, 2
#define PPUTLIMPL_UHEX_0xCB1u 3249u, 0xCB1, 0xCB1, 1, 0x34Eu, C, B, 1
#define PPUTLIMPL_UHEX_0xCB0u 3248u, 0xCB0, 0xCB0, 1, 0x34Fu, C, B, 0
#define PPUTLIMPL_UHEX_0xCAFu 3247u, 0xCAF, 0xCAF, 1, 0x350u, C, A, F
#define PPUTLIMPL_UHEX_0xCAEu 3246u, 0xCAE, 0xCAE, 1, 0x351u, C, A, E
#define PPUTLIMPL_UHEX_0xCADu 3245u, 0xCAD, 0xCAD, 1, 0x352u, C, A, D
#define PPUTLIMPL_UHEX_0xCACu 3244u, 0xCAC, 0xCAC, 1, 0x353u, C, A, C
#define PPUTLIMPL_UHEX_0xCABu 3243u, 0xCAB, 0xCAB, 1, 0x354u, C, A, B
#define PPUTLIMPL_UHEX_0xCAAu 3242u, 0xCAA, 0xCAA, 1, 0x355u, C, A, A
#define PPUTLIMPL_UHEX_0xCA9u 3241u, 0xCA9, 0xCA9, 1, 0x356u, C, A, 9
#define PPUTLIMPL_UHEX_0xCA8u 3240u, 0xCA8, 0xCA8, 1, 0x357u, C, A, 8
#define PPUTLIMPL_UHEX_0xCA7u 3239u, 0xCA7, 0xCA7, 1, 0x358u, C, A, 7
#define PPUTLIMPL_UHEX_0xCA6u 3238u, 0xCA6, 0xCA6, 1, 0x359u, C, A, 6
#define PPUTLIMPL_UHEX_0xCA5u 3237u, 0xCA5, 0xCA5, 1, 0x35Au, C, A, 5
#define PPUTLIMPL_UHEX_0xCA4u 3236u, 0xCA4, 0xCA4, 1, 0x35Bu, C, A, 4
#define PPUTLIMPL_UHEX_0xCA3u 3235u, 0xCA3, 0xCA3, 1, 0x35Cu, C, A, 3
#define PPUTLIMPL_UHEX_0xCA2u 3234u, 0xCA2, 0xCA2, 1, 0x35Du, C, A, 2
#define PPUTLIMPL_UHEX_0xCA1u 3233u, 0xCA1, 0xCA1, 1, 0x35Eu, C, A, 1
#define PPUTLIMPL_UHEX_0xCA0u 3232u, 0xCA0, 0xCA0, 1, 0x35Fu, C, A, 0
#define PPUTLIMPL_UHEX_0xC9Fu 3231u, 0xC9F, 0xC9F, 1, 0x360u, C, 9, F
#define PPUTLIMPL_UHEX_0xC9Eu 3230u, 0xC9E, 0xC9E, 1, 0x361u, C, 9, E
#define PPUTLIMPL_UHEX_0xC9Du 3229u, 0xC9D, 0xC9D, 1, 0x362u, C, 9, D
#define PPUTLIMPL_UHEX_0xC9Cu 3228u, 0xC9C, 0xC9C, 1, 0x363u, C, 9, C
#define PPUTLIMPL_UHEX_0xC9Bu 3227u, 0xC9B, 0xC9B, 1, 0x364u, C, 9, B
#define PPUTLIMPL_UHEX_0xC9Au 3226u, 0xC9A, 0xC9A, 1, 0x365u, C, 9, A
#define PPUTLIMPL_UHEX_0xC99u 3225u, 0xC99, 0xC99, 1, 0x366u, C, 9, 9
#define PPUTLIMPL_UHEX_0xC98u 3224u, 0xC98, 0xC98, 1, 0x367u, C, 9, 8
#define PPUTLIMPL_UHEX_0xC97u 3223u, 0xC97, 0xC97, 1, 0x368u, C, 9, 7
#define PPUTLIMPL_UHEX_0xC96u 3222u, 0xC96, 0xC96, 1, 0x369u, C, 9, 6
#define PPUTLIMPL_UHEX_0xC95u 3221u, 0xC95, 0xC95, 1, 0x36Au, C, 9, 5
#define PPUTLIMPL_UHEX_0xC94u 3220u, 0xC94, 0xC94, 1, 0x36Bu, C, 9, 4
#define PPUTLIMPL_UHEX_0xC93u 3219u, 0xC93, 0xC93, 1, 0x36Cu, C, 9, 3
#define PPUTLIMPL_UHEX_0xC92u 3218u, 0xC92, 0xC92, 1, 0x36Du, C, 9, 2
#define PPUTLIMPL_UHEX_0xC91u 3217u, 0xC91, 0xC91, 1, 0x36Eu, C, 9, 1
#define PPUTLIMPL_UHEX_0xC90u 3216u, 0xC90, 0xC90, 1, 0x36Fu, C, 9, 0
#define PPUTLIMPL_UHEX_0xC8Fu 3215u, 0xC8F, 0xC8F, 1, 0x370u, C, 8, F
#define PPUTLIMPL_UHEX_0xC8Eu 3214u, 0xC8E, 0xC8E, 1, 0x371u, C, 8, E
#define PPUTLIMPL_UHEX_0xC8Du 3213u, 0xC8D, 0xC8D, 1, 0x372u, C, 8, D
#define PPUTLIMPL_UHEX_0xC8Cu 3212u, 0xC8C, 0xC8C, 1, 0x373u, C, 8, C
#define PPUTLIMPL_UHEX_0xC8Bu 3211u, 0xC8B, 0xC8B, 1, 0x374u, C, 8, B
#define PPUTLIMPL_UHEX_0xC8Au 3210u, 0xC8A, 0xC8A, 1, 0x375u, C, 8, A
#define PPUTLIMPL_UHEX_0xC89u 3209u, 0xC89, 0xC89, 1, 0x376u, C, 8, 9
#define PPUTLIMPL_UHEX_0xC88u 3208u, 0xC88, 0xC88, 1, 0x377u, C, 8, 8
#define PPUTLIMPL_UHEX_0xC87u 3207u, 0xC87, 0xC87, 1, 0x378u, C, 8, 7
#define PPUTLIMPL_UHEX_0xC86u 3206u, 0xC86, 0xC86, 1, 0x379u, C, 8, 6
#define PPUTLIMPL_UHEX_0xC85u 3205u, 0xC85, 0xC85, 1, 0x37Au, C, 8, 5
#define PPUTLIMPL_UHEX_0xC84u 3204u, 0xC84, 0xC84, 1, 0x37Bu, C, 8, 4
#define PPUTLIMPL_UHEX_0xC83u 3203u, 0xC83, 0xC83, 1, 0x37Cu, C, 8, 3
#define PPUTLIMPL_UHEX_0xC82u 3202u, 0xC82, 0xC82, 1, 0x37Du, C, 8, 2
#define PPUTLIMPL_UHEX_0xC81u 3201u, 0xC81, 0xC81, 1, 0x37Eu, C, 8, 1
#define PPUTLIMPL_UHEX_0xC80u 3200u, 0xC80, 0xC80, 1, 0x37Fu, C, 8, 0
#define PPUTLIMPL_UHEX_0xC7Fu 3199u, 0xC7F, 0xC7F, 1, 0x380u, C, 7, F
#define PPUTLIMPL_UHEX_0xC7Eu 3198u, 0xC7E, 0xC7E, 1, 0x381u, C, 7, E
#define PPUTLIMPL_UHEX_0xC7Du 3197u, 0xC7D, 0xC7D, 1, 0x382u, C, 7, D
#define PPUTLIMPL_UHEX_0xC7Cu 3196u, 0xC7C, 0xC7C, 1, 0x383u, C, 7, C
#define PPUTLIMPL_UHEX_0xC7Bu 3195u, 0xC7B, 0xC7B, 1, 0x384u, C, 7, B
#define PPUTLIMPL_UHEX_0xC7Au 3194u, 0xC7A, 0xC7A, 1, 0x385u, C, 7, A
#define PPUTLIMPL_UHEX_0xC79u 3193u, 0xC79, 0xC79, 1, 0x386u, C, 7, 9
#define PPUTLIMPL_UHEX_0xC78u 3192u, 0xC78, 0xC78, 1, 0x387u, C, 7, 8
#define PPUTLIMPL_UHEX_0xC77u 3191u, 0xC77, 0xC77, 1, 0x388u, C, 7, 7
#define PPUTLIMPL_UHEX_0xC76u 3190u, 0xC76, 0xC76, 1, 0x389u, C, 7, 6
#define PPUTLIMPL_UHEX_0xC75u 3189u, 0xC75, 0xC75, 1, 0x38Au, C, 7, 5
#define PPUTLIMPL_UHEX_0xC74u 3188u, 0xC74, 0xC74, 1, 0x38Bu, C, 7, 4
#define PPUTLIMPL_UHEX_0xC73u 3187u, 0xC73, 0xC73, 1, 0x38Cu, C, 7, 3
#define PPUTLIMPL_UHEX_0xC72u 3186u, 0xC72, 0xC72, 1, 0x38Du, C, 7, 2
#define PPUTLIMPL_UHEX_0xC71u 3185u, 0xC71, 0xC71, 1, 0x38Eu, C, 7, 1
#define PPUTLIMPL_UHEX_0xC70u 3184u, 0xC70, 0xC70, 1, 0x38Fu, C, 7, 0
#define PPUTLIMPL_UHEX_0xC6Fu 3183u, 0xC6F, 0xC6F, 1, 0x390u, C, 6, F
#define PPUTLIMPL_UHEX_0xC6Eu 3182u, 0xC6E, 0xC6E, 1, 0x391u, C, 6, E
#define PPUTLIMPL_UHEX_0xC6Du 3181u, 0xC6D, 0xC6D, 1, 0x392u, C, 6, D
#define PPUTLIMPL_UHEX_0xC6Cu 3180u, 0xC6C, 0xC6C, 1, 0x393u, C, 6, C
#define PPUTLIMPL_UHEX_0xC6Bu 3179u, 0xC6B, 0xC6B, 1, 0x394u, C, 6, B
#define PPUTLIMPL_UHEX_0xC6Au 3178u, 0xC6A, 0xC6A, 1, 0x395u, C, 6, A
#define PPUTLIMPL_UHEX_0xC69u 3177u, 0xC69, 0xC69, 1, 0x396u, C, 6, 9
#define PPUTLIMPL_UHEX_0xC68u 3176u, 0xC68, 0xC68, 1, 0x397u, C, 6, 8
#define PPUTLIMPL_UHEX_0xC67u 3175u, 0xC67, 0xC67, 1, 0x398u, C, 6, 7
#define PPUTLIMPL_UHEX_0xC66u 3174u, 0xC66, 0xC66, 1, 0x399u, C, 6, 6
#define PPUTLIMPL_UHEX_0xC65u 3173u, 0xC65, 0xC65, 1, 0x39Au, C, 6, 5
#define PPUTLIMPL_UHEX_0xC64u 3172u, 0xC64, 0xC64, 1, 0x39Bu, C, 6, 4
#define PPUTLIMPL_UHEX_0xC63u 3171u, 0xC63, 0xC63, 1, 0x39Cu, C, 6, 3
#define PPUTLIMPL_UHEX_0xC62u 3170u, 0xC62, 0xC62, 1, 0x39Du, C, 6, 2
#define PPUTLIMPL_UHEX_0xC61u 3169u, 0xC61, 0xC61, 1, 0x39Eu, C, 6, 1
#define PPUTLIMPL_UHEX_0xC60u 3168u, 0xC60, 0xC60, 1, 0x39Fu, C, 6, 0
#define PPUTLIMPL_UHEX_0xC5Fu 3167u, 0xC5F, 0xC5F, 1, 0x3A0u, C, 5, F
#define PPUTLIMPL_UHEX_0xC5Eu 3166u, 0xC5E, 0xC5E, 1, 0x3A1u, C, 5, E
#define PPUTLIMPL_UHEX_0xC5Du 3165u, 0xC5D, 0xC5D, 1, 0x3A2u, C, 5, D
#define PPUTLIMPL_UHEX_0xC5Cu 3164u, 0xC5C, 0xC5C, 1, 0x3A3u, C, 5, C
#define PPUTLIMPL_UHEX_0xC5Bu 3163u, 0xC5B, 0xC5B, 1, 0x3A4u, C, 5, B
#define PPUTLIMPL_UHEX_0xC5Au 3162u, 0xC5A, 0xC5A, 1, 0x3A5u, C, 5, A
#define PPUTLIMPL_UHEX_0xC59u 3161u, 0xC59, 0xC59, 1, 0x3A6u, C, 5, 9
#define PPUTLIMPL_UHEX_0xC58u 3160u, 0xC58, 0xC58, 1, 0x3A7u, C, 5, 8
#define PPUTLIMPL_UHEX_0xC57u 3159u, 0xC57, 0xC57, 1, 0x3A8u, C, 5, 7
#define PPUTLIMPL_UHEX_0xC56u 3158u, 0xC56, 0xC56, 1, 0x3A9u, C, 5, 6
#define PPUTLIMPL_UHEX_0xC55u 3157u, 0xC55, 0xC55, 1, 0x3AAu, C, 5, 5
#define PPUTLIMPL_UHEX_0xC54u 3156u, 0xC54, 0xC54, 1, 0x3ABu, C, 5, 4
#define PPUTLIMPL_UHEX_0xC53u 3155u, 0xC53, 0xC53, 1, 0x3ACu, C, 5, 3
#define PPUTLIMPL_UHEX_0xC52u 3154u, 0xC52, 0xC52, 1, 0x3ADu, C, 5, 2
#define PPUTLIMPL_UHEX_0xC51u 3153u, 0xC51, 0xC51, 1, 0x3AEu, C, 5, 1
#define PPUTLIMPL_UHEX_0xC50u 3152u, 0xC50, 0xC50, 1, 0x3AFu, C, 5, 0
#define PPUTLIMPL_UHEX_0xC4Fu 3151u, 0xC4F, 0xC4F, 1, 0x3B0u, C, 4, F
#define PPUTLIMPL_UHEX_0xC4Eu 3150u, 0xC4E, 0xC4E, 1, 0x3B1u, C, 4, E
#define PPUTLIMPL_UHEX_0xC4Du 3149u, 0xC4D, 0xC4D, 1, 0x3B2u, C, 4, D
#define PPUTLIMPL_UHEX_0xC4Cu 3148u, 0xC4C, 0xC4C, 1, 0x3B3u, C, 4, C
#define PPUTLIMPL_UHEX_0xC4Bu 3147u, 0xC4B, 0xC4B, 1, 0x3B4u, C, 4, B
#define PPUTLIMPL_UHEX_0xC4Au 3146u, 0xC4A, 0xC4A, 1, 0x3B5u, C, 4, A
#define PPUTLIMPL_UHEX_0xC49u 3145u, 0xC49, 0xC49, 1, 0x3B6u, C, 4, 9
#define PPUTLIMPL_UHEX_0xC48u 3144u, 0xC48, 0xC48, 1, 0x3B7u, C, 4, 8
#define PPUTLIMPL_UHEX_0xC47u 3143u, 0xC47, 0xC47, 1, 0x3B8u, C, 4, 7
#define PPUTLIMPL_UHEX_0xC46u 3142u, 0xC46, 0xC46, 1, 0x3B9u, C, 4, 6
#define PPUTLIMPL_UHEX_0xC45u 3141u, 0xC45, 0xC45, 1, 0x3BAu, C, 4, 5
#define PPUTLIMPL_UHEX_0xC44u 3140u, 0xC44, 0xC44, 1, 0x3BBu, C, 4, 4
#define PPUTLIMPL_UHEX_0xC43u 3139u, 0xC43, 0xC43, 1, 0x3BCu, C, 4, 3
#define PPUTLIMPL_UHEX_0xC42u 3138u, 0xC42, 0xC42, 1, 0x3BDu, C, 4, 2
#define PPUTLIMPL_UHEX_0xC41u 3137u, 0xC41, 0xC41, 1, 0x3BEu, C, 4, 1
#define PPUTLIMPL_UHEX_0xC40u 3136u, 0xC40, 0xC40, 1, 0x3BFu, C, 4, 0
#define PPUTLIMPL_UHEX_0xC3Fu 3135u, 0xC3F, 0xC3F, 1, 0x3C0u, C, 3, F
#define PPUTLIMPL_UHEX_0xC3Eu 3134u, 0xC3E, 0xC3E, 1, 0x3C1u, C, 3, E
#define PPUTLIMPL_UHEX_0xC3Du 3133u, 0xC3D, 0xC3D, 1, 0x3C2u, C, 3, D
#define PPUTLIMPL_UHEX_0xC3Cu 3132u, 0xC3C, 0xC3C, 1, 0x3C3u, C, 3, C
#define PPUTLIMPL_UHEX_0xC3Bu 3131u, 0xC3B, 0xC3B, 1, 0x3C4u, C, 3, B
#define PPUTLIMPL_UHEX_0xC3Au 3130u, 0xC3A, 0xC3A, 1, 0x3C5u, C, 3, A
#define PPUTLIMPL_UHEX_0xC39u 3129u, 0xC39, 0xC39, 1, 0x3C6u, C, 3, 9
#define PPUTLIMPL_UHEX_0xC38u 3128u, 0xC38, 0xC38, 1, 0x3C7u, C, 3, 8
#define PPUTLIMPL_UHEX_0xC37u 3127u, 0xC37, 0xC37, 1, 0x3C8u, C, 3, 7
#define PPUTLIMPL_UHEX_0xC36u 3126u, 0xC36, 0xC36, 1, 0x3C9u, C, 3, 6
#define PPUTLIMPL_UHEX_0xC35u 3125u, 0xC35, 0xC35, 1, 0x3CAu, C, 3, 5
#define PPUTLIMPL_UHEX_0xC34u 3124u, 0xC34, 0xC34, 1, 0x3CBu, C, 3, 4
#define PPUTLIMPL_UHEX_0xC33u 3123u, 0xC33, 0xC33, 1, 0x3CCu, C, 3, 3
#define PPUTLIMPL_UHEX_0xC32u 3122u, 0xC32, 0xC32, 1, 0x3CDu, C, 3, 2
#define PPUTLIMPL_UHEX_0xC31u 3121u, 0xC31, 0xC31, 1, 0x3CEu, C, 3, 1
#define PPUTLIMPL_UHEX_0xC30u 3120u, 0xC30, 0xC30, 1, 0x3CFu, C, 3, 0
#define PPUTLIMPL_UHEX_0xC2Fu 3119u, 0xC2F, 0xC2F, 1, 0x3D0u, C, 2, F
#define PPUTLIMPL_UHEX_0xC2Eu 3118u, 0xC2E, 0xC2E, 1, 0x3D1u, C, 2, E
#define PPUTLIMPL_UHEX_0xC2Du 3117u, 0xC2D, 0xC2D, 1, 0x3D2u, C, 2, D
#define PPUTLIMPL_UHEX_0xC2Cu 3116u, 0xC2C, 0xC2C, 1, 0x3D3u, C, 2, C
#define PPUTLIMPL_UHEX_0xC2Bu 3115u, 0xC2B, 0xC2B, 1, 0x3D4u, C, 2, B
#define PPUTLIMPL_UHEX_0xC2Au 3114u, 0xC2A, 0xC2A, 1, 0x3D5u, C, 2, A
#define PPUTLIMPL_UHEX_0xC29u 3113u, 0xC29, 0xC29, 1, 0x3D6u, C, 2, 9
#define PPUTLIMPL_UHEX_0xC28u 3112u, 0xC28, 0xC28, 1, 0x3D7u, C, 2, 8
#define PPUTLIMPL_UHEX_0xC27u 3111u, 0xC27, 0xC27, 1, 0x3D8u, C, 2, 7
#define PPUTLIMPL_UHEX_0xC26u 3110u, 0xC26, 0xC26, 1, 0x3D9u, C, 2, 6
#define PPUTLIMPL_UHEX_0xC25u 3109u, 0xC25, 0xC25, 1, 0x3DAu, C, 2, 5
#define PPUTLIMPL_UHEX_0xC24u 3108u, 0xC24, 0xC24, 1, 0x3DBu, C, 2, 4
#define PPUTLIMPL_UHEX_0xC23u 3107u, 0xC23, 0xC23, 1, 0x3DCu, C, 2, 3
#define PPUTLIMPL_UHEX_0xC22u 3106u, 0xC22, 0xC22, 1, 0x3DDu, C, 2, 2
#define PPUTLIMPL_UHEX_0xC21u 3105u, 0xC21, 0xC21, 1, 0x3DEu, C, 2, 1
#define PPUTLIMPL_UHEX_0xC20u 3104u, 0xC20, 0xC20, 1, 0x3DFu, C, 2, 0
#define PPUTLIMPL_UHEX_0xC1Fu 3103u, 0xC1F, 0xC1F, 1, 0x3E0u, C, 1, F
#define PPUTLIMPL_UHEX_0xC1Eu 3102u, 0xC1E, 0xC1E, 1, 0x3E1u, C, 1, E
#define PPUTLIMPL_UHEX_0xC1Du 3101u, 0xC1D, 0xC1D, 1, 0x3E2u, C, 1, D
#define PPUTLIMPL_UHEX_0xC1Cu 3100u, 0xC1C, 0xC1C, 1, 0x3E3u, C, 1, C
#define PPUTLIMPL_UHEX_0xC1Bu 3099u, 0xC1B, 0xC1B, 1, 0x3E4u, C, 1, B
#define PPUTLIMPL_UHEX_0xC1Au 3098u, 0xC1A, 0xC1A, 1, 0x3E5u, C, 1, A
#define PPUTLIMPL_UHEX_0xC19u 3097u, 0xC19, 0xC19, 1, 0x3E6u, C, 1, 9
#define PPUTLIMPL_UHEX_0xC18u 3096u, 0xC18, 0xC18, 1, 0x3E7u, C, 1, 8
#define PPUTLIMPL_UHEX_0xC17u 3095u, 0xC17, 0xC17, 1, 0x3E8u, C, 1, 7
#define PPUTLIMPL_UHEX_0xC16u 3094u, 0xC16, 0xC16, 1, 0x3E9u, C, 1, 6
#define PPUTLIMPL_UHEX_0xC15u 3093u, 0xC15, 0xC15, 1, 0x3EAu, C, 1, 5
#define PPUTLIMPL_UHEX_0xC14u 3092u, 0xC14, 0xC14, 1, 0x3EBu, C, 1, 4
#define PPUTLIMPL_UHEX_0xC13u 3091u, 0xC13, 0xC13, 1, 0x3ECu, C, 1, 3
#define PPUTLIMPL_UHEX_0xC12u 3090u, 0xC12, 0xC12, 1, 0x3EDu, C, 1, 2
#define PPUTLIMPL_UHEX_0xC11u 3089u, 0xC11, 0xC11, 1, 0x3EEu, C, 1, 1
#define PPUTLIMPL_UHEX_0xC10u 3088u, 0xC10, 0xC10, 1, 0x3EFu, C, 1, 0
#define PPUTLIMPL_UHEX_0xC0Fu 3087u, 0xC0F, 0xC0F, 1, 0x3F0u, C, 0, F
#define PPUTLIMPL_UHEX_0xC0Eu 3086u, 0xC0E, 0xC0E, 1, 0x3F1u, C, 0, E
#define PPUTLIMPL_UHEX_0xC0Du 3085u, 0xC0D, 0xC0D, 1, 0x3F2u, C, 0, D
#define PPUTLIMPL_UHEX_0xC0Cu 3084u, 0xC0C, 0xC0C, 1, 0x3F3u, C, 0, C
#define PPUTLIMPL_UHEX_0xC0Bu 3083u, 0xC0B, 0xC0B, 1, 0x3F4u, C, 0, B
#define PPUTLIMPL_UHEX_0xC0Au 3082u, 0xC0A, 0xC0A, 1, 0x3F5u, C, 0, A
#define PPUTLIMPL_UHEX_0xC09u 3081u, 0xC09, 0xC09, 1, 0x3F6u, C, 0, 9
#define PPUTLIMPL_UHEX_0xC08u 3080u, 0xC08, 0xC08, 1, 0x3F7u, C, 0, 8
#define PPUTLIMPL_UHEX_0xC07u 3079u, 0xC07, 0xC07, 1, 0x3F8u, C, 0, 7
#define PPUTLIMPL_UHEX_0xC06u 3078u, 0xC06, 0xC06, 1, 0x3F9u, C, 0, 6
#define PPUTLIMPL_UHEX_0xC05u 3077u, 0xC05, 0xC05, 1, 0x3FAu, C, 0, 5
#define PPUTLIMPL_UHEX_0xC04u 3076u, 0xC04, 0xC04, 1, 0x3FBu, C, 0, 4
#define PPUTLIMPL_UHEX_0xC03u 3075u, 0xC03, 0xC03, 1, 0x3FCu, C, 0, 3
#define PPUTLIMPL_UHEX_0xC02u 3074u, 0xC02, 0xC02, 1, 0x3FDu, C, 0, 2
#define PPUTLIMPL_UHEX_0xC01u 3073u, 0xC01, 0xC01, 1, 0x3FEu, C, 0, 1
#define PPUTLIMPL_UHEX_0xC00u 3072u, 0xC00, 0xC00, 1, 0x3FFu, C, 0, 0
#define PPUTLIMPL_UHEX_0xBFFu 3071u, 0xBFF, 0xBFF, 1, 0x400u, B, F, F
#define PPUTLIMPL_UHEX_0xBFEu 3070u, 0xBFE, 0xBFE, 1, 0x401u, B, F, E
#define PPUTLIMPL_UHEX_0xBFDu 3069u, 0xBFD, 0xBFD, 1, 0x402u, B, F, D
#define PPUTLIMPL_UHEX_0xBFCu 3068u, 0xBFC, 0xBFC, 1, 0x403u, B, F, C
#define PPUTLIMPL_UHEX_0xBFBu 3067u, 0xBFB, 0xBFB, 1, 0x404u, B, F, B
#define PPUTLIMPL_UHEX_0xBFAu 3066u, 0xBFA, 0xBFA, 1, 0x405u, B, F, A
#define PPUTLIMPL_UHEX_0xBF9u 3065u, 0xBF9, 0xBF9, 1, 0x406u, B, F, 9
#define PPUTLIMPL_UHEX_0xBF8u 3064u, 0xBF8, 0xBF8, 1, 0x407u, B, F, 8
#define PPUTLIMPL_UHEX_0xBF7u 3063u, 0xBF7, 0xBF7, 1, 0x408u, B, F, 7
#define PPUTLIMPL_UHEX_0xBF6u 3062u, 0xBF6, 0xBF6, 1, 0x409u, B, F, 6
#define PPUTLIMPL_UHEX_0xBF5u 3061u, 0xBF5, 0xBF5, 1, 0x40Au, B, F, 5
#define PPUTLIMPL_UHEX_0xBF4u 3060u, 0xBF4, 0xBF4, 1, 0x40Bu, B, F, 4
#define PPUTLIMPL_UHEX_0xBF3u 3059u, 0xBF3, 0xBF3, 1, 0x40Cu, B, F, 3
#define PPUTLIMPL_UHEX_0xBF2u 3058u, 0xBF2, 0xBF2, 1, 0x40Du, B, F, 2
#define PPUTLIMPL_UHEX_0xBF1u 3057u, 0xBF1, 0xBF1, 1, 0x40Eu, B, F, 1
#define PPUTLIMPL_UHEX_0xBF0u 3056u, 0xBF0, 0xBF0, 1, 0x40Fu, B, F, 0
#define PPUTLIMPL_UHEX_0xBEFu 3055u, 0xBEF, 0xBEF, 1, 0x410u, B, E, F
#define PPUTLIMPL_UHEX_0xBEEu 3054u, 0xBEE, 0xBEE, 1, 0x411u, B, E, E
#define PPUTLIMPL_UHEX_0xBEDu 3053u, 0xBED, 0xBED, 1, 0x412u, B, E, D
#define PPUTLIMPL_UHEX_0xBECu 3052u, 0xBEC, 0xBEC, 1, 0x413u, B, E, C
#define PPUTLIMPL_UHEX_0xBEBu 3051u, 0xBEB, 0xBEB, 1, 0x414u, B, E, B
#define PPUTLIMPL_UHEX_0xBEAu 3050u, 0xBEA, 0xBEA, 1, 0x415u, B, E, A
#define PPUTLIMPL_UHEX_0xBE9u 3049u, 0xBE9, 0xBE9, 1, 0x416u, B, E, 9
#define PPUTLIMPL_UHEX_0xBE8u 3048u, 0xBE8, 0xBE8, 1, 0x417u, B, E, 8
#define PPUTLIMPL_UHEX_0xBE7u 3047u, 0xBE7, 0xBE7, 1, 0x418u, B, E, 7
#define PPUTLIMPL_UHEX_0xBE6u 3046u, 0xBE6, 0xBE6, 1, 0x419u, B, E, 6
#define PPUTLIMPL_UHEX_0xBE5u 3045u, 0xBE5, 0xBE5, 1, 0x41Au, B, E, 5
#define PPUTLIMPL_UHEX_0xBE4u 3044u, 0xBE4, 0xBE4, 1, 0x41Bu, B, E, 4
#define PPUTLIMPL_UHEX_0xBE3u 3043u, 0xBE3, 0xBE3, 1, 0x41Cu, B, E, 3
#define PPUTLIMPL_UHEX_0xBE2u 3042u, 0xBE2, 0xBE2, 1, 0x41Du, B, E, 2
#define PPUTLIMPL_UHEX_0xBE1u 3041u, 0xBE1, 0xBE1, 1, 0x41Eu, B, E, 1
#define PPUTLIMPL_UHEX_0xBE0u 3040u, 0xBE0, 0xBE0, 1, 0x41Fu, B, E, 0
#define PPUTLIMPL_UHEX_0xBDFu 3039u, 0xBDF, 0xBDF, 1, 0x420u, B, D, F
#define PPUTLIMPL_UHEX_0xBDEu 3038u, 0xBDE, 0xBDE, 1, 0x421u, B, D, E
#define PPUTLIMPL_UHEX_0xBDDu 3037u, 0xBDD, 0xBDD, 1, 0x422u, B, D, D
#define PPUTLIMPL_UHEX_0xBDCu 3036u, 0xBDC, 0xBDC, 1, 0x423u, B, D, C
#define PPUTLIMPL_UHEX_0xBDBu 3035u, 0xBDB, 0xBDB, 1, 0x424u, B, D, B
#define PPUTLIMPL_UHEX_0xBDAu 3034u, 0xBDA, 0xBDA, 1, 0x425u, B, D, A
#define PPUTLIMPL_UHEX_0xBD9u 3033u, 0xBD9, 0xBD9, 1, 0x426u, B, D, 9
#define PPUTLIMPL_UHEX_0xBD8u 3032u, 0xBD8, 0xBD8, 1, 0x427u, B, D, 8
#define PPUTLIMPL_UHEX_0xBD7u 3031u, 0xBD7, 0xBD7, 1, 0x428u, B, D, 7
#define PPUTLIMPL_UHEX_0xBD6u 3030u, 0xBD6, 0xBD6, 1, 0x429u, B, D, 6
#define PPUTLIMPL_UHEX_0xBD5u 3029u, 0xBD5, 0xBD5, 1, 0x42Au, B, D, 5
#define PPUTLIMPL_UHEX_0xBD4u 3028u, 0xBD4, 0xBD4, 1, 0x42Bu, B, D, 4
#define PPUTLIMPL_UHEX_0xBD3u 3027u, 0xBD3, 0xBD3, 1, 0x42Cu, B, D, 3
#define PPUTLIMPL_UHEX_0xBD2u 3026u, 0xBD2, 0xBD2, 1, 0x42Du, B, D, 2
#define PPUTLIMPL_UHEX_0xBD1u 3025u, 0xBD1, 0xBD1, 1, 0x42Eu, B, D, 1
#define PPUTLIMPL_UHEX_0xBD0u 3024u, 0xBD0, 0xBD0, 1, 0x42Fu, B, D, 0
#define PPUTLIMPL_UHEX_0xBCFu 3023u, 0xBCF, 0xBCF, 1, 0x430u, B, C, F
#define PPUTLIMPL_UHEX_0xBCEu 3022u, 0xBCE, 0xBCE, 1, 0x431u, B, C, E
#define PPUTLIMPL_UHEX_0xBCDu 3021u, 0xBCD, 0xBCD, 1, 0x432u, B, C, D
#define PPUTLIMPL_UHEX_0xBCCu 3020u, 0xBCC, 0xBCC, 1, 0x433u, B, C, C
#define PPUTLIMPL_UHEX_0xBCBu 3019u, 0xBCB, 0xBCB, 1, 0x434u, B, C, B
#define PPUTLIMPL_UHEX_0xBCAu 3018u, 0xBCA, 0xBCA, 1, 0x435u, B, C, A
#define PPUTLIMPL_UHEX_0xBC9u 3017u, 0xBC9, 0xBC9, 1, 0x436u, B, C, 9
#define PPUTLIMPL_UHEX_0xBC8u 3016u, 0xBC8, 0xBC8, 1, 0x437u, B, C, 8
#define PPUTLIMPL_UHEX_0xBC7u 3015u, 0xBC7, 0xBC7, 1, 0x438u, B, C, 7
#define PPUTLIMPL_UHEX_0xBC6u 3014u, 0xBC6, 0xBC6, 1, 0x439u, B, C, 6
#define PPUTLIMPL_UHEX_0xBC5u 3013u, 0xBC5, 0xBC5, 1, 0x43Au, B, C, 5
#define PPUTLIMPL_UHEX_0xBC4u 3012u, 0xBC4, 0xBC4, 1, 0x43Bu, B, C, 4
#define PPUTLIMPL_UHEX_0xBC3u 3011u, 0xBC3, 0xBC3, 1, 0x43Cu, B, C, 3
#define PPUTLIMPL_UHEX_0xBC2u 3010u, 0xBC2, 0xBC2, 1, 0x43Du, B, C, 2
#define PPUTLIMPL_UHEX_0xBC1u 3009u, 0xBC1, 0xBC1, 1, 0x43Eu, B, C, 1
#define PPUTLIMPL_UHEX_0xBC0u 3008u, 0xBC0, 0xBC0, 1, 0x43Fu, B, C, 0
#define PPUTLIMPL_UHEX_0xBBFu 3007u, 0xBBF, 0xBBF, 1, 0x440u, B, B, F
#define PPUTLIMPL_UHEX_0xBBEu 3006u, 0xBBE, 0xBBE, 1, 0x441u, B, B, E
#define PPUTLIMPL_UHEX_0xBBDu 3005u, 0xBBD, 0xBBD, 1, 0x442u, B, B, D
#define PPUTLIMPL_UHEX_0xBBCu 3004u, 0xBBC, 0xBBC, 1, 0x443u, B, B, C
#define PPUTLIMPL_UHEX_0xBBBu 3003u, 0xBBB, 0xBBB, 1, 0x444u, B, B, B
#define PPUTLIMPL_UHEX_0xBBAu 3002u, 0xBBA, 0xBBA, 1, 0x445u, B, B, A
#define PPUTLIMPL_UHEX_0xBB9u 3001u, 0xBB9, 0xBB9, 1, 0x446u, B, B, 9
#define PPUTLIMPL_UHEX_0xBB8u 3000u, 0xBB8, 0xBB8, 1, 0x447u, B, B, 8
#define PPUTLIMPL_UHEX_0xBB7u 2999u, 0xBB7, 0xBB7, 1, 0x448u, B, B, 7
#define PPUTLIMPL_UHEX_0xBB6u 2998u, 0xBB6, 0xBB6, 1, 0x449u, B, B, 6
#define PPUTLIMPL_UHEX_0xBB5u 2997u, 0xBB5, 0xBB5, 1, 0x44Au, B, B, 5
#define PPUTLIMPL_UHEX_0xBB4u 2996u, 0xBB4, 0xBB4, 1, 0x44Bu, B, B, 4
#define PPUTLIMPL_UHEX_0xBB3u 2995u, 0xBB3, 0xBB3, 1, 0x44Cu, B, B, 3
#define PPUTLIMPL_UHEX_0xBB2u 2994u, 0xBB2, 0xBB2, 1, 0x44Du, B, B, 2
#define PPUTLIMPL_UHEX_0xBB1u 2993u, 0xBB1, 0xBB1, 1, 0x44Eu, B, B, 1
#define PPUTLIMPL_UHEX_0xBB0u 2992u, 0xBB0, 0xBB0, 1, 0x44Fu, B, B, 0
#define PPUTLIMPL_UHEX_0xBAFu 2991u, 0xBAF, 0xBAF, 1, 0x450u, B, A, F
#define PPUTLIMPL_UHEX_0xBAEu 2990u, 0xBAE, 0xBAE, 1, 0x451u, B, A, E
#define PPUTLIMPL_UHEX_0xBADu 2989u, 0xBAD, 0xBAD, 1, 0x452u, B, A, D
#define PPUTLIMPL_UHEX_0xBACu 2988u, 0xBAC, 0xBAC, 1, 0x453u, B, A, C
#define PPUTLIMPL_UHEX_0xBABu 2987u, 0xBAB, 0xBAB, 1, 0x454u, B, A, B
#define PPUTLIMPL_UHEX_0xBAAu 2986u, 0xBAA, 0xBAA, 1, 0x455u, B, A, A
#define PPUTLIMPL_UHEX_0xBA9u 2985u, 0xBA9, 0xBA9, 1, 0x456u, B, A, 9
#define PPUTLIMPL_UHEX_0xBA8u 2984u, 0xBA8, 0xBA8, 1, 0x457u, B, A, 8
#define PPUTLIMPL_UHEX_0xBA7u 2983u, 0xBA7, 0xBA7, 1, 0x458u, B, A, 7
#define PPUTLIMPL_UHEX_0xBA6u 2982u, 0xBA6, 0xBA6, 1, 0x459u, B, A, 6
#define PPUTLIMPL_UHEX_0xBA5u 2981u, 0xBA5, 0xBA5, 1, 0x45Au, B, A, 5
#define PPUTLIMPL_UHEX_0xBA4u 2980u, 0xBA4, 0xBA4, 1, 0x45Bu, B, A, 4
#define PPUTLIMPL_UHEX_0xBA3u 2979u, 0xBA3, 0xBA3, 1, 0x45Cu, B, A, 3
#define PPUTLIMPL_UHEX_0xBA2u 2978u, 0xBA2, 0xBA2, 1, 0x45Du, B, A, 2
#define PPUTLIMPL_UHEX_0xBA1u 2977u, 0xBA1, 0xBA1, 1, 0x45Eu, B, A, 1
#define PPUTLIMPL_UHEX_0xBA0u 2976u, 0xBA0, 0xBA0, 1, 0x45Fu, B, A, 0
#define PPUTLIMPL_UHEX_0xB9Fu 2975u, 0xB9F, 0xB9F, 1, 0x460u, B, 9, F
#define PPUTLIMPL_UHEX_0xB9Eu 2974u, 0xB9E, 0xB9E, 1, 0x461u, B, 9, E
#define PPUTLIMPL_UHEX_0xB9Du 2973u, 0xB9D, 0xB9D, 1, 0x462u, B, 9, D
#define PPUTLIMPL_UHEX_0xB9Cu 2972u, 0xB9C, 0xB9C, 1, 0x463u, B, 9, C
#define PPUTLIMPL_UHEX_0xB9Bu 2971u, 0xB9B, 0xB9B, 1, 0x464u, B, 9, B
#define PPUTLIMPL_UHEX_0xB9Au 2970u, 0xB9A, 0xB9A, 1, 0x465u, B, 9, A
#define PPUTLIMPL_UHEX_0xB99u 2969u, 0xB99, 0xB99, 1, 0x466u, B, 9, 9
#define PPUTLIMPL_UHEX_0xB98u 2968u, 0xB98, 0xB98, 1, 0x467u, B, 9, 8
#define PPUTLIMPL_UHEX_0xB97u 2967u, 0xB97, 0xB97, 1, 0x468u, B, 9, 7
#define PPUTLIMPL_UHEX_0xB96u 2966u, 0xB96, 0xB96, 1, 0x469u, B, 9, 6
#define PPUTLIMPL_UHEX_0xB95u 2965u, 0xB95, 0xB95, 1, 0x46Au, B, 9, 5
#define PPUTLIMPL_UHEX_0xB94u 2964u, 0xB94, 0xB94, 1, 0x46Bu, B, 9, 4
#define PPUTLIMPL_UHEX_0xB93u 2963u, 0xB93, 0xB93, 1, 0x46Cu, B, 9, 3
#define PPUTLIMPL_UHEX_0xB92u 2962u, 0xB92, 0xB92, 1, 0x46Du, B, 9, 2
#define PPUTLIMPL_UHEX_0xB91u 2961u, 0xB91, 0xB91, 1, 0x46Eu, B, 9, 1
#define PPUTLIMPL_UHEX_0xB90u 2960u, 0xB90, 0xB90, 1, 0x46Fu, B, 9, 0
#define PPUTLIMPL_UHEX_0xB8Fu 2959u, 0xB8F, 0xB8F, 1, 0x470u, B, 8, F
#define PPUTLIMPL_UHEX_0xB8Eu 2958u, 0xB8E, 0xB8E, 1, 0x471u, B, 8, E
#define PPUTLIMPL_UHEX_0xB8Du 2957u, 0xB8D, 0xB8D, 1, 0x472u, B, 8, D
#define PPUTLIMPL_UHEX_0xB8Cu 2956u, 0xB8C, 0xB8C, 1, 0x473u, B, 8, C
#define PPUTLIMPL_UHEX_0xB8Bu 2955u, 0xB8B, 0xB8B, 1, 0x474u, B, 8, B
#define PPUTLIMPL_UHEX_0xB8Au 2954u, 0xB8A, 0xB8A, 1, 0x475u, B, 8, A
#define PPUTLIMPL_UHEX_0xB89u 2953u, 0xB89, 0xB89, 1, 0x476u, B, 8, 9
#define PPUTLIMPL_UHEX_0xB88u 2952u, 0xB88, 0xB88, 1, 0x477u, B, 8, 8
#define PPUTLIMPL_UHEX_0xB87u 2951u, 0xB87, 0xB87, 1, 0x478u, B, 8, 7
#define PPUTLIMPL_UHEX_0xB86u 2950u, 0xB86, 0xB86, 1, 0x479u, B, 8, 6
#define PPUTLIMPL_UHEX_0xB85u 2949u, 0xB85, 0xB85, 1, 0x47Au, B, 8, 5
#define PPUTLIMPL_UHEX_0xB84u 2948u, 0xB84, 0xB84, 1, 0x47Bu, B, 8, 4
#define PPUTLIMPL_UHEX_0xB83u 2947u, 0xB83, 0xB83, 1, 0x47Cu, B, 8, 3
#define PPUTLIMPL_UHEX_0xB82u 2946u, 0xB82, 0xB82, 1, 0x47Du, B, 8, 2
#define PPUTLIMPL_UHEX_0xB81u 2945u, 0xB81, 0xB81, 1, 0x47Eu, B, 8, 1
#define PPUTLIMPL_UHEX_0xB80u 2944u, 0xB80, 0xB80, 1, 0x47Fu, B, 8, 0
#define PPUTLIMPL_UHEX_0xB7Fu 2943u, 0xB7F, 0xB7F, 1, 0x480u, B, 7, F
#define PPUTLIMPL_UHEX_0xB7Eu 2942u, 0xB7E, 0xB7E, 1, 0x481u, B, 7, E
#define PPUTLIMPL_UHEX_0xB7Du 2941u, 0xB7D, 0xB7D, 1, 0x482u, B, 7, D
#define PPUTLIMPL_UHEX_0xB7Cu 2940u, 0xB7C, 0xB7C, 1, 0x483u, B, 7, C
#define PPUTLIMPL_UHEX_0xB7Bu 2939u, 0xB7B, 0xB7B, 1, 0x484u, B, 7, B
#define PPUTLIMPL_UHEX_0xB7Au 2938u, 0xB7A, 0xB7A, 1, 0x485u, B, 7, A
#define PPUTLIMPL_UHEX_0xB79u 2937u, 0xB79, 0xB79, 1, 0x486u, B, 7, 9
#define PPUTLIMPL_UHEX_0xB78u 2936u, 0xB78, 0xB78, 1, 0x487u, B, 7, 8
#define PPUTLIMPL_UHEX_0xB77u 2935u, 0xB77, 0xB77, 1, 0x488u, B, 7, 7
#define PPUTLIMPL_UHEX_0xB76u 2934u, 0xB76, 0xB76, 1, 0x489u, B, 7, 6
#define PPUTLIMPL_UHEX_0xB75u 2933u, 0xB75, 0xB75, 1, 0x48Au, B, 7, 5
#define PPUTLIMPL_UHEX_0xB74u 2932u, 0xB74, 0xB74, 1, 0x48Bu, B, 7, 4
#define PPUTLIMPL_UHEX_0xB73u 2931u, 0xB73, 0xB73, 1, 0x48Cu, B, 7, 3
#define PPUTLIMPL_UHEX_0xB72u 2930u, 0xB72, 0xB72, 1, 0x48Du, B, 7, 2
#define PPUTLIMPL_UHEX_0xB71u 2929u, 0xB71, 0xB71, 1, 0x48Eu, B, 7, 1
#define PPUTLIMPL_UHEX_0xB70u 2928u, 0xB70, 0xB70, 1, 0x48Fu, B, 7, 0
#define PPUTLIMPL_UHEX_0xB6Fu 2927u, 0xB6F, 0xB6F, 1, 0x490u, B, 6, F
#define PPUTLIMPL_UHEX_0xB6Eu 2926u, 0xB6E, 0xB6E, 1, 0x491u, B, 6, E
#define PPUTLIMPL_UHEX_0xB6Du 2925u, 0xB6D, 0xB6D, 1, 0x492u, B, 6, D
#define PPUTLIMPL_UHEX_0xB6Cu 2924u, 0xB6C, 0xB6C, 1, 0x493u, B, 6, C
#define PPUTLIMPL_UHEX_0xB6Bu 2923u, 0xB6B, 0xB6B, 1, 0x494u, B, 6, B
#define PPUTLIMPL_UHEX_0xB6Au 2922u, 0xB6A, 0xB6A, 1, 0x495u, B, 6, A
#define PPUTLIMPL_UHEX_0xB69u 2921u, 0xB69, 0xB69, 1, 0x496u, B, 6, 9
#define PPUTLIMPL_UHEX_0xB68u 2920u, 0xB68, 0xB68, 1, 0x497u, B, 6, 8
#define PPUTLIMPL_UHEX_0xB67u 2919u, 0xB67, 0xB67, 1, 0x498u, B, 6, 7
#define PPUTLIMPL_UHEX_0xB66u 2918u, 0xB66, 0xB66, 1, 0x499u, B, 6, 6
#define PPUTLIMPL_UHEX_0xB65u 2917u, 0xB65, 0xB65, 1, 0x49Au, B, 6, 5
#define PPUTLIMPL_UHEX_0xB64u 2916u, 0xB64, 0xB64, 1, 0x49Bu, B, 6, 4
#define PPUTLIMPL_UHEX_0xB63u 2915u, 0xB63, 0xB63, 1, 0x49Cu, B, 6, 3
#define PPUTLIMPL_UHEX_0xB62u 2914u, 0xB62, 0xB62, 1, 0x49Du, B, 6, 2
#define PPUTLIMPL_UHEX_0xB61u 2913u, 0xB61, 0xB61, 1, 0x49Eu, B, 6, 1
#define PPUTLIMPL_UHEX_0xB60u 2912u, 0xB60, 0xB60, 1, 0x49Fu, B, 6, 0
#define PPUTLIMPL_UHEX_0xB5Fu 2911u, 0xB5F, 0xB5F, 1, 0x4A0u, B, 5, F
#define PPUTLIMPL_UHEX_0xB5Eu 2910u, 0xB5E, 0xB5E, 1, 0x4A1u, B, 5, E
#define PPUTLIMPL_UHEX_0xB5Du 2909u, 0xB5D, 0xB5D, 1, 0x4A2u, B, 5, D
#define PPUTLIMPL_UHEX_0xB5Cu 2908u, 0xB5C, 0xB5C, 1, 0x4A3u, B, 5, C
#define PPUTLIMPL_UHEX_0xB5Bu 2907u, 0xB5B, 0xB5B, 1, 0x4A4u, B, 5, B
#define PPUTLIMPL_UHEX_0xB5Au 2906u, 0xB5A, 0xB5A, 1, 0x4A5u, B, 5, A
#define PPUTLIMPL_UHEX_0xB59u 2905u, 0xB59, 0xB59, 1, 0x4A6u, B, 5, 9
#define PPUTLIMPL_UHEX_0xB58u 2904u, 0xB58, 0xB58, 1, 0x4A7u, B, 5, 8
#define PPUTLIMPL_UHEX_0xB57u 2903u, 0xB57, 0xB57, 1, 0x4A8u, B, 5, 7
#define PPUTLIMPL_UHEX_0xB56u 2902u, 0xB56, 0xB56, 1, 0x4A9u, B, 5, 6
#define PPUTLIMPL_UHEX_0xB55u 2901u, 0xB55, 0xB55, 1, 0x4AAu, B, 5, 5
#define PPUTLIMPL_UHEX_0xB54u 2900u, 0xB54, 0xB54, 1, 0x4ABu, B, 5, 4
#define PPUTLIMPL_UHEX_0xB53u 2899u, 0xB53, 0xB53, 1, 0x4ACu, B, 5, 3
#define PPUTLIMPL_UHEX_0xB52u 2898u, 0xB52, 0xB52, 1, 0x4ADu, B, 5, 2
#define PPUTLIMPL_UHEX_0xB51u 2897u, 0xB51, 0xB51, 1, 0x4AEu, B, 5, 1
#define PPUTLIMPL_UHEX_0xB50u 2896u, 0xB50, 0xB50, 1, 0x4AFu, B, 5, 0
#define PPUTLIMPL_UHEX_0xB4Fu 2895u, 0xB4F, 0xB4F, 1, 0x4B0u, B, 4, F
#define PPUTLIMPL_UHEX_0xB4Eu 2894u, 0xB4E, 0xB4E, 1, 0x4B1u, B, 4, E
#define PPUTLIMPL_UHEX_0xB4Du 2893u, 0xB4D, 0xB4D, 1, 0x4B2u, B, 4, D
#define PPUTLIMPL_UHEX_0xB4Cu 2892u, 0xB4C, 0xB4C, 1, 0x4B3u, B, 4, C
#define PPUTLIMPL_UHEX_0xB4Bu 2891u, 0xB4B, 0xB4B, 1, 0x4B4u, B, 4, B
#define PPUTLIMPL_UHEX_0xB4Au 2890u, 0xB4A, 0xB4A, 1, 0x4B5u, B, 4, A
#define PPUTLIMPL_UHEX_0xB49u 2889u, 0xB49, 0xB49, 1, 0x4B6u, B, 4, 9
#define PPUTLIMPL_UHEX_0xB48u 2888u, 0xB48, 0xB48, 1, 0x4B7u, B, 4, 8
#define PPUTLIMPL_UHEX_0xB47u 2887u, 0xB47, 0xB47, 1, 0x4B8u, B, 4, 7
#define PPUTLIMPL_UHEX_0xB46u 2886u, 0xB46, 0xB46, 1, 0x4B9u, B, 4, 6
#define PPUTLIMPL_UHEX_0xB45u 2885u, 0xB45, 0xB45, 1, 0x4BAu, B, 4, 5
#define PPUTLIMPL_UHEX_0xB44u 2884u, 0xB44, 0xB44, 1, 0x4BBu, B, 4, 4
#define PPUTLIMPL_UHEX_0xB43u 2883u, 0xB43, 0xB43, 1, 0x4BCu, B, 4, 3
#define PPUTLIMPL_UHEX_0xB42u 2882u, 0xB42, 0xB42, 1, 0x4BDu, B, 4, 2
#define PPUTLIMPL_UHEX_0xB41u 2881u, 0xB41, 0xB41, 1, 0x4BEu, B, 4, 1
#define PPUTLIMPL_UHEX_0xB40u 2880u, 0xB40, 0xB40, 1, 0x4BFu, B, 4, 0
#define PPUTLIMPL_UHEX_0xB3Fu 2879u, 0xB3F, 0xB3F, 1, 0x4C0u, B, 3, F
#define PPUTLIMPL_UHEX_0xB3Eu 2878u, 0xB3E, 0xB3E, 1, 0x4C1u, B, 3, E
#define PPUTLIMPL_UHEX_0xB3Du 2877u, 0xB3D, 0xB3D, 1, 0x4C2u, B, 3, D
#define PPUTLIMPL_UHEX_0xB3Cu 2876u, 0xB3C, 0xB3C, 1, 0x4C3u, B, 3, C
#define PPUTLIMPL_UHEX_0xB3Bu 2875u, 0xB3B, 0xB3B, 1, 0x4C4u, B, 3, B
#define PPUTLIMPL_UHEX_0xB3Au 2874u, 0xB3A, 0xB3A, 1, 0x4C5u, B, 3, A
#define PPUTLIMPL_UHEX_0xB39u 2873u, 0xB39, 0xB39, 1, 0x4C6u, B, 3, 9
#define PPUTLIMPL_UHEX_0xB38u 2872u, 0xB38, 0xB38, 1, 0x4C7u, B, 3, 8
#define PPUTLIMPL_UHEX_0xB37u 2871u, 0xB37, 0xB37, 1, 0x4C8u, B, 3, 7
#define PPUTLIMPL_UHEX_0xB36u 2870u, 0xB36, 0xB36, 1, 0x4C9u, B, 3, 6
#define PPUTLIMPL_UHEX_0xB35u 2869u, 0xB35, 0xB35, 1, 0x4CAu, B, 3, 5
#define PPUTLIMPL_UHEX_0xB34u 2868u, 0xB34, 0xB34, 1, 0x4CBu, B, 3, 4
#define PPUTLIMPL_UHEX_0xB33u 2867u, 0xB33, 0xB33, 1, 0x4CCu, B, 3, 3
#define PPUTLIMPL_UHEX_0xB32u 2866u, 0xB32, 0xB32, 1, 0x4CDu, B, 3, 2
#define PPUTLIMPL_UHEX_0xB31u 2865u, 0xB31, 0xB31, 1, 0x4CEu, B, 3, 1
#define PPUTLIMPL_UHEX_0xB30u 2864u, 0xB30, 0xB30, 1, 0x4CFu, B, 3, 0
#define PPUTLIMPL_UHEX_0xB2Fu 2863u, 0xB2F, 0xB2F, 1, 0x4D0u, B, 2, F
#define PPUTLIMPL_UHEX_0xB2Eu 2862u, 0xB2E, 0xB2E, 1, 0x4D1u, B, 2, E
#define PPUTLIMPL_UHEX_0xB2Du 2861u, 0xB2D, 0xB2D, 1, 0x4D2u, B, 2, D
#define PPUTLIMPL_UHEX_0xB2Cu 2860u, 0xB2C, 0xB2C, 1, 0x4D3u, B, 2, C
#define PPUTLIMPL_UHEX_0xB2Bu 2859u, 0xB2B, 0xB2B, 1, 0x4D4u, B, 2, B
#define PPUTLIMPL_UHEX_0xB2Au 2858u, 0xB2A, 0xB2A, 1, 0x4D5u, B, 2, A
#define PPUTLIMPL_UHEX_0xB29u 2857u, 0xB29, 0xB29, 1, 0x4D6u, B, 2, 9
#define PPUTLIMPL_UHEX_0xB28u 2856u, 0xB28, 0xB28, 1, 0x4D7u, B, 2, 8
#define PPUTLIMPL_UHEX_0xB27u 2855u, 0xB27, 0xB27, 1, 0x4D8u, B, 2, 7
#define PPUTLIMPL_UHEX_0xB26u 2854u, 0xB26, 0xB26, 1, 0x4D9u, B, 2, 6
#define PPUTLIMPL_UHEX_0xB25u 2853u, 0xB25, 0xB25, 1, 0x4DAu, B, 2, 5
#define PPUTLIMPL_UHEX_0xB24u 2852u, 0xB24, 0xB24, 1, 0x4DBu, B, 2, 4
#define PPUTLIMPL_UHEX_0xB23u 2851u, 0xB23, 0xB23, 1, 0x4DCu, B, 2, 3
#define PPUTLIMPL_UHEX_0xB22u 2850u, 0xB22, 0xB22, 1, 0x4DDu, B, 2, 2
#define PPUTLIMPL_UHEX_0xB21u 2849u, 0xB21, 0xB21, 1, 0x4DEu, B, 2, 1
#define PPUTLIMPL_UHEX_0xB20u 2848u, 0xB20, 0xB20, 1, 0x4DFu, B, 2, 0
#define PPUTLIMPL_UHEX_0xB1Fu 2847u, 0xB1F, 0xB1F, 1, 0x4E0u, B, 1, F
#define PPUTLIMPL_UHEX_0xB1Eu 2846u, 0xB1E, 0xB1E, 1, 0x4E1u, B, 1, E
#define PPUTLIMPL_UHEX_0xB1Du 2845u, 0xB1D, 0xB1D, 1, 0x4E2u, B, 1, D
#define PPUTLIMPL_UHEX_0xB1Cu 2844u, 0xB1C, 0xB1C, 1, 0x4E3u, B, 1, C
#define PPUTLIMPL_UHEX_0xB1Bu 2843u, 0xB1B, 0xB1B, 1, 0x4E4u, B, 1, B
#define PPUTLIMPL_UHEX_0xB1Au 2842u, 0xB1A, 0xB1A, 1, 0x4E5u, B, 1, A
#define PPUTLIMPL_UHEX_0xB19u 2841u, 0xB19, 0xB19, 1, 0x4E6u, B, 1, 9
#define PPUTLIMPL_UHEX_0xB18u 2840u, 0xB18, 0xB18, 1, 0x4E7u, B, 1, 8
#define PPUTLIMPL_UHEX_0xB17u 2839u, 0xB17, 0xB17, 1, 0x4E8u, B, 1, 7
#define PPUTLIMPL_UHEX_0xB16u 2838u, 0xB16, 0xB16, 1, 0x4E9u, B, 1, 6
#define PPUTLIMPL_UHEX_0xB15u 2837u, 0xB15, 0xB15, 1, 0x4EAu, B, 1, 5
#define PPUTLIMPL_UHEX_0xB14u 2836u, 0xB14, 0xB14, 1, 0x4EBu, B, 1, 4
#define PPUTLIMPL_UHEX_0xB13u 2835u, 0xB13, 0xB13, 1, 0x4ECu, B, 1, 3
#define PPUTLIMPL_UHEX_0xB12u 2834u, 0xB12, 0xB12, 1, 0x4EDu, B, 1, 2
#define PPUTLIMPL_UHEX_0xB11u 2833u, 0xB11, 0xB11, 1, 0x4EEu, B, 1, 1
#define PPUTLIMPL_UHEX_0xB10u 2832u, 0xB10, 0xB10, 1, 0x4EFu, B, 1, 0
#define PPUTLIMPL_UHEX_0xB0Fu 2831u, 0xB0F, 0xB0F, 1, 0x4F0u, B, 0, F
#define PPUTLIMPL_UHEX_0xB0Eu 2830u, 0xB0E, 0xB0E, 1, 0x4F1u, B, 0, E
#define PPUTLIMPL_UHEX_0xB0Du 2829u, 0xB0D, 0xB0D, 1, 0x4F2u, B, 0, D
#define PPUTLIMPL_UHEX_0xB0Cu 2828u, 0xB0C, 0xB0C, 1, 0x4F3u, B, 0, C
#define PPUTLIMPL_UHEX_0xB0Bu 2827u, 0xB0B, 0xB0B, 1, 0x4F4u, B, 0, B
#define PPUTLIMPL_UHEX_0xB0Au 2826u, 0xB0A, 0xB0A, 1, 0x4F5u, B, 0, A
#define PPUTLIMPL_UHEX_0xB09u 2825u, 0xB09, 0xB09, 1, 0x4F6u, B, 0, 9
#define PPUTLIMPL_UHEX_0xB08u 2824u, 0xB08, 0xB08, 1, 0x4F7u, B, 0, 8
#define PPUTLIMPL_UHEX_0xB07u 2823u, 0xB07, 0xB07, 1, 0x4F8u, B, 0, 7
#define PPUTLIMPL_UHEX_0xB06u 2822u, 0xB06, 0xB06, 1, 0x4F9u, B, 0, 6
#define PPUTLIMPL_UHEX_0xB05u 2821u, 0xB05, 0xB05, 1, 0x4FAu, B, 0, 5
#define PPUTLIMPL_UHEX_0xB04u 2820u, 0xB04, 0xB04, 1, 0x4FBu, B, 0, 4
#define PPUTLIMPL_UHEX_0xB03u 2819u, 0xB03, 0xB03, 1, 0x4FCu, B, 0, 3
#define PPUTLIMPL_UHEX_0xB02u 2818u, 0xB02, 0xB02, 1, 0x4FDu, B, 0, 2
#define PPUTLIMPL_UHEX_0xB01u 2817u, 0xB01, 0xB01, 1, 0x4FEu, B, 0, 1
#define PPUTLIMPL_UHEX_0xB00u 2816u, 0xB00, 0xB00, 1, 0x4FFu, B, 0, 0
#define PPUTLIMPL_UHEX_0xAFFu 2815u, 0xAFF, 0xAFF, 1, 0x500u, A, F, F
#define PPUTLIMPL_UHEX_0xAFEu 2814u, 0xAFE, 0xAFE, 1, 0x501u, A, F, E
#define PPUTLIMPL_UHEX_0xAFDu 2813u, 0xAFD, 0xAFD, 1, 0x502u, A, F, D
#define PPUTLIMPL_UHEX_0xAFCu 2812u, 0xAFC, 0xAFC, 1, 0x503u, A, F, C
#define PPUTLIMPL_UHEX_0xAFBu 2811u, 0xAFB, 0xAFB, 1, 0x504u, A, F, B
#define PPUTLIMPL_UHEX_0xAFAu 2810u, 0xAFA, 0xAFA, 1, 0x505u, A, F, A
#define PPUTLIMPL_UHEX_0xAF9u 2809u, 0xAF9, 0xAF9, 1, 0x506u, A, F, 9
#define PPUTLIMPL_UHEX_0xAF8u 2808u, 0xAF8, 0xAF8, 1, 0x507u, A, F, 8
#define PPUTLIMPL_UHEX_0xAF7u 2807u, 0xAF7, 0xAF7, 1, 0x508u, A, F, 7
#define PPUTLIMPL_UHEX_0xAF6u 2806u, 0xAF6, 0xAF6, 1, 0x509u, A, F, 6
#define PPUTLIMPL_UHEX_0xAF5u 2805u, 0xAF5, 0xAF5, 1, 0x50Au, A, F, 5
#define PPUTLIMPL_UHEX_0xAF4u 2804u, 0xAF4, 0xAF4, 1, 0x50Bu, A, F, 4
#define PPUTLIMPL_UHEX_0xAF3u 2803u, 0xAF3, 0xAF3, 1, 0x50Cu, A, F, 3
#define PPUTLIMPL_UHEX_0xAF2u 2802u, 0xAF2, 0xAF2, 1, 0x50Du, A, F, 2
#define PPUTLIMPL_UHEX_0xAF1u 2801u, 0xAF1, 0xAF1, 1, 0x50Eu, A, F, 1
#define PPUTLIMPL_UHEX_0xAF0u 2800u, 0xAF0, 0xAF0, 1, 0x50Fu, A, F, 0
#define PPUTLIMPL_UHEX_0xAEFu 2799u, 0xAEF, 0xAEF, 1, 0x510u, A, E, F
#define PPUTLIMPL_UHEX_0xAEEu 2798u, 0xAEE, 0xAEE, 1, 0x511u, A, E, E
#define PPUTLIMPL_UHEX_0xAEDu 2797u, 0xAED, 0xAED, 1, 0x512u, A, E, D
#define PPUTLIMPL_UHEX_0xAECu 2796u, 0xAEC, 0xAEC, 1, 0x513u, A, E, C
#define PPUTLIMPL_UHEX_0xAEBu 2795u, 0xAEB, 0xAEB, 1, 0x514u, A, E, B
#define PPUTLIMPL_UHEX_0xAEAu 2794u, 0xAEA, 0xAEA, 1, 0x515u, A, E, A
#define PPUTLIMPL_UHEX_0xAE9u 2793u, 0xAE9, 0xAE9, 1, 0x516u, A, E, 9
#define PPUTLIMPL_UHEX_0xAE8u 2792u, 0xAE8, 0xAE8, 1, 0x517u, A, E, 8
#define PPUTLIMPL_UHEX_0xAE7u 2791u, 0xAE7, 0xAE7, 1, 0x518u, A, E, 7
#define PPUTLIMPL_UHEX_0xAE6u 2790u, 0xAE6, 0xAE6, 1, 0x519u, A, E, 6
#define PPUTLIMPL_UHEX_0xAE5u 2789u, 0xAE5, 0xAE5, 1, 0x51Au, A, E, 5
#define PPUTLIMPL_UHEX_0xAE4u 2788u, 0xAE4, 0xAE4, 1, 0x51Bu, A, E, 4
#define PPUTLIMPL_UHEX_0xAE3u 2787u, 0xAE3, 0xAE3, 1, 0x51Cu, A, E, 3
#define PPUTLIMPL_UHEX_0xAE2u 2786u, 0xAE2, 0xAE2, 1, 0x51Du, A, E, 2
#define PPUTLIMPL_UHEX_0xAE1u 2785u, 0xAE1, 0xAE1, 1, 0x51Eu, A, E, 1
#define PPUTLIMPL_UHEX_0xAE0u 2784u, 0xAE0, 0xAE0, 1, 0x51Fu, A, E, 0
#define PPUTLIMPL_UHEX_0xADFu 2783u, 0xADF, 0xADF, 1, 0x520u, A, D, F
#define PPUTLIMPL_UHEX_0xADEu 2782u, 0xADE, 0xADE, 1, 0x521u, A, D, E
#define PPUTLIMPL_UHEX_0xADDu 2781u, 0xADD, 0xADD, 1, 0x522u, A, D, D
#define PPUTLIMPL_UHEX_0xADCu 2780u, 0xADC, 0xADC, 1, 0x523u, A, D, C
#define PPUTLIMPL_UHEX_0xADBu 2779u, 0xADB, 0xADB, 1, 0x524u, A, D, B
#define PPUTLIMPL_UHEX_0xADAu 2778u, 0xADA, 0xADA, 1, 0x525u, A, D, A
#define PPUTLIMPL_UHEX_0xAD9u 2777u, 0xAD9, 0xAD9, 1, 0x526u, A, D, 9
#define PPUTLIMPL_UHEX_0xAD8u 2776u, 0xAD8, 0xAD8, 1, 0x527u, A, D, 8
#define PPUTLIMPL_UHEX_0xAD7u 2775u, 0xAD7, 0xAD7, 1, 0x528u, A, D, 7
#define PPUTLIMPL_UHEX_0xAD6u 2774u, 0xAD6, 0xAD6, 1, 0x529u, A, D, 6
#define PPUTLIMPL_UHEX_0xAD5u 2773u, 0xAD5, 0xAD5, 1, 0x52Au, A, D, 5
#define PPUTLIMPL_UHEX_0xAD4u 2772u, 0xAD4, 0xAD4, 1, 0x52Bu, A, D, 4
#define PPUTLIMPL_UHEX_0xAD3u 2771u, 0xAD3, 0xAD3, 1, 0x52Cu, A, D, 3
#define PPUTLIMPL_UHEX_0xAD2u 2770u, 0xAD2, 0xAD2, 1, 0x52Du, A, D, 2
#define PPUTLIMPL_UHEX_0xAD1u 2769u, 0xAD1, 0xAD1, 1, 0x52Eu, A, D, 1
#define PPUTLIMPL_UHEX_0xAD0u 2768u, 0xAD0, 0xAD0, 1, 0x52Fu, A, D, 0
#define PPUTLIMPL_UHEX_0xACFu 2767u, 0xACF, 0xACF, 1, 0x530u, A, C, F
#define PPUTLIMPL_UHEX_0xACEu 2766u, 0xACE, 0xACE, 1, 0x531u, A, C, E
#define PPUTLIMPL_UHEX_0xACDu 2765u, 0xACD, 0xACD, 1, 0x532u, A, C, D
#define PPUTLIMPL_UHEX_0xACCu 2764u, 0xACC, 0xACC, 1, 0x533u, A, C, C
#define PPUTLIMPL_UHEX_0xACBu 2763u, 0xACB, 0xACB, 1, 0x534u, A, C, B
#define PPUTLIMPL_UHEX_0xACAu 2762u, 0xACA, 0xACA, 1, 0x535u, A, C, A
#define PPUTLIMPL_UHEX_0xAC9u 2761u, 0xAC9, 0xAC9, 1, 0x536u, A, C, 9
#define PPUTLIMPL_UHEX_0xAC8u 2760u, 0xAC8, 0xAC8, 1, 0x537u, A, C, 8
#define PPUTLIMPL_UHEX_0xAC7u 2759u, 0xAC7, 0xAC7, 1, 0x538u, A, C, 7
#define PPUTLIMPL_UHEX_0xAC6u 2758u, 0xAC6, 0xAC6, 1, 0x539u, A, C, 6
#define PPUTLIMPL_UHEX_0xAC5u 2757u, 0xAC5, 0xAC5, 1, 0x53Au, A, C, 5
#define PPUTLIMPL_UHEX_0xAC4u 2756u, 0xAC4, 0xAC4, 1, 0x53Bu, A, C, 4
#define PPUTLIMPL_UHEX_0xAC3u 2755u, 0xAC3, 0xAC3, 1, 0x53Cu, A, C, 3
#define PPUTLIMPL_UHEX_0xAC2u 2754u, 0xAC2, 0xAC2, 1, 0x53Du, A, C, 2
#define PPUTLIMPL_UHEX_0xAC1u 2753u, 0xAC1, 0xAC1, 1, 0x53Eu, A, C, 1
#define PPUTLIMPL_UHEX_0xAC0u 2752u, 0xAC0, 0xAC0, 1, 0x53Fu, A, C, 0
#define PPUTLIMPL_UHEX_0xABFu 2751u, 0xABF, 0xABF, 1, 0x540u, A, B, F
#define PPUTLIMPL_UHEX_0xABEu 2750u, 0xABE, 0xABE, 1, 0x541u, A, B, E
#define PPUTLIMPL_UHEX_0xABDu 2749u, 0xABD, 0xABD, 1, 0x542u, A, B, D
#define PPUTLIMPL_UHEX_0xABCu 2748u, 0xABC, 0xABC, 1, 0x543u, A, B, C
#define PPUTLIMPL_UHEX_0xABBu 2747u, 0xABB, 0xABB, 1, 0x544u, A, B, B
#define PPUTLIMPL_UHEX_0xABAu 2746u, 0xABA, 0xABA, 1, 0x545u, A, B, A
#define PPUTLIMPL_UHEX_0xAB9u 2745u, 0xAB9, 0xAB9, 1, 0x546u, A, B, 9
#define PPUTLIMPL_UHEX_0xAB8u 2744u, 0xAB8, 0xAB8, 1, 0x547u, A, B, 8
#define PPUTLIMPL_UHEX_0xAB7u 2743u, 0xAB7, 0xAB7, 1, 0x548u, A, B, 7
#define PPUTLIMPL_UHEX_0xAB6u 2742u, 0xAB6, 0xAB6, 1, 0x549u, A, B, 6
#define PPUTLIMPL_UHEX_0xAB5u 2741u, 0xAB5, 0xAB5, 1, 0x54Au, A, B, 5
#define PPUTLIMPL_UHEX_0xAB4u 2740u, 0xAB4, 0xAB4, 1, 0x54Bu, A, B, 4
#define PPUTLIMPL_UHEX_0xAB3u 2739u, 0xAB3, 0xAB3, 1, 0x54Cu, A, B, 3
#define PPUTLIMPL_UHEX_0xAB2u 2738u, 0xAB2, 0xAB2, 1, 0x54Du, A, B, 2
#define PPUTLIMPL_UHEX_0xAB1u 2737u, 0xAB1, 0xAB1, 1, 0x54Eu, A, B, 1
#define PPUTLIMPL_UHEX_0xAB0u 2736u, 0xAB0, 0xAB0, 1, 0x54Fu, A, B, 0
#define PPUTLIMPL_UHEX_0xAAFu 2735u, 0xAAF, 0xAAF, 1, 0x550u, A, A, F
#define PPUTLIMPL_UHEX_0xAAEu 2734u, 0xAAE, 0xAAE, 1, 0x551u, A, A, E
#define PPUTLIMPL_UHEX_0xAADu 2733u, 0xAAD, 0xAAD, 1, 0x552u, A, A, D
#define PPUTLIMPL_UHEX_0xAACu 2732u, 0xAAC, 0xAAC, 1, 0x553u, A, A, C
#define PPUTLIMPL_UHEX_0xAABu 2731u, 0xAAB, 0xAAB, 1, 0x554u, A, A, B
#define PPUTLIMPL_UHEX_0xAAAu 2730u, 0xAAA, 0xAAA, 1, 0x555u, A, A, A
#define PPUTLIMPL_UHEX_0xAA9u 2729u, 0xAA9, 0xAA9, 1, 0x556u, A, A, 9
#define PPUTLIMPL_UHEX_0xAA8u 2728u, 0xAA8, 0xAA8, 1, 0x557u, A, A, 8
#define PPUTLIMPL_UHEX_0xAA7u 2727u, 0xAA7, 0xAA7, 1, 0x558u, A, A, 7
#define PPUTLIMPL_UHEX_0xAA6u 2726u, 0xAA6, 0xAA6, 1, 0x559u, A, A, 6
#define PPUTLIMPL_UHEX_0xAA5u 2725u, 0xAA5, 0xAA5, 1, 0x55Au, A, A, 5
#define PPUTLIMPL_UHEX_0xAA4u 2724u, 0xAA4, 0xAA4, 1, 0x55Bu, A, A, 4
#define PPUTLIMPL_UHEX_0xAA3u 2723u, 0xAA3, 0xAA3, 1, 0x55Cu, A, A, 3
#define PPUTLIMPL_UHEX_0xAA2u 2722u, 0xAA2, 0xAA2, 1, 0x55Du, A, A, 2
#define PPUTLIMPL_UHEX_0xAA1u 2721u, 0xAA1, 0xAA1, 1, 0x55Eu, A, A, 1
#define PPUTLIMPL_UHEX_0xAA0u 2720u, 0xAA0, 0xAA0, 1, 0x55Fu, A, A, 0
#define PPUTLIMPL_UHEX_0xA9Fu 2719u, 0xA9F, 0xA9F, 1, 0x560u, A, 9, F
#define PPUTLIMPL_UHEX_0xA9Eu 2718u, 0xA9E, 0xA9E, 1, 0x561u, A, 9, E
#define PPUTLIMPL_UHEX_0xA9Du 2717u, 0xA9D, 0xA9D, 1, 0x562u, A, 9, D
#define PPUTLIMPL_UHEX_0xA9Cu 2716u, 0xA9C, 0xA9C, 1, 0x563u, A, 9, C
#define PPUTLIMPL_UHEX_0xA9Bu 2715u, 0xA9B, 0xA9B, 1, 0x564u, A, 9, B
#define PPUTLIMPL_UHEX_0xA9Au 2714u, 0xA9A, 0xA9A, 1, 0x565u, A, 9, A
#define PPUTLIMPL_UHEX_0xA99u 2713u, 0xA99, 0xA99, 1, 0x566u, A, 9, 9
#define PPUTLIMPL_UHEX_0xA98u 2712u, 0xA98, 0xA98, 1, 0x567u, A, 9, 8
#define PPUTLIMPL_UHEX_0xA97u 2711u, 0xA97, 0xA97, 1, 0x568u, A, 9, 7
#define PPUTLIMPL_UHEX_0xA96u 2710u, 0xA96, 0xA96, 1, 0x569u, A, 9, 6
#define PPUTLIMPL_UHEX_0xA95u 2709u, 0xA95, 0xA95, 1, 0x56Au, A, 9, 5
#define PPUTLIMPL_UHEX_0xA94u 2708u, 0xA94, 0xA94, 1, 0x56Bu, A, 9, 4
#define PPUTLIMPL_UHEX_0xA93u 2707u, 0xA93, 0xA93, 1, 0x56Cu, A, 9, 3
#define PPUTLIMPL_UHEX_0xA92u 2706u, 0xA92, 0xA92, 1, 0x56Du, A, 9, 2
#define PPUTLIMPL_UHEX_0xA91u 2705u, 0xA91, 0xA91, 1, 0x56Eu, A, 9, 1
#define PPUTLIMPL_UHEX_0xA90u 2704u, 0xA90, 0xA90, 1, 0x56Fu, A, 9, 0
#define PPUTLIMPL_UHEX_0xA8Fu 2703u, 0xA8F, 0xA8F, 1, 0x570u, A, 8, F
#define PPUTLIMPL_UHEX_0xA8Eu 2702u, 0xA8E, 0xA8E, 1, 0x571u, A, 8, E
#define PPUTLIMPL_UHEX_0xA8Du 2701u, 0xA8D, 0xA8D, 1, 0x572u, A, 8, D
#define PPUTLIMPL_UHEX_0xA8Cu 2700u, 0xA8C, 0xA8C, 1, 0x573u, A, 8, C
#define PPUTLIMPL_UHEX_0xA8Bu 2699u, 0xA8B, 0xA8B, 1, 0x574u, A, 8, B
#define PPUTLIMPL_UHEX_0xA8Au 2698u, 0xA8A, 0xA8A, 1, 0x575u, A, 8, A
#define PPUTLIMPL_UHEX_0xA89u 2697u, 0xA89, 0xA89, 1, 0x576u, A, 8, 9
#define PPUTLIMPL_UHEX_0xA88u 2696u, 0xA88, 0xA88, 1, 0x577u, A, 8, 8
#define PPUTLIMPL_UHEX_0xA87u 2695u, 0xA87, 0xA87, 1, 0x578u, A, 8, 7
#define PPUTLIMPL_UHEX_0xA86u 2694u, 0xA86, 0xA86, 1, 0x579u, A, 8, 6
#define PPUTLIMPL_UHEX_0xA85u 2693u, 0xA85, 0xA85, 1, 0x57Au, A, 8, 5
#define PPUTLIMPL_UHEX_0xA84u 2692u, 0xA84, 0xA84, 1, 0x57Bu, A, 8, 4
#define PPUTLIMPL_UHEX_0xA83u 2691u, 0xA83, 0xA83, 1, 0x57Cu, A, 8, 3
#define PPUTLIMPL_UHEX_0xA82u 2690u, 0xA82, 0xA82, 1, 0x57Du, A, 8, 2
#define PPUTLIMPL_UHEX_0xA81u 2689u, 0xA81, 0xA81, 1, 0x57Eu, A, 8, 1
#define PPUTLIMPL_UHEX_0xA80u 2688u, 0xA80, 0xA80, 1, 0x57Fu, A, 8, 0
#define PPUTLIMPL_UHEX_0xA7Fu 2687u, 0xA7F, 0xA7F, 1, 0x580u, A, 7, F
#define PPUTLIMPL_UHEX_0xA7Eu 2686u, 0xA7E, 0xA7E, 1, 0x581u, A, 7, E
#define PPUTLIMPL_UHEX_0xA7Du 2685u, 0xA7D, 0xA7D, 1, 0x582u, A, 7, D
#define PPUTLIMPL_UHEX_0xA7Cu 2684u, 0xA7C, 0xA7C, 1, 0x583u, A, 7, C
#define PPUTLIMPL_UHEX_0xA7Bu 2683u, 0xA7B, 0xA7B, 1, 0x584u, A, 7, B
#define PPUTLIMPL_UHEX_0xA7Au 2682u, 0xA7A, 0xA7A, 1, 0x585u, A, 7, A
#define PPUTLIMPL_UHEX_0xA79u 2681u, 0xA79, 0xA79, 1, 0x586u, A, 7, 9
#define PPUTLIMPL_UHEX_0xA78u 2680u, 0xA78, 0xA78, 1, 0x587u, A, 7, 8
#define PPUTLIMPL_UHEX_0xA77u 2679u, 0xA77, 0xA77, 1, 0x588u, A, 7, 7
#define PPUTLIMPL_UHEX_0xA76u 2678u, 0xA76, 0xA76, 1, 0x589u, A, 7, 6
#define PPUTLIMPL_UHEX_0xA75u 2677u, 0xA75, 0xA75, 1, 0x58Au, A, 7, 5
#define PPUTLIMPL_UHEX_0xA74u 2676u, 0xA74, 0xA74, 1, 0x58Bu, A, 7, 4
#define PPUTLIMPL_UHEX_0xA73u 2675u, 0xA73, 0xA73, 1, 0x58Cu, A, 7, 3
#define PPUTLIMPL_UHEX_0xA72u 2674u, 0xA72, 0xA72, 1, 0x58Du, A, 7, 2
#define PPUTLIMPL_UHEX_0xA71u 2673u, 0xA71, 0xA71, 1, 0x58Eu, A, 7, 1
#define PPUTLIMPL_UHEX_0xA70u 2672u, 0xA70, 0xA70, 1, 0x58Fu, A, 7, 0
#define PPUTLIMPL_UHEX_0xA6Fu 2671u, 0xA6F, 0xA6F, 1, 0x590u, A, 6, F
#define PPUTLIMPL_UHEX_0xA6Eu 2670u, 0xA6E, 0xA6E, 1, 0x591u, A, 6, E
#define PPUTLIMPL_UHEX_0xA6Du 2669u, 0xA6D, 0xA6D, 1, 0x592u, A, 6, D
#define PPUTLIMPL_UHEX_0xA6Cu 2668u, 0xA6C, 0xA6C, 1, 0x593u, A, 6, C
#define PPUTLIMPL_UHEX_0xA6Bu 2667u, 0xA6B, 0xA6B, 1, 0x594u, A, 6, B
#define PPUTLIMPL_UHEX_0xA6Au 2666u, 0xA6A, 0xA6A, 1, 0x595u, A, 6, A
#define PPUTLIMPL_UHEX_0xA69u 2665u, 0xA69, 0xA69, 1, 0x596u, A, 6, 9
#define PPUTLIMPL_UHEX_0xA68u 2664u, 0xA68, 0xA68, 1, 0x597u, A, 6, 8
#define PPUTLIMPL_UHEX_0xA67u 2663u, 0xA67, 0xA67, 1, 0x598u, A, 6, 7
#define PPUTLIMPL_UHEX_0xA66u 2662u, 0xA66, 0xA66, 1, 0x599u, A, 6, 6
#define PPUTLIMPL_UHEX_0xA65u 2661u, 0xA65, 0xA65, 1, 0x59Au, A, 6, 5
#define PPUTLIMPL_UHEX_0xA64u 2660u, 0xA64, 0xA64, 1, 0x59Bu, A, 6, 4
#define PPUTLIMPL_UHEX_0xA63u 2659u, 0xA63, 0xA63, 1, 0x59Cu, A, 6, 3
#define PPUTLIMPL_UHEX_0xA62u 2658u, 0xA62, 0xA62, 1, 0x59Du, A, 6, 2
#define PPUTLIMPL_UHEX_0xA61u 2657u, 0xA61, 0xA61, 1, 0x59Eu, A, 6, 1
#define PPUTLIMPL_UHEX_0xA60u 2656u, 0xA60, 0xA60, 1, 0x59Fu, A, 6, 0
#define PPUTLIMPL_UHEX_0xA5Fu 2655u, 0xA5F, 0xA5F, 1, 0x5A0u, A, 5, F
#define PPUTLIMPL_UHEX_0xA5Eu 2654u, 0xA5E, 0xA5E, 1, 0x5A1u, A, 5, E
#define PPUTLIMPL_UHEX_0xA5Du 2653u, 0xA5D, 0xA5D, 1, 0x5A2u, A, 5, D
#define PPUTLIMPL_UHEX_0xA5Cu 2652u, 0xA5C, 0xA5C, 1, 0x5A3u, A, 5, C
#define PPUTLIMPL_UHEX_0xA5Bu 2651u, 0xA5B, 0xA5B, 1, 0x5A4u, A, 5, B
#define PPUTLIMPL_UHEX_0xA5Au 2650u, 0xA5A, 0xA5A, 1, 0x5A5u, A, 5, A
#define PPUTLIMPL_UHEX_0xA59u 2649u, 0xA59, 0xA59, 1, 0x5A6u, A, 5, 9
#define PPUTLIMPL_UHEX_0xA58u 2648u, 0xA58, 0xA58, 1, 0x5A7u, A, 5, 8
#define PPUTLIMPL_UHEX_0xA57u 2647u, 0xA57, 0xA57, 1, 0x5A8u, A, 5, 7
#define PPUTLIMPL_UHEX_0xA56u 2646u, 0xA56, 0xA56, 1, 0x5A9u, A, 5, 6
#define PPUTLIMPL_UHEX_0xA55u 2645u, 0xA55, 0xA55, 1, 0x5AAu, A, 5, 5
#define PPUTLIMPL_UHEX_0xA54u 2644u, 0xA54, 0xA54, 1, 0x5ABu, A, 5, 4
#define PPUTLIMPL_UHEX_0xA53u 2643u, 0xA53, 0xA53, 1, 0x5ACu, A, 5, 3
#define PPUTLIMPL_UHEX_0xA52u 2642u, 0xA52, 0xA52, 1, 0x5ADu, A, 5, 2
#define PPUTLIMPL_UHEX_0xA51u 2641u, 0xA51, 0xA51, 1, 0x5AEu, A, 5, 1
#define PPUTLIMPL_UHEX_0xA50u 2640u, 0xA50, 0xA50, 1, 0x5AFu, A, 5, 0
#define PPUTLIMPL_UHEX_0xA4Fu 2639u, 0xA4F, 0xA4F, 1, 0x5B0u, A, 4, F
#define PPUTLIMPL_UHEX_0xA4Eu 2638u, 0xA4E, 0xA4E, 1, 0x5B1u, A, 4, E
#define PPUTLIMPL_UHEX_0xA4Du 2637u, 0xA4D, 0xA4D, 1, 0x5B2u, A, 4, D
#define PPUTLIMPL_UHEX_0xA4Cu 2636u, 0xA4C, 0xA4C, 1, 0x5B3u, A, 4, C
#define PPUTLIMPL_UHEX_0xA4Bu 2635u, 0xA4B, 0xA4B, 1, 0x5B4u, A, 4, B
#define PPUTLIMPL_UHEX_0xA4Au 2634u, 0xA4A, 0xA4A, 1, 0x5B5u, A, 4, A
#define PPUTLIMPL_UHEX_0xA49u 2633u, 0xA49, 0xA49, 1, 0x5B6u, A, 4, 9
#define PPUTLIMPL_UHEX_0xA48u 2632u, 0xA48, 0xA48, 1, 0x5B7u, A, 4, 8
#define PPUTLIMPL_UHEX_0xA47u 2631u, 0xA47, 0xA47, 1, 0x5B8u, A, 4, 7
#define PPUTLIMPL_UHEX_0xA46u 2630u, 0xA46, 0xA46, 1, 0x5B9u, A, 4, 6
#define PPUTLIMPL_UHEX_0xA45u 2629u, 0xA45, 0xA45, 1, 0x5BAu, A, 4, 5
#define PPUTLIMPL_UHEX_0xA44u 2628u, 0xA44, 0xA44, 1, 0x5BBu, A, 4, 4
#define PPUTLIMPL_UHEX_0xA43u 2627u, 0xA43, 0xA43, 1, 0x5BCu, A, 4, 3
#define PPUTLIMPL_UHEX_0xA42u 2626u, 0xA42, 0xA42, 1, 0x5BDu, A, 4, 2
#define PPUTLIMPL_UHEX_0xA41u 2625u, 0xA41, 0xA41, 1, 0x5BEu, A, 4, 1
#define PPUTLIMPL_UHEX_0xA40u 2624u, 0xA40, 0xA40, 1, 0x5BFu, A, 4, 0
#define PPUTLIMPL_UHEX_0xA3Fu 2623u, 0xA3F, 0xA3F, 1, 0x5C0u, A, 3, F
#define PPUTLIMPL_UHEX_0xA3Eu 2622u, 0xA3E, 0xA3E, 1, 0x5C1u, A, 3, E
#define PPUTLIMPL_UHEX_0xA3Du 2621u, 0xA3D, 0xA3D, 1, 0x5C2u, A, 3, D
#define PPUTLIMPL_UHEX_0xA3Cu 2620u, 0xA3C, 0xA3C, 1, 0x5C3u, A, 3, C
#define PPUTLIMPL_UHEX_0xA3Bu 2619u, 0xA3B, 0xA3B, 1, 0x5C4u, A, 3, B
#define PPUTLIMPL_UHEX_0xA3Au 2618u, 0xA3A, 0xA3A, 1, 0x5C5u, A, 3, A
#define PPUTLIMPL_UHEX_0xA39u 2617u, 0xA39, 0xA39, 1, 0x5C6u, A, 3, 9
#define PPUTLIMPL_UHEX_0xA38u 2616u, 0xA38, 0xA38, 1, 0x5C7u, A, 3, 8
#define PPUTLIMPL_UHEX_0xA37u 2615u, 0xA37, 0xA37, 1, 0x5C8u, A, 3, 7
#define PPUTLIMPL_UHEX_0xA36u 2614u, 0xA36, 0xA36, 1, 0x5C9u, A, 3, 6
#define PPUTLIMPL_UHEX_0xA35u 2613u, 0xA35, 0xA35, 1, 0x5CAu, A, 3, 5
#define PPUTLIMPL_UHEX_0xA34u 2612u, 0xA34, 0xA34, 1, 0x5CBu, A, 3, 4
#define PPUTLIMPL_UHEX_0xA33u 2611u, 0xA33, 0xA33, 1, 0x5CCu, A, 3, 3
#define PPUTLIMPL_UHEX_0xA32u 2610u, 0xA32, 0xA32, 1, 0x5CDu, A, 3, 2
#define PPUTLIMPL_UHEX_0xA31u 2609u, 0xA31, 0xA31, 1, 0x5CEu, A, 3, 1
#define PPUTLIMPL_UHEX_0xA30u 2608u, 0xA30, 0xA30, 1, 0x5CFu, A, 3, 0
#define PPUTLIMPL_UHEX_0xA2Fu 2607u, 0xA2F, 0xA2F, 1, 0x5D0u, A, 2, F
#define PPUTLIMPL_UHEX_0xA2Eu 2606u, 0xA2E, 0xA2E, 1, 0x5D1u, A, 2, E
#define PPUTLIMPL_UHEX_0xA2Du 2605u, 0xA2D, 0xA2D, 1, 0x5D2u, A, 2, D
#define PPUTLIMPL_UHEX_0xA2Cu 2604u, 0xA2C, 0xA2C, 1, 0x5D3u, A, 2, C
#define PPUTLIMPL_UHEX_0xA2Bu 2603u, 0xA2B, 0xA2B, 1, 0x5D4u, A, 2, B
#define PPUTLIMPL_UHEX_0xA2Au 2602u, 0xA2A, 0xA2A, 1, 0x5D5u, A, 2, A
#define PPUTLIMPL_UHEX_0xA29u 2601u, 0xA29, 0xA29, 1, 0x5D6u, A, 2, 9
#define PPUTLIMPL_UHEX_0xA28u 2600u, 0xA28, 0xA28, 1, 0x5D7u, A, 2, 8
#define PPUTLIMPL_UHEX_0xA27u 2599u, 0xA27, 0xA27, 1, 0x5D8u, A, 2, 7
#define PPUTLIMPL_UHEX_0xA26u 2598u, 0xA26, 0xA26, 1, 0x5D9u, A, 2, 6
#define PPUTLIMPL_UHEX_0xA25u 2597u, 0xA25, 0xA25, 1, 0x5DAu, A, 2, 5
#define PPUTLIMPL_UHEX_0xA24u 2596u, 0xA24, 0xA24, 1, 0x5DBu, A, 2, 4
#define PPUTLIMPL_UHEX_0xA23u 2595u, 0xA23, 0xA23, 1, 0x5DCu, A, 2, 3
#define PPUTLIMPL_UHEX_0xA22u 2594u, 0xA22, 0xA22, 1, 0x5DDu, A, 2, 2
#define PPUTLIMPL_UHEX_0xA21u 2593u, 0xA21, 0xA21, 1, 0x5DEu, A, 2, 1
#define PPUTLIMPL_UHEX_0xA20u 2592u, 0xA20, 0xA20, 1, 0x5DFu, A, 2, 0
#define PPUTLIMPL_UHEX_0xA1Fu 2591u, 0xA1F, 0xA1F, 1, 0x5E0u, A, 1, F
#define PPUTLIMPL_UHEX_0xA1Eu 2590u, 0xA1E, 0xA1E, 1, 0x5E1u, A, 1, E
#define PPUTLIMPL_UHEX_0xA1Du 2589u, 0xA1D, 0xA1D, 1, 0x5E2u, A, 1, D
#define PPUTLIMPL_UHEX_0xA1Cu 2588u, 0xA1C, 0xA1C, 1, 0x5E3u, A, 1, C
#define PPUTLIMPL_UHEX_0xA1Bu 2587u, 0xA1B, 0xA1B, 1, 0x5E4u, A, 1, B
#define PPUTLIMPL_UHEX_0xA1Au 2586u, 0xA1A, 0xA1A, 1, 0x5E5u, A, 1, A
#define PPUTLIMPL_UHEX_0xA19u 2585u, 0xA19, 0xA19, 1, 0x5E6u, A, 1, 9
#define PPUTLIMPL_UHEX_0xA18u 2584u, 0xA18, 0xA18, 1, 0x5E7u, A, 1, 8
#define PPUTLIMPL_UHEX_0xA17u 2583u, 0xA17, 0xA17, 1, 0x5E8u, A, 1, 7
#define PPUTLIMPL_UHEX_0xA16u 2582u, 0xA16, 0xA16, 1, 0x5E9u, A, 1, 6
#define PPUTLIMPL_UHEX_0xA15u 2581u, 0xA15, 0xA15, 1, 0x5EAu, A, 1, 5
#define PPUTLIMPL_UHEX_0xA14u 2580u, 0xA14, 0xA14, 1, 0x5EBu, A, 1, 4
#define PPUTLIMPL_UHEX_0xA13u 2579u, 0xA13, 0xA13, 1, 0x5ECu, A, 1, 3
#define PPUTLIMPL_UHEX_0xA12u 2578u, 0xA12, 0xA12, 1, 0x5EDu, A, 1, 2
#define PPUTLIMPL_UHEX_0xA11u 2577u, 0xA11, 0xA11, 1, 0x5EEu, A, 1, 1
#define PPUTLIMPL_UHEX_0xA10u 2576u, 0xA10, 0xA10, 1, 0x5EFu, A, 1, 0
#define PPUTLIMPL_UHEX_0xA0Fu 2575u, 0xA0F, 0xA0F, 1, 0x5F0u, A, 0, F
#define PPUTLIMPL_UHEX_0xA0Eu 2574u, 0xA0E, 0xA0E, 1, 0x5F1u, A, 0, E
#define PPUTLIMPL_UHEX_0xA0Du 2573u, 0xA0D, 0xA0D, 1, 0x5F2u, A, 0, D
#define PPUTLIMPL_UHEX_0xA0Cu 2572u, 0xA0C, 0xA0C, 1, 0x5F3u, A, 0, C
#define PPUTLIMPL_UHEX_0xA0Bu 2571u, 0xA0B, 0xA0B, 1, 0x5F4u, A, 0, B
#define PPUTLIMPL_UHEX_0xA0Au 2570u, 0xA0A, 0xA0A, 1, 0x5F5u, A, 0, A
#define PPUTLIMPL_UHEX_0xA09u 2569u, 0xA09, 0xA09, 1, 0x5F6u, A, 0, 9
#define PPUTLIMPL_UHEX_0xA08u 2568u, 0xA08, 0xA08, 1, 0x5F7u, A, 0, 8
#define PPUTLIMPL_UHEX_0xA07u 2567u, 0xA07, 0xA07, 1, 0x5F8u, A, 0, 7
#define PPUTLIMPL_UHEX_0xA06u 2566u, 0xA06, 0xA06, 1, 0x5F9u, A, 0, 6
#define PPUTLIMPL_UHEX_0xA05u 2565u, 0xA05, 0xA05, 1, 0x5FAu, A, 0, 5
#define PPUTLIMPL_UHEX_0xA04u 2564u, 0xA04, 0xA04, 1, 0x5FBu, A, 0, 4
#define PPUTLIMPL_UHEX_0xA03u 2563u, 0xA03, 0xA03, 1, 0x5FCu, A, 0, 3
#define PPUTLIMPL_UHEX_0xA02u 2562u, 0xA02, 0xA02, 1, 0x5FDu, A, 0, 2
#define PPUTLIMPL_UHEX_0xA01u 2561u, 0xA01, 0xA01, 1, 0x5FEu, A, 0, 1
#define PPUTLIMPL_UHEX_0xA00u 2560u, 0xA00, 0xA00, 1, 0x5FFu, A, 0, 0
#define PPUTLIMPL_UHEX_0x9FFu 2559u, 0x9FF, 0x9FF, 1, 0x600u, 9, F, F
#define PPUTLIMPL_UHEX_0x9FEu 2558u, 0x9FE, 0x9FE, 1, 0x601u, 9, F, E
#define PPUTLIMPL_UHEX_0x9FDu 2557u, 0x9FD, 0x9FD, 1, 0x602u, 9, F, D
#define PPUTLIMPL_UHEX_0x9FCu 2556u, 0x9FC, 0x9FC, 1, 0x603u, 9, F, C
#define PPUTLIMPL_UHEX_0x9FBu 2555u, 0x9FB, 0x9FB, 1, 0x604u, 9, F, B
#define PPUTLIMPL_UHEX_0x9FAu 2554u, 0x9FA, 0x9FA, 1, 0x605u, 9, F, A
#define PPUTLIMPL_UHEX_0x9F9u 2553u, 0x9F9, 0x9F9, 1, 0x606u, 9, F, 9
#define PPUTLIMPL_UHEX_0x9F8u 2552u, 0x9F8, 0x9F8, 1, 0x607u, 9, F, 8
#define PPUTLIMPL_UHEX_0x9F7u 2551u, 0x9F7, 0x9F7, 1, 0x608u, 9, F, 7
#define PPUTLIMPL_UHEX_0x9F6u 2550u, 0x9F6, 0x9F6, 1, 0x609u, 9, F, 6
#define PPUTLIMPL_UHEX_0x9F5u 2549u, 0x9F5, 0x9F5, 1, 0x60Au, 9, F, 5
#define PPUTLIMPL_UHEX_0x9F4u 2548u, 0x9F4, 0x9F4, 1, 0x60Bu, 9, F, 4
#define PPUTLIMPL_UHEX_0x9F3u 2547u, 0x9F3, 0x9F3, 1, 0x60Cu, 9, F, 3
#define PPUTLIMPL_UHEX_0x9F2u 2546u, 0x9F2, 0x9F2, 1, 0x60Du, 9, F, 2
#define PPUTLIMPL_UHEX_0x9F1u 2545u, 0x9F1, 0x9F1, 1, 0x60Eu, 9, F, 1
#define PPUTLIMPL_UHEX_0x9F0u 2544u, 0x9F0, 0x9F0, 1, 0x60Fu, 9, F, 0
#define PPUTLIMPL_UHEX_0x9EFu 2543u, 0x9EF, 0x9EF, 1, 0x610u, 9, E, F
#define PPUTLIMPL_UHEX_0x9EEu 2542u, 0x9EE, 0x9EE, 1, 0x611u, 9, E, E
#define PPUTLIMPL_UHEX_0x9EDu 2541u, 0x9ED, 0x9ED, 1, 0x612u, 9, E, D
#define PPUTLIMPL_UHEX_0x9ECu 2540u, 0x9EC, 0x9EC, 1, 0x613u, 9, E, C
#define PPUTLIMPL_UHEX_0x9EBu 2539u, 0x9EB, 0x9EB, 1, 0x614u, 9, E, B
#define PPUTLIMPL_UHEX_0x9EAu 2538u, 0x9EA, 0x9EA, 1, 0x615u, 9, E, A
#define PPUTLIMPL_UHEX_0x9E9u 2537u, 0x9E9, 0x9E9, 1, 0x616u, 9, E, 9
#define PPUTLIMPL_UHEX_0x9E8u 2536u, 0x9E8, 0x9E8, 1, 0x617u, 9, E, 8
#define PPUTLIMPL_UHEX_0x9E7u 2535u, 0x9E7, 0x9E7, 1, 0x618u, 9, E, 7
#define PPUTLIMPL_UHEX_0x9E6u 2534u, 0x9E6, 0x9E6, 1, 0x619u, 9, E, 6
#define PPUTLIMPL_UHEX_0x9E5u 2533u, 0x9E5, 0x9E5, 1, 0x61Au, 9, E, 5
#define PPUTLIMPL_UHEX_0x9E4u 2532u, 0x9E4, 0x9E4, 1, 0x61Bu, 9, E, 4
#define PPUTLIMPL_UHEX_0x9E3u 2531u, 0x9E3, 0x9E3, 1, 0x61Cu, 9, E, 3
#define PPUTLIMPL_UHEX_0x9E2u 2530u, 0x9E2, 0x9E2, 1, 0x61Du, 9, E, 2
#define PPUTLIMPL_UHEX_0x9E1u 2529u, 0x9E1, 0x9E1, 1, 0x61Eu, 9, E, 1
#define PPUTLIMPL_UHEX_0x9E0u 2528u, 0x9E0, 0x9E0, 1, 0x61Fu, 9, E, 0
#define PPUTLIMPL_UHEX_0x9DFu 2527u, 0x9DF, 0x9DF, 1, 0x620u, 9, D, F
#define PPUTLIMPL_UHEX_0x9DEu 2526u, 0x9DE, 0x9DE, 1, 0x621u, 9, D, E
#define PPUTLIMPL_UHEX_0x9DDu 2525u, 0x9DD, 0x9DD, 1, 0x622u, 9, D, D
#define PPUTLIMPL_UHEX_0x9DCu 2524u, 0x9DC, 0x9DC, 1, 0x623u, 9, D, C
#define PPUTLIMPL_UHEX_0x9DBu 2523u, 0x9DB, 0x9DB, 1, 0x624u, 9, D, B
#define PPUTLIMPL_UHEX_0x9DAu 2522u, 0x9DA, 0x9DA, 1, 0x625u, 9, D, A
#define PPUTLIMPL_UHEX_0x9D9u 2521u, 0x9D9, 0x9D9, 1, 0x626u, 9, D, 9
#define PPUTLIMPL_UHEX_0x9D8u 2520u, 0x9D8, 0x9D8, 1, 0x627u, 9, D, 8
#define PPUTLIMPL_UHEX_0x9D7u 2519u, 0x9D7, 0x9D7, 1, 0x628u, 9, D, 7
#define PPUTLIMPL_UHEX_0x9D6u 2518u, 0x9D6, 0x9D6, 1, 0x629u, 9, D, 6
#define PPUTLIMPL_UHEX_0x9D5u 2517u, 0x9D5, 0x9D5, 1, 0x62Au, 9, D, 5
#define PPUTLIMPL_UHEX_0x9D4u 2516u, 0x9D4, 0x9D4, 1, 0x62Bu, 9, D, 4
#define PPUTLIMPL_UHEX_0x9D3u 2515u, 0x9D3, 0x9D3, 1, 0x62Cu, 9, D, 3
#define PPUTLIMPL_UHEX_0x9D2u 2514u, 0x9D2, 0x9D2, 1, 0x62Du, 9, D, 2
#define PPUTLIMPL_UHEX_0x9D1u 2513u, 0x9D1, 0x9D1, 1, 0x62Eu, 9, D, 1
#define PPUTLIMPL_UHEX_0x9D0u 2512u, 0x9D0, 0x9D0, 1, 0x62Fu, 9, D, 0
#define PPUTLIMPL_UHEX_0x9CFu 2511u, 0x9CF, 0x9CF, 1, 0x630u, 9, C, F
#define PPUTLIMPL_UHEX_0x9CEu 2510u, 0x9CE, 0x9CE, 1, 0x631u, 9, C, E
#define PPUTLIMPL_UHEX_0x9CDu 2509u, 0x9CD, 0x9CD, 1, 0x632u, 9, C, D
#define PPUTLIMPL_UHEX_0x9CCu 2508u, 0x9CC, 0x9CC, 1, 0x633u, 9, C, C
#define PPUTLIMPL_UHEX_0x9CBu 2507u, 0x9CB, 0x9CB, 1, 0x634u, 9, C, B
#define PPUTLIMPL_UHEX_0x9CAu 2506u, 0x9CA, 0x9CA, 1, 0x635u, 9, C, A
#define PPUTLIMPL_UHEX_0x9C9u 2505u, 0x9C9, 0x9C9, 1, 0x636u, 9, C, 9
#define PPUTLIMPL_UHEX_0x9C8u 2504u, 0x9C8, 0x9C8, 1, 0x637u, 9, C, 8
#define PPUTLIMPL_UHEX_0x9C7u 2503u, 0x9C7, 0x9C7, 1, 0x638u, 9, C, 7
#define PPUTLIMPL_UHEX_0x9C6u 2502u, 0x9C6, 0x9C6, 1, 0x639u, 9, C, 6
#define PPUTLIMPL_UHEX_0x9C5u 2501u, 0x9C5, 0x9C5, 1, 0x63Au, 9, C, 5
#define PPUTLIMPL_UHEX_0x9C4u 2500u, 0x9C4, 0x9C4, 1, 0x63Bu, 9, C, 4
#define PPUTLIMPL_UHEX_0x9C3u 2499u, 0x9C3, 0x9C3, 1, 0x63Cu, 9, C, 3
#define PPUTLIMPL_UHEX_0x9C2u 2498u, 0x9C2, 0x9C2, 1, 0x63Du, 9, C, 2
#define PPUTLIMPL_UHEX_0x9C1u 2497u, 0x9C1, 0x9C1, 1, 0x63Eu, 9, C, 1
#define PPUTLIMPL_UHEX_0x9C0u 2496u, 0x9C0, 0x9C0, 1, 0x63Fu, 9, C, 0
#define PPUTLIMPL_UHEX_0x9BFu 2495u, 0x9BF, 0x9BF, 1, 0x640u, 9, B, F
#define PPUTLIMPL_UHEX_0x9BEu 2494u, 0x9BE, 0x9BE, 1, 0x641u, 9, B, E
#define PPUTLIMPL_UHEX_0x9BDu 2493u, 0x9BD, 0x9BD, 1, 0x642u, 9, B, D
#define PPUTLIMPL_UHEX_0x9BCu 2492u, 0x9BC, 0x9BC, 1, 0x643u, 9, B, C
#define PPUTLIMPL_UHEX_0x9BBu 2491u, 0x9BB, 0x9BB, 1, 0x644u, 9, B, B
#define PPUTLIMPL_UHEX_0x9BAu 2490u, 0x9BA, 0x9BA, 1, 0x645u, 9, B, A
#define PPUTLIMPL_UHEX_0x9B9u 2489u, 0x9B9, 0x9B9, 1, 0x646u, 9, B, 9
#define PPUTLIMPL_UHEX_0x9B8u 2488u, 0x9B8, 0x9B8, 1, 0x647u, 9, B, 8
#define PPUTLIMPL_UHEX_0x9B7u 2487u, 0x9B7, 0x9B7, 1, 0x648u, 9, B, 7
#define PPUTLIMPL_UHEX_0x9B6u 2486u, 0x9B6, 0x9B6, 1, 0x649u, 9, B, 6
#define PPUTLIMPL_UHEX_0x9B5u 2485u, 0x9B5, 0x9B5, 1, 0x64Au, 9, B, 5
#define PPUTLIMPL_UHEX_0x9B4u 2484u, 0x9B4, 0x9B4, 1, 0x64Bu, 9, B, 4
#define PPUTLIMPL_UHEX_0x9B3u 2483u, 0x9B3, 0x9B3, 1, 0x64Cu, 9, B, 3
#define PPUTLIMPL_UHEX_0x9B2u 2482u, 0x9B2, 0x9B2, 1, 0x64Du, 9, B, 2
#define PPUTLIMPL_UHEX_0x9B1u 2481u, 0x9B1, 0x9B1, 1, 0x64Eu, 9, B, 1
#define PPUTLIMPL_UHEX_0x9B0u 2480u, 0x9B0, 0x9B0, 1, 0x64Fu, 9, B, 0
#define PPUTLIMPL_UHEX_0x9AFu 2479u, 0x9AF, 0x9AF, 1, 0x650u, 9, A, F
#define PPUTLIMPL_UHEX_0x9AEu 2478u, 0x9AE, 0x9AE, 1, 0x651u, 9, A, E
#define PPUTLIMPL_UHEX_0x9ADu 2477u, 0x9AD, 0x9AD, 1, 0x652u, 9, A, D
#define PPUTLIMPL_UHEX_0x9ACu 2476u, 0x9AC, 0x9AC, 1, 0x653u, 9, A, C
#define PPUTLIMPL_UHEX_0x9ABu 2475u, 0x9AB, 0x9AB, 1, 0x654u, 9, A, B
#define PPUTLIMPL_UHEX_0x9AAu 2474u, 0x9AA, 0x9AA, 1, 0x655u, 9, A, A
#define PPUTLIMPL_UHEX_0x9A9u 2473u, 0x9A9, 0x9A9, 1, 0x656u, 9, A, 9
#define PPUTLIMPL_UHEX_0x9A8u 2472u, 0x9A8, 0x9A8, 1, 0x657u, 9, A, 8
#define PPUTLIMPL_UHEX_0x9A7u 2471u, 0x9A7, 0x9A7, 1, 0x658u, 9, A, 7
#define PPUTLIMPL_UHEX_0x9A6u 2470u, 0x9A6, 0x9A6, 1, 0x659u, 9, A, 6
#define PPUTLIMPL_UHEX_0x9A5u 2469u, 0x9A5, 0x9A5, 1, 0x65Au, 9, A, 5
#define PPUTLIMPL_UHEX_0x9A4u 2468u, 0x9A4, 0x9A4, 1, 0x65Bu, 9, A, 4
#define PPUTLIMPL_UHEX_0x9A3u 2467u, 0x9A3, 0x9A3, 1, 0x65Cu, 9, A, 3
#define PPUTLIMPL_UHEX_0x9A2u 2466u, 0x9A2, 0x9A2, 1, 0x65Du, 9, A, 2
#define PPUTLIMPL_UHEX_0x9A1u 2465u, 0x9A1, 0x9A1, 1, 0x65Eu, 9, A, 1
#define PPUTLIMPL_UHEX_0x9A0u 2464u, 0x9A0, 0x9A0, 1, 0x65Fu, 9, A, 0
#define PPUTLIMPL_UHEX_0x99Fu 2463u, 0x99F, 0x99F, 1, 0x660u, 9, 9, F
#define PPUTLIMPL_UHEX_0x99Eu 2462u, 0x99E, 0x99E, 1, 0x661u, 9, 9, E
#define PPUTLIMPL_UHEX_0x99Du 2461u, 0x99D, 0x99D, 1, 0x662u, 9, 9, D
#define PPUTLIMPL_UHEX_0x99Cu 2460u, 0x99C, 0x99C, 1, 0x663u, 9, 9, C
#define PPUTLIMPL_UHEX_0x99Bu 2459u, 0x99B, 0x99B, 1, 0x664u, 9, 9, B
#define PPUTLIMPL_UHEX_0x99Au 2458u, 0x99A, 0x99A, 1, 0x665u, 9, 9, A
#define PPUTLIMPL_UHEX_0x999u 2457u, 0x999, 0x999, 1, 0x666u, 9, 9, 9
#define PPUTLIMPL_UHEX_0x998u 2456u, 0x998, 0x998, 1, 0x667u, 9, 9, 8
#define PPUTLIMPL_UHEX_0x997u 2455u, 0x997, 0x997, 1, 0x668u, 9, 9, 7
#define PPUTLIMPL_UHEX_0x996u 2454u, 0x996, 0x996, 1, 0x669u, 9, 9, 6
#define PPUTLIMPL_UHEX_0x995u 2453u, 0x995, 0x995, 1, 0x66Au, 9, 9, 5
#define PPUTLIMPL_UHEX_0x994u 2452u, 0x994, 0x994, 1, 0x66Bu, 9, 9, 4
#define PPUTLIMPL_UHEX_0x993u 2451u, 0x993, 0x993, 1, 0x66Cu, 9, 9, 3
#define PPUTLIMPL_UHEX_0x992u 2450u, 0x992, 0x992, 1, 0x66Du, 9, 9, 2
#define PPUTLIMPL_UHEX_0x991u 2449u, 0x991, 0x991, 1, 0x66Eu, 9, 9, 1
#define PPUTLIMPL_UHEX_0x990u 2448u, 0x990, 0x990, 1, 0x66Fu, 9, 9, 0
#define PPUTLIMPL_UHEX_0x98Fu 2447u, 0x98F, 0x98F, 1, 0x670u, 9, 8, F
#define PPUTLIMPL_UHEX_0x98Eu 2446u, 0x98E, 0x98E, 1, 0x671u, 9, 8, E
#define PPUTLIMPL_UHEX_0x98Du 2445u, 0x98D, 0x98D, 1, 0x672u, 9, 8, D
#define PPUTLIMPL_UHEX_0x98Cu 2444u, 0x98C, 0x98C, 1, 0x673u, 9, 8, C
#define PPUTLIMPL_UHEX_0x98Bu 2443u, 0x98B, 0x98B, 1, 0x674u, 9, 8, B
#define PPUTLIMPL_UHEX_0x98Au 2442u, 0x98A, 0x98A, 1, 0x675u, 9, 8, A
#define PPUTLIMPL_UHEX_0x989u 2441u, 0x989, 0x989, 1, 0x676u, 9, 8, 9
#define PPUTLIMPL_UHEX_0x988u 2440u, 0x988, 0x988, 1, 0x677u, 9, 8, 8
#define PPUTLIMPL_UHEX_0x987u 2439u, 0x987, 0x987, 1, 0x678u, 9, 8, 7
#define PPUTLIMPL_UHEX_0x986u 2438u, 0x986, 0x986, 1, 0x679u, 9, 8, 6
#define PPUTLIMPL_UHEX_0x985u 2437u, 0x985, 0x985, 1, 0x67Au, 9, 8, 5
#define PPUTLIMPL_UHEX_0x984u 2436u, 0x984, 0x984, 1, 0x67Bu, 9, 8, 4
#define PPUTLIMPL_UHEX_0x983u 2435u, 0x983, 0x983, 1, 0x67Cu, 9, 8, 3
#define PPUTLIMPL_UHEX_0x982u 2434u, 0x982, 0x982, 1, 0x67Du, 9, 8, 2
#define PPUTLIMPL_UHEX_0x981u 2433u, 0x981, 0x981, 1, 0x67Eu, 9, 8, 1
#define PPUTLIMPL_UHEX_0x980u 2432u, 0x980, 0x980, 1, 0x67Fu, 9, 8, 0
#define PPUTLIMPL_UHEX_0x97Fu 2431u, 0x97F, 0x97F, 1, 0x680u, 9, 7, F
#define PPUTLIMPL_UHEX_0x97Eu 2430u, 0x97E, 0x97E, 1, 0x681u, 9, 7, E
#define PPUTLIMPL_UHEX_0x97Du 2429u, 0x97D, 0x97D, 1, 0x682u, 9, 7, D
#define PPUTLIMPL_UHEX_0x97Cu 2428u, 0x97C, 0x97C, 1, 0x683u, 9, 7, C
#define PPUTLIMPL_UHEX_0x97Bu 2427u, 0x97B, 0x97B, 1, 0x684u, 9, 7, B
#define PPUTLIMPL_UHEX_0x97Au 2426u, 0x97A, 0x97A, 1, 0x685u, 9, 7, A
#define PPUTLIMPL_UHEX_0x979u 2425u, 0x979, 0x979, 1, 0x686u, 9, 7, 9
#define PPUTLIMPL_UHEX_0x978u 2424u, 0x978, 0x978, 1, 0x687u, 9, 7, 8
#define PPUTLIMPL_UHEX_0x977u 2423u, 0x977, 0x977, 1, 0x688u, 9, 7, 7
#define PPUTLIMPL_UHEX_0x976u 2422u, 0x976, 0x976, 1, 0x689u, 9, 7, 6
#define PPUTLIMPL_UHEX_0x975u 2421u, 0x975, 0x975, 1, 0x68Au, 9, 7, 5
#define PPUTLIMPL_UHEX_0x974u 2420u, 0x974, 0x974, 1, 0x68Bu, 9, 7, 4
#define PPUTLIMPL_UHEX_0x973u 2419u, 0x973, 0x973, 1, 0x68Cu, 9, 7, 3
#define PPUTLIMPL_UHEX_0x972u 2418u, 0x972, 0x972, 1, 0x68Du, 9, 7, 2
#define PPUTLIMPL_UHEX_0x971u 2417u, 0x971, 0x971, 1, 0x68Eu, 9, 7, 1
#define PPUTLIMPL_UHEX_0x970u 2416u, 0x970, 0x970, 1, 0x68Fu, 9, 7, 0
#define PPUTLIMPL_UHEX_0x96Fu 2415u, 0x96F, 0x96F, 1, 0x690u, 9, 6, F
#define PPUTLIMPL_UHEX_0x96Eu 2414u, 0x96E, 0x96E, 1, 0x691u, 9, 6, E
#define PPUTLIMPL_UHEX_0x96Du 2413u, 0x96D, 0x96D, 1, 0x692u, 9, 6, D
#define PPUTLIMPL_UHEX_0x96Cu 2412u, 0x96C, 0x96C, 1, 0x693u, 9, 6, C
#define PPUTLIMPL_UHEX_0x96Bu 2411u, 0x96B, 0x96B, 1, 0x694u, 9, 6, B
#define PPUTLIMPL_UHEX_0x96Au 2410u, 0x96A, 0x96A, 1, 0x695u, 9, 6, A
#define PPUTLIMPL_UHEX_0x969u 2409u, 0x969, 0x969, 1, 0x696u, 9, 6, 9
#define PPUTLIMPL_UHEX_0x968u 2408u, 0x968, 0x968, 1, 0x697u, 9, 6, 8
#define PPUTLIMPL_UHEX_0x967u 2407u, 0x967, 0x967, 1, 0x698u, 9, 6, 7
#define PPUTLIMPL_UHEX_0x966u 2406u, 0x966, 0x966, 1, 0x699u, 9, 6, 6
#define PPUTLIMPL_UHEX_0x965u 2405u, 0x965, 0x965, 1, 0x69Au, 9, 6, 5
#define PPUTLIMPL_UHEX_0x964u 2404u, 0x964, 0x964, 1, 0x69Bu, 9, 6, 4
#define PPUTLIMPL_UHEX_0x963u 2403u, 0x963, 0x963, 1, 0x69Cu, 9, 6, 3
#define PPUTLIMPL_UHEX_0x962u 2402u, 0x962, 0x962, 1, 0x69Du, 9, 6, 2
#define PPUTLIMPL_UHEX_0x961u 2401u, 0x961, 0x961, 1, 0x69Eu, 9, 6, 1
#define PPUTLIMPL_UHEX_0x960u 2400u, 0x960, 0x960, 1, 0x69Fu, 9, 6, 0
#define PPUTLIMPL_UHEX_0x95Fu 2399u, 0x95F, 0x95F, 1, 0x6A0u, 9, 5, F
#define PPUTLIMPL_UHEX_0x95Eu 2398u, 0x95E, 0x95E, 1, 0x6A1u, 9, 5, E
#define PPUTLIMPL_UHEX_0x95Du 2397u, 0x95D, 0x95D, 1, 0x6A2u, 9, 5, D
#define PPUTLIMPL_UHEX_0x95Cu 2396u, 0x95C, 0x95C, 1, 0x6A3u, 9, 5, C
#define PPUTLIMPL_UHEX_0x95Bu 2395u, 0x95B, 0x95B, 1, 0x6A4u, 9, 5, B
#define PPUTLIMPL_UHEX_0x95Au 2394u, 0x95A, 0x95A, 1, 0x6A5u, 9, 5, A
#define PPUTLIMPL_UHEX_0x959u 2393u, 0x959, 0x959, 1, 0x6A6u, 9, 5, 9
#define PPUTLIMPL_UHEX_0x958u 2392u, 0x958, 0x958, 1, 0x6A7u, 9, 5, 8
#define PPUTLIMPL_UHEX_0x957u 2391u, 0x957, 0x957, 1, 0x6A8u, 9, 5, 7
#define PPUTLIMPL_UHEX_0x956u 2390u, 0x956, 0x956, 1, 0x6A9u, 9, 5, 6
#define PPUTLIMPL_UHEX_0x955u 2389u, 0x955, 0x955, 1, 0x6AAu, 9, 5, 5
#define PPUTLIMPL_UHEX_0x954u 2388u, 0x954, 0x954, 1, 0x6ABu, 9, 5, 4
#define PPUTLIMPL_UHEX_0x953u 2387u, 0x953, 0x953, 1, 0x6ACu, 9, 5, 3
#define PPUTLIMPL_UHEX_0x952u 2386u, 0x952, 0x952, 1, 0x6ADu, 9, 5, 2
#define PPUTLIMPL_UHEX_0x951u 2385u, 0x951, 0x951, 1, 0x6AEu, 9, 5, 1
#define PPUTLIMPL_UHEX_0x950u 2384u, 0x950, 0x950, 1, 0x6AFu, 9, 5, 0
#define PPUTLIMPL_UHEX_0x94Fu 2383u, 0x94F, 0x94F, 1, 0x6B0u, 9, 4, F
#define PPUTLIMPL_UHEX_0x94Eu 2382u, 0x94E, 0x94E, 1, 0x6B1u, 9, 4, E
#define PPUTLIMPL_UHEX_0x94Du 2381u, 0x94D, 0x94D, 1, 0x6B2u, 9, 4, D
#define PPUTLIMPL_UHEX_0x94Cu 2380u, 0x94C, 0x94C, 1, 0x6B3u, 9, 4, C
#define PPUTLIMPL_UHEX_0x94Bu 2379u, 0x94B, 0x94B, 1, 0x6B4u, 9, 4, B
#define PPUTLIMPL_UHEX_0x94Au 2378u, 0x94A, 0x94A, 1, 0x6B5u, 9, 4, A
#define PPUTLIMPL_UHEX_0x949u 2377u, 0x949, 0x949, 1, 0x6B6u, 9, 4, 9
#define PPUTLIMPL_UHEX_0x948u 2376u, 0x948, 0x948, 1, 0x6B7u, 9, 4, 8
#define PPUTLIMPL_UHEX_0x947u 2375u, 0x947, 0x947, 1, 0x6B8u, 9, 4, 7
#define PPUTLIMPL_UHEX_0x946u 2374u, 0x946, 0x946, 1, 0x6B9u, 9, 4, 6
#define PPUTLIMPL_UHEX_0x945u 2373u, 0x945, 0x945, 1, 0x6BAu, 9, 4, 5
#define PPUTLIMPL_UHEX_0x944u 2372u, 0x944, 0x944, 1, 0x6BBu, 9, 4, 4
#define PPUTLIMPL_UHEX_0x943u 2371u, 0x943, 0x943, 1, 0x6BCu, 9, 4, 3
#define PPUTLIMPL_UHEX_0x942u 2370u, 0x942, 0x942, 1, 0x6BDu, 9, 4, 2
#define PPUTLIMPL_UHEX_0x941u 2369u, 0x941, 0x941, 1, 0x6BEu, 9, 4, 1
#define PPUTLIMPL_UHEX_0x940u 2368u, 0x940, 0x940, 1, 0x6BFu, 9, 4, 0
#define PPUTLIMPL_UHEX_0x93Fu 2367u, 0x93F, 0x93F, 1, 0x6C0u, 9, 3, F
#define PPUTLIMPL_UHEX_0x93Eu 2366u, 0x93E, 0x93E, 1, 0x6C1u, 9, 3, E
#define PPUTLIMPL_UHEX_0x93Du 2365u, 0x93D, 0x93D, 1, 0x6C2u, 9, 3, D
#define PPUTLIMPL_UHEX_0x93Cu 2364u, 0x93C, 0x93C, 1, 0x6C3u, 9, 3, C
#define PPUTLIMPL_UHEX_0x93Bu 2363u, 0x93B, 0x93B, 1, 0x6C4u, 9, 3, B
#define PPUTLIMPL_UHEX_0x93Au 2362u, 0x93A, 0x93A, 1, 0x6C5u, 9, 3, A
#define PPUTLIMPL_UHEX_0x939u 2361u, 0x939, 0x939, 1, 0x6C6u, 9, 3, 9
#define PPUTLIMPL_UHEX_0x938u 2360u, 0x938, 0x938, 1, 0x6C7u, 9, 3, 8
#define PPUTLIMPL_UHEX_0x937u 2359u, 0x937, 0x937, 1, 0x6C8u, 9, 3, 7
#define PPUTLIMPL_UHEX_0x936u 2358u, 0x936, 0x936, 1, 0x6C9u, 9, 3, 6
#define PPUTLIMPL_UHEX_0x935u 2357u, 0x935, 0x935, 1, 0x6CAu, 9, 3, 5
#define PPUTLIMPL_UHEX_0x934u 2356u, 0x934, 0x934, 1, 0x6CBu, 9, 3, 4
#define PPUTLIMPL_UHEX_0x933u 2355u, 0x933, 0x933, 1, 0x6CCu, 9, 3, 3
#define PPUTLIMPL_UHEX_0x932u 2354u, 0x932, 0x932, 1, 0x6CDu, 9, 3, 2
#define PPUTLIMPL_UHEX_0x931u 2353u, 0x931, 0x931, 1, 0x6CEu, 9, 3, 1
#define PPUTLIMPL_UHEX_0x930u 2352u, 0x930, 0x930, 1, 0x6CFu, 9, 3, 0
#define PPUTLIMPL_UHEX_0x92Fu 2351u, 0x92F, 0x92F, 1, 0x6D0u, 9, 2, F
#define PPUTLIMPL_UHEX_0x92Eu 2350u, 0x92E, 0x92E, 1, 0x6D1u, 9, 2, E
#define PPUTLIMPL_UHEX_0x92Du 2349u, 0x92D, 0x92D, 1, 0x6D2u, 9, 2, D
#define PPUTLIMPL_UHEX_0x92Cu 2348u, 0x92C, 0x92C, 1, 0x6D3u, 9, 2, C
#define PPUTLIMPL_UHEX_0x92Bu 2347u, 0x92B, 0x92B, 1, 0x6D4u, 9, 2, B
#define PPUTLIMPL_UHEX_0x92Au 2346u, 0x92A, 0x92A, 1, 0x6D5u, 9, 2, A
#define PPUTLIMPL_UHEX_0x929u 2345u, 0x929, 0x929, 1, 0x6D6u, 9, 2, 9
#define PPUTLIMPL_UHEX_0x928u 2344u, 0x928, 0x928, 1, 0x6D7u, 9, 2, 8
#define PPUTLIMPL_UHEX_0x927u 2343u, 0x927, 0x927, 1, 0x6D8u, 9, 2, 7
#define PPUTLIMPL_UHEX_0x926u 2342u, 0x926, 0x926, 1, 0x6D9u, 9, 2, 6
#define PPUTLIMPL_UHEX_0x925u 2341u, 0x925, 0x925, 1, 0x6DAu, 9, 2, 5
#define PPUTLIMPL_UHEX_0x924u 2340u, 0x924, 0x924, 1, 0x6DBu, 9, 2, 4
#define PPUTLIMPL_UHEX_0x923u 2339u, 0x923, 0x923, 1, 0x6DCu, 9, 2, 3
#define PPUTLIMPL_UHEX_0x922u 2338u, 0x922, 0x922, 1, 0x6DDu, 9, 2, 2
#define PPUTLIMPL_UHEX_0x921u 2337u, 0x921, 0x921, 1, 0x6DEu, 9, 2, 1
#define PPUTLIMPL_UHEX_0x920u 2336u, 0x920, 0x920, 1, 0x6DFu, 9, 2, 0
#define PPUTLIMPL_UHEX_0x91Fu 2335u, 0x91F, 0x91F, 1, 0x6E0u, 9, 1, F
#define PPUTLIMPL_UHEX_0x91Eu 2334u, 0x91E, 0x91E, 1, 0x6E1u, 9, 1, E
#define PPUTLIMPL_UHEX_0x91Du 2333u, 0x91D, 0x91D, 1, 0x6E2u, 9, 1, D
#define PPUTLIMPL_UHEX_0x91Cu 2332u, 0x91C, 0x91C, 1, 0x6E3u, 9, 1, C
#define PPUTLIMPL_UHEX_0x91Bu 2331u, 0x91B, 0x91B, 1, 0x6E4u, 9, 1, B
#define PPUTLIMPL_UHEX_0x91Au 2330u, 0x91A, 0x91A, 1, 0x6E5u, 9, 1, A
#define PPUTLIMPL_UHEX_0x919u 2329u, 0x919, 0x919, 1, 0x6E6u, 9, 1, 9
#define PPUTLIMPL_UHEX_0x918u 2328u, 0x918, 0x918, 1, 0x6E7u, 9, 1, 8
#define PPUTLIMPL_UHEX_0x917u 2327u, 0x917, 0x917, 1, 0x6E8u, 9, 1, 7
#define PPUTLIMPL_UHEX_0x916u 2326u, 0x916, 0x916, 1, 0x6E9u, 9, 1, 6
#define PPUTLIMPL_UHEX_0x915u 2325u, 0x915, 0x915, 1, 0x6EAu, 9, 1, 5
#define PPUTLIMPL_UHEX_0x914u 2324u, 0x914, 0x914, 1, 0x6EBu, 9, 1, 4
#define PPUTLIMPL_UHEX_0x913u 2323u, 0x913, 0x913, 1, 0x6ECu, 9, 1, 3
#define PPUTLIMPL_UHEX_0x912u 2322u, 0x912, 0x912, 1, 0x6EDu, 9, 1, 2
#define PPUTLIMPL_UHEX_0x911u 2321u, 0x911, 0x911, 1, 0x6EEu, 9, 1, 1
#define PPUTLIMPL_UHEX_0x910u 2320u, 0x910, 0x910, 1, 0x6EFu, 9, 1, 0
#define PPUTLIMPL_UHEX_0x90Fu 2319u, 0x90F, 0x90F, 1, 0x6F0u, 9, 0, F
#define PPUTLIMPL_UHEX_0x90Eu 2318u, 0x90E, 0x90E, 1, 0x6F1u, 9, 0, E
#define PPUTLIMPL_UHEX_0x90Du 2317u, 0x90D, 0x90D, 1, 0x6F2u, 9, 0, D
#define PPUTLIMPL_UHEX_0x90Cu 2316u, 0x90C, 0x90C, 1, 0x6F3u, 9, 0, C
#define PPUTLIMPL_UHEX_0x90Bu 2315u, 0x90B, 0x90B, 1, 0x6F4u, 9, 0, B
#define PPUTLIMPL_UHEX_0x90Au 2314u, 0x90A, 0x90A, 1, 0x6F5u, 9, 0, A
#define PPUTLIMPL_UHEX_0x909u 2313u, 0x909, 0x909, 1, 0x6F6u, 9, 0, 9
#define PPUTLIMPL_UHEX_0x908u 2312u, 0x908, 0x908, 1, 0x6F7u, 9, 0, 8
#define PPUTLIMPL_UHEX_0x907u 2311u, 0x907, 0x907, 1, 0x6F8u, 9, 0, 7
#define PPUTLIMPL_UHEX_0x906u 2310u, 0x906, 0x906, 1, 0x6F9u, 9, 0, 6
#define PPUTLIMPL_UHEX_0x905u 2309u, 0x905, 0x905, 1, 0x6FAu, 9, 0, 5
#define PPUTLIMPL_UHEX_0x904u 2308u, 0x904, 0x904, 1, 0x6FBu, 9, 0, 4
#define PPUTLIMPL_UHEX_0x903u 2307u, 0x903, 0x903, 1, 0x6FCu, 9, 0, 3
#define PPUTLIMPL_UHEX_0x902u 2306u, 0x902, 0x902, 1, 0x6FDu, 9, 0, 2
#define PPUTLIMPL_UHEX_0x901u 2305u, 0x901, 0x901, 1, 0x6FEu, 9, 0, 1
#define PPUTLIMPL_UHEX_0x900u 2304u, 0x900, 0x900, 1, 0x6FFu, 9, 0, 0
#define PPUTLIMPL_UHEX_0x8FFu 2303u, 0x8FF, 0x8FF, 1, 0x700u, 8, F, F
#define PPUTLIMPL_UHEX_0x8FEu 2302u, 0x8FE, 0x8FE, 1, 0x701u, 8, F, E
#define PPUTLIMPL_UHEX_0x8FDu 2301u, 0x8FD, 0x8FD, 1, 0x702u, 8, F, D
#define PPUTLIMPL_UHEX_0x8FCu 2300u, 0x8FC, 0x8FC, 1, 0x703u, 8, F, C
#define PPUTLIMPL_UHEX_0x8FBu 2299u, 0x8FB, 0x8FB, 1, 0x704u, 8, F, B
#define PPUTLIMPL_UHEX_0x8FAu 2298u, 0x8FA, 0x8FA, 1, 0x705u, 8, F, A
#define PPUTLIMPL_UHEX_0x8F9u 2297u, 0x8F9, 0x8F9, 1, 0x706u, 8, F, 9
#define PPUTLIMPL_UHEX_0x8F8u 2296u, 0x8F8, 0x8F8, 1, 0x707u, 8, F, 8
#define PPUTLIMPL_UHEX_0x8F7u 2295u, 0x8F7, 0x8F7, 1, 0x708u, 8, F, 7
#define PPUTLIMPL_UHEX_0x8F6u 2294u, 0x8F6, 0x8F6, 1, 0x709u, 8, F, 6
#define PPUTLIMPL_UHEX_0x8F5u 2293u, 0x8F5, 0x8F5, 1, 0x70Au, 8, F, 5
#define PPUTLIMPL_UHEX_0x8F4u 2292u, 0x8F4, 0x8F4, 1, 0x70Bu, 8, F, 4
#define PPUTLIMPL_UHEX_0x8F3u 2291u, 0x8F3, 0x8F3, 1, 0x70Cu, 8, F, 3
#define PPUTLIMPL_UHEX_0x8F2u 2290u, 0x8F2, 0x8F2, 1, 0x70Du, 8, F, 2
#define PPUTLIMPL_UHEX_0x8F1u 2289u, 0x8F1, 0x8F1, 1, 0x70Eu, 8, F, 1
#define PPUTLIMPL_UHEX_0x8F0u 2288u, 0x8F0, 0x8F0, 1, 0x70Fu, 8, F, 0
#define PPUTLIMPL_UHEX_0x8EFu 2287u, 0x8EF, 0x8EF, 1, 0x710u, 8, E, F
#define PPUTLIMPL_UHEX_0x8EEu 2286u, 0x8EE, 0x8EE, 1, 0x711u, 8, E, E
#define PPUTLIMPL_UHEX_0x8EDu 2285u, 0x8ED, 0x8ED, 1, 0x712u, 8, E, D
#define PPUTLIMPL_UHEX_0x8ECu 2284u, 0x8EC, 0x8EC, 1, 0x713u, 8, E, C
#define PPUTLIMPL_UHEX_0x8EBu 2283u, 0x8EB, 0x8EB, 1, 0x714u, 8, E, B
#define PPUTLIMPL_UHEX_0x8EAu 2282u, 0x8EA, 0x8EA, 1, 0x715u, 8, E, A
#define PPUTLIMPL_UHEX_0x8E9u 2281u, 0x8E9, 0x8E9, 1, 0x716u, 8, E, 9
#define PPUTLIMPL_UHEX_0x8E8u 2280u, 0x8E8, 0x8E8, 1, 0x717u, 8, E, 8
#define PPUTLIMPL_UHEX_0x8E7u 2279u, 0x8E7, 0x8E7, 1, 0x718u, 8, E, 7
#define PPUTLIMPL_UHEX_0x8E6u 2278u, 0x8E6, 0x8E6, 1, 0x719u, 8, E, 6
#define PPUTLIMPL_UHEX_0x8E5u 2277u, 0x8E5, 0x8E5, 1, 0x71Au, 8, E, 5
#define PPUTLIMPL_UHEX_0x8E4u 2276u, 0x8E4, 0x8E4, 1, 0x71Bu, 8, E, 4
#define PPUTLIMPL_UHEX_0x8E3u 2275u, 0x8E3, 0x8E3, 1, 0x71Cu, 8, E, 3
#define PPUTLIMPL_UHEX_0x8E2u 2274u, 0x8E2, 0x8E2, 1, 0x71Du, 8, E, 2
#define PPUTLIMPL_UHEX_0x8E1u 2273u, 0x8E1, 0x8E1, 1, 0x71Eu, 8, E, 1
#define PPUTLIMPL_UHEX_0x8E0u 2272u, 0x8E0, 0x8E0, 1, 0x71Fu, 8, E, 0
#define PPUTLIMPL_UHEX_0x8DFu 2271u, 0x8DF, 0x8DF, 1, 0x720u, 8, D, F
#define PPUTLIMPL_UHEX_0x8DEu 2270u, 0x8DE, 0x8DE, 1, 0x721u, 8, D, E
#define PPUTLIMPL_UHEX_0x8DDu 2269u, 0x8DD, 0x8DD, 1, 0x722u, 8, D, D
#define PPUTLIMPL_UHEX_0x8DCu 2268u, 0x8DC, 0x8DC, 1, 0x723u, 8, D, C
#define PPUTLIMPL_UHEX_0x8DBu 2267u, 0x8DB, 0x8DB, 1, 0x724u, 8, D, B
#define PPUTLIMPL_UHEX_0x8DAu 2266u, 0x8DA, 0x8DA, 1, 0x725u, 8, D, A
#define PPUTLIMPL_UHEX_0x8D9u 2265u, 0x8D9, 0x8D9, 1, 0x726u, 8, D, 9
#define PPUTLIMPL_UHEX_0x8D8u 2264u, 0x8D8, 0x8D8, 1, 0x727u, 8, D, 8
#define PPUTLIMPL_UHEX_0x8D7u 2263u, 0x8D7, 0x8D7, 1, 0x728u, 8, D, 7
#define PPUTLIMPL_UHEX_0x8D6u 2262u, 0x8D6, 0x8D6, 1, 0x729u, 8, D, 6
#define PPUTLIMPL_UHEX_0x8D5u 2261u, 0x8D5, 0x8D5, 1, 0x72Au, 8, D, 5
#define PPUTLIMPL_UHEX_0x8D4u 2260u, 0x8D4, 0x8D4, 1, 0x72Bu, 8, D, 4
#define PPUTLIMPL_UHEX_0x8D3u 2259u, 0x8D3, 0x8D3, 1, 0x72Cu, 8, D, 3
#define PPUTLIMPL_UHEX_0x8D2u 2258u, 0x8D2, 0x8D2, 1, 0x72Du, 8, D, 2
#define PPUTLIMPL_UHEX_0x8D1u 2257u, 0x8D1, 0x8D1, 1, 0x72Eu, 8, D, 1
#define PPUTLIMPL_UHEX_0x8D0u 2256u, 0x8D0, 0x8D0, 1, 0x72Fu, 8, D, 0
#define PPUTLIMPL_UHEX_0x8CFu 2255u, 0x8CF, 0x8CF, 1, 0x730u, 8, C, F
#define PPUTLIMPL_UHEX_0x8CEu 2254u, 0x8CE, 0x8CE, 1, 0x731u, 8, C, E
#define PPUTLIMPL_UHEX_0x8CDu 2253u, 0x8CD, 0x8CD, 1, 0x732u, 8, C, D
#define PPUTLIMPL_UHEX_0x8CCu 2252u, 0x8CC, 0x8CC, 1, 0x733u, 8, C, C
#define PPUTLIMPL_UHEX_0x8CBu 2251u, 0x8CB, 0x8CB, 1, 0x734u, 8, C, B
#define PPUTLIMPL_UHEX_0x8CAu 2250u, 0x8CA, 0x8CA, 1, 0x735u, 8, C, A
#define PPUTLIMPL_UHEX_0x8C9u 2249u, 0x8C9, 0x8C9, 1, 0x736u, 8, C, 9
#define PPUTLIMPL_UHEX_0x8C8u 2248u, 0x8C8, 0x8C8, 1, 0x737u, 8, C, 8
#define PPUTLIMPL_UHEX_0x8C7u 2247u, 0x8C7, 0x8C7, 1, 0x738u, 8, C, 7
#define PPUTLIMPL_UHEX_0x8C6u 2246u, 0x8C6, 0x8C6, 1, 0x739u, 8, C, 6
#define PPUTLIMPL_UHEX_0x8C5u 2245u, 0x8C5, 0x8C5, 1, 0x73Au, 8, C, 5
#define PPUTLIMPL_UHEX_0x8C4u 2244u, 0x8C4, 0x8C4, 1, 0x73Bu, 8, C, 4
#define PPUTLIMPL_UHEX_0x8C3u 2243u, 0x8C3, 0x8C3, 1, 0x73Cu, 8, C, 3
#define PPUTLIMPL_UHEX_0x8C2u 2242u, 0x8C2, 0x8C2, 1, 0x73Du, 8, C, 2
#define PPUTLIMPL_UHEX_0x8C1u 2241u, 0x8C1, 0x8C1, 1, 0x73Eu, 8, C, 1
#define PPUTLIMPL_UHEX_0x8C0u 2240u, 0x8C0, 0x8C0, 1, 0x73Fu, 8, C, 0
#define PPUTLIMPL_UHEX_0x8BFu 2239u, 0x8BF, 0x8BF, 1, 0x740u, 8, B, F
#define PPUTLIMPL_UHEX_0x8BEu 2238u, 0x8BE, 0x8BE, 1, 0x741u, 8, B, E
#define PPUTLIMPL_UHEX_0x8BDu 2237u, 0x8BD, 0x8BD, 1, 0x742u, 8, B, D
#define PPUTLIMPL_UHEX_0x8BCu 2236u, 0x8BC, 0x8BC, 1, 0x743u, 8, B, C
#define PPUTLIMPL_UHEX_0x8BBu 2235u, 0x8BB, 0x8BB, 1, 0x744u, 8, B, B
#define PPUTLIMPL_UHEX_0x8BAu 2234u, 0x8BA, 0x8BA, 1, 0x745u, 8, B, A
#define PPUTLIMPL_UHEX_0x8B9u 2233u, 0x8B9, 0x8B9, 1, 0x746u, 8, B, 9
#define PPUTLIMPL_UHEX_0x8B8u 2232u, 0x8B8, 0x8B8, 1, 0x747u, 8, B, 8
#define PPUTLIMPL_UHEX_0x8B7u 2231u, 0x8B7, 0x8B7, 1, 0x748u, 8, B, 7
#define PPUTLIMPL_UHEX_0x8B6u 2230u, 0x8B6, 0x8B6, 1, 0x749u, 8, B, 6
#define PPUTLIMPL_UHEX_0x8B5u 2229u, 0x8B5, 0x8B5, 1, 0x74Au, 8, B, 5
#define PPUTLIMPL_UHEX_0x8B4u 2228u, 0x8B4, 0x8B4, 1, 0x74Bu, 8, B, 4
#define PPUTLIMPL_UHEX_0x8B3u 2227u, 0x8B3, 0x8B3, 1, 0x74Cu, 8, B, 3
#define PPUTLIMPL_UHEX_0x8B2u 2226u, 0x8B2, 0x8B2, 1, 0x74Du, 8, B, 2
#define PPUTLIMPL_UHEX_0x8B1u 2225u, 0x8B1, 0x8B1, 1, 0x74Eu, 8, B, 1
#define PPUTLIMPL_UHEX_0x8B0u 2224u, 0x8B0, 0x8B0, 1, 0x74Fu, 8, B, 0
#define PPUTLIMPL_UHEX_0x8AFu 2223u, 0x8AF, 0x8AF, 1, 0x750u, 8, A, F
#define PPUTLIMPL_UHEX_0x8AEu 2222u, 0x8AE, 0x8AE, 1, 0x751u, 8, A, E
#define PPUTLIMPL_UHEX_0x8ADu 2221u, 0x8AD, 0x8AD, 1, 0x752u, 8, A, D
#define PPUTLIMPL_UHEX_0x8ACu 2220u, 0x8AC, 0x8AC, 1, 0x753u, 8, A, C
#define PPUTLIMPL_UHEX_0x8ABu 2219u, 0x8AB, 0x8AB, 1, 0x754u, 8, A, B
#define PPUTLIMPL_UHEX_0x8AAu 2218u, 0x8AA, 0x8AA, 1, 0x755u, 8, A, A
#define PPUTLIMPL_UHEX_0x8A9u 2217u, 0x8A9, 0x8A9, 1, 0x756u, 8, A, 9
#define PPUTLIMPL_UHEX_0x8A8u 2216u, 0x8A8, 0x8A8, 1, 0x757u, 8, A, 8
#define PPUTLIMPL_UHEX_0x8A7u 2215u, 0x8A7, 0x8A7, 1, 0x758u, 8, A, 7
#define PPUTLIMPL_UHEX_0x8A6u 2214u, 0x8A6, 0x8A6, 1, 0x759u, 8, A, 6
#define PPUTLIMPL_UHEX_0x8A5u 2213u, 0x8A5, 0x8A5, 1, 0x75Au, 8, A, 5
#define PPUTLIMPL_UHEX_0x8A4u 2212u, 0x8A4, 0x8A4, 1, 0x75Bu, 8, A, 4
#define PPUTLIMPL_UHEX_0x8A3u 2211u, 0x8A3, 0x8A3, 1, 0x75Cu, 8, A, 3
#define PPUTLIMPL_UHEX_0x8A2u 2210u, 0x8A2, 0x8A2, 1, 0x75Du, 8, A, 2
#define PPUTLIMPL_UHEX_0x8A1u 2209u, 0x8A1, 0x8A1, 1, 0x75Eu, 8, A, 1
#define PPUTLIMPL_UHEX_0x8A0u 2208u, 0x8A0, 0x8A0, 1, 0x75Fu, 8, A, 0
#define PPUTLIMPL_UHEX_0x89Fu 2207u, 0x89F, 0x89F, 1, 0x760u, 8, 9, F
#define PPUTLIMPL_UHEX_0x89Eu 2206u, 0x89E, 0x89E, 1, 0x761u, 8, 9, E
#define PPUTLIMPL_UHEX_0x89Du 2205u, 0x89D, 0x89D, 1, 0x762u, 8, 9, D
#define PPUTLIMPL_UHEX_0x89Cu 2204u, 0x89C, 0x89C, 1, 0x763u, 8, 9, C
#define PPUTLIMPL_UHEX_0x89Bu 2203u, 0x89B, 0x89B, 1, 0x764u, 8, 9, B
#define PPUTLIMPL_UHEX_0x89Au 2202u, 0x89A, 0x89A, 1, 0x765u, 8, 9, A
#define PPUTLIMPL_UHEX_0x899u 2201u, 0x899, 0x899, 1, 0x766u, 8, 9, 9
#define PPUTLIMPL_UHEX_0x898u 2200u, 0x898, 0x898, 1, 0x767u, 8, 9, 8
#define PPUTLIMPL_UHEX_0x897u 2199u, 0x897, 0x897, 1, 0x768u, 8, 9, 7
#define PPUTLIMPL_UHEX_0x896u 2198u, 0x896, 0x896, 1, 0x769u, 8, 9, 6
#define PPUTLIMPL_UHEX_0x895u 2197u, 0x895, 0x895, 1, 0x76Au, 8, 9, 5
#define PPUTLIMPL_UHEX_0x894u 2196u, 0x894, 0x894, 1, 0x76Bu, 8, 9, 4
#define PPUTLIMPL_UHEX_0x893u 2195u, 0x893, 0x893, 1, 0x76Cu, 8, 9, 3
#define PPUTLIMPL_UHEX_0x892u 2194u, 0x892, 0x892, 1, 0x76Du, 8, 9, 2
#define PPUTLIMPL_UHEX_0x891u 2193u, 0x891, 0x891, 1, 0x76Eu, 8, 9, 1
#define PPUTLIMPL_UHEX_0x890u 2192u, 0x890, 0x890, 1, 0x76Fu, 8, 9, 0
#define PPUTLIMPL_UHEX_0x88Fu 2191u, 0x88F, 0x88F, 1, 0x770u, 8, 8, F
#define PPUTLIMPL_UHEX_0x88Eu 2190u, 0x88E, 0x88E, 1, 0x771u, 8, 8, E
#define PPUTLIMPL_UHEX_0x88Du 2189u, 0x88D, 0x88D, 1, 0x772u, 8, 8, D
#define PPUTLIMPL_UHEX_0x88Cu 2188u, 0x88C, 0x88C, 1, 0x773u, 8, 8, C
#define PPUTLIMPL_UHEX_0x88Bu 2187u, 0x88B, 0x88B, 1, 0x774u, 8, 8, B
#define PPUTLIMPL_UHEX_0x88Au 2186u, 0x88A, 0x88A, 1, 0x775u, 8, 8, A
#define PPUTLIMPL_UHEX_0x889u 2185u, 0x889, 0x889, 1, 0x776u, 8, 8, 9
#define PPUTLIMPL_UHEX_0x888u 2184u, 0x888, 0x888, 1, 0x777u, 8, 8, 8
#define PPUTLIMPL_UHEX_0x887u 2183u, 0x887, 0x887, 1, 0x778u, 8, 8, 7
#define PPUTLIMPL_UHEX_0x886u 2182u, 0x886, 0x886, 1, 0x779u, 8, 8, 6
#define PPUTLIMPL_UHEX_0x885u 2181u, 0x885, 0x885, 1, 0x77Au, 8, 8, 5
#define PPUTLIMPL_UHEX_0x884u 2180u, 0x884, 0x884, 1, 0x77Bu, 8, 8, 4
#define PPUTLIMPL_UHEX_0x883u 2179u, 0x883, 0x883, 1, 0x77Cu, 8, 8, 3
#define PPUTLIMPL_UHEX_0x882u 2178u, 0x882, 0x882, 1, 0x77Du, 8, 8, 2
#define PPUTLIMPL_UHEX_0x881u 2177u, 0x881, 0x881, 1, 0x77Eu, 8, 8, 1
#define PPUTLIMPL_UHEX_0x880u 2176u, 0x880, 0x880, 1, 0x77Fu, 8, 8, 0
#define PPUTLIMPL_UHEX_0x87Fu 2175u, 0x87F, 0x87F, 1, 0x780u, 8, 7, F
#define PPUTLIMPL_UHEX_0x87Eu 2174u, 0x87E, 0x87E, 1, 0x781u, 8, 7, E
#define PPUTLIMPL_UHEX_0x87Du 2173u, 0x87D, 0x87D, 1, 0x782u, 8, 7, D
#define PPUTLIMPL_UHEX_0x87Cu 2172u, 0x87C, 0x87C, 1, 0x783u, 8, 7, C
#define PPUTLIMPL_UHEX_0x87Bu 2171u, 0x87B, 0x87B, 1, 0x784u, 8, 7, B
#define PPUTLIMPL_UHEX_0x87Au 2170u, 0x87A, 0x87A, 1, 0x785u, 8, 7, A
#define PPUTLIMPL_UHEX_0x879u 2169u, 0x879, 0x879, 1, 0x786u, 8, 7, 9
#define PPUTLIMPL_UHEX_0x878u 2168u, 0x878, 0x878, 1, 0x787u, 8, 7, 8
#define PPUTLIMPL_UHEX_0x877u 2167u, 0x877, 0x877, 1, 0x788u, 8, 7, 7
#define PPUTLIMPL_UHEX_0x876u 2166u, 0x876, 0x876, 1, 0x789u, 8, 7, 6
#define PPUTLIMPL_UHEX_0x875u 2165u, 0x875, 0x875, 1, 0x78Au, 8, 7, 5
#define PPUTLIMPL_UHEX_0x874u 2164u, 0x874, 0x874, 1, 0x78Bu, 8, 7, 4
#define PPUTLIMPL_UHEX_0x873u 2163u, 0x873, 0x873, 1, 0x78Cu, 8, 7, 3
#define PPUTLIMPL_UHEX_0x872u 2162u, 0x872, 0x872, 1, 0x78Du, 8, 7, 2
#define PPUTLIMPL_UHEX_0x871u 2161u, 0x871, 0x871, 1, 0x78Eu, 8, 7, 1
#define PPUTLIMPL_UHEX_0x870u 2160u, 0x870, 0x870, 1, 0x78Fu, 8, 7, 0
#define PPUTLIMPL_UHEX_0x86Fu 2159u, 0x86F, 0x86F, 1, 0x790u, 8, 6, F
#define PPUTLIMPL_UHEX_0x86Eu 2158u, 0x86E, 0x86E, 1, 0x791u, 8, 6, E
#define PPUTLIMPL_UHEX_0x86Du 2157u, 0x86D, 0x86D, 1, 0x792u, 8, 6, D
#define PPUTLIMPL_UHEX_0x86Cu 2156u, 0x86C, 0x86C, 1, 0x793u, 8, 6, C
#define PPUTLIMPL_UHEX_0x86Bu 2155u, 0x86B, 0x86B, 1, 0x794u, 8, 6, B
#define PPUTLIMPL_UHEX_0x86Au 2154u, 0x86A, 0x86A, 1, 0x795u, 8, 6, A
#define PPUTLIMPL_UHEX_0x869u 2153u, 0x869, 0x869, 1, 0x796u, 8, 6, 9
#define PPUTLIMPL_UHEX_0x868u 2152u, 0x868, 0x868, 1, 0x797u, 8, 6, 8
#define PPUTLIMPL_UHEX_0x867u 2151u, 0x867, 0x867, 1, 0x798u, 8, 6, 7
#define PPUTLIMPL_UHEX_0x866u 2150u, 0x866, 0x866, 1, 0x799u, 8, 6, 6
#define PPUTLIMPL_UHEX_0x865u 2149u, 0x865, 0x865, 1, 0x79Au, 8, 6, 5
#define PPUTLIMPL_UHEX_0x864u 2148u, 0x864, 0x864, 1, 0x79Bu, 8, 6, 4
#define PPUTLIMPL_UHEX_0x863u 2147u, 0x863, 0x863, 1, 0x79Cu, 8, 6, 3
#define PPUTLIMPL_UHEX_0x862u 2146u, 0x862, 0x862, 1, 0x79Du, 8, 6, 2
#define PPUTLIMPL_UHEX_0x861u 2145u, 0x861, 0x861, 1, 0x79Eu, 8, 6, 1
#define PPUTLIMPL_UHEX_0x860u 2144u, 0x860, 0x860, 1, 0x79Fu, 8, 6, 0
#define PPUTLIMPL_UHEX_0x85Fu 2143u, 0x85F, 0x85F, 1, 0x7A0u, 8, 5, F
#define PPUTLIMPL_UHEX_0x85Eu 2142u, 0x85E, 0x85E, 1, 0x7A1u, 8, 5, E
#define PPUTLIMPL_UHEX_0x85Du 2141u, 0x85D, 0x85D, 1, 0x7A2u, 8, 5, D
#define PPUTLIMPL_UHEX_0x85Cu 2140u, 0x85C, 0x85C, 1, 0x7A3u, 8, 5, C
#define PPUTLIMPL_UHEX_0x85Bu 2139u, 0x85B, 0x85B, 1, 0x7A4u, 8, 5, B
#define PPUTLIMPL_UHEX_0x85Au 2138u, 0x85A, 0x85A, 1, 0x7A5u, 8, 5, A
#define PPUTLIMPL_UHEX_0x859u 2137u, 0x859, 0x859, 1, 0x7A6u, 8, 5, 9
#define PPUTLIMPL_UHEX_0x858u 2136u, 0x858, 0x858, 1, 0x7A7u, 8, 5, 8
#define PPUTLIMPL_UHEX_0x857u 2135u, 0x857, 0x857, 1, 0x7A8u, 8, 5, 7
#define PPUTLIMPL_UHEX_0x856u 2134u, 0x856, 0x856, 1, 0x7A9u, 8, 5, 6
#define PPUTLIMPL_UHEX_0x855u 2133u, 0x855, 0x855, 1, 0x7AAu, 8, 5, 5
#define PPUTLIMPL_UHEX_0x854u 2132u, 0x854, 0x854, 1, 0x7ABu, 8, 5, 4
#define PPUTLIMPL_UHEX_0x853u 2131u, 0x853, 0x853, 1, 0x7ACu, 8, 5, 3
#define PPUTLIMPL_UHEX_0x852u 2130u, 0x852, 0x852, 1, 0x7ADu, 8, 5, 2
#define PPUTLIMPL_UHEX_0x851u 2129u, 0x851, 0x851, 1, 0x7AEu, 8, 5, 1
#define PPUTLIMPL_UHEX_0x850u 2128u, 0x850, 0x850, 1, 0x7AFu, 8, 5, 0
#define PPUTLIMPL_UHEX_0x84Fu 2127u, 0x84F, 0x84F, 1, 0x7B0u, 8, 4, F
#define PPUTLIMPL_UHEX_0x84Eu 2126u, 0x84E, 0x84E, 1, 0x7B1u, 8, 4, E
#define PPUTLIMPL_UHEX_0x84Du 2125u, 0x84D, 0x84D, 1, 0x7B2u, 8, 4, D
#define PPUTLIMPL_UHEX_0x84Cu 2124u, 0x84C, 0x84C, 1, 0x7B3u, 8, 4, C
#define PPUTLIMPL_UHEX_0x84Bu 2123u, 0x84B, 0x84B, 1, 0x7B4u, 8, 4, B
#define PPUTLIMPL_UHEX_0x84Au 2122u, 0x84A, 0x84A, 1, 0x7B5u, 8, 4, A
#define PPUTLIMPL_UHEX_0x849u 2121u, 0x849, 0x849, 1, 0x7B6u, 8, 4, 9
#define PPUTLIMPL_UHEX_0x848u 2120u, 0x848, 0x848, 1, 0x7B7u, 8, 4, 8
#define PPUTLIMPL_UHEX_0x847u 2119u, 0x847, 0x847, 1, 0x7B8u, 8, 4, 7
#define PPUTLIMPL_UHEX_0x846u 2118u, 0x846, 0x846, 1, 0x7B9u, 8, 4, 6
#define PPUTLIMPL_UHEX_0x845u 2117u, 0x845, 0x845, 1, 0x7BAu, 8, 4, 5
#define PPUTLIMPL_UHEX_0x844u 2116u, 0x844, 0x844, 1, 0x7BBu, 8, 4, 4
#define PPUTLIMPL_UHEX_0x843u 2115u, 0x843, 0x843, 1, 0x7BCu, 8, 4, 3
#define PPUTLIMPL_UHEX_0x842u 2114u, 0x842, 0x842, 1, 0x7BDu, 8, 4, 2
#define PPUTLIMPL_UHEX_0x841u 2113u, 0x841, 0x841, 1, 0x7BEu, 8, 4, 1
#define PPUTLIMPL_UHEX_0x840u 2112u, 0x840, 0x840, 1, 0x7BFu, 8, 4, 0
#define PPUTLIMPL_UHEX_0x83Fu 2111u, 0x83F, 0x83F, 1, 0x7C0u, 8, 3, F
#define PPUTLIMPL_UHEX_0x83Eu 2110u, 0x83E, 0x83E, 1, 0x7C1u, 8, 3, E
#define PPUTLIMPL_UHEX_0x83Du 2109u, 0x83D, 0x83D, 1, 0x7C2u, 8, 3, D
#define PPUTLIMPL_UHEX_0x83Cu 2108u, 0x83C, 0x83C, 1, 0x7C3u, 8, 3, C
#define PPUTLIMPL_UHEX_0x83Bu 2107u, 0x83B, 0x83B, 1, 0x7C4u, 8, 3, B
#define PPUTLIMPL_UHEX_0x83Au 2106u, 0x83A, 0x83A, 1, 0x7C5u, 8, 3, A
#define PPUTLIMPL_UHEX_0x839u 2105u, 0x839, 0x839, 1, 0x7C6u, 8, 3, 9
#define PPUTLIMPL_UHEX_0x838u 2104u, 0x838, 0x838, 1, 0x7C7u, 8, 3, 8
#define PPUTLIMPL_UHEX_0x837u 2103u, 0x837, 0x837, 1, 0x7C8u, 8, 3, 7
#define PPUTLIMPL_UHEX_0x836u 2102u, 0x836, 0x836, 1, 0x7C9u, 8, 3, 6
#define PPUTLIMPL_UHEX_0x835u 2101u, 0x835, 0x835, 1, 0x7CAu, 8, 3, 5
#define PPUTLIMPL_UHEX_0x834u 2100u, 0x834, 0x834, 1, 0x7CBu, 8, 3, 4
#define PPUTLIMPL_UHEX_0x833u 2099u, 0x833, 0x833, 1, 0x7CCu, 8, 3, 3
#define PPUTLIMPL_UHEX_0x832u 2098u, 0x832, 0x832, 1, 0x7CDu, 8, 3, 2
#define PPUTLIMPL_UHEX_0x831u 2097u, 0x831, 0x831, 1, 0x7CEu, 8, 3, 1
#define PPUTLIMPL_UHEX_0x830u 2096u, 0x830, 0x830, 1, 0x7CFu, 8, 3, 0
#define PPUTLIMPL_UHEX_0x82Fu 2095u, 0x82F, 0x82F, 1, 0x7D0u, 8, 2, F
#define PPUTLIMPL_UHEX_0x82Eu 2094u, 0x82E, 0x82E, 1, 0x7D1u, 8, 2, E
#define PPUTLIMPL_UHEX_0x82Du 2093u, 0x82D, 0x82D, 1, 0x7D2u, 8, 2, D
#define PPUTLIMPL_UHEX_0x82Cu 2092u, 0x82C, 0x82C, 1, 0x7D3u, 8, 2, C
#define PPUTLIMPL_UHEX_0x82Bu 2091u, 0x82B, 0x82B, 1, 0x7D4u, 8, 2, B
#define PPUTLIMPL_UHEX_0x82Au 2090u, 0x82A, 0x82A, 1, 0x7D5u, 8, 2, A
#define PPUTLIMPL_UHEX_0x829u 2089u, 0x829, 0x829, 1, 0x7D6u, 8, 2, 9
#define PPUTLIMPL_UHEX_0x828u 2088u, 0x828, 0x828, 1, 0x7D7u, 8, 2, 8
#define PPUTLIMPL_UHEX_0x827u 2087u, 0x827, 0x827, 1, 0x7D8u, 8, 2, 7
#define PPUTLIMPL_UHEX_0x826u 2086u, 0x826, 0x826, 1, 0x7D9u, 8, 2, 6
#define PPUTLIMPL_UHEX_0x825u 2085u, 0x825, 0x825, 1, 0x7DAu, 8, 2, 5
#define PPUTLIMPL_UHEX_0x824u 2084u, 0x824, 0x824, 1, 0x7DBu, 8, 2, 4
#define PPUTLIMPL_UHEX_0x823u 2083u, 0x823, 0x823, 1, 0x7DCu, 8, 2, 3
#define PPUTLIMPL_UHEX_0x822u 2082u, 0x822, 0x822, 1, 0x7DDu, 8, 2, 2
#define PPUTLIMPL_UHEX_0x821u 2081u, 0x821, 0x821, 1, 0x7DEu, 8, 2, 1
#define PPUTLIMPL_UHEX_0x820u 2080u, 0x820, 0x820, 1, 0x7DFu, 8, 2, 0
#define PPUTLIMPL_UHEX_0x81Fu 2079u, 0x81F, 0x81F, 1, 0x7E0u, 8, 1, F
#define PPUTLIMPL_UHEX_0x81Eu 2078u, 0x81E, 0x81E, 1, 0x7E1u, 8, 1, E
#define PPUTLIMPL_UHEX_0x81Du 2077u, 0x81D, 0x81D, 1, 0x7E2u, 8, 1, D
#define PPUTLIMPL_UHEX_0x81Cu 2076u, 0x81C, 0x81C, 1, 0x7E3u, 8, 1, C
#define PPUTLIMPL_UHEX_0x81Bu 2075u, 0x81B, 0x81B, 1, 0x7E4u, 8, 1, B
#define PPUTLIMPL_UHEX_0x81Au 2074u, 0x81A, 0x81A, 1, 0x7E5u, 8, 1, A
#define PPUTLIMPL_UHEX_0x819u 2073u, 0x819, 0x819, 1, 0x7E6u, 8, 1, 9
#define PPUTLIMPL_UHEX_0x818u 2072u, 0x818, 0x818, 1, 0x7E7u, 8, 1, 8
#define PPUTLIMPL_UHEX_0x817u 2071u, 0x817, 0x817, 1, 0x7E8u, 8, 1, 7
#define PPUTLIMPL_UHEX_0x816u 2070u, 0x816, 0x816, 1, 0x7E9u, 8, 1, 6
#define PPUTLIMPL_UHEX_0x815u 2069u, 0x815, 0x815, 1, 0x7EAu, 8, 1, 5
#define PPUTLIMPL_UHEX_0x814u 2068u, 0x814, 0x814, 1, 0x7EBu, 8, 1, 4
#define PPUTLIMPL_UHEX_0x813u 2067u, 0x813, 0x813, 1, 0x7ECu, 8, 1, 3
#define PPUTLIMPL_UHEX_0x812u 2066u, 0x812, 0x812, 1, 0x7EDu, 8, 1, 2
#define PPUTLIMPL_UHEX_0x811u 2065u, 0x811, 0x811, 1, 0x7EEu, 8, 1, 1
#define PPUTLIMPL_UHEX_0x810u 2064u, 0x810, 0x810, 1, 0x7EFu, 8, 1, 0
#define PPUTLIMPL_UHEX_0x80Fu 2063u, 0x80F, 0x80F, 1, 0x7F0u, 8, 0, F
#define PPUTLIMPL_UHEX_0x80Eu 2062u, 0x80E, 0x80E, 1, 0x7F1u, 8, 0, E
#define PPUTLIMPL_UHEX_0x80Du 2061u, 0x80D, 0x80D, 1, 0x7F2u, 8, 0, D
#define PPUTLIMPL_UHEX_0x80Cu 2060u, 0x80C, 0x80C, 1, 0x7F3u, 8, 0, C
#define PPUTLIMPL_UHEX_0x80Bu 2059u, 0x80B, 0x80B, 1, 0x7F4u, 8, 0, B
#define PPUTLIMPL_UHEX_0x80Au 2058u, 0x80A, 0x80A, 1, 0x7F5u, 8, 0, A
#define PPUTLIMPL_UHEX_0x809u 2057u, 0x809, 0x809, 1, 0x7F6u, 8, 0, 9
#define PPUTLIMPL_UHEX_0x808u 2056u, 0x808, 0x808, 1, 0x7F7u, 8, 0, 8
#define PPUTLIMPL_UHEX_0x807u 2055u, 0x807, 0x807, 1, 0x7F8u, 8, 0, 7
#define PPUTLIMPL_UHEX_0x806u 2054u, 0x806, 0x806, 1, 0x7F9u, 8, 0, 6
#define PPUTLIMPL_UHEX_0x805u 2053u, 0x805, 0x805, 1, 0x7FAu, 8, 0, 5
#define PPUTLIMPL_UHEX_0x804u 2052u, 0x804, 0x804, 1, 0x7FBu, 8, 0, 4
#define PPUTLIMPL_UHEX_0x803u 2051u, 0x803, 0x803, 1, 0x7FCu, 8, 0, 3
#define PPUTLIMPL_UHEX_0x802u 2050u, 0x802, 0x802, 1, 0x7FDu, 8, 0, 2
#define PPUTLIMPL_UHEX_0x801u 2049u, 0x801, 0x801, 1, 0x7FEu, 8, 0, 1
#define PPUTLIMPL_UHEX_0x800u 2048u, 0x800, 0x800, 1, 0x7FFu, 8, 0, 0
#define PPUTLIMPL_UHEX_0x7FFu 2047u, 0x7FF, 2047, 0, 0x800u, 7, F, F
#define PPUTLIMPL_UHEX_0x7FEu 2046u, 0x7FE, 2046, 0, 0x801u, 7, F, E
#define PPUTLIMPL_UHEX_0x7FDu 2045u, 0x7FD, 2045, 0, 0x802u, 7, F, D
#define PPUTLIMPL_UHEX_0x7FCu 2044u, 0x7FC, 2044, 0, 0x803u, 7, F, C
#define PPUTLIMPL_UHEX_0x7FBu 2043u, 0x7FB, 2043, 0, 0x804u, 7, F, B
#define PPUTLIMPL_UHEX_0x7FAu 2042u, 0x7FA, 2042, 0, 0x805u, 7, F, A
#define PPUTLIMPL_UHEX_0x7F9u 2041u, 0x7F9, 2041, 0, 0x806u, 7, F, 9
#define PPUTLIMPL_UHEX_0x7F8u 2040u, 0x7F8, 2040, 0, 0x807u, 7, F, 8
#define PPUTLIMPL_UHEX_0x7F7u 2039u, 0x7F7, 2039, 0, 0x808u, 7, F, 7
#define PPUTLIMPL_UHEX_0x7F6u 2038u, 0x7F6, 2038, 0, 0x809u, 7, F, 6
#define PPUTLIMPL_UHEX_0x7F5u 2037u, 0x7F5, 2037, 0, 0x80Au, 7, F, 5
#define PPUTLIMPL_UHEX_0x7F4u 2036u, 0x7F4, 2036, 0, 0x80Bu, 7, F, 4
#define PPUTLIMPL_UHEX_0x7F3u 2035u, 0x7F3, 2035, 0, 0x80Cu, 7, F, 3
#define PPUTLIMPL_UHEX_0x7F2u 2034u, 0x7F2, 2034, 0, 0x80Du, 7, F, 2
#define PPUTLIMPL_UHEX_0x7F1u 2033u, 0x7F1, 2033, 0, 0x80Eu, 7, F, 1
#define PPUTLIMPL_UHEX_0x7F0u 2032u, 0x7F0, 2032, 0, 0x80Fu, 7, F, 0
#define PPUTLIMPL_UHEX_0x7EFu 2031u, 0x7EF, 2031, 0, 0x810u, 7, E, F
#define PPUTLIMPL_UHEX_0x7EEu 2030u, 0x7EE, 2030, 0, 0x811u, 7, E, E
#define PPUTLIMPL_UHEX_0x7EDu 2029u, 0x7ED, 2029, 0, 0x812u, 7, E, D
#define PPUTLIMPL_UHEX_0x7ECu 2028u, 0x7EC, 2028, 0, 0x813u, 7, E, C
#define PPUTLIMPL_UHEX_0x7EBu 2027u, 0x7EB, 2027, 0, 0x814u, 7, E, B
#define PPUTLIMPL_UHEX_0x7EAu 2026u, 0x7EA, 2026, 0, 0x815u, 7, E, A
#define PPUTLIMPL_UHEX_0x7E9u 2025u, 0x7E9, 2025, 0, 0x816u, 7, E, 9
#define PPUTLIMPL_UHEX_0x7E8u 2024u, 0x7E8, 2024, 0, 0x817u, 7, E, 8
#define PPUTLIMPL_UHEX_0x7E7u 2023u, 0x7E7, 2023, 0, 0x818u, 7, E, 7
#define PPUTLIMPL_UHEX_0x7E6u 2022u, 0x7E6, 2022, 0, 0x819u, 7, E, 6
#define PPUTLIMPL_UHEX_0x7E5u 2021u, 0x7E5, 2021, 0, 0x81Au, 7, E, 5
#define PPUTLIMPL_UHEX_0x7E4u 2020u, 0x7E4, 2020, 0, 0x81Bu, 7, E, 4
#define PPUTLIMPL_UHEX_0x7E3u 2019u, 0x7E3, 2019, 0, 0x81Cu, 7, E, 3
#define PPUTLIMPL_UHEX_0x7E2u 2018u, 0x7E2, 2018, 0, 0x81Du, 7, E, 2
#define PPUTLIMPL_UHEX_0x7E1u 2017u, 0x7E1, 2017, 0, 0x81Eu, 7, E, 1
#define PPUTLIMPL_UHEX_0x7E0u 2016u, 0x7E0, 2016, 0, 0x81Fu, 7, E, 0
#define PPUTLIMPL_UHEX_0x7DFu 2015u, 0x7DF, 2015, 0, 0x820u, 7, D, F
#define PPUTLIMPL_UHEX_0x7DEu 2014u, 0x7DE, 2014, 0, 0x821u, 7, D, E
#define PPUTLIMPL_UHEX_0x7DDu 2013u, 0x7DD, 2013, 0, 0x822u, 7, D, D
#define PPUTLIMPL_UHEX_0x7DCu 2012u, 0x7DC, 2012, 0, 0x823u, 7, D, C
#define PPUTLIMPL_UHEX_0x7DBu 2011u, 0x7DB, 2011, 0, 0x824u, 7, D, B
#define PPUTLIMPL_UHEX_0x7DAu 2010u, 0x7DA, 2010, 0, 0x825u, 7, D, A
#define PPUTLIMPL_UHEX_0x7D9u 2009u, 0x7D9, 2009, 0, 0x826u, 7, D, 9
#define PPUTLIMPL_UHEX_0x7D8u 2008u, 0x7D8, 2008, 0, 0x827u, 7, D, 8
#define PPUTLIMPL_UHEX_0x7D7u 2007u, 0x7D7, 2007, 0, 0x828u, 7, D, 7
#define PPUTLIMPL_UHEX_0x7D6u 2006u, 0x7D6, 2006, 0, 0x829u, 7, D, 6
#define PPUTLIMPL_UHEX_0x7D5u 2005u, 0x7D5, 2005, 0, 0x82Au, 7, D, 5
#define PPUTLIMPL_UHEX_0x7D4u 2004u, 0x7D4, 2004, 0, 0x82Bu, 7, D, 4
#define PPUTLIMPL_UHEX_0x7D3u 2003u, 0x7D3, 2003, 0, 0x82Cu, 7, D, 3
#define PPUTLIMPL_UHEX_0x7D2u 2002u, 0x7D2, 2002, 0, 0x82Du, 7, D, 2
#define PPUTLIMPL_UHEX_0x7D1u 2001u, 0x7D1, 2001, 0, 0x82Eu, 7, D, 1
#define PPUTLIMPL_UHEX_0x7D0u 2000u, 0x7D0, 2000, 0, 0x82Fu, 7, D, 0
#define PPUTLIMPL_UHEX_0x7CFu 1999u, 0x7CF, 1999, 0, 0x830u, 7, C, F
#define PPUTLIMPL_UHEX_0x7CEu 1998u, 0x7CE, 1998, 0, 0x831u, 7, C, E
#define PPUTLIMPL_UHEX_0x7CDu 1997u, 0x7CD, 1997, 0, 0x832u, 7, C, D
#define PPUTLIMPL_UHEX_0x7CCu 1996u, 0x7CC, 1996, 0, 0x833u, 7, C, C
#define PPUTLIMPL_UHEX_0x7CBu 1995u, 0x7CB, 1995, 0, 0x834u, 7, C, B
#define PPUTLIMPL_UHEX_0x7CAu 1994u, 0x7CA, 1994, 0, 0x835u, 7, C, A
#define PPUTLIMPL_UHEX_0x7C9u 1993u, 0x7C9, 1993, 0, 0x836u, 7, C, 9
#define PPUTLIMPL_UHEX_0x7C8u 1992u, 0x7C8, 1992, 0, 0x837u, 7, C, 8
#define PPUTLIMPL_UHEX_0x7C7u 1991u, 0x7C7, 1991, 0, 0x838u, 7, C, 7
#define PPUTLIMPL_UHEX_0x7C6u 1990u, 0x7C6, 1990, 0, 0x839u, 7, C, 6
#define PPUTLIMPL_UHEX_0x7C5u 1989u, 0x7C5, 1989, 0, 0x83Au, 7, C, 5
#define PPUTLIMPL_UHEX_0x7C4u 1988u, 0x7C4, 1988, 0, 0x83Bu, 7, C, 4
#define PPUTLIMPL_UHEX_0x7C3u 1987u, 0x7C3, 1987, 0, 0x83Cu, 7, C, 3
#define PPUTLIMPL_UHEX_0x7C2u 1986u, 0x7C2, 1986, 0, 0x83Du, 7, C, 2
#define PPUTLIMPL_UHEX_0x7C1u 1985u, 0x7C1, 1985, 0, 0x83Eu, 7, C, 1
#define PPUTLIMPL_UHEX_0x7C0u 1984u, 0x7C0, 1984, 0, 0x83Fu, 7, C, 0
#define PPUTLIMPL_UHEX_0x7BFu 1983u, 0x7BF, 1983, 0, 0x840u, 7, B, F
#define PPUTLIMPL_UHEX_0x7BEu 1982u, 0x7BE, 1982, 0, 0x841u, 7, B, E
#define PPUTLIMPL_UHEX_0x7BDu 1981u, 0x7BD, 1981, 0, 0x842u, 7, B, D
#define PPUTLIMPL_UHEX_0x7BCu 1980u, 0x7BC, 1980, 0, 0x843u, 7, B, C
#define PPUTLIMPL_UHEX_0x7BBu 1979u, 0x7BB, 1979, 0, 0x844u, 7, B, B
#define PPUTLIMPL_UHEX_0x7BAu 1978u, 0x7BA, 1978, 0, 0x845u, 7, B, A
#define PPUTLIMPL_UHEX_0x7B9u 1977u, 0x7B9, 1977, 0, 0x846u, 7, B, 9
#define PPUTLIMPL_UHEX_0x7B8u 1976u, 0x7B8, 1976, 0, 0x847u, 7, B, 8
#define PPUTLIMPL_UHEX_0x7B7u 1975u, 0x7B7, 1975, 0, 0x848u, 7, B, 7
#define PPUTLIMPL_UHEX_0x7B6u 1974u, 0x7B6, 1974, 0, 0x849u, 7, B, 6
#define PPUTLIMPL_UHEX_0x7B5u 1973u, 0x7B5, 1973, 0, 0x84Au, 7, B, 5
#define PPUTLIMPL_UHEX_0x7B4u 1972u, 0x7B4, 1972, 0, 0x84Bu, 7, B, 4
#define PPUTLIMPL_UHEX_0x7B3u 1971u, 0x7B3, 1971, 0, 0x84Cu, 7, B, 3
#define PPUTLIMPL_UHEX_0x7B2u 1970u, 0x7B2, 1970, 0, 0x84Du, 7, B, 2
#define PPUTLIMPL_UHEX_0x7B1u 1969u, 0x7B1, 1969, 0, 0x84Eu, 7, B, 1
#define PPUTLIMPL_UHEX_0x7B0u 1968u, 0x7B0, 1968, 0, 0x84Fu, 7, B, 0
#define PPUTLIMPL_UHEX_0x7AFu 1967u, 0x7AF, 1967, 0, 0x850u, 7, A, F
#define PPUTLIMPL_UHEX_0x7AEu 1966u, 0x7AE, 1966, 0, 0x851u, 7, A, E
#define PPUTLIMPL_UHEX_0x7ADu 1965u, 0x7AD, 1965, 0, 0x852u, 7, A, D
#define PPUTLIMPL_UHEX_0x7ACu 1964u, 0x7AC, 1964, 0, 0x853u, 7, A, C
#define PPUTLIMPL_UHEX_0x7ABu 1963u, 0x7AB, 1963, 0, 0x854u, 7, A, B
#define PPUTLIMPL_UHEX_0x7AAu 1962u, 0x7AA, 1962, 0, 0x855u, 7, A, A
#define PPUTLIMPL_UHEX_0x7A9u 1961u, 0x7A9, 1961, 0, 0x856u, 7, A, 9
#define PPUTLIMPL_UHEX_0x7A8u 1960u, 0x7A8, 1960, 0, 0x857u, 7, A, 8
#define PPUTLIMPL_UHEX_0x7A7u 1959u, 0x7A7, 1959, 0, 0x858u, 7, A, 7
#define PPUTLIMPL_UHEX_0x7A6u 1958u, 0x7A6, 1958, 0, 0x859u, 7, A, 6
#define PPUTLIMPL_UHEX_0x7A5u 1957u, 0x7A5, 1957, 0, 0x85Au, 7, A, 5
#define PPUTLIMPL_UHEX_0x7A4u 1956u, 0x7A4, 1956, 0, 0x85Bu, 7, A, 4
#define PPUTLIMPL_UHEX_0x7A3u 1955u, 0x7A3, 1955, 0, 0x85Cu, 7, A, 3
#define PPUTLIMPL_UHEX_0x7A2u 1954u, 0x7A2, 1954, 0, 0x85Du, 7, A, 2
#define PPUTLIMPL_UHEX_0x7A1u 1953u, 0x7A1, 1953, 0, 0x85Eu, 7, A, 1
#define PPUTLIMPL_UHEX_0x7A0u 1952u, 0x7A0, 1952, 0, 0x85Fu, 7, A, 0
#define PPUTLIMPL_UHEX_0x79Fu 1951u, 0x79F, 1951, 0, 0x860u, 7, 9, F
#define PPUTLIMPL_UHEX_0x79Eu 1950u, 0x79E, 1950, 0, 0x861u, 7, 9, E
#define PPUTLIMPL_UHEX_0x79Du 1949u, 0x79D, 1949, 0, 0x862u, 7, 9, D
#define PPUTLIMPL_UHEX_0x79Cu 1948u, 0x79C, 1948, 0, 0x863u, 7, 9, C
#define PPUTLIMPL_UHEX_0x79Bu 1947u, 0x79B, 1947, 0, 0x864u, 7, 9, B
#define PPUTLIMPL_UHEX_0x79Au 1946u, 0x79A, 1946, 0, 0x865u, 7, 9, A
#define PPUTLIMPL_UHEX_0x799u 1945u, 0x799, 1945, 0, 0x866u, 7, 9, 9
#define PPUTLIMPL_UHEX_0x798u 1944u, 0x798, 1944, 0, 0x867u, 7, 9, 8
#define PPUTLIMPL_UHEX_0x797u 1943u, 0x797, 1943, 0, 0x868u, 7, 9, 7
#define PPUTLIMPL_UHEX_0x796u 1942u, 0x796, 1942, 0, 0x869u, 7, 9, 6
#define PPUTLIMPL_UHEX_0x795u 1941u, 0x795, 1941, 0, 0x86Au, 7, 9, 5
#define PPUTLIMPL_UHEX_0x794u 1940u, 0x794, 1940, 0, 0x86Bu, 7, 9, 4
#define PPUTLIMPL_UHEX_0x793u 1939u, 0x793, 1939, 0, 0x86Cu, 7, 9, 3
#define PPUTLIMPL_UHEX_0x792u 1938u, 0x792, 1938, 0, 0x86Du, 7, 9, 2
#define PPUTLIMPL_UHEX_0x791u 1937u, 0x791, 1937, 0, 0x86Eu, 7, 9, 1
#define PPUTLIMPL_UHEX_0x790u 1936u, 0x790, 1936, 0, 0x86Fu, 7, 9, 0
#define PPUTLIMPL_UHEX_0x78Fu 1935u, 0x78F, 1935, 0, 0x870u, 7, 8, F
#define PPUTLIMPL_UHEX_0x78Eu 1934u, 0x78E, 1934, 0, 0x871u, 7, 8, E
#define PPUTLIMPL_UHEX_0x78Du 1933u, 0x78D, 1933, 0, 0x872u, 7, 8, D
#define PPUTLIMPL_UHEX_0x78Cu 1932u, 0x78C, 1932, 0, 0x873u, 7, 8, C
#define PPUTLIMPL_UHEX_0x78Bu 1931u, 0x78B, 1931, 0, 0x874u, 7, 8, B
#define PPUTLIMPL_UHEX_0x78Au 1930u, 0x78A, 1930, 0, 0x875u, 7, 8, A
#define PPUTLIMPL_UHEX_0x789u 1929u, 0x789, 1929, 0, 0x876u, 7, 8, 9
#define PPUTLIMPL_UHEX_0x788u 1928u, 0x788, 1928, 0, 0x877u, 7, 8, 8
#define PPUTLIMPL_UHEX_0x787u 1927u, 0x787, 1927, 0, 0x878u, 7, 8, 7
#define PPUTLIMPL_UHEX_0x786u 1926u, 0x786, 1926, 0, 0x879u, 7, 8, 6
#define PPUTLIMPL_UHEX_0x785u 1925u, 0x785, 1925, 0, 0x87Au, 7, 8, 5
#define PPUTLIMPL_UHEX_0x784u 1924u, 0x784, 1924, 0, 0x87Bu, 7, 8, 4
#define PPUTLIMPL_UHEX_0x783u 1923u, 0x783, 1923, 0, 0x87Cu, 7, 8, 3
#define PPUTLIMPL_UHEX_0x782u 1922u, 0x782, 1922, 0, 0x87Du, 7, 8, 2
#define PPUTLIMPL_UHEX_0x781u 1921u, 0x781, 1921, 0, 0x87Eu, 7, 8, 1
#define PPUTLIMPL_UHEX_0x780u 1920u, 0x780, 1920, 0, 0x87Fu, 7, 8, 0
#define PPUTLIMPL_UHEX_0x77Fu 1919u, 0x77F, 1919, 0, 0x880u, 7, 7, F
#define PPUTLIMPL_UHEX_0x77Eu 1918u, 0x77E, 1918, 0, 0x881u, 7, 7, E
#define PPUTLIMPL_UHEX_0x77Du 1917u, 0x77D, 1917, 0, 0x882u, 7, 7, D
#define PPUTLIMPL_UHEX_0x77Cu 1916u, 0x77C, 1916, 0, 0x883u, 7, 7, C
#define PPUTLIMPL_UHEX_0x77Bu 1915u, 0x77B, 1915, 0, 0x884u, 7, 7, B
#define PPUTLIMPL_UHEX_0x77Au 1914u, 0x77A, 1914, 0, 0x885u, 7, 7, A
#define PPUTLIMPL_UHEX_0x779u 1913u, 0x779, 1913, 0, 0x886u, 7, 7, 9
#define PPUTLIMPL_UHEX_0x778u 1912u, 0x778, 1912, 0, 0x887u, 7, 7, 8
#define PPUTLIMPL_UHEX_0x777u 1911u, 0x777, 1911, 0, 0x888u, 7, 7, 7
#define PPUTLIMPL_UHEX_0x776u 1910u, 0x776, 1910, 0, 0x889u, 7, 7, 6
#define PPUTLIMPL_UHEX_0x775u 1909u, 0x775, 1909, 0, 0x88Au, 7, 7, 5
#define PPUTLIMPL_UHEX_0x774u 1908u, 0x774, 1908, 0, 0x88Bu, 7, 7, 4
#define PPUTLIMPL_UHEX_0x773u 1907u, 0x773, 1907, 0, 0x88Cu, 7, 7, 3
#define PPUTLIMPL_UHEX_0x772u 1906u, 0x772, 1906, 0, 0x88Du, 7, 7, 2
#define PPUTLIMPL_UHEX_0x771u 1905u, 0x771, 1905, 0, 0x88Eu, 7, 7, 1
#define PPUTLIMPL_UHEX_0x770u 1904u, 0x770, 1904, 0, 0x88Fu, 7, 7, 0
#define PPUTLIMPL_UHEX_0x76Fu 1903u, 0x76F, 1903, 0, 0x890u, 7, 6, F
#define PPUTLIMPL_UHEX_0x76Eu 1902u, 0x76E, 1902, 0, 0x891u, 7, 6, E
#define PPUTLIMPL_UHEX_0x76Du 1901u, 0x76D, 1901, 0, 0x892u, 7, 6, D
#define PPUTLIMPL_UHEX_0x76Cu 1900u, 0x76C, 1900, 0, 0x893u, 7, 6, C
#define PPUTLIMPL_UHEX_0x76Bu 1899u, 0x76B, 1899, 0, 0x894u, 7, 6, B
#define PPUTLIMPL_UHEX_0x76Au 1898u, 0x76A, 1898, 0, 0x895u, 7, 6, A
#define PPUTLIMPL_UHEX_0x769u 1897u, 0x769, 1897, 0, 0x896u, 7, 6, 9
#define PPUTLIMPL_UHEX_0x768u 1896u, 0x768, 1896, 0, 0x897u, 7, 6, 8
#define PPUTLIMPL_UHEX_0x767u 1895u, 0x767, 1895, 0, 0x898u, 7, 6, 7
#define PPUTLIMPL_UHEX_0x766u 1894u, 0x766, 1894, 0, 0x899u, 7, 6, 6
#define PPUTLIMPL_UHEX_0x765u 1893u, 0x765, 1893, 0, 0x89Au, 7, 6, 5
#define PPUTLIMPL_UHEX_0x764u 1892u, 0x764, 1892, 0, 0x89Bu, 7, 6, 4
#define PPUTLIMPL_UHEX_0x763u 1891u, 0x763, 1891, 0, 0x89Cu, 7, 6, 3
#define PPUTLIMPL_UHEX_0x762u 1890u, 0x762, 1890, 0, 0x89Du, 7, 6, 2
#define PPUTLIMPL_UHEX_0x761u 1889u, 0x761, 1889, 0, 0x89Eu, 7, 6, 1
#define PPUTLIMPL_UHEX_0x760u 1888u, 0x760, 1888, 0, 0x89Fu, 7, 6, 0
#define PPUTLIMPL_UHEX_0x75Fu 1887u, 0x75F, 1887, 0, 0x8A0u, 7, 5, F
#define PPUTLIMPL_UHEX_0x75Eu 1886u, 0x75E, 1886, 0, 0x8A1u, 7, 5, E
#define PPUTLIMPL_UHEX_0x75Du 1885u, 0x75D, 1885, 0, 0x8A2u, 7, 5, D
#define PPUTLIMPL_UHEX_0x75Cu 1884u, 0x75C, 1884, 0, 0x8A3u, 7, 5, C
#define PPUTLIMPL_UHEX_0x75Bu 1883u, 0x75B, 1883, 0, 0x8A4u, 7, 5, B
#define PPUTLIMPL_UHEX_0x75Au 1882u, 0x75A, 1882, 0, 0x8A5u, 7, 5, A
#define PPUTLIMPL_UHEX_0x759u 1881u, 0x759, 1881, 0, 0x8A6u, 7, 5, 9
#define PPUTLIMPL_UHEX_0x758u 1880u, 0x758, 1880, 0, 0x8A7u, 7, 5, 8
#define PPUTLIMPL_UHEX_0x757u 1879u, 0x757, 1879, 0, 0x8A8u, 7, 5, 7
#define PPUTLIMPL_UHEX_0x756u 1878u, 0x756, 1878, 0, 0x8A9u, 7, 5, 6
#define PPUTLIMPL_UHEX_0x755u 1877u, 0x755, 1877, 0, 0x8AAu, 7, 5, 5
#define PPUTLIMPL_UHEX_0x754u 1876u, 0x754, 1876, 0, 0x8ABu, 7, 5, 4
#define PPUTLIMPL_UHEX_0x753u 1875u, 0x753, 1875, 0, 0x8ACu, 7, 5, 3
#define PPUTLIMPL_UHEX_0x752u 1874u, 0x752, 1874, 0, 0x8ADu, 7, 5, 2
#define PPUTLIMPL_UHEX_0x751u 1873u, 0x751, 1873, 0, 0x8AEu, 7, 5, 1
#define PPUTLIMPL_UHEX_0x750u 1872u, 0x750, 1872, 0, 0x8AFu, 7, 5, 0
#define PPUTLIMPL_UHEX_0x74Fu 1871u, 0x74F, 1871, 0, 0x8B0u, 7, 4, F
#define PPUTLIMPL_UHEX_0x74Eu 1870u, 0x74E, 1870, 0, 0x8B1u, 7, 4, E
#define PPUTLIMPL_UHEX_0x74Du 1869u, 0x74D, 1869, 0, 0x8B2u, 7, 4, D
#define PPUTLIMPL_UHEX_0x74Cu 1868u, 0x74C, 1868, 0, 0x8B3u, 7, 4, C
#define PPUTLIMPL_UHEX_0x74Bu 1867u, 0x74B, 1867, 0, 0x8B4u, 7, 4, B
#define PPUTLIMPL_UHEX_0x74Au 1866u, 0x74A, 1866, 0, 0x8B5u, 7, 4, A
#define PPUTLIMPL_UHEX_0x749u 1865u, 0x749, 1865, 0, 0x8B6u, 7, 4, 9
#define PPUTLIMPL_UHEX_0x748u 1864u, 0x748, 1864, 0, 0x8B7u, 7, 4, 8
#define PPUTLIMPL_UHEX_0x747u 1863u, 0x747, 1863, 0, 0x8B8u, 7, 4, 7
#define PPUTLIMPL_UHEX_0x746u 1862u, 0x746, 1862, 0, 0x8B9u, 7, 4, 6
#define PPUTLIMPL_UHEX_0x745u 1861u, 0x745, 1861, 0, 0x8BAu, 7, 4, 5
#define PPUTLIMPL_UHEX_0x744u 1860u, 0x744, 1860, 0, 0x8BBu, 7, 4, 4
#define PPUTLIMPL_UHEX_0x743u 1859u, 0x743, 1859, 0, 0x8BCu, 7, 4, 3
#define PPUTLIMPL_UHEX_0x742u 1858u, 0x742, 1858, 0, 0x8BDu, 7, 4, 2
#define PPUTLIMPL_UHEX_0x741u 1857u, 0x741, 1857, 0, 0x8BEu, 7, 4, 1
#define PPUTLIMPL_UHEX_0x740u 1856u, 0x740, 1856, 0, 0x8BFu, 7, 4, 0
#define PPUTLIMPL_UHEX_0x73Fu 1855u, 0x73F, 1855, 0, 0x8C0u, 7, 3, F
#define PPUTLIMPL_UHEX_0x73Eu 1854u, 0x73E, 1854, 0, 0x8C1u, 7, 3, E
#define PPUTLIMPL_UHEX_0x73Du 1853u, 0x73D, 1853, 0, 0x8C2u, 7, 3, D
#define PPUTLIMPL_UHEX_0x73Cu 1852u, 0x73C, 1852, 0, 0x8C3u, 7, 3, C
#define PPUTLIMPL_UHEX_0x73Bu 1851u, 0x73B, 1851, 0, 0x8C4u, 7, 3, B
#define PPUTLIMPL_UHEX_0x73Au 1850u, 0x73A, 1850, 0, 0x8C5u, 7, 3, A
#define PPUTLIMPL_UHEX_0x739u 1849u, 0x739, 1849, 0, 0x8C6u, 7, 3, 9
#define PPUTLIMPL_UHEX_0x738u 1848u, 0x738, 1848, 0, 0x8C7u, 7, 3, 8
#define PPUTLIMPL_UHEX_0x737u 1847u, 0x737, 1847, 0, 0x8C8u, 7, 3, 7
#define PPUTLIMPL_UHEX_0x736u 1846u, 0x736, 1846, 0, 0x8C9u, 7, 3, 6
#define PPUTLIMPL_UHEX_0x735u 1845u, 0x735, 1845, 0, 0x8CAu, 7, 3, 5
#define PPUTLIMPL_UHEX_0x734u 1844u, 0x734, 1844, 0, 0x8CBu, 7, 3, 4
#define PPUTLIMPL_UHEX_0x733u 1843u, 0x733, 1843, 0, 0x8CCu, 7, 3, 3
#define PPUTLIMPL_UHEX_0x732u 1842u, 0x732, 1842, 0, 0x8CDu, 7, 3, 2
#define PPUTLIMPL_UHEX_0x731u 1841u, 0x731, 1841, 0, 0x8CEu, 7, 3, 1
#define PPUTLIMPL_UHEX_0x730u 1840u, 0x730, 1840, 0, 0x8CFu, 7, 3, 0
#define PPUTLIMPL_UHEX_0x72Fu 1839u, 0x72F, 1839, 0, 0x8D0u, 7, 2, F
#define PPUTLIMPL_UHEX_0x72Eu 1838u, 0x72E, 1838, 0, 0x8D1u, 7, 2, E
#define PPUTLIMPL_UHEX_0x72Du 1837u, 0x72D, 1837, 0, 0x8D2u, 7, 2, D
#define PPUTLIMPL_UHEX_0x72Cu 1836u, 0x72C, 1836, 0, 0x8D3u, 7, 2, C
#define PPUTLIMPL_UHEX_0x72Bu 1835u, 0x72B, 1835, 0, 0x8D4u, 7, 2, B
#define PPUTLIMPL_UHEX_0x72Au 1834u, 0x72A, 1834, 0, 0x8D5u, 7, 2, A
#define PPUTLIMPL_UHEX_0x729u 1833u, 0x729, 1833, 0, 0x8D6u, 7, 2, 9
#define PPUTLIMPL_UHEX_0x728u 1832u, 0x728, 1832, 0, 0x8D7u, 7, 2, 8
#define PPUTLIMPL_UHEX_0x727u 1831u, 0x727, 1831, 0, 0x8D8u, 7, 2, 7
#define PPUTLIMPL_UHEX_0x726u 1830u, 0x726, 1830, 0, 0x8D9u, 7, 2, 6
#define PPUTLIMPL_UHEX_0x725u 1829u, 0x725, 1829, 0, 0x8DAu, 7, 2, 5
#define PPUTLIMPL_UHEX_0x724u 1828u, 0x724, 1828, 0, 0x8DBu, 7, 2, 4
#define PPUTLIMPL_UHEX_0x723u 1827u, 0x723, 1827, 0, 0x8DCu, 7, 2, 3
#define PPUTLIMPL_UHEX_0x722u 1826u, 0x722, 1826, 0, 0x8DDu, 7, 2, 2
#define PPUTLIMPL_UHEX_0x721u 1825u, 0x721, 1825, 0, 0x8DEu, 7, 2, 1
#define PPUTLIMPL_UHEX_0x720u 1824u, 0x720, 1824, 0, 0x8DFu, 7, 2, 0
#define PPUTLIMPL_UHEX_0x71Fu 1823u, 0x71F, 1823, 0, 0x8E0u, 7, 1, F
#define PPUTLIMPL_UHEX_0x71Eu 1822u, 0x71E, 1822, 0, 0x8E1u, 7, 1, E
#define PPUTLIMPL_UHEX_0x71Du 1821u, 0x71D, 1821, 0, 0x8E2u, 7, 1, D
#define PPUTLIMPL_UHEX_0x71Cu 1820u, 0x71C, 1820, 0, 0x8E3u, 7, 1, C
#define PPUTLIMPL_UHEX_0x71Bu 1819u, 0x71B, 1819, 0, 0x8E4u, 7, 1, B
#define PPUTLIMPL_UHEX_0x71Au 1818u, 0x71A, 1818, 0, 0x8E5u, 7, 1, A
#define PPUTLIMPL_UHEX_0x719u 1817u, 0x719, 1817, 0, 0x8E6u, 7, 1, 9
#define PPUTLIMPL_UHEX_0x718u 1816u, 0x718, 1816, 0, 0x8E7u, 7, 1, 8
#define PPUTLIMPL_UHEX_0x717u 1815u, 0x717, 1815, 0, 0x8E8u, 7, 1, 7
#define PPUTLIMPL_UHEX_0x716u 1814u, 0x716, 1814, 0, 0x8E9u, 7, 1, 6
#define PPUTLIMPL_UHEX_0x715u 1813u, 0x715, 1813, 0, 0x8EAu, 7, 1, 5
#define PPUTLIMPL_UHEX_0x714u 1812u, 0x714, 1812, 0, 0x8EBu, 7, 1, 4
#define PPUTLIMPL_UHEX_0x713u 1811u, 0x713, 1811, 0, 0x8ECu, 7, 1, 3
#define PPUTLIMPL_UHEX_0x712u 1810u, 0x712, 1810, 0, 0x8EDu, 7, 1, 2
#define PPUTLIMPL_UHEX_0x711u 1809u, 0x711, 1809, 0, 0x8EEu, 7, 1, 1
#define PPUTLIMPL_UHEX_0x710u 1808u, 0x710, 1808, 0, 0x8EFu, 7, 1, 0
#define PPUTLIMPL_UHEX_0x70Fu 1807u, 0x70F, 1807, 0, 0x8F0u, 7, 0, F
#define PPUTLIMPL_UHEX_0x70Eu 1806u, 0x70E, 1806, 0, 0x8F1u, 7, 0, E
#define PPUTLIMPL_UHEX_0x70Du 1805u, 0x70D, 1805, 0, 0x8F2u, 7, 0, D
#define PPUTLIMPL_UHEX_0x70Cu 1804u, 0x70C, 1804, 0, 0x8F3u, 7, 0, C
#define PPUTLIMPL_UHEX_0x70Bu 1803u, 0x70B, 1803, 0, 0x8F4u, 7, 0, B
#define PPUTLIMPL_UHEX_0x70Au 1802u, 0x70A, 1802, 0, 0x8F5u, 7, 0, A
#define PPUTLIMPL_UHEX_0x709u 1801u, 0x709, 1801, 0, 0x8F6u, 7, 0, 9
#define PPUTLIMPL_UHEX_0x708u 1800u, 0x708, 1800, 0, 0x8F7u, 7, 0, 8
#define PPUTLIMPL_UHEX_0x707u 1799u, 0x707, 1799, 0, 0x8F8u, 7, 0, 7
#define PPUTLIMPL_UHEX_0x706u 1798u, 0x706, 1798, 0, 0x8F9u, 7, 0, 6
#define PPUTLIMPL_UHEX_0x705u 1797u, 0x705, 1797, 0, 0x8FAu, 7, 0, 5
#define PPUTLIMPL_UHEX_0x704u 1796u, 0x704, 1796, 0, 0x8FBu, 7, 0, 4
#define PPUTLIMPL_UHEX_0x703u 1795u, 0x703, 1795, 0, 0x8FCu, 7, 0, 3
#define PPUTLIMPL_UHEX_0x702u 1794u, 0x702, 1794, 0, 0x8FDu, 7, 0, 2
#define PPUTLIMPL_UHEX_0x701u 1793u, 0x701, 1793, 0, 0x8FEu, 7, 0, 1
#define PPUTLIMPL_UHEX_0x700u 1792u, 0x700, 1792, 0, 0x8FFu, 7, 0, 0
#define PPUTLIMPL_UHEX_0x6FFu 1791u, 0x6FF, 1791, 0, 0x900u, 6, F, F
#define PPUTLIMPL_UHEX_0x6FEu 1790u, 0x6FE, 1790, 0, 0x901u, 6, F, E
#define PPUTLIMPL_UHEX_0x6FDu 1789u, 0x6FD, 1789, 0, 0x902u, 6, F, D
#define PPUTLIMPL_UHEX_0x6FCu 1788u, 0x6FC, 1788, 0, 0x903u, 6, F, C
#define PPUTLIMPL_UHEX_0x6FBu 1787u, 0x6FB, 1787, 0, 0x904u, 6, F, B
#define PPUTLIMPL_UHEX_0x6FAu 1786u, 0x6FA, 1786, 0, 0x905u, 6, F, A
#define PPUTLIMPL_UHEX_0x6F9u 1785u, 0x6F9, 1785, 0, 0x906u, 6, F, 9
#define PPUTLIMPL_UHEX_0x6F8u 1784u, 0x6F8, 1784, 0, 0x907u, 6, F, 8
#define PPUTLIMPL_UHEX_0x6F7u 1783u, 0x6F7, 1783, 0, 0x908u, 6, F, 7
#define PPUTLIMPL_UHEX_0x6F6u 1782u, 0x6F6, 1782, 0, 0x909u, 6, F, 6
#define PPUTLIMPL_UHEX_0x6F5u 1781u, 0x6F5, 1781, 0, 0x90Au, 6, F, 5
#define PPUTLIMPL_UHEX_0x6F4u 1780u, 0x6F4, 1780, 0, 0x90Bu, 6, F, 4
#define PPUTLIMPL_UHEX_0x6F3u 1779u, 0x6F3, 1779, 0, 0x90Cu, 6, F, 3
#define PPUTLIMPL_UHEX_0x6F2u 1778u, 0x6F2, 1778, 0, 0x90Du, 6, F, 2
#define PPUTLIMPL_UHEX_0x6F1u 1777u, 0x6F1, 1777, 0, 0x90Eu, 6, F, 1
#define PPUTLIMPL_UHEX_0x6F0u 1776u, 0x6F0, 1776, 0, 0x90Fu, 6, F, 0
#define PPUTLIMPL_UHEX_0x6EFu 1775u, 0x6EF, 1775, 0, 0x910u, 6, E, F
#define PPUTLIMPL_UHEX_0x6EEu 1774u, 0x6EE, 1774, 0, 0x911u, 6, E, E
#define PPUTLIMPL_UHEX_0x6EDu 1773u, 0x6ED, 1773, 0, 0x912u, 6, E, D
#define PPUTLIMPL_UHEX_0x6ECu 1772u, 0x6EC, 1772, 0, 0x913u, 6, E, C
#define PPUTLIMPL_UHEX_0x6EBu 1771u, 0x6EB, 1771, 0, 0x914u, 6, E, B
#define PPUTLIMPL_UHEX_0x6EAu 1770u, 0x6EA, 1770, 0, 0x915u, 6, E, A
#define PPUTLIMPL_UHEX_0x6E9u 1769u, 0x6E9, 1769, 0, 0x916u, 6, E, 9
#define PPUTLIMPL_UHEX_0x6E8u 1768u, 0x6E8, 1768, 0, 0x917u, 6, E, 8
#define PPUTLIMPL_UHEX_0x6E7u 1767u, 0x6E7, 1767, 0, 0x918u, 6, E, 7
#define PPUTLIMPL_UHEX_0x6E6u 1766u, 0x6E6, 1766, 0, 0x919u, 6, E, 6
#define PPUTLIMPL_UHEX_0x6E5u 1765u, 0x6E5, 1765, 0, 0x91Au, 6, E, 5
#define PPUTLIMPL_UHEX_0x6E4u 1764u, 0x6E4, 1764, 0, 0x91Bu, 6, E, 4
#define PPUTLIMPL_UHEX_0x6E3u 1763u, 0x6E3, 1763, 0, 0x91Cu, 6, E, 3
#define PPUTLIMPL_UHEX_0x6E2u 1762u, 0x6E2, 1762, 0, 0x91Du, 6, E, 2
#define PPUTLIMPL_UHEX_0x6E1u 1761u, 0x6E1, 1761, 0, 0x91Eu, 6, E, 1
#define PPUTLIMPL_UHEX_0x6E0u 1760u, 0x6E0, 1760, 0, 0x91Fu, 6, E, 0
#define PPUTLIMPL_UHEX_0x6DFu 1759u, 0x6DF, 1759, 0, 0x920u, 6, D, F
#define PPUTLIMPL_UHEX_0x6DEu 1758u, 0x6DE, 1758, 0, 0x921u, 6, D, E
#define PPUTLIMPL_UHEX_0x6DDu 1757u, 0x6DD, 1757, 0, 0x922u, 6, D, D
#define PPUTLIMPL_UHEX_0x6DCu 1756u, 0x6DC, 1756, 0, 0x923u, 6, D, C
#define PPUTLIMPL_UHEX_0x6DBu 1755u, 0x6DB, 1755, 0, 0x924u, 6, D, B
#define PPUTLIMPL_UHEX_0x6DAu 1754u, 0x6DA, 1754, 0, 0x925u, 6, D, A
#define PPUTLIMPL_UHEX_0x6D9u 1753u, 0x6D9, 1753, 0, 0x926u, 6, D, 9
#define PPUTLIMPL_UHEX_0x6D8u 1752u, 0x6D8, 1752, 0, 0x927u, 6, D, 8
#define PPUTLIMPL_UHEX_0x6D7u 1751u, 0x6D7, 1751, 0, 0x928u, 6, D, 7
#define PPUTLIMPL_UHEX_0x6D6u 1750u, 0x6D6, 1750, 0, 0x929u, 6, D, 6
#define PPUTLIMPL_UHEX_0x6D5u 1749u, 0x6D5, 1749, 0, 0x92Au, 6, D, 5
#define PPUTLIMPL_UHEX_0x6D4u 1748u, 0x6D4, 1748, 0, 0x92Bu, 6, D, 4
#define PPUTLIMPL_UHEX_0x6D3u 1747u, 0x6D3, 1747, 0, 0x92Cu, 6, D, 3
#define PPUTLIMPL_UHEX_0x6D2u 1746u, 0x6D2, 1746, 0, 0x92Du, 6, D, 2
#define PPUTLIMPL_UHEX_0x6D1u 1745u, 0x6D1, 1745, 0, 0x92Eu, 6, D, 1
#define PPUTLIMPL_UHEX_0x6D0u 1744u, 0x6D0, 1744, 0, 0x92Fu, 6, D, 0
#define PPUTLIMPL_UHEX_0x6CFu 1743u, 0x6CF, 1743, 0, 0x930u, 6, C, F
#define PPUTLIMPL_UHEX_0x6CEu 1742u, 0x6CE, 1742, 0, 0x931u, 6, C, E
#define PPUTLIMPL_UHEX_0x6CDu 1741u, 0x6CD, 1741, 0, 0x932u, 6, C, D
#define PPUTLIMPL_UHEX_0x6CCu 1740u, 0x6CC, 1740, 0, 0x933u, 6, C, C
#define PPUTLIMPL_UHEX_0x6CBu 1739u, 0x6CB, 1739, 0, 0x934u, 6, C, B
#define PPUTLIMPL_UHEX_0x6CAu 1738u, 0x6CA, 1738, 0, 0x935u, 6, C, A
#define PPUTLIMPL_UHEX_0x6C9u 1737u, 0x6C9, 1737, 0, 0x936u, 6, C, 9
#define PPUTLIMPL_UHEX_0x6C8u 1736u, 0x6C8, 1736, 0, 0x937u, 6, C, 8
#define PPUTLIMPL_UHEX_0x6C7u 1735u, 0x6C7, 1735, 0, 0x938u, 6, C, 7
#define PPUTLIMPL_UHEX_0x6C6u 1734u, 0x6C6, 1734, 0, 0x939u, 6, C, 6
#define PPUTLIMPL_UHEX_0x6C5u 1733u, 0x6C5, 1733, 0, 0x93Au, 6, C, 5
#define PPUTLIMPL_UHEX_0x6C4u 1732u, 0x6C4, 1732, 0, 0x93Bu, 6, C, 4
#define PPUTLIMPL_UHEX_0x6C3u 1731u, 0x6C3, 1731, 0, 0x93Cu, 6, C, 3
#define PPUTLIMPL_UHEX_0x6C2u 1730u, 0x6C2, 1730, 0, 0x93Du, 6, C, 2
#define PPUTLIMPL_UHEX_0x6C1u 1729u, 0x6C1, 1729, 0, 0x93Eu, 6, C, 1
#define PPUTLIMPL_UHEX_0x6C0u 1728u, 0x6C0, 1728, 0, 0x93Fu, 6, C, 0
#define PPUTLIMPL_UHEX_0x6BFu 1727u, 0x6BF, 1727, 0, 0x940u, 6, B, F
#define PPUTLIMPL_UHEX_0x6BEu 1726u, 0x6BE, 1726, 0, 0x941u, 6, B, E
#define PPUTLIMPL_UHEX_0x6BDu 1725u, 0x6BD, 1725, 0, 0x942u, 6, B, D
#define PPUTLIMPL_UHEX_0x6BCu 1724u, 0x6BC, 1724, 0, 0x943u, 6, B, C
#define PPUTLIMPL_UHEX_0x6BBu 1723u, 0x6BB, 1723, 0, 0x944u, 6, B, B
#define PPUTLIMPL_UHEX_0x6BAu 1722u, 0x6BA, 1722, 0, 0x945u, 6, B, A
#define PPUTLIMPL_UHEX_0x6B9u 1721u, 0x6B9, 1721, 0, 0x946u, 6, B, 9
#define PPUTLIMPL_UHEX_0x6B8u 1720u, 0x6B8, 1720, 0, 0x947u, 6, B, 8
#define PPUTLIMPL_UHEX_0x6B7u 1719u, 0x6B7, 1719, 0, 0x948u, 6, B, 7
#define PPUTLIMPL_UHEX_0x6B6u 1718u, 0x6B6, 1718, 0, 0x949u, 6, B, 6
#define PPUTLIMPL_UHEX_0x6B5u 1717u, 0x6B5, 1717, 0, 0x94Au, 6, B, 5
#define PPUTLIMPL_UHEX_0x6B4u 1716u, 0x6B4, 1716, 0, 0x94Bu, 6, B, 4
#define PPUTLIMPL_UHEX_0x6B3u 1715u, 0x6B3, 1715, 0, 0x94Cu, 6, B, 3
#define PPUTLIMPL_UHEX_0x6B2u 1714u, 0x6B2, 1714, 0, 0x94Du, 6, B, 2
#define PPUTLIMPL_UHEX_0x6B1u 1713u, 0x6B1, 1713, 0, 0x94Eu, 6, B, 1
#define PPUTLIMPL_UHEX_0x6B0u 1712u, 0x6B0, 1712, 0, 0x94Fu, 6, B, 0
#define PPUTLIMPL_UHEX_0x6AFu 1711u, 0x6AF, 1711, 0, 0x950u, 6, A, F
#define PPUTLIMPL_UHEX_0x6AEu 1710u, 0x6AE, 1710, 0, 0x951u, 6, A, E
#define PPUTLIMPL_UHEX_0x6ADu 1709u, 0x6AD, 1709, 0, 0x952u, 6, A, D
#define PPUTLIMPL_UHEX_0x6ACu 1708u, 0x6AC, 1708, 0, 0x953u, 6, A, C
#define PPUTLIMPL_UHEX_0x6ABu 1707u, 0x6AB, 1707, 0, 0x954u, 6, A, B
#define PPUTLIMPL_UHEX_0x6AAu 1706u, 0x6AA, 1706, 0, 0x955u, 6, A, A
#define PPUTLIMPL_UHEX_0x6A9u 1705u, 0x6A9, 1705, 0, 0x956u, 6, A, 9
#define PPUTLIMPL_UHEX_0x6A8u 1704u, 0x6A8, 1704, 0, 0x957u, 6, A, 8
#define PPUTLIMPL_UHEX_0x6A7u 1703u, 0x6A7, 1703, 0, 0x958u, 6, A, 7
#define PPUTLIMPL_UHEX_0x6A6u 1702u, 0x6A6, 1702, 0, 0x959u, 6, A, 6
#define PPUTLIMPL_UHEX_0x6A5u 1701u, 0x6A5, 1701, 0, 0x95Au, 6, A, 5
#define PPUTLIMPL_UHEX_0x6A4u 1700u, 0x6A4, 1700, 0, 0x95Bu, 6, A, 4
#define PPUTLIMPL_UHEX_0x6A3u 1699u, 0x6A3, 1699, 0, 0x95Cu, 6, A, 3
#define PPUTLIMPL_UHEX_0x6A2u 1698u, 0x6A2, 1698, 0, 0x95Du, 6, A, 2
#define PPUTLIMPL_UHEX_0x6A1u 1697u, 0x6A1, 1697, 0, 0x95Eu, 6, A, 1
#define PPUTLIMPL_UHEX_0x6A0u 1696u, 0x6A0, 1696, 0, 0x95Fu, 6, A, 0
#define PPUTLIMPL_UHEX_0x69Fu 1695u, 0x69F, 1695, 0, 0x960u, 6, 9, F
#define PPUTLIMPL_UHEX_0x69Eu 1694u, 0x69E, 1694, 0, 0x961u, 6, 9, E
#define PPUTLIMPL_UHEX_0x69Du 1693u, 0x69D, 1693, 0, 0x962u, 6, 9, D
#define PPUTLIMPL_UHEX_0x69Cu 1692u, 0x69C, 1692, 0, 0x963u, 6, 9, C
#define PPUTLIMPL_UHEX_0x69Bu 1691u, 0x69B, 1691, 0, 0x964u, 6, 9, B
#define PPUTLIMPL_UHEX_0x69Au 1690u, 0x69A, 1690, 0, 0x965u, 6, 9, A
#define PPUTLIMPL_UHEX_0x699u 1689u, 0x699, 1689, 0, 0x966u, 6, 9, 9
#define PPUTLIMPL_UHEX_0x698u 1688u, 0x698, 1688, 0, 0x967u, 6, 9, 8
#define PPUTLIMPL_UHEX_0x697u 1687u, 0x697, 1687, 0, 0x968u, 6, 9, 7
#define PPUTLIMPL_UHEX_0x696u 1686u, 0x696, 1686, 0, 0x969u, 6, 9, 6
#define PPUTLIMPL_UHEX_0x695u 1685u, 0x695, 1685, 0, 0x96Au, 6, 9, 5
#define PPUTLIMPL_UHEX_0x694u 1684u, 0x694, 1684, 0, 0x96Bu, 6, 9, 4
#define PPUTLIMPL_UHEX_0x693u 1683u, 0x693, 1683, 0, 0x96Cu, 6, 9, 3
#define PPUTLIMPL_UHEX_0x692u 1682u, 0x692, 1682, 0, 0x96Du, 6, 9, 2
#define PPUTLIMPL_UHEX_0x691u 1681u, 0x691, 1681, 0, 0x96Eu, 6, 9, 1
#define PPUTLIMPL_UHEX_0x690u 1680u, 0x690, 1680, 0, 0x96Fu, 6, 9, 0
#define PPUTLIMPL_UHEX_0x68Fu 1679u, 0x68F, 1679, 0, 0x970u, 6, 8, F
#define PPUTLIMPL_UHEX_0x68Eu 1678u, 0x68E, 1678, 0, 0x971u, 6, 8, E
#define PPUTLIMPL_UHEX_0x68Du 1677u, 0x68D, 1677, 0, 0x972u, 6, 8, D
#define PPUTLIMPL_UHEX_0x68Cu 1676u, 0x68C, 1676, 0, 0x973u, 6, 8, C
#define PPUTLIMPL_UHEX_0x68Bu 1675u, 0x68B, 1675, 0, 0x974u, 6, 8, B
#define PPUTLIMPL_UHEX_0x68Au 1674u, 0x68A, 1674, 0, 0x975u, 6, 8, A
#define PPUTLIMPL_UHEX_0x689u 1673u, 0x689, 1673, 0, 0x976u, 6, 8, 9
#define PPUTLIMPL_UHEX_0x688u 1672u, 0x688, 1672, 0, 0x977u, 6, 8, 8
#define PPUTLIMPL_UHEX_0x687u 1671u, 0x687, 1671, 0, 0x978u, 6, 8, 7
#define PPUTLIMPL_UHEX_0x686u 1670u, 0x686, 1670, 0, 0x979u, 6, 8, 6
#define PPUTLIMPL_UHEX_0x685u 1669u, 0x685, 1669, 0, 0x97Au, 6, 8, 5
#define PPUTLIMPL_UHEX_0x684u 1668u, 0x684, 1668, 0, 0x97Bu, 6, 8, 4
#define PPUTLIMPL_UHEX_0x683u 1667u, 0x683, 1667, 0, 0x97Cu, 6, 8, 3
#define PPUTLIMPL_UHEX_0x682u 1666u, 0x682, 1666, 0, 0x97Du, 6, 8, 2
#define PPUTLIMPL_UHEX_0x681u 1665u, 0x681, 1665, 0, 0x97Eu, 6, 8, 1
#define PPUTLIMPL_UHEX_0x680u 1664u, 0x680, 1664, 0, 0x97Fu, 6, 8, 0
#define PPUTLIMPL_UHEX_0x67Fu 1663u, 0x67F, 1663, 0, 0x980u, 6, 7, F
#define PPUTLIMPL_UHEX_0x67Eu 1662u, 0x67E, 1662, 0, 0x981u, 6, 7, E
#define PPUTLIMPL_UHEX_0x67Du 1661u, 0x67D, 1661, 0, 0x982u, 6, 7, D
#define PPUTLIMPL_UHEX_0x67Cu 1660u, 0x67C, 1660, 0, 0x983u, 6, 7, C
#define PPUTLIMPL_UHEX_0x67Bu 1659u, 0x67B, 1659, 0, 0x984u, 6, 7, B
#define PPUTLIMPL_UHEX_0x67Au 1658u, 0x67A, 1658, 0, 0x985u, 6, 7, A
#define PPUTLIMPL_UHEX_0x679u 1657u, 0x679, 1657, 0, 0x986u, 6, 7, 9
#define PPUTLIMPL_UHEX_0x678u 1656u, 0x678, 1656, 0, 0x987u, 6, 7, 8
#define PPUTLIMPL_UHEX_0x677u 1655u, 0x677, 1655, 0, 0x988u, 6, 7, 7
#define PPUTLIMPL_UHEX_0x676u 1654u, 0x676, 1654, 0, 0x989u, 6, 7, 6
#define PPUTLIMPL_UHEX_0x675u 1653u, 0x675, 1653, 0, 0x98Au, 6, 7, 5
#define PPUTLIMPL_UHEX_0x674u 1652u, 0x674, 1652, 0, 0x98Bu, 6, 7, 4
#define PPUTLIMPL_UHEX_0x673u 1651u, 0x673, 1651, 0, 0x98Cu, 6, 7, 3
#define PPUTLIMPL_UHEX_0x672u 1650u, 0x672, 1650, 0, 0x98Du, 6, 7, 2
#define PPUTLIMPL_UHEX_0x671u 1649u, 0x671, 1649, 0, 0x98Eu, 6, 7, 1
#define PPUTLIMPL_UHEX_0x670u 1648u, 0x670, 1648, 0, 0x98Fu, 6, 7, 0
#define PPUTLIMPL_UHEX_0x66Fu 1647u, 0x66F, 1647, 0, 0x990u, 6, 6, F
#define PPUTLIMPL_UHEX_0x66Eu 1646u, 0x66E, 1646, 0, 0x991u, 6, 6, E
#define PPUTLIMPL_UHEX_0x66Du 1645u, 0x66D, 1645, 0, 0x992u, 6, 6, D
#define PPUTLIMPL_UHEX_0x66Cu 1644u, 0x66C, 1644, 0, 0x993u, 6, 6, C
#define PPUTLIMPL_UHEX_0x66Bu 1643u, 0x66B, 1643, 0, 0x994u, 6, 6, B
#define PPUTLIMPL_UHEX_0x66Au 1642u, 0x66A, 1642, 0, 0x995u, 6, 6, A
#define PPUTLIMPL_UHEX_0x669u 1641u, 0x669, 1641, 0, 0x996u, 6, 6, 9
#define PPUTLIMPL_UHEX_0x668u 1640u, 0x668, 1640, 0, 0x997u, 6, 6, 8
#define PPUTLIMPL_UHEX_0x667u 1639u, 0x667, 1639, 0, 0x998u, 6, 6, 7
#define PPUTLIMPL_UHEX_0x666u 1638u, 0x666, 1638, 0, 0x999u, 6, 6, 6
#define PPUTLIMPL_UHEX_0x665u 1637u, 0x665, 1637, 0, 0x99Au, 6, 6, 5
#define PPUTLIMPL_UHEX_0x664u 1636u, 0x664, 1636, 0, 0x99Bu, 6, 6, 4
#define PPUTLIMPL_UHEX_0x663u 1635u, 0x663, 1635, 0, 0x99Cu, 6, 6, 3
#define PPUTLIMPL_UHEX_0x662u 1634u, 0x662, 1634, 0, 0x99Du, 6, 6, 2
#define PPUTLIMPL_UHEX_0x661u 1633u, 0x661, 1633, 0, 0x99Eu, 6, 6, 1
#define PPUTLIMPL_UHEX_0x660u 1632u, 0x660, 1632, 0, 0x99Fu, 6, 6, 0
#define PPUTLIMPL_UHEX_0x65Fu 1631u, 0x65F, 1631, 0, 0x9A0u, 6, 5, F
#define PPUTLIMPL_UHEX_0x65Eu 1630u, 0x65E, 1630, 0, 0x9A1u, 6, 5, E
#define PPUTLIMPL_UHEX_0x65Du 1629u, 0x65D, 1629, 0, 0x9A2u, 6, 5, D
#define PPUTLIMPL_UHEX_0x65Cu 1628u, 0x65C, 1628, 0, 0x9A3u, 6, 5, C
#define PPUTLIMPL_UHEX_0x65Bu 1627u, 0x65B, 1627, 0, 0x9A4u, 6, 5, B
#define PPUTLIMPL_UHEX_0x65Au 1626u, 0x65A, 1626, 0, 0x9A5u, 6, 5, A
#define PPUTLIMPL_UHEX_0x659u 1625u, 0x659, 1625, 0, 0x9A6u, 6, 5, 9
#define PPUTLIMPL_UHEX_0x658u 1624u, 0x658, 1624, 0, 0x9A7u, 6, 5, 8
#define PPUTLIMPL_UHEX_0x657u 1623u, 0x657, 1623, 0, 0x9A8u, 6, 5, 7
#define PPUTLIMPL_UHEX_0x656u 1622u, 0x656, 1622, 0, 0x9A9u, 6, 5, 6
#define PPUTLIMPL_UHEX_0x655u 1621u, 0x655, 1621, 0, 0x9AAu, 6, 5, 5
#define PPUTLIMPL_UHEX_0x654u 1620u, 0x654, 1620, 0, 0x9ABu, 6, 5, 4
#define PPUTLIMPL_UHEX_0x653u 1619u, 0x653, 1619, 0, 0x9ACu, 6, 5, 3
#define PPUTLIMPL_UHEX_0x652u 1618u, 0x652, 1618, 0, 0x9ADu, 6, 5, 2
#define PPUTLIMPL_UHEX_0x651u 1617u, 0x651, 1617, 0, 0x9AEu, 6, 5, 1
#define PPUTLIMPL_UHEX_0x650u 1616u, 0x650, 1616, 0, 0x9AFu, 6, 5, 0
#define PPUTLIMPL_UHEX_0x64Fu 1615u, 0x64F, 1615, 0, 0x9B0u, 6, 4, F
#define PPUTLIMPL_UHEX_0x64Eu 1614u, 0x64E, 1614, 0, 0x9B1u, 6, 4, E
#define PPUTLIMPL_UHEX_0x64Du 1613u, 0x64D, 1613, 0, 0x9B2u, 6, 4, D
#define PPUTLIMPL_UHEX_0x64Cu 1612u, 0x64C, 1612, 0, 0x9B3u, 6, 4, C
#define PPUTLIMPL_UHEX_0x64Bu 1611u, 0x64B, 1611, 0, 0x9B4u, 6, 4, B
#define PPUTLIMPL_UHEX_0x64Au 1610u, 0x64A, 1610, 0, 0x9B5u, 6, 4, A
#define PPUTLIMPL_UHEX_0x649u 1609u, 0x649, 1609, 0, 0x9B6u, 6, 4, 9
#define PPUTLIMPL_UHEX_0x648u 1608u, 0x648, 1608, 0, 0x9B7u, 6, 4, 8
#define PPUTLIMPL_UHEX_0x647u 1607u, 0x647, 1607, 0, 0x9B8u, 6, 4, 7
#define PPUTLIMPL_UHEX_0x646u 1606u, 0x646, 1606, 0, 0x9B9u, 6, 4, 6
#define PPUTLIMPL_UHEX_0x645u 1605u, 0x645, 1605, 0, 0x9BAu, 6, 4, 5
#define PPUTLIMPL_UHEX_0x644u 1604u, 0x644, 1604, 0, 0x9BBu, 6, 4, 4
#define PPUTLIMPL_UHEX_0x643u 1603u, 0x643, 1603, 0, 0x9BCu, 6, 4, 3
#define PPUTLIMPL_UHEX_0x642u 1602u, 0x642, 1602, 0, 0x9BDu, 6, 4, 2
#define PPUTLIMPL_UHEX_0x641u 1601u, 0x641, 1601, 0, 0x9BEu, 6, 4, 1
#define PPUTLIMPL_UHEX_0x640u 1600u, 0x640, 1600, 0, 0x9BFu, 6, 4, 0
#define PPUTLIMPL_UHEX_0x63Fu 1599u, 0x63F, 1599, 0, 0x9C0u, 6, 3, F
#define PPUTLIMPL_UHEX_0x63Eu 1598u, 0x63E, 1598, 0, 0x9C1u, 6, 3, E
#define PPUTLIMPL_UHEX_0x63Du 1597u, 0x63D, 1597, 0, 0x9C2u, 6, 3, D
#define PPUTLIMPL_UHEX_0x63Cu 1596u, 0x63C, 1596, 0, 0x9C3u, 6, 3, C
#define PPUTLIMPL_UHEX_0x63Bu 1595u, 0x63B, 1595, 0, 0x9C4u, 6, 3, B
#define PPUTLIMPL_UHEX_0x63Au 1594u, 0x63A, 1594, 0, 0x9C5u, 6, 3, A
#define PPUTLIMPL_UHEX_0x639u 1593u, 0x639, 1593, 0, 0x9C6u, 6, 3, 9
#define PPUTLIMPL_UHEX_0x638u 1592u, 0x638, 1592, 0, 0x9C7u, 6, 3, 8
#define PPUTLIMPL_UHEX_0x637u 1591u, 0x637, 1591, 0, 0x9C8u, 6, 3, 7
#define PPUTLIMPL_UHEX_0x636u 1590u, 0x636, 1590, 0, 0x9C9u, 6, 3, 6
#define PPUTLIMPL_UHEX_0x635u 1589u, 0x635, 1589, 0, 0x9CAu, 6, 3, 5
#define PPUTLIMPL_UHEX_0x634u 1588u, 0x634, 1588, 0, 0x9CBu, 6, 3, 4
#define PPUTLIMPL_UHEX_0x633u 1587u, 0x633, 1587, 0, 0x9CCu, 6, 3, 3
#define PPUTLIMPL_UHEX_0x632u 1586u, 0x632, 1586, 0, 0x9CDu, 6, 3, 2
#define PPUTLIMPL_UHEX_0x631u 1585u, 0x631, 1585, 0, 0x9CEu, 6, 3, 1
#define PPUTLIMPL_UHEX_0x630u 1584u, 0x630, 1584, 0, 0x9CFu, 6, 3, 0
#define PPUTLIMPL_UHEX_0x62Fu 1583u, 0x62F, 1583, 0, 0x9D0u, 6, 2, F
#define PPUTLIMPL_UHEX_0x62Eu 1582u, 0x62E, 1582, 0, 0x9D1u, 6, 2, E
#define PPUTLIMPL_UHEX_0x62Du 1581u, 0x62D, 1581, 0, 0x9D2u, 6, 2, D
#define PPUTLIMPL_UHEX_0x62Cu 1580u, 0x62C, 1580, 0, 0x9D3u, 6, 2, C
#define PPUTLIMPL_UHEX_0x62Bu 1579u, 0x62B, 1579, 0, 0x9D4u, 6, 2, B
#define PPUTLIMPL_UHEX_0x62Au 1578u, 0x62A, 1578, 0, 0x9D5u, 6, 2, A
#define PPUTLIMPL_UHEX_0x629u 1577u, 0x629, 1577, 0, 0x9D6u, 6, 2, 9
#define PPUTLIMPL_UHEX_0x628u 1576u, 0x628, 1576, 0, 0x9D7u, 6, 2, 8
#define PPUTLIMPL_UHEX_0x627u 1575u, 0x627, 1575, 0, 0x9D8u, 6, 2, 7
#define PPUTLIMPL_UHEX_0x626u 1574u, 0x626, 1574, 0, 0x9D9u, 6, 2, 6
#define PPUTLIMPL_UHEX_0x625u 1573u, 0x625, 1573, 0, 0x9DAu, 6, 2, 5
#define PPUTLIMPL_UHEX_0x624u 1572u, 0x624, 1572, 0, 0x9DBu, 6, 2, 4
#define PPUTLIMPL_UHEX_0x623u 1571u, 0x623, 1571, 0, 0x9DCu, 6, 2, 3
#define PPUTLIMPL_UHEX_0x622u 1570u, 0x622, 1570, 0, 0x9DDu, 6, 2, 2
#define PPUTLIMPL_UHEX_0x621u 1569u, 0x621, 1569, 0, 0x9DEu, 6, 2, 1
#define PPUTLIMPL_UHEX_0x620u 1568u, 0x620, 1568, 0, 0x9DFu, 6, 2, 0
#define PPUTLIMPL_UHEX_0x61Fu 1567u, 0x61F, 1567, 0, 0x9E0u, 6, 1, F
#define PPUTLIMPL_UHEX_0x61Eu 1566u, 0x61E, 1566, 0, 0x9E1u, 6, 1, E
#define PPUTLIMPL_UHEX_0x61Du 1565u, 0x61D, 1565, 0, 0x9E2u, 6, 1, D
#define PPUTLIMPL_UHEX_0x61Cu 1564u, 0x61C, 1564, 0, 0x9E3u, 6, 1, C
#define PPUTLIMPL_UHEX_0x61Bu 1563u, 0x61B, 1563, 0, 0x9E4u, 6, 1, B
#define PPUTLIMPL_UHEX_0x61Au 1562u, 0x61A, 1562, 0, 0x9E5u, 6, 1, A
#define PPUTLIMPL_UHEX_0x619u 1561u, 0x619, 1561, 0, 0x9E6u, 6, 1, 9
#define PPUTLIMPL_UHEX_0x618u 1560u, 0x618, 1560, 0, 0x9E7u, 6, 1, 8
#define PPUTLIMPL_UHEX_0x617u 1559u, 0x617, 1559, 0, 0x9E8u, 6, 1, 7
#define PPUTLIMPL_UHEX_0x616u 1558u, 0x616, 1558, 0, 0x9E9u, 6, 1, 6
#define PPUTLIMPL_UHEX_0x615u 1557u, 0x615, 1557, 0, 0x9EAu, 6, 1, 5
#define PPUTLIMPL_UHEX_0x614u 1556u, 0x614, 1556, 0, 0x9EBu, 6, 1, 4
#define PPUTLIMPL_UHEX_0x613u 1555u, 0x613, 1555, 0, 0x9ECu, 6, 1, 3
#define PPUTLIMPL_UHEX_0x612u 1554u, 0x612, 1554, 0, 0x9EDu, 6, 1, 2
#define PPUTLIMPL_UHEX_0x611u 1553u, 0x611, 1553, 0, 0x9EEu, 6, 1, 1
#define PPUTLIMPL_UHEX_0x610u 1552u, 0x610, 1552, 0, 0x9EFu, 6, 1, 0
#define PPUTLIMPL_UHEX_0x60Fu 1551u, 0x60F, 1551, 0, 0x9F0u, 6, 0, F
#define PPUTLIMPL_UHEX_0x60Eu 1550u, 0x60E, 1550, 0, 0x9F1u, 6, 0, E
#define PPUTLIMPL_UHEX_0x60Du 1549u, 0x60D, 1549, 0, 0x9F2u, 6, 0, D
#define PPUTLIMPL_UHEX_0x60Cu 1548u, 0x60C, 1548, 0, 0x9F3u, 6, 0, C
#define PPUTLIMPL_UHEX_0x60Bu 1547u, 0x60B, 1547, 0, 0x9F4u, 6, 0, B
#define PPUTLIMPL_UHEX_0x60Au 1546u, 0x60A, 1546, 0, 0x9F5u, 6, 0, A
#define PPUTLIMPL_UHEX_0x609u 1545u, 0x609, 1545, 0, 0x9F6u, 6, 0, 9
#define PPUTLIMPL_UHEX_0x608u 1544u, 0x608, 1544, 0, 0x9F7u, 6, 0, 8
#define PPUTLIMPL_UHEX_0x607u 1543u, 0x607, 1543, 0, 0x9F8u, 6, 0, 7
#define PPUTLIMPL_UHEX_0x606u 1542u, 0x606, 1542, 0, 0x9F9u, 6, 0, 6
#define PPUTLIMPL_UHEX_0x605u 1541u, 0x605, 1541, 0, 0x9FAu, 6, 0, 5
#define PPUTLIMPL_UHEX_0x604u 1540u, 0x604, 1540, 0, 0x9FBu, 6, 0, 4
#define PPUTLIMPL_UHEX_0x603u 1539u, 0x603, 1539, 0, 0x9FCu, 6, 0, 3
#define PPUTLIMPL_UHEX_0x602u 1538u, 0x602, 1538, 0, 0x9FDu, 6, 0, 2
#define PPUTLIMPL_UHEX_0x601u 1537u, 0x601, 1537, 0, 0x9FEu, 6, 0, 1
#define PPUTLIMPL_UHEX_0x600u 1536u, 0x600, 1536, 0, 0x9FFu, 6, 0, 0
#define PPUTLIMPL_UHEX_0x5FFu 1535u, 0x5FF, 1535, 0, 0xA00u, 5, F, F
#define PPUTLIMPL_UHEX_0x5FEu 1534u, 0x5FE, 1534, 0, 0xA01u, 5, F, E
#define PPUTLIMPL_UHEX_0x5FDu 1533u, 0x5FD, 1533, 0, 0xA02u, 5, F, D
#define PPUTLIMPL_UHEX_0x5FCu 1532u, 0x5FC, 1532, 0, 0xA03u, 5, F, C
#define PPUTLIMPL_UHEX_0x5FBu 1531u, 0x5FB, 1531, 0, 0xA04u, 5, F, B
#define PPUTLIMPL_UHEX_0x5FAu 1530u, 0x5FA, 1530, 0, 0xA05u, 5, F, A
#define PPUTLIMPL_UHEX_0x5F9u 1529u, 0x5F9, 1529, 0, 0xA06u, 5, F, 9
#define PPUTLIMPL_UHEX_0x5F8u 1528u, 0x5F8, 1528, 0, 0xA07u, 5, F, 8
#define PPUTLIMPL_UHEX_0x5F7u 1527u, 0x5F7, 1527, 0, 0xA08u, 5, F, 7
#define PPUTLIMPL_UHEX_0x5F6u 1526u, 0x5F6, 1526, 0, 0xA09u, 5, F, 6
#define PPUTLIMPL_UHEX_0x5F5u 1525u, 0x5F5, 1525, 0, 0xA0Au, 5, F, 5
#define PPUTLIMPL_UHEX_0x5F4u 1524u, 0x5F4, 1524, 0, 0xA0Bu, 5, F, 4
#define PPUTLIMPL_UHEX_0x5F3u 1523u, 0x5F3, 1523, 0, 0xA0Cu, 5, F, 3
#define PPUTLIMPL_UHEX_0x5F2u 1522u, 0x5F2, 1522, 0, 0xA0Du, 5, F, 2
#define PPUTLIMPL_UHEX_0x5F1u 1521u, 0x5F1, 1521, 0, 0xA0Eu, 5, F, 1
#define PPUTLIMPL_UHEX_0x5F0u 1520u, 0x5F0, 1520, 0, 0xA0Fu, 5, F, 0
#define PPUTLIMPL_UHEX_0x5EFu 1519u, 0x5EF, 1519, 0, 0xA10u, 5, E, F
#define PPUTLIMPL_UHEX_0x5EEu 1518u, 0x5EE, 1518, 0, 0xA11u, 5, E, E
#define PPUTLIMPL_UHEX_0x5EDu 1517u, 0x5ED, 1517, 0, 0xA12u, 5, E, D
#define PPUTLIMPL_UHEX_0x5ECu 1516u, 0x5EC, 1516, 0, 0xA13u, 5, E, C
#define PPUTLIMPL_UHEX_0x5EBu 1515u, 0x5EB, 1515, 0, 0xA14u, 5, E, B
#define PPUTLIMPL_UHEX_0x5EAu 1514u, 0x5EA, 1514, 0, 0xA15u, 5, E, A
#define PPUTLIMPL_UHEX_0x5E9u 1513u, 0x5E9, 1513, 0, 0xA16u, 5, E, 9
#define PPUTLIMPL_UHEX_0x5E8u 1512u, 0x5E8, 1512, 0, 0xA17u, 5, E, 8
#define PPUTLIMPL_UHEX_0x5E7u 1511u, 0x5E7, 1511, 0, 0xA18u, 5, E, 7
#define PPUTLIMPL_UHEX_0x5E6u 1510u, 0x5E6, 1510, 0, 0xA19u, 5, E, 6
#define PPUTLIMPL_UHEX_0x5E5u 1509u, 0x5E5, 1509, 0, 0xA1Au, 5, E, 5
#define PPUTLIMPL_UHEX_0x5E4u 1508u, 0x5E4, 1508, 0, 0xA1Bu, 5, E, 4
#define PPUTLIMPL_UHEX_0x5E3u 1507u, 0x5E3, 1507, 0, 0xA1Cu, 5, E, 3
#define PPUTLIMPL_UHEX_0x5E2u 1506u, 0x5E2, 1506, 0, 0xA1Du, 5, E, 2
#define PPUTLIMPL_UHEX_0x5E1u 1505u, 0x5E1, 1505, 0, 0xA1Eu, 5, E, 1
#define PPUTLIMPL_UHEX_0x5E0u 1504u, 0x5E0, 1504, 0, 0xA1Fu, 5, E, 0
#define PPUTLIMPL_UHEX_0x5DFu 1503u, 0x5DF, 1503, 0, 0xA20u, 5, D, F
#define PPUTLIMPL_UHEX_0x5DEu 1502u, 0x5DE, 1502, 0, 0xA21u, 5, D, E
#define PPUTLIMPL_UHEX_0x5DDu 1501u, 0x5DD, 1501, 0, 0xA22u, 5, D, D
#define PPUTLIMPL_UHEX_0x5DCu 1500u, 0x5DC, 1500, 0, 0xA23u, 5, D, C
#define PPUTLIMPL_UHEX_0x5DBu 1499u, 0x5DB, 1499, 0, 0xA24u, 5, D, B
#define PPUTLIMPL_UHEX_0x5DAu 1498u, 0x5DA, 1498, 0, 0xA25u, 5, D, A
#define PPUTLIMPL_UHEX_0x5D9u 1497u, 0x5D9, 1497, 0, 0xA26u, 5, D, 9
#define PPUTLIMPL_UHEX_0x5D8u 1496u, 0x5D8, 1496, 0, 0xA27u, 5, D, 8
#define PPUTLIMPL_UHEX_0x5D7u 1495u, 0x5D7, 1495, 0, 0xA28u, 5, D, 7
#define PPUTLIMPL_UHEX_0x5D6u 1494u, 0x5D6, 1494, 0, 0xA29u, 5, D, 6
#define PPUTLIMPL_UHEX_0x5D5u 1493u, 0x5D5, 1493, 0, 0xA2Au, 5, D, 5
#define PPUTLIMPL_UHEX_0x5D4u 1492u, 0x5D4, 1492, 0, 0xA2Bu, 5, D, 4
#define PPUTLIMPL_UHEX_0x5D3u 1491u, 0x5D3, 1491, 0, 0xA2Cu, 5, D, 3
#define PPUTLIMPL_UHEX_0x5D2u 1490u, 0x5D2, 1490, 0, 0xA2Du, 5, D, 2
#define PPUTLIMPL_UHEX_0x5D1u 1489u, 0x5D1, 1489, 0, 0xA2Eu, 5, D, 1
#define PPUTLIMPL_UHEX_0x5D0u 1488u, 0x5D0, 1488, 0, 0xA2Fu, 5, D, 0
#define PPUTLIMPL_UHEX_0x5CFu 1487u, 0x5CF, 1487, 0, 0xA30u, 5, C, F
#define PPUTLIMPL_UHEX_0x5CEu 1486u, 0x5CE, 1486, 0, 0xA31u, 5, C, E
#define PPUTLIMPL_UHEX_0x5CDu 1485u, 0x5CD, 1485, 0, 0xA32u, 5, C, D
#define PPUTLIMPL_UHEX_0x5CCu 1484u, 0x5CC, 1484, 0, 0xA33u, 5, C, C
#define PPUTLIMPL_UHEX_0x5CBu 1483u, 0x5CB, 1483, 0, 0xA34u, 5, C, B
#define PPUTLIMPL_UHEX_0x5CAu 1482u, 0x5CA, 1482, 0, 0xA35u, 5, C, A
#define PPUTLIMPL_UHEX_0x5C9u 1481u, 0x5C9, 1481, 0, 0xA36u, 5, C, 9
#define PPUTLIMPL_UHEX_0x5C8u 1480u, 0x5C8, 1480, 0, 0xA37u, 5, C, 8
#define PPUTLIMPL_UHEX_0x5C7u 1479u, 0x5C7, 1479, 0, 0xA38u, 5, C, 7
#define PPUTLIMPL_UHEX_0x5C6u 1478u, 0x5C6, 1478, 0, 0xA39u, 5, C, 6
#define PPUTLIMPL_UHEX_0x5C5u 1477u, 0x5C5, 1477, 0, 0xA3Au, 5, C, 5
#define PPUTLIMPL_UHEX_0x5C4u 1476u, 0x5C4, 1476, 0, 0xA3Bu, 5, C, 4
#define PPUTLIMPL_UHEX_0x5C3u 1475u, 0x5C3, 1475, 0, 0xA3Cu, 5, C, 3
#define PPUTLIMPL_UHEX_0x5C2u 1474u, 0x5C2, 1474, 0, 0xA3Du, 5, C, 2
#define PPUTLIMPL_UHEX_0x5C1u 1473u, 0x5C1, 1473, 0, 0xA3Eu, 5, C, 1
#define PPUTLIMPL_UHEX_0x5C0u 1472u, 0x5C0, 1472, 0, 0xA3Fu, 5, C, 0
#define PPUTLIMPL_UHEX_0x5BFu 1471u, 0x5BF, 1471, 0, 0xA40u, 5, B, F
#define PPUTLIMPL_UHEX_0x5BEu 1470u, 0x5BE, 1470, 0, 0xA41u, 5, B, E
#define PPUTLIMPL_UHEX_0x5BDu 1469u, 0x5BD, 1469, 0, 0xA42u, 5, B, D
#define PPUTLIMPL_UHEX_0x5BCu 1468u, 0x5BC, 1468, 0, 0xA43u, 5, B, C
#define PPUTLIMPL_UHEX_0x5BBu 1467u, 0x5BB, 1467, 0, 0xA44u, 5, B, B
#define PPUTLIMPL_UHEX_0x5BAu 1466u, 0x5BA, 1466, 0, 0xA45u, 5, B, A
#define PPUTLIMPL_UHEX_0x5B9u 1465u, 0x5B9, 1465, 0, 0xA46u, 5, B, 9
#define PPUTLIMPL_UHEX_0x5B8u 1464u, 0x5B8, 1464, 0, 0xA47u, 5, B, 8
#define PPUTLIMPL_UHEX_0x5B7u 1463u, 0x5B7, 1463, 0, 0xA48u, 5, B, 7
#define PPUTLIMPL_UHEX_0x5B6u 1462u, 0x5B6, 1462, 0, 0xA49u, 5, B, 6
#define PPUTLIMPL_UHEX_0x5B5u 1461u, 0x5B5, 1461, 0, 0xA4Au, 5, B, 5
#define PPUTLIMPL_UHEX_0x5B4u 1460u, 0x5B4, 1460, 0, 0xA4Bu, 5, B, 4
#define PPUTLIMPL_UHEX_0x5B3u 1459u, 0x5B3, 1459, 0, 0xA4Cu, 5, B, 3
#define PPUTLIMPL_UHEX_0x5B2u 1458u, 0x5B2, 1458, 0, 0xA4Du, 5, B, 2
#define PPUTLIMPL_UHEX_0x5B1u 1457u, 0x5B1, 1457, 0, 0xA4Eu, 5, B, 1
#define PPUTLIMPL_UHEX_0x5B0u 1456u, 0x5B0, 1456, 0, 0xA4Fu, 5, B, 0
#define PPUTLIMPL_UHEX_0x5AFu 1455u, 0x5AF, 1455, 0, 0xA50u, 5, A, F
#define PPUTLIMPL_UHEX_0x5AEu 1454u, 0x5AE, 1454, 0, 0xA51u, 5, A, E
#define PPUTLIMPL_UHEX_0x5ADu 1453u, 0x5AD, 1453, 0, 0xA52u, 5, A, D
#define PPUTLIMPL_UHEX_0x5ACu 1452u, 0x5AC, 1452, 0, 0xA53u, 5, A, C
#define PPUTLIMPL_UHEX_0x5ABu 1451u, 0x5AB, 1451, 0, 0xA54u, 5, A, B
#define PPUTLIMPL_UHEX_0x5AAu 1450u, 0x5AA, 1450, 0, 0xA55u, 5, A, A
#define PPUTLIMPL_UHEX_0x5A9u 1449u, 0x5A9, 1449, 0, 0xA56u, 5, A, 9
#define PPUTLIMPL_UHEX_0x5A8u 1448u, 0x5A8, 1448, 0, 0xA57u, 5, A, 8
#define PPUTLIMPL_UHEX_0x5A7u 1447u, 0x5A7, 1447, 0, 0xA58u, 5, A, 7
#define PPUTLIMPL_UHEX_0x5A6u 1446u, 0x5A6, 1446, 0, 0xA59u, 5, A, 6
#define PPUTLIMPL_UHEX_0x5A5u 1445u, 0x5A5, 1445, 0, 0xA5Au, 5, A, 5
#define PPUTLIMPL_UHEX_0x5A4u 1444u, 0x5A4, 1444, 0, 0xA5Bu, 5, A, 4
#define PPUTLIMPL_UHEX_0x5A3u 1443u, 0x5A3, 1443, 0, 0xA5Cu, 5, A, 3
#define PPUTLIMPL_UHEX_0x5A2u 1442u, 0x5A2, 1442, 0, 0xA5Du, 5, A, 2
#define PPUTLIMPL_UHEX_0x5A1u 1441u, 0x5A1, 1441, 0, 0xA5Eu, 5, A, 1
#define PPUTLIMPL_UHEX_0x5A0u 1440u, 0x5A0, 1440, 0, 0xA5Fu, 5, A, 0
#define PPUTLIMPL_UHEX_0x59Fu 1439u, 0x59F, 1439, 0, 0xA60u, 5, 9, F
#define PPUTLIMPL_UHEX_0x59Eu 1438u, 0x59E, 1438, 0, 0xA61u, 5, 9, E
#define PPUTLIMPL_UHEX_0x59Du 1437u, 0x59D, 1437, 0, 0xA62u, 5, 9, D
#define PPUTLIMPL_UHEX_0x59Cu 1436u, 0x59C, 1436, 0, 0xA63u, 5, 9, C
#define PPUTLIMPL_UHEX_0x59Bu 1435u, 0x59B, 1435, 0, 0xA64u, 5, 9, B
#define PPUTLIMPL_UHEX_0x59Au 1434u, 0x59A, 1434, 0, 0xA65u, 5, 9, A
#define PPUTLIMPL_UHEX_0x599u 1433u, 0x599, 1433, 0, 0xA66u, 5, 9, 9
#define PPUTLIMPL_UHEX_0x598u 1432u, 0x598, 1432, 0, 0xA67u, 5, 9, 8
#define PPUTLIMPL_UHEX_0x597u 1431u, 0x597, 1431, 0, 0xA68u, 5, 9, 7
#define PPUTLIMPL_UHEX_0x596u 1430u, 0x596, 1430, 0, 0xA69u, 5, 9, 6
#define PPUTLIMPL_UHEX_0x595u 1429u, 0x595, 1429, 0, 0xA6Au, 5, 9, 5
#define PPUTLIMPL_UHEX_0x594u 1428u, 0x594, 1428, 0, 0xA6Bu, 5, 9, 4
#define PPUTLIMPL_UHEX_0x593u 1427u, 0x593, 1427, 0, 0xA6Cu, 5, 9, 3
#define PPUTLIMPL_UHEX_0x592u 1426u, 0x592, 1426, 0, 0xA6Du, 5, 9, 2
#define PPUTLIMPL_UHEX_0x591u 1425u, 0x591, 1425, 0, 0xA6Eu, 5, 9, 1
#define PPUTLIMPL_UHEX_0x590u 1424u, 0x590, 1424, 0, 0xA6Fu, 5, 9, 0
#define PPUTLIMPL_UHEX_0x58Fu 1423u, 0x58F, 1423, 0, 0xA70u, 5, 8, F
#define PPUTLIMPL_UHEX_0x58Eu 1422u, 0x58E, 1422, 0, 0xA71u, 5, 8, E
#define PPUTLIMPL_UHEX_0x58Du 1421u, 0x58D, 1421, 0, 0xA72u, 5, 8, D
#define PPUTLIMPL_UHEX_0x58Cu 1420u, 0x58C, 1420, 0, 0xA73u, 5, 8, C
#define PPUTLIMPL_UHEX_0x58Bu 1419u, 0x58B, 1419, 0, 0xA74u, 5, 8, B
#define PPUTLIMPL_UHEX_0x58Au 1418u, 0x58A, 1418, 0, 0xA75u, 5, 8, A
#define PPUTLIMPL_UHEX_0x589u 1417u, 0x589, 1417, 0, 0xA76u, 5, 8, 9
#define PPUTLIMPL_UHEX_0x588u 1416u, 0x588, 1416, 0, 0xA77u, 5, 8, 8
#define PPUTLIMPL_UHEX_0x587u 1415u, 0x587, 1415, 0, 0xA78u, 5, 8, 7
#define PPUTLIMPL_UHEX_0x586u 1414u, 0x586, 1414, 0, 0xA79u, 5, 8, 6
#define PPUTLIMPL_UHEX_0x585u 1413u, 0x585, 1413, 0, 0xA7Au, 5, 8, 5
#define PPUTLIMPL_UHEX_0x584u 1412u, 0x584, 1412, 0, 0xA7Bu, 5, 8, 4
#define PPUTLIMPL_UHEX_0x583u 1411u, 0x583, 1411, 0, 0xA7Cu, 5, 8, 3
#define PPUTLIMPL_UHEX_0x582u 1410u, 0x582, 1410, 0, 0xA7Du, 5, 8, 2
#define PPUTLIMPL_UHEX_0x581u 1409u, 0x581, 1409, 0, 0xA7Eu, 5, 8, 1
#define PPUTLIMPL_UHEX_0x580u 1408u, 0x580, 1408, 0, 0xA7Fu, 5, 8, 0
#define PPUTLIMPL_UHEX_0x57Fu 1407u, 0x57F, 1407, 0, 0xA80u, 5, 7, F
#define PPUTLIMPL_UHEX_0x57Eu 1406u, 0x57E, 1406, 0, 0xA81u, 5, 7, E
#define PPUTLIMPL_UHEX_0x57Du 1405u, 0x57D, 1405, 0, 0xA82u, 5, 7, D
#define PPUTLIMPL_UHEX_0x57Cu 1404u, 0x57C, 1404, 0, 0xA83u, 5, 7, C
#define PPUTLIMPL_UHEX_0x57Bu 1403u, 0x57B, 1403, 0, 0xA84u, 5, 7, B
#define PPUTLIMPL_UHEX_0x57Au 1402u, 0x57A, 1402, 0, 0xA85u, 5, 7, A
#define PPUTLIMPL_UHEX_0x579u 1401u, 0x579, 1401, 0, 0xA86u, 5, 7, 9
#define PPUTLIMPL_UHEX_0x578u 1400u, 0x578, 1400, 0, 0xA87u, 5, 7, 8
#define PPUTLIMPL_UHEX_0x577u 1399u, 0x577, 1399, 0, 0xA88u, 5, 7, 7
#define PPUTLIMPL_UHEX_0x576u 1398u, 0x576, 1398, 0, 0xA89u, 5, 7, 6
#define PPUTLIMPL_UHEX_0x575u 1397u, 0x575, 1397, 0, 0xA8Au, 5, 7, 5
#define PPUTLIMPL_UHEX_0x574u 1396u, 0x574, 1396, 0, 0xA8Bu, 5, 7, 4
#define PPUTLIMPL_UHEX_0x573u 1395u, 0x573, 1395, 0, 0xA8Cu, 5, 7, 3
#define PPUTLIMPL_UHEX_0x572u 1394u, 0x572, 1394, 0, 0xA8Du, 5, 7, 2
#define PPUTLIMPL_UHEX_0x571u 1393u, 0x571, 1393, 0, 0xA8Eu, 5, 7, 1
#define PPUTLIMPL_UHEX_0x570u 1392u, 0x570, 1392, 0, 0xA8Fu, 5, 7, 0
#define PPUTLIMPL_UHEX_0x56Fu 1391u, 0x56F, 1391, 0, 0xA90u, 5, 6, F
#define PPUTLIMPL_UHEX_0x56Eu 1390u, 0x56E, 1390, 0, 0xA91u, 5, 6, E
#define PPUTLIMPL_UHEX_0x56Du 1389u, 0x56D, 1389, 0, 0xA92u, 5, 6, D
#define PPUTLIMPL_UHEX_0x56Cu 1388u, 0x56C, 1388, 0, 0xA93u, 5, 6, C
#define PPUTLIMPL_UHEX_0x56Bu 1387u, 0x56B, 1387, 0, 0xA94u, 5, 6, B
#define PPUTLIMPL_UHEX_0x56Au 1386u, 0x56A, 1386, 0, 0xA95u, 5, 6, A
#define PPUTLIMPL_UHEX_0x569u 1385u, 0x569, 1385, 0, 0xA96u, 5, 6, 9
#define PPUTLIMPL_UHEX_0x568u 1384u, 0x568, 1384, 0, 0xA97u, 5, 6, 8
#define PPUTLIMPL_UHEX_0x567u 1383u, 0x567, 1383, 0, 0xA98u, 5, 6, 7
#define PPUTLIMPL_UHEX_0x566u 1382u, 0x566, 1382, 0, 0xA99u, 5, 6, 6
#define PPUTLIMPL_UHEX_0x565u 1381u, 0x565, 1381, 0, 0xA9Au, 5, 6, 5
#define PPUTLIMPL_UHEX_0x564u 1380u, 0x564, 1380, 0, 0xA9Bu, 5, 6, 4
#define PPUTLIMPL_UHEX_0x563u 1379u, 0x563, 1379, 0, 0xA9Cu, 5, 6, 3
#define PPUTLIMPL_UHEX_0x562u 1378u, 0x562, 1378, 0, 0xA9Du, 5, 6, 2
#define PPUTLIMPL_UHEX_0x561u 1377u, 0x561, 1377, 0, 0xA9Eu, 5, 6, 1
#define PPUTLIMPL_UHEX_0x560u 1376u, 0x560, 1376, 0, 0xA9Fu, 5, 6, 0
#define PPUTLIMPL_UHEX_0x55Fu 1375u, 0x55F, 1375, 0, 0xAA0u, 5, 5, F
#define PPUTLIMPL_UHEX_0x55Eu 1374u, 0x55E, 1374, 0, 0xAA1u, 5, 5, E
#define PPUTLIMPL_UHEX_0x55Du 1373u, 0x55D, 1373, 0, 0xAA2u, 5, 5, D
#define PPUTLIMPL_UHEX_0x55Cu 1372u, 0x55C, 1372, 0, 0xAA3u, 5, 5, C
#define PPUTLIMPL_UHEX_0x55Bu 1371u, 0x55B, 1371, 0, 0xAA4u, 5, 5, B
#define PPUTLIMPL_UHEX_0x55Au 1370u, 0x55A, 1370, 0, 0xAA5u, 5, 5, A
#define PPUTLIMPL_UHEX_0x559u 1369u, 0x559, 1369, 0, 0xAA6u, 5, 5, 9
#define PPUTLIMPL_UHEX_0x558u 1368u, 0x558, 1368, 0, 0xAA7u, 5, 5, 8
#define PPUTLIMPL_UHEX_0x557u 1367u, 0x557, 1367, 0, 0xAA8u, 5, 5, 7
#define PPUTLIMPL_UHEX_0x556u 1366u, 0x556, 1366, 0, 0xAA9u, 5, 5, 6
#define PPUTLIMPL_UHEX_0x555u 1365u, 0x555, 1365, 0, 0xAAAu, 5, 5, 5
#define PPUTLIMPL_UHEX_0x554u 1364u, 0x554, 1364, 0, 0xAABu, 5, 5, 4
#define PPUTLIMPL_UHEX_0x553u 1363u, 0x553, 1363, 0, 0xAACu, 5, 5, 3
#define PPUTLIMPL_UHEX_0x552u 1362u, 0x552, 1362, 0, 0xAADu, 5, 5, 2
#define PPUTLIMPL_UHEX_0x551u 1361u, 0x551, 1361, 0, 0xAAEu, 5, 5, 1
#define PPUTLIMPL_UHEX_0x550u 1360u, 0x550, 1360, 0, 0xAAFu, 5, 5, 0
#define PPUTLIMPL_UHEX_0x54Fu 1359u, 0x54F, 1359, 0, 0xAB0u, 5, 4, F
#define PPUTLIMPL_UHEX_0x54Eu 1358u, 0x54E, 1358, 0, 0xAB1u, 5, 4, E
#define PPUTLIMPL_UHEX_0x54Du 1357u, 0x54D, 1357, 0, 0xAB2u, 5, 4, D
#define PPUTLIMPL_UHEX_0x54Cu 1356u, 0x54C, 1356, 0, 0xAB3u, 5, 4, C
#define PPUTLIMPL_UHEX_0x54Bu 1355u, 0x54B, 1355, 0, 0xAB4u, 5, 4, B
#define PPUTLIMPL_UHEX_0x54Au 1354u, 0x54A, 1354, 0, 0xAB5u, 5, 4, A
#define PPUTLIMPL_UHEX_0x549u 1353u, 0x549, 1353, 0, 0xAB6u, 5, 4, 9
#define PPUTLIMPL_UHEX_0x548u 1352u, 0x548, 1352, 0, 0xAB7u, 5, 4, 8
#define PPUTLIMPL_UHEX_0x547u 1351u, 0x547, 1351, 0, 0xAB8u, 5, 4, 7
#define PPUTLIMPL_UHEX_0x546u 1350u, 0x546, 1350, 0, 0xAB9u, 5, 4, 6
#define PPUTLIMPL_UHEX_0x545u 1349u, 0x545, 1349, 0, 0xABAu, 5, 4, 5
#define PPUTLIMPL_UHEX_0x544u 1348u, 0x544, 1348, 0, 0xABBu, 5, 4, 4
#define PPUTLIMPL_UHEX_0x543u 1347u, 0x543, 1347, 0, 0xABCu, 5, 4, 3
#define PPUTLIMPL_UHEX_0x542u 1346u, 0x542, 1346, 0, 0xABDu, 5, 4, 2
#define PPUTLIMPL_UHEX_0x541u 1345u, 0x541, 1345, 0, 0xABEu, 5, 4, 1
#define PPUTLIMPL_UHEX_0x540u 1344u, 0x540, 1344, 0, 0xABFu, 5, 4, 0
#define PPUTLIMPL_UHEX_0x53Fu 1343u, 0x53F, 1343, 0, 0xAC0u, 5, 3, F
#define PPUTLIMPL_UHEX_0x53Eu 1342u, 0x53E, 1342, 0, 0xAC1u, 5, 3, E
#define PPUTLIMPL_UHEX_0x53Du 1341u, 0x53D, 1341, 0, 0xAC2u, 5, 3, D
#define PPUTLIMPL_UHEX_0x53Cu 1340u, 0x53C, 1340, 0, 0xAC3u, 5, 3, C
#define PPUTLIMPL_UHEX_0x53Bu 1339u, 0x53B, 1339, 0, 0xAC4u, 5, 3, B
#define PPUTLIMPL_UHEX_0x53Au 1338u, 0x53A, 1338, 0, 0xAC5u, 5, 3, A
#define PPUTLIMPL_UHEX_0x539u 1337u, 0x539, 1337, 0, 0xAC6u, 5, 3, 9
#define PPUTLIMPL_UHEX_0x538u 1336u, 0x538, 1336, 0, 0xAC7u, 5, 3, 8
#define PPUTLIMPL_UHEX_0x537u 1335u, 0x537, 1335, 0, 0xAC8u, 5, 3, 7
#define PPUTLIMPL_UHEX_0x536u 1334u, 0x536, 1334, 0, 0xAC9u, 5, 3, 6
#define PPUTLIMPL_UHEX_0x535u 1333u, 0x535, 1333, 0, 0xACAu, 5, 3, 5
#define PPUTLIMPL_UHEX_0x534u 1332u, 0x534, 1332, 0, 0xACBu, 5, 3, 4
#define PPUTLIMPL_UHEX_0x533u 1331u, 0x533, 1331, 0, 0xACCu, 5, 3, 3
#define PPUTLIMPL_UHEX_0x532u 1330u, 0x532, 1330, 0, 0xACDu, 5, 3, 2
#define PPUTLIMPL_UHEX_0x531u 1329u, 0x531, 1329, 0, 0xACEu, 5, 3, 1
#define PPUTLIMPL_UHEX_0x530u 1328u, 0x530, 1328, 0, 0xACFu, 5, 3, 0
#define PPUTLIMPL_UHEX_0x52Fu 1327u, 0x52F, 1327, 0, 0xAD0u, 5, 2, F
#define PPUTLIMPL_UHEX_0x52Eu 1326u, 0x52E, 1326, 0, 0xAD1u, 5, 2, E
#define PPUTLIMPL_UHEX_0x52Du 1325u, 0x52D, 1325, 0, 0xAD2u, 5, 2, D
#define PPUTLIMPL_UHEX_0x52Cu 1324u, 0x52C, 1324, 0, 0xAD3u, 5, 2, C
#define PPUTLIMPL_UHEX_0x52Bu 1323u, 0x52B, 1323, 0, 0xAD4u, 5, 2, B
#define PPUTLIMPL_UHEX_0x52Au 1322u, 0x52A, 1322, 0, 0xAD5u, 5, 2, A
#define PPUTLIMPL_UHEX_0x529u 1321u, 0x529, 1321, 0, 0xAD6u, 5, 2, 9
#define PPUTLIMPL_UHEX_0x528u 1320u, 0x528, 1320, 0, 0xAD7u, 5, 2, 8
#define PPUTLIMPL_UHEX_0x527u 1319u, 0x527, 1319, 0, 0xAD8u, 5, 2, 7
#define PPUTLIMPL_UHEX_0x526u 1318u, 0x526, 1318, 0, 0xAD9u, 5, 2, 6
#define PPUTLIMPL_UHEX_0x525u 1317u, 0x525, 1317, 0, 0xADAu, 5, 2, 5
#define PPUTLIMPL_UHEX_0x524u 1316u, 0x524, 1316, 0, 0xADBu, 5, 2, 4
#define PPUTLIMPL_UHEX_0x523u 1315u, 0x523, 1315, 0, 0xADCu, 5, 2, 3
#define PPUTLIMPL_UHEX_0x522u 1314u, 0x522, 1314, 0, 0xADDu, 5, 2, 2
#define PPUTLIMPL_UHEX_0x521u 1313u, 0x521, 1313, 0, 0xADEu, 5, 2, 1
#define PPUTLIMPL_UHEX_0x520u 1312u, 0x520, 1312, 0, 0xADFu, 5, 2, 0
#define PPUTLIMPL_UHEX_0x51Fu 1311u, 0x51F, 1311, 0, 0xAE0u, 5, 1, F
#define PPUTLIMPL_UHEX_0x51Eu 1310u, 0x51E, 1310, 0, 0xAE1u, 5, 1, E
#define PPUTLIMPL_UHEX_0x51Du 1309u, 0x51D, 1309, 0, 0xAE2u, 5, 1, D
#define PPUTLIMPL_UHEX_0x51Cu 1308u, 0x51C, 1308, 0, 0xAE3u, 5, 1, C
#define PPUTLIMPL_UHEX_0x51Bu 1307u, 0x51B, 1307, 0, 0xAE4u, 5, 1, B
#define PPUTLIMPL_UHEX_0x51Au 1306u, 0x51A, 1306, 0, 0xAE5u, 5, 1, A
#define PPUTLIMPL_UHEX_0x519u 1305u, 0x519, 1305, 0, 0xAE6u, 5, 1, 9
#define PPUTLIMPL_UHEX_0x518u 1304u, 0x518, 1304, 0, 0xAE7u, 5, 1, 8
#define PPUTLIMPL_UHEX_0x517u 1303u, 0x517, 1303, 0, 0xAE8u, 5, 1, 7
#define PPUTLIMPL_UHEX_0x516u 1302u, 0x516, 1302, 0, 0xAE9u, 5, 1, 6
#define PPUTLIMPL_UHEX_0x515u 1301u, 0x515, 1301, 0, 0xAEAu, 5, 1, 5
#define PPUTLIMPL_UHEX_0x514u 1300u, 0x514, 1300, 0, 0xAEBu, 5, 1, 4
#define PPUTLIMPL_UHEX_0x513u 1299u, 0x513, 1299, 0, 0xAECu, 5, 1, 3
#define PPUTLIMPL_UHEX_0x512u 1298u, 0x512, 1298, 0, 0xAEDu, 5, 1, 2
#define PPUTLIMPL_UHEX_0x511u 1297u, 0x511, 1297, 0, 0xAEEu, 5, 1, 1
#define PPUTLIMPL_UHEX_0x510u 1296u, 0x510, 1296, 0, 0xAEFu, 5, 1, 0
#define PPUTLIMPL_UHEX_0x50Fu 1295u, 0x50F, 1295, 0, 0xAF0u, 5, 0, F
#define PPUTLIMPL_UHEX_0x50Eu 1294u, 0x50E, 1294, 0, 0xAF1u, 5, 0, E
#define PPUTLIMPL_UHEX_0x50Du 1293u, 0x50D, 1293, 0, 0xAF2u, 5, 0, D
#define PPUTLIMPL_UHEX_0x50Cu 1292u, 0x50C, 1292, 0, 0xAF3u, 5, 0, C
#define PPUTLIMPL_UHEX_0x50Bu 1291u, 0x50B, 1291, 0, 0xAF4u, 5, 0, B
#define PPUTLIMPL_UHEX_0x50Au 1290u, 0x50A, 1290, 0, 0xAF5u, 5, 0, A
#define PPUTLIMPL_UHEX_0x509u 1289u, 0x509, 1289, 0, 0xAF6u, 5, 0, 9
#define PPUTLIMPL_UHEX_0x508u 1288u, 0x508, 1288, 0, 0xAF7u, 5, 0, 8
#define PPUTLIMPL_UHEX_0x507u 1287u, 0x507, 1287, 0, 0xAF8u, 5, 0, 7
#define PPUTLIMPL_UHEX_0x506u 1286u, 0x506, 1286, 0, 0xAF9u, 5, 0, 6
#define PPUTLIMPL_UHEX_0x505u 1285u, 0x505, 1285, 0, 0xAFAu, 5, 0, 5
#define PPUTLIMPL_UHEX_0x504u 1284u, 0x504, 1284, 0, 0xAFBu, 5, 0, 4
#define PPUTLIMPL_UHEX_0x503u 1283u, 0x503, 1283, 0, 0xAFCu, 5, 0, 3
#define PPUTLIMPL_UHEX_0x502u 1282u, 0x502, 1282, 0, 0xAFDu, 5, 0, 2
#define PPUTLIMPL_UHEX_0x501u 1281u, 0x501, 1281, 0, 0xAFEu, 5, 0, 1
#define PPUTLIMPL_UHEX_0x500u 1280u, 0x500, 1280, 0, 0xAFFu, 5, 0, 0
#define PPUTLIMPL_UHEX_0x4FFu 1279u, 0x4FF, 1279, 0, 0xB00u, 4, F, F
#define PPUTLIMPL_UHEX_0x4FEu 1278u, 0x4FE, 1278, 0, 0xB01u, 4, F, E
#define PPUTLIMPL_UHEX_0x4FDu 1277u, 0x4FD, 1277, 0, 0xB02u, 4, F, D
#define PPUTLIMPL_UHEX_0x4FCu 1276u, 0x4FC, 1276, 0, 0xB03u, 4, F, C
#define PPUTLIMPL_UHEX_0x4FBu 1275u, 0x4FB, 1275, 0, 0xB04u, 4, F, B
#define PPUTLIMPL_UHEX_0x4FAu 1274u, 0x4FA, 1274, 0, 0xB05u, 4, F, A
#define PPUTLIMPL_UHEX_0x4F9u 1273u, 0x4F9, 1273, 0, 0xB06u, 4, F, 9
#define PPUTLIMPL_UHEX_0x4F8u 1272u, 0x4F8, 1272, 0, 0xB07u, 4, F, 8
#define PPUTLIMPL_UHEX_0x4F7u 1271u, 0x4F7, 1271, 0, 0xB08u, 4, F, 7
#define PPUTLIMPL_UHEX_0x4F6u 1270u, 0x4F6, 1270, 0, 0xB09u, 4, F, 6
#define PPUTLIMPL_UHEX_0x4F5u 1269u, 0x4F5, 1269, 0, 0xB0Au, 4, F, 5
#define PPUTLIMPL_UHEX_0x4F4u 1268u, 0x4F4, 1268, 0, 0xB0Bu, 4, F, 4
#define PPUTLIMPL_UHEX_0x4F3u 1267u, 0x4F3, 1267, 0, 0xB0Cu, 4, F, 3
#define PPUTLIMPL_UHEX_0x4F2u 1266u, 0x4F2, 1266, 0, 0xB0Du, 4, F, 2
#define PPUTLIMPL_UHEX_0x4F1u 1265u, 0x4F1, 1265, 0, 0xB0Eu, 4, F, 1
#define PPUTLIMPL_UHEX_0x4F0u 1264u, 0x4F0, 1264, 0, 0xB0Fu, 4, F, 0
#define PPUTLIMPL_UHEX_0x4EFu 1263u, 0x4EF, 1263, 0, 0xB10u, 4, E, F
#define PPUTLIMPL_UHEX_0x4EEu 1262u, 0x4EE, 1262, 0, 0xB11u, 4, E, E
#define PPUTLIMPL_UHEX_0x4EDu 1261u, 0x4ED, 1261, 0, 0xB12u, 4, E, D
#define PPUTLIMPL_UHEX_0x4ECu 1260u, 0x4EC, 1260, 0, 0xB13u, 4, E, C
#define PPUTLIMPL_UHEX_0x4EBu 1259u, 0x4EB, 1259, 0, 0xB14u, 4, E, B
#define PPUTLIMPL_UHEX_0x4EAu 1258u, 0x4EA, 1258, 0, 0xB15u, 4, E, A
#define PPUTLIMPL_UHEX_0x4E9u 1257u, 0x4E9, 1257, 0, 0xB16u, 4, E, 9
#define PPUTLIMPL_UHEX_0x4E8u 1256u, 0x4E8, 1256, 0, 0xB17u, 4, E, 8
#define PPUTLIMPL_UHEX_0x4E7u 1255u, 0x4E7, 1255, 0, 0xB18u, 4, E, 7
#define PPUTLIMPL_UHEX_0x4E6u 1254u, 0x4E6, 1254, 0, 0xB19u, 4, E, 6
#define PPUTLIMPL_UHEX_0x4E5u 1253u, 0x4E5, 1253, 0, 0xB1Au, 4, E, 5
#define PPUTLIMPL_UHEX_0x4E4u 1252u, 0x4E4, 1252, 0, 0xB1Bu, 4, E, 4
#define PPUTLIMPL_UHEX_0x4E3u 1251u, 0x4E3, 1251, 0, 0xB1Cu, 4, E, 3
#define PPUTLIMPL_UHEX_0x4E2u 1250u, 0x4E2, 1250, 0, 0xB1Du, 4, E, 2
#define PPUTLIMPL_UHEX_0x4E1u 1249u, 0x4E1, 1249, 0, 0xB1Eu, 4, E, 1
#define PPUTLIMPL_UHEX_0x4E0u 1248u, 0x4E0, 1248, 0, 0xB1Fu, 4, E, 0
#define PPUTLIMPL_UHEX_0x4DFu 1247u, 0x4DF, 1247, 0, 0xB20u, 4, D, F
#define PPUTLIMPL_UHEX_0x4DEu 1246u, 0x4DE, 1246, 0, 0xB21u, 4, D, E
#define PPUTLIMPL_UHEX_0x4DDu 1245u, 0x4DD, 1245, 0, 0xB22u, 4, D, D
#define PPUTLIMPL_UHEX_0x4DCu 1244u, 0x4DC, 1244, 0, 0xB23u, 4, D, C
#define PPUTLIMPL_UHEX_0x4DBu 1243u, 0x4DB, 1243, 0, 0xB24u, 4, D, B
#define PPUTLIMPL_UHEX_0x4DAu 1242u, 0x4DA, 1242, 0, 0xB25u, 4, D, A
#define PPUTLIMPL_UHEX_0x4D9u 1241u, 0x4D9, 1241, 0, 0xB26u, 4, D, 9
#define PPUTLIMPL_UHEX_0x4D8u 1240u, 0x4D8, 1240, 0, 0xB27u, 4, D, 8
#define PPUTLIMPL_UHEX_0x4D7u 1239u, 0x4D7, 1239, 0, 0xB28u, 4, D, 7
#define PPUTLIMPL_UHEX_0x4D6u 1238u, 0x4D6, 1238, 0, 0xB29u, 4, D, 6
#define PPUTLIMPL_UHEX_0x4D5u 1237u, 0x4D5, 1237, 0, 0xB2Au, 4, D, 5
#define PPUTLIMPL_UHEX_0x4D4u 1236u, 0x4D4, 1236, 0, 0xB2Bu, 4, D, 4
#define PPUTLIMPL_UHEX_0x4D3u 1235u, 0x4D3, 1235, 0, 0xB2Cu, 4, D, 3
#define PPUTLIMPL_UHEX_0x4D2u 1234u, 0x4D2, 1234, 0, 0xB2Du, 4, D, 2
#define PPUTLIMPL_UHEX_0x4D1u 1233u, 0x4D1, 1233, 0, 0xB2Eu, 4, D, 1
#define PPUTLIMPL_UHEX_0x4D0u 1232u, 0x4D0, 1232, 0, 0xB2Fu, 4, D, 0
#define PPUTLIMPL_UHEX_0x4CFu 1231u, 0x4CF, 1231, 0, 0xB30u, 4, C, F
#define PPUTLIMPL_UHEX_0x4CEu 1230u, 0x4CE, 1230, 0, 0xB31u, 4, C, E
#define PPUTLIMPL_UHEX_0x4CDu 1229u, 0x4CD, 1229, 0, 0xB32u, 4, C, D
#define PPUTLIMPL_UHEX_0x4CCu 1228u, 0x4CC, 1228, 0, 0xB33u, 4, C, C
#define PPUTLIMPL_UHEX_0x4CBu 1227u, 0x4CB, 1227, 0, 0xB34u, 4, C, B
#define PPUTLIMPL_UHEX_0x4CAu 1226u, 0x4CA, 1226, 0, 0xB35u, 4, C, A
#define PPUTLIMPL_UHEX_0x4C9u 1225u, 0x4C9, 1225, 0, 0xB36u, 4, C, 9
#define PPUTLIMPL_UHEX_0x4C8u 1224u, 0x4C8, 1224, 0, 0xB37u, 4, C, 8
#define PPUTLIMPL_UHEX_0x4C7u 1223u, 0x4C7, 1223, 0, 0xB38u, 4, C, 7
#define PPUTLIMPL_UHEX_0x4C6u 1222u, 0x4C6, 1222, 0, 0xB39u, 4, C, 6
#define PPUTLIMPL_UHEX_0x4C5u 1221u, 0x4C5, 1221, 0, 0xB3Au, 4, C, 5
#define PPUTLIMPL_UHEX_0x4C4u 1220u, 0x4C4, 1220, 0, 0xB3Bu, 4, C, 4
#define PPUTLIMPL_UHEX_0x4C3u 1219u, 0x4C3, 1219, 0, 0xB3Cu, 4, C, 3
#define PPUTLIMPL_UHEX_0x4C2u 1218u, 0x4C2, 1218, 0, 0xB3Du, 4, C, 2
#define PPUTLIMPL_UHEX_0x4C1u 1217u, 0x4C1, 1217, 0, 0xB3Eu, 4, C, 1
#define PPUTLIMPL_UHEX_0x4C0u 1216u, 0x4C0, 1216, 0, 0xB3Fu, 4, C, 0
#define PPUTLIMPL_UHEX_0x4BFu 1215u, 0x4BF, 1215, 0, 0xB40u, 4, B, F
#define PPUTLIMPL_UHEX_0x4BEu 1214u, 0x4BE, 1214, 0, 0xB41u, 4, B, E
#define PPUTLIMPL_UHEX_0x4BDu 1213u, 0x4BD, 1213, 0, 0xB42u, 4, B, D
#define PPUTLIMPL_UHEX_0x4BCu 1212u, 0x4BC, 1212, 0, 0xB43u, 4, B, C
#define PPUTLIMPL_UHEX_0x4BBu 1211u, 0x4BB, 1211, 0, 0xB44u, 4, B, B
#define PPUTLIMPL_UHEX_0x4BAu 1210u, 0x4BA, 1210, 0, 0xB45u, 4, B, A
#define PPUTLIMPL_UHEX_0x4B9u 1209u, 0x4B9, 1209, 0, 0xB46u, 4, B, 9
#define PPUTLIMPL_UHEX_0x4B8u 1208u, 0x4B8, 1208, 0, 0xB47u, 4, B, 8
#define PPUTLIMPL_UHEX_0x4B7u 1207u, 0x4B7, 1207, 0, 0xB48u, 4, B, 7
#define PPUTLIMPL_UHEX_0x4B6u 1206u, 0x4B6, 1206, 0, 0xB49u, 4, B, 6
#define PPUTLIMPL_UHEX_0x4B5u 1205u, 0x4B5, 1205, 0, 0xB4Au, 4, B, 5
#define PPUTLIMPL_UHEX_0x4B4u 1204u, 0x4B4, 1204, 0, 0xB4Bu, 4, B, 4
#define PPUTLIMPL_UHEX_0x4B3u 1203u, 0x4B3, 1203, 0, 0xB4Cu, 4, B, 3
#define PPUTLIMPL_UHEX_0x4B2u 1202u, 0x4B2, 1202, 0, 0xB4Du, 4, B, 2
#define PPUTLIMPL_UHEX_0x4B1u 1201u, 0x4B1, 1201, 0, 0xB4Eu, 4, B, 1
#define PPUTLIMPL_UHEX_0x4B0u 1200u, 0x4B0, 1200, 0, 0xB4Fu, 4, B, 0
#define PPUTLIMPL_UHEX_0x4AFu 1199u, 0x4AF, 1199, 0, 0xB50u, 4, A, F
#define PPUTLIMPL_UHEX_0x4AEu 1198u, 0x4AE, 1198, 0, 0xB51u, 4, A, E
#define PPUTLIMPL_UHEX_0x4ADu 1197u, 0x4AD, 1197, 0, 0xB52u, 4, A, D
#define PPUTLIMPL_UHEX_0x4ACu 1196u, 0x4AC, 1196, 0, 0xB53u, 4, A, C
#define PPUTLIMPL_UHEX_0x4ABu 1195u, 0x4AB, 1195, 0, 0xB54u, 4, A, B
#define PPUTLIMPL_UHEX_0x4AAu 1194u, 0x4AA, 1194, 0, 0xB55u, 4, A, A
#define PPUTLIMPL_UHEX_0x4A9u 1193u, 0x4A9, 1193, 0, 0xB56u, 4, A, 9
#define PPUTLIMPL_UHEX_0x4A8u 1192u, 0x4A8, 1192, 0, 0xB57u, 4, A, 8
#define PPUTLIMPL_UHEX_0x4A7u 1191u, 0x4A7, 1191, 0, 0xB58u, 4, A, 7
#define PPUTLIMPL_UHEX_0x4A6u 1190u, 0x4A6, 1190, 0, 0xB59u, 4, A, 6
#define PPUTLIMPL_UHEX_0x4A5u 1189u, 0x4A5, 1189, 0, 0xB5Au, 4, A, 5
#define PPUTLIMPL_UHEX_0x4A4u 1188u, 0x4A4, 1188, 0, 0xB5Bu, 4, A, 4
#define PPUTLIMPL_UHEX_0x4A3u 1187u, 0x4A3, 1187, 0, 0xB5Cu, 4, A, 3
#define PPUTLIMPL_UHEX_0x4A2u 1186u, 0x4A2, 1186, 0, 0xB5Du, 4, A, 2
#define PPUTLIMPL_UHEX_0x4A1u 1185u, 0x4A1, 1185, 0, 0xB5Eu, 4, A, 1
#define PPUTLIMPL_UHEX_0x4A0u 1184u, 0x4A0, 1184, 0, 0xB5Fu, 4, A, 0
#define PPUTLIMPL_UHEX_0x49Fu 1183u, 0x49F, 1183, 0, 0xB60u, 4, 9, F
#define PPUTLIMPL_UHEX_0x49Eu 1182u, 0x49E, 1182, 0, 0xB61u, 4, 9, E
#define PPUTLIMPL_UHEX_0x49Du 1181u, 0x49D, 1181, 0, 0xB62u, 4, 9, D
#define PPUTLIMPL_UHEX_0x49Cu 1180u, 0x49C, 1180, 0, 0xB63u, 4, 9, C
#define PPUTLIMPL_UHEX_0x49Bu 1179u, 0x49B, 1179, 0, 0xB64u, 4, 9, B
#define PPUTLIMPL_UHEX_0x49Au 1178u, 0x49A, 1178, 0, 0xB65u, 4, 9, A
#define PPUTLIMPL_UHEX_0x499u 1177u, 0x499, 1177, 0, 0xB66u, 4, 9, 9
#define PPUTLIMPL_UHEX_0x498u 1176u, 0x498, 1176, 0, 0xB67u, 4, 9, 8
#define PPUTLIMPL_UHEX_0x497u 1175u, 0x497, 1175, 0, 0xB68u, 4, 9, 7
#define PPUTLIMPL_UHEX_0x496u 1174u, 0x496, 1174, 0, 0xB69u, 4, 9, 6
#define PPUTLIMPL_UHEX_0x495u 1173u, 0x495, 1173, 0, 0xB6Au, 4, 9, 5
#define PPUTLIMPL_UHEX_0x494u 1172u, 0x494, 1172, 0, 0xB6Bu, 4, 9, 4
#define PPUTLIMPL_UHEX_0x493u 1171u, 0x493, 1171, 0, 0xB6Cu, 4, 9, 3
#define PPUTLIMPL_UHEX_0x492u 1170u, 0x492, 1170, 0, 0xB6Du, 4, 9, 2
#define PPUTLIMPL_UHEX_0x491u 1169u, 0x491, 1169, 0, 0xB6Eu, 4, 9, 1
#define PPUTLIMPL_UHEX_0x490u 1168u, 0x490, 1168, 0, 0xB6Fu, 4, 9, 0
#define PPUTLIMPL_UHEX_0x48Fu 1167u, 0x48F, 1167, 0, 0xB70u, 4, 8, F
#define PPUTLIMPL_UHEX_0x48Eu 1166u, 0x48E, 1166, 0, 0xB71u, 4, 8, E
#define PPUTLIMPL_UHEX_0x48Du 1165u, 0x48D, 1165, 0, 0xB72u, 4, 8, D
#define PPUTLIMPL_UHEX_0x48Cu 1164u, 0x48C, 1164, 0, 0xB73u, 4, 8, C
#define PPUTLIMPL_UHEX_0x48Bu 1163u, 0x48B, 1163, 0, 0xB74u, 4, 8, B
#define PPUTLIMPL_UHEX_0x48Au 1162u, 0x48A, 1162, 0, 0xB75u, 4, 8, A
#define PPUTLIMPL_UHEX_0x489u 1161u, 0x489, 1161, 0, 0xB76u, 4, 8, 9
#define PPUTLIMPL_UHEX_0x488u 1160u, 0x488, 1160, 0, 0xB77u, 4, 8, 8
#define PPUTLIMPL_UHEX_0x487u 1159u, 0x487, 1159, 0, 0xB78u, 4, 8, 7
#define PPUTLIMPL_UHEX_0x486u 1158u, 0x486, 1158, 0, 0xB79u, 4, 8, 6
#define PPUTLIMPL_UHEX_0x485u 1157u, 0x485, 1157, 0, 0xB7Au, 4, 8, 5
#define PPUTLIMPL_UHEX_0x484u 1156u, 0x484, 1156, 0, 0xB7Bu, 4, 8, 4
#define PPUTLIMPL_UHEX_0x483u 1155u, 0x483, 1155, 0, 0xB7Cu, 4, 8, 3
#define PPUTLIMPL_UHEX_0x482u 1154u, 0x482, 1154, 0, 0xB7Du, 4, 8, 2
#define PPUTLIMPL_UHEX_0x481u 1153u, 0x481, 1153, 0, 0xB7Eu, 4, 8, 1
#define PPUTLIMPL_UHEX_0x480u 1152u, 0x480, 1152, 0, 0xB7Fu, 4, 8, 0
#define PPUTLIMPL_UHEX_0x47Fu 1151u, 0x47F, 1151, 0, 0xB80u, 4, 7, F
#define PPUTLIMPL_UHEX_0x47Eu 1150u, 0x47E, 1150, 0, 0xB81u, 4, 7, E
#define PPUTLIMPL_UHEX_0x47Du 1149u, 0x47D, 1149, 0, 0xB82u, 4, 7, D
#define PPUTLIMPL_UHEX_0x47Cu 1148u, 0x47C, 1148, 0, 0xB83u, 4, 7, C
#define PPUTLIMPL_UHEX_0x47Bu 1147u, 0x47B, 1147, 0, 0xB84u, 4, 7, B
#define PPUTLIMPL_UHEX_0x47Au 1146u, 0x47A, 1146, 0, 0xB85u, 4, 7, A
#define PPUTLIMPL_UHEX_0x479u 1145u, 0x479, 1145, 0, 0xB86u, 4, 7, 9
#define PPUTLIMPL_UHEX_0x478u 1144u, 0x478, 1144, 0, 0xB87u, 4, 7, 8
#define PPUTLIMPL_UHEX_0x477u 1143u, 0x477, 1143, 0, 0xB88u, 4, 7, 7
#define PPUTLIMPL_UHEX_0x476u 1142u, 0x476, 1142, 0, 0xB89u, 4, 7, 6
#define PPUTLIMPL_UHEX_0x475u 1141u, 0x475, 1141, 0, 0xB8Au, 4, 7, 5
#define PPUTLIMPL_UHEX_0x474u 1140u, 0x474, 1140, 0, 0xB8Bu, 4, 7, 4
#define PPUTLIMPL_UHEX_0x473u 1139u, 0x473, 1139, 0, 0xB8Cu, 4, 7, 3
#define PPUTLIMPL_UHEX_0x472u 1138u, 0x472, 1138, 0, 0xB8Du, 4, 7, 2
#define PPUTLIMPL_UHEX_0x471u 1137u, 0x471, 1137, 0, 0xB8Eu, 4, 7, 1
#define PPUTLIMPL_UHEX_0x470u 1136u, 0x470, 1136, 0, 0xB8Fu, 4, 7, 0
#define PPUTLIMPL_UHEX_0x46Fu 1135u, 0x46F, 1135, 0, 0xB90u, 4, 6, F
#define PPUTLIMPL_UHEX_0x46Eu 1134u, 0x46E, 1134, 0, 0xB91u, 4, 6, E
#define PPUTLIMPL_UHEX_0x46Du 1133u, 0x46D, 1133, 0, 0xB92u, 4, 6, D
#define PPUTLIMPL_UHEX_0x46Cu 1132u, 0x46C, 1132, 0, 0xB93u, 4, 6, C
#define PPUTLIMPL_UHEX_0x46Bu 1131u, 0x46B, 1131, 0, 0xB94u, 4, 6, B
#define PPUTLIMPL_UHEX_0x46Au 1130u, 0x46A, 1130, 0, 0xB95u, 4, 6, A
#define PPUTLIMPL_UHEX_0x469u 1129u, 0x469, 1129, 0, 0xB96u, 4, 6, 9
#define PPUTLIMPL_UHEX_0x468u 1128u, 0x468, 1128, 0, 0xB97u, 4, 6, 8
#define PPUTLIMPL_UHEX_0x467u 1127u, 0x467, 1127, 0, 0xB98u, 4, 6, 7
#define PPUTLIMPL_UHEX_0x466u 1126u, 0x466, 1126, 0, 0xB99u, 4, 6, 6
#define PPUTLIMPL_UHEX_0x465u 1125u, 0x465, 1125, 0, 0xB9Au, 4, 6, 5
#define PPUTLIMPL_UHEX_0x464u 1124u, 0x464, 1124, 0, 0xB9Bu, 4, 6, 4
#define PPUTLIMPL_UHEX_0x463u 1123u, 0x463, 1123, 0, 0xB9Cu, 4, 6, 3
#define PPUTLIMPL_UHEX_0x462u 1122u, 0x462, 1122, 0, 0xB9Du, 4, 6, 2
#define PPUTLIMPL_UHEX_0x461u 1121u, 0x461, 1121, 0, 0xB9Eu, 4, 6, 1
#define PPUTLIMPL_UHEX_0x460u 1120u, 0x460, 1120, 0, 0xB9Fu, 4, 6, 0
#define PPUTLIMPL_UHEX_0x45Fu 1119u, 0x45F, 1119, 0, 0xBA0u, 4, 5, F
#define PPUTLIMPL_UHEX_0x45Eu 1118u, 0x45E, 1118, 0, 0xBA1u, 4, 5, E
#define PPUTLIMPL_UHEX_0x45Du 1117u, 0x45D, 1117, 0, 0xBA2u, 4, 5, D
#define PPUTLIMPL_UHEX_0x45Cu 1116u, 0x45C, 1116, 0, 0xBA3u, 4, 5, C
#define PPUTLIMPL_UHEX_0x45Bu 1115u, 0x45B, 1115, 0, 0xBA4u, 4, 5, B
#define PPUTLIMPL_UHEX_0x45Au 1114u, 0x45A, 1114, 0, 0xBA5u, 4, 5, A
#define PPUTLIMPL_UHEX_0x459u 1113u, 0x459, 1113, 0, 0xBA6u, 4, 5, 9
#define PPUTLIMPL_UHEX_0x458u 1112u, 0x458, 1112, 0, 0xBA7u, 4, 5, 8
#define PPUTLIMPL_UHEX_0x457u 1111u, 0x457, 1111, 0, 0xBA8u, 4, 5, 7
#define PPUTLIMPL_UHEX_0x456u 1110u, 0x456, 1110, 0, 0xBA9u, 4, 5, 6
#define PPUTLIMPL_UHEX_0x455u 1109u, 0x455, 1109, 0, 0xBAAu, 4, 5, 5
#define PPUTLIMPL_UHEX_0x454u 1108u, 0x454, 1108, 0, 0xBABu, 4, 5, 4
#define PPUTLIMPL_UHEX_0x453u 1107u, 0x453, 1107, 0, 0xBACu, 4, 5, 3
#define PPUTLIMPL_UHEX_0x452u 1106u, 0x452, 1106, 0, 0xBADu, 4, 5, 2
#define PPUTLIMPL_UHEX_0x451u 1105u, 0x451, 1105, 0, 0xBAEu, 4, 5, 1
#define PPUTLIMPL_UHEX_0x450u 1104u, 0x450, 1104, 0, 0xBAFu, 4, 5, 0
#define PPUTLIMPL_UHEX_0x44Fu 1103u, 0x44F, 1103, 0, 0xBB0u, 4, 4, F
#define PPUTLIMPL_UHEX_0x44Eu 1102u, 0x44E, 1102, 0, 0xBB1u, 4, 4, E
#define PPUTLIMPL_UHEX_0x44Du 1101u, 0x44D, 1101, 0, 0xBB2u, 4, 4, D
#define PPUTLIMPL_UHEX_0x44Cu 1100u, 0x44C, 1100, 0, 0xBB3u, 4, 4, C
#define PPUTLIMPL_UHEX_0x44Bu 1099u, 0x44B, 1099, 0, 0xBB4u, 4, 4, B
#define PPUTLIMPL_UHEX_0x44Au 1098u, 0x44A, 1098, 0, 0xBB5u, 4, 4, A
#define PPUTLIMPL_UHEX_0x449u 1097u, 0x449, 1097, 0, 0xBB6u, 4, 4, 9
#define PPUTLIMPL_UHEX_0x448u 1096u, 0x448, 1096, 0, 0xBB7u, 4, 4, 8
#define PPUTLIMPL_UHEX_0x447u 1095u, 0x447, 1095, 0, 0xBB8u, 4, 4, 7
#define PPUTLIMPL_UHEX_0x446u 1094u, 0x446, 1094, 0, 0xBB9u, 4, 4, 6
#define PPUTLIMPL_UHEX_0x445u 1093u, 0x445, 1093, 0, 0xBBAu, 4, 4, 5
#define PPUTLIMPL_UHEX_0x444u 1092u, 0x444, 1092, 0, 0xBBBu, 4, 4, 4
#define PPUTLIMPL_UHEX_0x443u 1091u, 0x443, 1091, 0, 0xBBCu, 4, 4, 3
#define PPUTLIMPL_UHEX_0x442u 1090u, 0x442, 1090, 0, 0xBBDu, 4, 4, 2
#define PPUTLIMPL_UHEX_0x441u 1089u, 0x441, 1089, 0, 0xBBEu, 4, 4, 1
#define PPUTLIMPL_UHEX_0x440u 1088u, 0x440, 1088, 0, 0xBBFu, 4, 4, 0
#define PPUTLIMPL_UHEX_0x43Fu 1087u, 0x43F, 1087, 0, 0xBC0u, 4, 3, F
#define PPUTLIMPL_UHEX_0x43Eu 1086u, 0x43E, 1086, 0, 0xBC1u, 4, 3, E
#define PPUTLIMPL_UHEX_0x43Du 1085u, 0x43D, 1085, 0, 0xBC2u, 4, 3, D
#define PPUTLIMPL_UHEX_0x43Cu 1084u, 0x43C, 1084, 0, 0xBC3u, 4, 3, C
#define PPUTLIMPL_UHEX_0x43Bu 1083u, 0x43B, 1083, 0, 0xBC4u, 4, 3, B
#define PPUTLIMPL_UHEX_0x43Au 1082u, 0x43A, 1082, 0, 0xBC5u, 4, 3, A
#define PPUTLIMPL_UHEX_0x439u 1081u, 0x439, 1081, 0, 0xBC6u, 4, 3, 9
#define PPUTLIMPL_UHEX_0x438u 1080u, 0x438, 1080, 0, 0xBC7u, 4, 3, 8
#define PPUTLIMPL_UHEX_0x437u 1079u, 0x437, 1079, 0, 0xBC8u, 4, 3, 7
#define PPUTLIMPL_UHEX_0x436u 1078u, 0x436, 1078, 0, 0xBC9u, 4, 3, 6
#define PPUTLIMPL_UHEX_0x435u 1077u, 0x435, 1077, 0, 0xBCAu, 4, 3, 5
#define PPUTLIMPL_UHEX_0x434u 1076u, 0x434, 1076, 0, 0xBCBu, 4, 3, 4
#define PPUTLIMPL_UHEX_0x433u 1075u, 0x433, 1075, 0, 0xBCCu, 4, 3, 3
#define PPUTLIMPL_UHEX_0x432u 1074u, 0x432, 1074, 0, 0xBCDu, 4, 3, 2
#define PPUTLIMPL_UHEX_0x431u 1073u, 0x431, 1073, 0, 0xBCEu, 4, 3, 1
#define PPUTLIMPL_UHEX_0x430u 1072u, 0x430, 1072, 0, 0xBCFu, 4, 3, 0
#define PPUTLIMPL_UHEX_0x42Fu 1071u, 0x42F, 1071, 0, 0xBD0u, 4, 2, F
#define PPUTLIMPL_UHEX_0x42Eu 1070u, 0x42E, 1070, 0, 0xBD1u, 4, 2, E
#define PPUTLIMPL_UHEX_0x42Du 1069u, 0x42D, 1069, 0, 0xBD2u, 4, 2, D
#define PPUTLIMPL_UHEX_0x42Cu 1068u, 0x42C, 1068, 0, 0xBD3u, 4, 2, C
#define PPUTLIMPL_UHEX_0x42Bu 1067u, 0x42B, 1067, 0, 0xBD4u, 4, 2, B
#define PPUTLIMPL_UHEX_0x42Au 1066u, 0x42A, 1066, 0, 0xBD5u, 4, 2, A
#define PPUTLIMPL_UHEX_0x429u 1065u, 0x429, 1065, 0, 0xBD6u, 4, 2, 9
#define PPUTLIMPL_UHEX_0x428u 1064u, 0x428, 1064, 0, 0xBD7u, 4, 2, 8
#define PPUTLIMPL_UHEX_0x427u 1063u, 0x427, 1063, 0, 0xBD8u, 4, 2, 7
#define PPUTLIMPL_UHEX_0x426u 1062u, 0x426, 1062, 0, 0xBD9u, 4, 2, 6
#define PPUTLIMPL_UHEX_0x425u 1061u, 0x425, 1061, 0, 0xBDAu, 4, 2, 5
#define PPUTLIMPL_UHEX_0x424u 1060u, 0x424, 1060, 0, 0xBDBu, 4, 2, 4
#define PPUTLIMPL_UHEX_0x423u 1059u, 0x423, 1059, 0, 0xBDCu, 4, 2, 3
#define PPUTLIMPL_UHEX_0x422u 1058u, 0x422, 1058, 0, 0xBDDu, 4, 2, 2
#define PPUTLIMPL_UHEX_0x421u 1057u, 0x421, 1057, 0, 0xBDEu, 4, 2, 1
#define PPUTLIMPL_UHEX_0x420u 1056u, 0x420, 1056, 0, 0xBDFu, 4, 2, 0
#define PPUTLIMPL_UHEX_0x41Fu 1055u, 0x41F, 1055, 0, 0xBE0u, 4, 1, F
#define PPUTLIMPL_UHEX_0x41Eu 1054u, 0x41E, 1054, 0, 0xBE1u, 4, 1, E
#define PPUTLIMPL_UHEX_0x41Du 1053u, 0x41D, 1053, 0, 0xBE2u, 4, 1, D
#define PPUTLIMPL_UHEX_0x41Cu 1052u, 0x41C, 1052, 0, 0xBE3u, 4, 1, C
#define PPUTLIMPL_UHEX_0x41Bu 1051u, 0x41B, 1051, 0, 0xBE4u, 4, 1, B
#define PPUTLIMPL_UHEX_0x41Au 1050u, 0x41A, 1050, 0, 0xBE5u, 4, 1, A
#define PPUTLIMPL_UHEX_0x419u 1049u, 0x419, 1049, 0, 0xBE6u, 4, 1, 9
#define PPUTLIMPL_UHEX_0x418u 1048u, 0x418, 1048, 0, 0xBE7u, 4, 1, 8
#define PPUTLIMPL_UHEX_0x417u 1047u, 0x417, 1047, 0, 0xBE8u, 4, 1, 7
#define PPUTLIMPL_UHEX_0x416u 1046u, 0x416, 1046, 0, 0xBE9u, 4, 1, 6
#define PPUTLIMPL_UHEX_0x415u 1045u, 0x415, 1045, 0, 0xBEAu, 4, 1, 5
#define PPUTLIMPL_UHEX_0x414u 1044u, 0x414, 1044, 0, 0xBEBu, 4, 1, 4
#define PPUTLIMPL_UHEX_0x413u 1043u, 0x413, 1043, 0, 0xBECu, 4, 1, 3
#define PPUTLIMPL_UHEX_0x412u 1042u, 0x412, 1042, 0, 0xBEDu, 4, 1, 2
#define PPUTLIMPL_UHEX_0x411u 1041u, 0x411, 1041, 0, 0xBEEu, 4, 1, 1
#define PPUTLIMPL_UHEX_0x410u 1040u, 0x410, 1040, 0, 0xBEFu, 4, 1, 0
#define PPUTLIMPL_UHEX_0x40Fu 1039u, 0x40F, 1039, 0, 0xBF0u, 4, 0, F
#define PPUTLIMPL_UHEX_0x40Eu 1038u, 0x40E, 1038, 0, 0xBF1u, 4, 0, E
#define PPUTLIMPL_UHEX_0x40Du 1037u, 0x40D, 1037, 0, 0xBF2u, 4, 0, D
#define PPUTLIMPL_UHEX_0x40Cu 1036u, 0x40C, 1036, 0, 0xBF3u, 4, 0, C
#define PPUTLIMPL_UHEX_0x40Bu 1035u, 0x40B, 1035, 0, 0xBF4u, 4, 0, B
#define PPUTLIMPL_UHEX_0x40Au 1034u, 0x40A, 1034, 0, 0xBF5u, 4, 0, A
#define PPUTLIMPL_UHEX_0x409u 1033u, 0x409, 1033, 0, 0xBF6u, 4, 0, 9
#define PPUTLIMPL_UHEX_0x408u 1032u, 0x408, 1032, 0, 0xBF7u, 4, 0, 8
#define PPUTLIMPL_UHEX_0x407u 1031u, 0x407, 1031, 0, 0xBF8u, 4, 0, 7
#define PPUTLIMPL_UHEX_0x406u 1030u, 0x406, 1030, 0, 0xBF9u, 4, 0, 6
#define PPUTLIMPL_UHEX_0x405u 1029u, 0x405, 1029, 0, 0xBFAu, 4, 0, 5
#define PPUTLIMPL_UHEX_0x404u 1028u, 0x404, 1028, 0, 0xBFBu, 4, 0, 4
#define PPUTLIMPL_UHEX_0x403u 1027u, 0x403, 1027, 0, 0xBFCu, 4, 0, 3
#define PPUTLIMPL_UHEX_0x402u 1026u, 0x402, 1026, 0, 0xBFDu, 4, 0, 2
#define PPUTLIMPL_UHEX_0x401u 1025u, 0x401, 1025, 0, 0xBFEu, 4, 0, 1
#define PPUTLIMPL_UHEX_0x400u 1024u, 0x400, 1024, 0, 0xBFFu, 4, 0, 0
#define PPUTLIMPL_UHEX_0x3FFu 1023u, 0x3FF, 1023, 0, 0xC00u, 3, F, F
#define PPUTLIMPL_UHEX_0x3FEu 1022u, 0x3FE, 1022, 0, 0xC01u, 3, F, E
#define PPUTLIMPL_UHEX_0x3FDu 1021u, 0x3FD, 1021, 0, 0xC02u, 3, F, D
#define PPUTLIMPL_UHEX_0x3FCu 1020u, 0x3FC, 1020, 0, 0xC03u, 3, F, C
#define PPUTLIMPL_UHEX_0x3FBu 1019u, 0x3FB, 1019, 0, 0xC04u, 3, F, B
#define PPUTLIMPL_UHEX_0x3FAu 1018u, 0x3FA, 1018, 0, 0xC05u, 3, F, A
#define PPUTLIMPL_UHEX_0x3F9u 1017u, 0x3F9, 1017, 0, 0xC06u, 3, F, 9
#define PPUTLIMPL_UHEX_0x3F8u 1016u, 0x3F8, 1016, 0, 0xC07u, 3, F, 8
#define PPUTLIMPL_UHEX_0x3F7u 1015u, 0x3F7, 1015, 0, 0xC08u, 3, F, 7
#define PPUTLIMPL_UHEX_0x3F6u 1014u, 0x3F6, 1014, 0, 0xC09u, 3, F, 6
#define PPUTLIMPL_UHEX_0x3F5u 1013u, 0x3F5, 1013, 0, 0xC0Au, 3, F, 5
#define PPUTLIMPL_UHEX_0x3F4u 1012u, 0x3F4, 1012, 0, 0xC0Bu, 3, F, 4
#define PPUTLIMPL_UHEX_0x3F3u 1011u, 0x3F3, 1011, 0, 0xC0Cu, 3, F, 3
#define PPUTLIMPL_UHEX_0x3F2u 1010u, 0x3F2, 1010, 0, 0xC0Du, 3, F, 2
#define PPUTLIMPL_UHEX_0x3F1u 1009u, 0x3F1, 1009, 0, 0xC0Eu, 3, F, 1
#define PPUTLIMPL_UHEX_0x3F0u 1008u, 0x3F0, 1008, 0, 0xC0Fu, 3, F, 0
#define PPUTLIMPL_UHEX_0x3EFu 1007u, 0x3EF, 1007, 0, 0xC10u, 3, E, F
#define PPUTLIMPL_UHEX_0x3EEu 1006u, 0x3EE, 1006, 0, 0xC11u, 3, E, E
#define PPUTLIMPL_UHEX_0x3EDu 1005u, 0x3ED, 1005, 0, 0xC12u, 3, E, D
#define PPUTLIMPL_UHEX_0x3ECu 1004u, 0x3EC, 1004, 0, 0xC13u, 3, E, C
#define PPUTLIMPL_UHEX_0x3EBu 1003u, 0x3EB, 1003, 0, 0xC14u, 3, E, B
#define PPUTLIMPL_UHEX_0x3EAu 1002u, 0x3EA, 1002, 0, 0xC15u, 3, E, A
#define PPUTLIMPL_UHEX_0x3E9u 1001u, 0x3E9, 1001, 0, 0xC16u, 3, E, 9
#define PPUTLIMPL_UHEX_0x3E8u 1000u, 0x3E8, 1000, 0, 0xC17u, 3, E, 8
#define PPUTLIMPL_UHEX_0x3E7u 999u, 0x3E7, 999, 0, 0xC18u, 3, E, 7
#define PPUTLIMPL_UHEX_0x3E6u 998u, 0x3E6, 998, 0, 0xC19u, 3, E, 6
#define PPUTLIMPL_UHEX_0x3E5u 997u, 0x3E5, 997, 0, 0xC1Au, 3, E, 5
#define PPUTLIMPL_UHEX_0x3E4u 996u, 0x3E4, 996, 0, 0xC1Bu, 3, E, 4
#define PPUTLIMPL_UHEX_0x3E3u 995u, 0x3E3, 995, 0, 0xC1Cu, 3, E, 3
#define PPUTLIMPL_UHEX_0x3E2u 994u, 0x3E2, 994, 0, 0xC1Du, 3, E, 2
#define PPUTLIMPL_UHEX_0x3E1u 993u, 0x3E1, 993, 0, 0xC1Eu, 3, E, 1
#define PPUTLIMPL_UHEX_0x3E0u 992u, 0x3E0, 992, 0, 0xC1Fu, 3, E, 0
#define PPUTLIMPL_UHEX_0x3DFu 991u, 0x3DF, 991, 0, 0xC20u, 3, D, F
#define PPUTLIMPL_UHEX_0x3DEu 990u, 0x3DE, 990, 0, 0xC21u, 3, D, E
#define PPUTLIMPL_UHEX_0x3DDu 989u, 0x3DD, 989, 0, 0xC22u, 3, D, D
#define PPUTLIMPL_UHEX_0x3DCu 988u, 0x3DC, 988, 0, 0xC23u, 3, D, C
#define PPUTLIMPL_UHEX_0x3DBu 987u, 0x3DB, 987, 0, 0xC24u, 3, D, B
#define PPUTLIMPL_UHEX_0x3DAu 986u, 0x3DA, 986, 0, 0xC25u, 3, D, A
#define PPUTLIMPL_UHEX_0x3D9u 985u, 0x3D9, 985, 0, 0xC26u, 3, D, 9
#define PPUTLIMPL_UHEX_0x3D8u 984u, 0x3D8, 984, 0, 0xC27u, 3, D, 8
#define PPUTLIMPL_UHEX_0x3D7u 983u, 0x3D7, 983, 0, 0xC28u, 3, D, 7
#define PPUTLIMPL_UHEX_0x3D6u 982u, 0x3D6, 982, 0, 0xC29u, 3, D, 6
#define PPUTLIMPL_UHEX_0x3D5u 981u, 0x3D5, 981, 0, 0xC2Au, 3, D, 5
#define PPUTLIMPL_UHEX_0x3D4u 980u, 0x3D4, 980, 0, 0xC2Bu, 3, D, 4
#define PPUTLIMPL_UHEX_0x3D3u 979u, 0x3D3, 979, 0, 0xC2Cu, 3, D, 3
#define PPUTLIMPL_UHEX_0x3D2u 978u, 0x3D2, 978, 0, 0xC2Du, 3, D, 2
#define PPUTLIMPL_UHEX_0x3D1u 977u, 0x3D1, 977, 0, 0xC2Eu, 3, D, 1
#define PPUTLIMPL_UHEX_0x3D0u 976u, 0x3D0, 976, 0, 0xC2Fu, 3, D, 0
#define PPUTLIMPL_UHEX_0x3CFu 975u, 0x3CF, 975, 0, 0xC30u, 3, C, F
#define PPUTLIMPL_UHEX_0x3CEu 974u, 0x3CE, 974, 0, 0xC31u, 3, C, E
#define PPUTLIMPL_UHEX_0x3CDu 973u, 0x3CD, 973, 0, 0xC32u, 3, C, D
#define PPUTLIMPL_UHEX_0x3CCu 972u, 0x3CC, 972, 0, 0xC33u, 3, C, C
#define PPUTLIMPL_UHEX_0x3CBu 971u, 0x3CB, 971, 0, 0xC34u, 3, C, B
#define PPUTLIMPL_UHEX_0x3CAu 970u, 0x3CA, 970, 0, 0xC35u, 3, C, A
#define PPUTLIMPL_UHEX_0x3C9u 969u, 0x3C9, 969, 0, 0xC36u, 3, C, 9
#define PPUTLIMPL_UHEX_0x3C8u 968u, 0x3C8, 968, 0, 0xC37u, 3, C, 8
#define PPUTLIMPL_UHEX_0x3C7u 967u, 0x3C7, 967, 0, 0xC38u, 3, C, 7
#define PPUTLIMPL_UHEX_0x3C6u 966u, 0x3C6, 966, 0, 0xC39u, 3, C, 6
#define PPUTLIMPL_UHEX_0x3C5u 965u, 0x3C5, 965, 0, 0xC3Au, 3, C, 5
#define PPUTLIMPL_UHEX_0x3C4u 964u, 0x3C4, 964, 0, 0xC3Bu, 3, C, 4
#define PPUTLIMPL_UHEX_0x3C3u 963u, 0x3C3, 963, 0, 0xC3Cu, 3, C, 3
#define PPUTLIMPL_UHEX_0x3C2u 962u, 0x3C2, 962, 0, 0xC3Du, 3, C, 2
#define PPUTLIMPL_UHEX_0x3C1u 961u, 0x3C1, 961, 0, 0xC3Eu, 3, C, 1
#define PPUTLIMPL_UHEX_0x3C0u 960u, 0x3C0, 960, 0, 0xC3Fu, 3, C, 0
#define PPUTLIMPL_UHEX_0x3BFu 959u, 0x3BF, 959, 0, 0xC40u, 3, B, F
#define PPUTLIMPL_UHEX_0x3BEu 958u, 0x3BE, 958, 0, 0xC41u, 3, B, E
#define PPUTLIMPL_UHEX_0x3BDu 957u, 0x3BD, 957, 0, 0xC42u, 3, B, D
#define PPUTLIMPL_UHEX_0x3BCu 956u, 0x3BC, 956, 0, 0xC43u, 3, B, C
#define PPUTLIMPL_UHEX_0x3BBu 955u, 0x3BB, 955, 0, 0xC44u, 3, B, B
#define PPUTLIMPL_UHEX_0x3BAu 954u, 0x3BA, 954, 0, 0xC45u, 3, B, A
#define PPUTLIMPL_UHEX_0x3B9u 953u, 0x3B9, 953, 0, 0xC46u, 3, B, 9
#define PPUTLIMPL_UHEX_0x3B8u 952u, 0x3B8, 952, 0, 0xC47u, 3, B, 8
#define PPUTLIMPL_UHEX_0x3B7u 951u, 0x3B7, 951, 0, 0xC48u, 3, B, 7
#define PPUTLIMPL_UHEX_0x3B6u 950u, 0x3B6, 950, 0, 0xC49u, 3, B, 6
#define PPUTLIMPL_UHEX_0x3B5u 949u, 0x3B5, 949, 0, 0xC4Au, 3, B, 5
#define PPUTLIMPL_UHEX_0x3B4u 948u, 0x3B4, 948, 0, 0xC4Bu, 3, B, 4
#define PPUTLIMPL_UHEX_0x3B3u 947u, 0x3B3, 947, 0, 0xC4Cu, 3, B, 3
#define PPUTLIMPL_UHEX_0x3B2u 946u, 0x3B2, 946, 0, 0xC4Du, 3, B, 2
#define PPUTLIMPL_UHEX_0x3B1u 945u, 0x3B1, 945, 0, 0xC4Eu, 3, B, 1
#define PPUTLIMPL_UHEX_0x3B0u 944u, 0x3B0, 944, 0, 0xC4Fu, 3, B, 0
#define PPUTLIMPL_UHEX_0x3AFu 943u, 0x3AF, 943, 0, 0xC50u, 3, A, F
#define PPUTLIMPL_UHEX_0x3AEu 942u, 0x3AE, 942, 0, 0xC51u, 3, A, E
#define PPUTLIMPL_UHEX_0x3ADu 941u, 0x3AD, 941, 0, 0xC52u, 3, A, D
#define PPUTLIMPL_UHEX_0x3ACu 940u, 0x3AC, 940, 0, 0xC53u, 3, A, C
#define PPUTLIMPL_UHEX_0x3ABu 939u, 0x3AB, 939, 0, 0xC54u, 3, A, B
#define PPUTLIMPL_UHEX_0x3AAu 938u, 0x3AA, 938, 0, 0xC55u, 3, A, A
#define PPUTLIMPL_UHEX_0x3A9u 937u, 0x3A9, 937, 0, 0xC56u, 3, A, 9
#define PPUTLIMPL_UHEX_0x3A8u 936u, 0x3A8, 936, 0, 0xC57u, 3, A, 8
#define PPUTLIMPL_UHEX_0x3A7u 935u, 0x3A7, 935, 0, 0xC58u, 3, A, 7
#define PPUTLIMPL_UHEX_0x3A6u 934u, 0x3A6, 934, 0, 0xC59u, 3, A, 6
#define PPUTLIMPL_UHEX_0x3A5u 933u, 0x3A5, 933, 0, 0xC5Au, 3, A, 5
#define PPUTLIMPL_UHEX_0x3A4u 932u, 0x3A4, 932, 0, 0xC5Bu, 3, A, 4
#define PPUTLIMPL_UHEX_0x3A3u 931u, 0x3A3, 931, 0, 0xC5Cu, 3, A, 3
#define PPUTLIMPL_UHEX_0x3A2u 930u, 0x3A2, 930, 0, 0xC5Du, 3, A, 2
#define PPUTLIMPL_UHEX_0x3A1u 929u, 0x3A1, 929, 0, 0xC5Eu, 3, A, 1
#define PPUTLIMPL_UHEX_0x3A0u 928u, 0x3A0, 928, 0, 0xC5Fu, 3, A, 0
#define PPUTLIMPL_UHEX_0x39Fu 927u, 0x39F, 927, 0, 0xC60u, 3, 9, F
#define PPUTLIMPL_UHEX_0x39Eu 926u, 0x39E, 926, 0, 0xC61u, 3, 9, E
#define PPUTLIMPL_UHEX_0x39Du 925u, 0x39D, 925, 0, 0xC62u, 3, 9, D
#define PPUTLIMPL_UHEX_0x39Cu 924u, 0x39C, 924, 0, 0xC63u, 3, 9, C
#define PPUTLIMPL_UHEX_0x39Bu 923u, 0x39B, 923, 0, 0xC64u, 3, 9, B
#define PPUTLIMPL_UHEX_0x39Au 922u, 0x39A, 922, 0, 0xC65u, 3, 9, A
#define PPUTLIMPL_UHEX_0x399u 921u, 0x399, 921, 0, 0xC66u, 3, 9, 9
#define PPUTLIMPL_UHEX_0x398u 920u, 0x398, 920, 0, 0xC67u, 3, 9, 8
#define PPUTLIMPL_UHEX_0x397u 919u, 0x397, 919, 0, 0xC68u, 3, 9, 7
#define PPUTLIMPL_UHEX_0x396u 918u, 0x396, 918, 0, 0xC69u, 3, 9, 6
#define PPUTLIMPL_UHEX_0x395u 917u, 0x395, 917, 0, 0xC6Au, 3, 9, 5
#define PPUTLIMPL_UHEX_0x394u 916u, 0x394, 916, 0, 0xC6Bu, 3, 9, 4
#define PPUTLIMPL_UHEX_0x393u 915u, 0x393, 915, 0, 0xC6Cu, 3, 9, 3
#define PPUTLIMPL_UHEX_0x392u 914u, 0x392, 914, 0, 0xC6Du, 3, 9, 2
#define PPUTLIMPL_UHEX_0x391u 913u, 0x391, 913, 0, 0xC6Eu, 3, 9, 1
#define PPUTLIMPL_UHEX_0x390u 912u, 0x390, 912, 0, 0xC6Fu, 3, 9, 0
#define PPUTLIMPL_UHEX_0x38Fu 911u, 0x38F, 911, 0, 0xC70u, 3, 8, F
#define PPUTLIMPL_UHEX_0x38Eu 910u, 0x38E, 910, 0, 0xC71u, 3, 8, E
#define PPUTLIMPL_UHEX_0x38Du 909u, 0x38D, 909, 0, 0xC72u, 3, 8, D
#define PPUTLIMPL_UHEX_0x38Cu 908u, 0x38C, 908, 0, 0xC73u, 3, 8, C
#define PPUTLIMPL_UHEX_0x38Bu 907u, 0x38B, 907, 0, 0xC74u, 3, 8, B
#define PPUTLIMPL_UHEX_0x38Au 906u, 0x38A, 906, 0, 0xC75u, 3, 8, A
#define PPUTLIMPL_UHEX_0x389u 905u, 0x389, 905, 0, 0xC76u, 3, 8, 9
#define PPUTLIMPL_UHEX_0x388u 904u, 0x388, 904, 0, 0xC77u, 3, 8, 8
#define PPUTLIMPL_UHEX_0x387u 903u, 0x387, 903, 0, 0xC78u, 3, 8, 7
#define PPUTLIMPL_UHEX_0x386u 902u, 0x386, 902, 0, 0xC79u, 3, 8, 6
#define PPUTLIMPL_UHEX_0x385u 901u, 0x385, 901, 0, 0xC7Au, 3, 8, 5
#define PPUTLIMPL_UHEX_0x384u 900u, 0x384, 900, 0, 0xC7Bu, 3, 8, 4
#define PPUTLIMPL_UHEX_0x383u 899u, 0x383, 899, 0, 0xC7Cu, 3, 8, 3
#define PPUTLIMPL_UHEX_0x382u 898u, 0x382, 898, 0, 0xC7Du, 3, 8, 2
#define PPUTLIMPL_UHEX_0x381u 897u, 0x381, 897, 0, 0xC7Eu, 3, 8, 1
#define PPUTLIMPL_UHEX_0x380u 896u, 0x380, 896, 0, 0xC7Fu, 3, 8, 0
#define PPUTLIMPL_UHEX_0x37Fu 895u, 0x37F, 895, 0, 0xC80u, 3, 7, F
#define PPUTLIMPL_UHEX_0x37Eu 894u, 0x37E, 894, 0, 0xC81u, 3, 7, E
#define PPUTLIMPL_UHEX_0x37Du 893u, 0x37D, 893, 0, 0xC82u, 3, 7, D
#define PPUTLIMPL_UHEX_0x37Cu 892u, 0x37C, 892, 0, 0xC83u, 3, 7, C
#define PPUTLIMPL_UHEX_0x37Bu 891u, 0x37B, 891, 0, 0xC84u, 3, 7, B
#define PPUTLIMPL_UHEX_0x37Au 890u, 0x37A, 890, 0, 0xC85u, 3, 7, A
#define PPUTLIMPL_UHEX_0x379u 889u, 0x379, 889, 0, 0xC86u, 3, 7, 9
#define PPUTLIMPL_UHEX_0x378u 888u, 0x378, 888, 0, 0xC87u, 3, 7, 8
#define PPUTLIMPL_UHEX_0x377u 887u, 0x377, 887, 0, 0xC88u, 3, 7, 7
#define PPUTLIMPL_UHEX_0x376u 886u, 0x376, 886, 0, 0xC89u, 3, 7, 6
#define PPUTLIMPL_UHEX_0x375u 885u, 0x375, 885, 0, 0xC8Au, 3, 7, 5
#define PPUTLIMPL_UHEX_0x374u 884u, 0x374, 884, 0, 0xC8Bu, 3, 7, 4
#define PPUTLIMPL_UHEX_0x373u 883u, 0x373, 883, 0, 0xC8Cu, 3, 7, 3
#define PPUTLIMPL_UHEX_0x372u 882u, 0x372, 882, 0, 0xC8Du, 3, 7, 2
#define PPUTLIMPL_UHEX_0x371u 881u, 0x371, 881, 0, 0xC8Eu, 3, 7, 1
#define PPUTLIMPL_UHEX_0x370u 880u, 0x370, 880, 0, 0xC8Fu, 3, 7, 0
#define PPUTLIMPL_UHEX_0x36Fu 879u, 0x36F, 879, 0, 0xC90u, 3, 6, F
#define PPUTLIMPL_UHEX_0x36Eu 878u, 0x36E, 878, 0, 0xC91u, 3, 6, E
#define PPUTLIMPL_UHEX_0x36Du 877u, 0x36D, 877, 0, 0xC92u, 3, 6, D
#define PPUTLIMPL_UHEX_0x36Cu 876u, 0x36C, 876, 0, 0xC93u, 3, 6, C
#define PPUTLIMPL_UHEX_0x36Bu 875u, 0x36B, 875, 0, 0xC94u, 3, 6, B
#define PPUTLIMPL_UHEX_0x36Au 874u, 0x36A, 874, 0, 0xC95u, 3, 6, A
#define PPUTLIMPL_UHEX_0x369u 873u, 0x369, 873, 0, 0xC96u, 3, 6, 9
#define PPUTLIMPL_UHEX_0x368u 872u, 0x368, 872, 0, 0xC97u, 3, 6, 8
#define PPUTLIMPL_UHEX_0x367u 871u, 0x367, 871, 0, 0xC98u, 3, 6, 7
#define PPUTLIMPL_UHEX_0x366u 870u, 0x366, 870, 0, 0xC99u, 3, 6, 6
#define PPUTLIMPL_UHEX_0x365u 869u, 0x365, 869, 0, 0xC9Au, 3, 6, 5
#define PPUTLIMPL_UHEX_0x364u 868u, 0x364, 868, 0, 0xC9Bu, 3, 6, 4
#define PPUTLIMPL_UHEX_0x363u 867u, 0x363, 867, 0, 0xC9Cu, 3, 6, 3
#define PPUTLIMPL_UHEX_0x362u 866u, 0x362, 866, 0, 0xC9Du, 3, 6, 2
#define PPUTLIMPL_UHEX_0x361u 865u, 0x361, 865, 0, 0xC9Eu, 3, 6, 1
#define PPUTLIMPL_UHEX_0x360u 864u, 0x360, 864, 0, 0xC9Fu, 3, 6, 0
#define PPUTLIMPL_UHEX_0x35Fu 863u, 0x35F, 863, 0, 0xCA0u, 3, 5, F
#define PPUTLIMPL_UHEX_0x35Eu 862u, 0x35E, 862, 0, 0xCA1u, 3, 5, E
#define PPUTLIMPL_UHEX_0x35Du 861u, 0x35D, 861, 0, 0xCA2u, 3, 5, D
#define PPUTLIMPL_UHEX_0x35Cu 860u, 0x35C, 860, 0, 0xCA3u, 3, 5, C
#define PPUTLIMPL_UHEX_0x35Bu 859u, 0x35B, 859, 0, 0xCA4u, 3, 5, B
#define PPUTLIMPL_UHEX_0x35Au 858u, 0x35A, 858, 0, 0xCA5u, 3, 5, A
#define PPUTLIMPL_UHEX_0x359u 857u, 0x359, 857, 0, 0xCA6u, 3, 5, 9
#define PPUTLIMPL_UHEX_0x358u 856u, 0x358, 856, 0, 0xCA7u, 3, 5, 8
#define PPUTLIMPL_UHEX_0x357u 855u, 0x357, 855, 0, 0xCA8u, 3, 5, 7
#define PPUTLIMPL_UHEX_0x356u 854u, 0x356, 854, 0, 0xCA9u, 3, 5, 6
#define PPUTLIMPL_UHEX_0x355u 853u, 0x355, 853, 0, 0xCAAu, 3, 5, 5
#define PPUTLIMPL_UHEX_0x354u 852u, 0x354, 852, 0, 0xCABu, 3, 5, 4
#define PPUTLIMPL_UHEX_0x353u 851u, 0x353, 851, 0, 0xCACu, 3, 5, 3
#define PPUTLIMPL_UHEX_0x352u 850u, 0x352, 850, 0, 0xCADu, 3, 5, 2
#define PPUTLIMPL_UHEX_0x351u 849u, 0x351, 849, 0, 0xCAEu, 3, 5, 1
#define PPUTLIMPL_UHEX_0x350u 848u, 0x350, 848, 0, 0xCAFu, 3, 5, 0
#define PPUTLIMPL_UHEX_0x34Fu 847u, 0x34F, 847, 0, 0xCB0u, 3, 4, F
#define PPUTLIMPL_UHEX_0x34Eu 846u, 0x34E, 846, 0, 0xCB1u, 3, 4, E
#define PPUTLIMPL_UHEX_0x34Du 845u, 0x34D, 845, 0, 0xCB2u, 3, 4, D
#define PPUTLIMPL_UHEX_0x34Cu 844u, 0x34C, 844, 0, 0xCB3u, 3, 4, C
#define PPUTLIMPL_UHEX_0x34Bu 843u, 0x34B, 843, 0, 0xCB4u, 3, 4, B
#define PPUTLIMPL_UHEX_0x34Au 842u, 0x34A, 842, 0, 0xCB5u, 3, 4, A
#define PPUTLIMPL_UHEX_0x349u 841u, 0x349, 841, 0, 0xCB6u, 3, 4, 9
#define PPUTLIMPL_UHEX_0x348u 840u, 0x348, 840, 0, 0xCB7u, 3, 4, 8
#define PPUTLIMPL_UHEX_0x347u 839u, 0x347, 839, 0, 0xCB8u, 3, 4, 7
#define PPUTLIMPL_UHEX_0x346u 838u, 0x346, 838, 0, 0xCB9u, 3, 4, 6
#define PPUTLIMPL_UHEX_0x345u 837u, 0x345, 837, 0, 0xCBAu, 3, 4, 5
#define PPUTLIMPL_UHEX_0x344u 836u, 0x344, 836, 0, 0xCBBu, 3, 4, 4
#define PPUTLIMPL_UHEX_0x343u 835u, 0x343, 835, 0, 0xCBCu, 3, 4, 3
#define PPUTLIMPL_UHEX_0x342u 834u, 0x342, 834, 0, 0xCBDu, 3, 4, 2
#define PPUTLIMPL_UHEX_0x341u 833u, 0x341, 833, 0, 0xCBEu, 3, 4, 1
#define PPUTLIMPL_UHEX_0x340u 832u, 0x340, 832, 0, 0xCBFu, 3, 4, 0
#define PPUTLIMPL_UHEX_0x33Fu 831u, 0x33F, 831, 0, 0xCC0u, 3, 3, F
#define PPUTLIMPL_UHEX_0x33Eu 830u, 0x33E, 830, 0, 0xCC1u, 3, 3, E
#define PPUTLIMPL_UHEX_0x33Du 829u, 0x33D, 829, 0, 0xCC2u, 3, 3, D
#define PPUTLIMPL_UHEX_0x33Cu 828u, 0x33C, 828, 0, 0xCC3u, 3, 3, C
#define PPUTLIMPL_UHEX_0x33Bu 827u, 0x33B, 827, 0, 0xCC4u, 3, 3, B
#define PPUTLIMPL_UHEX_0x33Au 826u, 0x33A, 826, 0, 0xCC5u, 3, 3, A
#define PPUTLIMPL_UHEX_0x339u 825u, 0x339, 825, 0, 0xCC6u, 3, 3, 9
#define PPUTLIMPL_UHEX_0x338u 824u, 0x338, 824, 0, 0xCC7u, 3, 3, 8
#define PPUTLIMPL_UHEX_0x337u 823u, 0x337, 823, 0, 0xCC8u, 3, 3, 7
#define PPUTLIMPL_UHEX_0x336u 822u, 0x336, 822, 0, 0xCC9u, 3, 3, 6
#define PPUTLIMPL_UHEX_0x335u 821u, 0x335, 821, 0, 0xCCAu, 3, 3, 5
#define PPUTLIMPL_UHEX_0x334u 820u, 0x334, 820, 0, 0xCCBu, 3, 3, 4
#define PPUTLIMPL_UHEX_0x333u 819u, 0x333, 819, 0, 0xCCCu, 3, 3, 3
#define PPUTLIMPL_UHEX_0x332u 818u, 0x332, 818, 0, 0xCCDu, 3, 3, 2
#define PPUTLIMPL_UHEX_0x331u 817u, 0x331, 817, 0, 0xCCEu, 3, 3, 1
#define PPUTLIMPL_UHEX_0x330u 816u, 0x330, 816, 0, 0xCCFu, 3, 3, 0
#define PPUTLIMPL_UHEX_0x32Fu 815u, 0x32F, 815, 0, 0xCD0u, 3, 2, F
#define PPUTLIMPL_UHEX_0x32Eu 814u, 0x32E, 814, 0, 0xCD1u, 3, 2, E
#define PPUTLIMPL_UHEX_0x32Du 813u, 0x32D, 813, 0, 0xCD2u, 3, 2, D
#define PPUTLIMPL_UHEX_0x32Cu 812u, 0x32C, 812, 0, 0xCD3u, 3, 2, C
#define PPUTLIMPL_UHEX_0x32Bu 811u, 0x32B, 811, 0, 0xCD4u, 3, 2, B
#define PPUTLIMPL_UHEX_0x32Au 810u, 0x32A, 810, 0, 0xCD5u, 3, 2, A
#define PPUTLIMPL_UHEX_0x329u 809u, 0x329, 809, 0, 0xCD6u, 3, 2, 9
#define PPUTLIMPL_UHEX_0x328u 808u, 0x328, 808, 0, 0xCD7u, 3, 2, 8
#define PPUTLIMPL_UHEX_0x327u 807u, 0x327, 807, 0, 0xCD8u, 3, 2, 7
#define PPUTLIMPL_UHEX_0x326u 806u, 0x326, 806, 0, 0xCD9u, 3, 2, 6
#define PPUTLIMPL_UHEX_0x325u 805u, 0x325, 805, 0, 0xCDAu, 3, 2, 5
#define PPUTLIMPL_UHEX_0x324u 804u, 0x324, 804, 0, 0xCDBu, 3, 2, 4
#define PPUTLIMPL_UHEX_0x323u 803u, 0x323, 803, 0, 0xCDCu, 3, 2, 3
#define PPUTLIMPL_UHEX_0x322u 802u, 0x322, 802, 0, 0xCDDu, 3, 2, 2
#define PPUTLIMPL_UHEX_0x321u 801u, 0x321, 801, 0, 0xCDEu, 3, 2, 1
#define PPUTLIMPL_UHEX_0x320u 800u, 0x320, 800, 0, 0xCDFu, 3, 2, 0
#define PPUTLIMPL_UHEX_0x31Fu 799u, 0x31F, 799, 0, 0xCE0u, 3, 1, F
#define PPUTLIMPL_UHEX_0x31Eu 798u, 0x31E, 798, 0, 0xCE1u, 3, 1, E
#define PPUTLIMPL_UHEX_0x31Du 797u, 0x31D, 797, 0, 0xCE2u, 3, 1, D
#define PPUTLIMPL_UHEX_0x31Cu 796u, 0x31C, 796, 0, 0xCE3u, 3, 1, C
#define PPUTLIMPL_UHEX_0x31Bu 795u, 0x31B, 795, 0, 0xCE4u, 3, 1, B
#define PPUTLIMPL_UHEX_0x31Au 794u, 0x31A, 794, 0, 0xCE5u, 3, 1, A
#define PPUTLIMPL_UHEX_0x319u 793u, 0x319, 793, 0, 0xCE6u, 3, 1, 9
#define PPUTLIMPL_UHEX_0x318u 792u, 0x318, 792, 0, 0xCE7u, 3, 1, 8
#define PPUTLIMPL_UHEX_0x317u 791u, 0x317, 791, 0, 0xCE8u, 3, 1, 7
#define PPUTLIMPL_UHEX_0x316u 790u, 0x316, 790, 0, 0xCE9u, 3, 1, 6
#define PPUTLIMPL_UHEX_0x315u 789u, 0x315, 789, 0, 0xCEAu, 3, 1, 5
#define PPUTLIMPL_UHEX_0x314u 788u, 0x314, 788, 0, 0xCEBu, 3, 1, 4
#define PPUTLIMPL_UHEX_0x313u 787u, 0x313, 787, 0, 0xCECu, 3, 1, 3
#define PPUTLIMPL_UHEX_0x312u 786u, 0x312, 786, 0, 0xCEDu, 3, 1, 2
#define PPUTLIMPL_UHEX_0x311u 785u, 0x311, 785, 0, 0xCEEu, 3, 1, 1
#define PPUTLIMPL_UHEX_0x310u 784u, 0x310, 784, 0, 0xCEFu, 3, 1, 0
#define PPUTLIMPL_UHEX_0x30Fu 783u, 0x30F, 783, 0, 0xCF0u, 3, 0, F
#define PPUTLIMPL_UHEX_0x30Eu 782u, 0x30E, 782, 0, 0xCF1u, 3, 0, E
#define PPUTLIMPL_UHEX_0x30Du 781u, 0x30D, 781, 0, 0xCF2u, 3, 0, D
#define PPUTLIMPL_UHEX_0x30Cu 780u, 0x30C, 780, 0, 0xCF3u, 3, 0, C
#define PPUTLIMPL_UHEX_0x30Bu 779u, 0x30B, 779, 0, 0xCF4u, 3, 0, B
#define PPUTLIMPL_UHEX_0x30Au 778u, 0x30A, 778, 0, 0xCF5u, 3, 0, A
#define PPUTLIMPL_UHEX_0x309u 777u, 0x309, 777, 0, 0xCF6u, 3, 0, 9
#define PPUTLIMPL_UHEX_0x308u 776u, 0x308, 776, 0, 0xCF7u, 3, 0, 8
#define PPUTLIMPL_UHEX_0x307u 775u, 0x307, 775, 0, 0xCF8u, 3, 0, 7
#define PPUTLIMPL_UHEX_0x306u 774u, 0x306, 774, 0, 0xCF9u, 3, 0, 6
#define PPUTLIMPL_UHEX_0x305u 773u, 0x305, 773, 0, 0xCFAu, 3, 0, 5
#define PPUTLIMPL_UHEX_0x304u 772u, 0x304, 772, 0, 0xCFBu, 3, 0, 4
#define PPUTLIMPL_UHEX_0x303u 771u, 0x303, 771, 0, 0xCFCu, 3, 0, 3
#define PPUTLIMPL_UHEX_0x302u 770u, 0x302, 770, 0, 0xCFDu, 3, 0, 2
#define PPUTLIMPL_UHEX_0x301u 769u, 0x301, 769, 0, 0xCFEu, 3, 0, 1
#define PPUTLIMPL_UHEX_0x300u 768u, 0x300, 768, 0, 0xCFFu, 3, 0, 0
#define PPUTLIMPL_UHEX_0x2FFu 767u, 0x2FF, 767, 0, 0xD00u, 2, F, F
#define PPUTLIMPL_UHEX_0x2FEu 766u, 0x2FE, 766, 0, 0xD01u, 2, F, E
#define PPUTLIMPL_UHEX_0x2FDu 765u, 0x2FD, 765, 0, 0xD02u, 2, F, D
#define PPUTLIMPL_UHEX_0x2FCu 764u, 0x2FC, 764, 0, 0xD03u, 2, F, C
#define PPUTLIMPL_UHEX_0x2FBu 763u, 0x2FB, 763, 0, 0xD04u, 2, F, B
#define PPUTLIMPL_UHEX_0x2FAu 762u, 0x2FA, 762, 0, 0xD05u, 2, F, A
#define PPUTLIMPL_UHEX_0x2F9u 761u, 0x2F9, 761, 0, 0xD06u, 2, F, 9
#define PPUTLIMPL_UHEX_0x2F8u 760u, 0x2F8, 760, 0, 0xD07u, 2, F, 8
#define PPUTLIMPL_UHEX_0x2F7u 759u, 0x2F7, 759, 0, 0xD08u, 2, F, 7
#define PPUTLIMPL_UHEX_0x2F6u 758u, 0x2F6, 758, 0, 0xD09u, 2, F, 6
#define PPUTLIMPL_UHEX_0x2F5u 757u, 0x2F5, 757, 0, 0xD0Au, 2, F, 5
#define PPUTLIMPL_UHEX_0x2F4u 756u, 0x2F4, 756, 0, 0xD0Bu, 2, F, 4
#define PPUTLIMPL_UHEX_0x2F3u 755u, 0x2F3, 755, 0, 0xD0Cu, 2, F, 3
#define PPUTLIMPL_UHEX_0x2F2u 754u, 0x2F2, 754, 0, 0xD0Du, 2, F, 2
#define PPUTLIMPL_UHEX_0x2F1u 753u, 0x2F1, 753, 0, 0xD0Eu, 2, F, 1
#define PPUTLIMPL_UHEX_0x2F0u 752u, 0x2F0, 752, 0, 0xD0Fu, 2, F, 0
#define PPUTLIMPL_UHEX_0x2EFu 751u, 0x2EF, 751, 0, 0xD10u, 2, E, F
#define PPUTLIMPL_UHEX_0x2EEu 750u, 0x2EE, 750, 0, 0xD11u, 2, E, E
#define PPUTLIMPL_UHEX_0x2EDu 749u, 0x2ED, 749, 0, 0xD12u, 2, E, D
#define PPUTLIMPL_UHEX_0x2ECu 748u, 0x2EC, 748, 0, 0xD13u, 2, E, C
#define PPUTLIMPL_UHEX_0x2EBu 747u, 0x2EB, 747, 0, 0xD14u, 2, E, B
#define PPUTLIMPL_UHEX_0x2EAu 746u, 0x2EA, 746, 0, 0xD15u, 2, E, A
#define PPUTLIMPL_UHEX_0x2E9u 745u, 0x2E9, 745, 0, 0xD16u, 2, E, 9
#define PPUTLIMPL_UHEX_0x2E8u 744u, 0x2E8, 744, 0, 0xD17u, 2, E, 8
#define PPUTLIMPL_UHEX_0x2E7u 743u, 0x2E7, 743, 0, 0xD18u, 2, E, 7
#define PPUTLIMPL_UHEX_0x2E6u 742u, 0x2E6, 742, 0, 0xD19u, 2, E, 6
#define PPUTLIMPL_UHEX_0x2E5u 741u, 0x2E5, 741, 0, 0xD1Au, 2, E, 5
#define PPUTLIMPL_UHEX_0x2E4u 740u, 0x2E4, 740, 0, 0xD1Bu, 2, E, 4
#define PPUTLIMPL_UHEX_0x2E3u 739u, 0x2E3, 739, 0, 0xD1Cu, 2, E, 3
#define PPUTLIMPL_UHEX_0x2E2u 738u, 0x2E2, 738, 0, 0xD1Du, 2, E, 2
#define PPUTLIMPL_UHEX_0x2E1u 737u, 0x2E1, 737, 0, 0xD1Eu, 2, E, 1
#define PPUTLIMPL_UHEX_0x2E0u 736u, 0x2E0, 736, 0, 0xD1Fu, 2, E, 0
#define PPUTLIMPL_UHEX_0x2DFu 735u, 0x2DF, 735, 0, 0xD20u, 2, D, F
#define PPUTLIMPL_UHEX_0x2DEu 734u, 0x2DE, 734, 0, 0xD21u, 2, D, E
#define PPUTLIMPL_UHEX_0x2DDu 733u, 0x2DD, 733, 0, 0xD22u, 2, D, D
#define PPUTLIMPL_UHEX_0x2DCu 732u, 0x2DC, 732, 0, 0xD23u, 2, D, C
#define PPUTLIMPL_UHEX_0x2DBu 731u, 0x2DB, 731, 0, 0xD24u, 2, D, B
#define PPUTLIMPL_UHEX_0x2DAu 730u, 0x2DA, 730, 0, 0xD25u, 2, D, A
#define PPUTLIMPL_UHEX_0x2D9u 729u, 0x2D9, 729, 0, 0xD26u, 2, D, 9
#define PPUTLIMPL_UHEX_0x2D8u 728u, 0x2D8, 728, 0, 0xD27u, 2, D, 8
#define PPUTLIMPL_UHEX_0x2D7u 727u, 0x2D7, 727, 0, 0xD28u, 2, D, 7
#define PPUTLIMPL_UHEX_0x2D6u 726u, 0x2D6, 726, 0, 0xD29u, 2, D, 6
#define PPUTLIMPL_UHEX_0x2D5u 725u, 0x2D5, 725, 0, 0xD2Au, 2, D, 5
#define PPUTLIMPL_UHEX_0x2D4u 724u, 0x2D4, 724, 0, 0xD2Bu, 2, D, 4
#define PPUTLIMPL_UHEX_0x2D3u 723u, 0x2D3, 723, 0, 0xD2Cu, 2, D, 3
#define PPUTLIMPL_UHEX_0x2D2u 722u, 0x2D2, 722, 0, 0xD2Du, 2, D, 2
#define PPUTLIMPL_UHEX_0x2D1u 721u, 0x2D1, 721, 0, 0xD2Eu, 2, D, 1
#define PPUTLIMPL_UHEX_0x2D0u 720u, 0x2D0, 720, 0, 0xD2Fu, 2, D, 0
#define PPUTLIMPL_UHEX_0x2CFu 719u, 0x2CF, 719, 0, 0xD30u, 2, C, F
#define PPUTLIMPL_UHEX_0x2CEu 718u, 0x2CE, 718, 0, 0xD31u, 2, C, E
#define PPUTLIMPL_UHEX_0x2CDu 717u, 0x2CD, 717, 0, 0xD32u, 2, C, D
#define PPUTLIMPL_UHEX_0x2CCu 716u, 0x2CC, 716, 0, 0xD33u, 2, C, C
#define PPUTLIMPL_UHEX_0x2CBu 715u, 0x2CB, 715, 0, 0xD34u, 2, C, B
#define PPUTLIMPL_UHEX_0x2CAu 714u, 0x2CA, 714, 0, 0xD35u, 2, C, A
#define PPUTLIMPL_UHEX_0x2C9u 713u, 0x2C9, 713, 0, 0xD36u, 2, C, 9
#define PPUTLIMPL_UHEX_0x2C8u 712u, 0x2C8, 712, 0, 0xD37u, 2, C, 8
#define PPUTLIMPL_UHEX_0x2C7u 711u, 0x2C7, 711, 0, 0xD38u, 2, C, 7
#define PPUTLIMPL_UHEX_0x2C6u 710u, 0x2C6, 710, 0, 0xD39u, 2, C, 6
#define PPUTLIMPL_UHEX_0x2C5u 709u, 0x2C5, 709, 0, 0xD3Au, 2, C, 5
#define PPUTLIMPL_UHEX_0x2C4u 708u, 0x2C4, 708, 0, 0xD3Bu, 2, C, 4
#define PPUTLIMPL_UHEX_0x2C3u 707u, 0x2C3, 707, 0, 0xD3Cu, 2, C, 3
#define PPUTLIMPL_UHEX_0x2C2u 706u, 0x2C2, 706, 0, 0xD3Du, 2, C, 2
#define PPUTLIMPL_UHEX_0x2C1u 705u, 0x2C1, 705, 0, 0xD3Eu, 2, C, 1
#define PPUTLIMPL_UHEX_0x2C0u 704u, 0x2C0, 704, 0, 0xD3Fu, 2, C, 0
#define PPUTLIMPL_UHEX_0x2BFu 703u, 0x2BF, 703, 0, 0xD40u, 2, B, F
#define PPUTLIMPL_UHEX_0x2BEu 702u, 0x2BE, 702, 0, 0xD41u, 2, B, E
#define PPUTLIMPL_UHEX_0x2BDu 701u, 0x2BD, 701, 0, 0xD42u, 2, B, D
#define PPUTLIMPL_UHEX_0x2BCu 700u, 0x2BC, 700, 0, 0xD43u, 2, B, C
#define PPUTLIMPL_UHEX_0x2BBu 699u, 0x2BB, 699, 0, 0xD44u, 2, B, B
#define PPUTLIMPL_UHEX_0x2BAu 698u, 0x2BA, 698, 0, 0xD45u, 2, B, A
#define PPUTLIMPL_UHEX_0x2B9u 697u, 0x2B9, 697, 0, 0xD46u, 2, B, 9
#define PPUTLIMPL_UHEX_0x2B8u 696u, 0x2B8, 696, 0, 0xD47u, 2, B, 8
#define PPUTLIMPL_UHEX_0x2B7u 695u, 0x2B7, 695, 0, 0xD48u, 2, B, 7
#define PPUTLIMPL_UHEX_0x2B6u 694u, 0x2B6, 694, 0, 0xD49u, 2, B, 6
#define PPUTLIMPL_UHEX_0x2B5u 693u, 0x2B5, 693, 0, 0xD4Au, 2, B, 5
#define PPUTLIMPL_UHEX_0x2B4u 692u, 0x2B4, 692, 0, 0xD4Bu, 2, B, 4
#define PPUTLIMPL_UHEX_0x2B3u 691u, 0x2B3, 691, 0, 0xD4Cu, 2, B, 3
#define PPUTLIMPL_UHEX_0x2B2u 690u, 0x2B2, 690, 0, 0xD4Du, 2, B, 2
#define PPUTLIMPL_UHEX_0x2B1u 689u, 0x2B1, 689, 0, 0xD4Eu, 2, B, 1
#define PPUTLIMPL_UHEX_0x2B0u 688u, 0x2B0, 688, 0, 0xD4Fu, 2, B, 0
#define PPUTLIMPL_UHEX_0x2AFu 687u, 0x2AF, 687, 0, 0xD50u, 2, A, F
#define PPUTLIMPL_UHEX_0x2AEu 686u, 0x2AE, 686, 0, 0xD51u, 2, A, E
#define PPUTLIMPL_UHEX_0x2ADu 685u, 0x2AD, 685, 0, 0xD52u, 2, A, D
#define PPUTLIMPL_UHEX_0x2ACu 684u, 0x2AC, 684, 0, 0xD53u, 2, A, C
#define PPUTLIMPL_UHEX_0x2ABu 683u, 0x2AB, 683, 0, 0xD54u, 2, A, B
#define PPUTLIMPL_UHEX_0x2AAu 682u, 0x2AA, 682, 0, 0xD55u, 2, A, A
#define PPUTLIMPL_UHEX_0x2A9u 681u, 0x2A9, 681, 0, 0xD56u, 2, A, 9
#define PPUTLIMPL_UHEX_0x2A8u 680u, 0x2A8, 680, 0, 0xD57u, 2, A, 8
#define PPUTLIMPL_UHEX_0x2A7u 679u, 0x2A7, 679, 0, 0xD58u, 2, A, 7
#define PPUTLIMPL_UHEX_0x2A6u 678u, 0x2A6, 678, 0, 0xD59u, 2, A, 6
#define PPUTLIMPL_UHEX_0x2A5u 677u, 0x2A5, 677, 0, 0xD5Au, 2, A, 5
#define PPUTLIMPL_UHEX_0x2A4u 676u, 0x2A4, 676, 0, 0xD5Bu, 2, A, 4
#define PPUTLIMPL_UHEX_0x2A3u 675u, 0x2A3, 675, 0, 0xD5Cu, 2, A, 3
#define PPUTLIMPL_UHEX_0x2A2u 674u, 0x2A2, 674, 0, 0xD5Du, 2, A, 2
#define PPUTLIMPL_UHEX_0x2A1u 673u, 0x2A1, 673, 0, 0xD5Eu, 2, A, 1
#define PPUTLIMPL_UHEX_0x2A0u 672u, 0x2A0, 672, 0, 0xD5Fu, 2, A, 0
#define PPUTLIMPL_UHEX_0x29Fu 671u, 0x29F, 671, 0, 0xD60u, 2, 9, F
#define PPUTLIMPL_UHEX_0x29Eu 670u, 0x29E, 670, 0, 0xD61u, 2, 9, E
#define PPUTLIMPL_UHEX_0x29Du 669u, 0x29D, 669, 0, 0xD62u, 2, 9, D
#define PPUTLIMPL_UHEX_0x29Cu 668u, 0x29C, 668, 0, 0xD63u, 2, 9, C
#define PPUTLIMPL_UHEX_0x29Bu 667u, 0x29B, 667, 0, 0xD64u, 2, 9, B
#define PPUTLIMPL_UHEX_0x29Au 666u, 0x29A, 666, 0, 0xD65u, 2, 9, A
#define PPUTLIMPL_UHEX_0x299u 665u, 0x299, 665, 0, 0xD66u, 2, 9, 9
#define PPUTLIMPL_UHEX_0x298u 664u, 0x298, 664, 0, 0xD67u, 2, 9, 8
#define PPUTLIMPL_UHEX_0x297u 663u, 0x297, 663, 0, 0xD68u, 2, 9, 7
#define PPUTLIMPL_UHEX_0x296u 662u, 0x296, 662, 0, 0xD69u, 2, 9, 6
#define PPUTLIMPL_UHEX_0x295u 661u, 0x295, 661, 0, 0xD6Au, 2, 9, 5
#define PPUTLIMPL_UHEX_0x294u 660u, 0x294, 660, 0, 0xD6Bu, 2, 9, 4
#define PPUTLIMPL_UHEX_0x293u 659u, 0x293, 659, 0, 0xD6Cu, 2, 9, 3
#define PPUTLIMPL_UHEX_0x292u 658u, 0x292, 658, 0, 0xD6Du, 2, 9, 2
#define PPUTLIMPL_UHEX_0x291u 657u, 0x291, 657, 0, 0xD6Eu, 2, 9, 1
#define PPUTLIMPL_UHEX_0x290u 656u, 0x290, 656, 0, 0xD6Fu, 2, 9, 0
#define PPUTLIMPL_UHEX_0x28Fu 655u, 0x28F, 655, 0, 0xD70u, 2, 8, F
#define PPUTLIMPL_UHEX_0x28Eu 654u, 0x28E, 654, 0, 0xD71u, 2, 8, E
#define PPUTLIMPL_UHEX_0x28Du 653u, 0x28D, 653, 0, 0xD72u, 2, 8, D
#define PPUTLIMPL_UHEX_0x28Cu 652u, 0x28C, 652, 0, 0xD73u, 2, 8, C
#define PPUTLIMPL_UHEX_0x28Bu 651u, 0x28B, 651, 0, 0xD74u, 2, 8, B
#define PPUTLIMPL_UHEX_0x28Au 650u, 0x28A, 650, 0, 0xD75u, 2, 8, A
#define PPUTLIMPL_UHEX_0x289u 649u, 0x289, 649, 0, 0xD76u, 2, 8, 9
#define PPUTLIMPL_UHEX_0x288u 648u, 0x288, 648, 0, 0xD77u, 2, 8, 8
#define PPUTLIMPL_UHEX_0x287u 647u, 0x287, 647, 0, 0xD78u, 2, 8, 7
#define PPUTLIMPL_UHEX_0x286u 646u, 0x286, 646, 0, 0xD79u, 2, 8, 6
#define PPUTLIMPL_UHEX_0x285u 645u, 0x285, 645, 0, 0xD7Au, 2, 8, 5
#define PPUTLIMPL_UHEX_0x284u 644u, 0x284, 644, 0, 0xD7Bu, 2, 8, 4
#define PPUTLIMPL_UHEX_0x283u 643u, 0x283, 643, 0, 0xD7Cu, 2, 8, 3
#define PPUTLIMPL_UHEX_0x282u 642u, 0x282, 642, 0, 0xD7Du, 2, 8, 2
#define PPUTLIMPL_UHEX_0x281u 641u, 0x281, 641, 0, 0xD7Eu, 2, 8, 1
#define PPUTLIMPL_UHEX_0x280u 640u, 0x280, 640, 0, 0xD7Fu, 2, 8, 0
#define PPUTLIMPL_UHEX_0x27Fu 639u, 0x27F, 639, 0, 0xD80u, 2, 7, F
#define PPUTLIMPL_UHEX_0x27Eu 638u, 0x27E, 638, 0, 0xD81u, 2, 7, E
#define PPUTLIMPL_UHEX_0x27Du 637u, 0x27D, 637, 0, 0xD82u, 2, 7, D
#define PPUTLIMPL_UHEX_0x27Cu 636u, 0x27C, 636, 0, 0xD83u, 2, 7, C
#define PPUTLIMPL_UHEX_0x27Bu 635u, 0x27B, 635, 0, 0xD84u, 2, 7, B
#define PPUTLIMPL_UHEX_0x27Au 634u, 0x27A, 634, 0, 0xD85u, 2, 7, A
#define PPUTLIMPL_UHEX_0x279u 633u, 0x279, 633, 0, 0xD86u, 2, 7, 9
#define PPUTLIMPL_UHEX_0x278u 632u, 0x278, 632, 0, 0xD87u, 2, 7, 8
#define PPUTLIMPL_UHEX_0x277u 631u, 0x277, 631, 0, 0xD88u, 2, 7, 7
#define PPUTLIMPL_UHEX_0x276u 630u, 0x276, 630, 0, 0xD89u, 2, 7, 6
#define PPUTLIMPL_UHEX_0x275u 629u, 0x275, 629, 0, 0xD8Au, 2, 7, 5
#define PPUTLIMPL_UHEX_0x274u 628u, 0x274, 628, 0, 0xD8Bu, 2, 7, 4
#define PPUTLIMPL_UHEX_0x273u 627u, 0x273, 627, 0, 0xD8Cu, 2, 7, 3
#define PPUTLIMPL_UHEX_0x272u 626u, 0x272, 626, 0, 0xD8Du, 2, 7, 2
#define PPUTLIMPL_UHEX_0x271u 625u, 0x271, 625, 0, 0xD8Eu, 2, 7, 1
#define PPUTLIMPL_UHEX_0x270u 624u, 0x270, 624, 0, 0xD8Fu, 2, 7, 0
#define PPUTLIMPL_UHEX_0x26Fu 623u, 0x26F, 623, 0, 0xD90u, 2, 6, F
#define PPUTLIMPL_UHEX_0x26Eu 622u, 0x26E, 622, 0, 0xD91u, 2, 6, E
#define PPUTLIMPL_UHEX_0x26Du 621u, 0x26D, 621, 0, 0xD92u, 2, 6, D
#define PPUTLIMPL_UHEX_0x26Cu 620u, 0x26C, 620, 0, 0xD93u, 2, 6, C
#define PPUTLIMPL_UHEX_0x26Bu 619u, 0x26B, 619, 0, 0xD94u, 2, 6, B
#define PPUTLIMPL_UHEX_0x26Au 618u, 0x26A, 618, 0, 0xD95u, 2, 6, A
#define PPUTLIMPL_UHEX_0x269u 617u, 0x269, 617, 0, 0xD96u, 2, 6, 9
#define PPUTLIMPL_UHEX_0x268u 616u, 0x268, 616, 0, 0xD97u, 2, 6, 8
#define PPUTLIMPL_UHEX_0x267u 615u, 0x267, 615, 0, 0xD98u, 2, 6, 7
#define PPUTLIMPL_UHEX_0x266u 614u, 0x266, 614, 0, 0xD99u, 2, 6, 6
#define PPUTLIMPL_UHEX_0x265u 613u, 0x265, 613, 0, 0xD9Au, 2, 6, 5
#define PPUTLIMPL_UHEX_0x264u 612u, 0x264, 612, 0, 0xD9Bu, 2, 6, 4
#define PPUTLIMPL_UHEX_0x263u 611u, 0x263, 611, 0, 0xD9Cu, 2, 6, 3
#define PPUTLIMPL_UHEX_0x262u 610u, 0x262, 610, 0, 0xD9Du, 2, 6, 2
#define PPUTLIMPL_UHEX_0x261u 609u, 0x261, 609, 0, 0xD9Eu, 2, 6, 1
#define PPUTLIMPL_UHEX_0x260u 608u, 0x260, 608, 0, 0xD9Fu, 2, 6, 0
#define PPUTLIMPL_UHEX_0x25Fu 607u, 0x25F, 607, 0, 0xDA0u, 2, 5, F
#define PPUTLIMPL_UHEX_0x25Eu 606u, 0x25E, 606, 0, 0xDA1u, 2, 5, E
#define PPUTLIMPL_UHEX_0x25Du 605u, 0x25D, 605, 0, 0xDA2u, 2, 5, D
#define PPUTLIMPL_UHEX_0x25Cu 604u, 0x25C, 604, 0, 0xDA3u, 2, 5, C
#define PPUTLIMPL_UHEX_0x25Bu 603u, 0x25B, 603, 0, 0xDA4u, 2, 5, B
#define PPUTLIMPL_UHEX_0x25Au 602u, 0x25A, 602, 0, 0xDA5u, 2, 5, A
#define PPUTLIMPL_UHEX_0x259u 601u, 0x259, 601, 0, 0xDA6u, 2, 5, 9
#define PPUTLIMPL_UHEX_0x258u 600u, 0x258, 600, 0, 0xDA7u, 2, 5, 8
#define PPUTLIMPL_UHEX_0x257u 599u, 0x257, 599, 0, 0xDA8u, 2, 5, 7
#define PPUTLIMPL_UHEX_0x256u 598u, 0x256, 598, 0, 0xDA9u, 2, 5, 6
#define PPUTLIMPL_UHEX_0x255u 597u, 0x255, 597, 0, 0xDAAu, 2, 5, 5
#define PPUTLIMPL_UHEX_0x254u 596u, 0x254, 596, 0, 0xDABu, 2, 5, 4
#define PPUTLIMPL_UHEX_0x253u 595u, 0x253, 595, 0, 0xDACu, 2, 5, 3
#define PPUTLIMPL_UHEX_0x252u 594u, 0x252, 594, 0, 0xDADu, 2, 5, 2
#define PPUTLIMPL_UHEX_0x251u 593u, 0x251, 593, 0, 0xDAEu, 2, 5, 1
#define PPUTLIMPL_UHEX_0x250u 592u, 0x250, 592, 0, 0xDAFu, 2, 5, 0
#define PPUTLIMPL_UHEX_0x24Fu 591u, 0x24F, 591, 0, 0xDB0u, 2, 4, F
#define PPUTLIMPL_UHEX_0x24Eu 590u, 0x24E, 590, 0, 0xDB1u, 2, 4, E
#define PPUTLIMPL_UHEX_0x24Du 589u, 0x24D, 589, 0, 0xDB2u, 2, 4, D
#define PPUTLIMPL_UHEX_0x24Cu 588u, 0x24C, 588, 0, 0xDB3u, 2, 4, C
#define PPUTLIMPL_UHEX_0x24Bu 587u, 0x24B, 587, 0, 0xDB4u, 2, 4, B
#define PPUTLIMPL_UHEX_0x24Au 586u, 0x24A, 586, 0, 0xDB5u, 2, 4, A
#define PPUTLIMPL_UHEX_0x249u 585u, 0x249, 585, 0, 0xDB6u, 2, 4, 9
#define PPUTLIMPL_UHEX_0x248u 584u, 0x248, 584, 0, 0xDB7u, 2, 4, 8
#define PPUTLIMPL_UHEX_0x247u 583u, 0x247, 583, 0, 0xDB8u, 2, 4, 7
#define PPUTLIMPL_UHEX_0x246u 582u, 0x246, 582, 0, 0xDB9u, 2, 4, 6
#define PPUTLIMPL_UHEX_0x245u 581u, 0x245, 581, 0, 0xDBAu, 2, 4, 5
#define PPUTLIMPL_UHEX_0x244u 580u, 0x244, 580, 0, 0xDBBu, 2, 4, 4
#define PPUTLIMPL_UHEX_0x243u 579u, 0x243, 579, 0, 0xDBCu, 2, 4, 3
#define PPUTLIMPL_UHEX_0x242u 578u, 0x242, 578, 0, 0xDBDu, 2, 4, 2
#define PPUTLIMPL_UHEX_0x241u 577u, 0x241, 577, 0, 0xDBEu, 2, 4, 1
#define PPUTLIMPL_UHEX_0x240u 576u, 0x240, 576, 0, 0xDBFu, 2, 4, 0
#define PPUTLIMPL_UHEX_0x23Fu 575u, 0x23F, 575, 0, 0xDC0u, 2, 3, F
#define PPUTLIMPL_UHEX_0x23Eu 574u, 0x23E, 574, 0, 0xDC1u, 2, 3, E
#define PPUTLIMPL_UHEX_0x23Du 573u, 0x23D, 573, 0, 0xDC2u, 2, 3, D
#define PPUTLIMPL_UHEX_0x23Cu 572u, 0x23C, 572, 0, 0xDC3u, 2, 3, C
#define PPUTLIMPL_UHEX_0x23Bu 571u, 0x23B, 571, 0, 0xDC4u, 2, 3, B
#define PPUTLIMPL_UHEX_0x23Au 570u, 0x23A, 570, 0, 0xDC5u, 2, 3, A
#define PPUTLIMPL_UHEX_0x239u 569u, 0x239, 569, 0, 0xDC6u, 2, 3, 9
#define PPUTLIMPL_UHEX_0x238u 568u, 0x238, 568, 0, 0xDC7u, 2, 3, 8
#define PPUTLIMPL_UHEX_0x237u 567u, 0x237, 567, 0, 0xDC8u, 2, 3, 7
#define PPUTLIMPL_UHEX_0x236u 566u, 0x236, 566, 0, 0xDC9u, 2, 3, 6
#define PPUTLIMPL_UHEX_0x235u 565u, 0x235, 565, 0, 0xDCAu, 2, 3, 5
#define PPUTLIMPL_UHEX_0x234u 564u, 0x234, 564, 0, 0xDCBu, 2, 3, 4
#define PPUTLIMPL_UHEX_0x233u 563u, 0x233, 563, 0, 0xDCCu, 2, 3, 3
#define PPUTLIMPL_UHEX_0x232u 562u, 0x232, 562, 0, 0xDCDu, 2, 3, 2
#define PPUTLIMPL_UHEX_0x231u 561u, 0x231, 561, 0, 0xDCEu, 2, 3, 1
#define PPUTLIMPL_UHEX_0x230u 560u, 0x230, 560, 0, 0xDCFu, 2, 3, 0
#define PPUTLIMPL_UHEX_0x22Fu 559u, 0x22F, 559, 0, 0xDD0u, 2, 2, F
#define PPUTLIMPL_UHEX_0x22Eu 558u, 0x22E, 558, 0, 0xDD1u, 2, 2, E
#define PPUTLIMPL_UHEX_0x22Du 557u, 0x22D, 557, 0, 0xDD2u, 2, 2, D
#define PPUTLIMPL_UHEX_0x22Cu 556u, 0x22C, 556, 0, 0xDD3u, 2, 2, C
#define PPUTLIMPL_UHEX_0x22Bu 555u, 0x22B, 555, 0, 0xDD4u, 2, 2, B
#define PPUTLIMPL_UHEX_0x22Au 554u, 0x22A, 554, 0, 0xDD5u, 2, 2, A
#define PPUTLIMPL_UHEX_0x229u 553u, 0x229, 553, 0, 0xDD6u, 2, 2, 9
#define PPUTLIMPL_UHEX_0x228u 552u, 0x228, 552, 0, 0xDD7u, 2, 2, 8
#define PPUTLIMPL_UHEX_0x227u 551u, 0x227, 551, 0, 0xDD8u, 2, 2, 7
#define PPUTLIMPL_UHEX_0x226u 550u, 0x226, 550, 0, 0xDD9u, 2, 2, 6
#define PPUTLIMPL_UHEX_0x225u 549u, 0x225, 549, 0, 0xDDAu, 2, 2, 5
#define PPUTLIMPL_UHEX_0x224u 548u, 0x224, 548, 0, 0xDDBu, 2, 2, 4
#define PPUTLIMPL_UHEX_0x223u 547u, 0x223, 547, 0, 0xDDCu, 2, 2, 3
#define PPUTLIMPL_UHEX_0x222u 546u, 0x222, 546, 0, 0xDDDu, 2, 2, 2
#define PPUTLIMPL_UHEX_0x221u 545u, 0x221, 545, 0, 0xDDEu, 2, 2, 1
#define PPUTLIMPL_UHEX_0x220u 544u, 0x220, 544, 0, 0xDDFu, 2, 2, 0
#define PPUTLIMPL_UHEX_0x21Fu 543u, 0x21F, 543, 0, 0xDE0u, 2, 1, F
#define PPUTLIMPL_UHEX_0x21Eu 542u, 0x21E, 542, 0, 0xDE1u, 2, 1, E
#define PPUTLIMPL_UHEX_0x21Du 541u, 0x21D, 541, 0, 0xDE2u, 2, 1, D
#define PPUTLIMPL_UHEX_0x21Cu 540u, 0x21C, 540, 0, 0xDE3u, 2, 1, C
#define PPUTLIMPL_UHEX_0x21Bu 539u, 0x21B, 539, 0, 0xDE4u, 2, 1, B
#define PPUTLIMPL_UHEX_0x21Au 538u, 0x21A, 538, 0, 0xDE5u, 2, 1, A
#define PPUTLIMPL_UHEX_0x219u 537u, 0x219, 537, 0, 0xDE6u, 2, 1, 9
#define PPUTLIMPL_UHEX_0x218u 536u, 0x218, 536, 0, 0xDE7u, 2, 1, 8
#define PPUTLIMPL_UHEX_0x217u 535u, 0x217, 535, 0, 0xDE8u, 2, 1, 7
#define PPUTLIMPL_UHEX_0x216u 534u, 0x216, 534, 0, 0xDE9u, 2, 1, 6
#define PPUTLIMPL_UHEX_0x215u 533u, 0x215, 533, 0, 0xDEAu, 2, 1, 5
#define PPUTLIMPL_UHEX_0x214u 532u, 0x214, 532, 0, 0xDEBu, 2, 1, 4
#define PPUTLIMPL_UHEX_0x213u 531u, 0x213, 531, 0, 0xDECu, 2, 1, 3
#define PPUTLIMPL_UHEX_0x212u 530u, 0x212, 530, 0, 0xDEDu, 2, 1, 2
#define PPUTLIMPL_UHEX_0x211u 529u, 0x211, 529, 0, 0xDEEu, 2, 1, 1
#define PPUTLIMPL_UHEX_0x210u 528u, 0x210, 528, 0, 0xDEFu, 2, 1, 0
#define PPUTLIMPL_UHEX_0x20Fu 527u, 0x20F, 527, 0, 0xDF0u, 2, 0, F
#define PPUTLIMPL_UHEX_0x20Eu 526u, 0x20E, 526, 0, 0xDF1u, 2, 0, E
#define PPUTLIMPL_UHEX_0x20Du 525u, 0x20D, 525, 0, 0xDF2u, 2, 0, D
#define PPUTLIMPL_UHEX_0x20Cu 524u, 0x20C, 524, 0, 0xDF3u, 2, 0, C
#define PPUTLIMPL_UHEX_0x20Bu 523u, 0x20B, 523, 0, 0xDF4u, 2, 0, B
#define PPUTLIMPL_UHEX_0x20Au 522u, 0x20A, 522, 0, 0xDF5u, 2, 0, A
#define PPUTLIMPL_UHEX_0x209u 521u, 0x209, 521, 0, 0xDF6u, 2, 0, 9
#define PPUTLIMPL_UHEX_0x208u 520u, 0x208, 520, 0, 0xDF7u, 2, 0, 8
#define PPUTLIMPL_UHEX_0x207u 519u, 0x207, 519, 0, 0xDF8u, 2, 0, 7
#define PPUTLIMPL_UHEX_0x206u 518u, 0x206, 518, 0, 0xDF9u, 2, 0, 6
#define PPUTLIMPL_UHEX_0x205u 517u, 0x205, 517, 0, 0xDFAu, 2, 0, 5
#define PPUTLIMPL_UHEX_0x204u 516u, 0x204, 516, 0, 0xDFBu, 2, 0, 4
#define PPUTLIMPL_UHEX_0x203u 515u, 0x203, 515, 0, 0xDFCu, 2, 0, 3
#define PPUTLIMPL_UHEX_0x202u 514u, 0x202, 514, 0, 0xDFDu, 2, 0, 2
#define PPUTLIMPL_UHEX_0x201u 513u, 0x201, 513, 0, 0xDFEu, 2, 0, 1
#define PPUTLIMPL_UHEX_0x200u 512u, 0x200, 512, 0, 0xDFFu, 2, 0, 0
#define PPUTLIMPL_UHEX_0x1FFu 511u, 0x1FF, 511, 0, 0xE00u, 1, F, F
#define PPUTLIMPL_UHEX_0x1FEu 510u, 0x1FE, 510, 0, 0xE01u, 1, F, E
#define PPUTLIMPL_UHEX_0x1FDu 509u, 0x1FD, 509, 0, 0xE02u, 1, F, D
#define PPUTLIMPL_UHEX_0x1FCu 508u, 0x1FC, 508, 0, 0xE03u, 1, F, C
#define PPUTLIMPL_UHEX_0x1FBu 507u, 0x1FB, 507, 0, 0xE04u, 1, F, B
#define PPUTLIMPL_UHEX_0x1FAu 506u, 0x1FA, 506, 0, 0xE05u, 1, F, A
#define PPUTLIMPL_UHEX_0x1F9u 505u, 0x1F9, 505, 0, 0xE06u, 1, F, 9
#define PPUTLIMPL_UHEX_0x1F8u 504u, 0x1F8, 504, 0, 0xE07u, 1, F, 8
#define PPUTLIMPL_UHEX_0x1F7u 503u, 0x1F7, 503, 0, 0xE08u, 1, F, 7
#define PPUTLIMPL_UHEX_0x1F6u 502u, 0x1F6, 502, 0, 0xE09u, 1, F, 6
#define PPUTLIMPL_UHEX_0x1F5u 501u, 0x1F5, 501, 0, 0xE0Au, 1, F, 5
#define PPUTLIMPL_UHEX_0x1F4u 500u, 0x1F4, 500, 0, 0xE0Bu, 1, F, 4
#define PPUTLIMPL_UHEX_0x1F3u 499u, 0x1F3, 499, 0, 0xE0Cu, 1, F, 3
#define PPUTLIMPL_UHEX_0x1F2u 498u, 0x1F2, 498, 0, 0xE0Du, 1, F, 2
#define PPUTLIMPL_UHEX_0x1F1u 497u, 0x1F1, 497, 0, 0xE0Eu, 1, F, 1
#define PPUTLIMPL_UHEX_0x1F0u 496u, 0x1F0, 496, 0, 0xE0Fu, 1, F, 0
#define PPUTLIMPL_UHEX_0x1EFu 495u, 0x1EF, 495, 0, 0xE10u, 1, E, F
#define PPUTLIMPL_UHEX_0x1EEu 494u, 0x1EE, 494, 0, 0xE11u, 1, E, E
#define PPUTLIMPL_UHEX_0x1EDu 493u, 0x1ED, 493, 0, 0xE12u, 1, E, D
#define PPUTLIMPL_UHEX_0x1ECu 492u, 0x1EC, 492, 0, 0xE13u, 1, E, C
#define PPUTLIMPL_UHEX_0x1EBu 491u, 0x1EB, 491, 0, 0xE14u, 1, E, B
#define PPUTLIMPL_UHEX_0x1EAu 490u, 0x1EA, 490, 0, 0xE15u, 1, E, A
#define PPUTLIMPL_UHEX_0x1E9u 489u, 0x1E9, 489, 0, 0xE16u, 1, E, 9
#define PPUTLIMPL_UHEX_0x1E8u 488u, 0x1E8, 488, 0, 0xE17u, 1, E, 8
#define PPUTLIMPL_UHEX_0x1E7u 487u, 0x1E7, 487, 0, 0xE18u, 1, E, 7
#define PPUTLIMPL_UHEX_0x1E6u 486u, 0x1E6, 486, 0, 0xE19u, 1, E, 6
#define PPUTLIMPL_UHEX_0x1E5u 485u, 0x1E5, 485, 0, 0xE1Au, 1, E, 5
#define PPUTLIMPL_UHEX_0x1E4u 484u, 0x1E4, 484, 0, 0xE1Bu, 1, E, 4
#define PPUTLIMPL_UHEX_0x1E3u 483u, 0x1E3, 483, 0, 0xE1Cu, 1, E, 3
#define PPUTLIMPL_UHEX_0x1E2u 482u, 0x1E2, 482, 0, 0xE1Du, 1, E, 2
#define PPUTLIMPL_UHEX_0x1E1u 481u, 0x1E1, 481, 0, 0xE1Eu, 1, E, 1
#define PPUTLIMPL_UHEX_0x1E0u 480u, 0x1E0, 480, 0, 0xE1Fu, 1, E, 0
#define PPUTLIMPL_UHEX_0x1DFu 479u, 0x1DF, 479, 0, 0xE20u, 1, D, F
#define PPUTLIMPL_UHEX_0x1DEu 478u, 0x1DE, 478, 0, 0xE21u, 1, D, E
#define PPUTLIMPL_UHEX_0x1DDu 477u, 0x1DD, 477, 0, 0xE22u, 1, D, D
#define PPUTLIMPL_UHEX_0x1DCu 476u, 0x1DC, 476, 0, 0xE23u, 1, D, C
#define PPUTLIMPL_UHEX_0x1DBu 475u, 0x1DB, 475, 0, 0xE24u, 1, D, B
#define PPUTLIMPL_UHEX_0x1DAu 474u, 0x1DA, 474, 0, 0xE25u, 1, D, A
#define PPUTLIMPL_UHEX_0x1D9u 473u, 0x1D9, 473, 0, 0xE26u, 1, D, 9
#define PPUTLIMPL_UHEX_0x1D8u 472u, 0x1D8, 472, 0, 0xE27u, 1, D, 8
#define PPUTLIMPL_UHEX_0x1D7u 471u, 0x1D7, 471, 0, 0xE28u, 1, D, 7
#define PPUTLIMPL_UHEX_0x1D6u 470u, 0x1D6, 470, 0, 0xE29u, 1, D, 6
#define PPUTLIMPL_UHEX_0x1D5u 469u, 0x1D5, 469, 0, 0xE2Au, 1, D, 5
#define PPUTLIMPL_UHEX_0x1D4u 468u, 0x1D4, 468, 0, 0xE2Bu, 1, D, 4
#define PPUTLIMPL_UHEX_0x1D3u 467u, 0x1D3, 467, 0, 0xE2Cu, 1, D, 3
#define PPUTLIMPL_UHEX_0x1D2u 466u, 0x1D2, 466, 0, 0xE2Du, 1, D, 2
#define PPUTLIMPL_UHEX_0x1D1u 465u, 0x1D1, 465, 0, 0xE2Eu, 1, D, 1
#define PPUTLIMPL_UHEX_0x1D0u 464u, 0x1D0, 464, 0, 0xE2Fu, 1, D, 0
#define PPUTLIMPL_UHEX_0x1CFu 463u, 0x1CF, 463, 0, 0xE30u, 1, C, F
#define PPUTLIMPL_UHEX_0x1CEu 462u, 0x1CE, 462, 0, 0xE31u, 1, C, E
#define PPUTLIMPL_UHEX_0x1CDu 461u, 0x1CD, 461, 0, 0xE32u, 1, C, D
#define PPUTLIMPL_UHEX_0x1CCu 460u, 0x1CC, 460, 0, 0xE33u, 1, C, C
#define PPUTLIMPL_UHEX_0x1CBu 459u, 0x1CB, 459, 0, 0xE34u, 1, C, B
#define PPUTLIMPL_UHEX_0x1CAu 458u, 0x1CA, 458, 0, 0xE35u, 1, C, A
#define PPUTLIMPL_UHEX_0x1C9u 457u, 0x1C9, 457, 0, 0xE36u, 1, C, 9
#define PPUTLIMPL_UHEX_0x1C8u 456u, 0x1C8, 456, 0, 0xE37u, 1, C, 8
#define PPUTLIMPL_UHEX_0x1C7u 455u, 0x1C7, 455, 0, 0xE38u, 1, C, 7
#define PPUTLIMPL_UHEX_0x1C6u 454u, 0x1C6, 454, 0, 0xE39u, 1, C, 6
#define PPUTLIMPL_UHEX_0x1C5u 453u, 0x1C5, 453, 0, 0xE3Au, 1, C, 5
#define PPUTLIMPL_UHEX_0x1C4u 452u, 0x1C4, 452, 0, 0xE3Bu, 1, C, 4
#define PPUTLIMPL_UHEX_0x1C3u 451u, 0x1C3, 451, 0, 0xE3Cu, 1, C, 3
#define PPUTLIMPL_UHEX_0x1C2u 450u, 0x1C2, 450, 0, 0xE3Du, 1, C, 2
#define PPUTLIMPL_UHEX_0x1C1u 449u, 0x1C1, 449, 0, 0xE3Eu, 1, C, 1
#define PPUTLIMPL_UHEX_0x1C0u 448u, 0x1C0, 448, 0, 0xE3Fu, 1, C, 0
#define PPUTLIMPL_UHEX_0x1BFu 447u, 0x1BF, 447, 0, 0xE40u, 1, B, F
#define PPUTLIMPL_UHEX_0x1BEu 446u, 0x1BE, 446, 0, 0xE41u, 1, B, E
#define PPUTLIMPL_UHEX_0x1BDu 445u, 0x1BD, 445, 0, 0xE42u, 1, B, D
#define PPUTLIMPL_UHEX_0x1BCu 444u, 0x1BC, 444, 0, 0xE43u, 1, B, C
#define PPUTLIMPL_UHEX_0x1BBu 443u, 0x1BB, 443, 0, 0xE44u, 1, B, B
#define PPUTLIMPL_UHEX_0x1BAu 442u, 0x1BA, 442, 0, 0xE45u, 1, B, A
#define PPUTLIMPL_UHEX_0x1B9u 441u, 0x1B9, 441, 0, 0xE46u, 1, B, 9
#define PPUTLIMPL_UHEX_0x1B8u 440u, 0x1B8, 440, 0, 0xE47u, 1, B, 8
#define PPUTLIMPL_UHEX_0x1B7u 439u, 0x1B7, 439, 0, 0xE48u, 1, B, 7
#define PPUTLIMPL_UHEX_0x1B6u 438u, 0x1B6, 438, 0, 0xE49u, 1, B, 6
#define PPUTLIMPL_UHEX_0x1B5u 437u, 0x1B5, 437, 0, 0xE4Au, 1, B, 5
#define PPUTLIMPL_UHEX_0x1B4u 436u, 0x1B4, 436, 0, 0xE4Bu, 1, B, 4
#define PPUTLIMPL_UHEX_0x1B3u 435u, 0x1B3, 435, 0, 0xE4Cu, 1, B, 3
#define PPUTLIMPL_UHEX_0x1B2u 434u, 0x1B2, 434, 0, 0xE4Du, 1, B, 2
#define PPUTLIMPL_UHEX_0x1B1u 433u, 0x1B1, 433, 0, 0xE4Eu, 1, B, 1
#define PPUTLIMPL_UHEX_0x1B0u 432u, 0x1B0, 432, 0, 0xE4Fu, 1, B, 0
#define PPUTLIMPL_UHEX_0x1AFu 431u, 0x1AF, 431, 0, 0xE50u, 1, A, F
#define PPUTLIMPL_UHEX_0x1AEu 430u, 0x1AE, 430, 0, 0xE51u, 1, A, E
#define PPUTLIMPL_UHEX_0x1ADu 429u, 0x1AD, 429, 0, 0xE52u, 1, A, D
#define PPUTLIMPL_UHEX_0x1ACu 428u, 0x1AC, 428, 0, 0xE53u, 1, A, C
#define PPUTLIMPL_UHEX_0x1ABu 427u, 0x1AB, 427, 0, 0xE54u, 1, A, B
#define PPUTLIMPL_UHEX_0x1AAu 426u, 0x1AA, 426, 0, 0xE55u, 1, A, A
#define PPUTLIMPL_UHEX_0x1A9u 425u, 0x1A9, 425, 0, 0xE56u, 1, A, 9
#define PPUTLIMPL_UHEX_0x1A8u 424u, 0x1A8, 424, 0, 0xE57u, 1, A, 8
#define PPUTLIMPL_UHEX_0x1A7u 423u, 0x1A7, 423, 0, 0xE58u, 1, A, 7
#define PPUTLIMPL_UHEX_0x1A6u 422u, 0x1A6, 422, 0, 0xE59u, 1, A, 6
#define PPUTLIMPL_UHEX_0x1A5u 421u, 0x1A5, 421, 0, 0xE5Au, 1, A, 5
#define PPUTLIMPL_UHEX_0x1A4u 420u, 0x1A4, 420, 0, 0xE5Bu, 1, A, 4
#define PPUTLIMPL_UHEX_0x1A3u 419u, 0x1A3, 419, 0, 0xE5Cu, 1, A, 3
#define PPUTLIMPL_UHEX_0x1A2u 418u, 0x1A2, 418, 0, 0xE5Du, 1, A, 2
#define PPUTLIMPL_UHEX_0x1A1u 417u, 0x1A1, 417, 0, 0xE5Eu, 1, A, 1
#define PPUTLIMPL_UHEX_0x1A0u 416u, 0x1A0, 416, 0, 0xE5Fu, 1, A, 0
#define PPUTLIMPL_UHEX_0x19Fu 415u, 0x19F, 415, 0, 0xE60u, 1, 9, F
#define PPUTLIMPL_UHEX_0x19Eu 414u, 0x19E, 414, 0, 0xE61u, 1, 9, E
#define PPUTLIMPL_UHEX_0x19Du 413u, 0x19D, 413, 0, 0xE62u, 1, 9, D
#define PPUTLIMPL_UHEX_0x19Cu 412u, 0x19C, 412, 0, 0xE63u, 1, 9, C
#define PPUTLIMPL_UHEX_0x19Bu 411u, 0x19B, 411, 0, 0xE64u, 1, 9, B
#define PPUTLIMPL_UHEX_0x19Au 410u, 0x19A, 410, 0, 0xE65u, 1, 9, A
#define PPUTLIMPL_UHEX_0x199u 409u, 0x199, 409, 0, 0xE66u, 1, 9, 9
#define PPUTLIMPL_UHEX_0x198u 408u, 0x198, 408, 0, 0xE67u, 1, 9, 8
#define PPUTLIMPL_UHEX_0x197u 407u, 0x197, 407, 0, 0xE68u, 1, 9, 7
#define PPUTLIMPL_UHEX_0x196u 406u, 0x196, 406, 0, 0xE69u, 1, 9, 6
#define PPUTLIMPL_UHEX_0x195u 405u, 0x195, 405, 0, 0xE6Au, 1, 9, 5
#define PPUTLIMPL_UHEX_0x194u 404u, 0x194, 404, 0, 0xE6Bu, 1, 9, 4
#define PPUTLIMPL_UHEX_0x193u 403u, 0x193, 403, 0, 0xE6Cu, 1, 9, 3
#define PPUTLIMPL_UHEX_0x192u 402u, 0x192, 402, 0, 0xE6Du, 1, 9, 2
#define PPUTLIMPL_UHEX_0x191u 401u, 0x191, 401, 0, 0xE6Eu, 1, 9, 1
#define PPUTLIMPL_UHEX_0x190u 400u, 0x190, 400, 0, 0xE6Fu, 1, 9, 0
#define PPUTLIMPL_UHEX_0x18Fu 399u, 0x18F, 399, 0, 0xE70u, 1, 8, F
#define PPUTLIMPL_UHEX_0x18Eu 398u, 0x18E, 398, 0, 0xE71u, 1, 8, E
#define PPUTLIMPL_UHEX_0x18Du 397u, 0x18D, 397, 0, 0xE72u, 1, 8, D
#define PPUTLIMPL_UHEX_0x18Cu 396u, 0x18C, 396, 0, 0xE73u, 1, 8, C
#define PPUTLIMPL_UHEX_0x18Bu 395u, 0x18B, 395, 0, 0xE74u, 1, 8, B
#define PPUTLIMPL_UHEX_0x18Au 394u, 0x18A, 394, 0, 0xE75u, 1, 8, A
#define PPUTLIMPL_UHEX_0x189u 393u, 0x189, 393, 0, 0xE76u, 1, 8, 9
#define PPUTLIMPL_UHEX_0x188u 392u, 0x188, 392, 0, 0xE77u, 1, 8, 8
#define PPUTLIMPL_UHEX_0x187u 391u, 0x187, 391, 0, 0xE78u, 1, 8, 7
#define PPUTLIMPL_UHEX_0x186u 390u, 0x186, 390, 0, 0xE79u, 1, 8, 6
#define PPUTLIMPL_UHEX_0x185u 389u, 0x185, 389, 0, 0xE7Au, 1, 8, 5
#define PPUTLIMPL_UHEX_0x184u 388u, 0x184, 388, 0, 0xE7Bu, 1, 8, 4
#define PPUTLIMPL_UHEX_0x183u 387u, 0x183, 387, 0, 0xE7Cu, 1, 8, 3
#define PPUTLIMPL_UHEX_0x182u 386u, 0x182, 386, 0, 0xE7Du, 1, 8, 2
#define PPUTLIMPL_UHEX_0x181u 385u, 0x181, 385, 0, 0xE7Eu, 1, 8, 1
#define PPUTLIMPL_UHEX_0x180u 384u, 0x180, 384, 0, 0xE7Fu, 1, 8, 0
#define PPUTLIMPL_UHEX_0x17Fu 383u, 0x17F, 383, 0, 0xE80u, 1, 7, F
#define PPUTLIMPL_UHEX_0x17Eu 382u, 0x17E, 382, 0, 0xE81u, 1, 7, E
#define PPUTLIMPL_UHEX_0x17Du 381u, 0x17D, 381, 0, 0xE82u, 1, 7, D
#define PPUTLIMPL_UHEX_0x17Cu 380u, 0x17C, 380, 0, 0xE83u, 1, 7, C
#define PPUTLIMPL_UHEX_0x17Bu 379u, 0x17B, 379, 0, 0xE84u, 1, 7, B
#define PPUTLIMPL_UHEX_0x17Au 378u, 0x17A, 378, 0, 0xE85u, 1, 7, A
#define PPUTLIMPL_UHEX_0x179u 377u, 0x179, 377, 0, 0xE86u, 1, 7, 9
#define PPUTLIMPL_UHEX_0x178u 376u, 0x178, 376, 0, 0xE87u, 1, 7, 8
#define PPUTLIMPL_UHEX_0x177u 375u, 0x177, 375, 0, 0xE88u, 1, 7, 7
#define PPUTLIMPL_UHEX_0x176u 374u, 0x176, 374, 0, 0xE89u, 1, 7, 6
#define PPUTLIMPL_UHEX_0x175u 373u, 0x175, 373, 0, 0xE8Au, 1, 7, 5
#define PPUTLIMPL_UHEX_0x174u 372u, 0x174, 372, 0, 0xE8Bu, 1, 7, 4
#define PPUTLIMPL_UHEX_0x173u 371u, 0x173, 371, 0, 0xE8Cu, 1, 7, 3
#define PPUTLIMPL_UHEX_0x172u 370u, 0x172, 370, 0, 0xE8Du, 1, 7, 2
#define PPUTLIMPL_UHEX_0x171u 369u, 0x171, 369, 0, 0xE8Eu, 1, 7, 1
#define PPUTLIMPL_UHEX_0x170u 368u, 0x170, 368, 0, 0xE8Fu, 1, 7, 0
#define PPUTLIMPL_UHEX_0x16Fu 367u, 0x16F, 367, 0, 0xE90u, 1, 6, F
#define PPUTLIMPL_UHEX_0x16Eu 366u, 0x16E, 366, 0, 0xE91u, 1, 6, E
#define PPUTLIMPL_UHEX_0x16Du 365u, 0x16D, 365, 0, 0xE92u, 1, 6, D
#define PPUTLIMPL_UHEX_0x16Cu 364u, 0x16C, 364, 0, 0xE93u, 1, 6, C
#define PPUTLIMPL_UHEX_0x16Bu 363u, 0x16B, 363, 0, 0xE94u, 1, 6, B
#define PPUTLIMPL_UHEX_0x16Au 362u, 0x16A, 362, 0, 0xE95u, 1, 6, A
#define PPUTLIMPL_UHEX_0x169u 361u, 0x169, 361, 0, 0xE96u, 1, 6, 9
#define PPUTLIMPL_UHEX_0x168u 360u, 0x168, 360, 0, 0xE97u, 1, 6, 8
#define PPUTLIMPL_UHEX_0x167u 359u, 0x167, 359, 0, 0xE98u, 1, 6, 7
#define PPUTLIMPL_UHEX_0x166u 358u, 0x166, 358, 0, 0xE99u, 1, 6, 6
#define PPUTLIMPL_UHEX_0x165u 357u, 0x165, 357, 0, 0xE9Au, 1, 6, 5
#define PPUTLIMPL_UHEX_0x164u 356u, 0x164, 356, 0, 0xE9Bu, 1, 6, 4
#define PPUTLIMPL_UHEX_0x163u 355u, 0x163, 355, 0, 0xE9Cu, 1, 6, 3
#define PPUTLIMPL_UHEX_0x162u 354u, 0x162, 354, 0, 0xE9Du, 1, 6, 2
#define PPUTLIMPL_UHEX_0x161u 353u, 0x161, 353, 0, 0xE9Eu, 1, 6, 1
#define PPUTLIMPL_UHEX_0x160u 352u, 0x160, 352, 0, 0xE9Fu, 1, 6, 0
#define PPUTLIMPL_UHEX_0x15Fu 351u, 0x15F, 351, 0, 0xEA0u, 1, 5, F
#define PPUTLIMPL_UHEX_0x15Eu 350u, 0x15E, 350, 0, 0xEA1u, 1, 5, E
#define PPUTLIMPL_UHEX_0x15Du 349u, 0x15D, 349, 0, 0xEA2u, 1, 5, D
#define PPUTLIMPL_UHEX_0x15Cu 348u, 0x15C, 348, 0, 0xEA3u, 1, 5, C
#define PPUTLIMPL_UHEX_0x15Bu 347u, 0x15B, 347, 0, 0xEA4u, 1, 5, B
#define PPUTLIMPL_UHEX_0x15Au 346u, 0x15A, 346, 0, 0xEA5u, 1, 5, A
#define PPUTLIMPL_UHEX_0x159u 345u, 0x159, 345, 0, 0xEA6u, 1, 5, 9
#define PPUTLIMPL_UHEX_0x158u 344u, 0x158, 344, 0, 0xEA7u, 1, 5, 8
#define PPUTLIMPL_UHEX_0x157u 343u, 0x157, 343, 0, 0xEA8u, 1, 5, 7
#define PPUTLIMPL_UHEX_0x156u 342u, 0x156, 342, 0, 0xEA9u, 1, 5, 6
#define PPUTLIMPL_UHEX_0x155u 341u, 0x155, 341, 0, 0xEAAu, 1, 5, 5
#define PPUTLIMPL_UHEX_0x154u 340u, 0x154, 340, 0, 0xEABu, 1, 5, 4
#define PPUTLIMPL_UHEX_0x153u 339u, 0x153, 339, 0, 0xEACu, 1, 5, 3
#define PPUTLIMPL_UHEX_0x152u 338u, 0x152, 338, 0, 0xEADu, 1, 5, 2
#define PPUTLIMPL_UHEX_0x151u 337u, 0x151, 337, 0, 0xEAEu, 1, 5, 1
#define PPUTLIMPL_UHEX_0x150u 336u, 0x150, 336, 0, 0xEAFu, 1, 5, 0
#define PPUTLIMPL_UHEX_0x14Fu 335u, 0x14F, 335, 0, 0xEB0u, 1, 4, F
#define PPUTLIMPL_UHEX_0x14Eu 334u, 0x14E, 334, 0, 0xEB1u, 1, 4, E
#define PPUTLIMPL_UHEX_0x14Du 333u, 0x14D, 333, 0, 0xEB2u, 1, 4, D
#define PPUTLIMPL_UHEX_0x14Cu 332u, 0x14C, 332, 0, 0xEB3u, 1, 4, C
#define PPUTLIMPL_UHEX_0x14Bu 331u, 0x14B, 331, 0, 0xEB4u, 1, 4, B
#define PPUTLIMPL_UHEX_0x14Au 330u, 0x14A, 330, 0, 0xEB5u, 1, 4, A
#define PPUTLIMPL_UHEX_0x149u 329u, 0x149, 329, 0, 0xEB6u, 1, 4, 9
#define PPUTLIMPL_UHEX_0x148u 328u, 0x148, 328, 0, 0xEB7u, 1, 4, 8
#define PPUTLIMPL_UHEX_0x147u 327u, 0x147, 327, 0, 0xEB8u, 1, 4, 7
#define PPUTLIMPL_UHEX_0x146u 326u, 0x146, 326, 0, 0xEB9u, 1, 4, 6
#define PPUTLIMPL_UHEX_0x145u 325u, 0x145, 325, 0, 0xEBAu, 1, 4, 5
#define PPUTLIMPL_UHEX_0x144u 324u, 0x144, 324, 0, 0xEBBu, 1, 4, 4
#define PPUTLIMPL_UHEX_0x143u 323u, 0x143, 323, 0, 0xEBCu, 1, 4, 3
#define PPUTLIMPL_UHEX_0x142u 322u, 0x142, 322, 0, 0xEBDu, 1, 4, 2
#define PPUTLIMPL_UHEX_0x141u 321u, 0x141, 321, 0, 0xEBEu, 1, 4, 1
#define PPUTLIMPL_UHEX_0x140u 320u, 0x140, 320, 0, 0xEBFu, 1, 4, 0
#define PPUTLIMPL_UHEX_0x13Fu 319u, 0x13F, 319, 0, 0xEC0u, 1, 3, F
#define PPUTLIMPL_UHEX_0x13Eu 318u, 0x13E, 318, 0, 0xEC1u, 1, 3, E
#define PPUTLIMPL_UHEX_0x13Du 317u, 0x13D, 317, 0, 0xEC2u, 1, 3, D
#define PPUTLIMPL_UHEX_0x13Cu 316u, 0x13C, 316, 0, 0xEC3u, 1, 3, C
#define PPUTLIMPL_UHEX_0x13Bu 315u, 0x13B, 315, 0, 0xEC4u, 1, 3, B
#define PPUTLIMPL_UHEX_0x13Au 314u, 0x13A, 314, 0, 0xEC5u, 1, 3, A
#define PPUTLIMPL_UHEX_0x139u 313u, 0x139, 313, 0, 0xEC6u, 1, 3, 9
#define PPUTLIMPL_UHEX_0x138u 312u, 0x138, 312, 0, 0xEC7u, 1, 3, 8
#define PPUTLIMPL_UHEX_0x137u 311u, 0x137, 311, 0, 0xEC8u, 1, 3, 7
#define PPUTLIMPL_UHEX_0x136u 310u, 0x136, 310, 0, 0xEC9u, 1, 3, 6
#define PPUTLIMPL_UHEX_0x135u 309u, 0x135, 309, 0, 0xECAu, 1, 3, 5
#define PPUTLIMPL_UHEX_0x134u 308u, 0x134, 308, 0, 0xECBu, 1, 3, 4
#define PPUTLIMPL_UHEX_0x133u 307u, 0x133, 307, 0, 0xECCu, 1, 3, 3
#define PPUTLIMPL_UHEX_0x132u 306u, 0x132, 306, 0, 0xECDu, 1, 3, 2
#define PPUTLIMPL_UHEX_0x131u 305u, 0x131, 305, 0, 0xECEu, 1, 3, 1
#define PPUTLIMPL_UHEX_0x130u 304u, 0x130, 304, 0, 0xECFu, 1, 3, 0
#define PPUTLIMPL_UHEX_0x12Fu 303u, 0x12F, 303, 0, 0xED0u, 1, 2, F
#define PPUTLIMPL_UHEX_0x12Eu 302u, 0x12E, 302, 0, 0xED1u, 1, 2, E
#define PPUTLIMPL_UHEX_0x12Du 301u, 0x12D, 301, 0, 0xED2u, 1, 2, D
#define PPUTLIMPL_UHEX_0x12Cu 300u, 0x12C, 300, 0, 0xED3u, 1, 2, C
#define PPUTLIMPL_UHEX_0x12Bu 299u, 0x12B, 299, 0, 0xED4u, 1, 2, B
#define PPUTLIMPL_UHEX_0x12Au 298u, 0x12A, 298, 0, 0xED5u, 1, 2, A
#define PPUTLIMPL_UHEX_0x129u 297u, 0x129, 297, 0, 0xED6u, 1, 2, 9
#define PPUTLIMPL_UHEX_0x128u 296u, 0x128, 296, 0, 0xED7u, 1, 2, 8
#define PPUTLIMPL_UHEX_0x127u 295u, 0x127, 295, 0, 0xED8u, 1, 2, 7
#define PPUTLIMPL_UHEX_0x126u 294u, 0x126, 294, 0, 0xED9u, 1, 2, 6
#define PPUTLIMPL_UHEX_0x125u 293u, 0x125, 293, 0, 0xEDAu, 1, 2, 5
#define PPUTLIMPL_UHEX_0x124u 292u, 0x124, 292, 0, 0xEDBu, 1, 2, 4
#define PPUTLIMPL_UHEX_0x123u 291u, 0x123, 291, 0, 0xEDCu, 1, 2, 3
#define PPUTLIMPL_UHEX_0x122u 290u, 0x122, 290, 0, 0xEDDu, 1, 2, 2
#define PPUTLIMPL_UHEX_0x121u 289u, 0x121, 289, 0, 0xEDEu, 1, 2, 1
#define PPUTLIMPL_UHEX_0x120u 288u, 0x120, 288, 0, 0xEDFu, 1, 2, 0
#define PPUTLIMPL_UHEX_0x11Fu 287u, 0x11F, 287, 0, 0xEE0u, 1, 1, F
#define PPUTLIMPL_UHEX_0x11Eu 286u, 0x11E, 286, 0, 0xEE1u, 1, 1, E
#define PPUTLIMPL_UHEX_0x11Du 285u, 0x11D, 285, 0, 0xEE2u, 1, 1, D
#define PPUTLIMPL_UHEX_0x11Cu 284u, 0x11C, 284, 0, 0xEE3u, 1, 1, C
#define PPUTLIMPL_UHEX_0x11Bu 283u, 0x11B, 283, 0, 0xEE4u, 1, 1, B
#define PPUTLIMPL_UHEX_0x11Au 282u, 0x11A, 282, 0, 0xEE5u, 1, 1, A
#define PPUTLIMPL_UHEX_0x119u 281u, 0x119, 281, 0, 0xEE6u, 1, 1, 9
#define PPUTLIMPL_UHEX_0x118u 280u, 0x118, 280, 0, 0xEE7u, 1, 1, 8
#define PPUTLIMPL_UHEX_0x117u 279u, 0x117, 279, 0, 0xEE8u, 1, 1, 7
#define PPUTLIMPL_UHEX_0x116u 278u, 0x116, 278, 0, 0xEE9u, 1, 1, 6
#define PPUTLIMPL_UHEX_0x115u 277u, 0x115, 277, 0, 0xEEAu, 1, 1, 5
#define PPUTLIMPL_UHEX_0x114u 276u, 0x114, 276, 0, 0xEEBu, 1, 1, 4
#define PPUTLIMPL_UHEX_0x113u 275u, 0x113, 275, 0, 0xEECu, 1, 1, 3
#define PPUTLIMPL_UHEX_0x112u 274u, 0x112, 274, 0, 0xEEDu, 1, 1, 2
#define PPUTLIMPL_UHEX_0x111u 273u, 0x111, 273, 0, 0xEEEu, 1, 1, 1
#define PPUTLIMPL_UHEX_0x110u 272u, 0x110, 272, 0, 0xEEFu, 1, 1, 0
#define PPUTLIMPL_UHEX_0x10Fu 271u, 0x10F, 271, 0, 0xEF0u, 1, 0, F
#define PPUTLIMPL_UHEX_0x10Eu 270u, 0x10E, 270, 0, 0xEF1u, 1, 0, E
#define PPUTLIMPL_UHEX_0x10Du 269u, 0x10D, 269, 0, 0xEF2u, 1, 0, D
#define PPUTLIMPL_UHEX_0x10Cu 268u, 0x10C, 268, 0, 0xEF3u, 1, 0, C
#define PPUTLIMPL_UHEX_0x10Bu 267u, 0x10B, 267, 0, 0xEF4u, 1, 0, B
#define PPUTLIMPL_UHEX_0x10Au 266u, 0x10A, 266, 0, 0xEF5u, 1, 0, A
#define PPUTLIMPL_UHEX_0x109u 265u, 0x109, 265, 0, 0xEF6u, 1, 0, 9
#define PPUTLIMPL_UHEX_0x108u 264u, 0x108, 264, 0, 0xEF7u, 1, 0, 8
#define PPUTLIMPL_UHEX_0x107u 263u, 0x107, 263, 0, 0xEF8u, 1, 0, 7
#define PPUTLIMPL_UHEX_0x106u 262u, 0x106, 262, 0, 0xEF9u, 1, 0, 6
#define PPUTLIMPL_UHEX_0x105u 261u, 0x105, 261, 0, 0xEFAu, 1, 0, 5
#define PPUTLIMPL_UHEX_0x104u 260u, 0x104, 260, 0, 0xEFBu, 1, 0, 4
#define PPUTLIMPL_UHEX_0x103u 259u, 0x103, 259, 0, 0xEFCu, 1, 0, 3
#define PPUTLIMPL_UHEX_0x102u 258u, 0x102, 258, 0, 0xEFDu, 1, 0, 2
#define PPUTLIMPL_UHEX_0x101u 257u, 0x101, 257, 0, 0xEFEu, 1, 0, 1
#define PPUTLIMPL_UHEX_0x100u 256u, 0x100, 256, 0, 0xEFFu, 1, 0, 0
#define PPUTLIMPL_UHEX_0x0FFu 255u, 0x0FF, 255, 0, 0xF00u, 0, F, F
#define PPUTLIMPL_UHEX_0x0FEu 254u, 0x0FE, 254, 0, 0xF01u, 0, F, E
#define PPUTLIMPL_UHEX_0x0FDu 253u, 0x0FD, 253, 0, 0xF02u, 0, F, D
#define PPUTLIMPL_UHEX_0x0FCu 252u, 0x0FC, 252, 0, 0xF03u, 0, F, C
#define PPUTLIMPL_UHEX_0x0FBu 251u, 0x0FB, 251, 0, 0xF04u, 0, F, B
#define PPUTLIMPL_UHEX_0x0FAu 250u, 0x0FA, 250, 0, 0xF05u, 0, F, A
#define PPUTLIMPL_UHEX_0x0F9u 249u, 0x0F9, 249, 0, 0xF06u, 0, F, 9
#define PPUTLIMPL_UHEX_0x0F8u 248u, 0x0F8, 248, 0, 0xF07u, 0, F, 8
#define PPUTLIMPL_UHEX_0x0F7u 247u, 0x0F7, 247, 0, 0xF08u, 0, F, 7
#define PPUTLIMPL_UHEX_0x0F6u 246u, 0x0F6, 246, 0, 0xF09u, 0, F, 6
#define PPUTLIMPL_UHEX_0x0F5u 245u, 0x0F5, 245, 0, 0xF0Au, 0, F, 5
#define PPUTLIMPL_UHEX_0x0F4u 244u, 0x0F4, 244, 0, 0xF0Bu, 0, F, 4
#define PPUTLIMPL_UHEX_0x0F3u 243u, 0x0F3, 243, 0, 0xF0Cu, 0, F, 3
#define PPUTLIMPL_UHEX_0x0F2u 242u, 0x0F2, 242, 0, 0xF0Du, 0, F, 2
#define PPUTLIMPL_UHEX_0x0F1u 241u, 0x0F1, 241, 0, 0xF0Eu, 0, F, 1
#define PPUTLIMPL_UHEX_0x0F0u 240u, 0x0F0, 240, 0, 0xF0Fu, 0, F, 0
#define PPUTLIMPL_UHEX_0x0EFu 239u, 0x0EF, 239, 0, 0xF10u, 0, E, F
#define PPUTLIMPL_UHEX_0x0EEu 238u, 0x0EE, 238, 0, 0xF11u, 0, E, E
#define PPUTLIMPL_UHEX_0x0EDu 237u, 0x0ED, 237, 0, 0xF12u, 0, E, D
#define PPUTLIMPL_UHEX_0x0ECu 236u, 0x0EC, 236, 0, 0xF13u, 0, E, C
#define PPUTLIMPL_UHEX_0x0EBu 235u, 0x0EB, 235, 0, 0xF14u, 0, E, B
#define PPUTLIMPL_UHEX_0x0EAu 234u, 0x0EA, 234, 0, 0xF15u, 0, E, A
#define PPUTLIMPL_UHEX_0x0E9u 233u, 0x0E9, 233, 0, 0xF16u, 0, E, 9
#define PPUTLIMPL_UHEX_0x0E8u 232u, 0x0E8, 232, 0, 0xF17u, 0, E, 8
#define PPUTLIMPL_UHEX_0x0E7u 231u, 0x0E7, 231, 0, 0xF18u, 0, E, 7
#define PPUTLIMPL_UHEX_0x0E6u 230u, 0x0E6, 230, 0, 0xF19u, 0, E, 6
#define PPUTLIMPL_UHEX_0x0E5u 229u, 0x0E5, 229, 0, 0xF1Au, 0, E, 5
#define PPUTLIMPL_UHEX_0x0E4u 228u, 0x0E4, 228, 0, 0xF1Bu, 0, E, 4
#define PPUTLIMPL_UHEX_0x0E3u 227u, 0x0E3, 227, 0, 0xF1Cu, 0, E, 3
#define PPUTLIMPL_UHEX_0x0E2u 226u, 0x0E2, 226, 0, 0xF1Du, 0, E, 2
#define PPUTLIMPL_UHEX_0x0E1u 225u, 0x0E1, 225, 0, 0xF1Eu, 0, E, 1
#define PPUTLIMPL_UHEX_0x0E0u 224u, 0x0E0, 224, 0, 0xF1Fu, 0, E, 0
#define PPUTLIMPL_UHEX_0x0DFu 223u, 0x0DF, 223, 0, 0xF20u, 0, D, F
#define PPUTLIMPL_UHEX_0x0DEu 222u, 0x0DE, 222, 0, 0xF21u, 0, D, E
#define PPUTLIMPL_UHEX_0x0DDu 221u, 0x0DD, 221, 0, 0xF22u, 0, D, D
#define PPUTLIMPL_UHEX_0x0DCu 220u, 0x0DC, 220, 0, 0xF23u, 0, D, C
#define PPUTLIMPL_UHEX_0x0DBu 219u, 0x0DB, 219, 0, 0xF24u, 0, D, B
#define PPUTLIMPL_UHEX_0x0DAu 218u, 0x0DA, 218, 0, 0xF25u, 0, D, A
#define PPUTLIMPL_UHEX_0x0D9u 217u, 0x0D9, 217, 0, 0xF26u, 0, D, 9
#define PPUTLIMPL_UHEX_0x0D8u 216u, 0x0D8, 216, 0, 0xF27u, 0, D, 8
#define PPUTLIMPL_UHEX_0x0D7u 215u, 0x0D7, 215, 0, 0xF28u, 0, D, 7
#define PPUTLIMPL_UHEX_0x0D6u 214u, 0x0D6, 214, 0, 0xF29u, 0, D, 6
#define PPUTLIMPL_UHEX_0x0D5u 213u, 0x0D5, 213, 0, 0xF2Au, 0, D, 5
#define PPUTLIMPL_UHEX_0x0D4u 212u, 0x0D4, 212, 0, 0xF2Bu, 0, D, 4
#define PPUTLIMPL_UHEX_0x0D3u 211u, 0x0D3, 211, 0, 0xF2Cu, 0, D, 3
#define PPUTLIMPL_UHEX_0x0D2u 210u, 0x0D2, 210, 0, 0xF2Du, 0, D, 2
#define PPUTLIMPL_UHEX_0x0D1u 209u, 0x0D1, 209, 0, 0xF2Eu, 0, D, 1
#define PPUTLIMPL_UHEX_0x0D0u 208u, 0x0D0, 208, 0, 0xF2Fu, 0, D, 0
#define PPUTLIMPL_UHEX_0x0CFu 207u, 0x0CF, 207, 0, 0xF30u, 0, C, F
#define PPUTLIMPL_UHEX_0x0CEu 206u, 0x0CE, 206, 0, 0xF31u, 0, C, E
#define PPUTLIMPL_UHEX_0x0CDu 205u, 0x0CD, 205, 0, 0xF32u, 0, C, D
#define PPUTLIMPL_UHEX_0x0CCu 204u, 0x0CC, 204, 0, 0xF33u, 0, C, C
#define PPUTLIMPL_UHEX_0x0CBu 203u, 0x0CB, 203, 0, 0xF34u, 0, C, B
#define PPUTLIMPL_UHEX_0x0CAu 202u, 0x0CA, 202, 0, 0xF35u, 0, C, A
#define PPUTLIMPL_UHEX_0x0C9u 201u, 0x0C9, 201, 0, 0xF36u, 0, C, 9
#define PPUTLIMPL_UHEX_0x0C8u 200u, 0x0C8, 200, 0, 0xF37u, 0, C, 8
#define PPUTLIMPL_UHEX_0x0C7u 199u, 0x0C7, 199, 0, 0xF38u, 0, C, 7
#define PPUTLIMPL_UHEX_0x0C6u 198u, 0x0C6, 198, 0, 0xF39u, 0, C, 6
#define PPUTLIMPL_UHEX_0x0C5u 197u, 0x0C5, 197, 0, 0xF3Au, 0, C, 5
#define PPUTLIMPL_UHEX_0x0C4u 196u, 0x0C4, 196, 0, 0xF3Bu, 0, C, 4
#define PPUTLIMPL_UHEX_0x0C3u 195u, 0x0C3, 195, 0, 0xF3Cu, 0, C, 3
#define PPUTLIMPL_UHEX_0x0C2u 194u, 0x0C2, 194, 0, 0xF3Du, 0, C, 2
#define PPUTLIMPL_UHEX_0x0C1u 193u, 0x0C1, 193, 0, 0xF3Eu, 0, C, 1
#define PPUTLIMPL_UHEX_0x0C0u 192u, 0x0C0, 192, 0, 0xF3Fu, 0, C, 0
#define PPUTLIMPL_UHEX_0x0BFu 191u, 0x0BF, 191, 0, 0xF40u, 0, B, F
#define PPUTLIMPL_UHEX_0x0BEu 190u, 0x0BE, 190, 0, 0xF41u, 0, B, E
#define PPUTLIMPL_UHEX_0x0BDu 189u, 0x0BD, 189, 0, 0xF42u, 0, B, D
#define PPUTLIMPL_UHEX_0x0BCu 188u, 0x0BC, 188, 0, 0xF43u, 0, B, C
#define PPUTLIMPL_UHEX_0x0BBu 187u, 0x0BB, 187, 0, 0xF44u, 0, B, B
#define PPUTLIMPL_UHEX_0x0BAu 186u, 0x0BA, 186, 0, 0xF45u, 0, B, A
#define PPUTLIMPL_UHEX_0x0B9u 185u, 0x0B9, 185, 0, 0xF46u, 0, B, 9
#define PPUTLIMPL_UHEX_0x0B8u 184u, 0x0B8, 184, 0, 0xF47u, 0, B, 8
#define PPUTLIMPL_UHEX_0x0B7u 183u, 0x0B7, 183, 0, 0xF48u, 0, B, 7
#define PPUTLIMPL_UHEX_0x0B6u 182u, 0x0B6, 182, 0, 0xF49u, 0, B, 6
#define PPUTLIMPL_UHEX_0x0B5u 181u, 0x0B5, 181, 0, 0xF4Au, 0, B, 5
#define PPUTLIMPL_UHEX_0x0B4u 180u, 0x0B4, 180, 0, 0xF4Bu, 0, B, 4
#define PPUTLIMPL_UHEX_0x0B3u 179u, 0x0B3, 179, 0, 0xF4Cu, 0, B, 3
#define PPUTLIMPL_UHEX_0x0B2u 178u, 0x0B2, 178, 0, 0xF4Du, 0, B, 2
#define PPUTLIMPL_UHEX_0x0B1u 177u, 0x0B1, 177, 0, 0xF4Eu, 0, B, 1
#define PPUTLIMPL_UHEX_0x0B0u 176u, 0x0B0, 176, 0, 0xF4Fu, 0, B, 0
#define PPUTLIMPL_UHEX_0x0AFu 175u, 0x0AF, 175, 0, 0xF50u, 0, A, F
#define PPUTLIMPL_UHEX_0x0AEu 174u, 0x0AE, 174, 0, 0xF51u, 0, A, E
#define PPUTLIMPL_UHEX_0x0ADu 173u, 0x0AD, 173, 0, 0xF52u, 0, A, D
#define PPUTLIMPL_UHEX_0x0ACu 172u, 0x0AC, 172, 0, 0xF53u, 0, A, C
#define PPUTLIMPL_UHEX_0x0ABu 171u, 0x0AB, 171, 0, 0xF54u, 0, A, B
#define PPUTLIMPL_UHEX_0x0AAu 170u, 0x0AA, 170, 0, 0xF55u, 0, A, A
#define PPUTLIMPL_UHEX_0x0A9u 169u, 0x0A9, 169, 0, 0xF56u, 0, A, 9
#define PPUTLIMPL_UHEX_0x0A8u 168u, 0x0A8, 168, 0, 0xF57u, 0, A, 8
#define PPUTLIMPL_UHEX_0x0A7u 167u, 0x0A7, 167, 0, 0xF58u, 0, A, 7
#define PPUTLIMPL_UHEX_0x0A6u 166u, 0x0A6, 166, 0, 0xF59u, 0, A, 6
#define PPUTLIMPL_UHEX_0x0A5u 165u, 0x0A5, 165, 0, 0xF5Au, 0, A, 5
#define PPUTLIMPL_UHEX_0x0A4u 164u, 0x0A4, 164, 0, 0xF5Bu, 0, A, 4
#define PPUTLIMPL_UHEX_0x0A3u 163u, 0x0A3, 163, 0, 0xF5Cu, 0, A, 3
#define PPUTLIMPL_UHEX_0x0A2u 162u, 0x0A2, 162, 0, 0xF5Du, 0, A, 2
#define PPUTLIMPL_UHEX_0x0A1u 161u, 0x0A1, 161, 0, 0xF5Eu, 0, A, 1
#define PPUTLIMPL_UHEX_0x0A0u 160u, 0x0A0, 160, 0, 0xF5Fu, 0, A, 0
#define PPUTLIMPL_UHEX_0x09Fu 159u, 0x09F, 159, 0, 0xF60u, 0, 9, F
#define PPUTLIMPL_UHEX_0x09Eu 158u, 0x09E, 158, 0, 0xF61u, 0, 9, E
#define PPUTLIMPL_UHEX_0x09Du 157u, 0x09D, 157, 0, 0xF62u, 0, 9, D
#define PPUTLIMPL_UHEX_0x09Cu 156u, 0x09C, 156, 0, 0xF63u, 0, 9, C
#define PPUTLIMPL_UHEX_0x09Bu 155u, 0x09B, 155, 0, 0xF64u, 0, 9, B
#define PPUTLIMPL_UHEX_0x09Au 154u, 0x09A, 154, 0, 0xF65u, 0, 9, A
#define PPUTLIMPL_UHEX_0x099u 153u, 0x099, 153, 0, 0xF66u, 0, 9, 9
#define PPUTLIMPL_UHEX_0x098u 152u, 0x098, 152, 0, 0xF67u, 0, 9, 8
#define PPUTLIMPL_UHEX_0x097u 151u, 0x097, 151, 0, 0xF68u, 0, 9, 7
#define PPUTLIMPL_UHEX_0x096u 150u, 0x096, 150, 0, 0xF69u, 0, 9, 6
#define PPUTLIMPL_UHEX_0x095u 149u, 0x095, 149, 0, 0xF6Au, 0, 9, 5
#define PPUTLIMPL_UHEX_0x094u 148u, 0x094, 148, 0, 0xF6Bu, 0, 9, 4
#define PPUTLIMPL_UHEX_0x093u 147u, 0x093, 147, 0, 0xF6Cu, 0, 9, 3
#define PPUTLIMPL_UHEX_0x092u 146u, 0x092, 146, 0, 0xF6Du, 0, 9, 2
#define PPUTLIMPL_UHEX_0x091u 145u, 0x091, 145, 0, 0xF6Eu, 0, 9, 1
#define PPUTLIMPL_UHEX_0x090u 144u, 0x090, 144, 0, 0xF6Fu, 0, 9, 0
#define PPUTLIMPL_UHEX_0x08Fu 143u, 0x08F, 143, 0, 0xF70u, 0, 8, F
#define PPUTLIMPL_UHEX_0x08Eu 142u, 0x08E, 142, 0, 0xF71u, 0, 8, E
#define PPUTLIMPL_UHEX_0x08Du 141u, 0x08D, 141, 0, 0xF72u, 0, 8, D
#define PPUTLIMPL_UHEX_0x08Cu 140u, 0x08C, 140, 0, 0xF73u, 0, 8, C
#define PPUTLIMPL_UHEX_0x08Bu 139u, 0x08B, 139, 0, 0xF74u, 0, 8, B
#define PPUTLIMPL_UHEX_0x08Au 138u, 0x08A, 138, 0, 0xF75u, 0, 8, A
#define PPUTLIMPL_UHEX_0x089u 137u, 0x089, 137, 0, 0xF76u, 0, 8, 9
#define PPUTLIMPL_UHEX_0x088u 136u, 0x088, 136, 0, 0xF77u, 0, 8, 8
#define PPUTLIMPL_UHEX_0x087u 135u, 0x087, 135, 0, 0xF78u, 0, 8, 7
#define PPUTLIMPL_UHEX_0x086u 134u, 0x086, 134, 0, 0xF79u, 0, 8, 6
#define PPUTLIMPL_UHEX_0x085u 133u, 0x085, 133, 0, 0xF7Au, 0, 8, 5
#define PPUTLIMPL_UHEX_0x084u 132u, 0x084, 132, 0, 0xF7Bu, 0, 8, 4
#define PPUTLIMPL_UHEX_0x083u 131u, 0x083, 131, 0, 0xF7Cu, 0, 8, 3
#define PPUTLIMPL_UHEX_0x082u 130u, 0x082, 130, 0, 0xF7Du, 0, 8, 2
#define PPUTLIMPL_UHEX_0x081u 129u, 0x081, 129, 0, 0xF7Eu, 0, 8, 1
#define PPUTLIMPL_UHEX_0x080u 128u, 0x080, 128, 0, 0xF7Fu, 0, 8, 0
#define PPUTLIMPL_UHEX_0x07Fu 127u, 0x07F, 127, 0, 0xF80u, 0, 7, F
#define PPUTLIMPL_UHEX_0x07Eu 126u, 0x07E, 126, 0, 0xF81u, 0, 7, E
#define PPUTLIMPL_UHEX_0x07Du 125u, 0x07D, 125, 0, 0xF82u, 0, 7, D
#define PPUTLIMPL_UHEX_0x07Cu 124u, 0x07C, 124, 0, 0xF83u, 0, 7, C
#define PPUTLIMPL_UHEX_0x07Bu 123u, 0x07B, 123, 0, 0xF84u, 0, 7, B
#define PPUTLIMPL_UHEX_0x07Au 122u, 0x07A, 122, 0, 0xF85u, 0, 7, A
#define PPUTLIMPL_UHEX_0x079u 121u, 0x079, 121, 0, 0xF86u, 0, 7, 9
#define PPUTLIMPL_UHEX_0x078u 120u, 0x078, 120, 0, 0xF87u, 0, 7, 8
#define PPUTLIMPL_UHEX_0x077u 119u, 0x077, 119, 0, 0xF88u, 0, 7, 7
#define PPUTLIMPL_UHEX_0x076u 118u, 0x076, 118, 0, 0xF89u, 0, 7, 6
#define PPUTLIMPL_UHEX_0x075u 117u, 0x075, 117, 0, 0xF8Au, 0, 7, 5
#define PPUTLIMPL_UHEX_0x074u 116u, 0x074, 116, 0, 0xF8Bu, 0, 7, 4
#define PPUTLIMPL_UHEX_0x073u 115u, 0x073, 115, 0, 0xF8Cu, 0, 7, 3
#define PPUTLIMPL_UHEX_0x072u 114u, 0x072, 114, 0, 0xF8Du, 0, 7, 2
#define PPUTLIMPL_UHEX_0x071u 113u, 0x071, 113, 0, 0xF8Eu, 0, 7, 1
#define PPUTLIMPL_UHEX_0x070u 112u, 0x070, 112, 0, 0xF8Fu, 0, 7, 0
#define PPUTLIMPL_UHEX_0x06Fu 111u, 0x06F, 111, 0, 0xF90u, 0, 6, F
#define PPUTLIMPL_UHEX_0x06Eu 110u, 0x06E, 110, 0, 0xF91u, 0, 6, E
#define PPUTLIMPL_UHEX_0x06Du 109u, 0x06D, 109, 0, 0xF92u, 0, 6, D
#define PPUTLIMPL_UHEX_0x06Cu 108u, 0x06C, 108, 0, 0xF93u, 0, 6, C
#define PPUTLIMPL_UHEX_0x06Bu 107u, 0x06B, 107, 0, 0xF94u, 0, 6, B
#define PPUTLIMPL_UHEX_0x06Au 106u, 0x06A, 106, 0, 0xF95u, 0, 6, A
#define PPUTLIMPL_UHEX_0x069u 105u, 0x069, 105, 0, 0xF96u, 0, 6, 9
#define PPUTLIMPL_UHEX_0x068u 104u, 0x068, 104, 0, 0xF97u, 0, 6, 8
#define PPUTLIMPL_UHEX_0x067u 103u, 0x067, 103, 0, 0xF98u, 0, 6, 7
#define PPUTLIMPL_UHEX_0x066u 102u, 0x066, 102, 0, 0xF99u, 0, 6, 6
#define PPUTLIMPL_UHEX_0x065u 101u, 0x065, 101, 0, 0xF9Au, 0, 6, 5
#define PPUTLIMPL_UHEX_0x064u 100u, 0x064, 100, 0, 0xF9Bu, 0, 6, 4
#define PPUTLIMPL_UHEX_0x063u 99u, 0x063, 99, 0, 0xF9Cu, 0, 6, 3
#define PPUTLIMPL_UHEX_0x062u 98u, 0x062, 98, 0, 0xF9Du, 0, 6, 2
#define PPUTLIMPL_UHEX_0x061u 97u, 0x061, 97, 0, 0xF9Eu, 0, 6, 1
#define PPUTLIMPL_UHEX_0x060u 96u, 0x060, 96, 0, 0xF9Fu, 0, 6, 0
#define PPUTLIMPL_UHEX_0x05Fu 95u, 0x05F, 95, 0, 0xFA0u, 0, 5, F
#define PPUTLIMPL_UHEX_0x05Eu 94u, 0x05E, 94, 0, 0xFA1u, 0, 5, E
#define PPUTLIMPL_UHEX_0x05Du 93u, 0x05D, 93, 0, 0xFA2u, 0, 5, D
#define PPUTLIMPL_UHEX_0x05Cu 92u, 0x05C, 92, 0, 0xFA3u, 0, 5, C
#define PPUTLIMPL_UHEX_0x05Bu 91u, 0x05B, 91, 0, 0xFA4u, 0, 5, B
#define PPUTLIMPL_UHEX_0x05Au 90u, 0x05A, 90, 0, 0xFA5u, 0, 5, A
#define PPUTLIMPL_UHEX_0x059u 89u, 0x059, 89, 0, 0xFA6u, 0, 5, 9
#define PPUTLIMPL_UHEX_0x058u 88u, 0x058, 88, 0, 0xFA7u, 0, 5, 8
#define PPUTLIMPL_UHEX_0x057u 87u, 0x057, 87, 0, 0xFA8u, 0, 5, 7
#define PPUTLIMPL_UHEX_0x056u 86u, 0x056, 86, 0, 0xFA9u, 0, 5, 6
#define PPUTLIMPL_UHEX_0x055u 85u, 0x055, 85, 0, 0xFAAu, 0, 5, 5
#define PPUTLIMPL_UHEX_0x054u 84u, 0x054, 84, 0, 0xFABu, 0, 5, 4
#define PPUTLIMPL_UHEX_0x053u 83u, 0x053, 83, 0, 0xFACu, 0, 5, 3
#define PPUTLIMPL_UHEX_0x052u 82u, 0x052, 82, 0, 0xFADu, 0, 5, 2
#define PPUTLIMPL_UHEX_0x051u 81u, 0x051, 81, 0, 0xFAEu, 0, 5, 1
#define PPUTLIMPL_UHEX_0x050u 80u, 0x050, 80, 0, 0xFAFu, 0, 5, 0
#define PPUTLIMPL_UHEX_0x04Fu 79u, 0x04F, 79, 0, 0xFB0u, 0, 4, F
#define PPUTLIMPL_UHEX_0x04Eu 78u, 0x04E, 78, 0, 0xFB1u, 0, 4, E
#define PPUTLIMPL_UHEX_0x04Du 77u, 0x04D, 77, 0, 0xFB2u, 0, 4, D
#define PPUTLIMPL_UHEX_0x04Cu 76u, 0x04C, 76, 0, 0xFB3u, 0, 4, C
#define PPUTLIMPL_UHEX_0x04Bu 75u, 0x04B, 75, 0, 0xFB4u, 0, 4, B
#define PPUTLIMPL_UHEX_0x04Au 74u, 0x04A, 74, 0, 0xFB5u, 0, 4, A
#define PPUTLIMPL_UHEX_0x049u 73u, 0x049, 73, 0, 0xFB6u, 0, 4, 9
#define PPUTLIMPL_UHEX_0x048u 72u, 0x048, 72, 0, 0xFB7u, 0, 4, 8
#define PPUTLIMPL_UHEX_0x047u 71u, 0x047, 71, 0, 0xFB8u, 0, 4, 7
#define PPUTLIMPL_UHEX_0x046u 70u, 0x046, 70, 0, 0xFB9u, 0, 4, 6
#define PPUTLIMPL_UHEX_0x045u 69u, 0x045, 69, 0, 0xFBAu, 0, 4, 5
#define PPUTLIMPL_UHEX_0x044u 68u, 0x044, 68, 0, 0xFBBu, 0, 4, 4
#define PPUTLIMPL_UHEX_0x043u 67u, 0x043, 67, 0, 0xFBCu, 0, 4, 3
#define PPUTLIMPL_UHEX_0x042u 66u, 0x042, 66, 0, 0xFBDu, 0, 4, 2
#define PPUTLIMPL_UHEX_0x041u 65u, 0x041, 65, 0, 0xFBEu, 0, 4, 1
#define PPUTLIMPL_UHEX_0x040u 64u, 0x040, 64, 0, 0xFBFu, 0, 4, 0
#define PPUTLIMPL_UHEX_0x03Fu 63u, 0x03F, 63, 0, 0xFC0u, 0, 3, F
#define PPUTLIMPL_UHEX_0x03Eu 62u, 0x03E, 62, 0, 0xFC1u, 0, 3, E
#define PPUTLIMPL_UHEX_0x03Du 61u, 0x03D, 61, 0, 0xFC2u, 0, 3, D
#define PPUTLIMPL_UHEX_0x03Cu 60u, 0x03C, 60, 0, 0xFC3u, 0, 3, C
#define PPUTLIMPL_UHEX_0x03Bu 59u, 0x03B, 59, 0, 0xFC4u, 0, 3, B
#define PPUTLIMPL_UHEX_0x03Au 58u, 0x03A, 58, 0, 0xFC5u, 0, 3, A
#define PPUTLIMPL_UHEX_0x039u 57u, 0x039, 57, 0, 0xFC6u, 0, 3, 9
#define PPUTLIMPL_UHEX_0x038u 56u, 0x038, 56, 0, 0xFC7u, 0, 3, 8
#define PPUTLIMPL_UHEX_0x037u 55u, 0x037, 55, 0, 0xFC8u, 0, 3, 7
#define PPUTLIMPL_UHEX_0x036u 54u, 0x036, 54, 0, 0xFC9u, 0, 3, 6
#define PPUTLIMPL_UHEX_0x035u 53u, 0x035, 53, 0, 0xFCAu, 0, 3, 5
#define PPUTLIMPL_UHEX_0x034u 52u, 0x034, 52, 0, 0xFCBu, 0, 3, 4
#define PPUTLIMPL_UHEX_0x033u 51u, 0x033, 51, 0, 0xFCCu, 0, 3, 3
#define PPUTLIMPL_UHEX_0x032u 50u, 0x032, 50, 0, 0xFCDu, 0, 3, 2
#define PPUTLIMPL_UHEX_0x031u 49u, 0x031, 49, 0, 0xFCEu, 0, 3, 1
#define PPUTLIMPL_UHEX_0x030u 48u, 0x030, 48, 0, 0xFCFu, 0, 3, 0
#define PPUTLIMPL_UHEX_0x02Fu 47u, 0x02F, 47, 0, 0xFD0u, 0, 2, F
#define PPUTLIMPL_UHEX_0x02Eu 46u, 0x02E, 46, 0, 0xFD1u, 0, 2, E
#define PPUTLIMPL_UHEX_0x02Du 45u, 0x02D, 45, 0, 0xFD2u, 0, 2, D
#define PPUTLIMPL_UHEX_0x02Cu 44u, 0x02C, 44, 0, 0xFD3u, 0, 2, C
#define PPUTLIMPL_UHEX_0x02Bu 43u, 0x02B, 43, 0, 0xFD4u, 0, 2, B
#define PPUTLIMPL_UHEX_0x02Au 42u, 0x02A, 42, 0, 0xFD5u, 0, 2, A
#define PPUTLIMPL_UHEX_0x029u 41u, 0x029, 41, 0, 0xFD6u, 0, 2, 9
#define PPUTLIMPL_UHEX_0x028u 40u, 0x028, 40, 0, 0xFD7u, 0, 2, 8
#define PPUTLIMPL_UHEX_0x027u 39u, 0x027, 39, 0, 0xFD8u, 0, 2, 7
#define PPUTLIMPL_UHEX_0x026u 38u, 0x026, 38, 0, 0xFD9u, 0, 2, 6
#define PPUTLIMPL_UHEX_0x025u 37u, 0x025, 37, 0, 0xFDAu, 0, 2, 5
#define PPUTLIMPL_UHEX_0x024u 36u, 0x024, 36, 0, 0xFDBu, 0, 2, 4
#define PPUTLIMPL_UHEX_0x023u 35u, 0x023, 35, 0, 0xFDCu, 0, 2, 3
#define PPUTLIMPL_UHEX_0x022u 34u, 0x022, 34, 0, 0xFDDu, 0, 2, 2
#define PPUTLIMPL_UHEX_0x021u 33u, 0x021, 33, 0, 0xFDEu, 0, 2, 1
#define PPUTLIMPL_UHEX_0x020u 32u, 0x020, 32, 0, 0xFDFu, 0, 2, 0
#define PPUTLIMPL_UHEX_0x01Fu 31u, 0x01F, 31, 0, 0xFE0u, 0, 1, F
#define PPUTLIMPL_UHEX_0x01Eu 30u, 0x01E, 30, 0, 0xFE1u, 0, 1, E
#define PPUTLIMPL_UHEX_0x01Du 29u, 0x01D, 29, 0, 0xFE2u, 0, 1, D
#define PPUTLIMPL_UHEX_0x01Cu 28u, 0x01C, 28, 0, 0xFE3u, 0, 1, C
#define PPUTLIMPL_UHEX_0x01Bu 27u, 0x01B, 27, 0, 0xFE4u, 0, 1, B
#define PPUTLIMPL_UHEX_0x01Au 26u, 0x01A, 26, 0, 0xFE5u, 0, 1, A
#define PPUTLIMPL_UHEX_0x019u 25u, 0x019, 25, 0, 0xFE6u, 0, 1, 9
#define PPUTLIMPL_UHEX_0x018u 24u, 0x018, 24, 0, 0xFE7u, 0, 1, 8
#define PPUTLIMPL_UHEX_0x017u 23u, 0x017, 23, 0, 0xFE8u, 0, 1, 7
#define PPUTLIMPL_UHEX_0x016u 22u, 0x016, 22, 0, 0xFE9u, 0, 1, 6
#define PPUTLIMPL_UHEX_0x015u 21u, 0x015, 21, 0, 0xFEAu, 0, 1, 5
#define PPUTLIMPL_UHEX_0x014u 20u, 0x014, 20, 0, 0xFEBu, 0, 1, 4
#define PPUTLIMPL_UHEX_0x013u 19u, 0x013, 19, 0, 0xFECu, 0, 1, 3
#define PPUTLIMPL_UHEX_0x012u 18u, 0x012, 18, 0, 0xFEDu, 0, 1, 2
#define PPUTLIMPL_UHEX_0x011u 17u, 0x011, 17, 0, 0xFEEu, 0, 1, 1
#define PPUTLIMPL_UHEX_0x010u 16u, 0x010, 16, 0, 0xFEFu, 0, 1, 0
#define PPUTLIMPL_UHEX_0x00Fu 15u, 0x00F, 15, 0, 0xFF0u, 0, 0, F
#define PPUTLIMPL_UHEX_0x00Eu 14u, 0x00E, 14, 0, 0xFF1u, 0, 0, E
#define PPUTLIMPL_UHEX_0x00Du 13u, 0x00D, 13, 0, 0xFF2u, 0, 0, D
#define PPUTLIMPL_UHEX_0x00Cu 12u, 0x00C, 12, 0, 0xFF3u, 0, 0, C
#define PPUTLIMPL_UHEX_0x00Bu 11u, 0x00B, 11, 0, 0xFF4u, 0, 0, B
#define PPUTLIMPL_UHEX_0x00Au 10u, 0x00A, 10, 0, 0xFF5u, 0, 0, A
#define PPUTLIMPL_UHEX_0x009u 9u, 0x009, 9, 0, 0xFF6u, 0, 0, 9
#define PPUTLIMPL_UHEX_0x008u 8u, 0x008, 8, 0, 0xFF7u, 0, 0, 8
#define PPUTLIMPL_UHEX_0x007u 7u, 0x007, 7, 0, 0xFF8u, 0, 0, 7
#define PPUTLIMPL_UHEX_0x006u 6u, 0x006, 6, 0, 0xFF9u, 0, 0, 6
#define PPUTLIMPL_UHEX_0x005u 5u, 0x005, 5, 0, 0xFFAu, 0, 0, 5
#define PPUTLIMPL_UHEX_0x004u 4u, 0x004, 4, 0, 0xFFBu, 0, 0, 4
#define PPUTLIMPL_UHEX_0x003u 3u, 0x003, 3, 0, 0xFFCu, 0, 0, 3
#define PPUTLIMPL_UHEX_0x002u 2u, 0x002, 2, 0, 0xFFDu, 0, 0, 2
#define PPUTLIMPL_UHEX_0x001u 1u, 0x001, 1, 0, 0xFFEu, 0, 0, 1
#define PPUTLIMPL_UHEX_0x000u 0u, 0x000, 0, 0, 0xFFFu, 0, 0, 0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [impl.traits.arithhint]
/// -----------------------
/// [internal] two-operand arithmetic cast hint.
///
/// immediately concatenates args.
///
/// returns UDEC|UHEX if either operand is
/// UDEC|UHEX, UDEC|IDEC if either operand
/// is UDEC|IDEC, and UHEX|IHEX otherwise.
#define PPUTLIMPL_ARITHHINT(                                         \
    /* enum<IDEC|IHEX|UDEC|UHEX>, enum<IDEC|IHEX|UDEC|UHEX> */ a, b) \
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

/// [impl.traits.xarithhint]
/// ------------------------
/// [internal] two-operand arithmetic cast hint.
///
/// returns UDEC|UHEX if either operand is
/// UDEC|UHEX, UDEC|IDEC if either operand
/// is UDEC|IDEC, and UHEX|IHEX otherwise.
#define PPUTLIMPL_XARITHHINT(                                      \
    /* enum<IDEC|IHEX|UDEC|UHEX>, enum<IDEC|IHEX|UDEC|UHEX> */...) \
  PPUTLIMPL_ARITHHINT(__VA_ARGS__)

/// [impl.traits.hex_cat]
/// ---------------------
/// [internal] concatenates hexadecimal digits to a uhex expression.
#define PPUTLIMPL_HEX_CAT(/* enum<0|1|...|E|F>... */...) /* -> uhex */ \
  PPUTLIMPL_HEX_CAT_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_HEX_CAT_o(a, b, c) 0x##a##b##c##u

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_list]
/// ----------------
/// list detection; returns 1 in all cases (root type; everything matches).
///
/// PTL_IS_LIST()       // 1
/// PTL_IS_LIST(foo)    // 1
/// PTL_IS_LIST((a, b)) // 1
/// PTL_IS_LIST(a, b)   // 1
/// PTL_IS_LIST(, )     // 1
/// PTL_IS_LIST(, , )   // 1
#define PTL_IS_LIST(/* list */...) /* -> enum<1> */ 1

/// [traits.is_none]
/// ----------------
/// [extends PTL_IS_LIST] detects if args is nothing (an empty list).
///
/// PTL_IS_NONE()          // 1
/// PTL_IS_NONE(foo)       // 0
/// PTL_IS_NONE(foo, bar)  // 0
/// PTL_IS_NONE(PTL_ESC()) // 1
#define PTL_IS_NONE(/* list */...) /* -> bool */ PPUTLIS_NONE_0##__VA_OPT__(1)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_NONE_01 0
#define PPUTLIS_NONE_0  1

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_object]
/// ------------------
/// [extends PTL_IS_LIST] detects if args has exactly one element.
///
/// PTL_IS_OBJECT()         // 0
/// PTL_IS_OBJECT(,)        // 0
/// PTL_IS_OBJECT(foo,)     // 0
/// PTL_IS_OBJECT(foo, bar) // 0
/// PTL_IS_OBJECT(foo)      // 1
/// PTL_IS_OBJECT((42))     // 1
#define PTL_IS_OBJECT(/* list */...) /* -> bool */ \
  PPUTLIS_OBJECT_0##__VA_OPT__(1)(__VA_ARGS__.)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_OBJECT_01(_, ...) PPUTLIS_OBJECT_01_0##__VA_OPT__(1)
#define PPUTLIS_OBJECT_01_01      0
#define PPUTLIS_OBJECT_01_0       1
#define PPUTLIS_OBJECT_0(...)     0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_tuple]
/// -----------------
/// [extends PTL_IS_OBJECT] detects if args is a tuple (parenthesized list).
///
/// PTL_IS_TUPLE()       // 0
/// PTL_IS_TUPLE(1, 2)   // 0
/// PTL_IS_TUPLE(())     // 1
/// PTL_IS_TUPLE((1, 2)) // 1
#define PTL_IS_TUPLE(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_TUPLE_, PTL_IS_OBJECT(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_TUPLE_1         PPUTLIS_TUPLE_o
#define PPUTLIS_TUPLE_0         PPUTLIS_TUPLE_FAIL
#define PPUTLIS_TUPLE_FAIL(...) 0
#define PPUTLIS_TUPLE_o(obj)    PTL_IS_NONE(PTL_EAT obj)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_atom]
/// ----------------
/// [extends PTL_IS_OBJECT] detects if args is a sequence of digit|nondigit tokens.
///
/// this function only tests for nothing, tuples, and multiple values.
///
/// while not testable, the true semantics of atom implies
/// that its values are able to concatenate with identifiers
/// to form new identifiers, meaning that is must match /[\w\d_]+/.
///
/// this property is critical for value-based control flow
/// and must be observed by the user where applicable.
///
/// PTL_IS_ATOM()       // 0
/// PTL_IS_ATOM(foo)    // 1
/// PTL_IS_ATOM(0)      // 1
/// PTL_IS_ATOM(1, 2)   // 0
/// PTL_IS_ATOM(())     // 0
/// PTL_IS_ATOM((1, 2)) // 0
#define PTL_IS_ATOM(/* list */...) /* -> bool */ \
  PPUTLIS_ATOM_0##__VA_OPT__(1)(__VA_ARGS__.)(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_ATOM_01(_, ...) PPUTLIS_ATOM_01_0##__VA_OPT__(1)
#define PPUTLIS_ATOM_01_01      PPUTLIS_ATOM_FAIL
#define PPUTLIS_ATOM_01_0       PPUTLIS_ATOM_o
#define PPUTLIS_ATOM_0(...)     PPUTLIS_ATOM_FAIL
#define PPUTLIS_ATOM_o(obj)     PTL_XCAT(PPUTLIS_ATOM_o_, PTL_IS_NONE(PTL_EAT obj))
#define PPUTLIS_ATOM_o_1        0
#define PPUTLIS_ATOM_o_0        1
#define PPUTLIS_ATOM_FAIL(...)  0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_enum]
/// ----------------
/// [extends PTL_IS_ATOM] detects if args matches a defined enumeration.
/// fails if chkprefix is not an atom.
///
/// to use this function, define a set of
/// macros with the following characteristics:
///  ‐ object-like
///  ‐ common prefix
///  ‐ enum value suffixes
///  ‐ expansion contains a unique word as its first element
/// pass the common prefix as chkprefix.
///
/// example: (identifying an enum<GOOD|BAD>)
///  #define FOO_GOOD 0
///  #define FOO_BAD  1
///  PTL_IS_ENUM(FOO_, BLEH) // 0
///  PTL_IS_ENUM(FOO_, GOOD) // 1
///  PTL_IS_ENUM(FOO_, ,,,)  // 0
#define PTL_IS_ENUM(/* chkprefix: atom, list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_ENUM_, PTL_IS_ATOM(PTL_FIRST(__VA_ARGS__)))    \
  (PTL_STR("chkprefix must be an atom" : PTL_IS_ENUM(__VA_ARGS__)), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_ENUM_1(e, chk_atom, ...) PPUTLIS_ENUM_o(chk_atom, __VA_ARGS__)
#define PPUTLIS_ENUM_0(e, ...)           PTL_FAIL(e)
#define PPUTLIS_ENUM_o(chk_atom, ...) \
  PTL_XCAT(PPUTLIS_ENUM_o_, PTL_IS_ATOM(__VA_ARGS__))(chk_atom, __VA_ARGS__)
#define PPUTLIS_ENUM_o_1 PPUTLIS_ENUM_oo
#define PPUTLIS_ENUM_o_0 PPUTLIS_ENUM_FAIL
#define PPUTLIS_ENUM_oo(chk_atom, atom) \
  PTL_XCAT(PPUTLIS_ENUM_oo_, PTL_IS_OBJECT(PTL_XCAT(chk_atom, atom)))
#define PPUTLIS_ENUM_oo_1      0
#define PPUTLIS_ENUM_oo_0      1
#define PPUTLIS_ENUM_FAIL(...) 0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_idec]
/// ----------------
/// [extends PTL_IS_ENUM] detects if args is an enum<0|1|...|2046|2047>.
///
/// PTL_IS_IDEC(1)      // 1
/// PTL_IS_IDEC(1u)     // 0
/// PTL_IS_IDEC(2047)   // 1
/// PTL_IS_IDEC(4095)   // 0
/// PTL_IS_IDEC(0x000u) // 0
/// PTL_IS_IDEC(0xFFF)  // 0
/// PTL_IS_IDEC((), ()) // 0
#define PTL_IS_IDEC(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_IDEC_, PTL_IS_ATOM(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_IDEC_1         PPUTLIS_IDEC_o
#define PPUTLIS_IDEC_0         PPUTLIS_IDEC_FAIL
#define PPUTLIS_IDEC_FAIL(...) 0
#define PPUTLIS_IDEC_o(atom)   PPUTLIS_IDEC_o_o(atom##u)
#define PPUTLIS_IDEC_o_o(atom) \
  PTL_XCAT(PPUTLIS_IDEC_o_o_, PPUTLIS_ENUM_oo(PPUTLIMPL_UDEC_, atom))(atom)
#define PPUTLIS_IDEC_o_o_1(udec) \
  PTL_XCAT(PPUTLIS_IDEC_o_o_1_, PPUTLIMPL_UHEX(PPUTLIMPL_UDEC(udec, UHEX), ILTZ))
#define PPUTLIS_IDEC_o_o_1_1     0
#define PPUTLIS_IDEC_o_o_1_0     1
#define PPUTLIS_IDEC_o_o_0(atom) 0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_bool]
/// ----------------
/// [extends PTL_IS_ENUM] detects if args is an enum<0|1>.
///
/// PTL_IS_BOOL()      // 0
/// PTL_IS_BOOL(0)     // 1
/// PTL_IS_BOOL(1)     // 1
/// PTL_IS_BOOL(1u)    // 0
/// PTL_IS_BOOL(0x000) // 0
/// PTL_IS_BOOL(0, 1)  // 0
/// PTL_IS_BOOL((0))   // 0
#define PTL_IS_BOOL(/* list */...) /* -> bool */ \
  PPUTLIS_ENUM_o(PPUTLIS_BOOL_, __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_BOOL_1
#define PPUTLIS_BOOL_0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_ihex]
/// ----------------
/// [extends PTL_IS_ENUM] detects if args is an enum<0x000|0x001|...|0xFFE|0xFFF>.
///
/// PTL_IS_IHEX(1)      // 0
/// PTL_IS_IHEX(1u)     // 0
/// PTL_IS_IHEX(0x000)  // 1
/// PTL_IS_IHEX(0xFFF)  // 1
/// PTL_IS_IHEX(0xFFFu) // 0
/// PTL_IS_IHEX((), ()) // 0
#define PTL_IS_IHEX(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_IHEX_, PTL_IS_ATOM(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_IHEX_1         PPUTLIS_IHEX_o
#define PPUTLIS_IHEX_0         PPUTLIS_IHEX_FAIL
#define PPUTLIS_IHEX_FAIL(...) 0
#define PPUTLIS_IHEX_o(atom)   PPUTLIS_ENUM_oo(PPUTLIMPL_UHEX_, atom##u)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_udec]
/// ----------------
/// [extends PTL_IS_ENUM] detects if args is an enum<0u|1u|...|4094u|4095u>.
///
/// PTL_IS_UDEC(1)      // 0
/// PTL_IS_UDEC(1u)     // 1
/// PTL_IS_UDEC(4095)   // 0
/// PTL_IS_UDEC(4095u)  // 1
/// PTL_IS_UDEC(0x000u) // 0
/// PTL_IS_UDEC(0xFFF)  // 0
/// PTL_IS_UDEC((), ()) // 0
#define PTL_IS_UDEC(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_UDEC_, PTL_IS_ATOM(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_UDEC_1         PPUTLIS_UDEC_o
#define PPUTLIS_UDEC_0         PPUTLIS_UDEC_FAIL
#define PPUTLIS_UDEC_FAIL(...) 0
#define PPUTLIS_UDEC_o(atom)   PPUTLIS_ENUM_oo(PPUTLIMPL_UDEC_, atom)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_uhex]
/// ----------------
/// [extends PTL_IS_ENUM] detects if args is an enum<0x000u|0x001u|...|0xFFEu|0xFFFu>.
///
/// PTL_IS_UHEX(1)      // 0
/// PTL_IS_UHEX(1u)     // 0
/// PTL_IS_UHEX(0x000u) // 1
/// PTL_IS_UHEX(0xFFF)  // 0
/// PTL_IS_UHEX((), ()) // 0
#define PTL_IS_UHEX(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_UHEX_, PTL_IS_ATOM(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_UHEX_1         PPUTLIS_UHEX_o
#define PPUTLIS_UHEX_0         PPUTLIS_UHEX_FAIL
#define PPUTLIS_UHEX_FAIL(...) 0
#define PPUTLIS_UHEX_o(atom)   PPUTLIS_ENUM_oo(PPUTLIMPL_UHEX_, atom)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_int]
/// ---------------
/// [extends PTL_IS_IDEC|PTL_IS_IHEX] detects if args is a signed integer.
///
/// PTL_IS_INT()       // 0
/// PTL_IS_INT(foo)    // 0
/// PTL_IS_INT(0)      // 1
/// PTL_IS_INT(0u)     // 0
/// PTL_IS_INT(4095)   // 0
/// PTL_IS_INT(0x000u) // 0
/// PTL_IS_INT(0xFFF)  // 1
/// PTL_IS_INT(0xF)    // 0
/// PTL_IS_INT((), ()) // 0
#define PTL_IS_INT(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_INT_, PTL_IS_ATOM(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_INT_1         PPUTLIS_INT_o
#define PPUTLIS_INT_0         PPUTLIS_INT_FAIL
#define PPUTLIS_INT_FAIL(...) 0
#define PPUTLIS_INT_o(atom) \
  PTL_XCAT(PPUTLIS_INT_o_, PTL_XCAT(PPUTLIS_IDEC_o(atom), PPUTLIS_IHEX_o(atom)))
#define PPUTLIS_INT_o_10 1
#define PPUTLIS_INT_o_01 1
#define PPUTLIS_INT_o_00 0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_uint]
/// ----------------
/// [extends PTL_IS_UDEC|PTL_IS_UHEX] detects if args is an unsigned integer.
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
#define PTL_IS_UINT(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_UINT_, PTL_IS_ATOM(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_UINT_1         PPUTLIS_UINT_o
#define PPUTLIS_UINT_0         PPUTLIS_UINT_FAIL
#define PPUTLIS_UINT_FAIL(...) 0
#define PPUTLIS_UINT_o(atom)                                                 \
  PTL_XCAT(PPUTLIS_UINT_o_, PTL_XCAT(PPUTLIS_ENUM_oo(PPUTLIMPL_UDEC_, atom), \
                                     PPUTLIS_ENUM_oo(PPUTLIMPL_UHEX_, atom)))
#define PPUTLIS_UINT_o_10 1
#define PPUTLIS_UINT_o_01 1
#define PPUTLIS_UINT_o_00 0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_word]
/// ----------------
/// [extends PTL_IS_INT|PTL_IS_UINT] detects if args is an integer.
///
/// PTL_IS_WORD(0)      // 1
/// PTL_IS_WORD(0u)     // 1
/// PTL_IS_WORD(foo)    // 0
/// PTL_IS_WORD(())     // 0
/// PTL_IS_WORD(A)      // 0
/// PTL_IS_WORD(0x800)  // 1
/// PTL_IS_WORD(4095u)  // 1
/// PTL_IS_WORD(0xFFFu) // 1
#define PTL_IS_WORD(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_WORD_, PTL_IS_ATOM(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_WORD_1         PPUTLIS_WORD_o
#define PPUTLIS_WORD_0         PPUTLIS_WORD_FAIL
#define PPUTLIS_WORD_FAIL(...) 0
#define PPUTLIS_WORD_o(atom)   PTL_XCAT(PPUTLIS_WORD_o_, PPUTLIS_INT_o(atom))(atom)
#define PPUTLIS_WORD_o_1(int)  1
#define PPUTLIS_WORD_o_0(atom)                                                 \
  PTL_XCAT(PPUTLIS_WORD_o_0_, PTL_XCAT(PPUTLIS_ENUM_oo(PPUTLIMPL_UDEC_, atom), \
                                       PPUTLIS_ENUM_oo(PPUTLIMPL_UHEX_, atom)))
#define PPUTLIS_WORD_o_0_10 1
#define PPUTLIS_WORD_o_0_01 1
#define PPUTLIS_WORD_o_0_00 0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_size]
/// ----------------
/// [extends PTL_IS_WORD] detects if args is any non-negative word up to PTL_SIZE_MAX.
///
/// PTL_IS_SIZE(0)      // 1
/// PTL_IS_SIZE(0u)     // 1
/// PTL_IS_SIZE(foo)    // 0
/// PTL_IS_SIZE(())     // 0
/// PTL_IS_SIZE(A)      // 0
/// PTL_IS_SIZE(0x800)  // 0
/// PTL_IS_SIZE(4095u)  // 0
/// PTL_IS_SIZE(0xFFFu) // 0
/// PTL_IS_SIZE(255u)   // 1
#define PTL_IS_SIZE(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_SIZE_, PTL_IS_ATOM(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_SIZE_1         PPUTLIS_SIZE_o
#define PPUTLIS_SIZE_0         PPUTLIS_SIZE_FAIL
#define PPUTLIS_SIZE_FAIL(...) 0
#define PPUTLIS_SIZE_o(atom)                                              \
  PTL_XCAT(PPUTLIS_SIZE_o_,                                               \
           PTL_XCAT(PTL_XCAT(PPUTLIS_IDEC_o(atom), PPUTLIS_IHEX_o(atom)), \
                    PTL_XCAT(PPUTLIS_ENUM_oo(PPUTLIMPL_UDEC_, atom),      \
                             PPUTLIS_ENUM_oo(PPUTLIMPL_UHEX_, atom))))    \
  (atom)
#define PPUTLIS_SIZE_o_1000(idec) PPUTLIMPL_UDEC(idec##u, ISIZE)
#define PPUTLIS_SIZE_o_0100(ihex) PPUTLIMPL_UDEC(PPUTLIMPL_UHEX(ihex##u, UDEC), ISIZE)
#define PPUTLIS_SIZE_o_0010(udec) PPUTLIMPL_UDEC(udec, USIZE)
#define PPUTLIS_SIZE_o_0001(uhex) PPUTLIMPL_UDEC(PPUTLIMPL_UHEX(uhex, UDEC), USIZE)
#define PPUTLIS_SIZE_o_0000(atom) 0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_offset]
/// ------------------
/// [extends PTL_IS_WORD] detects if args is any word whose abs is a valid size.
///
/// PTL_IS_OFFSET(0)     // 1
/// PTL_IS_OFFSET(254)   // 1
/// PTL_IS_OFFSET(255u)  // 1
/// PTL_IS_OFFSET(256u)  // 0
/// PTL_IS_OFFSET(0xFFF) // 1
/// PTL_IS_OFFSET(0xF01) // 1
/// PTL_IS_OFFSET(0xF00) // 0
#define PTL_IS_OFFSET(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_OFFSET_, PTL_IS_ATOM(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_OFFSET_1         PPUTLIS_OFFSET_o
#define PPUTLIS_OFFSET_0         PPUTLIS_OFFSET_FAIL
#define PPUTLIS_OFFSET_FAIL(...) 0
#define PPUTLIS_OFFSET_o(atom)                                            \
  PTL_XCAT(PPUTLIS_OFFSET_o_,                                             \
           PTL_XCAT(PTL_XCAT(PPUTLIS_IDEC_o(atom), PPUTLIS_IHEX_o(atom)), \
                    PTL_XCAT(PPUTLIS_ENUM_oo(PPUTLIMPL_UDEC_, atom),      \
                             PPUTLIS_ENUM_oo(PPUTLIMPL_UHEX_, atom))))    \
  (atom)
#define PPUTLIS_OFFSET_o_1000(idec) PPUTLIMPL_UDEC(idec##u, IOFS)
#define PPUTLIS_OFFSET_o_0100(ihex) PPUTLIMPL_UDEC(PPUTLIMPL_UHEX(ihex##u, UDEC), IOFS)
#define PPUTLIS_OFFSET_o_0010(udec) PPUTLIMPL_UDEC(udec, UOFS)
#define PPUTLIS_OFFSET_o_0001(uhex) PPUTLIMPL_UDEC(PPUTLIMPL_UHEX(uhex, UDEC), UOFS)
#define PPUTLIS_OFFSET_o_0000(atom) 0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_array]
/// -----------------
/// [extends PTL_IS_OBJECT] detects if args is a pputl array object.
/// note: does not parse contained items during validity check.
///
/// PTL_IS_ARRAY()             // 0
/// PTL_IS_ARRAY(1, 2)         // 0
/// PTL_IS_ARRAY((a, b))       // 0
/// PTL_IS_ARRAY(PTL_ARR())    // 1
/// PTL_IS_ARRAY(PTL_MAP())    // 1
/// PTL_IS_ARRAY(PTL_PQUEUE()) // 1
/// PTL_IS_ARRAY(PTL_SET())    // 1
/// PTL_IS_ARRAY(PTL_STACK())  // 1
/// PTL_IS_ARRAY(PTL_QUEUE())  // 1
#define PTL_IS_ARRAY(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_ARRAY_, PTL_IS_OBJECT(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_ARRAY_1        PPUTLIS_ARRAY_o
#define PPUTLIS_ARRAY_0        PPUTLIS_ARRAY_FAIL
#define PPUTLIS_ARRAY_o(obj)   PTL_XCAT(PPUTLIS_ARRAY_o_, PPUTLIS_TUPLE_o(obj))(obj)
#define PPUTLIS_ARRAY_o_1      PPUTLIS_ARRAY_FAIL
#define PPUTLIS_ARRAY_o_0      PPUTLIS_ARRAY_oo
#define PPUTLIS_ARRAY_oo(atom) PTL_IS_NONE(PPUTLIS_ARRAY_oo_##atom)
#define PPUTLIS_ARRAY_oo_PTL_QUEUE(...)
#define PPUTLIS_ARRAY_oo_PTL_STACK(...)
#define PPUTLIS_ARRAY_oo_PTL_SET(...)
#define PPUTLIS_ARRAY_oo_PTL_PQUEUE(...)
#define PPUTLIS_ARRAY_oo_PTL_MAP(...)
#define PPUTLIS_ARRAY_oo_PTL_ARR(...)
#define PPUTLIS_ARRAY_FAIL(...) 0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_map]
/// ---------------
/// [extends PTL_IS_OBJECT] detects if args is a pputl map object.
/// note: does not parse contained items during validity check.
///
/// PTL_IS_MAP()          // 0
/// PTL_IS_MAP(1, 2)      // 0
/// PTL_IS_MAP(PTL_MAP()) // 1
#define PTL_IS_MAP(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_MAP_, PTL_IS_OBJECT(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_MAP_1                         PPUTLIS_MAP_o
#define PPUTLIS_MAP_0                         PPUTLIS_MAP_FAIL
#define PPUTLIS_MAP_o(obj)                    PTL_XCAT(PPUTLIS_MAP_o_, PPUTLIS_TUPLE_o(obj))(obj)
#define PPUTLIS_MAP_o_1                       PPUTLIS_MAP_FAIL
#define PPUTLIS_MAP_o_0                       PPUTLIS_MAP_oo
#define PPUTLIS_MAP_oo(/* <non tuple> */ obj) PTL_IS_NONE(PPUTLIS_MAP_CHK_##obj)
#define PPUTLIS_MAP_FAIL(...)                 0
#define PPUTLIS_MAP_CHK_PTL_MAP(...)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_pqueue]
/// ------------------
/// [extends PTL_IS_OBJECT] detects if args is a pputl priority queue object.
///
/// PTL_IS_PQUEUE()             // 0
/// PTL_IS_PQUEUE(1, 2)         // 0
/// PTL_IS_PQUEUE(PTL_PQUEUE()) // 1
#define PTL_IS_PQUEUE(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_PQUEUE_, PTL_IS_OBJECT(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_PQUEUE_1 PPUTLIS_PQUEUE_o
#define PPUTLIS_PQUEUE_0 PPUTLIS_PQUEUE_FAIL

#define PPUTLIS_PQUEUE_o(obj) PTL_XCAT(PPUTLIS_PQUEUE_o_, PPUTLIS_TUPLE_o(obj))(obj)

#define PPUTLIS_PQUEUE_o_1                       PPUTLIS_PQUEUE_FAIL
#define PPUTLIS_PQUEUE_o_0                       PPUTLIS_PQUEUE_oo
#define PPUTLIS_PQUEUE_oo(/* <non tuple> */ obj) PTL_IS_NONE(PPUTLIS_PQUEUE_CHK_##obj)
#define PPUTLIS_PQUEUE_FAIL(...)                 0
#define PPUTLIS_PQUEUE_CHK_PTL_PQUEUE(...)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_set]
/// ---------------
/// [extends PTL_IS_OBJECT] detects if args is a pputl set object.
///
/// PTL_IS_SET()          // 0
/// PTL_IS_SET(1, 2)      // 0
/// PTL_IS_SET(PTL_SET()) // 1
#define PTL_IS_SET(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_SET_, PTL_IS_OBJECT(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_SET_1                         PPUTLIS_SET_o
#define PPUTLIS_SET_0                         PPUTLIS_SET_FAIL
#define PPUTLIS_SET_o(obj)                    PTL_XCAT(PPUTLIS_SET_o_, PPUTLIS_TUPLE_o(obj))(obj)
#define PPUTLIS_SET_o_1                       PPUTLIS_SET_FAIL
#define PPUTLIS_SET_o_0                       PPUTLIS_SET_oo
#define PPUTLIS_SET_oo(/* <non tuple> */ obj) PTL_IS_NONE(PPUTLIS_SET_CHK_##obj)
#define PPUTLIS_SET_FAIL(...)                 0
#define PPUTLIS_SET_CHK_PTL_SET(...)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_stack]
/// -----------------
/// [extends PTL_IS_OBJECT] detects if args is a pputl stack object.
///
/// PTL_IS_STACK()            // 0
/// PTL_IS_STACK(1, 2)        // 0
/// PTL_IS_STACK(PTL_STACK()) // 1
#define PTL_IS_STACK(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_STACK_, PTL_IS_OBJECT(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_STACK_1 PPUTLIS_STACK_o
#define PPUTLIS_STACK_0 PPUTLIS_STACK_FAIL

#define PPUTLIS_STACK_o(obj) PTL_XCAT(PPUTLIS_STACK_o_, PPUTLIS_TUPLE_o(obj))(obj)

#define PPUTLIS_STACK_o_1                       PPUTLIS_STACK_FAIL
#define PPUTLIS_STACK_o_0                       PPUTLIS_STACK_oo
#define PPUTLIS_STACK_oo(/* <non tuple> */ obj) PTL_IS_NONE(PPUTLIS_STACK_CHK_##obj)
#define PPUTLIS_STACK_FAIL(...)                 0
#define PPUTLIS_STACK_CHK_PTL_STACK(...)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_queue]
/// -----------------
/// [extends PTL_IS_OBJECT] detects if args is a pputl queue object.
///
/// PTL_IS_QUEUE()            // 0
/// PTL_IS_QUEUE(1, 2)        // 0
/// PTL_IS_QUEUE(PTL_QUEUE()) // 1
#define PTL_IS_QUEUE(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_QUEUE_, PTL_IS_OBJECT(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_QUEUE_1 PPUTLIS_QUEUE_o
#define PPUTLIS_QUEUE_0 PPUTLIS_QUEUE_FAIL

#define PPUTLIS_QUEUE_o(obj) PTL_XCAT(PPUTLIS_QUEUE_o_, PPUTLIS_TUPLE_o(obj))(obj)

#define PPUTLIS_QUEUE_o_1                       PPUTLIS_QUEUE_FAIL
#define PPUTLIS_QUEUE_o_0                       PPUTLIS_QUEUE_oo
#define PPUTLIS_QUEUE_oo(/* <non tuple> */ obj) PTL_IS_NONE(PPUTLIS_QUEUE_CHK_##obj)
#define PPUTLIS_QUEUE_FAIL(...)                 0
#define PPUTLIS_QUEUE_CHK_PTL_QUEUE(...)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_range]
/// -----------------
/// [extends PTL_IS_TUPLE|PTL_IS_ARRAY] detects if args is a structured range of any.
///
/// PTL_IS_RANGE()             // 0
/// PTL_IS_RANGE(foo)          // 0
/// PTL_IS_RANGE((foo))        // 1
/// PTL_IS_RANGE(PTL_ARR())    // 1
/// PTL_IS_RANGE(PTL_MAP())    // 1
/// PTL_IS_RANGE(PTL_PQUEUE()) // 1
/// PTL_IS_RANGE(PTL_SET())    // 1
/// PTL_IS_RANGE(PTL_STACK())  // 1
/// PTL_IS_RANGE(PTL_QUEUE())  // 1
#define PTL_IS_RANGE(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_RANGE_, PTL_IS_OBJECT(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_RANGE_1         PPUTLIS_RANGE_o
#define PPUTLIS_RANGE_0         PPUTLIS_RANGE_FAIL
#define PPUTLIS_RANGE_FAIL(...) 0
#define PPUTLIS_RANGE_o(object) \
  PTL_XCAT(PPUTLIS_RANGE_o_, PPUTLIS_TUPLE_o(object))(object)
#define PPUTLIS_RANGE_o_1(tuple) 1
#define PPUTLIS_RANGE_o_0(atom)  PPUTLIS_ARRAY_oo(atom)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_any]
/// ---------------
/// [extends PTL_IS_NONE|PTL_IS_OBJECT] detects if args is a list with no separatory
/// commas.
///
/// PTL_IS_ANY()       // 1
/// PTL_IS_ANY(foo)    // 1
/// PTL_IS_ANY((a, b)) // 1
/// PTL_IS_ANY(a, b)   // 0
/// PTL_IS_ANY(, )     // 0
/// PTL_IS_ANY(, , )   // 0
#define PTL_IS_ANY(/* list */...) /* -> bool */ PPUTLIS_ANY_o(__VA_ARGS__.)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_ANY_o(_, ...) PPUTLIS_ANY_o_0##__VA_OPT__(1)
#define PPUTLIS_ANY_o_01      0
#define PPUTLIS_ANY_o_0       1

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.typeof]
/// ---------------
/// detects the most appropriate value type.
///
/// warn: must be one of the defined value types.
///       inputs such as `-`, `"`, `foo()`, etc., are not
///       detectable and will cause an unexpected pp error.
///
/// returns one of:
///
///  | NONE  | LIST  | TUPLE | ARRAY | MAP  | PQUEUE | SET
///  | STACK | QUEUE | ATOM  | IDEC  | IHEX | UDEC   | UHEX
///
/// PTL_TYPEOF((foo))        // TUPLE
/// PTL_TYPEOF(0)            // IDEC
/// PTL_TYPEOF(0u)           // UDEC
/// PTL_TYPEOF(4095)         // ATOM
/// PTL_TYPEOF(4095u)        // UDEC
/// PTL_TYPEOF(0xFFF)        // IHEX
/// PTL_TYPEOF(0xFFFu)       // UHEX
/// PTL_TYPEOF(foo)          // ATOM
/// PTL_TYPEOF(001)          // ATOM
/// PTL_TYPEOF(foo, bar)     // LIST
/// PTL_TYPEOF()             // NONE
/// PTL_TYPEOF(PTL_ARR())    // ARRAY
/// PTL_TYPEOF(PTL_MAP())    // MAP
/// PTL_TYPEOF(PTL_PQUEUE()) // PQUEUE
/// PTL_TYPEOF(PTL_SET())    // SET
/// PTL_TYPEOF(PTL_STACK())  // STACK
/// PTL_TYPEOF(PTL_QUEUE())  // QUEUE
#define PTL_TYPEOF(                                                                            \
    /* list */                                                                                 \
    ...)    /* ->                                                                              \
               enum<NONE|LIST|TUPLE|ARRAY|MAP|PQUEUE|SET|STACK|QUEUE|ATOM|IDEC|IHEX|UDEC|UHEX> \
             */                                                                                \
  PTL_XCAT(PPUTLTYPEOF_, PTL_IS_NONE(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

/// none
#define PPUTLTYPEOF_1(...) NONE
/// !none
#define PPUTLTYPEOF_0(...) \
  PTL_XCAT(PPUTLTYPEOF_0_, PTL_IS_OBJECT(__VA_ARGS__))(__VA_ARGS__)
/// !none → obj
#define PPUTLTYPEOF_0_1(obj)                PTL_XCAT(PPUTLTYPEOF_0_1_, PPUTLIS_TUPLE_o(obj))(obj)
/// !none → obj → tuple
#define PPUTLTYPEOF_0_1_1(tuple)            TUPLE
/// !none → obj → !tuple
#define PPUTLTYPEOF_0_1_0(atom)             PTL_XCAT(PPUTLTYPEOF_0_1_0_, PPUTLIS_ARRAY_oo(atom))(atom)
/// !none → obj → !tuple → array
#define PPUTLTYPEOF_0_1_0_1(array)          PPUTLTYPEOF_0_1_0_1_##array
#define PPUTLTYPEOF_0_1_0_1_PTL_QUEUE(...)  QUEUE
#define PPUTLTYPEOF_0_1_0_1_PTL_STACK(...)  STACK
#define PPUTLTYPEOF_0_1_0_1_PTL_SET(...)    SET
#define PPUTLTYPEOF_0_1_0_1_PTL_PQUEUE(...) PQUEUE
#define PPUTLTYPEOF_0_1_0_1_PTL_MAP(...)    MAP
#define PPUTLTYPEOF_0_1_0_1_PTL_ARR(...)    ARRAY
/// !none → obj → !tuple → !array
#define PPUTLTYPEOF_0_1_0_0(atom)                                         \
  PTL_XCAT(PPUTLTYPEOF_0_1_0_0_,                                          \
           PTL_XCAT(PTL_XCAT(PPUTLIS_IDEC_o(atom), PPUTLIS_IHEX_o(atom)), \
                    PTL_XCAT(PPUTLIS_UDEC_o(atom), PPUTLIS_UHEX_o(atom))))
#define PPUTLTYPEOF_0_1_0_0_1000 IDEC
#define PPUTLTYPEOF_0_1_0_0_0100 IHEX
#define PPUTLTYPEOF_0_1_0_0_0010 UDEC
#define PPUTLTYPEOF_0_1_0_0_0001 UHEX
#define PPUTLTYPEOF_0_1_0_0_0000 ATOM
/// !none → !obj
#define PPUTLTYPEOF_0_0(...)     LIST

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.countof]
/// ----------------
/// counts the number of arguments.
/// fails if larger than PTL_SIZE_MAX (255u)
///
/// PTL_COUNTOF()     // 0u
/// PTL_COUNTOF(a)    // 1u
/// PTL_COUNTOF(a, b) // 2u
/// PTL_COUNTOF(, )   // 2u
#define PTL_COUNTOF(/* list */...) /* -> udec&size */ \
  PPUTLCOUNTOF_o(PTL_STR("[PTL_COUNTOF] too many arguments" : __VA_ARGS__), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLCOUNTOF_o(e, ...) \
  PTL_XFIRST(__VA_OPT__(PPUTLCOUNTOF_N1(e, __VA_ARGS__.), ) 0u)
#define PPUTLCOUNTOF_N1(e, _, ...)   __VA_OPT__(PPUTLCOUNTOF_N2(e, __VA_ARGS__), ) 1u
#define PPUTLCOUNTOF_N2(e, _, ...)   __VA_OPT__(PPUTLCOUNTOF_N3(e, __VA_ARGS__), ) 2u
#define PPUTLCOUNTOF_N3(e, _, ...)   __VA_OPT__(PPUTLCOUNTOF_N4(e, __VA_ARGS__), ) 3u
#define PPUTLCOUNTOF_N4(e, _, ...)   __VA_OPT__(PPUTLCOUNTOF_N5(e, __VA_ARGS__), ) 4u
#define PPUTLCOUNTOF_N5(e, _, ...)   __VA_OPT__(PPUTLCOUNTOF_N6(e, __VA_ARGS__), ) 5u
#define PPUTLCOUNTOF_N6(e, _, ...)   __VA_OPT__(PPUTLCOUNTOF_N7(e, __VA_ARGS__), ) 6u
#define PPUTLCOUNTOF_N7(e, _, ...)   __VA_OPT__(PPUTLCOUNTOF_N8(e, __VA_ARGS__), ) 7u
#define PPUTLCOUNTOF_N8(e, _, ...)   __VA_OPT__(PPUTLCOUNTOF_N9(e, __VA_ARGS__), ) 8u
#define PPUTLCOUNTOF_N9(e, _, ...)   __VA_OPT__(PPUTLCOUNTOF_N10(e, __VA_ARGS__), ) 9u
#define PPUTLCOUNTOF_N10(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N11(e, __VA_ARGS__), ) 10u
#define PPUTLCOUNTOF_N11(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N12(e, __VA_ARGS__), ) 11u
#define PPUTLCOUNTOF_N12(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N13(e, __VA_ARGS__), ) 12u
#define PPUTLCOUNTOF_N13(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N14(e, __VA_ARGS__), ) 13u
#define PPUTLCOUNTOF_N14(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N15(e, __VA_ARGS__), ) 14u
#define PPUTLCOUNTOF_N15(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N16(e, __VA_ARGS__), ) 15u
#define PPUTLCOUNTOF_N16(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N17(e, __VA_ARGS__), ) 16u
#define PPUTLCOUNTOF_N17(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N18(e, __VA_ARGS__), ) 17u
#define PPUTLCOUNTOF_N18(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N19(e, __VA_ARGS__), ) 18u
#define PPUTLCOUNTOF_N19(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N20(e, __VA_ARGS__), ) 19u
#define PPUTLCOUNTOF_N20(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N21(e, __VA_ARGS__), ) 20u
#define PPUTLCOUNTOF_N21(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N22(e, __VA_ARGS__), ) 21u
#define PPUTLCOUNTOF_N22(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N23(e, __VA_ARGS__), ) 22u
#define PPUTLCOUNTOF_N23(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N24(e, __VA_ARGS__), ) 23u
#define PPUTLCOUNTOF_N24(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N25(e, __VA_ARGS__), ) 24u
#define PPUTLCOUNTOF_N25(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N26(e, __VA_ARGS__), ) 25u
#define PPUTLCOUNTOF_N26(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N27(e, __VA_ARGS__), ) 26u
#define PPUTLCOUNTOF_N27(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N28(e, __VA_ARGS__), ) 27u
#define PPUTLCOUNTOF_N28(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N29(e, __VA_ARGS__), ) 28u
#define PPUTLCOUNTOF_N29(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N30(e, __VA_ARGS__), ) 29u
#define PPUTLCOUNTOF_N30(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N31(e, __VA_ARGS__), ) 30u
#define PPUTLCOUNTOF_N31(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N32(e, __VA_ARGS__), ) 31u
#define PPUTLCOUNTOF_N32(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N33(e, __VA_ARGS__), ) 32u
#define PPUTLCOUNTOF_N33(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N34(e, __VA_ARGS__), ) 33u
#define PPUTLCOUNTOF_N34(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N35(e, __VA_ARGS__), ) 34u
#define PPUTLCOUNTOF_N35(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N36(e, __VA_ARGS__), ) 35u
#define PPUTLCOUNTOF_N36(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N37(e, __VA_ARGS__), ) 36u
#define PPUTLCOUNTOF_N37(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N38(e, __VA_ARGS__), ) 37u
#define PPUTLCOUNTOF_N38(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N39(e, __VA_ARGS__), ) 38u
#define PPUTLCOUNTOF_N39(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N40(e, __VA_ARGS__), ) 39u
#define PPUTLCOUNTOF_N40(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N41(e, __VA_ARGS__), ) 40u
#define PPUTLCOUNTOF_N41(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N42(e, __VA_ARGS__), ) 41u
#define PPUTLCOUNTOF_N42(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N43(e, __VA_ARGS__), ) 42u
#define PPUTLCOUNTOF_N43(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N44(e, __VA_ARGS__), ) 43u
#define PPUTLCOUNTOF_N44(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N45(e, __VA_ARGS__), ) 44u
#define PPUTLCOUNTOF_N45(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N46(e, __VA_ARGS__), ) 45u
#define PPUTLCOUNTOF_N46(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N47(e, __VA_ARGS__), ) 46u
#define PPUTLCOUNTOF_N47(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N48(e, __VA_ARGS__), ) 47u
#define PPUTLCOUNTOF_N48(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N49(e, __VA_ARGS__), ) 48u
#define PPUTLCOUNTOF_N49(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N50(e, __VA_ARGS__), ) 49u
#define PPUTLCOUNTOF_N50(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N51(e, __VA_ARGS__), ) 50u
#define PPUTLCOUNTOF_N51(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N52(e, __VA_ARGS__), ) 51u
#define PPUTLCOUNTOF_N52(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N53(e, __VA_ARGS__), ) 52u
#define PPUTLCOUNTOF_N53(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N54(e, __VA_ARGS__), ) 53u
#define PPUTLCOUNTOF_N54(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N55(e, __VA_ARGS__), ) 54u
#define PPUTLCOUNTOF_N55(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N56(e, __VA_ARGS__), ) 55u
#define PPUTLCOUNTOF_N56(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N57(e, __VA_ARGS__), ) 56u
#define PPUTLCOUNTOF_N57(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N58(e, __VA_ARGS__), ) 57u
#define PPUTLCOUNTOF_N58(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N59(e, __VA_ARGS__), ) 58u
#define PPUTLCOUNTOF_N59(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N60(e, __VA_ARGS__), ) 59u
#define PPUTLCOUNTOF_N60(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N61(e, __VA_ARGS__), ) 60u
#define PPUTLCOUNTOF_N61(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N62(e, __VA_ARGS__), ) 61u
#define PPUTLCOUNTOF_N62(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N63(e, __VA_ARGS__), ) 62u
#define PPUTLCOUNTOF_N63(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N64(e, __VA_ARGS__), ) 63u
#define PPUTLCOUNTOF_N64(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N65(e, __VA_ARGS__), ) 64u
#define PPUTLCOUNTOF_N65(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N66(e, __VA_ARGS__), ) 65u
#define PPUTLCOUNTOF_N66(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N67(e, __VA_ARGS__), ) 66u
#define PPUTLCOUNTOF_N67(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N68(e, __VA_ARGS__), ) 67u
#define PPUTLCOUNTOF_N68(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N69(e, __VA_ARGS__), ) 68u
#define PPUTLCOUNTOF_N69(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N70(e, __VA_ARGS__), ) 69u
#define PPUTLCOUNTOF_N70(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N71(e, __VA_ARGS__), ) 70u
#define PPUTLCOUNTOF_N71(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N72(e, __VA_ARGS__), ) 71u
#define PPUTLCOUNTOF_N72(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N73(e, __VA_ARGS__), ) 72u
#define PPUTLCOUNTOF_N73(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N74(e, __VA_ARGS__), ) 73u
#define PPUTLCOUNTOF_N74(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N75(e, __VA_ARGS__), ) 74u
#define PPUTLCOUNTOF_N75(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N76(e, __VA_ARGS__), ) 75u
#define PPUTLCOUNTOF_N76(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N77(e, __VA_ARGS__), ) 76u
#define PPUTLCOUNTOF_N77(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N78(e, __VA_ARGS__), ) 77u
#define PPUTLCOUNTOF_N78(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N79(e, __VA_ARGS__), ) 78u
#define PPUTLCOUNTOF_N79(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N80(e, __VA_ARGS__), ) 79u
#define PPUTLCOUNTOF_N80(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N81(e, __VA_ARGS__), ) 80u
#define PPUTLCOUNTOF_N81(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N82(e, __VA_ARGS__), ) 81u
#define PPUTLCOUNTOF_N82(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N83(e, __VA_ARGS__), ) 82u
#define PPUTLCOUNTOF_N83(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N84(e, __VA_ARGS__), ) 83u
#define PPUTLCOUNTOF_N84(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N85(e, __VA_ARGS__), ) 84u
#define PPUTLCOUNTOF_N85(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N86(e, __VA_ARGS__), ) 85u
#define PPUTLCOUNTOF_N86(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N87(e, __VA_ARGS__), ) 86u
#define PPUTLCOUNTOF_N87(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N88(e, __VA_ARGS__), ) 87u
#define PPUTLCOUNTOF_N88(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N89(e, __VA_ARGS__), ) 88u
#define PPUTLCOUNTOF_N89(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N90(e, __VA_ARGS__), ) 89u
#define PPUTLCOUNTOF_N90(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N91(e, __VA_ARGS__), ) 90u
#define PPUTLCOUNTOF_N91(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N92(e, __VA_ARGS__), ) 91u
#define PPUTLCOUNTOF_N92(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N93(e, __VA_ARGS__), ) 92u
#define PPUTLCOUNTOF_N93(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N94(e, __VA_ARGS__), ) 93u
#define PPUTLCOUNTOF_N94(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N95(e, __VA_ARGS__), ) 94u
#define PPUTLCOUNTOF_N95(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N96(e, __VA_ARGS__), ) 95u
#define PPUTLCOUNTOF_N96(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N97(e, __VA_ARGS__), ) 96u
#define PPUTLCOUNTOF_N97(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N98(e, __VA_ARGS__), ) 97u
#define PPUTLCOUNTOF_N98(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N99(e, __VA_ARGS__), ) 98u
#define PPUTLCOUNTOF_N99(e, _, ...)  __VA_OPT__(PPUTLCOUNTOF_N100(e, __VA_ARGS__), ) 99u
#define PPUTLCOUNTOF_N100(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N101(e, __VA_ARGS__), ) 100u
#define PPUTLCOUNTOF_N101(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N102(e, __VA_ARGS__), ) 101u
#define PPUTLCOUNTOF_N102(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N103(e, __VA_ARGS__), ) 102u
#define PPUTLCOUNTOF_N103(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N104(e, __VA_ARGS__), ) 103u
#define PPUTLCOUNTOF_N104(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N105(e, __VA_ARGS__), ) 104u
#define PPUTLCOUNTOF_N105(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N106(e, __VA_ARGS__), ) 105u
#define PPUTLCOUNTOF_N106(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N107(e, __VA_ARGS__), ) 106u
#define PPUTLCOUNTOF_N107(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N108(e, __VA_ARGS__), ) 107u
#define PPUTLCOUNTOF_N108(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N109(e, __VA_ARGS__), ) 108u
#define PPUTLCOUNTOF_N109(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N110(e, __VA_ARGS__), ) 109u
#define PPUTLCOUNTOF_N110(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N111(e, __VA_ARGS__), ) 110u
#define PPUTLCOUNTOF_N111(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N112(e, __VA_ARGS__), ) 111u
#define PPUTLCOUNTOF_N112(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N113(e, __VA_ARGS__), ) 112u
#define PPUTLCOUNTOF_N113(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N114(e, __VA_ARGS__), ) 113u
#define PPUTLCOUNTOF_N114(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N115(e, __VA_ARGS__), ) 114u
#define PPUTLCOUNTOF_N115(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N116(e, __VA_ARGS__), ) 115u
#define PPUTLCOUNTOF_N116(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N117(e, __VA_ARGS__), ) 116u
#define PPUTLCOUNTOF_N117(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N118(e, __VA_ARGS__), ) 117u
#define PPUTLCOUNTOF_N118(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N119(e, __VA_ARGS__), ) 118u
#define PPUTLCOUNTOF_N119(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N120(e, __VA_ARGS__), ) 119u
#define PPUTLCOUNTOF_N120(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N121(e, __VA_ARGS__), ) 120u
#define PPUTLCOUNTOF_N121(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N122(e, __VA_ARGS__), ) 121u
#define PPUTLCOUNTOF_N122(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N123(e, __VA_ARGS__), ) 122u
#define PPUTLCOUNTOF_N123(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N124(e, __VA_ARGS__), ) 123u
#define PPUTLCOUNTOF_N124(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N125(e, __VA_ARGS__), ) 124u
#define PPUTLCOUNTOF_N125(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N126(e, __VA_ARGS__), ) 125u
#define PPUTLCOUNTOF_N126(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N127(e, __VA_ARGS__), ) 126u
#define PPUTLCOUNTOF_N127(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N128(e, __VA_ARGS__), ) 127u
#define PPUTLCOUNTOF_N128(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N129(e, __VA_ARGS__), ) 128u
#define PPUTLCOUNTOF_N129(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N130(e, __VA_ARGS__), ) 129u
#define PPUTLCOUNTOF_N130(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N131(e, __VA_ARGS__), ) 130u
#define PPUTLCOUNTOF_N131(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N132(e, __VA_ARGS__), ) 131u
#define PPUTLCOUNTOF_N132(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N133(e, __VA_ARGS__), ) 132u
#define PPUTLCOUNTOF_N133(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N134(e, __VA_ARGS__), ) 133u
#define PPUTLCOUNTOF_N134(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N135(e, __VA_ARGS__), ) 134u
#define PPUTLCOUNTOF_N135(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N136(e, __VA_ARGS__), ) 135u
#define PPUTLCOUNTOF_N136(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N137(e, __VA_ARGS__), ) 136u
#define PPUTLCOUNTOF_N137(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N138(e, __VA_ARGS__), ) 137u
#define PPUTLCOUNTOF_N138(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N139(e, __VA_ARGS__), ) 138u
#define PPUTLCOUNTOF_N139(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N140(e, __VA_ARGS__), ) 139u
#define PPUTLCOUNTOF_N140(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N141(e, __VA_ARGS__), ) 140u
#define PPUTLCOUNTOF_N141(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N142(e, __VA_ARGS__), ) 141u
#define PPUTLCOUNTOF_N142(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N143(e, __VA_ARGS__), ) 142u
#define PPUTLCOUNTOF_N143(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N144(e, __VA_ARGS__), ) 143u
#define PPUTLCOUNTOF_N144(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N145(e, __VA_ARGS__), ) 144u
#define PPUTLCOUNTOF_N145(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N146(e, __VA_ARGS__), ) 145u
#define PPUTLCOUNTOF_N146(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N147(e, __VA_ARGS__), ) 146u
#define PPUTLCOUNTOF_N147(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N148(e, __VA_ARGS__), ) 147u
#define PPUTLCOUNTOF_N148(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N149(e, __VA_ARGS__), ) 148u
#define PPUTLCOUNTOF_N149(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N150(e, __VA_ARGS__), ) 149u
#define PPUTLCOUNTOF_N150(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N151(e, __VA_ARGS__), ) 150u
#define PPUTLCOUNTOF_N151(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N152(e, __VA_ARGS__), ) 151u
#define PPUTLCOUNTOF_N152(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N153(e, __VA_ARGS__), ) 152u
#define PPUTLCOUNTOF_N153(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N154(e, __VA_ARGS__), ) 153u
#define PPUTLCOUNTOF_N154(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N155(e, __VA_ARGS__), ) 154u
#define PPUTLCOUNTOF_N155(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N156(e, __VA_ARGS__), ) 155u
#define PPUTLCOUNTOF_N156(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N157(e, __VA_ARGS__), ) 156u
#define PPUTLCOUNTOF_N157(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N158(e, __VA_ARGS__), ) 157u
#define PPUTLCOUNTOF_N158(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N159(e, __VA_ARGS__), ) 158u
#define PPUTLCOUNTOF_N159(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N160(e, __VA_ARGS__), ) 159u
#define PPUTLCOUNTOF_N160(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N161(e, __VA_ARGS__), ) 160u
#define PPUTLCOUNTOF_N161(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N162(e, __VA_ARGS__), ) 161u
#define PPUTLCOUNTOF_N162(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N163(e, __VA_ARGS__), ) 162u
#define PPUTLCOUNTOF_N163(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N164(e, __VA_ARGS__), ) 163u
#define PPUTLCOUNTOF_N164(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N165(e, __VA_ARGS__), ) 164u
#define PPUTLCOUNTOF_N165(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N166(e, __VA_ARGS__), ) 165u
#define PPUTLCOUNTOF_N166(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N167(e, __VA_ARGS__), ) 166u
#define PPUTLCOUNTOF_N167(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N168(e, __VA_ARGS__), ) 167u
#define PPUTLCOUNTOF_N168(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N169(e, __VA_ARGS__), ) 168u
#define PPUTLCOUNTOF_N169(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N170(e, __VA_ARGS__), ) 169u
#define PPUTLCOUNTOF_N170(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N171(e, __VA_ARGS__), ) 170u
#define PPUTLCOUNTOF_N171(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N172(e, __VA_ARGS__), ) 171u
#define PPUTLCOUNTOF_N172(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N173(e, __VA_ARGS__), ) 172u
#define PPUTLCOUNTOF_N173(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N174(e, __VA_ARGS__), ) 173u
#define PPUTLCOUNTOF_N174(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N175(e, __VA_ARGS__), ) 174u
#define PPUTLCOUNTOF_N175(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N176(e, __VA_ARGS__), ) 175u
#define PPUTLCOUNTOF_N176(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N177(e, __VA_ARGS__), ) 176u
#define PPUTLCOUNTOF_N177(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N178(e, __VA_ARGS__), ) 177u
#define PPUTLCOUNTOF_N178(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N179(e, __VA_ARGS__), ) 178u
#define PPUTLCOUNTOF_N179(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N180(e, __VA_ARGS__), ) 179u
#define PPUTLCOUNTOF_N180(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N181(e, __VA_ARGS__), ) 180u
#define PPUTLCOUNTOF_N181(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N182(e, __VA_ARGS__), ) 181u
#define PPUTLCOUNTOF_N182(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N183(e, __VA_ARGS__), ) 182u
#define PPUTLCOUNTOF_N183(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N184(e, __VA_ARGS__), ) 183u
#define PPUTLCOUNTOF_N184(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N185(e, __VA_ARGS__), ) 184u
#define PPUTLCOUNTOF_N185(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N186(e, __VA_ARGS__), ) 185u
#define PPUTLCOUNTOF_N186(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N187(e, __VA_ARGS__), ) 186u
#define PPUTLCOUNTOF_N187(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N188(e, __VA_ARGS__), ) 187u
#define PPUTLCOUNTOF_N188(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N189(e, __VA_ARGS__), ) 188u
#define PPUTLCOUNTOF_N189(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N190(e, __VA_ARGS__), ) 189u
#define PPUTLCOUNTOF_N190(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N191(e, __VA_ARGS__), ) 190u
#define PPUTLCOUNTOF_N191(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N192(e, __VA_ARGS__), ) 191u
#define PPUTLCOUNTOF_N192(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N193(e, __VA_ARGS__), ) 192u
#define PPUTLCOUNTOF_N193(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N194(e, __VA_ARGS__), ) 193u
#define PPUTLCOUNTOF_N194(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N195(e, __VA_ARGS__), ) 194u
#define PPUTLCOUNTOF_N195(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N196(e, __VA_ARGS__), ) 195u
#define PPUTLCOUNTOF_N196(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N197(e, __VA_ARGS__), ) 196u
#define PPUTLCOUNTOF_N197(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N198(e, __VA_ARGS__), ) 197u
#define PPUTLCOUNTOF_N198(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N199(e, __VA_ARGS__), ) 198u
#define PPUTLCOUNTOF_N199(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N200(e, __VA_ARGS__), ) 199u
#define PPUTLCOUNTOF_N200(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N201(e, __VA_ARGS__), ) 200u
#define PPUTLCOUNTOF_N201(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N202(e, __VA_ARGS__), ) 201u
#define PPUTLCOUNTOF_N202(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N203(e, __VA_ARGS__), ) 202u
#define PPUTLCOUNTOF_N203(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N204(e, __VA_ARGS__), ) 203u
#define PPUTLCOUNTOF_N204(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N205(e, __VA_ARGS__), ) 204u
#define PPUTLCOUNTOF_N205(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N206(e, __VA_ARGS__), ) 205u
#define PPUTLCOUNTOF_N206(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N207(e, __VA_ARGS__), ) 206u
#define PPUTLCOUNTOF_N207(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N208(e, __VA_ARGS__), ) 207u
#define PPUTLCOUNTOF_N208(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N209(e, __VA_ARGS__), ) 208u
#define PPUTLCOUNTOF_N209(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N210(e, __VA_ARGS__), ) 209u
#define PPUTLCOUNTOF_N210(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N211(e, __VA_ARGS__), ) 210u
#define PPUTLCOUNTOF_N211(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N212(e, __VA_ARGS__), ) 211u
#define PPUTLCOUNTOF_N212(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N213(e, __VA_ARGS__), ) 212u
#define PPUTLCOUNTOF_N213(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N214(e, __VA_ARGS__), ) 213u
#define PPUTLCOUNTOF_N214(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N215(e, __VA_ARGS__), ) 214u
#define PPUTLCOUNTOF_N215(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N216(e, __VA_ARGS__), ) 215u
#define PPUTLCOUNTOF_N216(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N217(e, __VA_ARGS__), ) 216u
#define PPUTLCOUNTOF_N217(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N218(e, __VA_ARGS__), ) 217u
#define PPUTLCOUNTOF_N218(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N219(e, __VA_ARGS__), ) 218u
#define PPUTLCOUNTOF_N219(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N220(e, __VA_ARGS__), ) 219u
#define PPUTLCOUNTOF_N220(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N221(e, __VA_ARGS__), ) 220u
#define PPUTLCOUNTOF_N221(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N222(e, __VA_ARGS__), ) 221u
#define PPUTLCOUNTOF_N222(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N223(e, __VA_ARGS__), ) 222u
#define PPUTLCOUNTOF_N223(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N224(e, __VA_ARGS__), ) 223u
#define PPUTLCOUNTOF_N224(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N225(e, __VA_ARGS__), ) 224u
#define PPUTLCOUNTOF_N225(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N226(e, __VA_ARGS__), ) 225u
#define PPUTLCOUNTOF_N226(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N227(e, __VA_ARGS__), ) 226u
#define PPUTLCOUNTOF_N227(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N228(e, __VA_ARGS__), ) 227u
#define PPUTLCOUNTOF_N228(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N229(e, __VA_ARGS__), ) 228u
#define PPUTLCOUNTOF_N229(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N230(e, __VA_ARGS__), ) 229u
#define PPUTLCOUNTOF_N230(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N231(e, __VA_ARGS__), ) 230u
#define PPUTLCOUNTOF_N231(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N232(e, __VA_ARGS__), ) 231u
#define PPUTLCOUNTOF_N232(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N233(e, __VA_ARGS__), ) 232u
#define PPUTLCOUNTOF_N233(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N234(e, __VA_ARGS__), ) 233u
#define PPUTLCOUNTOF_N234(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N235(e, __VA_ARGS__), ) 234u
#define PPUTLCOUNTOF_N235(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N236(e, __VA_ARGS__), ) 235u
#define PPUTLCOUNTOF_N236(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N237(e, __VA_ARGS__), ) 236u
#define PPUTLCOUNTOF_N237(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N238(e, __VA_ARGS__), ) 237u
#define PPUTLCOUNTOF_N238(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N239(e, __VA_ARGS__), ) 238u
#define PPUTLCOUNTOF_N239(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N240(e, __VA_ARGS__), ) 239u
#define PPUTLCOUNTOF_N240(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N241(e, __VA_ARGS__), ) 240u
#define PPUTLCOUNTOF_N241(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N242(e, __VA_ARGS__), ) 241u
#define PPUTLCOUNTOF_N242(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N243(e, __VA_ARGS__), ) 242u
#define PPUTLCOUNTOF_N243(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N244(e, __VA_ARGS__), ) 243u
#define PPUTLCOUNTOF_N244(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N245(e, __VA_ARGS__), ) 244u
#define PPUTLCOUNTOF_N245(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N246(e, __VA_ARGS__), ) 245u
#define PPUTLCOUNTOF_N246(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N247(e, __VA_ARGS__), ) 246u
#define PPUTLCOUNTOF_N247(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N248(e, __VA_ARGS__), ) 247u
#define PPUTLCOUNTOF_N248(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N249(e, __VA_ARGS__), ) 248u
#define PPUTLCOUNTOF_N249(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N250(e, __VA_ARGS__), ) 249u
#define PPUTLCOUNTOF_N250(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N251(e, __VA_ARGS__), ) 250u
#define PPUTLCOUNTOF_N251(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N252(e, __VA_ARGS__), ) 251u
#define PPUTLCOUNTOF_N252(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N253(e, __VA_ARGS__), ) 252u
#define PPUTLCOUNTOF_N253(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N254(e, __VA_ARGS__), ) 253u
#define PPUTLCOUNTOF_N254(e, _, ...) __VA_OPT__(PPUTLCOUNTOF_N255(e, __VA_ARGS__), ) 254u
#define PPUTLCOUNTOF_N255(e, _, ...) __VA_OPT__(PTL_FAIL(e), ) 255u

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.list]
/// -----------
/// __VA_ARGS__ base type. returns args in all cases (matches everything)
///
/// PTL_LIST()         // <nothing>
/// PTL_LIST(foo)      // foo
/// PTL_LIST(foo, bar) // foo, bar
#define PTL_LIST(/* list */...) /* -> list */ __VA_ARGS__

/// [type.none]
/// -----------
/// [inherits from PTL_LIST] nothing; an absence of pp-tokens (an empty list).
///
/// PTL_NONE() // <nothing>
#define PTL_NONE(/* none */...) /* -> none */    \
  PTL_XCAT(PPUTLNONE_, PTL_IS_NONE(__VA_ARGS__)) \
  (PTL_STR("[PTL_NONE] none cannot describe something" : __VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLNONE_1(e)
#define PPUTLNONE_0(e) PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.obj]
/// ----------
/// [inherits from PTL_LIST] a list with exactly one element.
///
/// PTL_OBJ(foo) // foo
#define PTL_OBJ(/* obj */...) /* -> obj */        \
  PTL_XCAT(PPUTLOBJ_, PTL_IS_OBJECT(__VA_ARGS__)) \
  (PTL_STR("obj must describe exactly one element" : PTL_OBJ(__VA_ARGS__)), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLOBJ_1(e, obj) obj
#define PPUTLOBJ_0(e, ...) PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.atom]
/// -----------
/// [inherits from PTL_OBJ] a sequence of digit|nondigit tokens (/[\w\d_]+/).
///
/// this function only tests for nothing, tuples, and multiple values.
///
/// while not testable, the true semantics of atom implies
/// that its values are able to concatenate with identifiers
/// to form new identifiers, meaning that is must match /[\w\d_]+/.
///
/// this property is critical for value-based control flow
/// and must be observed by the user where applicable.
///
/// PTL_ATOM(foo) // foo
#define PTL_ATOM(/* obj */...) /* -> atom */                   \
  PPUTLATOM_o(PTL_STR("[PTL_ATOM] atom cannot describe tuples" \
                      : __VA_ARGS__),                          \
              PTL_OBJ(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLATOM_o(e, obj)  PTL_XCAT(PPUTLATOM_, PPUTLIS_ATOM_o(obj))(e, obj)
#define PPUTLATOM_1(e, atom) atom
#define PPUTLATOM_0(e, ...)  PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.enum]
/// -----------
/// [inherits from PTL_ATOM] an atom that matches a specified union.
///
/// to use this function, define a set of
/// macros with the following characteristics:
///  ‐ object-like
///  ‐ common prefix
///  ‐ enum value suffixes
///  ‐ expand to nothing OR expand to more than one value
/// pass the common prefix as chkprefix.
///
/// example: (asserting an enum<GOOD|BAD>)
///  #define FOO_GOOD
///  #define FOO_BAD
///  PTL_ENUM(FOO_, BLEH) // <fail>
///  PTL_ENUM(FOO_, GOOD) // GOOD
///  PTL_ENUM(FOO_, ,,,)  // <fail>
#define PTL_ENUM(/* chkprefix: atom, enum<...> */...) /* -> enum<...> */ \
  PPUTLENUM_o(PTL_STR("[PTL_ENUM] enum validation failure" : __VA_ARGS__), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLENUM_o(e, chk, v) PPUTLENUM_oo(e, PTL_ATOM(chk), PTL_ATOM(v))
#define PPUTLENUM_oo(e, chkatom, vatom) \
  PTL_XCAT(PPUTLENUM_, PPUTLIS_ENUM_oo(chkatom, vatom))(e, vatom)
#define PPUTLENUM_1(e, enum) enum
#define PPUTLENUM_0(e, ...)  PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.bool]
/// -----------
/// [PTL_ENUM<0|1>] boolean type.
///
/// PTL_BOOL(0) // 0
/// PTL_BOOL(1) // 1
#define PTL_BOOL(/* bool */...) /* -> bool */                                        \
  PPUTLBOOL_o(                                                                       \
      PTL_STR("[PTL_BOOL] bool cannot describe anything but the literal '1' and '0'" \
              : __VA_ARGS__),                                                        \
      PTL_ATOM(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBOOL_o(e, atom) PTL_XCAT(PPUTLBOOL_, PTL_IS_BOOL(atom))(e, atom)
#define PPUTLBOOL_1(e, bool) bool
#define PPUTLBOOL_0(e, ...)  PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.idec]
/// -----------
/// [PTL_ENUM<0|1|...|2046|2047>] a positive decimal integer.
/// constructible from any word within [0, int_max].
///
/// note: negative decimals may be pasted using fmt.c_int.
///
/// PTL_IDEC(0x000)  // 0
/// PTL_IDEC(0x001)  // 1
/// PTL_IDEC(0x005u) // 5
/// PTL_IDEC(0x7FF)  // 2047
/// PTL_IDEC(2047)   // 2047
#define PTL_IDEC(/* word */...) /* -> idec */                \
  PPUTLIDEC_o(PTL_STR("invalid word"                         \
                      : PTL_IDEC(__VA_ARGS__)),              \
              PTL_STR("idec cannot represent negative words" \
                      : PTL_IDEC(__VA_ARGS__)),              \
              PTL_ATOM(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIDEC_o(e0, e1, atom)                                                       \
  PTL_XCAT(PPUTLIDEC_o_, PTL_XCAT(PTL_XCAT(PPUTLIS_IDEC_o(atom), PPUTLIS_IHEX_o(atom)), \
                                  PTL_XCAT(PPUTLIS_ENUM_oo(PPUTLIMPL_UDEC_, atom),      \
                                           PPUTLIS_ENUM_oo(PPUTLIMPL_UHEX_, atom))))    \
  (e0, e1, atom)
#define PPUTLIDEC_o_1000(e0, e1, idec) idec
#define PPUTLIDEC_o_0100(e0, e1, ihex) PPUTLIDEC_o_0001(e0, e1, ihex##u)
#define PPUTLIDEC_o_0010(e0, e1, udec) \
  PPUTLIDEC_o_0001(e0, e1, PPUTLIMPL_UDEC(udec, UHEX))
#define PPUTLIDEC_o_0001(e0, e1, uhex) \
  PTL_XCAT(PPUTLIDEC_o_0001_, PPUTLIMPL_UHEX(uhex, ILTZ))(e1, uhex)
#define PPUTLIDEC_o_0001_1(e1, uhex)   PTL_FAIL(e1)
#define PPUTLIDEC_o_0001_0(e1, uhex)   PPUTLIMPL_UHEX(uhex, ICAST)
#define PPUTLIDEC_o_0000(e0, e1, atom) PTL_FAIL(e0)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.ihex]
/// -----------
/// [PTL_ENUM<0x000|0x001|...|0xFFE|0xFFF>] a hexadecimal integer.
/// constructible from any word within [int_min, int_max].
///
/// PTL_IHEX(0)     // 0x000
/// PTL_IHEX(1)     // 0x001
/// PTL_IHEX(5)     // 0x005
/// PTL_IHEX(4095u) // 0xFFF
/// PTL_IHEX(2047u) // 0x7FF
#define PTL_IHEX(/* word */...) /* -> ihex */ \
  PPUTLIHEX_o(PTL_STR("invalid word" : PTL_IHEX(__VA_ARGS__)), PTL_ATOM(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIHEX_o(e, atom)                                                            \
  PTL_XCAT(PPUTLIHEX_o_, PTL_XCAT(PTL_XCAT(PPUTLIS_IDEC_o(atom), PPUTLIS_IHEX_o(atom)), \
                                  PTL_XCAT(PPUTLIS_ENUM_oo(PPUTLIMPL_UDEC_, atom),      \
                                           PPUTLIS_ENUM_oo(PPUTLIMPL_UHEX_, atom))))    \
  (e, atom)
#define PPUTLIHEX_o_1000(e, idec) PPUTLIMPL_UHEX(PPUTLIMPL_UDEC(idec##u, UHEX), IHEX)
#define PPUTLIHEX_o_0100(e, ihex) ihex
#define PPUTLIHEX_o_0010(e, udec) PPUTLIMPL_UHEX(PPUTLIMPL_UDEC(udec, UHEX), IHEX)
#define PPUTLIHEX_o_0001(e, uhex) PPUTLIMPL_UHEX(uhex, IHEX)
#define PPUTLIHEX_o_0000(e, atom) PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.udec]
/// -----------
/// [PTL_ENUM<0u|1u|...|4094u|4095u>] an unsigned decimal integer.
///
/// PTL_UDEC(0x000u) // 0u
/// PTL_UDEC(1)      // 1u
/// PTL_UDEC(5)      // 5u
/// PTL_UDEC(0x005u) // 5u
/// PTL_UDEC(0xFFFu) // 4095u
/// PTL_UDEC(0xFFF)  // 4095u
#define PTL_UDEC(/* word */...) /* -> udec */ \
  PPUTLUDEC_o(PTL_STR("invalid word" : PTL_UDEC(__VA_ARGS__)), PTL_ATOM(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLUDEC_o(e, atom)                                                            \
  PTL_XCAT(PPUTLUDEC_o_, PTL_XCAT(PTL_XCAT(PPUTLIS_IDEC_o(atom), PPUTLIS_IHEX_o(atom)), \
                                  PTL_XCAT(PPUTLIS_ENUM_oo(PPUTLIMPL_UDEC_, atom),      \
                                           PPUTLIS_ENUM_oo(PPUTLIMPL_UHEX_, atom))))    \
  (e, atom)
#define PPUTLUDEC_o_1000(e, idec) idec##u
#define PPUTLUDEC_o_0100(e, ihex) PPUTLIMPL_UHEX(ihex##u, UDEC)
#define PPUTLUDEC_o_0010(e, udec) udec
#define PPUTLUDEC_o_0001(e, uhex) PPUTLIMPL_UHEX(uhex, UDEC)
#define PPUTLUDEC_o_0000(e, atom) PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.uhex]
/// -----------
/// [PTL_ENUM<0x000u|0x001u|...|0xFFEu|0xFFFu>] an unsigned hexadecimal integer.
///
/// PTL_UHEX(0)      // 0x000u
/// PTL_UHEX(1)      // 0x001u
/// PTL_UHEX(5)      // 0x005u
/// PTL_UHEX(4095u)  // 0xFFFu
/// PTL_UHEX(0x000u) // 0x000u
/// PTL_UHEX(0x001u) // 0x001u
/// PTL_UHEX(0xFFF)  // 0xFFFu
#define PTL_UHEX(/* word */...) /* -> uhex */ \
  PPUTLUHEX_o(PTL_STR("invalid word" : PTL_UHEX(__VA_ARGS__)), PTL_ATOM(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLUHEX_o(e, atom)                                                            \
  PTL_XCAT(PPUTLUHEX_o_, PTL_XCAT(PTL_XCAT(PPUTLIS_IDEC_o(atom), PPUTLIS_IHEX_o(atom)), \
                                  PTL_XCAT(PPUTLIS_ENUM_oo(PPUTLIMPL_UDEC_, atom),      \
                                           PPUTLIS_ENUM_oo(PPUTLIMPL_UHEX_, atom))))    \
  (e, atom)
#define PPUTLUHEX_o_1000(e, idec) PPUTLIMPL_UDEC(idec##u, UHEX)
#define PPUTLUHEX_o_0100(e, ihex) ihex##u
#define PPUTLUHEX_o_0010(e, udec) PPUTLIMPL_UDEC(udec, UHEX)
#define PPUTLUHEX_o_0001(e, uhex) uhex
#define PPUTLUHEX_o_0000(e, atom) PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.int]
/// ----------
/// [union PTL_IDEC|PTL_IHEX] 12-bit signed integer type.
/// constructible from any word type.
/// instance is either idec or ihex.
///
/// cannot parse negative decimals; use numeric.neg instead.
/// hex length is fixed. cannot parse shorter hex lengths.
///
/// cast modes:
///
///   idec → idec | [default]
///   idec → ihex | requires IHEX hint
///
///   ihex → ihex | [default]; fallback for failed ihex → idec
///   ihex → idec | requires IDEC hint and positive result
///
///   udec → idec | [default]; requires positive result
///   udec → ihex | requires IHEX hint or udec → idec failure
///
///   uhex → ihex | [default]; fallback for failed uhex → idec
///   uhex → idec | requires IDEC hint and positive result
///
/// attempts to preserve hex/decimal representation by default, but
/// will output hex if casting the input yields a negative number.
/// hint is ignored only if the result is negative and the hint is IDEC.
///
/// cast from unsigned reinterprets bits as signed two's complement.
///
/// values above the int max must have a 'u' suffix; implicit interpretation
/// as unsigned is not allowed (e.g. 4095 is not a valid integer).
///
/// PTL_INT(0)            // 0
/// PTL_INT(1, IHEX)      // 0x001
/// PTL_INT(0x002)        // 0x002
/// PTL_INT(0x800, IDEC)  // 0x800
/// PTL_INT(0x002, IDEC)  // 2
/// PTL_INT(7u)           // 7
/// PTL_INT(15u, IHEX)    // 0x00F
/// PTL_INT(4095u)        // 0xFFF
/// PTL_INT(0x007u)       // 0x007
/// PTL_INT(0xFFFu, IDEC) // 0xFFF
/// PTL_INT(0x005u, IDEC) // 5
#define PTL_INT(/* word, hint=AUTO: enum<IDEC|IHEX|AUTO> */...) /* -> int */ \
  PPUTLINT_o(PTL_STR("invalid arguments" : PTL_INT(__VA_ARGS__)), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLINT_o(e, v, ...)                                                       \
  PTL_XCAT(PPUTLINT_,                                                               \
           PPUTLINT_MODE(e, PTL_TYPEOF(v),                                          \
                         PTL_ENUM(PPUTLINT_HINT_, PTL_DEFAULT(AUTO, __VA_ARGS__)))) \
  (v)
#define PPUTLINT_UH_IH(uhex) PPUTLIMPL_UHEX(uhex, IHEX)
#define PPUTLINT_UH_IC(uhex) PPUTLIMPL_UHEX(uhex, ICAST)
#define PPUTLINT_UD_IH(udec) PPUTLIMPL_UHEX(PPUTLIMPL_UDEC(udec, UHEX), IHEX)
#define PPUTLINT_UD_IC(udec) PPUTLIMPL_UHEX(PPUTLIMPL_UDEC(udec, UHEX), ICAST)
#define PPUTLINT_IH_IH(ihex) ihex
#define PPUTLINT_IH_IC(ihex) PPUTLIMPL_UHEX(ihex##u, ICAST)
#define PPUTLINT_ID_IH(idec) PPUTLIMPL_UHEX(PPUTLIMPL_UDEC(idec##u, UHEX), IHEX)
#define PPUTLINT_ID_ID(idec) idec

/// cast mode selector and error detector
#define PPUTLINT_MODE(e, t, hint) \
  PTL_XCAT(PPUTLINT_MODE_, PTL_IS_NONE(PTL_XCAT(PPUTLINT_MODE_, t)))(e, t, hint)
#define PPUTLINT_MODE_1(e, t, hint) PPUTLINT_MODE_1_##t##hint
#define PPUTLINT_MODE_1_UHEXAUTO    UH_IH
#define PPUTLINT_MODE_1_UHEXIHEX    UH_IH
#define PPUTLINT_MODE_1_UHEXIDEC    UH_IC
#define PPUTLINT_MODE_1_UDECAUTO    UD_IC
#define PPUTLINT_MODE_1_UDECIHEX    UD_IH
#define PPUTLINT_MODE_1_UDECIDEC    UD_IC
#define PPUTLINT_MODE_1_IHEXAUTO    IH_IH
#define PPUTLINT_MODE_1_IHEXIHEX    IH_IH
#define PPUTLINT_MODE_1_IHEXIDEC    IH_IC
#define PPUTLINT_MODE_1_IDECAUTO    ID_ID
#define PPUTLINT_MODE_1_IDECIHEX    ID_IH
#define PPUTLINT_MODE_1_IDECIDEC    ID_ID
#define PPUTLINT_MODE_0(e, t, hint) PTL_FAIL(e)
#define PPUTLINT_MODE_UHEX
#define PPUTLINT_MODE_UDEC
#define PPUTLINT_MODE_IHEX
#define PPUTLINT_MODE_IDEC
#define PPUTLINT_HINT_AUTO
#define PPUTLINT_HINT_IHEX
#define PPUTLINT_HINT_IDEC

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.tup]
/// ----------
/// [inherits from PTL_OBJ] tuple type (parenthesized list).
/// expands to t if valid, else fails.
///
/// PTL_TUP(())     // ()
/// PTL_TUP((1, 2)) // (1, 2)
#define PTL_TUP(/* tup */...) /* -> tup */                            \
  PPUTLTUP_o(PTL_STR("[PTL_TUP] tuple must be wrapped in parentheses" \
                     : __VA_ARGS__),                                  \
             PTL_OBJ(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLTUP_o(e, obj) PTL_XCAT(PPUTLTUP_, PPUTLIS_TUPLE_o(obj))(e, obj)
#define PPUTLTUP_1(e, tup) tup
#define PPUTLTUP_0(e, ...) PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.uint]
/// -----------
/// [union PTL_UDEC|PTL_UHEX] 12-bit unsigned integer type.
/// constructible from any word type.
/// instance is either udec or uhex.
///
/// cannot parse negative decimals; use numeric.neg instead.
/// hex length is fixed. cannot parse shorter hex lengths.
///
/// cast modes:
///
///   idec → udec | [default]
///   idec → uhex | requires UHEX hint
///
///   ihex → uhex | [default]
///   ihex → udec | requires UDEC hint
///
///   udec → udec | [default]
///   udec → uhex | requires UHEX hint
///
///   uhex → uhex | [default]
///   uhex → udec | requires UDEC hint
///
/// preserves hex/decimal representation.
/// specify hint to choose a cast mode.
///
/// uses a 'u' suffix for hex/decimal representations.
/// see fmt.paste_uint to remove suffix before pasting.
///
/// cast from signed reinterprets bits as unsigned.
///
/// values above the int max must have a 'u' suffix; implicit interpretation
/// as unsigned is not allowed (e.g. 4095 is not a valid integer).
///
/// PTL_UINT(0)            // 0u
/// PTL_UINT(2, UHEX)      // 0x002u
/// PTL_UINT(0x007)        // 0x007u
/// PTL_UINT(0x00F, UDEC)  // 15u
/// PTL_UINT(8u)           // 8u
/// PTL_UINT(6u, UHEX)     // 0x006u
/// PTL_UINT(0x005u)       // 0x005u
/// PTL_UINT(0x004u, UDEC) // 4u
#define PTL_UINT(/* word, hint=AUTO: enum<UDEC|UHEX|AUTO> */...) /* -> uint */ \
  PPUTLUINT_o(PTL_STR("[PTL_UINT] invalid arguments" : __VA_ARGS__), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLUINT_o(e, v, ...)                                                        \
  PTL_XCAT(PPUTLUINT_,                                                                \
           PPUTLUINT_MODE(e, PTL_TYPEOF(v),                                           \
                          PTL_ENUM(PPUTLUINT_HINT_, PTL_DEFAULT(AUTO, __VA_ARGS__)))) \
  (v)
#define PPUTLUINT_UH_UH(uhex) uhex
#define PPUTLUINT_UH_UD(uhex) PPUTLIMPL_UHEX(uhex, UDEC)
#define PPUTLUINT_UD_UH(udec) PPUTLIMPL_UDEC(udec, UHEX)
#define PPUTLUINT_UD_UD(udec) udec
#define PPUTLUINT_IH_UH(ihex) ihex##u
#define PPUTLUINT_IH_UD(ihex) PPUTLIMPL_UHEX(ihex##u, UDEC)
#define PPUTLUINT_ID_UH(idec) PPUTLIMPL_UDEC(idec##u, UHEX)
#define PPUTLUINT_ID_UD(idec) idec##u

/// cast mode selector and error detector
#define PPUTLUINT_MODE(e, t, hint) /* -> <cast mode> */ \
  PTL_XCAT(PPUTLUINT_MODE_, PTL_IS_NONE(PTL_XCAT(PPUTLUINT_MODE_, t)))(e, t, hint)
#define PPUTLUINT_MODE_1(e, t, hint) PPUTLUINT_MODE_1_##t##hint
#define PPUTLUINT_MODE_1_UHEXAUTO    UH_UH
#define PPUTLUINT_MODE_1_UHEXUHEX    UH_UH
#define PPUTLUINT_MODE_1_UHEXUDEC    UH_UD
#define PPUTLUINT_MODE_1_UDECAUTO    UD_UD
#define PPUTLUINT_MODE_1_UDECUHEX    UD_UH
#define PPUTLUINT_MODE_1_UDECUDEC    UD_UD
#define PPUTLUINT_MODE_1_IHEXAUTO    IH_UH
#define PPUTLUINT_MODE_1_IHEXUHEX    IH_UH
#define PPUTLUINT_MODE_1_IHEXUDEC    IH_UD
#define PPUTLUINT_MODE_1_IDECAUTO    ID_UD
#define PPUTLUINT_MODE_1_IDECUHEX    ID_UH
#define PPUTLUINT_MODE_1_IDECUDEC    ID_UD
#define PPUTLUINT_MODE_0(e, t, hint) PTL_FAIL(e)
#define PPUTLUINT_MODE_UHEX
#define PPUTLUINT_MODE_UDEC
#define PPUTLUINT_MODE_IHEX
#define PPUTLUINT_MODE_IDEC
#define PPUTLUINT_HINT_AUTO
#define PPUTLUINT_HINT_UHEX
#define PPUTLUINT_HINT_UDEC

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.word]
/// -----------
/// [union PTL_INT|PTL_UINT] any defined integer representation.
/// constructibe from any word type.
///
/// cannot parse negative decimals; use numeric.neg instead.
/// hex length is fixed. cannot parse shorter hex lengths.
///
/// cast modes:
///
///   idec → idec | [default]
///   idec → ihex | requires IHEX hint
///   idec → udec | requires UDEC hint
///   idec → uhex | requires UHEX hint
///
///   ihex → ihex | [default]; failback for failed ihex → idec
///   ihex → idec | requires IDEC hint and positive result
///   ihex → udec | requires UDEC hint
///   ihex → uhex | requires UHEX hint
///
///   udec → udec | [default]
///   udec → idec | requires IDEC hint and positive result
///   udec → ihex | requires IHEX hint or udec → idec failure
///   udec → uhex | requires UHEX hint
///
///   uhex → uhex | [default]
///   uhex → idec | requires IDEC hint and positive result
///   uhex → ihex | requires IHEX hint or uhex → idec failure
///   uhex → udec | requires UDEC hint
///
/// attempts to preserve hex/decimal representation by default, but
/// will output hex if casting the input yields a negative number.
/// hint is ignored only if the result is negative and the hint is IDEC.
///
/// cast between signed and unsigned reinterprets bits.
///
/// values above the int max must have a 'u' suffix; implicit interpretation
/// as unsigned is not allowed (e.g. 4095 is not a valid integer).
///
/// PTL_WORD(0)            // 0
/// PTL_WORD(1, IHEX)      // 0x001
/// PTL_WORD(2, UDEC)      // 2u
/// PTL_WORD(3, UHEX)      // 0x003u
/// PTL_WORD(0x002)        // 0x002
/// PTL_WORD(0x800, IDEC)  // 0x800
/// PTL_WORD(0x002, IDEC)  // 2
/// PTL_WORD(0x00F, UDEC)  // 15u
/// PTL_WORD(0x007, UHEX)  // 0x007u
/// PTL_WORD(8u)           // 8u
/// PTL_WORD(7u, IDEC)     // 7
/// PTL_WORD(15u, IHEX)    // 0x00F
/// PTL_WORD(4095u, IDEC)  // 0xFFF
/// PTL_WORD(6u, UHEX)     // 0x006u
/// PTL_WORD(0x005u)       // 0x005u
/// PTL_WORD(0x005u, IDEC) // 5
/// PTL_WORD(0x007u, IHEX) // 0x007
/// PTL_WORD(0xFFFu, IDEC) // 0xFFF
/// PTL_WORD(0x004u, UDEC) // 4u
#define PTL_WORD(/* word, hint=AUTO: enum<IDEC|IHEX|UDEC|UHEX|AUTO> */...) /* -> word */ \
  PPUTLWORD_o(PTL_STR("[PTL_WORD] invalid arguments" : __VA_ARGS__), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLWORD_o(e, v, ...)                                                        \
  PTL_XCAT(PPUTLWORD_,                                                                \
           PPUTLWORD_MODE(e, PTL_TYPEOF(v),                                           \
                          PTL_ENUM(PPUTLWORD_HINT_, PTL_DEFAULT(AUTO, __VA_ARGS__)))) \
  (v)
#define PPUTLWORD_UHEX(word) PTL_UHEX(word)
#define PPUTLWORD_UDEC(word) PTL_UDEC(word)
#define PPUTLWORD_IHEX(word) PTL_IHEX(word)
#define PPUTLWORD_IDEC(word) PTL_INT(word, IDEC)

/// cast mode selector and error detector
#define PPUTLWORD_MODE(e, t, hint) \
  PTL_XCAT(PPUTLWORD_MODE_, PTL_IS_NONE(PTL_XCAT(PPUTLWORD_MODE_, t)))(e, t, hint)
#define PPUTLWORD_MODE_1(e, t, hint) PPUTLWORD_MODE_1_##hint(t)
#define PPUTLWORD_MODE_1_AUTO(t)     t
#define PPUTLWORD_MODE_1_UHEX(t)     UHEX
#define PPUTLWORD_MODE_1_UDEC(t)     UDEC
#define PPUTLWORD_MODE_1_IHEX(t)     IHEX
#define PPUTLWORD_MODE_1_IDEC(t)     IDEC
#define PPUTLWORD_MODE_0(e, t, hint) PTL_FAIL(e)
#define PPUTLWORD_MODE_UHEX
#define PPUTLWORD_MODE_UDEC
#define PPUTLWORD_MODE_IHEX
#define PPUTLWORD_MODE_IDEC
#define PPUTLWORD_HINT_AUTO
#define PPUTLWORD_HINT_UHEX
#define PPUTLWORD_HINT_UDEC
#define PPUTLWORD_HINT_IHEX
#define PPUTLWORD_HINT_IDEC

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.size]
/// -----------
/// [inherits from PTL_WORD] any non-negative word up to PTL_SIZE_MAX (255u).
/// constructibe from any word type.
///
/// cannot parse negative decimals; use numeric.neg instead.
/// hex length is fixed. cannot parse shorter hex lengths.
///
/// see type.word for available cast modes.
///
/// preserves hex/decimal representation by default.
///
/// cast between signed and unsigned reinterprets bits.
///
/// values above the int max must have a 'u' suffix; implicit interpretation
/// as unsigned is not allowed (e.g. 4095 is not a valid integer).
///
/// PTL_SIZE(0)     // 0
/// PTL_SIZE(1)     // 1
/// PTL_SIZE(0x007) // 0x007
/// PTL_SIZE(255u)  // 255u
#define PTL_SIZE(/* word, hint=AUTO: enum<IDEC|IHEX|UDEC|UHEX|AUTO> */...) /* -> size */ \
  PPUTLSIZE_o(                                                                           \
      PTL_STR("[PTL_SIZE] invalid size; must be within 0 and PTL_SIZE_MAX (255u)"        \
              : __VA_ARGS__),                                                            \
      PTL_WORD(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLSIZE_o(e, w)   PTL_XCAT(PPUTLSIZE_o_, PPUTLIS_SIZE_o(w))(e, w)
#define PPUTLSIZE_o_1(e, w) w
#define PPUTLSIZE_o_0(e, w) PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.ofs]
/// ----------
/// [inherits from PTL_WORD] any word whose absolute value is a valid size.
/// constructibe from any word type.
///
/// cannot parse negative decimals; use numeric.neg instead.
/// hex length is fixed. cannot parse shorter hex lengths.
///
/// see type.word for available cast modes.
///
/// preserves hex/decimal representation by default.
///
/// cast between signed and unsigned reinterprets bits.
///
/// values above the int max must have a 'u' suffix; implicit interpretation
/// as unsigned is not allowed (e.g. 4095 is not a valid integer).
///
/// PTL_OFS(0)     // 0
/// PTL_OFS(1)     // 1
/// PTL_OFS(0x007) // 0x007
/// PTL_OFS(0xFFF) // 0xFFF
/// PTL_OFS(254)   // 254
#define PTL_OFS(/* word, hint=AUTO: enum<IDEC|IHEX|UDEC|UHEX|AUTO> */...) /* -> ofs */ \
  PPUTLOFS_o(PTL_STR("invalid ofs; absolute value must be a valid size"                \
                     : PTL_OFS(__VA_ARGS__)),                                          \
             PTL_WORD(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLOFS_o(e, w)   PTL_XCAT(PPUTLOFS_o_, PPUTLIS_OFFSET_o(w))(e, w)
#define PPUTLOFS_o_1(e, w) w
#define PPUTLOFS_o_0(e, w) PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.arr]
/// ----------
/// [inherits from PTL_OBJ] a sized sequence of elements.
/// construction from derived array type casts to base.
///
/// resultant object is an expansion-terminated self-reference.
///
/// note: constructor never fails. construction from existing array type
///       assumes that the contained structure is valid, construction from
///       explicit size assumes size is valid, and any inputs that do not
///       match any other constructor fallback to list construction.
///
/// PTL_ARR()                  // PTL_ARR(0u, ())
/// PTL_ARR(a, b)              // PTL_ARR(2u, (a, b))
/// PTL_ARR(3, (, , ))         // PTL_ARR(3, (, , ))
/// PTL_ARR((1, 2, 3))         // PTL_ARR(1u, ((1, 2, 3)))
/// PTL_ARR(PTL_ARR(foo, bar)) // PTL_ARR(2u, (foo, bar))
#define PTL_ARR(/* o?: arr|tup */...) /* -> arr */ PTL_ARR(PPUTLARR_o(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

/// base array construction. returns array contents without type wrapper.
#define PPUTLARR_o(...) PTL_XCAT(PPUTLARR_o_, PTL_IS_NONE(__VA_ARGS__))(__VA_ARGS__)

#define PPUTLARR_o_1(...) 0u, ()

#define PPUTLARR_o_0(...) PTL_XCAT(PPUTLARR_o_0_, PTL_IS_OBJECT(__VA_ARGS__))(__VA_ARGS__)

#define PPUTLARR_o_0_1(obj)              PTL_XCAT(PPUTLARR_o_0_1_, PPUTLIS_ARRAY_o(obj))(obj)
#define PPUTLARR_o_0_1_1(arr)            PPUTLARR_o_0_1_1_##arr
#define PPUTLARR_o_0_1_1_PTL_PQUEUE(...) __VA_ARGS__
#define PPUTLARR_o_0_1_1_PTL_QUEUE(...)  __VA_ARGS__
#define PPUTLARR_o_0_1_1_PTL_STACK(...)  __VA_ARGS__
#define PPUTLARR_o_0_1_1_PTL_SET(...)    __VA_ARGS__
#define PPUTLARR_o_0_1_1_PTL_MAP(...)    __VA_ARGS__
#define PPUTLARR_o_0_1_1_PTL_ARR(...)    __VA_ARGS__
#define PPUTLARR_o_0_1_0(obj)            1u, (obj)
#define PPUTLARR_o_0_0(_, ...)                                                      \
  PTL_XCAT(PPUTLARR_o_0_0_, PTL_XCAT(PTL_IS_OBJECT(_), PTL_IS_OBJECT(__VA_ARGS__))) \
  (_, __VA_ARGS__)
#define PPUTLARR_o_0_0_11(obj_a, obj_b)                                              \
  PTL_XCAT(PPUTLARR_o_0_0_11_, PTL_XCAT(PTL_IS_SIZE(obj_a), PPUTLIS_TUPLE_o(obj_b))) \
  (obj_a, obj_b)
#define PPUTLARR_o_0_0_11_11(size, tup)          size, tup
#define PPUTLARR_o_0_0_11_10(/* size, obj */...) 2u, (__VA_ARGS__)
#define PPUTLARR_o_0_0_11_01(/* obj, tup */...)  2u, (__VA_ARGS__)
#define PPUTLARR_o_0_0_11_00(/* obj, obj */...)  2u, (__VA_ARGS__)
#define PPUTLARR_o_0_0_10(/* obj, list */...)    PTL_COUNTOF(__VA_ARGS__), (__VA_ARGS__)
#define PPUTLARR_o_0_0_01(/* none, obj */...)    2u, (__VA_ARGS__)
#define PPUTLARR_o_0_0_00(/* none, list */...)   PTL_ARR(__VA_ARGS__), (__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.map]
/// ----------
/// [inherits from PTL_ARR] a sorted mapping of words to elements.
/// overloads: map(arr), map(size, tup), map(tup...).
///
/// elements are stored as key/value pairs in sorted order; construction from
/// list or tuple assumes the inputs are valid (word, any) pairs in sorted order.
///
/// resultant object is an expansion-terminated self-reference.
///
/// note: constructor never fails. construction from existing array type
///       assumes that the contained structure is valid, construction from
///       explicit size assumes size is valid, and any inputs that do not
///       match any other constructor fallback to list construction.
///
/// PTL_MAP()          // PTL_MAP()
/// PTL_MAP(PTL_MAP()) // PTL_MAP()
#define PTL_MAP(/* o?: arr|tup */...) /* -> map */ \
  PPUTLMAP_o(PTL_STR("invalid map"                 \
                     : PTL_MAP(__VA_ARGS__)),      \
             PTL_OBJ(PTL_DEFAULT(PTL_MAP(), __VA_ARGS__)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLMAP_o(e, obj) PTL_XCAT(PPUTLMAP_, PPUTLIS_MAP_o(obj))(e, obj)
#define PPUTLMAP_1(e, map) map
#define PPUTLMAP_0(e, ...) PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.set]
/// ----------
/// [inherits from PTL_OBJ] a set of words.
/// optionally provide arguments to make a type assertion.
///
/// note: does not parse contained items during validity check.
///
/// PTL_SET()          // PTL_SET()
/// PTL_SET(PTL_SET()) // PTL_SET()
#define PTL_SET(/* o?: arr|tup */...) /* -> set */ \
  PPUTLSET_o(PTL_STR("invalid set"                 \
                     : PTL_SET(__VA_ARGS__)),      \
             PTL_OBJ(PTL_DEFAULT(PTL_SET(), __VA_ARGS__)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLSET_o(e, obj) PTL_XCAT(PPUTLSET_, PPUTLIS_SET_o(obj))(e, obj)
#define PPUTLSET_1(e, set) set
#define PPUTLSET_0(e, ...) PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.stack]
/// ------------
/// [inherits from PTL_OBJ] a LIFO stack of lists.
/// optionally provide arguments to make a type assertion.
///
/// note: does not parse contained items during validity check.
///
/// PTL_STACK()            // PTL_STACK()
/// PTL_STACK(PTL_STACK()) // PTL_STACK()
#define PTL_STACK(/* o?: arr|tup */...) /* -> stack */ \
  PPUTLSTACK_o(PTL_STR("invalid stack"                 \
                       : PTL_STACK(__VA_ARGS__)),      \
               PTL_OBJ(PTL_DEFAULT(PTL_STACK(), __VA_ARGS__)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLSTACK_o(e, obj)   PTL_XCAT(PPUTLSTACK_, PPUTLIS_STACK_o(obj))(e, obj)
#define PPUTLSTACK_1(e, stack) stack
#define PPUTLSTACK_0(e, ...)   PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.queue]
/// ------------
/// [inherits from PTL_OBJ] a FIFO queue of lists.
/// optionally provide arguments to make a type assertion.
///
/// note: does not parse contained items during validity check.
///
/// PTL_QUEUE()            // PTL_QUEUE()
/// PTL_QUEUE(PTL_QUEUE()) // PTL_QUEUE()
#define PTL_QUEUE(/* o?: arr|tup */...) /* -> queue */ \
  PPUTLQUEUE_o(PTL_STR("invalid queue"                 \
                       : PTL_QUEUE(__VA_ARGS__)),      \
               PTL_OBJ(PTL_DEFAULT(PTL_QUEUE(), __VA_ARGS__)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLQUEUE_o(e, obj)   PTL_XCAT(PPUTLQUEUE_, PPUTLIS_QUEUE_o(obj))(e, obj)
#define PPUTLQUEUE_1(e, queue) queue
#define PPUTLQUEUE_0(e, ...)   PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.pqueue]
/// -------------
/// [inherits from PTL_OBJ] a priority queue of lists.
/// optionally provide arguments to make a type assertion.
///
/// note: does not parse contained items during validity check.
///
/// PTL_PQUEUE()             // PTL_PQUEUE()
/// PTL_PQUEUE(PTL_PQUEUE()) // PTL_PQUEUE()
#define PTL_PQUEUE(/* o?: arr|tup */...) /* -> pqueue */ \
  PPUTLPQUEUE_o(PTL_STR("invalid pqueue"                 \
                        : PTL_PQUEUE(__VA_ARGS__)),      \
                PTL_OBJ(PTL_DEFAULT(PTL_PQUEUE(), __VA_ARGS__)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLPQUEUE_o(e, obj)    PTL_XCAT(PPUTLPQUEUE_, PPUTLIS_PQUEUE_o(obj))(e, obj)
#define PPUTLPQUEUE_1(e, pqueue) pqueue
#define PPUTLPQUEUE_0(e, ...)    PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.any]
/// ----------
/// [union PTL_NONE|PTL_OBJ] a list with no separatory commas.
/// fails if more than one arg.
///
/// PTL_ANY()    // <nothing>
/// PTL_ANY(foo) // foo
#define PTL_ANY(/* any */...) /* -> any */     \
  PTL_XCAT(PPUTLANY_, PTL_IS_ANY(__VA_ARGS__)) \
  (PTL_STR("any cannot describe multiple args" : PTL_ANY(__VA_ARGS__)), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLANY_1(e, obj) obj
#define PPUTLANY_0(e, ...) PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [logic.not]
/// -----------
/// logical NOT.
///
/// PTL_NOT(0) // 1
/// PTL_NOT(1) // 0
#define PTL_NOT(/* bool */...) /* -> bool */ PTL_XCAT(PPUTLNOT_, PTL_BOOL(__VA_ARGS__))

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
#define PTL_AND(/* bool, bool */...) /* -> bool */ PPUTLAND_X(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLAND_X(a, b) PTL_XCAT(PPUTLAND_, PTL_XCAT(PTL_BOOL(a), PTL_BOOL(b)))
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
#define PTL_OR(/* bool, bool */...) /* -> bool */ PPUTLOR_X(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLOR_X(a, b) PTL_XCAT(PPUTLOR_, PTL_XCAT(PTL_BOOL(a), PTL_BOOL(b)))
#define PPUTLOR_11      1
#define PPUTLOR_10      1
#define PPUTLOR_01      1
#define PPUTLOR_00      0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [logic.xor]
/// -----------
/// logical XOR.
///
/// PTL_XOR(0, 0) // 0
/// PTL_XOR(0, 1) // 1
/// PTL_XOR(1, 0) // 1
/// PTL_XOR(1, 1) // 0
#define PTL_XOR(/* bool, bool */...) /* -> bool */ PPUTLXOR_X(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLXOR_X(a, b) PTL_XCAT(PPUTLXOR_, PTL_XCAT(PTL_BOOL(a), PTL_BOOL(b)))
#define PPUTLXOR_11      0
#define PPUTLXOR_10      1
#define PPUTLXOR_01      1
#define PPUTLXOR_00      0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [logic.nand]
/// ------------
/// logical NAND.
///
/// PTL_NAND(0, 0) // 1
/// PTL_NAND(0, 1) // 1
/// PTL_NAND(1, 0) // 1
/// PTL_NAND(1, 1) // 0
#define PTL_NAND(/* bool, bool */...) /* -> bool */ PPUTLNAND_X(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLNAND_X(a, b) PTL_XCAT(PPUTLNAND_, PTL_XCAT(PTL_BOOL(a), PTL_BOOL(b)))
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
#define PTL_NOR(/* bool, bool */...) /* -> bool */ PPUTLNOR_X(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLNOR_X(a, b) PTL_XCAT(PPUTLNOR_, PTL_XCAT(PTL_BOOL(a), PTL_BOOL(b)))
#define PPUTLNOR_11      0
#define PPUTLNOR_10      0
#define PPUTLNOR_01      0
#define PPUTLNOR_00      1

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [logic.xnor]
/// ------------
/// logical XNOR.
///
/// PTL_XNOR(0, 0) // 1
/// PTL_XNOR(0, 1) // 0
/// PTL_XNOR(1, 0) // 0
/// PTL_XNOR(1, 1) // 1
#define PTL_XNOR(/* bool, bool */...) /* -> bool */ PPUTLXNOR_X(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLXNOR_X(a, b) PTL_XCAT(PPUTLXNOR_, PTL_XCAT(PTL_BOOL(a), PTL_BOOL(b)))
#define PPUTLXNOR_11      1
#define PPUTLXNOR_10      0
#define PPUTLXNOR_01      0
#define PPUTLXNOR_00      1

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

/// [impl.compare.lt]
/// -----------------
/// [internal] unsigned less-than comparison.
#define PPUTLIMPL_LT(/* uhex, uhex */ a, b) /* -> bool */ \
  PPUTLIMPL_LT_UCMP(PPUTLIMPL_UHEX(a, HDUMP), PPUTLIMPL_UHEX(b, HDUMP))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_LT_UCMP(...) \
  PTL_XFIRST(PPUTLIMPL_LT_R(   \
      PPUTLIMPL_LT_R(PPUTLIMPL_LT_R(0, 0, PPUTLIMPL_LT_ZIP(__VA_ARGS__)))))
#define PPUTLIMPL_LT_ZIP(...)                PPUTLIMPL_LT_ZIP_o(__VA_ARGS__)
#define PPUTLIMPL_LT_ZIP_o(a, b, c, d, e, f) a, d, b, e, c, f
#define PPUTLIMPL_LT_R(...)                  PPUTLIMPL_LT_R_o(__VA_ARGS__)
#define PPUTLIMPL_LT_R_o(fl, fg, a, b, ...)                    \
  PTL_XCAT(PPUTLIMPL_LT_##fl##fg, PPUTLIMPL_HEXHEX(a##b, LT)), \
      PTL_XCAT(PPUTLIMPL_LT_##fg##fl, PPUTLIMPL_HEXHEX(b##a, LT)), __VA_ARGS__
#define PPUTLIMPL_LT_111 1
#define PPUTLIMPL_LT_110 1
#define PPUTLIMPL_LT_101 1
#define PPUTLIMPL_LT_100 1
#define PPUTLIMPL_LT_011 0
#define PPUTLIMPL_LT_010 0
#define PPUTLIMPL_LT_001 1
#define PPUTLIMPL_LT_000 0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [compare.lt]
/// ------------
/// integral less-than comparison.
/// prohibits comparison of different signedness.
///
/// PTL_LT(0, 0)                  // 0
/// PTL_LT(0, 1)                  // 1
/// PTL_LT(7u, 8u)                // 1
/// PTL_LT(PTL_INT(4095u), 0)     // 1
/// PTL_LT(2047, 0x800)           // 0
/// PTL_LT(0x800, PTL_INT(2048u)) // 0
/// PTL_LT(0x800, PTL_INT(2049u)) // 1
#define PTL_LT(/* word, word */...) /* -> bool */                       \
  PPUTLLT_o(__VA_ARGS__)(                                               \
      PTL_STR("[PTL_LT] comparison of different signedness not allowed" \
              : __VA_ARGS__),                                           \
      __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLLT_o(l, r) \
  PTL_XCAT(PPUTLLT_o_, PTL_XCAT(PPUTLLT_SIGNOF(PTL_WORD(l)), PPUTLLT_SIGNOF(PTL_WORD(r))))
#define PPUTLLT_o_UU(e, l, r) PPUTLIMPL_LT(PTL_UHEX(l), PTL_UHEX(r))
#define PPUTLLT_o_UI(e, l, r) PTL_FAIL(e)
#define PPUTLLT_o_IU(e, l, r) PTL_FAIL(e)
#define PPUTLLT_o_II(e, l, r) \
  PPUTLLT_ICMP(PPUTLIMPL_UHEX(PTL_UHEX(l), HDUMP), PPUTLIMPL_UHEX(PTL_UHEX(r), HDUMP))
#define PPUTLLT_SIGNOF(word)  PTL_XCAT(PPUTLLT_SIGNOF_, PPUTLIS_TUPLE_o(word))(word)
#define PPUTLLT_SIGNOF_1(tup) U
#define PPUTLLT_SIGNOF_0(atom) \
  PTL_XCAT(PPUTLLT_SIGNOF_0, PTL_XCAT(PPUTLIS_INT_o(atom), PPUTLIS_UINT_o(atom)))
#define PPUTLLT_SIGNOF_010  I
#define PPUTLLT_SIGNOF_001  U
#define PPUTLLT_ICMP(...)   PPUTLLT_ICMP_o(__VA_ARGS__)
#define PPUTLLT_ICMP_o(...) PPUTLLT_ICMP_oo(__VA_ARGS__)
#define PPUTLLT_ICMP_oo(a, b, c, d, e, f)                                    \
  PTL_XCAT(PPUTLLT_ICMP_oo_,                                                 \
           PTL_XCAT(PPUTLIMPL_HEXHEX(7##a, LT), PPUTLIMPL_HEXHEX(7##d, LT))) \
  (a, b, c, d, e, f)
#define PPUTLLT_ICMP_oo_11(...) PPUTLIMPL_LT_UCMP(__VA_ARGS__)
#define PPUTLLT_ICMP_oo_10(...) 1
#define PPUTLLT_ICMP_oo_01(...) 0
#define PPUTLLT_ICMP_oo_00(...) PPUTLIMPL_LT_UCMP(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [compare.gt]
/// ------------
/// integral greater-than comparison.
/// prohibits comparison of different signedness.
///
/// PTL_GT(0, 0)                  // 0
/// PTL_GT(0, 1)                  // 0
/// PTL_GT(7u, 8u)                // 0
/// PTL_GT(PTL_INT(4095u), 0)     // 0
/// PTL_GT(2047, 0x800)           // 1
/// PTL_GT(0x800, PTL_INT(2048u)) // 0
/// PTL_GT(0x800, PTL_INT(2049u)) // 0
#define PTL_GT(/* word, word */...) /* -> bool */ PPUTLGT_X(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLGT_X(l, r) PTL_LT(r, l)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [compare.le]
/// ------------
/// integral less-than-or-equal-to comparison.
/// prohibits comparison of different signedness.
///
/// PTL_LE(0, 0)                  // 1
/// PTL_LE(0, 1)                  // 1
/// PTL_LE(7u, 8u)                // 1
/// PTL_LE(PTL_INT(4095u), 0)     // 1
/// PTL_LE(2047, 0x800)           // 0
/// PTL_LE(0x800, PTL_INT(2048u)) // 1
/// PTL_LE(0x800, PTL_INT(2049u)) // 1
#define PTL_LE(/* word, word */...) /* -> bool */ PTL_NOT(PTL_GT(__VA_ARGS__))

/// [compare.ge]
/// ------------
/// integral greater-than-or-equal-to comparison.
/// prohibits comparison of different signedness.
///
/// PTL_GE(0, 0)                  // 1
/// PTL_GE(0, 1)                  // 0
/// PTL_GE(7u, 8u)                // 0
/// PTL_GE(PTL_INT(4095u), 0)     // 0
/// PTL_GE(2047, 0x800)           // 1
/// PTL_GE(0x800, PTL_INT(2048u)) // 1
/// PTL_GE(0x800, PTL_INT(2049u)) // 0
#define PTL_GE(/* word, word */...) /* -> bool */ PTL_NOT(PTL_LT(__VA_ARGS__))

/// [compare.eq]
/// ------------
/// integral equal-to comparison.
/// prohibits comparison of different signedness.
///
/// PTL_EQ(0, 0)                  // 1
/// PTL_EQ(0, 1)                  // 0
/// PTL_EQ(7u, 8u)                // 0
/// PTL_EQ(PTL_INT(4095u), 0)     // 0
/// PTL_EQ(2047, 0x800)           // 0
/// PTL_EQ(0x800, PTL_INT(2048u)) // 1
/// PTL_EQ(0x800, PTL_INT(2049u)) // 0
#define PTL_EQ(/* word, word */...) /* -> bool */ \
  PTL_AND(PTL_LE(__VA_ARGS__), PTL_GE(__VA_ARGS__))

/// [compare.ne]
/// ------------
/// integral not-equal-to comparison.
/// prohibits comparison of different signedness.
///
/// PTL_NE(0, 0)                  // 0
/// PTL_NE(0, 1)                  // 1
/// PTL_NE(7u, 8u)                // 1
/// PTL_NE(PTL_INT(4095u), 0)     // 1
/// PTL_NE(2047, 0x800)           // 1
/// PTL_NE(0x800, PTL_INT(2048u)) // 0
/// PTL_NE(0x800, PTL_INT(2049u)) // 1
#define PTL_NE(/* word, word */...) /* -> bool */ PTL_NOT(PTL_EQ(__VA_ARGS__))

/// [compare.min]
/// -------------
/// integral minimum operation.
/// prohibits comparison of different signedness.
///
/// PTL_MIN(0, 0)                  // 0
/// PTL_MIN(0, 1)                  // 0
/// PTL_MIN(7u, 8u)                // 7u
/// PTL_MIN(PTL_INT(4095u), 0)     // 0xFFF
/// PTL_MIN(2047, 0x800)           // 0x800
/// PTL_MIN(0x800, PTL_INT(2048u)) // 0x800
/// PTL_MIN(0x800, PTL_INT(2049u)) // 0x800
#define PTL_MIN(/* word, word */...) /* -> word */ \
  PTL_XCAT(PPUTLMIN_, PTL_LT(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLMIN_1(a, b) a
#define PPUTLMIN_0(a, b) b

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [compare.max]
/// -------------
/// integral maximum operation.
/// prohibits comparison of different signedness.
///
/// PTL_MAX(0, 0)                  // 0
/// PTL_MAX(0, 1)                  // 1
/// PTL_MAX(7u, 8u)                // 8u
/// PTL_MAX(PTL_INT(4095u), 0)     // 0
/// PTL_MAX(2047, 0x800)           // 2047
/// PTL_MAX(0x800, PTL_INT(2048u)) // 0x800
/// PTL_MAX(0x800, PTL_INT(2049u)) // 0x801
#define PTL_MAX(/* word, word */...) /* -> word */ \
  PTL_XCAT(PPUTLMAX_, PTL_GT(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLMAX_1(a, b) a
#define PPUTLMAX_0(a, b) b

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

/// [impl.numeric.inc]
/// ------------------
/// [internal] numeric increment w/ overflow.
#define PPUTLIMPL_INC(/* uhex */ n) /* -> uhex */ \
  PPUTLIMPL_INC_RES(                              \
      PPUTLIMPL_INC_R(PPUTLIMPL_INC_R(PPUTLIMPL_INC_R(1, PPUTLIMPL_UHEX(n, HDUMP)))))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_INC_RES(...)          PPUTLIMPL_INC_RES_o(__VA_ARGS__)
#define PPUTLIMPL_INC_RES_o(_, a, b, c) 0x##a##b##c##u
#define PPUTLIMPL_INC_R(...)            PPUTLIMPL_INC_R_o(__VA_ARGS__)
#define PPUTLIMPL_INC_R_o(_, a, b, c)   PPUTLIMPL_HEX(c, INC##_), a, b

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [impl.numeric.dec]
/// ------------------
/// [internal] numeric decrement w/ underflow.
#define PPUTLIMPL_DEC(/* uhex */ n) /* -> uhex */ \
  PPUTLIMPL_DEC_RES(                              \
      PPUTLIMPL_DEC_R(PPUTLIMPL_DEC_R(PPUTLIMPL_DEC_R(1, PPUTLIMPL_UHEX(n, HDUMP)))))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_DEC_RES(...)          PPUTLIMPL_DEC_RES_o(__VA_ARGS__)
#define PPUTLIMPL_DEC_RES_o(_, a, b, c) 0x##a##b##c##u
#define PPUTLIMPL_DEC_R(...)            PPUTLIMPL_DEC_R_o(__VA_ARGS__)
#define PPUTLIMPL_DEC_R_o(_, a, b, c)   PPUTLIMPL_HEX(c, DEC##_), a, b

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [impl.numeric.neg]
/// ------------------
/// [internal] integral negation.
#define PPUTLIMPL_NEG(/* uhex */ n) /* -> uhex */ PPUTLIMPL_INC(PPUTLIMPL_UHEX(n, BNOT))

/// [impl.numeric.ltz]
/// ------------------
/// [internal] numeric less-than-zero detection (if interpreted as signed).
#define PPUTLIMPL_LTZ(/* uhex */ n) /* -> bool */ \
  PPUTLIMPL_LTZ_RES(PPUTLIMPL_UHEX(n, HDUMP))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_LTZ_RES(...)       PPUTLIMPL_LTZ_RES_o(__VA_ARGS__)
#define PPUTLIMPL_LTZ_RES_o(a, b, c) PPUTLIMPL_HEXHEX(7##a, LT)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [impl.math.add]
/// ---------------
/// [internal] addition with overflow.
#define PPUTLIMPL_ADD(/* uhex, uhex */ a, b) /* -> uhex */ \
  PPUTLIMPL_ADD_RES(PPUTLIMPL_ADD_R(PPUTLIMPL_ADD_R(       \
      PPUTLIMPL_ADD_R(0, PPUTLIMPL_UHEX(a, HDUMP), PPUTLIMPL_UHEX(b, HDUMP)))))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_ADD_RES(...)                   PPUTLIMPL_ADD_RES_o(__VA_ARGS__)
#define PPUTLIMPL_ADD_RES_o(_, a, b, c, d, e, f) 0x##a##b##c##u
#define PPUTLIMPL_ADD_R(...)                     PPUTLIMPL_ADD_R_o(__VA_ARGS__)
#define PPUTLIMPL_ADD_R_o(_, a, b, c, d, e, f) \
  PPUTLIMPL_HEXHEX(c##f, ADD##_), a, b, f, d, e

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [impl.math.sub]
/// ---------------
/// [internal] subtraction with underflow.
#define PPUTLIMPL_SUB(/* uhex, uhex */ a, b) /* -> uhex */ \
  PPUTLIMPL_SUB_RES(PPUTLIMPL_SUB_R(PPUTLIMPL_SUB_R(       \
      PPUTLIMPL_SUB_R(0, PPUTLIMPL_UHEX(a, HDUMP), PPUTLIMPL_UHEX(b, HDUMP)))))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_SUB_RES(...)                   PPUTLIMPL_SUB_RES_o(__VA_ARGS__)
#define PPUTLIMPL_SUB_RES_o(_, a, b, c, d, e, f) 0x##a##b##c##u
#define PPUTLIMPL_SUB_R(...)                     PPUTLIMPL_SUB_R_o(__VA_ARGS__)
#define PPUTLIMPL_SUB_R_o(_, a, b, c, d, e, f) \
  PPUTLIMPL_HEXHEX(c##f, SUB##_), a, b, f, d, e

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [impl.math.index]
/// -----------------
/// [internal] translates an idx to a non-negative zero-offset for a given range size.
#define PPUTLIMPL_INDEX(/* uhex, bool, uhex, obj */ i, sign, sz, err) /* -> uhex */ \
  PTL_CAT(PPUTLIMPL_INDEX_, sign)(i, sz, err)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_INDEX_1(i, sz, err) \
  PTL_XCAT(PPUTLIMPL_INDEX_1_, PPUTLIMPL_LTZ(i))(i, sz, err)
#define PPUTLIMPL_INDEX_1_1(i, sz, err)                                             \
  PTL_XCAT(PPUTLIMPL_INDEX_1_1_, PPUTLIMPL_LT(PPUTLIMPL_NEG(i), PPUTLIMPL_INC(sz))) \
  (i, sz, err)
#define PPUTLIMPL_INDEX_1_1_1(i, sz, err) PPUTLIMPL_ADD(i, sz)
#define PPUTLIMPL_INDEX_1_1_0(i, sz, err) PTL_FAIL(err)
#define PPUTLIMPL_INDEX_1_0(i, sz, err)   PPUTLIMPL_INDEX_0(i, sz, err)
#define PPUTLIMPL_INDEX_0(i, sz, err) \
  PTL_XCAT(PPUTLIMPL_INDEX_0_, PPUTLIMPL_LT(i, PPUTLIMPL_INC(sz)))(i, err)
#define PPUTLIMPL_INDEX_0_1(i, err) i
#define PPUTLIMPL_INDEX_0_0(i, err) PTL_FAIL(err)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [numeric.inc]
/// -------------
/// numeric increment w/ overflow.
///
/// PTL_INC(0)     // 1
/// PTL_INC(1u)    // 2u
/// PTL_INC(2047)  // 0x800
/// PTL_INC(0x7FF) // 0x800
/// PTL_INC(15u)   // 16u
/// PTL_INC(4095u) // 0u
#define PTL_INC(/* word */...) /* -> word */ \
  PTL_WORD(PPUTLIMPL_INC(PTL_UHEX(__VA_ARGS__)), PTL_TYPEOF(__VA_ARGS__))

/// [numeric.dec]
/// -------------
/// numeric decrement w/ underflow.
///
/// PTL_DEC(1)     // 0
/// PTL_DEC(2u)    // 1u
/// PTL_DEC(0)     // 0xFFF
/// PTL_DEC(0x800) // 0x7FF
/// PTL_DEC(16u)   // 15u
/// PTL_DEC(0u)    // 4095u
#define PTL_DEC(/* word */...) /* -> word */ \
  PTL_WORD(PPUTLIMPL_DEC(PTL_UHEX(__VA_ARGS__)), PTL_TYPEOF(__VA_ARGS__))

/// [numeric.neg]
/// -------------
/// integral negation.
///
/// PTL_NEG(0)     // 0
/// PTL_NEG(1)     // 0xFFF
/// PTL_NEG(0xFFF) // 0x001
/// PTL_NEG(1u)    // 4095u
#define PTL_NEG(/* word */...) /* -> word */ \
  PTL_WORD(PPUTLIMPL_NEG(PTL_UHEX(__VA_ARGS__)), PTL_TYPEOF(__VA_ARGS__))

/// [numeric.eqz]
/// -------------
/// numeric zero detection.
///
/// PTL_EQZ(0)      // 1
/// PTL_EQZ(0u)     // 1
/// PTL_EQZ(0x000)  // 1
/// PTL_EQZ(0x000u) // 1
/// PTL_EQZ(1u)     // 0
/// PTL_EQZ(2)      // 0
/// PTL_EQZ(4095u)  // 0
/// PTL_EQZ(0x800)  // 0
#define PTL_EQZ(/* word */...) /* -> bool */ \
  PTL_IS_NONE(PTL_XCAT(PPUTLEQZ_, PTL_UDEC(__VA_ARGS__)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLEQZ_0u

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [numeric.nez]
/// -------------
/// numeric non-zero detection.
///
/// PTL_NEZ(0)      // 0
/// PTL_NEZ(0u)     // 0
/// PTL_NEZ(0x000)  // 0
/// PTL_NEZ(0x000u) // 0
/// PTL_NEZ(1u)     // 1
/// PTL_NEZ(2)      // 1
/// PTL_NEZ(4095u)  // 1
/// PTL_NEZ(0x800)  // 1
#define PTL_NEZ(/* word */...) /* -> bool */ \
  PTL_NOT(PTL_IS_NONE(PTL_XCAT(PPUTLEQZ_, PTL_UDEC(__VA_ARGS__))))

/// [numeric.ltz]
/// -------------
/// numeric less-than-zero detection.
///
/// PTL_LTZ(0)             // 0
/// PTL_LTZ(1)             // 0
/// PTL_LTZ(0u)            // 0
/// PTL_LTZ(1u)            // 0
/// PTL_LTZ(2047)          // 0
/// PTL_LTZ(0x800)         // 1
/// PTL_LTZ(PTL_INC(2047)) // 1
#define PTL_LTZ(/* word */...) /* -> bool */ \
  PTL_XCAT(PPUTLLTZ_, PTL_IS_INT(__VA_ARGS__))(PTL_UHEX(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLLTZ_1(n) PPUTLIMPL_LTZ(n)
#define PPUTLLTZ_0(n) 0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [numeric.gtz]
/// -------------
/// numeric greater-than-zero detection.
///
/// PTL_GTZ(0)             // 0
/// PTL_GTZ(1)             // 1
/// PTL_GTZ(0u)            // 0
/// PTL_GTZ(1u)            // 1
/// PTL_GTZ(2047)          // 1
/// PTL_GTZ(0x800)         // 0
/// PTL_GTZ(PTL_INC(2047)) // 0
#define PTL_GTZ(/* word */...) /* -> bool */ \
  PTL_NOR(PTL_LTZ(__VA_ARGS__), PTL_EQZ(__VA_ARGS__))

/// [numeric.lez]
/// -------------
/// numeric less-than-or-equal-to-zero detection.
///
/// PTL_LEZ(0)             // 1
/// PTL_LEZ(1)             // 0
/// PTL_LEZ(0u)            // 1
/// PTL_LEZ(1u)            // 0
/// PTL_LEZ(2047)          // 0
/// PTL_LEZ(0x800)         // 1
/// PTL_LEZ(PTL_INC(2047)) // 1
#define PTL_LEZ(/* word */...) /* -> bool */ \
  PTL_OR(PTL_LTZ(__VA_ARGS__), PTL_EQZ(__VA_ARGS__))

/// [numeric.gez]
/// -------------
/// numeric greater-than-or-equal-to-zero detection.
///
/// PTL_GEZ(0)             // 1
/// PTL_GEZ(1)             // 1
/// PTL_GEZ(0u)            // 1
/// PTL_GEZ(1u)            // 1
/// PTL_GEZ(2047)          // 1
/// PTL_GEZ(0x800)         // 0
/// PTL_GEZ(PTL_INC(2047)) // 0
#define PTL_GEZ(/* word */...) /* -> bool */ PTL_NOT(PTL_LTZ(__VA_ARGS__))

/// [numeric.abs]
/// -------------
/// numeric absolute value.
/// casts result according to the input.
///
/// PTL_ABS(0)           // 0
/// PTL_ABS(1)           // 1
/// PTL_ABS(PTL_NEG(1))  // 0x001
/// PTL_ABS(PTL_NEG(15)) // 0x00F
#define PTL_ABS(/* word */...) /* -> word */ \
  PTL_XCAT(PPUTLABS_, PTL_LTZ(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLABS_1(n) PTL_NEG(n)
#define PPUTLABS_0(n) n

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [numeric.log2]
/// --------------
/// numeric log2. value must be greater than zero.
/// casts result according to the input.
///
/// PTL_LOG2(1u)    // 0u
/// PTL_LOG2(2)     // 1
/// PTL_LOG2(0x004) // 0x002
/// PTL_LOG2(4095u) // 11u
#define PTL_LOG2(/* word */...) /* -> word */ \
  PTL_XCAT(PPUTLLOG2_, PTL_GTZ(__VA_ARGS__))  \
  (PTL_STR("[PTL_LOG2] value must be greater than zero" : __VA_ARGS__), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLLOG2_1(e, n) PTL_WORD(PPUTLIMPL_UDEC(PTL_UDEC(n), LOG2), PTL_TYPEOF(n))
#define PPUTLLOG2_0(e, n) PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [numeric.sqrt]
/// --------------
/// numeric sqrt. value must be non-negative.
/// casts result according to the input.
///
/// PTL_SQRT(0u)    // 0u
/// PTL_SQRT(4)     // 2
/// PTL_SQRT(0x010) // 0x004
/// PTL_SQRT(4095u) // 63u
#define PTL_SQRT(/* word */...) /* -> word */ \
  PTL_XCAT(PPUTLSQRT_, PTL_LTZ(__VA_ARGS__))  \
  (PTL_STR("[PTL_SQRT] value must be non-negative" : __VA_ARGS__), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLSQRT_1(e, n) PTL_FAIL(e)
#define PPUTLSQRT_0(e, n) PTL_WORD(PPUTLIMPL_UDEC(PTL_UDEC(n), SQRT), PTL_TYPEOF(n))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [numeric.fact]
/// --------------
/// numeric prime factorization. value must be non-negative.
/// casts each result according to the input.
///
/// PTL_FACT(0u)  // <nothing>
/// PTL_FACT(13)  // <nothing>
/// PTL_FACT(4)   // 2, 2
/// PTL_FACT(12u) // 2u, 2u, 3u
#define PTL_FACT(/* word */...) /* -> word... */   \
  PTL_XCAT(PPUTLFACT_, PTL_LTZ(__VA_ARGS__))       \
  (PTL_STR("[PTL_FACT] value must be non-negative" \
           : __VA_ARGS__),                         \
   PPUTLFACT_R, __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLFACT_1(e, r, n) PTL_FAIL(e)
#define PPUTLFACT_0(e, r, n)                    \
  PPUTLFACT_X(PTL_ESC PTL_XFIRST(r(r(r(r(r(r(r( \
      r(r(r(PPUTLFACT_INIT(PTL_TYPEOF(n), PPUTLIMPL_UDEC(PTL_UDEC(n), FACT))))))))))))))
#define PPUTLFACT_R(...)            PPUTLFACT_R_o(__VA_ARGS__)
#define PPUTLFACT_R_o(a, t, ...)    PPUTLFACT_R0##__VA_OPT__(1)(a, t, __VA_ARGS__)
#define PPUTLFACT_R01(a, t, _, ...) PPUTLFACT_UPDATE(t, _, PTL_ESC a), t, __VA_ARGS__
#define PPUTLFACT_R0(a, t, ...)     a, t
#define PPUTLFACT_INIT(...)         PPUTLFACT_INIT_o(__VA_ARGS__)
#define PPUTLFACT_INIT_o(t, ...)    PPUTLFACT_INIT0##__VA_OPT__(1)(t, __VA_ARGS__)
#define PPUTLFACT_INIT01(t, _, ...) (PTL_WORD(_, t)), t, __VA_ARGS__
#define PPUTLFACT_INIT0(t, ...)     (), t
#define PPUTLFACT_UPDATE(t, _, ...) (__VA_ARGS__, PTL_WORD(_, t))
#define PPUTLFACT_X(...)            __VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [numeric.prime]
/// ---------------
/// numeric primality test. numbers less than 2 are non-prime.
///
/// PTL_PRIME(PTL_NEG(2)) // 0
/// PTL_PRIME(0)          // 0
/// PTL_PRIME(1)          // 0
/// PTL_PRIME(2)          // 1
/// PTL_PRIME(3)          // 1
/// PTL_PRIME(4)          // 0
/// PTL_PRIME(13)         // 1
/// PTL_PRIME(1024)       // 0
/// PTL_PRIME(2048u)      // 0
#define PTL_PRIME(/* word */...) /* -> bool */ \
  PTL_XCAT(PPUTLPRIME_, PTL_LEZ(PTL_DEC(__VA_ARGS__)))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLPRIME_1(n) 0
#define PPUTLPRIME_0(n) PTL_IS_NONE(PPUTLIMPL_UDEC(PTL_UDEC(n), FACT))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [bitwise.hexdump]
/// -----------------
/// dumps the hexadecimal digits of a word.
/// returns exactly PTL_WORD_SIZE (3) uppercase hex digits.
///
/// PTL_HEXDUMP(0)     // 0, 0, 0
/// PTL_HEXDUMP(0x800) // 8, 0, 0
#define PTL_HEXDUMP(/* word */...) /* -> enum<0|1|2|...|D|E|F>... */ \
  PPUTLIMPL_UHEX(PTL_UHEX(__VA_ARGS__), HDUMP)

/// [bitwise.bitdump]
/// -----------------
/// dumps the bits of a word.
/// returns exactly PTL_BIT_LENGTH (12) bools.
///
/// PTL_BITDUMP(0)     // 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
/// PTL_BITDUMP(0x800) // 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
#define PTL_BITDUMP(/* word */...) /* -> bool... */ \
  PPUTLBITDUMP_o(PTL_HEXDUMP(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBITDUMP_o(...) PPUTLBITDUMP_oo(__VA_ARGS__)
#define PPUTLBITDUMP_oo(a, b, c) \
  PPUTLIMPL_HEX(a, BITS), PPUTLIMPL_HEX(b, BITS), PPUTLIMPL_HEX(c, BITS)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [bitwise.bitsll]
/// ----------------
/// performs a logical bitwise left shift by n places.
///
/// PTL_BITSLL(0, 1)     // 0
/// PTL_BITSLL(1u, 1)    // 2u
/// PTL_BITSLL(0x002, 2) // 0x008
/// PTL_BITSLL(0x002, 3) // 0x010
/// PTL_BITSLL(4095u, 3) // 4088u
/// PTL_BITSLL(1, 10)    // 1024
/// PTL_BITSLL(1, 11)    // 0x800
/// PTL_BITSLL(1, 12)    // 0
/// PTL_BITSLL(1, 13)    // 0
#define PTL_BITSLL(/* word, size=1 */...) /* -> word */ PPUTLBITSLL_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBITSLL_o(v, ...)                                                     \
  PTL_WORD(PPUTLBITSLL_oo(PTL_IDEC(PTL_DEFAULT(1, __VA_ARGS__)), PTL_BITDUMP(v)), \
           PTL_TYPEOF(v))
#define PPUTLBITSLL_oo(i, ...)         PPUTLBITSLL_ooo(i, PTL_LT(i, 12), __VA_ARGS__)
#define PPUTLBITSLL_ooo(...)           PPUTLBITSLL_oooo(__VA_ARGS__)
#define PPUTLBITSLL_oooo(i, gelt, ...) PPUTLBITSLL_oooo_##gelt(i, __VA_ARGS__)
#define PPUTLBITSLL_oooo_1(i, ...)     PPUTLIMPL_HEX_CAT(PPUTLBITSLL_##i(__VA_ARGS__))
#define PPUTLBITSLL_oooo_0(...)        0

/// bit shifts
#define PPUTLBITSLL_11(a, b, c, d, e, f, g, h, i, j, k, l) \
  PPUTLIMPL_NYBL(l##000, HEX), 0, 0
#define PPUTLBITSLL_10(a, b, c, d, e, f, g, h, i, j, k, l) \
  PPUTLIMPL_NYBL(k##l##00, HEX), 0, 0
#define PPUTLBITSLL_9(a, b, c, d, e, f, g, h, i, j, k, l) \
  PPUTLIMPL_NYBL(j##k##l##0, HEX), 0, 0
#define PPUTLBITSLL_8(a, b, c, d, e, f, g, h, i, j, k, l) \
  PPUTLIMPL_NYBL(i##j##k##l, HEX), 0, 0
#define PPUTLBITSLL_7(a, b, c, d, e, f, g, h, i, j, k, l) \
  PPUTLIMPL_NYBL(h##i##j##k, HEX), PPUTLIMPL_NYBL(l##000, HEX), 0
#define PPUTLBITSLL_6(a, b, c, d, e, f, g, h, i, j, k, l) \
  PPUTLIMPL_NYBL(g##h##i##j, HEX), PPUTLIMPL_NYBL(k##l##00, HEX), 0
#define PPUTLBITSLL_5(a, b, c, d, e, f, g, h, i, j, k, l) \
  PPUTLIMPL_NYBL(f##g##h##i, HEX), PPUTLIMPL_NYBL(j##k##l##0, HEX), 0
#define PPUTLBITSLL_4(a, b, c, d, e, f, g, h, i, j, k, l) \
  PPUTLIMPL_NYBL(e##f##g##h, HEX), PPUTLIMPL_NYBL(i##j##k##l, HEX), 0
#define PPUTLBITSLL_3(a, b, c, d, e, f, g, h, i, j, k, l)           \
  PPUTLIMPL_NYBL(d##e##f##g, HEX), PPUTLIMPL_NYBL(h##i##j##k, HEX), \
      PPUTLIMPL_NYBL(l##000, HEX)
#define PPUTLBITSLL_2(a, b, c, d, e, f, g, h, i, j, k, l)           \
  PPUTLIMPL_NYBL(c##d##e##f, HEX), PPUTLIMPL_NYBL(g##h##i##j, HEX), \
      PPUTLIMPL_NYBL(k##l##00, HEX)
#define PPUTLBITSLL_1(a, b, c, d, e, f, g, h, i, j, k, l)           \
  PPUTLIMPL_NYBL(b##c##d##e, HEX), PPUTLIMPL_NYBL(f##g##h##i, HEX), \
      PPUTLIMPL_NYBL(j##k##l##0, HEX)
#define PPUTLBITSLL_0(a, b, c, d, e, f, g, h, i, j, k, l)           \
  PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
      PPUTLIMPL_NYBL(i##j##k##l, HEX)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [bitwise.bitsrl]
/// ----------------
/// performs a logical bitwise right shift by n places.
///
/// PTL_BITSRL(0, 1)      // 0
/// PTL_BITSRL(2, 1)      // 1
/// PTL_BITSRL(4, 1)      // 2
/// PTL_BITSRL(4, 2)      // 1
/// PTL_BITSRL(0x800, 11) // 0x001
/// PTL_BITSRL(0x800, 12) // 0x000
#define PTL_BITSRL(/* word, size=1 */...) /* -> word */ PPUTLBITSRL_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBITSRL_o(v, ...)                                                     \
  PTL_WORD(PPUTLBITSRL_oo(PTL_IDEC(PTL_DEFAULT(1, __VA_ARGS__)), PTL_BITDUMP(v)), \
           PTL_TYPEOF(v))
#define PPUTLBITSRL_oo(i, ...)         PPUTLBITSRL_ooo(i, PTL_LT(i, 12), __VA_ARGS__)
#define PPUTLBITSRL_ooo(...)           PPUTLBITSRL_oooo(__VA_ARGS__)
#define PPUTLBITSRL_oooo(i, gelt, ...) PPUTLBITSRL_oooo_##gelt(i, __VA_ARGS__)
#define PPUTLBITSRL_oooo_1(i, ...)     PPUTLIMPL_HEX_CAT(PPUTLBITSRL_##i(__VA_ARGS__))
#define PPUTLBITSRL_oooo_0(...)        0

/// bit shifts
#define PPUTLBITSRL_11(a, b, c, d, e, f, g, h, i, j, k, l) \
  0, 0, PPUTLIMPL_NYBL(000##a, HEX)
#define PPUTLBITSRL_10(a, b, c, d, e, f, g, h, i, j, k, l) \
  0, 0, PPUTLIMPL_NYBL(00##a##b, HEX)
#define PPUTLBITSRL_9(a, b, c, d, e, f, g, h, i, j, k, l) \
  0, 0, PPUTLIMPL_NYBL(0##a##b##c, HEX)
#define PPUTLBITSRL_8(a, b, c, d, e, f, g, h, i, j, k, l) \
  0, 0, PPUTLIMPL_NYBL(a##b##c##d, HEX)
#define PPUTLBITSRL_7(a, b, c, d, e, f, g, h, i, j, k, l) \
  0, PPUTLIMPL_NYBL(000##a, HEX), PPUTLIMPL_NYBL(b##c##d##e, HEX)
#define PPUTLBITSRL_6(a, b, c, d, e, f, g, h, i, j, k, l) \
  0, PPUTLIMPL_NYBL(00##a##b, HEX), PPUTLIMPL_NYBL(c##d##e##f, HEX)
#define PPUTLBITSRL_5(a, b, c, d, e, f, g, h, i, j, k, l) \
  0, PPUTLIMPL_NYBL(0##a##b##c, HEX), PPUTLIMPL_NYBL(d##e##f##g, HEX)
#define PPUTLBITSRL_4(a, b, c, d, e, f, g, h, i, j, k, l) \
  0, PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX)
#define PPUTLBITSRL_3(a, b, c, d, e, f, g, h, i, j, k, l)       \
  PPUTLIMPL_NYBL(000##a, HEX), PPUTLIMPL_NYBL(b##c##d##e, HEX), \
      PPUTLIMPL_NYBL(f##g##h##i, HEX)
#define PPUTLBITSRL_2(a, b, c, d, e, f, g, h, i, j, k, l)         \
  PPUTLIMPL_NYBL(00##a##b, HEX), PPUTLIMPL_NYBL(c##d##e##f, HEX), \
      PPUTLIMPL_NYBL(g##h##i##j, HEX)
#define PPUTLBITSRL_1(a, b, c, d, e, f, g, h, i, j, k, l)           \
  PPUTLIMPL_NYBL(0##a##b##c, HEX), PPUTLIMPL_NYBL(d##e##f##g, HEX), \
      PPUTLIMPL_NYBL(h##i##j##k, HEX)
#define PPUTLBITSRL_0(a, b, c, d, e, f, g, h, i, j, k, l)           \
  PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
      PPUTLIMPL_NYBL(i##j##k##l, HEX)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [bitwise.bitsra]
/// ----------------
/// performs an arithmetic bitwise right shift by n places.
///
/// PTL_BITSRA(0, 1)     // 0
/// PTL_BITSRA(2, 1)     // 1
/// PTL_BITSRA(0x800, 1) // 0xC00
/// PTL_BITSRA(0x800, 2) // 0xE00
/// PTL_BITSRA(0x800, 3) // 0xF00
/// PTL_BITSRA(0x800, 4) // 0xF80
#define PTL_BITSRA(/* word, size=1 */...) /* -> word */ PPUTLBITSRA_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBITSRA_o(v, ...)                                                          \
  PTL_WORD(                                                                            \
      PPUTLBITSRA_oo(PTL_IDEC(PTL_DEFAULT(1, __VA_ARGS__)), PTL_BITDUMP(PTL_WORD(v))), \
      PTL_TYPEOF(v))
#define PPUTLBITSRA_oo(i, ...)         PPUTLBITSRA_ooo(i, PTL_LT(i, 12), __VA_ARGS__)
#define PPUTLBITSRA_ooo(...)           PPUTLBITSRA_oooo(__VA_ARGS__)
#define PPUTLBITSRA_oooo(i, gelt, ...) PPUTLBITSRA_oooo_##gelt(i, __VA_ARGS__)
#define PPUTLBITSRA_oooo_1(i, ...)     PPUTLIMPL_HEX_CAT(PPUTLBITSRA_##i(__VA_ARGS__))
#define PPUTLBITSRA_oooo_0(...)        0

/// bit shifts
#define PPUTLBITSRA_11(a, b, c, d, e, f, g, h, i, j, k, l)          \
  PPUTLIMPL_NYBL(a##a##a##a, HEX), PPUTLIMPL_NYBL(a##a##a##a, HEX), \
      PPUTLIMPL_NYBL(a##a##a##a, HEX)
#define PPUTLBITSRA_10(a, b, c, d, e, f, g, h, i, j, k, l)          \
  PPUTLIMPL_NYBL(a##a##a##a, HEX), PPUTLIMPL_NYBL(a##a##a##a, HEX), \
      PPUTLIMPL_NYBL(a##a##a##b, HEX)
#define PPUTLBITSRA_9(a, b, c, d, e, f, g, h, i, j, k, l)           \
  PPUTLIMPL_NYBL(a##a##a##a, HEX), PPUTLIMPL_NYBL(a##a##a##a, HEX), \
      PPUTLIMPL_NYBL(a##a##b##c, HEX)
#define PPUTLBITSRA_8(a, b, c, d, e, f, g, h, i, j, k, l)           \
  PPUTLIMPL_NYBL(a##a##a##a, HEX), PPUTLIMPL_NYBL(a##a##a##a, HEX), \
      PPUTLIMPL_NYBL(a##b##c##d, HEX)
#define PPUTLBITSRA_7(a, b, c, d, e, f, g, h, i, j, k, l)           \
  PPUTLIMPL_NYBL(a##a##a##a, HEX), PPUTLIMPL_NYBL(a##a##a##a, HEX), \
      PPUTLIMPL_NYBL(b##c##d##e, HEX)
#define PPUTLBITSRA_6(a, b, c, d, e, f, g, h, i, j, k, l)           \
  PPUTLIMPL_NYBL(a##a##a##a, HEX), PPUTLIMPL_NYBL(a##a##a##b, HEX), \
      PPUTLIMPL_NYBL(c##d##e##f, HEX)
#define PPUTLBITSRA_5(a, b, c, d, e, f, g, h, i, j, k, l)           \
  PPUTLIMPL_NYBL(a##a##a##a, HEX), PPUTLIMPL_NYBL(a##a##b##c, HEX), \
      PPUTLIMPL_NYBL(d##e##f##g, HEX)
#define PPUTLBITSRA_4(a, b, c, d, e, f, g, h, i, j, k, l)           \
  PPUTLIMPL_NYBL(a##a##a##a, HEX), PPUTLIMPL_NYBL(a##b##c##d, HEX), \
      PPUTLIMPL_NYBL(e##f##g##h, HEX)
#define PPUTLBITSRA_3(a, b, c, d, e, f, g, h, i, j, k, l)           \
  PPUTLIMPL_NYBL(a##a##a##a, HEX), PPUTLIMPL_NYBL(b##c##d##e, HEX), \
      PPUTLIMPL_NYBL(f##g##h##i, HEX)
#define PPUTLBITSRA_2(a, b, c, d, e, f, g, h, i, j, k, l)           \
  PPUTLIMPL_NYBL(a##a##a##b, HEX), PPUTLIMPL_NYBL(c##d##e##f, HEX), \
      PPUTLIMPL_NYBL(g##h##i##j, HEX)
#define PPUTLBITSRA_1(a, b, c, d, e, f, g, h, i, j, k, l)           \
  PPUTLIMPL_NYBL(a##a##b##c, HEX), PPUTLIMPL_NYBL(d##e##f##g, HEX), \
      PPUTLIMPL_NYBL(h##i##j##k, HEX)
#define PPUTLBITSRA_0(a, b, c, d, e, f, g, h, i, j, k, l)           \
  PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
      PPUTLIMPL_NYBL(i##j##k##l, HEX)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [bitwise.bitnot]
/// ----------------
/// bitwise NOT.
///
/// PTL_BITNOT(0u)    // 4095u
/// PTL_BITNOT(0xFFF) // 0x000
#define PTL_BITNOT(/* word */...) /* -> word */ \
  PTL_WORD(PPUTLIMPL_UHEX(PTL_UHEX(__VA_ARGS__), BNOT), PTL_TYPEOF(__VA_ARGS__))

/// [bitwise.bitand]
/// ----------------
/// bitwise AND.
///
/// returns unsigned if either operand is unsigned,
/// decimal if either operand is decimal (and the
/// result is non-negative), and hex otherwise.
///
/// PTL_BITAND(0, 0)         // 0
/// PTL_BITAND(0, 1)         // 0
/// PTL_BITAND(3, 2)         // 2
/// PTL_BITAND(5, 6)         // 4
/// PTL_BITAND(0x800, 0xFFF) // 0x800
#define PTL_BITAND(/* word, word */...) /* -> word */ PPUTLBITAND_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBITAND_o(a, b)                                                         \
  PPUTLBITAND_RES(                                                                  \
      PPUTLIMPL_XARITHHINT(PTL_TYPEOF(a), PTL_TYPEOF(b)),                           \
      PPUTLBITAND_R(PPUTLBITAND_R(PPUTLBITAND_R(PPUTLIMPL_UHEX(PTL_UHEX(a), HDUMP), \
                                                PPUTLIMPL_UHEX(PTL_UHEX(b), HDUMP)))))
#define PPUTLBITAND_RES(...)                       PPUTLBITAND_RES_o(__VA_ARGS__)
#define PPUTLBITAND_RES_o(_type, a, b, c, d, e, f) PTL_WORD(0x##a##b##c##u, _type)
#define PPUTLBITAND_R(...)                         PPUTLBITAND_R_o(__VA_ARGS__)
#define PPUTLBITAND_R_o(a, b, c, d, e, f)          PPUTLIMPL_HEXHEX(c##f, AND), a, b, f, d, e

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [bitwise.bitor]
/// ---------------
/// bitwise OR.
///
/// returns unsigned if either operand is unsigned,
/// decimal if either operand is decimal (and the
/// result is non-negative), and hex otherwise.
///
/// PTL_BITOR(0, 0)        // 0
/// PTL_BITOR(0, 1)        // 1
/// PTL_BITOR(3, 4)        // 7
/// PTL_BITOR(5, 6)        // 7
/// PTL_BITOR(0x800, 2047) // 0xFFF
#define PTL_BITOR(/* word, word */...) /* -> word */ PPUTLBITOR_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBITOR_o(a, b)                                                       \
  PPUTLBITOR_RES(                                                                \
      PPUTLIMPL_XARITHHINT(PTL_TYPEOF(a), PTL_TYPEOF(b)),                        \
      PPUTLBITOR_R(PPUTLBITOR_R(PPUTLBITOR_R(PPUTLIMPL_UHEX(PTL_UHEX(a), HDUMP), \
                                             PPUTLIMPL_UHEX(PTL_UHEX(b), HDUMP)))))
#define PPUTLBITOR_RES(...)                       PPUTLBITOR_RES_o(__VA_ARGS__)
#define PPUTLBITOR_RES_o(_type, a, b, c, d, e, f) PTL_WORD(0x##a##b##c##u, _type)
#define PPUTLBITOR_R(...)                         PPUTLBITOR_R_o(__VA_ARGS__)
#define PPUTLBITOR_R_o(a, b, c, d, e, f)          PPUTLIMPL_HEXHEX(c##f, OR), a, b, f, d, e

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [bitwise.bitxor]
/// ----------------
/// bitwise XOR.
///
/// returns unsigned if either operand is unsigned,
/// decimal if either operand is decimal (and the
/// result is non-negative), and hex otherwise.
///
/// PTL_BITXOR(0, 0)         // 0
/// PTL_BITXOR(0, 1)         // 1
/// PTL_BITXOR(2, 1)         // 3
/// PTL_BITXOR(2, 3)         // 1
/// PTL_BITXOR(3, 4)         // 7
/// PTL_BITXOR(5, 6)         // 3
/// PTL_BITXOR(0x800, 0xFFF) // 0x7FF
#define PTL_BITXOR(/* word, word */...) /* -> word */ PPUTLBITXOR_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBITXOR_o(a, b)                                                         \
  PPUTLBITXOR_RES(                                                                  \
      PPUTLIMPL_XARITHHINT(PTL_TYPEOF(a), PTL_TYPEOF(b)),                           \
      PPUTLBITXOR_R(PPUTLBITXOR_R(PPUTLBITXOR_R(PPUTLIMPL_UHEX(PTL_UHEX(a), HDUMP), \
                                                PPUTLIMPL_UHEX(PTL_UHEX(b), HDUMP)))))
#define PPUTLBITXOR_RES(...)                       PPUTLBITXOR_RES_o(__VA_ARGS__)
#define PPUTLBITXOR_RES_o(_type, a, b, c, d, e, f) PTL_WORD(0x##a##b##c##u, _type)
#define PPUTLBITXOR_R(...)                         PPUTLBITXOR_R_o(__VA_ARGS__)
#define PPUTLBITXOR_R_o(a, b, c, d, e, f)          PPUTLIMPL_HEXHEX(c##f, XOR), a, b, f, d, e

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [bitwise.bitnand]
/// -----------------
/// bitwise NAND.
///
/// returns unsigned if either operand is unsigned,
/// decimal if either operand is decimal (and the
/// result is non-negative), and hex otherwise.
///
/// PTL_BITNAND(0, 0) // 0xFFF
/// PTL_BITNAND(5, 7) // 0xFFA
#define PTL_BITNAND(/* word, word */...) /* -> word */ PPUTLBITNAND_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBITNAND_o(a, b)                                 \
  PTL_WORD(PPUTLIMPL_UHEX(PTL_UHEX(PTL_BITAND(a, b)), BNOT), \
           PPUTLIMPL_XARITHHINT(PTL_TYPEOF(a), PTL_TYPEOF(b)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [bitwise.bitnor]
/// ----------------
/// bitwise NOR.
///
/// returns unsigned if either operand is unsigned,
/// decimal if either operand is decimal (and the
/// result is non-negative), and hex otherwise.
///
/// PTL_BITNOR(0, 0) // 0xFFF
/// PTL_BITNOR(0, 1) // 0xFFE
/// PTL_BITNOR(5, 7) // 0xFF8
/// PTL_BITNOR(7, 8) // 0xFF0
#define PTL_BITNOR(/* word, word */...) /* -> word */ PPUTLBITNOR_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBITNOR_o(a, b)                                 \
  PTL_WORD(PPUTLIMPL_UHEX(PTL_UHEX(PTL_BITOR(a, b)), BNOT), \
           PPUTLIMPL_XARITHHINT(PTL_TYPEOF(a), PTL_TYPEOF(b)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [bitwise.bitxnor]
/// -----------------
/// bitwise XNOR.
///
/// returns unsigned if either operand is unsigned,
/// decimal if either operand is decimal (and the
/// result is non-negative), and hex otherwise.
///
/// PTL_BITXNOR(0, 0)  // 0xFFF
/// PTL_BITXNOR(0, 1)  // 0xFFE
/// PTL_BITXNOR(5, 7)  // 0xFFD
/// PTL_BITXNOR(15, 8) // 0xFF8
#define PTL_BITXNOR(/* word, word */...) /* -> word */ PPUTLBITXNOR_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBITXNOR_o(a, b)                                 \
  PTL_WORD(PPUTLIMPL_UHEX(PTL_UHEX(PTL_BITXOR(a, b)), BNOT), \
           PPUTLIMPL_XARITHHINT(PTL_TYPEOF(a), PTL_TYPEOF(b)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [bitwise.bitget]
/// ----------------
/// gets the ith bit from the word, indexed from least to most significant.
/// fails on invalid bit index.
///
/// PTL_BITGET(2, 2)              // 0
/// PTL_BITGET(2, 1)              // 1
/// PTL_BITGET(2, 0)              // 0
/// PTL_BITGET(5u, 2)             // 1
/// PTL_BITGET(0xFFE, 1)          // 1
/// PTL_BITGET(0xFFEu, 0)         // 0
/// PTL_BITGET(0x800, PTL_NEG(1)) // 1
#define PTL_BITGET(/* word, ofs */...) /* -> bool */ \
  PPUTLBITGET_o(PTL_STR("[PTL_BITGET] invalid index" : __VA_ARGS__), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBITGET_o(e, v, i)                                                     \
  PPUTLBITGET_oo(PTL_IDEC(PPUTLIMPL_INDEX(PTL_UHEX(i), PTL_IS_INT(i), 0x00Cu, e)), \
                 PTL_BITDUMP(v))
#define PPUTLBITGET_oo(i, ...)                             PTL_CAT(PPUTLBITGET_, i)(__VA_ARGS__)
#define PPUTLBITGET_11(a, b, c, d, e, f, g, h, i, j, k, l) a
#define PPUTLBITGET_10(a, b, c, d, e, f, g, h, i, j, k, l) b
#define PPUTLBITGET_9(a, b, c, d, e, f, g, h, i, j, k, l)  c
#define PPUTLBITGET_8(a, b, c, d, e, f, g, h, i, j, k, l)  d
#define PPUTLBITGET_7(a, b, c, d, e, f, g, h, i, j, k, l)  e
#define PPUTLBITGET_6(a, b, c, d, e, f, g, h, i, j, k, l)  f
#define PPUTLBITGET_5(a, b, c, d, e, f, g, h, i, j, k, l)  g
#define PPUTLBITGET_4(a, b, c, d, e, f, g, h, i, j, k, l)  h
#define PPUTLBITGET_3(a, b, c, d, e, f, g, h, i, j, k, l)  i
#define PPUTLBITGET_2(a, b, c, d, e, f, g, h, i, j, k, l)  j
#define PPUTLBITGET_1(a, b, c, d, e, f, g, h, i, j, k, l)  k
#define PPUTLBITGET_0(a, b, c, d, e, f, g, h, i, j, k, l)  l

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [bitwise.bitset]
/// ----------------
/// sets the ith bit of the word to b, indexed from least to most significant.
/// fails on invalid bit index.
///
/// PTL_BITSET(0, 1, 1)      // 2
/// PTL_BITSET(1u, 2, 1)     // 5u
/// PTL_BITSET(5, 4, 1)      // 21
/// PTL_BITSET(0x002, 0, 1)  // 0x003
/// PTL_BITSET(0x003u, 0, 0) // 0x002u
#define PTL_BITSET(/* word, ofs, bool */...) /* -> word */ \
  PPUTLBITSET_o(PTL_STR("[PTL_BITSET] invalid index" : __VA_ARGS__), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBITSET_o(e, v, i, b)                                                \
  PTL_WORD(PPUTLIMPL_HEX_CAT(PPUTLBITSET_oo(                                     \
               PTL_IDEC(PPUTLIMPL_INDEX(PTL_UHEX(i), PTL_IS_INT(i), 0x00Cu, e)), \
               PTL_BOOL(b), PTL_BITDUMP(v))),                                    \
           PTL_TYPEOF(v))
#define PPUTLBITSET_oo(i, ...) PTL_CAT(PPUTLBITSET_, i)(__VA_ARGS__)
#define PPUTLBITSET_11(a, _, b, c, d, e, f, g, h, i, j, k, l)       \
  PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
      PPUTLIMPL_NYBL(i##j##k##l, HEX)
#define PPUTLBITSET_10(b, a, _, c, d, e, f, g, h, i, j, k, l)       \
  PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
      PPUTLIMPL_NYBL(i##j##k##l, HEX)
#define PPUTLBITSET_9(c, a, b, _, d, e, f, g, h, i, j, k, l)        \
  PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
      PPUTLIMPL_NYBL(i##j##k##l, HEX)
#define PPUTLBITSET_8(d, a, b, c, _, e, f, g, h, i, j, k, l)        \
  PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
      PPUTLIMPL_NYBL(i##j##k##l, HEX)
#define PPUTLBITSET_7(e, a, b, c, d, _, f, g, h, i, j, k, l)        \
  PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
      PPUTLIMPL_NYBL(i##j##k##l, HEX)
#define PPUTLBITSET_6(f, a, b, c, d, e, _, g, h, i, j, k, l)        \
  PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
      PPUTLIMPL_NYBL(i##j##k##l, HEX)
#define PPUTLBITSET_5(g, a, b, c, d, e, f, _, h, i, j, k, l)        \
  PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
      PPUTLIMPL_NYBL(i##j##k##l, HEX)
#define PPUTLBITSET_4(h, a, b, c, d, e, f, g, _, i, j, k, l)        \
  PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
      PPUTLIMPL_NYBL(i##j##k##l, HEX)
#define PPUTLBITSET_3(i, a, b, c, d, e, f, g, h, _, j, k, l)        \
  PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
      PPUTLIMPL_NYBL(i##j##k##l, HEX)
#define PPUTLBITSET_2(j, a, b, c, d, e, f, g, h, i, _, k, l)        \
  PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
      PPUTLIMPL_NYBL(i##j##k##l, HEX)
#define PPUTLBITSET_1(k, a, b, c, d, e, f, g, h, i, j, _, l)        \
  PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
      PPUTLIMPL_NYBL(i##j##k##l, HEX)
#define PPUTLBITSET_0(l, a, b, c, d, e, f, g, h, i, j, k, _)        \
  PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
      PPUTLIMPL_NYBL(i##j##k##l, HEX)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [bitwise.bitflip]
/// -----------------
/// flips the ith bit in the uint. indexed from least to most significant.
/// fails on invalid bit index.
///
/// PTL_BITFLIP(0, 0)      // 1
/// PTL_BITFLIP(1u, 1)     // 3u
/// PTL_BITFLIP(0x002, 2)  // 0x006
/// PTL_BITFLIP(0x003u, 3) // 0x00Bu
#define PTL_BITFLIP(/* word, ofs */...) /* -> word */ \
  PTL_BITSET(__VA_ARGS__, PTL_NOT(PTL_BITGET(__VA_ARGS__)))

/// [bitwise.bitrotl]
/// -----------------
/// bitwise left rotation by n places.
///
/// PTL_BITROTL(0x000, 0) // 0x000
/// PTL_BITROTL(0x001)    // 0x002
/// PTL_BITROTL(0x001, 1) // 0x002
/// PTL_BITROTL(0x001, 2) // 0x004
/// PTL_BITROTL(0x003, 2) // 0x00C
#define PTL_BITROTL(/* word, size=1 */...) /* -> word */ PPUTLBITROTL_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBITROTL_o(v, ...)                                                      \
  PTL_WORD(PPUTLIMPL_HEX_CAT(PPUTLBITROTL_oo(PTL_SIZE(PTL_DEFAULT(1, __VA_ARGS__)), \
                                             PTL_ESC(PTL_BITDUMP(v)))),             \
           PTL_TYPEOF(v))
#define PPUTLBITROTL_oo(n, ...) PTL_XCAT(PPUTLBITROTL_, PTL_BITAND(n, 0x00F))(__VA_ARGS__)
#define PPUTLBITROTL_15(a, b, c, d, e, f, g, h, i, j, k, l)         \
  PPUTLIMPL_NYBL(d##e##f##g, HEX), PPUTLIMPL_NYBL(h##i##j##k, HEX), \
      PPUTLIMPL_NYBL(l##a##b##c, HEX)
#define PPUTLBITROTL_14(a, b, c, d, e, f, g, h, i, j, k, l)         \
  PPUTLIMPL_NYBL(c##d##e##f, HEX), PPUTLIMPL_NYBL(g##h##i##j, HEX), \
      PPUTLIMPL_NYBL(k##l##a##b, HEX)
#define PPUTLBITROTL_13(a, b, c, d, e, f, g, h, i, j, k, l)         \
  PPUTLIMPL_NYBL(b##c##d##e, HEX), PPUTLIMPL_NYBL(f##g##h##i, HEX), \
      PPUTLIMPL_NYBL(j##k##l##a, HEX)
#define PPUTLBITROTL_12(a, b, c, d, e, f, g, h, i, j, k, l)         \
  PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
      PPUTLIMPL_NYBL(i##j##k##l, HEX)
#define PPUTLBITROTL_11(a, b, c, d, e, f, g, h, i, j, k, l)         \
  PPUTLIMPL_NYBL(l##a##b##c, HEX), PPUTLIMPL_NYBL(d##e##f##g, HEX), \
      PPUTLIMPL_NYBL(h##i##j##k, HEX)
#define PPUTLBITROTL_10(a, b, c, d, e, f, g, h, i, j, k, l)         \
  PPUTLIMPL_NYBL(k##l##a##b, HEX), PPUTLIMPL_NYBL(c##d##e##f, HEX), \
      PPUTLIMPL_NYBL(g##h##i##j, HEX)
#define PPUTLBITROTL_9(a, b, c, d, e, f, g, h, i, j, k, l)          \
  PPUTLIMPL_NYBL(j##k##l##a, HEX), PPUTLIMPL_NYBL(b##c##d##e, HEX), \
      PPUTLIMPL_NYBL(f##g##h##i, HEX)
#define PPUTLBITROTL_8(a, b, c, d, e, f, g, h, i, j, k, l)          \
  PPUTLIMPL_NYBL(i##j##k##l, HEX), PPUTLIMPL_NYBL(a##b##c##d, HEX), \
      PPUTLIMPL_NYBL(e##f##g##h, HEX)
#define PPUTLBITROTL_7(a, b, c, d, e, f, g, h, i, j, k, l)          \
  PPUTLIMPL_NYBL(h##i##j##k, HEX), PPUTLIMPL_NYBL(l##a##b##c, HEX), \
      PPUTLIMPL_NYBL(d##e##f##g, HEX)
#define PPUTLBITROTL_6(a, b, c, d, e, f, g, h, i, j, k, l)          \
  PPUTLIMPL_NYBL(g##h##i##j, HEX), PPUTLIMPL_NYBL(k##l##a##b, HEX), \
      PPUTLIMPL_NYBL(c##d##e##f, HEX)
#define PPUTLBITROTL_5(a, b, c, d, e, f, g, h, i, j, k, l)          \
  PPUTLIMPL_NYBL(f##g##h##i, HEX), PPUTLIMPL_NYBL(j##k##l##a, HEX), \
      PPUTLIMPL_NYBL(b##c##d##e, HEX)
#define PPUTLBITROTL_4(a, b, c, d, e, f, g, h, i, j, k, l)          \
  PPUTLIMPL_NYBL(e##f##g##h, HEX), PPUTLIMPL_NYBL(i##j##k##l, HEX), \
      PPUTLIMPL_NYBL(a##b##c##d, HEX)
#define PPUTLBITROTL_3(a, b, c, d, e, f, g, h, i, j, k, l)          \
  PPUTLIMPL_NYBL(d##e##f##g, HEX), PPUTLIMPL_NYBL(h##i##j##k, HEX), \
      PPUTLIMPL_NYBL(l##a##b##c, HEX)
#define PPUTLBITROTL_2(a, b, c, d, e, f, g, h, i, j, k, l)          \
  PPUTLIMPL_NYBL(c##d##e##f, HEX), PPUTLIMPL_NYBL(g##h##i##j, HEX), \
      PPUTLIMPL_NYBL(k##l##a##b, HEX)
#define PPUTLBITROTL_1(a, b, c, d, e, f, g, h, i, j, k, l)          \
  PPUTLIMPL_NYBL(b##c##d##e, HEX), PPUTLIMPL_NYBL(f##g##h##i, HEX), \
      PPUTLIMPL_NYBL(j##k##l##a, HEX)
#define PPUTLBITROTL_0(a, b, c, d, e, f, g, h, i, j, k, l)          \
  PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
      PPUTLIMPL_NYBL(i##j##k##l, HEX)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [bitwise.bitrotr]
/// -----------------
/// bitwise right rotation by n places.
///
/// PTL_BITROTR(0x000, 0) // 0x000
/// PTL_BITROTR(0x001)    // 0x800
/// PTL_BITROTR(0x001, 1) // 0x800
/// PTL_BITROTR(0x002, 1) // 0x001
/// PTL_BITROTR(0x7FF, 2) // 0xDFF
#define PTL_BITROTR(/* word, size=1 */...) /* -> word */ PPUTLBITROTR_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBITROTR_o(v, ...)                                                          \
  PTL_WORD(PPUTLIMPL_HEX_CAT(                                                           \
               PPUTLBITROTR_oo(PTL_SIZE(PTL_DEFAULT(1, __VA_ARGS__)), PTL_BITDUMP(v))), \
           PTL_TYPEOF(v))
#define PPUTLBITROTR_oo(n, ...) PTL_XCAT(PPUTLBITROTR_, PTL_BITAND(n, 0x00F))(__VA_ARGS__)
#define PPUTLBITROTR_15(a, b, c, d, e, f, g, h, i, j, k, l)         \
  PPUTLIMPL_NYBL(j##k##l##a, HEX), PPUTLIMPL_NYBL(b##c##d##e, HEX), \
      PPUTLIMPL_NYBL(f##g##h##i, HEX)
#define PPUTLBITROTR_14(a, b, c, d, e, f, g, h, i, j, k, l)         \
  PPUTLIMPL_NYBL(k##l##a##b, HEX), PPUTLIMPL_NYBL(c##d##e##f, HEX), \
      PPUTLIMPL_NYBL(g##h##i##j, HEX)
#define PPUTLBITROTR_13(a, b, c, d, e, f, g, h, i, j, k, l)         \
  PPUTLIMPL_NYBL(l##a##b##c, HEX), PPUTLIMPL_NYBL(d##e##f##g, HEX), \
      PPUTLIMPL_NYBL(h##i##j##k, HEX)
#define PPUTLBITROTR_12(a, b, c, d, e, f, g, h, i, j, k, l)         \
  PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
      PPUTLIMPL_NYBL(i##j##k##l, HEX)
#define PPUTLBITROTR_11(a, b, c, d, e, f, g, h, i, j, k, l)         \
  PPUTLIMPL_NYBL(b##c##d##e, HEX), PPUTLIMPL_NYBL(f##g##h##i, HEX), \
      PPUTLIMPL_NYBL(j##k##l##a, HEX)
#define PPUTLBITROTR_10(a, b, c, d, e, f, g, h, i, j, k, l)         \
  PPUTLIMPL_NYBL(c##d##e##f, HEX), PPUTLIMPL_NYBL(g##h##i##j, HEX), \
      PPUTLIMPL_NYBL(k##l##a##b, HEX)
#define PPUTLBITROTR_9(a, b, c, d, e, f, g, h, i, j, k, l)          \
  PPUTLIMPL_NYBL(d##e##f##g, HEX), PPUTLIMPL_NYBL(h##i##j##k, HEX), \
      PPUTLIMPL_NYBL(l##a##b##c, HEX)
#define PPUTLBITROTR_8(a, b, c, d, e, f, g, h, i, j, k, l)          \
  PPUTLIMPL_NYBL(e##f##g##h, HEX), PPUTLIMPL_NYBL(i##j##k##l, HEX), \
      PPUTLIMPL_NYBL(a##b##c##d, HEX)
#define PPUTLBITROTR_7(a, b, c, d, e, f, g, h, i, j, k, l)          \
  PPUTLIMPL_NYBL(f##g##h##i, HEX), PPUTLIMPL_NYBL(j##k##l##a, HEX), \
      PPUTLIMPL_NYBL(b##c##d##e, HEX)
#define PPUTLBITROTR_6(a, b, c, d, e, f, g, h, i, j, k, l)          \
  PPUTLIMPL_NYBL(g##h##i##j, HEX), PPUTLIMPL_NYBL(k##l##a##b, HEX), \
      PPUTLIMPL_NYBL(c##d##e##f, HEX)
#define PPUTLBITROTR_5(a, b, c, d, e, f, g, h, i, j, k, l)          \
  PPUTLIMPL_NYBL(h##i##j##k, HEX), PPUTLIMPL_NYBL(l##a##b##c, HEX), \
      PPUTLIMPL_NYBL(d##e##f##g, HEX)
#define PPUTLBITROTR_4(a, b, c, d, e, f, g, h, i, j, k, l)          \
  PPUTLIMPL_NYBL(i##j##k##l, HEX), PPUTLIMPL_NYBL(a##b##c##d, HEX), \
      PPUTLIMPL_NYBL(e##f##g##h, HEX)
#define PPUTLBITROTR_3(a, b, c, d, e, f, g, h, i, j, k, l)          \
  PPUTLIMPL_NYBL(j##k##l##a, HEX), PPUTLIMPL_NYBL(b##c##d##e, HEX), \
      PPUTLIMPL_NYBL(f##g##h##i, HEX)
#define PPUTLBITROTR_2(a, b, c, d, e, f, g, h, i, j, k, l)          \
  PPUTLIMPL_NYBL(k##l##a##b, HEX), PPUTLIMPL_NYBL(c##d##e##f, HEX), \
      PPUTLIMPL_NYBL(g##h##i##j, HEX)
#define PPUTLBITROTR_1(a, b, c, d, e, f, g, h, i, j, k, l)          \
  PPUTLIMPL_NYBL(l##a##b##c, HEX), PPUTLIMPL_NYBL(d##e##f##g, HEX), \
      PPUTLIMPL_NYBL(h##i##j##k, HEX)
#define PPUTLBITROTR_0(a, b, c, d, e, f, g, h, i, j, k, l)          \
  PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
      PPUTLIMPL_NYBL(i##j##k##l, HEX)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [meta.lp]
/// ---------
/// hides a left paren behind a secondary expansion.
#define PTL_LP() /* -> obj */ (

/// [meta.rp]
/// ---------
/// hides a right paren behind a secondary expansion.
///
/// PTL_LP() PTL_RP() // ( )
#define PTL_RP() /* -> obj> */ )

/// [meta.x]
/// --------
/// adds an additional expansion.
///
/// PTL_X()                                      // <nothing>
/// PTL_X(foo)                                   // foo
/// PTL_X(a, b, c)                               // a, b, c
/// PTL_XSTR(PTL_INC PTL_LP() 3 PTL_RP())        // "PTL_INC ( 3 )"
/// PTL_XSTR(PTL_X(PTL_INC PTL_LP() 3 PTL_RP())) // "4"
#define PTL_X(/* any... */...) /* -> any... */ __VA_ARGS__

/// [meta.xtrace]
/// -------------
/// tracks the number of expansions undergone after expression.
/// uses mutual recursion; can track any number of expansions.
/// the number of commas indicates the number of expansions.
///
/// PTL_XSTR(PTL_XTRACE)                      // "PPUTLXTRACE_A ( , )"
/// PTL_XSTR(PTL_X(PTL_XTRACE))               // "PPUTLXTRACE_B ( ,, )"
/// PTL_XSTR(PTL_X(PTL_X(PTL_XTRACE)))        // "PPUTLXTRACE_A ( ,,, )"
/// PTL_XSTR(PTL_X(PTL_X(PTL_X(PTL_XTRACE)))) // "PPUTLXTRACE_B ( ,,,, )"
#define PTL_XTRACE /* -> obj */ PPUTLXTRACE_A PTL_LP() /**/, PTL_RP()

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLXTRACE_B(...) PPUTLXTRACE_A PTL_LP() __VA_ARGS__, PTL_RP()
#define PPUTLXTRACE_A(...) PPUTLXTRACE_B PTL_LP() __VA_ARGS__, PTL_RP()

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [meta.xtrace_read]
/// ------------------
/// measures an xtrace expr to determine the number of expansions it experienced.
/// ignores the expansion required to read the result.
///
/// fails if xtrace is not a valid xtrace expression.
/// PTL_SIZE will fail if the xtrace expression is too large.
///
/// PTL_XTRACE_READ(PTL_XTRACE)                      // 0u
/// PTL_XTRACE_READ(PTL_X(PTL_XTRACE))               // 1u
/// PTL_XTRACE_READ(PTL_X(PTL_X(PTL_XTRACE)))        // 2u
/// PTL_XTRACE_READ(PTL_X(PTL_X(PTL_X(PTL_XTRACE)))) // 3u
#define PTL_XTRACE_READ(/* obj */...) /* -> udec&size */                 \
  PTL_XCAT(PPUTLXTRACE_READ_,                                            \
           PTL_IS_NONE(PTL_XCAT(PPUTLXTRACE_READ_DETECT_, __VA_ARGS__))) \
  (PTL_STR("[PTL_XTRACE_READ] invalid xtrace expr" : __VA_ARGS__), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLXTRACE_READ_1(err, ...)           PPUTLXTRACE_READ_##__VA_ARGS__
#define PPUTLXTRACE_READ_0(err, ...)           PTL_FAIL(err)
#define PPUTLXTRACE_READ_PPUTLXTRACE_B(_, ...) PPUTLXTRACE_READ_RES(__VA_ARGS__.)
#define PPUTLXTRACE_READ_PPUTLXTRACE_A(_, ...) PPUTLXTRACE_READ_RES(__VA_ARGS__.)
#define PPUTLXTRACE_READ_RES(_, ...)           PTL_COUNTOF(__VA_ARGS__)
#define PPUTLXTRACE_READ_DETECT_PPUTLXTRACE_B(...)
#define PPUTLXTRACE_READ_DETECT_PPUTLXTRACE_A(...)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [meta.recur_lp]
/// ---------------
/// constructs an in-place recursive expression.
/// the result must be expanded once to execute.
///
/// must be followed by an recur_rp expression with
/// the same value of n.
///
/// recur_lp repeats the function and open parens;
/// recur_rp repeats the close parens.
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
///   mul(a, b)  = first(x(recur_lp(a, madd) 0, b recur_rp(a)))
///   mul(2, 4) -> first(x(madd LP madd LP 0, 4 RP RP))
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
#define PTL_RECUR_LP(/* size, fn: obj */...) /* -> obj */ PPUTLRLP_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLRLP_o(n, f) PPUTLRLP_oo(PTL_SIZE(n, UDEC), f)
#define PPUTLRLP_oo(n, f) \
  PTL_XCAT(PPUTLRLP_oo_, PTL_BITAND(n, 3))(PTL_IDEC(PTL_BITSRL(n, 2)), f)
#define PPUTLRLP_oo_3u(n, f) f PTL_LP() f PTL_LP() f PTL_LP() PTL_XCAT(PPUTLRLP_, n)(f)
#define PPUTLRLP_oo_2u(n, f) f PTL_LP() f PTL_LP() PTL_XCAT(PPUTLRLP_, n)(f)
#define PPUTLRLP_oo_1u(n, f) f PTL_LP() PTL_XCAT(PPUTLRLP_, n)(f)
#define PPUTLRLP_oo_0u(n, f) PTL_XCAT(PPUTLRLP_, n)(f)
#define PPUTLRLP_63(f)       PPUTLRLP_15(f) PPUTLRLP_16(f) PPUTLRLP_16(f) PPUTLRLP_16(f)
#define PPUTLRLP_62(f)       PPUTLRLP_15(f) PPUTLRLP_15(f) PPUTLRLP_16(f) PPUTLRLP_16(f)
#define PPUTLRLP_61(f)       PPUTLRLP_15(f) PPUTLRLP_15(f) PPUTLRLP_15(f) PPUTLRLP_16(f)
#define PPUTLRLP_60(f)       PPUTLRLP_15(f) PPUTLRLP_15(f) PPUTLRLP_15(f) PPUTLRLP_15(f)
#define PPUTLRLP_59(f)       PPUTLRLP_14(f) PPUTLRLP_15(f) PPUTLRLP_15(f) PPUTLRLP_15(f)
#define PPUTLRLP_58(f)       PPUTLRLP_14(f) PPUTLRLP_14(f) PPUTLRLP_15(f) PPUTLRLP_15(f)
#define PPUTLRLP_57(f)       PPUTLRLP_14(f) PPUTLRLP_14(f) PPUTLRLP_14(f) PPUTLRLP_15(f)
#define PPUTLRLP_56(f)       PPUTLRLP_14(f) PPUTLRLP_14(f) PPUTLRLP_14(f) PPUTLRLP_14(f)
#define PPUTLRLP_55(f)       PPUTLRLP_13(f) PPUTLRLP_14(f) PPUTLRLP_14(f) PPUTLRLP_14(f)
#define PPUTLRLP_54(f)       PPUTLRLP_13(f) PPUTLRLP_13(f) PPUTLRLP_14(f) PPUTLRLP_14(f)
#define PPUTLRLP_53(f)       PPUTLRLP_13(f) PPUTLRLP_13(f) PPUTLRLP_13(f) PPUTLRLP_14(f)
#define PPUTLRLP_52(f)       PPUTLRLP_13(f) PPUTLRLP_13(f) PPUTLRLP_13(f) PPUTLRLP_13(f)
#define PPUTLRLP_51(f)       PPUTLRLP_12(f) PPUTLRLP_13(f) PPUTLRLP_13(f) PPUTLRLP_13(f)
#define PPUTLRLP_50(f)       PPUTLRLP_12(f) PPUTLRLP_12(f) PPUTLRLP_13(f) PPUTLRLP_13(f)
#define PPUTLRLP_49(f)       PPUTLRLP_12(f) PPUTLRLP_12(f) PPUTLRLP_12(f) PPUTLRLP_13(f)
#define PPUTLRLP_48(f)       PPUTLRLP_12(f) PPUTLRLP_12(f) PPUTLRLP_12(f) PPUTLRLP_12(f)
#define PPUTLRLP_47(f)       PPUTLRLP_11(f) PPUTLRLP_12(f) PPUTLRLP_12(f) PPUTLRLP_12(f)
#define PPUTLRLP_46(f)       PPUTLRLP_11(f) PPUTLRLP_11(f) PPUTLRLP_12(f) PPUTLRLP_12(f)
#define PPUTLRLP_45(f)       PPUTLRLP_11(f) PPUTLRLP_11(f) PPUTLRLP_11(f) PPUTLRLP_12(f)
#define PPUTLRLP_44(f)       PPUTLRLP_11(f) PPUTLRLP_11(f) PPUTLRLP_11(f) PPUTLRLP_11(f)
#define PPUTLRLP_43(f)       PPUTLRLP_10(f) PPUTLRLP_11(f) PPUTLRLP_11(f) PPUTLRLP_11(f)
#define PPUTLRLP_42(f)       PPUTLRLP_10(f) PPUTLRLP_10(f) PPUTLRLP_11(f) PPUTLRLP_11(f)
#define PPUTLRLP_41(f)       PPUTLRLP_10(f) PPUTLRLP_10(f) PPUTLRLP_10(f) PPUTLRLP_11(f)
#define PPUTLRLP_40(f)       PPUTLRLP_10(f) PPUTLRLP_10(f) PPUTLRLP_10(f) PPUTLRLP_10(f)
#define PPUTLRLP_39(f)       PPUTLRLP_9(f) PPUTLRLP_10(f) PPUTLRLP_10(f) PPUTLRLP_10(f)
#define PPUTLRLP_38(f)       PPUTLRLP_9(f) PPUTLRLP_9(f) PPUTLRLP_10(f) PPUTLRLP_10(f)
#define PPUTLRLP_37(f)       PPUTLRLP_9(f) PPUTLRLP_9(f) PPUTLRLP_9(f) PPUTLRLP_10(f)
#define PPUTLRLP_36(f)       PPUTLRLP_9(f) PPUTLRLP_9(f) PPUTLRLP_9(f) PPUTLRLP_9(f)
#define PPUTLRLP_35(f)       PPUTLRLP_8(f) PPUTLRLP_9(f) PPUTLRLP_9(f) PPUTLRLP_9(f)
#define PPUTLRLP_34(f)       PPUTLRLP_8(f) PPUTLRLP_8(f) PPUTLRLP_9(f) PPUTLRLP_9(f)
#define PPUTLRLP_33(f)       PPUTLRLP_8(f) PPUTLRLP_8(f) PPUTLRLP_8(f) PPUTLRLP_9(f)
#define PPUTLRLP_32(f)       PPUTLRLP_8(f) PPUTLRLP_8(f) PPUTLRLP_8(f) PPUTLRLP_8(f)
#define PPUTLRLP_31(f)       PPUTLRLP_7(f) PPUTLRLP_8(f) PPUTLRLP_8(f) PPUTLRLP_8(f)
#define PPUTLRLP_30(f)       PPUTLRLP_7(f) PPUTLRLP_7(f) PPUTLRLP_8(f) PPUTLRLP_8(f)
#define PPUTLRLP_29(f)       PPUTLRLP_7(f) PPUTLRLP_7(f) PPUTLRLP_7(f) PPUTLRLP_8(f)
#define PPUTLRLP_28(f)       PPUTLRLP_7(f) PPUTLRLP_7(f) PPUTLRLP_7(f) PPUTLRLP_7(f)
#define PPUTLRLP_27(f)       PPUTLRLP_6(f) PPUTLRLP_7(f) PPUTLRLP_7(f) PPUTLRLP_7(f)
#define PPUTLRLP_26(f)       PPUTLRLP_6(f) PPUTLRLP_6(f) PPUTLRLP_7(f) PPUTLRLP_7(f)
#define PPUTLRLP_25(f)       PPUTLRLP_6(f) PPUTLRLP_6(f) PPUTLRLP_6(f) PPUTLRLP_7(f)
#define PPUTLRLP_24(f)       PPUTLRLP_6(f) PPUTLRLP_6(f) PPUTLRLP_6(f) PPUTLRLP_6(f)
#define PPUTLRLP_23(f)       PPUTLRLP_5(f) PPUTLRLP_6(f) PPUTLRLP_6(f) PPUTLRLP_6(f)
#define PPUTLRLP_22(f)       PPUTLRLP_5(f) PPUTLRLP_5(f) PPUTLRLP_6(f) PPUTLRLP_6(f)
#define PPUTLRLP_21(f)       PPUTLRLP_5(f) PPUTLRLP_5(f) PPUTLRLP_5(f) PPUTLRLP_6(f)
#define PPUTLRLP_20(f)       PPUTLRLP_5(f) PPUTLRLP_5(f) PPUTLRLP_5(f) PPUTLRLP_5(f)
#define PPUTLRLP_19(f)       PPUTLRLP_4(f) PPUTLRLP_5(f) PPUTLRLP_5(f) PPUTLRLP_5(f)
#define PPUTLRLP_18(f)       PPUTLRLP_4(f) PPUTLRLP_4(f) PPUTLRLP_5(f) PPUTLRLP_5(f)
#define PPUTLRLP_17(f)       PPUTLRLP_4(f) PPUTLRLP_4(f) PPUTLRLP_4(f) PPUTLRLP_5(f)
#define PPUTLRLP_16(f)       PPUTLRLP_4(f) PPUTLRLP_4(f) PPUTLRLP_4(f) PPUTLRLP_4(f)
#define PPUTLRLP_15(f)       PPUTLRLP_3(f) PPUTLRLP_4(f) PPUTLRLP_4(f) PPUTLRLP_4(f)
#define PPUTLRLP_14(f)       PPUTLRLP_3(f) PPUTLRLP_3(f) PPUTLRLP_4(f) PPUTLRLP_4(f)
#define PPUTLRLP_13(f)       PPUTLRLP_3(f) PPUTLRLP_3(f) PPUTLRLP_3(f) PPUTLRLP_4(f)
#define PPUTLRLP_12(f)       PPUTLRLP_3(f) PPUTLRLP_3(f) PPUTLRLP_3(f) PPUTLRLP_3(f)
#define PPUTLRLP_11(f)       PPUTLRLP_2(f) PPUTLRLP_3(f) PPUTLRLP_3(f) PPUTLRLP_3(f)
#define PPUTLRLP_10(f)       PPUTLRLP_2(f) PPUTLRLP_2(f) PPUTLRLP_3(f) PPUTLRLP_3(f)
#define PPUTLRLP_9(f)        PPUTLRLP_2(f) PPUTLRLP_2(f) PPUTLRLP_2(f) PPUTLRLP_3(f)
#define PPUTLRLP_8(f)        PPUTLRLP_2(f) PPUTLRLP_2(f) PPUTLRLP_2(f) PPUTLRLP_2(f)
#define PPUTLRLP_7(f)        PPUTLRLP_1(f) PPUTLRLP_2(f) PPUTLRLP_2(f) PPUTLRLP_2(f)
#define PPUTLRLP_6(f)        PPUTLRLP_1(f) PPUTLRLP_1(f) PPUTLRLP_2(f) PPUTLRLP_2(f)
#define PPUTLRLP_5(f)        PPUTLRLP_1(f) PPUTLRLP_1(f) PPUTLRLP_1(f) PPUTLRLP_2(f)
#define PPUTLRLP_4(f)        PPUTLRLP_1(f) PPUTLRLP_1(f) PPUTLRLP_1(f) PPUTLRLP_1(f)
#define PPUTLRLP_3(f)        PPUTLRLP_1(f) PPUTLRLP_1(f) PPUTLRLP_1(f)
#define PPUTLRLP_2(f)        PPUTLRLP_1(f) PPUTLRLP_1(f)
#define PPUTLRLP_1(f)        f PTL_LP() f PTL_LP() f PTL_LP() f PTL_LP()
#define PPUTLRLP_0(f)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [meta.recur_rp]
/// ---------------
/// constructs an in-place recursive expression.
/// the result must be expanded once to execute.
///
/// must be preceeded by an recur_lp expression with
/// the same value of n.
///
/// recur_lp repeats the function and open parens;
/// recur_rp repeats the close parens.
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
///   mul(a, b)  = first(x(recur_lp(a, madd) 0, 4 recur_rp(a)))
///   mul(2, 4) -> first(x(madd LP madd LP 0, 4 RP RP))
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
/// PTL_XSTR(PTL_RECUR_LP(0, PTL_INC) 0 PTL_RECUR_RP(0)) // "0"
/// PTL_XSTR(PTL_RECUR_LP(1, PTL_INC) 0 PTL_RECUR_RP(1)) // "PTL_INC ( 0 )"
/// PTL_XSTR(PTL_RECUR_LP(2, PTL_INC) 0 PTL_RECUR_RP(2)) // "PTL_INC ( PTL_INC ( 0 ) )"
/// PTL_X(PTL_RECUR_LP(3, PTL_INC) 0 PTL_RECUR_RP(3))    // 3
#define PTL_RECUR_RP(/* size */...) /* -> obj */ PPUTLRRP_o(PTL_SIZE(__VA_ARGS__, UDEC))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLRRP_o(n)    PTL_XCAT(PPUTLRRP_o_, PTL_BITAND(n, 3))(PTL_IDEC(PTL_BITSRL(n, 2)))
#define PPUTLRRP_o_3u(n) PTL_XCAT(PPUTLRRP_, n) PTL_RP() PTL_RP() PTL_RP()
#define PPUTLRRP_o_2u(n) PTL_XCAT(PPUTLRRP_, n) PTL_RP() PTL_RP()
#define PPUTLRRP_o_1u(n) PTL_XCAT(PPUTLRRP_, n) PTL_RP()
#define PPUTLRRP_o_0u(n) PTL_XCAT(PPUTLRRP_, n)
#define PPUTLRRP_63      PPUTLRRP_15 PPUTLRRP_16 PPUTLRRP_16 PPUTLRRP_16
#define PPUTLRRP_62      PPUTLRRP_15 PPUTLRRP_15 PPUTLRRP_16 PPUTLRRP_16
#define PPUTLRRP_61      PPUTLRRP_15 PPUTLRRP_15 PPUTLRRP_15 PPUTLRRP_16
#define PPUTLRRP_60      PPUTLRRP_15 PPUTLRRP_15 PPUTLRRP_15 PPUTLRRP_15
#define PPUTLRRP_59      PPUTLRRP_14 PPUTLRRP_15 PPUTLRRP_15 PPUTLRRP_15
#define PPUTLRRP_58      PPUTLRRP_14 PPUTLRRP_14 PPUTLRRP_15 PPUTLRRP_15
#define PPUTLRRP_57      PPUTLRRP_14 PPUTLRRP_14 PPUTLRRP_14 PPUTLRRP_15
#define PPUTLRRP_56      PPUTLRRP_14 PPUTLRRP_14 PPUTLRRP_14 PPUTLRRP_14
#define PPUTLRRP_55      PPUTLRRP_13 PPUTLRRP_14 PPUTLRRP_14 PPUTLRRP_14
#define PPUTLRRP_54      PPUTLRRP_13 PPUTLRRP_13 PPUTLRRP_14 PPUTLRRP_14
#define PPUTLRRP_53      PPUTLRRP_13 PPUTLRRP_13 PPUTLRRP_13 PPUTLRRP_14
#define PPUTLRRP_52      PPUTLRRP_13 PPUTLRRP_13 PPUTLRRP_13 PPUTLRRP_13
#define PPUTLRRP_51      PPUTLRRP_12 PPUTLRRP_13 PPUTLRRP_13 PPUTLRRP_13
#define PPUTLRRP_50      PPUTLRRP_12 PPUTLRRP_12 PPUTLRRP_13 PPUTLRRP_13
#define PPUTLRRP_49      PPUTLRRP_12 PPUTLRRP_12 PPUTLRRP_12 PPUTLRRP_13
#define PPUTLRRP_48      PPUTLRRP_12 PPUTLRRP_12 PPUTLRRP_12 PPUTLRRP_12
#define PPUTLRRP_47      PPUTLRRP_11 PPUTLRRP_12 PPUTLRRP_12 PPUTLRRP_12
#define PPUTLRRP_46      PPUTLRRP_11 PPUTLRRP_11 PPUTLRRP_12 PPUTLRRP_12
#define PPUTLRRP_45      PPUTLRRP_11 PPUTLRRP_11 PPUTLRRP_11 PPUTLRRP_12
#define PPUTLRRP_44      PPUTLRRP_11 PPUTLRRP_11 PPUTLRRP_11 PPUTLRRP_11
#define PPUTLRRP_43      PPUTLRRP_10 PPUTLRRP_11 PPUTLRRP_11 PPUTLRRP_11
#define PPUTLRRP_42      PPUTLRRP_10 PPUTLRRP_10 PPUTLRRP_11 PPUTLRRP_11
#define PPUTLRRP_41      PPUTLRRP_10 PPUTLRRP_10 PPUTLRRP_10 PPUTLRRP_11
#define PPUTLRRP_40      PPUTLRRP_10 PPUTLRRP_10 PPUTLRRP_10 PPUTLRRP_10
#define PPUTLRRP_39      PPUTLRRP_9 PPUTLRRP_10 PPUTLRRP_10 PPUTLRRP_10
#define PPUTLRRP_38      PPUTLRRP_9 PPUTLRRP_9 PPUTLRRP_10 PPUTLRRP_10
#define PPUTLRRP_37      PPUTLRRP_9 PPUTLRRP_9 PPUTLRRP_9 PPUTLRRP_10
#define PPUTLRRP_36      PPUTLRRP_9 PPUTLRRP_9 PPUTLRRP_9 PPUTLRRP_9
#define PPUTLRRP_35      PPUTLRRP_8 PPUTLRRP_9 PPUTLRRP_9 PPUTLRRP_9
#define PPUTLRRP_34      PPUTLRRP_8 PPUTLRRP_8 PPUTLRRP_9 PPUTLRRP_9
#define PPUTLRRP_33      PPUTLRRP_8 PPUTLRRP_8 PPUTLRRP_8 PPUTLRRP_9
#define PPUTLRRP_32      PPUTLRRP_8 PPUTLRRP_8 PPUTLRRP_8 PPUTLRRP_8
#define PPUTLRRP_31      PPUTLRRP_7 PPUTLRRP_8 PPUTLRRP_8 PPUTLRRP_8
#define PPUTLRRP_30      PPUTLRRP_7 PPUTLRRP_7 PPUTLRRP_8 PPUTLRRP_8
#define PPUTLRRP_29      PPUTLRRP_7 PPUTLRRP_7 PPUTLRRP_7 PPUTLRRP_8
#define PPUTLRRP_28      PPUTLRRP_7 PPUTLRRP_7 PPUTLRRP_7 PPUTLRRP_7
#define PPUTLRRP_27      PPUTLRRP_6 PPUTLRRP_7 PPUTLRRP_7 PPUTLRRP_7
#define PPUTLRRP_26      PPUTLRRP_6 PPUTLRRP_6 PPUTLRRP_7 PPUTLRRP_7
#define PPUTLRRP_25      PPUTLRRP_6 PPUTLRRP_6 PPUTLRRP_6 PPUTLRRP_7
#define PPUTLRRP_24      PPUTLRRP_6 PPUTLRRP_6 PPUTLRRP_6 PPUTLRRP_6
#define PPUTLRRP_23      PPUTLRRP_5 PPUTLRRP_6 PPUTLRRP_6 PPUTLRRP_6
#define PPUTLRRP_22      PPUTLRRP_5 PPUTLRRP_5 PPUTLRRP_6 PPUTLRRP_6
#define PPUTLRRP_21      PPUTLRRP_5 PPUTLRRP_5 PPUTLRRP_5 PPUTLRRP_6
#define PPUTLRRP_20      PPUTLRRP_5 PPUTLRRP_5 PPUTLRRP_5 PPUTLRRP_5
#define PPUTLRRP_19      PPUTLRRP_4 PPUTLRRP_5 PPUTLRRP_5 PPUTLRRP_5
#define PPUTLRRP_18      PPUTLRRP_4 PPUTLRRP_4 PPUTLRRP_5 PPUTLRRP_5
#define PPUTLRRP_17      PPUTLRRP_4 PPUTLRRP_4 PPUTLRRP_4 PPUTLRRP_5
#define PPUTLRRP_16      PPUTLRRP_4 PPUTLRRP_4 PPUTLRRP_4 PPUTLRRP_4
#define PPUTLRRP_15      PPUTLRRP_3 PPUTLRRP_4 PPUTLRRP_4 PPUTLRRP_4
#define PPUTLRRP_14      PPUTLRRP_3 PPUTLRRP_3 PPUTLRRP_4 PPUTLRRP_4
#define PPUTLRRP_13      PPUTLRRP_3 PPUTLRRP_3 PPUTLRRP_3 PPUTLRRP_4
#define PPUTLRRP_12      PPUTLRRP_3 PPUTLRRP_3 PPUTLRRP_3 PPUTLRRP_3
#define PPUTLRRP_11      PPUTLRRP_2 PPUTLRRP_3 PPUTLRRP_3 PPUTLRRP_3
#define PPUTLRRP_10      PPUTLRRP_2 PPUTLRRP_2 PPUTLRRP_3 PPUTLRRP_3
#define PPUTLRRP_9       PPUTLRRP_2 PPUTLRRP_2 PPUTLRRP_2 PPUTLRRP_3
#define PPUTLRRP_8       PPUTLRRP_2 PPUTLRRP_2 PPUTLRRP_2 PPUTLRRP_2
#define PPUTLRRP_7       PPUTLRRP_1 PPUTLRRP_2 PPUTLRRP_2 PPUTLRRP_2
#define PPUTLRRP_6       PPUTLRRP_1 PPUTLRRP_1 PPUTLRRP_2 PPUTLRRP_2
#define PPUTLRRP_5       PPUTLRRP_1 PPUTLRRP_1 PPUTLRRP_1 PPUTLRRP_2
#define PPUTLRRP_4       PPUTLRRP_1 PPUTLRRP_1 PPUTLRRP_1 PPUTLRRP_1
#define PPUTLRRP_3       PPUTLRRP_1 PPUTLRRP_1 PPUTLRRP_1
#define PPUTLRRP_2       PPUTLRRP_1 PPUTLRRP_1
#define PPUTLRRP_1       PTL_RP() PTL_RP() PTL_RP() PTL_RP()
#define PPUTLRRP_0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [control.if]
/// ------------
/// conditionally expands based on a boolean.
///
/// PTL_IF(1, t,)   // t
/// PTL_IF(0, t,)   // <nothing>
/// PTL_IF(1, t, f) // t
/// PTL_IF(0, t, f) // f
#define PTL_IF(/* bool, t: any, f: any */...) /* -> any */ \
  PTL_XCAT(PPUTLIF_, PTL_BOOL(PTL_FIRST(__VA_ARGS__)))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIF_1(_, t, ...) t
#define PPUTLIF_0(_, t, ...) PTL_ANY(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [control.switch]
/// ----------------
/// conditionally expands based on a size.
/// the final case is default.
///
/// PTL_SWITCH(0, 1)         // 1
/// PTL_SWITCH(1, 1)         // 1
/// PTL_SWITCH(2, 1)         // 1
/// PTL_SWITCH(1, 1, 2)      // 2
/// PTL_SWITCH(2, 1, 2)      // 2
/// PTL_SWITCH(255, 1, 2, 3) // 3
#define PTL_SWITCH(/* size, ...cases: any */...) /* -> any */                            \
  PPUTLSWITCH_RES(PPUTLSWITCH_X(PTL_RECUR_LP(PTL_FIRST(__VA_ARGS__), PPUTLSWITCH_R) /**/ \
                                __VA_ARGS__                                         /**/ \
                                    PTL_RECUR_RP(PTL_FIRST(__VA_ARGS__))))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLSWITCH_RES(...)         PPUTLSWITCH_RES_X(__VA_ARGS__)
#define PPUTLSWITCH_RES_X(i, _, ...) _
#define PPUTLSWITCH_R(...)           PPUTLSWITCH_R_X(__VA_ARGS__)
#define PPUTLSWITCH_R_X(i, _, ...)                                         \
  PTL_IF(PTL_OR(PTL_EQZ(i), PTL_IS_NONE(__VA_ARGS__)), PPUTLSWITCH_R_BASE, \
         PPUTLSWITCH_R_RECR)                                               \
  (i, _, __VA_ARGS__)
#define PPUTLSWITCH_R_RECR(i, _, ...) PTL_DEC(i), __VA_ARGS__
#define PPUTLSWITCH_R_BASE(i, _, ...) 0, _
#define PPUTLSWITCH_X(...)            __VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [math.add]
/// ----------
/// addition with overflow.
///
/// returns unsigned if either operand is unsigned,
/// decimal if either operand is decimal (and the
/// result is non-negative), and hex otherwise.
///
/// PTL_ADD(0, 0)         // 0
/// PTL_ADD(0, 1)         // 1
/// PTL_ADD(1, 2)         // 3
/// PTL_ADD(3u, 4)        // 7u
/// PTL_ADD(5, 6u)        // 11u
/// PTL_ADD(4095u, 1)     // 0u
/// PTL_ADD(4095u, 2)     // 1u
/// PTL_ADD(4095u, 4095u) // 4094u
/// PTL_ADD(2047, 1)      // 0x800
#define PTL_ADD(/* word, word */...) /* -> word */ PPUTLADD_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLADD_o(a, b)                            \
  PTL_WORD(PPUTLIMPL_ADD(PTL_UHEX(a), PTL_UHEX(b)), \
           PPUTLIMPL_XARITHHINT(PTL_TYPEOF(PTL_WORD(a)), PTL_TYPEOF(PTL_WORD(b))))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [math.sub]
/// ----------
/// subtraction with underflow.
///
/// returns unsigned if either operand is unsigned,
/// decimal if either operand is decimal (and the
/// result is non-negative), and hex otherwise.
///
/// PTL_SUB(0, 0)      // 0
/// PTL_SUB(0, 1)      // 0xFFF
/// PTL_SUB(0u, 1u)    // 4095u
/// PTL_SUB(1, 0)      // 1
/// PTL_SUB(1, 1)      // 0
/// PTL_SUB(3, 1)      // 2
/// PTL_SUB(1u, 3u)    // 4094u
/// PTL_SUB(0, 0x800)  // 0x800
/// PTL_SUB(0u, 0x800) // 2048u
#define PTL_SUB(/* word, word */...) /* -> word */ PPUTLSUB_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLSUB_o(a, b)                            \
  PTL_WORD(PPUTLIMPL_SUB(PTL_UHEX(a), PTL_UHEX(b)), \
           PPUTLIMPL_XARITHHINT(PTL_TYPEOF(PTL_WORD(a)), PTL_TYPEOF(PTL_WORD(b))))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [math.mul]
/// ----------
/// numeric multiplication with overflow.
///
/// returns unsigned if either operand is unsigned,
/// decimal if either operand is decimal (and the
/// result is non-negative), and hex otherwise.
///
/// PTL_MUL(0, 0)                   // 0
/// PTL_MUL(0, 1)                   // 0
/// PTL_MUL(1, 1)                   // 1
/// PTL_MUL(1, 2)                   // 2
/// PTL_MUL(2, 2)                   // 4
/// PTL_MUL(PTL_NEG(2), 2)          // 0xFFC
/// PTL_MUL(PTL_NEG(2), PTL_NEG(2)) // 0x004
/// PTL_MUL(4095u, 1)               // 4095u
/// PTL_MUL(4095u, 4095u)           // 1u
/// PTL_MUL(2047, 4095u)            // 2049u
#define PTL_MUL(/* word, word */...) /* -> word */ \
  PTL_IF(PTL_EQZ(PTL_REST(__VA_ARGS__)), PPUTLMUL_BEQZ, PPUTLMUL_BNEZ)(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLMUL_BNEZ(a, b)                                  \
  PTL_XCAT(PPUTLMUL_BNEZ_, PTL_XCAT(PTL_LTZ(a), PTL_LTZ(b))) \
  (PTL_TYPEOF(a), PTL_TYPEOF(b), a, b)
#define PPUTLMUL_BNEZ_11(ta, tb, a, b) \
  PPUTLMUL_BNEZ_11_o(PPUTLIMPL_ARITHHINT(ta, tb), PTL_NEG(a), PTL_NEG(b))
#define PPUTLMUL_BNEZ_11_o(hint, a, b)                                        \
  PTL_WORD(PPUTLMUL_RES(PPUTLMUL_X(PTL_RECUR_LP(PTL_LOG2(b), PPUTLMUL_R) /**/ \
                                   a,                                    /**/ \
                                   b,                                    /**/ \
                                   0                                     /**/ \
                                   PTL_RECUR_RP(PTL_LOG2(b)))),               \
           hint)
#define PPUTLMUL_BNEZ_10(ta, tb, a, b)                                                \
  PTL_NEG(PTL_WORD(PPUTLMUL_RES(PPUTLMUL_X(PTL_RECUR_LP(PTL_LOG2(b), PPUTLMUL_R) /**/ \
                                           PTL_NEG(a),                           /**/ \
                                           b,                                    /**/ \
                                           0                                     /**/ \
                                           PTL_RECUR_RP(PTL_LOG2(b)))),               \
                   PPUTLIMPL_ARITHHINT(ta, tb)))
#define PPUTLMUL_BNEZ_01(ta, tb, a, b) \
  PPUTLMUL_BNEZ_01_o(PPUTLIMPL_ARITHHINT(ta, tb), a, PTL_NEG(b))
#define PPUTLMUL_BNEZ_01_o(hint, a, b)                                                \
  PTL_NEG(PTL_WORD(PPUTLMUL_RES(PPUTLMUL_X(PTL_RECUR_LP(PTL_LOG2(b), PPUTLMUL_R) /**/ \
                                           a,                                    /**/ \
                                           b,                                    /**/ \
                                           0                                     /**/ \
                                           PTL_RECUR_RP(PTL_LOG2(b)))),               \
                   hint))
#define PPUTLMUL_BNEZ_00(ta, tb, a, b)                                        \
  PTL_WORD(PPUTLMUL_RES(PPUTLMUL_X(PTL_RECUR_LP(PTL_LOG2(b), PPUTLMUL_R) /**/ \
                                   a,                                    /**/ \
                                   b,                                    /**/ \
                                   0                                     /**/ \
                                   PTL_RECUR_RP(PTL_LOG2(b)))),               \
           PPUTLIMPL_ARITHHINT(ta, tb))
#define PPUTLMUL_BEQZ(a, b) \
  PTL_WORD(0, PPUTLIMPL_XARITHHINT(PTL_TYPEOF(a), PTL_TYPEOF(b)))
#define PPUTLMUL_R(...) PPUTLMUL_R_o(__VA_ARGS__)
#define PPUTLMUL_R_o(a, b, s)   \
  PTL_BITSLL(a), PTL_BITSRA(b), \
      PTL_IF(PTL_BITGET(b, 0), PPUTLMUL_R_RECR, PPUTLMUL_R_BASE)(s, a)
#define PPUTLMUL_R_RECR(s, a)   PTL_ADD(s, a)
#define PPUTLMUL_R_BASE(s, a)   s
#define PPUTLMUL_RES(...)       PPUTLMUL_RES_o(__VA_ARGS__)
#define PPUTLMUL_RES_o(a, b, s) PTL_ADD(a, s)
#define PPUTLMUL_X(...)         __VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [math.divr]
/// -----------
/// truncated division with remainder.
/// fails on division by zero.
///
/// returns unsigned if either operand is unsigned,
/// decimal if either operand is decimal (and the
/// result is non-negative), and hex otherwise.
///
/// quotient and remainder may be formatted differently depending on sign.
///
/// PTL_DIVR(10, 5)                   // 2, 0
/// PTL_DIVR(11, 5)                   // 2, 1
/// PTL_DIVR(12, 5)                   // 2, 2
/// PTL_DIVR(13, 5)                   // 2, 3
/// PTL_DIVR(14, 5)                   // 2, 4
/// PTL_DIVR(PTL_NEG(10), 5)          // PTL_NEG(2), 0
/// PTL_DIVR(PTL_NEG(11), 5)          // PTL_NEG(2), PTL_NEG(1)
/// PTL_DIVR(PTL_NEG(12), 5)          // PTL_NEG(2), PTL_NEG(2)
/// PTL_DIVR(PTL_NEG(13), 5)          // PTL_NEG(2), PTL_NEG(3)
/// PTL_DIVR(PTL_NEG(14), 5)          // PTL_NEG(2), PTL_NEG(4)
/// PTL_DIVR(10, PTL_NEG(5))          // PTL_NEG(2), 0
/// PTL_DIVR(11, PTL_NEG(5))          // PTL_NEG(2), 1
/// PTL_DIVR(12, PTL_NEG(5))          // PTL_NEG(2), 2
/// PTL_DIVR(13, PTL_NEG(5))          // PTL_NEG(2), 3
/// PTL_DIVR(14, PTL_NEG(5))          // PTL_NEG(2), 4
/// PTL_DIVR(PTL_NEG(10), PTL_NEG(5)) // 0x002, 0x000
/// PTL_DIVR(PTL_NEG(11), PTL_NEG(5)) // 0x002, PTL_NEG(1)
/// PTL_DIVR(PTL_NEG(12), PTL_NEG(5)) // 0x002, PTL_NEG(2)
/// PTL_DIVR(PTL_NEG(13), PTL_NEG(5)) // 0x002, PTL_NEG(3)
/// PTL_DIVR(PTL_NEG(14), PTL_NEG(5)) // 0x002, PTL_NEG(4)
#define PTL_DIVR(/* word, word */...) /* -> word, word */                \
  PTL_IF(PTL_EQZ(PTL_REST(__VA_ARGS__)), PPUTLDIVR_BEQZ, PPUTLDIVR_BNEZ) \
  (PTL_STR("[PTL_DIVR] division by zero" : __VA_ARGS__), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLDIVR_BNEZ(e, a, b)                               \
  PTL_XCAT(PPUTLDIVR_BNEZ_, PTL_XCAT(PTL_LTZ(a), PTL_LTZ(b))) \
  (PTL_TYPEOF(a), PTL_TYPEOF(b), a, b)
#define PPUTLDIVR_BNEZ_11(ta, tb, a, b) \
  PPUTLDIVR_BNEZ_11_o(PPUTLIMPL_ARITHHINT(ta, tb), PTL_NEG(a), PTL_NEG(b))
#define PPUTLDIVR_BNEZ_11_o(hint, a, b) PPUTLDIVR_BNEZ_11_oo(hint, a, b, PTL_LOG2(a))
#define PPUTLDIVR_BNEZ_11_oo(hint, a, b, i) \
  PPUTLDIVR_BNEZ_11_ooo(hint, a, b, i, PTL_INC(i))
#define PPUTLDIVR_BNEZ_11_ooo(hint, a, b, i, iter)                       \
  PPUTLDIVR_BNEZ_11_res(PPUTLDIVR_X(PTL_RECUR_LP(iter, PPUTLDIVR_R) /**/ \
                                    i,                              /**/ \
                                    0,                              /**/ \
                                    0,                              /**/ \
                                    a,                              /**/ \
                                    b                               /**/ \
                                        PTL_RECUR_RP(iter)),             \
                        hint)
#define PPUTLDIVR_BNEZ_11_res(...) PPUTLDIVR_BNEZ_11_res_o(__VA_ARGS__)
#define PPUTLDIVR_BNEZ_11_res_o(i, q, r, a, b, hint) \
  PTL_WORD(q, hint), PTL_WORD(PTL_NEG(r), hint)
#define PPUTLDIVR_BNEZ_10(ta, tb, a, b) \
  PPUTLDIVR_BNEZ_10_o(PPUTLIMPL_ARITHHINT(ta, tb), PTL_NEG(a), b)
#define PPUTLDIVR_BNEZ_10_o(hint, a, b) PPUTLDIVR_BNEZ_10_oo(hint, a, b, PTL_LOG2(a))
#define PPUTLDIVR_BNEZ_10_oo(hint, a, b, i) \
  PPUTLDIVR_BNEZ_10_ooo(hint, a, b, i, PTL_INC(i))
#define PPUTLDIVR_BNEZ_10_ooo(hint, a, b, i, iter)                       \
  PPUTLDIVR_BNEZ_10_res(PPUTLDIVR_X(PTL_RECUR_LP(iter, PPUTLDIVR_R) /**/ \
                                    i,                              /**/ \
                                    0,                              /**/ \
                                    0,                              /**/ \
                                    a,                              /**/ \
                                    b                               /**/ \
                                        PTL_RECUR_RP(iter)),             \
                        hint)
#define PPUTLDIVR_BNEZ_10_res(...) PPUTLDIVR_BNEZ_10_res_o(__VA_ARGS__)
#define PPUTLDIVR_BNEZ_10_res_o(i, q, r, a, b, hint) \
  PTL_WORD(PTL_NEG(q), hint), PTL_WORD(PTL_NEG(r), hint)
#define PPUTLDIVR_BNEZ_01(ta, tb, a, b) \
  PPUTLDIVR_BNEZ_01_o(PPUTLIMPL_ARITHHINT(ta, tb), a, PTL_NEG(b), PTL_LOG2(a))
#define PPUTLDIVR_BNEZ_01_o(hint, a, b, i) PPUTLDIVR_BNEZ_01_oo(hint, a, b, i, PTL_INC(i))
#define PPUTLDIVR_BNEZ_01_oo(hint, a, b, i, iter)                        \
  PPUTLDIVR_BNEZ_01_res(PPUTLDIVR_X(PTL_RECUR_LP(iter, PPUTLDIVR_R) /**/ \
                                    i,                              /**/ \
                                    0,                              /**/ \
                                    0,                              /**/ \
                                    a,                              /**/ \
                                    b                               /**/ \
                                        PTL_RECUR_RP(iter)),             \
                        hint)
#define PPUTLDIVR_BNEZ_01_res(...) PPUTLDIVR_BNEZ_01_res_o(__VA_ARGS__)
#define PPUTLDIVR_BNEZ_01_res_o(i, q, r, a, b, hint) \
  PTL_WORD(PTL_NEG(q), hint), PTL_WORD(r, hint)
#define PPUTLDIVR_BNEZ_00(ta, tb, a, b) \
  PPUTLDIVR_BNEZ_00_o(PPUTLIMPL_ARITHHINT(ta, tb), a, b, PTL_LOG2(a))
#define PPUTLDIVR_BNEZ_00_o(hint, a, b, i) PPUTLDIVR_BNEZ_00_oo(hint, a, b, i, PTL_INC(i))
#define PPUTLDIVR_BNEZ_00_oo(hint, a, b, i, iter)                        \
  PPUTLDIVR_BNEZ_00_res(PPUTLDIVR_X(PTL_RECUR_LP(iter, PPUTLDIVR_R) /**/ \
                                    i,                              /**/ \
                                    0,                              /**/ \
                                    0,                              /**/ \
                                    a,                              /**/ \
                                    b                               /**/ \
                                        PTL_RECUR_RP(iter)),             \
                        hint)
#define PPUTLDIVR_BNEZ_00_res(...)                   PPUTLDIVR_BNEZ_00_res_o(__VA_ARGS__)
#define PPUTLDIVR_BNEZ_00_res_o(i, q, r, a, b, hint) PTL_WORD(q, hint), PTL_WORD(r, hint)
#define PPUTLDIVR_BEQZ(e, a, b)                      PTL_FAIL(e)
#define PPUTLDIVR_R(...)                             PPUTLDIVR_R_o(__VA_ARGS__)
#define PPUTLDIVR_R_o(i, q, r, a, b) \
  PPUTLDIVR_R_oo(i, q, PTL_BITSET(PTL_BITSLL(r), 0, PTL_BITGET(a, i)), a, b)
#define PPUTLDIVR_R_oo(i, q, r, a, b) \
  PTL_DEC(i), PTL_IF(PTL_GE(r, b), PPUTLDIVR_R_REM, PPUTLDIVR_R_NOREM)(i, q, r, b), a, b
#define PPUTLDIVR_R_REM(i, q, r, b)   PTL_BITSET(q, i, 1), PTL_SUB(r, b)
#define PPUTLDIVR_R_NOREM(i, q, r, b) q, r
#define PPUTLDIVR_X(...)              __VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [math.div]
/// ----------
/// truncated division.
/// fails on division by zero.
///
/// returns unsigned if either operand is unsigned,
/// decimal if either operand is decimal (and the
/// result is non-negative), and hex otherwise.
///
/// PTL_DIV(10, 5)                   // 2
/// PTL_DIV(11, 5)                   // 2
/// PTL_DIV(12, 5)                   // 2
/// PTL_DIV(13, 5)                   // 2
/// PTL_DIV(14, 5)                   // 2
/// PTL_DIV(PTL_NEG(10), 5)          // PTL_NEG(2)
/// PTL_DIV(PTL_NEG(11), 5)          // PTL_NEG(2)
/// PTL_DIV(PTL_NEG(12), 5)          // PTL_NEG(2)
/// PTL_DIV(PTL_NEG(13), 5)          // PTL_NEG(2)
/// PTL_DIV(PTL_NEG(14), 5)          // PTL_NEG(2)
/// PTL_DIV(10, PTL_NEG(5))          // PTL_NEG(2)
/// PTL_DIV(11, PTL_NEG(5))          // PTL_NEG(2)
/// PTL_DIV(12, PTL_NEG(5))          // PTL_NEG(2)
/// PTL_DIV(13, PTL_NEG(5))          // PTL_NEG(2)
/// PTL_DIV(14, PTL_NEG(5))          // PTL_NEG(2)
/// PTL_DIV(PTL_NEG(10), PTL_NEG(5)) // 0x002
/// PTL_DIV(PTL_NEG(11), PTL_NEG(5)) // 0x002
/// PTL_DIV(PTL_NEG(12), PTL_NEG(5)) // 0x002
/// PTL_DIV(PTL_NEG(13), PTL_NEG(5)) // 0x002
/// PTL_DIV(PTL_NEG(14), PTL_NEG(5)) // 0x002
#define PTL_DIV(/* word, word */...) /* -> word */ PTL_XFIRST(PTL_DIVR(__VA_ARGS__))

/// [math.rem]
/// ----------
/// truncated division remainder.
/// fails on division by zero.
///
/// returns unsigned if either operand is unsigned,
/// decimal if either operand is decimal (and the
/// result is non-negative), and hex otherwise.
///
/// PTL_REM(10, 5)                   // 0
/// PTL_REM(11, 5)                   // 1
/// PTL_REM(12, 5)                   // 2
/// PTL_REM(13, 5)                   // 3
/// PTL_REM(14, 5)                   // 4
/// PTL_REM(PTL_NEG(10), 5)          // 0
/// PTL_REM(PTL_NEG(11), 5)          // PTL_NEG(1)
/// PTL_REM(PTL_NEG(12), 5)          // PTL_NEG(2)
/// PTL_REM(PTL_NEG(13), 5)          // PTL_NEG(3)
/// PTL_REM(PTL_NEG(14), 5)          // PTL_NEG(4)
/// PTL_REM(10, PTL_NEG(5))          // 0
/// PTL_REM(11, PTL_NEG(5))          // 1
/// PTL_REM(12, PTL_NEG(5))          // 2
/// PTL_REM(13, PTL_NEG(5))          // 3
/// PTL_REM(14, PTL_NEG(5))          // 4
/// PTL_REM(PTL_NEG(10), PTL_NEG(5)) // 0x000
/// PTL_REM(PTL_NEG(11), PTL_NEG(5)) // PTL_NEG(1)
/// PTL_REM(PTL_NEG(12), PTL_NEG(5)) // PTL_NEG(2)
/// PTL_REM(PTL_NEG(13), PTL_NEG(5)) // PTL_NEG(3)
/// PTL_REM(PTL_NEG(14), PTL_NEG(5)) // PTL_NEG(4)
#define PTL_REM(/* word, word */...) /* -> word */ PTL_XREST(PTL_DIVR(__VA_ARGS__))

/// [math.index]
/// ------------
/// translates an ofs to a non-negative zero-offset for a given range size.
/// positive indices return unchanged, negative indices return added to the size.
///
/// fails if input is out of bounds: [-size, size];
/// allows one-past-the-end indexing.
///
/// casts to typeof input size.
///
/// PTL_INDEX(5, 0)           // 0
/// PTL_INDEX(5u, 1)          // 1u
/// PTL_INDEX(5, 2)           // 2
/// PTL_INDEX(5, 3)           // 3
/// PTL_INDEX(5, 4)           // 4
/// PTL_INDEX(5, 5)           // 5
/// PTL_INDEX(5, PTL_NEG(1))  // 4
/// PTL_INDEX(5, PTL_NEG(2))  // 3
/// PTL_INDEX(5, PTL_NEG(3))  // 2
/// PTL_INDEX(5u, PTL_NEG(4)) // 1u
/// PTL_INDEX(5, PTL_NEG(5))  // 0
#define PTL_INDEX(/* size, ofs */...) /* -> size */ \
  PPUTLINDEX_o(PTL_STR("index out of bounds" : PTL_INDEX(__VA_ARGS__)), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLINDEX_o(e, sz, i) \
  PTL_OFS(PPUTLIMPL_INDEX(PTL_UHEX(i), PTL_IS_INT(i), PTL_UHEX(sz), e), PTL_TYPEOF(sz))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [range.sizeof]
/// --------------
/// detects number of items. compatible with tup and all other pputl datastructures.
/// fails if size is larger than PTL_SIZE_MAX (255u)
///
/// note: complexity is O(size) for tup and O(1) for other pputl datastructures.
///
/// PTL_SIZEOF(())     // 0u
/// PTL_SIZEOF((a))    // 1u
/// PTL_SIZEOF((a, b)) // 2u
/// PTL_SIZEOF((, ))   // 2u
#define PTL_SIZEOF(/* mem */...) /* -> udec&size */ \
  PTL_ESC(PTL_COUNTOF PTL_TUP(__VA_ARGS__))

/// [range.itemsof]
/// ---------------
/// extracts item from a tuple or array.
///
/// PTL_ITEMSOF(())        // <nothing>
/// PTL_ITEMSOF((a))       // a
/// PTL_ITEMSOF((a, b))    // a, b
/// PTL_ITEMSOF((a, b, c)) // a, b, c
#define PTL_ITEMSOF(/* mem */...) /* -> list */ PPUTLITEMSOF_X(PTL_TUP(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLITEMSOF_X(...) PTL_ESC __VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [range.is_empty]
/// ----------------
/// true if the tuple has no elements.
///
/// PTL_IS_EMPTY(())  // 1
/// PTL_IS_EMPTY((a)) // 0
/// PTL_IS_EMPTY((,)) // 0
#define PTL_IS_EMPTY(/* mem */...) /* -> bool */ PTL_IS_NONE(PTL_ITEMSOF(__VA_ARGS__))

/// [range.bisect]
/// --------------
/// splits a tuple in two at a given index.
///
/// abs(index) must be less than or equal to the tuple size.
///
/// returns:
///   (1) head: a tuple of the items strictly before the index
///   (2) tail: a tuple of the items from the index to the end
///   (3) type: 1 if two empty elements were split (information loss), else 0
///
/// PTL_BISECT((a), 0)                // (), (a), 0
/// PTL_BISECT((a), 1)                // (a), (), 0
/// PTL_BISECT((), 0)                 // (), (), 0
/// PTL_BISECT((a, b, c), 1)          // (a), (b, c), 0
/// PTL_BISECT((a, b, c), 2)          // (a, b), (c), 0
/// PTL_BISECT((a, b, c), 3)          // (a, b, c), (), 0
/// PTL_BISECT((a, b, c), PTL_NEG(1)) // (a, b), (c), 0
/// PTL_BISECT((a, b, c), PTL_NEG(2)) // (a), (b, c), 0
/// PTL_BISECT((a, b, c), PTL_NEG(3)) // (), (a, b, c), 0
/// PTL_BISECT((, ), 0)               // (), (,), 0
/// PTL_BISECT((, ), 1)               // (), (), 1
/// PTL_BISECT((, ), 2)               // (,), (), 0
/// PTL_BISECT((, , ), 2)             // (,), (), 1
#define PTL_BISECT(/* mem, ofs */...) /* -> mem, mem, bool */ PPUTLBISECT_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBISECT_o(t, i) PPUTLBISECT_oo(i, PTL_ITEMSOF(t))
#define PPUTLBISECT_oo(i, ...) \
  PPUTLBISECT_ooo(PTL_INDEX(PTL_COUNTOF(__VA_ARGS__), i), __VA_ARGS__)
#define PPUTLBISECT_ooo(n, ...) \
  PTL_IF(PTL_EQZ(n), PPUTLBISECT_EQZ, PPUTLBISECT_NEZ)(n, __VA_ARGS__)
#define PPUTLBISECT_NEZ(n, ...) PPUTLBISECT_NEZ_o(PTL_DEC(n), __VA_ARGS__)
#define PPUTLBISECT_NEZ_o(n, ...)                                   \
  PPUTLBISECT_RES(PPUTLBISECT_X(PTL_RECUR_LP(n, PPUTLBISECT_R) /**/ \
                                (),                            /**/ \
                                __VA_ARGS__                    /**/ \
                                    PTL_RECUR_RP(n)))
#define PPUTLBISECT_EQZ(n, ...) (), (__VA_ARGS__), 0
#define PPUTLBISECT_RES(...)    PPUTLBISECT_RES_o(__VA_ARGS__)
#define PPUTLBISECT_RES_o(head, ...)                                          \
  (PTL_XREST(PTL_ESC head, PTL_FIRST(__VA_ARGS__))), (PTL_REST(__VA_ARGS__)), \
      PPUTLBISECT_TYPE(__VA_ARGS__, .)
#define PPUTLBISECT_R(...)            PPUTLBISECT_R_o(__VA_ARGS__)
#define PPUTLBISECT_R_o(head, _, ...) (PTL_ESC head, _), __VA_ARGS__
#define PPUTLBISECT_TYPE(_, __, ...) \
  PTL_OR(PTL_AND(PTL_IS_NONE(_), PTL_NOT(PTL_IS_NONE(__VA_ARGS__))), PTL_IS_NONE(__))
#define PPUTLBISECT_X(...) __VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [range.unite]
/// -------------
/// unites two ranges given an optional bisection type.
/// type is used to preserve information when two empty elements are split.
///
/// PTL_UNITE((a), ())             // (a)
/// PTL_UNITE((), (b))             // (b)
/// PTL_UNITE((a), (b))            // (a, b)
/// PTL_UNITE((a, b), (c, d))      // (a, b, c, d)
/// PTL_BISECT((), 0)              // (), (), 0
/// PTL_BISECT((, ), 1)            // (), (), 1
/// PTL_UNITE((), ())              // ()
/// PTL_UNITE((), (), 0)           // ()
/// PTL_UNITE((), (), 1)           // (, )
/// PTL_UNITE(PTL_BISECT((), 0))   // ()
/// PTL_UNITE(PTL_BISECT((, ), 1)) // (, )
#define PTL_UNITE(/* mem, mem, bool=0 */...) /* -> mem */ \
  PPUTLUNITE_o(__VA_ARGS__)(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLUNITE_o(head, tail, ...)                                    \
  PTL_XCAT(PTL_XCAT(PPUTLUNITE_, PTL_BOOL(PTL_DEFAULT(0, __VA_ARGS__))), \
           PTL_XCAT(PTL_IS_NONE(PTL_ITEMSOF(head)), PTL_IS_NONE(PTL_ITEMSOF(tail))))
#define PPUTLUNITE_111(head, tail, ...) (, )
#define PPUTLUNITE_110(head, tail, ...) (, PTL_ITEMSOF(tail))
#define PPUTLUNITE_101(head, tail, ...) (PTL_ITEMSOF(head), )
#define PPUTLUNITE_100(head, tail, ...) (PTL_ITEMSOF(head), PTL_ITEMSOF(tail))
#define PPUTLUNITE_011(head, tail, ...) ()
#define PPUTLUNITE_010(head, tail, ...) tail
#define PPUTLUNITE_001(head, tail, ...) head
#define PPUTLUNITE_000(head, tail, ...) (PTL_ITEMSOF(head), PTL_ITEMSOF(tail))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [range.head]
/// ------------
/// returns a tuple of the first n elements of a tuple.
/// n must be less than or equal to the tuple size.
///
/// PTL_HEAD((a), 0)       // ()
/// PTL_HEAD((a), 1)       // (a)
/// PTL_HEAD((a))          // (a)
/// PTL_HEAD((), 0)        // ()
/// PTL_HEAD((a, b, c), 1) // (a)
/// PTL_HEAD((a, b, c), 2) // (a, b)
/// PTL_HEAD((a, b, c), 3) // (a, b, c)
/// PTL_HEAD((, ), 0)      // ()
/// PTL_HEAD((, ), 1)      // ()
/// PTL_HEAD((, ), 2)      // (,)
/// PTL_HEAD((, , ), 2)    // (,)
#define PTL_HEAD(/* mem, size=1 */...) /* -> mem */ PPUTLHEAD_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLHEAD_o(t, ...) \
  PTL_XFIRST(PTL_BISECT(t, PTL_SIZE(PTL_DEFAULT(1, __VA_ARGS__))))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [range.tail]
/// ------------
/// returns a tuple of the last n elements of a tuple.
/// n must be less than or equal to the tuple size.
///
/// PTL_TAIL((a), 0)       // ()
/// PTL_TAIL((a), 1)       // (a)
/// PTL_TAIL((a))          // (a)
/// PTL_TAIL((), 0)        // ()
/// PTL_TAIL((a, b, c), 1) // (c)
/// PTL_TAIL((a, b, c), 2) // (b, c)
/// PTL_TAIL((a, b, c), 3) // (a, b, c)
/// PTL_TAIL((, ), 0)      // ()
/// PTL_TAIL((, ), 1)      // ()
/// PTL_TAIL((, ), 2)      // (,)
/// PTL_TAIL((, , ), 2)    // (,)
#define PTL_TAIL(/* mem, size=1 */...) /* -> mem */ PPUTLTAIL_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLTAIL_o(t, ...) \
  PPUTLTAIL_oo(PTL_BISECT(  \
      t, PTL_SIZE(PTL_SUB(PTL_COUNTOF(PTL_ITEMSOF(t)), PTL_DEFAULT(1, __VA_ARGS__)))))
#define PPUTLTAIL_oo(...)               PPUTLTAIL_ooo(__VA_ARGS__)
#define PPUTLTAIL_ooo(head, tail, type) tail

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [range.push_front]
/// ------------------
/// pushes items to the front of a tuple.
///
/// PTL_PUSH_FRONT(())           // ()
/// PTL_PUSH_FRONT((), a)        // (a)
/// PTL_PUSH_FRONT((a), b)       // (b, a)
/// PTL_PUSH_FRONT((b, a), c, d) // (c, d, b, a)
/// PTL_PUSH_FRONT((c, d, b, a)) // (c, d, b, a)
#define PTL_PUSH_FRONT(/* mem, any */...) /* -> mem */ PPUTLPUSH_FRONT_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLPUSH_FRONT_o(t, ...)                                                   \
  PTL_XCAT(PPUTLPUSH_FRONT_o_, PTL_XCAT(PTL_IS_EMPTY(t), PTL_IS_NONE(__VA_ARGS__))) \
  (t, __VA_ARGS__)
#define PPUTLPUSH_FRONT_o_11(t, ...) ()
#define PPUTLPUSH_FRONT_o_10(t, ...) (__VA_ARGS__)
#define PPUTLPUSH_FRONT_o_01(t, ...) t
#define PPUTLPUSH_FRONT_o_00(t, ...) (__VA_ARGS__, PTL_ESC t)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [range.push_back]
/// -----------------
/// pushes items to the back of a tuple.
///
/// PTL_PUSH_BACK(())           // ()
/// PTL_PUSH_BACK((), a)        // (a)
/// PTL_PUSH_BACK((a), b)       // (a, b)
/// PTL_PUSH_BACK((a, b), c, d) // (a, b, c, d)
/// PTL_PUSH_BACK((a, b, c, d)) // (a, b, c, d)
#define PTL_PUSH_BACK(/* mem, any */...) /* -> mem */ PPUTLPUSH_BACK_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLPUSH_BACK_o(t, ...)                                                   \
  PTL_XCAT(PPUTLPUSH_BACK_o_, PTL_XCAT(PTL_IS_EMPTY(t), PTL_IS_NONE(__VA_ARGS__))) \
  (t, __VA_ARGS__)
#define PPUTLPUSH_BACK_o_11(t, ...) ()
#define PPUTLPUSH_BACK_o_10(t, ...) (__VA_ARGS__)
#define PPUTLPUSH_BACK_o_01(t, ...) t
#define PPUTLPUSH_BACK_o_00(t, ...) (PTL_ESC t, __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [range.pop_front]
/// -----------------
/// removes the first n items from a tuple.
/// n must be less than or equal to the tuple size.
///
/// PTL_POP_FRONT((), 0)        // ()
/// PTL_POP_FRONT((a), 0)       // (a)
/// PTL_POP_FRONT((a), 1)       // ()
/// PTL_POP_FRONT((a, b))       // (b)
/// PTL_POP_FRONT((a, b), 1)    // (b)
/// PTL_POP_FRONT((a, b), 2)    // ()
/// PTL_POP_FRONT((a, b, c), 2) // (c)
#define PTL_POP_FRONT(/* mem, size=1 */...) /* -> tup */ PPUTLPOP_FRONT_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLPOP_FRONT_o(t, ...) \
  PPUTLPOP_FRONT_oo(PTL_BISECT(t, PTL_DEFAULT(1, __VA_ARGS__)))
#define PPUTLPOP_FRONT_oo(...)               PPUTLPOP_FRONT_ooo(__VA_ARGS__)
#define PPUTLPOP_FRONT_ooo(head, tail, type) tail

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [range.pop_back]
/// ----------------
/// removes the last n items from a tuple.
/// n must be less than or equal to the tuple size.
///
/// PTL_POP_BACK((), 0)        // ()
/// PTL_POP_BACK((a), 0)       // (a)
/// PTL_POP_BACK((a), 1)       // ()
/// PTL_POP_BACK((a, b))       // (a)
/// PTL_POP_BACK((a, b), 1)    // (a)
/// PTL_POP_BACK((a, b), 2)    // ()
/// PTL_POP_BACK((a, b, c), 2) // (a)
#define PTL_POP_BACK(/* mem, size=1 */...) /* -> tup */ PPUTLPOP_BACK_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLPOP_BACK_o(t, ...) \
  PTL_XFIRST(PTL_BISECT(        \
      t, PTL_SIZE(PTL_SUB(PTL_COUNTOF(PTL_ITEMSOF(t)), PTL_DEFAULT(1, __VA_ARGS__)))))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [range.get_item]
/// ----------------
/// get a tuple item. index must be within [-size, size).
///
/// PTL_GET_ITEM((a), 0)                // a
/// PTL_GET_ITEM((a, b), 0)             // a
/// PTL_GET_ITEM((a, b), 1)             // b
/// PTL_GET_ITEM((a, b, c), PTL_NEG(2)) // b
/// PTL_GET_ITEM((a, b, c), PTL_NEG(3)) // a
#define PTL_GET_ITEM(/* mem, k: ofs|any */...) /* -> any */ \
  PPUTLGET_ITEM_o(PTL_STR("invalid index"                   \
                          : PTL_GET_ITEM(__VA_ARGS__)),     \
                  PTL_BISECT(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLGET_ITEM_o(...)                  PPUTLGET_ITEM_oo(__VA_ARGS__)
#define PPUTLGET_ITEM_oo(e, head, tail, type) PPUTLGET_ITEM_CHK(e, type, PTL_ESC tail)
#define PPUTLGET_ITEM_CHK(e, type, ...)                                        \
  PTL_IF(PTL_AND(PTL_IS_NONE(__VA_ARGS__), PTL_NOT(type)), PPUTLGET_ITEM_FAIL, \
         PPUTLGET_ITEM_PASS)                                                   \
  (e, __VA_ARGS__)
#define PPUTLGET_ITEM_FAIL(e, ...)    PTL_FAIL(e)
#define PPUTLGET_ITEM_PASS(e, _, ...) _

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [range.set_item]
/// ----------------
/// set a tuple item to a new value. index must be within [-size, size).
#define PTL_SET_ITEM(/* mem, k: ofs|any, v: any */...) /* -> mem */ __VA_ARGS__

// vim: fdm=marker:fmr={{{,}}}

#endif

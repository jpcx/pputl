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
//    Supported integer modes:                                                //
//                                                                            //
//       word_size=1    :   4-bit integers  :  ~? KiB                         //
//       word_size=2    :   8-bit integers  :  ~? KiB                         //
//     [ word_size=3    :  12-bit integers  :  ~? MiB  (default) ]            //
//       word_size=4 †  :  16-bit integers  :  ~? MiB                         //
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
//     any: any potentially-empty argument in a __VA_ARGS__ expression        //
//      ├╴none: the literal nothing; an absence of pp-tokens                  //
//      ├╴atom: a non-empty sequence of pp-concatable tokens                  //
//      │  ├╴idec: atoms 0|1|...|2046|2047                                    //
//      │  │  └╴bool: idecs 0|1                                               //
//      │  ├╴ihex: atoms 0x000|0x001|...|0xFFE|0xFFF                          //
//      │  ├╴udec: atoms 0u|1u|...|4094u|4095u                                //
//      │  ├╴uhex: atoms 0x000u|0x001u|...|0xFFEu|0xFFFu                      //
//      │  ├╴int:  idec|ihex; a signed two's complement integer               //
//      │  ├╴uint: udec|uhex; an unsigned integer                             //
//      │  └╴word:  int|uint; any integer                                     //
//      │     ├╴size: any non-negative word up to size_max                    //
//      │     └╴ofs:  any word whose absolute value is a valid size           //
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
//    Negative ints  cannot be represented in decimal due to concatenation    //
//    restrictions. Arithmetic and bitwise functions attempt to cast their    //
//    results in the same form as their input, but will always return ihex    //
//    when an idec input becomes negative.  Decimal representations may be   ///
//    generated for pasting using fmt.c_int.                                ////
//                                                                         /////
///////////////////////////////////////////////////////////////////////////// */

/// [lang.esc]
/// ----------
/// escapes parentheses. identity function.
///
/// PTL_ESC ()        // <nothing>
/// PTL_ESC (a, b, c) // a, b, c
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
/// e.g. PTL_REST(__VA_ARGS__)
///      PTL_ESC(PTL_REST tup)
#define PTL_REST(/* first: any, ...rest: any */ _, ...) /* -> any... */ __VA_ARGS__

/// [lang.eat]
/// ----------
/// eats arguments; return nothing.
#define PTL_EAT(...) /* -> none */

/// [lang.cat]
/// ----------
/// immediately concatenates a with b.
/// must provide at least one arg.
/// args must be compatible with the ## operator.
///
/// PTL_CAT(foo, bar)          // foobar
/// PTL_CAT(foo, PTL_EAT(bar)) // fooPTL_EAT(bar)
#define PTL_CAT(/* a: any, b: any */ a, b) /* -> any */ a##b

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

/// [lang.xfirst]
/// -------------
/// returns the first argument.
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

// vim: fdm=marker:fmr={{{,}}}

#endif

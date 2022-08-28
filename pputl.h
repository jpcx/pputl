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
//    algorithms that generate template specializations.                      //
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
//       ‐ type casting                            [type; see TERMINOLOGY]    //
//           list  any   none  obj   atom  enum  bool  hex   nybl  int        //
//           idec  ihex  uint  udec  uhex  tup   utup  word  size             //
//       ‐ traits detection                                       [traits]    //
//           is_list  is_any   is_none  is_obj   is_atom  is_enum  is_bool    //
//           is_hex   is_nybl  is_int   is_idec  is_ihex  is_uint  is_udec    //
//           is_uhex  is_tup   is_utup  is_word  is_size  typeof   sizeof     //
//       ‐ boolean logic                                           [logic]    //
//           not  and  or  xor  nand  nor  xnor                               //
//       ‐ paste formatting                                    [lang, fmt]    //
//           str  xstr  cat  xcat  c_int  c_hex  c_bin                        //
//     ◆ signed and unsigned integers                                         //
//       ‐ arithmetic                                      [numeric, math]    //
//           inc  dec  add   sub   mul   divr                                 //
//           div  mod  pow2  sqrt  log2  fact                                 //
//       ‐ comparison                                   [numeric, compare]    //
//           eqz  nez  ltz  gtz  lez  gez  lt                                 //
//           gt   le   ge   eq   ne   min  max                                //
//       ‐ bitwise operations                                    [bitwise]    //
//           bdump  bsll  bsrl   bsra  bnot  band   bor    bxor               //
//           bnand  bnor  bxnor  bget  bset  bflip  brotl  brotr              //
//     ◆ range algorithms                                                     //
//       ‐ element access                                          [range]    //
//           items  bisect  unite  get  set  push  pop  slice                 //
//       ‐ generation                                               [algo]    //
//           seq  repeat  gen_lp  gen_rp                                      //
//       ‐ transformation                                           [algo]    //
//           rev  map_lp  map_rp  shift  rotate                               //
//       ‐ reduction                                                [algo]    //
//           reduce_lp  reduce_rp                                             //
//     ◆ metaprogramming utilities                                            //
//       ‐ expansion control and tracing                            [meta]    //
//           x  lp  rp  xtrace  xtrace_read                                   //
//       ‐ mutually recursive stack expansion                       [meta]    //
//           xx_lp  xx_rp                                                     //
//       ‐ inline recursive stack construction                      [meta]    //
//           recur_lp  recur_rp                                               //
//     ◆ configuration details                                    [config]    //
//           build    word_size  bit_length  int_min                          //
//           int_max  uint_max   size_max                                     //
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
//    traits identification and another for type casting and assertions.      //
//                                                                            //
//    API functions are strictly documented using this type system. Inputs    //
//    are verified directly or indirectly by invoking the appropriate type    //
//    constructors or by using some other form of inference.                  //
//                                                                            //
//     list: __VA_ARGS__; tokens delimited by non-parenthesized commas        //
//      └╴any: <abstract> a list with no separatory commas (0-1 elements)     //
//         ├╴none: nothing; an absence of pp-tokens                           //
//         └╴obj: a non-empty generic value                                   //
//            ├╴atom: an individual value that may form an identifier tail    //
//            │  ├╴enum<...>: a value that matches a specified atom union     //
//            │  │  └╴bool: enum<0|1>                                         //
//            │  ├╴hex:  a 4-bit uppercase hexadecimal digit [e.g. B]         //
//            │  ├╴nybl: a 4-bit bool concatenation [e.g. 0110]               //
//            │  ├╴int: <abstract> a word-sized signed integer                //
//            │  │  ├╴idec: a positive 2s-complement decimal [e.g. 3]         //
//            │  │  └╴ihex: a signed hex integer [e.g. 0x861]                 //
//            │  └╴uint: <abstract> a word-sized unsigned integer             //
//            │     ├╴udec: an unsigned decimal integer [e.g. 42u]            //
//            │     └╴uhex: an unsigned hex integer [e.g. 0x02Au]             //
//            ├╴tup: a parenthesized list [e.g ()] [e.g. (a, b)]              //
//            │  └╴utup: an unsigned word-sized hex tup [e.g. (6, D, 2)]      //
//            └╴word: <union> int | uint | utup                               //
//               └╴size: an unsigned word capped by the argument limit        //
//                                                                            //
//    All pputl traits are fully testable except for atom,  which requires    //
//    its values to match  /[\w\d_]+/  as they must be able to concatenate    //
//    with identifiers to create new identifiers. See C++20 [lex.name] for    //
//    details.  While undetectable, this requirement is crucial for value-    //
//    based control flow and must be observed by the user where applicable    //
//    (although atom does perform a tuple test and will fail when it can).    //
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
#define PTL_BUILD /* -> <c++ int> */ 20220828

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

/// [traits.is_any]
/// ---------------
/// detects if the list has either 0 or 1 elements.
///
/// PTL_IS_ANY()       // 1
/// PTL_IS_ANY(foo)    // 1
/// PTL_IS_ANY((a, b)) // 1
/// PTL_IS_ANY(a, b)   // 0
/// PTL_IS_ANY(, )     // 0
/// PTL_IS_ANY(, , )   // 0
#define PTL_IS_ANY(/* list */...) /* -> bool */ PPUTLIS_ANY_o(__VA_ARGS__.)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_ANY_o(/* <args + token; e.g. __VA_ARGS__.foo> */ _, ...) \
  PPUTLIS_ANY_0##__VA_OPT__(1)
#define PPUTLIS_ANY_01 0
#define PPUTLIS_ANY_0  1

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_none]
/// ----------------
/// [extends PTL_IS_ANY] detects if args is nothing.
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

/// [traits.is_obj]
/// ---------------
/// [extends PTL_IS_ANY] detects if args is a non-empty generic value.
///
/// PTL_IS_OBJ()         // 0
/// PTL_IS_OBJ(,)        // 0
/// PTL_IS_OBJ(foo,)     // 0
/// PTL_IS_OBJ(foo, bar) // 0
/// PTL_IS_OBJ(foo)      // 1
/// PTL_IS_OBJ((42))     // 1
#define PTL_IS_OBJ(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_OBJ_, PPUTLIS_ANY_o(__VA_ARGS__.))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_OBJ_1           PPUTLIS_OBJ_o
#define PPUTLIS_OBJ_0           PPUTLIS_OBJ_0_fail
#define PPUTLIS_OBJ_0_fail(...) 0
#define PPUTLIS_OBJ_o(any)      PTL_XCAT(PPUTLIS_OBJ_o_, PTL_IS_NONE(any))
#define PPUTLIS_OBJ_o_1         0
#define PPUTLIS_OBJ_o_0         1

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_atom]
/// ----------------
/// [extends PTL_IS_OBJ] detects if args is a value that may form an identifier tail.
///
/// this function only tests for tuples and multiple values.
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
  PTL_XCAT(PPUTLIS_ATOM_, PTL_IS_OBJ(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_ATOM_1           PPUTLIS_ATOM_o
#define PPUTLIS_ATOM_0           PPUTLIS_ATOM_0_fail
#define PPUTLIS_ATOM_0_fail(...) 0
#define PPUTLIS_ATOM_o(obj)      PTL_XCAT(PPUTLIS_ATOM_o_, PTL_IS_NONE(PTL_EAT obj))
#define PPUTLIS_ATOM_o_1         0
#define PPUTLIS_ATOM_o_0         1

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_enum]
/// ----------------
/// [extends PTL_IS_ATOM] detects if args matches a specified atom union.
///
/// to use this function, define a set of
/// macros with the following characteristics:
///  ‐ object-like
///  ‐ common prefix
///  ‐ enum value suffixes
///  ‐ expand to nothing
/// pass the common prefix as chkprefix.
///
/// example: (identifying an enum<GOOD|BAD>)
///  #define FOO_GOOD
///  #define FOO_BAD
///  PTL_IS_ENUM(FOO_, BLEH) // 0
///  PTL_IS_ENUM(FOO_, GOOD) // 1
///  PTL_IS_ENUM(FOO_, ,,,)  // 0
#define PTL_IS_ENUM(/* chkprefix: atom, list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_ENUM_, PTL_IS_ATOM(PTL_REST(__VA_ARGS__)))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_ENUM_1                 PPUTLIS_ENUM_o
#define PPUTLIS_ENUM_0                 PPUTLIS_ENUM_0_fail
#define PPUTLIS_ENUM_0_fail(...)       0
#define PPUTLIS_ENUM_o(chkatom, vatom) PTL_IS_NONE(PTL_XCAT(chkatom, vatom))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_bool]
/// ----------------
/// [extends PTL_IS_ENUM] detects if args is a bool (enum<0|1>).
///
/// PTL_IS_BOOL()      // 0
/// PTL_IS_BOOL(0)     // 1
/// PTL_IS_BOOL(1)     // 1
/// PTL_IS_BOOL(1u)    // 0
/// PTL_IS_BOOL(0x000) // 0
/// PTL_IS_BOOL(0, 1)  // 0
/// PTL_IS_BOOL((0))   // 0
#define PTL_IS_BOOL(/* list */...) /* -> bool */ PTL_IS_ENUM(PPUTLIS_BOOL_, __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_BOOL_1
#define PPUTLIS_BOOL_0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_hex]
/// ---------------
/// [extends PTL_IS_ATOM] detects if args is 4-bit uppercase hexadecimal digit.
///
/// PTL_IS_HEX()    // 0
/// PTL_IS_HEX(0)   // 1
/// PTL_IS_HEX(Q)   // 0
/// PTL_IS_HEX(foo) // 0
/// PTL_IS_HEX(B)   // 1
/// PTL_IS_HEX(b)   // 0
/// PTL_IS_HEX(F)   // 1
#define PTL_IS_HEX(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_HEX_, PTL_IS_ATOM(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_HEX_1           PPUTLIS_HEX_o
#define PPUTLIS_HEX_0           PPUTLIS_HEX_0_fail
#define PPUTLIS_HEX_0_fail(...) 0
#define PPUTLIS_HEX_o(atom)     PPUTLIMPL_HEX(atom, IS)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_nybl]
/// ----------------
/// [extends PTL_IS_ATOM] detects if args is a 4-bit bool concatenation.
///
/// PTL_IS_NYBL()     // 0
/// PTL_IS_NYBL(0)    // 0
/// PTL_IS_NYBL(B)    // 0
/// PTL_IS_NYBL(000)  // 0
/// PTL_IS_NYBL(0000) // 1
/// PTL_IS_NYBL(0110) // 1
#define PTL_IS_NYBL(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_NYBL_, PTL_IS_ATOM(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_NYBL_1           PPUTLIS_NYBL_o
#define PPUTLIS_NYBL_0           PPUTLIS_NYBL_0_fail
#define PPUTLIS_NYBL_0_fail(...) 0
#define PPUTLIS_NYBL_o(atom)     PPUTLIMPL_NYBL(atom, IS)

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
/// PTL_IS_INT(0xF)    // 0
/// PTL_IS_INT((), ()) // 0
#define PTL_IS_INT(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_INT_, PTL_IS_ATOM(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_INT_1           PPUTLIS_INT_o
#define PPUTLIS_INT_0           PPUTLIS_INT_0_fail
#define PPUTLIS_INT_0_fail(...) 0
#define PPUTLIS_INT_o(atom)                                                    \
  PTL_XCAT(PPUTLIS_INT_o_,                                                     \
           PTL_XCAT(PPUTLIMPL_UDEC(atom##u, IS), PPUTLIMPL_UHEX(atom##u, IS))) \
  (atom##u)
#define PPUTLIS_INT_o_10(udec) \
  PTL_XCAT(PPUTLIS_INT_o_10, PPUTLIMPL_UHEX(PPUTLIMPL_UDEC(udec, UHEX), ILTZ))
#define PPUTLIS_INT_o_101      0
#define PPUTLIS_INT_o_100      1
#define PPUTLIS_INT_o_01(uhex) 1
#define PPUTLIS_INT_o_00(...)  0

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
#define PTL_IS_IDEC(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_IDEC_, PTL_IS_INT(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_IDEC_1         PPUTLIS_IDEC_o
#define PPUTLIS_IDEC_0         PPUTLIS_IDEC_FAIL
#define PPUTLIS_IDEC_FAIL(...) 0
#define PPUTLIS_IDEC_o(int)    PTL_XCAT(PPUTLIS_IDEC_o_, PPUTLIMPL_UDEC(int##u, IS))
#define PPUTLIS_IDEC_o_1       1
#define PPUTLIS_IDEC_o_0       0

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
#define PTL_IS_IHEX(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_IHEX_, PTL_IS_INT(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_IHEX_1           PPUTLIS_IHEX_o
#define PPUTLIS_IHEX_0           PPUTLIS_IHEX_0_fail
#define PPUTLIS_IHEX_0_fail(...) 0
#define PPUTLIS_IHEX_o(int)      PTL_XCAT(PPUTLIS_IHEX_o_, PPUTLIMPL_UHEX(int##u, IS))
#define PPUTLIS_IHEX_o_1         1
#define PPUTLIS_IHEX_o_0         0

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
#define PTL_IS_UINT(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_UINT_, PTL_IS_ATOM(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_UINT_1           PPUTLIS_UINT_o
#define PPUTLIS_UINT_0           PPUTLIS_UINT_0_fail
#define PPUTLIS_UINT_0_fail(...) 0
#define PPUTLIS_UINT_o(atom) \
  PTL_XCAT(PPUTLIS_UINT_o_, PTL_XCAT(PPUTLIMPL_UDEC(atom, IS), PPUTLIMPL_UHEX(atom, IS)))
#define PPUTLIS_UINT_o_10 1
#define PPUTLIS_UINT_o_01 1
#define PPUTLIS_UINT_o_00 0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_udec]
/// ----------------
/// [extends PTL_IS_UINT] detects if args is an unsigned int in deicmal form (requires 'u'
/// suffix).
///
/// PTL_IS_UDEC(1)      // 0
/// PTL_IS_UDEC(1u)     // 1
/// PTL_IS_UDEC(4095)   // 0
/// PTL_IS_UDEC(4095u)  // 1
/// PTL_IS_UDEC(0x000u) // 0
/// PTL_IS_UDEC(0xFFF)  // 0
/// PTL_IS_UDEC((), ()) // 0
#define PTL_IS_UDEC(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_UDEC_, PTL_IS_UINT(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_UDEC_1           PPUTLIS_UDEC_o
#define PPUTLIS_UDEC_0           PPUTLIS_UDEC_0_fail
#define PPUTLIS_UDEC_0_fail(...) 0
#define PPUTLIS_UDEC_o(uint)     PTL_XCAT(PPUTLIS_UDEC_o_, PPUTLIMPL_UDEC(uint, IS))
#define PPUTLIS_UDEC_o_1         1
#define PPUTLIS_UDEC_o_0         0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_uhex]
/// ----------------
/// [extends PTL_IS_UINT] detects if args is an unsigned int in hex form (requires 'u'
/// suffix). hex length is fixed at PTL_WORD_SIZE (3).
///
/// PTL_IS_UHEX(1)      // 0
/// PTL_IS_UHEX(1u)     // 0
/// PTL_IS_UHEX(0x000u) // 1
/// PTL_IS_UHEX(0xFFF)  // 0
/// PTL_IS_UHEX((), ()) // 0
#define PTL_IS_UHEX(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_UHEX_, PTL_IS_UINT(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_UHEX_1           PPUTLIS_UHEX_o
#define PPUTLIS_UHEX_0           PPUTLIS_UHEX_0_fail
#define PPUTLIS_UHEX_0_fail(...) 0
#define PPUTLIS_UHEX_o(uint)     PTL_XCAT(PPUTLIS_UHEX_o_, PPUTLIMPL_UHEX(uint, IS))
#define PPUTLIS_UHEX_o_1         1
#define PPUTLIS_UHEX_o_0         0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_tup]
/// ---------------
/// [extends PTL_IS_OBJ] detects if args is a tuple (parenthesized list).
///
/// PTL_IS_TUP()       // 0
/// PTL_IS_TUP(1, 2)   // 0
/// PTL_IS_TUP(())     // 1
/// PTL_IS_TUP((1, 2)) // 1
#define PTL_IS_TUP(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_TUP_, PTL_IS_OBJ(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_TUP_1           PPUTLIS_TUP_o
#define PPUTLIS_TUP_0           PPUTLIS_TUP_0_fail
#define PPUTLIS_TUP_0_fail(...) 0
#define PPUTLIS_TUP_o(obj)      PTL_IS_NONE(PTL_EAT obj)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_utup]
/// ----------------
/// [extends PTL_IS_TUP] detects if args a tup of exactly PTL_WORD_SIZE (3) hex digits.
///
/// PTL_IS_UTUP()             // 0
/// PTL_IS_UTUP(foo)          // 0
/// PTL_IS_UTUP(0)            // 0
/// PTL_IS_UTUP(9, B, C)      // 0
/// PTL_IS_UTUP((9, B, C))    // 1
/// PTL_IS_UTUP((9, B, C,))   // 0
/// PTL_IS_UTUP((9, B, C, E)) // 0
#define PTL_IS_UTUP(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_UTUP_, PTL_IS_TUP(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_UTUP_1           PPUTLIS_UTUP_o
#define PPUTLIS_UTUP_0           PPUTLIS_UTUP_0_fail
#define PPUTLIS_UTUP_0_fail(...) 0
#define PPUTLIS_UTUP_o(tup) \
  PPUTLIS_UTUP_RES(PPUTLIS_UTUP_R(PPUTLIS_UTUP_R((), PTL_ESC tup)))
#define PPUTLIS_UTUP_R(...)            PPUTLIS_UTUP_R_o(__VA_ARGS__)
#define PPUTLIS_UTUP_R_o(ctup, _, ...) (PTL_IS_HEX(_), PTL_ESC ctup), __VA_ARGS__
#define PPUTLIS_UTUP_RES(...)          PPUTLIS_UTUP_RES_o(__VA_ARGS__)
#define PPUTLIS_UTUP_RES_o(ctup, ...) \
  PPUTLIS_UTUP_RES_oo(PTL_IS_NONE(PPUTLIS_UTUP_CHK ctup), PTL_IS_HEX(__VA_ARGS__))
#define PPUTLIS_UTUP_RES_oo(...)    PPUTLIS_UTUP_RES_ooo(__VA_ARGS__)
#define PPUTLIS_UTUP_RES_ooo(c, n)  PPUTLIS_UTUP_##c##n
#define PPUTLIS_UTUP_CHK(a, b, ...) PPUTLIS_UTUP_CHK_##a##b
#define PPUTLIS_UTUP_CHK_11
#define PPUTLIS_UTUP_11 1
#define PPUTLIS_UTUP_10 0
#define PPUTLIS_UTUP_01 0
#define PPUTLIS_UTUP_00 0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_word]
/// ----------------
/// [extends PTL_IS_OBJ] detects if args is one of int|uint|utup.
///
/// PTL_IS_WORD(0)         // 1
/// PTL_IS_WORD(0u)        // 1
/// PTL_IS_WORD(foo)       // 0
/// PTL_IS_WORD(())        // 0
/// PTL_IS_WORD(A)         // 0
/// PTL_IS_WORD(0x800)     // 1
/// PTL_IS_WORD(4095u)     // 1
/// PTL_IS_WORD(0xFFFu)    // 1
/// PTL_IS_WORD((0, 0, 8)) // 1
#define PTL_IS_WORD(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_WORD_, PTL_IS_OBJ(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_WORD_1           PPUTLIS_WORD_o
#define PPUTLIS_WORD_0           PPUTLIS_WORD_0_fail
#define PPUTLIS_WORD_0_fail(...) 0
#define PPUTLIS_WORD_o(obj)      PTL_XCAT(PPUTLIS_WORD_o_, PPUTLIS_ATOM_o(obj))(obj)
#define PPUTLIS_WORD_o_1(atom)   PTL_XCAT(PPUTLIS_WORD_o_1, PPUTLIS_INT_o(atom))(atom)
#define PPUTLIS_WORD_o_11(int)   1
#define PPUTLIS_WORD_o_10(atom)  PPUTLIS_UINT_o(atom)
#define PPUTLIS_WORD_o_0(obj)    PTL_IS_UTUP(obj)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_size]
/// ----------------
/// [extends PTL_IS_WORD] detects if args is a word within 0 and PTL_SIZE_MAX (255u).
///
/// PTL_IS_SIZE(0)         // 1
/// PTL_IS_SIZE(0u)        // 1
/// PTL_IS_SIZE(foo)       // 0
/// PTL_IS_SIZE(())        // 0
/// PTL_IS_SIZE(A)         // 0
/// PTL_IS_SIZE(0x800)     // 0
/// PTL_IS_SIZE(4095u)     // 0
/// PTL_IS_SIZE(0xFFFu)    // 0
/// PTL_IS_SIZE(255u)      // 1
/// PTL_IS_SIZE((0, 0, 8)) // 1
#define PTL_IS_SIZE(/* list */...) /* -> bool */ \
  PTL_XCAT(PPUTLIS_SIZE_, PTL_IS_WORD(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_SIZE_1           PPUTLIS_SIZE_o
#define PPUTLIS_SIZE_0           PPUTLIS_SIZE_0_fail
#define PPUTLIS_SIZE_0_fail(...) 0
#define PPUTLIS_SIZE_o(word)     PTL_XCAT(PPUTLIS_SIZE_o_, PPUTLIS_TUP_o(word))(word)
#define PPUTLIS_SIZE_o_1(utup)   PTL_ESC(PPUTLIS_SIZE_o_CHK utup)
#define PPUTLIS_SIZE_o_0(atom)   PTL_XCAT(PPUTLIS_SIZE_o_0, PPUTLIS_INT_o(atom))(atom)
#define PPUTLIS_SIZE_o_01(int)   PTL_XCAT(PPUTLIS_SIZE_o_01, PPUTLIS_IDEC_o(int))(int)
#define PPUTLIS_SIZE_o_011(idec) \
  PTL_XCAT(PPUTLIS_SIZE_o_011, PPUTLIMPL_UHEX(PPUTLIMPL_UDEC(idec##u, UHEX), ILTZ))
#define PPUTLIS_SIZE_o_0111 0
#define PPUTLIS_SIZE_o_0110 1
#define PPUTLIS_SIZE_o_010(ihex) \
  PTL_XCAT(PPUTLIS_SIZE_o_010, PPUTLIMPL_UHEX(ihex##u, ILTZ))
#define PPUTLIS_SIZE_o_0101     0
#define PPUTLIS_SIZE_o_0100     1
#define PPUTLIS_SIZE_o_00(uint) PTL_XCAT(PPUTLIS_SIZE_o_00, PPUTLIS_UDEC_o(uint))(uint)
#define PPUTLIS_SIZE_o_001(udec) \
  PTL_ESC(PPUTLIS_SIZE_o_CHK PPUTLIMPL_UHEX(PPUTLIMPL_UDEC(udec, UHEX), UTUP))
#define PPUTLIS_SIZE_o_000(uhex)    PTL_ESC(PPUTLIS_SIZE_o_CHK PPUTLIMPL_UHEX(uhex, UTUP))
#define PPUTLIS_SIZE_o_CHK(a, b, c) PTL_IS_NONE(PPUTLIS_SIZE_o_CHK_##a)
#define PPUTLIS_SIZE_o_CHK_0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.typeof]
/// ---------------
/// detects the value type. must be compatible with the ## operator.
///
/// note: literal 0-9 are always considered idec, not hex.
///       literal 1000-1111 are considered idec (not nybl) iff less than PTL_INT_MAX.
///       words less than PTL_SIZE_MAX are not specially considered to be size values.
///
/// returns one of:
///
///   | LIST | NONE | UTUP | TUP  | IDEC | IHEX
///   | UDEC | UHEX | HEX  | NYBL | ATOM
///
/// PTL_TYPEOF((foo))     // TUP
/// PTL_TYPEOF(0)         // IDEC
/// PTL_TYPEOF(0u)        // UDEC
/// PTL_TYPEOF(D)         // HEX
/// PTL_TYPEOF(4095)      // ATOM
/// PTL_TYPEOF(4095u)     // UDEC
/// PTL_TYPEOF(0xFFF)     // IHEX
/// PTL_TYPEOF(0xFFFu)    // UHEX
/// PTL_TYPEOF(foo)       // ATOM
/// PTL_TYPEOF(001)       // ATOM
/// PTL_TYPEOF(0010)      // NYBL
/// PTL_TYPEOF(foo, bar)  // LIST
/// PTL_TYPEOF((A))       // TUP
/// PTL_TYPEOF((0, 0, 0)) // UTUP
/// PTL_TYPEOF((F, F, F)) // UTUP
/// PTL_TYPEOF()          // NONE
#define PTL_TYPEOF(                                                                    \
    /* list */...) /* -> enum<LIST|NONE|UTUP|TUP|IDEC|IHEX|UDEC|UHEX|HEX|NYBL|ATOM> */ \
  PTL_XCAT(PPUTLTYPEOF_, PPUTLIS_ANY_o(__VA_ARGS__.))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

/// !any
#define PPUTLTYPEOF_1(any)        PTL_XCAT(PPUTLTYPEOF_1, PTL_IS_NONE(any))(any)
/// !any → none
#define PPUTLTYPEOF_11(...)       NONE
/// !any → !none
#define PPUTLTYPEOF_10(obj)       PTL_XCAT(PPUTLTYPEOF_10, PPUTLIS_TUP_o(obj))(obj)
/// !any → !none → tup
#define PPUTLTYPEOF_101(tup)      PTL_XCAT(PPUTLTYPEOF_101, PPUTLIS_UTUP_o(tup))(tup)
/// !any → !none → tup → utup
#define PPUTLTYPEOF_1011(utup)    UTUP
/// !any → !none → tup → !utup
#define PPUTLTYPEOF_1010(tup)     TUP
/// !any → !none → !tup
#define PPUTLTYPEOF_100(atom)     PTL_XCAT(PPUTLTYPEOF_100, PPUTLIS_INT_o(atom))(atom)
/// !any → !none → !tup → int
#define PPUTLTYPEOF_1001(int)     PTL_XCAT(PPUTLTYPEOF_1001, PPUTLIS_IDEC_o(int))(int)
/// !any → !none → !tup → int → idec
#define PPUTLTYPEOF_10011(idec)   IDEC
/// !any → !none → !tup → int → !idec
#define PPUTLTYPEOF_10010(ihex)   IHEX
/// !any → !none → !tup → !int
#define PPUTLTYPEOF_1000(atom)    PTL_XCAT(PPUTLTYPEOF_1000, PPUTLIS_UINT_o(atom))(atom)
/// !any → !none → !tup → !int → uint
#define PPUTLTYPEOF_10001(uint)   PTL_XCAT(PPUTLTYPEOF_10001, PPUTLIS_UDEC_o(uint))(uint)
/// !any → !none → !tup → !int → uint → udec
#define PPUTLTYPEOF_100011(udec)  UDEC
/// !any → !none → !tup → !int → uint → !udec
#define PPUTLTYPEOF_100010(uhex)  UHEX
/// !any → !none → !tup → !int → !uint
#define PPUTLTYPEOF_10000(atom)   PTL_XCAT(PPUTLTYPEOF_10000, PTL_IS_HEX(atom))(atom)
/// !any → !none → !tup → !int → !uint → hex
#define PPUTLTYPEOF_100001(hex)   HEX
/// !any → !none → !tup → !int → !uint → !hex
#define PPUTLTYPEOF_100000(atom)  PTL_XCAT(PPUTLTYPEOF_100000, PTL_IS_NYBL(atom))(atom)
/// !any → !none → !tup → !int → !uint → !hex → nybl
#define PPUTLTYPEOF_1000001(nybl) NYBL
/// !any → !none → !tup → !int → !uint → !hex → !nybl
#define PPUTLTYPEOF_1000000(atom) ATOM
/// !any
#define PPUTLTYPEOF_0(...)        LIST

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.sizeof]
/// ---------------
/// counts the number of arguments.
/// fails if larger than PTL_SIZE_MAX (255u)
///
/// PTL_SIZEOF()     // 0u
/// PTL_SIZEOF(a)    // 1u
/// PTL_SIZEOF(a, b) // 2u
/// PTL_SIZEOF(, )   // 2u
#define PTL_SIZEOF(/* list */...) /* -> udec&size */ \
  PPUTLSIZEOF_o(PTL_STR([PTL_SIZEOF] too many arguments : __VA_ARGS__), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLSIZEOF_o(e, ...) PTL_XFIRST(__VA_OPT__(PPUTLSIZEOF_N1(e, __VA_ARGS__.), ) 0u)

#define PPUTLSIZEOF_N1(e, _, ...)   __VA_OPT__(PPUTLSIZEOF_N2(e, __VA_ARGS__), ) 1u
#define PPUTLSIZEOF_N2(e, _, ...)   __VA_OPT__(PPUTLSIZEOF_N3(e, __VA_ARGS__), ) 2u
#define PPUTLSIZEOF_N3(e, _, ...)   __VA_OPT__(PPUTLSIZEOF_N4(e, __VA_ARGS__), ) 3u
#define PPUTLSIZEOF_N4(e, _, ...)   __VA_OPT__(PPUTLSIZEOF_N5(e, __VA_ARGS__), ) 4u
#define PPUTLSIZEOF_N5(e, _, ...)   __VA_OPT__(PPUTLSIZEOF_N6(e, __VA_ARGS__), ) 5u
#define PPUTLSIZEOF_N6(e, _, ...)   __VA_OPT__(PPUTLSIZEOF_N7(e, __VA_ARGS__), ) 6u
#define PPUTLSIZEOF_N7(e, _, ...)   __VA_OPT__(PPUTLSIZEOF_N8(e, __VA_ARGS__), ) 7u
#define PPUTLSIZEOF_N8(e, _, ...)   __VA_OPT__(PPUTLSIZEOF_N9(e, __VA_ARGS__), ) 8u
#define PPUTLSIZEOF_N9(e, _, ...)   __VA_OPT__(PPUTLSIZEOF_N10(e, __VA_ARGS__), ) 9u
#define PPUTLSIZEOF_N10(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N11(e, __VA_ARGS__), ) 10u
#define PPUTLSIZEOF_N11(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N12(e, __VA_ARGS__), ) 11u
#define PPUTLSIZEOF_N12(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N13(e, __VA_ARGS__), ) 12u
#define PPUTLSIZEOF_N13(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N14(e, __VA_ARGS__), ) 13u
#define PPUTLSIZEOF_N14(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N15(e, __VA_ARGS__), ) 14u
#define PPUTLSIZEOF_N15(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N16(e, __VA_ARGS__), ) 15u
#define PPUTLSIZEOF_N16(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N17(e, __VA_ARGS__), ) 16u
#define PPUTLSIZEOF_N17(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N18(e, __VA_ARGS__), ) 17u
#define PPUTLSIZEOF_N18(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N19(e, __VA_ARGS__), ) 18u
#define PPUTLSIZEOF_N19(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N20(e, __VA_ARGS__), ) 19u
#define PPUTLSIZEOF_N20(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N21(e, __VA_ARGS__), ) 20u
#define PPUTLSIZEOF_N21(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N22(e, __VA_ARGS__), ) 21u
#define PPUTLSIZEOF_N22(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N23(e, __VA_ARGS__), ) 22u
#define PPUTLSIZEOF_N23(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N24(e, __VA_ARGS__), ) 23u
#define PPUTLSIZEOF_N24(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N25(e, __VA_ARGS__), ) 24u
#define PPUTLSIZEOF_N25(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N26(e, __VA_ARGS__), ) 25u
#define PPUTLSIZEOF_N26(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N27(e, __VA_ARGS__), ) 26u
#define PPUTLSIZEOF_N27(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N28(e, __VA_ARGS__), ) 27u
#define PPUTLSIZEOF_N28(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N29(e, __VA_ARGS__), ) 28u
#define PPUTLSIZEOF_N29(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N30(e, __VA_ARGS__), ) 29u
#define PPUTLSIZEOF_N30(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N31(e, __VA_ARGS__), ) 30u
#define PPUTLSIZEOF_N31(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N32(e, __VA_ARGS__), ) 31u
#define PPUTLSIZEOF_N32(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N33(e, __VA_ARGS__), ) 32u
#define PPUTLSIZEOF_N33(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N34(e, __VA_ARGS__), ) 33u
#define PPUTLSIZEOF_N34(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N35(e, __VA_ARGS__), ) 34u
#define PPUTLSIZEOF_N35(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N36(e, __VA_ARGS__), ) 35u
#define PPUTLSIZEOF_N36(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N37(e, __VA_ARGS__), ) 36u
#define PPUTLSIZEOF_N37(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N38(e, __VA_ARGS__), ) 37u
#define PPUTLSIZEOF_N38(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N39(e, __VA_ARGS__), ) 38u
#define PPUTLSIZEOF_N39(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N40(e, __VA_ARGS__), ) 39u
#define PPUTLSIZEOF_N40(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N41(e, __VA_ARGS__), ) 40u
#define PPUTLSIZEOF_N41(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N42(e, __VA_ARGS__), ) 41u
#define PPUTLSIZEOF_N42(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N43(e, __VA_ARGS__), ) 42u
#define PPUTLSIZEOF_N43(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N44(e, __VA_ARGS__), ) 43u
#define PPUTLSIZEOF_N44(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N45(e, __VA_ARGS__), ) 44u
#define PPUTLSIZEOF_N45(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N46(e, __VA_ARGS__), ) 45u
#define PPUTLSIZEOF_N46(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N47(e, __VA_ARGS__), ) 46u
#define PPUTLSIZEOF_N47(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N48(e, __VA_ARGS__), ) 47u
#define PPUTLSIZEOF_N48(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N49(e, __VA_ARGS__), ) 48u
#define PPUTLSIZEOF_N49(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N50(e, __VA_ARGS__), ) 49u
#define PPUTLSIZEOF_N50(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N51(e, __VA_ARGS__), ) 50u
#define PPUTLSIZEOF_N51(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N52(e, __VA_ARGS__), ) 51u
#define PPUTLSIZEOF_N52(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N53(e, __VA_ARGS__), ) 52u
#define PPUTLSIZEOF_N53(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N54(e, __VA_ARGS__), ) 53u
#define PPUTLSIZEOF_N54(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N55(e, __VA_ARGS__), ) 54u
#define PPUTLSIZEOF_N55(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N56(e, __VA_ARGS__), ) 55u
#define PPUTLSIZEOF_N56(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N57(e, __VA_ARGS__), ) 56u
#define PPUTLSIZEOF_N57(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N58(e, __VA_ARGS__), ) 57u
#define PPUTLSIZEOF_N58(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N59(e, __VA_ARGS__), ) 58u
#define PPUTLSIZEOF_N59(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N60(e, __VA_ARGS__), ) 59u
#define PPUTLSIZEOF_N60(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N61(e, __VA_ARGS__), ) 60u
#define PPUTLSIZEOF_N61(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N62(e, __VA_ARGS__), ) 61u
#define PPUTLSIZEOF_N62(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N63(e, __VA_ARGS__), ) 62u
#define PPUTLSIZEOF_N63(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N64(e, __VA_ARGS__), ) 63u
#define PPUTLSIZEOF_N64(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N65(e, __VA_ARGS__), ) 64u
#define PPUTLSIZEOF_N65(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N66(e, __VA_ARGS__), ) 65u
#define PPUTLSIZEOF_N66(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N67(e, __VA_ARGS__), ) 66u
#define PPUTLSIZEOF_N67(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N68(e, __VA_ARGS__), ) 67u
#define PPUTLSIZEOF_N68(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N69(e, __VA_ARGS__), ) 68u
#define PPUTLSIZEOF_N69(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N70(e, __VA_ARGS__), ) 69u
#define PPUTLSIZEOF_N70(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N71(e, __VA_ARGS__), ) 70u
#define PPUTLSIZEOF_N71(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N72(e, __VA_ARGS__), ) 71u
#define PPUTLSIZEOF_N72(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N73(e, __VA_ARGS__), ) 72u
#define PPUTLSIZEOF_N73(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N74(e, __VA_ARGS__), ) 73u
#define PPUTLSIZEOF_N74(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N75(e, __VA_ARGS__), ) 74u
#define PPUTLSIZEOF_N75(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N76(e, __VA_ARGS__), ) 75u
#define PPUTLSIZEOF_N76(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N77(e, __VA_ARGS__), ) 76u
#define PPUTLSIZEOF_N77(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N78(e, __VA_ARGS__), ) 77u
#define PPUTLSIZEOF_N78(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N79(e, __VA_ARGS__), ) 78u
#define PPUTLSIZEOF_N79(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N80(e, __VA_ARGS__), ) 79u
#define PPUTLSIZEOF_N80(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N81(e, __VA_ARGS__), ) 80u
#define PPUTLSIZEOF_N81(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N82(e, __VA_ARGS__), ) 81u
#define PPUTLSIZEOF_N82(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N83(e, __VA_ARGS__), ) 82u
#define PPUTLSIZEOF_N83(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N84(e, __VA_ARGS__), ) 83u
#define PPUTLSIZEOF_N84(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N85(e, __VA_ARGS__), ) 84u
#define PPUTLSIZEOF_N85(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N86(e, __VA_ARGS__), ) 85u
#define PPUTLSIZEOF_N86(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N87(e, __VA_ARGS__), ) 86u
#define PPUTLSIZEOF_N87(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N88(e, __VA_ARGS__), ) 87u
#define PPUTLSIZEOF_N88(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N89(e, __VA_ARGS__), ) 88u
#define PPUTLSIZEOF_N89(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N90(e, __VA_ARGS__), ) 89u
#define PPUTLSIZEOF_N90(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N91(e, __VA_ARGS__), ) 90u
#define PPUTLSIZEOF_N91(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N92(e, __VA_ARGS__), ) 91u
#define PPUTLSIZEOF_N92(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N93(e, __VA_ARGS__), ) 92u
#define PPUTLSIZEOF_N93(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N94(e, __VA_ARGS__), ) 93u
#define PPUTLSIZEOF_N94(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N95(e, __VA_ARGS__), ) 94u
#define PPUTLSIZEOF_N95(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N96(e, __VA_ARGS__), ) 95u
#define PPUTLSIZEOF_N96(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N97(e, __VA_ARGS__), ) 96u
#define PPUTLSIZEOF_N97(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N98(e, __VA_ARGS__), ) 97u
#define PPUTLSIZEOF_N98(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N99(e, __VA_ARGS__), ) 98u
#define PPUTLSIZEOF_N99(e, _, ...)  __VA_OPT__(PPUTLSIZEOF_N100(e, __VA_ARGS__), ) 99u
#define PPUTLSIZEOF_N100(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N101(e, __VA_ARGS__), ) 100u
#define PPUTLSIZEOF_N101(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N102(e, __VA_ARGS__), ) 101u
#define PPUTLSIZEOF_N102(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N103(e, __VA_ARGS__), ) 102u
#define PPUTLSIZEOF_N103(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N104(e, __VA_ARGS__), ) 103u
#define PPUTLSIZEOF_N104(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N105(e, __VA_ARGS__), ) 104u
#define PPUTLSIZEOF_N105(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N106(e, __VA_ARGS__), ) 105u
#define PPUTLSIZEOF_N106(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N107(e, __VA_ARGS__), ) 106u
#define PPUTLSIZEOF_N107(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N108(e, __VA_ARGS__), ) 107u
#define PPUTLSIZEOF_N108(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N109(e, __VA_ARGS__), ) 108u
#define PPUTLSIZEOF_N109(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N110(e, __VA_ARGS__), ) 109u
#define PPUTLSIZEOF_N110(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N111(e, __VA_ARGS__), ) 110u
#define PPUTLSIZEOF_N111(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N112(e, __VA_ARGS__), ) 111u
#define PPUTLSIZEOF_N112(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N113(e, __VA_ARGS__), ) 112u
#define PPUTLSIZEOF_N113(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N114(e, __VA_ARGS__), ) 113u
#define PPUTLSIZEOF_N114(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N115(e, __VA_ARGS__), ) 114u
#define PPUTLSIZEOF_N115(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N116(e, __VA_ARGS__), ) 115u
#define PPUTLSIZEOF_N116(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N117(e, __VA_ARGS__), ) 116u
#define PPUTLSIZEOF_N117(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N118(e, __VA_ARGS__), ) 117u
#define PPUTLSIZEOF_N118(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N119(e, __VA_ARGS__), ) 118u
#define PPUTLSIZEOF_N119(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N120(e, __VA_ARGS__), ) 119u
#define PPUTLSIZEOF_N120(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N121(e, __VA_ARGS__), ) 120u
#define PPUTLSIZEOF_N121(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N122(e, __VA_ARGS__), ) 121u
#define PPUTLSIZEOF_N122(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N123(e, __VA_ARGS__), ) 122u
#define PPUTLSIZEOF_N123(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N124(e, __VA_ARGS__), ) 123u
#define PPUTLSIZEOF_N124(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N125(e, __VA_ARGS__), ) 124u
#define PPUTLSIZEOF_N125(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N126(e, __VA_ARGS__), ) 125u
#define PPUTLSIZEOF_N126(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N127(e, __VA_ARGS__), ) 126u
#define PPUTLSIZEOF_N127(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N128(e, __VA_ARGS__), ) 127u
#define PPUTLSIZEOF_N128(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N129(e, __VA_ARGS__), ) 128u
#define PPUTLSIZEOF_N129(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N130(e, __VA_ARGS__), ) 129u
#define PPUTLSIZEOF_N130(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N131(e, __VA_ARGS__), ) 130u
#define PPUTLSIZEOF_N131(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N132(e, __VA_ARGS__), ) 131u
#define PPUTLSIZEOF_N132(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N133(e, __VA_ARGS__), ) 132u
#define PPUTLSIZEOF_N133(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N134(e, __VA_ARGS__), ) 133u
#define PPUTLSIZEOF_N134(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N135(e, __VA_ARGS__), ) 134u
#define PPUTLSIZEOF_N135(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N136(e, __VA_ARGS__), ) 135u
#define PPUTLSIZEOF_N136(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N137(e, __VA_ARGS__), ) 136u
#define PPUTLSIZEOF_N137(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N138(e, __VA_ARGS__), ) 137u
#define PPUTLSIZEOF_N138(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N139(e, __VA_ARGS__), ) 138u
#define PPUTLSIZEOF_N139(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N140(e, __VA_ARGS__), ) 139u
#define PPUTLSIZEOF_N140(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N141(e, __VA_ARGS__), ) 140u
#define PPUTLSIZEOF_N141(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N142(e, __VA_ARGS__), ) 141u
#define PPUTLSIZEOF_N142(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N143(e, __VA_ARGS__), ) 142u
#define PPUTLSIZEOF_N143(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N144(e, __VA_ARGS__), ) 143u
#define PPUTLSIZEOF_N144(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N145(e, __VA_ARGS__), ) 144u
#define PPUTLSIZEOF_N145(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N146(e, __VA_ARGS__), ) 145u
#define PPUTLSIZEOF_N146(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N147(e, __VA_ARGS__), ) 146u
#define PPUTLSIZEOF_N147(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N148(e, __VA_ARGS__), ) 147u
#define PPUTLSIZEOF_N148(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N149(e, __VA_ARGS__), ) 148u
#define PPUTLSIZEOF_N149(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N150(e, __VA_ARGS__), ) 149u
#define PPUTLSIZEOF_N150(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N151(e, __VA_ARGS__), ) 150u
#define PPUTLSIZEOF_N151(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N152(e, __VA_ARGS__), ) 151u
#define PPUTLSIZEOF_N152(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N153(e, __VA_ARGS__), ) 152u
#define PPUTLSIZEOF_N153(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N154(e, __VA_ARGS__), ) 153u
#define PPUTLSIZEOF_N154(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N155(e, __VA_ARGS__), ) 154u
#define PPUTLSIZEOF_N155(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N156(e, __VA_ARGS__), ) 155u
#define PPUTLSIZEOF_N156(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N157(e, __VA_ARGS__), ) 156u
#define PPUTLSIZEOF_N157(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N158(e, __VA_ARGS__), ) 157u
#define PPUTLSIZEOF_N158(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N159(e, __VA_ARGS__), ) 158u
#define PPUTLSIZEOF_N159(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N160(e, __VA_ARGS__), ) 159u
#define PPUTLSIZEOF_N160(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N161(e, __VA_ARGS__), ) 160u
#define PPUTLSIZEOF_N161(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N162(e, __VA_ARGS__), ) 161u
#define PPUTLSIZEOF_N162(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N163(e, __VA_ARGS__), ) 162u
#define PPUTLSIZEOF_N163(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N164(e, __VA_ARGS__), ) 163u
#define PPUTLSIZEOF_N164(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N165(e, __VA_ARGS__), ) 164u
#define PPUTLSIZEOF_N165(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N166(e, __VA_ARGS__), ) 165u
#define PPUTLSIZEOF_N166(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N167(e, __VA_ARGS__), ) 166u
#define PPUTLSIZEOF_N167(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N168(e, __VA_ARGS__), ) 167u
#define PPUTLSIZEOF_N168(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N169(e, __VA_ARGS__), ) 168u
#define PPUTLSIZEOF_N169(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N170(e, __VA_ARGS__), ) 169u
#define PPUTLSIZEOF_N170(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N171(e, __VA_ARGS__), ) 170u
#define PPUTLSIZEOF_N171(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N172(e, __VA_ARGS__), ) 171u
#define PPUTLSIZEOF_N172(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N173(e, __VA_ARGS__), ) 172u
#define PPUTLSIZEOF_N173(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N174(e, __VA_ARGS__), ) 173u
#define PPUTLSIZEOF_N174(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N175(e, __VA_ARGS__), ) 174u
#define PPUTLSIZEOF_N175(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N176(e, __VA_ARGS__), ) 175u
#define PPUTLSIZEOF_N176(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N177(e, __VA_ARGS__), ) 176u
#define PPUTLSIZEOF_N177(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N178(e, __VA_ARGS__), ) 177u
#define PPUTLSIZEOF_N178(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N179(e, __VA_ARGS__), ) 178u
#define PPUTLSIZEOF_N179(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N180(e, __VA_ARGS__), ) 179u
#define PPUTLSIZEOF_N180(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N181(e, __VA_ARGS__), ) 180u
#define PPUTLSIZEOF_N181(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N182(e, __VA_ARGS__), ) 181u
#define PPUTLSIZEOF_N182(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N183(e, __VA_ARGS__), ) 182u
#define PPUTLSIZEOF_N183(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N184(e, __VA_ARGS__), ) 183u
#define PPUTLSIZEOF_N184(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N185(e, __VA_ARGS__), ) 184u
#define PPUTLSIZEOF_N185(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N186(e, __VA_ARGS__), ) 185u
#define PPUTLSIZEOF_N186(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N187(e, __VA_ARGS__), ) 186u
#define PPUTLSIZEOF_N187(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N188(e, __VA_ARGS__), ) 187u
#define PPUTLSIZEOF_N188(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N189(e, __VA_ARGS__), ) 188u
#define PPUTLSIZEOF_N189(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N190(e, __VA_ARGS__), ) 189u
#define PPUTLSIZEOF_N190(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N191(e, __VA_ARGS__), ) 190u
#define PPUTLSIZEOF_N191(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N192(e, __VA_ARGS__), ) 191u
#define PPUTLSIZEOF_N192(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N193(e, __VA_ARGS__), ) 192u
#define PPUTLSIZEOF_N193(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N194(e, __VA_ARGS__), ) 193u
#define PPUTLSIZEOF_N194(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N195(e, __VA_ARGS__), ) 194u
#define PPUTLSIZEOF_N195(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N196(e, __VA_ARGS__), ) 195u
#define PPUTLSIZEOF_N196(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N197(e, __VA_ARGS__), ) 196u
#define PPUTLSIZEOF_N197(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N198(e, __VA_ARGS__), ) 197u
#define PPUTLSIZEOF_N198(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N199(e, __VA_ARGS__), ) 198u
#define PPUTLSIZEOF_N199(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N200(e, __VA_ARGS__), ) 199u
#define PPUTLSIZEOF_N200(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N201(e, __VA_ARGS__), ) 200u
#define PPUTLSIZEOF_N201(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N202(e, __VA_ARGS__), ) 201u
#define PPUTLSIZEOF_N202(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N203(e, __VA_ARGS__), ) 202u
#define PPUTLSIZEOF_N203(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N204(e, __VA_ARGS__), ) 203u
#define PPUTLSIZEOF_N204(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N205(e, __VA_ARGS__), ) 204u
#define PPUTLSIZEOF_N205(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N206(e, __VA_ARGS__), ) 205u
#define PPUTLSIZEOF_N206(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N207(e, __VA_ARGS__), ) 206u
#define PPUTLSIZEOF_N207(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N208(e, __VA_ARGS__), ) 207u
#define PPUTLSIZEOF_N208(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N209(e, __VA_ARGS__), ) 208u
#define PPUTLSIZEOF_N209(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N210(e, __VA_ARGS__), ) 209u
#define PPUTLSIZEOF_N210(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N211(e, __VA_ARGS__), ) 210u
#define PPUTLSIZEOF_N211(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N212(e, __VA_ARGS__), ) 211u
#define PPUTLSIZEOF_N212(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N213(e, __VA_ARGS__), ) 212u
#define PPUTLSIZEOF_N213(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N214(e, __VA_ARGS__), ) 213u
#define PPUTLSIZEOF_N214(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N215(e, __VA_ARGS__), ) 214u
#define PPUTLSIZEOF_N215(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N216(e, __VA_ARGS__), ) 215u
#define PPUTLSIZEOF_N216(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N217(e, __VA_ARGS__), ) 216u
#define PPUTLSIZEOF_N217(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N218(e, __VA_ARGS__), ) 217u
#define PPUTLSIZEOF_N218(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N219(e, __VA_ARGS__), ) 218u
#define PPUTLSIZEOF_N219(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N220(e, __VA_ARGS__), ) 219u
#define PPUTLSIZEOF_N220(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N221(e, __VA_ARGS__), ) 220u
#define PPUTLSIZEOF_N221(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N222(e, __VA_ARGS__), ) 221u
#define PPUTLSIZEOF_N222(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N223(e, __VA_ARGS__), ) 222u
#define PPUTLSIZEOF_N223(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N224(e, __VA_ARGS__), ) 223u
#define PPUTLSIZEOF_N224(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N225(e, __VA_ARGS__), ) 224u
#define PPUTLSIZEOF_N225(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N226(e, __VA_ARGS__), ) 225u
#define PPUTLSIZEOF_N226(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N227(e, __VA_ARGS__), ) 226u
#define PPUTLSIZEOF_N227(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N228(e, __VA_ARGS__), ) 227u
#define PPUTLSIZEOF_N228(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N229(e, __VA_ARGS__), ) 228u
#define PPUTLSIZEOF_N229(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N230(e, __VA_ARGS__), ) 229u
#define PPUTLSIZEOF_N230(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N231(e, __VA_ARGS__), ) 230u
#define PPUTLSIZEOF_N231(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N232(e, __VA_ARGS__), ) 231u
#define PPUTLSIZEOF_N232(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N233(e, __VA_ARGS__), ) 232u
#define PPUTLSIZEOF_N233(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N234(e, __VA_ARGS__), ) 233u
#define PPUTLSIZEOF_N234(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N235(e, __VA_ARGS__), ) 234u
#define PPUTLSIZEOF_N235(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N236(e, __VA_ARGS__), ) 235u
#define PPUTLSIZEOF_N236(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N237(e, __VA_ARGS__), ) 236u
#define PPUTLSIZEOF_N237(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N238(e, __VA_ARGS__), ) 237u
#define PPUTLSIZEOF_N238(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N239(e, __VA_ARGS__), ) 238u
#define PPUTLSIZEOF_N239(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N240(e, __VA_ARGS__), ) 239u
#define PPUTLSIZEOF_N240(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N241(e, __VA_ARGS__), ) 240u
#define PPUTLSIZEOF_N241(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N242(e, __VA_ARGS__), ) 241u
#define PPUTLSIZEOF_N242(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N243(e, __VA_ARGS__), ) 242u
#define PPUTLSIZEOF_N243(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N244(e, __VA_ARGS__), ) 243u
#define PPUTLSIZEOF_N244(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N245(e, __VA_ARGS__), ) 244u
#define PPUTLSIZEOF_N245(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N246(e, __VA_ARGS__), ) 245u
#define PPUTLSIZEOF_N246(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N247(e, __VA_ARGS__), ) 246u
#define PPUTLSIZEOF_N247(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N248(e, __VA_ARGS__), ) 247u
#define PPUTLSIZEOF_N248(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N249(e, __VA_ARGS__), ) 248u
#define PPUTLSIZEOF_N249(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N250(e, __VA_ARGS__), ) 249u
#define PPUTLSIZEOF_N250(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N251(e, __VA_ARGS__), ) 250u
#define PPUTLSIZEOF_N251(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N252(e, __VA_ARGS__), ) 251u
#define PPUTLSIZEOF_N252(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N253(e, __VA_ARGS__), ) 252u
#define PPUTLSIZEOF_N253(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N254(e, __VA_ARGS__), ) 253u
#define PPUTLSIZEOF_N254(e, _, ...) __VA_OPT__(PPUTLSIZEOF_N255(e, __VA_ARGS__), ) 254u
#define PPUTLSIZEOF_N255(e, _, ...) __VA_OPT__(PTL_FAIL(e), ) 255u

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.list]
/// -----------
/// list type. returns args in all cases (root type; everything matches)
///
/// PTL_LIST()         // <nothing>
/// PTL_LIST(foo)      // foo
/// PTL_LIST(foo, bar) // foo, bar
#define PTL_LIST(/* list */...) /* -> list */ __VA_ARGS__

/// [type.any]
/// ----------
/// (none|obj) a potentially-empty, individual macro argument.
/// fails if more than one arg.
///
/// PTL_ANY()    // <nothing>
/// PTL_ANY(foo) // foo
#define PTL_ANY(/* any */...) /* -> any */         \
  PTL_XCAT(PPUTLANY_, PPUTLIS_ANY_o(__VA_ARGS__.)) \
  (PTL_STR([PTL_ANY] any cannot describe multiple args : __VA_ARGS__), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLANY_1(e, obj) obj
#define PPUTLANY_0(e, ...) PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.none]
/// -----------
/// [inherits from PTL_ANY] nothing. fails if something.
///
/// PTL_NONE() // <nothing>
#define PTL_NONE(/* none */...) /* -> none */    \
  PTL_XCAT(PPUTLNONE_, PTL_IS_NONE(__VA_ARGS__)) \
  (PTL_STR([PTL_NONE] none cannot describe something : __VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLNONE_1(e)
#define PPUTLNONE_0(e) PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.obj]
/// ----------
/// [inherits from PTL_ANY] exactly one non-empty generic value.
///
/// PTL_OBJ(foo) // foo
#define PTL_OBJ(/* obj */...) /* -> obj */                      \
  PPUTLOBJ_o(PTL_STR([PTL_OBJ] obj cannot describe empty values \
                     : __VA_ARGS__),                            \
             PTL_ANY(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLOBJ_o(e, ...) \
  PTL_XCAT(PPUTLOBJ_, PPUTLIS_OBJ_o(__VA_ARGS__.))(e, __VA_ARGS__)
#define PPUTLOBJ_1(e, obj) obj
#define PPUTLOBJ_0(e, ...) PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.atom]
/// -----------
/// [inherits from PTL_OBJ] an individual value that may form an identifier tail.
///
/// this function only tests for tuples and multiple values.
///
/// while not testable, the true semantics of atom implies
/// that its values are able to concatenate with identifiers
/// to form new identifiers, meaning that is must match /[\w\d_]+/.
///
/// this property is critical for value-based control flow
/// and must be observed by the user where applicable.
///
/// PTL_ATOM(foo) // foo
#define PTL_ATOM(/* obj */...) /* -> atom */                 \
  PPUTLATOM_o(PTL_STR([PTL_ATOM] atom cannot describe tuples \
                      : __VA_ARGS__),                        \
              PTL_OBJ(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLATOM_o(e, obj)  PTL_XCAT(PPUTLATOM_, PPUTLIS_ATOM_o(obj))(e, obj)
#define PPUTLATOM_1(e, atom) atom
#define PPUTLATOM_0(e, ...)  PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.enum]
/// -----------
/// [inherits from PTL_ATOM] enum type. describes an atom union.
///
/// to use this function, define a set of
/// macros with the following characteristics:
///  ‐ object-like
///  ‐ common prefix
///  ‐ enum value suffixes
///  ‐ expand to nothing
/// pass the common prefix as chkprefix.
///
/// example: (asserting an enum<GOOD|BAD>)
///  #define FOO_GOOD
///  #define FOO_BAD
///  PTL_ENUM(FOO_, BLEH) // <fail>
///  PTL_ENUM(FOO_, GOOD) // GOOD
///  PTL_ENUM(FOO_, ,,,)  // <fail>
#define PTL_ENUM(/* chkprefix: atom, enum<...> */...) /* -> enum<...> */ \
  PPUTLENUM_o(PTL_STR([PTL_ENUM] enum validation failure                 \
                      : __VA_ARGS__),                                    \
              PTL_ATOM(PTL_FIRST(__VA_ARGS__)), PTL_ATOM(PTL_REST(__VA_ARGS__)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLENUM_o(e, chkatom, vatom) \
  PTL_XCAT(PPUTLENUM_, PPUTLIS_ENUM_o(chkatom, vatom))(e, vatom)
#define PPUTLENUM_1(e, enum) enum
#define PPUTLENUM_0(e, ...)  PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.bool]
/// -----------
/// [specializes PTL_ENUM] bool type (enum<0|1>).
/// expands to b if valid, else fails.
///
/// PTL_BOOL(0) // 0
/// PTL_BOOL(1) // 1
#define PTL_BOOL(/* bool */...) /* -> bool */                                      \
  PPUTLBOOL_o(                                                                     \
      PTL_STR([PTL_BOOL] bool cannot describe anything but the literal '1' and '0' \
              : __VA_ARGS__),                                                      \
      PTL_ATOM(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBOOL_o(e, atom) PTL_XCAT(PPUTLBOOL_, PTL_IS_BOOL(atom))(e, atom)
#define PPUTLBOOL_1(e, bool) bool
#define PPUTLBOOL_0(e, ...)  PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.hex]
/// ----------
/// [inherits from PTL_ATOM] capital hex digit type.
/// constructible from either hex or nybl.
/// expands to v if valid, else fails.
///
/// PTL_HEX(0)    // 0
/// PTL_HEX(F)    // F
/// PTL_HEX(0110) // 6
/// PTL_HEX(1010) // A
#define PTL_HEX(/* hex|nybl */...) /* -> hex */                       \
  PPUTLHEX_o(PTL_STR([PTL_HEX] invalid arguments; must be hex or nybl \
                     : __VA_ARGS__),                                  \
             PTL_ATOM(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLHEX_o(e, atom) \
  PTL_XCAT(PPUTLHEX_, PTL_XCAT(PPUTLIS_HEX_o(atom), PPUTLIS_NYBL_o(atom)))(e, atom)
#define PPUTLHEX_10(e, hex)  hex
#define PPUTLHEX_01(e, nybl) PPUTLIMPL_NYBL(nybl, HEX)
#define PPUTLHEX_00(e, ...)  PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.nybl]
/// -----------
/// [inherits from PTL_ATOM] 4-bit bool concatenation type.
/// constructible from either nybl or hex.
/// expands to v if valid, else fails.
///
/// PTL_NYBL(0000) // 0000
/// PTL_NYBL(0110) // 0110
/// PTL_NYBL(5)    // 0101
/// PTL_NYBL(A)    // 1010
#define PTL_NYBL(/* hex|nybl */...) /* -> nybl */                       \
  PPUTLNYBL_o(PTL_STR([PTL_NYBL] invalid arguments; must be nybl or hex \
                      : __VA_ARGS__),                                   \
              PTL_ATOM(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLNYBL_o(e, atom) \
  PTL_XCAT(PPUTLNYBL_, PTL_XCAT(PPUTLIS_NYBL_o(atom), PPUTLIS_HEX_o(atom)))(e, atom)
#define PPUTLNYBL_10(e, nybl) nybl
#define PPUTLNYBL_01(e, hex)  PPUTLIMPL_HEX(hex, NYBL)
#define PPUTLNYBL_00(e, ...)  PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.int]
/// ----------
/// [inherits from PTL_ATOM] 12-bit signed integer type.
/// constructible from any word.
/// instance is either idec or ihex.
///
/// cannot parse negative decimals; use math.neg instead.
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
///   utup → ihex | [default]; fallback for failed utup → idec
///   utup → idec | requires IDEC hint and positive result
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
/// PTL_INT(0)               // 0
/// PTL_INT(1, IHEX)         // 0x001
/// PTL_INT(0x002)           // 0x002
/// PTL_INT(0x800, IDEC)     // 0x800
/// PTL_INT(0x002, IDEC)     // 2
/// PTL_INT(7u)              // 7
/// PTL_INT(15u, IHEX)       // 0x00F
/// PTL_INT(4095u)           // 0xFFF
/// PTL_INT(0x007u)          // 0x007
/// PTL_INT(0xFFFu, IDEC)    // 0xFFF
/// PTL_INT(0x005u, IDEC)    // 5
/// PTL_INT((0, 0, 0))       // 0x000
/// PTL_INT((8, 0, 0), IDEC) // 0x800
/// PTL_INT((7, F, F), IDEC) // 2047
#define PTL_INT(/* word, hint=AUTO: enum<IDEC|IHEX|AUTO> */...) /* -> int */ \
  PPUTLINT_o(PTL_STR([PTL_INT] invalid arguments : __VA_ARGS__), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLINT_o(e, v, ...)                                                       \
  PTL_XCAT(PPUTLINT_,                                                               \
           PPUTLINT_MODE(e, PTL_TYPEOF(v),                                          \
                         PTL_ENUM(PPUTLINT_HINT_, PTL_DEFAULT(AUTO, __VA_ARGS__)))) \
  (v)
#define PPUTLINT_XW_IH(utup)      PPUTLINT_XW_IH_o utup
#define PPUTLINT_XW_IH_o(a, b, c) PPUTLIMPL_UHEX(0x##a##b##c##u, IHEX)
#define PPUTLINT_XW_IC(utup)      PPUTLINT_XW_IC_o utup
#define PPUTLINT_XW_IC_o(a, b, c) PPUTLIMPL_UHEX(0x##a##b##c##u, ICAST)
#define PPUTLINT_UH_IH(uhex)      PPUTLIMPL_UHEX(uhex, IHEX)
#define PPUTLINT_UH_IC(uhex)      PPUTLIMPL_UHEX(uhex, ICAST)
#define PPUTLINT_UD_IH(udec)      PPUTLIMPL_UHEX(PPUTLIMPL_UDEC(udec, UHEX), IHEX)
#define PPUTLINT_UD_IC(udec)      PPUTLIMPL_UHEX(PPUTLIMPL_UDEC(udec, UHEX), ICAST)
#define PPUTLINT_IH_IH(ihex)      ihex
#define PPUTLINT_IH_IC(ihex)      PPUTLIMPL_UHEX(ihex##u, ICAST)
#define PPUTLINT_ID_IH(idec)      PPUTLIMPL_UHEX(PPUTLIMPL_UDEC(idec##u, UHEX), IHEX)
#define PPUTLINT_ID_ID(idec)      idec

/// cast mode selector and error detector
#define PPUTLINT_MODE(e, t, hint) \
  PTL_XCAT(PPUTLINT_MODE_, PTL_IS_NONE(PTL_XCAT(PPUTLINT_MODE_, t)))(e, t, hint)
#define PPUTLINT_MODE_1(e, t, hint) PPUTLINT_MODE_1_##t##hint
#define PPUTLINT_MODE_1_UTUPAUTO    XW_IH
#define PPUTLINT_MODE_1_UTUPIHEX    XW_IH
#define PPUTLINT_MODE_1_UTUPIDEC    XW_IC
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
#define PPUTLINT_MODE_UTUP
#define PPUTLINT_MODE_UHEX
#define PPUTLINT_MODE_UDEC
#define PPUTLINT_MODE_IHEX
#define PPUTLINT_MODE_IDEC
#define PPUTLINT_HINT_AUTO
#define PPUTLINT_HINT_IHEX
#define PPUTLINT_HINT_IDEC

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
#define PTL_IDEC(/* word */...) /* -> idec */                        \
  PPUTLIDEC_o(PTL_STR([PTL_IDEC] cannot represent negative in base10 \
                      : __VA_ARGS__),                                \
              PTL_INT(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIDEC_o(e, int)  PTL_XCAT(PPUTLIDEC_, PPUTLIS_IDEC_o(int))(e, int)
#define PPUTLIDEC_1(e, idec) idec
#define PPUTLIDEC_0(e, ihex) \
  PTL_XCAT(PPUTLIDEC_0, PPUTLIMPL_UHEX(ihex##u, ILTZ))(e, ihex##u)
#define PPUTLIDEC_01(e, uhex) PTL_FAIL(e)
#define PPUTLIDEC_00(e, uhex) PPUTLIMPL_UHEX(uhex, ICAST)

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
#define PTL_IHEX(/* word */...) /* -> ihex */ PPUTLIHEX_o(PTL_INT(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIHEX_o(int)  PTL_XCAT(PPUTLIHEX_, PPUTLIS_IHEX_o(int))(int)
#define PPUTLIHEX_1(ihex) ihex
#define PPUTLIHEX_0(idec) PPUTLIMPL_UHEX(PPUTLIMPL_UDEC(idec##u, UHEX), IHEX)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.uint]
/// -----------
/// [inherits from PTL_ATOM] 12-bit unsigned integer type.
/// constructible from any word.
/// instance is either udec or uhex.
///
/// cannot parse negative decimals; use math.neg instead.
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
///   utup → uhex | [default]
///   utup → udec | requires UDEC hint
///
/// preserves hex/decimal representation.
/// specify hint to choose a cast mode.
///
/// uses a 'u' suffix for both representations.
/// see fmt.paste_uint to remove suffix before pasting.
///
/// cast from signed reinterprets bits as unsigned.
///
/// values above the int max must have a 'u' suffix; implicit interpretation
/// as unsigned is not allowed (e.g. 4095 is not a valid integer).
///
/// PTL_UINT(0)               // 0u
/// PTL_UINT(2, UHEX)         // 0x002u
/// PTL_UINT(0x007)           // 0x007u
/// PTL_UINT(0x00F, UDEC)     // 15u
/// PTL_UINT(8u)              // 8u
/// PTL_UINT(6u, UHEX)        // 0x006u
/// PTL_UINT(0x005u)          // 0x005u
/// PTL_UINT(0x004u, UDEC)    // 4u
/// PTL_UINT((0, 0, 0))       // 0x000u
/// PTL_UINT((F, F, F), UDEC) // 4095u
#define PTL_UINT(/* word, hint=AUTO: enum<UDEC|UHEX|AUTO> */...) /* -> uint */ \
  PPUTLUINT_o(PTL_STR([PTL_UINT] invalid arguments : __VA_ARGS__), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLUINT_o(e, v, ...)                                                        \
  PTL_XCAT(PPUTLUINT_,                                                                \
           PPUTLUINT_MODE(e, PTL_TYPEOF(v),                                           \
                          PTL_ENUM(PPUTLUINT_HINT_, PTL_DEFAULT(AUTO, __VA_ARGS__)))) \
  (v)
#define PPUTLUINT_XW_UH(utup)      PPUTLUINT_XW_UH_o utup
#define PPUTLUINT_XW_UH_o(a, b, c) 0x##a##b##c##u
#define PPUTLUINT_XW_UD(utup)      PPUTLUINT_XW_UD_o utup
#define PPUTLUINT_XW_UD_o(a, b, c) PPUTLIMPL_UHEX(0x##a##b##c##u, UDEC)
#define PPUTLUINT_UH_UH(uhex)      uhex
#define PPUTLUINT_UH_UD(uhex)      PPUTLIMPL_UHEX(uhex, UDEC)
#define PPUTLUINT_UD_UH(udec)      PPUTLIMPL_UDEC(udec, UHEX)
#define PPUTLUINT_UD_UD(udec)      udec
#define PPUTLUINT_IH_UH(ihex)      ihex##u
#define PPUTLUINT_IH_UD(ihex)      PPUTLIMPL_UHEX(ihex##u, UDEC)
#define PPUTLUINT_ID_UH(idec)      PPUTLIMPL_UDEC(idec##u, UHEX)
#define PPUTLUINT_ID_UD(idec)      idec##u

/// cast mode selector and error detector
#define PPUTLUINT_MODE(e, t, hint) /* -> <cast mode> */ \
  PTL_XCAT(PPUTLUINT_MODE_, PTL_IS_NONE(PTL_XCAT(PPUTLUINT_MODE_, t)))(e, t, hint)
#define PPUTLUINT_MODE_1(e, t, hint) PPUTLUINT_MODE_1_##t##hint
#define PPUTLUINT_MODE_1_UTUPAUTO    XW_UH
#define PPUTLUINT_MODE_1_UTUPUHEX    XW_UH
#define PPUTLUINT_MODE_1_UTUPUDEC    XW_UD
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
#define PPUTLUINT_MODE_UTUP
#define PPUTLUINT_MODE_UHEX
#define PPUTLUINT_MODE_UDEC
#define PPUTLUINT_MODE_IHEX
#define PPUTLUINT_MODE_IDEC
#define PPUTLUINT_HINT_AUTO
#define PPUTLUINT_HINT_UHEX
#define PPUTLUINT_HINT_UDEC

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
#define PTL_UDEC(/* word */...) /* -> udec */ PPUTLUDEC_o(PTL_UINT(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLUDEC_o(uint) PTL_XCAT(PPUTLUDEC_, PPUTLIS_UDEC_o(uint))(uint)
#define PPUTLUDEC_1(udec) udec
#define PPUTLUDEC_0(uhex) PPUTLIMPL_UHEX(uhex, UDEC)

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
#define PTL_UHEX(/* word */...) /* -> uhex */ PPUTLUHEX_o(PTL_UINT(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLUHEX_o(uint) PTL_XCAT(PPUTLUHEX_, PPUTLIS_UHEX_o(uint))(uint)
#define PPUTLUHEX_1(uhex) uhex
#define PPUTLUHEX_0(udec) PPUTLIMPL_UDEC(udec, UHEX)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.tup]
/// ----------
/// [inherits from PTL_OBJ] tuple type (parenthesized list).
/// expands to t if valid, else fails.
///
/// PTL_TUP(())     // ()
/// PTL_TUP((1, 2)) // (1, 2)
#define PTL_TUP(/* tup */...) /* -> tup */                          \
  PPUTLTUP_o(PTL_STR([PTL_TUP] tuple must be wrapped in parentheses \
                     : __VA_ARGS__),                                \
             PTL_OBJ(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLTUP_o(e, obj) PTL_XCAT(PPUTLTUP_, PPUTLIS_TUP_o(obj))(e, obj)
#define PPUTLTUP_1(e, tup) tup
#define PPUTLTUP_0(e, ...) PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.utup]
/// -----------
/// [inherits from PTL_TUP] a tuple of exactly PTL_WORD_SIZE (3) hex digits.
/// constructibe from any word.
///
/// PTL_UTUP(0)         // (0, 0, 0)
/// PTL_UTUP(4095u)     // (F, F, F)
/// PTL_UTUP(0x800)     // (8, 0, 0)
/// PTL_UTUP(2047)      // (7, F, F)
/// PTL_UTUP((1, 0, 0)) // (1, 0, 0)
#define PTL_UTUP(/* word */...) /* -> utup */            \
  PPUTLUTUP_o(PTL_STR([PTL_UTUP] invalid integer or word \
                      : __VA_ARGS__),                    \
              PTL_OBJ(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLUTUP_o(e, obj) PTL_XCAT(PPUTLUTUP_, PPUTLIS_UTUP_o(obj))(e, obj)
#define PPUTLUTUP_1(e, ...) __VA_ARGS__
#define PPUTLUTUP_0(e, ...) \
  PTL_XCAT(PPUTLUTUP_0, PTL_IS_ATOM(__VA_ARGS__))(e, __VA_ARGS__)
#define PPUTLUTUP_01(e, atom)   PTL_XCAT(PPUTLUTUP_01, PPUTLIS_INT_o(atom))(e, atom)
#define PPUTLUTUP_011(e, int)   PPUTLIMPL_UHEX(PTL_UHEX(int), UTUP)
#define PPUTLUTUP_010(e, atom)  PTL_XCAT(PPUTLUTUP_010, PPUTLIS_UINT_o(atom))(e, atom)
#define PPUTLUTUP_0101(e, uint) PPUTLIMPL_UHEX(PTL_UHEX(uint), UTUP)
#define PPUTLUTUP_0100(e, atom) PTL_FAIL(e)
#define PPUTLUTUP_00(e, ...)    PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.word]
/// -----------
/// [inherits from PTL_OBJ] a union of int|uint|utup.
/// constructibe from any word.
///
/// cannot parse negative decimals; use math.neg instead.
/// hex length is fixed. cannot parse shorter hex lengths.
///
/// cast modes:
///
///   idec → idec | [default]
///   idec → ihex | requires IHEX hint
///   idec → udec | requires UDEC hint
///   idec → uhex | requires UHEX hint
///   idec → utup | requires UTUP hint
///
///   ihex → ihex | [default]; failback for failed ihex → idec
///   ihex → idec | requires IDEC hint and positive result
///   ihex → udec | requires UDEC hint
///   ihex → uhex | requires UHEX hint
///   ihex → utup | requires UTUP hint
///
///   udec → udec | [default]
///   udec → idec | requires IDEC hint and positive result
///   udec → ihex | requires IHEX hint or udec → idec failure
///   udec → uhex | requires UHEX hint
///   udec → utup | requires UTUP hint
///
///   uhex → uhex | [default]
///   uhex → idec | requires IDEC hint and positive result
///   uhex → ihex | requires IHEX hint or uhex → idec failure
///   uhex → udec | requires UDEC hint
///   uhex → utup | requires UTUP hint
///
///   utup → utup | [default]
///   utup → idec | requires IDEC hint and positive result
///   utup → ihex | requires IHEX hint or utup → idec failure
///   utup → udec | requires UDEC hint
///   utup → uhex | requires UHEX hint
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
/// PTL_WORD(0)               // 0
/// PTL_WORD(1, IHEX)         // 0x001
/// PTL_WORD(2, UDEC)         // 2u
/// PTL_WORD(3, UHEX)         // 0x003u
/// PTL_WORD(4, UTUP)         // (0, 0, 4)
/// PTL_WORD(0x002)           // 0x002
/// PTL_WORD(0x800, IDEC)     // 0x800
/// PTL_WORD(0x002, IDEC)     // 2
/// PTL_WORD(0x00F, UDEC)     // 15u
/// PTL_WORD(0x007, UHEX)     // 0x007u
/// PTL_WORD(0x008, UTUP)     // (0, 0, 8)
/// PTL_WORD(8u)              // 8u
/// PTL_WORD(7u, IDEC)        // 7
/// PTL_WORD(15u, IHEX)       // 0x00F
/// PTL_WORD(4095u, IDEC)     // 0xFFF
/// PTL_WORD(6u, UHEX)        // 0x006u
/// PTL_WORD(4u, UTUP)        // (0, 0, 4)
/// PTL_WORD(0x005u)          // 0x005u
/// PTL_WORD(0x005u, IDEC)    // 5
/// PTL_WORD(0x007u, IHEX)    // 0x007
/// PTL_WORD(0xFFFu, IDEC)    // 0xFFF
/// PTL_WORD(0x004u, UDEC)    // 4u
/// PTL_WORD(0x00Fu, UTUP)    // (0, 0, F)
/// PTL_WORD((0, 0, 3))       // (0, 0, 3)
/// PTL_WORD((7, F, F), IDEC) // 2047
/// PTL_WORD((0, 0, 0), IHEX) // 0x000
/// PTL_WORD((8, 0, 0), IDEC) // 0x800
/// PTL_WORD((F, F, F), UDEC) // 4095u
/// PTL_UINT((0, 0, 0), UHEX) // 0x000u
#define PTL_WORD(                                                                \
    /* word, hint=AUTO: enum<UTUP|IDEC|IHEX|UDEC|UHEX|AUTO> */...) /* -> word */ \
  PPUTLWORD_o(PTL_STR([PTL_WORD] invalid arguments : __VA_ARGS__), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLWORD_o(e, v, ...)                                                        \
  PTL_XCAT(PPUTLWORD_,                                                                \
           PPUTLWORD_MODE(e, PTL_TYPEOF(v),                                           \
                          PTL_ENUM(PPUTLWORD_HINT_, PTL_DEFAULT(AUTO, __VA_ARGS__)))) \
  (v)
#define PPUTLWORD_UTUP(word) PTL_UTUP(word)
#define PPUTLWORD_UHEX(word) PTL_UINT(word, UHEX)
#define PPUTLWORD_UDEC(word) PTL_UINT(word, UDEC)
#define PPUTLWORD_IHEX(word) PTL_INT(word, IHEX)
#define PPUTLWORD_IDEC(word) PTL_INT(word, IDEC)

/// cast mode selector and error detector
#define PPUTLWORD_MODE(e, t, hint) \
  PTL_XCAT(PPUTLWORD_MODE_, PTL_IS_NONE(PTL_XCAT(PPUTLWORD_MODE_, t)))(e, t, hint)
#define PPUTLWORD_MODE_1(e, t, hint) PPUTLWORD_MODE_1_##hint(t)
#define PPUTLWORD_MODE_1_AUTO(t)     t
#define PPUTLWORD_MODE_1_UTUP(t)     UTUP
#define PPUTLWORD_MODE_1_UHEX(t)     UHEX
#define PPUTLWORD_MODE_1_UDEC(t)     UDEC
#define PPUTLWORD_MODE_1_IHEX(t)     IHEX
#define PPUTLWORD_MODE_1_IDEC(t)     IDEC
#define PPUTLWORD_MODE_0(e, t, hint) PTL_FAIL(e)
#define PPUTLWORD_MODE_UTUP
#define PPUTLWORD_MODE_UHEX
#define PPUTLWORD_MODE_UDEC
#define PPUTLWORD_MODE_IHEX
#define PPUTLWORD_MODE_IDEC
#define PPUTLWORD_HINT_AUTO
#define PPUTLWORD_HINT_UTUP
#define PPUTLWORD_HINT_UHEX
#define PPUTLWORD_HINT_UDEC
#define PPUTLWORD_HINT_IHEX
#define PPUTLWORD_HINT_IDEC

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.size]
/// -----------
/// [inherits from PTL_WORD] a word within 0 and PTL_SIZE_MAX (255u).
/// constructibe from any word.
///
/// cannot parse negative decimals; use math.neg instead.
/// hex length is fixed. cannot parse shorter hex lengths.
///
/// see type.word for available cast modes.
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
/// PTL_SIZE(0)     // 0
/// PTL_SIZE(1)     // 1
/// PTL_SIZE(0x007) // 0x007
/// PTL_SIZE(255u)  // 255u
#define PTL_SIZE(                                                                      \
    /* word, hint=AUTO: enum<UTUP|IDEC|IHEX|UDEC|UHEX|AUTO> */...) /* -> size */       \
  PPUTLSIZE_o(PTL_STR([PTL_SIZE] invalid size; must be within 0 and PTL_SIZE_MAX(255u) \
                      : __VA_ARGS__),                                                  \
              PTL_WORD(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLSIZE_o(e, w)   PTL_XCAT(PPUTLSIZE_o_, PPUTLIS_SIZE_o(w))(e, w)
#define PPUTLSIZE_o_1(e, w) w
#define PPUTLSIZE_o_0(e, w) PTL_FAIL(e)

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

/// [compare.lt]
/// ------------
/// word less-than comparison.
/// prohibits comparison of different signedness.
/// utups are interpreted as (and are comparable with) unsigned.
///
/// PTL_LT(0, 0)                  // 0
/// PTL_LT(0, 1)                  // 1
/// PTL_LT(7u, 8u)                // 1
/// PTL_LT(PTL_INT(4095u), 0)     // 1
/// PTL_LT(2047, 0x800)           // 0
/// PTL_LT(0x800, PTL_INT(2048u)) // 0
/// PTL_LT(0x800, PTL_INT(2049u)) // 1
/// PTL_LT((F, F, F), (0, 0, 0))  // 0
/// PTL_LT((0, 0, 0), (F, F, F))  // 1
/// PTL_LT((7, F, F), 2048u)      // 1
/// PTL_LT(2048u, (7, F, F))      // 0
#define PTL_LT(/* word, word */...) /* -> bool */                                        \
  PPUTLLT_o(__VA_ARGS__)(PTL_STR([PTL_LT] comparison of different signedness not allowed \
                                 : __VA_ARGS__),                                         \
                         __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLLT_o(l, r) \
  PTL_XCAT(PPUTLLT_o_, PTL_XCAT(PPUTLLT_SIGNOF(PTL_WORD(l)), PPUTLLT_SIGNOF(PTL_WORD(r))))
#define PPUTLLT_o_UU(e, l, r) PPUTLLT_UCMP(PTL_ESC PTL_UTUP(l), PTL_ESC PTL_UTUP(r))
#define PPUTLLT_o_UI(e, l, r) PTL_FAIL(e)
#define PPUTLLT_o_IU(e, l, r) PTL_FAIL(e)
#define PPUTLLT_o_II(e, l, r) PPUTLLT_ICMP(PTL_ESC PTL_UTUP(l), PTL_ESC PTL_UTUP(r))
#define PPUTLLT_SIGNOF(word)  PTL_XCAT(PPUTLLT_SIGNOF_, PPUTLIS_TUP_o(word))(word)
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
#define PPUTLLT_ICMP_oo_11(...) PPUTLLT_UCMP(__VA_ARGS__)
#define PPUTLLT_ICMP_oo_10(...) 1
#define PPUTLLT_ICMP_oo_01(...) 0
#define PPUTLLT_ICMP_oo_00(...) PPUTLLT_UCMP(__VA_ARGS__)
#define PPUTLLT_UCMP(...) \
  PTL_XFIRST(PPUTLLT_R(PPUTLLT_R(PPUTLLT_R(0, 0, PPUTLLT_ZIP(__VA_ARGS__)))))
#define PPUTLLT_ZIP(...)                PPUTLLT_ZIP_o(__VA_ARGS__)
#define PPUTLLT_ZIP_o(a, b, c, d, e, f) a, d, b, e, c, f
#define PPUTLLT_R(...)                  PPUTLLT_R_o(__VA_ARGS__)
#define PPUTLLT_R_o(fl, fg, a, b, ...)                    \
  PTL_XCAT(PPUTLLT_##fl##fg, PPUTLIMPL_HEXHEX(a##b, LT)), \
      PTL_XCAT(PPUTLLT_##fg##fl, PPUTLIMPL_HEXHEX(b##a, LT)), __VA_ARGS__
#define PPUTLLT_111 1
#define PPUTLLT_110 1
#define PPUTLLT_101 1
#define PPUTLLT_100 1
#define PPUTLLT_011 0
#define PPUTLLT_010 0
#define PPUTLLT_001 1
#define PPUTLLT_000 0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [compare.gt]
/// ------------
/// word greater-than comparison.
/// prohibits comparison of different signedness.
/// utups are interpreted as (and are comparable with) unsigned.
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
/// word less-than-or-equal-to comparison.
/// prohibits comparison of different signedness.
/// utups are interpreted as (and are comparable with) unsigned.
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
/// word greater-than-or-equal-to comparison.
/// prohibits comparison of different signedness.
/// utups are interpreted as (and are comparable with) unsigned.
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
/// word equal-to comparison.
/// prohibits comparison of different signedness.
/// utups are interpreted as (and are comparable with) unsigned.
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
/// word not-equal-to comparison.
/// prohibits comparison of different signedness.
/// utups are interpreted as (and are comparable with) unsigned.
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
/// word minimum operation.
/// prohibits comparison of different signedness.
/// utups are interpreted as (and are comparable with) unsigned.
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
/// word maximum operation.
/// prohibits comparison of different signedness.
/// utups are interpreted as (and are comparable with) unsigned.
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

/// [numeric.inc]
/// -------------
/// numerical increment w/ overflow.
///
/// PTL_INC(0)     // 1
/// PTL_INC(1u)    // 2u
/// PTL_INC(2047)  // 0x800
/// PTL_INC(0x7FF) // 0x800
/// PTL_INC(15u)   // 16u
/// PTL_INC(4095u) // 0u
#define PTL_INC(/* word */...) /* -> word */ \
  PPUTLINC_RES(PTL_TYPEOF(__VA_ARGS__),      \
               PPUTLINC_o(PPUTLINC_X(PPUTLINC_INIT PTL_UTUP(__VA_ARGS__))))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLINC_o(...) PPUTLINC_R1(PPUTLINC_R0(PPUTLINC_R0(__VA_ARGS__)))

#define PPUTLINC_RES(...)                         PPUTLINC_RES_o(__VA_ARGS__)
#define PPUTLINC_RES_o(_hint, a, b, _carry, _inc) PTL_WORD((a, b, _inc), _hint)
#define PPUTLINC_R1(...)                          PPUTLINC_R1_o(__VA_ARGS__)
#define PPUTLINC_R1_o(a, b, _carry, _inc)         _inc, a, 0, b
#define PPUTLINC_R0(...)                          PPUTLINC_R0_o(__VA_ARGS__)
#define PPUTLINC_R0_o(a, b, _carry, _inc)         _inc, a, PPUTLINC_R0_o_##_carry(b)
#define PPUTLINC_R0_o_1(b)                        PPUTLINC_X(PTL_ESC PPUTLIMPL_HEX(b, INC))
#define PPUTLINC_R0_o_0(b)                        0, b
#define PPUTLINC_INIT(a, b, c)                    a, b, PTL_ESC PPUTLIMPL_HEX(c, INC)
#define PPUTLINC_X(...)                           __VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [numeric.dec]
/// -------------
/// numerical decrement w/ underflow.
///
/// PTL_DEC(1)     // 0
/// PTL_DEC(2u)    // 1u
/// PTL_DEC(0)     // 0xFFF
/// PTL_DEC(0x800) // 0x7FF
/// PTL_DEC(16u)   // 15u
/// PTL_DEC(0u)    // 4095u
#define PTL_DEC(/* word */...) /* -> word */ \
  PPUTLDEC_RES(PTL_TYPEOF(__VA_ARGS__),      \
               PPUTLDEC_o(PPUTLDEC_X(PPUTLDEC_INIT PTL_UTUP(__VA_ARGS__))))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLDEC_o(...) PPUTLDEC_R1(PPUTLDEC_R0(PPUTLDEC_R0(__VA_ARGS__)))

#define PPUTLDEC_RES(...)                         PPUTLDEC_RES_o(__VA_ARGS__)
#define PPUTLDEC_RES_o(_hint, a, b, _carry, _dec) PTL_WORD((a, b, _dec), _hint)
#define PPUTLDEC_R1(...)                          PPUTLDEC_R1_o(__VA_ARGS__)
#define PPUTLDEC_R1_o(a, b, _carry, _dec)         _dec, a, 0, b
#define PPUTLDEC_R0(...)                          PPUTLDEC_R0_o(__VA_ARGS__)
#define PPUTLDEC_R0_o(a, b, _carry, _dec)         _dec, a, PPUTLDEC_R0_o_##_carry(b)
#define PPUTLDEC_R0_o_1(b)                        PPUTLDEC_X(PTL_ESC PPUTLIMPL_HEX(b, DEC))
#define PPUTLDEC_R0_o_0(b)                        0, b
#define PPUTLDEC_INIT(a, b, c)                    a, b, PTL_ESC PPUTLIMPL_HEX(c, DEC)
#define PPUTLDEC_X(...)                           __VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [numeric.eqz]
/// -------------
/// numerical zero detection.
///
/// PTL_EQZ(0)         // 1
/// PTL_EQZ(0u)        // 1
/// PTL_EQZ(0x000)     // 1
/// PTL_EQZ(0x000u)    // 1
/// PTL_EQZ((0, 0, 0)) // 1
/// PTL_EQZ(1u)        // 0
/// PTL_EQZ(2)         // 0
/// PTL_EQZ(4095u)     // 0
/// PTL_EQZ(0x800)     // 0
#define PTL_EQZ(/* word */...) /* -> bool */ \
  PTL_IS_NONE(PTL_XCAT(PPUTLEQZ_, PTL_UDEC(__VA_ARGS__)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLEQZ_0u

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [numeric.nez]
/// -------------
/// numerical non-zero detection.
///
/// PTL_NEZ(0)         // 0
/// PTL_NEZ(0u)        // 0
/// PTL_NEZ(0x000)     // 0
/// PTL_NEZ(0x000u)    // 0
/// PTL_NEZ((0, 0, 0)) // 0
/// PTL_NEZ(1u)        // 1
/// PTL_NEZ(2)         // 1
/// PTL_NEZ(4095u)     // 1
/// PTL_NEZ(0x800)     // 1
#define PTL_NEZ(/* word */...) /* -> bool */ \
  PTL_NOT(PTL_IS_NONE(PTL_XCAT(PPUTLEQZ_, PTL_UDEC(__VA_ARGS__))))

/// [numeric.ltz]
/// -------------
/// signed integral less-than-zero detection.
///
/// PTL_LTZ(0)             // 0
/// PTL_LTZ(1)             // 0
/// PTL_LTZ(2047)          // 0
/// PTL_LTZ(0x800)         // 1
/// PTL_LTZ(PTL_INC(2047)) // 1
#define PTL_LTZ(/* int */...) /* -> bool */ \
  PPUTLLTZ_o(PPUTLLTZ_RES PTL_UTUP(PTL_INT(__VA_ARGS__)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLLTZ_o(...)       __VA_ARGS__
#define PPUTLLTZ_RES(a, b, c) PPUTLIMPL_HEXHEX(7##a, LT)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [numeric.gtz]
/// -------------
/// signed integral greater-than-zero detection.
///
/// PTL_GTZ(0)             // 0
/// PTL_GTZ(1)             // 1
/// PTL_GTZ(2047)          // 1
/// PTL_GTZ(0x800)         // 0
/// PTL_GTZ(PTL_INC(2047)) // 0
#define PTL_GTZ(/* int */...) /* -> bool */ \
  PTL_NOR(PTL_LTZ(__VA_ARGS__), PTL_EQZ(__VA_ARGS__))

/// [numeric.lez]
/// -------------
/// signed integral greater-than-or-equal-to-zero detection.
///
/// PTL_LEZ(0)             // 1
/// PTL_LEZ(1)             // 0
/// PTL_LEZ(2047)          // 0
/// PTL_LEZ(0x800)         // 1
/// PTL_LEZ(PTL_INC(2047)) // 1
#define PTL_LEZ(/* int */...) /* -> bool */ \
  PTL_OR(PTL_LTZ(__VA_ARGS__), PTL_EQZ(__VA_ARGS__))

/// [numeric.gez]
/// -------------
/// signed integral greater-than-or-equal-to-zero detection.
///
/// PTL_GEZ(0)             // 1
/// PTL_GEZ(1)             // 1
/// PTL_GEZ(2047)          // 1
/// PTL_GEZ(0x800)         // 0
/// PTL_GEZ(PTL_INC(2047)) // 0
#define PTL_GEZ(/* int */...) /* -> bool */ PTL_NOT(PTL_LTZ(__VA_ARGS__))

/// [bitwise.bdump]
/// ---------------
/// dumps the bits of a word.
/// returns exactly PTL_BIT_LENGTH (12) bools.
///
/// PTL_BDUMP(0)     // 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
/// PTL_BDUMP(0x800) // 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
#define PTL_BDUMP(/* word */...) /* -> bool... */ \
  PPUTLBDUMP_o(PPUTLBDUMP_BITS PTL_UTUP(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBDUMP_o(...) __VA_ARGS__
#define PPUTLBDUMP_BITS(a, b, c) \
  PPUTLIMPL_HEX(a, BITS), PPUTLIMPL_HEX(b, BITS), PPUTLIMPL_HEX(c, BITS)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [bitwise.bsll]
/// --------------
/// performs a logical bitwise left shift by n places.
///
/// PTL_BSLL(0, 1)     // 0
/// PTL_BSLL(1u, 1)    // 2u
/// PTL_BSLL(0x002, 2) // 0x008
/// PTL_BSLL(0x002, 3) // 0x010
/// PTL_BSLL(4095u, 3) // 4088u
/// PTL_BSLL(1, 10)    // 1024
/// PTL_BSLL(1, 11)    // 0x800
/// PTL_BSLL(1, 12)    // 0
/// PTL_BSLL(1, 13)    // 0
#define PTL_BSLL(/* word, n=1: idec */...) /* -> word */ PPUTLBSLL_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBSLL_o(v, ...)                                                   \
  PTL_WORD(PPUTLBSLL_oo(PTL_IDEC(PTL_DEFAULT(1, __VA_ARGS__)), PTL_BDUMP(v)), \
           PTL_TYPEOF(v))
#define PPUTLBSLL_oo(i, ...)         PPUTLBSLL_ooo(i, PTL_LT(i, 12), __VA_ARGS__)
#define PPUTLBSLL_ooo(...)           PPUTLBSLL_oooo(__VA_ARGS__)
#define PPUTLBSLL_oooo(i, gelt, ...) PPUTLBSLL_oooo_##gelt(i, __VA_ARGS__)
#define PPUTLBSLL_oooo_1(i, ...)     PPUTLBSLL_##i(__VA_ARGS__)
#define PPUTLBSLL_oooo_0(...)        0

/// bit shifts
#define PPUTLBSLL_11(a, b, c, d, e, f, g, h, i, j, k, l) \
  (PPUTLIMPL_NYBL(l##000, HEX), 0, 0)
#define PPUTLBSLL_10(a, b, c, d, e, f, g, h, i, j, k, l) \
  (PPUTLIMPL_NYBL(k##l##00, HEX), 0, 0)
#define PPUTLBSLL_9(a, b, c, d, e, f, g, h, i, j, k, l) \
  (PPUTLIMPL_NYBL(j##k##l##0, HEX), 0, 0)
#define PPUTLBSLL_8(a, b, c, d, e, f, g, h, i, j, k, l) \
  (PPUTLIMPL_NYBL(i##j##k##l, HEX), 0, 0)
#define PPUTLBSLL_7(a, b, c, d, e, f, g, h, i, j, k, l) \
  (PPUTLIMPL_NYBL(h##i##j##k, HEX), PPUTLIMPL_NYBL(l##000, HEX), 0)
#define PPUTLBSLL_6(a, b, c, d, e, f, g, h, i, j, k, l) \
  (PPUTLIMPL_NYBL(g##h##i##j, HEX), PPUTLIMPL_NYBL(k##l##00, HEX), 0)
#define PPUTLBSLL_5(a, b, c, d, e, f, g, h, i, j, k, l) \
  (PPUTLIMPL_NYBL(f##g##h##i, HEX), PPUTLIMPL_NYBL(j##k##l##0, HEX), 0)
#define PPUTLBSLL_4(a, b, c, d, e, f, g, h, i, j, k, l) \
  (PPUTLIMPL_NYBL(e##f##g##h, HEX), PPUTLIMPL_NYBL(i##j##k##l, HEX), 0)
#define PPUTLBSLL_3(a, b, c, d, e, f, g, h, i, j, k, l)              \
  (PPUTLIMPL_NYBL(d##e##f##g, HEX), PPUTLIMPL_NYBL(h##i##j##k, HEX), \
   PPUTLIMPL_NYBL(l##000, HEX))
#define PPUTLBSLL_2(a, b, c, d, e, f, g, h, i, j, k, l)              \
  (PPUTLIMPL_NYBL(c##d##e##f, HEX), PPUTLIMPL_NYBL(g##h##i##j, HEX), \
   PPUTLIMPL_NYBL(k##l##00, HEX))
#define PPUTLBSLL_1(a, b, c, d, e, f, g, h, i, j, k, l)              \
  (PPUTLIMPL_NYBL(b##c##d##e, HEX), PPUTLIMPL_NYBL(f##g##h##i, HEX), \
   PPUTLIMPL_NYBL(j##k##l##0, HEX))
#define PPUTLBSLL_0(a, b, c, d, e, f, g, h, i, j, k, l)              \
  (PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
   PPUTLIMPL_NYBL(i##j##k##l, HEX))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [bitwise.bsrl]
/// --------------
/// performs a logical bitwise right shift by n places.
///
/// PTL_BSRL(0, 1)      // 0
/// PTL_BSRL(2, 1)      // 1
/// PTL_BSRL(4, 1)      // 2
/// PTL_BSRL(4, 2)      // 1
/// PTL_BSRL(0x800, 11) // 0x001
/// PTL_BSRL(0x800, 12) // 0x000
#define PTL_BSRL(/* word, n=1: idec */...) /* -> word */ PPUTLBSRL_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBSRL_o(v, ...)                                                   \
  PTL_WORD(PPUTLBSRL_oo(PTL_IDEC(PTL_DEFAULT(1, __VA_ARGS__)), PTL_BDUMP(v)), \
           PTL_TYPEOF(v))
#define PPUTLBSRL_oo(i, ...)         PPUTLBSRL_ooo(i, PTL_LT(i, 12), __VA_ARGS__)
#define PPUTLBSRL_ooo(...)           PPUTLBSRL_oooo(__VA_ARGS__)
#define PPUTLBSRL_oooo(i, gelt, ...) PPUTLBSRL_oooo_##gelt(i, __VA_ARGS__)
#define PPUTLBSRL_oooo_1(i, ...)     PPUTLBSRL_##i(__VA_ARGS__)
#define PPUTLBSRL_oooo_0(...)        0

/// bit shifts
#define PPUTLBSRL_11(a, b, c, d, e, f, g, h, i, j, k, l) \
  (0, 0, PPUTLIMPL_NYBL(000##a, HEX))
#define PPUTLBSRL_10(a, b, c, d, e, f, g, h, i, j, k, l) \
  (0, 0, PPUTLIMPL_NYBL(00##a##b, HEX))
#define PPUTLBSRL_9(a, b, c, d, e, f, g, h, i, j, k, l) \
  (0, 0, PPUTLIMPL_NYBL(0##a##b##c, HEX))
#define PPUTLBSRL_8(a, b, c, d, e, f, g, h, i, j, k, l) \
  (0, 0, PPUTLIMPL_NYBL(a##b##c##d, HEX))
#define PPUTLBSRL_7(a, b, c, d, e, f, g, h, i, j, k, l) \
  (0, PPUTLIMPL_NYBL(000##a, HEX), PPUTLIMPL_NYBL(b##c##d##e, HEX))
#define PPUTLBSRL_6(a, b, c, d, e, f, g, h, i, j, k, l) \
  (0, PPUTLIMPL_NYBL(00##a##b, HEX), PPUTLIMPL_NYBL(c##d##e##f, HEX))
#define PPUTLBSRL_5(a, b, c, d, e, f, g, h, i, j, k, l) \
  (0, PPUTLIMPL_NYBL(0##a##b##c, HEX), PPUTLIMPL_NYBL(d##e##f##g, HEX))
#define PPUTLBSRL_4(a, b, c, d, e, f, g, h, i, j, k, l) \
  (0, PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX))
#define PPUTLBSRL_3(a, b, c, d, e, f, g, h, i, j, k, l)          \
  (PPUTLIMPL_NYBL(000##a, HEX), PPUTLIMPL_NYBL(b##c##d##e, HEX), \
   PPUTLIMPL_NYBL(f##g##h##i, HEX))
#define PPUTLBSRL_2(a, b, c, d, e, f, g, h, i, j, k, l)            \
  (PPUTLIMPL_NYBL(00##a##b, HEX), PPUTLIMPL_NYBL(c##d##e##f, HEX), \
   PPUTLIMPL_NYBL(g##h##i##j, HEX))
#define PPUTLBSRL_1(a, b, c, d, e, f, g, h, i, j, k, l)              \
  (PPUTLIMPL_NYBL(0##a##b##c, HEX), PPUTLIMPL_NYBL(d##e##f##g, HEX), \
   PPUTLIMPL_NYBL(h##i##j##k, HEX))
#define PPUTLBSRL_0(a, b, c, d, e, f, g, h, i, j, k, l)              \
  (PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
   PPUTLIMPL_NYBL(i##j##k##l, HEX))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [bitwise.bsra]
/// --------------
/// performs an arithmetic bitwise right shift by n places.
///
/// PTL_BSRA(0, 1)     // 0
/// PTL_BSRA(2, 1)     // 1
/// PTL_BSRA(0x800, 1) // 0xC00
/// PTL_BSRA(0x800, 2) // 0xE00
/// PTL_BSRA(0x800, 3) // 0xF00
/// PTL_BSRA(0x800, 4) // 0xF80
#define PTL_BSRA(/* word, n=1: idec */...) /* -> word */ PPUTLBSRA_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBSRA_o(v, ...)                                                             \
  PTL_WORD(PPUTLBSRA_oo(PTL_IDEC(PTL_DEFAULT(1, __VA_ARGS__)), PTL_BDUMP(PTL_WORD(v))), \
           PTL_TYPEOF(v))
#define PPUTLBSRA_oo(i, ...)         PPUTLBSRA_ooo(i, PTL_LT(i, 12), __VA_ARGS__)
#define PPUTLBSRA_ooo(...)           PPUTLBSRA_oooo(__VA_ARGS__)
#define PPUTLBSRA_oooo(i, gelt, ...) PPUTLBSRA_oooo_##gelt(i, __VA_ARGS__)
#define PPUTLBSRA_oooo_1(i, ...)     PPUTLBSRA_##i(__VA_ARGS__)
#define PPUTLBSRA_oooo_0(...)        0

/// bit shifts
#define PPUTLBSRA_11(a, b, c, d, e, f, g, h, i, j, k, l)             \
  (PPUTLIMPL_NYBL(a##a##a##a, HEX), PPUTLIMPL_NYBL(a##a##a##a, HEX), \
   PPUTLIMPL_NYBL(a##a##a##a, HEX))
#define PPUTLBSRA_10(a, b, c, d, e, f, g, h, i, j, k, l)             \
  (PPUTLIMPL_NYBL(a##a##a##a, HEX), PPUTLIMPL_NYBL(a##a##a##a, HEX), \
   PPUTLIMPL_NYBL(a##a##a##b, HEX))
#define PPUTLBSRA_9(a, b, c, d, e, f, g, h, i, j, k, l)              \
  (PPUTLIMPL_NYBL(a##a##a##a, HEX), PPUTLIMPL_NYBL(a##a##a##a, HEX), \
   PPUTLIMPL_NYBL(a##a##b##c, HEX))
#define PPUTLBSRA_8(a, b, c, d, e, f, g, h, i, j, k, l)              \
  (PPUTLIMPL_NYBL(a##a##a##a, HEX), PPUTLIMPL_NYBL(a##a##a##a, HEX), \
   PPUTLIMPL_NYBL(a##b##c##d, HEX))
#define PPUTLBSRA_7(a, b, c, d, e, f, g, h, i, j, k, l)              \
  (PPUTLIMPL_NYBL(a##a##a##a, HEX), PPUTLIMPL_NYBL(a##a##a##a, HEX), \
   PPUTLIMPL_NYBL(b##c##d##e, HEX))
#define PPUTLBSRA_6(a, b, c, d, e, f, g, h, i, j, k, l)              \
  (PPUTLIMPL_NYBL(a##a##a##a, HEX), PPUTLIMPL_NYBL(a##a##a##b, HEX), \
   PPUTLIMPL_NYBL(c##d##e##f, HEX))
#define PPUTLBSRA_5(a, b, c, d, e, f, g, h, i, j, k, l)              \
  (PPUTLIMPL_NYBL(a##a##a##a, HEX), PPUTLIMPL_NYBL(a##a##b##c, HEX), \
   PPUTLIMPL_NYBL(d##e##f##g, HEX))
#define PPUTLBSRA_4(a, b, c, d, e, f, g, h, i, j, k, l)              \
  (PPUTLIMPL_NYBL(a##a##a##a, HEX), PPUTLIMPL_NYBL(a##b##c##d, HEX), \
   PPUTLIMPL_NYBL(e##f##g##h, HEX))
#define PPUTLBSRA_3(a, b, c, d, e, f, g, h, i, j, k, l)              \
  (PPUTLIMPL_NYBL(a##a##a##a, HEX), PPUTLIMPL_NYBL(b##c##d##e, HEX), \
   PPUTLIMPL_NYBL(f##g##h##i, HEX))
#define PPUTLBSRA_2(a, b, c, d, e, f, g, h, i, j, k, l)              \
  (PPUTLIMPL_NYBL(a##a##a##b, HEX), PPUTLIMPL_NYBL(c##d##e##f, HEX), \
   PPUTLIMPL_NYBL(g##h##i##j, HEX))
#define PPUTLBSRA_1(a, b, c, d, e, f, g, h, i, j, k, l)              \
  (PPUTLIMPL_NYBL(a##a##b##c, HEX), PPUTLIMPL_NYBL(d##e##f##g, HEX), \
   PPUTLIMPL_NYBL(h##i##j##k, HEX))
#define PPUTLBSRA_0(a, b, c, d, e, f, g, h, i, j, k, l)              \
  (PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
   PPUTLIMPL_NYBL(i##j##k##l, HEX))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [bitwise.bnot]
/// --------------
/// bitwise NOT.
///
/// PTL_BNOT(0u)        // 4095u
/// PTL_BNOT(0)         // 0xFFF
/// PTL_BNOT((7, F, F)) // (8, 0, 0)
/// PTL_BNOT((7, F, F)) // (8, 0, 0)
#define PTL_BNOT(/* word */...) /* -> word */ \
  PTL_WORD(PPUTLIMPL_UHEX(PTL_UHEX(__VA_ARGS__), BNOT), PTL_TYPEOF(__VA_ARGS__))

/// [bitwise.band]
/// --------------
/// bitwise AND operation.
/// uses arg 'a' for result cast hint.
///
/// PTL_BAND(0, 0)         // 0
/// PTL_BAND(0, 1)         // 0
/// PTL_BAND(3, 2)         // 2
/// PTL_BAND(5, 6)         // 4
/// PTL_BAND(0x800, 0xFFF) // 0x800
#define PTL_BAND(/* word, word */...) /* -> word */ PPUTLBAND_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBAND_o(a, b)      \
  PPUTLBAND_RES(PTL_TYPEOF(a), \
                PPUTLBAND_oo(PPUTLBAND_X(PTL_ESC PTL_UTUP(a), PTL_ESC PTL_UTUP(b))))

#define PPUTLBAND_oo(...) PPUTLBAND_R(PPUTLBAND_R(PPUTLBAND_R(__VA_ARGS__)))

#define PPUTLBAND_RES(...)                       PPUTLBAND_RES_o(__VA_ARGS__)
#define PPUTLBAND_RES_o(_type, a, b, c, d, e, f) PTL_WORD((a, b, c), _type)
#define PPUTLBAND_R(...)                         PPUTLBAND_R_o(__VA_ARGS__)
#define PPUTLBAND_R_o(a, b, c, d, e, f)          PPUTLIMPL_HEXHEX(c##f, AND), a, b, f, d, e
#define PPUTLBAND_X(...)                         __VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [bitwise.bor]
/// -------------
/// bitwise OR operation.
/// uses arg 'a' for result cast hint.
///
/// PTL_BOR(0, 0)        // 0
/// PTL_BOR(0, 1)        // 1
/// PTL_BOR(3, 4)        // 7
/// PTL_BOR(5, 6)        // 7
/// PTL_BOR(0x800, 2047) // 0xFFF
#define PTL_BOR(/* word, word */...) /* -> word */ PPUTLBOR_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBOR_o(a, b)      \
  PPUTLBOR_RES(PTL_TYPEOF(a), \
               PPUTLBOR_oo(PPUTLBOR_X(PTL_ESC PTL_UTUP(a), PTL_ESC PTL_UTUP(b))))
#define PPUTLBOR_oo(...)                        PPUTLBOR_R(PPUTLBOR_R(PPUTLBOR_R(__VA_ARGS__)))
#define PPUTLBOR_RES(...)                       PPUTLBOR_RES_o(__VA_ARGS__)
#define PPUTLBOR_RES_o(_type, a, b, c, d, e, f) PTL_WORD((a, b, c), _type)
#define PPUTLBOR_R(...)                         PPUTLBOR_R_o(__VA_ARGS__)
#define PPUTLBOR_R_o(a, b, c, d, e, f)          PPUTLIMPL_HEXHEX(c##f, OR), a, b, f, d, e
#define PPUTLBOR_X(...)                         __VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [bitwise.bxor]
/// --------------
/// bitwise XOR operation.
/// uses arg 'a' for result cast hint.
///
/// PTL_BXOR(0, 0)         // 0
/// PTL_BXOR(0, 1)         // 1
/// PTL_BXOR(2, 1)         // 3
/// PTL_BXOR(2, 3)         // 1
/// PTL_BXOR(3, 4)         // 7
/// PTL_BXOR(5, 6)         // 3
/// PTL_BXOR(0x800, 0xFFF) // 0x7FF
#define PTL_BXOR(/* word, word */...) /* -> word */ PPUTLBXOR_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBXOR_o(a, b)      \
  PPUTLBXOR_RES(PTL_TYPEOF(a), \
                PPUTLBXOR_oo(PPUTLBXOR_X(PTL_ESC PTL_UTUP(a), PTL_ESC PTL_UTUP(b))))

#define PPUTLBXOR_oo(...) PPUTLBXOR_R(PPUTLBXOR_R(PPUTLBXOR_R(__VA_ARGS__)))

#define PPUTLBXOR_RES(...)                       PPUTLBXOR_RES_o(__VA_ARGS__)
#define PPUTLBXOR_RES_o(_type, a, b, c, d, e, f) PTL_WORD((a, b, c), _type)
#define PPUTLBXOR_R(...)                         PPUTLBXOR_R_o(__VA_ARGS__)
#define PPUTLBXOR_R_o(a, b, c, d, e, f)          PPUTLIMPL_HEXHEX(c##f, XOR), a, b, f, d, e
#define PPUTLBXOR_X(...)                         __VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [bitwise.bnand]
/// ---------------
/// bitwise NAND.
/// uses arg 'a' for result cast hint.
///
/// PTL_BNAND(0, 0)                 // 0xFFF
/// PTL_BNAND(5, 7)                 // 0xFFA
/// PTL_BNAND((7, F, F), (F, F, F)) // (8, 0, 0)
#define PTL_BNAND(/* word, word */...) /* -> word */              \
  PTL_WORD(PPUTLIMPL_UHEX(PTL_UHEX(PTL_BAND(__VA_ARGS__)), BNOT), \
           PTL_TYPEOF(PTL_FIRST(__VA_ARGS__)))

/// [bitwise.bnor]
/// --------------
/// bitwise NOR.
/// uses arg 'a' for result cast hint.
///
/// PTL_BNOR(0, 0) // 0xFFF
/// PTL_BNOR(0, 1) // 0xFFE
/// PTL_BNOR(5, 7) // 0xFF8
/// PTL_BNOR(7, 8) // 0xFF0
#define PTL_BNOR(/* word, word */...) /* -> word */              \
  PTL_WORD(PPUTLIMPL_UHEX(PTL_UHEX(PTL_BOR(__VA_ARGS__)), BNOT), \
           PTL_TYPEOF(PTL_FIRST(__VA_ARGS__)))

/// [bitwise.bxnor]
/// ---------------
/// bitwise XNOR.
/// uses arg 'a' for result cast hint.
///
/// PTL_BXNOR(0, 0)  // 0xFFF
/// PTL_BXNOR(0, 1)  // 0xFFE
/// PTL_BXNOR(5, 7)  // 0xFFD
/// PTL_BXNOR(15, 8) // 0xFF8
#define PTL_BXNOR(/* word, word */...) /* -> word */              \
  PTL_WORD(PPUTLIMPL_UHEX(PTL_UHEX(PTL_BXOR(__VA_ARGS__)), BNOT), \
           PTL_TYPEOF(PTL_FIRST(__VA_ARGS__)))

/// [bitwise.bget]
/// --------------
/// gets the ith bit from the word.
/// i must be between 0 and PTL_BIT_LENGTH (12).
///
/// PTL_BGET(2, 9)          // 0
/// PTL_BGET(2, 10)         // 1
/// PTL_BGET(2, 11)         // 0
/// PTL_BGET(5u, 9)         // 1
/// PTL_BGET(0xFFE, 10)     // 1
/// PTL_BGET(0xFFEu, 11)    // 0
/// PTL_BGET((F, F, F), 11) // 1
#define PTL_BGET(/* word, i: idec */...) /* -> bool */ \
  PPUTLBGET_o(PTL_STR([PTL_BGET] invalid index; args : __VA_ARGS__), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBGET_o(e, v, i) PPUTLBGET_oo(e, PTL_IDEC(i), PTL_BDUMP(v))
#define PPUTLBGET_oo(e, i, ...) \
  PPUTLBGET_ooo(e, i, PTL_AND(PTL_GE(i, 0), PTL_LT(i, 12)), __VA_ARGS__)
#define PPUTLBGET_ooo(...) PPUTLBGET_oooo(__VA_ARGS__)

#define PPUTLBGET_oooo(e, i, gelt, ...) PPUTLBGET_oooo_##gelt(e, i, __VA_ARGS__)

#define PPUTLBGET_oooo_1(e, i, ...)                      PPUTLBGET_##i(__VA_ARGS__)
#define PPUTLBGET_oooo_0(e, ...)                         PTL_FAIL(e)
#define PPUTLBGET_11(a, b, c, d, e, f, g, h, i, j, k, l) l
#define PPUTLBGET_10(a, b, c, d, e, f, g, h, i, j, k, l) k
#define PPUTLBGET_9(a, b, c, d, e, f, g, h, i, j, k, l)  j
#define PPUTLBGET_8(a, b, c, d, e, f, g, h, i, j, k, l)  i
#define PPUTLBGET_7(a, b, c, d, e, f, g, h, i, j, k, l)  h
#define PPUTLBGET_6(a, b, c, d, e, f, g, h, i, j, k, l)  g
#define PPUTLBGET_5(a, b, c, d, e, f, g, h, i, j, k, l)  f
#define PPUTLBGET_4(a, b, c, d, e, f, g, h, i, j, k, l)  e
#define PPUTLBGET_3(a, b, c, d, e, f, g, h, i, j, k, l)  d
#define PPUTLBGET_2(a, b, c, d, e, f, g, h, i, j, k, l)  c
#define PPUTLBGET_1(a, b, c, d, e, f, g, h, i, j, k, l)  b
#define PPUTLBGET_0(a, b, c, d, e, f, g, h, i, j, k, l)  a

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [bitwise.bset]
/// --------------
/// sets the ith bit of the word to b.
/// i must be between 0 and PTL_BIT_LENGTH (12).
///
/// PTL_BSET(0, 10, 1)        // 2
/// PTL_BSET(1u, 9, 1)        // 5u
/// PTL_BSET(5, 7, 1)         // 21
/// PTL_BSET(0x002, 11, 1)    // 0x003
/// PTL_BSET(0x003u, 11, 0)   // 0x002u
/// PTL_BSET((F, F, F), 0, 0) // (7, F, F)
#define PTL_BSET(/* word, i: idec, b: bool */...) /* -> word */ \
  PPUTLBSET_o(PTL_STR([PTL_BSET] invalid index; args : __VA_ARGS__), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBSET_o(e, v, i, b) \
  PTL_WORD(PPUTLBSET_oo(e, PTL_IDEC(i), PTL_BOOL(b), PTL_BDUMP(v)), PTL_TYPEOF(v))
#define PPUTLBSET_oo(e, i, b, ...)         PPUTLBSET_ooo(e, i, b, PTL_LT(i, 12), __VA_ARGS__)
#define PPUTLBSET_ooo(...)                 PPUTLBSET_oooo(__VA_ARGS__)
#define PPUTLBSET_oooo(e, i, b, gelt, ...) PPUTLBSET_oooo_##gelt(e, i, b, __VA_ARGS__)
#define PPUTLBSET_oooo_1(e, i, b, ...)     PPUTLBSET_##i(b, __VA_ARGS__)
#define PPUTLBSET_oooo_0(e, ...)           PTL_FAIL(e)
#define PPUTLBSET_11(l, a, b, c, d, e, f, g, h, i, j, k, _)          \
  (PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
   PPUTLIMPL_NYBL(i##j##k##l, HEX))
#define PPUTLBSET_10(k, a, b, c, d, e, f, g, h, i, j, _, l)          \
  (PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
   PPUTLIMPL_NYBL(i##j##k##l, HEX))
#define PPUTLBSET_9(j, a, b, c, d, e, f, g, h, i, _, k, l)           \
  (PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
   PPUTLIMPL_NYBL(i##j##k##l, HEX))
#define PPUTLBSET_8(i, a, b, c, d, e, f, g, h, _, j, k, l)           \
  (PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
   PPUTLIMPL_NYBL(i##j##k##l, HEX))
#define PPUTLBSET_7(h, a, b, c, d, e, f, g, _, i, j, k, l)           \
  (PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
   PPUTLIMPL_NYBL(i##j##k##l, HEX))
#define PPUTLBSET_6(g, a, b, c, d, e, f, _, h, i, j, k, l)           \
  (PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
   PPUTLIMPL_NYBL(i##j##k##l, HEX))
#define PPUTLBSET_5(f, a, b, c, d, e, _, g, h, i, j, k, l)           \
  (PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
   PPUTLIMPL_NYBL(i##j##k##l, HEX))
#define PPUTLBSET_4(e, a, b, c, d, _, f, g, h, i, j, k, l)           \
  (PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
   PPUTLIMPL_NYBL(i##j##k##l, HEX))
#define PPUTLBSET_3(d, a, b, c, _, e, f, g, h, i, j, k, l)           \
  (PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
   PPUTLIMPL_NYBL(i##j##k##l, HEX))
#define PPUTLBSET_2(c, a, b, _, d, e, f, g, h, i, j, k, l)           \
  (PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
   PPUTLIMPL_NYBL(i##j##k##l, HEX))
#define PPUTLBSET_1(b, a, _, c, d, e, f, g, h, i, j, k, l)           \
  (PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
   PPUTLIMPL_NYBL(i##j##k##l, HEX))
#define PPUTLBSET_0(a, _, b, c, d, e, f, g, h, i, j, k, l)           \
  (PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
   PPUTLIMPL_NYBL(i##j##k##l, HEX))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [bitwise.bflip]
/// ---------------
/// flips the ith bit in the uint.
/// i must be less than PTL_BIT_LENGTH (12).
///
/// PTL_BFLIP(0, 11)        // 1
/// PTL_BFLIP(1u, 10)       // 3u
/// PTL_BFLIP(0x002, 9)     // 0x006
/// PTL_BFLIP(0x003u, 8)    // 0x00Bu
/// PTL_BFLIP((F, F, F), 0) // (7, F, F)
#define PTL_BFLIP(/* word, i: idec */...) /* -> word */ \
  PTL_BSET(__VA_ARGS__, PTL_NOT(PTL_BGET(__VA_ARGS__)))

/// [bitwise.brotl]
/// ---------------
/// bitwise left rotation by n places.
///
/// PTL_BROTL(0x000, 0) // 0x000
/// PTL_BROTL(0x001, 1) // 0x002
/// PTL_BROTL(0x001, 2) // 0x004
/// PTL_BROTL(0x003, 2) // 0x00C
#define PTL_BROTL(/* word, n: idec */...) /* -> word */ PPUTLBROTL_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBROTL_o(v, n) \
  PTL_WORD(PPUTLBROTL_oo(n, PTL_ESC(PTL_BDUMP(v))), PTL_TYPEOF(v))
#define PPUTLBROTL_oo(n, ...) PTL_XCAT(PPUTLBROTL_, PTL_BAND(n, 0x00F))(__VA_ARGS__)
#define PPUTLBROTL_15(a, b, c, d, e, f, g, h, i, j, k, l)            \
  (PPUTLIMPL_NYBL(d##e##f##g, HEX), PPUTLIMPL_NYBL(h##i##j##k, HEX), \
   PPUTLIMPL_NYBL(l##a##b##c, HEX))
#define PPUTLBROTL_14(a, b, c, d, e, f, g, h, i, j, k, l)            \
  (PPUTLIMPL_NYBL(c##d##e##f, HEX), PPUTLIMPL_NYBL(g##h##i##j, HEX), \
   PPUTLIMPL_NYBL(k##l##a##b, HEX))
#define PPUTLBROTL_13(a, b, c, d, e, f, g, h, i, j, k, l)            \
  (PPUTLIMPL_NYBL(b##c##d##e, HEX), PPUTLIMPL_NYBL(f##g##h##i, HEX), \
   PPUTLIMPL_NYBL(j##k##l##a, HEX))
#define PPUTLBROTL_12(a, b, c, d, e, f, g, h, i, j, k, l)            \
  (PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
   PPUTLIMPL_NYBL(i##j##k##l, HEX))
#define PPUTLBROTL_11(a, b, c, d, e, f, g, h, i, j, k, l)            \
  (PPUTLIMPL_NYBL(l##a##b##c, HEX), PPUTLIMPL_NYBL(d##e##f##g, HEX), \
   PPUTLIMPL_NYBL(h##i##j##k, HEX))
#define PPUTLBROTL_10(a, b, c, d, e, f, g, h, i, j, k, l)            \
  (PPUTLIMPL_NYBL(k##l##a##b, HEX), PPUTLIMPL_NYBL(c##d##e##f, HEX), \
   PPUTLIMPL_NYBL(g##h##i##j, HEX))
#define PPUTLBROTL_9(a, b, c, d, e, f, g, h, i, j, k, l)             \
  (PPUTLIMPL_NYBL(j##k##l##a, HEX), PPUTLIMPL_NYBL(b##c##d##e, HEX), \
   PPUTLIMPL_NYBL(f##g##h##i, HEX))
#define PPUTLBROTL_8(a, b, c, d, e, f, g, h, i, j, k, l)             \
  (PPUTLIMPL_NYBL(i##j##k##l, HEX), PPUTLIMPL_NYBL(a##b##c##d, HEX), \
   PPUTLIMPL_NYBL(e##f##g##h, HEX))
#define PPUTLBROTL_7(a, b, c, d, e, f, g, h, i, j, k, l)             \
  (PPUTLIMPL_NYBL(h##i##j##k, HEX), PPUTLIMPL_NYBL(l##a##b##c, HEX), \
   PPUTLIMPL_NYBL(d##e##f##g, HEX))
#define PPUTLBROTL_6(a, b, c, d, e, f, g, h, i, j, k, l)             \
  (PPUTLIMPL_NYBL(g##h##i##j, HEX), PPUTLIMPL_NYBL(k##l##a##b, HEX), \
   PPUTLIMPL_NYBL(c##d##e##f, HEX))
#define PPUTLBROTL_5(a, b, c, d, e, f, g, h, i, j, k, l)             \
  (PPUTLIMPL_NYBL(f##g##h##i, HEX), PPUTLIMPL_NYBL(j##k##l##a, HEX), \
   PPUTLIMPL_NYBL(b##c##d##e, HEX))
#define PPUTLBROTL_4(a, b, c, d, e, f, g, h, i, j, k, l)             \
  (PPUTLIMPL_NYBL(e##f##g##h, HEX), PPUTLIMPL_NYBL(i##j##k##l, HEX), \
   PPUTLIMPL_NYBL(a##b##c##d, HEX))
#define PPUTLBROTL_3(a, b, c, d, e, f, g, h, i, j, k, l)             \
  (PPUTLIMPL_NYBL(d##e##f##g, HEX), PPUTLIMPL_NYBL(h##i##j##k, HEX), \
   PPUTLIMPL_NYBL(l##a##b##c, HEX))
#define PPUTLBROTL_2(a, b, c, d, e, f, g, h, i, j, k, l)             \
  (PPUTLIMPL_NYBL(c##d##e##f, HEX), PPUTLIMPL_NYBL(g##h##i##j, HEX), \
   PPUTLIMPL_NYBL(k##l##a##b, HEX))
#define PPUTLBROTL_1(a, b, c, d, e, f, g, h, i, j, k, l)             \
  (PPUTLIMPL_NYBL(b##c##d##e, HEX), PPUTLIMPL_NYBL(f##g##h##i, HEX), \
   PPUTLIMPL_NYBL(j##k##l##a, HEX))
#define PPUTLBROTL_0(a, b, c, d, e, f, g, h, i, j, k, l)             \
  (PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
   PPUTLIMPL_NYBL(i##j##k##l, HEX))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [bitwise.brotr]
/// ---------------
/// bitwise right rotation by n places.
///
/// PTL_BROTR(0x000, 0) // 0x000
/// PTL_BROTR(0x001, 0) // 0x001
/// PTL_BROTR(0x001, 1) // 0x800
/// PTL_BROTR(0x002, 1) // 0x001
/// PTL_BROTR(0x7FF, 2) // 0xDFF
#define PTL_BROTR(/* word, n: idec */...) /* -> word */ PPUTLBROTR_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLBROTR_o(v, n)    PTL_WORD(PPUTLBROTR_oo(n, PTL_BDUMP(v)), PTL_TYPEOF(v))
#define PPUTLBROTR_oo(n, ...) PTL_XCAT(PPUTLBROTR_, PTL_BAND(n, 0x00F))(__VA_ARGS__)
#define PPUTLBROTR_15(a, b, c, d, e, f, g, h, i, j, k, l)            \
  (PPUTLIMPL_NYBL(j##k##l##a, HEX), PPUTLIMPL_NYBL(b##c##d##e, HEX), \
   PPUTLIMPL_NYBL(f##g##h##i, HEX))
#define PPUTLBROTR_14(a, b, c, d, e, f, g, h, i, j, k, l)            \
  (PPUTLIMPL_NYBL(k##l##a##b, HEX), PPUTLIMPL_NYBL(c##d##e##f, HEX), \
   PPUTLIMPL_NYBL(g##h##i##j, HEX))
#define PPUTLBROTR_13(a, b, c, d, e, f, g, h, i, j, k, l)            \
  (PPUTLIMPL_NYBL(l##a##b##c, HEX), PPUTLIMPL_NYBL(d##e##f##g, HEX), \
   PPUTLIMPL_NYBL(h##i##j##k, HEX))
#define PPUTLBROTR_12(a, b, c, d, e, f, g, h, i, j, k, l)            \
  (PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
   PPUTLIMPL_NYBL(i##j##k##l, HEX))
#define PPUTLBROTR_11(a, b, c, d, e, f, g, h, i, j, k, l)            \
  (PPUTLIMPL_NYBL(b##c##d##e, HEX), PPUTLIMPL_NYBL(f##g##h##i, HEX), \
   PPUTLIMPL_NYBL(j##k##l##a, HEX))
#define PPUTLBROTR_10(a, b, c, d, e, f, g, h, i, j, k, l)            \
  (PPUTLIMPL_NYBL(c##d##e##f, HEX), PPUTLIMPL_NYBL(g##h##i##j, HEX), \
   PPUTLIMPL_NYBL(k##l##a##b, HEX))
#define PPUTLBROTR_9(a, b, c, d, e, f, g, h, i, j, k, l)             \
  (PPUTLIMPL_NYBL(d##e##f##g, HEX), PPUTLIMPL_NYBL(h##i##j##k, HEX), \
   PPUTLIMPL_NYBL(l##a##b##c, HEX))
#define PPUTLBROTR_8(a, b, c, d, e, f, g, h, i, j, k, l)             \
  (PPUTLIMPL_NYBL(e##f##g##h, HEX), PPUTLIMPL_NYBL(i##j##k##l, HEX), \
   PPUTLIMPL_NYBL(a##b##c##d, HEX))
#define PPUTLBROTR_7(a, b, c, d, e, f, g, h, i, j, k, l)             \
  (PPUTLIMPL_NYBL(f##g##h##i, HEX), PPUTLIMPL_NYBL(j##k##l##a, HEX), \
   PPUTLIMPL_NYBL(b##c##d##e, HEX))
#define PPUTLBROTR_6(a, b, c, d, e, f, g, h, i, j, k, l)             \
  (PPUTLIMPL_NYBL(g##h##i##j, HEX), PPUTLIMPL_NYBL(k##l##a##b, HEX), \
   PPUTLIMPL_NYBL(c##d##e##f, HEX))
#define PPUTLBROTR_5(a, b, c, d, e, f, g, h, i, j, k, l)             \
  (PPUTLIMPL_NYBL(h##i##j##k, HEX), PPUTLIMPL_NYBL(l##a##b##c, HEX), \
   PPUTLIMPL_NYBL(d##e##f##g, HEX))
#define PPUTLBROTR_4(a, b, c, d, e, f, g, h, i, j, k, l)             \
  (PPUTLIMPL_NYBL(i##j##k##l, HEX), PPUTLIMPL_NYBL(a##b##c##d, HEX), \
   PPUTLIMPL_NYBL(e##f##g##h, HEX))
#define PPUTLBROTR_3(a, b, c, d, e, f, g, h, i, j, k, l)             \
  (PPUTLIMPL_NYBL(j##k##l##a, HEX), PPUTLIMPL_NYBL(b##c##d##e, HEX), \
   PPUTLIMPL_NYBL(f##g##h##i, HEX))
#define PPUTLBROTR_2(a, b, c, d, e, f, g, h, i, j, k, l)             \
  (PPUTLIMPL_NYBL(k##l##a##b, HEX), PPUTLIMPL_NYBL(c##d##e##f, HEX), \
   PPUTLIMPL_NYBL(g##h##i##j, HEX))
#define PPUTLBROTR_1(a, b, c, d, e, f, g, h, i, j, k, l)             \
  (PPUTLIMPL_NYBL(l##a##b##c, HEX), PPUTLIMPL_NYBL(d##e##f##g, HEX), \
   PPUTLIMPL_NYBL(h##i##j##k, HEX))
#define PPUTLBROTR_0(a, b, c, d, e, f, g, h, i, j, k, l)             \
  (PPUTLIMPL_NYBL(a##b##c##d, HEX), PPUTLIMPL_NYBL(e##f##g##h, HEX), \
   PPUTLIMPL_NYBL(i##j##k##l, HEX))

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
  (PTL_STR([PTL_XTRACE_READ] invalid xtrace expr : __VA_ARGS__), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLXTRACE_READ_1(err, ...)           PPUTLXTRACE_READ_##__VA_ARGS__
#define PPUTLXTRACE_READ_0(err, ...)           PTL_FAIL(err)
#define PPUTLXTRACE_READ_PPUTLXTRACE_B(_, ...) PPUTLXTRACE_READ_RES(__VA_ARGS__.)
#define PPUTLXTRACE_READ_PPUTLXTRACE_A(_, ...) PPUTLXTRACE_READ_RES(__VA_ARGS__.)
#define PPUTLXTRACE_READ_RES(_, ...)           PTL_SIZEOF(__VA_ARGS__)
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
///   mul(a, b)  = first(id(recur_lp(a, madd) 0, b recur_rp(a)))
///   mul(2, 4) -> first(id(madd LP madd LP 0, 4 RP RP))
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
  PTL_XCAT(PPUTLRLP_oo_, PTL_BAND(n, 3))(PTL_IDEC(PTL_BSRL(n, 2)), f)
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
///   mul(a, b)  = first(id(recur_lp(a, madd) 0, 4 recur_rp(a)))
///   mul(2, 4) -> first(id(madd LP madd LP 0, 4 RP RP))
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

#define PPUTLRRP_o(n)    PTL_XCAT(PPUTLRRP_o_, PTL_BAND(n, 3))(PTL_IDEC(PTL_BSRL(n, 2)))
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
/// conditionally expands items based on a boolean.
///
/// PTL_IF(1, (t), ())     // t
/// PTL_IF(0, (t), ())     // <nothing>
/// PTL_IF(1, (t), (f))    // t
/// PTL_IF(0, (t), (f))    // f
/// PTL_IF(1, (a), (b, c)) // a
/// PTL_IF(0, (a), (b, c)) // b, c
#define PTL_IF(/* bool, t: tup, f: tup */...) /* -> list */ \
  PTL_XCAT(PPUTLIF_, PTL_BOOL(PTL_FIRST(__VA_ARGS__)))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIF_1(_, t, f) PTL_REST(PTL_TUP(f), PTL_ESC PTL_TUP(t))
#define PPUTLIF_0(_, t, f) PTL_REST(PTL_TUP(t), PTL_ESC PTL_TUP(f))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [control.switch]
/// ----------------
/// conditionally expands items based on a size.
/// the final tuple is the default case.
///
/// PTL_SWITCH(0, (1))                // 1
/// PTL_SWITCH(1, (1))                // 1
/// PTL_SWITCH(2, (1))                // 1
/// PTL_SWITCH(1, (1), (2))           // 2
/// PTL_SWITCH(2, (1), (2))           // 2
/// PTL_SWITCH(2, (1), (2), (3, 4))   // 3, 4
/// PTL_SWITCH(255, (1), (2), (3, 4)) // 3, 4
#define PTL_SWITCH(/* size, ...cases: tup */...) /* -> list */                      \
  PPUTLSWITCH_RES(PPUTLSWITCH_X(PTL_RECUR_LP(PTL_FIRST(__VA_ARGS__), PPUTLSWITCH_R) \
                                    __VA_ARGS__ PTL_RECUR_RP(PTL_FIRST(__VA_ARGS__))))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLSWITCH_RES(...)         PPUTLSWITCH_RES_X(__VA_ARGS__)
#define PPUTLSWITCH_RES_X(i, _, ...) PPUTLSWITCH_X(PTL_ESC PTL_TUP(_))
#define PPUTLSWITCH_X(...)           __VA_ARGS__
#define PPUTLSWITCH_R(...)           PPUTLSWITCH_R_X(__VA_ARGS__)
#define PPUTLSWITCH_R_X(i, _, ...)                                    \
  PTL_IF(PTL_IF(PTL_EQZ(i), (1), (PTL_IS_NONE(__VA_ARGS__))), (0, _), \
         (PTL_DEC(i), __VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [math.neg]
/// ----------
/// integral negation.
///
/// PTL_NEG(0)     // 0
/// PTL_NEG(1)     // 0xFFF
/// PTL_NEG(0xFFF) // 0x001
/// PTL_NEG(1u)    // 4095u
#define PTL_NEG(/* word */...) /* -> word */ \
  PTL_WORD(PTL_INC(PTL_BNOT(__VA_ARGS__)), PTL_TYPEOF(__VA_ARGS__))

/// [math.add]
/// ----------
/// addition with overflow.
///
/// returns unsigned if either operand is unsigned,
/// decimal if either operand is decimal, utup if
/// both operands are utup, and hex otherwise.
///
/// PTL_ADD(0, 0)            // 0
/// PTL_ADD(0, 1)            // 1
/// PTL_ADD(1, 2)            // 3
/// PTL_ADD(3u, 4)           // 7u
/// PTL_ADD(5, 6u)           // 11u
/// PTL_ADD(4095u, 1)        // 0u
/// PTL_ADD(4095u, 2)        // 1u
/// PTL_ADD(4095u, 4095u)    // 4094u
/// PTL_ADD(2047, 1)         // 0x800
/// PTL_ADD(2047, (0, 0, 1)) // 2048u
#define PTL_ADD(/* word, word */...) /* -> word */ PPUTLADD_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLADD_o(a, b)                         \
  PPUTLADD_RES(PTL_TYPEOF(a), PTL_TYPEOF(b),     \
               PPUTLADD_R(PPUTLADD_R(PPUTLADD_R( \
                   0, PPUTLADD_X(PTL_ESC PTL_UTUP(a), PTL_ESC PTL_UTUP(b))))))
#define PPUTLADD_RES(ta, tb, ...) \
  PTL_WORD(PPUTLADD_RES_o(__VA_ARGS__), PPUTLIMPL_ARITHHINT(ta, tb))
#define PPUTLADD_RES_o(_, a, b, c, d, e, f) (a, b, c)
#define PPUTLADD_R(...)                     PPUTLADD_R_o(__VA_ARGS__)
#define PPUTLADD_R_o(_, a, b, c, d, e, f)   PPUTLIMPL_HEXHEX(c##f, ADD##_), a, b, f, d, e
#define PPUTLADD_X(...)                     __VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [math.sub]
/// ----------
/// subtraction with underflow.
///
/// returns unsigned if either operand is unsigned,
/// decimal if either operand is decimal, utup if
/// both operands are utup, and hex otherwise.
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
#define PTL_SUB(/* word, word */...) /* -> word */ \
  PTL_ADD(PTL_FIRST(__VA_ARGS__), PTL_NEG(PTL_REST(__VA_ARGS__)))

/// [range.items]
/// -------------
/// extracts tuple items.
///
/// PTL_ITEMS(())        // <nothing>
/// PTL_ITEMS((a))       // a
/// PTL_ITEMS((a, b))    // a, b
/// PTL_ITEMS((a, b, c)) // a, b, c
#define PTL_ITEMS(/* tup */...) /* -> list */ PPUTLITEMS_X(PTL_TUP(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLITEMS_X(...) PTL_ESC __VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [impl]
/// ------

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

/// [impl.traits.hex]
/// -----------------
/// [internal] hex traits
#define PPUTLIMPL_HEX(/* {<atom>, IS}|{<hex>, NOT|DEC|INC|NYBL|BITS} */ v, t) \
  PPUTLIMPL_HEX_o(t, PTL_XCAT(PPUTLIMPL_HEX_, v))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_HEX_o(t, ...)              PPUTLIMPL_HEX_##t(__VA_ARGS__)
#define PPUTLIMPL_HEX_BITS(n, d, i, ny, ...) /* -> ...bool */ __VA_ARGS__
#define PPUTLIMPL_HEX_NYBL(n, d, i, ny, ...) /* -> nybl */ ny
#define PPUTLIMPL_HEX_INC(n, d, i, ...)      /* -> (bool, hex) */ i
#define PPUTLIMPL_HEX_DEC(n, d, ...)         /* -> (bool, hex) */ d
#define PPUTLIMPL_HEX_NOT(n, ...)            /* -> hex */ n
#define PPUTLIMPL_HEX_IS(_, ...)             /* -> bool */ PPUTLIMPL_HEX_IS_0##__VA_OPT__(1)
#define PPUTLIMPL_HEX_IS_01                  1
#define PPUTLIMPL_HEX_IS_0                   0

/// not, (dec carry, dec), (inc carry, inc), nybl, ...bits
#define PPUTLIMPL_HEX_F 0, (0, E), (1, 0), 1111, 1, 1, 1, 1
#define PPUTLIMPL_HEX_E 1, (0, D), (0, F), 1110, 1, 1, 1, 0
#define PPUTLIMPL_HEX_D 2, (0, C), (0, E), 1101, 1, 1, 0, 1
#define PPUTLIMPL_HEX_C 3, (0, B), (0, D), 1100, 1, 1, 0, 0
#define PPUTLIMPL_HEX_B 4, (0, A), (0, C), 1011, 1, 0, 1, 1
#define PPUTLIMPL_HEX_A 5, (0, 9), (0, B), 1010, 1, 0, 1, 0
#define PPUTLIMPL_HEX_9 6, (0, 8), (0, A), 1001, 1, 0, 0, 1
#define PPUTLIMPL_HEX_8 7, (0, 7), (0, 9), 1000, 1, 0, 0, 0
#define PPUTLIMPL_HEX_7 8, (0, 6), (0, 8), 0111, 0, 1, 1, 1
#define PPUTLIMPL_HEX_6 9, (0, 5), (0, 7), 0110, 0, 1, 1, 0
#define PPUTLIMPL_HEX_5 A, (0, 4), (0, 6), 0101, 0, 1, 0, 1
#define PPUTLIMPL_HEX_4 B, (0, 3), (0, 5), 0100, 0, 1, 0, 0
#define PPUTLIMPL_HEX_3 C, (0, 2), (0, 4), 0011, 0, 0, 1, 1
#define PPUTLIMPL_HEX_2 D, (0, 1), (0, 3), 0010, 0, 0, 1, 0
#define PPUTLIMPL_HEX_1 E, (0, 0), (0, 2), 0001, 0, 0, 0, 1
#define PPUTLIMPL_HEX_0 F, (1, F), (0, 1), 0000, 0, 0, 0, 0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [impl.traits.hexhex]
/// --------------------
/// [internal] hex pair (hex##hex) traits
#define PPUTLIMPL_HEXHEX(/* {<atom>, IS}|{<hex##hex>, LT|AND|OR|XOR|SUB|ADD0|ADD1} */ v, \
                         t)                                                              \
  PPUTLIMPL_HEXHEX_o(t, PTL_XCAT(PPUTLIMPL_HEXHEX_, v))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_HEXHEX_o(t, ...) PPUTLIMPL_HEXHEX_##t(__VA_ARGS__)

#define PPUTLIMPL_HEXHEX_ADD1(l, a, o, x, sc, ss, a0c, a0s, a1c, a1s) /* -> bool, hex */ \
  a1c, a1s
#define PPUTLIMPL_HEXHEX_ADD0(l, a, o, x, sc, ss, a0c, a0s, ...) /* -> bool, hex */ \
  a0c, a0s
#define PPUTLIMPL_HEXHEX_SUB(l, a, o, x, sc, ss, ...) /* -> bool, hex */ sc, ss
#define PPUTLIMPL_HEXHEX_XOR(l, a, o, x, ...)         /* -> hex */ x
#define PPUTLIMPL_HEXHEX_OR(l, a, o, ...)             /* -> hex */ o
#define PPUTLIMPL_HEXHEX_AND(l, a, ...)               /* -> hex */ a
#define PPUTLIMPL_HEXHEX_LT(l, ...)                   /* -> bool */ l

#define PPUTLIMPL_HEXHEX_IS(_, ...) /* -> bool */ PPUTLIMPL_HEXHEX_IS_0##__VA_OPT__(1)

#define PPUTLIMPL_HEXHEX_IS_01 1
#define PPUTLIMPL_HEXHEX_IS_0  0

/// lt, and, or, xor, sub carry, sub, add0 carry, add0 sum, add1 carry, add1 sum
#define PPUTLIMPL_HEXHEX_FF 0, F, F, 0, 0, 0, 1, E, 1, F
#define PPUTLIMPL_HEXHEX_FE 0, E, F, 1, 0, 1, 1, D, 1, E
#define PPUTLIMPL_HEXHEX_FD 0, D, F, 2, 0, 2, 1, C, 1, D
#define PPUTLIMPL_HEXHEX_FC 0, C, F, 3, 0, 3, 1, B, 1, C
#define PPUTLIMPL_HEXHEX_FB 0, B, F, 4, 0, 4, 1, A, 1, B
#define PPUTLIMPL_HEXHEX_FA 0, A, F, 5, 0, 5, 1, 9, 1, A
#define PPUTLIMPL_HEXHEX_F9 0, 9, F, 6, 0, 6, 1, 8, 1, 9
#define PPUTLIMPL_HEXHEX_F8 0, 8, F, 7, 0, 7, 1, 7, 1, 8
#define PPUTLIMPL_HEXHEX_F7 0, 7, F, 8, 0, 8, 1, 6, 1, 7
#define PPUTLIMPL_HEXHEX_F6 0, 6, F, 9, 0, 9, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_F5 0, 5, F, A, 0, A, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_F4 0, 4, F, B, 0, B, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_F3 0, 3, F, C, 0, C, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_F2 0, 2, F, D, 0, D, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_F1 0, 1, F, E, 0, E, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_F0 0, 0, F, F, 0, F, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_EF 1, E, F, 1, 1, F, 1, D, 1, E
#define PPUTLIMPL_HEXHEX_EE 0, E, E, 0, 0, 0, 1, C, 1, D
#define PPUTLIMPL_HEXHEX_ED 0, C, F, 3, 0, 1, 1, B, 1, C
#define PPUTLIMPL_HEXHEX_EC 0, C, E, 2, 0, 2, 1, A, 1, B
#define PPUTLIMPL_HEXHEX_EB 0, A, F, 5, 0, 3, 1, 9, 1, A
#define PPUTLIMPL_HEXHEX_EA 0, A, E, 4, 0, 4, 1, 8, 1, 9
#define PPUTLIMPL_HEXHEX_E9 0, 8, F, 7, 0, 5, 1, 7, 1, 8
#define PPUTLIMPL_HEXHEX_E8 0, 8, E, 6, 0, 6, 1, 6, 1, 7
#define PPUTLIMPL_HEXHEX_E7 0, 6, F, 9, 0, 7, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_E6 0, 6, E, 8, 0, 8, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_E5 0, 4, F, B, 0, 9, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_E4 0, 4, E, A, 0, A, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_E3 0, 2, F, D, 0, B, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_E2 0, 2, E, C, 0, C, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_E1 0, 0, F, F, 0, D, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_E0 0, 0, E, E, 0, E, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_DF 1, D, F, 2, 1, E, 1, C, 1, D
#define PPUTLIMPL_HEXHEX_DE 1, C, F, 3, 1, F, 1, B, 1, C
#define PPUTLIMPL_HEXHEX_DD 0, D, D, 0, 0, 0, 1, A, 1, B
#define PPUTLIMPL_HEXHEX_DC 0, C, D, 1, 0, 1, 1, 9, 1, A
#define PPUTLIMPL_HEXHEX_DB 0, 9, F, 6, 0, 2, 1, 8, 1, 9
#define PPUTLIMPL_HEXHEX_DA 0, 8, F, 7, 0, 3, 1, 7, 1, 8
#define PPUTLIMPL_HEXHEX_D9 0, 9, D, 4, 0, 4, 1, 6, 1, 7
#define PPUTLIMPL_HEXHEX_D8 0, 8, D, 5, 0, 5, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_D7 0, 5, F, A, 0, 6, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_D6 0, 4, F, B, 0, 7, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_D5 0, 5, D, 8, 0, 8, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_D4 0, 4, D, 9, 0, 9, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_D3 0, 1, F, E, 0, A, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_D2 0, 0, F, F, 0, B, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_D1 0, 1, D, C, 0, C, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_D0 0, 0, D, D, 0, D, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_CF 1, C, F, 3, 1, D, 1, B, 1, C
#define PPUTLIMPL_HEXHEX_CE 1, C, E, 2, 1, E, 1, A, 1, B
#define PPUTLIMPL_HEXHEX_CD 1, C, D, 1, 1, F, 1, 9, 1, A
#define PPUTLIMPL_HEXHEX_CC 0, C, C, 0, 0, 0, 1, 8, 1, 9
#define PPUTLIMPL_HEXHEX_CB 0, 8, F, 7, 0, 1, 1, 7, 1, 8
#define PPUTLIMPL_HEXHEX_CA 0, 8, E, 6, 0, 2, 1, 6, 1, 7
#define PPUTLIMPL_HEXHEX_C9 0, 8, D, 5, 0, 3, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_C8 0, 8, C, 4, 0, 4, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_C7 0, 4, F, B, 0, 5, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_C6 0, 4, E, A, 0, 6, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_C5 0, 4, D, 9, 0, 7, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_C4 0, 4, C, 8, 0, 8, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_C3 0, 0, F, F, 0, 9, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_C2 0, 0, E, E, 0, A, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_C1 0, 0, D, D, 0, B, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_C0 0, 0, C, C, 0, C, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_BF 1, B, F, 4, 1, C, 1, A, 1, B
#define PPUTLIMPL_HEXHEX_BE 1, A, F, 5, 1, D, 1, 9, 1, A
#define PPUTLIMPL_HEXHEX_BD 1, 9, F, 6, 1, E, 1, 8, 1, 9
#define PPUTLIMPL_HEXHEX_BC 1, 8, F, 7, 1, F, 1, 7, 1, 8
#define PPUTLIMPL_HEXHEX_BB 0, B, B, 0, 0, 0, 1, 6, 1, 7
#define PPUTLIMPL_HEXHEX_BA 0, A, B, 1, 0, 1, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_B9 0, 9, B, 2, 0, 2, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_B8 0, 8, B, 3, 0, 3, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_B7 0, 3, F, C, 0, 4, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_B6 0, 2, F, D, 0, 5, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_B5 0, 1, F, E, 0, 6, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_B4 0, 0, F, F, 0, 7, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_B3 0, 3, B, 8, 0, 8, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_B2 0, 2, B, 9, 0, 9, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_B1 0, 1, B, A, 0, A, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_B0 0, 0, B, B, 0, B, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_AF 1, A, F, 5, 1, B, 1, 9, 1, A
#define PPUTLIMPL_HEXHEX_AE 1, A, E, 4, 1, C, 1, 8, 1, 9
#define PPUTLIMPL_HEXHEX_AD 1, 8, F, 7, 1, D, 1, 7, 1, 8
#define PPUTLIMPL_HEXHEX_AC 1, 8, E, 6, 1, E, 1, 6, 1, 7
#define PPUTLIMPL_HEXHEX_AB 1, A, B, 1, 1, F, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_AA 0, A, A, 0, 0, 0, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_A9 0, 8, B, 3, 0, 1, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_A8 0, 8, A, 2, 0, 2, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_A7 0, 2, F, D, 0, 3, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_A6 0, 2, E, C, 0, 4, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_A5 0, 0, F, F, 0, 5, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_A4 0, 0, E, E, 0, 6, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_A3 0, 2, B, 9, 0, 7, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_A2 0, 2, A, 8, 0, 8, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_A1 0, 0, B, B, 0, 9, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_A0 0, 0, A, A, 0, A, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_9F 1, 9, F, 6, 1, A, 1, 8, 1, 9
#define PPUTLIMPL_HEXHEX_9E 1, 8, F, 7, 1, B, 1, 7, 1, 8
#define PPUTLIMPL_HEXHEX_9D 1, 9, D, 4, 1, C, 1, 6, 1, 7
#define PPUTLIMPL_HEXHEX_9C 1, 8, D, 5, 1, D, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_9B 1, 9, B, 2, 1, E, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_9A 1, 8, B, 3, 1, F, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_99 0, 9, 9, 0, 0, 0, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_98 0, 8, 9, 1, 0, 1, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_97 0, 1, F, E, 0, 2, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_96 0, 0, F, F, 0, 3, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_95 0, 1, D, C, 0, 4, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_94 0, 0, D, D, 0, 5, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_93 0, 1, B, A, 0, 6, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_92 0, 0, B, B, 0, 7, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_91 0, 1, 9, 8, 0, 8, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_90 0, 0, 9, 9, 0, 9, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_8F 1, 8, F, 7, 1, 9, 1, 7, 1, 8
#define PPUTLIMPL_HEXHEX_8E 1, 8, E, 6, 1, A, 1, 6, 1, 7
#define PPUTLIMPL_HEXHEX_8D 1, 8, D, 5, 1, B, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_8C 1, 8, C, 4, 1, C, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_8B 1, 8, B, 3, 1, D, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_8A 1, 8, A, 2, 1, E, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_89 1, 8, 9, 1, 1, F, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_88 0, 8, 8, 0, 0, 0, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_87 0, 0, F, F, 0, 1, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_86 0, 0, E, E, 0, 2, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_85 0, 0, D, D, 0, 3, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_84 0, 0, C, C, 0, 4, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_83 0, 0, B, B, 0, 5, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_82 0, 0, A, A, 0, 6, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_81 0, 0, 9, 9, 0, 7, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_80 0, 0, 8, 8, 0, 8, 0, 8, 0, 9
#define PPUTLIMPL_HEXHEX_7F 1, 7, F, 8, 1, 8, 1, 6, 1, 7
#define PPUTLIMPL_HEXHEX_7E 1, 6, F, 9, 1, 9, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_7D 1, 5, F, A, 1, A, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_7C 1, 4, F, B, 1, B, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_7B 1, 3, F, C, 1, C, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_7A 1, 2, F, D, 1, D, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_79 1, 1, F, E, 1, E, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_78 1, 0, F, F, 1, F, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_77 0, 7, 7, 0, 0, 0, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_76 0, 6, 7, 1, 0, 1, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_75 0, 5, 7, 2, 0, 2, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_74 0, 4, 7, 3, 0, 3, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_73 0, 3, 7, 4, 0, 4, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_72 0, 2, 7, 5, 0, 5, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_71 0, 1, 7, 6, 0, 6, 0, 8, 0, 9
#define PPUTLIMPL_HEXHEX_70 0, 0, 7, 7, 0, 7, 0, 7, 0, 8
#define PPUTLIMPL_HEXHEX_6F 1, 6, F, 9, 1, 7, 1, 5, 1, 6
#define PPUTLIMPL_HEXHEX_6E 1, 6, E, 8, 1, 8, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_6D 1, 4, F, B, 1, 9, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_6C 1, 4, E, A, 1, A, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_6B 1, 2, F, D, 1, B, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_6A 1, 2, E, C, 1, C, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_69 1, 0, F, F, 1, D, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_68 1, 0, E, E, 1, E, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_67 1, 6, 7, 1, 1, F, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_66 0, 6, 6, 0, 0, 0, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_65 0, 4, 7, 3, 0, 1, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_64 0, 4, 6, 2, 0, 2, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_63 0, 2, 7, 5, 0, 3, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_62 0, 2, 6, 4, 0, 4, 0, 8, 0, 9
#define PPUTLIMPL_HEXHEX_61 0, 0, 7, 7, 0, 5, 0, 7, 0, 8
#define PPUTLIMPL_HEXHEX_60 0, 0, 6, 6, 0, 6, 0, 6, 0, 7
#define PPUTLIMPL_HEXHEX_5F 1, 5, F, A, 1, 6, 1, 4, 1, 5
#define PPUTLIMPL_HEXHEX_5E 1, 4, F, B, 1, 7, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_5D 1, 5, D, 8, 1, 8, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_5C 1, 4, D, 9, 1, 9, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_5B 1, 1, F, E, 1, A, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_5A 1, 0, F, F, 1, B, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_59 1, 1, D, C, 1, C, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_58 1, 0, D, D, 1, D, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_57 1, 5, 7, 2, 1, E, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_56 1, 4, 7, 3, 1, F, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_55 0, 5, 5, 0, 0, 0, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_54 0, 4, 5, 1, 0, 1, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_53 0, 1, 7, 6, 0, 2, 0, 8, 0, 9
#define PPUTLIMPL_HEXHEX_52 0, 0, 7, 7, 0, 3, 0, 7, 0, 8
#define PPUTLIMPL_HEXHEX_51 0, 1, 5, 4, 0, 4, 0, 6, 0, 7
#define PPUTLIMPL_HEXHEX_50 0, 0, 5, 5, 0, 5, 0, 5, 0, 6
#define PPUTLIMPL_HEXHEX_4F 1, 4, F, B, 1, 5, 1, 3, 1, 4
#define PPUTLIMPL_HEXHEX_4E 1, 4, E, A, 1, 6, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_4D 1, 4, D, 9, 1, 7, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_4C 1, 4, C, 8, 1, 8, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_4B 1, 0, F, F, 1, 9, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_4A 1, 0, E, E, 1, A, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_49 1, 0, D, D, 1, B, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_48 1, 0, C, C, 1, C, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_47 1, 4, 7, 3, 1, D, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_46 1, 4, 6, 2, 1, E, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_45 1, 4, 5, 1, 1, F, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_44 0, 4, 4, 0, 0, 0, 0, 8, 0, 9
#define PPUTLIMPL_HEXHEX_43 0, 0, 7, 7, 0, 1, 0, 7, 0, 8
#define PPUTLIMPL_HEXHEX_42 0, 0, 6, 6, 0, 2, 0, 6, 0, 7
#define PPUTLIMPL_HEXHEX_41 0, 0, 5, 5, 0, 3, 0, 5, 0, 6
#define PPUTLIMPL_HEXHEX_40 0, 0, 4, 4, 0, 4, 0, 4, 0, 5
#define PPUTLIMPL_HEXHEX_3F 1, 3, F, C, 1, 4, 1, 2, 1, 3
#define PPUTLIMPL_HEXHEX_3E 1, 2, F, D, 1, 5, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_3D 1, 1, F, E, 1, 6, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_3C 1, 0, F, F, 1, 7, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_3B 1, 3, B, 8, 1, 8, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_3A 1, 2, B, 9, 1, 9, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_39 1, 1, B, A, 1, A, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_38 1, 0, B, B, 1, B, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_37 1, 3, 7, 4, 1, C, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_36 1, 2, 7, 5, 1, D, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_35 1, 1, 7, 6, 1, E, 0, 8, 0, 9
#define PPUTLIMPL_HEXHEX_34 1, 0, 7, 7, 1, F, 0, 7, 0, 8
#define PPUTLIMPL_HEXHEX_33 0, 3, 3, 0, 0, 0, 0, 6, 0, 7
#define PPUTLIMPL_HEXHEX_32 0, 2, 3, 1, 0, 1, 0, 5, 0, 6
#define PPUTLIMPL_HEXHEX_31 0, 1, 3, 2, 0, 2, 0, 4, 0, 5
#define PPUTLIMPL_HEXHEX_30 0, 0, 3, 3, 0, 3, 0, 3, 0, 4
#define PPUTLIMPL_HEXHEX_2F 1, 2, F, D, 1, 3, 1, 1, 1, 2
#define PPUTLIMPL_HEXHEX_2E 1, 2, E, C, 1, 4, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_2D 1, 0, F, F, 1, 5, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_2C 1, 0, E, E, 1, 6, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_2B 1, 2, B, 9, 1, 7, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_2A 1, 2, A, 8, 1, 8, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_29 1, 0, B, B, 1, 9, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_28 1, 0, A, A, 1, A, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_27 1, 2, 7, 5, 1, B, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_26 1, 2, 6, 4, 1, C, 0, 8, 0, 9
#define PPUTLIMPL_HEXHEX_25 1, 0, 7, 7, 1, D, 0, 7, 0, 8
#define PPUTLIMPL_HEXHEX_24 1, 0, 6, 6, 1, E, 0, 6, 0, 7
#define PPUTLIMPL_HEXHEX_23 1, 2, 3, 1, 1, F, 0, 5, 0, 6
#define PPUTLIMPL_HEXHEX_22 0, 2, 2, 0, 0, 0, 0, 4, 0, 5
#define PPUTLIMPL_HEXHEX_21 0, 0, 3, 3, 0, 1, 0, 3, 0, 4
#define PPUTLIMPL_HEXHEX_20 0, 0, 2, 2, 0, 2, 0, 2, 0, 3
#define PPUTLIMPL_HEXHEX_1F 1, 1, F, E, 1, 2, 1, 0, 1, 1
#define PPUTLIMPL_HEXHEX_1E 1, 0, F, F, 1, 3, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_1D 1, 1, D, C, 1, 4, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_1C 1, 0, D, D, 1, 5, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_1B 1, 1, B, A, 1, 6, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_1A 1, 0, B, B, 1, 7, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_19 1, 1, 9, 8, 1, 8, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_18 1, 0, 9, 9, 1, 9, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_17 1, 1, 7, 6, 1, A, 0, 8, 0, 9
#define PPUTLIMPL_HEXHEX_16 1, 0, 7, 7, 1, B, 0, 7, 0, 8
#define PPUTLIMPL_HEXHEX_15 1, 1, 5, 4, 1, C, 0, 6, 0, 7
#define PPUTLIMPL_HEXHEX_14 1, 0, 5, 5, 1, D, 0, 5, 0, 6
#define PPUTLIMPL_HEXHEX_13 1, 1, 3, 2, 1, E, 0, 4, 0, 5
#define PPUTLIMPL_HEXHEX_12 1, 0, 3, 3, 1, F, 0, 3, 0, 4
#define PPUTLIMPL_HEXHEX_11 0, 1, 1, 0, 0, 0, 0, 2, 0, 3
#define PPUTLIMPL_HEXHEX_10 0, 0, 1, 1, 0, 1, 0, 1, 0, 2
#define PPUTLIMPL_HEXHEX_0F 1, 0, F, F, 1, 1, 0, F, 1, 0
#define PPUTLIMPL_HEXHEX_0E 1, 0, E, E, 1, 2, 0, E, 0, F
#define PPUTLIMPL_HEXHEX_0D 1, 0, D, D, 1, 3, 0, D, 0, E
#define PPUTLIMPL_HEXHEX_0C 1, 0, C, C, 1, 4, 0, C, 0, D
#define PPUTLIMPL_HEXHEX_0B 1, 0, B, B, 1, 5, 0, B, 0, C
#define PPUTLIMPL_HEXHEX_0A 1, 0, A, A, 1, 6, 0, A, 0, B
#define PPUTLIMPL_HEXHEX_09 1, 0, 9, 9, 1, 7, 0, 9, 0, A
#define PPUTLIMPL_HEXHEX_08 1, 0, 8, 8, 1, 8, 0, 8, 0, 9
#define PPUTLIMPL_HEXHEX_07 1, 0, 7, 7, 1, 9, 0, 7, 0, 8
#define PPUTLIMPL_HEXHEX_06 1, 0, 6, 6, 1, A, 0, 6, 0, 7
#define PPUTLIMPL_HEXHEX_05 1, 0, 5, 5, 1, B, 0, 5, 0, 6
#define PPUTLIMPL_HEXHEX_04 1, 0, 4, 4, 1, C, 0, 4, 0, 5
#define PPUTLIMPL_HEXHEX_03 1, 0, 3, 3, 1, D, 0, 3, 0, 4
#define PPUTLIMPL_HEXHEX_02 1, 0, 2, 2, 1, E, 0, 2, 0, 3
#define PPUTLIMPL_HEXHEX_01 1, 0, 1, 1, 1, F, 0, 1, 0, 2
#define PPUTLIMPL_HEXHEX_00 0, 0, 0, 0, 0, 0, 0, 0, 0, 1

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [impl.traits.nybl]
/// ------------------
/// [internal] nybl traits
#define PPUTLIMPL_NYBL(/* {<atom>, IS}|{<nybl>, HEX|BITS} */ v, t) \
  PPUTLIMPL_NYBL_o(t, PTL_XCAT(PPUTLIMPL_NYBL_, v))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_NYBL_o(t, ...)      PPUTLIMPL_NYBL_##t(__VA_ARGS__)
#define PPUTLIMPL_NYBL_BITS(hex, ...) /* -> ...bool */ __VA_ARGS__
#define PPUTLIMPL_NYBL_HEX(hex, ...)  /* -> hex */ hex
#define PPUTLIMPL_NYBL_IS(_, ...)     /* -> bool */ PPUTLIMPL_NYBL_IS_0##__VA_OPT__(1)
#define PPUTLIMPL_NYBL_IS_01          1
#define PPUTLIMPL_NYBL_IS_0           0
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
#define PPUTLIMPL_UDEC(/* {<atom>, IS}|{<udec>, UHEX|LOG2|SQRT|FACT} */ v, t) \
  PPUTLIMPL_UDEC_o(t, PTL_XCAT(PPUTLIMPL_UDEC_, v))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_UDEC_o(t, ...)          PPUTLIMPL_UDEC_##t(__VA_ARGS__)
#define PPUTLIMPL_UDEC_FACT(u, l, s, f)   /* -> (udec...) */ f
#define PPUTLIMPL_UDEC_SQRT(u, l, s, ...) /* -> udec */ s
#define PPUTLIMPL_UDEC_LOG2(u, l, ...)    /* -> udec */ l
#define PPUTLIMPL_UDEC_UHEX(u, ...)       /* -> uhex */ u
#define PPUTLIMPL_UDEC_IS(_, ...)         /* -> bool */ PPUTLIMPL_UDEC_IS_0##__VA_OPT__(1)
#define PPUTLIMPL_UDEC_IS_01              1
#define PPUTLIMPL_UDEC_IS_0               0

/// UHEX, LOG2, SQRT, FACT
#define PPUTLIMPL_UDEC_4095u 0xFFFu, 11u, 63u, (3u, 3u, 5u, 7u, 13u)
#define PPUTLIMPL_UDEC_4094u 0xFFEu, 11u, 63u, (2u, 23u, 89u)
#define PPUTLIMPL_UDEC_4093u 0xFFDu, 11u, 63u, ()
#define PPUTLIMPL_UDEC_4092u 0xFFCu, 11u, 63u, (2u, 2u, 3u, 11u, 31u)
#define PPUTLIMPL_UDEC_4091u 0xFFBu, 11u, 63u, ()
#define PPUTLIMPL_UDEC_4090u 0xFFAu, 11u, 63u, (2u, 5u, 409u)
#define PPUTLIMPL_UDEC_4089u 0xFF9u, 11u, 63u, (3u, 29u, 47u)
#define PPUTLIMPL_UDEC_4088u 0xFF8u, 11u, 63u, (2u, 2u, 2u, 7u, 73u)
#define PPUTLIMPL_UDEC_4087u 0xFF7u, 11u, 63u, (61u, 67u)
#define PPUTLIMPL_UDEC_4086u 0xFF6u, 11u, 63u, (2u, 3u, 3u, 227u)
#define PPUTLIMPL_UDEC_4085u 0xFF5u, 11u, 63u, (5u, 19u, 43u)
#define PPUTLIMPL_UDEC_4084u 0xFF4u, 11u, 63u, (2u, 2u, 1021u)
#define PPUTLIMPL_UDEC_4083u 0xFF3u, 11u, 63u, (3u, 1361u)
#define PPUTLIMPL_UDEC_4082u 0xFF2u, 11u, 63u, (2u, 13u, 157u)
#define PPUTLIMPL_UDEC_4081u 0xFF1u, 11u, 63u, (7u, 11u, 53u)
#define PPUTLIMPL_UDEC_4080u 0xFF0u, 11u, 63u, (2u, 2u, 2u, 2u, 3u, 5u, 17u)
#define PPUTLIMPL_UDEC_4079u 0xFEFu, 11u, 63u, ()
#define PPUTLIMPL_UDEC_4078u 0xFEEu, 11u, 63u, (2u, 2039u)
#define PPUTLIMPL_UDEC_4077u 0xFEDu, 11u, 63u, (3u, 3u, 3u, 151u)
#define PPUTLIMPL_UDEC_4076u 0xFECu, 11u, 63u, (2u, 2u, 1019u)
#define PPUTLIMPL_UDEC_4075u 0xFEBu, 11u, 63u, (5u, 5u, 163u)
#define PPUTLIMPL_UDEC_4074u 0xFEAu, 11u, 63u, (2u, 3u, 7u, 97u)
#define PPUTLIMPL_UDEC_4073u 0xFE9u, 11u, 63u, ()
#define PPUTLIMPL_UDEC_4072u 0xFE8u, 11u, 63u, (2u, 2u, 2u, 509u)
#define PPUTLIMPL_UDEC_4071u 0xFE7u, 11u, 63u, (3u, 23u, 59u)
#define PPUTLIMPL_UDEC_4070u 0xFE6u, 11u, 63u, (2u, 5u, 11u, 37u)
#define PPUTLIMPL_UDEC_4069u 0xFE5u, 11u, 63u, (13u, 313u)
#define PPUTLIMPL_UDEC_4068u 0xFE4u, 11u, 63u, (2u, 2u, 3u, 3u, 113u)
#define PPUTLIMPL_UDEC_4067u 0xFE3u, 11u, 63u, (7u, 7u, 83u)
#define PPUTLIMPL_UDEC_4066u 0xFE2u, 11u, 63u, (2u, 19u, 107u)
#define PPUTLIMPL_UDEC_4065u 0xFE1u, 11u, 63u, (3u, 5u, 271u)
#define PPUTLIMPL_UDEC_4064u 0xFE0u, 11u, 63u, (2u, 2u, 2u, 2u, 2u, 127u)
#define PPUTLIMPL_UDEC_4063u 0xFDFu, 11u, 63u, (17u, 239u)
#define PPUTLIMPL_UDEC_4062u 0xFDEu, 11u, 63u, (2u, 3u, 677u)
#define PPUTLIMPL_UDEC_4061u 0xFDDu, 11u, 63u, (31u, 131u)
#define PPUTLIMPL_UDEC_4060u 0xFDCu, 11u, 63u, (2u, 2u, 5u, 7u, 29u)
#define PPUTLIMPL_UDEC_4059u 0xFDBu, 11u, 63u, (3u, 3u, 11u, 41u)
#define PPUTLIMPL_UDEC_4058u 0xFDAu, 11u, 63u, (2u, 2029u)
#define PPUTLIMPL_UDEC_4057u 0xFD9u, 11u, 63u, ()
#define PPUTLIMPL_UDEC_4056u 0xFD8u, 11u, 63u, (2u, 2u, 2u, 3u, 13u, 13u)
#define PPUTLIMPL_UDEC_4055u 0xFD7u, 11u, 63u, (5u, 811u)
#define PPUTLIMPL_UDEC_4054u 0xFD6u, 11u, 63u, (2u, 2027u)
#define PPUTLIMPL_UDEC_4053u 0xFD5u, 11u, 63u, (3u, 7u, 193u)
#define PPUTLIMPL_UDEC_4052u 0xFD4u, 11u, 63u, (2u, 2u, 1013u)
#define PPUTLIMPL_UDEC_4051u 0xFD3u, 11u, 63u, ()
#define PPUTLIMPL_UDEC_4050u 0xFD2u, 11u, 63u, (2u, 3u, 3u, 3u, 3u, 5u, 5u)
#define PPUTLIMPL_UDEC_4049u 0xFD1u, 11u, 63u, ()
#define PPUTLIMPL_UDEC_4048u 0xFD0u, 11u, 63u, (2u, 2u, 2u, 2u, 11u, 23u)
#define PPUTLIMPL_UDEC_4047u 0xFCFu, 11u, 63u, (3u, 19u, 71u)
#define PPUTLIMPL_UDEC_4046u 0xFCEu, 11u, 63u, (2u, 7u, 17u, 17u)
#define PPUTLIMPL_UDEC_4045u 0xFCDu, 11u, 63u, (5u, 809u)
#define PPUTLIMPL_UDEC_4044u 0xFCCu, 11u, 63u, (2u, 2u, 3u, 337u)
#define PPUTLIMPL_UDEC_4043u 0xFCBu, 11u, 63u, (13u, 311u)
#define PPUTLIMPL_UDEC_4042u 0xFCAu, 11u, 63u, (2u, 43u, 47u)
#define PPUTLIMPL_UDEC_4041u 0xFC9u, 11u, 63u, (3u, 3u, 449u)
#define PPUTLIMPL_UDEC_4040u 0xFC8u, 11u, 63u, (2u, 2u, 2u, 5u, 101u)
#define PPUTLIMPL_UDEC_4039u 0xFC7u, 11u, 63u, (7u, 577u)
#define PPUTLIMPL_UDEC_4038u 0xFC6u, 11u, 63u, (2u, 3u, 673u)
#define PPUTLIMPL_UDEC_4037u 0xFC5u, 11u, 63u, (11u, 367u)
#define PPUTLIMPL_UDEC_4036u 0xFC4u, 11u, 63u, (2u, 2u, 1009u)
#define PPUTLIMPL_UDEC_4035u 0xFC3u, 11u, 63u, (3u, 5u, 269u)
#define PPUTLIMPL_UDEC_4034u 0xFC2u, 11u, 63u, (2u, 2017u)
#define PPUTLIMPL_UDEC_4033u 0xFC1u, 11u, 63u, (37u, 109u)
#define PPUTLIMPL_UDEC_4032u 0xFC0u, 11u, 63u, (2u, 2u, 2u, 2u, 2u, 2u, 3u, 3u, 7u)
#define PPUTLIMPL_UDEC_4031u 0xFBFu, 11u, 63u, (29u, 139u)
#define PPUTLIMPL_UDEC_4030u 0xFBEu, 11u, 63u, (2u, 5u, 13u, 31u)
#define PPUTLIMPL_UDEC_4029u 0xFBDu, 11u, 63u, (3u, 17u, 79u)
#define PPUTLIMPL_UDEC_4028u 0xFBCu, 11u, 63u, (2u, 2u, 19u, 53u)
#define PPUTLIMPL_UDEC_4027u 0xFBBu, 11u, 63u, ()
#define PPUTLIMPL_UDEC_4026u 0xFBAu, 11u, 63u, (2u, 3u, 11u, 61u)
#define PPUTLIMPL_UDEC_4025u 0xFB9u, 11u, 63u, (5u, 5u, 7u, 23u)
#define PPUTLIMPL_UDEC_4024u 0xFB8u, 11u, 63u, (2u, 2u, 2u, 503u)
#define PPUTLIMPL_UDEC_4023u 0xFB7u, 11u, 63u, (3u, 3u, 3u, 149u)
#define PPUTLIMPL_UDEC_4022u 0xFB6u, 11u, 63u, (2u, 2011u)
#define PPUTLIMPL_UDEC_4021u 0xFB5u, 11u, 63u, ()
#define PPUTLIMPL_UDEC_4020u 0xFB4u, 11u, 63u, (2u, 2u, 3u, 5u, 67u)
#define PPUTLIMPL_UDEC_4019u 0xFB3u, 11u, 63u, ()
#define PPUTLIMPL_UDEC_4018u 0xFB2u, 11u, 63u, (2u, 7u, 7u, 41u)
#define PPUTLIMPL_UDEC_4017u 0xFB1u, 11u, 63u, (3u, 13u, 103u)
#define PPUTLIMPL_UDEC_4016u 0xFB0u, 11u, 63u, (2u, 2u, 2u, 2u, 251u)
#define PPUTLIMPL_UDEC_4015u 0xFAFu, 11u, 63u, (5u, 11u, 73u)
#define PPUTLIMPL_UDEC_4014u 0xFAEu, 11u, 63u, (2u, 3u, 3u, 223u)
#define PPUTLIMPL_UDEC_4013u 0xFADu, 11u, 63u, ()
#define PPUTLIMPL_UDEC_4012u 0xFACu, 11u, 63u, (2u, 2u, 17u, 59u)
#define PPUTLIMPL_UDEC_4011u 0xFABu, 11u, 63u, (3u, 7u, 191u)
#define PPUTLIMPL_UDEC_4010u 0xFAAu, 11u, 63u, (2u, 5u, 401u)
#define PPUTLIMPL_UDEC_4009u 0xFA9u, 11u, 63u, (19u, 211u)
#define PPUTLIMPL_UDEC_4008u 0xFA8u, 11u, 63u, (2u, 2u, 2u, 3u, 167u)
#define PPUTLIMPL_UDEC_4007u 0xFA7u, 11u, 63u, ()
#define PPUTLIMPL_UDEC_4006u 0xFA6u, 11u, 63u, (2u, 2003u)
#define PPUTLIMPL_UDEC_4005u 0xFA5u, 11u, 63u, (3u, 3u, 5u, 89u)
#define PPUTLIMPL_UDEC_4004u 0xFA4u, 11u, 63u, (2u, 2u, 7u, 11u, 13u)
#define PPUTLIMPL_UDEC_4003u 0xFA3u, 11u, 63u, ()
#define PPUTLIMPL_UDEC_4002u 0xFA2u, 11u, 63u, (2u, 3u, 23u, 29u)
#define PPUTLIMPL_UDEC_4001u 0xFA1u, 11u, 63u, ()
#define PPUTLIMPL_UDEC_4000u 0xFA0u, 11u, 63u, (2u, 2u, 2u, 2u, 2u, 5u, 5u, 5u)
#define PPUTLIMPL_UDEC_3999u 0xF9Fu, 11u, 63u, (3u, 31u, 43u)
#define PPUTLIMPL_UDEC_3998u 0xF9Eu, 11u, 63u, (2u, 1999u)
#define PPUTLIMPL_UDEC_3997u 0xF9Du, 11u, 63u, (7u, 571u)
#define PPUTLIMPL_UDEC_3996u 0xF9Cu, 11u, 63u, (2u, 2u, 3u, 3u, 3u, 37u)
#define PPUTLIMPL_UDEC_3995u 0xF9Bu, 11u, 63u, (5u, 17u, 47u)
#define PPUTLIMPL_UDEC_3994u 0xF9Au, 11u, 63u, (2u, 1997u)
#define PPUTLIMPL_UDEC_3993u 0xF99u, 11u, 63u, (3u, 11u, 11u, 11u)
#define PPUTLIMPL_UDEC_3992u 0xF98u, 11u, 63u, (2u, 2u, 2u, 499u)
#define PPUTLIMPL_UDEC_3991u 0xF97u, 11u, 63u, (13u, 307u)
#define PPUTLIMPL_UDEC_3990u 0xF96u, 11u, 63u, (2u, 3u, 5u, 7u, 19u)
#define PPUTLIMPL_UDEC_3989u 0xF95u, 11u, 63u, ()
#define PPUTLIMPL_UDEC_3988u 0xF94u, 11u, 63u, (2u, 2u, 997u)
#define PPUTLIMPL_UDEC_3987u 0xF93u, 11u, 63u, (3u, 3u, 443u)
#define PPUTLIMPL_UDEC_3986u 0xF92u, 11u, 63u, (2u, 1993u)
#define PPUTLIMPL_UDEC_3985u 0xF91u, 11u, 63u, (5u, 797u)
#define PPUTLIMPL_UDEC_3984u 0xF90u, 11u, 63u, (2u, 2u, 2u, 2u, 3u, 83u)
#define PPUTLIMPL_UDEC_3983u 0xF8Fu, 11u, 63u, (7u, 569u)
#define PPUTLIMPL_UDEC_3982u 0xF8Eu, 11u, 63u, (2u, 11u, 181u)
#define PPUTLIMPL_UDEC_3981u 0xF8Du, 11u, 63u, (3u, 1327u)
#define PPUTLIMPL_UDEC_3980u 0xF8Cu, 11u, 63u, (2u, 2u, 5u, 199u)
#define PPUTLIMPL_UDEC_3979u 0xF8Bu, 11u, 63u, (23u, 173u)
#define PPUTLIMPL_UDEC_3978u 0xF8Au, 11u, 63u, (2u, 3u, 3u, 13u, 17u)
#define PPUTLIMPL_UDEC_3977u 0xF89u, 11u, 63u, (41u, 97u)
#define PPUTLIMPL_UDEC_3976u 0xF88u, 11u, 63u, (2u, 2u, 2u, 7u, 71u)
#define PPUTLIMPL_UDEC_3975u 0xF87u, 11u, 63u, (3u, 5u, 5u, 53u)
#define PPUTLIMPL_UDEC_3974u 0xF86u, 11u, 63u, (2u, 1987u)
#define PPUTLIMPL_UDEC_3973u 0xF85u, 11u, 63u, (29u, 137u)
#define PPUTLIMPL_UDEC_3972u 0xF84u, 11u, 63u, (2u, 2u, 3u, 331u)
#define PPUTLIMPL_UDEC_3971u 0xF83u, 11u, 63u, (11u, 19u, 19u)
#define PPUTLIMPL_UDEC_3970u 0xF82u, 11u, 63u, (2u, 5u, 397u)
#define PPUTLIMPL_UDEC_3969u 0xF81u, 11u, 63u, (3u, 3u, 3u, 3u, 7u, 7u)
#define PPUTLIMPL_UDEC_3968u 0xF80u, 11u, 62u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 31u)
#define PPUTLIMPL_UDEC_3967u 0xF7Fu, 11u, 62u, ()
#define PPUTLIMPL_UDEC_3966u 0xF7Eu, 11u, 62u, (2u, 3u, 661u)
#define PPUTLIMPL_UDEC_3965u 0xF7Du, 11u, 62u, (5u, 13u, 61u)
#define PPUTLIMPL_UDEC_3964u 0xF7Cu, 11u, 62u, (2u, 2u, 991u)
#define PPUTLIMPL_UDEC_3963u 0xF7Bu, 11u, 62u, (3u, 1321u)
#define PPUTLIMPL_UDEC_3962u 0xF7Au, 11u, 62u, (2u, 7u, 283u)
#define PPUTLIMPL_UDEC_3961u 0xF79u, 11u, 62u, (17u, 233u)
#define PPUTLIMPL_UDEC_3960u 0xF78u, 11u, 62u, (2u, 2u, 2u, 3u, 3u, 5u, 11u)
#define PPUTLIMPL_UDEC_3959u 0xF77u, 11u, 62u, (37u, 107u)
#define PPUTLIMPL_UDEC_3958u 0xF76u, 11u, 62u, (2u, 1979u)
#define PPUTLIMPL_UDEC_3957u 0xF75u, 11u, 62u, (3u, 1319u)
#define PPUTLIMPL_UDEC_3956u 0xF74u, 11u, 62u, (2u, 2u, 23u, 43u)
#define PPUTLIMPL_UDEC_3955u 0xF73u, 11u, 62u, (5u, 7u, 113u)
#define PPUTLIMPL_UDEC_3954u 0xF72u, 11u, 62u, (2u, 3u, 659u)
#define PPUTLIMPL_UDEC_3953u 0xF71u, 11u, 62u, (59u, 67u)
#define PPUTLIMPL_UDEC_3952u 0xF70u, 11u, 62u, (2u, 2u, 2u, 2u, 13u, 19u)
#define PPUTLIMPL_UDEC_3951u 0xF6Fu, 11u, 62u, (3u, 3u, 439u)
#define PPUTLIMPL_UDEC_3950u 0xF6Eu, 11u, 62u, (2u, 5u, 5u, 79u)
#define PPUTLIMPL_UDEC_3949u 0xF6Du, 11u, 62u, (11u, 359u)
#define PPUTLIMPL_UDEC_3948u 0xF6Cu, 11u, 62u, (2u, 2u, 3u, 7u, 47u)
#define PPUTLIMPL_UDEC_3947u 0xF6Bu, 11u, 62u, ()
#define PPUTLIMPL_UDEC_3946u 0xF6Au, 11u, 62u, (2u, 1973u)
#define PPUTLIMPL_UDEC_3945u 0xF69u, 11u, 62u, (3u, 5u, 263u)
#define PPUTLIMPL_UDEC_3944u 0xF68u, 11u, 62u, (2u, 2u, 2u, 17u, 29u)
#define PPUTLIMPL_UDEC_3943u 0xF67u, 11u, 62u, ()
#define PPUTLIMPL_UDEC_3942u 0xF66u, 11u, 62u, (2u, 3u, 3u, 3u, 73u)
#define PPUTLIMPL_UDEC_3941u 0xF65u, 11u, 62u, (7u, 563u)
#define PPUTLIMPL_UDEC_3940u 0xF64u, 11u, 62u, (2u, 2u, 5u, 197u)
#define PPUTLIMPL_UDEC_3939u 0xF63u, 11u, 62u, (3u, 13u, 101u)
#define PPUTLIMPL_UDEC_3938u 0xF62u, 11u, 62u, (2u, 11u, 179u)
#define PPUTLIMPL_UDEC_3937u 0xF61u, 11u, 62u, (31u, 127u)
#define PPUTLIMPL_UDEC_3936u 0xF60u, 11u, 62u, (2u, 2u, 2u, 2u, 2u, 3u, 41u)
#define PPUTLIMPL_UDEC_3935u 0xF5Fu, 11u, 62u, (5u, 787u)
#define PPUTLIMPL_UDEC_3934u 0xF5Eu, 11u, 62u, (2u, 7u, 281u)
#define PPUTLIMPL_UDEC_3933u 0xF5Du, 11u, 62u, (3u, 3u, 19u, 23u)
#define PPUTLIMPL_UDEC_3932u 0xF5Cu, 11u, 62u, (2u, 2u, 983u)
#define PPUTLIMPL_UDEC_3931u 0xF5Bu, 11u, 62u, ()
#define PPUTLIMPL_UDEC_3930u 0xF5Au, 11u, 62u, (2u, 3u, 5u, 131u)
#define PPUTLIMPL_UDEC_3929u 0xF59u, 11u, 62u, ()
#define PPUTLIMPL_UDEC_3928u 0xF58u, 11u, 62u, (2u, 2u, 2u, 491u)
#define PPUTLIMPL_UDEC_3927u 0xF57u, 11u, 62u, (3u, 7u, 11u, 17u)
#define PPUTLIMPL_UDEC_3926u 0xF56u, 11u, 62u, (2u, 13u, 151u)
#define PPUTLIMPL_UDEC_3925u 0xF55u, 11u, 62u, (5u, 5u, 157u)
#define PPUTLIMPL_UDEC_3924u 0xF54u, 11u, 62u, (2u, 2u, 3u, 3u, 109u)
#define PPUTLIMPL_UDEC_3923u 0xF53u, 11u, 62u, ()
#define PPUTLIMPL_UDEC_3922u 0xF52u, 11u, 62u, (2u, 37u, 53u)
#define PPUTLIMPL_UDEC_3921u 0xF51u, 11u, 62u, (3u, 1307u)
#define PPUTLIMPL_UDEC_3920u 0xF50u, 11u, 62u, (2u, 2u, 2u, 2u, 5u, 7u, 7u)
#define PPUTLIMPL_UDEC_3919u 0xF4Fu, 11u, 62u, ()
#define PPUTLIMPL_UDEC_3918u 0xF4Eu, 11u, 62u, (2u, 3u, 653u)
#define PPUTLIMPL_UDEC_3917u 0xF4Du, 11u, 62u, ()
#define PPUTLIMPL_UDEC_3916u 0xF4Cu, 11u, 62u, (2u, 2u, 11u, 89u)
#define PPUTLIMPL_UDEC_3915u 0xF4Bu, 11u, 62u, (3u, 3u, 3u, 5u, 29u)
#define PPUTLIMPL_UDEC_3914u 0xF4Au, 11u, 62u, (2u, 19u, 103u)
#define PPUTLIMPL_UDEC_3913u 0xF49u, 11u, 62u, (7u, 13u, 43u)
#define PPUTLIMPL_UDEC_3912u 0xF48u, 11u, 62u, (2u, 2u, 2u, 3u, 163u)
#define PPUTLIMPL_UDEC_3911u 0xF47u, 11u, 62u, ()
#define PPUTLIMPL_UDEC_3910u 0xF46u, 11u, 62u, (2u, 5u, 17u, 23u)
#define PPUTLIMPL_UDEC_3909u 0xF45u, 11u, 62u, (3u, 1303u)
#define PPUTLIMPL_UDEC_3908u 0xF44u, 11u, 62u, (2u, 2u, 977u)
#define PPUTLIMPL_UDEC_3907u 0xF43u, 11u, 62u, ()
#define PPUTLIMPL_UDEC_3906u 0xF42u, 11u, 62u, (2u, 3u, 3u, 7u, 31u)
#define PPUTLIMPL_UDEC_3905u 0xF41u, 11u, 62u, (5u, 11u, 71u)
#define PPUTLIMPL_UDEC_3904u 0xF40u, 11u, 62u, (2u, 2u, 2u, 2u, 2u, 2u, 61u)
#define PPUTLIMPL_UDEC_3903u 0xF3Fu, 11u, 62u, (3u, 1301u)
#define PPUTLIMPL_UDEC_3902u 0xF3Eu, 11u, 62u, (2u, 1951u)
#define PPUTLIMPL_UDEC_3901u 0xF3Du, 11u, 62u, (47u, 83u)
#define PPUTLIMPL_UDEC_3900u 0xF3Cu, 11u, 62u, (2u, 2u, 3u, 5u, 5u, 13u)
#define PPUTLIMPL_UDEC_3899u 0xF3Bu, 11u, 62u, (7u, 557u)
#define PPUTLIMPL_UDEC_3898u 0xF3Au, 11u, 62u, (2u, 1949u)
#define PPUTLIMPL_UDEC_3897u 0xF39u, 11u, 62u, (3u, 3u, 433u)
#define PPUTLIMPL_UDEC_3896u 0xF38u, 11u, 62u, (2u, 2u, 2u, 487u)
#define PPUTLIMPL_UDEC_3895u 0xF37u, 11u, 62u, (5u, 19u, 41u)
#define PPUTLIMPL_UDEC_3894u 0xF36u, 11u, 62u, (2u, 3u, 11u, 59u)
#define PPUTLIMPL_UDEC_3893u 0xF35u, 11u, 62u, (17u, 229u)
#define PPUTLIMPL_UDEC_3892u 0xF34u, 11u, 62u, (2u, 2u, 7u, 139u)
#define PPUTLIMPL_UDEC_3891u 0xF33u, 11u, 62u, (3u, 1297u)
#define PPUTLIMPL_UDEC_3890u 0xF32u, 11u, 62u, (2u, 5u, 389u)
#define PPUTLIMPL_UDEC_3889u 0xF31u, 11u, 62u, ()
#define PPUTLIMPL_UDEC_3888u 0xF30u, 11u, 62u, (2u, 2u, 2u, 2u, 3u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UDEC_3887u 0xF2Fu, 11u, 62u, (13u, 13u, 23u)
#define PPUTLIMPL_UDEC_3886u 0xF2Eu, 11u, 62u, (2u, 29u, 67u)
#define PPUTLIMPL_UDEC_3885u 0xF2Du, 11u, 62u, (3u, 5u, 7u, 37u)
#define PPUTLIMPL_UDEC_3884u 0xF2Cu, 11u, 62u, (2u, 2u, 971u)
#define PPUTLIMPL_UDEC_3883u 0xF2Bu, 11u, 62u, (11u, 353u)
#define PPUTLIMPL_UDEC_3882u 0xF2Au, 11u, 62u, (2u, 3u, 647u)
#define PPUTLIMPL_UDEC_3881u 0xF29u, 11u, 62u, ()
#define PPUTLIMPL_UDEC_3880u 0xF28u, 11u, 62u, (2u, 2u, 2u, 5u, 97u)
#define PPUTLIMPL_UDEC_3879u 0xF27u, 11u, 62u, (3u, 3u, 431u)
#define PPUTLIMPL_UDEC_3878u 0xF26u, 11u, 62u, (2u, 7u, 277u)
#define PPUTLIMPL_UDEC_3877u 0xF25u, 11u, 62u, ()
#define PPUTLIMPL_UDEC_3876u 0xF24u, 11u, 62u, (2u, 2u, 3u, 17u, 19u)
#define PPUTLIMPL_UDEC_3875u 0xF23u, 11u, 62u, (5u, 5u, 5u, 31u)
#define PPUTLIMPL_UDEC_3874u 0xF22u, 11u, 62u, (2u, 13u, 149u)
#define PPUTLIMPL_UDEC_3873u 0xF21u, 11u, 62u, (3u, 1291u)
#define PPUTLIMPL_UDEC_3872u 0xF20u, 11u, 62u, (2u, 2u, 2u, 2u, 2u, 11u, 11u)
#define PPUTLIMPL_UDEC_3871u 0xF1Fu, 11u, 62u, (7u, 7u, 79u)
#define PPUTLIMPL_UDEC_3870u 0xF1Eu, 11u, 62u, (2u, 3u, 3u, 5u, 43u)
#define PPUTLIMPL_UDEC_3869u 0xF1Du, 11u, 62u, (53u, 73u)
#define PPUTLIMPL_UDEC_3868u 0xF1Cu, 11u, 62u, (2u, 2u, 967u)
#define PPUTLIMPL_UDEC_3867u 0xF1Bu, 11u, 62u, (3u, 1289u)
#define PPUTLIMPL_UDEC_3866u 0xF1Au, 11u, 62u, (2u, 1933u)
#define PPUTLIMPL_UDEC_3865u 0xF19u, 11u, 62u, (5u, 773u)
#define PPUTLIMPL_UDEC_3864u 0xF18u, 11u, 62u, (2u, 2u, 2u, 3u, 7u, 23u)
#define PPUTLIMPL_UDEC_3863u 0xF17u, 11u, 62u, ()
#define PPUTLIMPL_UDEC_3862u 0xF16u, 11u, 62u, (2u, 1931u)
#define PPUTLIMPL_UDEC_3861u 0xF15u, 11u, 62u, (3u, 3u, 3u, 11u, 13u)
#define PPUTLIMPL_UDEC_3860u 0xF14u, 11u, 62u, (2u, 2u, 5u, 193u)
#define PPUTLIMPL_UDEC_3859u 0xF13u, 11u, 62u, (17u, 227u)
#define PPUTLIMPL_UDEC_3858u 0xF12u, 11u, 62u, (2u, 3u, 643u)
#define PPUTLIMPL_UDEC_3857u 0xF11u, 11u, 62u, (7u, 19u, 29u)
#define PPUTLIMPL_UDEC_3856u 0xF10u, 11u, 62u, (2u, 2u, 2u, 2u, 241u)
#define PPUTLIMPL_UDEC_3855u 0xF0Fu, 11u, 62u, (3u, 5u, 257u)
#define PPUTLIMPL_UDEC_3854u 0xF0Eu, 11u, 62u, (2u, 41u, 47u)
#define PPUTLIMPL_UDEC_3853u 0xF0Du, 11u, 62u, ()
#define PPUTLIMPL_UDEC_3852u 0xF0Cu, 11u, 62u, (2u, 2u, 3u, 3u, 107u)
#define PPUTLIMPL_UDEC_3851u 0xF0Bu, 11u, 62u, ()
#define PPUTLIMPL_UDEC_3850u 0xF0Au, 11u, 62u, (2u, 5u, 5u, 7u, 11u)
#define PPUTLIMPL_UDEC_3849u 0xF09u, 11u, 62u, (3u, 1283u)
#define PPUTLIMPL_UDEC_3848u 0xF08u, 11u, 62u, (2u, 2u, 2u, 13u, 37u)
#define PPUTLIMPL_UDEC_3847u 0xF07u, 11u, 62u, ()
#define PPUTLIMPL_UDEC_3846u 0xF06u, 11u, 62u, (2u, 3u, 641u)
#define PPUTLIMPL_UDEC_3845u 0xF05u, 11u, 62u, (5u, 769u)
#define PPUTLIMPL_UDEC_3844u 0xF04u, 11u, 62u, (2u, 2u, 31u, 31u)
#define PPUTLIMPL_UDEC_3843u 0xF03u, 11u, 61u, (3u, 3u, 7u, 61u)
#define PPUTLIMPL_UDEC_3842u 0xF02u, 11u, 61u, (2u, 17u, 113u)
#define PPUTLIMPL_UDEC_3841u 0xF01u, 11u, 61u, (23u, 167u)
#define PPUTLIMPL_UDEC_3840u 0xF00u, 11u, 61u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 3u, 5u)
#define PPUTLIMPL_UDEC_3839u 0xEFFu, 11u, 61u, (11u, 349u)
#define PPUTLIMPL_UDEC_3838u 0xEFEu, 11u, 61u, (2u, 19u, 101u)
#define PPUTLIMPL_UDEC_3837u 0xEFDu, 11u, 61u, (3u, 1279u)
#define PPUTLIMPL_UDEC_3836u 0xEFCu, 11u, 61u, (2u, 2u, 7u, 137u)
#define PPUTLIMPL_UDEC_3835u 0xEFBu, 11u, 61u, (5u, 13u, 59u)
#define PPUTLIMPL_UDEC_3834u 0xEFAu, 11u, 61u, (2u, 3u, 3u, 3u, 71u)
#define PPUTLIMPL_UDEC_3833u 0xEF9u, 11u, 61u, ()
#define PPUTLIMPL_UDEC_3832u 0xEF8u, 11u, 61u, (2u, 2u, 2u, 479u)
#define PPUTLIMPL_UDEC_3831u 0xEF7u, 11u, 61u, (3u, 1277u)
#define PPUTLIMPL_UDEC_3830u 0xEF6u, 11u, 61u, (2u, 5u, 383u)
#define PPUTLIMPL_UDEC_3829u 0xEF5u, 11u, 61u, (7u, 547u)
#define PPUTLIMPL_UDEC_3828u 0xEF4u, 11u, 61u, (2u, 2u, 3u, 11u, 29u)
#define PPUTLIMPL_UDEC_3827u 0xEF3u, 11u, 61u, (43u, 89u)
#define PPUTLIMPL_UDEC_3826u 0xEF2u, 11u, 61u, (2u, 1913u)
#define PPUTLIMPL_UDEC_3825u 0xEF1u, 11u, 61u, (3u, 3u, 5u, 5u, 17u)
#define PPUTLIMPL_UDEC_3824u 0xEF0u, 11u, 61u, (2u, 2u, 2u, 2u, 239u)
#define PPUTLIMPL_UDEC_3823u 0xEEFu, 11u, 61u, ()
#define PPUTLIMPL_UDEC_3822u 0xEEEu, 11u, 61u, (2u, 3u, 7u, 7u, 13u)
#define PPUTLIMPL_UDEC_3821u 0xEEDu, 11u, 61u, ()
#define PPUTLIMPL_UDEC_3820u 0xEECu, 11u, 61u, (2u, 2u, 5u, 191u)
#define PPUTLIMPL_UDEC_3819u 0xEEBu, 11u, 61u, (3u, 19u, 67u)
#define PPUTLIMPL_UDEC_3818u 0xEEAu, 11u, 61u, (2u, 23u, 83u)
#define PPUTLIMPL_UDEC_3817u 0xEE9u, 11u, 61u, (11u, 347u)
#define PPUTLIMPL_UDEC_3816u 0xEE8u, 11u, 61u, (2u, 2u, 2u, 3u, 3u, 53u)
#define PPUTLIMPL_UDEC_3815u 0xEE7u, 11u, 61u, (5u, 7u, 109u)
#define PPUTLIMPL_UDEC_3814u 0xEE6u, 11u, 61u, (2u, 1907u)
#define PPUTLIMPL_UDEC_3813u 0xEE5u, 11u, 61u, (3u, 31u, 41u)
#define PPUTLIMPL_UDEC_3812u 0xEE4u, 11u, 61u, (2u, 2u, 953u)
#define PPUTLIMPL_UDEC_3811u 0xEE3u, 11u, 61u, (37u, 103u)
#define PPUTLIMPL_UDEC_3810u 0xEE2u, 11u, 61u, (2u, 3u, 5u, 127u)
#define PPUTLIMPL_UDEC_3809u 0xEE1u, 11u, 61u, (13u, 293u)
#define PPUTLIMPL_UDEC_3808u 0xEE0u, 11u, 61u, (2u, 2u, 2u, 2u, 2u, 7u, 17u)
#define PPUTLIMPL_UDEC_3807u 0xEDFu, 11u, 61u, (3u, 3u, 3u, 3u, 47u)
#define PPUTLIMPL_UDEC_3806u 0xEDEu, 11u, 61u, (2u, 11u, 173u)
#define PPUTLIMPL_UDEC_3805u 0xEDDu, 11u, 61u, (5u, 761u)
#define PPUTLIMPL_UDEC_3804u 0xEDCu, 11u, 61u, (2u, 2u, 3u, 317u)
#define PPUTLIMPL_UDEC_3803u 0xEDBu, 11u, 61u, ()
#define PPUTLIMPL_UDEC_3802u 0xEDAu, 11u, 61u, (2u, 1901u)
#define PPUTLIMPL_UDEC_3801u 0xED9u, 11u, 61u, (3u, 7u, 181u)
#define PPUTLIMPL_UDEC_3800u 0xED8u, 11u, 61u, (2u, 2u, 2u, 5u, 5u, 19u)
#define PPUTLIMPL_UDEC_3799u 0xED7u, 11u, 61u, (29u, 131u)
#define PPUTLIMPL_UDEC_3798u 0xED6u, 11u, 61u, (2u, 3u, 3u, 211u)
#define PPUTLIMPL_UDEC_3797u 0xED5u, 11u, 61u, ()
#define PPUTLIMPL_UDEC_3796u 0xED4u, 11u, 61u, (2u, 2u, 13u, 73u)
#define PPUTLIMPL_UDEC_3795u 0xED3u, 11u, 61u, (3u, 5u, 11u, 23u)
#define PPUTLIMPL_UDEC_3794u 0xED2u, 11u, 61u, (2u, 7u, 271u)
#define PPUTLIMPL_UDEC_3793u 0xED1u, 11u, 61u, ()
#define PPUTLIMPL_UDEC_3792u 0xED0u, 11u, 61u, (2u, 2u, 2u, 2u, 3u, 79u)
#define PPUTLIMPL_UDEC_3791u 0xECFu, 11u, 61u, (17u, 223u)
#define PPUTLIMPL_UDEC_3790u 0xECEu, 11u, 61u, (2u, 5u, 379u)
#define PPUTLIMPL_UDEC_3789u 0xECDu, 11u, 61u, (3u, 3u, 421u)
#define PPUTLIMPL_UDEC_3788u 0xECCu, 11u, 61u, (2u, 2u, 947u)
#define PPUTLIMPL_UDEC_3787u 0xECBu, 11u, 61u, (7u, 541u)
#define PPUTLIMPL_UDEC_3786u 0xECAu, 11u, 61u, (2u, 3u, 631u)
#define PPUTLIMPL_UDEC_3785u 0xEC9u, 11u, 61u, (5u, 757u)
#define PPUTLIMPL_UDEC_3784u 0xEC8u, 11u, 61u, (2u, 2u, 2u, 11u, 43u)
#define PPUTLIMPL_UDEC_3783u 0xEC7u, 11u, 61u, (3u, 13u, 97u)
#define PPUTLIMPL_UDEC_3782u 0xEC6u, 11u, 61u, (2u, 31u, 61u)
#define PPUTLIMPL_UDEC_3781u 0xEC5u, 11u, 61u, (19u, 199u)
#define PPUTLIMPL_UDEC_3780u 0xEC4u, 11u, 61u, (2u, 2u, 3u, 3u, 3u, 5u, 7u)
#define PPUTLIMPL_UDEC_3779u 0xEC3u, 11u, 61u, ()
#define PPUTLIMPL_UDEC_3778u 0xEC2u, 11u, 61u, (2u, 1889u)
#define PPUTLIMPL_UDEC_3777u 0xEC1u, 11u, 61u, (3u, 1259u)
#define PPUTLIMPL_UDEC_3776u 0xEC0u, 11u, 61u, (2u, 2u, 2u, 2u, 2u, 2u, 59u)
#define PPUTLIMPL_UDEC_3775u 0xEBFu, 11u, 61u, (5u, 5u, 151u)
#define PPUTLIMPL_UDEC_3774u 0xEBEu, 11u, 61u, (2u, 3u, 17u, 37u)
#define PPUTLIMPL_UDEC_3773u 0xEBDu, 11u, 61u, (7u, 7u, 7u, 11u)
#define PPUTLIMPL_UDEC_3772u 0xEBCu, 11u, 61u, (2u, 2u, 23u, 41u)
#define PPUTLIMPL_UDEC_3771u 0xEBBu, 11u, 61u, (3u, 3u, 419u)
#define PPUTLIMPL_UDEC_3770u 0xEBAu, 11u, 61u, (2u, 5u, 13u, 29u)
#define PPUTLIMPL_UDEC_3769u 0xEB9u, 11u, 61u, ()
#define PPUTLIMPL_UDEC_3768u 0xEB8u, 11u, 61u, (2u, 2u, 2u, 3u, 157u)
#define PPUTLIMPL_UDEC_3767u 0xEB7u, 11u, 61u, ()
#define PPUTLIMPL_UDEC_3766u 0xEB6u, 11u, 61u, (2u, 7u, 269u)
#define PPUTLIMPL_UDEC_3765u 0xEB5u, 11u, 61u, (3u, 5u, 251u)
#define PPUTLIMPL_UDEC_3764u 0xEB4u, 11u, 61u, (2u, 2u, 941u)
#define PPUTLIMPL_UDEC_3763u 0xEB3u, 11u, 61u, (53u, 71u)
#define PPUTLIMPL_UDEC_3762u 0xEB2u, 11u, 61u, (2u, 3u, 3u, 11u, 19u)
#define PPUTLIMPL_UDEC_3761u 0xEB1u, 11u, 61u, ()
#define PPUTLIMPL_UDEC_3760u 0xEB0u, 11u, 61u, (2u, 2u, 2u, 2u, 5u, 47u)
#define PPUTLIMPL_UDEC_3759u 0xEAFu, 11u, 61u, (3u, 7u, 179u)
#define PPUTLIMPL_UDEC_3758u 0xEAEu, 11u, 61u, (2u, 1879u)
#define PPUTLIMPL_UDEC_3757u 0xEADu, 11u, 61u, (13u, 17u, 17u)
#define PPUTLIMPL_UDEC_3756u 0xEACu, 11u, 61u, (2u, 2u, 3u, 313u)
#define PPUTLIMPL_UDEC_3755u 0xEABu, 11u, 61u, (5u, 751u)
#define PPUTLIMPL_UDEC_3754u 0xEAAu, 11u, 61u, (2u, 1877u)
#define PPUTLIMPL_UDEC_3753u 0xEA9u, 11u, 61u, (3u, 3u, 3u, 139u)
#define PPUTLIMPL_UDEC_3752u 0xEA8u, 11u, 61u, (2u, 2u, 2u, 7u, 67u)
#define PPUTLIMPL_UDEC_3751u 0xEA7u, 11u, 61u, (11u, 11u, 31u)
#define PPUTLIMPL_UDEC_3750u 0xEA6u, 11u, 61u, (2u, 3u, 5u, 5u, 5u, 5u)
#define PPUTLIMPL_UDEC_3749u 0xEA5u, 11u, 61u, (23u, 163u)
#define PPUTLIMPL_UDEC_3748u 0xEA4u, 11u, 61u, (2u, 2u, 937u)
#define PPUTLIMPL_UDEC_3747u 0xEA3u, 11u, 61u, (3u, 1249u)
#define PPUTLIMPL_UDEC_3746u 0xEA2u, 11u, 61u, (2u, 1873u)
#define PPUTLIMPL_UDEC_3745u 0xEA1u, 11u, 61u, (5u, 7u, 107u)
#define PPUTLIMPL_UDEC_3744u 0xEA0u, 11u, 61u, (2u, 2u, 2u, 2u, 2u, 3u, 3u, 13u)
#define PPUTLIMPL_UDEC_3743u 0xE9Fu, 11u, 61u, (19u, 197u)
#define PPUTLIMPL_UDEC_3742u 0xE9Eu, 11u, 61u, (2u, 1871u)
#define PPUTLIMPL_UDEC_3741u 0xE9Du, 11u, 61u, (3u, 29u, 43u)
#define PPUTLIMPL_UDEC_3740u 0xE9Cu, 11u, 61u, (2u, 2u, 5u, 11u, 17u)
#define PPUTLIMPL_UDEC_3739u 0xE9Bu, 11u, 61u, ()
#define PPUTLIMPL_UDEC_3738u 0xE9Au, 11u, 61u, (2u, 3u, 7u, 89u)
#define PPUTLIMPL_UDEC_3737u 0xE99u, 11u, 61u, (37u, 101u)
#define PPUTLIMPL_UDEC_3736u 0xE98u, 11u, 61u, (2u, 2u, 2u, 467u)
#define PPUTLIMPL_UDEC_3735u 0xE97u, 11u, 61u, (3u, 3u, 5u, 83u)
#define PPUTLIMPL_UDEC_3734u 0xE96u, 11u, 61u, (2u, 1867u)
#define PPUTLIMPL_UDEC_3733u 0xE95u, 11u, 61u, ()
#define PPUTLIMPL_UDEC_3732u 0xE94u, 11u, 61u, (2u, 2u, 3u, 311u)
#define PPUTLIMPL_UDEC_3731u 0xE93u, 11u, 61u, (7u, 13u, 41u)
#define PPUTLIMPL_UDEC_3730u 0xE92u, 11u, 61u, (2u, 5u, 373u)
#define PPUTLIMPL_UDEC_3729u 0xE91u, 11u, 61u, (3u, 11u, 113u)
#define PPUTLIMPL_UDEC_3728u 0xE90u, 11u, 61u, (2u, 2u, 2u, 2u, 233u)
#define PPUTLIMPL_UDEC_3727u 0xE8Fu, 11u, 61u, ()
#define PPUTLIMPL_UDEC_3726u 0xE8Eu, 11u, 61u, (2u, 3u, 3u, 3u, 3u, 23u)
#define PPUTLIMPL_UDEC_3725u 0xE8Du, 11u, 61u, (5u, 5u, 149u)
#define PPUTLIMPL_UDEC_3724u 0xE8Cu, 11u, 61u, (2u, 2u, 7u, 7u, 19u)
#define PPUTLIMPL_UDEC_3723u 0xE8Bu, 11u, 61u, (3u, 17u, 73u)
#define PPUTLIMPL_UDEC_3722u 0xE8Au, 11u, 61u, (2u, 1861u)
#define PPUTLIMPL_UDEC_3721u 0xE89u, 11u, 61u, (61u, 61u)
#define PPUTLIMPL_UDEC_3720u 0xE88u, 11u, 60u, (2u, 2u, 2u, 3u, 5u, 31u)
#define PPUTLIMPL_UDEC_3719u 0xE87u, 11u, 60u, ()
#define PPUTLIMPL_UDEC_3718u 0xE86u, 11u, 60u, (2u, 11u, 13u, 13u)
#define PPUTLIMPL_UDEC_3717u 0xE85u, 11u, 60u, (3u, 3u, 7u, 59u)
#define PPUTLIMPL_UDEC_3716u 0xE84u, 11u, 60u, (2u, 2u, 929u)
#define PPUTLIMPL_UDEC_3715u 0xE83u, 11u, 60u, (5u, 743u)
#define PPUTLIMPL_UDEC_3714u 0xE82u, 11u, 60u, (2u, 3u, 619u)
#define PPUTLIMPL_UDEC_3713u 0xE81u, 11u, 60u, (47u, 79u)
#define PPUTLIMPL_UDEC_3712u 0xE80u, 11u, 60u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 29u)
#define PPUTLIMPL_UDEC_3711u 0xE7Fu, 11u, 60u, (3u, 1237u)
#define PPUTLIMPL_UDEC_3710u 0xE7Eu, 11u, 60u, (2u, 5u, 7u, 53u)
#define PPUTLIMPL_UDEC_3709u 0xE7Du, 11u, 60u, ()
#define PPUTLIMPL_UDEC_3708u 0xE7Cu, 11u, 60u, (2u, 2u, 3u, 3u, 103u)
#define PPUTLIMPL_UDEC_3707u 0xE7Bu, 11u, 60u, (11u, 337u)
#define PPUTLIMPL_UDEC_3706u 0xE7Au, 11u, 60u, (2u, 17u, 109u)
#define PPUTLIMPL_UDEC_3705u 0xE79u, 11u, 60u, (3u, 5u, 13u, 19u)
#define PPUTLIMPL_UDEC_3704u 0xE78u, 11u, 60u, (2u, 2u, 2u, 463u)
#define PPUTLIMPL_UDEC_3703u 0xE77u, 11u, 60u, (7u, 23u, 23u)
#define PPUTLIMPL_UDEC_3702u 0xE76u, 11u, 60u, (2u, 3u, 617u)
#define PPUTLIMPL_UDEC_3701u 0xE75u, 11u, 60u, ()
#define PPUTLIMPL_UDEC_3700u 0xE74u, 11u, 60u, (2u, 2u, 5u, 5u, 37u)
#define PPUTLIMPL_UDEC_3699u 0xE73u, 11u, 60u, (3u, 3u, 3u, 137u)
#define PPUTLIMPL_UDEC_3698u 0xE72u, 11u, 60u, (2u, 43u, 43u)
#define PPUTLIMPL_UDEC_3697u 0xE71u, 11u, 60u, ()
#define PPUTLIMPL_UDEC_3696u 0xE70u, 11u, 60u, (2u, 2u, 2u, 2u, 3u, 7u, 11u)
#define PPUTLIMPL_UDEC_3695u 0xE6Fu, 11u, 60u, (5u, 739u)
#define PPUTLIMPL_UDEC_3694u 0xE6Eu, 11u, 60u, (2u, 1847u)
#define PPUTLIMPL_UDEC_3693u 0xE6Du, 11u, 60u, (3u, 1231u)
#define PPUTLIMPL_UDEC_3692u 0xE6Cu, 11u, 60u, (2u, 2u, 13u, 71u)
#define PPUTLIMPL_UDEC_3691u 0xE6Bu, 11u, 60u, ()
#define PPUTLIMPL_UDEC_3690u 0xE6Au, 11u, 60u, (2u, 3u, 3u, 5u, 41u)
#define PPUTLIMPL_UDEC_3689u 0xE69u, 11u, 60u, (7u, 17u, 31u)
#define PPUTLIMPL_UDEC_3688u 0xE68u, 11u, 60u, (2u, 2u, 2u, 461u)
#define PPUTLIMPL_UDEC_3687u 0xE67u, 11u, 60u, (3u, 1229u)
#define PPUTLIMPL_UDEC_3686u 0xE66u, 11u, 60u, (2u, 19u, 97u)
#define PPUTLIMPL_UDEC_3685u 0xE65u, 11u, 60u, (5u, 11u, 67u)
#define PPUTLIMPL_UDEC_3684u 0xE64u, 11u, 60u, (2u, 2u, 3u, 307u)
#define PPUTLIMPL_UDEC_3683u 0xE63u, 11u, 60u, (29u, 127u)
#define PPUTLIMPL_UDEC_3682u 0xE62u, 11u, 60u, (2u, 7u, 263u)
#define PPUTLIMPL_UDEC_3681u 0xE61u, 11u, 60u, (3u, 3u, 409u)
#define PPUTLIMPL_UDEC_3680u 0xE60u, 11u, 60u, (2u, 2u, 2u, 2u, 2u, 5u, 23u)
#define PPUTLIMPL_UDEC_3679u 0xE5Fu, 11u, 60u, (13u, 283u)
#define PPUTLIMPL_UDEC_3678u 0xE5Eu, 11u, 60u, (2u, 3u, 613u)
#define PPUTLIMPL_UDEC_3677u 0xE5Du, 11u, 60u, ()
#define PPUTLIMPL_UDEC_3676u 0xE5Cu, 11u, 60u, (2u, 2u, 919u)
#define PPUTLIMPL_UDEC_3675u 0xE5Bu, 11u, 60u, (3u, 5u, 5u, 7u, 7u)
#define PPUTLIMPL_UDEC_3674u 0xE5Au, 11u, 60u, (2u, 11u, 167u)
#define PPUTLIMPL_UDEC_3673u 0xE59u, 11u, 60u, ()
#define PPUTLIMPL_UDEC_3672u 0xE58u, 11u, 60u, (2u, 2u, 2u, 3u, 3u, 3u, 17u)
#define PPUTLIMPL_UDEC_3671u 0xE57u, 11u, 60u, ()
#define PPUTLIMPL_UDEC_3670u 0xE56u, 11u, 60u, (2u, 5u, 367u)
#define PPUTLIMPL_UDEC_3669u 0xE55u, 11u, 60u, (3u, 1223u)
#define PPUTLIMPL_UDEC_3668u 0xE54u, 11u, 60u, (2u, 2u, 7u, 131u)
#define PPUTLIMPL_UDEC_3667u 0xE53u, 11u, 60u, (19u, 193u)
#define PPUTLIMPL_UDEC_3666u 0xE52u, 11u, 60u, (2u, 3u, 13u, 47u)
#define PPUTLIMPL_UDEC_3665u 0xE51u, 11u, 60u, (5u, 733u)
#define PPUTLIMPL_UDEC_3664u 0xE50u, 11u, 60u, (2u, 2u, 2u, 2u, 229u)
#define PPUTLIMPL_UDEC_3663u 0xE4Fu, 11u, 60u, (3u, 3u, 11u, 37u)
#define PPUTLIMPL_UDEC_3662u 0xE4Eu, 11u, 60u, (2u, 1831u)
#define PPUTLIMPL_UDEC_3661u 0xE4Du, 11u, 60u, (7u, 523u)
#define PPUTLIMPL_UDEC_3660u 0xE4Cu, 11u, 60u, (2u, 2u, 3u, 5u, 61u)
#define PPUTLIMPL_UDEC_3659u 0xE4Bu, 11u, 60u, ()
#define PPUTLIMPL_UDEC_3658u 0xE4Au, 11u, 60u, (2u, 31u, 59u)
#define PPUTLIMPL_UDEC_3657u 0xE49u, 11u, 60u, (3u, 23u, 53u)
#define PPUTLIMPL_UDEC_3656u 0xE48u, 11u, 60u, (2u, 2u, 2u, 457u)
#define PPUTLIMPL_UDEC_3655u 0xE47u, 11u, 60u, (5u, 17u, 43u)
#define PPUTLIMPL_UDEC_3654u 0xE46u, 11u, 60u, (2u, 3u, 3u, 7u, 29u)
#define PPUTLIMPL_UDEC_3653u 0xE45u, 11u, 60u, (13u, 281u)
#define PPUTLIMPL_UDEC_3652u 0xE44u, 11u, 60u, (2u, 2u, 11u, 83u)
#define PPUTLIMPL_UDEC_3651u 0xE43u, 11u, 60u, (3u, 1217u)
#define PPUTLIMPL_UDEC_3650u 0xE42u, 11u, 60u, (2u, 5u, 5u, 73u)
#define PPUTLIMPL_UDEC_3649u 0xE41u, 11u, 60u, (41u, 89u)
#define PPUTLIMPL_UDEC_3648u 0xE40u, 11u, 60u, (2u, 2u, 2u, 2u, 2u, 2u, 3u, 19u)
#define PPUTLIMPL_UDEC_3647u 0xE3Fu, 11u, 60u, (7u, 521u)
#define PPUTLIMPL_UDEC_3646u 0xE3Eu, 11u, 60u, (2u, 1823u)
#define PPUTLIMPL_UDEC_3645u 0xE3Du, 11u, 60u, (3u, 3u, 3u, 3u, 3u, 3u, 5u)
#define PPUTLIMPL_UDEC_3644u 0xE3Cu, 11u, 60u, (2u, 2u, 911u)
#define PPUTLIMPL_UDEC_3643u 0xE3Bu, 11u, 60u, ()
#define PPUTLIMPL_UDEC_3642u 0xE3Au, 11u, 60u, (2u, 3u, 607u)
#define PPUTLIMPL_UDEC_3641u 0xE39u, 11u, 60u, (11u, 331u)
#define PPUTLIMPL_UDEC_3640u 0xE38u, 11u, 60u, (2u, 2u, 2u, 5u, 7u, 13u)
#define PPUTLIMPL_UDEC_3639u 0xE37u, 11u, 60u, (3u, 1213u)
#define PPUTLIMPL_UDEC_3638u 0xE36u, 11u, 60u, (2u, 17u, 107u)
#define PPUTLIMPL_UDEC_3637u 0xE35u, 11u, 60u, ()
#define PPUTLIMPL_UDEC_3636u 0xE34u, 11u, 60u, (2u, 2u, 3u, 3u, 101u)
#define PPUTLIMPL_UDEC_3635u 0xE33u, 11u, 60u, (5u, 727u)
#define PPUTLIMPL_UDEC_3634u 0xE32u, 11u, 60u, (2u, 23u, 79u)
#define PPUTLIMPL_UDEC_3633u 0xE31u, 11u, 60u, (3u, 7u, 173u)
#define PPUTLIMPL_UDEC_3632u 0xE30u, 11u, 60u, (2u, 2u, 2u, 2u, 227u)
#define PPUTLIMPL_UDEC_3631u 0xE2Fu, 11u, 60u, ()
#define PPUTLIMPL_UDEC_3630u 0xE2Eu, 11u, 60u, (2u, 3u, 5u, 11u, 11u)
#define PPUTLIMPL_UDEC_3629u 0xE2Du, 11u, 60u, (19u, 191u)
#define PPUTLIMPL_UDEC_3628u 0xE2Cu, 11u, 60u, (2u, 2u, 907u)
#define PPUTLIMPL_UDEC_3627u 0xE2Bu, 11u, 60u, (3u, 3u, 13u, 31u)
#define PPUTLIMPL_UDEC_3626u 0xE2Au, 11u, 60u, (2u, 7u, 7u, 37u)
#define PPUTLIMPL_UDEC_3625u 0xE29u, 11u, 60u, (5u, 5u, 5u, 29u)
#define PPUTLIMPL_UDEC_3624u 0xE28u, 11u, 60u, (2u, 2u, 2u, 3u, 151u)
#define PPUTLIMPL_UDEC_3623u 0xE27u, 11u, 60u, ()
#define PPUTLIMPL_UDEC_3622u 0xE26u, 11u, 60u, (2u, 1811u)
#define PPUTLIMPL_UDEC_3621u 0xE25u, 11u, 60u, (3u, 17u, 71u)
#define PPUTLIMPL_UDEC_3620u 0xE24u, 11u, 60u, (2u, 2u, 5u, 181u)
#define PPUTLIMPL_UDEC_3619u 0xE23u, 11u, 60u, (7u, 11u, 47u)
#define PPUTLIMPL_UDEC_3618u 0xE22u, 11u, 60u, (2u, 3u, 3u, 3u, 67u)
#define PPUTLIMPL_UDEC_3617u 0xE21u, 11u, 60u, ()
#define PPUTLIMPL_UDEC_3616u 0xE20u, 11u, 60u, (2u, 2u, 2u, 2u, 2u, 113u)
#define PPUTLIMPL_UDEC_3615u 0xE1Fu, 11u, 60u, (3u, 5u, 241u)
#define PPUTLIMPL_UDEC_3614u 0xE1Eu, 11u, 60u, (2u, 13u, 139u)
#define PPUTLIMPL_UDEC_3613u 0xE1Du, 11u, 60u, ()
#define PPUTLIMPL_UDEC_3612u 0xE1Cu, 11u, 60u, (2u, 2u, 3u, 7u, 43u)
#define PPUTLIMPL_UDEC_3611u 0xE1Bu, 11u, 60u, (23u, 157u)
#define PPUTLIMPL_UDEC_3610u 0xE1Au, 11u, 60u, (2u, 5u, 19u, 19u)
#define PPUTLIMPL_UDEC_3609u 0xE19u, 11u, 60u, (3u, 3u, 401u)
#define PPUTLIMPL_UDEC_3608u 0xE18u, 11u, 60u, (2u, 2u, 2u, 11u, 41u)
#define PPUTLIMPL_UDEC_3607u 0xE17u, 11u, 60u, ()
#define PPUTLIMPL_UDEC_3606u 0xE16u, 11u, 60u, (2u, 3u, 601u)
#define PPUTLIMPL_UDEC_3605u 0xE15u, 11u, 60u, (5u, 7u, 103u)
#define PPUTLIMPL_UDEC_3604u 0xE14u, 11u, 60u, (2u, 2u, 17u, 53u)
#define PPUTLIMPL_UDEC_3603u 0xE13u, 11u, 60u, (3u, 1201u)
#define PPUTLIMPL_UDEC_3602u 0xE12u, 11u, 60u, (2u, 1801u)
#define PPUTLIMPL_UDEC_3601u 0xE11u, 11u, 60u, (13u, 277u)
#define PPUTLIMPL_UDEC_3600u 0xE10u, 11u, 60u, (2u, 2u, 2u, 2u, 3u, 3u, 5u, 5u)
#define PPUTLIMPL_UDEC_3599u 0xE0Fu, 11u, 59u, (59u, 61u)
#define PPUTLIMPL_UDEC_3598u 0xE0Eu, 11u, 59u, (2u, 7u, 257u)
#define PPUTLIMPL_UDEC_3597u 0xE0Du, 11u, 59u, (3u, 11u, 109u)
#define PPUTLIMPL_UDEC_3596u 0xE0Cu, 11u, 59u, (2u, 2u, 29u, 31u)
#define PPUTLIMPL_UDEC_3595u 0xE0Bu, 11u, 59u, (5u, 719u)
#define PPUTLIMPL_UDEC_3594u 0xE0Au, 11u, 59u, (2u, 3u, 599u)
#define PPUTLIMPL_UDEC_3593u 0xE09u, 11u, 59u, ()
#define PPUTLIMPL_UDEC_3592u 0xE08u, 11u, 59u, (2u, 2u, 2u, 449u)
#define PPUTLIMPL_UDEC_3591u 0xE07u, 11u, 59u, (3u, 3u, 3u, 7u, 19u)
#define PPUTLIMPL_UDEC_3590u 0xE06u, 11u, 59u, (2u, 5u, 359u)
#define PPUTLIMPL_UDEC_3589u 0xE05u, 11u, 59u, (37u, 97u)
#define PPUTLIMPL_UDEC_3588u 0xE04u, 11u, 59u, (2u, 2u, 3u, 13u, 23u)
#define PPUTLIMPL_UDEC_3587u 0xE03u, 11u, 59u, (17u, 211u)
#define PPUTLIMPL_UDEC_3586u 0xE02u, 11u, 59u, (2u, 11u, 163u)
#define PPUTLIMPL_UDEC_3585u 0xE01u, 11u, 59u, (3u, 5u, 239u)
#define PPUTLIMPL_UDEC_3584u 0xE00u, 11u, 59u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 7u)
#define PPUTLIMPL_UDEC_3583u 0xDFFu, 11u, 59u, ()
#define PPUTLIMPL_UDEC_3582u 0xDFEu, 11u, 59u, (2u, 3u, 3u, 199u)
#define PPUTLIMPL_UDEC_3581u 0xDFDu, 11u, 59u, ()
#define PPUTLIMPL_UDEC_3580u 0xDFCu, 11u, 59u, (2u, 2u, 5u, 179u)
#define PPUTLIMPL_UDEC_3579u 0xDFBu, 11u, 59u, (3u, 1193u)
#define PPUTLIMPL_UDEC_3578u 0xDFAu, 11u, 59u, (2u, 1789u)
#define PPUTLIMPL_UDEC_3577u 0xDF9u, 11u, 59u, (7u, 7u, 73u)
#define PPUTLIMPL_UDEC_3576u 0xDF8u, 11u, 59u, (2u, 2u, 2u, 3u, 149u)
#define PPUTLIMPL_UDEC_3575u 0xDF7u, 11u, 59u, (5u, 5u, 11u, 13u)
#define PPUTLIMPL_UDEC_3574u 0xDF6u, 11u, 59u, (2u, 1787u)
#define PPUTLIMPL_UDEC_3573u 0xDF5u, 11u, 59u, (3u, 3u, 397u)
#define PPUTLIMPL_UDEC_3572u 0xDF4u, 11u, 59u, (2u, 2u, 19u, 47u)
#define PPUTLIMPL_UDEC_3571u 0xDF3u, 11u, 59u, ()
#define PPUTLIMPL_UDEC_3570u 0xDF2u, 11u, 59u, (2u, 3u, 5u, 7u, 17u)
#define PPUTLIMPL_UDEC_3569u 0xDF1u, 11u, 59u, (43u, 83u)
#define PPUTLIMPL_UDEC_3568u 0xDF0u, 11u, 59u, (2u, 2u, 2u, 2u, 223u)
#define PPUTLIMPL_UDEC_3567u 0xDEFu, 11u, 59u, (3u, 29u, 41u)
#define PPUTLIMPL_UDEC_3566u 0xDEEu, 11u, 59u, (2u, 1783u)
#define PPUTLIMPL_UDEC_3565u 0xDEDu, 11u, 59u, (5u, 23u, 31u)
#define PPUTLIMPL_UDEC_3564u 0xDECu, 11u, 59u, (2u, 2u, 3u, 3u, 3u, 3u, 11u)
#define PPUTLIMPL_UDEC_3563u 0xDEBu, 11u, 59u, (7u, 509u)
#define PPUTLIMPL_UDEC_3562u 0xDEAu, 11u, 59u, (2u, 13u, 137u)
#define PPUTLIMPL_UDEC_3561u 0xDE9u, 11u, 59u, (3u, 1187u)
#define PPUTLIMPL_UDEC_3560u 0xDE8u, 11u, 59u, (2u, 2u, 2u, 5u, 89u)
#define PPUTLIMPL_UDEC_3559u 0xDE7u, 11u, 59u, ()
#define PPUTLIMPL_UDEC_3558u 0xDE6u, 11u, 59u, (2u, 3u, 593u)
#define PPUTLIMPL_UDEC_3557u 0xDE5u, 11u, 59u, ()
#define PPUTLIMPL_UDEC_3556u 0xDE4u, 11u, 59u, (2u, 2u, 7u, 127u)
#define PPUTLIMPL_UDEC_3555u 0xDE3u, 11u, 59u, (3u, 3u, 5u, 79u)
#define PPUTLIMPL_UDEC_3554u 0xDE2u, 11u, 59u, (2u, 1777u)
#define PPUTLIMPL_UDEC_3553u 0xDE1u, 11u, 59u, (11u, 17u, 19u)
#define PPUTLIMPL_UDEC_3552u 0xDE0u, 11u, 59u, (2u, 2u, 2u, 2u, 2u, 3u, 37u)
#define PPUTLIMPL_UDEC_3551u 0xDDFu, 11u, 59u, (53u, 67u)
#define PPUTLIMPL_UDEC_3550u 0xDDEu, 11u, 59u, (2u, 5u, 5u, 71u)
#define PPUTLIMPL_UDEC_3549u 0xDDDu, 11u, 59u, (3u, 7u, 13u, 13u)
#define PPUTLIMPL_UDEC_3548u 0xDDCu, 11u, 59u, (2u, 2u, 887u)
#define PPUTLIMPL_UDEC_3547u 0xDDBu, 11u, 59u, ()
#define PPUTLIMPL_UDEC_3546u 0xDDAu, 11u, 59u, (2u, 3u, 3u, 197u)
#define PPUTLIMPL_UDEC_3545u 0xDD9u, 11u, 59u, (5u, 709u)
#define PPUTLIMPL_UDEC_3544u 0xDD8u, 11u, 59u, (2u, 2u, 2u, 443u)
#define PPUTLIMPL_UDEC_3543u 0xDD7u, 11u, 59u, (3u, 1181u)
#define PPUTLIMPL_UDEC_3542u 0xDD6u, 11u, 59u, (2u, 7u, 11u, 23u)
#define PPUTLIMPL_UDEC_3541u 0xDD5u, 11u, 59u, ()
#define PPUTLIMPL_UDEC_3540u 0xDD4u, 11u, 59u, (2u, 2u, 3u, 5u, 59u)
#define PPUTLIMPL_UDEC_3539u 0xDD3u, 11u, 59u, ()
#define PPUTLIMPL_UDEC_3538u 0xDD2u, 11u, 59u, (2u, 29u, 61u)
#define PPUTLIMPL_UDEC_3537u 0xDD1u, 11u, 59u, (3u, 3u, 3u, 131u)
#define PPUTLIMPL_UDEC_3536u 0xDD0u, 11u, 59u, (2u, 2u, 2u, 2u, 13u, 17u)
#define PPUTLIMPL_UDEC_3535u 0xDCFu, 11u, 59u, (5u, 7u, 101u)
#define PPUTLIMPL_UDEC_3534u 0xDCEu, 11u, 59u, (2u, 3u, 19u, 31u)
#define PPUTLIMPL_UDEC_3533u 0xDCDu, 11u, 59u, ()
#define PPUTLIMPL_UDEC_3532u 0xDCCu, 11u, 59u, (2u, 2u, 883u)
#define PPUTLIMPL_UDEC_3531u 0xDCBu, 11u, 59u, (3u, 11u, 107u)
#define PPUTLIMPL_UDEC_3530u 0xDCAu, 11u, 59u, (2u, 5u, 353u)
#define PPUTLIMPL_UDEC_3529u 0xDC9u, 11u, 59u, ()
#define PPUTLIMPL_UDEC_3528u 0xDC8u, 11u, 59u, (2u, 2u, 2u, 3u, 3u, 7u, 7u)
#define PPUTLIMPL_UDEC_3527u 0xDC7u, 11u, 59u, ()
#define PPUTLIMPL_UDEC_3526u 0xDC6u, 11u, 59u, (2u, 41u, 43u)
#define PPUTLIMPL_UDEC_3525u 0xDC5u, 11u, 59u, (3u, 5u, 5u, 47u)
#define PPUTLIMPL_UDEC_3524u 0xDC4u, 11u, 59u, (2u, 2u, 881u)
#define PPUTLIMPL_UDEC_3523u 0xDC3u, 11u, 59u, (13u, 271u)
#define PPUTLIMPL_UDEC_3522u 0xDC2u, 11u, 59u, (2u, 3u, 587u)
#define PPUTLIMPL_UDEC_3521u 0xDC1u, 11u, 59u, (7u, 503u)
#define PPUTLIMPL_UDEC_3520u 0xDC0u, 11u, 59u, (2u, 2u, 2u, 2u, 2u, 2u, 5u, 11u)
#define PPUTLIMPL_UDEC_3519u 0xDBFu, 11u, 59u, (3u, 3u, 17u, 23u)
#define PPUTLIMPL_UDEC_3518u 0xDBEu, 11u, 59u, (2u, 1759u)
#define PPUTLIMPL_UDEC_3517u 0xDBDu, 11u, 59u, ()
#define PPUTLIMPL_UDEC_3516u 0xDBCu, 11u, 59u, (2u, 2u, 3u, 293u)
#define PPUTLIMPL_UDEC_3515u 0xDBBu, 11u, 59u, (5u, 19u, 37u)
#define PPUTLIMPL_UDEC_3514u 0xDBAu, 11u, 59u, (2u, 7u, 251u)
#define PPUTLIMPL_UDEC_3513u 0xDB9u, 11u, 59u, (3u, 1171u)
#define PPUTLIMPL_UDEC_3512u 0xDB8u, 11u, 59u, (2u, 2u, 2u, 439u)
#define PPUTLIMPL_UDEC_3511u 0xDB7u, 11u, 59u, ()
#define PPUTLIMPL_UDEC_3510u 0xDB6u, 11u, 59u, (2u, 3u, 3u, 3u, 5u, 13u)
#define PPUTLIMPL_UDEC_3509u 0xDB5u, 11u, 59u, (11u, 11u, 29u)
#define PPUTLIMPL_UDEC_3508u 0xDB4u, 11u, 59u, (2u, 2u, 877u)
#define PPUTLIMPL_UDEC_3507u 0xDB3u, 11u, 59u, (3u, 7u, 167u)
#define PPUTLIMPL_UDEC_3506u 0xDB2u, 11u, 59u, (2u, 1753u)
#define PPUTLIMPL_UDEC_3505u 0xDB1u, 11u, 59u, (5u, 701u)
#define PPUTLIMPL_UDEC_3504u 0xDB0u, 11u, 59u, (2u, 2u, 2u, 2u, 3u, 73u)
#define PPUTLIMPL_UDEC_3503u 0xDAFu, 11u, 59u, (31u, 113u)
#define PPUTLIMPL_UDEC_3502u 0xDAEu, 11u, 59u, (2u, 17u, 103u)
#define PPUTLIMPL_UDEC_3501u 0xDADu, 11u, 59u, (3u, 3u, 389u)
#define PPUTLIMPL_UDEC_3500u 0xDACu, 11u, 59u, (2u, 2u, 5u, 5u, 5u, 7u)
#define PPUTLIMPL_UDEC_3499u 0xDABu, 11u, 59u, ()
#define PPUTLIMPL_UDEC_3498u 0xDAAu, 11u, 59u, (2u, 3u, 11u, 53u)
#define PPUTLIMPL_UDEC_3497u 0xDA9u, 11u, 59u, (13u, 269u)
#define PPUTLIMPL_UDEC_3496u 0xDA8u, 11u, 59u, (2u, 2u, 2u, 19u, 23u)
#define PPUTLIMPL_UDEC_3495u 0xDA7u, 11u, 59u, (3u, 5u, 233u)
#define PPUTLIMPL_UDEC_3494u 0xDA6u, 11u, 59u, (2u, 1747u)
#define PPUTLIMPL_UDEC_3493u 0xDA5u, 11u, 59u, (7u, 499u)
#define PPUTLIMPL_UDEC_3492u 0xDA4u, 11u, 59u, (2u, 2u, 3u, 3u, 97u)
#define PPUTLIMPL_UDEC_3491u 0xDA3u, 11u, 59u, ()
#define PPUTLIMPL_UDEC_3490u 0xDA2u, 11u, 59u, (2u, 5u, 349u)
#define PPUTLIMPL_UDEC_3489u 0xDA1u, 11u, 59u, (3u, 1163u)
#define PPUTLIMPL_UDEC_3488u 0xDA0u, 11u, 59u, (2u, 2u, 2u, 2u, 2u, 109u)
#define PPUTLIMPL_UDEC_3487u 0xD9Fu, 11u, 59u, (11u, 317u)
#define PPUTLIMPL_UDEC_3486u 0xD9Eu, 11u, 59u, (2u, 3u, 7u, 83u)
#define PPUTLIMPL_UDEC_3485u 0xD9Du, 11u, 59u, (5u, 17u, 41u)
#define PPUTLIMPL_UDEC_3484u 0xD9Cu, 11u, 59u, (2u, 2u, 13u, 67u)
#define PPUTLIMPL_UDEC_3483u 0xD9Bu, 11u, 59u, (3u, 3u, 3u, 3u, 43u)
#define PPUTLIMPL_UDEC_3482u 0xD9Au, 11u, 59u, (2u, 1741u)
#define PPUTLIMPL_UDEC_3481u 0xD99u, 11u, 59u, (59u, 59u)
#define PPUTLIMPL_UDEC_3480u 0xD98u, 11u, 58u, (2u, 2u, 2u, 3u, 5u, 29u)
#define PPUTLIMPL_UDEC_3479u 0xD97u, 11u, 58u, (7u, 7u, 71u)
#define PPUTLIMPL_UDEC_3478u 0xD96u, 11u, 58u, (2u, 37u, 47u)
#define PPUTLIMPL_UDEC_3477u 0xD95u, 11u, 58u, (3u, 19u, 61u)
#define PPUTLIMPL_UDEC_3476u 0xD94u, 11u, 58u, (2u, 2u, 11u, 79u)
#define PPUTLIMPL_UDEC_3475u 0xD93u, 11u, 58u, (5u, 5u, 139u)
#define PPUTLIMPL_UDEC_3474u 0xD92u, 11u, 58u, (2u, 3u, 3u, 193u)
#define PPUTLIMPL_UDEC_3473u 0xD91u, 11u, 58u, (23u, 151u)
#define PPUTLIMPL_UDEC_3472u 0xD90u, 11u, 58u, (2u, 2u, 2u, 2u, 7u, 31u)
#define PPUTLIMPL_UDEC_3471u 0xD8Fu, 11u, 58u, (3u, 13u, 89u)
#define PPUTLIMPL_UDEC_3470u 0xD8Eu, 11u, 58u, (2u, 5u, 347u)
#define PPUTLIMPL_UDEC_3469u 0xD8Du, 11u, 58u, ()
#define PPUTLIMPL_UDEC_3468u 0xD8Cu, 11u, 58u, (2u, 2u, 3u, 17u, 17u)
#define PPUTLIMPL_UDEC_3467u 0xD8Bu, 11u, 58u, ()
#define PPUTLIMPL_UDEC_3466u 0xD8Au, 11u, 58u, (2u, 1733u)
#define PPUTLIMPL_UDEC_3465u 0xD89u, 11u, 58u, (3u, 3u, 5u, 7u, 11u)
#define PPUTLIMPL_UDEC_3464u 0xD88u, 11u, 58u, (2u, 2u, 2u, 433u)
#define PPUTLIMPL_UDEC_3463u 0xD87u, 11u, 58u, ()
#define PPUTLIMPL_UDEC_3462u 0xD86u, 11u, 58u, (2u, 3u, 577u)
#define PPUTLIMPL_UDEC_3461u 0xD85u, 11u, 58u, ()
#define PPUTLIMPL_UDEC_3460u 0xD84u, 11u, 58u, (2u, 2u, 5u, 173u)
#define PPUTLIMPL_UDEC_3459u 0xD83u, 11u, 58u, (3u, 1153u)
#define PPUTLIMPL_UDEC_3458u 0xD82u, 11u, 58u, (2u, 7u, 13u, 19u)
#define PPUTLIMPL_UDEC_3457u 0xD81u, 11u, 58u, ()
#define PPUTLIMPL_UDEC_3456u 0xD80u, 11u, 58u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 3u, 3u, 3u)
#define PPUTLIMPL_UDEC_3455u 0xD7Fu, 11u, 58u, (5u, 691u)
#define PPUTLIMPL_UDEC_3454u 0xD7Eu, 11u, 58u, (2u, 11u, 157u)
#define PPUTLIMPL_UDEC_3453u 0xD7Du, 11u, 58u, (3u, 1151u)
#define PPUTLIMPL_UDEC_3452u 0xD7Cu, 11u, 58u, (2u, 2u, 863u)
#define PPUTLIMPL_UDEC_3451u 0xD7Bu, 11u, 58u, (7u, 17u, 29u)
#define PPUTLIMPL_UDEC_3450u 0xD7Au, 11u, 58u, (2u, 3u, 5u, 5u, 23u)
#define PPUTLIMPL_UDEC_3449u 0xD79u, 11u, 58u, ()
#define PPUTLIMPL_UDEC_3448u 0xD78u, 11u, 58u, (2u, 2u, 2u, 431u)
#define PPUTLIMPL_UDEC_3447u 0xD77u, 11u, 58u, (3u, 3u, 383u)
#define PPUTLIMPL_UDEC_3446u 0xD76u, 11u, 58u, (2u, 1723u)
#define PPUTLIMPL_UDEC_3445u 0xD75u, 11u, 58u, (5u, 13u, 53u)
#define PPUTLIMPL_UDEC_3444u 0xD74u, 11u, 58u, (2u, 2u, 3u, 7u, 41u)
#define PPUTLIMPL_UDEC_3443u 0xD73u, 11u, 58u, (11u, 313u)
#define PPUTLIMPL_UDEC_3442u 0xD72u, 11u, 58u, (2u, 1721u)
#define PPUTLIMPL_UDEC_3441u 0xD71u, 11u, 58u, (3u, 31u, 37u)
#define PPUTLIMPL_UDEC_3440u 0xD70u, 11u, 58u, (2u, 2u, 2u, 2u, 5u, 43u)
#define PPUTLIMPL_UDEC_3439u 0xD6Fu, 11u, 58u, (19u, 181u)
#define PPUTLIMPL_UDEC_3438u 0xD6Eu, 11u, 58u, (2u, 3u, 3u, 191u)
#define PPUTLIMPL_UDEC_3437u 0xD6Du, 11u, 58u, (7u, 491u)
#define PPUTLIMPL_UDEC_3436u 0xD6Cu, 11u, 58u, (2u, 2u, 859u)
#define PPUTLIMPL_UDEC_3435u 0xD6Bu, 11u, 58u, (3u, 5u, 229u)
#define PPUTLIMPL_UDEC_3434u 0xD6Au, 11u, 58u, (2u, 17u, 101u)
#define PPUTLIMPL_UDEC_3433u 0xD69u, 11u, 58u, ()
#define PPUTLIMPL_UDEC_3432u 0xD68u, 11u, 58u, (2u, 2u, 2u, 3u, 11u, 13u)
#define PPUTLIMPL_UDEC_3431u 0xD67u, 11u, 58u, (47u, 73u)
#define PPUTLIMPL_UDEC_3430u 0xD66u, 11u, 58u, (2u, 5u, 7u, 7u, 7u)
#define PPUTLIMPL_UDEC_3429u 0xD65u, 11u, 58u, (3u, 3u, 3u, 127u)
#define PPUTLIMPL_UDEC_3428u 0xD64u, 11u, 58u, (2u, 2u, 857u)
#define PPUTLIMPL_UDEC_3427u 0xD63u, 11u, 58u, (23u, 149u)
#define PPUTLIMPL_UDEC_3426u 0xD62u, 11u, 58u, (2u, 3u, 571u)
#define PPUTLIMPL_UDEC_3425u 0xD61u, 11u, 58u, (5u, 5u, 137u)
#define PPUTLIMPL_UDEC_3424u 0xD60u, 11u, 58u, (2u, 2u, 2u, 2u, 2u, 107u)
#define PPUTLIMPL_UDEC_3423u 0xD5Fu, 11u, 58u, (3u, 7u, 163u)
#define PPUTLIMPL_UDEC_3422u 0xD5Eu, 11u, 58u, (2u, 29u, 59u)
#define PPUTLIMPL_UDEC_3421u 0xD5Du, 11u, 58u, (11u, 311u)
#define PPUTLIMPL_UDEC_3420u 0xD5Cu, 11u, 58u, (2u, 2u, 3u, 3u, 5u, 19u)
#define PPUTLIMPL_UDEC_3419u 0xD5Bu, 11u, 58u, (13u, 263u)
#define PPUTLIMPL_UDEC_3418u 0xD5Au, 11u, 58u, (2u, 1709u)
#define PPUTLIMPL_UDEC_3417u 0xD59u, 11u, 58u, (3u, 17u, 67u)
#define PPUTLIMPL_UDEC_3416u 0xD58u, 11u, 58u, (2u, 2u, 2u, 7u, 61u)
#define PPUTLIMPL_UDEC_3415u 0xD57u, 11u, 58u, (5u, 683u)
#define PPUTLIMPL_UDEC_3414u 0xD56u, 11u, 58u, (2u, 3u, 569u)
#define PPUTLIMPL_UDEC_3413u 0xD55u, 11u, 58u, ()
#define PPUTLIMPL_UDEC_3412u 0xD54u, 11u, 58u, (2u, 2u, 853u)
#define PPUTLIMPL_UDEC_3411u 0xD53u, 11u, 58u, (3u, 3u, 379u)
#define PPUTLIMPL_UDEC_3410u 0xD52u, 11u, 58u, (2u, 5u, 11u, 31u)
#define PPUTLIMPL_UDEC_3409u 0xD51u, 11u, 58u, (7u, 487u)
#define PPUTLIMPL_UDEC_3408u 0xD50u, 11u, 58u, (2u, 2u, 2u, 2u, 3u, 71u)
#define PPUTLIMPL_UDEC_3407u 0xD4Fu, 11u, 58u, ()
#define PPUTLIMPL_UDEC_3406u 0xD4Eu, 11u, 58u, (2u, 13u, 131u)
#define PPUTLIMPL_UDEC_3405u 0xD4Du, 11u, 58u, (3u, 5u, 227u)
#define PPUTLIMPL_UDEC_3404u 0xD4Cu, 11u, 58u, (2u, 2u, 23u, 37u)
#define PPUTLIMPL_UDEC_3403u 0xD4Bu, 11u, 58u, (41u, 83u)
#define PPUTLIMPL_UDEC_3402u 0xD4Au, 11u, 58u, (2u, 3u, 3u, 3u, 3u, 3u, 7u)
#define PPUTLIMPL_UDEC_3401u 0xD49u, 11u, 58u, (19u, 179u)
#define PPUTLIMPL_UDEC_3400u 0xD48u, 11u, 58u, (2u, 2u, 2u, 5u, 5u, 17u)
#define PPUTLIMPL_UDEC_3399u 0xD47u, 11u, 58u, (3u, 11u, 103u)
#define PPUTLIMPL_UDEC_3398u 0xD46u, 11u, 58u, (2u, 1699u)
#define PPUTLIMPL_UDEC_3397u 0xD45u, 11u, 58u, (43u, 79u)
#define PPUTLIMPL_UDEC_3396u 0xD44u, 11u, 58u, (2u, 2u, 3u, 283u)
#define PPUTLIMPL_UDEC_3395u 0xD43u, 11u, 58u, (5u, 7u, 97u)
#define PPUTLIMPL_UDEC_3394u 0xD42u, 11u, 58u, (2u, 1697u)
#define PPUTLIMPL_UDEC_3393u 0xD41u, 11u, 58u, (3u, 3u, 13u, 29u)
#define PPUTLIMPL_UDEC_3392u 0xD40u, 11u, 58u, (2u, 2u, 2u, 2u, 2u, 2u, 53u)
#define PPUTLIMPL_UDEC_3391u 0xD3Fu, 11u, 58u, ()
#define PPUTLIMPL_UDEC_3390u 0xD3Eu, 11u, 58u, (2u, 3u, 5u, 113u)
#define PPUTLIMPL_UDEC_3389u 0xD3Du, 11u, 58u, ()
#define PPUTLIMPL_UDEC_3388u 0xD3Cu, 11u, 58u, (2u, 2u, 7u, 11u, 11u)
#define PPUTLIMPL_UDEC_3387u 0xD3Bu, 11u, 58u, (3u, 1129u)
#define PPUTLIMPL_UDEC_3386u 0xD3Au, 11u, 58u, (2u, 1693u)
#define PPUTLIMPL_UDEC_3385u 0xD39u, 11u, 58u, (5u, 677u)
#define PPUTLIMPL_UDEC_3384u 0xD38u, 11u, 58u, (2u, 2u, 2u, 3u, 3u, 47u)
#define PPUTLIMPL_UDEC_3383u 0xD37u, 11u, 58u, (17u, 199u)
#define PPUTLIMPL_UDEC_3382u 0xD36u, 11u, 58u, (2u, 19u, 89u)
#define PPUTLIMPL_UDEC_3381u 0xD35u, 11u, 58u, (3u, 7u, 7u, 23u)
#define PPUTLIMPL_UDEC_3380u 0xD34u, 11u, 58u, (2u, 2u, 5u, 13u, 13u)
#define PPUTLIMPL_UDEC_3379u 0xD33u, 11u, 58u, (31u, 109u)
#define PPUTLIMPL_UDEC_3378u 0xD32u, 11u, 58u, (2u, 3u, 563u)
#define PPUTLIMPL_UDEC_3377u 0xD31u, 11u, 58u, (11u, 307u)
#define PPUTLIMPL_UDEC_3376u 0xD30u, 11u, 58u, (2u, 2u, 2u, 2u, 211u)
#define PPUTLIMPL_UDEC_3375u 0xD2Fu, 11u, 58u, (3u, 3u, 3u, 5u, 5u, 5u)
#define PPUTLIMPL_UDEC_3374u 0xD2Eu, 11u, 58u, (2u, 7u, 241u)
#define PPUTLIMPL_UDEC_3373u 0xD2Du, 11u, 58u, ()
#define PPUTLIMPL_UDEC_3372u 0xD2Cu, 11u, 58u, (2u, 2u, 3u, 281u)
#define PPUTLIMPL_UDEC_3371u 0xD2Bu, 11u, 58u, ()
#define PPUTLIMPL_UDEC_3370u 0xD2Au, 11u, 58u, (2u, 5u, 337u)
#define PPUTLIMPL_UDEC_3369u 0xD29u, 11u, 58u, (3u, 1123u)
#define PPUTLIMPL_UDEC_3368u 0xD28u, 11u, 58u, (2u, 2u, 2u, 421u)
#define PPUTLIMPL_UDEC_3367u 0xD27u, 11u, 58u, (7u, 13u, 37u)
#define PPUTLIMPL_UDEC_3366u 0xD26u, 11u, 58u, (2u, 3u, 3u, 11u, 17u)
#define PPUTLIMPL_UDEC_3365u 0xD25u, 11u, 58u, (5u, 673u)
#define PPUTLIMPL_UDEC_3364u 0xD24u, 11u, 58u, (2u, 2u, 29u, 29u)
#define PPUTLIMPL_UDEC_3363u 0xD23u, 11u, 57u, (3u, 19u, 59u)
#define PPUTLIMPL_UDEC_3362u 0xD22u, 11u, 57u, (2u, 41u, 41u)
#define PPUTLIMPL_UDEC_3361u 0xD21u, 11u, 57u, ()
#define PPUTLIMPL_UDEC_3360u 0xD20u, 11u, 57u, (2u, 2u, 2u, 2u, 2u, 3u, 5u, 7u)
#define PPUTLIMPL_UDEC_3359u 0xD1Fu, 11u, 57u, ()
#define PPUTLIMPL_UDEC_3358u 0xD1Eu, 11u, 57u, (2u, 23u, 73u)
#define PPUTLIMPL_UDEC_3357u 0xD1Du, 11u, 57u, (3u, 3u, 373u)
#define PPUTLIMPL_UDEC_3356u 0xD1Cu, 11u, 57u, (2u, 2u, 839u)
#define PPUTLIMPL_UDEC_3355u 0xD1Bu, 11u, 57u, (5u, 11u, 61u)
#define PPUTLIMPL_UDEC_3354u 0xD1Au, 11u, 57u, (2u, 3u, 13u, 43u)
#define PPUTLIMPL_UDEC_3353u 0xD19u, 11u, 57u, (7u, 479u)
#define PPUTLIMPL_UDEC_3352u 0xD18u, 11u, 57u, (2u, 2u, 2u, 419u)
#define PPUTLIMPL_UDEC_3351u 0xD17u, 11u, 57u, (3u, 1117u)
#define PPUTLIMPL_UDEC_3350u 0xD16u, 11u, 57u, (2u, 5u, 5u, 67u)
#define PPUTLIMPL_UDEC_3349u 0xD15u, 11u, 57u, (17u, 197u)
#define PPUTLIMPL_UDEC_3348u 0xD14u, 11u, 57u, (2u, 2u, 3u, 3u, 3u, 31u)
#define PPUTLIMPL_UDEC_3347u 0xD13u, 11u, 57u, ()
#define PPUTLIMPL_UDEC_3346u 0xD12u, 11u, 57u, (2u, 7u, 239u)
#define PPUTLIMPL_UDEC_3345u 0xD11u, 11u, 57u, (3u, 5u, 223u)
#define PPUTLIMPL_UDEC_3344u 0xD10u, 11u, 57u, (2u, 2u, 2u, 2u, 11u, 19u)
#define PPUTLIMPL_UDEC_3343u 0xD0Fu, 11u, 57u, ()
#define PPUTLIMPL_UDEC_3342u 0xD0Eu, 11u, 57u, (2u, 3u, 557u)
#define PPUTLIMPL_UDEC_3341u 0xD0Du, 11u, 57u, (13u, 257u)
#define PPUTLIMPL_UDEC_3340u 0xD0Cu, 11u, 57u, (2u, 2u, 5u, 167u)
#define PPUTLIMPL_UDEC_3339u 0xD0Bu, 11u, 57u, (3u, 3u, 7u, 53u)
#define PPUTLIMPL_UDEC_3338u 0xD0Au, 11u, 57u, (2u, 1669u)
#define PPUTLIMPL_UDEC_3337u 0xD09u, 11u, 57u, (47u, 71u)
#define PPUTLIMPL_UDEC_3336u 0xD08u, 11u, 57u, (2u, 2u, 2u, 3u, 139u)
#define PPUTLIMPL_UDEC_3335u 0xD07u, 11u, 57u, (5u, 23u, 29u)
#define PPUTLIMPL_UDEC_3334u 0xD06u, 11u, 57u, (2u, 1667u)
#define PPUTLIMPL_UDEC_3333u 0xD05u, 11u, 57u, (3u, 11u, 101u)
#define PPUTLIMPL_UDEC_3332u 0xD04u, 11u, 57u, (2u, 2u, 7u, 7u, 17u)
#define PPUTLIMPL_UDEC_3331u 0xD03u, 11u, 57u, ()
#define PPUTLIMPL_UDEC_3330u 0xD02u, 11u, 57u, (2u, 3u, 3u, 5u, 37u)
#define PPUTLIMPL_UDEC_3329u 0xD01u, 11u, 57u, ()
#define PPUTLIMPL_UDEC_3328u 0xD00u, 11u, 57u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 13u)
#define PPUTLIMPL_UDEC_3327u 0xCFFu, 11u, 57u, (3u, 1109u)
#define PPUTLIMPL_UDEC_3326u 0xCFEu, 11u, 57u, (2u, 1663u)
#define PPUTLIMPL_UDEC_3325u 0xCFDu, 11u, 57u, (5u, 5u, 7u, 19u)
#define PPUTLIMPL_UDEC_3324u 0xCFCu, 11u, 57u, (2u, 2u, 3u, 277u)
#define PPUTLIMPL_UDEC_3323u 0xCFBu, 11u, 57u, ()
#define PPUTLIMPL_UDEC_3322u 0xCFAu, 11u, 57u, (2u, 11u, 151u)
#define PPUTLIMPL_UDEC_3321u 0xCF9u, 11u, 57u, (3u, 3u, 3u, 3u, 41u)
#define PPUTLIMPL_UDEC_3320u 0xCF8u, 11u, 57u, (2u, 2u, 2u, 5u, 83u)
#define PPUTLIMPL_UDEC_3319u 0xCF7u, 11u, 57u, ()
#define PPUTLIMPL_UDEC_3318u 0xCF6u, 11u, 57u, (2u, 3u, 7u, 79u)
#define PPUTLIMPL_UDEC_3317u 0xCF5u, 11u, 57u, (31u, 107u)
#define PPUTLIMPL_UDEC_3316u 0xCF4u, 11u, 57u, (2u, 2u, 829u)
#define PPUTLIMPL_UDEC_3315u 0xCF3u, 11u, 57u, (3u, 5u, 13u, 17u)
#define PPUTLIMPL_UDEC_3314u 0xCF2u, 11u, 57u, (2u, 1657u)
#define PPUTLIMPL_UDEC_3313u 0xCF1u, 11u, 57u, ()
#define PPUTLIMPL_UDEC_3312u 0xCF0u, 11u, 57u, (2u, 2u, 2u, 2u, 3u, 3u, 23u)
#define PPUTLIMPL_UDEC_3311u 0xCEFu, 11u, 57u, (7u, 11u, 43u)
#define PPUTLIMPL_UDEC_3310u 0xCEEu, 11u, 57u, (2u, 5u, 331u)
#define PPUTLIMPL_UDEC_3309u 0xCEDu, 11u, 57u, (3u, 1103u)
#define PPUTLIMPL_UDEC_3308u 0xCECu, 11u, 57u, (2u, 2u, 827u)
#define PPUTLIMPL_UDEC_3307u 0xCEBu, 11u, 57u, ()
#define PPUTLIMPL_UDEC_3306u 0xCEAu, 11u, 57u, (2u, 3u, 19u, 29u)
#define PPUTLIMPL_UDEC_3305u 0xCE9u, 11u, 57u, (5u, 661u)
#define PPUTLIMPL_UDEC_3304u 0xCE8u, 11u, 57u, (2u, 2u, 2u, 7u, 59u)
#define PPUTLIMPL_UDEC_3303u 0xCE7u, 11u, 57u, (3u, 3u, 367u)
#define PPUTLIMPL_UDEC_3302u 0xCE6u, 11u, 57u, (2u, 13u, 127u)
#define PPUTLIMPL_UDEC_3301u 0xCE5u, 11u, 57u, ()
#define PPUTLIMPL_UDEC_3300u 0xCE4u, 11u, 57u, (2u, 2u, 3u, 5u, 5u, 11u)
#define PPUTLIMPL_UDEC_3299u 0xCE3u, 11u, 57u, ()
#define PPUTLIMPL_UDEC_3298u 0xCE2u, 11u, 57u, (2u, 17u, 97u)
#define PPUTLIMPL_UDEC_3297u 0xCE1u, 11u, 57u, (3u, 7u, 157u)
#define PPUTLIMPL_UDEC_3296u 0xCE0u, 11u, 57u, (2u, 2u, 2u, 2u, 2u, 103u)
#define PPUTLIMPL_UDEC_3295u 0xCDFu, 11u, 57u, (5u, 659u)
#define PPUTLIMPL_UDEC_3294u 0xCDEu, 11u, 57u, (2u, 3u, 3u, 3u, 61u)
#define PPUTLIMPL_UDEC_3293u 0xCDDu, 11u, 57u, (37u, 89u)
#define PPUTLIMPL_UDEC_3292u 0xCDCu, 11u, 57u, (2u, 2u, 823u)
#define PPUTLIMPL_UDEC_3291u 0xCDBu, 11u, 57u, (3u, 1097u)
#define PPUTLIMPL_UDEC_3290u 0xCDAu, 11u, 57u, (2u, 5u, 7u, 47u)
#define PPUTLIMPL_UDEC_3289u 0xCD9u, 11u, 57u, (11u, 13u, 23u)
#define PPUTLIMPL_UDEC_3288u 0xCD8u, 11u, 57u, (2u, 2u, 2u, 3u, 137u)
#define PPUTLIMPL_UDEC_3287u 0xCD7u, 11u, 57u, (19u, 173u)
#define PPUTLIMPL_UDEC_3286u 0xCD6u, 11u, 57u, (2u, 31u, 53u)
#define PPUTLIMPL_UDEC_3285u 0xCD5u, 11u, 57u, (3u, 3u, 5u, 73u)
#define PPUTLIMPL_UDEC_3284u 0xCD4u, 11u, 57u, (2u, 2u, 821u)
#define PPUTLIMPL_UDEC_3283u 0xCD3u, 11u, 57u, (7u, 7u, 67u)
#define PPUTLIMPL_UDEC_3282u 0xCD2u, 11u, 57u, (2u, 3u, 547u)
#define PPUTLIMPL_UDEC_3281u 0xCD1u, 11u, 57u, (17u, 193u)
#define PPUTLIMPL_UDEC_3280u 0xCD0u, 11u, 57u, (2u, 2u, 2u, 2u, 5u, 41u)
#define PPUTLIMPL_UDEC_3279u 0xCCFu, 11u, 57u, (3u, 1093u)
#define PPUTLIMPL_UDEC_3278u 0xCCEu, 11u, 57u, (2u, 11u, 149u)
#define PPUTLIMPL_UDEC_3277u 0xCCDu, 11u, 57u, (29u, 113u)
#define PPUTLIMPL_UDEC_3276u 0xCCCu, 11u, 57u, (2u, 2u, 3u, 3u, 7u, 13u)
#define PPUTLIMPL_UDEC_3275u 0xCCBu, 11u, 57u, (5u, 5u, 131u)
#define PPUTLIMPL_UDEC_3274u 0xCCAu, 11u, 57u, (2u, 1637u)
#define PPUTLIMPL_UDEC_3273u 0xCC9u, 11u, 57u, (3u, 1091u)
#define PPUTLIMPL_UDEC_3272u 0xCC8u, 11u, 57u, (2u, 2u, 2u, 409u)
#define PPUTLIMPL_UDEC_3271u 0xCC7u, 11u, 57u, ()
#define PPUTLIMPL_UDEC_3270u 0xCC6u, 11u, 57u, (2u, 3u, 5u, 109u)
#define PPUTLIMPL_UDEC_3269u 0xCC5u, 11u, 57u, (7u, 467u)
#define PPUTLIMPL_UDEC_3268u 0xCC4u, 11u, 57u, (2u, 2u, 19u, 43u)
#define PPUTLIMPL_UDEC_3267u 0xCC3u, 11u, 57u, (3u, 3u, 3u, 11u, 11u)
#define PPUTLIMPL_UDEC_3266u 0xCC2u, 11u, 57u, (2u, 23u, 71u)
#define PPUTLIMPL_UDEC_3265u 0xCC1u, 11u, 57u, (5u, 653u)
#define PPUTLIMPL_UDEC_3264u 0xCC0u, 11u, 57u, (2u, 2u, 2u, 2u, 2u, 2u, 3u, 17u)
#define PPUTLIMPL_UDEC_3263u 0xCBFu, 11u, 57u, (13u, 251u)
#define PPUTLIMPL_UDEC_3262u 0xCBEu, 11u, 57u, (2u, 7u, 233u)
#define PPUTLIMPL_UDEC_3261u 0xCBDu, 11u, 57u, (3u, 1087u)
#define PPUTLIMPL_UDEC_3260u 0xCBCu, 11u, 57u, (2u, 2u, 5u, 163u)
#define PPUTLIMPL_UDEC_3259u 0xCBBu, 11u, 57u, ()
#define PPUTLIMPL_UDEC_3258u 0xCBAu, 11u, 57u, (2u, 3u, 3u, 181u)
#define PPUTLIMPL_UDEC_3257u 0xCB9u, 11u, 57u, ()
#define PPUTLIMPL_UDEC_3256u 0xCB8u, 11u, 57u, (2u, 2u, 2u, 11u, 37u)
#define PPUTLIMPL_UDEC_3255u 0xCB7u, 11u, 57u, (3u, 5u, 7u, 31u)
#define PPUTLIMPL_UDEC_3254u 0xCB6u, 11u, 57u, (2u, 1627u)
#define PPUTLIMPL_UDEC_3253u 0xCB5u, 11u, 57u, ()
#define PPUTLIMPL_UDEC_3252u 0xCB4u, 11u, 57u, (2u, 2u, 3u, 271u)
#define PPUTLIMPL_UDEC_3251u 0xCB3u, 11u, 57u, ()
#define PPUTLIMPL_UDEC_3250u 0xCB2u, 11u, 57u, (2u, 5u, 5u, 5u, 13u)
#define PPUTLIMPL_UDEC_3249u 0xCB1u, 11u, 57u, (3u, 3u, 19u, 19u)
#define PPUTLIMPL_UDEC_3248u 0xCB0u, 11u, 56u, (2u, 2u, 2u, 2u, 7u, 29u)
#define PPUTLIMPL_UDEC_3247u 0xCAFu, 11u, 56u, (17u, 191u)
#define PPUTLIMPL_UDEC_3246u 0xCAEu, 11u, 56u, (2u, 3u, 541u)
#define PPUTLIMPL_UDEC_3245u 0xCADu, 11u, 56u, (5u, 11u, 59u)
#define PPUTLIMPL_UDEC_3244u 0xCACu, 11u, 56u, (2u, 2u, 811u)
#define PPUTLIMPL_UDEC_3243u 0xCABu, 11u, 56u, (3u, 23u, 47u)
#define PPUTLIMPL_UDEC_3242u 0xCAAu, 11u, 56u, (2u, 1621u)
#define PPUTLIMPL_UDEC_3241u 0xCA9u, 11u, 56u, (7u, 463u)
#define PPUTLIMPL_UDEC_3240u 0xCA8u, 11u, 56u, (2u, 2u, 2u, 3u, 3u, 3u, 3u, 5u)
#define PPUTLIMPL_UDEC_3239u 0xCA7u, 11u, 56u, (41u, 79u)
#define PPUTLIMPL_UDEC_3238u 0xCA6u, 11u, 56u, (2u, 1619u)
#define PPUTLIMPL_UDEC_3237u 0xCA5u, 11u, 56u, (3u, 13u, 83u)
#define PPUTLIMPL_UDEC_3236u 0xCA4u, 11u, 56u, (2u, 2u, 809u)
#define PPUTLIMPL_UDEC_3235u 0xCA3u, 11u, 56u, (5u, 647u)
#define PPUTLIMPL_UDEC_3234u 0xCA2u, 11u, 56u, (2u, 3u, 7u, 7u, 11u)
#define PPUTLIMPL_UDEC_3233u 0xCA1u, 11u, 56u, (53u, 61u)
#define PPUTLIMPL_UDEC_3232u 0xCA0u, 11u, 56u, (2u, 2u, 2u, 2u, 2u, 101u)
#define PPUTLIMPL_UDEC_3231u 0xC9Fu, 11u, 56u, (3u, 3u, 359u)
#define PPUTLIMPL_UDEC_3230u 0xC9Eu, 11u, 56u, (2u, 5u, 17u, 19u)
#define PPUTLIMPL_UDEC_3229u 0xC9Du, 11u, 56u, ()
#define PPUTLIMPL_UDEC_3228u 0xC9Cu, 11u, 56u, (2u, 2u, 3u, 269u)
#define PPUTLIMPL_UDEC_3227u 0xC9Bu, 11u, 56u, (7u, 461u)
#define PPUTLIMPL_UDEC_3226u 0xC9Au, 11u, 56u, (2u, 1613u)
#define PPUTLIMPL_UDEC_3225u 0xC99u, 11u, 56u, (3u, 5u, 5u, 43u)
#define PPUTLIMPL_UDEC_3224u 0xC98u, 11u, 56u, (2u, 2u, 2u, 13u, 31u)
#define PPUTLIMPL_UDEC_3223u 0xC97u, 11u, 56u, (11u, 293u)
#define PPUTLIMPL_UDEC_3222u 0xC96u, 11u, 56u, (2u, 3u, 3u, 179u)
#define PPUTLIMPL_UDEC_3221u 0xC95u, 11u, 56u, ()
#define PPUTLIMPL_UDEC_3220u 0xC94u, 11u, 56u, (2u, 2u, 5u, 7u, 23u)
#define PPUTLIMPL_UDEC_3219u 0xC93u, 11u, 56u, (3u, 29u, 37u)
#define PPUTLIMPL_UDEC_3218u 0xC92u, 11u, 56u, (2u, 1609u)
#define PPUTLIMPL_UDEC_3217u 0xC91u, 11u, 56u, ()
#define PPUTLIMPL_UDEC_3216u 0xC90u, 11u, 56u, (2u, 2u, 2u, 2u, 3u, 67u)
#define PPUTLIMPL_UDEC_3215u 0xC8Fu, 11u, 56u, (5u, 643u)
#define PPUTLIMPL_UDEC_3214u 0xC8Eu, 11u, 56u, (2u, 1607u)
#define PPUTLIMPL_UDEC_3213u 0xC8Du, 11u, 56u, (3u, 3u, 3u, 7u, 17u)
#define PPUTLIMPL_UDEC_3212u 0xC8Cu, 11u, 56u, (2u, 2u, 11u, 73u)
#define PPUTLIMPL_UDEC_3211u 0xC8Bu, 11u, 56u, (13u, 13u, 19u)
#define PPUTLIMPL_UDEC_3210u 0xC8Au, 11u, 56u, (2u, 3u, 5u, 107u)
#define PPUTLIMPL_UDEC_3209u 0xC89u, 11u, 56u, ()
#define PPUTLIMPL_UDEC_3208u 0xC88u, 11u, 56u, (2u, 2u, 2u, 401u)
#define PPUTLIMPL_UDEC_3207u 0xC87u, 11u, 56u, (3u, 1069u)
#define PPUTLIMPL_UDEC_3206u 0xC86u, 11u, 56u, (2u, 7u, 229u)
#define PPUTLIMPL_UDEC_3205u 0xC85u, 11u, 56u, (5u, 641u)
#define PPUTLIMPL_UDEC_3204u 0xC84u, 11u, 56u, (2u, 2u, 3u, 3u, 89u)
#define PPUTLIMPL_UDEC_3203u 0xC83u, 11u, 56u, ()
#define PPUTLIMPL_UDEC_3202u 0xC82u, 11u, 56u, (2u, 1601u)
#define PPUTLIMPL_UDEC_3201u 0xC81u, 11u, 56u, (3u, 11u, 97u)
#define PPUTLIMPL_UDEC_3200u 0xC80u, 11u, 56u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 5u, 5u)
#define PPUTLIMPL_UDEC_3199u 0xC7Fu, 11u, 56u, (7u, 457u)
#define PPUTLIMPL_UDEC_3198u 0xC7Eu, 11u, 56u, (2u, 3u, 13u, 41u)
#define PPUTLIMPL_UDEC_3197u 0xC7Du, 11u, 56u, (23u, 139u)
#define PPUTLIMPL_UDEC_3196u 0xC7Cu, 11u, 56u, (2u, 2u, 17u, 47u)
#define PPUTLIMPL_UDEC_3195u 0xC7Bu, 11u, 56u, (3u, 3u, 5u, 71u)
#define PPUTLIMPL_UDEC_3194u 0xC7Au, 11u, 56u, (2u, 1597u)
#define PPUTLIMPL_UDEC_3193u 0xC79u, 11u, 56u, (31u, 103u)
#define PPUTLIMPL_UDEC_3192u 0xC78u, 11u, 56u, (2u, 2u, 2u, 3u, 7u, 19u)
#define PPUTLIMPL_UDEC_3191u 0xC77u, 11u, 56u, ()
#define PPUTLIMPL_UDEC_3190u 0xC76u, 11u, 56u, (2u, 5u, 11u, 29u)
#define PPUTLIMPL_UDEC_3189u 0xC75u, 11u, 56u, (3u, 1063u)
#define PPUTLIMPL_UDEC_3188u 0xC74u, 11u, 56u, (2u, 2u, 797u)
#define PPUTLIMPL_UDEC_3187u 0xC73u, 11u, 56u, ()
#define PPUTLIMPL_UDEC_3186u 0xC72u, 11u, 56u, (2u, 3u, 3u, 3u, 59u)
#define PPUTLIMPL_UDEC_3185u 0xC71u, 11u, 56u, (5u, 7u, 7u, 13u)
#define PPUTLIMPL_UDEC_3184u 0xC70u, 11u, 56u, (2u, 2u, 2u, 2u, 199u)
#define PPUTLIMPL_UDEC_3183u 0xC6Fu, 11u, 56u, (3u, 1061u)
#define PPUTLIMPL_UDEC_3182u 0xC6Eu, 11u, 56u, (2u, 37u, 43u)
#define PPUTLIMPL_UDEC_3181u 0xC6Du, 11u, 56u, ()
#define PPUTLIMPL_UDEC_3180u 0xC6Cu, 11u, 56u, (2u, 2u, 3u, 5u, 53u)
#define PPUTLIMPL_UDEC_3179u 0xC6Bu, 11u, 56u, (11u, 17u, 17u)
#define PPUTLIMPL_UDEC_3178u 0xC6Au, 11u, 56u, (2u, 7u, 227u)
#define PPUTLIMPL_UDEC_3177u 0xC69u, 11u, 56u, (3u, 3u, 353u)
#define PPUTLIMPL_UDEC_3176u 0xC68u, 11u, 56u, (2u, 2u, 2u, 397u)
#define PPUTLIMPL_UDEC_3175u 0xC67u, 11u, 56u, (5u, 5u, 127u)
#define PPUTLIMPL_UDEC_3174u 0xC66u, 11u, 56u, (2u, 3u, 23u, 23u)
#define PPUTLIMPL_UDEC_3173u 0xC65u, 11u, 56u, (19u, 167u)
#define PPUTLIMPL_UDEC_3172u 0xC64u, 11u, 56u, (2u, 2u, 13u, 61u)
#define PPUTLIMPL_UDEC_3171u 0xC63u, 11u, 56u, (3u, 7u, 151u)
#define PPUTLIMPL_UDEC_3170u 0xC62u, 11u, 56u, (2u, 5u, 317u)
#define PPUTLIMPL_UDEC_3169u 0xC61u, 11u, 56u, ()
#define PPUTLIMPL_UDEC_3168u 0xC60u, 11u, 56u, (2u, 2u, 2u, 2u, 2u, 3u, 3u, 11u)
#define PPUTLIMPL_UDEC_3167u 0xC5Fu, 11u, 56u, ()
#define PPUTLIMPL_UDEC_3166u 0xC5Eu, 11u, 56u, (2u, 1583u)
#define PPUTLIMPL_UDEC_3165u 0xC5Du, 11u, 56u, (3u, 5u, 211u)
#define PPUTLIMPL_UDEC_3164u 0xC5Cu, 11u, 56u, (2u, 2u, 7u, 113u)
#define PPUTLIMPL_UDEC_3163u 0xC5Bu, 11u, 56u, ()
#define PPUTLIMPL_UDEC_3162u 0xC5Au, 11u, 56u, (2u, 3u, 17u, 31u)
#define PPUTLIMPL_UDEC_3161u 0xC59u, 11u, 56u, (29u, 109u)
#define PPUTLIMPL_UDEC_3160u 0xC58u, 11u, 56u, (2u, 2u, 2u, 5u, 79u)
#define PPUTLIMPL_UDEC_3159u 0xC57u, 11u, 56u, (3u, 3u, 3u, 3u, 3u, 13u)
#define PPUTLIMPL_UDEC_3158u 0xC56u, 11u, 56u, (2u, 1579u)
#define PPUTLIMPL_UDEC_3157u 0xC55u, 11u, 56u, (7u, 11u, 41u)
#define PPUTLIMPL_UDEC_3156u 0xC54u, 11u, 56u, (2u, 2u, 3u, 263u)
#define PPUTLIMPL_UDEC_3155u 0xC53u, 11u, 56u, (5u, 631u)
#define PPUTLIMPL_UDEC_3154u 0xC52u, 11u, 56u, (2u, 19u, 83u)
#define PPUTLIMPL_UDEC_3153u 0xC51u, 11u, 56u, (3u, 1051u)
#define PPUTLIMPL_UDEC_3152u 0xC50u, 11u, 56u, (2u, 2u, 2u, 2u, 197u)
#define PPUTLIMPL_UDEC_3151u 0xC4Fu, 11u, 56u, (23u, 137u)
#define PPUTLIMPL_UDEC_3150u 0xC4Eu, 11u, 56u, (2u, 3u, 3u, 5u, 5u, 7u)
#define PPUTLIMPL_UDEC_3149u 0xC4Du, 11u, 56u, (47u, 67u)
#define PPUTLIMPL_UDEC_3148u 0xC4Cu, 11u, 56u, (2u, 2u, 787u)
#define PPUTLIMPL_UDEC_3147u 0xC4Bu, 11u, 56u, (3u, 1049u)
#define PPUTLIMPL_UDEC_3146u 0xC4Au, 11u, 56u, (2u, 11u, 11u, 13u)
#define PPUTLIMPL_UDEC_3145u 0xC49u, 11u, 56u, (5u, 17u, 37u)
#define PPUTLIMPL_UDEC_3144u 0xC48u, 11u, 56u, (2u, 2u, 2u, 3u, 131u)
#define PPUTLIMPL_UDEC_3143u 0xC47u, 11u, 56u, (7u, 449u)
#define PPUTLIMPL_UDEC_3142u 0xC46u, 11u, 56u, (2u, 1571u)
#define PPUTLIMPL_UDEC_3141u 0xC45u, 11u, 56u, (3u, 3u, 349u)
#define PPUTLIMPL_UDEC_3140u 0xC44u, 11u, 56u, (2u, 2u, 5u, 157u)
#define PPUTLIMPL_UDEC_3139u 0xC43u, 11u, 56u, (43u, 73u)
#define PPUTLIMPL_UDEC_3138u 0xC42u, 11u, 56u, (2u, 3u, 523u)
#define PPUTLIMPL_UDEC_3137u 0xC41u, 11u, 56u, ()
#define PPUTLIMPL_UDEC_3136u 0xC40u, 11u, 56u, (2u, 2u, 2u, 2u, 2u, 2u, 7u, 7u)
#define PPUTLIMPL_UDEC_3135u 0xC3Fu, 11u, 55u, (3u, 5u, 11u, 19u)
#define PPUTLIMPL_UDEC_3134u 0xC3Eu, 11u, 55u, (2u, 1567u)
#define PPUTLIMPL_UDEC_3133u 0xC3Du, 11u, 55u, (13u, 241u)
#define PPUTLIMPL_UDEC_3132u 0xC3Cu, 11u, 55u, (2u, 2u, 3u, 3u, 3u, 29u)
#define PPUTLIMPL_UDEC_3131u 0xC3Bu, 11u, 55u, (31u, 101u)
#define PPUTLIMPL_UDEC_3130u 0xC3Au, 11u, 55u, (2u, 5u, 313u)
#define PPUTLIMPL_UDEC_3129u 0xC39u, 11u, 55u, (3u, 7u, 149u)
#define PPUTLIMPL_UDEC_3128u 0xC38u, 11u, 55u, (2u, 2u, 2u, 17u, 23u)
#define PPUTLIMPL_UDEC_3127u 0xC37u, 11u, 55u, (53u, 59u)
#define PPUTLIMPL_UDEC_3126u 0xC36u, 11u, 55u, (2u, 3u, 521u)
#define PPUTLIMPL_UDEC_3125u 0xC35u, 11u, 55u, (5u, 5u, 5u, 5u, 5u)
#define PPUTLIMPL_UDEC_3124u 0xC34u, 11u, 55u, (2u, 2u, 11u, 71u)
#define PPUTLIMPL_UDEC_3123u 0xC33u, 11u, 55u, (3u, 3u, 347u)
#define PPUTLIMPL_UDEC_3122u 0xC32u, 11u, 55u, (2u, 7u, 223u)
#define PPUTLIMPL_UDEC_3121u 0xC31u, 11u, 55u, ()
#define PPUTLIMPL_UDEC_3120u 0xC30u, 11u, 55u, (2u, 2u, 2u, 2u, 3u, 5u, 13u)
#define PPUTLIMPL_UDEC_3119u 0xC2Fu, 11u, 55u, ()
#define PPUTLIMPL_UDEC_3118u 0xC2Eu, 11u, 55u, (2u, 1559u)
#define PPUTLIMPL_UDEC_3117u 0xC2Du, 11u, 55u, (3u, 1039u)
#define PPUTLIMPL_UDEC_3116u 0xC2Cu, 11u, 55u, (2u, 2u, 19u, 41u)
#define PPUTLIMPL_UDEC_3115u 0xC2Bu, 11u, 55u, (5u, 7u, 89u)
#define PPUTLIMPL_UDEC_3114u 0xC2Au, 11u, 55u, (2u, 3u, 3u, 173u)
#define PPUTLIMPL_UDEC_3113u 0xC29u, 11u, 55u, (11u, 283u)
#define PPUTLIMPL_UDEC_3112u 0xC28u, 11u, 55u, (2u, 2u, 2u, 389u)
#define PPUTLIMPL_UDEC_3111u 0xC27u, 11u, 55u, (3u, 17u, 61u)
#define PPUTLIMPL_UDEC_3110u 0xC26u, 11u, 55u, (2u, 5u, 311u)
#define PPUTLIMPL_UDEC_3109u 0xC25u, 11u, 55u, ()
#define PPUTLIMPL_UDEC_3108u 0xC24u, 11u, 55u, (2u, 2u, 3u, 7u, 37u)
#define PPUTLIMPL_UDEC_3107u 0xC23u, 11u, 55u, (13u, 239u)
#define PPUTLIMPL_UDEC_3106u 0xC22u, 11u, 55u, (2u, 1553u)
#define PPUTLIMPL_UDEC_3105u 0xC21u, 11u, 55u, (3u, 3u, 3u, 5u, 23u)
#define PPUTLIMPL_UDEC_3104u 0xC20u, 11u, 55u, (2u, 2u, 2u, 2u, 2u, 97u)
#define PPUTLIMPL_UDEC_3103u 0xC1Fu, 11u, 55u, (29u, 107u)
#define PPUTLIMPL_UDEC_3102u 0xC1Eu, 11u, 55u, (2u, 3u, 11u, 47u)
#define PPUTLIMPL_UDEC_3101u 0xC1Du, 11u, 55u, (7u, 443u)
#define PPUTLIMPL_UDEC_3100u 0xC1Cu, 11u, 55u, (2u, 2u, 5u, 5u, 31u)
#define PPUTLIMPL_UDEC_3099u 0xC1Bu, 11u, 55u, (3u, 1033u)
#define PPUTLIMPL_UDEC_3098u 0xC1Au, 11u, 55u, (2u, 1549u)
#define PPUTLIMPL_UDEC_3097u 0xC19u, 11u, 55u, (19u, 163u)
#define PPUTLIMPL_UDEC_3096u 0xC18u, 11u, 55u, (2u, 2u, 2u, 3u, 3u, 43u)
#define PPUTLIMPL_UDEC_3095u 0xC17u, 11u, 55u, (5u, 619u)
#define PPUTLIMPL_UDEC_3094u 0xC16u, 11u, 55u, (2u, 7u, 13u, 17u)
#define PPUTLIMPL_UDEC_3093u 0xC15u, 11u, 55u, (3u, 1031u)
#define PPUTLIMPL_UDEC_3092u 0xC14u, 11u, 55u, (2u, 2u, 773u)
#define PPUTLIMPL_UDEC_3091u 0xC13u, 11u, 55u, (11u, 281u)
#define PPUTLIMPL_UDEC_3090u 0xC12u, 11u, 55u, (2u, 3u, 5u, 103u)
#define PPUTLIMPL_UDEC_3089u 0xC11u, 11u, 55u, ()
#define PPUTLIMPL_UDEC_3088u 0xC10u, 11u, 55u, (2u, 2u, 2u, 2u, 193u)
#define PPUTLIMPL_UDEC_3087u 0xC0Fu, 11u, 55u, (3u, 3u, 7u, 7u, 7u)
#define PPUTLIMPL_UDEC_3086u 0xC0Eu, 11u, 55u, (2u, 1543u)
#define PPUTLIMPL_UDEC_3085u 0xC0Du, 11u, 55u, (5u, 617u)
#define PPUTLIMPL_UDEC_3084u 0xC0Cu, 11u, 55u, (2u, 2u, 3u, 257u)
#define PPUTLIMPL_UDEC_3083u 0xC0Bu, 11u, 55u, ()
#define PPUTLIMPL_UDEC_3082u 0xC0Au, 11u, 55u, (2u, 23u, 67u)
#define PPUTLIMPL_UDEC_3081u 0xC09u, 11u, 55u, (3u, 13u, 79u)
#define PPUTLIMPL_UDEC_3080u 0xC08u, 11u, 55u, (2u, 2u, 2u, 5u, 7u, 11u)
#define PPUTLIMPL_UDEC_3079u 0xC07u, 11u, 55u, ()
#define PPUTLIMPL_UDEC_3078u 0xC06u, 11u, 55u, (2u, 3u, 3u, 3u, 3u, 19u)
#define PPUTLIMPL_UDEC_3077u 0xC05u, 11u, 55u, (17u, 181u)
#define PPUTLIMPL_UDEC_3076u 0xC04u, 11u, 55u, (2u, 2u, 769u)
#define PPUTLIMPL_UDEC_3075u 0xC03u, 11u, 55u, (3u, 5u, 5u, 41u)
#define PPUTLIMPL_UDEC_3074u 0xC02u, 11u, 55u, (2u, 29u, 53u)
#define PPUTLIMPL_UDEC_3073u 0xC01u, 11u, 55u, (7u, 439u)
#define PPUTLIMPL_UDEC_3072u \
  0xC00u, 11u, 55u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 3u)
#define PPUTLIMPL_UDEC_3071u 0xBFFu, 11u, 55u, (37u, 83u)
#define PPUTLIMPL_UDEC_3070u 0xBFEu, 11u, 55u, (2u, 5u, 307u)
#define PPUTLIMPL_UDEC_3069u 0xBFDu, 11u, 55u, (3u, 3u, 11u, 31u)
#define PPUTLIMPL_UDEC_3068u 0xBFCu, 11u, 55u, (2u, 2u, 13u, 59u)
#define PPUTLIMPL_UDEC_3067u 0xBFBu, 11u, 55u, ()
#define PPUTLIMPL_UDEC_3066u 0xBFAu, 11u, 55u, (2u, 3u, 7u, 73u)
#define PPUTLIMPL_UDEC_3065u 0xBF9u, 11u, 55u, (5u, 613u)
#define PPUTLIMPL_UDEC_3064u 0xBF8u, 11u, 55u, (2u, 2u, 2u, 383u)
#define PPUTLIMPL_UDEC_3063u 0xBF7u, 11u, 55u, (3u, 1021u)
#define PPUTLIMPL_UDEC_3062u 0xBF6u, 11u, 55u, (2u, 1531u)
#define PPUTLIMPL_UDEC_3061u 0xBF5u, 11u, 55u, ()
#define PPUTLIMPL_UDEC_3060u 0xBF4u, 11u, 55u, (2u, 2u, 3u, 3u, 5u, 17u)
#define PPUTLIMPL_UDEC_3059u 0xBF3u, 11u, 55u, (7u, 19u, 23u)
#define PPUTLIMPL_UDEC_3058u 0xBF2u, 11u, 55u, (2u, 11u, 139u)
#define PPUTLIMPL_UDEC_3057u 0xBF1u, 11u, 55u, (3u, 1019u)
#define PPUTLIMPL_UDEC_3056u 0xBF0u, 11u, 55u, (2u, 2u, 2u, 2u, 191u)
#define PPUTLIMPL_UDEC_3055u 0xBEFu, 11u, 55u, (5u, 13u, 47u)
#define PPUTLIMPL_UDEC_3054u 0xBEEu, 11u, 55u, (2u, 3u, 509u)
#define PPUTLIMPL_UDEC_3053u 0xBEDu, 11u, 55u, (43u, 71u)
#define PPUTLIMPL_UDEC_3052u 0xBECu, 11u, 55u, (2u, 2u, 7u, 109u)
#define PPUTLIMPL_UDEC_3051u 0xBEBu, 11u, 55u, (3u, 3u, 3u, 113u)
#define PPUTLIMPL_UDEC_3050u 0xBEAu, 11u, 55u, (2u, 5u, 5u, 61u)
#define PPUTLIMPL_UDEC_3049u 0xBE9u, 11u, 55u, ()
#define PPUTLIMPL_UDEC_3048u 0xBE8u, 11u, 55u, (2u, 2u, 2u, 3u, 127u)
#define PPUTLIMPL_UDEC_3047u 0xBE7u, 11u, 55u, (11u, 277u)
#define PPUTLIMPL_UDEC_3046u 0xBE6u, 11u, 55u, (2u, 1523u)
#define PPUTLIMPL_UDEC_3045u 0xBE5u, 11u, 55u, (3u, 5u, 7u, 29u)
#define PPUTLIMPL_UDEC_3044u 0xBE4u, 11u, 55u, (2u, 2u, 761u)
#define PPUTLIMPL_UDEC_3043u 0xBE3u, 11u, 55u, (17u, 179u)
#define PPUTLIMPL_UDEC_3042u 0xBE2u, 11u, 55u, (2u, 3u, 3u, 13u, 13u)
#define PPUTLIMPL_UDEC_3041u 0xBE1u, 11u, 55u, ()
#define PPUTLIMPL_UDEC_3040u 0xBE0u, 11u, 55u, (2u, 2u, 2u, 2u, 2u, 5u, 19u)
#define PPUTLIMPL_UDEC_3039u 0xBDFu, 11u, 55u, (3u, 1013u)
#define PPUTLIMPL_UDEC_3038u 0xBDEu, 11u, 55u, (2u, 7u, 7u, 31u)
#define PPUTLIMPL_UDEC_3037u 0xBDDu, 11u, 55u, ()
#define PPUTLIMPL_UDEC_3036u 0xBDCu, 11u, 55u, (2u, 2u, 3u, 11u, 23u)
#define PPUTLIMPL_UDEC_3035u 0xBDBu, 11u, 55u, (5u, 607u)
#define PPUTLIMPL_UDEC_3034u 0xBDAu, 11u, 55u, (2u, 37u, 41u)
#define PPUTLIMPL_UDEC_3033u 0xBD9u, 11u, 55u, (3u, 3u, 337u)
#define PPUTLIMPL_UDEC_3032u 0xBD8u, 11u, 55u, (2u, 2u, 2u, 379u)
#define PPUTLIMPL_UDEC_3031u 0xBD7u, 11u, 55u, (7u, 433u)
#define PPUTLIMPL_UDEC_3030u 0xBD6u, 11u, 55u, (2u, 3u, 5u, 101u)
#define PPUTLIMPL_UDEC_3029u 0xBD5u, 11u, 55u, (13u, 233u)
#define PPUTLIMPL_UDEC_3028u 0xBD4u, 11u, 55u, (2u, 2u, 757u)
#define PPUTLIMPL_UDEC_3027u 0xBD3u, 11u, 55u, (3u, 1009u)
#define PPUTLIMPL_UDEC_3026u 0xBD2u, 11u, 55u, (2u, 17u, 89u)
#define PPUTLIMPL_UDEC_3025u 0xBD1u, 11u, 55u, (5u, 5u, 11u, 11u)
#define PPUTLIMPL_UDEC_3024u 0xBD0u, 11u, 54u, (2u, 2u, 2u, 2u, 3u, 3u, 3u, 7u)
#define PPUTLIMPL_UDEC_3023u 0xBCFu, 11u, 54u, ()
#define PPUTLIMPL_UDEC_3022u 0xBCEu, 11u, 54u, (2u, 1511u)
#define PPUTLIMPL_UDEC_3021u 0xBCDu, 11u, 54u, (3u, 19u, 53u)
#define PPUTLIMPL_UDEC_3020u 0xBCCu, 11u, 54u, (2u, 2u, 5u, 151u)
#define PPUTLIMPL_UDEC_3019u 0xBCBu, 11u, 54u, ()
#define PPUTLIMPL_UDEC_3018u 0xBCAu, 11u, 54u, (2u, 3u, 503u)
#define PPUTLIMPL_UDEC_3017u 0xBC9u, 11u, 54u, (7u, 431u)
#define PPUTLIMPL_UDEC_3016u 0xBC8u, 11u, 54u, (2u, 2u, 2u, 13u, 29u)
#define PPUTLIMPL_UDEC_3015u 0xBC7u, 11u, 54u, (3u, 3u, 5u, 67u)
#define PPUTLIMPL_UDEC_3014u 0xBC6u, 11u, 54u, (2u, 11u, 137u)
#define PPUTLIMPL_UDEC_3013u 0xBC5u, 11u, 54u, (23u, 131u)
#define PPUTLIMPL_UDEC_3012u 0xBC4u, 11u, 54u, (2u, 2u, 3u, 251u)
#define PPUTLIMPL_UDEC_3011u 0xBC3u, 11u, 54u, ()
#define PPUTLIMPL_UDEC_3010u 0xBC2u, 11u, 54u, (2u, 5u, 7u, 43u)
#define PPUTLIMPL_UDEC_3009u 0xBC1u, 11u, 54u, (3u, 17u, 59u)
#define PPUTLIMPL_UDEC_3008u 0xBC0u, 11u, 54u, (2u, 2u, 2u, 2u, 2u, 2u, 47u)
#define PPUTLIMPL_UDEC_3007u 0xBBFu, 11u, 54u, (31u, 97u)
#define PPUTLIMPL_UDEC_3006u 0xBBEu, 11u, 54u, (2u, 3u, 3u, 167u)
#define PPUTLIMPL_UDEC_3005u 0xBBDu, 11u, 54u, (5u, 601u)
#define PPUTLIMPL_UDEC_3004u 0xBBCu, 11u, 54u, (2u, 2u, 751u)
#define PPUTLIMPL_UDEC_3003u 0xBBBu, 11u, 54u, (3u, 7u, 11u, 13u)
#define PPUTLIMPL_UDEC_3002u 0xBBAu, 11u, 54u, (2u, 19u, 79u)
#define PPUTLIMPL_UDEC_3001u 0xBB9u, 11u, 54u, ()
#define PPUTLIMPL_UDEC_3000u 0xBB8u, 11u, 54u, (2u, 2u, 2u, 3u, 5u, 5u, 5u)
#define PPUTLIMPL_UDEC_2999u 0xBB7u, 11u, 54u, ()
#define PPUTLIMPL_UDEC_2998u 0xBB6u, 11u, 54u, (2u, 1499u)
#define PPUTLIMPL_UDEC_2997u 0xBB5u, 11u, 54u, (3u, 3u, 3u, 3u, 37u)
#define PPUTLIMPL_UDEC_2996u 0xBB4u, 11u, 54u, (2u, 2u, 7u, 107u)
#define PPUTLIMPL_UDEC_2995u 0xBB3u, 11u, 54u, (5u, 599u)
#define PPUTLIMPL_UDEC_2994u 0xBB2u, 11u, 54u, (2u, 3u, 499u)
#define PPUTLIMPL_UDEC_2993u 0xBB1u, 11u, 54u, (41u, 73u)
#define PPUTLIMPL_UDEC_2992u 0xBB0u, 11u, 54u, (2u, 2u, 2u, 2u, 11u, 17u)
#define PPUTLIMPL_UDEC_2991u 0xBAFu, 11u, 54u, (3u, 997u)
#define PPUTLIMPL_UDEC_2990u 0xBAEu, 11u, 54u, (2u, 5u, 13u, 23u)
#define PPUTLIMPL_UDEC_2989u 0xBADu, 11u, 54u, (7u, 7u, 61u)
#define PPUTLIMPL_UDEC_2988u 0xBACu, 11u, 54u, (2u, 2u, 3u, 3u, 83u)
#define PPUTLIMPL_UDEC_2987u 0xBABu, 11u, 54u, (29u, 103u)
#define PPUTLIMPL_UDEC_2986u 0xBAAu, 11u, 54u, (2u, 1493u)
#define PPUTLIMPL_UDEC_2985u 0xBA9u, 11u, 54u, (3u, 5u, 199u)
#define PPUTLIMPL_UDEC_2984u 0xBA8u, 11u, 54u, (2u, 2u, 2u, 373u)
#define PPUTLIMPL_UDEC_2983u 0xBA7u, 11u, 54u, (19u, 157u)
#define PPUTLIMPL_UDEC_2982u 0xBA6u, 11u, 54u, (2u, 3u, 7u, 71u)
#define PPUTLIMPL_UDEC_2981u 0xBA5u, 11u, 54u, (11u, 271u)
#define PPUTLIMPL_UDEC_2980u 0xBA4u, 11u, 54u, (2u, 2u, 5u, 149u)
#define PPUTLIMPL_UDEC_2979u 0xBA3u, 11u, 54u, (3u, 3u, 331u)
#define PPUTLIMPL_UDEC_2978u 0xBA2u, 11u, 54u, (2u, 1489u)
#define PPUTLIMPL_UDEC_2977u 0xBA1u, 11u, 54u, (13u, 229u)
#define PPUTLIMPL_UDEC_2976u 0xBA0u, 11u, 54u, (2u, 2u, 2u, 2u, 2u, 3u, 31u)
#define PPUTLIMPL_UDEC_2975u 0xB9Fu, 11u, 54u, (5u, 5u, 7u, 17u)
#define PPUTLIMPL_UDEC_2974u 0xB9Eu, 11u, 54u, (2u, 1487u)
#define PPUTLIMPL_UDEC_2973u 0xB9Du, 11u, 54u, (3u, 991u)
#define PPUTLIMPL_UDEC_2972u 0xB9Cu, 11u, 54u, (2u, 2u, 743u)
#define PPUTLIMPL_UDEC_2971u 0xB9Bu, 11u, 54u, ()
#define PPUTLIMPL_UDEC_2970u 0xB9Au, 11u, 54u, (2u, 3u, 3u, 3u, 5u, 11u)
#define PPUTLIMPL_UDEC_2969u 0xB99u, 11u, 54u, ()
#define PPUTLIMPL_UDEC_2968u 0xB98u, 11u, 54u, (2u, 2u, 2u, 7u, 53u)
#define PPUTLIMPL_UDEC_2967u 0xB97u, 11u, 54u, (3u, 23u, 43u)
#define PPUTLIMPL_UDEC_2966u 0xB96u, 11u, 54u, (2u, 1483u)
#define PPUTLIMPL_UDEC_2965u 0xB95u, 11u, 54u, (5u, 593u)
#define PPUTLIMPL_UDEC_2964u 0xB94u, 11u, 54u, (2u, 2u, 3u, 13u, 19u)
#define PPUTLIMPL_UDEC_2963u 0xB93u, 11u, 54u, ()
#define PPUTLIMPL_UDEC_2962u 0xB92u, 11u, 54u, (2u, 1481u)
#define PPUTLIMPL_UDEC_2961u 0xB91u, 11u, 54u, (3u, 3u, 7u, 47u)
#define PPUTLIMPL_UDEC_2960u 0xB90u, 11u, 54u, (2u, 2u, 2u, 2u, 5u, 37u)
#define PPUTLIMPL_UDEC_2959u 0xB8Fu, 11u, 54u, (11u, 269u)
#define PPUTLIMPL_UDEC_2958u 0xB8Eu, 11u, 54u, (2u, 3u, 17u, 29u)
#define PPUTLIMPL_UDEC_2957u 0xB8Du, 11u, 54u, ()
#define PPUTLIMPL_UDEC_2956u 0xB8Cu, 11u, 54u, (2u, 2u, 739u)
#define PPUTLIMPL_UDEC_2955u 0xB8Bu, 11u, 54u, (3u, 5u, 197u)
#define PPUTLIMPL_UDEC_2954u 0xB8Au, 11u, 54u, (2u, 7u, 211u)
#define PPUTLIMPL_UDEC_2953u 0xB89u, 11u, 54u, ()
#define PPUTLIMPL_UDEC_2952u 0xB88u, 11u, 54u, (2u, 2u, 2u, 3u, 3u, 41u)
#define PPUTLIMPL_UDEC_2951u 0xB87u, 11u, 54u, (13u, 227u)
#define PPUTLIMPL_UDEC_2950u 0xB86u, 11u, 54u, (2u, 5u, 5u, 59u)
#define PPUTLIMPL_UDEC_2949u 0xB85u, 11u, 54u, (3u, 983u)
#define PPUTLIMPL_UDEC_2948u 0xB84u, 11u, 54u, (2u, 2u, 11u, 67u)
#define PPUTLIMPL_UDEC_2947u 0xB83u, 11u, 54u, (7u, 421u)
#define PPUTLIMPL_UDEC_2946u 0xB82u, 11u, 54u, (2u, 3u, 491u)
#define PPUTLIMPL_UDEC_2945u 0xB81u, 11u, 54u, (5u, 19u, 31u)
#define PPUTLIMPL_UDEC_2944u 0xB80u, 11u, 54u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 23u)
#define PPUTLIMPL_UDEC_2943u 0xB7Fu, 11u, 54u, (3u, 3u, 3u, 109u)
#define PPUTLIMPL_UDEC_2942u 0xB7Eu, 11u, 54u, (2u, 1471u)
#define PPUTLIMPL_UDEC_2941u 0xB7Du, 11u, 54u, (17u, 173u)
#define PPUTLIMPL_UDEC_2940u 0xB7Cu, 11u, 54u, (2u, 2u, 3u, 5u, 7u, 7u)
#define PPUTLIMPL_UDEC_2939u 0xB7Bu, 11u, 54u, ()
#define PPUTLIMPL_UDEC_2938u 0xB7Au, 11u, 54u, (2u, 13u, 113u)
#define PPUTLIMPL_UDEC_2937u 0xB79u, 11u, 54u, (3u, 11u, 89u)
#define PPUTLIMPL_UDEC_2936u 0xB78u, 11u, 54u, (2u, 2u, 2u, 367u)
#define PPUTLIMPL_UDEC_2935u 0xB77u, 11u, 54u, (5u, 587u)
#define PPUTLIMPL_UDEC_2934u 0xB76u, 11u, 54u, (2u, 3u, 3u, 163u)
#define PPUTLIMPL_UDEC_2933u 0xB75u, 11u, 54u, (7u, 419u)
#define PPUTLIMPL_UDEC_2932u 0xB74u, 11u, 54u, (2u, 2u, 733u)
#define PPUTLIMPL_UDEC_2931u 0xB73u, 11u, 54u, (3u, 977u)
#define PPUTLIMPL_UDEC_2930u 0xB72u, 11u, 54u, (2u, 5u, 293u)
#define PPUTLIMPL_UDEC_2929u 0xB71u, 11u, 54u, (29u, 101u)
#define PPUTLIMPL_UDEC_2928u 0xB70u, 11u, 54u, (2u, 2u, 2u, 2u, 3u, 61u)
#define PPUTLIMPL_UDEC_2927u 0xB6Fu, 11u, 54u, ()
#define PPUTLIMPL_UDEC_2926u 0xB6Eu, 11u, 54u, (2u, 7u, 11u, 19u)
#define PPUTLIMPL_UDEC_2925u 0xB6Du, 11u, 54u, (3u, 3u, 5u, 5u, 13u)
#define PPUTLIMPL_UDEC_2924u 0xB6Cu, 11u, 54u, (2u, 2u, 17u, 43u)
#define PPUTLIMPL_UDEC_2923u 0xB6Bu, 11u, 54u, (37u, 79u)
#define PPUTLIMPL_UDEC_2922u 0xB6Au, 11u, 54u, (2u, 3u, 487u)
#define PPUTLIMPL_UDEC_2921u 0xB69u, 11u, 54u, (23u, 127u)
#define PPUTLIMPL_UDEC_2920u 0xB68u, 11u, 54u, (2u, 2u, 2u, 5u, 73u)
#define PPUTLIMPL_UDEC_2919u 0xB67u, 11u, 54u, (3u, 7u, 139u)
#define PPUTLIMPL_UDEC_2918u 0xB66u, 11u, 54u, (2u, 1459u)
#define PPUTLIMPL_UDEC_2917u 0xB65u, 11u, 54u, ()
#define PPUTLIMPL_UDEC_2916u 0xB64u, 11u, 54u, (2u, 2u, 3u, 3u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UDEC_2915u 0xB63u, 11u, 53u, (5u, 11u, 53u)
#define PPUTLIMPL_UDEC_2914u 0xB62u, 11u, 53u, (2u, 31u, 47u)
#define PPUTLIMPL_UDEC_2913u 0xB61u, 11u, 53u, (3u, 971u)
#define PPUTLIMPL_UDEC_2912u 0xB60u, 11u, 53u, (2u, 2u, 2u, 2u, 2u, 7u, 13u)
#define PPUTLIMPL_UDEC_2911u 0xB5Fu, 11u, 53u, (41u, 71u)
#define PPUTLIMPL_UDEC_2910u 0xB5Eu, 11u, 53u, (2u, 3u, 5u, 97u)
#define PPUTLIMPL_UDEC_2909u 0xB5Du, 11u, 53u, ()
#define PPUTLIMPL_UDEC_2908u 0xB5Cu, 11u, 53u, (2u, 2u, 727u)
#define PPUTLIMPL_UDEC_2907u 0xB5Bu, 11u, 53u, (3u, 3u, 17u, 19u)
#define PPUTLIMPL_UDEC_2906u 0xB5Au, 11u, 53u, (2u, 1453u)
#define PPUTLIMPL_UDEC_2905u 0xB59u, 11u, 53u, (5u, 7u, 83u)
#define PPUTLIMPL_UDEC_2904u 0xB58u, 11u, 53u, (2u, 2u, 2u, 3u, 11u, 11u)
#define PPUTLIMPL_UDEC_2903u 0xB57u, 11u, 53u, ()
#define PPUTLIMPL_UDEC_2902u 0xB56u, 11u, 53u, (2u, 1451u)
#define PPUTLIMPL_UDEC_2901u 0xB55u, 11u, 53u, (3u, 967u)
#define PPUTLIMPL_UDEC_2900u 0xB54u, 11u, 53u, (2u, 2u, 5u, 5u, 29u)
#define PPUTLIMPL_UDEC_2899u 0xB53u, 11u, 53u, (13u, 223u)
#define PPUTLIMPL_UDEC_2898u 0xB52u, 11u, 53u, (2u, 3u, 3u, 7u, 23u)
#define PPUTLIMPL_UDEC_2897u 0xB51u, 11u, 53u, ()
#define PPUTLIMPL_UDEC_2896u 0xB50u, 11u, 53u, (2u, 2u, 2u, 2u, 181u)
#define PPUTLIMPL_UDEC_2895u 0xB4Fu, 11u, 53u, (3u, 5u, 193u)
#define PPUTLIMPL_UDEC_2894u 0xB4Eu, 11u, 53u, (2u, 1447u)
#define PPUTLIMPL_UDEC_2893u 0xB4Du, 11u, 53u, (11u, 263u)
#define PPUTLIMPL_UDEC_2892u 0xB4Cu, 11u, 53u, (2u, 2u, 3u, 241u)
#define PPUTLIMPL_UDEC_2891u 0xB4Bu, 11u, 53u, (7u, 7u, 59u)
#define PPUTLIMPL_UDEC_2890u 0xB4Au, 11u, 53u, (2u, 5u, 17u, 17u)
#define PPUTLIMPL_UDEC_2889u 0xB49u, 11u, 53u, (3u, 3u, 3u, 107u)
#define PPUTLIMPL_UDEC_2888u 0xB48u, 11u, 53u, (2u, 2u, 2u, 19u, 19u)
#define PPUTLIMPL_UDEC_2887u 0xB47u, 11u, 53u, ()
#define PPUTLIMPL_UDEC_2886u 0xB46u, 11u, 53u, (2u, 3u, 13u, 37u)
#define PPUTLIMPL_UDEC_2885u 0xB45u, 11u, 53u, (5u, 577u)
#define PPUTLIMPL_UDEC_2884u 0xB44u, 11u, 53u, (2u, 2u, 7u, 103u)
#define PPUTLIMPL_UDEC_2883u 0xB43u, 11u, 53u, (3u, 31u, 31u)
#define PPUTLIMPL_UDEC_2882u 0xB42u, 11u, 53u, (2u, 11u, 131u)
#define PPUTLIMPL_UDEC_2881u 0xB41u, 11u, 53u, (43u, 67u)
#define PPUTLIMPL_UDEC_2880u 0xB40u, 11u, 53u, (2u, 2u, 2u, 2u, 2u, 2u, 3u, 3u, 5u)
#define PPUTLIMPL_UDEC_2879u 0xB3Fu, 11u, 53u, ()
#define PPUTLIMPL_UDEC_2878u 0xB3Eu, 11u, 53u, (2u, 1439u)
#define PPUTLIMPL_UDEC_2877u 0xB3Du, 11u, 53u, (3u, 7u, 137u)
#define PPUTLIMPL_UDEC_2876u 0xB3Cu, 11u, 53u, (2u, 2u, 719u)
#define PPUTLIMPL_UDEC_2875u 0xB3Bu, 11u, 53u, (5u, 5u, 5u, 23u)
#define PPUTLIMPL_UDEC_2874u 0xB3Au, 11u, 53u, (2u, 3u, 479u)
#define PPUTLIMPL_UDEC_2873u 0xB39u, 11u, 53u, (13u, 13u, 17u)
#define PPUTLIMPL_UDEC_2872u 0xB38u, 11u, 53u, (2u, 2u, 2u, 359u)
#define PPUTLIMPL_UDEC_2871u 0xB37u, 11u, 53u, (3u, 3u, 11u, 29u)
#define PPUTLIMPL_UDEC_2870u 0xB36u, 11u, 53u, (2u, 5u, 7u, 41u)
#define PPUTLIMPL_UDEC_2869u 0xB35u, 11u, 53u, (19u, 151u)
#define PPUTLIMPL_UDEC_2868u 0xB34u, 11u, 53u, (2u, 2u, 3u, 239u)
#define PPUTLIMPL_UDEC_2867u 0xB33u, 11u, 53u, (47u, 61u)
#define PPUTLIMPL_UDEC_2866u 0xB32u, 11u, 53u, (2u, 1433u)
#define PPUTLIMPL_UDEC_2865u 0xB31u, 11u, 53u, (3u, 5u, 191u)
#define PPUTLIMPL_UDEC_2864u 0xB30u, 11u, 53u, (2u, 2u, 2u, 2u, 179u)
#define PPUTLIMPL_UDEC_2863u 0xB2Fu, 11u, 53u, (7u, 409u)
#define PPUTLIMPL_UDEC_2862u 0xB2Eu, 11u, 53u, (2u, 3u, 3u, 3u, 53u)
#define PPUTLIMPL_UDEC_2861u 0xB2Du, 11u, 53u, ()
#define PPUTLIMPL_UDEC_2860u 0xB2Cu, 11u, 53u, (2u, 2u, 5u, 11u, 13u)
#define PPUTLIMPL_UDEC_2859u 0xB2Bu, 11u, 53u, (3u, 953u)
#define PPUTLIMPL_UDEC_2858u 0xB2Au, 11u, 53u, (2u, 1429u)
#define PPUTLIMPL_UDEC_2857u 0xB29u, 11u, 53u, ()
#define PPUTLIMPL_UDEC_2856u 0xB28u, 11u, 53u, (2u, 2u, 2u, 3u, 7u, 17u)
#define PPUTLIMPL_UDEC_2855u 0xB27u, 11u, 53u, (5u, 571u)
#define PPUTLIMPL_UDEC_2854u 0xB26u, 11u, 53u, (2u, 1427u)
#define PPUTLIMPL_UDEC_2853u 0xB25u, 11u, 53u, (3u, 3u, 317u)
#define PPUTLIMPL_UDEC_2852u 0xB24u, 11u, 53u, (2u, 2u, 23u, 31u)
#define PPUTLIMPL_UDEC_2851u 0xB23u, 11u, 53u, ()
#define PPUTLIMPL_UDEC_2850u 0xB22u, 11u, 53u, (2u, 3u, 5u, 5u, 19u)
#define PPUTLIMPL_UDEC_2849u 0xB21u, 11u, 53u, (7u, 11u, 37u)
#define PPUTLIMPL_UDEC_2848u 0xB20u, 11u, 53u, (2u, 2u, 2u, 2u, 2u, 89u)
#define PPUTLIMPL_UDEC_2847u 0xB1Fu, 11u, 53u, (3u, 13u, 73u)
#define PPUTLIMPL_UDEC_2846u 0xB1Eu, 11u, 53u, (2u, 1423u)
#define PPUTLIMPL_UDEC_2845u 0xB1Du, 11u, 53u, (5u, 569u)
#define PPUTLIMPL_UDEC_2844u 0xB1Cu, 11u, 53u, (2u, 2u, 3u, 3u, 79u)
#define PPUTLIMPL_UDEC_2843u 0xB1Bu, 11u, 53u, ()
#define PPUTLIMPL_UDEC_2842u 0xB1Au, 11u, 53u, (2u, 7u, 7u, 29u)
#define PPUTLIMPL_UDEC_2841u 0xB19u, 11u, 53u, (3u, 947u)
#define PPUTLIMPL_UDEC_2840u 0xB18u, 11u, 53u, (2u, 2u, 2u, 5u, 71u)
#define PPUTLIMPL_UDEC_2839u 0xB17u, 11u, 53u, (17u, 167u)
#define PPUTLIMPL_UDEC_2838u 0xB16u, 11u, 53u, (2u, 3u, 11u, 43u)
#define PPUTLIMPL_UDEC_2837u 0xB15u, 11u, 53u, ()
#define PPUTLIMPL_UDEC_2836u 0xB14u, 11u, 53u, (2u, 2u, 709u)
#define PPUTLIMPL_UDEC_2835u 0xB13u, 11u, 53u, (3u, 3u, 3u, 3u, 5u, 7u)
#define PPUTLIMPL_UDEC_2834u 0xB12u, 11u, 53u, (2u, 13u, 109u)
#define PPUTLIMPL_UDEC_2833u 0xB11u, 11u, 53u, ()
#define PPUTLIMPL_UDEC_2832u 0xB10u, 11u, 53u, (2u, 2u, 2u, 2u, 3u, 59u)
#define PPUTLIMPL_UDEC_2831u 0xB0Fu, 11u, 53u, (19u, 149u)
#define PPUTLIMPL_UDEC_2830u 0xB0Eu, 11u, 53u, (2u, 5u, 283u)
#define PPUTLIMPL_UDEC_2829u 0xB0Du, 11u, 53u, (3u, 23u, 41u)
#define PPUTLIMPL_UDEC_2828u 0xB0Cu, 11u, 53u, (2u, 2u, 7u, 101u)
#define PPUTLIMPL_UDEC_2827u 0xB0Bu, 11u, 53u, (11u, 257u)
#define PPUTLIMPL_UDEC_2826u 0xB0Au, 11u, 53u, (2u, 3u, 3u, 157u)
#define PPUTLIMPL_UDEC_2825u 0xB09u, 11u, 53u, (5u, 5u, 113u)
#define PPUTLIMPL_UDEC_2824u 0xB08u, 11u, 53u, (2u, 2u, 2u, 353u)
#define PPUTLIMPL_UDEC_2823u 0xB07u, 11u, 53u, (3u, 941u)
#define PPUTLIMPL_UDEC_2822u 0xB06u, 11u, 53u, (2u, 17u, 83u)
#define PPUTLIMPL_UDEC_2821u 0xB05u, 11u, 53u, (7u, 13u, 31u)
#define PPUTLIMPL_UDEC_2820u 0xB04u, 11u, 53u, (2u, 2u, 3u, 5u, 47u)
#define PPUTLIMPL_UDEC_2819u 0xB03u, 11u, 53u, ()
#define PPUTLIMPL_UDEC_2818u 0xB02u, 11u, 53u, (2u, 1409u)
#define PPUTLIMPL_UDEC_2817u 0xB01u, 11u, 53u, (3u, 3u, 313u)
#define PPUTLIMPL_UDEC_2816u 0xB00u, 11u, 53u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 11u)
#define PPUTLIMPL_UDEC_2815u 0xAFFu, 11u, 53u, (5u, 563u)
#define PPUTLIMPL_UDEC_2814u 0xAFEu, 11u, 53u, (2u, 3u, 7u, 67u)
#define PPUTLIMPL_UDEC_2813u 0xAFDu, 11u, 53u, (29u, 97u)
#define PPUTLIMPL_UDEC_2812u 0xAFCu, 11u, 53u, (2u, 2u, 19u, 37u)
#define PPUTLIMPL_UDEC_2811u 0xAFBu, 11u, 53u, (3u, 937u)
#define PPUTLIMPL_UDEC_2810u 0xAFAu, 11u, 53u, (2u, 5u, 281u)
#define PPUTLIMPL_UDEC_2809u 0xAF9u, 11u, 53u, (53u, 53u)
#define PPUTLIMPL_UDEC_2808u 0xAF8u, 11u, 52u, (2u, 2u, 2u, 3u, 3u, 3u, 13u)
#define PPUTLIMPL_UDEC_2807u 0xAF7u, 11u, 52u, (7u, 401u)
#define PPUTLIMPL_UDEC_2806u 0xAF6u, 11u, 52u, (2u, 23u, 61u)
#define PPUTLIMPL_UDEC_2805u 0xAF5u, 11u, 52u, (3u, 5u, 11u, 17u)
#define PPUTLIMPL_UDEC_2804u 0xAF4u, 11u, 52u, (2u, 2u, 701u)
#define PPUTLIMPL_UDEC_2803u 0xAF3u, 11u, 52u, ()
#define PPUTLIMPL_UDEC_2802u 0xAF2u, 11u, 52u, (2u, 3u, 467u)
#define PPUTLIMPL_UDEC_2801u 0xAF1u, 11u, 52u, ()
#define PPUTLIMPL_UDEC_2800u 0xAF0u, 11u, 52u, (2u, 2u, 2u, 2u, 5u, 5u, 7u)
#define PPUTLIMPL_UDEC_2799u 0xAEFu, 11u, 52u, (3u, 3u, 311u)
#define PPUTLIMPL_UDEC_2798u 0xAEEu, 11u, 52u, (2u, 1399u)
#define PPUTLIMPL_UDEC_2797u 0xAEDu, 11u, 52u, ()
#define PPUTLIMPL_UDEC_2796u 0xAECu, 11u, 52u, (2u, 2u, 3u, 233u)
#define PPUTLIMPL_UDEC_2795u 0xAEBu, 11u, 52u, (5u, 13u, 43u)
#define PPUTLIMPL_UDEC_2794u 0xAEAu, 11u, 52u, (2u, 11u, 127u)
#define PPUTLIMPL_UDEC_2793u 0xAE9u, 11u, 52u, (3u, 7u, 7u, 19u)
#define PPUTLIMPL_UDEC_2792u 0xAE8u, 11u, 52u, (2u, 2u, 2u, 349u)
#define PPUTLIMPL_UDEC_2791u 0xAE7u, 11u, 52u, ()
#define PPUTLIMPL_UDEC_2790u 0xAE6u, 11u, 52u, (2u, 3u, 3u, 5u, 31u)
#define PPUTLIMPL_UDEC_2789u 0xAE5u, 11u, 52u, ()
#define PPUTLIMPL_UDEC_2788u 0xAE4u, 11u, 52u, (2u, 2u, 17u, 41u)
#define PPUTLIMPL_UDEC_2787u 0xAE3u, 11u, 52u, (3u, 929u)
#define PPUTLIMPL_UDEC_2786u 0xAE2u, 11u, 52u, (2u, 7u, 199u)
#define PPUTLIMPL_UDEC_2785u 0xAE1u, 11u, 52u, (5u, 557u)
#define PPUTLIMPL_UDEC_2784u 0xAE0u, 11u, 52u, (2u, 2u, 2u, 2u, 2u, 3u, 29u)
#define PPUTLIMPL_UDEC_2783u 0xADFu, 11u, 52u, (11u, 11u, 23u)
#define PPUTLIMPL_UDEC_2782u 0xADEu, 11u, 52u, (2u, 13u, 107u)
#define PPUTLIMPL_UDEC_2781u 0xADDu, 11u, 52u, (3u, 3u, 3u, 103u)
#define PPUTLIMPL_UDEC_2780u 0xADCu, 11u, 52u, (2u, 2u, 5u, 139u)
#define PPUTLIMPL_UDEC_2779u 0xADBu, 11u, 52u, (7u, 397u)
#define PPUTLIMPL_UDEC_2778u 0xADAu, 11u, 52u, (2u, 3u, 463u)
#define PPUTLIMPL_UDEC_2777u 0xAD9u, 11u, 52u, ()
#define PPUTLIMPL_UDEC_2776u 0xAD8u, 11u, 52u, (2u, 2u, 2u, 347u)
#define PPUTLIMPL_UDEC_2775u 0xAD7u, 11u, 52u, (3u, 5u, 5u, 37u)
#define PPUTLIMPL_UDEC_2774u 0xAD6u, 11u, 52u, (2u, 19u, 73u)
#define PPUTLIMPL_UDEC_2773u 0xAD5u, 11u, 52u, (47u, 59u)
#define PPUTLIMPL_UDEC_2772u 0xAD4u, 11u, 52u, (2u, 2u, 3u, 3u, 7u, 11u)
#define PPUTLIMPL_UDEC_2771u 0xAD3u, 11u, 52u, (17u, 163u)
#define PPUTLIMPL_UDEC_2770u 0xAD2u, 11u, 52u, (2u, 5u, 277u)
#define PPUTLIMPL_UDEC_2769u 0xAD1u, 11u, 52u, (3u, 13u, 71u)
#define PPUTLIMPL_UDEC_2768u 0xAD0u, 11u, 52u, (2u, 2u, 2u, 2u, 173u)
#define PPUTLIMPL_UDEC_2767u 0xACFu, 11u, 52u, ()
#define PPUTLIMPL_UDEC_2766u 0xACEu, 11u, 52u, (2u, 3u, 461u)
#define PPUTLIMPL_UDEC_2765u 0xACDu, 11u, 52u, (5u, 7u, 79u)
#define PPUTLIMPL_UDEC_2764u 0xACCu, 11u, 52u, (2u, 2u, 691u)
#define PPUTLIMPL_UDEC_2763u 0xACBu, 11u, 52u, (3u, 3u, 307u)
#define PPUTLIMPL_UDEC_2762u 0xACAu, 11u, 52u, (2u, 1381u)
#define PPUTLIMPL_UDEC_2761u 0xAC9u, 11u, 52u, (11u, 251u)
#define PPUTLIMPL_UDEC_2760u 0xAC8u, 11u, 52u, (2u, 2u, 2u, 3u, 5u, 23u)
#define PPUTLIMPL_UDEC_2759u 0xAC7u, 11u, 52u, (31u, 89u)
#define PPUTLIMPL_UDEC_2758u 0xAC6u, 11u, 52u, (2u, 7u, 197u)
#define PPUTLIMPL_UDEC_2757u 0xAC5u, 11u, 52u, (3u, 919u)
#define PPUTLIMPL_UDEC_2756u 0xAC4u, 11u, 52u, (2u, 2u, 13u, 53u)
#define PPUTLIMPL_UDEC_2755u 0xAC3u, 11u, 52u, (5u, 19u, 29u)
#define PPUTLIMPL_UDEC_2754u 0xAC2u, 11u, 52u, (2u, 3u, 3u, 3u, 3u, 17u)
#define PPUTLIMPL_UDEC_2753u 0xAC1u, 11u, 52u, ()
#define PPUTLIMPL_UDEC_2752u 0xAC0u, 11u, 52u, (2u, 2u, 2u, 2u, 2u, 2u, 43u)
#define PPUTLIMPL_UDEC_2751u 0xABFu, 11u, 52u, (3u, 7u, 131u)
#define PPUTLIMPL_UDEC_2750u 0xABEu, 11u, 52u, (2u, 5u, 5u, 5u, 11u)
#define PPUTLIMPL_UDEC_2749u 0xABDu, 11u, 52u, ()
#define PPUTLIMPL_UDEC_2748u 0xABCu, 11u, 52u, (2u, 2u, 3u, 229u)
#define PPUTLIMPL_UDEC_2747u 0xABBu, 11u, 52u, (41u, 67u)
#define PPUTLIMPL_UDEC_2746u 0xABAu, 11u, 52u, (2u, 1373u)
#define PPUTLIMPL_UDEC_2745u 0xAB9u, 11u, 52u, (3u, 3u, 5u, 61u)
#define PPUTLIMPL_UDEC_2744u 0xAB8u, 11u, 52u, (2u, 2u, 2u, 7u, 7u, 7u)
#define PPUTLIMPL_UDEC_2743u 0xAB7u, 11u, 52u, (13u, 211u)
#define PPUTLIMPL_UDEC_2742u 0xAB6u, 11u, 52u, (2u, 3u, 457u)
#define PPUTLIMPL_UDEC_2741u 0xAB5u, 11u, 52u, ()
#define PPUTLIMPL_UDEC_2740u 0xAB4u, 11u, 52u, (2u, 2u, 5u, 137u)
#define PPUTLIMPL_UDEC_2739u 0xAB3u, 11u, 52u, (3u, 11u, 83u)
#define PPUTLIMPL_UDEC_2738u 0xAB2u, 11u, 52u, (2u, 37u, 37u)
#define PPUTLIMPL_UDEC_2737u 0xAB1u, 11u, 52u, (7u, 17u, 23u)
#define PPUTLIMPL_UDEC_2736u 0xAB0u, 11u, 52u, (2u, 2u, 2u, 2u, 3u, 3u, 19u)
#define PPUTLIMPL_UDEC_2735u 0xAAFu, 11u, 52u, (5u, 547u)
#define PPUTLIMPL_UDEC_2734u 0xAAEu, 11u, 52u, (2u, 1367u)
#define PPUTLIMPL_UDEC_2733u 0xAADu, 11u, 52u, (3u, 911u)
#define PPUTLIMPL_UDEC_2732u 0xAACu, 11u, 52u, (2u, 2u, 683u)
#define PPUTLIMPL_UDEC_2731u 0xAABu, 11u, 52u, ()
#define PPUTLIMPL_UDEC_2730u 0xAAAu, 11u, 52u, (2u, 3u, 5u, 7u, 13u)
#define PPUTLIMPL_UDEC_2729u 0xAA9u, 11u, 52u, ()
#define PPUTLIMPL_UDEC_2728u 0xAA8u, 11u, 52u, (2u, 2u, 2u, 11u, 31u)
#define PPUTLIMPL_UDEC_2727u 0xAA7u, 11u, 52u, (3u, 3u, 3u, 101u)
#define PPUTLIMPL_UDEC_2726u 0xAA6u, 11u, 52u, (2u, 29u, 47u)
#define PPUTLIMPL_UDEC_2725u 0xAA5u, 11u, 52u, (5u, 5u, 109u)
#define PPUTLIMPL_UDEC_2724u 0xAA4u, 11u, 52u, (2u, 2u, 3u, 227u)
#define PPUTLIMPL_UDEC_2723u 0xAA3u, 11u, 52u, (7u, 389u)
#define PPUTLIMPL_UDEC_2722u 0xAA2u, 11u, 52u, (2u, 1361u)
#define PPUTLIMPL_UDEC_2721u 0xAA1u, 11u, 52u, (3u, 907u)
#define PPUTLIMPL_UDEC_2720u 0xAA0u, 11u, 52u, (2u, 2u, 2u, 2u, 2u, 5u, 17u)
#define PPUTLIMPL_UDEC_2719u 0xA9Fu, 11u, 52u, ()
#define PPUTLIMPL_UDEC_2718u 0xA9Eu, 11u, 52u, (2u, 3u, 3u, 151u)
#define PPUTLIMPL_UDEC_2717u 0xA9Du, 11u, 52u, (11u, 13u, 19u)
#define PPUTLIMPL_UDEC_2716u 0xA9Cu, 11u, 52u, (2u, 2u, 7u, 97u)
#define PPUTLIMPL_UDEC_2715u 0xA9Bu, 11u, 52u, (3u, 5u, 181u)
#define PPUTLIMPL_UDEC_2714u 0xA9Au, 11u, 52u, (2u, 23u, 59u)
#define PPUTLIMPL_UDEC_2713u 0xA99u, 11u, 52u, ()
#define PPUTLIMPL_UDEC_2712u 0xA98u, 11u, 52u, (2u, 2u, 2u, 3u, 113u)
#define PPUTLIMPL_UDEC_2711u 0xA97u, 11u, 52u, ()
#define PPUTLIMPL_UDEC_2710u 0xA96u, 11u, 52u, (2u, 5u, 271u)
#define PPUTLIMPL_UDEC_2709u 0xA95u, 11u, 52u, (3u, 3u, 7u, 43u)
#define PPUTLIMPL_UDEC_2708u 0xA94u, 11u, 52u, (2u, 2u, 677u)
#define PPUTLIMPL_UDEC_2707u 0xA93u, 11u, 52u, ()
#define PPUTLIMPL_UDEC_2706u 0xA92u, 11u, 52u, (2u, 3u, 11u, 41u)
#define PPUTLIMPL_UDEC_2705u 0xA91u, 11u, 52u, (5u, 541u)
#define PPUTLIMPL_UDEC_2704u 0xA90u, 11u, 52u, (2u, 2u, 2u, 2u, 13u, 13u)
#define PPUTLIMPL_UDEC_2703u 0xA8Fu, 11u, 51u, (3u, 17u, 53u)
#define PPUTLIMPL_UDEC_2702u 0xA8Eu, 11u, 51u, (2u, 7u, 193u)
#define PPUTLIMPL_UDEC_2701u 0xA8Du, 11u, 51u, (37u, 73u)
#define PPUTLIMPL_UDEC_2700u 0xA8Cu, 11u, 51u, (2u, 2u, 3u, 3u, 3u, 5u, 5u)
#define PPUTLIMPL_UDEC_2699u 0xA8Bu, 11u, 51u, ()
#define PPUTLIMPL_UDEC_2698u 0xA8Au, 11u, 51u, (2u, 19u, 71u)
#define PPUTLIMPL_UDEC_2697u 0xA89u, 11u, 51u, (3u, 29u, 31u)
#define PPUTLIMPL_UDEC_2696u 0xA88u, 11u, 51u, (2u, 2u, 2u, 337u)
#define PPUTLIMPL_UDEC_2695u 0xA87u, 11u, 51u, (5u, 7u, 7u, 11u)
#define PPUTLIMPL_UDEC_2694u 0xA86u, 11u, 51u, (2u, 3u, 449u)
#define PPUTLIMPL_UDEC_2693u 0xA85u, 11u, 51u, ()
#define PPUTLIMPL_UDEC_2692u 0xA84u, 11u, 51u, (2u, 2u, 673u)
#define PPUTLIMPL_UDEC_2691u 0xA83u, 11u, 51u, (3u, 3u, 13u, 23u)
#define PPUTLIMPL_UDEC_2690u 0xA82u, 11u, 51u, (2u, 5u, 269u)
#define PPUTLIMPL_UDEC_2689u 0xA81u, 11u, 51u, ()
#define PPUTLIMPL_UDEC_2688u 0xA80u, 11u, 51u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 3u, 7u)
#define PPUTLIMPL_UDEC_2687u 0xA7Fu, 11u, 51u, ()
#define PPUTLIMPL_UDEC_2686u 0xA7Eu, 11u, 51u, (2u, 17u, 79u)
#define PPUTLIMPL_UDEC_2685u 0xA7Du, 11u, 51u, (3u, 5u, 179u)
#define PPUTLIMPL_UDEC_2684u 0xA7Cu, 11u, 51u, (2u, 2u, 11u, 61u)
#define PPUTLIMPL_UDEC_2683u 0xA7Bu, 11u, 51u, ()
#define PPUTLIMPL_UDEC_2682u 0xA7Au, 11u, 51u, (2u, 3u, 3u, 149u)
#define PPUTLIMPL_UDEC_2681u 0xA79u, 11u, 51u, (7u, 383u)
#define PPUTLIMPL_UDEC_2680u 0xA78u, 11u, 51u, (2u, 2u, 2u, 5u, 67u)
#define PPUTLIMPL_UDEC_2679u 0xA77u, 11u, 51u, (3u, 19u, 47u)
#define PPUTLIMPL_UDEC_2678u 0xA76u, 11u, 51u, (2u, 13u, 103u)
#define PPUTLIMPL_UDEC_2677u 0xA75u, 11u, 51u, ()
#define PPUTLIMPL_UDEC_2676u 0xA74u, 11u, 51u, (2u, 2u, 3u, 223u)
#define PPUTLIMPL_UDEC_2675u 0xA73u, 11u, 51u, (5u, 5u, 107u)
#define PPUTLIMPL_UDEC_2674u 0xA72u, 11u, 51u, (2u, 7u, 191u)
#define PPUTLIMPL_UDEC_2673u 0xA71u, 11u, 51u, (3u, 3u, 3u, 3u, 3u, 11u)
#define PPUTLIMPL_UDEC_2672u 0xA70u, 11u, 51u, (2u, 2u, 2u, 2u, 167u)
#define PPUTLIMPL_UDEC_2671u 0xA6Fu, 11u, 51u, ()
#define PPUTLIMPL_UDEC_2670u 0xA6Eu, 11u, 51u, (2u, 3u, 5u, 89u)
#define PPUTLIMPL_UDEC_2669u 0xA6Du, 11u, 51u, (17u, 157u)
#define PPUTLIMPL_UDEC_2668u 0xA6Cu, 11u, 51u, (2u, 2u, 23u, 29u)
#define PPUTLIMPL_UDEC_2667u 0xA6Bu, 11u, 51u, (3u, 7u, 127u)
#define PPUTLIMPL_UDEC_2666u 0xA6Au, 11u, 51u, (2u, 31u, 43u)
#define PPUTLIMPL_UDEC_2665u 0xA69u, 11u, 51u, (5u, 13u, 41u)
#define PPUTLIMPL_UDEC_2664u 0xA68u, 11u, 51u, (2u, 2u, 2u, 3u, 3u, 37u)
#define PPUTLIMPL_UDEC_2663u 0xA67u, 11u, 51u, ()
#define PPUTLIMPL_UDEC_2662u 0xA66u, 11u, 51u, (2u, 11u, 11u, 11u)
#define PPUTLIMPL_UDEC_2661u 0xA65u, 11u, 51u, (3u, 887u)
#define PPUTLIMPL_UDEC_2660u 0xA64u, 11u, 51u, (2u, 2u, 5u, 7u, 19u)
#define PPUTLIMPL_UDEC_2659u 0xA63u, 11u, 51u, ()
#define PPUTLIMPL_UDEC_2658u 0xA62u, 11u, 51u, (2u, 3u, 443u)
#define PPUTLIMPL_UDEC_2657u 0xA61u, 11u, 51u, ()
#define PPUTLIMPL_UDEC_2656u 0xA60u, 11u, 51u, (2u, 2u, 2u, 2u, 2u, 83u)
#define PPUTLIMPL_UDEC_2655u 0xA5Fu, 11u, 51u, (3u, 3u, 5u, 59u)
#define PPUTLIMPL_UDEC_2654u 0xA5Eu, 11u, 51u, (2u, 1327u)
#define PPUTLIMPL_UDEC_2653u 0xA5Du, 11u, 51u, (7u, 379u)
#define PPUTLIMPL_UDEC_2652u 0xA5Cu, 11u, 51u, (2u, 2u, 3u, 13u, 17u)
#define PPUTLIMPL_UDEC_2651u 0xA5Bu, 11u, 51u, (11u, 241u)
#define PPUTLIMPL_UDEC_2650u 0xA5Au, 11u, 51u, (2u, 5u, 5u, 53u)
#define PPUTLIMPL_UDEC_2649u 0xA59u, 11u, 51u, (3u, 883u)
#define PPUTLIMPL_UDEC_2648u 0xA58u, 11u, 51u, (2u, 2u, 2u, 331u)
#define PPUTLIMPL_UDEC_2647u 0xA57u, 11u, 51u, ()
#define PPUTLIMPL_UDEC_2646u 0xA56u, 11u, 51u, (2u, 3u, 3u, 3u, 7u, 7u)
#define PPUTLIMPL_UDEC_2645u 0xA55u, 11u, 51u, (5u, 23u, 23u)
#define PPUTLIMPL_UDEC_2644u 0xA54u, 11u, 51u, (2u, 2u, 661u)
#define PPUTLIMPL_UDEC_2643u 0xA53u, 11u, 51u, (3u, 881u)
#define PPUTLIMPL_UDEC_2642u 0xA52u, 11u, 51u, (2u, 1321u)
#define PPUTLIMPL_UDEC_2641u 0xA51u, 11u, 51u, (19u, 139u)
#define PPUTLIMPL_UDEC_2640u 0xA50u, 11u, 51u, (2u, 2u, 2u, 2u, 3u, 5u, 11u)
#define PPUTLIMPL_UDEC_2639u 0xA4Fu, 11u, 51u, (7u, 13u, 29u)
#define PPUTLIMPL_UDEC_2638u 0xA4Eu, 11u, 51u, (2u, 1319u)
#define PPUTLIMPL_UDEC_2637u 0xA4Du, 11u, 51u, (3u, 3u, 293u)
#define PPUTLIMPL_UDEC_2636u 0xA4Cu, 11u, 51u, (2u, 2u, 659u)
#define PPUTLIMPL_UDEC_2635u 0xA4Bu, 11u, 51u, (5u, 17u, 31u)
#define PPUTLIMPL_UDEC_2634u 0xA4Au, 11u, 51u, (2u, 3u, 439u)
#define PPUTLIMPL_UDEC_2633u 0xA49u, 11u, 51u, ()
#define PPUTLIMPL_UDEC_2632u 0xA48u, 11u, 51u, (2u, 2u, 2u, 7u, 47u)
#define PPUTLIMPL_UDEC_2631u 0xA47u, 11u, 51u, (3u, 877u)
#define PPUTLIMPL_UDEC_2630u 0xA46u, 11u, 51u, (2u, 5u, 263u)
#define PPUTLIMPL_UDEC_2629u 0xA45u, 11u, 51u, (11u, 239u)
#define PPUTLIMPL_UDEC_2628u 0xA44u, 11u, 51u, (2u, 2u, 3u, 3u, 73u)
#define PPUTLIMPL_UDEC_2627u 0xA43u, 11u, 51u, (37u, 71u)
#define PPUTLIMPL_UDEC_2626u 0xA42u, 11u, 51u, (2u, 13u, 101u)
#define PPUTLIMPL_UDEC_2625u 0xA41u, 11u, 51u, (3u, 5u, 5u, 5u, 7u)
#define PPUTLIMPL_UDEC_2624u 0xA40u, 11u, 51u, (2u, 2u, 2u, 2u, 2u, 2u, 41u)
#define PPUTLIMPL_UDEC_2623u 0xA3Fu, 11u, 51u, (43u, 61u)
#define PPUTLIMPL_UDEC_2622u 0xA3Eu, 11u, 51u, (2u, 3u, 19u, 23u)
#define PPUTLIMPL_UDEC_2621u 0xA3Du, 11u, 51u, ()
#define PPUTLIMPL_UDEC_2620u 0xA3Cu, 11u, 51u, (2u, 2u, 5u, 131u)
#define PPUTLIMPL_UDEC_2619u 0xA3Bu, 11u, 51u, (3u, 3u, 3u, 97u)
#define PPUTLIMPL_UDEC_2618u 0xA3Au, 11u, 51u, (2u, 7u, 11u, 17u)
#define PPUTLIMPL_UDEC_2617u 0xA39u, 11u, 51u, ()
#define PPUTLIMPL_UDEC_2616u 0xA38u, 11u, 51u, (2u, 2u, 2u, 3u, 109u)
#define PPUTLIMPL_UDEC_2615u 0xA37u, 11u, 51u, (5u, 523u)
#define PPUTLIMPL_UDEC_2614u 0xA36u, 11u, 51u, (2u, 1307u)
#define PPUTLIMPL_UDEC_2613u 0xA35u, 11u, 51u, (3u, 13u, 67u)
#define PPUTLIMPL_UDEC_2612u 0xA34u, 11u, 51u, (2u, 2u, 653u)
#define PPUTLIMPL_UDEC_2611u 0xA33u, 11u, 51u, (7u, 373u)
#define PPUTLIMPL_UDEC_2610u 0xA32u, 11u, 51u, (2u, 3u, 3u, 5u, 29u)
#define PPUTLIMPL_UDEC_2609u 0xA31u, 11u, 51u, ()
#define PPUTLIMPL_UDEC_2608u 0xA30u, 11u, 51u, (2u, 2u, 2u, 2u, 163u)
#define PPUTLIMPL_UDEC_2607u 0xA2Fu, 11u, 51u, (3u, 11u, 79u)
#define PPUTLIMPL_UDEC_2606u 0xA2Eu, 11u, 51u, (2u, 1303u)
#define PPUTLIMPL_UDEC_2605u 0xA2Du, 11u, 51u, (5u, 521u)
#define PPUTLIMPL_UDEC_2604u 0xA2Cu, 11u, 51u, (2u, 2u, 3u, 7u, 31u)
#define PPUTLIMPL_UDEC_2603u 0xA2Bu, 11u, 51u, (19u, 137u)
#define PPUTLIMPL_UDEC_2602u 0xA2Au, 11u, 51u, (2u, 1301u)
#define PPUTLIMPL_UDEC_2601u 0xA29u, 11u, 51u, (3u, 3u, 17u, 17u)
#define PPUTLIMPL_UDEC_2600u 0xA28u, 11u, 50u, (2u, 2u, 2u, 5u, 5u, 13u)
#define PPUTLIMPL_UDEC_2599u 0xA27u, 11u, 50u, (23u, 113u)
#define PPUTLIMPL_UDEC_2598u 0xA26u, 11u, 50u, (2u, 3u, 433u)
#define PPUTLIMPL_UDEC_2597u 0xA25u, 11u, 50u, (7u, 7u, 53u)
#define PPUTLIMPL_UDEC_2596u 0xA24u, 11u, 50u, (2u, 2u, 11u, 59u)
#define PPUTLIMPL_UDEC_2595u 0xA23u, 11u, 50u, (3u, 5u, 173u)
#define PPUTLIMPL_UDEC_2594u 0xA22u, 11u, 50u, (2u, 1297u)
#define PPUTLIMPL_UDEC_2593u 0xA21u, 11u, 50u, ()
#define PPUTLIMPL_UDEC_2592u 0xA20u, 11u, 50u, (2u, 2u, 2u, 2u, 2u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UDEC_2591u 0xA1Fu, 11u, 50u, ()
#define PPUTLIMPL_UDEC_2590u 0xA1Eu, 11u, 50u, (2u, 5u, 7u, 37u)
#define PPUTLIMPL_UDEC_2589u 0xA1Du, 11u, 50u, (3u, 863u)
#define PPUTLIMPL_UDEC_2588u 0xA1Cu, 11u, 50u, (2u, 2u, 647u)
#define PPUTLIMPL_UDEC_2587u 0xA1Bu, 11u, 50u, (13u, 199u)
#define PPUTLIMPL_UDEC_2586u 0xA1Au, 11u, 50u, (2u, 3u, 431u)
#define PPUTLIMPL_UDEC_2585u 0xA19u, 11u, 50u, (5u, 11u, 47u)
#define PPUTLIMPL_UDEC_2584u 0xA18u, 11u, 50u, (2u, 2u, 2u, 17u, 19u)
#define PPUTLIMPL_UDEC_2583u 0xA17u, 11u, 50u, (3u, 3u, 7u, 41u)
#define PPUTLIMPL_UDEC_2582u 0xA16u, 11u, 50u, (2u, 1291u)
#define PPUTLIMPL_UDEC_2581u 0xA15u, 11u, 50u, (29u, 89u)
#define PPUTLIMPL_UDEC_2580u 0xA14u, 11u, 50u, (2u, 2u, 3u, 5u, 43u)
#define PPUTLIMPL_UDEC_2579u 0xA13u, 11u, 50u, ()
#define PPUTLIMPL_UDEC_2578u 0xA12u, 11u, 50u, (2u, 1289u)
#define PPUTLIMPL_UDEC_2577u 0xA11u, 11u, 50u, (3u, 859u)
#define PPUTLIMPL_UDEC_2576u 0xA10u, 11u, 50u, (2u, 2u, 2u, 2u, 7u, 23u)
#define PPUTLIMPL_UDEC_2575u 0xA0Fu, 11u, 50u, (5u, 5u, 103u)
#define PPUTLIMPL_UDEC_2574u 0xA0Eu, 11u, 50u, (2u, 3u, 3u, 11u, 13u)
#define PPUTLIMPL_UDEC_2573u 0xA0Du, 11u, 50u, (31u, 83u)
#define PPUTLIMPL_UDEC_2572u 0xA0Cu, 11u, 50u, (2u, 2u, 643u)
#define PPUTLIMPL_UDEC_2571u 0xA0Bu, 11u, 50u, (3u, 857u)
#define PPUTLIMPL_UDEC_2570u 0xA0Au, 11u, 50u, (2u, 5u, 257u)
#define PPUTLIMPL_UDEC_2569u 0xA09u, 11u, 50u, (7u, 367u)
#define PPUTLIMPL_UDEC_2568u 0xA08u, 11u, 50u, (2u, 2u, 2u, 3u, 107u)
#define PPUTLIMPL_UDEC_2567u 0xA07u, 11u, 50u, (17u, 151u)
#define PPUTLIMPL_UDEC_2566u 0xA06u, 11u, 50u, (2u, 1283u)
#define PPUTLIMPL_UDEC_2565u 0xA05u, 11u, 50u, (3u, 3u, 3u, 5u, 19u)
#define PPUTLIMPL_UDEC_2564u 0xA04u, 11u, 50u, (2u, 2u, 641u)
#define PPUTLIMPL_UDEC_2563u 0xA03u, 11u, 50u, (11u, 233u)
#define PPUTLIMPL_UDEC_2562u 0xA02u, 11u, 50u, (2u, 3u, 7u, 61u)
#define PPUTLIMPL_UDEC_2561u 0xA01u, 11u, 50u, (13u, 197u)
#define PPUTLIMPL_UDEC_2560u 0xA00u, 11u, 50u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 5u)
#define PPUTLIMPL_UDEC_2559u 0x9FFu, 11u, 50u, (3u, 853u)
#define PPUTLIMPL_UDEC_2558u 0x9FEu, 11u, 50u, (2u, 1279u)
#define PPUTLIMPL_UDEC_2557u 0x9FDu, 11u, 50u, ()
#define PPUTLIMPL_UDEC_2556u 0x9FCu, 11u, 50u, (2u, 2u, 3u, 3u, 71u)
#define PPUTLIMPL_UDEC_2555u 0x9FBu, 11u, 50u, (5u, 7u, 73u)
#define PPUTLIMPL_UDEC_2554u 0x9FAu, 11u, 50u, (2u, 1277u)
#define PPUTLIMPL_UDEC_2553u 0x9F9u, 11u, 50u, (3u, 23u, 37u)
#define PPUTLIMPL_UDEC_2552u 0x9F8u, 11u, 50u, (2u, 2u, 2u, 11u, 29u)
#define PPUTLIMPL_UDEC_2551u 0x9F7u, 11u, 50u, ()
#define PPUTLIMPL_UDEC_2550u 0x9F6u, 11u, 50u, (2u, 3u, 5u, 5u, 17u)
#define PPUTLIMPL_UDEC_2549u 0x9F5u, 11u, 50u, ()
#define PPUTLIMPL_UDEC_2548u 0x9F4u, 11u, 50u, (2u, 2u, 7u, 7u, 13u)
#define PPUTLIMPL_UDEC_2547u 0x9F3u, 11u, 50u, (3u, 3u, 283u)
#define PPUTLIMPL_UDEC_2546u 0x9F2u, 11u, 50u, (2u, 19u, 67u)
#define PPUTLIMPL_UDEC_2545u 0x9F1u, 11u, 50u, (5u, 509u)
#define PPUTLIMPL_UDEC_2544u 0x9F0u, 11u, 50u, (2u, 2u, 2u, 2u, 3u, 53u)
#define PPUTLIMPL_UDEC_2543u 0x9EFu, 11u, 50u, ()
#define PPUTLIMPL_UDEC_2542u 0x9EEu, 11u, 50u, (2u, 31u, 41u)
#define PPUTLIMPL_UDEC_2541u 0x9EDu, 11u, 50u, (3u, 7u, 11u, 11u)
#define PPUTLIMPL_UDEC_2540u 0x9ECu, 11u, 50u, (2u, 2u, 5u, 127u)
#define PPUTLIMPL_UDEC_2539u 0x9EBu, 11u, 50u, ()
#define PPUTLIMPL_UDEC_2538u 0x9EAu, 11u, 50u, (2u, 3u, 3u, 3u, 47u)
#define PPUTLIMPL_UDEC_2537u 0x9E9u, 11u, 50u, (43u, 59u)
#define PPUTLIMPL_UDEC_2536u 0x9E8u, 11u, 50u, (2u, 2u, 2u, 317u)
#define PPUTLIMPL_UDEC_2535u 0x9E7u, 11u, 50u, (3u, 5u, 13u, 13u)
#define PPUTLIMPL_UDEC_2534u 0x9E6u, 11u, 50u, (2u, 7u, 181u)
#define PPUTLIMPL_UDEC_2533u 0x9E5u, 11u, 50u, (17u, 149u)
#define PPUTLIMPL_UDEC_2532u 0x9E4u, 11u, 50u, (2u, 2u, 3u, 211u)
#define PPUTLIMPL_UDEC_2531u 0x9E3u, 11u, 50u, ()
#define PPUTLIMPL_UDEC_2530u 0x9E2u, 11u, 50u, (2u, 5u, 11u, 23u)
#define PPUTLIMPL_UDEC_2529u 0x9E1u, 11u, 50u, (3u, 3u, 281u)
#define PPUTLIMPL_UDEC_2528u 0x9E0u, 11u, 50u, (2u, 2u, 2u, 2u, 2u, 79u)
#define PPUTLIMPL_UDEC_2527u 0x9DFu, 11u, 50u, (7u, 19u, 19u)
#define PPUTLIMPL_UDEC_2526u 0x9DEu, 11u, 50u, (2u, 3u, 421u)
#define PPUTLIMPL_UDEC_2525u 0x9DDu, 11u, 50u, (5u, 5u, 101u)
#define PPUTLIMPL_UDEC_2524u 0x9DCu, 11u, 50u, (2u, 2u, 631u)
#define PPUTLIMPL_UDEC_2523u 0x9DBu, 11u, 50u, (3u, 29u, 29u)
#define PPUTLIMPL_UDEC_2522u 0x9DAu, 11u, 50u, (2u, 13u, 97u)
#define PPUTLIMPL_UDEC_2521u 0x9D9u, 11u, 50u, ()
#define PPUTLIMPL_UDEC_2520u 0x9D8u, 11u, 50u, (2u, 2u, 2u, 3u, 3u, 5u, 7u)
#define PPUTLIMPL_UDEC_2519u 0x9D7u, 11u, 50u, (11u, 229u)
#define PPUTLIMPL_UDEC_2518u 0x9D6u, 11u, 50u, (2u, 1259u)
#define PPUTLIMPL_UDEC_2517u 0x9D5u, 11u, 50u, (3u, 839u)
#define PPUTLIMPL_UDEC_2516u 0x9D4u, 11u, 50u, (2u, 2u, 17u, 37u)
#define PPUTLIMPL_UDEC_2515u 0x9D3u, 11u, 50u, (5u, 503u)
#define PPUTLIMPL_UDEC_2514u 0x9D2u, 11u, 50u, (2u, 3u, 419u)
#define PPUTLIMPL_UDEC_2513u 0x9D1u, 11u, 50u, (7u, 359u)
#define PPUTLIMPL_UDEC_2512u 0x9D0u, 11u, 50u, (2u, 2u, 2u, 2u, 157u)
#define PPUTLIMPL_UDEC_2511u 0x9CFu, 11u, 50u, (3u, 3u, 3u, 3u, 31u)
#define PPUTLIMPL_UDEC_2510u 0x9CEu, 11u, 50u, (2u, 5u, 251u)
#define PPUTLIMPL_UDEC_2509u 0x9CDu, 11u, 50u, (13u, 193u)
#define PPUTLIMPL_UDEC_2508u 0x9CCu, 11u, 50u, (2u, 2u, 3u, 11u, 19u)
#define PPUTLIMPL_UDEC_2507u 0x9CBu, 11u, 50u, (23u, 109u)
#define PPUTLIMPL_UDEC_2506u 0x9CAu, 11u, 50u, (2u, 7u, 179u)
#define PPUTLIMPL_UDEC_2505u 0x9C9u, 11u, 50u, (3u, 5u, 167u)
#define PPUTLIMPL_UDEC_2504u 0x9C8u, 11u, 50u, (2u, 2u, 2u, 313u)
#define PPUTLIMPL_UDEC_2503u 0x9C7u, 11u, 50u, ()
#define PPUTLIMPL_UDEC_2502u 0x9C6u, 11u, 50u, (2u, 3u, 3u, 139u)
#define PPUTLIMPL_UDEC_2501u 0x9C5u, 11u, 50u, (41u, 61u)
#define PPUTLIMPL_UDEC_2500u 0x9C4u, 11u, 50u, (2u, 2u, 5u, 5u, 5u, 5u)
#define PPUTLIMPL_UDEC_2499u 0x9C3u, 11u, 49u, (3u, 7u, 7u, 17u)
#define PPUTLIMPL_UDEC_2498u 0x9C2u, 11u, 49u, (2u, 1249u)
#define PPUTLIMPL_UDEC_2497u 0x9C1u, 11u, 49u, (11u, 227u)
#define PPUTLIMPL_UDEC_2496u 0x9C0u, 11u, 49u, (2u, 2u, 2u, 2u, 2u, 2u, 3u, 13u)
#define PPUTLIMPL_UDEC_2495u 0x9BFu, 11u, 49u, (5u, 499u)
#define PPUTLIMPL_UDEC_2494u 0x9BEu, 11u, 49u, (2u, 29u, 43u)
#define PPUTLIMPL_UDEC_2493u 0x9BDu, 11u, 49u, (3u, 3u, 277u)
#define PPUTLIMPL_UDEC_2492u 0x9BCu, 11u, 49u, (2u, 2u, 7u, 89u)
#define PPUTLIMPL_UDEC_2491u 0x9BBu, 11u, 49u, (47u, 53u)
#define PPUTLIMPL_UDEC_2490u 0x9BAu, 11u, 49u, (2u, 3u, 5u, 83u)
#define PPUTLIMPL_UDEC_2489u 0x9B9u, 11u, 49u, (19u, 131u)
#define PPUTLIMPL_UDEC_2488u 0x9B8u, 11u, 49u, (2u, 2u, 2u, 311u)
#define PPUTLIMPL_UDEC_2487u 0x9B7u, 11u, 49u, (3u, 829u)
#define PPUTLIMPL_UDEC_2486u 0x9B6u, 11u, 49u, (2u, 11u, 113u)
#define PPUTLIMPL_UDEC_2485u 0x9B5u, 11u, 49u, (5u, 7u, 71u)
#define PPUTLIMPL_UDEC_2484u 0x9B4u, 11u, 49u, (2u, 2u, 3u, 3u, 3u, 23u)
#define PPUTLIMPL_UDEC_2483u 0x9B3u, 11u, 49u, (13u, 191u)
#define PPUTLIMPL_UDEC_2482u 0x9B2u, 11u, 49u, (2u, 17u, 73u)
#define PPUTLIMPL_UDEC_2481u 0x9B1u, 11u, 49u, (3u, 827u)
#define PPUTLIMPL_UDEC_2480u 0x9B0u, 11u, 49u, (2u, 2u, 2u, 2u, 5u, 31u)
#define PPUTLIMPL_UDEC_2479u 0x9AFu, 11u, 49u, (37u, 67u)
#define PPUTLIMPL_UDEC_2478u 0x9AEu, 11u, 49u, (2u, 3u, 7u, 59u)
#define PPUTLIMPL_UDEC_2477u 0x9ADu, 11u, 49u, ()
#define PPUTLIMPL_UDEC_2476u 0x9ACu, 11u, 49u, (2u, 2u, 619u)
#define PPUTLIMPL_UDEC_2475u 0x9ABu, 11u, 49u, (3u, 3u, 5u, 5u, 11u)
#define PPUTLIMPL_UDEC_2474u 0x9AAu, 11u, 49u, (2u, 1237u)
#define PPUTLIMPL_UDEC_2473u 0x9A9u, 11u, 49u, ()
#define PPUTLIMPL_UDEC_2472u 0x9A8u, 11u, 49u, (2u, 2u, 2u, 3u, 103u)
#define PPUTLIMPL_UDEC_2471u 0x9A7u, 11u, 49u, (7u, 353u)
#define PPUTLIMPL_UDEC_2470u 0x9A6u, 11u, 49u, (2u, 5u, 13u, 19u)
#define PPUTLIMPL_UDEC_2469u 0x9A5u, 11u, 49u, (3u, 823u)
#define PPUTLIMPL_UDEC_2468u 0x9A4u, 11u, 49u, (2u, 2u, 617u)
#define PPUTLIMPL_UDEC_2467u 0x9A3u, 11u, 49u, ()
#define PPUTLIMPL_UDEC_2466u 0x9A2u, 11u, 49u, (2u, 3u, 3u, 137u)
#define PPUTLIMPL_UDEC_2465u 0x9A1u, 11u, 49u, (5u, 17u, 29u)
#define PPUTLIMPL_UDEC_2464u 0x9A0u, 11u, 49u, (2u, 2u, 2u, 2u, 2u, 7u, 11u)
#define PPUTLIMPL_UDEC_2463u 0x99Fu, 11u, 49u, (3u, 821u)
#define PPUTLIMPL_UDEC_2462u 0x99Eu, 11u, 49u, (2u, 1231u)
#define PPUTLIMPL_UDEC_2461u 0x99Du, 11u, 49u, (23u, 107u)
#define PPUTLIMPL_UDEC_2460u 0x99Cu, 11u, 49u, (2u, 2u, 3u, 5u, 41u)
#define PPUTLIMPL_UDEC_2459u 0x99Bu, 11u, 49u, ()
#define PPUTLIMPL_UDEC_2458u 0x99Au, 11u, 49u, (2u, 1229u)
#define PPUTLIMPL_UDEC_2457u 0x999u, 11u, 49u, (3u, 3u, 3u, 7u, 13u)
#define PPUTLIMPL_UDEC_2456u 0x998u, 11u, 49u, (2u, 2u, 2u, 307u)
#define PPUTLIMPL_UDEC_2455u 0x997u, 11u, 49u, (5u, 491u)
#define PPUTLIMPL_UDEC_2454u 0x996u, 11u, 49u, (2u, 3u, 409u)
#define PPUTLIMPL_UDEC_2453u 0x995u, 11u, 49u, (11u, 223u)
#define PPUTLIMPL_UDEC_2452u 0x994u, 11u, 49u, (2u, 2u, 613u)
#define PPUTLIMPL_UDEC_2451u 0x993u, 11u, 49u, (3u, 19u, 43u)
#define PPUTLIMPL_UDEC_2450u 0x992u, 11u, 49u, (2u, 5u, 5u, 7u, 7u)
#define PPUTLIMPL_UDEC_2449u 0x991u, 11u, 49u, (31u, 79u)
#define PPUTLIMPL_UDEC_2448u 0x990u, 11u, 49u, (2u, 2u, 2u, 2u, 3u, 3u, 17u)
#define PPUTLIMPL_UDEC_2447u 0x98Fu, 11u, 49u, ()
#define PPUTLIMPL_UDEC_2446u 0x98Eu, 11u, 49u, (2u, 1223u)
#define PPUTLIMPL_UDEC_2445u 0x98Du, 11u, 49u, (3u, 5u, 163u)
#define PPUTLIMPL_UDEC_2444u 0x98Cu, 11u, 49u, (2u, 2u, 13u, 47u)
#define PPUTLIMPL_UDEC_2443u 0x98Bu, 11u, 49u, (7u, 349u)
#define PPUTLIMPL_UDEC_2442u 0x98Au, 11u, 49u, (2u, 3u, 11u, 37u)
#define PPUTLIMPL_UDEC_2441u 0x989u, 11u, 49u, ()
#define PPUTLIMPL_UDEC_2440u 0x988u, 11u, 49u, (2u, 2u, 2u, 5u, 61u)
#define PPUTLIMPL_UDEC_2439u 0x987u, 11u, 49u, (3u, 3u, 271u)
#define PPUTLIMPL_UDEC_2438u 0x986u, 11u, 49u, (2u, 23u, 53u)
#define PPUTLIMPL_UDEC_2437u 0x985u, 11u, 49u, ()
#define PPUTLIMPL_UDEC_2436u 0x984u, 11u, 49u, (2u, 2u, 3u, 7u, 29u)
#define PPUTLIMPL_UDEC_2435u 0x983u, 11u, 49u, (5u, 487u)
#define PPUTLIMPL_UDEC_2434u 0x982u, 11u, 49u, (2u, 1217u)
#define PPUTLIMPL_UDEC_2433u 0x981u, 11u, 49u, (3u, 811u)
#define PPUTLIMPL_UDEC_2432u 0x980u, 11u, 49u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 19u)
#define PPUTLIMPL_UDEC_2431u 0x97Fu, 11u, 49u, (11u, 13u, 17u)
#define PPUTLIMPL_UDEC_2430u 0x97Eu, 11u, 49u, (2u, 3u, 3u, 3u, 3u, 3u, 5u)
#define PPUTLIMPL_UDEC_2429u 0x97Du, 11u, 49u, (7u, 347u)
#define PPUTLIMPL_UDEC_2428u 0x97Cu, 11u, 49u, (2u, 2u, 607u)
#define PPUTLIMPL_UDEC_2427u 0x97Bu, 11u, 49u, (3u, 809u)
#define PPUTLIMPL_UDEC_2426u 0x97Au, 11u, 49u, (2u, 1213u)
#define PPUTLIMPL_UDEC_2425u 0x979u, 11u, 49u, (5u, 5u, 97u)
#define PPUTLIMPL_UDEC_2424u 0x978u, 11u, 49u, (2u, 2u, 2u, 3u, 101u)
#define PPUTLIMPL_UDEC_2423u 0x977u, 11u, 49u, ()
#define PPUTLIMPL_UDEC_2422u 0x976u, 11u, 49u, (2u, 7u, 173u)
#define PPUTLIMPL_UDEC_2421u 0x975u, 11u, 49u, (3u, 3u, 269u)
#define PPUTLIMPL_UDEC_2420u 0x974u, 11u, 49u, (2u, 2u, 5u, 11u, 11u)
#define PPUTLIMPL_UDEC_2419u 0x973u, 11u, 49u, (41u, 59u)
#define PPUTLIMPL_UDEC_2418u 0x972u, 11u, 49u, (2u, 3u, 13u, 31u)
#define PPUTLIMPL_UDEC_2417u 0x971u, 11u, 49u, ()
#define PPUTLIMPL_UDEC_2416u 0x970u, 11u, 49u, (2u, 2u, 2u, 2u, 151u)
#define PPUTLIMPL_UDEC_2415u 0x96Fu, 11u, 49u, (3u, 5u, 7u, 23u)
#define PPUTLIMPL_UDEC_2414u 0x96Eu, 11u, 49u, (2u, 17u, 71u)
#define PPUTLIMPL_UDEC_2413u 0x96Du, 11u, 49u, (19u, 127u)
#define PPUTLIMPL_UDEC_2412u 0x96Cu, 11u, 49u, (2u, 2u, 3u, 3u, 67u)
#define PPUTLIMPL_UDEC_2411u 0x96Bu, 11u, 49u, ()
#define PPUTLIMPL_UDEC_2410u 0x96Au, 11u, 49u, (2u, 5u, 241u)
#define PPUTLIMPL_UDEC_2409u 0x969u, 11u, 49u, (3u, 11u, 73u)
#define PPUTLIMPL_UDEC_2408u 0x968u, 11u, 49u, (2u, 2u, 2u, 7u, 43u)
#define PPUTLIMPL_UDEC_2407u 0x967u, 11u, 49u, (29u, 83u)
#define PPUTLIMPL_UDEC_2406u 0x966u, 11u, 49u, (2u, 3u, 401u)
#define PPUTLIMPL_UDEC_2405u 0x965u, 11u, 49u, (5u, 13u, 37u)
#define PPUTLIMPL_UDEC_2404u 0x964u, 11u, 49u, (2u, 2u, 601u)
#define PPUTLIMPL_UDEC_2403u 0x963u, 11u, 49u, (3u, 3u, 3u, 89u)
#define PPUTLIMPL_UDEC_2402u 0x962u, 11u, 49u, (2u, 1201u)
#define PPUTLIMPL_UDEC_2401u 0x961u, 11u, 49u, (7u, 7u, 7u, 7u)
#define PPUTLIMPL_UDEC_2400u 0x960u, 11u, 48u, (2u, 2u, 2u, 2u, 2u, 3u, 5u, 5u)
#define PPUTLIMPL_UDEC_2399u 0x95Fu, 11u, 48u, ()
#define PPUTLIMPL_UDEC_2398u 0x95Eu, 11u, 48u, (2u, 11u, 109u)
#define PPUTLIMPL_UDEC_2397u 0x95Du, 11u, 48u, (3u, 17u, 47u)
#define PPUTLIMPL_UDEC_2396u 0x95Cu, 11u, 48u, (2u, 2u, 599u)
#define PPUTLIMPL_UDEC_2395u 0x95Bu, 11u, 48u, (5u, 479u)
#define PPUTLIMPL_UDEC_2394u 0x95Au, 11u, 48u, (2u, 3u, 3u, 7u, 19u)
#define PPUTLIMPL_UDEC_2393u 0x959u, 11u, 48u, ()
#define PPUTLIMPL_UDEC_2392u 0x958u, 11u, 48u, (2u, 2u, 2u, 13u, 23u)
#define PPUTLIMPL_UDEC_2391u 0x957u, 11u, 48u, (3u, 797u)
#define PPUTLIMPL_UDEC_2390u 0x956u, 11u, 48u, (2u, 5u, 239u)
#define PPUTLIMPL_UDEC_2389u 0x955u, 11u, 48u, ()
#define PPUTLIMPL_UDEC_2388u 0x954u, 11u, 48u, (2u, 2u, 3u, 199u)
#define PPUTLIMPL_UDEC_2387u 0x953u, 11u, 48u, (7u, 11u, 31u)
#define PPUTLIMPL_UDEC_2386u 0x952u, 11u, 48u, (2u, 1193u)
#define PPUTLIMPL_UDEC_2385u 0x951u, 11u, 48u, (3u, 3u, 5u, 53u)
#define PPUTLIMPL_UDEC_2384u 0x950u, 11u, 48u, (2u, 2u, 2u, 2u, 149u)
#define PPUTLIMPL_UDEC_2383u 0x94Fu, 11u, 48u, ()
#define PPUTLIMPL_UDEC_2382u 0x94Eu, 11u, 48u, (2u, 3u, 397u)
#define PPUTLIMPL_UDEC_2381u 0x94Du, 11u, 48u, ()
#define PPUTLIMPL_UDEC_2380u 0x94Cu, 11u, 48u, (2u, 2u, 5u, 7u, 17u)
#define PPUTLIMPL_UDEC_2379u 0x94Bu, 11u, 48u, (3u, 13u, 61u)
#define PPUTLIMPL_UDEC_2378u 0x94Au, 11u, 48u, (2u, 29u, 41u)
#define PPUTLIMPL_UDEC_2377u 0x949u, 11u, 48u, ()
#define PPUTLIMPL_UDEC_2376u 0x948u, 11u, 48u, (2u, 2u, 2u, 3u, 3u, 3u, 11u)
#define PPUTLIMPL_UDEC_2375u 0x947u, 11u, 48u, (5u, 5u, 5u, 19u)
#define PPUTLIMPL_UDEC_2374u 0x946u, 11u, 48u, (2u, 1187u)
#define PPUTLIMPL_UDEC_2373u 0x945u, 11u, 48u, (3u, 7u, 113u)
#define PPUTLIMPL_UDEC_2372u 0x944u, 11u, 48u, (2u, 2u, 593u)
#define PPUTLIMPL_UDEC_2371u 0x943u, 11u, 48u, ()
#define PPUTLIMPL_UDEC_2370u 0x942u, 11u, 48u, (2u, 3u, 5u, 79u)
#define PPUTLIMPL_UDEC_2369u 0x941u, 11u, 48u, (23u, 103u)
#define PPUTLIMPL_UDEC_2368u 0x940u, 11u, 48u, (2u, 2u, 2u, 2u, 2u, 2u, 37u)
#define PPUTLIMPL_UDEC_2367u 0x93Fu, 11u, 48u, (3u, 3u, 263u)
#define PPUTLIMPL_UDEC_2366u 0x93Eu, 11u, 48u, (2u, 7u, 13u, 13u)
#define PPUTLIMPL_UDEC_2365u 0x93Du, 11u, 48u, (5u, 11u, 43u)
#define PPUTLIMPL_UDEC_2364u 0x93Cu, 11u, 48u, (2u, 2u, 3u, 197u)
#define PPUTLIMPL_UDEC_2363u 0x93Bu, 11u, 48u, (17u, 139u)
#define PPUTLIMPL_UDEC_2362u 0x93Au, 11u, 48u, (2u, 1181u)
#define PPUTLIMPL_UDEC_2361u 0x939u, 11u, 48u, (3u, 787u)
#define PPUTLIMPL_UDEC_2360u 0x938u, 11u, 48u, (2u, 2u, 2u, 5u, 59u)
#define PPUTLIMPL_UDEC_2359u 0x937u, 11u, 48u, (7u, 337u)
#define PPUTLIMPL_UDEC_2358u 0x936u, 11u, 48u, (2u, 3u, 3u, 131u)
#define PPUTLIMPL_UDEC_2357u 0x935u, 11u, 48u, ()
#define PPUTLIMPL_UDEC_2356u 0x934u, 11u, 48u, (2u, 2u, 19u, 31u)
#define PPUTLIMPL_UDEC_2355u 0x933u, 11u, 48u, (3u, 5u, 157u)
#define PPUTLIMPL_UDEC_2354u 0x932u, 11u, 48u, (2u, 11u, 107u)
#define PPUTLIMPL_UDEC_2353u 0x931u, 11u, 48u, (13u, 181u)
#define PPUTLIMPL_UDEC_2352u 0x930u, 11u, 48u, (2u, 2u, 2u, 2u, 3u, 7u, 7u)
#define PPUTLIMPL_UDEC_2351u 0x92Fu, 11u, 48u, ()
#define PPUTLIMPL_UDEC_2350u 0x92Eu, 11u, 48u, (2u, 5u, 5u, 47u)
#define PPUTLIMPL_UDEC_2349u 0x92Du, 11u, 48u, (3u, 3u, 3u, 3u, 29u)
#define PPUTLIMPL_UDEC_2348u 0x92Cu, 11u, 48u, (2u, 2u, 587u)
#define PPUTLIMPL_UDEC_2347u 0x92Bu, 11u, 48u, ()
#define PPUTLIMPL_UDEC_2346u 0x92Au, 11u, 48u, (2u, 3u, 17u, 23u)
#define PPUTLIMPL_UDEC_2345u 0x929u, 11u, 48u, (5u, 7u, 67u)
#define PPUTLIMPL_UDEC_2344u 0x928u, 11u, 48u, (2u, 2u, 2u, 293u)
#define PPUTLIMPL_UDEC_2343u 0x927u, 11u, 48u, (3u, 11u, 71u)
#define PPUTLIMPL_UDEC_2342u 0x926u, 11u, 48u, (2u, 1171u)
#define PPUTLIMPL_UDEC_2341u 0x925u, 11u, 48u, ()
#define PPUTLIMPL_UDEC_2340u 0x924u, 11u, 48u, (2u, 2u, 3u, 3u, 5u, 13u)
#define PPUTLIMPL_UDEC_2339u 0x923u, 11u, 48u, ()
#define PPUTLIMPL_UDEC_2338u 0x922u, 11u, 48u, (2u, 7u, 167u)
#define PPUTLIMPL_UDEC_2337u 0x921u, 11u, 48u, (3u, 19u, 41u)
#define PPUTLIMPL_UDEC_2336u 0x920u, 11u, 48u, (2u, 2u, 2u, 2u, 2u, 73u)
#define PPUTLIMPL_UDEC_2335u 0x91Fu, 11u, 48u, (5u, 467u)
#define PPUTLIMPL_UDEC_2334u 0x91Eu, 11u, 48u, (2u, 3u, 389u)
#define PPUTLIMPL_UDEC_2333u 0x91Du, 11u, 48u, ()
#define PPUTLIMPL_UDEC_2332u 0x91Cu, 11u, 48u, (2u, 2u, 11u, 53u)
#define PPUTLIMPL_UDEC_2331u 0x91Bu, 11u, 48u, (3u, 3u, 7u, 37u)
#define PPUTLIMPL_UDEC_2330u 0x91Au, 11u, 48u, (2u, 5u, 233u)
#define PPUTLIMPL_UDEC_2329u 0x919u, 11u, 48u, (17u, 137u)
#define PPUTLIMPL_UDEC_2328u 0x918u, 11u, 48u, (2u, 2u, 2u, 3u, 97u)
#define PPUTLIMPL_UDEC_2327u 0x917u, 11u, 48u, (13u, 179u)
#define PPUTLIMPL_UDEC_2326u 0x916u, 11u, 48u, (2u, 1163u)
#define PPUTLIMPL_UDEC_2325u 0x915u, 11u, 48u, (3u, 5u, 5u, 31u)
#define PPUTLIMPL_UDEC_2324u 0x914u, 11u, 48u, (2u, 2u, 7u, 83u)
#define PPUTLIMPL_UDEC_2323u 0x913u, 11u, 48u, (23u, 101u)
#define PPUTLIMPL_UDEC_2322u 0x912u, 11u, 48u, (2u, 3u, 3u, 3u, 43u)
#define PPUTLIMPL_UDEC_2321u 0x911u, 11u, 48u, (11u, 211u)
#define PPUTLIMPL_UDEC_2320u 0x910u, 11u, 48u, (2u, 2u, 2u, 2u, 5u, 29u)
#define PPUTLIMPL_UDEC_2319u 0x90Fu, 11u, 48u, (3u, 773u)
#define PPUTLIMPL_UDEC_2318u 0x90Eu, 11u, 48u, (2u, 19u, 61u)
#define PPUTLIMPL_UDEC_2317u 0x90Du, 11u, 48u, (7u, 331u)
#define PPUTLIMPL_UDEC_2316u 0x90Cu, 11u, 48u, (2u, 2u, 3u, 193u)
#define PPUTLIMPL_UDEC_2315u 0x90Bu, 11u, 48u, (5u, 463u)
#define PPUTLIMPL_UDEC_2314u 0x90Au, 11u, 48u, (2u, 13u, 89u)
#define PPUTLIMPL_UDEC_2313u 0x909u, 11u, 48u, (3u, 3u, 257u)
#define PPUTLIMPL_UDEC_2312u 0x908u, 11u, 48u, (2u, 2u, 2u, 17u, 17u)
#define PPUTLIMPL_UDEC_2311u 0x907u, 11u, 48u, ()
#define PPUTLIMPL_UDEC_2310u 0x906u, 11u, 48u, (2u, 3u, 5u, 7u, 11u)
#define PPUTLIMPL_UDEC_2309u 0x905u, 11u, 48u, ()
#define PPUTLIMPL_UDEC_2308u 0x904u, 11u, 48u, (2u, 2u, 577u)
#define PPUTLIMPL_UDEC_2307u 0x903u, 11u, 48u, (3u, 769u)
#define PPUTLIMPL_UDEC_2306u 0x902u, 11u, 48u, (2u, 1153u)
#define PPUTLIMPL_UDEC_2305u 0x901u, 11u, 48u, (5u, 461u)
#define PPUTLIMPL_UDEC_2304u 0x900u, 11u, 48u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 3u, 3u)
#define PPUTLIMPL_UDEC_2303u 0x8FFu, 11u, 47u, (7u, 7u, 47u)
#define PPUTLIMPL_UDEC_2302u 0x8FEu, 11u, 47u, (2u, 1151u)
#define PPUTLIMPL_UDEC_2301u 0x8FDu, 11u, 47u, (3u, 13u, 59u)
#define PPUTLIMPL_UDEC_2300u 0x8FCu, 11u, 47u, (2u, 2u, 5u, 5u, 23u)
#define PPUTLIMPL_UDEC_2299u 0x8FBu, 11u, 47u, (11u, 11u, 19u)
#define PPUTLIMPL_UDEC_2298u 0x8FAu, 11u, 47u, (2u, 3u, 383u)
#define PPUTLIMPL_UDEC_2297u 0x8F9u, 11u, 47u, ()
#define PPUTLIMPL_UDEC_2296u 0x8F8u, 11u, 47u, (2u, 2u, 2u, 7u, 41u)
#define PPUTLIMPL_UDEC_2295u 0x8F7u, 11u, 47u, (3u, 3u, 3u, 5u, 17u)
#define PPUTLIMPL_UDEC_2294u 0x8F6u, 11u, 47u, (2u, 31u, 37u)
#define PPUTLIMPL_UDEC_2293u 0x8F5u, 11u, 47u, ()
#define PPUTLIMPL_UDEC_2292u 0x8F4u, 11u, 47u, (2u, 2u, 3u, 191u)
#define PPUTLIMPL_UDEC_2291u 0x8F3u, 11u, 47u, (29u, 79u)
#define PPUTLIMPL_UDEC_2290u 0x8F2u, 11u, 47u, (2u, 5u, 229u)
#define PPUTLIMPL_UDEC_2289u 0x8F1u, 11u, 47u, (3u, 7u, 109u)
#define PPUTLIMPL_UDEC_2288u 0x8F0u, 11u, 47u, (2u, 2u, 2u, 2u, 11u, 13u)
#define PPUTLIMPL_UDEC_2287u 0x8EFu, 11u, 47u, ()
#define PPUTLIMPL_UDEC_2286u 0x8EEu, 11u, 47u, (2u, 3u, 3u, 127u)
#define PPUTLIMPL_UDEC_2285u 0x8EDu, 11u, 47u, (5u, 457u)
#define PPUTLIMPL_UDEC_2284u 0x8ECu, 11u, 47u, (2u, 2u, 571u)
#define PPUTLIMPL_UDEC_2283u 0x8EBu, 11u, 47u, (3u, 761u)
#define PPUTLIMPL_UDEC_2282u 0x8EAu, 11u, 47u, (2u, 7u, 163u)
#define PPUTLIMPL_UDEC_2281u 0x8E9u, 11u, 47u, ()
#define PPUTLIMPL_UDEC_2280u 0x8E8u, 11u, 47u, (2u, 2u, 2u, 3u, 5u, 19u)
#define PPUTLIMPL_UDEC_2279u 0x8E7u, 11u, 47u, (43u, 53u)
#define PPUTLIMPL_UDEC_2278u 0x8E6u, 11u, 47u, (2u, 17u, 67u)
#define PPUTLIMPL_UDEC_2277u 0x8E5u, 11u, 47u, (3u, 3u, 11u, 23u)
#define PPUTLIMPL_UDEC_2276u 0x8E4u, 11u, 47u, (2u, 2u, 569u)
#define PPUTLIMPL_UDEC_2275u 0x8E3u, 11u, 47u, (5u, 5u, 7u, 13u)
#define PPUTLIMPL_UDEC_2274u 0x8E2u, 11u, 47u, (2u, 3u, 379u)
#define PPUTLIMPL_UDEC_2273u 0x8E1u, 11u, 47u, ()
#define PPUTLIMPL_UDEC_2272u 0x8E0u, 11u, 47u, (2u, 2u, 2u, 2u, 2u, 71u)
#define PPUTLIMPL_UDEC_2271u 0x8DFu, 11u, 47u, (3u, 757u)
#define PPUTLIMPL_UDEC_2270u 0x8DEu, 11u, 47u, (2u, 5u, 227u)
#define PPUTLIMPL_UDEC_2269u 0x8DDu, 11u, 47u, ()
#define PPUTLIMPL_UDEC_2268u 0x8DCu, 11u, 47u, (2u, 2u, 3u, 3u, 3u, 3u, 7u)
#define PPUTLIMPL_UDEC_2267u 0x8DBu, 11u, 47u, ()
#define PPUTLIMPL_UDEC_2266u 0x8DAu, 11u, 47u, (2u, 11u, 103u)
#define PPUTLIMPL_UDEC_2265u 0x8D9u, 11u, 47u, (3u, 5u, 151u)
#define PPUTLIMPL_UDEC_2264u 0x8D8u, 11u, 47u, (2u, 2u, 2u, 283u)
#define PPUTLIMPL_UDEC_2263u 0x8D7u, 11u, 47u, (31u, 73u)
#define PPUTLIMPL_UDEC_2262u 0x8D6u, 11u, 47u, (2u, 3u, 13u, 29u)
#define PPUTLIMPL_UDEC_2261u 0x8D5u, 11u, 47u, (7u, 17u, 19u)
#define PPUTLIMPL_UDEC_2260u 0x8D4u, 11u, 47u, (2u, 2u, 5u, 113u)
#define PPUTLIMPL_UDEC_2259u 0x8D3u, 11u, 47u, (3u, 3u, 251u)
#define PPUTLIMPL_UDEC_2258u 0x8D2u, 11u, 47u, (2u, 1129u)
#define PPUTLIMPL_UDEC_2257u 0x8D1u, 11u, 47u, (37u, 61u)
#define PPUTLIMPL_UDEC_2256u 0x8D0u, 11u, 47u, (2u, 2u, 2u, 2u, 3u, 47u)
#define PPUTLIMPL_UDEC_2255u 0x8CFu, 11u, 47u, (5u, 11u, 41u)
#define PPUTLIMPL_UDEC_2254u 0x8CEu, 11u, 47u, (2u, 7u, 7u, 23u)
#define PPUTLIMPL_UDEC_2253u 0x8CDu, 11u, 47u, (3u, 751u)
#define PPUTLIMPL_UDEC_2252u 0x8CCu, 11u, 47u, (2u, 2u, 563u)
#define PPUTLIMPL_UDEC_2251u 0x8CBu, 11u, 47u, ()
#define PPUTLIMPL_UDEC_2250u 0x8CAu, 11u, 47u, (2u, 3u, 3u, 5u, 5u, 5u)
#define PPUTLIMPL_UDEC_2249u 0x8C9u, 11u, 47u, (13u, 173u)
#define PPUTLIMPL_UDEC_2248u 0x8C8u, 11u, 47u, (2u, 2u, 2u, 281u)
#define PPUTLIMPL_UDEC_2247u 0x8C7u, 11u, 47u, (3u, 7u, 107u)
#define PPUTLIMPL_UDEC_2246u 0x8C6u, 11u, 47u, (2u, 1123u)
#define PPUTLIMPL_UDEC_2245u 0x8C5u, 11u, 47u, (5u, 449u)
#define PPUTLIMPL_UDEC_2244u 0x8C4u, 11u, 47u, (2u, 2u, 3u, 11u, 17u)
#define PPUTLIMPL_UDEC_2243u 0x8C3u, 11u, 47u, ()
#define PPUTLIMPL_UDEC_2242u 0x8C2u, 11u, 47u, (2u, 19u, 59u)
#define PPUTLIMPL_UDEC_2241u 0x8C1u, 11u, 47u, (3u, 3u, 3u, 83u)
#define PPUTLIMPL_UDEC_2240u 0x8C0u, 11u, 47u, (2u, 2u, 2u, 2u, 2u, 2u, 5u, 7u)
#define PPUTLIMPL_UDEC_2239u 0x8BFu, 11u, 47u, ()
#define PPUTLIMPL_UDEC_2238u 0x8BEu, 11u, 47u, (2u, 3u, 373u)
#define PPUTLIMPL_UDEC_2237u 0x8BDu, 11u, 47u, ()
#define PPUTLIMPL_UDEC_2236u 0x8BCu, 11u, 47u, (2u, 2u, 13u, 43u)
#define PPUTLIMPL_UDEC_2235u 0x8BBu, 11u, 47u, (3u, 5u, 149u)
#define PPUTLIMPL_UDEC_2234u 0x8BAu, 11u, 47u, (2u, 1117u)
#define PPUTLIMPL_UDEC_2233u 0x8B9u, 11u, 47u, (7u, 11u, 29u)
#define PPUTLIMPL_UDEC_2232u 0x8B8u, 11u, 47u, (2u, 2u, 2u, 3u, 3u, 31u)
#define PPUTLIMPL_UDEC_2231u 0x8B7u, 11u, 47u, (23u, 97u)
#define PPUTLIMPL_UDEC_2230u 0x8B6u, 11u, 47u, (2u, 5u, 223u)
#define PPUTLIMPL_UDEC_2229u 0x8B5u, 11u, 47u, (3u, 743u)
#define PPUTLIMPL_UDEC_2228u 0x8B4u, 11u, 47u, (2u, 2u, 557u)
#define PPUTLIMPL_UDEC_2227u 0x8B3u, 11u, 47u, (17u, 131u)
#define PPUTLIMPL_UDEC_2226u 0x8B2u, 11u, 47u, (2u, 3u, 7u, 53u)
#define PPUTLIMPL_UDEC_2225u 0x8B1u, 11u, 47u, (5u, 5u, 89u)
#define PPUTLIMPL_UDEC_2224u 0x8B0u, 11u, 47u, (2u, 2u, 2u, 2u, 139u)
#define PPUTLIMPL_UDEC_2223u 0x8AFu, 11u, 47u, (3u, 3u, 13u, 19u)
#define PPUTLIMPL_UDEC_2222u 0x8AEu, 11u, 47u, (2u, 11u, 101u)
#define PPUTLIMPL_UDEC_2221u 0x8ADu, 11u, 47u, ()
#define PPUTLIMPL_UDEC_2220u 0x8ACu, 11u, 47u, (2u, 2u, 3u, 5u, 37u)
#define PPUTLIMPL_UDEC_2219u 0x8ABu, 11u, 47u, (7u, 317u)
#define PPUTLIMPL_UDEC_2218u 0x8AAu, 11u, 47u, (2u, 1109u)
#define PPUTLIMPL_UDEC_2217u 0x8A9u, 11u, 47u, (3u, 739u)
#define PPUTLIMPL_UDEC_2216u 0x8A8u, 11u, 47u, (2u, 2u, 2u, 277u)
#define PPUTLIMPL_UDEC_2215u 0x8A7u, 11u, 47u, (5u, 443u)
#define PPUTLIMPL_UDEC_2214u 0x8A6u, 11u, 47u, (2u, 3u, 3u, 3u, 41u)
#define PPUTLIMPL_UDEC_2213u 0x8A5u, 11u, 47u, ()
#define PPUTLIMPL_UDEC_2212u 0x8A4u, 11u, 47u, (2u, 2u, 7u, 79u)
#define PPUTLIMPL_UDEC_2211u 0x8A3u, 11u, 47u, (3u, 11u, 67u)
#define PPUTLIMPL_UDEC_2210u 0x8A2u, 11u, 47u, (2u, 5u, 13u, 17u)
#define PPUTLIMPL_UDEC_2209u 0x8A1u, 11u, 47u, (47u, 47u)
#define PPUTLIMPL_UDEC_2208u 0x8A0u, 11u, 46u, (2u, 2u, 2u, 2u, 2u, 3u, 23u)
#define PPUTLIMPL_UDEC_2207u 0x89Fu, 11u, 46u, ()
#define PPUTLIMPL_UDEC_2206u 0x89Eu, 11u, 46u, (2u, 1103u)
#define PPUTLIMPL_UDEC_2205u 0x89Du, 11u, 46u, (3u, 3u, 5u, 7u, 7u)
#define PPUTLIMPL_UDEC_2204u 0x89Cu, 11u, 46u, (2u, 2u, 19u, 29u)
#define PPUTLIMPL_UDEC_2203u 0x89Bu, 11u, 46u, ()
#define PPUTLIMPL_UDEC_2202u 0x89Au, 11u, 46u, (2u, 3u, 367u)
#define PPUTLIMPL_UDEC_2201u 0x899u, 11u, 46u, (31u, 71u)
#define PPUTLIMPL_UDEC_2200u 0x898u, 11u, 46u, (2u, 2u, 2u, 5u, 5u, 11u)
#define PPUTLIMPL_UDEC_2199u 0x897u, 11u, 46u, (3u, 733u)
#define PPUTLIMPL_UDEC_2198u 0x896u, 11u, 46u, (2u, 7u, 157u)
#define PPUTLIMPL_UDEC_2197u 0x895u, 11u, 46u, (13u, 13u, 13u)
#define PPUTLIMPL_UDEC_2196u 0x894u, 11u, 46u, (2u, 2u, 3u, 3u, 61u)
#define PPUTLIMPL_UDEC_2195u 0x893u, 11u, 46u, (5u, 439u)
#define PPUTLIMPL_UDEC_2194u 0x892u, 11u, 46u, (2u, 1097u)
#define PPUTLIMPL_UDEC_2193u 0x891u, 11u, 46u, (3u, 17u, 43u)
#define PPUTLIMPL_UDEC_2192u 0x890u, 11u, 46u, (2u, 2u, 2u, 2u, 137u)
#define PPUTLIMPL_UDEC_2191u 0x88Fu, 11u, 46u, (7u, 313u)
#define PPUTLIMPL_UDEC_2190u 0x88Eu, 11u, 46u, (2u, 3u, 5u, 73u)
#define PPUTLIMPL_UDEC_2189u 0x88Du, 11u, 46u, (11u, 199u)
#define PPUTLIMPL_UDEC_2188u 0x88Cu, 11u, 46u, (2u, 2u, 547u)
#define PPUTLIMPL_UDEC_2187u 0x88Bu, 11u, 46u, (3u, 3u, 3u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UDEC_2186u 0x88Au, 11u, 46u, (2u, 1093u)
#define PPUTLIMPL_UDEC_2185u 0x889u, 11u, 46u, (5u, 19u, 23u)
#define PPUTLIMPL_UDEC_2184u 0x888u, 11u, 46u, (2u, 2u, 2u, 3u, 7u, 13u)
#define PPUTLIMPL_UDEC_2183u 0x887u, 11u, 46u, (37u, 59u)
#define PPUTLIMPL_UDEC_2182u 0x886u, 11u, 46u, (2u, 1091u)
#define PPUTLIMPL_UDEC_2181u 0x885u, 11u, 46u, (3u, 727u)
#define PPUTLIMPL_UDEC_2180u 0x884u, 11u, 46u, (2u, 2u, 5u, 109u)
#define PPUTLIMPL_UDEC_2179u 0x883u, 11u, 46u, ()
#define PPUTLIMPL_UDEC_2178u 0x882u, 11u, 46u, (2u, 3u, 3u, 11u, 11u)
#define PPUTLIMPL_UDEC_2177u 0x881u, 11u, 46u, (7u, 311u)
#define PPUTLIMPL_UDEC_2176u 0x880u, 11u, 46u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 17u)
#define PPUTLIMPL_UDEC_2175u 0x87Fu, 11u, 46u, (3u, 5u, 5u, 29u)
#define PPUTLIMPL_UDEC_2174u 0x87Eu, 11u, 46u, (2u, 1087u)
#define PPUTLIMPL_UDEC_2173u 0x87Du, 11u, 46u, (41u, 53u)
#define PPUTLIMPL_UDEC_2172u 0x87Cu, 11u, 46u, (2u, 2u, 3u, 181u)
#define PPUTLIMPL_UDEC_2171u 0x87Bu, 11u, 46u, (13u, 167u)
#define PPUTLIMPL_UDEC_2170u 0x87Au, 11u, 46u, (2u, 5u, 7u, 31u)
#define PPUTLIMPL_UDEC_2169u 0x879u, 11u, 46u, (3u, 3u, 241u)
#define PPUTLIMPL_UDEC_2168u 0x878u, 11u, 46u, (2u, 2u, 2u, 271u)
#define PPUTLIMPL_UDEC_2167u 0x877u, 11u, 46u, (11u, 197u)
#define PPUTLIMPL_UDEC_2166u 0x876u, 11u, 46u, (2u, 3u, 19u, 19u)
#define PPUTLIMPL_UDEC_2165u 0x875u, 11u, 46u, (5u, 433u)
#define PPUTLIMPL_UDEC_2164u 0x874u, 11u, 46u, (2u, 2u, 541u)
#define PPUTLIMPL_UDEC_2163u 0x873u, 11u, 46u, (3u, 7u, 103u)
#define PPUTLIMPL_UDEC_2162u 0x872u, 11u, 46u, (2u, 23u, 47u)
#define PPUTLIMPL_UDEC_2161u 0x871u, 11u, 46u, ()
#define PPUTLIMPL_UDEC_2160u 0x870u, 11u, 46u, (2u, 2u, 2u, 2u, 3u, 3u, 3u, 5u)
#define PPUTLIMPL_UDEC_2159u 0x86Fu, 11u, 46u, (17u, 127u)
#define PPUTLIMPL_UDEC_2158u 0x86Eu, 11u, 46u, (2u, 13u, 83u)
#define PPUTLIMPL_UDEC_2157u 0x86Du, 11u, 46u, (3u, 719u)
#define PPUTLIMPL_UDEC_2156u 0x86Cu, 11u, 46u, (2u, 2u, 7u, 7u, 11u)
#define PPUTLIMPL_UDEC_2155u 0x86Bu, 11u, 46u, (5u, 431u)
#define PPUTLIMPL_UDEC_2154u 0x86Au, 11u, 46u, (2u, 3u, 359u)
#define PPUTLIMPL_UDEC_2153u 0x869u, 11u, 46u, ()
#define PPUTLIMPL_UDEC_2152u 0x868u, 11u, 46u, (2u, 2u, 2u, 269u)
#define PPUTLIMPL_UDEC_2151u 0x867u, 11u, 46u, (3u, 3u, 239u)
#define PPUTLIMPL_UDEC_2150u 0x866u, 11u, 46u, (2u, 5u, 5u, 43u)
#define PPUTLIMPL_UDEC_2149u 0x865u, 11u, 46u, (7u, 307u)
#define PPUTLIMPL_UDEC_2148u 0x864u, 11u, 46u, (2u, 2u, 3u, 179u)
#define PPUTLIMPL_UDEC_2147u 0x863u, 11u, 46u, (19u, 113u)
#define PPUTLIMPL_UDEC_2146u 0x862u, 11u, 46u, (2u, 29u, 37u)
#define PPUTLIMPL_UDEC_2145u 0x861u, 11u, 46u, (3u, 5u, 11u, 13u)
#define PPUTLIMPL_UDEC_2144u 0x860u, 11u, 46u, (2u, 2u, 2u, 2u, 2u, 67u)
#define PPUTLIMPL_UDEC_2143u 0x85Fu, 11u, 46u, ()
#define PPUTLIMPL_UDEC_2142u 0x85Eu, 11u, 46u, (2u, 3u, 3u, 7u, 17u)
#define PPUTLIMPL_UDEC_2141u 0x85Du, 11u, 46u, ()
#define PPUTLIMPL_UDEC_2140u 0x85Cu, 11u, 46u, (2u, 2u, 5u, 107u)
#define PPUTLIMPL_UDEC_2139u 0x85Bu, 11u, 46u, (3u, 23u, 31u)
#define PPUTLIMPL_UDEC_2138u 0x85Au, 11u, 46u, (2u, 1069u)
#define PPUTLIMPL_UDEC_2137u 0x859u, 11u, 46u, ()
#define PPUTLIMPL_UDEC_2136u 0x858u, 11u, 46u, (2u, 2u, 2u, 3u, 89u)
#define PPUTLIMPL_UDEC_2135u 0x857u, 11u, 46u, (5u, 7u, 61u)
#define PPUTLIMPL_UDEC_2134u 0x856u, 11u, 46u, (2u, 11u, 97u)
#define PPUTLIMPL_UDEC_2133u 0x855u, 11u, 46u, (3u, 3u, 3u, 79u)
#define PPUTLIMPL_UDEC_2132u 0x854u, 11u, 46u, (2u, 2u, 13u, 41u)
#define PPUTLIMPL_UDEC_2131u 0x853u, 11u, 46u, ()
#define PPUTLIMPL_UDEC_2130u 0x852u, 11u, 46u, (2u, 3u, 5u, 71u)
#define PPUTLIMPL_UDEC_2129u 0x851u, 11u, 46u, ()
#define PPUTLIMPL_UDEC_2128u 0x850u, 11u, 46u, (2u, 2u, 2u, 2u, 7u, 19u)
#define PPUTLIMPL_UDEC_2127u 0x84Fu, 11u, 46u, (3u, 709u)
#define PPUTLIMPL_UDEC_2126u 0x84Eu, 11u, 46u, (2u, 1063u)
#define PPUTLIMPL_UDEC_2125u 0x84Du, 11u, 46u, (5u, 5u, 5u, 17u)
#define PPUTLIMPL_UDEC_2124u 0x84Cu, 11u, 46u, (2u, 2u, 3u, 3u, 59u)
#define PPUTLIMPL_UDEC_2123u 0x84Bu, 11u, 46u, (11u, 193u)
#define PPUTLIMPL_UDEC_2122u 0x84Au, 11u, 46u, (2u, 1061u)
#define PPUTLIMPL_UDEC_2121u 0x849u, 11u, 46u, (3u, 7u, 101u)
#define PPUTLIMPL_UDEC_2120u 0x848u, 11u, 46u, (2u, 2u, 2u, 5u, 53u)
#define PPUTLIMPL_UDEC_2119u 0x847u, 11u, 46u, (13u, 163u)
#define PPUTLIMPL_UDEC_2118u 0x846u, 11u, 46u, (2u, 3u, 353u)
#define PPUTLIMPL_UDEC_2117u 0x845u, 11u, 46u, (29u, 73u)
#define PPUTLIMPL_UDEC_2116u 0x844u, 11u, 46u, (2u, 2u, 23u, 23u)
#define PPUTLIMPL_UDEC_2115u 0x843u, 11u, 45u, (3u, 3u, 5u, 47u)
#define PPUTLIMPL_UDEC_2114u 0x842u, 11u, 45u, (2u, 7u, 151u)
#define PPUTLIMPL_UDEC_2113u 0x841u, 11u, 45u, ()
#define PPUTLIMPL_UDEC_2112u 0x840u, 11u, 45u, (2u, 2u, 2u, 2u, 2u, 2u, 3u, 11u)
#define PPUTLIMPL_UDEC_2111u 0x83Fu, 11u, 45u, ()
#define PPUTLIMPL_UDEC_2110u 0x83Eu, 11u, 45u, (2u, 5u, 211u)
#define PPUTLIMPL_UDEC_2109u 0x83Du, 11u, 45u, (3u, 19u, 37u)
#define PPUTLIMPL_UDEC_2108u 0x83Cu, 11u, 45u, (2u, 2u, 17u, 31u)
#define PPUTLIMPL_UDEC_2107u 0x83Bu, 11u, 45u, (7u, 7u, 43u)
#define PPUTLIMPL_UDEC_2106u 0x83Au, 11u, 45u, (2u, 3u, 3u, 3u, 3u, 13u)
#define PPUTLIMPL_UDEC_2105u 0x839u, 11u, 45u, (5u, 421u)
#define PPUTLIMPL_UDEC_2104u 0x838u, 11u, 45u, (2u, 2u, 2u, 263u)
#define PPUTLIMPL_UDEC_2103u 0x837u, 11u, 45u, (3u, 701u)
#define PPUTLIMPL_UDEC_2102u 0x836u, 11u, 45u, (2u, 1051u)
#define PPUTLIMPL_UDEC_2101u 0x835u, 11u, 45u, (11u, 191u)
#define PPUTLIMPL_UDEC_2100u 0x834u, 11u, 45u, (2u, 2u, 3u, 5u, 5u, 7u)
#define PPUTLIMPL_UDEC_2099u 0x833u, 11u, 45u, ()
#define PPUTLIMPL_UDEC_2098u 0x832u, 11u, 45u, (2u, 1049u)
#define PPUTLIMPL_UDEC_2097u 0x831u, 11u, 45u, (3u, 3u, 233u)
#define PPUTLIMPL_UDEC_2096u 0x830u, 11u, 45u, (2u, 2u, 2u, 2u, 131u)
#define PPUTLIMPL_UDEC_2095u 0x82Fu, 11u, 45u, (5u, 419u)
#define PPUTLIMPL_UDEC_2094u 0x82Eu, 11u, 45u, (2u, 3u, 349u)
#define PPUTLIMPL_UDEC_2093u 0x82Du, 11u, 45u, (7u, 13u, 23u)
#define PPUTLIMPL_UDEC_2092u 0x82Cu, 11u, 45u, (2u, 2u, 523u)
#define PPUTLIMPL_UDEC_2091u 0x82Bu, 11u, 45u, (3u, 17u, 41u)
#define PPUTLIMPL_UDEC_2090u 0x82Au, 11u, 45u, (2u, 5u, 11u, 19u)
#define PPUTLIMPL_UDEC_2089u 0x829u, 11u, 45u, ()
#define PPUTLIMPL_UDEC_2088u 0x828u, 11u, 45u, (2u, 2u, 2u, 3u, 3u, 29u)
#define PPUTLIMPL_UDEC_2087u 0x827u, 11u, 45u, ()
#define PPUTLIMPL_UDEC_2086u 0x826u, 11u, 45u, (2u, 7u, 149u)
#define PPUTLIMPL_UDEC_2085u 0x825u, 11u, 45u, (3u, 5u, 139u)
#define PPUTLIMPL_UDEC_2084u 0x824u, 11u, 45u, (2u, 2u, 521u)
#define PPUTLIMPL_UDEC_2083u 0x823u, 11u, 45u, ()
#define PPUTLIMPL_UDEC_2082u 0x822u, 11u, 45u, (2u, 3u, 347u)
#define PPUTLIMPL_UDEC_2081u 0x821u, 11u, 45u, ()
#define PPUTLIMPL_UDEC_2080u 0x820u, 11u, 45u, (2u, 2u, 2u, 2u, 2u, 5u, 13u)
#define PPUTLIMPL_UDEC_2079u 0x81Fu, 11u, 45u, (3u, 3u, 3u, 7u, 11u)
#define PPUTLIMPL_UDEC_2078u 0x81Eu, 11u, 45u, (2u, 1039u)
#define PPUTLIMPL_UDEC_2077u 0x81Du, 11u, 45u, (31u, 67u)
#define PPUTLIMPL_UDEC_2076u 0x81Cu, 11u, 45u, (2u, 2u, 3u, 173u)
#define PPUTLIMPL_UDEC_2075u 0x81Bu, 11u, 45u, (5u, 5u, 83u)
#define PPUTLIMPL_UDEC_2074u 0x81Au, 11u, 45u, (2u, 17u, 61u)
#define PPUTLIMPL_UDEC_2073u 0x819u, 11u, 45u, (3u, 691u)
#define PPUTLIMPL_UDEC_2072u 0x818u, 11u, 45u, (2u, 2u, 2u, 7u, 37u)
#define PPUTLIMPL_UDEC_2071u 0x817u, 11u, 45u, (19u, 109u)
#define PPUTLIMPL_UDEC_2070u 0x816u, 11u, 45u, (2u, 3u, 3u, 5u, 23u)
#define PPUTLIMPL_UDEC_2069u 0x815u, 11u, 45u, ()
#define PPUTLIMPL_UDEC_2068u 0x814u, 11u, 45u, (2u, 2u, 11u, 47u)
#define PPUTLIMPL_UDEC_2067u 0x813u, 11u, 45u, (3u, 13u, 53u)
#define PPUTLIMPL_UDEC_2066u 0x812u, 11u, 45u, (2u, 1033u)
#define PPUTLIMPL_UDEC_2065u 0x811u, 11u, 45u, (5u, 7u, 59u)
#define PPUTLIMPL_UDEC_2064u 0x810u, 11u, 45u, (2u, 2u, 2u, 2u, 3u, 43u)
#define PPUTLIMPL_UDEC_2063u 0x80Fu, 11u, 45u, ()
#define PPUTLIMPL_UDEC_2062u 0x80Eu, 11u, 45u, (2u, 1031u)
#define PPUTLIMPL_UDEC_2061u 0x80Du, 11u, 45u, (3u, 3u, 229u)
#define PPUTLIMPL_UDEC_2060u 0x80Cu, 11u, 45u, (2u, 2u, 5u, 103u)
#define PPUTLIMPL_UDEC_2059u 0x80Bu, 11u, 45u, (29u, 71u)
#define PPUTLIMPL_UDEC_2058u 0x80Au, 11u, 45u, (2u, 3u, 7u, 7u, 7u)
#define PPUTLIMPL_UDEC_2057u 0x809u, 11u, 45u, (11u, 11u, 17u)
#define PPUTLIMPL_UDEC_2056u 0x808u, 11u, 45u, (2u, 2u, 2u, 257u)
#define PPUTLIMPL_UDEC_2055u 0x807u, 11u, 45u, (3u, 5u, 137u)
#define PPUTLIMPL_UDEC_2054u 0x806u, 11u, 45u, (2u, 13u, 79u)
#define PPUTLIMPL_UDEC_2053u 0x805u, 11u, 45u, ()
#define PPUTLIMPL_UDEC_2052u 0x804u, 11u, 45u, (2u, 2u, 3u, 3u, 3u, 19u)
#define PPUTLIMPL_UDEC_2051u 0x803u, 11u, 45u, (7u, 293u)
#define PPUTLIMPL_UDEC_2050u 0x802u, 11u, 45u, (2u, 5u, 5u, 41u)
#define PPUTLIMPL_UDEC_2049u 0x801u, 11u, 45u, (3u, 683u)
#define PPUTLIMPL_UDEC_2048u \
  0x800u, 11u, 45u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u)
#define PPUTLIMPL_UDEC_2047u 0x7FFu, 10u, 45u, (23u, 89u)
#define PPUTLIMPL_UDEC_2046u 0x7FEu, 10u, 45u, (2u, 3u, 11u, 31u)
#define PPUTLIMPL_UDEC_2045u 0x7FDu, 10u, 45u, (5u, 409u)
#define PPUTLIMPL_UDEC_2044u 0x7FCu, 10u, 45u, (2u, 2u, 7u, 73u)
#define PPUTLIMPL_UDEC_2043u 0x7FBu, 10u, 45u, (3u, 3u, 227u)
#define PPUTLIMPL_UDEC_2042u 0x7FAu, 10u, 45u, (2u, 1021u)
#define PPUTLIMPL_UDEC_2041u 0x7F9u, 10u, 45u, (13u, 157u)
#define PPUTLIMPL_UDEC_2040u 0x7F8u, 10u, 45u, (2u, 2u, 2u, 3u, 5u, 17u)
#define PPUTLIMPL_UDEC_2039u 0x7F7u, 10u, 45u, ()
#define PPUTLIMPL_UDEC_2038u 0x7F6u, 10u, 45u, (2u, 1019u)
#define PPUTLIMPL_UDEC_2037u 0x7F5u, 10u, 45u, (3u, 7u, 97u)
#define PPUTLIMPL_UDEC_2036u 0x7F4u, 10u, 45u, (2u, 2u, 509u)
#define PPUTLIMPL_UDEC_2035u 0x7F3u, 10u, 45u, (5u, 11u, 37u)
#define PPUTLIMPL_UDEC_2034u 0x7F2u, 10u, 45u, (2u, 3u, 3u, 113u)
#define PPUTLIMPL_UDEC_2033u 0x7F1u, 10u, 45u, (19u, 107u)
#define PPUTLIMPL_UDEC_2032u 0x7F0u, 10u, 45u, (2u, 2u, 2u, 2u, 127u)
#define PPUTLIMPL_UDEC_2031u 0x7EFu, 10u, 45u, (3u, 677u)
#define PPUTLIMPL_UDEC_2030u 0x7EEu, 10u, 45u, (2u, 5u, 7u, 29u)
#define PPUTLIMPL_UDEC_2029u 0x7EDu, 10u, 45u, ()
#define PPUTLIMPL_UDEC_2028u 0x7ECu, 10u, 45u, (2u, 2u, 3u, 13u, 13u)
#define PPUTLIMPL_UDEC_2027u 0x7EBu, 10u, 45u, ()
#define PPUTLIMPL_UDEC_2026u 0x7EAu, 10u, 45u, (2u, 1013u)
#define PPUTLIMPL_UDEC_2025u 0x7E9u, 10u, 45u, (3u, 3u, 3u, 3u, 5u, 5u)
#define PPUTLIMPL_UDEC_2024u 0x7E8u, 10u, 44u, (2u, 2u, 2u, 11u, 23u)
#define PPUTLIMPL_UDEC_2023u 0x7E7u, 10u, 44u, (7u, 17u, 17u)
#define PPUTLIMPL_UDEC_2022u 0x7E6u, 10u, 44u, (2u, 3u, 337u)
#define PPUTLIMPL_UDEC_2021u 0x7E5u, 10u, 44u, (43u, 47u)
#define PPUTLIMPL_UDEC_2020u 0x7E4u, 10u, 44u, (2u, 2u, 5u, 101u)
#define PPUTLIMPL_UDEC_2019u 0x7E3u, 10u, 44u, (3u, 673u)
#define PPUTLIMPL_UDEC_2018u 0x7E2u, 10u, 44u, (2u, 1009u)
#define PPUTLIMPL_UDEC_2017u 0x7E1u, 10u, 44u, ()
#define PPUTLIMPL_UDEC_2016u 0x7E0u, 10u, 44u, (2u, 2u, 2u, 2u, 2u, 3u, 3u, 7u)
#define PPUTLIMPL_UDEC_2015u 0x7DFu, 10u, 44u, (5u, 13u, 31u)
#define PPUTLIMPL_UDEC_2014u 0x7DEu, 10u, 44u, (2u, 19u, 53u)
#define PPUTLIMPL_UDEC_2013u 0x7DDu, 10u, 44u, (3u, 11u, 61u)
#define PPUTLIMPL_UDEC_2012u 0x7DCu, 10u, 44u, (2u, 2u, 503u)
#define PPUTLIMPL_UDEC_2011u 0x7DBu, 10u, 44u, ()
#define PPUTLIMPL_UDEC_2010u 0x7DAu, 10u, 44u, (2u, 3u, 5u, 67u)
#define PPUTLIMPL_UDEC_2009u 0x7D9u, 10u, 44u, (7u, 7u, 41u)
#define PPUTLIMPL_UDEC_2008u 0x7D8u, 10u, 44u, (2u, 2u, 2u, 251u)
#define PPUTLIMPL_UDEC_2007u 0x7D7u, 10u, 44u, (3u, 3u, 223u)
#define PPUTLIMPL_UDEC_2006u 0x7D6u, 10u, 44u, (2u, 17u, 59u)
#define PPUTLIMPL_UDEC_2005u 0x7D5u, 10u, 44u, (5u, 401u)
#define PPUTLIMPL_UDEC_2004u 0x7D4u, 10u, 44u, (2u, 2u, 3u, 167u)
#define PPUTLIMPL_UDEC_2003u 0x7D3u, 10u, 44u, ()
#define PPUTLIMPL_UDEC_2002u 0x7D2u, 10u, 44u, (2u, 7u, 11u, 13u)
#define PPUTLIMPL_UDEC_2001u 0x7D1u, 10u, 44u, (3u, 23u, 29u)
#define PPUTLIMPL_UDEC_2000u 0x7D0u, 10u, 44u, (2u, 2u, 2u, 2u, 5u, 5u, 5u)
#define PPUTLIMPL_UDEC_1999u 0x7CFu, 10u, 44u, ()
#define PPUTLIMPL_UDEC_1998u 0x7CEu, 10u, 44u, (2u, 3u, 3u, 3u, 37u)
#define PPUTLIMPL_UDEC_1997u 0x7CDu, 10u, 44u, ()
#define PPUTLIMPL_UDEC_1996u 0x7CCu, 10u, 44u, (2u, 2u, 499u)
#define PPUTLIMPL_UDEC_1995u 0x7CBu, 10u, 44u, (3u, 5u, 7u, 19u)
#define PPUTLIMPL_UDEC_1994u 0x7CAu, 10u, 44u, (2u, 997u)
#define PPUTLIMPL_UDEC_1993u 0x7C9u, 10u, 44u, ()
#define PPUTLIMPL_UDEC_1992u 0x7C8u, 10u, 44u, (2u, 2u, 2u, 3u, 83u)
#define PPUTLIMPL_UDEC_1991u 0x7C7u, 10u, 44u, (11u, 181u)
#define PPUTLIMPL_UDEC_1990u 0x7C6u, 10u, 44u, (2u, 5u, 199u)
#define PPUTLIMPL_UDEC_1989u 0x7C5u, 10u, 44u, (3u, 3u, 13u, 17u)
#define PPUTLIMPL_UDEC_1988u 0x7C4u, 10u, 44u, (2u, 2u, 7u, 71u)
#define PPUTLIMPL_UDEC_1987u 0x7C3u, 10u, 44u, ()
#define PPUTLIMPL_UDEC_1986u 0x7C2u, 10u, 44u, (2u, 3u, 331u)
#define PPUTLIMPL_UDEC_1985u 0x7C1u, 10u, 44u, (5u, 397u)
#define PPUTLIMPL_UDEC_1984u 0x7C0u, 10u, 44u, (2u, 2u, 2u, 2u, 2u, 2u, 31u)
#define PPUTLIMPL_UDEC_1983u 0x7BFu, 10u, 44u, (3u, 661u)
#define PPUTLIMPL_UDEC_1982u 0x7BEu, 10u, 44u, (2u, 991u)
#define PPUTLIMPL_UDEC_1981u 0x7BDu, 10u, 44u, (7u, 283u)
#define PPUTLIMPL_UDEC_1980u 0x7BCu, 10u, 44u, (2u, 2u, 3u, 3u, 5u, 11u)
#define PPUTLIMPL_UDEC_1979u 0x7BBu, 10u, 44u, ()
#define PPUTLIMPL_UDEC_1978u 0x7BAu, 10u, 44u, (2u, 23u, 43u)
#define PPUTLIMPL_UDEC_1977u 0x7B9u, 10u, 44u, (3u, 659u)
#define PPUTLIMPL_UDEC_1976u 0x7B8u, 10u, 44u, (2u, 2u, 2u, 13u, 19u)
#define PPUTLIMPL_UDEC_1975u 0x7B7u, 10u, 44u, (5u, 5u, 79u)
#define PPUTLIMPL_UDEC_1974u 0x7B6u, 10u, 44u, (2u, 3u, 7u, 47u)
#define PPUTLIMPL_UDEC_1973u 0x7B5u, 10u, 44u, ()
#define PPUTLIMPL_UDEC_1972u 0x7B4u, 10u, 44u, (2u, 2u, 17u, 29u)
#define PPUTLIMPL_UDEC_1971u 0x7B3u, 10u, 44u, (3u, 3u, 3u, 73u)
#define PPUTLIMPL_UDEC_1970u 0x7B2u, 10u, 44u, (2u, 5u, 197u)
#define PPUTLIMPL_UDEC_1969u 0x7B1u, 10u, 44u, (11u, 179u)
#define PPUTLIMPL_UDEC_1968u 0x7B0u, 10u, 44u, (2u, 2u, 2u, 2u, 3u, 41u)
#define PPUTLIMPL_UDEC_1967u 0x7AFu, 10u, 44u, (7u, 281u)
#define PPUTLIMPL_UDEC_1966u 0x7AEu, 10u, 44u, (2u, 983u)
#define PPUTLIMPL_UDEC_1965u 0x7ADu, 10u, 44u, (3u, 5u, 131u)
#define PPUTLIMPL_UDEC_1964u 0x7ACu, 10u, 44u, (2u, 2u, 491u)
#define PPUTLIMPL_UDEC_1963u 0x7ABu, 10u, 44u, (13u, 151u)
#define PPUTLIMPL_UDEC_1962u 0x7AAu, 10u, 44u, (2u, 3u, 3u, 109u)
#define PPUTLIMPL_UDEC_1961u 0x7A9u, 10u, 44u, (37u, 53u)
#define PPUTLIMPL_UDEC_1960u 0x7A8u, 10u, 44u, (2u, 2u, 2u, 5u, 7u, 7u)
#define PPUTLIMPL_UDEC_1959u 0x7A7u, 10u, 44u, (3u, 653u)
#define PPUTLIMPL_UDEC_1958u 0x7A6u, 10u, 44u, (2u, 11u, 89u)
#define PPUTLIMPL_UDEC_1957u 0x7A5u, 10u, 44u, (19u, 103u)
#define PPUTLIMPL_UDEC_1956u 0x7A4u, 10u, 44u, (2u, 2u, 3u, 163u)
#define PPUTLIMPL_UDEC_1955u 0x7A3u, 10u, 44u, (5u, 17u, 23u)
#define PPUTLIMPL_UDEC_1954u 0x7A2u, 10u, 44u, (2u, 977u)
#define PPUTLIMPL_UDEC_1953u 0x7A1u, 10u, 44u, (3u, 3u, 7u, 31u)
#define PPUTLIMPL_UDEC_1952u 0x7A0u, 10u, 44u, (2u, 2u, 2u, 2u, 2u, 61u)
#define PPUTLIMPL_UDEC_1951u 0x79Fu, 10u, 44u, ()
#define PPUTLIMPL_UDEC_1950u 0x79Eu, 10u, 44u, (2u, 3u, 5u, 5u, 13u)
#define PPUTLIMPL_UDEC_1949u 0x79Du, 10u, 44u, ()
#define PPUTLIMPL_UDEC_1948u 0x79Cu, 10u, 44u, (2u, 2u, 487u)
#define PPUTLIMPL_UDEC_1947u 0x79Bu, 10u, 44u, (3u, 11u, 59u)
#define PPUTLIMPL_UDEC_1946u 0x79Au, 10u, 44u, (2u, 7u, 139u)
#define PPUTLIMPL_UDEC_1945u 0x799u, 10u, 44u, (5u, 389u)
#define PPUTLIMPL_UDEC_1944u 0x798u, 10u, 44u, (2u, 2u, 2u, 3u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UDEC_1943u 0x797u, 10u, 44u, (29u, 67u)
#define PPUTLIMPL_UDEC_1942u 0x796u, 10u, 44u, (2u, 971u)
#define PPUTLIMPL_UDEC_1941u 0x795u, 10u, 44u, (3u, 647u)
#define PPUTLIMPL_UDEC_1940u 0x794u, 10u, 44u, (2u, 2u, 5u, 97u)
#define PPUTLIMPL_UDEC_1939u 0x793u, 10u, 44u, (7u, 277u)
#define PPUTLIMPL_UDEC_1938u 0x792u, 10u, 44u, (2u, 3u, 17u, 19u)
#define PPUTLIMPL_UDEC_1937u 0x791u, 10u, 44u, (13u, 149u)
#define PPUTLIMPL_UDEC_1936u 0x790u, 10u, 44u, (2u, 2u, 2u, 2u, 11u, 11u)
#define PPUTLIMPL_UDEC_1935u 0x78Fu, 10u, 43u, (3u, 3u, 5u, 43u)
#define PPUTLIMPL_UDEC_1934u 0x78Eu, 10u, 43u, (2u, 967u)
#define PPUTLIMPL_UDEC_1933u 0x78Du, 10u, 43u, ()
#define PPUTLIMPL_UDEC_1932u 0x78Cu, 10u, 43u, (2u, 2u, 3u, 7u, 23u)
#define PPUTLIMPL_UDEC_1931u 0x78Bu, 10u, 43u, ()
#define PPUTLIMPL_UDEC_1930u 0x78Au, 10u, 43u, (2u, 5u, 193u)
#define PPUTLIMPL_UDEC_1929u 0x789u, 10u, 43u, (3u, 643u)
#define PPUTLIMPL_UDEC_1928u 0x788u, 10u, 43u, (2u, 2u, 2u, 241u)
#define PPUTLIMPL_UDEC_1927u 0x787u, 10u, 43u, (41u, 47u)
#define PPUTLIMPL_UDEC_1926u 0x786u, 10u, 43u, (2u, 3u, 3u, 107u)
#define PPUTLIMPL_UDEC_1925u 0x785u, 10u, 43u, (5u, 5u, 7u, 11u)
#define PPUTLIMPL_UDEC_1924u 0x784u, 10u, 43u, (2u, 2u, 13u, 37u)
#define PPUTLIMPL_UDEC_1923u 0x783u, 10u, 43u, (3u, 641u)
#define PPUTLIMPL_UDEC_1922u 0x782u, 10u, 43u, (2u, 31u, 31u)
#define PPUTLIMPL_UDEC_1921u 0x781u, 10u, 43u, (17u, 113u)
#define PPUTLIMPL_UDEC_1920u 0x780u, 10u, 43u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 3u, 5u)
#define PPUTLIMPL_UDEC_1919u 0x77Fu, 10u, 43u, (19u, 101u)
#define PPUTLIMPL_UDEC_1918u 0x77Eu, 10u, 43u, (2u, 7u, 137u)
#define PPUTLIMPL_UDEC_1917u 0x77Du, 10u, 43u, (3u, 3u, 3u, 71u)
#define PPUTLIMPL_UDEC_1916u 0x77Cu, 10u, 43u, (2u, 2u, 479u)
#define PPUTLIMPL_UDEC_1915u 0x77Bu, 10u, 43u, (5u, 383u)
#define PPUTLIMPL_UDEC_1914u 0x77Au, 10u, 43u, (2u, 3u, 11u, 29u)
#define PPUTLIMPL_UDEC_1913u 0x779u, 10u, 43u, ()
#define PPUTLIMPL_UDEC_1912u 0x778u, 10u, 43u, (2u, 2u, 2u, 239u)
#define PPUTLIMPL_UDEC_1911u 0x777u, 10u, 43u, (3u, 7u, 7u, 13u)
#define PPUTLIMPL_UDEC_1910u 0x776u, 10u, 43u, (2u, 5u, 191u)
#define PPUTLIMPL_UDEC_1909u 0x775u, 10u, 43u, (23u, 83u)
#define PPUTLIMPL_UDEC_1908u 0x774u, 10u, 43u, (2u, 2u, 3u, 3u, 53u)
#define PPUTLIMPL_UDEC_1907u 0x773u, 10u, 43u, ()
#define PPUTLIMPL_UDEC_1906u 0x772u, 10u, 43u, (2u, 953u)
#define PPUTLIMPL_UDEC_1905u 0x771u, 10u, 43u, (3u, 5u, 127u)
#define PPUTLIMPL_UDEC_1904u 0x770u, 10u, 43u, (2u, 2u, 2u, 2u, 7u, 17u)
#define PPUTLIMPL_UDEC_1903u 0x76Fu, 10u, 43u, (11u, 173u)
#define PPUTLIMPL_UDEC_1902u 0x76Eu, 10u, 43u, (2u, 3u, 317u)
#define PPUTLIMPL_UDEC_1901u 0x76Du, 10u, 43u, ()
#define PPUTLIMPL_UDEC_1900u 0x76Cu, 10u, 43u, (2u, 2u, 5u, 5u, 19u)
#define PPUTLIMPL_UDEC_1899u 0x76Bu, 10u, 43u, (3u, 3u, 211u)
#define PPUTLIMPL_UDEC_1898u 0x76Au, 10u, 43u, (2u, 13u, 73u)
#define PPUTLIMPL_UDEC_1897u 0x769u, 10u, 43u, (7u, 271u)
#define PPUTLIMPL_UDEC_1896u 0x768u, 10u, 43u, (2u, 2u, 2u, 3u, 79u)
#define PPUTLIMPL_UDEC_1895u 0x767u, 10u, 43u, (5u, 379u)
#define PPUTLIMPL_UDEC_1894u 0x766u, 10u, 43u, (2u, 947u)
#define PPUTLIMPL_UDEC_1893u 0x765u, 10u, 43u, (3u, 631u)
#define PPUTLIMPL_UDEC_1892u 0x764u, 10u, 43u, (2u, 2u, 11u, 43u)
#define PPUTLIMPL_UDEC_1891u 0x763u, 10u, 43u, (31u, 61u)
#define PPUTLIMPL_UDEC_1890u 0x762u, 10u, 43u, (2u, 3u, 3u, 3u, 5u, 7u)
#define PPUTLIMPL_UDEC_1889u 0x761u, 10u, 43u, ()
#define PPUTLIMPL_UDEC_1888u 0x760u, 10u, 43u, (2u, 2u, 2u, 2u, 2u, 59u)
#define PPUTLIMPL_UDEC_1887u 0x75Fu, 10u, 43u, (3u, 17u, 37u)
#define PPUTLIMPL_UDEC_1886u 0x75Eu, 10u, 43u, (2u, 23u, 41u)
#define PPUTLIMPL_UDEC_1885u 0x75Du, 10u, 43u, (5u, 13u, 29u)
#define PPUTLIMPL_UDEC_1884u 0x75Cu, 10u, 43u, (2u, 2u, 3u, 157u)
#define PPUTLIMPL_UDEC_1883u 0x75Bu, 10u, 43u, (7u, 269u)
#define PPUTLIMPL_UDEC_1882u 0x75Au, 10u, 43u, (2u, 941u)
#define PPUTLIMPL_UDEC_1881u 0x759u, 10u, 43u, (3u, 3u, 11u, 19u)
#define PPUTLIMPL_UDEC_1880u 0x758u, 10u, 43u, (2u, 2u, 2u, 5u, 47u)
#define PPUTLIMPL_UDEC_1879u 0x757u, 10u, 43u, ()
#define PPUTLIMPL_UDEC_1878u 0x756u, 10u, 43u, (2u, 3u, 313u)
#define PPUTLIMPL_UDEC_1877u 0x755u, 10u, 43u, ()
#define PPUTLIMPL_UDEC_1876u 0x754u, 10u, 43u, (2u, 2u, 7u, 67u)
#define PPUTLIMPL_UDEC_1875u 0x753u, 10u, 43u, (3u, 5u, 5u, 5u, 5u)
#define PPUTLIMPL_UDEC_1874u 0x752u, 10u, 43u, (2u, 937u)
#define PPUTLIMPL_UDEC_1873u 0x751u, 10u, 43u, ()
#define PPUTLIMPL_UDEC_1872u 0x750u, 10u, 43u, (2u, 2u, 2u, 2u, 3u, 3u, 13u)
#define PPUTLIMPL_UDEC_1871u 0x74Fu, 10u, 43u, ()
#define PPUTLIMPL_UDEC_1870u 0x74Eu, 10u, 43u, (2u, 5u, 11u, 17u)
#define PPUTLIMPL_UDEC_1869u 0x74Du, 10u, 43u, (3u, 7u, 89u)
#define PPUTLIMPL_UDEC_1868u 0x74Cu, 10u, 43u, (2u, 2u, 467u)
#define PPUTLIMPL_UDEC_1867u 0x74Bu, 10u, 43u, ()
#define PPUTLIMPL_UDEC_1866u 0x74Au, 10u, 43u, (2u, 3u, 311u)
#define PPUTLIMPL_UDEC_1865u 0x749u, 10u, 43u, (5u, 373u)
#define PPUTLIMPL_UDEC_1864u 0x748u, 10u, 43u, (2u, 2u, 2u, 233u)
#define PPUTLIMPL_UDEC_1863u 0x747u, 10u, 43u, (3u, 3u, 3u, 3u, 23u)
#define PPUTLIMPL_UDEC_1862u 0x746u, 10u, 43u, (2u, 7u, 7u, 19u)
#define PPUTLIMPL_UDEC_1861u 0x745u, 10u, 43u, ()
#define PPUTLIMPL_UDEC_1860u 0x744u, 10u, 43u, (2u, 2u, 3u, 5u, 31u)
#define PPUTLIMPL_UDEC_1859u 0x743u, 10u, 43u, (11u, 13u, 13u)
#define PPUTLIMPL_UDEC_1858u 0x742u, 10u, 43u, (2u, 929u)
#define PPUTLIMPL_UDEC_1857u 0x741u, 10u, 43u, (3u, 619u)
#define PPUTLIMPL_UDEC_1856u 0x740u, 10u, 43u, (2u, 2u, 2u, 2u, 2u, 2u, 29u)
#define PPUTLIMPL_UDEC_1855u 0x73Fu, 10u, 43u, (5u, 7u, 53u)
#define PPUTLIMPL_UDEC_1854u 0x73Eu, 10u, 43u, (2u, 3u, 3u, 103u)
#define PPUTLIMPL_UDEC_1853u 0x73Du, 10u, 43u, (17u, 109u)
#define PPUTLIMPL_UDEC_1852u 0x73Cu, 10u, 43u, (2u, 2u, 463u)
#define PPUTLIMPL_UDEC_1851u 0x73Bu, 10u, 43u, (3u, 617u)
#define PPUTLIMPL_UDEC_1850u 0x73Au, 10u, 43u, (2u, 5u, 5u, 37u)
#define PPUTLIMPL_UDEC_1849u 0x739u, 10u, 43u, (43u, 43u)
#define PPUTLIMPL_UDEC_1848u 0x738u, 10u, 42u, (2u, 2u, 2u, 3u, 7u, 11u)
#define PPUTLIMPL_UDEC_1847u 0x737u, 10u, 42u, ()
#define PPUTLIMPL_UDEC_1846u 0x736u, 10u, 42u, (2u, 13u, 71u)
#define PPUTLIMPL_UDEC_1845u 0x735u, 10u, 42u, (3u, 3u, 5u, 41u)
#define PPUTLIMPL_UDEC_1844u 0x734u, 10u, 42u, (2u, 2u, 461u)
#define PPUTLIMPL_UDEC_1843u 0x733u, 10u, 42u, (19u, 97u)
#define PPUTLIMPL_UDEC_1842u 0x732u, 10u, 42u, (2u, 3u, 307u)
#define PPUTLIMPL_UDEC_1841u 0x731u, 10u, 42u, (7u, 263u)
#define PPUTLIMPL_UDEC_1840u 0x730u, 10u, 42u, (2u, 2u, 2u, 2u, 5u, 23u)
#define PPUTLIMPL_UDEC_1839u 0x72Fu, 10u, 42u, (3u, 613u)
#define PPUTLIMPL_UDEC_1838u 0x72Eu, 10u, 42u, (2u, 919u)
#define PPUTLIMPL_UDEC_1837u 0x72Du, 10u, 42u, (11u, 167u)
#define PPUTLIMPL_UDEC_1836u 0x72Cu, 10u, 42u, (2u, 2u, 3u, 3u, 3u, 17u)
#define PPUTLIMPL_UDEC_1835u 0x72Bu, 10u, 42u, (5u, 367u)
#define PPUTLIMPL_UDEC_1834u 0x72Au, 10u, 42u, (2u, 7u, 131u)
#define PPUTLIMPL_UDEC_1833u 0x729u, 10u, 42u, (3u, 13u, 47u)
#define PPUTLIMPL_UDEC_1832u 0x728u, 10u, 42u, (2u, 2u, 2u, 229u)
#define PPUTLIMPL_UDEC_1831u 0x727u, 10u, 42u, ()
#define PPUTLIMPL_UDEC_1830u 0x726u, 10u, 42u, (2u, 3u, 5u, 61u)
#define PPUTLIMPL_UDEC_1829u 0x725u, 10u, 42u, (31u, 59u)
#define PPUTLIMPL_UDEC_1828u 0x724u, 10u, 42u, (2u, 2u, 457u)
#define PPUTLIMPL_UDEC_1827u 0x723u, 10u, 42u, (3u, 3u, 7u, 29u)
#define PPUTLIMPL_UDEC_1826u 0x722u, 10u, 42u, (2u, 11u, 83u)
#define PPUTLIMPL_UDEC_1825u 0x721u, 10u, 42u, (5u, 5u, 73u)
#define PPUTLIMPL_UDEC_1824u 0x720u, 10u, 42u, (2u, 2u, 2u, 2u, 2u, 3u, 19u)
#define PPUTLIMPL_UDEC_1823u 0x71Fu, 10u, 42u, ()
#define PPUTLIMPL_UDEC_1822u 0x71Eu, 10u, 42u, (2u, 911u)
#define PPUTLIMPL_UDEC_1821u 0x71Du, 10u, 42u, (3u, 607u)
#define PPUTLIMPL_UDEC_1820u 0x71Cu, 10u, 42u, (2u, 2u, 5u, 7u, 13u)
#define PPUTLIMPL_UDEC_1819u 0x71Bu, 10u, 42u, (17u, 107u)
#define PPUTLIMPL_UDEC_1818u 0x71Au, 10u, 42u, (2u, 3u, 3u, 101u)
#define PPUTLIMPL_UDEC_1817u 0x719u, 10u, 42u, (23u, 79u)
#define PPUTLIMPL_UDEC_1816u 0x718u, 10u, 42u, (2u, 2u, 2u, 227u)
#define PPUTLIMPL_UDEC_1815u 0x717u, 10u, 42u, (3u, 5u, 11u, 11u)
#define PPUTLIMPL_UDEC_1814u 0x716u, 10u, 42u, (2u, 907u)
#define PPUTLIMPL_UDEC_1813u 0x715u, 10u, 42u, (7u, 7u, 37u)
#define PPUTLIMPL_UDEC_1812u 0x714u, 10u, 42u, (2u, 2u, 3u, 151u)
#define PPUTLIMPL_UDEC_1811u 0x713u, 10u, 42u, ()
#define PPUTLIMPL_UDEC_1810u 0x712u, 10u, 42u, (2u, 5u, 181u)
#define PPUTLIMPL_UDEC_1809u 0x711u, 10u, 42u, (3u, 3u, 3u, 67u)
#define PPUTLIMPL_UDEC_1808u 0x710u, 10u, 42u, (2u, 2u, 2u, 2u, 113u)
#define PPUTLIMPL_UDEC_1807u 0x70Fu, 10u, 42u, (13u, 139u)
#define PPUTLIMPL_UDEC_1806u 0x70Eu, 10u, 42u, (2u, 3u, 7u, 43u)
#define PPUTLIMPL_UDEC_1805u 0x70Du, 10u, 42u, (5u, 19u, 19u)
#define PPUTLIMPL_UDEC_1804u 0x70Cu, 10u, 42u, (2u, 2u, 11u, 41u)
#define PPUTLIMPL_UDEC_1803u 0x70Bu, 10u, 42u, (3u, 601u)
#define PPUTLIMPL_UDEC_1802u 0x70Au, 10u, 42u, (2u, 17u, 53u)
#define PPUTLIMPL_UDEC_1801u 0x709u, 10u, 42u, ()
#define PPUTLIMPL_UDEC_1800u 0x708u, 10u, 42u, (2u, 2u, 2u, 3u, 3u, 5u, 5u)
#define PPUTLIMPL_UDEC_1799u 0x707u, 10u, 42u, (7u, 257u)
#define PPUTLIMPL_UDEC_1798u 0x706u, 10u, 42u, (2u, 29u, 31u)
#define PPUTLIMPL_UDEC_1797u 0x705u, 10u, 42u, (3u, 599u)
#define PPUTLIMPL_UDEC_1796u 0x704u, 10u, 42u, (2u, 2u, 449u)
#define PPUTLIMPL_UDEC_1795u 0x703u, 10u, 42u, (5u, 359u)
#define PPUTLIMPL_UDEC_1794u 0x702u, 10u, 42u, (2u, 3u, 13u, 23u)
#define PPUTLIMPL_UDEC_1793u 0x701u, 10u, 42u, (11u, 163u)
#define PPUTLIMPL_UDEC_1792u 0x700u, 10u, 42u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 7u)
#define PPUTLIMPL_UDEC_1791u 0x6FFu, 10u, 42u, (3u, 3u, 199u)
#define PPUTLIMPL_UDEC_1790u 0x6FEu, 10u, 42u, (2u, 5u, 179u)
#define PPUTLIMPL_UDEC_1789u 0x6FDu, 10u, 42u, ()
#define PPUTLIMPL_UDEC_1788u 0x6FCu, 10u, 42u, (2u, 2u, 3u, 149u)
#define PPUTLIMPL_UDEC_1787u 0x6FBu, 10u, 42u, ()
#define PPUTLIMPL_UDEC_1786u 0x6FAu, 10u, 42u, (2u, 19u, 47u)
#define PPUTLIMPL_UDEC_1785u 0x6F9u, 10u, 42u, (3u, 5u, 7u, 17u)
#define PPUTLIMPL_UDEC_1784u 0x6F8u, 10u, 42u, (2u, 2u, 2u, 223u)
#define PPUTLIMPL_UDEC_1783u 0x6F7u, 10u, 42u, ()
#define PPUTLIMPL_UDEC_1782u 0x6F6u, 10u, 42u, (2u, 3u, 3u, 3u, 3u, 11u)
#define PPUTLIMPL_UDEC_1781u 0x6F5u, 10u, 42u, (13u, 137u)
#define PPUTLIMPL_UDEC_1780u 0x6F4u, 10u, 42u, (2u, 2u, 5u, 89u)
#define PPUTLIMPL_UDEC_1779u 0x6F3u, 10u, 42u, (3u, 593u)
#define PPUTLIMPL_UDEC_1778u 0x6F2u, 10u, 42u, (2u, 7u, 127u)
#define PPUTLIMPL_UDEC_1777u 0x6F1u, 10u, 42u, ()
#define PPUTLIMPL_UDEC_1776u 0x6F0u, 10u, 42u, (2u, 2u, 2u, 2u, 3u, 37u)
#define PPUTLIMPL_UDEC_1775u 0x6EFu, 10u, 42u, (5u, 5u, 71u)
#define PPUTLIMPL_UDEC_1774u 0x6EEu, 10u, 42u, (2u, 887u)
#define PPUTLIMPL_UDEC_1773u 0x6EDu, 10u, 42u, (3u, 3u, 197u)
#define PPUTLIMPL_UDEC_1772u 0x6ECu, 10u, 42u, (2u, 2u, 443u)
#define PPUTLIMPL_UDEC_1771u 0x6EBu, 10u, 42u, (7u, 11u, 23u)
#define PPUTLIMPL_UDEC_1770u 0x6EAu, 10u, 42u, (2u, 3u, 5u, 59u)
#define PPUTLIMPL_UDEC_1769u 0x6E9u, 10u, 42u, (29u, 61u)
#define PPUTLIMPL_UDEC_1768u 0x6E8u, 10u, 42u, (2u, 2u, 2u, 13u, 17u)
#define PPUTLIMPL_UDEC_1767u 0x6E7u, 10u, 42u, (3u, 19u, 31u)
#define PPUTLIMPL_UDEC_1766u 0x6E6u, 10u, 42u, (2u, 883u)
#define PPUTLIMPL_UDEC_1765u 0x6E5u, 10u, 42u, (5u, 353u)
#define PPUTLIMPL_UDEC_1764u 0x6E4u, 10u, 42u, (2u, 2u, 3u, 3u, 7u, 7u)
#define PPUTLIMPL_UDEC_1763u 0x6E3u, 10u, 41u, (41u, 43u)
#define PPUTLIMPL_UDEC_1762u 0x6E2u, 10u, 41u, (2u, 881u)
#define PPUTLIMPL_UDEC_1761u 0x6E1u, 10u, 41u, (3u, 587u)
#define PPUTLIMPL_UDEC_1760u 0x6E0u, 10u, 41u, (2u, 2u, 2u, 2u, 2u, 5u, 11u)
#define PPUTLIMPL_UDEC_1759u 0x6DFu, 10u, 41u, ()
#define PPUTLIMPL_UDEC_1758u 0x6DEu, 10u, 41u, (2u, 3u, 293u)
#define PPUTLIMPL_UDEC_1757u 0x6DDu, 10u, 41u, (7u, 251u)
#define PPUTLIMPL_UDEC_1756u 0x6DCu, 10u, 41u, (2u, 2u, 439u)
#define PPUTLIMPL_UDEC_1755u 0x6DBu, 10u, 41u, (3u, 3u, 3u, 5u, 13u)
#define PPUTLIMPL_UDEC_1754u 0x6DAu, 10u, 41u, (2u, 877u)
#define PPUTLIMPL_UDEC_1753u 0x6D9u, 10u, 41u, ()
#define PPUTLIMPL_UDEC_1752u 0x6D8u, 10u, 41u, (2u, 2u, 2u, 3u, 73u)
#define PPUTLIMPL_UDEC_1751u 0x6D7u, 10u, 41u, (17u, 103u)
#define PPUTLIMPL_UDEC_1750u 0x6D6u, 10u, 41u, (2u, 5u, 5u, 5u, 7u)
#define PPUTLIMPL_UDEC_1749u 0x6D5u, 10u, 41u, (3u, 11u, 53u)
#define PPUTLIMPL_UDEC_1748u 0x6D4u, 10u, 41u, (2u, 2u, 19u, 23u)
#define PPUTLIMPL_UDEC_1747u 0x6D3u, 10u, 41u, ()
#define PPUTLIMPL_UDEC_1746u 0x6D2u, 10u, 41u, (2u, 3u, 3u, 97u)
#define PPUTLIMPL_UDEC_1745u 0x6D1u, 10u, 41u, (5u, 349u)
#define PPUTLIMPL_UDEC_1744u 0x6D0u, 10u, 41u, (2u, 2u, 2u, 2u, 109u)
#define PPUTLIMPL_UDEC_1743u 0x6CFu, 10u, 41u, (3u, 7u, 83u)
#define PPUTLIMPL_UDEC_1742u 0x6CEu, 10u, 41u, (2u, 13u, 67u)
#define PPUTLIMPL_UDEC_1741u 0x6CDu, 10u, 41u, ()
#define PPUTLIMPL_UDEC_1740u 0x6CCu, 10u, 41u, (2u, 2u, 3u, 5u, 29u)
#define PPUTLIMPL_UDEC_1739u 0x6CBu, 10u, 41u, (37u, 47u)
#define PPUTLIMPL_UDEC_1738u 0x6CAu, 10u, 41u, (2u, 11u, 79u)
#define PPUTLIMPL_UDEC_1737u 0x6C9u, 10u, 41u, (3u, 3u, 193u)
#define PPUTLIMPL_UDEC_1736u 0x6C8u, 10u, 41u, (2u, 2u, 2u, 7u, 31u)
#define PPUTLIMPL_UDEC_1735u 0x6C7u, 10u, 41u, (5u, 347u)
#define PPUTLIMPL_UDEC_1734u 0x6C6u, 10u, 41u, (2u, 3u, 17u, 17u)
#define PPUTLIMPL_UDEC_1733u 0x6C5u, 10u, 41u, ()
#define PPUTLIMPL_UDEC_1732u 0x6C4u, 10u, 41u, (2u, 2u, 433u)
#define PPUTLIMPL_UDEC_1731u 0x6C3u, 10u, 41u, (3u, 577u)
#define PPUTLIMPL_UDEC_1730u 0x6C2u, 10u, 41u, (2u, 5u, 173u)
#define PPUTLIMPL_UDEC_1729u 0x6C1u, 10u, 41u, (7u, 13u, 19u)
#define PPUTLIMPL_UDEC_1728u 0x6C0u, 10u, 41u, (2u, 2u, 2u, 2u, 2u, 2u, 3u, 3u, 3u)
#define PPUTLIMPL_UDEC_1727u 0x6BFu, 10u, 41u, (11u, 157u)
#define PPUTLIMPL_UDEC_1726u 0x6BEu, 10u, 41u, (2u, 863u)
#define PPUTLIMPL_UDEC_1725u 0x6BDu, 10u, 41u, (3u, 5u, 5u, 23u)
#define PPUTLIMPL_UDEC_1724u 0x6BCu, 10u, 41u, (2u, 2u, 431u)
#define PPUTLIMPL_UDEC_1723u 0x6BBu, 10u, 41u, ()
#define PPUTLIMPL_UDEC_1722u 0x6BAu, 10u, 41u, (2u, 3u, 7u, 41u)
#define PPUTLIMPL_UDEC_1721u 0x6B9u, 10u, 41u, ()
#define PPUTLIMPL_UDEC_1720u 0x6B8u, 10u, 41u, (2u, 2u, 2u, 5u, 43u)
#define PPUTLIMPL_UDEC_1719u 0x6B7u, 10u, 41u, (3u, 3u, 191u)
#define PPUTLIMPL_UDEC_1718u 0x6B6u, 10u, 41u, (2u, 859u)
#define PPUTLIMPL_UDEC_1717u 0x6B5u, 10u, 41u, (17u, 101u)
#define PPUTLIMPL_UDEC_1716u 0x6B4u, 10u, 41u, (2u, 2u, 3u, 11u, 13u)
#define PPUTLIMPL_UDEC_1715u 0x6B3u, 10u, 41u, (5u, 7u, 7u, 7u)
#define PPUTLIMPL_UDEC_1714u 0x6B2u, 10u, 41u, (2u, 857u)
#define PPUTLIMPL_UDEC_1713u 0x6B1u, 10u, 41u, (3u, 571u)
#define PPUTLIMPL_UDEC_1712u 0x6B0u, 10u, 41u, (2u, 2u, 2u, 2u, 107u)
#define PPUTLIMPL_UDEC_1711u 0x6AFu, 10u, 41u, (29u, 59u)
#define PPUTLIMPL_UDEC_1710u 0x6AEu, 10u, 41u, (2u, 3u, 3u, 5u, 19u)
#define PPUTLIMPL_UDEC_1709u 0x6ADu, 10u, 41u, ()
#define PPUTLIMPL_UDEC_1708u 0x6ACu, 10u, 41u, (2u, 2u, 7u, 61u)
#define PPUTLIMPL_UDEC_1707u 0x6ABu, 10u, 41u, (3u, 569u)
#define PPUTLIMPL_UDEC_1706u 0x6AAu, 10u, 41u, (2u, 853u)
#define PPUTLIMPL_UDEC_1705u 0x6A9u, 10u, 41u, (5u, 11u, 31u)
#define PPUTLIMPL_UDEC_1704u 0x6A8u, 10u, 41u, (2u, 2u, 2u, 3u, 71u)
#define PPUTLIMPL_UDEC_1703u 0x6A7u, 10u, 41u, (13u, 131u)
#define PPUTLIMPL_UDEC_1702u 0x6A6u, 10u, 41u, (2u, 23u, 37u)
#define PPUTLIMPL_UDEC_1701u 0x6A5u, 10u, 41u, (3u, 3u, 3u, 3u, 3u, 7u)
#define PPUTLIMPL_UDEC_1700u 0x6A4u, 10u, 41u, (2u, 2u, 5u, 5u, 17u)
#define PPUTLIMPL_UDEC_1699u 0x6A3u, 10u, 41u, ()
#define PPUTLIMPL_UDEC_1698u 0x6A2u, 10u, 41u, (2u, 3u, 283u)
#define PPUTLIMPL_UDEC_1697u 0x6A1u, 10u, 41u, ()
#define PPUTLIMPL_UDEC_1696u 0x6A0u, 10u, 41u, (2u, 2u, 2u, 2u, 2u, 53u)
#define PPUTLIMPL_UDEC_1695u 0x69Fu, 10u, 41u, (3u, 5u, 113u)
#define PPUTLIMPL_UDEC_1694u 0x69Eu, 10u, 41u, (2u, 7u, 11u, 11u)
#define PPUTLIMPL_UDEC_1693u 0x69Du, 10u, 41u, ()
#define PPUTLIMPL_UDEC_1692u 0x69Cu, 10u, 41u, (2u, 2u, 3u, 3u, 47u)
#define PPUTLIMPL_UDEC_1691u 0x69Bu, 10u, 41u, (19u, 89u)
#define PPUTLIMPL_UDEC_1690u 0x69Au, 10u, 41u, (2u, 5u, 13u, 13u)
#define PPUTLIMPL_UDEC_1689u 0x699u, 10u, 41u, (3u, 563u)
#define PPUTLIMPL_UDEC_1688u 0x698u, 10u, 41u, (2u, 2u, 2u, 211u)
#define PPUTLIMPL_UDEC_1687u 0x697u, 10u, 41u, (7u, 241u)
#define PPUTLIMPL_UDEC_1686u 0x696u, 10u, 41u, (2u, 3u, 281u)
#define PPUTLIMPL_UDEC_1685u 0x695u, 10u, 41u, (5u, 337u)
#define PPUTLIMPL_UDEC_1684u 0x694u, 10u, 41u, (2u, 2u, 421u)
#define PPUTLIMPL_UDEC_1683u 0x693u, 10u, 41u, (3u, 3u, 11u, 17u)
#define PPUTLIMPL_UDEC_1682u 0x692u, 10u, 41u, (2u, 29u, 29u)
#define PPUTLIMPL_UDEC_1681u 0x691u, 10u, 41u, (41u, 41u)
#define PPUTLIMPL_UDEC_1680u 0x690u, 10u, 40u, (2u, 2u, 2u, 2u, 3u, 5u, 7u)
#define PPUTLIMPL_UDEC_1679u 0x68Fu, 10u, 40u, (23u, 73u)
#define PPUTLIMPL_UDEC_1678u 0x68Eu, 10u, 40u, (2u, 839u)
#define PPUTLIMPL_UDEC_1677u 0x68Du, 10u, 40u, (3u, 13u, 43u)
#define PPUTLIMPL_UDEC_1676u 0x68Cu, 10u, 40u, (2u, 2u, 419u)
#define PPUTLIMPL_UDEC_1675u 0x68Bu, 10u, 40u, (5u, 5u, 67u)
#define PPUTLIMPL_UDEC_1674u 0x68Au, 10u, 40u, (2u, 3u, 3u, 3u, 31u)
#define PPUTLIMPL_UDEC_1673u 0x689u, 10u, 40u, (7u, 239u)
#define PPUTLIMPL_UDEC_1672u 0x688u, 10u, 40u, (2u, 2u, 2u, 11u, 19u)
#define PPUTLIMPL_UDEC_1671u 0x687u, 10u, 40u, (3u, 557u)
#define PPUTLIMPL_UDEC_1670u 0x686u, 10u, 40u, (2u, 5u, 167u)
#define PPUTLIMPL_UDEC_1669u 0x685u, 10u, 40u, ()
#define PPUTLIMPL_UDEC_1668u 0x684u, 10u, 40u, (2u, 2u, 3u, 139u)
#define PPUTLIMPL_UDEC_1667u 0x683u, 10u, 40u, ()
#define PPUTLIMPL_UDEC_1666u 0x682u, 10u, 40u, (2u, 7u, 7u, 17u)
#define PPUTLIMPL_UDEC_1665u 0x681u, 10u, 40u, (3u, 3u, 5u, 37u)
#define PPUTLIMPL_UDEC_1664u 0x680u, 10u, 40u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 13u)
#define PPUTLIMPL_UDEC_1663u 0x67Fu, 10u, 40u, ()
#define PPUTLIMPL_UDEC_1662u 0x67Eu, 10u, 40u, (2u, 3u, 277u)
#define PPUTLIMPL_UDEC_1661u 0x67Du, 10u, 40u, (11u, 151u)
#define PPUTLIMPL_UDEC_1660u 0x67Cu, 10u, 40u, (2u, 2u, 5u, 83u)
#define PPUTLIMPL_UDEC_1659u 0x67Bu, 10u, 40u, (3u, 7u, 79u)
#define PPUTLIMPL_UDEC_1658u 0x67Au, 10u, 40u, (2u, 829u)
#define PPUTLIMPL_UDEC_1657u 0x679u, 10u, 40u, ()
#define PPUTLIMPL_UDEC_1656u 0x678u, 10u, 40u, (2u, 2u, 2u, 3u, 3u, 23u)
#define PPUTLIMPL_UDEC_1655u 0x677u, 10u, 40u, (5u, 331u)
#define PPUTLIMPL_UDEC_1654u 0x676u, 10u, 40u, (2u, 827u)
#define PPUTLIMPL_UDEC_1653u 0x675u, 10u, 40u, (3u, 19u, 29u)
#define PPUTLIMPL_UDEC_1652u 0x674u, 10u, 40u, (2u, 2u, 7u, 59u)
#define PPUTLIMPL_UDEC_1651u 0x673u, 10u, 40u, (13u, 127u)
#define PPUTLIMPL_UDEC_1650u 0x672u, 10u, 40u, (2u, 3u, 5u, 5u, 11u)
#define PPUTLIMPL_UDEC_1649u 0x671u, 10u, 40u, (17u, 97u)
#define PPUTLIMPL_UDEC_1648u 0x670u, 10u, 40u, (2u, 2u, 2u, 2u, 103u)
#define PPUTLIMPL_UDEC_1647u 0x66Fu, 10u, 40u, (3u, 3u, 3u, 61u)
#define PPUTLIMPL_UDEC_1646u 0x66Eu, 10u, 40u, (2u, 823u)
#define PPUTLIMPL_UDEC_1645u 0x66Du, 10u, 40u, (5u, 7u, 47u)
#define PPUTLIMPL_UDEC_1644u 0x66Cu, 10u, 40u, (2u, 2u, 3u, 137u)
#define PPUTLIMPL_UDEC_1643u 0x66Bu, 10u, 40u, (31u, 53u)
#define PPUTLIMPL_UDEC_1642u 0x66Au, 10u, 40u, (2u, 821u)
#define PPUTLIMPL_UDEC_1641u 0x669u, 10u, 40u, (3u, 547u)
#define PPUTLIMPL_UDEC_1640u 0x668u, 10u, 40u, (2u, 2u, 2u, 5u, 41u)
#define PPUTLIMPL_UDEC_1639u 0x667u, 10u, 40u, (11u, 149u)
#define PPUTLIMPL_UDEC_1638u 0x666u, 10u, 40u, (2u, 3u, 3u, 7u, 13u)
#define PPUTLIMPL_UDEC_1637u 0x665u, 10u, 40u, ()
#define PPUTLIMPL_UDEC_1636u 0x664u, 10u, 40u, (2u, 2u, 409u)
#define PPUTLIMPL_UDEC_1635u 0x663u, 10u, 40u, (3u, 5u, 109u)
#define PPUTLIMPL_UDEC_1634u 0x662u, 10u, 40u, (2u, 19u, 43u)
#define PPUTLIMPL_UDEC_1633u 0x661u, 10u, 40u, (23u, 71u)
#define PPUTLIMPL_UDEC_1632u 0x660u, 10u, 40u, (2u, 2u, 2u, 2u, 2u, 3u, 17u)
#define PPUTLIMPL_UDEC_1631u 0x65Fu, 10u, 40u, (7u, 233u)
#define PPUTLIMPL_UDEC_1630u 0x65Eu, 10u, 40u, (2u, 5u, 163u)
#define PPUTLIMPL_UDEC_1629u 0x65Du, 10u, 40u, (3u, 3u, 181u)
#define PPUTLIMPL_UDEC_1628u 0x65Cu, 10u, 40u, (2u, 2u, 11u, 37u)
#define PPUTLIMPL_UDEC_1627u 0x65Bu, 10u, 40u, ()
#define PPUTLIMPL_UDEC_1626u 0x65Au, 10u, 40u, (2u, 3u, 271u)
#define PPUTLIMPL_UDEC_1625u 0x659u, 10u, 40u, (5u, 5u, 5u, 13u)
#define PPUTLIMPL_UDEC_1624u 0x658u, 10u, 40u, (2u, 2u, 2u, 7u, 29u)
#define PPUTLIMPL_UDEC_1623u 0x657u, 10u, 40u, (3u, 541u)
#define PPUTLIMPL_UDEC_1622u 0x656u, 10u, 40u, (2u, 811u)
#define PPUTLIMPL_UDEC_1621u 0x655u, 10u, 40u, ()
#define PPUTLIMPL_UDEC_1620u 0x654u, 10u, 40u, (2u, 2u, 3u, 3u, 3u, 3u, 5u)
#define PPUTLIMPL_UDEC_1619u 0x653u, 10u, 40u, ()
#define PPUTLIMPL_UDEC_1618u 0x652u, 10u, 40u, (2u, 809u)
#define PPUTLIMPL_UDEC_1617u 0x651u, 10u, 40u, (3u, 7u, 7u, 11u)
#define PPUTLIMPL_UDEC_1616u 0x650u, 10u, 40u, (2u, 2u, 2u, 2u, 101u)
#define PPUTLIMPL_UDEC_1615u 0x64Fu, 10u, 40u, (5u, 17u, 19u)
#define PPUTLIMPL_UDEC_1614u 0x64Eu, 10u, 40u, (2u, 3u, 269u)
#define PPUTLIMPL_UDEC_1613u 0x64Du, 10u, 40u, ()
#define PPUTLIMPL_UDEC_1612u 0x64Cu, 10u, 40u, (2u, 2u, 13u, 31u)
#define PPUTLIMPL_UDEC_1611u 0x64Bu, 10u, 40u, (3u, 3u, 179u)
#define PPUTLIMPL_UDEC_1610u 0x64Au, 10u, 40u, (2u, 5u, 7u, 23u)
#define PPUTLIMPL_UDEC_1609u 0x649u, 10u, 40u, ()
#define PPUTLIMPL_UDEC_1608u 0x648u, 10u, 40u, (2u, 2u, 2u, 3u, 67u)
#define PPUTLIMPL_UDEC_1607u 0x647u, 10u, 40u, ()
#define PPUTLIMPL_UDEC_1606u 0x646u, 10u, 40u, (2u, 11u, 73u)
#define PPUTLIMPL_UDEC_1605u 0x645u, 10u, 40u, (3u, 5u, 107u)
#define PPUTLIMPL_UDEC_1604u 0x644u, 10u, 40u, (2u, 2u, 401u)
#define PPUTLIMPL_UDEC_1603u 0x643u, 10u, 40u, (7u, 229u)
#define PPUTLIMPL_UDEC_1602u 0x642u, 10u, 40u, (2u, 3u, 3u, 89u)
#define PPUTLIMPL_UDEC_1601u 0x641u, 10u, 40u, ()
#define PPUTLIMPL_UDEC_1600u 0x640u, 10u, 40u, (2u, 2u, 2u, 2u, 2u, 2u, 5u, 5u)
#define PPUTLIMPL_UDEC_1599u 0x63Fu, 10u, 39u, (3u, 13u, 41u)
#define PPUTLIMPL_UDEC_1598u 0x63Eu, 10u, 39u, (2u, 17u, 47u)
#define PPUTLIMPL_UDEC_1597u 0x63Du, 10u, 39u, ()
#define PPUTLIMPL_UDEC_1596u 0x63Cu, 10u, 39u, (2u, 2u, 3u, 7u, 19u)
#define PPUTLIMPL_UDEC_1595u 0x63Bu, 10u, 39u, (5u, 11u, 29u)
#define PPUTLIMPL_UDEC_1594u 0x63Au, 10u, 39u, (2u, 797u)
#define PPUTLIMPL_UDEC_1593u 0x639u, 10u, 39u, (3u, 3u, 3u, 59u)
#define PPUTLIMPL_UDEC_1592u 0x638u, 10u, 39u, (2u, 2u, 2u, 199u)
#define PPUTLIMPL_UDEC_1591u 0x637u, 10u, 39u, (37u, 43u)
#define PPUTLIMPL_UDEC_1590u 0x636u, 10u, 39u, (2u, 3u, 5u, 53u)
#define PPUTLIMPL_UDEC_1589u 0x635u, 10u, 39u, (7u, 227u)
#define PPUTLIMPL_UDEC_1588u 0x634u, 10u, 39u, (2u, 2u, 397u)
#define PPUTLIMPL_UDEC_1587u 0x633u, 10u, 39u, (3u, 23u, 23u)
#define PPUTLIMPL_UDEC_1586u 0x632u, 10u, 39u, (2u, 13u, 61u)
#define PPUTLIMPL_UDEC_1585u 0x631u, 10u, 39u, (5u, 317u)
#define PPUTLIMPL_UDEC_1584u 0x630u, 10u, 39u, (2u, 2u, 2u, 2u, 3u, 3u, 11u)
#define PPUTLIMPL_UDEC_1583u 0x62Fu, 10u, 39u, ()
#define PPUTLIMPL_UDEC_1582u 0x62Eu, 10u, 39u, (2u, 7u, 113u)
#define PPUTLIMPL_UDEC_1581u 0x62Du, 10u, 39u, (3u, 17u, 31u)
#define PPUTLIMPL_UDEC_1580u 0x62Cu, 10u, 39u, (2u, 2u, 5u, 79u)
#define PPUTLIMPL_UDEC_1579u 0x62Bu, 10u, 39u, ()
#define PPUTLIMPL_UDEC_1578u 0x62Au, 10u, 39u, (2u, 3u, 263u)
#define PPUTLIMPL_UDEC_1577u 0x629u, 10u, 39u, (19u, 83u)
#define PPUTLIMPL_UDEC_1576u 0x628u, 10u, 39u, (2u, 2u, 2u, 197u)
#define PPUTLIMPL_UDEC_1575u 0x627u, 10u, 39u, (3u, 3u, 5u, 5u, 7u)
#define PPUTLIMPL_UDEC_1574u 0x626u, 10u, 39u, (2u, 787u)
#define PPUTLIMPL_UDEC_1573u 0x625u, 10u, 39u, (11u, 11u, 13u)
#define PPUTLIMPL_UDEC_1572u 0x624u, 10u, 39u, (2u, 2u, 3u, 131u)
#define PPUTLIMPL_UDEC_1571u 0x623u, 10u, 39u, ()
#define PPUTLIMPL_UDEC_1570u 0x622u, 10u, 39u, (2u, 5u, 157u)
#define PPUTLIMPL_UDEC_1569u 0x621u, 10u, 39u, (3u, 523u)
#define PPUTLIMPL_UDEC_1568u 0x620u, 10u, 39u, (2u, 2u, 2u, 2u, 2u, 7u, 7u)
#define PPUTLIMPL_UDEC_1567u 0x61Fu, 10u, 39u, ()
#define PPUTLIMPL_UDEC_1566u 0x61Eu, 10u, 39u, (2u, 3u, 3u, 3u, 29u)
#define PPUTLIMPL_UDEC_1565u 0x61Du, 10u, 39u, (5u, 313u)
#define PPUTLIMPL_UDEC_1564u 0x61Cu, 10u, 39u, (2u, 2u, 17u, 23u)
#define PPUTLIMPL_UDEC_1563u 0x61Bu, 10u, 39u, (3u, 521u)
#define PPUTLIMPL_UDEC_1562u 0x61Au, 10u, 39u, (2u, 11u, 71u)
#define PPUTLIMPL_UDEC_1561u 0x619u, 10u, 39u, (7u, 223u)
#define PPUTLIMPL_UDEC_1560u 0x618u, 10u, 39u, (2u, 2u, 2u, 3u, 5u, 13u)
#define PPUTLIMPL_UDEC_1559u 0x617u, 10u, 39u, ()
#define PPUTLIMPL_UDEC_1558u 0x616u, 10u, 39u, (2u, 19u, 41u)
#define PPUTLIMPL_UDEC_1557u 0x615u, 10u, 39u, (3u, 3u, 173u)
#define PPUTLIMPL_UDEC_1556u 0x614u, 10u, 39u, (2u, 2u, 389u)
#define PPUTLIMPL_UDEC_1555u 0x613u, 10u, 39u, (5u, 311u)
#define PPUTLIMPL_UDEC_1554u 0x612u, 10u, 39u, (2u, 3u, 7u, 37u)
#define PPUTLIMPL_UDEC_1553u 0x611u, 10u, 39u, ()
#define PPUTLIMPL_UDEC_1552u 0x610u, 10u, 39u, (2u, 2u, 2u, 2u, 97u)
#define PPUTLIMPL_UDEC_1551u 0x60Fu, 10u, 39u, (3u, 11u, 47u)
#define PPUTLIMPL_UDEC_1550u 0x60Eu, 10u, 39u, (2u, 5u, 5u, 31u)
#define PPUTLIMPL_UDEC_1549u 0x60Du, 10u, 39u, ()
#define PPUTLIMPL_UDEC_1548u 0x60Cu, 10u, 39u, (2u, 2u, 3u, 3u, 43u)
#define PPUTLIMPL_UDEC_1547u 0x60Bu, 10u, 39u, (7u, 13u, 17u)
#define PPUTLIMPL_UDEC_1546u 0x60Au, 10u, 39u, (2u, 773u)
#define PPUTLIMPL_UDEC_1545u 0x609u, 10u, 39u, (3u, 5u, 103u)
#define PPUTLIMPL_UDEC_1544u 0x608u, 10u, 39u, (2u, 2u, 2u, 193u)
#define PPUTLIMPL_UDEC_1543u 0x607u, 10u, 39u, ()
#define PPUTLIMPL_UDEC_1542u 0x606u, 10u, 39u, (2u, 3u, 257u)
#define PPUTLIMPL_UDEC_1541u 0x605u, 10u, 39u, (23u, 67u)
#define PPUTLIMPL_UDEC_1540u 0x604u, 10u, 39u, (2u, 2u, 5u, 7u, 11u)
#define PPUTLIMPL_UDEC_1539u 0x603u, 10u, 39u, (3u, 3u, 3u, 3u, 19u)
#define PPUTLIMPL_UDEC_1538u 0x602u, 10u, 39u, (2u, 769u)
#define PPUTLIMPL_UDEC_1537u 0x601u, 10u, 39u, (29u, 53u)
#define PPUTLIMPL_UDEC_1536u 0x600u, 10u, 39u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 3u)
#define PPUTLIMPL_UDEC_1535u 0x5FFu, 10u, 39u, (5u, 307u)
#define PPUTLIMPL_UDEC_1534u 0x5FEu, 10u, 39u, (2u, 13u, 59u)
#define PPUTLIMPL_UDEC_1533u 0x5FDu, 10u, 39u, (3u, 7u, 73u)
#define PPUTLIMPL_UDEC_1532u 0x5FCu, 10u, 39u, (2u, 2u, 383u)
#define PPUTLIMPL_UDEC_1531u 0x5FBu, 10u, 39u, ()
#define PPUTLIMPL_UDEC_1530u 0x5FAu, 10u, 39u, (2u, 3u, 3u, 5u, 17u)
#define PPUTLIMPL_UDEC_1529u 0x5F9u, 10u, 39u, (11u, 139u)
#define PPUTLIMPL_UDEC_1528u 0x5F8u, 10u, 39u, (2u, 2u, 2u, 191u)
#define PPUTLIMPL_UDEC_1527u 0x5F7u, 10u, 39u, (3u, 509u)
#define PPUTLIMPL_UDEC_1526u 0x5F6u, 10u, 39u, (2u, 7u, 109u)
#define PPUTLIMPL_UDEC_1525u 0x5F5u, 10u, 39u, (5u, 5u, 61u)
#define PPUTLIMPL_UDEC_1524u 0x5F4u, 10u, 39u, (2u, 2u, 3u, 127u)
#define PPUTLIMPL_UDEC_1523u 0x5F3u, 10u, 39u, ()
#define PPUTLIMPL_UDEC_1522u 0x5F2u, 10u, 39u, (2u, 761u)
#define PPUTLIMPL_UDEC_1521u 0x5F1u, 10u, 39u, (3u, 3u, 13u, 13u)
#define PPUTLIMPL_UDEC_1520u 0x5F0u, 10u, 38u, (2u, 2u, 2u, 2u, 5u, 19u)
#define PPUTLIMPL_UDEC_1519u 0x5EFu, 10u, 38u, (7u, 7u, 31u)
#define PPUTLIMPL_UDEC_1518u 0x5EEu, 10u, 38u, (2u, 3u, 11u, 23u)
#define PPUTLIMPL_UDEC_1517u 0x5EDu, 10u, 38u, (37u, 41u)
#define PPUTLIMPL_UDEC_1516u 0x5ECu, 10u, 38u, (2u, 2u, 379u)
#define PPUTLIMPL_UDEC_1515u 0x5EBu, 10u, 38u, (3u, 5u, 101u)
#define PPUTLIMPL_UDEC_1514u 0x5EAu, 10u, 38u, (2u, 757u)
#define PPUTLIMPL_UDEC_1513u 0x5E9u, 10u, 38u, (17u, 89u)
#define PPUTLIMPL_UDEC_1512u 0x5E8u, 10u, 38u, (2u, 2u, 2u, 3u, 3u, 3u, 7u)
#define PPUTLIMPL_UDEC_1511u 0x5E7u, 10u, 38u, ()
#define PPUTLIMPL_UDEC_1510u 0x5E6u, 10u, 38u, (2u, 5u, 151u)
#define PPUTLIMPL_UDEC_1509u 0x5E5u, 10u, 38u, (3u, 503u)
#define PPUTLIMPL_UDEC_1508u 0x5E4u, 10u, 38u, (2u, 2u, 13u, 29u)
#define PPUTLIMPL_UDEC_1507u 0x5E3u, 10u, 38u, (11u, 137u)
#define PPUTLIMPL_UDEC_1506u 0x5E2u, 10u, 38u, (2u, 3u, 251u)
#define PPUTLIMPL_UDEC_1505u 0x5E1u, 10u, 38u, (5u, 7u, 43u)
#define PPUTLIMPL_UDEC_1504u 0x5E0u, 10u, 38u, (2u, 2u, 2u, 2u, 2u, 47u)
#define PPUTLIMPL_UDEC_1503u 0x5DFu, 10u, 38u, (3u, 3u, 167u)
#define PPUTLIMPL_UDEC_1502u 0x5DEu, 10u, 38u, (2u, 751u)
#define PPUTLIMPL_UDEC_1501u 0x5DDu, 10u, 38u, (19u, 79u)
#define PPUTLIMPL_UDEC_1500u 0x5DCu, 10u, 38u, (2u, 2u, 3u, 5u, 5u, 5u)
#define PPUTLIMPL_UDEC_1499u 0x5DBu, 10u, 38u, ()
#define PPUTLIMPL_UDEC_1498u 0x5DAu, 10u, 38u, (2u, 7u, 107u)
#define PPUTLIMPL_UDEC_1497u 0x5D9u, 10u, 38u, (3u, 499u)
#define PPUTLIMPL_UDEC_1496u 0x5D8u, 10u, 38u, (2u, 2u, 2u, 11u, 17u)
#define PPUTLIMPL_UDEC_1495u 0x5D7u, 10u, 38u, (5u, 13u, 23u)
#define PPUTLIMPL_UDEC_1494u 0x5D6u, 10u, 38u, (2u, 3u, 3u, 83u)
#define PPUTLIMPL_UDEC_1493u 0x5D5u, 10u, 38u, ()
#define PPUTLIMPL_UDEC_1492u 0x5D4u, 10u, 38u, (2u, 2u, 373u)
#define PPUTLIMPL_UDEC_1491u 0x5D3u, 10u, 38u, (3u, 7u, 71u)
#define PPUTLIMPL_UDEC_1490u 0x5D2u, 10u, 38u, (2u, 5u, 149u)
#define PPUTLIMPL_UDEC_1489u 0x5D1u, 10u, 38u, ()
#define PPUTLIMPL_UDEC_1488u 0x5D0u, 10u, 38u, (2u, 2u, 2u, 2u, 3u, 31u)
#define PPUTLIMPL_UDEC_1487u 0x5CFu, 10u, 38u, ()
#define PPUTLIMPL_UDEC_1486u 0x5CEu, 10u, 38u, (2u, 743u)
#define PPUTLIMPL_UDEC_1485u 0x5CDu, 10u, 38u, (3u, 3u, 3u, 5u, 11u)
#define PPUTLIMPL_UDEC_1484u 0x5CCu, 10u, 38u, (2u, 2u, 7u, 53u)
#define PPUTLIMPL_UDEC_1483u 0x5CBu, 10u, 38u, ()
#define PPUTLIMPL_UDEC_1482u 0x5CAu, 10u, 38u, (2u, 3u, 13u, 19u)
#define PPUTLIMPL_UDEC_1481u 0x5C9u, 10u, 38u, ()
#define PPUTLIMPL_UDEC_1480u 0x5C8u, 10u, 38u, (2u, 2u, 2u, 5u, 37u)
#define PPUTLIMPL_UDEC_1479u 0x5C7u, 10u, 38u, (3u, 17u, 29u)
#define PPUTLIMPL_UDEC_1478u 0x5C6u, 10u, 38u, (2u, 739u)
#define PPUTLIMPL_UDEC_1477u 0x5C5u, 10u, 38u, (7u, 211u)
#define PPUTLIMPL_UDEC_1476u 0x5C4u, 10u, 38u, (2u, 2u, 3u, 3u, 41u)
#define PPUTLIMPL_UDEC_1475u 0x5C3u, 10u, 38u, (5u, 5u, 59u)
#define PPUTLIMPL_UDEC_1474u 0x5C2u, 10u, 38u, (2u, 11u, 67u)
#define PPUTLIMPL_UDEC_1473u 0x5C1u, 10u, 38u, (3u, 491u)
#define PPUTLIMPL_UDEC_1472u 0x5C0u, 10u, 38u, (2u, 2u, 2u, 2u, 2u, 2u, 23u)
#define PPUTLIMPL_UDEC_1471u 0x5BFu, 10u, 38u, ()
#define PPUTLIMPL_UDEC_1470u 0x5BEu, 10u, 38u, (2u, 3u, 5u, 7u, 7u)
#define PPUTLIMPL_UDEC_1469u 0x5BDu, 10u, 38u, (13u, 113u)
#define PPUTLIMPL_UDEC_1468u 0x5BCu, 10u, 38u, (2u, 2u, 367u)
#define PPUTLIMPL_UDEC_1467u 0x5BBu, 10u, 38u, (3u, 3u, 163u)
#define PPUTLIMPL_UDEC_1466u 0x5BAu, 10u, 38u, (2u, 733u)
#define PPUTLIMPL_UDEC_1465u 0x5B9u, 10u, 38u, (5u, 293u)
#define PPUTLIMPL_UDEC_1464u 0x5B8u, 10u, 38u, (2u, 2u, 2u, 3u, 61u)
#define PPUTLIMPL_UDEC_1463u 0x5B7u, 10u, 38u, (7u, 11u, 19u)
#define PPUTLIMPL_UDEC_1462u 0x5B6u, 10u, 38u, (2u, 17u, 43u)
#define PPUTLIMPL_UDEC_1461u 0x5B5u, 10u, 38u, (3u, 487u)
#define PPUTLIMPL_UDEC_1460u 0x5B4u, 10u, 38u, (2u, 2u, 5u, 73u)
#define PPUTLIMPL_UDEC_1459u 0x5B3u, 10u, 38u, ()
#define PPUTLIMPL_UDEC_1458u 0x5B2u, 10u, 38u, (2u, 3u, 3u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UDEC_1457u 0x5B1u, 10u, 38u, (31u, 47u)
#define PPUTLIMPL_UDEC_1456u 0x5B0u, 10u, 38u, (2u, 2u, 2u, 2u, 7u, 13u)
#define PPUTLIMPL_UDEC_1455u 0x5AFu, 10u, 38u, (3u, 5u, 97u)
#define PPUTLIMPL_UDEC_1454u 0x5AEu, 10u, 38u, (2u, 727u)
#define PPUTLIMPL_UDEC_1453u 0x5ADu, 10u, 38u, ()
#define PPUTLIMPL_UDEC_1452u 0x5ACu, 10u, 38u, (2u, 2u, 3u, 11u, 11u)
#define PPUTLIMPL_UDEC_1451u 0x5ABu, 10u, 38u, ()
#define PPUTLIMPL_UDEC_1450u 0x5AAu, 10u, 38u, (2u, 5u, 5u, 29u)
#define PPUTLIMPL_UDEC_1449u 0x5A9u, 10u, 38u, (3u, 3u, 7u, 23u)
#define PPUTLIMPL_UDEC_1448u 0x5A8u, 10u, 38u, (2u, 2u, 2u, 181u)
#define PPUTLIMPL_UDEC_1447u 0x5A7u, 10u, 38u, ()
#define PPUTLIMPL_UDEC_1446u 0x5A6u, 10u, 38u, (2u, 3u, 241u)
#define PPUTLIMPL_UDEC_1445u 0x5A5u, 10u, 38u, (5u, 17u, 17u)
#define PPUTLIMPL_UDEC_1444u 0x5A4u, 10u, 38u, (2u, 2u, 19u, 19u)
#define PPUTLIMPL_UDEC_1443u 0x5A3u, 10u, 37u, (3u, 13u, 37u)
#define PPUTLIMPL_UDEC_1442u 0x5A2u, 10u, 37u, (2u, 7u, 103u)
#define PPUTLIMPL_UDEC_1441u 0x5A1u, 10u, 37u, (11u, 131u)
#define PPUTLIMPL_UDEC_1440u 0x5A0u, 10u, 37u, (2u, 2u, 2u, 2u, 2u, 3u, 3u, 5u)
#define PPUTLIMPL_UDEC_1439u 0x59Fu, 10u, 37u, ()
#define PPUTLIMPL_UDEC_1438u 0x59Eu, 10u, 37u, (2u, 719u)
#define PPUTLIMPL_UDEC_1437u 0x59Du, 10u, 37u, (3u, 479u)
#define PPUTLIMPL_UDEC_1436u 0x59Cu, 10u, 37u, (2u, 2u, 359u)
#define PPUTLIMPL_UDEC_1435u 0x59Bu, 10u, 37u, (5u, 7u, 41u)
#define PPUTLIMPL_UDEC_1434u 0x59Au, 10u, 37u, (2u, 3u, 239u)
#define PPUTLIMPL_UDEC_1433u 0x599u, 10u, 37u, ()
#define PPUTLIMPL_UDEC_1432u 0x598u, 10u, 37u, (2u, 2u, 2u, 179u)
#define PPUTLIMPL_UDEC_1431u 0x597u, 10u, 37u, (3u, 3u, 3u, 53u)
#define PPUTLIMPL_UDEC_1430u 0x596u, 10u, 37u, (2u, 5u, 11u, 13u)
#define PPUTLIMPL_UDEC_1429u 0x595u, 10u, 37u, ()
#define PPUTLIMPL_UDEC_1428u 0x594u, 10u, 37u, (2u, 2u, 3u, 7u, 17u)
#define PPUTLIMPL_UDEC_1427u 0x593u, 10u, 37u, ()
#define PPUTLIMPL_UDEC_1426u 0x592u, 10u, 37u, (2u, 23u, 31u)
#define PPUTLIMPL_UDEC_1425u 0x591u, 10u, 37u, (3u, 5u, 5u, 19u)
#define PPUTLIMPL_UDEC_1424u 0x590u, 10u, 37u, (2u, 2u, 2u, 2u, 89u)
#define PPUTLIMPL_UDEC_1423u 0x58Fu, 10u, 37u, ()
#define PPUTLIMPL_UDEC_1422u 0x58Eu, 10u, 37u, (2u, 3u, 3u, 79u)
#define PPUTLIMPL_UDEC_1421u 0x58Du, 10u, 37u, (7u, 7u, 29u)
#define PPUTLIMPL_UDEC_1420u 0x58Cu, 10u, 37u, (2u, 2u, 5u, 71u)
#define PPUTLIMPL_UDEC_1419u 0x58Bu, 10u, 37u, (3u, 11u, 43u)
#define PPUTLIMPL_UDEC_1418u 0x58Au, 10u, 37u, (2u, 709u)
#define PPUTLIMPL_UDEC_1417u 0x589u, 10u, 37u, (13u, 109u)
#define PPUTLIMPL_UDEC_1416u 0x588u, 10u, 37u, (2u, 2u, 2u, 3u, 59u)
#define PPUTLIMPL_UDEC_1415u 0x587u, 10u, 37u, (5u, 283u)
#define PPUTLIMPL_UDEC_1414u 0x586u, 10u, 37u, (2u, 7u, 101u)
#define PPUTLIMPL_UDEC_1413u 0x585u, 10u, 37u, (3u, 3u, 157u)
#define PPUTLIMPL_UDEC_1412u 0x584u, 10u, 37u, (2u, 2u, 353u)
#define PPUTLIMPL_UDEC_1411u 0x583u, 10u, 37u, (17u, 83u)
#define PPUTLIMPL_UDEC_1410u 0x582u, 10u, 37u, (2u, 3u, 5u, 47u)
#define PPUTLIMPL_UDEC_1409u 0x581u, 10u, 37u, ()
#define PPUTLIMPL_UDEC_1408u 0x580u, 10u, 37u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 11u)
#define PPUTLIMPL_UDEC_1407u 0x57Fu, 10u, 37u, (3u, 7u, 67u)
#define PPUTLIMPL_UDEC_1406u 0x57Eu, 10u, 37u, (2u, 19u, 37u)
#define PPUTLIMPL_UDEC_1405u 0x57Du, 10u, 37u, (5u, 281u)
#define PPUTLIMPL_UDEC_1404u 0x57Cu, 10u, 37u, (2u, 2u, 3u, 3u, 3u, 13u)
#define PPUTLIMPL_UDEC_1403u 0x57Bu, 10u, 37u, (23u, 61u)
#define PPUTLIMPL_UDEC_1402u 0x57Au, 10u, 37u, (2u, 701u)
#define PPUTLIMPL_UDEC_1401u 0x579u, 10u, 37u, (3u, 467u)
#define PPUTLIMPL_UDEC_1400u 0x578u, 10u, 37u, (2u, 2u, 2u, 5u, 5u, 7u)
#define PPUTLIMPL_UDEC_1399u 0x577u, 10u, 37u, ()
#define PPUTLIMPL_UDEC_1398u 0x576u, 10u, 37u, (2u, 3u, 233u)
#define PPUTLIMPL_UDEC_1397u 0x575u, 10u, 37u, (11u, 127u)
#define PPUTLIMPL_UDEC_1396u 0x574u, 10u, 37u, (2u, 2u, 349u)
#define PPUTLIMPL_UDEC_1395u 0x573u, 10u, 37u, (3u, 3u, 5u, 31u)
#define PPUTLIMPL_UDEC_1394u 0x572u, 10u, 37u, (2u, 17u, 41u)
#define PPUTLIMPL_UDEC_1393u 0x571u, 10u, 37u, (7u, 199u)
#define PPUTLIMPL_UDEC_1392u 0x570u, 10u, 37u, (2u, 2u, 2u, 2u, 3u, 29u)
#define PPUTLIMPL_UDEC_1391u 0x56Fu, 10u, 37u, (13u, 107u)
#define PPUTLIMPL_UDEC_1390u 0x56Eu, 10u, 37u, (2u, 5u, 139u)
#define PPUTLIMPL_UDEC_1389u 0x56Du, 10u, 37u, (3u, 463u)
#define PPUTLIMPL_UDEC_1388u 0x56Cu, 10u, 37u, (2u, 2u, 347u)
#define PPUTLIMPL_UDEC_1387u 0x56Bu, 10u, 37u, (19u, 73u)
#define PPUTLIMPL_UDEC_1386u 0x56Au, 10u, 37u, (2u, 3u, 3u, 7u, 11u)
#define PPUTLIMPL_UDEC_1385u 0x569u, 10u, 37u, (5u, 277u)
#define PPUTLIMPL_UDEC_1384u 0x568u, 10u, 37u, (2u, 2u, 2u, 173u)
#define PPUTLIMPL_UDEC_1383u 0x567u, 10u, 37u, (3u, 461u)
#define PPUTLIMPL_UDEC_1382u 0x566u, 10u, 37u, (2u, 691u)
#define PPUTLIMPL_UDEC_1381u 0x565u, 10u, 37u, ()
#define PPUTLIMPL_UDEC_1380u 0x564u, 10u, 37u, (2u, 2u, 3u, 5u, 23u)
#define PPUTLIMPL_UDEC_1379u 0x563u, 10u, 37u, (7u, 197u)
#define PPUTLIMPL_UDEC_1378u 0x562u, 10u, 37u, (2u, 13u, 53u)
#define PPUTLIMPL_UDEC_1377u 0x561u, 10u, 37u, (3u, 3u, 3u, 3u, 17u)
#define PPUTLIMPL_UDEC_1376u 0x560u, 10u, 37u, (2u, 2u, 2u, 2u, 2u, 43u)
#define PPUTLIMPL_UDEC_1375u 0x55Fu, 10u, 37u, (5u, 5u, 5u, 11u)
#define PPUTLIMPL_UDEC_1374u 0x55Eu, 10u, 37u, (2u, 3u, 229u)
#define PPUTLIMPL_UDEC_1373u 0x55Du, 10u, 37u, ()
#define PPUTLIMPL_UDEC_1372u 0x55Cu, 10u, 37u, (2u, 2u, 7u, 7u, 7u)
#define PPUTLIMPL_UDEC_1371u 0x55Bu, 10u, 37u, (3u, 457u)
#define PPUTLIMPL_UDEC_1370u 0x55Au, 10u, 37u, (2u, 5u, 137u)
#define PPUTLIMPL_UDEC_1369u 0x559u, 10u, 37u, (37u, 37u)
#define PPUTLIMPL_UDEC_1368u 0x558u, 10u, 36u, (2u, 2u, 2u, 3u, 3u, 19u)
#define PPUTLIMPL_UDEC_1367u 0x557u, 10u, 36u, ()
#define PPUTLIMPL_UDEC_1366u 0x556u, 10u, 36u, (2u, 683u)
#define PPUTLIMPL_UDEC_1365u 0x555u, 10u, 36u, (3u, 5u, 7u, 13u)
#define PPUTLIMPL_UDEC_1364u 0x554u, 10u, 36u, (2u, 2u, 11u, 31u)
#define PPUTLIMPL_UDEC_1363u 0x553u, 10u, 36u, (29u, 47u)
#define PPUTLIMPL_UDEC_1362u 0x552u, 10u, 36u, (2u, 3u, 227u)
#define PPUTLIMPL_UDEC_1361u 0x551u, 10u, 36u, ()
#define PPUTLIMPL_UDEC_1360u 0x550u, 10u, 36u, (2u, 2u, 2u, 2u, 5u, 17u)
#define PPUTLIMPL_UDEC_1359u 0x54Fu, 10u, 36u, (3u, 3u, 151u)
#define PPUTLIMPL_UDEC_1358u 0x54Eu, 10u, 36u, (2u, 7u, 97u)
#define PPUTLIMPL_UDEC_1357u 0x54Du, 10u, 36u, (23u, 59u)
#define PPUTLIMPL_UDEC_1356u 0x54Cu, 10u, 36u, (2u, 2u, 3u, 113u)
#define PPUTLIMPL_UDEC_1355u 0x54Bu, 10u, 36u, (5u, 271u)
#define PPUTLIMPL_UDEC_1354u 0x54Au, 10u, 36u, (2u, 677u)
#define PPUTLIMPL_UDEC_1353u 0x549u, 10u, 36u, (3u, 11u, 41u)
#define PPUTLIMPL_UDEC_1352u 0x548u, 10u, 36u, (2u, 2u, 2u, 13u, 13u)
#define PPUTLIMPL_UDEC_1351u 0x547u, 10u, 36u, (7u, 193u)
#define PPUTLIMPL_UDEC_1350u 0x546u, 10u, 36u, (2u, 3u, 3u, 3u, 5u, 5u)
#define PPUTLIMPL_UDEC_1349u 0x545u, 10u, 36u, (19u, 71u)
#define PPUTLIMPL_UDEC_1348u 0x544u, 10u, 36u, (2u, 2u, 337u)
#define PPUTLIMPL_UDEC_1347u 0x543u, 10u, 36u, (3u, 449u)
#define PPUTLIMPL_UDEC_1346u 0x542u, 10u, 36u, (2u, 673u)
#define PPUTLIMPL_UDEC_1345u 0x541u, 10u, 36u, (5u, 269u)
#define PPUTLIMPL_UDEC_1344u 0x540u, 10u, 36u, (2u, 2u, 2u, 2u, 2u, 2u, 3u, 7u)
#define PPUTLIMPL_UDEC_1343u 0x53Fu, 10u, 36u, (17u, 79u)
#define PPUTLIMPL_UDEC_1342u 0x53Eu, 10u, 36u, (2u, 11u, 61u)
#define PPUTLIMPL_UDEC_1341u 0x53Du, 10u, 36u, (3u, 3u, 149u)
#define PPUTLIMPL_UDEC_1340u 0x53Cu, 10u, 36u, (2u, 2u, 5u, 67u)
#define PPUTLIMPL_UDEC_1339u 0x53Bu, 10u, 36u, (13u, 103u)
#define PPUTLIMPL_UDEC_1338u 0x53Au, 10u, 36u, (2u, 3u, 223u)
#define PPUTLIMPL_UDEC_1337u 0x539u, 10u, 36u, (7u, 191u)
#define PPUTLIMPL_UDEC_1336u 0x538u, 10u, 36u, (2u, 2u, 2u, 167u)
#define PPUTLIMPL_UDEC_1335u 0x537u, 10u, 36u, (3u, 5u, 89u)
#define PPUTLIMPL_UDEC_1334u 0x536u, 10u, 36u, (2u, 23u, 29u)
#define PPUTLIMPL_UDEC_1333u 0x535u, 10u, 36u, (31u, 43u)
#define PPUTLIMPL_UDEC_1332u 0x534u, 10u, 36u, (2u, 2u, 3u, 3u, 37u)
#define PPUTLIMPL_UDEC_1331u 0x533u, 10u, 36u, (11u, 11u, 11u)
#define PPUTLIMPL_UDEC_1330u 0x532u, 10u, 36u, (2u, 5u, 7u, 19u)
#define PPUTLIMPL_UDEC_1329u 0x531u, 10u, 36u, (3u, 443u)
#define PPUTLIMPL_UDEC_1328u 0x530u, 10u, 36u, (2u, 2u, 2u, 2u, 83u)
#define PPUTLIMPL_UDEC_1327u 0x52Fu, 10u, 36u, ()
#define PPUTLIMPL_UDEC_1326u 0x52Eu, 10u, 36u, (2u, 3u, 13u, 17u)
#define PPUTLIMPL_UDEC_1325u 0x52Du, 10u, 36u, (5u, 5u, 53u)
#define PPUTLIMPL_UDEC_1324u 0x52Cu, 10u, 36u, (2u, 2u, 331u)
#define PPUTLIMPL_UDEC_1323u 0x52Bu, 10u, 36u, (3u, 3u, 3u, 7u, 7u)
#define PPUTLIMPL_UDEC_1322u 0x52Au, 10u, 36u, (2u, 661u)
#define PPUTLIMPL_UDEC_1321u 0x529u, 10u, 36u, ()
#define PPUTLIMPL_UDEC_1320u 0x528u, 10u, 36u, (2u, 2u, 2u, 3u, 5u, 11u)
#define PPUTLIMPL_UDEC_1319u 0x527u, 10u, 36u, ()
#define PPUTLIMPL_UDEC_1318u 0x526u, 10u, 36u, (2u, 659u)
#define PPUTLIMPL_UDEC_1317u 0x525u, 10u, 36u, (3u, 439u)
#define PPUTLIMPL_UDEC_1316u 0x524u, 10u, 36u, (2u, 2u, 7u, 47u)
#define PPUTLIMPL_UDEC_1315u 0x523u, 10u, 36u, (5u, 263u)
#define PPUTLIMPL_UDEC_1314u 0x522u, 10u, 36u, (2u, 3u, 3u, 73u)
#define PPUTLIMPL_UDEC_1313u 0x521u, 10u, 36u, (13u, 101u)
#define PPUTLIMPL_UDEC_1312u 0x520u, 10u, 36u, (2u, 2u, 2u, 2u, 2u, 41u)
#define PPUTLIMPL_UDEC_1311u 0x51Fu, 10u, 36u, (3u, 19u, 23u)
#define PPUTLIMPL_UDEC_1310u 0x51Eu, 10u, 36u, (2u, 5u, 131u)
#define PPUTLIMPL_UDEC_1309u 0x51Du, 10u, 36u, (7u, 11u, 17u)
#define PPUTLIMPL_UDEC_1308u 0x51Cu, 10u, 36u, (2u, 2u, 3u, 109u)
#define PPUTLIMPL_UDEC_1307u 0x51Bu, 10u, 36u, ()
#define PPUTLIMPL_UDEC_1306u 0x51Au, 10u, 36u, (2u, 653u)
#define PPUTLIMPL_UDEC_1305u 0x519u, 10u, 36u, (3u, 3u, 5u, 29u)
#define PPUTLIMPL_UDEC_1304u 0x518u, 10u, 36u, (2u, 2u, 2u, 163u)
#define PPUTLIMPL_UDEC_1303u 0x517u, 10u, 36u, ()
#define PPUTLIMPL_UDEC_1302u 0x516u, 10u, 36u, (2u, 3u, 7u, 31u)
#define PPUTLIMPL_UDEC_1301u 0x515u, 10u, 36u, ()
#define PPUTLIMPL_UDEC_1300u 0x514u, 10u, 36u, (2u, 2u, 5u, 5u, 13u)
#define PPUTLIMPL_UDEC_1299u 0x513u, 10u, 36u, (3u, 433u)
#define PPUTLIMPL_UDEC_1298u 0x512u, 10u, 36u, (2u, 11u, 59u)
#define PPUTLIMPL_UDEC_1297u 0x511u, 10u, 36u, ()
#define PPUTLIMPL_UDEC_1296u 0x510u, 10u, 36u, (2u, 2u, 2u, 2u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UDEC_1295u 0x50Fu, 10u, 35u, (5u, 7u, 37u)
#define PPUTLIMPL_UDEC_1294u 0x50Eu, 10u, 35u, (2u, 647u)
#define PPUTLIMPL_UDEC_1293u 0x50Du, 10u, 35u, (3u, 431u)
#define PPUTLIMPL_UDEC_1292u 0x50Cu, 10u, 35u, (2u, 2u, 17u, 19u)
#define PPUTLIMPL_UDEC_1291u 0x50Bu, 10u, 35u, ()
#define PPUTLIMPL_UDEC_1290u 0x50Au, 10u, 35u, (2u, 3u, 5u, 43u)
#define PPUTLIMPL_UDEC_1289u 0x509u, 10u, 35u, ()
#define PPUTLIMPL_UDEC_1288u 0x508u, 10u, 35u, (2u, 2u, 2u, 7u, 23u)
#define PPUTLIMPL_UDEC_1287u 0x507u, 10u, 35u, (3u, 3u, 11u, 13u)
#define PPUTLIMPL_UDEC_1286u 0x506u, 10u, 35u, (2u, 643u)
#define PPUTLIMPL_UDEC_1285u 0x505u, 10u, 35u, (5u, 257u)
#define PPUTLIMPL_UDEC_1284u 0x504u, 10u, 35u, (2u, 2u, 3u, 107u)
#define PPUTLIMPL_UDEC_1283u 0x503u, 10u, 35u, ()
#define PPUTLIMPL_UDEC_1282u 0x502u, 10u, 35u, (2u, 641u)
#define PPUTLIMPL_UDEC_1281u 0x501u, 10u, 35u, (3u, 7u, 61u)
#define PPUTLIMPL_UDEC_1280u 0x500u, 10u, 35u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 5u)
#define PPUTLIMPL_UDEC_1279u 0x4FFu, 10u, 35u, ()
#define PPUTLIMPL_UDEC_1278u 0x4FEu, 10u, 35u, (2u, 3u, 3u, 71u)
#define PPUTLIMPL_UDEC_1277u 0x4FDu, 10u, 35u, ()
#define PPUTLIMPL_UDEC_1276u 0x4FCu, 10u, 35u, (2u, 2u, 11u, 29u)
#define PPUTLIMPL_UDEC_1275u 0x4FBu, 10u, 35u, (3u, 5u, 5u, 17u)
#define PPUTLIMPL_UDEC_1274u 0x4FAu, 10u, 35u, (2u, 7u, 7u, 13u)
#define PPUTLIMPL_UDEC_1273u 0x4F9u, 10u, 35u, (19u, 67u)
#define PPUTLIMPL_UDEC_1272u 0x4F8u, 10u, 35u, (2u, 2u, 2u, 3u, 53u)
#define PPUTLIMPL_UDEC_1271u 0x4F7u, 10u, 35u, (31u, 41u)
#define PPUTLIMPL_UDEC_1270u 0x4F6u, 10u, 35u, (2u, 5u, 127u)
#define PPUTLIMPL_UDEC_1269u 0x4F5u, 10u, 35u, (3u, 3u, 3u, 47u)
#define PPUTLIMPL_UDEC_1268u 0x4F4u, 10u, 35u, (2u, 2u, 317u)
#define PPUTLIMPL_UDEC_1267u 0x4F3u, 10u, 35u, (7u, 181u)
#define PPUTLIMPL_UDEC_1266u 0x4F2u, 10u, 35u, (2u, 3u, 211u)
#define PPUTLIMPL_UDEC_1265u 0x4F1u, 10u, 35u, (5u, 11u, 23u)
#define PPUTLIMPL_UDEC_1264u 0x4F0u, 10u, 35u, (2u, 2u, 2u, 2u, 79u)
#define PPUTLIMPL_UDEC_1263u 0x4EFu, 10u, 35u, (3u, 421u)
#define PPUTLIMPL_UDEC_1262u 0x4EEu, 10u, 35u, (2u, 631u)
#define PPUTLIMPL_UDEC_1261u 0x4EDu, 10u, 35u, (13u, 97u)
#define PPUTLIMPL_UDEC_1260u 0x4ECu, 10u, 35u, (2u, 2u, 3u, 3u, 5u, 7u)
#define PPUTLIMPL_UDEC_1259u 0x4EBu, 10u, 35u, ()
#define PPUTLIMPL_UDEC_1258u 0x4EAu, 10u, 35u, (2u, 17u, 37u)
#define PPUTLIMPL_UDEC_1257u 0x4E9u, 10u, 35u, (3u, 419u)
#define PPUTLIMPL_UDEC_1256u 0x4E8u, 10u, 35u, (2u, 2u, 2u, 157u)
#define PPUTLIMPL_UDEC_1255u 0x4E7u, 10u, 35u, (5u, 251u)
#define PPUTLIMPL_UDEC_1254u 0x4E6u, 10u, 35u, (2u, 3u, 11u, 19u)
#define PPUTLIMPL_UDEC_1253u 0x4E5u, 10u, 35u, (7u, 179u)
#define PPUTLIMPL_UDEC_1252u 0x4E4u, 10u, 35u, (2u, 2u, 313u)
#define PPUTLIMPL_UDEC_1251u 0x4E3u, 10u, 35u, (3u, 3u, 139u)
#define PPUTLIMPL_UDEC_1250u 0x4E2u, 10u, 35u, (2u, 5u, 5u, 5u, 5u)
#define PPUTLIMPL_UDEC_1249u 0x4E1u, 10u, 35u, ()
#define PPUTLIMPL_UDEC_1248u 0x4E0u, 10u, 35u, (2u, 2u, 2u, 2u, 2u, 3u, 13u)
#define PPUTLIMPL_UDEC_1247u 0x4DFu, 10u, 35u, (29u, 43u)
#define PPUTLIMPL_UDEC_1246u 0x4DEu, 10u, 35u, (2u, 7u, 89u)
#define PPUTLIMPL_UDEC_1245u 0x4DDu, 10u, 35u, (3u, 5u, 83u)
#define PPUTLIMPL_UDEC_1244u 0x4DCu, 10u, 35u, (2u, 2u, 311u)
#define PPUTLIMPL_UDEC_1243u 0x4DBu, 10u, 35u, (11u, 113u)
#define PPUTLIMPL_UDEC_1242u 0x4DAu, 10u, 35u, (2u, 3u, 3u, 3u, 23u)
#define PPUTLIMPL_UDEC_1241u 0x4D9u, 10u, 35u, (17u, 73u)
#define PPUTLIMPL_UDEC_1240u 0x4D8u, 10u, 35u, (2u, 2u, 2u, 5u, 31u)
#define PPUTLIMPL_UDEC_1239u 0x4D7u, 10u, 35u, (3u, 7u, 59u)
#define PPUTLIMPL_UDEC_1238u 0x4D6u, 10u, 35u, (2u, 619u)
#define PPUTLIMPL_UDEC_1237u 0x4D5u, 10u, 35u, ()
#define PPUTLIMPL_UDEC_1236u 0x4D4u, 10u, 35u, (2u, 2u, 3u, 103u)
#define PPUTLIMPL_UDEC_1235u 0x4D3u, 10u, 35u, (5u, 13u, 19u)
#define PPUTLIMPL_UDEC_1234u 0x4D2u, 10u, 35u, (2u, 617u)
#define PPUTLIMPL_UDEC_1233u 0x4D1u, 10u, 35u, (3u, 3u, 137u)
#define PPUTLIMPL_UDEC_1232u 0x4D0u, 10u, 35u, (2u, 2u, 2u, 2u, 7u, 11u)
#define PPUTLIMPL_UDEC_1231u 0x4CFu, 10u, 35u, ()
#define PPUTLIMPL_UDEC_1230u 0x4CEu, 10u, 35u, (2u, 3u, 5u, 41u)
#define PPUTLIMPL_UDEC_1229u 0x4CDu, 10u, 35u, ()
#define PPUTLIMPL_UDEC_1228u 0x4CCu, 10u, 35u, (2u, 2u, 307u)
#define PPUTLIMPL_UDEC_1227u 0x4CBu, 10u, 35u, (3u, 409u)
#define PPUTLIMPL_UDEC_1226u 0x4CAu, 10u, 35u, (2u, 613u)
#define PPUTLIMPL_UDEC_1225u 0x4C9u, 10u, 35u, (5u, 5u, 7u, 7u)
#define PPUTLIMPL_UDEC_1224u 0x4C8u, 10u, 34u, (2u, 2u, 2u, 3u, 3u, 17u)
#define PPUTLIMPL_UDEC_1223u 0x4C7u, 10u, 34u, ()
#define PPUTLIMPL_UDEC_1222u 0x4C6u, 10u, 34u, (2u, 13u, 47u)
#define PPUTLIMPL_UDEC_1221u 0x4C5u, 10u, 34u, (3u, 11u, 37u)
#define PPUTLIMPL_UDEC_1220u 0x4C4u, 10u, 34u, (2u, 2u, 5u, 61u)
#define PPUTLIMPL_UDEC_1219u 0x4C3u, 10u, 34u, (23u, 53u)
#define PPUTLIMPL_UDEC_1218u 0x4C2u, 10u, 34u, (2u, 3u, 7u, 29u)
#define PPUTLIMPL_UDEC_1217u 0x4C1u, 10u, 34u, ()
#define PPUTLIMPL_UDEC_1216u 0x4C0u, 10u, 34u, (2u, 2u, 2u, 2u, 2u, 2u, 19u)
#define PPUTLIMPL_UDEC_1215u 0x4BFu, 10u, 34u, (3u, 3u, 3u, 3u, 3u, 5u)
#define PPUTLIMPL_UDEC_1214u 0x4BEu, 10u, 34u, (2u, 607u)
#define PPUTLIMPL_UDEC_1213u 0x4BDu, 10u, 34u, ()
#define PPUTLIMPL_UDEC_1212u 0x4BCu, 10u, 34u, (2u, 2u, 3u, 101u)
#define PPUTLIMPL_UDEC_1211u 0x4BBu, 10u, 34u, (7u, 173u)
#define PPUTLIMPL_UDEC_1210u 0x4BAu, 10u, 34u, (2u, 5u, 11u, 11u)
#define PPUTLIMPL_UDEC_1209u 0x4B9u, 10u, 34u, (3u, 13u, 31u)
#define PPUTLIMPL_UDEC_1208u 0x4B8u, 10u, 34u, (2u, 2u, 2u, 151u)
#define PPUTLIMPL_UDEC_1207u 0x4B7u, 10u, 34u, (17u, 71u)
#define PPUTLIMPL_UDEC_1206u 0x4B6u, 10u, 34u, (2u, 3u, 3u, 67u)
#define PPUTLIMPL_UDEC_1205u 0x4B5u, 10u, 34u, (5u, 241u)
#define PPUTLIMPL_UDEC_1204u 0x4B4u, 10u, 34u, (2u, 2u, 7u, 43u)
#define PPUTLIMPL_UDEC_1203u 0x4B3u, 10u, 34u, (3u, 401u)
#define PPUTLIMPL_UDEC_1202u 0x4B2u, 10u, 34u, (2u, 601u)
#define PPUTLIMPL_UDEC_1201u 0x4B1u, 10u, 34u, ()
#define PPUTLIMPL_UDEC_1200u 0x4B0u, 10u, 34u, (2u, 2u, 2u, 2u, 3u, 5u, 5u)
#define PPUTLIMPL_UDEC_1199u 0x4AFu, 10u, 34u, (11u, 109u)
#define PPUTLIMPL_UDEC_1198u 0x4AEu, 10u, 34u, (2u, 599u)
#define PPUTLIMPL_UDEC_1197u 0x4ADu, 10u, 34u, (3u, 3u, 7u, 19u)
#define PPUTLIMPL_UDEC_1196u 0x4ACu, 10u, 34u, (2u, 2u, 13u, 23u)
#define PPUTLIMPL_UDEC_1195u 0x4ABu, 10u, 34u, (5u, 239u)
#define PPUTLIMPL_UDEC_1194u 0x4AAu, 10u, 34u, (2u, 3u, 199u)
#define PPUTLIMPL_UDEC_1193u 0x4A9u, 10u, 34u, ()
#define PPUTLIMPL_UDEC_1192u 0x4A8u, 10u, 34u, (2u, 2u, 2u, 149u)
#define PPUTLIMPL_UDEC_1191u 0x4A7u, 10u, 34u, (3u, 397u)
#define PPUTLIMPL_UDEC_1190u 0x4A6u, 10u, 34u, (2u, 5u, 7u, 17u)
#define PPUTLIMPL_UDEC_1189u 0x4A5u, 10u, 34u, (29u, 41u)
#define PPUTLIMPL_UDEC_1188u 0x4A4u, 10u, 34u, (2u, 2u, 3u, 3u, 3u, 11u)
#define PPUTLIMPL_UDEC_1187u 0x4A3u, 10u, 34u, ()
#define PPUTLIMPL_UDEC_1186u 0x4A2u, 10u, 34u, (2u, 593u)
#define PPUTLIMPL_UDEC_1185u 0x4A1u, 10u, 34u, (3u, 5u, 79u)
#define PPUTLIMPL_UDEC_1184u 0x4A0u, 10u, 34u, (2u, 2u, 2u, 2u, 2u, 37u)
#define PPUTLIMPL_UDEC_1183u 0x49Fu, 10u, 34u, (7u, 13u, 13u)
#define PPUTLIMPL_UDEC_1182u 0x49Eu, 10u, 34u, (2u, 3u, 197u)
#define PPUTLIMPL_UDEC_1181u 0x49Du, 10u, 34u, ()
#define PPUTLIMPL_UDEC_1180u 0x49Cu, 10u, 34u, (2u, 2u, 5u, 59u)
#define PPUTLIMPL_UDEC_1179u 0x49Bu, 10u, 34u, (3u, 3u, 131u)
#define PPUTLIMPL_UDEC_1178u 0x49Au, 10u, 34u, (2u, 19u, 31u)
#define PPUTLIMPL_UDEC_1177u 0x499u, 10u, 34u, (11u, 107u)
#define PPUTLIMPL_UDEC_1176u 0x498u, 10u, 34u, (2u, 2u, 2u, 3u, 7u, 7u)
#define PPUTLIMPL_UDEC_1175u 0x497u, 10u, 34u, (5u, 5u, 47u)
#define PPUTLIMPL_UDEC_1174u 0x496u, 10u, 34u, (2u, 587u)
#define PPUTLIMPL_UDEC_1173u 0x495u, 10u, 34u, (3u, 17u, 23u)
#define PPUTLIMPL_UDEC_1172u 0x494u, 10u, 34u, (2u, 2u, 293u)
#define PPUTLIMPL_UDEC_1171u 0x493u, 10u, 34u, ()
#define PPUTLIMPL_UDEC_1170u 0x492u, 10u, 34u, (2u, 3u, 3u, 5u, 13u)
#define PPUTLIMPL_UDEC_1169u 0x491u, 10u, 34u, (7u, 167u)
#define PPUTLIMPL_UDEC_1168u 0x490u, 10u, 34u, (2u, 2u, 2u, 2u, 73u)
#define PPUTLIMPL_UDEC_1167u 0x48Fu, 10u, 34u, (3u, 389u)
#define PPUTLIMPL_UDEC_1166u 0x48Eu, 10u, 34u, (2u, 11u, 53u)
#define PPUTLIMPL_UDEC_1165u 0x48Du, 10u, 34u, (5u, 233u)
#define PPUTLIMPL_UDEC_1164u 0x48Cu, 10u, 34u, (2u, 2u, 3u, 97u)
#define PPUTLIMPL_UDEC_1163u 0x48Bu, 10u, 34u, ()
#define PPUTLIMPL_UDEC_1162u 0x48Au, 10u, 34u, (2u, 7u, 83u)
#define PPUTLIMPL_UDEC_1161u 0x489u, 10u, 34u, (3u, 3u, 3u, 43u)
#define PPUTLIMPL_UDEC_1160u 0x488u, 10u, 34u, (2u, 2u, 2u, 5u, 29u)
#define PPUTLIMPL_UDEC_1159u 0x487u, 10u, 34u, (19u, 61u)
#define PPUTLIMPL_UDEC_1158u 0x486u, 10u, 34u, (2u, 3u, 193u)
#define PPUTLIMPL_UDEC_1157u 0x485u, 10u, 34u, (13u, 89u)
#define PPUTLIMPL_UDEC_1156u 0x484u, 10u, 34u, (2u, 2u, 17u, 17u)
#define PPUTLIMPL_UDEC_1155u 0x483u, 10u, 33u, (3u, 5u, 7u, 11u)
#define PPUTLIMPL_UDEC_1154u 0x482u, 10u, 33u, (2u, 577u)
#define PPUTLIMPL_UDEC_1153u 0x481u, 10u, 33u, ()
#define PPUTLIMPL_UDEC_1152u 0x480u, 10u, 33u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 3u, 3u)
#define PPUTLIMPL_UDEC_1151u 0x47Fu, 10u, 33u, ()
#define PPUTLIMPL_UDEC_1150u 0x47Eu, 10u, 33u, (2u, 5u, 5u, 23u)
#define PPUTLIMPL_UDEC_1149u 0x47Du, 10u, 33u, (3u, 383u)
#define PPUTLIMPL_UDEC_1148u 0x47Cu, 10u, 33u, (2u, 2u, 7u, 41u)
#define PPUTLIMPL_UDEC_1147u 0x47Bu, 10u, 33u, (31u, 37u)
#define PPUTLIMPL_UDEC_1146u 0x47Au, 10u, 33u, (2u, 3u, 191u)
#define PPUTLIMPL_UDEC_1145u 0x479u, 10u, 33u, (5u, 229u)
#define PPUTLIMPL_UDEC_1144u 0x478u, 10u, 33u, (2u, 2u, 2u, 11u, 13u)
#define PPUTLIMPL_UDEC_1143u 0x477u, 10u, 33u, (3u, 3u, 127u)
#define PPUTLIMPL_UDEC_1142u 0x476u, 10u, 33u, (2u, 571u)
#define PPUTLIMPL_UDEC_1141u 0x475u, 10u, 33u, (7u, 163u)
#define PPUTLIMPL_UDEC_1140u 0x474u, 10u, 33u, (2u, 2u, 3u, 5u, 19u)
#define PPUTLIMPL_UDEC_1139u 0x473u, 10u, 33u, (17u, 67u)
#define PPUTLIMPL_UDEC_1138u 0x472u, 10u, 33u, (2u, 569u)
#define PPUTLIMPL_UDEC_1137u 0x471u, 10u, 33u, (3u, 379u)
#define PPUTLIMPL_UDEC_1136u 0x470u, 10u, 33u, (2u, 2u, 2u, 2u, 71u)
#define PPUTLIMPL_UDEC_1135u 0x46Fu, 10u, 33u, (5u, 227u)
#define PPUTLIMPL_UDEC_1134u 0x46Eu, 10u, 33u, (2u, 3u, 3u, 3u, 3u, 7u)
#define PPUTLIMPL_UDEC_1133u 0x46Du, 10u, 33u, (11u, 103u)
#define PPUTLIMPL_UDEC_1132u 0x46Cu, 10u, 33u, (2u, 2u, 283u)
#define PPUTLIMPL_UDEC_1131u 0x46Bu, 10u, 33u, (3u, 13u, 29u)
#define PPUTLIMPL_UDEC_1130u 0x46Au, 10u, 33u, (2u, 5u, 113u)
#define PPUTLIMPL_UDEC_1129u 0x469u, 10u, 33u, ()
#define PPUTLIMPL_UDEC_1128u 0x468u, 10u, 33u, (2u, 2u, 2u, 3u, 47u)
#define PPUTLIMPL_UDEC_1127u 0x467u, 10u, 33u, (7u, 7u, 23u)
#define PPUTLIMPL_UDEC_1126u 0x466u, 10u, 33u, (2u, 563u)
#define PPUTLIMPL_UDEC_1125u 0x465u, 10u, 33u, (3u, 3u, 5u, 5u, 5u)
#define PPUTLIMPL_UDEC_1124u 0x464u, 10u, 33u, (2u, 2u, 281u)
#define PPUTLIMPL_UDEC_1123u 0x463u, 10u, 33u, ()
#define PPUTLIMPL_UDEC_1122u 0x462u, 10u, 33u, (2u, 3u, 11u, 17u)
#define PPUTLIMPL_UDEC_1121u 0x461u, 10u, 33u, (19u, 59u)
#define PPUTLIMPL_UDEC_1120u 0x460u, 10u, 33u, (2u, 2u, 2u, 2u, 2u, 5u, 7u)
#define PPUTLIMPL_UDEC_1119u 0x45Fu, 10u, 33u, (3u, 373u)
#define PPUTLIMPL_UDEC_1118u 0x45Eu, 10u, 33u, (2u, 13u, 43u)
#define PPUTLIMPL_UDEC_1117u 0x45Du, 10u, 33u, ()
#define PPUTLIMPL_UDEC_1116u 0x45Cu, 10u, 33u, (2u, 2u, 3u, 3u, 31u)
#define PPUTLIMPL_UDEC_1115u 0x45Bu, 10u, 33u, (5u, 223u)
#define PPUTLIMPL_UDEC_1114u 0x45Au, 10u, 33u, (2u, 557u)
#define PPUTLIMPL_UDEC_1113u 0x459u, 10u, 33u, (3u, 7u, 53u)
#define PPUTLIMPL_UDEC_1112u 0x458u, 10u, 33u, (2u, 2u, 2u, 139u)
#define PPUTLIMPL_UDEC_1111u 0x457u, 10u, 33u, (11u, 101u)
#define PPUTLIMPL_UDEC_1110u 0x456u, 10u, 33u, (2u, 3u, 5u, 37u)
#define PPUTLIMPL_UDEC_1109u 0x455u, 10u, 33u, ()
#define PPUTLIMPL_UDEC_1108u 0x454u, 10u, 33u, (2u, 2u, 277u)
#define PPUTLIMPL_UDEC_1107u 0x453u, 10u, 33u, (3u, 3u, 3u, 41u)
#define PPUTLIMPL_UDEC_1106u 0x452u, 10u, 33u, (2u, 7u, 79u)
#define PPUTLIMPL_UDEC_1105u 0x451u, 10u, 33u, (5u, 13u, 17u)
#define PPUTLIMPL_UDEC_1104u 0x450u, 10u, 33u, (2u, 2u, 2u, 2u, 3u, 23u)
#define PPUTLIMPL_UDEC_1103u 0x44Fu, 10u, 33u, ()
#define PPUTLIMPL_UDEC_1102u 0x44Eu, 10u, 33u, (2u, 19u, 29u)
#define PPUTLIMPL_UDEC_1101u 0x44Du, 10u, 33u, (3u, 367u)
#define PPUTLIMPL_UDEC_1100u 0x44Cu, 10u, 33u, (2u, 2u, 5u, 5u, 11u)
#define PPUTLIMPL_UDEC_1099u 0x44Bu, 10u, 33u, (7u, 157u)
#define PPUTLIMPL_UDEC_1098u 0x44Au, 10u, 33u, (2u, 3u, 3u, 61u)
#define PPUTLIMPL_UDEC_1097u 0x449u, 10u, 33u, ()
#define PPUTLIMPL_UDEC_1096u 0x448u, 10u, 33u, (2u, 2u, 2u, 137u)
#define PPUTLIMPL_UDEC_1095u 0x447u, 10u, 33u, (3u, 5u, 73u)
#define PPUTLIMPL_UDEC_1094u 0x446u, 10u, 33u, (2u, 547u)
#define PPUTLIMPL_UDEC_1093u 0x445u, 10u, 33u, ()
#define PPUTLIMPL_UDEC_1092u 0x444u, 10u, 33u, (2u, 2u, 3u, 7u, 13u)
#define PPUTLIMPL_UDEC_1091u 0x443u, 10u, 33u, ()
#define PPUTLIMPL_UDEC_1090u 0x442u, 10u, 33u, (2u, 5u, 109u)
#define PPUTLIMPL_UDEC_1089u 0x441u, 10u, 33u, (3u, 3u, 11u, 11u)
#define PPUTLIMPL_UDEC_1088u 0x440u, 10u, 32u, (2u, 2u, 2u, 2u, 2u, 2u, 17u)
#define PPUTLIMPL_UDEC_1087u 0x43Fu, 10u, 32u, ()
#define PPUTLIMPL_UDEC_1086u 0x43Eu, 10u, 32u, (2u, 3u, 181u)
#define PPUTLIMPL_UDEC_1085u 0x43Du, 10u, 32u, (5u, 7u, 31u)
#define PPUTLIMPL_UDEC_1084u 0x43Cu, 10u, 32u, (2u, 2u, 271u)
#define PPUTLIMPL_UDEC_1083u 0x43Bu, 10u, 32u, (3u, 19u, 19u)
#define PPUTLIMPL_UDEC_1082u 0x43Au, 10u, 32u, (2u, 541u)
#define PPUTLIMPL_UDEC_1081u 0x439u, 10u, 32u, (23u, 47u)
#define PPUTLIMPL_UDEC_1080u 0x438u, 10u, 32u, (2u, 2u, 2u, 3u, 3u, 3u, 5u)
#define PPUTLIMPL_UDEC_1079u 0x437u, 10u, 32u, (13u, 83u)
#define PPUTLIMPL_UDEC_1078u 0x436u, 10u, 32u, (2u, 7u, 7u, 11u)
#define PPUTLIMPL_UDEC_1077u 0x435u, 10u, 32u, (3u, 359u)
#define PPUTLIMPL_UDEC_1076u 0x434u, 10u, 32u, (2u, 2u, 269u)
#define PPUTLIMPL_UDEC_1075u 0x433u, 10u, 32u, (5u, 5u, 43u)
#define PPUTLIMPL_UDEC_1074u 0x432u, 10u, 32u, (2u, 3u, 179u)
#define PPUTLIMPL_UDEC_1073u 0x431u, 10u, 32u, (29u, 37u)
#define PPUTLIMPL_UDEC_1072u 0x430u, 10u, 32u, (2u, 2u, 2u, 2u, 67u)
#define PPUTLIMPL_UDEC_1071u 0x42Fu, 10u, 32u, (3u, 3u, 7u, 17u)
#define PPUTLIMPL_UDEC_1070u 0x42Eu, 10u, 32u, (2u, 5u, 107u)
#define PPUTLIMPL_UDEC_1069u 0x42Du, 10u, 32u, ()
#define PPUTLIMPL_UDEC_1068u 0x42Cu, 10u, 32u, (2u, 2u, 3u, 89u)
#define PPUTLIMPL_UDEC_1067u 0x42Bu, 10u, 32u, (11u, 97u)
#define PPUTLIMPL_UDEC_1066u 0x42Au, 10u, 32u, (2u, 13u, 41u)
#define PPUTLIMPL_UDEC_1065u 0x429u, 10u, 32u, (3u, 5u, 71u)
#define PPUTLIMPL_UDEC_1064u 0x428u, 10u, 32u, (2u, 2u, 2u, 7u, 19u)
#define PPUTLIMPL_UDEC_1063u 0x427u, 10u, 32u, ()
#define PPUTLIMPL_UDEC_1062u 0x426u, 10u, 32u, (2u, 3u, 3u, 59u)
#define PPUTLIMPL_UDEC_1061u 0x425u, 10u, 32u, ()
#define PPUTLIMPL_UDEC_1060u 0x424u, 10u, 32u, (2u, 2u, 5u, 53u)
#define PPUTLIMPL_UDEC_1059u 0x423u, 10u, 32u, (3u, 353u)
#define PPUTLIMPL_UDEC_1058u 0x422u, 10u, 32u, (2u, 23u, 23u)
#define PPUTLIMPL_UDEC_1057u 0x421u, 10u, 32u, (7u, 151u)
#define PPUTLIMPL_UDEC_1056u 0x420u, 10u, 32u, (2u, 2u, 2u, 2u, 2u, 3u, 11u)
#define PPUTLIMPL_UDEC_1055u 0x41Fu, 10u, 32u, (5u, 211u)
#define PPUTLIMPL_UDEC_1054u 0x41Eu, 10u, 32u, (2u, 17u, 31u)
#define PPUTLIMPL_UDEC_1053u 0x41Du, 10u, 32u, (3u, 3u, 3u, 3u, 13u)
#define PPUTLIMPL_UDEC_1052u 0x41Cu, 10u, 32u, (2u, 2u, 263u)
#define PPUTLIMPL_UDEC_1051u 0x41Bu, 10u, 32u, ()
#define PPUTLIMPL_UDEC_1050u 0x41Au, 10u, 32u, (2u, 3u, 5u, 5u, 7u)
#define PPUTLIMPL_UDEC_1049u 0x419u, 10u, 32u, ()
#define PPUTLIMPL_UDEC_1048u 0x418u, 10u, 32u, (2u, 2u, 2u, 131u)
#define PPUTLIMPL_UDEC_1047u 0x417u, 10u, 32u, (3u, 349u)
#define PPUTLIMPL_UDEC_1046u 0x416u, 10u, 32u, (2u, 523u)
#define PPUTLIMPL_UDEC_1045u 0x415u, 10u, 32u, (5u, 11u, 19u)
#define PPUTLIMPL_UDEC_1044u 0x414u, 10u, 32u, (2u, 2u, 3u, 3u, 29u)
#define PPUTLIMPL_UDEC_1043u 0x413u, 10u, 32u, (7u, 149u)
#define PPUTLIMPL_UDEC_1042u 0x412u, 10u, 32u, (2u, 521u)
#define PPUTLIMPL_UDEC_1041u 0x411u, 10u, 32u, (3u, 347u)
#define PPUTLIMPL_UDEC_1040u 0x410u, 10u, 32u, (2u, 2u, 2u, 2u, 5u, 13u)
#define PPUTLIMPL_UDEC_1039u 0x40Fu, 10u, 32u, ()
#define PPUTLIMPL_UDEC_1038u 0x40Eu, 10u, 32u, (2u, 3u, 173u)
#define PPUTLIMPL_UDEC_1037u 0x40Du, 10u, 32u, (17u, 61u)
#define PPUTLIMPL_UDEC_1036u 0x40Cu, 10u, 32u, (2u, 2u, 7u, 37u)
#define PPUTLIMPL_UDEC_1035u 0x40Bu, 10u, 32u, (3u, 3u, 5u, 23u)
#define PPUTLIMPL_UDEC_1034u 0x40Au, 10u, 32u, (2u, 11u, 47u)
#define PPUTLIMPL_UDEC_1033u 0x409u, 10u, 32u, ()
#define PPUTLIMPL_UDEC_1032u 0x408u, 10u, 32u, (2u, 2u, 2u, 3u, 43u)
#define PPUTLIMPL_UDEC_1031u 0x407u, 10u, 32u, ()
#define PPUTLIMPL_UDEC_1030u 0x406u, 10u, 32u, (2u, 5u, 103u)
#define PPUTLIMPL_UDEC_1029u 0x405u, 10u, 32u, (3u, 7u, 7u, 7u)
#define PPUTLIMPL_UDEC_1028u 0x404u, 10u, 32u, (2u, 2u, 257u)
#define PPUTLIMPL_UDEC_1027u 0x403u, 10u, 32u, (13u, 79u)
#define PPUTLIMPL_UDEC_1026u 0x402u, 10u, 32u, (2u, 3u, 3u, 3u, 19u)
#define PPUTLIMPL_UDEC_1025u 0x401u, 10u, 32u, (5u, 5u, 41u)
#define PPUTLIMPL_UDEC_1024u 0x400u, 10u, 32u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u)
#define PPUTLIMPL_UDEC_1023u 0x3FFu, 9u, 31u, (3u, 11u, 31u)
#define PPUTLIMPL_UDEC_1022u 0x3FEu, 9u, 31u, (2u, 7u, 73u)
#define PPUTLIMPL_UDEC_1021u 0x3FDu, 9u, 31u, ()
#define PPUTLIMPL_UDEC_1020u 0x3FCu, 9u, 31u, (2u, 2u, 3u, 5u, 17u)
#define PPUTLIMPL_UDEC_1019u 0x3FBu, 9u, 31u, ()
#define PPUTLIMPL_UDEC_1018u 0x3FAu, 9u, 31u, (2u, 509u)
#define PPUTLIMPL_UDEC_1017u 0x3F9u, 9u, 31u, (3u, 3u, 113u)
#define PPUTLIMPL_UDEC_1016u 0x3F8u, 9u, 31u, (2u, 2u, 2u, 127u)
#define PPUTLIMPL_UDEC_1015u 0x3F7u, 9u, 31u, (5u, 7u, 29u)
#define PPUTLIMPL_UDEC_1014u 0x3F6u, 9u, 31u, (2u, 3u, 13u, 13u)
#define PPUTLIMPL_UDEC_1013u 0x3F5u, 9u, 31u, ()
#define PPUTLIMPL_UDEC_1012u 0x3F4u, 9u, 31u, (2u, 2u, 11u, 23u)
#define PPUTLIMPL_UDEC_1011u 0x3F3u, 9u, 31u, (3u, 337u)
#define PPUTLIMPL_UDEC_1010u 0x3F2u, 9u, 31u, (2u, 5u, 101u)
#define PPUTLIMPL_UDEC_1009u 0x3F1u, 9u, 31u, ()
#define PPUTLIMPL_UDEC_1008u 0x3F0u, 9u, 31u, (2u, 2u, 2u, 2u, 3u, 3u, 7u)
#define PPUTLIMPL_UDEC_1007u 0x3EFu, 9u, 31u, (19u, 53u)
#define PPUTLIMPL_UDEC_1006u 0x3EEu, 9u, 31u, (2u, 503u)
#define PPUTLIMPL_UDEC_1005u 0x3EDu, 9u, 31u, (3u, 5u, 67u)
#define PPUTLIMPL_UDEC_1004u 0x3ECu, 9u, 31u, (2u, 2u, 251u)
#define PPUTLIMPL_UDEC_1003u 0x3EBu, 9u, 31u, (17u, 59u)
#define PPUTLIMPL_UDEC_1002u 0x3EAu, 9u, 31u, (2u, 3u, 167u)
#define PPUTLIMPL_UDEC_1001u 0x3E9u, 9u, 31u, (7u, 11u, 13u)
#define PPUTLIMPL_UDEC_1000u 0x3E8u, 9u, 31u, (2u, 2u, 2u, 5u, 5u, 5u)
#define PPUTLIMPL_UDEC_999u  0x3E7u, 9u, 31u, (3u, 3u, 3u, 37u)
#define PPUTLIMPL_UDEC_998u  0x3E6u, 9u, 31u, (2u, 499u)
#define PPUTLIMPL_UDEC_997u  0x3E5u, 9u, 31u, ()
#define PPUTLIMPL_UDEC_996u  0x3E4u, 9u, 31u, (2u, 2u, 3u, 83u)
#define PPUTLIMPL_UDEC_995u  0x3E3u, 9u, 31u, (5u, 199u)
#define PPUTLIMPL_UDEC_994u  0x3E2u, 9u, 31u, (2u, 7u, 71u)
#define PPUTLIMPL_UDEC_993u  0x3E1u, 9u, 31u, (3u, 331u)
#define PPUTLIMPL_UDEC_992u  0x3E0u, 9u, 31u, (2u, 2u, 2u, 2u, 2u, 31u)
#define PPUTLIMPL_UDEC_991u  0x3DFu, 9u, 31u, ()
#define PPUTLIMPL_UDEC_990u  0x3DEu, 9u, 31u, (2u, 3u, 3u, 5u, 11u)
#define PPUTLIMPL_UDEC_989u  0x3DDu, 9u, 31u, (23u, 43u)
#define PPUTLIMPL_UDEC_988u  0x3DCu, 9u, 31u, (2u, 2u, 13u, 19u)
#define PPUTLIMPL_UDEC_987u  0x3DBu, 9u, 31u, (3u, 7u, 47u)
#define PPUTLIMPL_UDEC_986u  0x3DAu, 9u, 31u, (2u, 17u, 29u)
#define PPUTLIMPL_UDEC_985u  0x3D9u, 9u, 31u, (5u, 197u)
#define PPUTLIMPL_UDEC_984u  0x3D8u, 9u, 31u, (2u, 2u, 2u, 3u, 41u)
#define PPUTLIMPL_UDEC_983u  0x3D7u, 9u, 31u, ()
#define PPUTLIMPL_UDEC_982u  0x3D6u, 9u, 31u, (2u, 491u)
#define PPUTLIMPL_UDEC_981u  0x3D5u, 9u, 31u, (3u, 3u, 109u)
#define PPUTLIMPL_UDEC_980u  0x3D4u, 9u, 31u, (2u, 2u, 5u, 7u, 7u)
#define PPUTLIMPL_UDEC_979u  0x3D3u, 9u, 31u, (11u, 89u)
#define PPUTLIMPL_UDEC_978u  0x3D2u, 9u, 31u, (2u, 3u, 163u)
#define PPUTLIMPL_UDEC_977u  0x3D1u, 9u, 31u, ()
#define PPUTLIMPL_UDEC_976u  0x3D0u, 9u, 31u, (2u, 2u, 2u, 2u, 61u)
#define PPUTLIMPL_UDEC_975u  0x3CFu, 9u, 31u, (3u, 5u, 5u, 13u)
#define PPUTLIMPL_UDEC_974u  0x3CEu, 9u, 31u, (2u, 487u)
#define PPUTLIMPL_UDEC_973u  0x3CDu, 9u, 31u, (7u, 139u)
#define PPUTLIMPL_UDEC_972u  0x3CCu, 9u, 31u, (2u, 2u, 3u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UDEC_971u  0x3CBu, 9u, 31u, ()
#define PPUTLIMPL_UDEC_970u  0x3CAu, 9u, 31u, (2u, 5u, 97u)
#define PPUTLIMPL_UDEC_969u  0x3C9u, 9u, 31u, (3u, 17u, 19u)
#define PPUTLIMPL_UDEC_968u  0x3C8u, 9u, 31u, (2u, 2u, 2u, 11u, 11u)
#define PPUTLIMPL_UDEC_967u  0x3C7u, 9u, 31u, ()
#define PPUTLIMPL_UDEC_966u  0x3C6u, 9u, 31u, (2u, 3u, 7u, 23u)
#define PPUTLIMPL_UDEC_965u  0x3C5u, 9u, 31u, (5u, 193u)
#define PPUTLIMPL_UDEC_964u  0x3C4u, 9u, 31u, (2u, 2u, 241u)
#define PPUTLIMPL_UDEC_963u  0x3C3u, 9u, 31u, (3u, 3u, 107u)
#define PPUTLIMPL_UDEC_962u  0x3C2u, 9u, 31u, (2u, 13u, 37u)
#define PPUTLIMPL_UDEC_961u  0x3C1u, 9u, 31u, (31u, 31u)
#define PPUTLIMPL_UDEC_960u  0x3C0u, 9u, 30u, (2u, 2u, 2u, 2u, 2u, 2u, 3u, 5u)
#define PPUTLIMPL_UDEC_959u  0x3BFu, 9u, 30u, (7u, 137u)
#define PPUTLIMPL_UDEC_958u  0x3BEu, 9u, 30u, (2u, 479u)
#define PPUTLIMPL_UDEC_957u  0x3BDu, 9u, 30u, (3u, 11u, 29u)
#define PPUTLIMPL_UDEC_956u  0x3BCu, 9u, 30u, (2u, 2u, 239u)
#define PPUTLIMPL_UDEC_955u  0x3BBu, 9u, 30u, (5u, 191u)
#define PPUTLIMPL_UDEC_954u  0x3BAu, 9u, 30u, (2u, 3u, 3u, 53u)
#define PPUTLIMPL_UDEC_953u  0x3B9u, 9u, 30u, ()
#define PPUTLIMPL_UDEC_952u  0x3B8u, 9u, 30u, (2u, 2u, 2u, 7u, 17u)
#define PPUTLIMPL_UDEC_951u  0x3B7u, 9u, 30u, (3u, 317u)
#define PPUTLIMPL_UDEC_950u  0x3B6u, 9u, 30u, (2u, 5u, 5u, 19u)
#define PPUTLIMPL_UDEC_949u  0x3B5u, 9u, 30u, (13u, 73u)
#define PPUTLIMPL_UDEC_948u  0x3B4u, 9u, 30u, (2u, 2u, 3u, 79u)
#define PPUTLIMPL_UDEC_947u  0x3B3u, 9u, 30u, ()
#define PPUTLIMPL_UDEC_946u  0x3B2u, 9u, 30u, (2u, 11u, 43u)
#define PPUTLIMPL_UDEC_945u  0x3B1u, 9u, 30u, (3u, 3u, 3u, 5u, 7u)
#define PPUTLIMPL_UDEC_944u  0x3B0u, 9u, 30u, (2u, 2u, 2u, 2u, 59u)
#define PPUTLIMPL_UDEC_943u  0x3AFu, 9u, 30u, (23u, 41u)
#define PPUTLIMPL_UDEC_942u  0x3AEu, 9u, 30u, (2u, 3u, 157u)
#define PPUTLIMPL_UDEC_941u  0x3ADu, 9u, 30u, ()
#define PPUTLIMPL_UDEC_940u  0x3ACu, 9u, 30u, (2u, 2u, 5u, 47u)
#define PPUTLIMPL_UDEC_939u  0x3ABu, 9u, 30u, (3u, 313u)
#define PPUTLIMPL_UDEC_938u  0x3AAu, 9u, 30u, (2u, 7u, 67u)
#define PPUTLIMPL_UDEC_937u  0x3A9u, 9u, 30u, ()
#define PPUTLIMPL_UDEC_936u  0x3A8u, 9u, 30u, (2u, 2u, 2u, 3u, 3u, 13u)
#define PPUTLIMPL_UDEC_935u  0x3A7u, 9u, 30u, (5u, 11u, 17u)
#define PPUTLIMPL_UDEC_934u  0x3A6u, 9u, 30u, (2u, 467u)
#define PPUTLIMPL_UDEC_933u  0x3A5u, 9u, 30u, (3u, 311u)
#define PPUTLIMPL_UDEC_932u  0x3A4u, 9u, 30u, (2u, 2u, 233u)
#define PPUTLIMPL_UDEC_931u  0x3A3u, 9u, 30u, (7u, 7u, 19u)
#define PPUTLIMPL_UDEC_930u  0x3A2u, 9u, 30u, (2u, 3u, 5u, 31u)
#define PPUTLIMPL_UDEC_929u  0x3A1u, 9u, 30u, ()
#define PPUTLIMPL_UDEC_928u  0x3A0u, 9u, 30u, (2u, 2u, 2u, 2u, 2u, 29u)
#define PPUTLIMPL_UDEC_927u  0x39Fu, 9u, 30u, (3u, 3u, 103u)
#define PPUTLIMPL_UDEC_926u  0x39Eu, 9u, 30u, (2u, 463u)
#define PPUTLIMPL_UDEC_925u  0x39Du, 9u, 30u, (5u, 5u, 37u)
#define PPUTLIMPL_UDEC_924u  0x39Cu, 9u, 30u, (2u, 2u, 3u, 7u, 11u)
#define PPUTLIMPL_UDEC_923u  0x39Bu, 9u, 30u, (13u, 71u)
#define PPUTLIMPL_UDEC_922u  0x39Au, 9u, 30u, (2u, 461u)
#define PPUTLIMPL_UDEC_921u  0x399u, 9u, 30u, (3u, 307u)
#define PPUTLIMPL_UDEC_920u  0x398u, 9u, 30u, (2u, 2u, 2u, 5u, 23u)
#define PPUTLIMPL_UDEC_919u  0x397u, 9u, 30u, ()
#define PPUTLIMPL_UDEC_918u  0x396u, 9u, 30u, (2u, 3u, 3u, 3u, 17u)
#define PPUTLIMPL_UDEC_917u  0x395u, 9u, 30u, (7u, 131u)
#define PPUTLIMPL_UDEC_916u  0x394u, 9u, 30u, (2u, 2u, 229u)
#define PPUTLIMPL_UDEC_915u  0x393u, 9u, 30u, (3u, 5u, 61u)
#define PPUTLIMPL_UDEC_914u  0x392u, 9u, 30u, (2u, 457u)
#define PPUTLIMPL_UDEC_913u  0x391u, 9u, 30u, (11u, 83u)
#define PPUTLIMPL_UDEC_912u  0x390u, 9u, 30u, (2u, 2u, 2u, 2u, 3u, 19u)
#define PPUTLIMPL_UDEC_911u  0x38Fu, 9u, 30u, ()
#define PPUTLIMPL_UDEC_910u  0x38Eu, 9u, 30u, (2u, 5u, 7u, 13u)
#define PPUTLIMPL_UDEC_909u  0x38Du, 9u, 30u, (3u, 3u, 101u)
#define PPUTLIMPL_UDEC_908u  0x38Cu, 9u, 30u, (2u, 2u, 227u)
#define PPUTLIMPL_UDEC_907u  0x38Bu, 9u, 30u, ()
#define PPUTLIMPL_UDEC_906u  0x38Au, 9u, 30u, (2u, 3u, 151u)
#define PPUTLIMPL_UDEC_905u  0x389u, 9u, 30u, (5u, 181u)
#define PPUTLIMPL_UDEC_904u  0x388u, 9u, 30u, (2u, 2u, 2u, 113u)
#define PPUTLIMPL_UDEC_903u  0x387u, 9u, 30u, (3u, 7u, 43u)
#define PPUTLIMPL_UDEC_902u  0x386u, 9u, 30u, (2u, 11u, 41u)
#define PPUTLIMPL_UDEC_901u  0x385u, 9u, 30u, (17u, 53u)
#define PPUTLIMPL_UDEC_900u  0x384u, 9u, 30u, (2u, 2u, 3u, 3u, 5u, 5u)
#define PPUTLIMPL_UDEC_899u  0x383u, 9u, 29u, (29u, 31u)
#define PPUTLIMPL_UDEC_898u  0x382u, 9u, 29u, (2u, 449u)
#define PPUTLIMPL_UDEC_897u  0x381u, 9u, 29u, (3u, 13u, 23u)
#define PPUTLIMPL_UDEC_896u  0x380u, 9u, 29u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 7u)
#define PPUTLIMPL_UDEC_895u  0x37Fu, 9u, 29u, (5u, 179u)
#define PPUTLIMPL_UDEC_894u  0x37Eu, 9u, 29u, (2u, 3u, 149u)
#define PPUTLIMPL_UDEC_893u  0x37Du, 9u, 29u, (19u, 47u)
#define PPUTLIMPL_UDEC_892u  0x37Cu, 9u, 29u, (2u, 2u, 223u)
#define PPUTLIMPL_UDEC_891u  0x37Bu, 9u, 29u, (3u, 3u, 3u, 3u, 11u)
#define PPUTLIMPL_UDEC_890u  0x37Au, 9u, 29u, (2u, 5u, 89u)
#define PPUTLIMPL_UDEC_889u  0x379u, 9u, 29u, (7u, 127u)
#define PPUTLIMPL_UDEC_888u  0x378u, 9u, 29u, (2u, 2u, 2u, 3u, 37u)
#define PPUTLIMPL_UDEC_887u  0x377u, 9u, 29u, ()
#define PPUTLIMPL_UDEC_886u  0x376u, 9u, 29u, (2u, 443u)
#define PPUTLIMPL_UDEC_885u  0x375u, 9u, 29u, (3u, 5u, 59u)
#define PPUTLIMPL_UDEC_884u  0x374u, 9u, 29u, (2u, 2u, 13u, 17u)
#define PPUTLIMPL_UDEC_883u  0x373u, 9u, 29u, ()
#define PPUTLIMPL_UDEC_882u  0x372u, 9u, 29u, (2u, 3u, 3u, 7u, 7u)
#define PPUTLIMPL_UDEC_881u  0x371u, 9u, 29u, ()
#define PPUTLIMPL_UDEC_880u  0x370u, 9u, 29u, (2u, 2u, 2u, 2u, 5u, 11u)
#define PPUTLIMPL_UDEC_879u  0x36Fu, 9u, 29u, (3u, 293u)
#define PPUTLIMPL_UDEC_878u  0x36Eu, 9u, 29u, (2u, 439u)
#define PPUTLIMPL_UDEC_877u  0x36Du, 9u, 29u, ()
#define PPUTLIMPL_UDEC_876u  0x36Cu, 9u, 29u, (2u, 2u, 3u, 73u)
#define PPUTLIMPL_UDEC_875u  0x36Bu, 9u, 29u, (5u, 5u, 5u, 7u)
#define PPUTLIMPL_UDEC_874u  0x36Au, 9u, 29u, (2u, 19u, 23u)
#define PPUTLIMPL_UDEC_873u  0x369u, 9u, 29u, (3u, 3u, 97u)
#define PPUTLIMPL_UDEC_872u  0x368u, 9u, 29u, (2u, 2u, 2u, 109u)
#define PPUTLIMPL_UDEC_871u  0x367u, 9u, 29u, (13u, 67u)
#define PPUTLIMPL_UDEC_870u  0x366u, 9u, 29u, (2u, 3u, 5u, 29u)
#define PPUTLIMPL_UDEC_869u  0x365u, 9u, 29u, (11u, 79u)
#define PPUTLIMPL_UDEC_868u  0x364u, 9u, 29u, (2u, 2u, 7u, 31u)
#define PPUTLIMPL_UDEC_867u  0x363u, 9u, 29u, (3u, 17u, 17u)
#define PPUTLIMPL_UDEC_866u  0x362u, 9u, 29u, (2u, 433u)
#define PPUTLIMPL_UDEC_865u  0x361u, 9u, 29u, (5u, 173u)
#define PPUTLIMPL_UDEC_864u  0x360u, 9u, 29u, (2u, 2u, 2u, 2u, 2u, 3u, 3u, 3u)
#define PPUTLIMPL_UDEC_863u  0x35Fu, 9u, 29u, ()
#define PPUTLIMPL_UDEC_862u  0x35Eu, 9u, 29u, (2u, 431u)
#define PPUTLIMPL_UDEC_861u  0x35Du, 9u, 29u, (3u, 7u, 41u)
#define PPUTLIMPL_UDEC_860u  0x35Cu, 9u, 29u, (2u, 2u, 5u, 43u)
#define PPUTLIMPL_UDEC_859u  0x35Bu, 9u, 29u, ()
#define PPUTLIMPL_UDEC_858u  0x35Au, 9u, 29u, (2u, 3u, 11u, 13u)
#define PPUTLIMPL_UDEC_857u  0x359u, 9u, 29u, ()
#define PPUTLIMPL_UDEC_856u  0x358u, 9u, 29u, (2u, 2u, 2u, 107u)
#define PPUTLIMPL_UDEC_855u  0x357u, 9u, 29u, (3u, 3u, 5u, 19u)
#define PPUTLIMPL_UDEC_854u  0x356u, 9u, 29u, (2u, 7u, 61u)
#define PPUTLIMPL_UDEC_853u  0x355u, 9u, 29u, ()
#define PPUTLIMPL_UDEC_852u  0x354u, 9u, 29u, (2u, 2u, 3u, 71u)
#define PPUTLIMPL_UDEC_851u  0x353u, 9u, 29u, (23u, 37u)
#define PPUTLIMPL_UDEC_850u  0x352u, 9u, 29u, (2u, 5u, 5u, 17u)
#define PPUTLIMPL_UDEC_849u  0x351u, 9u, 29u, (3u, 283u)
#define PPUTLIMPL_UDEC_848u  0x350u, 9u, 29u, (2u, 2u, 2u, 2u, 53u)
#define PPUTLIMPL_UDEC_847u  0x34Fu, 9u, 29u, (7u, 11u, 11u)
#define PPUTLIMPL_UDEC_846u  0x34Eu, 9u, 29u, (2u, 3u, 3u, 47u)
#define PPUTLIMPL_UDEC_845u  0x34Du, 9u, 29u, (5u, 13u, 13u)
#define PPUTLIMPL_UDEC_844u  0x34Cu, 9u, 29u, (2u, 2u, 211u)
#define PPUTLIMPL_UDEC_843u  0x34Bu, 9u, 29u, (3u, 281u)
#define PPUTLIMPL_UDEC_842u  0x34Au, 9u, 29u, (2u, 421u)
#define PPUTLIMPL_UDEC_841u  0x349u, 9u, 29u, (29u, 29u)
#define PPUTLIMPL_UDEC_840u  0x348u, 9u, 28u, (2u, 2u, 2u, 3u, 5u, 7u)
#define PPUTLIMPL_UDEC_839u  0x347u, 9u, 28u, ()
#define PPUTLIMPL_UDEC_838u  0x346u, 9u, 28u, (2u, 419u)
#define PPUTLIMPL_UDEC_837u  0x345u, 9u, 28u, (3u, 3u, 3u, 31u)
#define PPUTLIMPL_UDEC_836u  0x344u, 9u, 28u, (2u, 2u, 11u, 19u)
#define PPUTLIMPL_UDEC_835u  0x343u, 9u, 28u, (5u, 167u)
#define PPUTLIMPL_UDEC_834u  0x342u, 9u, 28u, (2u, 3u, 139u)
#define PPUTLIMPL_UDEC_833u  0x341u, 9u, 28u, (7u, 7u, 17u)
#define PPUTLIMPL_UDEC_832u  0x340u, 9u, 28u, (2u, 2u, 2u, 2u, 2u, 2u, 13u)
#define PPUTLIMPL_UDEC_831u  0x33Fu, 9u, 28u, (3u, 277u)
#define PPUTLIMPL_UDEC_830u  0x33Eu, 9u, 28u, (2u, 5u, 83u)
#define PPUTLIMPL_UDEC_829u  0x33Du, 9u, 28u, ()
#define PPUTLIMPL_UDEC_828u  0x33Cu, 9u, 28u, (2u, 2u, 3u, 3u, 23u)
#define PPUTLIMPL_UDEC_827u  0x33Bu, 9u, 28u, ()
#define PPUTLIMPL_UDEC_826u  0x33Au, 9u, 28u, (2u, 7u, 59u)
#define PPUTLIMPL_UDEC_825u  0x339u, 9u, 28u, (3u, 5u, 5u, 11u)
#define PPUTLIMPL_UDEC_824u  0x338u, 9u, 28u, (2u, 2u, 2u, 103u)
#define PPUTLIMPL_UDEC_823u  0x337u, 9u, 28u, ()
#define PPUTLIMPL_UDEC_822u  0x336u, 9u, 28u, (2u, 3u, 137u)
#define PPUTLIMPL_UDEC_821u  0x335u, 9u, 28u, ()
#define PPUTLIMPL_UDEC_820u  0x334u, 9u, 28u, (2u, 2u, 5u, 41u)
#define PPUTLIMPL_UDEC_819u  0x333u, 9u, 28u, (3u, 3u, 7u, 13u)
#define PPUTLIMPL_UDEC_818u  0x332u, 9u, 28u, (2u, 409u)
#define PPUTLIMPL_UDEC_817u  0x331u, 9u, 28u, (19u, 43u)
#define PPUTLIMPL_UDEC_816u  0x330u, 9u, 28u, (2u, 2u, 2u, 2u, 3u, 17u)
#define PPUTLIMPL_UDEC_815u  0x32Fu, 9u, 28u, (5u, 163u)
#define PPUTLIMPL_UDEC_814u  0x32Eu, 9u, 28u, (2u, 11u, 37u)
#define PPUTLIMPL_UDEC_813u  0x32Du, 9u, 28u, (3u, 271u)
#define PPUTLIMPL_UDEC_812u  0x32Cu, 9u, 28u, (2u, 2u, 7u, 29u)
#define PPUTLIMPL_UDEC_811u  0x32Bu, 9u, 28u, ()
#define PPUTLIMPL_UDEC_810u  0x32Au, 9u, 28u, (2u, 3u, 3u, 3u, 3u, 5u)
#define PPUTLIMPL_UDEC_809u  0x329u, 9u, 28u, ()
#define PPUTLIMPL_UDEC_808u  0x328u, 9u, 28u, (2u, 2u, 2u, 101u)
#define PPUTLIMPL_UDEC_807u  0x327u, 9u, 28u, (3u, 269u)
#define PPUTLIMPL_UDEC_806u  0x326u, 9u, 28u, (2u, 13u, 31u)
#define PPUTLIMPL_UDEC_805u  0x325u, 9u, 28u, (5u, 7u, 23u)
#define PPUTLIMPL_UDEC_804u  0x324u, 9u, 28u, (2u, 2u, 3u, 67u)
#define PPUTLIMPL_UDEC_803u  0x323u, 9u, 28u, (11u, 73u)
#define PPUTLIMPL_UDEC_802u  0x322u, 9u, 28u, (2u, 401u)
#define PPUTLIMPL_UDEC_801u  0x321u, 9u, 28u, (3u, 3u, 89u)
#define PPUTLIMPL_UDEC_800u  0x320u, 9u, 28u, (2u, 2u, 2u, 2u, 2u, 5u, 5u)
#define PPUTLIMPL_UDEC_799u  0x31Fu, 9u, 28u, (17u, 47u)
#define PPUTLIMPL_UDEC_798u  0x31Eu, 9u, 28u, (2u, 3u, 7u, 19u)
#define PPUTLIMPL_UDEC_797u  0x31Du, 9u, 28u, ()
#define PPUTLIMPL_UDEC_796u  0x31Cu, 9u, 28u, (2u, 2u, 199u)
#define PPUTLIMPL_UDEC_795u  0x31Bu, 9u, 28u, (3u, 5u, 53u)
#define PPUTLIMPL_UDEC_794u  0x31Au, 9u, 28u, (2u, 397u)
#define PPUTLIMPL_UDEC_793u  0x319u, 9u, 28u, (13u, 61u)
#define PPUTLIMPL_UDEC_792u  0x318u, 9u, 28u, (2u, 2u, 2u, 3u, 3u, 11u)
#define PPUTLIMPL_UDEC_791u  0x317u, 9u, 28u, (7u, 113u)
#define PPUTLIMPL_UDEC_790u  0x316u, 9u, 28u, (2u, 5u, 79u)
#define PPUTLIMPL_UDEC_789u  0x315u, 9u, 28u, (3u, 263u)
#define PPUTLIMPL_UDEC_788u  0x314u, 9u, 28u, (2u, 2u, 197u)
#define PPUTLIMPL_UDEC_787u  0x313u, 9u, 28u, ()
#define PPUTLIMPL_UDEC_786u  0x312u, 9u, 28u, (2u, 3u, 131u)
#define PPUTLIMPL_UDEC_785u  0x311u, 9u, 28u, (5u, 157u)
#define PPUTLIMPL_UDEC_784u  0x310u, 9u, 28u, (2u, 2u, 2u, 2u, 7u, 7u)
#define PPUTLIMPL_UDEC_783u  0x30Fu, 9u, 27u, (3u, 3u, 3u, 29u)
#define PPUTLIMPL_UDEC_782u  0x30Eu, 9u, 27u, (2u, 17u, 23u)
#define PPUTLIMPL_UDEC_781u  0x30Du, 9u, 27u, (11u, 71u)
#define PPUTLIMPL_UDEC_780u  0x30Cu, 9u, 27u, (2u, 2u, 3u, 5u, 13u)
#define PPUTLIMPL_UDEC_779u  0x30Bu, 9u, 27u, (19u, 41u)
#define PPUTLIMPL_UDEC_778u  0x30Au, 9u, 27u, (2u, 389u)
#define PPUTLIMPL_UDEC_777u  0x309u, 9u, 27u, (3u, 7u, 37u)
#define PPUTLIMPL_UDEC_776u  0x308u, 9u, 27u, (2u, 2u, 2u, 97u)
#define PPUTLIMPL_UDEC_775u  0x307u, 9u, 27u, (5u, 5u, 31u)
#define PPUTLIMPL_UDEC_774u  0x306u, 9u, 27u, (2u, 3u, 3u, 43u)
#define PPUTLIMPL_UDEC_773u  0x305u, 9u, 27u, ()
#define PPUTLIMPL_UDEC_772u  0x304u, 9u, 27u, (2u, 2u, 193u)
#define PPUTLIMPL_UDEC_771u  0x303u, 9u, 27u, (3u, 257u)
#define PPUTLIMPL_UDEC_770u  0x302u, 9u, 27u, (2u, 5u, 7u, 11u)
#define PPUTLIMPL_UDEC_769u  0x301u, 9u, 27u, ()
#define PPUTLIMPL_UDEC_768u  0x300u, 9u, 27u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 3u)
#define PPUTLIMPL_UDEC_767u  0x2FFu, 9u, 27u, (13u, 59u)
#define PPUTLIMPL_UDEC_766u  0x2FEu, 9u, 27u, (2u, 383u)
#define PPUTLIMPL_UDEC_765u  0x2FDu, 9u, 27u, (3u, 3u, 5u, 17u)
#define PPUTLIMPL_UDEC_764u  0x2FCu, 9u, 27u, (2u, 2u, 191u)
#define PPUTLIMPL_UDEC_763u  0x2FBu, 9u, 27u, (7u, 109u)
#define PPUTLIMPL_UDEC_762u  0x2FAu, 9u, 27u, (2u, 3u, 127u)
#define PPUTLIMPL_UDEC_761u  0x2F9u, 9u, 27u, ()
#define PPUTLIMPL_UDEC_760u  0x2F8u, 9u, 27u, (2u, 2u, 2u, 5u, 19u)
#define PPUTLIMPL_UDEC_759u  0x2F7u, 9u, 27u, (3u, 11u, 23u)
#define PPUTLIMPL_UDEC_758u  0x2F6u, 9u, 27u, (2u, 379u)
#define PPUTLIMPL_UDEC_757u  0x2F5u, 9u, 27u, ()
#define PPUTLIMPL_UDEC_756u  0x2F4u, 9u, 27u, (2u, 2u, 3u, 3u, 3u, 7u)
#define PPUTLIMPL_UDEC_755u  0x2F3u, 9u, 27u, (5u, 151u)
#define PPUTLIMPL_UDEC_754u  0x2F2u, 9u, 27u, (2u, 13u, 29u)
#define PPUTLIMPL_UDEC_753u  0x2F1u, 9u, 27u, (3u, 251u)
#define PPUTLIMPL_UDEC_752u  0x2F0u, 9u, 27u, (2u, 2u, 2u, 2u, 47u)
#define PPUTLIMPL_UDEC_751u  0x2EFu, 9u, 27u, ()
#define PPUTLIMPL_UDEC_750u  0x2EEu, 9u, 27u, (2u, 3u, 5u, 5u, 5u)
#define PPUTLIMPL_UDEC_749u  0x2EDu, 9u, 27u, (7u, 107u)
#define PPUTLIMPL_UDEC_748u  0x2ECu, 9u, 27u, (2u, 2u, 11u, 17u)
#define PPUTLIMPL_UDEC_747u  0x2EBu, 9u, 27u, (3u, 3u, 83u)
#define PPUTLIMPL_UDEC_746u  0x2EAu, 9u, 27u, (2u, 373u)
#define PPUTLIMPL_UDEC_745u  0x2E9u, 9u, 27u, (5u, 149u)
#define PPUTLIMPL_UDEC_744u  0x2E8u, 9u, 27u, (2u, 2u, 2u, 3u, 31u)
#define PPUTLIMPL_UDEC_743u  0x2E7u, 9u, 27u, ()
#define PPUTLIMPL_UDEC_742u  0x2E6u, 9u, 27u, (2u, 7u, 53u)
#define PPUTLIMPL_UDEC_741u  0x2E5u, 9u, 27u, (3u, 13u, 19u)
#define PPUTLIMPL_UDEC_740u  0x2E4u, 9u, 27u, (2u, 2u, 5u, 37u)
#define PPUTLIMPL_UDEC_739u  0x2E3u, 9u, 27u, ()
#define PPUTLIMPL_UDEC_738u  0x2E2u, 9u, 27u, (2u, 3u, 3u, 41u)
#define PPUTLIMPL_UDEC_737u  0x2E1u, 9u, 27u, (11u, 67u)
#define PPUTLIMPL_UDEC_736u  0x2E0u, 9u, 27u, (2u, 2u, 2u, 2u, 2u, 23u)
#define PPUTLIMPL_UDEC_735u  0x2DFu, 9u, 27u, (3u, 5u, 7u, 7u)
#define PPUTLIMPL_UDEC_734u  0x2DEu, 9u, 27u, (2u, 367u)
#define PPUTLIMPL_UDEC_733u  0x2DDu, 9u, 27u, ()
#define PPUTLIMPL_UDEC_732u  0x2DCu, 9u, 27u, (2u, 2u, 3u, 61u)
#define PPUTLIMPL_UDEC_731u  0x2DBu, 9u, 27u, (17u, 43u)
#define PPUTLIMPL_UDEC_730u  0x2DAu, 9u, 27u, (2u, 5u, 73u)
#define PPUTLIMPL_UDEC_729u  0x2D9u, 9u, 27u, (3u, 3u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UDEC_728u  0x2D8u, 9u, 26u, (2u, 2u, 2u, 7u, 13u)
#define PPUTLIMPL_UDEC_727u  0x2D7u, 9u, 26u, ()
#define PPUTLIMPL_UDEC_726u  0x2D6u, 9u, 26u, (2u, 3u, 11u, 11u)
#define PPUTLIMPL_UDEC_725u  0x2D5u, 9u, 26u, (5u, 5u, 29u)
#define PPUTLIMPL_UDEC_724u  0x2D4u, 9u, 26u, (2u, 2u, 181u)
#define PPUTLIMPL_UDEC_723u  0x2D3u, 9u, 26u, (3u, 241u)
#define PPUTLIMPL_UDEC_722u  0x2D2u, 9u, 26u, (2u, 19u, 19u)
#define PPUTLIMPL_UDEC_721u  0x2D1u, 9u, 26u, (7u, 103u)
#define PPUTLIMPL_UDEC_720u  0x2D0u, 9u, 26u, (2u, 2u, 2u, 2u, 3u, 3u, 5u)
#define PPUTLIMPL_UDEC_719u  0x2CFu, 9u, 26u, ()
#define PPUTLIMPL_UDEC_718u  0x2CEu, 9u, 26u, (2u, 359u)
#define PPUTLIMPL_UDEC_717u  0x2CDu, 9u, 26u, (3u, 239u)
#define PPUTLIMPL_UDEC_716u  0x2CCu, 9u, 26u, (2u, 2u, 179u)
#define PPUTLIMPL_UDEC_715u  0x2CBu, 9u, 26u, (5u, 11u, 13u)
#define PPUTLIMPL_UDEC_714u  0x2CAu, 9u, 26u, (2u, 3u, 7u, 17u)
#define PPUTLIMPL_UDEC_713u  0x2C9u, 9u, 26u, (23u, 31u)
#define PPUTLIMPL_UDEC_712u  0x2C8u, 9u, 26u, (2u, 2u, 2u, 89u)
#define PPUTLIMPL_UDEC_711u  0x2C7u, 9u, 26u, (3u, 3u, 79u)
#define PPUTLIMPL_UDEC_710u  0x2C6u, 9u, 26u, (2u, 5u, 71u)
#define PPUTLIMPL_UDEC_709u  0x2C5u, 9u, 26u, ()
#define PPUTLIMPL_UDEC_708u  0x2C4u, 9u, 26u, (2u, 2u, 3u, 59u)
#define PPUTLIMPL_UDEC_707u  0x2C3u, 9u, 26u, (7u, 101u)
#define PPUTLIMPL_UDEC_706u  0x2C2u, 9u, 26u, (2u, 353u)
#define PPUTLIMPL_UDEC_705u  0x2C1u, 9u, 26u, (3u, 5u, 47u)
#define PPUTLIMPL_UDEC_704u  0x2C0u, 9u, 26u, (2u, 2u, 2u, 2u, 2u, 2u, 11u)
#define PPUTLIMPL_UDEC_703u  0x2BFu, 9u, 26u, (19u, 37u)
#define PPUTLIMPL_UDEC_702u  0x2BEu, 9u, 26u, (2u, 3u, 3u, 3u, 13u)
#define PPUTLIMPL_UDEC_701u  0x2BDu, 9u, 26u, ()
#define PPUTLIMPL_UDEC_700u  0x2BCu, 9u, 26u, (2u, 2u, 5u, 5u, 7u)
#define PPUTLIMPL_UDEC_699u  0x2BBu, 9u, 26u, (3u, 233u)
#define PPUTLIMPL_UDEC_698u  0x2BAu, 9u, 26u, (2u, 349u)
#define PPUTLIMPL_UDEC_697u  0x2B9u, 9u, 26u, (17u, 41u)
#define PPUTLIMPL_UDEC_696u  0x2B8u, 9u, 26u, (2u, 2u, 2u, 3u, 29u)
#define PPUTLIMPL_UDEC_695u  0x2B7u, 9u, 26u, (5u, 139u)
#define PPUTLIMPL_UDEC_694u  0x2B6u, 9u, 26u, (2u, 347u)
#define PPUTLIMPL_UDEC_693u  0x2B5u, 9u, 26u, (3u, 3u, 7u, 11u)
#define PPUTLIMPL_UDEC_692u  0x2B4u, 9u, 26u, (2u, 2u, 173u)
#define PPUTLIMPL_UDEC_691u  0x2B3u, 9u, 26u, ()
#define PPUTLIMPL_UDEC_690u  0x2B2u, 9u, 26u, (2u, 3u, 5u, 23u)
#define PPUTLIMPL_UDEC_689u  0x2B1u, 9u, 26u, (13u, 53u)
#define PPUTLIMPL_UDEC_688u  0x2B0u, 9u, 26u, (2u, 2u, 2u, 2u, 43u)
#define PPUTLIMPL_UDEC_687u  0x2AFu, 9u, 26u, (3u, 229u)
#define PPUTLIMPL_UDEC_686u  0x2AEu, 9u, 26u, (2u, 7u, 7u, 7u)
#define PPUTLIMPL_UDEC_685u  0x2ADu, 9u, 26u, (5u, 137u)
#define PPUTLIMPL_UDEC_684u  0x2ACu, 9u, 26u, (2u, 2u, 3u, 3u, 19u)
#define PPUTLIMPL_UDEC_683u  0x2ABu, 9u, 26u, ()
#define PPUTLIMPL_UDEC_682u  0x2AAu, 9u, 26u, (2u, 11u, 31u)
#define PPUTLIMPL_UDEC_681u  0x2A9u, 9u, 26u, (3u, 227u)
#define PPUTLIMPL_UDEC_680u  0x2A8u, 9u, 26u, (2u, 2u, 2u, 5u, 17u)
#define PPUTLIMPL_UDEC_679u  0x2A7u, 9u, 26u, (7u, 97u)
#define PPUTLIMPL_UDEC_678u  0x2A6u, 9u, 26u, (2u, 3u, 113u)
#define PPUTLIMPL_UDEC_677u  0x2A5u, 9u, 26u, ()
#define PPUTLIMPL_UDEC_676u  0x2A4u, 9u, 26u, (2u, 2u, 13u, 13u)
#define PPUTLIMPL_UDEC_675u  0x2A3u, 9u, 25u, (3u, 3u, 3u, 5u, 5u)
#define PPUTLIMPL_UDEC_674u  0x2A2u, 9u, 25u, (2u, 337u)
#define PPUTLIMPL_UDEC_673u  0x2A1u, 9u, 25u, ()
#define PPUTLIMPL_UDEC_672u  0x2A0u, 9u, 25u, (2u, 2u, 2u, 2u, 2u, 3u, 7u)
#define PPUTLIMPL_UDEC_671u  0x29Fu, 9u, 25u, (11u, 61u)
#define PPUTLIMPL_UDEC_670u  0x29Eu, 9u, 25u, (2u, 5u, 67u)
#define PPUTLIMPL_UDEC_669u  0x29Du, 9u, 25u, (3u, 223u)
#define PPUTLIMPL_UDEC_668u  0x29Cu, 9u, 25u, (2u, 2u, 167u)
#define PPUTLIMPL_UDEC_667u  0x29Bu, 9u, 25u, (23u, 29u)
#define PPUTLIMPL_UDEC_666u  0x29Au, 9u, 25u, (2u, 3u, 3u, 37u)
#define PPUTLIMPL_UDEC_665u  0x299u, 9u, 25u, (5u, 7u, 19u)
#define PPUTLIMPL_UDEC_664u  0x298u, 9u, 25u, (2u, 2u, 2u, 83u)
#define PPUTLIMPL_UDEC_663u  0x297u, 9u, 25u, (3u, 13u, 17u)
#define PPUTLIMPL_UDEC_662u  0x296u, 9u, 25u, (2u, 331u)
#define PPUTLIMPL_UDEC_661u  0x295u, 9u, 25u, ()
#define PPUTLIMPL_UDEC_660u  0x294u, 9u, 25u, (2u, 2u, 3u, 5u, 11u)
#define PPUTLIMPL_UDEC_659u  0x293u, 9u, 25u, ()
#define PPUTLIMPL_UDEC_658u  0x292u, 9u, 25u, (2u, 7u, 47u)
#define PPUTLIMPL_UDEC_657u  0x291u, 9u, 25u, (3u, 3u, 73u)
#define PPUTLIMPL_UDEC_656u  0x290u, 9u, 25u, (2u, 2u, 2u, 2u, 41u)
#define PPUTLIMPL_UDEC_655u  0x28Fu, 9u, 25u, (5u, 131u)
#define PPUTLIMPL_UDEC_654u  0x28Eu, 9u, 25u, (2u, 3u, 109u)
#define PPUTLIMPL_UDEC_653u  0x28Du, 9u, 25u, ()
#define PPUTLIMPL_UDEC_652u  0x28Cu, 9u, 25u, (2u, 2u, 163u)
#define PPUTLIMPL_UDEC_651u  0x28Bu, 9u, 25u, (3u, 7u, 31u)
#define PPUTLIMPL_UDEC_650u  0x28Au, 9u, 25u, (2u, 5u, 5u, 13u)
#define PPUTLIMPL_UDEC_649u  0x289u, 9u, 25u, (11u, 59u)
#define PPUTLIMPL_UDEC_648u  0x288u, 9u, 25u, (2u, 2u, 2u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UDEC_647u  0x287u, 9u, 25u, ()
#define PPUTLIMPL_UDEC_646u  0x286u, 9u, 25u, (2u, 17u, 19u)
#define PPUTLIMPL_UDEC_645u  0x285u, 9u, 25u, (3u, 5u, 43u)
#define PPUTLIMPL_UDEC_644u  0x284u, 9u, 25u, (2u, 2u, 7u, 23u)
#define PPUTLIMPL_UDEC_643u  0x283u, 9u, 25u, ()
#define PPUTLIMPL_UDEC_642u  0x282u, 9u, 25u, (2u, 3u, 107u)
#define PPUTLIMPL_UDEC_641u  0x281u, 9u, 25u, ()
#define PPUTLIMPL_UDEC_640u  0x280u, 9u, 25u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 5u)
#define PPUTLIMPL_UDEC_639u  0x27Fu, 9u, 25u, (3u, 3u, 71u)
#define PPUTLIMPL_UDEC_638u  0x27Eu, 9u, 25u, (2u, 11u, 29u)
#define PPUTLIMPL_UDEC_637u  0x27Du, 9u, 25u, (7u, 7u, 13u)
#define PPUTLIMPL_UDEC_636u  0x27Cu, 9u, 25u, (2u, 2u, 3u, 53u)
#define PPUTLIMPL_UDEC_635u  0x27Bu, 9u, 25u, (5u, 127u)
#define PPUTLIMPL_UDEC_634u  0x27Au, 9u, 25u, (2u, 317u)
#define PPUTLIMPL_UDEC_633u  0x279u, 9u, 25u, (3u, 211u)
#define PPUTLIMPL_UDEC_632u  0x278u, 9u, 25u, (2u, 2u, 2u, 79u)
#define PPUTLIMPL_UDEC_631u  0x277u, 9u, 25u, ()
#define PPUTLIMPL_UDEC_630u  0x276u, 9u, 25u, (2u, 3u, 3u, 5u, 7u)
#define PPUTLIMPL_UDEC_629u  0x275u, 9u, 25u, (17u, 37u)
#define PPUTLIMPL_UDEC_628u  0x274u, 9u, 25u, (2u, 2u, 157u)
#define PPUTLIMPL_UDEC_627u  0x273u, 9u, 25u, (3u, 11u, 19u)
#define PPUTLIMPL_UDEC_626u  0x272u, 9u, 25u, (2u, 313u)
#define PPUTLIMPL_UDEC_625u  0x271u, 9u, 25u, (5u, 5u, 5u, 5u)
#define PPUTLIMPL_UDEC_624u  0x270u, 9u, 24u, (2u, 2u, 2u, 2u, 3u, 13u)
#define PPUTLIMPL_UDEC_623u  0x26Fu, 9u, 24u, (7u, 89u)
#define PPUTLIMPL_UDEC_622u  0x26Eu, 9u, 24u, (2u, 311u)
#define PPUTLIMPL_UDEC_621u  0x26Du, 9u, 24u, (3u, 3u, 3u, 23u)
#define PPUTLIMPL_UDEC_620u  0x26Cu, 9u, 24u, (2u, 2u, 5u, 31u)
#define PPUTLIMPL_UDEC_619u  0x26Bu, 9u, 24u, ()
#define PPUTLIMPL_UDEC_618u  0x26Au, 9u, 24u, (2u, 3u, 103u)
#define PPUTLIMPL_UDEC_617u  0x269u, 9u, 24u, ()
#define PPUTLIMPL_UDEC_616u  0x268u, 9u, 24u, (2u, 2u, 2u, 7u, 11u)
#define PPUTLIMPL_UDEC_615u  0x267u, 9u, 24u, (3u, 5u, 41u)
#define PPUTLIMPL_UDEC_614u  0x266u, 9u, 24u, (2u, 307u)
#define PPUTLIMPL_UDEC_613u  0x265u, 9u, 24u, ()
#define PPUTLIMPL_UDEC_612u  0x264u, 9u, 24u, (2u, 2u, 3u, 3u, 17u)
#define PPUTLIMPL_UDEC_611u  0x263u, 9u, 24u, (13u, 47u)
#define PPUTLIMPL_UDEC_610u  0x262u, 9u, 24u, (2u, 5u, 61u)
#define PPUTLIMPL_UDEC_609u  0x261u, 9u, 24u, (3u, 7u, 29u)
#define PPUTLIMPL_UDEC_608u  0x260u, 9u, 24u, (2u, 2u, 2u, 2u, 2u, 19u)
#define PPUTLIMPL_UDEC_607u  0x25Fu, 9u, 24u, ()
#define PPUTLIMPL_UDEC_606u  0x25Eu, 9u, 24u, (2u, 3u, 101u)
#define PPUTLIMPL_UDEC_605u  0x25Du, 9u, 24u, (5u, 11u, 11u)
#define PPUTLIMPL_UDEC_604u  0x25Cu, 9u, 24u, (2u, 2u, 151u)
#define PPUTLIMPL_UDEC_603u  0x25Bu, 9u, 24u, (3u, 3u, 67u)
#define PPUTLIMPL_UDEC_602u  0x25Au, 9u, 24u, (2u, 7u, 43u)
#define PPUTLIMPL_UDEC_601u  0x259u, 9u, 24u, ()
#define PPUTLIMPL_UDEC_600u  0x258u, 9u, 24u, (2u, 2u, 2u, 3u, 5u, 5u)
#define PPUTLIMPL_UDEC_599u  0x257u, 9u, 24u, ()
#define PPUTLIMPL_UDEC_598u  0x256u, 9u, 24u, (2u, 13u, 23u)
#define PPUTLIMPL_UDEC_597u  0x255u, 9u, 24u, (3u, 199u)
#define PPUTLIMPL_UDEC_596u  0x254u, 9u, 24u, (2u, 2u, 149u)
#define PPUTLIMPL_UDEC_595u  0x253u, 9u, 24u, (5u, 7u, 17u)
#define PPUTLIMPL_UDEC_594u  0x252u, 9u, 24u, (2u, 3u, 3u, 3u, 11u)
#define PPUTLIMPL_UDEC_593u  0x251u, 9u, 24u, ()
#define PPUTLIMPL_UDEC_592u  0x250u, 9u, 24u, (2u, 2u, 2u, 2u, 37u)
#define PPUTLIMPL_UDEC_591u  0x24Fu, 9u, 24u, (3u, 197u)
#define PPUTLIMPL_UDEC_590u  0x24Eu, 9u, 24u, (2u, 5u, 59u)
#define PPUTLIMPL_UDEC_589u  0x24Du, 9u, 24u, (19u, 31u)
#define PPUTLIMPL_UDEC_588u  0x24Cu, 9u, 24u, (2u, 2u, 3u, 7u, 7u)
#define PPUTLIMPL_UDEC_587u  0x24Bu, 9u, 24u, ()
#define PPUTLIMPL_UDEC_586u  0x24Au, 9u, 24u, (2u, 293u)
#define PPUTLIMPL_UDEC_585u  0x249u, 9u, 24u, (3u, 3u, 5u, 13u)
#define PPUTLIMPL_UDEC_584u  0x248u, 9u, 24u, (2u, 2u, 2u, 73u)
#define PPUTLIMPL_UDEC_583u  0x247u, 9u, 24u, (11u, 53u)
#define PPUTLIMPL_UDEC_582u  0x246u, 9u, 24u, (2u, 3u, 97u)
#define PPUTLIMPL_UDEC_581u  0x245u, 9u, 24u, (7u, 83u)
#define PPUTLIMPL_UDEC_580u  0x244u, 9u, 24u, (2u, 2u, 5u, 29u)
#define PPUTLIMPL_UDEC_579u  0x243u, 9u, 24u, (3u, 193u)
#define PPUTLIMPL_UDEC_578u  0x242u, 9u, 24u, (2u, 17u, 17u)
#define PPUTLIMPL_UDEC_577u  0x241u, 9u, 24u, ()
#define PPUTLIMPL_UDEC_576u  0x240u, 9u, 24u, (2u, 2u, 2u, 2u, 2u, 2u, 3u, 3u)
#define PPUTLIMPL_UDEC_575u  0x23Fu, 9u, 23u, (5u, 5u, 23u)
#define PPUTLIMPL_UDEC_574u  0x23Eu, 9u, 23u, (2u, 7u, 41u)
#define PPUTLIMPL_UDEC_573u  0x23Du, 9u, 23u, (3u, 191u)
#define PPUTLIMPL_UDEC_572u  0x23Cu, 9u, 23u, (2u, 2u, 11u, 13u)
#define PPUTLIMPL_UDEC_571u  0x23Bu, 9u, 23u, ()
#define PPUTLIMPL_UDEC_570u  0x23Au, 9u, 23u, (2u, 3u, 5u, 19u)
#define PPUTLIMPL_UDEC_569u  0x239u, 9u, 23u, ()
#define PPUTLIMPL_UDEC_568u  0x238u, 9u, 23u, (2u, 2u, 2u, 71u)
#define PPUTLIMPL_UDEC_567u  0x237u, 9u, 23u, (3u, 3u, 3u, 3u, 7u)
#define PPUTLIMPL_UDEC_566u  0x236u, 9u, 23u, (2u, 283u)
#define PPUTLIMPL_UDEC_565u  0x235u, 9u, 23u, (5u, 113u)
#define PPUTLIMPL_UDEC_564u  0x234u, 9u, 23u, (2u, 2u, 3u, 47u)
#define PPUTLIMPL_UDEC_563u  0x233u, 9u, 23u, ()
#define PPUTLIMPL_UDEC_562u  0x232u, 9u, 23u, (2u, 281u)
#define PPUTLIMPL_UDEC_561u  0x231u, 9u, 23u, (3u, 11u, 17u)
#define PPUTLIMPL_UDEC_560u  0x230u, 9u, 23u, (2u, 2u, 2u, 2u, 5u, 7u)
#define PPUTLIMPL_UDEC_559u  0x22Fu, 9u, 23u, (13u, 43u)
#define PPUTLIMPL_UDEC_558u  0x22Eu, 9u, 23u, (2u, 3u, 3u, 31u)
#define PPUTLIMPL_UDEC_557u  0x22Du, 9u, 23u, ()
#define PPUTLIMPL_UDEC_556u  0x22Cu, 9u, 23u, (2u, 2u, 139u)
#define PPUTLIMPL_UDEC_555u  0x22Bu, 9u, 23u, (3u, 5u, 37u)
#define PPUTLIMPL_UDEC_554u  0x22Au, 9u, 23u, (2u, 277u)
#define PPUTLIMPL_UDEC_553u  0x229u, 9u, 23u, (7u, 79u)
#define PPUTLIMPL_UDEC_552u  0x228u, 9u, 23u, (2u, 2u, 2u, 3u, 23u)
#define PPUTLIMPL_UDEC_551u  0x227u, 9u, 23u, (19u, 29u)
#define PPUTLIMPL_UDEC_550u  0x226u, 9u, 23u, (2u, 5u, 5u, 11u)
#define PPUTLIMPL_UDEC_549u  0x225u, 9u, 23u, (3u, 3u, 61u)
#define PPUTLIMPL_UDEC_548u  0x224u, 9u, 23u, (2u, 2u, 137u)
#define PPUTLIMPL_UDEC_547u  0x223u, 9u, 23u, ()
#define PPUTLIMPL_UDEC_546u  0x222u, 9u, 23u, (2u, 3u, 7u, 13u)
#define PPUTLIMPL_UDEC_545u  0x221u, 9u, 23u, (5u, 109u)
#define PPUTLIMPL_UDEC_544u  0x220u, 9u, 23u, (2u, 2u, 2u, 2u, 2u, 17u)
#define PPUTLIMPL_UDEC_543u  0x21Fu, 9u, 23u, (3u, 181u)
#define PPUTLIMPL_UDEC_542u  0x21Eu, 9u, 23u, (2u, 271u)
#define PPUTLIMPL_UDEC_541u  0x21Du, 9u, 23u, ()
#define PPUTLIMPL_UDEC_540u  0x21Cu, 9u, 23u, (2u, 2u, 3u, 3u, 3u, 5u)
#define PPUTLIMPL_UDEC_539u  0x21Bu, 9u, 23u, (7u, 7u, 11u)
#define PPUTLIMPL_UDEC_538u  0x21Au, 9u, 23u, (2u, 269u)
#define PPUTLIMPL_UDEC_537u  0x219u, 9u, 23u, (3u, 179u)
#define PPUTLIMPL_UDEC_536u  0x218u, 9u, 23u, (2u, 2u, 2u, 67u)
#define PPUTLIMPL_UDEC_535u  0x217u, 9u, 23u, (5u, 107u)
#define PPUTLIMPL_UDEC_534u  0x216u, 9u, 23u, (2u, 3u, 89u)
#define PPUTLIMPL_UDEC_533u  0x215u, 9u, 23u, (13u, 41u)
#define PPUTLIMPL_UDEC_532u  0x214u, 9u, 23u, (2u, 2u, 7u, 19u)
#define PPUTLIMPL_UDEC_531u  0x213u, 9u, 23u, (3u, 3u, 59u)
#define PPUTLIMPL_UDEC_530u  0x212u, 9u, 23u, (2u, 5u, 53u)
#define PPUTLIMPL_UDEC_529u  0x211u, 9u, 23u, (23u, 23u)
#define PPUTLIMPL_UDEC_528u  0x210u, 9u, 22u, (2u, 2u, 2u, 2u, 3u, 11u)
#define PPUTLIMPL_UDEC_527u  0x20Fu, 9u, 22u, (17u, 31u)
#define PPUTLIMPL_UDEC_526u  0x20Eu, 9u, 22u, (2u, 263u)
#define PPUTLIMPL_UDEC_525u  0x20Du, 9u, 22u, (3u, 5u, 5u, 7u)
#define PPUTLIMPL_UDEC_524u  0x20Cu, 9u, 22u, (2u, 2u, 131u)
#define PPUTLIMPL_UDEC_523u  0x20Bu, 9u, 22u, ()
#define PPUTLIMPL_UDEC_522u  0x20Au, 9u, 22u, (2u, 3u, 3u, 29u)
#define PPUTLIMPL_UDEC_521u  0x209u, 9u, 22u, ()
#define PPUTLIMPL_UDEC_520u  0x208u, 9u, 22u, (2u, 2u, 2u, 5u, 13u)
#define PPUTLIMPL_UDEC_519u  0x207u, 9u, 22u, (3u, 173u)
#define PPUTLIMPL_UDEC_518u  0x206u, 9u, 22u, (2u, 7u, 37u)
#define PPUTLIMPL_UDEC_517u  0x205u, 9u, 22u, (11u, 47u)
#define PPUTLIMPL_UDEC_516u  0x204u, 9u, 22u, (2u, 2u, 3u, 43u)
#define PPUTLIMPL_UDEC_515u  0x203u, 9u, 22u, (5u, 103u)
#define PPUTLIMPL_UDEC_514u  0x202u, 9u, 22u, (2u, 257u)
#define PPUTLIMPL_UDEC_513u  0x201u, 9u, 22u, (3u, 3u, 3u, 19u)
#define PPUTLIMPL_UDEC_512u  0x200u, 9u, 22u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u)
#define PPUTLIMPL_UDEC_511u  0x1FFu, 8u, 22u, (7u, 73u)
#define PPUTLIMPL_UDEC_510u  0x1FEu, 8u, 22u, (2u, 3u, 5u, 17u)
#define PPUTLIMPL_UDEC_509u  0x1FDu, 8u, 22u, ()
#define PPUTLIMPL_UDEC_508u  0x1FCu, 8u, 22u, (2u, 2u, 127u)
#define PPUTLIMPL_UDEC_507u  0x1FBu, 8u, 22u, (3u, 13u, 13u)
#define PPUTLIMPL_UDEC_506u  0x1FAu, 8u, 22u, (2u, 11u, 23u)
#define PPUTLIMPL_UDEC_505u  0x1F9u, 8u, 22u, (5u, 101u)
#define PPUTLIMPL_UDEC_504u  0x1F8u, 8u, 22u, (2u, 2u, 2u, 3u, 3u, 7u)
#define PPUTLIMPL_UDEC_503u  0x1F7u, 8u, 22u, ()
#define PPUTLIMPL_UDEC_502u  0x1F6u, 8u, 22u, (2u, 251u)
#define PPUTLIMPL_UDEC_501u  0x1F5u, 8u, 22u, (3u, 167u)
#define PPUTLIMPL_UDEC_500u  0x1F4u, 8u, 22u, (2u, 2u, 5u, 5u, 5u)
#define PPUTLIMPL_UDEC_499u  0x1F3u, 8u, 22u, ()
#define PPUTLIMPL_UDEC_498u  0x1F2u, 8u, 22u, (2u, 3u, 83u)
#define PPUTLIMPL_UDEC_497u  0x1F1u, 8u, 22u, (7u, 71u)
#define PPUTLIMPL_UDEC_496u  0x1F0u, 8u, 22u, (2u, 2u, 2u, 2u, 31u)
#define PPUTLIMPL_UDEC_495u  0x1EFu, 8u, 22u, (3u, 3u, 5u, 11u)
#define PPUTLIMPL_UDEC_494u  0x1EEu, 8u, 22u, (2u, 13u, 19u)
#define PPUTLIMPL_UDEC_493u  0x1EDu, 8u, 22u, (17u, 29u)
#define PPUTLIMPL_UDEC_492u  0x1ECu, 8u, 22u, (2u, 2u, 3u, 41u)
#define PPUTLIMPL_UDEC_491u  0x1EBu, 8u, 22u, ()
#define PPUTLIMPL_UDEC_490u  0x1EAu, 8u, 22u, (2u, 5u, 7u, 7u)
#define PPUTLIMPL_UDEC_489u  0x1E9u, 8u, 22u, (3u, 163u)
#define PPUTLIMPL_UDEC_488u  0x1E8u, 8u, 22u, (2u, 2u, 2u, 61u)
#define PPUTLIMPL_UDEC_487u  0x1E7u, 8u, 22u, ()
#define PPUTLIMPL_UDEC_486u  0x1E6u, 8u, 22u, (2u, 3u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UDEC_485u  0x1E5u, 8u, 22u, (5u, 97u)
#define PPUTLIMPL_UDEC_484u  0x1E4u, 8u, 22u, (2u, 2u, 11u, 11u)
#define PPUTLIMPL_UDEC_483u  0x1E3u, 8u, 21u, (3u, 7u, 23u)
#define PPUTLIMPL_UDEC_482u  0x1E2u, 8u, 21u, (2u, 241u)
#define PPUTLIMPL_UDEC_481u  0x1E1u, 8u, 21u, (13u, 37u)
#define PPUTLIMPL_UDEC_480u  0x1E0u, 8u, 21u, (2u, 2u, 2u, 2u, 2u, 3u, 5u)
#define PPUTLIMPL_UDEC_479u  0x1DFu, 8u, 21u, ()
#define PPUTLIMPL_UDEC_478u  0x1DEu, 8u, 21u, (2u, 239u)
#define PPUTLIMPL_UDEC_477u  0x1DDu, 8u, 21u, (3u, 3u, 53u)
#define PPUTLIMPL_UDEC_476u  0x1DCu, 8u, 21u, (2u, 2u, 7u, 17u)
#define PPUTLIMPL_UDEC_475u  0x1DBu, 8u, 21u, (5u, 5u, 19u)
#define PPUTLIMPL_UDEC_474u  0x1DAu, 8u, 21u, (2u, 3u, 79u)
#define PPUTLIMPL_UDEC_473u  0x1D9u, 8u, 21u, (11u, 43u)
#define PPUTLIMPL_UDEC_472u  0x1D8u, 8u, 21u, (2u, 2u, 2u, 59u)
#define PPUTLIMPL_UDEC_471u  0x1D7u, 8u, 21u, (3u, 157u)
#define PPUTLIMPL_UDEC_470u  0x1D6u, 8u, 21u, (2u, 5u, 47u)
#define PPUTLIMPL_UDEC_469u  0x1D5u, 8u, 21u, (7u, 67u)
#define PPUTLIMPL_UDEC_468u  0x1D4u, 8u, 21u, (2u, 2u, 3u, 3u, 13u)
#define PPUTLIMPL_UDEC_467u  0x1D3u, 8u, 21u, ()
#define PPUTLIMPL_UDEC_466u  0x1D2u, 8u, 21u, (2u, 233u)
#define PPUTLIMPL_UDEC_465u  0x1D1u, 8u, 21u, (3u, 5u, 31u)
#define PPUTLIMPL_UDEC_464u  0x1D0u, 8u, 21u, (2u, 2u, 2u, 2u, 29u)
#define PPUTLIMPL_UDEC_463u  0x1CFu, 8u, 21u, ()
#define PPUTLIMPL_UDEC_462u  0x1CEu, 8u, 21u, (2u, 3u, 7u, 11u)
#define PPUTLIMPL_UDEC_461u  0x1CDu, 8u, 21u, ()
#define PPUTLIMPL_UDEC_460u  0x1CCu, 8u, 21u, (2u, 2u, 5u, 23u)
#define PPUTLIMPL_UDEC_459u  0x1CBu, 8u, 21u, (3u, 3u, 3u, 17u)
#define PPUTLIMPL_UDEC_458u  0x1CAu, 8u, 21u, (2u, 229u)
#define PPUTLIMPL_UDEC_457u  0x1C9u, 8u, 21u, ()
#define PPUTLIMPL_UDEC_456u  0x1C8u, 8u, 21u, (2u, 2u, 2u, 3u, 19u)
#define PPUTLIMPL_UDEC_455u  0x1C7u, 8u, 21u, (5u, 7u, 13u)
#define PPUTLIMPL_UDEC_454u  0x1C6u, 8u, 21u, (2u, 227u)
#define PPUTLIMPL_UDEC_453u  0x1C5u, 8u, 21u, (3u, 151u)
#define PPUTLIMPL_UDEC_452u  0x1C4u, 8u, 21u, (2u, 2u, 113u)
#define PPUTLIMPL_UDEC_451u  0x1C3u, 8u, 21u, (11u, 41u)
#define PPUTLIMPL_UDEC_450u  0x1C2u, 8u, 21u, (2u, 3u, 3u, 5u, 5u)
#define PPUTLIMPL_UDEC_449u  0x1C1u, 8u, 21u, ()
#define PPUTLIMPL_UDEC_448u  0x1C0u, 8u, 21u, (2u, 2u, 2u, 2u, 2u, 2u, 7u)
#define PPUTLIMPL_UDEC_447u  0x1BFu, 8u, 21u, (3u, 149u)
#define PPUTLIMPL_UDEC_446u  0x1BEu, 8u, 21u, (2u, 223u)
#define PPUTLIMPL_UDEC_445u  0x1BDu, 8u, 21u, (5u, 89u)
#define PPUTLIMPL_UDEC_444u  0x1BCu, 8u, 21u, (2u, 2u, 3u, 37u)
#define PPUTLIMPL_UDEC_443u  0x1BBu, 8u, 21u, ()
#define PPUTLIMPL_UDEC_442u  0x1BAu, 8u, 21u, (2u, 13u, 17u)
#define PPUTLIMPL_UDEC_441u  0x1B9u, 8u, 21u, (3u, 3u, 7u, 7u)
#define PPUTLIMPL_UDEC_440u  0x1B8u, 8u, 20u, (2u, 2u, 2u, 5u, 11u)
#define PPUTLIMPL_UDEC_439u  0x1B7u, 8u, 20u, ()
#define PPUTLIMPL_UDEC_438u  0x1B6u, 8u, 20u, (2u, 3u, 73u)
#define PPUTLIMPL_UDEC_437u  0x1B5u, 8u, 20u, (19u, 23u)
#define PPUTLIMPL_UDEC_436u  0x1B4u, 8u, 20u, (2u, 2u, 109u)
#define PPUTLIMPL_UDEC_435u  0x1B3u, 8u, 20u, (3u, 5u, 29u)
#define PPUTLIMPL_UDEC_434u  0x1B2u, 8u, 20u, (2u, 7u, 31u)
#define PPUTLIMPL_UDEC_433u  0x1B1u, 8u, 20u, ()
#define PPUTLIMPL_UDEC_432u  0x1B0u, 8u, 20u, (2u, 2u, 2u, 2u, 3u, 3u, 3u)
#define PPUTLIMPL_UDEC_431u  0x1AFu, 8u, 20u, ()
#define PPUTLIMPL_UDEC_430u  0x1AEu, 8u, 20u, (2u, 5u, 43u)
#define PPUTLIMPL_UDEC_429u  0x1ADu, 8u, 20u, (3u, 11u, 13u)
#define PPUTLIMPL_UDEC_428u  0x1ACu, 8u, 20u, (2u, 2u, 107u)
#define PPUTLIMPL_UDEC_427u  0x1ABu, 8u, 20u, (7u, 61u)
#define PPUTLIMPL_UDEC_426u  0x1AAu, 8u, 20u, (2u, 3u, 71u)
#define PPUTLIMPL_UDEC_425u  0x1A9u, 8u, 20u, (5u, 5u, 17u)
#define PPUTLIMPL_UDEC_424u  0x1A8u, 8u, 20u, (2u, 2u, 2u, 53u)
#define PPUTLIMPL_UDEC_423u  0x1A7u, 8u, 20u, (3u, 3u, 47u)
#define PPUTLIMPL_UDEC_422u  0x1A6u, 8u, 20u, (2u, 211u)
#define PPUTLIMPL_UDEC_421u  0x1A5u, 8u, 20u, ()
#define PPUTLIMPL_UDEC_420u  0x1A4u, 8u, 20u, (2u, 2u, 3u, 5u, 7u)
#define PPUTLIMPL_UDEC_419u  0x1A3u, 8u, 20u, ()
#define PPUTLIMPL_UDEC_418u  0x1A2u, 8u, 20u, (2u, 11u, 19u)
#define PPUTLIMPL_UDEC_417u  0x1A1u, 8u, 20u, (3u, 139u)
#define PPUTLIMPL_UDEC_416u  0x1A0u, 8u, 20u, (2u, 2u, 2u, 2u, 2u, 13u)
#define PPUTLIMPL_UDEC_415u  0x19Fu, 8u, 20u, (5u, 83u)
#define PPUTLIMPL_UDEC_414u  0x19Eu, 8u, 20u, (2u, 3u, 3u, 23u)
#define PPUTLIMPL_UDEC_413u  0x19Du, 8u, 20u, (7u, 59u)
#define PPUTLIMPL_UDEC_412u  0x19Cu, 8u, 20u, (2u, 2u, 103u)
#define PPUTLIMPL_UDEC_411u  0x19Bu, 8u, 20u, (3u, 137u)
#define PPUTLIMPL_UDEC_410u  0x19Au, 8u, 20u, (2u, 5u, 41u)
#define PPUTLIMPL_UDEC_409u  0x199u, 8u, 20u, ()
#define PPUTLIMPL_UDEC_408u  0x198u, 8u, 20u, (2u, 2u, 2u, 3u, 17u)
#define PPUTLIMPL_UDEC_407u  0x197u, 8u, 20u, (11u, 37u)
#define PPUTLIMPL_UDEC_406u  0x196u, 8u, 20u, (2u, 7u, 29u)
#define PPUTLIMPL_UDEC_405u  0x195u, 8u, 20u, (3u, 3u, 3u, 3u, 5u)
#define PPUTLIMPL_UDEC_404u  0x194u, 8u, 20u, (2u, 2u, 101u)
#define PPUTLIMPL_UDEC_403u  0x193u, 8u, 20u, (13u, 31u)
#define PPUTLIMPL_UDEC_402u  0x192u, 8u, 20u, (2u, 3u, 67u)
#define PPUTLIMPL_UDEC_401u  0x191u, 8u, 20u, ()
#define PPUTLIMPL_UDEC_400u  0x190u, 8u, 20u, (2u, 2u, 2u, 2u, 5u, 5u)
#define PPUTLIMPL_UDEC_399u  0x18Fu, 8u, 19u, (3u, 7u, 19u)
#define PPUTLIMPL_UDEC_398u  0x18Eu, 8u, 19u, (2u, 199u)
#define PPUTLIMPL_UDEC_397u  0x18Du, 8u, 19u, ()
#define PPUTLIMPL_UDEC_396u  0x18Cu, 8u, 19u, (2u, 2u, 3u, 3u, 11u)
#define PPUTLIMPL_UDEC_395u  0x18Bu, 8u, 19u, (5u, 79u)
#define PPUTLIMPL_UDEC_394u  0x18Au, 8u, 19u, (2u, 197u)
#define PPUTLIMPL_UDEC_393u  0x189u, 8u, 19u, (3u, 131u)
#define PPUTLIMPL_UDEC_392u  0x188u, 8u, 19u, (2u, 2u, 2u, 7u, 7u)
#define PPUTLIMPL_UDEC_391u  0x187u, 8u, 19u, (17u, 23u)
#define PPUTLIMPL_UDEC_390u  0x186u, 8u, 19u, (2u, 3u, 5u, 13u)
#define PPUTLIMPL_UDEC_389u  0x185u, 8u, 19u, ()
#define PPUTLIMPL_UDEC_388u  0x184u, 8u, 19u, (2u, 2u, 97u)
#define PPUTLIMPL_UDEC_387u  0x183u, 8u, 19u, (3u, 3u, 43u)
#define PPUTLIMPL_UDEC_386u  0x182u, 8u, 19u, (2u, 193u)
#define PPUTLIMPL_UDEC_385u  0x181u, 8u, 19u, (5u, 7u, 11u)
#define PPUTLIMPL_UDEC_384u  0x180u, 8u, 19u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 3u)
#define PPUTLIMPL_UDEC_383u  0x17Fu, 8u, 19u, ()
#define PPUTLIMPL_UDEC_382u  0x17Eu, 8u, 19u, (2u, 191u)
#define PPUTLIMPL_UDEC_381u  0x17Du, 8u, 19u, (3u, 127u)
#define PPUTLIMPL_UDEC_380u  0x17Cu, 8u, 19u, (2u, 2u, 5u, 19u)
#define PPUTLIMPL_UDEC_379u  0x17Bu, 8u, 19u, ()
#define PPUTLIMPL_UDEC_378u  0x17Au, 8u, 19u, (2u, 3u, 3u, 3u, 7u)
#define PPUTLIMPL_UDEC_377u  0x179u, 8u, 19u, (13u, 29u)
#define PPUTLIMPL_UDEC_376u  0x178u, 8u, 19u, (2u, 2u, 2u, 47u)
#define PPUTLIMPL_UDEC_375u  0x177u, 8u, 19u, (3u, 5u, 5u, 5u)
#define PPUTLIMPL_UDEC_374u  0x176u, 8u, 19u, (2u, 11u, 17u)
#define PPUTLIMPL_UDEC_373u  0x175u, 8u, 19u, ()
#define PPUTLIMPL_UDEC_372u  0x174u, 8u, 19u, (2u, 2u, 3u, 31u)
#define PPUTLIMPL_UDEC_371u  0x173u, 8u, 19u, (7u, 53u)
#define PPUTLIMPL_UDEC_370u  0x172u, 8u, 19u, (2u, 5u, 37u)
#define PPUTLIMPL_UDEC_369u  0x171u, 8u, 19u, (3u, 3u, 41u)
#define PPUTLIMPL_UDEC_368u  0x170u, 8u, 19u, (2u, 2u, 2u, 2u, 23u)
#define PPUTLIMPL_UDEC_367u  0x16Fu, 8u, 19u, ()
#define PPUTLIMPL_UDEC_366u  0x16Eu, 8u, 19u, (2u, 3u, 61u)
#define PPUTLIMPL_UDEC_365u  0x16Du, 8u, 19u, (5u, 73u)
#define PPUTLIMPL_UDEC_364u  0x16Cu, 8u, 19u, (2u, 2u, 7u, 13u)
#define PPUTLIMPL_UDEC_363u  0x16Bu, 8u, 19u, (3u, 11u, 11u)
#define PPUTLIMPL_UDEC_362u  0x16Au, 8u, 19u, (2u, 181u)
#define PPUTLIMPL_UDEC_361u  0x169u, 8u, 19u, (19u, 19u)
#define PPUTLIMPL_UDEC_360u  0x168u, 8u, 18u, (2u, 2u, 2u, 3u, 3u, 5u)
#define PPUTLIMPL_UDEC_359u  0x167u, 8u, 18u, ()
#define PPUTLIMPL_UDEC_358u  0x166u, 8u, 18u, (2u, 179u)
#define PPUTLIMPL_UDEC_357u  0x165u, 8u, 18u, (3u, 7u, 17u)
#define PPUTLIMPL_UDEC_356u  0x164u, 8u, 18u, (2u, 2u, 89u)
#define PPUTLIMPL_UDEC_355u  0x163u, 8u, 18u, (5u, 71u)
#define PPUTLIMPL_UDEC_354u  0x162u, 8u, 18u, (2u, 3u, 59u)
#define PPUTLIMPL_UDEC_353u  0x161u, 8u, 18u, ()
#define PPUTLIMPL_UDEC_352u  0x160u, 8u, 18u, (2u, 2u, 2u, 2u, 2u, 11u)
#define PPUTLIMPL_UDEC_351u  0x15Fu, 8u, 18u, (3u, 3u, 3u, 13u)
#define PPUTLIMPL_UDEC_350u  0x15Eu, 8u, 18u, (2u, 5u, 5u, 7u)
#define PPUTLIMPL_UDEC_349u  0x15Du, 8u, 18u, ()
#define PPUTLIMPL_UDEC_348u  0x15Cu, 8u, 18u, (2u, 2u, 3u, 29u)
#define PPUTLIMPL_UDEC_347u  0x15Bu, 8u, 18u, ()
#define PPUTLIMPL_UDEC_346u  0x15Au, 8u, 18u, (2u, 173u)
#define PPUTLIMPL_UDEC_345u  0x159u, 8u, 18u, (3u, 5u, 23u)
#define PPUTLIMPL_UDEC_344u  0x158u, 8u, 18u, (2u, 2u, 2u, 43u)
#define PPUTLIMPL_UDEC_343u  0x157u, 8u, 18u, (7u, 7u, 7u)
#define PPUTLIMPL_UDEC_342u  0x156u, 8u, 18u, (2u, 3u, 3u, 19u)
#define PPUTLIMPL_UDEC_341u  0x155u, 8u, 18u, (11u, 31u)
#define PPUTLIMPL_UDEC_340u  0x154u, 8u, 18u, (2u, 2u, 5u, 17u)
#define PPUTLIMPL_UDEC_339u  0x153u, 8u, 18u, (3u, 113u)
#define PPUTLIMPL_UDEC_338u  0x152u, 8u, 18u, (2u, 13u, 13u)
#define PPUTLIMPL_UDEC_337u  0x151u, 8u, 18u, ()
#define PPUTLIMPL_UDEC_336u  0x150u, 8u, 18u, (2u, 2u, 2u, 2u, 3u, 7u)
#define PPUTLIMPL_UDEC_335u  0x14Fu, 8u, 18u, (5u, 67u)
#define PPUTLIMPL_UDEC_334u  0x14Eu, 8u, 18u, (2u, 167u)
#define PPUTLIMPL_UDEC_333u  0x14Du, 8u, 18u, (3u, 3u, 37u)
#define PPUTLIMPL_UDEC_332u  0x14Cu, 8u, 18u, (2u, 2u, 83u)
#define PPUTLIMPL_UDEC_331u  0x14Bu, 8u, 18u, ()
#define PPUTLIMPL_UDEC_330u  0x14Au, 8u, 18u, (2u, 3u, 5u, 11u)
#define PPUTLIMPL_UDEC_329u  0x149u, 8u, 18u, (7u, 47u)
#define PPUTLIMPL_UDEC_328u  0x148u, 8u, 18u, (2u, 2u, 2u, 41u)
#define PPUTLIMPL_UDEC_327u  0x147u, 8u, 18u, (3u, 109u)
#define PPUTLIMPL_UDEC_326u  0x146u, 8u, 18u, (2u, 163u)
#define PPUTLIMPL_UDEC_325u  0x145u, 8u, 18u, (5u, 5u, 13u)
#define PPUTLIMPL_UDEC_324u  0x144u, 8u, 18u, (2u, 2u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UDEC_323u  0x143u, 8u, 17u, (17u, 19u)
#define PPUTLIMPL_UDEC_322u  0x142u, 8u, 17u, (2u, 7u, 23u)
#define PPUTLIMPL_UDEC_321u  0x141u, 8u, 17u, (3u, 107u)
#define PPUTLIMPL_UDEC_320u  0x140u, 8u, 17u, (2u, 2u, 2u, 2u, 2u, 2u, 5u)
#define PPUTLIMPL_UDEC_319u  0x13Fu, 8u, 17u, (11u, 29u)
#define PPUTLIMPL_UDEC_318u  0x13Eu, 8u, 17u, (2u, 3u, 53u)
#define PPUTLIMPL_UDEC_317u  0x13Du, 8u, 17u, ()
#define PPUTLIMPL_UDEC_316u  0x13Cu, 8u, 17u, (2u, 2u, 79u)
#define PPUTLIMPL_UDEC_315u  0x13Bu, 8u, 17u, (3u, 3u, 5u, 7u)
#define PPUTLIMPL_UDEC_314u  0x13Au, 8u, 17u, (2u, 157u)
#define PPUTLIMPL_UDEC_313u  0x139u, 8u, 17u, ()
#define PPUTLIMPL_UDEC_312u  0x138u, 8u, 17u, (2u, 2u, 2u, 3u, 13u)
#define PPUTLIMPL_UDEC_311u  0x137u, 8u, 17u, ()
#define PPUTLIMPL_UDEC_310u  0x136u, 8u, 17u, (2u, 5u, 31u)
#define PPUTLIMPL_UDEC_309u  0x135u, 8u, 17u, (3u, 103u)
#define PPUTLIMPL_UDEC_308u  0x134u, 8u, 17u, (2u, 2u, 7u, 11u)
#define PPUTLIMPL_UDEC_307u  0x133u, 8u, 17u, ()
#define PPUTLIMPL_UDEC_306u  0x132u, 8u, 17u, (2u, 3u, 3u, 17u)
#define PPUTLIMPL_UDEC_305u  0x131u, 8u, 17u, (5u, 61u)
#define PPUTLIMPL_UDEC_304u  0x130u, 8u, 17u, (2u, 2u, 2u, 2u, 19u)
#define PPUTLIMPL_UDEC_303u  0x12Fu, 8u, 17u, (3u, 101u)
#define PPUTLIMPL_UDEC_302u  0x12Eu, 8u, 17u, (2u, 151u)
#define PPUTLIMPL_UDEC_301u  0x12Du, 8u, 17u, (7u, 43u)
#define PPUTLIMPL_UDEC_300u  0x12Cu, 8u, 17u, (2u, 2u, 3u, 5u, 5u)
#define PPUTLIMPL_UDEC_299u  0x12Bu, 8u, 17u, (13u, 23u)
#define PPUTLIMPL_UDEC_298u  0x12Au, 8u, 17u, (2u, 149u)
#define PPUTLIMPL_UDEC_297u  0x129u, 8u, 17u, (3u, 3u, 3u, 11u)
#define PPUTLIMPL_UDEC_296u  0x128u, 8u, 17u, (2u, 2u, 2u, 37u)
#define PPUTLIMPL_UDEC_295u  0x127u, 8u, 17u, (5u, 59u)
#define PPUTLIMPL_UDEC_294u  0x126u, 8u, 17u, (2u, 3u, 7u, 7u)
#define PPUTLIMPL_UDEC_293u  0x125u, 8u, 17u, ()
#define PPUTLIMPL_UDEC_292u  0x124u, 8u, 17u, (2u, 2u, 73u)
#define PPUTLIMPL_UDEC_291u  0x123u, 8u, 17u, (3u, 97u)
#define PPUTLIMPL_UDEC_290u  0x122u, 8u, 17u, (2u, 5u, 29u)
#define PPUTLIMPL_UDEC_289u  0x121u, 8u, 17u, (17u, 17u)
#define PPUTLIMPL_UDEC_288u  0x120u, 8u, 16u, (2u, 2u, 2u, 2u, 2u, 3u, 3u)
#define PPUTLIMPL_UDEC_287u  0x11Fu, 8u, 16u, (7u, 41u)
#define PPUTLIMPL_UDEC_286u  0x11Eu, 8u, 16u, (2u, 11u, 13u)
#define PPUTLIMPL_UDEC_285u  0x11Du, 8u, 16u, (3u, 5u, 19u)
#define PPUTLIMPL_UDEC_284u  0x11Cu, 8u, 16u, (2u, 2u, 71u)
#define PPUTLIMPL_UDEC_283u  0x11Bu, 8u, 16u, ()
#define PPUTLIMPL_UDEC_282u  0x11Au, 8u, 16u, (2u, 3u, 47u)
#define PPUTLIMPL_UDEC_281u  0x119u, 8u, 16u, ()
#define PPUTLIMPL_UDEC_280u  0x118u, 8u, 16u, (2u, 2u, 2u, 5u, 7u)
#define PPUTLIMPL_UDEC_279u  0x117u, 8u, 16u, (3u, 3u, 31u)
#define PPUTLIMPL_UDEC_278u  0x116u, 8u, 16u, (2u, 139u)
#define PPUTLIMPL_UDEC_277u  0x115u, 8u, 16u, ()
#define PPUTLIMPL_UDEC_276u  0x114u, 8u, 16u, (2u, 2u, 3u, 23u)
#define PPUTLIMPL_UDEC_275u  0x113u, 8u, 16u, (5u, 5u, 11u)
#define PPUTLIMPL_UDEC_274u  0x112u, 8u, 16u, (2u, 137u)
#define PPUTLIMPL_UDEC_273u  0x111u, 8u, 16u, (3u, 7u, 13u)
#define PPUTLIMPL_UDEC_272u  0x110u, 8u, 16u, (2u, 2u, 2u, 2u, 17u)
#define PPUTLIMPL_UDEC_271u  0x10Fu, 8u, 16u, ()
#define PPUTLIMPL_UDEC_270u  0x10Eu, 8u, 16u, (2u, 3u, 3u, 3u, 5u)
#define PPUTLIMPL_UDEC_269u  0x10Du, 8u, 16u, ()
#define PPUTLIMPL_UDEC_268u  0x10Cu, 8u, 16u, (2u, 2u, 67u)
#define PPUTLIMPL_UDEC_267u  0x10Bu, 8u, 16u, (3u, 89u)
#define PPUTLIMPL_UDEC_266u  0x10Au, 8u, 16u, (2u, 7u, 19u)
#define PPUTLIMPL_UDEC_265u  0x109u, 8u, 16u, (5u, 53u)
#define PPUTLIMPL_UDEC_264u  0x108u, 8u, 16u, (2u, 2u, 2u, 3u, 11u)
#define PPUTLIMPL_UDEC_263u  0x107u, 8u, 16u, ()
#define PPUTLIMPL_UDEC_262u  0x106u, 8u, 16u, (2u, 131u)
#define PPUTLIMPL_UDEC_261u  0x105u, 8u, 16u, (3u, 3u, 29u)
#define PPUTLIMPL_UDEC_260u  0x104u, 8u, 16u, (2u, 2u, 5u, 13u)
#define PPUTLIMPL_UDEC_259u  0x103u, 8u, 16u, (7u, 37u)
#define PPUTLIMPL_UDEC_258u  0x102u, 8u, 16u, (2u, 3u, 43u)
#define PPUTLIMPL_UDEC_257u  0x101u, 8u, 16u, ()
#define PPUTLIMPL_UDEC_256u  0x100u, 8u, 16u, (2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u)
#define PPUTLIMPL_UDEC_255u  0x0FFu, 7u, 15u, (3u, 5u, 17u)
#define PPUTLIMPL_UDEC_254u  0x0FEu, 7u, 15u, (2u, 127u)
#define PPUTLIMPL_UDEC_253u  0x0FDu, 7u, 15u, (11u, 23u)
#define PPUTLIMPL_UDEC_252u  0x0FCu, 7u, 15u, (2u, 2u, 3u, 3u, 7u)
#define PPUTLIMPL_UDEC_251u  0x0FBu, 7u, 15u, ()
#define PPUTLIMPL_UDEC_250u  0x0FAu, 7u, 15u, (2u, 5u, 5u, 5u)
#define PPUTLIMPL_UDEC_249u  0x0F9u, 7u, 15u, (3u, 83u)
#define PPUTLIMPL_UDEC_248u  0x0F8u, 7u, 15u, (2u, 2u, 2u, 31u)
#define PPUTLIMPL_UDEC_247u  0x0F7u, 7u, 15u, (13u, 19u)
#define PPUTLIMPL_UDEC_246u  0x0F6u, 7u, 15u, (2u, 3u, 41u)
#define PPUTLIMPL_UDEC_245u  0x0F5u, 7u, 15u, (5u, 7u, 7u)
#define PPUTLIMPL_UDEC_244u  0x0F4u, 7u, 15u, (2u, 2u, 61u)
#define PPUTLIMPL_UDEC_243u  0x0F3u, 7u, 15u, (3u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UDEC_242u  0x0F2u, 7u, 15u, (2u, 11u, 11u)
#define PPUTLIMPL_UDEC_241u  0x0F1u, 7u, 15u, ()
#define PPUTLIMPL_UDEC_240u  0x0F0u, 7u, 15u, (2u, 2u, 2u, 2u, 3u, 5u)
#define PPUTLIMPL_UDEC_239u  0x0EFu, 7u, 15u, ()
#define PPUTLIMPL_UDEC_238u  0x0EEu, 7u, 15u, (2u, 7u, 17u)
#define PPUTLIMPL_UDEC_237u  0x0EDu, 7u, 15u, (3u, 79u)
#define PPUTLIMPL_UDEC_236u  0x0ECu, 7u, 15u, (2u, 2u, 59u)
#define PPUTLIMPL_UDEC_235u  0x0EBu, 7u, 15u, (5u, 47u)
#define PPUTLIMPL_UDEC_234u  0x0EAu, 7u, 15u, (2u, 3u, 3u, 13u)
#define PPUTLIMPL_UDEC_233u  0x0E9u, 7u, 15u, ()
#define PPUTLIMPL_UDEC_232u  0x0E8u, 7u, 15u, (2u, 2u, 2u, 29u)
#define PPUTLIMPL_UDEC_231u  0x0E7u, 7u, 15u, (3u, 7u, 11u)
#define PPUTLIMPL_UDEC_230u  0x0E6u, 7u, 15u, (2u, 5u, 23u)
#define PPUTLIMPL_UDEC_229u  0x0E5u, 7u, 15u, ()
#define PPUTLIMPL_UDEC_228u  0x0E4u, 7u, 15u, (2u, 2u, 3u, 19u)
#define PPUTLIMPL_UDEC_227u  0x0E3u, 7u, 15u, ()
#define PPUTLIMPL_UDEC_226u  0x0E2u, 7u, 15u, (2u, 113u)
#define PPUTLIMPL_UDEC_225u  0x0E1u, 7u, 15u, (3u, 3u, 5u, 5u)
#define PPUTLIMPL_UDEC_224u  0x0E0u, 7u, 14u, (2u, 2u, 2u, 2u, 2u, 7u)
#define PPUTLIMPL_UDEC_223u  0x0DFu, 7u, 14u, ()
#define PPUTLIMPL_UDEC_222u  0x0DEu, 7u, 14u, (2u, 3u, 37u)
#define PPUTLIMPL_UDEC_221u  0x0DDu, 7u, 14u, (13u, 17u)
#define PPUTLIMPL_UDEC_220u  0x0DCu, 7u, 14u, (2u, 2u, 5u, 11u)
#define PPUTLIMPL_UDEC_219u  0x0DBu, 7u, 14u, (3u, 73u)
#define PPUTLIMPL_UDEC_218u  0x0DAu, 7u, 14u, (2u, 109u)
#define PPUTLIMPL_UDEC_217u  0x0D9u, 7u, 14u, (7u, 31u)
#define PPUTLIMPL_UDEC_216u  0x0D8u, 7u, 14u, (2u, 2u, 2u, 3u, 3u, 3u)
#define PPUTLIMPL_UDEC_215u  0x0D7u, 7u, 14u, (5u, 43u)
#define PPUTLIMPL_UDEC_214u  0x0D6u, 7u, 14u, (2u, 107u)
#define PPUTLIMPL_UDEC_213u  0x0D5u, 7u, 14u, (3u, 71u)
#define PPUTLIMPL_UDEC_212u  0x0D4u, 7u, 14u, (2u, 2u, 53u)
#define PPUTLIMPL_UDEC_211u  0x0D3u, 7u, 14u, ()
#define PPUTLIMPL_UDEC_210u  0x0D2u, 7u, 14u, (2u, 3u, 5u, 7u)
#define PPUTLIMPL_UDEC_209u  0x0D1u, 7u, 14u, (11u, 19u)
#define PPUTLIMPL_UDEC_208u  0x0D0u, 7u, 14u, (2u, 2u, 2u, 2u, 13u)
#define PPUTLIMPL_UDEC_207u  0x0CFu, 7u, 14u, (3u, 3u, 23u)
#define PPUTLIMPL_UDEC_206u  0x0CEu, 7u, 14u, (2u, 103u)
#define PPUTLIMPL_UDEC_205u  0x0CDu, 7u, 14u, (5u, 41u)
#define PPUTLIMPL_UDEC_204u  0x0CCu, 7u, 14u, (2u, 2u, 3u, 17u)
#define PPUTLIMPL_UDEC_203u  0x0CBu, 7u, 14u, (7u, 29u)
#define PPUTLIMPL_UDEC_202u  0x0CAu, 7u, 14u, (2u, 101u)
#define PPUTLIMPL_UDEC_201u  0x0C9u, 7u, 14u, (3u, 67u)
#define PPUTLIMPL_UDEC_200u  0x0C8u, 7u, 14u, (2u, 2u, 2u, 5u, 5u)
#define PPUTLIMPL_UDEC_199u  0x0C7u, 7u, 14u, ()
#define PPUTLIMPL_UDEC_198u  0x0C6u, 7u, 14u, (2u, 3u, 3u, 11u)
#define PPUTLIMPL_UDEC_197u  0x0C5u, 7u, 14u, ()
#define PPUTLIMPL_UDEC_196u  0x0C4u, 7u, 14u, (2u, 2u, 7u, 7u)
#define PPUTLIMPL_UDEC_195u  0x0C3u, 7u, 13u, (3u, 5u, 13u)
#define PPUTLIMPL_UDEC_194u  0x0C2u, 7u, 13u, (2u, 97u)
#define PPUTLIMPL_UDEC_193u  0x0C1u, 7u, 13u, ()
#define PPUTLIMPL_UDEC_192u  0x0C0u, 7u, 13u, (2u, 2u, 2u, 2u, 2u, 2u, 3u)
#define PPUTLIMPL_UDEC_191u  0x0BFu, 7u, 13u, ()
#define PPUTLIMPL_UDEC_190u  0x0BEu, 7u, 13u, (2u, 5u, 19u)
#define PPUTLIMPL_UDEC_189u  0x0BDu, 7u, 13u, (3u, 3u, 3u, 7u)
#define PPUTLIMPL_UDEC_188u  0x0BCu, 7u, 13u, (2u, 2u, 47u)
#define PPUTLIMPL_UDEC_187u  0x0BBu, 7u, 13u, (11u, 17u)
#define PPUTLIMPL_UDEC_186u  0x0BAu, 7u, 13u, (2u, 3u, 31u)
#define PPUTLIMPL_UDEC_185u  0x0B9u, 7u, 13u, (5u, 37u)
#define PPUTLIMPL_UDEC_184u  0x0B8u, 7u, 13u, (2u, 2u, 2u, 23u)
#define PPUTLIMPL_UDEC_183u  0x0B7u, 7u, 13u, (3u, 61u)
#define PPUTLIMPL_UDEC_182u  0x0B6u, 7u, 13u, (2u, 7u, 13u)
#define PPUTLIMPL_UDEC_181u  0x0B5u, 7u, 13u, ()
#define PPUTLIMPL_UDEC_180u  0x0B4u, 7u, 13u, (2u, 2u, 3u, 3u, 5u)
#define PPUTLIMPL_UDEC_179u  0x0B3u, 7u, 13u, ()
#define PPUTLIMPL_UDEC_178u  0x0B2u, 7u, 13u, (2u, 89u)
#define PPUTLIMPL_UDEC_177u  0x0B1u, 7u, 13u, (3u, 59u)
#define PPUTLIMPL_UDEC_176u  0x0B0u, 7u, 13u, (2u, 2u, 2u, 2u, 11u)
#define PPUTLIMPL_UDEC_175u  0x0AFu, 7u, 13u, (5u, 5u, 7u)
#define PPUTLIMPL_UDEC_174u  0x0AEu, 7u, 13u, (2u, 3u, 29u)
#define PPUTLIMPL_UDEC_173u  0x0ADu, 7u, 13u, ()
#define PPUTLIMPL_UDEC_172u  0x0ACu, 7u, 13u, (2u, 2u, 43u)
#define PPUTLIMPL_UDEC_171u  0x0ABu, 7u, 13u, (3u, 3u, 19u)
#define PPUTLIMPL_UDEC_170u  0x0AAu, 7u, 13u, (2u, 5u, 17u)
#define PPUTLIMPL_UDEC_169u  0x0A9u, 7u, 13u, (13u, 13u)
#define PPUTLIMPL_UDEC_168u  0x0A8u, 7u, 12u, (2u, 2u, 2u, 3u, 7u)
#define PPUTLIMPL_UDEC_167u  0x0A7u, 7u, 12u, ()
#define PPUTLIMPL_UDEC_166u  0x0A6u, 7u, 12u, (2u, 83u)
#define PPUTLIMPL_UDEC_165u  0x0A5u, 7u, 12u, (3u, 5u, 11u)
#define PPUTLIMPL_UDEC_164u  0x0A4u, 7u, 12u, (2u, 2u, 41u)
#define PPUTLIMPL_UDEC_163u  0x0A3u, 7u, 12u, ()
#define PPUTLIMPL_UDEC_162u  0x0A2u, 7u, 12u, (2u, 3u, 3u, 3u, 3u)
#define PPUTLIMPL_UDEC_161u  0x0A1u, 7u, 12u, (7u, 23u)
#define PPUTLIMPL_UDEC_160u  0x0A0u, 7u, 12u, (2u, 2u, 2u, 2u, 2u, 5u)
#define PPUTLIMPL_UDEC_159u  0x09Fu, 7u, 12u, (3u, 53u)
#define PPUTLIMPL_UDEC_158u  0x09Eu, 7u, 12u, (2u, 79u)
#define PPUTLIMPL_UDEC_157u  0x09Du, 7u, 12u, ()
#define PPUTLIMPL_UDEC_156u  0x09Cu, 7u, 12u, (2u, 2u, 3u, 13u)
#define PPUTLIMPL_UDEC_155u  0x09Bu, 7u, 12u, (5u, 31u)
#define PPUTLIMPL_UDEC_154u  0x09Au, 7u, 12u, (2u, 7u, 11u)
#define PPUTLIMPL_UDEC_153u  0x099u, 7u, 12u, (3u, 3u, 17u)
#define PPUTLIMPL_UDEC_152u  0x098u, 7u, 12u, (2u, 2u, 2u, 19u)
#define PPUTLIMPL_UDEC_151u  0x097u, 7u, 12u, ()
#define PPUTLIMPL_UDEC_150u  0x096u, 7u, 12u, (2u, 3u, 5u, 5u)
#define PPUTLIMPL_UDEC_149u  0x095u, 7u, 12u, ()
#define PPUTLIMPL_UDEC_148u  0x094u, 7u, 12u, (2u, 2u, 37u)
#define PPUTLIMPL_UDEC_147u  0x093u, 7u, 12u, (3u, 7u, 7u)
#define PPUTLIMPL_UDEC_146u  0x092u, 7u, 12u, (2u, 73u)
#define PPUTLIMPL_UDEC_145u  0x091u, 7u, 12u, (5u, 29u)
#define PPUTLIMPL_UDEC_144u  0x090u, 7u, 12u, (2u, 2u, 2u, 2u, 3u, 3u)
#define PPUTLIMPL_UDEC_143u  0x08Fu, 7u, 11u, (11u, 13u)
#define PPUTLIMPL_UDEC_142u  0x08Eu, 7u, 11u, (2u, 71u)
#define PPUTLIMPL_UDEC_141u  0x08Du, 7u, 11u, (3u, 47u)
#define PPUTLIMPL_UDEC_140u  0x08Cu, 7u, 11u, (2u, 2u, 5u, 7u)
#define PPUTLIMPL_UDEC_139u  0x08Bu, 7u, 11u, ()
#define PPUTLIMPL_UDEC_138u  0x08Au, 7u, 11u, (2u, 3u, 23u)
#define PPUTLIMPL_UDEC_137u  0x089u, 7u, 11u, ()
#define PPUTLIMPL_UDEC_136u  0x088u, 7u, 11u, (2u, 2u, 2u, 17u)
#define PPUTLIMPL_UDEC_135u  0x087u, 7u, 11u, (3u, 3u, 3u, 5u)
#define PPUTLIMPL_UDEC_134u  0x086u, 7u, 11u, (2u, 67u)
#define PPUTLIMPL_UDEC_133u  0x085u, 7u, 11u, (7u, 19u)
#define PPUTLIMPL_UDEC_132u  0x084u, 7u, 11u, (2u, 2u, 3u, 11u)
#define PPUTLIMPL_UDEC_131u  0x083u, 7u, 11u, ()
#define PPUTLIMPL_UDEC_130u  0x082u, 7u, 11u, (2u, 5u, 13u)
#define PPUTLIMPL_UDEC_129u  0x081u, 7u, 11u, (3u, 43u)
#define PPUTLIMPL_UDEC_128u  0x080u, 7u, 11u, (2u, 2u, 2u, 2u, 2u, 2u, 2u)
#define PPUTLIMPL_UDEC_127u  0x07Fu, 6u, 11u, ()
#define PPUTLIMPL_UDEC_126u  0x07Eu, 6u, 11u, (2u, 3u, 3u, 7u)
#define PPUTLIMPL_UDEC_125u  0x07Du, 6u, 11u, (5u, 5u, 5u)
#define PPUTLIMPL_UDEC_124u  0x07Cu, 6u, 11u, (2u, 2u, 31u)
#define PPUTLIMPL_UDEC_123u  0x07Bu, 6u, 11u, (3u, 41u)
#define PPUTLIMPL_UDEC_122u  0x07Au, 6u, 11u, (2u, 61u)
#define PPUTLIMPL_UDEC_121u  0x079u, 6u, 11u, (11u, 11u)
#define PPUTLIMPL_UDEC_120u  0x078u, 6u, 10u, (2u, 2u, 2u, 3u, 5u)
#define PPUTLIMPL_UDEC_119u  0x077u, 6u, 10u, (7u, 17u)
#define PPUTLIMPL_UDEC_118u  0x076u, 6u, 10u, (2u, 59u)
#define PPUTLIMPL_UDEC_117u  0x075u, 6u, 10u, (3u, 3u, 13u)
#define PPUTLIMPL_UDEC_116u  0x074u, 6u, 10u, (2u, 2u, 29u)
#define PPUTLIMPL_UDEC_115u  0x073u, 6u, 10u, (5u, 23u)
#define PPUTLIMPL_UDEC_114u  0x072u, 6u, 10u, (2u, 3u, 19u)
#define PPUTLIMPL_UDEC_113u  0x071u, 6u, 10u, ()
#define PPUTLIMPL_UDEC_112u  0x070u, 6u, 10u, (2u, 2u, 2u, 2u, 7u)
#define PPUTLIMPL_UDEC_111u  0x06Fu, 6u, 10u, (3u, 37u)
#define PPUTLIMPL_UDEC_110u  0x06Eu, 6u, 10u, (2u, 5u, 11u)
#define PPUTLIMPL_UDEC_109u  0x06Du, 6u, 10u, ()
#define PPUTLIMPL_UDEC_108u  0x06Cu, 6u, 10u, (2u, 2u, 3u, 3u, 3u)
#define PPUTLIMPL_UDEC_107u  0x06Bu, 6u, 10u, ()
#define PPUTLIMPL_UDEC_106u  0x06Au, 6u, 10u, (2u, 53u)
#define PPUTLIMPL_UDEC_105u  0x069u, 6u, 10u, (3u, 5u, 7u)
#define PPUTLIMPL_UDEC_104u  0x068u, 6u, 10u, (2u, 2u, 2u, 13u)
#define PPUTLIMPL_UDEC_103u  0x067u, 6u, 10u, ()
#define PPUTLIMPL_UDEC_102u  0x066u, 6u, 10u, (2u, 3u, 17u)
#define PPUTLIMPL_UDEC_101u  0x065u, 6u, 10u, ()
#define PPUTLIMPL_UDEC_100u  0x064u, 6u, 10u, (2u, 2u, 5u, 5u)
#define PPUTLIMPL_UDEC_99u   0x063u, 6u, 9u, (3u, 3u, 11u)
#define PPUTLIMPL_UDEC_98u   0x062u, 6u, 9u, (2u, 7u, 7u)
#define PPUTLIMPL_UDEC_97u   0x061u, 6u, 9u, ()
#define PPUTLIMPL_UDEC_96u   0x060u, 6u, 9u, (2u, 2u, 2u, 2u, 2u, 3u)
#define PPUTLIMPL_UDEC_95u   0x05Fu, 6u, 9u, (5u, 19u)
#define PPUTLIMPL_UDEC_94u   0x05Eu, 6u, 9u, (2u, 47u)
#define PPUTLIMPL_UDEC_93u   0x05Du, 6u, 9u, (3u, 31u)
#define PPUTLIMPL_UDEC_92u   0x05Cu, 6u, 9u, (2u, 2u, 23u)
#define PPUTLIMPL_UDEC_91u   0x05Bu, 6u, 9u, (7u, 13u)
#define PPUTLIMPL_UDEC_90u   0x05Au, 6u, 9u, (2u, 3u, 3u, 5u)
#define PPUTLIMPL_UDEC_89u   0x059u, 6u, 9u, ()
#define PPUTLIMPL_UDEC_88u   0x058u, 6u, 9u, (2u, 2u, 2u, 11u)
#define PPUTLIMPL_UDEC_87u   0x057u, 6u, 9u, (3u, 29u)
#define PPUTLIMPL_UDEC_86u   0x056u, 6u, 9u, (2u, 43u)
#define PPUTLIMPL_UDEC_85u   0x055u, 6u, 9u, (5u, 17u)
#define PPUTLIMPL_UDEC_84u   0x054u, 6u, 9u, (2u, 2u, 3u, 7u)
#define PPUTLIMPL_UDEC_83u   0x053u, 6u, 9u, ()
#define PPUTLIMPL_UDEC_82u   0x052u, 6u, 9u, (2u, 41u)
#define PPUTLIMPL_UDEC_81u   0x051u, 6u, 9u, (3u, 3u, 3u, 3u)
#define PPUTLIMPL_UDEC_80u   0x050u, 6u, 8u, (2u, 2u, 2u, 2u, 5u)
#define PPUTLIMPL_UDEC_79u   0x04Fu, 6u, 8u, ()
#define PPUTLIMPL_UDEC_78u   0x04Eu, 6u, 8u, (2u, 3u, 13u)
#define PPUTLIMPL_UDEC_77u   0x04Du, 6u, 8u, (7u, 11u)
#define PPUTLIMPL_UDEC_76u   0x04Cu, 6u, 8u, (2u, 2u, 19u)
#define PPUTLIMPL_UDEC_75u   0x04Bu, 6u, 8u, (3u, 5u, 5u)
#define PPUTLIMPL_UDEC_74u   0x04Au, 6u, 8u, (2u, 37u)
#define PPUTLIMPL_UDEC_73u   0x049u, 6u, 8u, ()
#define PPUTLIMPL_UDEC_72u   0x048u, 6u, 8u, (2u, 2u, 2u, 3u, 3u)
#define PPUTLIMPL_UDEC_71u   0x047u, 6u, 8u, ()
#define PPUTLIMPL_UDEC_70u   0x046u, 6u, 8u, (2u, 5u, 7u)
#define PPUTLIMPL_UDEC_69u   0x045u, 6u, 8u, (3u, 23u)
#define PPUTLIMPL_UDEC_68u   0x044u, 6u, 8u, (2u, 2u, 17u)
#define PPUTLIMPL_UDEC_67u   0x043u, 6u, 8u, ()
#define PPUTLIMPL_UDEC_66u   0x042u, 6u, 8u, (2u, 3u, 11u)
#define PPUTLIMPL_UDEC_65u   0x041u, 6u, 8u, (5u, 13u)
#define PPUTLIMPL_UDEC_64u   0x040u, 6u, 8u, (2u, 2u, 2u, 2u, 2u, 2u)
#define PPUTLIMPL_UDEC_63u   0x03Fu, 5u, 7u, (3u, 3u, 7u)
#define PPUTLIMPL_UDEC_62u   0x03Eu, 5u, 7u, (2u, 31u)
#define PPUTLIMPL_UDEC_61u   0x03Du, 5u, 7u, ()
#define PPUTLIMPL_UDEC_60u   0x03Cu, 5u, 7u, (2u, 2u, 3u, 5u)
#define PPUTLIMPL_UDEC_59u   0x03Bu, 5u, 7u, ()
#define PPUTLIMPL_UDEC_58u   0x03Au, 5u, 7u, (2u, 29u)
#define PPUTLIMPL_UDEC_57u   0x039u, 5u, 7u, (3u, 19u)
#define PPUTLIMPL_UDEC_56u   0x038u, 5u, 7u, (2u, 2u, 2u, 7u)
#define PPUTLIMPL_UDEC_55u   0x037u, 5u, 7u, (5u, 11u)
#define PPUTLIMPL_UDEC_54u   0x036u, 5u, 7u, (2u, 3u, 3u, 3u)
#define PPUTLIMPL_UDEC_53u   0x035u, 5u, 7u, ()
#define PPUTLIMPL_UDEC_52u   0x034u, 5u, 7u, (2u, 2u, 13u)
#define PPUTLIMPL_UDEC_51u   0x033u, 5u, 7u, (3u, 17u)
#define PPUTLIMPL_UDEC_50u   0x032u, 5u, 7u, (2u, 5u, 5u)
#define PPUTLIMPL_UDEC_49u   0x031u, 5u, 7u, (7u, 7u)
#define PPUTLIMPL_UDEC_48u   0x030u, 5u, 6u, (2u, 2u, 2u, 2u, 3u)
#define PPUTLIMPL_UDEC_47u   0x02Fu, 5u, 6u, ()
#define PPUTLIMPL_UDEC_46u   0x02Eu, 5u, 6u, (2u, 23u)
#define PPUTLIMPL_UDEC_45u   0x02Du, 5u, 6u, (3u, 3u, 5u)
#define PPUTLIMPL_UDEC_44u   0x02Cu, 5u, 6u, (2u, 2u, 11u)
#define PPUTLIMPL_UDEC_43u   0x02Bu, 5u, 6u, ()
#define PPUTLIMPL_UDEC_42u   0x02Au, 5u, 6u, (2u, 3u, 7u)
#define PPUTLIMPL_UDEC_41u   0x029u, 5u, 6u, ()
#define PPUTLIMPL_UDEC_40u   0x028u, 5u, 6u, (2u, 2u, 2u, 5u)
#define PPUTLIMPL_UDEC_39u   0x027u, 5u, 6u, (3u, 13u)
#define PPUTLIMPL_UDEC_38u   0x026u, 5u, 6u, (2u, 19u)
#define PPUTLIMPL_UDEC_37u   0x025u, 5u, 6u, ()
#define PPUTLIMPL_UDEC_36u   0x024u, 5u, 6u, (2u, 2u, 3u, 3u)
#define PPUTLIMPL_UDEC_35u   0x023u, 5u, 5u, (5u, 7u)
#define PPUTLIMPL_UDEC_34u   0x022u, 5u, 5u, (2u, 17u)
#define PPUTLIMPL_UDEC_33u   0x021u, 5u, 5u, (3u, 11u)
#define PPUTLIMPL_UDEC_32u   0x020u, 5u, 5u, (2u, 2u, 2u, 2u, 2u)
#define PPUTLIMPL_UDEC_31u   0x01Fu, 4u, 5u, ()
#define PPUTLIMPL_UDEC_30u   0x01Eu, 4u, 5u, (2u, 3u, 5u)
#define PPUTLIMPL_UDEC_29u   0x01Du, 4u, 5u, ()
#define PPUTLIMPL_UDEC_28u   0x01Cu, 4u, 5u, (2u, 2u, 7u)
#define PPUTLIMPL_UDEC_27u   0x01Bu, 4u, 5u, (3u, 3u, 3u)
#define PPUTLIMPL_UDEC_26u   0x01Au, 4u, 5u, (2u, 13u)
#define PPUTLIMPL_UDEC_25u   0x019u, 4u, 5u, (5u, 5u)
#define PPUTLIMPL_UDEC_24u   0x018u, 4u, 4u, (2u, 2u, 2u, 3u)
#define PPUTLIMPL_UDEC_23u   0x017u, 4u, 4u, ()
#define PPUTLIMPL_UDEC_22u   0x016u, 4u, 4u, (2u, 11u)
#define PPUTLIMPL_UDEC_21u   0x015u, 4u, 4u, (3u, 7u)
#define PPUTLIMPL_UDEC_20u   0x014u, 4u, 4u, (2u, 2u, 5u)
#define PPUTLIMPL_UDEC_19u   0x013u, 4u, 4u, ()
#define PPUTLIMPL_UDEC_18u   0x012u, 4u, 4u, (2u, 3u, 3u)
#define PPUTLIMPL_UDEC_17u   0x011u, 4u, 4u, ()
#define PPUTLIMPL_UDEC_16u   0x010u, 4u, 4u, (2u, 2u, 2u, 2u)
#define PPUTLIMPL_UDEC_15u   0x00Fu, 3u, 3u, (3u, 5u)
#define PPUTLIMPL_UDEC_14u   0x00Eu, 3u, 3u, (2u, 7u)
#define PPUTLIMPL_UDEC_13u   0x00Du, 3u, 3u, ()
#define PPUTLIMPL_UDEC_12u   0x00Cu, 3u, 3u, (2u, 2u, 3u)
#define PPUTLIMPL_UDEC_11u   0x00Bu, 3u, 3u, ()
#define PPUTLIMPL_UDEC_10u   0x00Au, 3u, 3u, (2u, 5u)
#define PPUTLIMPL_UDEC_9u    0x009u, 3u, 3u, (3u, 3u)
#define PPUTLIMPL_UDEC_8u    0x008u, 3u, 2u, (2u, 2u, 2u)
#define PPUTLIMPL_UDEC_7u    0x007u, 2u, 2u, ()
#define PPUTLIMPL_UDEC_6u    0x006u, 2u, 2u, (2u, 3u)
#define PPUTLIMPL_UDEC_5u    0x005u, 2u, 2u, ()
#define PPUTLIMPL_UDEC_4u    0x004u, 2u, 2u, (2u, 2u)
#define PPUTLIMPL_UDEC_3u    0x003u, 1u, 1u, ()
#define PPUTLIMPL_UDEC_2u    0x002u, 1u, 1u, ()
#define PPUTLIMPL_UDEC_1u    0x001u, 0u, 1u, ()
#define PPUTLIMPL_UDEC_0u    0x000u, , 0u, ()

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [impl.traits.uhex]
/// ------------------
/// [internal] uhex traits
#define PPUTLIMPL_UHEX(/* {<atom>, IS}|{<uhex>, UDEC|UTUP|IHEX|ICAST|ILTZ|BNOT} */ v, t) \
  PPUTLIMPL_UHEX_o(t, PTL_XCAT(PPUTLIMPL_UHEX_, v))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_UHEX_o(t, ...)                PPUTLIMPL_UHEX_##t(__VA_ARGS__)
#define PPUTLIMPL_UHEX_BNOT(u, x, h, i, z, b)   /* -> uhex */ b
#define PPUTLIMPL_UHEX_ILTZ(u, x, h, i, z, ...) /* -> bool */ z
#define PPUTLIMPL_UHEX_ICAST(u, x, h, i, ...)   /* -> idec|ihex */ i
#define PPUTLIMPL_UHEX_IHEX(u, x, h, ...)       /* -> ihex */ h
#define PPUTLIMPL_UHEX_UTUP(u, x, ...)          /* -> utup */ x
#define PPUTLIMPL_UHEX_UDEC(u, ...)             /* -> udec */ u

#define PPUTLIMPL_UHEX_IS(_, ...) /* -> bool */ PPUTLIMPL_UHEX_IS_0##__VA_OPT__(1)

#define PPUTLIMPL_UHEX_IS_01 1
#define PPUTLIMPL_UHEX_IS_0  0

/// UDEC, UTUP, IHEX, ICAST, ILTZ, BNOT
#define PPUTLIMPL_UHEX_0xFFFu 4095u, (F, F, F), 0xFFF, 0xFFF, 1, 0x000u
#define PPUTLIMPL_UHEX_0xFFEu 4094u, (F, F, E), 0xFFE, 0xFFE, 1, 0x001u
#define PPUTLIMPL_UHEX_0xFFDu 4093u, (F, F, D), 0xFFD, 0xFFD, 1, 0x002u
#define PPUTLIMPL_UHEX_0xFFCu 4092u, (F, F, C), 0xFFC, 0xFFC, 1, 0x003u
#define PPUTLIMPL_UHEX_0xFFBu 4091u, (F, F, B), 0xFFB, 0xFFB, 1, 0x004u
#define PPUTLIMPL_UHEX_0xFFAu 4090u, (F, F, A), 0xFFA, 0xFFA, 1, 0x005u
#define PPUTLIMPL_UHEX_0xFF9u 4089u, (F, F, 9), 0xFF9, 0xFF9, 1, 0x006u
#define PPUTLIMPL_UHEX_0xFF8u 4088u, (F, F, 8), 0xFF8, 0xFF8, 1, 0x007u
#define PPUTLIMPL_UHEX_0xFF7u 4087u, (F, F, 7), 0xFF7, 0xFF7, 1, 0x008u
#define PPUTLIMPL_UHEX_0xFF6u 4086u, (F, F, 6), 0xFF6, 0xFF6, 1, 0x009u
#define PPUTLIMPL_UHEX_0xFF5u 4085u, (F, F, 5), 0xFF5, 0xFF5, 1, 0x00Au
#define PPUTLIMPL_UHEX_0xFF4u 4084u, (F, F, 4), 0xFF4, 0xFF4, 1, 0x00Bu
#define PPUTLIMPL_UHEX_0xFF3u 4083u, (F, F, 3), 0xFF3, 0xFF3, 1, 0x00Cu
#define PPUTLIMPL_UHEX_0xFF2u 4082u, (F, F, 2), 0xFF2, 0xFF2, 1, 0x00Du
#define PPUTLIMPL_UHEX_0xFF1u 4081u, (F, F, 1), 0xFF1, 0xFF1, 1, 0x00Eu
#define PPUTLIMPL_UHEX_0xFF0u 4080u, (F, F, 0), 0xFF0, 0xFF0, 1, 0x00Fu
#define PPUTLIMPL_UHEX_0xFEFu 4079u, (F, E, F), 0xFEF, 0xFEF, 1, 0x010u
#define PPUTLIMPL_UHEX_0xFEEu 4078u, (F, E, E), 0xFEE, 0xFEE, 1, 0x011u
#define PPUTLIMPL_UHEX_0xFEDu 4077u, (F, E, D), 0xFED, 0xFED, 1, 0x012u
#define PPUTLIMPL_UHEX_0xFECu 4076u, (F, E, C), 0xFEC, 0xFEC, 1, 0x013u
#define PPUTLIMPL_UHEX_0xFEBu 4075u, (F, E, B), 0xFEB, 0xFEB, 1, 0x014u
#define PPUTLIMPL_UHEX_0xFEAu 4074u, (F, E, A), 0xFEA, 0xFEA, 1, 0x015u
#define PPUTLIMPL_UHEX_0xFE9u 4073u, (F, E, 9), 0xFE9, 0xFE9, 1, 0x016u
#define PPUTLIMPL_UHEX_0xFE8u 4072u, (F, E, 8), 0xFE8, 0xFE8, 1, 0x017u
#define PPUTLIMPL_UHEX_0xFE7u 4071u, (F, E, 7), 0xFE7, 0xFE7, 1, 0x018u
#define PPUTLIMPL_UHEX_0xFE6u 4070u, (F, E, 6), 0xFE6, 0xFE6, 1, 0x019u
#define PPUTLIMPL_UHEX_0xFE5u 4069u, (F, E, 5), 0xFE5, 0xFE5, 1, 0x01Au
#define PPUTLIMPL_UHEX_0xFE4u 4068u, (F, E, 4), 0xFE4, 0xFE4, 1, 0x01Bu
#define PPUTLIMPL_UHEX_0xFE3u 4067u, (F, E, 3), 0xFE3, 0xFE3, 1, 0x01Cu
#define PPUTLIMPL_UHEX_0xFE2u 4066u, (F, E, 2), 0xFE2, 0xFE2, 1, 0x01Du
#define PPUTLIMPL_UHEX_0xFE1u 4065u, (F, E, 1), 0xFE1, 0xFE1, 1, 0x01Eu
#define PPUTLIMPL_UHEX_0xFE0u 4064u, (F, E, 0), 0xFE0, 0xFE0, 1, 0x01Fu
#define PPUTLIMPL_UHEX_0xFDFu 4063u, (F, D, F), 0xFDF, 0xFDF, 1, 0x020u
#define PPUTLIMPL_UHEX_0xFDEu 4062u, (F, D, E), 0xFDE, 0xFDE, 1, 0x021u
#define PPUTLIMPL_UHEX_0xFDDu 4061u, (F, D, D), 0xFDD, 0xFDD, 1, 0x022u
#define PPUTLIMPL_UHEX_0xFDCu 4060u, (F, D, C), 0xFDC, 0xFDC, 1, 0x023u
#define PPUTLIMPL_UHEX_0xFDBu 4059u, (F, D, B), 0xFDB, 0xFDB, 1, 0x024u
#define PPUTLIMPL_UHEX_0xFDAu 4058u, (F, D, A), 0xFDA, 0xFDA, 1, 0x025u
#define PPUTLIMPL_UHEX_0xFD9u 4057u, (F, D, 9), 0xFD9, 0xFD9, 1, 0x026u
#define PPUTLIMPL_UHEX_0xFD8u 4056u, (F, D, 8), 0xFD8, 0xFD8, 1, 0x027u
#define PPUTLIMPL_UHEX_0xFD7u 4055u, (F, D, 7), 0xFD7, 0xFD7, 1, 0x028u
#define PPUTLIMPL_UHEX_0xFD6u 4054u, (F, D, 6), 0xFD6, 0xFD6, 1, 0x029u
#define PPUTLIMPL_UHEX_0xFD5u 4053u, (F, D, 5), 0xFD5, 0xFD5, 1, 0x02Au
#define PPUTLIMPL_UHEX_0xFD4u 4052u, (F, D, 4), 0xFD4, 0xFD4, 1, 0x02Bu
#define PPUTLIMPL_UHEX_0xFD3u 4051u, (F, D, 3), 0xFD3, 0xFD3, 1, 0x02Cu
#define PPUTLIMPL_UHEX_0xFD2u 4050u, (F, D, 2), 0xFD2, 0xFD2, 1, 0x02Du
#define PPUTLIMPL_UHEX_0xFD1u 4049u, (F, D, 1), 0xFD1, 0xFD1, 1, 0x02Eu
#define PPUTLIMPL_UHEX_0xFD0u 4048u, (F, D, 0), 0xFD0, 0xFD0, 1, 0x02Fu
#define PPUTLIMPL_UHEX_0xFCFu 4047u, (F, C, F), 0xFCF, 0xFCF, 1, 0x030u
#define PPUTLIMPL_UHEX_0xFCEu 4046u, (F, C, E), 0xFCE, 0xFCE, 1, 0x031u
#define PPUTLIMPL_UHEX_0xFCDu 4045u, (F, C, D), 0xFCD, 0xFCD, 1, 0x032u
#define PPUTLIMPL_UHEX_0xFCCu 4044u, (F, C, C), 0xFCC, 0xFCC, 1, 0x033u
#define PPUTLIMPL_UHEX_0xFCBu 4043u, (F, C, B), 0xFCB, 0xFCB, 1, 0x034u
#define PPUTLIMPL_UHEX_0xFCAu 4042u, (F, C, A), 0xFCA, 0xFCA, 1, 0x035u
#define PPUTLIMPL_UHEX_0xFC9u 4041u, (F, C, 9), 0xFC9, 0xFC9, 1, 0x036u
#define PPUTLIMPL_UHEX_0xFC8u 4040u, (F, C, 8), 0xFC8, 0xFC8, 1, 0x037u
#define PPUTLIMPL_UHEX_0xFC7u 4039u, (F, C, 7), 0xFC7, 0xFC7, 1, 0x038u
#define PPUTLIMPL_UHEX_0xFC6u 4038u, (F, C, 6), 0xFC6, 0xFC6, 1, 0x039u
#define PPUTLIMPL_UHEX_0xFC5u 4037u, (F, C, 5), 0xFC5, 0xFC5, 1, 0x03Au
#define PPUTLIMPL_UHEX_0xFC4u 4036u, (F, C, 4), 0xFC4, 0xFC4, 1, 0x03Bu
#define PPUTLIMPL_UHEX_0xFC3u 4035u, (F, C, 3), 0xFC3, 0xFC3, 1, 0x03Cu
#define PPUTLIMPL_UHEX_0xFC2u 4034u, (F, C, 2), 0xFC2, 0xFC2, 1, 0x03Du
#define PPUTLIMPL_UHEX_0xFC1u 4033u, (F, C, 1), 0xFC1, 0xFC1, 1, 0x03Eu
#define PPUTLIMPL_UHEX_0xFC0u 4032u, (F, C, 0), 0xFC0, 0xFC0, 1, 0x03Fu
#define PPUTLIMPL_UHEX_0xFBFu 4031u, (F, B, F), 0xFBF, 0xFBF, 1, 0x040u
#define PPUTLIMPL_UHEX_0xFBEu 4030u, (F, B, E), 0xFBE, 0xFBE, 1, 0x041u
#define PPUTLIMPL_UHEX_0xFBDu 4029u, (F, B, D), 0xFBD, 0xFBD, 1, 0x042u
#define PPUTLIMPL_UHEX_0xFBCu 4028u, (F, B, C), 0xFBC, 0xFBC, 1, 0x043u
#define PPUTLIMPL_UHEX_0xFBBu 4027u, (F, B, B), 0xFBB, 0xFBB, 1, 0x044u
#define PPUTLIMPL_UHEX_0xFBAu 4026u, (F, B, A), 0xFBA, 0xFBA, 1, 0x045u
#define PPUTLIMPL_UHEX_0xFB9u 4025u, (F, B, 9), 0xFB9, 0xFB9, 1, 0x046u
#define PPUTLIMPL_UHEX_0xFB8u 4024u, (F, B, 8), 0xFB8, 0xFB8, 1, 0x047u
#define PPUTLIMPL_UHEX_0xFB7u 4023u, (F, B, 7), 0xFB7, 0xFB7, 1, 0x048u
#define PPUTLIMPL_UHEX_0xFB6u 4022u, (F, B, 6), 0xFB6, 0xFB6, 1, 0x049u
#define PPUTLIMPL_UHEX_0xFB5u 4021u, (F, B, 5), 0xFB5, 0xFB5, 1, 0x04Au
#define PPUTLIMPL_UHEX_0xFB4u 4020u, (F, B, 4), 0xFB4, 0xFB4, 1, 0x04Bu
#define PPUTLIMPL_UHEX_0xFB3u 4019u, (F, B, 3), 0xFB3, 0xFB3, 1, 0x04Cu
#define PPUTLIMPL_UHEX_0xFB2u 4018u, (F, B, 2), 0xFB2, 0xFB2, 1, 0x04Du
#define PPUTLIMPL_UHEX_0xFB1u 4017u, (F, B, 1), 0xFB1, 0xFB1, 1, 0x04Eu
#define PPUTLIMPL_UHEX_0xFB0u 4016u, (F, B, 0), 0xFB0, 0xFB0, 1, 0x04Fu
#define PPUTLIMPL_UHEX_0xFAFu 4015u, (F, A, F), 0xFAF, 0xFAF, 1, 0x050u
#define PPUTLIMPL_UHEX_0xFAEu 4014u, (F, A, E), 0xFAE, 0xFAE, 1, 0x051u
#define PPUTLIMPL_UHEX_0xFADu 4013u, (F, A, D), 0xFAD, 0xFAD, 1, 0x052u
#define PPUTLIMPL_UHEX_0xFACu 4012u, (F, A, C), 0xFAC, 0xFAC, 1, 0x053u
#define PPUTLIMPL_UHEX_0xFABu 4011u, (F, A, B), 0xFAB, 0xFAB, 1, 0x054u
#define PPUTLIMPL_UHEX_0xFAAu 4010u, (F, A, A), 0xFAA, 0xFAA, 1, 0x055u
#define PPUTLIMPL_UHEX_0xFA9u 4009u, (F, A, 9), 0xFA9, 0xFA9, 1, 0x056u
#define PPUTLIMPL_UHEX_0xFA8u 4008u, (F, A, 8), 0xFA8, 0xFA8, 1, 0x057u
#define PPUTLIMPL_UHEX_0xFA7u 4007u, (F, A, 7), 0xFA7, 0xFA7, 1, 0x058u
#define PPUTLIMPL_UHEX_0xFA6u 4006u, (F, A, 6), 0xFA6, 0xFA6, 1, 0x059u
#define PPUTLIMPL_UHEX_0xFA5u 4005u, (F, A, 5), 0xFA5, 0xFA5, 1, 0x05Au
#define PPUTLIMPL_UHEX_0xFA4u 4004u, (F, A, 4), 0xFA4, 0xFA4, 1, 0x05Bu
#define PPUTLIMPL_UHEX_0xFA3u 4003u, (F, A, 3), 0xFA3, 0xFA3, 1, 0x05Cu
#define PPUTLIMPL_UHEX_0xFA2u 4002u, (F, A, 2), 0xFA2, 0xFA2, 1, 0x05Du
#define PPUTLIMPL_UHEX_0xFA1u 4001u, (F, A, 1), 0xFA1, 0xFA1, 1, 0x05Eu
#define PPUTLIMPL_UHEX_0xFA0u 4000u, (F, A, 0), 0xFA0, 0xFA0, 1, 0x05Fu
#define PPUTLIMPL_UHEX_0xF9Fu 3999u, (F, 9, F), 0xF9F, 0xF9F, 1, 0x060u
#define PPUTLIMPL_UHEX_0xF9Eu 3998u, (F, 9, E), 0xF9E, 0xF9E, 1, 0x061u
#define PPUTLIMPL_UHEX_0xF9Du 3997u, (F, 9, D), 0xF9D, 0xF9D, 1, 0x062u
#define PPUTLIMPL_UHEX_0xF9Cu 3996u, (F, 9, C), 0xF9C, 0xF9C, 1, 0x063u
#define PPUTLIMPL_UHEX_0xF9Bu 3995u, (F, 9, B), 0xF9B, 0xF9B, 1, 0x064u
#define PPUTLIMPL_UHEX_0xF9Au 3994u, (F, 9, A), 0xF9A, 0xF9A, 1, 0x065u
#define PPUTLIMPL_UHEX_0xF99u 3993u, (F, 9, 9), 0xF99, 0xF99, 1, 0x066u
#define PPUTLIMPL_UHEX_0xF98u 3992u, (F, 9, 8), 0xF98, 0xF98, 1, 0x067u
#define PPUTLIMPL_UHEX_0xF97u 3991u, (F, 9, 7), 0xF97, 0xF97, 1, 0x068u
#define PPUTLIMPL_UHEX_0xF96u 3990u, (F, 9, 6), 0xF96, 0xF96, 1, 0x069u
#define PPUTLIMPL_UHEX_0xF95u 3989u, (F, 9, 5), 0xF95, 0xF95, 1, 0x06Au
#define PPUTLIMPL_UHEX_0xF94u 3988u, (F, 9, 4), 0xF94, 0xF94, 1, 0x06Bu
#define PPUTLIMPL_UHEX_0xF93u 3987u, (F, 9, 3), 0xF93, 0xF93, 1, 0x06Cu
#define PPUTLIMPL_UHEX_0xF92u 3986u, (F, 9, 2), 0xF92, 0xF92, 1, 0x06Du
#define PPUTLIMPL_UHEX_0xF91u 3985u, (F, 9, 1), 0xF91, 0xF91, 1, 0x06Eu
#define PPUTLIMPL_UHEX_0xF90u 3984u, (F, 9, 0), 0xF90, 0xF90, 1, 0x06Fu
#define PPUTLIMPL_UHEX_0xF8Fu 3983u, (F, 8, F), 0xF8F, 0xF8F, 1, 0x070u
#define PPUTLIMPL_UHEX_0xF8Eu 3982u, (F, 8, E), 0xF8E, 0xF8E, 1, 0x071u
#define PPUTLIMPL_UHEX_0xF8Du 3981u, (F, 8, D), 0xF8D, 0xF8D, 1, 0x072u
#define PPUTLIMPL_UHEX_0xF8Cu 3980u, (F, 8, C), 0xF8C, 0xF8C, 1, 0x073u
#define PPUTLIMPL_UHEX_0xF8Bu 3979u, (F, 8, B), 0xF8B, 0xF8B, 1, 0x074u
#define PPUTLIMPL_UHEX_0xF8Au 3978u, (F, 8, A), 0xF8A, 0xF8A, 1, 0x075u
#define PPUTLIMPL_UHEX_0xF89u 3977u, (F, 8, 9), 0xF89, 0xF89, 1, 0x076u
#define PPUTLIMPL_UHEX_0xF88u 3976u, (F, 8, 8), 0xF88, 0xF88, 1, 0x077u
#define PPUTLIMPL_UHEX_0xF87u 3975u, (F, 8, 7), 0xF87, 0xF87, 1, 0x078u
#define PPUTLIMPL_UHEX_0xF86u 3974u, (F, 8, 6), 0xF86, 0xF86, 1, 0x079u
#define PPUTLIMPL_UHEX_0xF85u 3973u, (F, 8, 5), 0xF85, 0xF85, 1, 0x07Au
#define PPUTLIMPL_UHEX_0xF84u 3972u, (F, 8, 4), 0xF84, 0xF84, 1, 0x07Bu
#define PPUTLIMPL_UHEX_0xF83u 3971u, (F, 8, 3), 0xF83, 0xF83, 1, 0x07Cu
#define PPUTLIMPL_UHEX_0xF82u 3970u, (F, 8, 2), 0xF82, 0xF82, 1, 0x07Du
#define PPUTLIMPL_UHEX_0xF81u 3969u, (F, 8, 1), 0xF81, 0xF81, 1, 0x07Eu
#define PPUTLIMPL_UHEX_0xF80u 3968u, (F, 8, 0), 0xF80, 0xF80, 1, 0x07Fu
#define PPUTLIMPL_UHEX_0xF7Fu 3967u, (F, 7, F), 0xF7F, 0xF7F, 1, 0x080u
#define PPUTLIMPL_UHEX_0xF7Eu 3966u, (F, 7, E), 0xF7E, 0xF7E, 1, 0x081u
#define PPUTLIMPL_UHEX_0xF7Du 3965u, (F, 7, D), 0xF7D, 0xF7D, 1, 0x082u
#define PPUTLIMPL_UHEX_0xF7Cu 3964u, (F, 7, C), 0xF7C, 0xF7C, 1, 0x083u
#define PPUTLIMPL_UHEX_0xF7Bu 3963u, (F, 7, B), 0xF7B, 0xF7B, 1, 0x084u
#define PPUTLIMPL_UHEX_0xF7Au 3962u, (F, 7, A), 0xF7A, 0xF7A, 1, 0x085u
#define PPUTLIMPL_UHEX_0xF79u 3961u, (F, 7, 9), 0xF79, 0xF79, 1, 0x086u
#define PPUTLIMPL_UHEX_0xF78u 3960u, (F, 7, 8), 0xF78, 0xF78, 1, 0x087u
#define PPUTLIMPL_UHEX_0xF77u 3959u, (F, 7, 7), 0xF77, 0xF77, 1, 0x088u
#define PPUTLIMPL_UHEX_0xF76u 3958u, (F, 7, 6), 0xF76, 0xF76, 1, 0x089u
#define PPUTLIMPL_UHEX_0xF75u 3957u, (F, 7, 5), 0xF75, 0xF75, 1, 0x08Au
#define PPUTLIMPL_UHEX_0xF74u 3956u, (F, 7, 4), 0xF74, 0xF74, 1, 0x08Bu
#define PPUTLIMPL_UHEX_0xF73u 3955u, (F, 7, 3), 0xF73, 0xF73, 1, 0x08Cu
#define PPUTLIMPL_UHEX_0xF72u 3954u, (F, 7, 2), 0xF72, 0xF72, 1, 0x08Du
#define PPUTLIMPL_UHEX_0xF71u 3953u, (F, 7, 1), 0xF71, 0xF71, 1, 0x08Eu
#define PPUTLIMPL_UHEX_0xF70u 3952u, (F, 7, 0), 0xF70, 0xF70, 1, 0x08Fu
#define PPUTLIMPL_UHEX_0xF6Fu 3951u, (F, 6, F), 0xF6F, 0xF6F, 1, 0x090u
#define PPUTLIMPL_UHEX_0xF6Eu 3950u, (F, 6, E), 0xF6E, 0xF6E, 1, 0x091u
#define PPUTLIMPL_UHEX_0xF6Du 3949u, (F, 6, D), 0xF6D, 0xF6D, 1, 0x092u
#define PPUTLIMPL_UHEX_0xF6Cu 3948u, (F, 6, C), 0xF6C, 0xF6C, 1, 0x093u
#define PPUTLIMPL_UHEX_0xF6Bu 3947u, (F, 6, B), 0xF6B, 0xF6B, 1, 0x094u
#define PPUTLIMPL_UHEX_0xF6Au 3946u, (F, 6, A), 0xF6A, 0xF6A, 1, 0x095u
#define PPUTLIMPL_UHEX_0xF69u 3945u, (F, 6, 9), 0xF69, 0xF69, 1, 0x096u
#define PPUTLIMPL_UHEX_0xF68u 3944u, (F, 6, 8), 0xF68, 0xF68, 1, 0x097u
#define PPUTLIMPL_UHEX_0xF67u 3943u, (F, 6, 7), 0xF67, 0xF67, 1, 0x098u
#define PPUTLIMPL_UHEX_0xF66u 3942u, (F, 6, 6), 0xF66, 0xF66, 1, 0x099u
#define PPUTLIMPL_UHEX_0xF65u 3941u, (F, 6, 5), 0xF65, 0xF65, 1, 0x09Au
#define PPUTLIMPL_UHEX_0xF64u 3940u, (F, 6, 4), 0xF64, 0xF64, 1, 0x09Bu
#define PPUTLIMPL_UHEX_0xF63u 3939u, (F, 6, 3), 0xF63, 0xF63, 1, 0x09Cu
#define PPUTLIMPL_UHEX_0xF62u 3938u, (F, 6, 2), 0xF62, 0xF62, 1, 0x09Du
#define PPUTLIMPL_UHEX_0xF61u 3937u, (F, 6, 1), 0xF61, 0xF61, 1, 0x09Eu
#define PPUTLIMPL_UHEX_0xF60u 3936u, (F, 6, 0), 0xF60, 0xF60, 1, 0x09Fu
#define PPUTLIMPL_UHEX_0xF5Fu 3935u, (F, 5, F), 0xF5F, 0xF5F, 1, 0x0A0u
#define PPUTLIMPL_UHEX_0xF5Eu 3934u, (F, 5, E), 0xF5E, 0xF5E, 1, 0x0A1u
#define PPUTLIMPL_UHEX_0xF5Du 3933u, (F, 5, D), 0xF5D, 0xF5D, 1, 0x0A2u
#define PPUTLIMPL_UHEX_0xF5Cu 3932u, (F, 5, C), 0xF5C, 0xF5C, 1, 0x0A3u
#define PPUTLIMPL_UHEX_0xF5Bu 3931u, (F, 5, B), 0xF5B, 0xF5B, 1, 0x0A4u
#define PPUTLIMPL_UHEX_0xF5Au 3930u, (F, 5, A), 0xF5A, 0xF5A, 1, 0x0A5u
#define PPUTLIMPL_UHEX_0xF59u 3929u, (F, 5, 9), 0xF59, 0xF59, 1, 0x0A6u
#define PPUTLIMPL_UHEX_0xF58u 3928u, (F, 5, 8), 0xF58, 0xF58, 1, 0x0A7u
#define PPUTLIMPL_UHEX_0xF57u 3927u, (F, 5, 7), 0xF57, 0xF57, 1, 0x0A8u
#define PPUTLIMPL_UHEX_0xF56u 3926u, (F, 5, 6), 0xF56, 0xF56, 1, 0x0A9u
#define PPUTLIMPL_UHEX_0xF55u 3925u, (F, 5, 5), 0xF55, 0xF55, 1, 0x0AAu
#define PPUTLIMPL_UHEX_0xF54u 3924u, (F, 5, 4), 0xF54, 0xF54, 1, 0x0ABu
#define PPUTLIMPL_UHEX_0xF53u 3923u, (F, 5, 3), 0xF53, 0xF53, 1, 0x0ACu
#define PPUTLIMPL_UHEX_0xF52u 3922u, (F, 5, 2), 0xF52, 0xF52, 1, 0x0ADu
#define PPUTLIMPL_UHEX_0xF51u 3921u, (F, 5, 1), 0xF51, 0xF51, 1, 0x0AEu
#define PPUTLIMPL_UHEX_0xF50u 3920u, (F, 5, 0), 0xF50, 0xF50, 1, 0x0AFu
#define PPUTLIMPL_UHEX_0xF4Fu 3919u, (F, 4, F), 0xF4F, 0xF4F, 1, 0x0B0u
#define PPUTLIMPL_UHEX_0xF4Eu 3918u, (F, 4, E), 0xF4E, 0xF4E, 1, 0x0B1u
#define PPUTLIMPL_UHEX_0xF4Du 3917u, (F, 4, D), 0xF4D, 0xF4D, 1, 0x0B2u
#define PPUTLIMPL_UHEX_0xF4Cu 3916u, (F, 4, C), 0xF4C, 0xF4C, 1, 0x0B3u
#define PPUTLIMPL_UHEX_0xF4Bu 3915u, (F, 4, B), 0xF4B, 0xF4B, 1, 0x0B4u
#define PPUTLIMPL_UHEX_0xF4Au 3914u, (F, 4, A), 0xF4A, 0xF4A, 1, 0x0B5u
#define PPUTLIMPL_UHEX_0xF49u 3913u, (F, 4, 9), 0xF49, 0xF49, 1, 0x0B6u
#define PPUTLIMPL_UHEX_0xF48u 3912u, (F, 4, 8), 0xF48, 0xF48, 1, 0x0B7u
#define PPUTLIMPL_UHEX_0xF47u 3911u, (F, 4, 7), 0xF47, 0xF47, 1, 0x0B8u
#define PPUTLIMPL_UHEX_0xF46u 3910u, (F, 4, 6), 0xF46, 0xF46, 1, 0x0B9u
#define PPUTLIMPL_UHEX_0xF45u 3909u, (F, 4, 5), 0xF45, 0xF45, 1, 0x0BAu
#define PPUTLIMPL_UHEX_0xF44u 3908u, (F, 4, 4), 0xF44, 0xF44, 1, 0x0BBu
#define PPUTLIMPL_UHEX_0xF43u 3907u, (F, 4, 3), 0xF43, 0xF43, 1, 0x0BCu
#define PPUTLIMPL_UHEX_0xF42u 3906u, (F, 4, 2), 0xF42, 0xF42, 1, 0x0BDu
#define PPUTLIMPL_UHEX_0xF41u 3905u, (F, 4, 1), 0xF41, 0xF41, 1, 0x0BEu
#define PPUTLIMPL_UHEX_0xF40u 3904u, (F, 4, 0), 0xF40, 0xF40, 1, 0x0BFu
#define PPUTLIMPL_UHEX_0xF3Fu 3903u, (F, 3, F), 0xF3F, 0xF3F, 1, 0x0C0u
#define PPUTLIMPL_UHEX_0xF3Eu 3902u, (F, 3, E), 0xF3E, 0xF3E, 1, 0x0C1u
#define PPUTLIMPL_UHEX_0xF3Du 3901u, (F, 3, D), 0xF3D, 0xF3D, 1, 0x0C2u
#define PPUTLIMPL_UHEX_0xF3Cu 3900u, (F, 3, C), 0xF3C, 0xF3C, 1, 0x0C3u
#define PPUTLIMPL_UHEX_0xF3Bu 3899u, (F, 3, B), 0xF3B, 0xF3B, 1, 0x0C4u
#define PPUTLIMPL_UHEX_0xF3Au 3898u, (F, 3, A), 0xF3A, 0xF3A, 1, 0x0C5u
#define PPUTLIMPL_UHEX_0xF39u 3897u, (F, 3, 9), 0xF39, 0xF39, 1, 0x0C6u
#define PPUTLIMPL_UHEX_0xF38u 3896u, (F, 3, 8), 0xF38, 0xF38, 1, 0x0C7u
#define PPUTLIMPL_UHEX_0xF37u 3895u, (F, 3, 7), 0xF37, 0xF37, 1, 0x0C8u
#define PPUTLIMPL_UHEX_0xF36u 3894u, (F, 3, 6), 0xF36, 0xF36, 1, 0x0C9u
#define PPUTLIMPL_UHEX_0xF35u 3893u, (F, 3, 5), 0xF35, 0xF35, 1, 0x0CAu
#define PPUTLIMPL_UHEX_0xF34u 3892u, (F, 3, 4), 0xF34, 0xF34, 1, 0x0CBu
#define PPUTLIMPL_UHEX_0xF33u 3891u, (F, 3, 3), 0xF33, 0xF33, 1, 0x0CCu
#define PPUTLIMPL_UHEX_0xF32u 3890u, (F, 3, 2), 0xF32, 0xF32, 1, 0x0CDu
#define PPUTLIMPL_UHEX_0xF31u 3889u, (F, 3, 1), 0xF31, 0xF31, 1, 0x0CEu
#define PPUTLIMPL_UHEX_0xF30u 3888u, (F, 3, 0), 0xF30, 0xF30, 1, 0x0CFu
#define PPUTLIMPL_UHEX_0xF2Fu 3887u, (F, 2, F), 0xF2F, 0xF2F, 1, 0x0D0u
#define PPUTLIMPL_UHEX_0xF2Eu 3886u, (F, 2, E), 0xF2E, 0xF2E, 1, 0x0D1u
#define PPUTLIMPL_UHEX_0xF2Du 3885u, (F, 2, D), 0xF2D, 0xF2D, 1, 0x0D2u
#define PPUTLIMPL_UHEX_0xF2Cu 3884u, (F, 2, C), 0xF2C, 0xF2C, 1, 0x0D3u
#define PPUTLIMPL_UHEX_0xF2Bu 3883u, (F, 2, B), 0xF2B, 0xF2B, 1, 0x0D4u
#define PPUTLIMPL_UHEX_0xF2Au 3882u, (F, 2, A), 0xF2A, 0xF2A, 1, 0x0D5u
#define PPUTLIMPL_UHEX_0xF29u 3881u, (F, 2, 9), 0xF29, 0xF29, 1, 0x0D6u
#define PPUTLIMPL_UHEX_0xF28u 3880u, (F, 2, 8), 0xF28, 0xF28, 1, 0x0D7u
#define PPUTLIMPL_UHEX_0xF27u 3879u, (F, 2, 7), 0xF27, 0xF27, 1, 0x0D8u
#define PPUTLIMPL_UHEX_0xF26u 3878u, (F, 2, 6), 0xF26, 0xF26, 1, 0x0D9u
#define PPUTLIMPL_UHEX_0xF25u 3877u, (F, 2, 5), 0xF25, 0xF25, 1, 0x0DAu
#define PPUTLIMPL_UHEX_0xF24u 3876u, (F, 2, 4), 0xF24, 0xF24, 1, 0x0DBu
#define PPUTLIMPL_UHEX_0xF23u 3875u, (F, 2, 3), 0xF23, 0xF23, 1, 0x0DCu
#define PPUTLIMPL_UHEX_0xF22u 3874u, (F, 2, 2), 0xF22, 0xF22, 1, 0x0DDu
#define PPUTLIMPL_UHEX_0xF21u 3873u, (F, 2, 1), 0xF21, 0xF21, 1, 0x0DEu
#define PPUTLIMPL_UHEX_0xF20u 3872u, (F, 2, 0), 0xF20, 0xF20, 1, 0x0DFu
#define PPUTLIMPL_UHEX_0xF1Fu 3871u, (F, 1, F), 0xF1F, 0xF1F, 1, 0x0E0u
#define PPUTLIMPL_UHEX_0xF1Eu 3870u, (F, 1, E), 0xF1E, 0xF1E, 1, 0x0E1u
#define PPUTLIMPL_UHEX_0xF1Du 3869u, (F, 1, D), 0xF1D, 0xF1D, 1, 0x0E2u
#define PPUTLIMPL_UHEX_0xF1Cu 3868u, (F, 1, C), 0xF1C, 0xF1C, 1, 0x0E3u
#define PPUTLIMPL_UHEX_0xF1Bu 3867u, (F, 1, B), 0xF1B, 0xF1B, 1, 0x0E4u
#define PPUTLIMPL_UHEX_0xF1Au 3866u, (F, 1, A), 0xF1A, 0xF1A, 1, 0x0E5u
#define PPUTLIMPL_UHEX_0xF19u 3865u, (F, 1, 9), 0xF19, 0xF19, 1, 0x0E6u
#define PPUTLIMPL_UHEX_0xF18u 3864u, (F, 1, 8), 0xF18, 0xF18, 1, 0x0E7u
#define PPUTLIMPL_UHEX_0xF17u 3863u, (F, 1, 7), 0xF17, 0xF17, 1, 0x0E8u
#define PPUTLIMPL_UHEX_0xF16u 3862u, (F, 1, 6), 0xF16, 0xF16, 1, 0x0E9u
#define PPUTLIMPL_UHEX_0xF15u 3861u, (F, 1, 5), 0xF15, 0xF15, 1, 0x0EAu
#define PPUTLIMPL_UHEX_0xF14u 3860u, (F, 1, 4), 0xF14, 0xF14, 1, 0x0EBu
#define PPUTLIMPL_UHEX_0xF13u 3859u, (F, 1, 3), 0xF13, 0xF13, 1, 0x0ECu
#define PPUTLIMPL_UHEX_0xF12u 3858u, (F, 1, 2), 0xF12, 0xF12, 1, 0x0EDu
#define PPUTLIMPL_UHEX_0xF11u 3857u, (F, 1, 1), 0xF11, 0xF11, 1, 0x0EEu
#define PPUTLIMPL_UHEX_0xF10u 3856u, (F, 1, 0), 0xF10, 0xF10, 1, 0x0EFu
#define PPUTLIMPL_UHEX_0xF0Fu 3855u, (F, 0, F), 0xF0F, 0xF0F, 1, 0x0F0u
#define PPUTLIMPL_UHEX_0xF0Eu 3854u, (F, 0, E), 0xF0E, 0xF0E, 1, 0x0F1u
#define PPUTLIMPL_UHEX_0xF0Du 3853u, (F, 0, D), 0xF0D, 0xF0D, 1, 0x0F2u
#define PPUTLIMPL_UHEX_0xF0Cu 3852u, (F, 0, C), 0xF0C, 0xF0C, 1, 0x0F3u
#define PPUTLIMPL_UHEX_0xF0Bu 3851u, (F, 0, B), 0xF0B, 0xF0B, 1, 0x0F4u
#define PPUTLIMPL_UHEX_0xF0Au 3850u, (F, 0, A), 0xF0A, 0xF0A, 1, 0x0F5u
#define PPUTLIMPL_UHEX_0xF09u 3849u, (F, 0, 9), 0xF09, 0xF09, 1, 0x0F6u
#define PPUTLIMPL_UHEX_0xF08u 3848u, (F, 0, 8), 0xF08, 0xF08, 1, 0x0F7u
#define PPUTLIMPL_UHEX_0xF07u 3847u, (F, 0, 7), 0xF07, 0xF07, 1, 0x0F8u
#define PPUTLIMPL_UHEX_0xF06u 3846u, (F, 0, 6), 0xF06, 0xF06, 1, 0x0F9u
#define PPUTLIMPL_UHEX_0xF05u 3845u, (F, 0, 5), 0xF05, 0xF05, 1, 0x0FAu
#define PPUTLIMPL_UHEX_0xF04u 3844u, (F, 0, 4), 0xF04, 0xF04, 1, 0x0FBu
#define PPUTLIMPL_UHEX_0xF03u 3843u, (F, 0, 3), 0xF03, 0xF03, 1, 0x0FCu
#define PPUTLIMPL_UHEX_0xF02u 3842u, (F, 0, 2), 0xF02, 0xF02, 1, 0x0FDu
#define PPUTLIMPL_UHEX_0xF01u 3841u, (F, 0, 1), 0xF01, 0xF01, 1, 0x0FEu
#define PPUTLIMPL_UHEX_0xF00u 3840u, (F, 0, 0), 0xF00, 0xF00, 1, 0x0FFu
#define PPUTLIMPL_UHEX_0xEFFu 3839u, (E, F, F), 0xEFF, 0xEFF, 1, 0x100u
#define PPUTLIMPL_UHEX_0xEFEu 3838u, (E, F, E), 0xEFE, 0xEFE, 1, 0x101u
#define PPUTLIMPL_UHEX_0xEFDu 3837u, (E, F, D), 0xEFD, 0xEFD, 1, 0x102u
#define PPUTLIMPL_UHEX_0xEFCu 3836u, (E, F, C), 0xEFC, 0xEFC, 1, 0x103u
#define PPUTLIMPL_UHEX_0xEFBu 3835u, (E, F, B), 0xEFB, 0xEFB, 1, 0x104u
#define PPUTLIMPL_UHEX_0xEFAu 3834u, (E, F, A), 0xEFA, 0xEFA, 1, 0x105u
#define PPUTLIMPL_UHEX_0xEF9u 3833u, (E, F, 9), 0xEF9, 0xEF9, 1, 0x106u
#define PPUTLIMPL_UHEX_0xEF8u 3832u, (E, F, 8), 0xEF8, 0xEF8, 1, 0x107u
#define PPUTLIMPL_UHEX_0xEF7u 3831u, (E, F, 7), 0xEF7, 0xEF7, 1, 0x108u
#define PPUTLIMPL_UHEX_0xEF6u 3830u, (E, F, 6), 0xEF6, 0xEF6, 1, 0x109u
#define PPUTLIMPL_UHEX_0xEF5u 3829u, (E, F, 5), 0xEF5, 0xEF5, 1, 0x10Au
#define PPUTLIMPL_UHEX_0xEF4u 3828u, (E, F, 4), 0xEF4, 0xEF4, 1, 0x10Bu
#define PPUTLIMPL_UHEX_0xEF3u 3827u, (E, F, 3), 0xEF3, 0xEF3, 1, 0x10Cu
#define PPUTLIMPL_UHEX_0xEF2u 3826u, (E, F, 2), 0xEF2, 0xEF2, 1, 0x10Du
#define PPUTLIMPL_UHEX_0xEF1u 3825u, (E, F, 1), 0xEF1, 0xEF1, 1, 0x10Eu
#define PPUTLIMPL_UHEX_0xEF0u 3824u, (E, F, 0), 0xEF0, 0xEF0, 1, 0x10Fu
#define PPUTLIMPL_UHEX_0xEEFu 3823u, (E, E, F), 0xEEF, 0xEEF, 1, 0x110u
#define PPUTLIMPL_UHEX_0xEEEu 3822u, (E, E, E), 0xEEE, 0xEEE, 1, 0x111u
#define PPUTLIMPL_UHEX_0xEEDu 3821u, (E, E, D), 0xEED, 0xEED, 1, 0x112u
#define PPUTLIMPL_UHEX_0xEECu 3820u, (E, E, C), 0xEEC, 0xEEC, 1, 0x113u
#define PPUTLIMPL_UHEX_0xEEBu 3819u, (E, E, B), 0xEEB, 0xEEB, 1, 0x114u
#define PPUTLIMPL_UHEX_0xEEAu 3818u, (E, E, A), 0xEEA, 0xEEA, 1, 0x115u
#define PPUTLIMPL_UHEX_0xEE9u 3817u, (E, E, 9), 0xEE9, 0xEE9, 1, 0x116u
#define PPUTLIMPL_UHEX_0xEE8u 3816u, (E, E, 8), 0xEE8, 0xEE8, 1, 0x117u
#define PPUTLIMPL_UHEX_0xEE7u 3815u, (E, E, 7), 0xEE7, 0xEE7, 1, 0x118u
#define PPUTLIMPL_UHEX_0xEE6u 3814u, (E, E, 6), 0xEE6, 0xEE6, 1, 0x119u
#define PPUTLIMPL_UHEX_0xEE5u 3813u, (E, E, 5), 0xEE5, 0xEE5, 1, 0x11Au
#define PPUTLIMPL_UHEX_0xEE4u 3812u, (E, E, 4), 0xEE4, 0xEE4, 1, 0x11Bu
#define PPUTLIMPL_UHEX_0xEE3u 3811u, (E, E, 3), 0xEE3, 0xEE3, 1, 0x11Cu
#define PPUTLIMPL_UHEX_0xEE2u 3810u, (E, E, 2), 0xEE2, 0xEE2, 1, 0x11Du
#define PPUTLIMPL_UHEX_0xEE1u 3809u, (E, E, 1), 0xEE1, 0xEE1, 1, 0x11Eu
#define PPUTLIMPL_UHEX_0xEE0u 3808u, (E, E, 0), 0xEE0, 0xEE0, 1, 0x11Fu
#define PPUTLIMPL_UHEX_0xEDFu 3807u, (E, D, F), 0xEDF, 0xEDF, 1, 0x120u
#define PPUTLIMPL_UHEX_0xEDEu 3806u, (E, D, E), 0xEDE, 0xEDE, 1, 0x121u
#define PPUTLIMPL_UHEX_0xEDDu 3805u, (E, D, D), 0xEDD, 0xEDD, 1, 0x122u
#define PPUTLIMPL_UHEX_0xEDCu 3804u, (E, D, C), 0xEDC, 0xEDC, 1, 0x123u
#define PPUTLIMPL_UHEX_0xEDBu 3803u, (E, D, B), 0xEDB, 0xEDB, 1, 0x124u
#define PPUTLIMPL_UHEX_0xEDAu 3802u, (E, D, A), 0xEDA, 0xEDA, 1, 0x125u
#define PPUTLIMPL_UHEX_0xED9u 3801u, (E, D, 9), 0xED9, 0xED9, 1, 0x126u
#define PPUTLIMPL_UHEX_0xED8u 3800u, (E, D, 8), 0xED8, 0xED8, 1, 0x127u
#define PPUTLIMPL_UHEX_0xED7u 3799u, (E, D, 7), 0xED7, 0xED7, 1, 0x128u
#define PPUTLIMPL_UHEX_0xED6u 3798u, (E, D, 6), 0xED6, 0xED6, 1, 0x129u
#define PPUTLIMPL_UHEX_0xED5u 3797u, (E, D, 5), 0xED5, 0xED5, 1, 0x12Au
#define PPUTLIMPL_UHEX_0xED4u 3796u, (E, D, 4), 0xED4, 0xED4, 1, 0x12Bu
#define PPUTLIMPL_UHEX_0xED3u 3795u, (E, D, 3), 0xED3, 0xED3, 1, 0x12Cu
#define PPUTLIMPL_UHEX_0xED2u 3794u, (E, D, 2), 0xED2, 0xED2, 1, 0x12Du
#define PPUTLIMPL_UHEX_0xED1u 3793u, (E, D, 1), 0xED1, 0xED1, 1, 0x12Eu
#define PPUTLIMPL_UHEX_0xED0u 3792u, (E, D, 0), 0xED0, 0xED0, 1, 0x12Fu
#define PPUTLIMPL_UHEX_0xECFu 3791u, (E, C, F), 0xECF, 0xECF, 1, 0x130u
#define PPUTLIMPL_UHEX_0xECEu 3790u, (E, C, E), 0xECE, 0xECE, 1, 0x131u
#define PPUTLIMPL_UHEX_0xECDu 3789u, (E, C, D), 0xECD, 0xECD, 1, 0x132u
#define PPUTLIMPL_UHEX_0xECCu 3788u, (E, C, C), 0xECC, 0xECC, 1, 0x133u
#define PPUTLIMPL_UHEX_0xECBu 3787u, (E, C, B), 0xECB, 0xECB, 1, 0x134u
#define PPUTLIMPL_UHEX_0xECAu 3786u, (E, C, A), 0xECA, 0xECA, 1, 0x135u
#define PPUTLIMPL_UHEX_0xEC9u 3785u, (E, C, 9), 0xEC9, 0xEC9, 1, 0x136u
#define PPUTLIMPL_UHEX_0xEC8u 3784u, (E, C, 8), 0xEC8, 0xEC8, 1, 0x137u
#define PPUTLIMPL_UHEX_0xEC7u 3783u, (E, C, 7), 0xEC7, 0xEC7, 1, 0x138u
#define PPUTLIMPL_UHEX_0xEC6u 3782u, (E, C, 6), 0xEC6, 0xEC6, 1, 0x139u
#define PPUTLIMPL_UHEX_0xEC5u 3781u, (E, C, 5), 0xEC5, 0xEC5, 1, 0x13Au
#define PPUTLIMPL_UHEX_0xEC4u 3780u, (E, C, 4), 0xEC4, 0xEC4, 1, 0x13Bu
#define PPUTLIMPL_UHEX_0xEC3u 3779u, (E, C, 3), 0xEC3, 0xEC3, 1, 0x13Cu
#define PPUTLIMPL_UHEX_0xEC2u 3778u, (E, C, 2), 0xEC2, 0xEC2, 1, 0x13Du
#define PPUTLIMPL_UHEX_0xEC1u 3777u, (E, C, 1), 0xEC1, 0xEC1, 1, 0x13Eu
#define PPUTLIMPL_UHEX_0xEC0u 3776u, (E, C, 0), 0xEC0, 0xEC0, 1, 0x13Fu
#define PPUTLIMPL_UHEX_0xEBFu 3775u, (E, B, F), 0xEBF, 0xEBF, 1, 0x140u
#define PPUTLIMPL_UHEX_0xEBEu 3774u, (E, B, E), 0xEBE, 0xEBE, 1, 0x141u
#define PPUTLIMPL_UHEX_0xEBDu 3773u, (E, B, D), 0xEBD, 0xEBD, 1, 0x142u
#define PPUTLIMPL_UHEX_0xEBCu 3772u, (E, B, C), 0xEBC, 0xEBC, 1, 0x143u
#define PPUTLIMPL_UHEX_0xEBBu 3771u, (E, B, B), 0xEBB, 0xEBB, 1, 0x144u
#define PPUTLIMPL_UHEX_0xEBAu 3770u, (E, B, A), 0xEBA, 0xEBA, 1, 0x145u
#define PPUTLIMPL_UHEX_0xEB9u 3769u, (E, B, 9), 0xEB9, 0xEB9, 1, 0x146u
#define PPUTLIMPL_UHEX_0xEB8u 3768u, (E, B, 8), 0xEB8, 0xEB8, 1, 0x147u
#define PPUTLIMPL_UHEX_0xEB7u 3767u, (E, B, 7), 0xEB7, 0xEB7, 1, 0x148u
#define PPUTLIMPL_UHEX_0xEB6u 3766u, (E, B, 6), 0xEB6, 0xEB6, 1, 0x149u
#define PPUTLIMPL_UHEX_0xEB5u 3765u, (E, B, 5), 0xEB5, 0xEB5, 1, 0x14Au
#define PPUTLIMPL_UHEX_0xEB4u 3764u, (E, B, 4), 0xEB4, 0xEB4, 1, 0x14Bu
#define PPUTLIMPL_UHEX_0xEB3u 3763u, (E, B, 3), 0xEB3, 0xEB3, 1, 0x14Cu
#define PPUTLIMPL_UHEX_0xEB2u 3762u, (E, B, 2), 0xEB2, 0xEB2, 1, 0x14Du
#define PPUTLIMPL_UHEX_0xEB1u 3761u, (E, B, 1), 0xEB1, 0xEB1, 1, 0x14Eu
#define PPUTLIMPL_UHEX_0xEB0u 3760u, (E, B, 0), 0xEB0, 0xEB0, 1, 0x14Fu
#define PPUTLIMPL_UHEX_0xEAFu 3759u, (E, A, F), 0xEAF, 0xEAF, 1, 0x150u
#define PPUTLIMPL_UHEX_0xEAEu 3758u, (E, A, E), 0xEAE, 0xEAE, 1, 0x151u
#define PPUTLIMPL_UHEX_0xEADu 3757u, (E, A, D), 0xEAD, 0xEAD, 1, 0x152u
#define PPUTLIMPL_UHEX_0xEACu 3756u, (E, A, C), 0xEAC, 0xEAC, 1, 0x153u
#define PPUTLIMPL_UHEX_0xEABu 3755u, (E, A, B), 0xEAB, 0xEAB, 1, 0x154u
#define PPUTLIMPL_UHEX_0xEAAu 3754u, (E, A, A), 0xEAA, 0xEAA, 1, 0x155u
#define PPUTLIMPL_UHEX_0xEA9u 3753u, (E, A, 9), 0xEA9, 0xEA9, 1, 0x156u
#define PPUTLIMPL_UHEX_0xEA8u 3752u, (E, A, 8), 0xEA8, 0xEA8, 1, 0x157u
#define PPUTLIMPL_UHEX_0xEA7u 3751u, (E, A, 7), 0xEA7, 0xEA7, 1, 0x158u
#define PPUTLIMPL_UHEX_0xEA6u 3750u, (E, A, 6), 0xEA6, 0xEA6, 1, 0x159u
#define PPUTLIMPL_UHEX_0xEA5u 3749u, (E, A, 5), 0xEA5, 0xEA5, 1, 0x15Au
#define PPUTLIMPL_UHEX_0xEA4u 3748u, (E, A, 4), 0xEA4, 0xEA4, 1, 0x15Bu
#define PPUTLIMPL_UHEX_0xEA3u 3747u, (E, A, 3), 0xEA3, 0xEA3, 1, 0x15Cu
#define PPUTLIMPL_UHEX_0xEA2u 3746u, (E, A, 2), 0xEA2, 0xEA2, 1, 0x15Du
#define PPUTLIMPL_UHEX_0xEA1u 3745u, (E, A, 1), 0xEA1, 0xEA1, 1, 0x15Eu
#define PPUTLIMPL_UHEX_0xEA0u 3744u, (E, A, 0), 0xEA0, 0xEA0, 1, 0x15Fu
#define PPUTLIMPL_UHEX_0xE9Fu 3743u, (E, 9, F), 0xE9F, 0xE9F, 1, 0x160u
#define PPUTLIMPL_UHEX_0xE9Eu 3742u, (E, 9, E), 0xE9E, 0xE9E, 1, 0x161u
#define PPUTLIMPL_UHEX_0xE9Du 3741u, (E, 9, D), 0xE9D, 0xE9D, 1, 0x162u
#define PPUTLIMPL_UHEX_0xE9Cu 3740u, (E, 9, C), 0xE9C, 0xE9C, 1, 0x163u
#define PPUTLIMPL_UHEX_0xE9Bu 3739u, (E, 9, B), 0xE9B, 0xE9B, 1, 0x164u
#define PPUTLIMPL_UHEX_0xE9Au 3738u, (E, 9, A), 0xE9A, 0xE9A, 1, 0x165u
#define PPUTLIMPL_UHEX_0xE99u 3737u, (E, 9, 9), 0xE99, 0xE99, 1, 0x166u
#define PPUTLIMPL_UHEX_0xE98u 3736u, (E, 9, 8), 0xE98, 0xE98, 1, 0x167u
#define PPUTLIMPL_UHEX_0xE97u 3735u, (E, 9, 7), 0xE97, 0xE97, 1, 0x168u
#define PPUTLIMPL_UHEX_0xE96u 3734u, (E, 9, 6), 0xE96, 0xE96, 1, 0x169u
#define PPUTLIMPL_UHEX_0xE95u 3733u, (E, 9, 5), 0xE95, 0xE95, 1, 0x16Au
#define PPUTLIMPL_UHEX_0xE94u 3732u, (E, 9, 4), 0xE94, 0xE94, 1, 0x16Bu
#define PPUTLIMPL_UHEX_0xE93u 3731u, (E, 9, 3), 0xE93, 0xE93, 1, 0x16Cu
#define PPUTLIMPL_UHEX_0xE92u 3730u, (E, 9, 2), 0xE92, 0xE92, 1, 0x16Du
#define PPUTLIMPL_UHEX_0xE91u 3729u, (E, 9, 1), 0xE91, 0xE91, 1, 0x16Eu
#define PPUTLIMPL_UHEX_0xE90u 3728u, (E, 9, 0), 0xE90, 0xE90, 1, 0x16Fu
#define PPUTLIMPL_UHEX_0xE8Fu 3727u, (E, 8, F), 0xE8F, 0xE8F, 1, 0x170u
#define PPUTLIMPL_UHEX_0xE8Eu 3726u, (E, 8, E), 0xE8E, 0xE8E, 1, 0x171u
#define PPUTLIMPL_UHEX_0xE8Du 3725u, (E, 8, D), 0xE8D, 0xE8D, 1, 0x172u
#define PPUTLIMPL_UHEX_0xE8Cu 3724u, (E, 8, C), 0xE8C, 0xE8C, 1, 0x173u
#define PPUTLIMPL_UHEX_0xE8Bu 3723u, (E, 8, B), 0xE8B, 0xE8B, 1, 0x174u
#define PPUTLIMPL_UHEX_0xE8Au 3722u, (E, 8, A), 0xE8A, 0xE8A, 1, 0x175u
#define PPUTLIMPL_UHEX_0xE89u 3721u, (E, 8, 9), 0xE89, 0xE89, 1, 0x176u
#define PPUTLIMPL_UHEX_0xE88u 3720u, (E, 8, 8), 0xE88, 0xE88, 1, 0x177u
#define PPUTLIMPL_UHEX_0xE87u 3719u, (E, 8, 7), 0xE87, 0xE87, 1, 0x178u
#define PPUTLIMPL_UHEX_0xE86u 3718u, (E, 8, 6), 0xE86, 0xE86, 1, 0x179u
#define PPUTLIMPL_UHEX_0xE85u 3717u, (E, 8, 5), 0xE85, 0xE85, 1, 0x17Au
#define PPUTLIMPL_UHEX_0xE84u 3716u, (E, 8, 4), 0xE84, 0xE84, 1, 0x17Bu
#define PPUTLIMPL_UHEX_0xE83u 3715u, (E, 8, 3), 0xE83, 0xE83, 1, 0x17Cu
#define PPUTLIMPL_UHEX_0xE82u 3714u, (E, 8, 2), 0xE82, 0xE82, 1, 0x17Du
#define PPUTLIMPL_UHEX_0xE81u 3713u, (E, 8, 1), 0xE81, 0xE81, 1, 0x17Eu
#define PPUTLIMPL_UHEX_0xE80u 3712u, (E, 8, 0), 0xE80, 0xE80, 1, 0x17Fu
#define PPUTLIMPL_UHEX_0xE7Fu 3711u, (E, 7, F), 0xE7F, 0xE7F, 1, 0x180u
#define PPUTLIMPL_UHEX_0xE7Eu 3710u, (E, 7, E), 0xE7E, 0xE7E, 1, 0x181u
#define PPUTLIMPL_UHEX_0xE7Du 3709u, (E, 7, D), 0xE7D, 0xE7D, 1, 0x182u
#define PPUTLIMPL_UHEX_0xE7Cu 3708u, (E, 7, C), 0xE7C, 0xE7C, 1, 0x183u
#define PPUTLIMPL_UHEX_0xE7Bu 3707u, (E, 7, B), 0xE7B, 0xE7B, 1, 0x184u
#define PPUTLIMPL_UHEX_0xE7Au 3706u, (E, 7, A), 0xE7A, 0xE7A, 1, 0x185u
#define PPUTLIMPL_UHEX_0xE79u 3705u, (E, 7, 9), 0xE79, 0xE79, 1, 0x186u
#define PPUTLIMPL_UHEX_0xE78u 3704u, (E, 7, 8), 0xE78, 0xE78, 1, 0x187u
#define PPUTLIMPL_UHEX_0xE77u 3703u, (E, 7, 7), 0xE77, 0xE77, 1, 0x188u
#define PPUTLIMPL_UHEX_0xE76u 3702u, (E, 7, 6), 0xE76, 0xE76, 1, 0x189u
#define PPUTLIMPL_UHEX_0xE75u 3701u, (E, 7, 5), 0xE75, 0xE75, 1, 0x18Au
#define PPUTLIMPL_UHEX_0xE74u 3700u, (E, 7, 4), 0xE74, 0xE74, 1, 0x18Bu
#define PPUTLIMPL_UHEX_0xE73u 3699u, (E, 7, 3), 0xE73, 0xE73, 1, 0x18Cu
#define PPUTLIMPL_UHEX_0xE72u 3698u, (E, 7, 2), 0xE72, 0xE72, 1, 0x18Du
#define PPUTLIMPL_UHEX_0xE71u 3697u, (E, 7, 1), 0xE71, 0xE71, 1, 0x18Eu
#define PPUTLIMPL_UHEX_0xE70u 3696u, (E, 7, 0), 0xE70, 0xE70, 1, 0x18Fu
#define PPUTLIMPL_UHEX_0xE6Fu 3695u, (E, 6, F), 0xE6F, 0xE6F, 1, 0x190u
#define PPUTLIMPL_UHEX_0xE6Eu 3694u, (E, 6, E), 0xE6E, 0xE6E, 1, 0x191u
#define PPUTLIMPL_UHEX_0xE6Du 3693u, (E, 6, D), 0xE6D, 0xE6D, 1, 0x192u
#define PPUTLIMPL_UHEX_0xE6Cu 3692u, (E, 6, C), 0xE6C, 0xE6C, 1, 0x193u
#define PPUTLIMPL_UHEX_0xE6Bu 3691u, (E, 6, B), 0xE6B, 0xE6B, 1, 0x194u
#define PPUTLIMPL_UHEX_0xE6Au 3690u, (E, 6, A), 0xE6A, 0xE6A, 1, 0x195u
#define PPUTLIMPL_UHEX_0xE69u 3689u, (E, 6, 9), 0xE69, 0xE69, 1, 0x196u
#define PPUTLIMPL_UHEX_0xE68u 3688u, (E, 6, 8), 0xE68, 0xE68, 1, 0x197u
#define PPUTLIMPL_UHEX_0xE67u 3687u, (E, 6, 7), 0xE67, 0xE67, 1, 0x198u
#define PPUTLIMPL_UHEX_0xE66u 3686u, (E, 6, 6), 0xE66, 0xE66, 1, 0x199u
#define PPUTLIMPL_UHEX_0xE65u 3685u, (E, 6, 5), 0xE65, 0xE65, 1, 0x19Au
#define PPUTLIMPL_UHEX_0xE64u 3684u, (E, 6, 4), 0xE64, 0xE64, 1, 0x19Bu
#define PPUTLIMPL_UHEX_0xE63u 3683u, (E, 6, 3), 0xE63, 0xE63, 1, 0x19Cu
#define PPUTLIMPL_UHEX_0xE62u 3682u, (E, 6, 2), 0xE62, 0xE62, 1, 0x19Du
#define PPUTLIMPL_UHEX_0xE61u 3681u, (E, 6, 1), 0xE61, 0xE61, 1, 0x19Eu
#define PPUTLIMPL_UHEX_0xE60u 3680u, (E, 6, 0), 0xE60, 0xE60, 1, 0x19Fu
#define PPUTLIMPL_UHEX_0xE5Fu 3679u, (E, 5, F), 0xE5F, 0xE5F, 1, 0x1A0u
#define PPUTLIMPL_UHEX_0xE5Eu 3678u, (E, 5, E), 0xE5E, 0xE5E, 1, 0x1A1u
#define PPUTLIMPL_UHEX_0xE5Du 3677u, (E, 5, D), 0xE5D, 0xE5D, 1, 0x1A2u
#define PPUTLIMPL_UHEX_0xE5Cu 3676u, (E, 5, C), 0xE5C, 0xE5C, 1, 0x1A3u
#define PPUTLIMPL_UHEX_0xE5Bu 3675u, (E, 5, B), 0xE5B, 0xE5B, 1, 0x1A4u
#define PPUTLIMPL_UHEX_0xE5Au 3674u, (E, 5, A), 0xE5A, 0xE5A, 1, 0x1A5u
#define PPUTLIMPL_UHEX_0xE59u 3673u, (E, 5, 9), 0xE59, 0xE59, 1, 0x1A6u
#define PPUTLIMPL_UHEX_0xE58u 3672u, (E, 5, 8), 0xE58, 0xE58, 1, 0x1A7u
#define PPUTLIMPL_UHEX_0xE57u 3671u, (E, 5, 7), 0xE57, 0xE57, 1, 0x1A8u
#define PPUTLIMPL_UHEX_0xE56u 3670u, (E, 5, 6), 0xE56, 0xE56, 1, 0x1A9u
#define PPUTLIMPL_UHEX_0xE55u 3669u, (E, 5, 5), 0xE55, 0xE55, 1, 0x1AAu
#define PPUTLIMPL_UHEX_0xE54u 3668u, (E, 5, 4), 0xE54, 0xE54, 1, 0x1ABu
#define PPUTLIMPL_UHEX_0xE53u 3667u, (E, 5, 3), 0xE53, 0xE53, 1, 0x1ACu
#define PPUTLIMPL_UHEX_0xE52u 3666u, (E, 5, 2), 0xE52, 0xE52, 1, 0x1ADu
#define PPUTLIMPL_UHEX_0xE51u 3665u, (E, 5, 1), 0xE51, 0xE51, 1, 0x1AEu
#define PPUTLIMPL_UHEX_0xE50u 3664u, (E, 5, 0), 0xE50, 0xE50, 1, 0x1AFu
#define PPUTLIMPL_UHEX_0xE4Fu 3663u, (E, 4, F), 0xE4F, 0xE4F, 1, 0x1B0u
#define PPUTLIMPL_UHEX_0xE4Eu 3662u, (E, 4, E), 0xE4E, 0xE4E, 1, 0x1B1u
#define PPUTLIMPL_UHEX_0xE4Du 3661u, (E, 4, D), 0xE4D, 0xE4D, 1, 0x1B2u
#define PPUTLIMPL_UHEX_0xE4Cu 3660u, (E, 4, C), 0xE4C, 0xE4C, 1, 0x1B3u
#define PPUTLIMPL_UHEX_0xE4Bu 3659u, (E, 4, B), 0xE4B, 0xE4B, 1, 0x1B4u
#define PPUTLIMPL_UHEX_0xE4Au 3658u, (E, 4, A), 0xE4A, 0xE4A, 1, 0x1B5u
#define PPUTLIMPL_UHEX_0xE49u 3657u, (E, 4, 9), 0xE49, 0xE49, 1, 0x1B6u
#define PPUTLIMPL_UHEX_0xE48u 3656u, (E, 4, 8), 0xE48, 0xE48, 1, 0x1B7u
#define PPUTLIMPL_UHEX_0xE47u 3655u, (E, 4, 7), 0xE47, 0xE47, 1, 0x1B8u
#define PPUTLIMPL_UHEX_0xE46u 3654u, (E, 4, 6), 0xE46, 0xE46, 1, 0x1B9u
#define PPUTLIMPL_UHEX_0xE45u 3653u, (E, 4, 5), 0xE45, 0xE45, 1, 0x1BAu
#define PPUTLIMPL_UHEX_0xE44u 3652u, (E, 4, 4), 0xE44, 0xE44, 1, 0x1BBu
#define PPUTLIMPL_UHEX_0xE43u 3651u, (E, 4, 3), 0xE43, 0xE43, 1, 0x1BCu
#define PPUTLIMPL_UHEX_0xE42u 3650u, (E, 4, 2), 0xE42, 0xE42, 1, 0x1BDu
#define PPUTLIMPL_UHEX_0xE41u 3649u, (E, 4, 1), 0xE41, 0xE41, 1, 0x1BEu
#define PPUTLIMPL_UHEX_0xE40u 3648u, (E, 4, 0), 0xE40, 0xE40, 1, 0x1BFu
#define PPUTLIMPL_UHEX_0xE3Fu 3647u, (E, 3, F), 0xE3F, 0xE3F, 1, 0x1C0u
#define PPUTLIMPL_UHEX_0xE3Eu 3646u, (E, 3, E), 0xE3E, 0xE3E, 1, 0x1C1u
#define PPUTLIMPL_UHEX_0xE3Du 3645u, (E, 3, D), 0xE3D, 0xE3D, 1, 0x1C2u
#define PPUTLIMPL_UHEX_0xE3Cu 3644u, (E, 3, C), 0xE3C, 0xE3C, 1, 0x1C3u
#define PPUTLIMPL_UHEX_0xE3Bu 3643u, (E, 3, B), 0xE3B, 0xE3B, 1, 0x1C4u
#define PPUTLIMPL_UHEX_0xE3Au 3642u, (E, 3, A), 0xE3A, 0xE3A, 1, 0x1C5u
#define PPUTLIMPL_UHEX_0xE39u 3641u, (E, 3, 9), 0xE39, 0xE39, 1, 0x1C6u
#define PPUTLIMPL_UHEX_0xE38u 3640u, (E, 3, 8), 0xE38, 0xE38, 1, 0x1C7u
#define PPUTLIMPL_UHEX_0xE37u 3639u, (E, 3, 7), 0xE37, 0xE37, 1, 0x1C8u
#define PPUTLIMPL_UHEX_0xE36u 3638u, (E, 3, 6), 0xE36, 0xE36, 1, 0x1C9u
#define PPUTLIMPL_UHEX_0xE35u 3637u, (E, 3, 5), 0xE35, 0xE35, 1, 0x1CAu
#define PPUTLIMPL_UHEX_0xE34u 3636u, (E, 3, 4), 0xE34, 0xE34, 1, 0x1CBu
#define PPUTLIMPL_UHEX_0xE33u 3635u, (E, 3, 3), 0xE33, 0xE33, 1, 0x1CCu
#define PPUTLIMPL_UHEX_0xE32u 3634u, (E, 3, 2), 0xE32, 0xE32, 1, 0x1CDu
#define PPUTLIMPL_UHEX_0xE31u 3633u, (E, 3, 1), 0xE31, 0xE31, 1, 0x1CEu
#define PPUTLIMPL_UHEX_0xE30u 3632u, (E, 3, 0), 0xE30, 0xE30, 1, 0x1CFu
#define PPUTLIMPL_UHEX_0xE2Fu 3631u, (E, 2, F), 0xE2F, 0xE2F, 1, 0x1D0u
#define PPUTLIMPL_UHEX_0xE2Eu 3630u, (E, 2, E), 0xE2E, 0xE2E, 1, 0x1D1u
#define PPUTLIMPL_UHEX_0xE2Du 3629u, (E, 2, D), 0xE2D, 0xE2D, 1, 0x1D2u
#define PPUTLIMPL_UHEX_0xE2Cu 3628u, (E, 2, C), 0xE2C, 0xE2C, 1, 0x1D3u
#define PPUTLIMPL_UHEX_0xE2Bu 3627u, (E, 2, B), 0xE2B, 0xE2B, 1, 0x1D4u
#define PPUTLIMPL_UHEX_0xE2Au 3626u, (E, 2, A), 0xE2A, 0xE2A, 1, 0x1D5u
#define PPUTLIMPL_UHEX_0xE29u 3625u, (E, 2, 9), 0xE29, 0xE29, 1, 0x1D6u
#define PPUTLIMPL_UHEX_0xE28u 3624u, (E, 2, 8), 0xE28, 0xE28, 1, 0x1D7u
#define PPUTLIMPL_UHEX_0xE27u 3623u, (E, 2, 7), 0xE27, 0xE27, 1, 0x1D8u
#define PPUTLIMPL_UHEX_0xE26u 3622u, (E, 2, 6), 0xE26, 0xE26, 1, 0x1D9u
#define PPUTLIMPL_UHEX_0xE25u 3621u, (E, 2, 5), 0xE25, 0xE25, 1, 0x1DAu
#define PPUTLIMPL_UHEX_0xE24u 3620u, (E, 2, 4), 0xE24, 0xE24, 1, 0x1DBu
#define PPUTLIMPL_UHEX_0xE23u 3619u, (E, 2, 3), 0xE23, 0xE23, 1, 0x1DCu
#define PPUTLIMPL_UHEX_0xE22u 3618u, (E, 2, 2), 0xE22, 0xE22, 1, 0x1DDu
#define PPUTLIMPL_UHEX_0xE21u 3617u, (E, 2, 1), 0xE21, 0xE21, 1, 0x1DEu
#define PPUTLIMPL_UHEX_0xE20u 3616u, (E, 2, 0), 0xE20, 0xE20, 1, 0x1DFu
#define PPUTLIMPL_UHEX_0xE1Fu 3615u, (E, 1, F), 0xE1F, 0xE1F, 1, 0x1E0u
#define PPUTLIMPL_UHEX_0xE1Eu 3614u, (E, 1, E), 0xE1E, 0xE1E, 1, 0x1E1u
#define PPUTLIMPL_UHEX_0xE1Du 3613u, (E, 1, D), 0xE1D, 0xE1D, 1, 0x1E2u
#define PPUTLIMPL_UHEX_0xE1Cu 3612u, (E, 1, C), 0xE1C, 0xE1C, 1, 0x1E3u
#define PPUTLIMPL_UHEX_0xE1Bu 3611u, (E, 1, B), 0xE1B, 0xE1B, 1, 0x1E4u
#define PPUTLIMPL_UHEX_0xE1Au 3610u, (E, 1, A), 0xE1A, 0xE1A, 1, 0x1E5u
#define PPUTLIMPL_UHEX_0xE19u 3609u, (E, 1, 9), 0xE19, 0xE19, 1, 0x1E6u
#define PPUTLIMPL_UHEX_0xE18u 3608u, (E, 1, 8), 0xE18, 0xE18, 1, 0x1E7u
#define PPUTLIMPL_UHEX_0xE17u 3607u, (E, 1, 7), 0xE17, 0xE17, 1, 0x1E8u
#define PPUTLIMPL_UHEX_0xE16u 3606u, (E, 1, 6), 0xE16, 0xE16, 1, 0x1E9u
#define PPUTLIMPL_UHEX_0xE15u 3605u, (E, 1, 5), 0xE15, 0xE15, 1, 0x1EAu
#define PPUTLIMPL_UHEX_0xE14u 3604u, (E, 1, 4), 0xE14, 0xE14, 1, 0x1EBu
#define PPUTLIMPL_UHEX_0xE13u 3603u, (E, 1, 3), 0xE13, 0xE13, 1, 0x1ECu
#define PPUTLIMPL_UHEX_0xE12u 3602u, (E, 1, 2), 0xE12, 0xE12, 1, 0x1EDu
#define PPUTLIMPL_UHEX_0xE11u 3601u, (E, 1, 1), 0xE11, 0xE11, 1, 0x1EEu
#define PPUTLIMPL_UHEX_0xE10u 3600u, (E, 1, 0), 0xE10, 0xE10, 1, 0x1EFu
#define PPUTLIMPL_UHEX_0xE0Fu 3599u, (E, 0, F), 0xE0F, 0xE0F, 1, 0x1F0u
#define PPUTLIMPL_UHEX_0xE0Eu 3598u, (E, 0, E), 0xE0E, 0xE0E, 1, 0x1F1u
#define PPUTLIMPL_UHEX_0xE0Du 3597u, (E, 0, D), 0xE0D, 0xE0D, 1, 0x1F2u
#define PPUTLIMPL_UHEX_0xE0Cu 3596u, (E, 0, C), 0xE0C, 0xE0C, 1, 0x1F3u
#define PPUTLIMPL_UHEX_0xE0Bu 3595u, (E, 0, B), 0xE0B, 0xE0B, 1, 0x1F4u
#define PPUTLIMPL_UHEX_0xE0Au 3594u, (E, 0, A), 0xE0A, 0xE0A, 1, 0x1F5u
#define PPUTLIMPL_UHEX_0xE09u 3593u, (E, 0, 9), 0xE09, 0xE09, 1, 0x1F6u
#define PPUTLIMPL_UHEX_0xE08u 3592u, (E, 0, 8), 0xE08, 0xE08, 1, 0x1F7u
#define PPUTLIMPL_UHEX_0xE07u 3591u, (E, 0, 7), 0xE07, 0xE07, 1, 0x1F8u
#define PPUTLIMPL_UHEX_0xE06u 3590u, (E, 0, 6), 0xE06, 0xE06, 1, 0x1F9u
#define PPUTLIMPL_UHEX_0xE05u 3589u, (E, 0, 5), 0xE05, 0xE05, 1, 0x1FAu
#define PPUTLIMPL_UHEX_0xE04u 3588u, (E, 0, 4), 0xE04, 0xE04, 1, 0x1FBu
#define PPUTLIMPL_UHEX_0xE03u 3587u, (E, 0, 3), 0xE03, 0xE03, 1, 0x1FCu
#define PPUTLIMPL_UHEX_0xE02u 3586u, (E, 0, 2), 0xE02, 0xE02, 1, 0x1FDu
#define PPUTLIMPL_UHEX_0xE01u 3585u, (E, 0, 1), 0xE01, 0xE01, 1, 0x1FEu
#define PPUTLIMPL_UHEX_0xE00u 3584u, (E, 0, 0), 0xE00, 0xE00, 1, 0x1FFu
#define PPUTLIMPL_UHEX_0xDFFu 3583u, (D, F, F), 0xDFF, 0xDFF, 1, 0x200u
#define PPUTLIMPL_UHEX_0xDFEu 3582u, (D, F, E), 0xDFE, 0xDFE, 1, 0x201u
#define PPUTLIMPL_UHEX_0xDFDu 3581u, (D, F, D), 0xDFD, 0xDFD, 1, 0x202u
#define PPUTLIMPL_UHEX_0xDFCu 3580u, (D, F, C), 0xDFC, 0xDFC, 1, 0x203u
#define PPUTLIMPL_UHEX_0xDFBu 3579u, (D, F, B), 0xDFB, 0xDFB, 1, 0x204u
#define PPUTLIMPL_UHEX_0xDFAu 3578u, (D, F, A), 0xDFA, 0xDFA, 1, 0x205u
#define PPUTLIMPL_UHEX_0xDF9u 3577u, (D, F, 9), 0xDF9, 0xDF9, 1, 0x206u
#define PPUTLIMPL_UHEX_0xDF8u 3576u, (D, F, 8), 0xDF8, 0xDF8, 1, 0x207u
#define PPUTLIMPL_UHEX_0xDF7u 3575u, (D, F, 7), 0xDF7, 0xDF7, 1, 0x208u
#define PPUTLIMPL_UHEX_0xDF6u 3574u, (D, F, 6), 0xDF6, 0xDF6, 1, 0x209u
#define PPUTLIMPL_UHEX_0xDF5u 3573u, (D, F, 5), 0xDF5, 0xDF5, 1, 0x20Au
#define PPUTLIMPL_UHEX_0xDF4u 3572u, (D, F, 4), 0xDF4, 0xDF4, 1, 0x20Bu
#define PPUTLIMPL_UHEX_0xDF3u 3571u, (D, F, 3), 0xDF3, 0xDF3, 1, 0x20Cu
#define PPUTLIMPL_UHEX_0xDF2u 3570u, (D, F, 2), 0xDF2, 0xDF2, 1, 0x20Du
#define PPUTLIMPL_UHEX_0xDF1u 3569u, (D, F, 1), 0xDF1, 0xDF1, 1, 0x20Eu
#define PPUTLIMPL_UHEX_0xDF0u 3568u, (D, F, 0), 0xDF0, 0xDF0, 1, 0x20Fu
#define PPUTLIMPL_UHEX_0xDEFu 3567u, (D, E, F), 0xDEF, 0xDEF, 1, 0x210u
#define PPUTLIMPL_UHEX_0xDEEu 3566u, (D, E, E), 0xDEE, 0xDEE, 1, 0x211u
#define PPUTLIMPL_UHEX_0xDEDu 3565u, (D, E, D), 0xDED, 0xDED, 1, 0x212u
#define PPUTLIMPL_UHEX_0xDECu 3564u, (D, E, C), 0xDEC, 0xDEC, 1, 0x213u
#define PPUTLIMPL_UHEX_0xDEBu 3563u, (D, E, B), 0xDEB, 0xDEB, 1, 0x214u
#define PPUTLIMPL_UHEX_0xDEAu 3562u, (D, E, A), 0xDEA, 0xDEA, 1, 0x215u
#define PPUTLIMPL_UHEX_0xDE9u 3561u, (D, E, 9), 0xDE9, 0xDE9, 1, 0x216u
#define PPUTLIMPL_UHEX_0xDE8u 3560u, (D, E, 8), 0xDE8, 0xDE8, 1, 0x217u
#define PPUTLIMPL_UHEX_0xDE7u 3559u, (D, E, 7), 0xDE7, 0xDE7, 1, 0x218u
#define PPUTLIMPL_UHEX_0xDE6u 3558u, (D, E, 6), 0xDE6, 0xDE6, 1, 0x219u
#define PPUTLIMPL_UHEX_0xDE5u 3557u, (D, E, 5), 0xDE5, 0xDE5, 1, 0x21Au
#define PPUTLIMPL_UHEX_0xDE4u 3556u, (D, E, 4), 0xDE4, 0xDE4, 1, 0x21Bu
#define PPUTLIMPL_UHEX_0xDE3u 3555u, (D, E, 3), 0xDE3, 0xDE3, 1, 0x21Cu
#define PPUTLIMPL_UHEX_0xDE2u 3554u, (D, E, 2), 0xDE2, 0xDE2, 1, 0x21Du
#define PPUTLIMPL_UHEX_0xDE1u 3553u, (D, E, 1), 0xDE1, 0xDE1, 1, 0x21Eu
#define PPUTLIMPL_UHEX_0xDE0u 3552u, (D, E, 0), 0xDE0, 0xDE0, 1, 0x21Fu
#define PPUTLIMPL_UHEX_0xDDFu 3551u, (D, D, F), 0xDDF, 0xDDF, 1, 0x220u
#define PPUTLIMPL_UHEX_0xDDEu 3550u, (D, D, E), 0xDDE, 0xDDE, 1, 0x221u
#define PPUTLIMPL_UHEX_0xDDDu 3549u, (D, D, D), 0xDDD, 0xDDD, 1, 0x222u
#define PPUTLIMPL_UHEX_0xDDCu 3548u, (D, D, C), 0xDDC, 0xDDC, 1, 0x223u
#define PPUTLIMPL_UHEX_0xDDBu 3547u, (D, D, B), 0xDDB, 0xDDB, 1, 0x224u
#define PPUTLIMPL_UHEX_0xDDAu 3546u, (D, D, A), 0xDDA, 0xDDA, 1, 0x225u
#define PPUTLIMPL_UHEX_0xDD9u 3545u, (D, D, 9), 0xDD9, 0xDD9, 1, 0x226u
#define PPUTLIMPL_UHEX_0xDD8u 3544u, (D, D, 8), 0xDD8, 0xDD8, 1, 0x227u
#define PPUTLIMPL_UHEX_0xDD7u 3543u, (D, D, 7), 0xDD7, 0xDD7, 1, 0x228u
#define PPUTLIMPL_UHEX_0xDD6u 3542u, (D, D, 6), 0xDD6, 0xDD6, 1, 0x229u
#define PPUTLIMPL_UHEX_0xDD5u 3541u, (D, D, 5), 0xDD5, 0xDD5, 1, 0x22Au
#define PPUTLIMPL_UHEX_0xDD4u 3540u, (D, D, 4), 0xDD4, 0xDD4, 1, 0x22Bu
#define PPUTLIMPL_UHEX_0xDD3u 3539u, (D, D, 3), 0xDD3, 0xDD3, 1, 0x22Cu
#define PPUTLIMPL_UHEX_0xDD2u 3538u, (D, D, 2), 0xDD2, 0xDD2, 1, 0x22Du
#define PPUTLIMPL_UHEX_0xDD1u 3537u, (D, D, 1), 0xDD1, 0xDD1, 1, 0x22Eu
#define PPUTLIMPL_UHEX_0xDD0u 3536u, (D, D, 0), 0xDD0, 0xDD0, 1, 0x22Fu
#define PPUTLIMPL_UHEX_0xDCFu 3535u, (D, C, F), 0xDCF, 0xDCF, 1, 0x230u
#define PPUTLIMPL_UHEX_0xDCEu 3534u, (D, C, E), 0xDCE, 0xDCE, 1, 0x231u
#define PPUTLIMPL_UHEX_0xDCDu 3533u, (D, C, D), 0xDCD, 0xDCD, 1, 0x232u
#define PPUTLIMPL_UHEX_0xDCCu 3532u, (D, C, C), 0xDCC, 0xDCC, 1, 0x233u
#define PPUTLIMPL_UHEX_0xDCBu 3531u, (D, C, B), 0xDCB, 0xDCB, 1, 0x234u
#define PPUTLIMPL_UHEX_0xDCAu 3530u, (D, C, A), 0xDCA, 0xDCA, 1, 0x235u
#define PPUTLIMPL_UHEX_0xDC9u 3529u, (D, C, 9), 0xDC9, 0xDC9, 1, 0x236u
#define PPUTLIMPL_UHEX_0xDC8u 3528u, (D, C, 8), 0xDC8, 0xDC8, 1, 0x237u
#define PPUTLIMPL_UHEX_0xDC7u 3527u, (D, C, 7), 0xDC7, 0xDC7, 1, 0x238u
#define PPUTLIMPL_UHEX_0xDC6u 3526u, (D, C, 6), 0xDC6, 0xDC6, 1, 0x239u
#define PPUTLIMPL_UHEX_0xDC5u 3525u, (D, C, 5), 0xDC5, 0xDC5, 1, 0x23Au
#define PPUTLIMPL_UHEX_0xDC4u 3524u, (D, C, 4), 0xDC4, 0xDC4, 1, 0x23Bu
#define PPUTLIMPL_UHEX_0xDC3u 3523u, (D, C, 3), 0xDC3, 0xDC3, 1, 0x23Cu
#define PPUTLIMPL_UHEX_0xDC2u 3522u, (D, C, 2), 0xDC2, 0xDC2, 1, 0x23Du
#define PPUTLIMPL_UHEX_0xDC1u 3521u, (D, C, 1), 0xDC1, 0xDC1, 1, 0x23Eu
#define PPUTLIMPL_UHEX_0xDC0u 3520u, (D, C, 0), 0xDC0, 0xDC0, 1, 0x23Fu
#define PPUTLIMPL_UHEX_0xDBFu 3519u, (D, B, F), 0xDBF, 0xDBF, 1, 0x240u
#define PPUTLIMPL_UHEX_0xDBEu 3518u, (D, B, E), 0xDBE, 0xDBE, 1, 0x241u
#define PPUTLIMPL_UHEX_0xDBDu 3517u, (D, B, D), 0xDBD, 0xDBD, 1, 0x242u
#define PPUTLIMPL_UHEX_0xDBCu 3516u, (D, B, C), 0xDBC, 0xDBC, 1, 0x243u
#define PPUTLIMPL_UHEX_0xDBBu 3515u, (D, B, B), 0xDBB, 0xDBB, 1, 0x244u
#define PPUTLIMPL_UHEX_0xDBAu 3514u, (D, B, A), 0xDBA, 0xDBA, 1, 0x245u
#define PPUTLIMPL_UHEX_0xDB9u 3513u, (D, B, 9), 0xDB9, 0xDB9, 1, 0x246u
#define PPUTLIMPL_UHEX_0xDB8u 3512u, (D, B, 8), 0xDB8, 0xDB8, 1, 0x247u
#define PPUTLIMPL_UHEX_0xDB7u 3511u, (D, B, 7), 0xDB7, 0xDB7, 1, 0x248u
#define PPUTLIMPL_UHEX_0xDB6u 3510u, (D, B, 6), 0xDB6, 0xDB6, 1, 0x249u
#define PPUTLIMPL_UHEX_0xDB5u 3509u, (D, B, 5), 0xDB5, 0xDB5, 1, 0x24Au
#define PPUTLIMPL_UHEX_0xDB4u 3508u, (D, B, 4), 0xDB4, 0xDB4, 1, 0x24Bu
#define PPUTLIMPL_UHEX_0xDB3u 3507u, (D, B, 3), 0xDB3, 0xDB3, 1, 0x24Cu
#define PPUTLIMPL_UHEX_0xDB2u 3506u, (D, B, 2), 0xDB2, 0xDB2, 1, 0x24Du
#define PPUTLIMPL_UHEX_0xDB1u 3505u, (D, B, 1), 0xDB1, 0xDB1, 1, 0x24Eu
#define PPUTLIMPL_UHEX_0xDB0u 3504u, (D, B, 0), 0xDB0, 0xDB0, 1, 0x24Fu
#define PPUTLIMPL_UHEX_0xDAFu 3503u, (D, A, F), 0xDAF, 0xDAF, 1, 0x250u
#define PPUTLIMPL_UHEX_0xDAEu 3502u, (D, A, E), 0xDAE, 0xDAE, 1, 0x251u
#define PPUTLIMPL_UHEX_0xDADu 3501u, (D, A, D), 0xDAD, 0xDAD, 1, 0x252u
#define PPUTLIMPL_UHEX_0xDACu 3500u, (D, A, C), 0xDAC, 0xDAC, 1, 0x253u
#define PPUTLIMPL_UHEX_0xDABu 3499u, (D, A, B), 0xDAB, 0xDAB, 1, 0x254u
#define PPUTLIMPL_UHEX_0xDAAu 3498u, (D, A, A), 0xDAA, 0xDAA, 1, 0x255u
#define PPUTLIMPL_UHEX_0xDA9u 3497u, (D, A, 9), 0xDA9, 0xDA9, 1, 0x256u
#define PPUTLIMPL_UHEX_0xDA8u 3496u, (D, A, 8), 0xDA8, 0xDA8, 1, 0x257u
#define PPUTLIMPL_UHEX_0xDA7u 3495u, (D, A, 7), 0xDA7, 0xDA7, 1, 0x258u
#define PPUTLIMPL_UHEX_0xDA6u 3494u, (D, A, 6), 0xDA6, 0xDA6, 1, 0x259u
#define PPUTLIMPL_UHEX_0xDA5u 3493u, (D, A, 5), 0xDA5, 0xDA5, 1, 0x25Au
#define PPUTLIMPL_UHEX_0xDA4u 3492u, (D, A, 4), 0xDA4, 0xDA4, 1, 0x25Bu
#define PPUTLIMPL_UHEX_0xDA3u 3491u, (D, A, 3), 0xDA3, 0xDA3, 1, 0x25Cu
#define PPUTLIMPL_UHEX_0xDA2u 3490u, (D, A, 2), 0xDA2, 0xDA2, 1, 0x25Du
#define PPUTLIMPL_UHEX_0xDA1u 3489u, (D, A, 1), 0xDA1, 0xDA1, 1, 0x25Eu
#define PPUTLIMPL_UHEX_0xDA0u 3488u, (D, A, 0), 0xDA0, 0xDA0, 1, 0x25Fu
#define PPUTLIMPL_UHEX_0xD9Fu 3487u, (D, 9, F), 0xD9F, 0xD9F, 1, 0x260u
#define PPUTLIMPL_UHEX_0xD9Eu 3486u, (D, 9, E), 0xD9E, 0xD9E, 1, 0x261u
#define PPUTLIMPL_UHEX_0xD9Du 3485u, (D, 9, D), 0xD9D, 0xD9D, 1, 0x262u
#define PPUTLIMPL_UHEX_0xD9Cu 3484u, (D, 9, C), 0xD9C, 0xD9C, 1, 0x263u
#define PPUTLIMPL_UHEX_0xD9Bu 3483u, (D, 9, B), 0xD9B, 0xD9B, 1, 0x264u
#define PPUTLIMPL_UHEX_0xD9Au 3482u, (D, 9, A), 0xD9A, 0xD9A, 1, 0x265u
#define PPUTLIMPL_UHEX_0xD99u 3481u, (D, 9, 9), 0xD99, 0xD99, 1, 0x266u
#define PPUTLIMPL_UHEX_0xD98u 3480u, (D, 9, 8), 0xD98, 0xD98, 1, 0x267u
#define PPUTLIMPL_UHEX_0xD97u 3479u, (D, 9, 7), 0xD97, 0xD97, 1, 0x268u
#define PPUTLIMPL_UHEX_0xD96u 3478u, (D, 9, 6), 0xD96, 0xD96, 1, 0x269u
#define PPUTLIMPL_UHEX_0xD95u 3477u, (D, 9, 5), 0xD95, 0xD95, 1, 0x26Au
#define PPUTLIMPL_UHEX_0xD94u 3476u, (D, 9, 4), 0xD94, 0xD94, 1, 0x26Bu
#define PPUTLIMPL_UHEX_0xD93u 3475u, (D, 9, 3), 0xD93, 0xD93, 1, 0x26Cu
#define PPUTLIMPL_UHEX_0xD92u 3474u, (D, 9, 2), 0xD92, 0xD92, 1, 0x26Du
#define PPUTLIMPL_UHEX_0xD91u 3473u, (D, 9, 1), 0xD91, 0xD91, 1, 0x26Eu
#define PPUTLIMPL_UHEX_0xD90u 3472u, (D, 9, 0), 0xD90, 0xD90, 1, 0x26Fu
#define PPUTLIMPL_UHEX_0xD8Fu 3471u, (D, 8, F), 0xD8F, 0xD8F, 1, 0x270u
#define PPUTLIMPL_UHEX_0xD8Eu 3470u, (D, 8, E), 0xD8E, 0xD8E, 1, 0x271u
#define PPUTLIMPL_UHEX_0xD8Du 3469u, (D, 8, D), 0xD8D, 0xD8D, 1, 0x272u
#define PPUTLIMPL_UHEX_0xD8Cu 3468u, (D, 8, C), 0xD8C, 0xD8C, 1, 0x273u
#define PPUTLIMPL_UHEX_0xD8Bu 3467u, (D, 8, B), 0xD8B, 0xD8B, 1, 0x274u
#define PPUTLIMPL_UHEX_0xD8Au 3466u, (D, 8, A), 0xD8A, 0xD8A, 1, 0x275u
#define PPUTLIMPL_UHEX_0xD89u 3465u, (D, 8, 9), 0xD89, 0xD89, 1, 0x276u
#define PPUTLIMPL_UHEX_0xD88u 3464u, (D, 8, 8), 0xD88, 0xD88, 1, 0x277u
#define PPUTLIMPL_UHEX_0xD87u 3463u, (D, 8, 7), 0xD87, 0xD87, 1, 0x278u
#define PPUTLIMPL_UHEX_0xD86u 3462u, (D, 8, 6), 0xD86, 0xD86, 1, 0x279u
#define PPUTLIMPL_UHEX_0xD85u 3461u, (D, 8, 5), 0xD85, 0xD85, 1, 0x27Au
#define PPUTLIMPL_UHEX_0xD84u 3460u, (D, 8, 4), 0xD84, 0xD84, 1, 0x27Bu
#define PPUTLIMPL_UHEX_0xD83u 3459u, (D, 8, 3), 0xD83, 0xD83, 1, 0x27Cu
#define PPUTLIMPL_UHEX_0xD82u 3458u, (D, 8, 2), 0xD82, 0xD82, 1, 0x27Du
#define PPUTLIMPL_UHEX_0xD81u 3457u, (D, 8, 1), 0xD81, 0xD81, 1, 0x27Eu
#define PPUTLIMPL_UHEX_0xD80u 3456u, (D, 8, 0), 0xD80, 0xD80, 1, 0x27Fu
#define PPUTLIMPL_UHEX_0xD7Fu 3455u, (D, 7, F), 0xD7F, 0xD7F, 1, 0x280u
#define PPUTLIMPL_UHEX_0xD7Eu 3454u, (D, 7, E), 0xD7E, 0xD7E, 1, 0x281u
#define PPUTLIMPL_UHEX_0xD7Du 3453u, (D, 7, D), 0xD7D, 0xD7D, 1, 0x282u
#define PPUTLIMPL_UHEX_0xD7Cu 3452u, (D, 7, C), 0xD7C, 0xD7C, 1, 0x283u
#define PPUTLIMPL_UHEX_0xD7Bu 3451u, (D, 7, B), 0xD7B, 0xD7B, 1, 0x284u
#define PPUTLIMPL_UHEX_0xD7Au 3450u, (D, 7, A), 0xD7A, 0xD7A, 1, 0x285u
#define PPUTLIMPL_UHEX_0xD79u 3449u, (D, 7, 9), 0xD79, 0xD79, 1, 0x286u
#define PPUTLIMPL_UHEX_0xD78u 3448u, (D, 7, 8), 0xD78, 0xD78, 1, 0x287u
#define PPUTLIMPL_UHEX_0xD77u 3447u, (D, 7, 7), 0xD77, 0xD77, 1, 0x288u
#define PPUTLIMPL_UHEX_0xD76u 3446u, (D, 7, 6), 0xD76, 0xD76, 1, 0x289u
#define PPUTLIMPL_UHEX_0xD75u 3445u, (D, 7, 5), 0xD75, 0xD75, 1, 0x28Au
#define PPUTLIMPL_UHEX_0xD74u 3444u, (D, 7, 4), 0xD74, 0xD74, 1, 0x28Bu
#define PPUTLIMPL_UHEX_0xD73u 3443u, (D, 7, 3), 0xD73, 0xD73, 1, 0x28Cu
#define PPUTLIMPL_UHEX_0xD72u 3442u, (D, 7, 2), 0xD72, 0xD72, 1, 0x28Du
#define PPUTLIMPL_UHEX_0xD71u 3441u, (D, 7, 1), 0xD71, 0xD71, 1, 0x28Eu
#define PPUTLIMPL_UHEX_0xD70u 3440u, (D, 7, 0), 0xD70, 0xD70, 1, 0x28Fu
#define PPUTLIMPL_UHEX_0xD6Fu 3439u, (D, 6, F), 0xD6F, 0xD6F, 1, 0x290u
#define PPUTLIMPL_UHEX_0xD6Eu 3438u, (D, 6, E), 0xD6E, 0xD6E, 1, 0x291u
#define PPUTLIMPL_UHEX_0xD6Du 3437u, (D, 6, D), 0xD6D, 0xD6D, 1, 0x292u
#define PPUTLIMPL_UHEX_0xD6Cu 3436u, (D, 6, C), 0xD6C, 0xD6C, 1, 0x293u
#define PPUTLIMPL_UHEX_0xD6Bu 3435u, (D, 6, B), 0xD6B, 0xD6B, 1, 0x294u
#define PPUTLIMPL_UHEX_0xD6Au 3434u, (D, 6, A), 0xD6A, 0xD6A, 1, 0x295u
#define PPUTLIMPL_UHEX_0xD69u 3433u, (D, 6, 9), 0xD69, 0xD69, 1, 0x296u
#define PPUTLIMPL_UHEX_0xD68u 3432u, (D, 6, 8), 0xD68, 0xD68, 1, 0x297u
#define PPUTLIMPL_UHEX_0xD67u 3431u, (D, 6, 7), 0xD67, 0xD67, 1, 0x298u
#define PPUTLIMPL_UHEX_0xD66u 3430u, (D, 6, 6), 0xD66, 0xD66, 1, 0x299u
#define PPUTLIMPL_UHEX_0xD65u 3429u, (D, 6, 5), 0xD65, 0xD65, 1, 0x29Au
#define PPUTLIMPL_UHEX_0xD64u 3428u, (D, 6, 4), 0xD64, 0xD64, 1, 0x29Bu
#define PPUTLIMPL_UHEX_0xD63u 3427u, (D, 6, 3), 0xD63, 0xD63, 1, 0x29Cu
#define PPUTLIMPL_UHEX_0xD62u 3426u, (D, 6, 2), 0xD62, 0xD62, 1, 0x29Du
#define PPUTLIMPL_UHEX_0xD61u 3425u, (D, 6, 1), 0xD61, 0xD61, 1, 0x29Eu
#define PPUTLIMPL_UHEX_0xD60u 3424u, (D, 6, 0), 0xD60, 0xD60, 1, 0x29Fu
#define PPUTLIMPL_UHEX_0xD5Fu 3423u, (D, 5, F), 0xD5F, 0xD5F, 1, 0x2A0u
#define PPUTLIMPL_UHEX_0xD5Eu 3422u, (D, 5, E), 0xD5E, 0xD5E, 1, 0x2A1u
#define PPUTLIMPL_UHEX_0xD5Du 3421u, (D, 5, D), 0xD5D, 0xD5D, 1, 0x2A2u
#define PPUTLIMPL_UHEX_0xD5Cu 3420u, (D, 5, C), 0xD5C, 0xD5C, 1, 0x2A3u
#define PPUTLIMPL_UHEX_0xD5Bu 3419u, (D, 5, B), 0xD5B, 0xD5B, 1, 0x2A4u
#define PPUTLIMPL_UHEX_0xD5Au 3418u, (D, 5, A), 0xD5A, 0xD5A, 1, 0x2A5u
#define PPUTLIMPL_UHEX_0xD59u 3417u, (D, 5, 9), 0xD59, 0xD59, 1, 0x2A6u
#define PPUTLIMPL_UHEX_0xD58u 3416u, (D, 5, 8), 0xD58, 0xD58, 1, 0x2A7u
#define PPUTLIMPL_UHEX_0xD57u 3415u, (D, 5, 7), 0xD57, 0xD57, 1, 0x2A8u
#define PPUTLIMPL_UHEX_0xD56u 3414u, (D, 5, 6), 0xD56, 0xD56, 1, 0x2A9u
#define PPUTLIMPL_UHEX_0xD55u 3413u, (D, 5, 5), 0xD55, 0xD55, 1, 0x2AAu
#define PPUTLIMPL_UHEX_0xD54u 3412u, (D, 5, 4), 0xD54, 0xD54, 1, 0x2ABu
#define PPUTLIMPL_UHEX_0xD53u 3411u, (D, 5, 3), 0xD53, 0xD53, 1, 0x2ACu
#define PPUTLIMPL_UHEX_0xD52u 3410u, (D, 5, 2), 0xD52, 0xD52, 1, 0x2ADu
#define PPUTLIMPL_UHEX_0xD51u 3409u, (D, 5, 1), 0xD51, 0xD51, 1, 0x2AEu
#define PPUTLIMPL_UHEX_0xD50u 3408u, (D, 5, 0), 0xD50, 0xD50, 1, 0x2AFu
#define PPUTLIMPL_UHEX_0xD4Fu 3407u, (D, 4, F), 0xD4F, 0xD4F, 1, 0x2B0u
#define PPUTLIMPL_UHEX_0xD4Eu 3406u, (D, 4, E), 0xD4E, 0xD4E, 1, 0x2B1u
#define PPUTLIMPL_UHEX_0xD4Du 3405u, (D, 4, D), 0xD4D, 0xD4D, 1, 0x2B2u
#define PPUTLIMPL_UHEX_0xD4Cu 3404u, (D, 4, C), 0xD4C, 0xD4C, 1, 0x2B3u
#define PPUTLIMPL_UHEX_0xD4Bu 3403u, (D, 4, B), 0xD4B, 0xD4B, 1, 0x2B4u
#define PPUTLIMPL_UHEX_0xD4Au 3402u, (D, 4, A), 0xD4A, 0xD4A, 1, 0x2B5u
#define PPUTLIMPL_UHEX_0xD49u 3401u, (D, 4, 9), 0xD49, 0xD49, 1, 0x2B6u
#define PPUTLIMPL_UHEX_0xD48u 3400u, (D, 4, 8), 0xD48, 0xD48, 1, 0x2B7u
#define PPUTLIMPL_UHEX_0xD47u 3399u, (D, 4, 7), 0xD47, 0xD47, 1, 0x2B8u
#define PPUTLIMPL_UHEX_0xD46u 3398u, (D, 4, 6), 0xD46, 0xD46, 1, 0x2B9u
#define PPUTLIMPL_UHEX_0xD45u 3397u, (D, 4, 5), 0xD45, 0xD45, 1, 0x2BAu
#define PPUTLIMPL_UHEX_0xD44u 3396u, (D, 4, 4), 0xD44, 0xD44, 1, 0x2BBu
#define PPUTLIMPL_UHEX_0xD43u 3395u, (D, 4, 3), 0xD43, 0xD43, 1, 0x2BCu
#define PPUTLIMPL_UHEX_0xD42u 3394u, (D, 4, 2), 0xD42, 0xD42, 1, 0x2BDu
#define PPUTLIMPL_UHEX_0xD41u 3393u, (D, 4, 1), 0xD41, 0xD41, 1, 0x2BEu
#define PPUTLIMPL_UHEX_0xD40u 3392u, (D, 4, 0), 0xD40, 0xD40, 1, 0x2BFu
#define PPUTLIMPL_UHEX_0xD3Fu 3391u, (D, 3, F), 0xD3F, 0xD3F, 1, 0x2C0u
#define PPUTLIMPL_UHEX_0xD3Eu 3390u, (D, 3, E), 0xD3E, 0xD3E, 1, 0x2C1u
#define PPUTLIMPL_UHEX_0xD3Du 3389u, (D, 3, D), 0xD3D, 0xD3D, 1, 0x2C2u
#define PPUTLIMPL_UHEX_0xD3Cu 3388u, (D, 3, C), 0xD3C, 0xD3C, 1, 0x2C3u
#define PPUTLIMPL_UHEX_0xD3Bu 3387u, (D, 3, B), 0xD3B, 0xD3B, 1, 0x2C4u
#define PPUTLIMPL_UHEX_0xD3Au 3386u, (D, 3, A), 0xD3A, 0xD3A, 1, 0x2C5u
#define PPUTLIMPL_UHEX_0xD39u 3385u, (D, 3, 9), 0xD39, 0xD39, 1, 0x2C6u
#define PPUTLIMPL_UHEX_0xD38u 3384u, (D, 3, 8), 0xD38, 0xD38, 1, 0x2C7u
#define PPUTLIMPL_UHEX_0xD37u 3383u, (D, 3, 7), 0xD37, 0xD37, 1, 0x2C8u
#define PPUTLIMPL_UHEX_0xD36u 3382u, (D, 3, 6), 0xD36, 0xD36, 1, 0x2C9u
#define PPUTLIMPL_UHEX_0xD35u 3381u, (D, 3, 5), 0xD35, 0xD35, 1, 0x2CAu
#define PPUTLIMPL_UHEX_0xD34u 3380u, (D, 3, 4), 0xD34, 0xD34, 1, 0x2CBu
#define PPUTLIMPL_UHEX_0xD33u 3379u, (D, 3, 3), 0xD33, 0xD33, 1, 0x2CCu
#define PPUTLIMPL_UHEX_0xD32u 3378u, (D, 3, 2), 0xD32, 0xD32, 1, 0x2CDu
#define PPUTLIMPL_UHEX_0xD31u 3377u, (D, 3, 1), 0xD31, 0xD31, 1, 0x2CEu
#define PPUTLIMPL_UHEX_0xD30u 3376u, (D, 3, 0), 0xD30, 0xD30, 1, 0x2CFu
#define PPUTLIMPL_UHEX_0xD2Fu 3375u, (D, 2, F), 0xD2F, 0xD2F, 1, 0x2D0u
#define PPUTLIMPL_UHEX_0xD2Eu 3374u, (D, 2, E), 0xD2E, 0xD2E, 1, 0x2D1u
#define PPUTLIMPL_UHEX_0xD2Du 3373u, (D, 2, D), 0xD2D, 0xD2D, 1, 0x2D2u
#define PPUTLIMPL_UHEX_0xD2Cu 3372u, (D, 2, C), 0xD2C, 0xD2C, 1, 0x2D3u
#define PPUTLIMPL_UHEX_0xD2Bu 3371u, (D, 2, B), 0xD2B, 0xD2B, 1, 0x2D4u
#define PPUTLIMPL_UHEX_0xD2Au 3370u, (D, 2, A), 0xD2A, 0xD2A, 1, 0x2D5u
#define PPUTLIMPL_UHEX_0xD29u 3369u, (D, 2, 9), 0xD29, 0xD29, 1, 0x2D6u
#define PPUTLIMPL_UHEX_0xD28u 3368u, (D, 2, 8), 0xD28, 0xD28, 1, 0x2D7u
#define PPUTLIMPL_UHEX_0xD27u 3367u, (D, 2, 7), 0xD27, 0xD27, 1, 0x2D8u
#define PPUTLIMPL_UHEX_0xD26u 3366u, (D, 2, 6), 0xD26, 0xD26, 1, 0x2D9u
#define PPUTLIMPL_UHEX_0xD25u 3365u, (D, 2, 5), 0xD25, 0xD25, 1, 0x2DAu
#define PPUTLIMPL_UHEX_0xD24u 3364u, (D, 2, 4), 0xD24, 0xD24, 1, 0x2DBu
#define PPUTLIMPL_UHEX_0xD23u 3363u, (D, 2, 3), 0xD23, 0xD23, 1, 0x2DCu
#define PPUTLIMPL_UHEX_0xD22u 3362u, (D, 2, 2), 0xD22, 0xD22, 1, 0x2DDu
#define PPUTLIMPL_UHEX_0xD21u 3361u, (D, 2, 1), 0xD21, 0xD21, 1, 0x2DEu
#define PPUTLIMPL_UHEX_0xD20u 3360u, (D, 2, 0), 0xD20, 0xD20, 1, 0x2DFu
#define PPUTLIMPL_UHEX_0xD1Fu 3359u, (D, 1, F), 0xD1F, 0xD1F, 1, 0x2E0u
#define PPUTLIMPL_UHEX_0xD1Eu 3358u, (D, 1, E), 0xD1E, 0xD1E, 1, 0x2E1u
#define PPUTLIMPL_UHEX_0xD1Du 3357u, (D, 1, D), 0xD1D, 0xD1D, 1, 0x2E2u
#define PPUTLIMPL_UHEX_0xD1Cu 3356u, (D, 1, C), 0xD1C, 0xD1C, 1, 0x2E3u
#define PPUTLIMPL_UHEX_0xD1Bu 3355u, (D, 1, B), 0xD1B, 0xD1B, 1, 0x2E4u
#define PPUTLIMPL_UHEX_0xD1Au 3354u, (D, 1, A), 0xD1A, 0xD1A, 1, 0x2E5u
#define PPUTLIMPL_UHEX_0xD19u 3353u, (D, 1, 9), 0xD19, 0xD19, 1, 0x2E6u
#define PPUTLIMPL_UHEX_0xD18u 3352u, (D, 1, 8), 0xD18, 0xD18, 1, 0x2E7u
#define PPUTLIMPL_UHEX_0xD17u 3351u, (D, 1, 7), 0xD17, 0xD17, 1, 0x2E8u
#define PPUTLIMPL_UHEX_0xD16u 3350u, (D, 1, 6), 0xD16, 0xD16, 1, 0x2E9u
#define PPUTLIMPL_UHEX_0xD15u 3349u, (D, 1, 5), 0xD15, 0xD15, 1, 0x2EAu
#define PPUTLIMPL_UHEX_0xD14u 3348u, (D, 1, 4), 0xD14, 0xD14, 1, 0x2EBu
#define PPUTLIMPL_UHEX_0xD13u 3347u, (D, 1, 3), 0xD13, 0xD13, 1, 0x2ECu
#define PPUTLIMPL_UHEX_0xD12u 3346u, (D, 1, 2), 0xD12, 0xD12, 1, 0x2EDu
#define PPUTLIMPL_UHEX_0xD11u 3345u, (D, 1, 1), 0xD11, 0xD11, 1, 0x2EEu
#define PPUTLIMPL_UHEX_0xD10u 3344u, (D, 1, 0), 0xD10, 0xD10, 1, 0x2EFu
#define PPUTLIMPL_UHEX_0xD0Fu 3343u, (D, 0, F), 0xD0F, 0xD0F, 1, 0x2F0u
#define PPUTLIMPL_UHEX_0xD0Eu 3342u, (D, 0, E), 0xD0E, 0xD0E, 1, 0x2F1u
#define PPUTLIMPL_UHEX_0xD0Du 3341u, (D, 0, D), 0xD0D, 0xD0D, 1, 0x2F2u
#define PPUTLIMPL_UHEX_0xD0Cu 3340u, (D, 0, C), 0xD0C, 0xD0C, 1, 0x2F3u
#define PPUTLIMPL_UHEX_0xD0Bu 3339u, (D, 0, B), 0xD0B, 0xD0B, 1, 0x2F4u
#define PPUTLIMPL_UHEX_0xD0Au 3338u, (D, 0, A), 0xD0A, 0xD0A, 1, 0x2F5u
#define PPUTLIMPL_UHEX_0xD09u 3337u, (D, 0, 9), 0xD09, 0xD09, 1, 0x2F6u
#define PPUTLIMPL_UHEX_0xD08u 3336u, (D, 0, 8), 0xD08, 0xD08, 1, 0x2F7u
#define PPUTLIMPL_UHEX_0xD07u 3335u, (D, 0, 7), 0xD07, 0xD07, 1, 0x2F8u
#define PPUTLIMPL_UHEX_0xD06u 3334u, (D, 0, 6), 0xD06, 0xD06, 1, 0x2F9u
#define PPUTLIMPL_UHEX_0xD05u 3333u, (D, 0, 5), 0xD05, 0xD05, 1, 0x2FAu
#define PPUTLIMPL_UHEX_0xD04u 3332u, (D, 0, 4), 0xD04, 0xD04, 1, 0x2FBu
#define PPUTLIMPL_UHEX_0xD03u 3331u, (D, 0, 3), 0xD03, 0xD03, 1, 0x2FCu
#define PPUTLIMPL_UHEX_0xD02u 3330u, (D, 0, 2), 0xD02, 0xD02, 1, 0x2FDu
#define PPUTLIMPL_UHEX_0xD01u 3329u, (D, 0, 1), 0xD01, 0xD01, 1, 0x2FEu
#define PPUTLIMPL_UHEX_0xD00u 3328u, (D, 0, 0), 0xD00, 0xD00, 1, 0x2FFu
#define PPUTLIMPL_UHEX_0xCFFu 3327u, (C, F, F), 0xCFF, 0xCFF, 1, 0x300u
#define PPUTLIMPL_UHEX_0xCFEu 3326u, (C, F, E), 0xCFE, 0xCFE, 1, 0x301u
#define PPUTLIMPL_UHEX_0xCFDu 3325u, (C, F, D), 0xCFD, 0xCFD, 1, 0x302u
#define PPUTLIMPL_UHEX_0xCFCu 3324u, (C, F, C), 0xCFC, 0xCFC, 1, 0x303u
#define PPUTLIMPL_UHEX_0xCFBu 3323u, (C, F, B), 0xCFB, 0xCFB, 1, 0x304u
#define PPUTLIMPL_UHEX_0xCFAu 3322u, (C, F, A), 0xCFA, 0xCFA, 1, 0x305u
#define PPUTLIMPL_UHEX_0xCF9u 3321u, (C, F, 9), 0xCF9, 0xCF9, 1, 0x306u
#define PPUTLIMPL_UHEX_0xCF8u 3320u, (C, F, 8), 0xCF8, 0xCF8, 1, 0x307u
#define PPUTLIMPL_UHEX_0xCF7u 3319u, (C, F, 7), 0xCF7, 0xCF7, 1, 0x308u
#define PPUTLIMPL_UHEX_0xCF6u 3318u, (C, F, 6), 0xCF6, 0xCF6, 1, 0x309u
#define PPUTLIMPL_UHEX_0xCF5u 3317u, (C, F, 5), 0xCF5, 0xCF5, 1, 0x30Au
#define PPUTLIMPL_UHEX_0xCF4u 3316u, (C, F, 4), 0xCF4, 0xCF4, 1, 0x30Bu
#define PPUTLIMPL_UHEX_0xCF3u 3315u, (C, F, 3), 0xCF3, 0xCF3, 1, 0x30Cu
#define PPUTLIMPL_UHEX_0xCF2u 3314u, (C, F, 2), 0xCF2, 0xCF2, 1, 0x30Du
#define PPUTLIMPL_UHEX_0xCF1u 3313u, (C, F, 1), 0xCF1, 0xCF1, 1, 0x30Eu
#define PPUTLIMPL_UHEX_0xCF0u 3312u, (C, F, 0), 0xCF0, 0xCF0, 1, 0x30Fu
#define PPUTLIMPL_UHEX_0xCEFu 3311u, (C, E, F), 0xCEF, 0xCEF, 1, 0x310u
#define PPUTLIMPL_UHEX_0xCEEu 3310u, (C, E, E), 0xCEE, 0xCEE, 1, 0x311u
#define PPUTLIMPL_UHEX_0xCEDu 3309u, (C, E, D), 0xCED, 0xCED, 1, 0x312u
#define PPUTLIMPL_UHEX_0xCECu 3308u, (C, E, C), 0xCEC, 0xCEC, 1, 0x313u
#define PPUTLIMPL_UHEX_0xCEBu 3307u, (C, E, B), 0xCEB, 0xCEB, 1, 0x314u
#define PPUTLIMPL_UHEX_0xCEAu 3306u, (C, E, A), 0xCEA, 0xCEA, 1, 0x315u
#define PPUTLIMPL_UHEX_0xCE9u 3305u, (C, E, 9), 0xCE9, 0xCE9, 1, 0x316u
#define PPUTLIMPL_UHEX_0xCE8u 3304u, (C, E, 8), 0xCE8, 0xCE8, 1, 0x317u
#define PPUTLIMPL_UHEX_0xCE7u 3303u, (C, E, 7), 0xCE7, 0xCE7, 1, 0x318u
#define PPUTLIMPL_UHEX_0xCE6u 3302u, (C, E, 6), 0xCE6, 0xCE6, 1, 0x319u
#define PPUTLIMPL_UHEX_0xCE5u 3301u, (C, E, 5), 0xCE5, 0xCE5, 1, 0x31Au
#define PPUTLIMPL_UHEX_0xCE4u 3300u, (C, E, 4), 0xCE4, 0xCE4, 1, 0x31Bu
#define PPUTLIMPL_UHEX_0xCE3u 3299u, (C, E, 3), 0xCE3, 0xCE3, 1, 0x31Cu
#define PPUTLIMPL_UHEX_0xCE2u 3298u, (C, E, 2), 0xCE2, 0xCE2, 1, 0x31Du
#define PPUTLIMPL_UHEX_0xCE1u 3297u, (C, E, 1), 0xCE1, 0xCE1, 1, 0x31Eu
#define PPUTLIMPL_UHEX_0xCE0u 3296u, (C, E, 0), 0xCE0, 0xCE0, 1, 0x31Fu
#define PPUTLIMPL_UHEX_0xCDFu 3295u, (C, D, F), 0xCDF, 0xCDF, 1, 0x320u
#define PPUTLIMPL_UHEX_0xCDEu 3294u, (C, D, E), 0xCDE, 0xCDE, 1, 0x321u
#define PPUTLIMPL_UHEX_0xCDDu 3293u, (C, D, D), 0xCDD, 0xCDD, 1, 0x322u
#define PPUTLIMPL_UHEX_0xCDCu 3292u, (C, D, C), 0xCDC, 0xCDC, 1, 0x323u
#define PPUTLIMPL_UHEX_0xCDBu 3291u, (C, D, B), 0xCDB, 0xCDB, 1, 0x324u
#define PPUTLIMPL_UHEX_0xCDAu 3290u, (C, D, A), 0xCDA, 0xCDA, 1, 0x325u
#define PPUTLIMPL_UHEX_0xCD9u 3289u, (C, D, 9), 0xCD9, 0xCD9, 1, 0x326u
#define PPUTLIMPL_UHEX_0xCD8u 3288u, (C, D, 8), 0xCD8, 0xCD8, 1, 0x327u
#define PPUTLIMPL_UHEX_0xCD7u 3287u, (C, D, 7), 0xCD7, 0xCD7, 1, 0x328u
#define PPUTLIMPL_UHEX_0xCD6u 3286u, (C, D, 6), 0xCD6, 0xCD6, 1, 0x329u
#define PPUTLIMPL_UHEX_0xCD5u 3285u, (C, D, 5), 0xCD5, 0xCD5, 1, 0x32Au
#define PPUTLIMPL_UHEX_0xCD4u 3284u, (C, D, 4), 0xCD4, 0xCD4, 1, 0x32Bu
#define PPUTLIMPL_UHEX_0xCD3u 3283u, (C, D, 3), 0xCD3, 0xCD3, 1, 0x32Cu
#define PPUTLIMPL_UHEX_0xCD2u 3282u, (C, D, 2), 0xCD2, 0xCD2, 1, 0x32Du
#define PPUTLIMPL_UHEX_0xCD1u 3281u, (C, D, 1), 0xCD1, 0xCD1, 1, 0x32Eu
#define PPUTLIMPL_UHEX_0xCD0u 3280u, (C, D, 0), 0xCD0, 0xCD0, 1, 0x32Fu
#define PPUTLIMPL_UHEX_0xCCFu 3279u, (C, C, F), 0xCCF, 0xCCF, 1, 0x330u
#define PPUTLIMPL_UHEX_0xCCEu 3278u, (C, C, E), 0xCCE, 0xCCE, 1, 0x331u
#define PPUTLIMPL_UHEX_0xCCDu 3277u, (C, C, D), 0xCCD, 0xCCD, 1, 0x332u
#define PPUTLIMPL_UHEX_0xCCCu 3276u, (C, C, C), 0xCCC, 0xCCC, 1, 0x333u
#define PPUTLIMPL_UHEX_0xCCBu 3275u, (C, C, B), 0xCCB, 0xCCB, 1, 0x334u
#define PPUTLIMPL_UHEX_0xCCAu 3274u, (C, C, A), 0xCCA, 0xCCA, 1, 0x335u
#define PPUTLIMPL_UHEX_0xCC9u 3273u, (C, C, 9), 0xCC9, 0xCC9, 1, 0x336u
#define PPUTLIMPL_UHEX_0xCC8u 3272u, (C, C, 8), 0xCC8, 0xCC8, 1, 0x337u
#define PPUTLIMPL_UHEX_0xCC7u 3271u, (C, C, 7), 0xCC7, 0xCC7, 1, 0x338u
#define PPUTLIMPL_UHEX_0xCC6u 3270u, (C, C, 6), 0xCC6, 0xCC6, 1, 0x339u
#define PPUTLIMPL_UHEX_0xCC5u 3269u, (C, C, 5), 0xCC5, 0xCC5, 1, 0x33Au
#define PPUTLIMPL_UHEX_0xCC4u 3268u, (C, C, 4), 0xCC4, 0xCC4, 1, 0x33Bu
#define PPUTLIMPL_UHEX_0xCC3u 3267u, (C, C, 3), 0xCC3, 0xCC3, 1, 0x33Cu
#define PPUTLIMPL_UHEX_0xCC2u 3266u, (C, C, 2), 0xCC2, 0xCC2, 1, 0x33Du
#define PPUTLIMPL_UHEX_0xCC1u 3265u, (C, C, 1), 0xCC1, 0xCC1, 1, 0x33Eu
#define PPUTLIMPL_UHEX_0xCC0u 3264u, (C, C, 0), 0xCC0, 0xCC0, 1, 0x33Fu
#define PPUTLIMPL_UHEX_0xCBFu 3263u, (C, B, F), 0xCBF, 0xCBF, 1, 0x340u
#define PPUTLIMPL_UHEX_0xCBEu 3262u, (C, B, E), 0xCBE, 0xCBE, 1, 0x341u
#define PPUTLIMPL_UHEX_0xCBDu 3261u, (C, B, D), 0xCBD, 0xCBD, 1, 0x342u
#define PPUTLIMPL_UHEX_0xCBCu 3260u, (C, B, C), 0xCBC, 0xCBC, 1, 0x343u
#define PPUTLIMPL_UHEX_0xCBBu 3259u, (C, B, B), 0xCBB, 0xCBB, 1, 0x344u
#define PPUTLIMPL_UHEX_0xCBAu 3258u, (C, B, A), 0xCBA, 0xCBA, 1, 0x345u
#define PPUTLIMPL_UHEX_0xCB9u 3257u, (C, B, 9), 0xCB9, 0xCB9, 1, 0x346u
#define PPUTLIMPL_UHEX_0xCB8u 3256u, (C, B, 8), 0xCB8, 0xCB8, 1, 0x347u
#define PPUTLIMPL_UHEX_0xCB7u 3255u, (C, B, 7), 0xCB7, 0xCB7, 1, 0x348u
#define PPUTLIMPL_UHEX_0xCB6u 3254u, (C, B, 6), 0xCB6, 0xCB6, 1, 0x349u
#define PPUTLIMPL_UHEX_0xCB5u 3253u, (C, B, 5), 0xCB5, 0xCB5, 1, 0x34Au
#define PPUTLIMPL_UHEX_0xCB4u 3252u, (C, B, 4), 0xCB4, 0xCB4, 1, 0x34Bu
#define PPUTLIMPL_UHEX_0xCB3u 3251u, (C, B, 3), 0xCB3, 0xCB3, 1, 0x34Cu
#define PPUTLIMPL_UHEX_0xCB2u 3250u, (C, B, 2), 0xCB2, 0xCB2, 1, 0x34Du
#define PPUTLIMPL_UHEX_0xCB1u 3249u, (C, B, 1), 0xCB1, 0xCB1, 1, 0x34Eu
#define PPUTLIMPL_UHEX_0xCB0u 3248u, (C, B, 0), 0xCB0, 0xCB0, 1, 0x34Fu
#define PPUTLIMPL_UHEX_0xCAFu 3247u, (C, A, F), 0xCAF, 0xCAF, 1, 0x350u
#define PPUTLIMPL_UHEX_0xCAEu 3246u, (C, A, E), 0xCAE, 0xCAE, 1, 0x351u
#define PPUTLIMPL_UHEX_0xCADu 3245u, (C, A, D), 0xCAD, 0xCAD, 1, 0x352u
#define PPUTLIMPL_UHEX_0xCACu 3244u, (C, A, C), 0xCAC, 0xCAC, 1, 0x353u
#define PPUTLIMPL_UHEX_0xCABu 3243u, (C, A, B), 0xCAB, 0xCAB, 1, 0x354u
#define PPUTLIMPL_UHEX_0xCAAu 3242u, (C, A, A), 0xCAA, 0xCAA, 1, 0x355u
#define PPUTLIMPL_UHEX_0xCA9u 3241u, (C, A, 9), 0xCA9, 0xCA9, 1, 0x356u
#define PPUTLIMPL_UHEX_0xCA8u 3240u, (C, A, 8), 0xCA8, 0xCA8, 1, 0x357u
#define PPUTLIMPL_UHEX_0xCA7u 3239u, (C, A, 7), 0xCA7, 0xCA7, 1, 0x358u
#define PPUTLIMPL_UHEX_0xCA6u 3238u, (C, A, 6), 0xCA6, 0xCA6, 1, 0x359u
#define PPUTLIMPL_UHEX_0xCA5u 3237u, (C, A, 5), 0xCA5, 0xCA5, 1, 0x35Au
#define PPUTLIMPL_UHEX_0xCA4u 3236u, (C, A, 4), 0xCA4, 0xCA4, 1, 0x35Bu
#define PPUTLIMPL_UHEX_0xCA3u 3235u, (C, A, 3), 0xCA3, 0xCA3, 1, 0x35Cu
#define PPUTLIMPL_UHEX_0xCA2u 3234u, (C, A, 2), 0xCA2, 0xCA2, 1, 0x35Du
#define PPUTLIMPL_UHEX_0xCA1u 3233u, (C, A, 1), 0xCA1, 0xCA1, 1, 0x35Eu
#define PPUTLIMPL_UHEX_0xCA0u 3232u, (C, A, 0), 0xCA0, 0xCA0, 1, 0x35Fu
#define PPUTLIMPL_UHEX_0xC9Fu 3231u, (C, 9, F), 0xC9F, 0xC9F, 1, 0x360u
#define PPUTLIMPL_UHEX_0xC9Eu 3230u, (C, 9, E), 0xC9E, 0xC9E, 1, 0x361u
#define PPUTLIMPL_UHEX_0xC9Du 3229u, (C, 9, D), 0xC9D, 0xC9D, 1, 0x362u
#define PPUTLIMPL_UHEX_0xC9Cu 3228u, (C, 9, C), 0xC9C, 0xC9C, 1, 0x363u
#define PPUTLIMPL_UHEX_0xC9Bu 3227u, (C, 9, B), 0xC9B, 0xC9B, 1, 0x364u
#define PPUTLIMPL_UHEX_0xC9Au 3226u, (C, 9, A), 0xC9A, 0xC9A, 1, 0x365u
#define PPUTLIMPL_UHEX_0xC99u 3225u, (C, 9, 9), 0xC99, 0xC99, 1, 0x366u
#define PPUTLIMPL_UHEX_0xC98u 3224u, (C, 9, 8), 0xC98, 0xC98, 1, 0x367u
#define PPUTLIMPL_UHEX_0xC97u 3223u, (C, 9, 7), 0xC97, 0xC97, 1, 0x368u
#define PPUTLIMPL_UHEX_0xC96u 3222u, (C, 9, 6), 0xC96, 0xC96, 1, 0x369u
#define PPUTLIMPL_UHEX_0xC95u 3221u, (C, 9, 5), 0xC95, 0xC95, 1, 0x36Au
#define PPUTLIMPL_UHEX_0xC94u 3220u, (C, 9, 4), 0xC94, 0xC94, 1, 0x36Bu
#define PPUTLIMPL_UHEX_0xC93u 3219u, (C, 9, 3), 0xC93, 0xC93, 1, 0x36Cu
#define PPUTLIMPL_UHEX_0xC92u 3218u, (C, 9, 2), 0xC92, 0xC92, 1, 0x36Du
#define PPUTLIMPL_UHEX_0xC91u 3217u, (C, 9, 1), 0xC91, 0xC91, 1, 0x36Eu
#define PPUTLIMPL_UHEX_0xC90u 3216u, (C, 9, 0), 0xC90, 0xC90, 1, 0x36Fu
#define PPUTLIMPL_UHEX_0xC8Fu 3215u, (C, 8, F), 0xC8F, 0xC8F, 1, 0x370u
#define PPUTLIMPL_UHEX_0xC8Eu 3214u, (C, 8, E), 0xC8E, 0xC8E, 1, 0x371u
#define PPUTLIMPL_UHEX_0xC8Du 3213u, (C, 8, D), 0xC8D, 0xC8D, 1, 0x372u
#define PPUTLIMPL_UHEX_0xC8Cu 3212u, (C, 8, C), 0xC8C, 0xC8C, 1, 0x373u
#define PPUTLIMPL_UHEX_0xC8Bu 3211u, (C, 8, B), 0xC8B, 0xC8B, 1, 0x374u
#define PPUTLIMPL_UHEX_0xC8Au 3210u, (C, 8, A), 0xC8A, 0xC8A, 1, 0x375u
#define PPUTLIMPL_UHEX_0xC89u 3209u, (C, 8, 9), 0xC89, 0xC89, 1, 0x376u
#define PPUTLIMPL_UHEX_0xC88u 3208u, (C, 8, 8), 0xC88, 0xC88, 1, 0x377u
#define PPUTLIMPL_UHEX_0xC87u 3207u, (C, 8, 7), 0xC87, 0xC87, 1, 0x378u
#define PPUTLIMPL_UHEX_0xC86u 3206u, (C, 8, 6), 0xC86, 0xC86, 1, 0x379u
#define PPUTLIMPL_UHEX_0xC85u 3205u, (C, 8, 5), 0xC85, 0xC85, 1, 0x37Au
#define PPUTLIMPL_UHEX_0xC84u 3204u, (C, 8, 4), 0xC84, 0xC84, 1, 0x37Bu
#define PPUTLIMPL_UHEX_0xC83u 3203u, (C, 8, 3), 0xC83, 0xC83, 1, 0x37Cu
#define PPUTLIMPL_UHEX_0xC82u 3202u, (C, 8, 2), 0xC82, 0xC82, 1, 0x37Du
#define PPUTLIMPL_UHEX_0xC81u 3201u, (C, 8, 1), 0xC81, 0xC81, 1, 0x37Eu
#define PPUTLIMPL_UHEX_0xC80u 3200u, (C, 8, 0), 0xC80, 0xC80, 1, 0x37Fu
#define PPUTLIMPL_UHEX_0xC7Fu 3199u, (C, 7, F), 0xC7F, 0xC7F, 1, 0x380u
#define PPUTLIMPL_UHEX_0xC7Eu 3198u, (C, 7, E), 0xC7E, 0xC7E, 1, 0x381u
#define PPUTLIMPL_UHEX_0xC7Du 3197u, (C, 7, D), 0xC7D, 0xC7D, 1, 0x382u
#define PPUTLIMPL_UHEX_0xC7Cu 3196u, (C, 7, C), 0xC7C, 0xC7C, 1, 0x383u
#define PPUTLIMPL_UHEX_0xC7Bu 3195u, (C, 7, B), 0xC7B, 0xC7B, 1, 0x384u
#define PPUTLIMPL_UHEX_0xC7Au 3194u, (C, 7, A), 0xC7A, 0xC7A, 1, 0x385u
#define PPUTLIMPL_UHEX_0xC79u 3193u, (C, 7, 9), 0xC79, 0xC79, 1, 0x386u
#define PPUTLIMPL_UHEX_0xC78u 3192u, (C, 7, 8), 0xC78, 0xC78, 1, 0x387u
#define PPUTLIMPL_UHEX_0xC77u 3191u, (C, 7, 7), 0xC77, 0xC77, 1, 0x388u
#define PPUTLIMPL_UHEX_0xC76u 3190u, (C, 7, 6), 0xC76, 0xC76, 1, 0x389u
#define PPUTLIMPL_UHEX_0xC75u 3189u, (C, 7, 5), 0xC75, 0xC75, 1, 0x38Au
#define PPUTLIMPL_UHEX_0xC74u 3188u, (C, 7, 4), 0xC74, 0xC74, 1, 0x38Bu
#define PPUTLIMPL_UHEX_0xC73u 3187u, (C, 7, 3), 0xC73, 0xC73, 1, 0x38Cu
#define PPUTLIMPL_UHEX_0xC72u 3186u, (C, 7, 2), 0xC72, 0xC72, 1, 0x38Du
#define PPUTLIMPL_UHEX_0xC71u 3185u, (C, 7, 1), 0xC71, 0xC71, 1, 0x38Eu
#define PPUTLIMPL_UHEX_0xC70u 3184u, (C, 7, 0), 0xC70, 0xC70, 1, 0x38Fu
#define PPUTLIMPL_UHEX_0xC6Fu 3183u, (C, 6, F), 0xC6F, 0xC6F, 1, 0x390u
#define PPUTLIMPL_UHEX_0xC6Eu 3182u, (C, 6, E), 0xC6E, 0xC6E, 1, 0x391u
#define PPUTLIMPL_UHEX_0xC6Du 3181u, (C, 6, D), 0xC6D, 0xC6D, 1, 0x392u
#define PPUTLIMPL_UHEX_0xC6Cu 3180u, (C, 6, C), 0xC6C, 0xC6C, 1, 0x393u
#define PPUTLIMPL_UHEX_0xC6Bu 3179u, (C, 6, B), 0xC6B, 0xC6B, 1, 0x394u
#define PPUTLIMPL_UHEX_0xC6Au 3178u, (C, 6, A), 0xC6A, 0xC6A, 1, 0x395u
#define PPUTLIMPL_UHEX_0xC69u 3177u, (C, 6, 9), 0xC69, 0xC69, 1, 0x396u
#define PPUTLIMPL_UHEX_0xC68u 3176u, (C, 6, 8), 0xC68, 0xC68, 1, 0x397u
#define PPUTLIMPL_UHEX_0xC67u 3175u, (C, 6, 7), 0xC67, 0xC67, 1, 0x398u
#define PPUTLIMPL_UHEX_0xC66u 3174u, (C, 6, 6), 0xC66, 0xC66, 1, 0x399u
#define PPUTLIMPL_UHEX_0xC65u 3173u, (C, 6, 5), 0xC65, 0xC65, 1, 0x39Au
#define PPUTLIMPL_UHEX_0xC64u 3172u, (C, 6, 4), 0xC64, 0xC64, 1, 0x39Bu
#define PPUTLIMPL_UHEX_0xC63u 3171u, (C, 6, 3), 0xC63, 0xC63, 1, 0x39Cu
#define PPUTLIMPL_UHEX_0xC62u 3170u, (C, 6, 2), 0xC62, 0xC62, 1, 0x39Du
#define PPUTLIMPL_UHEX_0xC61u 3169u, (C, 6, 1), 0xC61, 0xC61, 1, 0x39Eu
#define PPUTLIMPL_UHEX_0xC60u 3168u, (C, 6, 0), 0xC60, 0xC60, 1, 0x39Fu
#define PPUTLIMPL_UHEX_0xC5Fu 3167u, (C, 5, F), 0xC5F, 0xC5F, 1, 0x3A0u
#define PPUTLIMPL_UHEX_0xC5Eu 3166u, (C, 5, E), 0xC5E, 0xC5E, 1, 0x3A1u
#define PPUTLIMPL_UHEX_0xC5Du 3165u, (C, 5, D), 0xC5D, 0xC5D, 1, 0x3A2u
#define PPUTLIMPL_UHEX_0xC5Cu 3164u, (C, 5, C), 0xC5C, 0xC5C, 1, 0x3A3u
#define PPUTLIMPL_UHEX_0xC5Bu 3163u, (C, 5, B), 0xC5B, 0xC5B, 1, 0x3A4u
#define PPUTLIMPL_UHEX_0xC5Au 3162u, (C, 5, A), 0xC5A, 0xC5A, 1, 0x3A5u
#define PPUTLIMPL_UHEX_0xC59u 3161u, (C, 5, 9), 0xC59, 0xC59, 1, 0x3A6u
#define PPUTLIMPL_UHEX_0xC58u 3160u, (C, 5, 8), 0xC58, 0xC58, 1, 0x3A7u
#define PPUTLIMPL_UHEX_0xC57u 3159u, (C, 5, 7), 0xC57, 0xC57, 1, 0x3A8u
#define PPUTLIMPL_UHEX_0xC56u 3158u, (C, 5, 6), 0xC56, 0xC56, 1, 0x3A9u
#define PPUTLIMPL_UHEX_0xC55u 3157u, (C, 5, 5), 0xC55, 0xC55, 1, 0x3AAu
#define PPUTLIMPL_UHEX_0xC54u 3156u, (C, 5, 4), 0xC54, 0xC54, 1, 0x3ABu
#define PPUTLIMPL_UHEX_0xC53u 3155u, (C, 5, 3), 0xC53, 0xC53, 1, 0x3ACu
#define PPUTLIMPL_UHEX_0xC52u 3154u, (C, 5, 2), 0xC52, 0xC52, 1, 0x3ADu
#define PPUTLIMPL_UHEX_0xC51u 3153u, (C, 5, 1), 0xC51, 0xC51, 1, 0x3AEu
#define PPUTLIMPL_UHEX_0xC50u 3152u, (C, 5, 0), 0xC50, 0xC50, 1, 0x3AFu
#define PPUTLIMPL_UHEX_0xC4Fu 3151u, (C, 4, F), 0xC4F, 0xC4F, 1, 0x3B0u
#define PPUTLIMPL_UHEX_0xC4Eu 3150u, (C, 4, E), 0xC4E, 0xC4E, 1, 0x3B1u
#define PPUTLIMPL_UHEX_0xC4Du 3149u, (C, 4, D), 0xC4D, 0xC4D, 1, 0x3B2u
#define PPUTLIMPL_UHEX_0xC4Cu 3148u, (C, 4, C), 0xC4C, 0xC4C, 1, 0x3B3u
#define PPUTLIMPL_UHEX_0xC4Bu 3147u, (C, 4, B), 0xC4B, 0xC4B, 1, 0x3B4u
#define PPUTLIMPL_UHEX_0xC4Au 3146u, (C, 4, A), 0xC4A, 0xC4A, 1, 0x3B5u
#define PPUTLIMPL_UHEX_0xC49u 3145u, (C, 4, 9), 0xC49, 0xC49, 1, 0x3B6u
#define PPUTLIMPL_UHEX_0xC48u 3144u, (C, 4, 8), 0xC48, 0xC48, 1, 0x3B7u
#define PPUTLIMPL_UHEX_0xC47u 3143u, (C, 4, 7), 0xC47, 0xC47, 1, 0x3B8u
#define PPUTLIMPL_UHEX_0xC46u 3142u, (C, 4, 6), 0xC46, 0xC46, 1, 0x3B9u
#define PPUTLIMPL_UHEX_0xC45u 3141u, (C, 4, 5), 0xC45, 0xC45, 1, 0x3BAu
#define PPUTLIMPL_UHEX_0xC44u 3140u, (C, 4, 4), 0xC44, 0xC44, 1, 0x3BBu
#define PPUTLIMPL_UHEX_0xC43u 3139u, (C, 4, 3), 0xC43, 0xC43, 1, 0x3BCu
#define PPUTLIMPL_UHEX_0xC42u 3138u, (C, 4, 2), 0xC42, 0xC42, 1, 0x3BDu
#define PPUTLIMPL_UHEX_0xC41u 3137u, (C, 4, 1), 0xC41, 0xC41, 1, 0x3BEu
#define PPUTLIMPL_UHEX_0xC40u 3136u, (C, 4, 0), 0xC40, 0xC40, 1, 0x3BFu
#define PPUTLIMPL_UHEX_0xC3Fu 3135u, (C, 3, F), 0xC3F, 0xC3F, 1, 0x3C0u
#define PPUTLIMPL_UHEX_0xC3Eu 3134u, (C, 3, E), 0xC3E, 0xC3E, 1, 0x3C1u
#define PPUTLIMPL_UHEX_0xC3Du 3133u, (C, 3, D), 0xC3D, 0xC3D, 1, 0x3C2u
#define PPUTLIMPL_UHEX_0xC3Cu 3132u, (C, 3, C), 0xC3C, 0xC3C, 1, 0x3C3u
#define PPUTLIMPL_UHEX_0xC3Bu 3131u, (C, 3, B), 0xC3B, 0xC3B, 1, 0x3C4u
#define PPUTLIMPL_UHEX_0xC3Au 3130u, (C, 3, A), 0xC3A, 0xC3A, 1, 0x3C5u
#define PPUTLIMPL_UHEX_0xC39u 3129u, (C, 3, 9), 0xC39, 0xC39, 1, 0x3C6u
#define PPUTLIMPL_UHEX_0xC38u 3128u, (C, 3, 8), 0xC38, 0xC38, 1, 0x3C7u
#define PPUTLIMPL_UHEX_0xC37u 3127u, (C, 3, 7), 0xC37, 0xC37, 1, 0x3C8u
#define PPUTLIMPL_UHEX_0xC36u 3126u, (C, 3, 6), 0xC36, 0xC36, 1, 0x3C9u
#define PPUTLIMPL_UHEX_0xC35u 3125u, (C, 3, 5), 0xC35, 0xC35, 1, 0x3CAu
#define PPUTLIMPL_UHEX_0xC34u 3124u, (C, 3, 4), 0xC34, 0xC34, 1, 0x3CBu
#define PPUTLIMPL_UHEX_0xC33u 3123u, (C, 3, 3), 0xC33, 0xC33, 1, 0x3CCu
#define PPUTLIMPL_UHEX_0xC32u 3122u, (C, 3, 2), 0xC32, 0xC32, 1, 0x3CDu
#define PPUTLIMPL_UHEX_0xC31u 3121u, (C, 3, 1), 0xC31, 0xC31, 1, 0x3CEu
#define PPUTLIMPL_UHEX_0xC30u 3120u, (C, 3, 0), 0xC30, 0xC30, 1, 0x3CFu
#define PPUTLIMPL_UHEX_0xC2Fu 3119u, (C, 2, F), 0xC2F, 0xC2F, 1, 0x3D0u
#define PPUTLIMPL_UHEX_0xC2Eu 3118u, (C, 2, E), 0xC2E, 0xC2E, 1, 0x3D1u
#define PPUTLIMPL_UHEX_0xC2Du 3117u, (C, 2, D), 0xC2D, 0xC2D, 1, 0x3D2u
#define PPUTLIMPL_UHEX_0xC2Cu 3116u, (C, 2, C), 0xC2C, 0xC2C, 1, 0x3D3u
#define PPUTLIMPL_UHEX_0xC2Bu 3115u, (C, 2, B), 0xC2B, 0xC2B, 1, 0x3D4u
#define PPUTLIMPL_UHEX_0xC2Au 3114u, (C, 2, A), 0xC2A, 0xC2A, 1, 0x3D5u
#define PPUTLIMPL_UHEX_0xC29u 3113u, (C, 2, 9), 0xC29, 0xC29, 1, 0x3D6u
#define PPUTLIMPL_UHEX_0xC28u 3112u, (C, 2, 8), 0xC28, 0xC28, 1, 0x3D7u
#define PPUTLIMPL_UHEX_0xC27u 3111u, (C, 2, 7), 0xC27, 0xC27, 1, 0x3D8u
#define PPUTLIMPL_UHEX_0xC26u 3110u, (C, 2, 6), 0xC26, 0xC26, 1, 0x3D9u
#define PPUTLIMPL_UHEX_0xC25u 3109u, (C, 2, 5), 0xC25, 0xC25, 1, 0x3DAu
#define PPUTLIMPL_UHEX_0xC24u 3108u, (C, 2, 4), 0xC24, 0xC24, 1, 0x3DBu
#define PPUTLIMPL_UHEX_0xC23u 3107u, (C, 2, 3), 0xC23, 0xC23, 1, 0x3DCu
#define PPUTLIMPL_UHEX_0xC22u 3106u, (C, 2, 2), 0xC22, 0xC22, 1, 0x3DDu
#define PPUTLIMPL_UHEX_0xC21u 3105u, (C, 2, 1), 0xC21, 0xC21, 1, 0x3DEu
#define PPUTLIMPL_UHEX_0xC20u 3104u, (C, 2, 0), 0xC20, 0xC20, 1, 0x3DFu
#define PPUTLIMPL_UHEX_0xC1Fu 3103u, (C, 1, F), 0xC1F, 0xC1F, 1, 0x3E0u
#define PPUTLIMPL_UHEX_0xC1Eu 3102u, (C, 1, E), 0xC1E, 0xC1E, 1, 0x3E1u
#define PPUTLIMPL_UHEX_0xC1Du 3101u, (C, 1, D), 0xC1D, 0xC1D, 1, 0x3E2u
#define PPUTLIMPL_UHEX_0xC1Cu 3100u, (C, 1, C), 0xC1C, 0xC1C, 1, 0x3E3u
#define PPUTLIMPL_UHEX_0xC1Bu 3099u, (C, 1, B), 0xC1B, 0xC1B, 1, 0x3E4u
#define PPUTLIMPL_UHEX_0xC1Au 3098u, (C, 1, A), 0xC1A, 0xC1A, 1, 0x3E5u
#define PPUTLIMPL_UHEX_0xC19u 3097u, (C, 1, 9), 0xC19, 0xC19, 1, 0x3E6u
#define PPUTLIMPL_UHEX_0xC18u 3096u, (C, 1, 8), 0xC18, 0xC18, 1, 0x3E7u
#define PPUTLIMPL_UHEX_0xC17u 3095u, (C, 1, 7), 0xC17, 0xC17, 1, 0x3E8u
#define PPUTLIMPL_UHEX_0xC16u 3094u, (C, 1, 6), 0xC16, 0xC16, 1, 0x3E9u
#define PPUTLIMPL_UHEX_0xC15u 3093u, (C, 1, 5), 0xC15, 0xC15, 1, 0x3EAu
#define PPUTLIMPL_UHEX_0xC14u 3092u, (C, 1, 4), 0xC14, 0xC14, 1, 0x3EBu
#define PPUTLIMPL_UHEX_0xC13u 3091u, (C, 1, 3), 0xC13, 0xC13, 1, 0x3ECu
#define PPUTLIMPL_UHEX_0xC12u 3090u, (C, 1, 2), 0xC12, 0xC12, 1, 0x3EDu
#define PPUTLIMPL_UHEX_0xC11u 3089u, (C, 1, 1), 0xC11, 0xC11, 1, 0x3EEu
#define PPUTLIMPL_UHEX_0xC10u 3088u, (C, 1, 0), 0xC10, 0xC10, 1, 0x3EFu
#define PPUTLIMPL_UHEX_0xC0Fu 3087u, (C, 0, F), 0xC0F, 0xC0F, 1, 0x3F0u
#define PPUTLIMPL_UHEX_0xC0Eu 3086u, (C, 0, E), 0xC0E, 0xC0E, 1, 0x3F1u
#define PPUTLIMPL_UHEX_0xC0Du 3085u, (C, 0, D), 0xC0D, 0xC0D, 1, 0x3F2u
#define PPUTLIMPL_UHEX_0xC0Cu 3084u, (C, 0, C), 0xC0C, 0xC0C, 1, 0x3F3u
#define PPUTLIMPL_UHEX_0xC0Bu 3083u, (C, 0, B), 0xC0B, 0xC0B, 1, 0x3F4u
#define PPUTLIMPL_UHEX_0xC0Au 3082u, (C, 0, A), 0xC0A, 0xC0A, 1, 0x3F5u
#define PPUTLIMPL_UHEX_0xC09u 3081u, (C, 0, 9), 0xC09, 0xC09, 1, 0x3F6u
#define PPUTLIMPL_UHEX_0xC08u 3080u, (C, 0, 8), 0xC08, 0xC08, 1, 0x3F7u
#define PPUTLIMPL_UHEX_0xC07u 3079u, (C, 0, 7), 0xC07, 0xC07, 1, 0x3F8u
#define PPUTLIMPL_UHEX_0xC06u 3078u, (C, 0, 6), 0xC06, 0xC06, 1, 0x3F9u
#define PPUTLIMPL_UHEX_0xC05u 3077u, (C, 0, 5), 0xC05, 0xC05, 1, 0x3FAu
#define PPUTLIMPL_UHEX_0xC04u 3076u, (C, 0, 4), 0xC04, 0xC04, 1, 0x3FBu
#define PPUTLIMPL_UHEX_0xC03u 3075u, (C, 0, 3), 0xC03, 0xC03, 1, 0x3FCu
#define PPUTLIMPL_UHEX_0xC02u 3074u, (C, 0, 2), 0xC02, 0xC02, 1, 0x3FDu
#define PPUTLIMPL_UHEX_0xC01u 3073u, (C, 0, 1), 0xC01, 0xC01, 1, 0x3FEu
#define PPUTLIMPL_UHEX_0xC00u 3072u, (C, 0, 0), 0xC00, 0xC00, 1, 0x3FFu
#define PPUTLIMPL_UHEX_0xBFFu 3071u, (B, F, F), 0xBFF, 0xBFF, 1, 0x400u
#define PPUTLIMPL_UHEX_0xBFEu 3070u, (B, F, E), 0xBFE, 0xBFE, 1, 0x401u
#define PPUTLIMPL_UHEX_0xBFDu 3069u, (B, F, D), 0xBFD, 0xBFD, 1, 0x402u
#define PPUTLIMPL_UHEX_0xBFCu 3068u, (B, F, C), 0xBFC, 0xBFC, 1, 0x403u
#define PPUTLIMPL_UHEX_0xBFBu 3067u, (B, F, B), 0xBFB, 0xBFB, 1, 0x404u
#define PPUTLIMPL_UHEX_0xBFAu 3066u, (B, F, A), 0xBFA, 0xBFA, 1, 0x405u
#define PPUTLIMPL_UHEX_0xBF9u 3065u, (B, F, 9), 0xBF9, 0xBF9, 1, 0x406u
#define PPUTLIMPL_UHEX_0xBF8u 3064u, (B, F, 8), 0xBF8, 0xBF8, 1, 0x407u
#define PPUTLIMPL_UHEX_0xBF7u 3063u, (B, F, 7), 0xBF7, 0xBF7, 1, 0x408u
#define PPUTLIMPL_UHEX_0xBF6u 3062u, (B, F, 6), 0xBF6, 0xBF6, 1, 0x409u
#define PPUTLIMPL_UHEX_0xBF5u 3061u, (B, F, 5), 0xBF5, 0xBF5, 1, 0x40Au
#define PPUTLIMPL_UHEX_0xBF4u 3060u, (B, F, 4), 0xBF4, 0xBF4, 1, 0x40Bu
#define PPUTLIMPL_UHEX_0xBF3u 3059u, (B, F, 3), 0xBF3, 0xBF3, 1, 0x40Cu
#define PPUTLIMPL_UHEX_0xBF2u 3058u, (B, F, 2), 0xBF2, 0xBF2, 1, 0x40Du
#define PPUTLIMPL_UHEX_0xBF1u 3057u, (B, F, 1), 0xBF1, 0xBF1, 1, 0x40Eu
#define PPUTLIMPL_UHEX_0xBF0u 3056u, (B, F, 0), 0xBF0, 0xBF0, 1, 0x40Fu
#define PPUTLIMPL_UHEX_0xBEFu 3055u, (B, E, F), 0xBEF, 0xBEF, 1, 0x410u
#define PPUTLIMPL_UHEX_0xBEEu 3054u, (B, E, E), 0xBEE, 0xBEE, 1, 0x411u
#define PPUTLIMPL_UHEX_0xBEDu 3053u, (B, E, D), 0xBED, 0xBED, 1, 0x412u
#define PPUTLIMPL_UHEX_0xBECu 3052u, (B, E, C), 0xBEC, 0xBEC, 1, 0x413u
#define PPUTLIMPL_UHEX_0xBEBu 3051u, (B, E, B), 0xBEB, 0xBEB, 1, 0x414u
#define PPUTLIMPL_UHEX_0xBEAu 3050u, (B, E, A), 0xBEA, 0xBEA, 1, 0x415u
#define PPUTLIMPL_UHEX_0xBE9u 3049u, (B, E, 9), 0xBE9, 0xBE9, 1, 0x416u
#define PPUTLIMPL_UHEX_0xBE8u 3048u, (B, E, 8), 0xBE8, 0xBE8, 1, 0x417u
#define PPUTLIMPL_UHEX_0xBE7u 3047u, (B, E, 7), 0xBE7, 0xBE7, 1, 0x418u
#define PPUTLIMPL_UHEX_0xBE6u 3046u, (B, E, 6), 0xBE6, 0xBE6, 1, 0x419u
#define PPUTLIMPL_UHEX_0xBE5u 3045u, (B, E, 5), 0xBE5, 0xBE5, 1, 0x41Au
#define PPUTLIMPL_UHEX_0xBE4u 3044u, (B, E, 4), 0xBE4, 0xBE4, 1, 0x41Bu
#define PPUTLIMPL_UHEX_0xBE3u 3043u, (B, E, 3), 0xBE3, 0xBE3, 1, 0x41Cu
#define PPUTLIMPL_UHEX_0xBE2u 3042u, (B, E, 2), 0xBE2, 0xBE2, 1, 0x41Du
#define PPUTLIMPL_UHEX_0xBE1u 3041u, (B, E, 1), 0xBE1, 0xBE1, 1, 0x41Eu
#define PPUTLIMPL_UHEX_0xBE0u 3040u, (B, E, 0), 0xBE0, 0xBE0, 1, 0x41Fu
#define PPUTLIMPL_UHEX_0xBDFu 3039u, (B, D, F), 0xBDF, 0xBDF, 1, 0x420u
#define PPUTLIMPL_UHEX_0xBDEu 3038u, (B, D, E), 0xBDE, 0xBDE, 1, 0x421u
#define PPUTLIMPL_UHEX_0xBDDu 3037u, (B, D, D), 0xBDD, 0xBDD, 1, 0x422u
#define PPUTLIMPL_UHEX_0xBDCu 3036u, (B, D, C), 0xBDC, 0xBDC, 1, 0x423u
#define PPUTLIMPL_UHEX_0xBDBu 3035u, (B, D, B), 0xBDB, 0xBDB, 1, 0x424u
#define PPUTLIMPL_UHEX_0xBDAu 3034u, (B, D, A), 0xBDA, 0xBDA, 1, 0x425u
#define PPUTLIMPL_UHEX_0xBD9u 3033u, (B, D, 9), 0xBD9, 0xBD9, 1, 0x426u
#define PPUTLIMPL_UHEX_0xBD8u 3032u, (B, D, 8), 0xBD8, 0xBD8, 1, 0x427u
#define PPUTLIMPL_UHEX_0xBD7u 3031u, (B, D, 7), 0xBD7, 0xBD7, 1, 0x428u
#define PPUTLIMPL_UHEX_0xBD6u 3030u, (B, D, 6), 0xBD6, 0xBD6, 1, 0x429u
#define PPUTLIMPL_UHEX_0xBD5u 3029u, (B, D, 5), 0xBD5, 0xBD5, 1, 0x42Au
#define PPUTLIMPL_UHEX_0xBD4u 3028u, (B, D, 4), 0xBD4, 0xBD4, 1, 0x42Bu
#define PPUTLIMPL_UHEX_0xBD3u 3027u, (B, D, 3), 0xBD3, 0xBD3, 1, 0x42Cu
#define PPUTLIMPL_UHEX_0xBD2u 3026u, (B, D, 2), 0xBD2, 0xBD2, 1, 0x42Du
#define PPUTLIMPL_UHEX_0xBD1u 3025u, (B, D, 1), 0xBD1, 0xBD1, 1, 0x42Eu
#define PPUTLIMPL_UHEX_0xBD0u 3024u, (B, D, 0), 0xBD0, 0xBD0, 1, 0x42Fu
#define PPUTLIMPL_UHEX_0xBCFu 3023u, (B, C, F), 0xBCF, 0xBCF, 1, 0x430u
#define PPUTLIMPL_UHEX_0xBCEu 3022u, (B, C, E), 0xBCE, 0xBCE, 1, 0x431u
#define PPUTLIMPL_UHEX_0xBCDu 3021u, (B, C, D), 0xBCD, 0xBCD, 1, 0x432u
#define PPUTLIMPL_UHEX_0xBCCu 3020u, (B, C, C), 0xBCC, 0xBCC, 1, 0x433u
#define PPUTLIMPL_UHEX_0xBCBu 3019u, (B, C, B), 0xBCB, 0xBCB, 1, 0x434u
#define PPUTLIMPL_UHEX_0xBCAu 3018u, (B, C, A), 0xBCA, 0xBCA, 1, 0x435u
#define PPUTLIMPL_UHEX_0xBC9u 3017u, (B, C, 9), 0xBC9, 0xBC9, 1, 0x436u
#define PPUTLIMPL_UHEX_0xBC8u 3016u, (B, C, 8), 0xBC8, 0xBC8, 1, 0x437u
#define PPUTLIMPL_UHEX_0xBC7u 3015u, (B, C, 7), 0xBC7, 0xBC7, 1, 0x438u
#define PPUTLIMPL_UHEX_0xBC6u 3014u, (B, C, 6), 0xBC6, 0xBC6, 1, 0x439u
#define PPUTLIMPL_UHEX_0xBC5u 3013u, (B, C, 5), 0xBC5, 0xBC5, 1, 0x43Au
#define PPUTLIMPL_UHEX_0xBC4u 3012u, (B, C, 4), 0xBC4, 0xBC4, 1, 0x43Bu
#define PPUTLIMPL_UHEX_0xBC3u 3011u, (B, C, 3), 0xBC3, 0xBC3, 1, 0x43Cu
#define PPUTLIMPL_UHEX_0xBC2u 3010u, (B, C, 2), 0xBC2, 0xBC2, 1, 0x43Du
#define PPUTLIMPL_UHEX_0xBC1u 3009u, (B, C, 1), 0xBC1, 0xBC1, 1, 0x43Eu
#define PPUTLIMPL_UHEX_0xBC0u 3008u, (B, C, 0), 0xBC0, 0xBC0, 1, 0x43Fu
#define PPUTLIMPL_UHEX_0xBBFu 3007u, (B, B, F), 0xBBF, 0xBBF, 1, 0x440u
#define PPUTLIMPL_UHEX_0xBBEu 3006u, (B, B, E), 0xBBE, 0xBBE, 1, 0x441u
#define PPUTLIMPL_UHEX_0xBBDu 3005u, (B, B, D), 0xBBD, 0xBBD, 1, 0x442u
#define PPUTLIMPL_UHEX_0xBBCu 3004u, (B, B, C), 0xBBC, 0xBBC, 1, 0x443u
#define PPUTLIMPL_UHEX_0xBBBu 3003u, (B, B, B), 0xBBB, 0xBBB, 1, 0x444u
#define PPUTLIMPL_UHEX_0xBBAu 3002u, (B, B, A), 0xBBA, 0xBBA, 1, 0x445u
#define PPUTLIMPL_UHEX_0xBB9u 3001u, (B, B, 9), 0xBB9, 0xBB9, 1, 0x446u
#define PPUTLIMPL_UHEX_0xBB8u 3000u, (B, B, 8), 0xBB8, 0xBB8, 1, 0x447u
#define PPUTLIMPL_UHEX_0xBB7u 2999u, (B, B, 7), 0xBB7, 0xBB7, 1, 0x448u
#define PPUTLIMPL_UHEX_0xBB6u 2998u, (B, B, 6), 0xBB6, 0xBB6, 1, 0x449u
#define PPUTLIMPL_UHEX_0xBB5u 2997u, (B, B, 5), 0xBB5, 0xBB5, 1, 0x44Au
#define PPUTLIMPL_UHEX_0xBB4u 2996u, (B, B, 4), 0xBB4, 0xBB4, 1, 0x44Bu
#define PPUTLIMPL_UHEX_0xBB3u 2995u, (B, B, 3), 0xBB3, 0xBB3, 1, 0x44Cu
#define PPUTLIMPL_UHEX_0xBB2u 2994u, (B, B, 2), 0xBB2, 0xBB2, 1, 0x44Du
#define PPUTLIMPL_UHEX_0xBB1u 2993u, (B, B, 1), 0xBB1, 0xBB1, 1, 0x44Eu
#define PPUTLIMPL_UHEX_0xBB0u 2992u, (B, B, 0), 0xBB0, 0xBB0, 1, 0x44Fu
#define PPUTLIMPL_UHEX_0xBAFu 2991u, (B, A, F), 0xBAF, 0xBAF, 1, 0x450u
#define PPUTLIMPL_UHEX_0xBAEu 2990u, (B, A, E), 0xBAE, 0xBAE, 1, 0x451u
#define PPUTLIMPL_UHEX_0xBADu 2989u, (B, A, D), 0xBAD, 0xBAD, 1, 0x452u
#define PPUTLIMPL_UHEX_0xBACu 2988u, (B, A, C), 0xBAC, 0xBAC, 1, 0x453u
#define PPUTLIMPL_UHEX_0xBABu 2987u, (B, A, B), 0xBAB, 0xBAB, 1, 0x454u
#define PPUTLIMPL_UHEX_0xBAAu 2986u, (B, A, A), 0xBAA, 0xBAA, 1, 0x455u
#define PPUTLIMPL_UHEX_0xBA9u 2985u, (B, A, 9), 0xBA9, 0xBA9, 1, 0x456u
#define PPUTLIMPL_UHEX_0xBA8u 2984u, (B, A, 8), 0xBA8, 0xBA8, 1, 0x457u
#define PPUTLIMPL_UHEX_0xBA7u 2983u, (B, A, 7), 0xBA7, 0xBA7, 1, 0x458u
#define PPUTLIMPL_UHEX_0xBA6u 2982u, (B, A, 6), 0xBA6, 0xBA6, 1, 0x459u
#define PPUTLIMPL_UHEX_0xBA5u 2981u, (B, A, 5), 0xBA5, 0xBA5, 1, 0x45Au
#define PPUTLIMPL_UHEX_0xBA4u 2980u, (B, A, 4), 0xBA4, 0xBA4, 1, 0x45Bu
#define PPUTLIMPL_UHEX_0xBA3u 2979u, (B, A, 3), 0xBA3, 0xBA3, 1, 0x45Cu
#define PPUTLIMPL_UHEX_0xBA2u 2978u, (B, A, 2), 0xBA2, 0xBA2, 1, 0x45Du
#define PPUTLIMPL_UHEX_0xBA1u 2977u, (B, A, 1), 0xBA1, 0xBA1, 1, 0x45Eu
#define PPUTLIMPL_UHEX_0xBA0u 2976u, (B, A, 0), 0xBA0, 0xBA0, 1, 0x45Fu
#define PPUTLIMPL_UHEX_0xB9Fu 2975u, (B, 9, F), 0xB9F, 0xB9F, 1, 0x460u
#define PPUTLIMPL_UHEX_0xB9Eu 2974u, (B, 9, E), 0xB9E, 0xB9E, 1, 0x461u
#define PPUTLIMPL_UHEX_0xB9Du 2973u, (B, 9, D), 0xB9D, 0xB9D, 1, 0x462u
#define PPUTLIMPL_UHEX_0xB9Cu 2972u, (B, 9, C), 0xB9C, 0xB9C, 1, 0x463u
#define PPUTLIMPL_UHEX_0xB9Bu 2971u, (B, 9, B), 0xB9B, 0xB9B, 1, 0x464u
#define PPUTLIMPL_UHEX_0xB9Au 2970u, (B, 9, A), 0xB9A, 0xB9A, 1, 0x465u
#define PPUTLIMPL_UHEX_0xB99u 2969u, (B, 9, 9), 0xB99, 0xB99, 1, 0x466u
#define PPUTLIMPL_UHEX_0xB98u 2968u, (B, 9, 8), 0xB98, 0xB98, 1, 0x467u
#define PPUTLIMPL_UHEX_0xB97u 2967u, (B, 9, 7), 0xB97, 0xB97, 1, 0x468u
#define PPUTLIMPL_UHEX_0xB96u 2966u, (B, 9, 6), 0xB96, 0xB96, 1, 0x469u
#define PPUTLIMPL_UHEX_0xB95u 2965u, (B, 9, 5), 0xB95, 0xB95, 1, 0x46Au
#define PPUTLIMPL_UHEX_0xB94u 2964u, (B, 9, 4), 0xB94, 0xB94, 1, 0x46Bu
#define PPUTLIMPL_UHEX_0xB93u 2963u, (B, 9, 3), 0xB93, 0xB93, 1, 0x46Cu
#define PPUTLIMPL_UHEX_0xB92u 2962u, (B, 9, 2), 0xB92, 0xB92, 1, 0x46Du
#define PPUTLIMPL_UHEX_0xB91u 2961u, (B, 9, 1), 0xB91, 0xB91, 1, 0x46Eu
#define PPUTLIMPL_UHEX_0xB90u 2960u, (B, 9, 0), 0xB90, 0xB90, 1, 0x46Fu
#define PPUTLIMPL_UHEX_0xB8Fu 2959u, (B, 8, F), 0xB8F, 0xB8F, 1, 0x470u
#define PPUTLIMPL_UHEX_0xB8Eu 2958u, (B, 8, E), 0xB8E, 0xB8E, 1, 0x471u
#define PPUTLIMPL_UHEX_0xB8Du 2957u, (B, 8, D), 0xB8D, 0xB8D, 1, 0x472u
#define PPUTLIMPL_UHEX_0xB8Cu 2956u, (B, 8, C), 0xB8C, 0xB8C, 1, 0x473u
#define PPUTLIMPL_UHEX_0xB8Bu 2955u, (B, 8, B), 0xB8B, 0xB8B, 1, 0x474u
#define PPUTLIMPL_UHEX_0xB8Au 2954u, (B, 8, A), 0xB8A, 0xB8A, 1, 0x475u
#define PPUTLIMPL_UHEX_0xB89u 2953u, (B, 8, 9), 0xB89, 0xB89, 1, 0x476u
#define PPUTLIMPL_UHEX_0xB88u 2952u, (B, 8, 8), 0xB88, 0xB88, 1, 0x477u
#define PPUTLIMPL_UHEX_0xB87u 2951u, (B, 8, 7), 0xB87, 0xB87, 1, 0x478u
#define PPUTLIMPL_UHEX_0xB86u 2950u, (B, 8, 6), 0xB86, 0xB86, 1, 0x479u
#define PPUTLIMPL_UHEX_0xB85u 2949u, (B, 8, 5), 0xB85, 0xB85, 1, 0x47Au
#define PPUTLIMPL_UHEX_0xB84u 2948u, (B, 8, 4), 0xB84, 0xB84, 1, 0x47Bu
#define PPUTLIMPL_UHEX_0xB83u 2947u, (B, 8, 3), 0xB83, 0xB83, 1, 0x47Cu
#define PPUTLIMPL_UHEX_0xB82u 2946u, (B, 8, 2), 0xB82, 0xB82, 1, 0x47Du
#define PPUTLIMPL_UHEX_0xB81u 2945u, (B, 8, 1), 0xB81, 0xB81, 1, 0x47Eu
#define PPUTLIMPL_UHEX_0xB80u 2944u, (B, 8, 0), 0xB80, 0xB80, 1, 0x47Fu
#define PPUTLIMPL_UHEX_0xB7Fu 2943u, (B, 7, F), 0xB7F, 0xB7F, 1, 0x480u
#define PPUTLIMPL_UHEX_0xB7Eu 2942u, (B, 7, E), 0xB7E, 0xB7E, 1, 0x481u
#define PPUTLIMPL_UHEX_0xB7Du 2941u, (B, 7, D), 0xB7D, 0xB7D, 1, 0x482u
#define PPUTLIMPL_UHEX_0xB7Cu 2940u, (B, 7, C), 0xB7C, 0xB7C, 1, 0x483u
#define PPUTLIMPL_UHEX_0xB7Bu 2939u, (B, 7, B), 0xB7B, 0xB7B, 1, 0x484u
#define PPUTLIMPL_UHEX_0xB7Au 2938u, (B, 7, A), 0xB7A, 0xB7A, 1, 0x485u
#define PPUTLIMPL_UHEX_0xB79u 2937u, (B, 7, 9), 0xB79, 0xB79, 1, 0x486u
#define PPUTLIMPL_UHEX_0xB78u 2936u, (B, 7, 8), 0xB78, 0xB78, 1, 0x487u
#define PPUTLIMPL_UHEX_0xB77u 2935u, (B, 7, 7), 0xB77, 0xB77, 1, 0x488u
#define PPUTLIMPL_UHEX_0xB76u 2934u, (B, 7, 6), 0xB76, 0xB76, 1, 0x489u
#define PPUTLIMPL_UHEX_0xB75u 2933u, (B, 7, 5), 0xB75, 0xB75, 1, 0x48Au
#define PPUTLIMPL_UHEX_0xB74u 2932u, (B, 7, 4), 0xB74, 0xB74, 1, 0x48Bu
#define PPUTLIMPL_UHEX_0xB73u 2931u, (B, 7, 3), 0xB73, 0xB73, 1, 0x48Cu
#define PPUTLIMPL_UHEX_0xB72u 2930u, (B, 7, 2), 0xB72, 0xB72, 1, 0x48Du
#define PPUTLIMPL_UHEX_0xB71u 2929u, (B, 7, 1), 0xB71, 0xB71, 1, 0x48Eu
#define PPUTLIMPL_UHEX_0xB70u 2928u, (B, 7, 0), 0xB70, 0xB70, 1, 0x48Fu
#define PPUTLIMPL_UHEX_0xB6Fu 2927u, (B, 6, F), 0xB6F, 0xB6F, 1, 0x490u
#define PPUTLIMPL_UHEX_0xB6Eu 2926u, (B, 6, E), 0xB6E, 0xB6E, 1, 0x491u
#define PPUTLIMPL_UHEX_0xB6Du 2925u, (B, 6, D), 0xB6D, 0xB6D, 1, 0x492u
#define PPUTLIMPL_UHEX_0xB6Cu 2924u, (B, 6, C), 0xB6C, 0xB6C, 1, 0x493u
#define PPUTLIMPL_UHEX_0xB6Bu 2923u, (B, 6, B), 0xB6B, 0xB6B, 1, 0x494u
#define PPUTLIMPL_UHEX_0xB6Au 2922u, (B, 6, A), 0xB6A, 0xB6A, 1, 0x495u
#define PPUTLIMPL_UHEX_0xB69u 2921u, (B, 6, 9), 0xB69, 0xB69, 1, 0x496u
#define PPUTLIMPL_UHEX_0xB68u 2920u, (B, 6, 8), 0xB68, 0xB68, 1, 0x497u
#define PPUTLIMPL_UHEX_0xB67u 2919u, (B, 6, 7), 0xB67, 0xB67, 1, 0x498u
#define PPUTLIMPL_UHEX_0xB66u 2918u, (B, 6, 6), 0xB66, 0xB66, 1, 0x499u
#define PPUTLIMPL_UHEX_0xB65u 2917u, (B, 6, 5), 0xB65, 0xB65, 1, 0x49Au
#define PPUTLIMPL_UHEX_0xB64u 2916u, (B, 6, 4), 0xB64, 0xB64, 1, 0x49Bu
#define PPUTLIMPL_UHEX_0xB63u 2915u, (B, 6, 3), 0xB63, 0xB63, 1, 0x49Cu
#define PPUTLIMPL_UHEX_0xB62u 2914u, (B, 6, 2), 0xB62, 0xB62, 1, 0x49Du
#define PPUTLIMPL_UHEX_0xB61u 2913u, (B, 6, 1), 0xB61, 0xB61, 1, 0x49Eu
#define PPUTLIMPL_UHEX_0xB60u 2912u, (B, 6, 0), 0xB60, 0xB60, 1, 0x49Fu
#define PPUTLIMPL_UHEX_0xB5Fu 2911u, (B, 5, F), 0xB5F, 0xB5F, 1, 0x4A0u
#define PPUTLIMPL_UHEX_0xB5Eu 2910u, (B, 5, E), 0xB5E, 0xB5E, 1, 0x4A1u
#define PPUTLIMPL_UHEX_0xB5Du 2909u, (B, 5, D), 0xB5D, 0xB5D, 1, 0x4A2u
#define PPUTLIMPL_UHEX_0xB5Cu 2908u, (B, 5, C), 0xB5C, 0xB5C, 1, 0x4A3u
#define PPUTLIMPL_UHEX_0xB5Bu 2907u, (B, 5, B), 0xB5B, 0xB5B, 1, 0x4A4u
#define PPUTLIMPL_UHEX_0xB5Au 2906u, (B, 5, A), 0xB5A, 0xB5A, 1, 0x4A5u
#define PPUTLIMPL_UHEX_0xB59u 2905u, (B, 5, 9), 0xB59, 0xB59, 1, 0x4A6u
#define PPUTLIMPL_UHEX_0xB58u 2904u, (B, 5, 8), 0xB58, 0xB58, 1, 0x4A7u
#define PPUTLIMPL_UHEX_0xB57u 2903u, (B, 5, 7), 0xB57, 0xB57, 1, 0x4A8u
#define PPUTLIMPL_UHEX_0xB56u 2902u, (B, 5, 6), 0xB56, 0xB56, 1, 0x4A9u
#define PPUTLIMPL_UHEX_0xB55u 2901u, (B, 5, 5), 0xB55, 0xB55, 1, 0x4AAu
#define PPUTLIMPL_UHEX_0xB54u 2900u, (B, 5, 4), 0xB54, 0xB54, 1, 0x4ABu
#define PPUTLIMPL_UHEX_0xB53u 2899u, (B, 5, 3), 0xB53, 0xB53, 1, 0x4ACu
#define PPUTLIMPL_UHEX_0xB52u 2898u, (B, 5, 2), 0xB52, 0xB52, 1, 0x4ADu
#define PPUTLIMPL_UHEX_0xB51u 2897u, (B, 5, 1), 0xB51, 0xB51, 1, 0x4AEu
#define PPUTLIMPL_UHEX_0xB50u 2896u, (B, 5, 0), 0xB50, 0xB50, 1, 0x4AFu
#define PPUTLIMPL_UHEX_0xB4Fu 2895u, (B, 4, F), 0xB4F, 0xB4F, 1, 0x4B0u
#define PPUTLIMPL_UHEX_0xB4Eu 2894u, (B, 4, E), 0xB4E, 0xB4E, 1, 0x4B1u
#define PPUTLIMPL_UHEX_0xB4Du 2893u, (B, 4, D), 0xB4D, 0xB4D, 1, 0x4B2u
#define PPUTLIMPL_UHEX_0xB4Cu 2892u, (B, 4, C), 0xB4C, 0xB4C, 1, 0x4B3u
#define PPUTLIMPL_UHEX_0xB4Bu 2891u, (B, 4, B), 0xB4B, 0xB4B, 1, 0x4B4u
#define PPUTLIMPL_UHEX_0xB4Au 2890u, (B, 4, A), 0xB4A, 0xB4A, 1, 0x4B5u
#define PPUTLIMPL_UHEX_0xB49u 2889u, (B, 4, 9), 0xB49, 0xB49, 1, 0x4B6u
#define PPUTLIMPL_UHEX_0xB48u 2888u, (B, 4, 8), 0xB48, 0xB48, 1, 0x4B7u
#define PPUTLIMPL_UHEX_0xB47u 2887u, (B, 4, 7), 0xB47, 0xB47, 1, 0x4B8u
#define PPUTLIMPL_UHEX_0xB46u 2886u, (B, 4, 6), 0xB46, 0xB46, 1, 0x4B9u
#define PPUTLIMPL_UHEX_0xB45u 2885u, (B, 4, 5), 0xB45, 0xB45, 1, 0x4BAu
#define PPUTLIMPL_UHEX_0xB44u 2884u, (B, 4, 4), 0xB44, 0xB44, 1, 0x4BBu
#define PPUTLIMPL_UHEX_0xB43u 2883u, (B, 4, 3), 0xB43, 0xB43, 1, 0x4BCu
#define PPUTLIMPL_UHEX_0xB42u 2882u, (B, 4, 2), 0xB42, 0xB42, 1, 0x4BDu
#define PPUTLIMPL_UHEX_0xB41u 2881u, (B, 4, 1), 0xB41, 0xB41, 1, 0x4BEu
#define PPUTLIMPL_UHEX_0xB40u 2880u, (B, 4, 0), 0xB40, 0xB40, 1, 0x4BFu
#define PPUTLIMPL_UHEX_0xB3Fu 2879u, (B, 3, F), 0xB3F, 0xB3F, 1, 0x4C0u
#define PPUTLIMPL_UHEX_0xB3Eu 2878u, (B, 3, E), 0xB3E, 0xB3E, 1, 0x4C1u
#define PPUTLIMPL_UHEX_0xB3Du 2877u, (B, 3, D), 0xB3D, 0xB3D, 1, 0x4C2u
#define PPUTLIMPL_UHEX_0xB3Cu 2876u, (B, 3, C), 0xB3C, 0xB3C, 1, 0x4C3u
#define PPUTLIMPL_UHEX_0xB3Bu 2875u, (B, 3, B), 0xB3B, 0xB3B, 1, 0x4C4u
#define PPUTLIMPL_UHEX_0xB3Au 2874u, (B, 3, A), 0xB3A, 0xB3A, 1, 0x4C5u
#define PPUTLIMPL_UHEX_0xB39u 2873u, (B, 3, 9), 0xB39, 0xB39, 1, 0x4C6u
#define PPUTLIMPL_UHEX_0xB38u 2872u, (B, 3, 8), 0xB38, 0xB38, 1, 0x4C7u
#define PPUTLIMPL_UHEX_0xB37u 2871u, (B, 3, 7), 0xB37, 0xB37, 1, 0x4C8u
#define PPUTLIMPL_UHEX_0xB36u 2870u, (B, 3, 6), 0xB36, 0xB36, 1, 0x4C9u
#define PPUTLIMPL_UHEX_0xB35u 2869u, (B, 3, 5), 0xB35, 0xB35, 1, 0x4CAu
#define PPUTLIMPL_UHEX_0xB34u 2868u, (B, 3, 4), 0xB34, 0xB34, 1, 0x4CBu
#define PPUTLIMPL_UHEX_0xB33u 2867u, (B, 3, 3), 0xB33, 0xB33, 1, 0x4CCu
#define PPUTLIMPL_UHEX_0xB32u 2866u, (B, 3, 2), 0xB32, 0xB32, 1, 0x4CDu
#define PPUTLIMPL_UHEX_0xB31u 2865u, (B, 3, 1), 0xB31, 0xB31, 1, 0x4CEu
#define PPUTLIMPL_UHEX_0xB30u 2864u, (B, 3, 0), 0xB30, 0xB30, 1, 0x4CFu
#define PPUTLIMPL_UHEX_0xB2Fu 2863u, (B, 2, F), 0xB2F, 0xB2F, 1, 0x4D0u
#define PPUTLIMPL_UHEX_0xB2Eu 2862u, (B, 2, E), 0xB2E, 0xB2E, 1, 0x4D1u
#define PPUTLIMPL_UHEX_0xB2Du 2861u, (B, 2, D), 0xB2D, 0xB2D, 1, 0x4D2u
#define PPUTLIMPL_UHEX_0xB2Cu 2860u, (B, 2, C), 0xB2C, 0xB2C, 1, 0x4D3u
#define PPUTLIMPL_UHEX_0xB2Bu 2859u, (B, 2, B), 0xB2B, 0xB2B, 1, 0x4D4u
#define PPUTLIMPL_UHEX_0xB2Au 2858u, (B, 2, A), 0xB2A, 0xB2A, 1, 0x4D5u
#define PPUTLIMPL_UHEX_0xB29u 2857u, (B, 2, 9), 0xB29, 0xB29, 1, 0x4D6u
#define PPUTLIMPL_UHEX_0xB28u 2856u, (B, 2, 8), 0xB28, 0xB28, 1, 0x4D7u
#define PPUTLIMPL_UHEX_0xB27u 2855u, (B, 2, 7), 0xB27, 0xB27, 1, 0x4D8u
#define PPUTLIMPL_UHEX_0xB26u 2854u, (B, 2, 6), 0xB26, 0xB26, 1, 0x4D9u
#define PPUTLIMPL_UHEX_0xB25u 2853u, (B, 2, 5), 0xB25, 0xB25, 1, 0x4DAu
#define PPUTLIMPL_UHEX_0xB24u 2852u, (B, 2, 4), 0xB24, 0xB24, 1, 0x4DBu
#define PPUTLIMPL_UHEX_0xB23u 2851u, (B, 2, 3), 0xB23, 0xB23, 1, 0x4DCu
#define PPUTLIMPL_UHEX_0xB22u 2850u, (B, 2, 2), 0xB22, 0xB22, 1, 0x4DDu
#define PPUTLIMPL_UHEX_0xB21u 2849u, (B, 2, 1), 0xB21, 0xB21, 1, 0x4DEu
#define PPUTLIMPL_UHEX_0xB20u 2848u, (B, 2, 0), 0xB20, 0xB20, 1, 0x4DFu
#define PPUTLIMPL_UHEX_0xB1Fu 2847u, (B, 1, F), 0xB1F, 0xB1F, 1, 0x4E0u
#define PPUTLIMPL_UHEX_0xB1Eu 2846u, (B, 1, E), 0xB1E, 0xB1E, 1, 0x4E1u
#define PPUTLIMPL_UHEX_0xB1Du 2845u, (B, 1, D), 0xB1D, 0xB1D, 1, 0x4E2u
#define PPUTLIMPL_UHEX_0xB1Cu 2844u, (B, 1, C), 0xB1C, 0xB1C, 1, 0x4E3u
#define PPUTLIMPL_UHEX_0xB1Bu 2843u, (B, 1, B), 0xB1B, 0xB1B, 1, 0x4E4u
#define PPUTLIMPL_UHEX_0xB1Au 2842u, (B, 1, A), 0xB1A, 0xB1A, 1, 0x4E5u
#define PPUTLIMPL_UHEX_0xB19u 2841u, (B, 1, 9), 0xB19, 0xB19, 1, 0x4E6u
#define PPUTLIMPL_UHEX_0xB18u 2840u, (B, 1, 8), 0xB18, 0xB18, 1, 0x4E7u
#define PPUTLIMPL_UHEX_0xB17u 2839u, (B, 1, 7), 0xB17, 0xB17, 1, 0x4E8u
#define PPUTLIMPL_UHEX_0xB16u 2838u, (B, 1, 6), 0xB16, 0xB16, 1, 0x4E9u
#define PPUTLIMPL_UHEX_0xB15u 2837u, (B, 1, 5), 0xB15, 0xB15, 1, 0x4EAu
#define PPUTLIMPL_UHEX_0xB14u 2836u, (B, 1, 4), 0xB14, 0xB14, 1, 0x4EBu
#define PPUTLIMPL_UHEX_0xB13u 2835u, (B, 1, 3), 0xB13, 0xB13, 1, 0x4ECu
#define PPUTLIMPL_UHEX_0xB12u 2834u, (B, 1, 2), 0xB12, 0xB12, 1, 0x4EDu
#define PPUTLIMPL_UHEX_0xB11u 2833u, (B, 1, 1), 0xB11, 0xB11, 1, 0x4EEu
#define PPUTLIMPL_UHEX_0xB10u 2832u, (B, 1, 0), 0xB10, 0xB10, 1, 0x4EFu
#define PPUTLIMPL_UHEX_0xB0Fu 2831u, (B, 0, F), 0xB0F, 0xB0F, 1, 0x4F0u
#define PPUTLIMPL_UHEX_0xB0Eu 2830u, (B, 0, E), 0xB0E, 0xB0E, 1, 0x4F1u
#define PPUTLIMPL_UHEX_0xB0Du 2829u, (B, 0, D), 0xB0D, 0xB0D, 1, 0x4F2u
#define PPUTLIMPL_UHEX_0xB0Cu 2828u, (B, 0, C), 0xB0C, 0xB0C, 1, 0x4F3u
#define PPUTLIMPL_UHEX_0xB0Bu 2827u, (B, 0, B), 0xB0B, 0xB0B, 1, 0x4F4u
#define PPUTLIMPL_UHEX_0xB0Au 2826u, (B, 0, A), 0xB0A, 0xB0A, 1, 0x4F5u
#define PPUTLIMPL_UHEX_0xB09u 2825u, (B, 0, 9), 0xB09, 0xB09, 1, 0x4F6u
#define PPUTLIMPL_UHEX_0xB08u 2824u, (B, 0, 8), 0xB08, 0xB08, 1, 0x4F7u
#define PPUTLIMPL_UHEX_0xB07u 2823u, (B, 0, 7), 0xB07, 0xB07, 1, 0x4F8u
#define PPUTLIMPL_UHEX_0xB06u 2822u, (B, 0, 6), 0xB06, 0xB06, 1, 0x4F9u
#define PPUTLIMPL_UHEX_0xB05u 2821u, (B, 0, 5), 0xB05, 0xB05, 1, 0x4FAu
#define PPUTLIMPL_UHEX_0xB04u 2820u, (B, 0, 4), 0xB04, 0xB04, 1, 0x4FBu
#define PPUTLIMPL_UHEX_0xB03u 2819u, (B, 0, 3), 0xB03, 0xB03, 1, 0x4FCu
#define PPUTLIMPL_UHEX_0xB02u 2818u, (B, 0, 2), 0xB02, 0xB02, 1, 0x4FDu
#define PPUTLIMPL_UHEX_0xB01u 2817u, (B, 0, 1), 0xB01, 0xB01, 1, 0x4FEu
#define PPUTLIMPL_UHEX_0xB00u 2816u, (B, 0, 0), 0xB00, 0xB00, 1, 0x4FFu
#define PPUTLIMPL_UHEX_0xAFFu 2815u, (A, F, F), 0xAFF, 0xAFF, 1, 0x500u
#define PPUTLIMPL_UHEX_0xAFEu 2814u, (A, F, E), 0xAFE, 0xAFE, 1, 0x501u
#define PPUTLIMPL_UHEX_0xAFDu 2813u, (A, F, D), 0xAFD, 0xAFD, 1, 0x502u
#define PPUTLIMPL_UHEX_0xAFCu 2812u, (A, F, C), 0xAFC, 0xAFC, 1, 0x503u
#define PPUTLIMPL_UHEX_0xAFBu 2811u, (A, F, B), 0xAFB, 0xAFB, 1, 0x504u
#define PPUTLIMPL_UHEX_0xAFAu 2810u, (A, F, A), 0xAFA, 0xAFA, 1, 0x505u
#define PPUTLIMPL_UHEX_0xAF9u 2809u, (A, F, 9), 0xAF9, 0xAF9, 1, 0x506u
#define PPUTLIMPL_UHEX_0xAF8u 2808u, (A, F, 8), 0xAF8, 0xAF8, 1, 0x507u
#define PPUTLIMPL_UHEX_0xAF7u 2807u, (A, F, 7), 0xAF7, 0xAF7, 1, 0x508u
#define PPUTLIMPL_UHEX_0xAF6u 2806u, (A, F, 6), 0xAF6, 0xAF6, 1, 0x509u
#define PPUTLIMPL_UHEX_0xAF5u 2805u, (A, F, 5), 0xAF5, 0xAF5, 1, 0x50Au
#define PPUTLIMPL_UHEX_0xAF4u 2804u, (A, F, 4), 0xAF4, 0xAF4, 1, 0x50Bu
#define PPUTLIMPL_UHEX_0xAF3u 2803u, (A, F, 3), 0xAF3, 0xAF3, 1, 0x50Cu
#define PPUTLIMPL_UHEX_0xAF2u 2802u, (A, F, 2), 0xAF2, 0xAF2, 1, 0x50Du
#define PPUTLIMPL_UHEX_0xAF1u 2801u, (A, F, 1), 0xAF1, 0xAF1, 1, 0x50Eu
#define PPUTLIMPL_UHEX_0xAF0u 2800u, (A, F, 0), 0xAF0, 0xAF0, 1, 0x50Fu
#define PPUTLIMPL_UHEX_0xAEFu 2799u, (A, E, F), 0xAEF, 0xAEF, 1, 0x510u
#define PPUTLIMPL_UHEX_0xAEEu 2798u, (A, E, E), 0xAEE, 0xAEE, 1, 0x511u
#define PPUTLIMPL_UHEX_0xAEDu 2797u, (A, E, D), 0xAED, 0xAED, 1, 0x512u
#define PPUTLIMPL_UHEX_0xAECu 2796u, (A, E, C), 0xAEC, 0xAEC, 1, 0x513u
#define PPUTLIMPL_UHEX_0xAEBu 2795u, (A, E, B), 0xAEB, 0xAEB, 1, 0x514u
#define PPUTLIMPL_UHEX_0xAEAu 2794u, (A, E, A), 0xAEA, 0xAEA, 1, 0x515u
#define PPUTLIMPL_UHEX_0xAE9u 2793u, (A, E, 9), 0xAE9, 0xAE9, 1, 0x516u
#define PPUTLIMPL_UHEX_0xAE8u 2792u, (A, E, 8), 0xAE8, 0xAE8, 1, 0x517u
#define PPUTLIMPL_UHEX_0xAE7u 2791u, (A, E, 7), 0xAE7, 0xAE7, 1, 0x518u
#define PPUTLIMPL_UHEX_0xAE6u 2790u, (A, E, 6), 0xAE6, 0xAE6, 1, 0x519u
#define PPUTLIMPL_UHEX_0xAE5u 2789u, (A, E, 5), 0xAE5, 0xAE5, 1, 0x51Au
#define PPUTLIMPL_UHEX_0xAE4u 2788u, (A, E, 4), 0xAE4, 0xAE4, 1, 0x51Bu
#define PPUTLIMPL_UHEX_0xAE3u 2787u, (A, E, 3), 0xAE3, 0xAE3, 1, 0x51Cu
#define PPUTLIMPL_UHEX_0xAE2u 2786u, (A, E, 2), 0xAE2, 0xAE2, 1, 0x51Du
#define PPUTLIMPL_UHEX_0xAE1u 2785u, (A, E, 1), 0xAE1, 0xAE1, 1, 0x51Eu
#define PPUTLIMPL_UHEX_0xAE0u 2784u, (A, E, 0), 0xAE0, 0xAE0, 1, 0x51Fu
#define PPUTLIMPL_UHEX_0xADFu 2783u, (A, D, F), 0xADF, 0xADF, 1, 0x520u
#define PPUTLIMPL_UHEX_0xADEu 2782u, (A, D, E), 0xADE, 0xADE, 1, 0x521u
#define PPUTLIMPL_UHEX_0xADDu 2781u, (A, D, D), 0xADD, 0xADD, 1, 0x522u
#define PPUTLIMPL_UHEX_0xADCu 2780u, (A, D, C), 0xADC, 0xADC, 1, 0x523u
#define PPUTLIMPL_UHEX_0xADBu 2779u, (A, D, B), 0xADB, 0xADB, 1, 0x524u
#define PPUTLIMPL_UHEX_0xADAu 2778u, (A, D, A), 0xADA, 0xADA, 1, 0x525u
#define PPUTLIMPL_UHEX_0xAD9u 2777u, (A, D, 9), 0xAD9, 0xAD9, 1, 0x526u
#define PPUTLIMPL_UHEX_0xAD8u 2776u, (A, D, 8), 0xAD8, 0xAD8, 1, 0x527u
#define PPUTLIMPL_UHEX_0xAD7u 2775u, (A, D, 7), 0xAD7, 0xAD7, 1, 0x528u
#define PPUTLIMPL_UHEX_0xAD6u 2774u, (A, D, 6), 0xAD6, 0xAD6, 1, 0x529u
#define PPUTLIMPL_UHEX_0xAD5u 2773u, (A, D, 5), 0xAD5, 0xAD5, 1, 0x52Au
#define PPUTLIMPL_UHEX_0xAD4u 2772u, (A, D, 4), 0xAD4, 0xAD4, 1, 0x52Bu
#define PPUTLIMPL_UHEX_0xAD3u 2771u, (A, D, 3), 0xAD3, 0xAD3, 1, 0x52Cu
#define PPUTLIMPL_UHEX_0xAD2u 2770u, (A, D, 2), 0xAD2, 0xAD2, 1, 0x52Du
#define PPUTLIMPL_UHEX_0xAD1u 2769u, (A, D, 1), 0xAD1, 0xAD1, 1, 0x52Eu
#define PPUTLIMPL_UHEX_0xAD0u 2768u, (A, D, 0), 0xAD0, 0xAD0, 1, 0x52Fu
#define PPUTLIMPL_UHEX_0xACFu 2767u, (A, C, F), 0xACF, 0xACF, 1, 0x530u
#define PPUTLIMPL_UHEX_0xACEu 2766u, (A, C, E), 0xACE, 0xACE, 1, 0x531u
#define PPUTLIMPL_UHEX_0xACDu 2765u, (A, C, D), 0xACD, 0xACD, 1, 0x532u
#define PPUTLIMPL_UHEX_0xACCu 2764u, (A, C, C), 0xACC, 0xACC, 1, 0x533u
#define PPUTLIMPL_UHEX_0xACBu 2763u, (A, C, B), 0xACB, 0xACB, 1, 0x534u
#define PPUTLIMPL_UHEX_0xACAu 2762u, (A, C, A), 0xACA, 0xACA, 1, 0x535u
#define PPUTLIMPL_UHEX_0xAC9u 2761u, (A, C, 9), 0xAC9, 0xAC9, 1, 0x536u
#define PPUTLIMPL_UHEX_0xAC8u 2760u, (A, C, 8), 0xAC8, 0xAC8, 1, 0x537u
#define PPUTLIMPL_UHEX_0xAC7u 2759u, (A, C, 7), 0xAC7, 0xAC7, 1, 0x538u
#define PPUTLIMPL_UHEX_0xAC6u 2758u, (A, C, 6), 0xAC6, 0xAC6, 1, 0x539u
#define PPUTLIMPL_UHEX_0xAC5u 2757u, (A, C, 5), 0xAC5, 0xAC5, 1, 0x53Au
#define PPUTLIMPL_UHEX_0xAC4u 2756u, (A, C, 4), 0xAC4, 0xAC4, 1, 0x53Bu
#define PPUTLIMPL_UHEX_0xAC3u 2755u, (A, C, 3), 0xAC3, 0xAC3, 1, 0x53Cu
#define PPUTLIMPL_UHEX_0xAC2u 2754u, (A, C, 2), 0xAC2, 0xAC2, 1, 0x53Du
#define PPUTLIMPL_UHEX_0xAC1u 2753u, (A, C, 1), 0xAC1, 0xAC1, 1, 0x53Eu
#define PPUTLIMPL_UHEX_0xAC0u 2752u, (A, C, 0), 0xAC0, 0xAC0, 1, 0x53Fu
#define PPUTLIMPL_UHEX_0xABFu 2751u, (A, B, F), 0xABF, 0xABF, 1, 0x540u
#define PPUTLIMPL_UHEX_0xABEu 2750u, (A, B, E), 0xABE, 0xABE, 1, 0x541u
#define PPUTLIMPL_UHEX_0xABDu 2749u, (A, B, D), 0xABD, 0xABD, 1, 0x542u
#define PPUTLIMPL_UHEX_0xABCu 2748u, (A, B, C), 0xABC, 0xABC, 1, 0x543u
#define PPUTLIMPL_UHEX_0xABBu 2747u, (A, B, B), 0xABB, 0xABB, 1, 0x544u
#define PPUTLIMPL_UHEX_0xABAu 2746u, (A, B, A), 0xABA, 0xABA, 1, 0x545u
#define PPUTLIMPL_UHEX_0xAB9u 2745u, (A, B, 9), 0xAB9, 0xAB9, 1, 0x546u
#define PPUTLIMPL_UHEX_0xAB8u 2744u, (A, B, 8), 0xAB8, 0xAB8, 1, 0x547u
#define PPUTLIMPL_UHEX_0xAB7u 2743u, (A, B, 7), 0xAB7, 0xAB7, 1, 0x548u
#define PPUTLIMPL_UHEX_0xAB6u 2742u, (A, B, 6), 0xAB6, 0xAB6, 1, 0x549u
#define PPUTLIMPL_UHEX_0xAB5u 2741u, (A, B, 5), 0xAB5, 0xAB5, 1, 0x54Au
#define PPUTLIMPL_UHEX_0xAB4u 2740u, (A, B, 4), 0xAB4, 0xAB4, 1, 0x54Bu
#define PPUTLIMPL_UHEX_0xAB3u 2739u, (A, B, 3), 0xAB3, 0xAB3, 1, 0x54Cu
#define PPUTLIMPL_UHEX_0xAB2u 2738u, (A, B, 2), 0xAB2, 0xAB2, 1, 0x54Du
#define PPUTLIMPL_UHEX_0xAB1u 2737u, (A, B, 1), 0xAB1, 0xAB1, 1, 0x54Eu
#define PPUTLIMPL_UHEX_0xAB0u 2736u, (A, B, 0), 0xAB0, 0xAB0, 1, 0x54Fu
#define PPUTLIMPL_UHEX_0xAAFu 2735u, (A, A, F), 0xAAF, 0xAAF, 1, 0x550u
#define PPUTLIMPL_UHEX_0xAAEu 2734u, (A, A, E), 0xAAE, 0xAAE, 1, 0x551u
#define PPUTLIMPL_UHEX_0xAADu 2733u, (A, A, D), 0xAAD, 0xAAD, 1, 0x552u
#define PPUTLIMPL_UHEX_0xAACu 2732u, (A, A, C), 0xAAC, 0xAAC, 1, 0x553u
#define PPUTLIMPL_UHEX_0xAABu 2731u, (A, A, B), 0xAAB, 0xAAB, 1, 0x554u
#define PPUTLIMPL_UHEX_0xAAAu 2730u, (A, A, A), 0xAAA, 0xAAA, 1, 0x555u
#define PPUTLIMPL_UHEX_0xAA9u 2729u, (A, A, 9), 0xAA9, 0xAA9, 1, 0x556u
#define PPUTLIMPL_UHEX_0xAA8u 2728u, (A, A, 8), 0xAA8, 0xAA8, 1, 0x557u
#define PPUTLIMPL_UHEX_0xAA7u 2727u, (A, A, 7), 0xAA7, 0xAA7, 1, 0x558u
#define PPUTLIMPL_UHEX_0xAA6u 2726u, (A, A, 6), 0xAA6, 0xAA6, 1, 0x559u
#define PPUTLIMPL_UHEX_0xAA5u 2725u, (A, A, 5), 0xAA5, 0xAA5, 1, 0x55Au
#define PPUTLIMPL_UHEX_0xAA4u 2724u, (A, A, 4), 0xAA4, 0xAA4, 1, 0x55Bu
#define PPUTLIMPL_UHEX_0xAA3u 2723u, (A, A, 3), 0xAA3, 0xAA3, 1, 0x55Cu
#define PPUTLIMPL_UHEX_0xAA2u 2722u, (A, A, 2), 0xAA2, 0xAA2, 1, 0x55Du
#define PPUTLIMPL_UHEX_0xAA1u 2721u, (A, A, 1), 0xAA1, 0xAA1, 1, 0x55Eu
#define PPUTLIMPL_UHEX_0xAA0u 2720u, (A, A, 0), 0xAA0, 0xAA0, 1, 0x55Fu
#define PPUTLIMPL_UHEX_0xA9Fu 2719u, (A, 9, F), 0xA9F, 0xA9F, 1, 0x560u
#define PPUTLIMPL_UHEX_0xA9Eu 2718u, (A, 9, E), 0xA9E, 0xA9E, 1, 0x561u
#define PPUTLIMPL_UHEX_0xA9Du 2717u, (A, 9, D), 0xA9D, 0xA9D, 1, 0x562u
#define PPUTLIMPL_UHEX_0xA9Cu 2716u, (A, 9, C), 0xA9C, 0xA9C, 1, 0x563u
#define PPUTLIMPL_UHEX_0xA9Bu 2715u, (A, 9, B), 0xA9B, 0xA9B, 1, 0x564u
#define PPUTLIMPL_UHEX_0xA9Au 2714u, (A, 9, A), 0xA9A, 0xA9A, 1, 0x565u
#define PPUTLIMPL_UHEX_0xA99u 2713u, (A, 9, 9), 0xA99, 0xA99, 1, 0x566u
#define PPUTLIMPL_UHEX_0xA98u 2712u, (A, 9, 8), 0xA98, 0xA98, 1, 0x567u
#define PPUTLIMPL_UHEX_0xA97u 2711u, (A, 9, 7), 0xA97, 0xA97, 1, 0x568u
#define PPUTLIMPL_UHEX_0xA96u 2710u, (A, 9, 6), 0xA96, 0xA96, 1, 0x569u
#define PPUTLIMPL_UHEX_0xA95u 2709u, (A, 9, 5), 0xA95, 0xA95, 1, 0x56Au
#define PPUTLIMPL_UHEX_0xA94u 2708u, (A, 9, 4), 0xA94, 0xA94, 1, 0x56Bu
#define PPUTLIMPL_UHEX_0xA93u 2707u, (A, 9, 3), 0xA93, 0xA93, 1, 0x56Cu
#define PPUTLIMPL_UHEX_0xA92u 2706u, (A, 9, 2), 0xA92, 0xA92, 1, 0x56Du
#define PPUTLIMPL_UHEX_0xA91u 2705u, (A, 9, 1), 0xA91, 0xA91, 1, 0x56Eu
#define PPUTLIMPL_UHEX_0xA90u 2704u, (A, 9, 0), 0xA90, 0xA90, 1, 0x56Fu
#define PPUTLIMPL_UHEX_0xA8Fu 2703u, (A, 8, F), 0xA8F, 0xA8F, 1, 0x570u
#define PPUTLIMPL_UHEX_0xA8Eu 2702u, (A, 8, E), 0xA8E, 0xA8E, 1, 0x571u
#define PPUTLIMPL_UHEX_0xA8Du 2701u, (A, 8, D), 0xA8D, 0xA8D, 1, 0x572u
#define PPUTLIMPL_UHEX_0xA8Cu 2700u, (A, 8, C), 0xA8C, 0xA8C, 1, 0x573u
#define PPUTLIMPL_UHEX_0xA8Bu 2699u, (A, 8, B), 0xA8B, 0xA8B, 1, 0x574u
#define PPUTLIMPL_UHEX_0xA8Au 2698u, (A, 8, A), 0xA8A, 0xA8A, 1, 0x575u
#define PPUTLIMPL_UHEX_0xA89u 2697u, (A, 8, 9), 0xA89, 0xA89, 1, 0x576u
#define PPUTLIMPL_UHEX_0xA88u 2696u, (A, 8, 8), 0xA88, 0xA88, 1, 0x577u
#define PPUTLIMPL_UHEX_0xA87u 2695u, (A, 8, 7), 0xA87, 0xA87, 1, 0x578u
#define PPUTLIMPL_UHEX_0xA86u 2694u, (A, 8, 6), 0xA86, 0xA86, 1, 0x579u
#define PPUTLIMPL_UHEX_0xA85u 2693u, (A, 8, 5), 0xA85, 0xA85, 1, 0x57Au
#define PPUTLIMPL_UHEX_0xA84u 2692u, (A, 8, 4), 0xA84, 0xA84, 1, 0x57Bu
#define PPUTLIMPL_UHEX_0xA83u 2691u, (A, 8, 3), 0xA83, 0xA83, 1, 0x57Cu
#define PPUTLIMPL_UHEX_0xA82u 2690u, (A, 8, 2), 0xA82, 0xA82, 1, 0x57Du
#define PPUTLIMPL_UHEX_0xA81u 2689u, (A, 8, 1), 0xA81, 0xA81, 1, 0x57Eu
#define PPUTLIMPL_UHEX_0xA80u 2688u, (A, 8, 0), 0xA80, 0xA80, 1, 0x57Fu
#define PPUTLIMPL_UHEX_0xA7Fu 2687u, (A, 7, F), 0xA7F, 0xA7F, 1, 0x580u
#define PPUTLIMPL_UHEX_0xA7Eu 2686u, (A, 7, E), 0xA7E, 0xA7E, 1, 0x581u
#define PPUTLIMPL_UHEX_0xA7Du 2685u, (A, 7, D), 0xA7D, 0xA7D, 1, 0x582u
#define PPUTLIMPL_UHEX_0xA7Cu 2684u, (A, 7, C), 0xA7C, 0xA7C, 1, 0x583u
#define PPUTLIMPL_UHEX_0xA7Bu 2683u, (A, 7, B), 0xA7B, 0xA7B, 1, 0x584u
#define PPUTLIMPL_UHEX_0xA7Au 2682u, (A, 7, A), 0xA7A, 0xA7A, 1, 0x585u
#define PPUTLIMPL_UHEX_0xA79u 2681u, (A, 7, 9), 0xA79, 0xA79, 1, 0x586u
#define PPUTLIMPL_UHEX_0xA78u 2680u, (A, 7, 8), 0xA78, 0xA78, 1, 0x587u
#define PPUTLIMPL_UHEX_0xA77u 2679u, (A, 7, 7), 0xA77, 0xA77, 1, 0x588u
#define PPUTLIMPL_UHEX_0xA76u 2678u, (A, 7, 6), 0xA76, 0xA76, 1, 0x589u
#define PPUTLIMPL_UHEX_0xA75u 2677u, (A, 7, 5), 0xA75, 0xA75, 1, 0x58Au
#define PPUTLIMPL_UHEX_0xA74u 2676u, (A, 7, 4), 0xA74, 0xA74, 1, 0x58Bu
#define PPUTLIMPL_UHEX_0xA73u 2675u, (A, 7, 3), 0xA73, 0xA73, 1, 0x58Cu
#define PPUTLIMPL_UHEX_0xA72u 2674u, (A, 7, 2), 0xA72, 0xA72, 1, 0x58Du
#define PPUTLIMPL_UHEX_0xA71u 2673u, (A, 7, 1), 0xA71, 0xA71, 1, 0x58Eu
#define PPUTLIMPL_UHEX_0xA70u 2672u, (A, 7, 0), 0xA70, 0xA70, 1, 0x58Fu
#define PPUTLIMPL_UHEX_0xA6Fu 2671u, (A, 6, F), 0xA6F, 0xA6F, 1, 0x590u
#define PPUTLIMPL_UHEX_0xA6Eu 2670u, (A, 6, E), 0xA6E, 0xA6E, 1, 0x591u
#define PPUTLIMPL_UHEX_0xA6Du 2669u, (A, 6, D), 0xA6D, 0xA6D, 1, 0x592u
#define PPUTLIMPL_UHEX_0xA6Cu 2668u, (A, 6, C), 0xA6C, 0xA6C, 1, 0x593u
#define PPUTLIMPL_UHEX_0xA6Bu 2667u, (A, 6, B), 0xA6B, 0xA6B, 1, 0x594u
#define PPUTLIMPL_UHEX_0xA6Au 2666u, (A, 6, A), 0xA6A, 0xA6A, 1, 0x595u
#define PPUTLIMPL_UHEX_0xA69u 2665u, (A, 6, 9), 0xA69, 0xA69, 1, 0x596u
#define PPUTLIMPL_UHEX_0xA68u 2664u, (A, 6, 8), 0xA68, 0xA68, 1, 0x597u
#define PPUTLIMPL_UHEX_0xA67u 2663u, (A, 6, 7), 0xA67, 0xA67, 1, 0x598u
#define PPUTLIMPL_UHEX_0xA66u 2662u, (A, 6, 6), 0xA66, 0xA66, 1, 0x599u
#define PPUTLIMPL_UHEX_0xA65u 2661u, (A, 6, 5), 0xA65, 0xA65, 1, 0x59Au
#define PPUTLIMPL_UHEX_0xA64u 2660u, (A, 6, 4), 0xA64, 0xA64, 1, 0x59Bu
#define PPUTLIMPL_UHEX_0xA63u 2659u, (A, 6, 3), 0xA63, 0xA63, 1, 0x59Cu
#define PPUTLIMPL_UHEX_0xA62u 2658u, (A, 6, 2), 0xA62, 0xA62, 1, 0x59Du
#define PPUTLIMPL_UHEX_0xA61u 2657u, (A, 6, 1), 0xA61, 0xA61, 1, 0x59Eu
#define PPUTLIMPL_UHEX_0xA60u 2656u, (A, 6, 0), 0xA60, 0xA60, 1, 0x59Fu
#define PPUTLIMPL_UHEX_0xA5Fu 2655u, (A, 5, F), 0xA5F, 0xA5F, 1, 0x5A0u
#define PPUTLIMPL_UHEX_0xA5Eu 2654u, (A, 5, E), 0xA5E, 0xA5E, 1, 0x5A1u
#define PPUTLIMPL_UHEX_0xA5Du 2653u, (A, 5, D), 0xA5D, 0xA5D, 1, 0x5A2u
#define PPUTLIMPL_UHEX_0xA5Cu 2652u, (A, 5, C), 0xA5C, 0xA5C, 1, 0x5A3u
#define PPUTLIMPL_UHEX_0xA5Bu 2651u, (A, 5, B), 0xA5B, 0xA5B, 1, 0x5A4u
#define PPUTLIMPL_UHEX_0xA5Au 2650u, (A, 5, A), 0xA5A, 0xA5A, 1, 0x5A5u
#define PPUTLIMPL_UHEX_0xA59u 2649u, (A, 5, 9), 0xA59, 0xA59, 1, 0x5A6u
#define PPUTLIMPL_UHEX_0xA58u 2648u, (A, 5, 8), 0xA58, 0xA58, 1, 0x5A7u
#define PPUTLIMPL_UHEX_0xA57u 2647u, (A, 5, 7), 0xA57, 0xA57, 1, 0x5A8u
#define PPUTLIMPL_UHEX_0xA56u 2646u, (A, 5, 6), 0xA56, 0xA56, 1, 0x5A9u
#define PPUTLIMPL_UHEX_0xA55u 2645u, (A, 5, 5), 0xA55, 0xA55, 1, 0x5AAu
#define PPUTLIMPL_UHEX_0xA54u 2644u, (A, 5, 4), 0xA54, 0xA54, 1, 0x5ABu
#define PPUTLIMPL_UHEX_0xA53u 2643u, (A, 5, 3), 0xA53, 0xA53, 1, 0x5ACu
#define PPUTLIMPL_UHEX_0xA52u 2642u, (A, 5, 2), 0xA52, 0xA52, 1, 0x5ADu
#define PPUTLIMPL_UHEX_0xA51u 2641u, (A, 5, 1), 0xA51, 0xA51, 1, 0x5AEu
#define PPUTLIMPL_UHEX_0xA50u 2640u, (A, 5, 0), 0xA50, 0xA50, 1, 0x5AFu
#define PPUTLIMPL_UHEX_0xA4Fu 2639u, (A, 4, F), 0xA4F, 0xA4F, 1, 0x5B0u
#define PPUTLIMPL_UHEX_0xA4Eu 2638u, (A, 4, E), 0xA4E, 0xA4E, 1, 0x5B1u
#define PPUTLIMPL_UHEX_0xA4Du 2637u, (A, 4, D), 0xA4D, 0xA4D, 1, 0x5B2u
#define PPUTLIMPL_UHEX_0xA4Cu 2636u, (A, 4, C), 0xA4C, 0xA4C, 1, 0x5B3u
#define PPUTLIMPL_UHEX_0xA4Bu 2635u, (A, 4, B), 0xA4B, 0xA4B, 1, 0x5B4u
#define PPUTLIMPL_UHEX_0xA4Au 2634u, (A, 4, A), 0xA4A, 0xA4A, 1, 0x5B5u
#define PPUTLIMPL_UHEX_0xA49u 2633u, (A, 4, 9), 0xA49, 0xA49, 1, 0x5B6u
#define PPUTLIMPL_UHEX_0xA48u 2632u, (A, 4, 8), 0xA48, 0xA48, 1, 0x5B7u
#define PPUTLIMPL_UHEX_0xA47u 2631u, (A, 4, 7), 0xA47, 0xA47, 1, 0x5B8u
#define PPUTLIMPL_UHEX_0xA46u 2630u, (A, 4, 6), 0xA46, 0xA46, 1, 0x5B9u
#define PPUTLIMPL_UHEX_0xA45u 2629u, (A, 4, 5), 0xA45, 0xA45, 1, 0x5BAu
#define PPUTLIMPL_UHEX_0xA44u 2628u, (A, 4, 4), 0xA44, 0xA44, 1, 0x5BBu
#define PPUTLIMPL_UHEX_0xA43u 2627u, (A, 4, 3), 0xA43, 0xA43, 1, 0x5BCu
#define PPUTLIMPL_UHEX_0xA42u 2626u, (A, 4, 2), 0xA42, 0xA42, 1, 0x5BDu
#define PPUTLIMPL_UHEX_0xA41u 2625u, (A, 4, 1), 0xA41, 0xA41, 1, 0x5BEu
#define PPUTLIMPL_UHEX_0xA40u 2624u, (A, 4, 0), 0xA40, 0xA40, 1, 0x5BFu
#define PPUTLIMPL_UHEX_0xA3Fu 2623u, (A, 3, F), 0xA3F, 0xA3F, 1, 0x5C0u
#define PPUTLIMPL_UHEX_0xA3Eu 2622u, (A, 3, E), 0xA3E, 0xA3E, 1, 0x5C1u
#define PPUTLIMPL_UHEX_0xA3Du 2621u, (A, 3, D), 0xA3D, 0xA3D, 1, 0x5C2u
#define PPUTLIMPL_UHEX_0xA3Cu 2620u, (A, 3, C), 0xA3C, 0xA3C, 1, 0x5C3u
#define PPUTLIMPL_UHEX_0xA3Bu 2619u, (A, 3, B), 0xA3B, 0xA3B, 1, 0x5C4u
#define PPUTLIMPL_UHEX_0xA3Au 2618u, (A, 3, A), 0xA3A, 0xA3A, 1, 0x5C5u
#define PPUTLIMPL_UHEX_0xA39u 2617u, (A, 3, 9), 0xA39, 0xA39, 1, 0x5C6u
#define PPUTLIMPL_UHEX_0xA38u 2616u, (A, 3, 8), 0xA38, 0xA38, 1, 0x5C7u
#define PPUTLIMPL_UHEX_0xA37u 2615u, (A, 3, 7), 0xA37, 0xA37, 1, 0x5C8u
#define PPUTLIMPL_UHEX_0xA36u 2614u, (A, 3, 6), 0xA36, 0xA36, 1, 0x5C9u
#define PPUTLIMPL_UHEX_0xA35u 2613u, (A, 3, 5), 0xA35, 0xA35, 1, 0x5CAu
#define PPUTLIMPL_UHEX_0xA34u 2612u, (A, 3, 4), 0xA34, 0xA34, 1, 0x5CBu
#define PPUTLIMPL_UHEX_0xA33u 2611u, (A, 3, 3), 0xA33, 0xA33, 1, 0x5CCu
#define PPUTLIMPL_UHEX_0xA32u 2610u, (A, 3, 2), 0xA32, 0xA32, 1, 0x5CDu
#define PPUTLIMPL_UHEX_0xA31u 2609u, (A, 3, 1), 0xA31, 0xA31, 1, 0x5CEu
#define PPUTLIMPL_UHEX_0xA30u 2608u, (A, 3, 0), 0xA30, 0xA30, 1, 0x5CFu
#define PPUTLIMPL_UHEX_0xA2Fu 2607u, (A, 2, F), 0xA2F, 0xA2F, 1, 0x5D0u
#define PPUTLIMPL_UHEX_0xA2Eu 2606u, (A, 2, E), 0xA2E, 0xA2E, 1, 0x5D1u
#define PPUTLIMPL_UHEX_0xA2Du 2605u, (A, 2, D), 0xA2D, 0xA2D, 1, 0x5D2u
#define PPUTLIMPL_UHEX_0xA2Cu 2604u, (A, 2, C), 0xA2C, 0xA2C, 1, 0x5D3u
#define PPUTLIMPL_UHEX_0xA2Bu 2603u, (A, 2, B), 0xA2B, 0xA2B, 1, 0x5D4u
#define PPUTLIMPL_UHEX_0xA2Au 2602u, (A, 2, A), 0xA2A, 0xA2A, 1, 0x5D5u
#define PPUTLIMPL_UHEX_0xA29u 2601u, (A, 2, 9), 0xA29, 0xA29, 1, 0x5D6u
#define PPUTLIMPL_UHEX_0xA28u 2600u, (A, 2, 8), 0xA28, 0xA28, 1, 0x5D7u
#define PPUTLIMPL_UHEX_0xA27u 2599u, (A, 2, 7), 0xA27, 0xA27, 1, 0x5D8u
#define PPUTLIMPL_UHEX_0xA26u 2598u, (A, 2, 6), 0xA26, 0xA26, 1, 0x5D9u
#define PPUTLIMPL_UHEX_0xA25u 2597u, (A, 2, 5), 0xA25, 0xA25, 1, 0x5DAu
#define PPUTLIMPL_UHEX_0xA24u 2596u, (A, 2, 4), 0xA24, 0xA24, 1, 0x5DBu
#define PPUTLIMPL_UHEX_0xA23u 2595u, (A, 2, 3), 0xA23, 0xA23, 1, 0x5DCu
#define PPUTLIMPL_UHEX_0xA22u 2594u, (A, 2, 2), 0xA22, 0xA22, 1, 0x5DDu
#define PPUTLIMPL_UHEX_0xA21u 2593u, (A, 2, 1), 0xA21, 0xA21, 1, 0x5DEu
#define PPUTLIMPL_UHEX_0xA20u 2592u, (A, 2, 0), 0xA20, 0xA20, 1, 0x5DFu
#define PPUTLIMPL_UHEX_0xA1Fu 2591u, (A, 1, F), 0xA1F, 0xA1F, 1, 0x5E0u
#define PPUTLIMPL_UHEX_0xA1Eu 2590u, (A, 1, E), 0xA1E, 0xA1E, 1, 0x5E1u
#define PPUTLIMPL_UHEX_0xA1Du 2589u, (A, 1, D), 0xA1D, 0xA1D, 1, 0x5E2u
#define PPUTLIMPL_UHEX_0xA1Cu 2588u, (A, 1, C), 0xA1C, 0xA1C, 1, 0x5E3u
#define PPUTLIMPL_UHEX_0xA1Bu 2587u, (A, 1, B), 0xA1B, 0xA1B, 1, 0x5E4u
#define PPUTLIMPL_UHEX_0xA1Au 2586u, (A, 1, A), 0xA1A, 0xA1A, 1, 0x5E5u
#define PPUTLIMPL_UHEX_0xA19u 2585u, (A, 1, 9), 0xA19, 0xA19, 1, 0x5E6u
#define PPUTLIMPL_UHEX_0xA18u 2584u, (A, 1, 8), 0xA18, 0xA18, 1, 0x5E7u
#define PPUTLIMPL_UHEX_0xA17u 2583u, (A, 1, 7), 0xA17, 0xA17, 1, 0x5E8u
#define PPUTLIMPL_UHEX_0xA16u 2582u, (A, 1, 6), 0xA16, 0xA16, 1, 0x5E9u
#define PPUTLIMPL_UHEX_0xA15u 2581u, (A, 1, 5), 0xA15, 0xA15, 1, 0x5EAu
#define PPUTLIMPL_UHEX_0xA14u 2580u, (A, 1, 4), 0xA14, 0xA14, 1, 0x5EBu
#define PPUTLIMPL_UHEX_0xA13u 2579u, (A, 1, 3), 0xA13, 0xA13, 1, 0x5ECu
#define PPUTLIMPL_UHEX_0xA12u 2578u, (A, 1, 2), 0xA12, 0xA12, 1, 0x5EDu
#define PPUTLIMPL_UHEX_0xA11u 2577u, (A, 1, 1), 0xA11, 0xA11, 1, 0x5EEu
#define PPUTLIMPL_UHEX_0xA10u 2576u, (A, 1, 0), 0xA10, 0xA10, 1, 0x5EFu
#define PPUTLIMPL_UHEX_0xA0Fu 2575u, (A, 0, F), 0xA0F, 0xA0F, 1, 0x5F0u
#define PPUTLIMPL_UHEX_0xA0Eu 2574u, (A, 0, E), 0xA0E, 0xA0E, 1, 0x5F1u
#define PPUTLIMPL_UHEX_0xA0Du 2573u, (A, 0, D), 0xA0D, 0xA0D, 1, 0x5F2u
#define PPUTLIMPL_UHEX_0xA0Cu 2572u, (A, 0, C), 0xA0C, 0xA0C, 1, 0x5F3u
#define PPUTLIMPL_UHEX_0xA0Bu 2571u, (A, 0, B), 0xA0B, 0xA0B, 1, 0x5F4u
#define PPUTLIMPL_UHEX_0xA0Au 2570u, (A, 0, A), 0xA0A, 0xA0A, 1, 0x5F5u
#define PPUTLIMPL_UHEX_0xA09u 2569u, (A, 0, 9), 0xA09, 0xA09, 1, 0x5F6u
#define PPUTLIMPL_UHEX_0xA08u 2568u, (A, 0, 8), 0xA08, 0xA08, 1, 0x5F7u
#define PPUTLIMPL_UHEX_0xA07u 2567u, (A, 0, 7), 0xA07, 0xA07, 1, 0x5F8u
#define PPUTLIMPL_UHEX_0xA06u 2566u, (A, 0, 6), 0xA06, 0xA06, 1, 0x5F9u
#define PPUTLIMPL_UHEX_0xA05u 2565u, (A, 0, 5), 0xA05, 0xA05, 1, 0x5FAu
#define PPUTLIMPL_UHEX_0xA04u 2564u, (A, 0, 4), 0xA04, 0xA04, 1, 0x5FBu
#define PPUTLIMPL_UHEX_0xA03u 2563u, (A, 0, 3), 0xA03, 0xA03, 1, 0x5FCu
#define PPUTLIMPL_UHEX_0xA02u 2562u, (A, 0, 2), 0xA02, 0xA02, 1, 0x5FDu
#define PPUTLIMPL_UHEX_0xA01u 2561u, (A, 0, 1), 0xA01, 0xA01, 1, 0x5FEu
#define PPUTLIMPL_UHEX_0xA00u 2560u, (A, 0, 0), 0xA00, 0xA00, 1, 0x5FFu
#define PPUTLIMPL_UHEX_0x9FFu 2559u, (9, F, F), 0x9FF, 0x9FF, 1, 0x600u
#define PPUTLIMPL_UHEX_0x9FEu 2558u, (9, F, E), 0x9FE, 0x9FE, 1, 0x601u
#define PPUTLIMPL_UHEX_0x9FDu 2557u, (9, F, D), 0x9FD, 0x9FD, 1, 0x602u
#define PPUTLIMPL_UHEX_0x9FCu 2556u, (9, F, C), 0x9FC, 0x9FC, 1, 0x603u
#define PPUTLIMPL_UHEX_0x9FBu 2555u, (9, F, B), 0x9FB, 0x9FB, 1, 0x604u
#define PPUTLIMPL_UHEX_0x9FAu 2554u, (9, F, A), 0x9FA, 0x9FA, 1, 0x605u
#define PPUTLIMPL_UHEX_0x9F9u 2553u, (9, F, 9), 0x9F9, 0x9F9, 1, 0x606u
#define PPUTLIMPL_UHEX_0x9F8u 2552u, (9, F, 8), 0x9F8, 0x9F8, 1, 0x607u
#define PPUTLIMPL_UHEX_0x9F7u 2551u, (9, F, 7), 0x9F7, 0x9F7, 1, 0x608u
#define PPUTLIMPL_UHEX_0x9F6u 2550u, (9, F, 6), 0x9F6, 0x9F6, 1, 0x609u
#define PPUTLIMPL_UHEX_0x9F5u 2549u, (9, F, 5), 0x9F5, 0x9F5, 1, 0x60Au
#define PPUTLIMPL_UHEX_0x9F4u 2548u, (9, F, 4), 0x9F4, 0x9F4, 1, 0x60Bu
#define PPUTLIMPL_UHEX_0x9F3u 2547u, (9, F, 3), 0x9F3, 0x9F3, 1, 0x60Cu
#define PPUTLIMPL_UHEX_0x9F2u 2546u, (9, F, 2), 0x9F2, 0x9F2, 1, 0x60Du
#define PPUTLIMPL_UHEX_0x9F1u 2545u, (9, F, 1), 0x9F1, 0x9F1, 1, 0x60Eu
#define PPUTLIMPL_UHEX_0x9F0u 2544u, (9, F, 0), 0x9F0, 0x9F0, 1, 0x60Fu
#define PPUTLIMPL_UHEX_0x9EFu 2543u, (9, E, F), 0x9EF, 0x9EF, 1, 0x610u
#define PPUTLIMPL_UHEX_0x9EEu 2542u, (9, E, E), 0x9EE, 0x9EE, 1, 0x611u
#define PPUTLIMPL_UHEX_0x9EDu 2541u, (9, E, D), 0x9ED, 0x9ED, 1, 0x612u
#define PPUTLIMPL_UHEX_0x9ECu 2540u, (9, E, C), 0x9EC, 0x9EC, 1, 0x613u
#define PPUTLIMPL_UHEX_0x9EBu 2539u, (9, E, B), 0x9EB, 0x9EB, 1, 0x614u
#define PPUTLIMPL_UHEX_0x9EAu 2538u, (9, E, A), 0x9EA, 0x9EA, 1, 0x615u
#define PPUTLIMPL_UHEX_0x9E9u 2537u, (9, E, 9), 0x9E9, 0x9E9, 1, 0x616u
#define PPUTLIMPL_UHEX_0x9E8u 2536u, (9, E, 8), 0x9E8, 0x9E8, 1, 0x617u
#define PPUTLIMPL_UHEX_0x9E7u 2535u, (9, E, 7), 0x9E7, 0x9E7, 1, 0x618u
#define PPUTLIMPL_UHEX_0x9E6u 2534u, (9, E, 6), 0x9E6, 0x9E6, 1, 0x619u
#define PPUTLIMPL_UHEX_0x9E5u 2533u, (9, E, 5), 0x9E5, 0x9E5, 1, 0x61Au
#define PPUTLIMPL_UHEX_0x9E4u 2532u, (9, E, 4), 0x9E4, 0x9E4, 1, 0x61Bu
#define PPUTLIMPL_UHEX_0x9E3u 2531u, (9, E, 3), 0x9E3, 0x9E3, 1, 0x61Cu
#define PPUTLIMPL_UHEX_0x9E2u 2530u, (9, E, 2), 0x9E2, 0x9E2, 1, 0x61Du
#define PPUTLIMPL_UHEX_0x9E1u 2529u, (9, E, 1), 0x9E1, 0x9E1, 1, 0x61Eu
#define PPUTLIMPL_UHEX_0x9E0u 2528u, (9, E, 0), 0x9E0, 0x9E0, 1, 0x61Fu
#define PPUTLIMPL_UHEX_0x9DFu 2527u, (9, D, F), 0x9DF, 0x9DF, 1, 0x620u
#define PPUTLIMPL_UHEX_0x9DEu 2526u, (9, D, E), 0x9DE, 0x9DE, 1, 0x621u
#define PPUTLIMPL_UHEX_0x9DDu 2525u, (9, D, D), 0x9DD, 0x9DD, 1, 0x622u
#define PPUTLIMPL_UHEX_0x9DCu 2524u, (9, D, C), 0x9DC, 0x9DC, 1, 0x623u
#define PPUTLIMPL_UHEX_0x9DBu 2523u, (9, D, B), 0x9DB, 0x9DB, 1, 0x624u
#define PPUTLIMPL_UHEX_0x9DAu 2522u, (9, D, A), 0x9DA, 0x9DA, 1, 0x625u
#define PPUTLIMPL_UHEX_0x9D9u 2521u, (9, D, 9), 0x9D9, 0x9D9, 1, 0x626u
#define PPUTLIMPL_UHEX_0x9D8u 2520u, (9, D, 8), 0x9D8, 0x9D8, 1, 0x627u
#define PPUTLIMPL_UHEX_0x9D7u 2519u, (9, D, 7), 0x9D7, 0x9D7, 1, 0x628u
#define PPUTLIMPL_UHEX_0x9D6u 2518u, (9, D, 6), 0x9D6, 0x9D6, 1, 0x629u
#define PPUTLIMPL_UHEX_0x9D5u 2517u, (9, D, 5), 0x9D5, 0x9D5, 1, 0x62Au
#define PPUTLIMPL_UHEX_0x9D4u 2516u, (9, D, 4), 0x9D4, 0x9D4, 1, 0x62Bu
#define PPUTLIMPL_UHEX_0x9D3u 2515u, (9, D, 3), 0x9D3, 0x9D3, 1, 0x62Cu
#define PPUTLIMPL_UHEX_0x9D2u 2514u, (9, D, 2), 0x9D2, 0x9D2, 1, 0x62Du
#define PPUTLIMPL_UHEX_0x9D1u 2513u, (9, D, 1), 0x9D1, 0x9D1, 1, 0x62Eu
#define PPUTLIMPL_UHEX_0x9D0u 2512u, (9, D, 0), 0x9D0, 0x9D0, 1, 0x62Fu
#define PPUTLIMPL_UHEX_0x9CFu 2511u, (9, C, F), 0x9CF, 0x9CF, 1, 0x630u
#define PPUTLIMPL_UHEX_0x9CEu 2510u, (9, C, E), 0x9CE, 0x9CE, 1, 0x631u
#define PPUTLIMPL_UHEX_0x9CDu 2509u, (9, C, D), 0x9CD, 0x9CD, 1, 0x632u
#define PPUTLIMPL_UHEX_0x9CCu 2508u, (9, C, C), 0x9CC, 0x9CC, 1, 0x633u
#define PPUTLIMPL_UHEX_0x9CBu 2507u, (9, C, B), 0x9CB, 0x9CB, 1, 0x634u
#define PPUTLIMPL_UHEX_0x9CAu 2506u, (9, C, A), 0x9CA, 0x9CA, 1, 0x635u
#define PPUTLIMPL_UHEX_0x9C9u 2505u, (9, C, 9), 0x9C9, 0x9C9, 1, 0x636u
#define PPUTLIMPL_UHEX_0x9C8u 2504u, (9, C, 8), 0x9C8, 0x9C8, 1, 0x637u
#define PPUTLIMPL_UHEX_0x9C7u 2503u, (9, C, 7), 0x9C7, 0x9C7, 1, 0x638u
#define PPUTLIMPL_UHEX_0x9C6u 2502u, (9, C, 6), 0x9C6, 0x9C6, 1, 0x639u
#define PPUTLIMPL_UHEX_0x9C5u 2501u, (9, C, 5), 0x9C5, 0x9C5, 1, 0x63Au
#define PPUTLIMPL_UHEX_0x9C4u 2500u, (9, C, 4), 0x9C4, 0x9C4, 1, 0x63Bu
#define PPUTLIMPL_UHEX_0x9C3u 2499u, (9, C, 3), 0x9C3, 0x9C3, 1, 0x63Cu
#define PPUTLIMPL_UHEX_0x9C2u 2498u, (9, C, 2), 0x9C2, 0x9C2, 1, 0x63Du
#define PPUTLIMPL_UHEX_0x9C1u 2497u, (9, C, 1), 0x9C1, 0x9C1, 1, 0x63Eu
#define PPUTLIMPL_UHEX_0x9C0u 2496u, (9, C, 0), 0x9C0, 0x9C0, 1, 0x63Fu
#define PPUTLIMPL_UHEX_0x9BFu 2495u, (9, B, F), 0x9BF, 0x9BF, 1, 0x640u
#define PPUTLIMPL_UHEX_0x9BEu 2494u, (9, B, E), 0x9BE, 0x9BE, 1, 0x641u
#define PPUTLIMPL_UHEX_0x9BDu 2493u, (9, B, D), 0x9BD, 0x9BD, 1, 0x642u
#define PPUTLIMPL_UHEX_0x9BCu 2492u, (9, B, C), 0x9BC, 0x9BC, 1, 0x643u
#define PPUTLIMPL_UHEX_0x9BBu 2491u, (9, B, B), 0x9BB, 0x9BB, 1, 0x644u
#define PPUTLIMPL_UHEX_0x9BAu 2490u, (9, B, A), 0x9BA, 0x9BA, 1, 0x645u
#define PPUTLIMPL_UHEX_0x9B9u 2489u, (9, B, 9), 0x9B9, 0x9B9, 1, 0x646u
#define PPUTLIMPL_UHEX_0x9B8u 2488u, (9, B, 8), 0x9B8, 0x9B8, 1, 0x647u
#define PPUTLIMPL_UHEX_0x9B7u 2487u, (9, B, 7), 0x9B7, 0x9B7, 1, 0x648u
#define PPUTLIMPL_UHEX_0x9B6u 2486u, (9, B, 6), 0x9B6, 0x9B6, 1, 0x649u
#define PPUTLIMPL_UHEX_0x9B5u 2485u, (9, B, 5), 0x9B5, 0x9B5, 1, 0x64Au
#define PPUTLIMPL_UHEX_0x9B4u 2484u, (9, B, 4), 0x9B4, 0x9B4, 1, 0x64Bu
#define PPUTLIMPL_UHEX_0x9B3u 2483u, (9, B, 3), 0x9B3, 0x9B3, 1, 0x64Cu
#define PPUTLIMPL_UHEX_0x9B2u 2482u, (9, B, 2), 0x9B2, 0x9B2, 1, 0x64Du
#define PPUTLIMPL_UHEX_0x9B1u 2481u, (9, B, 1), 0x9B1, 0x9B1, 1, 0x64Eu
#define PPUTLIMPL_UHEX_0x9B0u 2480u, (9, B, 0), 0x9B0, 0x9B0, 1, 0x64Fu
#define PPUTLIMPL_UHEX_0x9AFu 2479u, (9, A, F), 0x9AF, 0x9AF, 1, 0x650u
#define PPUTLIMPL_UHEX_0x9AEu 2478u, (9, A, E), 0x9AE, 0x9AE, 1, 0x651u
#define PPUTLIMPL_UHEX_0x9ADu 2477u, (9, A, D), 0x9AD, 0x9AD, 1, 0x652u
#define PPUTLIMPL_UHEX_0x9ACu 2476u, (9, A, C), 0x9AC, 0x9AC, 1, 0x653u
#define PPUTLIMPL_UHEX_0x9ABu 2475u, (9, A, B), 0x9AB, 0x9AB, 1, 0x654u
#define PPUTLIMPL_UHEX_0x9AAu 2474u, (9, A, A), 0x9AA, 0x9AA, 1, 0x655u
#define PPUTLIMPL_UHEX_0x9A9u 2473u, (9, A, 9), 0x9A9, 0x9A9, 1, 0x656u
#define PPUTLIMPL_UHEX_0x9A8u 2472u, (9, A, 8), 0x9A8, 0x9A8, 1, 0x657u
#define PPUTLIMPL_UHEX_0x9A7u 2471u, (9, A, 7), 0x9A7, 0x9A7, 1, 0x658u
#define PPUTLIMPL_UHEX_0x9A6u 2470u, (9, A, 6), 0x9A6, 0x9A6, 1, 0x659u
#define PPUTLIMPL_UHEX_0x9A5u 2469u, (9, A, 5), 0x9A5, 0x9A5, 1, 0x65Au
#define PPUTLIMPL_UHEX_0x9A4u 2468u, (9, A, 4), 0x9A4, 0x9A4, 1, 0x65Bu
#define PPUTLIMPL_UHEX_0x9A3u 2467u, (9, A, 3), 0x9A3, 0x9A3, 1, 0x65Cu
#define PPUTLIMPL_UHEX_0x9A2u 2466u, (9, A, 2), 0x9A2, 0x9A2, 1, 0x65Du
#define PPUTLIMPL_UHEX_0x9A1u 2465u, (9, A, 1), 0x9A1, 0x9A1, 1, 0x65Eu
#define PPUTLIMPL_UHEX_0x9A0u 2464u, (9, A, 0), 0x9A0, 0x9A0, 1, 0x65Fu
#define PPUTLIMPL_UHEX_0x99Fu 2463u, (9, 9, F), 0x99F, 0x99F, 1, 0x660u
#define PPUTLIMPL_UHEX_0x99Eu 2462u, (9, 9, E), 0x99E, 0x99E, 1, 0x661u
#define PPUTLIMPL_UHEX_0x99Du 2461u, (9, 9, D), 0x99D, 0x99D, 1, 0x662u
#define PPUTLIMPL_UHEX_0x99Cu 2460u, (9, 9, C), 0x99C, 0x99C, 1, 0x663u
#define PPUTLIMPL_UHEX_0x99Bu 2459u, (9, 9, B), 0x99B, 0x99B, 1, 0x664u
#define PPUTLIMPL_UHEX_0x99Au 2458u, (9, 9, A), 0x99A, 0x99A, 1, 0x665u
#define PPUTLIMPL_UHEX_0x999u 2457u, (9, 9, 9), 0x999, 0x999, 1, 0x666u
#define PPUTLIMPL_UHEX_0x998u 2456u, (9, 9, 8), 0x998, 0x998, 1, 0x667u
#define PPUTLIMPL_UHEX_0x997u 2455u, (9, 9, 7), 0x997, 0x997, 1, 0x668u
#define PPUTLIMPL_UHEX_0x996u 2454u, (9, 9, 6), 0x996, 0x996, 1, 0x669u
#define PPUTLIMPL_UHEX_0x995u 2453u, (9, 9, 5), 0x995, 0x995, 1, 0x66Au
#define PPUTLIMPL_UHEX_0x994u 2452u, (9, 9, 4), 0x994, 0x994, 1, 0x66Bu
#define PPUTLIMPL_UHEX_0x993u 2451u, (9, 9, 3), 0x993, 0x993, 1, 0x66Cu
#define PPUTLIMPL_UHEX_0x992u 2450u, (9, 9, 2), 0x992, 0x992, 1, 0x66Du
#define PPUTLIMPL_UHEX_0x991u 2449u, (9, 9, 1), 0x991, 0x991, 1, 0x66Eu
#define PPUTLIMPL_UHEX_0x990u 2448u, (9, 9, 0), 0x990, 0x990, 1, 0x66Fu
#define PPUTLIMPL_UHEX_0x98Fu 2447u, (9, 8, F), 0x98F, 0x98F, 1, 0x670u
#define PPUTLIMPL_UHEX_0x98Eu 2446u, (9, 8, E), 0x98E, 0x98E, 1, 0x671u
#define PPUTLIMPL_UHEX_0x98Du 2445u, (9, 8, D), 0x98D, 0x98D, 1, 0x672u
#define PPUTLIMPL_UHEX_0x98Cu 2444u, (9, 8, C), 0x98C, 0x98C, 1, 0x673u
#define PPUTLIMPL_UHEX_0x98Bu 2443u, (9, 8, B), 0x98B, 0x98B, 1, 0x674u
#define PPUTLIMPL_UHEX_0x98Au 2442u, (9, 8, A), 0x98A, 0x98A, 1, 0x675u
#define PPUTLIMPL_UHEX_0x989u 2441u, (9, 8, 9), 0x989, 0x989, 1, 0x676u
#define PPUTLIMPL_UHEX_0x988u 2440u, (9, 8, 8), 0x988, 0x988, 1, 0x677u
#define PPUTLIMPL_UHEX_0x987u 2439u, (9, 8, 7), 0x987, 0x987, 1, 0x678u
#define PPUTLIMPL_UHEX_0x986u 2438u, (9, 8, 6), 0x986, 0x986, 1, 0x679u
#define PPUTLIMPL_UHEX_0x985u 2437u, (9, 8, 5), 0x985, 0x985, 1, 0x67Au
#define PPUTLIMPL_UHEX_0x984u 2436u, (9, 8, 4), 0x984, 0x984, 1, 0x67Bu
#define PPUTLIMPL_UHEX_0x983u 2435u, (9, 8, 3), 0x983, 0x983, 1, 0x67Cu
#define PPUTLIMPL_UHEX_0x982u 2434u, (9, 8, 2), 0x982, 0x982, 1, 0x67Du
#define PPUTLIMPL_UHEX_0x981u 2433u, (9, 8, 1), 0x981, 0x981, 1, 0x67Eu
#define PPUTLIMPL_UHEX_0x980u 2432u, (9, 8, 0), 0x980, 0x980, 1, 0x67Fu
#define PPUTLIMPL_UHEX_0x97Fu 2431u, (9, 7, F), 0x97F, 0x97F, 1, 0x680u
#define PPUTLIMPL_UHEX_0x97Eu 2430u, (9, 7, E), 0x97E, 0x97E, 1, 0x681u
#define PPUTLIMPL_UHEX_0x97Du 2429u, (9, 7, D), 0x97D, 0x97D, 1, 0x682u
#define PPUTLIMPL_UHEX_0x97Cu 2428u, (9, 7, C), 0x97C, 0x97C, 1, 0x683u
#define PPUTLIMPL_UHEX_0x97Bu 2427u, (9, 7, B), 0x97B, 0x97B, 1, 0x684u
#define PPUTLIMPL_UHEX_0x97Au 2426u, (9, 7, A), 0x97A, 0x97A, 1, 0x685u
#define PPUTLIMPL_UHEX_0x979u 2425u, (9, 7, 9), 0x979, 0x979, 1, 0x686u
#define PPUTLIMPL_UHEX_0x978u 2424u, (9, 7, 8), 0x978, 0x978, 1, 0x687u
#define PPUTLIMPL_UHEX_0x977u 2423u, (9, 7, 7), 0x977, 0x977, 1, 0x688u
#define PPUTLIMPL_UHEX_0x976u 2422u, (9, 7, 6), 0x976, 0x976, 1, 0x689u
#define PPUTLIMPL_UHEX_0x975u 2421u, (9, 7, 5), 0x975, 0x975, 1, 0x68Au
#define PPUTLIMPL_UHEX_0x974u 2420u, (9, 7, 4), 0x974, 0x974, 1, 0x68Bu
#define PPUTLIMPL_UHEX_0x973u 2419u, (9, 7, 3), 0x973, 0x973, 1, 0x68Cu
#define PPUTLIMPL_UHEX_0x972u 2418u, (9, 7, 2), 0x972, 0x972, 1, 0x68Du
#define PPUTLIMPL_UHEX_0x971u 2417u, (9, 7, 1), 0x971, 0x971, 1, 0x68Eu
#define PPUTLIMPL_UHEX_0x970u 2416u, (9, 7, 0), 0x970, 0x970, 1, 0x68Fu
#define PPUTLIMPL_UHEX_0x96Fu 2415u, (9, 6, F), 0x96F, 0x96F, 1, 0x690u
#define PPUTLIMPL_UHEX_0x96Eu 2414u, (9, 6, E), 0x96E, 0x96E, 1, 0x691u
#define PPUTLIMPL_UHEX_0x96Du 2413u, (9, 6, D), 0x96D, 0x96D, 1, 0x692u
#define PPUTLIMPL_UHEX_0x96Cu 2412u, (9, 6, C), 0x96C, 0x96C, 1, 0x693u
#define PPUTLIMPL_UHEX_0x96Bu 2411u, (9, 6, B), 0x96B, 0x96B, 1, 0x694u
#define PPUTLIMPL_UHEX_0x96Au 2410u, (9, 6, A), 0x96A, 0x96A, 1, 0x695u
#define PPUTLIMPL_UHEX_0x969u 2409u, (9, 6, 9), 0x969, 0x969, 1, 0x696u
#define PPUTLIMPL_UHEX_0x968u 2408u, (9, 6, 8), 0x968, 0x968, 1, 0x697u
#define PPUTLIMPL_UHEX_0x967u 2407u, (9, 6, 7), 0x967, 0x967, 1, 0x698u
#define PPUTLIMPL_UHEX_0x966u 2406u, (9, 6, 6), 0x966, 0x966, 1, 0x699u
#define PPUTLIMPL_UHEX_0x965u 2405u, (9, 6, 5), 0x965, 0x965, 1, 0x69Au
#define PPUTLIMPL_UHEX_0x964u 2404u, (9, 6, 4), 0x964, 0x964, 1, 0x69Bu
#define PPUTLIMPL_UHEX_0x963u 2403u, (9, 6, 3), 0x963, 0x963, 1, 0x69Cu
#define PPUTLIMPL_UHEX_0x962u 2402u, (9, 6, 2), 0x962, 0x962, 1, 0x69Du
#define PPUTLIMPL_UHEX_0x961u 2401u, (9, 6, 1), 0x961, 0x961, 1, 0x69Eu
#define PPUTLIMPL_UHEX_0x960u 2400u, (9, 6, 0), 0x960, 0x960, 1, 0x69Fu
#define PPUTLIMPL_UHEX_0x95Fu 2399u, (9, 5, F), 0x95F, 0x95F, 1, 0x6A0u
#define PPUTLIMPL_UHEX_0x95Eu 2398u, (9, 5, E), 0x95E, 0x95E, 1, 0x6A1u
#define PPUTLIMPL_UHEX_0x95Du 2397u, (9, 5, D), 0x95D, 0x95D, 1, 0x6A2u
#define PPUTLIMPL_UHEX_0x95Cu 2396u, (9, 5, C), 0x95C, 0x95C, 1, 0x6A3u
#define PPUTLIMPL_UHEX_0x95Bu 2395u, (9, 5, B), 0x95B, 0x95B, 1, 0x6A4u
#define PPUTLIMPL_UHEX_0x95Au 2394u, (9, 5, A), 0x95A, 0x95A, 1, 0x6A5u
#define PPUTLIMPL_UHEX_0x959u 2393u, (9, 5, 9), 0x959, 0x959, 1, 0x6A6u
#define PPUTLIMPL_UHEX_0x958u 2392u, (9, 5, 8), 0x958, 0x958, 1, 0x6A7u
#define PPUTLIMPL_UHEX_0x957u 2391u, (9, 5, 7), 0x957, 0x957, 1, 0x6A8u
#define PPUTLIMPL_UHEX_0x956u 2390u, (9, 5, 6), 0x956, 0x956, 1, 0x6A9u
#define PPUTLIMPL_UHEX_0x955u 2389u, (9, 5, 5), 0x955, 0x955, 1, 0x6AAu
#define PPUTLIMPL_UHEX_0x954u 2388u, (9, 5, 4), 0x954, 0x954, 1, 0x6ABu
#define PPUTLIMPL_UHEX_0x953u 2387u, (9, 5, 3), 0x953, 0x953, 1, 0x6ACu
#define PPUTLIMPL_UHEX_0x952u 2386u, (9, 5, 2), 0x952, 0x952, 1, 0x6ADu
#define PPUTLIMPL_UHEX_0x951u 2385u, (9, 5, 1), 0x951, 0x951, 1, 0x6AEu
#define PPUTLIMPL_UHEX_0x950u 2384u, (9, 5, 0), 0x950, 0x950, 1, 0x6AFu
#define PPUTLIMPL_UHEX_0x94Fu 2383u, (9, 4, F), 0x94F, 0x94F, 1, 0x6B0u
#define PPUTLIMPL_UHEX_0x94Eu 2382u, (9, 4, E), 0x94E, 0x94E, 1, 0x6B1u
#define PPUTLIMPL_UHEX_0x94Du 2381u, (9, 4, D), 0x94D, 0x94D, 1, 0x6B2u
#define PPUTLIMPL_UHEX_0x94Cu 2380u, (9, 4, C), 0x94C, 0x94C, 1, 0x6B3u
#define PPUTLIMPL_UHEX_0x94Bu 2379u, (9, 4, B), 0x94B, 0x94B, 1, 0x6B4u
#define PPUTLIMPL_UHEX_0x94Au 2378u, (9, 4, A), 0x94A, 0x94A, 1, 0x6B5u
#define PPUTLIMPL_UHEX_0x949u 2377u, (9, 4, 9), 0x949, 0x949, 1, 0x6B6u
#define PPUTLIMPL_UHEX_0x948u 2376u, (9, 4, 8), 0x948, 0x948, 1, 0x6B7u
#define PPUTLIMPL_UHEX_0x947u 2375u, (9, 4, 7), 0x947, 0x947, 1, 0x6B8u
#define PPUTLIMPL_UHEX_0x946u 2374u, (9, 4, 6), 0x946, 0x946, 1, 0x6B9u
#define PPUTLIMPL_UHEX_0x945u 2373u, (9, 4, 5), 0x945, 0x945, 1, 0x6BAu
#define PPUTLIMPL_UHEX_0x944u 2372u, (9, 4, 4), 0x944, 0x944, 1, 0x6BBu
#define PPUTLIMPL_UHEX_0x943u 2371u, (9, 4, 3), 0x943, 0x943, 1, 0x6BCu
#define PPUTLIMPL_UHEX_0x942u 2370u, (9, 4, 2), 0x942, 0x942, 1, 0x6BDu
#define PPUTLIMPL_UHEX_0x941u 2369u, (9, 4, 1), 0x941, 0x941, 1, 0x6BEu
#define PPUTLIMPL_UHEX_0x940u 2368u, (9, 4, 0), 0x940, 0x940, 1, 0x6BFu
#define PPUTLIMPL_UHEX_0x93Fu 2367u, (9, 3, F), 0x93F, 0x93F, 1, 0x6C0u
#define PPUTLIMPL_UHEX_0x93Eu 2366u, (9, 3, E), 0x93E, 0x93E, 1, 0x6C1u
#define PPUTLIMPL_UHEX_0x93Du 2365u, (9, 3, D), 0x93D, 0x93D, 1, 0x6C2u
#define PPUTLIMPL_UHEX_0x93Cu 2364u, (9, 3, C), 0x93C, 0x93C, 1, 0x6C3u
#define PPUTLIMPL_UHEX_0x93Bu 2363u, (9, 3, B), 0x93B, 0x93B, 1, 0x6C4u
#define PPUTLIMPL_UHEX_0x93Au 2362u, (9, 3, A), 0x93A, 0x93A, 1, 0x6C5u
#define PPUTLIMPL_UHEX_0x939u 2361u, (9, 3, 9), 0x939, 0x939, 1, 0x6C6u
#define PPUTLIMPL_UHEX_0x938u 2360u, (9, 3, 8), 0x938, 0x938, 1, 0x6C7u
#define PPUTLIMPL_UHEX_0x937u 2359u, (9, 3, 7), 0x937, 0x937, 1, 0x6C8u
#define PPUTLIMPL_UHEX_0x936u 2358u, (9, 3, 6), 0x936, 0x936, 1, 0x6C9u
#define PPUTLIMPL_UHEX_0x935u 2357u, (9, 3, 5), 0x935, 0x935, 1, 0x6CAu
#define PPUTLIMPL_UHEX_0x934u 2356u, (9, 3, 4), 0x934, 0x934, 1, 0x6CBu
#define PPUTLIMPL_UHEX_0x933u 2355u, (9, 3, 3), 0x933, 0x933, 1, 0x6CCu
#define PPUTLIMPL_UHEX_0x932u 2354u, (9, 3, 2), 0x932, 0x932, 1, 0x6CDu
#define PPUTLIMPL_UHEX_0x931u 2353u, (9, 3, 1), 0x931, 0x931, 1, 0x6CEu
#define PPUTLIMPL_UHEX_0x930u 2352u, (9, 3, 0), 0x930, 0x930, 1, 0x6CFu
#define PPUTLIMPL_UHEX_0x92Fu 2351u, (9, 2, F), 0x92F, 0x92F, 1, 0x6D0u
#define PPUTLIMPL_UHEX_0x92Eu 2350u, (9, 2, E), 0x92E, 0x92E, 1, 0x6D1u
#define PPUTLIMPL_UHEX_0x92Du 2349u, (9, 2, D), 0x92D, 0x92D, 1, 0x6D2u
#define PPUTLIMPL_UHEX_0x92Cu 2348u, (9, 2, C), 0x92C, 0x92C, 1, 0x6D3u
#define PPUTLIMPL_UHEX_0x92Bu 2347u, (9, 2, B), 0x92B, 0x92B, 1, 0x6D4u
#define PPUTLIMPL_UHEX_0x92Au 2346u, (9, 2, A), 0x92A, 0x92A, 1, 0x6D5u
#define PPUTLIMPL_UHEX_0x929u 2345u, (9, 2, 9), 0x929, 0x929, 1, 0x6D6u
#define PPUTLIMPL_UHEX_0x928u 2344u, (9, 2, 8), 0x928, 0x928, 1, 0x6D7u
#define PPUTLIMPL_UHEX_0x927u 2343u, (9, 2, 7), 0x927, 0x927, 1, 0x6D8u
#define PPUTLIMPL_UHEX_0x926u 2342u, (9, 2, 6), 0x926, 0x926, 1, 0x6D9u
#define PPUTLIMPL_UHEX_0x925u 2341u, (9, 2, 5), 0x925, 0x925, 1, 0x6DAu
#define PPUTLIMPL_UHEX_0x924u 2340u, (9, 2, 4), 0x924, 0x924, 1, 0x6DBu
#define PPUTLIMPL_UHEX_0x923u 2339u, (9, 2, 3), 0x923, 0x923, 1, 0x6DCu
#define PPUTLIMPL_UHEX_0x922u 2338u, (9, 2, 2), 0x922, 0x922, 1, 0x6DDu
#define PPUTLIMPL_UHEX_0x921u 2337u, (9, 2, 1), 0x921, 0x921, 1, 0x6DEu
#define PPUTLIMPL_UHEX_0x920u 2336u, (9, 2, 0), 0x920, 0x920, 1, 0x6DFu
#define PPUTLIMPL_UHEX_0x91Fu 2335u, (9, 1, F), 0x91F, 0x91F, 1, 0x6E0u
#define PPUTLIMPL_UHEX_0x91Eu 2334u, (9, 1, E), 0x91E, 0x91E, 1, 0x6E1u
#define PPUTLIMPL_UHEX_0x91Du 2333u, (9, 1, D), 0x91D, 0x91D, 1, 0x6E2u
#define PPUTLIMPL_UHEX_0x91Cu 2332u, (9, 1, C), 0x91C, 0x91C, 1, 0x6E3u
#define PPUTLIMPL_UHEX_0x91Bu 2331u, (9, 1, B), 0x91B, 0x91B, 1, 0x6E4u
#define PPUTLIMPL_UHEX_0x91Au 2330u, (9, 1, A), 0x91A, 0x91A, 1, 0x6E5u
#define PPUTLIMPL_UHEX_0x919u 2329u, (9, 1, 9), 0x919, 0x919, 1, 0x6E6u
#define PPUTLIMPL_UHEX_0x918u 2328u, (9, 1, 8), 0x918, 0x918, 1, 0x6E7u
#define PPUTLIMPL_UHEX_0x917u 2327u, (9, 1, 7), 0x917, 0x917, 1, 0x6E8u
#define PPUTLIMPL_UHEX_0x916u 2326u, (9, 1, 6), 0x916, 0x916, 1, 0x6E9u
#define PPUTLIMPL_UHEX_0x915u 2325u, (9, 1, 5), 0x915, 0x915, 1, 0x6EAu
#define PPUTLIMPL_UHEX_0x914u 2324u, (9, 1, 4), 0x914, 0x914, 1, 0x6EBu
#define PPUTLIMPL_UHEX_0x913u 2323u, (9, 1, 3), 0x913, 0x913, 1, 0x6ECu
#define PPUTLIMPL_UHEX_0x912u 2322u, (9, 1, 2), 0x912, 0x912, 1, 0x6EDu
#define PPUTLIMPL_UHEX_0x911u 2321u, (9, 1, 1), 0x911, 0x911, 1, 0x6EEu
#define PPUTLIMPL_UHEX_0x910u 2320u, (9, 1, 0), 0x910, 0x910, 1, 0x6EFu
#define PPUTLIMPL_UHEX_0x90Fu 2319u, (9, 0, F), 0x90F, 0x90F, 1, 0x6F0u
#define PPUTLIMPL_UHEX_0x90Eu 2318u, (9, 0, E), 0x90E, 0x90E, 1, 0x6F1u
#define PPUTLIMPL_UHEX_0x90Du 2317u, (9, 0, D), 0x90D, 0x90D, 1, 0x6F2u
#define PPUTLIMPL_UHEX_0x90Cu 2316u, (9, 0, C), 0x90C, 0x90C, 1, 0x6F3u
#define PPUTLIMPL_UHEX_0x90Bu 2315u, (9, 0, B), 0x90B, 0x90B, 1, 0x6F4u
#define PPUTLIMPL_UHEX_0x90Au 2314u, (9, 0, A), 0x90A, 0x90A, 1, 0x6F5u
#define PPUTLIMPL_UHEX_0x909u 2313u, (9, 0, 9), 0x909, 0x909, 1, 0x6F6u
#define PPUTLIMPL_UHEX_0x908u 2312u, (9, 0, 8), 0x908, 0x908, 1, 0x6F7u
#define PPUTLIMPL_UHEX_0x907u 2311u, (9, 0, 7), 0x907, 0x907, 1, 0x6F8u
#define PPUTLIMPL_UHEX_0x906u 2310u, (9, 0, 6), 0x906, 0x906, 1, 0x6F9u
#define PPUTLIMPL_UHEX_0x905u 2309u, (9, 0, 5), 0x905, 0x905, 1, 0x6FAu
#define PPUTLIMPL_UHEX_0x904u 2308u, (9, 0, 4), 0x904, 0x904, 1, 0x6FBu
#define PPUTLIMPL_UHEX_0x903u 2307u, (9, 0, 3), 0x903, 0x903, 1, 0x6FCu
#define PPUTLIMPL_UHEX_0x902u 2306u, (9, 0, 2), 0x902, 0x902, 1, 0x6FDu
#define PPUTLIMPL_UHEX_0x901u 2305u, (9, 0, 1), 0x901, 0x901, 1, 0x6FEu
#define PPUTLIMPL_UHEX_0x900u 2304u, (9, 0, 0), 0x900, 0x900, 1, 0x6FFu
#define PPUTLIMPL_UHEX_0x8FFu 2303u, (8, F, F), 0x8FF, 0x8FF, 1, 0x700u
#define PPUTLIMPL_UHEX_0x8FEu 2302u, (8, F, E), 0x8FE, 0x8FE, 1, 0x701u
#define PPUTLIMPL_UHEX_0x8FDu 2301u, (8, F, D), 0x8FD, 0x8FD, 1, 0x702u
#define PPUTLIMPL_UHEX_0x8FCu 2300u, (8, F, C), 0x8FC, 0x8FC, 1, 0x703u
#define PPUTLIMPL_UHEX_0x8FBu 2299u, (8, F, B), 0x8FB, 0x8FB, 1, 0x704u
#define PPUTLIMPL_UHEX_0x8FAu 2298u, (8, F, A), 0x8FA, 0x8FA, 1, 0x705u
#define PPUTLIMPL_UHEX_0x8F9u 2297u, (8, F, 9), 0x8F9, 0x8F9, 1, 0x706u
#define PPUTLIMPL_UHEX_0x8F8u 2296u, (8, F, 8), 0x8F8, 0x8F8, 1, 0x707u
#define PPUTLIMPL_UHEX_0x8F7u 2295u, (8, F, 7), 0x8F7, 0x8F7, 1, 0x708u
#define PPUTLIMPL_UHEX_0x8F6u 2294u, (8, F, 6), 0x8F6, 0x8F6, 1, 0x709u
#define PPUTLIMPL_UHEX_0x8F5u 2293u, (8, F, 5), 0x8F5, 0x8F5, 1, 0x70Au
#define PPUTLIMPL_UHEX_0x8F4u 2292u, (8, F, 4), 0x8F4, 0x8F4, 1, 0x70Bu
#define PPUTLIMPL_UHEX_0x8F3u 2291u, (8, F, 3), 0x8F3, 0x8F3, 1, 0x70Cu
#define PPUTLIMPL_UHEX_0x8F2u 2290u, (8, F, 2), 0x8F2, 0x8F2, 1, 0x70Du
#define PPUTLIMPL_UHEX_0x8F1u 2289u, (8, F, 1), 0x8F1, 0x8F1, 1, 0x70Eu
#define PPUTLIMPL_UHEX_0x8F0u 2288u, (8, F, 0), 0x8F0, 0x8F0, 1, 0x70Fu
#define PPUTLIMPL_UHEX_0x8EFu 2287u, (8, E, F), 0x8EF, 0x8EF, 1, 0x710u
#define PPUTLIMPL_UHEX_0x8EEu 2286u, (8, E, E), 0x8EE, 0x8EE, 1, 0x711u
#define PPUTLIMPL_UHEX_0x8EDu 2285u, (8, E, D), 0x8ED, 0x8ED, 1, 0x712u
#define PPUTLIMPL_UHEX_0x8ECu 2284u, (8, E, C), 0x8EC, 0x8EC, 1, 0x713u
#define PPUTLIMPL_UHEX_0x8EBu 2283u, (8, E, B), 0x8EB, 0x8EB, 1, 0x714u
#define PPUTLIMPL_UHEX_0x8EAu 2282u, (8, E, A), 0x8EA, 0x8EA, 1, 0x715u
#define PPUTLIMPL_UHEX_0x8E9u 2281u, (8, E, 9), 0x8E9, 0x8E9, 1, 0x716u
#define PPUTLIMPL_UHEX_0x8E8u 2280u, (8, E, 8), 0x8E8, 0x8E8, 1, 0x717u
#define PPUTLIMPL_UHEX_0x8E7u 2279u, (8, E, 7), 0x8E7, 0x8E7, 1, 0x718u
#define PPUTLIMPL_UHEX_0x8E6u 2278u, (8, E, 6), 0x8E6, 0x8E6, 1, 0x719u
#define PPUTLIMPL_UHEX_0x8E5u 2277u, (8, E, 5), 0x8E5, 0x8E5, 1, 0x71Au
#define PPUTLIMPL_UHEX_0x8E4u 2276u, (8, E, 4), 0x8E4, 0x8E4, 1, 0x71Bu
#define PPUTLIMPL_UHEX_0x8E3u 2275u, (8, E, 3), 0x8E3, 0x8E3, 1, 0x71Cu
#define PPUTLIMPL_UHEX_0x8E2u 2274u, (8, E, 2), 0x8E2, 0x8E2, 1, 0x71Du
#define PPUTLIMPL_UHEX_0x8E1u 2273u, (8, E, 1), 0x8E1, 0x8E1, 1, 0x71Eu
#define PPUTLIMPL_UHEX_0x8E0u 2272u, (8, E, 0), 0x8E0, 0x8E0, 1, 0x71Fu
#define PPUTLIMPL_UHEX_0x8DFu 2271u, (8, D, F), 0x8DF, 0x8DF, 1, 0x720u
#define PPUTLIMPL_UHEX_0x8DEu 2270u, (8, D, E), 0x8DE, 0x8DE, 1, 0x721u
#define PPUTLIMPL_UHEX_0x8DDu 2269u, (8, D, D), 0x8DD, 0x8DD, 1, 0x722u
#define PPUTLIMPL_UHEX_0x8DCu 2268u, (8, D, C), 0x8DC, 0x8DC, 1, 0x723u
#define PPUTLIMPL_UHEX_0x8DBu 2267u, (8, D, B), 0x8DB, 0x8DB, 1, 0x724u
#define PPUTLIMPL_UHEX_0x8DAu 2266u, (8, D, A), 0x8DA, 0x8DA, 1, 0x725u
#define PPUTLIMPL_UHEX_0x8D9u 2265u, (8, D, 9), 0x8D9, 0x8D9, 1, 0x726u
#define PPUTLIMPL_UHEX_0x8D8u 2264u, (8, D, 8), 0x8D8, 0x8D8, 1, 0x727u
#define PPUTLIMPL_UHEX_0x8D7u 2263u, (8, D, 7), 0x8D7, 0x8D7, 1, 0x728u
#define PPUTLIMPL_UHEX_0x8D6u 2262u, (8, D, 6), 0x8D6, 0x8D6, 1, 0x729u
#define PPUTLIMPL_UHEX_0x8D5u 2261u, (8, D, 5), 0x8D5, 0x8D5, 1, 0x72Au
#define PPUTLIMPL_UHEX_0x8D4u 2260u, (8, D, 4), 0x8D4, 0x8D4, 1, 0x72Bu
#define PPUTLIMPL_UHEX_0x8D3u 2259u, (8, D, 3), 0x8D3, 0x8D3, 1, 0x72Cu
#define PPUTLIMPL_UHEX_0x8D2u 2258u, (8, D, 2), 0x8D2, 0x8D2, 1, 0x72Du
#define PPUTLIMPL_UHEX_0x8D1u 2257u, (8, D, 1), 0x8D1, 0x8D1, 1, 0x72Eu
#define PPUTLIMPL_UHEX_0x8D0u 2256u, (8, D, 0), 0x8D0, 0x8D0, 1, 0x72Fu
#define PPUTLIMPL_UHEX_0x8CFu 2255u, (8, C, F), 0x8CF, 0x8CF, 1, 0x730u
#define PPUTLIMPL_UHEX_0x8CEu 2254u, (8, C, E), 0x8CE, 0x8CE, 1, 0x731u
#define PPUTLIMPL_UHEX_0x8CDu 2253u, (8, C, D), 0x8CD, 0x8CD, 1, 0x732u
#define PPUTLIMPL_UHEX_0x8CCu 2252u, (8, C, C), 0x8CC, 0x8CC, 1, 0x733u
#define PPUTLIMPL_UHEX_0x8CBu 2251u, (8, C, B), 0x8CB, 0x8CB, 1, 0x734u
#define PPUTLIMPL_UHEX_0x8CAu 2250u, (8, C, A), 0x8CA, 0x8CA, 1, 0x735u
#define PPUTLIMPL_UHEX_0x8C9u 2249u, (8, C, 9), 0x8C9, 0x8C9, 1, 0x736u
#define PPUTLIMPL_UHEX_0x8C8u 2248u, (8, C, 8), 0x8C8, 0x8C8, 1, 0x737u
#define PPUTLIMPL_UHEX_0x8C7u 2247u, (8, C, 7), 0x8C7, 0x8C7, 1, 0x738u
#define PPUTLIMPL_UHEX_0x8C6u 2246u, (8, C, 6), 0x8C6, 0x8C6, 1, 0x739u
#define PPUTLIMPL_UHEX_0x8C5u 2245u, (8, C, 5), 0x8C5, 0x8C5, 1, 0x73Au
#define PPUTLIMPL_UHEX_0x8C4u 2244u, (8, C, 4), 0x8C4, 0x8C4, 1, 0x73Bu
#define PPUTLIMPL_UHEX_0x8C3u 2243u, (8, C, 3), 0x8C3, 0x8C3, 1, 0x73Cu
#define PPUTLIMPL_UHEX_0x8C2u 2242u, (8, C, 2), 0x8C2, 0x8C2, 1, 0x73Du
#define PPUTLIMPL_UHEX_0x8C1u 2241u, (8, C, 1), 0x8C1, 0x8C1, 1, 0x73Eu
#define PPUTLIMPL_UHEX_0x8C0u 2240u, (8, C, 0), 0x8C0, 0x8C0, 1, 0x73Fu
#define PPUTLIMPL_UHEX_0x8BFu 2239u, (8, B, F), 0x8BF, 0x8BF, 1, 0x740u
#define PPUTLIMPL_UHEX_0x8BEu 2238u, (8, B, E), 0x8BE, 0x8BE, 1, 0x741u
#define PPUTLIMPL_UHEX_0x8BDu 2237u, (8, B, D), 0x8BD, 0x8BD, 1, 0x742u
#define PPUTLIMPL_UHEX_0x8BCu 2236u, (8, B, C), 0x8BC, 0x8BC, 1, 0x743u
#define PPUTLIMPL_UHEX_0x8BBu 2235u, (8, B, B), 0x8BB, 0x8BB, 1, 0x744u
#define PPUTLIMPL_UHEX_0x8BAu 2234u, (8, B, A), 0x8BA, 0x8BA, 1, 0x745u
#define PPUTLIMPL_UHEX_0x8B9u 2233u, (8, B, 9), 0x8B9, 0x8B9, 1, 0x746u
#define PPUTLIMPL_UHEX_0x8B8u 2232u, (8, B, 8), 0x8B8, 0x8B8, 1, 0x747u
#define PPUTLIMPL_UHEX_0x8B7u 2231u, (8, B, 7), 0x8B7, 0x8B7, 1, 0x748u
#define PPUTLIMPL_UHEX_0x8B6u 2230u, (8, B, 6), 0x8B6, 0x8B6, 1, 0x749u
#define PPUTLIMPL_UHEX_0x8B5u 2229u, (8, B, 5), 0x8B5, 0x8B5, 1, 0x74Au
#define PPUTLIMPL_UHEX_0x8B4u 2228u, (8, B, 4), 0x8B4, 0x8B4, 1, 0x74Bu
#define PPUTLIMPL_UHEX_0x8B3u 2227u, (8, B, 3), 0x8B3, 0x8B3, 1, 0x74Cu
#define PPUTLIMPL_UHEX_0x8B2u 2226u, (8, B, 2), 0x8B2, 0x8B2, 1, 0x74Du
#define PPUTLIMPL_UHEX_0x8B1u 2225u, (8, B, 1), 0x8B1, 0x8B1, 1, 0x74Eu
#define PPUTLIMPL_UHEX_0x8B0u 2224u, (8, B, 0), 0x8B0, 0x8B0, 1, 0x74Fu
#define PPUTLIMPL_UHEX_0x8AFu 2223u, (8, A, F), 0x8AF, 0x8AF, 1, 0x750u
#define PPUTLIMPL_UHEX_0x8AEu 2222u, (8, A, E), 0x8AE, 0x8AE, 1, 0x751u
#define PPUTLIMPL_UHEX_0x8ADu 2221u, (8, A, D), 0x8AD, 0x8AD, 1, 0x752u
#define PPUTLIMPL_UHEX_0x8ACu 2220u, (8, A, C), 0x8AC, 0x8AC, 1, 0x753u
#define PPUTLIMPL_UHEX_0x8ABu 2219u, (8, A, B), 0x8AB, 0x8AB, 1, 0x754u
#define PPUTLIMPL_UHEX_0x8AAu 2218u, (8, A, A), 0x8AA, 0x8AA, 1, 0x755u
#define PPUTLIMPL_UHEX_0x8A9u 2217u, (8, A, 9), 0x8A9, 0x8A9, 1, 0x756u
#define PPUTLIMPL_UHEX_0x8A8u 2216u, (8, A, 8), 0x8A8, 0x8A8, 1, 0x757u
#define PPUTLIMPL_UHEX_0x8A7u 2215u, (8, A, 7), 0x8A7, 0x8A7, 1, 0x758u
#define PPUTLIMPL_UHEX_0x8A6u 2214u, (8, A, 6), 0x8A6, 0x8A6, 1, 0x759u
#define PPUTLIMPL_UHEX_0x8A5u 2213u, (8, A, 5), 0x8A5, 0x8A5, 1, 0x75Au
#define PPUTLIMPL_UHEX_0x8A4u 2212u, (8, A, 4), 0x8A4, 0x8A4, 1, 0x75Bu
#define PPUTLIMPL_UHEX_0x8A3u 2211u, (8, A, 3), 0x8A3, 0x8A3, 1, 0x75Cu
#define PPUTLIMPL_UHEX_0x8A2u 2210u, (8, A, 2), 0x8A2, 0x8A2, 1, 0x75Du
#define PPUTLIMPL_UHEX_0x8A1u 2209u, (8, A, 1), 0x8A1, 0x8A1, 1, 0x75Eu
#define PPUTLIMPL_UHEX_0x8A0u 2208u, (8, A, 0), 0x8A0, 0x8A0, 1, 0x75Fu
#define PPUTLIMPL_UHEX_0x89Fu 2207u, (8, 9, F), 0x89F, 0x89F, 1, 0x760u
#define PPUTLIMPL_UHEX_0x89Eu 2206u, (8, 9, E), 0x89E, 0x89E, 1, 0x761u
#define PPUTLIMPL_UHEX_0x89Du 2205u, (8, 9, D), 0x89D, 0x89D, 1, 0x762u
#define PPUTLIMPL_UHEX_0x89Cu 2204u, (8, 9, C), 0x89C, 0x89C, 1, 0x763u
#define PPUTLIMPL_UHEX_0x89Bu 2203u, (8, 9, B), 0x89B, 0x89B, 1, 0x764u
#define PPUTLIMPL_UHEX_0x89Au 2202u, (8, 9, A), 0x89A, 0x89A, 1, 0x765u
#define PPUTLIMPL_UHEX_0x899u 2201u, (8, 9, 9), 0x899, 0x899, 1, 0x766u
#define PPUTLIMPL_UHEX_0x898u 2200u, (8, 9, 8), 0x898, 0x898, 1, 0x767u
#define PPUTLIMPL_UHEX_0x897u 2199u, (8, 9, 7), 0x897, 0x897, 1, 0x768u
#define PPUTLIMPL_UHEX_0x896u 2198u, (8, 9, 6), 0x896, 0x896, 1, 0x769u
#define PPUTLIMPL_UHEX_0x895u 2197u, (8, 9, 5), 0x895, 0x895, 1, 0x76Au
#define PPUTLIMPL_UHEX_0x894u 2196u, (8, 9, 4), 0x894, 0x894, 1, 0x76Bu
#define PPUTLIMPL_UHEX_0x893u 2195u, (8, 9, 3), 0x893, 0x893, 1, 0x76Cu
#define PPUTLIMPL_UHEX_0x892u 2194u, (8, 9, 2), 0x892, 0x892, 1, 0x76Du
#define PPUTLIMPL_UHEX_0x891u 2193u, (8, 9, 1), 0x891, 0x891, 1, 0x76Eu
#define PPUTLIMPL_UHEX_0x890u 2192u, (8, 9, 0), 0x890, 0x890, 1, 0x76Fu
#define PPUTLIMPL_UHEX_0x88Fu 2191u, (8, 8, F), 0x88F, 0x88F, 1, 0x770u
#define PPUTLIMPL_UHEX_0x88Eu 2190u, (8, 8, E), 0x88E, 0x88E, 1, 0x771u
#define PPUTLIMPL_UHEX_0x88Du 2189u, (8, 8, D), 0x88D, 0x88D, 1, 0x772u
#define PPUTLIMPL_UHEX_0x88Cu 2188u, (8, 8, C), 0x88C, 0x88C, 1, 0x773u
#define PPUTLIMPL_UHEX_0x88Bu 2187u, (8, 8, B), 0x88B, 0x88B, 1, 0x774u
#define PPUTLIMPL_UHEX_0x88Au 2186u, (8, 8, A), 0x88A, 0x88A, 1, 0x775u
#define PPUTLIMPL_UHEX_0x889u 2185u, (8, 8, 9), 0x889, 0x889, 1, 0x776u
#define PPUTLIMPL_UHEX_0x888u 2184u, (8, 8, 8), 0x888, 0x888, 1, 0x777u
#define PPUTLIMPL_UHEX_0x887u 2183u, (8, 8, 7), 0x887, 0x887, 1, 0x778u
#define PPUTLIMPL_UHEX_0x886u 2182u, (8, 8, 6), 0x886, 0x886, 1, 0x779u
#define PPUTLIMPL_UHEX_0x885u 2181u, (8, 8, 5), 0x885, 0x885, 1, 0x77Au
#define PPUTLIMPL_UHEX_0x884u 2180u, (8, 8, 4), 0x884, 0x884, 1, 0x77Bu
#define PPUTLIMPL_UHEX_0x883u 2179u, (8, 8, 3), 0x883, 0x883, 1, 0x77Cu
#define PPUTLIMPL_UHEX_0x882u 2178u, (8, 8, 2), 0x882, 0x882, 1, 0x77Du
#define PPUTLIMPL_UHEX_0x881u 2177u, (8, 8, 1), 0x881, 0x881, 1, 0x77Eu
#define PPUTLIMPL_UHEX_0x880u 2176u, (8, 8, 0), 0x880, 0x880, 1, 0x77Fu
#define PPUTLIMPL_UHEX_0x87Fu 2175u, (8, 7, F), 0x87F, 0x87F, 1, 0x780u
#define PPUTLIMPL_UHEX_0x87Eu 2174u, (8, 7, E), 0x87E, 0x87E, 1, 0x781u
#define PPUTLIMPL_UHEX_0x87Du 2173u, (8, 7, D), 0x87D, 0x87D, 1, 0x782u
#define PPUTLIMPL_UHEX_0x87Cu 2172u, (8, 7, C), 0x87C, 0x87C, 1, 0x783u
#define PPUTLIMPL_UHEX_0x87Bu 2171u, (8, 7, B), 0x87B, 0x87B, 1, 0x784u
#define PPUTLIMPL_UHEX_0x87Au 2170u, (8, 7, A), 0x87A, 0x87A, 1, 0x785u
#define PPUTLIMPL_UHEX_0x879u 2169u, (8, 7, 9), 0x879, 0x879, 1, 0x786u
#define PPUTLIMPL_UHEX_0x878u 2168u, (8, 7, 8), 0x878, 0x878, 1, 0x787u
#define PPUTLIMPL_UHEX_0x877u 2167u, (8, 7, 7), 0x877, 0x877, 1, 0x788u
#define PPUTLIMPL_UHEX_0x876u 2166u, (8, 7, 6), 0x876, 0x876, 1, 0x789u
#define PPUTLIMPL_UHEX_0x875u 2165u, (8, 7, 5), 0x875, 0x875, 1, 0x78Au
#define PPUTLIMPL_UHEX_0x874u 2164u, (8, 7, 4), 0x874, 0x874, 1, 0x78Bu
#define PPUTLIMPL_UHEX_0x873u 2163u, (8, 7, 3), 0x873, 0x873, 1, 0x78Cu
#define PPUTLIMPL_UHEX_0x872u 2162u, (8, 7, 2), 0x872, 0x872, 1, 0x78Du
#define PPUTLIMPL_UHEX_0x871u 2161u, (8, 7, 1), 0x871, 0x871, 1, 0x78Eu
#define PPUTLIMPL_UHEX_0x870u 2160u, (8, 7, 0), 0x870, 0x870, 1, 0x78Fu
#define PPUTLIMPL_UHEX_0x86Fu 2159u, (8, 6, F), 0x86F, 0x86F, 1, 0x790u
#define PPUTLIMPL_UHEX_0x86Eu 2158u, (8, 6, E), 0x86E, 0x86E, 1, 0x791u
#define PPUTLIMPL_UHEX_0x86Du 2157u, (8, 6, D), 0x86D, 0x86D, 1, 0x792u
#define PPUTLIMPL_UHEX_0x86Cu 2156u, (8, 6, C), 0x86C, 0x86C, 1, 0x793u
#define PPUTLIMPL_UHEX_0x86Bu 2155u, (8, 6, B), 0x86B, 0x86B, 1, 0x794u
#define PPUTLIMPL_UHEX_0x86Au 2154u, (8, 6, A), 0x86A, 0x86A, 1, 0x795u
#define PPUTLIMPL_UHEX_0x869u 2153u, (8, 6, 9), 0x869, 0x869, 1, 0x796u
#define PPUTLIMPL_UHEX_0x868u 2152u, (8, 6, 8), 0x868, 0x868, 1, 0x797u
#define PPUTLIMPL_UHEX_0x867u 2151u, (8, 6, 7), 0x867, 0x867, 1, 0x798u
#define PPUTLIMPL_UHEX_0x866u 2150u, (8, 6, 6), 0x866, 0x866, 1, 0x799u
#define PPUTLIMPL_UHEX_0x865u 2149u, (8, 6, 5), 0x865, 0x865, 1, 0x79Au
#define PPUTLIMPL_UHEX_0x864u 2148u, (8, 6, 4), 0x864, 0x864, 1, 0x79Bu
#define PPUTLIMPL_UHEX_0x863u 2147u, (8, 6, 3), 0x863, 0x863, 1, 0x79Cu
#define PPUTLIMPL_UHEX_0x862u 2146u, (8, 6, 2), 0x862, 0x862, 1, 0x79Du
#define PPUTLIMPL_UHEX_0x861u 2145u, (8, 6, 1), 0x861, 0x861, 1, 0x79Eu
#define PPUTLIMPL_UHEX_0x860u 2144u, (8, 6, 0), 0x860, 0x860, 1, 0x79Fu
#define PPUTLIMPL_UHEX_0x85Fu 2143u, (8, 5, F), 0x85F, 0x85F, 1, 0x7A0u
#define PPUTLIMPL_UHEX_0x85Eu 2142u, (8, 5, E), 0x85E, 0x85E, 1, 0x7A1u
#define PPUTLIMPL_UHEX_0x85Du 2141u, (8, 5, D), 0x85D, 0x85D, 1, 0x7A2u
#define PPUTLIMPL_UHEX_0x85Cu 2140u, (8, 5, C), 0x85C, 0x85C, 1, 0x7A3u
#define PPUTLIMPL_UHEX_0x85Bu 2139u, (8, 5, B), 0x85B, 0x85B, 1, 0x7A4u
#define PPUTLIMPL_UHEX_0x85Au 2138u, (8, 5, A), 0x85A, 0x85A, 1, 0x7A5u
#define PPUTLIMPL_UHEX_0x859u 2137u, (8, 5, 9), 0x859, 0x859, 1, 0x7A6u
#define PPUTLIMPL_UHEX_0x858u 2136u, (8, 5, 8), 0x858, 0x858, 1, 0x7A7u
#define PPUTLIMPL_UHEX_0x857u 2135u, (8, 5, 7), 0x857, 0x857, 1, 0x7A8u
#define PPUTLIMPL_UHEX_0x856u 2134u, (8, 5, 6), 0x856, 0x856, 1, 0x7A9u
#define PPUTLIMPL_UHEX_0x855u 2133u, (8, 5, 5), 0x855, 0x855, 1, 0x7AAu
#define PPUTLIMPL_UHEX_0x854u 2132u, (8, 5, 4), 0x854, 0x854, 1, 0x7ABu
#define PPUTLIMPL_UHEX_0x853u 2131u, (8, 5, 3), 0x853, 0x853, 1, 0x7ACu
#define PPUTLIMPL_UHEX_0x852u 2130u, (8, 5, 2), 0x852, 0x852, 1, 0x7ADu
#define PPUTLIMPL_UHEX_0x851u 2129u, (8, 5, 1), 0x851, 0x851, 1, 0x7AEu
#define PPUTLIMPL_UHEX_0x850u 2128u, (8, 5, 0), 0x850, 0x850, 1, 0x7AFu
#define PPUTLIMPL_UHEX_0x84Fu 2127u, (8, 4, F), 0x84F, 0x84F, 1, 0x7B0u
#define PPUTLIMPL_UHEX_0x84Eu 2126u, (8, 4, E), 0x84E, 0x84E, 1, 0x7B1u
#define PPUTLIMPL_UHEX_0x84Du 2125u, (8, 4, D), 0x84D, 0x84D, 1, 0x7B2u
#define PPUTLIMPL_UHEX_0x84Cu 2124u, (8, 4, C), 0x84C, 0x84C, 1, 0x7B3u
#define PPUTLIMPL_UHEX_0x84Bu 2123u, (8, 4, B), 0x84B, 0x84B, 1, 0x7B4u
#define PPUTLIMPL_UHEX_0x84Au 2122u, (8, 4, A), 0x84A, 0x84A, 1, 0x7B5u
#define PPUTLIMPL_UHEX_0x849u 2121u, (8, 4, 9), 0x849, 0x849, 1, 0x7B6u
#define PPUTLIMPL_UHEX_0x848u 2120u, (8, 4, 8), 0x848, 0x848, 1, 0x7B7u
#define PPUTLIMPL_UHEX_0x847u 2119u, (8, 4, 7), 0x847, 0x847, 1, 0x7B8u
#define PPUTLIMPL_UHEX_0x846u 2118u, (8, 4, 6), 0x846, 0x846, 1, 0x7B9u
#define PPUTLIMPL_UHEX_0x845u 2117u, (8, 4, 5), 0x845, 0x845, 1, 0x7BAu
#define PPUTLIMPL_UHEX_0x844u 2116u, (8, 4, 4), 0x844, 0x844, 1, 0x7BBu
#define PPUTLIMPL_UHEX_0x843u 2115u, (8, 4, 3), 0x843, 0x843, 1, 0x7BCu
#define PPUTLIMPL_UHEX_0x842u 2114u, (8, 4, 2), 0x842, 0x842, 1, 0x7BDu
#define PPUTLIMPL_UHEX_0x841u 2113u, (8, 4, 1), 0x841, 0x841, 1, 0x7BEu
#define PPUTLIMPL_UHEX_0x840u 2112u, (8, 4, 0), 0x840, 0x840, 1, 0x7BFu
#define PPUTLIMPL_UHEX_0x83Fu 2111u, (8, 3, F), 0x83F, 0x83F, 1, 0x7C0u
#define PPUTLIMPL_UHEX_0x83Eu 2110u, (8, 3, E), 0x83E, 0x83E, 1, 0x7C1u
#define PPUTLIMPL_UHEX_0x83Du 2109u, (8, 3, D), 0x83D, 0x83D, 1, 0x7C2u
#define PPUTLIMPL_UHEX_0x83Cu 2108u, (8, 3, C), 0x83C, 0x83C, 1, 0x7C3u
#define PPUTLIMPL_UHEX_0x83Bu 2107u, (8, 3, B), 0x83B, 0x83B, 1, 0x7C4u
#define PPUTLIMPL_UHEX_0x83Au 2106u, (8, 3, A), 0x83A, 0x83A, 1, 0x7C5u
#define PPUTLIMPL_UHEX_0x839u 2105u, (8, 3, 9), 0x839, 0x839, 1, 0x7C6u
#define PPUTLIMPL_UHEX_0x838u 2104u, (8, 3, 8), 0x838, 0x838, 1, 0x7C7u
#define PPUTLIMPL_UHEX_0x837u 2103u, (8, 3, 7), 0x837, 0x837, 1, 0x7C8u
#define PPUTLIMPL_UHEX_0x836u 2102u, (8, 3, 6), 0x836, 0x836, 1, 0x7C9u
#define PPUTLIMPL_UHEX_0x835u 2101u, (8, 3, 5), 0x835, 0x835, 1, 0x7CAu
#define PPUTLIMPL_UHEX_0x834u 2100u, (8, 3, 4), 0x834, 0x834, 1, 0x7CBu
#define PPUTLIMPL_UHEX_0x833u 2099u, (8, 3, 3), 0x833, 0x833, 1, 0x7CCu
#define PPUTLIMPL_UHEX_0x832u 2098u, (8, 3, 2), 0x832, 0x832, 1, 0x7CDu
#define PPUTLIMPL_UHEX_0x831u 2097u, (8, 3, 1), 0x831, 0x831, 1, 0x7CEu
#define PPUTLIMPL_UHEX_0x830u 2096u, (8, 3, 0), 0x830, 0x830, 1, 0x7CFu
#define PPUTLIMPL_UHEX_0x82Fu 2095u, (8, 2, F), 0x82F, 0x82F, 1, 0x7D0u
#define PPUTLIMPL_UHEX_0x82Eu 2094u, (8, 2, E), 0x82E, 0x82E, 1, 0x7D1u
#define PPUTLIMPL_UHEX_0x82Du 2093u, (8, 2, D), 0x82D, 0x82D, 1, 0x7D2u
#define PPUTLIMPL_UHEX_0x82Cu 2092u, (8, 2, C), 0x82C, 0x82C, 1, 0x7D3u
#define PPUTLIMPL_UHEX_0x82Bu 2091u, (8, 2, B), 0x82B, 0x82B, 1, 0x7D4u
#define PPUTLIMPL_UHEX_0x82Au 2090u, (8, 2, A), 0x82A, 0x82A, 1, 0x7D5u
#define PPUTLIMPL_UHEX_0x829u 2089u, (8, 2, 9), 0x829, 0x829, 1, 0x7D6u
#define PPUTLIMPL_UHEX_0x828u 2088u, (8, 2, 8), 0x828, 0x828, 1, 0x7D7u
#define PPUTLIMPL_UHEX_0x827u 2087u, (8, 2, 7), 0x827, 0x827, 1, 0x7D8u
#define PPUTLIMPL_UHEX_0x826u 2086u, (8, 2, 6), 0x826, 0x826, 1, 0x7D9u
#define PPUTLIMPL_UHEX_0x825u 2085u, (8, 2, 5), 0x825, 0x825, 1, 0x7DAu
#define PPUTLIMPL_UHEX_0x824u 2084u, (8, 2, 4), 0x824, 0x824, 1, 0x7DBu
#define PPUTLIMPL_UHEX_0x823u 2083u, (8, 2, 3), 0x823, 0x823, 1, 0x7DCu
#define PPUTLIMPL_UHEX_0x822u 2082u, (8, 2, 2), 0x822, 0x822, 1, 0x7DDu
#define PPUTLIMPL_UHEX_0x821u 2081u, (8, 2, 1), 0x821, 0x821, 1, 0x7DEu
#define PPUTLIMPL_UHEX_0x820u 2080u, (8, 2, 0), 0x820, 0x820, 1, 0x7DFu
#define PPUTLIMPL_UHEX_0x81Fu 2079u, (8, 1, F), 0x81F, 0x81F, 1, 0x7E0u
#define PPUTLIMPL_UHEX_0x81Eu 2078u, (8, 1, E), 0x81E, 0x81E, 1, 0x7E1u
#define PPUTLIMPL_UHEX_0x81Du 2077u, (8, 1, D), 0x81D, 0x81D, 1, 0x7E2u
#define PPUTLIMPL_UHEX_0x81Cu 2076u, (8, 1, C), 0x81C, 0x81C, 1, 0x7E3u
#define PPUTLIMPL_UHEX_0x81Bu 2075u, (8, 1, B), 0x81B, 0x81B, 1, 0x7E4u
#define PPUTLIMPL_UHEX_0x81Au 2074u, (8, 1, A), 0x81A, 0x81A, 1, 0x7E5u
#define PPUTLIMPL_UHEX_0x819u 2073u, (8, 1, 9), 0x819, 0x819, 1, 0x7E6u
#define PPUTLIMPL_UHEX_0x818u 2072u, (8, 1, 8), 0x818, 0x818, 1, 0x7E7u
#define PPUTLIMPL_UHEX_0x817u 2071u, (8, 1, 7), 0x817, 0x817, 1, 0x7E8u
#define PPUTLIMPL_UHEX_0x816u 2070u, (8, 1, 6), 0x816, 0x816, 1, 0x7E9u
#define PPUTLIMPL_UHEX_0x815u 2069u, (8, 1, 5), 0x815, 0x815, 1, 0x7EAu
#define PPUTLIMPL_UHEX_0x814u 2068u, (8, 1, 4), 0x814, 0x814, 1, 0x7EBu
#define PPUTLIMPL_UHEX_0x813u 2067u, (8, 1, 3), 0x813, 0x813, 1, 0x7ECu
#define PPUTLIMPL_UHEX_0x812u 2066u, (8, 1, 2), 0x812, 0x812, 1, 0x7EDu
#define PPUTLIMPL_UHEX_0x811u 2065u, (8, 1, 1), 0x811, 0x811, 1, 0x7EEu
#define PPUTLIMPL_UHEX_0x810u 2064u, (8, 1, 0), 0x810, 0x810, 1, 0x7EFu
#define PPUTLIMPL_UHEX_0x80Fu 2063u, (8, 0, F), 0x80F, 0x80F, 1, 0x7F0u
#define PPUTLIMPL_UHEX_0x80Eu 2062u, (8, 0, E), 0x80E, 0x80E, 1, 0x7F1u
#define PPUTLIMPL_UHEX_0x80Du 2061u, (8, 0, D), 0x80D, 0x80D, 1, 0x7F2u
#define PPUTLIMPL_UHEX_0x80Cu 2060u, (8, 0, C), 0x80C, 0x80C, 1, 0x7F3u
#define PPUTLIMPL_UHEX_0x80Bu 2059u, (8, 0, B), 0x80B, 0x80B, 1, 0x7F4u
#define PPUTLIMPL_UHEX_0x80Au 2058u, (8, 0, A), 0x80A, 0x80A, 1, 0x7F5u
#define PPUTLIMPL_UHEX_0x809u 2057u, (8, 0, 9), 0x809, 0x809, 1, 0x7F6u
#define PPUTLIMPL_UHEX_0x808u 2056u, (8, 0, 8), 0x808, 0x808, 1, 0x7F7u
#define PPUTLIMPL_UHEX_0x807u 2055u, (8, 0, 7), 0x807, 0x807, 1, 0x7F8u
#define PPUTLIMPL_UHEX_0x806u 2054u, (8, 0, 6), 0x806, 0x806, 1, 0x7F9u
#define PPUTLIMPL_UHEX_0x805u 2053u, (8, 0, 5), 0x805, 0x805, 1, 0x7FAu
#define PPUTLIMPL_UHEX_0x804u 2052u, (8, 0, 4), 0x804, 0x804, 1, 0x7FBu
#define PPUTLIMPL_UHEX_0x803u 2051u, (8, 0, 3), 0x803, 0x803, 1, 0x7FCu
#define PPUTLIMPL_UHEX_0x802u 2050u, (8, 0, 2), 0x802, 0x802, 1, 0x7FDu
#define PPUTLIMPL_UHEX_0x801u 2049u, (8, 0, 1), 0x801, 0x801, 1, 0x7FEu
#define PPUTLIMPL_UHEX_0x800u 2048u, (8, 0, 0), 0x800, 0x800, 1, 0x7FFu
#define PPUTLIMPL_UHEX_0x7FFu 2047u, (7, F, F), 0x7FF, 2047, 0, 0x800u
#define PPUTLIMPL_UHEX_0x7FEu 2046u, (7, F, E), 0x7FE, 2046, 0, 0x801u
#define PPUTLIMPL_UHEX_0x7FDu 2045u, (7, F, D), 0x7FD, 2045, 0, 0x802u
#define PPUTLIMPL_UHEX_0x7FCu 2044u, (7, F, C), 0x7FC, 2044, 0, 0x803u
#define PPUTLIMPL_UHEX_0x7FBu 2043u, (7, F, B), 0x7FB, 2043, 0, 0x804u
#define PPUTLIMPL_UHEX_0x7FAu 2042u, (7, F, A), 0x7FA, 2042, 0, 0x805u
#define PPUTLIMPL_UHEX_0x7F9u 2041u, (7, F, 9), 0x7F9, 2041, 0, 0x806u
#define PPUTLIMPL_UHEX_0x7F8u 2040u, (7, F, 8), 0x7F8, 2040, 0, 0x807u
#define PPUTLIMPL_UHEX_0x7F7u 2039u, (7, F, 7), 0x7F7, 2039, 0, 0x808u
#define PPUTLIMPL_UHEX_0x7F6u 2038u, (7, F, 6), 0x7F6, 2038, 0, 0x809u
#define PPUTLIMPL_UHEX_0x7F5u 2037u, (7, F, 5), 0x7F5, 2037, 0, 0x80Au
#define PPUTLIMPL_UHEX_0x7F4u 2036u, (7, F, 4), 0x7F4, 2036, 0, 0x80Bu
#define PPUTLIMPL_UHEX_0x7F3u 2035u, (7, F, 3), 0x7F3, 2035, 0, 0x80Cu
#define PPUTLIMPL_UHEX_0x7F2u 2034u, (7, F, 2), 0x7F2, 2034, 0, 0x80Du
#define PPUTLIMPL_UHEX_0x7F1u 2033u, (7, F, 1), 0x7F1, 2033, 0, 0x80Eu
#define PPUTLIMPL_UHEX_0x7F0u 2032u, (7, F, 0), 0x7F0, 2032, 0, 0x80Fu
#define PPUTLIMPL_UHEX_0x7EFu 2031u, (7, E, F), 0x7EF, 2031, 0, 0x810u
#define PPUTLIMPL_UHEX_0x7EEu 2030u, (7, E, E), 0x7EE, 2030, 0, 0x811u
#define PPUTLIMPL_UHEX_0x7EDu 2029u, (7, E, D), 0x7ED, 2029, 0, 0x812u
#define PPUTLIMPL_UHEX_0x7ECu 2028u, (7, E, C), 0x7EC, 2028, 0, 0x813u
#define PPUTLIMPL_UHEX_0x7EBu 2027u, (7, E, B), 0x7EB, 2027, 0, 0x814u
#define PPUTLIMPL_UHEX_0x7EAu 2026u, (7, E, A), 0x7EA, 2026, 0, 0x815u
#define PPUTLIMPL_UHEX_0x7E9u 2025u, (7, E, 9), 0x7E9, 2025, 0, 0x816u
#define PPUTLIMPL_UHEX_0x7E8u 2024u, (7, E, 8), 0x7E8, 2024, 0, 0x817u
#define PPUTLIMPL_UHEX_0x7E7u 2023u, (7, E, 7), 0x7E7, 2023, 0, 0x818u
#define PPUTLIMPL_UHEX_0x7E6u 2022u, (7, E, 6), 0x7E6, 2022, 0, 0x819u
#define PPUTLIMPL_UHEX_0x7E5u 2021u, (7, E, 5), 0x7E5, 2021, 0, 0x81Au
#define PPUTLIMPL_UHEX_0x7E4u 2020u, (7, E, 4), 0x7E4, 2020, 0, 0x81Bu
#define PPUTLIMPL_UHEX_0x7E3u 2019u, (7, E, 3), 0x7E3, 2019, 0, 0x81Cu
#define PPUTLIMPL_UHEX_0x7E2u 2018u, (7, E, 2), 0x7E2, 2018, 0, 0x81Du
#define PPUTLIMPL_UHEX_0x7E1u 2017u, (7, E, 1), 0x7E1, 2017, 0, 0x81Eu
#define PPUTLIMPL_UHEX_0x7E0u 2016u, (7, E, 0), 0x7E0, 2016, 0, 0x81Fu
#define PPUTLIMPL_UHEX_0x7DFu 2015u, (7, D, F), 0x7DF, 2015, 0, 0x820u
#define PPUTLIMPL_UHEX_0x7DEu 2014u, (7, D, E), 0x7DE, 2014, 0, 0x821u
#define PPUTLIMPL_UHEX_0x7DDu 2013u, (7, D, D), 0x7DD, 2013, 0, 0x822u
#define PPUTLIMPL_UHEX_0x7DCu 2012u, (7, D, C), 0x7DC, 2012, 0, 0x823u
#define PPUTLIMPL_UHEX_0x7DBu 2011u, (7, D, B), 0x7DB, 2011, 0, 0x824u
#define PPUTLIMPL_UHEX_0x7DAu 2010u, (7, D, A), 0x7DA, 2010, 0, 0x825u
#define PPUTLIMPL_UHEX_0x7D9u 2009u, (7, D, 9), 0x7D9, 2009, 0, 0x826u
#define PPUTLIMPL_UHEX_0x7D8u 2008u, (7, D, 8), 0x7D8, 2008, 0, 0x827u
#define PPUTLIMPL_UHEX_0x7D7u 2007u, (7, D, 7), 0x7D7, 2007, 0, 0x828u
#define PPUTLIMPL_UHEX_0x7D6u 2006u, (7, D, 6), 0x7D6, 2006, 0, 0x829u
#define PPUTLIMPL_UHEX_0x7D5u 2005u, (7, D, 5), 0x7D5, 2005, 0, 0x82Au
#define PPUTLIMPL_UHEX_0x7D4u 2004u, (7, D, 4), 0x7D4, 2004, 0, 0x82Bu
#define PPUTLIMPL_UHEX_0x7D3u 2003u, (7, D, 3), 0x7D3, 2003, 0, 0x82Cu
#define PPUTLIMPL_UHEX_0x7D2u 2002u, (7, D, 2), 0x7D2, 2002, 0, 0x82Du
#define PPUTLIMPL_UHEX_0x7D1u 2001u, (7, D, 1), 0x7D1, 2001, 0, 0x82Eu
#define PPUTLIMPL_UHEX_0x7D0u 2000u, (7, D, 0), 0x7D0, 2000, 0, 0x82Fu
#define PPUTLIMPL_UHEX_0x7CFu 1999u, (7, C, F), 0x7CF, 1999, 0, 0x830u
#define PPUTLIMPL_UHEX_0x7CEu 1998u, (7, C, E), 0x7CE, 1998, 0, 0x831u
#define PPUTLIMPL_UHEX_0x7CDu 1997u, (7, C, D), 0x7CD, 1997, 0, 0x832u
#define PPUTLIMPL_UHEX_0x7CCu 1996u, (7, C, C), 0x7CC, 1996, 0, 0x833u
#define PPUTLIMPL_UHEX_0x7CBu 1995u, (7, C, B), 0x7CB, 1995, 0, 0x834u
#define PPUTLIMPL_UHEX_0x7CAu 1994u, (7, C, A), 0x7CA, 1994, 0, 0x835u
#define PPUTLIMPL_UHEX_0x7C9u 1993u, (7, C, 9), 0x7C9, 1993, 0, 0x836u
#define PPUTLIMPL_UHEX_0x7C8u 1992u, (7, C, 8), 0x7C8, 1992, 0, 0x837u
#define PPUTLIMPL_UHEX_0x7C7u 1991u, (7, C, 7), 0x7C7, 1991, 0, 0x838u
#define PPUTLIMPL_UHEX_0x7C6u 1990u, (7, C, 6), 0x7C6, 1990, 0, 0x839u
#define PPUTLIMPL_UHEX_0x7C5u 1989u, (7, C, 5), 0x7C5, 1989, 0, 0x83Au
#define PPUTLIMPL_UHEX_0x7C4u 1988u, (7, C, 4), 0x7C4, 1988, 0, 0x83Bu
#define PPUTLIMPL_UHEX_0x7C3u 1987u, (7, C, 3), 0x7C3, 1987, 0, 0x83Cu
#define PPUTLIMPL_UHEX_0x7C2u 1986u, (7, C, 2), 0x7C2, 1986, 0, 0x83Du
#define PPUTLIMPL_UHEX_0x7C1u 1985u, (7, C, 1), 0x7C1, 1985, 0, 0x83Eu
#define PPUTLIMPL_UHEX_0x7C0u 1984u, (7, C, 0), 0x7C0, 1984, 0, 0x83Fu
#define PPUTLIMPL_UHEX_0x7BFu 1983u, (7, B, F), 0x7BF, 1983, 0, 0x840u
#define PPUTLIMPL_UHEX_0x7BEu 1982u, (7, B, E), 0x7BE, 1982, 0, 0x841u
#define PPUTLIMPL_UHEX_0x7BDu 1981u, (7, B, D), 0x7BD, 1981, 0, 0x842u
#define PPUTLIMPL_UHEX_0x7BCu 1980u, (7, B, C), 0x7BC, 1980, 0, 0x843u
#define PPUTLIMPL_UHEX_0x7BBu 1979u, (7, B, B), 0x7BB, 1979, 0, 0x844u
#define PPUTLIMPL_UHEX_0x7BAu 1978u, (7, B, A), 0x7BA, 1978, 0, 0x845u
#define PPUTLIMPL_UHEX_0x7B9u 1977u, (7, B, 9), 0x7B9, 1977, 0, 0x846u
#define PPUTLIMPL_UHEX_0x7B8u 1976u, (7, B, 8), 0x7B8, 1976, 0, 0x847u
#define PPUTLIMPL_UHEX_0x7B7u 1975u, (7, B, 7), 0x7B7, 1975, 0, 0x848u
#define PPUTLIMPL_UHEX_0x7B6u 1974u, (7, B, 6), 0x7B6, 1974, 0, 0x849u
#define PPUTLIMPL_UHEX_0x7B5u 1973u, (7, B, 5), 0x7B5, 1973, 0, 0x84Au
#define PPUTLIMPL_UHEX_0x7B4u 1972u, (7, B, 4), 0x7B4, 1972, 0, 0x84Bu
#define PPUTLIMPL_UHEX_0x7B3u 1971u, (7, B, 3), 0x7B3, 1971, 0, 0x84Cu
#define PPUTLIMPL_UHEX_0x7B2u 1970u, (7, B, 2), 0x7B2, 1970, 0, 0x84Du
#define PPUTLIMPL_UHEX_0x7B1u 1969u, (7, B, 1), 0x7B1, 1969, 0, 0x84Eu
#define PPUTLIMPL_UHEX_0x7B0u 1968u, (7, B, 0), 0x7B0, 1968, 0, 0x84Fu
#define PPUTLIMPL_UHEX_0x7AFu 1967u, (7, A, F), 0x7AF, 1967, 0, 0x850u
#define PPUTLIMPL_UHEX_0x7AEu 1966u, (7, A, E), 0x7AE, 1966, 0, 0x851u
#define PPUTLIMPL_UHEX_0x7ADu 1965u, (7, A, D), 0x7AD, 1965, 0, 0x852u
#define PPUTLIMPL_UHEX_0x7ACu 1964u, (7, A, C), 0x7AC, 1964, 0, 0x853u
#define PPUTLIMPL_UHEX_0x7ABu 1963u, (7, A, B), 0x7AB, 1963, 0, 0x854u
#define PPUTLIMPL_UHEX_0x7AAu 1962u, (7, A, A), 0x7AA, 1962, 0, 0x855u
#define PPUTLIMPL_UHEX_0x7A9u 1961u, (7, A, 9), 0x7A9, 1961, 0, 0x856u
#define PPUTLIMPL_UHEX_0x7A8u 1960u, (7, A, 8), 0x7A8, 1960, 0, 0x857u
#define PPUTLIMPL_UHEX_0x7A7u 1959u, (7, A, 7), 0x7A7, 1959, 0, 0x858u
#define PPUTLIMPL_UHEX_0x7A6u 1958u, (7, A, 6), 0x7A6, 1958, 0, 0x859u
#define PPUTLIMPL_UHEX_0x7A5u 1957u, (7, A, 5), 0x7A5, 1957, 0, 0x85Au
#define PPUTLIMPL_UHEX_0x7A4u 1956u, (7, A, 4), 0x7A4, 1956, 0, 0x85Bu
#define PPUTLIMPL_UHEX_0x7A3u 1955u, (7, A, 3), 0x7A3, 1955, 0, 0x85Cu
#define PPUTLIMPL_UHEX_0x7A2u 1954u, (7, A, 2), 0x7A2, 1954, 0, 0x85Du
#define PPUTLIMPL_UHEX_0x7A1u 1953u, (7, A, 1), 0x7A1, 1953, 0, 0x85Eu
#define PPUTLIMPL_UHEX_0x7A0u 1952u, (7, A, 0), 0x7A0, 1952, 0, 0x85Fu
#define PPUTLIMPL_UHEX_0x79Fu 1951u, (7, 9, F), 0x79F, 1951, 0, 0x860u
#define PPUTLIMPL_UHEX_0x79Eu 1950u, (7, 9, E), 0x79E, 1950, 0, 0x861u
#define PPUTLIMPL_UHEX_0x79Du 1949u, (7, 9, D), 0x79D, 1949, 0, 0x862u
#define PPUTLIMPL_UHEX_0x79Cu 1948u, (7, 9, C), 0x79C, 1948, 0, 0x863u
#define PPUTLIMPL_UHEX_0x79Bu 1947u, (7, 9, B), 0x79B, 1947, 0, 0x864u
#define PPUTLIMPL_UHEX_0x79Au 1946u, (7, 9, A), 0x79A, 1946, 0, 0x865u
#define PPUTLIMPL_UHEX_0x799u 1945u, (7, 9, 9), 0x799, 1945, 0, 0x866u
#define PPUTLIMPL_UHEX_0x798u 1944u, (7, 9, 8), 0x798, 1944, 0, 0x867u
#define PPUTLIMPL_UHEX_0x797u 1943u, (7, 9, 7), 0x797, 1943, 0, 0x868u
#define PPUTLIMPL_UHEX_0x796u 1942u, (7, 9, 6), 0x796, 1942, 0, 0x869u
#define PPUTLIMPL_UHEX_0x795u 1941u, (7, 9, 5), 0x795, 1941, 0, 0x86Au
#define PPUTLIMPL_UHEX_0x794u 1940u, (7, 9, 4), 0x794, 1940, 0, 0x86Bu
#define PPUTLIMPL_UHEX_0x793u 1939u, (7, 9, 3), 0x793, 1939, 0, 0x86Cu
#define PPUTLIMPL_UHEX_0x792u 1938u, (7, 9, 2), 0x792, 1938, 0, 0x86Du
#define PPUTLIMPL_UHEX_0x791u 1937u, (7, 9, 1), 0x791, 1937, 0, 0x86Eu
#define PPUTLIMPL_UHEX_0x790u 1936u, (7, 9, 0), 0x790, 1936, 0, 0x86Fu
#define PPUTLIMPL_UHEX_0x78Fu 1935u, (7, 8, F), 0x78F, 1935, 0, 0x870u
#define PPUTLIMPL_UHEX_0x78Eu 1934u, (7, 8, E), 0x78E, 1934, 0, 0x871u
#define PPUTLIMPL_UHEX_0x78Du 1933u, (7, 8, D), 0x78D, 1933, 0, 0x872u
#define PPUTLIMPL_UHEX_0x78Cu 1932u, (7, 8, C), 0x78C, 1932, 0, 0x873u
#define PPUTLIMPL_UHEX_0x78Bu 1931u, (7, 8, B), 0x78B, 1931, 0, 0x874u
#define PPUTLIMPL_UHEX_0x78Au 1930u, (7, 8, A), 0x78A, 1930, 0, 0x875u
#define PPUTLIMPL_UHEX_0x789u 1929u, (7, 8, 9), 0x789, 1929, 0, 0x876u
#define PPUTLIMPL_UHEX_0x788u 1928u, (7, 8, 8), 0x788, 1928, 0, 0x877u
#define PPUTLIMPL_UHEX_0x787u 1927u, (7, 8, 7), 0x787, 1927, 0, 0x878u
#define PPUTLIMPL_UHEX_0x786u 1926u, (7, 8, 6), 0x786, 1926, 0, 0x879u
#define PPUTLIMPL_UHEX_0x785u 1925u, (7, 8, 5), 0x785, 1925, 0, 0x87Au
#define PPUTLIMPL_UHEX_0x784u 1924u, (7, 8, 4), 0x784, 1924, 0, 0x87Bu
#define PPUTLIMPL_UHEX_0x783u 1923u, (7, 8, 3), 0x783, 1923, 0, 0x87Cu
#define PPUTLIMPL_UHEX_0x782u 1922u, (7, 8, 2), 0x782, 1922, 0, 0x87Du
#define PPUTLIMPL_UHEX_0x781u 1921u, (7, 8, 1), 0x781, 1921, 0, 0x87Eu
#define PPUTLIMPL_UHEX_0x780u 1920u, (7, 8, 0), 0x780, 1920, 0, 0x87Fu
#define PPUTLIMPL_UHEX_0x77Fu 1919u, (7, 7, F), 0x77F, 1919, 0, 0x880u
#define PPUTLIMPL_UHEX_0x77Eu 1918u, (7, 7, E), 0x77E, 1918, 0, 0x881u
#define PPUTLIMPL_UHEX_0x77Du 1917u, (7, 7, D), 0x77D, 1917, 0, 0x882u
#define PPUTLIMPL_UHEX_0x77Cu 1916u, (7, 7, C), 0x77C, 1916, 0, 0x883u
#define PPUTLIMPL_UHEX_0x77Bu 1915u, (7, 7, B), 0x77B, 1915, 0, 0x884u
#define PPUTLIMPL_UHEX_0x77Au 1914u, (7, 7, A), 0x77A, 1914, 0, 0x885u
#define PPUTLIMPL_UHEX_0x779u 1913u, (7, 7, 9), 0x779, 1913, 0, 0x886u
#define PPUTLIMPL_UHEX_0x778u 1912u, (7, 7, 8), 0x778, 1912, 0, 0x887u
#define PPUTLIMPL_UHEX_0x777u 1911u, (7, 7, 7), 0x777, 1911, 0, 0x888u
#define PPUTLIMPL_UHEX_0x776u 1910u, (7, 7, 6), 0x776, 1910, 0, 0x889u
#define PPUTLIMPL_UHEX_0x775u 1909u, (7, 7, 5), 0x775, 1909, 0, 0x88Au
#define PPUTLIMPL_UHEX_0x774u 1908u, (7, 7, 4), 0x774, 1908, 0, 0x88Bu
#define PPUTLIMPL_UHEX_0x773u 1907u, (7, 7, 3), 0x773, 1907, 0, 0x88Cu
#define PPUTLIMPL_UHEX_0x772u 1906u, (7, 7, 2), 0x772, 1906, 0, 0x88Du
#define PPUTLIMPL_UHEX_0x771u 1905u, (7, 7, 1), 0x771, 1905, 0, 0x88Eu
#define PPUTLIMPL_UHEX_0x770u 1904u, (7, 7, 0), 0x770, 1904, 0, 0x88Fu
#define PPUTLIMPL_UHEX_0x76Fu 1903u, (7, 6, F), 0x76F, 1903, 0, 0x890u
#define PPUTLIMPL_UHEX_0x76Eu 1902u, (7, 6, E), 0x76E, 1902, 0, 0x891u
#define PPUTLIMPL_UHEX_0x76Du 1901u, (7, 6, D), 0x76D, 1901, 0, 0x892u
#define PPUTLIMPL_UHEX_0x76Cu 1900u, (7, 6, C), 0x76C, 1900, 0, 0x893u
#define PPUTLIMPL_UHEX_0x76Bu 1899u, (7, 6, B), 0x76B, 1899, 0, 0x894u
#define PPUTLIMPL_UHEX_0x76Au 1898u, (7, 6, A), 0x76A, 1898, 0, 0x895u
#define PPUTLIMPL_UHEX_0x769u 1897u, (7, 6, 9), 0x769, 1897, 0, 0x896u
#define PPUTLIMPL_UHEX_0x768u 1896u, (7, 6, 8), 0x768, 1896, 0, 0x897u
#define PPUTLIMPL_UHEX_0x767u 1895u, (7, 6, 7), 0x767, 1895, 0, 0x898u
#define PPUTLIMPL_UHEX_0x766u 1894u, (7, 6, 6), 0x766, 1894, 0, 0x899u
#define PPUTLIMPL_UHEX_0x765u 1893u, (7, 6, 5), 0x765, 1893, 0, 0x89Au
#define PPUTLIMPL_UHEX_0x764u 1892u, (7, 6, 4), 0x764, 1892, 0, 0x89Bu
#define PPUTLIMPL_UHEX_0x763u 1891u, (7, 6, 3), 0x763, 1891, 0, 0x89Cu
#define PPUTLIMPL_UHEX_0x762u 1890u, (7, 6, 2), 0x762, 1890, 0, 0x89Du
#define PPUTLIMPL_UHEX_0x761u 1889u, (7, 6, 1), 0x761, 1889, 0, 0x89Eu
#define PPUTLIMPL_UHEX_0x760u 1888u, (7, 6, 0), 0x760, 1888, 0, 0x89Fu
#define PPUTLIMPL_UHEX_0x75Fu 1887u, (7, 5, F), 0x75F, 1887, 0, 0x8A0u
#define PPUTLIMPL_UHEX_0x75Eu 1886u, (7, 5, E), 0x75E, 1886, 0, 0x8A1u
#define PPUTLIMPL_UHEX_0x75Du 1885u, (7, 5, D), 0x75D, 1885, 0, 0x8A2u
#define PPUTLIMPL_UHEX_0x75Cu 1884u, (7, 5, C), 0x75C, 1884, 0, 0x8A3u
#define PPUTLIMPL_UHEX_0x75Bu 1883u, (7, 5, B), 0x75B, 1883, 0, 0x8A4u
#define PPUTLIMPL_UHEX_0x75Au 1882u, (7, 5, A), 0x75A, 1882, 0, 0x8A5u
#define PPUTLIMPL_UHEX_0x759u 1881u, (7, 5, 9), 0x759, 1881, 0, 0x8A6u
#define PPUTLIMPL_UHEX_0x758u 1880u, (7, 5, 8), 0x758, 1880, 0, 0x8A7u
#define PPUTLIMPL_UHEX_0x757u 1879u, (7, 5, 7), 0x757, 1879, 0, 0x8A8u
#define PPUTLIMPL_UHEX_0x756u 1878u, (7, 5, 6), 0x756, 1878, 0, 0x8A9u
#define PPUTLIMPL_UHEX_0x755u 1877u, (7, 5, 5), 0x755, 1877, 0, 0x8AAu
#define PPUTLIMPL_UHEX_0x754u 1876u, (7, 5, 4), 0x754, 1876, 0, 0x8ABu
#define PPUTLIMPL_UHEX_0x753u 1875u, (7, 5, 3), 0x753, 1875, 0, 0x8ACu
#define PPUTLIMPL_UHEX_0x752u 1874u, (7, 5, 2), 0x752, 1874, 0, 0x8ADu
#define PPUTLIMPL_UHEX_0x751u 1873u, (7, 5, 1), 0x751, 1873, 0, 0x8AEu
#define PPUTLIMPL_UHEX_0x750u 1872u, (7, 5, 0), 0x750, 1872, 0, 0x8AFu
#define PPUTLIMPL_UHEX_0x74Fu 1871u, (7, 4, F), 0x74F, 1871, 0, 0x8B0u
#define PPUTLIMPL_UHEX_0x74Eu 1870u, (7, 4, E), 0x74E, 1870, 0, 0x8B1u
#define PPUTLIMPL_UHEX_0x74Du 1869u, (7, 4, D), 0x74D, 1869, 0, 0x8B2u
#define PPUTLIMPL_UHEX_0x74Cu 1868u, (7, 4, C), 0x74C, 1868, 0, 0x8B3u
#define PPUTLIMPL_UHEX_0x74Bu 1867u, (7, 4, B), 0x74B, 1867, 0, 0x8B4u
#define PPUTLIMPL_UHEX_0x74Au 1866u, (7, 4, A), 0x74A, 1866, 0, 0x8B5u
#define PPUTLIMPL_UHEX_0x749u 1865u, (7, 4, 9), 0x749, 1865, 0, 0x8B6u
#define PPUTLIMPL_UHEX_0x748u 1864u, (7, 4, 8), 0x748, 1864, 0, 0x8B7u
#define PPUTLIMPL_UHEX_0x747u 1863u, (7, 4, 7), 0x747, 1863, 0, 0x8B8u
#define PPUTLIMPL_UHEX_0x746u 1862u, (7, 4, 6), 0x746, 1862, 0, 0x8B9u
#define PPUTLIMPL_UHEX_0x745u 1861u, (7, 4, 5), 0x745, 1861, 0, 0x8BAu
#define PPUTLIMPL_UHEX_0x744u 1860u, (7, 4, 4), 0x744, 1860, 0, 0x8BBu
#define PPUTLIMPL_UHEX_0x743u 1859u, (7, 4, 3), 0x743, 1859, 0, 0x8BCu
#define PPUTLIMPL_UHEX_0x742u 1858u, (7, 4, 2), 0x742, 1858, 0, 0x8BDu
#define PPUTLIMPL_UHEX_0x741u 1857u, (7, 4, 1), 0x741, 1857, 0, 0x8BEu
#define PPUTLIMPL_UHEX_0x740u 1856u, (7, 4, 0), 0x740, 1856, 0, 0x8BFu
#define PPUTLIMPL_UHEX_0x73Fu 1855u, (7, 3, F), 0x73F, 1855, 0, 0x8C0u
#define PPUTLIMPL_UHEX_0x73Eu 1854u, (7, 3, E), 0x73E, 1854, 0, 0x8C1u
#define PPUTLIMPL_UHEX_0x73Du 1853u, (7, 3, D), 0x73D, 1853, 0, 0x8C2u
#define PPUTLIMPL_UHEX_0x73Cu 1852u, (7, 3, C), 0x73C, 1852, 0, 0x8C3u
#define PPUTLIMPL_UHEX_0x73Bu 1851u, (7, 3, B), 0x73B, 1851, 0, 0x8C4u
#define PPUTLIMPL_UHEX_0x73Au 1850u, (7, 3, A), 0x73A, 1850, 0, 0x8C5u
#define PPUTLIMPL_UHEX_0x739u 1849u, (7, 3, 9), 0x739, 1849, 0, 0x8C6u
#define PPUTLIMPL_UHEX_0x738u 1848u, (7, 3, 8), 0x738, 1848, 0, 0x8C7u
#define PPUTLIMPL_UHEX_0x737u 1847u, (7, 3, 7), 0x737, 1847, 0, 0x8C8u
#define PPUTLIMPL_UHEX_0x736u 1846u, (7, 3, 6), 0x736, 1846, 0, 0x8C9u
#define PPUTLIMPL_UHEX_0x735u 1845u, (7, 3, 5), 0x735, 1845, 0, 0x8CAu
#define PPUTLIMPL_UHEX_0x734u 1844u, (7, 3, 4), 0x734, 1844, 0, 0x8CBu
#define PPUTLIMPL_UHEX_0x733u 1843u, (7, 3, 3), 0x733, 1843, 0, 0x8CCu
#define PPUTLIMPL_UHEX_0x732u 1842u, (7, 3, 2), 0x732, 1842, 0, 0x8CDu
#define PPUTLIMPL_UHEX_0x731u 1841u, (7, 3, 1), 0x731, 1841, 0, 0x8CEu
#define PPUTLIMPL_UHEX_0x730u 1840u, (7, 3, 0), 0x730, 1840, 0, 0x8CFu
#define PPUTLIMPL_UHEX_0x72Fu 1839u, (7, 2, F), 0x72F, 1839, 0, 0x8D0u
#define PPUTLIMPL_UHEX_0x72Eu 1838u, (7, 2, E), 0x72E, 1838, 0, 0x8D1u
#define PPUTLIMPL_UHEX_0x72Du 1837u, (7, 2, D), 0x72D, 1837, 0, 0x8D2u
#define PPUTLIMPL_UHEX_0x72Cu 1836u, (7, 2, C), 0x72C, 1836, 0, 0x8D3u
#define PPUTLIMPL_UHEX_0x72Bu 1835u, (7, 2, B), 0x72B, 1835, 0, 0x8D4u
#define PPUTLIMPL_UHEX_0x72Au 1834u, (7, 2, A), 0x72A, 1834, 0, 0x8D5u
#define PPUTLIMPL_UHEX_0x729u 1833u, (7, 2, 9), 0x729, 1833, 0, 0x8D6u
#define PPUTLIMPL_UHEX_0x728u 1832u, (7, 2, 8), 0x728, 1832, 0, 0x8D7u
#define PPUTLIMPL_UHEX_0x727u 1831u, (7, 2, 7), 0x727, 1831, 0, 0x8D8u
#define PPUTLIMPL_UHEX_0x726u 1830u, (7, 2, 6), 0x726, 1830, 0, 0x8D9u
#define PPUTLIMPL_UHEX_0x725u 1829u, (7, 2, 5), 0x725, 1829, 0, 0x8DAu
#define PPUTLIMPL_UHEX_0x724u 1828u, (7, 2, 4), 0x724, 1828, 0, 0x8DBu
#define PPUTLIMPL_UHEX_0x723u 1827u, (7, 2, 3), 0x723, 1827, 0, 0x8DCu
#define PPUTLIMPL_UHEX_0x722u 1826u, (7, 2, 2), 0x722, 1826, 0, 0x8DDu
#define PPUTLIMPL_UHEX_0x721u 1825u, (7, 2, 1), 0x721, 1825, 0, 0x8DEu
#define PPUTLIMPL_UHEX_0x720u 1824u, (7, 2, 0), 0x720, 1824, 0, 0x8DFu
#define PPUTLIMPL_UHEX_0x71Fu 1823u, (7, 1, F), 0x71F, 1823, 0, 0x8E0u
#define PPUTLIMPL_UHEX_0x71Eu 1822u, (7, 1, E), 0x71E, 1822, 0, 0x8E1u
#define PPUTLIMPL_UHEX_0x71Du 1821u, (7, 1, D), 0x71D, 1821, 0, 0x8E2u
#define PPUTLIMPL_UHEX_0x71Cu 1820u, (7, 1, C), 0x71C, 1820, 0, 0x8E3u
#define PPUTLIMPL_UHEX_0x71Bu 1819u, (7, 1, B), 0x71B, 1819, 0, 0x8E4u
#define PPUTLIMPL_UHEX_0x71Au 1818u, (7, 1, A), 0x71A, 1818, 0, 0x8E5u
#define PPUTLIMPL_UHEX_0x719u 1817u, (7, 1, 9), 0x719, 1817, 0, 0x8E6u
#define PPUTLIMPL_UHEX_0x718u 1816u, (7, 1, 8), 0x718, 1816, 0, 0x8E7u
#define PPUTLIMPL_UHEX_0x717u 1815u, (7, 1, 7), 0x717, 1815, 0, 0x8E8u
#define PPUTLIMPL_UHEX_0x716u 1814u, (7, 1, 6), 0x716, 1814, 0, 0x8E9u
#define PPUTLIMPL_UHEX_0x715u 1813u, (7, 1, 5), 0x715, 1813, 0, 0x8EAu
#define PPUTLIMPL_UHEX_0x714u 1812u, (7, 1, 4), 0x714, 1812, 0, 0x8EBu
#define PPUTLIMPL_UHEX_0x713u 1811u, (7, 1, 3), 0x713, 1811, 0, 0x8ECu
#define PPUTLIMPL_UHEX_0x712u 1810u, (7, 1, 2), 0x712, 1810, 0, 0x8EDu
#define PPUTLIMPL_UHEX_0x711u 1809u, (7, 1, 1), 0x711, 1809, 0, 0x8EEu
#define PPUTLIMPL_UHEX_0x710u 1808u, (7, 1, 0), 0x710, 1808, 0, 0x8EFu
#define PPUTLIMPL_UHEX_0x70Fu 1807u, (7, 0, F), 0x70F, 1807, 0, 0x8F0u
#define PPUTLIMPL_UHEX_0x70Eu 1806u, (7, 0, E), 0x70E, 1806, 0, 0x8F1u
#define PPUTLIMPL_UHEX_0x70Du 1805u, (7, 0, D), 0x70D, 1805, 0, 0x8F2u
#define PPUTLIMPL_UHEX_0x70Cu 1804u, (7, 0, C), 0x70C, 1804, 0, 0x8F3u
#define PPUTLIMPL_UHEX_0x70Bu 1803u, (7, 0, B), 0x70B, 1803, 0, 0x8F4u
#define PPUTLIMPL_UHEX_0x70Au 1802u, (7, 0, A), 0x70A, 1802, 0, 0x8F5u
#define PPUTLIMPL_UHEX_0x709u 1801u, (7, 0, 9), 0x709, 1801, 0, 0x8F6u
#define PPUTLIMPL_UHEX_0x708u 1800u, (7, 0, 8), 0x708, 1800, 0, 0x8F7u
#define PPUTLIMPL_UHEX_0x707u 1799u, (7, 0, 7), 0x707, 1799, 0, 0x8F8u
#define PPUTLIMPL_UHEX_0x706u 1798u, (7, 0, 6), 0x706, 1798, 0, 0x8F9u
#define PPUTLIMPL_UHEX_0x705u 1797u, (7, 0, 5), 0x705, 1797, 0, 0x8FAu
#define PPUTLIMPL_UHEX_0x704u 1796u, (7, 0, 4), 0x704, 1796, 0, 0x8FBu
#define PPUTLIMPL_UHEX_0x703u 1795u, (7, 0, 3), 0x703, 1795, 0, 0x8FCu
#define PPUTLIMPL_UHEX_0x702u 1794u, (7, 0, 2), 0x702, 1794, 0, 0x8FDu
#define PPUTLIMPL_UHEX_0x701u 1793u, (7, 0, 1), 0x701, 1793, 0, 0x8FEu
#define PPUTLIMPL_UHEX_0x700u 1792u, (7, 0, 0), 0x700, 1792, 0, 0x8FFu
#define PPUTLIMPL_UHEX_0x6FFu 1791u, (6, F, F), 0x6FF, 1791, 0, 0x900u
#define PPUTLIMPL_UHEX_0x6FEu 1790u, (6, F, E), 0x6FE, 1790, 0, 0x901u
#define PPUTLIMPL_UHEX_0x6FDu 1789u, (6, F, D), 0x6FD, 1789, 0, 0x902u
#define PPUTLIMPL_UHEX_0x6FCu 1788u, (6, F, C), 0x6FC, 1788, 0, 0x903u
#define PPUTLIMPL_UHEX_0x6FBu 1787u, (6, F, B), 0x6FB, 1787, 0, 0x904u
#define PPUTLIMPL_UHEX_0x6FAu 1786u, (6, F, A), 0x6FA, 1786, 0, 0x905u
#define PPUTLIMPL_UHEX_0x6F9u 1785u, (6, F, 9), 0x6F9, 1785, 0, 0x906u
#define PPUTLIMPL_UHEX_0x6F8u 1784u, (6, F, 8), 0x6F8, 1784, 0, 0x907u
#define PPUTLIMPL_UHEX_0x6F7u 1783u, (6, F, 7), 0x6F7, 1783, 0, 0x908u
#define PPUTLIMPL_UHEX_0x6F6u 1782u, (6, F, 6), 0x6F6, 1782, 0, 0x909u
#define PPUTLIMPL_UHEX_0x6F5u 1781u, (6, F, 5), 0x6F5, 1781, 0, 0x90Au
#define PPUTLIMPL_UHEX_0x6F4u 1780u, (6, F, 4), 0x6F4, 1780, 0, 0x90Bu
#define PPUTLIMPL_UHEX_0x6F3u 1779u, (6, F, 3), 0x6F3, 1779, 0, 0x90Cu
#define PPUTLIMPL_UHEX_0x6F2u 1778u, (6, F, 2), 0x6F2, 1778, 0, 0x90Du
#define PPUTLIMPL_UHEX_0x6F1u 1777u, (6, F, 1), 0x6F1, 1777, 0, 0x90Eu
#define PPUTLIMPL_UHEX_0x6F0u 1776u, (6, F, 0), 0x6F0, 1776, 0, 0x90Fu
#define PPUTLIMPL_UHEX_0x6EFu 1775u, (6, E, F), 0x6EF, 1775, 0, 0x910u
#define PPUTLIMPL_UHEX_0x6EEu 1774u, (6, E, E), 0x6EE, 1774, 0, 0x911u
#define PPUTLIMPL_UHEX_0x6EDu 1773u, (6, E, D), 0x6ED, 1773, 0, 0x912u
#define PPUTLIMPL_UHEX_0x6ECu 1772u, (6, E, C), 0x6EC, 1772, 0, 0x913u
#define PPUTLIMPL_UHEX_0x6EBu 1771u, (6, E, B), 0x6EB, 1771, 0, 0x914u
#define PPUTLIMPL_UHEX_0x6EAu 1770u, (6, E, A), 0x6EA, 1770, 0, 0x915u
#define PPUTLIMPL_UHEX_0x6E9u 1769u, (6, E, 9), 0x6E9, 1769, 0, 0x916u
#define PPUTLIMPL_UHEX_0x6E8u 1768u, (6, E, 8), 0x6E8, 1768, 0, 0x917u
#define PPUTLIMPL_UHEX_0x6E7u 1767u, (6, E, 7), 0x6E7, 1767, 0, 0x918u
#define PPUTLIMPL_UHEX_0x6E6u 1766u, (6, E, 6), 0x6E6, 1766, 0, 0x919u
#define PPUTLIMPL_UHEX_0x6E5u 1765u, (6, E, 5), 0x6E5, 1765, 0, 0x91Au
#define PPUTLIMPL_UHEX_0x6E4u 1764u, (6, E, 4), 0x6E4, 1764, 0, 0x91Bu
#define PPUTLIMPL_UHEX_0x6E3u 1763u, (6, E, 3), 0x6E3, 1763, 0, 0x91Cu
#define PPUTLIMPL_UHEX_0x6E2u 1762u, (6, E, 2), 0x6E2, 1762, 0, 0x91Du
#define PPUTLIMPL_UHEX_0x6E1u 1761u, (6, E, 1), 0x6E1, 1761, 0, 0x91Eu
#define PPUTLIMPL_UHEX_0x6E0u 1760u, (6, E, 0), 0x6E0, 1760, 0, 0x91Fu
#define PPUTLIMPL_UHEX_0x6DFu 1759u, (6, D, F), 0x6DF, 1759, 0, 0x920u
#define PPUTLIMPL_UHEX_0x6DEu 1758u, (6, D, E), 0x6DE, 1758, 0, 0x921u
#define PPUTLIMPL_UHEX_0x6DDu 1757u, (6, D, D), 0x6DD, 1757, 0, 0x922u
#define PPUTLIMPL_UHEX_0x6DCu 1756u, (6, D, C), 0x6DC, 1756, 0, 0x923u
#define PPUTLIMPL_UHEX_0x6DBu 1755u, (6, D, B), 0x6DB, 1755, 0, 0x924u
#define PPUTLIMPL_UHEX_0x6DAu 1754u, (6, D, A), 0x6DA, 1754, 0, 0x925u
#define PPUTLIMPL_UHEX_0x6D9u 1753u, (6, D, 9), 0x6D9, 1753, 0, 0x926u
#define PPUTLIMPL_UHEX_0x6D8u 1752u, (6, D, 8), 0x6D8, 1752, 0, 0x927u
#define PPUTLIMPL_UHEX_0x6D7u 1751u, (6, D, 7), 0x6D7, 1751, 0, 0x928u
#define PPUTLIMPL_UHEX_0x6D6u 1750u, (6, D, 6), 0x6D6, 1750, 0, 0x929u
#define PPUTLIMPL_UHEX_0x6D5u 1749u, (6, D, 5), 0x6D5, 1749, 0, 0x92Au
#define PPUTLIMPL_UHEX_0x6D4u 1748u, (6, D, 4), 0x6D4, 1748, 0, 0x92Bu
#define PPUTLIMPL_UHEX_0x6D3u 1747u, (6, D, 3), 0x6D3, 1747, 0, 0x92Cu
#define PPUTLIMPL_UHEX_0x6D2u 1746u, (6, D, 2), 0x6D2, 1746, 0, 0x92Du
#define PPUTLIMPL_UHEX_0x6D1u 1745u, (6, D, 1), 0x6D1, 1745, 0, 0x92Eu
#define PPUTLIMPL_UHEX_0x6D0u 1744u, (6, D, 0), 0x6D0, 1744, 0, 0x92Fu
#define PPUTLIMPL_UHEX_0x6CFu 1743u, (6, C, F), 0x6CF, 1743, 0, 0x930u
#define PPUTLIMPL_UHEX_0x6CEu 1742u, (6, C, E), 0x6CE, 1742, 0, 0x931u
#define PPUTLIMPL_UHEX_0x6CDu 1741u, (6, C, D), 0x6CD, 1741, 0, 0x932u
#define PPUTLIMPL_UHEX_0x6CCu 1740u, (6, C, C), 0x6CC, 1740, 0, 0x933u
#define PPUTLIMPL_UHEX_0x6CBu 1739u, (6, C, B), 0x6CB, 1739, 0, 0x934u
#define PPUTLIMPL_UHEX_0x6CAu 1738u, (6, C, A), 0x6CA, 1738, 0, 0x935u
#define PPUTLIMPL_UHEX_0x6C9u 1737u, (6, C, 9), 0x6C9, 1737, 0, 0x936u
#define PPUTLIMPL_UHEX_0x6C8u 1736u, (6, C, 8), 0x6C8, 1736, 0, 0x937u
#define PPUTLIMPL_UHEX_0x6C7u 1735u, (6, C, 7), 0x6C7, 1735, 0, 0x938u
#define PPUTLIMPL_UHEX_0x6C6u 1734u, (6, C, 6), 0x6C6, 1734, 0, 0x939u
#define PPUTLIMPL_UHEX_0x6C5u 1733u, (6, C, 5), 0x6C5, 1733, 0, 0x93Au
#define PPUTLIMPL_UHEX_0x6C4u 1732u, (6, C, 4), 0x6C4, 1732, 0, 0x93Bu
#define PPUTLIMPL_UHEX_0x6C3u 1731u, (6, C, 3), 0x6C3, 1731, 0, 0x93Cu
#define PPUTLIMPL_UHEX_0x6C2u 1730u, (6, C, 2), 0x6C2, 1730, 0, 0x93Du
#define PPUTLIMPL_UHEX_0x6C1u 1729u, (6, C, 1), 0x6C1, 1729, 0, 0x93Eu
#define PPUTLIMPL_UHEX_0x6C0u 1728u, (6, C, 0), 0x6C0, 1728, 0, 0x93Fu
#define PPUTLIMPL_UHEX_0x6BFu 1727u, (6, B, F), 0x6BF, 1727, 0, 0x940u
#define PPUTLIMPL_UHEX_0x6BEu 1726u, (6, B, E), 0x6BE, 1726, 0, 0x941u
#define PPUTLIMPL_UHEX_0x6BDu 1725u, (6, B, D), 0x6BD, 1725, 0, 0x942u
#define PPUTLIMPL_UHEX_0x6BCu 1724u, (6, B, C), 0x6BC, 1724, 0, 0x943u
#define PPUTLIMPL_UHEX_0x6BBu 1723u, (6, B, B), 0x6BB, 1723, 0, 0x944u
#define PPUTLIMPL_UHEX_0x6BAu 1722u, (6, B, A), 0x6BA, 1722, 0, 0x945u
#define PPUTLIMPL_UHEX_0x6B9u 1721u, (6, B, 9), 0x6B9, 1721, 0, 0x946u
#define PPUTLIMPL_UHEX_0x6B8u 1720u, (6, B, 8), 0x6B8, 1720, 0, 0x947u
#define PPUTLIMPL_UHEX_0x6B7u 1719u, (6, B, 7), 0x6B7, 1719, 0, 0x948u
#define PPUTLIMPL_UHEX_0x6B6u 1718u, (6, B, 6), 0x6B6, 1718, 0, 0x949u
#define PPUTLIMPL_UHEX_0x6B5u 1717u, (6, B, 5), 0x6B5, 1717, 0, 0x94Au
#define PPUTLIMPL_UHEX_0x6B4u 1716u, (6, B, 4), 0x6B4, 1716, 0, 0x94Bu
#define PPUTLIMPL_UHEX_0x6B3u 1715u, (6, B, 3), 0x6B3, 1715, 0, 0x94Cu
#define PPUTLIMPL_UHEX_0x6B2u 1714u, (6, B, 2), 0x6B2, 1714, 0, 0x94Du
#define PPUTLIMPL_UHEX_0x6B1u 1713u, (6, B, 1), 0x6B1, 1713, 0, 0x94Eu
#define PPUTLIMPL_UHEX_0x6B0u 1712u, (6, B, 0), 0x6B0, 1712, 0, 0x94Fu
#define PPUTLIMPL_UHEX_0x6AFu 1711u, (6, A, F), 0x6AF, 1711, 0, 0x950u
#define PPUTLIMPL_UHEX_0x6AEu 1710u, (6, A, E), 0x6AE, 1710, 0, 0x951u
#define PPUTLIMPL_UHEX_0x6ADu 1709u, (6, A, D), 0x6AD, 1709, 0, 0x952u
#define PPUTLIMPL_UHEX_0x6ACu 1708u, (6, A, C), 0x6AC, 1708, 0, 0x953u
#define PPUTLIMPL_UHEX_0x6ABu 1707u, (6, A, B), 0x6AB, 1707, 0, 0x954u
#define PPUTLIMPL_UHEX_0x6AAu 1706u, (6, A, A), 0x6AA, 1706, 0, 0x955u
#define PPUTLIMPL_UHEX_0x6A9u 1705u, (6, A, 9), 0x6A9, 1705, 0, 0x956u
#define PPUTLIMPL_UHEX_0x6A8u 1704u, (6, A, 8), 0x6A8, 1704, 0, 0x957u
#define PPUTLIMPL_UHEX_0x6A7u 1703u, (6, A, 7), 0x6A7, 1703, 0, 0x958u
#define PPUTLIMPL_UHEX_0x6A6u 1702u, (6, A, 6), 0x6A6, 1702, 0, 0x959u
#define PPUTLIMPL_UHEX_0x6A5u 1701u, (6, A, 5), 0x6A5, 1701, 0, 0x95Au
#define PPUTLIMPL_UHEX_0x6A4u 1700u, (6, A, 4), 0x6A4, 1700, 0, 0x95Bu
#define PPUTLIMPL_UHEX_0x6A3u 1699u, (6, A, 3), 0x6A3, 1699, 0, 0x95Cu
#define PPUTLIMPL_UHEX_0x6A2u 1698u, (6, A, 2), 0x6A2, 1698, 0, 0x95Du
#define PPUTLIMPL_UHEX_0x6A1u 1697u, (6, A, 1), 0x6A1, 1697, 0, 0x95Eu
#define PPUTLIMPL_UHEX_0x6A0u 1696u, (6, A, 0), 0x6A0, 1696, 0, 0x95Fu
#define PPUTLIMPL_UHEX_0x69Fu 1695u, (6, 9, F), 0x69F, 1695, 0, 0x960u
#define PPUTLIMPL_UHEX_0x69Eu 1694u, (6, 9, E), 0x69E, 1694, 0, 0x961u
#define PPUTLIMPL_UHEX_0x69Du 1693u, (6, 9, D), 0x69D, 1693, 0, 0x962u
#define PPUTLIMPL_UHEX_0x69Cu 1692u, (6, 9, C), 0x69C, 1692, 0, 0x963u
#define PPUTLIMPL_UHEX_0x69Bu 1691u, (6, 9, B), 0x69B, 1691, 0, 0x964u
#define PPUTLIMPL_UHEX_0x69Au 1690u, (6, 9, A), 0x69A, 1690, 0, 0x965u
#define PPUTLIMPL_UHEX_0x699u 1689u, (6, 9, 9), 0x699, 1689, 0, 0x966u
#define PPUTLIMPL_UHEX_0x698u 1688u, (6, 9, 8), 0x698, 1688, 0, 0x967u
#define PPUTLIMPL_UHEX_0x697u 1687u, (6, 9, 7), 0x697, 1687, 0, 0x968u
#define PPUTLIMPL_UHEX_0x696u 1686u, (6, 9, 6), 0x696, 1686, 0, 0x969u
#define PPUTLIMPL_UHEX_0x695u 1685u, (6, 9, 5), 0x695, 1685, 0, 0x96Au
#define PPUTLIMPL_UHEX_0x694u 1684u, (6, 9, 4), 0x694, 1684, 0, 0x96Bu
#define PPUTLIMPL_UHEX_0x693u 1683u, (6, 9, 3), 0x693, 1683, 0, 0x96Cu
#define PPUTLIMPL_UHEX_0x692u 1682u, (6, 9, 2), 0x692, 1682, 0, 0x96Du
#define PPUTLIMPL_UHEX_0x691u 1681u, (6, 9, 1), 0x691, 1681, 0, 0x96Eu
#define PPUTLIMPL_UHEX_0x690u 1680u, (6, 9, 0), 0x690, 1680, 0, 0x96Fu
#define PPUTLIMPL_UHEX_0x68Fu 1679u, (6, 8, F), 0x68F, 1679, 0, 0x970u
#define PPUTLIMPL_UHEX_0x68Eu 1678u, (6, 8, E), 0x68E, 1678, 0, 0x971u
#define PPUTLIMPL_UHEX_0x68Du 1677u, (6, 8, D), 0x68D, 1677, 0, 0x972u
#define PPUTLIMPL_UHEX_0x68Cu 1676u, (6, 8, C), 0x68C, 1676, 0, 0x973u
#define PPUTLIMPL_UHEX_0x68Bu 1675u, (6, 8, B), 0x68B, 1675, 0, 0x974u
#define PPUTLIMPL_UHEX_0x68Au 1674u, (6, 8, A), 0x68A, 1674, 0, 0x975u
#define PPUTLIMPL_UHEX_0x689u 1673u, (6, 8, 9), 0x689, 1673, 0, 0x976u
#define PPUTLIMPL_UHEX_0x688u 1672u, (6, 8, 8), 0x688, 1672, 0, 0x977u
#define PPUTLIMPL_UHEX_0x687u 1671u, (6, 8, 7), 0x687, 1671, 0, 0x978u
#define PPUTLIMPL_UHEX_0x686u 1670u, (6, 8, 6), 0x686, 1670, 0, 0x979u
#define PPUTLIMPL_UHEX_0x685u 1669u, (6, 8, 5), 0x685, 1669, 0, 0x97Au
#define PPUTLIMPL_UHEX_0x684u 1668u, (6, 8, 4), 0x684, 1668, 0, 0x97Bu
#define PPUTLIMPL_UHEX_0x683u 1667u, (6, 8, 3), 0x683, 1667, 0, 0x97Cu
#define PPUTLIMPL_UHEX_0x682u 1666u, (6, 8, 2), 0x682, 1666, 0, 0x97Du
#define PPUTLIMPL_UHEX_0x681u 1665u, (6, 8, 1), 0x681, 1665, 0, 0x97Eu
#define PPUTLIMPL_UHEX_0x680u 1664u, (6, 8, 0), 0x680, 1664, 0, 0x97Fu
#define PPUTLIMPL_UHEX_0x67Fu 1663u, (6, 7, F), 0x67F, 1663, 0, 0x980u
#define PPUTLIMPL_UHEX_0x67Eu 1662u, (6, 7, E), 0x67E, 1662, 0, 0x981u
#define PPUTLIMPL_UHEX_0x67Du 1661u, (6, 7, D), 0x67D, 1661, 0, 0x982u
#define PPUTLIMPL_UHEX_0x67Cu 1660u, (6, 7, C), 0x67C, 1660, 0, 0x983u
#define PPUTLIMPL_UHEX_0x67Bu 1659u, (6, 7, B), 0x67B, 1659, 0, 0x984u
#define PPUTLIMPL_UHEX_0x67Au 1658u, (6, 7, A), 0x67A, 1658, 0, 0x985u
#define PPUTLIMPL_UHEX_0x679u 1657u, (6, 7, 9), 0x679, 1657, 0, 0x986u
#define PPUTLIMPL_UHEX_0x678u 1656u, (6, 7, 8), 0x678, 1656, 0, 0x987u
#define PPUTLIMPL_UHEX_0x677u 1655u, (6, 7, 7), 0x677, 1655, 0, 0x988u
#define PPUTLIMPL_UHEX_0x676u 1654u, (6, 7, 6), 0x676, 1654, 0, 0x989u
#define PPUTLIMPL_UHEX_0x675u 1653u, (6, 7, 5), 0x675, 1653, 0, 0x98Au
#define PPUTLIMPL_UHEX_0x674u 1652u, (6, 7, 4), 0x674, 1652, 0, 0x98Bu
#define PPUTLIMPL_UHEX_0x673u 1651u, (6, 7, 3), 0x673, 1651, 0, 0x98Cu
#define PPUTLIMPL_UHEX_0x672u 1650u, (6, 7, 2), 0x672, 1650, 0, 0x98Du
#define PPUTLIMPL_UHEX_0x671u 1649u, (6, 7, 1), 0x671, 1649, 0, 0x98Eu
#define PPUTLIMPL_UHEX_0x670u 1648u, (6, 7, 0), 0x670, 1648, 0, 0x98Fu
#define PPUTLIMPL_UHEX_0x66Fu 1647u, (6, 6, F), 0x66F, 1647, 0, 0x990u
#define PPUTLIMPL_UHEX_0x66Eu 1646u, (6, 6, E), 0x66E, 1646, 0, 0x991u
#define PPUTLIMPL_UHEX_0x66Du 1645u, (6, 6, D), 0x66D, 1645, 0, 0x992u
#define PPUTLIMPL_UHEX_0x66Cu 1644u, (6, 6, C), 0x66C, 1644, 0, 0x993u
#define PPUTLIMPL_UHEX_0x66Bu 1643u, (6, 6, B), 0x66B, 1643, 0, 0x994u
#define PPUTLIMPL_UHEX_0x66Au 1642u, (6, 6, A), 0x66A, 1642, 0, 0x995u
#define PPUTLIMPL_UHEX_0x669u 1641u, (6, 6, 9), 0x669, 1641, 0, 0x996u
#define PPUTLIMPL_UHEX_0x668u 1640u, (6, 6, 8), 0x668, 1640, 0, 0x997u
#define PPUTLIMPL_UHEX_0x667u 1639u, (6, 6, 7), 0x667, 1639, 0, 0x998u
#define PPUTLIMPL_UHEX_0x666u 1638u, (6, 6, 6), 0x666, 1638, 0, 0x999u
#define PPUTLIMPL_UHEX_0x665u 1637u, (6, 6, 5), 0x665, 1637, 0, 0x99Au
#define PPUTLIMPL_UHEX_0x664u 1636u, (6, 6, 4), 0x664, 1636, 0, 0x99Bu
#define PPUTLIMPL_UHEX_0x663u 1635u, (6, 6, 3), 0x663, 1635, 0, 0x99Cu
#define PPUTLIMPL_UHEX_0x662u 1634u, (6, 6, 2), 0x662, 1634, 0, 0x99Du
#define PPUTLIMPL_UHEX_0x661u 1633u, (6, 6, 1), 0x661, 1633, 0, 0x99Eu
#define PPUTLIMPL_UHEX_0x660u 1632u, (6, 6, 0), 0x660, 1632, 0, 0x99Fu
#define PPUTLIMPL_UHEX_0x65Fu 1631u, (6, 5, F), 0x65F, 1631, 0, 0x9A0u
#define PPUTLIMPL_UHEX_0x65Eu 1630u, (6, 5, E), 0x65E, 1630, 0, 0x9A1u
#define PPUTLIMPL_UHEX_0x65Du 1629u, (6, 5, D), 0x65D, 1629, 0, 0x9A2u
#define PPUTLIMPL_UHEX_0x65Cu 1628u, (6, 5, C), 0x65C, 1628, 0, 0x9A3u
#define PPUTLIMPL_UHEX_0x65Bu 1627u, (6, 5, B), 0x65B, 1627, 0, 0x9A4u
#define PPUTLIMPL_UHEX_0x65Au 1626u, (6, 5, A), 0x65A, 1626, 0, 0x9A5u
#define PPUTLIMPL_UHEX_0x659u 1625u, (6, 5, 9), 0x659, 1625, 0, 0x9A6u
#define PPUTLIMPL_UHEX_0x658u 1624u, (6, 5, 8), 0x658, 1624, 0, 0x9A7u
#define PPUTLIMPL_UHEX_0x657u 1623u, (6, 5, 7), 0x657, 1623, 0, 0x9A8u
#define PPUTLIMPL_UHEX_0x656u 1622u, (6, 5, 6), 0x656, 1622, 0, 0x9A9u
#define PPUTLIMPL_UHEX_0x655u 1621u, (6, 5, 5), 0x655, 1621, 0, 0x9AAu
#define PPUTLIMPL_UHEX_0x654u 1620u, (6, 5, 4), 0x654, 1620, 0, 0x9ABu
#define PPUTLIMPL_UHEX_0x653u 1619u, (6, 5, 3), 0x653, 1619, 0, 0x9ACu
#define PPUTLIMPL_UHEX_0x652u 1618u, (6, 5, 2), 0x652, 1618, 0, 0x9ADu
#define PPUTLIMPL_UHEX_0x651u 1617u, (6, 5, 1), 0x651, 1617, 0, 0x9AEu
#define PPUTLIMPL_UHEX_0x650u 1616u, (6, 5, 0), 0x650, 1616, 0, 0x9AFu
#define PPUTLIMPL_UHEX_0x64Fu 1615u, (6, 4, F), 0x64F, 1615, 0, 0x9B0u
#define PPUTLIMPL_UHEX_0x64Eu 1614u, (6, 4, E), 0x64E, 1614, 0, 0x9B1u
#define PPUTLIMPL_UHEX_0x64Du 1613u, (6, 4, D), 0x64D, 1613, 0, 0x9B2u
#define PPUTLIMPL_UHEX_0x64Cu 1612u, (6, 4, C), 0x64C, 1612, 0, 0x9B3u
#define PPUTLIMPL_UHEX_0x64Bu 1611u, (6, 4, B), 0x64B, 1611, 0, 0x9B4u
#define PPUTLIMPL_UHEX_0x64Au 1610u, (6, 4, A), 0x64A, 1610, 0, 0x9B5u
#define PPUTLIMPL_UHEX_0x649u 1609u, (6, 4, 9), 0x649, 1609, 0, 0x9B6u
#define PPUTLIMPL_UHEX_0x648u 1608u, (6, 4, 8), 0x648, 1608, 0, 0x9B7u
#define PPUTLIMPL_UHEX_0x647u 1607u, (6, 4, 7), 0x647, 1607, 0, 0x9B8u
#define PPUTLIMPL_UHEX_0x646u 1606u, (6, 4, 6), 0x646, 1606, 0, 0x9B9u
#define PPUTLIMPL_UHEX_0x645u 1605u, (6, 4, 5), 0x645, 1605, 0, 0x9BAu
#define PPUTLIMPL_UHEX_0x644u 1604u, (6, 4, 4), 0x644, 1604, 0, 0x9BBu
#define PPUTLIMPL_UHEX_0x643u 1603u, (6, 4, 3), 0x643, 1603, 0, 0x9BCu
#define PPUTLIMPL_UHEX_0x642u 1602u, (6, 4, 2), 0x642, 1602, 0, 0x9BDu
#define PPUTLIMPL_UHEX_0x641u 1601u, (6, 4, 1), 0x641, 1601, 0, 0x9BEu
#define PPUTLIMPL_UHEX_0x640u 1600u, (6, 4, 0), 0x640, 1600, 0, 0x9BFu
#define PPUTLIMPL_UHEX_0x63Fu 1599u, (6, 3, F), 0x63F, 1599, 0, 0x9C0u
#define PPUTLIMPL_UHEX_0x63Eu 1598u, (6, 3, E), 0x63E, 1598, 0, 0x9C1u
#define PPUTLIMPL_UHEX_0x63Du 1597u, (6, 3, D), 0x63D, 1597, 0, 0x9C2u
#define PPUTLIMPL_UHEX_0x63Cu 1596u, (6, 3, C), 0x63C, 1596, 0, 0x9C3u
#define PPUTLIMPL_UHEX_0x63Bu 1595u, (6, 3, B), 0x63B, 1595, 0, 0x9C4u
#define PPUTLIMPL_UHEX_0x63Au 1594u, (6, 3, A), 0x63A, 1594, 0, 0x9C5u
#define PPUTLIMPL_UHEX_0x639u 1593u, (6, 3, 9), 0x639, 1593, 0, 0x9C6u
#define PPUTLIMPL_UHEX_0x638u 1592u, (6, 3, 8), 0x638, 1592, 0, 0x9C7u
#define PPUTLIMPL_UHEX_0x637u 1591u, (6, 3, 7), 0x637, 1591, 0, 0x9C8u
#define PPUTLIMPL_UHEX_0x636u 1590u, (6, 3, 6), 0x636, 1590, 0, 0x9C9u
#define PPUTLIMPL_UHEX_0x635u 1589u, (6, 3, 5), 0x635, 1589, 0, 0x9CAu
#define PPUTLIMPL_UHEX_0x634u 1588u, (6, 3, 4), 0x634, 1588, 0, 0x9CBu
#define PPUTLIMPL_UHEX_0x633u 1587u, (6, 3, 3), 0x633, 1587, 0, 0x9CCu
#define PPUTLIMPL_UHEX_0x632u 1586u, (6, 3, 2), 0x632, 1586, 0, 0x9CDu
#define PPUTLIMPL_UHEX_0x631u 1585u, (6, 3, 1), 0x631, 1585, 0, 0x9CEu
#define PPUTLIMPL_UHEX_0x630u 1584u, (6, 3, 0), 0x630, 1584, 0, 0x9CFu
#define PPUTLIMPL_UHEX_0x62Fu 1583u, (6, 2, F), 0x62F, 1583, 0, 0x9D0u
#define PPUTLIMPL_UHEX_0x62Eu 1582u, (6, 2, E), 0x62E, 1582, 0, 0x9D1u
#define PPUTLIMPL_UHEX_0x62Du 1581u, (6, 2, D), 0x62D, 1581, 0, 0x9D2u
#define PPUTLIMPL_UHEX_0x62Cu 1580u, (6, 2, C), 0x62C, 1580, 0, 0x9D3u
#define PPUTLIMPL_UHEX_0x62Bu 1579u, (6, 2, B), 0x62B, 1579, 0, 0x9D4u
#define PPUTLIMPL_UHEX_0x62Au 1578u, (6, 2, A), 0x62A, 1578, 0, 0x9D5u
#define PPUTLIMPL_UHEX_0x629u 1577u, (6, 2, 9), 0x629, 1577, 0, 0x9D6u
#define PPUTLIMPL_UHEX_0x628u 1576u, (6, 2, 8), 0x628, 1576, 0, 0x9D7u
#define PPUTLIMPL_UHEX_0x627u 1575u, (6, 2, 7), 0x627, 1575, 0, 0x9D8u
#define PPUTLIMPL_UHEX_0x626u 1574u, (6, 2, 6), 0x626, 1574, 0, 0x9D9u
#define PPUTLIMPL_UHEX_0x625u 1573u, (6, 2, 5), 0x625, 1573, 0, 0x9DAu
#define PPUTLIMPL_UHEX_0x624u 1572u, (6, 2, 4), 0x624, 1572, 0, 0x9DBu
#define PPUTLIMPL_UHEX_0x623u 1571u, (6, 2, 3), 0x623, 1571, 0, 0x9DCu
#define PPUTLIMPL_UHEX_0x622u 1570u, (6, 2, 2), 0x622, 1570, 0, 0x9DDu
#define PPUTLIMPL_UHEX_0x621u 1569u, (6, 2, 1), 0x621, 1569, 0, 0x9DEu
#define PPUTLIMPL_UHEX_0x620u 1568u, (6, 2, 0), 0x620, 1568, 0, 0x9DFu
#define PPUTLIMPL_UHEX_0x61Fu 1567u, (6, 1, F), 0x61F, 1567, 0, 0x9E0u
#define PPUTLIMPL_UHEX_0x61Eu 1566u, (6, 1, E), 0x61E, 1566, 0, 0x9E1u
#define PPUTLIMPL_UHEX_0x61Du 1565u, (6, 1, D), 0x61D, 1565, 0, 0x9E2u
#define PPUTLIMPL_UHEX_0x61Cu 1564u, (6, 1, C), 0x61C, 1564, 0, 0x9E3u
#define PPUTLIMPL_UHEX_0x61Bu 1563u, (6, 1, B), 0x61B, 1563, 0, 0x9E4u
#define PPUTLIMPL_UHEX_0x61Au 1562u, (6, 1, A), 0x61A, 1562, 0, 0x9E5u
#define PPUTLIMPL_UHEX_0x619u 1561u, (6, 1, 9), 0x619, 1561, 0, 0x9E6u
#define PPUTLIMPL_UHEX_0x618u 1560u, (6, 1, 8), 0x618, 1560, 0, 0x9E7u
#define PPUTLIMPL_UHEX_0x617u 1559u, (6, 1, 7), 0x617, 1559, 0, 0x9E8u
#define PPUTLIMPL_UHEX_0x616u 1558u, (6, 1, 6), 0x616, 1558, 0, 0x9E9u
#define PPUTLIMPL_UHEX_0x615u 1557u, (6, 1, 5), 0x615, 1557, 0, 0x9EAu
#define PPUTLIMPL_UHEX_0x614u 1556u, (6, 1, 4), 0x614, 1556, 0, 0x9EBu
#define PPUTLIMPL_UHEX_0x613u 1555u, (6, 1, 3), 0x613, 1555, 0, 0x9ECu
#define PPUTLIMPL_UHEX_0x612u 1554u, (6, 1, 2), 0x612, 1554, 0, 0x9EDu
#define PPUTLIMPL_UHEX_0x611u 1553u, (6, 1, 1), 0x611, 1553, 0, 0x9EEu
#define PPUTLIMPL_UHEX_0x610u 1552u, (6, 1, 0), 0x610, 1552, 0, 0x9EFu
#define PPUTLIMPL_UHEX_0x60Fu 1551u, (6, 0, F), 0x60F, 1551, 0, 0x9F0u
#define PPUTLIMPL_UHEX_0x60Eu 1550u, (6, 0, E), 0x60E, 1550, 0, 0x9F1u
#define PPUTLIMPL_UHEX_0x60Du 1549u, (6, 0, D), 0x60D, 1549, 0, 0x9F2u
#define PPUTLIMPL_UHEX_0x60Cu 1548u, (6, 0, C), 0x60C, 1548, 0, 0x9F3u
#define PPUTLIMPL_UHEX_0x60Bu 1547u, (6, 0, B), 0x60B, 1547, 0, 0x9F4u
#define PPUTLIMPL_UHEX_0x60Au 1546u, (6, 0, A), 0x60A, 1546, 0, 0x9F5u
#define PPUTLIMPL_UHEX_0x609u 1545u, (6, 0, 9), 0x609, 1545, 0, 0x9F6u
#define PPUTLIMPL_UHEX_0x608u 1544u, (6, 0, 8), 0x608, 1544, 0, 0x9F7u
#define PPUTLIMPL_UHEX_0x607u 1543u, (6, 0, 7), 0x607, 1543, 0, 0x9F8u
#define PPUTLIMPL_UHEX_0x606u 1542u, (6, 0, 6), 0x606, 1542, 0, 0x9F9u
#define PPUTLIMPL_UHEX_0x605u 1541u, (6, 0, 5), 0x605, 1541, 0, 0x9FAu
#define PPUTLIMPL_UHEX_0x604u 1540u, (6, 0, 4), 0x604, 1540, 0, 0x9FBu
#define PPUTLIMPL_UHEX_0x603u 1539u, (6, 0, 3), 0x603, 1539, 0, 0x9FCu
#define PPUTLIMPL_UHEX_0x602u 1538u, (6, 0, 2), 0x602, 1538, 0, 0x9FDu
#define PPUTLIMPL_UHEX_0x601u 1537u, (6, 0, 1), 0x601, 1537, 0, 0x9FEu
#define PPUTLIMPL_UHEX_0x600u 1536u, (6, 0, 0), 0x600, 1536, 0, 0x9FFu
#define PPUTLIMPL_UHEX_0x5FFu 1535u, (5, F, F), 0x5FF, 1535, 0, 0xA00u
#define PPUTLIMPL_UHEX_0x5FEu 1534u, (5, F, E), 0x5FE, 1534, 0, 0xA01u
#define PPUTLIMPL_UHEX_0x5FDu 1533u, (5, F, D), 0x5FD, 1533, 0, 0xA02u
#define PPUTLIMPL_UHEX_0x5FCu 1532u, (5, F, C), 0x5FC, 1532, 0, 0xA03u
#define PPUTLIMPL_UHEX_0x5FBu 1531u, (5, F, B), 0x5FB, 1531, 0, 0xA04u
#define PPUTLIMPL_UHEX_0x5FAu 1530u, (5, F, A), 0x5FA, 1530, 0, 0xA05u
#define PPUTLIMPL_UHEX_0x5F9u 1529u, (5, F, 9), 0x5F9, 1529, 0, 0xA06u
#define PPUTLIMPL_UHEX_0x5F8u 1528u, (5, F, 8), 0x5F8, 1528, 0, 0xA07u
#define PPUTLIMPL_UHEX_0x5F7u 1527u, (5, F, 7), 0x5F7, 1527, 0, 0xA08u
#define PPUTLIMPL_UHEX_0x5F6u 1526u, (5, F, 6), 0x5F6, 1526, 0, 0xA09u
#define PPUTLIMPL_UHEX_0x5F5u 1525u, (5, F, 5), 0x5F5, 1525, 0, 0xA0Au
#define PPUTLIMPL_UHEX_0x5F4u 1524u, (5, F, 4), 0x5F4, 1524, 0, 0xA0Bu
#define PPUTLIMPL_UHEX_0x5F3u 1523u, (5, F, 3), 0x5F3, 1523, 0, 0xA0Cu
#define PPUTLIMPL_UHEX_0x5F2u 1522u, (5, F, 2), 0x5F2, 1522, 0, 0xA0Du
#define PPUTLIMPL_UHEX_0x5F1u 1521u, (5, F, 1), 0x5F1, 1521, 0, 0xA0Eu
#define PPUTLIMPL_UHEX_0x5F0u 1520u, (5, F, 0), 0x5F0, 1520, 0, 0xA0Fu
#define PPUTLIMPL_UHEX_0x5EFu 1519u, (5, E, F), 0x5EF, 1519, 0, 0xA10u
#define PPUTLIMPL_UHEX_0x5EEu 1518u, (5, E, E), 0x5EE, 1518, 0, 0xA11u
#define PPUTLIMPL_UHEX_0x5EDu 1517u, (5, E, D), 0x5ED, 1517, 0, 0xA12u
#define PPUTLIMPL_UHEX_0x5ECu 1516u, (5, E, C), 0x5EC, 1516, 0, 0xA13u
#define PPUTLIMPL_UHEX_0x5EBu 1515u, (5, E, B), 0x5EB, 1515, 0, 0xA14u
#define PPUTLIMPL_UHEX_0x5EAu 1514u, (5, E, A), 0x5EA, 1514, 0, 0xA15u
#define PPUTLIMPL_UHEX_0x5E9u 1513u, (5, E, 9), 0x5E9, 1513, 0, 0xA16u
#define PPUTLIMPL_UHEX_0x5E8u 1512u, (5, E, 8), 0x5E8, 1512, 0, 0xA17u
#define PPUTLIMPL_UHEX_0x5E7u 1511u, (5, E, 7), 0x5E7, 1511, 0, 0xA18u
#define PPUTLIMPL_UHEX_0x5E6u 1510u, (5, E, 6), 0x5E6, 1510, 0, 0xA19u
#define PPUTLIMPL_UHEX_0x5E5u 1509u, (5, E, 5), 0x5E5, 1509, 0, 0xA1Au
#define PPUTLIMPL_UHEX_0x5E4u 1508u, (5, E, 4), 0x5E4, 1508, 0, 0xA1Bu
#define PPUTLIMPL_UHEX_0x5E3u 1507u, (5, E, 3), 0x5E3, 1507, 0, 0xA1Cu
#define PPUTLIMPL_UHEX_0x5E2u 1506u, (5, E, 2), 0x5E2, 1506, 0, 0xA1Du
#define PPUTLIMPL_UHEX_0x5E1u 1505u, (5, E, 1), 0x5E1, 1505, 0, 0xA1Eu
#define PPUTLIMPL_UHEX_0x5E0u 1504u, (5, E, 0), 0x5E0, 1504, 0, 0xA1Fu
#define PPUTLIMPL_UHEX_0x5DFu 1503u, (5, D, F), 0x5DF, 1503, 0, 0xA20u
#define PPUTLIMPL_UHEX_0x5DEu 1502u, (5, D, E), 0x5DE, 1502, 0, 0xA21u
#define PPUTLIMPL_UHEX_0x5DDu 1501u, (5, D, D), 0x5DD, 1501, 0, 0xA22u
#define PPUTLIMPL_UHEX_0x5DCu 1500u, (5, D, C), 0x5DC, 1500, 0, 0xA23u
#define PPUTLIMPL_UHEX_0x5DBu 1499u, (5, D, B), 0x5DB, 1499, 0, 0xA24u
#define PPUTLIMPL_UHEX_0x5DAu 1498u, (5, D, A), 0x5DA, 1498, 0, 0xA25u
#define PPUTLIMPL_UHEX_0x5D9u 1497u, (5, D, 9), 0x5D9, 1497, 0, 0xA26u
#define PPUTLIMPL_UHEX_0x5D8u 1496u, (5, D, 8), 0x5D8, 1496, 0, 0xA27u
#define PPUTLIMPL_UHEX_0x5D7u 1495u, (5, D, 7), 0x5D7, 1495, 0, 0xA28u
#define PPUTLIMPL_UHEX_0x5D6u 1494u, (5, D, 6), 0x5D6, 1494, 0, 0xA29u
#define PPUTLIMPL_UHEX_0x5D5u 1493u, (5, D, 5), 0x5D5, 1493, 0, 0xA2Au
#define PPUTLIMPL_UHEX_0x5D4u 1492u, (5, D, 4), 0x5D4, 1492, 0, 0xA2Bu
#define PPUTLIMPL_UHEX_0x5D3u 1491u, (5, D, 3), 0x5D3, 1491, 0, 0xA2Cu
#define PPUTLIMPL_UHEX_0x5D2u 1490u, (5, D, 2), 0x5D2, 1490, 0, 0xA2Du
#define PPUTLIMPL_UHEX_0x5D1u 1489u, (5, D, 1), 0x5D1, 1489, 0, 0xA2Eu
#define PPUTLIMPL_UHEX_0x5D0u 1488u, (5, D, 0), 0x5D0, 1488, 0, 0xA2Fu
#define PPUTLIMPL_UHEX_0x5CFu 1487u, (5, C, F), 0x5CF, 1487, 0, 0xA30u
#define PPUTLIMPL_UHEX_0x5CEu 1486u, (5, C, E), 0x5CE, 1486, 0, 0xA31u
#define PPUTLIMPL_UHEX_0x5CDu 1485u, (5, C, D), 0x5CD, 1485, 0, 0xA32u
#define PPUTLIMPL_UHEX_0x5CCu 1484u, (5, C, C), 0x5CC, 1484, 0, 0xA33u
#define PPUTLIMPL_UHEX_0x5CBu 1483u, (5, C, B), 0x5CB, 1483, 0, 0xA34u
#define PPUTLIMPL_UHEX_0x5CAu 1482u, (5, C, A), 0x5CA, 1482, 0, 0xA35u
#define PPUTLIMPL_UHEX_0x5C9u 1481u, (5, C, 9), 0x5C9, 1481, 0, 0xA36u
#define PPUTLIMPL_UHEX_0x5C8u 1480u, (5, C, 8), 0x5C8, 1480, 0, 0xA37u
#define PPUTLIMPL_UHEX_0x5C7u 1479u, (5, C, 7), 0x5C7, 1479, 0, 0xA38u
#define PPUTLIMPL_UHEX_0x5C6u 1478u, (5, C, 6), 0x5C6, 1478, 0, 0xA39u
#define PPUTLIMPL_UHEX_0x5C5u 1477u, (5, C, 5), 0x5C5, 1477, 0, 0xA3Au
#define PPUTLIMPL_UHEX_0x5C4u 1476u, (5, C, 4), 0x5C4, 1476, 0, 0xA3Bu
#define PPUTLIMPL_UHEX_0x5C3u 1475u, (5, C, 3), 0x5C3, 1475, 0, 0xA3Cu
#define PPUTLIMPL_UHEX_0x5C2u 1474u, (5, C, 2), 0x5C2, 1474, 0, 0xA3Du
#define PPUTLIMPL_UHEX_0x5C1u 1473u, (5, C, 1), 0x5C1, 1473, 0, 0xA3Eu
#define PPUTLIMPL_UHEX_0x5C0u 1472u, (5, C, 0), 0x5C0, 1472, 0, 0xA3Fu
#define PPUTLIMPL_UHEX_0x5BFu 1471u, (5, B, F), 0x5BF, 1471, 0, 0xA40u
#define PPUTLIMPL_UHEX_0x5BEu 1470u, (5, B, E), 0x5BE, 1470, 0, 0xA41u
#define PPUTLIMPL_UHEX_0x5BDu 1469u, (5, B, D), 0x5BD, 1469, 0, 0xA42u
#define PPUTLIMPL_UHEX_0x5BCu 1468u, (5, B, C), 0x5BC, 1468, 0, 0xA43u
#define PPUTLIMPL_UHEX_0x5BBu 1467u, (5, B, B), 0x5BB, 1467, 0, 0xA44u
#define PPUTLIMPL_UHEX_0x5BAu 1466u, (5, B, A), 0x5BA, 1466, 0, 0xA45u
#define PPUTLIMPL_UHEX_0x5B9u 1465u, (5, B, 9), 0x5B9, 1465, 0, 0xA46u
#define PPUTLIMPL_UHEX_0x5B8u 1464u, (5, B, 8), 0x5B8, 1464, 0, 0xA47u
#define PPUTLIMPL_UHEX_0x5B7u 1463u, (5, B, 7), 0x5B7, 1463, 0, 0xA48u
#define PPUTLIMPL_UHEX_0x5B6u 1462u, (5, B, 6), 0x5B6, 1462, 0, 0xA49u
#define PPUTLIMPL_UHEX_0x5B5u 1461u, (5, B, 5), 0x5B5, 1461, 0, 0xA4Au
#define PPUTLIMPL_UHEX_0x5B4u 1460u, (5, B, 4), 0x5B4, 1460, 0, 0xA4Bu
#define PPUTLIMPL_UHEX_0x5B3u 1459u, (5, B, 3), 0x5B3, 1459, 0, 0xA4Cu
#define PPUTLIMPL_UHEX_0x5B2u 1458u, (5, B, 2), 0x5B2, 1458, 0, 0xA4Du
#define PPUTLIMPL_UHEX_0x5B1u 1457u, (5, B, 1), 0x5B1, 1457, 0, 0xA4Eu
#define PPUTLIMPL_UHEX_0x5B0u 1456u, (5, B, 0), 0x5B0, 1456, 0, 0xA4Fu
#define PPUTLIMPL_UHEX_0x5AFu 1455u, (5, A, F), 0x5AF, 1455, 0, 0xA50u
#define PPUTLIMPL_UHEX_0x5AEu 1454u, (5, A, E), 0x5AE, 1454, 0, 0xA51u
#define PPUTLIMPL_UHEX_0x5ADu 1453u, (5, A, D), 0x5AD, 1453, 0, 0xA52u
#define PPUTLIMPL_UHEX_0x5ACu 1452u, (5, A, C), 0x5AC, 1452, 0, 0xA53u
#define PPUTLIMPL_UHEX_0x5ABu 1451u, (5, A, B), 0x5AB, 1451, 0, 0xA54u
#define PPUTLIMPL_UHEX_0x5AAu 1450u, (5, A, A), 0x5AA, 1450, 0, 0xA55u
#define PPUTLIMPL_UHEX_0x5A9u 1449u, (5, A, 9), 0x5A9, 1449, 0, 0xA56u
#define PPUTLIMPL_UHEX_0x5A8u 1448u, (5, A, 8), 0x5A8, 1448, 0, 0xA57u
#define PPUTLIMPL_UHEX_0x5A7u 1447u, (5, A, 7), 0x5A7, 1447, 0, 0xA58u
#define PPUTLIMPL_UHEX_0x5A6u 1446u, (5, A, 6), 0x5A6, 1446, 0, 0xA59u
#define PPUTLIMPL_UHEX_0x5A5u 1445u, (5, A, 5), 0x5A5, 1445, 0, 0xA5Au
#define PPUTLIMPL_UHEX_0x5A4u 1444u, (5, A, 4), 0x5A4, 1444, 0, 0xA5Bu
#define PPUTLIMPL_UHEX_0x5A3u 1443u, (5, A, 3), 0x5A3, 1443, 0, 0xA5Cu
#define PPUTLIMPL_UHEX_0x5A2u 1442u, (5, A, 2), 0x5A2, 1442, 0, 0xA5Du
#define PPUTLIMPL_UHEX_0x5A1u 1441u, (5, A, 1), 0x5A1, 1441, 0, 0xA5Eu
#define PPUTLIMPL_UHEX_0x5A0u 1440u, (5, A, 0), 0x5A0, 1440, 0, 0xA5Fu
#define PPUTLIMPL_UHEX_0x59Fu 1439u, (5, 9, F), 0x59F, 1439, 0, 0xA60u
#define PPUTLIMPL_UHEX_0x59Eu 1438u, (5, 9, E), 0x59E, 1438, 0, 0xA61u
#define PPUTLIMPL_UHEX_0x59Du 1437u, (5, 9, D), 0x59D, 1437, 0, 0xA62u
#define PPUTLIMPL_UHEX_0x59Cu 1436u, (5, 9, C), 0x59C, 1436, 0, 0xA63u
#define PPUTLIMPL_UHEX_0x59Bu 1435u, (5, 9, B), 0x59B, 1435, 0, 0xA64u
#define PPUTLIMPL_UHEX_0x59Au 1434u, (5, 9, A), 0x59A, 1434, 0, 0xA65u
#define PPUTLIMPL_UHEX_0x599u 1433u, (5, 9, 9), 0x599, 1433, 0, 0xA66u
#define PPUTLIMPL_UHEX_0x598u 1432u, (5, 9, 8), 0x598, 1432, 0, 0xA67u
#define PPUTLIMPL_UHEX_0x597u 1431u, (5, 9, 7), 0x597, 1431, 0, 0xA68u
#define PPUTLIMPL_UHEX_0x596u 1430u, (5, 9, 6), 0x596, 1430, 0, 0xA69u
#define PPUTLIMPL_UHEX_0x595u 1429u, (5, 9, 5), 0x595, 1429, 0, 0xA6Au
#define PPUTLIMPL_UHEX_0x594u 1428u, (5, 9, 4), 0x594, 1428, 0, 0xA6Bu
#define PPUTLIMPL_UHEX_0x593u 1427u, (5, 9, 3), 0x593, 1427, 0, 0xA6Cu
#define PPUTLIMPL_UHEX_0x592u 1426u, (5, 9, 2), 0x592, 1426, 0, 0xA6Du
#define PPUTLIMPL_UHEX_0x591u 1425u, (5, 9, 1), 0x591, 1425, 0, 0xA6Eu
#define PPUTLIMPL_UHEX_0x590u 1424u, (5, 9, 0), 0x590, 1424, 0, 0xA6Fu
#define PPUTLIMPL_UHEX_0x58Fu 1423u, (5, 8, F), 0x58F, 1423, 0, 0xA70u
#define PPUTLIMPL_UHEX_0x58Eu 1422u, (5, 8, E), 0x58E, 1422, 0, 0xA71u
#define PPUTLIMPL_UHEX_0x58Du 1421u, (5, 8, D), 0x58D, 1421, 0, 0xA72u
#define PPUTLIMPL_UHEX_0x58Cu 1420u, (5, 8, C), 0x58C, 1420, 0, 0xA73u
#define PPUTLIMPL_UHEX_0x58Bu 1419u, (5, 8, B), 0x58B, 1419, 0, 0xA74u
#define PPUTLIMPL_UHEX_0x58Au 1418u, (5, 8, A), 0x58A, 1418, 0, 0xA75u
#define PPUTLIMPL_UHEX_0x589u 1417u, (5, 8, 9), 0x589, 1417, 0, 0xA76u
#define PPUTLIMPL_UHEX_0x588u 1416u, (5, 8, 8), 0x588, 1416, 0, 0xA77u
#define PPUTLIMPL_UHEX_0x587u 1415u, (5, 8, 7), 0x587, 1415, 0, 0xA78u
#define PPUTLIMPL_UHEX_0x586u 1414u, (5, 8, 6), 0x586, 1414, 0, 0xA79u
#define PPUTLIMPL_UHEX_0x585u 1413u, (5, 8, 5), 0x585, 1413, 0, 0xA7Au
#define PPUTLIMPL_UHEX_0x584u 1412u, (5, 8, 4), 0x584, 1412, 0, 0xA7Bu
#define PPUTLIMPL_UHEX_0x583u 1411u, (5, 8, 3), 0x583, 1411, 0, 0xA7Cu
#define PPUTLIMPL_UHEX_0x582u 1410u, (5, 8, 2), 0x582, 1410, 0, 0xA7Du
#define PPUTLIMPL_UHEX_0x581u 1409u, (5, 8, 1), 0x581, 1409, 0, 0xA7Eu
#define PPUTLIMPL_UHEX_0x580u 1408u, (5, 8, 0), 0x580, 1408, 0, 0xA7Fu
#define PPUTLIMPL_UHEX_0x57Fu 1407u, (5, 7, F), 0x57F, 1407, 0, 0xA80u
#define PPUTLIMPL_UHEX_0x57Eu 1406u, (5, 7, E), 0x57E, 1406, 0, 0xA81u
#define PPUTLIMPL_UHEX_0x57Du 1405u, (5, 7, D), 0x57D, 1405, 0, 0xA82u
#define PPUTLIMPL_UHEX_0x57Cu 1404u, (5, 7, C), 0x57C, 1404, 0, 0xA83u
#define PPUTLIMPL_UHEX_0x57Bu 1403u, (5, 7, B), 0x57B, 1403, 0, 0xA84u
#define PPUTLIMPL_UHEX_0x57Au 1402u, (5, 7, A), 0x57A, 1402, 0, 0xA85u
#define PPUTLIMPL_UHEX_0x579u 1401u, (5, 7, 9), 0x579, 1401, 0, 0xA86u
#define PPUTLIMPL_UHEX_0x578u 1400u, (5, 7, 8), 0x578, 1400, 0, 0xA87u
#define PPUTLIMPL_UHEX_0x577u 1399u, (5, 7, 7), 0x577, 1399, 0, 0xA88u
#define PPUTLIMPL_UHEX_0x576u 1398u, (5, 7, 6), 0x576, 1398, 0, 0xA89u
#define PPUTLIMPL_UHEX_0x575u 1397u, (5, 7, 5), 0x575, 1397, 0, 0xA8Au
#define PPUTLIMPL_UHEX_0x574u 1396u, (5, 7, 4), 0x574, 1396, 0, 0xA8Bu
#define PPUTLIMPL_UHEX_0x573u 1395u, (5, 7, 3), 0x573, 1395, 0, 0xA8Cu
#define PPUTLIMPL_UHEX_0x572u 1394u, (5, 7, 2), 0x572, 1394, 0, 0xA8Du
#define PPUTLIMPL_UHEX_0x571u 1393u, (5, 7, 1), 0x571, 1393, 0, 0xA8Eu
#define PPUTLIMPL_UHEX_0x570u 1392u, (5, 7, 0), 0x570, 1392, 0, 0xA8Fu
#define PPUTLIMPL_UHEX_0x56Fu 1391u, (5, 6, F), 0x56F, 1391, 0, 0xA90u
#define PPUTLIMPL_UHEX_0x56Eu 1390u, (5, 6, E), 0x56E, 1390, 0, 0xA91u
#define PPUTLIMPL_UHEX_0x56Du 1389u, (5, 6, D), 0x56D, 1389, 0, 0xA92u
#define PPUTLIMPL_UHEX_0x56Cu 1388u, (5, 6, C), 0x56C, 1388, 0, 0xA93u
#define PPUTLIMPL_UHEX_0x56Bu 1387u, (5, 6, B), 0x56B, 1387, 0, 0xA94u
#define PPUTLIMPL_UHEX_0x56Au 1386u, (5, 6, A), 0x56A, 1386, 0, 0xA95u
#define PPUTLIMPL_UHEX_0x569u 1385u, (5, 6, 9), 0x569, 1385, 0, 0xA96u
#define PPUTLIMPL_UHEX_0x568u 1384u, (5, 6, 8), 0x568, 1384, 0, 0xA97u
#define PPUTLIMPL_UHEX_0x567u 1383u, (5, 6, 7), 0x567, 1383, 0, 0xA98u
#define PPUTLIMPL_UHEX_0x566u 1382u, (5, 6, 6), 0x566, 1382, 0, 0xA99u
#define PPUTLIMPL_UHEX_0x565u 1381u, (5, 6, 5), 0x565, 1381, 0, 0xA9Au
#define PPUTLIMPL_UHEX_0x564u 1380u, (5, 6, 4), 0x564, 1380, 0, 0xA9Bu
#define PPUTLIMPL_UHEX_0x563u 1379u, (5, 6, 3), 0x563, 1379, 0, 0xA9Cu
#define PPUTLIMPL_UHEX_0x562u 1378u, (5, 6, 2), 0x562, 1378, 0, 0xA9Du
#define PPUTLIMPL_UHEX_0x561u 1377u, (5, 6, 1), 0x561, 1377, 0, 0xA9Eu
#define PPUTLIMPL_UHEX_0x560u 1376u, (5, 6, 0), 0x560, 1376, 0, 0xA9Fu
#define PPUTLIMPL_UHEX_0x55Fu 1375u, (5, 5, F), 0x55F, 1375, 0, 0xAA0u
#define PPUTLIMPL_UHEX_0x55Eu 1374u, (5, 5, E), 0x55E, 1374, 0, 0xAA1u
#define PPUTLIMPL_UHEX_0x55Du 1373u, (5, 5, D), 0x55D, 1373, 0, 0xAA2u
#define PPUTLIMPL_UHEX_0x55Cu 1372u, (5, 5, C), 0x55C, 1372, 0, 0xAA3u
#define PPUTLIMPL_UHEX_0x55Bu 1371u, (5, 5, B), 0x55B, 1371, 0, 0xAA4u
#define PPUTLIMPL_UHEX_0x55Au 1370u, (5, 5, A), 0x55A, 1370, 0, 0xAA5u
#define PPUTLIMPL_UHEX_0x559u 1369u, (5, 5, 9), 0x559, 1369, 0, 0xAA6u
#define PPUTLIMPL_UHEX_0x558u 1368u, (5, 5, 8), 0x558, 1368, 0, 0xAA7u
#define PPUTLIMPL_UHEX_0x557u 1367u, (5, 5, 7), 0x557, 1367, 0, 0xAA8u
#define PPUTLIMPL_UHEX_0x556u 1366u, (5, 5, 6), 0x556, 1366, 0, 0xAA9u
#define PPUTLIMPL_UHEX_0x555u 1365u, (5, 5, 5), 0x555, 1365, 0, 0xAAAu
#define PPUTLIMPL_UHEX_0x554u 1364u, (5, 5, 4), 0x554, 1364, 0, 0xAABu
#define PPUTLIMPL_UHEX_0x553u 1363u, (5, 5, 3), 0x553, 1363, 0, 0xAACu
#define PPUTLIMPL_UHEX_0x552u 1362u, (5, 5, 2), 0x552, 1362, 0, 0xAADu
#define PPUTLIMPL_UHEX_0x551u 1361u, (5, 5, 1), 0x551, 1361, 0, 0xAAEu
#define PPUTLIMPL_UHEX_0x550u 1360u, (5, 5, 0), 0x550, 1360, 0, 0xAAFu
#define PPUTLIMPL_UHEX_0x54Fu 1359u, (5, 4, F), 0x54F, 1359, 0, 0xAB0u
#define PPUTLIMPL_UHEX_0x54Eu 1358u, (5, 4, E), 0x54E, 1358, 0, 0xAB1u
#define PPUTLIMPL_UHEX_0x54Du 1357u, (5, 4, D), 0x54D, 1357, 0, 0xAB2u
#define PPUTLIMPL_UHEX_0x54Cu 1356u, (5, 4, C), 0x54C, 1356, 0, 0xAB3u
#define PPUTLIMPL_UHEX_0x54Bu 1355u, (5, 4, B), 0x54B, 1355, 0, 0xAB4u
#define PPUTLIMPL_UHEX_0x54Au 1354u, (5, 4, A), 0x54A, 1354, 0, 0xAB5u
#define PPUTLIMPL_UHEX_0x549u 1353u, (5, 4, 9), 0x549, 1353, 0, 0xAB6u
#define PPUTLIMPL_UHEX_0x548u 1352u, (5, 4, 8), 0x548, 1352, 0, 0xAB7u
#define PPUTLIMPL_UHEX_0x547u 1351u, (5, 4, 7), 0x547, 1351, 0, 0xAB8u
#define PPUTLIMPL_UHEX_0x546u 1350u, (5, 4, 6), 0x546, 1350, 0, 0xAB9u
#define PPUTLIMPL_UHEX_0x545u 1349u, (5, 4, 5), 0x545, 1349, 0, 0xABAu
#define PPUTLIMPL_UHEX_0x544u 1348u, (5, 4, 4), 0x544, 1348, 0, 0xABBu
#define PPUTLIMPL_UHEX_0x543u 1347u, (5, 4, 3), 0x543, 1347, 0, 0xABCu
#define PPUTLIMPL_UHEX_0x542u 1346u, (5, 4, 2), 0x542, 1346, 0, 0xABDu
#define PPUTLIMPL_UHEX_0x541u 1345u, (5, 4, 1), 0x541, 1345, 0, 0xABEu
#define PPUTLIMPL_UHEX_0x540u 1344u, (5, 4, 0), 0x540, 1344, 0, 0xABFu
#define PPUTLIMPL_UHEX_0x53Fu 1343u, (5, 3, F), 0x53F, 1343, 0, 0xAC0u
#define PPUTLIMPL_UHEX_0x53Eu 1342u, (5, 3, E), 0x53E, 1342, 0, 0xAC1u
#define PPUTLIMPL_UHEX_0x53Du 1341u, (5, 3, D), 0x53D, 1341, 0, 0xAC2u
#define PPUTLIMPL_UHEX_0x53Cu 1340u, (5, 3, C), 0x53C, 1340, 0, 0xAC3u
#define PPUTLIMPL_UHEX_0x53Bu 1339u, (5, 3, B), 0x53B, 1339, 0, 0xAC4u
#define PPUTLIMPL_UHEX_0x53Au 1338u, (5, 3, A), 0x53A, 1338, 0, 0xAC5u
#define PPUTLIMPL_UHEX_0x539u 1337u, (5, 3, 9), 0x539, 1337, 0, 0xAC6u
#define PPUTLIMPL_UHEX_0x538u 1336u, (5, 3, 8), 0x538, 1336, 0, 0xAC7u
#define PPUTLIMPL_UHEX_0x537u 1335u, (5, 3, 7), 0x537, 1335, 0, 0xAC8u
#define PPUTLIMPL_UHEX_0x536u 1334u, (5, 3, 6), 0x536, 1334, 0, 0xAC9u
#define PPUTLIMPL_UHEX_0x535u 1333u, (5, 3, 5), 0x535, 1333, 0, 0xACAu
#define PPUTLIMPL_UHEX_0x534u 1332u, (5, 3, 4), 0x534, 1332, 0, 0xACBu
#define PPUTLIMPL_UHEX_0x533u 1331u, (5, 3, 3), 0x533, 1331, 0, 0xACCu
#define PPUTLIMPL_UHEX_0x532u 1330u, (5, 3, 2), 0x532, 1330, 0, 0xACDu
#define PPUTLIMPL_UHEX_0x531u 1329u, (5, 3, 1), 0x531, 1329, 0, 0xACEu
#define PPUTLIMPL_UHEX_0x530u 1328u, (5, 3, 0), 0x530, 1328, 0, 0xACFu
#define PPUTLIMPL_UHEX_0x52Fu 1327u, (5, 2, F), 0x52F, 1327, 0, 0xAD0u
#define PPUTLIMPL_UHEX_0x52Eu 1326u, (5, 2, E), 0x52E, 1326, 0, 0xAD1u
#define PPUTLIMPL_UHEX_0x52Du 1325u, (5, 2, D), 0x52D, 1325, 0, 0xAD2u
#define PPUTLIMPL_UHEX_0x52Cu 1324u, (5, 2, C), 0x52C, 1324, 0, 0xAD3u
#define PPUTLIMPL_UHEX_0x52Bu 1323u, (5, 2, B), 0x52B, 1323, 0, 0xAD4u
#define PPUTLIMPL_UHEX_0x52Au 1322u, (5, 2, A), 0x52A, 1322, 0, 0xAD5u
#define PPUTLIMPL_UHEX_0x529u 1321u, (5, 2, 9), 0x529, 1321, 0, 0xAD6u
#define PPUTLIMPL_UHEX_0x528u 1320u, (5, 2, 8), 0x528, 1320, 0, 0xAD7u
#define PPUTLIMPL_UHEX_0x527u 1319u, (5, 2, 7), 0x527, 1319, 0, 0xAD8u
#define PPUTLIMPL_UHEX_0x526u 1318u, (5, 2, 6), 0x526, 1318, 0, 0xAD9u
#define PPUTLIMPL_UHEX_0x525u 1317u, (5, 2, 5), 0x525, 1317, 0, 0xADAu
#define PPUTLIMPL_UHEX_0x524u 1316u, (5, 2, 4), 0x524, 1316, 0, 0xADBu
#define PPUTLIMPL_UHEX_0x523u 1315u, (5, 2, 3), 0x523, 1315, 0, 0xADCu
#define PPUTLIMPL_UHEX_0x522u 1314u, (5, 2, 2), 0x522, 1314, 0, 0xADDu
#define PPUTLIMPL_UHEX_0x521u 1313u, (5, 2, 1), 0x521, 1313, 0, 0xADEu
#define PPUTLIMPL_UHEX_0x520u 1312u, (5, 2, 0), 0x520, 1312, 0, 0xADFu
#define PPUTLIMPL_UHEX_0x51Fu 1311u, (5, 1, F), 0x51F, 1311, 0, 0xAE0u
#define PPUTLIMPL_UHEX_0x51Eu 1310u, (5, 1, E), 0x51E, 1310, 0, 0xAE1u
#define PPUTLIMPL_UHEX_0x51Du 1309u, (5, 1, D), 0x51D, 1309, 0, 0xAE2u
#define PPUTLIMPL_UHEX_0x51Cu 1308u, (5, 1, C), 0x51C, 1308, 0, 0xAE3u
#define PPUTLIMPL_UHEX_0x51Bu 1307u, (5, 1, B), 0x51B, 1307, 0, 0xAE4u
#define PPUTLIMPL_UHEX_0x51Au 1306u, (5, 1, A), 0x51A, 1306, 0, 0xAE5u
#define PPUTLIMPL_UHEX_0x519u 1305u, (5, 1, 9), 0x519, 1305, 0, 0xAE6u
#define PPUTLIMPL_UHEX_0x518u 1304u, (5, 1, 8), 0x518, 1304, 0, 0xAE7u
#define PPUTLIMPL_UHEX_0x517u 1303u, (5, 1, 7), 0x517, 1303, 0, 0xAE8u
#define PPUTLIMPL_UHEX_0x516u 1302u, (5, 1, 6), 0x516, 1302, 0, 0xAE9u
#define PPUTLIMPL_UHEX_0x515u 1301u, (5, 1, 5), 0x515, 1301, 0, 0xAEAu
#define PPUTLIMPL_UHEX_0x514u 1300u, (5, 1, 4), 0x514, 1300, 0, 0xAEBu
#define PPUTLIMPL_UHEX_0x513u 1299u, (5, 1, 3), 0x513, 1299, 0, 0xAECu
#define PPUTLIMPL_UHEX_0x512u 1298u, (5, 1, 2), 0x512, 1298, 0, 0xAEDu
#define PPUTLIMPL_UHEX_0x511u 1297u, (5, 1, 1), 0x511, 1297, 0, 0xAEEu
#define PPUTLIMPL_UHEX_0x510u 1296u, (5, 1, 0), 0x510, 1296, 0, 0xAEFu
#define PPUTLIMPL_UHEX_0x50Fu 1295u, (5, 0, F), 0x50F, 1295, 0, 0xAF0u
#define PPUTLIMPL_UHEX_0x50Eu 1294u, (5, 0, E), 0x50E, 1294, 0, 0xAF1u
#define PPUTLIMPL_UHEX_0x50Du 1293u, (5, 0, D), 0x50D, 1293, 0, 0xAF2u
#define PPUTLIMPL_UHEX_0x50Cu 1292u, (5, 0, C), 0x50C, 1292, 0, 0xAF3u
#define PPUTLIMPL_UHEX_0x50Bu 1291u, (5, 0, B), 0x50B, 1291, 0, 0xAF4u
#define PPUTLIMPL_UHEX_0x50Au 1290u, (5, 0, A), 0x50A, 1290, 0, 0xAF5u
#define PPUTLIMPL_UHEX_0x509u 1289u, (5, 0, 9), 0x509, 1289, 0, 0xAF6u
#define PPUTLIMPL_UHEX_0x508u 1288u, (5, 0, 8), 0x508, 1288, 0, 0xAF7u
#define PPUTLIMPL_UHEX_0x507u 1287u, (5, 0, 7), 0x507, 1287, 0, 0xAF8u
#define PPUTLIMPL_UHEX_0x506u 1286u, (5, 0, 6), 0x506, 1286, 0, 0xAF9u
#define PPUTLIMPL_UHEX_0x505u 1285u, (5, 0, 5), 0x505, 1285, 0, 0xAFAu
#define PPUTLIMPL_UHEX_0x504u 1284u, (5, 0, 4), 0x504, 1284, 0, 0xAFBu
#define PPUTLIMPL_UHEX_0x503u 1283u, (5, 0, 3), 0x503, 1283, 0, 0xAFCu
#define PPUTLIMPL_UHEX_0x502u 1282u, (5, 0, 2), 0x502, 1282, 0, 0xAFDu
#define PPUTLIMPL_UHEX_0x501u 1281u, (5, 0, 1), 0x501, 1281, 0, 0xAFEu
#define PPUTLIMPL_UHEX_0x500u 1280u, (5, 0, 0), 0x500, 1280, 0, 0xAFFu
#define PPUTLIMPL_UHEX_0x4FFu 1279u, (4, F, F), 0x4FF, 1279, 0, 0xB00u
#define PPUTLIMPL_UHEX_0x4FEu 1278u, (4, F, E), 0x4FE, 1278, 0, 0xB01u
#define PPUTLIMPL_UHEX_0x4FDu 1277u, (4, F, D), 0x4FD, 1277, 0, 0xB02u
#define PPUTLIMPL_UHEX_0x4FCu 1276u, (4, F, C), 0x4FC, 1276, 0, 0xB03u
#define PPUTLIMPL_UHEX_0x4FBu 1275u, (4, F, B), 0x4FB, 1275, 0, 0xB04u
#define PPUTLIMPL_UHEX_0x4FAu 1274u, (4, F, A), 0x4FA, 1274, 0, 0xB05u
#define PPUTLIMPL_UHEX_0x4F9u 1273u, (4, F, 9), 0x4F9, 1273, 0, 0xB06u
#define PPUTLIMPL_UHEX_0x4F8u 1272u, (4, F, 8), 0x4F8, 1272, 0, 0xB07u
#define PPUTLIMPL_UHEX_0x4F7u 1271u, (4, F, 7), 0x4F7, 1271, 0, 0xB08u
#define PPUTLIMPL_UHEX_0x4F6u 1270u, (4, F, 6), 0x4F6, 1270, 0, 0xB09u
#define PPUTLIMPL_UHEX_0x4F5u 1269u, (4, F, 5), 0x4F5, 1269, 0, 0xB0Au
#define PPUTLIMPL_UHEX_0x4F4u 1268u, (4, F, 4), 0x4F4, 1268, 0, 0xB0Bu
#define PPUTLIMPL_UHEX_0x4F3u 1267u, (4, F, 3), 0x4F3, 1267, 0, 0xB0Cu
#define PPUTLIMPL_UHEX_0x4F2u 1266u, (4, F, 2), 0x4F2, 1266, 0, 0xB0Du
#define PPUTLIMPL_UHEX_0x4F1u 1265u, (4, F, 1), 0x4F1, 1265, 0, 0xB0Eu
#define PPUTLIMPL_UHEX_0x4F0u 1264u, (4, F, 0), 0x4F0, 1264, 0, 0xB0Fu
#define PPUTLIMPL_UHEX_0x4EFu 1263u, (4, E, F), 0x4EF, 1263, 0, 0xB10u
#define PPUTLIMPL_UHEX_0x4EEu 1262u, (4, E, E), 0x4EE, 1262, 0, 0xB11u
#define PPUTLIMPL_UHEX_0x4EDu 1261u, (4, E, D), 0x4ED, 1261, 0, 0xB12u
#define PPUTLIMPL_UHEX_0x4ECu 1260u, (4, E, C), 0x4EC, 1260, 0, 0xB13u
#define PPUTLIMPL_UHEX_0x4EBu 1259u, (4, E, B), 0x4EB, 1259, 0, 0xB14u
#define PPUTLIMPL_UHEX_0x4EAu 1258u, (4, E, A), 0x4EA, 1258, 0, 0xB15u
#define PPUTLIMPL_UHEX_0x4E9u 1257u, (4, E, 9), 0x4E9, 1257, 0, 0xB16u
#define PPUTLIMPL_UHEX_0x4E8u 1256u, (4, E, 8), 0x4E8, 1256, 0, 0xB17u
#define PPUTLIMPL_UHEX_0x4E7u 1255u, (4, E, 7), 0x4E7, 1255, 0, 0xB18u
#define PPUTLIMPL_UHEX_0x4E6u 1254u, (4, E, 6), 0x4E6, 1254, 0, 0xB19u
#define PPUTLIMPL_UHEX_0x4E5u 1253u, (4, E, 5), 0x4E5, 1253, 0, 0xB1Au
#define PPUTLIMPL_UHEX_0x4E4u 1252u, (4, E, 4), 0x4E4, 1252, 0, 0xB1Bu
#define PPUTLIMPL_UHEX_0x4E3u 1251u, (4, E, 3), 0x4E3, 1251, 0, 0xB1Cu
#define PPUTLIMPL_UHEX_0x4E2u 1250u, (4, E, 2), 0x4E2, 1250, 0, 0xB1Du
#define PPUTLIMPL_UHEX_0x4E1u 1249u, (4, E, 1), 0x4E1, 1249, 0, 0xB1Eu
#define PPUTLIMPL_UHEX_0x4E0u 1248u, (4, E, 0), 0x4E0, 1248, 0, 0xB1Fu
#define PPUTLIMPL_UHEX_0x4DFu 1247u, (4, D, F), 0x4DF, 1247, 0, 0xB20u
#define PPUTLIMPL_UHEX_0x4DEu 1246u, (4, D, E), 0x4DE, 1246, 0, 0xB21u
#define PPUTLIMPL_UHEX_0x4DDu 1245u, (4, D, D), 0x4DD, 1245, 0, 0xB22u
#define PPUTLIMPL_UHEX_0x4DCu 1244u, (4, D, C), 0x4DC, 1244, 0, 0xB23u
#define PPUTLIMPL_UHEX_0x4DBu 1243u, (4, D, B), 0x4DB, 1243, 0, 0xB24u
#define PPUTLIMPL_UHEX_0x4DAu 1242u, (4, D, A), 0x4DA, 1242, 0, 0xB25u
#define PPUTLIMPL_UHEX_0x4D9u 1241u, (4, D, 9), 0x4D9, 1241, 0, 0xB26u
#define PPUTLIMPL_UHEX_0x4D8u 1240u, (4, D, 8), 0x4D8, 1240, 0, 0xB27u
#define PPUTLIMPL_UHEX_0x4D7u 1239u, (4, D, 7), 0x4D7, 1239, 0, 0xB28u
#define PPUTLIMPL_UHEX_0x4D6u 1238u, (4, D, 6), 0x4D6, 1238, 0, 0xB29u
#define PPUTLIMPL_UHEX_0x4D5u 1237u, (4, D, 5), 0x4D5, 1237, 0, 0xB2Au
#define PPUTLIMPL_UHEX_0x4D4u 1236u, (4, D, 4), 0x4D4, 1236, 0, 0xB2Bu
#define PPUTLIMPL_UHEX_0x4D3u 1235u, (4, D, 3), 0x4D3, 1235, 0, 0xB2Cu
#define PPUTLIMPL_UHEX_0x4D2u 1234u, (4, D, 2), 0x4D2, 1234, 0, 0xB2Du
#define PPUTLIMPL_UHEX_0x4D1u 1233u, (4, D, 1), 0x4D1, 1233, 0, 0xB2Eu
#define PPUTLIMPL_UHEX_0x4D0u 1232u, (4, D, 0), 0x4D0, 1232, 0, 0xB2Fu
#define PPUTLIMPL_UHEX_0x4CFu 1231u, (4, C, F), 0x4CF, 1231, 0, 0xB30u
#define PPUTLIMPL_UHEX_0x4CEu 1230u, (4, C, E), 0x4CE, 1230, 0, 0xB31u
#define PPUTLIMPL_UHEX_0x4CDu 1229u, (4, C, D), 0x4CD, 1229, 0, 0xB32u
#define PPUTLIMPL_UHEX_0x4CCu 1228u, (4, C, C), 0x4CC, 1228, 0, 0xB33u
#define PPUTLIMPL_UHEX_0x4CBu 1227u, (4, C, B), 0x4CB, 1227, 0, 0xB34u
#define PPUTLIMPL_UHEX_0x4CAu 1226u, (4, C, A), 0x4CA, 1226, 0, 0xB35u
#define PPUTLIMPL_UHEX_0x4C9u 1225u, (4, C, 9), 0x4C9, 1225, 0, 0xB36u
#define PPUTLIMPL_UHEX_0x4C8u 1224u, (4, C, 8), 0x4C8, 1224, 0, 0xB37u
#define PPUTLIMPL_UHEX_0x4C7u 1223u, (4, C, 7), 0x4C7, 1223, 0, 0xB38u
#define PPUTLIMPL_UHEX_0x4C6u 1222u, (4, C, 6), 0x4C6, 1222, 0, 0xB39u
#define PPUTLIMPL_UHEX_0x4C5u 1221u, (4, C, 5), 0x4C5, 1221, 0, 0xB3Au
#define PPUTLIMPL_UHEX_0x4C4u 1220u, (4, C, 4), 0x4C4, 1220, 0, 0xB3Bu
#define PPUTLIMPL_UHEX_0x4C3u 1219u, (4, C, 3), 0x4C3, 1219, 0, 0xB3Cu
#define PPUTLIMPL_UHEX_0x4C2u 1218u, (4, C, 2), 0x4C2, 1218, 0, 0xB3Du
#define PPUTLIMPL_UHEX_0x4C1u 1217u, (4, C, 1), 0x4C1, 1217, 0, 0xB3Eu
#define PPUTLIMPL_UHEX_0x4C0u 1216u, (4, C, 0), 0x4C0, 1216, 0, 0xB3Fu
#define PPUTLIMPL_UHEX_0x4BFu 1215u, (4, B, F), 0x4BF, 1215, 0, 0xB40u
#define PPUTLIMPL_UHEX_0x4BEu 1214u, (4, B, E), 0x4BE, 1214, 0, 0xB41u
#define PPUTLIMPL_UHEX_0x4BDu 1213u, (4, B, D), 0x4BD, 1213, 0, 0xB42u
#define PPUTLIMPL_UHEX_0x4BCu 1212u, (4, B, C), 0x4BC, 1212, 0, 0xB43u
#define PPUTLIMPL_UHEX_0x4BBu 1211u, (4, B, B), 0x4BB, 1211, 0, 0xB44u
#define PPUTLIMPL_UHEX_0x4BAu 1210u, (4, B, A), 0x4BA, 1210, 0, 0xB45u
#define PPUTLIMPL_UHEX_0x4B9u 1209u, (4, B, 9), 0x4B9, 1209, 0, 0xB46u
#define PPUTLIMPL_UHEX_0x4B8u 1208u, (4, B, 8), 0x4B8, 1208, 0, 0xB47u
#define PPUTLIMPL_UHEX_0x4B7u 1207u, (4, B, 7), 0x4B7, 1207, 0, 0xB48u
#define PPUTLIMPL_UHEX_0x4B6u 1206u, (4, B, 6), 0x4B6, 1206, 0, 0xB49u
#define PPUTLIMPL_UHEX_0x4B5u 1205u, (4, B, 5), 0x4B5, 1205, 0, 0xB4Au
#define PPUTLIMPL_UHEX_0x4B4u 1204u, (4, B, 4), 0x4B4, 1204, 0, 0xB4Bu
#define PPUTLIMPL_UHEX_0x4B3u 1203u, (4, B, 3), 0x4B3, 1203, 0, 0xB4Cu
#define PPUTLIMPL_UHEX_0x4B2u 1202u, (4, B, 2), 0x4B2, 1202, 0, 0xB4Du
#define PPUTLIMPL_UHEX_0x4B1u 1201u, (4, B, 1), 0x4B1, 1201, 0, 0xB4Eu
#define PPUTLIMPL_UHEX_0x4B0u 1200u, (4, B, 0), 0x4B0, 1200, 0, 0xB4Fu
#define PPUTLIMPL_UHEX_0x4AFu 1199u, (4, A, F), 0x4AF, 1199, 0, 0xB50u
#define PPUTLIMPL_UHEX_0x4AEu 1198u, (4, A, E), 0x4AE, 1198, 0, 0xB51u
#define PPUTLIMPL_UHEX_0x4ADu 1197u, (4, A, D), 0x4AD, 1197, 0, 0xB52u
#define PPUTLIMPL_UHEX_0x4ACu 1196u, (4, A, C), 0x4AC, 1196, 0, 0xB53u
#define PPUTLIMPL_UHEX_0x4ABu 1195u, (4, A, B), 0x4AB, 1195, 0, 0xB54u
#define PPUTLIMPL_UHEX_0x4AAu 1194u, (4, A, A), 0x4AA, 1194, 0, 0xB55u
#define PPUTLIMPL_UHEX_0x4A9u 1193u, (4, A, 9), 0x4A9, 1193, 0, 0xB56u
#define PPUTLIMPL_UHEX_0x4A8u 1192u, (4, A, 8), 0x4A8, 1192, 0, 0xB57u
#define PPUTLIMPL_UHEX_0x4A7u 1191u, (4, A, 7), 0x4A7, 1191, 0, 0xB58u
#define PPUTLIMPL_UHEX_0x4A6u 1190u, (4, A, 6), 0x4A6, 1190, 0, 0xB59u
#define PPUTLIMPL_UHEX_0x4A5u 1189u, (4, A, 5), 0x4A5, 1189, 0, 0xB5Au
#define PPUTLIMPL_UHEX_0x4A4u 1188u, (4, A, 4), 0x4A4, 1188, 0, 0xB5Bu
#define PPUTLIMPL_UHEX_0x4A3u 1187u, (4, A, 3), 0x4A3, 1187, 0, 0xB5Cu
#define PPUTLIMPL_UHEX_0x4A2u 1186u, (4, A, 2), 0x4A2, 1186, 0, 0xB5Du
#define PPUTLIMPL_UHEX_0x4A1u 1185u, (4, A, 1), 0x4A1, 1185, 0, 0xB5Eu
#define PPUTLIMPL_UHEX_0x4A0u 1184u, (4, A, 0), 0x4A0, 1184, 0, 0xB5Fu
#define PPUTLIMPL_UHEX_0x49Fu 1183u, (4, 9, F), 0x49F, 1183, 0, 0xB60u
#define PPUTLIMPL_UHEX_0x49Eu 1182u, (4, 9, E), 0x49E, 1182, 0, 0xB61u
#define PPUTLIMPL_UHEX_0x49Du 1181u, (4, 9, D), 0x49D, 1181, 0, 0xB62u
#define PPUTLIMPL_UHEX_0x49Cu 1180u, (4, 9, C), 0x49C, 1180, 0, 0xB63u
#define PPUTLIMPL_UHEX_0x49Bu 1179u, (4, 9, B), 0x49B, 1179, 0, 0xB64u
#define PPUTLIMPL_UHEX_0x49Au 1178u, (4, 9, A), 0x49A, 1178, 0, 0xB65u
#define PPUTLIMPL_UHEX_0x499u 1177u, (4, 9, 9), 0x499, 1177, 0, 0xB66u
#define PPUTLIMPL_UHEX_0x498u 1176u, (4, 9, 8), 0x498, 1176, 0, 0xB67u
#define PPUTLIMPL_UHEX_0x497u 1175u, (4, 9, 7), 0x497, 1175, 0, 0xB68u
#define PPUTLIMPL_UHEX_0x496u 1174u, (4, 9, 6), 0x496, 1174, 0, 0xB69u
#define PPUTLIMPL_UHEX_0x495u 1173u, (4, 9, 5), 0x495, 1173, 0, 0xB6Au
#define PPUTLIMPL_UHEX_0x494u 1172u, (4, 9, 4), 0x494, 1172, 0, 0xB6Bu
#define PPUTLIMPL_UHEX_0x493u 1171u, (4, 9, 3), 0x493, 1171, 0, 0xB6Cu
#define PPUTLIMPL_UHEX_0x492u 1170u, (4, 9, 2), 0x492, 1170, 0, 0xB6Du
#define PPUTLIMPL_UHEX_0x491u 1169u, (4, 9, 1), 0x491, 1169, 0, 0xB6Eu
#define PPUTLIMPL_UHEX_0x490u 1168u, (4, 9, 0), 0x490, 1168, 0, 0xB6Fu
#define PPUTLIMPL_UHEX_0x48Fu 1167u, (4, 8, F), 0x48F, 1167, 0, 0xB70u
#define PPUTLIMPL_UHEX_0x48Eu 1166u, (4, 8, E), 0x48E, 1166, 0, 0xB71u
#define PPUTLIMPL_UHEX_0x48Du 1165u, (4, 8, D), 0x48D, 1165, 0, 0xB72u
#define PPUTLIMPL_UHEX_0x48Cu 1164u, (4, 8, C), 0x48C, 1164, 0, 0xB73u
#define PPUTLIMPL_UHEX_0x48Bu 1163u, (4, 8, B), 0x48B, 1163, 0, 0xB74u
#define PPUTLIMPL_UHEX_0x48Au 1162u, (4, 8, A), 0x48A, 1162, 0, 0xB75u
#define PPUTLIMPL_UHEX_0x489u 1161u, (4, 8, 9), 0x489, 1161, 0, 0xB76u
#define PPUTLIMPL_UHEX_0x488u 1160u, (4, 8, 8), 0x488, 1160, 0, 0xB77u
#define PPUTLIMPL_UHEX_0x487u 1159u, (4, 8, 7), 0x487, 1159, 0, 0xB78u
#define PPUTLIMPL_UHEX_0x486u 1158u, (4, 8, 6), 0x486, 1158, 0, 0xB79u
#define PPUTLIMPL_UHEX_0x485u 1157u, (4, 8, 5), 0x485, 1157, 0, 0xB7Au
#define PPUTLIMPL_UHEX_0x484u 1156u, (4, 8, 4), 0x484, 1156, 0, 0xB7Bu
#define PPUTLIMPL_UHEX_0x483u 1155u, (4, 8, 3), 0x483, 1155, 0, 0xB7Cu
#define PPUTLIMPL_UHEX_0x482u 1154u, (4, 8, 2), 0x482, 1154, 0, 0xB7Du
#define PPUTLIMPL_UHEX_0x481u 1153u, (4, 8, 1), 0x481, 1153, 0, 0xB7Eu
#define PPUTLIMPL_UHEX_0x480u 1152u, (4, 8, 0), 0x480, 1152, 0, 0xB7Fu
#define PPUTLIMPL_UHEX_0x47Fu 1151u, (4, 7, F), 0x47F, 1151, 0, 0xB80u
#define PPUTLIMPL_UHEX_0x47Eu 1150u, (4, 7, E), 0x47E, 1150, 0, 0xB81u
#define PPUTLIMPL_UHEX_0x47Du 1149u, (4, 7, D), 0x47D, 1149, 0, 0xB82u
#define PPUTLIMPL_UHEX_0x47Cu 1148u, (4, 7, C), 0x47C, 1148, 0, 0xB83u
#define PPUTLIMPL_UHEX_0x47Bu 1147u, (4, 7, B), 0x47B, 1147, 0, 0xB84u
#define PPUTLIMPL_UHEX_0x47Au 1146u, (4, 7, A), 0x47A, 1146, 0, 0xB85u
#define PPUTLIMPL_UHEX_0x479u 1145u, (4, 7, 9), 0x479, 1145, 0, 0xB86u
#define PPUTLIMPL_UHEX_0x478u 1144u, (4, 7, 8), 0x478, 1144, 0, 0xB87u
#define PPUTLIMPL_UHEX_0x477u 1143u, (4, 7, 7), 0x477, 1143, 0, 0xB88u
#define PPUTLIMPL_UHEX_0x476u 1142u, (4, 7, 6), 0x476, 1142, 0, 0xB89u
#define PPUTLIMPL_UHEX_0x475u 1141u, (4, 7, 5), 0x475, 1141, 0, 0xB8Au
#define PPUTLIMPL_UHEX_0x474u 1140u, (4, 7, 4), 0x474, 1140, 0, 0xB8Bu
#define PPUTLIMPL_UHEX_0x473u 1139u, (4, 7, 3), 0x473, 1139, 0, 0xB8Cu
#define PPUTLIMPL_UHEX_0x472u 1138u, (4, 7, 2), 0x472, 1138, 0, 0xB8Du
#define PPUTLIMPL_UHEX_0x471u 1137u, (4, 7, 1), 0x471, 1137, 0, 0xB8Eu
#define PPUTLIMPL_UHEX_0x470u 1136u, (4, 7, 0), 0x470, 1136, 0, 0xB8Fu
#define PPUTLIMPL_UHEX_0x46Fu 1135u, (4, 6, F), 0x46F, 1135, 0, 0xB90u
#define PPUTLIMPL_UHEX_0x46Eu 1134u, (4, 6, E), 0x46E, 1134, 0, 0xB91u
#define PPUTLIMPL_UHEX_0x46Du 1133u, (4, 6, D), 0x46D, 1133, 0, 0xB92u
#define PPUTLIMPL_UHEX_0x46Cu 1132u, (4, 6, C), 0x46C, 1132, 0, 0xB93u
#define PPUTLIMPL_UHEX_0x46Bu 1131u, (4, 6, B), 0x46B, 1131, 0, 0xB94u
#define PPUTLIMPL_UHEX_0x46Au 1130u, (4, 6, A), 0x46A, 1130, 0, 0xB95u
#define PPUTLIMPL_UHEX_0x469u 1129u, (4, 6, 9), 0x469, 1129, 0, 0xB96u
#define PPUTLIMPL_UHEX_0x468u 1128u, (4, 6, 8), 0x468, 1128, 0, 0xB97u
#define PPUTLIMPL_UHEX_0x467u 1127u, (4, 6, 7), 0x467, 1127, 0, 0xB98u
#define PPUTLIMPL_UHEX_0x466u 1126u, (4, 6, 6), 0x466, 1126, 0, 0xB99u
#define PPUTLIMPL_UHEX_0x465u 1125u, (4, 6, 5), 0x465, 1125, 0, 0xB9Au
#define PPUTLIMPL_UHEX_0x464u 1124u, (4, 6, 4), 0x464, 1124, 0, 0xB9Bu
#define PPUTLIMPL_UHEX_0x463u 1123u, (4, 6, 3), 0x463, 1123, 0, 0xB9Cu
#define PPUTLIMPL_UHEX_0x462u 1122u, (4, 6, 2), 0x462, 1122, 0, 0xB9Du
#define PPUTLIMPL_UHEX_0x461u 1121u, (4, 6, 1), 0x461, 1121, 0, 0xB9Eu
#define PPUTLIMPL_UHEX_0x460u 1120u, (4, 6, 0), 0x460, 1120, 0, 0xB9Fu
#define PPUTLIMPL_UHEX_0x45Fu 1119u, (4, 5, F), 0x45F, 1119, 0, 0xBA0u
#define PPUTLIMPL_UHEX_0x45Eu 1118u, (4, 5, E), 0x45E, 1118, 0, 0xBA1u
#define PPUTLIMPL_UHEX_0x45Du 1117u, (4, 5, D), 0x45D, 1117, 0, 0xBA2u
#define PPUTLIMPL_UHEX_0x45Cu 1116u, (4, 5, C), 0x45C, 1116, 0, 0xBA3u
#define PPUTLIMPL_UHEX_0x45Bu 1115u, (4, 5, B), 0x45B, 1115, 0, 0xBA4u
#define PPUTLIMPL_UHEX_0x45Au 1114u, (4, 5, A), 0x45A, 1114, 0, 0xBA5u
#define PPUTLIMPL_UHEX_0x459u 1113u, (4, 5, 9), 0x459, 1113, 0, 0xBA6u
#define PPUTLIMPL_UHEX_0x458u 1112u, (4, 5, 8), 0x458, 1112, 0, 0xBA7u
#define PPUTLIMPL_UHEX_0x457u 1111u, (4, 5, 7), 0x457, 1111, 0, 0xBA8u
#define PPUTLIMPL_UHEX_0x456u 1110u, (4, 5, 6), 0x456, 1110, 0, 0xBA9u
#define PPUTLIMPL_UHEX_0x455u 1109u, (4, 5, 5), 0x455, 1109, 0, 0xBAAu
#define PPUTLIMPL_UHEX_0x454u 1108u, (4, 5, 4), 0x454, 1108, 0, 0xBABu
#define PPUTLIMPL_UHEX_0x453u 1107u, (4, 5, 3), 0x453, 1107, 0, 0xBACu
#define PPUTLIMPL_UHEX_0x452u 1106u, (4, 5, 2), 0x452, 1106, 0, 0xBADu
#define PPUTLIMPL_UHEX_0x451u 1105u, (4, 5, 1), 0x451, 1105, 0, 0xBAEu
#define PPUTLIMPL_UHEX_0x450u 1104u, (4, 5, 0), 0x450, 1104, 0, 0xBAFu
#define PPUTLIMPL_UHEX_0x44Fu 1103u, (4, 4, F), 0x44F, 1103, 0, 0xBB0u
#define PPUTLIMPL_UHEX_0x44Eu 1102u, (4, 4, E), 0x44E, 1102, 0, 0xBB1u
#define PPUTLIMPL_UHEX_0x44Du 1101u, (4, 4, D), 0x44D, 1101, 0, 0xBB2u
#define PPUTLIMPL_UHEX_0x44Cu 1100u, (4, 4, C), 0x44C, 1100, 0, 0xBB3u
#define PPUTLIMPL_UHEX_0x44Bu 1099u, (4, 4, B), 0x44B, 1099, 0, 0xBB4u
#define PPUTLIMPL_UHEX_0x44Au 1098u, (4, 4, A), 0x44A, 1098, 0, 0xBB5u
#define PPUTLIMPL_UHEX_0x449u 1097u, (4, 4, 9), 0x449, 1097, 0, 0xBB6u
#define PPUTLIMPL_UHEX_0x448u 1096u, (4, 4, 8), 0x448, 1096, 0, 0xBB7u
#define PPUTLIMPL_UHEX_0x447u 1095u, (4, 4, 7), 0x447, 1095, 0, 0xBB8u
#define PPUTLIMPL_UHEX_0x446u 1094u, (4, 4, 6), 0x446, 1094, 0, 0xBB9u
#define PPUTLIMPL_UHEX_0x445u 1093u, (4, 4, 5), 0x445, 1093, 0, 0xBBAu
#define PPUTLIMPL_UHEX_0x444u 1092u, (4, 4, 4), 0x444, 1092, 0, 0xBBBu
#define PPUTLIMPL_UHEX_0x443u 1091u, (4, 4, 3), 0x443, 1091, 0, 0xBBCu
#define PPUTLIMPL_UHEX_0x442u 1090u, (4, 4, 2), 0x442, 1090, 0, 0xBBDu
#define PPUTLIMPL_UHEX_0x441u 1089u, (4, 4, 1), 0x441, 1089, 0, 0xBBEu
#define PPUTLIMPL_UHEX_0x440u 1088u, (4, 4, 0), 0x440, 1088, 0, 0xBBFu
#define PPUTLIMPL_UHEX_0x43Fu 1087u, (4, 3, F), 0x43F, 1087, 0, 0xBC0u
#define PPUTLIMPL_UHEX_0x43Eu 1086u, (4, 3, E), 0x43E, 1086, 0, 0xBC1u
#define PPUTLIMPL_UHEX_0x43Du 1085u, (4, 3, D), 0x43D, 1085, 0, 0xBC2u
#define PPUTLIMPL_UHEX_0x43Cu 1084u, (4, 3, C), 0x43C, 1084, 0, 0xBC3u
#define PPUTLIMPL_UHEX_0x43Bu 1083u, (4, 3, B), 0x43B, 1083, 0, 0xBC4u
#define PPUTLIMPL_UHEX_0x43Au 1082u, (4, 3, A), 0x43A, 1082, 0, 0xBC5u
#define PPUTLIMPL_UHEX_0x439u 1081u, (4, 3, 9), 0x439, 1081, 0, 0xBC6u
#define PPUTLIMPL_UHEX_0x438u 1080u, (4, 3, 8), 0x438, 1080, 0, 0xBC7u
#define PPUTLIMPL_UHEX_0x437u 1079u, (4, 3, 7), 0x437, 1079, 0, 0xBC8u
#define PPUTLIMPL_UHEX_0x436u 1078u, (4, 3, 6), 0x436, 1078, 0, 0xBC9u
#define PPUTLIMPL_UHEX_0x435u 1077u, (4, 3, 5), 0x435, 1077, 0, 0xBCAu
#define PPUTLIMPL_UHEX_0x434u 1076u, (4, 3, 4), 0x434, 1076, 0, 0xBCBu
#define PPUTLIMPL_UHEX_0x433u 1075u, (4, 3, 3), 0x433, 1075, 0, 0xBCCu
#define PPUTLIMPL_UHEX_0x432u 1074u, (4, 3, 2), 0x432, 1074, 0, 0xBCDu
#define PPUTLIMPL_UHEX_0x431u 1073u, (4, 3, 1), 0x431, 1073, 0, 0xBCEu
#define PPUTLIMPL_UHEX_0x430u 1072u, (4, 3, 0), 0x430, 1072, 0, 0xBCFu
#define PPUTLIMPL_UHEX_0x42Fu 1071u, (4, 2, F), 0x42F, 1071, 0, 0xBD0u
#define PPUTLIMPL_UHEX_0x42Eu 1070u, (4, 2, E), 0x42E, 1070, 0, 0xBD1u
#define PPUTLIMPL_UHEX_0x42Du 1069u, (4, 2, D), 0x42D, 1069, 0, 0xBD2u
#define PPUTLIMPL_UHEX_0x42Cu 1068u, (4, 2, C), 0x42C, 1068, 0, 0xBD3u
#define PPUTLIMPL_UHEX_0x42Bu 1067u, (4, 2, B), 0x42B, 1067, 0, 0xBD4u
#define PPUTLIMPL_UHEX_0x42Au 1066u, (4, 2, A), 0x42A, 1066, 0, 0xBD5u
#define PPUTLIMPL_UHEX_0x429u 1065u, (4, 2, 9), 0x429, 1065, 0, 0xBD6u
#define PPUTLIMPL_UHEX_0x428u 1064u, (4, 2, 8), 0x428, 1064, 0, 0xBD7u
#define PPUTLIMPL_UHEX_0x427u 1063u, (4, 2, 7), 0x427, 1063, 0, 0xBD8u
#define PPUTLIMPL_UHEX_0x426u 1062u, (4, 2, 6), 0x426, 1062, 0, 0xBD9u
#define PPUTLIMPL_UHEX_0x425u 1061u, (4, 2, 5), 0x425, 1061, 0, 0xBDAu
#define PPUTLIMPL_UHEX_0x424u 1060u, (4, 2, 4), 0x424, 1060, 0, 0xBDBu
#define PPUTLIMPL_UHEX_0x423u 1059u, (4, 2, 3), 0x423, 1059, 0, 0xBDCu
#define PPUTLIMPL_UHEX_0x422u 1058u, (4, 2, 2), 0x422, 1058, 0, 0xBDDu
#define PPUTLIMPL_UHEX_0x421u 1057u, (4, 2, 1), 0x421, 1057, 0, 0xBDEu
#define PPUTLIMPL_UHEX_0x420u 1056u, (4, 2, 0), 0x420, 1056, 0, 0xBDFu
#define PPUTLIMPL_UHEX_0x41Fu 1055u, (4, 1, F), 0x41F, 1055, 0, 0xBE0u
#define PPUTLIMPL_UHEX_0x41Eu 1054u, (4, 1, E), 0x41E, 1054, 0, 0xBE1u
#define PPUTLIMPL_UHEX_0x41Du 1053u, (4, 1, D), 0x41D, 1053, 0, 0xBE2u
#define PPUTLIMPL_UHEX_0x41Cu 1052u, (4, 1, C), 0x41C, 1052, 0, 0xBE3u
#define PPUTLIMPL_UHEX_0x41Bu 1051u, (4, 1, B), 0x41B, 1051, 0, 0xBE4u
#define PPUTLIMPL_UHEX_0x41Au 1050u, (4, 1, A), 0x41A, 1050, 0, 0xBE5u
#define PPUTLIMPL_UHEX_0x419u 1049u, (4, 1, 9), 0x419, 1049, 0, 0xBE6u
#define PPUTLIMPL_UHEX_0x418u 1048u, (4, 1, 8), 0x418, 1048, 0, 0xBE7u
#define PPUTLIMPL_UHEX_0x417u 1047u, (4, 1, 7), 0x417, 1047, 0, 0xBE8u
#define PPUTLIMPL_UHEX_0x416u 1046u, (4, 1, 6), 0x416, 1046, 0, 0xBE9u
#define PPUTLIMPL_UHEX_0x415u 1045u, (4, 1, 5), 0x415, 1045, 0, 0xBEAu
#define PPUTLIMPL_UHEX_0x414u 1044u, (4, 1, 4), 0x414, 1044, 0, 0xBEBu
#define PPUTLIMPL_UHEX_0x413u 1043u, (4, 1, 3), 0x413, 1043, 0, 0xBECu
#define PPUTLIMPL_UHEX_0x412u 1042u, (4, 1, 2), 0x412, 1042, 0, 0xBEDu
#define PPUTLIMPL_UHEX_0x411u 1041u, (4, 1, 1), 0x411, 1041, 0, 0xBEEu
#define PPUTLIMPL_UHEX_0x410u 1040u, (4, 1, 0), 0x410, 1040, 0, 0xBEFu
#define PPUTLIMPL_UHEX_0x40Fu 1039u, (4, 0, F), 0x40F, 1039, 0, 0xBF0u
#define PPUTLIMPL_UHEX_0x40Eu 1038u, (4, 0, E), 0x40E, 1038, 0, 0xBF1u
#define PPUTLIMPL_UHEX_0x40Du 1037u, (4, 0, D), 0x40D, 1037, 0, 0xBF2u
#define PPUTLIMPL_UHEX_0x40Cu 1036u, (4, 0, C), 0x40C, 1036, 0, 0xBF3u
#define PPUTLIMPL_UHEX_0x40Bu 1035u, (4, 0, B), 0x40B, 1035, 0, 0xBF4u
#define PPUTLIMPL_UHEX_0x40Au 1034u, (4, 0, A), 0x40A, 1034, 0, 0xBF5u
#define PPUTLIMPL_UHEX_0x409u 1033u, (4, 0, 9), 0x409, 1033, 0, 0xBF6u
#define PPUTLIMPL_UHEX_0x408u 1032u, (4, 0, 8), 0x408, 1032, 0, 0xBF7u
#define PPUTLIMPL_UHEX_0x407u 1031u, (4, 0, 7), 0x407, 1031, 0, 0xBF8u
#define PPUTLIMPL_UHEX_0x406u 1030u, (4, 0, 6), 0x406, 1030, 0, 0xBF9u
#define PPUTLIMPL_UHEX_0x405u 1029u, (4, 0, 5), 0x405, 1029, 0, 0xBFAu
#define PPUTLIMPL_UHEX_0x404u 1028u, (4, 0, 4), 0x404, 1028, 0, 0xBFBu
#define PPUTLIMPL_UHEX_0x403u 1027u, (4, 0, 3), 0x403, 1027, 0, 0xBFCu
#define PPUTLIMPL_UHEX_0x402u 1026u, (4, 0, 2), 0x402, 1026, 0, 0xBFDu
#define PPUTLIMPL_UHEX_0x401u 1025u, (4, 0, 1), 0x401, 1025, 0, 0xBFEu
#define PPUTLIMPL_UHEX_0x400u 1024u, (4, 0, 0), 0x400, 1024, 0, 0xBFFu
#define PPUTLIMPL_UHEX_0x3FFu 1023u, (3, F, F), 0x3FF, 1023, 0, 0xC00u
#define PPUTLIMPL_UHEX_0x3FEu 1022u, (3, F, E), 0x3FE, 1022, 0, 0xC01u
#define PPUTLIMPL_UHEX_0x3FDu 1021u, (3, F, D), 0x3FD, 1021, 0, 0xC02u
#define PPUTLIMPL_UHEX_0x3FCu 1020u, (3, F, C), 0x3FC, 1020, 0, 0xC03u
#define PPUTLIMPL_UHEX_0x3FBu 1019u, (3, F, B), 0x3FB, 1019, 0, 0xC04u
#define PPUTLIMPL_UHEX_0x3FAu 1018u, (3, F, A), 0x3FA, 1018, 0, 0xC05u
#define PPUTLIMPL_UHEX_0x3F9u 1017u, (3, F, 9), 0x3F9, 1017, 0, 0xC06u
#define PPUTLIMPL_UHEX_0x3F8u 1016u, (3, F, 8), 0x3F8, 1016, 0, 0xC07u
#define PPUTLIMPL_UHEX_0x3F7u 1015u, (3, F, 7), 0x3F7, 1015, 0, 0xC08u
#define PPUTLIMPL_UHEX_0x3F6u 1014u, (3, F, 6), 0x3F6, 1014, 0, 0xC09u
#define PPUTLIMPL_UHEX_0x3F5u 1013u, (3, F, 5), 0x3F5, 1013, 0, 0xC0Au
#define PPUTLIMPL_UHEX_0x3F4u 1012u, (3, F, 4), 0x3F4, 1012, 0, 0xC0Bu
#define PPUTLIMPL_UHEX_0x3F3u 1011u, (3, F, 3), 0x3F3, 1011, 0, 0xC0Cu
#define PPUTLIMPL_UHEX_0x3F2u 1010u, (3, F, 2), 0x3F2, 1010, 0, 0xC0Du
#define PPUTLIMPL_UHEX_0x3F1u 1009u, (3, F, 1), 0x3F1, 1009, 0, 0xC0Eu
#define PPUTLIMPL_UHEX_0x3F0u 1008u, (3, F, 0), 0x3F0, 1008, 0, 0xC0Fu
#define PPUTLIMPL_UHEX_0x3EFu 1007u, (3, E, F), 0x3EF, 1007, 0, 0xC10u
#define PPUTLIMPL_UHEX_0x3EEu 1006u, (3, E, E), 0x3EE, 1006, 0, 0xC11u
#define PPUTLIMPL_UHEX_0x3EDu 1005u, (3, E, D), 0x3ED, 1005, 0, 0xC12u
#define PPUTLIMPL_UHEX_0x3ECu 1004u, (3, E, C), 0x3EC, 1004, 0, 0xC13u
#define PPUTLIMPL_UHEX_0x3EBu 1003u, (3, E, B), 0x3EB, 1003, 0, 0xC14u
#define PPUTLIMPL_UHEX_0x3EAu 1002u, (3, E, A), 0x3EA, 1002, 0, 0xC15u
#define PPUTLIMPL_UHEX_0x3E9u 1001u, (3, E, 9), 0x3E9, 1001, 0, 0xC16u
#define PPUTLIMPL_UHEX_0x3E8u 1000u, (3, E, 8), 0x3E8, 1000, 0, 0xC17u
#define PPUTLIMPL_UHEX_0x3E7u 999u, (3, E, 7), 0x3E7, 999, 0, 0xC18u
#define PPUTLIMPL_UHEX_0x3E6u 998u, (3, E, 6), 0x3E6, 998, 0, 0xC19u
#define PPUTLIMPL_UHEX_0x3E5u 997u, (3, E, 5), 0x3E5, 997, 0, 0xC1Au
#define PPUTLIMPL_UHEX_0x3E4u 996u, (3, E, 4), 0x3E4, 996, 0, 0xC1Bu
#define PPUTLIMPL_UHEX_0x3E3u 995u, (3, E, 3), 0x3E3, 995, 0, 0xC1Cu
#define PPUTLIMPL_UHEX_0x3E2u 994u, (3, E, 2), 0x3E2, 994, 0, 0xC1Du
#define PPUTLIMPL_UHEX_0x3E1u 993u, (3, E, 1), 0x3E1, 993, 0, 0xC1Eu
#define PPUTLIMPL_UHEX_0x3E0u 992u, (3, E, 0), 0x3E0, 992, 0, 0xC1Fu
#define PPUTLIMPL_UHEX_0x3DFu 991u, (3, D, F), 0x3DF, 991, 0, 0xC20u
#define PPUTLIMPL_UHEX_0x3DEu 990u, (3, D, E), 0x3DE, 990, 0, 0xC21u
#define PPUTLIMPL_UHEX_0x3DDu 989u, (3, D, D), 0x3DD, 989, 0, 0xC22u
#define PPUTLIMPL_UHEX_0x3DCu 988u, (3, D, C), 0x3DC, 988, 0, 0xC23u
#define PPUTLIMPL_UHEX_0x3DBu 987u, (3, D, B), 0x3DB, 987, 0, 0xC24u
#define PPUTLIMPL_UHEX_0x3DAu 986u, (3, D, A), 0x3DA, 986, 0, 0xC25u
#define PPUTLIMPL_UHEX_0x3D9u 985u, (3, D, 9), 0x3D9, 985, 0, 0xC26u
#define PPUTLIMPL_UHEX_0x3D8u 984u, (3, D, 8), 0x3D8, 984, 0, 0xC27u
#define PPUTLIMPL_UHEX_0x3D7u 983u, (3, D, 7), 0x3D7, 983, 0, 0xC28u
#define PPUTLIMPL_UHEX_0x3D6u 982u, (3, D, 6), 0x3D6, 982, 0, 0xC29u
#define PPUTLIMPL_UHEX_0x3D5u 981u, (3, D, 5), 0x3D5, 981, 0, 0xC2Au
#define PPUTLIMPL_UHEX_0x3D4u 980u, (3, D, 4), 0x3D4, 980, 0, 0xC2Bu
#define PPUTLIMPL_UHEX_0x3D3u 979u, (3, D, 3), 0x3D3, 979, 0, 0xC2Cu
#define PPUTLIMPL_UHEX_0x3D2u 978u, (3, D, 2), 0x3D2, 978, 0, 0xC2Du
#define PPUTLIMPL_UHEX_0x3D1u 977u, (3, D, 1), 0x3D1, 977, 0, 0xC2Eu
#define PPUTLIMPL_UHEX_0x3D0u 976u, (3, D, 0), 0x3D0, 976, 0, 0xC2Fu
#define PPUTLIMPL_UHEX_0x3CFu 975u, (3, C, F), 0x3CF, 975, 0, 0xC30u
#define PPUTLIMPL_UHEX_0x3CEu 974u, (3, C, E), 0x3CE, 974, 0, 0xC31u
#define PPUTLIMPL_UHEX_0x3CDu 973u, (3, C, D), 0x3CD, 973, 0, 0xC32u
#define PPUTLIMPL_UHEX_0x3CCu 972u, (3, C, C), 0x3CC, 972, 0, 0xC33u
#define PPUTLIMPL_UHEX_0x3CBu 971u, (3, C, B), 0x3CB, 971, 0, 0xC34u
#define PPUTLIMPL_UHEX_0x3CAu 970u, (3, C, A), 0x3CA, 970, 0, 0xC35u
#define PPUTLIMPL_UHEX_0x3C9u 969u, (3, C, 9), 0x3C9, 969, 0, 0xC36u
#define PPUTLIMPL_UHEX_0x3C8u 968u, (3, C, 8), 0x3C8, 968, 0, 0xC37u
#define PPUTLIMPL_UHEX_0x3C7u 967u, (3, C, 7), 0x3C7, 967, 0, 0xC38u
#define PPUTLIMPL_UHEX_0x3C6u 966u, (3, C, 6), 0x3C6, 966, 0, 0xC39u
#define PPUTLIMPL_UHEX_0x3C5u 965u, (3, C, 5), 0x3C5, 965, 0, 0xC3Au
#define PPUTLIMPL_UHEX_0x3C4u 964u, (3, C, 4), 0x3C4, 964, 0, 0xC3Bu
#define PPUTLIMPL_UHEX_0x3C3u 963u, (3, C, 3), 0x3C3, 963, 0, 0xC3Cu
#define PPUTLIMPL_UHEX_0x3C2u 962u, (3, C, 2), 0x3C2, 962, 0, 0xC3Du
#define PPUTLIMPL_UHEX_0x3C1u 961u, (3, C, 1), 0x3C1, 961, 0, 0xC3Eu
#define PPUTLIMPL_UHEX_0x3C0u 960u, (3, C, 0), 0x3C0, 960, 0, 0xC3Fu
#define PPUTLIMPL_UHEX_0x3BFu 959u, (3, B, F), 0x3BF, 959, 0, 0xC40u
#define PPUTLIMPL_UHEX_0x3BEu 958u, (3, B, E), 0x3BE, 958, 0, 0xC41u
#define PPUTLIMPL_UHEX_0x3BDu 957u, (3, B, D), 0x3BD, 957, 0, 0xC42u
#define PPUTLIMPL_UHEX_0x3BCu 956u, (3, B, C), 0x3BC, 956, 0, 0xC43u
#define PPUTLIMPL_UHEX_0x3BBu 955u, (3, B, B), 0x3BB, 955, 0, 0xC44u
#define PPUTLIMPL_UHEX_0x3BAu 954u, (3, B, A), 0x3BA, 954, 0, 0xC45u
#define PPUTLIMPL_UHEX_0x3B9u 953u, (3, B, 9), 0x3B9, 953, 0, 0xC46u
#define PPUTLIMPL_UHEX_0x3B8u 952u, (3, B, 8), 0x3B8, 952, 0, 0xC47u
#define PPUTLIMPL_UHEX_0x3B7u 951u, (3, B, 7), 0x3B7, 951, 0, 0xC48u
#define PPUTLIMPL_UHEX_0x3B6u 950u, (3, B, 6), 0x3B6, 950, 0, 0xC49u
#define PPUTLIMPL_UHEX_0x3B5u 949u, (3, B, 5), 0x3B5, 949, 0, 0xC4Au
#define PPUTLIMPL_UHEX_0x3B4u 948u, (3, B, 4), 0x3B4, 948, 0, 0xC4Bu
#define PPUTLIMPL_UHEX_0x3B3u 947u, (3, B, 3), 0x3B3, 947, 0, 0xC4Cu
#define PPUTLIMPL_UHEX_0x3B2u 946u, (3, B, 2), 0x3B2, 946, 0, 0xC4Du
#define PPUTLIMPL_UHEX_0x3B1u 945u, (3, B, 1), 0x3B1, 945, 0, 0xC4Eu
#define PPUTLIMPL_UHEX_0x3B0u 944u, (3, B, 0), 0x3B0, 944, 0, 0xC4Fu
#define PPUTLIMPL_UHEX_0x3AFu 943u, (3, A, F), 0x3AF, 943, 0, 0xC50u
#define PPUTLIMPL_UHEX_0x3AEu 942u, (3, A, E), 0x3AE, 942, 0, 0xC51u
#define PPUTLIMPL_UHEX_0x3ADu 941u, (3, A, D), 0x3AD, 941, 0, 0xC52u
#define PPUTLIMPL_UHEX_0x3ACu 940u, (3, A, C), 0x3AC, 940, 0, 0xC53u
#define PPUTLIMPL_UHEX_0x3ABu 939u, (3, A, B), 0x3AB, 939, 0, 0xC54u
#define PPUTLIMPL_UHEX_0x3AAu 938u, (3, A, A), 0x3AA, 938, 0, 0xC55u
#define PPUTLIMPL_UHEX_0x3A9u 937u, (3, A, 9), 0x3A9, 937, 0, 0xC56u
#define PPUTLIMPL_UHEX_0x3A8u 936u, (3, A, 8), 0x3A8, 936, 0, 0xC57u
#define PPUTLIMPL_UHEX_0x3A7u 935u, (3, A, 7), 0x3A7, 935, 0, 0xC58u
#define PPUTLIMPL_UHEX_0x3A6u 934u, (3, A, 6), 0x3A6, 934, 0, 0xC59u
#define PPUTLIMPL_UHEX_0x3A5u 933u, (3, A, 5), 0x3A5, 933, 0, 0xC5Au
#define PPUTLIMPL_UHEX_0x3A4u 932u, (3, A, 4), 0x3A4, 932, 0, 0xC5Bu
#define PPUTLIMPL_UHEX_0x3A3u 931u, (3, A, 3), 0x3A3, 931, 0, 0xC5Cu
#define PPUTLIMPL_UHEX_0x3A2u 930u, (3, A, 2), 0x3A2, 930, 0, 0xC5Du
#define PPUTLIMPL_UHEX_0x3A1u 929u, (3, A, 1), 0x3A1, 929, 0, 0xC5Eu
#define PPUTLIMPL_UHEX_0x3A0u 928u, (3, A, 0), 0x3A0, 928, 0, 0xC5Fu
#define PPUTLIMPL_UHEX_0x39Fu 927u, (3, 9, F), 0x39F, 927, 0, 0xC60u
#define PPUTLIMPL_UHEX_0x39Eu 926u, (3, 9, E), 0x39E, 926, 0, 0xC61u
#define PPUTLIMPL_UHEX_0x39Du 925u, (3, 9, D), 0x39D, 925, 0, 0xC62u
#define PPUTLIMPL_UHEX_0x39Cu 924u, (3, 9, C), 0x39C, 924, 0, 0xC63u
#define PPUTLIMPL_UHEX_0x39Bu 923u, (3, 9, B), 0x39B, 923, 0, 0xC64u
#define PPUTLIMPL_UHEX_0x39Au 922u, (3, 9, A), 0x39A, 922, 0, 0xC65u
#define PPUTLIMPL_UHEX_0x399u 921u, (3, 9, 9), 0x399, 921, 0, 0xC66u
#define PPUTLIMPL_UHEX_0x398u 920u, (3, 9, 8), 0x398, 920, 0, 0xC67u
#define PPUTLIMPL_UHEX_0x397u 919u, (3, 9, 7), 0x397, 919, 0, 0xC68u
#define PPUTLIMPL_UHEX_0x396u 918u, (3, 9, 6), 0x396, 918, 0, 0xC69u
#define PPUTLIMPL_UHEX_0x395u 917u, (3, 9, 5), 0x395, 917, 0, 0xC6Au
#define PPUTLIMPL_UHEX_0x394u 916u, (3, 9, 4), 0x394, 916, 0, 0xC6Bu
#define PPUTLIMPL_UHEX_0x393u 915u, (3, 9, 3), 0x393, 915, 0, 0xC6Cu
#define PPUTLIMPL_UHEX_0x392u 914u, (3, 9, 2), 0x392, 914, 0, 0xC6Du
#define PPUTLIMPL_UHEX_0x391u 913u, (3, 9, 1), 0x391, 913, 0, 0xC6Eu
#define PPUTLIMPL_UHEX_0x390u 912u, (3, 9, 0), 0x390, 912, 0, 0xC6Fu
#define PPUTLIMPL_UHEX_0x38Fu 911u, (3, 8, F), 0x38F, 911, 0, 0xC70u
#define PPUTLIMPL_UHEX_0x38Eu 910u, (3, 8, E), 0x38E, 910, 0, 0xC71u
#define PPUTLIMPL_UHEX_0x38Du 909u, (3, 8, D), 0x38D, 909, 0, 0xC72u
#define PPUTLIMPL_UHEX_0x38Cu 908u, (3, 8, C), 0x38C, 908, 0, 0xC73u
#define PPUTLIMPL_UHEX_0x38Bu 907u, (3, 8, B), 0x38B, 907, 0, 0xC74u
#define PPUTLIMPL_UHEX_0x38Au 906u, (3, 8, A), 0x38A, 906, 0, 0xC75u
#define PPUTLIMPL_UHEX_0x389u 905u, (3, 8, 9), 0x389, 905, 0, 0xC76u
#define PPUTLIMPL_UHEX_0x388u 904u, (3, 8, 8), 0x388, 904, 0, 0xC77u
#define PPUTLIMPL_UHEX_0x387u 903u, (3, 8, 7), 0x387, 903, 0, 0xC78u
#define PPUTLIMPL_UHEX_0x386u 902u, (3, 8, 6), 0x386, 902, 0, 0xC79u
#define PPUTLIMPL_UHEX_0x385u 901u, (3, 8, 5), 0x385, 901, 0, 0xC7Au
#define PPUTLIMPL_UHEX_0x384u 900u, (3, 8, 4), 0x384, 900, 0, 0xC7Bu
#define PPUTLIMPL_UHEX_0x383u 899u, (3, 8, 3), 0x383, 899, 0, 0xC7Cu
#define PPUTLIMPL_UHEX_0x382u 898u, (3, 8, 2), 0x382, 898, 0, 0xC7Du
#define PPUTLIMPL_UHEX_0x381u 897u, (3, 8, 1), 0x381, 897, 0, 0xC7Eu
#define PPUTLIMPL_UHEX_0x380u 896u, (3, 8, 0), 0x380, 896, 0, 0xC7Fu
#define PPUTLIMPL_UHEX_0x37Fu 895u, (3, 7, F), 0x37F, 895, 0, 0xC80u
#define PPUTLIMPL_UHEX_0x37Eu 894u, (3, 7, E), 0x37E, 894, 0, 0xC81u
#define PPUTLIMPL_UHEX_0x37Du 893u, (3, 7, D), 0x37D, 893, 0, 0xC82u
#define PPUTLIMPL_UHEX_0x37Cu 892u, (3, 7, C), 0x37C, 892, 0, 0xC83u
#define PPUTLIMPL_UHEX_0x37Bu 891u, (3, 7, B), 0x37B, 891, 0, 0xC84u
#define PPUTLIMPL_UHEX_0x37Au 890u, (3, 7, A), 0x37A, 890, 0, 0xC85u
#define PPUTLIMPL_UHEX_0x379u 889u, (3, 7, 9), 0x379, 889, 0, 0xC86u
#define PPUTLIMPL_UHEX_0x378u 888u, (3, 7, 8), 0x378, 888, 0, 0xC87u
#define PPUTLIMPL_UHEX_0x377u 887u, (3, 7, 7), 0x377, 887, 0, 0xC88u
#define PPUTLIMPL_UHEX_0x376u 886u, (3, 7, 6), 0x376, 886, 0, 0xC89u
#define PPUTLIMPL_UHEX_0x375u 885u, (3, 7, 5), 0x375, 885, 0, 0xC8Au
#define PPUTLIMPL_UHEX_0x374u 884u, (3, 7, 4), 0x374, 884, 0, 0xC8Bu
#define PPUTLIMPL_UHEX_0x373u 883u, (3, 7, 3), 0x373, 883, 0, 0xC8Cu
#define PPUTLIMPL_UHEX_0x372u 882u, (3, 7, 2), 0x372, 882, 0, 0xC8Du
#define PPUTLIMPL_UHEX_0x371u 881u, (3, 7, 1), 0x371, 881, 0, 0xC8Eu
#define PPUTLIMPL_UHEX_0x370u 880u, (3, 7, 0), 0x370, 880, 0, 0xC8Fu
#define PPUTLIMPL_UHEX_0x36Fu 879u, (3, 6, F), 0x36F, 879, 0, 0xC90u
#define PPUTLIMPL_UHEX_0x36Eu 878u, (3, 6, E), 0x36E, 878, 0, 0xC91u
#define PPUTLIMPL_UHEX_0x36Du 877u, (3, 6, D), 0x36D, 877, 0, 0xC92u
#define PPUTLIMPL_UHEX_0x36Cu 876u, (3, 6, C), 0x36C, 876, 0, 0xC93u
#define PPUTLIMPL_UHEX_0x36Bu 875u, (3, 6, B), 0x36B, 875, 0, 0xC94u
#define PPUTLIMPL_UHEX_0x36Au 874u, (3, 6, A), 0x36A, 874, 0, 0xC95u
#define PPUTLIMPL_UHEX_0x369u 873u, (3, 6, 9), 0x369, 873, 0, 0xC96u
#define PPUTLIMPL_UHEX_0x368u 872u, (3, 6, 8), 0x368, 872, 0, 0xC97u
#define PPUTLIMPL_UHEX_0x367u 871u, (3, 6, 7), 0x367, 871, 0, 0xC98u
#define PPUTLIMPL_UHEX_0x366u 870u, (3, 6, 6), 0x366, 870, 0, 0xC99u
#define PPUTLIMPL_UHEX_0x365u 869u, (3, 6, 5), 0x365, 869, 0, 0xC9Au
#define PPUTLIMPL_UHEX_0x364u 868u, (3, 6, 4), 0x364, 868, 0, 0xC9Bu
#define PPUTLIMPL_UHEX_0x363u 867u, (3, 6, 3), 0x363, 867, 0, 0xC9Cu
#define PPUTLIMPL_UHEX_0x362u 866u, (3, 6, 2), 0x362, 866, 0, 0xC9Du
#define PPUTLIMPL_UHEX_0x361u 865u, (3, 6, 1), 0x361, 865, 0, 0xC9Eu
#define PPUTLIMPL_UHEX_0x360u 864u, (3, 6, 0), 0x360, 864, 0, 0xC9Fu
#define PPUTLIMPL_UHEX_0x35Fu 863u, (3, 5, F), 0x35F, 863, 0, 0xCA0u
#define PPUTLIMPL_UHEX_0x35Eu 862u, (3, 5, E), 0x35E, 862, 0, 0xCA1u
#define PPUTLIMPL_UHEX_0x35Du 861u, (3, 5, D), 0x35D, 861, 0, 0xCA2u
#define PPUTLIMPL_UHEX_0x35Cu 860u, (3, 5, C), 0x35C, 860, 0, 0xCA3u
#define PPUTLIMPL_UHEX_0x35Bu 859u, (3, 5, B), 0x35B, 859, 0, 0xCA4u
#define PPUTLIMPL_UHEX_0x35Au 858u, (3, 5, A), 0x35A, 858, 0, 0xCA5u
#define PPUTLIMPL_UHEX_0x359u 857u, (3, 5, 9), 0x359, 857, 0, 0xCA6u
#define PPUTLIMPL_UHEX_0x358u 856u, (3, 5, 8), 0x358, 856, 0, 0xCA7u
#define PPUTLIMPL_UHEX_0x357u 855u, (3, 5, 7), 0x357, 855, 0, 0xCA8u
#define PPUTLIMPL_UHEX_0x356u 854u, (3, 5, 6), 0x356, 854, 0, 0xCA9u
#define PPUTLIMPL_UHEX_0x355u 853u, (3, 5, 5), 0x355, 853, 0, 0xCAAu
#define PPUTLIMPL_UHEX_0x354u 852u, (3, 5, 4), 0x354, 852, 0, 0xCABu
#define PPUTLIMPL_UHEX_0x353u 851u, (3, 5, 3), 0x353, 851, 0, 0xCACu
#define PPUTLIMPL_UHEX_0x352u 850u, (3, 5, 2), 0x352, 850, 0, 0xCADu
#define PPUTLIMPL_UHEX_0x351u 849u, (3, 5, 1), 0x351, 849, 0, 0xCAEu
#define PPUTLIMPL_UHEX_0x350u 848u, (3, 5, 0), 0x350, 848, 0, 0xCAFu
#define PPUTLIMPL_UHEX_0x34Fu 847u, (3, 4, F), 0x34F, 847, 0, 0xCB0u
#define PPUTLIMPL_UHEX_0x34Eu 846u, (3, 4, E), 0x34E, 846, 0, 0xCB1u
#define PPUTLIMPL_UHEX_0x34Du 845u, (3, 4, D), 0x34D, 845, 0, 0xCB2u
#define PPUTLIMPL_UHEX_0x34Cu 844u, (3, 4, C), 0x34C, 844, 0, 0xCB3u
#define PPUTLIMPL_UHEX_0x34Bu 843u, (3, 4, B), 0x34B, 843, 0, 0xCB4u
#define PPUTLIMPL_UHEX_0x34Au 842u, (3, 4, A), 0x34A, 842, 0, 0xCB5u
#define PPUTLIMPL_UHEX_0x349u 841u, (3, 4, 9), 0x349, 841, 0, 0xCB6u
#define PPUTLIMPL_UHEX_0x348u 840u, (3, 4, 8), 0x348, 840, 0, 0xCB7u
#define PPUTLIMPL_UHEX_0x347u 839u, (3, 4, 7), 0x347, 839, 0, 0xCB8u
#define PPUTLIMPL_UHEX_0x346u 838u, (3, 4, 6), 0x346, 838, 0, 0xCB9u
#define PPUTLIMPL_UHEX_0x345u 837u, (3, 4, 5), 0x345, 837, 0, 0xCBAu
#define PPUTLIMPL_UHEX_0x344u 836u, (3, 4, 4), 0x344, 836, 0, 0xCBBu
#define PPUTLIMPL_UHEX_0x343u 835u, (3, 4, 3), 0x343, 835, 0, 0xCBCu
#define PPUTLIMPL_UHEX_0x342u 834u, (3, 4, 2), 0x342, 834, 0, 0xCBDu
#define PPUTLIMPL_UHEX_0x341u 833u, (3, 4, 1), 0x341, 833, 0, 0xCBEu
#define PPUTLIMPL_UHEX_0x340u 832u, (3, 4, 0), 0x340, 832, 0, 0xCBFu
#define PPUTLIMPL_UHEX_0x33Fu 831u, (3, 3, F), 0x33F, 831, 0, 0xCC0u
#define PPUTLIMPL_UHEX_0x33Eu 830u, (3, 3, E), 0x33E, 830, 0, 0xCC1u
#define PPUTLIMPL_UHEX_0x33Du 829u, (3, 3, D), 0x33D, 829, 0, 0xCC2u
#define PPUTLIMPL_UHEX_0x33Cu 828u, (3, 3, C), 0x33C, 828, 0, 0xCC3u
#define PPUTLIMPL_UHEX_0x33Bu 827u, (3, 3, B), 0x33B, 827, 0, 0xCC4u
#define PPUTLIMPL_UHEX_0x33Au 826u, (3, 3, A), 0x33A, 826, 0, 0xCC5u
#define PPUTLIMPL_UHEX_0x339u 825u, (3, 3, 9), 0x339, 825, 0, 0xCC6u
#define PPUTLIMPL_UHEX_0x338u 824u, (3, 3, 8), 0x338, 824, 0, 0xCC7u
#define PPUTLIMPL_UHEX_0x337u 823u, (3, 3, 7), 0x337, 823, 0, 0xCC8u
#define PPUTLIMPL_UHEX_0x336u 822u, (3, 3, 6), 0x336, 822, 0, 0xCC9u
#define PPUTLIMPL_UHEX_0x335u 821u, (3, 3, 5), 0x335, 821, 0, 0xCCAu
#define PPUTLIMPL_UHEX_0x334u 820u, (3, 3, 4), 0x334, 820, 0, 0xCCBu
#define PPUTLIMPL_UHEX_0x333u 819u, (3, 3, 3), 0x333, 819, 0, 0xCCCu
#define PPUTLIMPL_UHEX_0x332u 818u, (3, 3, 2), 0x332, 818, 0, 0xCCDu
#define PPUTLIMPL_UHEX_0x331u 817u, (3, 3, 1), 0x331, 817, 0, 0xCCEu
#define PPUTLIMPL_UHEX_0x330u 816u, (3, 3, 0), 0x330, 816, 0, 0xCCFu
#define PPUTLIMPL_UHEX_0x32Fu 815u, (3, 2, F), 0x32F, 815, 0, 0xCD0u
#define PPUTLIMPL_UHEX_0x32Eu 814u, (3, 2, E), 0x32E, 814, 0, 0xCD1u
#define PPUTLIMPL_UHEX_0x32Du 813u, (3, 2, D), 0x32D, 813, 0, 0xCD2u
#define PPUTLIMPL_UHEX_0x32Cu 812u, (3, 2, C), 0x32C, 812, 0, 0xCD3u
#define PPUTLIMPL_UHEX_0x32Bu 811u, (3, 2, B), 0x32B, 811, 0, 0xCD4u
#define PPUTLIMPL_UHEX_0x32Au 810u, (3, 2, A), 0x32A, 810, 0, 0xCD5u
#define PPUTLIMPL_UHEX_0x329u 809u, (3, 2, 9), 0x329, 809, 0, 0xCD6u
#define PPUTLIMPL_UHEX_0x328u 808u, (3, 2, 8), 0x328, 808, 0, 0xCD7u
#define PPUTLIMPL_UHEX_0x327u 807u, (3, 2, 7), 0x327, 807, 0, 0xCD8u
#define PPUTLIMPL_UHEX_0x326u 806u, (3, 2, 6), 0x326, 806, 0, 0xCD9u
#define PPUTLIMPL_UHEX_0x325u 805u, (3, 2, 5), 0x325, 805, 0, 0xCDAu
#define PPUTLIMPL_UHEX_0x324u 804u, (3, 2, 4), 0x324, 804, 0, 0xCDBu
#define PPUTLIMPL_UHEX_0x323u 803u, (3, 2, 3), 0x323, 803, 0, 0xCDCu
#define PPUTLIMPL_UHEX_0x322u 802u, (3, 2, 2), 0x322, 802, 0, 0xCDDu
#define PPUTLIMPL_UHEX_0x321u 801u, (3, 2, 1), 0x321, 801, 0, 0xCDEu
#define PPUTLIMPL_UHEX_0x320u 800u, (3, 2, 0), 0x320, 800, 0, 0xCDFu
#define PPUTLIMPL_UHEX_0x31Fu 799u, (3, 1, F), 0x31F, 799, 0, 0xCE0u
#define PPUTLIMPL_UHEX_0x31Eu 798u, (3, 1, E), 0x31E, 798, 0, 0xCE1u
#define PPUTLIMPL_UHEX_0x31Du 797u, (3, 1, D), 0x31D, 797, 0, 0xCE2u
#define PPUTLIMPL_UHEX_0x31Cu 796u, (3, 1, C), 0x31C, 796, 0, 0xCE3u
#define PPUTLIMPL_UHEX_0x31Bu 795u, (3, 1, B), 0x31B, 795, 0, 0xCE4u
#define PPUTLIMPL_UHEX_0x31Au 794u, (3, 1, A), 0x31A, 794, 0, 0xCE5u
#define PPUTLIMPL_UHEX_0x319u 793u, (3, 1, 9), 0x319, 793, 0, 0xCE6u
#define PPUTLIMPL_UHEX_0x318u 792u, (3, 1, 8), 0x318, 792, 0, 0xCE7u
#define PPUTLIMPL_UHEX_0x317u 791u, (3, 1, 7), 0x317, 791, 0, 0xCE8u
#define PPUTLIMPL_UHEX_0x316u 790u, (3, 1, 6), 0x316, 790, 0, 0xCE9u
#define PPUTLIMPL_UHEX_0x315u 789u, (3, 1, 5), 0x315, 789, 0, 0xCEAu
#define PPUTLIMPL_UHEX_0x314u 788u, (3, 1, 4), 0x314, 788, 0, 0xCEBu
#define PPUTLIMPL_UHEX_0x313u 787u, (3, 1, 3), 0x313, 787, 0, 0xCECu
#define PPUTLIMPL_UHEX_0x312u 786u, (3, 1, 2), 0x312, 786, 0, 0xCEDu
#define PPUTLIMPL_UHEX_0x311u 785u, (3, 1, 1), 0x311, 785, 0, 0xCEEu
#define PPUTLIMPL_UHEX_0x310u 784u, (3, 1, 0), 0x310, 784, 0, 0xCEFu
#define PPUTLIMPL_UHEX_0x30Fu 783u, (3, 0, F), 0x30F, 783, 0, 0xCF0u
#define PPUTLIMPL_UHEX_0x30Eu 782u, (3, 0, E), 0x30E, 782, 0, 0xCF1u
#define PPUTLIMPL_UHEX_0x30Du 781u, (3, 0, D), 0x30D, 781, 0, 0xCF2u
#define PPUTLIMPL_UHEX_0x30Cu 780u, (3, 0, C), 0x30C, 780, 0, 0xCF3u
#define PPUTLIMPL_UHEX_0x30Bu 779u, (3, 0, B), 0x30B, 779, 0, 0xCF4u
#define PPUTLIMPL_UHEX_0x30Au 778u, (3, 0, A), 0x30A, 778, 0, 0xCF5u
#define PPUTLIMPL_UHEX_0x309u 777u, (3, 0, 9), 0x309, 777, 0, 0xCF6u
#define PPUTLIMPL_UHEX_0x308u 776u, (3, 0, 8), 0x308, 776, 0, 0xCF7u
#define PPUTLIMPL_UHEX_0x307u 775u, (3, 0, 7), 0x307, 775, 0, 0xCF8u
#define PPUTLIMPL_UHEX_0x306u 774u, (3, 0, 6), 0x306, 774, 0, 0xCF9u
#define PPUTLIMPL_UHEX_0x305u 773u, (3, 0, 5), 0x305, 773, 0, 0xCFAu
#define PPUTLIMPL_UHEX_0x304u 772u, (3, 0, 4), 0x304, 772, 0, 0xCFBu
#define PPUTLIMPL_UHEX_0x303u 771u, (3, 0, 3), 0x303, 771, 0, 0xCFCu
#define PPUTLIMPL_UHEX_0x302u 770u, (3, 0, 2), 0x302, 770, 0, 0xCFDu
#define PPUTLIMPL_UHEX_0x301u 769u, (3, 0, 1), 0x301, 769, 0, 0xCFEu
#define PPUTLIMPL_UHEX_0x300u 768u, (3, 0, 0), 0x300, 768, 0, 0xCFFu
#define PPUTLIMPL_UHEX_0x2FFu 767u, (2, F, F), 0x2FF, 767, 0, 0xD00u
#define PPUTLIMPL_UHEX_0x2FEu 766u, (2, F, E), 0x2FE, 766, 0, 0xD01u
#define PPUTLIMPL_UHEX_0x2FDu 765u, (2, F, D), 0x2FD, 765, 0, 0xD02u
#define PPUTLIMPL_UHEX_0x2FCu 764u, (2, F, C), 0x2FC, 764, 0, 0xD03u
#define PPUTLIMPL_UHEX_0x2FBu 763u, (2, F, B), 0x2FB, 763, 0, 0xD04u
#define PPUTLIMPL_UHEX_0x2FAu 762u, (2, F, A), 0x2FA, 762, 0, 0xD05u
#define PPUTLIMPL_UHEX_0x2F9u 761u, (2, F, 9), 0x2F9, 761, 0, 0xD06u
#define PPUTLIMPL_UHEX_0x2F8u 760u, (2, F, 8), 0x2F8, 760, 0, 0xD07u
#define PPUTLIMPL_UHEX_0x2F7u 759u, (2, F, 7), 0x2F7, 759, 0, 0xD08u
#define PPUTLIMPL_UHEX_0x2F6u 758u, (2, F, 6), 0x2F6, 758, 0, 0xD09u
#define PPUTLIMPL_UHEX_0x2F5u 757u, (2, F, 5), 0x2F5, 757, 0, 0xD0Au
#define PPUTLIMPL_UHEX_0x2F4u 756u, (2, F, 4), 0x2F4, 756, 0, 0xD0Bu
#define PPUTLIMPL_UHEX_0x2F3u 755u, (2, F, 3), 0x2F3, 755, 0, 0xD0Cu
#define PPUTLIMPL_UHEX_0x2F2u 754u, (2, F, 2), 0x2F2, 754, 0, 0xD0Du
#define PPUTLIMPL_UHEX_0x2F1u 753u, (2, F, 1), 0x2F1, 753, 0, 0xD0Eu
#define PPUTLIMPL_UHEX_0x2F0u 752u, (2, F, 0), 0x2F0, 752, 0, 0xD0Fu
#define PPUTLIMPL_UHEX_0x2EFu 751u, (2, E, F), 0x2EF, 751, 0, 0xD10u
#define PPUTLIMPL_UHEX_0x2EEu 750u, (2, E, E), 0x2EE, 750, 0, 0xD11u
#define PPUTLIMPL_UHEX_0x2EDu 749u, (2, E, D), 0x2ED, 749, 0, 0xD12u
#define PPUTLIMPL_UHEX_0x2ECu 748u, (2, E, C), 0x2EC, 748, 0, 0xD13u
#define PPUTLIMPL_UHEX_0x2EBu 747u, (2, E, B), 0x2EB, 747, 0, 0xD14u
#define PPUTLIMPL_UHEX_0x2EAu 746u, (2, E, A), 0x2EA, 746, 0, 0xD15u
#define PPUTLIMPL_UHEX_0x2E9u 745u, (2, E, 9), 0x2E9, 745, 0, 0xD16u
#define PPUTLIMPL_UHEX_0x2E8u 744u, (2, E, 8), 0x2E8, 744, 0, 0xD17u
#define PPUTLIMPL_UHEX_0x2E7u 743u, (2, E, 7), 0x2E7, 743, 0, 0xD18u
#define PPUTLIMPL_UHEX_0x2E6u 742u, (2, E, 6), 0x2E6, 742, 0, 0xD19u
#define PPUTLIMPL_UHEX_0x2E5u 741u, (2, E, 5), 0x2E5, 741, 0, 0xD1Au
#define PPUTLIMPL_UHEX_0x2E4u 740u, (2, E, 4), 0x2E4, 740, 0, 0xD1Bu
#define PPUTLIMPL_UHEX_0x2E3u 739u, (2, E, 3), 0x2E3, 739, 0, 0xD1Cu
#define PPUTLIMPL_UHEX_0x2E2u 738u, (2, E, 2), 0x2E2, 738, 0, 0xD1Du
#define PPUTLIMPL_UHEX_0x2E1u 737u, (2, E, 1), 0x2E1, 737, 0, 0xD1Eu
#define PPUTLIMPL_UHEX_0x2E0u 736u, (2, E, 0), 0x2E0, 736, 0, 0xD1Fu
#define PPUTLIMPL_UHEX_0x2DFu 735u, (2, D, F), 0x2DF, 735, 0, 0xD20u
#define PPUTLIMPL_UHEX_0x2DEu 734u, (2, D, E), 0x2DE, 734, 0, 0xD21u
#define PPUTLIMPL_UHEX_0x2DDu 733u, (2, D, D), 0x2DD, 733, 0, 0xD22u
#define PPUTLIMPL_UHEX_0x2DCu 732u, (2, D, C), 0x2DC, 732, 0, 0xD23u
#define PPUTLIMPL_UHEX_0x2DBu 731u, (2, D, B), 0x2DB, 731, 0, 0xD24u
#define PPUTLIMPL_UHEX_0x2DAu 730u, (2, D, A), 0x2DA, 730, 0, 0xD25u
#define PPUTLIMPL_UHEX_0x2D9u 729u, (2, D, 9), 0x2D9, 729, 0, 0xD26u
#define PPUTLIMPL_UHEX_0x2D8u 728u, (2, D, 8), 0x2D8, 728, 0, 0xD27u
#define PPUTLIMPL_UHEX_0x2D7u 727u, (2, D, 7), 0x2D7, 727, 0, 0xD28u
#define PPUTLIMPL_UHEX_0x2D6u 726u, (2, D, 6), 0x2D6, 726, 0, 0xD29u
#define PPUTLIMPL_UHEX_0x2D5u 725u, (2, D, 5), 0x2D5, 725, 0, 0xD2Au
#define PPUTLIMPL_UHEX_0x2D4u 724u, (2, D, 4), 0x2D4, 724, 0, 0xD2Bu
#define PPUTLIMPL_UHEX_0x2D3u 723u, (2, D, 3), 0x2D3, 723, 0, 0xD2Cu
#define PPUTLIMPL_UHEX_0x2D2u 722u, (2, D, 2), 0x2D2, 722, 0, 0xD2Du
#define PPUTLIMPL_UHEX_0x2D1u 721u, (2, D, 1), 0x2D1, 721, 0, 0xD2Eu
#define PPUTLIMPL_UHEX_0x2D0u 720u, (2, D, 0), 0x2D0, 720, 0, 0xD2Fu
#define PPUTLIMPL_UHEX_0x2CFu 719u, (2, C, F), 0x2CF, 719, 0, 0xD30u
#define PPUTLIMPL_UHEX_0x2CEu 718u, (2, C, E), 0x2CE, 718, 0, 0xD31u
#define PPUTLIMPL_UHEX_0x2CDu 717u, (2, C, D), 0x2CD, 717, 0, 0xD32u
#define PPUTLIMPL_UHEX_0x2CCu 716u, (2, C, C), 0x2CC, 716, 0, 0xD33u
#define PPUTLIMPL_UHEX_0x2CBu 715u, (2, C, B), 0x2CB, 715, 0, 0xD34u
#define PPUTLIMPL_UHEX_0x2CAu 714u, (2, C, A), 0x2CA, 714, 0, 0xD35u
#define PPUTLIMPL_UHEX_0x2C9u 713u, (2, C, 9), 0x2C9, 713, 0, 0xD36u
#define PPUTLIMPL_UHEX_0x2C8u 712u, (2, C, 8), 0x2C8, 712, 0, 0xD37u
#define PPUTLIMPL_UHEX_0x2C7u 711u, (2, C, 7), 0x2C7, 711, 0, 0xD38u
#define PPUTLIMPL_UHEX_0x2C6u 710u, (2, C, 6), 0x2C6, 710, 0, 0xD39u
#define PPUTLIMPL_UHEX_0x2C5u 709u, (2, C, 5), 0x2C5, 709, 0, 0xD3Au
#define PPUTLIMPL_UHEX_0x2C4u 708u, (2, C, 4), 0x2C4, 708, 0, 0xD3Bu
#define PPUTLIMPL_UHEX_0x2C3u 707u, (2, C, 3), 0x2C3, 707, 0, 0xD3Cu
#define PPUTLIMPL_UHEX_0x2C2u 706u, (2, C, 2), 0x2C2, 706, 0, 0xD3Du
#define PPUTLIMPL_UHEX_0x2C1u 705u, (2, C, 1), 0x2C1, 705, 0, 0xD3Eu
#define PPUTLIMPL_UHEX_0x2C0u 704u, (2, C, 0), 0x2C0, 704, 0, 0xD3Fu
#define PPUTLIMPL_UHEX_0x2BFu 703u, (2, B, F), 0x2BF, 703, 0, 0xD40u
#define PPUTLIMPL_UHEX_0x2BEu 702u, (2, B, E), 0x2BE, 702, 0, 0xD41u
#define PPUTLIMPL_UHEX_0x2BDu 701u, (2, B, D), 0x2BD, 701, 0, 0xD42u
#define PPUTLIMPL_UHEX_0x2BCu 700u, (2, B, C), 0x2BC, 700, 0, 0xD43u
#define PPUTLIMPL_UHEX_0x2BBu 699u, (2, B, B), 0x2BB, 699, 0, 0xD44u
#define PPUTLIMPL_UHEX_0x2BAu 698u, (2, B, A), 0x2BA, 698, 0, 0xD45u
#define PPUTLIMPL_UHEX_0x2B9u 697u, (2, B, 9), 0x2B9, 697, 0, 0xD46u
#define PPUTLIMPL_UHEX_0x2B8u 696u, (2, B, 8), 0x2B8, 696, 0, 0xD47u
#define PPUTLIMPL_UHEX_0x2B7u 695u, (2, B, 7), 0x2B7, 695, 0, 0xD48u
#define PPUTLIMPL_UHEX_0x2B6u 694u, (2, B, 6), 0x2B6, 694, 0, 0xD49u
#define PPUTLIMPL_UHEX_0x2B5u 693u, (2, B, 5), 0x2B5, 693, 0, 0xD4Au
#define PPUTLIMPL_UHEX_0x2B4u 692u, (2, B, 4), 0x2B4, 692, 0, 0xD4Bu
#define PPUTLIMPL_UHEX_0x2B3u 691u, (2, B, 3), 0x2B3, 691, 0, 0xD4Cu
#define PPUTLIMPL_UHEX_0x2B2u 690u, (2, B, 2), 0x2B2, 690, 0, 0xD4Du
#define PPUTLIMPL_UHEX_0x2B1u 689u, (2, B, 1), 0x2B1, 689, 0, 0xD4Eu
#define PPUTLIMPL_UHEX_0x2B0u 688u, (2, B, 0), 0x2B0, 688, 0, 0xD4Fu
#define PPUTLIMPL_UHEX_0x2AFu 687u, (2, A, F), 0x2AF, 687, 0, 0xD50u
#define PPUTLIMPL_UHEX_0x2AEu 686u, (2, A, E), 0x2AE, 686, 0, 0xD51u
#define PPUTLIMPL_UHEX_0x2ADu 685u, (2, A, D), 0x2AD, 685, 0, 0xD52u
#define PPUTLIMPL_UHEX_0x2ACu 684u, (2, A, C), 0x2AC, 684, 0, 0xD53u
#define PPUTLIMPL_UHEX_0x2ABu 683u, (2, A, B), 0x2AB, 683, 0, 0xD54u
#define PPUTLIMPL_UHEX_0x2AAu 682u, (2, A, A), 0x2AA, 682, 0, 0xD55u
#define PPUTLIMPL_UHEX_0x2A9u 681u, (2, A, 9), 0x2A9, 681, 0, 0xD56u
#define PPUTLIMPL_UHEX_0x2A8u 680u, (2, A, 8), 0x2A8, 680, 0, 0xD57u
#define PPUTLIMPL_UHEX_0x2A7u 679u, (2, A, 7), 0x2A7, 679, 0, 0xD58u
#define PPUTLIMPL_UHEX_0x2A6u 678u, (2, A, 6), 0x2A6, 678, 0, 0xD59u
#define PPUTLIMPL_UHEX_0x2A5u 677u, (2, A, 5), 0x2A5, 677, 0, 0xD5Au
#define PPUTLIMPL_UHEX_0x2A4u 676u, (2, A, 4), 0x2A4, 676, 0, 0xD5Bu
#define PPUTLIMPL_UHEX_0x2A3u 675u, (2, A, 3), 0x2A3, 675, 0, 0xD5Cu
#define PPUTLIMPL_UHEX_0x2A2u 674u, (2, A, 2), 0x2A2, 674, 0, 0xD5Du
#define PPUTLIMPL_UHEX_0x2A1u 673u, (2, A, 1), 0x2A1, 673, 0, 0xD5Eu
#define PPUTLIMPL_UHEX_0x2A0u 672u, (2, A, 0), 0x2A0, 672, 0, 0xD5Fu
#define PPUTLIMPL_UHEX_0x29Fu 671u, (2, 9, F), 0x29F, 671, 0, 0xD60u
#define PPUTLIMPL_UHEX_0x29Eu 670u, (2, 9, E), 0x29E, 670, 0, 0xD61u
#define PPUTLIMPL_UHEX_0x29Du 669u, (2, 9, D), 0x29D, 669, 0, 0xD62u
#define PPUTLIMPL_UHEX_0x29Cu 668u, (2, 9, C), 0x29C, 668, 0, 0xD63u
#define PPUTLIMPL_UHEX_0x29Bu 667u, (2, 9, B), 0x29B, 667, 0, 0xD64u
#define PPUTLIMPL_UHEX_0x29Au 666u, (2, 9, A), 0x29A, 666, 0, 0xD65u
#define PPUTLIMPL_UHEX_0x299u 665u, (2, 9, 9), 0x299, 665, 0, 0xD66u
#define PPUTLIMPL_UHEX_0x298u 664u, (2, 9, 8), 0x298, 664, 0, 0xD67u
#define PPUTLIMPL_UHEX_0x297u 663u, (2, 9, 7), 0x297, 663, 0, 0xD68u
#define PPUTLIMPL_UHEX_0x296u 662u, (2, 9, 6), 0x296, 662, 0, 0xD69u
#define PPUTLIMPL_UHEX_0x295u 661u, (2, 9, 5), 0x295, 661, 0, 0xD6Au
#define PPUTLIMPL_UHEX_0x294u 660u, (2, 9, 4), 0x294, 660, 0, 0xD6Bu
#define PPUTLIMPL_UHEX_0x293u 659u, (2, 9, 3), 0x293, 659, 0, 0xD6Cu
#define PPUTLIMPL_UHEX_0x292u 658u, (2, 9, 2), 0x292, 658, 0, 0xD6Du
#define PPUTLIMPL_UHEX_0x291u 657u, (2, 9, 1), 0x291, 657, 0, 0xD6Eu
#define PPUTLIMPL_UHEX_0x290u 656u, (2, 9, 0), 0x290, 656, 0, 0xD6Fu
#define PPUTLIMPL_UHEX_0x28Fu 655u, (2, 8, F), 0x28F, 655, 0, 0xD70u
#define PPUTLIMPL_UHEX_0x28Eu 654u, (2, 8, E), 0x28E, 654, 0, 0xD71u
#define PPUTLIMPL_UHEX_0x28Du 653u, (2, 8, D), 0x28D, 653, 0, 0xD72u
#define PPUTLIMPL_UHEX_0x28Cu 652u, (2, 8, C), 0x28C, 652, 0, 0xD73u
#define PPUTLIMPL_UHEX_0x28Bu 651u, (2, 8, B), 0x28B, 651, 0, 0xD74u
#define PPUTLIMPL_UHEX_0x28Au 650u, (2, 8, A), 0x28A, 650, 0, 0xD75u
#define PPUTLIMPL_UHEX_0x289u 649u, (2, 8, 9), 0x289, 649, 0, 0xD76u
#define PPUTLIMPL_UHEX_0x288u 648u, (2, 8, 8), 0x288, 648, 0, 0xD77u
#define PPUTLIMPL_UHEX_0x287u 647u, (2, 8, 7), 0x287, 647, 0, 0xD78u
#define PPUTLIMPL_UHEX_0x286u 646u, (2, 8, 6), 0x286, 646, 0, 0xD79u
#define PPUTLIMPL_UHEX_0x285u 645u, (2, 8, 5), 0x285, 645, 0, 0xD7Au
#define PPUTLIMPL_UHEX_0x284u 644u, (2, 8, 4), 0x284, 644, 0, 0xD7Bu
#define PPUTLIMPL_UHEX_0x283u 643u, (2, 8, 3), 0x283, 643, 0, 0xD7Cu
#define PPUTLIMPL_UHEX_0x282u 642u, (2, 8, 2), 0x282, 642, 0, 0xD7Du
#define PPUTLIMPL_UHEX_0x281u 641u, (2, 8, 1), 0x281, 641, 0, 0xD7Eu
#define PPUTLIMPL_UHEX_0x280u 640u, (2, 8, 0), 0x280, 640, 0, 0xD7Fu
#define PPUTLIMPL_UHEX_0x27Fu 639u, (2, 7, F), 0x27F, 639, 0, 0xD80u
#define PPUTLIMPL_UHEX_0x27Eu 638u, (2, 7, E), 0x27E, 638, 0, 0xD81u
#define PPUTLIMPL_UHEX_0x27Du 637u, (2, 7, D), 0x27D, 637, 0, 0xD82u
#define PPUTLIMPL_UHEX_0x27Cu 636u, (2, 7, C), 0x27C, 636, 0, 0xD83u
#define PPUTLIMPL_UHEX_0x27Bu 635u, (2, 7, B), 0x27B, 635, 0, 0xD84u
#define PPUTLIMPL_UHEX_0x27Au 634u, (2, 7, A), 0x27A, 634, 0, 0xD85u
#define PPUTLIMPL_UHEX_0x279u 633u, (2, 7, 9), 0x279, 633, 0, 0xD86u
#define PPUTLIMPL_UHEX_0x278u 632u, (2, 7, 8), 0x278, 632, 0, 0xD87u
#define PPUTLIMPL_UHEX_0x277u 631u, (2, 7, 7), 0x277, 631, 0, 0xD88u
#define PPUTLIMPL_UHEX_0x276u 630u, (2, 7, 6), 0x276, 630, 0, 0xD89u
#define PPUTLIMPL_UHEX_0x275u 629u, (2, 7, 5), 0x275, 629, 0, 0xD8Au
#define PPUTLIMPL_UHEX_0x274u 628u, (2, 7, 4), 0x274, 628, 0, 0xD8Bu
#define PPUTLIMPL_UHEX_0x273u 627u, (2, 7, 3), 0x273, 627, 0, 0xD8Cu
#define PPUTLIMPL_UHEX_0x272u 626u, (2, 7, 2), 0x272, 626, 0, 0xD8Du
#define PPUTLIMPL_UHEX_0x271u 625u, (2, 7, 1), 0x271, 625, 0, 0xD8Eu
#define PPUTLIMPL_UHEX_0x270u 624u, (2, 7, 0), 0x270, 624, 0, 0xD8Fu
#define PPUTLIMPL_UHEX_0x26Fu 623u, (2, 6, F), 0x26F, 623, 0, 0xD90u
#define PPUTLIMPL_UHEX_0x26Eu 622u, (2, 6, E), 0x26E, 622, 0, 0xD91u
#define PPUTLIMPL_UHEX_0x26Du 621u, (2, 6, D), 0x26D, 621, 0, 0xD92u
#define PPUTLIMPL_UHEX_0x26Cu 620u, (2, 6, C), 0x26C, 620, 0, 0xD93u
#define PPUTLIMPL_UHEX_0x26Bu 619u, (2, 6, B), 0x26B, 619, 0, 0xD94u
#define PPUTLIMPL_UHEX_0x26Au 618u, (2, 6, A), 0x26A, 618, 0, 0xD95u
#define PPUTLIMPL_UHEX_0x269u 617u, (2, 6, 9), 0x269, 617, 0, 0xD96u
#define PPUTLIMPL_UHEX_0x268u 616u, (2, 6, 8), 0x268, 616, 0, 0xD97u
#define PPUTLIMPL_UHEX_0x267u 615u, (2, 6, 7), 0x267, 615, 0, 0xD98u
#define PPUTLIMPL_UHEX_0x266u 614u, (2, 6, 6), 0x266, 614, 0, 0xD99u
#define PPUTLIMPL_UHEX_0x265u 613u, (2, 6, 5), 0x265, 613, 0, 0xD9Au
#define PPUTLIMPL_UHEX_0x264u 612u, (2, 6, 4), 0x264, 612, 0, 0xD9Bu
#define PPUTLIMPL_UHEX_0x263u 611u, (2, 6, 3), 0x263, 611, 0, 0xD9Cu
#define PPUTLIMPL_UHEX_0x262u 610u, (2, 6, 2), 0x262, 610, 0, 0xD9Du
#define PPUTLIMPL_UHEX_0x261u 609u, (2, 6, 1), 0x261, 609, 0, 0xD9Eu
#define PPUTLIMPL_UHEX_0x260u 608u, (2, 6, 0), 0x260, 608, 0, 0xD9Fu
#define PPUTLIMPL_UHEX_0x25Fu 607u, (2, 5, F), 0x25F, 607, 0, 0xDA0u
#define PPUTLIMPL_UHEX_0x25Eu 606u, (2, 5, E), 0x25E, 606, 0, 0xDA1u
#define PPUTLIMPL_UHEX_0x25Du 605u, (2, 5, D), 0x25D, 605, 0, 0xDA2u
#define PPUTLIMPL_UHEX_0x25Cu 604u, (2, 5, C), 0x25C, 604, 0, 0xDA3u
#define PPUTLIMPL_UHEX_0x25Bu 603u, (2, 5, B), 0x25B, 603, 0, 0xDA4u
#define PPUTLIMPL_UHEX_0x25Au 602u, (2, 5, A), 0x25A, 602, 0, 0xDA5u
#define PPUTLIMPL_UHEX_0x259u 601u, (2, 5, 9), 0x259, 601, 0, 0xDA6u
#define PPUTLIMPL_UHEX_0x258u 600u, (2, 5, 8), 0x258, 600, 0, 0xDA7u
#define PPUTLIMPL_UHEX_0x257u 599u, (2, 5, 7), 0x257, 599, 0, 0xDA8u
#define PPUTLIMPL_UHEX_0x256u 598u, (2, 5, 6), 0x256, 598, 0, 0xDA9u
#define PPUTLIMPL_UHEX_0x255u 597u, (2, 5, 5), 0x255, 597, 0, 0xDAAu
#define PPUTLIMPL_UHEX_0x254u 596u, (2, 5, 4), 0x254, 596, 0, 0xDABu
#define PPUTLIMPL_UHEX_0x253u 595u, (2, 5, 3), 0x253, 595, 0, 0xDACu
#define PPUTLIMPL_UHEX_0x252u 594u, (2, 5, 2), 0x252, 594, 0, 0xDADu
#define PPUTLIMPL_UHEX_0x251u 593u, (2, 5, 1), 0x251, 593, 0, 0xDAEu
#define PPUTLIMPL_UHEX_0x250u 592u, (2, 5, 0), 0x250, 592, 0, 0xDAFu
#define PPUTLIMPL_UHEX_0x24Fu 591u, (2, 4, F), 0x24F, 591, 0, 0xDB0u
#define PPUTLIMPL_UHEX_0x24Eu 590u, (2, 4, E), 0x24E, 590, 0, 0xDB1u
#define PPUTLIMPL_UHEX_0x24Du 589u, (2, 4, D), 0x24D, 589, 0, 0xDB2u
#define PPUTLIMPL_UHEX_0x24Cu 588u, (2, 4, C), 0x24C, 588, 0, 0xDB3u
#define PPUTLIMPL_UHEX_0x24Bu 587u, (2, 4, B), 0x24B, 587, 0, 0xDB4u
#define PPUTLIMPL_UHEX_0x24Au 586u, (2, 4, A), 0x24A, 586, 0, 0xDB5u
#define PPUTLIMPL_UHEX_0x249u 585u, (2, 4, 9), 0x249, 585, 0, 0xDB6u
#define PPUTLIMPL_UHEX_0x248u 584u, (2, 4, 8), 0x248, 584, 0, 0xDB7u
#define PPUTLIMPL_UHEX_0x247u 583u, (2, 4, 7), 0x247, 583, 0, 0xDB8u
#define PPUTLIMPL_UHEX_0x246u 582u, (2, 4, 6), 0x246, 582, 0, 0xDB9u
#define PPUTLIMPL_UHEX_0x245u 581u, (2, 4, 5), 0x245, 581, 0, 0xDBAu
#define PPUTLIMPL_UHEX_0x244u 580u, (2, 4, 4), 0x244, 580, 0, 0xDBBu
#define PPUTLIMPL_UHEX_0x243u 579u, (2, 4, 3), 0x243, 579, 0, 0xDBCu
#define PPUTLIMPL_UHEX_0x242u 578u, (2, 4, 2), 0x242, 578, 0, 0xDBDu
#define PPUTLIMPL_UHEX_0x241u 577u, (2, 4, 1), 0x241, 577, 0, 0xDBEu
#define PPUTLIMPL_UHEX_0x240u 576u, (2, 4, 0), 0x240, 576, 0, 0xDBFu
#define PPUTLIMPL_UHEX_0x23Fu 575u, (2, 3, F), 0x23F, 575, 0, 0xDC0u
#define PPUTLIMPL_UHEX_0x23Eu 574u, (2, 3, E), 0x23E, 574, 0, 0xDC1u
#define PPUTLIMPL_UHEX_0x23Du 573u, (2, 3, D), 0x23D, 573, 0, 0xDC2u
#define PPUTLIMPL_UHEX_0x23Cu 572u, (2, 3, C), 0x23C, 572, 0, 0xDC3u
#define PPUTLIMPL_UHEX_0x23Bu 571u, (2, 3, B), 0x23B, 571, 0, 0xDC4u
#define PPUTLIMPL_UHEX_0x23Au 570u, (2, 3, A), 0x23A, 570, 0, 0xDC5u
#define PPUTLIMPL_UHEX_0x239u 569u, (2, 3, 9), 0x239, 569, 0, 0xDC6u
#define PPUTLIMPL_UHEX_0x238u 568u, (2, 3, 8), 0x238, 568, 0, 0xDC7u
#define PPUTLIMPL_UHEX_0x237u 567u, (2, 3, 7), 0x237, 567, 0, 0xDC8u
#define PPUTLIMPL_UHEX_0x236u 566u, (2, 3, 6), 0x236, 566, 0, 0xDC9u
#define PPUTLIMPL_UHEX_0x235u 565u, (2, 3, 5), 0x235, 565, 0, 0xDCAu
#define PPUTLIMPL_UHEX_0x234u 564u, (2, 3, 4), 0x234, 564, 0, 0xDCBu
#define PPUTLIMPL_UHEX_0x233u 563u, (2, 3, 3), 0x233, 563, 0, 0xDCCu
#define PPUTLIMPL_UHEX_0x232u 562u, (2, 3, 2), 0x232, 562, 0, 0xDCDu
#define PPUTLIMPL_UHEX_0x231u 561u, (2, 3, 1), 0x231, 561, 0, 0xDCEu
#define PPUTLIMPL_UHEX_0x230u 560u, (2, 3, 0), 0x230, 560, 0, 0xDCFu
#define PPUTLIMPL_UHEX_0x22Fu 559u, (2, 2, F), 0x22F, 559, 0, 0xDD0u
#define PPUTLIMPL_UHEX_0x22Eu 558u, (2, 2, E), 0x22E, 558, 0, 0xDD1u
#define PPUTLIMPL_UHEX_0x22Du 557u, (2, 2, D), 0x22D, 557, 0, 0xDD2u
#define PPUTLIMPL_UHEX_0x22Cu 556u, (2, 2, C), 0x22C, 556, 0, 0xDD3u
#define PPUTLIMPL_UHEX_0x22Bu 555u, (2, 2, B), 0x22B, 555, 0, 0xDD4u
#define PPUTLIMPL_UHEX_0x22Au 554u, (2, 2, A), 0x22A, 554, 0, 0xDD5u
#define PPUTLIMPL_UHEX_0x229u 553u, (2, 2, 9), 0x229, 553, 0, 0xDD6u
#define PPUTLIMPL_UHEX_0x228u 552u, (2, 2, 8), 0x228, 552, 0, 0xDD7u
#define PPUTLIMPL_UHEX_0x227u 551u, (2, 2, 7), 0x227, 551, 0, 0xDD8u
#define PPUTLIMPL_UHEX_0x226u 550u, (2, 2, 6), 0x226, 550, 0, 0xDD9u
#define PPUTLIMPL_UHEX_0x225u 549u, (2, 2, 5), 0x225, 549, 0, 0xDDAu
#define PPUTLIMPL_UHEX_0x224u 548u, (2, 2, 4), 0x224, 548, 0, 0xDDBu
#define PPUTLIMPL_UHEX_0x223u 547u, (2, 2, 3), 0x223, 547, 0, 0xDDCu
#define PPUTLIMPL_UHEX_0x222u 546u, (2, 2, 2), 0x222, 546, 0, 0xDDDu
#define PPUTLIMPL_UHEX_0x221u 545u, (2, 2, 1), 0x221, 545, 0, 0xDDEu
#define PPUTLIMPL_UHEX_0x220u 544u, (2, 2, 0), 0x220, 544, 0, 0xDDFu
#define PPUTLIMPL_UHEX_0x21Fu 543u, (2, 1, F), 0x21F, 543, 0, 0xDE0u
#define PPUTLIMPL_UHEX_0x21Eu 542u, (2, 1, E), 0x21E, 542, 0, 0xDE1u
#define PPUTLIMPL_UHEX_0x21Du 541u, (2, 1, D), 0x21D, 541, 0, 0xDE2u
#define PPUTLIMPL_UHEX_0x21Cu 540u, (2, 1, C), 0x21C, 540, 0, 0xDE3u
#define PPUTLIMPL_UHEX_0x21Bu 539u, (2, 1, B), 0x21B, 539, 0, 0xDE4u
#define PPUTLIMPL_UHEX_0x21Au 538u, (2, 1, A), 0x21A, 538, 0, 0xDE5u
#define PPUTLIMPL_UHEX_0x219u 537u, (2, 1, 9), 0x219, 537, 0, 0xDE6u
#define PPUTLIMPL_UHEX_0x218u 536u, (2, 1, 8), 0x218, 536, 0, 0xDE7u
#define PPUTLIMPL_UHEX_0x217u 535u, (2, 1, 7), 0x217, 535, 0, 0xDE8u
#define PPUTLIMPL_UHEX_0x216u 534u, (2, 1, 6), 0x216, 534, 0, 0xDE9u
#define PPUTLIMPL_UHEX_0x215u 533u, (2, 1, 5), 0x215, 533, 0, 0xDEAu
#define PPUTLIMPL_UHEX_0x214u 532u, (2, 1, 4), 0x214, 532, 0, 0xDEBu
#define PPUTLIMPL_UHEX_0x213u 531u, (2, 1, 3), 0x213, 531, 0, 0xDECu
#define PPUTLIMPL_UHEX_0x212u 530u, (2, 1, 2), 0x212, 530, 0, 0xDEDu
#define PPUTLIMPL_UHEX_0x211u 529u, (2, 1, 1), 0x211, 529, 0, 0xDEEu
#define PPUTLIMPL_UHEX_0x210u 528u, (2, 1, 0), 0x210, 528, 0, 0xDEFu
#define PPUTLIMPL_UHEX_0x20Fu 527u, (2, 0, F), 0x20F, 527, 0, 0xDF0u
#define PPUTLIMPL_UHEX_0x20Eu 526u, (2, 0, E), 0x20E, 526, 0, 0xDF1u
#define PPUTLIMPL_UHEX_0x20Du 525u, (2, 0, D), 0x20D, 525, 0, 0xDF2u
#define PPUTLIMPL_UHEX_0x20Cu 524u, (2, 0, C), 0x20C, 524, 0, 0xDF3u
#define PPUTLIMPL_UHEX_0x20Bu 523u, (2, 0, B), 0x20B, 523, 0, 0xDF4u
#define PPUTLIMPL_UHEX_0x20Au 522u, (2, 0, A), 0x20A, 522, 0, 0xDF5u
#define PPUTLIMPL_UHEX_0x209u 521u, (2, 0, 9), 0x209, 521, 0, 0xDF6u
#define PPUTLIMPL_UHEX_0x208u 520u, (2, 0, 8), 0x208, 520, 0, 0xDF7u
#define PPUTLIMPL_UHEX_0x207u 519u, (2, 0, 7), 0x207, 519, 0, 0xDF8u
#define PPUTLIMPL_UHEX_0x206u 518u, (2, 0, 6), 0x206, 518, 0, 0xDF9u
#define PPUTLIMPL_UHEX_0x205u 517u, (2, 0, 5), 0x205, 517, 0, 0xDFAu
#define PPUTLIMPL_UHEX_0x204u 516u, (2, 0, 4), 0x204, 516, 0, 0xDFBu
#define PPUTLIMPL_UHEX_0x203u 515u, (2, 0, 3), 0x203, 515, 0, 0xDFCu
#define PPUTLIMPL_UHEX_0x202u 514u, (2, 0, 2), 0x202, 514, 0, 0xDFDu
#define PPUTLIMPL_UHEX_0x201u 513u, (2, 0, 1), 0x201, 513, 0, 0xDFEu
#define PPUTLIMPL_UHEX_0x200u 512u, (2, 0, 0), 0x200, 512, 0, 0xDFFu
#define PPUTLIMPL_UHEX_0x1FFu 511u, (1, F, F), 0x1FF, 511, 0, 0xE00u
#define PPUTLIMPL_UHEX_0x1FEu 510u, (1, F, E), 0x1FE, 510, 0, 0xE01u
#define PPUTLIMPL_UHEX_0x1FDu 509u, (1, F, D), 0x1FD, 509, 0, 0xE02u
#define PPUTLIMPL_UHEX_0x1FCu 508u, (1, F, C), 0x1FC, 508, 0, 0xE03u
#define PPUTLIMPL_UHEX_0x1FBu 507u, (1, F, B), 0x1FB, 507, 0, 0xE04u
#define PPUTLIMPL_UHEX_0x1FAu 506u, (1, F, A), 0x1FA, 506, 0, 0xE05u
#define PPUTLIMPL_UHEX_0x1F9u 505u, (1, F, 9), 0x1F9, 505, 0, 0xE06u
#define PPUTLIMPL_UHEX_0x1F8u 504u, (1, F, 8), 0x1F8, 504, 0, 0xE07u
#define PPUTLIMPL_UHEX_0x1F7u 503u, (1, F, 7), 0x1F7, 503, 0, 0xE08u
#define PPUTLIMPL_UHEX_0x1F6u 502u, (1, F, 6), 0x1F6, 502, 0, 0xE09u
#define PPUTLIMPL_UHEX_0x1F5u 501u, (1, F, 5), 0x1F5, 501, 0, 0xE0Au
#define PPUTLIMPL_UHEX_0x1F4u 500u, (1, F, 4), 0x1F4, 500, 0, 0xE0Bu
#define PPUTLIMPL_UHEX_0x1F3u 499u, (1, F, 3), 0x1F3, 499, 0, 0xE0Cu
#define PPUTLIMPL_UHEX_0x1F2u 498u, (1, F, 2), 0x1F2, 498, 0, 0xE0Du
#define PPUTLIMPL_UHEX_0x1F1u 497u, (1, F, 1), 0x1F1, 497, 0, 0xE0Eu
#define PPUTLIMPL_UHEX_0x1F0u 496u, (1, F, 0), 0x1F0, 496, 0, 0xE0Fu
#define PPUTLIMPL_UHEX_0x1EFu 495u, (1, E, F), 0x1EF, 495, 0, 0xE10u
#define PPUTLIMPL_UHEX_0x1EEu 494u, (1, E, E), 0x1EE, 494, 0, 0xE11u
#define PPUTLIMPL_UHEX_0x1EDu 493u, (1, E, D), 0x1ED, 493, 0, 0xE12u
#define PPUTLIMPL_UHEX_0x1ECu 492u, (1, E, C), 0x1EC, 492, 0, 0xE13u
#define PPUTLIMPL_UHEX_0x1EBu 491u, (1, E, B), 0x1EB, 491, 0, 0xE14u
#define PPUTLIMPL_UHEX_0x1EAu 490u, (1, E, A), 0x1EA, 490, 0, 0xE15u
#define PPUTLIMPL_UHEX_0x1E9u 489u, (1, E, 9), 0x1E9, 489, 0, 0xE16u
#define PPUTLIMPL_UHEX_0x1E8u 488u, (1, E, 8), 0x1E8, 488, 0, 0xE17u
#define PPUTLIMPL_UHEX_0x1E7u 487u, (1, E, 7), 0x1E7, 487, 0, 0xE18u
#define PPUTLIMPL_UHEX_0x1E6u 486u, (1, E, 6), 0x1E6, 486, 0, 0xE19u
#define PPUTLIMPL_UHEX_0x1E5u 485u, (1, E, 5), 0x1E5, 485, 0, 0xE1Au
#define PPUTLIMPL_UHEX_0x1E4u 484u, (1, E, 4), 0x1E4, 484, 0, 0xE1Bu
#define PPUTLIMPL_UHEX_0x1E3u 483u, (1, E, 3), 0x1E3, 483, 0, 0xE1Cu
#define PPUTLIMPL_UHEX_0x1E2u 482u, (1, E, 2), 0x1E2, 482, 0, 0xE1Du
#define PPUTLIMPL_UHEX_0x1E1u 481u, (1, E, 1), 0x1E1, 481, 0, 0xE1Eu
#define PPUTLIMPL_UHEX_0x1E0u 480u, (1, E, 0), 0x1E0, 480, 0, 0xE1Fu
#define PPUTLIMPL_UHEX_0x1DFu 479u, (1, D, F), 0x1DF, 479, 0, 0xE20u
#define PPUTLIMPL_UHEX_0x1DEu 478u, (1, D, E), 0x1DE, 478, 0, 0xE21u
#define PPUTLIMPL_UHEX_0x1DDu 477u, (1, D, D), 0x1DD, 477, 0, 0xE22u
#define PPUTLIMPL_UHEX_0x1DCu 476u, (1, D, C), 0x1DC, 476, 0, 0xE23u
#define PPUTLIMPL_UHEX_0x1DBu 475u, (1, D, B), 0x1DB, 475, 0, 0xE24u
#define PPUTLIMPL_UHEX_0x1DAu 474u, (1, D, A), 0x1DA, 474, 0, 0xE25u
#define PPUTLIMPL_UHEX_0x1D9u 473u, (1, D, 9), 0x1D9, 473, 0, 0xE26u
#define PPUTLIMPL_UHEX_0x1D8u 472u, (1, D, 8), 0x1D8, 472, 0, 0xE27u
#define PPUTLIMPL_UHEX_0x1D7u 471u, (1, D, 7), 0x1D7, 471, 0, 0xE28u
#define PPUTLIMPL_UHEX_0x1D6u 470u, (1, D, 6), 0x1D6, 470, 0, 0xE29u
#define PPUTLIMPL_UHEX_0x1D5u 469u, (1, D, 5), 0x1D5, 469, 0, 0xE2Au
#define PPUTLIMPL_UHEX_0x1D4u 468u, (1, D, 4), 0x1D4, 468, 0, 0xE2Bu
#define PPUTLIMPL_UHEX_0x1D3u 467u, (1, D, 3), 0x1D3, 467, 0, 0xE2Cu
#define PPUTLIMPL_UHEX_0x1D2u 466u, (1, D, 2), 0x1D2, 466, 0, 0xE2Du
#define PPUTLIMPL_UHEX_0x1D1u 465u, (1, D, 1), 0x1D1, 465, 0, 0xE2Eu
#define PPUTLIMPL_UHEX_0x1D0u 464u, (1, D, 0), 0x1D0, 464, 0, 0xE2Fu
#define PPUTLIMPL_UHEX_0x1CFu 463u, (1, C, F), 0x1CF, 463, 0, 0xE30u
#define PPUTLIMPL_UHEX_0x1CEu 462u, (1, C, E), 0x1CE, 462, 0, 0xE31u
#define PPUTLIMPL_UHEX_0x1CDu 461u, (1, C, D), 0x1CD, 461, 0, 0xE32u
#define PPUTLIMPL_UHEX_0x1CCu 460u, (1, C, C), 0x1CC, 460, 0, 0xE33u
#define PPUTLIMPL_UHEX_0x1CBu 459u, (1, C, B), 0x1CB, 459, 0, 0xE34u
#define PPUTLIMPL_UHEX_0x1CAu 458u, (1, C, A), 0x1CA, 458, 0, 0xE35u
#define PPUTLIMPL_UHEX_0x1C9u 457u, (1, C, 9), 0x1C9, 457, 0, 0xE36u
#define PPUTLIMPL_UHEX_0x1C8u 456u, (1, C, 8), 0x1C8, 456, 0, 0xE37u
#define PPUTLIMPL_UHEX_0x1C7u 455u, (1, C, 7), 0x1C7, 455, 0, 0xE38u
#define PPUTLIMPL_UHEX_0x1C6u 454u, (1, C, 6), 0x1C6, 454, 0, 0xE39u
#define PPUTLIMPL_UHEX_0x1C5u 453u, (1, C, 5), 0x1C5, 453, 0, 0xE3Au
#define PPUTLIMPL_UHEX_0x1C4u 452u, (1, C, 4), 0x1C4, 452, 0, 0xE3Bu
#define PPUTLIMPL_UHEX_0x1C3u 451u, (1, C, 3), 0x1C3, 451, 0, 0xE3Cu
#define PPUTLIMPL_UHEX_0x1C2u 450u, (1, C, 2), 0x1C2, 450, 0, 0xE3Du
#define PPUTLIMPL_UHEX_0x1C1u 449u, (1, C, 1), 0x1C1, 449, 0, 0xE3Eu
#define PPUTLIMPL_UHEX_0x1C0u 448u, (1, C, 0), 0x1C0, 448, 0, 0xE3Fu
#define PPUTLIMPL_UHEX_0x1BFu 447u, (1, B, F), 0x1BF, 447, 0, 0xE40u
#define PPUTLIMPL_UHEX_0x1BEu 446u, (1, B, E), 0x1BE, 446, 0, 0xE41u
#define PPUTLIMPL_UHEX_0x1BDu 445u, (1, B, D), 0x1BD, 445, 0, 0xE42u
#define PPUTLIMPL_UHEX_0x1BCu 444u, (1, B, C), 0x1BC, 444, 0, 0xE43u
#define PPUTLIMPL_UHEX_0x1BBu 443u, (1, B, B), 0x1BB, 443, 0, 0xE44u
#define PPUTLIMPL_UHEX_0x1BAu 442u, (1, B, A), 0x1BA, 442, 0, 0xE45u
#define PPUTLIMPL_UHEX_0x1B9u 441u, (1, B, 9), 0x1B9, 441, 0, 0xE46u
#define PPUTLIMPL_UHEX_0x1B8u 440u, (1, B, 8), 0x1B8, 440, 0, 0xE47u
#define PPUTLIMPL_UHEX_0x1B7u 439u, (1, B, 7), 0x1B7, 439, 0, 0xE48u
#define PPUTLIMPL_UHEX_0x1B6u 438u, (1, B, 6), 0x1B6, 438, 0, 0xE49u
#define PPUTLIMPL_UHEX_0x1B5u 437u, (1, B, 5), 0x1B5, 437, 0, 0xE4Au
#define PPUTLIMPL_UHEX_0x1B4u 436u, (1, B, 4), 0x1B4, 436, 0, 0xE4Bu
#define PPUTLIMPL_UHEX_0x1B3u 435u, (1, B, 3), 0x1B3, 435, 0, 0xE4Cu
#define PPUTLIMPL_UHEX_0x1B2u 434u, (1, B, 2), 0x1B2, 434, 0, 0xE4Du
#define PPUTLIMPL_UHEX_0x1B1u 433u, (1, B, 1), 0x1B1, 433, 0, 0xE4Eu
#define PPUTLIMPL_UHEX_0x1B0u 432u, (1, B, 0), 0x1B0, 432, 0, 0xE4Fu
#define PPUTLIMPL_UHEX_0x1AFu 431u, (1, A, F), 0x1AF, 431, 0, 0xE50u
#define PPUTLIMPL_UHEX_0x1AEu 430u, (1, A, E), 0x1AE, 430, 0, 0xE51u
#define PPUTLIMPL_UHEX_0x1ADu 429u, (1, A, D), 0x1AD, 429, 0, 0xE52u
#define PPUTLIMPL_UHEX_0x1ACu 428u, (1, A, C), 0x1AC, 428, 0, 0xE53u
#define PPUTLIMPL_UHEX_0x1ABu 427u, (1, A, B), 0x1AB, 427, 0, 0xE54u
#define PPUTLIMPL_UHEX_0x1AAu 426u, (1, A, A), 0x1AA, 426, 0, 0xE55u
#define PPUTLIMPL_UHEX_0x1A9u 425u, (1, A, 9), 0x1A9, 425, 0, 0xE56u
#define PPUTLIMPL_UHEX_0x1A8u 424u, (1, A, 8), 0x1A8, 424, 0, 0xE57u
#define PPUTLIMPL_UHEX_0x1A7u 423u, (1, A, 7), 0x1A7, 423, 0, 0xE58u
#define PPUTLIMPL_UHEX_0x1A6u 422u, (1, A, 6), 0x1A6, 422, 0, 0xE59u
#define PPUTLIMPL_UHEX_0x1A5u 421u, (1, A, 5), 0x1A5, 421, 0, 0xE5Au
#define PPUTLIMPL_UHEX_0x1A4u 420u, (1, A, 4), 0x1A4, 420, 0, 0xE5Bu
#define PPUTLIMPL_UHEX_0x1A3u 419u, (1, A, 3), 0x1A3, 419, 0, 0xE5Cu
#define PPUTLIMPL_UHEX_0x1A2u 418u, (1, A, 2), 0x1A2, 418, 0, 0xE5Du
#define PPUTLIMPL_UHEX_0x1A1u 417u, (1, A, 1), 0x1A1, 417, 0, 0xE5Eu
#define PPUTLIMPL_UHEX_0x1A0u 416u, (1, A, 0), 0x1A0, 416, 0, 0xE5Fu
#define PPUTLIMPL_UHEX_0x19Fu 415u, (1, 9, F), 0x19F, 415, 0, 0xE60u
#define PPUTLIMPL_UHEX_0x19Eu 414u, (1, 9, E), 0x19E, 414, 0, 0xE61u
#define PPUTLIMPL_UHEX_0x19Du 413u, (1, 9, D), 0x19D, 413, 0, 0xE62u
#define PPUTLIMPL_UHEX_0x19Cu 412u, (1, 9, C), 0x19C, 412, 0, 0xE63u
#define PPUTLIMPL_UHEX_0x19Bu 411u, (1, 9, B), 0x19B, 411, 0, 0xE64u
#define PPUTLIMPL_UHEX_0x19Au 410u, (1, 9, A), 0x19A, 410, 0, 0xE65u
#define PPUTLIMPL_UHEX_0x199u 409u, (1, 9, 9), 0x199, 409, 0, 0xE66u
#define PPUTLIMPL_UHEX_0x198u 408u, (1, 9, 8), 0x198, 408, 0, 0xE67u
#define PPUTLIMPL_UHEX_0x197u 407u, (1, 9, 7), 0x197, 407, 0, 0xE68u
#define PPUTLIMPL_UHEX_0x196u 406u, (1, 9, 6), 0x196, 406, 0, 0xE69u
#define PPUTLIMPL_UHEX_0x195u 405u, (1, 9, 5), 0x195, 405, 0, 0xE6Au
#define PPUTLIMPL_UHEX_0x194u 404u, (1, 9, 4), 0x194, 404, 0, 0xE6Bu
#define PPUTLIMPL_UHEX_0x193u 403u, (1, 9, 3), 0x193, 403, 0, 0xE6Cu
#define PPUTLIMPL_UHEX_0x192u 402u, (1, 9, 2), 0x192, 402, 0, 0xE6Du
#define PPUTLIMPL_UHEX_0x191u 401u, (1, 9, 1), 0x191, 401, 0, 0xE6Eu
#define PPUTLIMPL_UHEX_0x190u 400u, (1, 9, 0), 0x190, 400, 0, 0xE6Fu
#define PPUTLIMPL_UHEX_0x18Fu 399u, (1, 8, F), 0x18F, 399, 0, 0xE70u
#define PPUTLIMPL_UHEX_0x18Eu 398u, (1, 8, E), 0x18E, 398, 0, 0xE71u
#define PPUTLIMPL_UHEX_0x18Du 397u, (1, 8, D), 0x18D, 397, 0, 0xE72u
#define PPUTLIMPL_UHEX_0x18Cu 396u, (1, 8, C), 0x18C, 396, 0, 0xE73u
#define PPUTLIMPL_UHEX_0x18Bu 395u, (1, 8, B), 0x18B, 395, 0, 0xE74u
#define PPUTLIMPL_UHEX_0x18Au 394u, (1, 8, A), 0x18A, 394, 0, 0xE75u
#define PPUTLIMPL_UHEX_0x189u 393u, (1, 8, 9), 0x189, 393, 0, 0xE76u
#define PPUTLIMPL_UHEX_0x188u 392u, (1, 8, 8), 0x188, 392, 0, 0xE77u
#define PPUTLIMPL_UHEX_0x187u 391u, (1, 8, 7), 0x187, 391, 0, 0xE78u
#define PPUTLIMPL_UHEX_0x186u 390u, (1, 8, 6), 0x186, 390, 0, 0xE79u
#define PPUTLIMPL_UHEX_0x185u 389u, (1, 8, 5), 0x185, 389, 0, 0xE7Au
#define PPUTLIMPL_UHEX_0x184u 388u, (1, 8, 4), 0x184, 388, 0, 0xE7Bu
#define PPUTLIMPL_UHEX_0x183u 387u, (1, 8, 3), 0x183, 387, 0, 0xE7Cu
#define PPUTLIMPL_UHEX_0x182u 386u, (1, 8, 2), 0x182, 386, 0, 0xE7Du
#define PPUTLIMPL_UHEX_0x181u 385u, (1, 8, 1), 0x181, 385, 0, 0xE7Eu
#define PPUTLIMPL_UHEX_0x180u 384u, (1, 8, 0), 0x180, 384, 0, 0xE7Fu
#define PPUTLIMPL_UHEX_0x17Fu 383u, (1, 7, F), 0x17F, 383, 0, 0xE80u
#define PPUTLIMPL_UHEX_0x17Eu 382u, (1, 7, E), 0x17E, 382, 0, 0xE81u
#define PPUTLIMPL_UHEX_0x17Du 381u, (1, 7, D), 0x17D, 381, 0, 0xE82u
#define PPUTLIMPL_UHEX_0x17Cu 380u, (1, 7, C), 0x17C, 380, 0, 0xE83u
#define PPUTLIMPL_UHEX_0x17Bu 379u, (1, 7, B), 0x17B, 379, 0, 0xE84u
#define PPUTLIMPL_UHEX_0x17Au 378u, (1, 7, A), 0x17A, 378, 0, 0xE85u
#define PPUTLIMPL_UHEX_0x179u 377u, (1, 7, 9), 0x179, 377, 0, 0xE86u
#define PPUTLIMPL_UHEX_0x178u 376u, (1, 7, 8), 0x178, 376, 0, 0xE87u
#define PPUTLIMPL_UHEX_0x177u 375u, (1, 7, 7), 0x177, 375, 0, 0xE88u
#define PPUTLIMPL_UHEX_0x176u 374u, (1, 7, 6), 0x176, 374, 0, 0xE89u
#define PPUTLIMPL_UHEX_0x175u 373u, (1, 7, 5), 0x175, 373, 0, 0xE8Au
#define PPUTLIMPL_UHEX_0x174u 372u, (1, 7, 4), 0x174, 372, 0, 0xE8Bu
#define PPUTLIMPL_UHEX_0x173u 371u, (1, 7, 3), 0x173, 371, 0, 0xE8Cu
#define PPUTLIMPL_UHEX_0x172u 370u, (1, 7, 2), 0x172, 370, 0, 0xE8Du
#define PPUTLIMPL_UHEX_0x171u 369u, (1, 7, 1), 0x171, 369, 0, 0xE8Eu
#define PPUTLIMPL_UHEX_0x170u 368u, (1, 7, 0), 0x170, 368, 0, 0xE8Fu
#define PPUTLIMPL_UHEX_0x16Fu 367u, (1, 6, F), 0x16F, 367, 0, 0xE90u
#define PPUTLIMPL_UHEX_0x16Eu 366u, (1, 6, E), 0x16E, 366, 0, 0xE91u
#define PPUTLIMPL_UHEX_0x16Du 365u, (1, 6, D), 0x16D, 365, 0, 0xE92u
#define PPUTLIMPL_UHEX_0x16Cu 364u, (1, 6, C), 0x16C, 364, 0, 0xE93u
#define PPUTLIMPL_UHEX_0x16Bu 363u, (1, 6, B), 0x16B, 363, 0, 0xE94u
#define PPUTLIMPL_UHEX_0x16Au 362u, (1, 6, A), 0x16A, 362, 0, 0xE95u
#define PPUTLIMPL_UHEX_0x169u 361u, (1, 6, 9), 0x169, 361, 0, 0xE96u
#define PPUTLIMPL_UHEX_0x168u 360u, (1, 6, 8), 0x168, 360, 0, 0xE97u
#define PPUTLIMPL_UHEX_0x167u 359u, (1, 6, 7), 0x167, 359, 0, 0xE98u
#define PPUTLIMPL_UHEX_0x166u 358u, (1, 6, 6), 0x166, 358, 0, 0xE99u
#define PPUTLIMPL_UHEX_0x165u 357u, (1, 6, 5), 0x165, 357, 0, 0xE9Au
#define PPUTLIMPL_UHEX_0x164u 356u, (1, 6, 4), 0x164, 356, 0, 0xE9Bu
#define PPUTLIMPL_UHEX_0x163u 355u, (1, 6, 3), 0x163, 355, 0, 0xE9Cu
#define PPUTLIMPL_UHEX_0x162u 354u, (1, 6, 2), 0x162, 354, 0, 0xE9Du
#define PPUTLIMPL_UHEX_0x161u 353u, (1, 6, 1), 0x161, 353, 0, 0xE9Eu
#define PPUTLIMPL_UHEX_0x160u 352u, (1, 6, 0), 0x160, 352, 0, 0xE9Fu
#define PPUTLIMPL_UHEX_0x15Fu 351u, (1, 5, F), 0x15F, 351, 0, 0xEA0u
#define PPUTLIMPL_UHEX_0x15Eu 350u, (1, 5, E), 0x15E, 350, 0, 0xEA1u
#define PPUTLIMPL_UHEX_0x15Du 349u, (1, 5, D), 0x15D, 349, 0, 0xEA2u
#define PPUTLIMPL_UHEX_0x15Cu 348u, (1, 5, C), 0x15C, 348, 0, 0xEA3u
#define PPUTLIMPL_UHEX_0x15Bu 347u, (1, 5, B), 0x15B, 347, 0, 0xEA4u
#define PPUTLIMPL_UHEX_0x15Au 346u, (1, 5, A), 0x15A, 346, 0, 0xEA5u
#define PPUTLIMPL_UHEX_0x159u 345u, (1, 5, 9), 0x159, 345, 0, 0xEA6u
#define PPUTLIMPL_UHEX_0x158u 344u, (1, 5, 8), 0x158, 344, 0, 0xEA7u
#define PPUTLIMPL_UHEX_0x157u 343u, (1, 5, 7), 0x157, 343, 0, 0xEA8u
#define PPUTLIMPL_UHEX_0x156u 342u, (1, 5, 6), 0x156, 342, 0, 0xEA9u
#define PPUTLIMPL_UHEX_0x155u 341u, (1, 5, 5), 0x155, 341, 0, 0xEAAu
#define PPUTLIMPL_UHEX_0x154u 340u, (1, 5, 4), 0x154, 340, 0, 0xEABu
#define PPUTLIMPL_UHEX_0x153u 339u, (1, 5, 3), 0x153, 339, 0, 0xEACu
#define PPUTLIMPL_UHEX_0x152u 338u, (1, 5, 2), 0x152, 338, 0, 0xEADu
#define PPUTLIMPL_UHEX_0x151u 337u, (1, 5, 1), 0x151, 337, 0, 0xEAEu
#define PPUTLIMPL_UHEX_0x150u 336u, (1, 5, 0), 0x150, 336, 0, 0xEAFu
#define PPUTLIMPL_UHEX_0x14Fu 335u, (1, 4, F), 0x14F, 335, 0, 0xEB0u
#define PPUTLIMPL_UHEX_0x14Eu 334u, (1, 4, E), 0x14E, 334, 0, 0xEB1u
#define PPUTLIMPL_UHEX_0x14Du 333u, (1, 4, D), 0x14D, 333, 0, 0xEB2u
#define PPUTLIMPL_UHEX_0x14Cu 332u, (1, 4, C), 0x14C, 332, 0, 0xEB3u
#define PPUTLIMPL_UHEX_0x14Bu 331u, (1, 4, B), 0x14B, 331, 0, 0xEB4u
#define PPUTLIMPL_UHEX_0x14Au 330u, (1, 4, A), 0x14A, 330, 0, 0xEB5u
#define PPUTLIMPL_UHEX_0x149u 329u, (1, 4, 9), 0x149, 329, 0, 0xEB6u
#define PPUTLIMPL_UHEX_0x148u 328u, (1, 4, 8), 0x148, 328, 0, 0xEB7u
#define PPUTLIMPL_UHEX_0x147u 327u, (1, 4, 7), 0x147, 327, 0, 0xEB8u
#define PPUTLIMPL_UHEX_0x146u 326u, (1, 4, 6), 0x146, 326, 0, 0xEB9u
#define PPUTLIMPL_UHEX_0x145u 325u, (1, 4, 5), 0x145, 325, 0, 0xEBAu
#define PPUTLIMPL_UHEX_0x144u 324u, (1, 4, 4), 0x144, 324, 0, 0xEBBu
#define PPUTLIMPL_UHEX_0x143u 323u, (1, 4, 3), 0x143, 323, 0, 0xEBCu
#define PPUTLIMPL_UHEX_0x142u 322u, (1, 4, 2), 0x142, 322, 0, 0xEBDu
#define PPUTLIMPL_UHEX_0x141u 321u, (1, 4, 1), 0x141, 321, 0, 0xEBEu
#define PPUTLIMPL_UHEX_0x140u 320u, (1, 4, 0), 0x140, 320, 0, 0xEBFu
#define PPUTLIMPL_UHEX_0x13Fu 319u, (1, 3, F), 0x13F, 319, 0, 0xEC0u
#define PPUTLIMPL_UHEX_0x13Eu 318u, (1, 3, E), 0x13E, 318, 0, 0xEC1u
#define PPUTLIMPL_UHEX_0x13Du 317u, (1, 3, D), 0x13D, 317, 0, 0xEC2u
#define PPUTLIMPL_UHEX_0x13Cu 316u, (1, 3, C), 0x13C, 316, 0, 0xEC3u
#define PPUTLIMPL_UHEX_0x13Bu 315u, (1, 3, B), 0x13B, 315, 0, 0xEC4u
#define PPUTLIMPL_UHEX_0x13Au 314u, (1, 3, A), 0x13A, 314, 0, 0xEC5u
#define PPUTLIMPL_UHEX_0x139u 313u, (1, 3, 9), 0x139, 313, 0, 0xEC6u
#define PPUTLIMPL_UHEX_0x138u 312u, (1, 3, 8), 0x138, 312, 0, 0xEC7u
#define PPUTLIMPL_UHEX_0x137u 311u, (1, 3, 7), 0x137, 311, 0, 0xEC8u
#define PPUTLIMPL_UHEX_0x136u 310u, (1, 3, 6), 0x136, 310, 0, 0xEC9u
#define PPUTLIMPL_UHEX_0x135u 309u, (1, 3, 5), 0x135, 309, 0, 0xECAu
#define PPUTLIMPL_UHEX_0x134u 308u, (1, 3, 4), 0x134, 308, 0, 0xECBu
#define PPUTLIMPL_UHEX_0x133u 307u, (1, 3, 3), 0x133, 307, 0, 0xECCu
#define PPUTLIMPL_UHEX_0x132u 306u, (1, 3, 2), 0x132, 306, 0, 0xECDu
#define PPUTLIMPL_UHEX_0x131u 305u, (1, 3, 1), 0x131, 305, 0, 0xECEu
#define PPUTLIMPL_UHEX_0x130u 304u, (1, 3, 0), 0x130, 304, 0, 0xECFu
#define PPUTLIMPL_UHEX_0x12Fu 303u, (1, 2, F), 0x12F, 303, 0, 0xED0u
#define PPUTLIMPL_UHEX_0x12Eu 302u, (1, 2, E), 0x12E, 302, 0, 0xED1u
#define PPUTLIMPL_UHEX_0x12Du 301u, (1, 2, D), 0x12D, 301, 0, 0xED2u
#define PPUTLIMPL_UHEX_0x12Cu 300u, (1, 2, C), 0x12C, 300, 0, 0xED3u
#define PPUTLIMPL_UHEX_0x12Bu 299u, (1, 2, B), 0x12B, 299, 0, 0xED4u
#define PPUTLIMPL_UHEX_0x12Au 298u, (1, 2, A), 0x12A, 298, 0, 0xED5u
#define PPUTLIMPL_UHEX_0x129u 297u, (1, 2, 9), 0x129, 297, 0, 0xED6u
#define PPUTLIMPL_UHEX_0x128u 296u, (1, 2, 8), 0x128, 296, 0, 0xED7u
#define PPUTLIMPL_UHEX_0x127u 295u, (1, 2, 7), 0x127, 295, 0, 0xED8u
#define PPUTLIMPL_UHEX_0x126u 294u, (1, 2, 6), 0x126, 294, 0, 0xED9u
#define PPUTLIMPL_UHEX_0x125u 293u, (1, 2, 5), 0x125, 293, 0, 0xEDAu
#define PPUTLIMPL_UHEX_0x124u 292u, (1, 2, 4), 0x124, 292, 0, 0xEDBu
#define PPUTLIMPL_UHEX_0x123u 291u, (1, 2, 3), 0x123, 291, 0, 0xEDCu
#define PPUTLIMPL_UHEX_0x122u 290u, (1, 2, 2), 0x122, 290, 0, 0xEDDu
#define PPUTLIMPL_UHEX_0x121u 289u, (1, 2, 1), 0x121, 289, 0, 0xEDEu
#define PPUTLIMPL_UHEX_0x120u 288u, (1, 2, 0), 0x120, 288, 0, 0xEDFu
#define PPUTLIMPL_UHEX_0x11Fu 287u, (1, 1, F), 0x11F, 287, 0, 0xEE0u
#define PPUTLIMPL_UHEX_0x11Eu 286u, (1, 1, E), 0x11E, 286, 0, 0xEE1u
#define PPUTLIMPL_UHEX_0x11Du 285u, (1, 1, D), 0x11D, 285, 0, 0xEE2u
#define PPUTLIMPL_UHEX_0x11Cu 284u, (1, 1, C), 0x11C, 284, 0, 0xEE3u
#define PPUTLIMPL_UHEX_0x11Bu 283u, (1, 1, B), 0x11B, 283, 0, 0xEE4u
#define PPUTLIMPL_UHEX_0x11Au 282u, (1, 1, A), 0x11A, 282, 0, 0xEE5u
#define PPUTLIMPL_UHEX_0x119u 281u, (1, 1, 9), 0x119, 281, 0, 0xEE6u
#define PPUTLIMPL_UHEX_0x118u 280u, (1, 1, 8), 0x118, 280, 0, 0xEE7u
#define PPUTLIMPL_UHEX_0x117u 279u, (1, 1, 7), 0x117, 279, 0, 0xEE8u
#define PPUTLIMPL_UHEX_0x116u 278u, (1, 1, 6), 0x116, 278, 0, 0xEE9u
#define PPUTLIMPL_UHEX_0x115u 277u, (1, 1, 5), 0x115, 277, 0, 0xEEAu
#define PPUTLIMPL_UHEX_0x114u 276u, (1, 1, 4), 0x114, 276, 0, 0xEEBu
#define PPUTLIMPL_UHEX_0x113u 275u, (1, 1, 3), 0x113, 275, 0, 0xEECu
#define PPUTLIMPL_UHEX_0x112u 274u, (1, 1, 2), 0x112, 274, 0, 0xEEDu
#define PPUTLIMPL_UHEX_0x111u 273u, (1, 1, 1), 0x111, 273, 0, 0xEEEu
#define PPUTLIMPL_UHEX_0x110u 272u, (1, 1, 0), 0x110, 272, 0, 0xEEFu
#define PPUTLIMPL_UHEX_0x10Fu 271u, (1, 0, F), 0x10F, 271, 0, 0xEF0u
#define PPUTLIMPL_UHEX_0x10Eu 270u, (1, 0, E), 0x10E, 270, 0, 0xEF1u
#define PPUTLIMPL_UHEX_0x10Du 269u, (1, 0, D), 0x10D, 269, 0, 0xEF2u
#define PPUTLIMPL_UHEX_0x10Cu 268u, (1, 0, C), 0x10C, 268, 0, 0xEF3u
#define PPUTLIMPL_UHEX_0x10Bu 267u, (1, 0, B), 0x10B, 267, 0, 0xEF4u
#define PPUTLIMPL_UHEX_0x10Au 266u, (1, 0, A), 0x10A, 266, 0, 0xEF5u
#define PPUTLIMPL_UHEX_0x109u 265u, (1, 0, 9), 0x109, 265, 0, 0xEF6u
#define PPUTLIMPL_UHEX_0x108u 264u, (1, 0, 8), 0x108, 264, 0, 0xEF7u
#define PPUTLIMPL_UHEX_0x107u 263u, (1, 0, 7), 0x107, 263, 0, 0xEF8u
#define PPUTLIMPL_UHEX_0x106u 262u, (1, 0, 6), 0x106, 262, 0, 0xEF9u
#define PPUTLIMPL_UHEX_0x105u 261u, (1, 0, 5), 0x105, 261, 0, 0xEFAu
#define PPUTLIMPL_UHEX_0x104u 260u, (1, 0, 4), 0x104, 260, 0, 0xEFBu
#define PPUTLIMPL_UHEX_0x103u 259u, (1, 0, 3), 0x103, 259, 0, 0xEFCu
#define PPUTLIMPL_UHEX_0x102u 258u, (1, 0, 2), 0x102, 258, 0, 0xEFDu
#define PPUTLIMPL_UHEX_0x101u 257u, (1, 0, 1), 0x101, 257, 0, 0xEFEu
#define PPUTLIMPL_UHEX_0x100u 256u, (1, 0, 0), 0x100, 256, 0, 0xEFFu
#define PPUTLIMPL_UHEX_0x0FFu 255u, (0, F, F), 0x0FF, 255, 0, 0xF00u
#define PPUTLIMPL_UHEX_0x0FEu 254u, (0, F, E), 0x0FE, 254, 0, 0xF01u
#define PPUTLIMPL_UHEX_0x0FDu 253u, (0, F, D), 0x0FD, 253, 0, 0xF02u
#define PPUTLIMPL_UHEX_0x0FCu 252u, (0, F, C), 0x0FC, 252, 0, 0xF03u
#define PPUTLIMPL_UHEX_0x0FBu 251u, (0, F, B), 0x0FB, 251, 0, 0xF04u
#define PPUTLIMPL_UHEX_0x0FAu 250u, (0, F, A), 0x0FA, 250, 0, 0xF05u
#define PPUTLIMPL_UHEX_0x0F9u 249u, (0, F, 9), 0x0F9, 249, 0, 0xF06u
#define PPUTLIMPL_UHEX_0x0F8u 248u, (0, F, 8), 0x0F8, 248, 0, 0xF07u
#define PPUTLIMPL_UHEX_0x0F7u 247u, (0, F, 7), 0x0F7, 247, 0, 0xF08u
#define PPUTLIMPL_UHEX_0x0F6u 246u, (0, F, 6), 0x0F6, 246, 0, 0xF09u
#define PPUTLIMPL_UHEX_0x0F5u 245u, (0, F, 5), 0x0F5, 245, 0, 0xF0Au
#define PPUTLIMPL_UHEX_0x0F4u 244u, (0, F, 4), 0x0F4, 244, 0, 0xF0Bu
#define PPUTLIMPL_UHEX_0x0F3u 243u, (0, F, 3), 0x0F3, 243, 0, 0xF0Cu
#define PPUTLIMPL_UHEX_0x0F2u 242u, (0, F, 2), 0x0F2, 242, 0, 0xF0Du
#define PPUTLIMPL_UHEX_0x0F1u 241u, (0, F, 1), 0x0F1, 241, 0, 0xF0Eu
#define PPUTLIMPL_UHEX_0x0F0u 240u, (0, F, 0), 0x0F0, 240, 0, 0xF0Fu
#define PPUTLIMPL_UHEX_0x0EFu 239u, (0, E, F), 0x0EF, 239, 0, 0xF10u
#define PPUTLIMPL_UHEX_0x0EEu 238u, (0, E, E), 0x0EE, 238, 0, 0xF11u
#define PPUTLIMPL_UHEX_0x0EDu 237u, (0, E, D), 0x0ED, 237, 0, 0xF12u
#define PPUTLIMPL_UHEX_0x0ECu 236u, (0, E, C), 0x0EC, 236, 0, 0xF13u
#define PPUTLIMPL_UHEX_0x0EBu 235u, (0, E, B), 0x0EB, 235, 0, 0xF14u
#define PPUTLIMPL_UHEX_0x0EAu 234u, (0, E, A), 0x0EA, 234, 0, 0xF15u
#define PPUTLIMPL_UHEX_0x0E9u 233u, (0, E, 9), 0x0E9, 233, 0, 0xF16u
#define PPUTLIMPL_UHEX_0x0E8u 232u, (0, E, 8), 0x0E8, 232, 0, 0xF17u
#define PPUTLIMPL_UHEX_0x0E7u 231u, (0, E, 7), 0x0E7, 231, 0, 0xF18u
#define PPUTLIMPL_UHEX_0x0E6u 230u, (0, E, 6), 0x0E6, 230, 0, 0xF19u
#define PPUTLIMPL_UHEX_0x0E5u 229u, (0, E, 5), 0x0E5, 229, 0, 0xF1Au
#define PPUTLIMPL_UHEX_0x0E4u 228u, (0, E, 4), 0x0E4, 228, 0, 0xF1Bu
#define PPUTLIMPL_UHEX_0x0E3u 227u, (0, E, 3), 0x0E3, 227, 0, 0xF1Cu
#define PPUTLIMPL_UHEX_0x0E2u 226u, (0, E, 2), 0x0E2, 226, 0, 0xF1Du
#define PPUTLIMPL_UHEX_0x0E1u 225u, (0, E, 1), 0x0E1, 225, 0, 0xF1Eu
#define PPUTLIMPL_UHEX_0x0E0u 224u, (0, E, 0), 0x0E0, 224, 0, 0xF1Fu
#define PPUTLIMPL_UHEX_0x0DFu 223u, (0, D, F), 0x0DF, 223, 0, 0xF20u
#define PPUTLIMPL_UHEX_0x0DEu 222u, (0, D, E), 0x0DE, 222, 0, 0xF21u
#define PPUTLIMPL_UHEX_0x0DDu 221u, (0, D, D), 0x0DD, 221, 0, 0xF22u
#define PPUTLIMPL_UHEX_0x0DCu 220u, (0, D, C), 0x0DC, 220, 0, 0xF23u
#define PPUTLIMPL_UHEX_0x0DBu 219u, (0, D, B), 0x0DB, 219, 0, 0xF24u
#define PPUTLIMPL_UHEX_0x0DAu 218u, (0, D, A), 0x0DA, 218, 0, 0xF25u
#define PPUTLIMPL_UHEX_0x0D9u 217u, (0, D, 9), 0x0D9, 217, 0, 0xF26u
#define PPUTLIMPL_UHEX_0x0D8u 216u, (0, D, 8), 0x0D8, 216, 0, 0xF27u
#define PPUTLIMPL_UHEX_0x0D7u 215u, (0, D, 7), 0x0D7, 215, 0, 0xF28u
#define PPUTLIMPL_UHEX_0x0D6u 214u, (0, D, 6), 0x0D6, 214, 0, 0xF29u
#define PPUTLIMPL_UHEX_0x0D5u 213u, (0, D, 5), 0x0D5, 213, 0, 0xF2Au
#define PPUTLIMPL_UHEX_0x0D4u 212u, (0, D, 4), 0x0D4, 212, 0, 0xF2Bu
#define PPUTLIMPL_UHEX_0x0D3u 211u, (0, D, 3), 0x0D3, 211, 0, 0xF2Cu
#define PPUTLIMPL_UHEX_0x0D2u 210u, (0, D, 2), 0x0D2, 210, 0, 0xF2Du
#define PPUTLIMPL_UHEX_0x0D1u 209u, (0, D, 1), 0x0D1, 209, 0, 0xF2Eu
#define PPUTLIMPL_UHEX_0x0D0u 208u, (0, D, 0), 0x0D0, 208, 0, 0xF2Fu
#define PPUTLIMPL_UHEX_0x0CFu 207u, (0, C, F), 0x0CF, 207, 0, 0xF30u
#define PPUTLIMPL_UHEX_0x0CEu 206u, (0, C, E), 0x0CE, 206, 0, 0xF31u
#define PPUTLIMPL_UHEX_0x0CDu 205u, (0, C, D), 0x0CD, 205, 0, 0xF32u
#define PPUTLIMPL_UHEX_0x0CCu 204u, (0, C, C), 0x0CC, 204, 0, 0xF33u
#define PPUTLIMPL_UHEX_0x0CBu 203u, (0, C, B), 0x0CB, 203, 0, 0xF34u
#define PPUTLIMPL_UHEX_0x0CAu 202u, (0, C, A), 0x0CA, 202, 0, 0xF35u
#define PPUTLIMPL_UHEX_0x0C9u 201u, (0, C, 9), 0x0C9, 201, 0, 0xF36u
#define PPUTLIMPL_UHEX_0x0C8u 200u, (0, C, 8), 0x0C8, 200, 0, 0xF37u
#define PPUTLIMPL_UHEX_0x0C7u 199u, (0, C, 7), 0x0C7, 199, 0, 0xF38u
#define PPUTLIMPL_UHEX_0x0C6u 198u, (0, C, 6), 0x0C6, 198, 0, 0xF39u
#define PPUTLIMPL_UHEX_0x0C5u 197u, (0, C, 5), 0x0C5, 197, 0, 0xF3Au
#define PPUTLIMPL_UHEX_0x0C4u 196u, (0, C, 4), 0x0C4, 196, 0, 0xF3Bu
#define PPUTLIMPL_UHEX_0x0C3u 195u, (0, C, 3), 0x0C3, 195, 0, 0xF3Cu
#define PPUTLIMPL_UHEX_0x0C2u 194u, (0, C, 2), 0x0C2, 194, 0, 0xF3Du
#define PPUTLIMPL_UHEX_0x0C1u 193u, (0, C, 1), 0x0C1, 193, 0, 0xF3Eu
#define PPUTLIMPL_UHEX_0x0C0u 192u, (0, C, 0), 0x0C0, 192, 0, 0xF3Fu
#define PPUTLIMPL_UHEX_0x0BFu 191u, (0, B, F), 0x0BF, 191, 0, 0xF40u
#define PPUTLIMPL_UHEX_0x0BEu 190u, (0, B, E), 0x0BE, 190, 0, 0xF41u
#define PPUTLIMPL_UHEX_0x0BDu 189u, (0, B, D), 0x0BD, 189, 0, 0xF42u
#define PPUTLIMPL_UHEX_0x0BCu 188u, (0, B, C), 0x0BC, 188, 0, 0xF43u
#define PPUTLIMPL_UHEX_0x0BBu 187u, (0, B, B), 0x0BB, 187, 0, 0xF44u
#define PPUTLIMPL_UHEX_0x0BAu 186u, (0, B, A), 0x0BA, 186, 0, 0xF45u
#define PPUTLIMPL_UHEX_0x0B9u 185u, (0, B, 9), 0x0B9, 185, 0, 0xF46u
#define PPUTLIMPL_UHEX_0x0B8u 184u, (0, B, 8), 0x0B8, 184, 0, 0xF47u
#define PPUTLIMPL_UHEX_0x0B7u 183u, (0, B, 7), 0x0B7, 183, 0, 0xF48u
#define PPUTLIMPL_UHEX_0x0B6u 182u, (0, B, 6), 0x0B6, 182, 0, 0xF49u
#define PPUTLIMPL_UHEX_0x0B5u 181u, (0, B, 5), 0x0B5, 181, 0, 0xF4Au
#define PPUTLIMPL_UHEX_0x0B4u 180u, (0, B, 4), 0x0B4, 180, 0, 0xF4Bu
#define PPUTLIMPL_UHEX_0x0B3u 179u, (0, B, 3), 0x0B3, 179, 0, 0xF4Cu
#define PPUTLIMPL_UHEX_0x0B2u 178u, (0, B, 2), 0x0B2, 178, 0, 0xF4Du
#define PPUTLIMPL_UHEX_0x0B1u 177u, (0, B, 1), 0x0B1, 177, 0, 0xF4Eu
#define PPUTLIMPL_UHEX_0x0B0u 176u, (0, B, 0), 0x0B0, 176, 0, 0xF4Fu
#define PPUTLIMPL_UHEX_0x0AFu 175u, (0, A, F), 0x0AF, 175, 0, 0xF50u
#define PPUTLIMPL_UHEX_0x0AEu 174u, (0, A, E), 0x0AE, 174, 0, 0xF51u
#define PPUTLIMPL_UHEX_0x0ADu 173u, (0, A, D), 0x0AD, 173, 0, 0xF52u
#define PPUTLIMPL_UHEX_0x0ACu 172u, (0, A, C), 0x0AC, 172, 0, 0xF53u
#define PPUTLIMPL_UHEX_0x0ABu 171u, (0, A, B), 0x0AB, 171, 0, 0xF54u
#define PPUTLIMPL_UHEX_0x0AAu 170u, (0, A, A), 0x0AA, 170, 0, 0xF55u
#define PPUTLIMPL_UHEX_0x0A9u 169u, (0, A, 9), 0x0A9, 169, 0, 0xF56u
#define PPUTLIMPL_UHEX_0x0A8u 168u, (0, A, 8), 0x0A8, 168, 0, 0xF57u
#define PPUTLIMPL_UHEX_0x0A7u 167u, (0, A, 7), 0x0A7, 167, 0, 0xF58u
#define PPUTLIMPL_UHEX_0x0A6u 166u, (0, A, 6), 0x0A6, 166, 0, 0xF59u
#define PPUTLIMPL_UHEX_0x0A5u 165u, (0, A, 5), 0x0A5, 165, 0, 0xF5Au
#define PPUTLIMPL_UHEX_0x0A4u 164u, (0, A, 4), 0x0A4, 164, 0, 0xF5Bu
#define PPUTLIMPL_UHEX_0x0A3u 163u, (0, A, 3), 0x0A3, 163, 0, 0xF5Cu
#define PPUTLIMPL_UHEX_0x0A2u 162u, (0, A, 2), 0x0A2, 162, 0, 0xF5Du
#define PPUTLIMPL_UHEX_0x0A1u 161u, (0, A, 1), 0x0A1, 161, 0, 0xF5Eu
#define PPUTLIMPL_UHEX_0x0A0u 160u, (0, A, 0), 0x0A0, 160, 0, 0xF5Fu
#define PPUTLIMPL_UHEX_0x09Fu 159u, (0, 9, F), 0x09F, 159, 0, 0xF60u
#define PPUTLIMPL_UHEX_0x09Eu 158u, (0, 9, E), 0x09E, 158, 0, 0xF61u
#define PPUTLIMPL_UHEX_0x09Du 157u, (0, 9, D), 0x09D, 157, 0, 0xF62u
#define PPUTLIMPL_UHEX_0x09Cu 156u, (0, 9, C), 0x09C, 156, 0, 0xF63u
#define PPUTLIMPL_UHEX_0x09Bu 155u, (0, 9, B), 0x09B, 155, 0, 0xF64u
#define PPUTLIMPL_UHEX_0x09Au 154u, (0, 9, A), 0x09A, 154, 0, 0xF65u
#define PPUTLIMPL_UHEX_0x099u 153u, (0, 9, 9), 0x099, 153, 0, 0xF66u
#define PPUTLIMPL_UHEX_0x098u 152u, (0, 9, 8), 0x098, 152, 0, 0xF67u
#define PPUTLIMPL_UHEX_0x097u 151u, (0, 9, 7), 0x097, 151, 0, 0xF68u
#define PPUTLIMPL_UHEX_0x096u 150u, (0, 9, 6), 0x096, 150, 0, 0xF69u
#define PPUTLIMPL_UHEX_0x095u 149u, (0, 9, 5), 0x095, 149, 0, 0xF6Au
#define PPUTLIMPL_UHEX_0x094u 148u, (0, 9, 4), 0x094, 148, 0, 0xF6Bu
#define PPUTLIMPL_UHEX_0x093u 147u, (0, 9, 3), 0x093, 147, 0, 0xF6Cu
#define PPUTLIMPL_UHEX_0x092u 146u, (0, 9, 2), 0x092, 146, 0, 0xF6Du
#define PPUTLIMPL_UHEX_0x091u 145u, (0, 9, 1), 0x091, 145, 0, 0xF6Eu
#define PPUTLIMPL_UHEX_0x090u 144u, (0, 9, 0), 0x090, 144, 0, 0xF6Fu
#define PPUTLIMPL_UHEX_0x08Fu 143u, (0, 8, F), 0x08F, 143, 0, 0xF70u
#define PPUTLIMPL_UHEX_0x08Eu 142u, (0, 8, E), 0x08E, 142, 0, 0xF71u
#define PPUTLIMPL_UHEX_0x08Du 141u, (0, 8, D), 0x08D, 141, 0, 0xF72u
#define PPUTLIMPL_UHEX_0x08Cu 140u, (0, 8, C), 0x08C, 140, 0, 0xF73u
#define PPUTLIMPL_UHEX_0x08Bu 139u, (0, 8, B), 0x08B, 139, 0, 0xF74u
#define PPUTLIMPL_UHEX_0x08Au 138u, (0, 8, A), 0x08A, 138, 0, 0xF75u
#define PPUTLIMPL_UHEX_0x089u 137u, (0, 8, 9), 0x089, 137, 0, 0xF76u
#define PPUTLIMPL_UHEX_0x088u 136u, (0, 8, 8), 0x088, 136, 0, 0xF77u
#define PPUTLIMPL_UHEX_0x087u 135u, (0, 8, 7), 0x087, 135, 0, 0xF78u
#define PPUTLIMPL_UHEX_0x086u 134u, (0, 8, 6), 0x086, 134, 0, 0xF79u
#define PPUTLIMPL_UHEX_0x085u 133u, (0, 8, 5), 0x085, 133, 0, 0xF7Au
#define PPUTLIMPL_UHEX_0x084u 132u, (0, 8, 4), 0x084, 132, 0, 0xF7Bu
#define PPUTLIMPL_UHEX_0x083u 131u, (0, 8, 3), 0x083, 131, 0, 0xF7Cu
#define PPUTLIMPL_UHEX_0x082u 130u, (0, 8, 2), 0x082, 130, 0, 0xF7Du
#define PPUTLIMPL_UHEX_0x081u 129u, (0, 8, 1), 0x081, 129, 0, 0xF7Eu
#define PPUTLIMPL_UHEX_0x080u 128u, (0, 8, 0), 0x080, 128, 0, 0xF7Fu
#define PPUTLIMPL_UHEX_0x07Fu 127u, (0, 7, F), 0x07F, 127, 0, 0xF80u
#define PPUTLIMPL_UHEX_0x07Eu 126u, (0, 7, E), 0x07E, 126, 0, 0xF81u
#define PPUTLIMPL_UHEX_0x07Du 125u, (0, 7, D), 0x07D, 125, 0, 0xF82u
#define PPUTLIMPL_UHEX_0x07Cu 124u, (0, 7, C), 0x07C, 124, 0, 0xF83u
#define PPUTLIMPL_UHEX_0x07Bu 123u, (0, 7, B), 0x07B, 123, 0, 0xF84u
#define PPUTLIMPL_UHEX_0x07Au 122u, (0, 7, A), 0x07A, 122, 0, 0xF85u
#define PPUTLIMPL_UHEX_0x079u 121u, (0, 7, 9), 0x079, 121, 0, 0xF86u
#define PPUTLIMPL_UHEX_0x078u 120u, (0, 7, 8), 0x078, 120, 0, 0xF87u
#define PPUTLIMPL_UHEX_0x077u 119u, (0, 7, 7), 0x077, 119, 0, 0xF88u
#define PPUTLIMPL_UHEX_0x076u 118u, (0, 7, 6), 0x076, 118, 0, 0xF89u
#define PPUTLIMPL_UHEX_0x075u 117u, (0, 7, 5), 0x075, 117, 0, 0xF8Au
#define PPUTLIMPL_UHEX_0x074u 116u, (0, 7, 4), 0x074, 116, 0, 0xF8Bu
#define PPUTLIMPL_UHEX_0x073u 115u, (0, 7, 3), 0x073, 115, 0, 0xF8Cu
#define PPUTLIMPL_UHEX_0x072u 114u, (0, 7, 2), 0x072, 114, 0, 0xF8Du
#define PPUTLIMPL_UHEX_0x071u 113u, (0, 7, 1), 0x071, 113, 0, 0xF8Eu
#define PPUTLIMPL_UHEX_0x070u 112u, (0, 7, 0), 0x070, 112, 0, 0xF8Fu
#define PPUTLIMPL_UHEX_0x06Fu 111u, (0, 6, F), 0x06F, 111, 0, 0xF90u
#define PPUTLIMPL_UHEX_0x06Eu 110u, (0, 6, E), 0x06E, 110, 0, 0xF91u
#define PPUTLIMPL_UHEX_0x06Du 109u, (0, 6, D), 0x06D, 109, 0, 0xF92u
#define PPUTLIMPL_UHEX_0x06Cu 108u, (0, 6, C), 0x06C, 108, 0, 0xF93u
#define PPUTLIMPL_UHEX_0x06Bu 107u, (0, 6, B), 0x06B, 107, 0, 0xF94u
#define PPUTLIMPL_UHEX_0x06Au 106u, (0, 6, A), 0x06A, 106, 0, 0xF95u
#define PPUTLIMPL_UHEX_0x069u 105u, (0, 6, 9), 0x069, 105, 0, 0xF96u
#define PPUTLIMPL_UHEX_0x068u 104u, (0, 6, 8), 0x068, 104, 0, 0xF97u
#define PPUTLIMPL_UHEX_0x067u 103u, (0, 6, 7), 0x067, 103, 0, 0xF98u
#define PPUTLIMPL_UHEX_0x066u 102u, (0, 6, 6), 0x066, 102, 0, 0xF99u
#define PPUTLIMPL_UHEX_0x065u 101u, (0, 6, 5), 0x065, 101, 0, 0xF9Au
#define PPUTLIMPL_UHEX_0x064u 100u, (0, 6, 4), 0x064, 100, 0, 0xF9Bu
#define PPUTLIMPL_UHEX_0x063u 99u, (0, 6, 3), 0x063, 99, 0, 0xF9Cu
#define PPUTLIMPL_UHEX_0x062u 98u, (0, 6, 2), 0x062, 98, 0, 0xF9Du
#define PPUTLIMPL_UHEX_0x061u 97u, (0, 6, 1), 0x061, 97, 0, 0xF9Eu
#define PPUTLIMPL_UHEX_0x060u 96u, (0, 6, 0), 0x060, 96, 0, 0xF9Fu
#define PPUTLIMPL_UHEX_0x05Fu 95u, (0, 5, F), 0x05F, 95, 0, 0xFA0u
#define PPUTLIMPL_UHEX_0x05Eu 94u, (0, 5, E), 0x05E, 94, 0, 0xFA1u
#define PPUTLIMPL_UHEX_0x05Du 93u, (0, 5, D), 0x05D, 93, 0, 0xFA2u
#define PPUTLIMPL_UHEX_0x05Cu 92u, (0, 5, C), 0x05C, 92, 0, 0xFA3u
#define PPUTLIMPL_UHEX_0x05Bu 91u, (0, 5, B), 0x05B, 91, 0, 0xFA4u
#define PPUTLIMPL_UHEX_0x05Au 90u, (0, 5, A), 0x05A, 90, 0, 0xFA5u
#define PPUTLIMPL_UHEX_0x059u 89u, (0, 5, 9), 0x059, 89, 0, 0xFA6u
#define PPUTLIMPL_UHEX_0x058u 88u, (0, 5, 8), 0x058, 88, 0, 0xFA7u
#define PPUTLIMPL_UHEX_0x057u 87u, (0, 5, 7), 0x057, 87, 0, 0xFA8u
#define PPUTLIMPL_UHEX_0x056u 86u, (0, 5, 6), 0x056, 86, 0, 0xFA9u
#define PPUTLIMPL_UHEX_0x055u 85u, (0, 5, 5), 0x055, 85, 0, 0xFAAu
#define PPUTLIMPL_UHEX_0x054u 84u, (0, 5, 4), 0x054, 84, 0, 0xFABu
#define PPUTLIMPL_UHEX_0x053u 83u, (0, 5, 3), 0x053, 83, 0, 0xFACu
#define PPUTLIMPL_UHEX_0x052u 82u, (0, 5, 2), 0x052, 82, 0, 0xFADu
#define PPUTLIMPL_UHEX_0x051u 81u, (0, 5, 1), 0x051, 81, 0, 0xFAEu
#define PPUTLIMPL_UHEX_0x050u 80u, (0, 5, 0), 0x050, 80, 0, 0xFAFu
#define PPUTLIMPL_UHEX_0x04Fu 79u, (0, 4, F), 0x04F, 79, 0, 0xFB0u
#define PPUTLIMPL_UHEX_0x04Eu 78u, (0, 4, E), 0x04E, 78, 0, 0xFB1u
#define PPUTLIMPL_UHEX_0x04Du 77u, (0, 4, D), 0x04D, 77, 0, 0xFB2u
#define PPUTLIMPL_UHEX_0x04Cu 76u, (0, 4, C), 0x04C, 76, 0, 0xFB3u
#define PPUTLIMPL_UHEX_0x04Bu 75u, (0, 4, B), 0x04B, 75, 0, 0xFB4u
#define PPUTLIMPL_UHEX_0x04Au 74u, (0, 4, A), 0x04A, 74, 0, 0xFB5u
#define PPUTLIMPL_UHEX_0x049u 73u, (0, 4, 9), 0x049, 73, 0, 0xFB6u
#define PPUTLIMPL_UHEX_0x048u 72u, (0, 4, 8), 0x048, 72, 0, 0xFB7u
#define PPUTLIMPL_UHEX_0x047u 71u, (0, 4, 7), 0x047, 71, 0, 0xFB8u
#define PPUTLIMPL_UHEX_0x046u 70u, (0, 4, 6), 0x046, 70, 0, 0xFB9u
#define PPUTLIMPL_UHEX_0x045u 69u, (0, 4, 5), 0x045, 69, 0, 0xFBAu
#define PPUTLIMPL_UHEX_0x044u 68u, (0, 4, 4), 0x044, 68, 0, 0xFBBu
#define PPUTLIMPL_UHEX_0x043u 67u, (0, 4, 3), 0x043, 67, 0, 0xFBCu
#define PPUTLIMPL_UHEX_0x042u 66u, (0, 4, 2), 0x042, 66, 0, 0xFBDu
#define PPUTLIMPL_UHEX_0x041u 65u, (0, 4, 1), 0x041, 65, 0, 0xFBEu
#define PPUTLIMPL_UHEX_0x040u 64u, (0, 4, 0), 0x040, 64, 0, 0xFBFu
#define PPUTLIMPL_UHEX_0x03Fu 63u, (0, 3, F), 0x03F, 63, 0, 0xFC0u
#define PPUTLIMPL_UHEX_0x03Eu 62u, (0, 3, E), 0x03E, 62, 0, 0xFC1u
#define PPUTLIMPL_UHEX_0x03Du 61u, (0, 3, D), 0x03D, 61, 0, 0xFC2u
#define PPUTLIMPL_UHEX_0x03Cu 60u, (0, 3, C), 0x03C, 60, 0, 0xFC3u
#define PPUTLIMPL_UHEX_0x03Bu 59u, (0, 3, B), 0x03B, 59, 0, 0xFC4u
#define PPUTLIMPL_UHEX_0x03Au 58u, (0, 3, A), 0x03A, 58, 0, 0xFC5u
#define PPUTLIMPL_UHEX_0x039u 57u, (0, 3, 9), 0x039, 57, 0, 0xFC6u
#define PPUTLIMPL_UHEX_0x038u 56u, (0, 3, 8), 0x038, 56, 0, 0xFC7u
#define PPUTLIMPL_UHEX_0x037u 55u, (0, 3, 7), 0x037, 55, 0, 0xFC8u
#define PPUTLIMPL_UHEX_0x036u 54u, (0, 3, 6), 0x036, 54, 0, 0xFC9u
#define PPUTLIMPL_UHEX_0x035u 53u, (0, 3, 5), 0x035, 53, 0, 0xFCAu
#define PPUTLIMPL_UHEX_0x034u 52u, (0, 3, 4), 0x034, 52, 0, 0xFCBu
#define PPUTLIMPL_UHEX_0x033u 51u, (0, 3, 3), 0x033, 51, 0, 0xFCCu
#define PPUTLIMPL_UHEX_0x032u 50u, (0, 3, 2), 0x032, 50, 0, 0xFCDu
#define PPUTLIMPL_UHEX_0x031u 49u, (0, 3, 1), 0x031, 49, 0, 0xFCEu
#define PPUTLIMPL_UHEX_0x030u 48u, (0, 3, 0), 0x030, 48, 0, 0xFCFu
#define PPUTLIMPL_UHEX_0x02Fu 47u, (0, 2, F), 0x02F, 47, 0, 0xFD0u
#define PPUTLIMPL_UHEX_0x02Eu 46u, (0, 2, E), 0x02E, 46, 0, 0xFD1u
#define PPUTLIMPL_UHEX_0x02Du 45u, (0, 2, D), 0x02D, 45, 0, 0xFD2u
#define PPUTLIMPL_UHEX_0x02Cu 44u, (0, 2, C), 0x02C, 44, 0, 0xFD3u
#define PPUTLIMPL_UHEX_0x02Bu 43u, (0, 2, B), 0x02B, 43, 0, 0xFD4u
#define PPUTLIMPL_UHEX_0x02Au 42u, (0, 2, A), 0x02A, 42, 0, 0xFD5u
#define PPUTLIMPL_UHEX_0x029u 41u, (0, 2, 9), 0x029, 41, 0, 0xFD6u
#define PPUTLIMPL_UHEX_0x028u 40u, (0, 2, 8), 0x028, 40, 0, 0xFD7u
#define PPUTLIMPL_UHEX_0x027u 39u, (0, 2, 7), 0x027, 39, 0, 0xFD8u
#define PPUTLIMPL_UHEX_0x026u 38u, (0, 2, 6), 0x026, 38, 0, 0xFD9u
#define PPUTLIMPL_UHEX_0x025u 37u, (0, 2, 5), 0x025, 37, 0, 0xFDAu
#define PPUTLIMPL_UHEX_0x024u 36u, (0, 2, 4), 0x024, 36, 0, 0xFDBu
#define PPUTLIMPL_UHEX_0x023u 35u, (0, 2, 3), 0x023, 35, 0, 0xFDCu
#define PPUTLIMPL_UHEX_0x022u 34u, (0, 2, 2), 0x022, 34, 0, 0xFDDu
#define PPUTLIMPL_UHEX_0x021u 33u, (0, 2, 1), 0x021, 33, 0, 0xFDEu
#define PPUTLIMPL_UHEX_0x020u 32u, (0, 2, 0), 0x020, 32, 0, 0xFDFu
#define PPUTLIMPL_UHEX_0x01Fu 31u, (0, 1, F), 0x01F, 31, 0, 0xFE0u
#define PPUTLIMPL_UHEX_0x01Eu 30u, (0, 1, E), 0x01E, 30, 0, 0xFE1u
#define PPUTLIMPL_UHEX_0x01Du 29u, (0, 1, D), 0x01D, 29, 0, 0xFE2u
#define PPUTLIMPL_UHEX_0x01Cu 28u, (0, 1, C), 0x01C, 28, 0, 0xFE3u
#define PPUTLIMPL_UHEX_0x01Bu 27u, (0, 1, B), 0x01B, 27, 0, 0xFE4u
#define PPUTLIMPL_UHEX_0x01Au 26u, (0, 1, A), 0x01A, 26, 0, 0xFE5u
#define PPUTLIMPL_UHEX_0x019u 25u, (0, 1, 9), 0x019, 25, 0, 0xFE6u
#define PPUTLIMPL_UHEX_0x018u 24u, (0, 1, 8), 0x018, 24, 0, 0xFE7u
#define PPUTLIMPL_UHEX_0x017u 23u, (0, 1, 7), 0x017, 23, 0, 0xFE8u
#define PPUTLIMPL_UHEX_0x016u 22u, (0, 1, 6), 0x016, 22, 0, 0xFE9u
#define PPUTLIMPL_UHEX_0x015u 21u, (0, 1, 5), 0x015, 21, 0, 0xFEAu
#define PPUTLIMPL_UHEX_0x014u 20u, (0, 1, 4), 0x014, 20, 0, 0xFEBu
#define PPUTLIMPL_UHEX_0x013u 19u, (0, 1, 3), 0x013, 19, 0, 0xFECu
#define PPUTLIMPL_UHEX_0x012u 18u, (0, 1, 2), 0x012, 18, 0, 0xFEDu
#define PPUTLIMPL_UHEX_0x011u 17u, (0, 1, 1), 0x011, 17, 0, 0xFEEu
#define PPUTLIMPL_UHEX_0x010u 16u, (0, 1, 0), 0x010, 16, 0, 0xFEFu
#define PPUTLIMPL_UHEX_0x00Fu 15u, (0, 0, F), 0x00F, 15, 0, 0xFF0u
#define PPUTLIMPL_UHEX_0x00Eu 14u, (0, 0, E), 0x00E, 14, 0, 0xFF1u
#define PPUTLIMPL_UHEX_0x00Du 13u, (0, 0, D), 0x00D, 13, 0, 0xFF2u
#define PPUTLIMPL_UHEX_0x00Cu 12u, (0, 0, C), 0x00C, 12, 0, 0xFF3u
#define PPUTLIMPL_UHEX_0x00Bu 11u, (0, 0, B), 0x00B, 11, 0, 0xFF4u
#define PPUTLIMPL_UHEX_0x00Au 10u, (0, 0, A), 0x00A, 10, 0, 0xFF5u
#define PPUTLIMPL_UHEX_0x009u 9u, (0, 0, 9), 0x009, 9, 0, 0xFF6u
#define PPUTLIMPL_UHEX_0x008u 8u, (0, 0, 8), 0x008, 8, 0, 0xFF7u
#define PPUTLIMPL_UHEX_0x007u 7u, (0, 0, 7), 0x007, 7, 0, 0xFF8u
#define PPUTLIMPL_UHEX_0x006u 6u, (0, 0, 6), 0x006, 6, 0, 0xFF9u
#define PPUTLIMPL_UHEX_0x005u 5u, (0, 0, 5), 0x005, 5, 0, 0xFFAu
#define PPUTLIMPL_UHEX_0x004u 4u, (0, 0, 4), 0x004, 4, 0, 0xFFBu
#define PPUTLIMPL_UHEX_0x003u 3u, (0, 0, 3), 0x003, 3, 0, 0xFFCu
#define PPUTLIMPL_UHEX_0x002u 2u, (0, 0, 2), 0x002, 2, 0, 0xFFDu
#define PPUTLIMPL_UHEX_0x001u 1u, (0, 0, 1), 0x001, 1, 0, 0xFFEu
#define PPUTLIMPL_UHEX_0x000u 0u, (0, 0, 0), 0x000, 0, 0, 0xFFFu

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [impl.traits.arithhint]
/// -----------------------
/// [internal] two-operand arithmetic cast hint.
///
/// immediately concatenates args.
///
/// returns unsigned if either operand is unsigned,
/// decimal if either operand is decimal, utup if
/// both operands are utup, and hex otherwise.
#define PPUTLIMPL_ARITHHINT(                                                   \
    /* enum<IDEC|IHEX|UDEC|UHEX|UTUP>, enum<IDEC|IHEX|UDEC|UHEX|UTUP> */ a, b) \
  PPUTLIMPL_ARITHHINT_##a##b

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_ARITHHINT_UTUPUTUP UTUP
#define PPUTLIMPL_ARITHHINT_UTUPUHEX UHEX
#define PPUTLIMPL_ARITHHINT_UTUPUDEC UDEC
#define PPUTLIMPL_ARITHHINT_UTUPIHEX UHEX
#define PPUTLIMPL_ARITHHINT_UTUPIDEC UDEC
#define PPUTLIMPL_ARITHHINT_UHEXUTUP UHEX
#define PPUTLIMPL_ARITHHINT_UHEXUHEX UHEX
#define PPUTLIMPL_ARITHHINT_UHEXUDEC UDEC
#define PPUTLIMPL_ARITHHINT_UHEXIHEX UHEX
#define PPUTLIMPL_ARITHHINT_UHEXIDEC UDEC
#define PPUTLIMPL_ARITHHINT_UDECUTUP UDEC
#define PPUTLIMPL_ARITHHINT_UDECUHEX UDEC
#define PPUTLIMPL_ARITHHINT_UDECUDEC UDEC
#define PPUTLIMPL_ARITHHINT_UDECIHEX UDEC
#define PPUTLIMPL_ARITHHINT_UDECIDEC UDEC
#define PPUTLIMPL_ARITHHINT_IHEXUTUP UHEX
#define PPUTLIMPL_ARITHHINT_IHEXUHEX UHEX
#define PPUTLIMPL_ARITHHINT_IHEXUDEC UDEC
#define PPUTLIMPL_ARITHHINT_IHEXIHEX IHEX
#define PPUTLIMPL_ARITHHINT_IHEXIDEC IDEC
#define PPUTLIMPL_ARITHHINT_IDECUTUP UDEC
#define PPUTLIMPL_ARITHHINT_IDECUHEX UDEC
#define PPUTLIMPL_ARITHHINT_IDECUDEC UDEC
#define PPUTLIMPL_ARITHHINT_IDECIHEX IDEC
#define PPUTLIMPL_ARITHHINT_IDECIDEC IDEC

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

// vim: fdm=marker:fmr={{{,}}}

#endif

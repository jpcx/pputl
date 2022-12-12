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
//    many language constructs including  integers, recursion, queues, and    //
//    inheritable objects.                                                    //
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
//    The default build defines 12-bit words and an 8-bit size cap, which     //
//    complies with the following C++20 implementation limits [implimits]:    //
//                                                                            //
//     ‐ Macro identifiers simultaneously                                     //
//       defined in one translation unit: [65536].                            //
//     ‐ Parameters in one macro definition: [256].                           //
//     ‐ Arguments in one macro invocation: [256].                            //
//                                                                            //
//    Exceeding these limits is possible but depends on the preprocessor.     //
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
//    to (or interpreted as) its paramter type without losing information.    //
//                                                                            //
//     any: any potentially-empty, individual argument in __VA_ARGS__         //
//      ├╴none: an empty argument; an absence of pp-tokens                    //
//      └╴some: a non-empty argument; a presence of pp-tokens                 //
//         ├╴tup: a parenthesized item sequence [e.g. (a, b, c)]              //
//         │  └╴pair: a two-tuple [e.g. (foo, bar)]                           //
//         ├╴obj: an inheritable, name-addressable state container            //
//         │  ├╴err: an error message container for invoking a failure        //
//         │  ├╴dq:  a size/ofs-keyed, double-ended queue                     //
//         │  └╴pq:  a word-keyed priority queue                              //
//         └╴atom: a non-empty argument that is not a tup or obj              //
//            └╴word: a C++ integral expression matching a predefined set     //
//               ├╴int: 0x800-4096|0x801-4096|...|0|...|2046|2047             //
//               │  ├╴bool: 0|1                                               //
//               │  └╴ofs:  an int in the range of (-size_max, size_max)      //
//               └╴uint: 0u|1u|...|4094u|4095u                                //
//                  └╴size: a uint in the range of [0u, size_max]             //
//                                                                            //
//    FUNDAMENTALS                                                            //
//    ------------                                                            //
//                                                                            //
//    pputl errors  execute an invalid preprocessor operation by using the    //
//    concatenation operator (incorrectly) on a string error message.  All    //
//    errors produced by pputl functions include the macro name, a textual    //
//    description, and the primary expansion arguments.                       //
//                                                                            //
//    pputl API functions  are fully variadic and chainable,  meaning that    //
//    their arguments can be populated using macro expansion results. Args    //
//    must not grow, shrink, or change types after the primary expansion.     //
//                                                                            //
//    Negative ints are represented as valid C++ arithmetic expressions in    //
//    order to avoid post-processing:  pputl arithmetic operations  always    //
//    expand to values  that are usable in both preprocessor and C++ code.    //
//    When constructing  preprocessor identifiers  from negative integers,   ///
//    use int_name to extract the hex prefix before concatenation.          ////
//                                                                         /////
///////////////////////////////////////////////////////////////////////////// */

/// [config.bit_length]
/// -------------------
/// the number of bits that can be used to represent pputl integers.
/// see the readme code generation section to configure.
#define PTL_BIT_LENGTH /* -> int */ 8

/// [config.size_max]
/// -----------------
/// the maximum number of arguments bounded by the C++20 standard.
/// set to min(255, uint_max) unless built with cpp20_arglimit=false
/// (which sets size_max to uint_max).
#define PTL_SIZE_MAX /* -> size */ 255u

/// [config.build]
/// --------------
/// the build number of this pputl release (UTC ISO8601).
#define PTL_BUILD /* -> atom */ 20221212034339

/// [config.uint_max]
/// -----------------
/// the maximum value of a pputl unsigned int.
#define PTL_UINT_MAX /* -> uint */ 255u

// clang-format off

/// [config.int_min]
/// ----------------
/// the minimum value of a pputl signed int.
#define PTL_INT_MIN /* -> int */ 0x80-256

// clang-format on

/// [config.word_size]
/// ------------------
/// the number of hex digits used to represent pputl integers.
/// hex representations of integers are fixed at this length.
/// see the readme code generation section to configure.
#define PTL_WORD_SIZE /* -> int */ 2

/// [config.int_max]
/// ----------------
/// the maximum value of a pputl signed int.
#define PTL_INT_MAX /* -> int */ 127

/// [lang.esc]
/// ----------
/// escapes parentheses. identity function.
///
/// e.g. #define FOO (a, b, c)
///      PTL_ESC FOO // a, b, c
#define PTL_ESC(/* any... */...) /* -> any... */ __VA_ARGS__

/// [lang.first]
/// ------------
/// returns the first argument.
///
/// PTL_FIRST()       // <nothing>
/// PTL_FIRST(, )     // <nothing>
/// PTL_FIRST(a)      // a
/// PTL_FIRST(a, b)   // a
/// PTL_FIRST((a, b)) // (a, b)
#define PTL_FIRST(/* any... */...) /* -> any */ PPUTLIMPL_FIRST_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_FIRST_o(_, ...) _

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [lang.rest]
/// -----------
/// returns all but the first argument.
///
/// PTL_REST()                  // <nothing>
/// PTL_REST(, )                // <nothing>
/// PTL_REST(a)                 // <nothing>
/// PTL_REST(a, b)              // b
/// PTL_REST(a, b, c)           // b, c
/// PTL_REST(PTL_REST(a, b, c)) // c
/// PTL_REST(a, , )             // ,
/// PTL_REST(a, b, , )          // b, ,
#define PTL_REST(/* any... */...) /* -> any... */ PPUTLIMPL_REST_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_REST_o(_, ...) __VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [lang.eat]
/// ----------
/// eats arguments; return nothing.
///
/// PTL_EAT()    // <nothing>
/// PTL_EAT(foo) // <nothing>
#define PTL_EAT(/* any... */...) /* -> none */

/// [lang.cat]
/// ----------
/// concatenates two args. must be compatible with the ## operator.
///
/// PTL_CAT(foo, bar)          // foobar
/// PTL_CAT(foo, PTL_EAT(bar)) // foo
/// PTL_CAT(,)                 // <nothing>
#define PTL_CAT(/* a: any, b: any */...) /* -> any */ PPUTLIMPL_CAT_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_CAT_o(a, b) a##b

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [lang.str]
/// ----------
/// stringizes args.
///
/// PTL_STR()                  // ""
/// PTL_STR(foo, bar)          // "foo, bar"
/// PTL_STR(PTL_CAT(foo, bar)) // "foobar"
#define PTL_STR(/* any... */...) /* -> some */ PPUTLIMPL_STR_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_STR_o(...) #__VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [lang.default]
/// --------------
/// returns the first argument iff the rest of the arguments are nothing.
/// else, returns only the rest of the arguments.
///
/// PTL_DEFAULT()        // <nothing>
/// PTL_DEFAULT(a)       // a
/// PTL_DEFAULT(a,)      // a
/// PTL_DEFAULT(a, b)    // b
/// PTL_DEFAULT(a, b, c) // b, c
#define PTL_DEFAULT(/* default: any [, ...args: any] */...) /* -> any... */ \
  PPUTLIMPL_DEFAULT_o(__VA_ARGS__)(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_DEFAULT_o(_, ...)  PPUTLIMPL_DEFAULT_0##__VA_OPT__(1)
#define PPUTLIMPL_DEFAULT_01(_, ...) __VA_ARGS__
#define PPUTLIMPL_DEFAULT_0(_, ...)  _

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
#define PTL_TRIM(/* any... */...) /* -> any... */                              \
  PTL_CAT(PPUTLIMPL_TRIM_, PTL_CAT(PPUTLIMPL_TRIM_SEL(PTL_FIRST(__VA_ARGS__)), \
                                   PPUTLIMPL_TRIM_SEL(PTL_REST(__VA_ARGS__)))) \
  (__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_TRIM_SEL(...)     0##__VA_OPT__(1)
#define PPUTLIMPL_TRIM_0101(_, ...) _, __VA_ARGS__
#define PPUTLIMPL_TRIM_010(_, ...)  _
#define PPUTLIMPL_TRIM_001(_, ...)  __VA_ARGS__
#define PPUTLIMPL_TRIM_00(...)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.obj]
/// ----------
/// [extends some] an inheritable, name-addressable state container
///
/// PTL_OBJ((), ())        // PTL_OBJ((), ())
/// PTL_OBJ((FOO), ())     // PTL_OBJ((FOO), ())
/// PTL_OBJ((BAR), (a, b)) // PTL_OBJ((BAR), (a, b))
#define PTL_OBJ(/* scope: tup, state: tup */...) /* -> obj */ PTL_OBJ(__VA_ARGS__)

/// [traits.is_none]
/// ----------------
/// [extends is_any] checks if args is the literal nothing
///
/// PTL_IS_NONE()          // 1
/// PTL_IS_NONE(foo)       // 0
/// PTL_IS_NONE(foo, bar)  // 0
/// PTL_IS_NONE(PTL_ESC()) // 1
#define PTL_IS_NONE(/* any... */...) /* -> bool */ PPUTLIMPL_IS_NONE_0##__VA_OPT__(1)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_IS_NONE_01 0
#define PPUTLIMPL_IS_NONE_0  1

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_some]
/// ----------------
/// [extends is_any] checks if args is a singluar value
///
/// PTL_IS_SOME()          // 0
/// PTL_IS_SOME(foo)       // 1
/// PTL_IS_SOME(())        // 1
/// PTL_IS_SOME((a, b))    // 1
/// PTL_IS_SOME(foo, bar)  // 0
/// PTL_IS_SOME(PTL_ESC()) // 0
#define PTL_IS_SOME(/* any... */...) /* -> bool */ \
  PPUTLIMPL_IS_SOME_0##__VA_OPT__(1)(__VA_ARGS__.)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_IS_SOME_01(_, ...) PPUTLIMPL_IS_SOME_01_0##__VA_OPT__(1)
#define PPUTLIMPL_IS_SOME_01_01      0
#define PPUTLIMPL_IS_SOME_01_0       1
#define PPUTLIMPL_IS_SOME_0(_, ...)  0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_tup]
/// ---------------
/// [extends is_some] checks if args is a tuple
///
/// PTL_IS_TUP()       // 0
/// PTL_IS_TUP(1, 2)   // 0
/// PTL_IS_TUP(())     // 1
/// PTL_IS_TUP((1, 2)) // 1
#define PTL_IS_TUP(/* any... */...) /* -> bool */ \
  PTL_CAT(PPUTLIMPL_IS_TUP_, PTL_IS_SOME(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_IS_TUP_1           PPUTLIMPL_IS_TUP_1_o
#define PPUTLIMPL_IS_TUP_1_o(obj)    PTL_IS_NONE(PTL_EAT obj)
#define PPUTLIMPL_IS_TUP_0           PPUTLIMPL_IS_TUP_0_fail
#define PPUTLIMPL_IS_TUP_0_fail(...) 0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_any]
/// ---------------
/// checks if args is a single, potentially empty arg
///
/// PTL_IS_ANY()       // 1
/// PTL_IS_ANY(foo)    // 1
/// PTL_IS_ANY((a, b)) // 1
/// PTL_IS_ANY(a, b)   // 0
/// PTL_IS_ANY(, )     // 0
/// PTL_IS_ANY(, , )   // 0
#define PTL_IS_ANY(/* any... */...) /* -> bool */ PPUTLIMPL_IS_ANY_o(__VA_ARGS__.)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_IS_ANY_o(_, ...) PPUTLIMPL_IS_ANY_o_0##__VA_OPT__(1)
#define PPUTLIMPL_IS_ANY_o_01      0
#define PPUTLIMPL_IS_ANY_o_0       1

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

// vim: fdm=marker:fmr={{{,}}}

#endif

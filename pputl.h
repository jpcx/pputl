#ifndef PPUTL_H_INCLUDED
#define PPUTL_H_INCLUDED
/* /////////////////////////////////////////////////////////////////////////////
//                          __    ___                                     `/////
//                         /\ \__/\_ \                                     `////
//   _____   _____   __  __\ \ ,_\//\ \                                     `///
//  /\ '__`\/\ '__`\/\ \/\ \\ \ \/ \ \ \                                     `//
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
//    many language constructs  including a type system, integers, errors,    //
//    recursion, namespaces, polymorphic objects, and datastructures.         //
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
//    pputl is a standalone single-header library. include pputl.h to use.    //
//    A C++20-compliant preprocessor is required.                             //
//                                                                            //
//    pputl  is completely generated and tested by a custom C++ framework.    //
//    See the codegen/ folder for the full source.                            //
//                                                                            //
//    Various settings including word size and naming rules may be changed    //
//    by modifying the head of codegen/codegen.h and running `make`.          //
//                                                                            //
//    The default build defines 12-bit words and an 8-bit size cap,  which    //
//    complies with the following C++20 implementation limits [implimits]:    //
//                                                                            //
//     ‐ Macro identifiers simultaneously                                     //
//       defined in one translation unit: [65536].                            //
//     ‐ Parameters in one macro definition: [256].                           //
//     ‐ Arguments in one macro invocation: [256].                            //
//                                                                            //
//    Exceeding these limits  is possible but depends on the preprocessor.    //
//    The size cap is bounded by the maximum number of parameters, and the    //
//    word size is directly proportional to the number of macros defined.     //
//                                                                            //
//    pputl has been tested with:                                             //
//                                                                            //
//      gcc   11.2.1                                                          //
//      clang 13.0.0                                                          //
//      MSVC  2022 (requires /Zc:preprocessor)                                //
//                                                                            //
//    Run `make test` to validate on your system.                             //
//                                                                            //
//    TYPES                                                                   //
//    -----                                                                   //
//                                                                            //
//    pputl makes extensive use of duck-typing  for control flow and error    //
//    management.  pputl types are essentially pairs of functions: one for    //
//    traits identification and another for construction and assertion.       //
//                                                                            //
//    API functions are strictly documented using this type system. Inputs    //
//    are validated by invoking the associated constructor or through some    //
//    other form of inference. An argument is valid if it can be converted    //
//    to (or interpreted as) its parameter without losing information.        //
//                                                                            //
//     any: any potentially-empty, individual argument in __VA_ARGS__         //
//      ├╴none: an empty argument; an absence of pp-tokens                    //
//      └╴some: a non-empty argument; a presence of pp-tokens                 //
//         ├╴tup: a parenthesized item sequence [e.g. (a, b, c)]              //
//         │  └╴pair: a two-tuple [e.g. (foo, bar)]                           //
//         ├╴sym: a global or namespace-qualified equality-comparable name    //
//         │  └╴num: a builtin totally-ordered arithmetic sym                 //
//         │     ├╴bool: false|true                                           //
//         │     ├╴hex:  0x0u|0x1u|...|0xEu|0xFu                              //
//         │     ├╴size: 0x00u|0x01u|...|0xFEu|0xFFu                          //
//         │     ├╴uint: 0u|1u|...|4094u|4095u                                //
//         │     └╴int:  compl(0x7FF)|compl(0x7FE)|...|0|...|2046|2047        //
//         └╴obj: a polymorphic sym-addressable mutable state container       //
//            ├╴err:   an error message container for lang.fail               //
//            ├╴vec:   a resizable array                                      //
//            ├╴map:   a mapping of equality-comparable keys to any           //
//            ├╴pq:    a priority queue                                       //
//            ├╴queue: a FIFO queue                                           //
//            └╴stack: a LIFO queue                                           //
//                                                                            //
//    NOTES                                                                   //
//    -----                                                                   //
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
//    The sym type lays the foundation for arithmetic literals, obj member    //
//    access, and negative integers.  Since arithmetic symbols cannot form    //
//    identifiers,  the C++ compl operator is used to ensure that negative    //
//    ints can be parsed by the library  and have the same meaning in both    //
//    the preprocessor and C++ code. When using an int or num to construct   ///
//    an identifier, use lang.cat (which converts ints < 0 to 12-bit hex).  ////
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
#define PTL_BUILD /* -> atom */ 20221217005741

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
/// converts args to a C string literal.
///
/// C strings cannot be detected by macro functions or used
/// in any meaningful way (except as generic arguments), as
/// they are completely incompatible with the concatenation
/// operator. as such, their type is considered `some`.
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
/// [extends some] a named, polymorphic, atom-addressable state container.
///
/// objects have the form of NAME(...state) and can be created either directly
/// or using a directly-recursive constructor (direct recursion freezes tokens).
///
/// example:
///
///   // obj types must be properly declared as a pputl symbol and object.
///   // object declarations represent the heirarchy and root obj construction.
///   #define PTL_SYM_POS2D_IS_POS2D (POS2D)
///   #define PTL_OBJ_POS2D(...) PTL_OBJ(__VA_ARGS__)
///
///   // member names are atoms defined as follows:
///   #define PTL_IDX_POS2D_Y 0 // atom Y refers to POS2D idx 0
///   #define PTL_IDX_POS2D_X 1 // atom X refers to POS2D idx 1
///
///   // a constructor that handles copies, default, and member initialization
///   #define POS2D(...)
///     PTL_IF(
///         PTL_ISA(__VA_ARGS__, POS2D), POS2D_COPY,
///         PTL_IF(PTL_IS_NONE(__VA_ARGS__), POS2D_DEFAULT, POS2D_ARGS))
///     (__VA_ARGS__)
///
///   // explicitly retrieving Y and X instead of simply
///   // returning o, as it may be a derived object
///   #define POS2D_COPY(o)      PTL_COPY(PTL_COPY(POS2D_DEFAULT(), o, Y), X)
///   #define POS2D_DEFAULT(...) POS2D(0, 0)
///   #define POS2D_ARGS(y, x)   POS2D(y, x)
///
///   // deriving from POS2D. note: derived comes before base in heirarchy
///   #define PTL_SYM_POS3D_IS_POS3D (POS3D, POS2D)
///   #define PTL_OBJ_POS3D(...) PTL_OBJ(__VA_ARGS__)
///
///   // using a new index for this member. reusing an index overrides base.
///   #define PTL_IDX_POS3D_Z 2
///
///   // this constructor also handles cast from base
///   #define POS3D(...)
///     PTL_IF(PTL_ISA(__VA_ARGS__, POS3D), POS3D_COPY,
///            PTL_IF(PTL_ISA(__VA_ARGS__, POS2D), POS3D_BASE,
///                   PTL_IF(PTL_IS_NONE(__VA_ARGS__), POS2D_DEFAULT,
///                          POS2D_ARGS)))
///     (__VA_ARGS__)
///
///   #define POS3D_COPY(o)
///     PTL_COPY(PTL_COPY(PTL_COPY(POS3D_DEFAULT(), o, Y), X), Z)
///   #define POS3D_BASE(o)       POS3D(PTL_GET(o, Y), PTL_GET(o, X), 0)
///   #define POS3D_DEFAULT(...)  POS3D(0, 0, 0)
///   #define POS3D_ARGS(y, x, z) POS3D(y, x, z)
///
///   // elements from objects can be indexed using PTL_GET and PTL_SET
///   #define POS   POS3D(3, 5, 4)
///   #define MOVED PTL_SET(FOO, X, PTL_INC(PTL_GET(FOO, X)))
#define PTL_OBJ(/* ...state: any... */...) /* -> obj */ PTL_OBJ(__VA_ARGS__)

/// [traits.is_none]
/// ----------------
/// [extends is_any] checks if args is the literal nothing
///
/// PTL_IS_NONE()          // true
/// PTL_IS_NONE(foo)       // false
/// PTL_IS_NONE(foo, bar)  // false
/// PTL_IS_NONE(PTL_ESC()) // true
#define PTL_IS_NONE(/* any... */...) /* -> bool */ PPUTLIMPL_IS_NONE_0##__VA_OPT__(1)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_IS_NONE_01 false
#define PPUTLIMPL_IS_NONE_0  true

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_some]
/// ----------------
/// [extends is_any] checks if args is a singluar value
///
/// PTL_IS_SOME()          // false
/// PTL_IS_SOME(foo)       // true
/// PTL_IS_SOME(())        // true
/// PTL_IS_SOME((a, b))    // true
/// PTL_IS_SOME(foo, bar)  // false
/// PTL_IS_SOME(PTL_ESC()) // false
#define PTL_IS_SOME(/* any... */...) /* -> bool */ \
  PPUTLIMPL_IS_SOME_0##__VA_OPT__(1)(__VA_ARGS__.)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_IS_SOME_01(_, ...) PPUTLIMPL_IS_SOME_01_0##__VA_OPT__(1)
#define PPUTLIMPL_IS_SOME_01_01      false
#define PPUTLIMPL_IS_SOME_01_0       true
#define PPUTLIMPL_IS_SOME_0(_, ...)  false

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_tup]
/// ---------------
/// [extends is_some] checks if args is a tuple
///
/// PTL_IS_TUP()       // false
/// PTL_IS_TUP(1, 2)   // false
/// PTL_IS_TUP(())     // true
/// PTL_IS_TUP((1, 2)) // true
#define PTL_IS_TUP(/* any... */...) /* -> bool */ \
  PTL_CAT(PPUTLIMPL_IS_TUP_, PTL_IS_SOME(__VA_ARGS__))(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_IS_TUP_true            PPUTLIMPL_IS_TUP_true_o
#define PPUTLIMPL_IS_TUP_true_o(obj)     PTL_IS_NONE(PTL_EAT obj)
#define PPUTLIMPL_IS_TUP_false           PPUTLIMPL_IS_TUP_false_fail
#define PPUTLIMPL_IS_TUP_false_fail(...) false

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.sym]
/// ----------
/// [extends some] a global or namespace-qualified equality-comparable name.
///
/// syms are equality-comparable names that point to static storage.
/// a sym can either be declared globally or wrapped in a namespace.
///
/// global syms match /[\w\d_]+/ and are defined as follows:
///
///   #define PTL_SYM_<name>_IS_<name> (<sym traits...>)
///
/// namespaced syms match /[\w\d_]+\([\w\d_]+\)/ and are defined as follows:
///
///   #define PTL_SYM_<ns>(name)              (<ns>, name)
///   #define PTL_SYM_<ns>_<name1>_IS_<name1> (<sym traits...>)
///   #define PTL_SYM_<ns>_<name2>_IS_<name2> (<sym traits...>)
///   ...
///
/// the sym type lays the foundation for pputl artihmetic literals,
/// object member access, and negative integers.  negative integers
/// cannot be represented using arithmetic symbols  and instead use
/// C++ compl expressions  that can be parsed by the library  while
/// retaining the same meaning in both the preprocessor and C++. In
/// those cases, the namespace is compl and name is an integer.
#define PTL_SYM(/* any... */...) /* -> sym */ __VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

// clang-format off
#define PPUTLIMPL_SYM_compl(ihex) (compl, ihex)
#define PPUTLIMPL_SYM_compl_0x00_IS_0x00 (compl(0x00), -1),
#define PPUTLIMPL_SYM_compl_0x01_IS_0x01 (compl(0x01), -2),
#define PPUTLIMPL_SYM_compl_0x02_IS_0x02 (compl(0x02), -3),
#define PPUTLIMPL_SYM_compl_0x03_IS_0x03 (compl(0x03), -4),
#define PPUTLIMPL_SYM_compl_0x04_IS_0x04 (compl(0x04), -5),
#define PPUTLIMPL_SYM_compl_0x05_IS_0x05 (compl(0x05), -6),
#define PPUTLIMPL_SYM_compl_0x06_IS_0x06 (compl(0x06), -7),
#define PPUTLIMPL_SYM_compl_0x07_IS_0x07 (compl(0x07), -8),
#define PPUTLIMPL_SYM_compl_0x08_IS_0x08 (compl(0x08), -9),
#define PPUTLIMPL_SYM_compl_0x09_IS_0x09 (compl(0x09), -10),
#define PPUTLIMPL_SYM_compl_0x0A_IS_0x0A (compl(0x0A), -11),
#define PPUTLIMPL_SYM_compl_0x0B_IS_0x0B (compl(0x0B), -12),
#define PPUTLIMPL_SYM_compl_0x0C_IS_0x0C (compl(0x0C), -13),
#define PPUTLIMPL_SYM_compl_0x0D_IS_0x0D (compl(0x0D), -14),
#define PPUTLIMPL_SYM_compl_0x0E_IS_0x0E (compl(0x0E), -15),
#define PPUTLIMPL_SYM_compl_0x0F_IS_0x0F (compl(0x0F), -16),
#define PPUTLIMPL_SYM_compl_0x10_IS_0x10 (compl(0x10), -17),
#define PPUTLIMPL_SYM_compl_0x11_IS_0x11 (compl(0x11), -18),
#define PPUTLIMPL_SYM_compl_0x12_IS_0x12 (compl(0x12), -19),
#define PPUTLIMPL_SYM_compl_0x13_IS_0x13 (compl(0x13), -20),
#define PPUTLIMPL_SYM_compl_0x14_IS_0x14 (compl(0x14), -21),
#define PPUTLIMPL_SYM_compl_0x15_IS_0x15 (compl(0x15), -22),
#define PPUTLIMPL_SYM_compl_0x16_IS_0x16 (compl(0x16), -23),
#define PPUTLIMPL_SYM_compl_0x17_IS_0x17 (compl(0x17), -24),
#define PPUTLIMPL_SYM_compl_0x18_IS_0x18 (compl(0x18), -25),
#define PPUTLIMPL_SYM_compl_0x19_IS_0x19 (compl(0x19), -26),
#define PPUTLIMPL_SYM_compl_0x1A_IS_0x1A (compl(0x1A), -27),
#define PPUTLIMPL_SYM_compl_0x1B_IS_0x1B (compl(0x1B), -28),
#define PPUTLIMPL_SYM_compl_0x1C_IS_0x1C (compl(0x1C), -29),
#define PPUTLIMPL_SYM_compl_0x1D_IS_0x1D (compl(0x1D), -30),
#define PPUTLIMPL_SYM_compl_0x1E_IS_0x1E (compl(0x1E), -31),
#define PPUTLIMPL_SYM_compl_0x1F_IS_0x1F (compl(0x1F), -32),
#define PPUTLIMPL_SYM_compl_0x20_IS_0x20 (compl(0x20), -33),
#define PPUTLIMPL_SYM_compl_0x21_IS_0x21 (compl(0x21), -34),
#define PPUTLIMPL_SYM_compl_0x22_IS_0x22 (compl(0x22), -35),
#define PPUTLIMPL_SYM_compl_0x23_IS_0x23 (compl(0x23), -36),
#define PPUTLIMPL_SYM_compl_0x24_IS_0x24 (compl(0x24), -37),
#define PPUTLIMPL_SYM_compl_0x25_IS_0x25 (compl(0x25), -38),
#define PPUTLIMPL_SYM_compl_0x26_IS_0x26 (compl(0x26), -39),
#define PPUTLIMPL_SYM_compl_0x27_IS_0x27 (compl(0x27), -40),
#define PPUTLIMPL_SYM_compl_0x28_IS_0x28 (compl(0x28), -41),
#define PPUTLIMPL_SYM_compl_0x29_IS_0x29 (compl(0x29), -42),
#define PPUTLIMPL_SYM_compl_0x2A_IS_0x2A (compl(0x2A), -43),
#define PPUTLIMPL_SYM_compl_0x2B_IS_0x2B (compl(0x2B), -44),
#define PPUTLIMPL_SYM_compl_0x2C_IS_0x2C (compl(0x2C), -45),
#define PPUTLIMPL_SYM_compl_0x2D_IS_0x2D (compl(0x2D), -46),
#define PPUTLIMPL_SYM_compl_0x2E_IS_0x2E (compl(0x2E), -47),
#define PPUTLIMPL_SYM_compl_0x2F_IS_0x2F (compl(0x2F), -48),
#define PPUTLIMPL_SYM_compl_0x30_IS_0x30 (compl(0x30), -49),
#define PPUTLIMPL_SYM_compl_0x31_IS_0x31 (compl(0x31), -50),
#define PPUTLIMPL_SYM_compl_0x32_IS_0x32 (compl(0x32), -51),
#define PPUTLIMPL_SYM_compl_0x33_IS_0x33 (compl(0x33), -52),
#define PPUTLIMPL_SYM_compl_0x34_IS_0x34 (compl(0x34), -53),
#define PPUTLIMPL_SYM_compl_0x35_IS_0x35 (compl(0x35), -54),
#define PPUTLIMPL_SYM_compl_0x36_IS_0x36 (compl(0x36), -55),
#define PPUTLIMPL_SYM_compl_0x37_IS_0x37 (compl(0x37), -56),
#define PPUTLIMPL_SYM_compl_0x38_IS_0x38 (compl(0x38), -57),
#define PPUTLIMPL_SYM_compl_0x39_IS_0x39 (compl(0x39), -58),
#define PPUTLIMPL_SYM_compl_0x3A_IS_0x3A (compl(0x3A), -59),
#define PPUTLIMPL_SYM_compl_0x3B_IS_0x3B (compl(0x3B), -60),
#define PPUTLIMPL_SYM_compl_0x3C_IS_0x3C (compl(0x3C), -61),
#define PPUTLIMPL_SYM_compl_0x3D_IS_0x3D (compl(0x3D), -62),
#define PPUTLIMPL_SYM_compl_0x3E_IS_0x3E (compl(0x3E), -63),
#define PPUTLIMPL_SYM_compl_0x3F_IS_0x3F (compl(0x3F), -64),
#define PPUTLIMPL_SYM_compl_0x40_IS_0x40 (compl(0x40), -65),
#define PPUTLIMPL_SYM_compl_0x41_IS_0x41 (compl(0x41), -66),
#define PPUTLIMPL_SYM_compl_0x42_IS_0x42 (compl(0x42), -67),
#define PPUTLIMPL_SYM_compl_0x43_IS_0x43 (compl(0x43), -68),
#define PPUTLIMPL_SYM_compl_0x44_IS_0x44 (compl(0x44), -69),
#define PPUTLIMPL_SYM_compl_0x45_IS_0x45 (compl(0x45), -70),
#define PPUTLIMPL_SYM_compl_0x46_IS_0x46 (compl(0x46), -71),
#define PPUTLIMPL_SYM_compl_0x47_IS_0x47 (compl(0x47), -72),
#define PPUTLIMPL_SYM_compl_0x48_IS_0x48 (compl(0x48), -73),
#define PPUTLIMPL_SYM_compl_0x49_IS_0x49 (compl(0x49), -74),
#define PPUTLIMPL_SYM_compl_0x4A_IS_0x4A (compl(0x4A), -75),
#define PPUTLIMPL_SYM_compl_0x4B_IS_0x4B (compl(0x4B), -76),
#define PPUTLIMPL_SYM_compl_0x4C_IS_0x4C (compl(0x4C), -77),
#define PPUTLIMPL_SYM_compl_0x4D_IS_0x4D (compl(0x4D), -78),
#define PPUTLIMPL_SYM_compl_0x4E_IS_0x4E (compl(0x4E), -79),
#define PPUTLIMPL_SYM_compl_0x4F_IS_0x4F (compl(0x4F), -80),
#define PPUTLIMPL_SYM_compl_0x50_IS_0x50 (compl(0x50), -81),
#define PPUTLIMPL_SYM_compl_0x51_IS_0x51 (compl(0x51), -82),
#define PPUTLIMPL_SYM_compl_0x52_IS_0x52 (compl(0x52), -83),
#define PPUTLIMPL_SYM_compl_0x53_IS_0x53 (compl(0x53), -84),
#define PPUTLIMPL_SYM_compl_0x54_IS_0x54 (compl(0x54), -85),
#define PPUTLIMPL_SYM_compl_0x55_IS_0x55 (compl(0x55), -86),
#define PPUTLIMPL_SYM_compl_0x56_IS_0x56 (compl(0x56), -87),
#define PPUTLIMPL_SYM_compl_0x57_IS_0x57 (compl(0x57), -88),
#define PPUTLIMPL_SYM_compl_0x58_IS_0x58 (compl(0x58), -89),
#define PPUTLIMPL_SYM_compl_0x59_IS_0x59 (compl(0x59), -90),
#define PPUTLIMPL_SYM_compl_0x5A_IS_0x5A (compl(0x5A), -91),
#define PPUTLIMPL_SYM_compl_0x5B_IS_0x5B (compl(0x5B), -92),
#define PPUTLIMPL_SYM_compl_0x5C_IS_0x5C (compl(0x5C), -93),
#define PPUTLIMPL_SYM_compl_0x5D_IS_0x5D (compl(0x5D), -94),
#define PPUTLIMPL_SYM_compl_0x5E_IS_0x5E (compl(0x5E), -95),
#define PPUTLIMPL_SYM_compl_0x5F_IS_0x5F (compl(0x5F), -96),
#define PPUTLIMPL_SYM_compl_0x60_IS_0x60 (compl(0x60), -97),
#define PPUTLIMPL_SYM_compl_0x61_IS_0x61 (compl(0x61), -98),
#define PPUTLIMPL_SYM_compl_0x62_IS_0x62 (compl(0x62), -99),
#define PPUTLIMPL_SYM_compl_0x63_IS_0x63 (compl(0x63), -100),
#define PPUTLIMPL_SYM_compl_0x64_IS_0x64 (compl(0x64), -101),
#define PPUTLIMPL_SYM_compl_0x65_IS_0x65 (compl(0x65), -102),
#define PPUTLIMPL_SYM_compl_0x66_IS_0x66 (compl(0x66), -103),
#define PPUTLIMPL_SYM_compl_0x67_IS_0x67 (compl(0x67), -104),
#define PPUTLIMPL_SYM_compl_0x68_IS_0x68 (compl(0x68), -105),
#define PPUTLIMPL_SYM_compl_0x69_IS_0x69 (compl(0x69), -106),
#define PPUTLIMPL_SYM_compl_0x6A_IS_0x6A (compl(0x6A), -107),
#define PPUTLIMPL_SYM_compl_0x6B_IS_0x6B (compl(0x6B), -108),
#define PPUTLIMPL_SYM_compl_0x6C_IS_0x6C (compl(0x6C), -109),
#define PPUTLIMPL_SYM_compl_0x6D_IS_0x6D (compl(0x6D), -110),
#define PPUTLIMPL_SYM_compl_0x6E_IS_0x6E (compl(0x6E), -111),
#define PPUTLIMPL_SYM_compl_0x6F_IS_0x6F (compl(0x6F), -112),
#define PPUTLIMPL_SYM_compl_0x70_IS_0x70 (compl(0x70), -113),
#define PPUTLIMPL_SYM_compl_0x71_IS_0x71 (compl(0x71), -114),
#define PPUTLIMPL_SYM_compl_0x72_IS_0x72 (compl(0x72), -115),
#define PPUTLIMPL_SYM_compl_0x73_IS_0x73 (compl(0x73), -116),
#define PPUTLIMPL_SYM_compl_0x74_IS_0x74 (compl(0x74), -117),
#define PPUTLIMPL_SYM_compl_0x75_IS_0x75 (compl(0x75), -118),
#define PPUTLIMPL_SYM_compl_0x76_IS_0x76 (compl(0x76), -119),
#define PPUTLIMPL_SYM_compl_0x77_IS_0x77 (compl(0x77), -120),
#define PPUTLIMPL_SYM_compl_0x78_IS_0x78 (compl(0x78), -121),
#define PPUTLIMPL_SYM_compl_0x79_IS_0x79 (compl(0x79), -122),
#define PPUTLIMPL_SYM_compl_0x7A_IS_0x7A (compl(0x7A), -123),
#define PPUTLIMPL_SYM_compl_0x7B_IS_0x7B (compl(0x7B), -124),
#define PPUTLIMPL_SYM_compl_0x7C_IS_0x7C (compl(0x7C), -125),
#define PPUTLIMPL_SYM_compl_0x7D_IS_0x7D (compl(0x7D), -126),
#define PPUTLIMPL_SYM_compl_0x7E_IS_0x7E (compl(0x7E), -127),
#define PPUTLIMPL_SYM_compl_0x7F_IS_0x7F (compl(0x7F), -128),
#define PPUTLIMPL_SYM_127_IS_127 (127, 127)
#define PPUTLIMPL_SYM_126_IS_126 (126, 126)
#define PPUTLIMPL_SYM_125_IS_125 (125, 125)
#define PPUTLIMPL_SYM_124_IS_124 (124, 124)
#define PPUTLIMPL_SYM_123_IS_123 (123, 123)
#define PPUTLIMPL_SYM_122_IS_122 (122, 122)
#define PPUTLIMPL_SYM_121_IS_121 (121, 121)
#define PPUTLIMPL_SYM_120_IS_120 (120, 120)
#define PPUTLIMPL_SYM_119_IS_119 (119, 119)
#define PPUTLIMPL_SYM_118_IS_118 (118, 118)
#define PPUTLIMPL_SYM_117_IS_117 (117, 117)
#define PPUTLIMPL_SYM_116_IS_116 (116, 116)
#define PPUTLIMPL_SYM_115_IS_115 (115, 115)
#define PPUTLIMPL_SYM_114_IS_114 (114, 114)
#define PPUTLIMPL_SYM_113_IS_113 (113, 113)
#define PPUTLIMPL_SYM_112_IS_112 (112, 112)
#define PPUTLIMPL_SYM_111_IS_111 (111, 111)
#define PPUTLIMPL_SYM_110_IS_110 (110, 110)
#define PPUTLIMPL_SYM_109_IS_109 (109, 109)
#define PPUTLIMPL_SYM_108_IS_108 (108, 108)
#define PPUTLIMPL_SYM_107_IS_107 (107, 107)
#define PPUTLIMPL_SYM_106_IS_106 (106, 106)
#define PPUTLIMPL_SYM_105_IS_105 (105, 105)
#define PPUTLIMPL_SYM_104_IS_104 (104, 104)
#define PPUTLIMPL_SYM_103_IS_103 (103, 103)
#define PPUTLIMPL_SYM_102_IS_102 (102, 102)
#define PPUTLIMPL_SYM_101_IS_101 (101, 101)
#define PPUTLIMPL_SYM_100_IS_100 (100, 100)
#define PPUTLIMPL_SYM_99_IS_99 (99, 99)
#define PPUTLIMPL_SYM_98_IS_98 (98, 98)
#define PPUTLIMPL_SYM_97_IS_97 (97, 97)
#define PPUTLIMPL_SYM_96_IS_96 (96, 96)
#define PPUTLIMPL_SYM_95_IS_95 (95, 95)
#define PPUTLIMPL_SYM_94_IS_94 (94, 94)
#define PPUTLIMPL_SYM_93_IS_93 (93, 93)
#define PPUTLIMPL_SYM_92_IS_92 (92, 92)
#define PPUTLIMPL_SYM_91_IS_91 (91, 91)
#define PPUTLIMPL_SYM_90_IS_90 (90, 90)
#define PPUTLIMPL_SYM_89_IS_89 (89, 89)
#define PPUTLIMPL_SYM_88_IS_88 (88, 88)
#define PPUTLIMPL_SYM_87_IS_87 (87, 87)
#define PPUTLIMPL_SYM_86_IS_86 (86, 86)
#define PPUTLIMPL_SYM_85_IS_85 (85, 85)
#define PPUTLIMPL_SYM_84_IS_84 (84, 84)
#define PPUTLIMPL_SYM_83_IS_83 (83, 83)
#define PPUTLIMPL_SYM_82_IS_82 (82, 82)
#define PPUTLIMPL_SYM_81_IS_81 (81, 81)
#define PPUTLIMPL_SYM_80_IS_80 (80, 80)
#define PPUTLIMPL_SYM_79_IS_79 (79, 79)
#define PPUTLIMPL_SYM_78_IS_78 (78, 78)
#define PPUTLIMPL_SYM_77_IS_77 (77, 77)
#define PPUTLIMPL_SYM_76_IS_76 (76, 76)
#define PPUTLIMPL_SYM_75_IS_75 (75, 75)
#define PPUTLIMPL_SYM_74_IS_74 (74, 74)
#define PPUTLIMPL_SYM_73_IS_73 (73, 73)
#define PPUTLIMPL_SYM_72_IS_72 (72, 72)
#define PPUTLIMPL_SYM_71_IS_71 (71, 71)
#define PPUTLIMPL_SYM_70_IS_70 (70, 70)
#define PPUTLIMPL_SYM_69_IS_69 (69, 69)
#define PPUTLIMPL_SYM_68_IS_68 (68, 68)
#define PPUTLIMPL_SYM_67_IS_67 (67, 67)
#define PPUTLIMPL_SYM_66_IS_66 (66, 66)
#define PPUTLIMPL_SYM_65_IS_65 (65, 65)
#define PPUTLIMPL_SYM_64_IS_64 (64, 64)
#define PPUTLIMPL_SYM_63_IS_63 (63, 63)
#define PPUTLIMPL_SYM_62_IS_62 (62, 62)
#define PPUTLIMPL_SYM_61_IS_61 (61, 61)
#define PPUTLIMPL_SYM_60_IS_60 (60, 60)
#define PPUTLIMPL_SYM_59_IS_59 (59, 59)
#define PPUTLIMPL_SYM_58_IS_58 (58, 58)
#define PPUTLIMPL_SYM_57_IS_57 (57, 57)
#define PPUTLIMPL_SYM_56_IS_56 (56, 56)
#define PPUTLIMPL_SYM_55_IS_55 (55, 55)
#define PPUTLIMPL_SYM_54_IS_54 (54, 54)
#define PPUTLIMPL_SYM_53_IS_53 (53, 53)
#define PPUTLIMPL_SYM_52_IS_52 (52, 52)
#define PPUTLIMPL_SYM_51_IS_51 (51, 51)
#define PPUTLIMPL_SYM_50_IS_50 (50, 50)
#define PPUTLIMPL_SYM_49_IS_49 (49, 49)
#define PPUTLIMPL_SYM_48_IS_48 (48, 48)
#define PPUTLIMPL_SYM_47_IS_47 (47, 47)
#define PPUTLIMPL_SYM_46_IS_46 (46, 46)
#define PPUTLIMPL_SYM_45_IS_45 (45, 45)
#define PPUTLIMPL_SYM_44_IS_44 (44, 44)
#define PPUTLIMPL_SYM_43_IS_43 (43, 43)
#define PPUTLIMPL_SYM_42_IS_42 (42, 42)
#define PPUTLIMPL_SYM_41_IS_41 (41, 41)
#define PPUTLIMPL_SYM_40_IS_40 (40, 40)
#define PPUTLIMPL_SYM_39_IS_39 (39, 39)
#define PPUTLIMPL_SYM_38_IS_38 (38, 38)
#define PPUTLIMPL_SYM_37_IS_37 (37, 37)
#define PPUTLIMPL_SYM_36_IS_36 (36, 36)
#define PPUTLIMPL_SYM_35_IS_35 (35, 35)
#define PPUTLIMPL_SYM_34_IS_34 (34, 34)
#define PPUTLIMPL_SYM_33_IS_33 (33, 33)
#define PPUTLIMPL_SYM_32_IS_32 (32, 32)
#define PPUTLIMPL_SYM_31_IS_31 (31, 31)
#define PPUTLIMPL_SYM_30_IS_30 (30, 30)
#define PPUTLIMPL_SYM_29_IS_29 (29, 29)
#define PPUTLIMPL_SYM_28_IS_28 (28, 28)
#define PPUTLIMPL_SYM_27_IS_27 (27, 27)
#define PPUTLIMPL_SYM_26_IS_26 (26, 26)
#define PPUTLIMPL_SYM_25_IS_25 (25, 25)
#define PPUTLIMPL_SYM_24_IS_24 (24, 24)
#define PPUTLIMPL_SYM_23_IS_23 (23, 23)
#define PPUTLIMPL_SYM_22_IS_22 (22, 22)
#define PPUTLIMPL_SYM_21_IS_21 (21, 21)
#define PPUTLIMPL_SYM_20_IS_20 (20, 20)
#define PPUTLIMPL_SYM_19_IS_19 (19, 19)
#define PPUTLIMPL_SYM_18_IS_18 (18, 18)
#define PPUTLIMPL_SYM_17_IS_17 (17, 17)
#define PPUTLIMPL_SYM_16_IS_16 (16, 16)
#define PPUTLIMPL_SYM_15_IS_15 (15, 15)
#define PPUTLIMPL_SYM_14_IS_14 (14, 14)
#define PPUTLIMPL_SYM_13_IS_13 (13, 13)
#define PPUTLIMPL_SYM_12_IS_12 (12, 12)
#define PPUTLIMPL_SYM_11_IS_11 (11, 11)
#define PPUTLIMPL_SYM_10_IS_10 (10, 10)
#define PPUTLIMPL_SYM_9_IS_9 (9, 9)
#define PPUTLIMPL_SYM_8_IS_8 (8, 8)
#define PPUTLIMPL_SYM_7_IS_7 (7, 7)
#define PPUTLIMPL_SYM_6_IS_6 (6, 6)
#define PPUTLIMPL_SYM_5_IS_5 (5, 5)
#define PPUTLIMPL_SYM_4_IS_4 (4, 4)
#define PPUTLIMPL_SYM_3_IS_3 (3, 3)
#define PPUTLIMPL_SYM_2_IS_2 (2, 2)
#define PPUTLIMPL_SYM_1_IS_1 (1, 1)
#define PPUTLIMPL_SYM_0_IS_0 (0, 0)
// clang-format on

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_any]
/// ---------------
/// checks if args is a single, potentially empty arg
///
/// PTL_IS_ANY()       // true
/// PTL_IS_ANY(foo)    // true
/// PTL_IS_ANY((a, b)) // true
/// PTL_IS_ANY(a, b)   // false
/// PTL_IS_ANY(, )     // false
/// PTL_IS_ANY(, , )   // false
#define PTL_IS_ANY(/* any... */...) /* -> bool */ PPUTLIMPL_IS_ANY_o(__VA_ARGS__.)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_IS_ANY_o(_, ...) PPUTLIMPL_IS_ANY_o_0##__VA_OPT__(1)
#define PPUTLIMPL_IS_ANY_o_01      false
#define PPUTLIMPL_IS_ANY_o_0       true

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.any]
/// ----------
/// any potentially-empty, individual argument in __VA_ARGS__.
/// this constructor asserts that args are either 0 or 1 in size.
#define PTL_ANY(/* any... */...) /* -> any */                                   \
  PTL_CAT(PPUTLIMPL_ANY_, PTL_IS_ANY(__VA_ARGS__))                              \
  (PTL_ERR(PTL_ANY, "any can only represent args of size 0 or 1", __VA_ARGS__), \
   __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_ANY_true(e, ...)  __VA_ARGS__
#define PPUTLIMPL_ANY_false(e, ...) PTL_FAIL(e)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_atom]
/// ----------------
/// [extends is_some] checks if args is not a tup or obj.
#define PTL_IS_ATOM(/* any... */...) /* -> bool */ __VA_ARGS__

/// [traits.is_obj]
/// ---------------
/// [extends is_some] checks if args is a pputl object.
#define PTL_IS_OBJ(/* any... */...) /* -> bool */ __VA_ARGS__

/// [traits.is_sym]
/// ---------------
/// [extends is_some] checks if args is a pputl sym.
///
/// syms are equality-comparable names that point to static storage.
/// a sym can either be declared globally or wrapped in a namespace.
///
/// global syms match /[\w\d_]+/ and are defined as follows:
///
///   #define PTL_SYM_<name>_IS_<name> (<sym traits...>)
///
/// namespaced syms match /[\w\d_]+\([\w\d_]+\)/ and are defined as follows:
///
///   #define PTL_SYM_<ns>(name)              (<ns>, name)
///   #define PTL_SYM_<ns>_<name1>_IS_<name1> (<sym traits...>)
///   #define PTL_SYM_<ns>_<name2>_IS_<name2> (<sym traits...>)
///   ...
///
/// the sym type lays the foundation for pputl artihmetic literals,
/// object member access, and negative integers.  negative integers
/// cannot be represented using arithmetic symbols  and instead use
/// C++ compl expressions  that can be parsed by the library  while
/// retaining the same meaning in both the preprocessor and C++. In
/// those cases, the namespace is compl and name is an integer.
#define PTL_IS_SYM(/* any... */...) /* -> bool */                            \
  PPUTLIMPL_IS_SYM_o(                                                        \
      PTL_ERR(PTL_IS_SYM, "detected a redefined reserved sym", __VA_ARGS__), \
      __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_IS_SYM_o(e, ...)                                               \
  PTL_CAT(PPUTLIMPL_IS_SYM_, PTL_CAT(PTL_IS_TUP(PTL_SYM##_##__VA_ARGS__),        \
                                     PTL_IS_TUP(PPUTLIMPL_SYM##_##__VA_ARGS__))) \
  (e)
#define PPUTLIMPL_IS_SYM_truetrue(e)   PTL_FAIL(e)
#define PPUTLIMPL_IS_SYM_truefalse(e)  true
#define PPUTLIMPL_IS_SYM_falsetrue(e)  true
#define PPUTLIMPL_IS_SYM_falsefalse(e) false

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

// vim: fdm=marker:fmr={{{,}}}

#endif

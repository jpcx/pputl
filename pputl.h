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
//    recursion, polymorphic objects, and common datastructures.              //
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
//         ├╴sym: an explicitly defined equality-comparable token sequence    //
//         │  └╴num: a builtin, totally-ordered, arithmetic sym               //
//         │     ├╴bool: false|true                                           //
//         │     ├╴hex:  0x0u|0x1u|...|0xEu|0xFu                              //
//         │     ├╴size: 0x00u|0x01u|...|0xFEu|0xFFu                          //
//         │     └╴word: a 12-bit integer                                     //
//         │        ├╴int:  0x800-4096|0x801-4096|...|0|...|2046|2047         //
//         │        └╴uint: 0u|1u|...|4094u|4095u                             //
//         └╴obj: a named, polymorphic, member-addressable state container    //
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
//    pputl num types are mutually exclusive token sequences that have the    //
//    same numeric meaning in pputl, the preprocessor, and C++.  Since the    //
//    preprocessor does not support hyphens in identifiers,  negative ints    //
//    are special syms that cannot form part of an identifier  (unlike all    //
//    other syms).  However, the library is able to detect these sequences    //
//    by concatenating with the hex prefix and dropping the rest. If using   ///
//    an int to construct an identifier, use lang.cat to extract the hex.   ////
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
#define PTL_BUILD /* -> atom */ 20221216004346

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
/// [extends some] an explicitly defined equality-comparable token sequence.
///
/// syms can be created by defining a macro as follows:
///   #define PTL_SYM_<sym name>_IS_<sym name> (<any...>)
///
/// builtin syms (such as int and uint) use a different
/// configuration to reduce the number of API identifiers.
///
/// this format enables sym equality comparisons and data storage.
/// declaration macros must expand to a tuple, which may be empty.
/// use lang.lookup to retrieve the items stored in the sym tuple.
#define PTL_SYM(/* any... */...) /* -> sym */ __VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

// clang-format off
#define PPUTLIMPL_SYM_0xFF (0xFF-256, -1),
#define PPUTLIMPL_SYM_0xFE (0xFE-256, -2),
#define PPUTLIMPL_SYM_0xFD (0xFD-256, -3),
#define PPUTLIMPL_SYM_0xFC (0xFC-256, -4),
#define PPUTLIMPL_SYM_0xFB (0xFB-256, -5),
#define PPUTLIMPL_SYM_0xFA (0xFA-256, -6),
#define PPUTLIMPL_SYM_0xF9 (0xF9-256, -7),
#define PPUTLIMPL_SYM_0xF8 (0xF8-256, -8),
#define PPUTLIMPL_SYM_0xF7 (0xF7-256, -9),
#define PPUTLIMPL_SYM_0xF6 (0xF6-256, -10),
#define PPUTLIMPL_SYM_0xF5 (0xF5-256, -11),
#define PPUTLIMPL_SYM_0xF4 (0xF4-256, -12),
#define PPUTLIMPL_SYM_0xF3 (0xF3-256, -13),
#define PPUTLIMPL_SYM_0xF2 (0xF2-256, -14),
#define PPUTLIMPL_SYM_0xF1 (0xF1-256, -15),
#define PPUTLIMPL_SYM_0xF0 (0xF0-256, -16),
#define PPUTLIMPL_SYM_0xEF (0xEF-256, -17),
#define PPUTLIMPL_SYM_0xEE (0xEE-256, -18),
#define PPUTLIMPL_SYM_0xED (0xED-256, -19),
#define PPUTLIMPL_SYM_0xEC (0xEC-256, -20),
#define PPUTLIMPL_SYM_0xEB (0xEB-256, -21),
#define PPUTLIMPL_SYM_0xEA (0xEA-256, -22),
#define PPUTLIMPL_SYM_0xE9 (0xE9-256, -23),
#define PPUTLIMPL_SYM_0xE8 (0xE8-256, -24),
#define PPUTLIMPL_SYM_0xE7 (0xE7-256, -25),
#define PPUTLIMPL_SYM_0xE6 (0xE6-256, -26),
#define PPUTLIMPL_SYM_0xE5 (0xE5-256, -27),
#define PPUTLIMPL_SYM_0xE4 (0xE4-256, -28),
#define PPUTLIMPL_SYM_0xE3 (0xE3-256, -29),
#define PPUTLIMPL_SYM_0xE2 (0xE2-256, -30),
#define PPUTLIMPL_SYM_0xE1 (0xE1-256, -31),
#define PPUTLIMPL_SYM_0xE0 (0xE0-256, -32),
#define PPUTLIMPL_SYM_0xDF (0xDF-256, -33),
#define PPUTLIMPL_SYM_0xDE (0xDE-256, -34),
#define PPUTLIMPL_SYM_0xDD (0xDD-256, -35),
#define PPUTLIMPL_SYM_0xDC (0xDC-256, -36),
#define PPUTLIMPL_SYM_0xDB (0xDB-256, -37),
#define PPUTLIMPL_SYM_0xDA (0xDA-256, -38),
#define PPUTLIMPL_SYM_0xD9 (0xD9-256, -39),
#define PPUTLIMPL_SYM_0xD8 (0xD8-256, -40),
#define PPUTLIMPL_SYM_0xD7 (0xD7-256, -41),
#define PPUTLIMPL_SYM_0xD6 (0xD6-256, -42),
#define PPUTLIMPL_SYM_0xD5 (0xD5-256, -43),
#define PPUTLIMPL_SYM_0xD4 (0xD4-256, -44),
#define PPUTLIMPL_SYM_0xD3 (0xD3-256, -45),
#define PPUTLIMPL_SYM_0xD2 (0xD2-256, -46),
#define PPUTLIMPL_SYM_0xD1 (0xD1-256, -47),
#define PPUTLIMPL_SYM_0xD0 (0xD0-256, -48),
#define PPUTLIMPL_SYM_0xCF (0xCF-256, -49),
#define PPUTLIMPL_SYM_0xCE (0xCE-256, -50),
#define PPUTLIMPL_SYM_0xCD (0xCD-256, -51),
#define PPUTLIMPL_SYM_0xCC (0xCC-256, -52),
#define PPUTLIMPL_SYM_0xCB (0xCB-256, -53),
#define PPUTLIMPL_SYM_0xCA (0xCA-256, -54),
#define PPUTLIMPL_SYM_0xC9 (0xC9-256, -55),
#define PPUTLIMPL_SYM_0xC8 (0xC8-256, -56),
#define PPUTLIMPL_SYM_0xC7 (0xC7-256, -57),
#define PPUTLIMPL_SYM_0xC6 (0xC6-256, -58),
#define PPUTLIMPL_SYM_0xC5 (0xC5-256, -59),
#define PPUTLIMPL_SYM_0xC4 (0xC4-256, -60),
#define PPUTLIMPL_SYM_0xC3 (0xC3-256, -61),
#define PPUTLIMPL_SYM_0xC2 (0xC2-256, -62),
#define PPUTLIMPL_SYM_0xC1 (0xC1-256, -63),
#define PPUTLIMPL_SYM_0xC0 (0xC0-256, -64),
#define PPUTLIMPL_SYM_0xBF (0xBF-256, -65),
#define PPUTLIMPL_SYM_0xBE (0xBE-256, -66),
#define PPUTLIMPL_SYM_0xBD (0xBD-256, -67),
#define PPUTLIMPL_SYM_0xBC (0xBC-256, -68),
#define PPUTLIMPL_SYM_0xBB (0xBB-256, -69),
#define PPUTLIMPL_SYM_0xBA (0xBA-256, -70),
#define PPUTLIMPL_SYM_0xB9 (0xB9-256, -71),
#define PPUTLIMPL_SYM_0xB8 (0xB8-256, -72),
#define PPUTLIMPL_SYM_0xB7 (0xB7-256, -73),
#define PPUTLIMPL_SYM_0xB6 (0xB6-256, -74),
#define PPUTLIMPL_SYM_0xB5 (0xB5-256, -75),
#define PPUTLIMPL_SYM_0xB4 (0xB4-256, -76),
#define PPUTLIMPL_SYM_0xB3 (0xB3-256, -77),
#define PPUTLIMPL_SYM_0xB2 (0xB2-256, -78),
#define PPUTLIMPL_SYM_0xB1 (0xB1-256, -79),
#define PPUTLIMPL_SYM_0xB0 (0xB0-256, -80),
#define PPUTLIMPL_SYM_0xAF (0xAF-256, -81),
#define PPUTLIMPL_SYM_0xAE (0xAE-256, -82),
#define PPUTLIMPL_SYM_0xAD (0xAD-256, -83),
#define PPUTLIMPL_SYM_0xAC (0xAC-256, -84),
#define PPUTLIMPL_SYM_0xAB (0xAB-256, -85),
#define PPUTLIMPL_SYM_0xAA (0xAA-256, -86),
#define PPUTLIMPL_SYM_0xA9 (0xA9-256, -87),
#define PPUTLIMPL_SYM_0xA8 (0xA8-256, -88),
#define PPUTLIMPL_SYM_0xA7 (0xA7-256, -89),
#define PPUTLIMPL_SYM_0xA6 (0xA6-256, -90),
#define PPUTLIMPL_SYM_0xA5 (0xA5-256, -91),
#define PPUTLIMPL_SYM_0xA4 (0xA4-256, -92),
#define PPUTLIMPL_SYM_0xA3 (0xA3-256, -93),
#define PPUTLIMPL_SYM_0xA2 (0xA2-256, -94),
#define PPUTLIMPL_SYM_0xA1 (0xA1-256, -95),
#define PPUTLIMPL_SYM_0xA0 (0xA0-256, -96),
#define PPUTLIMPL_SYM_0x9F (0x9F-256, -97),
#define PPUTLIMPL_SYM_0x9E (0x9E-256, -98),
#define PPUTLIMPL_SYM_0x9D (0x9D-256, -99),
#define PPUTLIMPL_SYM_0x9C (0x9C-256, -100),
#define PPUTLIMPL_SYM_0x9B (0x9B-256, -101),
#define PPUTLIMPL_SYM_0x9A (0x9A-256, -102),
#define PPUTLIMPL_SYM_0x99 (0x99-256, -103),
#define PPUTLIMPL_SYM_0x98 (0x98-256, -104),
#define PPUTLIMPL_SYM_0x97 (0x97-256, -105),
#define PPUTLIMPL_SYM_0x96 (0x96-256, -106),
#define PPUTLIMPL_SYM_0x95 (0x95-256, -107),
#define PPUTLIMPL_SYM_0x94 (0x94-256, -108),
#define PPUTLIMPL_SYM_0x93 (0x93-256, -109),
#define PPUTLIMPL_SYM_0x92 (0x92-256, -110),
#define PPUTLIMPL_SYM_0x91 (0x91-256, -111),
#define PPUTLIMPL_SYM_0x90 (0x90-256, -112),
#define PPUTLIMPL_SYM_0x8F (0x8F-256, -113),
#define PPUTLIMPL_SYM_0x8E (0x8E-256, -114),
#define PPUTLIMPL_SYM_0x8D (0x8D-256, -115),
#define PPUTLIMPL_SYM_0x8C (0x8C-256, -116),
#define PPUTLIMPL_SYM_0x8B (0x8B-256, -117),
#define PPUTLIMPL_SYM_0x8A (0x8A-256, -118),
#define PPUTLIMPL_SYM_0x89 (0x89-256, -119),
#define PPUTLIMPL_SYM_0x88 (0x88-256, -120),
#define PPUTLIMPL_SYM_0x87 (0x87-256, -121),
#define PPUTLIMPL_SYM_0x86 (0x86-256, -122),
#define PPUTLIMPL_SYM_0x85 (0x85-256, -123),
#define PPUTLIMPL_SYM_0x84 (0x84-256, -124),
#define PPUTLIMPL_SYM_0x83 (0x83-256, -125),
#define PPUTLIMPL_SYM_0x82 (0x82-256, -126),
#define PPUTLIMPL_SYM_0x81 (0x81-256, -127),
#define PPUTLIMPL_SYM_0x80 (0x80-256, -128),
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
/// syms are created by defining a macro as follows:
///   #define PTL_SYM_<sym name>_IS_<sym name> (<any...>)
///
/// this format enables sym equality comparisons and data storage.
/// declaration macros must expand to a tuple, which may be empty.
/// use lang.lookup to retrieve the items stored in the sym tuple.
#define PTL_IS_SYM(/* any... */...) /* -> bool */ PPUTLIMPL_IS_SYM_o(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_IS_SYM_o(...) PTL_IS_TUP(PTL_SYM##_##__VA_ARGS__##_IS_##__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

// vim: fdm=marker:fmr={{{,}}}

#define PTL_SYM_0x80 (),

constexpr auto fo = compl(compl(0x80) xor (compl(0xFF)));

constexpr auto ff = compl(127) + 1;

#define PTL_INTCHK_compl(...) __VA_ARGS__

constexpr auto foo = PTL_STR(PTL_CAT(PTL_SYM_, PTL_CAT(128, PTL_CAT(_IS_, 128))));
constexpr auto bar = PTL_STR(PTL_CAT(PTL_INTCHK_, compl(compl(0x80)xor(compl(0xFF)))));

#endif

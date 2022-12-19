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
//    pputl is a standalone single-header library. Include pputl.h to use.    //
//    Requires a C++ compiler  that supports C++20 preprocesssor standards    //
//    for text replacement macros (especially __VA_OPT__).                    //
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
//     ‐ Macro identifiers per translation unit: [65536].                     //
//     ‐ Parameters per macro definition:        [256].                       //
//     ‐ Arguments per macro invocation:         [256].                       //
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
//      ├╴none: an empty argument; an absence of non-whitespace tokens        //
//      └╴some: a non-empty argument; a presence of non-whitespace tokens     //
//         ├╴tup: a parenthesized token sequence [e.g. (a, b, c)]             //
//         │  └╴pair: a two-tuple [e.g. (foo, bar)]                           //
//         ├╴sym: a global or namespace-qualified static storage pointer      //
//         │  └╴word: a 12-bit unsigned or signed two's complement integer    //
//         │     ├╴int: compl(2047)|compl(2046)|...|0|...|2046|2047           //
//         │     │  └╴bool: 0|1                                               //
//         │     └╴uint: 0u|1u|...|4094u|4095u                                //
//         │        └╴size: 0u|1u|...|254u|255u                               //
//         └╴obj: a polymorphic sym-addressable state container               //
//            ├╴err:   an error message container for lang.fail               //
//            └╴range: a sized sequence container with element accessors      //
//               ├╴vec:   a resizable array                                   //
//               ├╴map:   a mapping of equality-comparable keys to any        //
//               ├╴pq:    a priority queue                                    //
//               ├╴queue: a FIFO queue                                        //
//               └╴stack: a LIFO queue                                        //
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
//    sym serves as the foundation for obj member accessors and arithmetic    //
//    literals, especially negative integers. Since special symbols cannot    //
//    form identifiers, the C++ compl operator is used as a representation    //
//    of negative integers that can be equivalently parsed by the library,    //
//    the preprocessor, and the C++ compiler.                                 //
//                                                                            //
//    pputl arithmetic and comparison functions  accept and return generic    //
//    words and perform casts when necessary. ints cannot be compared with    //
//    uints but are promoted to uints during arithmetic if at least one of    //
//    the operands is unsigned. words overflow or underflow without error.   ///
//    All words except 0 and 0u are considered truthy; 0 and 0u are falsy.  ////
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
#define PTL_BUILD /* -> atom */ 20221218160141

/// [config.uint_max]
/// -----------------
/// the maximum value of a pputl unsigned int.
#define PTL_UINT_MAX /* -> uint */ 255u

// clang-format off

/// [config.int_min]
/// ----------------
/// the minimum value of a pputl signed int.
#define PTL_INT_MIN /* -> int */ compl(0x7F)
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

/// [lang.cat]
/// ----------
/// sensibly concatenates two args. must be compatible with the ## operator.
///
/// cat performs the following conversions for simplified identifier construction:
///  - true                             -> T
///  - false                            -> F
///  - compl(0x7FF), compl(0x7FE), etc. -> 0x800, 0x801, etc.
///
/// PTL_CAT(foo, bar)           // foobar
/// PTL_CAT(foo, PTL_EAT(bar))  // foo
/// PTL_CAT(,)                  // <nothing>
/// PTL_CAT(is_, true)          // is_T
/// PTL_CAT(is_, false)         // is_F
/// PTL_CAT(foo_, compl(0x7FF)) // foo_0x800
#define PTL_CAT(/* a: any, b: any */...) /* -> any */ \
  PPUTLIMPL_CAT_o(PPUTLIMPL_CAT_CHK(__VA_ARGS__), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_CAT_o(...)           PPUTLIMPL_CAT_oo(__VA_ARGS__)
#define PPUTLIMPL_CAT_oo(ca, cb, a, b) PPUTLIMPL_CAT_oo_##ca##cb(a, b)
#define PPUTLIMPL_CAT_oo_truetrue(a, b) \
  PPUTLIMPL_CAT_oo_truetrue_o(PPUTLIMPL_CAT_oo_truetrue_switch(a, b), a, b)
#define PPUTLIMPL_CAT_oo_truetrue_o(...) PPUTLIMPL_CAT_oo_truetrue_oo(__VA_ARGS__)
#define PPUTLIMPL_CAT_oo_truetrue_oo(csa, csb, a, b) \
  PPUTLIMPL_CAT_oo_truetrue_oo_##csa##csb(a, b)
#define PPUTLIMPL_CAT_oo_truetrue_oo_00(a, b) TT
#define PPUTLIMPL_CAT_oo_truetrue_oo_01(a, b) TF
#define PPUTLIMPL_CAT_oo_truetrue_oo_02(a, b) NOT_IMPLEMENTED
#define PPUTLIMPL_CAT_oo_truetrue_oo_10(a, b) FT
#define PPUTLIMPL_CAT_oo_truetrue_oo_11(a, b) FF
#define PPUTLIMPL_CAT_oo_truetrue_oo_12(a, b) NOT_IMPLEMENTED
#define PPUTLIMPL_CAT_oo_truetrue_oo_20(a, b) NOT_IMPLEMENTED
#define PPUTLIMPL_CAT_oo_truetrue_oo_21(a, b) NOT_IMPLEMENTED
#define PPUTLIMPL_CAT_oo_truetrue_oo_22(a, b) NOT_IMPLEMENTED
#define PPUTLIMPL_CAT_oo_truetrue_switch(a, b) \
  PPUTLIMPL_CAT_oo_truetrue_switch_##a, PPUTLIMPL_CAT_oo_truetrue_switch_##b
#define PPUTLIMPL_CAT_oo_truetrue_switch_true       0
#define PPUTLIMPL_CAT_oo_truetrue_switch_false      1
#define PPUTLIMPL_CAT_oo_truetrue_switch_compl(...) 2
#define PPUTLIMPL_CAT_oo_truefalse(a, b) \
  PPUTLIMPL_CAT_oo_truefalse_o(PPUTLIMPL_CAT_oo_truefalse_switch(a), a, b)
#define PPUTLIMPL_CAT_oo_truefalse_o(...)            PPUTLIMPL_CAT_oo_truefalse_oo(__VA_ARGS__)
#define PPUTLIMPL_CAT_oo_truefalse_oo(cs, a, b)      PPUTLIMPL_CAT_oo_truefalse_oo_##cs(a, b)
#define PPUTLIMPL_CAT_oo_truefalse_oo_0(a, b)        T##b
#define PPUTLIMPL_CAT_oo_truefalse_oo_1(a, b)        F##b
#define PPUTLIMPL_CAT_oo_truefalse_oo_2(a, b)        NOT_IMPLEMENTED
#define PPUTLIMPL_CAT_oo_truefalse_switch(a)         PPUTLIMPL_CAT_oo_truefalse_switch_##a
#define PPUTLIMPL_CAT_oo_truefalse_switch_true       0
#define PPUTLIMPL_CAT_oo_truefalse_switch_false      1
#define PPUTLIMPL_CAT_oo_truefalse_switch_compl(...) 2
#define PPUTLIMPL_CAT_oo_falsetrue(a, b) \
  PPUTLIMPL_CAT_oo_falsetrue_o(PPUTLIMPL_CAT_oo_falsetrue_switch(b), a, b)
#define PPUTLIMPL_CAT_oo_falsetrue_o(...)            PPUTLIMPL_CAT_oo_falsetrue_oo(__VA_ARGS__)
#define PPUTLIMPL_CAT_oo_falsetrue_oo(cs, a, b)      PPUTLIMPL_CAT_oo_falsetrue_oo_##cs(a, b)
#define PPUTLIMPL_CAT_oo_falsetrue_oo_0(a, b)        a##T
#define PPUTLIMPL_CAT_oo_falsetrue_oo_1(a, b)        a##F
#define PPUTLIMPL_CAT_oo_falsetrue_oo_2(a, b)        NOT_IMPLEMENTED
#define PPUTLIMPL_CAT_oo_falsetrue_switch(b)         PPUTLIMPL_CAT_oo_falsetrue_switch_##b
#define PPUTLIMPL_CAT_oo_falsetrue_switch_true       0
#define PPUTLIMPL_CAT_oo_falsetrue_switch_false      1
#define PPUTLIMPL_CAT_oo_falsetrue_switch_compl(...) 2
#define PPUTLIMPL_CAT_oo_falsefalse(a, b)            a##b
#define PPUTLIMPL_CAT_CHK(a, b) \
  PTL_IS_NONE(PPUTLIMPL_CAT_CHK_##a), PTL_IS_NONE(PPUTLIMPL_CAT_CHK_##b)
#define PPUTLIMPL_CAT_CHK_true
#define PPUTLIMPL_CAT_CHK_false
#define PPUTLIMPL_CAT_CHK_compl(...)

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

#define PPUTLIMPL_IS_TUP_T           PPUTLIMPL_IS_TUP_T_o
#define PPUTLIMPL_IS_TUP_T_o(obj)    PTL_IS_NONE(PTL_EAT obj)
#define PPUTLIMPL_IS_TUP_F           PPUTLIMPL_IS_TUP_F_fail
#define PPUTLIMPL_IS_TUP_F_fail(...) false

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.sym]
/// ----------
/// [extends some] a global or namespace-qualified static storage pointer.
///
/// syms are defined in two ways:
///
/// globally: (sym matches /[\w\d_]+/)
///
///   #define PTL_SYM_<name>_is_<name> (<data...>)
///
/// namespace-qualified: (sym matches /[\w\d_]+\([\w\d_]+\)/)
///
///   #define PTL_SYM_<ns>(name)              (<ns>, name)
///   #define PTL_SYM_<ns>_<name1>_is_<name1> (<data...>)
///   #define PTL_SYM_<ns>_<name2>_is_<name2> (<data...>)
///   ...
///
/// this layout allows syms to be compared using compare.eq and compare.ne.
///
/// the sym type lays the foundation for arithmetic literals, obj member
/// access,  and negative integers.  since arithmetic tokens cannot form
/// identifiers,  the C++ compl operator is used to ensure that negative
/// ints  can be parsed by the library and have the same meaning in both
/// the preprocessor and C++ code. when using an int or num to construct
/// an identifier, use lang.cat (which converts ints < 0 to 12-bit hex).
#define PTL_SYM(/* any... */...) /* -> sym */ __VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

// clang-format off
#define PPUTLIMPL_SYM_compl(int) (compl, int)
#define PPUTLIMPL_SYM_compl_127_is_127 (I, (F, F))
#define PPUTLIMPL_SYM_compl_126_is_126 (I, (F, E))
#define PPUTLIMPL_SYM_compl_125_is_125 (I, (F, D))
#define PPUTLIMPL_SYM_compl_124_is_124 (I, (F, C))
#define PPUTLIMPL_SYM_compl_123_is_123 (I, (F, B))
#define PPUTLIMPL_SYM_compl_122_is_122 (I, (F, A))
#define PPUTLIMPL_SYM_compl_121_is_121 (I, (F, 9))
#define PPUTLIMPL_SYM_compl_120_is_120 (I, (F, 8))
#define PPUTLIMPL_SYM_compl_119_is_119 (I, (F, 7))
#define PPUTLIMPL_SYM_compl_118_is_118 (I, (F, 6))
#define PPUTLIMPL_SYM_compl_117_is_117 (I, (F, 5))
#define PPUTLIMPL_SYM_compl_116_is_116 (I, (F, 4))
#define PPUTLIMPL_SYM_compl_115_is_115 (I, (F, 3))
#define PPUTLIMPL_SYM_compl_114_is_114 (I, (F, 2))
#define PPUTLIMPL_SYM_compl_113_is_113 (I, (F, 1))
#define PPUTLIMPL_SYM_compl_112_is_112 (I, (F, 0))
#define PPUTLIMPL_SYM_compl_111_is_111 (I, (E, F))
#define PPUTLIMPL_SYM_compl_110_is_110 (I, (E, E))
#define PPUTLIMPL_SYM_compl_109_is_109 (I, (E, D))
#define PPUTLIMPL_SYM_compl_108_is_108 (I, (E, C))
#define PPUTLIMPL_SYM_compl_107_is_107 (I, (E, B))
#define PPUTLIMPL_SYM_compl_106_is_106 (I, (E, A))
#define PPUTLIMPL_SYM_compl_105_is_105 (I, (E, 9))
#define PPUTLIMPL_SYM_compl_104_is_104 (I, (E, 8))
#define PPUTLIMPL_SYM_compl_103_is_103 (I, (E, 7))
#define PPUTLIMPL_SYM_compl_102_is_102 (I, (E, 6))
#define PPUTLIMPL_SYM_compl_101_is_101 (I, (E, 5))
#define PPUTLIMPL_SYM_compl_100_is_100 (I, (E, 4))
#define PPUTLIMPL_SYM_compl_99_is_99 (I, (E, 3))
#define PPUTLIMPL_SYM_compl_98_is_98 (I, (E, 2))
#define PPUTLIMPL_SYM_compl_97_is_97 (I, (E, 1))
#define PPUTLIMPL_SYM_compl_96_is_96 (I, (E, 0))
#define PPUTLIMPL_SYM_compl_95_is_95 (I, (D, F))
#define PPUTLIMPL_SYM_compl_94_is_94 (I, (D, E))
#define PPUTLIMPL_SYM_compl_93_is_93 (I, (D, D))
#define PPUTLIMPL_SYM_compl_92_is_92 (I, (D, C))
#define PPUTLIMPL_SYM_compl_91_is_91 (I, (D, B))
#define PPUTLIMPL_SYM_compl_90_is_90 (I, (D, A))
#define PPUTLIMPL_SYM_compl_89_is_89 (I, (D, 9))
#define PPUTLIMPL_SYM_compl_88_is_88 (I, (D, 8))
#define PPUTLIMPL_SYM_compl_87_is_87 (I, (D, 7))
#define PPUTLIMPL_SYM_compl_86_is_86 (I, (D, 6))
#define PPUTLIMPL_SYM_compl_85_is_85 (I, (D, 5))
#define PPUTLIMPL_SYM_compl_84_is_84 (I, (D, 4))
#define PPUTLIMPL_SYM_compl_83_is_83 (I, (D, 3))
#define PPUTLIMPL_SYM_compl_82_is_82 (I, (D, 2))
#define PPUTLIMPL_SYM_compl_81_is_81 (I, (D, 1))
#define PPUTLIMPL_SYM_compl_80_is_80 (I, (D, 0))
#define PPUTLIMPL_SYM_compl_79_is_79 (I, (C, F))
#define PPUTLIMPL_SYM_compl_78_is_78 (I, (C, E))
#define PPUTLIMPL_SYM_compl_77_is_77 (I, (C, D))
#define PPUTLIMPL_SYM_compl_76_is_76 (I, (C, C))
#define PPUTLIMPL_SYM_compl_75_is_75 (I, (C, B))
#define PPUTLIMPL_SYM_compl_74_is_74 (I, (C, A))
#define PPUTLIMPL_SYM_compl_73_is_73 (I, (C, 9))
#define PPUTLIMPL_SYM_compl_72_is_72 (I, (C, 8))
#define PPUTLIMPL_SYM_compl_71_is_71 (I, (C, 7))
#define PPUTLIMPL_SYM_compl_70_is_70 (I, (C, 6))
#define PPUTLIMPL_SYM_compl_69_is_69 (I, (C, 5))
#define PPUTLIMPL_SYM_compl_68_is_68 (I, (C, 4))
#define PPUTLIMPL_SYM_compl_67_is_67 (I, (C, 3))
#define PPUTLIMPL_SYM_compl_66_is_66 (I, (C, 2))
#define PPUTLIMPL_SYM_compl_65_is_65 (I, (C, 1))
#define PPUTLIMPL_SYM_compl_64_is_64 (I, (C, 0))
#define PPUTLIMPL_SYM_compl_63_is_63 (I, (B, F))
#define PPUTLIMPL_SYM_compl_62_is_62 (I, (B, E))
#define PPUTLIMPL_SYM_compl_61_is_61 (I, (B, D))
#define PPUTLIMPL_SYM_compl_60_is_60 (I, (B, C))
#define PPUTLIMPL_SYM_compl_59_is_59 (I, (B, B))
#define PPUTLIMPL_SYM_compl_58_is_58 (I, (B, A))
#define PPUTLIMPL_SYM_compl_57_is_57 (I, (B, 9))
#define PPUTLIMPL_SYM_compl_56_is_56 (I, (B, 8))
#define PPUTLIMPL_SYM_compl_55_is_55 (I, (B, 7))
#define PPUTLIMPL_SYM_compl_54_is_54 (I, (B, 6))
#define PPUTLIMPL_SYM_compl_53_is_53 (I, (B, 5))
#define PPUTLIMPL_SYM_compl_52_is_52 (I, (B, 4))
#define PPUTLIMPL_SYM_compl_51_is_51 (I, (B, 3))
#define PPUTLIMPL_SYM_compl_50_is_50 (I, (B, 2))
#define PPUTLIMPL_SYM_compl_49_is_49 (I, (B, 1))
#define PPUTLIMPL_SYM_compl_48_is_48 (I, (B, 0))
#define PPUTLIMPL_SYM_compl_47_is_47 (I, (A, F))
#define PPUTLIMPL_SYM_compl_46_is_46 (I, (A, E))
#define PPUTLIMPL_SYM_compl_45_is_45 (I, (A, D))
#define PPUTLIMPL_SYM_compl_44_is_44 (I, (A, C))
#define PPUTLIMPL_SYM_compl_43_is_43 (I, (A, B))
#define PPUTLIMPL_SYM_compl_42_is_42 (I, (A, A))
#define PPUTLIMPL_SYM_compl_41_is_41 (I, (A, 9))
#define PPUTLIMPL_SYM_compl_40_is_40 (I, (A, 8))
#define PPUTLIMPL_SYM_compl_39_is_39 (I, (A, 7))
#define PPUTLIMPL_SYM_compl_38_is_38 (I, (A, 6))
#define PPUTLIMPL_SYM_compl_37_is_37 (I, (A, 5))
#define PPUTLIMPL_SYM_compl_36_is_36 (I, (A, 4))
#define PPUTLIMPL_SYM_compl_35_is_35 (I, (A, 3))
#define PPUTLIMPL_SYM_compl_34_is_34 (I, (A, 2))
#define PPUTLIMPL_SYM_compl_33_is_33 (I, (A, 1))
#define PPUTLIMPL_SYM_compl_32_is_32 (I, (A, 0))
#define PPUTLIMPL_SYM_compl_31_is_31 (I, (9, F))
#define PPUTLIMPL_SYM_compl_30_is_30 (I, (9, E))
#define PPUTLIMPL_SYM_compl_29_is_29 (I, (9, D))
#define PPUTLIMPL_SYM_compl_28_is_28 (I, (9, C))
#define PPUTLIMPL_SYM_compl_27_is_27 (I, (9, B))
#define PPUTLIMPL_SYM_compl_26_is_26 (I, (9, A))
#define PPUTLIMPL_SYM_compl_25_is_25 (I, (9, 9))
#define PPUTLIMPL_SYM_compl_24_is_24 (I, (9, 8))
#define PPUTLIMPL_SYM_compl_23_is_23 (I, (9, 7))
#define PPUTLIMPL_SYM_compl_22_is_22 (I, (9, 6))
#define PPUTLIMPL_SYM_compl_21_is_21 (I, (9, 5))
#define PPUTLIMPL_SYM_compl_20_is_20 (I, (9, 4))
#define PPUTLIMPL_SYM_compl_19_is_19 (I, (9, 3))
#define PPUTLIMPL_SYM_compl_18_is_18 (I, (9, 2))
#define PPUTLIMPL_SYM_compl_17_is_17 (I, (9, 1))
#define PPUTLIMPL_SYM_compl_16_is_16 (I, (9, 0))
#define PPUTLIMPL_SYM_compl_15_is_15 (I, (8, F))
#define PPUTLIMPL_SYM_compl_14_is_14 (I, (8, E))
#define PPUTLIMPL_SYM_compl_13_is_13 (I, (8, D))
#define PPUTLIMPL_SYM_compl_12_is_12 (I, (8, C))
#define PPUTLIMPL_SYM_compl_11_is_11 (I, (8, B))
#define PPUTLIMPL_SYM_compl_10_is_10 (I, (8, A))
#define PPUTLIMPL_SYM_compl_9_is_9 (I, (8, 9))
#define PPUTLIMPL_SYM_compl_8_is_8 (I, (8, 8))
#define PPUTLIMPL_SYM_compl_7_is_7 (I, (8, 7))
#define PPUTLIMPL_SYM_compl_6_is_6 (I, (8, 6))
#define PPUTLIMPL_SYM_compl_5_is_5 (I, (8, 5))
#define PPUTLIMPL_SYM_compl_4_is_4 (I, (8, 4))
#define PPUTLIMPL_SYM_compl_3_is_3 (I, (8, 3))
#define PPUTLIMPL_SYM_compl_2_is_2 (I, (8, 2))
#define PPUTLIMPL_SYM_compl_1_is_1 (I, (8, 1))
#define PPUTLIMPL_SYM_compl_0_is_0 (I, (8, 0))
#define PPUTLIMPL_SYM_127_is_127 (I, (7, F))
#define PPUTLIMPL_SYM_126_is_126 (I, (7, E))
#define PPUTLIMPL_SYM_125_is_125 (I, (7, D))
#define PPUTLIMPL_SYM_124_is_124 (I, (7, C))
#define PPUTLIMPL_SYM_123_is_123 (I, (7, B))
#define PPUTLIMPL_SYM_122_is_122 (I, (7, A))
#define PPUTLIMPL_SYM_121_is_121 (I, (7, 9))
#define PPUTLIMPL_SYM_120_is_120 (I, (7, 8))
#define PPUTLIMPL_SYM_119_is_119 (I, (7, 7))
#define PPUTLIMPL_SYM_118_is_118 (I, (7, 6))
#define PPUTLIMPL_SYM_117_is_117 (I, (7, 5))
#define PPUTLIMPL_SYM_116_is_116 (I, (7, 4))
#define PPUTLIMPL_SYM_115_is_115 (I, (7, 3))
#define PPUTLIMPL_SYM_114_is_114 (I, (7, 2))
#define PPUTLIMPL_SYM_113_is_113 (I, (7, 1))
#define PPUTLIMPL_SYM_112_is_112 (I, (7, 0))
#define PPUTLIMPL_SYM_111_is_111 (I, (6, F))
#define PPUTLIMPL_SYM_110_is_110 (I, (6, E))
#define PPUTLIMPL_SYM_109_is_109 (I, (6, D))
#define PPUTLIMPL_SYM_108_is_108 (I, (6, C))
#define PPUTLIMPL_SYM_107_is_107 (I, (6, B))
#define PPUTLIMPL_SYM_106_is_106 (I, (6, A))
#define PPUTLIMPL_SYM_105_is_105 (I, (6, 9))
#define PPUTLIMPL_SYM_104_is_104 (I, (6, 8))
#define PPUTLIMPL_SYM_103_is_103 (I, (6, 7))
#define PPUTLIMPL_SYM_102_is_102 (I, (6, 6))
#define PPUTLIMPL_SYM_101_is_101 (I, (6, 5))
#define PPUTLIMPL_SYM_100_is_100 (I, (6, 4))
#define PPUTLIMPL_SYM_99_is_99 (I, (6, 3))
#define PPUTLIMPL_SYM_98_is_98 (I, (6, 2))
#define PPUTLIMPL_SYM_97_is_97 (I, (6, 1))
#define PPUTLIMPL_SYM_96_is_96 (I, (6, 0))
#define PPUTLIMPL_SYM_95_is_95 (I, (5, F))
#define PPUTLIMPL_SYM_94_is_94 (I, (5, E))
#define PPUTLIMPL_SYM_93_is_93 (I, (5, D))
#define PPUTLIMPL_SYM_92_is_92 (I, (5, C))
#define PPUTLIMPL_SYM_91_is_91 (I, (5, B))
#define PPUTLIMPL_SYM_90_is_90 (I, (5, A))
#define PPUTLIMPL_SYM_89_is_89 (I, (5, 9))
#define PPUTLIMPL_SYM_88_is_88 (I, (5, 8))
#define PPUTLIMPL_SYM_87_is_87 (I, (5, 7))
#define PPUTLIMPL_SYM_86_is_86 (I, (5, 6))
#define PPUTLIMPL_SYM_85_is_85 (I, (5, 5))
#define PPUTLIMPL_SYM_84_is_84 (I, (5, 4))
#define PPUTLIMPL_SYM_83_is_83 (I, (5, 3))
#define PPUTLIMPL_SYM_82_is_82 (I, (5, 2))
#define PPUTLIMPL_SYM_81_is_81 (I, (5, 1))
#define PPUTLIMPL_SYM_80_is_80 (I, (5, 0))
#define PPUTLIMPL_SYM_79_is_79 (I, (4, F))
#define PPUTLIMPL_SYM_78_is_78 (I, (4, E))
#define PPUTLIMPL_SYM_77_is_77 (I, (4, D))
#define PPUTLIMPL_SYM_76_is_76 (I, (4, C))
#define PPUTLIMPL_SYM_75_is_75 (I, (4, B))
#define PPUTLIMPL_SYM_74_is_74 (I, (4, A))
#define PPUTLIMPL_SYM_73_is_73 (I, (4, 9))
#define PPUTLIMPL_SYM_72_is_72 (I, (4, 8))
#define PPUTLIMPL_SYM_71_is_71 (I, (4, 7))
#define PPUTLIMPL_SYM_70_is_70 (I, (4, 6))
#define PPUTLIMPL_SYM_69_is_69 (I, (4, 5))
#define PPUTLIMPL_SYM_68_is_68 (I, (4, 4))
#define PPUTLIMPL_SYM_67_is_67 (I, (4, 3))
#define PPUTLIMPL_SYM_66_is_66 (I, (4, 2))
#define PPUTLIMPL_SYM_65_is_65 (I, (4, 1))
#define PPUTLIMPL_SYM_64_is_64 (I, (4, 0))
#define PPUTLIMPL_SYM_63_is_63 (I, (3, F))
#define PPUTLIMPL_SYM_62_is_62 (I, (3, E))
#define PPUTLIMPL_SYM_61_is_61 (I, (3, D))
#define PPUTLIMPL_SYM_60_is_60 (I, (3, C))
#define PPUTLIMPL_SYM_59_is_59 (I, (3, B))
#define PPUTLIMPL_SYM_58_is_58 (I, (3, A))
#define PPUTLIMPL_SYM_57_is_57 (I, (3, 9))
#define PPUTLIMPL_SYM_56_is_56 (I, (3, 8))
#define PPUTLIMPL_SYM_55_is_55 (I, (3, 7))
#define PPUTLIMPL_SYM_54_is_54 (I, (3, 6))
#define PPUTLIMPL_SYM_53_is_53 (I, (3, 5))
#define PPUTLIMPL_SYM_52_is_52 (I, (3, 4))
#define PPUTLIMPL_SYM_51_is_51 (I, (3, 3))
#define PPUTLIMPL_SYM_50_is_50 (I, (3, 2))
#define PPUTLIMPL_SYM_49_is_49 (I, (3, 1))
#define PPUTLIMPL_SYM_48_is_48 (I, (3, 0))
#define PPUTLIMPL_SYM_47_is_47 (I, (2, F))
#define PPUTLIMPL_SYM_46_is_46 (I, (2, E))
#define PPUTLIMPL_SYM_45_is_45 (I, (2, D))
#define PPUTLIMPL_SYM_44_is_44 (I, (2, C))
#define PPUTLIMPL_SYM_43_is_43 (I, (2, B))
#define PPUTLIMPL_SYM_42_is_42 (I, (2, A))
#define PPUTLIMPL_SYM_41_is_41 (I, (2, 9))
#define PPUTLIMPL_SYM_40_is_40 (I, (2, 8))
#define PPUTLIMPL_SYM_39_is_39 (I, (2, 7))
#define PPUTLIMPL_SYM_38_is_38 (I, (2, 6))
#define PPUTLIMPL_SYM_37_is_37 (I, (2, 5))
#define PPUTLIMPL_SYM_36_is_36 (I, (2, 4))
#define PPUTLIMPL_SYM_35_is_35 (I, (2, 3))
#define PPUTLIMPL_SYM_34_is_34 (I, (2, 2))
#define PPUTLIMPL_SYM_33_is_33 (I, (2, 1))
#define PPUTLIMPL_SYM_32_is_32 (I, (2, 0))
#define PPUTLIMPL_SYM_31_is_31 (I, (1, F))
#define PPUTLIMPL_SYM_30_is_30 (I, (1, E))
#define PPUTLIMPL_SYM_29_is_29 (I, (1, D))
#define PPUTLIMPL_SYM_28_is_28 (I, (1, C))
#define PPUTLIMPL_SYM_27_is_27 (I, (1, B))
#define PPUTLIMPL_SYM_26_is_26 (I, (1, A))
#define PPUTLIMPL_SYM_25_is_25 (I, (1, 9))
#define PPUTLIMPL_SYM_24_is_24 (I, (1, 8))
#define PPUTLIMPL_SYM_23_is_23 (I, (1, 7))
#define PPUTLIMPL_SYM_22_is_22 (I, (1, 6))
#define PPUTLIMPL_SYM_21_is_21 (I, (1, 5))
#define PPUTLIMPL_SYM_20_is_20 (I, (1, 4))
#define PPUTLIMPL_SYM_19_is_19 (I, (1, 3))
#define PPUTLIMPL_SYM_18_is_18 (I, (1, 2))
#define PPUTLIMPL_SYM_17_is_17 (I, (1, 1))
#define PPUTLIMPL_SYM_16_is_16 (I, (1, 0))
#define PPUTLIMPL_SYM_15_is_15 (I, (0, F))
#define PPUTLIMPL_SYM_14_is_14 (I, (0, E))
#define PPUTLIMPL_SYM_13_is_13 (I, (0, D))
#define PPUTLIMPL_SYM_12_is_12 (I, (0, C))
#define PPUTLIMPL_SYM_11_is_11 (I, (0, B))
#define PPUTLIMPL_SYM_10_is_10 (I, (0, A))
#define PPUTLIMPL_SYM_9_is_9 (I, (0, 9))
#define PPUTLIMPL_SYM_8_is_8 (I, (0, 8))
#define PPUTLIMPL_SYM_7_is_7 (I, (0, 7))
#define PPUTLIMPL_SYM_6_is_6 (I, (0, 6))
#define PPUTLIMPL_SYM_5_is_5 (I, (0, 5))
#define PPUTLIMPL_SYM_4_is_4 (I, (0, 4))
#define PPUTLIMPL_SYM_3_is_3 (I, (0, 3))
#define PPUTLIMPL_SYM_2_is_2 (I, (0, 2))
#define PPUTLIMPL_SYM_1_is_1 (I, (0, 1))
#define PPUTLIMPL_SYM_0_is_0 (I, (0, 0))
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
/// warning: breaks on single inputs that are not one of the
///          predefined types and are not compatible with the
///          concatenation operator.
///
/// syms are defined in two ways:
///
/// globally: (sym matches /[\w\d_]+/)
///
///   #define PTL_SYM_<name>_is_<name> (<data...>)
///
/// namespace-qualified: (sym matches /[\w\d_]+\([\w\d_]+\)/)
///
///   #define PTL_SYM_<ns>(name)              (<ns>, name)
///   #define PTL_SYM_<ns>_<name1>_is_<name1> (<data...>)
///   #define PTL_SYM_<ns>_<name2>_is_<name2> (<data...>)
///   ...
///
/// this layout allows syms to be compared using compare.eq and compare.ne.
///
/// the sym type lays the foundation for arithmetic literals, obj member
/// access,  and negative integers.  since arithmetic tokens cannot form
/// identifiers,  the C++ compl operator is used to ensure that negative
/// ints  can be parsed by the library and have the same meaning in both
/// the preprocessor and C++ code. when using an int or num to construct
/// an identifier, use lang.cat (which converts ints < 0 to 12-bit hex).
///
/// PTL_IS_SYM()            // false
/// PTL_IS_SYM(())          // false
/// PTL_IS_SYM(foo)         // false
/// PTL_IS_SYM(foo, bar)    // false
/// PTL_IS_SYM(false)       // true
/// PTL_IS_SYM(0x00u)       // true
/// PTL_IS_SYM(0)           // true
/// PTL_IS_SYM(PTL_INT_MIN) // true
#define PTL_IS_SYM(/* any... */...) /* -> bool */                                        \
  PTL_CAT(PPUTLIMPL_IS_SYM_, PTL_CAT(PTL_IS_SOME(__VA_ARGS__), PTL_IS_TUP(__VA_ARGS__))) \
  (PTL_ERR(PTL_IS_SYM, "detected a redefined reserved sym", __VA_ARGS__), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIMPL_IS_SYM_TT(e, v) false
#define PPUTLIMPL_IS_SYM_TF(e, v)                                          \
  PTL_CAT(PPUTLIMPL_IS_SYM_TF_,                                            \
          PTL_CAT(PTL_IS_TUP(PTL_SYM_##v), PTL_IS_TUP(PPUTLIMPL_SYM_##v))) \
  (e, v)
#define PPUTLIMPL_IS_SYM_TF_TT(e, v) PTL_FAIL(e)
#define PPUTLIMPL_IS_SYM_TF_TF(e, v) true
#define PPUTLIMPL_IS_SYM_TF_FT(e, v) true
#define PPUTLIMPL_IS_SYM_TF_FF(e, v)                                                \
  PTL_CAT(PPUTLIMPL_IS_SYM_TF_FF_, PTL_CAT(PTL_IS_TUP(PTL_SYM_##v##_is_##v),        \
                                           PTL_IS_TUP(PPUTLIMPL_SYM_##v##_is_##v))) \
  (e)
#define PPUTLIMPL_IS_SYM_TF_FF_TT(e) PTL_FAIL(e)
#define PPUTLIMPL_IS_SYM_TF_FF_TF(e) true
#define PPUTLIMPL_IS_SYM_TF_FF_FT(e) true
#define PPUTLIMPL_IS_SYM_TF_FF_FF(e) false
#define PPUTLIMPL_IS_SYM_FF(e, ...)  false

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

// vim: fdm=marker:fmr={{{,}}}

#endif

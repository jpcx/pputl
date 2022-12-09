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

#include "pputl.h"

namespace {
// streq that trims spacing around outer ASSERT_PP_EQ
// parens to account for preprocessor discrepancies
// / *( *\(.*\) */
static constexpr bool
pp_streq(char const* l, char const* r) {
  for (; *l and *l != '('; ++l)
    ;
  for (; *l and *r != '('; ++r)
    ;
  if (not *l)
    return not *r;
  if (not *r)
    return not *l;
  ++l;
  ++r;
  for (; *l == ' '; ++l)
    ;
  for (; *r == ' '; ++r)
    ;
  char const* le = l;
  char const* re = r;
  for (; *le; ++le)
    ;
  for (; *re; ++re)
    ;
  for (; le > l and *(le - 1) != ')'; --le)
    ;
  for (; re > r and *(re - 1) != ')'; --re)
    ;
  --le;
  --re;
  for (; le > l and *(le - 1) == ' '; --le)
    ;
  for (; re > r and *(re - 1) == ' '; --re)
    ;
  for (; l != le and r != re and *l == *r; ++l, ++r)
    ;
  return l == le and r == re;
}
} // namespace

#define ASSERT_PP_EQ_X(a, b) static_assert(pp_streq(#a, #b), #a " != " #b)
#define ASSERT_PP_EQ(a, b)   ASSERT_PP_EQ_X(a, b)

// clang-format off
ASSERT_PP_EQ((PTL_ESC ()), ());
ASSERT_PP_EQ((PTL_ESC (a, b, c)), (a, b, c));

ASSERT_PP_EQ((PTL_CAT(foo, bar)), (foobar));
ASSERT_PP_EQ((PTL_CAT(foo, PTL_EAT(bar))), (fooPTL_EAT(bar)));

ASSERT_PP_EQ((PTL_XCAT(foo, bar)), (foobar));
ASSERT_PP_EQ((PTL_XCAT(foo, PTL_EAT(bar))), (foo));
ASSERT_PP_EQ((PTL_XCAT(,)), ());

ASSERT_PP_EQ((PTL_STR()), (""));
ASSERT_PP_EQ((PTL_STR(foo, bar)), ("foo, bar"));
ASSERT_PP_EQ((PTL_STR(PTL_CAT(foo, bar))), ("PTL_CAT(foo, bar)"));
ASSERT_PP_EQ((PTL_STR(foo)), ("foo"));
ASSERT_PP_EQ((PTL_STR(, )), (","));
ASSERT_PP_EQ((PTL_STR(, , )), (", ,"));
ASSERT_PP_EQ((PTL_STR(a, )), ("a,"));
ASSERT_PP_EQ((PTL_STR(a, , )), ("a, ,"));
ASSERT_PP_EQ((PTL_STR(, a)), (", a"));
ASSERT_PP_EQ((PTL_STR(, a, )), (", a,"));
ASSERT_PP_EQ((PTL_STR(, , a)), (", , a"));

ASSERT_PP_EQ((PTL_XSTR()), (""));
ASSERT_PP_EQ((PTL_XSTR(foo, bar)), ("foo, bar"));
ASSERT_PP_EQ((PTL_XSTR(PTL_CAT(foo, bar))), ("foobar"));
ASSERT_PP_EQ((PTL_XSTR(foo)), ("foo"));
ASSERT_PP_EQ((PTL_XSTR(, )), (","));
ASSERT_PP_EQ((PTL_XSTR(, , )), (", ,"));
ASSERT_PP_EQ((PTL_XSTR(a, )), ("a,"));
ASSERT_PP_EQ((PTL_XSTR(a, , )), ("a, ,"));
ASSERT_PP_EQ((PTL_XSTR(, a)), (", a"));
ASSERT_PP_EQ((PTL_XSTR(, a, )), (", a,"));
ASSERT_PP_EQ((PTL_XSTR(, , a)), (", , a"));

ASSERT_PP_EQ((PTL_XFIRST()), ());
ASSERT_PP_EQ((PTL_XFIRST(, )), ());
ASSERT_PP_EQ((PTL_XFIRST(a)), (a));
ASSERT_PP_EQ((PTL_XFIRST(a, b)), (a));

ASSERT_PP_EQ((PTL_XREST()), ());
ASSERT_PP_EQ((PTL_XREST(, )), ());
ASSERT_PP_EQ((PTL_XREST(a)), ());
ASSERT_PP_EQ((PTL_XREST(a, b)), (b));
ASSERT_PP_EQ((PTL_XREST(a, b, c)), (b, c));
ASSERT_PP_EQ((PTL_XREST(PTL_XREST(a, b, c))), (c));
ASSERT_PP_EQ((PTL_XREST(a, , )), (,));
ASSERT_PP_EQ((PTL_XREST(a, b, , )), (b, ,));

ASSERT_PP_EQ((PTL_TRIM()), ());
ASSERT_PP_EQ((PTL_TRIM(, )), ());
ASSERT_PP_EQ((PTL_TRIM(a)), (a));
ASSERT_PP_EQ((PTL_TRIM(a, b)), (a, b));
ASSERT_PP_EQ((PTL_TRIM(a, )), (a));
ASSERT_PP_EQ((PTL_TRIM(, b, c)), (b, c));
ASSERT_PP_EQ((PTL_TRIM(a, b, )), (a, b,));
ASSERT_PP_EQ((PTL_TRIM(a, b, c)), (a, b, c));
ASSERT_PP_EQ((PTL_TRIM(, b)), (b));
ASSERT_PP_EQ((PTL_TRIM(a, , c)), (a,  , c));

ASSERT_PP_EQ((PTL_DEFAULT(a)), (a));
ASSERT_PP_EQ((PTL_DEFAULT(a,)), (a));
ASSERT_PP_EQ((PTL_DEFAULT(a, b)), (b));
ASSERT_PP_EQ((PTL_DEFAULT(a, b, c)), (b, c));

ASSERT_PP_EQ((PTL_IS_ANY()), (1));
ASSERT_PP_EQ((PTL_IS_ANY(foo)), (1));
ASSERT_PP_EQ((PTL_IS_ANY((a, b))), (1));
ASSERT_PP_EQ((PTL_IS_ANY(a, b)), (0));
ASSERT_PP_EQ((PTL_IS_ANY(, )), (0));
ASSERT_PP_EQ((PTL_IS_ANY(, , )), (0));
ASSERT_PP_EQ((PTL_IS_ANY(a, )), (0));
ASSERT_PP_EQ((PTL_IS_ANY(a, , )), (0));
ASSERT_PP_EQ((PTL_IS_ANY(, a)), (0));
ASSERT_PP_EQ((PTL_IS_ANY(, a, )), (0));
ASSERT_PP_EQ((PTL_IS_ANY(, , a)), (0));

ASSERT_PP_EQ((PTL_IS_NONE()), (1));
ASSERT_PP_EQ((PTL_IS_NONE(foo)), (0));
ASSERT_PP_EQ((PTL_IS_NONE(foo, bar)), (0));
ASSERT_PP_EQ((PTL_IS_NONE(PTL_ESC())), (1));
ASSERT_PP_EQ((PTL_IS_NONE(, )), (0));
ASSERT_PP_EQ((PTL_IS_NONE(, , )), (0));
ASSERT_PP_EQ((PTL_IS_NONE(a, )), (0));
ASSERT_PP_EQ((PTL_IS_NONE(a, , )), (0));
ASSERT_PP_EQ((PTL_IS_NONE(, a)), (0));
ASSERT_PP_EQ((PTL_IS_NONE(, a, )), (0));
ASSERT_PP_EQ((PTL_IS_NONE(, , a)), (0));

ASSERT_PP_EQ((PTL_IS_ATOM()), (0));
ASSERT_PP_EQ((PTL_IS_ATOM(foo)), (1));
ASSERT_PP_EQ((PTL_IS_ATOM(0)), (1));
ASSERT_PP_EQ((PTL_IS_ATOM(1, 2)), (0));
ASSERT_PP_EQ((PTL_IS_ATOM(())), (0));
ASSERT_PP_EQ((PTL_IS_ATOM((1, 2))), (0));
ASSERT_PP_EQ((PTL_IS_ATOM(PTL_OBJ())), (0));
ASSERT_PP_EQ((PTL_IS_ATOM((), ())), (0));
ASSERT_PP_EQ((PTL_IS_ATOM(PTL_ESC(()))), (0));
ASSERT_PP_EQ((PTL_IS_ATOM(PTL_ESC((1, 2)))), (0));
ASSERT_PP_EQ((PTL_IS_ATOM(, )), (0));
ASSERT_PP_EQ((PTL_IS_ATOM(, , )), (0));
ASSERT_PP_EQ((PTL_IS_ATOM(a, )), (0));
ASSERT_PP_EQ((PTL_IS_ATOM(a, , )), (0));
ASSERT_PP_EQ((PTL_IS_ATOM(, a)), (0));
ASSERT_PP_EQ((PTL_IS_ATOM(, a, )), (0));
ASSERT_PP_EQ((PTL_IS_ATOM(, , a)), (0));
ASSERT_PP_EQ((PTL_IS_ATOM((, ))), (0));
ASSERT_PP_EQ((PTL_IS_ATOM((, , ))), (0));
ASSERT_PP_EQ((PTL_IS_ATOM((a, ))), (0));
ASSERT_PP_EQ((PTL_IS_ATOM((a, , ))), (0));
ASSERT_PP_EQ((PTL_IS_ATOM((, a))), (0));
ASSERT_PP_EQ((PTL_IS_ATOM((, a, ))), (0));
ASSERT_PP_EQ((PTL_IS_ATOM((, , a))), (0));

ASSERT_PP_EQ((PTL_IS_IDEC(1)), (1));
ASSERT_PP_EQ((PTL_IS_IDEC(1u)), (0));
ASSERT_PP_EQ((PTL_IS_IDEC(127)), (1));
ASSERT_PP_EQ((PTL_IS_IDEC(255)), (0));
ASSERT_PP_EQ((PTL_IS_IDEC(0x00u)), (0));
ASSERT_PP_EQ((PTL_IS_IDEC(0xFF)), (0));
ASSERT_PP_EQ((PTL_IS_IDEC((), ())), (0));

ASSERT_PP_EQ((PTL_IS_BOOL()), (0));
ASSERT_PP_EQ((PTL_IS_BOOL(0)), (1));
ASSERT_PP_EQ((PTL_IS_BOOL(1)), (1));
ASSERT_PP_EQ((PTL_IS_BOOL(1u)), (0));
ASSERT_PP_EQ((PTL_IS_BOOL(0x00)), (0));
ASSERT_PP_EQ((PTL_IS_BOOL(0, 1)), (0));
ASSERT_PP_EQ((PTL_IS_BOOL((0))), (0));
ASSERT_PP_EQ((PTL_IS_BOOL(())), (0));
ASSERT_PP_EQ((PTL_IS_BOOL((), ())), (0));
ASSERT_PP_EQ((PTL_IS_BOOL(0, )), (0));
ASSERT_PP_EQ((PTL_IS_BOOL(, )), (0));
ASSERT_PP_EQ((PTL_IS_BOOL(, , )), (0));
ASSERT_PP_EQ((PTL_IS_BOOL(a, )), (0));
ASSERT_PP_EQ((PTL_IS_BOOL(a, , )), (0));
ASSERT_PP_EQ((PTL_IS_BOOL(, a)), (0));
ASSERT_PP_EQ((PTL_IS_BOOL(, a, )), (0));
ASSERT_PP_EQ((PTL_IS_BOOL(, , a)), (0));

ASSERT_PP_EQ((PTL_IS_IHEX(1)), (0));
ASSERT_PP_EQ((PTL_IS_IHEX(1u)), (0));
ASSERT_PP_EQ((PTL_IS_IHEX(0x00)), (1));
ASSERT_PP_EQ((PTL_IS_IHEX(0xFF)), (1));
ASSERT_PP_EQ((PTL_IS_IHEX(0xFFu)), (0));
ASSERT_PP_EQ((PTL_IS_IHEX((), ())), (0));

ASSERT_PP_EQ((PTL_IS_UDEC(1)), (0));
ASSERT_PP_EQ((PTL_IS_UDEC(1u)), (1));
ASSERT_PP_EQ((PTL_IS_UDEC(255)), (0));
ASSERT_PP_EQ((PTL_IS_UDEC(255u)), (1));
ASSERT_PP_EQ((PTL_IS_UDEC(0x00u)), (0));
ASSERT_PP_EQ((PTL_IS_UDEC(0xFF)), (0));
ASSERT_PP_EQ((PTL_IS_UDEC((), ())), (0));

ASSERT_PP_EQ((PTL_IS_UHEX(1)), (0));
ASSERT_PP_EQ((PTL_IS_UHEX(1u)), (0));
ASSERT_PP_EQ((PTL_IS_UHEX(0x00u)), (1));
ASSERT_PP_EQ((PTL_IS_UHEX(0xFF)), (0));
ASSERT_PP_EQ((PTL_IS_UHEX((), ())), (0));

ASSERT_PP_EQ((PTL_IS_TUP()), (0));
ASSERT_PP_EQ((PTL_IS_TUP(1, 2)), (0));
ASSERT_PP_EQ((PTL_IS_TUP(())), (1));
ASSERT_PP_EQ((PTL_IS_TUP((1, 2))), (1));
ASSERT_PP_EQ((PTL_IS_TUP((), ())), (0));
ASSERT_PP_EQ((PTL_IS_TUP(PTL_ESC(()))), (1));
ASSERT_PP_EQ((PTL_IS_TUP(PTL_ESC((1, 2)))), (1));
ASSERT_PP_EQ((PTL_IS_TUP(, )), (0));
ASSERT_PP_EQ((PTL_IS_TUP(, , )), (0));
ASSERT_PP_EQ((PTL_IS_TUP(a, )), (0));
ASSERT_PP_EQ((PTL_IS_TUP(a, , )), (0));
ASSERT_PP_EQ((PTL_IS_TUP(, a)), (0));
ASSERT_PP_EQ((PTL_IS_TUP(, a, )), (0));
ASSERT_PP_EQ((PTL_IS_TUP(, , a)), (0));
ASSERT_PP_EQ((PTL_IS_TUP((, ))), (1));
ASSERT_PP_EQ((PTL_IS_TUP((, , ))), (1));
ASSERT_PP_EQ((PTL_IS_TUP((a, ))), (1));
ASSERT_PP_EQ((PTL_IS_TUP((a, , ))), (1));
ASSERT_PP_EQ((PTL_IS_TUP((, a))), (1));
ASSERT_PP_EQ((PTL_IS_TUP((, a, ))), (1));
ASSERT_PP_EQ((PTL_IS_TUP((, , a))), (1));

ASSERT_PP_EQ((PTL_IS_PAIR()), (0));
ASSERT_PP_EQ((PTL_IS_PAIR(1, 2)), (0));
ASSERT_PP_EQ((PTL_IS_PAIR(())), (0));
ASSERT_PP_EQ((PTL_IS_PAIR((1, 2))), (1));
ASSERT_PP_EQ((PTL_IS_PAIR((), ())), (0));
ASSERT_PP_EQ((PTL_IS_PAIR(, )), (0));
ASSERT_PP_EQ((PTL_IS_PAIR(, , )), (0));
ASSERT_PP_EQ((PTL_IS_PAIR(a, )), (0));
ASSERT_PP_EQ((PTL_IS_PAIR(a, , )), (0));
ASSERT_PP_EQ((PTL_IS_PAIR(, a)), (0));
ASSERT_PP_EQ((PTL_IS_PAIR(, a, )), (0));
ASSERT_PP_EQ((PTL_IS_PAIR(, , a)), (0));
ASSERT_PP_EQ((PTL_IS_PAIR((, ))), (1));
ASSERT_PP_EQ((PTL_IS_PAIR((, , ))), (0));
ASSERT_PP_EQ((PTL_IS_PAIR((a, ))), (1));
ASSERT_PP_EQ((PTL_IS_PAIR((a, , ))), (0));
ASSERT_PP_EQ((PTL_IS_PAIR((, a))), (1));
ASSERT_PP_EQ((PTL_IS_PAIR((, a, ))), (0));
ASSERT_PP_EQ((PTL_IS_PAIR((, , a))), (0));

ASSERT_PP_EQ((PTL_ANY()), ());
ASSERT_PP_EQ((PTL_ANY(foo)), (foo));

ASSERT_PP_EQ((PTL_NONE()), ());

ASSERT_PP_EQ((PTL_ATOM()), (0));
ASSERT_PP_EQ((PTL_ATOM(1)), (1));
ASSERT_PP_EQ((PTL_ATOM(foo)), (foo));

ASSERT_PP_EQ((PTL_IDEC(0x00)), (0));
ASSERT_PP_EQ((PTL_IDEC(0x01)), (1));
ASSERT_PP_EQ((PTL_IDEC(0x05u)), (5));
ASSERT_PP_EQ((PTL_IDEC(0x7F)), (127));
ASSERT_PP_EQ((PTL_IDEC(127)), (127));

ASSERT_PP_EQ((PTL_IHEX()), (0x00));
ASSERT_PP_EQ((PTL_IHEX(0)), (0x00));
ASSERT_PP_EQ((PTL_IHEX(1)), (0x01));
ASSERT_PP_EQ((PTL_IHEX(5)), (0x05));
ASSERT_PP_EQ((PTL_IHEX(255u)), (0xFF));
ASSERT_PP_EQ((PTL_IHEX(127u)), (0x7F));
// clang-format on

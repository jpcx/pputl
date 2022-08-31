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
//           inc   dec  neg  add   sub   log2  mul                            //
//           divr  div  mod  pow2  sqrt  fact                                 //
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
//            │  ├╴enum<v0|v1|...>: an atom that matches a specified union    //
//            │  ├╴bool: a literal `1` or `0`                                 //
//            │  ├╴hex:  a literal uppercase hexadecimal digit [e.g. F]       //
//            │  ├╴nybl: a literal 4-bit bool concatenation [e.g. 0110]       //
//            │  ├╴int: <abstract> a word-sized signed integer                //
//            │  │  ├╴idec: a positive 2s-complement decimal [e.g. 3]         //
//            │  │  └╴ihex: a signed hex integer [e.g. 0x861]                 //
//            │  └╴uint: <abstract> a word-sized unsigned integer             //
//            │     ├╴udec: an unsigned decimal integer [e.g. 42u]            //
//            │     └╴uhex: an unsigned hex integer [e.g. 0x02Au]             //
//            ├╴tup: a parenthesized list [e.g ()] [e.g. (a, b)]              //
//            │  └╴utup: an unsigned word-sized hex tup [e.g. (6, D, 2)]      //
//            └╴word: <union> int | uint | utup                               //
//               └╴size: a non-negative word capped by the argument limit     //
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

ASSERT_PP_EQ((PTL_IS_LIST()), (1));
ASSERT_PP_EQ((PTL_IS_LIST(foo)), (1));
ASSERT_PP_EQ((PTL_IS_LIST((a, b))), (1));
ASSERT_PP_EQ((PTL_IS_LIST(a, b)), (1));
ASSERT_PP_EQ((PTL_IS_LIST(, )), (1));
ASSERT_PP_EQ((PTL_IS_LIST(, , )), (1));
ASSERT_PP_EQ((PTL_IS_LIST(a, )), (1));
ASSERT_PP_EQ((PTL_IS_LIST(a, , )), (1));
ASSERT_PP_EQ((PTL_IS_LIST(, a)), (1));
ASSERT_PP_EQ((PTL_IS_LIST(, a, )), (1));
ASSERT_PP_EQ((PTL_IS_LIST(, , a)), (1));

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

ASSERT_PP_EQ((PTL_IS_OBJ()), (0));
ASSERT_PP_EQ((PTL_IS_OBJ(,)), (0));
ASSERT_PP_EQ((PTL_IS_OBJ(foo,)), (0));
ASSERT_PP_EQ((PTL_IS_OBJ(foo, bar)), (0));
ASSERT_PP_EQ((PTL_IS_OBJ(foo)), (1));
ASSERT_PP_EQ((PTL_IS_OBJ((42))), (1));

ASSERT_PP_EQ((PTL_IS_ATOM()), (0));
ASSERT_PP_EQ((PTL_IS_ATOM(foo)), (1));
ASSERT_PP_EQ((PTL_IS_ATOM(0)), (1));
ASSERT_PP_EQ((PTL_IS_ATOM(1, 2)), (0));
ASSERT_PP_EQ((PTL_IS_ATOM(())), (0));
ASSERT_PP_EQ((PTL_IS_ATOM((1, 2))), (0));
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

ASSERT_PP_EQ((PTL_IS_BOOL()), (0));
ASSERT_PP_EQ((PTL_IS_BOOL(0)), (1));
ASSERT_PP_EQ((PTL_IS_BOOL(1)), (1));
ASSERT_PP_EQ((PTL_IS_BOOL(1u)), (0));
ASSERT_PP_EQ((PTL_IS_BOOL(0x000)), (0));
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

ASSERT_PP_EQ((PTL_IS_HEX()), (0));
ASSERT_PP_EQ((PTL_IS_HEX(0)), (1));
ASSERT_PP_EQ((PTL_IS_HEX(Q)), (0));
ASSERT_PP_EQ((PTL_IS_HEX(foo)), (0));
ASSERT_PP_EQ((PTL_IS_HEX(B)), (1));
ASSERT_PP_EQ((PTL_IS_HEX(b)), (0));
ASSERT_PP_EQ((PTL_IS_HEX(F)), (1));

ASSERT_PP_EQ((PTL_IS_NYBL()), (0));
ASSERT_PP_EQ((PTL_IS_NYBL(0)), (0));
ASSERT_PP_EQ((PTL_IS_NYBL(B)), (0));
ASSERT_PP_EQ((PTL_IS_NYBL(000)), (0));
ASSERT_PP_EQ((PTL_IS_NYBL(0000)), (1));
ASSERT_PP_EQ((PTL_IS_NYBL(0110)), (1));

ASSERT_PP_EQ((PTL_IS_INT()), (0));
ASSERT_PP_EQ((PTL_IS_INT(foo)), (0));
ASSERT_PP_EQ((PTL_IS_INT(0)), (1));
ASSERT_PP_EQ((PTL_IS_INT(0u)), (0));
ASSERT_PP_EQ((PTL_IS_INT(4095)), (0));
ASSERT_PP_EQ((PTL_IS_INT(0x000u)), (0));
ASSERT_PP_EQ((PTL_IS_INT(0xFFF)), (1));
ASSERT_PP_EQ((PTL_IS_INT(0xF)), (0));
ASSERT_PP_EQ((PTL_IS_INT((), ())), (0));

ASSERT_PP_EQ((PTL_IS_IDEC(1)), (1));
ASSERT_PP_EQ((PTL_IS_IDEC(1u)), (0));
ASSERT_PP_EQ((PTL_IS_IDEC(2047)), (1));
ASSERT_PP_EQ((PTL_IS_IDEC(4095)), (0));
ASSERT_PP_EQ((PTL_IS_IDEC(0x000u)), (0));
ASSERT_PP_EQ((PTL_IS_IDEC(0xFFF)), (0));
ASSERT_PP_EQ((PTL_IS_IDEC((), ())), (0));

ASSERT_PP_EQ((PTL_IS_IHEX(1)), (0));
ASSERT_PP_EQ((PTL_IS_IHEX(1u)), (0));
ASSERT_PP_EQ((PTL_IS_IHEX(0x000)), (1));
ASSERT_PP_EQ((PTL_IS_IHEX(0xFFF)), (1));
ASSERT_PP_EQ((PTL_IS_IHEX(0xFFFu)), (0));
ASSERT_PP_EQ((PTL_IS_IHEX((), ())), (0));

ASSERT_PP_EQ((PTL_IS_UINT()), (0));
ASSERT_PP_EQ((PTL_IS_UINT(foo)), (0));
ASSERT_PP_EQ((PTL_IS_UINT(0)), (0));
ASSERT_PP_EQ((PTL_IS_UINT(0u)), (1));
ASSERT_PP_EQ((PTL_IS_UINT(4095)), (0));
ASSERT_PP_EQ((PTL_IS_UINT(4095u)), (1));
ASSERT_PP_EQ((PTL_IS_UINT(0x000u)), (1));
ASSERT_PP_EQ((PTL_IS_UINT(0xFFF)), (0));
ASSERT_PP_EQ((PTL_IS_UINT(0b110u)), (0));
ASSERT_PP_EQ((PTL_IS_UINT((), ())), (0));

ASSERT_PP_EQ((PTL_IS_UDEC(1)), (0));
ASSERT_PP_EQ((PTL_IS_UDEC(1u)), (1));
ASSERT_PP_EQ((PTL_IS_UDEC(4095)), (0));
ASSERT_PP_EQ((PTL_IS_UDEC(4095u)), (1));
ASSERT_PP_EQ((PTL_IS_UDEC(0x000u)), (0));
ASSERT_PP_EQ((PTL_IS_UDEC(0xFFF)), (0));
ASSERT_PP_EQ((PTL_IS_UDEC((), ())), (0));

ASSERT_PP_EQ((PTL_IS_UHEX(1)), (0));
ASSERT_PP_EQ((PTL_IS_UHEX(1u)), (0));
ASSERT_PP_EQ((PTL_IS_UHEX(0x000u)), (1));
ASSERT_PP_EQ((PTL_IS_UHEX(0xFFF)), (0));
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

ASSERT_PP_EQ((PTL_IS_UTUP()), (0));
ASSERT_PP_EQ((PTL_IS_UTUP(foo)), (0));
ASSERT_PP_EQ((PTL_IS_UTUP(0)), (0));
ASSERT_PP_EQ((PTL_IS_UTUP(9, B, C)), (0));
ASSERT_PP_EQ((PTL_IS_UTUP((9, B, C))), (1));
ASSERT_PP_EQ((PTL_IS_UTUP((9, B, C,))), (0));
ASSERT_PP_EQ((PTL_IS_UTUP((9, B, C, E))), (0));
ASSERT_PP_EQ((PTL_IS_UTUP(())), (0));
ASSERT_PP_EQ((PTL_IS_UTUP((0))), (0));

ASSERT_PP_EQ((PTL_IS_WORD(0)), (1));
ASSERT_PP_EQ((PTL_IS_WORD(0u)), (1));
ASSERT_PP_EQ((PTL_IS_WORD(foo)), (0));
ASSERT_PP_EQ((PTL_IS_WORD(())), (0));
ASSERT_PP_EQ((PTL_IS_WORD(A)), (0));
ASSERT_PP_EQ((PTL_IS_WORD(0x800)), (1));
ASSERT_PP_EQ((PTL_IS_WORD(4095u)), (1));
ASSERT_PP_EQ((PTL_IS_WORD(0xFFFu)), (1));
ASSERT_PP_EQ((PTL_IS_WORD((0, 0, 8))), (1));

ASSERT_PP_EQ((PTL_IS_SIZE(0)), (1));
ASSERT_PP_EQ((PTL_IS_SIZE(0u)), (1));
ASSERT_PP_EQ((PTL_IS_SIZE(foo)), (0));
ASSERT_PP_EQ((PTL_IS_SIZE(())), (0));
ASSERT_PP_EQ((PTL_IS_SIZE(A)), (0));
ASSERT_PP_EQ((PTL_IS_SIZE(0x800)), (0));
ASSERT_PP_EQ((PTL_IS_SIZE(4095u)), (0));
ASSERT_PP_EQ((PTL_IS_SIZE(0xFFFu)), (0));
ASSERT_PP_EQ((PTL_IS_SIZE(255u)), (1));
ASSERT_PP_EQ((PTL_IS_SIZE((0, 0, 8))), (1));

ASSERT_PP_EQ((PTL_TYPEOF((foo))), (TUP));
ASSERT_PP_EQ((PTL_TYPEOF(0)), (IDEC));
ASSERT_PP_EQ((PTL_TYPEOF(0u)), (UDEC));
ASSERT_PP_EQ((PTL_TYPEOF(D)), (HEX));
ASSERT_PP_EQ((PTL_TYPEOF(4095)), (ATOM));
ASSERT_PP_EQ((PTL_TYPEOF(4095u)), (UDEC));
ASSERT_PP_EQ((PTL_TYPEOF(0xFFF)), (IHEX));
ASSERT_PP_EQ((PTL_TYPEOF(0xFFFu)), (UHEX));
ASSERT_PP_EQ((PTL_TYPEOF(foo)), (ATOM));
ASSERT_PP_EQ((PTL_TYPEOF(001)), (ATOM));
ASSERT_PP_EQ((PTL_TYPEOF(0010)), (NYBL));
ASSERT_PP_EQ((PTL_TYPEOF(foo, bar)), (LIST));
ASSERT_PP_EQ((PTL_TYPEOF((A))), (TUP));
ASSERT_PP_EQ((PTL_TYPEOF((0, 0, 0))), (UTUP));
ASSERT_PP_EQ((PTL_TYPEOF((F, F, F))), (UTUP));
ASSERT_PP_EQ((PTL_TYPEOF()), (NONE));

ASSERT_PP_EQ((PTL_SIZEOF()), (0u));
ASSERT_PP_EQ((PTL_SIZEOF(a)), (1u));
ASSERT_PP_EQ((PTL_SIZEOF(a, b)), (2u));
ASSERT_PP_EQ((PTL_SIZEOF(, )), (2u));
ASSERT_PP_EQ((PTL_SIZEOF(a, b, c)), (3u));
ASSERT_PP_EQ((PTL_SIZEOF(,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,)), (255u));
ASSERT_PP_EQ((PTL_SIZEOF(, , )), (3u));
ASSERT_PP_EQ((PTL_SIZEOF(a, )), (2u));
ASSERT_PP_EQ((PTL_SIZEOF(a, , )), (3u));
ASSERT_PP_EQ((PTL_SIZEOF(, a)), (2u));
ASSERT_PP_EQ((PTL_SIZEOF(, a, )), (3u));
ASSERT_PP_EQ((PTL_SIZEOF(, , a)), (3u));

ASSERT_PP_EQ((PTL_LIST()), ());
ASSERT_PP_EQ((PTL_LIST(foo)), (foo));
ASSERT_PP_EQ((PTL_LIST(foo, bar)), (foo, bar));

ASSERT_PP_EQ((PTL_ANY()), ());
ASSERT_PP_EQ((PTL_ANY(foo)), (foo));

ASSERT_PP_EQ((PTL_NONE()), ());

ASSERT_PP_EQ((PTL_OBJ(foo)), (foo));

ASSERT_PP_EQ((PTL_ATOM(foo)), (foo));

ASSERT_PP_EQ((PTL_BOOL(0)), (0));
ASSERT_PP_EQ((PTL_BOOL(1)), (1));

ASSERT_PP_EQ((PTL_HEX(0)), (0));
ASSERT_PP_EQ((PTL_HEX(F)), (F));
ASSERT_PP_EQ((PTL_HEX(0110)), (6));
ASSERT_PP_EQ((PTL_HEX(1010)), (A));

ASSERT_PP_EQ((PTL_NYBL(0000)), (0000));
ASSERT_PP_EQ((PTL_NYBL(0110)), (0110));
ASSERT_PP_EQ((PTL_NYBL(5)), (0101));
ASSERT_PP_EQ((PTL_NYBL(A)), (1010));

ASSERT_PP_EQ((PTL_INT(0)), (0));
ASSERT_PP_EQ((PTL_INT(1, IHEX)), (0x001));
ASSERT_PP_EQ((PTL_INT(0x002)), (0x002));
ASSERT_PP_EQ((PTL_INT(0x800, IDEC)), (0x800));
ASSERT_PP_EQ((PTL_INT(0x002, IDEC)), (2));
ASSERT_PP_EQ((PTL_INT(7u)), (7));
ASSERT_PP_EQ((PTL_INT(15u, IHEX)), (0x00F));
ASSERT_PP_EQ((PTL_INT(4095u)), (0xFFF));
ASSERT_PP_EQ((PTL_INT(0x007u)), (0x007));
ASSERT_PP_EQ((PTL_INT(0xFFFu, IDEC)), (0xFFF));
ASSERT_PP_EQ((PTL_INT(0x005u, IDEC)), (5));
ASSERT_PP_EQ((PTL_INT((0, 0, 0))), (0x000));
ASSERT_PP_EQ((PTL_INT((8, 0, 0), IDEC)), (0x800));
ASSERT_PP_EQ((PTL_INT((7, F, F), IDEC)), (2047));

ASSERT_PP_EQ((PTL_IDEC(0x000)), (0));
ASSERT_PP_EQ((PTL_IDEC(0x001)), (1));
ASSERT_PP_EQ((PTL_IDEC(0x005u)), (5));
ASSERT_PP_EQ((PTL_IDEC(0x7FF)), (2047));
ASSERT_PP_EQ((PTL_IDEC(2047)), (2047));

ASSERT_PP_EQ((PTL_IHEX(0)), (0x000));
ASSERT_PP_EQ((PTL_IHEX(1)), (0x001));
ASSERT_PP_EQ((PTL_IHEX(5)), (0x005));
ASSERT_PP_EQ((PTL_IHEX(4095u)), (0xFFF));
ASSERT_PP_EQ((PTL_IHEX(2047u)), (0x7FF));

ASSERT_PP_EQ((PTL_UINT(0)), (0u));
ASSERT_PP_EQ((PTL_UINT(2, UHEX)), (0x002u));
ASSERT_PP_EQ((PTL_UINT(0x007)), (0x007u));
ASSERT_PP_EQ((PTL_UINT(0x00F, UDEC)), (15u));
ASSERT_PP_EQ((PTL_UINT(8u)), (8u));
ASSERT_PP_EQ((PTL_UINT(6u, UHEX)), (0x006u));
ASSERT_PP_EQ((PTL_UINT(0x005u)), (0x005u));
ASSERT_PP_EQ((PTL_UINT(0x004u, UDEC)), (4u));
ASSERT_PP_EQ((PTL_UINT((0, 0, 0))), (0x000u));
ASSERT_PP_EQ((PTL_UINT((F, F, F), UDEC)), (4095u));

ASSERT_PP_EQ((PTL_UDEC(0x000u)), (0u));
ASSERT_PP_EQ((PTL_UDEC(1)), (1u));
ASSERT_PP_EQ((PTL_UDEC(5)), (5u));
ASSERT_PP_EQ((PTL_UDEC(0x005u)), (5u));
ASSERT_PP_EQ((PTL_UDEC(0xFFFu)), (4095u));
ASSERT_PP_EQ((PTL_UDEC(0xFFF)), (4095u));

ASSERT_PP_EQ((PTL_UHEX(0)), (0x000u));
ASSERT_PP_EQ((PTL_UHEX(1)), (0x001u));
ASSERT_PP_EQ((PTL_UHEX(5)), (0x005u));
ASSERT_PP_EQ((PTL_UHEX(4095u)), (0xFFFu));
ASSERT_PP_EQ((PTL_UHEX(0x000u)), (0x000u));
ASSERT_PP_EQ((PTL_UHEX(0x001u)), (0x001u));
ASSERT_PP_EQ((PTL_UHEX(0xFFF)), (0xFFFu));

ASSERT_PP_EQ((PTL_TUP(())), (()));
ASSERT_PP_EQ((PTL_TUP((1, 2))), ((1, 2)));

ASSERT_PP_EQ((PTL_UTUP(0)), ((0, 0, 0)));
ASSERT_PP_EQ((PTL_UTUP(4095u)), ((F, F, F)));
ASSERT_PP_EQ((PTL_UTUP(0x800)), ((8, 0, 0)));
ASSERT_PP_EQ((PTL_UTUP(2047)), ((7, F, F)));
ASSERT_PP_EQ((PTL_UTUP((1, 0, 0))), ((1, 0, 0)));

ASSERT_PP_EQ((PTL_WORD(0)), (0));
ASSERT_PP_EQ((PTL_WORD(1, IHEX)), (0x001));
ASSERT_PP_EQ((PTL_WORD(2, UDEC)), (2u));
ASSERT_PP_EQ((PTL_WORD(3, UHEX)), (0x003u));
ASSERT_PP_EQ((PTL_WORD(4, UTUP)), ((0, 0, 4)));
ASSERT_PP_EQ((PTL_WORD(0x002)), (0x002));
ASSERT_PP_EQ((PTL_WORD(0x800, IDEC)), (0x800));
ASSERT_PP_EQ((PTL_WORD(0x002, IDEC)), (2));
ASSERT_PP_EQ((PTL_WORD(0x00F, UDEC)), (15u));
ASSERT_PP_EQ((PTL_WORD(0x007, UHEX)), (0x007u));
ASSERT_PP_EQ((PTL_WORD(0x008, UTUP)), ((0, 0, 8)));
ASSERT_PP_EQ((PTL_WORD(8u)), (8u));
ASSERT_PP_EQ((PTL_WORD(7u, IDEC)), (7));
ASSERT_PP_EQ((PTL_WORD(15u, IHEX)), (0x00F));
ASSERT_PP_EQ((PTL_WORD(4095u, IDEC)), (0xFFF));
ASSERT_PP_EQ((PTL_WORD(6u, UHEX)), (0x006u));
ASSERT_PP_EQ((PTL_WORD(4u, UTUP)), ((0, 0, 4)));
ASSERT_PP_EQ((PTL_WORD(0x005u)), (0x005u));
ASSERT_PP_EQ((PTL_WORD(0x005u, IDEC)), (5));
ASSERT_PP_EQ((PTL_WORD(0x007u, IHEX)), (0x007));
ASSERT_PP_EQ((PTL_WORD(0xFFFu, IDEC)), (0xFFF));
ASSERT_PP_EQ((PTL_WORD(0x004u, UDEC)), (4u));
ASSERT_PP_EQ((PTL_WORD(0x00Fu, UTUP)), ((0, 0, F)));
ASSERT_PP_EQ((PTL_WORD((0, 0, 3))), ((0, 0, 3)));
ASSERT_PP_EQ((PTL_WORD((7, F, F), IDEC)), (2047));
ASSERT_PP_EQ((PTL_WORD((0, 0, 0), IHEX)), (0x000));
ASSERT_PP_EQ((PTL_WORD((8, 0, 0), IDEC)), (0x800));
ASSERT_PP_EQ((PTL_WORD((F, F, F), UDEC)), (4095u));
ASSERT_PP_EQ((PTL_UINT((0, 0, 0), UHEX)), (0x000u));

ASSERT_PP_EQ((PTL_SIZE(0)), (0));
ASSERT_PP_EQ((PTL_SIZE(1)), (1));
ASSERT_PP_EQ((PTL_SIZE(0x007)), (0x007));
ASSERT_PP_EQ((PTL_SIZE(255u)), (255u));

ASSERT_PP_EQ((PTL_NOT(0)), (1));
ASSERT_PP_EQ((PTL_NOT(1)), (0));

ASSERT_PP_EQ((PTL_AND(0, 0)), (0));
ASSERT_PP_EQ((PTL_AND(0, 1)), (0));
ASSERT_PP_EQ((PTL_AND(1, 0)), (0));
ASSERT_PP_EQ((PTL_AND(1, 1)), (1));

ASSERT_PP_EQ((PTL_OR(0, 0)), (0));
ASSERT_PP_EQ((PTL_OR(0, 1)), (1));
ASSERT_PP_EQ((PTL_OR(1, 0)), (1));
ASSERT_PP_EQ((PTL_OR(1, 1)), (1));

ASSERT_PP_EQ((PTL_XOR(0, 0)), (0));
ASSERT_PP_EQ((PTL_XOR(0, 1)), (1));
ASSERT_PP_EQ((PTL_XOR(1, 0)), (1));
ASSERT_PP_EQ((PTL_XOR(1, 1)), (0));

ASSERT_PP_EQ((PTL_NAND(0, 0)), (1));
ASSERT_PP_EQ((PTL_NAND(0, 1)), (1));
ASSERT_PP_EQ((PTL_NAND(1, 0)), (1));
ASSERT_PP_EQ((PTL_NAND(1, 1)), (0));

ASSERT_PP_EQ((PTL_NOR(0, 0)), (1));
ASSERT_PP_EQ((PTL_NOR(0, 1)), (0));
ASSERT_PP_EQ((PTL_NOR(1, 0)), (0));
ASSERT_PP_EQ((PTL_NOR(1, 1)), (0));

ASSERT_PP_EQ((PTL_XNOR(0, 0)), (1));
ASSERT_PP_EQ((PTL_XNOR(0, 1)), (0));
ASSERT_PP_EQ((PTL_XNOR(1, 0)), (0));
ASSERT_PP_EQ((PTL_XNOR(1, 1)), (1));

ASSERT_PP_EQ((PTL_LT(0, 0)), (0));
ASSERT_PP_EQ((PTL_LT(0, 1)), (1));
ASSERT_PP_EQ((PTL_LT(7u, 8u)), (1));
ASSERT_PP_EQ((PTL_LT(PTL_INT(4095u), 0)), (1));
ASSERT_PP_EQ((PTL_LT(2047, 0x800)), (0));
ASSERT_PP_EQ((PTL_LT(0x800, PTL_INT(2048u))), (0));
ASSERT_PP_EQ((PTL_LT(0x800, PTL_INT(2049u))), (1));
ASSERT_PP_EQ((PTL_LT((F, F, F), (0, 0, 0))), (0));
ASSERT_PP_EQ((PTL_LT((0, 0, 0), (F, F, F))), (1));
ASSERT_PP_EQ((PTL_LT((7, F, F), 2048u)), (1));
ASSERT_PP_EQ((PTL_LT(2048u, (7, F, F))), (0));
ASSERT_PP_EQ((PTL_LT(8u, 7u)), (0));
ASSERT_PP_EQ((PTL_LT(0x800, 2047)), (1));
ASSERT_PP_EQ((PTL_LT(0u, 4095u)), (1));
ASSERT_PP_EQ((PTL_LT(4095u, 0u)), (0));
ASSERT_PP_EQ((PTL_LT(2047u, 2046u)), (0));
ASSERT_PP_EQ((PTL_LT(2047u, 2047u)), (0));
ASSERT_PP_EQ((PTL_LT(2047u, 2048u)), (1));
ASSERT_PP_EQ((PTL_LT(1023, 1022)), (0));
ASSERT_PP_EQ((PTL_LT(1023, 1023)), (0));
ASSERT_PP_EQ((PTL_LT(1023, 1024)), (1));

ASSERT_PP_EQ((PTL_GT(0, 0)), (0));
ASSERT_PP_EQ((PTL_GT(0, 1)), (0));
ASSERT_PP_EQ((PTL_GT(7u, 8u)), (0));
ASSERT_PP_EQ((PTL_GT(PTL_INT(4095u), 0)), (0));
ASSERT_PP_EQ((PTL_GT(2047, 0x800)), (1));
ASSERT_PP_EQ((PTL_GT(0x800, PTL_INT(2048u))), (0));
ASSERT_PP_EQ((PTL_GT(0x800, PTL_INT(2049u))), (0));
ASSERT_PP_EQ((PTL_GT(8u, 7u)), (1));
ASSERT_PP_EQ((PTL_GT(0x800, 2047)), (0));
ASSERT_PP_EQ((PTL_GT(0u, 4095u)), (0));
ASSERT_PP_EQ((PTL_GT(4095u, 0u)), (1));
ASSERT_PP_EQ((PTL_GT(2047u, 2046u)), (1));
ASSERT_PP_EQ((PTL_GT(2047u, 2047u)), (0));
ASSERT_PP_EQ((PTL_GT(2047u, 2048u)), (0));
ASSERT_PP_EQ((PTL_GT(1023, 1022)), (1));
ASSERT_PP_EQ((PTL_GT(1023, 1023)), (0));
ASSERT_PP_EQ((PTL_GT(1023, 1024)), (0));

ASSERT_PP_EQ((PTL_LE(0, 0)), (1));
ASSERT_PP_EQ((PTL_LE(0, 1)), (1));
ASSERT_PP_EQ((PTL_LE(7u, 8u)), (1));
ASSERT_PP_EQ((PTL_LE(PTL_INT(4095u), 0)), (1));
ASSERT_PP_EQ((PTL_LE(2047, 0x800)), (0));
ASSERT_PP_EQ((PTL_LE(0x800, PTL_INT(2048u))), (1));
ASSERT_PP_EQ((PTL_LE(0x800, PTL_INT(2049u))), (1));
ASSERT_PP_EQ((PTL_LE(8u, 7u)), (0));
ASSERT_PP_EQ((PTL_LE(0x800, 2047)), (1));
ASSERT_PP_EQ((PTL_LE(0u, 4095u)), (1));
ASSERT_PP_EQ((PTL_LE(4095u, 0u)), (0));
ASSERT_PP_EQ((PTL_LE(2047u, 2046u)), (0));
ASSERT_PP_EQ((PTL_LE(2047u, 2047u)), (1));
ASSERT_PP_EQ((PTL_LE(2047u, 2048u)), (1));
ASSERT_PP_EQ((PTL_LE(1023, 1022)), (0));
ASSERT_PP_EQ((PTL_LE(1023, 1023)), (1));
ASSERT_PP_EQ((PTL_LE(1023, 1024)), (1));

ASSERT_PP_EQ((PTL_GE(0, 0)), (1));
ASSERT_PP_EQ((PTL_GE(0, 1)), (0));
ASSERT_PP_EQ((PTL_GE(7u, 8u)), (0));
ASSERT_PP_EQ((PTL_GE(PTL_INT(4095u), 0)), (0));
ASSERT_PP_EQ((PTL_GE(2047, 0x800)), (1));
ASSERT_PP_EQ((PTL_GE(0x800, PTL_INT(2048u))), (1));
ASSERT_PP_EQ((PTL_GE(0x800, PTL_INT(2049u))), (0));
ASSERT_PP_EQ((PTL_GE(8u, 7u)), (1));
ASSERT_PP_EQ((PTL_GE(0x800, 2047)), (0));
ASSERT_PP_EQ((PTL_GE(0u, 4095u)), (0));
ASSERT_PP_EQ((PTL_GE(4095u, 0u)), (1));
ASSERT_PP_EQ((PTL_GE(2047u, 2046u)), (1));
ASSERT_PP_EQ((PTL_GE(2047u, 2047u)), (1));
ASSERT_PP_EQ((PTL_GE(2047u, 2048u)), (0));
ASSERT_PP_EQ((PTL_GE(1023, 1022)), (1));
ASSERT_PP_EQ((PTL_GE(1023, 1023)), (1));
ASSERT_PP_EQ((PTL_GE(1023, 1024)), (0));

ASSERT_PP_EQ((PTL_EQ(0, 0)), (1));
ASSERT_PP_EQ((PTL_EQ(0, 1)), (0));
ASSERT_PP_EQ((PTL_EQ(7u, 8u)), (0));
ASSERT_PP_EQ((PTL_EQ(PTL_INT(4095u), 0)), (0));
ASSERT_PP_EQ((PTL_EQ(2047, 0x800)), (0));
ASSERT_PP_EQ((PTL_EQ(0x800, PTL_INT(2048u))), (1));
ASSERT_PP_EQ((PTL_EQ(0x800, PTL_INT(2049u))), (0));
ASSERT_PP_EQ((PTL_EQ(8u, 7u)), (0));
ASSERT_PP_EQ((PTL_EQ(0x800, 2047)), (0));
ASSERT_PP_EQ((PTL_EQ(0u, 4095u)), (0));
ASSERT_PP_EQ((PTL_EQ(4095u, 0u)), (0));
ASSERT_PP_EQ((PTL_EQ(2047u, 2046u)), (0));
ASSERT_PP_EQ((PTL_EQ(2047u, 2047u)), (1));
ASSERT_PP_EQ((PTL_EQ(2047u, 2048u)), (0));
ASSERT_PP_EQ((PTL_EQ(1023, 1022)), (0));
ASSERT_PP_EQ((PTL_EQ(1023, 1023)), (1));
ASSERT_PP_EQ((PTL_EQ(1023, 1024)), (0));

ASSERT_PP_EQ((PTL_NE(0, 0)), (0));
ASSERT_PP_EQ((PTL_NE(0, 1)), (1));
ASSERT_PP_EQ((PTL_NE(7u, 8u)), (1));
ASSERT_PP_EQ((PTL_NE(PTL_INT(4095u), 0)), (1));
ASSERT_PP_EQ((PTL_NE(2047, 0x800)), (1));
ASSERT_PP_EQ((PTL_NE(0x800, PTL_INT(2048u))), (0));
ASSERT_PP_EQ((PTL_NE(0x800, PTL_INT(2049u))), (1));
ASSERT_PP_EQ((PTL_NE(8u, 7u)), (1));
ASSERT_PP_EQ((PTL_NE(0x800, 2047)), (1));
ASSERT_PP_EQ((PTL_NE(0u, 4095u)), (1));
ASSERT_PP_EQ((PTL_NE(4095u, 0u)), (1));
ASSERT_PP_EQ((PTL_NE(2047u, 2046u)), (1));
ASSERT_PP_EQ((PTL_NE(2047u, 2047u)), (0));
ASSERT_PP_EQ((PTL_NE(2047u, 2048u)), (1));
ASSERT_PP_EQ((PTL_NE(1023, 1022)), (1));
ASSERT_PP_EQ((PTL_NE(1023, 1023)), (0));
ASSERT_PP_EQ((PTL_NE(1023, 1024)), (1));

ASSERT_PP_EQ((PTL_MIN(0, 0)), (0));
ASSERT_PP_EQ((PTL_MIN(0, 1)), (0));
ASSERT_PP_EQ((PTL_MIN(7u, 8u)), (7u));
ASSERT_PP_EQ((PTL_MIN(PTL_INT(4095u), 0)), (0xFFF));
ASSERT_PP_EQ((PTL_MIN(2047, 0x800)), (0x800));
ASSERT_PP_EQ((PTL_MIN(0x800, PTL_INT(2048u))), (0x800));
ASSERT_PP_EQ((PTL_MIN(0x800, PTL_INT(2049u))), (0x800));
ASSERT_PP_EQ((PTL_MIN(8u, 7u)), (7u));
ASSERT_PP_EQ((PTL_MIN(0x800, 2047)), (0x800));
ASSERT_PP_EQ((PTL_MIN(0u, 4095u)), (0u));
ASSERT_PP_EQ((PTL_MIN(4095u, 0u)), (0u));
ASSERT_PP_EQ((PTL_MIN(2047u, 2046u)), (2046u));
ASSERT_PP_EQ((PTL_MIN(2047u, 2047u)), (2047u));
ASSERT_PP_EQ((PTL_MIN(2047u, 2048u)), (2047u));
ASSERT_PP_EQ((PTL_MIN(1023, 1022)), (1022));
ASSERT_PP_EQ((PTL_MIN(1023, 1023)), (1023));
ASSERT_PP_EQ((PTL_MIN(1023, 1024)), (1023));

ASSERT_PP_EQ((PTL_MAX(0, 0)), (0));
ASSERT_PP_EQ((PTL_MAX(0, 1)), (1));
ASSERT_PP_EQ((PTL_MAX(7u, 8u)), (8u));
ASSERT_PP_EQ((PTL_MAX(PTL_INT(4095u), 0)), (0));
ASSERT_PP_EQ((PTL_MAX(2047, 0x800)), (2047));
ASSERT_PP_EQ((PTL_MAX(0x800, PTL_INT(2048u))), (0x800));
ASSERT_PP_EQ((PTL_MAX(0x800, PTL_INT(2049u))), (0x801));
ASSERT_PP_EQ((PTL_MAX(8u, 7u)), (8u));
ASSERT_PP_EQ((PTL_MAX(0x800, 2047)), (2047));
ASSERT_PP_EQ((PTL_MAX(0u, 4095u)), (4095u));
ASSERT_PP_EQ((PTL_MAX(4095u, 0u)), (4095u));
ASSERT_PP_EQ((PTL_MAX(2047u, 2046u)), (2047u));
ASSERT_PP_EQ((PTL_MAX(2047u, 2047u)), (2047u));
ASSERT_PP_EQ((PTL_MAX(2047u, 2048u)), (2048u));
ASSERT_PP_EQ((PTL_MAX(1023, 1022)), (1023));
ASSERT_PP_EQ((PTL_MAX(1023, 1023)), (1023));
ASSERT_PP_EQ((PTL_MAX(1023, 1024)), (1024));

ASSERT_PP_EQ((PTL_INC(0)), (1));
ASSERT_PP_EQ((PTL_INC(1u)), (2u));
ASSERT_PP_EQ((PTL_INC(2047)), (0x800));
ASSERT_PP_EQ((PTL_INC(0x7FF)), (0x800));
ASSERT_PP_EQ((PTL_INC(15u)), (16u));
ASSERT_PP_EQ((PTL_INC(4095u)), (0u));

ASSERT_PP_EQ((PTL_DEC(1)), (0));
ASSERT_PP_EQ((PTL_DEC(2u)), (1u));
ASSERT_PP_EQ((PTL_DEC(0)), (0xFFF));
ASSERT_PP_EQ((PTL_DEC(0x800)), (0x7FF));
ASSERT_PP_EQ((PTL_DEC(16u)), (15u));
ASSERT_PP_EQ((PTL_DEC(0u)), (4095u));

ASSERT_PP_EQ((PTL_NEG(0)), (0));
ASSERT_PP_EQ((PTL_NEG(1)), (0xFFF));
ASSERT_PP_EQ((PTL_NEG(0xFFF)), (0x001));
ASSERT_PP_EQ((PTL_NEG(1u)), (4095u));

ASSERT_PP_EQ((PTL_EQZ(0)), (1));
ASSERT_PP_EQ((PTL_EQZ(0u)), (1));
ASSERT_PP_EQ((PTL_EQZ(0x000)), (1));
ASSERT_PP_EQ((PTL_EQZ(0x000u)), (1));
ASSERT_PP_EQ((PTL_EQZ((0, 0, 0))), (1));
ASSERT_PP_EQ((PTL_EQZ(1u)), (0));
ASSERT_PP_EQ((PTL_EQZ(2)), (0));
ASSERT_PP_EQ((PTL_EQZ(4095u)), (0));
ASSERT_PP_EQ((PTL_EQZ(0x800)), (0));

ASSERT_PP_EQ((PTL_NEZ(0)), (0));
ASSERT_PP_EQ((PTL_NEZ(0u)), (0));
ASSERT_PP_EQ((PTL_NEZ(0x000)), (0));
ASSERT_PP_EQ((PTL_NEZ(0x000u)), (0));
ASSERT_PP_EQ((PTL_NEZ((0, 0, 0))), (0));
ASSERT_PP_EQ((PTL_NEZ(1u)), (1));
ASSERT_PP_EQ((PTL_NEZ(2)), (1));
ASSERT_PP_EQ((PTL_NEZ(4095u)), (1));
ASSERT_PP_EQ((PTL_NEZ(0x800)), (1));

ASSERT_PP_EQ((PTL_LTZ(0)), (0));
ASSERT_PP_EQ((PTL_LTZ(1)), (0));
ASSERT_PP_EQ((PTL_LTZ(0u)), (0));
ASSERT_PP_EQ((PTL_LTZ(1u)), (0));
ASSERT_PP_EQ((PTL_LTZ(2047)), (0));
ASSERT_PP_EQ((PTL_LTZ(0x800)), (1));
ASSERT_PP_EQ((PTL_LTZ(PTL_INC(2047))), (1));

ASSERT_PP_EQ((PTL_GTZ(0)), (0));
ASSERT_PP_EQ((PTL_GTZ(1)), (1));
ASSERT_PP_EQ((PTL_GTZ(0u)), (0));
ASSERT_PP_EQ((PTL_GTZ(1u)), (1));
ASSERT_PP_EQ((PTL_GTZ(2047)), (1));
ASSERT_PP_EQ((PTL_GTZ(0x800)), (0));
ASSERT_PP_EQ((PTL_GTZ(PTL_INC(2047))), (0));

ASSERT_PP_EQ((PTL_LEZ(0)), (1));
ASSERT_PP_EQ((PTL_LEZ(1)), (0));
ASSERT_PP_EQ((PTL_LEZ(0u)), (1));
ASSERT_PP_EQ((PTL_LEZ(1u)), (0));
ASSERT_PP_EQ((PTL_LEZ(2047)), (0));
ASSERT_PP_EQ((PTL_LEZ(0x800)), (1));
ASSERT_PP_EQ((PTL_LEZ(PTL_INC(2047))), (1));

ASSERT_PP_EQ((PTL_GEZ(0)), (1));
ASSERT_PP_EQ((PTL_GEZ(1)), (1));
ASSERT_PP_EQ((PTL_GEZ(0u)), (1));
ASSERT_PP_EQ((PTL_GEZ(1u)), (1));
ASSERT_PP_EQ((PTL_GEZ(2047)), (1));
ASSERT_PP_EQ((PTL_GEZ(0x800)), (0));
ASSERT_PP_EQ((PTL_GEZ(PTL_INC(2047))), (0));

ASSERT_PP_EQ((PTL_ABS(0)), (0));
ASSERT_PP_EQ((PTL_ABS(1)), (1));
ASSERT_PP_EQ((PTL_ABS(PTL_NEG(1))), (0x001));
ASSERT_PP_EQ((PTL_ABS(PTL_NEG(15))), (0x00F));

ASSERT_PP_EQ((PTL_LOG2(1u)), (0u));
ASSERT_PP_EQ((PTL_LOG2(2)), (1));
ASSERT_PP_EQ((PTL_LOG2(0x004)), (0x002));
ASSERT_PP_EQ((PTL_LOG2(4095u)), (11u));

ASSERT_PP_EQ((PTL_SQRT(0u)), (0u));
ASSERT_PP_EQ((PTL_SQRT(4)), (2));
ASSERT_PP_EQ((PTL_SQRT(0x010)), (0x004));
ASSERT_PP_EQ((PTL_SQRT(4095u)), (63u));

ASSERT_PP_EQ((PTL_FACT(0u)), ());
ASSERT_PP_EQ((PTL_FACT(13)), ());
ASSERT_PP_EQ((PTL_FACT(4)), (2, 2));
ASSERT_PP_EQ((PTL_FACT(12u)), (2u, 2u, 3u));

ASSERT_PP_EQ((PTL_PRIME(PTL_NEG(2))), (0));
ASSERT_PP_EQ((PTL_PRIME(0)), (0));
ASSERT_PP_EQ((PTL_PRIME(1)), (0));
ASSERT_PP_EQ((PTL_PRIME(2)), (1));
ASSERT_PP_EQ((PTL_PRIME(3)), (1));
ASSERT_PP_EQ((PTL_PRIME(4)), (0));
ASSERT_PP_EQ((PTL_PRIME(13)), (1));
ASSERT_PP_EQ((PTL_PRIME(1024)), (0));
ASSERT_PP_EQ((PTL_PRIME(2048u)), (0));

ASSERT_PP_EQ((PTL_BDUMP(0)), (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
ASSERT_PP_EQ((PTL_BDUMP(0x800)), (1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));

ASSERT_PP_EQ((PTL_BSLL(0, 1)), (0));
ASSERT_PP_EQ((PTL_BSLL(1u, 1)), (2u));
ASSERT_PP_EQ((PTL_BSLL(0x002, 2)), (0x008));
ASSERT_PP_EQ((PTL_BSLL(0x002, 3)), (0x010));
ASSERT_PP_EQ((PTL_BSLL(4095u, 3)), (4088u));
ASSERT_PP_EQ((PTL_BSLL(1, 10)), (1024));
ASSERT_PP_EQ((PTL_BSLL(1, 11)), (0x800));
ASSERT_PP_EQ((PTL_BSLL(1, 12)), (0));
ASSERT_PP_EQ((PTL_BSLL(1, 13)), (0));

ASSERT_PP_EQ((PTL_BSRL(0, 1)), (0));
ASSERT_PP_EQ((PTL_BSRL(2, 1)), (1));
ASSERT_PP_EQ((PTL_BSRL(4, 1)), (2));
ASSERT_PP_EQ((PTL_BSRL(4, 2)), (1));
ASSERT_PP_EQ((PTL_BSRL(0x800, 11)), (0x001));
ASSERT_PP_EQ((PTL_BSRL(0x800, 12)), (0x000));
ASSERT_PP_EQ((PTL_BSRL(1, 1)), (0));
ASSERT_PP_EQ((PTL_BSRL(3, 1)), (1));

ASSERT_PP_EQ((PTL_BSRA(0, 1)), (0));
ASSERT_PP_EQ((PTL_BSRA(2, 1)), (1));
ASSERT_PP_EQ((PTL_BSRA(0x800, 1)), (0xC00));
ASSERT_PP_EQ((PTL_BSRA(0x800, 2)), (0xE00));
ASSERT_PP_EQ((PTL_BSRA(0x800, 3)), (0xF00));
ASSERT_PP_EQ((PTL_BSRA(0x800, 4)), (0xF80));
ASSERT_PP_EQ((PTL_BSRA(1, 1)), (0));
ASSERT_PP_EQ((PTL_BSRA(3, 1)), (1));
ASSERT_PP_EQ((PTL_BSRA(4, 1)), (2));
ASSERT_PP_EQ((PTL_BSRA(4, 2)), (1));

ASSERT_PP_EQ((PTL_BNOT(0u)), (4095u));
ASSERT_PP_EQ((PTL_BNOT(0)), (0xFFF));
ASSERT_PP_EQ((PTL_BNOT((7, F, F))), ((8, 0, 0)));
ASSERT_PP_EQ((PTL_BNOT((7, F, F))), ((8, 0, 0)));

ASSERT_PP_EQ((PTL_BAND(0, 0)), (0));
ASSERT_PP_EQ((PTL_BAND(0, 1)), (0));
ASSERT_PP_EQ((PTL_BAND(3, 2)), (2));
ASSERT_PP_EQ((PTL_BAND(5, 6)), (4));
ASSERT_PP_EQ((PTL_BAND(0x800, 0xFFF)), (0x800));

ASSERT_PP_EQ((PTL_BOR(0, 0)), (0));
ASSERT_PP_EQ((PTL_BOR(0, 1)), (1));
ASSERT_PP_EQ((PTL_BOR(3, 4)), (7));
ASSERT_PP_EQ((PTL_BOR(5, 6)), (7));
ASSERT_PP_EQ((PTL_BOR(0x800, 2047)), (0xFFF));

ASSERT_PP_EQ((PTL_BXOR(0, 0)), (0));
ASSERT_PP_EQ((PTL_BXOR(0, 1)), (1));
ASSERT_PP_EQ((PTL_BXOR(2, 1)), (3));
ASSERT_PP_EQ((PTL_BXOR(2, 3)), (1));
ASSERT_PP_EQ((PTL_BXOR(3, 4)), (7));
ASSERT_PP_EQ((PTL_BXOR(5, 6)), (3));
ASSERT_PP_EQ((PTL_BXOR(0x800, 0xFFF)), (0x7FF));

ASSERT_PP_EQ((PTL_BNAND(0, 0)), (0xFFF));
ASSERT_PP_EQ((PTL_BNAND(5, 7)), (0xFFA));
ASSERT_PP_EQ((PTL_BNAND((7, F, F), (F, F, F))), ((8, 0, 0)));

ASSERT_PP_EQ((PTL_BNOR(0, 0)), (0xFFF));
ASSERT_PP_EQ((PTL_BNOR(0, 1)), (0xFFE));
ASSERT_PP_EQ((PTL_BNOR(5, 7)), (0xFF8));
ASSERT_PP_EQ((PTL_BNOR(7, 8)), (0xFF0));

ASSERT_PP_EQ((PTL_BXNOR(0, 0)), (0xFFF));
ASSERT_PP_EQ((PTL_BXNOR(0, 1)), (0xFFE));
ASSERT_PP_EQ((PTL_BXNOR(5, 7)), (0xFFD));
ASSERT_PP_EQ((PTL_BXNOR(15, 8)), (0xFF8));

ASSERT_PP_EQ((PTL_BGET(2, 2)), (0));
ASSERT_PP_EQ((PTL_BGET(2, 1)), (1));
ASSERT_PP_EQ((PTL_BGET(2, 0)), (0));
ASSERT_PP_EQ((PTL_BGET(5u, 2)), (1));
ASSERT_PP_EQ((PTL_BGET(0xFFE, 1)), (1));
ASSERT_PP_EQ((PTL_BGET(0xFFEu, 0)), (0));
ASSERT_PP_EQ((PTL_BGET((F, F, F), 0)), (1));

ASSERT_PP_EQ((PTL_BSET(0, 1, 1)), (2));
ASSERT_PP_EQ((PTL_BSET(1u, 2, 1)), (5u));
ASSERT_PP_EQ((PTL_BSET(5, 4, 1)), (21));
ASSERT_PP_EQ((PTL_BSET(0x002, 0, 1)), (0x003));
ASSERT_PP_EQ((PTL_BSET(0x003u, 0, 0)), (0x002u));
ASSERT_PP_EQ((PTL_BSET((F, F, F), 11, 0)), ((7, F, F)));

ASSERT_PP_EQ((PTL_BFLIP(0, 0)), (1));
ASSERT_PP_EQ((PTL_BFLIP(1u, 1)), (3u));
ASSERT_PP_EQ((PTL_BFLIP(0x002, 2)), (0x006));
ASSERT_PP_EQ((PTL_BFLIP(0x003u, 3)), (0x00Bu));
ASSERT_PP_EQ((PTL_BFLIP((F, F, F), 11)), ((7, F, F)));

ASSERT_PP_EQ((PTL_BROTL(0x000, 0)), (0x000));
ASSERT_PP_EQ((PTL_BROTL(0x001, 1)), (0x002));
ASSERT_PP_EQ((PTL_BROTL(0x001, 2)), (0x004));
ASSERT_PP_EQ((PTL_BROTL(0x003, 2)), (0x00C));

ASSERT_PP_EQ((PTL_BROTR(0x000, 0)), (0x000));
ASSERT_PP_EQ((PTL_BROTR(0x001, 0)), (0x001));
ASSERT_PP_EQ((PTL_BROTR(0x001, 1)), (0x800));
ASSERT_PP_EQ((PTL_BROTR(0x002, 1)), (0x001));
ASSERT_PP_EQ((PTL_BROTR(0x7FF, 2)), (0xDFF));

ASSERT_PP_EQ((PTL_LP() PTL_RP()), (( )));

ASSERT_PP_EQ((PTL_X()), ());
ASSERT_PP_EQ((PTL_X(foo)), (foo));
ASSERT_PP_EQ((PTL_X(a, b, c)), (a, b, c));
ASSERT_PP_EQ((PTL_XSTR(PTL_INC PTL_LP() 3 PTL_RP())), ("PTL_INC ( 3 )"));
ASSERT_PP_EQ((PTL_XSTR(PTL_X(PTL_INC PTL_LP() 3 PTL_RP()))), ("4"));

ASSERT_PP_EQ((PTL_XSTR(PTL_XTRACE)), ("PPUTLXTRACE_A ( , )"));
ASSERT_PP_EQ((PTL_XSTR(PTL_X(PTL_XTRACE))), ("PPUTLXTRACE_B ( ,, )"));
ASSERT_PP_EQ((PTL_XSTR(PTL_X(PTL_X(PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,, )"));
ASSERT_PP_EQ((PTL_XSTR(PTL_X(PTL_X(PTL_X(PTL_XTRACE))))), ("PPUTLXTRACE_B ( ,,,, )"));

ASSERT_PP_EQ((PTL_XTRACE_READ(PTL_XTRACE)), (0u));
ASSERT_PP_EQ((PTL_XTRACE_READ(PTL_X(PTL_XTRACE))), (1u));
ASSERT_PP_EQ((PTL_XTRACE_READ(PTL_X(PTL_X(PTL_XTRACE)))), (2u));
ASSERT_PP_EQ((PTL_XTRACE_READ(PTL_X(PTL_X(PTL_X(PTL_XTRACE))))), (3u));
ASSERT_PP_EQ((PTL_XTRACE_READ(PPUTLXTRACE_A ( ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,, ))), (255u));

ASSERT_PP_EQ((PTL_XSTR(PTL_RECUR_LP(0, PTL_INC) 0 PTL_RECUR_RP(0))), ("0"));
ASSERT_PP_EQ((PTL_XSTR(PTL_RECUR_LP(1, PTL_INC) 0 PTL_RECUR_RP(1))), ("PTL_INC ( 0 )"));
ASSERT_PP_EQ((PTL_XSTR(PTL_RECUR_LP(2, PTL_INC) 0 PTL_RECUR_RP(2))), ("PTL_INC ( PTL_INC ( 0 ) )"));
ASSERT_PP_EQ((PTL_X(PTL_RECUR_LP(3, PTL_INC) 0 PTL_RECUR_RP(3))), (3));
ASSERT_PP_EQ((PTL_XSTR(PTL_RECUR_LP(3, PTL_INC) 0 PTL_RECUR_RP(3))), ("PTL_INC ( PTL_INC ( PTL_INC ( 0 ) ) )"));
ASSERT_PP_EQ((PTL_X(PTL_RECUR_LP(2, PTL_DEC) PTL_RECUR_LP(3, PTL_INC) 0 PTL_RECUR_RP(3) PTL_RECUR_RP(2))), (1));
ASSERT_PP_EQ((PTL_XTRACE_READ(PTL_X(PTL_RECUR_LP(0, PTL_ESC) PTL_XTRACE PTL_RECUR_RP(0)))), (1u));
ASSERT_PP_EQ((PTL_XTRACE_READ(PTL_X(PTL_RECUR_LP(1, PTL_ESC) PTL_XTRACE PTL_RECUR_RP(1)))), (2u));
ASSERT_PP_EQ((PTL_XTRACE_READ(PTL_X(PTL_RECUR_LP(2, PTL_ESC) PTL_XTRACE PTL_RECUR_RP(2)))), (3u));
ASSERT_PP_EQ((PTL_XTRACE_READ(PTL_X(PTL_RECUR_LP(3, PTL_ESC) PTL_XTRACE PTL_RECUR_RP(3)))), (4u));
ASSERT_PP_EQ((PTL_XTRACE_READ(PTL_X(PTL_RECUR_LP(4, PTL_ESC) PTL_XTRACE PTL_RECUR_RP(4)))), (5u));
ASSERT_PP_EQ((PTL_XTRACE_READ(PTL_X(PTL_RECUR_LP(5, PTL_ESC) PTL_XTRACE PTL_RECUR_RP(5)))), (6u));
ASSERT_PP_EQ((PTL_XTRACE_READ(PTL_X(PTL_RECUR_LP(6, PTL_ESC) PTL_XTRACE PTL_RECUR_RP(6)))), (7u));
ASSERT_PP_EQ((PTL_XTRACE_READ(PTL_X(PTL_RECUR_LP(254, PTL_ESC) PTL_XTRACE PTL_RECUR_RP(254)))), (255u));

ASSERT_PP_EQ((PTL_IF(1, t,)), (t));
ASSERT_PP_EQ((PTL_IF(0, t,)), ());
ASSERT_PP_EQ((PTL_IF(1, t, f)), (t));
ASSERT_PP_EQ((PTL_IF(0, t, f)), (f));

ASSERT_PP_EQ((PTL_SWITCH(0, 1)), (1));
ASSERT_PP_EQ((PTL_SWITCH(1, 1)), (1));
ASSERT_PP_EQ((PTL_SWITCH(2, 1)), (1));
ASSERT_PP_EQ((PTL_SWITCH(1, 1, 2)), (2));
ASSERT_PP_EQ((PTL_SWITCH(2, 1, 2)), (2));
ASSERT_PP_EQ((PTL_SWITCH(255, 1, 2, 3)), (3));
ASSERT_PP_EQ((PTL_SWITCH(0, 1, 2)), (1));
ASSERT_PP_EQ((PTL_SWITCH(1, 1, 2, 3)), (2));
ASSERT_PP_EQ((PTL_SWITCH(2, 1, 2, 3)), (3));
ASSERT_PP_EQ((PTL_SWITCH(0, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, ba, bb, bc, bd, be, bf, bg, bh, bi, bj, bk, bl, bm, bn, bo, bp, bq, br, bs, bt, bu, bv, bw, bx, by, bz, bA, bB, bC, bD, bE, bF, bG, bH, bI, bJ, bK, bL, bM, bN, bO, bP, bQ, bR, bS, bT, bU, bV, bW, bX, bY, bZ, ca, cb, cc, cd, ce, cf, cg, ch, ci, cj, ck, cl, cm, cn, co, cp, cq, cr, cs, ct, cu, cv, cw, cx, cy, cz, cA, cB, cC, cD, cE, cF, cG, cH, cI, cJ, cK, cL, cM, cN, cO, cP, cQ, cR, cS, cT, cU, cV, cW, cX, cY, cZ, da, db, dc, dd, de, df, dg, dh, di, dj, dk, dl, dm, dn, do, dp, dq, dr, ds, dt, du, dv, dw, dx, dy, dz, dA, dB, dC, dD, dE, dF, dG, dH, dI, dJ, dK, dL, dM, dN, dO, dP, dQ, dR, dS, dT, dU, dV, dW, dX, dY, dZ, ea, eb, ec, ed, ee, ef, eg, eh, ei, ej, ek, el, em, en, eo, ep, eq, er, es, et, eu, ev, ew, ex, ey, ez, eA, eB, eC, eD, eE, eF, eG, eH, eI, eJ, eK, eL, eM, eN, eO, eP, eQ, eR, eS, eT, eU, eV)), (a));
ASSERT_PP_EQ((PTL_SWITCH(1, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, ba, bb, bc, bd, be, bf, bg, bh, bi, bj, bk, bl, bm, bn, bo, bp, bq, br, bs, bt, bu, bv, bw, bx, by, bz, bA, bB, bC, bD, bE, bF, bG, bH, bI, bJ, bK, bL, bM, bN, bO, bP, bQ, bR, bS, bT, bU, bV, bW, bX, bY, bZ, ca, cb, cc, cd, ce, cf, cg, ch, ci, cj, ck, cl, cm, cn, co, cp, cq, cr, cs, ct, cu, cv, cw, cx, cy, cz, cA, cB, cC, cD, cE, cF, cG, cH, cI, cJ, cK, cL, cM, cN, cO, cP, cQ, cR, cS, cT, cU, cV, cW, cX, cY, cZ, da, db, dc, dd, de, df, dg, dh, di, dj, dk, dl, dm, dn, do, dp, dq, dr, ds, dt, du, dv, dw, dx, dy, dz, dA, dB, dC, dD, dE, dF, dG, dH, dI, dJ, dK, dL, dM, dN, dO, dP, dQ, dR, dS, dT, dU, dV, dW, dX, dY, dZ, ea, eb, ec, ed, ee, ef, eg, eh, ei, ej, ek, el, em, en, eo, ep, eq, er, es, et, eu, ev, ew, ex, ey, ez, eA, eB, eC, eD, eE, eF, eG, eH, eI, eJ, eK, eL, eM, eN, eO, eP, eQ, eR, eS, eT, eU, eV)), (b));
ASSERT_PP_EQ((PTL_SWITCH(255, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, ba, bb, bc, bd, be, bf, bg, bh, bi, bj, bk, bl, bm, bn, bo, bp, bq, br, bs, bt, bu, bv, bw, bx, by, bz, bA, bB, bC, bD, bE, bF, bG, bH, bI, bJ, bK, bL, bM, bN, bO, bP, bQ, bR, bS, bT, bU, bV, bW, bX, bY, bZ, ca, cb, cc, cd, ce, cf, cg, ch, ci, cj, ck, cl, cm, cn, co, cp, cq, cr, cs, ct, cu, cv, cw, cx, cy, cz, cA, cB, cC, cD, cE, cF, cG, cH, cI, cJ, cK, cL, cM, cN, cO, cP, cQ, cR, cS, cT, cU, cV, cW, cX, cY, cZ, da, db, dc, dd, de, df, dg, dh, di, dj, dk, dl, dm, dn, do, dp, dq, dr, ds, dt, du, dv, dw, dx, dy, dz, dA, dB, dC, dD, dE, dF, dG, dH, dI, dJ, dK, dL, dM, dN, dO, dP, dQ, dR, dS, dT, dU, dV, dW, dX, dY, dZ, ea, eb, ec, ed, ee, ef, eg, eh, ei, ej, ek, el, em, en, eo, ep, eq, er, es, et, eu, ev, ew, ex, ey, ez, eA, eB, eC, eD, eE, eF, eG, eH, eI, eJ, eK, eL, eM, eN, eO, eP, eQ, eR, eS, eT, eU, eV)), (eV));

ASSERT_PP_EQ((PTL_ADD(0, 0)), (0));
ASSERT_PP_EQ((PTL_ADD(0, 1)), (1));
ASSERT_PP_EQ((PTL_ADD(1, 2)), (3));
ASSERT_PP_EQ((PTL_ADD(3u, 4)), (7u));
ASSERT_PP_EQ((PTL_ADD(5, 6u)), (11u));
ASSERT_PP_EQ((PTL_ADD(4095u, 1)), (0u));
ASSERT_PP_EQ((PTL_ADD(4095u, 2)), (1u));
ASSERT_PP_EQ((PTL_ADD(4095u, 4095u)), (4094u));
ASSERT_PP_EQ((PTL_ADD(2047, 1)), (0x800));
ASSERT_PP_EQ((PTL_ADD(2047, (0, 0, 1))), (2048u));
ASSERT_PP_EQ((PTL_ADD(1, 4095u)), (0u));
ASSERT_PP_EQ((PTL_ADD(2, 4095u)), (1u));

ASSERT_PP_EQ((PTL_SUB(0, 0)), (0));
ASSERT_PP_EQ((PTL_SUB(0, 1)), (0xFFF));
ASSERT_PP_EQ((PTL_SUB(0u, 1u)), (4095u));
ASSERT_PP_EQ((PTL_SUB(1, 0)), (1));
ASSERT_PP_EQ((PTL_SUB(1, 1)), (0));
ASSERT_PP_EQ((PTL_SUB(3, 1)), (2));
ASSERT_PP_EQ((PTL_SUB(1u, 3u)), (4094u));
ASSERT_PP_EQ((PTL_SUB(0, 0x800)), (0x800));
ASSERT_PP_EQ((PTL_SUB(0u, 0x800)), (2048u));
ASSERT_PP_EQ((PTL_SUB(4095u, 0)), (4095u));
ASSERT_PP_EQ((PTL_SUB(4095u, 1)), (4094u));

ASSERT_PP_EQ((PTL_MUL(0, 0)), (0));
ASSERT_PP_EQ((PTL_MUL(0, 1)), (0));
ASSERT_PP_EQ((PTL_MUL(1, 1)), (1));
ASSERT_PP_EQ((PTL_MUL(1, 2)), (2));
ASSERT_PP_EQ((PTL_MUL(2, 2)), (4));
ASSERT_PP_EQ((PTL_MUL(PTL_NEG(2), 2)), (0xFFC));
ASSERT_PP_EQ((PTL_MUL(PTL_NEG(2), PTL_NEG(2))), (0x004));
ASSERT_PP_EQ((PTL_MUL(4095u, 1)), (4095u));
ASSERT_PP_EQ((PTL_MUL(4095u, 4095u)), (1u));
ASSERT_PP_EQ((PTL_MUL(2047, 4095u)), (2049u));
ASSERT_PP_EQ((PTL_MUL(2, PTL_NEG(2))), (0xFFC));

ASSERT_PP_EQ((PTL_ITEMS(())), ());
ASSERT_PP_EQ((PTL_ITEMS((a))), (a));
ASSERT_PP_EQ((PTL_ITEMS((a, b))), (a, b));
ASSERT_PP_EQ((PTL_ITEMS((a, b, c))), (a, b, c));
ASSERT_PP_EQ((PTL_ITEMS(((a), (b), (c)))), ((a), (b), (c)));
ASSERT_PP_EQ((PTL_ITEMS((, ))), (,));
ASSERT_PP_EQ((PTL_ITEMS((, , ))), (, ,));
ASSERT_PP_EQ((PTL_ITEMS((a, ))), (a,));
ASSERT_PP_EQ((PTL_ITEMS((a, , ))), (a, ,));
ASSERT_PP_EQ((PTL_ITEMS((, a))), (, a));
ASSERT_PP_EQ((PTL_ITEMS((, a, ))), (, a,));
ASSERT_PP_EQ((PTL_ITEMS((, , a))), (, , a));
// clang-format on

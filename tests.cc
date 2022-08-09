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
//    Caution:  macros should be used sparingly or not at all if possible.    //
//    C++ has evolved to facilitate  countless  metaprogramming techniques    //
//    that  should be  preferred in most cases,  as they are  predictable,    //
//    type-safe, scoped, and easier to debug. pputl is primarily  intended    //
//    for  research purposes and for various edge cases that still must be    //
//    solved using  text replacement,  such as optimizations that minimize    //
//    template specializations and syntactical boilerplate reductions.        //
//                                                                            //
//    ABOUT                                                                   //
//    -----                                                                   //
//                                                                            //
//    pputl is a powerful C++ preprocessor utilities library that provides    //
//    many high-level programming constructs  and 12-bit binary arithmetic    //
//    for both unsigned and signed two's complement integers.                 //
//                                                                            //
//    pputl algorithms  are built using a preprocessor syntax manipulation    //
//    technique for constructing inline recursive call stacks that execute    //
//    much faster than mutually-recursive methods.                            //
//                                                                            //
//    pputl requires __VA_ARGS__, __VA_OPT__, and empty variadic arguments    //
//    support (which are guaranteed by C++20) but has no dependencies.        //
//                                                                            //
//    pputl is completely generated and tested  by a custom C++ framework.    //
//    See the codegen/ folder for the full source.                            //
//                                                                            //
//    USAGE                                                                   //
//    -----                                                                   //
//    Copy pputl.h and include. The distribution is single-header.            //
//                                                                            //
//    Modify the head of codegen/codegen.h  to configure the bit length or    //
//    naming preferences and run `make` to regenerate.                        //
//                                                                            //
//    Run `make test` to validate the library on your system.                 //
//                                                                            //
//    FUNDAMENTALS                                                            //
//    ------------                                                            //
//                                                                            //
//    Non-nullary API functions are fully variadic and chainable such that    //
//    the outputs of one may be used as inputs to another. Parameters must    //
//    be fully expanded and distinguishable after the primary expansion.      //
//                                                                            //
//    Tuples are used only when necessary.  Most functions that operate on    //
//    generic data ranges  both input and output a variadic argument list.    //
//    Creating a tuple is trivial but extraction costs an expansion.          //
//                                                                            //
//    pputl defines several types and uses type identification and casting    //
//    for control flow and error reporting. See the [type] section.           //
//                                                                            //
//      tuple : anything in parentheses                                       //
//            :                                                               //
//      int   : <abstract> signed integer                                     //
//      idec  :  [int]   (positive-only) 2s-complement decimal int; e.g. 353  //
//      bool  :   [idec] the literal '1' or '0'                               //
//      ihex  :  [int]   signed hexidecimal integer; e.g. 0x161               //
//            :                                                               //
//      uint  : <abstract> unsigned integer                                   //
//      udec  :  [uint]  unsigned decimal integer;     e.g. 42u               //
//      uhex  :  [uint]  unsigned hexidecimal integer; e.g. 0x02Au            //
//            :                                                               //
//      any   : exactly one generic value                                     //
//                                                                            //
//    Negative ints cannot be represented in decimal  due to concatenation    //
//    restrictions. Arithmetic and bitwise functions attempt to cast their    //
//    results in the same form as their input, but will always return ihex    //
//    when an idec input becomes negative.  Decimal representations can be    //
//    generated for pasting using fmt.paste.                                  //
//                                                                            //
//    pputl errors execute  an invalid preprocessor operation by using the    //
//    concatenation operator (incorrectly) on a string error message.  All    //
//    errors  triggered by  pputl functions  will include  the macro name,   ///
//    a textual description, and the primary expansion arguments.           ////
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
ASSERT_PP_EQ((PTL_CAT(foo, PTL_EAT(bar))), (foo));

ASSERT_PP_EQ((PTL_ISTR()), (""));
ASSERT_PP_EQ((PTL_ISTR(foo, bar)), ("foo, bar"));
ASSERT_PP_EQ((PTL_ISTR(PTL_CAT(foo, bar))), ("PTL_CAT(foo, bar)"));
ASSERT_PP_EQ((PTL_ISTR(foo)), ("foo"));
ASSERT_PP_EQ((PTL_ISTR(, )), (","));
ASSERT_PP_EQ((PTL_ISTR(, , )), (", ,"));
ASSERT_PP_EQ((PTL_ISTR(a, )), ("a,"));
ASSERT_PP_EQ((PTL_ISTR(a, , )), ("a, ,"));
ASSERT_PP_EQ((PTL_ISTR(, a)), (", a"));
ASSERT_PP_EQ((PTL_ISTR(, a, )), (", a,"));
ASSERT_PP_EQ((PTL_ISTR(, , a)), (", , a"));

ASSERT_PP_EQ((PTL_STR()), (""));
ASSERT_PP_EQ((PTL_STR(foo, bar)), ("foo, bar"));
ASSERT_PP_EQ((PTL_STR(PTL_CAT(foo, bar))), ("foobar"));
ASSERT_PP_EQ((PTL_STR(foo)), ("foo"));
ASSERT_PP_EQ((PTL_STR(, )), (","));
ASSERT_PP_EQ((PTL_STR(, , )), (", ,"));
ASSERT_PP_EQ((PTL_STR(a, )), ("a,"));
ASSERT_PP_EQ((PTL_STR(a, , )), ("a, ,"));
ASSERT_PP_EQ((PTL_STR(, a)), (", a"));
ASSERT_PP_EQ((PTL_STR(, a, )), (", a,"));
ASSERT_PP_EQ((PTL_STR(, , a)), (", , a"));

ASSERT_PP_EQ((PTL_FIRST()), ());
ASSERT_PP_EQ((PTL_FIRST(, )), ());
ASSERT_PP_EQ((PTL_FIRST(a)), (a));
ASSERT_PP_EQ((PTL_FIRST(a, b)), (a));

ASSERT_PP_EQ((PTL_REST()), ());
ASSERT_PP_EQ((PTL_REST(, )), ());
ASSERT_PP_EQ((PTL_REST(a)), ());
ASSERT_PP_EQ((PTL_REST(a, b)), (b));
ASSERT_PP_EQ((PTL_REST(a, b, c)), (b, c));
ASSERT_PP_EQ((PTL_REST(PTL_REST(a, b, c))), (c));
ASSERT_PP_EQ((PTL_REST(a, , )), (,));
ASSERT_PP_EQ((PTL_REST(a, b, , )), (b, ,));

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

ASSERT_PP_EQ((PTL_TUPLE(())), (()));
ASSERT_PP_EQ((PTL_TUPLE((1, 2))), ((1, 2)));

ASSERT_PP_EQ((PTL_BOOL(0)), (0));
ASSERT_PP_EQ((PTL_BOOL(1)), (1));

ASSERT_PP_EQ((PTL_UINT(0)), (0u));
ASSERT_PP_EQ((PTL_UINT(1)), (1u));
ASSERT_PP_EQ((PTL_UINT(2u)), (2u));
ASSERT_PP_EQ((PTL_UINT(4095u)), (4095u));
ASSERT_PP_EQ((PTL_UINT(0x000u)), (0x000u));
ASSERT_PP_EQ((PTL_UINT(0xFFF)), (0xFFFu));

ASSERT_PP_EQ((PTL_INT(0)), (0));
ASSERT_PP_EQ((PTL_INT(1u)), (1));
ASSERT_PP_EQ((PTL_INT(0x000)), (0x000));
ASSERT_PP_EQ((PTL_INT(2047)), (2047));
ASSERT_PP_EQ((PTL_INT(0xFFFu)), (0xFFF));
ASSERT_PP_EQ((PTL_INT(4095u)), (0xFFF));

ASSERT_PP_EQ((PTL_UHEX(0)), (0x000u));
ASSERT_PP_EQ((PTL_UHEX(1)), (0x001u));
ASSERT_PP_EQ((PTL_UHEX(5)), (0x005u));
ASSERT_PP_EQ((PTL_UHEX(4095u)), (0xFFFu));
ASSERT_PP_EQ((PTL_UHEX(0x000u)), (0x000u));
ASSERT_PP_EQ((PTL_UHEX(0x001u)), (0x001u));
ASSERT_PP_EQ((PTL_UHEX(0xFFF)), (0xFFFu));

ASSERT_PP_EQ((PTL_IHEX(0)), (0x000));
ASSERT_PP_EQ((PTL_IHEX(1)), (0x001));
ASSERT_PP_EQ((PTL_IHEX(5)), (0x005));
ASSERT_PP_EQ((PTL_IHEX(4095u)), (0xFFF));
ASSERT_PP_EQ((PTL_IHEX(2047u)), (0x7FF));

ASSERT_PP_EQ((PTL_UDEC(0x000u)), (0u));
ASSERT_PP_EQ((PTL_UDEC(1)), (1u));
ASSERT_PP_EQ((PTL_UDEC(5)), (5u));
ASSERT_PP_EQ((PTL_UDEC(0x005u)), (5u));
ASSERT_PP_EQ((PTL_UDEC(0xFFFu)), (4095u));
ASSERT_PP_EQ((PTL_UDEC(0xFFF)), (4095u));

ASSERT_PP_EQ((PTL_IDEC(0x000)), (0));
ASSERT_PP_EQ((PTL_IDEC(0x001)), (1));
ASSERT_PP_EQ((PTL_IDEC(0x005u)), (5));
ASSERT_PP_EQ((PTL_IDEC(0x7FF)), (2047));
ASSERT_PP_EQ((PTL_IDEC(2047)), (2047));

ASSERT_PP_EQ((PTL_ANY(foo)), (foo));

ASSERT_PP_EQ((PTL_TYPEOF((foo))), (PTL_TUPLE));
ASSERT_PP_EQ((PTL_TYPEOF(0)), (PTL_IDEC));
ASSERT_PP_EQ((PTL_TYPEOF(0u)), (PTL_UDEC));
ASSERT_PP_EQ((PTL_TYPEOF(4095)), (PTL_ANY));
ASSERT_PP_EQ((PTL_TYPEOF(4095u)), (PTL_UDEC));
ASSERT_PP_EQ((PTL_TYPEOF(0xFFF)), (PTL_IHEX));
ASSERT_PP_EQ((PTL_TYPEOF(0xFFFu)), (PTL_UHEX));
ASSERT_PP_EQ((PTL_TYPEOF(foo)), (PTL_ANY));

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

ASSERT_PP_EQ((PTL_NAND(0, 0)), (1));
ASSERT_PP_EQ((PTL_NAND(0, 1)), (1));
ASSERT_PP_EQ((PTL_NAND(1, 0)), (1));
ASSERT_PP_EQ((PTL_NAND(1, 1)), (0));

ASSERT_PP_EQ((PTL_NOR(0, 0)), (1));
ASSERT_PP_EQ((PTL_NOR(0, 1)), (0));
ASSERT_PP_EQ((PTL_NOR(1, 0)), (0));
ASSERT_PP_EQ((PTL_NOR(1, 1)), (0));

ASSERT_PP_EQ((PTL_XOR(0, 0)), (0));
ASSERT_PP_EQ((PTL_XOR(0, 1)), (1));
ASSERT_PP_EQ((PTL_XOR(1, 0)), (1));
ASSERT_PP_EQ((PTL_XOR(1, 1)), (0));

ASSERT_PP_EQ((PTL_XNOR(0, 0)), (1));
ASSERT_PP_EQ((PTL_XNOR(0, 1)), (0));
ASSERT_PP_EQ((PTL_XNOR(1, 0)), (0));
ASSERT_PP_EQ((PTL_XNOR(1, 1)), (1));

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

ASSERT_PP_EQ((PTL_IS_SOME()), (0));
ASSERT_PP_EQ((PTL_IS_SOME(foo)), (1));
ASSERT_PP_EQ((PTL_IS_SOME(foo, bar)), (1));
ASSERT_PP_EQ((PTL_IS_SOME(PTL_ESC())), (0));
ASSERT_PP_EQ((PTL_IS_SOME(, )), (1));
ASSERT_PP_EQ((PTL_IS_SOME(, , )), (1));
ASSERT_PP_EQ((PTL_IS_SOME(a, )), (1));
ASSERT_PP_EQ((PTL_IS_SOME(a, , )), (1));
ASSERT_PP_EQ((PTL_IS_SOME(, a)), (1));
ASSERT_PP_EQ((PTL_IS_SOME(, a, )), (1));
ASSERT_PP_EQ((PTL_IS_SOME(, , a)), (1));

ASSERT_PP_EQ((PTL_IS_ANY()), (0));
ASSERT_PP_EQ((PTL_IS_ANY(,)), (0));
ASSERT_PP_EQ((PTL_IS_ANY(foo,)), (0));
ASSERT_PP_EQ((PTL_IS_ANY(foo, bar)), (0));
ASSERT_PP_EQ((PTL_IS_ANY(foo)), (1));
ASSERT_PP_EQ((PTL_IS_ANY((42))), (1));

ASSERT_PP_EQ((PTL_SIZE()), (0u));
ASSERT_PP_EQ((PTL_SIZE(a)), (1u));
ASSERT_PP_EQ((PTL_SIZE(a, b)), (2u));
ASSERT_PP_EQ((PTL_SIZE(, )), (2u));
ASSERT_PP_EQ((PTL_SIZE(a, b, c)), (3u));
ASSERT_PP_EQ((PTL_SIZE(, , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , )), (4095u));
ASSERT_PP_EQ((PTL_SIZE(, , )), (3u));
ASSERT_PP_EQ((PTL_SIZE(a, )), (2u));
ASSERT_PP_EQ((PTL_SIZE(a, , )), (3u));
ASSERT_PP_EQ((PTL_SIZE(, a)), (2u));
ASSERT_PP_EQ((PTL_SIZE(, a, )), (3u));
ASSERT_PP_EQ((PTL_SIZE(, , a)), (3u));

ASSERT_PP_EQ((PTL_IS_TUPLE()), (0));
ASSERT_PP_EQ((PTL_IS_TUPLE(1, 2)), (0));
ASSERT_PP_EQ((PTL_IS_TUPLE(())), (1));
ASSERT_PP_EQ((PTL_IS_TUPLE((1, 2))), (1));
ASSERT_PP_EQ((PTL_IS_TUPLE((), ())), (0));
ASSERT_PP_EQ((PTL_IS_TUPLE(PTL_ESC(()))), (1));
ASSERT_PP_EQ((PTL_IS_TUPLE(PTL_ESC((1, 2)))), (1));
ASSERT_PP_EQ((PTL_IS_TUPLE(, )), (0));
ASSERT_PP_EQ((PTL_IS_TUPLE(, , )), (0));
ASSERT_PP_EQ((PTL_IS_TUPLE(a, )), (0));
ASSERT_PP_EQ((PTL_IS_TUPLE(a, , )), (0));
ASSERT_PP_EQ((PTL_IS_TUPLE(, a)), (0));
ASSERT_PP_EQ((PTL_IS_TUPLE(, a, )), (0));
ASSERT_PP_EQ((PTL_IS_TUPLE(, , a)), (0));
ASSERT_PP_EQ((PTL_IS_TUPLE((, ))), (1));
ASSERT_PP_EQ((PTL_IS_TUPLE((, , ))), (1));
ASSERT_PP_EQ((PTL_IS_TUPLE((a, ))), (1));
ASSERT_PP_EQ((PTL_IS_TUPLE((a, , ))), (1));
ASSERT_PP_EQ((PTL_IS_TUPLE((, a))), (1));
ASSERT_PP_EQ((PTL_IS_TUPLE((, a, ))), (1));
ASSERT_PP_EQ((PTL_IS_TUPLE((, , a))), (1));

ASSERT_PP_EQ((PTL_IS_BOOL()), (0));
ASSERT_PP_EQ((PTL_IS_BOOL(0)), (1));
ASSERT_PP_EQ((PTL_IS_BOOL(1)), (1));
ASSERT_PP_EQ((PTL_IS_BOOL(1u)), (0));
ASSERT_PP_EQ((PTL_IS_BOOL(0b000000000000)), (0));
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

ASSERT_PP_EQ((PTL_IS_UHEX(1)), (0));
ASSERT_PP_EQ((PTL_IS_UHEX(1u)), (0));
ASSERT_PP_EQ((PTL_IS_UHEX(0x000u)), (1));
ASSERT_PP_EQ((PTL_IS_UHEX(0xFFF)), (0));
ASSERT_PP_EQ((PTL_IS_UHEX((), ())), (0));

ASSERT_PP_EQ((PTL_IS_UDEC(1)), (0));
ASSERT_PP_EQ((PTL_IS_UDEC(1u)), (1));
ASSERT_PP_EQ((PTL_IS_UDEC(4095)), (0));
ASSERT_PP_EQ((PTL_IS_UDEC(4095u)), (1));
ASSERT_PP_EQ((PTL_IS_UDEC(0x000u)), (0));
ASSERT_PP_EQ((PTL_IS_UDEC(0xFFF)), (0));
ASSERT_PP_EQ((PTL_IS_UDEC((), ())), (0));

ASSERT_PP_EQ((PTL_IS_IHEX(1)), (0));
ASSERT_PP_EQ((PTL_IS_IHEX(1u)), (0));
ASSERT_PP_EQ((PTL_IS_IHEX(0x000)), (1));
ASSERT_PP_EQ((PTL_IS_IHEX(0xFFF)), (1));
ASSERT_PP_EQ((PTL_IS_IHEX(0xFFFu)), (0));
ASSERT_PP_EQ((PTL_IS_IHEX((), ())), (0));

ASSERT_PP_EQ((PTL_IS_IDEC(1)), (1));
ASSERT_PP_EQ((PTL_IS_IDEC(1u)), (0));
ASSERT_PP_EQ((PTL_IS_IDEC(2047)), (1));
ASSERT_PP_EQ((PTL_IS_IDEC(4095)), (0));
ASSERT_PP_EQ((PTL_IS_IDEC(0x000u)), (0));
ASSERT_PP_EQ((PTL_IS_IDEC(0xFFF)), (0));
ASSERT_PP_EQ((PTL_IS_IDEC((), ())), (0));

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

ASSERT_PP_EQ((PTL_IS_INT()), (0));
ASSERT_PP_EQ((PTL_IS_INT(foo)), (0));
ASSERT_PP_EQ((PTL_IS_INT(0)), (1));
ASSERT_PP_EQ((PTL_IS_INT(0u)), (0));
ASSERT_PP_EQ((PTL_IS_INT(4095)), (0));
ASSERT_PP_EQ((PTL_IS_INT(0x000u)), (0));
ASSERT_PP_EQ((PTL_IS_INT(0xFFF)), (1));
ASSERT_PP_EQ((PTL_IS_INT(0b110u)), (0));
ASSERT_PP_EQ((PTL_IS_INT((), ())), (0));

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

ASSERT_PP_EQ((PTL_BITS(0)), (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
ASSERT_PP_EQ((PTL_BITS(1)), (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
ASSERT_PP_EQ((PTL_BITS(0xFFEu)), (1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0));

ASSERT_PP_EQ((PTL_ID()), ());
ASSERT_PP_EQ((PTL_ID(foo)), (foo));
ASSERT_PP_EQ((PTL_ID(a, b, c)), (a, b, c));

ASSERT_PP_EQ((PTL_STR(PTL_XCT)), ("PPUTLXCT_A ( , )"));
ASSERT_PP_EQ((PTL_STR(PTL_ESC(PTL_XCT))), ("PPUTLXCT_B ( ,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_ESC(PTL_ESC(PTL_XCT)))), ("PPUTLXCT_A ( ,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_ESC(PTL_ESC(PTL_ESC(PTL_XCT))))), ("PPUTLXCT_B ( ,,,, )"));

ASSERT_PP_EQ((PTL_XCT_SIZE(PTL_XCT)), (0u));
ASSERT_PP_EQ((PTL_XCT_SIZE(PTL_ESC(PTL_XCT))), (1u));
ASSERT_PP_EQ((PTL_XCT_SIZE(PTL_ESC(PTL_ESC(PTL_XCT)))), (2u));
ASSERT_PP_EQ((PTL_XCT_SIZE(PTL_ESC(PTL_ESC(PTL_ESC(PTL_XCT))))), (3u));
ASSERT_PP_EQ((PTL_XCT_SIZE(PPUTLXCT_A ( ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,, ))), (4095u));

ASSERT_PP_EQ((PTL_IF(1, (t), ())), (t));
ASSERT_PP_EQ((PTL_IF(0, (t), ())), ());
ASSERT_PP_EQ((PTL_IF(1, (t), (f))), (t));
ASSERT_PP_EQ((PTL_IF(0, (t), (f))), (f));
ASSERT_PP_EQ((PTL_IF(1, (a), (b, c))), (a));
ASSERT_PP_EQ((PTL_IF(0, (a), (b, c))), (b, c));

ASSERT_PP_EQ((PTL_LT(0, 0)), (0));
ASSERT_PP_EQ((PTL_LT(0, 1)), (1));
ASSERT_PP_EQ((PTL_LT(7u, 8u)), (1));
ASSERT_PP_EQ((PTL_LT(PTL_INT(4095u), 0)), (1));
ASSERT_PP_EQ((PTL_LT(2047, 0x800)), (0));
ASSERT_PP_EQ((PTL_LT(0x800, PTL_INT(2048u))), (0));
ASSERT_PP_EQ((PTL_LT(0x800, PTL_INT(2049u))), (1));
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
// clang-format on

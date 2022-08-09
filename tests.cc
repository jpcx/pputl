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
//    many high-level programming constructs  and 10-bit binary arithmetic    //
//    for both unsigned and signed two's complement integers.                 //
//                                                                            //
//    pputl algorithms  are built using a preprocessor syntax manipulation    //
//    technique for constructing inline recursive call stacks that execute    //
//    much faster than mutually-recursive methods.                            //
//                                                                            //
//    pputl requires __VA_ARGS__, __VA_OPT__, and empty variadic arguments    //
//    support (which are guaranteed by C++20) but has no dependencies.        //
//                                                                            //
//    pputl  is completely generated and tested by a custom C++ framework.    //
//    See the codegen/ folder for the full source.                            //
//                                                                            //
//    USAGE                                                                   //
//    -----                                                                   //
//    Copy pputl.h and include. The distribution is single-header.            //
//                                                                            //
//    Modify  the head of  codegen/codegen.h  to configure the bit size or    //
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
//      - tuple   : anything in parentheses                                   //
//      - uint    : [abstract] unsigned integer                               //
//      - int     : [abstract] signed integer                                 //
//      - ubase2  : [uint] unsigned base 2 integer;  e.g. 0b0000101010u       //
//      - ubase10 : [uint] unsigned base 10 integer; e.g. 42u                 //
//      - bool    : [int]  the literal '1' or '0'                             //
//      - ibase2  : [int]  signed base 2 integer;    e.g. 0b1100100100        //
//      - ibase10 : [int]  signed base 10 integer;   e.g. 353                 //
//      - any     : exactly one generic value                                 //
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
ASSERT_PP_EQ((PTL_UINT(1023u)), (1023u));
ASSERT_PP_EQ((PTL_UINT(0b0000000000u)), (0b0000000000u));
ASSERT_PP_EQ((PTL_UINT(0b1111111111)), (0b1111111111u));

ASSERT_PP_EQ((PTL_INT(0)), (0));
ASSERT_PP_EQ((PTL_INT(1u)), (1));
ASSERT_PP_EQ((PTL_INT(0b0000000000)), (0b0000000000));
ASSERT_PP_EQ((PTL_INT(511)), (511));
ASSERT_PP_EQ((PTL_INT(0b0111111111u)), (0b0111111111));
ASSERT_PP_EQ((PTL_INT(1023u)), (0b1111111111));

ASSERT_PP_EQ((PTL_UBASE2(0)), (0b0000000000u));
ASSERT_PP_EQ((PTL_UBASE2(1)), (0b0000000001u));
ASSERT_PP_EQ((PTL_UBASE2(5)), (0b0000000101u));
ASSERT_PP_EQ((PTL_UBASE2(1023u)), (0b1111111111u));
ASSERT_PP_EQ((PTL_UBASE2(0b0000000000u)), (0b0000000000u));
ASSERT_PP_EQ((PTL_UBASE2(0b0000000001u)), (0b0000000001u));
ASSERT_PP_EQ((PTL_UBASE2(0b1111111111)), (0b1111111111u));

ASSERT_PP_EQ((PTL_IBASE2(0)), (0b0000000000));
ASSERT_PP_EQ((PTL_IBASE2(5)), (0b0000000101));
ASSERT_PP_EQ((PTL_IBASE2(1023u)), (0b1111111111));
ASSERT_PP_EQ((PTL_IBASE2(511u)), (0b0111111111));

ASSERT_PP_EQ((PTL_UBASE10(0b0000000000u)), (0u));
ASSERT_PP_EQ((PTL_UBASE10(1)), (1u));
ASSERT_PP_EQ((PTL_UBASE10(0b0000000101)), (5u));
ASSERT_PP_EQ((PTL_UBASE10(0b1111111111u)), (1023u));
ASSERT_PP_EQ((PTL_UBASE10(0b1000000000)), (512u));

ASSERT_PP_EQ((PTL_IBASE10(0b0000000000)), (0));
ASSERT_PP_EQ((PTL_IBASE10(0b0000000101u)), (5));
ASSERT_PP_EQ((PTL_IBASE10(0b0111111111)), (511));
ASSERT_PP_EQ((PTL_IBASE10(511)), (511));

ASSERT_PP_EQ((PTL_ANY(foo)), (foo));

ASSERT_PP_EQ((PTL_TYPEOF((foo))), (PTL_TUPLE));
ASSERT_PP_EQ((PTL_TYPEOF(0)), (PTL_IBASE10));
ASSERT_PP_EQ((PTL_TYPEOF(0u)), (PTL_UBASE10));
ASSERT_PP_EQ((PTL_TYPEOF(1023)), (PTL_ANY));
ASSERT_PP_EQ((PTL_TYPEOF(1023u)), (PTL_UBASE10));
ASSERT_PP_EQ((PTL_TYPEOF(0b1111111111)), (PTL_IBASE2));
ASSERT_PP_EQ((PTL_TYPEOF(0b1111111111u)), (PTL_UBASE2));
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
ASSERT_PP_EQ((PTL_SIZE(, , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , )), (1023u));
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
ASSERT_PP_EQ((PTL_IS_BOOL(0b0000000000)), (0));
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

ASSERT_PP_EQ((PTL_IS_UBASE2(1)), (0));
ASSERT_PP_EQ((PTL_IS_UBASE2(1u)), (0));
ASSERT_PP_EQ((PTL_IS_UBASE2(0b0000000000u)), (1));
ASSERT_PP_EQ((PTL_IS_UBASE2(0b1111111111)), (0));
ASSERT_PP_EQ((PTL_IS_UBASE2((), ())), (0));

ASSERT_PP_EQ((PTL_IS_UBASE10(1)), (0));
ASSERT_PP_EQ((PTL_IS_UBASE10(1u)), (1));
ASSERT_PP_EQ((PTL_IS_UBASE10(1023)), (0));
ASSERT_PP_EQ((PTL_IS_UBASE10(1023u)), (1));
ASSERT_PP_EQ((PTL_IS_UBASE10(0b0000000000u)), (0));
ASSERT_PP_EQ((PTL_IS_UBASE10(0b1111111111)), (0));
ASSERT_PP_EQ((PTL_IS_UBASE10((), ())), (0));

ASSERT_PP_EQ((PTL_IS_IBASE2(1)), (0));
ASSERT_PP_EQ((PTL_IS_IBASE2(1u)), (0));
ASSERT_PP_EQ((PTL_IS_IBASE2(0b0000000000)), (1));
ASSERT_PP_EQ((PTL_IS_IBASE2(0b1111111111)), (1));
ASSERT_PP_EQ((PTL_IS_IBASE2(0b1111111111u)), (0));
ASSERT_PP_EQ((PTL_IS_IBASE2((), ())), (0));

ASSERT_PP_EQ((PTL_IS_IBASE10(1)), (1));
ASSERT_PP_EQ((PTL_IS_IBASE10(1u)), (0));
ASSERT_PP_EQ((PTL_IS_IBASE10(511)), (1));
ASSERT_PP_EQ((PTL_IS_IBASE10(1023)), (0));
ASSERT_PP_EQ((PTL_IS_IBASE10(0b0000000000u)), (0));
ASSERT_PP_EQ((PTL_IS_IBASE10(0b1111111111)), (0));
ASSERT_PP_EQ((PTL_IS_IBASE10((), ())), (0));

ASSERT_PP_EQ((PTL_IS_UINT()), (0));
ASSERT_PP_EQ((PTL_IS_UINT(foo)), (0));
ASSERT_PP_EQ((PTL_IS_UINT(0)), (0));
ASSERT_PP_EQ((PTL_IS_UINT(0u)), (1));
ASSERT_PP_EQ((PTL_IS_UINT(1023)), (0));
ASSERT_PP_EQ((PTL_IS_UINT(1023u)), (1));
ASSERT_PP_EQ((PTL_IS_UINT(0b0000000000u)), (1));
ASSERT_PP_EQ((PTL_IS_UINT(0b1111111111)), (0));
ASSERT_PP_EQ((PTL_IS_UINT(0b110u)), (0));
ASSERT_PP_EQ((PTL_IS_UINT((), ())), (0));

ASSERT_PP_EQ((PTL_IS_INT()), (0));
ASSERT_PP_EQ((PTL_IS_INT(foo)), (0));
ASSERT_PP_EQ((PTL_IS_INT(0)), (1));
ASSERT_PP_EQ((PTL_IS_INT(0u)), (0));
ASSERT_PP_EQ((PTL_IS_INT(1023)), (0));
ASSERT_PP_EQ((PTL_IS_INT(0b0000000000u)), (0));
ASSERT_PP_EQ((PTL_IS_INT(0b1111111111)), (1));
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

ASSERT_PP_EQ((PTL_BITS(0)), (0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
ASSERT_PP_EQ((PTL_BITS(1)), (0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
ASSERT_PP_EQ((PTL_BITS(0b1111111110u)), (1, 1, 1, 1, 1, 1, 1, 1, 1, 0));

ASSERT_PP_EQ((PTL_LT(0, 0)), (0));
ASSERT_PP_EQ((PTL_LT(0, 1)), (1));
ASSERT_PP_EQ((PTL_LT(7u, 8u)), (1));
ASSERT_PP_EQ((PTL_LT(PTL_INT(1023u), 0)), (1));
ASSERT_PP_EQ((PTL_LT(511, 0b1000000000)), (0));
ASSERT_PP_EQ((PTL_LT(0b1000000000, PTL_INT(512u))), (0));
ASSERT_PP_EQ((PTL_LT(0b1000000000, PTL_INT(513u))), (1));
ASSERT_PP_EQ((PTL_LT(8u, 7u)), (0));
ASSERT_PP_EQ((PTL_LT(0b1000000000, 511)), (1));
ASSERT_PP_EQ((PTL_LT(0u, 1023u)), (1));
ASSERT_PP_EQ((PTL_LT(1023u, 0u)), (0));
ASSERT_PP_EQ((PTL_LT(511u, 510u)), (0));
ASSERT_PP_EQ((PTL_LT(511u, 511u)), (0));
ASSERT_PP_EQ((PTL_LT(511u, 512u)), (1));
ASSERT_PP_EQ((PTL_LT(255, 254)), (0));
ASSERT_PP_EQ((PTL_LT(255, 255)), (0));
ASSERT_PP_EQ((PTL_LT(255, 256)), (1));

ASSERT_PP_EQ((PTL_GT(0, 0)), (0));
ASSERT_PP_EQ((PTL_GT(0, 1)), (0));
ASSERT_PP_EQ((PTL_GT(7u, 8u)), (0));
ASSERT_PP_EQ((PTL_GT(PTL_INT(1023u), 0)), (0));
ASSERT_PP_EQ((PTL_GT(511, 0b1000000000)), (1));
ASSERT_PP_EQ((PTL_GT(0b1000000000, PTL_INT(512u))), (0));
ASSERT_PP_EQ((PTL_GT(0b1000000000, PTL_INT(513u))), (0));
ASSERT_PP_EQ((PTL_GT(8u, 7u)), (1));
ASSERT_PP_EQ((PTL_GT(0b1000000000, 511)), (0));
ASSERT_PP_EQ((PTL_GT(0u, 1023u)), (0));
ASSERT_PP_EQ((PTL_GT(1023u, 0u)), (1));
ASSERT_PP_EQ((PTL_GT(511u, 510u)), (1));
ASSERT_PP_EQ((PTL_GT(511u, 511u)), (0));
ASSERT_PP_EQ((PTL_GT(511u, 512u)), (0));
ASSERT_PP_EQ((PTL_GT(255, 254)), (1));
ASSERT_PP_EQ((PTL_GT(255, 255)), (0));
ASSERT_PP_EQ((PTL_GT(255, 256)), (0));

ASSERT_PP_EQ((PTL_LE(0, 0)), (1));
ASSERT_PP_EQ((PTL_LE(0, 1)), (1));
ASSERT_PP_EQ((PTL_LE(7u, 8u)), (1));
ASSERT_PP_EQ((PTL_LE(PTL_INT(1023u), 0)), (1));
ASSERT_PP_EQ((PTL_LE(511, 0b1000000000)), (0));
ASSERT_PP_EQ((PTL_LE(0b1000000000, PTL_INT(512u))), (1));
ASSERT_PP_EQ((PTL_LE(0b1000000000, PTL_INT(513u))), (1));
ASSERT_PP_EQ((PTL_LE(8u, 7u)), (0));
ASSERT_PP_EQ((PTL_LE(0b1000000000, 511)), (1));
ASSERT_PP_EQ((PTL_LE(0u, 1023u)), (1));
ASSERT_PP_EQ((PTL_LE(1023u, 0u)), (0));
ASSERT_PP_EQ((PTL_LE(511u, 510u)), (0));
ASSERT_PP_EQ((PTL_LE(511u, 511u)), (1));
ASSERT_PP_EQ((PTL_LE(511u, 512u)), (1));
ASSERT_PP_EQ((PTL_LE(255, 254)), (0));
ASSERT_PP_EQ((PTL_LE(255, 255)), (1));
ASSERT_PP_EQ((PTL_LE(255, 256)), (1));

ASSERT_PP_EQ((PTL_GE(0, 0)), (1));
ASSERT_PP_EQ((PTL_GE(0, 1)), (0));
ASSERT_PP_EQ((PTL_GE(7u, 8u)), (0));
ASSERT_PP_EQ((PTL_GE(PTL_INT(1023u), 0)), (0));
ASSERT_PP_EQ((PTL_GE(511, 0b1000000000)), (1));
ASSERT_PP_EQ((PTL_GE(0b1000000000, PTL_INT(512u))), (1));
ASSERT_PP_EQ((PTL_GE(0b1000000000, PTL_INT(513u))), (0));
ASSERT_PP_EQ((PTL_GE(8u, 7u)), (1));
ASSERT_PP_EQ((PTL_GE(0b1000000000, 511)), (0));
ASSERT_PP_EQ((PTL_GE(0u, 1023u)), (0));
ASSERT_PP_EQ((PTL_GE(1023u, 0u)), (1));
ASSERT_PP_EQ((PTL_GE(511u, 510u)), (1));
ASSERT_PP_EQ((PTL_GE(511u, 511u)), (1));
ASSERT_PP_EQ((PTL_GE(511u, 512u)), (0));
ASSERT_PP_EQ((PTL_GE(255, 254)), (1));
ASSERT_PP_EQ((PTL_GE(255, 255)), (1));
ASSERT_PP_EQ((PTL_GE(255, 256)), (0));

ASSERT_PP_EQ((PTL_EQ(0, 0)), (1));
ASSERT_PP_EQ((PTL_EQ(0, 1)), (0));
ASSERT_PP_EQ((PTL_EQ(7u, 8u)), (0));
ASSERT_PP_EQ((PTL_EQ(PTL_INT(1023u), 0)), (0));
ASSERT_PP_EQ((PTL_EQ(511, 0b1000000000)), (0));
ASSERT_PP_EQ((PTL_EQ(0b1000000000, PTL_INT(512u))), (1));
ASSERT_PP_EQ((PTL_EQ(0b1000000000, PTL_INT(513u))), (0));
ASSERT_PP_EQ((PTL_EQ(8u, 7u)), (0));
ASSERT_PP_EQ((PTL_EQ(0b1000000000, 511)), (0));
ASSERT_PP_EQ((PTL_EQ(0u, 1023u)), (0));
ASSERT_PP_EQ((PTL_EQ(1023u, 0u)), (0));
ASSERT_PP_EQ((PTL_EQ(511u, 510u)), (0));
ASSERT_PP_EQ((PTL_EQ(511u, 511u)), (1));
ASSERT_PP_EQ((PTL_EQ(511u, 512u)), (0));
ASSERT_PP_EQ((PTL_EQ(255, 254)), (0));
ASSERT_PP_EQ((PTL_EQ(255, 255)), (1));
ASSERT_PP_EQ((PTL_EQ(255, 256)), (0));

ASSERT_PP_EQ((PTL_NE(0, 0)), (0));
ASSERT_PP_EQ((PTL_NE(0, 1)), (1));
ASSERT_PP_EQ((PTL_NE(7u, 8u)), (1));
ASSERT_PP_EQ((PTL_NE(PTL_INT(1023u), 0)), (1));
ASSERT_PP_EQ((PTL_NE(511, 0b1000000000)), (1));
ASSERT_PP_EQ((PTL_NE(0b1000000000, PTL_INT(512u))), (0));
ASSERT_PP_EQ((PTL_NE(0b1000000000, PTL_INT(513u))), (1));
ASSERT_PP_EQ((PTL_NE(8u, 7u)), (1));
ASSERT_PP_EQ((PTL_NE(0b1000000000, 511)), (1));
ASSERT_PP_EQ((PTL_NE(0u, 1023u)), (1));
ASSERT_PP_EQ((PTL_NE(1023u, 0u)), (1));
ASSERT_PP_EQ((PTL_NE(511u, 510u)), (1));
ASSERT_PP_EQ((PTL_NE(511u, 511u)), (0));
ASSERT_PP_EQ((PTL_NE(511u, 512u)), (1));
ASSERT_PP_EQ((PTL_NE(255, 254)), (1));
ASSERT_PP_EQ((PTL_NE(255, 255)), (0));
ASSERT_PP_EQ((PTL_NE(255, 256)), (1));

ASSERT_PP_EQ((PTL_MIN(0, 0)), (0));
ASSERT_PP_EQ((PTL_MIN(0, 1)), (0));
ASSERT_PP_EQ((PTL_MIN(7u, 8u)), (7u));
ASSERT_PP_EQ((PTL_MIN(PTL_INT(1023u), 0)), (0b1111111111));
ASSERT_PP_EQ((PTL_MIN(511, 0b1000000000)), (0b1000000000));
ASSERT_PP_EQ((PTL_MIN(0b1000000000, PTL_INT(512u))), (0b1000000000));
ASSERT_PP_EQ((PTL_MIN(0b1000000000, PTL_INT(513u))), (0b1000000000));
ASSERT_PP_EQ((PTL_MIN(8u, 7u)), (7u));
ASSERT_PP_EQ((PTL_MIN(0b1000000000, 511)), (0b1000000000));
ASSERT_PP_EQ((PTL_MIN(0u, 1023u)), (0u));
ASSERT_PP_EQ((PTL_MIN(1023u, 0u)), (0u));
ASSERT_PP_EQ((PTL_MIN(511u, 510u)), (510u));
ASSERT_PP_EQ((PTL_MIN(511u, 511u)), (511u));
ASSERT_PP_EQ((PTL_MIN(511u, 512u)), (511u));
ASSERT_PP_EQ((PTL_MIN(255, 254)), (254));
ASSERT_PP_EQ((PTL_MIN(255, 255)), (255));
ASSERT_PP_EQ((PTL_MIN(255, 256)), (255));

ASSERT_PP_EQ((PTL_MAX(0, 0)), (0));
ASSERT_PP_EQ((PTL_MAX(0, 1)), (1));
ASSERT_PP_EQ((PTL_MAX(7u, 8u)), (8u));
ASSERT_PP_EQ((PTL_MAX(PTL_INT(1023u), 0)), (0));
ASSERT_PP_EQ((PTL_MAX(511, 0b1000000000)), (511));
ASSERT_PP_EQ((PTL_MAX(0b1000000000, PTL_INT(512u))), (0b1000000000));
ASSERT_PP_EQ((PTL_MAX(0b1000000000, PTL_INT(513u))), (0b1000000001));
ASSERT_PP_EQ((PTL_MAX(8u, 7u)), (8u));
ASSERT_PP_EQ((PTL_MAX(0b1000000000, 511)), (511));
ASSERT_PP_EQ((PTL_MAX(0u, 1023u)), (1023u));
ASSERT_PP_EQ((PTL_MAX(1023u, 0u)), (1023u));
ASSERT_PP_EQ((PTL_MAX(511u, 510u)), (511u));
ASSERT_PP_EQ((PTL_MAX(511u, 511u)), (511u));
ASSERT_PP_EQ((PTL_MAX(511u, 512u)), (512u));
ASSERT_PP_EQ((PTL_MAX(255, 254)), (255));
ASSERT_PP_EQ((PTL_MAX(255, 255)), (255));
ASSERT_PP_EQ((PTL_MAX(255, 256)), (256));

ASSERT_PP_EQ((PTL_BITGET(2, 7)), (0));
ASSERT_PP_EQ((PTL_BITGET(2, 8)), (1));
ASSERT_PP_EQ((PTL_BITGET(2, 9)), (0));
ASSERT_PP_EQ((PTL_BITGET(0b1111111110u, 9)), (0));

ASSERT_PP_EQ((PTL_BITSET(0, 8, 1)), (2));
ASSERT_PP_EQ((PTL_BITSET(1, 7, 1)), (5));
ASSERT_PP_EQ((PTL_BITSET(0b1111111111u, 9, 0)), (0b1111111110u));

ASSERT_PP_EQ((PTL_BITFLIP(0, 9)), (1));
ASSERT_PP_EQ((PTL_BITFLIP(0, 7)), (4));
ASSERT_PP_EQ((PTL_BITFLIP(0b1111111110u, 9)), (0b1111111111u));

ASSERT_PP_EQ((PTL_BITNOT(0u)), (1023u));
ASSERT_PP_EQ((PTL_BITNOT(1u)), (1022u));
ASSERT_PP_EQ((PTL_BITNOT(0)), (0b1111111111));
ASSERT_PP_EQ((PTL_BITNOT(1)), (0b1111111110));
ASSERT_PP_EQ((PTL_BITNOT(0b0000000000u)), (0b1111111111u));
ASSERT_PP_EQ((PTL_BITNOT(0b0000000001u)), (0b1111111110u));

ASSERT_PP_EQ((PTL_BITSHIFT_LEFT(0b1111111111u, 1)), (0b1111111110u));

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
ASSERT_PP_EQ((PTL_XCT_SIZE(PPUTLXCT_A ( ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,, ))), (1023u));

ASSERT_PP_EQ((PTL_IF(1, (t), ())), (t));
ASSERT_PP_EQ((PTL_IF(0, (t), ())), ());
ASSERT_PP_EQ((PTL_IF(1, (t), (f))), (t));
ASSERT_PP_EQ((PTL_IF(0, (t), (f))), (f));
ASSERT_PP_EQ((PTL_IF(1, (a), (b, c))), (a));
ASSERT_PP_EQ((PTL_IF(0, (a), (b, c))), (b, c));
// clang-format on

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
//    countless metaprogramming techniques that are preferable.               //
//                                                                            //
//    This library is built to provide a strong, safe set of functionality    //
//    for edge cases that uniquely benefit from text replacement and would   //
//    would otherwise utilize a separate code generation script, including    //
//    test case generation and template specialization minimizations.         //
//                                                                            //
//    At its core, pputl is primarily a research project that explores new    //
//    language possibilities  regarding the reduction of terse syntax  and    //
//    the implementation of various forms of reflection.                      //
//                                                                            //
//    ABOUT                                                                   //
//    -----                                                                   //
//                                                                            //
//    pputl  is a powerful, typed C++ preprocessor utilities library  that    //
//    implements many high-level programming constructs,  including 12-bit    //
//    signed and unsigned integers with arithmetic and comparison support.    //
//                                                                            //
//    pputl implements  recursive algorithms  by manipulating preprocessor    //
//    syntax to construct inline call stacks that execute much faster than    //
//    mutually-recursive patterns and can be nested indefinitely.             //
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
//    Configuration of the word size and naming scheme  can be achieved by    //
//    modifying the head of codegen/codegen.h and running `make`.             //
//                                                                            //
//    Run `make test` to validate the library on your system.                 //
//                                                                            //
//    TERMINOLOGY                                                             //
//    -----------                                                             //
//                                                                            //
//    pputl defines several types  to describe different kinds of variadic    //
//    arguments  (potentially empty,  comma-delimited tokens of any kind).    //
//    Type identification and conversion is used extensively.                 //
//                                                                            //
//    Each type  is represented by  two functions:  a predicate for traits    //
//    testing, and a constructor that validates, converts, or fails.   All    //
//    functions that use these types in their parameter docs  assert their    //
//    argument sanity by calling the appropriate constructor functions.       //
//                                                                            //
//      none: nothing                                                         //
//      some: <abstract> something; not nothing                               //
//       |- any: exactly one generic value                                    //
//          |- atom: a generic value not surrounded by parentheses            //
//          |   |- bool: a literal '1' or '0'                                 //
//          |   |- nybl: a (4-bit) literal uppercase hex digit [e.g. B]       //
//          |   |- int: <abstract> a word-sized signed integer                //
//          |   |   |- idec: a positive 2s-complement decimal int [e.g. 3]    //
//          |   |   |- ihex: a signed hex integer [e.g. 0x861]                //
//          |   |- uint: <abstract> a word-sized unsigned integer             //
//          |       |- udec: an unsigned decimal integer [e.g. 42u]           //
//          |       |- uhex: an unsigned hex integer [e.g. 0x02Au]            //
//          |- tup: parenthesised items [typedocs: tup, (T...), (T, U), etc.] //
//              |- word: a word-sized tup of nybls [e.g. (6, D, 2)]           //
//                                                                            //
//    FUNDAMENTALS                                                            //
//    ------------                                                            //
//                                                                            //
//    pputl errors execute  an invalid preprocessor operation by using the    //
//    concatenation operator (incorrectly) on a string error message.  All    //
//    errors  triggered by  pputl functions  will include  the macro name,    //
//    a textual description, and its primary expansion arguments.             //
//                                                                            //
//    Non-nullary API functions are fully variadic and chainable such that    //
//    the outputs of one may be used as inputs to another. Parameters must    //
//    be fully expanded and distinguishable after the primary expansion.      //
//                                                                            //
//    Hexadecimal integers are always represented by fixed-length strings.    //
//    Negative ints cannot be represented in decimal  due to concatenation    //
//    restrictions. Arithmetic and bitwise functions attempt to cast their    //
//    results in the same form as their input, but will always return ihex    //
//    when an idec input becomes negative.  Decimal representations can be   ///
//    generated for pasting using fmt.paste.                                ////
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

ASSERT_PP_EQ((PTL_NIBBLE(0, 0, 0, 0)), (0));
ASSERT_PP_EQ((PTL_NIBBLE(0, 0, 1, 0)), (2));
ASSERT_PP_EQ((PTL_NIBBLE(1, 0, 0, 1)), (9));
ASSERT_PP_EQ((PTL_NIBBLE(1, 1, 0, 1)), (D));
ASSERT_PP_EQ((PTL_NIBBLE(1, 1, 1, 1)), (F));
ASSERT_PP_EQ((PTL_NIBBLE(0, 0, 0, 1)), (1));
ASSERT_PP_EQ((PTL_NIBBLE(0, 0, 1, 1)), (3));
ASSERT_PP_EQ((PTL_NIBBLE(0, 1, 0, 0)), (4));
ASSERT_PP_EQ((PTL_NIBBLE(0, 1, 0, 1)), (5));
ASSERT_PP_EQ((PTL_NIBBLE(0, 1, 1, 0)), (6));
ASSERT_PP_EQ((PTL_NIBBLE(0, 1, 1, 1)), (7));
ASSERT_PP_EQ((PTL_NIBBLE(1, 0, 0, 0)), (8));
ASSERT_PP_EQ((PTL_NIBBLE(1, 0, 1, 0)), (A));
ASSERT_PP_EQ((PTL_NIBBLE(1, 0, 1, 1)), (B));
ASSERT_PP_EQ((PTL_NIBBLE(1, 1, 0, 0)), (C));
ASSERT_PP_EQ((PTL_NIBBLE(1, 1, 1, 0)), (E));

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

ASSERT_PP_EQ((PTL_IS_NYBL()), (0));
ASSERT_PP_EQ((PTL_IS_NYBL(0)), (1));
ASSERT_PP_EQ((PTL_IS_NYBL(Q)), (0));
ASSERT_PP_EQ((PTL_IS_NYBL(foo)), (0));
ASSERT_PP_EQ((PTL_IS_NYBL(B)), (1));
ASSERT_PP_EQ((PTL_IS_NYBL(b)), (0));
ASSERT_PP_EQ((PTL_IS_NYBL(F)), (1));

ASSERT_PP_EQ((PTL_IS_INT()), (0));
ASSERT_PP_EQ((PTL_IS_INT(foo)), (0));
ASSERT_PP_EQ((PTL_IS_INT(0)), (1));
ASSERT_PP_EQ((PTL_IS_INT(0u)), (0));
ASSERT_PP_EQ((PTL_IS_INT(4095)), (0));
ASSERT_PP_EQ((PTL_IS_INT(0x000u)), (0));
ASSERT_PP_EQ((PTL_IS_INT(0xFFF)), (1));
ASSERT_PP_EQ((PTL_IS_INT(0b110u)), (0));
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

ASSERT_PP_EQ((PTL_IS_WORD()), (0));
ASSERT_PP_EQ((PTL_IS_WORD(foo)), (0));
ASSERT_PP_EQ((PTL_IS_WORD(0)), (0));
ASSERT_PP_EQ((PTL_IS_WORD(9, B, C)), (0));
ASSERT_PP_EQ((PTL_IS_WORD((9, B, C))), (1));
ASSERT_PP_EQ((PTL_IS_WORD((9, B, C,))), (0));
ASSERT_PP_EQ((PTL_IS_WORD((9, B, C, E))), (0));
ASSERT_PP_EQ((PTL_IS_WORD(())), (0));
ASSERT_PP_EQ((PTL_IS_WORD((0))), (0));

ASSERT_PP_EQ((PTL_NONE()), ());

ASSERT_PP_EQ((PTL_SOME(foo)), (foo));
ASSERT_PP_EQ((PTL_SOME(foo, bar)), (foo, bar));
ASSERT_PP_EQ((PTL_SOME(foo, 42, (, , ))), (foo, 42, (, , )));
ASSERT_PP_EQ((PTL_SOME(, )), (,));

ASSERT_PP_EQ((PTL_ANY(foo)), (foo));

ASSERT_PP_EQ((PTL_ATOM(foo)), (foo));

ASSERT_PP_EQ((PTL_BOOL(0)), (0));
ASSERT_PP_EQ((PTL_BOOL(1)), (1));

ASSERT_PP_EQ((PTL_NYBL(0)), (0));
ASSERT_PP_EQ((PTL_NYBL(1)), (1));
ASSERT_PP_EQ((PTL_NYBL(B)), (B));
ASSERT_PP_EQ((PTL_NYBL(F)), (F));
ASSERT_PP_EQ((PTL_NYBL(2)), (2));
ASSERT_PP_EQ((PTL_NYBL(3)), (3));
ASSERT_PP_EQ((PTL_NYBL(4)), (4));
ASSERT_PP_EQ((PTL_NYBL(5)), (5));
ASSERT_PP_EQ((PTL_NYBL(6)), (6));
ASSERT_PP_EQ((PTL_NYBL(7)), (7));
ASSERT_PP_EQ((PTL_NYBL(8)), (8));
ASSERT_PP_EQ((PTL_NYBL(9)), (9));
ASSERT_PP_EQ((PTL_NYBL(A)), (A));
ASSERT_PP_EQ((PTL_NYBL(C)), (C));
ASSERT_PP_EQ((PTL_NYBL(D)), (D));
ASSERT_PP_EQ((PTL_NYBL(E)), (E));

ASSERT_PP_EQ((PTL_INT(0)), (0));
ASSERT_PP_EQ((PTL_INT(1u)), (1));
ASSERT_PP_EQ((PTL_INT(0x000)), (0x000));
ASSERT_PP_EQ((PTL_INT(2047)), (2047));
ASSERT_PP_EQ((PTL_INT(0xFFFu)), (0xFFF));
ASSERT_PP_EQ((PTL_INT(4095u)), (0xFFF));
ASSERT_PP_EQ((PTL_INT((8, 0, 0))), (0x800));
ASSERT_PP_EQ((PTL_INT((7, F, F))), (0x7FF));

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
ASSERT_PP_EQ((PTL_UINT(1)), (1u));
ASSERT_PP_EQ((PTL_UINT(2u)), (2u));
ASSERT_PP_EQ((PTL_UINT(4095u)), (4095u));
ASSERT_PP_EQ((PTL_UINT(0x000u)), (0x000u));
ASSERT_PP_EQ((PTL_UINT(0xFFF)), (0xFFFu));
ASSERT_PP_EQ((PTL_UINT((0, 0, 0))), (0x000u));
ASSERT_PP_EQ((PTL_UINT((F, F, F))), (0xFFFu));

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

ASSERT_PP_EQ((PTL_WORD(0)), ((0, 0, 0)));
ASSERT_PP_EQ((PTL_WORD(4095u)), ((F, F, F)));
ASSERT_PP_EQ((PTL_WORD(0x800)), ((8, 0, 0)));
ASSERT_PP_EQ((PTL_WORD(2047)), ((7, F, F)));
ASSERT_PP_EQ((PTL_WORD((1, 0, 0))), ((1, 0, 0)));

ASSERT_PP_EQ((PTL_TYPEOF((foo))), (PTL_TUP));
ASSERT_PP_EQ((PTL_TYPEOF(0)), (PTL_IDEC));
ASSERT_PP_EQ((PTL_TYPEOF(0u)), (PTL_UDEC));
ASSERT_PP_EQ((PTL_TYPEOF(D)), (PTL_NYBL));
ASSERT_PP_EQ((PTL_TYPEOF(4095)), (PTL_ATOM));
ASSERT_PP_EQ((PTL_TYPEOF(4095u)), (PTL_UDEC));
ASSERT_PP_EQ((PTL_TYPEOF(0xFFF)), (PTL_IHEX));
ASSERT_PP_EQ((PTL_TYPEOF(0xFFFu)), (PTL_UHEX));
ASSERT_PP_EQ((PTL_TYPEOF(foo)), (PTL_ATOM));
ASSERT_PP_EQ((PTL_TYPEOF(foo, bar)), (PTL_SOME));
ASSERT_PP_EQ((PTL_TYPEOF((A))), (PTL_TUP));
ASSERT_PP_EQ((PTL_TYPEOF((0, 0, 0))), (PTL_WORD));
ASSERT_PP_EQ((PTL_TYPEOF((F, F, F))), (PTL_WORD));
ASSERT_PP_EQ((PTL_TYPEOF()), (PTL_NONE));

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

ASSERT_PP_EQ((PTL_ID()), ());
ASSERT_PP_EQ((PTL_ID(foo)), (foo));
ASSERT_PP_EQ((PTL_ID(a, b, c)), (a, b, c));

ASSERT_PP_EQ((PTL_STR(PTL_XCT)), ("PPUTLXCT_A ( , )"));
ASSERT_PP_EQ((PTL_STR(PTL_ESC(PTL_XCT))), ("PPUTLXCT_B ( ,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_ESC(PTL_ESC(PTL_XCT)))), ("PPUTLXCT_A ( ,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_ESC(PTL_ESC(PTL_ESC(PTL_XCT))))), ("PPUTLXCT_B ( ,,,, )"));

ASSERT_PP_EQ((PTL_SIZE()), (0u));
ASSERT_PP_EQ((PTL_SIZE(a)), (1u));
ASSERT_PP_EQ((PTL_SIZE(a, b)), (2u));
ASSERT_PP_EQ((PTL_SIZE(, )), (2u));
ASSERT_PP_EQ((PTL_SIZE(a, b, c)), (3u));
ASSERT_PP_EQ((PTL_SIZE(,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,)), (4095u));
ASSERT_PP_EQ((PTL_SIZE(, , )), (3u));
ASSERT_PP_EQ((PTL_SIZE(a, )), (2u));
ASSERT_PP_EQ((PTL_SIZE(a, , )), (3u));
ASSERT_PP_EQ((PTL_SIZE(, a)), (2u));
ASSERT_PP_EQ((PTL_SIZE(, a, )), (3u));
ASSERT_PP_EQ((PTL_SIZE(, , a)), (3u));

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

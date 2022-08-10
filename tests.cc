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
//     none: nothing                                                          //
//     some: <abstract> anything                                              //
//      |-any: exactly one generic value                                      //
//         |-tup: anything in parentheses                                     //
//         |-atom: a generic non-tuple value                                  //
//            |- bool: a literal '1' or '0'                                   //
//            |- bit: an alias for bool                                       //
//            |- nybl: a literal uppercase hexadecimal digit (e.g. B)         //
//            |- uint: <abstract> an unsigned integer                         //
//            |   |- udec: an unsigned decimal integer (e.g. 42u)             //
//            |   |- uhex: an unsigned hex integer (e.g. 0x02Au)              //
//            |- int: <abstract> a signed integer                             //
//                |- idec: a positive 2s-complement decimal int (e.g. 353)    //
//                |- ihex: a signed hex integer (e.g. 0x161)                  //
//                                                                            //
//    Hexadecimal integers are always represented by fixed-length strings.    //
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

ASSERT_PP_EQ((PTL_NIBBLE(0, 0, 0, 0)), (0));
ASSERT_PP_EQ((PTL_NIBBLE(0, 0, 1, 0)), (2));
ASSERT_PP_EQ((PTL_NIBBLE(1, 0, 0, 1)), (9));
ASSERT_PP_EQ((PTL_NIBBLE(1, 1, 0, 1)), (D));
ASSERT_PP_EQ((PTL_NIBBLE(1, 1, 1, 1)), (F));

ASSERT_PP_EQ((PTL_NONE()), ());

ASSERT_PP_EQ((PTL_SOME(foo)), (foo));
ASSERT_PP_EQ((PTL_SOME(foo, bar)), (foo, bar));
ASSERT_PP_EQ((PTL_SOME(foo, 42, (, , ))), (foo, 42, (, , )));
ASSERT_PP_EQ((PTL_SOME(, )), (,));

ASSERT_PP_EQ((PTL_ANY(foo)), (foo));

ASSERT_PP_EQ((PTL_TUP(())), (()));
ASSERT_PP_EQ((PTL_TUP((1, 2))), ((1, 2)));

ASSERT_PP_EQ((PTL_ATOM(foo)), (foo));

ASSERT_PP_EQ((PTL_BOOL(0)), (0));
ASSERT_PP_EQ((PTL_BOOL(1)), (1));

ASSERT_PP_EQ((PTL_UINT(0)), (0u));
ASSERT_PP_EQ((PTL_UINT(1)), (1u));
ASSERT_PP_EQ((PTL_UINT(2u)), (2u));
ASSERT_PP_EQ((PTL_UINT(255u)), (255u));
ASSERT_PP_EQ((PTL_UINT(0x00u)), (0x00u));
ASSERT_PP_EQ((PTL_UINT(0xFF)), (0xFFu));

ASSERT_PP_EQ((PTL_INT(0)), (0));
ASSERT_PP_EQ((PTL_INT(1u)), (1));
ASSERT_PP_EQ((PTL_INT(0x00)), (0x00));
ASSERT_PP_EQ((PTL_INT(127)), (127));
ASSERT_PP_EQ((PTL_INT(0xFFu)), (0xFF));
ASSERT_PP_EQ((PTL_INT(255u)), (0xFF));

ASSERT_PP_EQ((PTL_UHEX(0)), (0x00u));
ASSERT_PP_EQ((PTL_UHEX(1)), (0x01u));
ASSERT_PP_EQ((PTL_UHEX(5)), (0x05u));
ASSERT_PP_EQ((PTL_UHEX(255u)), (0xFFu));
ASSERT_PP_EQ((PTL_UHEX(0x00u)), (0x00u));
ASSERT_PP_EQ((PTL_UHEX(0x01u)), (0x01u));
ASSERT_PP_EQ((PTL_UHEX(0xFF)), (0xFFu));

ASSERT_PP_EQ((PTL_IHEX(0)), (0x00));
ASSERT_PP_EQ((PTL_IHEX(1)), (0x01));
ASSERT_PP_EQ((PTL_IHEX(5)), (0x05));
ASSERT_PP_EQ((PTL_IHEX(255u)), (0xFF));
ASSERT_PP_EQ((PTL_IHEX(127u)), (0x7F));

ASSERT_PP_EQ((PTL_UDEC(0x00u)), (0u));
ASSERT_PP_EQ((PTL_UDEC(1)), (1u));
ASSERT_PP_EQ((PTL_UDEC(5)), (5u));
ASSERT_PP_EQ((PTL_UDEC(0x05u)), (5u));
ASSERT_PP_EQ((PTL_UDEC(0xFFu)), (255u));
ASSERT_PP_EQ((PTL_UDEC(0xFF)), (255u));

ASSERT_PP_EQ((PTL_IDEC(0x00)), (0));
ASSERT_PP_EQ((PTL_IDEC(0x01)), (1));
ASSERT_PP_EQ((PTL_IDEC(0x05u)), (5));
ASSERT_PP_EQ((PTL_IDEC(0x7F)), (127));
ASSERT_PP_EQ((PTL_IDEC(127)), (127));

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
// clang-format on

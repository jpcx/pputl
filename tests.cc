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
//    many high-level programming constructs and a 10-bit unsigned integer    //
//    space with binary arithmetic capabilities.                              //
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
//    Modify the head of codegen/codegen.h  to configure the  unsigned bit    //
//    size or naming preferences and run `make` to regenerate.                //
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
//    pputl defines three types: tuple, bool, and uint.  Features that use    //
//    one of  these types  in their  parameter documentation  assert their    //
//    validity by type-casting.  Type casts expand to their original value    //
//    if successful, else they trigger a preprocessor error.                  //
//                                                                            //
//    uint values are one of two subtypes: decimal or binary.  uint may be    //
//    constructed from either of these representations.  Binary values are    //
//    represented using an '0b' prefix and 'u' suffix and their bit length    //
//    is always fixed to the configured uint bits.                            //
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

ASSERT_PP_EQ((PTL_UINT(0)), (0));
ASSERT_PP_EQ((PTL_UINT(1)), (1));
ASSERT_PP_EQ((PTL_UINT(2)), (2));
ASSERT_PP_EQ((PTL_UINT(1023)), (1023));
ASSERT_PP_EQ((PTL_UINT(0b0000000000u)), (0b0000000000u));
ASSERT_PP_EQ((PTL_UINT(0b1111111111u)), (0b1111111111u));

ASSERT_PP_EQ((PTL_BINARY(0)), (0b0000000000u));
ASSERT_PP_EQ((PTL_BINARY(1)), (0b0000000001u));
ASSERT_PP_EQ((PTL_BINARY(1023)), (0b1111111111u));
ASSERT_PP_EQ((PTL_BINARY(0b0000000000u)), (0b0000000000u));
ASSERT_PP_EQ((PTL_BINARY(0b0000000001u)), (0b0000000001u));
ASSERT_PP_EQ((PTL_BINARY(0b1111111111u)), (0b1111111111u));

ASSERT_PP_EQ((PTL_DECIMAL(0)), (0));
ASSERT_PP_EQ((PTL_DECIMAL(1)), (1));
ASSERT_PP_EQ((PTL_DECIMAL(1023)), (1023));
ASSERT_PP_EQ((PTL_DECIMAL(0b0000000000u)), (0));
ASSERT_PP_EQ((PTL_DECIMAL(0b0000000001u)), (1));
ASSERT_PP_EQ((PTL_DECIMAL(0b1111111111u)), (1023));

ASSERT_PP_EQ((PTL_TYPEOF((foo))), (PTL_TUPLE));
ASSERT_PP_EQ((PTL_TYPEOF(0)), (PTL_DECIMAL));
ASSERT_PP_EQ((PTL_TYPEOF(0b1111111111u)), (PTL_BINARY));

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

ASSERT_PP_EQ((PTL_SIZE()), (0));
ASSERT_PP_EQ((PTL_SIZE(a)), (1));
ASSERT_PP_EQ((PTL_SIZE(a, b)), (2));
ASSERT_PP_EQ((PTL_SIZE(, )), (2));
ASSERT_PP_EQ((PTL_SIZE(a, b, c)), (3));
ASSERT_PP_EQ((PTL_SIZE(, , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , )), (1023));
ASSERT_PP_EQ((PTL_SIZE(, , )), (3));
ASSERT_PP_EQ((PTL_SIZE(a, )), (2));
ASSERT_PP_EQ((PTL_SIZE(a, , )), (3));
ASSERT_PP_EQ((PTL_SIZE(, a)), (2));
ASSERT_PP_EQ((PTL_SIZE(, a, )), (3));
ASSERT_PP_EQ((PTL_SIZE(, , a)), (3));

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

ASSERT_PP_EQ((PTL_IS_UINT()), (0));
ASSERT_PP_EQ((PTL_IS_UINT(foo)), (0));
ASSERT_PP_EQ((PTL_IS_UINT(0)), (1));
ASSERT_PP_EQ((PTL_IS_UINT(())), (0));
ASSERT_PP_EQ((PTL_IS_UINT((), ())), (0));
ASSERT_PP_EQ((PTL_IS_UINT(0, 1)), (0));
ASSERT_PP_EQ((PTL_IS_UINT(1023)), (1));
ASSERT_PP_EQ((PTL_IS_UINT(0b0000000000u)), (1));
ASSERT_PP_EQ((PTL_IS_UINT(0b1111111111u)), (1));
ASSERT_PP_EQ((PTL_IS_UINT(0b1111111111)), (0));
ASSERT_PP_EQ((PTL_IS_UINT(0b110u)), (0));
ASSERT_PP_EQ((PTL_IS_UINT(foo, bar)), (0));
ASSERT_PP_EQ((PTL_IS_UINT(1022)), (1));
ASSERT_PP_EQ((PTL_IS_UINT(0, )), (0));
ASSERT_PP_EQ((PTL_IS_UINT(, )), (0));
ASSERT_PP_EQ((PTL_IS_UINT(, , )), (0));
ASSERT_PP_EQ((PTL_IS_UINT(a, )), (0));
ASSERT_PP_EQ((PTL_IS_UINT(a, , )), (0));
ASSERT_PP_EQ((PTL_IS_UINT(, a)), (0));
ASSERT_PP_EQ((PTL_IS_UINT(, a, )), (0));
ASSERT_PP_EQ((PTL_IS_UINT(, , a)), (0));

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

ASSERT_PP_EQ((PTL_IS_BINARY()), (0));
ASSERT_PP_EQ((PTL_IS_BINARY(48)), (0));
ASSERT_PP_EQ((PTL_IS_BINARY(1023)), (0));
ASSERT_PP_EQ((PTL_IS_BINARY(foo)), (0));
ASSERT_PP_EQ((PTL_IS_BINARY(0b0000000000u)), (1));
ASSERT_PP_EQ((PTL_IS_BINARY(0b1111111111u)), (1));

ASSERT_PP_EQ((PTL_IS_DECIMAL()), (0));
ASSERT_PP_EQ((PTL_IS_DECIMAL(48)), (1));
ASSERT_PP_EQ((PTL_IS_DECIMAL(1023)), (1));
ASSERT_PP_EQ((PTL_IS_DECIMAL(foo)), (0));
ASSERT_PP_EQ((PTL_IS_DECIMAL(0b0000000000u)), (0));
ASSERT_PP_EQ((PTL_IS_DECIMAL(0b1111111111u)), (0));

ASSERT_PP_EQ((PTL_BITS(0)), (0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
ASSERT_PP_EQ((PTL_BITS(1)), (0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
ASSERT_PP_EQ((PTL_BITS(0b1111111110u)), (1, 1, 1, 1, 1, 1, 1, 1, 1, 0));

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

ASSERT_PP_EQ((PTL_BITNOT(0)), (1023));
ASSERT_PP_EQ((PTL_BITNOT(1)), (1022));
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

ASSERT_PP_EQ((PTL_XCT_SIZE(PTL_XCT)), (0));
ASSERT_PP_EQ((PTL_XCT_SIZE(PTL_ESC(PTL_XCT))), (1));
ASSERT_PP_EQ((PTL_XCT_SIZE(PTL_ESC(PTL_ESC(PTL_XCT)))), (2));
ASSERT_PP_EQ((PTL_XCT_SIZE(PTL_ESC(PTL_ESC(PTL_ESC(PTL_XCT))))), (3));
ASSERT_PP_EQ((PTL_XCT_SIZE(PPUTLXCT_A ( ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,, ))), (1023));

ASSERT_PP_EQ((PTL_IF(1, (t), ())), (t));
ASSERT_PP_EQ((PTL_IF(0, (t), ())), ());
ASSERT_PP_EQ((PTL_IF(1, (t), (f))), (t));
ASSERT_PP_EQ((PTL_IF(0, (t), (f))), (f));
ASSERT_PP_EQ((PTL_IF(1, (a), (b, c))), (a));
ASSERT_PP_EQ((PTL_IF(0, (a), (b, c))), (b, c));

ASSERT_PP_EQ((PTL_LT(0, 0)), (0));
ASSERT_PP_EQ((PTL_LT(0, 1)), (1));
ASSERT_PP_EQ((PTL_LT(1, 0)), (0));
ASSERT_PP_EQ((PTL_LT(1, 1)), (0));
ASSERT_PP_EQ((PTL_LT(0, 2)), (1));
ASSERT_PP_EQ((PTL_LT(0, 3)), (1));
ASSERT_PP_EQ((PTL_LT(1, 2)), (1));
ASSERT_PP_EQ((PTL_LT(1, 3)), (1));
ASSERT_PP_EQ((PTL_LT(2, 0)), (0));
ASSERT_PP_EQ((PTL_LT(2, 1)), (0));
ASSERT_PP_EQ((PTL_LT(2, 2)), (0));
ASSERT_PP_EQ((PTL_LT(2, 3)), (1));
ASSERT_PP_EQ((PTL_LT(3, 0)), (0));
ASSERT_PP_EQ((PTL_LT(3, 1)), (0));
ASSERT_PP_EQ((PTL_LT(3, 2)), (0));
ASSERT_PP_EQ((PTL_LT(3, 3)), (0));
ASSERT_PP_EQ((PTL_LT(0, 1023)), (1));
ASSERT_PP_EQ((PTL_LT(0, 1022)), (1));
ASSERT_PP_EQ((PTL_LT(1, 1023)), (1));
ASSERT_PP_EQ((PTL_LT(1, 1022)), (1));
ASSERT_PP_EQ((PTL_LT(1023, 0)), (0));
ASSERT_PP_EQ((PTL_LT(1022, 0)), (0));
ASSERT_PP_EQ((PTL_LT(1023, 1)), (0));
ASSERT_PP_EQ((PTL_LT(1022, 1)), (0));
ASSERT_PP_EQ((PTL_LT(1023, 1023)), (0));
ASSERT_PP_EQ((PTL_LT(1023, 1022)), (0));
ASSERT_PP_EQ((PTL_LT(1022, 1023)), (1));
ASSERT_PP_EQ((PTL_LT(1022, 1022)), (0));

ASSERT_PP_EQ((PTL_GT(0, 0)), (0));
ASSERT_PP_EQ((PTL_GT(0, 1)), (0));
ASSERT_PP_EQ((PTL_GT(1, 0)), (1));
ASSERT_PP_EQ((PTL_GT(1, 1)), (0));
ASSERT_PP_EQ((PTL_GT(0, 2)), (0));
ASSERT_PP_EQ((PTL_GT(0, 3)), (0));
ASSERT_PP_EQ((PTL_GT(1, 2)), (0));
ASSERT_PP_EQ((PTL_GT(1, 3)), (0));
ASSERT_PP_EQ((PTL_GT(2, 0)), (1));
ASSERT_PP_EQ((PTL_GT(2, 1)), (1));
ASSERT_PP_EQ((PTL_GT(2, 2)), (0));
ASSERT_PP_EQ((PTL_GT(2, 3)), (0));
ASSERT_PP_EQ((PTL_GT(3, 0)), (1));
ASSERT_PP_EQ((PTL_GT(3, 1)), (1));
ASSERT_PP_EQ((PTL_GT(3, 2)), (1));
ASSERT_PP_EQ((PTL_GT(3, 3)), (0));
ASSERT_PP_EQ((PTL_GT(0, 1023)), (0));
ASSERT_PP_EQ((PTL_GT(0, 1022)), (0));
ASSERT_PP_EQ((PTL_GT(1, 1023)), (0));
ASSERT_PP_EQ((PTL_GT(1, 1022)), (0));
ASSERT_PP_EQ((PTL_GT(1023, 0)), (1));
ASSERT_PP_EQ((PTL_GT(1022, 0)), (1));
ASSERT_PP_EQ((PTL_GT(1023, 1)), (1));
ASSERT_PP_EQ((PTL_GT(1022, 1)), (1));
ASSERT_PP_EQ((PTL_GT(1023, 1023)), (0));
ASSERT_PP_EQ((PTL_GT(1023, 1022)), (1));
ASSERT_PP_EQ((PTL_GT(1022, 1023)), (0));
ASSERT_PP_EQ((PTL_GT(1022, 1022)), (0));

ASSERT_PP_EQ((PTL_LE(0, 0)), (1));
ASSERT_PP_EQ((PTL_LE(0, 1)), (1));
ASSERT_PP_EQ((PTL_LE(1, 0)), (0));
ASSERT_PP_EQ((PTL_LE(1, 1)), (1));
ASSERT_PP_EQ((PTL_LE(0, 2)), (1));
ASSERT_PP_EQ((PTL_LE(0, 3)), (1));
ASSERT_PP_EQ((PTL_LE(1, 2)), (1));
ASSERT_PP_EQ((PTL_LE(1, 3)), (1));
ASSERT_PP_EQ((PTL_LE(2, 0)), (0));
ASSERT_PP_EQ((PTL_LE(2, 1)), (0));
ASSERT_PP_EQ((PTL_LE(2, 2)), (1));
ASSERT_PP_EQ((PTL_LE(2, 3)), (1));
ASSERT_PP_EQ((PTL_LE(3, 0)), (0));
ASSERT_PP_EQ((PTL_LE(3, 1)), (0));
ASSERT_PP_EQ((PTL_LE(3, 2)), (0));
ASSERT_PP_EQ((PTL_LE(3, 3)), (1));
ASSERT_PP_EQ((PTL_LE(0, 1023)), (1));
ASSERT_PP_EQ((PTL_LE(0, 1022)), (1));
ASSERT_PP_EQ((PTL_LE(1, 1023)), (1));
ASSERT_PP_EQ((PTL_LE(1, 1022)), (1));
ASSERT_PP_EQ((PTL_LE(1023, 0)), (0));
ASSERT_PP_EQ((PTL_LE(1022, 0)), (0));
ASSERT_PP_EQ((PTL_LE(1023, 1)), (0));
ASSERT_PP_EQ((PTL_LE(1022, 1)), (0));
ASSERT_PP_EQ((PTL_LE(1023, 1023)), (1));
ASSERT_PP_EQ((PTL_LE(1023, 1022)), (0));
ASSERT_PP_EQ((PTL_LE(1022, 1023)), (1));
ASSERT_PP_EQ((PTL_LE(1022, 1022)), (1));

ASSERT_PP_EQ((PTL_GE(0, 0)), (1));
ASSERT_PP_EQ((PTL_GE(0, 1)), (0));
ASSERT_PP_EQ((PTL_GE(1, 0)), (1));
ASSERT_PP_EQ((PTL_GE(1, 1)), (1));
ASSERT_PP_EQ((PTL_GE(0, 2)), (0));
ASSERT_PP_EQ((PTL_GE(0, 3)), (0));
ASSERT_PP_EQ((PTL_GE(1, 2)), (0));
ASSERT_PP_EQ((PTL_GE(1, 3)), (0));
ASSERT_PP_EQ((PTL_GE(2, 0)), (1));
ASSERT_PP_EQ((PTL_GE(2, 1)), (1));
ASSERT_PP_EQ((PTL_GE(2, 2)), (1));
ASSERT_PP_EQ((PTL_GE(2, 3)), (0));
ASSERT_PP_EQ((PTL_GE(3, 0)), (1));
ASSERT_PP_EQ((PTL_GE(3, 1)), (1));
ASSERT_PP_EQ((PTL_GE(3, 2)), (1));
ASSERT_PP_EQ((PTL_GE(3, 3)), (1));
ASSERT_PP_EQ((PTL_GE(0, 1023)), (0));
ASSERT_PP_EQ((PTL_GE(0, 1022)), (0));
ASSERT_PP_EQ((PTL_GE(1, 1023)), (0));
ASSERT_PP_EQ((PTL_GE(1, 1022)), (0));
ASSERT_PP_EQ((PTL_GE(1023, 0)), (1));
ASSERT_PP_EQ((PTL_GE(1022, 0)), (1));
ASSERT_PP_EQ((PTL_GE(1023, 1)), (1));
ASSERT_PP_EQ((PTL_GE(1022, 1)), (1));
ASSERT_PP_EQ((PTL_GE(1023, 1023)), (1));
ASSERT_PP_EQ((PTL_GE(1023, 1022)), (1));
ASSERT_PP_EQ((PTL_GE(1022, 1023)), (0));
ASSERT_PP_EQ((PTL_GE(1022, 1022)), (1));

ASSERT_PP_EQ((PTL_EQ(0, 0)), (1));
ASSERT_PP_EQ((PTL_EQ(0, 1)), (0));
ASSERT_PP_EQ((PTL_EQ(1, 0)), (0));
ASSERT_PP_EQ((PTL_EQ(1, 1)), (1));
ASSERT_PP_EQ((PTL_EQ(0, 2)), (0));
ASSERT_PP_EQ((PTL_EQ(0, 3)), (0));
ASSERT_PP_EQ((PTL_EQ(1, 2)), (0));
ASSERT_PP_EQ((PTL_EQ(1, 3)), (0));
ASSERT_PP_EQ((PTL_EQ(2, 0)), (0));
ASSERT_PP_EQ((PTL_EQ(2, 1)), (0));
ASSERT_PP_EQ((PTL_EQ(2, 2)), (1));
ASSERT_PP_EQ((PTL_EQ(2, 3)), (0));
ASSERT_PP_EQ((PTL_EQ(3, 0)), (0));
ASSERT_PP_EQ((PTL_EQ(3, 1)), (0));
ASSERT_PP_EQ((PTL_EQ(3, 2)), (0));
ASSERT_PP_EQ((PTL_EQ(3, 3)), (1));
ASSERT_PP_EQ((PTL_EQ(0, 1023)), (0));
ASSERT_PP_EQ((PTL_EQ(0, 1022)), (0));
ASSERT_PP_EQ((PTL_EQ(1, 1023)), (0));
ASSERT_PP_EQ((PTL_EQ(1, 1022)), (0));
ASSERT_PP_EQ((PTL_EQ(1023, 0)), (0));
ASSERT_PP_EQ((PTL_EQ(1022, 0)), (0));
ASSERT_PP_EQ((PTL_EQ(1023, 1)), (0));
ASSERT_PP_EQ((PTL_EQ(1022, 1)), (0));
ASSERT_PP_EQ((PTL_EQ(1023, 1023)), (1));
ASSERT_PP_EQ((PTL_EQ(1023, 1022)), (0));
ASSERT_PP_EQ((PTL_EQ(1022, 1023)), (0));
ASSERT_PP_EQ((PTL_EQ(1022, 1022)), (1));

ASSERT_PP_EQ((PTL_EQ(0, 0)), (1));
ASSERT_PP_EQ((PTL_EQ(0, 1)), (0));
ASSERT_PP_EQ((PTL_EQ(1, 0)), (0));
ASSERT_PP_EQ((PTL_EQ(1, 1)), (1));
ASSERT_PP_EQ((PTL_EQ(0, 2)), (0));
ASSERT_PP_EQ((PTL_EQ(0, 3)), (0));
ASSERT_PP_EQ((PTL_EQ(1, 2)), (0));
ASSERT_PP_EQ((PTL_EQ(1, 3)), (0));
ASSERT_PP_EQ((PTL_EQ(2, 0)), (0));
ASSERT_PP_EQ((PTL_EQ(2, 1)), (0));
ASSERT_PP_EQ((PTL_EQ(2, 2)), (1));
ASSERT_PP_EQ((PTL_EQ(2, 3)), (0));
ASSERT_PP_EQ((PTL_EQ(3, 0)), (0));
ASSERT_PP_EQ((PTL_EQ(3, 1)), (0));
ASSERT_PP_EQ((PTL_EQ(3, 2)), (0));
ASSERT_PP_EQ((PTL_EQ(3, 3)), (1));
ASSERT_PP_EQ((PTL_EQ(0, 1023)), (0));
ASSERT_PP_EQ((PTL_EQ(0, 1022)), (0));
ASSERT_PP_EQ((PTL_EQ(1, 1023)), (0));
ASSERT_PP_EQ((PTL_EQ(1, 1022)), (0));
ASSERT_PP_EQ((PTL_EQ(1023, 0)), (0));
ASSERT_PP_EQ((PTL_EQ(1022, 0)), (0));
ASSERT_PP_EQ((PTL_EQ(1023, 1)), (0));
ASSERT_PP_EQ((PTL_EQ(1022, 1)), (0));
ASSERT_PP_EQ((PTL_EQ(1023, 1023)), (1));
ASSERT_PP_EQ((PTL_EQ(1023, 1022)), (0));
ASSERT_PP_EQ((PTL_EQ(1022, 1023)), (0));
ASSERT_PP_EQ((PTL_EQ(1022, 1022)), (1));

ASSERT_PP_EQ((PTL_MIN(0, 0)), (0));
ASSERT_PP_EQ((PTL_MIN(0, 1)), (0));
ASSERT_PP_EQ((PTL_MIN(1, 0)), (0));
ASSERT_PP_EQ((PTL_MIN(1, 1)), (1));
ASSERT_PP_EQ((PTL_MIN(0, 2)), (0));
ASSERT_PP_EQ((PTL_MIN(0, 3)), (0));
ASSERT_PP_EQ((PTL_MIN(1, 2)), (1));
ASSERT_PP_EQ((PTL_MIN(1, 3)), (1));
ASSERT_PP_EQ((PTL_MIN(2, 0)), (0));
ASSERT_PP_EQ((PTL_MIN(2, 1)), (1));
ASSERT_PP_EQ((PTL_MIN(2, 2)), (2));
ASSERT_PP_EQ((PTL_MIN(2, 3)), (2));
ASSERT_PP_EQ((PTL_MIN(3, 0)), (0));
ASSERT_PP_EQ((PTL_MIN(3, 1)), (1));
ASSERT_PP_EQ((PTL_MIN(3, 2)), (2));
ASSERT_PP_EQ((PTL_MIN(3, 3)), (3));
ASSERT_PP_EQ((PTL_MIN(0, 1023)), (0));
ASSERT_PP_EQ((PTL_MIN(0, 1022)), (0));
ASSERT_PP_EQ((PTL_MIN(1, 1023)), (1));
ASSERT_PP_EQ((PTL_MIN(1, 1022)), (1));
ASSERT_PP_EQ((PTL_MIN(1023, 0)), (0));
ASSERT_PP_EQ((PTL_MIN(1022, 0)), (0));
ASSERT_PP_EQ((PTL_MIN(1023, 1)), (1));
ASSERT_PP_EQ((PTL_MIN(1022, 1)), (1));
ASSERT_PP_EQ((PTL_MIN(1023, 1023)), (1023));
ASSERT_PP_EQ((PTL_MIN(1023, 1022)), (1022));
ASSERT_PP_EQ((PTL_MIN(1022, 1023)), (1022));
ASSERT_PP_EQ((PTL_MIN(1022, 1022)), (1022));

ASSERT_PP_EQ((PTL_MAX(0, 0)), (0));
ASSERT_PP_EQ((PTL_MAX(0, 1)), (1));
ASSERT_PP_EQ((PTL_MAX(1, 0)), (1));
ASSERT_PP_EQ((PTL_MAX(1, 1)), (1));
ASSERT_PP_EQ((PTL_MAX(0, 2)), (2));
ASSERT_PP_EQ((PTL_MAX(0, 3)), (3));
ASSERT_PP_EQ((PTL_MAX(1, 2)), (2));
ASSERT_PP_EQ((PTL_MAX(1, 3)), (3));
ASSERT_PP_EQ((PTL_MAX(2, 0)), (2));
ASSERT_PP_EQ((PTL_MAX(2, 1)), (2));
ASSERT_PP_EQ((PTL_MAX(2, 2)), (2));
ASSERT_PP_EQ((PTL_MAX(2, 3)), (3));
ASSERT_PP_EQ((PTL_MAX(3, 0)), (3));
ASSERT_PP_EQ((PTL_MAX(3, 1)), (3));
ASSERT_PP_EQ((PTL_MAX(3, 2)), (3));
ASSERT_PP_EQ((PTL_MAX(3, 3)), (3));
ASSERT_PP_EQ((PTL_MAX(0, 1023)), (1023));
ASSERT_PP_EQ((PTL_MAX(0, 1022)), (1022));
ASSERT_PP_EQ((PTL_MAX(1, 1023)), (1023));
ASSERT_PP_EQ((PTL_MAX(1, 1022)), (1022));
ASSERT_PP_EQ((PTL_MAX(1023, 0)), (1023));
ASSERT_PP_EQ((PTL_MAX(1022, 0)), (1022));
ASSERT_PP_EQ((PTL_MAX(1023, 1)), (1023));
ASSERT_PP_EQ((PTL_MAX(1022, 1)), (1022));
ASSERT_PP_EQ((PTL_MAX(1023, 1023)), (1023));
ASSERT_PP_EQ((PTL_MAX(1023, 1022)), (1023));
ASSERT_PP_EQ((PTL_MAX(1022, 1023)), (1023));
ASSERT_PP_EQ((PTL_MAX(1022, 1022)), (1022));
// clang-format on

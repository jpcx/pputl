#ifndef PPUTL_H_INCLUDED
#define PPUTL_H_INCLUDED
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
//    for research purposes  and for various edge cases that still must be    //
//    solved using text replacement,  such as certain  optimizations  that    //
//    reduce the number of template specializations.                          //
//                                                                            //
//    ABOUT                                                                   //
//    -----                                                                   //
//                                                                            //
//    pputl is a powerful C++ preprocessor utilities library that provides    //
//    many high-level programming constructs including unsigned arithmetic    //
//    and comparisons,  logic,  control flow,  generation, transformation,    //
//    reduction, function binding, overloading, and range selection. pputl    //
//    is  completely  generated  by a  custom  framework  that  transforms    //
//    shorthand signatures and library invocations into macros, tests, and    //
//    documentation.                                                          //
//                                                                            //
//    pputl makes use of recursion to the maximum extent possible to limit    //
//    the number of macro definitions for the configured unsigned maximum.    //
//    See range.split and algo.reduce for useful examples of the two types    //
//    of recursive calls supported by this library.                           //
//                                                                            //
//    pputl requires __VA_ARGS__, __VA_OPT__, and empty variadic arguments    //
//    support (which are guaranteed by C++20)  but has no dependencies and    //
//    is single-header.                                                       //
//                                                                            //
//    USAGE                                                                   //
//    -----                                                                   //
//    Copy pputl.h and include. The default build uses a 10-bit uint range    //
//    for  arithmetic  and  comparisons.  Integers  overflow and underflow    //
//    according to  standard unsigned rules.  Variadic argument sizes  are    //
//    usually capped by the uint max. Modify the head of codegen/codegen.h    //
//    and run `make` to set custom integer limits and naming preferences.     //
//                                                                            //
//    GUIDELINES                                                              //
//    ----------                                                              //
//                                                                            //
//    Non-nullary API functions are fully variadic and chainable such that    //
//    the outputs of one may be used as inputs to another. Parameters must    //
//    be fully expanded and distinguishable after the primary expansion.      //
//                                                                            //
//    Tuples are used only when necessary;  most functions that operate on    //
//    data ranges both input and output a variadic argument list. Creating    //
//    a tuple is trivial but extraction costs an expansion.                   //
//                                                                            //
//    pputl has three major types: tup, uint, and bool. pputl types verify    //
//    that the type  is as expected  using various detection methods.  Any    //
//    function signature  that accepts  one of these types  as a parameter    //
//    will cast the value using the associated verifier.                      //
//                                                                            //
//    TESTING                                                                 //
//    -------                                                                 //
//    pputl  is statically tested by the build system.  Run `make test` to   ///
//    validate the library or open tests.cc in an LSP-enabled editor.       ////
//                                                                         /////
///////////////////////////////////////////////////////////////////////////// */

/// [config.build]
/// --------------
/// the build number of this pputl release (ISO8601).
#define PTL_BUILD /* -> <c++ int> */ 20220803

/// [config.uint_max]
/// -----------------
/// the maximum value of a pputl unsigned int.
/// upper bound on the number of args for many pputl functions.
/// see the readme code generation section to configure.
#define PTL_UINT_MAX /* -> uint */ 1023

/// [lang.lp]
/// ---------
/// hides a left parens behind an expansion.
/// needed for implementing pair recursion.
#define PTL_LP /* -> <left parens> */ (

/// [lang.rp]
/// ---------
/// hides a right parens behind an expansion.
/// needed for implementing pair recursion.
#define PTL_RP /* -> <left parens> */ )

/// [lang.eat]
/// ----------
/// eats arguments; return nothing.
#define PTL_EAT(...) /* -> <nothing> */

/// [lang.esc]
/// ----------
/// escapes parentheses. identity function.
///
/// PTL_ESC ()        // <nothing>
/// PTL_ESC (a, b, c) // a, b, c
#define PTL_ESC(/* v: any... */...) /* -> ...v */ __VA_ARGS__

/// [lang.str]
/// ----------
/// stringizes args after an expansion.
///
/// PTL_STR()         // ""
/// PTL_STR(foo, bar) // "foo, bar"
#define PTL_STR(/* v: any... */...) /* -> #...v */ PPUTLSTR_X(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLSTR_X(...) #__VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [lang.cat]
/// ----------
/// concatenates two args after an expansion.
/// args must be compatible with the ## operator.
///
/// PTL_CAT(foo, bar)          // foobar
/// PTL_CAT(foo, PTL_EAT(bar)) // foo
#define PTL_CAT(/* a: any, b: any */...) /* -> a##b */ __VA_OPT__(PPUTLCAT_X(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLCAT_X(a, b) a##b

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [lang.first]
/// ------------
/// returns the first argument.
///
/// PTL_FIRST()     // <nothing>
/// PTL_FIRST(, )   // <nothing>
/// PTL_FIRST(a)    // a
/// PTL_FIRST(a, b) // a
#define PTL_FIRST(/* v: any... */...) /* -> v[0] */ __VA_OPT__(PPUTLFIRST_X(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLFIRST_X(_, ...) _

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [lang.rest]
/// -----------
/// returns all arguments except for the first.
///
/// PTL_REST()                  // <nothing>
/// PTL_REST(, )                // <nothing>
/// PTL_REST(a)                 // <nothing>
/// PTL_REST(a, b)              // b
/// PTL_REST(a, b, c)           // b, c
/// PTL_REST(PTL_REST(a, b, c)) // c
/// PTL_REST(a, , )             // ,
/// PTL_REST(a, b, , )          // b, ,
#define PTL_REST(/* v: any... */...) /* -> ...v */ __VA_OPT__(PPUTLREST_X(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLREST_X(_, ...) __VA_ARGS__

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
#define PTL_TRIM(/* v: any... */...) /* -> v[0] ? (v[1:] ? ...v : v[0]) : ...v[1:] */                       \
  PTL_CAT(PPUTLTRIM_, PTL_CAT(PPUTLTRIM_SEL(PTL_FIRST(__VA_ARGS__)), PPUTLTRIM_SEL(PTL_REST(__VA_ARGS__)))) \
  (__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLTRIM_SEL(...)     0##__VA_OPT__(1)
#define PPUTLTRIM_0101(_, ...) _, __VA_ARGS__
#define PPUTLTRIM_010(_, ...)  _
#define PPUTLTRIM_001(_, ...)  __VA_ARGS__
#define PPUTLTRIM_00(...)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.tup]
/// ----------
/// tuple type (any...).
/// expands to t if valid, else, terminates expansion with a self-reference.
///
/// PTL_TUP(())              // ()
/// PTL_TUP((1, 2))          // (1, 2)
/// PTL_STR(PTL_TUP(0))      // "PTL_TUP(0)"
/// PTL_STR(PTL_TUP(1, 2))   // "PTL_TUP(1, 2)"
/// PTL_STR(PTL_TUP(1,))     // "PTL_TUP(1,)"
/// PTL_STR(PTL_TUP(foo))    // "PTL_TUP(foo)"
/// PTL_STR(PTL_TUP((), ())) // "PTL_TUP((), ())"
#define PTL_TUP(/* t: any... */...) /* -> tup{t} */ PPUTLTUP_O(__VA_ARGS__.)(__VA_ARGS__)(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

/// first parentheses; asserts only one arg.
#define PPUTLTUP_O(_, ...)      PPUTLTUP_O_##__VA_OPT__(NO_)##PASS()
#define PPUTLTUP_O_NO_PASS(...) PPUTLTUP_OO_NO_PASS
#define PPUTLTUP_O_PASS(...)    PPUTLTUP_OO

/// second parentheses; asserts tuple.
#define PPUTLTUP_OO(_, ...)      PPUTLTUP_OO_RES(PTL_EAT _)
#define PPUTLTUP_OO_RES(...)     PPUTLTUP_OO_##__VA_OPT__(NO_)##PASS()
#define PPUTLTUP_OO_PASS(...)    PPUTLTUP_PASS
#define PPUTLTUP_OO_NO_PASS(...) PPUTLTUP_FAIL

/// fourth parentheses; returns
#define PPUTLTUP_PASS(...) __VA_ARGS__
#define PPUTLTUP_FAIL(...) PTL_TUP(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.bool]
/// -----------
/// bool type (0 or 1).
/// expands to b if valid, else, terminates expansion with a self-reference.
///
/// PTL_BOOL(0)               // 0
/// PTL_BOOL(1)               // 1
/// PTL_STR(PTL_BOOL(2))      // "PTL_BOOL(2)"
/// PTL_STR(PTL_BOOL(1, 2))   // "PTL_BOOL(1, 2)"
/// PTL_STR(PTL_BOOL(1,))     // "PTL_BOOL(1,)"
/// PTL_STR(PTL_BOOL(foo))    // "PTL_BOOL(foo)"
/// PTL_STR(PTL_BOOL(()))     // "PTL_BOOL(())"
/// PTL_STR(PTL_BOOL((), ())) // "PTL_BOOL((), ())"
#define PTL_BOOL(/* b: any... */...) /* -> bool{b} */ \
  PPUTLBOOL_O(__VA_ARGS__.)(__VA_ARGS__)(__VA_ARGS__)(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

/// first parentheses; asserts only one arg.
#define PPUTLBOOL_O(_, ...)      PPUTLBOOL_O_##__VA_OPT__(NO_)##PASS()
#define PPUTLBOOL_O_NO_PASS(...) PPUTLBOOL_OO_FAIL
#define PPUTLBOOL_O_PASS(...)    PPUTLBOOL_OO

/// second parentheses; asserts non-tuple.
#define PPUTLBOOL_OO(_, ...)      PPUTLBOOL_OO_RES(PTL_EAT _)
#define PPUTLBOOL_OO_RES(...)     PPUTLBOOL_OO_##__VA_OPT__(NO_)##FAIL()
#define PPUTLBOOL_OO_NO_FAIL(...) PPUTLBOOL_OOO
#define PPUTLBOOL_OO_FAIL(...)    PPUTLBOOL_OOO_NO_PASS

/// third parentheses; asserts either 0 or 1.
#define PPUTLBOOL_OOO(...)         PPUTLBOOL_OOO_RES(PPUTLBOOL_CHK_##__VA_ARGS__)
#define PPUTLBOOL_OOO_RES(...)     PPUTLBOOL_OOO_##__VA_OPT__(NO_)##PASS()
#define PPUTLBOOL_OOO_PASS(...)    PPUTLBOOL_PASS
#define PPUTLBOOL_OOO_NO_PASS(...) PPUTLBOOL_FAIL

/// fourth parentheses; returns
#define PPUTLBOOL_PASS(...) __VA_ARGS__
#define PPUTLBOOL_FAIL(...) PTL_BOOL(__VA_ARGS__)

/// concat existence checks
#define PPUTLBOOL_CHK_1
#define PPUTLBOOL_CHK_0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [type.uint]
/// -----------
/// uint type (0 through 1023).
/// expands to n if valid, else, terminates expansion with a self-reference.
///
/// PTL_UINT(0)               // 0
/// PTL_UINT(1)               // 1
/// PTL_UINT(2)               // 2
/// PTL_UINT(1023)            // 1023
/// PTL_STR(PTL_UINT(1024))   // "PTL_UINT(1024)"
/// PTL_STR(PTL_UINT(1, 2))   // "PTL_UINT(1, 2)"
/// PTL_STR(PTL_UINT(1,))     // "PTL_UINT(1,)"
/// PTL_STR(PTL_UINT(foo))    // "PTL_UINT(foo)"
/// PTL_STR(PTL_UINT(()))     // "PTL_UINT(())"
/// PTL_STR(PTL_UINT((), ())) // "PTL_UINT((), ())"
#define PTL_UINT(/* n: any... */...) /* -> uint{n} */ \
  PPUTLUINT_O(__VA_ARGS__.)(__VA_ARGS__)(__VA_ARGS__)(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

/// first parentheses; asserts only one arg.
#define PPUTLUINT_O(_, ...)      PPUTLUINT_O_##__VA_OPT__(NO_)##PASS()
#define PPUTLUINT_O_NO_PASS(...) PPUTLUINT_OO_FAIL
#define PPUTLUINT_O_PASS(...)    PPUTLUINT_OO

/// second parentheses; asserts non-tuple.
#define PPUTLUINT_OO(_, ...)      PPUTLUINT_OO_RES(PTL_EAT _)
#define PPUTLUINT_OO_RES(...)     PPUTLUINT_OO_##__VA_OPT__(NO_)##FAIL()
#define PPUTLUINT_OO_NO_FAIL(...) PPUTLUINT_OOO
#define PPUTLUINT_OO_FAIL(...)    PPUTLUINT_OOO_FAIL

/// third parentheses; asserts one of 0 through 1023.
#define PPUTLUINT_OOO(...)          PPUTLUINT_OOO_RES(PPUTLUINT_RANGE_##__VA_ARGS__)
#define PPUTLUINT_OOO_RES(...)      PPUTLUINT_OOO_RES_X(__VA_ARGS__)
#define PPUTLUINT_OOO_RES_X(_, ...) PPUTLUINT_OOO_##__VA_OPT__(NO_)##FAIL()
#define PPUTLUINT_OOO_NO_FAIL(...)  PPUTLUINT_PASS
#define PPUTLUINT_OOO_FAIL(...)     PPUTLUINT_FAIL

/// fourth parentheses; returns
#define PPUTLUINT_PASS(...) __VA_ARGS__
#define PPUTLUINT_FAIL(...) PTL_UINT(__VA_ARGS__)

/// int decrement and increment values.
#define PPUTLUINT_RANGE_1023 1022, 0
#define PPUTLUINT_RANGE_1022 1021, 1023
#define PPUTLUINT_RANGE_1021 1020, 1022
#define PPUTLUINT_RANGE_1020 1019, 1021
#define PPUTLUINT_RANGE_1019 1018, 1020
#define PPUTLUINT_RANGE_1018 1017, 1019
#define PPUTLUINT_RANGE_1017 1016, 1018
#define PPUTLUINT_RANGE_1016 1015, 1017
#define PPUTLUINT_RANGE_1015 1014, 1016
#define PPUTLUINT_RANGE_1014 1013, 1015
#define PPUTLUINT_RANGE_1013 1012, 1014
#define PPUTLUINT_RANGE_1012 1011, 1013
#define PPUTLUINT_RANGE_1011 1010, 1012
#define PPUTLUINT_RANGE_1010 1009, 1011
#define PPUTLUINT_RANGE_1009 1008, 1010
#define PPUTLUINT_RANGE_1008 1007, 1009
#define PPUTLUINT_RANGE_1007 1006, 1008
#define PPUTLUINT_RANGE_1006 1005, 1007
#define PPUTLUINT_RANGE_1005 1004, 1006
#define PPUTLUINT_RANGE_1004 1003, 1005
#define PPUTLUINT_RANGE_1003 1002, 1004
#define PPUTLUINT_RANGE_1002 1001, 1003
#define PPUTLUINT_RANGE_1001 1000, 1002
#define PPUTLUINT_RANGE_1000 999, 1001
#define PPUTLUINT_RANGE_999  998, 1000
#define PPUTLUINT_RANGE_998  997, 999
#define PPUTLUINT_RANGE_997  996, 998
#define PPUTLUINT_RANGE_996  995, 997
#define PPUTLUINT_RANGE_995  994, 996
#define PPUTLUINT_RANGE_994  993, 995
#define PPUTLUINT_RANGE_993  992, 994
#define PPUTLUINT_RANGE_992  991, 993
#define PPUTLUINT_RANGE_991  990, 992
#define PPUTLUINT_RANGE_990  989, 991
#define PPUTLUINT_RANGE_989  988, 990
#define PPUTLUINT_RANGE_988  987, 989
#define PPUTLUINT_RANGE_987  986, 988
#define PPUTLUINT_RANGE_986  985, 987
#define PPUTLUINT_RANGE_985  984, 986
#define PPUTLUINT_RANGE_984  983, 985
#define PPUTLUINT_RANGE_983  982, 984
#define PPUTLUINT_RANGE_982  981, 983
#define PPUTLUINT_RANGE_981  980, 982
#define PPUTLUINT_RANGE_980  979, 981
#define PPUTLUINT_RANGE_979  978, 980
#define PPUTLUINT_RANGE_978  977, 979
#define PPUTLUINT_RANGE_977  976, 978
#define PPUTLUINT_RANGE_976  975, 977
#define PPUTLUINT_RANGE_975  974, 976
#define PPUTLUINT_RANGE_974  973, 975
#define PPUTLUINT_RANGE_973  972, 974
#define PPUTLUINT_RANGE_972  971, 973
#define PPUTLUINT_RANGE_971  970, 972
#define PPUTLUINT_RANGE_970  969, 971
#define PPUTLUINT_RANGE_969  968, 970
#define PPUTLUINT_RANGE_968  967, 969
#define PPUTLUINT_RANGE_967  966, 968
#define PPUTLUINT_RANGE_966  965, 967
#define PPUTLUINT_RANGE_965  964, 966
#define PPUTLUINT_RANGE_964  963, 965
#define PPUTLUINT_RANGE_963  962, 964
#define PPUTLUINT_RANGE_962  961, 963
#define PPUTLUINT_RANGE_961  960, 962
#define PPUTLUINT_RANGE_960  959, 961
#define PPUTLUINT_RANGE_959  958, 960
#define PPUTLUINT_RANGE_958  957, 959
#define PPUTLUINT_RANGE_957  956, 958
#define PPUTLUINT_RANGE_956  955, 957
#define PPUTLUINT_RANGE_955  954, 956
#define PPUTLUINT_RANGE_954  953, 955
#define PPUTLUINT_RANGE_953  952, 954
#define PPUTLUINT_RANGE_952  951, 953
#define PPUTLUINT_RANGE_951  950, 952
#define PPUTLUINT_RANGE_950  949, 951
#define PPUTLUINT_RANGE_949  948, 950
#define PPUTLUINT_RANGE_948  947, 949
#define PPUTLUINT_RANGE_947  946, 948
#define PPUTLUINT_RANGE_946  945, 947
#define PPUTLUINT_RANGE_945  944, 946
#define PPUTLUINT_RANGE_944  943, 945
#define PPUTLUINT_RANGE_943  942, 944
#define PPUTLUINT_RANGE_942  941, 943
#define PPUTLUINT_RANGE_941  940, 942
#define PPUTLUINT_RANGE_940  939, 941
#define PPUTLUINT_RANGE_939  938, 940
#define PPUTLUINT_RANGE_938  937, 939
#define PPUTLUINT_RANGE_937  936, 938
#define PPUTLUINT_RANGE_936  935, 937
#define PPUTLUINT_RANGE_935  934, 936
#define PPUTLUINT_RANGE_934  933, 935
#define PPUTLUINT_RANGE_933  932, 934
#define PPUTLUINT_RANGE_932  931, 933
#define PPUTLUINT_RANGE_931  930, 932
#define PPUTLUINT_RANGE_930  929, 931
#define PPUTLUINT_RANGE_929  928, 930
#define PPUTLUINT_RANGE_928  927, 929
#define PPUTLUINT_RANGE_927  926, 928
#define PPUTLUINT_RANGE_926  925, 927
#define PPUTLUINT_RANGE_925  924, 926
#define PPUTLUINT_RANGE_924  923, 925
#define PPUTLUINT_RANGE_923  922, 924
#define PPUTLUINT_RANGE_922  921, 923
#define PPUTLUINT_RANGE_921  920, 922
#define PPUTLUINT_RANGE_920  919, 921
#define PPUTLUINT_RANGE_919  918, 920
#define PPUTLUINT_RANGE_918  917, 919
#define PPUTLUINT_RANGE_917  916, 918
#define PPUTLUINT_RANGE_916  915, 917
#define PPUTLUINT_RANGE_915  914, 916
#define PPUTLUINT_RANGE_914  913, 915
#define PPUTLUINT_RANGE_913  912, 914
#define PPUTLUINT_RANGE_912  911, 913
#define PPUTLUINT_RANGE_911  910, 912
#define PPUTLUINT_RANGE_910  909, 911
#define PPUTLUINT_RANGE_909  908, 910
#define PPUTLUINT_RANGE_908  907, 909
#define PPUTLUINT_RANGE_907  906, 908
#define PPUTLUINT_RANGE_906  905, 907
#define PPUTLUINT_RANGE_905  904, 906
#define PPUTLUINT_RANGE_904  903, 905
#define PPUTLUINT_RANGE_903  902, 904
#define PPUTLUINT_RANGE_902  901, 903
#define PPUTLUINT_RANGE_901  900, 902
#define PPUTLUINT_RANGE_900  899, 901
#define PPUTLUINT_RANGE_899  898, 900
#define PPUTLUINT_RANGE_898  897, 899
#define PPUTLUINT_RANGE_897  896, 898
#define PPUTLUINT_RANGE_896  895, 897
#define PPUTLUINT_RANGE_895  894, 896
#define PPUTLUINT_RANGE_894  893, 895
#define PPUTLUINT_RANGE_893  892, 894
#define PPUTLUINT_RANGE_892  891, 893
#define PPUTLUINT_RANGE_891  890, 892
#define PPUTLUINT_RANGE_890  889, 891
#define PPUTLUINT_RANGE_889  888, 890
#define PPUTLUINT_RANGE_888  887, 889
#define PPUTLUINT_RANGE_887  886, 888
#define PPUTLUINT_RANGE_886  885, 887
#define PPUTLUINT_RANGE_885  884, 886
#define PPUTLUINT_RANGE_884  883, 885
#define PPUTLUINT_RANGE_883  882, 884
#define PPUTLUINT_RANGE_882  881, 883
#define PPUTLUINT_RANGE_881  880, 882
#define PPUTLUINT_RANGE_880  879, 881
#define PPUTLUINT_RANGE_879  878, 880
#define PPUTLUINT_RANGE_878  877, 879
#define PPUTLUINT_RANGE_877  876, 878
#define PPUTLUINT_RANGE_876  875, 877
#define PPUTLUINT_RANGE_875  874, 876
#define PPUTLUINT_RANGE_874  873, 875
#define PPUTLUINT_RANGE_873  872, 874
#define PPUTLUINT_RANGE_872  871, 873
#define PPUTLUINT_RANGE_871  870, 872
#define PPUTLUINT_RANGE_870  869, 871
#define PPUTLUINT_RANGE_869  868, 870
#define PPUTLUINT_RANGE_868  867, 869
#define PPUTLUINT_RANGE_867  866, 868
#define PPUTLUINT_RANGE_866  865, 867
#define PPUTLUINT_RANGE_865  864, 866
#define PPUTLUINT_RANGE_864  863, 865
#define PPUTLUINT_RANGE_863  862, 864
#define PPUTLUINT_RANGE_862  861, 863
#define PPUTLUINT_RANGE_861  860, 862
#define PPUTLUINT_RANGE_860  859, 861
#define PPUTLUINT_RANGE_859  858, 860
#define PPUTLUINT_RANGE_858  857, 859
#define PPUTLUINT_RANGE_857  856, 858
#define PPUTLUINT_RANGE_856  855, 857
#define PPUTLUINT_RANGE_855  854, 856
#define PPUTLUINT_RANGE_854  853, 855
#define PPUTLUINT_RANGE_853  852, 854
#define PPUTLUINT_RANGE_852  851, 853
#define PPUTLUINT_RANGE_851  850, 852
#define PPUTLUINT_RANGE_850  849, 851
#define PPUTLUINT_RANGE_849  848, 850
#define PPUTLUINT_RANGE_848  847, 849
#define PPUTLUINT_RANGE_847  846, 848
#define PPUTLUINT_RANGE_846  845, 847
#define PPUTLUINT_RANGE_845  844, 846
#define PPUTLUINT_RANGE_844  843, 845
#define PPUTLUINT_RANGE_843  842, 844
#define PPUTLUINT_RANGE_842  841, 843
#define PPUTLUINT_RANGE_841  840, 842
#define PPUTLUINT_RANGE_840  839, 841
#define PPUTLUINT_RANGE_839  838, 840
#define PPUTLUINT_RANGE_838  837, 839
#define PPUTLUINT_RANGE_837  836, 838
#define PPUTLUINT_RANGE_836  835, 837
#define PPUTLUINT_RANGE_835  834, 836
#define PPUTLUINT_RANGE_834  833, 835
#define PPUTLUINT_RANGE_833  832, 834
#define PPUTLUINT_RANGE_832  831, 833
#define PPUTLUINT_RANGE_831  830, 832
#define PPUTLUINT_RANGE_830  829, 831
#define PPUTLUINT_RANGE_829  828, 830
#define PPUTLUINT_RANGE_828  827, 829
#define PPUTLUINT_RANGE_827  826, 828
#define PPUTLUINT_RANGE_826  825, 827
#define PPUTLUINT_RANGE_825  824, 826
#define PPUTLUINT_RANGE_824  823, 825
#define PPUTLUINT_RANGE_823  822, 824
#define PPUTLUINT_RANGE_822  821, 823
#define PPUTLUINT_RANGE_821  820, 822
#define PPUTLUINT_RANGE_820  819, 821
#define PPUTLUINT_RANGE_819  818, 820
#define PPUTLUINT_RANGE_818  817, 819
#define PPUTLUINT_RANGE_817  816, 818
#define PPUTLUINT_RANGE_816  815, 817
#define PPUTLUINT_RANGE_815  814, 816
#define PPUTLUINT_RANGE_814  813, 815
#define PPUTLUINT_RANGE_813  812, 814
#define PPUTLUINT_RANGE_812  811, 813
#define PPUTLUINT_RANGE_811  810, 812
#define PPUTLUINT_RANGE_810  809, 811
#define PPUTLUINT_RANGE_809  808, 810
#define PPUTLUINT_RANGE_808  807, 809
#define PPUTLUINT_RANGE_807  806, 808
#define PPUTLUINT_RANGE_806  805, 807
#define PPUTLUINT_RANGE_805  804, 806
#define PPUTLUINT_RANGE_804  803, 805
#define PPUTLUINT_RANGE_803  802, 804
#define PPUTLUINT_RANGE_802  801, 803
#define PPUTLUINT_RANGE_801  800, 802
#define PPUTLUINT_RANGE_800  799, 801
#define PPUTLUINT_RANGE_799  798, 800
#define PPUTLUINT_RANGE_798  797, 799
#define PPUTLUINT_RANGE_797  796, 798
#define PPUTLUINT_RANGE_796  795, 797
#define PPUTLUINT_RANGE_795  794, 796
#define PPUTLUINT_RANGE_794  793, 795
#define PPUTLUINT_RANGE_793  792, 794
#define PPUTLUINT_RANGE_792  791, 793
#define PPUTLUINT_RANGE_791  790, 792
#define PPUTLUINT_RANGE_790  789, 791
#define PPUTLUINT_RANGE_789  788, 790
#define PPUTLUINT_RANGE_788  787, 789
#define PPUTLUINT_RANGE_787  786, 788
#define PPUTLUINT_RANGE_786  785, 787
#define PPUTLUINT_RANGE_785  784, 786
#define PPUTLUINT_RANGE_784  783, 785
#define PPUTLUINT_RANGE_783  782, 784
#define PPUTLUINT_RANGE_782  781, 783
#define PPUTLUINT_RANGE_781  780, 782
#define PPUTLUINT_RANGE_780  779, 781
#define PPUTLUINT_RANGE_779  778, 780
#define PPUTLUINT_RANGE_778  777, 779
#define PPUTLUINT_RANGE_777  776, 778
#define PPUTLUINT_RANGE_776  775, 777
#define PPUTLUINT_RANGE_775  774, 776
#define PPUTLUINT_RANGE_774  773, 775
#define PPUTLUINT_RANGE_773  772, 774
#define PPUTLUINT_RANGE_772  771, 773
#define PPUTLUINT_RANGE_771  770, 772
#define PPUTLUINT_RANGE_770  769, 771
#define PPUTLUINT_RANGE_769  768, 770
#define PPUTLUINT_RANGE_768  767, 769
#define PPUTLUINT_RANGE_767  766, 768
#define PPUTLUINT_RANGE_766  765, 767
#define PPUTLUINT_RANGE_765  764, 766
#define PPUTLUINT_RANGE_764  763, 765
#define PPUTLUINT_RANGE_763  762, 764
#define PPUTLUINT_RANGE_762  761, 763
#define PPUTLUINT_RANGE_761  760, 762
#define PPUTLUINT_RANGE_760  759, 761
#define PPUTLUINT_RANGE_759  758, 760
#define PPUTLUINT_RANGE_758  757, 759
#define PPUTLUINT_RANGE_757  756, 758
#define PPUTLUINT_RANGE_756  755, 757
#define PPUTLUINT_RANGE_755  754, 756
#define PPUTLUINT_RANGE_754  753, 755
#define PPUTLUINT_RANGE_753  752, 754
#define PPUTLUINT_RANGE_752  751, 753
#define PPUTLUINT_RANGE_751  750, 752
#define PPUTLUINT_RANGE_750  749, 751
#define PPUTLUINT_RANGE_749  748, 750
#define PPUTLUINT_RANGE_748  747, 749
#define PPUTLUINT_RANGE_747  746, 748
#define PPUTLUINT_RANGE_746  745, 747
#define PPUTLUINT_RANGE_745  744, 746
#define PPUTLUINT_RANGE_744  743, 745
#define PPUTLUINT_RANGE_743  742, 744
#define PPUTLUINT_RANGE_742  741, 743
#define PPUTLUINT_RANGE_741  740, 742
#define PPUTLUINT_RANGE_740  739, 741
#define PPUTLUINT_RANGE_739  738, 740
#define PPUTLUINT_RANGE_738  737, 739
#define PPUTLUINT_RANGE_737  736, 738
#define PPUTLUINT_RANGE_736  735, 737
#define PPUTLUINT_RANGE_735  734, 736
#define PPUTLUINT_RANGE_734  733, 735
#define PPUTLUINT_RANGE_733  732, 734
#define PPUTLUINT_RANGE_732  731, 733
#define PPUTLUINT_RANGE_731  730, 732
#define PPUTLUINT_RANGE_730  729, 731
#define PPUTLUINT_RANGE_729  728, 730
#define PPUTLUINT_RANGE_728  727, 729
#define PPUTLUINT_RANGE_727  726, 728
#define PPUTLUINT_RANGE_726  725, 727
#define PPUTLUINT_RANGE_725  724, 726
#define PPUTLUINT_RANGE_724  723, 725
#define PPUTLUINT_RANGE_723  722, 724
#define PPUTLUINT_RANGE_722  721, 723
#define PPUTLUINT_RANGE_721  720, 722
#define PPUTLUINT_RANGE_720  719, 721
#define PPUTLUINT_RANGE_719  718, 720
#define PPUTLUINT_RANGE_718  717, 719
#define PPUTLUINT_RANGE_717  716, 718
#define PPUTLUINT_RANGE_716  715, 717
#define PPUTLUINT_RANGE_715  714, 716
#define PPUTLUINT_RANGE_714  713, 715
#define PPUTLUINT_RANGE_713  712, 714
#define PPUTLUINT_RANGE_712  711, 713
#define PPUTLUINT_RANGE_711  710, 712
#define PPUTLUINT_RANGE_710  709, 711
#define PPUTLUINT_RANGE_709  708, 710
#define PPUTLUINT_RANGE_708  707, 709
#define PPUTLUINT_RANGE_707  706, 708
#define PPUTLUINT_RANGE_706  705, 707
#define PPUTLUINT_RANGE_705  704, 706
#define PPUTLUINT_RANGE_704  703, 705
#define PPUTLUINT_RANGE_703  702, 704
#define PPUTLUINT_RANGE_702  701, 703
#define PPUTLUINT_RANGE_701  700, 702
#define PPUTLUINT_RANGE_700  699, 701
#define PPUTLUINT_RANGE_699  698, 700
#define PPUTLUINT_RANGE_698  697, 699
#define PPUTLUINT_RANGE_697  696, 698
#define PPUTLUINT_RANGE_696  695, 697
#define PPUTLUINT_RANGE_695  694, 696
#define PPUTLUINT_RANGE_694  693, 695
#define PPUTLUINT_RANGE_693  692, 694
#define PPUTLUINT_RANGE_692  691, 693
#define PPUTLUINT_RANGE_691  690, 692
#define PPUTLUINT_RANGE_690  689, 691
#define PPUTLUINT_RANGE_689  688, 690
#define PPUTLUINT_RANGE_688  687, 689
#define PPUTLUINT_RANGE_687  686, 688
#define PPUTLUINT_RANGE_686  685, 687
#define PPUTLUINT_RANGE_685  684, 686
#define PPUTLUINT_RANGE_684  683, 685
#define PPUTLUINT_RANGE_683  682, 684
#define PPUTLUINT_RANGE_682  681, 683
#define PPUTLUINT_RANGE_681  680, 682
#define PPUTLUINT_RANGE_680  679, 681
#define PPUTLUINT_RANGE_679  678, 680
#define PPUTLUINT_RANGE_678  677, 679
#define PPUTLUINT_RANGE_677  676, 678
#define PPUTLUINT_RANGE_676  675, 677
#define PPUTLUINT_RANGE_675  674, 676
#define PPUTLUINT_RANGE_674  673, 675
#define PPUTLUINT_RANGE_673  672, 674
#define PPUTLUINT_RANGE_672  671, 673
#define PPUTLUINT_RANGE_671  670, 672
#define PPUTLUINT_RANGE_670  669, 671
#define PPUTLUINT_RANGE_669  668, 670
#define PPUTLUINT_RANGE_668  667, 669
#define PPUTLUINT_RANGE_667  666, 668
#define PPUTLUINT_RANGE_666  665, 667
#define PPUTLUINT_RANGE_665  664, 666
#define PPUTLUINT_RANGE_664  663, 665
#define PPUTLUINT_RANGE_663  662, 664
#define PPUTLUINT_RANGE_662  661, 663
#define PPUTLUINT_RANGE_661  660, 662
#define PPUTLUINT_RANGE_660  659, 661
#define PPUTLUINT_RANGE_659  658, 660
#define PPUTLUINT_RANGE_658  657, 659
#define PPUTLUINT_RANGE_657  656, 658
#define PPUTLUINT_RANGE_656  655, 657
#define PPUTLUINT_RANGE_655  654, 656
#define PPUTLUINT_RANGE_654  653, 655
#define PPUTLUINT_RANGE_653  652, 654
#define PPUTLUINT_RANGE_652  651, 653
#define PPUTLUINT_RANGE_651  650, 652
#define PPUTLUINT_RANGE_650  649, 651
#define PPUTLUINT_RANGE_649  648, 650
#define PPUTLUINT_RANGE_648  647, 649
#define PPUTLUINT_RANGE_647  646, 648
#define PPUTLUINT_RANGE_646  645, 647
#define PPUTLUINT_RANGE_645  644, 646
#define PPUTLUINT_RANGE_644  643, 645
#define PPUTLUINT_RANGE_643  642, 644
#define PPUTLUINT_RANGE_642  641, 643
#define PPUTLUINT_RANGE_641  640, 642
#define PPUTLUINT_RANGE_640  639, 641
#define PPUTLUINT_RANGE_639  638, 640
#define PPUTLUINT_RANGE_638  637, 639
#define PPUTLUINT_RANGE_637  636, 638
#define PPUTLUINT_RANGE_636  635, 637
#define PPUTLUINT_RANGE_635  634, 636
#define PPUTLUINT_RANGE_634  633, 635
#define PPUTLUINT_RANGE_633  632, 634
#define PPUTLUINT_RANGE_632  631, 633
#define PPUTLUINT_RANGE_631  630, 632
#define PPUTLUINT_RANGE_630  629, 631
#define PPUTLUINT_RANGE_629  628, 630
#define PPUTLUINT_RANGE_628  627, 629
#define PPUTLUINT_RANGE_627  626, 628
#define PPUTLUINT_RANGE_626  625, 627
#define PPUTLUINT_RANGE_625  624, 626
#define PPUTLUINT_RANGE_624  623, 625
#define PPUTLUINT_RANGE_623  622, 624
#define PPUTLUINT_RANGE_622  621, 623
#define PPUTLUINT_RANGE_621  620, 622
#define PPUTLUINT_RANGE_620  619, 621
#define PPUTLUINT_RANGE_619  618, 620
#define PPUTLUINT_RANGE_618  617, 619
#define PPUTLUINT_RANGE_617  616, 618
#define PPUTLUINT_RANGE_616  615, 617
#define PPUTLUINT_RANGE_615  614, 616
#define PPUTLUINT_RANGE_614  613, 615
#define PPUTLUINT_RANGE_613  612, 614
#define PPUTLUINT_RANGE_612  611, 613
#define PPUTLUINT_RANGE_611  610, 612
#define PPUTLUINT_RANGE_610  609, 611
#define PPUTLUINT_RANGE_609  608, 610
#define PPUTLUINT_RANGE_608  607, 609
#define PPUTLUINT_RANGE_607  606, 608
#define PPUTLUINT_RANGE_606  605, 607
#define PPUTLUINT_RANGE_605  604, 606
#define PPUTLUINT_RANGE_604  603, 605
#define PPUTLUINT_RANGE_603  602, 604
#define PPUTLUINT_RANGE_602  601, 603
#define PPUTLUINT_RANGE_601  600, 602
#define PPUTLUINT_RANGE_600  599, 601
#define PPUTLUINT_RANGE_599  598, 600
#define PPUTLUINT_RANGE_598  597, 599
#define PPUTLUINT_RANGE_597  596, 598
#define PPUTLUINT_RANGE_596  595, 597
#define PPUTLUINT_RANGE_595  594, 596
#define PPUTLUINT_RANGE_594  593, 595
#define PPUTLUINT_RANGE_593  592, 594
#define PPUTLUINT_RANGE_592  591, 593
#define PPUTLUINT_RANGE_591  590, 592
#define PPUTLUINT_RANGE_590  589, 591
#define PPUTLUINT_RANGE_589  588, 590
#define PPUTLUINT_RANGE_588  587, 589
#define PPUTLUINT_RANGE_587  586, 588
#define PPUTLUINT_RANGE_586  585, 587
#define PPUTLUINT_RANGE_585  584, 586
#define PPUTLUINT_RANGE_584  583, 585
#define PPUTLUINT_RANGE_583  582, 584
#define PPUTLUINT_RANGE_582  581, 583
#define PPUTLUINT_RANGE_581  580, 582
#define PPUTLUINT_RANGE_580  579, 581
#define PPUTLUINT_RANGE_579  578, 580
#define PPUTLUINT_RANGE_578  577, 579
#define PPUTLUINT_RANGE_577  576, 578
#define PPUTLUINT_RANGE_576  575, 577
#define PPUTLUINT_RANGE_575  574, 576
#define PPUTLUINT_RANGE_574  573, 575
#define PPUTLUINT_RANGE_573  572, 574
#define PPUTLUINT_RANGE_572  571, 573
#define PPUTLUINT_RANGE_571  570, 572
#define PPUTLUINT_RANGE_570  569, 571
#define PPUTLUINT_RANGE_569  568, 570
#define PPUTLUINT_RANGE_568  567, 569
#define PPUTLUINT_RANGE_567  566, 568
#define PPUTLUINT_RANGE_566  565, 567
#define PPUTLUINT_RANGE_565  564, 566
#define PPUTLUINT_RANGE_564  563, 565
#define PPUTLUINT_RANGE_563  562, 564
#define PPUTLUINT_RANGE_562  561, 563
#define PPUTLUINT_RANGE_561  560, 562
#define PPUTLUINT_RANGE_560  559, 561
#define PPUTLUINT_RANGE_559  558, 560
#define PPUTLUINT_RANGE_558  557, 559
#define PPUTLUINT_RANGE_557  556, 558
#define PPUTLUINT_RANGE_556  555, 557
#define PPUTLUINT_RANGE_555  554, 556
#define PPUTLUINT_RANGE_554  553, 555
#define PPUTLUINT_RANGE_553  552, 554
#define PPUTLUINT_RANGE_552  551, 553
#define PPUTLUINT_RANGE_551  550, 552
#define PPUTLUINT_RANGE_550  549, 551
#define PPUTLUINT_RANGE_549  548, 550
#define PPUTLUINT_RANGE_548  547, 549
#define PPUTLUINT_RANGE_547  546, 548
#define PPUTLUINT_RANGE_546  545, 547
#define PPUTLUINT_RANGE_545  544, 546
#define PPUTLUINT_RANGE_544  543, 545
#define PPUTLUINT_RANGE_543  542, 544
#define PPUTLUINT_RANGE_542  541, 543
#define PPUTLUINT_RANGE_541  540, 542
#define PPUTLUINT_RANGE_540  539, 541
#define PPUTLUINT_RANGE_539  538, 540
#define PPUTLUINT_RANGE_538  537, 539
#define PPUTLUINT_RANGE_537  536, 538
#define PPUTLUINT_RANGE_536  535, 537
#define PPUTLUINT_RANGE_535  534, 536
#define PPUTLUINT_RANGE_534  533, 535
#define PPUTLUINT_RANGE_533  532, 534
#define PPUTLUINT_RANGE_532  531, 533
#define PPUTLUINT_RANGE_531  530, 532
#define PPUTLUINT_RANGE_530  529, 531
#define PPUTLUINT_RANGE_529  528, 530
#define PPUTLUINT_RANGE_528  527, 529
#define PPUTLUINT_RANGE_527  526, 528
#define PPUTLUINT_RANGE_526  525, 527
#define PPUTLUINT_RANGE_525  524, 526
#define PPUTLUINT_RANGE_524  523, 525
#define PPUTLUINT_RANGE_523  522, 524
#define PPUTLUINT_RANGE_522  521, 523
#define PPUTLUINT_RANGE_521  520, 522
#define PPUTLUINT_RANGE_520  519, 521
#define PPUTLUINT_RANGE_519  518, 520
#define PPUTLUINT_RANGE_518  517, 519
#define PPUTLUINT_RANGE_517  516, 518
#define PPUTLUINT_RANGE_516  515, 517
#define PPUTLUINT_RANGE_515  514, 516
#define PPUTLUINT_RANGE_514  513, 515
#define PPUTLUINT_RANGE_513  512, 514
#define PPUTLUINT_RANGE_512  511, 513
#define PPUTLUINT_RANGE_511  510, 512
#define PPUTLUINT_RANGE_510  509, 511
#define PPUTLUINT_RANGE_509  508, 510
#define PPUTLUINT_RANGE_508  507, 509
#define PPUTLUINT_RANGE_507  506, 508
#define PPUTLUINT_RANGE_506  505, 507
#define PPUTLUINT_RANGE_505  504, 506
#define PPUTLUINT_RANGE_504  503, 505
#define PPUTLUINT_RANGE_503  502, 504
#define PPUTLUINT_RANGE_502  501, 503
#define PPUTLUINT_RANGE_501  500, 502
#define PPUTLUINT_RANGE_500  499, 501
#define PPUTLUINT_RANGE_499  498, 500
#define PPUTLUINT_RANGE_498  497, 499
#define PPUTLUINT_RANGE_497  496, 498
#define PPUTLUINT_RANGE_496  495, 497
#define PPUTLUINT_RANGE_495  494, 496
#define PPUTLUINT_RANGE_494  493, 495
#define PPUTLUINT_RANGE_493  492, 494
#define PPUTLUINT_RANGE_492  491, 493
#define PPUTLUINT_RANGE_491  490, 492
#define PPUTLUINT_RANGE_490  489, 491
#define PPUTLUINT_RANGE_489  488, 490
#define PPUTLUINT_RANGE_488  487, 489
#define PPUTLUINT_RANGE_487  486, 488
#define PPUTLUINT_RANGE_486  485, 487
#define PPUTLUINT_RANGE_485  484, 486
#define PPUTLUINT_RANGE_484  483, 485
#define PPUTLUINT_RANGE_483  482, 484
#define PPUTLUINT_RANGE_482  481, 483
#define PPUTLUINT_RANGE_481  480, 482
#define PPUTLUINT_RANGE_480  479, 481
#define PPUTLUINT_RANGE_479  478, 480
#define PPUTLUINT_RANGE_478  477, 479
#define PPUTLUINT_RANGE_477  476, 478
#define PPUTLUINT_RANGE_476  475, 477
#define PPUTLUINT_RANGE_475  474, 476
#define PPUTLUINT_RANGE_474  473, 475
#define PPUTLUINT_RANGE_473  472, 474
#define PPUTLUINT_RANGE_472  471, 473
#define PPUTLUINT_RANGE_471  470, 472
#define PPUTLUINT_RANGE_470  469, 471
#define PPUTLUINT_RANGE_469  468, 470
#define PPUTLUINT_RANGE_468  467, 469
#define PPUTLUINT_RANGE_467  466, 468
#define PPUTLUINT_RANGE_466  465, 467
#define PPUTLUINT_RANGE_465  464, 466
#define PPUTLUINT_RANGE_464  463, 465
#define PPUTLUINT_RANGE_463  462, 464
#define PPUTLUINT_RANGE_462  461, 463
#define PPUTLUINT_RANGE_461  460, 462
#define PPUTLUINT_RANGE_460  459, 461
#define PPUTLUINT_RANGE_459  458, 460
#define PPUTLUINT_RANGE_458  457, 459
#define PPUTLUINT_RANGE_457  456, 458
#define PPUTLUINT_RANGE_456  455, 457
#define PPUTLUINT_RANGE_455  454, 456
#define PPUTLUINT_RANGE_454  453, 455
#define PPUTLUINT_RANGE_453  452, 454
#define PPUTLUINT_RANGE_452  451, 453
#define PPUTLUINT_RANGE_451  450, 452
#define PPUTLUINT_RANGE_450  449, 451
#define PPUTLUINT_RANGE_449  448, 450
#define PPUTLUINT_RANGE_448  447, 449
#define PPUTLUINT_RANGE_447  446, 448
#define PPUTLUINT_RANGE_446  445, 447
#define PPUTLUINT_RANGE_445  444, 446
#define PPUTLUINT_RANGE_444  443, 445
#define PPUTLUINT_RANGE_443  442, 444
#define PPUTLUINT_RANGE_442  441, 443
#define PPUTLUINT_RANGE_441  440, 442
#define PPUTLUINT_RANGE_440  439, 441
#define PPUTLUINT_RANGE_439  438, 440
#define PPUTLUINT_RANGE_438  437, 439
#define PPUTLUINT_RANGE_437  436, 438
#define PPUTLUINT_RANGE_436  435, 437
#define PPUTLUINT_RANGE_435  434, 436
#define PPUTLUINT_RANGE_434  433, 435
#define PPUTLUINT_RANGE_433  432, 434
#define PPUTLUINT_RANGE_432  431, 433
#define PPUTLUINT_RANGE_431  430, 432
#define PPUTLUINT_RANGE_430  429, 431
#define PPUTLUINT_RANGE_429  428, 430
#define PPUTLUINT_RANGE_428  427, 429
#define PPUTLUINT_RANGE_427  426, 428
#define PPUTLUINT_RANGE_426  425, 427
#define PPUTLUINT_RANGE_425  424, 426
#define PPUTLUINT_RANGE_424  423, 425
#define PPUTLUINT_RANGE_423  422, 424
#define PPUTLUINT_RANGE_422  421, 423
#define PPUTLUINT_RANGE_421  420, 422
#define PPUTLUINT_RANGE_420  419, 421
#define PPUTLUINT_RANGE_419  418, 420
#define PPUTLUINT_RANGE_418  417, 419
#define PPUTLUINT_RANGE_417  416, 418
#define PPUTLUINT_RANGE_416  415, 417
#define PPUTLUINT_RANGE_415  414, 416
#define PPUTLUINT_RANGE_414  413, 415
#define PPUTLUINT_RANGE_413  412, 414
#define PPUTLUINT_RANGE_412  411, 413
#define PPUTLUINT_RANGE_411  410, 412
#define PPUTLUINT_RANGE_410  409, 411
#define PPUTLUINT_RANGE_409  408, 410
#define PPUTLUINT_RANGE_408  407, 409
#define PPUTLUINT_RANGE_407  406, 408
#define PPUTLUINT_RANGE_406  405, 407
#define PPUTLUINT_RANGE_405  404, 406
#define PPUTLUINT_RANGE_404  403, 405
#define PPUTLUINT_RANGE_403  402, 404
#define PPUTLUINT_RANGE_402  401, 403
#define PPUTLUINT_RANGE_401  400, 402
#define PPUTLUINT_RANGE_400  399, 401
#define PPUTLUINT_RANGE_399  398, 400
#define PPUTLUINT_RANGE_398  397, 399
#define PPUTLUINT_RANGE_397  396, 398
#define PPUTLUINT_RANGE_396  395, 397
#define PPUTLUINT_RANGE_395  394, 396
#define PPUTLUINT_RANGE_394  393, 395
#define PPUTLUINT_RANGE_393  392, 394
#define PPUTLUINT_RANGE_392  391, 393
#define PPUTLUINT_RANGE_391  390, 392
#define PPUTLUINT_RANGE_390  389, 391
#define PPUTLUINT_RANGE_389  388, 390
#define PPUTLUINT_RANGE_388  387, 389
#define PPUTLUINT_RANGE_387  386, 388
#define PPUTLUINT_RANGE_386  385, 387
#define PPUTLUINT_RANGE_385  384, 386
#define PPUTLUINT_RANGE_384  383, 385
#define PPUTLUINT_RANGE_383  382, 384
#define PPUTLUINT_RANGE_382  381, 383
#define PPUTLUINT_RANGE_381  380, 382
#define PPUTLUINT_RANGE_380  379, 381
#define PPUTLUINT_RANGE_379  378, 380
#define PPUTLUINT_RANGE_378  377, 379
#define PPUTLUINT_RANGE_377  376, 378
#define PPUTLUINT_RANGE_376  375, 377
#define PPUTLUINT_RANGE_375  374, 376
#define PPUTLUINT_RANGE_374  373, 375
#define PPUTLUINT_RANGE_373  372, 374
#define PPUTLUINT_RANGE_372  371, 373
#define PPUTLUINT_RANGE_371  370, 372
#define PPUTLUINT_RANGE_370  369, 371
#define PPUTLUINT_RANGE_369  368, 370
#define PPUTLUINT_RANGE_368  367, 369
#define PPUTLUINT_RANGE_367  366, 368
#define PPUTLUINT_RANGE_366  365, 367
#define PPUTLUINT_RANGE_365  364, 366
#define PPUTLUINT_RANGE_364  363, 365
#define PPUTLUINT_RANGE_363  362, 364
#define PPUTLUINT_RANGE_362  361, 363
#define PPUTLUINT_RANGE_361  360, 362
#define PPUTLUINT_RANGE_360  359, 361
#define PPUTLUINT_RANGE_359  358, 360
#define PPUTLUINT_RANGE_358  357, 359
#define PPUTLUINT_RANGE_357  356, 358
#define PPUTLUINT_RANGE_356  355, 357
#define PPUTLUINT_RANGE_355  354, 356
#define PPUTLUINT_RANGE_354  353, 355
#define PPUTLUINT_RANGE_353  352, 354
#define PPUTLUINT_RANGE_352  351, 353
#define PPUTLUINT_RANGE_351  350, 352
#define PPUTLUINT_RANGE_350  349, 351
#define PPUTLUINT_RANGE_349  348, 350
#define PPUTLUINT_RANGE_348  347, 349
#define PPUTLUINT_RANGE_347  346, 348
#define PPUTLUINT_RANGE_346  345, 347
#define PPUTLUINT_RANGE_345  344, 346
#define PPUTLUINT_RANGE_344  343, 345
#define PPUTLUINT_RANGE_343  342, 344
#define PPUTLUINT_RANGE_342  341, 343
#define PPUTLUINT_RANGE_341  340, 342
#define PPUTLUINT_RANGE_340  339, 341
#define PPUTLUINT_RANGE_339  338, 340
#define PPUTLUINT_RANGE_338  337, 339
#define PPUTLUINT_RANGE_337  336, 338
#define PPUTLUINT_RANGE_336  335, 337
#define PPUTLUINT_RANGE_335  334, 336
#define PPUTLUINT_RANGE_334  333, 335
#define PPUTLUINT_RANGE_333  332, 334
#define PPUTLUINT_RANGE_332  331, 333
#define PPUTLUINT_RANGE_331  330, 332
#define PPUTLUINT_RANGE_330  329, 331
#define PPUTLUINT_RANGE_329  328, 330
#define PPUTLUINT_RANGE_328  327, 329
#define PPUTLUINT_RANGE_327  326, 328
#define PPUTLUINT_RANGE_326  325, 327
#define PPUTLUINT_RANGE_325  324, 326
#define PPUTLUINT_RANGE_324  323, 325
#define PPUTLUINT_RANGE_323  322, 324
#define PPUTLUINT_RANGE_322  321, 323
#define PPUTLUINT_RANGE_321  320, 322
#define PPUTLUINT_RANGE_320  319, 321
#define PPUTLUINT_RANGE_319  318, 320
#define PPUTLUINT_RANGE_318  317, 319
#define PPUTLUINT_RANGE_317  316, 318
#define PPUTLUINT_RANGE_316  315, 317
#define PPUTLUINT_RANGE_315  314, 316
#define PPUTLUINT_RANGE_314  313, 315
#define PPUTLUINT_RANGE_313  312, 314
#define PPUTLUINT_RANGE_312  311, 313
#define PPUTLUINT_RANGE_311  310, 312
#define PPUTLUINT_RANGE_310  309, 311
#define PPUTLUINT_RANGE_309  308, 310
#define PPUTLUINT_RANGE_308  307, 309
#define PPUTLUINT_RANGE_307  306, 308
#define PPUTLUINT_RANGE_306  305, 307
#define PPUTLUINT_RANGE_305  304, 306
#define PPUTLUINT_RANGE_304  303, 305
#define PPUTLUINT_RANGE_303  302, 304
#define PPUTLUINT_RANGE_302  301, 303
#define PPUTLUINT_RANGE_301  300, 302
#define PPUTLUINT_RANGE_300  299, 301
#define PPUTLUINT_RANGE_299  298, 300
#define PPUTLUINT_RANGE_298  297, 299
#define PPUTLUINT_RANGE_297  296, 298
#define PPUTLUINT_RANGE_296  295, 297
#define PPUTLUINT_RANGE_295  294, 296
#define PPUTLUINT_RANGE_294  293, 295
#define PPUTLUINT_RANGE_293  292, 294
#define PPUTLUINT_RANGE_292  291, 293
#define PPUTLUINT_RANGE_291  290, 292
#define PPUTLUINT_RANGE_290  289, 291
#define PPUTLUINT_RANGE_289  288, 290
#define PPUTLUINT_RANGE_288  287, 289
#define PPUTLUINT_RANGE_287  286, 288
#define PPUTLUINT_RANGE_286  285, 287
#define PPUTLUINT_RANGE_285  284, 286
#define PPUTLUINT_RANGE_284  283, 285
#define PPUTLUINT_RANGE_283  282, 284
#define PPUTLUINT_RANGE_282  281, 283
#define PPUTLUINT_RANGE_281  280, 282
#define PPUTLUINT_RANGE_280  279, 281
#define PPUTLUINT_RANGE_279  278, 280
#define PPUTLUINT_RANGE_278  277, 279
#define PPUTLUINT_RANGE_277  276, 278
#define PPUTLUINT_RANGE_276  275, 277
#define PPUTLUINT_RANGE_275  274, 276
#define PPUTLUINT_RANGE_274  273, 275
#define PPUTLUINT_RANGE_273  272, 274
#define PPUTLUINT_RANGE_272  271, 273
#define PPUTLUINT_RANGE_271  270, 272
#define PPUTLUINT_RANGE_270  269, 271
#define PPUTLUINT_RANGE_269  268, 270
#define PPUTLUINT_RANGE_268  267, 269
#define PPUTLUINT_RANGE_267  266, 268
#define PPUTLUINT_RANGE_266  265, 267
#define PPUTLUINT_RANGE_265  264, 266
#define PPUTLUINT_RANGE_264  263, 265
#define PPUTLUINT_RANGE_263  262, 264
#define PPUTLUINT_RANGE_262  261, 263
#define PPUTLUINT_RANGE_261  260, 262
#define PPUTLUINT_RANGE_260  259, 261
#define PPUTLUINT_RANGE_259  258, 260
#define PPUTLUINT_RANGE_258  257, 259
#define PPUTLUINT_RANGE_257  256, 258
#define PPUTLUINT_RANGE_256  255, 257
#define PPUTLUINT_RANGE_255  254, 256
#define PPUTLUINT_RANGE_254  253, 255
#define PPUTLUINT_RANGE_253  252, 254
#define PPUTLUINT_RANGE_252  251, 253
#define PPUTLUINT_RANGE_251  250, 252
#define PPUTLUINT_RANGE_250  249, 251
#define PPUTLUINT_RANGE_249  248, 250
#define PPUTLUINT_RANGE_248  247, 249
#define PPUTLUINT_RANGE_247  246, 248
#define PPUTLUINT_RANGE_246  245, 247
#define PPUTLUINT_RANGE_245  244, 246
#define PPUTLUINT_RANGE_244  243, 245
#define PPUTLUINT_RANGE_243  242, 244
#define PPUTLUINT_RANGE_242  241, 243
#define PPUTLUINT_RANGE_241  240, 242
#define PPUTLUINT_RANGE_240  239, 241
#define PPUTLUINT_RANGE_239  238, 240
#define PPUTLUINT_RANGE_238  237, 239
#define PPUTLUINT_RANGE_237  236, 238
#define PPUTLUINT_RANGE_236  235, 237
#define PPUTLUINT_RANGE_235  234, 236
#define PPUTLUINT_RANGE_234  233, 235
#define PPUTLUINT_RANGE_233  232, 234
#define PPUTLUINT_RANGE_232  231, 233
#define PPUTLUINT_RANGE_231  230, 232
#define PPUTLUINT_RANGE_230  229, 231
#define PPUTLUINT_RANGE_229  228, 230
#define PPUTLUINT_RANGE_228  227, 229
#define PPUTLUINT_RANGE_227  226, 228
#define PPUTLUINT_RANGE_226  225, 227
#define PPUTLUINT_RANGE_225  224, 226
#define PPUTLUINT_RANGE_224  223, 225
#define PPUTLUINT_RANGE_223  222, 224
#define PPUTLUINT_RANGE_222  221, 223
#define PPUTLUINT_RANGE_221  220, 222
#define PPUTLUINT_RANGE_220  219, 221
#define PPUTLUINT_RANGE_219  218, 220
#define PPUTLUINT_RANGE_218  217, 219
#define PPUTLUINT_RANGE_217  216, 218
#define PPUTLUINT_RANGE_216  215, 217
#define PPUTLUINT_RANGE_215  214, 216
#define PPUTLUINT_RANGE_214  213, 215
#define PPUTLUINT_RANGE_213  212, 214
#define PPUTLUINT_RANGE_212  211, 213
#define PPUTLUINT_RANGE_211  210, 212
#define PPUTLUINT_RANGE_210  209, 211
#define PPUTLUINT_RANGE_209  208, 210
#define PPUTLUINT_RANGE_208  207, 209
#define PPUTLUINT_RANGE_207  206, 208
#define PPUTLUINT_RANGE_206  205, 207
#define PPUTLUINT_RANGE_205  204, 206
#define PPUTLUINT_RANGE_204  203, 205
#define PPUTLUINT_RANGE_203  202, 204
#define PPUTLUINT_RANGE_202  201, 203
#define PPUTLUINT_RANGE_201  200, 202
#define PPUTLUINT_RANGE_200  199, 201
#define PPUTLUINT_RANGE_199  198, 200
#define PPUTLUINT_RANGE_198  197, 199
#define PPUTLUINT_RANGE_197  196, 198
#define PPUTLUINT_RANGE_196  195, 197
#define PPUTLUINT_RANGE_195  194, 196
#define PPUTLUINT_RANGE_194  193, 195
#define PPUTLUINT_RANGE_193  192, 194
#define PPUTLUINT_RANGE_192  191, 193
#define PPUTLUINT_RANGE_191  190, 192
#define PPUTLUINT_RANGE_190  189, 191
#define PPUTLUINT_RANGE_189  188, 190
#define PPUTLUINT_RANGE_188  187, 189
#define PPUTLUINT_RANGE_187  186, 188
#define PPUTLUINT_RANGE_186  185, 187
#define PPUTLUINT_RANGE_185  184, 186
#define PPUTLUINT_RANGE_184  183, 185
#define PPUTLUINT_RANGE_183  182, 184
#define PPUTLUINT_RANGE_182  181, 183
#define PPUTLUINT_RANGE_181  180, 182
#define PPUTLUINT_RANGE_180  179, 181
#define PPUTLUINT_RANGE_179  178, 180
#define PPUTLUINT_RANGE_178  177, 179
#define PPUTLUINT_RANGE_177  176, 178
#define PPUTLUINT_RANGE_176  175, 177
#define PPUTLUINT_RANGE_175  174, 176
#define PPUTLUINT_RANGE_174  173, 175
#define PPUTLUINT_RANGE_173  172, 174
#define PPUTLUINT_RANGE_172  171, 173
#define PPUTLUINT_RANGE_171  170, 172
#define PPUTLUINT_RANGE_170  169, 171
#define PPUTLUINT_RANGE_169  168, 170
#define PPUTLUINT_RANGE_168  167, 169
#define PPUTLUINT_RANGE_167  166, 168
#define PPUTLUINT_RANGE_166  165, 167
#define PPUTLUINT_RANGE_165  164, 166
#define PPUTLUINT_RANGE_164  163, 165
#define PPUTLUINT_RANGE_163  162, 164
#define PPUTLUINT_RANGE_162  161, 163
#define PPUTLUINT_RANGE_161  160, 162
#define PPUTLUINT_RANGE_160  159, 161
#define PPUTLUINT_RANGE_159  158, 160
#define PPUTLUINT_RANGE_158  157, 159
#define PPUTLUINT_RANGE_157  156, 158
#define PPUTLUINT_RANGE_156  155, 157
#define PPUTLUINT_RANGE_155  154, 156
#define PPUTLUINT_RANGE_154  153, 155
#define PPUTLUINT_RANGE_153  152, 154
#define PPUTLUINT_RANGE_152  151, 153
#define PPUTLUINT_RANGE_151  150, 152
#define PPUTLUINT_RANGE_150  149, 151
#define PPUTLUINT_RANGE_149  148, 150
#define PPUTLUINT_RANGE_148  147, 149
#define PPUTLUINT_RANGE_147  146, 148
#define PPUTLUINT_RANGE_146  145, 147
#define PPUTLUINT_RANGE_145  144, 146
#define PPUTLUINT_RANGE_144  143, 145
#define PPUTLUINT_RANGE_143  142, 144
#define PPUTLUINT_RANGE_142  141, 143
#define PPUTLUINT_RANGE_141  140, 142
#define PPUTLUINT_RANGE_140  139, 141
#define PPUTLUINT_RANGE_139  138, 140
#define PPUTLUINT_RANGE_138  137, 139
#define PPUTLUINT_RANGE_137  136, 138
#define PPUTLUINT_RANGE_136  135, 137
#define PPUTLUINT_RANGE_135  134, 136
#define PPUTLUINT_RANGE_134  133, 135
#define PPUTLUINT_RANGE_133  132, 134
#define PPUTLUINT_RANGE_132  131, 133
#define PPUTLUINT_RANGE_131  130, 132
#define PPUTLUINT_RANGE_130  129, 131
#define PPUTLUINT_RANGE_129  128, 130
#define PPUTLUINT_RANGE_128  127, 129
#define PPUTLUINT_RANGE_127  126, 128
#define PPUTLUINT_RANGE_126  125, 127
#define PPUTLUINT_RANGE_125  124, 126
#define PPUTLUINT_RANGE_124  123, 125
#define PPUTLUINT_RANGE_123  122, 124
#define PPUTLUINT_RANGE_122  121, 123
#define PPUTLUINT_RANGE_121  120, 122
#define PPUTLUINT_RANGE_120  119, 121
#define PPUTLUINT_RANGE_119  118, 120
#define PPUTLUINT_RANGE_118  117, 119
#define PPUTLUINT_RANGE_117  116, 118
#define PPUTLUINT_RANGE_116  115, 117
#define PPUTLUINT_RANGE_115  114, 116
#define PPUTLUINT_RANGE_114  113, 115
#define PPUTLUINT_RANGE_113  112, 114
#define PPUTLUINT_RANGE_112  111, 113
#define PPUTLUINT_RANGE_111  110, 112
#define PPUTLUINT_RANGE_110  109, 111
#define PPUTLUINT_RANGE_109  108, 110
#define PPUTLUINT_RANGE_108  107, 109
#define PPUTLUINT_RANGE_107  106, 108
#define PPUTLUINT_RANGE_106  105, 107
#define PPUTLUINT_RANGE_105  104, 106
#define PPUTLUINT_RANGE_104  103, 105
#define PPUTLUINT_RANGE_103  102, 104
#define PPUTLUINT_RANGE_102  101, 103
#define PPUTLUINT_RANGE_101  100, 102
#define PPUTLUINT_RANGE_100  99, 101
#define PPUTLUINT_RANGE_99   98, 100
#define PPUTLUINT_RANGE_98   97, 99
#define PPUTLUINT_RANGE_97   96, 98
#define PPUTLUINT_RANGE_96   95, 97
#define PPUTLUINT_RANGE_95   94, 96
#define PPUTLUINT_RANGE_94   93, 95
#define PPUTLUINT_RANGE_93   92, 94
#define PPUTLUINT_RANGE_92   91, 93
#define PPUTLUINT_RANGE_91   90, 92
#define PPUTLUINT_RANGE_90   89, 91
#define PPUTLUINT_RANGE_89   88, 90
#define PPUTLUINT_RANGE_88   87, 89
#define PPUTLUINT_RANGE_87   86, 88
#define PPUTLUINT_RANGE_86   85, 87
#define PPUTLUINT_RANGE_85   84, 86
#define PPUTLUINT_RANGE_84   83, 85
#define PPUTLUINT_RANGE_83   82, 84
#define PPUTLUINT_RANGE_82   81, 83
#define PPUTLUINT_RANGE_81   80, 82
#define PPUTLUINT_RANGE_80   79, 81
#define PPUTLUINT_RANGE_79   78, 80
#define PPUTLUINT_RANGE_78   77, 79
#define PPUTLUINT_RANGE_77   76, 78
#define PPUTLUINT_RANGE_76   75, 77
#define PPUTLUINT_RANGE_75   74, 76
#define PPUTLUINT_RANGE_74   73, 75
#define PPUTLUINT_RANGE_73   72, 74
#define PPUTLUINT_RANGE_72   71, 73
#define PPUTLUINT_RANGE_71   70, 72
#define PPUTLUINT_RANGE_70   69, 71
#define PPUTLUINT_RANGE_69   68, 70
#define PPUTLUINT_RANGE_68   67, 69
#define PPUTLUINT_RANGE_67   66, 68
#define PPUTLUINT_RANGE_66   65, 67
#define PPUTLUINT_RANGE_65   64, 66
#define PPUTLUINT_RANGE_64   63, 65
#define PPUTLUINT_RANGE_63   62, 64
#define PPUTLUINT_RANGE_62   61, 63
#define PPUTLUINT_RANGE_61   60, 62
#define PPUTLUINT_RANGE_60   59, 61
#define PPUTLUINT_RANGE_59   58, 60
#define PPUTLUINT_RANGE_58   57, 59
#define PPUTLUINT_RANGE_57   56, 58
#define PPUTLUINT_RANGE_56   55, 57
#define PPUTLUINT_RANGE_55   54, 56
#define PPUTLUINT_RANGE_54   53, 55
#define PPUTLUINT_RANGE_53   52, 54
#define PPUTLUINT_RANGE_52   51, 53
#define PPUTLUINT_RANGE_51   50, 52
#define PPUTLUINT_RANGE_50   49, 51
#define PPUTLUINT_RANGE_49   48, 50
#define PPUTLUINT_RANGE_48   47, 49
#define PPUTLUINT_RANGE_47   46, 48
#define PPUTLUINT_RANGE_46   45, 47
#define PPUTLUINT_RANGE_45   44, 46
#define PPUTLUINT_RANGE_44   43, 45
#define PPUTLUINT_RANGE_43   42, 44
#define PPUTLUINT_RANGE_42   41, 43
#define PPUTLUINT_RANGE_41   40, 42
#define PPUTLUINT_RANGE_40   39, 41
#define PPUTLUINT_RANGE_39   38, 40
#define PPUTLUINT_RANGE_38   37, 39
#define PPUTLUINT_RANGE_37   36, 38
#define PPUTLUINT_RANGE_36   35, 37
#define PPUTLUINT_RANGE_35   34, 36
#define PPUTLUINT_RANGE_34   33, 35
#define PPUTLUINT_RANGE_33   32, 34
#define PPUTLUINT_RANGE_32   31, 33
#define PPUTLUINT_RANGE_31   30, 32
#define PPUTLUINT_RANGE_30   29, 31
#define PPUTLUINT_RANGE_29   28, 30
#define PPUTLUINT_RANGE_28   27, 29
#define PPUTLUINT_RANGE_27   26, 28
#define PPUTLUINT_RANGE_26   25, 27
#define PPUTLUINT_RANGE_25   24, 26
#define PPUTLUINT_RANGE_24   23, 25
#define PPUTLUINT_RANGE_23   22, 24
#define PPUTLUINT_RANGE_22   21, 23
#define PPUTLUINT_RANGE_21   20, 22
#define PPUTLUINT_RANGE_20   19, 21
#define PPUTLUINT_RANGE_19   18, 20
#define PPUTLUINT_RANGE_18   17, 19
#define PPUTLUINT_RANGE_17   16, 18
#define PPUTLUINT_RANGE_16   15, 17
#define PPUTLUINT_RANGE_15   14, 16
#define PPUTLUINT_RANGE_14   13, 15
#define PPUTLUINT_RANGE_13   12, 14
#define PPUTLUINT_RANGE_12   11, 13
#define PPUTLUINT_RANGE_11   10, 12
#define PPUTLUINT_RANGE_10   9, 11
#define PPUTLUINT_RANGE_9    8, 10
#define PPUTLUINT_RANGE_8    7, 9
#define PPUTLUINT_RANGE_7    6, 8
#define PPUTLUINT_RANGE_6    5, 7
#define PPUTLUINT_RANGE_5    4, 6
#define PPUTLUINT_RANGE_4    3, 5
#define PPUTLUINT_RANGE_3    2, 4
#define PPUTLUINT_RANGE_2    1, 3
#define PPUTLUINT_RANGE_1    0, 2
#define PPUTLUINT_RANGE_0    1023, 1

/// full unsigned integer sequences
#define PPUTLUINT_SEQ                                                                                        \
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,  \
      29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53,    \
      54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78,    \
      79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102,     \
      103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,    \
      123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142,    \
      143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162,    \
      163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182,    \
      183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202,    \
      203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222,    \
      223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242,    \
      243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262,    \
      263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282,    \
      283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 301, 302,    \
      303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318, 319, 320, 321, 322,    \
      323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 340, 341, 342,    \
      343, 344, 345, 346, 347, 348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359, 360, 361, 362,    \
      363, 364, 365, 366, 367, 368, 369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 380, 381, 382,    \
      383, 384, 385, 386, 387, 388, 389, 390, 391, 392, 393, 394, 395, 396, 397, 398, 399, 400, 401, 402,    \
      403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416, 417, 418, 419, 420, 421, 422,    \
      423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433, 434, 435, 436, 437, 438, 439, 440, 441, 442,    \
      443, 444, 445, 446, 447, 448, 449, 450, 451, 452, 453, 454, 455, 456, 457, 458, 459, 460, 461, 462,    \
      463, 464, 465, 466, 467, 468, 469, 470, 471, 472, 473, 474, 475, 476, 477, 478, 479, 480, 481, 482,    \
      483, 484, 485, 486, 487, 488, 489, 490, 491, 492, 493, 494, 495, 496, 497, 498, 499, 500, 501, 502,    \
      503, 504, 505, 506, 507, 508, 509, 510, 511, 512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 522,    \
      523, 524, 525, 526, 527, 528, 529, 530, 531, 532, 533, 534, 535, 536, 537, 538, 539, 540, 541, 542,    \
      543, 544, 545, 546, 547, 548, 549, 550, 551, 552, 553, 554, 555, 556, 557, 558, 559, 560, 561, 562,    \
      563, 564, 565, 566, 567, 568, 569, 570, 571, 572, 573, 574, 575, 576, 577, 578, 579, 580, 581, 582,    \
      583, 584, 585, 586, 587, 588, 589, 590, 591, 592, 593, 594, 595, 596, 597, 598, 599, 600, 601, 602,    \
      603, 604, 605, 606, 607, 608, 609, 610, 611, 612, 613, 614, 615, 616, 617, 618, 619, 620, 621, 622,    \
      623, 624, 625, 626, 627, 628, 629, 630, 631, 632, 633, 634, 635, 636, 637, 638, 639, 640, 641, 642,    \
      643, 644, 645, 646, 647, 648, 649, 650, 651, 652, 653, 654, 655, 656, 657, 658, 659, 660, 661, 662,    \
      663, 664, 665, 666, 667, 668, 669, 670, 671, 672, 673, 674, 675, 676, 677, 678, 679, 680, 681, 682,    \
      683, 684, 685, 686, 687, 688, 689, 690, 691, 692, 693, 694, 695, 696, 697, 698, 699, 700, 701, 702,    \
      703, 704, 705, 706, 707, 708, 709, 710, 711, 712, 713, 714, 715, 716, 717, 718, 719, 720, 721, 722,    \
      723, 724, 725, 726, 727, 728, 729, 730, 731, 732, 733, 734, 735, 736, 737, 738, 739, 740, 741, 742,    \
      743, 744, 745, 746, 747, 748, 749, 750, 751, 752, 753, 754, 755, 756, 757, 758, 759, 760, 761, 762,    \
      763, 764, 765, 766, 767, 768, 769, 770, 771, 772, 773, 774, 775, 776, 777, 778, 779, 780, 781, 782,    \
      783, 784, 785, 786, 787, 788, 789, 790, 791, 792, 793, 794, 795, 796, 797, 798, 799, 800, 801, 802,    \
      803, 804, 805, 806, 807, 808, 809, 810, 811, 812, 813, 814, 815, 816, 817, 818, 819, 820, 821, 822,    \
      823, 824, 825, 826, 827, 828, 829, 830, 831, 832, 833, 834, 835, 836, 837, 838, 839, 840, 841, 842,    \
      843, 844, 845, 846, 847, 848, 849, 850, 851, 852, 853, 854, 855, 856, 857, 858, 859, 860, 861, 862,    \
      863, 864, 865, 866, 867, 868, 869, 870, 871, 872, 873, 874, 875, 876, 877, 878, 879, 880, 881, 882,    \
      883, 884, 885, 886, 887, 888, 889, 890, 891, 892, 893, 894, 895, 896, 897, 898, 899, 900, 901, 902,    \
      903, 904, 905, 906, 907, 908, 909, 910, 911, 912, 913, 914, 915, 916, 917, 918, 919, 920, 921, 922,    \
      923, 924, 925, 926, 927, 928, 929, 930, 931, 932, 933, 934, 935, 936, 937, 938, 939, 940, 941, 942,    \
      943, 944, 945, 946, 947, 948, 949, 950, 951, 952, 953, 954, 955, 956, 957, 958, 959, 960, 961, 962,    \
      963, 964, 965, 966, 967, 968, 969, 970, 971, 972, 973, 974, 975, 976, 977, 978, 979, 980, 981, 982,    \
      983, 984, 985, 986, 987, 988, 989, 990, 991, 992, 993, 994, 995, 996, 997, 998, 999, 1000, 1001, 1002, \
      1003, 1004, 1005, 1006, 1007, 1008, 1009, 1010, 1011, 1012, 1013, 1014, 1015, 1016, 1017, 1018, 1019,  \
      1020, 1021, 1022, 1023
#define PPUTLUINT_RSEQ                                                                                       \
  1023, 1022, 1021, 1020, 1019, 1018, 1017, 1016, 1015, 1014, 1013, 1012, 1011, 1010, 1009, 1008, 1007,      \
      1006, 1005, 1004, 1003, 1002, 1001, 1000, 999, 998, 997, 996, 995, 994, 993, 992, 991, 990, 989, 988,  \
      987, 986, 985, 984, 983, 982, 981, 980, 979, 978, 977, 976, 975, 974, 973, 972, 971, 970, 969, 968,    \
      967, 966, 965, 964, 963, 962, 961, 960, 959, 958, 957, 956, 955, 954, 953, 952, 951, 950, 949, 948,    \
      947, 946, 945, 944, 943, 942, 941, 940, 939, 938, 937, 936, 935, 934, 933, 932, 931, 930, 929, 928,    \
      927, 926, 925, 924, 923, 922, 921, 920, 919, 918, 917, 916, 915, 914, 913, 912, 911, 910, 909, 908,    \
      907, 906, 905, 904, 903, 902, 901, 900, 899, 898, 897, 896, 895, 894, 893, 892, 891, 890, 889, 888,    \
      887, 886, 885, 884, 883, 882, 881, 880, 879, 878, 877, 876, 875, 874, 873, 872, 871, 870, 869, 868,    \
      867, 866, 865, 864, 863, 862, 861, 860, 859, 858, 857, 856, 855, 854, 853, 852, 851, 850, 849, 848,    \
      847, 846, 845, 844, 843, 842, 841, 840, 839, 838, 837, 836, 835, 834, 833, 832, 831, 830, 829, 828,    \
      827, 826, 825, 824, 823, 822, 821, 820, 819, 818, 817, 816, 815, 814, 813, 812, 811, 810, 809, 808,    \
      807, 806, 805, 804, 803, 802, 801, 800, 799, 798, 797, 796, 795, 794, 793, 792, 791, 790, 789, 788,    \
      787, 786, 785, 784, 783, 782, 781, 780, 779, 778, 777, 776, 775, 774, 773, 772, 771, 770, 769, 768,    \
      767, 766, 765, 764, 763, 762, 761, 760, 759, 758, 757, 756, 755, 754, 753, 752, 751, 750, 749, 748,    \
      747, 746, 745, 744, 743, 742, 741, 740, 739, 738, 737, 736, 735, 734, 733, 732, 731, 730, 729, 728,    \
      727, 726, 725, 724, 723, 722, 721, 720, 719, 718, 717, 716, 715, 714, 713, 712, 711, 710, 709, 708,    \
      707, 706, 705, 704, 703, 702, 701, 700, 699, 698, 697, 696, 695, 694, 693, 692, 691, 690, 689, 688,    \
      687, 686, 685, 684, 683, 682, 681, 680, 679, 678, 677, 676, 675, 674, 673, 672, 671, 670, 669, 668,    \
      667, 666, 665, 664, 663, 662, 661, 660, 659, 658, 657, 656, 655, 654, 653, 652, 651, 650, 649, 648,    \
      647, 646, 645, 644, 643, 642, 641, 640, 639, 638, 637, 636, 635, 634, 633, 632, 631, 630, 629, 628,    \
      627, 626, 625, 624, 623, 622, 621, 620, 619, 618, 617, 616, 615, 614, 613, 612, 611, 610, 609, 608,    \
      607, 606, 605, 604, 603, 602, 601, 600, 599, 598, 597, 596, 595, 594, 593, 592, 591, 590, 589, 588,    \
      587, 586, 585, 584, 583, 582, 581, 580, 579, 578, 577, 576, 575, 574, 573, 572, 571, 570, 569, 568,    \
      567, 566, 565, 564, 563, 562, 561, 560, 559, 558, 557, 556, 555, 554, 553, 552, 551, 550, 549, 548,    \
      547, 546, 545, 544, 543, 542, 541, 540, 539, 538, 537, 536, 535, 534, 533, 532, 531, 530, 529, 528,    \
      527, 526, 525, 524, 523, 522, 521, 520, 519, 518, 517, 516, 515, 514, 513, 512, 511, 510, 509, 508,    \
      507, 506, 505, 504, 503, 502, 501, 500, 499, 498, 497, 496, 495, 494, 493, 492, 491, 490, 489, 488,    \
      487, 486, 485, 484, 483, 482, 481, 480, 479, 478, 477, 476, 475, 474, 473, 472, 471, 470, 469, 468,    \
      467, 466, 465, 464, 463, 462, 461, 460, 459, 458, 457, 456, 455, 454, 453, 452, 451, 450, 449, 448,    \
      447, 446, 445, 444, 443, 442, 441, 440, 439, 438, 437, 436, 435, 434, 433, 432, 431, 430, 429, 428,    \
      427, 426, 425, 424, 423, 422, 421, 420, 419, 418, 417, 416, 415, 414, 413, 412, 411, 410, 409, 408,    \
      407, 406, 405, 404, 403, 402, 401, 400, 399, 398, 397, 396, 395, 394, 393, 392, 391, 390, 389, 388,    \
      387, 386, 385, 384, 383, 382, 381, 380, 379, 378, 377, 376, 375, 374, 373, 372, 371, 370, 369, 368,    \
      367, 366, 365, 364, 363, 362, 361, 360, 359, 358, 357, 356, 355, 354, 353, 352, 351, 350, 349, 348,    \
      347, 346, 345, 344, 343, 342, 341, 340, 339, 338, 337, 336, 335, 334, 333, 332, 331, 330, 329, 328,    \
      327, 326, 325, 324, 323, 322, 321, 320, 319, 318, 317, 316, 315, 314, 313, 312, 311, 310, 309, 308,    \
      307, 306, 305, 304, 303, 302, 301, 300, 299, 298, 297, 296, 295, 294, 293, 292, 291, 290, 289, 288,    \
      287, 286, 285, 284, 283, 282, 281, 280, 279, 278, 277, 276, 275, 274, 273, 272, 271, 270, 269, 268,    \
      267, 266, 265, 264, 263, 262, 261, 260, 259, 258, 257, 256, 255, 254, 253, 252, 251, 250, 249, 248,    \
      247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 237, 236, 235, 234, 233, 232, 231, 230, 229, 228,    \
      227, 226, 225, 224, 223, 222, 221, 220, 219, 218, 217, 216, 215, 214, 213, 212, 211, 210, 209, 208,    \
      207, 206, 205, 204, 203, 202, 201, 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 189, 188,    \
      187, 186, 185, 184, 183, 182, 181, 180, 179, 178, 177, 176, 175, 174, 173, 172, 171, 170, 169, 168,    \
      167, 166, 165, 164, 163, 162, 161, 160, 159, 158, 157, 156, 155, 154, 153, 152, 151, 150, 149, 148,    \
      147, 146, 145, 144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128,    \
      127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108,    \
      107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85,    \
      84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60,    \
      59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35,    \
      34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, \
      8, 7, 6, 5, 4, 3, 2, 1, 0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [numeric.inc]
/// -------------
/// uint increment w/ overflow.
///
/// PTL_INC(0)    // 1
/// PTL_INC(1)    // 2
/// PTL_INC(1023) // 0
#define PTL_INC(/* n: uint */...) /* -> uint{n+1} */ \
  PTL_REST(PTL_CAT(PPUTLUINT_RANGE_, PTL_UINT(__VA_ARGS__)))

/// [numeric.dec]
/// -------------
/// uint decrement w/ underflow.
///
/// PTL_DEC(0)    // 1023
/// PTL_DEC(1)    // 0
/// PTL_DEC(1023) // 1022
#define PTL_DEC(/* n: uint */...) /* -> uint{n-1} */ \
  PTL_FIRST(PTL_CAT(PPUTLUINT_RANGE_, PTL_UINT(__VA_ARGS__)))

/// [numeric.eqz]
/// -------------
/// detects if uint n is zero.
///
/// PTL_EQZ(0)             // 1
/// PTL_EQZ(1)             // 0
/// PTL_EQZ(2)             // 0
/// PTL_EQZ(1023)          // 0
/// PTL_EQZ(PTL_INC(1023)) // 1
#define PTL_EQZ(/* n: uint */...) /* -> uint{n==0} */ PPUTLEQZ_X(PTL_CAT(PPUTLEQZ_, PTL_UINT(__VA_ARGS__)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLEQZ_X(...)  PPUTLEQZ_##__VA_OPT__(NO_)##PASS
#define PPUTLEQZ_NO_PASS 0
#define PPUTLEQZ_PASS    1
#define PPUTLEQZ_0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [numeric.nez]
/// -------------
/// detects if uint n is not zero.
///
/// PTL_NEZ(0)             // 0
/// PTL_NEZ(1)             // 1
/// PTL_NEZ(2)             // 1
/// PTL_NEZ(1023)          // 1
/// PTL_NEZ(PTL_INC(1023)) // 0
#define PTL_NEZ(/* n: uint */...) /* -> uint{n!=0} */ PPUTLNEZ_X(PTL_CAT(PPUTLNEZ_, PTL_UINT(__VA_ARGS__)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLNEZ_X(...)  PPUTLNEZ_##__VA_OPT__(NO_)##FAIL
#define PPUTLNEZ_NO_FAIL 1
#define PPUTLNEZ_FAIL    0
#define PPUTLNEZ_0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.items]
/// --------------
/// extracts tuple items.
///
/// PTL_ITEMS(())        // <nothing>
/// PTL_ITEMS((a))       // a
/// PTL_ITEMS((a, b))    // a, b
/// PTL_ITEMS((a, b, c)) // a, b, c
#define PTL_ITEMS(/* tup */...) /* -> ...tup */ PTL_ESC __VA_ARGS__

/// [traits.is_none]
/// ----------------
/// detects if args is nothing.
///
/// PTL_IS_NONE()          // 1
/// PTL_IS_NONE(foo)       // 0
/// PTL_IS_NONE(foo, bar)  // 0
/// PTL_IS_NONE(PTL_ESC()) // 1
#define PTL_IS_NONE(...) /* -> bool */ PPUTLIS_NONE_O##__VA_OPT__(0)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_NONE_O0 0
#define PPUTLIS_NONE_O  1

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_some]
/// ----------------
/// detects if args is something.
///
/// PTL_IS_SOME()          // 0
/// PTL_IS_SOME(foo)       // 1
/// PTL_IS_SOME(foo, bar)  // 1
/// PTL_IS_SOME(PTL_ESC()) // 0
#define PTL_IS_SOME(...) /* -> bool */ PPUTLIS_SOME_O##__VA_OPT__(1)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_SOME_O1 1
#define PPUTLIS_SOME_O  0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.is_tup]
/// ---------------
/// detects if args is a tuple.
///
/// PTL_IS_TUP()       // 0
/// PTL_IS_TUP(1, 2)   // 0
/// PTL_IS_TUP(())     // 1
/// PTL_IS_TUP((1, 2)) // 1
#define PTL_IS_TUP(...) /* -> bool */ PTL_IS_NONE(PTL_EAT __VA_ARGS__)

/// [traits.is_uint]
/// ----------------
/// detects if args is a uint.
///
/// PTL_IS_UINT()       // 0
/// PTL_IS_UINT(foo)    // 0
/// PTL_IS_UINT(0)      // 1
/// PTL_IS_UINT(())     // 0
/// PTL_IS_UINT((), ()) // 0
/// PTL_IS_UINT(0, 1)   // 0
/// PTL_IS_UINT(1023)   // 1
#define PTL_IS_UINT(...) /* -> bool */ PTL_IS_SOME(PTL_CAT(PPUTLIS_UINT_CHK_, PTL_UINT(__VA_ARGS__)))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLIS_UINT_CHK_PTL_UINT(...)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [traits.size]
/// -------------
/// computes the uint size of args in O(1) time.
/// terminates expansion with an error if too many args passed.
///
/// PTL_SIZE()     // 0
/// PTL_SIZE(a)    // 1
/// PTL_SIZE(a, b) // 2
/// PTL_SIZE(, )   // 2
#define PTL_SIZE(...) /* -> uint */ PPUTLSIZE_X(PPUTLSIZE, __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLSIZE_X(_, ...)                                                                                  \
  PPUTLSIZE_READ(                                                                                            \
      __VA_ARGS__ __VA_OPT__(, ) _(1023), _(1022), _(1021), _(1020), _(1019), _(1018), _(1017), _(1016),     \
      _(1015), _(1014), _(1013), _(1012), _(1011), _(1010), _(1009), _(1008), _(1007), _(1006), _(1005),     \
      _(1004), _(1003), _(1002), _(1001), _(1000), _(999), _(998), _(997), _(996), _(995), _(994), _(993),   \
      _(992), _(991), _(990), _(989), _(988), _(987), _(986), _(985), _(984), _(983), _(982), _(981),        \
      _(980), _(979), _(978), _(977), _(976), _(975), _(974), _(973), _(972), _(971), _(970), _(969),        \
      _(968), _(967), _(966), _(965), _(964), _(963), _(962), _(961), _(960), _(959), _(958), _(957),        \
      _(956), _(955), _(954), _(953), _(952), _(951), _(950), _(949), _(948), _(947), _(946), _(945),        \
      _(944), _(943), _(942), _(941), _(940), _(939), _(938), _(937), _(936), _(935), _(934), _(933),        \
      _(932), _(931), _(930), _(929), _(928), _(927), _(926), _(925), _(924), _(923), _(922), _(921),        \
      _(920), _(919), _(918), _(917), _(916), _(915), _(914), _(913), _(912), _(911), _(910), _(909),        \
      _(908), _(907), _(906), _(905), _(904), _(903), _(902), _(901), _(900), _(899), _(898), _(897),        \
      _(896), _(895), _(894), _(893), _(892), _(891), _(890), _(889), _(888), _(887), _(886), _(885),        \
      _(884), _(883), _(882), _(881), _(880), _(879), _(878), _(877), _(876), _(875), _(874), _(873),        \
      _(872), _(871), _(870), _(869), _(868), _(867), _(866), _(865), _(864), _(863), _(862), _(861),        \
      _(860), _(859), _(858), _(857), _(856), _(855), _(854), _(853), _(852), _(851), _(850), _(849),        \
      _(848), _(847), _(846), _(845), _(844), _(843), _(842), _(841), _(840), _(839), _(838), _(837),        \
      _(836), _(835), _(834), _(833), _(832), _(831), _(830), _(829), _(828), _(827), _(826), _(825),        \
      _(824), _(823), _(822), _(821), _(820), _(819), _(818), _(817), _(816), _(815), _(814), _(813),        \
      _(812), _(811), _(810), _(809), _(808), _(807), _(806), _(805), _(804), _(803), _(802), _(801),        \
      _(800), _(799), _(798), _(797), _(796), _(795), _(794), _(793), _(792), _(791), _(790), _(789),        \
      _(788), _(787), _(786), _(785), _(784), _(783), _(782), _(781), _(780), _(779), _(778), _(777),        \
      _(776), _(775), _(774), _(773), _(772), _(771), _(770), _(769), _(768), _(767), _(766), _(765),        \
      _(764), _(763), _(762), _(761), _(760), _(759), _(758), _(757), _(756), _(755), _(754), _(753),        \
      _(752), _(751), _(750), _(749), _(748), _(747), _(746), _(745), _(744), _(743), _(742), _(741),        \
      _(740), _(739), _(738), _(737), _(736), _(735), _(734), _(733), _(732), _(731), _(730), _(729),        \
      _(728), _(727), _(726), _(725), _(724), _(723), _(722), _(721), _(720), _(719), _(718), _(717),        \
      _(716), _(715), _(714), _(713), _(712), _(711), _(710), _(709), _(708), _(707), _(706), _(705),        \
      _(704), _(703), _(702), _(701), _(700), _(699), _(698), _(697), _(696), _(695), _(694), _(693),        \
      _(692), _(691), _(690), _(689), _(688), _(687), _(686), _(685), _(684), _(683), _(682), _(681),        \
      _(680), _(679), _(678), _(677), _(676), _(675), _(674), _(673), _(672), _(671), _(670), _(669),        \
      _(668), _(667), _(666), _(665), _(664), _(663), _(662), _(661), _(660), _(659), _(658), _(657),        \
      _(656), _(655), _(654), _(653), _(652), _(651), _(650), _(649), _(648), _(647), _(646), _(645),        \
      _(644), _(643), _(642), _(641), _(640), _(639), _(638), _(637), _(636), _(635), _(634), _(633),        \
      _(632), _(631), _(630), _(629), _(628), _(627), _(626), _(625), _(624), _(623), _(622), _(621),        \
      _(620), _(619), _(618), _(617), _(616), _(615), _(614), _(613), _(612), _(611), _(610), _(609),        \
      _(608), _(607), _(606), _(605), _(604), _(603), _(602), _(601), _(600), _(599), _(598), _(597),        \
      _(596), _(595), _(594), _(593), _(592), _(591), _(590), _(589), _(588), _(587), _(586), _(585),        \
      _(584), _(583), _(582), _(581), _(580), _(579), _(578), _(577), _(576), _(575), _(574), _(573),        \
      _(572), _(571), _(570), _(569), _(568), _(567), _(566), _(565), _(564), _(563), _(562), _(561),        \
      _(560), _(559), _(558), _(557), _(556), _(555), _(554), _(553), _(552), _(551), _(550), _(549),        \
      _(548), _(547), _(546), _(545), _(544), _(543), _(542), _(541), _(540), _(539), _(538), _(537),        \
      _(536), _(535), _(534), _(533), _(532), _(531), _(530), _(529), _(528), _(527), _(526), _(525),        \
      _(524), _(523), _(522), _(521), _(520), _(519), _(518), _(517), _(516), _(515), _(514), _(513),        \
      _(512), _(511), _(510), _(509), _(508), _(507), _(506), _(505), _(504), _(503), _(502), _(501),        \
      _(500), _(499), _(498), _(497), _(496), _(495), _(494), _(493), _(492), _(491), _(490), _(489),        \
      _(488), _(487), _(486), _(485), _(484), _(483), _(482), _(481), _(480), _(479), _(478), _(477),        \
      _(476), _(475), _(474), _(473), _(472), _(471), _(470), _(469), _(468), _(467), _(466), _(465),        \
      _(464), _(463), _(462), _(461), _(460), _(459), _(458), _(457), _(456), _(455), _(454), _(453),        \
      _(452), _(451), _(450), _(449), _(448), _(447), _(446), _(445), _(444), _(443), _(442), _(441),        \
      _(440), _(439), _(438), _(437), _(436), _(435), _(434), _(433), _(432), _(431), _(430), _(429),        \
      _(428), _(427), _(426), _(425), _(424), _(423), _(422), _(421), _(420), _(419), _(418), _(417),        \
      _(416), _(415), _(414), _(413), _(412), _(411), _(410), _(409), _(408), _(407), _(406), _(405),        \
      _(404), _(403), _(402), _(401), _(400), _(399), _(398), _(397), _(396), _(395), _(394), _(393),        \
      _(392), _(391), _(390), _(389), _(388), _(387), _(386), _(385), _(384), _(383), _(382), _(381),        \
      _(380), _(379), _(378), _(377), _(376), _(375), _(374), _(373), _(372), _(371), _(370), _(369),        \
      _(368), _(367), _(366), _(365), _(364), _(363), _(362), _(361), _(360), _(359), _(358), _(357),        \
      _(356), _(355), _(354), _(353), _(352), _(351), _(350), _(349), _(348), _(347), _(346), _(345),        \
      _(344), _(343), _(342), _(341), _(340), _(339), _(338), _(337), _(336), _(335), _(334), _(333),        \
      _(332), _(331), _(330), _(329), _(328), _(327), _(326), _(325), _(324), _(323), _(322), _(321),        \
      _(320), _(319), _(318), _(317), _(316), _(315), _(314), _(313), _(312), _(311), _(310), _(309),        \
      _(308), _(307), _(306), _(305), _(304), _(303), _(302), _(301), _(300), _(299), _(298), _(297),        \
      _(296), _(295), _(294), _(293), _(292), _(291), _(290), _(289), _(288), _(287), _(286), _(285),        \
      _(284), _(283), _(282), _(281), _(280), _(279), _(278), _(277), _(276), _(275), _(274), _(273),        \
      _(272), _(271), _(270), _(269), _(268), _(267), _(266), _(265), _(264), _(263), _(262), _(261),        \
      _(260), _(259), _(258), _(257), _(256), _(255), _(254), _(253), _(252), _(251), _(250), _(249),        \
      _(248), _(247), _(246), _(245), _(244), _(243), _(242), _(241), _(240), _(239), _(238), _(237),        \
      _(236), _(235), _(234), _(233), _(232), _(231), _(230), _(229), _(228), _(227), _(226), _(225),        \
      _(224), _(223), _(222), _(221), _(220), _(219), _(218), _(217), _(216), _(215), _(214), _(213),        \
      _(212), _(211), _(210), _(209), _(208), _(207), _(206), _(205), _(204), _(203), _(202), _(201),        \
      _(200), _(199), _(198), _(197), _(196), _(195), _(194), _(193), _(192), _(191), _(190), _(189),        \
      _(188), _(187), _(186), _(185), _(184), _(183), _(182), _(181), _(180), _(179), _(178), _(177),        \
      _(176), _(175), _(174), _(173), _(172), _(171), _(170), _(169), _(168), _(167), _(166), _(165),        \
      _(164), _(163), _(162), _(161), _(160), _(159), _(158), _(157), _(156), _(155), _(154), _(153),        \
      _(152), _(151), _(150), _(149), _(148), _(147), _(146), _(145), _(144), _(143), _(142), _(141),        \
      _(140), _(139), _(138), _(137), _(136), _(135), _(134), _(133), _(132), _(131), _(130), _(129),        \
      _(128), _(127), _(126), _(125), _(124), _(123), _(122), _(121), _(120), _(119), _(118), _(117),        \
      _(116), _(115), _(114), _(113), _(112), _(111), _(110), _(109), _(108), _(107), _(106), _(105),        \
      _(104), _(103), _(102), _(101), _(100), _(99), _(98), _(97), _(96), _(95), _(94), _(93), _(92), _(91), \
      _(90), _(89), _(88), _(87), _(86), _(85), _(84), _(83), _(82), _(81), _(80), _(79), _(78), _(77),      \
      _(76), _(75), _(74), _(73), _(72), _(71), _(70), _(69), _(68), _(67), _(66), _(65), _(64), _(63),      \
      _(62), _(61), _(60), _(59), _(58), _(57), _(56), _(55), _(54), _(53), _(52), _(51), _(50), _(49),      \
      _(48), _(47), _(46), _(45), _(44), _(43), _(42), _(41), _(40), _(39), _(38), _(37), _(36), _(35),      \
      _(34), _(33), _(32), _(31), _(30), _(29), _(28), _(27), _(26), _(25), _(24), _(23), _(22), _(21),      \
      _(20), _(19), _(18), _(17), _(16), _(15), _(14), _(13), _(12), _(11), _(10), _(9), _(8), _(7), _(6),   \
      _(5), _(4), _(3), _(2), _(1), _(0))
#define PPUTLSIZE_READ(                                                                                      \
    a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, A, B, C, D, E, F, G, H, I, \
    J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, ba, bb, bc, bd, be, bf, bg, bh, bi, bj, bk, bl, bm,   \
    bn, bo, bp, bq, br, bs, bt, bu, bv, bw, bx, by, bz, bA, bB, bC, bD, bE, bF, bG, bH, bI, bJ, bK, bL, bM,  \
    bN, bO, bP, bQ, bR, bS, bT, bU, bV, bW, bX, bY, bZ, ca, cb, cc, cd, ce, cf, cg, ch, ci, cj, ck, cl, cm,  \
    cn, co, cp, cq, cr, cs, ct, cu, cv, cw, cx, cy, cz, cA, cB, cC, cD, cE, cF, cG, cH, cI, cJ, cK, cL, cM,  \
    cN, cO, cP, cQ, cR, cS, cT, cU, cV, cW, cX, cY, cZ, da, db, dc, dd, de, df, dg, dh, di, dj, dk, dl, dm,  \
    dn, do, dp, dq, dr, ds, dt, du, dv, dw, dx, dy, dz, dA, dB, dC, dD, dE, dF, dG, dH, dI, dJ, dK, dL, dM,  \
    dN, dO, dP, dQ, dR, dS, dT, dU, dV, dW, dX, dY, dZ, ea, eb, ec, ed, ee, ef, eg, eh, ei, ej, ek, el, em,  \
    en, eo, ep, eq, er, es, et, eu, ev, ew, ex, ey, ez, eA, eB, eC, eD, eE, eF, eG, eH, eI, eJ, eK, eL, eM,  \
    eN, eO, eP, eQ, eR, eS, eT, eU, eV, eW, eX, eY, eZ, fa, fb, fc, fd, fe, ff, fg, fh, fi, fj, fk, fl, fm,  \
    fn, fo, fp, fq, fr, fs, ft, fu, fv, fw, fx, fy, fz, fA, fB, fC, fD, fE, fF, fG, fH, fI, fJ, fK, fL, fM,  \
    fN, fO, fP, fQ, fR, fS, fT, fU, fV, fW, fX, fY, fZ, ga, gb, gc, gd, ge, gf, gg, gh, gi, gj, gk, gl, gm,  \
    gn, go, gp, gq, gr, gs, gt, gu, gv, gw, gx, gy, gz, gA, gB, gC, gD, gE, gF, gG, gH, gI, gJ, gK, gL, gM,  \
    gN, gO, gP, gQ, gR, gS, gT, gU, gV, gW, gX, gY, gZ, ha, hb, hc, hd, he, hf, hg, hh, hi, hj, hk, hl, hm,  \
    hn, ho, hp, hq, hr, hs, ht, hu, hv, hw, hx, hy, hz, hA, hB, hC, hD, hE, hF, hG, hH, hI, hJ, hK, hL, hM,  \
    hN, hO, hP, hQ, hR, hS, hT, hU, hV, hW, hX, hY, hZ, ia, ib, ic, id, ie, if, ig, ih, ii, ij, ik, il, im,  \
    in, io, ip, iq, ir, is, it, iu, iv, iw, ix, iy, iz, iA, iB, iC, iD, iE, iF, iG, iH, iI, iJ, iK, iL, iM,  \
    iN, iO, iP, iQ, iR, iS, iT, iU, iV, iW, iX, iY, iZ, ja, jb, jc, jd, je, jf, jg, jh, ji, jj, jk, jl, jm,  \
    jn, jo, jp, jq, jr, js, jt, ju, jv, jw, jx, jy, jz, jA, jB, jC, jD, jE, jF, jG, jH, jI, jJ, jK, jL, jM,  \
    jN, jO, jP, jQ, jR, jS, jT, jU, jV, jW, jX, jY, jZ, ka, kb, kc, kd, ke, kf, kg, kh, ki, kj, kk, kl, km,  \
    kn, ko, kp, kq, kr, ks, kt, ku, kv, kw, kx, ky, kz, kA, kB, kC, kD, kE, kF, kG, kH, kI, kJ, kK, kL, kM,  \
    kN, kO, kP, kQ, kR, kS, kT, kU, kV, kW, kX, kY, kZ, la, lb, lc, ld, le, lf, lg, lh, li, lj, lk, ll, lm,  \
    ln, lo, lp, lq, lr, ls, lt, lu, lv, lw, lx, ly, lz, lA, lB, lC, lD, lE, lF, lG, lH, lI, lJ, lK, lL, lM,  \
    lN, lO, lP, lQ, lR, lS, lT, lU, lV, lW, lX, lY, lZ, ma, mb, mc, md, me, mf, mg, mh, mi, mj, mk, ml, mm,  \
    mn, mo, mp, mq, mr, ms, mt, mu, mv, mw, mx, my, mz, mA, mB, mC, mD, mE, mF, mG, mH, mI, mJ, mK, mL, mM,  \
    mN, mO, mP, mQ, mR, mS, mT, mU, mV, mW, mX, mY, mZ, na, nb, nc, nd, ne, nf, ng, nh, ni, nj, nk, nl, nm,  \
    nn, no, np, nq, nr, ns, nt, nu, nv, nw, nx, ny, nz, nA, nB, nC, nD, nE, nF, nG, nH, nI, nJ, nK, nL, nM,  \
    nN, nO, nP, nQ, nR, nS, nT, nU, nV, nW, nX, nY, nZ, oa, ob, oc, od, oe, of, og, oh, oi, oj, ok, ol, om,  \
    on, oo, op, oq, _or, os, ot, ou, ov, ow, ox, oy, oz, oA, oB, oC, oD, oE, oF, oG, oH, oI, oJ, oK, oL, oM, \
    oN, oO, oP, oQ, oR, oS, oT, oU, oV, oW, oX, oY, oZ, pa, pb, pc, pd, pe, pf, pg, ph, pi, pj, pk, pl, pm,  \
    pn, po, pp, pq, pr, ps, pt, pu, pv, pw, px, py, pz, pA, pB, pC, pD, pE, pF, pG, pH, pI, pJ, pK, pL, pM,  \
    pN, pO, pP, pQ, pR, pS, pT, pU, pV, pW, pX, pY, pZ, qa, qb, qc, qd, qe, qf, qg, qh, qi, qj, qk, ql, qm,  \
    qn, qo, qp, qq, qr, qs, qt, qu, qv, qw, qx, qy, qz, qA, qB, qC, qD, qE, qF, qG, qH, qI, qJ, qK, qL, qM,  \
    qN, qO, qP, qQ, qR, qS, qT, qU, qV, qW, qX, qY, qZ, ra, rb, rc, rd, re, rf, rg, rh, ri, rj, rk, rl, rm,  \
    rn, ro, rp, rq, rr, rs, rt, ru, rv, rw, rx, ry, rz, rA, rB, rC, rD, rE, rF, rG, rH, rI, rJ, rK, rL, rM,  \
    rN, rO, rP, rQ, rR, rS, rT, rU, rV, rW, rX, rY, rZ, sa, sb, sc, sd, se, sf, sg, sh, si, sj, sk, sl, sm,  \
    sn, so, sp, sq, sr, ss, st, su, sv, sw, sx, sy, sz, sA, sB, sC, sD, sE, sF, sG, sH, sI, sJ, sK, sL, sM,  \
    sN, sO, sP, sQ, sR, sS, sT, sU, sV, sW, sX, sY, sZ, ta, tb, tc, td, te, tf, tg, th, ti, tj, tk, tl, tm,  \
    tn, to, tp, tq, tr, ts, tt, tu, tv, tw, tx, ty, tz, tA, tB, tC, tD, tE, tF, tG, tH, tI, _sz, ...)        \
  PTL_CAT(PPUTLSIZE_READ_, PTL_IS_NONE(PPUTLSIZE_##_sz))(_sz)
#define PPUTLSIZE_READ_1(_sz)         PPUTLSIZE_READ_##_sz
#define PPUTLSIZE_READ_0(_sz)         PTL_UINT(PTL_SIZE(Error : too many args))
#define PPUTLSIZE_READ_PPUTLSIZE(...) __VA_ARGS__
#define PPUTLSIZE_PPUTLSIZE(...)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [meta.xtrace]
/// -------------
/// counts the number of expansions undergone after expression.
/// uses recursion; can track any number of expansions.
/// the number of commas indicates the number of expansions.
///
/// PTL_STR(PTL_XTRACE)                            // "PPUTLXTRACE_A ( , )"
/// PTL_STR(PTL_ESC(PTL_XTRACE))                   // "PPUTLXTRACE_B ( ,, )"
/// PTL_STR(PTL_ESC(PTL_ESC(PTL_XTRACE)))          // "PPUTLXTRACE_A ( ,,, )"
/// PTL_STR(PTL_ESC(PTL_ESC(PTL_ESC(PTL_XTRACE)))) // "PPUTLXTRACE_B ( ,,,, )"
#define PTL_XTRACE /* -> <xtrace expr> */ PPUTLXTRACE_A PTL_LP /**/, PTL_RP

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLXTRACE_B(...) PPUTLXTRACE_A PTL_LP __VA_ARGS__, PTL_RP
#define PPUTLXTRACE_A(...) PPUTLXTRACE_B PTL_LP __VA_ARGS__, PTL_RP

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [meta.xcount]
/// -------------
/// extracts the result from an PTL_XTRACE expression.
/// expansion count must be no greater than 1023.
///
/// ignores the expansion required to read the result;
/// result ranges from 0 to 1023.
///
/// PTL_XCOUNT(PTL_XTRACE)                            // 0
/// PTL_XCOUNT(PTL_ESC(PTL_XTRACE))                   // 1
/// PTL_XCOUNT(PTL_ESC(PTL_ESC(PTL_XTRACE)))          // 2
/// PTL_XCOUNT(PTL_ESC(PTL_ESC(PTL_ESC(PTL_XTRACE)))) // 3
#define PTL_XCOUNT(/* <xtrace expr> */...) /* -> uint */ PPUTLXCOUNT_X(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLXCOUNT_X(...)             PPUTLXCOUNT_##__VA_ARGS__
#define PPUTLXCOUNT_PPUTLXTRACE_B(...) PPUTLXCOUNT_RES(__VA_ARGS__.)
#define PPUTLXCOUNT_PPUTLXTRACE_A(...) PPUTLXCOUNT_RES(__VA_ARGS__.)
#define PPUTLXCOUNT_RES(_, __, ...)    PTL_SIZE(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [meta.x]
/// --------
/// performs uint n secondary expansions (n=0 -> identity).
/// args are expressed after n+1 expansions in total.
/// useful for implementing mutual recursion.
///
/// PTL_X(0)(PTL_XTRACE) // PTL_ESC(PTL_XTRACE)
/// PTL_X(1)(PTL_XTRACE) // PTL_ESC(PTL_ESC(PTL_XTRACE))
/// PTL_X(0)(PTL_XTRACE) // PPUTLXTRACE_A ( , )
/// PTL_X(1)(PTL_XTRACE) // PPUTLXTRACE_B ( ,, )
#define PTL_X(/* n: uint */...) /* -> (args: any...) -<n>-> ...args */ PTL_CAT(PPUTLX_, PTL_UINT(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLX_1023(...) PPUTLX_254(PPUTLX_255(PPUTLX_255(PPUTLX_255(__VA_ARGS__))))
#define PPUTLX_1022(...) PPUTLX_254(PPUTLX_254(PPUTLX_255(PPUTLX_255(__VA_ARGS__))))
#define PPUTLX_1021(...) PPUTLX_254(PPUTLX_254(PPUTLX_254(PPUTLX_255(__VA_ARGS__))))
#define PPUTLX_1020(...) PPUTLX_254(PPUTLX_254(PPUTLX_254(PPUTLX_254(__VA_ARGS__))))
#define PPUTLX_1019(...) PPUTLX_253(PPUTLX_254(PPUTLX_254(PPUTLX_254(__VA_ARGS__))))
#define PPUTLX_1018(...) PPUTLX_253(PPUTLX_253(PPUTLX_254(PPUTLX_254(__VA_ARGS__))))
#define PPUTLX_1017(...) PPUTLX_253(PPUTLX_253(PPUTLX_253(PPUTLX_254(__VA_ARGS__))))
#define PPUTLX_1016(...) PPUTLX_253(PPUTLX_253(PPUTLX_253(PPUTLX_253(__VA_ARGS__))))
#define PPUTLX_1015(...) PPUTLX_252(PPUTLX_253(PPUTLX_253(PPUTLX_253(__VA_ARGS__))))
#define PPUTLX_1014(...) PPUTLX_252(PPUTLX_252(PPUTLX_253(PPUTLX_253(__VA_ARGS__))))
#define PPUTLX_1013(...) PPUTLX_252(PPUTLX_252(PPUTLX_252(PPUTLX_253(__VA_ARGS__))))
#define PPUTLX_1012(...) PPUTLX_252(PPUTLX_252(PPUTLX_252(PPUTLX_252(__VA_ARGS__))))
#define PPUTLX_1011(...) PPUTLX_251(PPUTLX_252(PPUTLX_252(PPUTLX_252(__VA_ARGS__))))
#define PPUTLX_1010(...) PPUTLX_251(PPUTLX_251(PPUTLX_252(PPUTLX_252(__VA_ARGS__))))
#define PPUTLX_1009(...) PPUTLX_251(PPUTLX_251(PPUTLX_251(PPUTLX_252(__VA_ARGS__))))
#define PPUTLX_1008(...) PPUTLX_251(PPUTLX_251(PPUTLX_251(PPUTLX_251(__VA_ARGS__))))
#define PPUTLX_1007(...) PPUTLX_250(PPUTLX_251(PPUTLX_251(PPUTLX_251(__VA_ARGS__))))
#define PPUTLX_1006(...) PPUTLX_250(PPUTLX_250(PPUTLX_251(PPUTLX_251(__VA_ARGS__))))
#define PPUTLX_1005(...) PPUTLX_250(PPUTLX_250(PPUTLX_250(PPUTLX_251(__VA_ARGS__))))
#define PPUTLX_1004(...) PPUTLX_250(PPUTLX_250(PPUTLX_250(PPUTLX_250(__VA_ARGS__))))
#define PPUTLX_1003(...) PPUTLX_249(PPUTLX_250(PPUTLX_250(PPUTLX_250(__VA_ARGS__))))
#define PPUTLX_1002(...) PPUTLX_249(PPUTLX_249(PPUTLX_250(PPUTLX_250(__VA_ARGS__))))
#define PPUTLX_1001(...) PPUTLX_249(PPUTLX_249(PPUTLX_249(PPUTLX_250(__VA_ARGS__))))
#define PPUTLX_1000(...) PPUTLX_249(PPUTLX_249(PPUTLX_249(PPUTLX_249(__VA_ARGS__))))
#define PPUTLX_999(...)  PPUTLX_248(PPUTLX_249(PPUTLX_249(PPUTLX_249(__VA_ARGS__))))
#define PPUTLX_998(...)  PPUTLX_248(PPUTLX_248(PPUTLX_249(PPUTLX_249(__VA_ARGS__))))
#define PPUTLX_997(...)  PPUTLX_248(PPUTLX_248(PPUTLX_248(PPUTLX_249(__VA_ARGS__))))
#define PPUTLX_996(...)  PPUTLX_248(PPUTLX_248(PPUTLX_248(PPUTLX_248(__VA_ARGS__))))
#define PPUTLX_995(...)  PPUTLX_247(PPUTLX_248(PPUTLX_248(PPUTLX_248(__VA_ARGS__))))
#define PPUTLX_994(...)  PPUTLX_247(PPUTLX_247(PPUTLX_248(PPUTLX_248(__VA_ARGS__))))
#define PPUTLX_993(...)  PPUTLX_247(PPUTLX_247(PPUTLX_247(PPUTLX_248(__VA_ARGS__))))
#define PPUTLX_992(...)  PPUTLX_247(PPUTLX_247(PPUTLX_247(PPUTLX_247(__VA_ARGS__))))
#define PPUTLX_991(...)  PPUTLX_246(PPUTLX_247(PPUTLX_247(PPUTLX_247(__VA_ARGS__))))
#define PPUTLX_990(...)  PPUTLX_246(PPUTLX_246(PPUTLX_247(PPUTLX_247(__VA_ARGS__))))
#define PPUTLX_989(...)  PPUTLX_246(PPUTLX_246(PPUTLX_246(PPUTLX_247(__VA_ARGS__))))
#define PPUTLX_988(...)  PPUTLX_246(PPUTLX_246(PPUTLX_246(PPUTLX_246(__VA_ARGS__))))
#define PPUTLX_987(...)  PPUTLX_245(PPUTLX_246(PPUTLX_246(PPUTLX_246(__VA_ARGS__))))
#define PPUTLX_986(...)  PPUTLX_245(PPUTLX_245(PPUTLX_246(PPUTLX_246(__VA_ARGS__))))
#define PPUTLX_985(...)  PPUTLX_245(PPUTLX_245(PPUTLX_245(PPUTLX_246(__VA_ARGS__))))
#define PPUTLX_984(...)  PPUTLX_245(PPUTLX_245(PPUTLX_245(PPUTLX_245(__VA_ARGS__))))
#define PPUTLX_983(...)  PPUTLX_244(PPUTLX_245(PPUTLX_245(PPUTLX_245(__VA_ARGS__))))
#define PPUTLX_982(...)  PPUTLX_244(PPUTLX_244(PPUTLX_245(PPUTLX_245(__VA_ARGS__))))
#define PPUTLX_981(...)  PPUTLX_244(PPUTLX_244(PPUTLX_244(PPUTLX_245(__VA_ARGS__))))
#define PPUTLX_980(...)  PPUTLX_244(PPUTLX_244(PPUTLX_244(PPUTLX_244(__VA_ARGS__))))
#define PPUTLX_979(...)  PPUTLX_243(PPUTLX_244(PPUTLX_244(PPUTLX_244(__VA_ARGS__))))
#define PPUTLX_978(...)  PPUTLX_243(PPUTLX_243(PPUTLX_244(PPUTLX_244(__VA_ARGS__))))
#define PPUTLX_977(...)  PPUTLX_243(PPUTLX_243(PPUTLX_243(PPUTLX_244(__VA_ARGS__))))
#define PPUTLX_976(...)  PPUTLX_243(PPUTLX_243(PPUTLX_243(PPUTLX_243(__VA_ARGS__))))
#define PPUTLX_975(...)  PPUTLX_242(PPUTLX_243(PPUTLX_243(PPUTLX_243(__VA_ARGS__))))
#define PPUTLX_974(...)  PPUTLX_242(PPUTLX_242(PPUTLX_243(PPUTLX_243(__VA_ARGS__))))
#define PPUTLX_973(...)  PPUTLX_242(PPUTLX_242(PPUTLX_242(PPUTLX_243(__VA_ARGS__))))
#define PPUTLX_972(...)  PPUTLX_242(PPUTLX_242(PPUTLX_242(PPUTLX_242(__VA_ARGS__))))
#define PPUTLX_971(...)  PPUTLX_241(PPUTLX_242(PPUTLX_242(PPUTLX_242(__VA_ARGS__))))
#define PPUTLX_970(...)  PPUTLX_241(PPUTLX_241(PPUTLX_242(PPUTLX_242(__VA_ARGS__))))
#define PPUTLX_969(...)  PPUTLX_241(PPUTLX_241(PPUTLX_241(PPUTLX_242(__VA_ARGS__))))
#define PPUTLX_968(...)  PPUTLX_241(PPUTLX_241(PPUTLX_241(PPUTLX_241(__VA_ARGS__))))
#define PPUTLX_967(...)  PPUTLX_240(PPUTLX_241(PPUTLX_241(PPUTLX_241(__VA_ARGS__))))
#define PPUTLX_966(...)  PPUTLX_240(PPUTLX_240(PPUTLX_241(PPUTLX_241(__VA_ARGS__))))
#define PPUTLX_965(...)  PPUTLX_240(PPUTLX_240(PPUTLX_240(PPUTLX_241(__VA_ARGS__))))
#define PPUTLX_964(...)  PPUTLX_240(PPUTLX_240(PPUTLX_240(PPUTLX_240(__VA_ARGS__))))
#define PPUTLX_963(...)  PPUTLX_239(PPUTLX_240(PPUTLX_240(PPUTLX_240(__VA_ARGS__))))
#define PPUTLX_962(...)  PPUTLX_239(PPUTLX_239(PPUTLX_240(PPUTLX_240(__VA_ARGS__))))
#define PPUTLX_961(...)  PPUTLX_239(PPUTLX_239(PPUTLX_239(PPUTLX_240(__VA_ARGS__))))
#define PPUTLX_960(...)  PPUTLX_239(PPUTLX_239(PPUTLX_239(PPUTLX_239(__VA_ARGS__))))
#define PPUTLX_959(...)  PPUTLX_238(PPUTLX_239(PPUTLX_239(PPUTLX_239(__VA_ARGS__))))
#define PPUTLX_958(...)  PPUTLX_238(PPUTLX_238(PPUTLX_239(PPUTLX_239(__VA_ARGS__))))
#define PPUTLX_957(...)  PPUTLX_238(PPUTLX_238(PPUTLX_238(PPUTLX_239(__VA_ARGS__))))
#define PPUTLX_956(...)  PPUTLX_238(PPUTLX_238(PPUTLX_238(PPUTLX_238(__VA_ARGS__))))
#define PPUTLX_955(...)  PPUTLX_237(PPUTLX_238(PPUTLX_238(PPUTLX_238(__VA_ARGS__))))
#define PPUTLX_954(...)  PPUTLX_237(PPUTLX_237(PPUTLX_238(PPUTLX_238(__VA_ARGS__))))
#define PPUTLX_953(...)  PPUTLX_237(PPUTLX_237(PPUTLX_237(PPUTLX_238(__VA_ARGS__))))
#define PPUTLX_952(...)  PPUTLX_237(PPUTLX_237(PPUTLX_237(PPUTLX_237(__VA_ARGS__))))
#define PPUTLX_951(...)  PPUTLX_236(PPUTLX_237(PPUTLX_237(PPUTLX_237(__VA_ARGS__))))
#define PPUTLX_950(...)  PPUTLX_236(PPUTLX_236(PPUTLX_237(PPUTLX_237(__VA_ARGS__))))
#define PPUTLX_949(...)  PPUTLX_236(PPUTLX_236(PPUTLX_236(PPUTLX_237(__VA_ARGS__))))
#define PPUTLX_948(...)  PPUTLX_236(PPUTLX_236(PPUTLX_236(PPUTLX_236(__VA_ARGS__))))
#define PPUTLX_947(...)  PPUTLX_235(PPUTLX_236(PPUTLX_236(PPUTLX_236(__VA_ARGS__))))
#define PPUTLX_946(...)  PPUTLX_235(PPUTLX_235(PPUTLX_236(PPUTLX_236(__VA_ARGS__))))
#define PPUTLX_945(...)  PPUTLX_235(PPUTLX_235(PPUTLX_235(PPUTLX_236(__VA_ARGS__))))
#define PPUTLX_944(...)  PPUTLX_235(PPUTLX_235(PPUTLX_235(PPUTLX_235(__VA_ARGS__))))
#define PPUTLX_943(...)  PPUTLX_234(PPUTLX_235(PPUTLX_235(PPUTLX_235(__VA_ARGS__))))
#define PPUTLX_942(...)  PPUTLX_234(PPUTLX_234(PPUTLX_235(PPUTLX_235(__VA_ARGS__))))
#define PPUTLX_941(...)  PPUTLX_234(PPUTLX_234(PPUTLX_234(PPUTLX_235(__VA_ARGS__))))
#define PPUTLX_940(...)  PPUTLX_234(PPUTLX_234(PPUTLX_234(PPUTLX_234(__VA_ARGS__))))
#define PPUTLX_939(...)  PPUTLX_233(PPUTLX_234(PPUTLX_234(PPUTLX_234(__VA_ARGS__))))
#define PPUTLX_938(...)  PPUTLX_233(PPUTLX_233(PPUTLX_234(PPUTLX_234(__VA_ARGS__))))
#define PPUTLX_937(...)  PPUTLX_233(PPUTLX_233(PPUTLX_233(PPUTLX_234(__VA_ARGS__))))
#define PPUTLX_936(...)  PPUTLX_233(PPUTLX_233(PPUTLX_233(PPUTLX_233(__VA_ARGS__))))
#define PPUTLX_935(...)  PPUTLX_232(PPUTLX_233(PPUTLX_233(PPUTLX_233(__VA_ARGS__))))
#define PPUTLX_934(...)  PPUTLX_232(PPUTLX_232(PPUTLX_233(PPUTLX_233(__VA_ARGS__))))
#define PPUTLX_933(...)  PPUTLX_232(PPUTLX_232(PPUTLX_232(PPUTLX_233(__VA_ARGS__))))
#define PPUTLX_932(...)  PPUTLX_232(PPUTLX_232(PPUTLX_232(PPUTLX_232(__VA_ARGS__))))
#define PPUTLX_931(...)  PPUTLX_231(PPUTLX_232(PPUTLX_232(PPUTLX_232(__VA_ARGS__))))
#define PPUTLX_930(...)  PPUTLX_231(PPUTLX_231(PPUTLX_232(PPUTLX_232(__VA_ARGS__))))
#define PPUTLX_929(...)  PPUTLX_231(PPUTLX_231(PPUTLX_231(PPUTLX_232(__VA_ARGS__))))
#define PPUTLX_928(...)  PPUTLX_231(PPUTLX_231(PPUTLX_231(PPUTLX_231(__VA_ARGS__))))
#define PPUTLX_927(...)  PPUTLX_230(PPUTLX_231(PPUTLX_231(PPUTLX_231(__VA_ARGS__))))
#define PPUTLX_926(...)  PPUTLX_230(PPUTLX_230(PPUTLX_231(PPUTLX_231(__VA_ARGS__))))
#define PPUTLX_925(...)  PPUTLX_230(PPUTLX_230(PPUTLX_230(PPUTLX_231(__VA_ARGS__))))
#define PPUTLX_924(...)  PPUTLX_230(PPUTLX_230(PPUTLX_230(PPUTLX_230(__VA_ARGS__))))
#define PPUTLX_923(...)  PPUTLX_229(PPUTLX_230(PPUTLX_230(PPUTLX_230(__VA_ARGS__))))
#define PPUTLX_922(...)  PPUTLX_229(PPUTLX_229(PPUTLX_230(PPUTLX_230(__VA_ARGS__))))
#define PPUTLX_921(...)  PPUTLX_229(PPUTLX_229(PPUTLX_229(PPUTLX_230(__VA_ARGS__))))
#define PPUTLX_920(...)  PPUTLX_229(PPUTLX_229(PPUTLX_229(PPUTLX_229(__VA_ARGS__))))
#define PPUTLX_919(...)  PPUTLX_228(PPUTLX_229(PPUTLX_229(PPUTLX_229(__VA_ARGS__))))
#define PPUTLX_918(...)  PPUTLX_228(PPUTLX_228(PPUTLX_229(PPUTLX_229(__VA_ARGS__))))
#define PPUTLX_917(...)  PPUTLX_228(PPUTLX_228(PPUTLX_228(PPUTLX_229(__VA_ARGS__))))
#define PPUTLX_916(...)  PPUTLX_228(PPUTLX_228(PPUTLX_228(PPUTLX_228(__VA_ARGS__))))
#define PPUTLX_915(...)  PPUTLX_227(PPUTLX_228(PPUTLX_228(PPUTLX_228(__VA_ARGS__))))
#define PPUTLX_914(...)  PPUTLX_227(PPUTLX_227(PPUTLX_228(PPUTLX_228(__VA_ARGS__))))
#define PPUTLX_913(...)  PPUTLX_227(PPUTLX_227(PPUTLX_227(PPUTLX_228(__VA_ARGS__))))
#define PPUTLX_912(...)  PPUTLX_227(PPUTLX_227(PPUTLX_227(PPUTLX_227(__VA_ARGS__))))
#define PPUTLX_911(...)  PPUTLX_226(PPUTLX_227(PPUTLX_227(PPUTLX_227(__VA_ARGS__))))
#define PPUTLX_910(...)  PPUTLX_226(PPUTLX_226(PPUTLX_227(PPUTLX_227(__VA_ARGS__))))
#define PPUTLX_909(...)  PPUTLX_226(PPUTLX_226(PPUTLX_226(PPUTLX_227(__VA_ARGS__))))
#define PPUTLX_908(...)  PPUTLX_226(PPUTLX_226(PPUTLX_226(PPUTLX_226(__VA_ARGS__))))
#define PPUTLX_907(...)  PPUTLX_225(PPUTLX_226(PPUTLX_226(PPUTLX_226(__VA_ARGS__))))
#define PPUTLX_906(...)  PPUTLX_225(PPUTLX_225(PPUTLX_226(PPUTLX_226(__VA_ARGS__))))
#define PPUTLX_905(...)  PPUTLX_225(PPUTLX_225(PPUTLX_225(PPUTLX_226(__VA_ARGS__))))
#define PPUTLX_904(...)  PPUTLX_225(PPUTLX_225(PPUTLX_225(PPUTLX_225(__VA_ARGS__))))
#define PPUTLX_903(...)  PPUTLX_224(PPUTLX_225(PPUTLX_225(PPUTLX_225(__VA_ARGS__))))
#define PPUTLX_902(...)  PPUTLX_224(PPUTLX_224(PPUTLX_225(PPUTLX_225(__VA_ARGS__))))
#define PPUTLX_901(...)  PPUTLX_224(PPUTLX_224(PPUTLX_224(PPUTLX_225(__VA_ARGS__))))
#define PPUTLX_900(...)  PPUTLX_224(PPUTLX_224(PPUTLX_224(PPUTLX_224(__VA_ARGS__))))
#define PPUTLX_899(...)  PPUTLX_223(PPUTLX_224(PPUTLX_224(PPUTLX_224(__VA_ARGS__))))
#define PPUTLX_898(...)  PPUTLX_223(PPUTLX_223(PPUTLX_224(PPUTLX_224(__VA_ARGS__))))
#define PPUTLX_897(...)  PPUTLX_223(PPUTLX_223(PPUTLX_223(PPUTLX_224(__VA_ARGS__))))
#define PPUTLX_896(...)  PPUTLX_223(PPUTLX_223(PPUTLX_223(PPUTLX_223(__VA_ARGS__))))
#define PPUTLX_895(...)  PPUTLX_222(PPUTLX_223(PPUTLX_223(PPUTLX_223(__VA_ARGS__))))
#define PPUTLX_894(...)  PPUTLX_222(PPUTLX_222(PPUTLX_223(PPUTLX_223(__VA_ARGS__))))
#define PPUTLX_893(...)  PPUTLX_222(PPUTLX_222(PPUTLX_222(PPUTLX_223(__VA_ARGS__))))
#define PPUTLX_892(...)  PPUTLX_222(PPUTLX_222(PPUTLX_222(PPUTLX_222(__VA_ARGS__))))
#define PPUTLX_891(...)  PPUTLX_221(PPUTLX_222(PPUTLX_222(PPUTLX_222(__VA_ARGS__))))
#define PPUTLX_890(...)  PPUTLX_221(PPUTLX_221(PPUTLX_222(PPUTLX_222(__VA_ARGS__))))
#define PPUTLX_889(...)  PPUTLX_221(PPUTLX_221(PPUTLX_221(PPUTLX_222(__VA_ARGS__))))
#define PPUTLX_888(...)  PPUTLX_221(PPUTLX_221(PPUTLX_221(PPUTLX_221(__VA_ARGS__))))
#define PPUTLX_887(...)  PPUTLX_220(PPUTLX_221(PPUTLX_221(PPUTLX_221(__VA_ARGS__))))
#define PPUTLX_886(...)  PPUTLX_220(PPUTLX_220(PPUTLX_221(PPUTLX_221(__VA_ARGS__))))
#define PPUTLX_885(...)  PPUTLX_220(PPUTLX_220(PPUTLX_220(PPUTLX_221(__VA_ARGS__))))
#define PPUTLX_884(...)  PPUTLX_220(PPUTLX_220(PPUTLX_220(PPUTLX_220(__VA_ARGS__))))
#define PPUTLX_883(...)  PPUTLX_219(PPUTLX_220(PPUTLX_220(PPUTLX_220(__VA_ARGS__))))
#define PPUTLX_882(...)  PPUTLX_219(PPUTLX_219(PPUTLX_220(PPUTLX_220(__VA_ARGS__))))
#define PPUTLX_881(...)  PPUTLX_219(PPUTLX_219(PPUTLX_219(PPUTLX_220(__VA_ARGS__))))
#define PPUTLX_880(...)  PPUTLX_219(PPUTLX_219(PPUTLX_219(PPUTLX_219(__VA_ARGS__))))
#define PPUTLX_879(...)  PPUTLX_218(PPUTLX_219(PPUTLX_219(PPUTLX_219(__VA_ARGS__))))
#define PPUTLX_878(...)  PPUTLX_218(PPUTLX_218(PPUTLX_219(PPUTLX_219(__VA_ARGS__))))
#define PPUTLX_877(...)  PPUTLX_218(PPUTLX_218(PPUTLX_218(PPUTLX_219(__VA_ARGS__))))
#define PPUTLX_876(...)  PPUTLX_218(PPUTLX_218(PPUTLX_218(PPUTLX_218(__VA_ARGS__))))
#define PPUTLX_875(...)  PPUTLX_217(PPUTLX_218(PPUTLX_218(PPUTLX_218(__VA_ARGS__))))
#define PPUTLX_874(...)  PPUTLX_217(PPUTLX_217(PPUTLX_218(PPUTLX_218(__VA_ARGS__))))
#define PPUTLX_873(...)  PPUTLX_217(PPUTLX_217(PPUTLX_217(PPUTLX_218(__VA_ARGS__))))
#define PPUTLX_872(...)  PPUTLX_217(PPUTLX_217(PPUTLX_217(PPUTLX_217(__VA_ARGS__))))
#define PPUTLX_871(...)  PPUTLX_216(PPUTLX_217(PPUTLX_217(PPUTLX_217(__VA_ARGS__))))
#define PPUTLX_870(...)  PPUTLX_216(PPUTLX_216(PPUTLX_217(PPUTLX_217(__VA_ARGS__))))
#define PPUTLX_869(...)  PPUTLX_216(PPUTLX_216(PPUTLX_216(PPUTLX_217(__VA_ARGS__))))
#define PPUTLX_868(...)  PPUTLX_216(PPUTLX_216(PPUTLX_216(PPUTLX_216(__VA_ARGS__))))
#define PPUTLX_867(...)  PPUTLX_215(PPUTLX_216(PPUTLX_216(PPUTLX_216(__VA_ARGS__))))
#define PPUTLX_866(...)  PPUTLX_215(PPUTLX_215(PPUTLX_216(PPUTLX_216(__VA_ARGS__))))
#define PPUTLX_865(...)  PPUTLX_215(PPUTLX_215(PPUTLX_215(PPUTLX_216(__VA_ARGS__))))
#define PPUTLX_864(...)  PPUTLX_215(PPUTLX_215(PPUTLX_215(PPUTLX_215(__VA_ARGS__))))
#define PPUTLX_863(...)  PPUTLX_214(PPUTLX_215(PPUTLX_215(PPUTLX_215(__VA_ARGS__))))
#define PPUTLX_862(...)  PPUTLX_214(PPUTLX_214(PPUTLX_215(PPUTLX_215(__VA_ARGS__))))
#define PPUTLX_861(...)  PPUTLX_214(PPUTLX_214(PPUTLX_214(PPUTLX_215(__VA_ARGS__))))
#define PPUTLX_860(...)  PPUTLX_214(PPUTLX_214(PPUTLX_214(PPUTLX_214(__VA_ARGS__))))
#define PPUTLX_859(...)  PPUTLX_213(PPUTLX_214(PPUTLX_214(PPUTLX_214(__VA_ARGS__))))
#define PPUTLX_858(...)  PPUTLX_213(PPUTLX_213(PPUTLX_214(PPUTLX_214(__VA_ARGS__))))
#define PPUTLX_857(...)  PPUTLX_213(PPUTLX_213(PPUTLX_213(PPUTLX_214(__VA_ARGS__))))
#define PPUTLX_856(...)  PPUTLX_213(PPUTLX_213(PPUTLX_213(PPUTLX_213(__VA_ARGS__))))
#define PPUTLX_855(...)  PPUTLX_212(PPUTLX_213(PPUTLX_213(PPUTLX_213(__VA_ARGS__))))
#define PPUTLX_854(...)  PPUTLX_212(PPUTLX_212(PPUTLX_213(PPUTLX_213(__VA_ARGS__))))
#define PPUTLX_853(...)  PPUTLX_212(PPUTLX_212(PPUTLX_212(PPUTLX_213(__VA_ARGS__))))
#define PPUTLX_852(...)  PPUTLX_212(PPUTLX_212(PPUTLX_212(PPUTLX_212(__VA_ARGS__))))
#define PPUTLX_851(...)  PPUTLX_211(PPUTLX_212(PPUTLX_212(PPUTLX_212(__VA_ARGS__))))
#define PPUTLX_850(...)  PPUTLX_211(PPUTLX_211(PPUTLX_212(PPUTLX_212(__VA_ARGS__))))
#define PPUTLX_849(...)  PPUTLX_211(PPUTLX_211(PPUTLX_211(PPUTLX_212(__VA_ARGS__))))
#define PPUTLX_848(...)  PPUTLX_211(PPUTLX_211(PPUTLX_211(PPUTLX_211(__VA_ARGS__))))
#define PPUTLX_847(...)  PPUTLX_210(PPUTLX_211(PPUTLX_211(PPUTLX_211(__VA_ARGS__))))
#define PPUTLX_846(...)  PPUTLX_210(PPUTLX_210(PPUTLX_211(PPUTLX_211(__VA_ARGS__))))
#define PPUTLX_845(...)  PPUTLX_210(PPUTLX_210(PPUTLX_210(PPUTLX_211(__VA_ARGS__))))
#define PPUTLX_844(...)  PPUTLX_210(PPUTLX_210(PPUTLX_210(PPUTLX_210(__VA_ARGS__))))
#define PPUTLX_843(...)  PPUTLX_209(PPUTLX_210(PPUTLX_210(PPUTLX_210(__VA_ARGS__))))
#define PPUTLX_842(...)  PPUTLX_209(PPUTLX_209(PPUTLX_210(PPUTLX_210(__VA_ARGS__))))
#define PPUTLX_841(...)  PPUTLX_209(PPUTLX_209(PPUTLX_209(PPUTLX_210(__VA_ARGS__))))
#define PPUTLX_840(...)  PPUTLX_209(PPUTLX_209(PPUTLX_209(PPUTLX_209(__VA_ARGS__))))
#define PPUTLX_839(...)  PPUTLX_208(PPUTLX_209(PPUTLX_209(PPUTLX_209(__VA_ARGS__))))
#define PPUTLX_838(...)  PPUTLX_208(PPUTLX_208(PPUTLX_209(PPUTLX_209(__VA_ARGS__))))
#define PPUTLX_837(...)  PPUTLX_208(PPUTLX_208(PPUTLX_208(PPUTLX_209(__VA_ARGS__))))
#define PPUTLX_836(...)  PPUTLX_208(PPUTLX_208(PPUTLX_208(PPUTLX_208(__VA_ARGS__))))
#define PPUTLX_835(...)  PPUTLX_207(PPUTLX_208(PPUTLX_208(PPUTLX_208(__VA_ARGS__))))
#define PPUTLX_834(...)  PPUTLX_207(PPUTLX_207(PPUTLX_208(PPUTLX_208(__VA_ARGS__))))
#define PPUTLX_833(...)  PPUTLX_207(PPUTLX_207(PPUTLX_207(PPUTLX_208(__VA_ARGS__))))
#define PPUTLX_832(...)  PPUTLX_207(PPUTLX_207(PPUTLX_207(PPUTLX_207(__VA_ARGS__))))
#define PPUTLX_831(...)  PPUTLX_206(PPUTLX_207(PPUTLX_207(PPUTLX_207(__VA_ARGS__))))
#define PPUTLX_830(...)  PPUTLX_206(PPUTLX_206(PPUTLX_207(PPUTLX_207(__VA_ARGS__))))
#define PPUTLX_829(...)  PPUTLX_206(PPUTLX_206(PPUTLX_206(PPUTLX_207(__VA_ARGS__))))
#define PPUTLX_828(...)  PPUTLX_206(PPUTLX_206(PPUTLX_206(PPUTLX_206(__VA_ARGS__))))
#define PPUTLX_827(...)  PPUTLX_205(PPUTLX_206(PPUTLX_206(PPUTLX_206(__VA_ARGS__))))
#define PPUTLX_826(...)  PPUTLX_205(PPUTLX_205(PPUTLX_206(PPUTLX_206(__VA_ARGS__))))
#define PPUTLX_825(...)  PPUTLX_205(PPUTLX_205(PPUTLX_205(PPUTLX_206(__VA_ARGS__))))
#define PPUTLX_824(...)  PPUTLX_205(PPUTLX_205(PPUTLX_205(PPUTLX_205(__VA_ARGS__))))
#define PPUTLX_823(...)  PPUTLX_204(PPUTLX_205(PPUTLX_205(PPUTLX_205(__VA_ARGS__))))
#define PPUTLX_822(...)  PPUTLX_204(PPUTLX_204(PPUTLX_205(PPUTLX_205(__VA_ARGS__))))
#define PPUTLX_821(...)  PPUTLX_204(PPUTLX_204(PPUTLX_204(PPUTLX_205(__VA_ARGS__))))
#define PPUTLX_820(...)  PPUTLX_204(PPUTLX_204(PPUTLX_204(PPUTLX_204(__VA_ARGS__))))
#define PPUTLX_819(...)  PPUTLX_203(PPUTLX_204(PPUTLX_204(PPUTLX_204(__VA_ARGS__))))
#define PPUTLX_818(...)  PPUTLX_203(PPUTLX_203(PPUTLX_204(PPUTLX_204(__VA_ARGS__))))
#define PPUTLX_817(...)  PPUTLX_203(PPUTLX_203(PPUTLX_203(PPUTLX_204(__VA_ARGS__))))
#define PPUTLX_816(...)  PPUTLX_203(PPUTLX_203(PPUTLX_203(PPUTLX_203(__VA_ARGS__))))
#define PPUTLX_815(...)  PPUTLX_202(PPUTLX_203(PPUTLX_203(PPUTLX_203(__VA_ARGS__))))
#define PPUTLX_814(...)  PPUTLX_202(PPUTLX_202(PPUTLX_203(PPUTLX_203(__VA_ARGS__))))
#define PPUTLX_813(...)  PPUTLX_202(PPUTLX_202(PPUTLX_202(PPUTLX_203(__VA_ARGS__))))
#define PPUTLX_812(...)  PPUTLX_202(PPUTLX_202(PPUTLX_202(PPUTLX_202(__VA_ARGS__))))
#define PPUTLX_811(...)  PPUTLX_201(PPUTLX_202(PPUTLX_202(PPUTLX_202(__VA_ARGS__))))
#define PPUTLX_810(...)  PPUTLX_201(PPUTLX_201(PPUTLX_202(PPUTLX_202(__VA_ARGS__))))
#define PPUTLX_809(...)  PPUTLX_201(PPUTLX_201(PPUTLX_201(PPUTLX_202(__VA_ARGS__))))
#define PPUTLX_808(...)  PPUTLX_201(PPUTLX_201(PPUTLX_201(PPUTLX_201(__VA_ARGS__))))
#define PPUTLX_807(...)  PPUTLX_200(PPUTLX_201(PPUTLX_201(PPUTLX_201(__VA_ARGS__))))
#define PPUTLX_806(...)  PPUTLX_200(PPUTLX_200(PPUTLX_201(PPUTLX_201(__VA_ARGS__))))
#define PPUTLX_805(...)  PPUTLX_200(PPUTLX_200(PPUTLX_200(PPUTLX_201(__VA_ARGS__))))
#define PPUTLX_804(...)  PPUTLX_200(PPUTLX_200(PPUTLX_200(PPUTLX_200(__VA_ARGS__))))
#define PPUTLX_803(...)  PPUTLX_199(PPUTLX_200(PPUTLX_200(PPUTLX_200(__VA_ARGS__))))
#define PPUTLX_802(...)  PPUTLX_199(PPUTLX_199(PPUTLX_200(PPUTLX_200(__VA_ARGS__))))
#define PPUTLX_801(...)  PPUTLX_199(PPUTLX_199(PPUTLX_199(PPUTLX_200(__VA_ARGS__))))
#define PPUTLX_800(...)  PPUTLX_199(PPUTLX_199(PPUTLX_199(PPUTLX_199(__VA_ARGS__))))
#define PPUTLX_799(...)  PPUTLX_198(PPUTLX_199(PPUTLX_199(PPUTLX_199(__VA_ARGS__))))
#define PPUTLX_798(...)  PPUTLX_198(PPUTLX_198(PPUTLX_199(PPUTLX_199(__VA_ARGS__))))
#define PPUTLX_797(...)  PPUTLX_198(PPUTLX_198(PPUTLX_198(PPUTLX_199(__VA_ARGS__))))
#define PPUTLX_796(...)  PPUTLX_198(PPUTLX_198(PPUTLX_198(PPUTLX_198(__VA_ARGS__))))
#define PPUTLX_795(...)  PPUTLX_197(PPUTLX_198(PPUTLX_198(PPUTLX_198(__VA_ARGS__))))
#define PPUTLX_794(...)  PPUTLX_197(PPUTLX_197(PPUTLX_198(PPUTLX_198(__VA_ARGS__))))
#define PPUTLX_793(...)  PPUTLX_197(PPUTLX_197(PPUTLX_197(PPUTLX_198(__VA_ARGS__))))
#define PPUTLX_792(...)  PPUTLX_197(PPUTLX_197(PPUTLX_197(PPUTLX_197(__VA_ARGS__))))
#define PPUTLX_791(...)  PPUTLX_196(PPUTLX_197(PPUTLX_197(PPUTLX_197(__VA_ARGS__))))
#define PPUTLX_790(...)  PPUTLX_196(PPUTLX_196(PPUTLX_197(PPUTLX_197(__VA_ARGS__))))
#define PPUTLX_789(...)  PPUTLX_196(PPUTLX_196(PPUTLX_196(PPUTLX_197(__VA_ARGS__))))
#define PPUTLX_788(...)  PPUTLX_196(PPUTLX_196(PPUTLX_196(PPUTLX_196(__VA_ARGS__))))
#define PPUTLX_787(...)  PPUTLX_195(PPUTLX_196(PPUTLX_196(PPUTLX_196(__VA_ARGS__))))
#define PPUTLX_786(...)  PPUTLX_195(PPUTLX_195(PPUTLX_196(PPUTLX_196(__VA_ARGS__))))
#define PPUTLX_785(...)  PPUTLX_195(PPUTLX_195(PPUTLX_195(PPUTLX_196(__VA_ARGS__))))
#define PPUTLX_784(...)  PPUTLX_195(PPUTLX_195(PPUTLX_195(PPUTLX_195(__VA_ARGS__))))
#define PPUTLX_783(...)  PPUTLX_194(PPUTLX_195(PPUTLX_195(PPUTLX_195(__VA_ARGS__))))
#define PPUTLX_782(...)  PPUTLX_194(PPUTLX_194(PPUTLX_195(PPUTLX_195(__VA_ARGS__))))
#define PPUTLX_781(...)  PPUTLX_194(PPUTLX_194(PPUTLX_194(PPUTLX_195(__VA_ARGS__))))
#define PPUTLX_780(...)  PPUTLX_194(PPUTLX_194(PPUTLX_194(PPUTLX_194(__VA_ARGS__))))
#define PPUTLX_779(...)  PPUTLX_193(PPUTLX_194(PPUTLX_194(PPUTLX_194(__VA_ARGS__))))
#define PPUTLX_778(...)  PPUTLX_193(PPUTLX_193(PPUTLX_194(PPUTLX_194(__VA_ARGS__))))
#define PPUTLX_777(...)  PPUTLX_193(PPUTLX_193(PPUTLX_193(PPUTLX_194(__VA_ARGS__))))
#define PPUTLX_776(...)  PPUTLX_193(PPUTLX_193(PPUTLX_193(PPUTLX_193(__VA_ARGS__))))
#define PPUTLX_775(...)  PPUTLX_192(PPUTLX_193(PPUTLX_193(PPUTLX_193(__VA_ARGS__))))
#define PPUTLX_774(...)  PPUTLX_192(PPUTLX_192(PPUTLX_193(PPUTLX_193(__VA_ARGS__))))
#define PPUTLX_773(...)  PPUTLX_192(PPUTLX_192(PPUTLX_192(PPUTLX_193(__VA_ARGS__))))
#define PPUTLX_772(...)  PPUTLX_192(PPUTLX_192(PPUTLX_192(PPUTLX_192(__VA_ARGS__))))
#define PPUTLX_771(...)  PPUTLX_191(PPUTLX_192(PPUTLX_192(PPUTLX_192(__VA_ARGS__))))
#define PPUTLX_770(...)  PPUTLX_191(PPUTLX_191(PPUTLX_192(PPUTLX_192(__VA_ARGS__))))
#define PPUTLX_769(...)  PPUTLX_191(PPUTLX_191(PPUTLX_191(PPUTLX_192(__VA_ARGS__))))
#define PPUTLX_768(...)  PPUTLX_191(PPUTLX_191(PPUTLX_191(PPUTLX_191(__VA_ARGS__))))
#define PPUTLX_767(...)  PPUTLX_190(PPUTLX_191(PPUTLX_191(PPUTLX_191(__VA_ARGS__))))
#define PPUTLX_766(...)  PPUTLX_190(PPUTLX_190(PPUTLX_191(PPUTLX_191(__VA_ARGS__))))
#define PPUTLX_765(...)  PPUTLX_190(PPUTLX_190(PPUTLX_190(PPUTLX_191(__VA_ARGS__))))
#define PPUTLX_764(...)  PPUTLX_190(PPUTLX_190(PPUTLX_190(PPUTLX_190(__VA_ARGS__))))
#define PPUTLX_763(...)  PPUTLX_189(PPUTLX_190(PPUTLX_190(PPUTLX_190(__VA_ARGS__))))
#define PPUTLX_762(...)  PPUTLX_189(PPUTLX_189(PPUTLX_190(PPUTLX_190(__VA_ARGS__))))
#define PPUTLX_761(...)  PPUTLX_189(PPUTLX_189(PPUTLX_189(PPUTLX_190(__VA_ARGS__))))
#define PPUTLX_760(...)  PPUTLX_189(PPUTLX_189(PPUTLX_189(PPUTLX_189(__VA_ARGS__))))
#define PPUTLX_759(...)  PPUTLX_188(PPUTLX_189(PPUTLX_189(PPUTLX_189(__VA_ARGS__))))
#define PPUTLX_758(...)  PPUTLX_188(PPUTLX_188(PPUTLX_189(PPUTLX_189(__VA_ARGS__))))
#define PPUTLX_757(...)  PPUTLX_188(PPUTLX_188(PPUTLX_188(PPUTLX_189(__VA_ARGS__))))
#define PPUTLX_756(...)  PPUTLX_188(PPUTLX_188(PPUTLX_188(PPUTLX_188(__VA_ARGS__))))
#define PPUTLX_755(...)  PPUTLX_187(PPUTLX_188(PPUTLX_188(PPUTLX_188(__VA_ARGS__))))
#define PPUTLX_754(...)  PPUTLX_187(PPUTLX_187(PPUTLX_188(PPUTLX_188(__VA_ARGS__))))
#define PPUTLX_753(...)  PPUTLX_187(PPUTLX_187(PPUTLX_187(PPUTLX_188(__VA_ARGS__))))
#define PPUTLX_752(...)  PPUTLX_187(PPUTLX_187(PPUTLX_187(PPUTLX_187(__VA_ARGS__))))
#define PPUTLX_751(...)  PPUTLX_186(PPUTLX_187(PPUTLX_187(PPUTLX_187(__VA_ARGS__))))
#define PPUTLX_750(...)  PPUTLX_186(PPUTLX_186(PPUTLX_187(PPUTLX_187(__VA_ARGS__))))
#define PPUTLX_749(...)  PPUTLX_186(PPUTLX_186(PPUTLX_186(PPUTLX_187(__VA_ARGS__))))
#define PPUTLX_748(...)  PPUTLX_186(PPUTLX_186(PPUTLX_186(PPUTLX_186(__VA_ARGS__))))
#define PPUTLX_747(...)  PPUTLX_185(PPUTLX_186(PPUTLX_186(PPUTLX_186(__VA_ARGS__))))
#define PPUTLX_746(...)  PPUTLX_185(PPUTLX_185(PPUTLX_186(PPUTLX_186(__VA_ARGS__))))
#define PPUTLX_745(...)  PPUTLX_185(PPUTLX_185(PPUTLX_185(PPUTLX_186(__VA_ARGS__))))
#define PPUTLX_744(...)  PPUTLX_185(PPUTLX_185(PPUTLX_185(PPUTLX_185(__VA_ARGS__))))
#define PPUTLX_743(...)  PPUTLX_184(PPUTLX_185(PPUTLX_185(PPUTLX_185(__VA_ARGS__))))
#define PPUTLX_742(...)  PPUTLX_184(PPUTLX_184(PPUTLX_185(PPUTLX_185(__VA_ARGS__))))
#define PPUTLX_741(...)  PPUTLX_184(PPUTLX_184(PPUTLX_184(PPUTLX_185(__VA_ARGS__))))
#define PPUTLX_740(...)  PPUTLX_184(PPUTLX_184(PPUTLX_184(PPUTLX_184(__VA_ARGS__))))
#define PPUTLX_739(...)  PPUTLX_183(PPUTLX_184(PPUTLX_184(PPUTLX_184(__VA_ARGS__))))
#define PPUTLX_738(...)  PPUTLX_183(PPUTLX_183(PPUTLX_184(PPUTLX_184(__VA_ARGS__))))
#define PPUTLX_737(...)  PPUTLX_183(PPUTLX_183(PPUTLX_183(PPUTLX_184(__VA_ARGS__))))
#define PPUTLX_736(...)  PPUTLX_183(PPUTLX_183(PPUTLX_183(PPUTLX_183(__VA_ARGS__))))
#define PPUTLX_735(...)  PPUTLX_182(PPUTLX_183(PPUTLX_183(PPUTLX_183(__VA_ARGS__))))
#define PPUTLX_734(...)  PPUTLX_182(PPUTLX_182(PPUTLX_183(PPUTLX_183(__VA_ARGS__))))
#define PPUTLX_733(...)  PPUTLX_182(PPUTLX_182(PPUTLX_182(PPUTLX_183(__VA_ARGS__))))
#define PPUTLX_732(...)  PPUTLX_182(PPUTLX_182(PPUTLX_182(PPUTLX_182(__VA_ARGS__))))
#define PPUTLX_731(...)  PPUTLX_181(PPUTLX_182(PPUTLX_182(PPUTLX_182(__VA_ARGS__))))
#define PPUTLX_730(...)  PPUTLX_181(PPUTLX_181(PPUTLX_182(PPUTLX_182(__VA_ARGS__))))
#define PPUTLX_729(...)  PPUTLX_181(PPUTLX_181(PPUTLX_181(PPUTLX_182(__VA_ARGS__))))
#define PPUTLX_728(...)  PPUTLX_181(PPUTLX_181(PPUTLX_181(PPUTLX_181(__VA_ARGS__))))
#define PPUTLX_727(...)  PPUTLX_180(PPUTLX_181(PPUTLX_181(PPUTLX_181(__VA_ARGS__))))
#define PPUTLX_726(...)  PPUTLX_180(PPUTLX_180(PPUTLX_181(PPUTLX_181(__VA_ARGS__))))
#define PPUTLX_725(...)  PPUTLX_180(PPUTLX_180(PPUTLX_180(PPUTLX_181(__VA_ARGS__))))
#define PPUTLX_724(...)  PPUTLX_180(PPUTLX_180(PPUTLX_180(PPUTLX_180(__VA_ARGS__))))
#define PPUTLX_723(...)  PPUTLX_179(PPUTLX_180(PPUTLX_180(PPUTLX_180(__VA_ARGS__))))
#define PPUTLX_722(...)  PPUTLX_179(PPUTLX_179(PPUTLX_180(PPUTLX_180(__VA_ARGS__))))
#define PPUTLX_721(...)  PPUTLX_179(PPUTLX_179(PPUTLX_179(PPUTLX_180(__VA_ARGS__))))
#define PPUTLX_720(...)  PPUTLX_179(PPUTLX_179(PPUTLX_179(PPUTLX_179(__VA_ARGS__))))
#define PPUTLX_719(...)  PPUTLX_178(PPUTLX_179(PPUTLX_179(PPUTLX_179(__VA_ARGS__))))
#define PPUTLX_718(...)  PPUTLX_178(PPUTLX_178(PPUTLX_179(PPUTLX_179(__VA_ARGS__))))
#define PPUTLX_717(...)  PPUTLX_178(PPUTLX_178(PPUTLX_178(PPUTLX_179(__VA_ARGS__))))
#define PPUTLX_716(...)  PPUTLX_178(PPUTLX_178(PPUTLX_178(PPUTLX_178(__VA_ARGS__))))
#define PPUTLX_715(...)  PPUTLX_177(PPUTLX_178(PPUTLX_178(PPUTLX_178(__VA_ARGS__))))
#define PPUTLX_714(...)  PPUTLX_177(PPUTLX_177(PPUTLX_178(PPUTLX_178(__VA_ARGS__))))
#define PPUTLX_713(...)  PPUTLX_177(PPUTLX_177(PPUTLX_177(PPUTLX_178(__VA_ARGS__))))
#define PPUTLX_712(...)  PPUTLX_177(PPUTLX_177(PPUTLX_177(PPUTLX_177(__VA_ARGS__))))
#define PPUTLX_711(...)  PPUTLX_176(PPUTLX_177(PPUTLX_177(PPUTLX_177(__VA_ARGS__))))
#define PPUTLX_710(...)  PPUTLX_176(PPUTLX_176(PPUTLX_177(PPUTLX_177(__VA_ARGS__))))
#define PPUTLX_709(...)  PPUTLX_176(PPUTLX_176(PPUTLX_176(PPUTLX_177(__VA_ARGS__))))
#define PPUTLX_708(...)  PPUTLX_176(PPUTLX_176(PPUTLX_176(PPUTLX_176(__VA_ARGS__))))
#define PPUTLX_707(...)  PPUTLX_175(PPUTLX_176(PPUTLX_176(PPUTLX_176(__VA_ARGS__))))
#define PPUTLX_706(...)  PPUTLX_175(PPUTLX_175(PPUTLX_176(PPUTLX_176(__VA_ARGS__))))
#define PPUTLX_705(...)  PPUTLX_175(PPUTLX_175(PPUTLX_175(PPUTLX_176(__VA_ARGS__))))
#define PPUTLX_704(...)  PPUTLX_175(PPUTLX_175(PPUTLX_175(PPUTLX_175(__VA_ARGS__))))
#define PPUTLX_703(...)  PPUTLX_174(PPUTLX_175(PPUTLX_175(PPUTLX_175(__VA_ARGS__))))
#define PPUTLX_702(...)  PPUTLX_174(PPUTLX_174(PPUTLX_175(PPUTLX_175(__VA_ARGS__))))
#define PPUTLX_701(...)  PPUTLX_174(PPUTLX_174(PPUTLX_174(PPUTLX_175(__VA_ARGS__))))
#define PPUTLX_700(...)  PPUTLX_174(PPUTLX_174(PPUTLX_174(PPUTLX_174(__VA_ARGS__))))
#define PPUTLX_699(...)  PPUTLX_173(PPUTLX_174(PPUTLX_174(PPUTLX_174(__VA_ARGS__))))
#define PPUTLX_698(...)  PPUTLX_173(PPUTLX_173(PPUTLX_174(PPUTLX_174(__VA_ARGS__))))
#define PPUTLX_697(...)  PPUTLX_173(PPUTLX_173(PPUTLX_173(PPUTLX_174(__VA_ARGS__))))
#define PPUTLX_696(...)  PPUTLX_173(PPUTLX_173(PPUTLX_173(PPUTLX_173(__VA_ARGS__))))
#define PPUTLX_695(...)  PPUTLX_172(PPUTLX_173(PPUTLX_173(PPUTLX_173(__VA_ARGS__))))
#define PPUTLX_694(...)  PPUTLX_172(PPUTLX_172(PPUTLX_173(PPUTLX_173(__VA_ARGS__))))
#define PPUTLX_693(...)  PPUTLX_172(PPUTLX_172(PPUTLX_172(PPUTLX_173(__VA_ARGS__))))
#define PPUTLX_692(...)  PPUTLX_172(PPUTLX_172(PPUTLX_172(PPUTLX_172(__VA_ARGS__))))
#define PPUTLX_691(...)  PPUTLX_171(PPUTLX_172(PPUTLX_172(PPUTLX_172(__VA_ARGS__))))
#define PPUTLX_690(...)  PPUTLX_171(PPUTLX_171(PPUTLX_172(PPUTLX_172(__VA_ARGS__))))
#define PPUTLX_689(...)  PPUTLX_171(PPUTLX_171(PPUTLX_171(PPUTLX_172(__VA_ARGS__))))
#define PPUTLX_688(...)  PPUTLX_171(PPUTLX_171(PPUTLX_171(PPUTLX_171(__VA_ARGS__))))
#define PPUTLX_687(...)  PPUTLX_170(PPUTLX_171(PPUTLX_171(PPUTLX_171(__VA_ARGS__))))
#define PPUTLX_686(...)  PPUTLX_170(PPUTLX_170(PPUTLX_171(PPUTLX_171(__VA_ARGS__))))
#define PPUTLX_685(...)  PPUTLX_170(PPUTLX_170(PPUTLX_170(PPUTLX_171(__VA_ARGS__))))
#define PPUTLX_684(...)  PPUTLX_170(PPUTLX_170(PPUTLX_170(PPUTLX_170(__VA_ARGS__))))
#define PPUTLX_683(...)  PPUTLX_169(PPUTLX_170(PPUTLX_170(PPUTLX_170(__VA_ARGS__))))
#define PPUTLX_682(...)  PPUTLX_169(PPUTLX_169(PPUTLX_170(PPUTLX_170(__VA_ARGS__))))
#define PPUTLX_681(...)  PPUTLX_169(PPUTLX_169(PPUTLX_169(PPUTLX_170(__VA_ARGS__))))
#define PPUTLX_680(...)  PPUTLX_169(PPUTLX_169(PPUTLX_169(PPUTLX_169(__VA_ARGS__))))
#define PPUTLX_679(...)  PPUTLX_168(PPUTLX_169(PPUTLX_169(PPUTLX_169(__VA_ARGS__))))
#define PPUTLX_678(...)  PPUTLX_168(PPUTLX_168(PPUTLX_169(PPUTLX_169(__VA_ARGS__))))
#define PPUTLX_677(...)  PPUTLX_168(PPUTLX_168(PPUTLX_168(PPUTLX_169(__VA_ARGS__))))
#define PPUTLX_676(...)  PPUTLX_168(PPUTLX_168(PPUTLX_168(PPUTLX_168(__VA_ARGS__))))
#define PPUTLX_675(...)  PPUTLX_167(PPUTLX_168(PPUTLX_168(PPUTLX_168(__VA_ARGS__))))
#define PPUTLX_674(...)  PPUTLX_167(PPUTLX_167(PPUTLX_168(PPUTLX_168(__VA_ARGS__))))
#define PPUTLX_673(...)  PPUTLX_167(PPUTLX_167(PPUTLX_167(PPUTLX_168(__VA_ARGS__))))
#define PPUTLX_672(...)  PPUTLX_167(PPUTLX_167(PPUTLX_167(PPUTLX_167(__VA_ARGS__))))
#define PPUTLX_671(...)  PPUTLX_166(PPUTLX_167(PPUTLX_167(PPUTLX_167(__VA_ARGS__))))
#define PPUTLX_670(...)  PPUTLX_166(PPUTLX_166(PPUTLX_167(PPUTLX_167(__VA_ARGS__))))
#define PPUTLX_669(...)  PPUTLX_166(PPUTLX_166(PPUTLX_166(PPUTLX_167(__VA_ARGS__))))
#define PPUTLX_668(...)  PPUTLX_166(PPUTLX_166(PPUTLX_166(PPUTLX_166(__VA_ARGS__))))
#define PPUTLX_667(...)  PPUTLX_165(PPUTLX_166(PPUTLX_166(PPUTLX_166(__VA_ARGS__))))
#define PPUTLX_666(...)  PPUTLX_165(PPUTLX_165(PPUTLX_166(PPUTLX_166(__VA_ARGS__))))
#define PPUTLX_665(...)  PPUTLX_165(PPUTLX_165(PPUTLX_165(PPUTLX_166(__VA_ARGS__))))
#define PPUTLX_664(...)  PPUTLX_165(PPUTLX_165(PPUTLX_165(PPUTLX_165(__VA_ARGS__))))
#define PPUTLX_663(...)  PPUTLX_164(PPUTLX_165(PPUTLX_165(PPUTLX_165(__VA_ARGS__))))
#define PPUTLX_662(...)  PPUTLX_164(PPUTLX_164(PPUTLX_165(PPUTLX_165(__VA_ARGS__))))
#define PPUTLX_661(...)  PPUTLX_164(PPUTLX_164(PPUTLX_164(PPUTLX_165(__VA_ARGS__))))
#define PPUTLX_660(...)  PPUTLX_164(PPUTLX_164(PPUTLX_164(PPUTLX_164(__VA_ARGS__))))
#define PPUTLX_659(...)  PPUTLX_163(PPUTLX_164(PPUTLX_164(PPUTLX_164(__VA_ARGS__))))
#define PPUTLX_658(...)  PPUTLX_163(PPUTLX_163(PPUTLX_164(PPUTLX_164(__VA_ARGS__))))
#define PPUTLX_657(...)  PPUTLX_163(PPUTLX_163(PPUTLX_163(PPUTLX_164(__VA_ARGS__))))
#define PPUTLX_656(...)  PPUTLX_163(PPUTLX_163(PPUTLX_163(PPUTLX_163(__VA_ARGS__))))
#define PPUTLX_655(...)  PPUTLX_162(PPUTLX_163(PPUTLX_163(PPUTLX_163(__VA_ARGS__))))
#define PPUTLX_654(...)  PPUTLX_162(PPUTLX_162(PPUTLX_163(PPUTLX_163(__VA_ARGS__))))
#define PPUTLX_653(...)  PPUTLX_162(PPUTLX_162(PPUTLX_162(PPUTLX_163(__VA_ARGS__))))
#define PPUTLX_652(...)  PPUTLX_162(PPUTLX_162(PPUTLX_162(PPUTLX_162(__VA_ARGS__))))
#define PPUTLX_651(...)  PPUTLX_161(PPUTLX_162(PPUTLX_162(PPUTLX_162(__VA_ARGS__))))
#define PPUTLX_650(...)  PPUTLX_161(PPUTLX_161(PPUTLX_162(PPUTLX_162(__VA_ARGS__))))
#define PPUTLX_649(...)  PPUTLX_161(PPUTLX_161(PPUTLX_161(PPUTLX_162(__VA_ARGS__))))
#define PPUTLX_648(...)  PPUTLX_161(PPUTLX_161(PPUTLX_161(PPUTLX_161(__VA_ARGS__))))
#define PPUTLX_647(...)  PPUTLX_160(PPUTLX_161(PPUTLX_161(PPUTLX_161(__VA_ARGS__))))
#define PPUTLX_646(...)  PPUTLX_160(PPUTLX_160(PPUTLX_161(PPUTLX_161(__VA_ARGS__))))
#define PPUTLX_645(...)  PPUTLX_160(PPUTLX_160(PPUTLX_160(PPUTLX_161(__VA_ARGS__))))
#define PPUTLX_644(...)  PPUTLX_160(PPUTLX_160(PPUTLX_160(PPUTLX_160(__VA_ARGS__))))
#define PPUTLX_643(...)  PPUTLX_159(PPUTLX_160(PPUTLX_160(PPUTLX_160(__VA_ARGS__))))
#define PPUTLX_642(...)  PPUTLX_159(PPUTLX_159(PPUTLX_160(PPUTLX_160(__VA_ARGS__))))
#define PPUTLX_641(...)  PPUTLX_159(PPUTLX_159(PPUTLX_159(PPUTLX_160(__VA_ARGS__))))
#define PPUTLX_640(...)  PPUTLX_159(PPUTLX_159(PPUTLX_159(PPUTLX_159(__VA_ARGS__))))
#define PPUTLX_639(...)  PPUTLX_158(PPUTLX_159(PPUTLX_159(PPUTLX_159(__VA_ARGS__))))
#define PPUTLX_638(...)  PPUTLX_158(PPUTLX_158(PPUTLX_159(PPUTLX_159(__VA_ARGS__))))
#define PPUTLX_637(...)  PPUTLX_158(PPUTLX_158(PPUTLX_158(PPUTLX_159(__VA_ARGS__))))
#define PPUTLX_636(...)  PPUTLX_158(PPUTLX_158(PPUTLX_158(PPUTLX_158(__VA_ARGS__))))
#define PPUTLX_635(...)  PPUTLX_157(PPUTLX_158(PPUTLX_158(PPUTLX_158(__VA_ARGS__))))
#define PPUTLX_634(...)  PPUTLX_157(PPUTLX_157(PPUTLX_158(PPUTLX_158(__VA_ARGS__))))
#define PPUTLX_633(...)  PPUTLX_157(PPUTLX_157(PPUTLX_157(PPUTLX_158(__VA_ARGS__))))
#define PPUTLX_632(...)  PPUTLX_157(PPUTLX_157(PPUTLX_157(PPUTLX_157(__VA_ARGS__))))
#define PPUTLX_631(...)  PPUTLX_156(PPUTLX_157(PPUTLX_157(PPUTLX_157(__VA_ARGS__))))
#define PPUTLX_630(...)  PPUTLX_156(PPUTLX_156(PPUTLX_157(PPUTLX_157(__VA_ARGS__))))
#define PPUTLX_629(...)  PPUTLX_156(PPUTLX_156(PPUTLX_156(PPUTLX_157(__VA_ARGS__))))
#define PPUTLX_628(...)  PPUTLX_156(PPUTLX_156(PPUTLX_156(PPUTLX_156(__VA_ARGS__))))
#define PPUTLX_627(...)  PPUTLX_155(PPUTLX_156(PPUTLX_156(PPUTLX_156(__VA_ARGS__))))
#define PPUTLX_626(...)  PPUTLX_155(PPUTLX_155(PPUTLX_156(PPUTLX_156(__VA_ARGS__))))
#define PPUTLX_625(...)  PPUTLX_155(PPUTLX_155(PPUTLX_155(PPUTLX_156(__VA_ARGS__))))
#define PPUTLX_624(...)  PPUTLX_155(PPUTLX_155(PPUTLX_155(PPUTLX_155(__VA_ARGS__))))
#define PPUTLX_623(...)  PPUTLX_154(PPUTLX_155(PPUTLX_155(PPUTLX_155(__VA_ARGS__))))
#define PPUTLX_622(...)  PPUTLX_154(PPUTLX_154(PPUTLX_155(PPUTLX_155(__VA_ARGS__))))
#define PPUTLX_621(...)  PPUTLX_154(PPUTLX_154(PPUTLX_154(PPUTLX_155(__VA_ARGS__))))
#define PPUTLX_620(...)  PPUTLX_154(PPUTLX_154(PPUTLX_154(PPUTLX_154(__VA_ARGS__))))
#define PPUTLX_619(...)  PPUTLX_153(PPUTLX_154(PPUTLX_154(PPUTLX_154(__VA_ARGS__))))
#define PPUTLX_618(...)  PPUTLX_153(PPUTLX_153(PPUTLX_154(PPUTLX_154(__VA_ARGS__))))
#define PPUTLX_617(...)  PPUTLX_153(PPUTLX_153(PPUTLX_153(PPUTLX_154(__VA_ARGS__))))
#define PPUTLX_616(...)  PPUTLX_153(PPUTLX_153(PPUTLX_153(PPUTLX_153(__VA_ARGS__))))
#define PPUTLX_615(...)  PPUTLX_152(PPUTLX_153(PPUTLX_153(PPUTLX_153(__VA_ARGS__))))
#define PPUTLX_614(...)  PPUTLX_152(PPUTLX_152(PPUTLX_153(PPUTLX_153(__VA_ARGS__))))
#define PPUTLX_613(...)  PPUTLX_152(PPUTLX_152(PPUTLX_152(PPUTLX_153(__VA_ARGS__))))
#define PPUTLX_612(...)  PPUTLX_152(PPUTLX_152(PPUTLX_152(PPUTLX_152(__VA_ARGS__))))
#define PPUTLX_611(...)  PPUTLX_151(PPUTLX_152(PPUTLX_152(PPUTLX_152(__VA_ARGS__))))
#define PPUTLX_610(...)  PPUTLX_151(PPUTLX_151(PPUTLX_152(PPUTLX_152(__VA_ARGS__))))
#define PPUTLX_609(...)  PPUTLX_151(PPUTLX_151(PPUTLX_151(PPUTLX_152(__VA_ARGS__))))
#define PPUTLX_608(...)  PPUTLX_151(PPUTLX_151(PPUTLX_151(PPUTLX_151(__VA_ARGS__))))
#define PPUTLX_607(...)  PPUTLX_150(PPUTLX_151(PPUTLX_151(PPUTLX_151(__VA_ARGS__))))
#define PPUTLX_606(...)  PPUTLX_150(PPUTLX_150(PPUTLX_151(PPUTLX_151(__VA_ARGS__))))
#define PPUTLX_605(...)  PPUTLX_150(PPUTLX_150(PPUTLX_150(PPUTLX_151(__VA_ARGS__))))
#define PPUTLX_604(...)  PPUTLX_150(PPUTLX_150(PPUTLX_150(PPUTLX_150(__VA_ARGS__))))
#define PPUTLX_603(...)  PPUTLX_149(PPUTLX_150(PPUTLX_150(PPUTLX_150(__VA_ARGS__))))
#define PPUTLX_602(...)  PPUTLX_149(PPUTLX_149(PPUTLX_150(PPUTLX_150(__VA_ARGS__))))
#define PPUTLX_601(...)  PPUTLX_149(PPUTLX_149(PPUTLX_149(PPUTLX_150(__VA_ARGS__))))
#define PPUTLX_600(...)  PPUTLX_149(PPUTLX_149(PPUTLX_149(PPUTLX_149(__VA_ARGS__))))
#define PPUTLX_599(...)  PPUTLX_148(PPUTLX_149(PPUTLX_149(PPUTLX_149(__VA_ARGS__))))
#define PPUTLX_598(...)  PPUTLX_148(PPUTLX_148(PPUTLX_149(PPUTLX_149(__VA_ARGS__))))
#define PPUTLX_597(...)  PPUTLX_148(PPUTLX_148(PPUTLX_148(PPUTLX_149(__VA_ARGS__))))
#define PPUTLX_596(...)  PPUTLX_148(PPUTLX_148(PPUTLX_148(PPUTLX_148(__VA_ARGS__))))
#define PPUTLX_595(...)  PPUTLX_147(PPUTLX_148(PPUTLX_148(PPUTLX_148(__VA_ARGS__))))
#define PPUTLX_594(...)  PPUTLX_147(PPUTLX_147(PPUTLX_148(PPUTLX_148(__VA_ARGS__))))
#define PPUTLX_593(...)  PPUTLX_147(PPUTLX_147(PPUTLX_147(PPUTLX_148(__VA_ARGS__))))
#define PPUTLX_592(...)  PPUTLX_147(PPUTLX_147(PPUTLX_147(PPUTLX_147(__VA_ARGS__))))
#define PPUTLX_591(...)  PPUTLX_146(PPUTLX_147(PPUTLX_147(PPUTLX_147(__VA_ARGS__))))
#define PPUTLX_590(...)  PPUTLX_146(PPUTLX_146(PPUTLX_147(PPUTLX_147(__VA_ARGS__))))
#define PPUTLX_589(...)  PPUTLX_146(PPUTLX_146(PPUTLX_146(PPUTLX_147(__VA_ARGS__))))
#define PPUTLX_588(...)  PPUTLX_146(PPUTLX_146(PPUTLX_146(PPUTLX_146(__VA_ARGS__))))
#define PPUTLX_587(...)  PPUTLX_145(PPUTLX_146(PPUTLX_146(PPUTLX_146(__VA_ARGS__))))
#define PPUTLX_586(...)  PPUTLX_145(PPUTLX_145(PPUTLX_146(PPUTLX_146(__VA_ARGS__))))
#define PPUTLX_585(...)  PPUTLX_145(PPUTLX_145(PPUTLX_145(PPUTLX_146(__VA_ARGS__))))
#define PPUTLX_584(...)  PPUTLX_145(PPUTLX_145(PPUTLX_145(PPUTLX_145(__VA_ARGS__))))
#define PPUTLX_583(...)  PPUTLX_144(PPUTLX_145(PPUTLX_145(PPUTLX_145(__VA_ARGS__))))
#define PPUTLX_582(...)  PPUTLX_144(PPUTLX_144(PPUTLX_145(PPUTLX_145(__VA_ARGS__))))
#define PPUTLX_581(...)  PPUTLX_144(PPUTLX_144(PPUTLX_144(PPUTLX_145(__VA_ARGS__))))
#define PPUTLX_580(...)  PPUTLX_144(PPUTLX_144(PPUTLX_144(PPUTLX_144(__VA_ARGS__))))
#define PPUTLX_579(...)  PPUTLX_143(PPUTLX_144(PPUTLX_144(PPUTLX_144(__VA_ARGS__))))
#define PPUTLX_578(...)  PPUTLX_143(PPUTLX_143(PPUTLX_144(PPUTLX_144(__VA_ARGS__))))
#define PPUTLX_577(...)  PPUTLX_143(PPUTLX_143(PPUTLX_143(PPUTLX_144(__VA_ARGS__))))
#define PPUTLX_576(...)  PPUTLX_143(PPUTLX_143(PPUTLX_143(PPUTLX_143(__VA_ARGS__))))
#define PPUTLX_575(...)  PPUTLX_142(PPUTLX_143(PPUTLX_143(PPUTLX_143(__VA_ARGS__))))
#define PPUTLX_574(...)  PPUTLX_142(PPUTLX_142(PPUTLX_143(PPUTLX_143(__VA_ARGS__))))
#define PPUTLX_573(...)  PPUTLX_142(PPUTLX_142(PPUTLX_142(PPUTLX_143(__VA_ARGS__))))
#define PPUTLX_572(...)  PPUTLX_142(PPUTLX_142(PPUTLX_142(PPUTLX_142(__VA_ARGS__))))
#define PPUTLX_571(...)  PPUTLX_141(PPUTLX_142(PPUTLX_142(PPUTLX_142(__VA_ARGS__))))
#define PPUTLX_570(...)  PPUTLX_141(PPUTLX_141(PPUTLX_142(PPUTLX_142(__VA_ARGS__))))
#define PPUTLX_569(...)  PPUTLX_141(PPUTLX_141(PPUTLX_141(PPUTLX_142(__VA_ARGS__))))
#define PPUTLX_568(...)  PPUTLX_141(PPUTLX_141(PPUTLX_141(PPUTLX_141(__VA_ARGS__))))
#define PPUTLX_567(...)  PPUTLX_140(PPUTLX_141(PPUTLX_141(PPUTLX_141(__VA_ARGS__))))
#define PPUTLX_566(...)  PPUTLX_140(PPUTLX_140(PPUTLX_141(PPUTLX_141(__VA_ARGS__))))
#define PPUTLX_565(...)  PPUTLX_140(PPUTLX_140(PPUTLX_140(PPUTLX_141(__VA_ARGS__))))
#define PPUTLX_564(...)  PPUTLX_140(PPUTLX_140(PPUTLX_140(PPUTLX_140(__VA_ARGS__))))
#define PPUTLX_563(...)  PPUTLX_139(PPUTLX_140(PPUTLX_140(PPUTLX_140(__VA_ARGS__))))
#define PPUTLX_562(...)  PPUTLX_139(PPUTLX_139(PPUTLX_140(PPUTLX_140(__VA_ARGS__))))
#define PPUTLX_561(...)  PPUTLX_139(PPUTLX_139(PPUTLX_139(PPUTLX_140(__VA_ARGS__))))
#define PPUTLX_560(...)  PPUTLX_139(PPUTLX_139(PPUTLX_139(PPUTLX_139(__VA_ARGS__))))
#define PPUTLX_559(...)  PPUTLX_138(PPUTLX_139(PPUTLX_139(PPUTLX_139(__VA_ARGS__))))
#define PPUTLX_558(...)  PPUTLX_138(PPUTLX_138(PPUTLX_139(PPUTLX_139(__VA_ARGS__))))
#define PPUTLX_557(...)  PPUTLX_138(PPUTLX_138(PPUTLX_138(PPUTLX_139(__VA_ARGS__))))
#define PPUTLX_556(...)  PPUTLX_138(PPUTLX_138(PPUTLX_138(PPUTLX_138(__VA_ARGS__))))
#define PPUTLX_555(...)  PPUTLX_137(PPUTLX_138(PPUTLX_138(PPUTLX_138(__VA_ARGS__))))
#define PPUTLX_554(...)  PPUTLX_137(PPUTLX_137(PPUTLX_138(PPUTLX_138(__VA_ARGS__))))
#define PPUTLX_553(...)  PPUTLX_137(PPUTLX_137(PPUTLX_137(PPUTLX_138(__VA_ARGS__))))
#define PPUTLX_552(...)  PPUTLX_137(PPUTLX_137(PPUTLX_137(PPUTLX_137(__VA_ARGS__))))
#define PPUTLX_551(...)  PPUTLX_136(PPUTLX_137(PPUTLX_137(PPUTLX_137(__VA_ARGS__))))
#define PPUTLX_550(...)  PPUTLX_136(PPUTLX_136(PPUTLX_137(PPUTLX_137(__VA_ARGS__))))
#define PPUTLX_549(...)  PPUTLX_136(PPUTLX_136(PPUTLX_136(PPUTLX_137(__VA_ARGS__))))
#define PPUTLX_548(...)  PPUTLX_136(PPUTLX_136(PPUTLX_136(PPUTLX_136(__VA_ARGS__))))
#define PPUTLX_547(...)  PPUTLX_135(PPUTLX_136(PPUTLX_136(PPUTLX_136(__VA_ARGS__))))
#define PPUTLX_546(...)  PPUTLX_135(PPUTLX_135(PPUTLX_136(PPUTLX_136(__VA_ARGS__))))
#define PPUTLX_545(...)  PPUTLX_135(PPUTLX_135(PPUTLX_135(PPUTLX_136(__VA_ARGS__))))
#define PPUTLX_544(...)  PPUTLX_135(PPUTLX_135(PPUTLX_135(PPUTLX_135(__VA_ARGS__))))
#define PPUTLX_543(...)  PPUTLX_134(PPUTLX_135(PPUTLX_135(PPUTLX_135(__VA_ARGS__))))
#define PPUTLX_542(...)  PPUTLX_134(PPUTLX_134(PPUTLX_135(PPUTLX_135(__VA_ARGS__))))
#define PPUTLX_541(...)  PPUTLX_134(PPUTLX_134(PPUTLX_134(PPUTLX_135(__VA_ARGS__))))
#define PPUTLX_540(...)  PPUTLX_134(PPUTLX_134(PPUTLX_134(PPUTLX_134(__VA_ARGS__))))
#define PPUTLX_539(...)  PPUTLX_133(PPUTLX_134(PPUTLX_134(PPUTLX_134(__VA_ARGS__))))
#define PPUTLX_538(...)  PPUTLX_133(PPUTLX_133(PPUTLX_134(PPUTLX_134(__VA_ARGS__))))
#define PPUTLX_537(...)  PPUTLX_133(PPUTLX_133(PPUTLX_133(PPUTLX_134(__VA_ARGS__))))
#define PPUTLX_536(...)  PPUTLX_133(PPUTLX_133(PPUTLX_133(PPUTLX_133(__VA_ARGS__))))
#define PPUTLX_535(...)  PPUTLX_132(PPUTLX_133(PPUTLX_133(PPUTLX_133(__VA_ARGS__))))
#define PPUTLX_534(...)  PPUTLX_132(PPUTLX_132(PPUTLX_133(PPUTLX_133(__VA_ARGS__))))
#define PPUTLX_533(...)  PPUTLX_132(PPUTLX_132(PPUTLX_132(PPUTLX_133(__VA_ARGS__))))
#define PPUTLX_532(...)  PPUTLX_132(PPUTLX_132(PPUTLX_132(PPUTLX_132(__VA_ARGS__))))
#define PPUTLX_531(...)  PPUTLX_131(PPUTLX_132(PPUTLX_132(PPUTLX_132(__VA_ARGS__))))
#define PPUTLX_530(...)  PPUTLX_131(PPUTLX_131(PPUTLX_132(PPUTLX_132(__VA_ARGS__))))
#define PPUTLX_529(...)  PPUTLX_131(PPUTLX_131(PPUTLX_131(PPUTLX_132(__VA_ARGS__))))
#define PPUTLX_528(...)  PPUTLX_131(PPUTLX_131(PPUTLX_131(PPUTLX_131(__VA_ARGS__))))
#define PPUTLX_527(...)  PPUTLX_130(PPUTLX_131(PPUTLX_131(PPUTLX_131(__VA_ARGS__))))
#define PPUTLX_526(...)  PPUTLX_130(PPUTLX_130(PPUTLX_131(PPUTLX_131(__VA_ARGS__))))
#define PPUTLX_525(...)  PPUTLX_130(PPUTLX_130(PPUTLX_130(PPUTLX_131(__VA_ARGS__))))
#define PPUTLX_524(...)  PPUTLX_130(PPUTLX_130(PPUTLX_130(PPUTLX_130(__VA_ARGS__))))
#define PPUTLX_523(...)  PPUTLX_129(PPUTLX_130(PPUTLX_130(PPUTLX_130(__VA_ARGS__))))
#define PPUTLX_522(...)  PPUTLX_129(PPUTLX_129(PPUTLX_130(PPUTLX_130(__VA_ARGS__))))
#define PPUTLX_521(...)  PPUTLX_129(PPUTLX_129(PPUTLX_129(PPUTLX_130(__VA_ARGS__))))
#define PPUTLX_520(...)  PPUTLX_129(PPUTLX_129(PPUTLX_129(PPUTLX_129(__VA_ARGS__))))
#define PPUTLX_519(...)  PPUTLX_128(PPUTLX_129(PPUTLX_129(PPUTLX_129(__VA_ARGS__))))
#define PPUTLX_518(...)  PPUTLX_128(PPUTLX_128(PPUTLX_129(PPUTLX_129(__VA_ARGS__))))
#define PPUTLX_517(...)  PPUTLX_128(PPUTLX_128(PPUTLX_128(PPUTLX_129(__VA_ARGS__))))
#define PPUTLX_516(...)  PPUTLX_128(PPUTLX_128(PPUTLX_128(PPUTLX_128(__VA_ARGS__))))
#define PPUTLX_515(...)  PPUTLX_127(PPUTLX_128(PPUTLX_128(PPUTLX_128(__VA_ARGS__))))
#define PPUTLX_514(...)  PPUTLX_127(PPUTLX_127(PPUTLX_128(PPUTLX_128(__VA_ARGS__))))
#define PPUTLX_513(...)  PPUTLX_127(PPUTLX_127(PPUTLX_127(PPUTLX_128(__VA_ARGS__))))
#define PPUTLX_512(...)  PPUTLX_127(PPUTLX_127(PPUTLX_127(PPUTLX_127(__VA_ARGS__))))
#define PPUTLX_511(...)  PPUTLX_126(PPUTLX_127(PPUTLX_127(PPUTLX_127(__VA_ARGS__))))
#define PPUTLX_510(...)  PPUTLX_126(PPUTLX_126(PPUTLX_127(PPUTLX_127(__VA_ARGS__))))
#define PPUTLX_509(...)  PPUTLX_126(PPUTLX_126(PPUTLX_126(PPUTLX_127(__VA_ARGS__))))
#define PPUTLX_508(...)  PPUTLX_126(PPUTLX_126(PPUTLX_126(PPUTLX_126(__VA_ARGS__))))
#define PPUTLX_507(...)  PPUTLX_125(PPUTLX_126(PPUTLX_126(PPUTLX_126(__VA_ARGS__))))
#define PPUTLX_506(...)  PPUTLX_125(PPUTLX_125(PPUTLX_126(PPUTLX_126(__VA_ARGS__))))
#define PPUTLX_505(...)  PPUTLX_125(PPUTLX_125(PPUTLX_125(PPUTLX_126(__VA_ARGS__))))
#define PPUTLX_504(...)  PPUTLX_125(PPUTLX_125(PPUTLX_125(PPUTLX_125(__VA_ARGS__))))
#define PPUTLX_503(...)  PPUTLX_124(PPUTLX_125(PPUTLX_125(PPUTLX_125(__VA_ARGS__))))
#define PPUTLX_502(...)  PPUTLX_124(PPUTLX_124(PPUTLX_125(PPUTLX_125(__VA_ARGS__))))
#define PPUTLX_501(...)  PPUTLX_124(PPUTLX_124(PPUTLX_124(PPUTLX_125(__VA_ARGS__))))
#define PPUTLX_500(...)  PPUTLX_124(PPUTLX_124(PPUTLX_124(PPUTLX_124(__VA_ARGS__))))
#define PPUTLX_499(...)  PPUTLX_123(PPUTLX_124(PPUTLX_124(PPUTLX_124(__VA_ARGS__))))
#define PPUTLX_498(...)  PPUTLX_123(PPUTLX_123(PPUTLX_124(PPUTLX_124(__VA_ARGS__))))
#define PPUTLX_497(...)  PPUTLX_123(PPUTLX_123(PPUTLX_123(PPUTLX_124(__VA_ARGS__))))
#define PPUTLX_496(...)  PPUTLX_123(PPUTLX_123(PPUTLX_123(PPUTLX_123(__VA_ARGS__))))
#define PPUTLX_495(...)  PPUTLX_122(PPUTLX_123(PPUTLX_123(PPUTLX_123(__VA_ARGS__))))
#define PPUTLX_494(...)  PPUTLX_122(PPUTLX_122(PPUTLX_123(PPUTLX_123(__VA_ARGS__))))
#define PPUTLX_493(...)  PPUTLX_122(PPUTLX_122(PPUTLX_122(PPUTLX_123(__VA_ARGS__))))
#define PPUTLX_492(...)  PPUTLX_122(PPUTLX_122(PPUTLX_122(PPUTLX_122(__VA_ARGS__))))
#define PPUTLX_491(...)  PPUTLX_121(PPUTLX_122(PPUTLX_122(PPUTLX_122(__VA_ARGS__))))
#define PPUTLX_490(...)  PPUTLX_121(PPUTLX_121(PPUTLX_122(PPUTLX_122(__VA_ARGS__))))
#define PPUTLX_489(...)  PPUTLX_121(PPUTLX_121(PPUTLX_121(PPUTLX_122(__VA_ARGS__))))
#define PPUTLX_488(...)  PPUTLX_121(PPUTLX_121(PPUTLX_121(PPUTLX_121(__VA_ARGS__))))
#define PPUTLX_487(...)  PPUTLX_120(PPUTLX_121(PPUTLX_121(PPUTLX_121(__VA_ARGS__))))
#define PPUTLX_486(...)  PPUTLX_120(PPUTLX_120(PPUTLX_121(PPUTLX_121(__VA_ARGS__))))
#define PPUTLX_485(...)  PPUTLX_120(PPUTLX_120(PPUTLX_120(PPUTLX_121(__VA_ARGS__))))
#define PPUTLX_484(...)  PPUTLX_120(PPUTLX_120(PPUTLX_120(PPUTLX_120(__VA_ARGS__))))
#define PPUTLX_483(...)  PPUTLX_119(PPUTLX_120(PPUTLX_120(PPUTLX_120(__VA_ARGS__))))
#define PPUTLX_482(...)  PPUTLX_119(PPUTLX_119(PPUTLX_120(PPUTLX_120(__VA_ARGS__))))
#define PPUTLX_481(...)  PPUTLX_119(PPUTLX_119(PPUTLX_119(PPUTLX_120(__VA_ARGS__))))
#define PPUTLX_480(...)  PPUTLX_119(PPUTLX_119(PPUTLX_119(PPUTLX_119(__VA_ARGS__))))
#define PPUTLX_479(...)  PPUTLX_118(PPUTLX_119(PPUTLX_119(PPUTLX_119(__VA_ARGS__))))
#define PPUTLX_478(...)  PPUTLX_118(PPUTLX_118(PPUTLX_119(PPUTLX_119(__VA_ARGS__))))
#define PPUTLX_477(...)  PPUTLX_118(PPUTLX_118(PPUTLX_118(PPUTLX_119(__VA_ARGS__))))
#define PPUTLX_476(...)  PPUTLX_118(PPUTLX_118(PPUTLX_118(PPUTLX_118(__VA_ARGS__))))
#define PPUTLX_475(...)  PPUTLX_117(PPUTLX_118(PPUTLX_118(PPUTLX_118(__VA_ARGS__))))
#define PPUTLX_474(...)  PPUTLX_117(PPUTLX_117(PPUTLX_118(PPUTLX_118(__VA_ARGS__))))
#define PPUTLX_473(...)  PPUTLX_117(PPUTLX_117(PPUTLX_117(PPUTLX_118(__VA_ARGS__))))
#define PPUTLX_472(...)  PPUTLX_117(PPUTLX_117(PPUTLX_117(PPUTLX_117(__VA_ARGS__))))
#define PPUTLX_471(...)  PPUTLX_116(PPUTLX_117(PPUTLX_117(PPUTLX_117(__VA_ARGS__))))
#define PPUTLX_470(...)  PPUTLX_116(PPUTLX_116(PPUTLX_117(PPUTLX_117(__VA_ARGS__))))
#define PPUTLX_469(...)  PPUTLX_116(PPUTLX_116(PPUTLX_116(PPUTLX_117(__VA_ARGS__))))
#define PPUTLX_468(...)  PPUTLX_116(PPUTLX_116(PPUTLX_116(PPUTLX_116(__VA_ARGS__))))
#define PPUTLX_467(...)  PPUTLX_115(PPUTLX_116(PPUTLX_116(PPUTLX_116(__VA_ARGS__))))
#define PPUTLX_466(...)  PPUTLX_115(PPUTLX_115(PPUTLX_116(PPUTLX_116(__VA_ARGS__))))
#define PPUTLX_465(...)  PPUTLX_115(PPUTLX_115(PPUTLX_115(PPUTLX_116(__VA_ARGS__))))
#define PPUTLX_464(...)  PPUTLX_115(PPUTLX_115(PPUTLX_115(PPUTLX_115(__VA_ARGS__))))
#define PPUTLX_463(...)  PPUTLX_114(PPUTLX_115(PPUTLX_115(PPUTLX_115(__VA_ARGS__))))
#define PPUTLX_462(...)  PPUTLX_114(PPUTLX_114(PPUTLX_115(PPUTLX_115(__VA_ARGS__))))
#define PPUTLX_461(...)  PPUTLX_114(PPUTLX_114(PPUTLX_114(PPUTLX_115(__VA_ARGS__))))
#define PPUTLX_460(...)  PPUTLX_114(PPUTLX_114(PPUTLX_114(PPUTLX_114(__VA_ARGS__))))
#define PPUTLX_459(...)  PPUTLX_113(PPUTLX_114(PPUTLX_114(PPUTLX_114(__VA_ARGS__))))
#define PPUTLX_458(...)  PPUTLX_113(PPUTLX_113(PPUTLX_114(PPUTLX_114(__VA_ARGS__))))
#define PPUTLX_457(...)  PPUTLX_113(PPUTLX_113(PPUTLX_113(PPUTLX_114(__VA_ARGS__))))
#define PPUTLX_456(...)  PPUTLX_113(PPUTLX_113(PPUTLX_113(PPUTLX_113(__VA_ARGS__))))
#define PPUTLX_455(...)  PPUTLX_112(PPUTLX_113(PPUTLX_113(PPUTLX_113(__VA_ARGS__))))
#define PPUTLX_454(...)  PPUTLX_112(PPUTLX_112(PPUTLX_113(PPUTLX_113(__VA_ARGS__))))
#define PPUTLX_453(...)  PPUTLX_112(PPUTLX_112(PPUTLX_112(PPUTLX_113(__VA_ARGS__))))
#define PPUTLX_452(...)  PPUTLX_112(PPUTLX_112(PPUTLX_112(PPUTLX_112(__VA_ARGS__))))
#define PPUTLX_451(...)  PPUTLX_111(PPUTLX_112(PPUTLX_112(PPUTLX_112(__VA_ARGS__))))
#define PPUTLX_450(...)  PPUTLX_111(PPUTLX_111(PPUTLX_112(PPUTLX_112(__VA_ARGS__))))
#define PPUTLX_449(...)  PPUTLX_111(PPUTLX_111(PPUTLX_111(PPUTLX_112(__VA_ARGS__))))
#define PPUTLX_448(...)  PPUTLX_111(PPUTLX_111(PPUTLX_111(PPUTLX_111(__VA_ARGS__))))
#define PPUTLX_447(...)  PPUTLX_110(PPUTLX_111(PPUTLX_111(PPUTLX_111(__VA_ARGS__))))
#define PPUTLX_446(...)  PPUTLX_110(PPUTLX_110(PPUTLX_111(PPUTLX_111(__VA_ARGS__))))
#define PPUTLX_445(...)  PPUTLX_110(PPUTLX_110(PPUTLX_110(PPUTLX_111(__VA_ARGS__))))
#define PPUTLX_444(...)  PPUTLX_110(PPUTLX_110(PPUTLX_110(PPUTLX_110(__VA_ARGS__))))
#define PPUTLX_443(...)  PPUTLX_109(PPUTLX_110(PPUTLX_110(PPUTLX_110(__VA_ARGS__))))
#define PPUTLX_442(...)  PPUTLX_109(PPUTLX_109(PPUTLX_110(PPUTLX_110(__VA_ARGS__))))
#define PPUTLX_441(...)  PPUTLX_109(PPUTLX_109(PPUTLX_109(PPUTLX_110(__VA_ARGS__))))
#define PPUTLX_440(...)  PPUTLX_109(PPUTLX_109(PPUTLX_109(PPUTLX_109(__VA_ARGS__))))
#define PPUTLX_439(...)  PPUTLX_108(PPUTLX_109(PPUTLX_109(PPUTLX_109(__VA_ARGS__))))
#define PPUTLX_438(...)  PPUTLX_108(PPUTLX_108(PPUTLX_109(PPUTLX_109(__VA_ARGS__))))
#define PPUTLX_437(...)  PPUTLX_108(PPUTLX_108(PPUTLX_108(PPUTLX_109(__VA_ARGS__))))
#define PPUTLX_436(...)  PPUTLX_108(PPUTLX_108(PPUTLX_108(PPUTLX_108(__VA_ARGS__))))
#define PPUTLX_435(...)  PPUTLX_107(PPUTLX_108(PPUTLX_108(PPUTLX_108(__VA_ARGS__))))
#define PPUTLX_434(...)  PPUTLX_107(PPUTLX_107(PPUTLX_108(PPUTLX_108(__VA_ARGS__))))
#define PPUTLX_433(...)  PPUTLX_107(PPUTLX_107(PPUTLX_107(PPUTLX_108(__VA_ARGS__))))
#define PPUTLX_432(...)  PPUTLX_107(PPUTLX_107(PPUTLX_107(PPUTLX_107(__VA_ARGS__))))
#define PPUTLX_431(...)  PPUTLX_106(PPUTLX_107(PPUTLX_107(PPUTLX_107(__VA_ARGS__))))
#define PPUTLX_430(...)  PPUTLX_106(PPUTLX_106(PPUTLX_107(PPUTLX_107(__VA_ARGS__))))
#define PPUTLX_429(...)  PPUTLX_106(PPUTLX_106(PPUTLX_106(PPUTLX_107(__VA_ARGS__))))
#define PPUTLX_428(...)  PPUTLX_106(PPUTLX_106(PPUTLX_106(PPUTLX_106(__VA_ARGS__))))
#define PPUTLX_427(...)  PPUTLX_105(PPUTLX_106(PPUTLX_106(PPUTLX_106(__VA_ARGS__))))
#define PPUTLX_426(...)  PPUTLX_105(PPUTLX_105(PPUTLX_106(PPUTLX_106(__VA_ARGS__))))
#define PPUTLX_425(...)  PPUTLX_105(PPUTLX_105(PPUTLX_105(PPUTLX_106(__VA_ARGS__))))
#define PPUTLX_424(...)  PPUTLX_105(PPUTLX_105(PPUTLX_105(PPUTLX_105(__VA_ARGS__))))
#define PPUTLX_423(...)  PPUTLX_104(PPUTLX_105(PPUTLX_105(PPUTLX_105(__VA_ARGS__))))
#define PPUTLX_422(...)  PPUTLX_104(PPUTLX_104(PPUTLX_105(PPUTLX_105(__VA_ARGS__))))
#define PPUTLX_421(...)  PPUTLX_104(PPUTLX_104(PPUTLX_104(PPUTLX_105(__VA_ARGS__))))
#define PPUTLX_420(...)  PPUTLX_104(PPUTLX_104(PPUTLX_104(PPUTLX_104(__VA_ARGS__))))
#define PPUTLX_419(...)  PPUTLX_103(PPUTLX_104(PPUTLX_104(PPUTLX_104(__VA_ARGS__))))
#define PPUTLX_418(...)  PPUTLX_103(PPUTLX_103(PPUTLX_104(PPUTLX_104(__VA_ARGS__))))
#define PPUTLX_417(...)  PPUTLX_103(PPUTLX_103(PPUTLX_103(PPUTLX_104(__VA_ARGS__))))
#define PPUTLX_416(...)  PPUTLX_103(PPUTLX_103(PPUTLX_103(PPUTLX_103(__VA_ARGS__))))
#define PPUTLX_415(...)  PPUTLX_102(PPUTLX_103(PPUTLX_103(PPUTLX_103(__VA_ARGS__))))
#define PPUTLX_414(...)  PPUTLX_102(PPUTLX_102(PPUTLX_103(PPUTLX_103(__VA_ARGS__))))
#define PPUTLX_413(...)  PPUTLX_102(PPUTLX_102(PPUTLX_102(PPUTLX_103(__VA_ARGS__))))
#define PPUTLX_412(...)  PPUTLX_102(PPUTLX_102(PPUTLX_102(PPUTLX_102(__VA_ARGS__))))
#define PPUTLX_411(...)  PPUTLX_101(PPUTLX_102(PPUTLX_102(PPUTLX_102(__VA_ARGS__))))
#define PPUTLX_410(...)  PPUTLX_101(PPUTLX_101(PPUTLX_102(PPUTLX_102(__VA_ARGS__))))
#define PPUTLX_409(...)  PPUTLX_101(PPUTLX_101(PPUTLX_101(PPUTLX_102(__VA_ARGS__))))
#define PPUTLX_408(...)  PPUTLX_101(PPUTLX_101(PPUTLX_101(PPUTLX_101(__VA_ARGS__))))
#define PPUTLX_407(...)  PPUTLX_100(PPUTLX_101(PPUTLX_101(PPUTLX_101(__VA_ARGS__))))
#define PPUTLX_406(...)  PPUTLX_100(PPUTLX_100(PPUTLX_101(PPUTLX_101(__VA_ARGS__))))
#define PPUTLX_405(...)  PPUTLX_100(PPUTLX_100(PPUTLX_100(PPUTLX_101(__VA_ARGS__))))
#define PPUTLX_404(...)  PPUTLX_100(PPUTLX_100(PPUTLX_100(PPUTLX_100(__VA_ARGS__))))
#define PPUTLX_403(...)  PPUTLX_99(PPUTLX_100(PPUTLX_100(PPUTLX_100(__VA_ARGS__))))
#define PPUTLX_402(...)  PPUTLX_99(PPUTLX_99(PPUTLX_100(PPUTLX_100(__VA_ARGS__))))
#define PPUTLX_401(...)  PPUTLX_99(PPUTLX_99(PPUTLX_99(PPUTLX_100(__VA_ARGS__))))
#define PPUTLX_400(...)  PPUTLX_99(PPUTLX_99(PPUTLX_99(PPUTLX_99(__VA_ARGS__))))
#define PPUTLX_399(...)  PPUTLX_98(PPUTLX_99(PPUTLX_99(PPUTLX_99(__VA_ARGS__))))
#define PPUTLX_398(...)  PPUTLX_98(PPUTLX_98(PPUTLX_99(PPUTLX_99(__VA_ARGS__))))
#define PPUTLX_397(...)  PPUTLX_98(PPUTLX_98(PPUTLX_98(PPUTLX_99(__VA_ARGS__))))
#define PPUTLX_396(...)  PPUTLX_98(PPUTLX_98(PPUTLX_98(PPUTLX_98(__VA_ARGS__))))
#define PPUTLX_395(...)  PPUTLX_97(PPUTLX_98(PPUTLX_98(PPUTLX_98(__VA_ARGS__))))
#define PPUTLX_394(...)  PPUTLX_97(PPUTLX_97(PPUTLX_98(PPUTLX_98(__VA_ARGS__))))
#define PPUTLX_393(...)  PPUTLX_97(PPUTLX_97(PPUTLX_97(PPUTLX_98(__VA_ARGS__))))
#define PPUTLX_392(...)  PPUTLX_97(PPUTLX_97(PPUTLX_97(PPUTLX_97(__VA_ARGS__))))
#define PPUTLX_391(...)  PPUTLX_96(PPUTLX_97(PPUTLX_97(PPUTLX_97(__VA_ARGS__))))
#define PPUTLX_390(...)  PPUTLX_96(PPUTLX_96(PPUTLX_97(PPUTLX_97(__VA_ARGS__))))
#define PPUTLX_389(...)  PPUTLX_96(PPUTLX_96(PPUTLX_96(PPUTLX_97(__VA_ARGS__))))
#define PPUTLX_388(...)  PPUTLX_96(PPUTLX_96(PPUTLX_96(PPUTLX_96(__VA_ARGS__))))
#define PPUTLX_387(...)  PPUTLX_95(PPUTLX_96(PPUTLX_96(PPUTLX_96(__VA_ARGS__))))
#define PPUTLX_386(...)  PPUTLX_95(PPUTLX_95(PPUTLX_96(PPUTLX_96(__VA_ARGS__))))
#define PPUTLX_385(...)  PPUTLX_95(PPUTLX_95(PPUTLX_95(PPUTLX_96(__VA_ARGS__))))
#define PPUTLX_384(...)  PPUTLX_95(PPUTLX_95(PPUTLX_95(PPUTLX_95(__VA_ARGS__))))
#define PPUTLX_383(...)  PPUTLX_94(PPUTLX_95(PPUTLX_95(PPUTLX_95(__VA_ARGS__))))
#define PPUTLX_382(...)  PPUTLX_94(PPUTLX_94(PPUTLX_95(PPUTLX_95(__VA_ARGS__))))
#define PPUTLX_381(...)  PPUTLX_94(PPUTLX_94(PPUTLX_94(PPUTLX_95(__VA_ARGS__))))
#define PPUTLX_380(...)  PPUTLX_94(PPUTLX_94(PPUTLX_94(PPUTLX_94(__VA_ARGS__))))
#define PPUTLX_379(...)  PPUTLX_93(PPUTLX_94(PPUTLX_94(PPUTLX_94(__VA_ARGS__))))
#define PPUTLX_378(...)  PPUTLX_93(PPUTLX_93(PPUTLX_94(PPUTLX_94(__VA_ARGS__))))
#define PPUTLX_377(...)  PPUTLX_93(PPUTLX_93(PPUTLX_93(PPUTLX_94(__VA_ARGS__))))
#define PPUTLX_376(...)  PPUTLX_93(PPUTLX_93(PPUTLX_93(PPUTLX_93(__VA_ARGS__))))
#define PPUTLX_375(...)  PPUTLX_92(PPUTLX_93(PPUTLX_93(PPUTLX_93(__VA_ARGS__))))
#define PPUTLX_374(...)  PPUTLX_92(PPUTLX_92(PPUTLX_93(PPUTLX_93(__VA_ARGS__))))
#define PPUTLX_373(...)  PPUTLX_92(PPUTLX_92(PPUTLX_92(PPUTLX_93(__VA_ARGS__))))
#define PPUTLX_372(...)  PPUTLX_92(PPUTLX_92(PPUTLX_92(PPUTLX_92(__VA_ARGS__))))
#define PPUTLX_371(...)  PPUTLX_91(PPUTLX_92(PPUTLX_92(PPUTLX_92(__VA_ARGS__))))
#define PPUTLX_370(...)  PPUTLX_91(PPUTLX_91(PPUTLX_92(PPUTLX_92(__VA_ARGS__))))
#define PPUTLX_369(...)  PPUTLX_91(PPUTLX_91(PPUTLX_91(PPUTLX_92(__VA_ARGS__))))
#define PPUTLX_368(...)  PPUTLX_91(PPUTLX_91(PPUTLX_91(PPUTLX_91(__VA_ARGS__))))
#define PPUTLX_367(...)  PPUTLX_90(PPUTLX_91(PPUTLX_91(PPUTLX_91(__VA_ARGS__))))
#define PPUTLX_366(...)  PPUTLX_90(PPUTLX_90(PPUTLX_91(PPUTLX_91(__VA_ARGS__))))
#define PPUTLX_365(...)  PPUTLX_90(PPUTLX_90(PPUTLX_90(PPUTLX_91(__VA_ARGS__))))
#define PPUTLX_364(...)  PPUTLX_90(PPUTLX_90(PPUTLX_90(PPUTLX_90(__VA_ARGS__))))
#define PPUTLX_363(...)  PPUTLX_89(PPUTLX_90(PPUTLX_90(PPUTLX_90(__VA_ARGS__))))
#define PPUTLX_362(...)  PPUTLX_89(PPUTLX_89(PPUTLX_90(PPUTLX_90(__VA_ARGS__))))
#define PPUTLX_361(...)  PPUTLX_89(PPUTLX_89(PPUTLX_89(PPUTLX_90(__VA_ARGS__))))
#define PPUTLX_360(...)  PPUTLX_89(PPUTLX_89(PPUTLX_89(PPUTLX_89(__VA_ARGS__))))
#define PPUTLX_359(...)  PPUTLX_88(PPUTLX_89(PPUTLX_89(PPUTLX_89(__VA_ARGS__))))
#define PPUTLX_358(...)  PPUTLX_88(PPUTLX_88(PPUTLX_89(PPUTLX_89(__VA_ARGS__))))
#define PPUTLX_357(...)  PPUTLX_88(PPUTLX_88(PPUTLX_88(PPUTLX_89(__VA_ARGS__))))
#define PPUTLX_356(...)  PPUTLX_88(PPUTLX_88(PPUTLX_88(PPUTLX_88(__VA_ARGS__))))
#define PPUTLX_355(...)  PPUTLX_87(PPUTLX_88(PPUTLX_88(PPUTLX_88(__VA_ARGS__))))
#define PPUTLX_354(...)  PPUTLX_87(PPUTLX_87(PPUTLX_88(PPUTLX_88(__VA_ARGS__))))
#define PPUTLX_353(...)  PPUTLX_87(PPUTLX_87(PPUTLX_87(PPUTLX_88(__VA_ARGS__))))
#define PPUTLX_352(...)  PPUTLX_87(PPUTLX_87(PPUTLX_87(PPUTLX_87(__VA_ARGS__))))
#define PPUTLX_351(...)  PPUTLX_86(PPUTLX_87(PPUTLX_87(PPUTLX_87(__VA_ARGS__))))
#define PPUTLX_350(...)  PPUTLX_86(PPUTLX_86(PPUTLX_87(PPUTLX_87(__VA_ARGS__))))
#define PPUTLX_349(...)  PPUTLX_86(PPUTLX_86(PPUTLX_86(PPUTLX_87(__VA_ARGS__))))
#define PPUTLX_348(...)  PPUTLX_86(PPUTLX_86(PPUTLX_86(PPUTLX_86(__VA_ARGS__))))
#define PPUTLX_347(...)  PPUTLX_85(PPUTLX_86(PPUTLX_86(PPUTLX_86(__VA_ARGS__))))
#define PPUTLX_346(...)  PPUTLX_85(PPUTLX_85(PPUTLX_86(PPUTLX_86(__VA_ARGS__))))
#define PPUTLX_345(...)  PPUTLX_85(PPUTLX_85(PPUTLX_85(PPUTLX_86(__VA_ARGS__))))
#define PPUTLX_344(...)  PPUTLX_85(PPUTLX_85(PPUTLX_85(PPUTLX_85(__VA_ARGS__))))
#define PPUTLX_343(...)  PPUTLX_84(PPUTLX_85(PPUTLX_85(PPUTLX_85(__VA_ARGS__))))
#define PPUTLX_342(...)  PPUTLX_84(PPUTLX_84(PPUTLX_85(PPUTLX_85(__VA_ARGS__))))
#define PPUTLX_341(...)  PPUTLX_84(PPUTLX_84(PPUTLX_84(PPUTLX_85(__VA_ARGS__))))
#define PPUTLX_340(...)  PPUTLX_84(PPUTLX_84(PPUTLX_84(PPUTLX_84(__VA_ARGS__))))
#define PPUTLX_339(...)  PPUTLX_83(PPUTLX_84(PPUTLX_84(PPUTLX_84(__VA_ARGS__))))
#define PPUTLX_338(...)  PPUTLX_83(PPUTLX_83(PPUTLX_84(PPUTLX_84(__VA_ARGS__))))
#define PPUTLX_337(...)  PPUTLX_83(PPUTLX_83(PPUTLX_83(PPUTLX_84(__VA_ARGS__))))
#define PPUTLX_336(...)  PPUTLX_83(PPUTLX_83(PPUTLX_83(PPUTLX_83(__VA_ARGS__))))
#define PPUTLX_335(...)  PPUTLX_82(PPUTLX_83(PPUTLX_83(PPUTLX_83(__VA_ARGS__))))
#define PPUTLX_334(...)  PPUTLX_82(PPUTLX_82(PPUTLX_83(PPUTLX_83(__VA_ARGS__))))
#define PPUTLX_333(...)  PPUTLX_82(PPUTLX_82(PPUTLX_82(PPUTLX_83(__VA_ARGS__))))
#define PPUTLX_332(...)  PPUTLX_82(PPUTLX_82(PPUTLX_82(PPUTLX_82(__VA_ARGS__))))
#define PPUTLX_331(...)  PPUTLX_81(PPUTLX_82(PPUTLX_82(PPUTLX_82(__VA_ARGS__))))
#define PPUTLX_330(...)  PPUTLX_81(PPUTLX_81(PPUTLX_82(PPUTLX_82(__VA_ARGS__))))
#define PPUTLX_329(...)  PPUTLX_81(PPUTLX_81(PPUTLX_81(PPUTLX_82(__VA_ARGS__))))
#define PPUTLX_328(...)  PPUTLX_81(PPUTLX_81(PPUTLX_81(PPUTLX_81(__VA_ARGS__))))
#define PPUTLX_327(...)  PPUTLX_80(PPUTLX_81(PPUTLX_81(PPUTLX_81(__VA_ARGS__))))
#define PPUTLX_326(...)  PPUTLX_80(PPUTLX_80(PPUTLX_81(PPUTLX_81(__VA_ARGS__))))
#define PPUTLX_325(...)  PPUTLX_80(PPUTLX_80(PPUTLX_80(PPUTLX_81(__VA_ARGS__))))
#define PPUTLX_324(...)  PPUTLX_80(PPUTLX_80(PPUTLX_80(PPUTLX_80(__VA_ARGS__))))
#define PPUTLX_323(...)  PPUTLX_79(PPUTLX_80(PPUTLX_80(PPUTLX_80(__VA_ARGS__))))
#define PPUTLX_322(...)  PPUTLX_79(PPUTLX_79(PPUTLX_80(PPUTLX_80(__VA_ARGS__))))
#define PPUTLX_321(...)  PPUTLX_79(PPUTLX_79(PPUTLX_79(PPUTLX_80(__VA_ARGS__))))
#define PPUTLX_320(...)  PPUTLX_79(PPUTLX_79(PPUTLX_79(PPUTLX_79(__VA_ARGS__))))
#define PPUTLX_319(...)  PPUTLX_78(PPUTLX_79(PPUTLX_79(PPUTLX_79(__VA_ARGS__))))
#define PPUTLX_318(...)  PPUTLX_78(PPUTLX_78(PPUTLX_79(PPUTLX_79(__VA_ARGS__))))
#define PPUTLX_317(...)  PPUTLX_78(PPUTLX_78(PPUTLX_78(PPUTLX_79(__VA_ARGS__))))
#define PPUTLX_316(...)  PPUTLX_78(PPUTLX_78(PPUTLX_78(PPUTLX_78(__VA_ARGS__))))
#define PPUTLX_315(...)  PPUTLX_77(PPUTLX_78(PPUTLX_78(PPUTLX_78(__VA_ARGS__))))
#define PPUTLX_314(...)  PPUTLX_77(PPUTLX_77(PPUTLX_78(PPUTLX_78(__VA_ARGS__))))
#define PPUTLX_313(...)  PPUTLX_77(PPUTLX_77(PPUTLX_77(PPUTLX_78(__VA_ARGS__))))
#define PPUTLX_312(...)  PPUTLX_77(PPUTLX_77(PPUTLX_77(PPUTLX_77(__VA_ARGS__))))
#define PPUTLX_311(...)  PPUTLX_76(PPUTLX_77(PPUTLX_77(PPUTLX_77(__VA_ARGS__))))
#define PPUTLX_310(...)  PPUTLX_76(PPUTLX_76(PPUTLX_77(PPUTLX_77(__VA_ARGS__))))
#define PPUTLX_309(...)  PPUTLX_76(PPUTLX_76(PPUTLX_76(PPUTLX_77(__VA_ARGS__))))
#define PPUTLX_308(...)  PPUTLX_76(PPUTLX_76(PPUTLX_76(PPUTLX_76(__VA_ARGS__))))
#define PPUTLX_307(...)  PPUTLX_75(PPUTLX_76(PPUTLX_76(PPUTLX_76(__VA_ARGS__))))
#define PPUTLX_306(...)  PPUTLX_75(PPUTLX_75(PPUTLX_76(PPUTLX_76(__VA_ARGS__))))
#define PPUTLX_305(...)  PPUTLX_75(PPUTLX_75(PPUTLX_75(PPUTLX_76(__VA_ARGS__))))
#define PPUTLX_304(...)  PPUTLX_75(PPUTLX_75(PPUTLX_75(PPUTLX_75(__VA_ARGS__))))
#define PPUTLX_303(...)  PPUTLX_74(PPUTLX_75(PPUTLX_75(PPUTLX_75(__VA_ARGS__))))
#define PPUTLX_302(...)  PPUTLX_74(PPUTLX_74(PPUTLX_75(PPUTLX_75(__VA_ARGS__))))
#define PPUTLX_301(...)  PPUTLX_74(PPUTLX_74(PPUTLX_74(PPUTLX_75(__VA_ARGS__))))
#define PPUTLX_300(...)  PPUTLX_74(PPUTLX_74(PPUTLX_74(PPUTLX_74(__VA_ARGS__))))
#define PPUTLX_299(...)  PPUTLX_73(PPUTLX_74(PPUTLX_74(PPUTLX_74(__VA_ARGS__))))
#define PPUTLX_298(...)  PPUTLX_73(PPUTLX_73(PPUTLX_74(PPUTLX_74(__VA_ARGS__))))
#define PPUTLX_297(...)  PPUTLX_73(PPUTLX_73(PPUTLX_73(PPUTLX_74(__VA_ARGS__))))
#define PPUTLX_296(...)  PPUTLX_73(PPUTLX_73(PPUTLX_73(PPUTLX_73(__VA_ARGS__))))
#define PPUTLX_295(...)  PPUTLX_72(PPUTLX_73(PPUTLX_73(PPUTLX_73(__VA_ARGS__))))
#define PPUTLX_294(...)  PPUTLX_72(PPUTLX_72(PPUTLX_73(PPUTLX_73(__VA_ARGS__))))
#define PPUTLX_293(...)  PPUTLX_72(PPUTLX_72(PPUTLX_72(PPUTLX_73(__VA_ARGS__))))
#define PPUTLX_292(...)  PPUTLX_72(PPUTLX_72(PPUTLX_72(PPUTLX_72(__VA_ARGS__))))
#define PPUTLX_291(...)  PPUTLX_71(PPUTLX_72(PPUTLX_72(PPUTLX_72(__VA_ARGS__))))
#define PPUTLX_290(...)  PPUTLX_71(PPUTLX_71(PPUTLX_72(PPUTLX_72(__VA_ARGS__))))
#define PPUTLX_289(...)  PPUTLX_71(PPUTLX_71(PPUTLX_71(PPUTLX_72(__VA_ARGS__))))
#define PPUTLX_288(...)  PPUTLX_71(PPUTLX_71(PPUTLX_71(PPUTLX_71(__VA_ARGS__))))
#define PPUTLX_287(...)  PPUTLX_70(PPUTLX_71(PPUTLX_71(PPUTLX_71(__VA_ARGS__))))
#define PPUTLX_286(...)  PPUTLX_70(PPUTLX_70(PPUTLX_71(PPUTLX_71(__VA_ARGS__))))
#define PPUTLX_285(...)  PPUTLX_70(PPUTLX_70(PPUTLX_70(PPUTLX_71(__VA_ARGS__))))
#define PPUTLX_284(...)  PPUTLX_70(PPUTLX_70(PPUTLX_70(PPUTLX_70(__VA_ARGS__))))
#define PPUTLX_283(...)  PPUTLX_69(PPUTLX_70(PPUTLX_70(PPUTLX_70(__VA_ARGS__))))
#define PPUTLX_282(...)  PPUTLX_69(PPUTLX_69(PPUTLX_70(PPUTLX_70(__VA_ARGS__))))
#define PPUTLX_281(...)  PPUTLX_69(PPUTLX_69(PPUTLX_69(PPUTLX_70(__VA_ARGS__))))
#define PPUTLX_280(...)  PPUTLX_69(PPUTLX_69(PPUTLX_69(PPUTLX_69(__VA_ARGS__))))
#define PPUTLX_279(...)  PPUTLX_68(PPUTLX_69(PPUTLX_69(PPUTLX_69(__VA_ARGS__))))
#define PPUTLX_278(...)  PPUTLX_68(PPUTLX_68(PPUTLX_69(PPUTLX_69(__VA_ARGS__))))
#define PPUTLX_277(...)  PPUTLX_68(PPUTLX_68(PPUTLX_68(PPUTLX_69(__VA_ARGS__))))
#define PPUTLX_276(...)  PPUTLX_68(PPUTLX_68(PPUTLX_68(PPUTLX_68(__VA_ARGS__))))
#define PPUTLX_275(...)  PPUTLX_67(PPUTLX_68(PPUTLX_68(PPUTLX_68(__VA_ARGS__))))
#define PPUTLX_274(...)  PPUTLX_67(PPUTLX_67(PPUTLX_68(PPUTLX_68(__VA_ARGS__))))
#define PPUTLX_273(...)  PPUTLX_67(PPUTLX_67(PPUTLX_67(PPUTLX_68(__VA_ARGS__))))
#define PPUTLX_272(...)  PPUTLX_67(PPUTLX_67(PPUTLX_67(PPUTLX_67(__VA_ARGS__))))
#define PPUTLX_271(...)  PPUTLX_66(PPUTLX_67(PPUTLX_67(PPUTLX_67(__VA_ARGS__))))
#define PPUTLX_270(...)  PPUTLX_66(PPUTLX_66(PPUTLX_67(PPUTLX_67(__VA_ARGS__))))
#define PPUTLX_269(...)  PPUTLX_66(PPUTLX_66(PPUTLX_66(PPUTLX_67(__VA_ARGS__))))
#define PPUTLX_268(...)  PPUTLX_66(PPUTLX_66(PPUTLX_66(PPUTLX_66(__VA_ARGS__))))
#define PPUTLX_267(...)  PPUTLX_65(PPUTLX_66(PPUTLX_66(PPUTLX_66(__VA_ARGS__))))
#define PPUTLX_266(...)  PPUTLX_65(PPUTLX_65(PPUTLX_66(PPUTLX_66(__VA_ARGS__))))
#define PPUTLX_265(...)  PPUTLX_65(PPUTLX_65(PPUTLX_65(PPUTLX_66(__VA_ARGS__))))
#define PPUTLX_264(...)  PPUTLX_65(PPUTLX_65(PPUTLX_65(PPUTLX_65(__VA_ARGS__))))
#define PPUTLX_263(...)  PPUTLX_64(PPUTLX_65(PPUTLX_65(PPUTLX_65(__VA_ARGS__))))
#define PPUTLX_262(...)  PPUTLX_64(PPUTLX_64(PPUTLX_65(PPUTLX_65(__VA_ARGS__))))
#define PPUTLX_261(...)  PPUTLX_64(PPUTLX_64(PPUTLX_64(PPUTLX_65(__VA_ARGS__))))
#define PPUTLX_260(...)  PPUTLX_64(PPUTLX_64(PPUTLX_64(PPUTLX_64(__VA_ARGS__))))
#define PPUTLX_259(...)  PPUTLX_63(PPUTLX_64(PPUTLX_64(PPUTLX_64(__VA_ARGS__))))
#define PPUTLX_258(...)  PPUTLX_63(PPUTLX_63(PPUTLX_64(PPUTLX_64(__VA_ARGS__))))
#define PPUTLX_257(...)  PPUTLX_63(PPUTLX_63(PPUTLX_63(PPUTLX_64(__VA_ARGS__))))
#define PPUTLX_256(...)  PPUTLX_63(PPUTLX_63(PPUTLX_63(PPUTLX_63(__VA_ARGS__))))
#define PPUTLX_255(...)  PPUTLX_62(PPUTLX_63(PPUTLX_63(PPUTLX_63(__VA_ARGS__))))
#define PPUTLX_254(...)  PPUTLX_62(PPUTLX_62(PPUTLX_63(PPUTLX_63(__VA_ARGS__))))
#define PPUTLX_253(...)  PPUTLX_62(PPUTLX_62(PPUTLX_62(PPUTLX_63(__VA_ARGS__))))
#define PPUTLX_252(...)  PPUTLX_62(PPUTLX_62(PPUTLX_62(PPUTLX_62(__VA_ARGS__))))
#define PPUTLX_251(...)  PPUTLX_61(PPUTLX_62(PPUTLX_62(PPUTLX_62(__VA_ARGS__))))
#define PPUTLX_250(...)  PPUTLX_61(PPUTLX_61(PPUTLX_62(PPUTLX_62(__VA_ARGS__))))
#define PPUTLX_249(...)  PPUTLX_61(PPUTLX_61(PPUTLX_61(PPUTLX_62(__VA_ARGS__))))
#define PPUTLX_248(...)  PPUTLX_61(PPUTLX_61(PPUTLX_61(PPUTLX_61(__VA_ARGS__))))
#define PPUTLX_247(...)  PPUTLX_60(PPUTLX_61(PPUTLX_61(PPUTLX_61(__VA_ARGS__))))
#define PPUTLX_246(...)  PPUTLX_60(PPUTLX_60(PPUTLX_61(PPUTLX_61(__VA_ARGS__))))
#define PPUTLX_245(...)  PPUTLX_60(PPUTLX_60(PPUTLX_60(PPUTLX_61(__VA_ARGS__))))
#define PPUTLX_244(...)  PPUTLX_60(PPUTLX_60(PPUTLX_60(PPUTLX_60(__VA_ARGS__))))
#define PPUTLX_243(...)  PPUTLX_59(PPUTLX_60(PPUTLX_60(PPUTLX_60(__VA_ARGS__))))
#define PPUTLX_242(...)  PPUTLX_59(PPUTLX_59(PPUTLX_60(PPUTLX_60(__VA_ARGS__))))
#define PPUTLX_241(...)  PPUTLX_59(PPUTLX_59(PPUTLX_59(PPUTLX_60(__VA_ARGS__))))
#define PPUTLX_240(...)  PPUTLX_59(PPUTLX_59(PPUTLX_59(PPUTLX_59(__VA_ARGS__))))
#define PPUTLX_239(...)  PPUTLX_58(PPUTLX_59(PPUTLX_59(PPUTLX_59(__VA_ARGS__))))
#define PPUTLX_238(...)  PPUTLX_58(PPUTLX_58(PPUTLX_59(PPUTLX_59(__VA_ARGS__))))
#define PPUTLX_237(...)  PPUTLX_58(PPUTLX_58(PPUTLX_58(PPUTLX_59(__VA_ARGS__))))
#define PPUTLX_236(...)  PPUTLX_58(PPUTLX_58(PPUTLX_58(PPUTLX_58(__VA_ARGS__))))
#define PPUTLX_235(...)  PPUTLX_57(PPUTLX_58(PPUTLX_58(PPUTLX_58(__VA_ARGS__))))
#define PPUTLX_234(...)  PPUTLX_57(PPUTLX_57(PPUTLX_58(PPUTLX_58(__VA_ARGS__))))
#define PPUTLX_233(...)  PPUTLX_57(PPUTLX_57(PPUTLX_57(PPUTLX_58(__VA_ARGS__))))
#define PPUTLX_232(...)  PPUTLX_57(PPUTLX_57(PPUTLX_57(PPUTLX_57(__VA_ARGS__))))
#define PPUTLX_231(...)  PPUTLX_56(PPUTLX_57(PPUTLX_57(PPUTLX_57(__VA_ARGS__))))
#define PPUTLX_230(...)  PPUTLX_56(PPUTLX_56(PPUTLX_57(PPUTLX_57(__VA_ARGS__))))
#define PPUTLX_229(...)  PPUTLX_56(PPUTLX_56(PPUTLX_56(PPUTLX_57(__VA_ARGS__))))
#define PPUTLX_228(...)  PPUTLX_56(PPUTLX_56(PPUTLX_56(PPUTLX_56(__VA_ARGS__))))
#define PPUTLX_227(...)  PPUTLX_55(PPUTLX_56(PPUTLX_56(PPUTLX_56(__VA_ARGS__))))
#define PPUTLX_226(...)  PPUTLX_55(PPUTLX_55(PPUTLX_56(PPUTLX_56(__VA_ARGS__))))
#define PPUTLX_225(...)  PPUTLX_55(PPUTLX_55(PPUTLX_55(PPUTLX_56(__VA_ARGS__))))
#define PPUTLX_224(...)  PPUTLX_55(PPUTLX_55(PPUTLX_55(PPUTLX_55(__VA_ARGS__))))
#define PPUTLX_223(...)  PPUTLX_54(PPUTLX_55(PPUTLX_55(PPUTLX_55(__VA_ARGS__))))
#define PPUTLX_222(...)  PPUTLX_54(PPUTLX_54(PPUTLX_55(PPUTLX_55(__VA_ARGS__))))
#define PPUTLX_221(...)  PPUTLX_54(PPUTLX_54(PPUTLX_54(PPUTLX_55(__VA_ARGS__))))
#define PPUTLX_220(...)  PPUTLX_54(PPUTLX_54(PPUTLX_54(PPUTLX_54(__VA_ARGS__))))
#define PPUTLX_219(...)  PPUTLX_53(PPUTLX_54(PPUTLX_54(PPUTLX_54(__VA_ARGS__))))
#define PPUTLX_218(...)  PPUTLX_53(PPUTLX_53(PPUTLX_54(PPUTLX_54(__VA_ARGS__))))
#define PPUTLX_217(...)  PPUTLX_53(PPUTLX_53(PPUTLX_53(PPUTLX_54(__VA_ARGS__))))
#define PPUTLX_216(...)  PPUTLX_53(PPUTLX_53(PPUTLX_53(PPUTLX_53(__VA_ARGS__))))
#define PPUTLX_215(...)  PPUTLX_52(PPUTLX_53(PPUTLX_53(PPUTLX_53(__VA_ARGS__))))
#define PPUTLX_214(...)  PPUTLX_52(PPUTLX_52(PPUTLX_53(PPUTLX_53(__VA_ARGS__))))
#define PPUTLX_213(...)  PPUTLX_52(PPUTLX_52(PPUTLX_52(PPUTLX_53(__VA_ARGS__))))
#define PPUTLX_212(...)  PPUTLX_52(PPUTLX_52(PPUTLX_52(PPUTLX_52(__VA_ARGS__))))
#define PPUTLX_211(...)  PPUTLX_51(PPUTLX_52(PPUTLX_52(PPUTLX_52(__VA_ARGS__))))
#define PPUTLX_210(...)  PPUTLX_51(PPUTLX_51(PPUTLX_52(PPUTLX_52(__VA_ARGS__))))
#define PPUTLX_209(...)  PPUTLX_51(PPUTLX_51(PPUTLX_51(PPUTLX_52(__VA_ARGS__))))
#define PPUTLX_208(...)  PPUTLX_51(PPUTLX_51(PPUTLX_51(PPUTLX_51(__VA_ARGS__))))
#define PPUTLX_207(...)  PPUTLX_50(PPUTLX_51(PPUTLX_51(PPUTLX_51(__VA_ARGS__))))
#define PPUTLX_206(...)  PPUTLX_50(PPUTLX_50(PPUTLX_51(PPUTLX_51(__VA_ARGS__))))
#define PPUTLX_205(...)  PPUTLX_50(PPUTLX_50(PPUTLX_50(PPUTLX_51(__VA_ARGS__))))
#define PPUTLX_204(...)  PPUTLX_50(PPUTLX_50(PPUTLX_50(PPUTLX_50(__VA_ARGS__))))
#define PPUTLX_203(...)  PPUTLX_49(PPUTLX_50(PPUTLX_50(PPUTLX_50(__VA_ARGS__))))
#define PPUTLX_202(...)  PPUTLX_49(PPUTLX_49(PPUTLX_50(PPUTLX_50(__VA_ARGS__))))
#define PPUTLX_201(...)  PPUTLX_49(PPUTLX_49(PPUTLX_49(PPUTLX_50(__VA_ARGS__))))
#define PPUTLX_200(...)  PPUTLX_49(PPUTLX_49(PPUTLX_49(PPUTLX_49(__VA_ARGS__))))
#define PPUTLX_199(...)  PPUTLX_48(PPUTLX_49(PPUTLX_49(PPUTLX_49(__VA_ARGS__))))
#define PPUTLX_198(...)  PPUTLX_48(PPUTLX_48(PPUTLX_49(PPUTLX_49(__VA_ARGS__))))
#define PPUTLX_197(...)  PPUTLX_48(PPUTLX_48(PPUTLX_48(PPUTLX_49(__VA_ARGS__))))
#define PPUTLX_196(...)  PPUTLX_48(PPUTLX_48(PPUTLX_48(PPUTLX_48(__VA_ARGS__))))
#define PPUTLX_195(...)  PPUTLX_47(PPUTLX_48(PPUTLX_48(PPUTLX_48(__VA_ARGS__))))
#define PPUTLX_194(...)  PPUTLX_47(PPUTLX_47(PPUTLX_48(PPUTLX_48(__VA_ARGS__))))
#define PPUTLX_193(...)  PPUTLX_47(PPUTLX_47(PPUTLX_47(PPUTLX_48(__VA_ARGS__))))
#define PPUTLX_192(...)  PPUTLX_47(PPUTLX_47(PPUTLX_47(PPUTLX_47(__VA_ARGS__))))
#define PPUTLX_191(...)  PPUTLX_46(PPUTLX_47(PPUTLX_47(PPUTLX_47(__VA_ARGS__))))
#define PPUTLX_190(...)  PPUTLX_46(PPUTLX_46(PPUTLX_47(PPUTLX_47(__VA_ARGS__))))
#define PPUTLX_189(...)  PPUTLX_46(PPUTLX_46(PPUTLX_46(PPUTLX_47(__VA_ARGS__))))
#define PPUTLX_188(...)  PPUTLX_46(PPUTLX_46(PPUTLX_46(PPUTLX_46(__VA_ARGS__))))
#define PPUTLX_187(...)  PPUTLX_45(PPUTLX_46(PPUTLX_46(PPUTLX_46(__VA_ARGS__))))
#define PPUTLX_186(...)  PPUTLX_45(PPUTLX_45(PPUTLX_46(PPUTLX_46(__VA_ARGS__))))
#define PPUTLX_185(...)  PPUTLX_45(PPUTLX_45(PPUTLX_45(PPUTLX_46(__VA_ARGS__))))
#define PPUTLX_184(...)  PPUTLX_45(PPUTLX_45(PPUTLX_45(PPUTLX_45(__VA_ARGS__))))
#define PPUTLX_183(...)  PPUTLX_44(PPUTLX_45(PPUTLX_45(PPUTLX_45(__VA_ARGS__))))
#define PPUTLX_182(...)  PPUTLX_44(PPUTLX_44(PPUTLX_45(PPUTLX_45(__VA_ARGS__))))
#define PPUTLX_181(...)  PPUTLX_44(PPUTLX_44(PPUTLX_44(PPUTLX_45(__VA_ARGS__))))
#define PPUTLX_180(...)  PPUTLX_44(PPUTLX_44(PPUTLX_44(PPUTLX_44(__VA_ARGS__))))
#define PPUTLX_179(...)  PPUTLX_43(PPUTLX_44(PPUTLX_44(PPUTLX_44(__VA_ARGS__))))
#define PPUTLX_178(...)  PPUTLX_43(PPUTLX_43(PPUTLX_44(PPUTLX_44(__VA_ARGS__))))
#define PPUTLX_177(...)  PPUTLX_43(PPUTLX_43(PPUTLX_43(PPUTLX_44(__VA_ARGS__))))
#define PPUTLX_176(...)  PPUTLX_43(PPUTLX_43(PPUTLX_43(PPUTLX_43(__VA_ARGS__))))
#define PPUTLX_175(...)  PPUTLX_42(PPUTLX_43(PPUTLX_43(PPUTLX_43(__VA_ARGS__))))
#define PPUTLX_174(...)  PPUTLX_42(PPUTLX_42(PPUTLX_43(PPUTLX_43(__VA_ARGS__))))
#define PPUTLX_173(...)  PPUTLX_42(PPUTLX_42(PPUTLX_42(PPUTLX_43(__VA_ARGS__))))
#define PPUTLX_172(...)  PPUTLX_42(PPUTLX_42(PPUTLX_42(PPUTLX_42(__VA_ARGS__))))
#define PPUTLX_171(...)  PPUTLX_41(PPUTLX_42(PPUTLX_42(PPUTLX_42(__VA_ARGS__))))
#define PPUTLX_170(...)  PPUTLX_41(PPUTLX_41(PPUTLX_42(PPUTLX_42(__VA_ARGS__))))
#define PPUTLX_169(...)  PPUTLX_41(PPUTLX_41(PPUTLX_41(PPUTLX_42(__VA_ARGS__))))
#define PPUTLX_168(...)  PPUTLX_41(PPUTLX_41(PPUTLX_41(PPUTLX_41(__VA_ARGS__))))
#define PPUTLX_167(...)  PPUTLX_40(PPUTLX_41(PPUTLX_41(PPUTLX_41(__VA_ARGS__))))
#define PPUTLX_166(...)  PPUTLX_40(PPUTLX_40(PPUTLX_41(PPUTLX_41(__VA_ARGS__))))
#define PPUTLX_165(...)  PPUTLX_40(PPUTLX_40(PPUTLX_40(PPUTLX_41(__VA_ARGS__))))
#define PPUTLX_164(...)  PPUTLX_40(PPUTLX_40(PPUTLX_40(PPUTLX_40(__VA_ARGS__))))
#define PPUTLX_163(...)  PPUTLX_39(PPUTLX_40(PPUTLX_40(PPUTLX_40(__VA_ARGS__))))
#define PPUTLX_162(...)  PPUTLX_39(PPUTLX_39(PPUTLX_40(PPUTLX_40(__VA_ARGS__))))
#define PPUTLX_161(...)  PPUTLX_39(PPUTLX_39(PPUTLX_39(PPUTLX_40(__VA_ARGS__))))
#define PPUTLX_160(...)  PPUTLX_39(PPUTLX_39(PPUTLX_39(PPUTLX_39(__VA_ARGS__))))
#define PPUTLX_159(...)  PPUTLX_38(PPUTLX_39(PPUTLX_39(PPUTLX_39(__VA_ARGS__))))
#define PPUTLX_158(...)  PPUTLX_38(PPUTLX_38(PPUTLX_39(PPUTLX_39(__VA_ARGS__))))
#define PPUTLX_157(...)  PPUTLX_38(PPUTLX_38(PPUTLX_38(PPUTLX_39(__VA_ARGS__))))
#define PPUTLX_156(...)  PPUTLX_38(PPUTLX_38(PPUTLX_38(PPUTLX_38(__VA_ARGS__))))
#define PPUTLX_155(...)  PPUTLX_37(PPUTLX_38(PPUTLX_38(PPUTLX_38(__VA_ARGS__))))
#define PPUTLX_154(...)  PPUTLX_37(PPUTLX_37(PPUTLX_38(PPUTLX_38(__VA_ARGS__))))
#define PPUTLX_153(...)  PPUTLX_37(PPUTLX_37(PPUTLX_37(PPUTLX_38(__VA_ARGS__))))
#define PPUTLX_152(...)  PPUTLX_37(PPUTLX_37(PPUTLX_37(PPUTLX_37(__VA_ARGS__))))
#define PPUTLX_151(...)  PPUTLX_36(PPUTLX_37(PPUTLX_37(PPUTLX_37(__VA_ARGS__))))
#define PPUTLX_150(...)  PPUTLX_36(PPUTLX_36(PPUTLX_37(PPUTLX_37(__VA_ARGS__))))
#define PPUTLX_149(...)  PPUTLX_36(PPUTLX_36(PPUTLX_36(PPUTLX_37(__VA_ARGS__))))
#define PPUTLX_148(...)  PPUTLX_36(PPUTLX_36(PPUTLX_36(PPUTLX_36(__VA_ARGS__))))
#define PPUTLX_147(...)  PPUTLX_35(PPUTLX_36(PPUTLX_36(PPUTLX_36(__VA_ARGS__))))
#define PPUTLX_146(...)  PPUTLX_35(PPUTLX_35(PPUTLX_36(PPUTLX_36(__VA_ARGS__))))
#define PPUTLX_145(...)  PPUTLX_35(PPUTLX_35(PPUTLX_35(PPUTLX_36(__VA_ARGS__))))
#define PPUTLX_144(...)  PPUTLX_35(PPUTLX_35(PPUTLX_35(PPUTLX_35(__VA_ARGS__))))
#define PPUTLX_143(...)  PPUTLX_34(PPUTLX_35(PPUTLX_35(PPUTLX_35(__VA_ARGS__))))
#define PPUTLX_142(...)  PPUTLX_34(PPUTLX_34(PPUTLX_35(PPUTLX_35(__VA_ARGS__))))
#define PPUTLX_141(...)  PPUTLX_34(PPUTLX_34(PPUTLX_34(PPUTLX_35(__VA_ARGS__))))
#define PPUTLX_140(...)  PPUTLX_34(PPUTLX_34(PPUTLX_34(PPUTLX_34(__VA_ARGS__))))
#define PPUTLX_139(...)  PPUTLX_33(PPUTLX_34(PPUTLX_34(PPUTLX_34(__VA_ARGS__))))
#define PPUTLX_138(...)  PPUTLX_33(PPUTLX_33(PPUTLX_34(PPUTLX_34(__VA_ARGS__))))
#define PPUTLX_137(...)  PPUTLX_33(PPUTLX_33(PPUTLX_33(PPUTLX_34(__VA_ARGS__))))
#define PPUTLX_136(...)  PPUTLX_33(PPUTLX_33(PPUTLX_33(PPUTLX_33(__VA_ARGS__))))
#define PPUTLX_135(...)  PPUTLX_32(PPUTLX_33(PPUTLX_33(PPUTLX_33(__VA_ARGS__))))
#define PPUTLX_134(...)  PPUTLX_32(PPUTLX_32(PPUTLX_33(PPUTLX_33(__VA_ARGS__))))
#define PPUTLX_133(...)  PPUTLX_32(PPUTLX_32(PPUTLX_32(PPUTLX_33(__VA_ARGS__))))
#define PPUTLX_132(...)  PPUTLX_32(PPUTLX_32(PPUTLX_32(PPUTLX_32(__VA_ARGS__))))
#define PPUTLX_131(...)  PPUTLX_31(PPUTLX_32(PPUTLX_32(PPUTLX_32(__VA_ARGS__))))
#define PPUTLX_130(...)  PPUTLX_31(PPUTLX_31(PPUTLX_32(PPUTLX_32(__VA_ARGS__))))
#define PPUTLX_129(...)  PPUTLX_31(PPUTLX_31(PPUTLX_31(PPUTLX_32(__VA_ARGS__))))
#define PPUTLX_128(...)  PPUTLX_31(PPUTLX_31(PPUTLX_31(PPUTLX_31(__VA_ARGS__))))
#define PPUTLX_127(...)  PPUTLX_30(PPUTLX_31(PPUTLX_31(PPUTLX_31(__VA_ARGS__))))
#define PPUTLX_126(...)  PPUTLX_30(PPUTLX_30(PPUTLX_31(PPUTLX_31(__VA_ARGS__))))
#define PPUTLX_125(...)  PPUTLX_30(PPUTLX_30(PPUTLX_30(PPUTLX_31(__VA_ARGS__))))
#define PPUTLX_124(...)  PPUTLX_30(PPUTLX_30(PPUTLX_30(PPUTLX_30(__VA_ARGS__))))
#define PPUTLX_123(...)  PPUTLX_29(PPUTLX_30(PPUTLX_30(PPUTLX_30(__VA_ARGS__))))
#define PPUTLX_122(...)  PPUTLX_29(PPUTLX_29(PPUTLX_30(PPUTLX_30(__VA_ARGS__))))
#define PPUTLX_121(...)  PPUTLX_29(PPUTLX_29(PPUTLX_29(PPUTLX_30(__VA_ARGS__))))
#define PPUTLX_120(...)  PPUTLX_29(PPUTLX_29(PPUTLX_29(PPUTLX_29(__VA_ARGS__))))
#define PPUTLX_119(...)  PPUTLX_28(PPUTLX_29(PPUTLX_29(PPUTLX_29(__VA_ARGS__))))
#define PPUTLX_118(...)  PPUTLX_28(PPUTLX_28(PPUTLX_29(PPUTLX_29(__VA_ARGS__))))
#define PPUTLX_117(...)  PPUTLX_28(PPUTLX_28(PPUTLX_28(PPUTLX_29(__VA_ARGS__))))
#define PPUTLX_116(...)  PPUTLX_28(PPUTLX_28(PPUTLX_28(PPUTLX_28(__VA_ARGS__))))
#define PPUTLX_115(...)  PPUTLX_27(PPUTLX_28(PPUTLX_28(PPUTLX_28(__VA_ARGS__))))
#define PPUTLX_114(...)  PPUTLX_27(PPUTLX_27(PPUTLX_28(PPUTLX_28(__VA_ARGS__))))
#define PPUTLX_113(...)  PPUTLX_27(PPUTLX_27(PPUTLX_27(PPUTLX_28(__VA_ARGS__))))
#define PPUTLX_112(...)  PPUTLX_27(PPUTLX_27(PPUTLX_27(PPUTLX_27(__VA_ARGS__))))
#define PPUTLX_111(...)  PPUTLX_26(PPUTLX_27(PPUTLX_27(PPUTLX_27(__VA_ARGS__))))
#define PPUTLX_110(...)  PPUTLX_26(PPUTLX_26(PPUTLX_27(PPUTLX_27(__VA_ARGS__))))
#define PPUTLX_109(...)  PPUTLX_26(PPUTLX_26(PPUTLX_26(PPUTLX_27(__VA_ARGS__))))
#define PPUTLX_108(...)  PPUTLX_26(PPUTLX_26(PPUTLX_26(PPUTLX_26(__VA_ARGS__))))
#define PPUTLX_107(...)  PPUTLX_25(PPUTLX_26(PPUTLX_26(PPUTLX_26(__VA_ARGS__))))
#define PPUTLX_106(...)  PPUTLX_25(PPUTLX_25(PPUTLX_26(PPUTLX_26(__VA_ARGS__))))
#define PPUTLX_105(...)  PPUTLX_25(PPUTLX_25(PPUTLX_25(PPUTLX_26(__VA_ARGS__))))
#define PPUTLX_104(...)  PPUTLX_25(PPUTLX_25(PPUTLX_25(PPUTLX_25(__VA_ARGS__))))
#define PPUTLX_103(...)  PPUTLX_24(PPUTLX_25(PPUTLX_25(PPUTLX_25(__VA_ARGS__))))
#define PPUTLX_102(...)  PPUTLX_24(PPUTLX_24(PPUTLX_25(PPUTLX_25(__VA_ARGS__))))
#define PPUTLX_101(...)  PPUTLX_24(PPUTLX_24(PPUTLX_24(PPUTLX_25(__VA_ARGS__))))
#define PPUTLX_100(...)  PPUTLX_24(PPUTLX_24(PPUTLX_24(PPUTLX_24(__VA_ARGS__))))
#define PPUTLX_99(...)   PPUTLX_23(PPUTLX_24(PPUTLX_24(PPUTLX_24(__VA_ARGS__))))
#define PPUTLX_98(...)   PPUTLX_23(PPUTLX_23(PPUTLX_24(PPUTLX_24(__VA_ARGS__))))
#define PPUTLX_97(...)   PPUTLX_23(PPUTLX_23(PPUTLX_23(PPUTLX_24(__VA_ARGS__))))
#define PPUTLX_96(...)   PPUTLX_23(PPUTLX_23(PPUTLX_23(PPUTLX_23(__VA_ARGS__))))
#define PPUTLX_95(...)   PPUTLX_22(PPUTLX_23(PPUTLX_23(PPUTLX_23(__VA_ARGS__))))
#define PPUTLX_94(...)   PPUTLX_22(PPUTLX_22(PPUTLX_23(PPUTLX_23(__VA_ARGS__))))
#define PPUTLX_93(...)   PPUTLX_22(PPUTLX_22(PPUTLX_22(PPUTLX_23(__VA_ARGS__))))
#define PPUTLX_92(...)   PPUTLX_22(PPUTLX_22(PPUTLX_22(PPUTLX_22(__VA_ARGS__))))
#define PPUTLX_91(...)   PPUTLX_21(PPUTLX_22(PPUTLX_22(PPUTLX_22(__VA_ARGS__))))
#define PPUTLX_90(...)   PPUTLX_21(PPUTLX_21(PPUTLX_22(PPUTLX_22(__VA_ARGS__))))
#define PPUTLX_89(...)   PPUTLX_21(PPUTLX_21(PPUTLX_21(PPUTLX_22(__VA_ARGS__))))
#define PPUTLX_88(...)   PPUTLX_21(PPUTLX_21(PPUTLX_21(PPUTLX_21(__VA_ARGS__))))
#define PPUTLX_87(...)   PPUTLX_20(PPUTLX_21(PPUTLX_21(PPUTLX_21(__VA_ARGS__))))
#define PPUTLX_86(...)   PPUTLX_20(PPUTLX_20(PPUTLX_21(PPUTLX_21(__VA_ARGS__))))
#define PPUTLX_85(...)   PPUTLX_20(PPUTLX_20(PPUTLX_20(PPUTLX_21(__VA_ARGS__))))
#define PPUTLX_84(...)   PPUTLX_20(PPUTLX_20(PPUTLX_20(PPUTLX_20(__VA_ARGS__))))
#define PPUTLX_83(...)   PPUTLX_19(PPUTLX_20(PPUTLX_20(PPUTLX_20(__VA_ARGS__))))
#define PPUTLX_82(...)   PPUTLX_19(PPUTLX_19(PPUTLX_20(PPUTLX_20(__VA_ARGS__))))
#define PPUTLX_81(...)   PPUTLX_19(PPUTLX_19(PPUTLX_19(PPUTLX_20(__VA_ARGS__))))
#define PPUTLX_80(...)   PPUTLX_19(PPUTLX_19(PPUTLX_19(PPUTLX_19(__VA_ARGS__))))
#define PPUTLX_79(...)   PPUTLX_18(PPUTLX_19(PPUTLX_19(PPUTLX_19(__VA_ARGS__))))
#define PPUTLX_78(...)   PPUTLX_18(PPUTLX_18(PPUTLX_19(PPUTLX_19(__VA_ARGS__))))
#define PPUTLX_77(...)   PPUTLX_18(PPUTLX_18(PPUTLX_18(PPUTLX_19(__VA_ARGS__))))
#define PPUTLX_76(...)   PPUTLX_18(PPUTLX_18(PPUTLX_18(PPUTLX_18(__VA_ARGS__))))
#define PPUTLX_75(...)   PPUTLX_17(PPUTLX_18(PPUTLX_18(PPUTLX_18(__VA_ARGS__))))
#define PPUTLX_74(...)   PPUTLX_17(PPUTLX_17(PPUTLX_18(PPUTLX_18(__VA_ARGS__))))
#define PPUTLX_73(...)   PPUTLX_17(PPUTLX_17(PPUTLX_17(PPUTLX_18(__VA_ARGS__))))
#define PPUTLX_72(...)   PPUTLX_17(PPUTLX_17(PPUTLX_17(PPUTLX_17(__VA_ARGS__))))
#define PPUTLX_71(...)   PPUTLX_16(PPUTLX_17(PPUTLX_17(PPUTLX_17(__VA_ARGS__))))
#define PPUTLX_70(...)   PPUTLX_16(PPUTLX_16(PPUTLX_17(PPUTLX_17(__VA_ARGS__))))
#define PPUTLX_69(...)   PPUTLX_16(PPUTLX_16(PPUTLX_16(PPUTLX_17(__VA_ARGS__))))
#define PPUTLX_68(...)   PPUTLX_16(PPUTLX_16(PPUTLX_16(PPUTLX_16(__VA_ARGS__))))
#define PPUTLX_67(...)   PPUTLX_15(PPUTLX_16(PPUTLX_16(PPUTLX_16(__VA_ARGS__))))
#define PPUTLX_66(...)   PPUTLX_15(PPUTLX_15(PPUTLX_16(PPUTLX_16(__VA_ARGS__))))
#define PPUTLX_65(...)   PPUTLX_15(PPUTLX_15(PPUTLX_15(PPUTLX_16(__VA_ARGS__))))
#define PPUTLX_64(...)   PPUTLX_15(PPUTLX_15(PPUTLX_15(PPUTLX_15(__VA_ARGS__))))
#define PPUTLX_63(...)   PPUTLX_14(PPUTLX_15(PPUTLX_15(PPUTLX_15(__VA_ARGS__))))
#define PPUTLX_62(...)   PPUTLX_14(PPUTLX_14(PPUTLX_15(PPUTLX_15(__VA_ARGS__))))
#define PPUTLX_61(...)   PPUTLX_14(PPUTLX_14(PPUTLX_14(PPUTLX_15(__VA_ARGS__))))
#define PPUTLX_60(...)   PPUTLX_14(PPUTLX_14(PPUTLX_14(PPUTLX_14(__VA_ARGS__))))
#define PPUTLX_59(...)   PPUTLX_13(PPUTLX_14(PPUTLX_14(PPUTLX_14(__VA_ARGS__))))
#define PPUTLX_58(...)   PPUTLX_13(PPUTLX_13(PPUTLX_14(PPUTLX_14(__VA_ARGS__))))
#define PPUTLX_57(...)   PPUTLX_13(PPUTLX_13(PPUTLX_13(PPUTLX_14(__VA_ARGS__))))
#define PPUTLX_56(...)   PPUTLX_13(PPUTLX_13(PPUTLX_13(PPUTLX_13(__VA_ARGS__))))
#define PPUTLX_55(...)   PPUTLX_12(PPUTLX_13(PPUTLX_13(PPUTLX_13(__VA_ARGS__))))
#define PPUTLX_54(...)   PPUTLX_12(PPUTLX_12(PPUTLX_13(PPUTLX_13(__VA_ARGS__))))
#define PPUTLX_53(...)   PPUTLX_12(PPUTLX_12(PPUTLX_12(PPUTLX_13(__VA_ARGS__))))
#define PPUTLX_52(...)   PPUTLX_12(PPUTLX_12(PPUTLX_12(PPUTLX_12(__VA_ARGS__))))
#define PPUTLX_51(...)   PPUTLX_11(PPUTLX_12(PPUTLX_12(PPUTLX_12(__VA_ARGS__))))
#define PPUTLX_50(...)   PPUTLX_11(PPUTLX_11(PPUTLX_12(PPUTLX_12(__VA_ARGS__))))
#define PPUTLX_49(...)   PPUTLX_11(PPUTLX_11(PPUTLX_11(PPUTLX_12(__VA_ARGS__))))
#define PPUTLX_48(...)   PPUTLX_11(PPUTLX_11(PPUTLX_11(PPUTLX_11(__VA_ARGS__))))
#define PPUTLX_47(...)   PPUTLX_10(PPUTLX_11(PPUTLX_11(PPUTLX_11(__VA_ARGS__))))
#define PPUTLX_46(...)   PPUTLX_10(PPUTLX_10(PPUTLX_11(PPUTLX_11(__VA_ARGS__))))
#define PPUTLX_45(...)   PPUTLX_10(PPUTLX_10(PPUTLX_10(PPUTLX_11(__VA_ARGS__))))
#define PPUTLX_44(...)   PPUTLX_10(PPUTLX_10(PPUTLX_10(PPUTLX_10(__VA_ARGS__))))
#define PPUTLX_43(...)   PPUTLX_9(PPUTLX_10(PPUTLX_10(PPUTLX_10(__VA_ARGS__))))
#define PPUTLX_42(...)   PPUTLX_9(PPUTLX_9(PPUTLX_10(PPUTLX_10(__VA_ARGS__))))
#define PPUTLX_41(...)   PPUTLX_9(PPUTLX_9(PPUTLX_9(PPUTLX_10(__VA_ARGS__))))
#define PPUTLX_40(...)   PPUTLX_9(PPUTLX_9(PPUTLX_9(PPUTLX_9(__VA_ARGS__))))
#define PPUTLX_39(...)   PPUTLX_8(PPUTLX_9(PPUTLX_9(PPUTLX_9(__VA_ARGS__))))
#define PPUTLX_38(...)   PPUTLX_8(PPUTLX_8(PPUTLX_9(PPUTLX_9(__VA_ARGS__))))
#define PPUTLX_37(...)   PPUTLX_8(PPUTLX_8(PPUTLX_8(PPUTLX_9(__VA_ARGS__))))
#define PPUTLX_36(...)   PPUTLX_8(PPUTLX_8(PPUTLX_8(PPUTLX_8(__VA_ARGS__))))
#define PPUTLX_35(...)   PPUTLX_7(PPUTLX_8(PPUTLX_8(PPUTLX_8(__VA_ARGS__))))
#define PPUTLX_34(...)   PPUTLX_7(PPUTLX_7(PPUTLX_8(PPUTLX_8(__VA_ARGS__))))
#define PPUTLX_33(...)   PPUTLX_7(PPUTLX_7(PPUTLX_7(PPUTLX_8(__VA_ARGS__))))
#define PPUTLX_32(...)   PPUTLX_7(PPUTLX_7(PPUTLX_7(PPUTLX_7(__VA_ARGS__))))
#define PPUTLX_31(...)   PPUTLX_6(PPUTLX_7(PPUTLX_7(PPUTLX_7(__VA_ARGS__))))
#define PPUTLX_30(...)   PPUTLX_6(PPUTLX_6(PPUTLX_7(PPUTLX_7(__VA_ARGS__))))
#define PPUTLX_29(...)   PPUTLX_6(PPUTLX_6(PPUTLX_6(PPUTLX_7(__VA_ARGS__))))
#define PPUTLX_28(...)   PPUTLX_6(PPUTLX_6(PPUTLX_6(PPUTLX_6(__VA_ARGS__))))
#define PPUTLX_27(...)   PPUTLX_5(PPUTLX_6(PPUTLX_6(PPUTLX_6(__VA_ARGS__))))
#define PPUTLX_26(...)   PPUTLX_5(PPUTLX_5(PPUTLX_6(PPUTLX_6(__VA_ARGS__))))
#define PPUTLX_25(...)   PPUTLX_5(PPUTLX_5(PPUTLX_5(PPUTLX_6(__VA_ARGS__))))
#define PPUTLX_24(...)   PPUTLX_5(PPUTLX_5(PPUTLX_5(PPUTLX_5(__VA_ARGS__))))
#define PPUTLX_23(...)   PPUTLX_4(PPUTLX_5(PPUTLX_5(PPUTLX_5(__VA_ARGS__))))
#define PPUTLX_22(...)   PPUTLX_4(PPUTLX_4(PPUTLX_5(PPUTLX_5(__VA_ARGS__))))
#define PPUTLX_21(...)   PPUTLX_4(PPUTLX_4(PPUTLX_4(PPUTLX_5(__VA_ARGS__))))
#define PPUTLX_20(...)   PPUTLX_4(PPUTLX_4(PPUTLX_4(PPUTLX_4(__VA_ARGS__))))
#define PPUTLX_19(...)   PPUTLX_3(PPUTLX_4(PPUTLX_4(PPUTLX_4(__VA_ARGS__))))
#define PPUTLX_18(...)   PPUTLX_3(PPUTLX_3(PPUTLX_4(PPUTLX_4(__VA_ARGS__))))
#define PPUTLX_17(...)   PPUTLX_3(PPUTLX_3(PPUTLX_3(PPUTLX_4(__VA_ARGS__))))
#define PPUTLX_16(...)   PPUTLX_3(PPUTLX_3(PPUTLX_3(PPUTLX_3(__VA_ARGS__))))
#define PPUTLX_15(...)   PPUTLX_2(PPUTLX_3(PPUTLX_3(PPUTLX_3(__VA_ARGS__))))
#define PPUTLX_14(...)   PPUTLX_2(PPUTLX_2(PPUTLX_3(PPUTLX_3(__VA_ARGS__))))
#define PPUTLX_13(...)   PPUTLX_2(PPUTLX_2(PPUTLX_2(PPUTLX_3(__VA_ARGS__))))
#define PPUTLX_12(...)   PPUTLX_2(PPUTLX_2(PPUTLX_2(PPUTLX_2(__VA_ARGS__))))
#define PPUTLX_11(...)   PPUTLX_1(PPUTLX_2(PPUTLX_2(PPUTLX_2(__VA_ARGS__))))
#define PPUTLX_10(...)   PPUTLX_1(PPUTLX_1(PPUTLX_2(PPUTLX_2(__VA_ARGS__))))
#define PPUTLX_9(...)    PPUTLX_1(PPUTLX_1(PPUTLX_1(PPUTLX_2(__VA_ARGS__))))
#define PPUTLX_8(...)    PPUTLX_1(PPUTLX_1(PPUTLX_1(PPUTLX_1(__VA_ARGS__))))
#define PPUTLX_7(...)    PPUTLX_0(PPUTLX_1(PPUTLX_1(PPUTLX_1(__VA_ARGS__))))
#define PPUTLX_6(...)    PPUTLX_0(PPUTLX_0(PPUTLX_1(PPUTLX_1(__VA_ARGS__))))
#define PPUTLX_5(...)    PPUTLX_0(PPUTLX_0(PPUTLX_0(PPUTLX_1(__VA_ARGS__))))
#define PPUTLX_4(...)    PPUTLX_0(PPUTLX_0(PPUTLX_0(PPUTLX_0(__VA_ARGS__))))
#define PPUTLX_3(...)    PPUTLX_0(PPUTLX_0(PPUTLX_0(__VA_ARGS__)))
#define PPUTLX_2(...)    PPUTLX_0(PPUTLX_0(__VA_ARGS__))
#define PPUTLX_1(...)    PPUTLX_0(__VA_ARGS__)
#define PPUTLX_0(...)    __VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [meta.paste]
/// ------------
/// pastes an expression by separating arguments with spaces.
///
/// PTL_PASTE()        // <nothing>
/// PTL_PASTE(a)       // a
/// PTL_PASTE(a, b)    // a b
/// PTL_PASTE(a, b, c) // a b c
#define PTL_PASTE(/* v: any... */...) /* -> v[0] v[1] ... */ \
  __VA_OPT__(PTL_ITEMS((PTL_X(PTL_SIZE(__VA_ARGS__))(PPUTLPASTE_A(__VA_ARGS__)))))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

#define PPUTLPASTE_B(_, ...) _ __VA_OPT__(PPUTLPASTE_A PTL_LP __VA_ARGS__ PTL_RP)
#define PPUTLPASTE_A(_, ...) _ __VA_OPT__(PPUTLPASTE_B PTL_LP __VA_ARGS__ PTL_RP)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [control.if]
/// ------------
/// conditionally expands items based on a boolean.
///
/// PTL_IF(1, (t), ())     // t
/// PTL_IF(0, (t), ())     // <nothing>
/// PTL_IF(1, (t), (f))    // t
/// PTL_IF(0, (t), (f))    // f
/// PTL_IF(1, (a), (b, c)) // a
/// PTL_IF(0, (a), (b, c)) // b, c
#define PTL_IF(/* b: bool, t: tup, f: tup */...) /* -> b ? ...t : ...f */ PPUTLIF_O(__VA_ARGS__)(__VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

/// first parentheses; chooses next function based on b
#define PPUTLIF_O(b, ...) PTL_CAT(PPUTLIF_OO_, PTL_BOOL(b))

/// second parentheses; true result
#define PPUTLIF_OO_1(b, t, f) PPUTLIF_OO_1_X(PTL_TUP(t))
#define PPUTLIF_OO_1_X(t)     PTL_ESC t

/// second parentheses; false result
#define PPUTLIF_OO_0(b, t, f) PPUTLIF_OO_0_X(PTL_TUP(f))
#define PPUTLIF_OO_0_X(f)     PTL_ESC f

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [control.switch]
/// ----------------
/// conditionally expands items based on a uint.
///
/// PTL_SWITCH(0, (1))              // 1
/// PTL_SWITCH(1, (1), (2))         // 2
/// PTL_SWITCH(2, (1), (2), (3, 4)) // 3, 4
#define PTL_SWITCH(/* cs: uint, cases: tup... */...) /* -> ...cases[cs] */ \
  PTL_X(PTL_FIRST(__VA_ARGS__))(PPUTLSWITCH_A(PTL_UINT(PTL_FIRST(__VA_ARGS__)))(__VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - {{{

/// mutually recursive branch selector B
#define PPUTLSWITCH_B(i) PTL_IF(PTL_EQZ(i), (PPUTLSWITCH_B_BREAK), (PPUTLSWITCH_B_CONT))

/// B branches
#define PPUTLSWITCH_B_BREAK(i, _, ...) PTL_ESC _
#define PPUTLSWITCH_B_CONT(i, _, ...)  PPUTLSWITCH_A PTL_LP PTL_DEC(i) PTL_RP(PTL_DEC(i), __VA_ARGS__)

/// mutually recursive branch selector A
#define PPUTLSWITCH_A(i) PTL_IF(PTL_EQZ(i), (PPUTLSWITCH_A_BREAK), (PPUTLSWITCH_A_CONT))

/// A branches
#define PPUTLSWITCH_A_BREAK(i, _, ...) PTL_ESC _
#define PPUTLSWITCH_A_CONT(i, _, ...)  PPUTLSWITCH_B PTL_LP PTL_DEC(i) PTL_RP(PTL_DEC(i), __VA_ARGS__)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - }}}

/// [logic.not]
/// -----------
/// logical NOT.
///
/// PTL_NOT(0) // 1
/// PTL_NOT(1) // 0
#define PTL_NOT(/* v: bool */...) /* -> bool{!v} */ PTL_IF(__VA_ARGS__, (0), (1))

/// [logic.and]
/// -----------
/// logical AND.
///
/// PTL_AND(0, 0) // 0
/// PTL_AND(0, 1) // 0
/// PTL_AND(1, 0) // 0
/// PTL_AND(1, 1) // 1
#define PTL_AND(/* a: bool, b: bool */...) /* -> bool{a and b} */ \
  PTL_IF(PTL_FIRST(__VA_ARGS__), (PTL_BOOL(PTL_REST(__VA_ARGS__))), (0))

/// [logic.or]
/// ----------
/// logical OR.
///
/// PTL_OR(0, 0) // 0
/// PTL_OR(0, 1) // 1
/// PTL_OR(1, 0) // 1
/// PTL_OR(1, 1) // 1
#define PTL_OR(/* a: bool, b: bool */...) /* -> bool{a or b} */ \
  PTL_IF(PTL_FIRST(__VA_ARGS__), (1), (PTL_BOOL(PTL_REST(__VA_ARGS__))))

/// [logic.nand]
/// ------------
/// logical NAND.
///
/// PTL_NAND(0, 0) // 1
/// PTL_NAND(0, 1) // 1
/// PTL_NAND(1, 0) // 1
/// PTL_NAND(1, 1) // 0
#define PTL_NAND(/* a: bool, b: bool */...) /* -> bool{!(a and b)} */ \
  PTL_IF(PTL_FIRST(__VA_ARGS__), (PTL_NOT(PTL_REST(__VA_ARGS__))), (1))

/// [logic.nor]
/// -----------
/// logical NOR.
///
/// PTL_NOR(0, 0) // 1
/// PTL_NOR(0, 1) // 0
/// PTL_NOR(1, 0) // 0
/// PTL_NOR(1, 1) // 0
#define PTL_NOR(/* a: bool, b: bool */...) /* -> bool{!(a or b)} */ \
  PTL_IF(PTL_FIRST(__VA_ARGS__), (0), (PTL_NOT(PTL_REST(__VA_ARGS__))))

/// [logic.xor]
/// -----------
/// logical XOR.
///
/// PTL_XOR(0, 0) // 0
/// PTL_XOR(0, 1) // 1
/// PTL_XOR(1, 0) // 1
/// PTL_XOR(1, 1) // 0
#define PTL_XOR(/* a: bool, b: bool */...) /* -> bool{a xor b} */ \
  PTL_IF(PTL_FIRST(__VA_ARGS__), (PTL_NOT(PTL_REST(__VA_ARGS__))), (PTL_BOOL(PTL_REST(__VA_ARGS__))))

/// [logic.xnor]
/// ------------
/// logical XNOR.
///
/// PTL_XNOR(0, 0) // 1
/// PTL_XNOR(0, 1) // 0
/// PTL_XNOR(1, 0) // 0
/// PTL_XNOR(1, 1) // 1
#define PTL_XNOR(/* a: bool, b: bool */...) /* -> bool{!(a xor b)} */ \
  PTL_IF(PTL_FIRST(__VA_ARGS__), (PTL_BOOL(PTL_REST(__VA_ARGS__))), (PTL_NOT(PTL_REST(__VA_ARGS__))))

// vim: fdm=marker:fmr={{{,}}}

#endif

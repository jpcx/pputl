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
//    the outputs of one may be used as inputs to another.                    //
//                                                                            //
//    Tuples are used only when necessary;  most functions that operate on    //
//    data ranges both input and output a variadic argument list. Creating    //
//    a tuple is trivial compared to extracting tuple items.                  //
//                                                                            //
//    Functions that parse or transform generic data are designed to allow    //
//    for algorithmic manipulation of preprocessor syntax. This means that    //
//    expansions  are applied  consistently  to all input elements and are    //
//    known,  documented  and tested.  This is necessary for several pputl    //
//    features including algo.recur and algo.reduce that use deferred left    //
//    and right parens to construct recursive statements.                     //
//                                                                            //
//    API functions are documented with the following shorthand syntax:       //
//                                                                            //
//      name({in: type}) [-<xct>]-> {out[: type]}                             //
//                                                                            //
//      name: name of the feature                                             //
//      in:   any number of input parameter names                             //
//      type: a shorthand type description                                    //
//      xct:  number of secondary expansions applied to generic inputs.       //
//            any generic args deferred past xct will not be expanded.        //
//      out:  name of an output; may be an input                              //
//            name with a shorthand transformation.                           //
//                                                                            //
//    Generic types are documented with "any" and are the only targets for    //
//    expansion documentation. Input parameter separations, variadic sizes    //
//    and non-generic types (e.g. uint) must be fully expressed during the    //
//    primary expansion.                                                      //
//                                                                            //
//    Library example [range.split]:                                          //
//                                                                            //
//      split(n: uint, args: any...) -<1>-> head: (...args[:n]),              //
//                                          tail: (...args[n-size:]),         //
//                                          type: bool                        //
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
#define PTL_ESC(/* args: any... */...) /* -> ...args */ __VA_ARGS__

/// [lang.str]
/// ----------
/// stringizes args after an expansion.
///
/// PTL_STR()         // ""
/// PTL_STR(foo, bar) // "foo, bar"
#define PTL_STR(/* args: any... */...) /* -> #...args */ PPUTLSTR_X(__VA_ARGS__)

//                                                                           {{{

#define PPUTLSTR_X(...) #__VA_ARGS__

//                                                                           }}}

/// [lang.cat]
/// ----------
/// concatenates two args after an expansion.
/// args must be compatible with the ## operator.
///
/// PTL_CAT(foo, bar)          // foobar
/// PTL_CAT(foo, PTL_EAT(bar)) // foo
#define PTL_CAT(/* a: any, b: any */...) /* -> a##b */ __VA_OPT__(PPUTLCAT_X(__VA_ARGS__))

//                                                                           {{{

#define PPUTLCAT_X(a, b) a##b

//                                                                           }}}

/// [lang.first]
/// ------------
/// returns the first argument.
///
/// PTL_FIRST()     // <nothing>
/// PTL_FIRST(, )   // <nothing>
/// PTL_FIRST(a)    // a
/// PTL_FIRST(a, b) // a
#define PTL_FIRST(/* _: any, args: any... */...) /* -> a */ __VA_OPT__(PPUTLFIRST_X(__VA_ARGS__))

//                                                                           {{{

#define PPUTLFIRST_X(_, ...) _

//                                                                           }}}

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
#define PTL_REST(/* _: any, args: any... */...) /* -> ...args */ __VA_OPT__(PPUTLREST_X(__VA_ARGS__))

//                                                                           {{{

#define PPUTLREST_X(_, ...) __VA_ARGS__

//                                                                           }}}

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

//                                                                           {{{

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

//                                                                           }}}

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

//                                                                           {{{

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

//                                                                           }}}

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

//                                                                           {{{

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
#define PPUTLUINT_OOO(...)          PPUTLUINT_OOO_RES(PPUTLUINT_NEXT_##__VA_ARGS__)
#define PPUTLUINT_OOO_RES(...)      PPUTLUINT_OOO_RES_X(__VA_ARGS__)
#define PPUTLUINT_OOO_RES_X(_, ...) PPUTLUINT_OOO_##__VA_OPT__(NO_)##FAIL()
#define PPUTLUINT_OOO_NO_FAIL(...)  PPUTLUINT_PASS
#define PPUTLUINT_OOO_FAIL(...)     PPUTLUINT_FAIL

/// fourth parentheses; returns
#define PPUTLUINT_PASS(...) __VA_ARGS__
#define PPUTLUINT_FAIL(...) PTL_UINT(__VA_ARGS__)

/// uint cycle macros. verifies uint validity if cat result has two values
#define PPUTLUINT_NEXT_1023 0, .
#define PPUTLUINT_NEXT_1022 1023, .
#define PPUTLUINT_NEXT_1021 1022, .
#define PPUTLUINT_NEXT_1020 1021, .
#define PPUTLUINT_NEXT_1019 1020, .
#define PPUTLUINT_NEXT_1018 1019, .
#define PPUTLUINT_NEXT_1017 1018, .
#define PPUTLUINT_NEXT_1016 1017, .
#define PPUTLUINT_NEXT_1015 1016, .
#define PPUTLUINT_NEXT_1014 1015, .
#define PPUTLUINT_NEXT_1013 1014, .
#define PPUTLUINT_NEXT_1012 1013, .
#define PPUTLUINT_NEXT_1011 1012, .
#define PPUTLUINT_NEXT_1010 1011, .
#define PPUTLUINT_NEXT_1009 1010, .
#define PPUTLUINT_NEXT_1008 1009, .
#define PPUTLUINT_NEXT_1007 1008, .
#define PPUTLUINT_NEXT_1006 1007, .
#define PPUTLUINT_NEXT_1005 1006, .
#define PPUTLUINT_NEXT_1004 1005, .
#define PPUTLUINT_NEXT_1003 1004, .
#define PPUTLUINT_NEXT_1002 1003, .
#define PPUTLUINT_NEXT_1001 1002, .
#define PPUTLUINT_NEXT_1000 1001, .
#define PPUTLUINT_NEXT_999  1000, .
#define PPUTLUINT_NEXT_998  999, .
#define PPUTLUINT_NEXT_997  998, .
#define PPUTLUINT_NEXT_996  997, .
#define PPUTLUINT_NEXT_995  996, .
#define PPUTLUINT_NEXT_994  995, .
#define PPUTLUINT_NEXT_993  994, .
#define PPUTLUINT_NEXT_992  993, .
#define PPUTLUINT_NEXT_991  992, .
#define PPUTLUINT_NEXT_990  991, .
#define PPUTLUINT_NEXT_989  990, .
#define PPUTLUINT_NEXT_988  989, .
#define PPUTLUINT_NEXT_987  988, .
#define PPUTLUINT_NEXT_986  987, .
#define PPUTLUINT_NEXT_985  986, .
#define PPUTLUINT_NEXT_984  985, .
#define PPUTLUINT_NEXT_983  984, .
#define PPUTLUINT_NEXT_982  983, .
#define PPUTLUINT_NEXT_981  982, .
#define PPUTLUINT_NEXT_980  981, .
#define PPUTLUINT_NEXT_979  980, .
#define PPUTLUINT_NEXT_978  979, .
#define PPUTLUINT_NEXT_977  978, .
#define PPUTLUINT_NEXT_976  977, .
#define PPUTLUINT_NEXT_975  976, .
#define PPUTLUINT_NEXT_974  975, .
#define PPUTLUINT_NEXT_973  974, .
#define PPUTLUINT_NEXT_972  973, .
#define PPUTLUINT_NEXT_971  972, .
#define PPUTLUINT_NEXT_970  971, .
#define PPUTLUINT_NEXT_969  970, .
#define PPUTLUINT_NEXT_968  969, .
#define PPUTLUINT_NEXT_967  968, .
#define PPUTLUINT_NEXT_966  967, .
#define PPUTLUINT_NEXT_965  966, .
#define PPUTLUINT_NEXT_964  965, .
#define PPUTLUINT_NEXT_963  964, .
#define PPUTLUINT_NEXT_962  963, .
#define PPUTLUINT_NEXT_961  962, .
#define PPUTLUINT_NEXT_960  961, .
#define PPUTLUINT_NEXT_959  960, .
#define PPUTLUINT_NEXT_958  959, .
#define PPUTLUINT_NEXT_957  958, .
#define PPUTLUINT_NEXT_956  957, .
#define PPUTLUINT_NEXT_955  956, .
#define PPUTLUINT_NEXT_954  955, .
#define PPUTLUINT_NEXT_953  954, .
#define PPUTLUINT_NEXT_952  953, .
#define PPUTLUINT_NEXT_951  952, .
#define PPUTLUINT_NEXT_950  951, .
#define PPUTLUINT_NEXT_949  950, .
#define PPUTLUINT_NEXT_948  949, .
#define PPUTLUINT_NEXT_947  948, .
#define PPUTLUINT_NEXT_946  947, .
#define PPUTLUINT_NEXT_945  946, .
#define PPUTLUINT_NEXT_944  945, .
#define PPUTLUINT_NEXT_943  944, .
#define PPUTLUINT_NEXT_942  943, .
#define PPUTLUINT_NEXT_941  942, .
#define PPUTLUINT_NEXT_940  941, .
#define PPUTLUINT_NEXT_939  940, .
#define PPUTLUINT_NEXT_938  939, .
#define PPUTLUINT_NEXT_937  938, .
#define PPUTLUINT_NEXT_936  937, .
#define PPUTLUINT_NEXT_935  936, .
#define PPUTLUINT_NEXT_934  935, .
#define PPUTLUINT_NEXT_933  934, .
#define PPUTLUINT_NEXT_932  933, .
#define PPUTLUINT_NEXT_931  932, .
#define PPUTLUINT_NEXT_930  931, .
#define PPUTLUINT_NEXT_929  930, .
#define PPUTLUINT_NEXT_928  929, .
#define PPUTLUINT_NEXT_927  928, .
#define PPUTLUINT_NEXT_926  927, .
#define PPUTLUINT_NEXT_925  926, .
#define PPUTLUINT_NEXT_924  925, .
#define PPUTLUINT_NEXT_923  924, .
#define PPUTLUINT_NEXT_922  923, .
#define PPUTLUINT_NEXT_921  922, .
#define PPUTLUINT_NEXT_920  921, .
#define PPUTLUINT_NEXT_919  920, .
#define PPUTLUINT_NEXT_918  919, .
#define PPUTLUINT_NEXT_917  918, .
#define PPUTLUINT_NEXT_916  917, .
#define PPUTLUINT_NEXT_915  916, .
#define PPUTLUINT_NEXT_914  915, .
#define PPUTLUINT_NEXT_913  914, .
#define PPUTLUINT_NEXT_912  913, .
#define PPUTLUINT_NEXT_911  912, .
#define PPUTLUINT_NEXT_910  911, .
#define PPUTLUINT_NEXT_909  910, .
#define PPUTLUINT_NEXT_908  909, .
#define PPUTLUINT_NEXT_907  908, .
#define PPUTLUINT_NEXT_906  907, .
#define PPUTLUINT_NEXT_905  906, .
#define PPUTLUINT_NEXT_904  905, .
#define PPUTLUINT_NEXT_903  904, .
#define PPUTLUINT_NEXT_902  903, .
#define PPUTLUINT_NEXT_901  902, .
#define PPUTLUINT_NEXT_900  901, .
#define PPUTLUINT_NEXT_899  900, .
#define PPUTLUINT_NEXT_898  899, .
#define PPUTLUINT_NEXT_897  898, .
#define PPUTLUINT_NEXT_896  897, .
#define PPUTLUINT_NEXT_895  896, .
#define PPUTLUINT_NEXT_894  895, .
#define PPUTLUINT_NEXT_893  894, .
#define PPUTLUINT_NEXT_892  893, .
#define PPUTLUINT_NEXT_891  892, .
#define PPUTLUINT_NEXT_890  891, .
#define PPUTLUINT_NEXT_889  890, .
#define PPUTLUINT_NEXT_888  889, .
#define PPUTLUINT_NEXT_887  888, .
#define PPUTLUINT_NEXT_886  887, .
#define PPUTLUINT_NEXT_885  886, .
#define PPUTLUINT_NEXT_884  885, .
#define PPUTLUINT_NEXT_883  884, .
#define PPUTLUINT_NEXT_882  883, .
#define PPUTLUINT_NEXT_881  882, .
#define PPUTLUINT_NEXT_880  881, .
#define PPUTLUINT_NEXT_879  880, .
#define PPUTLUINT_NEXT_878  879, .
#define PPUTLUINT_NEXT_877  878, .
#define PPUTLUINT_NEXT_876  877, .
#define PPUTLUINT_NEXT_875  876, .
#define PPUTLUINT_NEXT_874  875, .
#define PPUTLUINT_NEXT_873  874, .
#define PPUTLUINT_NEXT_872  873, .
#define PPUTLUINT_NEXT_871  872, .
#define PPUTLUINT_NEXT_870  871, .
#define PPUTLUINT_NEXT_869  870, .
#define PPUTLUINT_NEXT_868  869, .
#define PPUTLUINT_NEXT_867  868, .
#define PPUTLUINT_NEXT_866  867, .
#define PPUTLUINT_NEXT_865  866, .
#define PPUTLUINT_NEXT_864  865, .
#define PPUTLUINT_NEXT_863  864, .
#define PPUTLUINT_NEXT_862  863, .
#define PPUTLUINT_NEXT_861  862, .
#define PPUTLUINT_NEXT_860  861, .
#define PPUTLUINT_NEXT_859  860, .
#define PPUTLUINT_NEXT_858  859, .
#define PPUTLUINT_NEXT_857  858, .
#define PPUTLUINT_NEXT_856  857, .
#define PPUTLUINT_NEXT_855  856, .
#define PPUTLUINT_NEXT_854  855, .
#define PPUTLUINT_NEXT_853  854, .
#define PPUTLUINT_NEXT_852  853, .
#define PPUTLUINT_NEXT_851  852, .
#define PPUTLUINT_NEXT_850  851, .
#define PPUTLUINT_NEXT_849  850, .
#define PPUTLUINT_NEXT_848  849, .
#define PPUTLUINT_NEXT_847  848, .
#define PPUTLUINT_NEXT_846  847, .
#define PPUTLUINT_NEXT_845  846, .
#define PPUTLUINT_NEXT_844  845, .
#define PPUTLUINT_NEXT_843  844, .
#define PPUTLUINT_NEXT_842  843, .
#define PPUTLUINT_NEXT_841  842, .
#define PPUTLUINT_NEXT_840  841, .
#define PPUTLUINT_NEXT_839  840, .
#define PPUTLUINT_NEXT_838  839, .
#define PPUTLUINT_NEXT_837  838, .
#define PPUTLUINT_NEXT_836  837, .
#define PPUTLUINT_NEXT_835  836, .
#define PPUTLUINT_NEXT_834  835, .
#define PPUTLUINT_NEXT_833  834, .
#define PPUTLUINT_NEXT_832  833, .
#define PPUTLUINT_NEXT_831  832, .
#define PPUTLUINT_NEXT_830  831, .
#define PPUTLUINT_NEXT_829  830, .
#define PPUTLUINT_NEXT_828  829, .
#define PPUTLUINT_NEXT_827  828, .
#define PPUTLUINT_NEXT_826  827, .
#define PPUTLUINT_NEXT_825  826, .
#define PPUTLUINT_NEXT_824  825, .
#define PPUTLUINT_NEXT_823  824, .
#define PPUTLUINT_NEXT_822  823, .
#define PPUTLUINT_NEXT_821  822, .
#define PPUTLUINT_NEXT_820  821, .
#define PPUTLUINT_NEXT_819  820, .
#define PPUTLUINT_NEXT_818  819, .
#define PPUTLUINT_NEXT_817  818, .
#define PPUTLUINT_NEXT_816  817, .
#define PPUTLUINT_NEXT_815  816, .
#define PPUTLUINT_NEXT_814  815, .
#define PPUTLUINT_NEXT_813  814, .
#define PPUTLUINT_NEXT_812  813, .
#define PPUTLUINT_NEXT_811  812, .
#define PPUTLUINT_NEXT_810  811, .
#define PPUTLUINT_NEXT_809  810, .
#define PPUTLUINT_NEXT_808  809, .
#define PPUTLUINT_NEXT_807  808, .
#define PPUTLUINT_NEXT_806  807, .
#define PPUTLUINT_NEXT_805  806, .
#define PPUTLUINT_NEXT_804  805, .
#define PPUTLUINT_NEXT_803  804, .
#define PPUTLUINT_NEXT_802  803, .
#define PPUTLUINT_NEXT_801  802, .
#define PPUTLUINT_NEXT_800  801, .
#define PPUTLUINT_NEXT_799  800, .
#define PPUTLUINT_NEXT_798  799, .
#define PPUTLUINT_NEXT_797  798, .
#define PPUTLUINT_NEXT_796  797, .
#define PPUTLUINT_NEXT_795  796, .
#define PPUTLUINT_NEXT_794  795, .
#define PPUTLUINT_NEXT_793  794, .
#define PPUTLUINT_NEXT_792  793, .
#define PPUTLUINT_NEXT_791  792, .
#define PPUTLUINT_NEXT_790  791, .
#define PPUTLUINT_NEXT_789  790, .
#define PPUTLUINT_NEXT_788  789, .
#define PPUTLUINT_NEXT_787  788, .
#define PPUTLUINT_NEXT_786  787, .
#define PPUTLUINT_NEXT_785  786, .
#define PPUTLUINT_NEXT_784  785, .
#define PPUTLUINT_NEXT_783  784, .
#define PPUTLUINT_NEXT_782  783, .
#define PPUTLUINT_NEXT_781  782, .
#define PPUTLUINT_NEXT_780  781, .
#define PPUTLUINT_NEXT_779  780, .
#define PPUTLUINT_NEXT_778  779, .
#define PPUTLUINT_NEXT_777  778, .
#define PPUTLUINT_NEXT_776  777, .
#define PPUTLUINT_NEXT_775  776, .
#define PPUTLUINT_NEXT_774  775, .
#define PPUTLUINT_NEXT_773  774, .
#define PPUTLUINT_NEXT_772  773, .
#define PPUTLUINT_NEXT_771  772, .
#define PPUTLUINT_NEXT_770  771, .
#define PPUTLUINT_NEXT_769  770, .
#define PPUTLUINT_NEXT_768  769, .
#define PPUTLUINT_NEXT_767  768, .
#define PPUTLUINT_NEXT_766  767, .
#define PPUTLUINT_NEXT_765  766, .
#define PPUTLUINT_NEXT_764  765, .
#define PPUTLUINT_NEXT_763  764, .
#define PPUTLUINT_NEXT_762  763, .
#define PPUTLUINT_NEXT_761  762, .
#define PPUTLUINT_NEXT_760  761, .
#define PPUTLUINT_NEXT_759  760, .
#define PPUTLUINT_NEXT_758  759, .
#define PPUTLUINT_NEXT_757  758, .
#define PPUTLUINT_NEXT_756  757, .
#define PPUTLUINT_NEXT_755  756, .
#define PPUTLUINT_NEXT_754  755, .
#define PPUTLUINT_NEXT_753  754, .
#define PPUTLUINT_NEXT_752  753, .
#define PPUTLUINT_NEXT_751  752, .
#define PPUTLUINT_NEXT_750  751, .
#define PPUTLUINT_NEXT_749  750, .
#define PPUTLUINT_NEXT_748  749, .
#define PPUTLUINT_NEXT_747  748, .
#define PPUTLUINT_NEXT_746  747, .
#define PPUTLUINT_NEXT_745  746, .
#define PPUTLUINT_NEXT_744  745, .
#define PPUTLUINT_NEXT_743  744, .
#define PPUTLUINT_NEXT_742  743, .
#define PPUTLUINT_NEXT_741  742, .
#define PPUTLUINT_NEXT_740  741, .
#define PPUTLUINT_NEXT_739  740, .
#define PPUTLUINT_NEXT_738  739, .
#define PPUTLUINT_NEXT_737  738, .
#define PPUTLUINT_NEXT_736  737, .
#define PPUTLUINT_NEXT_735  736, .
#define PPUTLUINT_NEXT_734  735, .
#define PPUTLUINT_NEXT_733  734, .
#define PPUTLUINT_NEXT_732  733, .
#define PPUTLUINT_NEXT_731  732, .
#define PPUTLUINT_NEXT_730  731, .
#define PPUTLUINT_NEXT_729  730, .
#define PPUTLUINT_NEXT_728  729, .
#define PPUTLUINT_NEXT_727  728, .
#define PPUTLUINT_NEXT_726  727, .
#define PPUTLUINT_NEXT_725  726, .
#define PPUTLUINT_NEXT_724  725, .
#define PPUTLUINT_NEXT_723  724, .
#define PPUTLUINT_NEXT_722  723, .
#define PPUTLUINT_NEXT_721  722, .
#define PPUTLUINT_NEXT_720  721, .
#define PPUTLUINT_NEXT_719  720, .
#define PPUTLUINT_NEXT_718  719, .
#define PPUTLUINT_NEXT_717  718, .
#define PPUTLUINT_NEXT_716  717, .
#define PPUTLUINT_NEXT_715  716, .
#define PPUTLUINT_NEXT_714  715, .
#define PPUTLUINT_NEXT_713  714, .
#define PPUTLUINT_NEXT_712  713, .
#define PPUTLUINT_NEXT_711  712, .
#define PPUTLUINT_NEXT_710  711, .
#define PPUTLUINT_NEXT_709  710, .
#define PPUTLUINT_NEXT_708  709, .
#define PPUTLUINT_NEXT_707  708, .
#define PPUTLUINT_NEXT_706  707, .
#define PPUTLUINT_NEXT_705  706, .
#define PPUTLUINT_NEXT_704  705, .
#define PPUTLUINT_NEXT_703  704, .
#define PPUTLUINT_NEXT_702  703, .
#define PPUTLUINT_NEXT_701  702, .
#define PPUTLUINT_NEXT_700  701, .
#define PPUTLUINT_NEXT_699  700, .
#define PPUTLUINT_NEXT_698  699, .
#define PPUTLUINT_NEXT_697  698, .
#define PPUTLUINT_NEXT_696  697, .
#define PPUTLUINT_NEXT_695  696, .
#define PPUTLUINT_NEXT_694  695, .
#define PPUTLUINT_NEXT_693  694, .
#define PPUTLUINT_NEXT_692  693, .
#define PPUTLUINT_NEXT_691  692, .
#define PPUTLUINT_NEXT_690  691, .
#define PPUTLUINT_NEXT_689  690, .
#define PPUTLUINT_NEXT_688  689, .
#define PPUTLUINT_NEXT_687  688, .
#define PPUTLUINT_NEXT_686  687, .
#define PPUTLUINT_NEXT_685  686, .
#define PPUTLUINT_NEXT_684  685, .
#define PPUTLUINT_NEXT_683  684, .
#define PPUTLUINT_NEXT_682  683, .
#define PPUTLUINT_NEXT_681  682, .
#define PPUTLUINT_NEXT_680  681, .
#define PPUTLUINT_NEXT_679  680, .
#define PPUTLUINT_NEXT_678  679, .
#define PPUTLUINT_NEXT_677  678, .
#define PPUTLUINT_NEXT_676  677, .
#define PPUTLUINT_NEXT_675  676, .
#define PPUTLUINT_NEXT_674  675, .
#define PPUTLUINT_NEXT_673  674, .
#define PPUTLUINT_NEXT_672  673, .
#define PPUTLUINT_NEXT_671  672, .
#define PPUTLUINT_NEXT_670  671, .
#define PPUTLUINT_NEXT_669  670, .
#define PPUTLUINT_NEXT_668  669, .
#define PPUTLUINT_NEXT_667  668, .
#define PPUTLUINT_NEXT_666  667, .
#define PPUTLUINT_NEXT_665  666, .
#define PPUTLUINT_NEXT_664  665, .
#define PPUTLUINT_NEXT_663  664, .
#define PPUTLUINT_NEXT_662  663, .
#define PPUTLUINT_NEXT_661  662, .
#define PPUTLUINT_NEXT_660  661, .
#define PPUTLUINT_NEXT_659  660, .
#define PPUTLUINT_NEXT_658  659, .
#define PPUTLUINT_NEXT_657  658, .
#define PPUTLUINT_NEXT_656  657, .
#define PPUTLUINT_NEXT_655  656, .
#define PPUTLUINT_NEXT_654  655, .
#define PPUTLUINT_NEXT_653  654, .
#define PPUTLUINT_NEXT_652  653, .
#define PPUTLUINT_NEXT_651  652, .
#define PPUTLUINT_NEXT_650  651, .
#define PPUTLUINT_NEXT_649  650, .
#define PPUTLUINT_NEXT_648  649, .
#define PPUTLUINT_NEXT_647  648, .
#define PPUTLUINT_NEXT_646  647, .
#define PPUTLUINT_NEXT_645  646, .
#define PPUTLUINT_NEXT_644  645, .
#define PPUTLUINT_NEXT_643  644, .
#define PPUTLUINT_NEXT_642  643, .
#define PPUTLUINT_NEXT_641  642, .
#define PPUTLUINT_NEXT_640  641, .
#define PPUTLUINT_NEXT_639  640, .
#define PPUTLUINT_NEXT_638  639, .
#define PPUTLUINT_NEXT_637  638, .
#define PPUTLUINT_NEXT_636  637, .
#define PPUTLUINT_NEXT_635  636, .
#define PPUTLUINT_NEXT_634  635, .
#define PPUTLUINT_NEXT_633  634, .
#define PPUTLUINT_NEXT_632  633, .
#define PPUTLUINT_NEXT_631  632, .
#define PPUTLUINT_NEXT_630  631, .
#define PPUTLUINT_NEXT_629  630, .
#define PPUTLUINT_NEXT_628  629, .
#define PPUTLUINT_NEXT_627  628, .
#define PPUTLUINT_NEXT_626  627, .
#define PPUTLUINT_NEXT_625  626, .
#define PPUTLUINT_NEXT_624  625, .
#define PPUTLUINT_NEXT_623  624, .
#define PPUTLUINT_NEXT_622  623, .
#define PPUTLUINT_NEXT_621  622, .
#define PPUTLUINT_NEXT_620  621, .
#define PPUTLUINT_NEXT_619  620, .
#define PPUTLUINT_NEXT_618  619, .
#define PPUTLUINT_NEXT_617  618, .
#define PPUTLUINT_NEXT_616  617, .
#define PPUTLUINT_NEXT_615  616, .
#define PPUTLUINT_NEXT_614  615, .
#define PPUTLUINT_NEXT_613  614, .
#define PPUTLUINT_NEXT_612  613, .
#define PPUTLUINT_NEXT_611  612, .
#define PPUTLUINT_NEXT_610  611, .
#define PPUTLUINT_NEXT_609  610, .
#define PPUTLUINT_NEXT_608  609, .
#define PPUTLUINT_NEXT_607  608, .
#define PPUTLUINT_NEXT_606  607, .
#define PPUTLUINT_NEXT_605  606, .
#define PPUTLUINT_NEXT_604  605, .
#define PPUTLUINT_NEXT_603  604, .
#define PPUTLUINT_NEXT_602  603, .
#define PPUTLUINT_NEXT_601  602, .
#define PPUTLUINT_NEXT_600  601, .
#define PPUTLUINT_NEXT_599  600, .
#define PPUTLUINT_NEXT_598  599, .
#define PPUTLUINT_NEXT_597  598, .
#define PPUTLUINT_NEXT_596  597, .
#define PPUTLUINT_NEXT_595  596, .
#define PPUTLUINT_NEXT_594  595, .
#define PPUTLUINT_NEXT_593  594, .
#define PPUTLUINT_NEXT_592  593, .
#define PPUTLUINT_NEXT_591  592, .
#define PPUTLUINT_NEXT_590  591, .
#define PPUTLUINT_NEXT_589  590, .
#define PPUTLUINT_NEXT_588  589, .
#define PPUTLUINT_NEXT_587  588, .
#define PPUTLUINT_NEXT_586  587, .
#define PPUTLUINT_NEXT_585  586, .
#define PPUTLUINT_NEXT_584  585, .
#define PPUTLUINT_NEXT_583  584, .
#define PPUTLUINT_NEXT_582  583, .
#define PPUTLUINT_NEXT_581  582, .
#define PPUTLUINT_NEXT_580  581, .
#define PPUTLUINT_NEXT_579  580, .
#define PPUTLUINT_NEXT_578  579, .
#define PPUTLUINT_NEXT_577  578, .
#define PPUTLUINT_NEXT_576  577, .
#define PPUTLUINT_NEXT_575  576, .
#define PPUTLUINT_NEXT_574  575, .
#define PPUTLUINT_NEXT_573  574, .
#define PPUTLUINT_NEXT_572  573, .
#define PPUTLUINT_NEXT_571  572, .
#define PPUTLUINT_NEXT_570  571, .
#define PPUTLUINT_NEXT_569  570, .
#define PPUTLUINT_NEXT_568  569, .
#define PPUTLUINT_NEXT_567  568, .
#define PPUTLUINT_NEXT_566  567, .
#define PPUTLUINT_NEXT_565  566, .
#define PPUTLUINT_NEXT_564  565, .
#define PPUTLUINT_NEXT_563  564, .
#define PPUTLUINT_NEXT_562  563, .
#define PPUTLUINT_NEXT_561  562, .
#define PPUTLUINT_NEXT_560  561, .
#define PPUTLUINT_NEXT_559  560, .
#define PPUTLUINT_NEXT_558  559, .
#define PPUTLUINT_NEXT_557  558, .
#define PPUTLUINT_NEXT_556  557, .
#define PPUTLUINT_NEXT_555  556, .
#define PPUTLUINT_NEXT_554  555, .
#define PPUTLUINT_NEXT_553  554, .
#define PPUTLUINT_NEXT_552  553, .
#define PPUTLUINT_NEXT_551  552, .
#define PPUTLUINT_NEXT_550  551, .
#define PPUTLUINT_NEXT_549  550, .
#define PPUTLUINT_NEXT_548  549, .
#define PPUTLUINT_NEXT_547  548, .
#define PPUTLUINT_NEXT_546  547, .
#define PPUTLUINT_NEXT_545  546, .
#define PPUTLUINT_NEXT_544  545, .
#define PPUTLUINT_NEXT_543  544, .
#define PPUTLUINT_NEXT_542  543, .
#define PPUTLUINT_NEXT_541  542, .
#define PPUTLUINT_NEXT_540  541, .
#define PPUTLUINT_NEXT_539  540, .
#define PPUTLUINT_NEXT_538  539, .
#define PPUTLUINT_NEXT_537  538, .
#define PPUTLUINT_NEXT_536  537, .
#define PPUTLUINT_NEXT_535  536, .
#define PPUTLUINT_NEXT_534  535, .
#define PPUTLUINT_NEXT_533  534, .
#define PPUTLUINT_NEXT_532  533, .
#define PPUTLUINT_NEXT_531  532, .
#define PPUTLUINT_NEXT_530  531, .
#define PPUTLUINT_NEXT_529  530, .
#define PPUTLUINT_NEXT_528  529, .
#define PPUTLUINT_NEXT_527  528, .
#define PPUTLUINT_NEXT_526  527, .
#define PPUTLUINT_NEXT_525  526, .
#define PPUTLUINT_NEXT_524  525, .
#define PPUTLUINT_NEXT_523  524, .
#define PPUTLUINT_NEXT_522  523, .
#define PPUTLUINT_NEXT_521  522, .
#define PPUTLUINT_NEXT_520  521, .
#define PPUTLUINT_NEXT_519  520, .
#define PPUTLUINT_NEXT_518  519, .
#define PPUTLUINT_NEXT_517  518, .
#define PPUTLUINT_NEXT_516  517, .
#define PPUTLUINT_NEXT_515  516, .
#define PPUTLUINT_NEXT_514  515, .
#define PPUTLUINT_NEXT_513  514, .
#define PPUTLUINT_NEXT_512  513, .
#define PPUTLUINT_NEXT_511  512, .
#define PPUTLUINT_NEXT_510  511, .
#define PPUTLUINT_NEXT_509  510, .
#define PPUTLUINT_NEXT_508  509, .
#define PPUTLUINT_NEXT_507  508, .
#define PPUTLUINT_NEXT_506  507, .
#define PPUTLUINT_NEXT_505  506, .
#define PPUTLUINT_NEXT_504  505, .
#define PPUTLUINT_NEXT_503  504, .
#define PPUTLUINT_NEXT_502  503, .
#define PPUTLUINT_NEXT_501  502, .
#define PPUTLUINT_NEXT_500  501, .
#define PPUTLUINT_NEXT_499  500, .
#define PPUTLUINT_NEXT_498  499, .
#define PPUTLUINT_NEXT_497  498, .
#define PPUTLUINT_NEXT_496  497, .
#define PPUTLUINT_NEXT_495  496, .
#define PPUTLUINT_NEXT_494  495, .
#define PPUTLUINT_NEXT_493  494, .
#define PPUTLUINT_NEXT_492  493, .
#define PPUTLUINT_NEXT_491  492, .
#define PPUTLUINT_NEXT_490  491, .
#define PPUTLUINT_NEXT_489  490, .
#define PPUTLUINT_NEXT_488  489, .
#define PPUTLUINT_NEXT_487  488, .
#define PPUTLUINT_NEXT_486  487, .
#define PPUTLUINT_NEXT_485  486, .
#define PPUTLUINT_NEXT_484  485, .
#define PPUTLUINT_NEXT_483  484, .
#define PPUTLUINT_NEXT_482  483, .
#define PPUTLUINT_NEXT_481  482, .
#define PPUTLUINT_NEXT_480  481, .
#define PPUTLUINT_NEXT_479  480, .
#define PPUTLUINT_NEXT_478  479, .
#define PPUTLUINT_NEXT_477  478, .
#define PPUTLUINT_NEXT_476  477, .
#define PPUTLUINT_NEXT_475  476, .
#define PPUTLUINT_NEXT_474  475, .
#define PPUTLUINT_NEXT_473  474, .
#define PPUTLUINT_NEXT_472  473, .
#define PPUTLUINT_NEXT_471  472, .
#define PPUTLUINT_NEXT_470  471, .
#define PPUTLUINT_NEXT_469  470, .
#define PPUTLUINT_NEXT_468  469, .
#define PPUTLUINT_NEXT_467  468, .
#define PPUTLUINT_NEXT_466  467, .
#define PPUTLUINT_NEXT_465  466, .
#define PPUTLUINT_NEXT_464  465, .
#define PPUTLUINT_NEXT_463  464, .
#define PPUTLUINT_NEXT_462  463, .
#define PPUTLUINT_NEXT_461  462, .
#define PPUTLUINT_NEXT_460  461, .
#define PPUTLUINT_NEXT_459  460, .
#define PPUTLUINT_NEXT_458  459, .
#define PPUTLUINT_NEXT_457  458, .
#define PPUTLUINT_NEXT_456  457, .
#define PPUTLUINT_NEXT_455  456, .
#define PPUTLUINT_NEXT_454  455, .
#define PPUTLUINT_NEXT_453  454, .
#define PPUTLUINT_NEXT_452  453, .
#define PPUTLUINT_NEXT_451  452, .
#define PPUTLUINT_NEXT_450  451, .
#define PPUTLUINT_NEXT_449  450, .
#define PPUTLUINT_NEXT_448  449, .
#define PPUTLUINT_NEXT_447  448, .
#define PPUTLUINT_NEXT_446  447, .
#define PPUTLUINT_NEXT_445  446, .
#define PPUTLUINT_NEXT_444  445, .
#define PPUTLUINT_NEXT_443  444, .
#define PPUTLUINT_NEXT_442  443, .
#define PPUTLUINT_NEXT_441  442, .
#define PPUTLUINT_NEXT_440  441, .
#define PPUTLUINT_NEXT_439  440, .
#define PPUTLUINT_NEXT_438  439, .
#define PPUTLUINT_NEXT_437  438, .
#define PPUTLUINT_NEXT_436  437, .
#define PPUTLUINT_NEXT_435  436, .
#define PPUTLUINT_NEXT_434  435, .
#define PPUTLUINT_NEXT_433  434, .
#define PPUTLUINT_NEXT_432  433, .
#define PPUTLUINT_NEXT_431  432, .
#define PPUTLUINT_NEXT_430  431, .
#define PPUTLUINT_NEXT_429  430, .
#define PPUTLUINT_NEXT_428  429, .
#define PPUTLUINT_NEXT_427  428, .
#define PPUTLUINT_NEXT_426  427, .
#define PPUTLUINT_NEXT_425  426, .
#define PPUTLUINT_NEXT_424  425, .
#define PPUTLUINT_NEXT_423  424, .
#define PPUTLUINT_NEXT_422  423, .
#define PPUTLUINT_NEXT_421  422, .
#define PPUTLUINT_NEXT_420  421, .
#define PPUTLUINT_NEXT_419  420, .
#define PPUTLUINT_NEXT_418  419, .
#define PPUTLUINT_NEXT_417  418, .
#define PPUTLUINT_NEXT_416  417, .
#define PPUTLUINT_NEXT_415  416, .
#define PPUTLUINT_NEXT_414  415, .
#define PPUTLUINT_NEXT_413  414, .
#define PPUTLUINT_NEXT_412  413, .
#define PPUTLUINT_NEXT_411  412, .
#define PPUTLUINT_NEXT_410  411, .
#define PPUTLUINT_NEXT_409  410, .
#define PPUTLUINT_NEXT_408  409, .
#define PPUTLUINT_NEXT_407  408, .
#define PPUTLUINT_NEXT_406  407, .
#define PPUTLUINT_NEXT_405  406, .
#define PPUTLUINT_NEXT_404  405, .
#define PPUTLUINT_NEXT_403  404, .
#define PPUTLUINT_NEXT_402  403, .
#define PPUTLUINT_NEXT_401  402, .
#define PPUTLUINT_NEXT_400  401, .
#define PPUTLUINT_NEXT_399  400, .
#define PPUTLUINT_NEXT_398  399, .
#define PPUTLUINT_NEXT_397  398, .
#define PPUTLUINT_NEXT_396  397, .
#define PPUTLUINT_NEXT_395  396, .
#define PPUTLUINT_NEXT_394  395, .
#define PPUTLUINT_NEXT_393  394, .
#define PPUTLUINT_NEXT_392  393, .
#define PPUTLUINT_NEXT_391  392, .
#define PPUTLUINT_NEXT_390  391, .
#define PPUTLUINT_NEXT_389  390, .
#define PPUTLUINT_NEXT_388  389, .
#define PPUTLUINT_NEXT_387  388, .
#define PPUTLUINT_NEXT_386  387, .
#define PPUTLUINT_NEXT_385  386, .
#define PPUTLUINT_NEXT_384  385, .
#define PPUTLUINT_NEXT_383  384, .
#define PPUTLUINT_NEXT_382  383, .
#define PPUTLUINT_NEXT_381  382, .
#define PPUTLUINT_NEXT_380  381, .
#define PPUTLUINT_NEXT_379  380, .
#define PPUTLUINT_NEXT_378  379, .
#define PPUTLUINT_NEXT_377  378, .
#define PPUTLUINT_NEXT_376  377, .
#define PPUTLUINT_NEXT_375  376, .
#define PPUTLUINT_NEXT_374  375, .
#define PPUTLUINT_NEXT_373  374, .
#define PPUTLUINT_NEXT_372  373, .
#define PPUTLUINT_NEXT_371  372, .
#define PPUTLUINT_NEXT_370  371, .
#define PPUTLUINT_NEXT_369  370, .
#define PPUTLUINT_NEXT_368  369, .
#define PPUTLUINT_NEXT_367  368, .
#define PPUTLUINT_NEXT_366  367, .
#define PPUTLUINT_NEXT_365  366, .
#define PPUTLUINT_NEXT_364  365, .
#define PPUTLUINT_NEXT_363  364, .
#define PPUTLUINT_NEXT_362  363, .
#define PPUTLUINT_NEXT_361  362, .
#define PPUTLUINT_NEXT_360  361, .
#define PPUTLUINT_NEXT_359  360, .
#define PPUTLUINT_NEXT_358  359, .
#define PPUTLUINT_NEXT_357  358, .
#define PPUTLUINT_NEXT_356  357, .
#define PPUTLUINT_NEXT_355  356, .
#define PPUTLUINT_NEXT_354  355, .
#define PPUTLUINT_NEXT_353  354, .
#define PPUTLUINT_NEXT_352  353, .
#define PPUTLUINT_NEXT_351  352, .
#define PPUTLUINT_NEXT_350  351, .
#define PPUTLUINT_NEXT_349  350, .
#define PPUTLUINT_NEXT_348  349, .
#define PPUTLUINT_NEXT_347  348, .
#define PPUTLUINT_NEXT_346  347, .
#define PPUTLUINT_NEXT_345  346, .
#define PPUTLUINT_NEXT_344  345, .
#define PPUTLUINT_NEXT_343  344, .
#define PPUTLUINT_NEXT_342  343, .
#define PPUTLUINT_NEXT_341  342, .
#define PPUTLUINT_NEXT_340  341, .
#define PPUTLUINT_NEXT_339  340, .
#define PPUTLUINT_NEXT_338  339, .
#define PPUTLUINT_NEXT_337  338, .
#define PPUTLUINT_NEXT_336  337, .
#define PPUTLUINT_NEXT_335  336, .
#define PPUTLUINT_NEXT_334  335, .
#define PPUTLUINT_NEXT_333  334, .
#define PPUTLUINT_NEXT_332  333, .
#define PPUTLUINT_NEXT_331  332, .
#define PPUTLUINT_NEXT_330  331, .
#define PPUTLUINT_NEXT_329  330, .
#define PPUTLUINT_NEXT_328  329, .
#define PPUTLUINT_NEXT_327  328, .
#define PPUTLUINT_NEXT_326  327, .
#define PPUTLUINT_NEXT_325  326, .
#define PPUTLUINT_NEXT_324  325, .
#define PPUTLUINT_NEXT_323  324, .
#define PPUTLUINT_NEXT_322  323, .
#define PPUTLUINT_NEXT_321  322, .
#define PPUTLUINT_NEXT_320  321, .
#define PPUTLUINT_NEXT_319  320, .
#define PPUTLUINT_NEXT_318  319, .
#define PPUTLUINT_NEXT_317  318, .
#define PPUTLUINT_NEXT_316  317, .
#define PPUTLUINT_NEXT_315  316, .
#define PPUTLUINT_NEXT_314  315, .
#define PPUTLUINT_NEXT_313  314, .
#define PPUTLUINT_NEXT_312  313, .
#define PPUTLUINT_NEXT_311  312, .
#define PPUTLUINT_NEXT_310  311, .
#define PPUTLUINT_NEXT_309  310, .
#define PPUTLUINT_NEXT_308  309, .
#define PPUTLUINT_NEXT_307  308, .
#define PPUTLUINT_NEXT_306  307, .
#define PPUTLUINT_NEXT_305  306, .
#define PPUTLUINT_NEXT_304  305, .
#define PPUTLUINT_NEXT_303  304, .
#define PPUTLUINT_NEXT_302  303, .
#define PPUTLUINT_NEXT_301  302, .
#define PPUTLUINT_NEXT_300  301, .
#define PPUTLUINT_NEXT_299  300, .
#define PPUTLUINT_NEXT_298  299, .
#define PPUTLUINT_NEXT_297  298, .
#define PPUTLUINT_NEXT_296  297, .
#define PPUTLUINT_NEXT_295  296, .
#define PPUTLUINT_NEXT_294  295, .
#define PPUTLUINT_NEXT_293  294, .
#define PPUTLUINT_NEXT_292  293, .
#define PPUTLUINT_NEXT_291  292, .
#define PPUTLUINT_NEXT_290  291, .
#define PPUTLUINT_NEXT_289  290, .
#define PPUTLUINT_NEXT_288  289, .
#define PPUTLUINT_NEXT_287  288, .
#define PPUTLUINT_NEXT_286  287, .
#define PPUTLUINT_NEXT_285  286, .
#define PPUTLUINT_NEXT_284  285, .
#define PPUTLUINT_NEXT_283  284, .
#define PPUTLUINT_NEXT_282  283, .
#define PPUTLUINT_NEXT_281  282, .
#define PPUTLUINT_NEXT_280  281, .
#define PPUTLUINT_NEXT_279  280, .
#define PPUTLUINT_NEXT_278  279, .
#define PPUTLUINT_NEXT_277  278, .
#define PPUTLUINT_NEXT_276  277, .
#define PPUTLUINT_NEXT_275  276, .
#define PPUTLUINT_NEXT_274  275, .
#define PPUTLUINT_NEXT_273  274, .
#define PPUTLUINT_NEXT_272  273, .
#define PPUTLUINT_NEXT_271  272, .
#define PPUTLUINT_NEXT_270  271, .
#define PPUTLUINT_NEXT_269  270, .
#define PPUTLUINT_NEXT_268  269, .
#define PPUTLUINT_NEXT_267  268, .
#define PPUTLUINT_NEXT_266  267, .
#define PPUTLUINT_NEXT_265  266, .
#define PPUTLUINT_NEXT_264  265, .
#define PPUTLUINT_NEXT_263  264, .
#define PPUTLUINT_NEXT_262  263, .
#define PPUTLUINT_NEXT_261  262, .
#define PPUTLUINT_NEXT_260  261, .
#define PPUTLUINT_NEXT_259  260, .
#define PPUTLUINT_NEXT_258  259, .
#define PPUTLUINT_NEXT_257  258, .
#define PPUTLUINT_NEXT_256  257, .
#define PPUTLUINT_NEXT_255  256, .
#define PPUTLUINT_NEXT_254  255, .
#define PPUTLUINT_NEXT_253  254, .
#define PPUTLUINT_NEXT_252  253, .
#define PPUTLUINT_NEXT_251  252, .
#define PPUTLUINT_NEXT_250  251, .
#define PPUTLUINT_NEXT_249  250, .
#define PPUTLUINT_NEXT_248  249, .
#define PPUTLUINT_NEXT_247  248, .
#define PPUTLUINT_NEXT_246  247, .
#define PPUTLUINT_NEXT_245  246, .
#define PPUTLUINT_NEXT_244  245, .
#define PPUTLUINT_NEXT_243  244, .
#define PPUTLUINT_NEXT_242  243, .
#define PPUTLUINT_NEXT_241  242, .
#define PPUTLUINT_NEXT_240  241, .
#define PPUTLUINT_NEXT_239  240, .
#define PPUTLUINT_NEXT_238  239, .
#define PPUTLUINT_NEXT_237  238, .
#define PPUTLUINT_NEXT_236  237, .
#define PPUTLUINT_NEXT_235  236, .
#define PPUTLUINT_NEXT_234  235, .
#define PPUTLUINT_NEXT_233  234, .
#define PPUTLUINT_NEXT_232  233, .
#define PPUTLUINT_NEXT_231  232, .
#define PPUTLUINT_NEXT_230  231, .
#define PPUTLUINT_NEXT_229  230, .
#define PPUTLUINT_NEXT_228  229, .
#define PPUTLUINT_NEXT_227  228, .
#define PPUTLUINT_NEXT_226  227, .
#define PPUTLUINT_NEXT_225  226, .
#define PPUTLUINT_NEXT_224  225, .
#define PPUTLUINT_NEXT_223  224, .
#define PPUTLUINT_NEXT_222  223, .
#define PPUTLUINT_NEXT_221  222, .
#define PPUTLUINT_NEXT_220  221, .
#define PPUTLUINT_NEXT_219  220, .
#define PPUTLUINT_NEXT_218  219, .
#define PPUTLUINT_NEXT_217  218, .
#define PPUTLUINT_NEXT_216  217, .
#define PPUTLUINT_NEXT_215  216, .
#define PPUTLUINT_NEXT_214  215, .
#define PPUTLUINT_NEXT_213  214, .
#define PPUTLUINT_NEXT_212  213, .
#define PPUTLUINT_NEXT_211  212, .
#define PPUTLUINT_NEXT_210  211, .
#define PPUTLUINT_NEXT_209  210, .
#define PPUTLUINT_NEXT_208  209, .
#define PPUTLUINT_NEXT_207  208, .
#define PPUTLUINT_NEXT_206  207, .
#define PPUTLUINT_NEXT_205  206, .
#define PPUTLUINT_NEXT_204  205, .
#define PPUTLUINT_NEXT_203  204, .
#define PPUTLUINT_NEXT_202  203, .
#define PPUTLUINT_NEXT_201  202, .
#define PPUTLUINT_NEXT_200  201, .
#define PPUTLUINT_NEXT_199  200, .
#define PPUTLUINT_NEXT_198  199, .
#define PPUTLUINT_NEXT_197  198, .
#define PPUTLUINT_NEXT_196  197, .
#define PPUTLUINT_NEXT_195  196, .
#define PPUTLUINT_NEXT_194  195, .
#define PPUTLUINT_NEXT_193  194, .
#define PPUTLUINT_NEXT_192  193, .
#define PPUTLUINT_NEXT_191  192, .
#define PPUTLUINT_NEXT_190  191, .
#define PPUTLUINT_NEXT_189  190, .
#define PPUTLUINT_NEXT_188  189, .
#define PPUTLUINT_NEXT_187  188, .
#define PPUTLUINT_NEXT_186  187, .
#define PPUTLUINT_NEXT_185  186, .
#define PPUTLUINT_NEXT_184  185, .
#define PPUTLUINT_NEXT_183  184, .
#define PPUTLUINT_NEXT_182  183, .
#define PPUTLUINT_NEXT_181  182, .
#define PPUTLUINT_NEXT_180  181, .
#define PPUTLUINT_NEXT_179  180, .
#define PPUTLUINT_NEXT_178  179, .
#define PPUTLUINT_NEXT_177  178, .
#define PPUTLUINT_NEXT_176  177, .
#define PPUTLUINT_NEXT_175  176, .
#define PPUTLUINT_NEXT_174  175, .
#define PPUTLUINT_NEXT_173  174, .
#define PPUTLUINT_NEXT_172  173, .
#define PPUTLUINT_NEXT_171  172, .
#define PPUTLUINT_NEXT_170  171, .
#define PPUTLUINT_NEXT_169  170, .
#define PPUTLUINT_NEXT_168  169, .
#define PPUTLUINT_NEXT_167  168, .
#define PPUTLUINT_NEXT_166  167, .
#define PPUTLUINT_NEXT_165  166, .
#define PPUTLUINT_NEXT_164  165, .
#define PPUTLUINT_NEXT_163  164, .
#define PPUTLUINT_NEXT_162  163, .
#define PPUTLUINT_NEXT_161  162, .
#define PPUTLUINT_NEXT_160  161, .
#define PPUTLUINT_NEXT_159  160, .
#define PPUTLUINT_NEXT_158  159, .
#define PPUTLUINT_NEXT_157  158, .
#define PPUTLUINT_NEXT_156  157, .
#define PPUTLUINT_NEXT_155  156, .
#define PPUTLUINT_NEXT_154  155, .
#define PPUTLUINT_NEXT_153  154, .
#define PPUTLUINT_NEXT_152  153, .
#define PPUTLUINT_NEXT_151  152, .
#define PPUTLUINT_NEXT_150  151, .
#define PPUTLUINT_NEXT_149  150, .
#define PPUTLUINT_NEXT_148  149, .
#define PPUTLUINT_NEXT_147  148, .
#define PPUTLUINT_NEXT_146  147, .
#define PPUTLUINT_NEXT_145  146, .
#define PPUTLUINT_NEXT_144  145, .
#define PPUTLUINT_NEXT_143  144, .
#define PPUTLUINT_NEXT_142  143, .
#define PPUTLUINT_NEXT_141  142, .
#define PPUTLUINT_NEXT_140  141, .
#define PPUTLUINT_NEXT_139  140, .
#define PPUTLUINT_NEXT_138  139, .
#define PPUTLUINT_NEXT_137  138, .
#define PPUTLUINT_NEXT_136  137, .
#define PPUTLUINT_NEXT_135  136, .
#define PPUTLUINT_NEXT_134  135, .
#define PPUTLUINT_NEXT_133  134, .
#define PPUTLUINT_NEXT_132  133, .
#define PPUTLUINT_NEXT_131  132, .
#define PPUTLUINT_NEXT_130  131, .
#define PPUTLUINT_NEXT_129  130, .
#define PPUTLUINT_NEXT_128  129, .
#define PPUTLUINT_NEXT_127  128, .
#define PPUTLUINT_NEXT_126  127, .
#define PPUTLUINT_NEXT_125  126, .
#define PPUTLUINT_NEXT_124  125, .
#define PPUTLUINT_NEXT_123  124, .
#define PPUTLUINT_NEXT_122  123, .
#define PPUTLUINT_NEXT_121  122, .
#define PPUTLUINT_NEXT_120  121, .
#define PPUTLUINT_NEXT_119  120, .
#define PPUTLUINT_NEXT_118  119, .
#define PPUTLUINT_NEXT_117  118, .
#define PPUTLUINT_NEXT_116  117, .
#define PPUTLUINT_NEXT_115  116, .
#define PPUTLUINT_NEXT_114  115, .
#define PPUTLUINT_NEXT_113  114, .
#define PPUTLUINT_NEXT_112  113, .
#define PPUTLUINT_NEXT_111  112, .
#define PPUTLUINT_NEXT_110  111, .
#define PPUTLUINT_NEXT_109  110, .
#define PPUTLUINT_NEXT_108  109, .
#define PPUTLUINT_NEXT_107  108, .
#define PPUTLUINT_NEXT_106  107, .
#define PPUTLUINT_NEXT_105  106, .
#define PPUTLUINT_NEXT_104  105, .
#define PPUTLUINT_NEXT_103  104, .
#define PPUTLUINT_NEXT_102  103, .
#define PPUTLUINT_NEXT_101  102, .
#define PPUTLUINT_NEXT_100  101, .
#define PPUTLUINT_NEXT_99   100, .
#define PPUTLUINT_NEXT_98   99, .
#define PPUTLUINT_NEXT_97   98, .
#define PPUTLUINT_NEXT_96   97, .
#define PPUTLUINT_NEXT_95   96, .
#define PPUTLUINT_NEXT_94   95, .
#define PPUTLUINT_NEXT_93   94, .
#define PPUTLUINT_NEXT_92   93, .
#define PPUTLUINT_NEXT_91   92, .
#define PPUTLUINT_NEXT_90   91, .
#define PPUTLUINT_NEXT_89   90, .
#define PPUTLUINT_NEXT_88   89, .
#define PPUTLUINT_NEXT_87   88, .
#define PPUTLUINT_NEXT_86   87, .
#define PPUTLUINT_NEXT_85   86, .
#define PPUTLUINT_NEXT_84   85, .
#define PPUTLUINT_NEXT_83   84, .
#define PPUTLUINT_NEXT_82   83, .
#define PPUTLUINT_NEXT_81   82, .
#define PPUTLUINT_NEXT_80   81, .
#define PPUTLUINT_NEXT_79   80, .
#define PPUTLUINT_NEXT_78   79, .
#define PPUTLUINT_NEXT_77   78, .
#define PPUTLUINT_NEXT_76   77, .
#define PPUTLUINT_NEXT_75   76, .
#define PPUTLUINT_NEXT_74   75, .
#define PPUTLUINT_NEXT_73   74, .
#define PPUTLUINT_NEXT_72   73, .
#define PPUTLUINT_NEXT_71   72, .
#define PPUTLUINT_NEXT_70   71, .
#define PPUTLUINT_NEXT_69   70, .
#define PPUTLUINT_NEXT_68   69, .
#define PPUTLUINT_NEXT_67   68, .
#define PPUTLUINT_NEXT_66   67, .
#define PPUTLUINT_NEXT_65   66, .
#define PPUTLUINT_NEXT_64   65, .
#define PPUTLUINT_NEXT_63   64, .
#define PPUTLUINT_NEXT_62   63, .
#define PPUTLUINT_NEXT_61   62, .
#define PPUTLUINT_NEXT_60   61, .
#define PPUTLUINT_NEXT_59   60, .
#define PPUTLUINT_NEXT_58   59, .
#define PPUTLUINT_NEXT_57   58, .
#define PPUTLUINT_NEXT_56   57, .
#define PPUTLUINT_NEXT_55   56, .
#define PPUTLUINT_NEXT_54   55, .
#define PPUTLUINT_NEXT_53   54, .
#define PPUTLUINT_NEXT_52   53, .
#define PPUTLUINT_NEXT_51   52, .
#define PPUTLUINT_NEXT_50   51, .
#define PPUTLUINT_NEXT_49   50, .
#define PPUTLUINT_NEXT_48   49, .
#define PPUTLUINT_NEXT_47   48, .
#define PPUTLUINT_NEXT_46   47, .
#define PPUTLUINT_NEXT_45   46, .
#define PPUTLUINT_NEXT_44   45, .
#define PPUTLUINT_NEXT_43   44, .
#define PPUTLUINT_NEXT_42   43, .
#define PPUTLUINT_NEXT_41   42, .
#define PPUTLUINT_NEXT_40   41, .
#define PPUTLUINT_NEXT_39   40, .
#define PPUTLUINT_NEXT_38   39, .
#define PPUTLUINT_NEXT_37   38, .
#define PPUTLUINT_NEXT_36   37, .
#define PPUTLUINT_NEXT_35   36, .
#define PPUTLUINT_NEXT_34   35, .
#define PPUTLUINT_NEXT_33   34, .
#define PPUTLUINT_NEXT_32   33, .
#define PPUTLUINT_NEXT_31   32, .
#define PPUTLUINT_NEXT_30   31, .
#define PPUTLUINT_NEXT_29   30, .
#define PPUTLUINT_NEXT_28   29, .
#define PPUTLUINT_NEXT_27   28, .
#define PPUTLUINT_NEXT_26   27, .
#define PPUTLUINT_NEXT_25   26, .
#define PPUTLUINT_NEXT_24   25, .
#define PPUTLUINT_NEXT_23   24, .
#define PPUTLUINT_NEXT_22   23, .
#define PPUTLUINT_NEXT_21   22, .
#define PPUTLUINT_NEXT_20   21, .
#define PPUTLUINT_NEXT_19   20, .
#define PPUTLUINT_NEXT_18   19, .
#define PPUTLUINT_NEXT_17   18, .
#define PPUTLUINT_NEXT_16   17, .
#define PPUTLUINT_NEXT_15   16, .
#define PPUTLUINT_NEXT_14   15, .
#define PPUTLUINT_NEXT_13   14, .
#define PPUTLUINT_NEXT_12   13, .
#define PPUTLUINT_NEXT_11   12, .
#define PPUTLUINT_NEXT_10   11, .
#define PPUTLUINT_NEXT_9    10, .
#define PPUTLUINT_NEXT_8    9, .
#define PPUTLUINT_NEXT_7    8, .
#define PPUTLUINT_NEXT_6    7, .
#define PPUTLUINT_NEXT_5    6, .
#define PPUTLUINT_NEXT_4    5, .
#define PPUTLUINT_NEXT_3    4, .
#define PPUTLUINT_NEXT_2    3, .
#define PPUTLUINT_NEXT_1    2, .
#define PPUTLUINT_NEXT_0    1, .
#define PPUTLUINT_PREV_1023 1022, .
#define PPUTLUINT_PREV_1022 1021, .
#define PPUTLUINT_PREV_1021 1020, .
#define PPUTLUINT_PREV_1020 1019, .
#define PPUTLUINT_PREV_1019 1018, .
#define PPUTLUINT_PREV_1018 1017, .
#define PPUTLUINT_PREV_1017 1016, .
#define PPUTLUINT_PREV_1016 1015, .
#define PPUTLUINT_PREV_1015 1014, .
#define PPUTLUINT_PREV_1014 1013, .
#define PPUTLUINT_PREV_1013 1012, .
#define PPUTLUINT_PREV_1012 1011, .
#define PPUTLUINT_PREV_1011 1010, .
#define PPUTLUINT_PREV_1010 1009, .
#define PPUTLUINT_PREV_1009 1008, .
#define PPUTLUINT_PREV_1008 1007, .
#define PPUTLUINT_PREV_1007 1006, .
#define PPUTLUINT_PREV_1006 1005, .
#define PPUTLUINT_PREV_1005 1004, .
#define PPUTLUINT_PREV_1004 1003, .
#define PPUTLUINT_PREV_1003 1002, .
#define PPUTLUINT_PREV_1002 1001, .
#define PPUTLUINT_PREV_1001 1000, .
#define PPUTLUINT_PREV_1000 999, .
#define PPUTLUINT_PREV_999  998, .
#define PPUTLUINT_PREV_998  997, .
#define PPUTLUINT_PREV_997  996, .
#define PPUTLUINT_PREV_996  995, .
#define PPUTLUINT_PREV_995  994, .
#define PPUTLUINT_PREV_994  993, .
#define PPUTLUINT_PREV_993  992, .
#define PPUTLUINT_PREV_992  991, .
#define PPUTLUINT_PREV_991  990, .
#define PPUTLUINT_PREV_990  989, .
#define PPUTLUINT_PREV_989  988, .
#define PPUTLUINT_PREV_988  987, .
#define PPUTLUINT_PREV_987  986, .
#define PPUTLUINT_PREV_986  985, .
#define PPUTLUINT_PREV_985  984, .
#define PPUTLUINT_PREV_984  983, .
#define PPUTLUINT_PREV_983  982, .
#define PPUTLUINT_PREV_982  981, .
#define PPUTLUINT_PREV_981  980, .
#define PPUTLUINT_PREV_980  979, .
#define PPUTLUINT_PREV_979  978, .
#define PPUTLUINT_PREV_978  977, .
#define PPUTLUINT_PREV_977  976, .
#define PPUTLUINT_PREV_976  975, .
#define PPUTLUINT_PREV_975  974, .
#define PPUTLUINT_PREV_974  973, .
#define PPUTLUINT_PREV_973  972, .
#define PPUTLUINT_PREV_972  971, .
#define PPUTLUINT_PREV_971  970, .
#define PPUTLUINT_PREV_970  969, .
#define PPUTLUINT_PREV_969  968, .
#define PPUTLUINT_PREV_968  967, .
#define PPUTLUINT_PREV_967  966, .
#define PPUTLUINT_PREV_966  965, .
#define PPUTLUINT_PREV_965  964, .
#define PPUTLUINT_PREV_964  963, .
#define PPUTLUINT_PREV_963  962, .
#define PPUTLUINT_PREV_962  961, .
#define PPUTLUINT_PREV_961  960, .
#define PPUTLUINT_PREV_960  959, .
#define PPUTLUINT_PREV_959  958, .
#define PPUTLUINT_PREV_958  957, .
#define PPUTLUINT_PREV_957  956, .
#define PPUTLUINT_PREV_956  955, .
#define PPUTLUINT_PREV_955  954, .
#define PPUTLUINT_PREV_954  953, .
#define PPUTLUINT_PREV_953  952, .
#define PPUTLUINT_PREV_952  951, .
#define PPUTLUINT_PREV_951  950, .
#define PPUTLUINT_PREV_950  949, .
#define PPUTLUINT_PREV_949  948, .
#define PPUTLUINT_PREV_948  947, .
#define PPUTLUINT_PREV_947  946, .
#define PPUTLUINT_PREV_946  945, .
#define PPUTLUINT_PREV_945  944, .
#define PPUTLUINT_PREV_944  943, .
#define PPUTLUINT_PREV_943  942, .
#define PPUTLUINT_PREV_942  941, .
#define PPUTLUINT_PREV_941  940, .
#define PPUTLUINT_PREV_940  939, .
#define PPUTLUINT_PREV_939  938, .
#define PPUTLUINT_PREV_938  937, .
#define PPUTLUINT_PREV_937  936, .
#define PPUTLUINT_PREV_936  935, .
#define PPUTLUINT_PREV_935  934, .
#define PPUTLUINT_PREV_934  933, .
#define PPUTLUINT_PREV_933  932, .
#define PPUTLUINT_PREV_932  931, .
#define PPUTLUINT_PREV_931  930, .
#define PPUTLUINT_PREV_930  929, .
#define PPUTLUINT_PREV_929  928, .
#define PPUTLUINT_PREV_928  927, .
#define PPUTLUINT_PREV_927  926, .
#define PPUTLUINT_PREV_926  925, .
#define PPUTLUINT_PREV_925  924, .
#define PPUTLUINT_PREV_924  923, .
#define PPUTLUINT_PREV_923  922, .
#define PPUTLUINT_PREV_922  921, .
#define PPUTLUINT_PREV_921  920, .
#define PPUTLUINT_PREV_920  919, .
#define PPUTLUINT_PREV_919  918, .
#define PPUTLUINT_PREV_918  917, .
#define PPUTLUINT_PREV_917  916, .
#define PPUTLUINT_PREV_916  915, .
#define PPUTLUINT_PREV_915  914, .
#define PPUTLUINT_PREV_914  913, .
#define PPUTLUINT_PREV_913  912, .
#define PPUTLUINT_PREV_912  911, .
#define PPUTLUINT_PREV_911  910, .
#define PPUTLUINT_PREV_910  909, .
#define PPUTLUINT_PREV_909  908, .
#define PPUTLUINT_PREV_908  907, .
#define PPUTLUINT_PREV_907  906, .
#define PPUTLUINT_PREV_906  905, .
#define PPUTLUINT_PREV_905  904, .
#define PPUTLUINT_PREV_904  903, .
#define PPUTLUINT_PREV_903  902, .
#define PPUTLUINT_PREV_902  901, .
#define PPUTLUINT_PREV_901  900, .
#define PPUTLUINT_PREV_900  899, .
#define PPUTLUINT_PREV_899  898, .
#define PPUTLUINT_PREV_898  897, .
#define PPUTLUINT_PREV_897  896, .
#define PPUTLUINT_PREV_896  895, .
#define PPUTLUINT_PREV_895  894, .
#define PPUTLUINT_PREV_894  893, .
#define PPUTLUINT_PREV_893  892, .
#define PPUTLUINT_PREV_892  891, .
#define PPUTLUINT_PREV_891  890, .
#define PPUTLUINT_PREV_890  889, .
#define PPUTLUINT_PREV_889  888, .
#define PPUTLUINT_PREV_888  887, .
#define PPUTLUINT_PREV_887  886, .
#define PPUTLUINT_PREV_886  885, .
#define PPUTLUINT_PREV_885  884, .
#define PPUTLUINT_PREV_884  883, .
#define PPUTLUINT_PREV_883  882, .
#define PPUTLUINT_PREV_882  881, .
#define PPUTLUINT_PREV_881  880, .
#define PPUTLUINT_PREV_880  879, .
#define PPUTLUINT_PREV_879  878, .
#define PPUTLUINT_PREV_878  877, .
#define PPUTLUINT_PREV_877  876, .
#define PPUTLUINT_PREV_876  875, .
#define PPUTLUINT_PREV_875  874, .
#define PPUTLUINT_PREV_874  873, .
#define PPUTLUINT_PREV_873  872, .
#define PPUTLUINT_PREV_872  871, .
#define PPUTLUINT_PREV_871  870, .
#define PPUTLUINT_PREV_870  869, .
#define PPUTLUINT_PREV_869  868, .
#define PPUTLUINT_PREV_868  867, .
#define PPUTLUINT_PREV_867  866, .
#define PPUTLUINT_PREV_866  865, .
#define PPUTLUINT_PREV_865  864, .
#define PPUTLUINT_PREV_864  863, .
#define PPUTLUINT_PREV_863  862, .
#define PPUTLUINT_PREV_862  861, .
#define PPUTLUINT_PREV_861  860, .
#define PPUTLUINT_PREV_860  859, .
#define PPUTLUINT_PREV_859  858, .
#define PPUTLUINT_PREV_858  857, .
#define PPUTLUINT_PREV_857  856, .
#define PPUTLUINT_PREV_856  855, .
#define PPUTLUINT_PREV_855  854, .
#define PPUTLUINT_PREV_854  853, .
#define PPUTLUINT_PREV_853  852, .
#define PPUTLUINT_PREV_852  851, .
#define PPUTLUINT_PREV_851  850, .
#define PPUTLUINT_PREV_850  849, .
#define PPUTLUINT_PREV_849  848, .
#define PPUTLUINT_PREV_848  847, .
#define PPUTLUINT_PREV_847  846, .
#define PPUTLUINT_PREV_846  845, .
#define PPUTLUINT_PREV_845  844, .
#define PPUTLUINT_PREV_844  843, .
#define PPUTLUINT_PREV_843  842, .
#define PPUTLUINT_PREV_842  841, .
#define PPUTLUINT_PREV_841  840, .
#define PPUTLUINT_PREV_840  839, .
#define PPUTLUINT_PREV_839  838, .
#define PPUTLUINT_PREV_838  837, .
#define PPUTLUINT_PREV_837  836, .
#define PPUTLUINT_PREV_836  835, .
#define PPUTLUINT_PREV_835  834, .
#define PPUTLUINT_PREV_834  833, .
#define PPUTLUINT_PREV_833  832, .
#define PPUTLUINT_PREV_832  831, .
#define PPUTLUINT_PREV_831  830, .
#define PPUTLUINT_PREV_830  829, .
#define PPUTLUINT_PREV_829  828, .
#define PPUTLUINT_PREV_828  827, .
#define PPUTLUINT_PREV_827  826, .
#define PPUTLUINT_PREV_826  825, .
#define PPUTLUINT_PREV_825  824, .
#define PPUTLUINT_PREV_824  823, .
#define PPUTLUINT_PREV_823  822, .
#define PPUTLUINT_PREV_822  821, .
#define PPUTLUINT_PREV_821  820, .
#define PPUTLUINT_PREV_820  819, .
#define PPUTLUINT_PREV_819  818, .
#define PPUTLUINT_PREV_818  817, .
#define PPUTLUINT_PREV_817  816, .
#define PPUTLUINT_PREV_816  815, .
#define PPUTLUINT_PREV_815  814, .
#define PPUTLUINT_PREV_814  813, .
#define PPUTLUINT_PREV_813  812, .
#define PPUTLUINT_PREV_812  811, .
#define PPUTLUINT_PREV_811  810, .
#define PPUTLUINT_PREV_810  809, .
#define PPUTLUINT_PREV_809  808, .
#define PPUTLUINT_PREV_808  807, .
#define PPUTLUINT_PREV_807  806, .
#define PPUTLUINT_PREV_806  805, .
#define PPUTLUINT_PREV_805  804, .
#define PPUTLUINT_PREV_804  803, .
#define PPUTLUINT_PREV_803  802, .
#define PPUTLUINT_PREV_802  801, .
#define PPUTLUINT_PREV_801  800, .
#define PPUTLUINT_PREV_800  799, .
#define PPUTLUINT_PREV_799  798, .
#define PPUTLUINT_PREV_798  797, .
#define PPUTLUINT_PREV_797  796, .
#define PPUTLUINT_PREV_796  795, .
#define PPUTLUINT_PREV_795  794, .
#define PPUTLUINT_PREV_794  793, .
#define PPUTLUINT_PREV_793  792, .
#define PPUTLUINT_PREV_792  791, .
#define PPUTLUINT_PREV_791  790, .
#define PPUTLUINT_PREV_790  789, .
#define PPUTLUINT_PREV_789  788, .
#define PPUTLUINT_PREV_788  787, .
#define PPUTLUINT_PREV_787  786, .
#define PPUTLUINT_PREV_786  785, .
#define PPUTLUINT_PREV_785  784, .
#define PPUTLUINT_PREV_784  783, .
#define PPUTLUINT_PREV_783  782, .
#define PPUTLUINT_PREV_782  781, .
#define PPUTLUINT_PREV_781  780, .
#define PPUTLUINT_PREV_780  779, .
#define PPUTLUINT_PREV_779  778, .
#define PPUTLUINT_PREV_778  777, .
#define PPUTLUINT_PREV_777  776, .
#define PPUTLUINT_PREV_776  775, .
#define PPUTLUINT_PREV_775  774, .
#define PPUTLUINT_PREV_774  773, .
#define PPUTLUINT_PREV_773  772, .
#define PPUTLUINT_PREV_772  771, .
#define PPUTLUINT_PREV_771  770, .
#define PPUTLUINT_PREV_770  769, .
#define PPUTLUINT_PREV_769  768, .
#define PPUTLUINT_PREV_768  767, .
#define PPUTLUINT_PREV_767  766, .
#define PPUTLUINT_PREV_766  765, .
#define PPUTLUINT_PREV_765  764, .
#define PPUTLUINT_PREV_764  763, .
#define PPUTLUINT_PREV_763  762, .
#define PPUTLUINT_PREV_762  761, .
#define PPUTLUINT_PREV_761  760, .
#define PPUTLUINT_PREV_760  759, .
#define PPUTLUINT_PREV_759  758, .
#define PPUTLUINT_PREV_758  757, .
#define PPUTLUINT_PREV_757  756, .
#define PPUTLUINT_PREV_756  755, .
#define PPUTLUINT_PREV_755  754, .
#define PPUTLUINT_PREV_754  753, .
#define PPUTLUINT_PREV_753  752, .
#define PPUTLUINT_PREV_752  751, .
#define PPUTLUINT_PREV_751  750, .
#define PPUTLUINT_PREV_750  749, .
#define PPUTLUINT_PREV_749  748, .
#define PPUTLUINT_PREV_748  747, .
#define PPUTLUINT_PREV_747  746, .
#define PPUTLUINT_PREV_746  745, .
#define PPUTLUINT_PREV_745  744, .
#define PPUTLUINT_PREV_744  743, .
#define PPUTLUINT_PREV_743  742, .
#define PPUTLUINT_PREV_742  741, .
#define PPUTLUINT_PREV_741  740, .
#define PPUTLUINT_PREV_740  739, .
#define PPUTLUINT_PREV_739  738, .
#define PPUTLUINT_PREV_738  737, .
#define PPUTLUINT_PREV_737  736, .
#define PPUTLUINT_PREV_736  735, .
#define PPUTLUINT_PREV_735  734, .
#define PPUTLUINT_PREV_734  733, .
#define PPUTLUINT_PREV_733  732, .
#define PPUTLUINT_PREV_732  731, .
#define PPUTLUINT_PREV_731  730, .
#define PPUTLUINT_PREV_730  729, .
#define PPUTLUINT_PREV_729  728, .
#define PPUTLUINT_PREV_728  727, .
#define PPUTLUINT_PREV_727  726, .
#define PPUTLUINT_PREV_726  725, .
#define PPUTLUINT_PREV_725  724, .
#define PPUTLUINT_PREV_724  723, .
#define PPUTLUINT_PREV_723  722, .
#define PPUTLUINT_PREV_722  721, .
#define PPUTLUINT_PREV_721  720, .
#define PPUTLUINT_PREV_720  719, .
#define PPUTLUINT_PREV_719  718, .
#define PPUTLUINT_PREV_718  717, .
#define PPUTLUINT_PREV_717  716, .
#define PPUTLUINT_PREV_716  715, .
#define PPUTLUINT_PREV_715  714, .
#define PPUTLUINT_PREV_714  713, .
#define PPUTLUINT_PREV_713  712, .
#define PPUTLUINT_PREV_712  711, .
#define PPUTLUINT_PREV_711  710, .
#define PPUTLUINT_PREV_710  709, .
#define PPUTLUINT_PREV_709  708, .
#define PPUTLUINT_PREV_708  707, .
#define PPUTLUINT_PREV_707  706, .
#define PPUTLUINT_PREV_706  705, .
#define PPUTLUINT_PREV_705  704, .
#define PPUTLUINT_PREV_704  703, .
#define PPUTLUINT_PREV_703  702, .
#define PPUTLUINT_PREV_702  701, .
#define PPUTLUINT_PREV_701  700, .
#define PPUTLUINT_PREV_700  699, .
#define PPUTLUINT_PREV_699  698, .
#define PPUTLUINT_PREV_698  697, .
#define PPUTLUINT_PREV_697  696, .
#define PPUTLUINT_PREV_696  695, .
#define PPUTLUINT_PREV_695  694, .
#define PPUTLUINT_PREV_694  693, .
#define PPUTLUINT_PREV_693  692, .
#define PPUTLUINT_PREV_692  691, .
#define PPUTLUINT_PREV_691  690, .
#define PPUTLUINT_PREV_690  689, .
#define PPUTLUINT_PREV_689  688, .
#define PPUTLUINT_PREV_688  687, .
#define PPUTLUINT_PREV_687  686, .
#define PPUTLUINT_PREV_686  685, .
#define PPUTLUINT_PREV_685  684, .
#define PPUTLUINT_PREV_684  683, .
#define PPUTLUINT_PREV_683  682, .
#define PPUTLUINT_PREV_682  681, .
#define PPUTLUINT_PREV_681  680, .
#define PPUTLUINT_PREV_680  679, .
#define PPUTLUINT_PREV_679  678, .
#define PPUTLUINT_PREV_678  677, .
#define PPUTLUINT_PREV_677  676, .
#define PPUTLUINT_PREV_676  675, .
#define PPUTLUINT_PREV_675  674, .
#define PPUTLUINT_PREV_674  673, .
#define PPUTLUINT_PREV_673  672, .
#define PPUTLUINT_PREV_672  671, .
#define PPUTLUINT_PREV_671  670, .
#define PPUTLUINT_PREV_670  669, .
#define PPUTLUINT_PREV_669  668, .
#define PPUTLUINT_PREV_668  667, .
#define PPUTLUINT_PREV_667  666, .
#define PPUTLUINT_PREV_666  665, .
#define PPUTLUINT_PREV_665  664, .
#define PPUTLUINT_PREV_664  663, .
#define PPUTLUINT_PREV_663  662, .
#define PPUTLUINT_PREV_662  661, .
#define PPUTLUINT_PREV_661  660, .
#define PPUTLUINT_PREV_660  659, .
#define PPUTLUINT_PREV_659  658, .
#define PPUTLUINT_PREV_658  657, .
#define PPUTLUINT_PREV_657  656, .
#define PPUTLUINT_PREV_656  655, .
#define PPUTLUINT_PREV_655  654, .
#define PPUTLUINT_PREV_654  653, .
#define PPUTLUINT_PREV_653  652, .
#define PPUTLUINT_PREV_652  651, .
#define PPUTLUINT_PREV_651  650, .
#define PPUTLUINT_PREV_650  649, .
#define PPUTLUINT_PREV_649  648, .
#define PPUTLUINT_PREV_648  647, .
#define PPUTLUINT_PREV_647  646, .
#define PPUTLUINT_PREV_646  645, .
#define PPUTLUINT_PREV_645  644, .
#define PPUTLUINT_PREV_644  643, .
#define PPUTLUINT_PREV_643  642, .
#define PPUTLUINT_PREV_642  641, .
#define PPUTLUINT_PREV_641  640, .
#define PPUTLUINT_PREV_640  639, .
#define PPUTLUINT_PREV_639  638, .
#define PPUTLUINT_PREV_638  637, .
#define PPUTLUINT_PREV_637  636, .
#define PPUTLUINT_PREV_636  635, .
#define PPUTLUINT_PREV_635  634, .
#define PPUTLUINT_PREV_634  633, .
#define PPUTLUINT_PREV_633  632, .
#define PPUTLUINT_PREV_632  631, .
#define PPUTLUINT_PREV_631  630, .
#define PPUTLUINT_PREV_630  629, .
#define PPUTLUINT_PREV_629  628, .
#define PPUTLUINT_PREV_628  627, .
#define PPUTLUINT_PREV_627  626, .
#define PPUTLUINT_PREV_626  625, .
#define PPUTLUINT_PREV_625  624, .
#define PPUTLUINT_PREV_624  623, .
#define PPUTLUINT_PREV_623  622, .
#define PPUTLUINT_PREV_622  621, .
#define PPUTLUINT_PREV_621  620, .
#define PPUTLUINT_PREV_620  619, .
#define PPUTLUINT_PREV_619  618, .
#define PPUTLUINT_PREV_618  617, .
#define PPUTLUINT_PREV_617  616, .
#define PPUTLUINT_PREV_616  615, .
#define PPUTLUINT_PREV_615  614, .
#define PPUTLUINT_PREV_614  613, .
#define PPUTLUINT_PREV_613  612, .
#define PPUTLUINT_PREV_612  611, .
#define PPUTLUINT_PREV_611  610, .
#define PPUTLUINT_PREV_610  609, .
#define PPUTLUINT_PREV_609  608, .
#define PPUTLUINT_PREV_608  607, .
#define PPUTLUINT_PREV_607  606, .
#define PPUTLUINT_PREV_606  605, .
#define PPUTLUINT_PREV_605  604, .
#define PPUTLUINT_PREV_604  603, .
#define PPUTLUINT_PREV_603  602, .
#define PPUTLUINT_PREV_602  601, .
#define PPUTLUINT_PREV_601  600, .
#define PPUTLUINT_PREV_600  599, .
#define PPUTLUINT_PREV_599  598, .
#define PPUTLUINT_PREV_598  597, .
#define PPUTLUINT_PREV_597  596, .
#define PPUTLUINT_PREV_596  595, .
#define PPUTLUINT_PREV_595  594, .
#define PPUTLUINT_PREV_594  593, .
#define PPUTLUINT_PREV_593  592, .
#define PPUTLUINT_PREV_592  591, .
#define PPUTLUINT_PREV_591  590, .
#define PPUTLUINT_PREV_590  589, .
#define PPUTLUINT_PREV_589  588, .
#define PPUTLUINT_PREV_588  587, .
#define PPUTLUINT_PREV_587  586, .
#define PPUTLUINT_PREV_586  585, .
#define PPUTLUINT_PREV_585  584, .
#define PPUTLUINT_PREV_584  583, .
#define PPUTLUINT_PREV_583  582, .
#define PPUTLUINT_PREV_582  581, .
#define PPUTLUINT_PREV_581  580, .
#define PPUTLUINT_PREV_580  579, .
#define PPUTLUINT_PREV_579  578, .
#define PPUTLUINT_PREV_578  577, .
#define PPUTLUINT_PREV_577  576, .
#define PPUTLUINT_PREV_576  575, .
#define PPUTLUINT_PREV_575  574, .
#define PPUTLUINT_PREV_574  573, .
#define PPUTLUINT_PREV_573  572, .
#define PPUTLUINT_PREV_572  571, .
#define PPUTLUINT_PREV_571  570, .
#define PPUTLUINT_PREV_570  569, .
#define PPUTLUINT_PREV_569  568, .
#define PPUTLUINT_PREV_568  567, .
#define PPUTLUINT_PREV_567  566, .
#define PPUTLUINT_PREV_566  565, .
#define PPUTLUINT_PREV_565  564, .
#define PPUTLUINT_PREV_564  563, .
#define PPUTLUINT_PREV_563  562, .
#define PPUTLUINT_PREV_562  561, .
#define PPUTLUINT_PREV_561  560, .
#define PPUTLUINT_PREV_560  559, .
#define PPUTLUINT_PREV_559  558, .
#define PPUTLUINT_PREV_558  557, .
#define PPUTLUINT_PREV_557  556, .
#define PPUTLUINT_PREV_556  555, .
#define PPUTLUINT_PREV_555  554, .
#define PPUTLUINT_PREV_554  553, .
#define PPUTLUINT_PREV_553  552, .
#define PPUTLUINT_PREV_552  551, .
#define PPUTLUINT_PREV_551  550, .
#define PPUTLUINT_PREV_550  549, .
#define PPUTLUINT_PREV_549  548, .
#define PPUTLUINT_PREV_548  547, .
#define PPUTLUINT_PREV_547  546, .
#define PPUTLUINT_PREV_546  545, .
#define PPUTLUINT_PREV_545  544, .
#define PPUTLUINT_PREV_544  543, .
#define PPUTLUINT_PREV_543  542, .
#define PPUTLUINT_PREV_542  541, .
#define PPUTLUINT_PREV_541  540, .
#define PPUTLUINT_PREV_540  539, .
#define PPUTLUINT_PREV_539  538, .
#define PPUTLUINT_PREV_538  537, .
#define PPUTLUINT_PREV_537  536, .
#define PPUTLUINT_PREV_536  535, .
#define PPUTLUINT_PREV_535  534, .
#define PPUTLUINT_PREV_534  533, .
#define PPUTLUINT_PREV_533  532, .
#define PPUTLUINT_PREV_532  531, .
#define PPUTLUINT_PREV_531  530, .
#define PPUTLUINT_PREV_530  529, .
#define PPUTLUINT_PREV_529  528, .
#define PPUTLUINT_PREV_528  527, .
#define PPUTLUINT_PREV_527  526, .
#define PPUTLUINT_PREV_526  525, .
#define PPUTLUINT_PREV_525  524, .
#define PPUTLUINT_PREV_524  523, .
#define PPUTLUINT_PREV_523  522, .
#define PPUTLUINT_PREV_522  521, .
#define PPUTLUINT_PREV_521  520, .
#define PPUTLUINT_PREV_520  519, .
#define PPUTLUINT_PREV_519  518, .
#define PPUTLUINT_PREV_518  517, .
#define PPUTLUINT_PREV_517  516, .
#define PPUTLUINT_PREV_516  515, .
#define PPUTLUINT_PREV_515  514, .
#define PPUTLUINT_PREV_514  513, .
#define PPUTLUINT_PREV_513  512, .
#define PPUTLUINT_PREV_512  511, .
#define PPUTLUINT_PREV_511  510, .
#define PPUTLUINT_PREV_510  509, .
#define PPUTLUINT_PREV_509  508, .
#define PPUTLUINT_PREV_508  507, .
#define PPUTLUINT_PREV_507  506, .
#define PPUTLUINT_PREV_506  505, .
#define PPUTLUINT_PREV_505  504, .
#define PPUTLUINT_PREV_504  503, .
#define PPUTLUINT_PREV_503  502, .
#define PPUTLUINT_PREV_502  501, .
#define PPUTLUINT_PREV_501  500, .
#define PPUTLUINT_PREV_500  499, .
#define PPUTLUINT_PREV_499  498, .
#define PPUTLUINT_PREV_498  497, .
#define PPUTLUINT_PREV_497  496, .
#define PPUTLUINT_PREV_496  495, .
#define PPUTLUINT_PREV_495  494, .
#define PPUTLUINT_PREV_494  493, .
#define PPUTLUINT_PREV_493  492, .
#define PPUTLUINT_PREV_492  491, .
#define PPUTLUINT_PREV_491  490, .
#define PPUTLUINT_PREV_490  489, .
#define PPUTLUINT_PREV_489  488, .
#define PPUTLUINT_PREV_488  487, .
#define PPUTLUINT_PREV_487  486, .
#define PPUTLUINT_PREV_486  485, .
#define PPUTLUINT_PREV_485  484, .
#define PPUTLUINT_PREV_484  483, .
#define PPUTLUINT_PREV_483  482, .
#define PPUTLUINT_PREV_482  481, .
#define PPUTLUINT_PREV_481  480, .
#define PPUTLUINT_PREV_480  479, .
#define PPUTLUINT_PREV_479  478, .
#define PPUTLUINT_PREV_478  477, .
#define PPUTLUINT_PREV_477  476, .
#define PPUTLUINT_PREV_476  475, .
#define PPUTLUINT_PREV_475  474, .
#define PPUTLUINT_PREV_474  473, .
#define PPUTLUINT_PREV_473  472, .
#define PPUTLUINT_PREV_472  471, .
#define PPUTLUINT_PREV_471  470, .
#define PPUTLUINT_PREV_470  469, .
#define PPUTLUINT_PREV_469  468, .
#define PPUTLUINT_PREV_468  467, .
#define PPUTLUINT_PREV_467  466, .
#define PPUTLUINT_PREV_466  465, .
#define PPUTLUINT_PREV_465  464, .
#define PPUTLUINT_PREV_464  463, .
#define PPUTLUINT_PREV_463  462, .
#define PPUTLUINT_PREV_462  461, .
#define PPUTLUINT_PREV_461  460, .
#define PPUTLUINT_PREV_460  459, .
#define PPUTLUINT_PREV_459  458, .
#define PPUTLUINT_PREV_458  457, .
#define PPUTLUINT_PREV_457  456, .
#define PPUTLUINT_PREV_456  455, .
#define PPUTLUINT_PREV_455  454, .
#define PPUTLUINT_PREV_454  453, .
#define PPUTLUINT_PREV_453  452, .
#define PPUTLUINT_PREV_452  451, .
#define PPUTLUINT_PREV_451  450, .
#define PPUTLUINT_PREV_450  449, .
#define PPUTLUINT_PREV_449  448, .
#define PPUTLUINT_PREV_448  447, .
#define PPUTLUINT_PREV_447  446, .
#define PPUTLUINT_PREV_446  445, .
#define PPUTLUINT_PREV_445  444, .
#define PPUTLUINT_PREV_444  443, .
#define PPUTLUINT_PREV_443  442, .
#define PPUTLUINT_PREV_442  441, .
#define PPUTLUINT_PREV_441  440, .
#define PPUTLUINT_PREV_440  439, .
#define PPUTLUINT_PREV_439  438, .
#define PPUTLUINT_PREV_438  437, .
#define PPUTLUINT_PREV_437  436, .
#define PPUTLUINT_PREV_436  435, .
#define PPUTLUINT_PREV_435  434, .
#define PPUTLUINT_PREV_434  433, .
#define PPUTLUINT_PREV_433  432, .
#define PPUTLUINT_PREV_432  431, .
#define PPUTLUINT_PREV_431  430, .
#define PPUTLUINT_PREV_430  429, .
#define PPUTLUINT_PREV_429  428, .
#define PPUTLUINT_PREV_428  427, .
#define PPUTLUINT_PREV_427  426, .
#define PPUTLUINT_PREV_426  425, .
#define PPUTLUINT_PREV_425  424, .
#define PPUTLUINT_PREV_424  423, .
#define PPUTLUINT_PREV_423  422, .
#define PPUTLUINT_PREV_422  421, .
#define PPUTLUINT_PREV_421  420, .
#define PPUTLUINT_PREV_420  419, .
#define PPUTLUINT_PREV_419  418, .
#define PPUTLUINT_PREV_418  417, .
#define PPUTLUINT_PREV_417  416, .
#define PPUTLUINT_PREV_416  415, .
#define PPUTLUINT_PREV_415  414, .
#define PPUTLUINT_PREV_414  413, .
#define PPUTLUINT_PREV_413  412, .
#define PPUTLUINT_PREV_412  411, .
#define PPUTLUINT_PREV_411  410, .
#define PPUTLUINT_PREV_410  409, .
#define PPUTLUINT_PREV_409  408, .
#define PPUTLUINT_PREV_408  407, .
#define PPUTLUINT_PREV_407  406, .
#define PPUTLUINT_PREV_406  405, .
#define PPUTLUINT_PREV_405  404, .
#define PPUTLUINT_PREV_404  403, .
#define PPUTLUINT_PREV_403  402, .
#define PPUTLUINT_PREV_402  401, .
#define PPUTLUINT_PREV_401  400, .
#define PPUTLUINT_PREV_400  399, .
#define PPUTLUINT_PREV_399  398, .
#define PPUTLUINT_PREV_398  397, .
#define PPUTLUINT_PREV_397  396, .
#define PPUTLUINT_PREV_396  395, .
#define PPUTLUINT_PREV_395  394, .
#define PPUTLUINT_PREV_394  393, .
#define PPUTLUINT_PREV_393  392, .
#define PPUTLUINT_PREV_392  391, .
#define PPUTLUINT_PREV_391  390, .
#define PPUTLUINT_PREV_390  389, .
#define PPUTLUINT_PREV_389  388, .
#define PPUTLUINT_PREV_388  387, .
#define PPUTLUINT_PREV_387  386, .
#define PPUTLUINT_PREV_386  385, .
#define PPUTLUINT_PREV_385  384, .
#define PPUTLUINT_PREV_384  383, .
#define PPUTLUINT_PREV_383  382, .
#define PPUTLUINT_PREV_382  381, .
#define PPUTLUINT_PREV_381  380, .
#define PPUTLUINT_PREV_380  379, .
#define PPUTLUINT_PREV_379  378, .
#define PPUTLUINT_PREV_378  377, .
#define PPUTLUINT_PREV_377  376, .
#define PPUTLUINT_PREV_376  375, .
#define PPUTLUINT_PREV_375  374, .
#define PPUTLUINT_PREV_374  373, .
#define PPUTLUINT_PREV_373  372, .
#define PPUTLUINT_PREV_372  371, .
#define PPUTLUINT_PREV_371  370, .
#define PPUTLUINT_PREV_370  369, .
#define PPUTLUINT_PREV_369  368, .
#define PPUTLUINT_PREV_368  367, .
#define PPUTLUINT_PREV_367  366, .
#define PPUTLUINT_PREV_366  365, .
#define PPUTLUINT_PREV_365  364, .
#define PPUTLUINT_PREV_364  363, .
#define PPUTLUINT_PREV_363  362, .
#define PPUTLUINT_PREV_362  361, .
#define PPUTLUINT_PREV_361  360, .
#define PPUTLUINT_PREV_360  359, .
#define PPUTLUINT_PREV_359  358, .
#define PPUTLUINT_PREV_358  357, .
#define PPUTLUINT_PREV_357  356, .
#define PPUTLUINT_PREV_356  355, .
#define PPUTLUINT_PREV_355  354, .
#define PPUTLUINT_PREV_354  353, .
#define PPUTLUINT_PREV_353  352, .
#define PPUTLUINT_PREV_352  351, .
#define PPUTLUINT_PREV_351  350, .
#define PPUTLUINT_PREV_350  349, .
#define PPUTLUINT_PREV_349  348, .
#define PPUTLUINT_PREV_348  347, .
#define PPUTLUINT_PREV_347  346, .
#define PPUTLUINT_PREV_346  345, .
#define PPUTLUINT_PREV_345  344, .
#define PPUTLUINT_PREV_344  343, .
#define PPUTLUINT_PREV_343  342, .
#define PPUTLUINT_PREV_342  341, .
#define PPUTLUINT_PREV_341  340, .
#define PPUTLUINT_PREV_340  339, .
#define PPUTLUINT_PREV_339  338, .
#define PPUTLUINT_PREV_338  337, .
#define PPUTLUINT_PREV_337  336, .
#define PPUTLUINT_PREV_336  335, .
#define PPUTLUINT_PREV_335  334, .
#define PPUTLUINT_PREV_334  333, .
#define PPUTLUINT_PREV_333  332, .
#define PPUTLUINT_PREV_332  331, .
#define PPUTLUINT_PREV_331  330, .
#define PPUTLUINT_PREV_330  329, .
#define PPUTLUINT_PREV_329  328, .
#define PPUTLUINT_PREV_328  327, .
#define PPUTLUINT_PREV_327  326, .
#define PPUTLUINT_PREV_326  325, .
#define PPUTLUINT_PREV_325  324, .
#define PPUTLUINT_PREV_324  323, .
#define PPUTLUINT_PREV_323  322, .
#define PPUTLUINT_PREV_322  321, .
#define PPUTLUINT_PREV_321  320, .
#define PPUTLUINT_PREV_320  319, .
#define PPUTLUINT_PREV_319  318, .
#define PPUTLUINT_PREV_318  317, .
#define PPUTLUINT_PREV_317  316, .
#define PPUTLUINT_PREV_316  315, .
#define PPUTLUINT_PREV_315  314, .
#define PPUTLUINT_PREV_314  313, .
#define PPUTLUINT_PREV_313  312, .
#define PPUTLUINT_PREV_312  311, .
#define PPUTLUINT_PREV_311  310, .
#define PPUTLUINT_PREV_310  309, .
#define PPUTLUINT_PREV_309  308, .
#define PPUTLUINT_PREV_308  307, .
#define PPUTLUINT_PREV_307  306, .
#define PPUTLUINT_PREV_306  305, .
#define PPUTLUINT_PREV_305  304, .
#define PPUTLUINT_PREV_304  303, .
#define PPUTLUINT_PREV_303  302, .
#define PPUTLUINT_PREV_302  301, .
#define PPUTLUINT_PREV_301  300, .
#define PPUTLUINT_PREV_300  299, .
#define PPUTLUINT_PREV_299  298, .
#define PPUTLUINT_PREV_298  297, .
#define PPUTLUINT_PREV_297  296, .
#define PPUTLUINT_PREV_296  295, .
#define PPUTLUINT_PREV_295  294, .
#define PPUTLUINT_PREV_294  293, .
#define PPUTLUINT_PREV_293  292, .
#define PPUTLUINT_PREV_292  291, .
#define PPUTLUINT_PREV_291  290, .
#define PPUTLUINT_PREV_290  289, .
#define PPUTLUINT_PREV_289  288, .
#define PPUTLUINT_PREV_288  287, .
#define PPUTLUINT_PREV_287  286, .
#define PPUTLUINT_PREV_286  285, .
#define PPUTLUINT_PREV_285  284, .
#define PPUTLUINT_PREV_284  283, .
#define PPUTLUINT_PREV_283  282, .
#define PPUTLUINT_PREV_282  281, .
#define PPUTLUINT_PREV_281  280, .
#define PPUTLUINT_PREV_280  279, .
#define PPUTLUINT_PREV_279  278, .
#define PPUTLUINT_PREV_278  277, .
#define PPUTLUINT_PREV_277  276, .
#define PPUTLUINT_PREV_276  275, .
#define PPUTLUINT_PREV_275  274, .
#define PPUTLUINT_PREV_274  273, .
#define PPUTLUINT_PREV_273  272, .
#define PPUTLUINT_PREV_272  271, .
#define PPUTLUINT_PREV_271  270, .
#define PPUTLUINT_PREV_270  269, .
#define PPUTLUINT_PREV_269  268, .
#define PPUTLUINT_PREV_268  267, .
#define PPUTLUINT_PREV_267  266, .
#define PPUTLUINT_PREV_266  265, .
#define PPUTLUINT_PREV_265  264, .
#define PPUTLUINT_PREV_264  263, .
#define PPUTLUINT_PREV_263  262, .
#define PPUTLUINT_PREV_262  261, .
#define PPUTLUINT_PREV_261  260, .
#define PPUTLUINT_PREV_260  259, .
#define PPUTLUINT_PREV_259  258, .
#define PPUTLUINT_PREV_258  257, .
#define PPUTLUINT_PREV_257  256, .
#define PPUTLUINT_PREV_256  255, .
#define PPUTLUINT_PREV_255  254, .
#define PPUTLUINT_PREV_254  253, .
#define PPUTLUINT_PREV_253  252, .
#define PPUTLUINT_PREV_252  251, .
#define PPUTLUINT_PREV_251  250, .
#define PPUTLUINT_PREV_250  249, .
#define PPUTLUINT_PREV_249  248, .
#define PPUTLUINT_PREV_248  247, .
#define PPUTLUINT_PREV_247  246, .
#define PPUTLUINT_PREV_246  245, .
#define PPUTLUINT_PREV_245  244, .
#define PPUTLUINT_PREV_244  243, .
#define PPUTLUINT_PREV_243  242, .
#define PPUTLUINT_PREV_242  241, .
#define PPUTLUINT_PREV_241  240, .
#define PPUTLUINT_PREV_240  239, .
#define PPUTLUINT_PREV_239  238, .
#define PPUTLUINT_PREV_238  237, .
#define PPUTLUINT_PREV_237  236, .
#define PPUTLUINT_PREV_236  235, .
#define PPUTLUINT_PREV_235  234, .
#define PPUTLUINT_PREV_234  233, .
#define PPUTLUINT_PREV_233  232, .
#define PPUTLUINT_PREV_232  231, .
#define PPUTLUINT_PREV_231  230, .
#define PPUTLUINT_PREV_230  229, .
#define PPUTLUINT_PREV_229  228, .
#define PPUTLUINT_PREV_228  227, .
#define PPUTLUINT_PREV_227  226, .
#define PPUTLUINT_PREV_226  225, .
#define PPUTLUINT_PREV_225  224, .
#define PPUTLUINT_PREV_224  223, .
#define PPUTLUINT_PREV_223  222, .
#define PPUTLUINT_PREV_222  221, .
#define PPUTLUINT_PREV_221  220, .
#define PPUTLUINT_PREV_220  219, .
#define PPUTLUINT_PREV_219  218, .
#define PPUTLUINT_PREV_218  217, .
#define PPUTLUINT_PREV_217  216, .
#define PPUTLUINT_PREV_216  215, .
#define PPUTLUINT_PREV_215  214, .
#define PPUTLUINT_PREV_214  213, .
#define PPUTLUINT_PREV_213  212, .
#define PPUTLUINT_PREV_212  211, .
#define PPUTLUINT_PREV_211  210, .
#define PPUTLUINT_PREV_210  209, .
#define PPUTLUINT_PREV_209  208, .
#define PPUTLUINT_PREV_208  207, .
#define PPUTLUINT_PREV_207  206, .
#define PPUTLUINT_PREV_206  205, .
#define PPUTLUINT_PREV_205  204, .
#define PPUTLUINT_PREV_204  203, .
#define PPUTLUINT_PREV_203  202, .
#define PPUTLUINT_PREV_202  201, .
#define PPUTLUINT_PREV_201  200, .
#define PPUTLUINT_PREV_200  199, .
#define PPUTLUINT_PREV_199  198, .
#define PPUTLUINT_PREV_198  197, .
#define PPUTLUINT_PREV_197  196, .
#define PPUTLUINT_PREV_196  195, .
#define PPUTLUINT_PREV_195  194, .
#define PPUTLUINT_PREV_194  193, .
#define PPUTLUINT_PREV_193  192, .
#define PPUTLUINT_PREV_192  191, .
#define PPUTLUINT_PREV_191  190, .
#define PPUTLUINT_PREV_190  189, .
#define PPUTLUINT_PREV_189  188, .
#define PPUTLUINT_PREV_188  187, .
#define PPUTLUINT_PREV_187  186, .
#define PPUTLUINT_PREV_186  185, .
#define PPUTLUINT_PREV_185  184, .
#define PPUTLUINT_PREV_184  183, .
#define PPUTLUINT_PREV_183  182, .
#define PPUTLUINT_PREV_182  181, .
#define PPUTLUINT_PREV_181  180, .
#define PPUTLUINT_PREV_180  179, .
#define PPUTLUINT_PREV_179  178, .
#define PPUTLUINT_PREV_178  177, .
#define PPUTLUINT_PREV_177  176, .
#define PPUTLUINT_PREV_176  175, .
#define PPUTLUINT_PREV_175  174, .
#define PPUTLUINT_PREV_174  173, .
#define PPUTLUINT_PREV_173  172, .
#define PPUTLUINT_PREV_172  171, .
#define PPUTLUINT_PREV_171  170, .
#define PPUTLUINT_PREV_170  169, .
#define PPUTLUINT_PREV_169  168, .
#define PPUTLUINT_PREV_168  167, .
#define PPUTLUINT_PREV_167  166, .
#define PPUTLUINT_PREV_166  165, .
#define PPUTLUINT_PREV_165  164, .
#define PPUTLUINT_PREV_164  163, .
#define PPUTLUINT_PREV_163  162, .
#define PPUTLUINT_PREV_162  161, .
#define PPUTLUINT_PREV_161  160, .
#define PPUTLUINT_PREV_160  159, .
#define PPUTLUINT_PREV_159  158, .
#define PPUTLUINT_PREV_158  157, .
#define PPUTLUINT_PREV_157  156, .
#define PPUTLUINT_PREV_156  155, .
#define PPUTLUINT_PREV_155  154, .
#define PPUTLUINT_PREV_154  153, .
#define PPUTLUINT_PREV_153  152, .
#define PPUTLUINT_PREV_152  151, .
#define PPUTLUINT_PREV_151  150, .
#define PPUTLUINT_PREV_150  149, .
#define PPUTLUINT_PREV_149  148, .
#define PPUTLUINT_PREV_148  147, .
#define PPUTLUINT_PREV_147  146, .
#define PPUTLUINT_PREV_146  145, .
#define PPUTLUINT_PREV_145  144, .
#define PPUTLUINT_PREV_144  143, .
#define PPUTLUINT_PREV_143  142, .
#define PPUTLUINT_PREV_142  141, .
#define PPUTLUINT_PREV_141  140, .
#define PPUTLUINT_PREV_140  139, .
#define PPUTLUINT_PREV_139  138, .
#define PPUTLUINT_PREV_138  137, .
#define PPUTLUINT_PREV_137  136, .
#define PPUTLUINT_PREV_136  135, .
#define PPUTLUINT_PREV_135  134, .
#define PPUTLUINT_PREV_134  133, .
#define PPUTLUINT_PREV_133  132, .
#define PPUTLUINT_PREV_132  131, .
#define PPUTLUINT_PREV_131  130, .
#define PPUTLUINT_PREV_130  129, .
#define PPUTLUINT_PREV_129  128, .
#define PPUTLUINT_PREV_128  127, .
#define PPUTLUINT_PREV_127  126, .
#define PPUTLUINT_PREV_126  125, .
#define PPUTLUINT_PREV_125  124, .
#define PPUTLUINT_PREV_124  123, .
#define PPUTLUINT_PREV_123  122, .
#define PPUTLUINT_PREV_122  121, .
#define PPUTLUINT_PREV_121  120, .
#define PPUTLUINT_PREV_120  119, .
#define PPUTLUINT_PREV_119  118, .
#define PPUTLUINT_PREV_118  117, .
#define PPUTLUINT_PREV_117  116, .
#define PPUTLUINT_PREV_116  115, .
#define PPUTLUINT_PREV_115  114, .
#define PPUTLUINT_PREV_114  113, .
#define PPUTLUINT_PREV_113  112, .
#define PPUTLUINT_PREV_112  111, .
#define PPUTLUINT_PREV_111  110, .
#define PPUTLUINT_PREV_110  109, .
#define PPUTLUINT_PREV_109  108, .
#define PPUTLUINT_PREV_108  107, .
#define PPUTLUINT_PREV_107  106, .
#define PPUTLUINT_PREV_106  105, .
#define PPUTLUINT_PREV_105  104, .
#define PPUTLUINT_PREV_104  103, .
#define PPUTLUINT_PREV_103  102, .
#define PPUTLUINT_PREV_102  101, .
#define PPUTLUINT_PREV_101  100, .
#define PPUTLUINT_PREV_100  99, .
#define PPUTLUINT_PREV_99   98, .
#define PPUTLUINT_PREV_98   97, .
#define PPUTLUINT_PREV_97   96, .
#define PPUTLUINT_PREV_96   95, .
#define PPUTLUINT_PREV_95   94, .
#define PPUTLUINT_PREV_94   93, .
#define PPUTLUINT_PREV_93   92, .
#define PPUTLUINT_PREV_92   91, .
#define PPUTLUINT_PREV_91   90, .
#define PPUTLUINT_PREV_90   89, .
#define PPUTLUINT_PREV_89   88, .
#define PPUTLUINT_PREV_88   87, .
#define PPUTLUINT_PREV_87   86, .
#define PPUTLUINT_PREV_86   85, .
#define PPUTLUINT_PREV_85   84, .
#define PPUTLUINT_PREV_84   83, .
#define PPUTLUINT_PREV_83   82, .
#define PPUTLUINT_PREV_82   81, .
#define PPUTLUINT_PREV_81   80, .
#define PPUTLUINT_PREV_80   79, .
#define PPUTLUINT_PREV_79   78, .
#define PPUTLUINT_PREV_78   77, .
#define PPUTLUINT_PREV_77   76, .
#define PPUTLUINT_PREV_76   75, .
#define PPUTLUINT_PREV_75   74, .
#define PPUTLUINT_PREV_74   73, .
#define PPUTLUINT_PREV_73   72, .
#define PPUTLUINT_PREV_72   71, .
#define PPUTLUINT_PREV_71   70, .
#define PPUTLUINT_PREV_70   69, .
#define PPUTLUINT_PREV_69   68, .
#define PPUTLUINT_PREV_68   67, .
#define PPUTLUINT_PREV_67   66, .
#define PPUTLUINT_PREV_66   65, .
#define PPUTLUINT_PREV_65   64, .
#define PPUTLUINT_PREV_64   63, .
#define PPUTLUINT_PREV_63   62, .
#define PPUTLUINT_PREV_62   61, .
#define PPUTLUINT_PREV_61   60, .
#define PPUTLUINT_PREV_60   59, .
#define PPUTLUINT_PREV_59   58, .
#define PPUTLUINT_PREV_58   57, .
#define PPUTLUINT_PREV_57   56, .
#define PPUTLUINT_PREV_56   55, .
#define PPUTLUINT_PREV_55   54, .
#define PPUTLUINT_PREV_54   53, .
#define PPUTLUINT_PREV_53   52, .
#define PPUTLUINT_PREV_52   51, .
#define PPUTLUINT_PREV_51   50, .
#define PPUTLUINT_PREV_50   49, .
#define PPUTLUINT_PREV_49   48, .
#define PPUTLUINT_PREV_48   47, .
#define PPUTLUINT_PREV_47   46, .
#define PPUTLUINT_PREV_46   45, .
#define PPUTLUINT_PREV_45   44, .
#define PPUTLUINT_PREV_44   43, .
#define PPUTLUINT_PREV_43   42, .
#define PPUTLUINT_PREV_42   41, .
#define PPUTLUINT_PREV_41   40, .
#define PPUTLUINT_PREV_40   39, .
#define PPUTLUINT_PREV_39   38, .
#define PPUTLUINT_PREV_38   37, .
#define PPUTLUINT_PREV_37   36, .
#define PPUTLUINT_PREV_36   35, .
#define PPUTLUINT_PREV_35   34, .
#define PPUTLUINT_PREV_34   33, .
#define PPUTLUINT_PREV_33   32, .
#define PPUTLUINT_PREV_32   31, .
#define PPUTLUINT_PREV_31   30, .
#define PPUTLUINT_PREV_30   29, .
#define PPUTLUINT_PREV_29   28, .
#define PPUTLUINT_PREV_28   27, .
#define PPUTLUINT_PREV_27   26, .
#define PPUTLUINT_PREV_26   25, .
#define PPUTLUINT_PREV_25   24, .
#define PPUTLUINT_PREV_24   23, .
#define PPUTLUINT_PREV_23   22, .
#define PPUTLUINT_PREV_22   21, .
#define PPUTLUINT_PREV_21   20, .
#define PPUTLUINT_PREV_20   19, .
#define PPUTLUINT_PREV_19   18, .
#define PPUTLUINT_PREV_18   17, .
#define PPUTLUINT_PREV_17   16, .
#define PPUTLUINT_PREV_16   15, .
#define PPUTLUINT_PREV_15   14, .
#define PPUTLUINT_PREV_14   13, .
#define PPUTLUINT_PREV_13   12, .
#define PPUTLUINT_PREV_12   11, .
#define PPUTLUINT_PREV_11   10, .
#define PPUTLUINT_PREV_10   9, .
#define PPUTLUINT_PREV_9    8, .
#define PPUTLUINT_PREV_8    7, .
#define PPUTLUINT_PREV_7    6, .
#define PPUTLUINT_PREV_6    5, .
#define PPUTLUINT_PREV_5    4, .
#define PPUTLUINT_PREV_4    3, .
#define PPUTLUINT_PREV_3    2, .
#define PPUTLUINT_PREV_2    1, .
#define PPUTLUINT_PREV_1    0, .
#define PPUTLUINT_PREV_0    1023, .

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

//                                                                           }}}

/// [meta.lp]
/// ---------
/// hides a left parens behind an expansion.
/// needed for implementing pair recursion.
#define PTL_LP /* -> <left parens> */ (

/// [meta.rp]
/// ---------
/// hides a right parens behind an expansion.
/// needed for implementing pair recursion.
#define PTL_RP /* -> <left parens> */ )

/// [meta.xct]
/// ----------
/// counts the number of expansions undergone after expression.
/// uses recursion; can track any number of expansions.
/// the number of commas indicates the number of expansions.
///
/// PTL_STR(PTL_XCT)                            // "PPUTLXCT_A ( , )"
/// PTL_STR(PTL_ESC(PTL_XCT))                   // "PPUTLXCT_B ( ,, )"
/// PTL_STR(PTL_ESC(PTL_ESC(PTL_XCT)))          // "PPUTLXCT_A ( ,,, )"
/// PTL_STR(PTL_ESC(PTL_ESC(PTL_ESC(PTL_XCT)))) // "PPUTLXCT_B ( ,,,, )"
#define PTL_XCT /* -> <xct expr> */ PPUTLXCT_A PTL_LP /**/, PTL_RP

//                                                                           {{{

#define PPUTLXCT_B(...) PPUTLXCT_A PTL_LP __VA_ARGS__, PTL_RP
#define PPUTLXCT_A(...) PPUTLXCT_B PTL_LP __VA_ARGS__, PTL_RP

//                                                                           }}}

/// [meta.x]
/// --------
/// performs uint n secondary expansions (n=0 -> identity).
/// args are expressed after n+1 expansions in total.
/// useful for implementing mutual recursion.
///
/// PTL_X(0)(PTL_XCT) // PTL_ESC(PTL_XCT)
/// PTL_X(1)(PTL_XCT) // PTL_ESC(PTL_ESC(PTL_XCT))
/// PTL_X(0)(PTL_XCT) // PPUTLXCT_A ( , )
/// PTL_X(1)(PTL_XCT) // PPUTLXCT_B ( ,, )
#define PTL_X(/* n: uint */...) /* -> (args: any...) -<n>-> ...args */ PTL_CAT(PPUTLX_, PTL_UINT(__VA_ARGS__))

//                                                                           {{{

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

//                                                                           }}}

/// [numeric.inc]
/// -------------
/// uint increment w/ overflow.
///
/// PTL_INC(0)    // 1
/// PTL_INC(1)    // 2
/// PTL_INC(1023) // 0
#define PTL_INC(/* n: uint */...) /* -> uint{n+1} */ \
  PTL_FIRST(PTL_CAT(PPUTLUINT_NEXT_, PTL_UINT(__VA_ARGS__)))

/// [numeric.dec]
/// -------------
/// uint decrement w/ underflow.
///
/// PTL_DEC(0)    // 1023
/// PTL_DEC(1)    // 0
/// PTL_DEC(1023) // 1022
#define PTL_DEC(/* n: uint */...) /* -> uint{n-1} */ \
  PTL_FIRST(PTL_CAT(PPUTLUINT_PREV_, PTL_UINT(__VA_ARGS__)))

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

//                                                                           {{{

#define PPUTLEQZ_X(...)  PPUTLEQZ_##__VA_OPT__(NO_)##PASS
#define PPUTLEQZ_NO_PASS 0
#define PPUTLEQZ_PASS    1
#define PPUTLEQZ_0

//                                                                           }}}

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

//                                                                           {{{

#define PPUTLNEZ_X(...)  PPUTLNEZ_##__VA_OPT__(NO_)##FAIL
#define PPUTLNEZ_NO_FAIL 1
#define PPUTLNEZ_FAIL    0
#define PPUTLNEZ_0

//                                                                           }}}

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

//                                                                           {{{

/// first parentheses; chooses next function based on b
#define PPUTLIF_O(b, ...) PTL_CAT(PPUTLIF_OO_, PTL_BOOL(b))

/// second parentheses; true result
#define PPUTLIF_OO_1(b, t, f) PPUTLIF_OO_1_X(PTL_TUP(t))
#define PPUTLIF_OO_1_X(t)     PTL_ESC t

/// second parentheses; false result
#define PPUTLIF_OO_0(b, t, f) PPUTLIF_OO_0_X(PTL_TUP(f))
#define PPUTLIF_OO_0_X(f)     PTL_ESC f

//                                                                           }}}

/// [control.switch]
/// ----------------
/// conditionally expands items based on a uint.
///
/// PTL_SWITCH(0, (1))              // 1
/// PTL_SWITCH(1, (1), (2))         // 2
/// PTL_SWITCH(2, (1), (2), (3, 4)) // 3, 4
#define PTL_SWITCH(/* cs: uint, cases: tup... */...) /* -<4+2cs>-> ...cases[cs] */ \
  PTL_X(PTL_FIRST(__VA_ARGS__))(PPUTLSWITCH_A(PTL_UINT(PTL_FIRST(__VA_ARGS__)))(__VA_ARGS__))

//                                                                           {{{

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

//                                                                           }}}

/// [traits.is_none]
/// ----------------
/// detects if args is nothing.
///
/// PTL_IS_NONE()          // 1
/// PTL_IS_NONE(foo)       // 0
/// PTL_IS_NONE(foo, bar)  // 0
/// PTL_IS_NONE(PTL_ESC()) // 1
#define PTL_IS_NONE(...) /* -> bool */ PPUTLIS_NONE_O##__VA_OPT__(0)

//                                                                           {{{

#define PPUTLIS_NONE_O0 0
#define PPUTLIS_NONE_O  1

//                                                                           }}}

/// [traits.is_some]
/// ----------------
/// detects if args is something.
///
/// PTL_IS_SOME()          // 0
/// PTL_IS_SOME(foo)       // 1
/// PTL_IS_SOME(foo, bar)  // 1
/// PTL_IS_SOME(PTL_ESC()) // 0
#define PTL_IS_SOME(...) /* -> bool */ PPUTLIS_SOME_O##__VA_OPT__(1)

//                                                                           {{{

#define PPUTLIS_SOME_O1 1
#define PPUTLIS_SOME_O  0

//                                                                           }}}

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

//                                                                           {{{

#define PPUTLIS_UINT_CHK_PTL_UINT(...)

//                                                                           }}}

/// [traits.count]
/// --------------
/// computes the uint quantity of args.
/// terminates expansion with an error if too many args passed.
///
/// PTL_COUNT()     // 0
/// PTL_COUNT(a)    // 1
/// PTL_COUNT(a, b) // 2
/// PTL_COUNT(, )   // 2
#define PTL_COUNT(...) /* -> uint */ \
  PTL_IF(PTL_IS_NONE(__VA_ARGS__), (0), (PTL_X(1022)(PPUTLCOUNT_A(1, __VA_ARGS__, ))))

//                                                                           {{{

/// mutually recursive branch B
#define PPUTLCOUNT_B(i, ...)                                            \
  PTL_IF(PTL_IS_NONE(__VA_ARGS__), (PPUTLCOUNT_RETURN),                 \
         (PTL_IF(PTL_EQZ(i), (PPUTLCOUNT_THROW), (PPUTLCOUNT_B_CONT)))) \
  (i, __VA_ARGS__)
#define PPUTLCOUNT_B_CONT(i, _, ...) PPUTLCOUNT_A PTL_LP PTL_INC(i), __VA_ARGS__ PTL_RP

/// mutually recursive branch A
#define PPUTLCOUNT_A(i, ...)                                            \
  PTL_IF(PTL_IS_NONE(__VA_ARGS__), (PPUTLCOUNT_RETURN),                 \
         (PTL_IF(PTL_EQZ(i), (PPUTLCOUNT_THROW), (PPUTLCOUNT_A_CONT)))) \
  (i, __VA_ARGS__)
#define PPUTLCOUNT_A_CONT(i, _, ...) PPUTLCOUNT_B PTL_LP PTL_INC(i), __VA_ARGS__ PTL_RP

/// exit macros
#define PPUTLCOUNT_THROW(...)     PTL_COUNT(Error : too many args)
#define PPUTLCOUNT_RETURN(i, ...) PTL_DEC(i)

//                                                                           }}}

// vim: fdm=marker:fmr={{{,}}}

#endif

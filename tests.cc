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

#include "pputl.h"

static constexpr bool
streq(char const* l, char const* r) {
  for (; *l and *r and *l == *r; ++l, ++r)
    ;
  return *l == *r;
}

#define ASSERT_PP_EQ_X(a, b) static_assert(streq(#a, #b), #a " != " #b)
#define ASSERT_PP_EQ(a, b)   ASSERT_PP_EQ_X(a, b)

// clang-format off
ASSERT_PP_EQ((PTL_ESC ()), ());
ASSERT_PP_EQ((PTL_ESC (a, b, c)), (a, b, c));

ASSERT_PP_EQ((PTL_STR()), (""));
ASSERT_PP_EQ((PTL_STR(foo, bar)), ("foo, bar"));
ASSERT_PP_EQ((PTL_STR(foo)), ("foo"));
ASSERT_PP_EQ((PTL_STR(, )), (","));
ASSERT_PP_EQ((PTL_STR(, , )), (", ,"));
ASSERT_PP_EQ((PTL_STR(a, )), ("a,"));
ASSERT_PP_EQ((PTL_STR(a, , )), ("a, ,"));
ASSERT_PP_EQ((PTL_STR(, a)), (", a"));
ASSERT_PP_EQ((PTL_STR(, a, )), (", a,"));
ASSERT_PP_EQ((PTL_STR(, , a)), (", , a"));

ASSERT_PP_EQ((PTL_CAT(foo, bar)), (foobar));
ASSERT_PP_EQ((PTL_CAT(foo, PTL_EAT(bar))), (foo));

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

ASSERT_PP_EQ((PTL_TUP(())), (()));
ASSERT_PP_EQ((PTL_TUP((1, 2))), ((1, 2)));
ASSERT_PP_EQ((PTL_STR(PTL_TUP(0))), ("PTL_TUP(0)"));
ASSERT_PP_EQ((PTL_STR(PTL_TUP(1, 2))), ("PTL_TUP(1, 2)"));
ASSERT_PP_EQ((PTL_STR(PTL_TUP(1,))), ("PTL_TUP(1,)"));
ASSERT_PP_EQ((PTL_STR(PTL_TUP(foo))), ("PTL_TUP(foo)"));
ASSERT_PP_EQ((PTL_STR(PTL_TUP((), ()))), ("PTL_TUP((), ())"));

ASSERT_PP_EQ((PTL_BOOL(0)), (0));
ASSERT_PP_EQ((PTL_BOOL(1)), (1));
ASSERT_PP_EQ((PTL_STR(PTL_BOOL(2))), ("PTL_BOOL(2)"));
ASSERT_PP_EQ((PTL_STR(PTL_BOOL(1, 2))), ("PTL_BOOL(1, 2)"));
ASSERT_PP_EQ((PTL_STR(PTL_BOOL(1,))), ("PTL_BOOL(1,)"));
ASSERT_PP_EQ((PTL_STR(PTL_BOOL(foo))), ("PTL_BOOL(foo)"));
ASSERT_PP_EQ((PTL_STR(PTL_BOOL(()))), ("PTL_BOOL(())"));
ASSERT_PP_EQ((PTL_STR(PTL_BOOL((), ()))), ("PTL_BOOL((), ())"));

ASSERT_PP_EQ((PTL_UINT(0)), (0));
ASSERT_PP_EQ((PTL_UINT(1)), (1));
ASSERT_PP_EQ((PTL_UINT(2)), (2));
ASSERT_PP_EQ((PTL_UINT(1023)), (1023));
ASSERT_PP_EQ((PTL_STR(PTL_UINT(1024))), ("PTL_UINT(1024)"));
ASSERT_PP_EQ((PTL_STR(PTL_UINT(1, 2))), ("PTL_UINT(1, 2)"));
ASSERT_PP_EQ((PTL_STR(PTL_UINT(1,))), ("PTL_UINT(1,)"));
ASSERT_PP_EQ((PTL_STR(PTL_UINT(foo))), ("PTL_UINT(foo)"));
ASSERT_PP_EQ((PTL_STR(PTL_UINT(()))), ("PTL_UINT(())"));
ASSERT_PP_EQ((PTL_STR(PTL_UINT((), ()))), ("PTL_UINT((), ())"));

ASSERT_PP_EQ((PTL_INC(0)), (1));
ASSERT_PP_EQ((PTL_INC(1)), (2));
ASSERT_PP_EQ((PTL_INC(1023)), (0));
ASSERT_PP_EQ((PTL_INC(2)), (3));
ASSERT_PP_EQ((PTL_INC(1022)), (1023));

ASSERT_PP_EQ((PTL_DEC(0)), (1023));
ASSERT_PP_EQ((PTL_DEC(1)), (0));
ASSERT_PP_EQ((PTL_DEC(1023)), (1022));
ASSERT_PP_EQ((PTL_DEC(2)), (1));
ASSERT_PP_EQ((PTL_DEC(1022)), (1021));

ASSERT_PP_EQ((PTL_EQZ(0)), (1));
ASSERT_PP_EQ((PTL_EQZ(1)), (0));
ASSERT_PP_EQ((PTL_EQZ(2)), (0));
ASSERT_PP_EQ((PTL_EQZ(1023)), (0));
ASSERT_PP_EQ((PTL_EQZ(PTL_INC(1023))), (1));

ASSERT_PP_EQ((PTL_NEZ(0)), (0));
ASSERT_PP_EQ((PTL_NEZ(1)), (1));
ASSERT_PP_EQ((PTL_NEZ(2)), (1));
ASSERT_PP_EQ((PTL_NEZ(1023)), (1));
ASSERT_PP_EQ((PTL_NEZ(PTL_INC(1023))), (0));

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

ASSERT_PP_EQ((PTL_IS_UINT()), (0));
ASSERT_PP_EQ((PTL_IS_UINT(foo)), (0));
ASSERT_PP_EQ((PTL_IS_UINT(0)), (1));
ASSERT_PP_EQ((PTL_IS_UINT(())), (0));
ASSERT_PP_EQ((PTL_IS_UINT((), ())), (0));
ASSERT_PP_EQ((PTL_IS_UINT(0, 1)), (0));
ASSERT_PP_EQ((PTL_IS_UINT(1023)), (1));
ASSERT_PP_EQ((PTL_IS_UINT(foo, bar)), (0));
ASSERT_PP_EQ((PTL_IS_UINT(1022)), (1));
ASSERT_PP_EQ((PTL_IS_UINT(, )), (0));
ASSERT_PP_EQ((PTL_IS_UINT(, , )), (0));
ASSERT_PP_EQ((PTL_IS_UINT(a, )), (0));
ASSERT_PP_EQ((PTL_IS_UINT(a, , )), (0));
ASSERT_PP_EQ((PTL_IS_UINT(, a)), (0));
ASSERT_PP_EQ((PTL_IS_UINT(, a, )), (0));
ASSERT_PP_EQ((PTL_IS_UINT(, , a)), (0));

ASSERT_PP_EQ((PTL_SIZE()), (0));
ASSERT_PP_EQ((PTL_SIZE(a)), (1));
ASSERT_PP_EQ((PTL_SIZE(a, b)), (2));
ASSERT_PP_EQ((PTL_SIZE(, )), (2));
ASSERT_PP_EQ((PTL_SIZE(a, b, c)), (3));
ASSERT_PP_EQ((PTL_SIZE(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318, 319, 320, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359, 360, 361, 362, 363, 364, 365, 366, 367, 368, 369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 380, 381, 382, 383, 384, 385, 386, 387, 388, 389, 390, 391, 392, 393, 394, 395, 396, 397, 398, 399, 400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416, 417, 418, 419, 420, 421, 422, 423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433, 434, 435, 436, 437, 438, 439, 440, 441, 442, 443, 444, 445, 446, 447, 448, 449, 450, 451, 452, 453, 454, 455, 456, 457, 458, 459, 460, 461, 462, 463, 464, 465, 466, 467, 468, 469, 470, 471, 472, 473, 474, 475, 476, 477, 478, 479, 480, 481, 482, 483, 484, 485, 486, 487, 488, 489, 490, 491, 492, 493, 494, 495, 496, 497, 498, 499, 500, 501, 502, 503, 504, 505, 506, 507, 508, 509, 510, 511, 512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523, 524, 525, 526, 527, 528, 529, 530, 531, 532, 533, 534, 535, 536, 537, 538, 539, 540, 541, 542, 543, 544, 545, 546, 547, 548, 549, 550, 551, 552, 553, 554, 555, 556, 557, 558, 559, 560, 561, 562, 563, 564, 565, 566, 567, 568, 569, 570, 571, 572, 573, 574, 575, 576, 577, 578, 579, 580, 581, 582, 583, 584, 585, 586, 587, 588, 589, 590, 591, 592, 593, 594, 595, 596, 597, 598, 599, 600, 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611, 612, 613, 614, 615, 616, 617, 618, 619, 620, 621, 622, 623, 624, 625, 626, 627, 628, 629, 630, 631, 632, 633, 634, 635, 636, 637, 638, 639, 640, 641, 642, 643, 644, 645, 646, 647, 648, 649, 650, 651, 652, 653, 654, 655, 656, 657, 658, 659, 660, 661, 662, 663, 664, 665, 666, 667, 668, 669, 670, 671, 672, 673, 674, 675, 676, 677, 678, 679, 680, 681, 682, 683, 684, 685, 686, 687, 688, 689, 690, 691, 692, 693, 694, 695, 696, 697, 698, 699, 700, 701, 702, 703, 704, 705, 706, 707, 708, 709, 710, 711, 712, 713, 714, 715, 716, 717, 718, 719, 720, 721, 722, 723, 724, 725, 726, 727, 728, 729, 730, 731, 732, 733, 734, 735, 736, 737, 738, 739, 740, 741, 742, 743, 744, 745, 746, 747, 748, 749, 750, 751, 752, 753, 754, 755, 756, 757, 758, 759, 760, 761, 762, 763, 764, 765, 766, 767, 768, 769, 770, 771, 772, 773, 774, 775, 776, 777, 778, 779, 780, 781, 782, 783, 784, 785, 786, 787, 788, 789, 790, 791, 792, 793, 794, 795, 796, 797, 798, 799, 800, 801, 802, 803, 804, 805, 806, 807, 808, 809, 810, 811, 812, 813, 814, 815, 816, 817, 818, 819, 820, 821, 822, 823, 824, 825, 826, 827, 828, 829, 830, 831, 832, 833, 834, 835, 836, 837, 838, 839, 840, 841, 842, 843, 844, 845, 846, 847, 848, 849, 850, 851, 852, 853, 854, 855, 856, 857, 858, 859, 860, 861, 862, 863, 864, 865, 866, 867, 868, 869, 870, 871, 872, 873, 874, 875, 876, 877, 878, 879, 880, 881, 882, 883, 884, 885, 886, 887, 888, 889, 890, 891, 892, 893, 894, 895, 896, 897, 898, 899, 900, 901, 902, 903, 904, 905, 906, 907, 908, 909, 910, 911, 912, 913, 914, 915, 916, 917, 918, 919, 920, 921, 922, 923, 924, 925, 926, 927, 928, 929, 930, 931, 932, 933, 934, 935, 936, 937, 938, 939, 940, 941, 942, 943, 944, 945, 946, 947, 948, 949, 950, 951, 952, 953, 954, 955, 956, 957, 958, 959, 960, 961, 962, 963, 964, 965, 966, 967, 968, 969, 970, 971, 972, 973, 974, 975, 976, 977, 978, 979, 980, 981, 982, 983, 984, 985, 986, 987, 988, 989, 990, 991, 992, 993, 994, 995, 996, 997, 998, 999, 1000, 1001, 1002, 1003, 1004, 1005, 1006, 1007, 1008, 1009, 1010, 1011, 1012, 1013, 1014, 1015, 1016, 1017, 1018, 1019, 1020, 1021, 1022)), (1023));
ASSERT_PP_EQ((PTL_SIZE(, , )), (3));
ASSERT_PP_EQ((PTL_SIZE(a, )), (2));
ASSERT_PP_EQ((PTL_SIZE(a, , )), (3));
ASSERT_PP_EQ((PTL_SIZE(, a)), (2));
ASSERT_PP_EQ((PTL_SIZE(, a, )), (3));
ASSERT_PP_EQ((PTL_SIZE(, , a)), (3));

ASSERT_PP_EQ((PTL_STR(PTL_XTRACE)), ("PPUTLXTRACE_A ( , )"));
ASSERT_PP_EQ((PTL_STR(PTL_ESC(PTL_XTRACE))), ("PPUTLXTRACE_B ( ,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_ESC(PTL_ESC(PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_ESC(PTL_ESC(PTL_ESC(PTL_XTRACE))))), ("PPUTLXTRACE_B ( ,,,, )"));

ASSERT_PP_EQ((PTL_XCOUNT(PTL_XTRACE)), (0));
ASSERT_PP_EQ((PTL_XCOUNT(PTL_ESC(PTL_XTRACE))), (1));
ASSERT_PP_EQ((PTL_XCOUNT(PTL_ESC(PTL_ESC(PTL_XTRACE)))), (2));
ASSERT_PP_EQ((PTL_XCOUNT(PTL_ESC(PTL_ESC(PTL_ESC(PTL_XTRACE))))), (3));

ASSERT_PP_EQ((PTL_X(0)(PTL_XTRACE)), (PTL_ESC(PTL_XTRACE)));
ASSERT_PP_EQ((PTL_X(1)(PTL_XTRACE)), (PTL_ESC(PTL_ESC(PTL_XTRACE))));
ASSERT_PP_EQ((PTL_X(0)(PTL_XTRACE)), (PPUTLXTRACE_A ( , )));
ASSERT_PP_EQ((PTL_X(1)(PTL_XTRACE)), (PPUTLXTRACE_B ( ,, )));
ASSERT_PP_EQ((PTL_X(2)(PTL_XTRACE)), (PPUTLXTRACE_A ( ,,, )));
ASSERT_PP_EQ((PTL_X(3)(PTL_XTRACE)), (PPUTLXTRACE_B ( ,,,, )));
ASSERT_PP_EQ((PTL_X(4)(PTL_XTRACE)), (PPUTLXTRACE_A ( ,,,,, )));
ASSERT_PP_EQ((PTL_X(1023)(PTL_XTRACE)), (PPUTLXTRACE_B ( ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,, )));
ASSERT_PP_EQ((PTL_XCOUNT(PTL_X(1022)(PTL_XTRACE))), (1023));

ASSERT_PP_EQ((PTL_PASTE()), ());
ASSERT_PP_EQ((PTL_PASTE(a)), (a));
ASSERT_PP_EQ((PTL_PASTE(a, b)), (a b));
ASSERT_PP_EQ((PTL_PASTE(a, b, c)), (a b c));
ASSERT_PP_EQ((PTL_PASTE(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, ba, bb, bc, bd, be, bf, bg, bh, bi, bj, bk, bl, bm, bn, bo, bp, bq, br, bs, bt, bu, bv, bw, bx, by, bz, bA, bB, bC, bD, bE, bF, bG, bH, bI, bJ, bK, bL, bM, bN, bO, bP, bQ, bR, bS, bT, bU, bV, bW, bX, bY, bZ, ca, cb, cc, cd, ce, cf, cg, ch, ci, cj, ck, cl, cm, cn, co, cp, cq, cr, cs, ct, cu, cv, cw, cx, cy, cz, cA, cB, cC, cD, cE, cF, cG, cH, cI, cJ, cK, cL, cM, cN, cO, cP, cQ, cR, cS, cT, cU, cV, cW, cX, cY, cZ, da, db, dc, dd, de, df, dg, dh, di, dj, dk, dl, dm, dn, do, dp, dq, dr, ds, dt, du, dv, dw, dx, dy, dz, dA, dB, dC, dD, dE, dF, dG, dH, dI, dJ, dK, dL, dM, dN, dO, dP, dQ, dR, dS, dT, dU, dV, dW, dX, dY, dZ, ea, eb, ec, ed, ee, ef, eg, eh, ei, ej, ek, el, em, en, eo, ep, eq, er, es, et, eu, ev, ew, ex, ey, ez, eA, eB, eC, eD, eE, eF, eG, eH, eI, eJ, eK, eL, eM, eN, eO, eP, eQ, eR, eS, eT, eU, eV, eW, eX, eY, eZ, fa, fb, fc, fd, fe, ff, fg, fh, fi, fj, fk, fl, fm, fn, fo, fp, fq, fr, fs, ft, fu, fv, fw, fx, fy, fz, fA, fB, fC, fD, fE, fF, fG, fH, fI, fJ, fK, fL, fM, fN, fO, fP, fQ, fR, fS, fT, fU, fV, fW, fX, fY, fZ, ga, gb, gc, gd, ge, gf, gg, gh, gi, gj, gk, gl, gm, gn, go, gp, gq, gr, gs, gt, gu, gv, gw, gx, gy, gz, gA, gB, gC, gD, gE, gF, gG, gH, gI, gJ, gK, gL, gM, gN, gO, gP, gQ, gR, gS, gT, gU, gV, gW, gX, gY, gZ, ha, hb, hc, hd, he, hf, hg, hh, hi, hj, hk, hl, hm, hn, ho, hp, hq, hr, hs, ht, hu, hv, hw, hx, hy, hz, hA, hB, hC, hD, hE, hF, hG, hH, hI, hJ, hK, hL, hM, hN, hO, hP, hQ, hR, hS, hT, hU, hV, hW, hX, hY, hZ, ia, ib, ic, id, ie, if, ig, ih, ii, ij, ik, il, im, in, io, ip, iq, ir, is, it, iu, iv, iw, ix, iy, iz, iA, iB, iC, iD, iE, iF, iG, iH, iI, iJ, iK, iL, iM, iN, iO, iP, iQ, iR, iS, iT, iU, iV, iW, iX, iY, iZ, ja, jb, jc, jd, je, jf, jg, jh, ji, jj, jk, jl, jm, jn, jo, jp, jq, jr, js, jt, ju, jv, jw, jx, jy, jz, jA, jB, jC, jD, jE, jF, jG, jH, jI, jJ, jK, jL, jM, jN, jO, jP, jQ, jR, jS, jT, jU, jV, jW, jX, jY, jZ, ka, kb, kc, kd, ke, kf, kg, kh, ki, kj, kk, kl, km, kn, ko, kp, kq, kr, ks, kt, ku, kv, kw, kx, ky, kz, kA, kB, kC, kD, kE, kF, kG, kH, kI, kJ, kK, kL, kM, kN, kO, kP, kQ, kR, kS, kT, kU, kV, kW, kX, kY, kZ, la, lb, lc, ld, le, lf, lg, lh, li, lj, lk, ll, lm, ln, lo, lp, lq, lr, ls, lt, lu, lv, lw, lx, ly, lz, lA, lB, lC, lD, lE, lF, lG, lH, lI, lJ, lK, lL, lM, lN, lO, lP, lQ, lR, lS, lT, lU, lV, lW, lX, lY, lZ, ma, mb, mc, md, me, mf, mg, mh, mi, mj, mk, ml, mm, mn, mo, mp, mq, mr, ms, mt, mu, mv, mw, mx, my, mz, mA, mB, mC, mD, mE, mF, mG, mH, mI, mJ, mK, mL, mM, mN, mO, mP, mQ, mR, mS, mT, mU, mV, mW, mX, mY, mZ, na, nb, nc, nd, ne, nf, ng, nh, ni, nj, nk, nl, nm, nn, no, np, nq, nr, ns, nt, nu, nv, nw, nx, ny, nz, nA, nB, nC, nD, nE, nF, nG, nH, nI, nJ, nK, nL, nM, nN, nO, nP, nQ, nR, nS, nT, nU, nV, nW, nX, nY, nZ, oa, ob, oc, od, oe, of, og, oh, oi, oj, ok, ol, om, on, oo, op, oq, or, os, ot, ou, ov, ow, ox, oy, oz, oA, oB, oC, oD, oE, oF, oG, oH, oI, oJ, oK, oL, oM, oN, oO, oP, oQ, oR, oS, oT, oU, oV, oW, oX, oY, oZ, pa, pb, pc, pd, pe, pf, pg, ph, pi, pj, pk, pl, pm, pn, po, pp, pq, pr, ps, pt, pu, pv, pw, px, py, pz, pA, pB, pC, pD, pE, pF, pG, pH, pI, pJ, pK, pL, pM, pN, pO, pP, pQ, pR, pS, pT, pU, pV, pW, pX, pY, pZ, qa, qb, qc, qd, qe, qf, qg, qh, qi, qj, qk, ql, qm, qn, qo, qp, qq, qr, qs, qt, qu, qv, qw, qx, qy, qz, qA, qB, qC, qD, qE, qF, qG, qH, qI, qJ, qK, qL, qM, qN, qO, qP, qQ, qR, qS, qT, qU, qV, qW, qX, qY, qZ, ra, rb, rc, rd, re, rf, rg, rh, ri, rj, rk, rl, rm, rn, ro, rp, rq, rr, rs, rt, ru, rv, rw, rx, ry, rz, rA, rB, rC, rD, rE, rF, rG, rH, rI, rJ, rK, rL, rM, rN, rO, rP, rQ, rR, rS, rT, rU, rV, rW, rX, rY, rZ, sa, sb, sc, sd, se, sf, sg, sh, si, sj, sk, sl, sm, sn, so, sp, sq, sr, ss, st, su, sv, sw, sx, sy, sz, sA, sB, sC, sD, sE, sF, sG, sH, sI, sJ, sK, sL, sM, sN, sO, sP, sQ, sR, sS, sT, sU, sV, sW, sX, sY, sZ, ta, tb, tc, td, te, tf, tg, th, ti, tj, tk, tl, tm, tn, to, tp, tq, tr, ts, tt, tu, tv, tw, tx, ty, tz, tA, tB, tC, tD, tE, tF, tG, tH, tI)), (a b c d e f g h i j k l m n o p q r s t u v w x y z A B C D E F G H I J K L M N O P Q R S T U V W X Y Z ba bb bc bd be bf bg bh bi bj bk bl bm bn bo bp bq br bs bt bu bv bw bx by bz bA bB bC bD bE bF bG bH bI bJ bK bL bM bN bO bP bQ bR bS bT bU bV bW bX bY bZ ca cb cc cd ce cf cg ch ci cj ck cl cm cn co cp cq cr cs ct cu cv cw cx cy cz cA cB cC cD cE cF cG cH cI cJ cK cL cM cN cO cP cQ cR cS cT cU cV cW cX cY cZ da db dc dd de df dg dh di dj dk dl dm dn do dp dq dr ds dt du dv dw dx dy dz dA dB dC dD dE dF dG dH dI dJ dK dL dM dN dO dP dQ dR dS dT dU dV dW dX dY dZ ea eb ec ed ee ef eg eh ei ej ek el em en eo ep eq er es et eu ev ew ex ey ez eA eB eC eD eE eF eG eH eI eJ eK eL eM eN eO eP eQ eR eS eT eU eV eW eX eY eZ fa fb fc fd fe ff fg fh fi fj fk fl fm fn fo fp fq fr fs ft fu fv fw fx fy fz fA fB fC fD fE fF fG fH fI fJ fK fL fM fN fO fP fQ fR fS fT fU fV fW fX fY fZ ga gb gc gd ge gf gg gh gi gj gk gl gm gn go gp gq gr gs gt gu gv gw gx gy gz gA gB gC gD gE gF gG gH gI gJ gK gL gM gN gO gP gQ gR gS gT gU gV gW gX gY gZ ha hb hc hd he hf hg hh hi hj hk hl hm hn ho hp hq hr hs ht hu hv hw hx hy hz hA hB hC hD hE hF hG hH hI hJ hK hL hM hN hO hP hQ hR hS hT hU hV hW hX hY hZ ia ib ic id ie if ig ih ii ij ik il im in io ip iq ir is it iu iv iw ix iy iz iA iB iC iD iE iF iG iH iI iJ iK iL iM iN iO iP iQ iR iS iT iU iV iW iX iY iZ ja jb jc jd je jf jg jh ji jj jk jl jm jn jo jp jq jr js jt ju jv jw jx jy jz jA jB jC jD jE jF jG jH jI jJ jK jL jM jN jO jP jQ jR jS jT jU jV jW jX jY jZ ka kb kc kd ke kf kg kh ki kj kk kl km kn ko kp kq kr ks kt ku kv kw kx ky kz kA kB kC kD kE kF kG kH kI kJ kK kL kM kN kO kP kQ kR kS kT kU kV kW kX kY kZ la lb lc ld le lf lg lh li lj lk ll lm ln lo lp lq lr ls lt lu lv lw lx ly lz lA lB lC lD lE lF lG lH lI lJ lK lL lM lN lO lP lQ lR lS lT lU lV lW lX lY lZ ma mb mc md me mf mg mh mi mj mk ml mm mn mo mp mq mr ms mt mu mv mw mx my mz mA mB mC mD mE mF mG mH mI mJ mK mL mM mN mO mP mQ mR mS mT mU mV mW mX mY mZ na nb nc nd ne nf ng nh ni nj nk nl nm nn no np nq nr ns nt nu nv nw nx ny nz nA nB nC nD nE nF nG nH nI nJ nK nL nM nN nO nP nQ nR nS nT nU nV nW nX nY nZ oa ob oc od oe of og oh oi oj ok ol om on oo op oq or os ot ou ov ow ox oy oz oA oB oC oD oE oF oG oH oI oJ oK oL oM oN oO oP oQ oR oS oT oU oV oW oX oY oZ pa pb pc pd pe pf pg ph pi pj pk pl pm pn po pp pq pr ps pt pu pv pw px py pz pA pB pC pD pE pF pG pH pI pJ pK pL pM pN pO pP pQ pR pS pT pU pV pW pX pY pZ qa qb qc qd qe qf qg qh qi qj qk ql qm qn qo qp qq qr qs qt qu qv qw qx qy qz qA qB qC qD qE qF qG qH qI qJ qK qL qM qN qO qP qQ qR qS qT qU qV qW qX qY qZ ra rb rc rd re rf rg rh ri rj rk rl rm rn ro rp rq rr rs rt ru rv rw rx ry rz rA rB rC rD rE rF rG rH rI rJ rK rL rM rN rO rP rQ rR rS rT rU rV rW rX rY rZ sa sb sc sd se sf sg sh si sj sk sl sm sn so sp sq sr ss st su sv sw sx sy sz sA sB sC sD sE sF sG sH sI sJ sK sL sM sN sO sP sQ sR sS sT sU sV sW sX sY sZ ta tb tc td te tf tg th ti tj tk tl tm tn to tp tq tr ts tt tu tv tw tx ty tz tA tB tC tD tE tF tG tH tI));

ASSERT_PP_EQ((PTL_IF(1, (t), ())), (t));
ASSERT_PP_EQ((PTL_IF(0, (t), ())), ());
ASSERT_PP_EQ((PTL_IF(1, (t), (f))), (t));
ASSERT_PP_EQ((PTL_IF(0, (t), (f))), (f));
ASSERT_PP_EQ((PTL_IF(1, (a), (b, c))), (a));
ASSERT_PP_EQ((PTL_IF(0, (a), (b, c))), (b, c));

ASSERT_PP_EQ((PTL_SWITCH(0, (1))), (1));
ASSERT_PP_EQ((PTL_SWITCH(1, (1), (2))), (2));
ASSERT_PP_EQ((PTL_SWITCH(2, (1), (2), (3, 4))), (3, 4));
ASSERT_PP_EQ((PTL_SWITCH(0, (1), (2))), (1));
ASSERT_PP_EQ((PTL_SWITCH(1, (1), (2), (3))), (2));
ASSERT_PP_EQ((PTL_SWITCH(2, (1), (2), (3))), (3));
ASSERT_PP_EQ((PTL_SWITCH(0, (a), (b), (c), (d), (e), (f), (g), (h), (i), (j), (k), (l), (m), (n), (o), (p), (q), (r), (s), (t), (u), (v), (w), (x), (y), (z), (A), (B), (C), (D), (E), (F), (G), (H), (I), (J), (K), (L), (M), (N), (O), (P), (Q), (R), (S), (T), (U), (V), (W), (X), (Y), (Z), (ba), (bb), (bc), (bd), (be), (bf), (bg), (bh), (bi), (bj), (bk), (bl), (bm), (bn), (bo), (bp), (bq), (br), (bs), (bt), (bu), (bv), (bw), (bx), (by), (bz), (bA), (bB), (bC), (bD), (bE), (bF), (bG), (bH), (bI), (bJ), (bK), (bL), (bM), (bN), (bO), (bP), (bQ), (bR), (bS), (bT), (bU), (bV), (bW), (bX), (bY), (bZ), (ca), (cb), (cc), (cd), (ce), (cf), (cg), (ch), (ci), (cj), (ck), (cl), (cm), (cn), (co), (cp), (cq), (cr), (cs), (ct), (cu), (cv), (cw), (cx), (cy), (cz), (cA), (cB), (cC), (cD), (cE), (cF), (cG), (cH), (cI), (cJ), (cK), (cL), (cM), (cN), (cO), (cP), (cQ), (cR), (cS), (cT), (cU), (cV), (cW), (cX), (cY), (cZ), (da), (db), (dc), (dd), (de), (df), (dg), (dh), (di), (dj), (dk), (dl), (dm), (dn), (do), (dp), (dq), (dr), (ds), (dt), (du), (dv), (dw), (dx), (dy), (dz), (dA), (dB), (dC), (dD), (dE), (dF), (dG), (dH), (dI), (dJ), (dK), (dL), (dM), (dN), (dO), (dP), (dQ), (dR), (dS), (dT), (dU), (dV), (dW), (dX), (dY), (dZ), (ea), (eb), (ec), (ed), (ee), (ef), (eg), (eh), (ei), (ej), (ek), (el), (em), (en), (eo), (ep), (eq), (er), (es), (et), (eu), (ev), (ew), (ex), (ey), (ez), (eA), (eB), (eC), (eD), (eE), (eF), (eG), (eH), (eI), (eJ), (eK), (eL), (eM), (eN), (eO), (eP), (eQ), (eR), (eS), (eT), (eU), (eV), (eW), (eX), (eY), (eZ), (fa), (fb), (fc), (fd), (fe), (ff), (fg), (fh), (fi), (fj), (fk), (fl), (fm), (fn), (fo), (fp), (fq), (fr), (fs), (ft), (fu), (fv), (fw), (fx), (fy), (fz), (fA), (fB), (fC), (fD), (fE), (fF), (fG), (fH), (fI), (fJ), (fK), (fL), (fM), (fN), (fO), (fP), (fQ), (fR), (fS), (fT), (fU), (fV), (fW), (fX), (fY), (fZ), (ga), (gb), (gc), (gd), (ge), (gf), (gg), (gh), (gi), (gj), (gk), (gl), (gm), (gn), (go), (gp), (gq), (gr), (gs), (gt), (gu), (gv), (gw), (gx), (gy), (gz), (gA), (gB), (gC), (gD), (gE), (gF), (gG), (gH), (gI), (gJ), (gK), (gL), (gM), (gN), (gO), (gP), (gQ), (gR), (gS), (gT), (gU), (gV), (gW), (gX), (gY), (gZ), (ha), (hb), (hc), (hd), (he), (hf), (hg), (hh), (hi), (hj), (hk), (hl), (hm), (hn), (ho), (hp), (hq), (hr), (hs), (ht), (hu), (hv), (hw), (hx), (hy), (hz), (hA), (hB), (hC), (hD), (hE), (hF), (hG), (hH), (hI), (hJ), (hK), (hL), (hM), (hN), (hO), (hP), (hQ), (hR), (hS), (hT), (hU), (hV), (hW), (hX), (hY), (hZ), (ia), (ib), (ic), (id), (ie), (if), (ig), (ih), (ii), (ij), (ik), (il), (im), (in), (io), (ip), (iq), (ir), (is), (it), (iu), (iv), (iw), (ix), (iy), (iz), (iA), (iB), (iC), (iD), (iE), (iF), (iG), (iH), (iI), (iJ), (iK), (iL), (iM), (iN), (iO), (iP), (iQ), (iR), (iS), (iT), (iU), (iV), (iW), (iX), (iY), (iZ), (ja), (jb), (jc), (jd), (je), (jf), (jg), (jh), (ji), (jj), (jk), (jl), (jm), (jn), (jo), (jp), (jq), (jr), (js), (jt), (ju), (jv), (jw), (jx), (jy), (jz), (jA), (jB), (jC), (jD), (jE), (jF), (jG), (jH), (jI), (jJ), (jK), (jL), (jM), (jN), (jO), (jP), (jQ), (jR), (jS), (jT), (jU), (jV), (jW), (jX), (jY), (jZ), (ka), (kb), (kc), (kd), (ke), (kf), (kg), (kh), (ki), (kj), (kk), (kl), (km), (kn), (ko), (kp), (kq), (kr), (ks), (kt), (ku), (kv), (kw), (kx), (ky), (kz), (kA), (kB), (kC), (kD), (kE), (kF), (kG), (kH), (kI), (kJ), (kK), (kL), (kM), (kN), (kO), (kP), (kQ), (kR), (kS), (kT), (kU), (kV), (kW), (kX), (kY), (kZ), (la), (lb), (lc), (ld), (le), (lf), (lg), (lh), (li), (lj), (lk), (ll), (lm), (ln), (lo), (lp), (lq), (lr), (ls), (lt), (lu), (lv), (lw), (lx), (ly), (lz), (lA), (lB), (lC), (lD), (lE), (lF), (lG), (lH), (lI), (lJ), (lK), (lL), (lM), (lN), (lO), (lP), (lQ), (lR), (lS), (lT), (lU), (lV), (lW), (lX), (lY), (lZ), (ma), (mb), (mc), (md), (me), (mf), (mg), (mh), (mi), (mj), (mk), (ml), (mm), (mn), (mo), (mp), (mq), (mr), (ms), (mt), (mu), (mv), (mw), (mx), (my), (mz), (mA), (mB), (mC), (mD), (mE), (mF), (mG), (mH), (mI), (mJ), (mK), (mL), (mM), (mN), (mO), (mP), (mQ), (mR), (mS), (mT), (mU), (mV), (mW), (mX), (mY), (mZ), (na), (nb), (nc), (nd), (ne), (nf), (ng), (nh), (ni), (nj), (nk), (nl), (nm), (nn), (no), (np), (nq), (nr), (ns), (nt), (nu), (nv), (nw), (nx), (ny), (nz), (nA), (nB), (nC), (nD), (nE), (nF), (nG), (nH), (nI), (nJ), (nK), (nL), (nM), (nN), (nO), (nP), (nQ), (nR), (nS), (nT), (nU), (nV), (nW), (nX), (nY), (nZ), (oa), (ob), (oc), (od), (oe), (of), (og), (oh), (oi), (oj), (ok), (ol), (om), (on), (oo), (op), (oq), (or), (os), (ot), (ou), (ov), (ow), (ox), (oy), (oz), (oA), (oB), (oC), (oD), (oE), (oF), (oG), (oH), (oI), (oJ), (oK), (oL), (oM), (oN), (oO), (oP), (oQ), (oR), (oS), (oT), (oU), (oV), (oW), (oX), (oY), (oZ), (pa), (pb), (pc), (pd), (pe), (pf), (pg), (ph), (pi), (pj), (pk), (pl), (pm), (pn), (po), (pp), (pq), (pr), (ps), (pt), (pu), (pv), (pw), (px), (py), (pz), (pA), (pB), (pC), (pD), (pE), (pF), (pG), (pH), (pI), (pJ), (pK), (pL), (pM), (pN), (pO), (pP), (pQ), (pR), (pS), (pT), (pU), (pV), (pW), (pX), (pY), (pZ), (qa), (qb), (qc), (qd), (qe), (qf), (qg), (qh), (qi), (qj), (qk), (ql), (qm), (qn), (qo), (qp), (qq), (qr), (qs), (qt), (qu), (qv), (qw), (qx), (qy), (qz), (qA), (qB), (qC), (qD), (qE), (qF), (qG), (qH), (qI), (qJ), (qK), (qL), (qM), (qN), (qO), (qP), (qQ), (qR), (qS), (qT), (qU), (qV), (qW), (qX), (qY), (qZ), (ra), (rb), (rc), (rd), (re), (rf), (rg), (rh), (ri), (rj), (rk), (rl), (rm), (rn), (ro), (rp), (rq), (rr), (rs), (rt), (ru), (rv), (rw), (rx), (ry), (rz), (rA), (rB), (rC), (rD), (rE), (rF), (rG), (rH), (rI), (rJ), (rK), (rL), (rM), (rN), (rO), (rP), (rQ), (rR), (rS), (rT), (rU), (rV), (rW), (rX), (rY), (rZ), (sa), (sb), (sc), (sd), (se), (sf), (sg), (sh), (si), (sj), (sk), (sl), (sm), (sn), (so), (sp), (sq), (sr), (ss), (st), (su), (sv), (sw), (sx), (sy), (sz), (sA), (sB), (sC), (sD), (sE), (sF), (sG), (sH), (sI), (sJ), (sK), (sL), (sM), (sN), (sO), (sP), (sQ), (sR), (sS), (sT), (sU), (sV), (sW), (sX), (sY), (sZ), (ta), (tb), (tc), (td), (te), (tf), (tg), (th), (ti), (tj), (tk), (tl), (tm), (tn), (to), (tp), (tq), (tr), (ts), (tt), (tu), (tv), (tw), (tx), (ty), (tz), (tA), (tB), (tC), (tD), (tE), (tF), (tG), (tH), (tI), (tJ))), (a));
ASSERT_PP_EQ((PTL_SWITCH(1, (a), (b), (c), (d), (e), (f), (g), (h), (i), (j), (k), (l), (m), (n), (o), (p), (q), (r), (s), (t), (u), (v), (w), (x), (y), (z), (A), (B), (C), (D), (E), (F), (G), (H), (I), (J), (K), (L), (M), (N), (O), (P), (Q), (R), (S), (T), (U), (V), (W), (X), (Y), (Z), (ba), (bb), (bc), (bd), (be), (bf), (bg), (bh), (bi), (bj), (bk), (bl), (bm), (bn), (bo), (bp), (bq), (br), (bs), (bt), (bu), (bv), (bw), (bx), (by), (bz), (bA), (bB), (bC), (bD), (bE), (bF), (bG), (bH), (bI), (bJ), (bK), (bL), (bM), (bN), (bO), (bP), (bQ), (bR), (bS), (bT), (bU), (bV), (bW), (bX), (bY), (bZ), (ca), (cb), (cc), (cd), (ce), (cf), (cg), (ch), (ci), (cj), (ck), (cl), (cm), (cn), (co), (cp), (cq), (cr), (cs), (ct), (cu), (cv), (cw), (cx), (cy), (cz), (cA), (cB), (cC), (cD), (cE), (cF), (cG), (cH), (cI), (cJ), (cK), (cL), (cM), (cN), (cO), (cP), (cQ), (cR), (cS), (cT), (cU), (cV), (cW), (cX), (cY), (cZ), (da), (db), (dc), (dd), (de), (df), (dg), (dh), (di), (dj), (dk), (dl), (dm), (dn), (do), (dp), (dq), (dr), (ds), (dt), (du), (dv), (dw), (dx), (dy), (dz), (dA), (dB), (dC), (dD), (dE), (dF), (dG), (dH), (dI), (dJ), (dK), (dL), (dM), (dN), (dO), (dP), (dQ), (dR), (dS), (dT), (dU), (dV), (dW), (dX), (dY), (dZ), (ea), (eb), (ec), (ed), (ee), (ef), (eg), (eh), (ei), (ej), (ek), (el), (em), (en), (eo), (ep), (eq), (er), (es), (et), (eu), (ev), (ew), (ex), (ey), (ez), (eA), (eB), (eC), (eD), (eE), (eF), (eG), (eH), (eI), (eJ), (eK), (eL), (eM), (eN), (eO), (eP), (eQ), (eR), (eS), (eT), (eU), (eV), (eW), (eX), (eY), (eZ), (fa), (fb), (fc), (fd), (fe), (ff), (fg), (fh), (fi), (fj), (fk), (fl), (fm), (fn), (fo), (fp), (fq), (fr), (fs), (ft), (fu), (fv), (fw), (fx), (fy), (fz), (fA), (fB), (fC), (fD), (fE), (fF), (fG), (fH), (fI), (fJ), (fK), (fL), (fM), (fN), (fO), (fP), (fQ), (fR), (fS), (fT), (fU), (fV), (fW), (fX), (fY), (fZ), (ga), (gb), (gc), (gd), (ge), (gf), (gg), (gh), (gi), (gj), (gk), (gl), (gm), (gn), (go), (gp), (gq), (gr), (gs), (gt), (gu), (gv), (gw), (gx), (gy), (gz), (gA), (gB), (gC), (gD), (gE), (gF), (gG), (gH), (gI), (gJ), (gK), (gL), (gM), (gN), (gO), (gP), (gQ), (gR), (gS), (gT), (gU), (gV), (gW), (gX), (gY), (gZ), (ha), (hb), (hc), (hd), (he), (hf), (hg), (hh), (hi), (hj), (hk), (hl), (hm), (hn), (ho), (hp), (hq), (hr), (hs), (ht), (hu), (hv), (hw), (hx), (hy), (hz), (hA), (hB), (hC), (hD), (hE), (hF), (hG), (hH), (hI), (hJ), (hK), (hL), (hM), (hN), (hO), (hP), (hQ), (hR), (hS), (hT), (hU), (hV), (hW), (hX), (hY), (hZ), (ia), (ib), (ic), (id), (ie), (if), (ig), (ih), (ii), (ij), (ik), (il), (im), (in), (io), (ip), (iq), (ir), (is), (it), (iu), (iv), (iw), (ix), (iy), (iz), (iA), (iB), (iC), (iD), (iE), (iF), (iG), (iH), (iI), (iJ), (iK), (iL), (iM), (iN), (iO), (iP), (iQ), (iR), (iS), (iT), (iU), (iV), (iW), (iX), (iY), (iZ), (ja), (jb), (jc), (jd), (je), (jf), (jg), (jh), (ji), (jj), (jk), (jl), (jm), (jn), (jo), (jp), (jq), (jr), (js), (jt), (ju), (jv), (jw), (jx), (jy), (jz), (jA), (jB), (jC), (jD), (jE), (jF), (jG), (jH), (jI), (jJ), (jK), (jL), (jM), (jN), (jO), (jP), (jQ), (jR), (jS), (jT), (jU), (jV), (jW), (jX), (jY), (jZ), (ka), (kb), (kc), (kd), (ke), (kf), (kg), (kh), (ki), (kj), (kk), (kl), (km), (kn), (ko), (kp), (kq), (kr), (ks), (kt), (ku), (kv), (kw), (kx), (ky), (kz), (kA), (kB), (kC), (kD), (kE), (kF), (kG), (kH), (kI), (kJ), (kK), (kL), (kM), (kN), (kO), (kP), (kQ), (kR), (kS), (kT), (kU), (kV), (kW), (kX), (kY), (kZ), (la), (lb), (lc), (ld), (le), (lf), (lg), (lh), (li), (lj), (lk), (ll), (lm), (ln), (lo), (lp), (lq), (lr), (ls), (lt), (lu), (lv), (lw), (lx), (ly), (lz), (lA), (lB), (lC), (lD), (lE), (lF), (lG), (lH), (lI), (lJ), (lK), (lL), (lM), (lN), (lO), (lP), (lQ), (lR), (lS), (lT), (lU), (lV), (lW), (lX), (lY), (lZ), (ma), (mb), (mc), (md), (me), (mf), (mg), (mh), (mi), (mj), (mk), (ml), (mm), (mn), (mo), (mp), (mq), (mr), (ms), (mt), (mu), (mv), (mw), (mx), (my), (mz), (mA), (mB), (mC), (mD), (mE), (mF), (mG), (mH), (mI), (mJ), (mK), (mL), (mM), (mN), (mO), (mP), (mQ), (mR), (mS), (mT), (mU), (mV), (mW), (mX), (mY), (mZ), (na), (nb), (nc), (nd), (ne), (nf), (ng), (nh), (ni), (nj), (nk), (nl), (nm), (nn), (no), (np), (nq), (nr), (ns), (nt), (nu), (nv), (nw), (nx), (ny), (nz), (nA), (nB), (nC), (nD), (nE), (nF), (nG), (nH), (nI), (nJ), (nK), (nL), (nM), (nN), (nO), (nP), (nQ), (nR), (nS), (nT), (nU), (nV), (nW), (nX), (nY), (nZ), (oa), (ob), (oc), (od), (oe), (of), (og), (oh), (oi), (oj), (ok), (ol), (om), (on), (oo), (op), (oq), (or), (os), (ot), (ou), (ov), (ow), (ox), (oy), (oz), (oA), (oB), (oC), (oD), (oE), (oF), (oG), (oH), (oI), (oJ), (oK), (oL), (oM), (oN), (oO), (oP), (oQ), (oR), (oS), (oT), (oU), (oV), (oW), (oX), (oY), (oZ), (pa), (pb), (pc), (pd), (pe), (pf), (pg), (ph), (pi), (pj), (pk), (pl), (pm), (pn), (po), (pp), (pq), (pr), (ps), (pt), (pu), (pv), (pw), (px), (py), (pz), (pA), (pB), (pC), (pD), (pE), (pF), (pG), (pH), (pI), (pJ), (pK), (pL), (pM), (pN), (pO), (pP), (pQ), (pR), (pS), (pT), (pU), (pV), (pW), (pX), (pY), (pZ), (qa), (qb), (qc), (qd), (qe), (qf), (qg), (qh), (qi), (qj), (qk), (ql), (qm), (qn), (qo), (qp), (qq), (qr), (qs), (qt), (qu), (qv), (qw), (qx), (qy), (qz), (qA), (qB), (qC), (qD), (qE), (qF), (qG), (qH), (qI), (qJ), (qK), (qL), (qM), (qN), (qO), (qP), (qQ), (qR), (qS), (qT), (qU), (qV), (qW), (qX), (qY), (qZ), (ra), (rb), (rc), (rd), (re), (rf), (rg), (rh), (ri), (rj), (rk), (rl), (rm), (rn), (ro), (rp), (rq), (rr), (rs), (rt), (ru), (rv), (rw), (rx), (ry), (rz), (rA), (rB), (rC), (rD), (rE), (rF), (rG), (rH), (rI), (rJ), (rK), (rL), (rM), (rN), (rO), (rP), (rQ), (rR), (rS), (rT), (rU), (rV), (rW), (rX), (rY), (rZ), (sa), (sb), (sc), (sd), (se), (sf), (sg), (sh), (si), (sj), (sk), (sl), (sm), (sn), (so), (sp), (sq), (sr), (ss), (st), (su), (sv), (sw), (sx), (sy), (sz), (sA), (sB), (sC), (sD), (sE), (sF), (sG), (sH), (sI), (sJ), (sK), (sL), (sM), (sN), (sO), (sP), (sQ), (sR), (sS), (sT), (sU), (sV), (sW), (sX), (sY), (sZ), (ta), (tb), (tc), (td), (te), (tf), (tg), (th), (ti), (tj), (tk), (tl), (tm), (tn), (to), (tp), (tq), (tr), (ts), (tt), (tu), (tv), (tw), (tx), (ty), (tz), (tA), (tB), (tC), (tD), (tE), (tF), (tG), (tH), (tI), (tJ))), (b));
ASSERT_PP_EQ((PTL_SWITCH(1023, (a), (b), (c), (d), (e), (f), (g), (h), (i), (j), (k), (l), (m), (n), (o), (p), (q), (r), (s), (t), (u), (v), (w), (x), (y), (z), (A), (B), (C), (D), (E), (F), (G), (H), (I), (J), (K), (L), (M), (N), (O), (P), (Q), (R), (S), (T), (U), (V), (W), (X), (Y), (Z), (ba), (bb), (bc), (bd), (be), (bf), (bg), (bh), (bi), (bj), (bk), (bl), (bm), (bn), (bo), (bp), (bq), (br), (bs), (bt), (bu), (bv), (bw), (bx), (by), (bz), (bA), (bB), (bC), (bD), (bE), (bF), (bG), (bH), (bI), (bJ), (bK), (bL), (bM), (bN), (bO), (bP), (bQ), (bR), (bS), (bT), (bU), (bV), (bW), (bX), (bY), (bZ), (ca), (cb), (cc), (cd), (ce), (cf), (cg), (ch), (ci), (cj), (ck), (cl), (cm), (cn), (co), (cp), (cq), (cr), (cs), (ct), (cu), (cv), (cw), (cx), (cy), (cz), (cA), (cB), (cC), (cD), (cE), (cF), (cG), (cH), (cI), (cJ), (cK), (cL), (cM), (cN), (cO), (cP), (cQ), (cR), (cS), (cT), (cU), (cV), (cW), (cX), (cY), (cZ), (da), (db), (dc), (dd), (de), (df), (dg), (dh), (di), (dj), (dk), (dl), (dm), (dn), (do), (dp), (dq), (dr), (ds), (dt), (du), (dv), (dw), (dx), (dy), (dz), (dA), (dB), (dC), (dD), (dE), (dF), (dG), (dH), (dI), (dJ), (dK), (dL), (dM), (dN), (dO), (dP), (dQ), (dR), (dS), (dT), (dU), (dV), (dW), (dX), (dY), (dZ), (ea), (eb), (ec), (ed), (ee), (ef), (eg), (eh), (ei), (ej), (ek), (el), (em), (en), (eo), (ep), (eq), (er), (es), (et), (eu), (ev), (ew), (ex), (ey), (ez), (eA), (eB), (eC), (eD), (eE), (eF), (eG), (eH), (eI), (eJ), (eK), (eL), (eM), (eN), (eO), (eP), (eQ), (eR), (eS), (eT), (eU), (eV), (eW), (eX), (eY), (eZ), (fa), (fb), (fc), (fd), (fe), (ff), (fg), (fh), (fi), (fj), (fk), (fl), (fm), (fn), (fo), (fp), (fq), (fr), (fs), (ft), (fu), (fv), (fw), (fx), (fy), (fz), (fA), (fB), (fC), (fD), (fE), (fF), (fG), (fH), (fI), (fJ), (fK), (fL), (fM), (fN), (fO), (fP), (fQ), (fR), (fS), (fT), (fU), (fV), (fW), (fX), (fY), (fZ), (ga), (gb), (gc), (gd), (ge), (gf), (gg), (gh), (gi), (gj), (gk), (gl), (gm), (gn), (go), (gp), (gq), (gr), (gs), (gt), (gu), (gv), (gw), (gx), (gy), (gz), (gA), (gB), (gC), (gD), (gE), (gF), (gG), (gH), (gI), (gJ), (gK), (gL), (gM), (gN), (gO), (gP), (gQ), (gR), (gS), (gT), (gU), (gV), (gW), (gX), (gY), (gZ), (ha), (hb), (hc), (hd), (he), (hf), (hg), (hh), (hi), (hj), (hk), (hl), (hm), (hn), (ho), (hp), (hq), (hr), (hs), (ht), (hu), (hv), (hw), (hx), (hy), (hz), (hA), (hB), (hC), (hD), (hE), (hF), (hG), (hH), (hI), (hJ), (hK), (hL), (hM), (hN), (hO), (hP), (hQ), (hR), (hS), (hT), (hU), (hV), (hW), (hX), (hY), (hZ), (ia), (ib), (ic), (id), (ie), (if), (ig), (ih), (ii), (ij), (ik), (il), (im), (in), (io), (ip), (iq), (ir), (is), (it), (iu), (iv), (iw), (ix), (iy), (iz), (iA), (iB), (iC), (iD), (iE), (iF), (iG), (iH), (iI), (iJ), (iK), (iL), (iM), (iN), (iO), (iP), (iQ), (iR), (iS), (iT), (iU), (iV), (iW), (iX), (iY), (iZ), (ja), (jb), (jc), (jd), (je), (jf), (jg), (jh), (ji), (jj), (jk), (jl), (jm), (jn), (jo), (jp), (jq), (jr), (js), (jt), (ju), (jv), (jw), (jx), (jy), (jz), (jA), (jB), (jC), (jD), (jE), (jF), (jG), (jH), (jI), (jJ), (jK), (jL), (jM), (jN), (jO), (jP), (jQ), (jR), (jS), (jT), (jU), (jV), (jW), (jX), (jY), (jZ), (ka), (kb), (kc), (kd), (ke), (kf), (kg), (kh), (ki), (kj), (kk), (kl), (km), (kn), (ko), (kp), (kq), (kr), (ks), (kt), (ku), (kv), (kw), (kx), (ky), (kz), (kA), (kB), (kC), (kD), (kE), (kF), (kG), (kH), (kI), (kJ), (kK), (kL), (kM), (kN), (kO), (kP), (kQ), (kR), (kS), (kT), (kU), (kV), (kW), (kX), (kY), (kZ), (la), (lb), (lc), (ld), (le), (lf), (lg), (lh), (li), (lj), (lk), (ll), (lm), (ln), (lo), (lp), (lq), (lr), (ls), (lt), (lu), (lv), (lw), (lx), (ly), (lz), (lA), (lB), (lC), (lD), (lE), (lF), (lG), (lH), (lI), (lJ), (lK), (lL), (lM), (lN), (lO), (lP), (lQ), (lR), (lS), (lT), (lU), (lV), (lW), (lX), (lY), (lZ), (ma), (mb), (mc), (md), (me), (mf), (mg), (mh), (mi), (mj), (mk), (ml), (mm), (mn), (mo), (mp), (mq), (mr), (ms), (mt), (mu), (mv), (mw), (mx), (my), (mz), (mA), (mB), (mC), (mD), (mE), (mF), (mG), (mH), (mI), (mJ), (mK), (mL), (mM), (mN), (mO), (mP), (mQ), (mR), (mS), (mT), (mU), (mV), (mW), (mX), (mY), (mZ), (na), (nb), (nc), (nd), (ne), (nf), (ng), (nh), (ni), (nj), (nk), (nl), (nm), (nn), (no), (np), (nq), (nr), (ns), (nt), (nu), (nv), (nw), (nx), (ny), (nz), (nA), (nB), (nC), (nD), (nE), (nF), (nG), (nH), (nI), (nJ), (nK), (nL), (nM), (nN), (nO), (nP), (nQ), (nR), (nS), (nT), (nU), (nV), (nW), (nX), (nY), (nZ), (oa), (ob), (oc), (od), (oe), (of), (og), (oh), (oi), (oj), (ok), (ol), (om), (on), (oo), (op), (oq), (or), (os), (ot), (ou), (ov), (ow), (ox), (oy), (oz), (oA), (oB), (oC), (oD), (oE), (oF), (oG), (oH), (oI), (oJ), (oK), (oL), (oM), (oN), (oO), (oP), (oQ), (oR), (oS), (oT), (oU), (oV), (oW), (oX), (oY), (oZ), (pa), (pb), (pc), (pd), (pe), (pf), (pg), (ph), (pi), (pj), (pk), (pl), (pm), (pn), (po), (pp), (pq), (pr), (ps), (pt), (pu), (pv), (pw), (px), (py), (pz), (pA), (pB), (pC), (pD), (pE), (pF), (pG), (pH), (pI), (pJ), (pK), (pL), (pM), (pN), (pO), (pP), (pQ), (pR), (pS), (pT), (pU), (pV), (pW), (pX), (pY), (pZ), (qa), (qb), (qc), (qd), (qe), (qf), (qg), (qh), (qi), (qj), (qk), (ql), (qm), (qn), (qo), (qp), (qq), (qr), (qs), (qt), (qu), (qv), (qw), (qx), (qy), (qz), (qA), (qB), (qC), (qD), (qE), (qF), (qG), (qH), (qI), (qJ), (qK), (qL), (qM), (qN), (qO), (qP), (qQ), (qR), (qS), (qT), (qU), (qV), (qW), (qX), (qY), (qZ), (ra), (rb), (rc), (rd), (re), (rf), (rg), (rh), (ri), (rj), (rk), (rl), (rm), (rn), (ro), (rp), (rq), (rr), (rs), (rt), (ru), (rv), (rw), (rx), (ry), (rz), (rA), (rB), (rC), (rD), (rE), (rF), (rG), (rH), (rI), (rJ), (rK), (rL), (rM), (rN), (rO), (rP), (rQ), (rR), (rS), (rT), (rU), (rV), (rW), (rX), (rY), (rZ), (sa), (sb), (sc), (sd), (se), (sf), (sg), (sh), (si), (sj), (sk), (sl), (sm), (sn), (so), (sp), (sq), (sr), (ss), (st), (su), (sv), (sw), (sx), (sy), (sz), (sA), (sB), (sC), (sD), (sE), (sF), (sG), (sH), (sI), (sJ), (sK), (sL), (sM), (sN), (sO), (sP), (sQ), (sR), (sS), (sT), (sU), (sV), (sW), (sX), (sY), (sZ), (ta), (tb), (tc), (td), (te), (tf), (tg), (th), (ti), (tj), (tk), (tl), (tm), (tn), (to), (tp), (tq), (tr), (ts), (tt), (tu), (tv), (tw), (tx), (ty), (tz), (tA), (tB), (tC), (tD), (tE), (tF), (tG), (tH), (tI), (tJ))), (tJ));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(0, (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(1, (), (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(2, (), (), (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(3, (), (), (), (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(4, (), (), (), (), (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(5, (), (), (), (), (), (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(6, (), (), (), (), (), (), (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(7, (), (), (), (), (), (), (), (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,,,,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(8, (), (), (), (), (), (), (), (), (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,,,,,,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(9, (), (), (), (), (), (), (), (), (), (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,,,,,,,,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(10, (), (), (), (), (), (), (), (), (), (), (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,,,,,,,,,,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(11, (), (), (), (), (), (), (), (), (), (), (), (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,,,,,,,,,,,,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(12, (), (), (), (), (), (), (), (), (), (), (), (), (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,,,,,,,,,,,,,,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(13, (), (), (), (), (), (), (), (), (), (), (), (), (), (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,, )"));
ASSERT_PP_EQ((PTL_STR(PTL_SWITCH(14, (), (), (), (), (), (), (), (), (), (), (), (), (), (), (PTL_XTRACE)))), ("PPUTLXTRACE_A ( ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,, )"));

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
// clang-format on

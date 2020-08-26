#ifndef PPUTL_CAT_H_INCLUDED
#define PPUTL_CAT_H_INCLUDED
#/////                                                                     c++20
#///////////////////////////////////////////////////////////////////////////////
#/// @brief \link PPUTL_CAT PPUTL_CAT\endlink -
#/// concatenate [0, 256) args
#/// @file
#//                          __    ___
#//                         /\ \__/\_ \
#//   _____   _____   __  __\ \ ,_\//\ \
#//  /\ '__`\/\ '__`\/\ \/\ \\ \ \/ \ \ \
#//  \ \ \_\ \ \ \_\ \ \ \_\ \\ \ \_ \_\ \_
#//   \ \ ,__/\ \ ,__/\ \____/ \ \__\/\____\
#//    \ \ \   \ \ \   \/___/   \/__/\/____/
#//     \/_/    \/_/
#//
#//  pputl Preprocessor Utilities
#//  Copyright (C) 2020 Justin Collier <m@jpcx.dev>
#//
#//    This program is free software: you can redistribute it and/or modify
#//    it under the terms of the GNU General Public License as published by
#//    the Free Software Foundation, either version 3 of the License, or
#//    (at your option) any later version.
#//
#//    This program is distributed in the hope that it will be useful,
#//    but WITHOUT ANY WARRANTY; without even the internalied warranty of
#//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#//    GNU General Public License for more details.
#//                                                                           //
#//  You should have received a copy of the GNU General Public License       ///
#//  along with this program.  If not, see <https://www.gnu.org/licenses/>. ////
#///////////////////////////////////////////////////////////////////////////////
#
#include "pputl/nargs.h"
#
#/// concatenate [0, 256) args
#/// @ingroup pputl
#/// @anchor  PPUTL_CAT
#define PPUTL_CAT(...) PPUTL_DETAIL_CAT_X(__VA_ARGS__)
#
#define PPUTL_DETAIL_CAT_X(...)                                                \
  PPUTL_DETAIL_CAT_CHOOSER(PPUTL_NARGS(__VA_ARGS__))                           \
  (__VA_ARGS__)
#define PPUTL_DETAIL_CAT_CHOOSER(nargs) PPUTL_DETAIL_CAT_CHOOSER_X(nargs)
#define PPUTL_DETAIL_CAT_CHOOSER_X(nargs) PPUTL_DETAIL_CAT_##nargs
#
#// clang-format off
#define PPUTL_DETAIL_CAT_0()
#define PPUTL_DETAIL_CAT_1(a)           a
#define PPUTL_DETAIL_CAT_2(a, b)        a##b
#define PPUTL_DETAIL_CAT_3(a, b, ...)   PPUTL_DETAIL_CAT_2(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_4(a, b, ...)   PPUTL_DETAIL_CAT_3(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_5(a, b, ...)   PPUTL_DETAIL_CAT_4(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_6(a, b, ...)   PPUTL_DETAIL_CAT_5(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_7(a, b, ...)   PPUTL_DETAIL_CAT_6(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_8(a, b, ...)   PPUTL_DETAIL_CAT_7(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_9(a, b, ...)   PPUTL_DETAIL_CAT_8(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_10(a, b, ...)  PPUTL_DETAIL_CAT_9(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_11(a, b, ...)  PPUTL_DETAIL_CAT_10(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_12(a, b, ...)  PPUTL_DETAIL_CAT_11(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_13(a, b, ...)  PPUTL_DETAIL_CAT_12(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_14(a, b, ...)  PPUTL_DETAIL_CAT_13(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_15(a, b, ...)  PPUTL_DETAIL_CAT_14(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_16(a, b, ...)  PPUTL_DETAIL_CAT_15(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_17(a, b, ...)  PPUTL_DETAIL_CAT_16(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_18(a, b, ...)  PPUTL_DETAIL_CAT_17(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_19(a, b, ...)  PPUTL_DETAIL_CAT_18(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_20(a, b, ...)  PPUTL_DETAIL_CAT_19(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_21(a, b, ...)  PPUTL_DETAIL_CAT_20(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_22(a, b, ...)  PPUTL_DETAIL_CAT_21(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_23(a, b, ...)  PPUTL_DETAIL_CAT_22(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_24(a, b, ...)  PPUTL_DETAIL_CAT_23(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_25(a, b, ...)  PPUTL_DETAIL_CAT_24(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_26(a, b, ...)  PPUTL_DETAIL_CAT_25(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_27(a, b, ...)  PPUTL_DETAIL_CAT_26(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_28(a, b, ...)  PPUTL_DETAIL_CAT_27(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_29(a, b, ...)  PPUTL_DETAIL_CAT_28(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_30(a, b, ...)  PPUTL_DETAIL_CAT_29(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_31(a, b, ...)  PPUTL_DETAIL_CAT_30(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_32(a, b, ...)  PPUTL_DETAIL_CAT_31(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_33(a, b, ...)  PPUTL_DETAIL_CAT_32(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_34(a, b, ...)  PPUTL_DETAIL_CAT_33(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_35(a, b, ...)  PPUTL_DETAIL_CAT_34(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_36(a, b, ...)  PPUTL_DETAIL_CAT_35(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_37(a, b, ...)  PPUTL_DETAIL_CAT_36(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_38(a, b, ...)  PPUTL_DETAIL_CAT_37(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_39(a, b, ...)  PPUTL_DETAIL_CAT_38(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_40(a, b, ...)  PPUTL_DETAIL_CAT_39(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_41(a, b, ...)  PPUTL_DETAIL_CAT_40(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_42(a, b, ...)  PPUTL_DETAIL_CAT_41(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_43(a, b, ...)  PPUTL_DETAIL_CAT_42(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_44(a, b, ...)  PPUTL_DETAIL_CAT_43(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_45(a, b, ...)  PPUTL_DETAIL_CAT_44(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_46(a, b, ...)  PPUTL_DETAIL_CAT_45(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_47(a, b, ...)  PPUTL_DETAIL_CAT_46(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_48(a, b, ...)  PPUTL_DETAIL_CAT_47(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_49(a, b, ...)  PPUTL_DETAIL_CAT_48(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_50(a, b, ...)  PPUTL_DETAIL_CAT_49(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_51(a, b, ...)  PPUTL_DETAIL_CAT_50(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_52(a, b, ...)  PPUTL_DETAIL_CAT_51(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_53(a, b, ...)  PPUTL_DETAIL_CAT_52(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_54(a, b, ...)  PPUTL_DETAIL_CAT_53(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_55(a, b, ...)  PPUTL_DETAIL_CAT_54(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_56(a, b, ...)  PPUTL_DETAIL_CAT_55(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_57(a, b, ...)  PPUTL_DETAIL_CAT_56(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_58(a, b, ...)  PPUTL_DETAIL_CAT_57(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_59(a, b, ...)  PPUTL_DETAIL_CAT_58(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_60(a, b, ...)  PPUTL_DETAIL_CAT_59(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_61(a, b, ...)  PPUTL_DETAIL_CAT_60(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_62(a, b, ...)  PPUTL_DETAIL_CAT_61(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_63(a, b, ...)  PPUTL_DETAIL_CAT_62(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_64(a, b, ...)  PPUTL_DETAIL_CAT_63(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_65(a, b, ...)  PPUTL_DETAIL_CAT_64(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_66(a, b, ...)  PPUTL_DETAIL_CAT_65(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_67(a, b, ...)  PPUTL_DETAIL_CAT_66(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_68(a, b, ...)  PPUTL_DETAIL_CAT_67(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_69(a, b, ...)  PPUTL_DETAIL_CAT_68(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_70(a, b, ...)  PPUTL_DETAIL_CAT_69(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_71(a, b, ...)  PPUTL_DETAIL_CAT_70(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_72(a, b, ...)  PPUTL_DETAIL_CAT_71(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_73(a, b, ...)  PPUTL_DETAIL_CAT_72(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_74(a, b, ...)  PPUTL_DETAIL_CAT_73(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_75(a, b, ...)  PPUTL_DETAIL_CAT_74(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_76(a, b, ...)  PPUTL_DETAIL_CAT_75(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_77(a, b, ...)  PPUTL_DETAIL_CAT_76(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_78(a, b, ...)  PPUTL_DETAIL_CAT_77(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_79(a, b, ...)  PPUTL_DETAIL_CAT_78(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_80(a, b, ...)  PPUTL_DETAIL_CAT_79(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_81(a, b, ...)  PPUTL_DETAIL_CAT_80(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_82(a, b, ...)  PPUTL_DETAIL_CAT_81(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_83(a, b, ...)  PPUTL_DETAIL_CAT_82(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_84(a, b, ...)  PPUTL_DETAIL_CAT_83(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_85(a, b, ...)  PPUTL_DETAIL_CAT_84(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_86(a, b, ...)  PPUTL_DETAIL_CAT_85(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_87(a, b, ...)  PPUTL_DETAIL_CAT_86(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_88(a, b, ...)  PPUTL_DETAIL_CAT_87(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_89(a, b, ...)  PPUTL_DETAIL_CAT_88(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_90(a, b, ...)  PPUTL_DETAIL_CAT_89(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_91(a, b, ...)  PPUTL_DETAIL_CAT_90(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_92(a, b, ...)  PPUTL_DETAIL_CAT_91(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_93(a, b, ...)  PPUTL_DETAIL_CAT_92(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_94(a, b, ...)  PPUTL_DETAIL_CAT_93(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_95(a, b, ...)  PPUTL_DETAIL_CAT_94(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_96(a, b, ...)  PPUTL_DETAIL_CAT_95(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_97(a, b, ...)  PPUTL_DETAIL_CAT_96(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_98(a, b, ...)  PPUTL_DETAIL_CAT_97(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_99(a, b, ...)  PPUTL_DETAIL_CAT_98(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_100(a, b, ...) PPUTL_DETAIL_CAT_99(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_101(a, b, ...) PPUTL_DETAIL_CAT_100(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_102(a, b, ...) PPUTL_DETAIL_CAT_101(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_103(a, b, ...) PPUTL_DETAIL_CAT_102(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_104(a, b, ...) PPUTL_DETAIL_CAT_103(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_105(a, b, ...) PPUTL_DETAIL_CAT_104(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_106(a, b, ...) PPUTL_DETAIL_CAT_105(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_107(a, b, ...) PPUTL_DETAIL_CAT_106(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_108(a, b, ...) PPUTL_DETAIL_CAT_107(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_109(a, b, ...) PPUTL_DETAIL_CAT_108(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_110(a, b, ...) PPUTL_DETAIL_CAT_109(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_111(a, b, ...) PPUTL_DETAIL_CAT_110(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_112(a, b, ...) PPUTL_DETAIL_CAT_111(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_113(a, b, ...) PPUTL_DETAIL_CAT_112(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_114(a, b, ...) PPUTL_DETAIL_CAT_113(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_115(a, b, ...) PPUTL_DETAIL_CAT_114(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_116(a, b, ...) PPUTL_DETAIL_CAT_115(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_117(a, b, ...) PPUTL_DETAIL_CAT_116(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_118(a, b, ...) PPUTL_DETAIL_CAT_117(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_119(a, b, ...) PPUTL_DETAIL_CAT_118(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_120(a, b, ...) PPUTL_DETAIL_CAT_119(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_121(a, b, ...) PPUTL_DETAIL_CAT_120(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_122(a, b, ...) PPUTL_DETAIL_CAT_121(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_123(a, b, ...) PPUTL_DETAIL_CAT_122(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_124(a, b, ...) PPUTL_DETAIL_CAT_123(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_125(a, b, ...) PPUTL_DETAIL_CAT_124(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_126(a, b, ...) PPUTL_DETAIL_CAT_125(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_127(a, b, ...) PPUTL_DETAIL_CAT_126(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_128(a, b, ...) PPUTL_DETAIL_CAT_127(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_129(a, b, ...) PPUTL_DETAIL_CAT_128(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_130(a, b, ...) PPUTL_DETAIL_CAT_129(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_131(a, b, ...) PPUTL_DETAIL_CAT_130(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_132(a, b, ...) PPUTL_DETAIL_CAT_131(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_133(a, b, ...) PPUTL_DETAIL_CAT_132(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_134(a, b, ...) PPUTL_DETAIL_CAT_133(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_135(a, b, ...) PPUTL_DETAIL_CAT_134(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_136(a, b, ...) PPUTL_DETAIL_CAT_135(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_137(a, b, ...) PPUTL_DETAIL_CAT_136(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_138(a, b, ...) PPUTL_DETAIL_CAT_137(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_139(a, b, ...) PPUTL_DETAIL_CAT_138(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_140(a, b, ...) PPUTL_DETAIL_CAT_139(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_141(a, b, ...) PPUTL_DETAIL_CAT_140(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_142(a, b, ...) PPUTL_DETAIL_CAT_141(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_143(a, b, ...) PPUTL_DETAIL_CAT_142(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_144(a, b, ...) PPUTL_DETAIL_CAT_143(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_145(a, b, ...) PPUTL_DETAIL_CAT_144(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_146(a, b, ...) PPUTL_DETAIL_CAT_145(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_147(a, b, ...) PPUTL_DETAIL_CAT_146(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_148(a, b, ...) PPUTL_DETAIL_CAT_147(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_149(a, b, ...) PPUTL_DETAIL_CAT_148(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_150(a, b, ...) PPUTL_DETAIL_CAT_149(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_151(a, b, ...) PPUTL_DETAIL_CAT_150(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_152(a, b, ...) PPUTL_DETAIL_CAT_151(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_153(a, b, ...) PPUTL_DETAIL_CAT_152(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_154(a, b, ...) PPUTL_DETAIL_CAT_153(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_155(a, b, ...) PPUTL_DETAIL_CAT_154(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_156(a, b, ...) PPUTL_DETAIL_CAT_155(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_157(a, b, ...) PPUTL_DETAIL_CAT_156(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_158(a, b, ...) PPUTL_DETAIL_CAT_157(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_159(a, b, ...) PPUTL_DETAIL_CAT_158(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_160(a, b, ...) PPUTL_DETAIL_CAT_159(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_161(a, b, ...) PPUTL_DETAIL_CAT_160(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_162(a, b, ...) PPUTL_DETAIL_CAT_161(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_163(a, b, ...) PPUTL_DETAIL_CAT_162(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_164(a, b, ...) PPUTL_DETAIL_CAT_163(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_165(a, b, ...) PPUTL_DETAIL_CAT_164(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_166(a, b, ...) PPUTL_DETAIL_CAT_165(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_167(a, b, ...) PPUTL_DETAIL_CAT_166(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_168(a, b, ...) PPUTL_DETAIL_CAT_167(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_169(a, b, ...) PPUTL_DETAIL_CAT_168(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_170(a, b, ...) PPUTL_DETAIL_CAT_169(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_171(a, b, ...) PPUTL_DETAIL_CAT_170(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_172(a, b, ...) PPUTL_DETAIL_CAT_171(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_173(a, b, ...) PPUTL_DETAIL_CAT_172(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_174(a, b, ...) PPUTL_DETAIL_CAT_173(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_175(a, b, ...) PPUTL_DETAIL_CAT_174(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_176(a, b, ...) PPUTL_DETAIL_CAT_175(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_177(a, b, ...) PPUTL_DETAIL_CAT_176(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_178(a, b, ...) PPUTL_DETAIL_CAT_177(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_179(a, b, ...) PPUTL_DETAIL_CAT_178(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_180(a, b, ...) PPUTL_DETAIL_CAT_179(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_181(a, b, ...) PPUTL_DETAIL_CAT_180(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_182(a, b, ...) PPUTL_DETAIL_CAT_181(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_183(a, b, ...) PPUTL_DETAIL_CAT_182(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_184(a, b, ...) PPUTL_DETAIL_CAT_183(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_185(a, b, ...) PPUTL_DETAIL_CAT_184(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_186(a, b, ...) PPUTL_DETAIL_CAT_185(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_187(a, b, ...) PPUTL_DETAIL_CAT_186(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_188(a, b, ...) PPUTL_DETAIL_CAT_187(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_189(a, b, ...) PPUTL_DETAIL_CAT_188(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_190(a, b, ...) PPUTL_DETAIL_CAT_189(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_191(a, b, ...) PPUTL_DETAIL_CAT_190(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_192(a, b, ...) PPUTL_DETAIL_CAT_191(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_193(a, b, ...) PPUTL_DETAIL_CAT_192(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_194(a, b, ...) PPUTL_DETAIL_CAT_193(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_195(a, b, ...) PPUTL_DETAIL_CAT_194(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_196(a, b, ...) PPUTL_DETAIL_CAT_195(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_197(a, b, ...) PPUTL_DETAIL_CAT_196(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_198(a, b, ...) PPUTL_DETAIL_CAT_197(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_199(a, b, ...) PPUTL_DETAIL_CAT_198(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_200(a, b, ...) PPUTL_DETAIL_CAT_199(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_201(a, b, ...) PPUTL_DETAIL_CAT_200(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_202(a, b, ...) PPUTL_DETAIL_CAT_201(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_203(a, b, ...) PPUTL_DETAIL_CAT_202(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_204(a, b, ...) PPUTL_DETAIL_CAT_203(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_205(a, b, ...) PPUTL_DETAIL_CAT_204(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_206(a, b, ...) PPUTL_DETAIL_CAT_205(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_207(a, b, ...) PPUTL_DETAIL_CAT_206(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_208(a, b, ...) PPUTL_DETAIL_CAT_207(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_209(a, b, ...) PPUTL_DETAIL_CAT_208(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_210(a, b, ...) PPUTL_DETAIL_CAT_209(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_211(a, b, ...) PPUTL_DETAIL_CAT_210(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_212(a, b, ...) PPUTL_DETAIL_CAT_211(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_213(a, b, ...) PPUTL_DETAIL_CAT_212(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_214(a, b, ...) PPUTL_DETAIL_CAT_213(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_215(a, b, ...) PPUTL_DETAIL_CAT_214(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_216(a, b, ...) PPUTL_DETAIL_CAT_215(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_217(a, b, ...) PPUTL_DETAIL_CAT_216(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_218(a, b, ...) PPUTL_DETAIL_CAT_217(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_219(a, b, ...) PPUTL_DETAIL_CAT_218(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_220(a, b, ...) PPUTL_DETAIL_CAT_219(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_221(a, b, ...) PPUTL_DETAIL_CAT_220(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_222(a, b, ...) PPUTL_DETAIL_CAT_221(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_223(a, b, ...) PPUTL_DETAIL_CAT_222(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_224(a, b, ...) PPUTL_DETAIL_CAT_223(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_225(a, b, ...) PPUTL_DETAIL_CAT_224(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_226(a, b, ...) PPUTL_DETAIL_CAT_225(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_227(a, b, ...) PPUTL_DETAIL_CAT_226(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_228(a, b, ...) PPUTL_DETAIL_CAT_227(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_229(a, b, ...) PPUTL_DETAIL_CAT_228(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_230(a, b, ...) PPUTL_DETAIL_CAT_229(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_231(a, b, ...) PPUTL_DETAIL_CAT_230(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_232(a, b, ...) PPUTL_DETAIL_CAT_231(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_233(a, b, ...) PPUTL_DETAIL_CAT_232(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_234(a, b, ...) PPUTL_DETAIL_CAT_233(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_235(a, b, ...) PPUTL_DETAIL_CAT_234(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_236(a, b, ...) PPUTL_DETAIL_CAT_235(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_237(a, b, ...) PPUTL_DETAIL_CAT_236(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_238(a, b, ...) PPUTL_DETAIL_CAT_237(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_239(a, b, ...) PPUTL_DETAIL_CAT_238(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_240(a, b, ...) PPUTL_DETAIL_CAT_239(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_241(a, b, ...) PPUTL_DETAIL_CAT_240(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_242(a, b, ...) PPUTL_DETAIL_CAT_241(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_243(a, b, ...) PPUTL_DETAIL_CAT_242(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_244(a, b, ...) PPUTL_DETAIL_CAT_243(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_245(a, b, ...) PPUTL_DETAIL_CAT_244(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_246(a, b, ...) PPUTL_DETAIL_CAT_245(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_247(a, b, ...) PPUTL_DETAIL_CAT_246(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_248(a, b, ...) PPUTL_DETAIL_CAT_247(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_249(a, b, ...) PPUTL_DETAIL_CAT_248(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_250(a, b, ...) PPUTL_DETAIL_CAT_249(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_251(a, b, ...) PPUTL_DETAIL_CAT_250(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_252(a, b, ...) PPUTL_DETAIL_CAT_251(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_253(a, b, ...) PPUTL_DETAIL_CAT_252(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_254(a, b, ...) PPUTL_DETAIL_CAT_253(a##b, __VA_ARGS__)
#define PPUTL_DETAIL_CAT_255(a, b, ...) PPUTL_DETAIL_CAT_254(a##b, __VA_ARGS__)
#// clang-format on
#
#endif
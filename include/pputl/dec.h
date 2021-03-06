#ifndef PPUTL_DEC_H_INCLUDED
#define PPUTL_DEC_H_INCLUDED
#/////                                                                     c++20
#///////////////////////////////////////////////////////////////////////////////
#/// @brief \link PPUTL_DEC PPUTL_DEC\endlink -
#/// decrement a number [1, 256)
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
#//  Copyright (C) 2020, 2021 Justin Collier <m@jpcx.dev>
#//
#//	   This program is free software: you can redistribute it and/or modify
#//	   it under the terms of the GNU General Public License as published by
#//	   the Free Software Foundation, either version 3 of the License, or
#//	   (at your option) any later version.
#//
#//	   This program is distributed in the hope that it will be useful,
#//	   but WITHOUT ANY WARRANTY; without even the internalied warranty of
#//	   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#//	   GNU General Public License for more details.
#//                                                                           //
#//  You should have received a copy of the GNU General Public License       ///
#//  along with this program.  If not, see <https://www.gnu.org/licenses/>. ////
#///////////////////////////////////////////////////////////////////////////////
#
#include "pputl/cat.h"
#include "pputl/eq.h"
#include "pputl/not.h"
#
#/// decrement a number [1, 256)
#/// @ingroup pputl
#/// @anchor  PPUTL_DEC
#/// @param n [1, 256)
#define PPUTL_DEC(n) PPUTL_CAT(PPUTL_DETAIL_DEC_, n)
#
#define PPUTL_DETAIL_DEC_1 0
#define PPUTL_DETAIL_DEC_2 1
#define PPUTL_DETAIL_DEC_3 2
#define PPUTL_DETAIL_DEC_4 3
#define PPUTL_DETAIL_DEC_5 4
#define PPUTL_DETAIL_DEC_6 5
#define PPUTL_DETAIL_DEC_7 6
#define PPUTL_DETAIL_DEC_8 7
#define PPUTL_DETAIL_DEC_9 8
#define PPUTL_DETAIL_DEC_10 9
#define PPUTL_DETAIL_DEC_11 10
#define PPUTL_DETAIL_DEC_12 11
#define PPUTL_DETAIL_DEC_13 12
#define PPUTL_DETAIL_DEC_14 13
#define PPUTL_DETAIL_DEC_15 14
#define PPUTL_DETAIL_DEC_16 15
#define PPUTL_DETAIL_DEC_17 16
#define PPUTL_DETAIL_DEC_18 17
#define PPUTL_DETAIL_DEC_19 18
#define PPUTL_DETAIL_DEC_20 19
#define PPUTL_DETAIL_DEC_21 20
#define PPUTL_DETAIL_DEC_22 21
#define PPUTL_DETAIL_DEC_23 22
#define PPUTL_DETAIL_DEC_24 23
#define PPUTL_DETAIL_DEC_25 24
#define PPUTL_DETAIL_DEC_26 25
#define PPUTL_DETAIL_DEC_27 26
#define PPUTL_DETAIL_DEC_28 27
#define PPUTL_DETAIL_DEC_29 28
#define PPUTL_DETAIL_DEC_30 29
#define PPUTL_DETAIL_DEC_31 30
#define PPUTL_DETAIL_DEC_32 31
#define PPUTL_DETAIL_DEC_33 32
#define PPUTL_DETAIL_DEC_34 33
#define PPUTL_DETAIL_DEC_35 34
#define PPUTL_DETAIL_DEC_36 35
#define PPUTL_DETAIL_DEC_37 36
#define PPUTL_DETAIL_DEC_38 37
#define PPUTL_DETAIL_DEC_39 38
#define PPUTL_DETAIL_DEC_40 39
#define PPUTL_DETAIL_DEC_41 40
#define PPUTL_DETAIL_DEC_42 41
#define PPUTL_DETAIL_DEC_43 42
#define PPUTL_DETAIL_DEC_44 43
#define PPUTL_DETAIL_DEC_45 44
#define PPUTL_DETAIL_DEC_46 45
#define PPUTL_DETAIL_DEC_47 46
#define PPUTL_DETAIL_DEC_48 47
#define PPUTL_DETAIL_DEC_49 48
#define PPUTL_DETAIL_DEC_50 49
#define PPUTL_DETAIL_DEC_51 50
#define PPUTL_DETAIL_DEC_52 51
#define PPUTL_DETAIL_DEC_53 52
#define PPUTL_DETAIL_DEC_54 53
#define PPUTL_DETAIL_DEC_55 54
#define PPUTL_DETAIL_DEC_56 55
#define PPUTL_DETAIL_DEC_57 56
#define PPUTL_DETAIL_DEC_58 57
#define PPUTL_DETAIL_DEC_59 58
#define PPUTL_DETAIL_DEC_60 59
#define PPUTL_DETAIL_DEC_61 60
#define PPUTL_DETAIL_DEC_62 61
#define PPUTL_DETAIL_DEC_63 62
#define PPUTL_DETAIL_DEC_64 63
#define PPUTL_DETAIL_DEC_65 64
#define PPUTL_DETAIL_DEC_66 65
#define PPUTL_DETAIL_DEC_67 66
#define PPUTL_DETAIL_DEC_68 67
#define PPUTL_DETAIL_DEC_69 68
#define PPUTL_DETAIL_DEC_70 69
#define PPUTL_DETAIL_DEC_71 70
#define PPUTL_DETAIL_DEC_72 71
#define PPUTL_DETAIL_DEC_73 72
#define PPUTL_DETAIL_DEC_74 73
#define PPUTL_DETAIL_DEC_75 74
#define PPUTL_DETAIL_DEC_76 75
#define PPUTL_DETAIL_DEC_77 76
#define PPUTL_DETAIL_DEC_78 77
#define PPUTL_DETAIL_DEC_79 78
#define PPUTL_DETAIL_DEC_80 79
#define PPUTL_DETAIL_DEC_81 80
#define PPUTL_DETAIL_DEC_82 81
#define PPUTL_DETAIL_DEC_83 82
#define PPUTL_DETAIL_DEC_84 83
#define PPUTL_DETAIL_DEC_85 84
#define PPUTL_DETAIL_DEC_86 85
#define PPUTL_DETAIL_DEC_87 86
#define PPUTL_DETAIL_DEC_88 87
#define PPUTL_DETAIL_DEC_89 88
#define PPUTL_DETAIL_DEC_90 89
#define PPUTL_DETAIL_DEC_91 90
#define PPUTL_DETAIL_DEC_92 91
#define PPUTL_DETAIL_DEC_93 92
#define PPUTL_DETAIL_DEC_94 93
#define PPUTL_DETAIL_DEC_95 94
#define PPUTL_DETAIL_DEC_96 95
#define PPUTL_DETAIL_DEC_97 96
#define PPUTL_DETAIL_DEC_98 97
#define PPUTL_DETAIL_DEC_99 98
#define PPUTL_DETAIL_DEC_100 99
#define PPUTL_DETAIL_DEC_101 100
#define PPUTL_DETAIL_DEC_102 101
#define PPUTL_DETAIL_DEC_103 102
#define PPUTL_DETAIL_DEC_104 103
#define PPUTL_DETAIL_DEC_105 104
#define PPUTL_DETAIL_DEC_106 105
#define PPUTL_DETAIL_DEC_107 106
#define PPUTL_DETAIL_DEC_108 107
#define PPUTL_DETAIL_DEC_109 108
#define PPUTL_DETAIL_DEC_110 109
#define PPUTL_DETAIL_DEC_111 110
#define PPUTL_DETAIL_DEC_112 111
#define PPUTL_DETAIL_DEC_113 112
#define PPUTL_DETAIL_DEC_114 113
#define PPUTL_DETAIL_DEC_115 114
#define PPUTL_DETAIL_DEC_116 115
#define PPUTL_DETAIL_DEC_117 116
#define PPUTL_DETAIL_DEC_118 117
#define PPUTL_DETAIL_DEC_119 118
#define PPUTL_DETAIL_DEC_120 119
#define PPUTL_DETAIL_DEC_121 120
#define PPUTL_DETAIL_DEC_122 121
#define PPUTL_DETAIL_DEC_123 122
#define PPUTL_DETAIL_DEC_124 123
#define PPUTL_DETAIL_DEC_125 124
#define PPUTL_DETAIL_DEC_126 125
#define PPUTL_DETAIL_DEC_127 126
#define PPUTL_DETAIL_DEC_128 127
#define PPUTL_DETAIL_DEC_129 128
#define PPUTL_DETAIL_DEC_130 129
#define PPUTL_DETAIL_DEC_131 130
#define PPUTL_DETAIL_DEC_132 131
#define PPUTL_DETAIL_DEC_133 132
#define PPUTL_DETAIL_DEC_134 133
#define PPUTL_DETAIL_DEC_135 134
#define PPUTL_DETAIL_DEC_136 135
#define PPUTL_DETAIL_DEC_137 136
#define PPUTL_DETAIL_DEC_138 137
#define PPUTL_DETAIL_DEC_139 138
#define PPUTL_DETAIL_DEC_140 139
#define PPUTL_DETAIL_DEC_141 140
#define PPUTL_DETAIL_DEC_142 141
#define PPUTL_DETAIL_DEC_143 142
#define PPUTL_DETAIL_DEC_144 143
#define PPUTL_DETAIL_DEC_145 144
#define PPUTL_DETAIL_DEC_146 145
#define PPUTL_DETAIL_DEC_147 146
#define PPUTL_DETAIL_DEC_148 147
#define PPUTL_DETAIL_DEC_149 148
#define PPUTL_DETAIL_DEC_150 149
#define PPUTL_DETAIL_DEC_151 150
#define PPUTL_DETAIL_DEC_152 151
#define PPUTL_DETAIL_DEC_153 152
#define PPUTL_DETAIL_DEC_154 153
#define PPUTL_DETAIL_DEC_155 154
#define PPUTL_DETAIL_DEC_156 155
#define PPUTL_DETAIL_DEC_157 156
#define PPUTL_DETAIL_DEC_158 157
#define PPUTL_DETAIL_DEC_159 158
#define PPUTL_DETAIL_DEC_160 159
#define PPUTL_DETAIL_DEC_161 160
#define PPUTL_DETAIL_DEC_162 161
#define PPUTL_DETAIL_DEC_163 162
#define PPUTL_DETAIL_DEC_164 163
#define PPUTL_DETAIL_DEC_165 164
#define PPUTL_DETAIL_DEC_166 165
#define PPUTL_DETAIL_DEC_167 166
#define PPUTL_DETAIL_DEC_168 167
#define PPUTL_DETAIL_DEC_169 168
#define PPUTL_DETAIL_DEC_170 169
#define PPUTL_DETAIL_DEC_171 170
#define PPUTL_DETAIL_DEC_172 171
#define PPUTL_DETAIL_DEC_173 172
#define PPUTL_DETAIL_DEC_174 173
#define PPUTL_DETAIL_DEC_175 174
#define PPUTL_DETAIL_DEC_176 175
#define PPUTL_DETAIL_DEC_177 176
#define PPUTL_DETAIL_DEC_178 177
#define PPUTL_DETAIL_DEC_179 178
#define PPUTL_DETAIL_DEC_180 179
#define PPUTL_DETAIL_DEC_181 180
#define PPUTL_DETAIL_DEC_182 181
#define PPUTL_DETAIL_DEC_183 182
#define PPUTL_DETAIL_DEC_184 183
#define PPUTL_DETAIL_DEC_185 184
#define PPUTL_DETAIL_DEC_186 185
#define PPUTL_DETAIL_DEC_187 186
#define PPUTL_DETAIL_DEC_188 187
#define PPUTL_DETAIL_DEC_189 188
#define PPUTL_DETAIL_DEC_190 189
#define PPUTL_DETAIL_DEC_191 190
#define PPUTL_DETAIL_DEC_192 191
#define PPUTL_DETAIL_DEC_193 192
#define PPUTL_DETAIL_DEC_194 193
#define PPUTL_DETAIL_DEC_195 194
#define PPUTL_DETAIL_DEC_196 195
#define PPUTL_DETAIL_DEC_197 196
#define PPUTL_DETAIL_DEC_198 197
#define PPUTL_DETAIL_DEC_199 198
#define PPUTL_DETAIL_DEC_200 199
#define PPUTL_DETAIL_DEC_201 200
#define PPUTL_DETAIL_DEC_202 201
#define PPUTL_DETAIL_DEC_203 202
#define PPUTL_DETAIL_DEC_204 203
#define PPUTL_DETAIL_DEC_205 204
#define PPUTL_DETAIL_DEC_206 205
#define PPUTL_DETAIL_DEC_207 206
#define PPUTL_DETAIL_DEC_208 207
#define PPUTL_DETAIL_DEC_209 208
#define PPUTL_DETAIL_DEC_210 209
#define PPUTL_DETAIL_DEC_211 210
#define PPUTL_DETAIL_DEC_212 211
#define PPUTL_DETAIL_DEC_213 212
#define PPUTL_DETAIL_DEC_214 213
#define PPUTL_DETAIL_DEC_215 214
#define PPUTL_DETAIL_DEC_216 215
#define PPUTL_DETAIL_DEC_217 216
#define PPUTL_DETAIL_DEC_218 217
#define PPUTL_DETAIL_DEC_219 218
#define PPUTL_DETAIL_DEC_220 219
#define PPUTL_DETAIL_DEC_221 220
#define PPUTL_DETAIL_DEC_222 221
#define PPUTL_DETAIL_DEC_223 222
#define PPUTL_DETAIL_DEC_224 223
#define PPUTL_DETAIL_DEC_225 224
#define PPUTL_DETAIL_DEC_226 225
#define PPUTL_DETAIL_DEC_227 226
#define PPUTL_DETAIL_DEC_228 227
#define PPUTL_DETAIL_DEC_229 228
#define PPUTL_DETAIL_DEC_230 229
#define PPUTL_DETAIL_DEC_231 230
#define PPUTL_DETAIL_DEC_232 231
#define PPUTL_DETAIL_DEC_233 232
#define PPUTL_DETAIL_DEC_234 233
#define PPUTL_DETAIL_DEC_235 234
#define PPUTL_DETAIL_DEC_236 235
#define PPUTL_DETAIL_DEC_237 236
#define PPUTL_DETAIL_DEC_238 237
#define PPUTL_DETAIL_DEC_239 238
#define PPUTL_DETAIL_DEC_240 239
#define PPUTL_DETAIL_DEC_241 240
#define PPUTL_DETAIL_DEC_242 241
#define PPUTL_DETAIL_DEC_243 242
#define PPUTL_DETAIL_DEC_244 243
#define PPUTL_DETAIL_DEC_245 244
#define PPUTL_DETAIL_DEC_246 245
#define PPUTL_DETAIL_DEC_247 246
#define PPUTL_DETAIL_DEC_248 247
#define PPUTL_DETAIL_DEC_249 248
#define PPUTL_DETAIL_DEC_250 249
#define PPUTL_DETAIL_DEC_251 250
#define PPUTL_DETAIL_DEC_252 251
#define PPUTL_DETAIL_DEC_253 252
#define PPUTL_DETAIL_DEC_254 253
#define PPUTL_DETAIL_DEC_255 254
#
#endif

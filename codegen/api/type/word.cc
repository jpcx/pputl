/* /////////////////////////////////////////////////////////////////////////////
//                          __    ___
//                         /\ \__/\_ \
//   _____   _____   __  __\ \ ,_\//\ \
//  /\ '__`\/\ '__`\/\ \/\ \\ \ \/ \ \ \
//  \ \ \_\ \ \ \_\ \ \ \_\ \\ \ \_ \_\ \_
//   \ \ ,__/\ \ ,__/\ \____/ \ \__\/\____\
//    \ \ \   \ \ \   \/___/   \/__/\/____/
//     \/_/    \/_/
//
//  pputl Preprocessor Utilities
//  Copyright (C) 2020 - 2022 Justin Collier <m@jpcx.dev>
//
//	   This program is free software: you can redistribute it and/or modify
//	   it under the terms of the GNU General Public License as published by
//	   the Free Software Foundation, either version 3 of the License, or
//	   (at your option) any later version.
//
//	   This program is distributed in the hope that it will be useful,
//	   but WITHOUT ANY WARRANTY; without even the implied warranty of
//	   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	   GNU General Public License for more details.
//                                                                            //
//  You should have received a copy of the GNU General Public License        ///
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.  ////
///////////////////////////////////////////////////////////////////////////// */

#include "type.h"

namespace api {

using namespace codegen;

decltype(word) word = NIFTY_DEF(word, [&](va args) {
  docs << "[inherits from " + any + "] a union of int|uint|hword.";

  tests << word("0")                               = "0" >> docs;
  tests << word("0u")                              = "0u" >> docs;
  tests << word(int_min_s)                         = int_min_s >> docs;
  tests << word(uint_max_s)                        = uint_max_s >> docs;
  tests << word("0x" + utl::cat(samp::hmax) + "u") = ("0x" + utl::cat(samp::hmax) + "u") >> docs;
  tests << word(pp::tup(samp::h8))                 = pp::tup(samp::h8) >> docs;

  def<"0(e, ...)"> _0 = [](arg e, va) {
    return fail(e);
  };

  def<"1(e, word)">{} = [](arg, arg word) {
    return word;
  };

  return def<"o(e, any)">{[&](arg e, arg any) {
    return pp::call(cat(utl::slice(_0, -1), detail::is_word_o(any)), e, any);
  }}(istr("[" + word + "] invalid word; must be int, uint, or hword : " + args), any(args));
});

} // namespace api

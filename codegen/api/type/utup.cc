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

decltype(utup) utup = NIFTY_DEF(utup, [&](va args) {
  docs << "[inherits from " + some + "] a tuple of exactly " + word_size + " ("
              + std::to_string(conf::word_size) + ") nybls."
       << "constructibe from any word.";

  tests << utup(0) = pp::tup(std::vector<std::string>(conf::word_size, "0")) >> docs;
  tests << utup(uint_max_s) =
      pp::tup(std::vector<std::string>(conf::word_size, "F")) >> docs;
  if constexpr (conf::word_size > 1) {
    tests << utup(int_min_s) =
        pp::tup("8, "
                + utl::cat(std::vector<std::string>(conf::word_size - 1, "0"), ", "))
        >> docs;
    tests << utup(int_max_s) =
        pp::tup("7, "
                + utl::cat(std::vector<std::string>(conf::word_size - 1, "F"), ", "))
        >> docs;
    auto cpy = pp::tup(
        "1, " + utl::cat(std::vector<std::string>(conf::word_size - 1, "0"), ", "));
    tests << utup(cpy) = cpy >> docs;
  }

  def<"\\0(e, ...)"> _0 = [&](arg e, va some) {
    def<"<\\0(e, ...)"> _0 = [&](arg e, va) { return fail(e); };

    def<"<\\1(e, atom)">{} = [&](arg e, arg atom) {
      def<"<\\0(e, atom)"> _0 = [&](arg e, arg atom) {
        def<"<\\0(e, atom)"> _0 = [&](arg e, arg) { return fail(e); };
        def<"<\\1(e, uint)">{}  = [&](arg, arg uint) {
          return impl::uhex(uhex(uint), "UTUP");
        };
        return pp::call(cat(utl::slice(_0, -1), detail::is_uint_o(atom)), e, atom);
      };

      def<"<\\1(e, int)">{} = [&](arg, arg int_) {
        return impl::uhex(uhex(int_), "UTUP");
      };

      return pp::call(cat(utl::slice(_0, -1), detail::is_int_o(atom)), e, atom);
    };

    return pp::call(cat(utl::slice(_0, -1), is_atom(some)), e, some);
  };

  def<"\\1(e, ...)">{} = [](arg, va word) { return word; };

  return def<"o(e, ...)">{[&](arg e, va some) {
    return pp::call(cat(utl::slice(_0, -1), detail::is_utup_o(some)), e, some);
  }}(str("[" + utup + "] invalid integer or word : " + args), some(args));
});

} // namespace api

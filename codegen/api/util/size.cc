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

#include "util.h"

namespace api {

using namespace codegen;

decltype(size) size = NIFTY_DEF(size, [&](va args) {
  docs << "tuple size detection."
       << "fails if larger than " + uint_max + " (" + std::to_string(conf::uint_max) + ")";

  tests << size(pp::tup())                                                         = "0u" >> docs;
  tests << size(pp::tup("a"))                                                      = "1u" >> docs;
  tests << size(pp::tup("a, b"))                                                   = "2u" >> docs;
  tests << size(pp::tup("a, b, c"))                                                = "3u";
  tests << size(pp::tup(utl::cat(std::array<std::string, conf::uint_max>{}, ","))) = uint_max_s;
  tests << size(pp::tup(", "))                                                     = "2u" >> docs;
  tests << size(pp::tup(", , "))                                                   = "3u";
  tests << size(pp::tup("a, "))                                                    = "2u";
  tests << size(pp::tup("a, , "))                                                  = "3u";
  tests << size(pp::tup(", a"))                                                    = "2u";
  tests << size(pp::tup(", a, "))                                                  = "3u";
  tests << size(pp::tup(", , a"))                                                  = "3u";

  def read =
      def{"read(_err, " + utl::cat(utl::alpha_base52_seq(conf::uint_max), ", ") + ", _sz, ...)"};

  std::string prefix = utl::slice(read, -4);
  if (prefix.back() == '_')
    prefix.pop_back();

  def verifier = def{prefix + "(...)"} = [&] {
    return "";
  };

  read = [&](pack args) {
    def res = def{prefix + "(...)"} = [&](va args) {
      return args;
    };

    def<"0(_err, _sz)"> _0 = [&](arg _err, arg) {
      return fail(_err);
    };

    def<"1(_err, _sz)">{} = [&](arg, arg sz) {
      return pp::cat(utl::slice(res, -prefix.size()), sz);
    };

    auto sz = args[args.size() - 2];
    return pp::call(
        cat(utl::slice(_0, -1), is_none(pp::cat(utl::slice(verifier, -prefix.size()), sz))),
        args.front(), sz);
  };

  return def<"o(...)">{[&](va args) {
    return def<"<o(e, _, ...)">{[&](arg e, arg op, va args) {
      auto rseq = utl::base10_seq(conf::uint_max + 1);
      for (auto&& v : rseq)
        v = v + "u";
      std::ranges::reverse(rseq);
      for (auto&& v : rseq)
        v = pp::call(op, v);
      return read(e, args + " " + pp::va_opt(", ") + " " + utl::cat(rseq, ", "));
    }}(args);
  }}(istr("[" + size + "] tuple too large : " + args), prefix, esc + " " + tup(args));
});

} // namespace api

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

#include "traits.h"

namespace api {

using namespace codegen;

decltype(size) size = NIFTY_DEF(size, [&](va args) {
  docs << "computes the uint size of args in O(1) time."
       << "terminates expansion if too many args passed.";

  auto bigseq = utl::cat(std::array<std::string, conf::uint_max + 1>{}, ",");
  auto maxseq = bigseq;
  maxseq.pop_back();

  tests << size()            = "0" >> docs;
  tests << size("a")         = "1" >> docs;
  tests << size("a, b")      = "2" >> docs;
  tests << size("a, b, c")   = "3";
  tests << size(maxseq)      = uint_max_s;
  tests << size(", ")        = "2" >> docs;
  tests << size(", , ")      = "3";
  tests << size("a, ")       = "2";
  tests << size("a, , ")     = "3";
  tests << size(", a")       = "2";
  tests << size(", a, ")     = "3";
  tests << size(", , a")     = "3";
  tests << str(size(bigseq)) = pp::str(size(bigseq));

  def read = def{"read(_va, " + utl::cat(utl::alpha_base52_seq(conf::uint_max), ", ") + ", _sz, ...)"};

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

    def<"0(_va, _sz)"> _0 = [&](arg _va, arg) {
      return size(items(_va));
    };

    def<"1(_va, _sz)">{} = [&](arg, arg sz) {
      return pp::cat(utl::slice(res, -prefix.size()), sz);
    };

    auto sz = args[args.size() - 2];
    return pp::call(cat(utl::slice(_0, -1), is_none(pp::cat(utl::slice(verifier, -prefix.size()), sz))),
                    args.front(), sz);
  };

  return def<"x(_va, _, ...)">{[&](arg _va, arg _, va args) {
    auto rseq = utl::base10_seq(conf::uint_max + 1);
    std::ranges::reverse(rseq);
    for (auto&& v : rseq)
      v = pp::call(_, v);
    return read(_va, args + " " + pp::va_opt(", ") + " " + utl::cat(rseq, ", "));
  }}(pp::tup(args), prefix, args);
});

} // namespace api
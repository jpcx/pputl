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

namespace detail {
decltype(uint_seq)  uint_seq  = NIFTY_DEF(uint_seq);
decltype(uint_rseq) uint_rseq = NIFTY_DEF(uint_rseq);
decltype(uint_next) uint_next = NIFTY_DEF(uint_next);
decltype(uint_prev) uint_prev = NIFTY_DEF(uint_prev);
} // namespace detail

decltype(uint) uint = NIFTY_DEF(uint, [&](va args) {
  docs << "uint type (0 through " + uint_max_s + ")."
       << "expands to n if valid, else, terminates expansion with a self-reference.";

  tests << uint(0)              = "0" >> docs;
  tests << uint(1)              = "1" >> docs;
  tests << uint(2)              = "2" >> docs;
  tests << uint(conf::uint_max) = uint_max_s >> docs;
  tests << str(uint(conf::uint_max + 1)) =
      ("\"" + uint + "(" + std::to_string(conf::uint_max + 1) + ")" + "\"") >> docs;
  tests << str(uint("1, 2"))   = ("\"" + uint + "(1, 2)" + "\"") >> docs;
  tests << str(uint("1,"))     = ("\"" + uint + "(1,)" + "\"") >> docs;
  tests << str(uint("foo"))    = ("\"" + uint + "(foo)" + "\"") >> docs;
  tests << str(uint("()"))     = ("\"" + uint + "(())" + "\"") >> docs;
  tests << str(uint("(), ()")) = ("\"" + uint + "((), ())" + "\"") >> docs;

  auto seq  = utl::base10_seq(conf::uint_max + 1);
  auto rseq = seq;
  std::ranges::reverse(rseq);

  // set up uint seqs
  detail::uint_rseq = def{"rseq"} = [&] {
    return utl::cat(rseq, ", ");
  };
  detail::uint_seq = def{"seq"} = [&] {
    docs << "full unsigned integer sequences";
    return utl::cat(seq, ", ");
  };

  // set up next/prev
  detail::uint_prev[0] = def{"prev_0"} = [&] {
    return uint_max_s + ", .";
  };
  for (size_t i = 1; i < detail::uint_prev.size(); ++i) {
    detail::uint_prev[i] = def{"prev_" + utl::to_string(i)} = [&] {
      return utl::to_string(i - 1) + ", .";
    };
  }

  for (size_t i = 0; i < detail::uint_next.size() - 1; ++i) {
    detail::uint_next[i] = def{"next_" + utl::to_string(i)} = [&] {
      return utl::to_string(i + 1) + ", .";
    };
  }
  detail::uint_next[conf::uint_max] = def{"next_" + uint_max_s} = [&] {
    docs << "uint cycle macros. verifies uint validity if cat result has two values";
    return "0, .";
  };

  def<"fail(...)"> fail = [&](va args) {
    return uint(args);
  };

  def<"pass(...)"> pass = [&](va args) {
    docs << "fourth parentheses; returns";
    return args;
  };

  def<>           ooo_fail{};
  def<"ooo(...)"> ooo = [&](va args) {
    docs << "third parentheses; asserts one of 0 through " + uint_max_s + ".";

    ooo_fail = def{"fail(...)"} = [&](va) {
      return fail;
    };

    def<"no_fail(...)"> ooo_no_fail = [&](va) {
      return pass;
    };

    return def<"res(...)">{[&](va args) {
      return def<"x(_, ...)">{[&](arg, va) {
        std::string const prefix    = utl::slice(ooo_fail, -4);
        std::string const fail_s    = utl::slice(ooo_fail, prefix.size(), 0);
        std::string const no_fail_s = utl::slice(ooo_no_fail, prefix.size(), 0);

        return pp::call(pp::cat(
            prefix, pp::va_opt(utl::slice(no_fail_s, (no_fail_s.size() == 7 ? 3 : 2) - no_fail_s.size())),
            fail_s));
      }}(args);
    }}(pp::cat(utl::slice(detail::uint_next[0], -1), args));
  };

  def<>             oo_fail{};
  def<"oo(_, ...)"> oo = [&](arg _, va) {
    docs << "second parentheses; asserts non-tuple.";

    oo_fail = def{"fail(...)"} = [&](va) {
      return ooo_fail;
    };

    def<"no_fail(...)"> oo_no_fail = [&](va) {
      return ooo;
    };

    return def<"res(...)">{[&](va) {
      std::string const prefix    = utl::slice(oo_fail, -4);
      std::string const fail_s    = utl::slice(oo_fail, prefix.size(), 0);
      std::string const no_fail_s = utl::slice(oo_no_fail, prefix.size(), 0);

      return pp::call(pp::cat(
          prefix, pp::va_opt(utl::slice(no_fail_s, (no_fail_s.size() == 7 ? 3 : 2) - no_fail_s.size())),
          fail_s));
    }}(eat + " " + _);
  };

  def<"o(_, ...)"> o = [&](arg, va) {
    docs << "first parentheses; asserts only one arg.";

    def<"pass(...)"> pass = [&](va) {
      return oo;
    };

    def<"no_pass(...)"> no_pass = [&](va) {
      return oo_fail;
    };

    std::string const prefix    = utl::slice(pass, -4);
    std::string const pass_s    = utl::slice(pass, prefix.size(), 0);
    std::string const no_pass_s = utl::slice(no_pass, prefix.size(), 0);

    return pp::call(
        pp::cat(prefix, pp::va_opt(utl::slice(no_pass_s, (no_pass_s.size() == 7 ? 3 : 2) - no_pass_s.size())),
                pass_s));
  };

  return pp::call(pp::call(pp::call(o(args + "."), args), args), args);
});

} // namespace api

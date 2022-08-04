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
decltype(uint_seq)    uint_seq    = NIFTY_DEF(uint_seq);
decltype(uint_rseq)   uint_rseq   = NIFTY_DEF(uint_rseq);
decltype(uint_traits) uint_traits = NIFTY_DEF(uint_traits);
decltype(uint_pass)   uint_pass   = NIFTY_DEF(uint_pass);
decltype(uint_fail)   uint_fail   = NIFTY_DEF(uint_fail);
decltype(uint_o)      uint_o      = NIFTY_DEF(uint_o);
} // namespace detail

decltype(uint) uint = NIFTY_DEF(uint, [&](va args) {
  docs << "uint type (0 through " + uint_max_s + ")."
       << "expands to n if valid, else fails.";

  tests << uint(0)              = "0" >> docs;
  tests << uint(1)              = "1" >> docs;
  tests << uint(2)              = "2" >> docs;
  tests << uint(conf::uint_max) = uint_max_s >> docs;

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

  // set up traits
  {
    static constexpr auto m = conf::uint_max + 1;
    using std::to_string;
    std::size_t i = 0;
    for (; i < detail::uint_traits.size() - 1; ++i) {
      detail::uint_traits[i] = def{"traits_" + to_string(i)} = [&] {
        return to_string((m + (i - 1)) % m) + ", " + to_string((m + (i + 1)) % m) + ", "
             + to_string((m + (i / 2)) % m) + ", " + to_string((m + (i * 2)) % m) + ", "
             + to_string((m + (i % 2)) % m);
      };
    }
    detail::uint_traits[i] = def{"traits_" + to_string(i)} = [&] {
      docs << "-1,  +1,  /2,  *2,  %2";
      return to_string((m + (i - 1)) % m) + ", " + to_string((m + (i + 1)) % m) + ", "
           + to_string((m + (i / 2)) % m) + ", " + to_string((m + (i * 2)) % m) + ", "
           + to_string((m + (i % 2)) % m);
    };
  }

  detail::uint_fail = def{"fail(err, ...)"} = [&](arg err, va) {
    return fail(err);
  };

  detail::uint_pass = def{"pass(err, ...)"} = [&](arg, va args) {
    docs << "fourth parentheses; returns";
    return args;
  };

  def<>           ooo_fail{};
  def<"ooo(...)"> ooo = [&](va args) {
    docs << "third parentheses; asserts one of 0 through " + uint_max_s + ".";

    ooo_fail = def{"fail(...)"} = [&](va) {
      return detail::uint_fail;
    };

    def<"no_fail(...)"> ooo_no_fail = [&](va) {
      return detail::uint_pass;
    };

    return def<"res(...)">{[&](va args) {
      return def<"x(_, ...)">{[&](arg, va) {
        std::string const prefix    = utl::slice(ooo_fail, -4);
        std::string const fail_s    = utl::slice(ooo_fail, prefix.size(), 0);
        std::string const no_fail_s = utl::slice(ooo_no_fail, prefix.size(), 0);

        return pp::call(pp::cat(
            prefix,
            pp::va_opt(utl::slice(no_fail_s, (no_fail_s.size() == 7 ? 3 : 2) - no_fail_s.size())),
            fail_s));
      }}(args);
    }}(pp::cat(utl::slice(detail::uint_traits[0], -1), args));
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
          prefix,
          pp::va_opt(utl::slice(no_fail_s, (no_fail_s.size() == 7 ? 3 : 2) - no_fail_s.size())),
          fail_s));
    }}(eat + " " + _);
  };

  detail::uint_o = def{"o(_, ...)"} = [&](arg, va) {
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

    return pp::call(pp::cat(
        prefix,
        pp::va_opt(utl::slice(no_pass_s, (no_pass_s.size() == 7 ? 3 : 2) - no_pass_s.size())),
        pass_s));
  };

  return pp::call(pp::call(pp::call(detail::uint_o(args + "."), args), args),
                  istr("[" + uint + "] invalid uint : " + args), args);
});

} // namespace api

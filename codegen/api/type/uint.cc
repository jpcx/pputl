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

static std::string
log2(unsigned n) {
  if (n == 0)
    return "";
  return std::to_string(static_cast<unsigned>(std::log2(n)));
}

static std::string
sqrt(unsigned n) {
  return std::to_string(static_cast<unsigned>(std::sqrt(n)));
}

static std::string
factors(unsigned n) {
  auto                     facts = utl::prime_factors(n);
  std::vector<std::string> sfacts(facts.size());
  std::ranges::transform(facts, std::begin(sfacts), [](auto&& v) {
    return std::to_string(v);
  });
  return pp::tup(utl::cat(sfacts, ", "));
}

static std::array<std::string, conf::uint_bits>
bitnot(std::array<std::string, conf::uint_bits> const& n) {
  auto res = n;
  for (auto&& v : res)
    v = v == "1" ? "0" : "1";
  return res;
}

static std::array<std::string, conf::uint_bits>
binary(unsigned n) {
  std::array<std::string, conf::uint_bits> res{};
  std::ranges::fill(res, "0");
  for (unsigned i = 0; n; ++i) {
    res[res.size() - 1 - i] = std::to_string(n % 2);
    n /= 2;
  }
  return res;
}

static std::string
binary_str(std::array<std::string, conf::uint_bits> const& n) {
  return "0b" + utl::cat(n) + "u";
}

decltype(uint) uint = NIFTY_DEF(uint, [&](va args) {
  docs << "uint type (0 through " + uint_max_s + ")."
       << "expands to n if valid, else fails."
       << ""
       << "constructible from binary or decimal."
       << "binary bit length is fixed at " + uint_bits + " (" + std::to_string(conf::uint_bits)
              + ").";

  auto binmin = "0b" + utl::cat(std::vector<std::string>(conf::uint_bits, "0")) + "u";
  auto binmax = "0b" + utl::cat(std::vector<std::string>(conf::uint_bits, "1")) + "u";

  tests << uint(0)              = "0" >> docs;
  tests << uint(1)              = "1" >> docs;
  tests << uint(2)              = "2" >> docs;
  tests << uint(conf::uint_max) = uint_max_s >> docs;
  tests << uint(binmin)         = binmin >> docs;
  tests << uint(binmax)         = binmax >> docs;

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
    std::size_t _ = 0;
    std::size_t n = 0;
    for (; _ < (detail::uint_traits.size() / 2) - 1; ++_, ++n) {
      auto bin               = binary(n);
      detail::uint_traits[_] = def{"traits_" + std::to_string(n)} = [&] {
        return utl::cat(
            std::array{std::string{"DEC"}, binary_str(bin), log2(n), sqrt(n), factors(n)}, ",");
      };
    }
    {
      auto bin                 = binary(n);
      detail::uint_traits[_++] = def{"traits_" + std::to_string(n)} = [&] {
        docs << "type, binary, log2, sqrt, factors";
        return utl::cat(
            std::array{std::string{"DEC"}, binary_str(bin), log2(n), sqrt(n), factors(n)}, ",");
      };
    }

    n = 0;
    for (; _ < detail::uint_traits.size() - 1; ++_, ++n) {
      auto bin               = binary(n);
      detail::uint_traits[_] = def{"traits_\\" + binary_str(bin)} = [&] {
        return utl::cat(std::array{std::string{"BIN"}, std::to_string(n),
                                   pp::tup(utl::cat(bin, ", ")), binary_str(bitnot(bin))},
                        ",");
      };
    }
    {
      auto bin               = binary(n);
      detail::uint_traits[_] = def{"traits_\\" + binary_str(bin)} = [&] {
        docs << "type, decimal, bits, bitnot";
        return utl::cat(std::array{std::string{"BIN"}, std::to_string(n),
                                   pp::tup(utl::cat(bin, ", ")), binary_str(bitnot(bin))},
                        ",");
      };
    }
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

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
decltype(uint_seq)     uint_seq     = NIFTY_DEF(uint_seq);
decltype(uint_rseq)    uint_rseq    = NIFTY_DEF(uint_rseq);
decltype(uint_decimal) uint_decimal = NIFTY_DEF(uint_decimal);
decltype(uint_traits)  uint_traits  = NIFTY_DEF(uint_traits);
decltype(uint_pass)    uint_pass    = NIFTY_DEF(uint_pass);
decltype(uint_fail)    uint_fail    = NIFTY_DEF(uint_fail);
decltype(uint_o)       uint_o       = NIFTY_DEF(uint_o);
} // namespace detail

static constexpr auto mod = conf::uint_max + 1;

static std::string
dec(unsigned n) {
  return std::to_string((mod + (n - 1)) % mod);
}

static std::string
inc(unsigned n) {
  return std::to_string((n + 1) % mod);
}

static std::string
log2(unsigned n) {
  if (n == 0)
    return "";
  return std::to_string(static_cast<unsigned>(std::log2(n)));
}

static std::string
div2(unsigned n) {
  return std::to_string(n / 2);
}

static std::string
mul2(unsigned n) {
  auto res = (n * 2) % mod;
  return std::to_string(res) + "," + std::to_string(res < n);
}

static std::string
sqrt(unsigned n) {
  return std::to_string(static_cast<unsigned>(std::sqrt(n)));
}

static std::string
pow2(unsigned n) {
  std::size_t n_overflows = 0;
  unsigned    res{0};
  for (std::size_t i = 0; i < n; ++i) {
    auto next = (res + n) % mod;
    n_overflows += next < res;
    res = next;
  }
  return std::to_string(res) + "," + std::to_string(n_overflows);
}

static std::string
mod2(unsigned n) {
  return std::to_string(n % 2);
}

static std::string
mod4(unsigned n) {
  return std::to_string(n % 4);
}

static std::string
mod8(unsigned n) {
  return std::to_string(n % 8);
}

static std::string
mod16(unsigned n) {
  return std::to_string(n % 16);
}

static std::string
mod32(unsigned n) {
  return std::to_string(n % 32);
}

static std::string
mod64(unsigned n) {
  return std::to_string(n % 64);
}

static std::string
factor(unsigned n) {
  auto                     facts = utl::prime_factors(n);
  std::vector<std::string> sfacts(facts.size());
  std::ranges::transform(facts, std::begin(sfacts), [](auto&& v) {
    return std::to_string(v);
  });
  return pp::tup(utl::cat(sfacts, ", "));
}

static std::array<std::string, conf::uint_bits>
binary_arr(unsigned n) {
  std::array<std::string, conf::uint_bits> res{};
  std::ranges::fill(res, "0");
  for (unsigned i = 0; n; ++i) {
    res[res.size() - 1 - i] = std::to_string(n % 2);
    n /= 2;
  }
  return res;
}

static std::string
binary(unsigned n) {
  return pp::tup(utl::cat(binary_arr(n), ","));
}

decltype(uint) uint = NIFTY_DEF(uint, [&](va args) {
  docs << "uint type (0 through " + uint_max_s + ")."
       << "expands to n if valid, else fails."
       << ""
       << "constructible from binary or decimal.";

  tests << uint(0)              = "0" >> docs;
  tests << uint(1)              = "1" >> docs;
  tests << uint(2)              = "2" >> docs;
  tests << uint(conf::uint_max) = uint_max_s >> docs;
  /* tests << uint("(0)")          = "0" >> docs; */
  /* tests << uint("(1,0)")        = "2" >> docs; */
  /* tests << uint("(1,0,1)")      = "5" >> docs; */

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

  // set up binary
  for (std::size_t i = 0; i < detail::uint_decimal.size(); ++i)
    detail::uint_decimal[i] = def{"decimal_" + utl::cat(binary_arr(i))} = [&] {
      return std::to_string(i);
    };

  // set up traits
  {
    clang_format  = false;
    std::size_t i = 0;
    for (; i < detail::uint_traits.size() - 1; ++i) {
      detail::uint_traits[i] = def{"traits_" + std::to_string(i)} = [&] {
        return utl::cat(std::array{dec(i), inc(i), log2(i), div2(i), mul2(i), sqrt(i), pow2(i),
                                   mod2(i), mod4(i), mod8(i), mod16(i), mod32(i), mod64(i),
                                   factor(i), binary(i)},
                        ",");
      };
    }
    detail::uint_traits[i] = def{"traits_" + std::to_string(i)} = [&] {
      docs << "dec, inc, log2, div2, mul2, mflo, sqrt, pow2, pflo, mod2, mod4, mod8, mod16, "
              "mod32, mod64, fact, bin";
      return utl::cat(std::array{dec(i), inc(i), log2(i), div2(i), mul2(i), sqrt(i), pow2(i),
                                 mod2(i), mod4(i), mod8(i), mod16(i), mod32(i), mod64(i), factor(i),
                                 binary(i)},
                      ",");
    };
    clang_format = true;
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

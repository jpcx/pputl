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
decltype(uint_traits) uint_traits = NIFTY_DEF(uint_traits);
decltype(uint_trait)  uint_trait  = NIFTY_DEF(uint_trait);
decltype(uint_upass)  uint_upass  = NIFTY_DEF(uint_upass);
decltype(uint_ipass)  uint_ipass  = NIFTY_DEF(uint_ipass);
decltype(uint_fail)   uint_fail   = NIFTY_DEF(uint_fail);
decltype(uint_o)      uint_o      = NIFTY_DEF(uint_o);
} // namespace detail

namespace impl {
static std::string
log2(unsigned n) {
  if (n == 0)
    return "";
  return std::to_string(static_cast<unsigned>(std::log2(n))) + "u";
}

static std::string
sqrt(unsigned n) {
  return std::to_string(static_cast<unsigned>(std::sqrt(n))) + "u";
}

static std::string
factors(unsigned n) {
  auto                     facts = utl::prime_factors(n);
  std::vector<std::string> sfacts(facts.size());
  std::ranges::transform(facts, std::begin(sfacts), [](auto&& v) {
    return std::to_string(v) + "u";
  });
  return pp::tup(utl::cat(sfacts, ", "));
}

static std::array<std::string, conf::bit_length>
bitnot(std::array<std::string, conf::bit_length> const& n) {
  auto res = n;
  for (auto&& v : res)
    v = v == "1" ? "0" : "1";
  return res;
}

static std::array<std::string, conf::bit_length>
binary(unsigned n) {
  std::array<std::string, conf::bit_length> res{};
  std::ranges::fill(res, "0");
  for (unsigned i = 0; n; ++i) {
    res[res.size() - 1 - i] = std::to_string(n % 2);
    n /= 2;
  }
  return res;
}

static std::string
hex_str(std::array<std::string, conf::bit_length> const& n, bool uint = true) {
  std::string res{"0x"};
  for (std::size_t ofs = 0; ofs < n.size(); ofs += 4) {
    auto grp = n[ofs + 0] + n[ofs + 1] + n[ofs + 2] + n[ofs + 3];
    if (grp == "0000") {
      res += "0";
    } else if (grp == "0001") {
      res += "1";
    } else if (grp == "0010") {
      res += "2";
    } else if (grp == "0011") {
      res += "3";
    } else if (grp == "0100") {
      res += "4";
    } else if (grp == "0101") {
      res += "5";
    } else if (grp == "0110") {
      res += "6";
    } else if (grp == "0111") {
      res += "7";
    } else if (grp == "1000") {
      res += "8";
    } else if (grp == "1001") {
      res += "9";
    } else if (grp == "1010") {
      res += "A";
    } else if (grp == "1011") {
      res += "B";
    } else if (grp == "1100") {
      res += "C";
    } else if (grp == "1101") {
      res += "D";
    } else if (grp == "1110") {
      res += "E";
    } else {
      res += "F";
    }
  }

  return res + (uint ? "u" : "");
}

static int
uint_to_int(unsigned n) {
  auto mod = (conf::uint_max + 1) / 2;
  if (n < mod)
    return n;
  else
    return -((mod * 2) - n);
}
} // namespace impl

decltype(uint) uint = NIFTY_DEF(uint, [&](va args) {
  docs << std::to_string(conf::bit_length) + "-bit unsigned integer type."
       << "may be constructed from either unsigned or signed ints."
       << "cannot parse negative decimals; use math.neg instead."
       << ""
       << "hex length is fixed. cannot parse shorter hex lengths."
       << ""
       << "preserves hex/decimal representation."
       << ""
       << "uses a 'u' suffix for both representations."
       << "see fmt.paste_uint to remove suffix before pasting."
       << ""
       << "cast from signed reinterprets bits as unsigned."
       << "value must be a valid signed int; implicit interpretation"
       << "as unsigned is not allowed (e.g. " + std::to_string(conf::uint_max)
              + " is not a valid integer).";

  auto hexmin    = "0x" + utl::cat(std::vector<std::string>(conf::hex_length, "0")) + "u";
  auto hexmax    = "0x" + utl::cat(std::vector<std::string>(conf::hex_length, "F")) + "u";
  auto hexmaxpop = hexmax;
  hexmaxpop.pop_back();

  tests << uint(0)          = "0u" >> docs;
  tests << uint(1)          = "1u" >> docs;
  tests << uint("2u")       = "2u" >> docs;
  tests << uint(uint_max_s) = uint_max_s >> docs;
  tests << uint(hexmin)     = hexmin >> docs;
  tests << uint(hexmaxpop)  = hexmax >> docs;

  // set up traits
  {
    clang_format = false;

    std::size_t _ = 0;
    std::size_t n = 0;
    for (; _ < (detail::uint_traits.size() / 2) - 1; ++_, ++n) {
      auto bin               = impl::binary(n);
      detail::uint_traits[_] = def{"traits_" + std::to_string(n) + "\\u"} = [&] {
        return utl::cat(std::array{std::string{"DEC"}, impl::hex_str(bin, false),
                                   std::string{n > conf::int_max ? "1" : "0"}, impl::log2(n),
                                   impl::sqrt(n), impl::factors(n)},
                        ",");
      };
    }
    {
      auto bin                 = impl::binary(n);
      detail::uint_traits[_++] = def{"traits_" + std::to_string(n) + "\\u"} = [&] {
        docs << "type, signed hex, signed is negative, log2, sqrt, factors";
        return utl::cat(std::array{std::string{"DEC"}, impl::hex_str(bin, false),
                                   std::string{n > conf::int_max ? "1" : "0"}, impl::log2(n),
                                   impl::sqrt(n), impl::factors(n)},
                        ",");
      };
    }

    n = 0;
    for (; _ < detail::uint_traits.size() - 1; ++_, ++n) {
      auto bin               = impl::binary(n);
      detail::uint_traits[_] = def{"traits_\\" + impl::hex_str(bin)} = [&] {
        return utl::cat(std::array{std::string{"HEX"}, std::to_string(n) + "u",
                                   std::to_string(impl::uint_to_int(n)),
                                   pp::tup(utl::cat(bin, ",")), impl::hex_str(impl::bitnot(bin))},
                        ",");
      };
    }
    {
      auto bin               = impl::binary(n);
      detail::uint_traits[_] = def{"traits_\\" + impl::hex_str(bin)} = [&] {
        docs << "type, unsigned decimal, signed decimal, bits, bitnot";
        return utl::cat(std::array{std::string{"HEX"}, std::to_string(n) + "u",
                                   std::to_string(impl::uint_to_int(n)),
                                   pp::tup(utl::cat(bin, ",")), impl::hex_str(impl::bitnot(bin))},
                        ",");
      };
    }

    clang_format = true;
  }

  detail::uint_trait = def{"trait(...)"} = [&](va args) {
    docs << "internal traits retrieval. uint must be valid and have a suffix.";

    def<"\\TYPE(t, ...) -> HEX|DEC"> type = [&](arg t, va) {
      return t;
    };

    def<"\\HEX_UDEC(t, ud, id, b, bn) -> ubase10">{} = [&](pack args) {
      return args[1];
    };

    def<"\\HEX_IDEC(t, ud, id, b, bn) -> ibase10">{} = [&](pack args) {
      return args[2];
    };

    def<"\\HEX_BITS(t, ud, id, b, bn) -> tuple<bool...>">{} = [&](pack args) {
      return args[3];
    };

    def<"\\HEX_BNOT(t, ud, id, b, bn) -> ubase16">{} = [&](pack args) {
      return args[4];
    };

    def<"\\DEC_IHEX(t, ib, in, l2, sq, f) -> ibase16">{} = [&](pack args) {
      return args[1];
    };

    def<"\\DEC_INEG(t, ib, in, l2, sq, f) -> bool{signed<0}">{} = [&](pack args) {
      return args[2];
    };

    def<"\\DEC_LOG2(t, ib, in, l2, sq, f) -> ubase10{log2(n)}">{} = [&](pack args) {
      return args[3];
    };

    def<"\\DEC_SQRT(t, ib, in, l2, sq, f) -> ubase10{sqrt(n)}">{} = [&](pack args) {
      return args[4];
    };

    def<"\\DEC_FACT(t, ib, in, l2, sq, f) -> typle<ubase10{factor}...>">{} = [&](pack args) {
      docs << "uint traits. trait name follows the " + utl::slice(type, -4) + " prefix";
      return args[5];
    };

    return def<"o(uint, trait)">{[&](arg uint, arg trait) {
      return def<"o(trait, ...)">{[&](arg trait, va args) {
        return pp::call(pp::cat(utl::slice(type, -4), trait), args);
      }}(trait, cat(utl::slice(detail::uint_traits[0], -2), uint));
    }}(args);
  };

  detail::uint_fail = def{"fail(e, ...)"} = [&](arg e, va) {
    return fail(e);
  };

  detail::uint_ipass = def{"ipass(e, v)"} = [&](arg, arg v) {
    return pp::cat(v, "u");
  };

  detail::uint_upass = def{"upass(e, v)"} = [&](arg, arg v) {
    docs << "final parentheses; returns";
    return v;
  };

  def<>            oooo_upass{};
  def<"oooo(...)"> oooo = [&](va args) {
    docs << "fourth parentheses; attempts cast from signed.";

    def<"ichk_\\DEC(u)"> ichk_dec = [&](arg u) {
      def<"0"> _0 = [&] {
        return detail::uint_ipass;
      };

      def<"1">{} = [&] {
        return detail::uint_fail;
      };

      return cat(utl::slice(_0, -1), detail::uint_trait(u, "DEC_INEG"));
    };

    def<"ichk_\\HEX(u)">{} = [&](arg) {
      return detail::uint_ipass;
    };

    def<"fail(u)"> oooo_fail = [&](arg) {
      return detail::uint_fail;
    };

    def<"no_fail(u)"> oooo_no_fail = [&](arg u) {
      return pp::call(cat(utl::slice(ichk_dec, -3), detail::uint_trait(u, "TYPE")), u);
    };

    oooo_upass = def{"upass(...)"} = [&](va) {
      return detail::uint_upass;
    };

    return def<"res(u, ...)">{[&](arg u, va args) {
      return def<"o(u, _, ...)">{[&](arg u, arg, va) {
        std::string const prefix    = utl::slice(oooo_fail, -4);
        std::string const fail_s    = utl::slice(oooo_fail, prefix.size(), 0);
        std::string const no_fail_s = utl::slice(oooo_no_fail, prefix.size(), 0);

        return pp::call(pp::cat(prefix,
                                pp::va_opt(utl::slice(no_fail_s, (no_fail_s.size() == 7 ? 3 : 2)
                                                                     - no_fail_s.size())),
                                fail_s),
                        u);
      }}(u, args);
    }}(pp::cat(args, "u"), pp::cat(utl::slice(detail::uint_traits[0], -2), args, "u"));
  };

  def<>           ooo_fail{};
  def<"ooo(...)"> ooo = [&](va args) {
    docs << "third parentheses; attempts cast from unsigned.";

    ooo_fail = def{"fail(...)"} = [&](va) {
      return oooo;
    };

    def<"no_fail(...)"> ooo_no_fail = [&](va) {
      return oooo_upass;
    };

    return def<"res(...)">{[&](va args) {
      return def<"o(_, ...)">{[&](arg, va) {
        std::string const prefix    = utl::slice(ooo_fail, -4);
        std::string const fail_s    = utl::slice(ooo_fail, prefix.size(), 0);
        std::string const no_fail_s = utl::slice(ooo_no_fail, prefix.size(), 0);

        return pp::call(pp::cat(
            prefix,
            pp::va_opt(utl::slice(no_fail_s, (no_fail_s.size() == 7 ? 3 : 2) - no_fail_s.size())),
            fail_s));
      }}(args);
    }}(pp::cat(utl::slice(detail::uint_traits[0], -2), args));
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

  return pp::call(pp::call(pp::call(pp::call(detail::uint_o(args + "."), args), args), args),
                  istr("[" + uint + "] invalid integer : " + args), args);
});

} // namespace api

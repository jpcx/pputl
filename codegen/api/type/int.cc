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

decltype(int_) int_ = NIFTY_DEF(int_, [&](va args) {
  docs << std::to_string(conf::bit_length) + "-bit signed integer type."
       << "may be constructed from either unsigned or signed ints."
       << "cannot parse negative decimals; use math.neg instead."
       << ""
       << "bit length is fixed. cannot parse shorter bit lengths."
       << ""
       << "attempts to preserve binary/decimal representation, but will"
       << "output binary if casting the input yields a negative number"
       << ""
       << "cast from unsigned reinterprets bits as signed two's complement."
       << ""
       << "decimals larger than the int maximum that lack a 'u' suffix"
       << "are interpreted as uints and converted to binary.";

  auto binmin    = "0b" + utl::cat(std::vector<std::string>(conf::bit_length, "0"));
  auto binmax    = "0b0" + utl::cat(std::vector<std::string>(conf::bit_length - 1, "1"));
  auto binneg1   = "0b" + utl::cat(std::vector<std::string>(conf::bit_length, "1"));
  auto iuint_max = uint_max_s;
  iuint_max.pop_back();

  tests << int_(0)             = "0" >> docs;
  tests << int_("1u")          = "1" >> docs;
  tests << int_(binmin)        = binmin >> docs;
  tests << int_(conf::int_max) = std::to_string(conf::int_max) >> docs;
  tests << int_(binmax + "u")  = binmax >> docs;
  tests << int_(uint_max_s)    = binneg1 >> docs;
  tests << int_(iuint_max)     = binneg1 >> docs;

  def<"fail(e, ...)"> fail_ = [&](arg e, va) {
    return fail(e);
  };

  def<"msb(a, ...)"> msb = [&](arg a, va) {
    return a;
  };

  def<"pass_cast(e, v)"> pass_cast = [&](arg, arg v) {
    def<"\\DEC(v)"> dec = [&](arg v) {
      def<"0(v, ibin, ubin)"> _0 = [&](arg, arg, arg ubin) {
        return detail::uint_trait(ubin, "BIN_IDEC");
      };

      def<"1(v, ibin, ubin)">{} = [&](arg, arg ibin, arg) {
        return ibin;
      };

      return def<"o(v, ibin)">{[&](arg v, arg ibin) {
        return def<"o(v, ibin)">{[&](arg v, arg ibin) {
          return def<"o(v, ibin, ubin)">{[&](arg v, arg ibin, arg ubin) {
            return pp::call(
                cat(utl::slice(_0, -1), esc(msb + " " + detail::uint_trait(ubin, "BIN_BITS"))), v,
                ibin, ubin);
          }}(v, ibin, pp::cat(ibin, "u"));
        }}(v, ibin);
      }}(v, detail::uint_trait(v, "DEC_IBIN"));
    };

    def<"\\BIN(v)">{} = [&](arg v) {
      return detail::uint_trait(detail::uint_trait(v, "BIN_UDEC"), "DEC_IBIN");
    };

    return pp::call(cat(utl::slice(dec, -3), detail::uint_trait(v, "TYPE")), v);
  };

  def<"pass(e, v)"> pass = [&](arg, arg v) {
    docs << "fifth parentheses; returns";

    def<"\\DEC(v, u)"> dec = [&](arg v, arg u) {
      def<"0(v, ibin)"> _0 = [&](arg v, arg) {
        return v;
      };

      def<"1(v, ibin)">{} = [&](arg, arg ibin) {
        return ibin;
      };

      return def<"o(v, ibin)">{[&](arg v, arg ibin) {
        return def<"o(v, ibin)">{[&](arg v, arg ibin) {
          return def<"o(v, ibin, ubin)">{[&](arg v, arg ibin, arg ubin) {
            return pp::call(
                cat(utl::slice(_0, -1), esc(msb + " " + detail::uint_trait(ubin, "BIN_BITS"))), v,
                ibin);
          }}(v, ibin, pp::cat(ibin, "u"));
        }}(v, ibin);
      }}(v, detail::uint_trait(u, "DEC_IBIN"));
    };

    def<"\\BIN(v, u)">{} = [&](arg v, arg) {
      return v;
    };

    return def<"o(...)">{[&](va args) {
      return def<"o(v, u)">{[&](arg v, arg u) {
        return pp::call(cat(utl::slice(dec, -3), detail::uint_trait(u, "TYPE")), v, u);
      }}(args);
    }}(v, pp::cat(v, "u"));
  };

  def<>            oooo_passthrough{};
  def<"oooo(...)"> oooo = [&](va args) {
    docs << "fourth parentheses; checks for validity without added 'u' suffix (cast from uint).";

    def<"fail"> oooo_fail = [&] {
      return fail_;
    };

    def<"no_fail"> oooo_no_fail = [&] {
      return pass_cast;
    };

    oooo_passthrough = def{"passthrough(...)"} = [&](va) {
      return pass;
    };

    return def<"res(...)">{[&](va args) {
      return def<"o(_, ...)">{[&](arg, va) {
        std::string const prefix    = utl::slice(oooo_fail, -4);
        std::string const fail_s    = utl::slice(oooo_fail, prefix.size(), 0);
        std::string const no_fail_s = utl::slice(oooo_no_fail, prefix.size(), 0);

        return pp::cat(
            prefix,
            pp::va_opt(utl::slice(no_fail_s, (no_fail_s.size() == 7 ? 3 : 2) - no_fail_s.size())),
            fail_s);
      }}(args);
    }}(pp::cat(utl::slice(detail::uint_traits[0], -2), args));
  };

  def<>           ooo_fail{};
  def<"ooo(...)"> ooo = [&](va args) {
    docs << "third parentheses; checks for validity with added 'u' suffix.";

    ooo_fail = def{"fail(...)"} = [&](va) {
      return oooo;
    };

    def<"no_fail(...)"> ooo_no_fail = [&](va) {
      return oooo_passthrough;
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
    }}(pp::cat(utl::slice(detail::uint_traits[0], -2), args, "u"));
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
                  istr("[" + int_ + "] invalid int : " + args), args);
});

} // namespace api

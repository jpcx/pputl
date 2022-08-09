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
       << "hex length is fixed. cannot parse shorter hex lengths."
       << ""
       << "attempts to preserve hex/decimal representation, but will"
       << "output hex if casting the input yields a negative number"
       << ""
       << "cast from unsigned reinterprets bits as signed two's complement."
       << ""
       << "value must be a valid signed int; implicit interpretation"
       << "as unsigned is not allowed (e.g. " + std::to_string(conf::uint_max)
              + " is not a valid integer).";

  auto hexmin = "0x" + utl::cat(std::vector<std::string>(conf::hex_length, "0"));
  auto hexmax = "0x" + utl::cat(std::vector<std::string>(conf::hex_length, "F"));

  tests << int_(0)             = "0" >> docs;
  tests << int_("1u")          = "1" >> docs;
  tests << int_(hexmin)        = hexmin >> docs;
  tests << int_(conf::int_max) = std::to_string(conf::int_max) >> docs;
  tests << int_(hexmax + "u")  = hexmax >> docs;
  tests << int_(uint_max_s)    = hexmax >> docs;

  def<"fail(e, ...)"> fail_ = [&](arg e, va) {
    docs << "final parentheses (fail)";
    return fail(e);
  };

  def<"upass(e, u)"> upass = [&](arg, arg u) {
    docs << "final parentheses (cast from unsigned)";

    def<"\\DEC(u)"> dec = [&](arg u) {
      def<"0(u, ibin, ubin)"> _0 = [&](arg, arg, arg ubin) {
        return detail::uint_trait(ubin, "HEX_IDEC");
      };

      def<"1(u, ibin, ubin)">{} = [&](arg, arg ibin, arg) {
        return ibin;
      };

      return def<"o(u, ibin)">{[&](arg u, arg ibin) {
        return def<"o(u_, ibin)">{[&](arg u, arg ibin) {
          return def<"o(u, ibin, ubin)">{[&](arg u, arg ibin, arg ubin) {
            return pp::call(
                cat(utl::slice(_0, -1), esc(ifirst + " " + detail::uint_trait(ubin, "HEX_BITS"))),
                u, ibin, ubin);
          }}(u, ibin, pp::cat(ibin, "u"));
        }}(u, ibin);
      }}(u, detail::uint_trait(u, "DEC_IHEX"));
    };

    def<"\\HEX(u)">{} = [&](arg u) {
      return detail::uint_trait(detail::uint_trait(u, "HEX_UDEC"), "DEC_IHEX");
    };

    return pp::call(cat(utl::slice(dec, -3), detail::uint_trait(u, "TYPE")), u);
  };

  def<"ipass(e, i)"> ipass = [&](arg, arg i) {
    docs << "final parentheses (cast from signed)";

    return i;
  };

  def<>            oooo_fail{};
  def<>            oooo_ipass{};
  def<"oooo(...)"> oooo = [&](va args) {
    docs << "fourth parentheses; attempts cast from unsigned.";

    oooo_fail = def{"fail(...)"} = [&](va) {
      return fail_;
    };

    def<"no_fail(...)"> oooo_no_fail = [&](va) {
      return upass;
    };

    oooo_ipass = def{"ipass(...)"} = [&](va) {
      return ipass;
    };

    return def<"res(...)">{[&](va args) {
      return def<"o(_, ...)">{[&](arg, va) {
        std::string const prefix    = utl::slice(oooo_fail, -4);
        std::string const fail_s    = utl::slice(oooo_fail, prefix.size(), 0);
        std::string const no_fail_s = utl::slice(oooo_no_fail, prefix.size(), 0);

        return pp::call(pp::cat(
            prefix,
            pp::va_opt(utl::slice(no_fail_s, (no_fail_s.size() == 7 ? 3 : 2) - no_fail_s.size())),
            fail_s));
      }}(args);
    }}(pp::cat(utl::slice(detail::uint_traits[0], -2), args));
  };

  def<>           ooo_fail{};
  def<"ooo(...)"> ooo = [&](va args) {
    docs << "third parentheses; attempts cast from signed.";

    def<"ichk_\\DEC(u)"> ichk_dec = [&](arg u) {
      def<"0"> _0 = [&] {
        return oooo_ipass;
      };

      def<"1">{} = [&] {
        return oooo_fail;
      };

      return cat(utl::slice(_0, -1), detail::uint_trait(u, "DEC_INEG"));
    };

    def<"ichk_\\HEX(u)">{} = [&](arg) {
      return oooo_ipass;
    };

    ooo_fail = def{"fail(u)"} = [&](arg) {
      return oooo;
    };

    def<"no_fail(u)"> ooo_no_fail = [&](arg u) {
      return pp::call(cat(utl::slice(ichk_dec, -3), detail::uint_trait(u, "TYPE")), u);
    };

    return def<"res(u, ...)">{[&](arg u, va args) {
      return def<"o(u, _, ...)">{[&](arg u, arg, va) {
        std::string const prefix    = utl::slice(ooo_fail, -4);
        std::string const fail_s    = utl::slice(ooo_fail, prefix.size(), 0);
        std::string const no_fail_s = utl::slice(ooo_no_fail, prefix.size(), 0);

        return pp::call(pp::cat(prefix,
                                pp::va_opt(utl::slice(no_fail_s, (no_fail_s.size() == 7 ? 3 : 2)
                                                                     - no_fail_s.size())),
                                fail_s),
                        u);
      }}(u, args);
    }}(pp::cat(args, "u"), pp::cat(utl::slice(detail::uint_traits[0], -2), args, "u"));
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

    return pp::call(pp::cat(
        prefix,
        pp::va_opt(utl::slice(no_pass_s, (no_pass_s.size() == 7 ? 3 : 2) - no_pass_s.size())),
        pass_s));
  };

  return pp::call(pp::call(pp::call(pp::call(o(args + "."), args), args), args),
                  istr("[" + int_ + "] invalid int : " + args), args);
});

} // namespace api

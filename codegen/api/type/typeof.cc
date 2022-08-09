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

decltype(typeof) typeof = NIFTY_DEF(typeof, [&](va args) {
  docs << "detects the value type. must be compatible with the ## operator."
       << "literal 0 and 1 are considered ibase10 rather than bool."
       << "defaults to any if a type could not be determined."
       << ""
       << "returns one of:"
       << "- " + tuple << "- " + ubase2 << "- " + ibase2 << "- " + ubase10 << "- " + ibase10
       << "- " + any;

  auto ibinneg1 = "0b" + utl::cat(std::vector<std::string>(conf::bit_length, "1"));
  auto ubinmax  = ibinneg1 + "u";

  tests << typeof("(foo)")        = tuple >> docs;
  tests << typeof(0)              = ibase10 >> docs;
  tests << typeof("0u")           = ubase10 >> docs;
  tests << typeof(conf::uint_max) = any >> docs;
  tests << typeof(uint_max_s)     = ubase10 >> docs;
  tests << typeof(ibinneg1)       = ibase2 >> docs;
  tests << typeof(ubinmax)        = ubase2 >> docs;
  tests << typeof("foo")          = any >> docs;

  def<"ooo_any(...)"> ooo_any = [&](va) {
    return any;
  };

  def<"ooo_tuple(...)"> ooo_tuple = [&](va) {
    return tuple;
  };

  def<"ooo_uint(u)"> ooo_uint = [&](arg u) {
    def<"\\BIN"> bin = [&] {
      return ubase2;
    };

    def<"\\DEC">{} = [&] {
      return ubase10;
    };

    return cat(utl::slice(bin, -3), detail::uint_trait(uint(u), "TYPE"));
  };

  def<"ooo_int(i)"> ooo_int = [&](arg i) {
    def<"\\BIN"> bin = [&] {
      return ibase2;
    };

    def<"\\DEC">{} = [&] {
      return ibase10;
    };

    return cat(utl::slice(bin, -3), detail::uint_trait(cat(int_(i), "u"), "TYPE"));
  };

  def<"oo_tuple(...)"> oo_tuple = [&](va) {
    return ooo_tuple;
  };

  def<"oo_no_tuple(...)"> oo_no_tuple = [&](va args) {
    def fail_ = def{(std::string const&)detail::uint_fail} = [&] {
      return ooo_any;
    };

    def{(std::string const&)detail::uint_upass} = [&] {
      return ooo_uint;
    };

    def{(std::string const&)detail::uint_ipass} = [&] {
      return ooo_int;
    };

    return cat(utl::slice(fail_, -((std::string const&)detail::uint_fail).size()),
               pp::call(pp::call(pp::call(detail::uint_o(args + "."), args), args), args));
  };

  return pp::call(pp::call(def<"o(...)">{[&](va) {
                             std::string prefix = utl::slice(oo_tuple, -5);
                             if (prefix.back() == '_')
                               prefix.pop_back();
                             std::string tuple_s    = utl::slice(oo_tuple, prefix.size(), 0);
                             std::string no_tuple_s = utl::slice(oo_no_tuple, prefix.size(), 0);
                             std::string no_s       = utl::slice(no_tuple_s, -tuple_s.size());

                             return pp::cat(prefix, pp::va_opt(no_s), tuple_s);
                           }}(eat + " " + args),
                           args),
                  args);
});

} // namespace api

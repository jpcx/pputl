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
       << "literal 0 through 9 are considered ibase10 rather than bool or nybl."
       << ""
       << "returns one of:"
       << "- " + none << "- " + some << "- " + tup << "- " + atom << "- " + udec << "- " + uhex
       << "- " + idec << "- " + ihex << "- " + nybl;

  auto ihexneg1 = "0x" + utl::cat(std::vector<std::string>(conf::hex_length, "F"));
  auto ubinmax  = ihexneg1 + "u";

  tests << typeof("(foo)")        = tup >> docs;
  tests << typeof(0)              = idec >> docs;
  tests << typeof("0u")           = udec >> docs;
  tests << typeof(conf::uint_max) = atom >> docs;
  tests << typeof(uint_max_s)     = udec >> docs;
  tests << typeof(ihexneg1)       = ihex >> docs;
  tests << typeof(ubinmax)        = uhex >> docs;
  tests << typeof("foo")          = atom >> docs;
  tests << typeof("foo, bar")     = some >> docs;
  tests << typeof()               = none >> docs;

  def<"nybl0(atom)"> nybl0 = [&](arg) {
    return atom;
  };

  def<"nybl1(nybl)">{} = [&](arg) {
    return nybl;
  };

  def<"ihex0(atom)"> ihex0 = [&](arg atom) {
    return pp::call(cat(utl::slice(nybl0, -1), is_nybl(atom)), atom);
  };

  def<"ihex1(ihex)">{} = [&](arg) {
    return ihex;
  };

  def<"idec0(atom)"> idec0 = [&](arg atom) {
    return pp::call(cat(utl::slice(ihex0, -1), is_ihex(atom)), atom);
  };

  def<"idec1(idec)">{} = [&](arg) {
    return idec;
  };

  def<"uhex0(atom)"> uhex0 = [&](arg atom) {
    return pp::call(cat(utl::slice(idec0, -1), is_idec(atom)), atom);
  };

  def<"uhex1(uhex)">{} = [&](arg) {
    return uhex;
  };

  def<"udec0(atom)"> udec0 = [&](arg atom) {
    return pp::call(cat(utl::slice(uhex0, -1), is_uhex(atom)), atom);
  };

  def<"udec1(udec)">{} = [&](arg) {
    return udec;
  };

  def<"atom1(atom)">{} = [&](arg atom) {
    return pp::call(cat(utl::slice(udec0, -1), is_udec(atom)), atom);
  };

  def<"atom0(...)"> atom0 = [&](va) {
    return some;
  };

  def<"tup0(...)"> tup0 = [&](va args) {
    return pp::call(cat(utl::slice(atom0, -1), is_atom(args)), args);
  };

  def<"tup1(tup)">{} = [&](arg) {
    return tup;
  };

  def<"none0(...)"> none0 = [&](va args) {
    return pp::call(cat(utl::slice(tup0, -1), is_tup(args)), args);
  };

  def<"none1(...)">{} = [&](va) {
    return none;
  };

  return pp::call(cat(utl::slice(none0, -1), is_none(args)), args);
});

} // namespace api

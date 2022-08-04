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

#include "control.h"

namespace api {

using namespace codegen;

decltype(if_) if_ = NIFTY_DEF(if_, [&](va args) {
  docs << "conditionally expands items based on a boolean."
       << "terminates expansion on invalid args size or types.";

  tests << if_(1, "(t), ()")         = "t" >> docs;
  tests << if_(0, "(t), ()")         = "" >> docs;
  tests << if_(1, "(t), (f)")        = "t" >> docs;
  tests << if_(0, "(t), (f)")        = "f" >> docs;
  tests << if_(1, "(a), (b, c)")     = "a" >> docs;
  tests << if_(0, "(a), (b, c)")     = "b, c" >> docs;
  tests << str(if_())                = pp::str(if_()) >> docs;
  tests << str(if_(0))               = pp::str(if_(0)) >> docs;
  tests << str(if_(0, "()"))         = pp::str(if_(0, "()")) >> docs;
  tests << str(if_('a', "()", "()")) = pp::str(if_('a', "()", "()")) >> docs;

  def<"res_0(...)"> res_0 = [&](va args) {
    return def<"x(t, f)">{[&](arg, arg f) {
      return esc + " " + f;
    }}(args);
  };

  def<"res_1(...)">{} = [&](va args) {
    docs << "t/f items return";
    return def<"x(t, f)">{[&](arg t, arg) {
      return esc + " " + t;
    }}(args);
  };

  def<"000(...)"> _000 = [&](va args) {
    return if_(args);
  };

  def<"001(...)">{} = [&](va args) {
    return if_(args);
  };

  def<"010(...)">{} = [&](va args) {
    return if_(args);
  };

  def<"011(...)">{} = [&](va args) {
    return if_(args);
  };

  def<"100(...)">{} = [&](va args) {
    return if_(args);
  };

  def<"101(...)">{} = [&](va args) {
    return if_(args);
  };

  def<"110(...)">{} = [&](va args) {
    return if_(args);
  };

  def<"111(...)">{} = [&](va args) {
    docs << "arg validation branches";
    return pp::call(cat(utl::slice(res_0, -1), first(args)), rest(args));
  };

  return pp::call(cat(cat(utl::slice(_000, -3), is_bool(first(args))),
                      cat(is_tuple(first(rest(args))), is_tuple(rest(rest(args))))),
                  args);
});

} // namespace api

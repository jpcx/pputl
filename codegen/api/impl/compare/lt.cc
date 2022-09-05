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

#include "impl/compare.h"

namespace api {
namespace impl {

using namespace codegen;

decltype(lt_ucmp) lt_ucmp = NIFTY_DEF(lt_ucmp);

decltype(lt) lt = NIFTY_DEF(lt, [&](arg a, arg b) {
  docs << "[internal] utup less-than comparison.";

  using std::to_string;
  using conf::uint_max;
  using conf::int_max;

  def<"\\000"> _000 = [&] { return "0"; };
  def<"\\001">{}    = [&] { return "1"; };
  def<"\\010">{}    = [&] { return "0"; };
  def<"\\011">{}    = [&] { return "0"; };
  def<"\\100">{}    = [&] { return "1"; };
  def<"\\101">{}    = [&] { return "1"; };
  def<"\\110">{}    = [&] { return "1"; };
  def<"\\111">{}    = [&] { return "1"; };

  def<"r(...)"> recur = [&](va args) {
    return def<"o(fl, fg, a, b, ...)">{[&](arg fl, arg fg, arg a, arg b, va args) {
      return xcat(pp::cat(utl::slice(_000, -3), fl, fg),
                  impl::hexhex(pp::cat(a, b), "LT"))
           + ", "
           + xcat(pp::cat(utl::slice(_000, -3), fg, fl),
                  impl::hexhex(pp::cat(b, a), "LT"))
           + ", " + args;
    }}(args);
  };

  def<"zip(...)"> zip = [&](va args) {
    return def{"o(" + utl::cat(utl::alpha_base52_seq(conf::word_size * 2), ", ") + ")",
               [&](pack args) {
                 svect res;
                 for (std::size_t i = 0; i < args.size() / 2; ++i) {
                   res.push_back(args[i]);
                   res.push_back(args[i + (args.size() / 2)]);
                 }
                 return utl::cat(res, ", ");
               }}(args);
  };

  lt_ucmp = def{"ucmp(...)"} = [&](va args) {
    return xfirst(utl::cat(svect(conf::word_size, recur + "(")) + "0, 0, " + zip(args)
                  + utl::cat(svect(conf::word_size, ")")));
  };

  return lt_ucmp(esc + " " + a, esc + " " + b);
});

} // namespace impl
} // namespace api

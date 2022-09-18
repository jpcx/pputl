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

#include "impl/traits.h"

namespace api {
namespace impl {

using namespace codegen;

decltype(sized_items) sized_items = NIFTY_DEF(sized_items, [&](arg v) {
  docs << "[internal] extracts the size and items from a pputl datastructure.";

  def array_ = def{"\\" + fwd::array + "(sz, items)"} = [&](arg sz, arg items) {
    return sz + ", " + items;
  };
  def{"\\" + fwd::order + "(sz, dir, pre, items)"} = [&](arg sz, arg, arg, arg items) {
    return sz + ", " + items;
  };
  def{"\\" + fwd::map + "(sz, pre, items)"} = [&](arg sz, arg, arg items) {
    return sz + ", " + items;
  };
  def{"\\" + fwd::set + "(sz, pre, items)"} = [&](arg sz, arg, arg items) {
    return sz + ", " + items;
  };
  def{"\\" + fwd::stack + "(sz, items)"} = [&](arg sz, arg items) {
    return sz + ", " + items;
  };
  def{"\\" + fwd::queue + "(sz, items)"} = [&](arg sz, arg items) {
    return sz + ", " + items;
  };
  def{"\\" + fwd::pqueue + "(sz, pre, items)"} = [&](arg sz, arg, arg items) {
    return sz + ", " + items;
  };

  return pp::cat(utl::slice(array_, -((std::string const&)fwd::array).size()), v);
});

} // namespace impl
} // namespace api

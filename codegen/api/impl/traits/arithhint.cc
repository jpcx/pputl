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

decltype(arithhint) arithhint = NIFTY_DEF(arithhint, [&](arg a, arg b) {
  docs << "[internal] two-operand arithmetic cast hint."
       << ""
       << "immediately concatenates args."
       << ""
       << "returns unsigned if either operand is unsigned,"
       << "decimal if either operand is decimal, utup if"
       << "both operands are utup, and hex otherwise.";

  def<"\\IDECIDEC"> idecidec = [&] { return "IDEC"; };
  def<"\\IDECIHEX">{}        = [&] { return "IDEC"; };
  def<"\\IDECUDEC">{}        = [&] { return "UDEC"; };
  def<"\\IDECUHEX">{}        = [&] { return "UDEC"; };
  def<"\\IDECUTUP">{}        = [&] { return "UDEC"; };
  def<"\\IHEXIDEC">{}        = [&] { return "IDEC"; };
  def<"\\IHEXIHEX">{}        = [&] { return "IHEX"; };
  def<"\\IHEXUDEC">{}        = [&] { return "UDEC"; };
  def<"\\IHEXUHEX">{}        = [&] { return "UHEX"; };
  def<"\\IHEXUTUP">{}        = [&] { return "UHEX"; };
  def<"\\UDECIDEC">{}        = [&] { return "UDEC"; };
  def<"\\UDECIHEX">{}        = [&] { return "UDEC"; };
  def<"\\UDECUDEC">{}        = [&] { return "UDEC"; };
  def<"\\UDECUHEX">{}        = [&] { return "UDEC"; };
  def<"\\UDECUTUP">{}        = [&] { return "UDEC"; };
  def<"\\UHEXIDEC">{}        = [&] { return "UDEC"; };
  def<"\\UHEXIHEX">{}        = [&] { return "UHEX"; };
  def<"\\UHEXUDEC">{}        = [&] { return "UDEC"; };
  def<"\\UHEXUHEX">{}        = [&] { return "UHEX"; };
  def<"\\UHEXUTUP">{}        = [&] { return "UHEX"; };
  def<"\\UTUPIDEC">{}        = [&] { return "UDEC"; };
  def<"\\UTUPIHEX">{}        = [&] { return "UHEX"; };
  def<"\\UTUPUDEC">{}        = [&] { return "UDEC"; };
  def<"\\UTUPUHEX">{}        = [&] { return "UHEX"; };
  def<"\\UTUPUTUP">{}        = [&] { return "UTUP"; };

  return pp::cat(utl::slice(idecidec, -8), a, b);
});

} // namespace impl
} // namespace api

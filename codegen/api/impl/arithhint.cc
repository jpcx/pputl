#ifndef CODEGEN_API_IMPL_ARITHHINT_CC
#define CODEGEN_API_IMPL_ARITHHINT_CC
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
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//                                                                            //
//  You should have received a copy of the GNU General Public License        ///
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.  ////
///////////////////////////////////////////////////////////////////////////// */

#include "codegen.h"

namespace codegen {
namespace api {

using namespace std;

inline def<"impl_arithhint(a, b: INT|UINT, INT|UINT) -> INT|UINT"> arithhint = [](arg a,
                                                                                  arg b) {
  category = "impl";

  docs << "[internal] two-operand arithmetic cast hint."
       << ""
       << "immediately concatenates args."
       << ""
       << "returns UINT if either operand is UINT, else INT.";

  def<"\\INTINT"> idecidec = [&] {
    return "INT";
  };
  def<"\\INTUINT">{} = [&] {
    return "UINT";
  };
  def<"\\UINTINT">{} = [&] {
    return "UINT";
  };
  def<"\\UINTUINT">{} = [&] {
    return "UINT";
  };

  return pp::cat(utl::slice(idecidec, -8), a, b);
};

} // namespace api
} // namespace codegen

#endif

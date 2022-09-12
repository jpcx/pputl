#pragma once
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

#include <iomanip>

#include "codegen.h"
#include "config.h"
#include "lang.h"
//
#include "impl/traits.h"

namespace api {
namespace impl {

inline codegen::category<"impl.numeric"> numeric;

inline std::string bitlen = codegen::utl::ii << [] {
  std::ostringstream oss;
  oss << std::setfill('0') << std::setw(codegen::conf::word_size) << std::uppercase
      << std::hex << (codegen::conf::bit_length | 0);
  auto hex = oss.str();

  codegen::svect res{hex.size()};
  for (std::size_t i = 0; i < res.size(); ++i)
    res[i] = std::string{hex[i]};

  return "0x" + codegen::utl::cat(res) + "u";
};

extern codegen::def<"inc(n: uhex) -> uhex"> const& inc;
extern codegen::def<"dec(n: uhex) -> uhex"> const& dec;
extern codegen::def<"neg(n: uhex) -> uhex"> const& neg;
extern codegen::def<"ltz(n: uhex) -> bool"> const& ltz;

NIFTY_DECL(inc);
NIFTY_DECL(dec);
NIFTY_DECL(neg);
NIFTY_DECL(ltz);

inline codegen::end_category<"impl.numeric"> numeric_end;

} // namespace impl
} // namespace api

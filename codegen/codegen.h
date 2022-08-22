#ifndef CODEGEN_H_INCLUDED
#define CODEGEN_H_INCLUDED
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

#ifdef __cpp_lib_source_location
#  include <source_location>
#else
#  include <experimental/source_location>
#endif
#include <algorithm>
#include <limits>
#include <list>
#include <optional>
#include <regex>
#include <span>
#include <unordered_set>

namespace codegen {

namespace conf {

// prefix for feature macros.
constexpr char const api_prefix[]{"PTL_"};

// prefix for implementation macros.
// no trailing underscore by default to improve code completion.
constexpr char const impl_prefix[]{"PPUTL"};

// set the case of pputl names.
// ignores any chars after a backslash.
constexpr enum class name_case {
  screaming, // SCREAMING_CASE
  snake,     // snake_case
  camel,     // camelCase
  pascal,    // PascalCase
} name_case{name_case::screaming};

// the number of hex digits used to describes integers.
// hex representations are fixed at this length.
// word_size may range from 1 to 4 inclusive.
// word_size=4 requires cpp20_deflimit=false.
constexpr std::uint8_t word_size = 3;

// obeys maximum 256 args in all cases, in compliance with the C++20 standard.
// setting false allows type.size to describe tuples with up to uint_max items.
constexpr bool cpp20_arglimit = true;

// obeys maximum 65536 macro definitions in compliance with the C++20 standard.
// must be set false for word_size=4.
constexpr bool cpp20_deflimit = true;

// implementation name replacements for size reduction
constexpr std::array<char const*, 2> impl_aliases[]{
    {"recur_lp", "rlp"},
    {"recur_rp", "rrp"},
};

// output file for all preprocessor definitions.
constexpr char const lib_output[]{"./pputl.h"};

// header guard for the lib output.
constexpr char const header_guard[]{"PPUTL_H_INCLUDED"};

// output file for all tests.
constexpr char const tests_output[]{"./tests.cc"};

// clang format style file (for final formatting).
constexpr char const clang_format_input[]{"./.clang-format"};

constexpr char const project_header[]{
    "/* /////////////////////////////////////////////////////////////////////////////\n"
    "//                          __    ___                                         //\n"
    "//                         /\\ \\__/\\_ \\                                        "
    "//\n"
    "//   _____   _____   __  __\\ \\ ,_\\//\\ \\                                       "
    "//\n"
    "//  /\\ '__`\\/\\ '__`\\/\\ \\/\\ \\\\ \\ \\/ \\ \\ \\                              "
    "        "
    "//\n"
    "//  \\ \\ \\_\\ \\ \\ \\_\\ \\ \\ \\_\\ \\\\ \\ \\_ \\_\\ \\_                       "
    "          "
    "   //\n"
    "//   \\ \\ ,__/\\ \\ ,__/\\ \\____/ \\ \\__\\/\\____\\                              "
    "     //\n"
    "//    \\ \\ \\   \\ \\ \\   \\/___/   \\/__/\\/____/                                "
    "   //\n"
    "//     \\/_/    \\/_/                                                           //\n"
    "//                                                                            //\n"
    "//  pputl Preprocessor Utilities                                              //\n"
    "//  Copyright (C) 2020 - 2022 Justin Collier <m@jpcx.dev>                     //\n"
    "//                                                                            //\n"
    "//    This program is free software: you can redistribute it and/or modify    //\n"
    "//    it under the terms of the GNU General Public License as published by    //\n"
    "//    the Free Software Foundation, either version 3 of the License, or       //\n"
    "//    (at your option) any later version.                                     //\n"
    "//                                                                            //\n"
    "//    This program is distributed in the hope that it will be useful,         //\n"
    "//    but WITHOUT ANY WARRANTY; without even the implied warranty of          //\n"
    "//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           //\n"
    "//    GNU General Public License for more details.                            //\n"
    "//                                                                            //\n"
    "//  You should have received a copy of the GNU General Public License        ///\n"
    "//  along with this program.  If not, see <https://www.gnu.org/licenses/>.  ////\n"
    "//                                                                         /////\n"
    "////////////////////////////////////////////////////////////////////////////////\n"
    "//                                                                        `/////\n"
    "//    PREAMBLE                                                             `////\n"
    "//    --------                                                              `///\n"
    "//                                                                           `//\n"
    "//    Macro functions are generally not advisable in production code. They    //\n"
    "//    are difficult to reason about, pollute the global namespace, and can    //\n"
    "//    hinder debugging and refactoring efforts.  C++ has evolved to enable    //\n"
    "//    countless metaprogramming techniques that should be preferred.          //\n"
    "//                                                                            //\n"
    "//    This library is built to provide a strong, safe set of functionality    //\n"
    "//    for edge cases that still uniquely benefit from text replacement and    //\n"
    "//    would otherwise utilize a separate code generation script or require    //\n"
    "//    higly verbose/redundant syntax, such as comprehensive test coverage,    //\n"
    "//    struct reflection, static initialization control, or optimization of    //\n"
    "//    algorithms that generate template specializations.                      //\n"
    "//                                                                            //\n"
    "//    ABOUT                                                                   //\n"
    "//    -----                                                                   //\n"
    "//                                                                            //\n"
    "//    pputl is a powerful C++ preprocessor utilities library that features    //\n"
    "//    many high-level constructs including integers and range algorithms.     //\n"
    "//                                                                            //\n"
    "//    pputl is designed with speed, safety, and flexibility in mind.          //\n"
    "//                                                                            //\n"
    "//    Algorithms are implemented  using a preprocessor syntax manipulation    //\n"
    "//    technique for constructing inline recursive call stacks that execute    //\n"
    "//    much faster than mutually recursive patterns and can be nested.         //\n"
    "//                                                                            //\n"
    "//    Metaprogramming utilities are provided for arbitrary recursion.         //\n"
    "//                                                                            //\n"
    "//    Several arithmetic, bitwise, and comparison functions  are partially    //\n"
    "//    pre-computed;  lookup tables  are generated by the build system that    //\n"
    "//    assist with the calculation of several critical bitwise operations.     //\n"
    "//                                                                            //\n"
    "//    Functions verify their arguments and use type casting to ensure that    //\n"
    "//    useful error messages are generated as soon as an issue manifests.      //\n"
    "//                                                                            //\n"
    "//    Features overview:                                                      //\n"
    "//                                                                            //\n"
    "//     ◆ language enhancements                                                //\n"
    "//       ‐ basic argument manipulation                              [lang]    //\n"
    "//           eat  esc  first  xfirst  rest  xrest  trim                       //\n"
    "//       ‐ control flow                                    [lang, control]    //\n"
    "//           default  fail  if  switch                                        //\n"
    "//       ‐ type casting                            [type; see TERMINOLOGY]    //\n"
    "//           none  some  obj   atom  bool  hex   nybl  int   idec             //\n"
    "//           ihex  uint  udec  uhex  tup   utup  word  size  any              //\n"
    "//       ‐ traits detection                                       [traits]    //\n"
    "//           is_none  is_some  is_obj   is_atom  is_bool  is_hex   is_nybl    //\n"
    "//           is_int   is_idec  is_ihex  is_uint  is_udec  is_uhex  is_tup     //\n"
    "//           is_utup  is_word  is_size  is_any   typeof   sizeof              //\n"
    "//       ‐ boolean logic                                           [logic]    //\n"
    "//           not  and  or  xor  nand  nor  xnor                               //\n"
    "//       ‐ paste formatting                                    [lang, fmt]    //\n"
    "//           str  xstr  cat  xcat  c_int  c_hex  c_bin                        //\n"
    "//     ◆ signed and unsigned integers                                         //\n"
    "//       ‐ arithmetic                                      [numeric, math]    //\n"
    "//           inc  dec  add   sub   mul   divr                                 //\n"
    "//           div  mod  pow2  sqrt  log2  fact                                 //\n"
    "//       ‐ comparison                                   [numeric, compare]    //\n"
    "//           eqz  nez  ltz  gtz  lez  gez  lt                                 //\n"
    "//           gt   le   ge   eq   ne   min  max                                //\n"
    "//       ‐ bitwise operations                                    [bitwise]    //\n"
    "//           bdump  bsll  bsrl   bsra  bnot  band   bor    bxor               //\n"
    "//           bnand  bnor  bxnor  bget  bset  bflip  brotl  brotr              //\n"
    "//     ◆ range algorithms                                                     //\n"
    "//       ‐ element access                                          [range]    //\n"
    "//           items  bisect  unite  get  set  push  pop  slice                 //\n"
    "//       ‐ generation                                               [algo]    //\n"
    "//           seq  repeat  gen_lp  gen_rp                                      //\n"
    "//       ‐ transformation                                           [algo]    //\n"
    "//           rev  map_lp  map_rp  shift  rotate                               //\n"
    "//       ‐ reduction                                                [algo]    //\n"
    "//           reduce_lp  reduce_rp                                             //\n"
    "//     ◆ metaprogramming utilities                                            //\n"
    "//       ‐ expansion control and tracing                            [meta]    //\n"
    "//           x  lp  rp  xtrace  xtrace_read                                   //\n"
    "//       ‐ mutually recursive stack expansion                       [meta]    //\n"
    "//           xx_lp  xx_rp                                                     //\n"
    "//       ‐ inline recursive stack construction                      [meta]    //\n"
    "//           recur_lp  recur_rp                                               //\n"
    "//     ◆ configuration details                                    [config]    //\n"
    "//           build    word_size  bit_length  int_min                          //\n"
    "//           int_max  uint_max   size_max                                     //\n"
    "//                                                                            //\n"
    "//    USAGE                                                                   //\n"
    "//    -----                                                                   //\n"
    "//                                                                            //\n"
    "//    Copy pputl.h and include. The distribution is single-header.            //\n"
    "//                                                                            //\n"
    "//    pputl needs a C++20-compliant preprocessor but has no dependencies.     //\n"
    "//                                                                            //\n"
    "//    pputl is completely generated and tested by a custom C++ framework.     //\n"
    "//    See the codegen/ folder for the full source.                            //\n"
    "//                                                                            //\n"
    "//    Various settings including word size and naming rules may be changed    //\n"
    "//    by modifying the head of codegen/codegen.h and running `make`.          //\n"
    "//                                                                            //\n"
    "//    Supported integer modes:                                                //\n"
    "//                                                                            //\n"
    "//      word_size=1    ⋮   4-bit integers  ⋮   ~? KiB                         //\n"
    "//      word_size=2    ⋮   8-bit integers  ⋮   ~? KiB                         //\n"
    "//      word_size=3    ⋮  12-bit integers  ⋮  [~? MiB (default)]              //\n"
    "//      word_size=4 †  ⋮  16-bit integers  ⋮   ~? MiB                         //\n"
    "//                                          ________________________________  //\n"
    "//                                          †: requires cpp20_deflimit=false  //\n"
    "//                                                                            //\n"
    "//    By default, pputl is fully compliant with the C++20 standard,           //\n"
    "//    which defines the following implementation limits in [implimits]:       //\n"
    "//                                                                            //\n"
    "//     ‐ Macro identifiers simultaneously                                     //\n"
    "//       defined in one translation unit: [65536].                            //\n"
    "//     ‐ Parameters in one macro definition: [256].                           //\n"
    "//     ‐ Arguments in one macro invocation: [256].                            //\n"
    "//                                                                            //\n"
    "//    Settings are available to ignore these limits, but support for sizes    //\n"
    "//    and macro definition counts higher than the standard is variable.       //\n"
    "//                                                                            //\n"
    "//    pputl has been tested with:                                             //\n"
    "//                                                                            //\n"
    "//      gcc   11.2.1                                                          //\n"
    "//      clang 13.0.0                                                          //\n"
    "//      MSVC  2022 (requires /Zc:preprocessor)                                //\n"
    "//                                                                            //\n"
    "//    Run `make test` to validate on your system.                             //\n"
    "//                                                                            //\n"
    "//    TERMINOLOGY                                                             //\n"
    "//    -----------                                                             //\n"
    "//                                                                            //\n"
    "//    pputl defines several types and uses type casting and traits testing    //\n"
    "//    for control flow and safety;  parameters documented with these types    //\n"
    "//    are verified by invoking the appropriate cast or traits function.       //\n"
    "//                                                                            //\n"
    "//      none: an absence of tokens                                            //\n"
    "//      some: <abstract> something; not nothing [e.g. foo] [e.g. ,,,]         //\n"
    "//       └─ obj: exactly one generic value                                    //\n"
    "//          ├─ atom: a generic value not surrounded by parentheses            //\n"
    "//          │   ├─ bool: a literal '1' or '0'                                 //\n"
    "//          │   ├─ hex:  a literal uppercase hexadecimal digit [e.g. B]       //\n"
    "//          │   ├─ nybl: a literal 4-bit bool concatenation [e.g. 0110]       //\n"
    "//          │   ├─ int: <abstract> a word-sized signed integer                //\n"
    "//          │   │   ├─ idec: a positive 2s-complement decimal [e.g. 3]        //\n"
    "//          │   │   └─ ihex: a signed hex integer [e.g. 0x861]                //\n"
    "//          │   └─ uint: <abstract> a word-sized unsigned integer             //\n"
    "//          │       ├─ udec: an unsigned decimal integer [e.g. 42u]           //\n"
    "//          │       └─ uhex: an unsigned hex integer [e.g. 0x02Au]            //\n"
    "//          ├─ tup: parenthesised items [docs: tup, (T...), (T, U), etc.]     //\n"
    "//          │   └─ utup: an unsigned word as a tup of hex [e.g. (6, D, 2)]    //\n"
    "//          └─ word: <union> int | uint | utup                                //\n"
    "//              └─ size: an unsigned word capped by the argument limit        //\n"
    "//      any: <union> none | obj; any kind of individual macro argument        //\n"
    "//                                                                            //\n"
    "//    FUNDAMENTALS                                                            //\n"
    "//    ------------                                                            //\n"
    "//                                                                            //\n"
    "//    pputl errors execute  an invalid preprocessor operation by using the    //\n"
    "//    concatenation operator (incorrectly) on a string error message.  All    //\n"
    "//    errors  triggered by  pputl functions  will include  the macro name,    //\n"
    "//    a textual description, and its primary expansion arguments.             //\n"
    "//                                                                            //\n"
    "//    With a few exceptions in [lang], non-nullary API functions are fully    //\n"
    "//    variadic and chainable  such that the outputs of one  may be used as    //\n"
    "//    inputs to another.  Inputs must be distinguishable after the primary    //\n"
    "//    expansion; deferred input behavior is undefined.                        //\n"
    "//                                                                            //\n"
    "//    Hexadecimal integers are always represented by fixed-length strings.    //\n"
    "//    Negative ints cannot be represented in decimal  due to concatenation    //\n"
    "//    restrictions. Arithmetic and bitwise functions attempt to cast their    //\n"
    "//    results in the same form as their input, but will always return ihex    //\n"
    "//    when an idec input becomes negative.  Decimal representations may be   ///\n"
    "//    generated for pasting using fmt.paste.                                ////\n"
    "//                                                                         /////\n"
    "///////////////////////////////////////////////////////////////////////////// */"};

// derived config:

constexpr std::uint8_t bit_length = word_size * 4;
constexpr unsigned     uint_max   = ([] {
  unsigned res{1};
  for (unsigned i = 0; i < word_size * 4; ++i)
    res *= 2;
  return res - 1;
})();
constexpr unsigned     size_max   = cpp20_arglimit ? std::min(255u, uint_max) : uint_max;
constexpr int          int_max    = uint_max / 2;
constexpr int          int_min    = -((uint_max + 1) / 2);

static_assert(word_size >= 1);
static_assert(word_size <= 4, "higher values may be possible, but are not tested");
static_assert(word_size != 4 or not cpp20_deflimit,
              "word_size=4 requires ignoring cpp20 definition limit");

} // namespace conf

// shortcuts
using svect = std::vector<std::string>;

// samples for docs and tests
namespace samp {
inline auto hmin  = svect(conf::word_size, "0");
inline auto hmax  = svect(conf::word_size, "F");
inline auto himax = ([] {
  auto _ = hmax;
  _[0]   = "7";
  return _;
})();
inline auto himin = ([] {
  auto _ = hmin;
  _[0]   = "8";
  return _;
})();
inline auto h1    = ([] {
  auto res   = hmin;
  res.back() = "1";
  return res;
})();
inline auto h2    = ([] {
  auto res   = hmin;
  res.back() = "2";
  return res;
})();
inline auto h3    = ([] {
  auto res   = hmin;
  res.back() = "3";
  return res;
})();
inline auto h4    = ([] {
  auto res   = hmin;
  res.back() = "4";
  return res;
})();
inline auto h5    = ([] {
  auto res   = hmin;
  res.back() = "5";
  return res;
})();
inline auto h6    = ([] {
  auto res   = hmin;
  res.back() = "6";
  return res;
})();
inline auto h7    = ([] {
  auto res   = hmin;
  res.back() = "7";
  return res;
})();
inline auto h8    = ([] {
  auto res   = hmin;
  res.back() = "8";
  return res;
})();
inline auto h9    = ([] {
  auto res   = hmin;
  res.back() = "9";
  return res;
})();
inline auto h10   = ([] {
  auto res   = hmin;
  res.back() = "A";
  return res;
})();
inline auto h11   = ([] {
  auto res   = hmin;
  res.back() = "B";
  return res;
})();
inline auto h12   = ([] {
  auto res   = hmin;
  res.back() = "C";
  return res;
})();
inline auto h13   = ([] {
  auto res   = hmin;
  res.back() = "D";
  return res;
})();
inline auto h14   = ([] {
  auto res   = hmin;
  res.back() = "E";
  return res;
})();
inline auto h15   = ([] {
  auto res   = hmin;
  res.back() = "F";
  return res;
})();
inline auto h16   = ([] {
  if constexpr (conf::word_size > 1) {
    auto res            = hmin;
    *(res.rbegin() + 1) = "1";
    return res;
  } else {
    return svect();
  }
})();
} // namespace samp

namespace detail {
template<std::size_t Ext>
  requires(Ext > 0)
struct string_literal {
  std::array<char, Ext> const _data;

  constexpr string_literal(char const (&s)[Ext])
      : _data{([&] {
          std::array<char, Ext> data{};
          std::ranges::copy(s, data.begin());
          return data;
        })()} {
  }

  [[nodiscard]] constexpr bool
  empty() const noexcept {
    return _data.size() == 1;
  }

  [[nodiscard]] constexpr std::size_t
  size() const noexcept {
    return _data.size() - 1;
  }

  [[nodiscard]] constexpr char const*
  c_str() const noexcept {
    return _data.data();
  }

  [[nodiscard]] constexpr char const*
  begin() const noexcept {
    return _data.data();
  }

  [[nodiscard]] constexpr char const*
  end() const noexcept {
    return _data.data() + size();
  }

  [[nodiscard]] constexpr operator char const*() const noexcept {
    return _data.data();
  }
};

/// describes forward ranges whose reference type is convertible to V&.
template<class R, class V>
concept forward_iterable_for = (std::ranges::forward_range<R>)and(
    std::convertible_to<std::ranges::range_reference_t<R>, V&>);
} // namespace detail

namespace utl {

/// describes types that are either string-convertible or compatible with std::to_string.
template<class T>
concept string_representable = std::convertible_to<T, std::string> or requires(T&& v) {
  { std::to_string(std::forward<T>(v)) } -> std::same_as<std::string>;
};

/// an overload of std::to_string that allows string-convertible types.
template<string_representable T>
[[nodiscard]] std::string to_string(T&& v);

[[nodiscard]] std::string slice(std::string const& s, int end_ofs);
[[nodiscard]] std::string slice(std::string const& s, int begin_ofs, int end_ofs);
[[nodiscard]] std::vector<std::string> split(std::string const& target,
                                             std::regex const&  delim);
template<detail::forward_iterable_for<std::string const> Strs>
[[nodiscard]] std::string              cat(Strs&& strs, std::string const& delim = "");
[[nodiscard]] std::string              alpha_base52(std::size_t num); // a-zA-Z
[[nodiscard]] std::vector<std::string> base10_seq(std::size_t size,
                                                  std::string begin = "0"); // 0-9
[[nodiscard]] std::vector<std::string>
                          alpha_base52_seq(std::size_t size,
                                           std::string begin = "a"); // a-zA-Z
[[nodiscard]] std::string prefix_lines(std::string const& prefix,
                                       std::string const& multiline);
template<std::unsigned_integral Int>
[[nodiscard]] std::vector<Int> prime_factors(Int n);
template<std::unsigned_integral Int>
[[nodiscard]] inline Int next_le_pow2(Int n);
template<std::unsigned_integral Int>
[[nodiscard]] inline Int next_ge_pow2(Int n);

template<class T>
using storage = std::aligned_storage_t<sizeof(std::remove_cvref_t<T>),
                                       alignof(std::remove_cvref_t<T>)>;

template<class T>
class nifty {
 public:
  using value_type = std::remove_cvref_t<T>;

 private:
  unsigned            _refct{0};
  storage<value_type> _data{};

 public:
  [[nodiscard]] constexpr operator value_type const&() const noexcept {
    return reinterpret_cast<value_type const&>(_data);
  }

  [[nodiscard]] constexpr
  operator value_type&() noexcept {
    return reinterpret_cast<value_type&>(_data);
  }

  template<class... Args>
    requires(std::constructible_from<value_type, Args...>)
  void
  ref(Args&&... args) noexcept {
    if (not _refct++)
      new (&_data) value_type{std::forward<Args>(args)...};
  }

  void
  unref() noexcept {
    if (not --_refct)
      reinterpret_cast<value_type&>(_data).~value_type();
  }
};

#define NIFTY_DECL(extern_ref_name)                      \
  extern codegen::utl::nifty<decltype(extern_ref_name)>  \
      internal_nifty##extern_ref_name##storage_;         \
  static struct internal_nifty##extern_ref_name##decl_ { \
    internal_nifty##extern_ref_name##decl_();            \
    ~internal_nifty##extern_ref_name##decl_();           \
  } internal_nifty##extern_ref_name##decl_

#define NIFTY_DEF(extern_ref_name, ...)                                               \
  internal_nifty##extern_ref_name##storage_;                                          \
  internal_nifty##extern_ref_name##decl_::internal_nifty##extern_ref_name##decl_() {  \
    internal_nifty##extern_ref_name##storage_.ref(__VA_ARGS__);                       \
  }                                                                                   \
  internal_nifty##extern_ref_name##decl_::~internal_nifty##extern_ref_name##decl_() { \
    internal_nifty##extern_ref_name##storage_.unref();                                \
  }                                                                                   \
  codegen::utl::nifty<decltype(extern_ref_name)>                                      \
      internal_nifty##extern_ref_name##storage_ {                                     \
  }

} // namespace utl

[[nodiscard]] std::string apiname(std::string const& short_name); // uses conf::api_prefix
[[nodiscard]] std::string implname(std::string short_name); // uses conf::impl_prefix

namespace pp {
template<detail::forward_iterable_for<std::string const> Args>
[[nodiscard]] std::string va_opt(Args&& args);
template<detail::forward_iterable_for<std::string const> Args>
[[nodiscard]] std::string cat(Args&& args);
template<bool Space = true, detail::forward_iterable_for<std::string const> Args>
[[nodiscard]] std::string tup(Args&& args);
template<std::convertible_to<std::string>                Fn,
         detail::forward_iterable_for<std::string const> Args>
[[nodiscard]] std::string call(Fn&& fn, Args&& args);

template<utl::string_representable... Args>
[[nodiscard]] std::string va_opt(Args&&... args);
template<utl::string_representable... Args>
[[nodiscard]] std::string cat(Args&&... args);
template<bool Space = true, utl::string_representable... Args>
[[nodiscard]] std::string tup(Args&&... args);
template<utl::string_representable... Args>
[[nodiscard]] std::string str(Args&&... args);
template<utl::string_representable Fn, utl::string_representable... Args>
[[nodiscard]] std::string call(Fn&& fn, Args&&... args);

} // namespace pp

class arg;
class va;
class pack;

namespace detail {

/// describes types that can be used to construct a standard function.
/// supports any kind of non-templated signature.
template<class F>
concept functor = requires(F&& v) {
  {std::function{std::forward<F>(v)}};
};

template<functor>
struct functor_traits;

template<class Ret, class... Args>
struct functor_traits<std::function<Ret(Args...)>> {
  using return_type = Ret;
  using params_type = std::tuple<Args...>;
};

/// a tuple of the params of a functor.
template<functor F>
using functor_params_type =
    typename functor_traits<decltype(std::function{std::declval<F>()})>::params_type;

/// the return type of a functor.
template<functor F>
using functor_return_type =
    typename functor_traits<decltype(std::function{std::declval<F>()})>::return_type;

template<class T, class FirstAllowed,
         class... RestAllowed> // enforce at least one comparison
concept same_as_any_of =
    std::same_as<T, FirstAllowed> or(std::same_as<T, RestAllowed> || ...);

template<class Tup, class FirstAllowed, class... RestAllowed, std::size_t... Idx>
[[nodiscard]] inline constexpr bool
elements_same_as_any_of_impl(std::index_sequence<Idx...>&&) noexcept {
  return (same_as_any_of<std::tuple_element_t<Idx, Tup>, FirstAllowed,
                         RestAllowed...> && ...);
}

template<class Tup, class FirstAllowed, class... RestAllowed>
concept elements_same_as_any_of = ([] {
  if constexpr (std::tuple_size_v<Tup> == 0) {
    return true;
  } else {
    return elements_same_as_any_of_impl<Tup, FirstAllowed, RestAllowed...>(
        std::make_index_sequence<std::tuple_size_v<Tup>>{});
  }
})();

/// describes a functor that returns string and takes any number of string parameters or a
/// vector<string>.
template<class T>
concept body_functor = functor<T>                                                       //
    and std::convertible_to<detail::functor_return_type<T>, std::string>                //
    and(                                                                                //
        std::tuple_size_v<detail::functor_params_type<T>> == 0                          //
        or (std::tuple_size_v<detail::functor_params_type<T>> == 1                      //
            and elements_same_as_any_of<detail::functor_params_type<T>, arg, va, pack>) //
        or (std::tuple_size_v<detail::functor_params_type<T>> > 1                       //
            and elements_same_as_any_of<detail::functor_params_type<T>, arg, va>)       //
    );

#ifdef __cpp_lib_source_location
using std::source_location;
#else
using std::experimental::source_location;
#endif

[[nodiscard]] constexpr bool
isspace(char c) noexcept {
  return c == ' ' or c == '\t' or c == '\r' or c == '\n';
}

template<std::forward_iterator Begin, std::sentinel_for<Begin> End>
  requires(std::same_as<std::iter_value_t<Begin>, char>)
[[nodiscard]] inline constexpr Begin
match_syntax_grouping(Begin begin, End end, char open, char close) noexcept {
  if (*begin != open)
    return end;
  int depth = 0;
  for (; begin != end; ++begin) {
    if (*begin == open)
      ++depth;
    else if (*begin == close)
      if (--depth == 0)
        break;
  }
  return begin;
}

template<std::forward_iterator Begin, std::sentinel_for<Begin> End>
  requires(std::same_as<std::iter_value_t<Begin>, char>)
[[nodiscard]] inline constexpr Begin
trim_begin(Begin begin, End end) noexcept {
  for (; begin != end and isspace(*begin); ++begin)
    ;
  return begin;
}

template<std::bidirectional_iterator End, std::sentinel_for<End> Begin>
  requires(std::same_as<std::iter_value_t<End>, char>)
[[nodiscard]] inline constexpr End
trim_end(Begin begin, End end) noexcept {
  for (; begin != end and isspace(*(end - 1)); --end)
    ;
  return end;
}

[[nodiscard]] inline constexpr bool
is_alphanum(char ch) noexcept {
  return (ch >= '0' and ch <= '9') or (ch >= 'A' and ch <= 'Z')
      or (ch >= 'a' and ch <= 'z');
}

} // namespace detail

namespace utl {

// immediate invocation helper.
inline struct {
  template<detail::functor Fn>
    requires(std::same_as<detail::functor_params_type<Fn>, std::tuple<>>)
  constexpr auto
  operator<<(Fn&& f) const noexcept {
    if constexpr (std::same_as<detail::functor_return_type<Fn>, void>) {
      std::forward<Fn>(f)();
      return nullptr;
    } else {
      return std::forward<Fn>(f)();
    }
  }
} ii;

} // namespace utl

namespace detail {

template<class T>
struct span_literal {
  using value_type = T;

  T* const _begin;
  T* const _end;

  constexpr span_literal() : _begin{nullptr}, _end{nullptr} {
  }

  template<std::contiguous_iterator Begin, std::sentinel_for<Begin> End>
  constexpr span_literal(Begin begin, End end) : _begin{&*begin}, _end{&*end} {
  }

  template<std::ranges::contiguous_range Rng>
    requires(std::same_as<std::ranges::range_value_t<Rng>, T>)
  constexpr span_literal(Rng&& rng)
      : _begin{&*std::ranges::begin(rng)}, _end{&*std::ranges::end(rng)} {
  }

  [[nodiscard]] constexpr T*
  begin() const noexcept {
    return _begin;
  }

  [[nodiscard]] constexpr T*
  end() const noexcept {
    return _end;
  }

  [[nodiscard]] constexpr std::size_t
  size() const noexcept {
    return std::distance(_begin, _end);
  }

  [[nodiscard]] constexpr bool
  empty() const noexcept {
    return size() == 0;
  }
};

template<class T>
  requires(std::is_default_constructible_v<T>)
struct opt_literal {
  T const    _data;
  bool const _has_value;

  constexpr opt_literal() : _data{}, _has_value{false} {
  }

  constexpr opt_literal(T const& data) : _data{data}, _has_value{true} {
  }

  [[nodiscard]] constexpr operator bool() const noexcept {
    return _has_value;
  }

  [[nodiscard]] constexpr T const*
  operator->() const noexcept {
    return &_data;
  }

  [[nodiscard]] constexpr T const&
  operator*() const noexcept {
    return _data;
  }
};

} // namespace detail

// parses a macro definition signature. fully constexpr, literal type.
// all spans are trimmed of whitespace.
//
// allows names to start with a digit with the expectation that some signatures
// will be concatenated with their context.
//
// [[ warning: does not own char data ]]
//
// examples:
//                            name,  params,     docparams, xct, docreturn
//   foo                      "foo", "",         "",        ""   ""
//   foo -> bar               "foo", "",         "",        ""   "bar"
//   foo()                    "foo", "",         "",        ""   ""
//   foo()         -> bar     "foo", "",         "",        ""   "bar"
//   foo(bar)      -> baz     "foo", "bar",      "",        ""   "baz"
//   foo(bar, baz) -> qux     "foo", "bar, baz", "",        ""   "qux"
//   foo(...: bar) -> baz     "foo", "...",      "bar",     ""   "baz"
//   foo(bar: baz) -> qux     "foo", "bar",      "baz",     ""   "qux"
//   foo(bar: baz) -<3>-> qux "foo", "bar",      "baz",     "3"  "qux"
struct signature_literal {
  using span_literal = detail::span_literal<char const>;
  template<class T>
  using opt_literal               = detail::opt_literal<T>;
  static constexpr auto const& ii = utl::ii;

  opt_literal<span_literal> const data;
  constexpr signature_literal(char const* sig)
      : // set data to [first nonspace char, last nonspace char]
        data{ii << [&]() -> decltype(data) {
          // invalid if null or empty
          if (not sig or not *sig)
            return {};

          char const* begin = sig;
          char const* end   = begin; // nullterm
          for (; *end; ++end)
            ;

          begin = detail::trim_begin(begin, end);
          end   = detail::trim_end(begin, end);

          // fail if only whitespace
          if (begin == end)
            return {};

          return span_literal{begin, end};
        }} {
  }

  // set name to [first nonspace char, first non-name char)
  opt_literal<span_literal> const name = ii << [&]() -> decltype(name) {
    // propagate failures
    if (not data)
      return {};

    return {span_literal{data->begin(), std::ranges::find_if_not(*data, [](char ch) {
                           return detail::is_alphanum(ch) or ch == '_' or ch == '$'
                               or ch == '\\' or ch == '<';
                         })}};
  };

  // marked a function if there is at least one open parens
  opt_literal<span_literal> const params_decl = ii << [&]() -> decltype(params_decl) {
    // propagate failures
    if (not data)
      return {};

    // find first parens
    auto begin = std::ranges::find(*data, '(');
    if (begin == data->end())
      return span_literal{begin, begin};

    // find matching parens
    auto end = detail::match_syntax_grouping(begin, data->end(), '(', ')');

    // fail if no matching parens found
    if (end == data->end())
      return {};

    // ends just after closing parens
    return span_literal{begin, std::next(end)};
  };

  // set params to
  //   [ first nonspace char after first parens,
  //     last nonspace char before inner ':' or outer ')' ]
  //
  // empty if nullary or non-function
  opt_literal<span_literal> const params = ii << [&]() -> decltype(params) {
    // propagate failures
    if (not data or not name or not params_decl)
      return {};

    // return empty if non-function
    if (params_decl->empty())
      return {span_literal{data->begin(), data->begin()}};

    // copy outer params iterators
    // set end to closing parens
    auto       begin = params_decl->begin();
    auto const end   = std::prev(params_decl->end());

    // move begin to the first non-space after the open parens
    begin = detail::trim_begin(begin + 1, end);

    // hit the end; this must be a nullary function
    if (begin == end)
      return span_literal{begin, end};

    // either ends at outer_end (')') or docparams begin (':')
    auto params_end = std::find(begin, end, ':');

    return span_literal{detail::trim_begin(begin, params_end),
                        detail::trim_end(begin, params_end)};
  };

  // set docparams to
  //   [ first nonspace char after first ':',
  //     last nonspace char before outer ')' ]
  opt_literal<span_literal> const docparams = ii << [&]() -> decltype(docparams) {
    // propagate failures
    if (not data or not name or not params_decl or not params)
      return {};

    // don't parse docparams if this is a non-function or a nullary function
    if (params->empty())
      return span_literal{data->begin(), data->begin()};

    // find ':' within parentheses
    // set end to closing parens
    auto       begin = std::find(params->end(), params_decl->end(), ':');
    auto const end   = std::prev(params_decl->end());

    // no docparams to parse
    if (begin == params_decl->end())
      return span_literal{begin, begin};

    // fail if ':' is not followed by anything meaningful
    if (detail::trim_begin(begin + 1, end) == end)
      return {};

    return span_literal{detail::trim_begin(begin + 1, end),
                        detail::trim_end(begin + 1, end)};
  };

  opt_literal<span_literal> const xout = ii << [&]() -> decltype(xout) {
    // propagate failures
    if (not data or not name or not params or not docparams)
      return {};

    // start search at the furthest end parsed
    auto begin = not docparams->empty() ? docparams->end()
               : not params->empty()    ? params->end()
                                        : name->end();

    // restart search at first occurence of '-'
    begin = std::find(begin, data->end(), '-');

    // return empty if no '-' found, with begin/end set to data end
    if (begin == data->end())
      return span_literal{begin, begin};

    // fail if '-' is the last char
    if (begin + 1 == data->end())
      return {};

    if (*(begin + 1) == '<') {
      // expansion docs found
      ++begin;

      // find closing '>'
      auto end = detail::match_syntax_grouping(begin, data->end(), '<', '>');

      // fail if no closing '>' found
      if (end == data->end())
        return {};

      // adjust begin to first nonspace
      begin = detail::trim_begin(begin + 1, end);

      // fail if first nonspace is ending '>'
      if (begin == end)
        return {};

      // returns sizey range
      return span_literal{
          begin,
          detail::trim_end(begin, end)}; // -> end is last nonspace before closing '>'
    } else if (*(begin + 1) == '>') {
      // return empty if no xout but valid docreturn arrow;
      // set begin to arrow body
      return span_literal{begin + 1, begin + 1}; // -> end is last '>' before docreturn
    } else {
      // fail if some other char follows '-' that is not the arrow end
      return {};
    }
  };

  opt_literal<span_literal> const docreturn = ii << [&]() -> decltype(docreturn) {
    // propagate failures
    if (not data or not xout)
      return {};

    // start search at xout end
    auto begin = xout->end();

    // if xout ends at data end, no docreturn
    if (begin == data->end())
      return span_literal{begin, begin};

    // xout was included; find arrow end
    if (not xout->empty()) {
      auto it = std::find(begin, data->end(), '>');

      // fail if '>' was somehow not found
      if (it == data->end())
        return {};

      // fail if closing '>' is not followed by "->"
      if (it + 1 == data->end() or it + 2 == data->end() or *(it + 1) != '-'
          or *(it + 2) != '>')
        return {};

      begin = it + 2; // set to last '>' before docreturn
    }

    // find first nonspace after arrow-end '>'
    begin = detail::trim_begin(begin + 1, data->end());

    // return docreturn
    return span_literal{begin, detail::trim_end(begin, data->end())};
  };

  // invalid if any member returned nullopt
  bool const valid{data and name and params_decl and params and docparams and xout
                   and docreturn};
};

struct runtime_signature : private signature_literal {
  runtime_signature(signature_literal const& sig) : signature_literal{sig} {
    if (not valid)
      throw std::runtime_error{"invalid signature copied from signature literal"};
  }

  template<std::size_t Ext>
  runtime_signature(char const (&sig)[Ext]) : signature_literal{sig} {
    if (not valid)
      throw std::runtime_error{"invalid signature " + std::string{sig}};
  }

  runtime_signature(char const* sig) : signature_literal{sig} {
    if (not valid)
      throw std::runtime_error{"invalid signature " + std::string{sig}};
  }

  runtime_signature(std::string const& sig) : signature_literal{sig.c_str()} {
    if (not valid)
      throw std::runtime_error{"invalid signature " + sig};
  }

  std::string const data{
      signature_literal::data
          ? std::string{signature_literal::data->begin(), signature_literal::data->end()}
          : ""};
  std::string const name{
      signature_literal::name
          ? std::string{signature_literal::name->begin(), signature_literal::name->end()}
          : ""};
  bool const        is_fn{signature_literal::params_decl
                   and not signature_literal::params_decl->empty()};
  std::string const params = utl::ii << [&] {
    std::string params = signature_literal::params
                           ? std::string{signature_literal::params->begin(),
                                         signature_literal::params->end()}
                           : "";

    if (params.empty())
      return params;
    // replace any keywords in parameters
    static std::unordered_set<std::string> const reserved{
        "and",    "and_eq", "bitand", "bitor", "compl", "not",
        "not_eq", "or",     "or_eq",  "xor",   "xor_eq"};

    std::vector<std::string> clean_params{};
    for (auto&& v : utl::split(params, std::regex{", ?"})) {
      if (not reserved.contains(v))
        clean_params.push_back(v);
      else
        clean_params.push_back("_" + v);
    }
    params = utl::cat(clean_params, ", ");
    return params;
  };

  std::string const docparams{signature_literal::docparams
                                  ? std::string{signature_literal::docparams->begin(),
                                                signature_literal::docparams->end()}
                                  : ""};
  std::string const xout{
      signature_literal::xout
          ? std::string{signature_literal::xout->begin(), signature_literal::xout->end()}
          : ""};
  std::string const docreturn{signature_literal::docreturn
                                  ? std::string{signature_literal::docreturn->begin(),
                                                signature_literal::docreturn->end()}
                                  : ""};
};

/// base class for macro construction. manages static instances by id.
/// ids are constructed using the macro name and the current execution stack.
/// macro bodies are executed on construction and may define additional macros.
class def_base {
 private:
  struct instance;

  /// all macro instances uniquely identifiable by their id.
  /// existing instances are reused by new macros with a matching id.
  static inline std::list<instance> _instances{};

  /// a stack of macro instance iterators that is pushed to when a
  /// macro definition is being calculated (its body is
  /// being called), and popped from after body execution.
  static inline std::vector<instance*> _exec_stack{};

  /// all defined categories sorted by their position in the dependency graph.
  static inline std::vector<std::string> _defined_categories{};

  /// the current feature category. set by a codegen::category definition.
  static inline std::string _cur_category{};

  /// the current clang_format preference.
  static inline bool _cur_clang_format{true};

  struct instance {
    /// a copy of the exec_stack at the time of creation.
    /// ensures that no two macros defined in different places have the same ID.
    std::vector<instance*> const context;

    /// case-adjusted, prefixed name of the macro based on the execution stack.
    /// if this instance is a child, uses implname. else, uses apiname.
    /// no two instances may share the same id.
    std::string const id;
    //
    /// category of the macro.
    /// set by codegen::begin_category.
    std::string const category;

    /// name of the macro.
    std::string const name;

    /// the line number of the definition used for documentation and errors.
    detail::source_location const source_loc;

    /// whether or not the macro should be formatted.
    /// modified by clang_format = false in macro body;
    bool const clang_format;

    /// a vector of child instances. assigned by children.
    std::vector<instance*> children{};

    /// macro parameters, if this is a function.
    std::optional<std::string> params{};

    /// the description of the macro.
    /// appended to using docs << "line" statements within macro body.
    std::string description{};

    /// parameter docs that appear as an inline comment in the signature.
    std::optional<std::string> docparams{};

    /// expansion docs that appear as an inline comment in the signature.
    std::optional<std::string> xout{};

    /// return type docs that appear as an inline comment in the signature.
    std::optional<std::string> docreturn{};

    /// full macro definition. result of body execution.
    /// can only be assigned once.
    std::optional<std::string> definition{};

    /// macro unit tests.
    /// added using tests << "actual" = "expected" within macro body.
    /// may be preceded by docs<< to make it an example.
    ///
    /// tests are outputted as static assertions.
    std::vector<std::array<std::string, 2>> tests{};

    /// macro examples.
    /// added using docs << tests << "actual" = "expected" within macro body.
    ///
    /// examples are outputted both as doc lines and as static assertions.
    std::vector<std::array<std::string, 2>> examples{};
  };

  struct instance* _instance;

  [[nodiscard]] static instance* get_instance();
  [[nodiscard]] static instance* get_instance(std::string const&             name,
                                              detail::source_location const& loc);

  /// computes the definition of the macro (preprocessor-ready string).
  /// executes body. body may define additional macros or documentation/tests.
  ///
  /// only executed once per instance. results are saved for future invocations.
  template<detail::body_functor Body>
  void define(Body&& body);

  void update_instance(runtime_signature const& sig);

 public:
  friend class tests;
  friend class docs;
  friend class clang_format;
  template<detail::string_literal Name>
    requires(not Name.empty())
  friend class category;
  template<detail::string_literal Name>
    requires(not Name.empty())
  friend class end_category;
  friend class arg;
  friend class va;
  friend class pack;

  /// creates an output string of the README project synopsis.
  [[nodiscard]] static std::string synopsis();
  /// creates an output string of all preprocessor definitions and docs.
  [[nodiscard]] static std::string definitions();
  /// creates an output string of all static assertions.
  [[nodiscard]] static std::string assertions() noexcept;

  /// default construction. does not register anything.
  def_base();

  /// updates signature data.
  explicit def_base(runtime_signature const& sig, detail::source_location const& loc =
                                                      detail::source_location::current());

  /// updates signature data and defines.
  /// macros cannot be redefined.
  template<detail::body_functor Body>
  def_base(runtime_signature const& sig, Body&& body,
           detail::source_location const& loc = detail::source_location::current());

  /// defines the macro.
  /// macros cannot be redefined.
  template<detail::body_functor Body>
  def_base& operator=(Body&& body) noexcept;

  /// implicitly convertible to string using the macro id.
  [[nodiscard]] operator std::string const&() const noexcept;

  /// construct a call string using this id and any string-representable args.
  template<utl::string_representable... Args>
  [[nodiscard]] std::string operator()(Args&&... args) const;

  /// construct a call string using this id and any string-representable args.
  template<detail::forward_iterable_for<std::string const> Args>
  [[nodiscard]] std::string operator()(Args&& args) const;
};

template<std::convertible_to<std::string> T>
[[nodiscard]] std::string operator+(def_base const& lhs, T&& rhs) noexcept;

template<std::convertible_to<std::string> T>
[[nodiscard]] std::string operator+(T&& lhs, def_base const& rhs) noexcept;

/// stream object for adding docs to the current execution macro.
/// can be used as an indicator for tests to specify an example.
inline class docs {
 public:
  docs const& operator<<(std::string const& line) const; // add line to descr
} docs{};

/// stream object for adding tests to the current execution macro.
inline class tests {
  /// indicator for tests that must be documented examples
  class example_tag {
    std::string _expected;

   public:
    example_tag(std::string const& expected);

    [[nodiscard]] constexpr std::string const&
    expected() const noexcept {
      return _expected;
    }
  };

  friend tests::example_tag operator>>(std::string const& expected,
                                       class docs const&) noexcept;

  class expected {
    std::string _actual;

   public:
    explicit expected(std::string const& actual);
    tests const& operator=(std::string const& expected) const;
    tests const& operator=(example_tag const& expected) const;
  };

 public:
  expected operator<<(std::string const& actual) const;
} tests{};

inline class clang_format {
 public:
  // set clang_format for all macros defined after a clang_format= statement.
  // default is true. set true again after setting false within the same body.
  //
  // must be used within macro body (only disable formatting for impl macros)
  void operator=(bool on) const;
} clang_format{};

/// create an example.
/// ex:
///   tests << "actual" = "expected";         // tested, but not documented
///   tests << "actual" = "expected" >> docs; // both tested and documented
[[nodiscard]] tests::example_tag operator>>(std::string const& expected,
                                            class docs const&) noexcept;

/// sets the category for all newly constructed defs.
template<detail::string_literal Name>
  requires(not Name.empty())
class category {
 public:
  category() {
    if (not def_base::_cur_category.empty())
      throw std::runtime_error{"cannot start a new category before ending category "
                               + def_base::_cur_category};
    def_base::_cur_category = Name.c_str();
    def_base::_defined_categories.push_back(def_base::_cur_category);
  }
};

/// sets the category for all newly constructed defs.
template<detail::string_literal Name>
  requires(not Name.empty())
class end_category {
 public:
  end_category() {
    if (def_base::_cur_category != Name.c_str())
      throw std::runtime_error{std::string{"end_category <"} + Name.c_str()
                               + ">not preceeded by same category: "
                               + def_base::_cur_category};
    def_base::_cur_category = "";
  }
};

class arg {
  std::string         _value;
  def_base::instance* _parent;
  void                validate_context() const;

 public:
  arg(std::string const& value);

  [[nodiscard]] operator std::string const&() const;

  friend std::string operator+(arg const& l, std::string const& r);
  friend std::string operator+(std::string const& l, arg const& r);
};

[[nodiscard]] std::string operator+(arg const& l, std::string const& r);
[[nodiscard]] std::string operator+(std::string const& l, arg const& r);

class va {
  static inline std::string const _value{"__VA_ARGS__"};
  def_base::instance*             _parent;
  void                            validate_context() const;

 public:
  va(std::string const& value);

  [[nodiscard]] operator std::string const&() const;

  friend std::string operator+(va const& l, std::string const& r);
  friend std::string operator+(std::string const& l, va const& r);
};

[[nodiscard]] std::string operator+(va const& l, std::string const& r);
[[nodiscard]] std::string operator+(std::string const& l, va const& r);

class pack {
  std::vector<std::string> _value;
  def_base::instance*      _parent;
  void                     validate_context() const;

 public:
  using value_type = std::string;

  pack(std::vector<std::string> const& value);

  [[nodiscard]] operator std::vector<std::string> const&() const;

  [[nodiscard]] std::vector<std::string>::const_iterator         begin() const;
  [[nodiscard]] std::vector<std::string>::const_iterator         end() const;
  [[nodiscard]] std::vector<std::string>::const_reverse_iterator rbegin() const;
  [[nodiscard]] std::vector<std::string>::const_reverse_iterator rend() const;

  [[nodiscard]] std::string const& front() const;
  [[nodiscard]] std::string const& back() const;

  [[nodiscard]] std::string const& operator[](std::size_t idx) const;

  [[nodiscard]] std::size_t size() const;
};

template<detail::string_literal Sig = "">
  requires(Sig.empty() or signature_literal{Sig}.valid)
class def : public def_base {
 public:
  /// passes static signature to macro
  template<class = void>
    requires(not Sig.empty())
  def(detail::source_location const& loc = detail::source_location::current());

  /// defines using static signature.
  /// macros cannot be redefined.
  template<detail::body_functor Body>
    requires(not Sig.empty())
  def(Body&&                         body,
      detail::source_location const& loc = detail::source_location::current());

  /// default construction. does not register anything.
  template<class = void>
    requires(Sig.empty())
  def();

  /// updates signature data.
  template<class = void>
    requires(Sig.empty())
  explicit def(runtime_signature const&       sig,
               detail::source_location const& loc = detail::source_location::current());

  /// updates signature data and defines.
  /// macros cannot be redefined.
  template<detail::body_functor Body>
    requires(Sig.empty())
  def(runtime_signature const& sig, Body&& body,
      detail::source_location const& loc = detail::source_location::current());

  /// defines the macro.
  /// macros cannot be redefined.
  template<detail::body_functor Body>
  def& operator=(Body&& body) noexcept;
};

} // namespace codegen

#ifndef CODEGEN_CODEGEN_TCC_INCLUDED
#  include "codegen/codegen.tcc"
#endif

#endif

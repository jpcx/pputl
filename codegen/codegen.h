#ifndef CODEGEN_CODEGEN_H
#define CODEGEN_CODEGEN_H
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
#include <cmath>
#include <limits>
#include <list>
#include <optional>
#include <regex>
#include <span>
#include <type_traits>
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
constexpr std::uint8_t word_size = 2;

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
    "//    template specialization algorithms.                                     //\n"
    "//                                                                            //\n"
    "//    ABOUT                                                                   //\n"
    "//    -----                                                                   //\n"
    "//                                                                            //\n"
    "//    pputl is a powerful C++ preprocessor utilities library that provides    //\n"
    "//    many language constructs  including a type system, integers, errors,    //\n"
    "//    recursion, polymorphic objects, and common datastructures.              //\n"
    "//                                                                            //\n"
    "//    Speed, safety, and flexibility are its primary goals.                   //\n"
    "//                                                                            //\n"
    "//    Algorithms operate using an inline recursion technique that requires    //\n"
    "//    fewer expansions than mutually-recursive patterns and can be nested.    //\n"
    "//    Macros are provided for performing arbitrary, nestable recursion.       //\n"
    "//                                                                            //\n"
    "//    Functions verify their arguments and use type casting to ensure that    //\n"
    "//    useful error messages are generated as soon as an issue manifests.      //\n"
    "//                                                                            //\n"
    "//    USAGE                                                                   //\n"
    "//    -----                                                                   //\n"
    "//                                                                            //\n"
    "//    pputl is a standalone single-header library. include pputl.h to use.    //\n"
    "//    A C++20-compliant preprocessor is required.                             //\n"
    "//                                                                            //\n"
    "//    pputl  is completely generated and tested by a custom C++ framework.    //\n"
    "//    See the codegen/ folder for the full source.                            //\n"
    "//                                                                            //\n"
    "//    Various settings including word size and naming rules may be changed    //\n"
    "//    by modifying the head of codegen/codegen.h and running `make`.          //\n"
    "//                                                                            //\n"
    "//    The default build defines 12-bit words and an 8-bit size cap,  which    //\n"
    "//    complies with the following C++20 implementation limits [implimits]:    //\n"
    "//                                                                            //\n"
    "//     ‐ Macro identifiers simultaneously                                     //\n"
    "//       defined in one translation unit: [65536].                            //\n"
    "//     ‐ Parameters in one macro definition: [256].                           //\n"
    "//     ‐ Arguments in one macro invocation: [256].                            //\n"
    "//                                                                            //\n"
    "//    Exceeding these limits  is possible but depends on the preprocessor.    //\n"
    "//    The size cap is bounded by the maximum number of parameters, and the    //\n"
    "//    word size is directly proportional to the number of macros defined.     //\n"
    "//                                                                            //\n"
    "//    pputl has been tested with:                                             //\n"
    "//                                                                            //\n"
    "//      gcc   11.2.1                                                          //\n"
    "//      clang 13.0.0                                                          //\n"
    "//      MSVC  2022 (requires /Zc:preprocessor)                                //\n"
    "//                                                                            //\n"
    "//    Run `make test` to validate on your system.                             //\n"
    "//                                                                            //\n"
    "//    TYPES                                                                   //\n"
    "//    -----                                                                   //\n"
    "//                                                                            //\n"
    "//    pputl makes extensive use of duck-typing  for control flow and error    //\n"
    "//    management.  pputl types are essentially pairs of functions: one for    //\n"
    "//    traits identification and another for construction and assertion.       //\n"
    "//                                                                            //\n"
    "//    API functions are strictly documented using this type system. Inputs    //\n"
    "//    are validated by invoking the associated constructor or through some    //\n"
    "//    other form of inference. An argument is valid if it can be converted    //\n"
    "//    to (or interpreted as) its parameter without losing information.        //\n"
    "//                                                                            //\n"
    "//     any: any potentially-empty, individual argument in __VA_ARGS__         //\n"
    "//      ├╴none: an empty argument; an absence of pp-tokens                    //\n"
    "//      └╴some: a non-empty argument; a presence of pp-tokens                 //\n"
    "//         ├╴tup: a parenthesized item sequence [e.g. (a, b, c)]              //\n"
    "//         │  └╴pair: a two-tuple [e.g. (foo, bar)]                           //\n"
    "//         ├╴obj: a named, polymorphic, atom-addressable state container      //\n"
    "//         │  ├╴err: an error message container for invoking a failure        //\n"
    "//         │  └╴range: a sized tuple container                                //\n"
    "//         │     ├╴vec: a resizable, freely-mutable array                     //\n"
    "//         │     ├╴map: a mapping of equality-comparable keys to any          //\n"
    "//         │     └╴pq:  a priority queue                                      //\n"
    "//         │        ├╴queue: a FIFO queue                                     //\n"
    "//         │        └╴stack: a LIFO queue                                     //\n"
    "//         └╴atom: a non-empty argument that is not a tup or obj              //\n"
    "//            └╴sym: an explicitly defined, equality-comparable atom          //\n"
    "//               └╴word: a builtin C++ integral expression                    //\n"
    "//                  ├╴int: 0x800-4096|0x801-4096|...|0|...|2046|2047          //\n"
    "//                  │  ├╴bool: 0|1                                            //\n"
    "//                  │  └╴ofs:  an int within (-size_max, size_max)            //\n"
    "//                  └╴uint: 0u|1u|...|4094u|4095u                             //\n"
    "//                     └╴size: a uint within [0u, size_max]                   //\n"
    "//                                                                            //\n"
    "//    NOTES                                                                   //\n"
    "//    -----                                                                   //\n"
    "//                                                                            //\n"
    "//    pputl errors  execute an invalid preprocessor operation by using the    //\n"
    "//    concatenation operator (incorrectly) on a string error message.  All    //\n"
    "//    errors produced by pputl functions include the macro name, a textual    //\n"
    "//    description, and the primary expansion arguments.                       //\n"
    "//                                                                            //\n"
    "//    pputl API functions  are fully variadic and chainable,  meaning that    //\n"
    "//    their arguments can be populated using macro expansion results. Args    //\n"
    "//    must not grow, shrink, or change types after the primary expansion.     //\n"
    "//                                                                            //\n"
    "//    Negative ints are represented as valid C++ arithmetic expressions in    //\n"
    "//    order to avoid post-processing:  pputl arithmetic operations  always    //\n"
    "//    expand to values  that are usable in both preprocessor and C++ code.    //\n"
    "//    When constructing  preprocessor identifiers  from negative integers,   ///\n"
    "//    use int_name to extract the hex prefix before concatenation.          ////\n"
    "//                                                                         /////\n"
    "///////////////////////////////////////////////////////////////////////////// */"};

// derived config:

constexpr std::uint8_t bit_length = word_size * 4;

constexpr unsigned uint_max = ([] {
  unsigned res{1};
  for (unsigned i = 0; i < word_size * 4; ++i)
    res *= 2;
  return res - 1;
})();

constexpr unsigned size_max = cpp20_arglimit ? std::min(255u, uint_max) : uint_max;
constexpr int      int_max  = uint_max / 2;
constexpr int      int_min  = -((uint_max + 1) / 2);

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
[[nodiscard]] std::string
to_string(T&& v) {
  if constexpr (std::convertible_to<T, std::string>) {
    return std::string{std::forward<T>(v)};
  } else if constexpr (std::same_as<std::remove_cvref_t<T>, char>) {
    return {v};
  } else {
    return std::to_string(std::forward<T>(v));
  }
}

[[nodiscard]] std::string slice(std::string const& s, int end_ofs);
[[nodiscard]] std::string slice(std::string const& s, int begin_ofs, int end_ofs);
[[nodiscard]] std::vector<std::string> split(std::string const& target,
                                             std::regex const&  delim);

template<detail::forward_iterable_for<std::string const> Strs>
std::string
cat(Strs&& strs, std::string const& delim = "") {
  if (strs.begin() == strs.end())
    return {};
  std::string res{};
  auto        it = strs.begin();
  for (; it != std::prev(strs.end()); ++it)
    res += *it + delim;
  res += *it;
  return res;
}

template<detail::forward_iterable_for<std::string const> Strs>
  requires(not std::is_const_v<Strs>)
auto
replace(Strs&& strs, std::vector<std::pair<std::string, std::string>> const& repl) {
  for (auto&& v : strs) {
    for (auto&& [t, r] : repl) {
      if (v == t)
        v = r;
    }
  }
  return strs;
}

[[nodiscard]] std::string              alpha_base52(std::size_t num); // a-zA-Z
[[nodiscard]] std::vector<std::string> base10_seq(std::size_t size,
                                                  std::string begin = "0"); // 0-9
[[nodiscard]] std::vector<std::string>
                          alpha_base52_seq(std::size_t size,
                                           std::string begin = "a"); // a-zA-Z
[[nodiscard]] std::string prefix_lines(std::string const& prefix,
                                       std::string const& multiline);

template<std::unsigned_integral Int>
[[nodiscard]] std::vector<Int>
prime_factors(Int n) {
  if (n <= 3)
    return {};
  std::vector<Int> res{};
  Int              cur{n};
  while (cur % 2 == 0) {
    res.push_back(2);
    cur /= 2;
  }
  for (Int i = 3; i <= std::sqrt(cur); i += 2) {
    while (cur % i == 0) {
      res.push_back(i);
      cur /= i;
    }
  }
  if (cur > 2 and cur != n)
    res.push_back(cur);
  return res;
}

template<std::unsigned_integral Int>
[[nodiscard]] Int
next_le_pow2(Int n) {
  if (n == 0)
    throw std::runtime_error{"cannot find a power of two before zero"};
  Int res{1};
  for (Int cur = 1; cur <= n; res = cur, cur *= 2)
    ;
  return res;
}

template<std::unsigned_integral Int>
[[nodiscard]] Int
next_ge_pow2(Int n) {
  if (n == 0)
    return 1;
  auto res = next_le_pow2(n);
  if (res == n)
    return n;
  return res * 2;
}

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

} // namespace utl

[[nodiscard]] std::string apiname(std::string const& short_name); // uses conf::api_prefix
[[nodiscard]] std::string implname(std::string short_name); // uses conf::impl_prefix

namespace pp {

template<detail::forward_iterable_for<std::string const> Args>
[[nodiscard]] std::string
va_opt(Args&& args) {
  return "__VA_OPT__(" + utl::cat(std::forward<Args>(args), ", ") + ")";
}

template<utl::string_representable... Args>
[[nodiscard]] std::string
va_opt(Args&&... args) {
  return va_opt(std::vector<std::string>{utl::to_string(std::forward<Args>(args))...});
}

template<detail::forward_iterable_for<std::string const> Args>
[[nodiscard]] std::string
cat(Args&& args) {
  return utl::cat(std::forward<Args>(args), "##");
}

template<utl::string_representable... Args>
[[nodiscard]] std::string
cat(Args&&... args) {
  return cat(std::vector<std::string>{utl::to_string(std::forward<Args>(args))...});
}

template<bool Space = true, detail::forward_iterable_for<std::string const> Args>
[[nodiscard]] std::string
tup(Args&& args) {
  return "(" + utl::cat(std::forward<Args>(args), (Space ? ", " : ",")) + ")";
}

template<bool Space = true, utl::string_representable... Args>
[[nodiscard]] std::string
tup(Args&&... args) {
  return tup<Space>(
      std::vector<std::string>{utl::to_string(std::forward<Args>(args))...});
}

template<std::convertible_to<std::string>                Fn,
         detail::forward_iterable_for<std::string const> Args>
[[nodiscard]] std::string
call(Fn&& fn, Args&& args) {
  return std::string{std::forward<Fn>(fn)} + tup(std::forward<Args>(args));
}

template<utl::string_representable Fn, utl::string_representable... Args>
[[nodiscard]] std::string
call(Fn&& fn, Args&&... args) {
  return call(std::string{std::forward<Fn>(fn)},
              std::vector<std::string>{utl::to_string(std::forward<Args>(args))...});
}

template<detail::forward_iterable_for<std::string const> Args>
[[nodiscard]] std::string
str(Args&& args) {
  return "\"" + utl::cat(std::forward<Args>(args), ", ") + "\"";
}

template<utl::string_representable... Args>
[[nodiscard]] std::string
str(Args&&... args) {
  return str(std::vector<std::string>{utl::to_string(std::forward<Args>(args))...});
}

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

template<functor F, class T, std::same_as<std::string>... Args, std::size_t... Idxs>
[[nodiscard]] inline auto
iter_invoke_impl(F&& f, std::deque<T>& src_args, std::tuple<Args...>&& args = {},
                 std::index_sequence<Idxs...>&& = {}) {
  if constexpr (sizeof...(Idxs) == std::tuple_size_v<functor_params_type<F>>) {
    if (not src_args.empty())
      throw std::runtime_error{"too many args provided to iter_invoke"};
    return f(get<Idxs>(args)...);
  } else {
    if (src_args.empty())
      throw std::runtime_error{"too few args provided to iter_invoke"};
    std::string next = src_args.front();
    src_args.pop_front();
    return iter_invoke_impl(std::forward<F>(f), src_args,
                            std::tuple{get<Idxs>(args)..., next},
                            std::make_index_sequence<sizeof...(Idxs) + 1>{});
  }
}

template<functor F, std::ranges::forward_range Range>
[[nodiscard]] inline auto
iter_invoke(F&& f, Range const& args) {
  std::deque<std::ranges::range_value_t<Range>> src_args{};
  src_args.resize(args.size());
  std::ranges::copy(args, src_args.begin());
  return iter_invoke_impl(std::forward<F>(f), src_args);
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

  struct instance {
    /// a copy of the exec_stack at the time of creation.
    /// ensures that no two macros defined in different places have the same ID.
    std::vector<instance*> const context;

    /// case-adjusted, prefixed name of the macro based on the execution stack.
    /// if this instance is a child, uses implname. else, uses apiname.
    /// no two instances may share the same id.
    std::string const id;

    /// name of the macro.
    std::string const name;

    /// the line number of the definition used for documentation and errors.
    detail::source_location const source_loc;

    /// whether or not the macro should be formatted.
    /// modified by using the clang_format object in a macro body.
    bool clang_format{true};

    /// category of the macro.
    /// set by codegen::begin_category.
    std::string category{};

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
  [[nodiscard]] static instance* get_instance(std::string                    name,
                                              detail::source_location const& loc);

  /// computes the definition of the macro (preprocessor-ready string).
  /// executes body. body may define additional macros or documentation/tests.
  ///
  /// only executed once per instance. results are saved for future invocations.
  template<detail::body_functor Body>
  void
  define(Body&& body) {
    static std::regex argdelim{", *", std::regex_constants::optimize};
    static std::regex variadic{"[^.]*\\.{3}", std::regex_constants::optimize};

    if (_instance->definition)
      throw std::runtime_error{"macro " + _instance->id + " defined twice"};

    { // ensure instance directly follows parent (fix forward declaration misplacements)
      auto it = std::ranges::find(_instances, _instance, [](auto&& v) {
        return &v;
      });
      if (it == _instances.end())
        throw std::logic_error{"could not find instance " + _instance->id
                               + " in the instances list"};

      auto ins = _instances.end();
      if (not _instance->context.empty()) {
        ins = std::ranges::find(_instances, _instance->context.back(), [](auto&& v) {
          return &v;
        });
        if (ins != _instances.end()) {
          // register this child with its parent
          ins->children.push_back(_instance);
          ++ins;
        }
      }

      _instances.splice(ins, _instances, it);
    }

    // copy category from parent
    if (not _exec_stack.empty())
      _instance->category = _exec_stack.back()->category;

    std::string def{};
    std::string body_s{};

    std::optional<std::vector<std::string>> args{
        ([&]() -> std::optional<std::vector<std::string>> {
          if (_instance->params) {
            std::vector<std::string> args{};
            for (auto&& v : utl::split(*_instance->params, argdelim)) {
              if (not std::regex_match(v, variadic))
                args.push_back(v);
              else
                args.push_back("__VA_ARGS__");
            }
            return {args};
          } else {
            return {std::nullopt};
          }
        })()};

    if constexpr (std::tuple_size_v<detail::functor_params_type<Body>> == 0) {
      // may either be a nullary function or a non-function

      _exec_stack.push_back(_instance);
      body_s = body();
      _exec_stack.pop_back();
    } else if constexpr (detail::elements_same_as_any_of<
                             detail::functor_params_type<Body>, arg, va>) {
      // must be a function that accept individual arg|va arguments

      if (not args)
        throw std::runtime_error{"missing params for " + _instance->id};

      _exec_stack.push_back(_instance);
      try {
        body_s = detail::iter_invoke(std::forward<Body>(body), *args);
      } catch (std::runtime_error const& e) {
        throw std::runtime_error{
            "found invalid parameter/body configuration while executing " + _instance->id
            + ": " + e.what()};
      }
      _exec_stack.pop_back();
    } else {
      // must be a function that accepts pack arguments
      static_assert(
          std::tuple_size_v<detail::functor_params_type<Body>> == 1
          and std::same_as<std::tuple_element_t<0, detail::functor_params_type<Body>>,
                           pack>);

      if (not args)
        throw std::runtime_error{"missing params for " + _instance->id};

      _exec_stack.push_back(_instance);
      body_s = body(*args);
      _exec_stack.pop_back();
    }

    if (_instance->category.empty())
      throw std::runtime_error{"missing category for " + _instance->id};

    if (not _instance->description.empty() or not _instance->examples.empty()) {
      if (_instance->description.front() != '^')
        def += "\n";

      if (_exec_stack.empty()) { // only put category for top-level macros
        auto catstr = utl::ii << [&] {
          if (_instance->category.starts_with("impl.")) {
            auto catname =
                std::string{_instance->category.begin() + 5, _instance->category.end()};
            auto catless_name =
                std::regex_replace(_instance->name, std::regex{catname + "_?"}, "");
            return "[" + _instance->category + "." + catless_name + "]";
          } else {
            return "[" + _instance->category + "." + _instance->name + "]";
          }
        };
        def += "/// " + catstr + "\n";
        def += "/// " + utl::cat(std::vector<std::string>(catstr.size(), "-")) + "\n";
      }

      if (not _instance->description.empty()) {
        if (_instance->description.front() != '^')
          def += utl::prefix_lines("/// ", _instance->description) + "\n";
        else
          def += utl::prefix_lines("/// ", std::string{_instance->description.begin() + 1,
                                                       _instance->description.end()})
               + "\n";
      }
      if (not _instance->examples.empty()) {
        def += "///\n";
        std::size_t max_expectedsz{0};
        for (auto&& v : _instance->examples) {
          if (v[0].size() > max_expectedsz)
            max_expectedsz = v[0].size();
        }
        for (auto&& v : _instance->examples) {
          def += "/// " + v[0];
          for (std::size_t _ = v[0].size(); _ < max_expectedsz + 1; ++_)
            def += " ";
          if (v[1] != "") {
            def += "// " + v[1] + "\n";
          } else {
            def += "// <nothing>\n";
          }
        }
      }
    }

    def += "#define " + _instance->id;
    if (_instance->params) {
      if (_instance->docparams)
        def += "( /* " + *_instance->docparams + " */" + *_instance->params + ")";
      else
        def += "(" + *_instance->params + ")";
    }
    if (_instance->xout or _instance->docreturn) {
      def += " /* ";
      if (_instance->xout)
        def += "-<" + *_instance->xout + ">";
      def += "->";
      if (_instance->docreturn)
        def += " " + *_instance->docreturn;
      def += " */";
    }

    def += " ";

    def += body_s;

    _instance->definition = def;
  }

  void update_instance(runtime_signature const& sig);

 public:
  friend class tests;
  friend class docs;
  friend class clang_format;
  friend class category;
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
           detail::source_location const& loc = detail::source_location::current())
      : _instance{get_instance(sig.name, loc)} {
    update_instance(sig);
    define(std::forward<Body>(body));
  }

  /// defines the macro.
  /// macros cannot be redefined.
  template<detail::body_functor Body>
  def_base&
  operator=(Body&& body) noexcept {
    define(std::forward<Body>(body));
    return *this;
  }

  /// implicitly convertible to string using the macro id.
  [[nodiscard]] operator std::string const&() const noexcept;

  /// construct a call string using this id and any string-representable args.
  template<utl::string_representable... Args>
  [[nodiscard]] std::string
  operator()(Args&&... args) const {
    if (not _instance->params)
      throw std::runtime_error{"cannot call a non-function macro " + _instance->id};
    return pp::call(*this, std::forward<Args>(args)...);
  }

  /// construct a call string using this id and any string-representable args.
  template<detail::forward_iterable_for<std::string const> Args>
  [[nodiscard]] std::string
  operator()(Args&& args) const {
    if (not _instance->params)
      throw std::runtime_error{"cannot call a non-function macro " + _instance->id};
    return pp::call(*this, std::forward<Args>(args));
  }
};

template<std::convertible_to<std::string> T>
[[nodiscard]] std::string
operator+(def_base const& lhs, T&& rhs) noexcept {
  return std::string{lhs} + std::string{std::forward<T>(rhs)};
}

template<std::convertible_to<std::string> T>
[[nodiscard]] std::string
operator+(T&& lhs, def_base const& rhs) noexcept {
  return std::string{std::forward<T>(lhs)} + std::string{rhs};
}

/// stream object for adding docs to the current execution macro.
/// can be used as an indicator for tests to specify an example.
inline class docs {
 public:
  docs const& operator<<(std::string const& line) const; // add line to descr
} docs{};

/// sets current category.
/// must be called within def execution body.
inline class category {
 public:
  category const& operator=(std::string const& name) const;
} category{};

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
  // set clang-format preference.
  // must be used within macro body.
  void operator=(bool on) const;
} clang_format{};

/// create an example.
/// ex:
///   tests << "actual" == "expected";         // tested, but not documented
///   tests << "actual" == "expected" >> docs; // both tested and documented
[[nodiscard]] tests::example_tag operator>>(std::string const& expected,
                                            class docs const&) noexcept;

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
  def(detail::source_location const& loc = detail::source_location::current())
      : def_base{runtime_signature{Sig}, loc} {
  }

  /// defines using static signature.
  /// macros cannot be redefined.
  template<detail::body_functor Body>
    requires(not Sig.empty())
  def(Body&&                         body,
      detail::source_location const& loc = detail::source_location::current())
      : def_base{runtime_signature{Sig}, std::forward<Body>(body), loc} {
  }

  /// default construction. does not register anything.
  template<class = void>
    requires(Sig.empty())
  def() : def_base{} {
  }

  /// updates signature data.
  template<class = void>
    requires(Sig.empty())
  explicit def(runtime_signature const&       sig,
               detail::source_location const& loc = detail::source_location::current())
      : def_base{sig, loc} {
  }

  /// updates signature data and defines.
  /// macros cannot be redefined.
  template<detail::body_functor Body>
    requires(Sig.empty())
  def(runtime_signature const& sig, Body&& body,
      detail::source_location const& loc = detail::source_location::current())
      : def_base{sig, std::forward<Body>(body), loc} {
  }

  /// defines the macro.
  /// macros cannot be redefined.
  template<detail::body_functor Body>
  def&
  operator=(Body&& body) noexcept {
    def_base::operator=(std::forward<Body>(body));
    return *this;
  }
};

} // namespace codegen

#endif

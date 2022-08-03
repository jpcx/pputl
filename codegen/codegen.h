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

namespace codegen {

namespace conf {

// output file for all preprocessor definitions.
constexpr char const lib_output[]{"../pputl.h"};

// header guard for the lib output.
constexpr char const header_guard[]{"PPUTL_H_INCLUDED"};

// output file for all tests.
constexpr char const tests_output[]{"../tests.cc"};

// clang format style file (for final formatting).
constexpr char const clang_format_input[]{"../.clang-format"};

// prefix for feature macros.
constexpr char const api_prefix[]{"PTL_"};

// prefix for implementation macros.
// no trailing underscore by default to improve code completion.
constexpr char const impl_prefix[]{"PPUTL"};

// maximum value of a pputl unsigned integer.
// acts as an upper bound for the number of arguments to many functions.
constexpr unsigned uint_max = 1023;

// needed for documentation and tests
static_assert(uint_max >= 7);

// set the case of pputl names.
constexpr enum class name_case {
  screaming, // SCREAMING_CASE
  snake,     // snake_case
  camel,     // camelCase
  pascal,    // PascalCase
} name_case{name_case::screaming};

constexpr char const project_header[]{
    "/* /////////////////////////////////////////////////////////////////////////////\n"
    "//                          __    ___                                         //\n"
    "//                         /\\ \\__/\\_ \\                                        //\n"
    "//   _____   _____   __  __\\ \\ ,_\\//\\ \\                                       //\n"
    "//  /\\ '__`\\/\\ '__`\\/\\ \\/\\ \\\\ \\ \\/ \\ \\ \\                                      //\n"
    "//  \\ \\ \\_\\ \\ \\ \\_\\ \\ \\ \\_\\ \\\\ \\ \\_ \\_\\ \\_                                    //\n"
    "//   \\ \\ ,__/\\ \\ ,__/\\ \\____/ \\ \\__\\/\\____\\                                   //\n"
    "//    \\ \\ \\   \\ \\ \\   \\/___/   \\/__/\\/____/                                   //\n"
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
    "//    Caution:  macros should be used sparingly or not at all if possible.    //\n"
    "//    C++ has evolved to facilitate  countless  metaprogramming techniques    //\n"
    "//    that  should be  preferred in most cases,  as they are  predictable,    //\n"
    "//    type-safe, scoped, and easier to debug. pputl is primarily  intended    //\n"
    "//    for research purposes  and for various edge cases that still must be    //\n"
    "//    solved using text replacement,  such as certain  optimizations  that    //\n"
    "//    reduce the number of template specializations.                          //\n"
    "//                                                                            //\n"
    "//    ABOUT                                                                   //\n"
    "//    -----                                                                   //\n"
    "//                                                                            //\n"
    "//    pputl is a powerful C++ preprocessor utilities library that provides    //\n"
    "//    many high-level programming constructs including unsigned arithmetic    //\n"
    "//    and comparisons,  logic,  control flow,  generation, transformation,    //\n"
    "//    reduction, function binding, overloading, and range selection. pputl    //\n"
    "//    is  completely  generated  by a  custom  framework  that  transforms    //\n"
    "//    shorthand signatures and library invocations into macros, tests, and    //\n"
    "//    documentation.                                                          //\n"
    "//                                                                            //\n"
    "//    pputl makes use of recursion to the maximum extent possible to limit    //\n"
    "//    the number of macro definitions for the configured unsigned maximum.    //\n"
    "//    See range.split and algo.reduce for useful examples of the two types    //\n"
    "//    of recursive calls supported by this library.                           //\n"
    "//                                                                            //\n"
    "//    pputl requires __VA_ARGS__, __VA_OPT__, and empty variadic arguments    //\n"
    "//    support (which are guaranteed by C++20)  but has no dependencies and    //\n"
    "//    is single-header.                                                       //\n"
    "//                                                                            //\n"
    "//    USAGE                                                                   //\n"
    "//    -----                                                                   //\n"
    "//    Copy pputl.h and include. The default build uses a 10-bit uint range    //\n"
    "//    for  arithmetic  and  comparisons.  Integers  overflow and underflow    //\n"
    "//    according to  standard unsigned rules.  Variadic argument sizes  are    //\n"
    "//    usually capped by the uint max. Modify the head of codegen/codegen.h    //\n"
    "//    and run `make` to set custom integer limits and naming preferences.     //\n"
    "//                                                                            //\n"
    "//    GUIDELINES                                                              //\n"
    "//    ----------                                                              //\n"
    "//                                                                            //\n"
    "//    Non-nullary API functions are fully variadic and chainable such that    //\n"
    "//    the outputs of one may be used as inputs to another.                    //\n"
    "//                                                                            //\n"
    "//    Tuples are used only when necessary;  most functions that operate on    //\n"
    "//    data ranges both input and output a variadic argument list. Creating    //\n"
    "//    a tuple is trivial compared to extracting tuple items.                  //\n"
    "//                                                                            //\n"
    "//    Functions that parse or transform generic data are designed to allow    //\n"
    "//    for algorithmic manipulation of preprocessor syntax. This means that    //\n"
    "//    expansions  are applied  consistently  to all input elements and are    //\n"
    "//    known,  documented  and tested.  This is necessary for several pputl    //\n"
    "//    features including algo.recur and algo.reduce that use deferred left    //\n"
    "//    and right parens to construct recursive statements.                     //\n"
    "//                                                                            //\n"
    "//    API functions are documented with the following shorthand syntax:       //\n"
    "//                                                                            //\n"
    "//      name({in: type}) [-<xct>]-> {out[: type]}                             //\n"
    "//                                                                            //\n"
    "//      name: name of the feature                                             //\n"
    "//      in:   any number of input parameter names                             //\n"
    "//      type: a shorthand type description                                    //\n"
    "//      xct:  number of secondary expansions applied to generic inputs.       //\n"
    "//            any generic args deferred past xct will not be expanded.        //\n"
    "//      out:  name of an output; may be an input                              //\n"
    "//            name with a shorthand transformation.                           //\n"
    "//                                                                            //\n"
    "//    Generic types are documented with \"any\" and are the only targets for    //\n"
    "//    expansion documentation. Input parameter separations, variadic sizes    //\n"
    "//    and non-generic types (e.g. uint) must be fully expressed during the    //\n"
    "//    primary expansion.                                                      //\n"
    "//                                                                            //\n"
    "//    Library example [range.split]:                                          //\n"
    "//                                                                            //\n"
    "//      split(n: uint, args: any...) -<1>-> head: (...args[:n]),              //\n"
    "//                                          tail: (...args[n-size:]),         //\n"
    "//                                          type: bool                        //\n"
    "//                                                                            //\n"
    "//    TESTING                                                                 //\n"
    "//    -------                                                                 //\n"
    "//    pputl  is statically tested by the build system.  Run `make test` to   ///\n"
    "//    validate the library or open tests.cc in an LSP-enabled editor.       ////\n"
    "//                                                                         /////\n"
    "///////////////////////////////////////////////////////////////////////////// */"};

} // namespace conf

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
concept forward_iterable_for =
    (std::ranges::forward_range<R>)and(std::convertible_to<std::ranges::range_reference_t<R>, V&>);
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

[[nodiscard]] std::string              slice(std::string const& s, int end_ofs);
[[nodiscard]] std::string              slice(std::string const& s, int begin_ofs, int end_ofs);
[[nodiscard]] std::vector<std::string> split(std::string const& target, std::regex const& delim);
template<detail::forward_iterable_for<std::string const> Strs>
[[nodiscard]] std::string              cat(Strs&& strs, std::string const& delim = "");
[[nodiscard]] std::string              alpha_base52(std::size_t num);                               // a-zA-Z
[[nodiscard]] std::vector<std::string> base10_seq(std::size_t size, std::string begin = "0");       // 0-9
[[nodiscard]] std::vector<std::string> alpha_base52_seq(std::size_t size, std::string begin = "a"); // a-zA-Z
[[nodiscard]] std::string              prefix_lines(std::string const& prefix, std::string const& multiline);
template<std::unsigned_integral Int>
[[nodiscard]] std::vector<Int> prime_factors(Int n);
template<std::unsigned_integral Int>
[[nodiscard]] inline Int largest_pow2_up_to(Int n);

template<class T>
using storage = std::aligned_storage_t<sizeof(std::remove_cvref_t<T>), alignof(std::remove_cvref_t<T>)>;

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

#define NIFTY_DECL(extern_ref_name)                                                                \
  extern codegen::utl::nifty<decltype(extern_ref_name)> internal_nifty##extern_ref_name##storage_; \
  static struct internal_nifty##extern_ref_name##decl_ {                                           \
    internal_nifty##extern_ref_name##decl_();                                                      \
    ~internal_nifty##extern_ref_name##decl_();                                                     \
  } internal_nifty##extern_ref_name##decl_

#define NIFTY_DEF(extern_ref_name, ...)                                                      \
  internal_nifty##extern_ref_name##storage_;                                                 \
  internal_nifty##extern_ref_name##decl_::internal_nifty##extern_ref_name##decl_() {         \
    internal_nifty##extern_ref_name##storage_.ref(__VA_ARGS__);                              \
  }                                                                                          \
  internal_nifty##extern_ref_name##decl_::~internal_nifty##extern_ref_name##decl_() {        \
    internal_nifty##extern_ref_name##storage_.unref();                                       \
  }                                                                                          \
  codegen::utl::nifty<decltype(extern_ref_name)> internal_nifty##extern_ref_name##storage_ { \
  }

} // namespace utl

[[nodiscard]] std::string apiname(std::string const& short_name);  // uses conf::api_prefix
[[nodiscard]] std::string implname(std::string const& short_name); // uses conf::impl_prefix

namespace pp {
template<detail::forward_iterable_for<std::string const> Args>
[[nodiscard]] std::string va_opt(Args&& args);
template<detail::forward_iterable_for<std::string const> Args>
[[nodiscard]] std::string cat(Args&& args);
template<detail::forward_iterable_for<std::string const> Args>
[[nodiscard]] std::string tup(Args&& args);
template<std::convertible_to<std::string> Fn, detail::forward_iterable_for<std::string const> Args>
[[nodiscard]] std::string call(Fn&& fn, Args&& args);

template<utl::string_representable... Args>
[[nodiscard]] std::string va_opt(Args&&... args);
template<utl::string_representable... Args>
[[nodiscard]] std::string cat(Args&&... args);
template<utl::string_representable... Args>
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
using functor_params_type = typename functor_traits<decltype(std::function{std::declval<F>()})>::params_type;

/// the return type of a functor.
template<functor F>
using functor_return_type = typename functor_traits<decltype(std::function{std::declval<F>()})>::return_type;

template<class T, class FirstAllowed, class... RestAllowed> // enforce at least one comparison
concept same_as_any_of = std::same_as<T, FirstAllowed> or(std::same_as<T, RestAllowed> || ...);

template<class Tup, class FirstAllowed, class... RestAllowed, std::size_t... Idx>
[[nodiscard]] inline constexpr bool
elements_same_as_any_of_impl(std::index_sequence<Idx...>&&) noexcept {
  return (same_as_any_of<std::tuple_element_t<Idx, Tup>, FirstAllowed, RestAllowed...> && ...);
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

/// describes a functor that returns string and takes any number of string parameters or a vector<string>.
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
  return (ch >= '0' and ch <= '9') or (ch >= 'A' and ch <= 'Z') or (ch >= 'a' and ch <= 'z');
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
  constexpr span_literal(Rng&& rng) : _begin{&*std::ranges::begin(rng)}, _end{&*std::ranges::end(rng)} {
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
                           return detail::is_alphanum(ch) or ch == '_' or ch == '$';
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

    return span_literal{detail::trim_begin(begin, params_end), detail::trim_end(begin, params_end)};
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

    return span_literal{detail::trim_begin(begin + 1, end), detail::trim_end(begin + 1, end)};
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
      return span_literal{begin, detail::trim_end(begin, end)}; // -> end is last nonspace before closing '>'
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
      if (it + 1 == data->end() or it + 2 == data->end() or *(it + 1) != '-' or *(it + 2) != '>')
        return {};

      begin = it + 2; // set to last '>' before docreturn
    }

    // find first nonspace after arrow-end '>'
    begin = detail::trim_begin(begin + 1, data->end());

    // return docreturn
    return span_literal{begin, detail::trim_end(begin, data->end())};
  };

  // invalid if any member returned nullopt
  bool const valid{data and name and params_decl and params and docparams and xout and docreturn};
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

  std::string const data{signature_literal::data
                             ? std::string{signature_literal::data->begin(), signature_literal::data->end()}
                             : ""};
  std::string const name{signature_literal::name
                             ? std::string{signature_literal::name->begin(), signature_literal::name->end()}
                             : ""};
  bool const        is_fn{signature_literal::params_decl and not signature_literal::params_decl->empty()};
  std::string const params{signature_literal::params ? std::string{signature_literal::params->begin(),
                                                                   signature_literal::params->end()}
                                                     : ""};
  std::string const docparams{
      signature_literal::docparams
          ? std::string{signature_literal::docparams->begin(), signature_literal::docparams->end()}
          : ""};
  std::string const xout{signature_literal::xout
                             ? std::string{signature_literal::xout->begin(), signature_literal::xout->end()}
                             : ""};
  std::string const docreturn{
      signature_literal::docreturn
          ? std::string{signature_literal::docreturn->begin(), signature_literal::docreturn->end()}
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
  [[nodiscard]] static instance* get_instance(std::string const& name, detail::source_location const& loc);

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
  template<detail::string_literal Name>
    requires(not Name.empty())
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
  explicit def_base(runtime_signature const&       sig,
                    detail::source_location const& loc = detail::source_location::current());

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

  friend tests::example_tag operator>>(std::string const& expected, class docs const&) noexcept;

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

/// create an example.
/// ex:
///   tests << "actual" = "expected";         // tested, but not documented
///   tests << "actual" = "expected" >> docs; // both tested and documented
[[nodiscard]] tests::example_tag operator>>(std::string const& expected, class docs const&) noexcept;

/// sets the category for all newly constructed defs.
template<detail::string_literal Name>
  requires(not Name.empty())
class category {
 public:
  category() {
    def_base::_cur_category = Name.c_str();
    def_base::_defined_categories.push_back(def_base::_cur_category);
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

  [[nodiscard]] std::vector<std::string>::const_iterator begin() const;
  [[nodiscard]] std::vector<std::string>::const_iterator end() const;

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
  def(Body&& body, detail::source_location const& loc = detail::source_location::current());

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

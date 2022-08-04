#ifndef CODEGEN_CODEGEN_TCC_INCLUDED
#define CODEGEN_CODEGEN_TCC_INCLUDED
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

#ifndef CODEGEN_H_INCLUDED
#  include "codegen.h"
#endif

#include <cmath>

namespace codegen {

namespace utl {

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

template<detail::forward_iterable_for<std::string const> Strs>
std::string
cat(Strs&& strs, std::string const& delim) {
  if (strs.begin() == strs.end())
    return {};
  std::string res{};
  auto        it = strs.begin();
  for (; it != std::prev(strs.end()); ++it)
    res += *it + delim;
  res += *it;
  return res;
}

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
largest_pow2_up_to(Int n) {
  if (n == 0)
    throw std::runtime_error{"cannot find a power of two before zero"};
  Int res{1};
  for (Int cur = 1; cur <= n; res = cur, cur *= 2)
    ;
  return res;
}

} // namespace utl

namespace pp {
template<detail::forward_iterable_for<std::string const> Args>
[[nodiscard]] std::string
va_opt(Args&& args) {
  return "__VA_OPT__(" + utl::cat(std::forward<Args>(args), ", ") + ")";
}

template<detail::forward_iterable_for<std::string const> Args>
[[nodiscard]] std::string
cat(Args&& args) {
  return utl::cat(std::forward<Args>(args), "##");
}

template<detail::forward_iterable_for<std::string const> Args>
[[nodiscard]] std::string
tup(Args&& args) {
  return "(" + utl::cat(std::forward<Args>(args), ", ") + ")";
}

template<detail::forward_iterable_for<std::string const> Args>
[[nodiscard]] std::string
str(Args&& args) {
  return "\"" + utl::cat(std::forward<Args>(args), ", ") + "\"";
}

template<std::convertible_to<std::string> Fn, detail::forward_iterable_for<std::string const> Args>
[[nodiscard]] std::string
call(Fn&& fn, Args&& args) {
  return std::string{std::forward<Fn>(fn)} + tup(std::forward<Args>(args));
}

template<utl::string_representable... Args>
[[nodiscard]] std::string
va_opt(Args&&... args) {
  return va_opt(std::vector<std::string>{utl::to_string(std::forward<Args>(args))...});
}

template<utl::string_representable... Args>
[[nodiscard]] std::string
cat(Args&&... args) {
  return cat(std::vector<std::string>{utl::to_string(std::forward<Args>(args))...});
}

template<utl::string_representable... Args>
[[nodiscard]] std::string
tup(Args&&... args) {
  return tup(std::vector<std::string>{utl::to_string(std::forward<Args>(args))...});
}

template<utl::string_representable... Args>
[[nodiscard]] std::string
str(Args&&... args) {
  return str(std::vector<std::string>{utl::to_string(std::forward<Args>(args))...});
}

template<utl::string_representable Fn, utl::string_representable... Args>
[[nodiscard]] std::string
call(Fn&& fn, Args&&... args) {
  return call(std::string{std::forward<Fn>(fn)},
              std::vector<std::string>{utl::to_string(std::forward<Args>(args))...});
}
} // namespace pp

namespace detail {

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
    return iter_invoke_impl(std::forward<F>(f), src_args, std::tuple{get<Idxs>(args)..., next},
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

template<detail::body_functor Body>
void
def_base::define(Body&& body) {
  static std::regex argdelim{", *", std::regex_constants::optimize};
  static std::regex variadic{"[^.]*\\.{3}", std::regex_constants::optimize};

  if (_instance->definition)
    throw std::runtime_error{"macro " + _instance->id + " defined twice"};

  { // ensure instance directly follows parent (fix forward declaration misplacements)
    auto it = std::ranges::find(_instances, _instance, [](auto&& v) {
      return &v;
    });
    if (it == _instances.end())
      throw std::logic_error{"could not find instance " + _instance->id + " in the instances list"};

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

  std::string def{};
  std::string body_s{};

  std::optional<std::vector<std::string>> args{([&]() -> std::optional<std::vector<std::string>> {
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
  } else if constexpr (detail::elements_same_as_any_of<detail::functor_params_type<Body>, arg,
                                                       va>) {
    // must be a function that accept individual arg|va arguments

    if (not args)
      throw std::runtime_error{"missing params for " + _instance->id};

    _exec_stack.push_back(_instance);
    try {
      body_s = detail::iter_invoke(std::forward<Body>(body), *args);
    } catch (std::runtime_error const& e) {
      throw std::runtime_error{"found invalid parameter/body configuration while executing "
                               + _instance->id + ": " + e.what()};
    }
    _exec_stack.pop_back();
  } else {
    // must be a function that accepts pack arguments
    static_assert(
        std::tuple_size_v<detail::functor_params_type<Body>> == 1
        and std::same_as<std::tuple_element_t<0, detail::functor_params_type<Body>>, pack>);

    if (not args)
      throw std::runtime_error{"missing params for " + _instance->id};

    _exec_stack.push_back(_instance);
    body_s = body(*args);
    _exec_stack.pop_back();
  }

  if (_instance->category.empty())
    throw std::runtime_error{"missing category for " + _instance->id};

  if (not _instance->description.empty() or not _instance->examples.empty()) {
    def += "\n";

    if (_exec_stack.empty()) { // only put category for top-level macros
      auto catstr = "[" + _instance->category + "." + _instance->name + "]";
      def += "/// " + catstr + "\n";
      def += "/// " + utl::cat(std::vector<std::string>(catstr.size(), "-")) + "\n";
    }

    if (not _instance->description.empty())
      def += utl::prefix_lines("/// ", _instance->description) + "\n";
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

template<detail::body_functor Body>
def_base::def_base(runtime_signature const& sig, Body&& body, detail::source_location const& loc)
    : _instance{get_instance(sig.name, loc)} {
  update_instance(sig);
  define(std::forward<Body>(body));
}

template<detail::body_functor Body>
def_base&
def_base::operator=(Body&& body) noexcept {
  define(std::forward<Body>(body));
  return *this;
}

template<utl::string_representable... Args>
[[nodiscard]] std::string
def_base::operator()(Args&&... args) const {
  if (not _instance->params)
    throw std::runtime_error{"cannot call a non-function macro " + _instance->id};
  return pp::call(*this, std::forward<Args>(args)...);
}

template<detail::forward_iterable_for<std::string const> Args>
[[nodiscard]] std::string
def_base::operator()(Args&& args) const {
  if (not _instance->params)
    throw std::runtime_error{"cannot call a non-function macro " + _instance->id};
  return pp::call(*this, std::forward<Args>(args));
}

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

template<detail::string_literal Sig>
  requires(Sig.empty() or signature_literal{Sig}.valid)
template<class>
  requires(not Sig.empty())
def<Sig>::def(detail::source_location const& loc) : def_base{runtime_signature{Sig}, loc} {
}

template<detail::string_literal Sig>
  requires(Sig.empty() or signature_literal{Sig}.valid)
template<detail::body_functor Body>
  requires(not Sig.empty())
def<Sig>::def(Body&& body, detail::source_location const& loc)
    : def_base{runtime_signature{Sig}, std::forward<Body>(body), loc} {
}

template<detail::string_literal Sig>
  requires(Sig.empty() or signature_literal{Sig}.valid)
template<class>
  requires(Sig.empty())
def<Sig>::def() : def_base{} {
}

template<detail::string_literal Sig>
  requires(Sig.empty() or signature_literal{Sig}.valid)
template<class>
  requires(Sig.empty())
def<Sig>::def(runtime_signature const& sig, detail::source_location const& loc)
    : def_base{sig, loc} {
}

template<detail::string_literal Sig>
  requires(Sig.empty() or signature_literal{Sig}.valid)
template<detail::body_functor Body>
  requires(Sig.empty())
def<Sig>::def(runtime_signature const& sig, Body&& body, detail::source_location const& loc)
    : def_base{sig, std::forward<Body>(body), loc} {
}

template<detail::string_literal Sig>
  requires(Sig.empty() or signature_literal{Sig}.valid)
template<detail::body_functor Body>
def<Sig>&
def<Sig>::operator=(Body&& body) noexcept {
  def_base::operator=(std::forward<Body>(body));
  return *this;
}

} // namespace codegen

#endif

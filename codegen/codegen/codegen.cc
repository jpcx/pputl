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

#include "codegen.h"

#include <unordered_set>

namespace codegen {

namespace utl {

std::string
slice(std::string const& s, int end_ofs) {
  std::string res{};
  res.resize(s.size() + end_ofs);
  std::ranges::copy(s.begin(), std::prev(s.end(), -end_ofs), res.begin());
  return res;
}

std::string
slice(std::string const& s, int begin_ofs, int end_ofs) {
  std::string res{};
  if (begin_ofs < 0)
    begin_ofs = s.size() + begin_ofs;
  res.resize(s.size() + end_ofs - begin_ofs);
  std::ranges::copy(std::next(s.begin(), begin_ofs), std::prev(s.end(), -end_ofs), res.begin());
  return res;
}

std::vector<std::string>
split(std::string const& target, std::regex const& delim) {
  std::vector<std::string> res{};
  std::string              cur{};
  auto                     begin = target.begin();
  for (auto it = std::sregex_iterator{target.begin(), target.end(), delim};
       it != std::sregex_iterator{}; ++it) {
    if (it->size() > 1)
      throw std::runtime_error{"split regex cannot use match groups"};
    std::copy(begin, it->begin()->first, std::back_inserter(cur));
    res.push_back(cur);
    cur   = {};
    begin = it->begin()->second;
  }
  std::copy(begin, target.end(), std::back_inserter(cur));
  res.push_back(cur);
  return res;
}

std::string
alpha_base52(std::size_t num) {
  static constexpr char dict[]{"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
  std::string           res{};
  do {
    res = dict[num % (sizeof dict - 1)] + res;
    num /= (sizeof dict - 1);
  } while (num > 0);
  return res;
}

std::vector<std::string>
base10_seq(std::size_t size, std::string begin) {
  if (size == 0)
    return {};

  if (begin.empty())
    throw std::runtime_error{"cannot use base10_seq with an empty begin"};
  for (auto&& v : begin)
    if (not(v >= '0' and v <= '9'))
      throw std::runtime_error{"cannot use base10_seq with a non-base10 begin"};

  std::vector<std::string> res{size};

  // find position of begin string
  std::size_t pos{};
  for (std::size_t i = 0; i < begin.size() - 1; ++i) {
    pos += begin[i] - '0';
    pos *= 10;
  }
  pos += begin.back() - '0';

  for (std::size_t i = pos; i < pos + size; ++i)
    res[i - pos] = utl::to_string(i);

  return res;
}

std::vector<std::string>
alpha_base52_seq(std::size_t size, std::string begin) {
  if (size == 0)
    return {};

  if (begin.empty())
    throw std::runtime_error{"cannot use alpha_base52_seq with an empty begin"};
  for (auto&& v : begin)
    if (not(v >= 'a' and v <= 'z') and not(v >= 'A' and v <= 'Z'))
      throw std::runtime_error{"cannot use alpha_base52_seq with a non-alpha-base52 begin"};

  std::vector<std::string> res{size};

  // find position of begin string
  std::size_t pos{};
  for (std::size_t i = 0; i < begin.size() - 1; ++i) {
    pos += begin[i] - 'a';
    pos *= 52;
  }
  pos += begin.back() - 'a';

  for (std::size_t i = pos; i < pos + size; ++i)
    res[i - pos] = alpha_base52(i);

  return res;
}

/// adds a prefix to each line in a multiline string.
[[nodiscard]] std::string
prefix_lines(std::string const& prefix, std::string const& multiline) {
  std::string res{prefix};
  for (auto&& v : multiline) {
    res.push_back(v);
    if (v == '\n')
      res += prefix;
  }
  return res;
}

} // namespace utl

namespace impl {
/// true if a char is a-z.
[[nodiscard]] inline constexpr bool
is_lower(char ch) {
  return (ch >= 'a' and ch <= 'z');
}

/// true if a char is A-Z.
[[nodiscard]] inline constexpr bool
is_upper(char ch) {
  return (ch >= 'A' and ch <= 'Z');
}

/// true if a char is a-zA-Z.
[[nodiscard]] inline constexpr bool
is_alpha(char ch) {
  return is_lower(ch) or is_upper(ch);
}

/// converts a string to SCREAMING_CASE.
/// any boundaries where an upper case char is preceeded
/// by a lower case one will have an underscore inserted.
[[nodiscard]] inline std::string
screaming_case(std::string const& input) {
  if (input.empty())
    return {};

  std::string out{};

  for (std::size_t i = 0; i < input.size(); ++i) {
    if (input[i] != '_') {
      out += toupper(input[i]);

      if (i > 0 and isupper(input[i]) and islower(input[i - 1]))
        out += '_';

    } else {
      out += '_';
    }
  }
  return out;
}

/// converts a string to snake_case.
/// any boundaries where an upper case char is preceeded
/// by a lower case one will have an underscore inserted.
[[nodiscard]] inline std::string
snake_case(std::string const& input) {
  if (input.empty())
    return {};

  std::string out{};

  for (std::size_t i = 0; i < input.size(); ++i) {
    if (input[i] != '_') {
      out += tolower(input[i]);

      if (i > 0 and isupper(input[i]) and islower(input[i - 1]))
        out += '_';

    } else {
      out += '_';
    }
  }
  return out;
}

/// converts a string to camelCase.
/// any underscore boundaries will be converted to case-based boundaries.
[[nodiscard]] inline std::string
camel_case(std::string const& input) {
  if (input.empty())
    return {};

  std::string out{};

  std::size_t i = 0;
  for (; i < input.size() and not is_alpha(input[i]); ++i)
    out += input[i];

  for (; i < input.size(); ++i) {
    if (input[i] == '_') {
      for (; i < input.size() and input[i] == '_'; ++i)
        ;
      if (i < input.size())
        out += toupper(input[i]);
    } else {
      out += tolower(input[i]);
    }
  }
  return out;
}

/// converts a string to PamelCase.
/// any underscore boundaries will be converted to case-based boundaries.
[[nodiscard]] inline std::string
pascal_case(std::string const& input) {
  if (input.empty())
    return {};

  std::string out{};

  std::size_t i = 0;
  for (; i < input.size() and not is_alpha(input[i]); ++i)
    out += input[i];

  if (i < input.size()) {
    out.push_back(toupper(input[i++]));

    for (; i < input.size(); ++i) {
      if (input[i] == '_') {
        for (; i < input.size() and input[i] == '_'; ++i)
          ;
        if (i < input.size())
          out += toupper(input[i]);
      } else {
        out += tolower(input[i]);
      }
    }
  }
  return out;
}

inline std::string
set_case(std::string const& input) {
  static_assert(
      conf::name_case == conf::name_case::screaming or conf::name_case == conf::name_case::snake
      or conf::name_case == conf::name_case::camel or conf::name_case == conf::name_case::pascal);
  if constexpr (conf::name_case == conf::name_case::screaming) {
    return screaming_case(input);
  } else if constexpr (conf::name_case == conf::name_case::snake) {
    return snake_case(input);
  } else if constexpr (conf::name_case == conf::name_case::camel) {
    return camel_case(input);
  } else {
    return pascal_case(input);
  }
}

inline std::string
libname(std::string const& short_name, std::string const& prefix) {
  if constexpr (conf::name_case == conf::name_case::camel
                or conf::name_case == conf::name_case::pascal) {
    if (not prefix.empty() and prefix.back() != '_') {
      return set_case(prefix + "_" + short_name); // enforce caps
    } else
      return set_case(prefix + short_name);
  } else {
    return set_case(prefix + short_name);
  }
}
} // namespace impl

// auto dunder = std::ranges::find(short_name, '_');
// while (dunder != short_name.end() and *(dunder + 1) != '_')
//   dunder = std::ranges::find(dunder + 1, short_name.end(), '_');
// std::string pre_esc{short_name.begin(), dunder};
// std::string post_esc{dunder + (pre_esc.size() < short_name.size()), short_name.end()};

std::string
apiname(std::string const& short_name) {
  std::string pre_esc{short_name.begin(), std::ranges::find(short_name, '\\')};
  std::string post_esc{
      std::next(short_name.begin(), pre_esc.size() + (pre_esc.size() < short_name.size())),
      short_name.end()};
  return impl::libname(pre_esc, conf::api_prefix) + post_esc;
}

std::string
implname(std::string short_name) {
  std::string pre_esc{short_name.begin(), std::ranges::find(short_name, '\\')};
  std::string post_esc{
      std::next(short_name.begin(), pre_esc.size() + (pre_esc.size() < short_name.size())),
      short_name.end()};
  static std::array<std::pair<std::regex, std::string>,
                    (sizeof conf::impl_shortnames) / (sizeof(std::array<char const*, 2>))>
      repls = utl::ii << [&]() -> decltype(repls) {
    decltype(repls) res{};
    for (std::size_t i = 0; i < res.size(); ++i)
      res[i] = {std::regex{conf::impl_shortnames[i][0], std::regex_constants::optimize},
                conf::impl_shortnames[i][1]};
    return res;
  };

  for (auto&& [r, s] : repls)
    pre_esc = std::regex_replace(pre_esc, r, s);

  return impl::libname(pre_esc, conf::impl_prefix) + post_esc;
}

namespace impl {
namespace tests {

template<std::ranges::forward_range L, std::ranges::forward_range R>
  requires(
      std::ranges::sized_range<L>and std::ranges::sized_range<R>and std::equality_comparable_with<
          std::ranges::range_reference_t<L>, std::ranges::range_reference_t<R>>)
[[nodiscard]] inline constexpr bool
range_eq(L&& l, R&& r) noexcept {
  using namespace std::ranges;
  if (size(l) != size(r))
    return false;
  auto lit = begin(l);
  auto rit = end(r);
  for (; lit != end(l) and rit != end(r); ++lit, ++rit) {
    if (*lit != *rit)
      return false;
  }
  return true;
}

#define ASSERT(...)                 \
  if constexpr (not(__VA_ARGS__)) { \
    return false;                   \
    static_assert(__VA_ARGS__);     \
  }                                 \
  do {                              \
  } while (0)

#define IF_PASS(...)                \
  if constexpr (not(__VA_ARGS__)) { \
    return false;                   \
    static_assert(__VA_ARGS__);     \
  } else

template<detail::string_literal Sig, detail::string_literal Name,
         detail::string_literal ParamsDecl = "", detail::string_literal Params = "",
         detail::string_literal DocParams = "", detail::string_literal XOut = "",
         detail::string_literal DocReturn = "", bool Valid = true>
inline constexpr bool test_signature = ([] {
  constexpr auto sig = signature_literal{Sig};

  ASSERT(sig.valid == Valid);

  if constexpr (Valid) {
    ASSERT(sig.data);
    ASSERT(sig.name);
    ASSERT(sig.params_decl);
    ASSERT(sig.params);
    ASSERT(sig.docparams);
    ASSERT(sig.xout);
    ASSERT(sig.docreturn);

    ASSERT(range_eq(*sig.name, Name));
    ASSERT(range_eq(*sig.params_decl, ParamsDecl));
    ASSERT(range_eq(*sig.params, Params));
    ASSERT(range_eq(*sig.docparams, DocParams));
    ASSERT(range_eq(*sig.xout, XOut));
    ASSERT(range_eq(*sig.docreturn, DocReturn));
    ASSERT(sig.valid == Valid);
  } else {
    if constexpr (not sig.data) {
      ASSERT(not sig.name and not sig.params_decl and not sig.params and not sig.docparams
             and not sig.xout and not sig.docreturn);
    } else if constexpr (not sig.name) {
      ASSERT(not sig.params_decl and not sig.params and not sig.docparams and not sig.xout
             and not sig.docreturn);
    } else if constexpr (not sig.params_decl) {
      ASSERT(not sig.params and not sig.docparams and not sig.xout and not sig.docreturn);
    } else if constexpr (not sig.params) {
      ASSERT(not sig.docparams and not sig.xout and not sig.docreturn);
    } else if constexpr (not sig.docparams) {
      ASSERT(not sig.xout and not sig.docreturn);
    } else if constexpr (not sig.xout) {
      ASSERT(not sig.docreturn);
    }
  }

  return true;
})();

static_assert(test_signature<           //
              /* sig:         */ "foo", //
              /* name:        */ "foo", //
              /* params_decl: */ ""     //
              /* params:      */
              /* docparams:   */
              /* xout:        */
              /* docreturn:   */
              /* valid:       */
              >);

static_assert(test_signature<              //
              /* sig:         */ "foo(a)", //
              /* name:        */ "foo",    //
              /* params_decl: */ "(a)",    //
              /* params:      */ "a"       //
              /* docparams:   */
              /* xout:        */
              /* docreturn:   */
              /* valid:       */
              >);

static_assert(test_signature<                      //
              /* sig:         */ "foo (bar, baz)", //
              /* name:        */ "foo",            //
              /* params_decl: */ "(bar, baz)",     //
              /* params:      */ "bar, baz"        //
              /* docparams:   */
              /* xout:        */
              /* docreturn:   */
              /* valid:       */
              >);

static_assert(test_signature<                            //
              /* sig:         */ "  foo (  bar, baz  )", //
              /* name:        */ "foo",                  //
              /* params_decl: */ "(  bar, baz  )",       //
              /* params:      */ "bar, baz"              //
              /* docparams:   */
              /* xout:        */
              /* docreturn:   */
              /* valid:       */
              >);

static_assert(test_signature<                          //
              /* sig:         */ "foo (baz: qux quz)", //
              /* name:        */ "foo",                //
              /* params_decl: */ "(baz: qux quz)",     //
              /* params:      */ "baz",                //
              /* docparams:   */ "qux quz"
              /* xout:        */
              /* docreturn:   */
              /* valid:       */
              >);

static_assert(test_signature<                                       //
              /* sig:         */ "  foo (  ...   :    qux quz   )", //
              /* name:        */ "foo",                             //
              /* params_decl: */ "(  ...   :    qux quz   )",       //
              /* params:      */ "...",                             //
              /* docparams:   */ "qux quz"
              /* xout:        */
              /* docreturn:   */
              /* valid:       */
              >);

static_assert(test_signature<              //
              /* sig:         */ "foo ()", //
              /* name:        */ "foo",    //
              /* params_decl: */ "()"      //
              /* params:      */           //
              /* docparams:   */
              /* xout:        */
              /* docreturn:   */
              /* valid:       */
              >);

static_assert(test_signature<                //
              /* sig:         */ "foo (  )", //
              /* name:        */ "foo",      //
              /* params_decl: */ "(  )"      //
              /* params:      */             //
              /* docparams:   */
              /* xout:        */
              /* docreturn:   */
              /* valid:       */
              >);

static_assert(test_signature<                  //
              /* sig:         */ "foo (  )  ", //
              /* name:        */ "foo",        //
              /* params_decl: */ "(  )"        //
              /* params:      */               //
              /* docparams:   */
              /* xout:        */
              /* docreturn:   */
              /* valid:       */
              >);

static_assert(test_signature<                                //
              /* sig:         */ "foo ( bar  :    baz  )  ", //
              /* name:        */ "foo",                      //
              /* params_decl: */ "( bar  :    baz  )",       //
              /* params:      */ "bar",                      //
              /* docparams:   */ "baz"
              /* xout:        */
              /* docreturn:   */
              /* valid:       */
              >);

static_assert(test_signature<                                      //
              /* sig:         */ "foo ( bar  :    baz  )  -> foo", //
              /* name:        */ "foo",                            //
              /* params_decl: */ "( bar  :    baz  )",             //
              /* params:      */ "bar",                            //
              /* docparams:   */ "baz",
              /* xout:        */ "",
              /* docreturn:   */ "foo"
              /* valid:       */
              >);

static_assert(test_signature<                             //
              /* sig:         */ "foo  -> f bar , bazoo", //
              /* name:        */ "foo",                   //
              /* params_decl: */ "",                      //
              /* params:      */ "",                      //
              /* docparams:   */ "",
              /* xout:        */ "",
              /* docreturn:   */ "f bar , bazoo"
              /* valid:       */
              >);

static_assert(test_signature<                       //
              /* sig:         */ "foo -<32>-> bar", //
              /* name:        */ "foo",             //
              /* params_decl: */ "",                //
              /* params:      */ "",                //
              /* docparams:   */ "",
              /* xout:        */ "32",
              /* docreturn:   */ "bar"
              /* valid:       */
              >);

static_assert(test_signature<                   //
              /* sig:         */ "foo -<32>->", //
              /* name:        */ "foo",         //
              /* params_decl: */ "",            //
              /* params:      */ "",            //
              /* docparams:   */ "",
              /* xout:        */ "32",
              /* docreturn:   */ ""
              /* valid:       */
              >);

static_assert(test_signature<                               //
              /* sig:         */ "foo -< 32   >->  f  foo", //
              /* name:        */ "foo",                     //
              /* params_decl: */ "",                        //
              /* params:      */ "",                        //
              /* docparams:   */ "",
              /* xout:        */ "32",
              /* docreturn:   */ "f  foo"
              /* valid:       */
              >);

static_assert(test_signature<                                   //
              /* sig:         */ "foo -< <<<>>>   >->  f  foo", //
              /* name:        */ "foo",                         //
              /* params_decl: */ "",                            //
              /* params:      */ "",                            //
              /* docparams:   */ "",
              /* xout:        */ "<<<>>>",
              /* docreturn:   */ "f  foo"
              /* valid:       */
              >);

static_assert(test_signature<                         //
              /* sig:         */ "foo() -<32>-> bar", //
              /* name:        */ "foo",               //
              /* params_decl: */ "()",                //
              /* params:      */ "",                  //
              /* docparams:   */ "",
              /* xout:        */ "32",
              /* docreturn:   */ "bar"
              /* valid:       */
              >);

static_assert(test_signature<                        //
              /* sig:         */ "foo(bar) -<32>->", //
              /* name:        */ "foo",              //
              /* params_decl: */ "(bar)",            //
              /* params:      */ "bar",              //
              /* docparams:   */ "",
              /* xout:        */ "32",
              /* docreturn:   */ ""
              /* valid:       */
              >);

static_assert(test_signature<                                         //
              /* sig:         */ "foo(bar, baz) -< 32   >->  f  foo", //
              /* name:        */ "foo",                               //
              /* params_decl: */ "(bar, baz)",                        //
              /* params:      */ "bar, baz",                          //
              /* docparams:   */ "",
              /* xout:        */ "32",
              /* docreturn:   */ "f  foo"
              /* valid:       */
              >);

static_assert(test_signature<                                             //
              /* sig:         */ "foo(...: bar) -< <<<>>>   >->  f  foo", //
              /* name:        */ "foo",                                   //
              /* params_decl: */ "(...: bar)",                            //
              /* params:      */ "...",                                   //
              /* docparams:   */ "bar",
              /* xout:        */ "<<<>>>",
              /* docreturn:   */ "f  foo"
              /* valid:       */
              >);

static_assert(test_signature<              //
              /* sig:         */ "foo  -", //
              /* name:        */ "",       //
              /* params_decl: */ "",       //
              /* params:      */ "",       //
              /* docparams:   */ "",
              /* xout:        */ "",
              /* docreturn:   */ "",
              /* valid:       */ false>);

static_assert(test_signature<                //
              /* sig:         */ "foo()  -", //
              /* name:        */ "",         //
              /* params_decl: */ "",         //
              /* params:      */ "",         //
              /* docparams:   */ "",
              /* xout:        */ "",
              /* docreturn:   */ "",
              /* valid:       */ false>);

static_assert(test_signature<                     //
              /* sig:         */ "foo(:jfes)  -", //
              /* name:        */ "",              //
              /* params_decl: */ "",              //
              /* params:      */ "",              //
              /* docparams:   */ "",
              /* xout:        */ "",
              /* docreturn:   */ "",
              /* valid:       */ false>);

static_assert(test_signature<             //
              /* sig:         */ "3foo ", //
              /* name:        */ "3foo",  //
              /* params_decl: */ "",      //
              /* params:      */ "",      //
              /* docparams:   */ "",
              /* xout:        */ "",
              /* docreturn:   */ "",
              /* valid:       */ true>);

static_assert(test_signature<               //
              /* sig:         */ "3foo() ", //
              /* name:        */ "3foo",    //
              /* params_decl: */ "()",      //
              /* params:      */ "",        //
              /* docparams:   */ "",
              /* xout:        */ "",
              /* docreturn:   */ "",
              /* valid:       */ true>);

static_assert(test_signature<                  //
              /* sig:         */ "foo(foo:) ", //
              /* name:        */ "foo",        //
              /* params_decl: */ "(foo:)",     //
              /* params:      */ "foo",        //
              /* docparams:   */ "",
              /* xout:        */ "",
              /* docreturn:   */ "",
              /* valid:       */ false>);

static_assert(test_signature<                               //
              /* sig:         */ " foo (foo : (bar...) ) ", //
              /* name:        */ "foo",                     //
              /* params_decl: */ "(foo : (bar...) )",       //
              /* params:      */ "foo",                     //
              /* docparams:   */ "(bar...)",
              /* xout:        */ "",
              /* docreturn:   */ "",
              /* valid:       */ true>);

static_assert(test_signature<                //
              /* sig:         */ "foo -<>>", //
              /* name:        */ "foo",      //
              /* params_decl: */ "",         //
              /* params:      */ "",         //
              /* docparams:   */ "",
              /* xout:        */ "",
              /* docreturn:   */ "",
              /* valid:       */ false>);

static_assert(test_signature<               //
              /* sig:         */ "foo -<>", //
              /* name:        */ "foo",     //
              /* params_decl: */ "",        //
              /* params:      */ "",        //
              /* docparams:   */ "",
              /* xout:        */ "",
              /* docreturn:   */ "",
              /* valid:       */ false>);

static_assert(test_signature<                  //
              /* sig:         */ "foo -<>-->", //
              /* name:        */ "foo",        //
              /* params_decl: */ "",           //
              /* params:      */ "",           //
              /* docparams:   */ "",
              /* xout:        */ "",
              /* docreturn:   */ "",
              /* valid:       */ false>);

static_assert(test_signature<                  //
              /* sig:         */ "foo --<>->", //
              /* name:        */ "foo",        //
              /* params_decl: */ "",           //
              /* params:      */ "",           //
              /* docparams:   */ "",
              /* xout:        */ "",
              /* docreturn:   */ "",
              /* valid:       */ false>);

} // namespace tests
} // namespace impl

struct def_base::instance*
def_base::get_instance() {
  /// the empty instance to be returned on macro default constructions.
  static struct instance default_instance {
    .context = {}, .id = {}, .category = {}, .name = {}, .source_loc = {}, .clang_format = {},
  };
  return &default_instance;
}

struct def_base::instance*
def_base::get_instance(std::string const& name, detail::source_location const& loc) {
  // construct id
  std::string id{};
  for (auto it = _exec_stack.begin(); it != _exec_stack.end(); ++it)
    id += (*it)->name + "_";

  id += name;

  // better nesting names

  static std::regex xx_xx{"_([xX]+)_([xX])(_|$)", std::regex_constants::optimize};
  static std::regex oo_oo{"_([oO]+)_([oO])(_|$)", std::regex_constants::optimize};
  while (std::regex_search(id, xx_xx))
    id = std::regex_replace(id, xx_xx, "_$1$2$3");
  while (std::regex_search(id, oo_oo))
    id = std::regex_replace(id, oo_oo, "_$1$2$3");

  // apply naming scheme
  if (not _exec_stack.empty()) {
    id = implname(id);
  } else {
    id = apiname(id);
  }

  // better nesting names, cont.

  static std::regex oo{"_([oO]+)(_|$)", std::regex_constants::optimize};
  for (auto it = std::sregex_iterator{id.begin(), id.end(), oo}; it != std::sregex_iterator{};
       ++it) {
    for (long i = 0; i < it->length(1); ++i)
      id[it->position(1) + i] = 'o';
  }

  // return the macro associated with this id if it exists
  // (and check for exec_stack discrepancies to ensure no id collisions)
  for (auto&& v : _instances) {
    if (v.id == id) {
      if (v.context != _exec_stack)
        throw std::runtime_error{"two macros have the same id: " + id + "(L"
                                 + utl::to_string(v.source_loc.line()) + " and L"
                                 + utl::to_string(loc.line())};
      return &v;
    }
  }

  std::list<instance>::iterator ins_it;
  instance*                     parent{nullptr};
  if (not _exec_stack.empty()) {
    parent = _exec_stack.back();
    ins_it = std::ranges::find(_instances, parent, [](auto&& v) {
      return &v;
    });
    if (ins_it == _instances.end())
      throw std::logic_error{"cannot find instance " + _exec_stack.back()->id
                             + " in the instances list"};
    ins_it = std::next(ins_it);
  } else {
    ins_it = _instances.end();
  }

  // insert after parent (if any), and return
  auto res = &*_instances.insert(ins_it, instance{
                                             .context      = _exec_stack,
                                             .id           = id,
                                             .category     = _cur_category,
                                             .name         = name,
                                             .source_loc   = loc,
                                             .clang_format = _cur_clang_format,
                                         });
  return res;
}

void
def_base::update_instance(runtime_signature const& sig) {
  if (not _instance)
    throw std::logic_error{"error in update_instance: instance pointer was somehow not assigned"};
  if (_instance->name.empty())
    throw std::logic_error{"attempted to update instance before assigning a name"};

  auto e = std::runtime_error{
      "attempting to add more signature details to an already defined macro " + _instance->id};

  if (not sig.params.empty()) {
    if (_instance->params and sig.params != *_instance->params)
      throw std::runtime_error{"params mismatch in signature update for " + _instance->id + ". "
                               + sig.params + " != " + *_instance->params + "."};
    if (_instance->definition)
      throw e;
    _instance->params = sig.params;
  } else if (sig.is_fn) {
    if (_instance->params and not _instance->params->empty())
      throw std::runtime_error{"attempted to update non-nullary " + _instance->id
                               + " with a nullary signature."};
    if (_instance->definition)
      throw e;
    _instance->params = std::string{};
  }

  if (not sig.docparams.empty()) {
    if (_instance->docparams and sig.docparams != *_instance->docparams)
      throw std::runtime_error{"docparams mismatch in signature update for " + _instance->id + ". "
                               + sig.docparams + " != " + *_instance->docparams + "."};
    if (_instance->definition)
      throw e;
    _instance->docparams = sig.docparams;
  }

  if (not sig.xout.empty()) {
    if (_instance->xout and sig.xout != *_instance->xout)
      throw std::runtime_error{"xout mismatch in signature update for " + _instance->id + ". "
                               + sig.xout + " != " + *_instance->xout + "."};
    if (_instance->definition)
      throw e;
    _instance->xout = sig.xout;
  }

  if (not sig.docreturn.empty()) {
    if (_instance->docreturn and sig.docreturn != *_instance->docreturn)
      throw std::runtime_error{"docreturn mismatch in signature update for " + _instance->id + ". "
                               + sig.docreturn + " != " + *_instance->docreturn + "."};
    if (_instance->definition)
      throw e;
    _instance->docreturn = sig.docreturn;
  }
}

namespace impl {
[[nodiscard]] inline constexpr char const*
skip_spaces(char const* s) {
  if (not s)
    return s;
  for (; *s and *s == ' '; ++s)
    ;
  return s;
}
} // namespace impl

std::string
def_base::synopsis() {
  std::string                                                    summary{"# Synopsis\n\n"};
  std::string                                                    details{};
  std::unordered_map<std::string, std::vector<struct instance*>> by_category{};
  for (auto&& v : _instances) {
    if (v.category.empty())
      throw std::runtime_error{"macro " + v.id
                               + " does not have a category. all macros must have a category"};
    if (v.context.empty())
      by_category[v.category].push_back(&v);
  }
  if (by_category.size() != _defined_categories.size())
    throw std::logic_error{"mismatch between defined categories and instance-derived categories"};
  std::unordered_set<std::string> seen_categories{};
  for (auto&& v : _defined_categories) {
    if (seen_categories.contains(v))
      continue;
    seen_categories.insert(v);
    if (not by_category.contains(v))
      throw std::logic_error{"mismatch between defined categories and instance-derived categories"};
    summary += "## [" + v + "](#" + v + ")\n\n";
    details += "[" + v + "]\n\n";
    for (auto&& inst : by_category[v]) {
      if (inst->description.empty())
        throw std::logic_error{"must provide a description for macro " + inst->id};
      std::string desc_firstln{};
      for (auto&& v : inst->description) {
        if (v == '\n')
          break;
        desc_firstln += v;
      }
      if (desc_firstln.back() != '.')
        desc_firstln.push_back('.');
      summary += "- [" + inst->name + "](#" + inst->name + "): " + desc_firstln + "\n";
      details += utl::prefix_lines("/// ", inst->description) + "\n";
      details += inst->id;
      if (inst->params) {
        if (inst->docparams) {
          details += "(" + *inst->docparams + ")";
        } else {
          details += "(" + *inst->params + ")";
        }
      }
      details += "\n\n";
    }
    summary += "\n";
  }
  if (not details.empty())
    details.pop_back();
  if (not details.empty())
    details.pop_back();
  return summary + "\n" + details;
}

std::string
def_base::definitions() {
  std::string res{};
  instance*   fdm_end{nullptr};
  bool        last_clang_format = true;
  for (auto&& v : _instances) {
    if (not v.definition)
      throw std::runtime_error{"macro " + v.id + " missing definition"};

    {
      if (last_clang_format and not v.clang_format)
        res += "\n// clang-format off\n";
      else if (not last_clang_format and v.clang_format)
        res += "\n// clang-format on\n";
      last_clang_format = v.clang_format;

      res += *v.definition + "\n";
    }

    static auto buf = utl::ii << [&] {
      std::array<std::string, 73> dashes;
      for (std::size_t i = 0; i < 73; ++i) {
        if (i % 2 == 0)
          dashes[i] = "-";
        else
          dashes[i] = " ";
      }
      return " " + utl::cat(dashes) + " ";
    };
    if (&v == fdm_end) {
      res += "\n\n//" + buf + "}}}\n\n";
      fdm_end = nullptr;
    } else if (v.context.empty() and not v.children.empty()) {
      fdm_end = v.children.front();
      while (not fdm_end->children.empty())
        fdm_end = fdm_end->children.front();
      res += "\n\n//" + buf + "{{{\n\n";
    }
  }
  if (not res.empty())
    res.pop_back();
  return res;
}

std::string
def_base::assertions() noexcept {
  static std::unordered_map<std::string, std::string> cache{};

  std::string res{"namespace {\n"
                  "// streq that trims spacing around outer ASSERT_PP_EQ\n"
                  "// parens to account for preprocessor discrepancies\n"
                  "// / *( *\\(.*\\) */\n"
                  "static constexpr bool\n"
                  "pp_streq(char const* l, char const* r) {\n"
                  "  for (; *l and *l != '('; ++l)\n"
                  "    ;\n"
                  "  for (; *l and *r != '('; ++r)\n"
                  "    ;\n"
                  "  if (not *l)\n"
                  "    return not *r;\n"
                  "  if (not *r)\n"
                  "    return not *l;\n"
                  "  ++l;\n"
                  "  ++r;\n"
                  "  for (; *l == ' '; ++l)\n"
                  "    ;\n"
                  "  for (; *r == ' '; ++r)\n"
                  "    ;\n"
                  "  char const* le = l;\n"
                  "  char const* re = r;\n"
                  "  for (; *le; ++le)\n"
                  "    ;\n"
                  "  for (; *re; ++re)\n"
                  "    ;\n"
                  "  for (; le > l and *(le - 1) != ')'; --le)\n"
                  "    ;\n"
                  "  for (; re > r and *(re - 1) != ')'; --re)\n"
                  "    ;\n"
                  "  --le;\n"
                  "  --re;\n"
                  "  for (; le > l and *(le - 1) == ' '; --le)\n"
                  "    ;\n"
                  "  for (; re > r and *(re - 1) == ' '; --re)\n"
                  "    ;\n"
                  "  for (; l != le and r != re and *l == *r; ++l, ++r)\n"
                  "    ;\n"
                  "  return l == le and r == re;\n"
                  "}\n"
                  "} // namespace\n"
                  "\n"
                  "#define ASSERT_PP_EQ_X(a, b) static_assert(pp_streq(#a, #b), #a \" != \" #b)\n"
                  "#define ASSERT_PP_EQ(a, b)   ASSERT_PP_EQ_X(a, b)\n"
                  "\n"
                  "// clang-format off\n"};
  for (auto&& v : _instances) {
    if (v.tests.empty() and v.examples.empty())
      continue;
    if (not cache.contains(v.id))
      cache[v.id] = ([&] {
        std::string cases{};
        for (auto&& v : v.examples)
          cases += "ASSERT_PP_EQ((" + v[0] + "), (" + v[1] + "));\n";
        for (auto&& v : v.tests)
          cases += "ASSERT_PP_EQ((" + v[0] + "), (" + v[1] + "));\n";
        if (not cases.empty())
          cases.pop_back();
        return cases;
      })();
    res += cache[v.id] + "\n\n";
  }
  if (not res.empty())
    res.pop_back();
  res += "// clang-format on";
  return res;
}

def_base::def_base() : _instance{get_instance()} {
}

def_base::def_base(runtime_signature const& sig, detail::source_location const& loc)
    : _instance{get_instance(sig.name, loc)} {
  update_instance(sig);
}

def_base::operator std::string const&() const noexcept {
  return _instance->id;
}

class docs const&
docs::operator<<(std::string const& line) const {
  if (def_base::_exec_stack.empty())
    throw std::runtime_error{"cannot use docs outside of a macro body"};
  if (not def_base::_exec_stack.back()->description.empty())
    def_base::_exec_stack.back()->description += "\n";
  def_base::_exec_stack.back()->description += line;
  return *this;
}

tests::example_tag::example_tag(std::string const& expected) : _expected{expected} {
}

tests::example_tag
operator>>(std::string const& expected, class docs const&) noexcept {
  return {expected};
}

tests::expected::expected(std::string const& actual) : _actual{actual} {
}

class tests const&
tests::expected::operator=(std::string const& expected) const {
  if (def_base::_exec_stack.empty())
    throw std::runtime_error{"cannot use tests outside of a macro body"};
  def_base::_exec_stack.back()->tests.push_back({_actual, expected});

  return codegen::tests;
}

class tests const&
tests::expected::operator=(example_tag const& expected) const {
  if (def_base::_exec_stack.empty())
    throw std::runtime_error{"cannot use tests outside of a macro body"};
  def_base::_exec_stack.back()->examples.push_back({_actual, expected.expected()});

  return codegen::tests;
}

tests::expected
tests::operator<<(std::string const& actual) const {
  return expected{actual};
}

void
clang_format::operator=(bool on) const {
  if (def_base::_exec_stack.empty())
    throw std::runtime_error{"cannot use clang_format outside of a macro body"};
  static def_base::instance* off_setter{nullptr};
  if (def_base::_cur_clang_format == on)
    throw std::runtime_error{"clang_format must only be toggled. redundant setting: "
                             + std::to_string(on)};

  if (on) {
    if (off_setter == nullptr)
      throw std::runtime_error{"cannot set clang_format on without setting off first"};
    if (def_base::_exec_stack.back() != off_setter)
      throw std::runtime_error{
          "setting clang_format on from a different macro than the off setter: "
          + def_base::_exec_stack.back()->id + " != " + off_setter->id};
    def_base::_cur_clang_format = true;
    off_setter                  = nullptr;
  } else {
    def_base::_cur_clang_format = false;
    off_setter                  = def_base::_exec_stack.back();
  }
}

void
arg::validate_context() const {
  if (def_base::_exec_stack.empty() or _parent != def_base::_exec_stack.back())
    throw std::runtime_error{"preprocessor arg \"" + _value
                             + "\" used outside of its defining function: " + _parent->id};
}

arg::arg(std::string const& value)
    : _value{value}, _parent{def_base::_exec_stack.empty() ? nullptr
                                                           : def_base::_exec_stack.back()} {
  if (value == "__VA_ARGS__")
    throw std::runtime_error{"cannot use codegen::arg for variadic arguments"};
  if (def_base::_exec_stack.empty())
    throw std::runtime_error{"cannot use codegen::arg outside of a macro body"};
}

arg::operator std::string const&() const {
  validate_context();
  return _value;
}

std::string
operator+(arg const& l, std::string const& r) {
  l.validate_context();
  return l._value + r;
}

std::string
operator+(std::string const& l, arg const& r) {
  r.validate_context();
  return l + r._value;
}

void
va::validate_context() const {
  if (def_base::_exec_stack.empty() or _parent != def_base::_exec_stack.back())
    throw std::runtime_error{"preprocessor arg \"" + _value
                             + "\" used outside of its defining function: " + _parent->id};
}

va::va(std::string const& value)
    : _parent{def_base::_exec_stack.empty() ? nullptr : def_base::_exec_stack.back()} {
  if (value != "__VA_ARGS__")
    throw std::runtime_error{"cannot use codegen::va for non-variadic arguments: " + value};
  if (def_base::_exec_stack.empty())
    throw std::runtime_error{"cannot use codegen::va outside of a macro body"};
}

va::operator std::string const&() const {
  validate_context();
  return _value;
}

std::string
operator+(va const& l, std::string const& r) {
  l.validate_context();
  return l._value + r;
}

std::string
operator+(std::string const& l, va const& r) {
  r.validate_context();
  return l + r._value;
}

void
pack::validate_context() const {
  if (def_base::_exec_stack.empty() or _parent != def_base::_exec_stack.back())
    throw std::runtime_error{"preprocessor pack args used outside of its defining function: "
                             + _parent->id};
}

pack::pack(std::vector<std::string> const& value)
    : _value{value}, _parent{def_base::_exec_stack.empty() ? nullptr
                                                           : def_base::_exec_stack.back()} {
  if (def_base::_exec_stack.empty())
    throw std::runtime_error{"cannot use codegen::pack outside of a macro body"};
}

pack::operator std::vector<std::string> const&() const {
  validate_context();
  return _value;
}

std::vector<std::string>::const_iterator
pack::begin() const {
  validate_context();
  return _value.begin();
}

std::vector<std::string>::const_iterator
pack::end() const {
  validate_context();
  return _value.end();
}

std::string const&
pack::front() const {
  validate_context();
  return _value.front();
}

std::string const&
pack::back() const {
  validate_context();
  return _value.back();
}

std::string const&
pack::operator[](std::size_t idx) const {
  validate_context();
  return _value[idx];
}

std::size_t
pack::size() const {
  validate_context();
  return _value.size();
}

} // namespace codegen

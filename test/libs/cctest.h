#ifndef CCTEST_H_INCLUDED
#define CCTEST_H_INCLUDED
/*                                                                         C++17

                                __                   __
                               /\ \__               /\ \__
                     ___    ___\ \ ,_\    __    ____\ \ ,_\
                    /'___\ /'___\ \ \/  /'__`\ /',__\\ \ \/
                   /\ \__//\ \__/\ \ \_/\  __//\__, `\\ \ \_
                   \ \____\ \____\\ \__\ \____\/\____/ \ \__\
                    \/____/\/____/ \/__/\/____/\/___/   \/__/

                        a tiny test framework for C++17

    Copyright (C) 2020, 2021 Justin Collier

      This program is free software: you can redistribute it and/or modify
      it under the terms of the GNU General Public License as published by
      the Free Software Foundation, either version 3 of the License, or
      (at your option) any later version.

      This program is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the internalied warranty of
      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
      GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#if (!defined(__cplusplus) || __cplusplus < 201703L)
#error "cctest requires at least C++17"
#endif

/// \file
/// \brief cctest is a tiny test framework for C++17 [single-header]
///
/// Creating test files:
///
/// \code
///   #include <cctest.h>
///   #include <cstring>
///   
///   TEST("test all the things") {
///     static_assert(true, "use static_assert wherever possible!");
///     ASSERT(1 == 1); // make a runtime assertion
///     auto e = ASSERT_THROWS(std::runtime_error) { // returns thrown error by value
///       throw std::runtime_error{"hello"};
///     };
///     ASSERT(strcmp(e.what(), "hello") == 0);
///   };
///   
///   TEST("multiple tests per file are OK") {
///     static_assert(true, "runtime assertions are not required");
///   };
/// \endcode
///
/// Creating a main file:
///
/// \code
///   #define CCTEST_MAIN
///   #include <cctest.h>
/// \endcode
///
/// Execution:
///
/// \code
///   ${CXX} ${CXXFLAGS} -std=c++17 main.cc test0.cc test1.cc -o test
///   ./test
/// \endcode

#include <atomic>
#include <iostream>
#include <string_view>

#if defined(__clang__) && __clang__ == 1
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++98-compat"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#pragma clang diagnostic ignored "-Wpadded"
#endif

namespace cctest {

/*.----------------------------------------------------------------------------,
 /                             internal details                              */

namespace detail_ {
extern std::atomic<int> n_tests_failed;

namespace is_void_fn_ {
/// sfinae fallback false
template <class, class = void>
struct sfinae {
  static constexpr bool result = false;
};
/// sfinae valid if F returns void
template <class F>
struct sfinae<
    F, std::enable_if_t<std::is_same_v<decltype(std::declval<F>()()), void>>> {
  static constexpr bool result = true;
};
} // namespace is_void_fn_
template <class F>
inline constexpr bool is_void_fn = is_void_fn_::sfinae<F>::result;
template <class Exception>
struct assert_throws {
  template <class Op, std::enable_if_t<is_void_fn<Op>>* = nullptr>
  inline Exception
  operator<<(Op&& op) const {
    try {
      op();
    } catch (Exception const& except) { return except; } catch (...) {
      throw std::runtime_error("[cctest]: unexpected throw type");
    }
    throw std::runtime_error("[cctest]: throw assertion failed");
  }
};
} // namespace detail_

/*.----------------------------------------------------------------------------,
 /                               cctest::test                                */

/// assertion class and description logger
struct test {
  constexpr test()            = delete;
  constexpr test(test const&) = delete;
  constexpr test&
  operator=(test const&) = delete;
  test(test&&)           = default;
  test&
  operator=(test&&) = default;
  /// default destructor
  ~test() noexcept = default;
  /// load the description
  test(char const* description) : description_(description) {
  }
  /// load the description, filename, and line number
  test(char const* description, char const* file, int line)
      : description_(description), file_(file), line_(line) {
  }

  /// invoke and assert a test body
  template <class Fn, std::enable_if_t<detail_::is_void_fn<Fn>>* = nullptr>
  inline test&&
  operator<<(Fn&& pred) noexcept {
    try {
      pred();
      pass();
    } catch (...) { fail(std::current_exception()); }
    return std::move(*this);
  }

 private:
  std::string description_ = "";
  std::string file_        = "";
  int line_                = -1;
  void
  pass() const noexcept {
    std::cout << get_description();
  }
  /// logs a failure message
  void
  fail(std::exception_ptr exception = nullptr) const noexcept {
    ++detail_::n_tests_failed;
    auto descr = get_description();
    descr += "\n     \033[01m\033[31mFAIL\033[0m\033[0m ";
    descr += file_;
    descr += ":";
    descr += std::to_string(line_);
    descr += '\n';
    try {
      if (exception != nullptr)
        std::rethrow_exception(exception);
      else
        throw std::runtime_error{""};
    } catch (std::exception const& e) {
      descr += "     what: ";
      descr += e.what();
      descr += "\n";
    } catch (...) {}
    std::cout << descr;
  }
  /// creates an initial description
  inline std::string
  get_description() const noexcept {
    return std::string{"\n\033[90m>\033[0m "} + description_;
  }
};

/*.----------------------------------------------------------------------------,
 /                   main execution success message logger                   */

/// prints "All Tests Passed!" in bold green to the console to indicate success
inline void
success() noexcept {
  std::cout << "\n\n\033[01m\033[32mAll Tests Passed!\033[0m\033[0m\n";
}

/// prints "[n] Tests Failed" in bold red to the console to indicate success
inline void
failure() noexcept {
  std::cout << "\n\n\033[01m\033[31m" << detail_::n_tests_failed << " Test"
            << (detail_::n_tests_failed > 1 ? "s" : "")
            << " Failed\033[0m\033[0m\n";
}

} // namespace cctest

/*.----------------------------------------------------------------------------,
 /                                   main                                    */

#ifndef CCTEST_DISABLE_MACROS
#ifdef TEST
#error \
    "TEST macro is already in use. please remove the library responsible or " \
    "#define CCTEST_DISABLE_MACROS"
#endif
#ifdef ASSERT
#error \
    "ASSERT macro is already in use. please remove the library responsible or " \
    "#define CCTEST_DISABLE_MACROS"
#endif
#ifdef ASSERT_THROWS
#error "ASSERT_THROWS macro is already in use. please remove the library " \
       "responsible or #define CCTEST_DISABLE_MACROS"
#endif

#define CCTEST_IMPL_CAT(_a, _b)   CCTEST_IMPL_CAT_X(_a, _b)
#define CCTEST_IMPL_CAT_X(_a, _b) _a##_b

#define TEST(_descr)                                        \
  static cctest::test CCTEST_IMPL_CAT(test_at_, __LINE__) = \
      cctest::test{_descr, __FILE__, __LINE__} << []

#define ASSERT(...)                     \
  if (!(__VA_ARGS__))                   \
    throw std::runtime_error {          \
      "assertion failed: " #__VA_ARGS__ \
    }
#define ASSERT_THROWS(...) cctest::detail_::assert_throws<__VA_ARGS__>{} << [&]

#ifdef CCTEST_MAIN
namespace cctest {
namespace detail_ {
std::atomic<int> n_tests_failed = 0;
} // namespace detail_
} // namespace cctest

int
main(int, char**) {
  if (cctest::detail_::n_tests_failed == 0) {
    cctest::success();
    return 0;
  } else {
    cctest::failure();
    return 1;
  }
}
#endif
#endif

#if defined(__clang__) && __clang__ == 1
#pragma clang diagnostic pop
#endif

#endif

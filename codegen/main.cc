////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////

#include <filesystem>
#include <fstream>
#include <iostream>

#include "codegen.h"

static char
last_nonspace_char(std::string const& s) {
  for (auto it = s.rbegin(); it != s.rend(); ++it) {
    if (not codegen::detail::isspace(*it)) {
      return *it;
    }
  }
  return '0';
}

int
main() {
  using namespace codegen;
  namespace fs = std::filesystem;
  {
    std::ofstream lout{conf::lib_output};
    std::ofstream tout{conf::tests_output};
    lout << "#ifndef " << conf::header_guard << "\n#define " << conf::header_guard << "\n"
         << conf::project_header << "\n\n"
         << def_base::definitions() << "\n\n#endif\n";
    tout << conf::project_header << "\n\n"
         << "#include " << fs::relative(conf::lib_output, fs::path(conf::tests_output).remove_filename())
         << "\n\n"
         << def_base::assertions() << '\n';
  } // namespace std::filesystem;

  /* printf("%s\n", macro::synopsis().c_str()); */

  system(std::string{"clang-format -i " + std::string{conf::lib_output}}.c_str());

  // clang-format AlignConsecutiveMacros breaks ColumnLimit; add newlines around sections where this happens
  // make sure that we are not splitting lines that necessarily need to be long (e.g. PTL_CAT)
  {
    unsigned col_lim{0};
    {
      std::ifstream clang_in{conf::clang_format_input, std::ios::in | std::ios::binary};
      auto          sz = fs::file_size(conf::clang_format_input);
      std::string   clang(sz, '\0');
      clang_in.read(clang.data(), sz);
      std::smatch m;
      if (std::regex_search(clang, m,
                            std::regex{"ColumnLimit\\s*:\\s*(\\d+)", std::regex_constants::optimize})) {
        std::istringstream iss{m[1]};
        iss >> col_lim;
      } else {
        std::cout << "warn: clang-format ColumnLimit not found\n";
        col_lim = 80;
      }
    }

    std::ifstream lin{conf::lib_output, std::ios::in | std::ios::binary};
    auto          sz = fs::file_size(conf::lib_output);
    std::string   lib(sz, '\0');
    lin.read(lib.data(), sz);
    lin.close();
    std::string formatted{};
    enum {
      find,
      skip,
    } mode{find};
    bool prev_line_was_comment{false};
    for (auto&& line : utl::split(lib, std::regex{"\n", std::regex_constants::optimize})) {
      if (line[0] == '/' and line[1] == '/') {
        prev_line_was_comment = true;
      } else {
        auto last_char = last_nonspace_char(formatted);
        if (mode == find) {
          if ((last_char != '\\' and last_char != ',') and line.size() > col_lim) {
            if (not prev_line_was_comment)
              formatted += '\n';
            mode = skip;
          }
        } else {
          if (line.size() <= col_lim) {
            if (last_char != '\\' and last_char != ',')
              if (not prev_line_was_comment)
                formatted += '\n';
            mode = find;
          }
        }
        prev_line_was_comment = false;
      }
      formatted += line + '\n';
    }

    {
      std::ofstream lout{conf::lib_output};
      lout << formatted;
    }
    system(std::string{"clang-format -i " + std::string{conf::lib_output}}.c_str());
  }

  system(std::string{"clang-format -i " + std::string{conf::tests_output}}.c_str());
  return 0;
}
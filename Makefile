# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#                          __    ___
#                         /\ \__/\_ \
#   _____   _____   __  __\ \ ,_\//\ \
#  /\ '__`\/\ '__`\/\ \/\ \\ \ \/ \ \ \
#  \ \ \_\ \ \ \_\ \ \ \_\ \\ \ \_ \_\ \_
#   \ \ ,__/\ \ ,__/\ \____/ \ \__\/\____\
#    \ \ \   \ \ \   \/___/   \/__/\/____/
#     \/_/    \/_/
#
#  pputl Preprocessor Utilities
#  Copyright (C) 2020 - 2022 Justin Collier <m@jpcx.dev>
#
#	   This program is free software: you can redistribute it and/or modify
#	   it under the terms of the GNU General Public License as published by
#	   the Free Software Foundation, either version 3 of the License, or
#	   (at your option) any later version.
#
#	   This program is distributed in the hope that it will be useful,
#	   but WITHOUT ANY WARRANTY; without even the implied warranty of
#	   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	   GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License          #
#  along with this program.  If not, see <https://www.gnu.org/licenses/>.   # #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

CP        ?= cp
CXX       ?= g++
CXXFLAGS  += -std=c++20 -Wall -Wextra -pedantic -Wno-gnu-zero-variadic-macro-arguments
TEST_SRC   = tests.cc
MAKEFLAGS += --no-print-directory

all:
	$(MAKE) -C codegen
	$(MAKE) run -C codegen
	$(MAKE) test

dev:
	$(MAKE) clean
	bear -- $(MAKE)
	$(CP) compile_commands.json codegen

test: $(TEST_SRC)
	$(CXX) -c $(CXXFLAGS) -o /dev/null $<

clean:
	$(MAKE) clean -C codegen

.PHONY: all dev test clean

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

CXX       ?= g++
WARNINGS   = -Wall -Wextra -Werror -pedantic -Wno-gnu-zero-variadic-macro-arguments
CXXFLAGS  += -std=c++20 -Icodegen -Icodegen/api $(WARNINGS)
RM        ?= rm -f
MKDIR     ?= mkdir
BIN       ?= .build/bin
SRCS      += $(shell find codegen -type f -name "*.cc")
TEST_SRC   = tests.cc
OBJS       = $(patsubst codegen/%.cc, .build/%.o, $(SRCS))
DEPS       = $(patsubst %.o, %.d, $(OBJS))

all: $(TEST_SRC)

$(TEST_SRC): $(BIN) pputl.h
	./$(BIN)

run: $(BIN) $(CGEN_OUT)
	./$(BIN)

clean:
	$(RM) $(BIN)
	$(RM) $(CGEN_OUT)
	$(RM) $(TEST_SRC)
	$(RM) .depend
	$(RM) -r .build

test: $(TEST_SRC)
	$(CXX) -c $(CXXFLAGS) -o /dev/null $<

.build:
	@$(MKDIR) .build

.build/%.o: codegen/%.cc
	@$(MKDIR) -p $(shell dirname $@)
	$(CXX) $(CXXFLAGS) -MMD -c -o $@ $<

%.d: %.o
	@find .build/ -type f -name "*.d" -exec cat > .depend {} +

$(BIN): $(OBJS) | $(DEPS)
	$(CXX) $(CXXFLAGS) -o $(BIN) $^

.depend:
	@touch .depend

include .depend

.PHONY: all run clean test

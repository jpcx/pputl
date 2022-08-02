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

CXX      ?= g++
CXXFLAGS += -std=c++20 -I. -pedantic
RM       ?= rm -f
MKDIR    ?= mkdir
BIN      ?= bin
SRCS     += $(shell find . -type f -name "*.cc")
OBJS      = $(patsubst ./%.cc, .build/%.o, $(SRCS))
DEPS      = $(patsubst %.o, %.d, $(OBJS))

all: $(BIN)

run: $(BIN)
	./$(BIN)

clean:
	$(RM) $(BIN)
	$(RM) .depend
	$(RM) -r .build

.build:
	@$(MKDIR) .build

.build/%.o: %.cc
	@$(MKDIR) -p $(shell dirname $@)
	$(CXX) $(CXXFLAGS) -MMD -c -o $@ $<

%.d: %.o
	@find .build/ -type f -name "*.d" -exec cat > .depend {} +

$(BIN): $(OBJS) | $(DEPS)
	$(CXX) $(CXXFLAGS) -o $(BIN) $^

.depend:
	@touch .depend

include .depend

.PHONY: all clean

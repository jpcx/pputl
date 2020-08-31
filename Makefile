# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#                           __    ___
#                          /\ \__/\_ \
#    _____   _____   __  __\ \ ,_\//\ \
#   /\ '__`\/\ '__`\/\ \/\ \\ \ \/ \ \ \
#   \ \ \_\ \ \ \_\ \ \ \_\ \\ \ \_ \_\ \_
#    \ \ ,__/\ \ ,__/\ \____/ \ \__\/\____\
#     \ \ \   \ \ \   \/___/   \/__/\/____/
#      \/_/    \/_/
# 
#   pputl Preprocessor Utilities
#   Copyright (C) 2020 Justin Collier <m@jpcx.dev>
# 
#     This program is free software: you can redistribute it and/or modify
#     it under the terms of the GNU General Public License as published by
#     the Free Software Foundation, either version 3 of the License, or
#     (at your option) any later version.
# 
#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the internalied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <https://www.gnu.org/licenses/>.
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

#   - - - - - - - - - - - - -    configuration     - - - - - - - - - - - - -   #

CXXFLAGS += -Iinclude -Itest -Ilibs/cctest -std=c++20

# set prefix to /usr/local by default
ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

PPUTL_INSTALL_DIR = ${DESTDIR}${PREFIX}/include/pputl/

PPUTL_GEN_HEADERS   = ${shell find include/pputl/gen   -maxdepth 1 -type f -name "*.h"}
PPUTL_JOIN_HEADERS  = ${shell find include/pputl/join  -maxdepth 1 -type f -name "*.h"}
PPUTL_TUPLE_HEADERS = ${shell find include/pputl/tuple -maxdepth 1 -type f -name "*.h"}

PPUTL_HEADERS       = ${shell find include/pputl       -maxdepth 1 -type f -name "*.h"}
PPUTL_ALL_HEADERS   = ${PPUTL_GEN_HEADERS} ${PPUTL_JOIN_HEADERS} ${PPUTL_TUPLE_HEADERS} ${PPUTL_HEADERS}

TEST_SOURCES = ${shell find test -type f -name "*.cc"}
TEST_OBJECTS = $(patsubst test/%.cc, .build/%.o, ${TEST_SOURCES})

#   - - - - - - - - - - - - - - -    phony   - - - - - - - - - - - - - - - -   #

all: testpputl

test: testpputl
	./testpputl

install: ${PPUTL_ALL_HEADERS}
	install -d ${PPUTL_INSTALL_DIR}
	install -d ${PPUTL_INSTALL_DIR}gen/
	install -d ${PPUTL_INSTALL_DIR}join/
	install -d ${PPUTL_INSTALL_DIR}tuple/
	install -m 644 ${PPUTL_HEADERS} ${PPUTL_INSTALL_DIR}
	install -m 644 ${PPUTL_GEN_HEADERS} ${PPUTL_INSTALL_DIR}gen/
	install -m 644 ${PPUTL_JOIN_HEADERS} ${PPUTL_INSTALL_DIR}join/
	install -m 644 ${PPUTL_TUPLE_HEADERS} ${PPUTL_INSTALL_DIR}tuple/

clean:
	${RM} testpputl
	${RM} -r .build

.PHONY: all test install clean

#   - - - - - - - - - - - - - -   compilation    - - - - - - - - - - - - - -   #

${TEST_OBJECTS}: | .build .build/pputl .build/pputl/gen .build/pputl/join .build/pputl/tuple

.build:
	@mkdir -p $@
.build/pputl:
	@mkdir -p $@
.build/pputl/gen:
	@mkdir -p $@
.build/pputl/join:
	@mkdir -p $@
.build/pputl/tuple:
	@mkdir -p $@

.build/%.o: test/%.cc
	${CXX} ${CXXFLAGS} -MD -MP -c $< -o $@

.build/main.o: test/main.cc
	${CXX} ${CXXFLAGS} -MD -MP -c $< -o $@

testpputl: ${TEST_OBJECTS}
	${CXX} ${CXXFLAGS} $^ -o $@

-include $(patsubst %.o, %.d, ${TEST_OBJECTS})

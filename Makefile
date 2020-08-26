CXXFLAGS += -I. -Itest -Ilibs/cctest -std=c++20

# set prefix to /usr/local by default
ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

PPUTL_INSTALL_DIR = ${DESTDIR}${PREFIX}/include/pputl/

PPUTL_TUPLE_HEADERS = pputl/tuple/args.h \
                      pputl/tuple/cat.h  \
                      pputl/tuple/chk.h  \
                      pputl/tuple/elem.h \
                      pputl/tuple/size.h

PPUTL_HEADERS = pputl/add.h       \
                pputl/cat.h       \
                pputl/dec.h       \
                pputl/eat.h       \
                pputl/empty.h     \
                pputl/eq.h        \
                pputl/expand.h    \
                pputl/gen.h       \
                pputl/genrepeat.h \
                pputl/iif.h       \
                pputl/inc.h       \
                pputl/includes.h  \
                pputl/nargs.h     \
                pputl/neq.h       \
                pputl/not.h       \
                pputl/nothing.h   \
                pputl/reduce.h    \
                pputl/separate.h  \
                pputl/stringize.h \
                pputl/sum.h       \
                pputl/switch.h    \
                pputl/tuple.h     \
								pputl/pputl.h

PPUTL_ALL_HEADERS = ${PPUTL_TUPLE_HEADERS} \
										${PPUTL_HEADERS}

TEST_SOURCES = test/main.cc             \
               test/pputl/add.cc        \
               test/pputl/cat.cc        \
               test/pputl/dec.cc        \
               test/pputl/eat.cc        \
               test/pputl/empty.cc      \
               test/pputl/eq.cc         \
               test/pputl/expand.cc     \
               test/pputl/gen.cc        \
               test/pputl/genrepeat.cc  \
               test/pputl/iif.cc        \
               test/pputl/inc.cc        \
               test/pputl/includes.cc   \
               test/pputl/nargs.cc      \
               test/pputl/neq.cc        \
               test/pputl/not.cc        \
               test/pputl/nothing.cc    \
               test/pputl/reduce.cc     \
               test/pputl/separate.cc   \
               test/pputl/stringize.cc  \
               test/pputl/sum.cc        \
               test/pputl/switch.cc     \
               test/pputl/tuple/args.cc \
               test/pputl/tuple/cat.cc  \
               test/pputl/tuple/chk.cc  \
               test/pputl/tuple/elem.cc \
               test/pputl/tuple/size.cc \

TEST_HEADERS = test/streq.h

all: testpputl

testpputl: ${PPUTL_ALL_HEADERS} ${TEST_SOURCES} ${TEST_HEADERS}
	${CXX} ${CXXFLAGS} ${TEST_SOURCES} -o $@

test: testpputl
	./testpputl

install: ${PPUTL_ALL_HEADERS}
	install -d ${PPUTL_INSTALL_DIR}/tuple/
	install -m 644 ${PPUTL_HEADERS} ${PPUTL_INSTALL_DIR}
	install -m 644 ${PPUTL_TUPLE_HEADERS} ${PPUTL_INSTALL_DIR}/tuple/

clean:
	${RM} -f testpputl

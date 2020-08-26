#include <pputl/pputl.h>

#define THREE PPUTL_SUM(1, 1, 1)

#if PPUTL_NEQ(THREE, 3)
static_assert(false);
#endif

#define MULTICAT PPUTL_CAT(my, cool, cat)

#define SUM_REDUCER(accum, val, i) PPUTL_ADD(a, v)
#define SUM(...) PPUTL_REDUCE(SUM_REDUCER, 0, __VA_ARGS__)

#define SEVEN SUM(1, 2, 4)

#define CCTEST_MAIN
#include <cctest/cctest.h>

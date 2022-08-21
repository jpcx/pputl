#include "pputl.h"

#define H3(X, ...) #__VA_OPT__(X##X X##X)

constexpr auto foo = H3(, 0);

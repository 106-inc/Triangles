#ifndef __TEST_UNIT_TEST_HEADER_HH__
#define __TEST_UNIT_TEST_HEADER_HH__

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"

#include  <gtest/gtest.h>
using FPTypes = testing::Types<float, double, long double>;

#endif // __TEST_UNIT_TEST_HEADER_HH__

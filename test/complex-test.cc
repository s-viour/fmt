#include "fmt/complex.h"
#include <complex>
#include <numeric>
#include "gmock.h"


TEST(ComplexTest, Format) {
  std::string s = fmt::format("{}", std::complex<double>(1, 2));
  EXPECT_EQ(s, "(1+2i)");
  EXPECT_EQ(fmt::format("{:.2f}", std::complex<double>(1, 2)), "(1.00+2.00i)");
  EXPECT_EQ(fmt::format("{:8}", std::complex<double>(1, 2)), "  (1+2i)");
  EXPECT_EQ(fmt::format("{:.2f}", std::complex<double>(1.52, 63.25)), "(1.52+63.25i)");
  EXPECT_EQ(fmt::format("{}", std::complex<double>(0, 1)), "i");
}

TEST(ComplexTest, LongDouble) {
	auto big = std::numeric_limits<long double>::max();
	auto c1 = std::complex<long double>(big, 0);
	auto c2 = std::complex<long double>(0, big);
	
	EXPECT_EQ("(1.18973e+4932+0i)", fmt::format("{}", c1));
	EXPECT_EQ("1.18973e+4932i", fmt::format("{}", c2));
}
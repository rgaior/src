#include <stdexcept>
#include <vector>

#include <gtest/gtest.h>

#include <MathFunctions.h>

using namespace std;
using namespace utl;


TEST(MathFunctionsTest, GenerateBinEdges)
{
  // Test input validation
  // - 0 bins
  ASSERT_THROW(GenerateBinEdges(0., 1., 0);, runtime_error);
  // - equal upper and lower limits
  ASSERT_THROW(GenerateBinEdges(0., 0., 1);, runtime_error);
  // - swapped limits
  ASSERT_THROW(GenerateBinEdges(1., 0., 1);, runtime_error);

  // Test function for one, two and many bins
  for (unsigned int nBins = 1; nBins < 10; nBins++) {
    vector<double> edges = GenerateBinEdges(10., 10. + 1. * nBins, nBins);
    EXPECT_EQ(edges.size(), nBins + 1);
    for (unsigned int i; i < nBins + 1; i++) {
      EXPECT_EQ(edges[i], 10. + 1. * i);
    }
  }
}


TEST(MathFunctionsTest, GenerateBinCenters)
{
  // Test input validation
  // - 0 bins
  ASSERT_THROW(GenerateBinCenters(0., 1., 0);, runtime_error);
  // - equal upper and lower limits
  ASSERT_THROW(GenerateBinCenters(0., 0., 1);, runtime_error);
  // - swapped limits
  ASSERT_THROW(GenerateBinCenters(1., 0., 1);, runtime_error);

  // Test function for one, two and many bins
  for (unsigned int nBins = 1; nBins < 10; nBins++) {
    vector<double> centers = GenerateBinCenters(10., 10. + 1. * nBins, nBins);
    EXPECT_EQ(centers.size(), nBins);
    for (unsigned int i; i < nBins; i++) {
      EXPECT_EQ(centers[i], 10. + (i + 0.5) * 1.0);
    }
  }
}

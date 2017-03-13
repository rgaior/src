#include <stdexcept>
#include <iomanip>

#include <gtest/gtest.h>

#include <TVector3.h>

#include <AugerUnits.h>
#include <ChannelGain.h>


using namespace std;
using namespace utl;


TEST(ChannelGainTest, HandlesNullPointer)
{
  ChannelGain channelGain;

  ASSERT_THROW({
      channelGain.Eval(TVector3());
    }, runtime_error);
}


TEST(ChannelGainTest, EvaluatesPointsCorrectly)
{
  // Create a histogram with 11x11 bins, centered at full degrees
  TH2D gainHist("hGain", "",
                11, -0.5 * degree / radian, 10.5 * degree / radian,
                11, -0.5 * degree / radian, 10.5 * degree / radian);
  // Fill the histogram with 11 * theta + phi
  for (int iTheta = 1; iTheta < 12; iTheta++) {
    for (int iPhi = 1; iPhi < 12; iPhi++) {
      if (iTheta == 1) {  // Set all zenith bins to the same value
        gainHist.SetBinContent(iTheta, iPhi, 11 * iTheta);
      } else {
        gainHist.SetBinContent(iTheta, iPhi, 11 * iTheta + iPhi - 1);
      }
    }
  }

  ChannelGain channelGain(&gainHist);
  for (double theta = 0.0; theta < 10.5 * degree; theta += 1.0 * degree) {
    for (double phi = 0.0; phi < 10.5 * degree; phi += 1.0 * degree) {
      TVector3 v(0.0, 0.0, 1.0);
      v.SetTheta(theta / radian);
      v.SetPhi(phi / radian);
      ASSERT_DOUBLE_EQ(11 * (v.Theta() * radian / degree + 1.0) +
                       v.Phi() * radian / degree, channelGain.Eval(v));
    }
  }
}

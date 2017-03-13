#ifndef ___MATHFUNCTIONS_H__
#define ___MATHFUNCTIONS_H__

#include <vector>
#include <TVector3.h>


//double Integrate(std::vector<double>& v , double a, double b);

namespace utl {
  std::vector<double> GenerateBinEdges(const double& xLow,
                                       const double& xHigh,
                                       const size_t& nBins);

  std::vector<double> GenerateBinCenters(const double& xLow,
                                         const double& xHigh,
                                         const size_t& nBins);

  std::vector<TVector3> RayConeIntersection(const TVector3& ray_point,
                                            const TVector3& ray_axis,
                                            const TVector3& cone_origin,
                                            const TVector3& cone_axis,
                                            const double cone_angle);

  double kthElement(std::vector<double> set, int k);
  double GetMedian(std::vector<double> set);
  double Integrate(std::vector<double>& v , double a, double b);
  double LogIntegrate (double (*f)(double), double a, double b, int n);
  /*
    Returns the signum (-1, 0, 1) of the given value. From
    <http://stackoverflow.com/questions/1903954/>.
  */
  template <typename T> int signum(const T& x) {
    return (T(0) < x) - (x < T(0));
  }

}

#endif

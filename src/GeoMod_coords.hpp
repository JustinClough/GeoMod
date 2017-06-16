#ifndef GEOMOD_COORDS_HPP
#define GEOMOD_COORDS_HPP

namespace GMD
{
  void sum_coords(double x[3], double y[3], double ans[3]);

  void subtract_coords( double fin[3], double intil[3], double ans[3]);

  void divide( double vec[3], double denom, double ans[3]);

  void get_mag(double vec[3], double& mag);

  void get_unit_vector( double vec[3], double unit[3]);

  void cross_product( double x[3], double y[3], double ans[3]);

  void dot_product( double x[3], double y[3], double& ans);

}
#endif

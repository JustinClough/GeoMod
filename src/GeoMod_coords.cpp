#include <GeoMod_coords.hpp>
#include <GeoMod_printer.hpp>
#include <cstdlib>
#include <math.h>

namespace GMD
{
  void sum_coords(double x[3], double y[3], double ans[3])
  {
    for(int i=0; i<3; i++)
    {
      ans[i] = x[i]+y[i];
    }
    return;
  }

  void subtract_coords( double fin[3], double intil[3], double ans[3])
  {
    for(int i=0; i<3; i++)
    {
      ans[i] = fin[i] - intil[i];
    }
    return;
  }

  void divide( double vec[3], double denom, double ans[3])
  {
    if(denom == 0.0)
    { 
      print_error("Denominator is zero. Division not defined.");
    }
    else
    {
      for(int i=0; i<3; i++)
      {
        ans[i] = vec[i]/denom;
      }
    }
    return;
  }

  void get_mag(double vec[3], double& mag)
  {
    double tmp = 0.0;
    for (int i=0; i<3; i++)
    {
      tmp += (vec[i]*vec[i]);
    }
    mag = sqrt( tmp);
  }

  void get_unit_vector( double vec[3], double unit[3])
  {
    double mag = 0.0;
    get_mag( vec, mag);
    divide(vec, mag, unit);
    return;
  }

  void cross_product( double x[3], double y[3], double ans[3])
  {
    ans[0] = x[1]*y[2]-x[2]*y[1];
    ans[1] = x[2]*y[0]-x[0]*y[2];
    ans[2] = x[0]*y[1]-x[1]*y[0];
    return;
  }

  void dot_product( double x[3], double y[3], double& ans)
  {
    for(int i=0; i<3; i++)
    {
      ans = x[i]*y[i];
    }
    return;
  }

}

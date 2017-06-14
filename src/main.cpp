// GeoMod Header
#include "GeoMod.hpp"

#include "GeoMod_Tests.hpp"

int main( int argc, char** argv)
{
  std::cout << "START" << std::endl;
  GMD::sim_start();

/*
  std::ofstream res;
  res.open("results.txt", std::ios_base::app);
  res << "N_dis\tN_FE\tN_surf\n" ;
  res.close();

  srand(time(NULL));
  for( int i=0; i< 41; i++)
  { 
    point_density_test( i);
  }
*/

  test1();
  test2();
  test3();
  test4();
  test5();
  //test6(); // edge to surface connection fails to mesh

  GMD::sim_end();
  std::cout << "END" << std::endl;
  return 0;
}

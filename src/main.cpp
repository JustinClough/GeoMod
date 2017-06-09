// GeoMod Header
#include "GeoMod.hpp"

#include "GeoMod_Tests.hpp"

int main( int argc, char** argv)
{
  std::cout << "START" << std::endl;
  GMD::sim_start();
  
  test1();
  test2();
  test3();
  test4();
  
  GMD::sim_end();
  std::cout << "END" << std::endl;
  return 0;
}

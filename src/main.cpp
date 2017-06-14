// GeoMod Header
#include "GeoMod.hpp"

#include "GeoMod_Tests.hpp"

int main( int argc, char** argv)
{
  char sim_log[] = "Sim_log.log";
  std::cout << "START" << std::endl;
  GMD::sim_start( sim_log);

  // mesh_growth_test();
  test1();
  test2();
  test3();
  test4();
  test5();
  test6(); 
  //test7();// edge to surface connection fails to mesh

  GMD::sim_end();
  std::cout << "END" << std::endl;
  return 0;
}

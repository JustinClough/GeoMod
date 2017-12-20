// GeoMod Header
#include "GeoMod.hpp"
#include "GeoMod_Tests.hpp"

#include <iostream>

int main( int argc, char** argv)
{
  char sim_log[] = "Sim_log.log";
  std::cout << "START" << std::endl;
  GMD::sim_start( sim_log, argc, argv);

  run_all_tests();

  GMD::sim_end();
  std::cout << "END" << std::endl;
  return 0;
}

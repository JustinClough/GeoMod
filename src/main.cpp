// GeoMod Header
#include "GeoMod.hpp"
#include "GeoMod_Tests.hpp"

#include <iostream>

int main( int argc, char** argv)
{
  char sim_log[] = "Sim_log.log";
  std::cout << "START" << std::endl;
  GMD::sim_start( sim_log, argc, argv);

  test0();
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  test8();
  test9();
  test10();
  test11();
  test12();
  test13();
  test14();
  test15();

  GMD::sim_end();
  std::cout << "END" << std::endl;
  return 0;
}

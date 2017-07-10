// GeoMod Header
#include "GeoMod.hpp"
#include "GeoMod_Tests.hpp"

#include "density_test.hpp"

#include <iostream>


int main( int argc, char** argv)
{
  char sim_log[] = "Sim_log.log";
  std::cout << "START" << std::endl;
  GMD::sim_start( sim_log, argc, argv);

  //int numSegs = 0;
#if 0
  numSegs = 1;
  std::cout << "NumSegs = " << numSegs << std::endl;
  test_density_random_walk( numSegs);
  numSegs = 10;
  std::cout << "NumSegs = " << numSegs << std::endl;
  test_density_random_walk( numSegs);
  numSegs = 25;
  std::cout << "NumSegs = " << numSegs << std::endl;
  test_density_random_walk( numSegs);
  numSegs = 50;
  std::cout << "NumSegs = " << numSegs << std::endl;
  test_density_random_walk( numSegs);
  numSegs = 100;
  std::cout << "NumSegs = " << numSegs << std::endl;
  test_density_random_walk( numSegs);
  numSegs = 250;
  std::cout << "NumSegs = " << numSegs << std::endl;
  test_density_random_walk( numSegs);
  numSegs = 500;
  std::cout << "NumSegs = " << numSegs << std::endl;
  test_density_random_walk( numSegs);
  numSegs = 1000;
  std::cout << "NumSegs = " << numSegs << std::endl;
  test_density_random_walk( numSegs);
  numSegs = 2000;
  std::cout << "NumSegs = " << numSegs << std::endl;
  test_density_random_walk( numSegs);
#endif

#if 0
  int numSegs = 0;
  int base = 0;
  // Need to test at 10, 25, 50, 100, 250, 500, 1000, 2500, 5000
    // Runs out of memory for 2500 & greater cases
  for( int i=0; i<3; i++)
  {
    if( i==0)
    { base = 10;}
    else if(i ==1)
    { base = 25;}
    else if(i ==2)
    { base = 50;}
    for (int j = 0; j<3; j++)
    {
      numSegs = base;
      for (int k=0; k<j; k++)
      {
        numSegs *= 10;
      }
      std::cout << "NumSegs = " << numSegs << std::endl;
      test_density_random_walk( numSegs);
    }
  }
#endif

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

  GMD::sim_end();
  std::cout << "END" << std::endl;
  return 0;
}

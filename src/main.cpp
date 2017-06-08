// GeoMod Header
#include "GeoMod.hpp"

int main( int argc, char** argv)
{
  GMD::sim_start();


  { // Need to scope so that gmd_t destructor is called before sim_end();

  
  
  }
  
  GMD::sim_end();

  std::cout << "END" << std::endl;
  return 0;
}

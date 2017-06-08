// GeoMod Header
#include "GeoMod.hpp"

int main( int argc, char** argv)
{
  std::cout << "START" << std::endl;
  GMD::sim_start();


  { // Need to scope so that gmd_t destructor is called before sim_end();

    
    GMD::gmd_t gmd( GMD::create_cube( 1.0));
    double point[] = {0.0, 0.0, 0.0};
    gmd.place_point( point, 0.1);
    char name[] = "test_point.smd";
    gmd.set_model_name( name);
    gmd.write_model();
  }
  
  GMD::sim_end();

  std::cout << "END" << std::endl;
  return 0;
}

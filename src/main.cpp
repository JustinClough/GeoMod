// Standard Library Headers
#include <iostream>

// GeoMod Header
#include "GeoMod.hpp"

int main( int argc, char** argv)
{
  GMD::sim_start();

  pGModel model = GMD::create_2D_bar( 6.0, 4.0);

  GMD::write_model(model, "name");
  
  GMD::release_model(model);
  
  GMD::sim_end();
  return 0;
}


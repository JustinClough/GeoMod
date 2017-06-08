// GeoMod Header
#include "GeoMod.hpp"

int main( int argc, char** argv)
{
  std::cout << "START" << std::endl;
  GMD::sim_start();


  { // Need to scope so that gmd_t destructor is called before sim_end();

    pGModel rect =  GMD::create_2D_rectangle( 1.0, 1.0);
    GMD::gmd_t gmd( rect);
    //double point[] = {0.0, 0.0, 0.0};
    //gmd.place_point( point, 0.1);
    //char name[] = "test_point.smd";
    //gmd.set_model_name( name);
    //gmd.write_model();
    //gmd.create_mesh();

    //gmd.update_model( GMD::create_cube(2.0));
    //char name2[] = "test_point2.smd";
    //gmd.set_model_name( name2);
    //gmd.write_model();
  }
  
  GMD::sim_end();

  std::cout << "END" << std::endl;
  return 0;
}

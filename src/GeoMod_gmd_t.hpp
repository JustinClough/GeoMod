#ifndef GEOMOD_GMD_T_HPP
#define GEOMOD_GMD_T_HPP

#include <GeoMod_model_helper.hpp>
#include <GeoMod_mesh_helper.hpp>

namespace GMD
{

  class gmd_t
  {
    public:
      gmd_t( pGModel in_model);
      ~gmd_t();
      void set_abort_on_fail( bool abort_on_fail);
      void place_point( double coords[3]);
      void test_printers();
    
    private:
      bool panic;
      model_helper_t* modeler;
      mesh_helper_t* mesher;
  };

}

#endif

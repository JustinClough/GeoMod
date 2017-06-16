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
    
    private:
      model_helper_t* modeler;
      mesh_helper_t* mesher;
  };

}

#endif

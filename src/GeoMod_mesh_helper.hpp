#ifndef GEOMOD_MESH_HELPER_HPP
#define GEOMOD_MESH_HELPER_HPP

#include <GeoMod_SIM.hpp>

namespace GMD
{
  class mesh_helper_t
  {
    friend class gmd_t;
    private:
      mesh_helper_t( pGModel in_model);
      ~mesh_helper_t();
      pMesh mesh;
      pACase m_case;
      void mesh_print();
      void place_point( double coords[3], double refine, double radius, bool abort_on_fail);

  };

}

#endif

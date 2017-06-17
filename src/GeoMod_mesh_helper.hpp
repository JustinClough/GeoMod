#ifndef GEOMOD_MESH_HELPER_HPP
#define GEOMOD_MESH_HELPER_HPP

#include <GeoMod_SIM.hpp>
#include <GeoMod_printer.hpp>
#include <string>

namespace GMD
{
  class mesh_helper_t
  {
    friend class gmd_t;
    private:
      // Util methods
      mesh_helper_t( pGModel in_model);
      ~mesh_helper_t();
      void mesh_print();
      void write( std::string name);

      // Members
      pMesh mesh;
      pACase m_case;

      // Mesh preping methods
      void place_point( double coords[3], double refine, double radius, bool abort_on_fail);

  };

}

#endif

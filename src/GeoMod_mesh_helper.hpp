#ifndef GEOMOD_MESH_HELPER_HPP
#define GEOMOD_MESH_HELPER_HPP

#include <string>
#include <vector>
#include <GeoMod_SIM.hpp>
#include <GeoMod_printer.hpp>

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
      bool isValid();
      void create();

      // Members
      pMesh mesh;
      pACase m_case;
      bool globalSet;
      double order;
      double refine;
      double grad_rate;

      // Mesh preping methods
      void place_point( double coords[3], double refine, double radius, bool abort_on_fail);
      void set_global( int order_in, double refine_in, double grad_rate_in);
      void refine_edge( double refine, pGEdge edge);

  };

}

#endif

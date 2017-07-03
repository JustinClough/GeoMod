#ifndef GEOMOD_MESH_HELPER_HPP
#define GEOMOD_MESH_HELPER_HPP

#include <string>
#include <vector>
#include "GeoMod_SIM.hpp"
#include "GeoMod_printer.hpp"

namespace GMD
{
  class mesh_helper_t
  {
    friend class gmd_t;
    private:
      // Util methods
      mesh_helper_t( pGModel in_model, int numParts);
      ~mesh_helper_t();
      void mesh_print();
      void write( std::string name);
      bool isValid();
      void create();
      void release();
      void print_mesh_info();

      // Members
      bool isPar;
      pMesh mesh;
      pParMesh parMesh;
      pACase m_case;
      bool globalSet;
      double order;
      double refine;
      double grad_rate;

      // Mesh preping methods
      void place_point( 
          double coords[3], 
          double refine, 
          double radius, 
          bool abort_on_fail);
      void set_global( int order_in, double refine_in, double grad_rate_in);
      void refine_vertex( double refine, pGVertex vert);
      void refine_edge( double refine, pGEdge edge);
      void refine_face( double refine, pGFace face);

  };

}

#endif

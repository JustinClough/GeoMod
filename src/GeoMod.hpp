#ifndef GEOMOD_HPP
#define GEOMOD_HPP

// Simmetrix Headers
#include "SimUtil.h"
#include "SimModel.h"
#include "SimAdvModel.h"
#include "MeshSim.h"

// Standard C++ Headers
#include <iostream>
#include <vector>
#include <cstdlib>

namespace GMD
{
  class gmd_t
  {
    public:
      gmd_t( pGModel geom);
      ~gmd_t();
      void update_model(pGModel geom);
      void write_mesh( );
      void write_model( );

      void set_mesh_name (char* name);
      char* get_mesh_name();
      void set_model_name (char* name);
      char* get_model_name();
      void verify_model( bool abort_on_fail=true);

      void set_global_mesh_params( double order, double refine);
      void place_point(double* point, double local_refine);
      pMesh get_mesh();
      pGModel get_model();
      pMesh create_mesh();

    protected:
      void set_point_refine( double* point, double local_refine);
      void set_mesh( );
      void set_model(pGModel geom);
      char* mesh_name;
      char* model_name;
      pGModel model;
      pMesh mesh;
      pACase m_case;
      bool g_mesh_set;
      double m_order;
      double m_g_refine;
  };

  pGModel create_2D_rectangle( double y_length, double x_width);

  void insert_vertex_on_face( pGModel geom, double* point);

  void release_model(pGModel model);

  void release_mesh(pMesh mesh);

  void sim_start();

  void sim_end();

  pGModel create_cube(double length);
} //END namespace GMD

pGVertex GIP_insertVertexInFace(pGIPart part, double* xyz, pGFace face);

#endif // GEOMOD_HPP

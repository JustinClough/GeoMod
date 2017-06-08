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
  class refine_t
  {
    

  };

  class mesh_helper_t
  {
    public:
      int mesh_order;
      double mesh_size;
      double grad_rate;
      mesh_helper_t ( pGModel model);
      ~mesh_helper_t ();
      pACase m_case;
      double global_refine;
      std::vector<refine_t> refines;
  };

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
      void place_edge_lagraneInterp (std::vector<double*> pts)
      pMesh get_mesh();
      pGModel get_model();
      pMesh create_mesh();

    protected:
      void set_mesh(pMesh m);
      void set_model(pGModel geom);
      char* mesh_name;
      char* model_name;
      pMesh mesh;
      pGModel model;
      mesh_helper_t* m_helper;
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

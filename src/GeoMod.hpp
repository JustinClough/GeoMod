#ifndef GEOMOD_HPP
#define GEOMOD_HPP

// Simmetrix Headers
#include "SimUtil.h"
#include "SimModel.h"
#include "SimAdvModel.h"
#include "MeshSim.h"

// Standard C++ Headers
#include <iostream>
#include <stdio.h>
#include <vector>
#include <cstdlib>

namespace GMD
{
  class mesh_helper_t
  {
    public:
      mesh_helper_t( pMesh m, pGModel geom);
      pMesh mesh;
      pGModel model;
  
    protected:

  };

  class gmd_t
  {
    public:
      gmd_t( pMesh m, pGModel geom);
      ~gmd_t();
      void update_mesh(pMesh m);
      void update_model(pGModel geom);
      void write_mesh( const char* filename);
      void write_model( const char* filename);
      pMesh get_mesh();
      pGModel get_model();

    protected:
      void set_mesh(pMesh m);
      void set_model(pGModel geom);
      pMesh mesh;
      pGModel model;
      mesh_helper_t m_helper (pMesh m, pGModel geom);
  };

  pGModel create_2D_bar( double length, double width);

  pMesh create_mesh( pGModel geom);

  void insert_vertex_on_face( pGModel geom, double* point);

  void release_model(pGModel model);

  void release_mesh(pMesh mesh);

  void sim_start();

  void sim_end();

  pGModel make_box(double length);
} //END namespace GMD

pGVertex GIP_insertVertexInFace(pGIPart part, double* xyz, pGFace face);

#endif // GEOMOD_HPP

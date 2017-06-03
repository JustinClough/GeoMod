#ifndef GEOMOD_HPP
#define GEOMOD_HPP

// Simmetrix Headers
#include "SimUtil.h"
#include "SimModel.h"
#include "SimAdvModel.h"

// Standard C++ Headers
#include <iostream>
#include <stdio.h>
#include <vector>

namespace GMD
{
  class mesh_helper_t
  {

  };

  class gmd_t
  {
    public:
      gmd_t( pMesh m, pGModel geom);
      ~gmd_t();
      void write_mesh( const char* filename);
      void write_model( const char* filename);
      pMesh get_mesh();
      pGModel get_model();

    private:
      void set_mesh(pMesh m);
      void set_model(pGModel geom);
      pMesh mesh;
      pGModel model;
  };

  pGModel create_2D_bar( double length, double width);

  void release_model(pGModel model);

  void release_mesh(pMesh mesh);

  void sim_start();

  void sim_end();
} //END namespace GMD

#endif // GEOMOD_HPP

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
  pGModel create_2D_bar( double length, double width);

  void write_mesh(pMesh& mesh, const char* filename);

  void write_model(pGModel& model, const char* filename);

  void release_model(pGModel model);

  void release_mesh(pMesh mesh);

  void sim_start();

  void sim_end();
} //END namespace GMD

#endif

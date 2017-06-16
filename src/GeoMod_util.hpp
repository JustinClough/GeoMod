#ifndef GEOMOD_UTIL_HPP
#define GEOMOD_UTIL_HPP

// Simmetrix Headers
#include "SimUtil.h"
#include "SimModel.h"
#include "SimAdvModel.h"
#include "MeshSim.h"
#include "SimPartitionedMesh.h"

// Standard C++ Headers
#include <cstdlib>
#include <iostream>

namespace GMD
{
  void sim_start( char* Sim_log_file_name, int argc, char** argv);

  void sim_end();
  pGModel create_cube(double length);
  pGModel create_2D_rectangle( double y_length, double x_width);
}
#endif

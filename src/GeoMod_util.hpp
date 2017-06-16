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
#include <vector>

namespace GMD
{
  void sim_start( char* Sim_log_file_name, int argc, char** argv);

  void sim_end();
}
#endif

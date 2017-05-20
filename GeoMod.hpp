#ifndef GEOMOD_HPP
#define GEOMOD_HPP

// Simmetrix Headers
#include "SimUtil.h"

namespace GMD
{

  void sim_start()
  {
    std::cout << "Starting Sim" << std::endl;
    SimUtil_start();
  }

  void sim_end()
  {
    std::cout << "Stopping Sim" << std::endl;
    SimUtil_stop();
  }
} //END namespace GMD

#endif

#include <GeoMod_util.hpp>

namespace GMD
{
  void sim_start( char* Sim_log_file_name, int argc, char** argv)
  {
    std::cout << "Starting Simmetrix" << std::endl;
    SimPartitionedMesh_start(&argc, &argv);
    Sim_logOn( Sim_log_file_name);
    SimUtil_start();
    Sim_readLicenseFile(0);
    SimModel_start();
    MS_init();
  }

  void sim_end()
  {
    std::cout << "Stopping Simmetrix" << std::endl;
    MS_exit();
    SimModel_stop();
    Sim_unregisterAllKeys();
    SimUtil_stop();
    Sim_logOff();
    SimPartitionedMesh_stop();
  }
}

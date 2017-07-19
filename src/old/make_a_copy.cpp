// Simmetrix Headers
#include <SimUtil.h>
#include <SimModel.h>
#include <SimAdvModel.h>
#include <MeshSim.h>
#include <SimPartitionedMesh.h>

// Standard Headers
#include <stdlib.h>
#include <iostream>

int main()
{
  // Init sim
  char log_file[] = "Sim_log_file";
  Sim_logOn( log_file);
  SimUtil_start();
  Sim_readLicenseFile(0);
  SimModel_start();

  // Load model "test2_cube.smd" from file.
  // The first NULL is for no native model. 
  //   The model was originally made using GeoSim Geometry.
  // The second NULL is to not report progress
  char fileName[] = "test2_cube.smd";
  pGModel Model_A = GM_load( fileName, NULL, NULL);

  // Check if loaded model is valid
  if( GM_isValid( Model_A, 1, NULL) == 1)
  {
    std::cout << "Model is valid" << std::endl;
  }
  else 
  {
    std::cout << "Model NOT valid" << std::endl;
  }

  // Check if loaded model is an assembly model
  if( GM_isAssemblyModel( Model_A) == 1)
  {
    std::cout << "Model is an Assembly Model" << std::endl;
  }
  else
  {
    std::cout << "Model NOT an Assembly Model" << std::endl;
  }

  std::cout << "Copying Model_A to Model_B" << std::endl;
  // Try to use GM_copy to make a copy of Model_A to Model_B
  // The NULL is to not report progress
  pGModel Model_B = GM_copy( Model_A, NULL);

  /* From here Model_B would be modified by inserting vertices
  *  and edges. Model_A remains unchanged. The idea is that
  *  a Model_B would be copied from Model_A in a loop 
  *  and modified differently each time through the loop.
  */ 

  // Free memory for Model_A and Model_B
  GM_release( Model_A);
  GM_release( Model_B);

  // Final sim
  SimModel_stop();
  Sim_unregisterAllKeys();
  SimUtil_stop();
  Sim_logOff();
  return 0;
}

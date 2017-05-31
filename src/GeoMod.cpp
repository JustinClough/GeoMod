// This File's header
#include "GeoMod.hpp"

// NOTE: All needed headers belong in GeoMod.hpp

namespace GMD
{
  pGModel create_2D_bar( double length, double width)
  {
    // Create an empty modeling space to work with
    pGModel model = GM_new();
    pGIPart part = GM_part(model);
    pGRegion outRegion = GIP_outerRegion(part);

    // Create model vertices
    pGVertex verts[4];
    double vert_xyz[4][3] ={{ 0.0, 0.0, 0.0},
                            { length, 0.0, 0.0},
                            { length, width, 0.0},
                            { 0.0, width, 0.0}}; 
    for(int i=0; i<4; i++)
    {
      verts[i] = GIP_insertVertexInRegion( part, vert_xyz[i], outRegion);
    }

    // Create model edges
    pCurve line;
    pGEdge edges[4];
    for(int i=0; i<4; i++)
    {
      pGVertex start = verts[i];
      pGVertex end = verts[(i+1)%4];
      line = SCurve_createLine(vert_xyz[i], vert_xyz[(i+1)%4]);
      edges[i] = GIP_insertEdgeInRegion( part, start, end, line, 1, outRegion);
    }

    // Create Face
    

    return model;
  }

  void write_model(pGModel model, const char* filename)
  {

    return;
  }

  void release_model(pGModel model)
  {
    GM_release(model);
    return;
  }

  void sim_start()
  {
    std::cout << "Starting Simmetrix" << std::endl;
    SimUtil_start();
    Sim_readLicenseFile(0);
    SimModel_start();
  }

  void sim_end()
  {
    std::cout << "Stopping Simmetrix" << std::endl;
    SimModel_stop();
    Sim_unregisterAllKeys();
    SimUtil_stop();
  }

} // END namespace GMD

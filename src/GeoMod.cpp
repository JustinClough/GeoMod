// This File's header
#include "GeoMod.hpp"

// NOTE: All needed headers belong in GeoMod.hpp

namespace GMD
{
  
  void release_mesh(pMesh mesh)
  {
    M_release(mesh);
    return;
  }

  void write_mesh(pMesh mesh, const char* filename)
  {
    std::cout << "MESH INFORMATION: "
      << "\nVertices: "<< M_numVertices(mesh)
      << "\nEdges: "<< M_numEdges(mesh)
      << "\nFaces: "<< M_numFaces(mesh)
      << "\nRegions: "<< M_numRegions(mesh) << std::endl;

    int writestat = M_write(mesh, filename, 0,0);
    if(writestat == 0)
    {
      std::cout << "Mesh " << filename << " written." << std::endl;
    }
    else
    {
      std::cout << "Mesh " << filename << "failed to be written." << std::endl;
    }
    return;
  }

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
    pSurface plane;
    pGFace face;
    int face_dirs[4] = {1,1,1,1};
    int loopDef[1] = {0};
    plane = SSurface_createPlane(vert_xyz[0], vert_xyz[1], vert_xyz[2]);
    std::cout << "Created Plane" << std::endl;
    face = GIP_insertFaceInRegion(part, 4, edges, face_dirs, 1, loopDef, plane, 1, outRegion);

    std::cout << "Inserted Face" << std::endl;

    return model;
  }

  void write_model(pGModel& model, const char* filename)
  {
    std::cout << "MODEL INFORMATION: "
      << "\nVertices: "<< GM_numVertices(model)
      << "\nEdges: "<< GM_numEdges(model)
      << "\nFaces: "<< GM_numFaces(model)
      << "\nRegions: "<< GM_numRegions(model) << std::endl;

    int writestat = GM_write(model, filename, 0,0);
    if(writestat == 0)
    {
      std::cout << "Model " << filename << " written." << std::endl;
    }
    else
    {
      std::cout << "Model " << filename << "failed to be written." << std::endl;
    }

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

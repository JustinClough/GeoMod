// This File's Header
#include <GeoMod_util.hpp>
// NOTE: All needed headers belong in GeoMod_util.hpp

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
  pGModel create_cube(double length)
  {
    pGModel model;
    pGIPart part;
    pGRegion outerRegion;
    pGVertex vertices[8]; // array to store the returned model vertices
    pGEdge edges[12]; // array to store the returned model edges
    double hl = length/2;
    model = GM_new();
    part = GM_part(model);
    outerRegion = GIP_outerRegion(part);

    double vert_xyz[8][3] = 
    { {-hl,-hl,-hl},
      { hl,-hl,-hl},
      { hl, hl,-hl},
      {-hl, hl,-hl}, 
      {-hl,-hl, hl},
      { hl,-hl, hl},
      { hl, hl, hl},
      {-hl, hl, hl} };

    int i;
    for(i=0; i<8; i++)
      vertices[i] = GIP_insertVertexInRegion(part,vert_xyz[i],outerRegion);

    pGVertex startVert, endVert;
    double point0[3],point1[3];  // xyz locations of the two vertices
    pCurve linearCurve;

    // First, the bottom edges
    for(i=0; i<4; i++) {
      startVert = vertices[i];
      endVert = vertices[(i+1)%4];
      GV_point(startVert, point0);
      GV_point(endVert, point1);
      linearCurve = SCurve_createLine(point0, point1);
      edges[i] = GIP_insertEdgeInRegion(part, startVert, endVert, linearCurve, 1, outerRegion);
    }

    // Now the side edges of the box
    for(i=0; i<4; i++) {
      startVert = vertices[i];
      endVert = vertices[i+4];
      GV_point(startVert,point0);
      GV_point(endVert,point1);
      linearCurve = SCurve_createLine(point0,point1);
      edges[i+4] = GIP_insertEdgeInRegion(part,startVert, endVert, linearCurve, 1, outerRegion);
    }

    // Finally the top edges 
    for(i=0; i<4; i++) {
      startVert = vertices[i+4];
      endVert = vertices[(i+1)%4+4];
      GV_point(startVert, point0);
      GV_point(endVert, point1);
      linearCurve = SCurve_createLine(point0, point1);
      edges[i+8] = GIP_insertEdgeInRegion(part, startVert, endVert, linearCurve, 1, outerRegion);
    }

    double corner[3], xPt[3], yPt[3];  // the points defining the surface of the face
    pGEdge faceEdges[4];               // the array of edges connected to the face
    int faceDirs[4];                   // the direction of the edge with respect to the face
    int loopDef[1] = {0};        
    pSurface planarSurface;

    // First the bottom face
    // Define the surface - we want the normal to point out of the box
    for(i=0; i<3; i++)
    {
      corner[i] = vert_xyz[1][i];
      xPt[i] = vert_xyz[0][i];
      yPt[i] = vert_xyz[2][i];
    }
    planarSurface = SSurface_createPlane(corner,xPt,yPt);
    // Define and insert the face into the outer "void" region
    for(i=0; i<4; i++) {
      faceDirs[i] = 0;
      faceEdges[i] = edges[3-i]; // edge order 3->2->1->0
    }
    GIP_insertFaceInRegion(part,4,faceEdges,faceDirs,1,loopDef,planarSurface,1,outerRegion);

    // Now the side faces of the box - each side face has the edges defined in the same way
    // for the first side face, the edge order is 0->5->8->4
    for(i=0; i<4; i++) {
      //Define surface such that normals all point out of the box
      for(int j=0; j<3; j++) {
        corner[j] = vert_xyz[i][j];      // the corner is the lower left vertex location
        xPt[j] = vert_xyz[(i+1)%4][j];   // the xPt the lower right vertex location
        yPt[j] = vert_xyz[i+4][j];       // the yPt is the upper left vertex location
      }
      planarSurface = SSurface_createPlane(corner,xPt,yPt);

      faceEdges[0] = edges[i];
      faceDirs[0] = 1;
      faceEdges[1] = edges[(i+1)%4+4];
      faceDirs[1] = 1;
      faceEdges[2] = edges[i+8];
      faceDirs[2] = 0;
      faceEdges[3] = edges[i+4];
      faceDirs[3] = 0;

      GIP_insertFaceInRegion(part,4,faceEdges,faceDirs,1,loopDef,planarSurface,1,outerRegion);
    }

    // Finally the top face of the box
    // Define the surface - we want the normal to point out of the box
    for(i=0; i<3; i++) {
      corner[i] = vert_xyz[4][i]; 
      xPt[i] = vert_xyz[5][i];   
      yPt[i] = vert_xyz[7][i];  
    }
    planarSurface = SSurface_createPlane(corner,xPt,yPt);
    // Define and insert the face
    for(i=0; i<4; i++) {
      faceDirs[i] = 1;
      faceEdges[i] = edges[i+8]; // edge order 8->9->10->11
    }
    // when this face is inserted, a new model region will automatically be created
    GIP_insertFaceInRegion(part,4,faceEdges,faceDirs,1,loopDef,planarSurface,1,outerRegion);

    return model;
  }

  pGModel create_2D_rectangle( double y_length, double x_width)
  {
    // Create an empty modeling space to work with
    pGModel model = GM_new();
    pGIPart part = GM_part(model);
    pGRegion outRegion = GIP_outerRegion(part);

    // Create model vertices
    pGVertex verts[4];
    double vert_xyz[4][3] ={{ 0.0, 0.0, 0.0},
      { y_length, 0.0, 0.0},
      { y_length, x_width, 0.0},
      { 0.0, x_width, 0.0}}; 
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
    face = GIP_insertFaceInRegion(part, 4, edges, face_dirs, 1, loopDef, plane, 1, outRegion);

    return model;
  }
}

// This File's header
#include "GeoMod.hpp"
// NOTE: All needed headers belong in GeoMod.hpp

namespace GMD
{
  void print_error(std::string message)
  {
    std::cout << "ERROR: " << message << "\n" ;
    std::abort();
  }

  void gmd_t::place_point( double* point, double local_refine)
  {
    pGIPart part = GM_part(model);
    pGRegion in_region;
    pGRegion region;

    bool placed = false;
    GRIter r_it = GM_regionIter( model);
    while((in_region = GRIter_next(r_it)) && !placed)
    {
      if(GR_containsPoint(in_region, point) == 1)
      { // Point is in region or on boundary
        region = in_region;
      }
      else if(GR_containsPoint(in_region, point) == 0)
      { // Point is not in region
        region = GIP_outerRegion( part);
      }
      else
      { // No possible solution
        print_error("CANNOT DETERMINE CLASSIFICATION FOR POINT PLACEMENT.");
      }

      pGVertex vert = GIP_insertVertexInRegion( part, point, region);
      placed = true;
    }
    GRIter_delete(r_it);

    if(!placed)
    { print_error("FAILED TO PLACE POINT");}
  
    return;
  }


  mesh_helper_t::mesh_helper_t ( pGModel geom)
  {
    mesh_order = 1;
    mesh_size = 0.1;
    grad_rate = 2.0;
    mesh = M_new( 0, geom);
    m_case = MS_newMeshCase( geom);

    if(m_case == NULL)
    { print_error("NULL m_case");}
    return;
  }

  mesh_helper_t::~mesh_helper_t()
  {
    MS_deleteMeshCase( m_case);
    release_mesh( mesh);
    return;
  }

  gmd_t::gmd_t (pGModel geom)
  {
    mesh_name = NULL;
    model_name = NULL;
    set_model( geom);
    return;
  }

  void gmd_t::set_mesh_name( char* name)
  {
    mesh_name = &name[0];
    return;
  }

  void gmd_t::set_model_name( char* name)
  {
    model_name = &name[0];
    return;
  }

  void gmd_t::verify_model( bool abort_on_fail)
  {
    if(GM_isValid(model, 0, 0))
    {
      return;
    }
    else if (abort_on_fail)
    { print_error("MODEL NOT VALID");}
    else
    { std::cout << "MODEL" << model_name << " NOT VALID" << std::endl;return;}
  }

  gmd_t::~gmd_t()
  {
    release_model(model);
    return;
  }

  void gmd_t::update_model( pGModel geom)
  {
    release_model(this->model);
    set_model(geom);
    return;
  }

  void gmd_t::set_mesh( pMesh m)
  { m_helper->mesh = m;}

  void gmd_t::set_model( pGModel geom)
  { 
    model = geom;
    mesh_helper_t mh (geom);
    m_helper = &mh;
  }

  pMesh gmd_t::get_mesh()
  { return m_helper->mesh; }

  pGModel gmd_t::get_model()
  { return model; }

  void release_mesh(pMesh mesh)
  {
    M_release(mesh);
    return;
  }

  char* gmd_t::get_mesh_name()
  {
    char* ans = &mesh_name[0];
    return ans;
  }

  char* gmd_t::get_model_name()
  {
    char* ans = &model_name[0];
    return ans;
  }

  void gmd_t::write_mesh()
  {
    if(mesh_name==NULL)
    { print_error("NO NAME ASSIGNED TO MESH");}
    pMesh mesh = get_mesh();
    std::cout << "MESH INFORMATION: "
      << "\nVertices: "<< M_numVertices(mesh)
      << "\nEdges: "<< M_numEdges(mesh)
      << "\nFaces: "<< M_numFaces(mesh)
      << "\nRegions: "<< M_numRegions(mesh) << std::endl;

    int writestat = M_write(mesh, mesh_name, 0,0);
    if(writestat == 0)
    {
      std::cout << "Mesh " << mesh_name << " written." << std::endl;
    }
    else
    {
      std::cout << "Mesh " << mesh_name << "failed to be written." << std::endl;
    }
    return;
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
    std::cout << "Created Plane" << std::endl;
    face = GIP_insertFaceInRegion(part, 4, edges, face_dirs, 1, loopDef, plane, 1, outRegion);

    std::cout << "Inserted Face" << std::endl;
    return model;
  }

  void insert_vertex_on_face( pGModel geom, double* point)
  {
    pGIPart part = GM_part(geom);
    pGRegion out_region = GIP_outerRegion(part);

    GFIter f_it = GIP_faceIter( part);
    GFIter_reset(f_it);
    pGFace face = GFIter_next(f_it);

    //GIP_insertVertexInRegion( part, point, out_region);
    GIP_insertVertexInFace(part, point, face);

    GFIter_delete(f_it);
    return;
  }

  void gmd_t::write_model( )
  {
    if(model_name==NULL)
    { print_error("NO NAME ASSIGNED TO MODEL");}
    if(GM_isValid(model, 0, 0))
    {
      std::cout << "MODEL INFORMATION: "
        << "\nVertices: "<< GM_numVertices(model)
        << "\nEdges: "<< GM_numEdges(model)
        << "\nFaces: "<< GM_numFaces(model)
        << "\nRegions: "<< GM_numRegions(model) << std::endl;

      int writestat = GM_write(model, model_name, 0,0);
      if(writestat == 0)
      {
        std::cout << "Model " << model_name << " written." << std::endl;
      }
      else
      {
        std::cout << "Model " << model_name << "failed to be written." << std::endl;
      }
    }
    else
    { print_error("MODEL NOT VALID"); }

    return;
  }

  pMesh gmd_t::create_mesh( )
  {
    pModelItem model_domain = GM_domain(model);
    MS_setMeshSize( m_helper->m_case, model_domain, 2, 1.0, 0);

    pSurfaceMesher surface_mesher = SurfaceMesher_new( m_helper->m_case, m_helper->mesh);
    SurfaceMesher_execute( surface_mesher, 0);
    pVolumeMesher volume_mesher = VolumeMesher_new( m_helper->m_case, m_helper->mesh);
    VolumeMesher_execute ( volume_mesher, 0);

    return m_helper->mesh;
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
    MS_init();
  }

  void sim_end()
  {
    std::cout << "Stopping Simmetrix" << std::endl;
    MS_exit();
    SimModel_stop();
    Sim_unregisterAllKeys();
    SimUtil_stop();
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

} // END namespace GMD

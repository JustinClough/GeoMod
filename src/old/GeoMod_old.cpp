// This File's header
#include "GeoMod.hpp"
// NOTE: All needed headers belong in GeoMod.hpp

static void sum_coords( double* p1, double* p2, double* sum)
{
  for(int i=0; i<3; i++)
  {
    sum[i] = p1[i] + p2[i];
  }
  return;
}

static void compare_coords( double* p1, double* p2, bool& areSame)
{
  areSame = true;
  for(int i=0; i<3; i++)
  {
    if( p1[i] != p2[i])
    {
      areSame = false;
    }
  }
  return;
}
    

static void subtract_coords( double* p1, double* p2, double* diff)
{
  for(int i=0; i<3; i++)
  {
    diff[i] = p2[i] - p1[i];
  }
  return;
}

static void magnitude( double* vec, double& mag)
{
  mag = 0;
  for(int i=0; i<3; i++)
  {
    mag+=(vec[i])*(vec[i]);
  }
  mag = sqrt(mag);
  return;
}

static void ave_coords( double* p1, double* p2, double* ans)
{
  sum_coords( p1, p2, ans);
  for(int i=0;i<3; i++)
  {
    ans[i]/=2;
  }
  return;
}

static void get_unit_vector( double* point1, double* point2, double* normal)
{
  double diff[] = {0.0, 0.0, 0.0};
  subtract_coords(point1, point2, diff);
  double mag = 0;
  magnitude( diff, mag);
  for(int i=0; i<3; i++)
  {
    normal[i] = diff[i]/mag;
  }

  return;
}

static void print_coords( double* point)
{
  std::cout << "(" ;
  for(int i=0;i<3;i++)
  {
    std::cout << point[i] << ", " ;
  }
  std::cout << "\b\b)\n";
  return;
}

double fRand(double fMin, double fMax)
{
  double f = (double)rand() / RAND_MAX;
  return fMin + f * (fMax - fMin);
}

void get_rand_line_ends(double** line_ends, double length, double* range)
{
  double closeness = 0.9;
  double max = range[0]*closeness;
  double in_max = max - length;
  double min = range[1]*closeness;
  double in_min = min -length;
  double dir[] = {0.0, 0.0, 0.0};
  for(int i=0; i<3; i++)
  {
    line_ends[0][i] = fRand( in_min, in_max);
    dir[i] = fRand(-1.0, 1.0);
  }
  double origin[] = {0.0, 0.0, 0.0};
  double normal[] = {0.0, 0.0, 0.0};
  get_unit_vector(dir, origin, normal);
  *dir = *normal;
  for(int i=0; i<3; i++)
  {
    dir[i] *= length;
  }
  sum_coords(line_ends[0], dir, line_ends[1]);
  

  return;
}

namespace GMD
{
  void print_error(std::string message)
  {
    std::cout << "ERROR: " << message << "\n" ;
    std::abort();
  }

  int gmd_t::get_mesh_numVertsOnFaces()
  {
    int total = 0;
    VIter v_it = M_vertexIter(mesh);
    pVertex vertex;
    while((vertex = VIter_next(v_it)))
    {
      gType type = V_whatInType(vertex);
      if(type == Gface || type == Gedge)
      {
        total++;
      }
    }
    VIter_delete(v_it);

    return total;
  }

  void gmd_t::place_line( double** points, double local_refine, double refine_radius)
  {
    bool areSame = false;
    compare_coords( points[0], points[1], areSame);
    if( areSame)
    { print_error("CANNOT CREATE LINE OF ZERO LENGTH");}
    
    bool vert1OnFace = false;
    double* point1 = points[0];
    pGVertex vert1 = place_point( point1, local_refine, refine_radius, vert1OnFace);

    bool vert2OnFace = false;
    double* point2 = points[1];
    pGVertex vert2 = place_point( point2, local_refine, refine_radius, vert2OnFace);

    pGIPart part = GM_part(model);
    GRIter r_it = GM_regionIter( model);
    pGRegion in_region;
    bool placed = false;
    while(!placed && (in_region = GRIter_next(r_it)))
    {
      if( vert1 == vert2)
      { std::cout << "vert1 == vert2" << std::endl;}
      pCurve line = SCurve_createLine( point1, point2);
      GIP_insertEdgeInRegion( part, vert1, vert2, line, 1, in_region);
      placed = true;
    }
    GRIter_delete(r_it);
    if(refine_radius == 0.0)
    { 
      MS_addLineRefinement( m_case, local_refine, point1, point2);
    }
    else if(refine_radius > 0.0)
    {
      double center[] = {0.0, 0.0, 0.0};
      ave_coords( point1, point2, center);
      double normal[] = {0.0, 0.0, 0.0};
      get_unit_vector( point1, point2, normal);
      double length_vec[] = {0.0, 0.0, 0.0};
      subtract_coords( point1, point2, length_vec);
      double length = 0;
      magnitude( length_vec, length);

      MS_addCylinderRefinement( m_case, local_refine, refine_radius, length, center, normal);
    }
    else
    { print_error("BAD REFINE_RADIUS (must be equal to or greater than zero)");}

    return;
  }

  void gmd_t::verify_mesh( bool abort_on_fail )
  {
    int rank = PMU_rank();
    int np = PMU_size();
    pParMesh par_mesh;
    pPList partBndryEnts = PList_new();
    pPList entInfo = PList_new();
    pPList entCopyPtrs = PList_new();
    pPList gids = PList_new();
    pPList meshes = PList_new();
    PList_append( meshes, mesh);
    int m_rep = M_representation( mesh);
    par_mesh = PM_createFromMesh( model, m_rep, meshes, NULL, NULL, NULL);
    {
      for(int entType = 0; entType<3; entType++)
      {

      }
    }

    if(!PM_verify( par_mesh, 0, NULL))
    { print_error("BAD PARTITIONED MESH AFTER JOIN");}
     
    //apf::Mesh2* apf_mesh = apf::createMesh( par_mesh);
    apf::Mesh* apf_mesh = (apf::Mesh*) mesh;
    apf::verify( apf_mesh, abort_on_fail );
    std::cout << "HERE1" << std::endl;
    apf::destroyMesh( apf_mesh);
    std::cout << "HERE2" << std::endl;

    M_release( par_mesh);
    PList_delete( meshes);
    PList_delete( partBndryEnts);
    PList_delete( entInfo);
    PList_delete( entCopyPtrs);
    PList_delete( gids);
    
    return;
  }

  void gmd_t::count_face_loops()
  {
    pGIPart part = GM_part( model);
    GFIter f_it = GIP_faceIter( part);
    GFIter_reset(f_it);
    pGFace face;
    while(( face = GFIter_next(f_it)))
    {
      std::cout<< "Face Number = " << GEN_tag( face) << std::endl;
      std::cout<< "Number of Loops on Face = " << GF_numLoops(face) << std::endl;
    }
    
    return;
  }
  pGVertex gmd_t::place_point( double* point, double local_refine, double refine_radius, bool& onFace)
  {
    pGIPart part = GM_part(model);
    pGRegion region;

    bool vert_on_face = false;
    pGVertex vert_f;
    pGFace face;

    bool placed = false;
    GRIter r_it = GM_regionIter( model);
    pGRegion in_region;
    pGVertex vert;
    while(!placed && (in_region = GRIter_next(r_it)))
    {
      if(GR_containsPoint(in_region, point) == 1)
      { // Point is in region or on boundary of region
        region = in_region;
        vert_on_face = insert_vertex_on_face( model, point, vert_f, face);
        placed = vert_on_face;
        onFace = vert_on_face;
        if(!placed)
        {
          vert = GIP_insertVertexInRegion( part, point, region);
          placed = true;
        }
      }
      else if(GR_containsPoint(in_region, point) == 0)
      { // Point is not in region
        region = GIP_outerRegion( part);
        vert = GIP_insertVertexInRegion( part, point, region);
        placed = true;
      }
      else
      { // No possible solution
        print_error("CANNOT DETERMINE CLASSIFICATION FOR POINT PLACEMENT.");
      }
    }
    GRIter_delete(r_it);

    if(!placed)
    { print_error("FAILED TO PLACE POINT");}

    set_point_refine( point, local_refine, refine_radius);

    return vert;
  }

  void gmd_t::set_point_refine( double* point, double refine, double refine_radius)
  {
    MS_addPointRefinement( m_case, refine, point);
    if( refine_radius > 0.0)
    { 
      MS_addSphereRefinement( m_case, refine, refine_radius, point);
    }
    return;
  }

  void gmd_t::set_global_mesh_params( double order, double refine, double grad_rate_in)
  {
    g_mesh_set = true;
    m_order = order;
    m_g_refine = refine;
    grad_rate = grad_rate_in;
    return;
  }

  gmd_t::gmd_t (pGModel geom)
  {
    g_mesh_set = false;
    mesh_name = NULL;
    model_name = NULL;
    set_model( geom);
    set_mesh( );
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
    MS_deleteMeshCase(m_case);
    release_mesh(mesh);
    release_model(model);
    return;
  }

  void gmd_t::update_model( pGModel geom)
  {
    release_model(this->model);
    set_model(geom);
    return;
  }

  void gmd_t::set_mesh( )
  { 
    mesh = M_new(0, model);
    m_case = MS_newMeshCase ( model);
    return;
  }

  void gmd_t::set_model( pGModel geom)
  { 
    model = geom;
  }

  pMesh gmd_t::get_mesh()
  { return mesh; }

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

  void gmd_t::assign_model_name_to_mesh()
  {
    if(model_name == NULL)
    { print_error("NO NAME ASSIGNED TO MODEL");}
    mesh_name = model_name;
    int i = 0;
    char token = mesh_name[i];
    while(token != '.')
    {
      token = mesh_name[i];
      i++;
    }
    mesh_name[i+2] = 's';

    return;
  }

  void gmd_t::write_mesh()
  {
    if(mesh_name==NULL)
    { 
      assign_model_name_to_mesh();
    }
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
    face = GIP_insertFaceInRegion(part, 4, edges, face_dirs, 1, loopDef, plane, 1, outRegion);

    return model;
  }

  bool gmd_t::insert_vertex_on_face( pGModel geom, double* point, pGVertex vert, pGFace face)
  {
    pGIPart part = GM_part(geom);
    GFIter f_it = GIP_faceIter( part);
    GFIter_reset(f_it);
    bool foundFace = false;
    while( !foundFace && (face = GFIter_next(f_it)))
    {
      double closest[] = {0.0, 0.0, 0.0};
      double param[] = {0.0, 0.0};
      GF_closestPoint( face, point, closest, param);
      compare_coords(point, closest, foundFace);
    }
    GFIter_delete(f_it);

    if(foundFace)
    {
      vert = GIP_insertVertexInFace(part, point, face);
      return true;
    }
    else
    {
      return false;
    }
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

  pMesh gmd_t::create_mesh( bool abort_on_fail)
  {
    if(!g_mesh_set)
    { print_error("GLOBAL MESH PARAMETERS NOT SET");}

    pModelItem model_domain = GM_domain(model);
    MS_setMeshSize( m_case, model_domain, 2, m_g_refine, 0);
    MS_setGlobalSizeGradationRate( m_case, grad_rate);
    const unsigned char edges = 0x02;
    const unsigned char faces = 0x04;
    const unsigned char dimfilter = edges | faces;
    MS_ensureGFaceVertices( m_case, model_domain, dimfilter);

    pPList Plist;
    int selfInter = MS_checkMeshIntersections( mesh, Plist, NULL);
    if ( selfInter == 1)
    {
      std::cout << "Mesh is self intersecting.\n" ;
    }
    else if( selfInter == 0)
    {
      std::cout << "Mesh is not self intersecting.\n";
    }

    if(m_order == 2) 
      // order is assumed to be 1 and can only be set to 2 according to Simmetrix documentation
    { MS_setMeshOrder(m_case, m_order);}

    pSurfaceMesher surface_mesher = SurfaceMesher_new( m_case, mesh);
    SurfaceMesher_execute( surface_mesher, NULL);
    SurfaceMesher_delete(surface_mesher);

    pVolumeMesher volume_mesher = VolumeMesher_new( m_case, mesh);
    VolumeMesher_execute ( volume_mesher, 0);
    VolumeMesher_delete(volume_mesher);

    verify_mesh( );

    return mesh;
  }

  void release_model(pGModel model)
  {
    GM_release(model);
    return;
  }

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


} // END namespace GMD
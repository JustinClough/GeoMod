// This file's header
#include "GeoMod_model_helper.hpp"

namespace GMD
{
  model_helper_t::model_helper_t( pGModel& in_model)
  {
    model = in_model;
    Written = false;
    if(!isValid())
    {
      print_warning("Created gmd object on invalid model.");
    }
    part = GM_part( model);
    if( part == NULL)
    {
      print_error("GeoMod does not support assembly models.");
    }
    return;
  }

  model_helper_t::~model_helper_t()
  {
    // Assumes the user will delete the model
    //  when they are done with it.
    return;
  }

  void model_helper_t::release()
  {
    GM_release( model);
    return;
  }

  void model_helper_t::model_print()
  {
    std::cout << "Modeler says hello!" << std::endl;
    return;
  }

  bool model_helper_t::isValid()
  {
    if(model == NULL)
    {
      print_warning("Attempting to validate NULL model.");
    }
    if(GM_isValid( model, 1, NULL) == 1)
    { return true; }
    else
    { return false; }
  }

  void model_helper_t::write( std::string name)
  {
    name = name + ".smd";
    const char* name_c = name.c_str();
    if( !isValid())
    { print_warning("Attempting to write invalid model.");}

    std::cout << "MODEL INFORMATION: "
      << "\nVertices: "<< GM_numVertices(model)
      << "\nEdges: "<< GM_numEdges(model)
      << "\nFaces: "<< GM_numFaces(model)
      << "\nRegions: "<< GM_numRegions(model) << std::endl;

    int writestat = GM_write(model, name_c, 0,0);
    if(writestat == 0)
    {
      std::cout << "Model " << name << " written." << std::endl;
      Written = true;
    }
    else
    {
      std::cout << "Model " << name
        << " failed to be written." << std::endl;
    }

    return;
  }

  bool model_helper_t::point_on_dim( int dim, double coords[3])
  {
    double tol = GM_tolerance( model);
    bool answer = false;
    bool areSame = false;
    double closest[] = {0.0, 0.0, 0.0};
    if( dim == 0)
    {
      GVIter v_it = GM_vertexIter( model);
      pGVertex v;
      while (( v = GVIter_next( v_it)))
      {
        double tmp[3] = { 0.0, 0.0, 0.0};
        GV_point( v , tmp);
        compare_coords( coords, tmp, areSame, tol);
        if( areSame)
        {
          answer = true;
        }
      }
      GVIter_delete( v_it);
    }
    if( dim == 1)
    {
      GEIter e_it = GM_edgeIter( model);
      pGEdge e;
      while (( e = GEIter_next(e_it)))
      {
        GE_closestPoint(e, coords, closest, NULL);
        compare_coords(coords, closest, areSame, tol);
        if(areSame)
        {
          answer = true;
        }
      }
      GEIter_delete( e_it);
    }
    else if (dim == 2)
    {
      GFIter f_it = GM_faceIter( model);
      pGFace f;
      while (( f = GFIter_next(f_it)))
      {
        GF_closestPoint(f, coords, closest, NULL);
        compare_coords(coords, closest, areSame, tol);
        if(areSame)
        {
          answer = true;
        }
      }
      GFIter_delete( f_it);
    }
    else if (dim == 3)
    {
      print_warning("point_on_dim does not support this dimension");
    }
    return answer;
  }

  int model_helper_t::point_location(double coords[3])
  {
    int answer = -1;
    // Want to find classification on lowest gEnt dim
    // start from top work down, overwrite old answer
    for(int i=3; i>(-1); i--)
    {
      if (i<3)
      {
        if(point_on_dim( i, coords))
        {
          answer = i;
        }
      }
      else
      {
        GRIter r_it = GM_regionIter( model);
        pGRegion region;
        while(( region= GRIter_next(r_it)))
        {
          if(GR_containsPoint( region, coords) == 0)
          { // Point is in the void region (spooky!)
            GRIter_delete( r_it);
            return -1;
          }
          else if (GR_containsPoint( region, coords) ==1)
          {
            answer = 3;
          }
          else
          { print_error("Point placement not possible.");}
        }
        GRIter_delete( r_it);
      }
    }
    return answer;
  }

  bool model_helper_t::PointOnFace( double coords[3], pGFace face)
  {
    double tol = GM_tolerance( model);
    bool ans = false;
    double cp[] = {0.0, 0.0, 0.0};
    GF_closestPoint( face, coords, cp, NULL);
    compare_coords(coords, cp, ans, tol);
    return ans;
  }

  bool model_helper_t::PointOnEdge( double coords[3], pGEdge edge)
  {
    double tol = GM_tolerance( model);
    bool ans = false;
    double cp[] = {0.0, 0.0, 0.0};
    double para[] = {0.0, 0.0, 0.0};
    GE_closestPoint( edge, coords, cp, para);
    compare_coords(coords, cp, ans, tol);

    return ans;
  }

  void model_helper_t::put_point_outside( double coords[3], pGVertex& vert)
  {
    pGRegion out_region = GIP_outerRegion( part);
    vert = GIP_insertVertexInRegion( part, coords, out_region);
    return;
  }

  void model_helper_t::put_point_in_line( double coords[3], pGVertex& vert)
  {
    bool placed = false;
    GEIter e_it = GM_edgeIter( model);
    pGEdge edge;
    while ( !placed && (edge = GEIter_next( e_it)))
    {
      if (PointOnEdge(coords,edge))
      {
        double param = 0.0;
        GE_closestPoint( edge, coords, NULL, &param);
        vert = GM_splitEdge( edge, param);
        if(vert == NULL)
        {
          pPList vert_list = PList_new();
          vert_list = GE_vertices( edge);
          pGVertex tmp_vert;
          void* iter = 0;
          bool found = false;
          while((!found) && ( tmp_vert = (pGVertex) PList_next( vert_list, &iter)))
          {
            bool areSame = false;
            double tol = GM_tolerance( model);
            double conf_coords[3] = {0.0};
            GV_point( tmp_vert, conf_coords);
            compare_coords( coords, conf_coords, areSame, tol);
            if( areSame)
            {
              found = true;
              vert = tmp_vert;
            }
          }
          PList_delete( vert_list);
        }
        placed = true;
      }
    }
    GEIter_delete( e_it);

    if( !placed)
    {
      print_warning("Failed to place point on edge at");
      print_coords( coords);
    }
    return;
  }

  void model_helper_t::put_point_in_face( double coords[3], pGVertex& vert)
  {
    bool placed = false;
    GFIter f_it = GM_faceIter( model);
    pGFace face;
    while ( !placed && (face = GFIter_next( f_it)))
    {
      if (PointOnFace(coords,face))
      {
        vert = GIP_insertVertexInFace( part, coords, face);
        placed = true;
      }
    }
    GFIter_delete( f_it);

    if( !placed)
    {
      print_warning("Failed to place point in face at");
      print_coords( coords);
    }

    return;
  }

  void model_helper_t::put_point_in_region(
      double coords[3],
      pGVertex& vert)
  {
    bool placed = false;
    GRIter r_it = GM_regionIter( model);
    pGRegion region;
    while( !placed && ( region= GRIter_next(r_it)))
    {
      if (GR_containsPoint( region, coords) ==1)
      {
        vert = GIP_insertVertexInRegion( part, coords, region);
        placed = true;
      }
    }
    GRIter_delete( r_it);
    return;
  }

  bool model_helper_t::place_point(
      double coords[3],
      pGVertex& vert,
      bool abort_on_fail)
  {
    bool updateMesh = true;
    int location = point_location(coords);
    if( location == -1)
    {
      if(abort_on_fail)
      {
        print_error("Point outside of known regions.");
      }
      else
      {
        print_warning("Point outside of known regions.");
      }
      updateMesh = false;
      put_point_outside( coords, vert);
    }
    else if ( location == 0)
    {
      double tol = GM_tolerance( model);
      bool areSame = false;
      GVIter v_it = GM_vertexIter( model);
      pGVertex v;
      while (( v = GVIter_next( v_it)))
      {
        double tmp[3] = { 0.0, 0.0, 0.0};
        GV_point( v , tmp);
        compare_coords( coords, tmp, areSame, tol);
        if( areSame)
        {
          vert = v;
        }
      }
      GVIter_delete( v_it);
    }
    else if ( location == 1)
    {
      put_point_in_line( coords, vert);
    }
    else if ( location == 2)
    {
      put_point_in_face( coords, vert);
    }
    else if ( location == 3)
    {
      put_point_in_region( coords, vert);
    }
    return updateMesh;
  }

  void model_helper_t::unpack_vector_spline_points(
      std::vector<double*> vec,
      double* x)
  {
    int pos = 0;
    double* tmp = vec[0];
    for( int i=0; i<(int)vec.size(); i++)
    {
      tmp = vec[i];
      for( int j=0; j<3; j++)
      {
        pos = j+3*i;
        x[pos] = tmp[j];
      }
    }
    return;
  }

  void model_helper_t::unpack_vector( std::vector<double> vec, double* x)
  {
    for (int i=0; i<(int)vec.size(); i++)
    {
      x[i] = vec[i];
    }
    return;
  }

  void model_helper_t::create_curve(
      int order,
      std::vector<double*> points,
      std::vector<double> knots,
      std::vector<double> weights,
      pCurve& curve)
  {
    int num_points = (int)points.size();
    bool weightLess = false;
    if((int)weights.size() == 1 && weights[0] == 0.0)
    { weightLess = true;}

    double u_points[num_points*3] = {0.0};
    double u_knots[(int)knots.size()] = {0.0};
    double un_weights[(int)weights.size()] = {0.0};
    unpack_vector_spline_points( points, u_points);
    unpack_vector( knots, u_knots);

    if(weightLess)
    {
      curve = SCurve_createBSpline(
        order, num_points, u_points, u_knots, NULL);
    }
    else
    {
      unpack_vector( weights, un_weights);
      curve = SCurve_createBSpline(
        order, num_points, u_points, u_knots, un_weights);
    }
    return;
  }

  bool model_helper_t::PointsOnSameFace( std::vector<double*> points)
  {
    bool onFace = true;
    for( int i=0; i<(int)points.size(); i++)
    { // Check if all points are on any face
      if( !point_on_dim( 2, points[i]))
      {
        onFace = false;
      }
    }

    if( onFace)
    {
      bool answer = false;
      pGFace face;
      pGFace conFace = NULL;
      GFIter f_it = GM_faceIter( model);
      while(( face = GFIter_next(f_it)))
      {
        for(int i=0; i<(int)points.size(); i++)
        {
          if( (i==0) && PointOnFace( points[i], face))
          {
            conFace = face;
          }
          else if( PointOnFace( points[i], face))
          {
            if( face == conFace)
            { answer = true; }
            else
            { answer = false; }
          }
        }
      }
      GFIter_delete(f_it);
      return answer;
    }
    else
    {
      return false;
    }
  }

  void model_helper_t::create_edge(
      std::vector<double*> points,
      pCurve& curve,
      pGEdge& edge)
  {
    double* start_point = points[0];
    pGVertex start_vert = NULL;
    if( start_point == NULL)
    { print_error(" start_point is NULL ");}
    place_point( start_point, start_vert, false);

    double* end_point = points[(int)points.size()-1];
    pGVertex end_vert = NULL;
    place_point( end_point, end_vert, false);

    // Assumes a one region, one part model
    pGIPart part = GM_part( model);
    GRIter r_it = GM_regionIter( model);
    pGRegion region = GRIter_next( r_it);

    if( part == NULL)
    { print_error("Part is NULL");}
    if( start_vert == NULL)
    { print_error("start_vert is NULL");}
    if( end_vert == NULL)
    { print_error("end_vert is NULL");}
    if( curve == NULL)
    { print_error("curve is NULL");}
    if( region == NULL)
    { print_error("region is NULL");}

    edge = GIP_insertEdgeInRegion(
        part, start_vert, end_vert, curve, 1, region);
    GRIter_delete( r_it);

    bool onSameFace = PointsOnSameFace( points);
    if( onSameFace)
    {
      pGFace face;
      GFIter f_it = GM_faceIter( model);
      bool found = false;
      while( !found && (face = GFIter_next(f_it)))
      {
        if(PointOnFace(points[0], face))
        { found = true; }
      }
      GFIter_delete( f_it);
      pGFace new_faces[2] = {NULL, NULL};
      GM_insertEdgeOnFace( face, edge, new_faces);
    }

    return;
  }

  void model_helper_t::place_edge(
      int order,
      std::vector<double*> points,
      std::vector<double> knots,
      std::vector<double> weights,
      pGEdge& edge)
  {
    pCurve curve;
    create_curve( order, points, knots, weights, curve);
    create_edge( points, curve, edge);
    return;
  }

  void model_helper_t::place_surface_by_spline(
      int u_order,
      int v_order,
      int u_num,
      int v_num,
      int periodicity,
      std::vector<double*> points,
      std::vector<double> u_knots,
      std::vector<double> v_knots,
      std::vector<double> weights,
      pGFace& face)
  {
    pSurface surf;
    std::vector<pGEdge> edges;

    create_surface(
        u_order, v_order, u_num, v_num, periodicity,
        points, u_knots, v_knots, weights, surf, edges);
    create_face( surf, edges, face);
    return;
  }

  void model_helper_t::unpack_bounding_edges(
      std::vector<pGEdge>& edges,
      pGEdge* bounding_edges)
  {
    for( int i=0; i<(int)edges.size(); i++)
    {
      bounding_edges[i] = edges[i];
    }
    return;
  }

  void model_helper_t::create_face(
      pSurface& surface,
      std::vector<pGEdge>& edges,
      pGFace& face)
  {

    pGIPart part = GM_part( model);
    int numEdges = (int)edges.size();
    pGEdge bounding_edges[numEdges] = {NULL};
    unpack_bounding_edges( edges, bounding_edges);
    int dirs[numEdges] = {0};
    for( int i = 0; i< numEdges; i++)
    {
      dirs[i] = 1;
    }
    int numLoops = 1;
    int indLoop[numLoops] = {0};
    for( int i = 0; i< numLoops; i++)
    {
      indLoop[i] = 0;
    }
    int normal = 1;

    GRIter r_it = GM_regionIter( model);
    pGRegion region = GRIter_next( r_it);

    face = GIP_insertFaceInRegion(
        part, numEdges, bounding_edges, dirs,
        numLoops, indLoop, surface, normal, region);

    GRIter_delete( r_it);
    return;
  }

  void model_helper_t::create_bounding_edges(
      int u_order,
      int v_order,
      int u_num,
      int v_num,
      std::vector<double*> points,
      std::vector<double> u_knots,
      std::vector<double> v_knots,
      std::vector<double> weights,
      std::vector<pGEdge>& edges)
  {
    // Since GeomSim needs splines surface to be four sided
    // and have a regular control point spacing (X by Y),
    // the edges need can be infered from the surface points

    int N = u_num;
    int M = v_num;
    pGEdge tmp_edge0;
    bool weightLess = false;
    if((int)weights.size() == 1 && weights[0] == 0.0)
    { weightLess = true;}

    std::vector<double*> edge_points;
    std::vector<double> edge_weights;
    for( int n = 0; n<N; n++)
    {
      edge_points.push_back( points[n] );
      if(!weightLess)
      {
        edge_weights.push_back( weights[n]);
      }
    }

    if(weightLess)
    {
      edge_weights.push_back( 0.0);
    }
    place_edge( u_order, edge_points, u_knots, edge_weights, tmp_edge0);
    edges.push_back(tmp_edge0);
    edge_points.clear();

    pGEdge tmp_edge1;
    for( int m=0; m<M; m++)
    {
      int ind = (N-1)+m*N;
      edge_points.push_back( points[ind] );
      if(!weightLess)
      {
        edge_weights.push_back( weights[ind]);
      }
    }

    place_edge( v_order, edge_points, v_knots, edge_weights, tmp_edge1);
    edges.push_back(tmp_edge1);

    edge_points.clear();

    pGEdge tmp_edge2;
    for( int n=(N-1); n>(-1); n--)
    {
      int ind = n+(M-1)*N;
      edge_points.push_back( points[ind] );
      if(!weightLess)
      {
        edge_weights.push_back( weights[ind]);
      }
    }

    place_edge( u_order, edge_points, u_knots, edge_weights, tmp_edge2);
    edges.push_back(tmp_edge2);
    edge_points.clear();

    pGEdge tmp_edge3;
    for( int m=(M-1); m>(-1); m--)
    {
      int ind = N*m;
      edge_points.push_back( points[ind] );
      if(!weightLess)
      {
        edge_weights.push_back( weights[ind]);
      }
    }

    place_edge( v_order, edge_points, v_knots, edge_weights, tmp_edge3);
    edges.push_back(tmp_edge3);

    edge_points.clear();
    edge_weights.clear();

    return;
  }

  void model_helper_t::create_surface(
      int u_order,
      int v_order,
      int u_num,
      int v_num,
      int periodicity,
      std::vector<double*> points,
      std::vector<double> u_knots,
      std::vector<double> v_knots,
      std::vector<double> weights,
      pSurface& surface,
      std::vector<pGEdge>& edges)
  {
    int u_per = 0;
    int v_per = 0;
    if (periodicity == 0)
    { // No chages from default
    }
    else if (periodicity == 1)
    {
      u_per = 1;
    }
    else if (periodicity == 2)
    {
      v_per = 1;
    }
    else if (periodicity == 3)
    {
      u_per = 1;
      v_per = 1;
    }

    bool weightLess = false;
    if((int)weights.size() == 1 && weights[0] == 0.0)
    { weightLess = true;}

    int num_points = (int)points.size();

    double unp_u_knots[(int)u_knots.size()] = {0.0};
    unpack_vector( u_knots, unp_u_knots);

    double unp_v_knots[(int)v_knots.size()] = {0.0};
    unpack_vector( v_knots, unp_v_knots);

    double all_points[3*num_points] = {0.0};
    unpack_vector_spline_points( points, all_points);

    create_bounding_edges(
        u_order, v_order, u_num, v_num,
        points, u_knots, v_knots, weights, edges);

    if(weightLess)
    {
      surface = SSurface_createBSpline(
          u_order, v_order,
          u_num, v_num,
          u_per, v_per,
          all_points, NULL,
          unp_u_knots, unp_v_knots);
    }
    else
    {
      double unp_weights[num_points] = {0.0};
      unpack_vector( weights, unp_weights);
      surface = SSurface_createBSpline(
          u_order, v_order,
          u_num,   v_num,
          u_per,   v_per,
          all_points, unp_weights,
          unp_u_knots, unp_v_knots);
    }
    return;
  }

  bool model_helper_t::isWritten()
  { return Written;}
}

// This files's header
#include "GeoMod_gmd_t.hpp"

namespace GMD
{
  gmd_t::gmd_t( pGModel in_model, int numParts)
  {
    panicStatus = true;
    modeler = new model_helper_t( in_model);
    mesher = new mesh_helper_t ( in_model, numParts);
    return;
  }

  gmd_t::gmd_t( std::string fileName, int numParts)
  {
    panicStatus = true;
    const char* name = fileName.c_str();
    pGModel tmp = GM_load( name, NULL, NULL);
    modeler = new model_helper_t ( tmp);
    mesher = new mesh_helper_t ( tmp, numParts);
    return;
  }

  gmd_t::~gmd_t()
  {
    delete modeler;
    delete mesher;
    return;
  }

  void gmd_t::verify_model()
  {
    if(!(modeler->isValid()))
    { print_warning("Model Not Valid. ");}
    return;
  }

  void gmd_t::release_model()
  {
    modeler->release();
    return;
  }

  pGModel gmd_t::get_model()
  {
    return modeler->model;
  }

  pMesh gmd_t::get_mesh()
  {
    if(mesher->isPar)
    { print_error("Cannot retrieve parallel mesh with this method.");}
    return mesher->mesh;
  }

  void gmd_t::release_mesh()
  {
    mesher->release();
    return;
  }

  void gmd_t::set_abort_on_fail( bool abort_on_fail)
  {
    panicStatus = abort_on_fail;
    return;
  }

  void gmd_t::test_printers()
  {
    modeler->model_print();
    mesher->mesh_print();
    return;
  }

  void gmd_t::place_point( double coords[3], double refine, double radius, pGVertex& vert)
  {
    bool updateMesh = modeler->place_point( coords, vert, panicStatus);
    if( updateMesh)
    {
      mesher->place_point( coords, refine, radius, panicStatus);
    }

    return;
  }

  void gmd_t::set_name( std::string file_name)
  {
    name = file_name;
    return;
  }

  void gmd_t::check_spline_params(
      int order,
      std::vector<double*> points,
      std::vector<double> knots,
      std::vector<double> weights)
  {
    if( order <=1)
    { print_error("Spline order too low. Must be polnomial order + 1.");}
    if( (int)points.size() < order)
    { print_error("Spline order too high for number of given control points.");}
    if( ((int)points.size()+order) != (int)knots.size())
    { print_error("Mismatch between knot vector size and sum of order with control point size.");}

    for( int i=0; i<(int)knots.size(); i++)
    {
      double tmp1 = knots[i];
      if( i<((int)knots.size() -1) )
      {
        double tmp2 = knots[i+1];
        if (tmp2 < tmp1)
        { print_error("Knots must be in accending order");}
      }
      if( i<order && tmp1 != 0.0)
      { print_error("First order_number of knots must be zero.");}
      if( i>((int)knots.size()-order) && tmp1 != 1.0)
      { print_error("Last order_number of knots must be one.");}

      if( tmp1 > 1.0 || tmp1 < 0.0)
      {
        print_error("Knots must satisfy 0.0<=k[i]<=1.0 .");
      }
    }

    if( (int) weights.size() != 1 && (int) weights.size() != (int) points.size())
    { print_error("Mismatch between weight vector size and number of points.");}
    return;
  }

  void gmd_t::place_edge_spline(
      int order,
      std::vector<double*> points,
      std::vector<double> knots,
      std::vector<double> weights,
      double refine,
      pGEdge& edge)
  {
    check_spline_params( order, points, knots, weights);
    modeler->place_edge( order, points, knots, weights, edge);
    if( refine > 0.0)
    { mesher->refine_edge( refine, edge);}
    return;
  }

  void gmd_t::place_edge_line(
      double* start,
      double* end,
      double refine,
      pGEdge& edge)
  {
    int order = 2;

    std::vector<double*> points;
    points.push_back( start);
    points.push_back( end);

    std::vector<double> knots;
    knots.push_back( 0.0);
    knots.push_back( 0.0);
    knots.push_back( 1.0);
    knots.push_back( 1.0);

    std::vector<double> weights;
    weights.push_back( 0.0);

    place_edge_spline( order, points, knots, weights, refine, edge);
    return;
  }

  void gmd_t::write_model()
  {
    modeler->write( name);
    return;
  }

  void gmd_t::create_mesh()
  {
    if(modeler->isValid())
    {
      mesher->create();
    }
    else if( (modeler->isValid()) && !panicStatus)
    {
      print_warning("Attempting to mesh invalid model.");
    }
    else
    {
      print_error("Invalid model. No mesh created");
    }
    return;
  }

  pParMesh gmd_t::get_par_mesh()
  {
    if( !(mesher->isPar))
    { print_error("Cannot retrieve serial mesh with this method.");}
    return mesher->parMesh;
  }

  void gmd_t::verify_mesh()
  {
    if(!mesher->isValid())
    {
      if(panicStatus)
      { print_error("Mesh Not Valid.");}
      else if( !panicStatus)
      { print_warning("Mesh Not Valid.");}
    }
    return;
  }

  void gmd_t::write_mesh()
  {
    if(modeler->isWritten())
    { mesher->write( name); }
    else
    { print_error("Model must be written before mesh."); }
    return;
  }

  void gmd_t::set_global_mesh_params( int order_in, double refine_in, double grad_rate_in)
  {
    mesher->set_global( order_in, refine_in, grad_rate_in);
    return;
  }

  void gmd_t::place_surface_by_spline(
      int u_order,
      int v_order,
      int u_num,
      int v_num,
      int periodicity,
      std::vector<double*> points,
      std::vector<double> u_knots,
      std::vector<double> v_knots,
      std::vector<double> weights,
      double refine,
      pGFace& face)
  {
    check_surface_params(
        u_order, v_order, u_num, v_num, periodicity, points, u_knots, v_knots, weights);

    modeler->place_surface_by_spline(
        u_order, v_order, u_num, v_num, periodicity, points, u_knots, v_knots, weights, face);

    mesher->refine_face( refine, face);

    return;
  }

  void gmd_t::check_surface_params(
      int u_order,
      int v_order,
      int u_num,
      int v_num,
      int periodicity,
      std::vector<double*> points,
      std::vector<double> u_knots,
      std::vector<double> v_knots,
      std::vector<double> weights)
  {
    if( periodicity < 0 || periodicity > 4)
    { print_error("Bad periodicity.");}

    if( u_order<= 1 )
    { print_error("u_order too low.");}
    if( v_order<= 1 )
    { print_error("v_order too low.");}

    if( (int)points.size() != (u_num*v_num))
    { print_error("Mismatched number of declared and given control points.");}

    if( (int)weights.size() != 1 &&  (int)weights.size() != (u_num*v_num))
    { print_error("Mismatched number of weights and declared control points.");}
    if( (int)weights.size() == 1 && weights[0] != 0.0)
    { print_error("Bad weights vector.");}

    if( (int)u_knots.size() != (u_num+u_order))
    { print_error("Mismatch between number of u_knots and u_order+u_num.");}

    for( int i=0; i<(int)u_knots.size(); i++)
    {
      double tmp1 = u_knots[i];
      if( i<((int)u_knots.size() -1) )
      {
        double tmp2 = u_knots[i+1];
        if (tmp2 < tmp1)
        { print_error("Knots must be in accending u_order");}
      }
      if( i<u_order && tmp1 != 0.0)
      { print_error("First u_order_number of u_knots must be zero.");}
      if( i>((int)u_knots.size()-u_order) && tmp1 != 1.0)
      { print_error("Last u_order_number of u_knots must be one.");}

      if( tmp1 > 1.0 || tmp1 < 0.0)
      {
        print_error("Knots must satisfy 0.0<=k[i]<=1.0 .");
      }
    }

    if( (int)v_knots.size() != (v_num+v_order))
    { print_error("Mismatch between number of v_knots and v_order+v_num.");}

    for( int i=0; i<(int)v_knots.size(); i++)
    {
      double tmp1 = v_knots[i];
      if( i<((int)v_knots.size() -1) )
      {
        double tmp2 = v_knots[i+1];
        if (tmp2 < tmp1)
        { print_error("Knots must be in accending v_order");}
      }
      if( i<v_order && tmp1 != 0.0)
      { print_error("First v_order_number of v_knots must be zero.");}
      if( i>((int)v_knots.size()-v_order) && tmp1 != 1.0)
      { print_error("Last v_order_number of v_knots must be one.");}

      if( tmp1 > 1.0 || tmp1 < 0.0)
      {
        print_error("Knots must satisfy 0.0<=k[i]<=1.0 .");
      }
    }

    return;
  }

  void gmd_t::force_surf_quads()
  {
    mesher->force_surf_quad();
    return;
  }

}

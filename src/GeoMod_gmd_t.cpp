#include <GeoMod_gmd_t.hpp>

namespace GMD
{
  gmd_t::gmd_t( pGModel in_model)
  {
    panicStatus = true;
    modeler = new model_helper_t( in_model);
    mesher = new mesh_helper_t ( in_model);
    return;
  }

  gmd_t::~gmd_t()
  {
    delete modeler;
    delete mesher;
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

  void gmd_t::place_point( double coords[3], double refine, double radius)
  {
    pGVertex vert;
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

  void gmd_t::check_spline_params( int order, std::vector<double*> points, std::vector<double> knots, std::vector<double> weights)
  {
    if( order <=1)
    { print_error("Spline order too low. Must be polnomial order + 1.");}
    if( points.size() < order)
    { print_error("Spline order too high for number of given control points.");}
    if( (points.size()+order) != knots.size())
    { print_error("Mismatch between knot vector size and sum of order with control point size.");}

    for( int i=0; i<knots.size(); i++)
    {
      double tmp1 = knots[i];
      if( i<(knots.size() -1) )
      { 
        double tmp2 = knots[i+1];
        if (tmp2 < tmp1)
        { print_error("Knots must be in accending order");}
      }
      if( i<order && tmp1 != 0.0)
      { print_error("First order_number of knots must be zero.");}
      if( i>(knots.size()-order) && tmp1 != 1.0)
      { print_error("Last order_number of knots must be one.");}

      if( tmp1 > 1.0 || tmp1 < 0.0)
      { 
        print_error("Knots must satisfy 0.0<=k[i]<=1.0 .");
      }
    }
    return;
  }

  void gmd_t::place_edge( int order, std::vector<double*> points, std::vector<double> knots, std::vector<double> weights, double refine)
  {
    check_spline_params( order, points, knots, weights);
    pGEdge edge;
    modeler->place_edge( order, points, knots, weights, edge);
    if( refine > 0.0)
    { mesher->refine_edge( refine, edge);}
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
}

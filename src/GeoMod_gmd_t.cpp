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

  void gmd_t::write_model()
  {
    modeler->write( name);
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
  void gmd_t::set_global_mesh_params( double order_in, double refine_in, double grad_rate_in)
  {
    mesher->set_global( order_in, refine_in, grad_rate_in);
    return;
  }
}

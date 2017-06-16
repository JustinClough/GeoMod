#include <GeoMod_gmd_t.hpp>

namespace GMD
{
  gmd_t::gmd_t( pGModel in_model)
  {
    panic = true;
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
    panic = abort_on_fail;
    return;
  }

  void gmd_t::test_printers()
  {
    modeler->model_print();
    mesher->mesh_print();
    return;
  }

  void gmd_t::place_point( double coords[3])
  {

    return;
  }

}

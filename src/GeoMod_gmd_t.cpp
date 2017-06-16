#include <GeoMod_gmd_t.hpp>

namespace GMD
{
  gmd_t::gmd_t( pGModel in_model)
  {
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
}

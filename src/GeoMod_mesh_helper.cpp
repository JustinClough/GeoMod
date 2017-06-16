#include <GeoMod_mesh_helper.hpp>
#include <GeoMod_printer.hpp>

namespace GMD
{
  mesh_helper_t::mesh_helper_t( pGModel in_model)
  {
    mesh = M_new( 0, in_model);
    return;
  }

  mesh_helper_t::~mesh_helper_t()
  {
    M_release( mesh);
    return;
  }

  void mesh_helper_t::mesh_print()
  {
    std::cout << "Mesher says hello!" << std::endl;
    return;
  }
  
}

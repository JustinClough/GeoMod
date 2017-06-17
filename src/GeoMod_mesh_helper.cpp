#include <GeoMod_mesh_helper.hpp>

namespace GMD
{
  mesh_helper_t::mesh_helper_t( pGModel in_model)
  {
    mesh = M_new( 0, in_model);
    m_case = MS_newMeshCase( in_model);
    return;
  }

  mesh_helper_t::~mesh_helper_t()
  {
    MS_deleteMeshCase(m_case);
    M_release( mesh);
    return;
  }

  void mesh_helper_t::mesh_print()
  {
    std::cout << "Mesher says hello!" << std::endl;
    return;
  }

  void mesh_helper_t::write( std::string name)
  {

    return;
  }

  void mesh_helper_t::place_point( double coords[3], double refine, double radius, bool abort_on_fail)
  {
    if (refine >0.0)
    {
      if (radius == 0.0)
      {
        MS_addPointRefinement( m_case, refine, coords); 
      }
      else if (radius > 0.0)
      {
        MS_addSphereRefinement( m_case, refine, radius, coords);
      }
      else
      { 
        print_error( "Refinement radius must be zero or greater");
      }
    }
    return;
  }

}

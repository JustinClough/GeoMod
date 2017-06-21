#include <GeoMod_mesh_helper.hpp>

namespace GMD
{
  mesh_helper_t::mesh_helper_t( pGModel in_model)
  {
    mesh = M_new( 0, in_model);
    m_case = MS_newMeshCase( in_model);
    order = 0.0;
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

  bool mesh_helper_t::isValid()
  {
    return true;
  }

  void mesh_helper_t::write( std::string name)
  {
    name = name + ".sms";
    const char* name_c = name.c_str();
    if( !isValid())
    { print_warning("Attempting to write invalid mesh.");}

    std::cout << "MESH INFORMATION: "
      << "\nVertices: "<< M_numVertices(mesh)
      << "\nEdges: "<< M_numEdges(mesh)
      << "\nFaces: "<< M_numFaces(mesh)
      << "\nRegions: "<< M_numRegions(mesh) << std::endl;

    int writestat = M_write(mesh, name_c, 0,0);
    if(writestat == 0)
    { 
      std::cout << "Mesh " << name << " written." << std::endl; 
    }
    else
    { std::cout << "Mesh " << name << " failed to be written." << std::endl; }
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

  void mesh_helper_t::set_global( double order_in, double refine_in, double grad_rate_in)
  {
    order = order_in;
    refine = refine_in;
    grad_rate = grad_rate_in;
    return;
  }
}

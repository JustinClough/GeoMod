// This file's header
#include "GeoMod_mesh_helper.hpp"

namespace GMD
{

  mesh_helper_t::mesh_helper_t( pGModel in_model, int numParts)
  {
    if( numParts == 1)
    {
      mesh = M_new( 0, in_model);
      isPar = false;
      parMesh = NULL;
    }
    else if ( numParts > 1)
    {
      mesh = NULL;
      parMesh = PM_new( 0, in_model, numParts);
      isPar = true;
    }
    m_case = MS_newMeshCase( in_model);
    globalSet = false;

    return;
  }

  mesh_helper_t::~mesh_helper_t()
  {
    // Assumes the user will delete the mesh and
    //  mesh case when they are done with them.
    return;
  }

  void mesh_helper_t::release()
  {
    release_case();
    if(isPar)
    {
      M_release( parMesh);
    }
    else
    {
      M_release( mesh);
    }
    return;
  }

  void mesh_helper_t::release_case()
  {
    MS_deleteMeshCase(m_case);
    return;
  }

  void mesh_helper_t::cast_serial_to_par()
  {
    if( isPar)
    { // Nothing to do. ParMesh aleady exists/ will exist
    }
    else
    {
      pGModel model = M_model( mesh);
      pPList mesh_list = PList_new();
      PList_append( mesh_list, mesh);
      parMesh = PM_createFromMesh(
          model,
          M_representation(mesh),
          mesh_list, NULL, NULL, NULL);
      PList_delete( mesh_list);
      M_release( mesh);
      isPar = true;
    }
    return;
  }

  void mesh_helper_t::mesh_print()
  {
    std::cout << "Mesher says hello!" << std::endl;
    return;
  }

  bool mesh_helper_t::isValid()
  { // Only validates serial meshes for now
    bool ans = false;
    pParMesh tmp;
    if (!isPar)
    {
      pGModel model = M_model( mesh);
      pPList mesh_list = PList_new();
      PList_append( mesh_list, mesh);
      tmp = PM_createFromMesh(
          model,
          M_representation(mesh),
          mesh_list, NULL, NULL, NULL);
      PList_delete( mesh_list);
    }
    else
    {
      tmp = parMesh;
    }

    int status = PM_verify(tmp, 0, NULL);
    if (status == 1)
    { ans = true;}

    if(!isPar)
    {
      M_release( tmp);
    }
    return ans;
  }

  void mesh_helper_t::create()
  {
    if(!globalSet)
    { print_error("Global Mesh Parameters not set.");}

    pModelItem domain;
    if(!isPar)
    {
      domain = GM_domain( M_model(mesh));
    }
    else
    {
      domain = GM_domain( M_model(parMesh));
    }

    MS_setMeshSize(m_case, domain, 2, refine, NULL);

    if( grad_rate > 0.0)
    { MS_setGlobalSizeGradationRate(m_case, grad_rate); }

    if( order == 2)
    { MS_setMeshOrder(m_case, order);}

    pSurfaceMesher surf = SurfaceMesher_new(m_case, mesh);
    SurfaceMesher_execute( surf, NULL);
    SurfaceMesher_delete( surf);

    pVolumeMesher vol = VolumeMesher_new( m_case, mesh);
    VolumeMesher_execute( vol, NULL);
    VolumeMesher_delete(vol);

    return;
  }

  void mesh_helper_t::print_mesh_info()
  {
    int partCount = 0;
    pMesh tmp = NULL;
    if(!isPar)
    {
      partCount = 1;
      tmp = mesh;
    }
    else
    {
      partCount = PM_totalNumParts( parMesh);
    }
    for( int i = 0; i < partCount; ++i)
    {
      if(isPar)
      {
        tmp = PM_mesh( parMesh, i);
      }
      std::cout << "MESH INFORMATION of part: " << i
        << "\nVertices: "<< M_numVertices(tmp)
        << "\nEdges: "<< M_numEdges(tmp)
        << "\nFaces: "<< M_numFaces(tmp)
        << "\nRegions: "<< M_numRegions(tmp) << std::endl;
    }
    return;
  }

  void mesh_helper_t::write( std::string name)
  {
    std::string tmp_name = name + ".sms";
    const char* name_c = tmp_name.c_str();
    if( !isValid())
    { print_warning("Attempting to write invalid mesh.");}

    print_mesh_info();

    int writestat = 1;
    if(!isPar)
    {
      writestat = M_write(mesh, name_c, 0,0);
    }
    else
    {
      PM_write( parMesh, name_c, NULL);
      // Since no status is reported, we'll
      // just assume everything went well
      // if we made it this far.
      writestat = 0;
    }

    if(writestat == 0)
    {
      std::cout << "Mesh " << name << " written." << std::endl;
    }
    else
    { std::cout << "Mesh " << name << " failed to be written." << std::endl; }

    return;
  }

  void mesh_helper_t::refine_vertex( double refine, pGVertex vert)
  {
    MS_setMeshSize(m_case, vert, 2, refine, NULL);
    return;
  }

  void mesh_helper_t::place_point(
      double coords[3],
      double refine,
      double radius,
      bool abort_on_fail)
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
      else if(abort_on_fail)
      {
        print_error( "Refinement radius must be zero or greater");
      }
      else
      {
        print_warning( "Refinement radius must be zero or greater");
      }
    }
    return;
  }

  void mesh_helper_t::set_global( int order_in, double refine_in, double grad_rate_in)
  {
    globalSet = true;
    order = order_in;
    refine = refine_in;
    grad_rate = grad_rate_in;
    return;
  }

  void mesh_helper_t::force_surf_quad()
  {
    MS_setSurfaceMeshType( m_case, GM_domain( M_model(mesh)), 3);
    return;
  }

  void mesh_helper_t::refine_edge( double refine, pGEdge edge)
  {
    MS_setMeshSize(m_case, edge, 2, refine, NULL);
    return;
  }

  void mesh_helper_t::refine_face( double refine, pGFace face)
  {
    MS_setMeshSize(m_case, face, 2, refine, NULL);
    return;
  }
}

#include <GeoMod_model_helper.hpp>

namespace GMD
{
  model_helper_t::model_helper_t( pGModel in_model)
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
    { std::cout << "Model " << name << " failed to be written." << std::endl; }

    return;
  }

  bool model_helper_t::point_on_dim( int dim, double coords[3])
  {
    bool answer = false;
    bool areSame = false;
    double closest[] = {0.0, 0.0, 0.0};
    if( dim == 1)
    {
      GEIter e_it = GM_edgeIter( model);
      pGEdge e;
      while (( e = GEIter_next(e_it)))
      {
        GE_closestPoint(e, coords, closest, NULL);
        compare_coords(coords, closest, areSame);
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
        compare_coords(coords, closest, areSame);
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
    int answer = 0;
    // Want to find classification on lowest gEnt dim
    // start from top work down, overwrite old answer
    for(int i=3; i>0; i--) 
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
            return 0;
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
    bool ans = false;
    double cp[] = {0.0, 0.0, 0.0};
    GF_closestPoint( face, coords, cp, NULL);
    compare_coords(coords, cp, ans);
    return ans;
  }

  void model_helper_t::put_point_outside( double coords[3], pGVertex vert)
  {
    pGRegion out_region = GIP_outerRegion( part);
    vert = GIP_insertVertexInRegion( part, coords, out_region);
    return;
  }

  void model_helper_t::put_point_in_line( double coords[3], pGVertex vert)
  {
    return;
  }
  
  void model_helper_t::put_point_in_face( double coords[3], pGVertex vert)
  {
    bool placed = false;
    GFIter f_it = GM_faceIter( model);
    pGFace face;
    while (( face = GFIter_next( f_it)))
    {
      if (PointOnFace(coords,face))
      {
        vert = GIP_insertVertexInFace( part, coords, face);
        placed = true;
      }
    }
    GFIter_delete( f_it);
    
    return;
  }

  void model_helper_t::put_point_in_region( double coords[3], pGVertex vert)
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

  bool model_helper_t::place_point( double coords[3], pGVertex vert, bool abort_on_fail)
  {
    bool updateMesh = true;
    int location = point_location(coords);
    if( location == 0)
    {
      print_warning("Point outside of known regions.");
      updateMesh = false;
      put_point_outside( coords, vert);
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

  bool model_helper_t::isWritten()
  { return Written;}
}

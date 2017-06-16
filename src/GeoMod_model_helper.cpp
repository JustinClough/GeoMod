#include <GeoMod_model_helper.hpp>
#include <GeoMod_printer.hpp>

namespace GMD
{
  model_helper_t::model_helper_t( pGModel in_model)
  {
    model = in_model;
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

  int model_helper_t::point_location(double coords[3])
  {
    return 0;
  }

  void model_helper_t::put_point_outside( double coords[3], pGVertex vert)
  {
    return;
  }

  void model_helper_t::put_point_in_line( double coords[3], pGVertex vert)
  {
    return;
  }
  
  void model_helper_t::put_point_in_face( double coords[3], pGVertex vert)
  {
    return;
  }

  void model_helper_t::put_point_in_region( double coords[3], pGVertex vert)
  {
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
}

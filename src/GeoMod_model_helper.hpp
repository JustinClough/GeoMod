#ifndef GEOMOD_MODEL_HELPER_HPP
#define GEOMOD_MODEL_HELPER_HPP

#include <GeoMod_SIM.hpp>

namespace GMD
{
  class model_helper_t
  {
    friend class gmd_t;
    private:
      model_helper_t (pGModel in_model);
      ~model_helper_t();
      pGModel model;
      void model_print();

      bool place_point( double coords[3], pGVertex vert, bool abort_on_fail);
      bool point_on_dim( int dim, double coords[3]);
      int point_location( double coords[3]);
      void put_point_outside( double coords[3], pGVertex vert);
      void put_point_in_line( double coords[3], pGVertex vert);
      void put_point_in_face( double coords[3], pGVertex vert);
      void put_point_in_region( double coords[3], pGVertex vert);

  };

}

#endif

#ifndef GEOMOD_MODEL_HELPER_HPP
#define GEOMOD_MODEL_HELPER_HPP

#include <GeoMod_SIM.hpp>
#include <GeoMod_printer.hpp>
#include <GeoMod_coords.hpp>
#include <string>

namespace GMD
{
  class model_helper_t
  {
    friend class gmd_t;
    private:
      // Util methods
      model_helper_t (pGModel in_model);
      ~model_helper_t();
      void model_print();
      void write( std::string name);
      bool isValid();
      bool isWritten();

      // Members
      pGModel model;
      pGIPart part;
      bool Written;

      // Methods to place a point
      bool place_point( double coords[3], pGVertex vert, bool abort_on_fail);
      bool point_on_dim( int dim, double coords[3]);
      int point_location( double coords[3]);
      void put_point_outside( double coords[3], pGVertex vert);
      void put_point_in_line( double coords[3], pGVertex vert);
      void put_point_in_face( double coords[3], pGVertex vert);
      void put_point_in_region( double coords[3], pGVertex vert);

      // Methods to place an edge

      // Methods to place a face

  };

}

#endif

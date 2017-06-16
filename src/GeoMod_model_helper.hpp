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

  };

}

#endif

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
}

#ifndef GEOMOD_PRINTER_HPP
#define GEOMOD_PRINTER_HPP

#include <cstdlib>
#include <iostream>
#include <string.h>

namespace GMD
{
  void print_error( std::string message, bool abort_on_fail=true);

  void print_warning( std::string message);

  void print_coords( double x[3]);
}
#endif

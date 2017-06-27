#include "GeoMod_printer.hpp"

#include <cstdlib>
#include <iostream>
#include <string.h>

namespace GMD
{
  void print_error( std::string message, bool abort_on_fail)
  {
    std::cout << "Error: " <<  message << std::endl;
    if( abort_on_fail)
    {
      std::abort();
    }
    return;
  }

  void print_warning( std::string message)
  {
    std::cout << "Warning: " << message << std::endl;
    return;
  }

  void print_coords( double x[3])
  {
    std::cout << "(" ;
    for (int i=0; i<3; i++)
    {
      std::cout << x[i] << ", ";
    }
    std::cout << "\b\b)\n";
    return;
  }
}

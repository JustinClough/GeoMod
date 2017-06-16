#ifndef GEOMOD_PRINTER_HPP
#define GEOMOD_PRINTER_HPP

#include <cstdlib>
#include <iostream>
#include <string.h>

namespace GMD
{
  void print_error( std::string message, bool abort_on_fail=true)
  {
    std::cout << "Error: " <<  message << std::endl;
    if( abort_on_fail)
    {
      std::abort();
    }
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
#endif

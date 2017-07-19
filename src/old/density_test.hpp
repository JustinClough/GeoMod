#ifndef DENSITY_TEST_HPP
#define DENSITY_TEST_HPP

#include "GeoMod.hpp"

void normalize( double* in, double* normed);

void test_density_random_walk( int numSegs);

void get_start_point( double* xyz1, double* range);

void get_direction( double* dir, double* oldDir);

void get_end_point( double** line_ends, double* dir, bool& touchedSurface, double* range, double length);

double get_rand_double( double min, double max);

void update_oldDir( double* dir, double* oldDir);

#endif

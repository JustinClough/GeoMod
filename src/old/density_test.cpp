#include "density_test.hpp"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void update_oldDir( double* dir, double* oldDir)
{
  for( int i=0; i<3; i++)
  {
    oldDir[i] = dir[i];
  }
}

double get_rand_double( double min, double max)
{
  return ((double)rand())/RAND_MAX*(max-min)+min;
}

int get_mesh_numVertsOnFaces( pMesh mesh)
{
  int total = 0;
  VIter v_it = M_vertexIter(mesh);
  pVertex vertex;
  while((vertex = VIter_next(v_it)))
  {
    gType type = V_whatInType(vertex);
    if(type == Gface || type == Gedge)
    {
      total++;
    }
  }
  VIter_delete(v_it);

  return total;
}

void get_start_point( double* xyz1, double* range)
{
  double min = range[0];
  double max = range[1];
  for (int i=0; i<3; i++)
  {
    xyz1[i] = get_rand_double( min, max);
  }
  return;
}

void normalize( double* in, double* normed)
{
  double mag = 0.0;
  for( int i=0; i<3; i++)
  {
    mag += (in[i])*(in[i]);
  }
  double mag_rt = sqrt(mag);
  for( int i=0; i<3; i++)
  {
    normed[i] = in[i]/mag_rt;
  }
  return;
}

void get_direction( double* dir, double* oldDir)
{
  if( oldDir == NULL)
  { // this is the starting direction. Any direction is okay
    for(int i=0; i<3; i++)
    {
      double max =  1;
      double min = -1;
      dir[i] = get_rand_double( min, max);
    }
  }
  else
  { // this is NOT the starting direction, new direction must 'cap' old
    double pi = 22.0/7.0; // ~3.14...
    double theta = get_rand_double( 0.0, 360.0)*pi/180.0;
    double phi = get_rand_double( -90.0, 90.0)*pi/180.0;
    double alpha = atan2(oldDir[1], oldDir[0]);
    double eta = atan2(oldDir[2], oldDir[0]);
    double gamma = pi/2.0 - alpha;
    double beta  = pi/2.0 - phi;

    double Ca = cos(alpha);
    double Cb = cos(beta);
    double Cg = cos(gamma);
    double Ce = cos(eta);
    double Ct = cos(theta);

    double Sa = sin(alpha);
    double Sb = sin(beta);
    double Sg = sin(gamma);
    double Se = sin(eta);
    double St = sin(theta);
  
    dir[0] = Cb*St*Cg*Ce + Sb*Ca*Ce - Cb*Ct*Se;
    dir[1] = -Cb*St*Sg   + 0        + Sb*Sa;
    dir[2] = Cb*St*Cg*Se + Sb*Ca*Se + Cb*Ct*Ce;
  }
  double normalized[] = {0.0, 0.0, 0.0};
  normalize( dir, normalized);
  for( int i=0; i<3; i++)
  {
    dir[i] = normalized[i];
  }

  return;
}

void get_end_point
  ( double** line_ends, double* dir, bool& touchedSurface, double* range, double length)
{
  touchedSurface = false;
  double max = range[1];
  double min = range[0];
  
  double* start = line_ends[0];
  double* end   = line_ends[1];

  for( int i=0; i<3; i++)
  {
    end[i] = start[i]+length*dir[i];
    if (end[i] > max || end[i] < min)
    {
      touchedSurface = true;
    }
  }
  return;
}

void test_density_random_walk( int numSegs)
{
  srand (time(NULL));

  double cube_edge = 1.0;
  double length = (200.0/17500.0);
  double range[] = {-(cube_edge)/2.0, (cube_edge)/2.0};

  pGModel cube = GMD::create_cube( cube_edge);
  GMD::gmd_t gmd( cube);

  
  double dir[] = {0.0, 0.0, 0.0};
  double oldDir[] = {0.0, 0.0, 0.0};
  double xyz1[] = {0.0, 0.0, 0.0};
  double xyz2[] = {0.0, 0.0, 0.0};
  double* line_ends[] = {xyz1, xyz2};
  double refine = cube_edge*length;

  std::vector<double> weights;
  weights.push_back(0.0);
  int order = 2;
  std::vector<double*> points;
  points.push_back(line_ends[0]);
  points.push_back(line_ends[1]);
  std::vector<double> knots;
  for( int i=0; i<(order+(int)points.size()); i++)
  {
    if( i<=order)
    {
      knots.push_back( 0.0);
    }
    else
    {
      knots.push_back( 1.0);
    }
  }
  pGEdge edge;

  for( int i=0; i<numSegs; i++)
  {
    if( i == 0)
    {
      get_start_point( xyz1, range);
      get_direction( dir, NULL);
    }
    else
    {
      get_direction( dir, oldDir);
    }

    bool touchedSurface = false;
    int attempt = 0;
    get_end_point( line_ends, dir, touchedSurface, range, length);
    while ( touchedSurface)
    {
      attempt++;
      get_start_point( xyz1, range);
      get_direction( dir, NULL);
      get_end_point( line_ends, dir, touchedSurface, range, length);
      if(attempt>50)
      {
        GMD::print_error("Maximum start attempts reached");
      }
    }
    gmd.place_edge_spline( order, points, knots, weights, refine, edge);
    update_oldDir( dir, oldDir);
    for (int i=0; i<3; i++)
    {
      line_ends[0][i] = line_ends[1][i];
    }
  }

  gmd.set_global_mesh_params( 1, 0.5, 1.0);
  gmd.create_mesh();

  pMesh mesh = gmd.get_mesh();
  std::ofstream res;
  res.open("results.txt", std::ios_base::app);
  int N_surf = get_mesh_numVertsOnFaces( mesh);
  int N_dis = numSegs;
  int N_FE = M_numVertices( mesh);
  res << N_dis << "\t";
  res << N_FE << "\t";
  res << N_surf << "\n";

  res.close();

  char name[] = "density_500";
  gmd.set_name( name);
  gmd.write_model();
  gmd.write_mesh();

  return;
}

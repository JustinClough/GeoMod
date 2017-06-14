#ifndef GEOMOD_TESTS_HPP
#define GEOMOD_TESTS_HPP

#include <GeoMod.hpp>

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>

void point_density_test( int i)
{
  double cube_edge = 1.0;
  double length = (200.0/17500.0);
  double range[] = {(cube_edge)/2, -(cube_edge)/2};

  pGModel cube = GMD::create_cube( cube_edge);
  GMD::gmd_t gmd( cube);

  double xyz1[] = {0.0, 0.0, 0.0};
  double xyz2[] = {0.0, 0.0, 0.0};
  double* line_ends[] = {xyz1, xyz2};
  double local_refine = cube_edge*length;
  double refine_radius = 0.0;

  int points = 1;
  for(int j = 0; j<i;j++)
  {
    points +=50;
  }
  for(int j = 0; j<points; j++)
  {
    get_rand_line_ends( line_ends, length, range); 
    gmd.place_line( line_ends, local_refine, refine_radius);
  }
  
  gmd.set_global_mesh_params( 2, 0.9, 1);
  gmd.create_mesh();

  if(points == 1)
  {
    char name[] = "Density_level_01.smd";
    gmd.set_model_name( name);
    gmd.write_model();
    gmd.write_mesh();
  }
  if(points == 51)
  {
    char name[] = "Density_level_51.smd";
    gmd.set_model_name( name);
    gmd.write_model();
    gmd.write_mesh();
  }
  if(points == 501)
  {
    char name[] = "Density_level_501.smd";
    gmd.set_model_name( name);
    gmd.write_model();
    gmd.write_mesh();
  }
  if(points == 1501)
  {
    char name[] = "Density_level_1501.smd";
    gmd.set_model_name( name);
    gmd.write_model();
    gmd.write_mesh();
  }
  int N_surf = gmd.get_mesh_numVertsOnFaces();
  std::cout<< "Surface Mesh Nodes = " << N_surf << std::endl;

  std::ofstream res;
  res.open("results.txt", std::ios_base::app);
  int N_FE = M_numVertices(gmd.get_mesh());
  int N_dis = points;
  res << N_dis << "\t";
  res << N_FE << "\t";
  res << N_surf << "\n";

  res.close();

  return;
}


/* test1(): 
*     - Create 2D model
*     - Create gmd_t instance with model
*     - Write model
*     - Create mesh from model
*     - Write mesh
*/
void test1()
{ 
  pGModel rect =  GMD::create_2D_rectangle( 3.0, 10.0);
  GMD::gmd_t gmd( rect);
  char name[] = "2D_rect.smd";
  gmd.set_model_name( name);
  gmd.write_model();

  gmd.set_global_mesh_params( 1, 0.1, 0.0);
  gmd.create_mesh();
  char m_name[] = "2D_rect.sms";
  gmd.set_mesh_name( m_name);
  gmd.write_mesh();
  std::cout << "\nPassed Test1\n\n";
  
  return;
}

/* test2(): 
*     - Create a 3D model
*     - Create gmd_t instance with model
*     - Write model
*     - Create mesh from model
*     - Write mesh
*/
void test2()
{
  pGModel cube = GMD::create_cube( 10.0);
  GMD::gmd_t gmd( cube);
  char name[] = "3D_cube.smd";
  gmd.set_model_name( name);
  gmd.write_model();

  gmd.set_global_mesh_params( 1, 0.1, 0.0);
  gmd.create_mesh();
  char mesh_name[] = "3D_cube.sms";
  gmd.set_mesh_name( mesh_name);
  gmd.write_mesh();
  std::cout << "\nPassed Test2\n\n";

  return;
}

/* test3():
*     - Create a 3D model
*     - Place a point with defined mesh refinement in center
*     - Write model
*     - Create a mesh from model
*     - Write mesh
*/
void test3()
{
  pGModel cube = GMD::create_cube( 2.0);
  GMD::gmd_t gmd(cube);
  double point[] = {0.0, 0.0, 0.0};
  bool tmp = false;
  gmd.place_point( point, 0.1, 0.1, tmp);
  char model_name[] = "centered_point.smd";
  gmd.set_model_name( model_name);
  gmd.write_model();

  gmd.set_global_mesh_params( 1, 0.1, 0.0);
  char mesh_name[] = "centered_point.sms";
  gmd.set_mesh_name( mesh_name );
  gmd.create_mesh();
  gmd.write_mesh();

  std::cout << "\nPassed Test3\n\n" ;
  return;
}

/* test4():
*     - Create a 3D model
*     - Place a point with defined mesh refinement on surface
*     - Write model
*     - Create mesh from model
*     - Write mesh
*/
void test4()
{
  pGModel cube = GMD::create_cube( 2.0);
  GMD::gmd_t gmd(cube);
  double point[] = {1.0, 0.0, 0.0};
  bool tmp = false;
  gmd.place_point( point, 0.1, 0.1, tmp);
  char model_name[] = "surface_point_3D.smd";
  gmd.set_model_name( model_name);
  gmd.write_model();

  gmd.set_global_mesh_params(1, 0.1, 0.0);
  char mesh_name[] = "surface_point_3D.sms";
  gmd.set_mesh_name( mesh_name);
  gmd.create_mesh();
  gmd.write_mesh();

  std::cout << "\nPassed Test4\n\n" ;
  return;
}

/* test5():
*     - Create a 3D model 
*     - Place a fully interior line with define mesh refinement for line only
*     - Place a fully interior line with define mesh refinement for cyln. surrounding
*     - Write model
*     - Do not assign name so auto-nameing feature is checked
*     - Create mesh
*     - Write mesh
*/
void test5()
{
  pGModel cube = GMD::create_cube( 2.0);
  GMD::gmd_t gmd(cube);
  double start_point[] = { 0.75, 0.0, 0.75};
  double end_point[] = { 0.75, 0.0, -0.75};
  double* points[2] = { start_point, end_point};
  double local_refine = 0.1;
  double zero_radius = 0.0;
  gmd.place_line( points, local_refine, zero_radius);

  start_point[0] = -0.75;
  end_point[0] = -0.75;
  double radius = 0.5;
  gmd.place_line( points, local_refine, radius);
  char model_name[] = "interior_line_3D.smd";
  gmd.set_model_name( model_name);
  gmd.write_model();

  gmd.set_global_mesh_params(1, 0.1, 0.0);
  gmd.create_mesh();
  gmd.write_mesh();

  std::cout << "\nPassed Test5\n\n" ;
  return;
}

/* test6():
*     - Create a 3D model
*     - Create one edge
*     - Create another edge that is collinear to the first edge
*     - Write model
*     - Create mesh
*     - Write mesh
*/

void test6()
{
  pGModel cube = GMD::create_cube( 2.0);
  GMD::gmd_t gmd (cube);

  double p1[] = {-0.75, 0.0, 0.0};
  double p2[] = {0.5, 0.0, 0.0};
  double* pts[]  = {p1, p2};
  double local_refine = 0.2;
  double refine_radius = 0.0;
  gmd.place_line( pts, local_refine, refine_radius);
  std::cout << "Placed first line.\n" ;

  double p1b[] = {-0.5, 0.01, 0.0};
  double p2b[] = {0.75, 0.0, 0.0};
  double* pts2[]  = {p1b, p2b};
  gmd.place_line( pts2, local_refine, refine_radius);
  std::cout << "Placed second line.\n" ;

  char name[] = "Overlapped_lines.smd";
  gmd.set_model_name( name);
  gmd.write_model();

  gmd.set_global_mesh_params(1, 0.1, 0.0);
  gmd.create_mesh();
  gmd.write_mesh();

  std::cout << "\nPassed Test6\n\n" ;
  return;
}

/*  test7():
*     - Create a 3D model
*     - Create an edge from a surface point to an interior point
*     - Write model
*     - Create mesh
*     - Write mesh
*/
void test7()
{
  pGModel cube = GMD::create_cube( 2.0);
  GMD::gmd_t gmd( cube);

  double p1c[] = {-0.5, 0.0, 0.0};
  double p2c[] = {1.0, 0.0, 0.0};
  double local_refine = 0.01;
  double refine_radius = 0.0;
  double* pts3[]  = {p1c, p2c};
  gmd.set_global_mesh_params( 1, 0.5, 0);
  gmd.place_line( pts3, local_refine, refine_radius);

  char name[] = "Surface_to_interior_line.smd";
  gmd.set_model_name( name);
  gmd.write_model();

  gmd.create_mesh();
  gmd.write_mesh();

  std::cout << "\nPassed Test7\n\n" ;
  return;
}

#endif

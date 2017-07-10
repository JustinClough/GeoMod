#include "GeoMod_Tests.hpp"
#include <string>

using std::cout;

/* test0(): 
 *     - Create a 3D model
 *     - Create a gmd object
 *     - Test printing features
 *     - Implicitly destory the gmd object
 */
void test0()
{
  pGModel cube = GMD::create_cube( 10.0);
  GMD::gmd_t gmd( cube);
  gmd.test_printers();

  gmd.release_model();
  gmd.release_mesh();

  cout << "\n\nPassed test0\n\n" ;
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
  pGModel rectangle = GMD::create_2D_rectangle( 5.0, 7.0);
  GMD::gmd_t gmd( rectangle);

  std::string name = "test1_rectangle";
  gmd.set_name( name);
  gmd.write_model();
  gmd.set_global_mesh_params( 1, 0.9, 0.0);
  gmd.create_mesh();
  gmd.write_mesh();
  
  gmd.release_model();
  gmd.release_mesh();

  cout << "\n\nPassed test1\n\n";
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
  std::string name = "test2_cube";
  gmd.set_name( name);
  gmd.write_model();
  gmd.set_global_mesh_params( 1, 0.9, 0.0);
  gmd.create_mesh();
  gmd.write_mesh();

  gmd.release_model();
  gmd.release_mesh();

  cout << "\n\nPassed test2\n\n";
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
  pGModel cube = GMD::create_cube( 5.0);
  GMD::gmd_t gmd( cube);
  std::string name = "test3_cube";
  gmd.set_name( name);

  gmd.set_global_mesh_params( 1, 0.5, 0.3);
  double coords[3] = {0.0, 0.0, 0.0};
  double refine = 0.01;
  double radius = 0.01;
  pGVertex vert;
  gmd.place_point( coords, refine, radius, vert);

  gmd.write_model();
  gmd.create_mesh();
  gmd.write_mesh();

  gmd.release_model();
  gmd.release_mesh();

  cout << "\n\nPassed test3\n\n";
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
  GMD::gmd_t gmd( cube);
  std::string name = "test4_cube";
  gmd.set_name( name);

  double coords[3] = {1.0, 0.0, 0.0};
  double refine = 0.1;
  double radius = 0.5;
  pGVertex vert;
  gmd.place_point( coords, refine, radius, vert);

  gmd.write_model();
  gmd.set_global_mesh_params( 1, 0.9, 0.0);
  gmd.create_mesh();
  gmd.write_mesh();

  gmd.release_model();
  gmd.release_mesh();

  cout << "\n\nPassed test4\n\n";
  return;
}

/* test5():
 *     - Create a 3D model
 *     - Place a point with defined mesh refinement on an edge
 *     - Write model
 *     - Create mesh from model
 *     - Write mesh
 */
void test5()
{
  pGModel cube = GMD::create_cube( 2.0);
  GMD::gmd_t gmd( cube);
  std::string name = "test5_cube";
  gmd.set_name( name);

  double coords[3] = {1.0, 1.0, 0.0};
  double refine = 0.1;
  double radius = 0.5;
  pGVertex vert;
  gmd.place_point( coords, refine, radius, vert);

  gmd.write_model();
  gmd.set_global_mesh_params( 1, 0.9, 0.0);
  gmd.create_mesh();
  gmd.write_mesh();

  gmd.release_model();
  gmd.release_mesh();

  cout << "\n\nPassed test5\n\n";
  return;
}

/* test6():
 *     - Create a 3D model 
 *     - Place a fully interior edge define by three points
 *     - Write model
 *     - Do not assign name so auto-nameing feature is checked
 *     - Create mesh
 *     - Write mesh
 */
void test6()
{
  pGModel cube = GMD::create_cube( 2.0);
  GMD::gmd_t gmd( cube);
  std::string name = "test6_cube";
  gmd.set_name( name);

  int order = 4;

  double p1[3] = {0.7, 0.0, 0.0};
  double p2[3] = {0.0, 0.3, 0.0};
  double p3[3] = {0.0, 0.0, 0.0};
  double p4[3] = {0.0, -0.3, 0.0};
  double p5[3] = {-0.7, 0.0, 0.0};
  std::vector<double*> points;
  points.push_back(p1); 
  points.push_back(p2);
  points.push_back(p3);
  points.push_back(p4);
  points.push_back(p5);

  std::vector<double> knots;
  knots.push_back(0.0);
  knots.push_back(0.0);
  knots.push_back(0.0);
  knots.push_back(0.0);
  knots.push_back(0.5);
  knots.push_back(1.0);
  knots.push_back(1.0);
  knots.push_back(1.0);
  knots.push_back(1.0);

  std::vector<double> weights;
  weights.push_back(0.0);

  pGEdge edge;
  double refine = 0.1;
  gmd.place_edge_spline( order, points, knots, weights, refine, edge);

  gmd.write_model();
  gmd.set_global_mesh_params( 1, 0.9, 0.0);
  gmd.create_mesh();
  gmd.write_mesh();

  gmd.release_model();
  gmd.release_mesh();

  cout << "\n\nPassed test6\n\n";
  return;
}

/* test7():
 *     - Create a 3D model
 *     - Create one spline based on-face edge defined by five points
 *     - Write model
 *     - Create mesh
 *     - Write mesh
 */

void test7()
{
  pGModel cube = GMD::create_cube( 2.0);
  GMD::gmd_t gmd( cube);
  std::string name = "test7_cube";
  gmd.set_name( name);

  int order = 4;

  double p1[3] = {1.0, 0.0, 0.9};
  double p2[3] = {1.0, 0.6, 0.0};
  double p3[3] = {1.0, 0.0, -0.9};
  double p4[3] = {1.0, -0.3, 0.0};
  double p5[3] = {1.0, 0.0, 0.4};
  std::vector<double*> points;
  points.push_back(p1); 
  points.push_back(p2);
  points.push_back(p3);
  points.push_back(p4);
  points.push_back(p5);

  std::vector<double> knots;
  knots.push_back(0.0);
  knots.push_back(0.0);
  knots.push_back(0.0);
  knots.push_back(0.0);
  knots.push_back(0.5);
  knots.push_back(1.0);
  knots.push_back(1.0);
  knots.push_back(1.0);
  knots.push_back(1.0);

  std::vector<double> weights;
  weights.push_back(0.0);

  pGEdge edge;
  double refine = 0.1;
  gmd.place_edge_spline( order, points, knots, weights, refine, edge);

  gmd.write_model();
  gmd.set_global_mesh_params( 1, 0.9, 0.0);
  gmd.create_mesh();
  gmd.write_mesh();

  gmd.release_model();
  gmd.release_mesh();

  cout << "\n\nPassed test7\n\n";
  return;
}

/*  test8():
 *     - Create a 3D model
 *     - Create an edge from a surface point to an interior point
 *     - Write model
 *     - Create mesh
 *     - Write mesh
 */
void test8()
{
  pGModel cube = GMD::create_cube( 2.0);
  GMD::gmd_t gmd( cube);
  std::string name = "test8_cube";
  gmd.set_name( name);

  int order = 4;

  double p1[3] = {1.0, 0.0, 0.9};
  double p2[3] = {0.1, 0.6, 0.0};
  double p3[3] = {0.3, 0.0, -0.9};
  double p4[3] = {0.0, -0.3, 0.0};
  double p5[3] = {0.0, 0.0, 0.4};
  std::vector<double*> points;
  points.push_back(p1); 
  points.push_back(p2);
  points.push_back(p3);
  points.push_back(p4);
  points.push_back(p5);

  std::vector<double> knots;
  knots.push_back(0.0);
  knots.push_back(0.0);
  knots.push_back(0.0);
  knots.push_back(0.0);
  knots.push_back(0.5);
  knots.push_back(1.0);
  knots.push_back(1.0);
  knots.push_back(1.0);
  knots.push_back(1.0);

  std::vector<double> weights;
  weights.push_back(0.0);

  pGEdge edge;
  double refine = 0.1;
  gmd.place_edge_spline( order, points, knots, weights, refine, edge);

  gmd.write_model();
  gmd.set_global_mesh_params( 1, 0.9, 0.0);
  gmd.create_mesh();
  gmd.write_mesh();

  gmd.release_model();
  gmd.release_mesh();

  cout << "\n\nPassed test8\n\n";
  return;
}

/* test9();
 *     - Create a 3D model
 *     - Create an edge that starts at a pre existing edge and then terminates
 *        in the region
 *     - Write Model
 *     - Create Mesh
 *     - Write Mesh
 */
void test9()
{
  pGModel cube = GMD::create_cube( 2.0);
  GMD::gmd_t gmd( cube);
  std::string name = "test9_cube";
  gmd.set_name( name);

  int order = 4;

  double p1[3] = {1.0, 1.0, 0.9};
  double p2[3] = {0.1, 0.6, 0.0};
  double p3[3] = {0.3, 0.0, -0.9};
  double p4[3] = {0.0, -0.3, 0.0};
  double p5[3] = {0.0, 0.0, 0.4};
  std::vector<double*> points;
  points.push_back(p1); 
  points.push_back(p2);
  points.push_back(p3);
  points.push_back(p4);
  points.push_back(p5);

  std::vector<double> knots;
  knots.push_back(0.0);
  knots.push_back(0.0);
  knots.push_back(0.0);
  knots.push_back(0.0);
  knots.push_back(0.5);
  knots.push_back(1.0);
  knots.push_back(1.0);
  knots.push_back(1.0);
  knots.push_back(1.0);

  std::vector<double> weights;
  weights.push_back(0.0);

  pGEdge edge;
  double refine = 0.1;
  gmd.place_edge_spline( order, points, knots, weights, refine, edge);

  gmd.write_model();
  gmd.set_global_mesh_params( 1, 0.9, 0.0);
  gmd.create_mesh();
  gmd.write_mesh();

  gmd.release_model();
  gmd.release_mesh();

  cout << "\n\nPassed test9\n\n";
  return;
}

/* test10();
 *     - Create a 3D model
 *     - Create a face completely internal to the region
 *     - Write Model
 *     - Create Mesh
 *     - Write Mesh
 */
void test10()
{
  pGModel cube = GMD::create_cube( 20.0);
  GMD::gmd_t gmd( cube);
  std::string name = "test10_cube";
  gmd.set_name( name);

  int u_order = 3;
  int v_order = 2;
  int u_num = 4;
  int v_num = 3;
  int periodicity = 0;

  // first 'row'     x     y    z
  double p1[3]  = {-6.0, -7.0, 3.0};
  double p2[3]  = {-2.0, -4.0, 0.0};
  double p3[3]  = { 3.0, -3.0, 1.0};
  double p4[3]  = { 7.0, -5.0, -3.0}; 
  // second 'row'               
  double p5[3]  = {-5.0, 0.0, -1.0};
  double p6[3]  = {-2.0, 0.0, 3.0};
  double p7[3]  = { 3.0, 1.0, -7.0};
  double p8[3]  = { 7.0, 0.0, 2.0}; 
  // third 'row'            
  double p9[3]  = {-6.0, 4.0, 1.0};
  double p10[3] = {-2.0, 2.0, 0.0};
  double p11[3] = { 3.0, 4.0, -3.0};
  double p12[3] = { 7.0, 3.0, 0.0};

  std::vector<double*> points;
  points.push_back(p1); 
  points.push_back(p2);
  points.push_back(p3);
  points.push_back(p4);
  points.push_back(p5);
  points.push_back(p6);
  points.push_back(p7);
  points.push_back(p8);
  points.push_back(p9);
  points.push_back(p10);
  points.push_back(p11);
  points.push_back(p12);

  std::vector<double> u_knots;
  u_knots.push_back(0.0);
  u_knots.push_back(0.0);
  u_knots.push_back(0.0);
  u_knots.push_back(0.5);
  u_knots.push_back(1.0);
  u_knots.push_back(1.0);
  u_knots.push_back(1.0);

  std::vector<double> v_knots;
  v_knots.push_back(0.0);
  v_knots.push_back(0.0);
  v_knots.push_back(0.5);
  v_knots.push_back(1.0);
  v_knots.push_back(1.0);

  std::vector<double> weights;
  weights.push_back(0.0);

  pGFace face;
  double refine = 0.1;
  gmd.place_surface_by_spline( 
      u_order, v_order, u_num, v_num, periodicity,
      points, u_knots, v_knots, weights, 
      refine, face);

  gmd.write_model();
  gmd.set_global_mesh_params( 1, 0.9, 0.0);
  gmd.create_mesh();
  gmd.write_mesh();

  gmd.release_model();
  gmd.release_mesh();

  cout << "\n\nPassed test10\n\n";
  return;
}

/* test11();
 *     - Create a 3D model
 *     - Create a face completely internal to the region expect for one point
 *        which is on a pre-existing surface
 *     - Write Model
 *     - Create Mesh
 *     - Write Mesh
 */
void test11()
{
  pGModel cube = GMD::create_cube( 20.0);
  GMD::gmd_t gmd( cube);
  std::string name = "test11_cube";
  gmd.set_name( name);

  int u_order = 3;
  int v_order = 2;
  int u_num = 4;
  int v_num = 3;
  int periodicity = 0;

  // first 'row'     x     y    z
  double p1[3]  = {-6.0, -7.0, 3.0};
  double p2[3]  = {-2.0, -4.0, 0.0};
  double p3[3]  = { 3.0, -3.0, 1.0};
  double p4[3]  = { 7.0, -5.0, -3.0}; 
  // second 'row'               
  double p5[3]  = {-5.0, 0.0, -1.0};
  double p6[3]  = {-2.0, 0.0, 3.0};
  double p7[3]  = { 3.0, 1.0, -7.0};
  double p8[3]  = { 7.0, 0.0, 2.0}; 
  // third 'row'            
  double p9[3]  = {-10.0, 4.0, 1.0};
  double p10[3] = {-2.0, 2.0, 0.0};
  double p11[3] = { 3.0, 4.0, -3.0};
  double p12[3] = { 7.0, 3.0, 0.0};

  std::vector<double*> points;
  points.push_back(p1); 
  points.push_back(p2);
  points.push_back(p3);
  points.push_back(p4);
  points.push_back(p5);
  points.push_back(p6);
  points.push_back(p7);
  points.push_back(p8);
  points.push_back(p9);
  points.push_back(p10);
  points.push_back(p11);
  points.push_back(p12);

  std::vector<double> u_knots;
  u_knots.push_back(0.0);
  u_knots.push_back(0.0);
  u_knots.push_back(0.0);
  u_knots.push_back(0.5);
  u_knots.push_back(1.0);
  u_knots.push_back(1.0);
  u_knots.push_back(1.0);

  std::vector<double> v_knots;
  v_knots.push_back(0.0);
  v_knots.push_back(0.0);
  v_knots.push_back(0.5);
  v_knots.push_back(1.0);
  v_knots.push_back(1.0);

  std::vector<double> weights;
  weights.push_back(0.0);

  pGFace face;
  double refine = 0.1;
  gmd.place_surface_by_spline( 
      u_order, v_order, u_num, v_num, periodicity,
      points, u_knots, v_knots, weights, 
      refine, face);

  gmd.write_model();
  gmd.set_global_mesh_params( 1, 0.9, 0.0);
  gmd.create_mesh();
  gmd.write_mesh();

  gmd.release_model();
  gmd.release_mesh();

  cout << "\n\nPassed test11\n\n";
  return;
}

/* test12();
 *     - Create a 3D model
 *     - Create a face completely internal to the region expect for one edge
 *        which is on a pre-existing surface
 *     - Write Model
 *     - Create Mesh
 *     - Write Mesh
 */
void test12()
{
  pGModel cube = GMD::create_cube( 20.0);
  GMD::gmd_t gmd( cube);
  std::string name = "test12_cube";
  gmd.set_name( name);

  int u_order = 3;
  int v_order = 2;
  int u_num = 4;
  int v_num = 3;
  int periodicity = 0;

  // first 'row'     x     y    z
  double p1[3]  = {-10.0, -7.0, 3.0};
  double p2[3]  = {-2.0, -4.0, 0.0};
  double p3[3]  = { 3.0, -3.0, 1.0};
  double p4[3]  = { 7.0, -5.0, -3.0}; 
  // second 'row'               
  double p5[3]  = {-10.0, 0.0, -1.0};
  double p6[3]  = {-2.0, 0.0, 3.0};
  double p7[3]  = { 3.0, 1.0, -7.0};
  double p8[3]  = { 7.0, 0.0, 2.0}; 
  // third 'row'            
  double p9[3]  = {-10.0, 4.0, 1.0};
  double p10[3] = {-5.0, 2.0, 0.0};
  double p11[3] = { 3.0, 4.0, -3.0};
  double p12[3] = { 7.0, 3.0, 0.0};

  std::vector<double*> points;
  points.push_back(p1); 
  points.push_back(p2);
  points.push_back(p3);
  points.push_back(p4);
  points.push_back(p5);
  points.push_back(p6);
  points.push_back(p7);
  points.push_back(p8);
  points.push_back(p9);
  points.push_back(p10);
  points.push_back(p11);
  points.push_back(p12);

  std::vector<double> u_knots;
  u_knots.push_back(0.0);
  u_knots.push_back(0.0);
  u_knots.push_back(0.0);
  u_knots.push_back(0.5);
  u_knots.push_back(1.0);
  u_knots.push_back(1.0);
  u_knots.push_back(1.0);

  std::vector<double> v_knots;
  v_knots.push_back(0.0);
  v_knots.push_back(0.0);
  v_knots.push_back(0.5);
  v_knots.push_back(1.0);
  v_knots.push_back(1.0);

  std::vector<double> weights;
  weights.push_back(0.0);

  pGFace face;
  double refine = 0.1;
  gmd.place_surface_by_spline( 
      u_order, v_order, u_num, v_num, periodicity,
      points, u_knots, v_knots, weights, 
      refine, face);

  gmd.write_model();
  gmd.set_global_mesh_params( 1, 0.9, 0.0);
  gmd.create_mesh();
  gmd.write_mesh();

  gmd.release_model();
  gmd.release_mesh();

  cout << "\n\nPassed test12\n\n";
  return;
}

/* test13();
 *     - Create a 3D model
 *     - Add a straight edge
 *     - Write Model
 *     - Create Mesh
 *     - Write Mesh
 */
void test13()
{
  pGModel cube = GMD::create_cube( 2.0);
  GMD::gmd_t gmd( cube);

  double start[3] = {0.5, 0.5, 0.5};
  double end[3] = {-0.5, -0.5, -0.5};
  double refine = 0.1;
  pGEdge edge;
  gmd.place_edge_line( start, end, refine, edge);

  char name[] = "test13_cube";
  gmd.set_name( name);

  gmd.write_model();
  gmd.set_global_mesh_params( 1, 0.9, 0.0);
  gmd.create_mesh();
  gmd.write_mesh();

  gmd.release_model();
  gmd.release_mesh();

  cout << "\n\nPassed test13\n\n";
  return;
}

/* test14()
 *     - Construct a gmd by loading from file
 *     - Test printer features 
 *     - Implicitly destroy the gmd object
 */
void test14()
{
  std::string fileName = "test2_cube.smd";
  GMD::gmd_t gmd( fileName, 1);
  gmd.test_printers();

  gmd.release_model();
  gmd.release_mesh();

  cout << "\n\nPassed test14\n\n";
  return;
}


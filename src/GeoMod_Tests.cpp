#include <GeoMod_Tests.hpp>
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

  double coords[3] = {0.0, 0.0, 0.0};
  double refine = 0.1;
  double radius = 1;
  gmd.place_point( coords, refine, radius);

  gmd.write_model();
  gmd.set_global_mesh_params( 1, 0.9, 0.0);
  gmd.create_mesh();
  gmd.write_mesh();

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
  gmd.place_point( coords, refine, radius);

  gmd.write_model();
  gmd.set_global_mesh_params( 1, 0.9, 0.0);
  gmd.create_mesh();
  gmd.write_mesh();

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
  gmd.place_point( coords, refine, radius);

  gmd.write_model();
  gmd.set_global_mesh_params( 1, 0.9, 0.0);
  gmd.create_mesh();
  gmd.write_mesh();

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

  double refine = 0.1;
  gmd.place_edge( order, points, knots, weights, refine);

  gmd.write_model();
  gmd.set_global_mesh_params( 1, 0.9, 0.0);
  gmd.create_mesh();
  gmd.write_mesh();

  cout << "\n\nPassed test6\n\n";
  return;
}

/* test7():
 *     - Create a 3D model
 *     - Create one spline based on-face edge defined by three points
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

  double refine = 0.1;
  gmd.place_edge( order, points, knots, weights, refine);

  gmd.write_model();
  gmd.set_global_mesh_params( 1, 0.9, 0.0);
  gmd.create_mesh();
  gmd.write_mesh();

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

  double refine = 0.1;
  gmd.place_edge( order, points, knots, weights, refine);

  gmd.write_model();
  gmd.set_global_mesh_params( 1, 0.9, 0.0);
  gmd.create_mesh();
  gmd.write_mesh();

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

  double refine = 0.1;
  gmd.place_edge( order, points, knots, weights, refine);

  gmd.write_model();
  gmd.set_global_mesh_params( 1, 0.9, 0.0);
  gmd.create_mesh();
  gmd.write_mesh();

  cout << "\n\nPassed test9\n\n";
  return;
}

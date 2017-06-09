#ifndef GEOMOD_TESTS_HPP
#define GEOMOD_TESTS_HPP

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

  gmd.set_global_mesh_params( 1, 0.1);
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

  gmd.set_global_mesh_params( 1, 0.1);
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
  gmd.place_point( point, 0.1);
  char model_name[] = "centered_point.smd";
  gmd.set_model_name( model_name);
  gmd.write_model();

  gmd.set_global_mesh_params( 1, 0.1);
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
  gmd.place_point( point, 0.1);
  char model_name[] = "surface_point_3D.smd";
  gmd.set_model_name( model_name);
  gmd.write_model();

  gmd.set_global_mesh_params(1, 0.1);
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

  gmd.set_global_mesh_params(1, 0.1);
  char mesh_name[] = "interior_line_3D.sms";
  gmd.set_mesh_name( mesh_name);
  gmd.create_mesh();
  gmd.write_mesh();

  std::cout << "\nPassed Test5\n\n" ;
  return;
}

#endif

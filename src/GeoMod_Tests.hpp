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

  gmd.create_mesh();
  char mesh_name[] = "3D_cube.sms";
  gmd.set_mesh_name( mesh_name);
  gmd.write_mesh();
  std::cout << "\nPassed Test2\n\n";

  return;
}

#endif

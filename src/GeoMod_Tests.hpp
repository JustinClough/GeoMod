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
}

#endif

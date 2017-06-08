#ifndef GEOMOD_TESTS_HPP
#define GEOMOD_TESTS_HPP

void test1()
{ // Need to scope so that gmd_t destructor is called before sim_end();

  pGModel rect =  GMD::create_2D_rectangle( 1.0, 1.0);
  GMD::gmd_t gmd( rect);
  //double point[] = {0.0, 0.0, 0.0};
  //gmd.place_point( point, 0.1);
  char name[] = "test_point.smd";
  gmd.set_model_name( name);
  gmd.write_model();
  gmd.create_mesh();
  char m_name[] = "test_point.sms";
  gmd.set_mesh_name( m_name);
  gmd.write_mesh();
}
#endif

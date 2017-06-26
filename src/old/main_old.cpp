// Not really old, just a backup

// GeoMod Header
#include "GeoMod.hpp"

#include "GeoMod_Tests.hpp"

int main( int argc, char** argv)
{
  char sim_log[] = "Sim_log.log";
  std::cout << "START" << std::endl;
  GMD::sim_start( sim_log, argc, argv);

  // mesh_growth_test();

#if 0
  pGModel cube = GMD::create_cube(2.0);
  double point1[] = {1.0, 0.0, 0.0};
  double point2[] = {0.0, 0.0, 0.0};

  pGIPart part = GM_part( cube);
  GRIter r_it = GIP_regionIter( part);
  GFIter f_it = GIP_faceIter( part);
  pGRegion region = GRIter_next( r_it);
  pGFace face = GFIter_next( f_it);
  face = GFIter_next( f_it);
  face = GFIter_next( f_it);

  pGVertex vert1 = GIP_insertVertexInFace( part, point1, face);
  pGVertex vert2 = GIP_insertVertexInRegion( part, point2, region);

  pCurve line = SCurve_createLine( point1, point2);
  GIP_insertEdgeInRegion( part, vert1, vert2, line, 1, region);

  GM_write( cube, "test_cube.smd", 0, 0);
#endif

  GMD::sim_end();
  std::cout << "END" << std::endl;
  return 0;
}

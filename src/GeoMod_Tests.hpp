#ifndef GEOMOD_TESTS_HPP
#define GEOMOD_TESTS_HPP

#include <GeoMod.hpp>


/* test0(): 
 *     - Create a 3D model
 *     - Create a gmd object
 *     - Test printing features
 *     - Implicitly destory the gmd object
 */
void test0();

/* test1(): 
 *     - Create 2D model
 *     - Create gmd_t instance with model
 *     - Write model
 *     - Create mesh from model
 *     - Write mesh
 */
void test1();

/* test2(): 
 *     - Create a 3D model
 *     - Create gmd_t instance with model
 *     - Write model
 *     - Create mesh from model
 *     - Write mesh
 */
void test2();

/* test3():
 *     - Create a 3D model
 *     - Place a point with defined mesh refinement in center
 *     - Write model
 *     - Create a mesh from model
 *     - Write mesh
 */
void test3();

/* test4():
 *     - Create a 3D model
 *     - Place a point with defined mesh refinement on surface
 *     - Write model
 *     - Create mesh from model
 *     - Write mesh
 */
void test4();

/* test5():
 *     - Create a 3D model 
 *     - Place a fully interior line with define mesh refinement for line only
 *     - Place a fully interior line with define mesh refinement for cyln. surrounding
 *     - Write model
 *     - Do not assign name so auto-nameing feature is checked
 *     - Create mesh
 *     - Write mesh
 */
void test5();

/* test6():
 *     - Create a 3D model
 *     - Create one edge
 *     - Create another edge that is collinear to the first edge
 *     - Write model
 *     - Create mesh
 *     - Write mesh
 */

void test6();

/*  test7():
 *     - Create a 3D model
 *     - Create an edge from a surface point to an interior point
 *     - Write model
 *     - Create mesh
 *     - Write mesh
 */
void test7();

/* test8();
 *     - Create a 3D model
 *     - Create an interpolating edge from 4 interior points
 *     - Write Model
 *     - Create Mesh
 *     - Write Mesh
 */
void test8();

#endif

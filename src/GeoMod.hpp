#ifndef GEOMOD_HPP
#define GEOMOD_HPP

// Simmetrix Headers
#include "SimUtil.h"
#include "SimModel.h"
#include "SimAdvModel.h"
#include "MeshSim.h"

// Standard C++ Headers
#include <cstdlib>
#include <iostream>
#include <vector>
#include <math.h>

namespace GMD
{
  class gmd_t
  {
    public:
      gmd_t( pGModel geom);
      ~gmd_t();
      void update_model(pGModel geom);
      void write_mesh( );
      void write_model( );

      void set_mesh_name (char* name);
      char* get_mesh_name();
      void set_model_name (char* name);
      char* get_model_name();
      void verify_model( bool abort_on_fail=true);

      void set_global_mesh_params( double order, double refine, double grad_rate_in);
      pGVertex place_point(double* point, double local_refine, double refine_radius, bool& onFace);
      void place_line( double** points, double local_refine, double refine_radius);
      pMesh get_mesh();
      pGModel get_model();
      pMesh create_mesh( bool abort_on_fail = true);

      int get_mesh_numVertsOnFaces();
      void count_face_loops();
      bool verify_mesh( bool abort_on_fail=true);

    protected:
      void set_point_refine( double* point, double local_refine, double refine_radius);
      void set_mesh( );
      void set_model(pGModel geom);
      char* mesh_name;
      char* model_name;
      pGModel model;
      pMesh mesh;
      pACase m_case;
      bool g_mesh_set;
      double m_order;
      double m_g_refine;
      double grad_rate;
      void assign_model_name_to_mesh();
  };

  pGModel create_2D_rectangle( double y_length, double x_width);

  bool insert_vertex_on_face( pGModel geom, double* point, pGVertex vert, pGFace face);

  void release_model(pGModel model);

  void release_mesh(pMesh mesh);

  void sim_start( char* Sim_log_file_name);

  void sim_end();

  pGModel create_cube(double length);
} //END namespace GMD

double fRand(double fMin, double fMax);
void get_rand_line_ends( double** line_ends, double length, double* range);

pGVertex GIP_insertVertexInFace(pGIPart part, double* xyz, pGFace face);

#endif // GEOMOD_HPP

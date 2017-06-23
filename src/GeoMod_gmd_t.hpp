#ifndef GEOMOD_GMD_T_HPP
#define GEOMOD_GMD_T_HPP

#include <string>
#include <vector>
#include <GeoMod_printer.hpp>
#include <GeoMod_model_helper.hpp>
#include <GeoMod_mesh_helper.hpp>

namespace GMD
{

  class gmd_t
  {
    public:
      // Util methods
      gmd_t( pGModel in_model);
      ~gmd_t();
      void set_abort_on_fail( bool abort_on_fail);
      void test_printers();
      void verify_mesh();

      // Writing methods
      void set_name( std::string file_name);
      void write_model();
      void write_mesh();

      // Methods to modify the geometry
      void place_point( 
          double coords[3], 
          double refine, 
          double radius, 
          pGVertex& vert);
      void place_edge( 
          int order,
          std::vector<double*> points, 
          std::vector<double> knots,
          std::vector<double> weights,
          double refine,
          pGEdge& edge);
      void place_surface_by_spline(
          int u_order, 
          int v_order, 
          int periodicity, 
            // 0=none, 1=u periodic, 2=v periodic, 3=u&&v periodic
          std::vector<double*> u_points,
          std::vector<double*> v_points,
          std::vector<double> u_knots,
          std::vector<double> v_knots,
          std::vector<double> weights,
          double refine,
          pGFace& face);
      void place_surface_by_loops( 
          std::vector<pGEdge> edges, 
          pGFace& face);

      // Methods for only meshing
      void set_global_mesh_params( 
          int order_in, 
          double refine_in, 
          double grad_rate_in);
      void create_mesh();
    
    private:
      bool panicStatus;
      std::string name;
      model_helper_t* modeler;
      mesh_helper_t* mesher;
      void check_spline_params( 
          int order,
          std::vector<double*> points,
          std::vector<double> knots,
          std::vector<double> weights);
      void check_surface_params( 
          int u_order, 
          int v_order, 
          int periodicity, 
          std::vector<double*> u_points,
          std::vector<double*>  v_points, 
          std::vector<double> u_knots, 
          std::vector<double> v_knots, 
          std::vector<double> weights);
  };

}

#endif

#ifndef GEOMOD_MODEL_HELPER_HPP
#define GEOMOD_MODEL_HELPER_HPP

#include <string>
#include <vector>
#include <GeoMod_SIM.hpp>
#include <GeoMod_printer.hpp>
#include <GeoMod_coords.hpp>

pGVertex GIP_insertVertexInFace(pGIPart part, double* xyz, pGFace face);

namespace GMD
{
  class model_helper_t
  {
    friend class gmd_t;
    private:
      // Util methods
      model_helper_t (pGModel& in_model);
      ~model_helper_t();
      void model_print();
      void write( std::string name);
      bool isValid();
      bool isWritten();
      void unpack_vector_spline_points( 
          std::vector<double*> vec,
          double* x);
      void unpack_vector( std::vector<double> vec, double* x);
      void unpack_surface_vector( 
          std::vector<double*> points, 
          double* all_points);
      void create_bounding_edges( 
          int u_order, 
          int v_order, 
          int u_num,
          int v_num,
          std::vector<double*> points, 
          std::vector<double> u_knots, 
          std::vector<double> v_knots, 
          std::vector<double> weights,
          std::vector<pGEdge>& edges);

      // Members
      pGModel model;
      pGIPart part;
      bool Written;

      // Methods to place a point
      bool place_point( 
          double coords[3], 
          pGVertex& vert, 
          bool abort_on_fail);
      bool point_on_dim( int dim, double coords[3]);
      int point_location( double coords[3]);
      bool PointOnFace( double coords[3], pGFace face);
      bool PointOnEdge( double coords[3], pGEdge edge);
      void put_point_outside( double coords[3], pGVertex& vert);
      void put_point_in_line( double coords[3], pGVertex& vert);
      void put_point_in_face( double coords[3], pGVertex& vert);
      void put_point_in_region( double coords[3], pGVertex& vert);

      // Methods to place an edge
      void place_edge( 
          int order, 
          std::vector<double*> points, 
          std::vector<double> knots,
          std::vector<double> weights,
          pGEdge& edge);
      void create_curve( 
          int order, 
          std::vector<double*> points, 
          std::vector<double> knots,
          std::vector<double> weights,
          pCurve& curve);
      void create_edge( 
          int order, 
          std::vector<double*> points, 
          pCurve& curve,
          pGEdge& edge);
      bool PointsOnSameFace( std::vector<double*> points);

      // Methods to place a surface
      void place_surface_by_spline( 
          int u_order, 
          int v_order, 
          int u_num,
          int v_num,
          int periodicity, 
          std::vector<double*> points,
          std::vector<double> u_knots,
          std::vector<double> v_knots,
          std::vector<double> weights,
          pGFace& face);
      void create_surface( 
          int u_order, 
          int v_order, 
          int u_num,
          int v_num,
          int periodicity, 
          std::vector<double*> points,
          std::vector<double> u_knots,
          std::vector<double> v_knots,
          std::vector<double> weights,
          pSurface& surface,
          std::vector<pGEdge>& edges);
      void create_face(
          pSurface& surface,
          std::vector<pGEdge>& edges,
          pGFace& face);
          
  };

}

#endif

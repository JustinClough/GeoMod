#ifndef GEOMOD_MODEL_HELPER_HPP
#define GEOMOD_MODEL_HELPER_HPP

// Standard Library
#include <string>
#include <vector>

// GeoMod Headers
#include "GeoMod_SIM.hpp"
#include "GeoMod_printer.hpp"
#include "GeoMod_coords.hpp"

pGVertex GIP_insertVertexInFace(pGIPart part, double* xyz, pGFace face);

namespace GMD
{
  class model_helper_t
  {
    friend class gmd_t;
    private:
      // Util methods

      /// @brief Constructor.
      /// @param in_model Pointer to the preexisiting Simmetrix model.
      model_helper_t (pGModel& in_model);

      /// @brief Destructor.
      ~model_helper_t();

      /// @brief Print a test message to stdout.
      void model_print();

      /// @brief Release the model.
      void release();

      /// @brief Write the model to disk.
      /// @param name Name of the file. '.smd' is appended.
      void write( std::string name);

      /// @brief Returns true if the model is topologically valid. 
      bool isValid();

      /// @brief Returns true is the model has been written.
      bool isWritten();

      /// @brief Puts the values pointed to in vec into array x.
      /// @param vec The vector of coordinates.
      /// @param x   The array of coordinates to write to.
      void unpack_vector_spline_points( 
          std::vector<double*> vec,
          double* x);

      /// @brief Puts the values in vec into array x.
      /// @param vec The vector of values.
      /// @param x   The array of values to write to. 
      void unpack_vector( std::vector<double> vec, double* x);

      /// @brief Puts the values pointed to in points into all_points.
      /// @param points     The vector of coordinates. Order is
      ///                   all u points in v=0, then v=1, ... etc.
      /// @param all_points The array to write to.
      void unpack_surface_vector( 
          std::vector<double*> points, 
          double* all_points);

      /// @brief Creates the bounding edges for a surface to become a face.
      /// @param u_order Order of the curves in the u direction.
      /// @param v_order Order of the curves in the v direction.
      /// @param u_num   Number of control points in the u direction.
      /// @param v_num   Number of control points in the v direction.
      /// @param points  Vector of control points.
      /// @param u_knots Vector of knots in the u direction.
      /// @param v_knots Vector of knots in the v direction.
      /// @param weights Vector of weights. Pass a single value of 0.0
      ///                for non-rational splines.
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
      void unpack_bounding_edges( 
          std::vector<pGEdge>& edges, 
          pGEdge* bounding_edges);

      // Members
      pGModel model;
      pGIPart part;
      bool Written;

      // Methods to place a point
      /// @brief Create a vertex in the model.
      /// @param coords The coordinates (x,y,z) to place the vertex at.
      /// @param vert   Pointer to the created vertex.
      /// @param abort_on_fail
      ///               Pass false to print only an warning if vertex placement fails.
      bool place_point( 
          double coords[3], 
          pGVertex& vert, 
          bool abort_on_fail);

      /// @brief Returns true if the coordinate is on the given dimension
      /// @param dim    The dimension (0, 1, or 2)
      /// @param coords Coordinates (x,y,z) of the point.
      bool point_on_dim( int dim, double coords[3]);

      /// @brief Returns the lowest order dimension a point can be classified on
      ///        while remaining topologically valid. The native model tolerance
      ///        is used for all comparisons.
      /// @param coords Coordinates (x,y,z) of the point of interest.
      int point_location( double coords[3]);

      /// @brief Returns true if point is on the given model face.
      /// @param coords Coordinates (x,y,z) of the point of interest.
      /// @param face   The model face of interest.
      bool PointOnFace( double coords[3], pGFace face);

      /// @brief Returns true if point is on the given model edge.
      /// @param coords Coordinates (x,y,z) of the point of interest.
      /// @param face   The model edge of interest.
      bool PointOnEdge( double coords[3], pGEdge edge);

      /// @brief Creates a vertex in the void region of a model.
      /// @param coords Coordinates of the vertex to create.
      /// @param vert   Pointer to the created vertex.
      void put_point_outside( double coords[3], pGVertex& vert);

      /// @brief Creates a vertex on an edge by splitting edge.
      /// @param coords Coordinates of the vertex to create.
      /// @param vert   Pointer to the created model vertex.
      void put_point_in_line( double coords[3], pGVertex& vert);

      /// @brief Creates a vertex on a model face.
      /// @param coords Coordinates of the vertex to create.
      /// @param vert   Pointer to the created vertex.
      void put_point_in_face( double coords[3], pGVertex& vert);

      /// @brief Creates a vertex in a model region.
      /// @param coords Coordinates of the vertex to create.
      /// @param vert   Pointer to the created vertex.
      void put_point_in_region( double coords[3], pGVertex& vert);

      // Methods to place an edge
      /// @brief Creates an basis spline define edge with end points
      ///        created as model vertices.
      /// @param order   Order of the spline.
      /// @param points  Vector of coordinates of control points.
      /// @param knots   Vector of spline knots. 
      /// @param weights Weights for control points. 
      ///                Pass single 0.0 for non-rational curve
      /// @param edge    Pointer to the created edge. 
      void place_edge( 
          int order, 
          std::vector<double*> points, 
          std::vector<double> knots,
          std::vector<double> weights,
          pGEdge& edge);

      /// @brief Creates the curve defined by a basis spline.
      /// @param order   Order of the spline.
      /// @param points  Vector of coordinates of control points.
      /// @param knots   Vector of spline knots. 
      /// @param weights Weights for control points. 
      ///                Pass single 0.0 for non-rational curve
      /// @param curve   Pointer to the created curve. 
      void create_curve( 
          int order, 
          std::vector<double*> points, 
          std::vector<double> knots,
          std::vector<double> weights,
          pCurve& curve);

      /// @brief Creates the edge defined by a curve.
      /// @param points  Vector of coordinates of control points.
      /// @param curve   Pointer to the curve. 
      /// @param edge    Pointer to the created edge. 
      void create_edge( 
          std::vector<double*> points, 
          pCurve& curve,
          pGEdge& edge);

      /// @brief Returns true if all points are on the same model face.
      /// @param points Vector of point coordinates.
      bool PointsOnSameFace( std::vector<double*> points);

      // Methods to place a surface
      /// @brief Adds a surface to the model by basis spline definition.
      /// @param u_order     The order of the u direction spline.
      /// @param v_order     The order of the v direction spline.
      /// @param u_num       Number of control points in the u direction.
      /// @param v_num       Number of control points in the v direction.
      /// @param periodicity Pass 0 for no period.
      ///                    1 for periodic in u direction. 
      ///                    2 for periodic in v direction. 
      ///                    3 for periodic in u and v direction. 
      /// @param points      Control points of the surface. Formatted as 
      ///                    all for v=1, increasing u, then v=2, increasing u... etc.
      /// @param u_knots     The spline knots for the u direction.
      /// @param v_knots     The spline knots for the v direction.
      /// @param weights     The weights of each control. Must be formatted 
      ///                    the same as the control points. Pass a single 
      ///                    entry 0.0 for non-rational surface.
      /// @param face        Pointer to the created surface.
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

      /// @brief Creates a surface by basis spline definition.
      /// @param u_order     The order of the u direction spline.
      /// @param v_order     The order of the v direction spline.
      /// @param u_num       Number of control points in the u direction.
      /// @param v_num       Number of control points in the v direction.
      /// @param periodicity Pass 0 for no period.
      ///                    1 for periodic in u direction. 
      ///                    2 for periodic in v direction. 
      ///                    3 for periodic in u and v direction. 
      /// @param points      Control points of the surface. Formatted as 
      ///                    all for v=1, increasing u, then v=2, increasing u... etc.
      /// @param u_knots     The spline knots for the u direction.
      /// @param v_knots     The spline knots for the v direction.
      /// @param weights     The weights of each control. Must be formatted 
      ///                    the same as the control points. Pass a single 
      ///                    entry 0.0 for non-rational surface.
      /// @param surface     Pointer to the created surface.
      /// @param edges       Vector of the bounding edges.
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

      /// @brief Creates and adds a face to the model.
      /// @param surface     Pointer to the created surface.
      /// @param edges       Vector of the bounding edges.
      /// @param face        Pointer to the created surface.
      void create_face(
          pSurface& surface,
          std::vector<pGEdge>& edges,
          pGFace& face);
          
  };

}

#endif

#ifndef GEOMOD_GMD_T_HPP
#define GEOMOD_GMD_T_HPP

// Standard Library
#include <string>
#include <vector>

// GeoMod Headers
#include "GeoMod_printer.hpp"
#include "GeoMod_model_helper.hpp"
#include "GeoMod_mesh_helper.hpp"

namespace GMD
{

  class gmd_t
  {
    public:
      // Basic methods

      /// @brief Constructor for models already in memory.
      /// @param in_model A pointer to a preexisting Simmetrix model.
      /// @param numParts The number of parts to partition mesh.
      gmd_t( pGModel in_model, int numParts = 1);

      /// @brief Constructor for models loaded from file.
      /// @param fileName Filename, including relative path, of the model.
      ///                 Must end with '.smd'.
      /// @param numParts Number of parallel components.
      gmd_t( std::string fileName, int numParts = 1);

      /// @brief Destructor.
      ~gmd_t();

      /// @brief Sets whether to abort on failure. Constructor set default is true.
      /// @param abort_on_fail Set True to treat warnings as errors.
      void set_abort_on_fail( bool abort_on_fail);

      /// @brief Has friend classes print message to stdout.
      void test_printers();

      // Util Methods
      /// @brief Vertify the topolgy and connectivity of the model.
      void verify_model();

      /// @brief Release the Simmetrix model.
      void release_model();

      /// @brief Get a pointer to the Simmetrix model.
      pGModel get_model();

      /// @brief Verify the Simmetrix mesh.
      void verify_mesh();

      /// @brief Get a pointer to the Simmetrix mesh (serial only).
      pMesh get_mesh();

      /// @brief Get a pointer to the Simmetrix mesh (parallel only).
      pParMesh get_par_mesh();

      /// @brief Release the Simmetrix mesh and mesh case.
      void release_mesh();

      // Writing methods
      /// @brief Set the name of the model and mesh.
      /// @param file_name The name of the file.
      ///                  '.smd' and '.sms' will be appended as needed.
      void set_name( std::string file_name);

      /// @brief Write the model to disk.
      void write_model();

      /// @brief Write the mesh to disk.
      void write_mesh();

      // Methods to modify the geometry
      /// @brief Adds a vertex to the model. Sets mesh refinement for created vertex.
      ///         Classification will be on the lowest order geometric entity.
      /// @param coords The (x,y,z) coordinates of the vertex.
      /// @param refine The relative mesh refinement for the vertex.
      ///               Ignored if in the void region.
      /// @param radius The refinement radius around the vertex.
      ///               Ignored if in the void region. Pass 0.0 for no radius.
      /// @param vert Pointer to the created vertex.
      void place_point(
          double coords[3],
          double refine,
          double radius,
          pGVertex& vert);

      /// @brief Adds an edge to the model by basis spline definition.
      ///        Sets mesh refinement for the edge.
      /// @param order   The order of the spline.
      /// @param points  The control points of the spline.
      ///                Each point must be in (x,y,z) format.
      /// @param knots   The knots for the spline.
      /// @param weights The weights for the spline.
      ///                Pass a vector with one entry of 0.0
      ///                to create a non rational spline
      /// @param refine  The relative mesh refinement for the edge.
      /// @param edge    Pointer to the created edge.
      void place_edge_spline(
          int order,
          std::vector<double*> points,
          std::vector<double> knots,
          std::vector<double> weights,
          double refine,
          pGEdge& edge);

      /// @brief Adds a straight edge to the model.
      /// @param start The start point of the edge (x,y,z).
      /// @param end   The end point of the edge (x,y,z).
      /// @param edge  Pointer to the created edge.
      void place_edge_line(
          double* start,
          double* end,
          double refine,
          pGEdge& edge);

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
      /// @param refine      The relative mesh refinement for the surface.
      /// @param face        Pointer to the created surface.
      void place_surface_by_spline(
          int u_order,
          int v_order,
          int u_num,
          int v_num,
          int periodicity,
            // 0=none, 1=u periodic, 2=v periodic, 3=u&&v periodic
          std::vector<double*> points,
          std::vector<double> u_knots,
          std::vector<double> v_knots,
          std::vector<double> weights,
          double refine,
          pGFace& face);

      // Methods for only meshing
      /// @brief Sets global meshing parameters.
      /// @param order_in     Order of the mesh. Options are currently 1 or 2.
      /// @param refine_in    Global relative level of mesh refinement.
      /// @param grad_rate_in Gradation rate. Pass 0.0 to use Simmetrix default.
      void set_global_mesh_params(
          int order_in,
          double refine_in,
          double grad_rate_in);

      /// @brief Creates the mesh.
      void create_mesh();

      /// @brief Forces surface meshes to use only quad elements.
      void force_surf_quads();

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
          int u_num,
          int v_num,
          int periodicity,
          std::vector<double*> points,
          std::vector<double> u_knots,
          std::vector<double> v_knots,
          std::vector<double> weights);
  };

}

#endif

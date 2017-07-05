#ifndef GEOMOD_MESH_HELPER_HPP
#define GEOMOD_MESH_HELPER_HPP

#include <string>
#include <vector>
#include "GeoMod_SIM.hpp"
#include "GeoMod_printer.hpp"

namespace GMD
{
  class mesh_helper_t
  {
    friend class gmd_t;
    private:
      // Util methods
      /// @brief Constuctor.
      /// @param in_model The preexisting model.
      /// @param numParts Number of parallel parts.
      mesh_helper_t( pGModel in_model, int numParts);

      /// @brief Destructor.
      ~mesh_helper_t();

      /// @brief Print a test message to stdout.
      void mesh_print();

      /// @brief Write the mesh to disk.
      /// @param name The name of the mesh. '.sms' is applied.
      void write( std::string name);

      /// @brief Check is the mesh is valid.
      bool isValid();

      /// @brief Create the mesh. Calls on surface and voume meshers.
      void create();

      /// @brief Free memory of the mesh and mesh case.
      void release();

      /// @brief Print mesh info to stdout.
      ///        Info includes number of mesh vertices, 
      ///        edges, faces, and regions for each part.
      void print_mesh_info();

      // Members
      bool isPar;
      pMesh mesh;
      pParMesh parMesh;
      pACase m_case;
      bool globalSet;
      double order;
      double refine;
      double grad_rate;

      // Mesh preping methods
      /// @brief Defines a refinement point.
      /// @param coords  Coordinates (x,y,z) of refinement.
      /// @param refine  Relative mesh refinement level.
      /// @param radius  Radius of mesh refinement. Pass 0.0 to ignore.
      /// @param abort_on_fail 
      ///                Abort or continue on failure.
      void place_point( 
          double coords[3], 
          double refine, 
          double radius, 
          bool abort_on_fail);

      /// @brief Sets basic global mesh details.
      /// @param order_in  Order of the mesh. Pass 1 or 2 ONLY!!!
      /// @param refine_in Relative global refinement level.
      /// @param grad_rate_in 
      ///                  Gradation rate; pass 0.0 to use default.
      void set_global( int order_in, double refine_in, double grad_rate_in);

      /// @brief Sets refinement around a model vertex.
      /// @param refine Relative level of refinement.
      /// @param vert   Existing model vertex.
      void refine_vertex( double refine, pGVertex vert);

      /// @brief Sets refinement around a mesh vertex. 
      /// @param refine Relative level of refinement.
      /// @param edge   Existing model edge.
      void refine_edge( double refine, pGEdge edge);

      /// @brief Sets refinement around a mesh face.
      /// @param refine Relative level of refinement.
      /// @param face   Existing model face.
      void refine_face( double refine, pGFace face);

      /// @brief Forces surface meshes to use quads only.
      void force_surf_quad();

  };

}

#endif

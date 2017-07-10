set(GeoMod_sources
GeoMod_coords.cpp
GeoMod_gmd_t.cpp
GeoMod_mesh_helper.cpp
GeoMod_model_helper.cpp
GeoMod_printer.cpp
GeoMod_util.cpp
GeoMod_Tests.cpp
density_test.cpp)

set(GeoMod_headers
GeoMod_coords.hpp
GeoMod_gmd_t.hpp
GeoMod.hpp
GeoMod_mesh_helper.hpp
GeoMod_model_helper.hpp
GeoMod_printer.hpp
GeoMod_SIM.hpp
GeoMod_Tests.hpp
GeoMod_util.hpp)

add_library(GeoMod ${GeoMod_sources})
target_link_libraries(GeoMod PUBLIC SCOREC::core)
target_include_directories(GeoMod PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

bob_export_target(GeoMod)

add_executable(GeoMod_app main.cpp)
target_link_libraries(GeoMod_app PRIVATE GeoMod)

bob_export_target(GeoMod_app)

install(FILES ${GeoMod_headers} DESTINATION include)

bob_end_subdir()

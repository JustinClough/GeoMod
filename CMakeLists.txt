cmake_minimum_required(VERSION 3.0.0)

project(GeoMod VERSION 1.0.0 LANGUAGES CXX)

include(cmake/bob.cmake)

bob_begin_package()

bob_begin_cxx_flags()
bob_cxx11_flags()
bob_end_cxx_flags()

set(GeoMod_USE_SCOREC_DEFAULT ON)
bob_public_dep(SCOREC)

add_subdirectory(src)

bob_end_package()

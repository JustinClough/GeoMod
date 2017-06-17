cmake_minimum_required(VERSION 3.0.0)

project(GeoMod VERSION 1.0.0 LANGUAGES CXX)
set(PROJ_PREFIX /lore/clougj/GeoMod)

include_directories(${PROJ_PREFIX})
set(SOURCE_PREFIX ${PROJ_PREFIX}/src/)

# Allow the user to indicate where they installed SCOREC
# via "-DSCOREC_PREFIX=/home/somewhere" when calling `cmake`
set(SCOREC_PREFIX "" CACHE STRING "Directory where SCOREC is installed")

# If SCOREC_PREFIX was specified, only link to that directory,
# i.e. don't link to another installation in /usr/lib by mistake
if (SCOREC_PREFIX)
  find_package(SCOREC 2.1.0 REQUIRED CONFIG PATHS ${SCOREC_PREFIX} NO_DEFAULT_PATH)
else()
# IF SCOREC_PREFIX was not specified, look in typical system directories,
# and also in CMAKE_PREFIX_PATH (environment variable)
  find_package(
      SCOREC #package name, has to be SCOREC
      2.1.0  #version. can be omitted, and will match any installed version
             #greater than or equal to this one, as long as the major number
             #is the same
      REQUIRED #indicate that SCOREC is really needed to compile
      CONFIG   #skip the 'MODULE' search system, save some time and confusion
      )
endif()

add_executable(GeoMod 
${SOURCE_PREFIX}/GeoMod_coords.cpp
${SOURCE_PREFIX}/GeoMod_gmd_t.cpp
${SOURCE_PREFIX}/GeoMod_mesh_helper.cpp
${SOURCE_PREFIX}/GeoMod_model_helper.cpp
${SOURCE_PREFIX}/GeoMod_printer.cpp
${SOURCE_PREFIX}/GeoMod_util.cpp
${SOURCE_PREFIX}/GeoMod_Tests.cpp
${SOURCE_PREFIX}/main.cpp)
target_include_directories( GeoMod PRIVATE ${SOURCE_PREFIX})

#for any targets that use PUMI, just use this command
#to it to include the right directories and link to all
#the scorec libraries automatically.
#PUBLIC if the target is a library and
#PRIVATE if the target is an executable
target_link_libraries(GeoMod PRIVATE SCOREC::core)

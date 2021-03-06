# ======================================================================
#  art main build file
#
#  cd .../path/to/build/directory
#  source .../path/to/art/ups/setup_for_development
#  cmake [-DCMAKE_INSTALL_PREFIX=/install/path]
#        -DCMAKE_BUILD_TYPE=$CETPKG_TYPE
#        $CETPKG_SOURCE
#  make
#  make test
#  make install
#  make package (builds distribution tarfile)
# ======================================================================


# use cmake 2.8 or later
cmake_minimum_required (VERSION 2.8)

project(art)

# cetbuildtools contains our cmake modules
find_package(cetbuildtools REQUIRED)

list(APPEND CMAKE_MODULE_PATH
  $ENV{CANVAS_DIR}/Modules
  ${CMAKE_CURRENT_SOURCE_DIR}/Modules)

include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS VIGILANT
  WERROR
  NO_UNDEFINED
  EXTRA_FLAGS -pedantic
  )
cet_have_qual(sse2 SSE2)
if ( SSE2 )
  cet_add_compiler_flags(CXX -msse2 -ftree-vectorizer-verbose=2)
endif()

cet_report_compiler_flags()

# these are minimum required versions, not the actual product versions
find_ups_product( canvas )
find_ups_product( messagefacility )
find_ups_product( fhiclcpp )
find_ups_product( cetlib )
find_ups_product( tbb )
find_ups_product( clhep )
find_ups_root()
find_ups_boost()
find_ups_product( cppunit )
find_ups_product( sqlite )
find_ups_product (cetbuildtools)

# define CLHEP
cet_find_library( CLHEP  NAMES CLHEP   PATHS ENV CLHEP_LIB_DIR NO_DEFAULT_PATH )

# define cppunit
cet_find_library( CPPUNIT NAMES cppunit   PATHS ENV CPPUNIT_LIB   NO_DEFAULT_PATH )

# macros for art_dictionary and simple_plugin
include(ArtDictionary)
include(BuildPlugins)

if (NOT APPLE)
  set(RT -lrt)
endif()

# SQLite
cet_find_library(SQLITE3 NAMES sqlite3_ups PATHS ENV SQLITE_LIB NO_DEFAULT_PATH)

# Plugin skeleton generators for cetskelgen.
add_subdirectory(perllib)

# tools
add_subdirectory(tools)

# source
add_subdirectory (art)


# ups - table and config files
add_subdirectory(ups)

# CMake modules
add_subdirectory(Modules)

# packaging utility
include(UseCPack)

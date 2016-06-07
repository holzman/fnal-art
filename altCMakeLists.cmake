# - Art top level build
# Project setup
# Require 2.8.12 to:
# - get new interface export features
# - target ALIAS
cmake_minimum_required(VERSION 2.8.12)

# - Policies - set all here as may influence project() call
# Always id Clang as Clang, defer use of AppleClang id
if(POLICY CMP0025)
  cmake_policy(SET CMP0025 OLD)
endif()

# Always use rpath on Mac, as it's supported in out min version, and
# CMake 3 and higher prefer it
if(POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW)
endif()

project(art)

#-----------------------------------------------------------------------
# API and ABI versioning
# NB - this only demonstrates that it *can* be done
# More info on this at:
# - http://public.kitware.com/Bug/view.php?id=4383
# - http://techbase.kde.org/Policies/Binary_Compatibility_Issues_With_C++
#
# The following numbers are *arbitrary* for now. Remember that
# VERSION and SOVERSION do not neccessarily evolve in sync
# - Hard code version plus splits (derive one from t'other later)
set(art_VERSION "2.00.00")
set(art_VERSION_MAJOR 1)
set(art_VERSION_MINOR 13)
set(art_VERSION_PATCH 01)

set(art_SOVERSION "2.0.0")

# - We can also use a postfix to distinguish the debug lib from
# others if different build modes are ABI incompatible (can be
# extended to other modes)
set(art_DEBUG_POSTFIX "d")



#-----------------------------------------------------------------------
# Standard and Custom CMake Modules
#
#
# - Cetbuildtools, version2
find_package(cetbuildtools2 0.1.0 REQUIRED)
set(CMAKE_MODULE_PATH ${cetbuildtools2_MODULE_PATH})
include(CetInstallDirs)
message(STATUS "CMAKE_INSTALL_CMAKEDIR : x${CMAKE_INSTALL_FHICLDIR}")
include(CetCMakeSettings)
include(CetCompilerSettings)
include(FindThreads)
# C++ Standard Config
set(CMAKE_CXX_EXTENSIONS OFF)
set(canvas_COMPILE_FEATURES
  cxx_auto_type
  cxx_generic_lambdas
  )

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_LIST_DIR}/Modules)

# - Standard Support
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(CheckCXXCompilerFlag)

# - Local Customs
include(artInternalTools)
include(artTools)
include(ArtDictionary)

# - Build product locations
# The variables CMAKE_{RUNTIME,LIBRARY,ARCHIVE}_OUTPUT_DIRECTORY can
# be used to specify where executables, dynamic and static libraries
# are output. They initialize the {RUNTIME,LIBRARY,ARCHIVE}_OUTPUT_DIRECTORY
# properties of targets added via add_{executable,target}, so can
# targets can override if need be.
#
# - Assume for now that GNUInstallDirs provides relative (to
#   CMAKE_INSTALL_PREFIX) paths, and reflect this layout in the
#   output directories. This should be o.k. even on DLL platforms
#   as CMake should output these to the RUNTIME directory.
#
set(BASE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/BuildProducts")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIRECTORY}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIRECTORY}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIRECTORY}/${CMAKE_INSTALL_LIBDIR}")

# Implement SSE2 as option?
#cet_have_qual(sse2 SSE2)
#if ( SSE2 )
#  cet_add_compiler_flags(CXX -msse2 -ftree-vectorizer-verbose=2)
#endif()

# Use an imported target for checkClassVersion to give transparency
# between art build and client usage.
# This *is* a little messy, but eases the writing of the scripts for
# full portability between the art build and clients.
# We set it up *here* because we need to append the library output
# directory to checkClassVersion's search path.
#add_executable(art::checkClassVersion IMPORTED)
#set_target_properties(art::checkClassVersion PROPERTIES IMPORTED_LOCATION
#  ${CMAKE_CURRENT_SOURCE_DIR}/tools/checkClassVersion
#  )
#checkclassversion_append_path(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
#-----------------------------------------------------------------------
# Required Third Party Packages

set(canvas_MIN_BOOST_VERSION "1.60.0")

# - Ensure we can refind Boost and extra components we need
find_package(Boost ${canvas_MIN_BOOST_VERSION}
  REQUIRED
    date_time
    unit_test_framework
    program_options
    thread
  )

# CLHEP supplies a CMake project config...
find_package(CLHEP 2.3.2.2 REQUIRED)

# SQLite3 - NB messagefacility also depends on and exposes this...
find_package(SQLite 3.8.5 REQUIRED)

# Cross check with what ROOT supply - ideally would like component
# based checks.
find_package(ROOT 6.06.02 REQUIRED)
# - Must have Python support - it doesn't work as a COMPONENT
# argument to find_package because Root's component lookup only
# works for libraries.
if(NOT ROOT_python_FOUND)
  message(FATAL_ERROR "canvas requires ROOT with Python support")
endif()

find_package(cetlib 1.17.4 REQUIRED)
find_package(fhiclcpp 3.18.4 REQUIRED)
find_package(messagefacility 1.16.24 REQUIRED)

# Need to implement
find_package(TBB 4.3.0 REQUIRED)

find_package(canvas 1.0.0 REQUIRED)
list(APPEND CMAKE_MODULE_PATH
  ${canvas_DIR}/Modules)
#-----------------------------------------------------------------------
# Set up paths for all subbuilds
#
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})

# tools (first)
#add_subdirectory(tools)

# source
add_subdirectory(art)

# cmake modules

add_subdirectory(Modules)

#-----------------------------------------------------------------------
# Install support files - usage from install tree only...
#
configure_package_config_file(
  Modules/artConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/artConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/art-${art_VERSION}
  PATH_VARS
    CMAKE_INSTALL_INCLUDEDIR
    CMAKE_INSTALL_LIBDIR
    CMAKE_INSTALL_BINDIR
  )

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/artConfigVersion.cmake
  VERSION ${art_VERSION}
  COMPATIBILITY AnyNewerVersion
  )

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/artConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/artConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/art-${art_VERSION}
  COMPONENT Development
  )

install(EXPORT ArtLibraries
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/art-${art_VERSION}
  NAMESPACE art::
  COMPONENT Development
  )

#-----------------------------------------------------------------------
# Package for Source and Binary
#
#include(ArtCPack)

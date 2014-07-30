# - Art top level build
# Project setup
# Require 2.8.11 to get new interface export features
cmake_minimum_required(VERSION 2.8.11)
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
set(art_VERSION "1.0.0")
set(art_VERSION_MAJOR 1)
set(art_VERSION_MINOR 0)
set(art_VERSION_PATCH 0)

set(art_SOVERSION "1.0.0")

# - We can also use a postfix to distinguish the debug lib from
# others if different build modes are ABI incompatible (can be
# extended to other modes)
set(art_DEBUG_POSTFIX "d")

#-----------------------------------------------------------------------
# Standard and Custom CMake Modules
#
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_LIST_DIR}/Modules)

# - Standard Support
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(CheckCXXCompilerFlag)

# Implement this as option?

#cet_have_qual(sse2 SSE2)
#if ( SSE2 )
#  cet_add_compiler_flags(CXX -msse2 -ftree-vectorizer-verbose=2)
#endif()

#-----------------------------------------------------------------------
# Required Third Party
# - Core library(ies), which will supply us with Boost
# For now use FNALCore, easy switch out to multi modules later if needed
find_package(FNALCore 1.0.0 REQUIRED)

# TODO: FNALCore needs to tell us about the Boost it used.
# FNALCore provides an interface to the C++ standard, so use its flags
# Could also check for compiler/stdlib compatiblity.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FNALCore_CXX_FLAGS}")

# - Needs review and implementation where required (though should
# be off-the-shelf FindX for most)
# Other issues - could ROOT supply some of these, e.g. sqlite3
# gccxml, tbb?
# Need to implement

# Is supplied by CMake, but no version checking, so need to check
# Also, this is only used "behind the scenes" via ROOT's genreflex script
# never directly, so strictly speaking it's ROOT's responsibility
# to handle this for us.
find_package(GCCXML 0.9.0 REQUIRED)

# CLHEP supplies a CMake project config...
find_package(CLHEP 2.1.2.2 REQUIRED)

# Need to implement
# Could ROOT supply it (or just TSQLiteServer?)
find_package(SQLite3 3.7.8 REQUIRED)

# Cross check with what ROOT supply - ideally would like component
# based checks.
find_package(ROOT 5.34.00 REQUIRED
  Core
  Cint
  Cintex
  Reflex
  RIO
  Thread
  )

# Need to implement
find_package(TBB 4.1.0 REQUIRED)

#-----------------------------------------------------------------------
# Set up paths for all subbuilds
#
include_directories(${PROJECT_SOURCE_DIR})

# macros for art_dictionary and simple_plugin
#include(ArtDictionary)
#include(BuildPlugins)

# tools (first)
#add_subdirectory(tools)

# source
add_subdirectory(art)

# testing
option(ART_ENABLE_TESTING "Built unit tests for art" OFF)
if(ART_ENABLE_TESTING)
  # cppunit *only* needed for testing
  enable_testing()
  find_package(CppUnit 1.12.1 REQUIRED)
  #add_subdirectory(test)
endif()

# ups - table and config files
#add_subdirectory(ups)

# CMake modules
#add_subdirectory(Modules)

#-----------------------------------------------------------------------
# Documentation
#
option(ART_BUILD_DOCS "Build documentation for Art" ON)
if(ART_BUILD_DOCS)
  add_subdirectory(doc)
endif()

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
include(ArtCPack)


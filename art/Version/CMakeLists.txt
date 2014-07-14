# project name
#project(art_Version)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/GetReleaseVersion.cc.in
  ${CMAKE_CURRENT_BINARY_DIR}/GetReleaseVersion.cc
  @ONLY
  )

add_library(art_Version SHARED
  GetReleaseVersion.h
  ${CMAKE_CURRENT_BINARY_DIR}/GetReleaseVersion.cc
  )



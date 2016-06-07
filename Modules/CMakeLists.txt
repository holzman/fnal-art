if (ALT_CMAKE)
include(altCMakeLists.cmake)
else(ALT_CMAKE)

INSTALL(FILES
  ArtMake.cmake
  BuildPlugins.cmake
  DESTINATION ${product}/${version}/Modules
  )

endif(ALT_CMAKE)

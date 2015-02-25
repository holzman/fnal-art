if (ALT_CMAKE) 
include(altCMakeLists.cmake)
else()
INSTALL(FILES
  ArtDictionary.cmake
  ArtMake.cmake
  BuildPlugins.cmake
  CheckClassVersion.cmake
  DESTINATION ${product}/${version}/Modules
  )
endif()

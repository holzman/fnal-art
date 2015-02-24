if (PURE_CMAKE) 
include(altCMakeLists.txt)
else()
INSTALL(FILES
  ArtDictionary.cmake
  ArtMake.cmake
  BuildPlugins.cmake
  CheckClassVersion.cmake
  DESTINATION ${product}/${version}/Modules
  )
endif()

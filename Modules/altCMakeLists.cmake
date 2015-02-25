INSTALL(FILES
  ArtCPack.cmake
  ArtDictionary.cmake
  ArtMake.cmake
  BuildDictionary.cmake
  BuildPlugins.cmake
  CetRegexEscape.cmake
  CetTest.cmake
  CheckClassVersion.cmake
  FindCppUnit.cmake
  FindSQLite3.cmake
  FindTBB.cmake
  altArtDictionary.cmake
  altCMakeLists.cmake
  altCheckClassVersion.cmake
  artInternalTools.cmake
  artTools.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/art-${art_VERSION}
  COMPONENT Development
  )


# - Build art_Ntuple libray
# Define headers
set(art_Ntuple_HEADERS
  Ntuple.h
  sqlite_DBmanager.h
  sqlite_helpers.h
  sqlite_stringstream.h
  Transaction.h
  )

# Describe library
add_library(art_Ntuple SHARED
  ${art_Ntuple_HEADERS}
  sqlite_helpers.cc
  Transaction.cc
  )

# Describe library include interface
target_include_directories(art_Ntuple
  PUBLIC
   ${SQLite_INCLUDEDIR}
   ${cetlib_INCLUDEDIR}
   ${fhiclcpp_INCLUDEDIR}
   ${Boost_INCLUDE_DIRS}
   )

# Describe library link interface
target_link_libraries(art_Ntuple art_Utilities ${SQLite3_LIBRARIES})

# Set any additional properties
set_target_properties(art_Ntuple
  PROPERTIES
   VERSION ${art_VERSION}
   SOVERSION ${art_SOVERSION}
  )

# - Now the plugin
add_library(art_Ntuple_mfPlugin SHARED sqlite_mfPlugin.cc)
target_link_libraries(art_Ntuple_mfPlugin art_Ntuple cetlib::cetlib fhiclcpp::fhiclcpp)

install(TARGETS art_Ntuple art_Ntuple_mfPlugin
  EXPORT ArtLibraries
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Runtime
  )
install(FILES ${art_Ntuple_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/art/Ntuple
  COMPONENT Development
  )


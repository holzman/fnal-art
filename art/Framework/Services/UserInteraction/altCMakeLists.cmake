
# - Build art_Framework_Services_UserInteraction

# Describe library
add_library(art_Framework_Services_UserInteraction SHARED
  UserInteraction.h
  UserInteraction.cc
  )

# Describe library link interface - all Public for now
target_link_libraries(art_Framework_Services_UserInteraction
  art_Framework_Core
  art_Framework_Principal
  art_Persistency_Provenance
  art_Framework_Services_Registry
  cetlib::cetlib fhiclcpp::fhiclcpp
  )

# Set any additional properties
set_target_properties(art_Framework_Services_UserInteraction
  PROPERTIES
   VERSION ${art_VERSION}
   SOVERSION ${art_SOVERSION}
  )

install(TARGETS art_Framework_Services_UserInteraction
  EXPORT ArtLibraries
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Runtime
  )
install(FILES UserInteraction.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/art/Framework/UserInteraction
  COMPONENT Development
  )


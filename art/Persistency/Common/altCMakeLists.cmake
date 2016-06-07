# - Build art_Persistency_Common lib
# Define headers
set(art_Persistency_Common_HEADERS
CollectionUtilities.h
debugging_allocator.h
DelayedReader.h
fwd.h
GroupQueryResult.h
PostReadFixupTrait.h
  )


# Describe library
add_library(art_Persistency_Common SHARED
  ${art_Persistency_Common_HEADERS}
  DelayedReader.cc
  GroupQueryResult.cc
  )

# Describe library link interface
target_link_libraries(art_Persistency_Common
  PUBLIC
  cetlib::cetlib fhiclcpp::fhiclcpp
  art_Utilities
  art_Persistency_Provenance
  ${ROOT_Core_LIBRARY}
  )

# Set any additional properties
set_target_properties(art_Persistency_Common
  PROPERTIES
   VERSION ${art_VERSION}
   SOVERSION ${art_SOVERSION}
  )

install(TARGETS art_Persistency_Common
  EXPORT ArtLibraries
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Runtime
  )
install(FILES ${art_Persistency_Common_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/art/Persistency/Common
  COMPONENT Development
  )


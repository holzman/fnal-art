# - Build art_Persistency_Provenance lib
# Define headers
set(art_Persistency_Provenance_HEADERS
BranchIDListHelper.h
BranchIDListRegistry.h
EventProcessHistoryID.h
MasterProductRegistry.h
ProcessConfigurationRegistry.h
ProcessHistoryRegistry.h
ProductMetaData.h
Selections.h  )

# Describe library
add_library(art_Persistency_Provenance SHARED
  ${art_Persistency_Provenance_HEADERS}
BranchIDListHelper.cc
MasterProductRegistry.cc
ProductMetaData.cc
  )

# Describe library include interface
target_include_directories(art_Persistency_Provenance
  PUBLIC
   ${ROOT_INCLUDE_DIRS}
  PRIVATE
   ${BOOST_INCLUDE_DIRS}
   )

# Describe library link interface - all Public for now
target_link_libraries(art_Persistency_Provenance
  LINK_PUBLIC
   art_Persistency_RootDB
   art_Utilities
   canvas_Persistency_Provenance
   ${ROOT_Cintex_LIBRARY}
  LINK_PRIVATE
   ${BOOST_THREAD_LIBRARY}
  )

# Set any additional properties
set_target_properties(art_Persistency_Provenance
  PROPERTIES
   VERSION ${art_VERSION}
   SOVERSION ${art_SOVERSION}
  )

# - Dictify
art_dictionary(DICTIONARY_LIBRARIES art_Persistency_Provenance)


install(TARGETS art_Persistency_Provenance art_Persistency_Provenance_dict
  EXPORT ArtLibraries
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Runtime
  )
install(FILES ${art_Persistency_Provenance_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/art/Persistency/Provenance
  COMPONENT Development
  )



# - Build art_Framework_Modules modules

# - BlockingPrescaler
add_library(art_Framework_Modules_BlockingPrescaler_module SHARED
  BlockingPrescaler_module.cc
  )
target_link_libraries(art_Framework_Modules_BlockingPrescaler_module
  art_Framework_Core
  art_Framework_Principal
  art_Persistency_Common
  art_Persistency_Provenance
  art_Utilities
  )

# - EmptyEvent
add_library(art_Framework_Modules_EmptyEvent_source SHARED
  EmptyEvent_source.cc
  )
target_link_libraries(art_Framework_Modules_EmptyEvent_source
  art_Framework_Core
  art_Framework_Principal
  art_Persistency_Common
  art_Persistency_Provenance
  art_Utilities
  )

# - FileDumperOutput
add_library(art_Framework_Modules_FileDumperOutput_module SHARED
  FileDumperOutput_module.cc
  )
target_link_libraries(art_Framework_Modules_FileDumperOutput_module
  art_Framework_Core
  art_Framework_Principal
  art_Persistency_Common
  art_Persistency_Provenance
  art_Utilities
  )

# - Prescaler
add_library(art_Framework_Modules_Prescaler_module SHARED
  Prescaler_module.cc
  )
target_link_libraries(art_Framework_Modules_Prescaler_module
  art_Framework_Core
  art_Framework_Principal
  art_Persistency_Common
  art_Persistency_Provenance
  art_Utilities
  )

# - ProvenanceCheckerOutput
add_library(art_Framework_Modules_ProvenanceCheckerOutput_module SHARED
  ProvenanceCheckerOutput_module.cc
  )
target_link_libraries(art_Framework_Modules_ProvenanceCheckerOutput_module
  art_Framework_Core
  art_Framework_Principal
  art_Persistency_Common
  art_Persistency_Provenance
  art_Utilities
  )

# - RandomNumberSaver
add_library(art_Framework_Modules_RandomNumberSaver_module SHARED
  RandomNumberSaver_module.cc
  )
target_link_libraries(art_Framework_Modules_RandomNumberSaver_module
  art_Framework_Core
  art_Framework_Principal
  art_Persistency_Common
  art_Persistency_Provenance
  art_Utilities
  )

# - Set properties
set_target_properties(
  art_Framework_Modules_BlockingPrescaler_module
  art_Framework_Modules_EmptyEvent_source
  art_Framework_Modules_FileDumperOutput_module
  art_Framework_Modules_Prescaler_module
  art_Framework_Modules_ProvenanceCheckerOutput_module
  art_Framework_Modules_RandomNumberSaver_module
  PROPERTIES
   VERSION ${art_VERSION}
   SOVERSION ${art_SOVERSION}
  )

# - Dictify
include_directories(${canvas_INCLUDEDIR})
include_directories(${cetlib_INCLUDEDIR})
include_directories(${Boost_INCLUDE_DIR})
include_directories(${CLHEP_INCLUDE_DIRS})
art_dictionary(DICTIONARY_LIBRARIES art_Persistency_Common)

# - Install targets, files
install(TARGETS
  art_Framework_Modules_BlockingPrescaler_module
  art_Framework_Modules_EmptyEvent_source
  art_Framework_Modules_FileDumperOutput_module
  art_Framework_Modules_Prescaler_module
  art_Framework_Modules_ProvenanceCheckerOutput_module
  art_Framework_Modules_RandomNumberSaver_module
  art_Framework_Modules_dict
  EXPORT ArtLibraries
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Runtime
  )
install(FILES MixFilter.h ProvenanceDumper.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/art/Framework/Modules
  COMPONENT Development
  )


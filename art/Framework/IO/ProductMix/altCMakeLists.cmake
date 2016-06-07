include_directories(${canvas_INCLUDEDIR})

# - Build art_Framework_IO_ProductMix library

set(art_Framework_IO_ProductMix_HEADERS
  MixContainerTypes.h
  MixHelper.h
  MixOpBase.h
  MixOp.h
  ProdToProdMapBuilder.h
  )

# Define library
add_library(art_Framework_IO_ProductMix SHARED
  MixHelper.cc
  ProdToProdMapBuilder.cc
  )

# Describe library link interface - all Public for now
target_link_libraries(art_Framework_IO_ProductMix
  art_Framework_IO_Root
  art_Framework_Services_System_CurrentModule_service
  art_Framework_Services_System_TriggerNamesService_service
  art_Framework_Services_Optional_RandomNumberGenerator_service
  art_Framework_Core
  art_Framework_Principal
  art_Framework_Services_Registry
  art_Persistency_Common
  art_Persistency_Provenance
  art_Utilities
  canvas::canvas_Persistency_Provenance
  canvas::canvas_Utilities
  messagefacility::MF_MessageLogger
  fhiclcpp::fhiclcpp
  cetlib::cetlib
  )

# Set any additional properties
set_target_properties(art_Framework_IO_ProductMix
  PROPERTIES
   VERSION ${art_VERSION}
   SOVERSION ${art_SOVERSION}
  )

# - Dictify
#
include_directories(${canvas_INCLUDE_DIRS})
include_directories(${cetlib_INCLUDEDIR})
art_dictionary(DICTIONARY_LIBRARIES art_Persistency_Provenance canvas::canvas_Persistency_Provenance)

install(TARGETS art_Framework_IO_ProductMix art_Framework_IO_ProductMix_dict
  EXPORT ArtLibraries
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Runtime
  )
install(FILES ${art_Framework_IO_ProductMix_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/art/Framework/IO/ProductMix
  COMPONENT Development
  )



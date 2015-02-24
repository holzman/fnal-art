
# - Build art_Framework_Services_Optional lib and plugins

# - art_Framework_Services_Optional
add_library(art_Framework_Services_Optional SHARED
  TFileDirectory.h
  TFileDirectory.cc
  detail/TH1AddDirectorySentry.h
  detail/TH1AddDirectorySentry.cc
  )

target_link_libraries(art_Framework_Services_Optional
  FNALCore::FNALCore
  ${ROOT_Core_LIBRARY}
  ${ROOT_Hist_LIBRARY}
  )
target_include_directories(art_Framework_Services_Optional
  PUBLIC
   ${ROOT_INCLUDE_DIRS}
  )
set_target_properties(art_Framework_Services_Optional
  PROPERTIES
   VERSION ${art_VERSION}
   SOVERSION ${art_SOVERSION}
  )
install(TARGETS art_Framework_Services_Optional
  EXPORT ArtLibraries
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Runtime
  )
install(FILES TFileDirectory.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/art/Framework/Services/Optional
  COMPONENT Development
  )
install(FILES detail/TH1AddDirectorySentry.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/art/Framework/Services/Optional/detail
  COMPONENT Development
  )

# - RandomNumberGeneratorService
add_library(art_Framework_Services_Optional_RandomNumberGenerator_service
  SHARED
  RandomNumberGenerator.h
  RandomNumberGenerator_service.cc
  )
target_link_libraries(art_Framework_Services_Optional_RandomNumberGenerator_service
  FNALCore::FNALCore
  ${CLHEP_LIBRARIES}
  art_Framework_Principal
  )
set_target_properties(art_Framework_Services_Optional_RandomNumberGenerator_service
  PROPERTIES
   VERSION ${art_VERSION}
   SOVERSION ${art_SOVERSION}
  )
install(TARGETS art_Framework_Services_Optional_RandomNumberGenerator_service
  EXPORT ArtLibraries
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Runtime
  )
install(FILES RandomNumberGenerator.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/art/Framework/Services/Optional
  COMPONENT Development
  )

# - SimpleInteraction_service
add_library(art_Framework_Services_Optional_SimpleInteraction_service
  SHARED
  SimpleInteraction.h
  SimpleInteraction_service.cc
  )
target_link_libraries(art_Framework_Services_Optional_SimpleInteraction_service
  art_Framework_Services_UserInteraction
  )
set_target_properties(art_Framework_Services_Optional_SimpleInteraction_service
  PROPERTIES
   VERSION ${art_VERSION}
   SOVERSION ${art_SOVERSION}
  )
install(TARGETS art_Framework_Services_Optional_SimpleInteraction_service
  EXPORT ArtLibraries
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Runtime
  )
install(FILES SimpleInteraction.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/art/Framework/Services/Optional
  COMPONENT Development
  )

# - SimpleMemoryCheck_service
add_library(art_Framework_Services_Optional_SimpleMemoryCheck_service
  SHARED
  SimpleMemoryCheck_service.cc
  )
target_link_libraries(art_Framework_Services_Optional_SimpleMemoryCheck_service
  art_Persistency_Provenance
  )
set_target_properties(art_Framework_Services_Optional_SimpleMemoryCheck_service
  PROPERTIES
   VERSION ${art_VERSION}
   SOVERSION ${art_SOVERSION}
  )
install(TARGETS art_Framework_Services_Optional_SimpleMemoryCheck_service
  EXPORT ArtLibraries
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Runtime
  )

add_library(art_Framework_Services_Optional_TFileService_service
  SHARED
  TFileService.h
  TFileService_service.cc
  )
target_link_libraries(art_Framework_Services_Optional_TFileService_service
  art_Framework_Services_System_TriggerNamesService_service
  art_Framework_Services_Optional
  art_Framework_IO
  art_Framework_Principal
  ${ROOT_RIO_LIBRARY}
  ${ROOT_Thread_LIBRARY}
  )
set_target_properties(art_Framework_Services_Optional_TFileService_service
  PROPERTIES
   VERSION ${art_VERSION}
   SOVERSION ${art_SOVERSION}
  )
install(TARGETS art_Framework_Services_Optional_TFileService_service
  EXPORT ArtLibraries
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Runtime
  )
install(FILES TFileService.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/art/Framework/Services/Optional
  COMPONENT Development
  )



# - Timing_service
add_library(art_Framework_Services_Optional_Timing_service
  SHARED
  Timing_service.cc
  )
target_link_libraries(art_Framework_Services_Optional_Timing_service
  art_Persistency_Provenance
  )

target_include_directories(art_Framework_Services_Optional_Timing_service
  PRIVATE
   ${TBB_INCLUDE_DIRS}
  )


set_target_properties(art_Framework_Services_Optional_Timing_service
  PROPERTIES
   VERSION ${art_VERSION}
   SOVERSION ${art_SOVERSION}
  )
install(TARGETS art_Framework_Services_Optional_Timing_service
  EXPORT ArtLibraries
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Runtime
  )

# - Tracer_service
add_library(art_Framework_Services_Optional_Tracer_service
  SHARED
  Tracer_service.cc
  )
target_link_libraries(art_Framework_Services_Optional_Tracer_service
  art_Persistency_Provenance
  )
set_target_properties(art_Framework_Services_Optional_Tracer_service
  PROPERTIES
   VERSION ${art_VERSION}
   SOVERSION ${art_SOVERSION}
  )
install(TARGETS art_Framework_Services_Optional_Tracer_service
  EXPORT ArtLibraries
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Runtime
  )

# - TrivialFileDelivery_service
add_library(art_Framework_Services_Optional_TrivialFileDelivery_service
  SHARED
  TrivialFileDelivery.h
  TrivialFileDelivery_service.cc
  )
target_link_libraries(art_Framework_Services_Optional_TrivialFileDelivery_service
  art_Framework_Services_Registry
  )
set_target_properties(art_Framework_Services_Optional_TrivialFileDelivery_service
  PROPERTIES
   VERSION ${art_VERSION}
   SOVERSION ${art_SOVERSION}
  )
install(TARGETS art_Framework_Services_Optional_TrivialFileDelivery_service
  EXPORT ArtLibraries
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Runtime
  )
install(FILES TrivialFileDelivery.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/art/Framework/Services/Optional
  COMPONENT Development
  )

# - TrivialFileTransfer_service
add_library(art_Framework_Services_Optional_TrivialFileTransfer_service
  SHARED
  TrivialFileTransfer.h
  TrivialFileTransfer_service.cc
  )
target_link_libraries(art_Framework_Services_Optional_TrivialFileTransfer_service
  art_Framework_Services_Registry
  )
set_target_properties(art_Framework_Services_Optional_TrivialFileTransfer_service
  PROPERTIES
   VERSION ${art_VERSION}
   SOVERSION ${art_SOVERSION}
  )
install(TARGETS art_Framework_Services_Optional_TrivialFileTransfer_service
  EXPORT ArtLibraries
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Runtime
  )
install(FILES TrivialFileTransfer.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/art/Framework/Services/Optional
  COMPONENT Development
  )



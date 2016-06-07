# - Build art_Framework_Services_Optional lib and plugins
#-----------------------------------------------------------------------
# - art_Framework_Services_Optional
set(art_Framework_Services_Optional_HEADERS
MemoryTrackerDarwin.h
MemoryTracker.h
MemoryTrackerLinux.h
RandomNumberGenerator.h
SimpleInteraction.h
SimpleMemoryCheckDarwin.h
SimpleMemoryCheck.h
SimpleMemoryCheckLinux.h
TFileDirectory.h
TFileService.h
TimeTracker.h
TrivialFileDelivery.h
TrivialFileTransfer.h
  )
set(art_Framework_Services_Optional_DETAIL_HEADERS
detail/constrained_multimap.h
detail/LinuxMallInfo.h
detail/LinuxProcData.h
detail/LinuxProcMgr.h
detail/MemoryTrackerLinuxCallbackPair.h
detail/SimpleMemoryCheckConfig.h
detail/TH1AddDirectorySentry.h
  )

set(art_Framework_Services_Optional_SOURCES
  TFileDirectory.cc
  detail/TH1AddDirectorySentry.cc
  )

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  list(APPEND 
    art_Framework_Services_Optional_SOURCES
    MemoryTrackerLinux.cc
    SimpleMemoryCheckLinux.cc
    detail/LinuxProcMgr.cc
    )
endif()


add_library(art_Framework_Services_Optional SHARED
  ${art_Framework_Services_Optional_SOURCES}
  )

target_link_libraries(art_Framework_Services_Optional
  cetlib::cetlib
  ${ROOT_Core_LIBRARY}
  ${ROOT_Hist_LIBRARY}
  art_Framework_Services_Registry
  art_Persistency_Provenance
  canvas::canvas_Persistency_Provenance
  art_Utilities
  canvas::canvas_Utilities
  art_Ntuple
  )

target_include_directories(art_Framework_Services_Optional
  PUBLIC
   ${ROOT_INCLUDE_DIRS}
  )

art_set_standard_target_properties(art_Framework_Services_Optional)

install(TARGETS art_Framework_Services_Optional ${art_TARGET_INSTALL_ARGS})

install(FILES ${art_Framework_Services_Optional_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/art/Framework/Services/Optional
  COMPONENT Development
  )
install(FILES ${art_Framework_Services_Optional_DETAIL_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/art/Framework/Services/Optional/detail
  COMPONENT Development
  )

#-----------------------------------------------------------------------
# SERVICE PLUGINS
#-----------------------------------------------------------------------
# - RandomNumberGeneratorService
art_add_service(art_Framework_Services_Optional_RandomNumberGenerator_service
  RandomNumberGenerator_service.cc
  )

target_link_libraries(art_Framework_Services_Optional_RandomNumberGenerator_service
  cetlib::cetlib
  messagefacility::MF_MessageLogger
  messagefacility::MF_Utilities
  ${CLHEP_LIBRARIES}
  art_Framework_Principal
  art_Persistency_Common
  canvas::canvas_Persistency_Provenance
  )

art_set_standard_target_properties(art_Framework_Services_Optional_RandomNumberGenerator_service)

install(TARGETS art_Framework_Services_Optional_RandomNumberGenerator_service ${art_TARGET_INSTALL_ARGS})

install(FILES RandomNumberGenerator.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/art/Framework/Services/Optional
  COMPONENT Development
  )

#-----------------------------------------------------------------------
# - SimpleInteraction_service
art_add_service(art_Framework_Services_Optional_SimpleInteraction_service
  SimpleInteraction_service.cc
  )

target_link_libraries(art_Framework_Services_Optional_SimpleInteraction_service
  art_Framework_Services_UserInteraction
  )

art_set_standard_target_properties(art_Framework_Services_Optional_SimpleInteraction_service)

install(TARGETS art_Framework_Services_Optional_SimpleInteraction_service
  ${art_TARGET_INSTALL_ARGS}
  )

#-----------------------------------------------------------------------
# - SimpleMemoryCheck_service
art_add_service(art_Framework_Services_Optional_SimpleMemoryCheck_service
  SimpleMemoryCheck_service.cc
  )

target_link_libraries(art_Framework_Services_Optional_SimpleMemoryCheck_service
  art_Framework_Services_Optional
  art_Persistency_Provenance
  canvas::canvas_Persistency_Provenance
  )

art_set_standard_target_properties(art_Framework_Services_Optional_SimpleMemoryCheck_service)

install(TARGETS art_Framework_Services_Optional_SimpleMemoryCheck_service
  ${art_TARGET_INSTALL_ARGS}
  )

#-----------------------------------------------------------------------
# MemoryAdjuster service
#
art_add_service(art_Framework_Services_Optional_MemoryAdjuster_service
  MemoryAdjuster_service.cc
  )

target_link_libraries(art_Framework_Services_Optional_MemoryAdjuster_service
  art_Framework_Services_Optional
  )

art_set_standard_target_properties(art_Framework_Services_Optional_MemoryAdjuster_service)

install(TARGETS art_Framework_Services_Optional_MemoryAdjuster_service
  ${art_TARGET_INSTALL_ARGS}
  )

#-----------------------------------------------------------------------
# MemoryTracker service
#
art_add_service(art_Framework_Services_Optional_MemoryTracker_service
  MemoryTracker_service.cc
  )

target_link_libraries(art_Framework_Services_Optional_MemoryTracker_service
  art_Framework_Services_Optional
  art_Ntuple
  art_Persistency_Provenance 
  canvas_Persistency_Common
  )

art_set_standard_target_properties(art_Framework_Services_Optional_MemoryTracker_service)

install(TARGETS art_Framework_Services_Optional_MemoryTracker_service
  ${art_TARGET_INSTALL_ARGS}
  )

#-----------------------------------------------------------------------
# TFileService service
#
art_add_service(art_Framework_Services_Optional_TFileService_service
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

art_set_standard_target_properties(art_Framework_Services_Optional_TFileService_service)

install(TARGETS art_Framework_Services_Optional_TFileService_service
  ${art_TARGET_INSTALL_ARGS}
  )

#-----------------------------------------------------------------------
# - Timing_service
#
art_add_service(art_Framework_Services_Optional_Timing_service
  Timing_service.cc
  )
target_link_libraries(art_Framework_Services_Optional_Timing_service
  art_Persistency_Provenance 
  canvas_Persistency_Provenance
  )

target_include_directories(art_Framework_Services_Optional_Timing_service
  PRIVATE
   ${TBB_INCLUDE_DIRS}
  )


art_set_standard_target_properties(art_Framework_Services_Optional_Timing_service)

install(TARGETS art_Framework_Services_Optional_Timing_service
  ${art_TARGET_INSTALL_ARGS}
  )

#-----------------------------------------------------------------------
# - TimeTracker_service
art_add_service(art_Framework_Services_Optional_TimeTracker_service
  TimeTracker_service.cc
  )
target_link_libraries(art_Framework_Services_Optional_TimeTracker_service
  art_Ntuple
  art_Persistency_Provenance
  canvas_Persistency_Provenance
  ${TBB_LIBRARIES}
  )

target_include_directories(art_Framework_Services_Optional_TimeTracker_service
  PRIVATE
   ${TBB_INCLUDE_DIRS}
  )


art_set_standard_target_properties(art_Framework_Services_Optional_TimeTracker_service)

install(TARGETS art_Framework_Services_Optional_TimeTracker_service
  ${art_TARGET_INSTALL_ARGS}
  )

#-----------------------------------------------------------------------
# - Tracer_service
art_add_service(art_Framework_Services_Optional_Tracer_service
  Tracer_service.cc
  )
target_link_libraries(art_Framework_Services_Optional_Tracer_service
  art_Persistency_Provenance
  canvas_Persistency_Provenance
  )

art_set_standard_target_properties(art_Framework_Services_Optional_Tracer_service)

install(TARGETS art_Framework_Services_Optional_Tracer_service
  ${art_TARGET_INSTALL_ARGS}
  )

#-----------------------------------------------------------------------
# - TrivialFileDelivery_service
art_add_service(art_Framework_Services_Optional_TrivialFileDelivery_service
  TrivialFileDelivery_service.cc
  )

art_set_standard_target_properties(art_Framework_Services_Optional_TrivialFileDelivery_service)

install(TARGETS art_Framework_Services_Optional_TrivialFileDelivery_service
  ${art_TARGET_INSTALL_ARGS}
  )

#-----------------------------------------------------------------------
# - TrivialFileTransfer_service
art_add_service(art_Framework_Services_Optional_TrivialFileTransfer_service
  TrivialFileTransfer_service.cc
  )

art_set_standard_target_properties(art_Framework_Services_Optional_TrivialFileTransfer_service)

install(TARGETS art_Framework_Services_Optional_TrivialFileTransfer_service
  ${art_TARGET_INSTALL_ARGS}
  )


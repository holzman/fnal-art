
# - Build art_Framework_Principal lib
# Define headers
set(art_Framework_Principal_HEADERS
ActionCodes.h
Actions.h
AssnsGroup.h
CurrentProcessingContext.h
DataViewImpl.h
DeferredProductGetter.h
Event.h
EventPrincipal.h
fwd.h
GroupFactory.h
Group.h
Handle.h
NoDelayedReader.h
OccurrenceTraits.h
OutputHandle.h
Principal.h
Provenance.h
Results.h
ResultsPrincipal.h
RPParams.h
RPWorker.h
Run.h
RunPrincipal.h
RunStopwatch.h
SelectorBase.h
Selector.h
SubRun.h
SubRunPrincipal.h
View.h
Worker.h
WorkerParams.h  )

set(art_Framework_Principal_DETAIL_HEADERS
  detail/maybe_record_parents.h
  )

# Describe library
add_library(art_Framework_Principal SHARED
Actions.cc
AssnsGroup.cc
CurrentProcessingContext.cc
DataViewImpl.cc
DeferredProductGetter.cc
Event.cc
EventPrincipal.cc
Group.cc
GroupFactory.cc
NoDelayedReader.cc
Principal.cc
Provenance.cc
Results.cc
ResultsPrincipal.cc
Run.cc
RunPrincipal.cc
SelectorBase.cc
Selector.cc
SubRun.cc
SubRunPrincipal.cc
Worker.cc
  )

# Describe library include interface
target_include_directories(art_Framework_Principal
  PUBLIC
   ${CLHEP_INCLUDE_DIRS}
  )

# Describe library link interface - all Public for now
target_link_libraries(art_Framework_Principal
  art_Version
  art_Utilities
  art_Persistency_Common
  art_Framework_Services_Registry
  ${CLHEP_LIBRARIES}
  )

# Set any additional properties
set_target_properties(art_Framework_Principal
  PROPERTIES
   VERSION ${art_VERSION}
   SOVERSION ${art_SOVERSION}
  )

install(TARGETS art_Framework_Principal
  EXPORT ArtLibraries
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Runtime
  )
install(FILES ${art_Framework_Principal_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/art/Framework/Principal
  COMPONENT Development
  )
install(FILES ${art_Framework_Principal_DETAIL_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/art/Framework/Principal/detail
  COMPONENT Development
  )


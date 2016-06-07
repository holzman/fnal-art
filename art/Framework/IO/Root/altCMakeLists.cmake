# - Build art_Framework_IO_Root{Version} plus modules/apps

# - art_Framework_IO_RootVersion
add_library(art_Framework_IO_RootVersion SHARED
  GetFileFormatEra.cc
  GetFileFormatVersion.cc
  )

# Set any additional properties
set_target_properties(art_Framework_IO_RootVersion
  PROPERTIES
   VERSION ${art_VERSION}
   SOVERSION ${art_SOVERSION}
  )

install(TARGETS art_Framework_IO_RootVersion
  EXPORT ArtLibraries
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Runtime
  )

# - art_Framework_IO_Root
set(art_Framework_IO_Root_HEADERS
BranchMapperWithReader.h
DropMetaData.h
DuplicateChecker.h
FastCloningInfoProvider.h
Inputfwd.h
RootBranchInfo.h
RootBranchInfoList.h
RootDelayedReader.h
rootErrMsgs.h
RootInputFile.h
RootInputFileSequence.h
RootInput.h
RootOutputFile.h
RootOutputTree.h
RootSizeOnDisk.h
RootTree.h
setFileIndexPointer.h
  )

add_library(art_Framework_IO_Root SHARED
DuplicateChecker.cc
FastCloningInfoProvider.cc
RootBranchInfo.cc
RootBranchInfoList.cc
RootDelayedReader.cc
RootInputFile.cc
RootInputFileSequence.cc
RootInput_source.cc
RootOutputFile.cc
RootOutput_module.cc
RootOutputTree.cc
RootSizeOnDisk.cc
RootTree.cc
  )

# Describe library link interface - all Public for now
target_link_libraries(art_Framework_IO_Root
  art_Framework_Core
  art_Framework_IO
  art_Framework_IO_Catalog
  art_Framework_Principal
  art_Framework_Services_Registry
  art_Persistency_Common
  art_Persistency_Provenance
  art_Framework_IO_RootVersion
  ${ROOT_Tree_LIBRARY}
  ${ROOT_Net_LIBRARY}
  ${ROOT_MatchCore_LIBRARY}
  )

# Set any additional properties
set_target_properties(art_Framework_IO_Root
  PROPERTIES
   VERSION ${art_VERSION}
   SOVERSION ${art_SOVERSION}
  )

install(TARGETS art_Framework_IO_Root
  EXPORT ArtLibraries
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Runtime
  )
install(FILES ${art_Framework_IO_Root_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/art/Framework/IO/Root
  COMPONENT Development
  )

# - art_Framework_IO_RootInput_source
add_library(art_Framework_IO_RootInput_source SHARED
  RootInput_source.cc
  )
target_link_libraries(art_Framework_IO_RootInput_source
  art_Framework_IO_Root
  art_Framework_IO_Catalog
  )
# Set any additional properties
set_target_properties(art_Framework_IO_RootInput_source
  PROPERTIES
   VERSION ${art_VERSION}
   SOVERSION ${art_SOVERSION}
  )

install(TARGETS art_Framework_IO_RootInput_source
  EXPORT ArtLibraries
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Runtime
  )

# art_Framework_IO_RootOutput_module
add_library(art_Framework_IO_RootOutput_module SHARED
  RootOutput_module.cc
  )
target_link_libraries(art_Framework_IO_RootOutput_module
  art_Framework_IO_Root
  )
# Set any additional properties
set_target_properties(art_Framework_IO_RootOutput_module
  PROPERTIES
   VERSION ${art_VERSION}
   SOVERSION ${art_SOVERSION}
  )

install(TARGETS art_Framework_IO_RootOutput_module
  EXPORT ArtLibraries
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Runtime
  )

# config_dumper
add_executable(config_dumper config_dumper.cc)
target_link_libraries(config_dumper
  art_Framework_IO_Root
  art_Utilities canvas::canvas_Utilities
  art_Framework_Core
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  ${ROOT_Tree_LIBRARY}
  ${ROOT_RIO_LIBRARY}
  )
install(TARGETS config_dumper
  EXPORT ArtLibraries
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Runtime
  )

# sam_metadata_dumper
add_executable(sam_metadata_dumper sam_metadata_dumper.cc)
target_link_libraries(sam_metadata_dumper
  art_Framework_IO_Root
  art_Utilities canvas::canvas_Utilities
  art_Framework_Core
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  ${ROOT_RIO_LIBRARY}
  )
install(TARGETS sam_metadata_dumper
  EXPORT ArtLibraries
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Runtime
  )

# product_sizes_dumper
add_executable(product_sizes_dumper product_sizes_dumper.cc)
target_link_libraries(product_sizes_dumper
  art_Framework_IO_Root
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  cetlib::cetlib
  )
install(TARGETS product_sizes_dumper
  EXPORT ArtLibraries
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Runtime
  )

# count_events
add_executable(count_events count_events.cc)
target_link_libraries(count_events
  art_Framework_IO_Root
  art_Utilities canvas::canvas_Utilities
  art_Framework_Core
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  ${ROOT_RIO_LIBRARY}
  ${ROOT_TREE_LIBRARY}
  )
install(TARGETS count_events
  EXPORT ArtLibraries
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Runtime
  )

